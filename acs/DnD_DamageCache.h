
#ifndef DND_DAMAGECACHE_IN
#define DND_DAMAGECACHE_IN

#include "DnD_DamageCacheDefs.h"
#include "DnD_Common.h"
#include "DnD_WeaponDefs.h"

// How many distinct weapons one player holds cached damage for at once. This table used to be
// dimensioned by MAXWEPS -- every weapon in the game, for every player -- when a realistic loadout
// is a handful. Everything in here is derived state, so a weapon that loses its slot just
// recomputes the next time it fires.
#define MAX_CACHED_WEAPONS 16
#define DND_WEPCACHE_NOSLOT -1

// the damage_cache holds base damage values and their rng ranges
// end_factor is (the factor we multiply our base value with using all damage increases) * (the constant to keep precision, which is 100k)
//
// Everything below is indexed by SLOT, not by wepid. Resolve once per shot and pass the slot down;
// resolving per accessor would repeat the lookup eight times on the hot path.
typedef struct pdmg_cache {
	bool massRecalculationRequested;

	// Both maps are stored +1 so the zero-initialized state already reads as "empty" -- weapon id 0
	// and slot 0 are both perfectly valid values, so 0 cannot double as the sentinel.
	int slot_of_wep[MAXWEPS];														// wepid -> slot + 1, 0 == not cached
	int wep_of_slot[MAX_CACHED_WEAPONS];											// slot -> wepid + 1, 0 == free
	int next_evict;																	// clock hand, used only once every slot is taken

	bool norecalculate[MAX_CACHED_WEAPONS][MAX_CACHE_ELEMENTS]; 					// false => needs recalculation otherwise no
	pdmg_T damage_cache[MAX_CACHED_WEAPONS][MAX_CACHE_ELEMENTS];
	int flat_values[MAX_CACHED_WEAPONS][MAX_CACHE_ELEMENTS];						// holds flat dmg bonuses
	int flat_factor[MAX_CACHED_WEAPONS][MAX_CACHE_ELEMENTS];						// holds added flat damage bonus efficiency

	// These two are deliberately kept apart rather than pre-multiplied into one
	// factor: the buff layer's own "increased" has to rejoin the SAME additive pool
	// at request time, and once the two are collapsed into a single number the buff
	// term can never be taken back out again.
	int inc_sum[MAX_CACHED_WEAPONS][MAX_CACHE_ELEMENTS];							// integer percent DELTA, baseline 0
	int more_packed[MAX_CACHED_WEAPONS][MAX_CACHE_ELEMENTS];						// packed normalized multiplier; 0 == hard zero

	// The two pools above hold only what is true of this weapon NO MATTER WHICH TYPE it deals --
	// weapon class bonuses, enchants, weapon mods. Everything keyed on the damage category lives
	// here instead, one pool per category, so a component of a different element can take the
	// weapon's increases without also taking "increased physical damage" and friends.
	// Filled for every category in one pass at cache time; see ScaleCachedDamage.
	int inc_typed[MAX_CACHED_WEAPONS][MAX_CACHE_ELEMENTS][MAX_DAMAGE_CATEGORIES];
	int more_typed[MAX_CACHED_WEAPONS][MAX_CACHE_ELEMENTS][MAX_DAMAGE_CATEGORIES];

	// ---- damage conversion ---------------------------------------------------------------------
	// Player scope. conv_raw is ACCUMULATED as items are equipped and unequipped, exactly the way
	// PlayerModData is -- it is not derived from anything and must survive every cache wipe. Each
	// conversion mod names its own (source, destination) pair, so a single summed attribute value
	// could not tell two of them apart; this table is what replaces it.
	//
	// conv_pct is the usable form, re-derived from conv_raw on every cached block: rows clamped so
	// each sums to conv_row_total, never above 100. The two are kept apart because the clamp is
	// destructive -- normalizing in place would eat the accumulation, and the next item equipped
	// would compound the loss.
	//
	// The DND_DAMAGECATEGORY order IS the conversion ladder: a category only ever converts into one
	// that sits later, which is what makes the walk below a single ascending pass.
	int conv_raw[MAX_DAMAGE_CATEGORIES][MAX_DAMAGE_CATEGORIES];						// [source][destination], accumulated percent
	int conv_pct[MAX_DAMAGE_CATEGORIES][MAX_DAMAGE_CATEGORIES];						// [source][destination], clamped percent
	int conv_row_total[MAX_DAMAGE_CATEGORIES];										// row sum, 0 == converts into nothing

	// "10% of physical damage added as extra cold". Same tables, same ladder, same paths -- the one
	// difference is that gain does NOT take its share away from the source, which is the entire
	// meaning of "extra". It is therefore NOT clamped either: two mods gaining 60% each should give
	// 120%, not 100%, and there is no source damage to run out of.
	int gain_raw[MAX_DAMAGE_CATEGORIES][MAX_DAMAGE_CATEGORIES];
	int gain_pct[MAX_DAMAGE_CATEGORIES][MAX_DAMAGE_CATEGORIES];
	int gain_row_total[MAX_DAMAGE_CATEGORIES];										// only a gate for the walk, never a cap

	// Per (slot, dmgid, destination) result of the ladder walk. Two numbers per destination is all
	// it takes: amt is the amount with every "more" on its conversion path already folded in, and
	// inc is the amount-weighted sum of the "increased" on that path. Both are LINEAR in the
	// contributions, so these two accumulators reproduce exactly what walking each conversion path
	// separately would give -- including two different chains landing on the same destination, which
	// a single merged path mask would over-reward.
	//
	// Two trees because the amounts are of different kinds. The weapon's roll is not known until the
	// shot, so its side is a FRACTION of the base (DND_CONVFRAC_ONE == 1.0); added flat damage is a
	// known number and is stored absolute, in the same fixed point. They also take different flat
	// effectiveness treatment at request time, so they could not share storage anyway.
	int conv_wfrac[MAX_CACHED_WEAPONS][MAX_CACHE_ELEMENTS][MAX_DAMAGE_CATEGORIES];
	int conv_winc[MAX_CACHED_WEAPONS][MAX_CACHE_ELEMENTS][MAX_DAMAGE_CATEGORIES];
	int conv_aamt[MAX_CACHED_WEAPONS][MAX_CACHE_ELEMENTS][MAX_DAMAGE_CATEGORIES];
	int conv_ainc[MAX_CACHED_WEAPONS][MAX_CACHE_ELEMENTS][MAX_DAMAGE_CATEGORIES];

	// One bit per destination category with anything to deal. Per (slot, dmgid) rather than per
	// player: both conversion and the physical category depend on the weapon.
	int comp_mask[MAX_CACHED_WEAPONS][MAX_CACHE_ELEMENTS];

	// Player-scope mirror of the above, so the accumulator can ask "does this player deal more than
	// one damage type at all" without a slot in hand. Whatever the last cached block resolved, which
	// is the same scope the old added_mask had.
	int extra_mask;

	// The ladder walk runs in here and is copied out, so one implementation serves both trees.
	int conv_walk_amt[MAX_DAMAGE_CATEGORIES];
	int conv_walk_inc[MAX_DAMAGE_CATEGORIES];

	// ---- the current hit's component split -------------------------------------------------------
	// Staged by ScaleCachedDamage, where the rolled base still exists, and consumed at the emit point.
	// Only ever used as RATIOS against comp_stage_total: everything between the two points scales the
	// primary by type-agnostic factors that the components must take as well, so the split has to land
	// on the final number. Using ratios is also what makes this safe against a re-entrant hit -- the
	// key rejects another weapon outright, and the same weapon has the same ratios by construction.
	int comp_stage[MAX_DAMAGE_CATEGORIES];
	int comp_stage_total;															// primary + every component
	int comp_stage_primary;															// the primary's own share, 0 at full conversion
	int comp_stage_mask;
	int comp_stage_key;

	// A split parked for a hit that lands LATER. Gravdis takes its damage number when it grabs the
	// monster and does not deal it until the monster comes back down several seconds afterwards, by
	// which time the live stage belongs to whatever the player has shot since. Only weapons that
	// hold a number across a delay need this -- everything else stages and consumes in one
	// uninterrupted stretch, which is what makes the live stage safe for them.
	int defer_stage[MAX_DAMAGE_CATEGORIES];
	int defer_total;
	int defer_primary;
	int defer_mask;
	int defer_key;

	int shotgun_count;																// last CountShotgunWeaponsOwned() seen, to detect changes
} pdmg_cache_T;

