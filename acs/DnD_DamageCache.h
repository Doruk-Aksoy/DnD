
#ifndef DND_DAMAGECACHE_IN
#define DND_DAMAGECACHE_IN

#include "DnD_DamageCacheDefs.h"
#include "DnD_Common.h"
#include "DnD_WeaponDefs.h"

// the damage_cache holds base damage values and their rng ranges
// end_factor is (the factor we multiply our base value with using all damage increases) * (the constant to keep precision, which is 100k)
typedef struct pdmg_cache {
	bool massRecalculationRequested;
	bool norecalculate[MAXWEPS][MAX_CACHE_ELEMENTS]; 								// false => needs recalculation otherwise no
	pdmg_T damage_cache[MAXWEPS][MAX_CACHE_ELEMENTS];
	int flat_values[MAXWEPS][MAX_CACHE_ELEMENTS];									// holds flat dmg bonuses
	int flat_factor[MAXWEPS][MAX_CACHE_ELEMENTS];									// holds added flat damage bonus efficiency

	// These two are deliberately kept apart rather than pre-multiplied into one
	// factor: the buff layer's own "increased" has to rejoin the SAME additive pool
	// at request time, and once the two are collapsed into a single number the buff
	// term can never be taken back out again.
	int inc_sum[MAXWEPS][MAX_CACHE_ELEMENTS];										// integer percent DELTA, baseline 0
	int more_packed[MAXWEPS][MAX_CACHE_ELEMENTS];									// packed normalized multiplier; 0 == hard zero

	int shotgun_count;																// last CountShotgunWeaponsOwned() seen, to detect changes
} pdmg_cache_T;

pdmg_cache_T module& GetPlayerDamageCache(int pnum) {
	static pdmg_cache_T cache[MAXPLAYERS];
	return cache[pnum];
}

bool PlayerDamageNeedsCaching(int pnum, int wepid, int dmgid) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	return !cache.norecalculate[wepid][dmgid];
}

void ClearCache(int pnum, int wepid, int dmgid) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.flat_values[wepid][dmgid] = 0;
	cache.inc_sum[wepid][dmgid] = 0;
	cache.more_packed[wepid][dmgid] = DND_PACKED_MULT_IDENTITY;
}

// this guy gets called last, so we mark recalc stuff here
void MarkCachingComplete(int pnum, int wepid, int dmgid) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.norecalculate[wepid][dmgid] = true;
	// clean the previously issued mass recalc request, since player fired now they might want to craft afterwards
	cache.massRecalculationRequested = false;
}

void CachePlayerDamage(int pnum, int dmg, int wepid, int dmgid, int dmg_rand, int flat_factor = 100) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	
	cache.damage_cache[wepid][dmgid].dmg = dmg;
	cache.damage_cache[wepid][dmgid].dmg_low = dmg_rand & 0xFFFF;
	cache.damage_cache[wepid][dmgid].dmg_high = dmg_rand >> 16;
	cache.norecalculate[wepid][dmgid] = false;
	cache.flat_factor[wepid][dmgid] = flat_factor;
}

void CachePlayerFlatDamage(int pnum, int dmg, int wepid, int dmgid) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	// not cached
	if(!cache.norecalculate[wepid][dmgid]) {
		dmg = dmg * cache.flat_factor[wepid][dmgid] / 100;
		cache.flat_values[wepid][dmgid] = dmg;
	}
}

// "increased" sums into its own pool; "more" folds into the packed product.
// factor for the multiplicative path is an integer percent that ALREADY carries the
// 100 baseline (e.g. 150 for "+50% more").
void InsertCacheFactor(int pnum, int wepid, int dmgid, int factor, bool isAdditive) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);

	if(isAdditive) {
		cache.inc_sum[wepid][dmgid] += factor;
		return;
	}

	// FixedDiv, not (factor << 16) / 100 -- the shift overflows past factor 32767
	cache.more_packed[wepid][dmgid] = CombinePackedMultiplier(cache.more_packed[wepid][dmgid], FixedDiv(factor, 100));
}

// used for multiplicative item mods that are by default fixed point.
// factor is an ABSOLUTE 16.16 multiplier (1.0 == no change), which is what both
// live callers already produce.
void InsertCacheFactor_Fixed(int pnum, int wepid, int dmgid, int factor) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.more_packed[wepid][dmgid] = CombinePackedMultiplier(cache.more_packed[wepid][dmgid], factor);
}

int GetCachedPlayerDamage(int pnum, int wepid, int dmgid) {
	return GetPlayerDamageCache(pnum).damage_cache[wepid][dmgid].dmg;
}

int GetCachedPlayerFlatDamage(int pnum, int wepid, int dmgid) {
	return GetPlayerDamageCache(pnum).flat_values[wepid][dmgid];
}

// integer percent DELTA -- the caller adds the 100 baseline, and the buff layer's
// own "increased" joins this same pool before the more-product is applied
int GetCachedPlayerIncreased(int pnum, int wepid, int dmgid) {
	return GetPlayerDamageCache(pnum).inc_sum[wepid][dmgid];
}

int GetCachedPlayerMorePacked(int pnum, int wepid, int dmgid) {
	return GetPlayerDamageCache(pnum).more_packed[wepid][dmgid];
}

int GetPlayerShotgunCount(int pnum) {
	return GetPlayerDamageCache(pnum).shotgun_count;
}

void SetPlayerShotgunCount(int pnum, int count) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	cache.shotgun_count = count;
}

int GetCachedPlayerRandomRange(int pnum, int wepid, int dmgid) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	if(cache.damage_cache[wepid][dmgid].dmg_low && cache.damage_cache[wepid][dmgid].dmg_high)
		return cache.damage_cache[wepid][dmgid].dmg_low | (cache.damage_cache[wepid][dmgid].dmg_high << 16);
	return 1;
}

// Invalidates ONE weapon's slots. Prefer this over ForcePlayerDamageCaching wherever
// the change is known to be weapon-local -- the full sweep touches
// MAXWEPS x MAX_CACHE_ELEMENTS (~816 slots), this touches 8.
void ForceWeaponDamageCaching(int pnum, int wepid) {
	pdmg_cache_T module& cache = GetPlayerDamageCache(pnum);
	for(int j = 0; j < MAX_CACHE_ELEMENTS; ++j) {
		cache.norecalculate[wepid][j] = false;
		cache.inc_sum[wepid][j] = 0;
		cache.more_packed[wepid][j] = DND_PACKED_MULT_IDENTITY;
		cache.flat_values[wepid][j] = 0;
	}
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

// routed through the single-weapon form so the reset field list can never drift
void ForceClearCache(pdmg_cache_T module& cache, int pnum) {
	for(int i = 0; i < MAXWEPS; ++i)
		ForceWeaponDamageCaching(pnum, i);
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