// Fixed point for the conversion walk. The "increased" pools are never multiplied into these -- the
// weighted mean is folded with MulPercent_Exact instead -- so the only headroom needed is
// amount x more.
//
// FOURTEEN bits, not sixteen. This value is used as a DIVISOR when the fraction is folded into a
// component's factor, and the MulPercent split only holds while div * div fits in an int, i.e.
// div <= 46340. At 65536 that quietly broke: the (v % div) * (pct % div) term could overflow and
// hand back a wrong number. 16384 is the largest power of two under the limit, so this gives up as
// little of the walk's accuracy as the constraint allows -- dropping to 12 doubled the worst-case
// error on chained conversions, 14 keeps it where 16 had it.
#define DND_CONVFRAC_BITS 14
#define DND_CONVFRAC_ONE (1 << DND_CONVFRAC_BITS)
#define DND_COMPSTAGE_NOKEY -1
#define DND_CONV_NOSKIP -1

// A component's factor carries two extra decimal digits before the fraction of the base is folded
// into it. Without them the factor floors to a whole percent, and with the base split several ways
// that costs about 1% per destination -- a full conversion across three elements was losing 3.6% of
// the hit. Costs nothing: the same two multiplies, a bigger divisor.
#define DND_COMPFACTOR_SCALE 100

pdmg_cache_T module& GetPlayerDamageCache(int pnum) {
	static pdmg_cache_T cache[MAXPLAYERS];
	return cache[pnum];
}

// wipes one slot's derived state -- shared by invalidation and by eviction, so the field list
// cannot drift between them
void ClearWeaponCacheSlot(pdmg_cache_T module& cache, int slot) {
	for(int j = 0; j < MAX_CACHE_ELEMENTS; ++j) {
		cache.norecalculate[slot][j] = false;
		cache.inc_sum[slot][j] = 0;
		cache.more_packed[slot][j] = DND_PACKED_MULT_IDENTITY;
		cache.flat_values[slot][j] = 0;
		cache.comp_mask[slot][j] = 0;

		for(int c = 0; c < MAX_DAMAGE_CATEGORIES; ++c) {
			cache.inc_typed[slot][j][c] = 0;
			cache.more_typed[slot][j][c] = DND_PACKED_MULT_IDENTITY;
			cache.conv_wfrac[slot][j][c] = 0;
			cache.conv_winc[slot][j][c] = 0;
			cache.conv_aamt[slot][j][c] = 0;
			cache.conv_ainc[slot][j][c] = 0;
		}
	}
}

// Slot of an already cached weapon, or DND_WEPCACHE_NOSLOT. Never allocates: invalidation has to go
// through this, because forcing a recalculation on a weapon nobody has fired must not spend a slot.
int LookupWeaponCacheSlot(int pnum, int wepid) {
	return GetPlayerDamageCache(pnum).slot_of_wep[wepid] - 1;
}

// Slot of a weapon, taking one on first use.
int ResolveWeaponCacheSlot(int pnum, int wepid) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);

	int slot = cache.slot_of_wep[wepid] - 1;
	if(slot != DND_WEPCACHE_NOSLOT)
		return slot;

	// a free slot if there is one
	for(int i = 0; i < MAX_CACHED_WEAPONS; ++i) {
		if(!cache.wep_of_slot[i]) {
			slot = i;
			break;
		}
	}

	// otherwise evict on a clock hand -- O(1), and reaching here at all needs a player cycling more
	// than MAX_CACHED_WEAPONS weapons. The loop above guarantees every slot is occupied here, so
	// wep_of_slot[slot] is non-zero and the back-reference below is in range.
	if(slot == DND_WEPCACHE_NOSLOT) {
		slot = cache.next_evict;
		cache.next_evict = (slot + 1) % MAX_CACHED_WEAPONS;
		cache.slot_of_wep[cache.wep_of_slot[slot] - 1] = 0;
	}

	cache.slot_of_wep[wepid] = slot + 1;
	cache.wep_of_slot[slot] = wepid + 1;
	ClearWeaponCacheSlot(cache, slot);

	return slot;
}

bool PlayerDamageNeedsCaching(int pnum, int slot, int dmgid) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	return !cache.norecalculate[slot][dmgid];
}

void ClearCache(int pnum, int slot, int dmgid) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.flat_values[slot][dmgid] = 0;
	cache.inc_sum[slot][dmgid] = 0;
	cache.more_packed[slot][dmgid] = DND_PACKED_MULT_IDENTITY;

	cache.comp_mask[slot][dmgid] = 0;

	for(int c = 0; c < MAX_DAMAGE_CATEGORIES; ++c) {
		cache.inc_typed[slot][dmgid][c] = 0;
		cache.more_typed[slot][dmgid][c] = DND_PACKED_MULT_IDENTITY;
		cache.conv_wfrac[slot][dmgid][c] = 0;
		cache.conv_winc[slot][dmgid][c] = 0;
		cache.conv_aamt[slot][dmgid][c] = 0;
		cache.conv_ainc[slot][dmgid][c] = 0;
	}
}

// this guy gets called last, so we mark recalc stuff here
void MarkCachingComplete(int pnum, int slot, int dmgid) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.norecalculate[slot][dmgid] = true;
	// clean the previously issued mass recalc request, since player fired now they might want to craft afterwards
	cache.massRecalculationRequested = false;
}

// Takes a wepid rather than a slot: this is the weapon raise path, and it is the moment a weapon
// earns its slot in the first place.
void CachePlayerDamage(int pnum, int dmg, int wepid, int dmgid, int dmg_rand, int flat_factor = 100) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	int slot = ResolveWeaponCacheSlot(pnum, wepid);

	cache.damage_cache[slot][dmgid].dmg = dmg;
	cache.damage_cache[slot][dmgid].dmg_low = dmg_rand & 0xFFFF;
	cache.damage_cache[slot][dmgid].dmg_high = dmg_rand >> 16;
	cache.norecalculate[slot][dmgid] = false;
	cache.flat_factor[slot][dmgid] = flat_factor;
}

void CachePlayerFlatDamage(int pnum, int dmg, int slot, int dmgid) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	// not cached
	if(!cache.norecalculate[slot][dmgid]) {
		dmg = dmg * cache.flat_factor[slot][dmgid] / 100;
		cache.flat_values[slot][dmgid] = dmg;
	}
}

// "increased" sums into its own pool; "more" folds into the packed product.
// factor for the multiplicative path is an integer percent that ALREADY carries the
// 100 baseline (e.g. 150 for "+50% more").
void InsertCacheFactor(int pnum, int slot, int dmgid, int factor, bool isAdditive) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);

	if(isAdditive) {
		cache.inc_sum[slot][dmgid] += factor;
		return;
	}

	// FixedDiv, not (factor << 16) / 100 -- the shift overflows past factor 32767
	cache.more_packed[slot][dmgid] = CombinePackedMultiplier(cache.more_packed[slot][dmgid], FixedDiv(factor, 100));
}

// used for multiplicative item mods that are by default fixed point.
// factor is an ABSOLUTE 16.16 multiplier (1.0 == no change), which is what both
// live callers already produce.
void InsertCacheFactor_Fixed(int pnum, int slot, int dmgid, int factor) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.more_packed[slot][dmgid] = CombinePackedMultiplier(cache.more_packed[slot][dmgid], factor);
}

int GetCachedPlayerDamage(int pnum, int slot, int dmgid) {
	return GetPlayerDamageCache(pnum).damage_cache[slot][dmgid].dmg;
}

int GetCachedPlayerFlatDamage(int pnum, int slot, int dmgid) {
	return GetPlayerDamageCache(pnum).flat_values[slot][dmgid];
}

// integer percent DELTA -- the caller adds the 100 baseline, and the buff layer's
// own "increased" joins this same pool before the more-product is applied
int GetCachedPlayerIncreased(int pnum, int slot, int dmgid) {
	return GetPlayerDamageCache(pnum).inc_sum[slot][dmgid];
}

int GetCachedPlayerMorePacked(int pnum, int slot, int dmgid) {
	return GetPlayerDamageCache(pnum).more_packed[slot][dmgid];
}

// Same two pools as InsertCacheFactor, but for terms that only hold for one damage category.
void InsertCacheFactor_Typed(int pnum, int slot, int dmgid, int category, int factor, bool isAdditive) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);

	if(isAdditive) {
		cache.inc_typed[slot][dmgid][category] += factor;
		return;
	}

	cache.more_typed[slot][dmgid][category] = CombinePackedMultiplier(cache.more_typed[slot][dmgid][category], FixedDiv(factor, 100));
}

// joins the same additive pool as the generic increased and the live buff term
int GetCachedPlayerIncreasedTyped(int pnum, int slot, int dmgid, int category) {
	return GetPlayerDamageCache(pnum).inc_typed[slot][dmgid][category];
}

int GetCachedPlayerMorePackedTyped(int pnum, int slot, int dmgid, int category) {
	return GetPlayerDamageCache(pnum).more_typed[slot][dmgid][category];
}

int GetCachedPlayerFlatFactor(int pnum, int slot, int dmgid) {
	return GetPlayerDamageCache(pnum).flat_factor[slot][dmgid];
}

// ---- conversion table ---------------------------------------------------------------------------

int GetPlayerConversionRowTotal(int pnum, int category) {
	return GetPlayerDamageCache(pnum).conv_row_total[category];
}

// Corruption implicits accumulate here as they are equipped and unequipped. delta already arrives
// negated on removal, the same way aval does everywhere else in ProcessItemImplicit.
//
// Rejects anything the ladder cannot carry rather than storing it and hoping: a destination at or
// before its source is either a no-op the walk skips or a cycle, and soul sits outside the ladder
// entirely. A pair that lands here wrong would otherwise be silently invisible.
void IncPlayerConversionPercent(int pnum, int source, int dest, int delta) {
	if(source < DND_DAMAGECATEGORY_BEGIN || source >= DND_DAMAGECATEGORY_END || dest <= source || dest >= DND_DAMAGECATEGORY_END)
		return;

	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.conv_raw[source][dest] += delta;

	// a removal that undershoots its own addition would otherwise leave a permanent negative that
	// eats the next item's roll
	if(cache.conv_raw[source][dest] < 0)
		cache.conv_raw[source][dest] = 0;
}

// Same accumulator, but for "% of one type added as extra of another". The source keeps its damage;
// this only ever creates.
void IncPlayerDamageGainPercent(int pnum, int source, int dest, int delta) {
	if(source < DND_DAMAGECATEGORY_BEGIN || source >= DND_DAMAGECATEGORY_END)
		return;
	if(dest <= source || dest >= DND_DAMAGECATEGORY_END)
		return;

	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.gain_raw[source][dest] += delta;

	if(cache.gain_raw[source][dest] < 0)
		cache.gain_raw[source][dest] = 0;
}

// Character loads re-equip everything, so this rides along with ResetPlayerModList for the same
// reason PlayerModData does -- without it a reload doubles every conversion the player is wearing.
void ResetPlayerConversionTable(int pnum) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);

	for(int src = 0; src < MAX_DAMAGE_CATEGORIES; ++src) {
		cache.conv_row_total[src] = 0;
		cache.gain_row_total[src] = 0;
		for(int dst = 0; dst < MAX_DAMAGE_CATEGORIES; ++dst) {
			cache.conv_raw[src][dst] = 0;
			cache.conv_pct[src][dst] = 0;
			cache.gain_raw[src][dst] = 0;
			cache.gain_pct[src][dst] = 0;
		}
	}
}

// Derives one usable row from the accumulated one: applies the "over 100% converts nothing extra"
// clamp and works out the row total. The excess is dropped PRO RATA rather than by mod order, so
// which conversion loses out does not depend on which item happened to be equipped first.
//
// The re-sum after the clamp is not optional: the pro rata divides truncate, so the row lands a
// little under 100, and a conv_row_total of a flat 100 would then take damage out of the primary and
// hand it to nobody.
void FinalizePlayerConversionRow(int pnum, int source) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	int dest, total = 0;

	for(dest = DND_DAMAGECATEGORY_BEGIN; dest < DND_DAMAGECATEGORY_END; ++dest)
		total += cache.conv_raw[source][dest];

	if(total <= 100) {
		for(dest = DND_DAMAGECATEGORY_BEGIN; dest < DND_DAMAGECATEGORY_END; ++dest)
			cache.conv_pct[source][dest] = cache.conv_raw[source][dest];
	}
	else {
		int clamped = 0, biggest = DND_CONV_NOSKIP;

		for(dest = DND_DAMAGECATEGORY_BEGIN; dest < DND_DAMAGECATEGORY_END; ++dest) {
			cache.conv_pct[source][dest] = cache.conv_raw[source][dest] * 100 / total;
			clamped += cache.conv_pct[source][dest];

			if(biggest == DND_CONV_NOSKIP || cache.conv_pct[source][dest] > cache.conv_pct[source][biggest])
				biggest = dest;
		}

		// The pro rata divides truncate, so the row lands a point or two short of 100. Over 100% is
		// CAPPED, not reduced -- a capped row still converts every last point of the source type --
		// so the remainder goes back into the largest share rather than leaving a sliver of the
		// original type on a build that converted all of it.
		if(biggest != DND_CONV_NOSKIP && clamped < 100) {
			cache.conv_pct[source][biggest] += 100 - clamped;
			clamped = 100;
		}

		total = clamped;
	}

	cache.conv_row_total[source] = total;
}

// Re-derived with the rest of the cached block, so it rides the same invalidation edge as everything
// else in there.
//
// That edge is NOT the weapon raise. The menu does not lower and raise your weapon unless a purchase
// replaces what you are holding, so equipping a conversion mod would otherwise not be noticed until
// you happened to switch weapons. What actually covers it is the explicit
// "DnD Force Damage Cache Recalculation" script, fired by UseItem on equip and by the menu's
// unequip path -- both of which run ProcessItemImplicit first, so conv_raw is already updated by the
// time the cache is cleared.
// No clamp, deliberately -- see gain_raw. This exists so the ladder can gate a source on one read
// instead of scanning the row.
void FinalizePlayerGainRow(int pnum, int source) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	int total = 0;

	for(int dest = DND_DAMAGECATEGORY_BEGIN; dest < DND_DAMAGECATEGORY_END; ++dest) {
		cache.gain_pct[source][dest] = cache.gain_raw[source][dest];
		total += cache.gain_raw[source][dest];
	}

	cache.gain_row_total[source] = total;
}

void BuildPlayerConversionTable(int pnum) {
	int dest;

	for(int src = DND_DAMAGECATEGORY_BEGIN; src < DND_DAMAGECATEGORY_END; ++src) {
		FinalizePlayerConversionRow(pnum, src);
		FinalizePlayerGainRow(pnum, src);
	}

	// "% of physical damage converted" has to hold whether the weapon swings or shoots, but the mods
	// only ever name BULLET as the physical source (DND_DAMAGECONVERSION_BEGIN). Melee borrows that
	// row rather than needing a roll of its own -- the same resolution added physical damage already
	// uses when it picks between the two physical categories.
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	for(dest = DND_DAMAGECATEGORY_BEGIN; dest < DND_DAMAGECATEGORY_END; ++dest) {
		cache.conv_pct[DND_DAMAGECATEGORY_MELEE][dest] = cache.conv_pct[DND_DAMAGECATEGORY_BULLET][dest];
		cache.gain_pct[DND_DAMAGECATEGORY_MELEE][dest] = cache.gain_pct[DND_DAMAGECATEGORY_BULLET][dest];
	}
	cache.conv_row_total[DND_DAMAGECATEGORY_MELEE] = cache.conv_row_total[DND_DAMAGECATEGORY_BULLET];
	cache.gain_row_total[DND_DAMAGECATEGORY_MELEE] = cache.gain_row_total[DND_DAMAGECATEGORY_BULLET];
}

// ---- the conversion ladder -----------------------------------------------------------------------

// Weight ceiling for the merge below. inc x weight has to leave room for two terms inside an int, and
// even a preposterous 50000% increase clears it with a factor of two to spare at this size.
#define DND_CONVMERGE_MAXWEIGHT 8192

// Amount-weighted mean of two (increase, amount) pairs -- the operation that keeps two conversion
// chains landing on the same destination honest, and the only place the walk loses anything to
// rounding.
//
// Only the RATIO of the two weights matters, so oversized ones are halved until the products fit
// rather than run through MulPercent_Exact. That buys the rounding term: flooring here biases every
// merge downward and a chain of them accumulates the drift, which measured at up to 2% on a real
// component. Rounding halves the worst case and centres the error instead of leaning on it. The
// shift costs at most one part in DND_CONVMERGE_MAXWEIGHT of ratio, two orders below what it saves.
//
// The sign rides outside the arithmetic because a negative increase is uncommon but real, and
// dropping it would quietly turn a cursed mod into a free one.
int MergeWeightedIncrease(int inc_a, int amt_a, int inc_b, int amt_b) {
	int total = amt_a + amt_b;
	if(total <= 0)
		return 0;

	while(amt_a > DND_CONVMERGE_MAXWEIGHT || amt_b > DND_CONVMERGE_MAXWEIGHT) {
		amt_a >>= 1;
		amt_b >>= 1;
		total = amt_a + amt_b;
		if(total <= 0)
			return 0;
	}

	return (inc_a * amt_a + inc_b * amt_b + total / 2) / total;
}

void ResetConversionWalk(pdmg_cache_T module& cache) {
	for(int c = DND_DAMAGECATEGORY_BEGIN; c < DND_DAMAGECATEGORY_END; ++c) {
		cache.conv_walk_amt[c] = 0;
		cache.conv_walk_inc[c] = 0;
	}
}

// Seeds one starting category, amount in DND_CONVFRAC fixed point. The category's OWN typed pools
// join the path right here, because converted damage keeps the modifiers of every type it has been
// -- including the one it started out as.
void SeedConversionWalk(pdmg_cache_T module& cache, int slot, int dmgid, int category, int amount) {
	if(amount <= 0)
		return;

	int seeded = ApplyPackedMultiplier(amount, cache.more_typed[slot][dmgid][category]);
	if(seeded <= 0)
		return;

	cache.conv_walk_inc[category] = MergeWeightedIncrease(
		cache.conv_walk_inc[category], cache.conv_walk_amt[category],
		cache.inc_typed[slot][dmgid][category], seeded
	);
	cache.conv_walk_amt[category] += seeded;
}

// One ascending pass over the ladder. A category has already received everything it will ever receive
// by the time it is reached as a source, because conversion only ever moves FORWARD along the enum --
// which is the whole reason the enum order is the ladder.
void RunConversionLadder(pdmg_cache_T module& cache, int slot, int dmgid) {
	int src, dst, share, moved, gained, left, last_dst, origin;

	for(src = DND_DAMAGECATEGORY_BEGIN; src < DND_DAMAGECATEGORY_END; ++src) {
		if(cache.conv_walk_amt[src] <= 0 || (!cache.conv_row_total[src] && !cache.gain_row_total[src]))
			continue;

		// Both conversion and gain take their share of the amount as it stands BEFORE either has
		// touched it, so the shares cannot compound and "10% of physical as extra cold" does not
		// shrink because some other mod converts physical elsewhere. The conversion remainder is
		// subtracted rather than recomputed so the pieces sum back to the whole.
		origin = cache.conv_walk_amt[src];
		left = origin;
		last_dst = DND_CONV_NOSKIP;

		for(dst = src + 1; dst < DND_DAMAGECATEGORY_END; ++dst) {
			if(!cache.conv_pct[src][dst] && !cache.gain_pct[src][dst])
				continue;

			// conversion leaves the source...
			share = MulPercent_Exact(origin, cache.conv_pct[src][dst]);
			left -= share;

			// ...gain does not, and that is the whole difference. It merges into the same
			// contribution rather than being tracked apart: identical source, identical destination,
			// identical path, so identical modifiers.
			share += MulPercent_Exact(origin, cache.gain_pct[src][dst]);
			if(share <= 0)
				continue;

			// arriving at dst adds its increased to the path and folds in its more
			gained = cache.conv_walk_inc[src] + cache.inc_typed[slot][dmgid][dst];
			moved = ApplyPackedMultiplier(share, cache.more_typed[slot][dmgid][dst]);
			if(moved <= 0)
				continue;

			cache.conv_walk_inc[dst] = MergeWeightedIncrease(
				cache.conv_walk_inc[dst], cache.conv_walk_amt[dst],
				gained, moved
			);
			cache.conv_walk_amt[dst] += moved;
			last_dst = dst;
		}

		// A full conversion has to leave NOTHING of the source type behind. Each destination's share
		// floors on its own, so the leftovers would otherwise pool into a sliver that still emits a
		// damage instance of the type the player converted away entirely.
		if(left > 0 && cache.conv_row_total[src] >= 100 && last_dst != DND_CONV_NOSKIP) {
			cache.conv_walk_amt[last_dst] += left;
			left = 0;
		}

		cache.conv_walk_amt[src] = left;
	}
}

// Copies the finished walk into one of the two trees.
//
// skip_category is the weapon's own type in the weapon tree: whatever conversion left behind there is
// dealt by the PRIMARY, which applies that category's own pools itself, so storing it would deal it
// twice. The added tree passes -1, because added flat damage that never converted is still a
// component of its own.
void StoreConversionWalk(pdmg_cache_T module& cache, int slot, int dmgid, int skip_category, bool is_weapon) {
	int amt, mask = 0;

	for(int c = DND_DAMAGECATEGORY_BEGIN; c < DND_DAMAGECATEGORY_END; ++c) {
		amt = c == skip_category ? 0 : cache.conv_walk_amt[c];
		if(amt < 0)
			amt = 0;

		if(is_weapon) {
			cache.conv_wfrac[slot][dmgid][c] = amt;
			cache.conv_winc[slot][dmgid][c] = cache.conv_walk_inc[c];
		}
		else {
			cache.conv_aamt[slot][dmgid][c] = amt;
			cache.conv_ainc[slot][dmgid][c] = cache.conv_walk_inc[c];
		}

		mask |= (!!amt) << c;
	}

	cache.comp_mask[slot][dmgid] |= mask;
}

// Parked in the added tree's own output slot until the walk overwrites it. The walk reads every seed
// before it writes anything back, so the two cannot collide and the raw values need no home of their
// own.
void SeedRawAddedFlat(int pnum, int slot, int dmgid, int category, int dmg) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.conv_aamt[slot][dmgid][category] = dmg;
}

// Runs both conversion trees for one (slot, dmgid). Called once at the end of the cached block, when
// inc_typed, more_typed and the raw added seeds are all final.
void ResolveDamageComponents(int pnum, int slot, int dmgid, int damage_category) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	int c;

	// the weapon's own base, normalized to 1.0 so what comes out is a fraction the shot can scale
	ResetConversionWalk(cache);
	SeedConversionWalk(cache, slot, dmgid, damage_category, DND_CONVFRAC_ONE);
	RunConversionLadder(cache, slot, dmgid);
	StoreConversionWalk(cache, slot, dmgid, damage_category, true);

	ResetConversionWalk(cache);
	for(c = DND_DAMAGECATEGORY_BEGIN; c < DND_DAMAGECATEGORY_END; ++c)
		SeedConversionWalk(cache, slot, dmgid, c, cache.conv_aamt[slot][dmgid][c] << DND_CONVFRAC_BITS);
	RunConversionLadder(cache, slot, dmgid);
	StoreConversionWalk(cache, slot, dmgid, DND_CONV_NOSKIP, false);

	cache.extra_mask = cache.comp_mask[slot][dmgid];
}

// Non-zero when this player deals anything beyond their weapon's own type, added or converted. The
// gate the mixed-damage bookkeeping tests before doing any per-element work.
int GetPlayerExtraDamageMask(int pnum) {
	return GetPlayerDamageCache(pnum).extra_mask;
}

int GetCachedConvWeaponFrac(int pnum, int slot, int dmgid, int category) {
	return GetPlayerDamageCache(pnum).conv_wfrac[slot][dmgid][category];
}

int GetCachedConvWeaponInc(int pnum, int slot, int dmgid, int category) {
	return GetPlayerDamageCache(pnum).conv_winc[slot][dmgid][category];
}

int GetCachedConvAddedAmount(int pnum, int slot, int dmgid, int category) {
	return GetPlayerDamageCache(pnum).conv_aamt[slot][dmgid][category];
}

int GetCachedConvAddedInc(int pnum, int slot, int dmgid, int category) {
	return GetPlayerDamageCache(pnum).conv_ainc[slot][dmgid][category];
}

// Zero for anyone with neither added damage nor conversion, which is the gate the hot paths test
// before doing any per-category work at all.
int GetCachedComponentMask(int pnum, int slot, int dmgid) {
	return GetPlayerDamageCache(pnum).comp_mask[slot][dmgid];
}

// ---- the current hit's component split -----------------------------------------------------------

int MakeComponentStageKey(int slot, int dmgid, int category) {
	return slot | (dmgid << 8) | (category << 16);
}

void ResetComponentStage(int pnum) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.comp_stage_mask = 0;
	cache.comp_stage_total = 0;
	cache.comp_stage_primary = 0;
	cache.comp_stage_key = DND_COMPSTAGE_NOKEY;
}

void AddComponentStageValue(int pnum, int category, int dmg) {
	if(dmg <= 0)
		return;

	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.comp_stage[category] = dmg;
	cache.comp_stage_mask |= 1 << category;
	cache.comp_stage_total += dmg;
}

void CommitComponentStage(int pnum, int slot, int dmgid, int category, int primary) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.comp_stage_primary = primary;
	cache.comp_stage_total += primary;
	cache.comp_stage_key = MakeComponentStageKey(slot, dmgid, category);
}

int GetComponentStageMask(int pnum) {
	return GetPlayerDamageCache(pnum).comp_stage_mask;
}

int GetComponentStageTotal(int pnum) {
	return GetPlayerDamageCache(pnum).comp_stage_total;
}

int GetComponentStagePrimary(int pnum) {
	return GetPlayerDamageCache(pnum).comp_stage_primary;
}

int GetComponentStageValue(int pnum, int category) {
	return GetPlayerDamageCache(pnum).comp_stage[category];
}

int GetComponentStageKey(int pnum) {
	return GetPlayerDamageCache(pnum).comp_stage_key;
}

// For a weapon that will not deal its damage until well after it rolled it. Take the snapshot in the
// same breath as the roll, put it back immediately before dealing.
//
// Restoring clobbers the live stage, which is safe because nothing else can be mid-split at that
// moment: ACS runs a script to completion between delays, and every other consumer stages and splits
// without one in between.
void SnapshotComponentStage(int pnum) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);

	for(int c = DND_DAMAGECATEGORY_BEGIN; c < DND_DAMAGECATEGORY_END; ++c)
		cache.defer_stage[c] = cache.comp_stage[c];

	cache.defer_total = cache.comp_stage_total;
	cache.defer_primary = cache.comp_stage_primary;
	cache.defer_mask = cache.comp_stage_mask;
	cache.defer_key = cache.comp_stage_key;
}

void RestoreComponentStage(int pnum) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);

	for(int c = DND_DAMAGECATEGORY_BEGIN; c < DND_DAMAGECATEGORY_END; ++c)
		cache.comp_stage[c] = cache.defer_stage[c];

	cache.comp_stage_total = cache.defer_total;
	cache.comp_stage_primary = cache.defer_primary;
	cache.comp_stage_mask = cache.defer_mask;
	cache.comp_stage_key = cache.defer_key;
}

int GetPlayerShotgunCount(int pnum) {
	return GetPlayerDamageCache(pnum).shotgun_count;
}

void SetPlayerShotgunCount(int pnum, int count) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.shotgun_count = count;
}

int GetCachedPlayerRandomRange(int pnum, int slot, int dmgid) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	if(cache.damage_cache[slot][dmgid].dmg_low && cache.damage_cache[slot][dmgid].dmg_high)
		return cache.damage_cache[slot][dmgid].dmg_low | (cache.damage_cache[slot][dmgid].dmg_high << 16);
	return 1;
}

// Invalidates ONE weapon's slots. Prefer this over ForcePlayerDamageCaching wherever
// the change is known to be weapon-local -- the full sweep touches
// MAX_CACHED_WEAPONS x MAX_CACHE_ELEMENTS (128 slots), this touches 8.
// A weapon with no slot has nothing cached to invalidate, so it is skipped rather than allocated.
void ForceWeaponDamageCaching(int pnum, int wepid) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	int slot = LookupWeaponCacheSlot(pnum, wepid);
	if(slot != DND_WEPCACHE_NOSLOT)
		ClearWeaponCacheSlot(cache, slot);
}

// The shotgun-owned flat bonus is a function of the TOTAL shotgun count, so gaining or
// losing one shifts every OTHER boomstick's cached flat -- and nothing else's. This is
// the exact blast radius: no reason to invalidate a plasma rifle because a shotgun was
// bought.
void ForceShotgunDamageCaching(int pnum) {
	auto shotguns = GetShotgunWeaponList();

	for(int i = 0; i < DND_SHOTGUN_WEAPON_COUNT; ++i)
		ForceWeaponDamageCaching(pnum, shotguns[i]);
}

// routed through the shared per-slot wipe so the reset field list can never drift.
// Mappings are deliberately left in place: they are not stale state, they only say which weapon
// owns which slot, and keeping them means a re-fire is a hit rather than a re-allocation.
void ForceClearCache(pdmg_cache_T module& cache, int pnum) {
	for(int i = 0; i < MAX_CACHED_WEAPONS; ++i)
		ClearWeaponCacheSlot(cache, i);
}

// forces player to recalculate the damage values
void ForcePlayerDamageCaching(int pnum) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	// if a mass recalculation was issued, so we don't keep repeating this over and over if player is simply crafting
	if(!cache.massRecalculationRequested) {
		cache.massRecalculationRequested = true;
		ForceClearCache(cache, pnum);
	}
}

#endif
