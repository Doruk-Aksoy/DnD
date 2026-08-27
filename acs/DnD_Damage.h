#ifndef DND_DAMAGE_IN
#define DND_DAMAGE_IN

#include "DnD_DoT.h"
#include "DnD_Physics.h"
#include "DnD_AttackInfo.h"

#ifdef ISDEBUGBUILD
	int damage_script_count = 0;
#endif

#define APROP_PLAYERSOURCED APROP_AMBUSH
#define APROP_STOREDREFLECTDAMAGETYPE APROP_HEALTH

#define DND_CRIT_TOKEN 69

#define DND_PLAYER_HITSCAN_Z 38.0
#define MAX_RIPPERS_ACTIVE 256
#define MAX_RIPPER_HITS_STORED 128

#define DND_RUINATION_REDUCE_PER_STACK 5

#define DND_MAX_DAMAGELOSEHITS 7 // we let maximum of 70% reduction, so you'll do min 30%
#define DND_DAMAGELOST_PERCENT 10 // 10%

#define DND_HARDCORE_DEBUFF 15 // 15% more damage taken

#define DND_INVULSPHERE_FACTOR 10 // 10% damage taken, divides by 10 => 90% reduce

#define DND_EXPLOSION_FLAGVARIABLE "user_flags"

#define DND_ADDEDIGNITE_FACTOR 50 // 50%

#define DND_MONSTER_PERCENTDAMAGEBASE 10 // 10%
#define DND_MONSTER_PERCENTDAMAGEBASE_LOW 2 // 2%

#define DND_MONSTER_POISONPERCENT 33 // 33% of damage taken from a hit is dealt as poison damage again over the duration
#define DND_MONSTER_POISONDOT_MINTIME 2
#define DND_MONSTER_POISONDOT_MAXTIME 5

#define OCCULT_WEAKEN_DURATION 2

#define VAAJ_WEAKEN_DURATION 3
#define DND_VAAJ_WEAKENPCT 10

#define DND_CORRUPTORB_DMGREDUCE 75 // /4 => 75% reduced dmg

#define DND_MONSTER_BURN_PERCENT 10
#define DND_PLAYER_BURNING_CHANCE 0.33
#define DND_PLAYER_BURNING_MINTIME 3
#define DND_PLAYER_BURNING_MAXTIME 6

#define MAX_RIPCOUNT 4096

#define DND_MAX_CHEGOVAX_TICS 15 // INV_ESS_CHEGOVAX stops ramping an ignite after this many damage tics
#define DND_MAX_POISON_TICDMG_CAP 100 // 100% multiplier on the tic dmg, essentially double damage

enum {
	DND_DAMAGETYPE_MELEE,
	DND_DAMAGETYPE_MELEEOCCULT,
	DND_DAMAGETYPE_PARRY,
	DND_DAMAGETYPE_PHYSICAL,
	DND_DAMAGETYPE_SILVERBULLET,
	DND_DAMAGETYPE_ENERGY,
	DND_DAMAGETYPE_ENERGYEXPLOSION,
	//DND_DAMAGETYPE_EXPLOSIVES,
	
	// occult
	DND_DAMAGETYPE_OCCULT,
	DND_DAMAGETYPE_OCCULTFIRE,
	DND_DAMAGETYPE_OCCULTEXPLOSION,
	DND_DAMAGETYPE_MAGICSEAL,

	// elemental types
	DND_DAMAGETYPE_FIRE,
	DND_DAMAGETYPE_ICE,
	DND_DAMAGETYPE_POISON,
	DND_DAMAGETYPE_DESOLATOR,
	DND_DAMAGETYPE_EMERALD,
	DND_DAMAGETYPE_LIGHTNING,
	
	DND_DAMAGETYPE_SOUL
};

// monster flag encoding in stamina
enum {
	DND_DAMAGETYPEFLAG_PHYSICAL = 1,
	DND_DAMAGETYPEFLAG_HITSCAN = 2,
	DND_DAMAGETYPEFLAG_EXPLOSIVE = 4, // is used as a hint to the game to treat it as radius damage, not explosive damage type
	DND_DAMAGETYPEFLAG_MAGICAL = 8,
	DND_DAMAGETYPEFLAG_ENERGY = 16,
	DND_DAMAGETYPEFLAG_FIRE = 32,
	DND_DAMAGETYPEFLAG_ICE = 64,
	DND_DAMAGETYPEFLAG_POISON = 128,
	DND_DAMAGETYPEFLAG_LIGHTNING = 256,
	DND_DAMAGETYPEFLAG_PERCENTHP = 512,
	DND_DAMAGETYPEFLAG_SPELL = 1024,
	DND_DAMAGETYPEFLAG_PERCENTHP_LOW = 2048,
	DND_DAMAGETYPEFLAG_DOT = 4096,
	DND_DAMAGETYPEFLAG_ISBLEED = 8192,
	DND_DAMAGETYPEFLAG_IGNORERESISTS = 16384,

	DND_DAMAGETYPEFLAG_HURTSPECIES = 268435456,
	DND_DAMAGETYPEFLAG_USEMASTER = 536870912,
	DND_DAMAGETYPEFLAG_REFLECTABLE = 1073741824
};

// The bits that say what KIND of damage this is, as opposed to how it is delivered. EXPLOSIVE and
// HITSCAN are deliberately not in here -- they are delivery hints, and an actor carrying only one of
// them has no damage type at all.
#define DND_DAMAGETYPEFLAG_ALLTYPES (DND_DAMAGETYPEFLAG_PHYSICAL | DND_DAMAGETYPEFLAG_MAGICAL | DND_DAMAGETYPEFLAG_ENERGY | DND_DAMAGETYPEFLAG_FIRE | DND_DAMAGETYPEFLAG_ICE | DND_DAMAGETYPEFLAG_POISON | DND_DAMAGETYPEFLAG_LIGHTNING)

// The "touch" elite traits -- DND_EMBERTOUCH / RIMETOUCH / STORMTOUCH. The monster gains a share of
// whatever its attack's OWN damage type is as extra damage of another element, so an energy attack
// from an EMBERTOUCH monster lands its energy plus extra fire.
//
// GAIN, not conversion: the source keeps its full damage and the extra rides on top. Because the
// gained portion is never fed back through the resolver there is no chaining, which is why none of
// the acyclic ladder machinery the player's own conversion needs exists here -- any (source,
// destination) pair is safe, in any order.
#define DND_MONSTER_TOUCHGAIN_PCT 20 // 20% of the attack's own damage, per touch trait

enum {
	DND_IGNITEFLAG_CANPROLIF = 1,
	DND_IGNITEFLAG_ADDEDIGN = 2 // reserved, currently unread -- the burn is handed a resolved damage now
};

int MonsterDamageTypeToDamageCategory(int d) {
	if(d & DND_DAMAGETYPEFLAG_PHYSICAL)
		return DND_DAMAGECATEGORY_BULLET;

	if(d & DND_DAMAGETYPEFLAG_FIRE)
		return DND_DAMAGECATEGORY_FIRE;
	if(d & DND_DAMAGETYPEFLAG_ICE)
		return DND_DAMAGECATEGORY_ICE;
	if(d & DND_DAMAGETYPEFLAG_POISON)
		return DND_DAMAGECATEGORY_POISON;
	if(d & DND_DAMAGETYPEFLAG_LIGHTNING)
		return DND_DAMAGECATEGORY_LIGHTNING;

	if(d & DND_DAMAGETYPEFLAG_ENERGY)
		return DND_DAMAGECATEGORY_ENERGY;

	if(d & DND_DAMAGETYPEFLAG_MAGICAL)
		return DND_DAMAGECATEGORY_OCCULT;

	return DND_DAMAGECATEGORY_MELEE;
}

int DamageCategoryToMonsterDamageType(int d) {
	switch(d) {
		case DND_DAMAGECATEGORY_BULLET:
		return DND_DAMAGETYPEFLAG_PHYSICAL;

		case DND_DAMAGECATEGORY_FIRE:
		return DND_DAMAGETYPEFLAG_FIRE;
		case DND_DAMAGECATEGORY_ICE:
		return DND_DAMAGETYPEFLAG_ICE;
		case DND_DAMAGECATEGORY_POISON:
		return DND_DAMAGETYPEFLAG_POISON;
		case DND_DAMAGECATEGORY_LIGHTNING:
		return DND_DAMAGETYPEFLAG_LIGHTNING;

		case DND_DAMAGECATEGORY_ENERGY:
		return DND_DAMAGETYPEFLAG_ENERGY;

		case DND_DAMAGECATEGORY_OCCULT:
		return DND_DAMAGETYPEFLAG_MAGICAL;
	}

	return DND_DAMAGETYPEFLAG_EXPLOSIVE | DND_DAMAGETYPEFLAG_PHYSICAL;
}

#define MAX_DAMAGE_TYPES (DND_DAMAGETYPE_SOUL + 1)
#define DAMAGE_TYPE_SHIFT 5
#define DAMAGE_TYPE_MASK 0x1F

bool IsMeleeDamage(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_MELEE && damage_type <= DND_DAMAGETYPE_PARRY;
}

bool IsBulletDamage(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_PHYSICAL && damage_type <= DND_DAMAGETYPE_SILVERBULLET;
}

bool IsEnergyDamage(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_ENERGY && damage_type <= DND_DAMAGETYPE_ENERGYEXPLOSION;
}

bool IsOccultDamage(int damage_type) {
	return (damage_type >= DND_DAMAGETYPE_OCCULT && damage_type <= DND_DAMAGETYPE_MAGICSEAL) || damage_type == DND_DAMAGETYPE_MELEEOCCULT || damage_type == DND_DAMAGETYPE_SOUL;
}

bool IsFireDamage(int damage_type) {
	return damage_type == DND_DAMAGETYPE_FIRE;
}

bool IsIceDamage(int damage_type) {
	return damage_type == DND_DAMAGETYPE_ICE;
}

bool IsPoisonDamage(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_POISON && damage_type <= DND_DAMAGETYPE_EMERALD;
}

bool IsLightningDamage(int damage_type) {
	return damage_type == DND_DAMAGETYPE_LIGHTNING;
}

bool IsElementalDamageType(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_FIRE && damage_type <= DND_DAMAGETYPE_LIGHTNING;
}

bool IsElementalDamageCategory(int damage_category) {
	return damage_category >= DND_ELECATEGORY_BEGIN && damage_category <= DND_ELECATEGORY_END;
}

bool IsDamageCapableOfIgnite(int damage_type) {
	return damage_type == DND_DAMAGETYPE_FIRE || damage_type == DND_DAMAGETYPE_OCCULTFIRE;
}

int GetDamageCategory(int damage_type, int flags) {
	if(IsBulletDamage(damage_type))
		return DND_DAMAGECATEGORY_BULLET;
	else if(IsMeleeDamage(damage_type) && damage_type != DND_DAMAGETYPE_MELEEOCCULT) // little note: while the weapon is melee and can benefit from melee bonuses, if it's occult damage type it'd be occult category
		return DND_DAMAGECATEGORY_MELEE;
	else if(IsEnergyDamage(damage_type))
		return DND_DAMAGECATEGORY_ENERGY;
	else if(IsFireDamage(damage_type))
		return DND_DAMAGECATEGORY_FIRE;
	else if(IsIceDamage(damage_type))
		return DND_DAMAGECATEGORY_ICE;
	else if(IsPoisonDamage(damage_type))
		return DND_DAMAGECATEGORY_POISON;
	else if(IsLightningDamage(damage_type))
		return DND_DAMAGECATEGORY_LIGHTNING;
	else if(damage_type == DND_DAMAGETYPE_SOUL || (flags & DND_DAMAGEFLAG_SOULATTACK))
		return DND_DAMAGECATEGORY_SOUL;
	// switched soul up because IsOccultDamage contains soul damage in it, and it'd return occult, absorbing other checks
	return DND_DAMAGECATEGORY_OCCULT;
}

str DamageTypeList[MAX_DAMAGE_TYPES] = {
	"Melee",
	"Melee_Magic",
	"Bullet",
	"BulletMagicX",
	"Energy",
	"EnergyExp",

	"Magic",
	"MagicFire",
	"Explosives_Magic",
	"MagicSealing",
	
	// elemental
	"Fire",
	"P_Ice",
	"Poison",
	"Desolator",
	"Emerald",
	"Lightning",
	
	"SoulType"
};

vec3_T PlayerDamageVector[MAXPLAYERS];

enum {
	DND_SPECIALBLOOD_STONE
};

#define MAX_BLOOD_TYPES (DND_SPECIALBLOOD_STONE + 1)
str SpecialBloodFX[MAX_BLOOD_TYPES] = {
	"StoneBitsSpawner"
};

enum {
	DND_HITBEEP_WEAKNESS,
	DND_HITBEEP_RESIST,
	DND_HITBEEP_IMMUNITY,
	DND_HITBEEP_INVULNERABLE
};
#define DND_MAX_HITBEEPS (DND_HITBEEP_INVULNERABLE + 1)
#define HITBEEP_SOUND 0
#define HITBEEP_TIMER 1
str HitBeepSounds[DND_MAX_HITBEEPS][2] = {
	{ "HitBeep/Weakness", 		"WeaknessBeepTimer"		},
	{ "HitBeep/Resist", 		"BeepTimer"				},
	{ "HitBeep/Immune", 		"HardBeepTimer"			},
	{ "HitBeep/Invulnerable",  	"InvulBeepTimer"		}
};

#define DND_CULL_BASEPERCENT 15 // percent
#define DND_CULL_BASEPERCENT_VS_PLAYER 10 // percent
#define DND_DESOLATOR_DMG_GAIN 10 // 10%
#define DND_DISTANCEDAMAGE_VARIABLE "user_tics"

#define DND_BASE_FREEZETIMER 21 // 3 seconds base time (21 x 5 = 105)
#define DND_BASE_CHILL_CAP 5 // 50% health dealt in cold = maximum slow

#define DND_BASE_OVERLOADCHANCE 5
#define DND_BASE_OVERLOADBUFF 20 // 20%
#define DND_MAX_OVERLOADTARGETS 128 // up to 128 allowed
#define DND_BASE_OVERLOADZAPDELAY 3 // 3 tics


#define DND_EXTRAUNDEADDMG_MULTIPLIER 300

// Indexed by monster id (victim tid - DND_MONSTERTID_BEGIN), so DND_MAX_MONSTERS is the real bound.
// The mask and shift below stay at 14 bits regardless: they are about packing the id into a script
// argument alongside the flags, and 12800 still fits in 14 bits.
#define DND_MONSTER_TICDATA_BITMASK 0x3FFF // 14 bits
#define DND_DAMAGE_ACCUM_SHIFT 14 // 2^14 = 16384
// `running` is "is a tic script alive for this pair" and is kept OUT of `total`. While the
// accumulator doubled as the latch, the bank had to precede every reduction or a fully absorbed hit
// read as "no script running" and the next hit started a second one. One bit per id, so the latch
// costs 1/32 of what a second array would. Both live in one struct to spend one global index, not two.
#define DND_TICLATCH_WORDS (DND_MAX_MONSTERS / 32)

typedef struct {
	int total[DND_MAX_MONSTERS];					// damage banked this tic, per monster id
	int running[DND_TICLATCH_WORDS];				// one bit per monster id
} dmg_tic_T;

global dmg_tic_T 27: PlayerDamageTic[MAXPLAYERS];

bool IsDamageTicRunning(int pnum, int m_id) {
	return !!(PlayerDamageTic[pnum].running[m_id >> 5] & (1 << (m_id & 31)));
}

void SetDamageTicRunning(int pnum, int m_id) {
	PlayerDamageTic[pnum].running[m_id >> 5] |= 1 << (m_id & 31);
}

void ClearDamageTicRunning(int pnum, int m_id) {
	PlayerDamageTic[pnum].running[m_id >> 5] &= ~(1 << (m_id & 31));
}

// ============================================================================
//  MIXED DAMAGE WORKING SET
//
//  PlayerDamageTic[].total is the per victim TOTAL, which for a single element hit already IS that
//  element's subtotal. It stays the fast path: direct index, no lookup.
//
//  A hit carrying more than one element needs a breakdown -- ignite must burn for the fire part,
//  not the sum -- and a flat per victim total cannot carry one: an element axis would multiply a 819k
//  int array. The live working set is tiny by comparison (the victims THIS player hit THIS tic,
//  taking mixed damage), so this is a small direct mapped table, touched only when the player
//  actually has added damage.
// ============================================================================
#define DND_MAX_MIXEDVICTIMS 256                        // power of two, doubles as the direct map mask
#define DND_MIXEDVICTIM_MASK (DND_MAX_MIXEDVICTIMS - 1)
#define DND_MIXEDTIC_NOSLOT -1
#define DND_MIXEDTIC_TOMBSTONE -1                       // in the victim field: released, reusable, not a run terminator

// Elements an ailment can key off. Deliberately not all nine damage categories: only these carry a
// status effect, and two of them (ice, lightning) need the flag rather than a magnitude.
enum {
	DND_TICELEM_PHYSICAL,
	DND_TICELEM_FIRE,
	DND_TICELEM_ICE,
	DND_TICELEM_LIGHTNING,
	DND_TICELEM_POISON,

	DND_MAX_TICELEMS
};

// Gains are summed per DESTINATION element, so the most portions a hit can ever carry is the number
// of destinations that exist -- not the number of sources that can grant one. Tying the bound to the
// enum means a new gain source can never overflow the portion arrays, however many of them roll.
#define DND_MAX_TOUCHGAINS DND_MAX_TICELEMS

typedef struct {
	int victim;										// monster id + 1, 0 == free, so a zeroed table reads as empty
	int flags_or;									// OR of the tic flags of EVERY hit that contributed
	int subtotal[DND_MAX_TICELEMS];
} mixed_tic_T;

global mixed_tic_T 39: PlayerMixedTicData[MAXPLAYERS][DND_MAX_MIXEDVICTIMS];

// which subtotal a hit's tic flags belong to, or DND_MIXEDTIC_NOSLOT for an element that has none
int GetTicElementOfFlags(int tflags) {
	if(tflags & DND_DAMAGETICFLAG_PHYSICAL)
		return DND_TICELEM_PHYSICAL;
	if(tflags & DND_DAMAGETICFLAG_FIRE)
		return DND_TICELEM_FIRE;
	if(tflags & DND_DAMAGETICFLAG_ICE)
		return DND_TICELEM_ICE;
	if(tflags & DND_DAMAGETICFLAG_LIGHTNING)
		return DND_TICELEM_LIGHTNING;
	if(tflags & DND_DAMAGETICFLAG_POISON)
		return DND_TICELEM_POISON;
	return DND_MIXEDTIC_NOSLOT;
}

// Direct mapped on the monster id with linear probing. Returns DND_MIXEDTIC_NOSLOT when the entry
// is absent and create is false, or when the table is genuinely full -- callers must treat that as
// "fall back to the total", which is exactly today's behaviour.
//
// Release leaves a TOMBSTONE rather than an empty slot. Zeroing would cut a run in half and hide
// every entry that had probed past it, and entries from one tic are released while the next tic is
// already inserting, so runs are live across that boundary. A tombstone is reusable but does not
// stop a probe.
int FindMixedTicSlot(int pnum, int m_id, bool create) {
	int start = m_id & DND_MIXEDVICTIM_MASK;
	int reusable = DND_MIXEDTIC_NOSLOT;
	int i, slot, occupant;

	for(i = 0; i < DND_MAX_MIXEDVICTIMS; ++i) {
		slot = (start + i) & DND_MIXEDVICTIM_MASK;
		occupant = PlayerMixedTicData[pnum][slot].victim;

		if(occupant == m_id + 1)
			return slot;

		// never used: the entry cannot be further along, so the search ends here either way
		if(!occupant) {
			if(reusable == DND_MIXEDTIC_NOSLOT)
				reusable = slot;
			break;
		}

		// tombstone: claimable, but keep probing in case the entry itself is further along
		if(occupant == DND_MIXEDTIC_TOMBSTONE && reusable == DND_MIXEDTIC_NOSLOT)
			reusable = slot;
	}

	if(!create || reusable == DND_MIXEDTIC_NOSLOT)
		return DND_MIXEDTIC_NOSLOT;

	PlayerMixedTicData[pnum][reusable].victim = m_id + 1;
	PlayerMixedTicData[pnum][reusable].flags_or = 0;
	for(i = 0; i < DND_MAX_TICELEMS; ++i)
		PlayerMixedTicData[pnum][reusable].subtotal[i] = 0;

	return reusable;
}

void RecordMixedTicDamage(int pnum, int m_id, int tflags, int dmg) {
	int slot = FindMixedTicSlot(pnum, m_id, true);
	if(slot == DND_MIXEDTIC_NOSLOT)
		return;

	PlayerMixedTicData[pnum][slot].flags_or |= tflags;

	int elem = GetTicElementOfFlags(tflags);
	if(elem != DND_MIXEDTIC_NOSLOT)
		PlayerMixedTicData[pnum][slot].subtotal[elem] += dmg;
}

// -1 when this player has no breakdown for this victim, so the caller keeps using the total
int GetMixedTicSubtotal(int pnum, int m_id, int elem) {
	int slot = FindMixedTicSlot(pnum, m_id, false);
	if(slot == DND_MIXEDTIC_NOSLOT)
		return -1;
	return PlayerMixedTicData[pnum][slot].subtotal[elem];
}

// The damage this element alone did this tic, falling back to the total when no breakdown exists.
// That fallback covers three cases and all of them want the total: a single element hit (where the
// total already IS this element's subtotal), a table miss, and ADDEDIGNITE on a weapon with no fire
// component at all, where the ignite has always scaled off whatever was actually dealt.
int GetTicElementDamage(int pnum, int m_id, int elem) {
	int sub = GetMixedTicSubtotal(pnum, m_id, elem);
	if(sub > 0)
		return sub;
	return PlayerDamageTic[pnum].total[m_id];
}

// Same map, but to the receive path's flag bits rather than the dealing path's type ids. The two
// paths speak different vocabularies for the same five elements, and gains are built on this side.
int MapTicElementToDamageTypeFlag(int elem) {
	switch(elem) {
		case DND_TICELEM_PHYSICAL:
		return DND_DAMAGETYPEFLAG_PHYSICAL;
		case DND_TICELEM_FIRE:
		return DND_DAMAGETYPEFLAG_FIRE;
		case DND_TICELEM_ICE:
		return DND_DAMAGETYPEFLAG_ICE;
		case DND_TICELEM_LIGHTNING:
		return DND_DAMAGETYPEFLAG_LIGHTNING;
		case DND_TICELEM_POISON:
		return DND_DAMAGETYPEFLAG_POISON;
	}
	return 0;
}

int MapTicElementToDamageType(int elem) {
	switch(elem) {
		case DND_TICELEM_PHYSICAL:
		return DND_DAMAGETYPE_PHYSICAL;
		case DND_TICELEM_FIRE:
		return DND_DAMAGETYPE_FIRE;
		case DND_TICELEM_ICE:
		return DND_DAMAGETYPE_ICE;
		case DND_TICELEM_LIGHTNING:
		return DND_DAMAGETYPE_LIGHTNING;
		case DND_TICELEM_POISON:
		return DND_DAMAGETYPE_POISON;
	}
	return DND_DAMAGETYPE_PHYSICAL;
}

// Applies the buff layer's more-multipliers to each element's own share of the tic instead of to the
// whole thing. Everything HandlePlayerBuffs weighs -- the elemental sigils, the accessory checks, a
// future "more cold damage for 10 seconds" -- is keyed on the damage type, so on a mixed hit it must
// only reach the component that actually is that type.
//
// With a single element in the tic this is exactly the old arithmetic: one subtotal, equal to the
// total, multiplied by that element's factor. Damage no element claimed (an untyped component, or a
// table miss) falls back to the first hit's type, which is what governed all of it before.
int ApplyPerElementBuffDamage(int pnum, int victim_tid, int victim_data, int wepid, int flags, int damage_type, int total) {
	int claimed = 0, scaled = 0;
	int elem, sub;

	for(elem = 0; elem < DND_MAX_TICELEMS; ++elem) {
		sub = GetMixedTicSubtotal(pnum, victim_data, elem);
		if(sub <= 0)
			continue;

		claimed += sub;
		scaled += MulPercent_Exact(sub, HandlePlayerBuffs(pnum + P_TIDSTART, victim_tid, MapTicElementToDamageType(elem), wepid, flags));
	}

	if(claimed < total)
		scaled += MulPercent_Exact(total - claimed, HandlePlayerBuffs(pnum + P_TIDSTART, victim_tid, damage_type, wepid, flags));

	return scaled;
}

int GetMixedTicFlags(int pnum, int m_id) {
	int slot = FindMixedTicSlot(pnum, m_id, false);
	if(slot == DND_MIXEDTIC_NOSLOT)
		return 0;
	return PlayerMixedTicData[pnum][slot].flags_or;
}

void ReleaseMixedTicSlot(int pnum, int m_id) {
	int slot = FindMixedTicSlot(pnum, m_id, false);
	if(slot != DND_MIXEDTIC_NOSLOT)
		PlayerMixedTicData[pnum][slot].victim = DND_MIXEDTIC_TOMBSTONE;
}

// Drops every damage tic latch left behind by the map that is ending, and the mixed slots under
// them. A stranded latch silently loses every ailment, the damage numbers, the push, lifesteal and
// the hurt token for that (player, monster) pair, while health damage still lands -- so it reads in
// play as "this monster cannot be ignited any more", not as a damage bug.
//
// The array is `global` and ids are handed out from 0 again next map, so a slot bricked on one map
// resurfaces under whatever monster inherits that id later.
//
// CALL THIS BESIDE EVERY ResetUsedTIDs, NOT JUST THE MAP CHANGE ONES. The ROUND restarts (survival
// wipe in "DnD On Death", GAMEEVENT_ROUND_ABORTED) restart ids on the SAME map, where nothing
// unloads and so nothing else would clear the latches. Those two went unflushed and were the
// reported "after a while nothing can ignite anything": ids start at 0 every round, so the LOW ids
// brick first and it reads as a player-wide loss of every ailment.
//
// Two details in the body:
//  - The write is conditional. World and global arrays are sparse maps, so assigning 0 to an
//    untouched entry MATERIALISES it, and the sweep would build the table it is walking.
//  - NO PlayerInGame filter. That is true for how residue is CREATED but not for how it is
//    inherited: a disconnected slot kept its residue and handed it to whoever took the slot next.
//
// THIS IS THE BACKSTOP, NOT THE PRIMARY DEFENCE -- DropMonsterTicResidue below needs no bound at
// all. This sweep can only reach ids it can DESCRIBE, and the id space is sparse: GiveMonsterTID
// KEEPS a mapper-assigned tid already inside the pool, so a monster on tid 500 takes m_id 498 on a
// map whose counters only reached 200. It still earns its place for the SKIPPED ids, which belong
// to non-monster things on pool tids and never pass through GiveMonsterTID.
void FlushDamageTicResidue() {
	int high = InformationInLevel[LEVELINFO_TID_MONSTER] + InformationInLevel[LEVELINFO_SKIPPEDMONSTERTID];
	if(high > DND_MAX_MONSTERS)
		high = DND_MAX_MONSTERS;

	for(int p = 0; p < MAXPLAYERS; ++p) {
		// MAXPLAYERS is 64 and a busy map hands out ~750 monster ids, so sweeping empty slots costs
		// 48k iterations inside the same tic as SaveAllPlayerData -- that is a runaway, and it is
		// exactly what killed "DnD On Unloading" and "DnD On Map Load" on CHX05. An empty slot is
		// cleared by DropPlayerTicResidue as the player LEAVES instead.
		if(!PlayerInGame(p))
			continue;

		for(int i = 0; i < high; ++i) {
			// latch first: a hit fully eaten by fortify banks nothing, so a stranded latch can sit
			// over a zero accumulator and block that pair's ailments for good
			if(IsDamageTicRunning(p, i) || PlayerDamageTic[p].total[i]) {
				ClearDamageTicRunning(p, i);
				PlayerDamageTic[p].total[i] = 0;
				ReleaseMixedTicSlot(p, i);
			}
		}
	}
}

// Clears the latch for ONE monster id at the instant that id changes hands. Primary defence; the
// sweep above is the backstop.
//
// Bound free by construction, and it clears BEFORE first use, so a latch left by a monster that
// never finished its own setup is gone before anything can trip over it.
//
// NOTHING CAN RACE THIS: it runs from GiveMonsterTID, which has no delays. Do not move it behind a
// Delay -- that is what would create a window.
// One player's whole row. FlushDamageTicResidue skips slots with nobody in them, so a leaver's
// residue has to be dropped here or the next player to occupy that pnum inherits a stranded latch --
// which silently kills every ailment for that pair. One row, not 64.
void DropPlayerTicResidue(int pnum) {
	if(pnum < 0 || pnum >= MAXPLAYERS)
		return;

	int high = InformationInLevel[LEVELINFO_TID_MONSTER] + InformationInLevel[LEVELINFO_SKIPPEDMONSTERTID];
	if(high > DND_MAX_MONSTERS)
		high = DND_MAX_MONSTERS;

	for(int i = 0; i < high; ++i) {
		if(IsDamageTicRunning(pnum, i) || PlayerDamageTic[pnum].total[i]) {
			ClearDamageTicRunning(pnum, i);
			PlayerDamageTic[pnum].total[i] = 0;
			ReleaseMixedTicSlot(pnum, i);
		}
	}
}

void DropMonsterTicResidue(int m_id) {
	// this indexes a global array; a junk id would corrupt whatever sits at that offset
	if(m_id < 0 || m_id >= DND_MAX_MONSTERS)
		return;

	int p;
	int n = InformationInLevel[LEVELINFO_ACTIVEPLAYERCOUNT];

	for(int i = 0; i < n; ++i) {
		p = InformationInLevel[LEVELINFO_ACTIVEPLAYERS + i];

		if(IsDamageTicRunning(p, m_id) || PlayerDamageTic[p].total[m_id]) {
			ClearDamageTicRunning(p, m_id);
			PlayerDamageTic[p].total[m_id] = 0;
			ReleaseMixedTicSlot(p, m_id);
		}
	}
}

// ============================================================================
//  DEAD AILMENT TICS
//
//  An ailment that cannot damage its victim must not keep holding that victim's ailment slot. The
//  loop keeps the timer non-zero, every application path reads that as "already ailed, just
//  refresh", and the monster burns or bleeds forever taking nothing. That is the "on fire and
//  taking no damage" report AND the "some monsters cannot bleed" one, in three loops.
//
//  Fire creatures are the worst case, immune BY DESIGN: InitMonsterResists gives DND_FIRECREATURE
//  DND_IMMUNITY_FACTOR plus a level bonus, so a level 90 one sits at 165.
//
//  OUTCOME BASED on purpose -- predicting it off the resist tables would duplicate FactorResists
//  and the penetration rules on top of it. Two consecutive dead tics, not one, so a single
//  transient cannot cancel a burn that was about to start working.
// ============================================================================
#define DND_AILMENT_DEADTICS 2

bool IsAilmentTicWasted(int victim, int dealt) {
	if(dealt > 0)
		return false;

	// TRANSIENT ABSORBERS -- each eats a whole tic then goes away on its own, so a dead tic under one
	// means "wait", not "give up". A standing fortify pool is a shield draining, not immunity.
	if(CheckActorInventory(victim, "MonsterFortifyCount"))
		return false;

	// Blocking and invulnerability are timed states (Hellsmith raises DND_ISBLOCKING in its charge
	// and pain states). HandleDamageDeal bails on both before FactorResists, so they leave no trace.
	if(HasMonsterTrait(victim - DND_MONSTERTID_BEGIN, DND_ISBLOCKING) || CheckFlag(victim, "INVULNERABLE"))
		return false;

	return true;
}

// we use this as a bitfield -- 64 players => 2 ints
// stores player weapon crit state
int PlayerDamageCritState[MAXWEPS][2];
int PlayerDamageCritLock[MAXWEPS][2];
#define CRIT_CLEAR_WAIT_TIME 2 // +1 tics added on top

bool GetPlayerWeaponCritState(int pnum, int wepid) {
	if(pnum > 31)
		return PlayerDamageCritState[wepid][1] & (1 << (pnum - 32));
	return PlayerDamageCritState[wepid][0] & (1 << pnum);
}

void SetPlayerWeaponCritState(int pnum, int wepid) {
	if(pnum > 31)
		PlayerDamageCritState[wepid][1] |= (1 << (pnum - 32));
	PlayerDamageCritState[wepid][0] |= (1 << pnum);
}

void UnsetPlayerWeaponCritState(int pnum, int wepid) {
	if(pnum > 31)
		PlayerDamageCritState[wepid][1] &= ~(1 << (pnum - 32));
	PlayerDamageCritState[wepid][0] &= ~(1 << pnum);
}

bool GetPlayerWeaponCritLock(int pnum, int wepid) {
	if(pnum > 31)
		return PlayerDamageCritLock[wepid][1] & (1 << (pnum - 32));
	return PlayerDamageCritLock[wepid][0] & (1 << pnum);
}

void LockPlayerCritState(int pnum, int wepid) {
	if(pnum > 31)
		PlayerDamageCritLock[wepid][1] |= (1 << (pnum - 32));
	PlayerDamageCritLock[wepid][0] |= (1 << pnum);
}

void UnlockPlayerCritState(int pnum, int wepid) {
	if(pnum > 31)
		PlayerDamageCritLock[wepid][1] &= ~(1 << (pnum - 32));
	PlayerDamageCritLock[wepid][0] &= ~(1 << pnum);
}

void HandleMonsterDeathConfirm(int tid, int dmg) {
	if(GetActorProperty(tid, APROP_HEALTH) <= dmg)
		GiveActorInventory(tid, "MonsterKilledByPlayer", 1);
}

// All resists uniformly follow same factors.
//
// Renamed from ApplyPlayerResist on purpose. It used to take an ATTRIBUTE ID and now takes a
// DND_PRESIST_* INDEX. The signature is otherwise identical, so a call site left holding
// INV_DMGREDUCE_PHYS would have compiled happily and indexed vals[] 77 entries past the resist run.
// Renaming was the only way to make every one of them fail loudly instead.
int ApplyPlayerDamageResist(int pnum, int dmg, int res_id, int bonus = 0) {
	//printbold(s:"called resist check for ", d:dmg, s:" with res id ", d:res_id);
	int unity = 1.0 * PlayerModData[pnum].vals[PSTAT_EX_UNITY_RES_BONUS] * GetUnity() / DND_UNITY_DIVISOR;
	int temp = 	bonus + 
				PlayerModData[pnum].vals[PSTAT_RESIST_ALL] +
				unity +
				DND_PLAYER_RESIST_REDUCE * (GetLevel() / DND_PLAYER_WEAKEN_LEVELS);

	// BUGFIX, and the reason the untyped case needed a name of its own. Damage matching none of the
	// type flags used to arrive here as res_attribute 0 -- and attribute id 0 is INV_HP_INCREASE, so
	// the flat max HP bonus of the player was summed in as a resistance against untyped damage, and
	// grew with every HP mod worn. Everything keyed on a type is now gated on the type being known.
	if(res_id != DND_PRESIST_NONE) {
		temp += PlayerModData[pnum].vals[PSTAT_RESIST_BASE + res_id];

		switch(res_id) {
			case DND_PRESIST_ELEM:
				temp += pbuffs[pnum].buff_net_values[BUFF_ELEMENTALRESIST].additive;
			break;
			case DND_PRESIST_ENERGY:
				temp += pbuffs[pnum].buff_net_values[BUFF_ENERGYRESIST].additive;
			break;
			case DND_PRESIST_PHYS:
				temp += pbuffs[pnum].buff_net_values[BUFF_PHYSRESIST].additive;
			break;
			case DND_PRESIST_MAGIC:
				temp += pbuffs[pnum].buff_net_values[BUFF_MAGICRESIST].additive;
			break;
		}
	}

	unity = PlayerModData[pnum].vals[PSTAT_EX_RESPERESHIELD];
	if(unity)
		temp += DND_RES_PER_PRISMGUARD * (CheckInventory("EShieldAmount") / unity);

	if(!temp)
		return dmg;
	
	// roll damage up
	temp = ApplyResistCap(pnum, temp) + 0.05;
	
	return dmg * ((100.0 - temp) >> 16) / 100;
}

// One row per player resist attribute. DND_LOWESTRESIST_ENTRIES below MUST stay equal to this row
// count -- the loop used to read i < 7 against a 4 row table, three rows past the end. The junk ids
// it picked up out there landed in the unused part of the attribute id space, so every one of them
// read back 0: the running minimum collapsed to 0 for every player, and the garbage damage type
// flags got OR-ed into the result. INV_EX_DAMAGELOWESTTAKENASPHYS therefore fired on the wrong
// damage types, for players whose real lowest resist was nowhere near zero.
#define DND_LOWESTRESIST_ENTRIES 4

int GetLowestResist(int pnum) {
	static int res_ids[DND_LOWESTRESIST_ENTRIES][2] = {
		{ DND_PRESIST_PHYS, DND_DAMAGETYPEFLAG_PHYSICAL },
		{ DND_PRESIST_MAGIC, DND_DAMAGETYPEFLAG_MAGICAL },
		{ DND_PRESIST_ENERGY, DND_DAMAGETYPEFLAG_ENERGY },
		{ DND_PRESIST_ELEM, DND_DAMAGETYPEFLAG_FIRE | DND_DAMAGETYPEFLAG_ICE | DND_DAMAGETYPEFLAG_LIGHTNING | DND_DAMAGETYPEFLAG_POISON }
	};

	int val = bcs::INT_MAX;
	int min_type = 0;
	for(int i = 0; i < DND_LOWESTRESIST_ENTRIES; ++i) {
		int temp = PlayerModData[pnum].vals[PSTAT_RESIST_BASE + res_ids[i][0]];
		if(val >= temp) {
			// not yet established, reset
			if(val != temp)
				min_type = 0;
			val = temp;
			min_type |= res_ids[i][1];
		}
	}

	return min_type;
}

bool AdjustDamageRetrievePointers(int flags, bool crit_check = false, int wepid = -1) {
	int temp;
	bool res = false;
	//printbold(s:"prev score? ", d:GetActorProperty(0, APROP_SCORE));
	if(flags & DND_WDMG_USETARGET) { // use target
		// hopefully no projectile uses score
		if(!GetActorProperty(0, APROP_SCORE)) {
			temp = GetActorProperty(0, APROP_TARGETTID);
			SetActorProperty(0, APROP_SCORE, temp);
			
			/*res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
			SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);*/
			
			SetActivator(0, AAPTR_TARGET);
		}
	}
	else if(flags & DND_WDMG_USEMASTER) {
		if(flags & DND_WDMG_SETMASTER) {
			temp = GetActorProperty(0, APROP_MASTERTID);
		
			/*res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
			SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);*/
		
			// this is a hack
			SetPointer(AAPTR_TARGET, temp);
		}
		if(!GetActorProperty(0, APROP_SCORE)) {
			temp = GetActorProperty(0, APROP_TARGETTID);
		
			SetActorProperty(0, APROP_SCORE, temp);
			
			/*res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
			SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);*/
			
			SetActivator(GetActorProperty(0, APROP_MASTERTID));
		}
	}
	else if(flags & DND_WDMG_USETRACER) {
		temp = GetActorProperty(0, APROP_TRACERTID);
		
		/*res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
		SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);*/
	
		SetActivator(0, AAPTR_TRACER);
	}
	else if(flags & DND_WDMG_USETARGETSMASTER) {
		temp = ACS_NamedExecuteWithResult("DnD Get Master of Target");
		if(!GetActorProperty(0, APROP_SCORE))
			SetActorProperty(0, APROP_SCORE, temp);
		
		/*res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
		SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);*/
		
		SetActivator(temp);
	}

	// printbold(s:"owner ", d:ActivatorTID(), s: " ", d:GetActorProperty(0, APROP_SCORE));
	// no owner can potentially mean this was a reflected projectile, we can manipulate this a bit
	if(!ActivatorTID()) {
		GiveInventory("MarkAsReflected", 1);
		SetActivator(GetActorProperty(0, APROP_SCORE));
	}
	
	return res;
}

void HandleOnHitEffects(int owner) {
	if(HasActorMasteredPerk(owner, STAT_DED)) {
		if(!CheckActorInventory(owner, "DnD_DeadlinessMasteryWindow")) {
			// didn't have before, reset (or had some, we dont care, it needs to be reset)
			// reset to 1, as this is a hit by itself
			SetActorInventory(owner, "DnD_DeadlinessMasteryCounter", 1);
			GiveActorInventory(owner, "DnD_DeadlinessMasteryWindow", 1);
		}
		else {
			GiveActorInventory(owner, "DnD_DeadlinessMasteryCounter", 1);
			if(CheckActorInventory(owner, "DnD_DeadlinessMasteryCounter") == DND_MASTERY_DEADLINESSCOUNTER)
				GiveActorInventory(owner, "DnD_GuaranteeCrit_FromDeadliness", 1);
		}
	}
}

int BigNumberFormula(int dmg, int f) {
	int p = PowersOf10[(digitcount(dmg) + 1) / 2];
	int wepid = dmg % p;
	int temp = 0;
	dmg /= p;
	
	// get rid of 0.9999 crap
	f = f * 100 + 0.001;
	f >>= 16;
	
	dmg *= f;
	temp = (dmg % 100) * p / 100;
	dmg /= 100;
	dmg *= p;
	
	wepid *= f;
	wepid /= 100;
	
	dmg += wepid + temp;

	return dmg;
}

// for now does nothing per weapon, later add new corruptions perhaps
int GetPlayerIgniteAddedDmg(int pnum, int wepid, int added_dmg) {
	return added_dmg * DND_ADDEDIGNITE_FACTOR / 100;
}

// Factors in generic DOT percentages to a base damage, use for weapons that do DOT on their own!
int FactorDOT(int pnum, int dmg, int percent_increase = 0) {
	// flat portion
	dmg += PlayerModData[pnum].vals[PSTAT_DOT_FLAT];

	// dot %
	dmg = dmg * (100 + PlayerModData[pnum].vals[PSTAT_DOT_INCREASED] + percent_increase) / 100;
		
	// dot multi
	dmg = dmg * (100 + PlayerModData[pnum].vals[PSTAT_DOTMULTI]) / 100;
	
	return dmg;
}

// set pointers appropriately beforehand!
// uses DND_DAMAGEFLAG for flags
int RetrieveWeaponDamage(int pnum, int wepid, int dmgid, int damage_category, int flags, int isSpecial, bool allow_components = false) {
	// do not lose the weaponid on special ammo -- normally its DMG_ID & (wepid << 16) but special ammo just have the id of the special ammo instead of dmg_id
	// add +1 because flechette is id 0
	// correction code moved within main code block
	
	//printbold(s:"retrieved acc ", d:GetActorProperty(0, APROP_ACCURACY));

	int res = ScaleCachedDamage(wepid, pnum, dmgid, damage_category, flags, isSpecial, allow_components);
	
	// special weapons checks -- these are added on top of everything else as the last thing, before crits
	// they are also dynamic and cant be cached...
	if(wepid == DND_WEAPON_LIGHTNINGGUN)
		res = res * (100 + DND_LIGHTNINGGUN_DMGPERSTACK * CheckInventory("LightningStacks")) / 100;
	else if(wepid == DND_WEAPON_DUSKBLADE)
		res = res * (100 + DND_DUSKBLADE_DMGPERSTACK * CheckInventory("SwordHitCharge")) / 100;
		
	return res;
}

int ApplyNonWeaponBaseDamageBonus(int tid, int dmg, int damage_type, int flags) {
	// we need to assign a damage category to this first
	//printbold(d:damage_type, s: " ", d:flags);
	int damage_category = GetDamageCategory(damage_type, flags);
	int damage_category_flags = 0;
	int pnum = tid - P_TIDSTART;
	
	//printbold(s:"add ", d:MapDamageCategoryToFlatBonus(pnum, damage_category, damage_category_flags));
	dmg += MapDamageCategoryToFlatBonus(pnum, damage_category, flags);
	
	// overall percentage bonuses -- this is basically ScaleCachedDamage but unwrapped, we need to rewrite these into a common function that just retrieves the overall bonus factor to multiply with!
	// uncached path, so the buff term is read live right here
	int factor = 100 + GetPlayerPercentDamage(pnum, -1, damage_category, flags) + GetPlayerBuffIncreasedDamage(pnum) + GetPlayerAccuracyDamageBonus(pnum, -1);
	
	// apply flat health to damage conversion if player has any
	int temp = PlayerModData[pnum].vals[PSTAT_EX_PHYSDAMAGEPER_FLATHEALTH];
	if((damage_category == DND_DAMAGECATEGORY_MELEE || damage_category == DND_DAMAGECATEGORY_BULLET) && temp)
		factor += GetFlatHealthDamageFactor(temp);
		
	temp = PlayerModData[pnum].vals[PSTAT_EX_DMGINCREASE_LIGHTNING];
	if(temp && IsLightningDamage(damage_type))
		factor += temp;

	// THESE ARE MULTIPLICATIVE STACKING BONUSES BELOW -- HAVE KEYWORD: MORE
	// quest or accessory bonuses	
	// is occult (add demon bane bonus)
	
	if(damage_category == DND_DAMAGECATEGORY_OCCULT)
		factor = factor * (100 + DND_DEMONBANE_GAIN * (!!IsAccessoryEquipped(tid, DND_ACCESSORY_DEMONBANE))) / 100;
	
	// % more damage from charms -- already contains 100 in it as it's a multiplicative mod -- its also fixed!
	if(!(flags & DND_DAMAGEFLAG_ISSPELL)) {
		temp = (PlayerModData[pnum].vals[PSTAT_DAMAGEPERCENT_MORE] * 100) >> 16;
		if(temp)
			factor = factor * (100 + temp) / 100;
	}
		
	// if we had a factor of 0, dont bother here
	if(factor <= 0)
		return 0;
		
	//printbold(s:"dmg factor mult by ", d:factor, s: " base dmg: ", d:dmg, s: " end result: ", d:dmg * factor / 100);
		
	if(dmg < bcs::INT_MAX / factor) {
		dmg *= factor;
		dmg /= 100;
		// no longer fixed
		//dmg >>= 16;
	}
	else {
		// beyond this point wepid doesnt matter so use that instead
		dmg = bcs::INT_MAX;//BigNumberFormula(dmg, factor);
	}
	
	return dmg;
}

// Computes everything this hit deals on top of its own damage type -- flat damage added as another
// type, and whatever conversion moved out of the weapon's own -- and stages it for the emit point.
//
// The two sources are summed per destination rather than kept apart: they land as the same damage
// type, so emitting them separately would pay for two rounds of resists and two accumulator entries
// without changing the number. They are computed apart because their amounts are of different kinds
// and they take different flat effectiveness treatment.
int StageDamageComponents(int pnum, int slot, int dmgid, int wepid, int damage_category, int base) {
	ResetComponentStage(pnum);

	int mask = GetCachedComponentMask(pnum, slot, dmgid);
	if(!mask)
		return 0;

	// the generic pools apply to every component because they are true of the weapon whatever it
	// fires; the per-path typed pools are already folded into the cached inc below
	int generic = 100 + GetCachedPlayerIncreased(pnum, slot, dmgid) +
				  GetPlayerBuffIncreasedDamage(pnum) + GetPlayerAccuracyDamageBonus(pnum, wepid);
	int more_gen = GetCachedPlayerMorePacked(pnum, slot, dmgid);
	int flat_eff = GetCachedPlayerFlatFactor(pnum, slot, dmgid);
	int total = 0;
	int dmg, factor, part;

	for(int cat = DND_DAMAGECATEGORY_BEGIN; cat < DND_DAMAGECATEGORY_END; ++cat) {
		if(!(mask & (1 << cat)))
			continue;

		dmg = 0;

		// the share of the weapon's own base that conversion moved into this category
		part = GetCachedConvWeaponFrac(pnum, slot, dmgid, cat);
		if(part > 0 && base > 0) {
			factor = ApplyPackedMultiplier(generic + GetCachedConvWeaponInc(pnum, slot, dmgid, cat), more_gen) * DND_COMPFACTOR_SCALE;
			// the fraction joins the FACTOR rather than the base, so the divide still lands once
			factor = MulPercent_Round(factor, part, DND_CONVFRAC_ONE);
			if(factor > 0)
				dmg += MulPercent_Round(base, factor, 100 * DND_COMPFACTOR_SCALE);
		}

		// flat damage added as this type, which conversion may have moved here as well
		part = GetCachedConvAddedAmount(pnum, slot, dmgid, cat);
		if(part > 0) {
			factor = ApplyPackedMultiplier(generic + GetCachedConvAddedInc(pnum, slot, dmgid, cat), more_gen) * DND_COMPFACTOR_SCALE;
			// added flat is stored raw, so weapon effectiveness applies here -- and joins the factor
			// for the same reason: a shotgun at 25% would floor "+3 cold" to nothing on its own
			if(flat_eff != 100)
				factor = MulPercent_Round(factor, flat_eff);
			if(factor > 0)
				// two steps rather than one giant divisor: DND_CONVFRAC_ONE and 100 x the factor
				// scale multiply out past what MulPercent's split can carry, and the shift is exact
				dmg += (MulPercent_Round(part, factor, 100 * DND_COMPFACTOR_SCALE) + (DND_CONVFRAC_ONE >> 1)) >> DND_CONVFRAC_BITS;
		}

		if(dmg > 0) {
			AddComponentStageValue(pnum, cat, dmg);
			total += dmg;
		}
	}

	return total;
}

// use only flags with DND_WDMG header here!!!
// NOTE: DO NOT FACTOR ANY DOT MULTIPLIER IN HERE!
// isSpecial is id of the special ammo + 1
// special ammo replaces dmgid 0 of the weapon in cache, so everytime we switch special ammo type we must force damage cache recalc
//
// allow_components is opt-in because only a caller that goes on to call DealDamageComponents may take
// it: the return value then covers the converted and added portions too, and a caller that never
// emits them would deal the lot as the weapon's own type, on the wrong resists.
int ScaleCachedDamage(int wepid, int pnum, int dmgid, int damage_category, int flags, int isSpecial, bool allow_components = false) {
	// we don't cache special ammo damage
	int dmg = 0;
	// One lookup per shot. The cache is keyed by a per-player slot rather than by weapon id, so
	// resolving inside each accessor would repeat this eight times on the hot path.
	int slot = ResolveWeaponCacheSlot(pnum, wepid);
	int temp,  pct_tmp = 0;
	int tid = pnum + P_TIDSTART;

	// The base roll is the one thing in the slot that does not rebuild itself, and the slot it was
	// written into is not necessarily the slot this weapon holds now: the eviction clock hand hands
	// slots out on a first-come basis and pays no attention to what the player is currently holding,
	// so a proc, a dot tick or a projectile still in the air resolving a slot for its OWN weapon can
	// take the slot out from under the weapon in your hands. It then fires for zero -- and stays that
	// way until a weapon swap pushes the base back in, which is exactly how this reads in play.
	//
	// The weapon table is pure constants keyed on wepid, so pull it instead of waiting for the push.
	// Runs once per handover, not per shot. Passing pnum + 1 tells the script this is a rebuild: it
	// takes the player number from the argument rather than the activator (this block is not
	// guaranteed to run with the player as activator) and skips the weapon-swap bookkeeping, which
	// belongs to the raise and would otherwise fire against whatever the activator happens to be.
	if(!IsWeaponCacheBaseFilled(pnum, slot)) {
		ACS_NamedExecuteWithResult("DnD Weapon Damage Cache", wepid, pnum + 1);
		MarkWeaponCacheBaseFilled(pnum, slot);
	}

	// get the damage
	if(!isSpecial)
		temp = GetCachedPlayerDamage(pnum, slot, dmgid);
	else {
		// special ammo damage
		temp = GetSpecialAmmoDamage(isSpecial - 1, dmgid);
	}
		
	// check if we have a random range cached -- special ammo types dont use this
	int range = GetCachedPlayerRandomRange(pnum, slot, dmgid);
	if(range > 1 && !isSpecial)
		dmg += temp * random(range & 0xFFFF, range >> 16);
	else // no rng, so just set it to temp
		dmg = temp;

	bool is_melee_mastery_exception = (IsMeleeWeapon(wepid) || (flags & DND_DAMAGEFLAG_COUNTSASMELEE)) && HasMasteredPerk(STAT_BRUT);

	// only store scaling factors here for later use, no modifying damage in this block
	// damage modifications are done at the end
	if(PlayerDamageNeedsCaching(pnum, slot, dmgid)) {
		// add potential shotgun flat damage
		temp = (!!IsBoomstick(wepid)) * PlayerModData[pnum].vals[PSTAT_EX_FLATPERSHOTGUNOWNED] * CountShotgunWeaponsOwned();
		
		// add flat damage bonus mapping talent name to flat bonus type
		temp += MapDamageCategoryToFlatBonus(pnum, damage_category, flags);
		
		ClearCache(pnum, slot, dmgid);
		
		if(flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME)
			temp += PlayerModData[pnum].vals[PSTAT_DOT_FLAT];

		// special weapon type checks
		if(IsTechWeapon(wepid)) {
			temp += PlayerModData[pnum].vals[PSTAT_FLAT_TECH];
			pct_tmp += PlayerModData[pnum].vals[PSTAT_TECH_PERCENT];

			// Cyborg perk 1: +33% MORE on tech weapons. This is weapon-conditional, so
			// it belongs in the per-weapon cache rather than the buff layer -- and perk
			// spend already calls ForcePlayerDamageCaching, so it has an invalidation
			// edge. Actor-based lookup because the cached block is not guaranteed to
			// run with the player as activator.
			if(HasActorClassPerk_Fast(tid, DND_PLAYER_CYBORG, DND_CLASSPERK_1))
				InsertCacheFactor(pnum, slot, dmgid, 100 + DND_CYBERNETIC_FACTOR, false);
		}

		if(IsHandgun(wepid)) {
			temp += PlayerModData[pnum].vals[PSTAT_FLAT_HANDGUN];
			pct_tmp += PlayerModData[pnum].vals[PSTAT_HANDGUN_PERCENT];
		}

		if(IsBoomstick(wepid)) {
			temp += PlayerModData[pnum].vals[PSTAT_FLAT_SHOTGUN];
			pct_tmp += PlayerModData[pnum].vals[PSTAT_SHOTGUN_PERCENT];
		}

		if(IsAutomaticWeapon(wepid)) {
			temp += PlayerModData[pnum].vals[PSTAT_FLAT_AUTOMATIC];
			pct_tmp += PlayerModData[pnum].vals[PSTAT_AUTOMATIC_PERCENT];
		}

		if(IsPrecisionWeapon(wepid)) {
			temp += PlayerModData[pnum].vals[PSTAT_FLAT_PRECISION];
			pct_tmp += PlayerModData[pnum].vals[PSTAT_PRECISION_PERCENT];
		}

		if(IsArtilleryWeapon(wepid)) {
			temp += PlayerModData[pnum].vals[PSTAT_FLAT_ARTILLERY];
			pct_tmp += PlayerModData[pnum].vals[PSTAT_ARTILLERY_PERCENT];
		}

		if(IsMagicalWeapon(wepid)) {
			temp += PlayerModData[pnum].vals[PSTAT_FLAT_MAGIC];
			pct_tmp += PlayerModData[pnum].vals[PSTAT_MAGIC_PERCENT];
		}
		
		CachePlayerFlatDamage(pnum, temp, slot, dmgid);

		// the weapon class percents gathered above are true whatever type this weapon deals
		if(pct_tmp)
			InsertCacheFactor(pnum, slot, dmgid, pct_tmp, true);

		// include enhancement orb bonuses
		temp = GetPlayerWeaponEnchant(pnum, wepid);
		if(temp)
			InsertCacheFactor(pnum, slot, dmgid, temp, true);

		// factor dot % increase if this is a dot attack
		if(flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME)
			InsertCacheFactor(pnum, slot, dmgid, PlayerModData[pnum].vals[PSTAT_DOT_INCREASED], true);

		// Everything keyed on the damage CATEGORY goes into the per-category pools instead of the
		// weapon pools above. That separation is the whole point: a component of a different element
		// can then take this weapon's increases without also taking "increased physical damage".
		//
		// Every category is filled in one pass rather than lazily, because this block already runs
		// only once per weapon raise and a partial fill would need its own per-category dirty state.
		// Nine categories of a few array reads is nothing next to the chain above it.
		temp = PlayerModData[pnum].vals[PSTAT_EX_PHYSDAMAGEPER_FLATHEALTH];
		bool has_demonbane = IsAccessoryEquipped(tid, DND_ACCESSORY_DEMONBANE);
		int lightning_inc = PlayerModData[pnum].vals[PSTAT_EX_DMGINCREASE_LIGHTNING];
		int phys_cat = IsMeleeWeapon(wepid) ? DND_DAMAGECATEGORY_MELEE : DND_DAMAGECATEGORY_BULLET;
		int typed;

		BuildPlayerConversionTable(pnum);

		for(int cat = DND_DAMAGECATEGORY_BEGIN; cat < MAX_DAMAGE_CATEGORIES; ++cat) {
			// stat attunement -- category enters only through the melee test
			typed = GetStatAttunementBonus(pnum, wepid, cat == DND_DAMAGECATEGORY_MELEE || is_melee_mastery_exception);

			// damage type percentage bonuses
			// last one is for ghost hit power, we reduce its power by a factor
			typed += GetPlayerPercentDamage(pnum, wepid, cat, flags);
			if(cat != DND_DAMAGECATEGORY_MELEE && is_melee_mastery_exception)
				typed += MapDamageCategoryToPercentBonus(pnum, DND_DAMAGECATEGORY_MELEE, flags); // prevent double dipping

			// apply flat health to damage conversion if player has any
			if(temp && (cat == DND_DAMAGECATEGORY_MELEE || cat == DND_DAMAGECATEGORY_BULLET))
				typed += GetFlatHealthDamageFactor(temp);

			// special damage increase attributes -- usually obtained by means of charms.
			// This is an increase to LIGHTNING DAMAGE, so it keys on the category being dealt. It
			// used to be gated on IsWeaponLightningType(wepid), which was the same thing back when a
			// weapon only ever dealt one type -- now it would hand the bonus to a lightning weapon's
			// physical component and withhold it from an added lightning component on anything else.
			if(lightning_inc && cat == DND_DAMAGECATEGORY_LIGHTNING)
				typed += lightning_inc;

			// these all land in one additive pool, so summing before the insert is the same result
			if(typed)
				InsertCacheFactor_Typed(pnum, slot, dmgid, cat, typed, true);

			// MULTIPLICATIVE -- quest or accessory bonuses
			// is occult (add demon bane bonus)
			if((flags & DND_DAMAGEFLAG_COUNTSASMAGIC || cat == DND_DAMAGECATEGORY_OCCULT) && has_demonbane)
				InsertCacheFactor_Typed(pnum, slot, dmgid, cat, 100 + DND_DEMONBANE_GAIN, false);

			// Flat damage added as this type. Physical is the one added type with two categories,
			// and this weapon only ever deals one of them -- resolving it here rather than at the
			// consumer keeps a melee weapon's added physical on the melee attunement and stops the
			// other category from handing out a second, duplicate component.
			//
			// Parked RAW: the conversion walk below takes it as its starting vector, because added
			// physical converts exactly the way the weapon's own physical does. Soul is past the end
			// of the ladder and takes no part in any of it.
			if(cat < DND_DAMAGECATEGORY_END) {
				typed = GetPlayerAddedFlatDamage(pnum, cat);
				if(typed && (cat == DND_DAMAGECATEGORY_MELEE || cat == DND_DAMAGECATEGORY_BULLET) && cat != phys_cat)
					typed = 0;

				SeedRawAddedFlat(pnum, slot, dmgid, cat, typed);
			}
		}

		// Both conversion trees, now that every typed pool they walk is final. The generic pools
		// below are deliberately NOT in scope here -- they apply to every component alike and join
		// at request time, so folding them in would freeze the buff half of them into the cache.
		ResolveDamageComponents(pnum, slot, dmgid, damage_category);

		// THESE ARE MULTIPLICATIVE STACKING BONUSES BELOW -- HAVE KEYWORD: MORE
		// add other multiplicative factors below
		
		// % more damage from charms -- already contains 100 in it as it's a multiplicative mod
		temp = PlayerModData[pnum].vals[PSTAT_DAMAGEPERCENT_MORE];
		if(temp)
			InsertCacheFactor_Fixed(pnum, slot, dmgid, 1.0 + temp);
			
		// % more / less damage from wepmod or orbs
		temp = GetWeaponModValue(pnum, wepid, WEP_MOD_DMG);
		if(temp)
			InsertCacheFactor_Fixed(pnum, slot, dmgid, temp);
			//InsertCacheFactor(pnum, slot, dmgid, temp, false);
		
		// New multipliers go through the stat cache: infrequent ones as cached
		// factors, frequent ones as buffs.

		MarkCachingComplete(pnum, slot, dmgid);
		
		//printbold(s:"pre-scale: ", d:temp);
	}

	// Get the cached flat dmg and factor and apply them both
	temp = GetCachedPlayerFlatDamage(pnum, slot, dmgid);
	if(isSpecial && isSpecial - 1 == SSAM_FLECHETTE)
		temp /= 3;

	// IsTechWeapon is a property-table bit, the inventory lookup is a name-based native
	// call -- test the cheap half first so the native one is skipped on most shots
	if(IsTechWeapon(wepid)) {
		range = CheckActorInventory(tid, "Cyborg_InstabilityStack");
		if(range == DND_MAXCYBORG_INSTABILITY)
			temp += DND_DMG_PER_INSTABILITY * range;
	}
	
	// isSpecial isn't used or kept track of below here, so re-use
	// 66% effectiveness of damage scaling on tracer -- dmgid is 1 on tracers
	isSpecial = (IsSuperWeapon(wepid)) && (wepid == DND_WEAPON_BFG32768 || wepid == DND_WEAPON_BFG6000) && dmgid == 1;
	if(isSpecial) {
		temp *= 2;
		temp /= 3;
	}
	
	// add flat bonus here
	dmg += temp;

	// Conversion and added damage components, computed HERE because this is the only point where the
	// rolled base still exists. Attacks only, exactly like added flat damage was: a DOT tick has no
	// emit point of its own, so its components would never be dealt and would be re-added every tick.
	int extra = 0;
	if(allow_components) {
		if(flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME)
			ResetComponentStage(pnum);
		else {
			extra = StageDamageComponents(pnum, slot, dmgid, wepid, damage_category, dmg);

			// whatever conversion moved out is no longer the weapon's own type to deal. Taken off the
			// base rather than the result so each half meets its own category's percent layers.
			range = GetPlayerConversionRowTotal(pnum, damage_category);
			if(range)
				dmg -= MulPercent_Exact(dmg, range);
		}
	}
	
	// The weapon layer's "increased" pool, kept SEPARATE from its "more" product so
	// the buff layer's own increased can join this same pool at request time. Fusing
	// them into one number is what let a buff percent freeze into the weapon cache.
	// THE STALENESS FIX: the weapon layer's cached "increased" and the buff layer's
	// live "increased" join the SAME additive pool, here, at request time. The buff
	// term is read fresh on every shot while the weapon half stays cached, so a buff
	// takes effect the instant it lands and stops the instant it expires -- without
	// buff churn ever invalidating the per-weapon cache.
	// The weapon's generic pool, its pool for THIS category, and the live buff term all join here.
	// Splitting generic from typed changes where the terms are stored, not which pool they land in.
	temp = 100 + GetCachedPlayerIncreased(pnum, slot, dmgid) + GetCachedPlayerIncreasedTyped(pnum, slot, dmgid, damage_category) +
		   GetPlayerBuffIncreasedDamage(pnum) + GetPlayerAccuracyDamageBonus(pnum, wepid);

	// Collapse the two layers into ONE integer percent before touching dmg.
	// Scaling dmg by the increased pool first would quantize it to an integer and
	// then amplify that error by the whole more-product -- at dmg 5 with a 17x
	// factor that is a 10% loss. One truncation at the end instead of two.
	// merge the generic and per-category products BEFORE applying -- applying them in sequence
	// truncates twice, which is the exact error the comment above is about
	temp = ApplyPackedMultiplier(temp, CombinePackedMultipliers(
		GetCachedPlayerMorePacked(pnum, slot, dmgid),
		GetCachedPlayerMorePackedTyped(pnum, slot, dmgid, damage_category)
	));

	if(isSpecial) {
		// the tracer scales the bonus of the COMBINED factor, which is why the
		// collapse above has to happen first
		temp = ScaleFactorBonus(temp, 2, 3);
	}

	// if we had a factor of 0, dont bother here
	if(temp <= 0)
		dmg = 0;
	else
		// exact, and saturates only when the true product really exceeds INT_MAX
		dmg = MulPercent_Exact(dmg, temp);

	// The emit point splits the FINAL number back into these shares, so the primary has to be part of
	// the total it divides by -- and it has to be the primary as computed HERE, tracer bonus and all,
	// or the ratio would quietly drift from what each piece actually earned.
	if(allow_components)
		CommitComponentStage(pnum, slot, dmgid, damage_category, dmg);

	return dmg + extra;
}

// The concrete damage type an added component of this category is dealt as -- resists, ailments and
// the buff layer are all keyed on the type, not the category.
int MapDamageCategoryToDamageType(int category) {
	switch(category) {
		case DND_DAMAGECATEGORY_MELEE:
		return DND_DAMAGETYPE_MELEE;

		case DND_DAMAGECATEGORY_BULLET:
		return DND_DAMAGETYPE_PHYSICAL;

		case DND_DAMAGECATEGORY_ENERGY:
		return DND_DAMAGETYPE_ENERGY;

		case DND_DAMAGECATEGORY_OCCULT:
		return DND_DAMAGETYPE_OCCULT;

		case DND_DAMAGECATEGORY_FIRE:
		return DND_DAMAGETYPE_FIRE;

		case DND_DAMAGECATEGORY_ICE:
		return DND_DAMAGETYPE_ICE;

		case DND_DAMAGECATEGORY_LIGHTNING:
		return DND_DAMAGETYPE_LIGHTNING;

		case DND_DAMAGECATEGORY_POISON:
		return DND_DAMAGETYPE_POISON;
	}
	return DND_DAMAGETYPE_PHYSICAL;
}

// Deals every part of this hit that is NOT the weapon's own damage type and returns what is left
// for the caller. Zero means the components finished the monster, or conversion took the whole hit.
//
// Each is its own damage instance on purpose: cold that misses cold resists and cannot chill is not
// cold. That buys each its own resists, penetration, mixed tic entry and ailment. Crit is inherited,
// not rolled -- they share one PlayerDamageTic total, so all-or-nothing falls out for free. The
// split is proportional against the staged total, since everything above scales the primary by
// type-agnostic factors the components must take too.
//
// ATTACKS ONLY. Never call this from a DOT tick, proc or spell.
int DealDamageComponents(int pnum, int shooter, int victim, int wepid, int dmgid, int category, int dmg, int flags, int actor_flags) {
	if(dmg <= 0 || wepid < 0)
		return dmg;

	int mask = GetComponentStageMask(pnum);
	if(!mask || !IsActorAlive(victim))
		return dmg;

	// Another hit slipped in between the staging and here and left its own numbers behind, so there
	// is nothing trustworthy to split by -- deal the lot as the weapon's own type rather than guess.
	// The same weapon re-entering is harmless and passes: only the RATIOS are read, and those are a
	// function of cached data, so its stage carries the same ones.
	int slot = LookupWeaponCacheSlot(pnum, wepid);
	if(slot == DND_WEPCACHE_NOSLOT || GetComponentStageKey(pnum) != MakeComponentStageKey(slot, dmgid, category))
		return dmg;

	int total = GetComponentStageTotal(pnum);
	if(total <= 0)
		return dmg;

	int part, dealt = 0;
	int last_cat = DND_CONV_NOSKIP;

	// The primary pushes when there is one. At full conversion there is not, so the first component
	// has to, or a fully converted hit would stop shoving anything.
	if(GetComponentStagePrimary(pnum) > 0)
		flags |= DND_DAMAGEFLAG_NOPUSH;
	else {
		// Nothing may survive as the weapon's own type when every point of it was converted. The
		// per-component shares each floor, so without this the leftover comes back as a sliver of
		// exactly the damage type the player converted away. The last component absorbs it instead.
		for(int c = DND_DAMAGECATEGORY_BEGIN; c < DND_DAMAGECATEGORY_END; ++c)
			if(mask & (1 << c))
				last_cat = c;
	}

	for(int cat = DND_DAMAGECATEGORY_BEGIN; cat < DND_DAMAGECATEGORY_END; ++cat) {
		if(!(mask & (1 << cat)))
			continue;

		if(cat == last_cat)
			part = dmg - dealt;
		else
			part = MulPercent_Exact(dmg, GetComponentStageValue(pnum, cat), total);

		if(part <= 0)
			continue;

		// taken off the primary's share BEFORE resists, so the pieces still sum to the whole hit
		dealt += part;

		// resists, accumulation and the ailment bookkeeping for this component's own type
		part = HandleDamageDeal(shooter, victim, part, MapDamageCategoryToDamageType(cat), wepid, flags, 0, 0, 0, actor_flags);
		flags |= DND_DAMAGEFLAG_NOPUSH;

		// and then actually apply it. The primary reaches the monster through the damage event's
		// return value; a component dealt from here has no event of its own, so HandleDamageDeal
		// would leave it accumulated for the damage number and the ailments while never taking a
		// single point of health. Special_NoPain is on the exception list, so this does not
		// re-enter the handler and re-scale.
		if(part > 0) {
			HandleMonsterDeathConfirm(victim, part);
			Thing_Damage2(victim, part, "Special_NoPain");
		}

		// Stop once there is nothing left to hurt. Unlike the primary, these are applied here and
		// now, so one can finish the monster off mid loop -- every remaining mask bit would then
		// scale, accumulate and swing at a corpse.
		if(!IsActorAlive(victim))
			return 0;
	}

	return dmg - dealt;
}

// there may be things that add + to cull % later
bool CheckCullRange(int source, int victim, int dmg) {
	int base = DND_CULL_BASEPERCENT;
	base += HasActorClassPerk_Fast(source, DND_PLAYER_DOOMGUY, 4) * DND_DOOMGUY_CULLBONUS;
	return GetActorProperty(victim, APROP_HEALTH) - dmg <= MulPercent_Exact(MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp, base);
}

bool CheckCullRangeVsPlayer(int source, int victim, int dmg) {
	int base = DND_CULL_BASEPERCENT_VS_PLAYER;
	return GetActorProperty(victim, APROP_HEALTH) - dmg <= CheckActorInventory(victim, "PlayerHealthCap") * base / 100;
}

void HandleChillEffects(int pnum, int victim) {
	// not ailment immune
	if(CheckAilmentImmunity(pnum, victim - DND_MONSTERTID_BEGIN, DND_FROSTBLOOD)) {
		// check health thresholds --- get missing health
		int hpdiff = MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp - GetActorProperty(victim, APROP_HEALTH);
		int stacks = CheckActorInventory(victim, "DnD_ChillStacks");
		int threshold = MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp * GetChillThreshold(pnum, stacks + 1) / 100;

		if(hpdiff >= threshold) {
			// add a new stack of chill and check for freeze
			if(!stacks) {
				GiveActorInventory(victim, "DnD_ChillStacks", 1);
				ACS_NamedExecuteWithResult("DnD Monster Chill", victim, pnum);
			}
			else if(stacks < DND_BASE_CHILL_CAP)
				GiveActorInventory(victim, "DnD_ChillStacks", 1);
			
			// freeze checks --- added freeze chance % increase -- unique boss is immune to freeze
			if(IsUniqueBossMonster(victim - DND_MONSTERTID_BEGIN))
				return;
			
			hpdiff = GetFreezeChance(pnum, CheckActorInventory(victim, "DnD_ChillStacks"));
			if(random(1, 100) <= hpdiff) {
				if(GetActorProperty(victim, APROP_HEALTH) > 0) {
					if(CheckFlag(victim, "BOSS"))
						stacks = DND_BASE_FREEZETIMER / 3;
					else
						stacks = DND_BASE_FREEZETIMER;
					
					// set freeze timer and run script
					if(!CheckActorInventory(victim, "DnD_FreezeTimer")) {
						GiveActorInventory(victim, "DnD_FreezeTimer", stacks);
						ACS_NamedExecuteWithResult("DnD Monster Freeze", victim);
					}
					else
						SetActorInventory(victim, "DnD_FreezeTimer", stacks);
				}
			}
		}
	}
}

void HandleBleedEffects(int pnum, int victim, int wepid, int overall_dmg) {
	if
	(
		CheckAilmentImmunity(pnum, victim - DND_MONSTERTID_BEGIN, DND_STONECREATURE) &&
		CheckBleedChance(pnum, wepid, victim)
	)
	{
		int amt = GetPlayerBleedTime(pnum);

		// only the refresh branch consults this now; it is no longer the ownership test
		int current_bleed_time = CheckActorInventory(victim, "DnD_BleedTimer");

		// Ownership is the SCRIPT REFCOUNT, never the timer -- same rule as HandleIgniteEffects.
		// Inventory outlives scripts, so a stale timer used to trap this in the refresh branch for
		// good. A script ending for ANY reason frees its count.
		if(!CheckActorInventory(victim, "DnD_BleedScripts")) {
			SetActorInventory(victim, "DnD_BleedTimer", amt);
			SetActorInventory(victim, "DnD_CurrentBleedDamage", overall_dmg);

			// Claim BEFORE launching: the first iteration runs inline, so a second application in the same
			// tic would otherwise start a second script on one timer.
			GiveActorInventory(victim, "DnD_BleedScripts", 1);
			ACS_NamedExecuteWithResult("DnD Monster Bleed (Player)", victim, wepid, overall_dmg);
		}
		else {
			SetActorInventory(victim, "DnD_BleedTimer", Max(amt, current_bleed_time));
			// update with higher damage
			if(overall_dmg > CheckActorInventory(victim, "DnD_CurrentBleedDamage"))
				SetActorInventory(victim, "DnD_CurrentBleedDamage", overall_dmg);
		}
	}
}

// HandleIgniteEffects lives further down, just under HandleNonWeaponDamageScale -- it has to price
// the tick it is applying, and that needs a function BCS has not parsed yet up here.

void HandleOverloadEffects(int pnum, int victim) {
	int temp;
	if
	(
		CheckAilmentImmunity(pnum, victim - DND_MONSTERTID_BEGIN, DND_INSULATED) &&
		random(1, 100) <= DND_BASE_OVERLOADCHANCE * (100 + PlayerModData[pnum].vals[PSTAT_OVERLOADCHANCE]) / 100 && IsActorAlive(victim)
	)
	{
		if(!CheckActorInventory(victim, "DnD_OverloadTimer")) {
			SetActorInventory(victim, "DnD_OverloadTimer", GetOverloadTime(pnum));
			
			// DnD_OverloadDamage is a BONUS percent added on top of DND_BASE_OVERLOADBUFF
			// at the consumption site, so it must NOT carry the 100 baseline.
			// ConvertFixedFactorToInt returns 100 + pct (and 100 for a zero attribute).
			temp = (PlayerModData[pnum].vals[PSTAT_OVERLOAD_DMGINCREASE] * 100) >> 16;
			int all_effect = PlayerModData[pnum].vals[PSTAT_INC_ALLOVERLOAD];
			if(all_effect) {
				// reduced effect if this mod is there
				temp = temp * (100 - all_effect) / 100;
				if(temp < 0)
					temp = 0;
			}
			SetActorInventory(victim, "DnD_OverloadDamage", Max(temp, CheckActorInventory(victim, "DnD_OverloadDamage")));
			
			ACS_NamedExecuteWithResult("DnD Monster Overload", victim);
		}
		else
			SetActorInventory(victim, "DnD_OverloadTimer", GetOverloadTime(pnum));
	}
}

enum {
	DND_MONSTERDEBUFF_VAAJ,
	DND_MONSTERDEBUFF_ZRAVOG,
};

Script "DnD Sync Monster Debuff" (int debuff, int victim, int val) CLIENTSIDE {
	switch(debuff) {
		case DND_MONSTERDEBUFF_VAAJ:
			SetActorInventory(victim, "VaajWeakness", val);
		break;
		case DND_MONSTERDEBUFF_ZRAVOG:
			SetActorInventory(victim, "OccultWeaknessStack", val);
		break;
	}

	SetResultValue(0);
}

// tid, mon_id, weaken %
Script "DnD Occult Weaken" (int victim, int mon_id) {
	SetActivator(victim);
	int time = 0;
	ACS_NamedExecuteWithResult("DnD Sync Monster Debuff", DND_MONSTERDEBUFF_ZRAVOG, victim, CheckInventory("OccultWeaknessStack"));
	while(time < OCCULT_WEAKEN_DURATION) {
		int prev = CheckInventory("OccultWeaknessStack");
		Delay(const:TICRATE);
		++time;
		if(prev != CheckInventory("OccultWeaknessStack") || CheckInventory("OccultWeaknessTimeReset"))
			time = 0;
	}
	SetInventory("OccultWeaknessStack", 0);
	SetInventory("OccultWeaknessReduction", 0);
	ACS_NamedExecuteWithResult("DnD Sync Monster Debuff", DND_MONSTERDEBUFF_ZRAVOG, victim, 0);
	SetResultValue(0);
}

Script "DnD Vaaj Weaken" (int victim) {
	SetActivator(victim);
	int time = 0;
	ACS_NamedExecuteWithResult("DnD Sync Monster Debuff", DND_MONSTERDEBUFF_VAAJ, victim, CheckInventory("VaajWeakness"));
	while(time < VAAJ_WEAKEN_DURATION) {
		Delay(const:TICRATE);
		++time;
		if(CheckInventory("VaajWeaknessTimeReset"))
			time = 0;
	}
	SetInventory("VaajReduction", 0);
	TakeInventory("VaajWeakness", 1);
	ACS_NamedExecuteWithResult("DnD Sync Monster Debuff", DND_MONSTERDEBUFF_VAAJ, victim, 0);
	SetResultValue(0);
}

int ApplyPenetrationToDamage(int pnum, int victim, int dmg, int damage_category, int flags, int resist, int pen) {
	// factor is the final resistance the monster will have against the attack
	int factor = Clamp_Between((100 - resist + pen), 0, 200);

	// non-zero, we're good
	int res = dmg * factor / 100;

	return res;
}

int FactorResists(int source, int victim, int wepid, int dmg, int damage_type, int actor_flags, int flags, bool forced_full, bool wep_neg = false) {
	// check penetration stuff on source -- set it accordingly to damage type being checked down below
	int mon_id = victim - DND_MONSTERTID_BEGIN;
	int damage_category = GetDamageCategory(damage_type, flags);
	int pnum = PlayerNumber();
	int pen = GetResistPenetration(pnum, damage_category);
	
	// if doomguy perk 50 is there and this is a monster, ignore res
	// added crit ignore res modifier here from below
	forced_full |= (!wep_neg && (actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT) && PlayerModData[pnum].vals[PSTAT_EX_CRITIGNORERESCHANCE] >= random(1, 100));
	
	// The monster's TRUE resist, which can sit well above DND_IMMUNITY_FACTOR -- see InitMonsterResists.
	// Every reduction below works on this number; the cap goes on further down, once they have landed.
	int resist = MonsterProperties[mon_id].resists[damage_category];

	// Ward aura, handed out by A_RadiusGive from a nearby source. Read LIVE rather than stored as a
	// delta on the monster: there is nothing to apply, nothing to remove, and no state to go stale if
	// the source dies or the receiver walks away mid burn.
	//
	// It is added into the true resist here rather than expressed as a PowerProtection damagefactor,
	// and that distinction is the whole reason this is not pure DECORATE. A damagefactor multiplies
	// FINAL damage: penetration could not answer it, DND_IMMUNITY_FACTOR would not contain it, and
	// Zravog could not reduce it. Added here it is ordinary resistance in every respect.
	if(CheckActorInventory(victim, "Aura_Ward_Token"))
		resist += DND_AURAWARD_RESIST;

	int temp;
	int pct_val = 0;

	// apply percentage reductions to resist HERE, ABOVE checking the penetration
	// if occult weakness exists, apply it checking monster's debuff -- to be done as a resist reduction to affect all players later
	// we will handle all percentage reductions here deliberately so that we don't mess up the base resist value of the monster!
	if((flags & DND_DAMAGEFLAG_ISRADIUSDMG) && PlayerModData[pnum].vals[PSTAT_ESS_VAAJ] >= random(1, 100)) {
		if(!CheckActorInventory(victim, "VaajWeakness")) {
			GiveActorInventory(victim, "VaajWeakness", 1);
			SetActorInventory(victim, "VaajReduction", ReadPlayerModExtra(pnum, INV_ESS_VAAJ));
			ACS_NamedExecuteWithResult("DnD Vaaj Weaken", victim);
		}
		else {
			// latter forces the reset on debuff timer
			GiveActorInventory(victim, "VaajWeaknessTimeReset", 1);
		}
	}

	// other players can reuse the previously applied value by someone else
	pct_val += CheckActorInventory(victim, "VaajReduction");

	if(damage_category == DND_DAMAGECATEGORY_OCCULT || damage_category == DND_DAMAGECATEGORY_SOUL || (flags & DND_DAMAGEFLAG_SOULATTACK)) {
		temp = PlayerModData[pnum].vals[PSTAT_ESS_ZRAVOG];
		if(temp) {
			if(!CheckActorInventory(victim, "OccultWeaknessStack")) {
				GiveActorInventory(victim, "OccultWeaknessStack", 1);
				ACS_NamedExecuteWithResult("DnD Occult Weaken", victim, mon_id);
			}
			else {
				// latter forces the reset on debuff timer
				GiveActorInventory(victim, "OccultWeaknessStack", 1);
				GiveActorInventory(victim, "OccultWeaknessTimeReset", 1);
			}
			SetActorInventory(victim, "OccultWeaknessReduction", temp * CheckActorInventory(victim, "OccultWeaknessStack"));
		}
		pct_val += CheckActorInventory(victim, "OccultWeaknessReduction");
	}
	else if(damage_category == DND_DAMAGECATEGORY_LIGHTNING) {
		pct_val += DND_THUNDERAXE_WEAKENPCT * (!!CheckActorInventory(victim, "ThunderAxeWeakenTimer"));
	}

	if(IsBoomstick(wepid) && HasClassPerk_Fast(DND_PLAYER_HOBO, 2)) 
		pct_val += DND_HOBO_RESISTPCT + (GetLevel() / DND_PERK_REGULARTHRESHOLD) * DND_HOBO_RESISTPCT_PERLVL;
	
	if(CheckActorInventory(victim, "Doomguy_ResistReduced"))
		pct_val += DND_DOOMGUY_RESISTPCT;

	// apply pct reduction
	if(pct_val) {
		if(pct_val > 100)
			pct_val = 100;
		resist = resist * (100 - pct_val) / 100;
	}

	// debuffs to reduce flat
	resist -= CountMonsterAilments(victim) * DND_WANDERER_RESREDUCE;

	// Every reduction above has landed on the TRUE resist, so this is where the cap finally goes on.
	// Capping here rather than at spawn is what makes an overcapped monster genuinely resistant to
	// having its resistance reduced: a level 90 magic immune monster is really at 165, so a 33% Zravog
	// stack takes it to 110 -- still above the cap, and it has not gained a point of damage taken yet.
	// The same stack on a monster at 120 lands it at 80, below the cap, and that one is felt in full.
	//
	// Penetration is applied after this, against the capped number, never the true one.
	if(resist > DND_IMMUNITY_FACTOR)
		resist = DND_IMMUNITY_FACTOR;

	// A weakness is a multiplier on damage TAKEN, not a subtraction from the resist, so it has to come
	// after the cap -- (100 - resist) has to be positive for it to mean what it says.
	//
	// Running it here rather than baking it in at spawn also fixes what the old order did to a monster
	// whose resist was already NEGATIVE from a weakness. The percentage reductions above multiply, so
	// they pull a negative number toward zero: a -25 fire resist under a 33% Doomguy reduction became
	// -16, ie. the reduction made an elementally weak monster take LESS damage (116% instead of 125%).
	// Applying the weakness last means a reduction can only ever help.
	resist = ApplyResistWeakness(resist, GetMonsterResistWeakness(mon_id, damage_category));

	temp = PlayerModData[pnum].vals[PSTAT_INC_INVERTRESISTANCES];
	if(temp >= random(1, 100))
		resist = -resist;

	//printbold(s:"new res ", d:resist);

	// if we do full dmg, either do dmg as is or check for pen overpowering the resist, so we can go ahead and do extra damage
	if(forced_full) {
		if(pen - resist > 0)
			return ApplyPenetrationToDamage(pnum, victim, dmg, damage_category, flags, resist, pen);
		return dmg;
	}
	
	// no full dmg, so we just factor resist in as usual
	return ApplyPenetrationToDamage(pnum, victim, dmg, damage_category, flags, resist, pen);
}

// for player hitting others damage
int HandlePlayerBuffs(int p_tid, int enemy_tid, int damage_type, int wepid, int flags) {
	int more_bonus = 100;
	if(!IsOccultDamage(damage_type) && IsAccessoryEquipped(p_tid, DND_ACCESSORY_DEMONBANE))
		more_bonus = more_bonus * (100 - DND_DEMONBANE_REDUCE) / 100;
		
	// ghost enemies take more damage if nether mask is equipped
	if(CheckFlag(enemy_tid, "GHOST") && IsAccessoryEquipped(p_tid, DND_ACCESSORY_NETHERMASK))
		more_bonus = more_bonus * (100 + DND_NETHERGHOST_BONUS) / 100;
		
	// amps fire damage, reduces cold damage
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_AMULETHELLFIRE)) {
		// we handle ignite damage buff in the dot calculation
		if(IsFireDamage(damage_type) && !(flags & DND_DAMAGETICFLAG_NOIGNITESTACK))
			more_bonus = more_bonus * (100 + DND_AMULETHELL_DAMAGE) / 100;
		else if(IsIceDamage(damage_type))
			more_bonus = more_bonus * (100 - DND_AMULETHELL_DAMAGE) / 100;
	}
	
	if(!(IsMeleeDamage(damage_type) || (flags & DND_DAMAGETICFLAG_CONSIDERMELEE)) && IsAccessoryEquipped(p_tid, DND_ACCESSORY_HATESHARD))
		more_bonus = more_bonus * (100 - DND_HATESHARD_REDUCTION) / 100;
	
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_HANDARTEMIS) && wepid >= 0) {
		if(IsSuperWeapon(wepid))
			more_bonus = more_bonus * (100 - DND_ARTEMIS_REDUCE_SUPER) / 100;
		else
			more_bonus = more_bonus * (100 - DND_ARTEMIS_REDUCE) / 100;
	}
		
	// if there's poison stack that means it's a regular poison attack, but if there's not that means its a poison dot, do not include it again for that
	if(CheckInventory("AgamottoOffense") && !(flags & DND_DAMAGEFLAG_NOPOISONSTACK))
		more_bonus = more_bonus * (100 + DND_AGAMOTTO_OFFENSE) / 100;
	
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_LICHARM)) {
		if(damage_type == DND_DAMAGETYPE_SOUL || (flags & DND_DAMAGETICFLAG_SOULATTACK))
			more_bonus = more_bonus * (100 + DND_LICHARM_BUFF) / 100;
		else
			more_bonus = more_bonus * (100 - DND_LICHARM_NERF) / 100;
	}

	// Sigil  of elements powers live in the buff layer now
	int ele_idx = -1;
	if(IsFireDamage(damage_type))
		ele_idx = DND_DAMAGECATEGORY_FIRE - DND_ELECATEGORY_BEGIN;
	else if(IsIceDamage(damage_type))
		ele_idx = DND_DAMAGECATEGORY_ICE - DND_ELECATEGORY_BEGIN;
	else if(IsLightningDamage(damage_type))
		ele_idx = DND_DAMAGECATEGORY_LIGHTNING - DND_ELECATEGORY_BEGIN;
	else if(IsPoisonDamage(damage_type))
		ele_idx = DND_DAMAGECATEGORY_POISON - DND_ELECATEGORY_BEGIN;

	int ele_packed = GetPlayerBuffElementalMorePacked(p_tid - P_TIDSTART, ele_idx);
	if(ele_packed != DND_PACKED_MULT_IDENTITY)
		more_bonus = ApplyPackedMultiplier(more_bonus, ele_packed);
	else if(IsElementalDamageType(damage_type) && IsAccessoryEquipped(p_tid, DND_ACCESSORY_SIGILELEMENTS))
		more_bonus = more_bonus * (100 - DND_SIGIL_NERF) / 100; // DND_SIGIL_NERF is 100: total negation, by design
	
	// 50% more damage taken
	// the token is granted to the MONSTER (OnPlayerHit), so a bare CheckInventory
	// read the player and this branch never fired
	if(CheckActorInventory(enemy_tid, "HateWeakness"))
		more_bonus = more_bonus * (100 + DND_HATESHARD_BUFF) / 100;
	
	return more_bonus;
}

// for others hitting player damage
int HandlePlayerOnHitBuffs(int p_tid, int enemy_tid, int dmg, int dmg_data, str arg2) {
	// take extra damage only if they aren't ghost
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_NETHERMASK) && !CheckFlag(enemy_tid, "GHOST"))
		dmg = MulPercent_Exact(dmg, DND_NETHERMASK_AMP, DND_NETHERMASK_DIV);
		
	// amps cold damage taken, reduces fire damage
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_AMULETHELLFIRE)) {
		if((dmg_data & DND_DAMAGETYPEFLAG_FIRE) || arg2 == "Slime")
			dmg /= DND_AMULETHELL_FACTOR;
		else if(dmg_data & DND_DAMAGETYPEFLAG_ICE)
			dmg = MulPercent_Exact(dmg, DND_AMULETHELL_AMP, DND_AMULETHELL_FACTOR);
	}
	
	// agamotto defense
	if(CheckActorInventory(p_tid, "AgamottoDefense"))
		dmg = MulPercent_Exact(dmg, DND_AGAMOTTO_DEFENSE, DND_AGAMOTTO_DEFENSE_FACTOR);
		
	if(CheckActorInventory(enemy_tid, "HunterTalismanDebuff"))
		dmg -= dmg / DND_HUNTERTALISMAN_NERF;
		
	// 10%
	if(CheckActorInventory(p_tid, "Invulnerable_Better"))
		dmg /= DND_INVULSPHERE_FACTOR;

	if(CheckActorInventory(p_tid, "Wanderer_Ascended"))
		dmg = dmg * (100 - ETHEREAL_RESIST) / 100;

	// check flask charge gain on hit mod
	CheckFlasksOnHitGain(p_tid - P_TIDSTART);
	
	return dmg;
}

// This function is responsible for handling all damage effects player has that affect their damage some way
// ex: curses etc.
int HandleGenericPlayerMoreDamageEffects(int pnum, int wepid) {
	int more_bonus = 100;
	int temp;
	temp = PlayerModData[pnum].vals[PSTAT_EX_MOREDAMAGEPERCHARGE];
	if(temp)
		more_bonus = more_bonus * (100 + temp * GetChargeCount(pnum)) / 100;

	more_bonus = ApplyPackedMultiplier(more_bonus, GetPlayerBuffMoreDamagePacked(pnum));
	
	return more_bonus;
}

void HandleTargetPicking(int montid) {
	// off cd and we dont have a target, since we damaged it we can pick it for now
	int curr_tid = CheckInventory("TargetTID");
	if(!curr_tid) {
		SetInventory("TargetTID", montid);
		
		if(!CheckInventory("TargetPickCooldown"))
			GiveInventory("TargetPickCooldown", 1);
	}
	else if(montid == curr_tid) // attacking same monster, refresh cd
		GiveInventory("TargetPickCooldown", 1);
}

// returns the filtered, reduced etc. damage when factoring in all resists or weaknesses ie. this is the final damage the actor will take
// This is strictly for player doing damage to other monsters or shootables!
// All damage factors here are applied in the "more" method, ie. multiplicative
int HandleDamageDeal(int source, int victim, int dmg, int damage_type, int wepid, int flags, int ox, int oy, int oz, int actor_flags, bool wep_neg = false, bool oneTimeRipperHack = false) {
	str s_damagetype = DamageTypeList[damage_type];
	bool forced_full = false;
	bool no_ignite_stack = flags & DND_DAMAGEFLAG_NOIGNITESTACK;
	int temp;
	
	int pnum = source - P_TIDSTART;

	// check blocking/invulnerable status of monster -- if they are and we dont have foilinvul on this, no penetration
	if
	(
		(HasMonsterTrait(victim - DND_MONSTERTID_BEGIN, DND_ISBLOCKING) || CheckFlag(victim, "INVULNERABLE")) && !(actor_flags & DND_ACTORFLAG_FOILINVUL)
	)
	{
		temp = PlayerModData[pnum].vals[PSTAT_ESS_HARKIMONDE];
		// we have 0 chance or we have chance but it didn't roll in our favor
		if(!temp || temp < random(1, 100)) {
			ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, 0, 0, DND_HITBEEP_INVULNERABLE);

			temp = PlayerModData[pnum].vals[PSTAT_INC_BLOCKPREVENTION];
			if(temp && random(1, 100) <= temp) {
				if(!CheckActorInventory(victim, "DnD_AntiBlockCounter")) {
					SetActorInventory(victim, "DnD_AntiBlockCounter", DND_INC_BLOCKPREVENTIONTIME);
					ACS_NamedExecuteWithResult("DnD Block Prevention Timer", victim);
				}
				else
					SetActorInventory(victim, "DnD_AntiBlockCounter", DND_INC_BLOCKPREVENTIONTIME);
			}

			return 0;
		}
	}

	if(HasMonsterTrait(victim - DND_MONSTERTID_BEGIN, DND_TEMPORALBUBBLE) && !CheckActorInventory(victim, "TemporalBubbleCooldown")) {
		GiveActorInventory(victim, "TemporalBubbleCooldown", 1);
		PlaySound(victim, "TemporalBubble/Pop", CHAN_7, 1.0);
		ACS_NamedExecuteAlways("DnD Temporal Bubble Ticker", 0, victim, victim - DND_MONSTERTID_BEGIN);
		return 0;
	}

	// check if the damage is to be dealt without any reductions from resistances or immunities
	forced_full |= 	(flags & DND_DAMAGEFLAG_DOFULLDAMAGE)																		||
					((flags & DND_DAMAGEFLAG_ISSPELL) && CheckUniquePropertyOnPlayer(pnum, PUP_SPELLSDOFULL))					||
					(damage_type == DND_DAMAGETYPE_SOUL && CheckUniquePropertyOnPlayer(pnum, PUP_SOULWEPSDOFULL));
					
	int extra = 0;
	
	// pain checks -- they dont do anything anymore because we dont deal damage here, we just send it outside
	/*if(actor_flags & DND_ACTORFLAG_PAINLESS)
		s_damagetype = StrParam(s:s_damagetype, s:"_NoPain");
	else if(actor_flags & DND_ACTORFLAG_FORCEPAIN)
		s_damagetype = StrParam(s:s_damagetype, s:"_ForcePain");*/
	
	// extra property checks moved here
	// WE CHECK FOR CRITS HERE, EITHER WEAPON OR SPELL! THE FINAL STEP BEFORE RESISTS
	if(!wep_neg) {
		// chance to force pain
		extra = GetPlayerWeaponModVal(pnum, wepid, WEP_MOD_FORCEPAINCHANCE);
		if(extra && extra >= random(1, 100)) {
			HandleMonsterDeathConfirm(victim, 1);
			Thing_Damage2(victim, 1, "ForcedPainBypass");
		}
		
		// percent damage of monster if it exists
		extra = GetPlayerWeaponModVal(pnum, wepid, WEP_MOD_PERCENTDAMAGE);
		dmg += (MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp * extra) / 100;
		flags |= (!!extra) * DND_DAMAGEFLAG_PERCENTHEALTH;

		// not DOT and we can roll crit
		if
		(
			!(actor_flags & DND_ACTORFLAG_ISDAMAGEOVERTIME) && 
			!(flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME) && 
			((actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT) || GetPlayerWeaponCritState(pnum, wepid) || (!GetPlayerWeaponCritLock(pnum, wepid) && CheckCritChance(pnum, victim, wepid, IsLightningDamage(damage_type))))
		)
		{
			SetPlayerWeaponCritState(pnum, wepid);
			actor_flags |= DND_ACTORFLAG_CONFIRMEDCRIT;
			CheckFlasksOnCritGain(pnum);
		}

		LockPlayerCritState(pnum, wepid);
	}
	else if(flags & DND_DAMAGEFLAG_ISSPELL) {
		if
		(
			!(actor_flags & DND_ACTORFLAG_ISDAMAGEOVERTIME) && 
			!(flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME) && 
			((actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT) || CheckCritChance(pnum, victim, -1, 0, IsLightningDamage(damage_type)))
		)
		{
			actor_flags |= DND_ACTORFLAG_CONFIRMEDCRIT;
			CheckFlasksOnCritGain(pnum);
		}
	}

	// FINALIZED DAMAGE WILL BE BELOW, AFTER RESISTS!
	//printbold(s:"res calc");
	temp = dmg;
	//if(!wep_neg)
	dmg = FactorResists(source, victim, wepid, dmg, damage_type, actor_flags, flags, forced_full, wep_neg);
	
	// handle poison checks
	// printbold(d:damage_type, s: " ", d:IsPoisonDamage(damage_type), s: " ", d:!(flags & DND_DAMAGEFLAG_NOPOISONSTACK), s: " ", d:flags);

	// hit beeps and stuff
	// if more that means we hit a weakness, otherwise below conditions check immune and resist respectively
	extra = (dmg < temp) * DND_DAMAGETICFLAG_LESSENED;
	
	ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, dmg, temp);

	// damage number handling - NO MORE DAMAGE FIDDLING FROM BELOW HERE
	// all damage calculations should be done by this point, besides cull --- cull should not reflect on here
	// printbold(s:"apply ", d:dmg, s: " of type ", s:s_damagetype, s: " pnum: ", d:pnum);
	// this part handles damage pushing
	temp = victim - DND_MONSTERTID_BEGIN;
	
	// extra represents the flag list of damageticflag
	extra |= 	(!(flags & DND_DAMAGEFLAG_NOPUSH) * DND_DAMAGETICFLAG_PUSH) 						|
				(!!(actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT) * DND_DAMAGETICFLAG_CRIT)			|
				(!!(actor_flags & DND_ACTORFLAG_COUNTSASMELEE) * DND_DAMAGETICFLAG_CONSIDERMELEE)	|
				(!!(flags & DND_DAMAGEFLAG_ADDEDIGNITE) * DND_DAMAGETICFLAG_ADDEDIGNITE)			|
				(!!(flags & DND_DAMAGEFLAG_SCALEIGNITE) * DND_DAMAGETICFLAG_SCALEIGNITE)			|
				(!!(flags & DND_DAMAGEFLAG_EXTRATOUNDEAD) * DND_DAMAGETICFLAG_EXTRATOUNDEAD)		|
				(!!(flags & DND_DAMAGEFLAG_NOPOISONSTACK) * DND_DAMAGETICFLAG_NOPOISONSTACK)		|
				(!!(flags & DND_DAMAGEFLAG_INFLICTPOISON) * DND_DAMAGETICFLAG_INFLICTPOISON)		|
				(no_ignite_stack * DND_DAMAGETICFLAG_NOIGNITESTACK)									|
				(!!(flags & DND_DAMAGEFLAG_SOULATTACK) * DND_DAMAGETICFLAG_SOULATTACK)				|
				(!!(flags & DND_DAMAGEFLAG_ISSPELL) * DND_DAMAGETICFLAG_SPELL)						|
				(!!((actor_flags & DND_ACTORFLAG_ISDAMAGEOVERTIME) || (flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME)) * DND_DAMAGETICFLAG_DOT);
	
	// we send particular damage types in that can cause certain status effects like chill, freeze etc.
	if(damage_type == DND_DAMAGETYPE_ICE)
		extra |= DND_DAMAGETICFLAG_ICE;
	else if(IsDamageCapableOfIgnite(damage_type) && !no_ignite_stack)
		extra |= DND_DAMAGETICFLAG_FIRE;
	else if(damage_type == DND_DAMAGETYPE_LIGHTNING)
		extra |= DND_DAMAGETICFLAG_LIGHTNING;
	else if(IsPoisonDamage(damage_type))
		extra |= DND_DAMAGETICFLAG_POISON;
	else if(damage_type == DND_DAMAGETYPE_PHYSICAL || damage_type == DND_DAMAGETYPE_MELEE) {
		extra |= DND_DAMAGETICFLAG_PHYSICAL;
		if(pbuffs[pnum].buff_net_values[BUFF_SILVERIMBUE].additive)
			extra |= DND_DAMAGETICFLAG_EXTRATOUNDEAD;
	}

	//printbold(s:"before num pnum ", d:pnum, s: " ", d:temp, s:" dmg ", d:dmg);
	int m_id = temp;
	if(!IsDamageTicRunning(pnum, m_id)) {
		SetDamageTicRunning(pnum, m_id);
		PlayerDamageVector[pnum].x = ox;
		PlayerDamageVector[pnum].y = oy;
		PlayerDamageVector[pnum].z = oz;
		ACS_NamedExecuteWithResult("DnD Damage Accumulate", m_id | ((wep_neg | (oneTimeRipperHack << 1)) << DND_DAMAGE_ACCUM_SHIFT), wepid, extra, damage_type);
	}

	// bank happens at the bottom now, on the number the monster is actually dealt
	if((temp = CheckActorInventory(victim, "MonsterFortifyCount")) && !(actor_flags & DND_ACTORFLAG_FOILINVUL)) {
		// The pool eats min(pool, dmg) and both sides settle from that one number. Reusing temp as
		// scratch got each wrong: a holding shield subtracted the WHOLE pool from the hit, and a
		// breaking one overwrote the pool before use, cancelling the hit instead of overflowing it.
		int absorbed = temp;
		if(absorbed > dmg)
			absorbed = dmg;

		if(temp <= dmg) {
			PlaySound(victim, "Elite/FortifyCrack", CHAN_VOICE, 1.0);
			// remove fortify modifier from monster
			ACS_NamedExecuteWithResult("DnD Monster Trait Take Single", victim, DND_FORTIFIED);
		}

		SetActorInventory(victim, "MonsterFortifyCount", temp - absorbed);
		dmg -= absorbed;
	}
	
	HandleTargetPicking(victim);

	// What the monster is actually dealt, which is what gets banked. Cull's doubling below is a kill
	// device, not damage, so it stays out of the numbers and the statistics.
	int dealt = dmg;

	// cull checks
	if(CheckCullRange(source, victim, dmg)) {
		//printbold(s:"can cull");
		if((flags & DND_DAMAGEFLAG_CULL) || (!wep_neg && HasWeaponPower(pnum, wepid, WEP_POWER_CULL))) {
			// if self cull is in effect simply destroy it otherwise return from here
			GiveActorInventory(victim, "MonsterKilledByPlayer", 1);
			dmg = GetActorProperty(victim, APROP_HEALTH) * 2;
		}
		else {
			//printbold(s:"color change");
			GiveActorInventory(victim, "Doomguy_CanExecute", 1);
		}
	}
	
	if(dmg > 0) {
		// give this token early to prevent order of events getting mixed up
		// is victim dead from this damage?
		if(GetActorProperty(victim, APROP_HEALTH) <= dmg) {
			if(HasMonsterTrait(victim - DND_MONSTERTID_BEGIN, DND_BORROWEDTIME) && !CheckActorInventory(victim, "DnD_BorrowedTimeActive")) {
				GiveActorInventory(victim, "DnD_BorrowedTimeActive", 1);
				ACS_NamedExecuteAlways("DnD Borrowed Time FX", 0, victim);
				ACS_NamedExecuteWithResult("DnD Borrowed Time Decay", victim);
				return 0;
			}

			GiveActorInventory(victim, "MonsterKilledByPlayer", 1);

			// overkill damage lifesteal check
			if(HasActorClassPerk_Fast(source, DND_PLAYER_PUNISHER, 5)) {
				temp = (dmg - GetActorProperty(victim, APROP_HEALTH)) * DND_PUNISHER_OVERKILL_LEECHFACTOR / 100;
				if(temp > 0)
					ResolveLifesteal(pnum, temp, CheckActorInventory(source, "PlayerHealthCap"));
			}

			if(actor_flags & DND_ACTORFLAG_DROPSOUL)
				GiveActorInventory(victim, "BookofDeadCausedDeath", 1);

			// give this for non-magic seal weapons (seals their souls...)
			if(damage_type != DND_DAMAGETYPE_MAGICSEAL && (IsOccultDamage(damage_type) || (!wep_neg && IsSoulDroppingWeapon(wepid))))
				GiveActorInventory(victim, "MagicCausedDeath", 1);

			if(HasActorClassPerk_Fast(source, DND_PLAYER_BERSERKER, 4) && (IsMeleeDamage(damage_type) || flags & DND_DAMAGETICFLAG_CONSIDERMELEE)) {
				SetActorInventory(source, "Berserker_HitTimer", DND_BERSERKER_PERK60_TIMER);
				if((temp = CheckActorInventory(source, "Berserker_HitTracker")) < DND_BERSERKER_PERK60_MAXSTACKS) {
					GiveActorInventory(source, "Berserker_HitTracker", 1);
					if(!temp)
						ACS_NamedExecuteAlways("DnD Berserker Perk50 Timer", 0, source);
				}
				if(temp + 1 >= DND_BERSERKER_PERK60_MAXSTACKS) {
					if(!CheckActorInventory(source, "Berserker_NoRoar"))
						HandleBerserkerRoar(source);
					GiveActorInventory(source, "Berserker_Perk60_Speed", 1);
				}
			}

			// stamina gain
			if(!wep_neg  && (IsMeleeDamage(damage_type) || flags & DND_DAMAGETICFLAG_CONSIDERMELEE) && CanGainStaminaOnKill(pnum))
				GiveStamina(GetStaminaGainOnKill(pnum));

			// charge conditional kill checks
			temp = PlayerModData[pnum].vals[PSTAT_FRENZYCHARGE_ONSHATTER];
			if(temp && IsIceDamage(damage_type) && random(1, 100) <= temp)
				HandlePlayerBuffAssignment(pnum, 0, BTI_FRENZYCHARGE);

			temp = PlayerModData[pnum].vals[PSTAT_ENDURANCECHARGE_ONMELEE];
			if(temp && IsMeleeDamage(damage_type) && random(1, 100) <= temp)
				HandlePlayerBuffAssignment(pnum, 0, BTI_ENDURANCECHARGE);

			temp = PlayerModData[pnum].vals[PSTAT_POWERCHARGE_ONOVERLOAD];
			if(temp && CheckActorInventory(victim, "DnD_OverloadTimer") && random(1, 100) <= temp)
				HandlePlayerBuffAssignment(pnum, 0, BTI_POWERCHARGE);
		}
		else if(HasClassPerk_Fast(DND_PLAYER_DOOMGUY, 5)) 
			GiveActorInventory(victim, "Doomguy_ResistReduced", 1);
	}

	// Banked only now, so the accumulator holds what the monster took and not what it was swung at;
	// "DnD Damage Accumulate" reconciles health against this total. Borrowed time returns above and
	// so banks nothing, which is correct. The per element breakdown is only for players with added
	// damage -- for everyone else this total already IS the one element's subtotal.
	if(dealt > 0) {
		if(GetPlayerExtraDamageMask(pnum))
			RecordMixedTicDamage(pnum, m_id, extra, dealt);
		PlayerDamageTic[pnum].total[m_id] += dealt;
	}

	return dmg;
}

Script "DnD Crossbow Explosion" (int this, int target) {
	int exptid = DND_CROSSBOW_EXPLOSIONTID + target - P_TIDSTART;
	SpawnForced("Crossbow_Explosion", GetActorX(this), GetActorY(this), GetActorZ(this) + GetActorProperty(this, APROP_HEIGHT) / 2 + 24.0, exptid);
	SetActivator(exptid);
	SetPointer(AAPTR_TARGET, target);
	Thing_ChangeTID(exptid, 0);
	
	SetResultValue(0);
}

// we check with if statement here now just in case we add more flags in the future, the check below is to ensure we bypass script execution
Script "DnD Adjust Impact Damage" (int flags, int dmg, int owner) {
	//if(flags & DND_DAMAGEFLAG_LOSEDAMAGEPERHIT) {
	int dummy_tid = TEMPORARY_DATADUMMY_TID + owner - P_TIDSTART;
	if(!CheckActorInventory(owner, "DnD_DummySpawned")) {
		//printbold(s:"make dummy");
		GiveActorInventory(owner, "DnD_DummySpawned", 1);
		SpawnForced("DnD_DataDummy", 0, 0, 0, dummy_tid);
		SetActivator(dummy_tid);
		SetPointer(AAPTR_TARGET, owner);
	}
	
	int hitcount = GetUserVariable(dummy_tid, "user_hitcount");
	if(hitcount != DND_MAX_DAMAGELOSEHITS) {
		dmg = dmg * (100 - DND_DAMAGELOST_PERCENT * hitcount) / 100;
		if(dmg < 1)
			dmg = 1;
		
		++hitcount;
		SetUserVariable(dummy_tid, "user_hitcount", hitcount);
		//printbold(s:"hitc ", d:hitcount);
	}
	//}
	SetResultValue(dmg);
}

void HandleRipperHitSound(int tid, int owner, int wepid) {
	switch(wepid) {
		case DND_WEAPON_AXE:
			PlaySound(tid, "Axe/HitBleeding", 5, 1.0);
			SpawnForced("BladeHitFX", GetActorX(tid), GetActorY(tid), GetActorZ(tid));
		break;
		case DND_WEAPON_CROSSBOW:
			PlaySound(tid, "Crossbow/Hit", 5, 1.0);
		break;
		case DND_WEAPON_DARKLANCE:
			// stack building on kill
			GiveActorInventory(owner, "LanceStacks", 1);
		break;
	}
}

Script "DnD Handle Hitbeep" (int dmg, int orig_dmg, int type_override) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	int beep_type = -1;

	if(!type_override) {
		if(dmg > orig_dmg)
			beep_type = DND_HITBEEP_WEAKNESS;
		else if(dmg < orig_dmg / 4)
			beep_type = DND_HITBEEP_IMMUNITY;
		else if(dmg < orig_dmg)
			beep_type = DND_HITBEEP_RESIST;
	}
	else
		beep_type = type_override;

	if(GetCVar("dnd_hitbeeps") && beep_type != -1 && !CheckInventory(HitBeepSounds[beep_type][HITBEEP_TIMER])) {
		LocalAmbientSound(HitBeepSounds[beep_type][HITBEEP_SOUND], 127);
		GiveInventory(HitBeepSounds[beep_type][HITBEEP_TIMER], 1);
	}
}

void ResolveLifesteal(int pnum, int amt, int spawn_health) {
	// give up to the lifesteal limit
	int ptid = pnum + P_TIDSTART;
	int toAdd = CheckActorInventory(ptid, "LifeStealAmount");
	int cap = GetLifestealCap(pnum);
	int toCompare = GetActorProperty(ptid, APROP_HEALTH);
	bool cyborgCheck = HasActorClassPerk_Fast(ptid, DND_PLAYER_CYBORG, 5);

	if(cyborgCheck) {
		cap = GetPlayerEnergyShieldCap(pnum);
		amt /= 2;
		if(!amt)
			amt = 1;
		toCompare = CheckActorInventory(ptid, "EShieldAmount");
	}

	// if over the cap, make it so that it would only be gaining up to reach the cap
	if(amt + toAdd > cap) {
		if(!HasActorClassPerk_Fast(ptid, DND_PLAYER_PUNISHER, 4))
			amt = cap - toAdd;
		else { // go over cap if punisher perk exists
			amt += amt * DND_PUNISHER_OVERLEECHVAL / 100;
			if(amt + toAdd > cap * 2)
				amt = cap * 2 - toAdd;
		}
	}
	
	// not using lifesteal cap from here on
	cap = PlayerModData[pnum].vals[PSTAT_CORR_INSTALEECHPCT] + PlayerModData[pnum].vals[PSTAT_INC_INSTANTLIFESTEAL];
	if(cap) {
		if(cap > 100)
			cap = 100;

		// take a bit away from this
		cap = amt * cap / 100;
		if(cap) {
			amt -= cap;

			// give player instant leech here
			if(toCompare + cap < spawn_health) {
				if(!cyborgCheck)
					GiveActorInventory(ptid, "HealthBonusX", cap);
				else
					AddActorEnergyShield(ptid, cap);
			}
			else {
				// we can put "lifesteal effect not removed when reaching max life" here in the future if needed to not break, but also not heal
				if(!cyborgCheck)
					GiveActorInventory(ptid, "HealthBonusX", spawn_health - toCompare);
				else
					AddActorEnergyShield(ptid,  spawn_health - toCompare);
			}

			if(amt <= 0)
				return;
		}
	}

	if(!toAdd && !CheckActorInventory(ptid, "LifestealCooldown")) {
		GiveActorInventory(ptid, "LifeStealAmount", amt);
		GiveActorInventory(ptid, "LifestealCooldown", 1);
		GiveActorInventory(ptid, "LifestealScriptRunning", 1);
		ACS_NamedExecuteAlways("DnD Lifesteal Script", 0, ptid);
	}
	else {
		GiveActorInventory(ptid, "LifeStealAmount", amt);

		// fixes a corner-case timing issue
		if(!CheckActorInventory(ptid, "LifestealScriptRunning")) {
			GiveActorInventory(ptid, "LifestealCooldown", 1);
			GiveActorInventory(ptid, "LifestealScriptRunning", 1);
			ACS_NamedExecuteAlways("DnD Lifesteal Script", 0, ptid);
		}
	}
}

void HandleLifesteal(int pnum, int wepid, int flags, int dmg) {
	// in order for this to work we must have less health than our cap
	int spawn_health = GetSpawnHealth();
	int comp = GetActorProperty(0, APROP_HEALTH);
	if(HasClassPerk_Fast(DND_PLAYER_CYBORG, 5)) {
		spawn_health = GetPlayerEnergyShieldCap(pnum);
		comp = CheckInventory("EShieldAmount");
	}

	if(comp >= spawn_health || !dmg)
		return;
		
	int taltos = (IsMeleeWeapon(wepid) || (flags & DND_DAMAGETICFLAG_CONSIDERMELEE)) && CheckInventory("TaltosUp");
	int brune_1 = CheckInventory("FakeBloodPower");
	int brune_2 = CheckInventory("FakeBloodPowerBetter");
	int cap = GetLifesteal(pnum);
	if(cap || taltos || brune_1 || brune_2) {
		taltos = cap + taltos * DND_TALTOS_LIFESTEAL + brune_1 * BLOODRUNE_LIFESTEAL_AMT + brune_2 * BLOODRUNE_LIFESTEAL_AMT2;
		
		//printbold(s:"lifesteal factor ", f:taltos);
		
		// divide by 100 as its a percentage -- and >> 16 to make it int -- added little overflow check here too
		taltos /= 100;
		// FixedMul carries a 64-bit intermediate, so no branch is needed. The old
		// "overflow safe" branch did taltos >>= 16 on a sub-1.0 fraction, which is
		// 0 -- 25% lifesteal healed 32767 at dmg 131071 and NOTHING at 131072, so
		// sustain vanished on exactly the biggest hits. It also divided by dmg,
		// which terminates the script when dmg is 0.
		taltos = FixedMul(dmg, taltos);

		//printbold(s:"to be given ", d:taltos);
		
		// no longer accept 1 point of lifesteal
		if(taltos <= 0)
			return;
		
		ResolveLifesteal(pnum, taltos, spawn_health);
	}
}

// str_att, dex_att and int_att must assume 0.01 to be 1%!
int HandleNonWeaponDamageScale(int dmg, int damage_category, int flags, int str_att = 0,  int dex_att = 0, int int_att = 0) {
	int temp, temp2;
	int pnum = PlayerNumber();
	int pct_bonus = 0;
	bool isSpell = flags & DND_WDMG_ISSPELL;
	int dmg_flag_mapping = (flags & DND_WDMG_ISRADIUSDMG) ? DND_DAMAGEFLAG_ISRADIUSDMG : 0;

	// spell damage is now stored as raw value in the dmg
	/*if(isSpell) {
		// we take id of spell from the table as input, then read its damage
		temp2 = dmg;
		dmg = SpellDamageTable[dmg].dmg;
	}*/
	
	if(flags & DMG_WDMG_ESHIELDSCALE)
		dmg += CheckInventory("EShieldAmount") / 25; // 4%
	
	// attribute bonus only applied if not DOT
	if(!(flags & DND_WDMG_ISDOT)) {
		dmg += (!isSpell) * MapDamageCategoryToFlatBonus(pnum, damage_category, dmg_flag_mapping);

		bool isMelee = damage_category == DND_DAMAGECATEGORY_MELEE || (flags & DND_WDMG_ISMELEE);
		if(isMelee)
			str_att = !str_att ? DND_STAT_ATTUNEMENT_GAIN : str_att;
		else if((flags & DND_WDMG_ISOCCULT) || damage_category == DND_DAMAGECATEGORY_OCCULT || isSpell)
			int_att = !int_att ? DND_STAT_ATTUNEMENT_GAIN : int_att;
		else if(!dex_att)
			dex_att = DND_STAT_ATTUNEMENT_GAIN;
		pct_bonus += HandleStatBonus(pnum, str_att, dex_att, int_att, isMelee);
	}
	
	if((flags & DND_WDMG_ISOCCULT) || damage_category == DND_DAMAGECATEGORY_OCCULT) // is occult (add demon bane bonus)
		dmg = dmg * (100 + DND_DEMONBANE_GAIN * IsAccessoryEquipped(ActivatorTID(), DND_ACCESSORY_DEMONBANE)) / 100;
		
	if(flags & DMG_WDMG_ISARTIFACT)
		dmg = dmg * (100 + DND_ARTIFACT_GAIN * CheckInventory("Ability_Arcanery")) / 100;
	
	temp = PlayerModData[pnum].vals[PSTAT_EX_PHYSDAMAGEPER_FLATHEALTH];
	if((damage_category == DND_DAMAGECATEGORY_MELEE || damage_category == DND_DAMAGECATEGORY_BULLET) && temp)
		pct_bonus += GetFlatHealthDamageFactor(temp);
	
	// dont let dot double dip
	if(!(flags & DND_WDMG_ISDOT)) {
		// uncached path, so the buff term is read live right here
		temp = GetPlayerPercentDamage(pnum, -1, damage_category, dmg_flag_mapping) + GetPlayerBuffIncreasedDamage(pnum) + GetPlayerAccuracyDamageBonus(pnum, -1);
		if(temp/* && !isSpell*/)
			pct_bonus += temp;

		// apply the % bonus now -- guard the collapse, the weapon paths already do.
		// Golgoth Weaken (-75) + Fleshwizard Weaken (-25) sum to exactly -100, and
		// anything past that produced NEGATIVE damage here.
		pct_bonus += 100;
		if(pct_bonus <= 0)
			dmg = 0;
		else
			dmg = MulPercent_Exact(dmg, pct_bonus);
	}
		
	// finally crit chance
	// spells will have their own crit source compared to attacks
	/*if(!isSpell && CheckCritChance(-1, false, -1)) {
		dmg = dmg * GetCritModifier(-1) / 100;
		HandleHunterTalisman();
	}*/
	
	// final additions
	
	/*if(isSpell) {
		if(SpellDamageTable[temp2].dmg_low)
			dmg *= random(SpellDamageTable[temp2].dmg_low, SpellDamageTable[temp2].dmg_high);
	}*/
	
	//printbold(s:"scaled player damage ", d:dmg);
	return dmg;
}

// What one tick is worth, priced with the applier's stats and weapon. Pulled out of
// "DnD Monster Ignite" so an application landing on an ALREADY burning monster can price itself.
int GetIgniteTickDamage(int pnum, int victim, int wepid, int added_dmg) {
	return HandleNonWeaponDamageScale(GetFireDOTDamage(pnum, added_dmg, victim, wepid), DND_DAMAGECATEGORY_FIRE, DND_WDMG_ISDOT);
}

void HandleIgniteEffects(int pnum, int victim, int wepid, int flags, int dmg_within_tic) {
	// addedIgn adds damage to ignite from weapons' base and gives extra ignite chance, scaleIgn is just damage
	bool addedIgn = !!(flags & DND_DAMAGETICFLAG_ADDEDIGNITE);
	bool scaleIgn = addedIgn || (flags & DND_DAMAGETICFLAG_SCALEIGNITE);
	if
	(
		CheckAilmentImmunity(pnum, victim - DND_MONSTERTID_BEGIN, DND_MOLTENBLOOD) &&
		CheckIgniteChance(pnum, addedIgn * DND_ADDEDIGNITE_CHANCE)
	)
	{
		int amt = GetIgniteDuration(pnum);
		int current_ign_time = CheckActorInventory(victim, "DnD_IgniteTimer");
		int ign_flags = DND_IGNITEFLAG_CANPROLIF;

		// Price this application even if it cannot start the script: a refresh only pushes the timer
		// out, so without parking the number the FIRST application would set the burn for its life.
		int tick_dmg = GetIgniteTickDamage(pnum, victim, wepid, scaleIgn ? dmg_within_tic : 0);

		// Ownership is the SCRIPT REFCOUNT, never the timer. Inventory outlives scripts, so a stale
		// timer used to trap the monster in the refresh branch for good -- on fire, taking nothing.
		// A script that ends for ANY reason frees its count, so this is self healing.
		if(!CheckActorInventory(victim, "DnD_IgniteScripts")) {
			SetActorInventory(victim, "DnD_IgniteTimer", amt);
			SetActorInventory(victim, "DnD_CurrentIgniteDamage", tick_dmg);

			// Claim BEFORE launching: ACS_NamedExecuteWithResult runs the first iteration inline, so a
			// second application in the same tic would otherwise start a second script on one timer.
			GiveActorInventory(victim, "DnD_IgniteScripts", 1);
			ACS_NamedExecuteWithResult("DnD Monster Ignite", victim, wepid, ign_flags, tick_dmg);
		}
		else {
			// only replace timer if this is higher
			SetActorInventory(victim, "DnD_IgniteTimer", Max(amt, current_ign_time));
			// and the damage on the same rule -- a refresh may improve a burn, never weaken one
			// somebody else's stats are paying for
			if(tick_dmg > CheckActorInventory(victim, "DnD_CurrentIgniteDamage"))
				SetActorInventory(victim, "DnD_CurrentIgniteDamage", tick_dmg);
		}
	}
}

// What the ignite prices itself off. The two flags promise different things and now read differently:
// SCALEIGNITE is "scales off the FIRE in the hit", ADDEDIGNITE is "this damage is contributed on top",
// ie. the whole hit. Reading the fire subtotal for both made them non-monotonic -- a weapon with no
// fire at all falls back to the total, but a small added fire component returned only that slice, so
// adding a LITTLE fire made the burn far weaker than none. It also tied the magnitude to winning a
// mixed tic slot, which varies per victim inside one crowd.
int GetIgniteScaleSource(int pnum, int m_id, int tic_flags) {
	if(tic_flags & DND_DAMAGETICFLAG_ADDEDIGNITE)
		return PlayerDamageTic[pnum].total[m_id];

	return GetTicElementDamage(pnum, m_id, DND_TICELEM_FIRE);
}

// ASSUMPTION: PLAYER RUNS THIS! -- care if adapting this later for other things
Script "DnD Damage Accumulate" (int victim_data, int wepid, int flags, int damage_type) {
	int pnum = PlayerNumber();
	int wep_neg = victim_data >> DND_DAMAGE_ACCUM_SHIFT;
	victim_data &= DND_MONSTER_TICDATA_BITMASK;

	int ox = PlayerDamageVector[pnum].x;
	int oy = PlayerDamageVector[pnum].y;
	int oz = PlayerDamageVector[pnum].z;

	int victim_tid = victim_data + DND_MONSTERTID_BEGIN;
	int temp;

	// Health before anything this tic landed -- this instance is launched inline from the first
	// HandleDamageDeal, ahead of the engine applying anything. Bounds the heal back below.
	int hp_at_launch = GetActorProperty(victim_tid, APROP_HEALTH);

	Delay(const:1);

	/*
		THINGS THAT ALTER DAMAGE IN ANY WAY AFTER ACCUMULATION END UP HERE!!!!
	*/
	int prev_dmg = PlayerDamageTic[pnum].total[victim_data];
	int more_dmg = 100; // baseline damage, 100% is regular value

	// desolator damage increase
	if(damage_type == DND_DAMAGETYPE_DESOLATOR) {
		if(!CheckActorInventory(victim_tid, "DesolatorStackCounter")) {
			GiveActorInventory(victim_tid, "DesolatorStackTimer", 52);
			ACS_NamedExecuteAlways("DND Desolator Debuff FX", 0, victim_tid);
		}
		else
			GiveActorInventory(victim_tid, "DesolatorStackTimer", 17);
		GiveActorInventory(victim_tid, "DesolatorStackCounter", 1);
	}

	// increase damage they take from elemental attacks for each stack
	// poison damage gets sent already scaled, dont scale twice
	if(IsElementalDamageType(damage_type) && !(flags & DND_DAMAGETICFLAG_NOPOISONSTACK)) {
		temp = CheckActorInventory(victim_tid, "DesolatorStackCounter");
		// 10% increase from desolator
		if(temp)
			more_dmg = more_dmg * (100 + temp * DND_DESOLATOR_DMG_GAIN) / 100;
	}

	if((flags & DND_DAMAGETICFLAG_EXTRATOUNDEAD) && HasMonsterTrait(victim_data, DND_SILVER_WEAKNESS))
		more_dmg = more_dmg * (100 + DND_EXTRAUNDEADDMG_MULTIPLIER) / 100;

	// check blockers take more dmg modifier
	if(HasMonsterTrait(victim_data, DND_ISBLOCKING) && (temp = PlayerModData[pnum].vals[PSTAT_BLOCKERS_MOREDMG]))
		more_dmg = more_dmg * (100 + ((temp * 100) >> 16)) / 100;
	
	// buff effectiveness is the maximum of what the monster might have had previously from another player vs. most up-to-date, which is overwritten into its DnD_OverloadDamage item
	if(CheckActorInventory(victim_tid, "DnD_OverloadTimer"))
		more_dmg = more_dmg * (100 + DND_BASE_OVERLOADBUFF + CheckActorInventory(victim_tid, "DnD_OverloadDamage")) / 100;
	
	if(IsMonsterIdBoss(MonsterProperties[victim_data].id) && (temp = PlayerModData[pnum].vals[PSTAT_IMP_MOREDAMAGETOBOSSES]))
		more_dmg = more_dmg * (100 + temp) / 100;

	// additional damage vs frozen enemies modifier
	if(CheckActorInventory(victim_tid, "DnD_FreezeTimer") && (temp = PlayerModData[pnum].vals[PSTAT_ESS_ERYXIA]))
		more_dmg = more_dmg * (100 + ((temp * 100) >> 16)) / 100;
		
	// 50% more damage taken, so dmg * 3 / 2
	if(CheckActorInventory(victim_tid, "DemonSealResistDebuff"))
		more_dmg = more_dmg * (100 + DEMONSEAL_DMGTAKEN_DEBUFF) / 100;

	// General buff effects, includes curses and stuff too
	more_dmg = more_dmg * HandleGenericPlayerMoreDamageEffects(pnum, wepid) / 100;
	
	// ACCESSORY EFFECTS -- applied per element rather than folded into more_dmg, because everything
	// inside HandlePlayerBuffs is keyed on the damage type. Multiplying the whole tic by it would let
	// a "more cold damage" buff scale the physical portion of the same hit too.
	PlayerDamageTic[pnum].total[victim_data] = ApplyPerElementBuffDamage(
		pnum, victim_tid, victim_data, wepid, flags | GetMixedTicFlags(pnum, victim_data), damage_type,
		PlayerDamageTic[pnum].total[victim_data]
	);

	// check hobo's level 50 perk here, after 1 tic, and deal the extra damage with "_NoPain" attached
	// this is the most efficient way to handle this bonus as then we won't be calculating the distance check PER PELLET!!!
	// plus we get to adjust the push factor and other things before they affect the monster proper here
	bool isHoboPowerApplicable = !(wep_neg & 1) && wepid >= 0 && IsBoomstick(wepid) && HasClassPerk_Fast(DND_PLAYER_HOBO, 3);
	if(isHoboPowerApplicable && CheckInventory("Hobo_ShotgunFrenzyTimer")) {
		temp = fdistance_delta(ox - GetActorX(victim_tid), oy - GetActorY(victim_tid), oz - GetActorZ(victim_tid));
		temp -= FixedMul(GetActorProperty(victim_tid, APROP_RADIUS) + DND_PLAYER_RADIUS, 1.207);

		// give leeway for max scale window
		if(temp < DND_HOBO_SHOTGUNMINBESTDIST)
			temp = DND_HOBO_SHOTGUNMINBESTDIST;
		temp >>= 16;

		if(temp <= DND_HOBO_SHOTGUNFALLOFFDIST) {
			temp = LinearMap(temp, DND_HOBO_SHOTGUNMINBESTDIST_INT, DND_HOBO_SHOTGUNFALLOFFDIST, 0, 100);
			// 100 + (100 - temp) would mean 200 - temp, and we scale inversely with distance so if we are farthest, we will be getting 100 to be dealing the same amount of damage anyway
			more_dmg = more_dmg * (100 + DND_HOBO_SHOTGUNDISTMOREDMG * (1 + HasClassPerk_Fast(DND_PLAYER_HOBO, 5)) - temp) / 100;
		}
	}

	// deadly strike doubles damage
	if
	(
		(IsMeleeWeapon(wepid) || (flags & DND_DAMAGETICFLAG_CONSIDERMELEE)) && !(flags & DND_DAMAGETICFLAG_DOT) &&
		(temp = PlayerModData[pnum].vals[PSTAT_DEADLYSTRIKE]) && random(1, 100) <= temp
	) 
	{
		ACS_NamedExecuteAlways("DnD Special Fx Spawner", 0, victim_tid, INV_DEADLYSTRIKE);
		more_dmg *= 2;
	}

	// moved crit at the end here -- copied code to save from 1 extra if check to see if more_dmg or crit is non-zero
	if(flags & DND_DAMAGETICFLAG_CRIT) {
		if(more_dmg != 100)
			PlayerDamageTic[pnum].total[victim_data] = MulPercent_Exact(PlayerDamageTic[pnum].total[victim_data], more_dmg);

		// amplify the overall damage as a crit here -- wepid negativity check happens inside np
		more_dmg = GetCritModifier(pnum, victim_tid, wepid);

		PlayerDamageTic[pnum].total[victim_data] = MulPercent_Exact(PlayerDamageTic[pnum].total[victim_data], more_dmg);

		HandleHunterTalisman();
	}
	else if(more_dmg != 100)
		PlayerDamageTic[pnum].total[victim_data] = MulPercent_Exact(PlayerDamageTic[pnum].total[victim_data], more_dmg);

	//printbold(s:"before ", d:prev_dmg, s: " new dmg: ", d:PlayerDamageTic[pnum].total[victim_data], s: " ", d:more_dmg);

	// deal the damage difference between the crit and original on top, like hobo thing -- note use of Special_NoPain
	if(PlayerDamageTic[pnum].total[victim_data] > prev_dmg) {
		prev_dmg = PlayerDamageTic[pnum].total[victim_data] - prev_dmg;
		HandleMonsterDeathConfirm(victim_tid, prev_dmg);
		Thing_Damage2(victim_tid, prev_dmg, "Special_NoPain");
	}
	else if(IsActorAlive(victim_tid) && PlayerDamageTic[pnum].total[victim_data] != prev_dmg) {
		// we have reduced the overall damage instead, heal for the difference instead -- hope we dont need HealThing here...
		prev_dmg = GetactorProperty(victim_tid, APROP_HEALTH) + prev_dmg - PlayerDamageTic[pnum].total[victim_data];
		if(prev_dmg > MonsterProperties[victim_data].maxhp)
			prev_dmg = MonsterProperties[victim_data].maxhp;

		// This branch assumes the engine already took EXACTLY prev_dmg. Insurance only now that the
		// bank equals the delivery -- it used to be taken before fortify absorption and before the
		// borrowed time return, so the gap came back as health nothing had removed.
		// Inert when the assumption holds: health is then hp_at_launch - prev_dmg, so the sum is
		// hp_at_launch - new, already under this bound.
		if(prev_dmg > hp_at_launch)
			prev_dmg = hp_at_launch;

		SetActorProperty(victim_tid, APROP_HEALTH, prev_dmg);
	}

	// prev_dmg is unused from here below

	/*
		DMG ALTERING ENDS BY HERE! NO MORE! FINALIZED!
	*/

	temp = PlayerModData[pnum].vals[PSTAT_EX_CHANCEGAINXCHARGE];
	if(temp && (IsMonsterIdBoss(MonsterProperties[victim_data].id) || (MonsterProperties[victim_data].flags & DND_MONFLAG_ISELITE)) && random(1, 100) <= temp) {
		// give a charge
		HandlePlayerBuffAssignment(pnum, 0, BTI_FRENZYCHARGE + ReadPlayerModExtra(pnum, INV_EX_CHANCEGAINXCHARGE));
	}

	// moved here as it's simpler and more efficient to run this function after 1 tic rather than immediately with multiple instances
	IncrementStatistic(DND_STATISTIC_DAMAGEDEALT, PlayerDamageTic[pnum].total[victim_data], pnum + P_TIDSTART);

	// do the real pushing after 1 tic of dmg data has been accumulated and we have non-zero damage in effect
	// wep_neg here contains 2 bits: was it negative at 1st bit and was it a one time ripper in 2nd bit
	if((flags & DND_DAMAGETICFLAG_PUSH) && PlayerDamageTic[pnum].total[victim_data] > 0)
		HandleDamagePush(2 * PlayerDamageTic[pnum].total[victim_data], ox, oy, oz, victim_tid, wep_neg & 2);
	
	// has wepid non neg
	if(!(wep_neg & 1)) {
		// check if player has lifesteal, if they do reward some hp back
		if(!HasMonsterTrait(victim_data, DND_BLOODLESS) && !(flags & DND_DAMAGETICFLAG_DOT))
			HandleLifesteal(pnum, wepid, flags, PlayerDamageTic[pnum].total[victim_data]);
	}

	// ox, oy and oz arent used below
	
	// if cold damage, add stacks of slow and check for potential freeze chance
	// do these if only the actor was alive after the tic they received dmg
	if(IsActorAlive(victim_tid)) {
		// The flags of EVERY hit that contributed this tic, not just the one that happened to land
		// first and launch this instance. Reads back as plain `flags` for anyone without added
		// damage, since nothing was recorded for them.
		int tic_flags = flags | GetMixedTicFlags(pnum, victim_data);

		// INV_INC_CRITFORDOT makes crit the trigger for ailments instead of a damage bonus (see
		// IATTR_TINC4); true and inert without it. Read off tic_flags so a crit from ANY component of
		// a mixed hit counts. Application only -- proliferation and spread are not re-tested.
		bool can_ail = !PlayerModData[pnum].vals[PSTAT_INC_CRITFORDOT] || !!(tic_flags & DND_DAMAGETICFLAG_CRIT);

		// Independent tests, not an if/else chain. A chain can only ever fire ONE ailment, so a
		// physical weapon with added cold would chill or bleed depending on which component landed
		// first and never both.
		if(can_ail && (tic_flags & DND_DAMAGETICFLAG_ICE))
			HandleChillEffects(pnum, victim_tid);

		if(can_ail && ((tic_flags & DND_DAMAGETICFLAG_FIRE) || (tic_flags & DND_DAMAGETICFLAG_ADDEDIGNITE))) // should be able to ign if it has addedignite flag even if damagetype isnt fire!
			HandleIgniteEffects(pnum, victim_tid, wepid, tic_flags, GetPlayerIgniteAddedDmg(pnum, wepid, GetIgniteScaleSource(pnum, victim_data, tic_flags)));

		if(can_ail && ((tic_flags & DND_DAMAGETICFLAG_LIGHTNING) || (PlayerModData[pnum].vals[PSTAT_INC_ALLOVERLOAD] && (tic_flags & (DND_DAMAGETICFLAG_ICE | DND_DAMAGETICFLAG_FIRE | DND_DAMAGETICFLAG_POISON)))))
			HandleOverloadEffects(pnum, victim_tid);

		if(can_ail && (tic_flags & DND_DAMAGETICFLAG_PHYSICAL) && !(tic_flags & DND_DAMAGETICFLAG_DOT))
			HandleBleedEffects(pnum, victim_tid, wepid, GetTicElementDamage(pnum, victim_data, DND_TICELEM_PHYSICAL));

		if(can_ail && (tic_flags & (DND_DAMAGETICFLAG_POISON | DND_DAMAGETICFLAG_INFLICTPOISON)) && !(tic_flags & DND_DAMAGETICFLAG_NOPOISONSTACK) && CheckAilmentImmunity(pnum, victim_data, DND_TOXICBLOOD)) {
			// poison damage deals 10% of its damage per stack over 3 seconds
			// 5% of damage or by the factor -- if factor is with a weapon that already has inflictpoison, it empowers poison of the weapon by +2%
			if(!(tic_flags & DND_DAMAGETICFLAG_SPELL)) {
				ox = GetPlayerWeaponModVal(pnum, wepid, WEP_MOD_POISONFORPERCENTDAMAGE);
				oy = ox + GetWeaponPoisonBaseFactor(wepid);
			}
			else {
				ox = GetSpellPoisonFactor(wepid);
				oy = ox;
			}

			oy = Max(DND_BASE_POISON_FACTOR, oy);
			ox = Max((GetTicElementDamage(pnum, victim_data, DND_TICELEM_POISON) * oy) / 100, 1);

			oy = !CheckActorInventory(victim_tid, "DnD_PoisonStacks");

			if(CheckActorInventory(victim_tid, "DnD_PoisonStacks") < GetPlayerPoisonStacks(pnum))
				GiveActorInventory(victim_tid, "DnD_PoisonStacks", 1);

			ACS_NamedExecuteWithResult("DnD Do Poison Damage", victim_tid, ox, wepid, oy);
			//printbold(s:"poison received by ", d:victim);
		}
		
		// frozen monsters cant retaliate	
		if(!CheckActorInventory(victim_tid, "DnD_FreezeTimer")) {
			if(HasMonsterTrait(victim_data, DND_VIOLENTRETALIATION) && random(1, 100) <= DND_VIOLENTRETALIATION_CHANCE && !CheckActorInventory(victim_tid, "DnD_ViolentRetaliationCooldown")) {
				GiveActorInventory(victim_tid, "DnD_ViolentRetaliationItem", 1);
				GiveActorInventory(victim_tid, "DnD_ViolentRetaliationCounter", 1);
				if(CheckActorInventory(victim_tid, "DnD_ViolentRetaliationCounter") == DND_MAX_VIOLENTRETALIATES) {
					SetActorInventory(victim_tid, "DnD_ViolentRetaliationCounter", 0);
					GiveActorInventory(victim_tid, "DnD_ViolentRetaliationCooldown", 1);
				}
			}
			if(HasMonsterTrait(victim_data, DND_THUNDERSTRUCK) && !CheckInventory("ThunderstruckCooldown")) {
				ACS_NamedExecuteAlways("DnD Thunderstruck", 0, victim_tid);
				GiveInventory("ThunderstruckCooldown", 1);
			}
		}
		GiveActorInventory(victim_tid, "DnD_HurtToken", 1);

		// actor is alive, we can tag with shotgun for hobo perk 50
		if(isHoboPowerApplicable && !CheckInventory("Hobo_ShotgunFrenzyTimer")) {
			// if the window passed, ignore remaining tags
			if(!CheckActorInventory(victim_tid, "Hobo_ShotgunTag_Window"))
				SetActorInventory(victim_tid, "Hobo_ShotgunTag", 0);
			GiveActorInventory(victim_tid, "Hobo_ShotgunTag_Window", 1);
			GiveActorInventory(victim_tid, "Hobo_ShotgunTag", 1);

			if(CheckActorInventory(victim_tid, "Hobo_ShotgunTag") >= DND_HOBO_SHOTGUNTAGLIMIT) {
				SetActorInventory(victim_tid, "Hobo_ShotgunTag", 0);
				GiveInventory("Hobo_ShotgunFrenzyTimer", DND_HOBO_FRENZYBASETIME);
				CalculatePlayerAccuracy(pnum);
				ACS_NamedExecuteAlways("DnD Hobo Frenzy Timer", 0);
			}
		}

		if(!CheckActorInventory(victim_tid, "Doomguy_ChangedColor") && CheckActorInventory(victim_tid, "Doomguy_CanExecute")) {
			GiveActorInventory(victim_tid, "Doomguy_ChangedColor", 1);
			ACS_NamedExecuteWithResult("DnD Doomguy Execute Translation", victim_tid);
		}
	}

	ACS_NamedExecuteWithResult("DnD Damage Numbers", victim_tid, PlayerDamageTic[pnum].total[victim_data], flags);

	if(CheckInventory("Marine_DamageReduction_Timer"))
		GiveInventory("Marine_Perk50_DamageDealt", PlayerDamageTic[pnum].total[victim_data]);
	
	// reset dmg counter on this mob. The latch drops HERE, not after the delay below, or the pair
	// would be blocked from starting a new tic for CRIT_CLEAR_WAIT_TIME.
	PlayerDamageTic[pnum].total[victim_data] = 0;
	ReleaseMixedTicSlot(pnum, victim_data);
	ClearDamageTicRunning(pnum, victim_data);

	Delay(const:CRIT_CLEAR_WAIT_TIME);
	UnsetPlayerWeaponCritState(pnum, wepid);
	UnlockPlayerCritState(pnum, wepid);
	
	SetResultValue(0);
}

Script "DnD Damage Numbers" (int tid, int dmg, int flags) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber() || !GetCVar("dnd_dmgnum"))
		Terminate;

	bool show_k = dmg > 9999 && dmg < 500'000;
	bool show_m = dmg >= 500'000;
	if(show_k)
		dmg /= 1000;
	else if(show_m) {
		dmg /= 10000;
	}
	
	int digit_pos = 1;
	int r = GetActorProperty(tid, APROP_RADIUS) / 2;
	int x = GetActorX(tid) + random(-r, r) / 2;
	int y = GetActorY(tid) + random(-r, r) / 2;
	int z = GetActorZ(tid) + random(0.0, 16.0);
	
	// dead monsters have their height divided by 4
	if(IsActorAlive(tid))
		z += GetActorProperty(tid, APROP_HEIGHT);
	else
		z += GetActorProperty(tid, APROP_HEIGHT) * 4;
		
	// Log(f:x, s: " ", f:y, s: " ", f:z);
	
	if(!show_m) {
		while(dmg > 0) {
			SpawnForced(StrParam(s:"Digit", d:digit_pos, s:"Num", d:dmg % 10), x, y, z, DND_DAMAGENUMBER_TID);
			dmg /= 10;
			++digit_pos;
		}
	}
	else {
		// millions show dot to make it smaller looking
		while(dmg > 0) {
			if(digit_pos < 3)
				SpawnForced(StrParam(s:"Digit", d:digit_pos, s:"Num", d:dmg % 10), x, y, z, DND_DAMAGENUMBER_TID);
			else
				SpawnForced(StrParam(s:"DigitDot", d:digit_pos, s:"Num", d:dmg % 10), x, y, z, DND_DAMAGENUMBER_TID);
			dmg /= 10;
			++digit_pos;

			if(digit_pos == 3)
				SpawnForced(StrParam(s:"Digit", d:digit_pos, s:"Dot"), x, y, z, DND_DAMAGENUMBER_TID);
		}
		// if not zero we print it anyway so
		if(!dmg && digit_pos == 3)
			SpawnForced(StrParam(s:"Digit", d:4, s:"Num", d:0), x, y, z, DND_DAMAGENUMBER_TID);
	}
	
	if(show_k)
		SpawnForced("ThousandSymbol", x, y, z, DND_DAMAGENUMBER_TID);
	else if(show_m)
		SpawnForced("MillionSymbol", x, y, z, DND_DAMAGENUMBER_TID);
	
	SetActorVelocity(DND_DAMAGENUMBER_TID, random(-0.5, 0.5), random(-0.5, 0.5), random(0.0, 0.5), false, false);
	
	// set translation depending on crit dealt
	if(flags & DND_DAMAGETICFLAG_CRIT) {
		SetActorProperty(DND_DAMAGENUMBER_TID, APROP_SCALEX, 1.25);
		SetActorProperty(DND_DAMAGENUMBER_TID, APROP_SCALEY, 1.25);
		Thing_SetTranslation(DND_DAMAGENUMBER_TID, DND_CRIT_TRANSLATION);
	}
	else if(flags & DND_DAMAGETICFLAG_LESSENED) {
		// apply the gray translation
		Thing_SetTranslation(DND_DAMAGENUMBER_TID, DND_RESIST_TRANSLATION);
	}
	
	Thing_ChangeTID(DND_DAMAGENUMBER_TID, 0);
	
	SetResultValue(0);
}

// HandlePoisonDamage
Script "DnD Do Poison Damage" (int victim, int dmg, int wepid, int firstEntry) {
	int pnum = PlayerNumber();
	int source = pnum + P_TIDSTART;
	int time_limit = GetPoisonDuration(pnum);
	int trigger_tic = GetPoisonTicrate(pnum);
	
	int tic_temp = trigger_tic;
	int counter = 0;
	int stacks = CheckActorInventory(victim, "DnD_PoisonStacks");
	int temp, i;
	int mid = victim - DND_MONSTERTID_BEGIN;

	// How much of the stack's own damage INV_POISON_TICDMG adds back per tic already dealt, the count
	// of those tics, and the bonus the two produce. Read once here, like chegovax does on the ignite
	// side, but capped differently -- see the ramp below.
	int dmg_tic_buff = PlayerModData[pnum].vals[PSTAT_POIS_TICDMG];
	int poison_ticks = 0;
	int dead_tics = 0;
	int ramp = 0;

	dmg = GetPoisonDOTDamage(pnum, dmg, victim, wepid);

	static mon_dot_cache_T dot_cache[DND_MAX_MONSTERS][MAX_DOT_STACKS];

	// if monster has only 1 stack that means they enter here for the first time, so reset
	if(firstEntry) {
		for(temp = 0; temp < MAX_DOT_STACKS; ++temp) {
			dot_cache[mid][temp].dmg = 0;
			dot_cache[mid][temp].tics = 0;
		}

		dot_cache[mid][0].dmg = dmg;
		dot_cache[mid][0].tics = time_limit;
		//printbold(s:"adding to cache first element dmg ", d:dmg, s:" tics: ", f:time_limit);
	}
	else {
		// just add it to the cache and terminate
		counter = bcs::INT_MAX, i = 0;
		for(temp = 0; temp < MAX_DOT_STACKS; ++temp) {
			if(!dot_cache[mid][temp].dmg) {
				dot_cache[mid][temp].dmg = dmg;
				dot_cache[mid][temp].tics = time_limit;
				dmg = -1;
				break;
			}
			else if(counter > dot_cache[mid][temp].dmg) {
				// find the min in case this is full
				counter = dot_cache[mid][temp].dmg;
				i = temp;
			}
		}

		// did not get placed into the list, replace the lowest damage one with this one
		if(dmg != -1 && counter != bcs::INT_MAX) {
			dot_cache[mid][i].dmg = dmg;
			dot_cache[mid][i].tics = time_limit;
		}

		//printbold(s:"adding to cache dmg ", d:dmg, s:" tics: ", f:time_limit);
		Terminate;
	}
	
	// divide trigger tic count by half to make it twice as fast -- if poison ticrate is 100% reduction we'll do poison damage at every 2 tics, which is the most one would need
	if(CheckUniquePropertyOnPlayer(pnum, PUP_POISONTICSTWICE))
		trigger_tic /= 2;

	if(HasActorClassPerk_Fast(source, DND_PLAYER_WANDERER, 2))
		AddMonsterAilment(source, victim, DND_AILMENT_POISON);

	//printbold(s:"proceeding to loop dot on ", d:victim);
		
	while((stacks = CheckActorInventory(victim, "DnD_PoisonStacks")) && IsActorAlive(victim) && IsActorAlive(source)) {
		if(counter >= trigger_tic) {
			// dmg to deal is the sum of all dot damages
			time_limit = 0;
			dmg = 0;
			for(i = 0; i < MAX_DOT_STACKS; ++i) {
				if(dot_cache[mid][i].dmg) {
					dmg += dot_cache[mid][i].dmg;
					++time_limit;

					// tic the dot, if its the final tic of it we need to mark it as removed
					dot_cache[mid][i].tics -= tic_temp;
					if(dot_cache[mid][i].tics <= 0) {
						// removed
						dot_cache[mid][i].dmg = 0;
						dot_cache[mid][i].tics = 0;
						TakeActorInventory(victim, "DnD_PoisonStacks", 1);
						//printbold(s:"time ran out, stacks: ", d:CheckActorInventory(victim, "DnD_PoisonStacks"));
					}

					//printbold(s:"finish loop? ", d:time_limit, s:" vs ", d:stacks);

					if(time_limit >= stacks)
						break;
				}
			}

			// Gated by DAMAGE, not by time. Chegovax caps the tic COUNT, so its ceiling rides on the
			// mod roll and a bigger roll simply ends up worth more; this one caps the BONUS itself at
			// DND_MAX_POISON_TICDMG_CAP, so every roll converges on the same ceiling and a bigger roll
			// only reaches it sooner. That makes the mod a "how fast does the poison spin up" stat
			// rather than a "how high does it go" one.
			// The ramp lands on the SUM, after every live stack has been added up, so a stack that
			// arrives mid poison inherits whatever the monster has already earned rather than starting
			// its own -- an ignite re-application under chegovax behaves the same way.
			ramp = Min(dmg_tic_buff * poison_ticks, DND_MAX_POISON_TICDMG_CAP);
			if(ramp)
				dmg += dmg * ramp / 100;

			if(CheckFlag(victim, "SHOOTABLE")) {
				temp = HandleDamageDeal(source, victim, dmg, DND_DAMAGETYPE_POISON, wepid, DND_DAMAGEFLAG_NOPOISONSTACK | DND_DAMAGEFLAG_NOPUSH, 0, 0, 0, DND_ACTORFLAG_PAINLESS | DND_ACTORFLAG_FOILINVUL | DND_ACTORFLAG_ISDAMAGEOVERTIME, wepid < 0);
				if(temp > 0)
					Thing_Damage2(victim, temp, "Special_NoPain");
				ACS_NamedExecuteAlways("DnD Spawn Poison FX", 0, victim, stacks);

				// This ailment cannot hurt this monster. Drop it instead of holding its slot -- see
				// IsAilmentTicWasted. Breaking here lands in the teardown below, which clears the timer, so
				// the monster goes back to being ailable immediately.
				if(IsAilmentTicWasted(victim, temp)) {
					if(++dead_tics >= DND_AILMENT_DEADTICS)
						break;
				}
				else
					dead_tics = 0;

				// Counted only where damage actually landed, and stopped once the bonus has reached the
				// cap -- past that point another tic changes nothing, and the counter must not run free:
				// fresh stacks keep DnD_PoisonStacks non zero, so a monster held poisoned holds this
				// loop open indefinitely and dmg_tic_buff * poison_ticks would eventually overflow.
				// The dmg_tic_buff test is what stops a player without the mod counting forever, since
				// a zero buff never reaches the cap.
				if(dmg_tic_buff && ramp < DND_MAX_POISON_TICDMG_CAP)
					++poison_ticks;
			}
			
			// go up to the next threshold for next tic etc.
			trigger_tic += tic_temp;
		}
		counter += DND_POISON_CHECKRATE;
		Delay(const:DND_POISON_TICCHECK);
	}

	SetActorInventory(victim, "DnD_PoisonStacks", 0);

	if(HasActorClassPerk_Fast(source, DND_PLAYER_WANDERER, 2))
		RemoveMonsterAilment(victim, DND_AILMENT_POISON);

	if(IsActorAlive(victim)) {
		SetResultValue(0);
		Terminate;
	}

	if(HasActorMasteredPerk(source, STAT_ACRM) && random(0, 1.0) <= DND_ACRIMONY_RECOVERCHANCE)
		HandleHealthPickup(DND_ACRIMONY_RECOVERPERCENT, 0, true, true);

	temp = PlayerModData[pnum].vals[PSTAT_INC_POISONSPREAD];
	if(random(1, 100) <= temp) {
		// DONT USE PNUM FOR PLAYER ANYMORE HERE, SOURCE ALREADY HAS IT
		// PNUM STORES INDEX FOR STORING DISTANCE INFO
		// reuse dot cache static array here -- MAXDOTSTACKS is high enough for this (16 vs 8)
		pnum = DND_MAX_MONSTERS - (pnum + 1) * DND_INC_POISONSPREAD_COUNT;
		for(temp = 0; temp < DND_INC_POISONSPREAD_COUNT; ++temp) {
			// dmg holds tid of monster, tics is distance
			dot_cache[pnum][temp].dmg = 0;
			dot_cache[pnum][temp].tics = 0;
		}

		int j, k;
		temp = 0;
		tic_temp = 0;
		for(counter = 0; counter < InformationInLevel[LEVELINFO_TID_MONSTER]; ++counter) {
			i = UsedMonsterTIDs[counter];
			if(IsActorAlive(i) && CheckFlag(i, "ISMONSTER")) {
				dmg = fdistance(victim, i);
				if(dmg < DND_INC_POISONSPREAD_R && CheckSight(victim, i, CSF_NOBLOCKALL)) {
					// insert sorted
					temp = tic_temp;
					// while our calc dist > alloc dist, keep going -- we add things to the end
					// if we come by a point where we are smaller, shift things
					for(j = 0; j < temp && dmg > dot_cache[pnum][j].tics; ++j);

					// we know where to add, check if we must shift (if we should)
					if(j < temp) {
						// less, so that means we are in-between things
						// push everything for insertion
						// this is needed to move in 0 index shifts
						if(temp == DND_INC_POISONSPREAD_COUNT)
							--temp;
						
						for(k = temp; k > j; --k) {
							// slide data
							dot_cache[pnum][k].tics = dot_cache[pnum][k - 1].tics;
							dot_cache[pnum][k].dmg = dot_cache[pnum][k - 1].dmg;
						}
					}
					
					dot_cache[pnum][j].tics = dmg;
					dot_cache[pnum][j].dmg = i;
					
					if(tic_temp < DND_INC_POISONSPREAD_COUNT)
						++tic_temp;
				}
			}
		}
		
		// we have things to prolif to
		if(tic_temp) {
			// obtain the damage sum of poisons available then send them summed up as one poison with total duration
			dmg = 0;
			for(temp = 0; temp < MAX_DOT_STACKS; ++temp) {
				if(!dot_cache[mid][temp].dmg)
					dmg += dot_cache[mid][temp].dmg;
			}

			for(i = 0, j = 0; i < DND_INC_POISONSPREAD_COUNT; ++i) {
				if(dot_cache[pnum][i].dmg) {
					//printbold(s:"prolif to ", d:tlist[pnum][i].tid);
					counter = !CheckActorInventory(dot_cache[pnum][i].dmg, "DnD_PoisonStacks");

					if(CheckActorInventory(dot_cache[pnum][i].dmg, "DnD_PoisonStacks") < GetPlayerPoisonStacks(source - P_TIDSTART))
						GiveActorInventory(dot_cache[pnum][i].dmg, "DnD_PoisonStacks", 1);

					ACS_NamedExecuteWithResult("DnD Do Poison Damage", dot_cache[pnum][i].dmg, dmg, wepid, counter);

					// abort if we reached our count
					++j;
					if(j == tic_temp)
						break;
				}
			}
		}
	}

	SetResultValue(0);
}

Script "DnD Spawn Poison FX" (int orig, int amt) CLIENTSIDE {
	int r = GetActorProperty(orig, APROP_RADIUS);
	int h = GetActorProperty(orig, APROP_HEIGHT);
	for(int i = 0; i <= amt; ++i) {
		SpawnForced("DnD_PoisonFX", GetActorX(orig) + random(-r, r), GetActorY(orig) + random(-r, r), GetActorZ(orig) + random(0, h), 0);
		Delay(random(1, 3));
	}
}

Script "DnD Monster Chill" (int victim, int pnum) {
	int cur_stacks;
	int base_speed = GetActorProperty(victim, APROP_SPEED);
	
	// revoke monster's extra fast flag if it has it
	if(HasMonsterTrait(victim - DND_MONSTERTID_BEGIN, DND_HASTE))
		GiveActorInventory(victim, "UnMakeFaster", 1);

	if(HasClassPerk_Fast(DND_PLAYER_WANDERER, 2))
		AddMonsterAilment(ActivatorTID(), victim, DND_AILMENT_CHILL);
	
	while((cur_stacks = CheckActorInventory(victim, "DnD_ChillStacks"))) {
		// slow down
		SetActorProperty(victim, APROP_SPEED, FixedMul(base_speed, 1.0 - GetChillEffect(pnum, cur_stacks)));
		ACS_NamedExecuteAlways("DnD Monster Chill FX", 0, victim);
		Delay(const:TICRATE);
		TakeActorInventory(victim, "DnD_ChillStacks", 1);
	}
	
	SetActorProperty(victim, APROP_SPEED, base_speed);
	
	// retain super fast property after chill ends
	if(HasMonsterTrait(victim - DND_MONSTERTID_BEGIN, DND_HASTE))
		GiveActorInventory(victim, "MakeFaster", 1);

	if(HasClassPerk_Fast(DND_PLAYER_WANDERER, 2))
		RemoveMonsterAilment(victim, DND_AILMENT_CHILL);
}

Script "DnD Bleed FX" (int tid, int isRobot) CLIENTSIDE {
	SetActivator(tid);

	int r = GetActorProperty(tid, APROP_RADIUS);
	int h = GetActorProperty(tid, APROP_HEIGHT);

	str blood_fx = "BloodFXSpawner";
	if(isRobot)
		blood_fx = "RobotOilFXSpawner";

	for(int i = 0; i < 5; ++i) {
		Delay(const:2);
		if(!isAlive())
			Terminate;

		GiveActorInventory(tid, blood_fx, 1);
		//SpawnForced("NashGore_Blood", GetActorX(tid) + random(-r, r) / 2, GetActorY(tid) + random(-r, r) / 2, GetActorZ(tid) + (random(16.0, h + 32.0)) / 2, 0);

		Delay(const:5);
	}
}

Script "DnD Monster Chill FX" (int tid) CLIENTSIDE {
	SetActivator(tid);
	
	for(int i = 0; i < 5; ++i) {
		Delay(const:7);
		
		// don't continue if actor no longer exists -- returns 0 if thats the case
		if(!ActivatorTID())
			Terminate;
		
		if(random(0, 1))
			GiveInventory("DnD_ChillWindSpawner", 1);
	}
}

Script "DnD Monster Freeze" (int victim) {
	SetActorState(victim, "Frozen", 0);
	
	// make sure to check BEFORE actually giving NOPAIN property... this would give the trait too, oopsie here!
	bool hasNoPain = HasMonsterTrait(victim - DND_MONSTERTID_BEGIN, DND_NOPAIN);
	
	GiveActorInventory(victim, "MakeNoPain", 1);

	if(HasClassPerk_Fast(DND_PLAYER_WANDERER, 2))
		AddMonsterAilment(ActivatorTID(), victim, DND_AILMENT_FREEZE);
	
	// actor flags dont get changed properly this way for some reason
	//printbold(s:"actor flag: ", d:CheckFlag(victim, "NOPAIN"));
	int tics = 0;
	while(CheckActorInventory(victim, "DnD_FreezeTimer")) {
		// create freeze fx and adjust it every 5 tics
		ACS_NamedExecuteWithResult("DnD Monster Freeze Adjust", victim, tics, tics >= 2, CheckActorInventory(victim, "DnD_FreezeTimer") == 1);
		Delay(const:6);
		TakeActorInventory(victim, "DnD_FreezeTimer", 1);
		tics = (tics + 1) % 4;
	}

	if(HasClassPerk_Fast(DND_PLAYER_WANDERER, 2))
		RemoveMonsterAilment(victim, DND_AILMENT_FREEZE);
	
	// remove frozen nopain thing if monster didnt have it before
	if(!hasNoPain)
		GiveActorInventory(victim, "TakeNoPain", 1);
	SetResultValue(0);
}

Script "DnD Monster Freeze Adjust" (int victim, int tics, int reverse, int is_last) CLIENTSIDE {
	SpawnForced("FrozenFX", GetActorX(victim), GetActorY(victim), GetActorZ(victim) + 16.0, DND_FROZENFX_TID);
	SetActorProperty(DND_FROZENFX_TID, APROP_MASTERTID, victim);
	// 127 and 121 are sprite dimensions
	SetActorProperty(DND_FROZENFX_TID, APROP_SCALEX, (GetActorProperty(victim, APROP_RADIUS) + 16.0) / 127);
	SetActorProperty(DND_FROZENFX_TID, APROP_SCALEY, (GetActorProperty(victim, APROP_HEIGHT) + 16.0) / 121);
	
	SetActivator(DND_FROZENFX_TID);
	Thing_ChangeTID(0, 0);
	
	// always face viewer
	for(int i = 0; i < 6; ++i) {
		int ang = 0;
		if(victim != ConsolePlayerNumber() + P_TIDSTART)
			ang = AngleToFace(victim, ConsolePlayerNumber() + P_TIDSTART);
		else
			ang = GetActorAngle(victim);
		SetActorPosition(0, GetActorX(victim) + 8 * cos(ang), GetActorY(victim) + 8 * sin(ang), GetActorZ(victim) + 16.0, 0);
		if(!reverse)
			SetActorProperty(0, APROP_ALPHA, 1.0 - (i + 5 * tics) * 0.025);
		else
			SetActorProperty(0, APROP_ALPHA, 0.75 + (i + 5 * (tics - 2)) * 0.025);
		Delay(const:1);
		
		// thawing
		if(is_last) {
			SetActorProperty(0, APROP_SCALEX, 8 * GetActorProperty(0, APROP_SCALEX) / 10);
			SetActorProperty(0, APROP_SCALEY, 8 * GetActorProperty(0, APROP_SCALEY) / 10);
		}
	}
	SetResultValue(0);
}

// Bleed player does on monsters
Script "DnD Monster Bleed (Player)" (int victim, int wepid, int dmg) {
	int pnum = PlayerNumber();
	int source = pnum + P_TIDSTART;

	int base_dmg = dmg;

	dmg = GetBleedDamage(pnum, wepid, dmg, victim);

	int next_dmg = dmg; // holds scaled bleed damage

	if(HasActorClassPerk_Fast(source, DND_PLAYER_WANDERER, 2))
		AddMonsterAilment(source, victim, DND_AILMENT_BLEED);

	int dead_tics = 0;
	bool isRobot = IsActorFullRobotic(victim);
	bool isMoving = false;

	do {
		// monsters dont set velxyz fields, so check prev pos vs curr pos for delta -- for extra guarantee on this specific thing, checking for 2 tic window instead
		isMoving = false;

		int px = GetActorX(victim);
		int py = GetActorY(victim);
		int pz = GetActorZ(victim);

		Delay(const:2);

		isMoving = px != GetActorX(victim) || py != GetActorY(victim) || pz != GetActorZ(victim);

		if(CheckFlag(victim, "SHOOTABLE")) {
			ACS_NamedExecuteAlways("DnD Bleed FX", 0, victim, isRobot);
			dmg = HandleDamageDeal(
				source, 
				victim, 
				next_dmg * (1 + 2 * isMoving), 
				DND_DAMAGETYPE_PHYSICAL, wepid, DND_DAMAGEFLAG_NOPUSH, 0, 0, 0, DND_ACTORFLAG_ISDAMAGEOVERTIME | DND_ACTORFLAG_PAINLESS
			);
			if(dmg > 0)
				Thing_Damage2(victim, dmg, "Special_NoPain");

			// This ailment cannot hurt this monster. Drop it instead of holding its slot -- see
			// IsAilmentTicWasted. Breaking here lands in the teardown below, which clears the timer, so
			// the monster goes back to being ailable immediately.
			if(IsAilmentTicWasted(victim, dmg)) {
				if(++dead_tics >= DND_AILMENT_DEADTICS)
					break;
			}
			else
				dead_tics = 0;
		}

		// x 5
		Delay(const:DND_BLEED_TICRATE - 2);

		// Decrement only after the delay, as in the ignite loop. The timer is still what this loop
		// tests, so the decrement, the test and the teardown have to stay in one uninterrupted step.
		TakeActorInventory(victim, "DnD_BleedTimer", 1);

		// update the newer bleed damage now
		px = CheckActorInventory(victim, "DnD_CurrentBleedDamage");
		if(px > base_dmg) {
			next_dmg = GetBleedDamage(pnum, wepid, px, victim);
			base_dmg = px;
		}
	} while(CheckActorInventory(victim, "DnD_BleedTimer") && IsActorAlive(victim));

	// Release ownership the moment the loop stops, and before anything below can terminate or yield.
	// Everything past this point is epilogue that no longer owns the monster, and holding the count
	// across it would block a fresh application for that whole window.
	TakeActorInventory(victim, "DnD_BleedScripts", 1);

	// Bleed was the one ailment that added its token and never took it back, so a monster bled once
	// by a wanderer counted as bleeding forever. CountMonsterAilments feeds the flat resist reduction
	// in FactorResists, so that fed straight into damage. Every other ailment loop pairs these.
	if(HasActorClassPerk_Fast(source, DND_PLAYER_WANDERER, 2))
		RemoveMonsterAilment(victim, DND_AILMENT_BLEED);

	if(!IsActorAlive(victim) && HasActorMasteredPerk(source, STAT_ACRM) && random(0, 1.0) <= DND_ACRIMONY_RECOVERCHANCE) {
		HandleHealthPickup(DND_ACRIMONY_RECOVERPERCENT, 0, true, true);
	}

	SetActorInventory(victim, "DnD_BleedTimer", 0);
	SetActorInventory(victim, "DnD_CurrentBleedDamage", 0);

	SetResultValue(0);
}

// tick_dmg is what one tick is worth, already priced by GetIgniteTickDamage on the applying side.
Script "DnD Monster Ignite" (int victim, int wepid, int ign_flags, int tick_dmg) {
	int pnum = PlayerNumber();
	int source = pnum + P_TIDSTART;

	int dmg_tic_buff = PlayerModData[pnum].vals[PSTAT_ESS_CHEGOVAX];

	// Re-read off the victim every tick, not captured once: a hit on an already burning monster only
	// pushes the timer out, so a captured base would freeze the burn at whatever opened it.
	int base_dmg = tick_dmg;
	int next_dmg;
	int ticks = 0;
	int dead_tics = 0;
	int temp;
	int inc_by;
	int i;

	// this is the value we will use to set the ignite timers on proliferated targets, if any
	int ign_time = CheckActorInventory(victim, "DnD_IgniteTimer");

	if(HasActorClassPerk_Fast(source, DND_PLAYER_WANDERER, 2))
		AddMonsterAilment(source, victim, DND_AILMENT_IGNITE);

	do {
		// pick up any stronger application that landed while we were asleep
		temp = CheckActorInventory(victim, "DnD_CurrentIgniteDamage");
		if(temp > base_dmg)
			base_dmg = temp;

		// chegovax ramps the burn by a share of the base for every tick it has already run, up to
		// DND_MAX_CHEGOVAX_TICS -- see the capped increment below
		next_dmg = base_dmg + (base_dmg * dmg_tic_buff / 100) * ticks;

		// only apply ignite if target is shootable ie. not teleporting
		if(CheckFlag(victim, "SHOOTABLE")) {
			ACS_NamedExecuteAlways("DnD Monster Ignite FX", 0, victim, 2);

			i = HandleDamageDeal(source, victim, next_dmg, DND_DAMAGETYPE_FIRE, wepid, DND_DAMAGEFLAG_NOIGNITESTACK | DND_DAMAGEFLAG_NOPUSH, 0, 0, 0, DND_ACTORFLAG_ISDAMAGEOVERTIME | DND_ACTORFLAG_PAINLESS);

			// Special_NoPain, not SkipHandle. The handler treats both identically; the only difference is
			// the pain table, and only Special_NoPain has an entry (MonsterBase.dec). SkipHandle fell
			// through to the monster's default pain chance, so every tick rolled to stagger it.
			if(i > 0)
				Thing_Damage2(victim, i, "Special_NoPain");

			// This ailment cannot hurt this monster -- drop it instead of holding its slot. See
			// IsAilmentTicWasted. DND_AILMENT_DEADTICS is 2 so the earliest break is the second
			// iteration, which guarantees a Delay before the prolif dispatch gets control back.
			if(IsAilmentTicWasted(victim, i)) {
				if(++dead_tics >= DND_AILMENT_DEADTICS)
					break;
			}
			else
				dead_tics = 0;

			// Capped, not clamped at the read. Refreshes keep this script alive indefinitely, so the
			// counter would climb with no ceiling and overflow the multiply above.
			if(ticks < DND_MAX_CHEGOVAX_TICS)
				++ticks;
		}

		// x 5
		// UNCONDITIONAL, outside the SHOOTABLE test: the prolif dispatch runs this script inline and
		// must reach a delay before returning to a caller still walking the shared tlist row.
		Delay(const:DND_IGNITE_TICKRATE);

		// Decrement AFTER the delay. A nonzero DnD_IgniteTimer is what HandleIgniteEffects reads as
		// "already owned, just refresh", so it must never read 0 while this script is still alive.
		TakeActorInventory(victim, "DnD_IgniteTimer", 1);
	} while(CheckActorInventory(victim, "DnD_IgniteTimer") && IsActorAlive(victim));

	// Release ownership the moment the burn stops looping, and before anything below can terminate or
	// yield. Everything past this point (teardown, proliferation) is epilogue that no longer owns the
	// monster, and holding the count across it would block a fresh application for the whole epilogue.
	TakeActorInventory(victim, "DnD_IgniteScripts", 1);

	if(HasActorClassPerk_Fast(source, DND_PLAYER_WANDERER, 2))
		RemoveMonsterAilment(victim, DND_AILMENT_IGNITE);

	SetActorInventory(victim, "DnD_IgniteTimer", 0);
	SetActorInventory(victim, "DnD_CurrentIgniteDamage", 0);

	if(IsActorAlive(victim)) {
		SetResultValue(0);
		Terminate;
	}

	if(HasActorMasteredPerk(source, STAT_ACRM) && random(0, 1.0) <= DND_ACRIMONY_RECOVERCHANCE)
		HandleHealthPickup(DND_ACRIMONY_RECOVERPERCENT, 0, true, true);
	
	// find N closest targets to victim for igniting
	//printbold(d:canProlif, s: " ", d:!IsActorAlive(victim), s: " ", d:CheckIgniteProlifChance(pnum));
	if((ign_flags & DND_IGNITEFLAG_CANPROLIF) && CheckIgniteProlifChance(pnum)) {
		// Moved here, makes more sense to only check if applicable...
		// check ignite prolif
		int prolif_dist = GetIgniteProlifRange(pnum);
		int prolif_count = GetIgniteProlifCount(pnum);
		
		// clear ignite prolif from subsequent ignites from this monster jumping, we don't want that, too laggy
		ign_flags &= ~DND_IGNITEFLAG_CANPROLIF;
		next_dmg = 0; // used as temp variable
		inc_by = 0; // same as above
		dmg_tic_buff = 0; // same as above...
		
		static dist_tid_pair_T tlist[MAXPLAYERS][DND_MAX_IGNITEPROLIFS];
		
		// init list
		for(i = 0; i < DND_MAX_IGNITEPROLIFS; ++i) {
			tlist[pnum][i].tid = 0;
			tlist[pnum][i].dist = prolif_dist;
		}

		int j, k;
		for(int mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
			i = UsedMonsterTIDs[mn];
			if(IsActorAlive(i) && CheckFlag(i, "ISMONSTER")) {
				next_dmg = fdistance(victim, i);
				// The ignite immunity check belongs here too, not just on the HandleIgniteEffects
				// path. Without it proliferation sets a timer and starts a burn script on a molten
				// blood monster that is supposed to be unignitable: it plays the fire FX and takes
				// nothing, and the nonzero timer it now carries makes every legitimate fire hit for
				// the rest of that duration take the "already burning" branch instead of igniting it.
				if(next_dmg < prolif_dist && CheckSight(victim, i, CSF_NOBLOCKALL) && CheckAilmentImmunity(pnum, i - DND_MONSTERTID_BEGIN, DND_MOLTENBLOOD)) {
					// insert sorted
					inc_by = dmg_tic_buff;
					// while our calc dist > alloc dist, keep going -- we add things to the end
					// if we come by a point where we are smaller, shift things
					for(j = 0; j < inc_by && next_dmg > tlist[pnum][j].dist; ++j);

					// The list is already full and this one is farther than everything in it, so it
					// has no slot -- drop it instead of writing past the end. j can only reach
					// prolif_count here, and at the DND_MAX_IGNITEPROLIFS cap that index is one past
					// this player's row, ie. the next player's first entry.
					if(j >= prolif_count)
						continue;

					// we know where to add, check if we must shift (if we should)
					if(j < inc_by) {
						// less, so that means we are in-between things
						// push everything for insertion
						// this is needed to move in 0 index shifts
						if(inc_by == prolif_count)
							--inc_by;
						
						for(k = inc_by; k > j; --k) {
							// slide data
							tlist[pnum][k].dist = tlist[pnum][k - 1].dist;
							tlist[pnum][k].tid = tlist[pnum][k - 1].tid;
						}
					}
					
					tlist[pnum][j].dist = next_dmg;
					tlist[pnum][j].tid = i;
					
					if(dmg_tic_buff < prolif_count)
						++dmg_tic_buff;
				}
			}
		}
		
		//printbold(s:"check prolif ", d:dmg_tic_buff);
		// we have things to prolif to
		if(dmg_tic_buff) {
			//printbold(s:"begin prolif");
			for(i = 0, j = 0; i < prolif_count; ++i) {
				if(tlist[pnum][i].tid) {
					//printbold(s:"prolif to ", d:tlist[pnum][i].tid);
					// Ownership is the script refcount here too, for the same reason it is in
					// HandleIgniteEffects: the timer is inventory and outlives the script, so gating a
					// jump on the timer left a proliferated monster permanently lit and taking nothing
					// the moment its script ended for any reason other than the timer running out.
					next_dmg = CheckActorInventory(tlist[pnum][i].tid, "DnD_IgniteScripts");

					// Spread THIS burn's magnitude, do not re-derive one. base_dmg already carries the added fire
					// from the hit that started the chain, which is a property of the build, not of the victim.
					if(!next_dmg) {
						SetActorInventory(tlist[pnum][i].tid, "DnD_IgniteTimer", ign_time);
						SetActorInventory(tlist[pnum][i].tid, "DnD_CurrentIgniteDamage", base_dmg);

						// claim before launching -- see the same call in HandleIgniteEffects
						GiveActorInventory(tlist[pnum][i].tid, "DnD_IgniteScripts", 1);

						// we don't proliferate from the proliferated targets... that'd be busted
						// note: WAIT AND SEE IF ITS OP!
						ACS_NamedExecuteWithResult("DnD Monster Ignite", tlist[pnum][i].tid, wepid, ign_flags, base_dmg);
					}
					else {
						// A live burn is already there, so extend and improve it rather than starting a
						// second one. next_dmg is the refcount now, not a duration, so the timer has to
						// be re-read for the Max.
						SetActorInventory(
							tlist[pnum][i].tid, "DnD_IgniteTimer",
							Max(ign_time, CheckActorInventory(tlist[pnum][i].tid, "DnD_IgniteTimer"))
						);

						if(base_dmg > CheckActorInventory(tlist[pnum][i].tid, "DnD_CurrentIgniteDamage"))
							SetActorInventory(tlist[pnum][i].tid, "DnD_CurrentIgniteDamage", base_dmg);
					}

					// abort if we reached our count
					++j;
					if(j == dmg_tic_buff)
						break;
				}
			}
		}
	}

	SetResultValue(0);
}

Script "DnD Monster Ignite FX" (int tid, int amt) CLIENTSIDE {
	SetActivator(tid);
	
	for(int i = 0; i < amt; ++i) {
		Delay(const:DND_IGNITE_TICKRATE);
		
		// if thing no longer exists, stop
		if(!ActivatorTID())
			Terminate;
		
		if(random(0, 1))
			GiveInventory("DnD_IgniteFXSpawner", 1);
	}
}

Script "DnD Monster Overload" (int victim) {
	int source = ActivatorTID();
	// we dont have any player involvement here so
	SetActivator(victim);
	
	PlaySound(victim, "Overload/Loop", CHAN_ITEM, 1.0, true);

	if(HasActorClassPerk_Fast(source, DND_PLAYER_WANDERER, 2))
		AddMonsterAilment(source, victim, DND_AILMENT_OVERLOAD);
	
	while(CheckInventory("DnD_OverloadTimer")) {
		if(!ActivatorTID())
			Terminate;
	
		ACS_NamedExecuteWithResult("DnD Monster Overload Particles", victim);
		TakeInventory("DnD_OverloadTimer", 1);
		Delay(const:DND_BASE_OVERLOADTICK);
		GiveInventory("Overload_SoundStopper", 1);
	}

	if(HasActorClassPerk_Fast(source, DND_PLAYER_WANDERER, 2))
		RemoveMonsterAilment(victim, DND_AILMENT_OVERLOAD);

	// remove accumulated damage
	SetInventory("DnD_OverloadDamage", 0);
	StopSound(victim, CHAN_ITEM);
	
	SetResultValue(0);
}

Script "DnD Monster Overload Particles" (int tid) CLIENTSIDE {
	int r = GetActorProperty(tid, APROP_RADIUS);
	int h = GetActorProperty(tid, APROP_HEIGHT);
	for(int i = 0; i < 3; ++i) {
		SpawnForced("OverloadZap_Particles", GetActorX(tid) + random(-r, r) / 2, GetActorY(tid) + random(-r, r) / 2, GetActorZ(tid) + (random(16.0, h + 32.0)) / 2, 0);
		Delay(random(1, 3));
	}

	SetResultValue(0);
}

// this simply distributes the overload debuff, no more zapping special fx!!
Script "DnD Monster Overload Zap" (int this, int killer) {
	if(!isPlayer(killer))
		Terminate;
	
	PlaySound(this, "Overload/ZapBegin", CHAN_ITEM, 1.0);
	//SpawnForced("OverloadZap_Source", GetActorX(this), GetActorY(this), GetActorZ(this) + GetActorProperty(this, APROP_HEIGHT) + 16.0, 0);
	
	// first look up potential targets and then store them, we'll zap later with some delay
	int pnum = killer - P_TIDSTART;
	
	int i;
	int zap_count = PlayerModData[pnum].vals[PSTAT_OVERLOAD_ZAPCOUNT] + 1;
	int cur_count = 0;
	static int zap_tids[MAXPLAYERS][DND_MAX_OVERLOADTARGETS];
	for(i = 0; i < zap_count; ++i)
		zap_tids[pnum][i] = 0;
	
	// we dont deal damage now just apply debuff!
	//int dmg = ACS_NamedExecuteWithResult("DND Player Damage Scale", CheckInventory("DnD_OverloadDamage") * (100 + CheckActorInventory(killer, "IATTR_OverloadZapDmg")) / 100, TALENT_ELEMENTAL, DND_WDMG_LIGHTNINGDAMAGE);
	for(int mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER] && zap_count; ++mn) {
		// if currently alive and received the checker item
		i = UsedMonsterTIDs[mn];
		if(CheckActorInventory(i, "DnD_OverloadZapCandidate") && isActorAlive(i) && CheckFlag(i, "ISMONSTER") && i != this)
			zap_tids[pnum][cur_count++] = i;
	}
	
	for(i = 0; i < cur_count; ++i) {
		// no more zap fx
		// ACS_NamedExecuteAlways("DnD Overload Zap FX", 0, this, zap_tids[pnum][i]);
		
		// do damage, give credit to killer
		SetActivator(killer);
		
		// no more damage
		// HandleDamageDeal(killer, zap_tids[pnum][i], dmg, DND_DAMAGETYPE_LIGHTNING, 0, GetActorX(this), GetActorY(this), GetActorZ(this), DND_ACTORFLAG_FOILINVUL | DND_ACTORFLAG_FORCEPAIN);
		
		// overload this monster if its still alive
		if(isActorAlive(zap_tids[pnum][i])) {
			PlaySound(zap_tids[pnum][i], "Overload/Zap", CHAN_ITEM, 1.0);

			if(!CheckActorInventory(zap_tids[pnum][i], "DnD_OverloadTimer")) {
				SetActorInventory(zap_tids[pnum][i], "DnD_OverloadTimer", GetOverloadTime(pnum));
				// overload damage amp is set to maximum of whatever the monster might have had (from another player) or this new instance of overload
				SetActorInventory(zap_tids[pnum][i], "DnD_OverloadDamage", Max((PlayerModData[pnum].vals[PSTAT_OVERLOAD_DMGINCREASE] * 100) >> 16, CheckActorInventory(zap_tids[pnum][i], "DnD_OverloadDamage")));
				ACS_NamedExecuteWithResult("DnD Monster Overload", zap_tids[pnum][i]);
			}
			else
				SetActorInventory(zap_tids[pnum][i], "DnD_OverloadTimer", GetOverloadTime(pnum));
			GiveActorInventory(zap_tids[pnum][i], "DnD_OverloadLockTime", 1);
		}
		SetActivator(this);
		
		Delay(const:DND_BASE_OVERLOADZAPDELAY);
	}
}

Script "DnD Spawn Bloodtype" (int type) CLIENTSIDE {
	SpawnForced(SpecialBloodFX[type], GetActorX(0), GetActorY(0), GetActorZ(0), SPECIAL_FX_TID);
	SetResultValue(0);
}

Script "DnD Check Explosion Repeat" (void) {
	// player information
	int owner = GetActorProperty(0, APROP_TARGETTID);
	if(!isPlayer(owner))
		owner = GetActorProperty(0, APROP_SCORE);

	int res = 0;
	int pnum = owner - P_TIDSTART;
	
	// if explosion did not repeat and we have chance for it to repeat, go for it
	int chanceSum = CheckInventory("DnD_ExplosionRepeatChance");
	if(!chanceSum) {
		// +1 so this isn't entered the 2nd time we check for repeats, it acts as sentinel value here
		chanceSum = GetExplosiveRepeatChance(pnum) + 1;
		if(CheckFlag(0, "RIPPER")) {
			//printbold(s:"is ripper explosive!");
			chanceSum += PlayerModData[pnum].vals[PSTAT_INC_RIPPERSEXPLODE];
		}
		SetInventory("DnD_ExplosionRepeatChance", chanceSum);
	}

	if(random(1, 100) <= chanceSum - 1) {
		// check rekindled sparks
		res = PlayerModData[pnum].vals[PSTAT_EX_SECONDEXPBONUS];
		if(res && !CheckInventory("DnD_ExplosiveRepeated")) {
			res += 100;

			GiveInventory("DnD_ExplosiveRepeated", 1);

			// we embed a damage factor into this, so we can scale that and reput it
			int temp = GetUserVariable(0, "user_expdmg");
			int factor = temp >> DPCT_SHIFT;
			temp &= BITMASK_NOFACTOR;

			// this undoes the first explosion reduction effect
			int x = ReadPlayerModExtra(pnum, INV_EX_SECONDEXPBONUS);
			factor = factor * 100 / (100 - x);
			factor = factor * res / 100;

			SetUserVariable(0, "user_expdmg", temp | (factor << DPCT_SHIFT));

			// Undo the first explosion's shrink and apply this one's bonus. AREA, matching
			// "DnD Explosion Radius Retrieve" -- a LINEAR undo does not cancel a square root shrink, it
			// overshoots. Both steps in one so the truncations do not compound.
			//
			// Damage above stays linear on purpose: it is not a geometric quantity. Sprite scale is
			// linear in radius, so it follows the radius ratio rather than the area percentage.
			int r_old = GetUserVariable(0, "user_expradius");
			if(r_old > 0) {
				int r_new = sqrt_z(r_old * r_old * res / (100 - x));

				SetUserVariable(0, "user_expradius", r_new);
				SetUserVariable(0, "user_fullexpradius", GetUserVariable(0, "user_fullexpradius") * r_new / r_old);
				SetActorProperty(0, APROP_SCALEX, GetActorProperty(0, APROP_SCALEX) * r_new / r_old);
				SetActorProperty(0, APROP_SCALEY, GetActorProperty(0, APROP_SCALEY) * r_new / r_old);
			}
		}

		res = 1;

		if(chanceSum - 100 <= 0)
			SetInventory("DnD_ExplosionRepeatChance", 1);
		else
			SetInventory("DnD_ExplosionRepeatChance", chanceSum - 100);
	}
	
	SetResultValue(res);
}

Script "DnD Check Explosion Ripper" (void) {
	int owner = GetActorProperty(0, APROP_TARGETTID);
	if(!isPlayer(owner))
		owner = GetActorProperty(0, APROP_SCORE);

	int pnum = owner - P_TIDSTART;
	bool res = PlayerModData[pnum].vals[PSTAT_INC_RIPPERSEXPLODE] >= random(1, 100);

	if(res)
		GiveInventory("DnD_RipperExploded", 1);

	SetResultValue(res);
}

int HandlePlayerSelfDamage(int pnum, int dmg, int dmg_type, int wepid, int flags, bool isArmorPiercing) {
	dmg = dmg * ((GetSelfExplosiveResist(pnum) * 100) >> 16) / 100;

	int tflag = PlayerModData[pnum].vals[PSTAT_IMP_LESSSELFDAMAGETAKEN];
	if(tflag)
		dmg = dmg * (100 - tflag) / 100;
	
	// apply accessory and other sources of damage -- convert to dmg tic flag due to the recent rewrite
	tflag = (!!(flags & DND_DAMAGEFLAG_ADDEDIGNITE) * DND_DAMAGETICFLAG_ADDEDIGNITE)			|
			(!!(flags & DND_DAMAGEFLAG_SCALEIGNITE) * DND_DAMAGETICFLAG_SCALEIGNITE)			|
			(!!(flags & DND_DAMAGEFLAG_EXTRATOUNDEAD) * DND_DAMAGETICFLAG_EXTRATOUNDEAD)		|
			(!!(flags & DND_DAMAGEFLAG_NOPOISONSTACK) * DND_DAMAGETICFLAG_NOPOISONSTACK)		|
			(!!(flags & DND_DAMAGEFLAG_NOIGNITESTACK) * DND_DAMAGETICFLAG_NOIGNITESTACK)		|
			(!!(flags & DND_DAMAGEFLAG_SOULATTACK) * DND_DAMAGETICFLAG_SOULATTACK)				|
			(!!(flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME) * DND_DAMAGETICFLAG_DOT);

	int amp = HandlePlayerBuffs(pnum + P_TIDSTART, pnum + P_TIDSTART, dmg_type, wepid, tflag);
	// amp already carries the 100 baseline -- "100 + amp" doubled it, and the
	// amp != 100 guard is exactly what hid the silent x2 on the no-accessory case
	if(amp != 100)
		dmg = MulPercent_Exact(dmg, amp);

	// factor in players armor here!!! -- NO DON'T DO THAT! We have a generic resist and armor handle in main dmg script
	//dmg = HandlePlayerArmor(pnum, dmg, "null", DND_DAMAGETYPEFLAG_EXPLOSIVE, isArmorPiercing);
	return dmg;
}

// dmg data encapsulates the information about what damage types this attack involved
// uses DND_DAMAGETYPEFLAG enums
// The element a touch trait grants, as a DND_TICELEM_* index rather than a flag, so gains from
// different sources that name the same element land on the same accumulator slot.
int GetTouchTraitElement(int trait) {
	switch(trait) {
		case DND_EMBERTOUCH:
		return DND_TICELEM_FIRE;
		case DND_RIMETOUCH:
		return DND_TICELEM_ICE;
		case DND_STORMTOUCH:
		return DND_TICELEM_LIGHTNING;
		case DND_VILETOUCH:
		return DND_TICELEM_POISON;
	}
	return DND_TICELEM_PHYSICAL;
}

// Percent of the attack's own damage a touch trait gains as its element. This is the single seam a
// dungeon modifier would read through to push the gain above the flat elite value -- add the
// HasDungeonAttributeVal lookup here and every touch trait picks it up at once. Nothing supplies one
// yet, so today it is the elite constant alone.
int GetMonsterTouchGainPercent(int m_id) {
	return DND_MONSTER_TOUCHGAIN_PCT;
}

// What the touch traits contributed to the hit a player just took, kept for the stages that run
// after HandlePlayerResists returns. HandlePlayerChill is the one that needs it: it fires later, on
// the post armor number, and its caller only ever had the attack's original type flags.
//
// Written on every hit that reaches HandlePlayerResists and read in the same damage event with no
// delay in between, so neither value can go stale between the write and the read.
int PlayerHitGainedFlags[MAXPLAYERS];
int PlayerHitIceShare[MAXPLAYERS];

int HandlePlayerResists(int pnum, int dmg, str dmg_string, int dmg_data, bool isReflected, str inflictor_class, int m_id = -1) {
	int temp;

	bool isDot = IsDamageStringDOT(dmg_string) || (dmg_data & DND_DAMAGETYPEFLAG_DOT);

	int add = pbuffs[pnum].buff_net_values[BUFF_DAMAGETAKEN].additive;
	int mult = 1.0;
	mult = FixedMul(mult, pbuffs[pnum].buff_net_values[BUFF_DAMAGETAKEN].multiplicative);
	mult = FixedMul(mult, pbuffs[pnum].buff_net_values[BUFF_ENDURANCECHARGE].multiplicative);

	int res_to_apply = DND_PRESIST_NONE;
	int res_bonus = 0;
	
	// reflection becomes its own thing not affected by other damage type functions, so we can immediately return here
	if(isReflected) {
		// 90% reduction
		if(HasPlayerFlag(pnum, PFLAG_LOWERREFLECT))
			mult = CombineFactors(mult, -0.9);
		SetActivator(0, AAPTR_DAMAGE_INFLICTOR);
		dmg_data = GetActorProperty(0, APROP_STOREDREFLECTDAMAGETYPE);
		SetActivator(0, AAPTR_DAMAGE_TARGET);
	}

	// Type agnostic factors first. These apply to the whole hit no matter what it is made of, so
	// they are gathered once here and every portion below inherits them.
	//
	// explosion sources
	if((dmg_data & DND_DAMAGETYPEFLAG_EXPLOSIVE) && HasClassPerk_Fast(DND_PLAYER_MARINE, 3))
		mult = CombineFactors(mult, DND_MARINE_EXPLOSIVEREDUCTION);

	// marine perk 50's 50% reduction is BTI_MARINE_DAMAGEREDUCTION now, so it is already
	// folded into BUFF_DAMAGETAKEN's multiplicative above

	// overheat unique charm
	temp = PlayerModData[pnum].vals[PSTAT_EX_LESSDMGTAKENMAXOVERHEAT];
	if(temp && HasRunningOverheatCooldown(pnum + P_TIDSTART))
		mult = CombineFactors(mult, -((temp << 16)) / 100);

	temp = PlayerModData[pnum].vals[PSTAT_EX_DMGINCREASE_TAKEN];
	if(temp)
		mult = CombineFactors(mult, (temp << 16) / 100);

	if(m_id != -1 && HasMonsterTrait(m_id, DND_PENETRATOR))
		res_bonus += DND_PENETRATOR_PIERCE;

	// A hit is normally one damage type the whole way down, and for that case there is exactly one
	// portion here and everything below reduces to what it has always been. A monster with a touch
	// trait adds one portion per trait: still a single damage event, but each part meets its own
	// resist and its own typed reduction implicit, and inflicts its own ailment at its own size.
	//
	// This is deliberately NOT the dealing side's approach of emitting a second damage instance.
	// Everything after this function -- armor, the damage floor, the eshield deduction, the on hit
	// buffs, the hit cooldown, the push -- is once per hit machinery, and a second Thing_Damage2 on
	// the player would re-enter the damage event and pay every bit of it twice.
	int p_flags[DND_MAX_TOUCHGAINS + 1];
	int p_amt[DND_MAX_TOUCHGAINS + 1];
	int p_count = 1;

	// Player-side DoTs are delivered by DealDOTDamage through Thing_Damage2, which carries no
	// inflictor -- so dmg_data was never theirs and whatever it holds is stale or empty. The damage
	// type STRING is the only reliable signal for these four, and it is authoritative: the type bits
	// are cleared and rewritten rather than OR-ed, so a leftover bit from an unrelated actor cannot
	// steer a burn into the wrong branch below.
	//
	// PoisonDOT was special cased further down for exactly this reason and had been since forever.
	// The other three were simply never added, so ignite, bleed and physical damage over time on a
	// PLAYER matched no branch at all and resolved as untyped -- which, before DND_PRESIST_NONE, is
	// the path that read INV_HP_INCREASE as a resistance.
	if(IsDamageStringDOT(dmg_string)) {
		dmg_data &= ~DND_DAMAGETYPEFLAG_ALLTYPES;

		if(dmg_string == "PoisonDOT")
			dmg_data |= DND_DAMAGETYPEFLAG_POISON | DND_DAMAGETYPEFLAG_DOT;
		else if(dmg_string == "FireDOT")
			dmg_data |= DND_DAMAGETYPEFLAG_FIRE | DND_DAMAGETYPEFLAG_DOT;
		else if(dmg_string == "BleedDOT")
			dmg_data |= DND_DAMAGETYPEFLAG_PHYSICAL | DND_DAMAGETYPEFLAG_DOT | DND_DAMAGETYPEFLAG_ISBLEED;
		else // PhysicalDOT
			dmg_data |= DND_DAMAGETYPEFLAG_PHYSICAL | DND_DAMAGETYPEFLAG_DOT;
	}

	p_flags[0] = dmg_data;
	p_amt[0] = dmg;

	// Gain, not conversion: the source keeps its full damage and this rides on top. Attacks only,
	// never DoT ticks -- the hit that applied the DoT already gained, and gaining again on every
	// tick would compound it. Reflected damage is the player's own coming back, so a monster's
	// traits have no claim on that either.
	if(m_id != -1 && !isDot && !isReflected && MonsterHasAnyTouchTrait(m_id)) {
		// Percent is accumulated per DESTINATION element, then turned into portions -- not one
		// portion per source. Two sources naming the same element (a rolled RIMETOUCH at 20% and a
		// dungeon granting 10% cold) become a single 30% cold portion, so cold meets the player's
		// cold resist once and truncates once. Two separate cold portions would pay both twice,
		// which is the same "one bucket per destination" rule the dealing side's conversion follows.
		//
		// It is also what makes the portion arrays safe: the bound is the number of destinations,
		// so no number of gain sources can overflow them.
		int gain_pct[DND_MAX_TICELEMS];
		int e;
		for(e = 0; e < DND_MAX_TICELEMS; ++e)
			gain_pct[e] = 0;

		// the touch traits are one contiguous run in the trait enum, see DnD_EliteInfo.h
		for(int t = DND_FIRST_TOUCHTRAIT; t <= DND_LAST_TOUCHTRAIT; ++t)
			if(HasMonsterTrait(m_id, t))
				gain_pct[GetTouchTraitElement(t)] += GetMonsterTouchGainPercent(m_id);

		// A dungeon granted gain joins here, on the same array, and stacks with a rolled trait of
		// the same element for free. A source-typed one ("physical gained as cold") gates itself on
		// the hit's own type first, e.g.
		//     if(dmg_data & DND_DAMAGETYPEFLAG_PHYSICAL)
		//         gain_pct[dest] += pct;
		// NOTE: MonsterHasAnyTouchTrait above short-circuits on the monster's TRAIT bits, so a
		// dungeon gain that applies without a trait needs that gate widened or it never runs.

		for(e = 0; e < DND_MAX_TICELEMS; ++e) {
			if(!gain_pct[e])
				continue;

			int elem = MapTicElementToDamageTypeFlag(e);

			// nothing to gain when the attack already is that element
			if(dmg_data & elem)
				continue;

			p_flags[p_count] = elem;
			p_amt[p_count] = MulPercent_Exact(dmg, gain_pct[e]);
			++p_count;
		}
	}

	// chill is the only ailment that needs its element's own subtotal -- see the block below
	int sub_ice = 0;
	int gained_flags = 0;
	int total = 0;

	for(int p = 0; p < p_count; ++p) {
		int p_type = p_flags[p];
		int amt = p_amt[p];		// read once; everything below works on the scalar
		int p_add = add;
		int p_mult = mult;
		res_to_apply = DND_PRESIST_NONE;

		if(p_type & DND_DAMAGETYPEFLAG_PHYSICAL)
			res_to_apply = DND_PRESIST_PHYS;
		else if(p_type & DND_DAMAGETYPEFLAG_MAGICAL)
			res_to_apply = DND_PRESIST_MAGIC;
		else if(p_type & DND_DAMAGETYPEFLAG_FIRE) {
			res_to_apply = DND_PRESIST_ELEM;

			temp = PlayerModData[pnum].vals[PSTAT_IMP_LESSFIRETAKEN];
			if(temp)
				p_mult = CombineFactors(p_mult, -temp);
		}
		else if(p_type & DND_DAMAGETYPEFLAG_ICE) {
			res_to_apply = DND_PRESIST_ELEM;
		}
		else if(p_type & DND_DAMAGETYPEFLAG_LIGHTNING) {
			res_to_apply = DND_PRESIST_ELEM;

			temp = PlayerModData[pnum].vals[PSTAT_IMP_LESSLIGHTNINGTAKEN];
			if(temp)
				p_mult = CombineFactors(p_mult, -temp);
		}
		else if(p_type & DND_DAMAGETYPEFLAG_POISON) {
			// The `|| dmg_string == "PoisonDOT"` that used to be on this line is gone: the block at the
			// top of the function now stamps the POISON bit on for it, along with the three DoT kinds
			// that never had a test here at all.
			temp = PlayerModData[pnum].vals[PSTAT_IMP_LESSPOISONTAKEN];
			if(temp)
				p_mult = CombineFactors(p_mult, -temp);

			// reduced poison damage taken
			res_to_apply = DND_PRESIST_ELEM;

			// toxicology ability
			if(CheckInventory("Ability_AntiPoison")) {
				if(!HasClassPerk_Fast(DND_PLAYER_CYBORG, 1))
					p_add -= DND_TOXICOLOGY_REDUCE;
				else
					p_add -= CombineFactors(DND_TOXICOLOGY_REDUCE, DND_CYBORG_CYBERF);
			}
		}
		else if(p_type & DND_DAMAGETYPEFLAG_ENERGY)
			res_to_apply = DND_PRESIST_ENERGY;
		// ELEMENTAL DAMAGE BLOCK ENDS

		// Apply additive and multiplicative effects together now -- minimum 10% damage taken.
		//
		// Everything stays in 16.16 until the single multiply at the end.
		//
		// Do NOT convert to integer percent first the way the damage-DEALT path does. There
		// the factor is (100 + increased), which is large, so a 1% quantum is noise. Here a
		// reduction makes the factor SMALL, and collapsing a 37% x 17% chain through integer
		// percent gives 37 * 16 / 100 = 5% -- a fifth of the value thrown away. Keeping 16.16
		// resolution costs nothing and cuts the mean error about sevenfold.
		//
		// (add * 100) >> 16 is avoided for a second reason: it wraps once add passes 327.68
		// in fixed, and add is a sum of buff values. That wrap could turn an 884 damage hit
		// into 2.9 million.
		if(p_add < -0.9)
			p_add = -0.9;

		int combined = 1.0 + p_add;
		if(p_mult != 1.0)
			combined = FixedMul(combined, p_mult);

		// exact, saturating, and yields 0 for a total-negation factor
		amt = MulPercent_Exact(amt, combined, 1.0);

		// finally include resists as their own multiplicative factor
		amt = ApplyPlayerDamageResist(pnum, amt, res_to_apply, res_bonus);

		if(isReflected)
			amt = ApplyPlayerDamageResist(pnum, amt, DND_PRESIST_REFL, 0);

		if(p_type & DND_DAMAGETYPEFLAG_ICE)
			sub_ice += amt;

		total += amt;

		// a gained portion genuinely IS its element, so the ailment tests below have to see it
		if(p)
			gained_flags |= p_type;
	}

	dmg = total;
	dmg_data |= gained_flags;

	// Ailment MAGNITUDES deliberately use the whole hit, not the element's share of it. The
	// constants they scale by -- DND_MONSTER_BURN_PERCENT, DND_MONSTER_POISONPERCENT -- were tuned
	// as "percent of the hit", and DND_SCORCHED already grants a full strength burn on a monster
	// that deals no fire at all. Sizing a touch trait's burn by its 20% portion made the trait that
	// DOES deal fire burn ten times weaker than the one that does not, which is backwards; at these
	// numbers it floored to 1 damage a tick and read as the ailment being broken outright.
	//
	// Chill is the exception and keeps its share. Its test is a threshold against a fraction of the
	// player's health pool rather than a damage amount, so a 20% ice portion genuinely should not
	// freeze like a full ice hit would. HandlePlayerChill runs later still, after armor and the
	// damage floor, on a number this function never sees -- so the share travels as a percentage
	// rather than an amount, the same reason the dealing side stages its components as ratios.
	//
	// 100 is the default for the single type hit and for a FRIGID monster whose attack carries no
	// ice at all: both want the whole number, which is what they have always used.
	int share_ice = 100;
	if(p_count > 1 && total > 0 && sub_ice)
		share_ice = sub_ice * 100 / total;

	PlayerHitGainedFlags[pnum] = gained_flags;
	PlayerHitIceShare[pnum] = share_ice;

	//printbold(s:"res applied dmg: ", d:dmg);

	// find player's lowest resist
	temp = PlayerModData[pnum].vals[PSTAT_EX_DAMAGELOWESTTAKENASPHYS];
	if(temp && !isDot && (GetLowestResist(pnum) & dmg_data)) {
		// create new dot instance of phys damage to player making sure to only get a portion of it as DoT
		temp = dmg * temp / 100;
		if(!temp)
		temp = 1;

		// this is the "instead" part of the "DoT", the rest
		dmg -= temp;
		if(dmg < 1)
			dmg = 1;

		if(m_id != -1)
			RegisterDoTDamage(temp, 5, DND_DAMAGETYPEFLAG_PHYSICAL, m_id + DND_MONSTERTID_BEGIN, inflictor_class);
	}
	
	// gravecaller unique mod -- multiplicative so leave it last
	if(CheckUniquePropertyOnPlayer(pnum, PUP_PAINSHAREDWITHPETS)) {
		// damage is shared between you and pets, therefore if you have 1 pet you take half
		// you have 2 you get 1/3rd, which is what this'll do
		temp = CheckInventory("PetCounter") + 1;
		if(temp > DND_MAX_PET_DAMAGESHARE)
			temp = DND_MAX_PET_DAMAGESHARE;
		dmg /= temp;
		
		// distribute this damage to other pets
	}

	// final thing to check after damage reductions are applied, DoTs
	// do not register more instances on dots from dots themselves as well
	if(m_id != -1 && !isDot && dmg) {
		if
		(
			(dmg_data & DND_DAMAGETYPEFLAG_PHYSICAL) && !(dmg_data & DND_DAMAGETYPEFLAG_EXPLOSIVE) && 
			random(1, 100) <= GetMonsterBleedChance(m_id, pnum, dmg_string == "Melee", dmg_data & DND_DAMAGETYPEFLAG_HITSCAN) &&
			GetPlayerBleedAvoidChance(pnum) < random(1, 100)
		)
		{
			HandleRiskAversion();

			temp = GetMonsterBleedDamage(dmg, m_id, pnum);
			if(!temp)
				temp = 1;

			RegisterDoTDamage(
				temp,
				GetMonsterBleedDuration(m_id, pnum),
				DND_DAMAGETYPEFLAG_PHYSICAL | DND_DAMAGETYPEFLAG_DOT | DND_DAMAGETYPEFLAG_ISBLEED, 
				m_id + DND_MONSTERTID_BEGIN,
				inflictor_class
			);
		}
		
		if
		(
			((dmg_data & DND_DAMAGETYPEFLAG_LIGHTNING) || HasMonsterTrait(m_id, DND_VOLTAIC)) && 
			random(1, 100) <= GetMonsterOverloadChance(m_id, pnum) &&
			GetPlayerElementalAvoidChance(pnum, DND_PAVOID_OVERLOAD) < random(1, 100)
		)
		{
			HandleRiskAversion();
			SetInventory("DnD_OverloadTimer", GetMonsterOverloadTime(m_id, pnum));
			ACS_NamedExecuteWithResult("DnD Give Buff", DND_DEBUFF_OVERLOAD, DEBUFF_F_PLAYERISACTIVATOR | DEBUFF_F_OWNERISTARGET);
		}
		
		if
		(
			((dmg_data & DND_DAMAGETYPEFLAG_POISON) || HasMonsterTrait(m_id, DND_VENOMANCER)) &&
			GetPlayerElementalAvoidChance(pnum, DND_PAVOID_POISON) < random(1, 100)
		)
		{
			HandleRiskAversion();
			temp = MulPercent_Exact(dmg, DND_MONSTER_POISONPERCENT);
			if(!temp)
				temp = 1;

			RegisterDoTDamage(
				temp, 
				random(DND_MONSTER_POISONDOT_MINTIME, DND_MONSTER_POISONDOT_MAXTIME),
				DND_DAMAGETYPEFLAG_POISON, 
				m_id + DND_MONSTERTID_BEGIN,
				inflictor_class
			);
		}
		
		if
		(
			((dmg_data & DND_DAMAGETYPEFLAG_FIRE) || HasMonsterTrait(m_id, DND_SCORCHED)) && 
			random(0, 1.0) < DND_PLAYER_BURNING_CHANCE &&
			GetPlayerElementalAvoidChance(pnum, DND_PAVOID_IGNITE) < random(1, 100)
		) 
		{
			HandleRiskAversion();
			temp = MulPercent_Exact(dmg, DND_MONSTER_BURN_PERCENT);
			if(!temp)
				temp = 1;
			RegisterDoTDamage(
				temp, 
				random(DND_PLAYER_BURNING_MINTIME, DND_PLAYER_BURNING_MAXTIME),
				DND_DAMAGETYPEFLAG_FIRE, 
				m_id + DND_MONSTERTID_BEGIN,
				inflictor_class
			);
		}
	}
	
	return dmg;
}

int GetArmorRatingEffect(int pnum, int dmg, int armor_id, int dmg_data, bool isArmorPiercing) {
	int rating = GetPlayerArmor(pnum);

	int temp = PlayerModData[pnum].vals[PSTAT_ARMOR_DOUBLEDEF];
	if(temp && random(1, 100) <= temp)
		rating <<= 1;

	// DONT USE armor_id below here!!
	if(dmg_data & DND_DAMAGETYPEFLAG_MAGICAL) {
		// armor_id stores negation factor
		armor_id = PlayerModData[pnum].vals[PSTAT_MAGIC_NEGATION];
		if(armor_id > 100)
			armor_id = 100;
		armor_id = BASE_ARMOR_MAGIC_EFFECT * (100 - armor_id) / 100;
		rating = rating * (100 - armor_id) / 100;
	}

	// rating is treated as 40% instead of 100% if monster is armor piercing
	if(isArmorPiercing)
		rating = rating * 2 / 5;

	armor_id = pbuffs[pnum].buff_net_values[BUFF_ARMORINCREASE].additive;
	if(armor_id) {
		armor_id = (armor_id * 100) >> 16;
		if(armor_id < -100)
			armor_id = -100;
		rating = rating * (100 + armor_id) / 100;
	}

	armor_id = pbuffs[pnum].buff_net_values[BUFF_ARMORINCREASE].multiplicative;
	if(armor_id != 1.0) {
		armor_id = (armor_id * 100) >> 16;
		rating = rating * armor_id / 100;
	}

	if(CheckInventory("RuinationHardDebuff"))
		rating /= 4;
	else {
		pnum = CheckInventory("RuinationStacks");
		if(pnum)
			rating -= rating * pnum * DND_RUINATION_REDUCE_PER_STACK / 100;
	}

	return DoArmorRatingEffect(dmg, rating);
}

bool IsDamageStringDOT(str s) {
	return s == "PoisonDOT" || s == "FireDOT" || s == "BleedDOT" || s == "PhysicalDOT";
}

int HandlePlayerArmor(int pnum, int dmg, str dmg_string, int dmg_data, bool isArmorPiercing) {
	int armor_id = GetArmorID();
	bool is_dot = IsDamageStringDOT(dmg_string);
	int factor = 0;

	// DoT is not negated by armor
	if(armor_id != -1 && !is_dot) {
		// retrieve and convert factor to an integer, we convert ex: 0.417 to 417, we will apply damage factor safe method
		// dmg here is the one to be dealt to the player's health pool

		// apply armor effect on this damage
		dmg = GetArmorRatingEffect(pnum, dmg, armor_id, dmg_data, isArmorPiercing);
		
		// special armor cases: Knight gives more reduction if using melee weapon, Duelist negates all hitscan 100% at cost of armor
		if(armor_id == BODYARMOR_KNIGHT && IsUsingMeleeWeapon())
			factor += ReadPlayerModExtra(pnum, INV_IMP_KNIGHTARMOR);

		if(HasClassPerk_Fast(DND_PLAYER_BERSERKER, 1))
			factor += DND_BERSERKER_MELEEWEPRESIST;
		
		// armor reduced factor amount of damage, this is what the player will take as damage
		dmg = MulPercent_Exact(dmg, 100 - factor, 100);
		
		// if we have ravager armor and on killing spree, reduce damage to 17/20 (15% reduced)
		if(armor_id == BODYARMOR_RAVAGER && HasPlayerBuff(pnum, BTI_RAVAGER_POWER))
			dmg = MulPercent_Exact(dmg, DND_RAVAGER_FACTOR, DND_RAVAGER_REDUCE);
		else if(armor_id == BODYARMOR_KNIGHT && dmg_string == "Melee") // apply special reductions offered by certain armors
			dmg = MulPercent_Exact(dmg, 100 - PlayerModData[pnum].vals[PSTAT_IMP_KNIGHTARMOR]);
	}

	// mitigation -- poison goes through as well
	int temp;
	if(!is_dot && CouldMitigateDamage(pnum)) {
		temp = GetMitigationEffect(pnum);
		dmg = dmg * ((100.0 - temp) >> 16) / 100;

		// only cooldown for sound because apparently localambientsound does not respect sound limits
		if(!CheckInventory("DnD_MitigationCooldown")) {
			LocalAmbientSound("Mitigation/Success", 80);
			GiveInventory("DnD_MitigationCooldown", 1);
		}
	}
	
	return dmg;
}

// energy shield reduction and other true flat damage lowering things, these are the final defense
int ApplyTrueDamageDeductions(int pnum, int dmg, str dmg_string, int dmg_data) {
	int temp = CheckInventory("EShieldAmount");
	int factor = GetEShieldMagicAbsorbValue(pnum);
	bool is_dot = IsDamageStringDOT(dmg_string);
	int armor_id = GetArmorID();
	int to_take = 0;

	if(temp) {
		// this isn't DOT or magical attack and we have energy shield, so we can deduct damage from it
		if(!is_dot && !(dmg_data & DND_DAMAGETYPEFLAG_MAGICAL))
			factor = 100;

		if(factor) {
			// lightning coil absorbs 80% by itself, so 20% of the damage will go through
			if((dmg_data & DND_DAMAGETYPEFLAG_LIGHTNING) && armor_id == BODYARMOR_LIGHTNINGCOIL)
				factor += LIGHTNINGCOIL_ABSORBFACTOR;

			// force clamp
			if(factor > 100)
				factor = 100;

			// only block this much if this is on
			to_take = PlayerModData[pnum].vals[PSTAT_EX_ESHIELDONLYBLOCKPCT];
			if(to_take)
				factor = to_take;

			// only this much is prevented
			to_take = Min(dmg * factor / 100, temp);
			if(to_take < 1)
				to_take = 1;
			dmg -= to_take;

			// completely absorbed by our shield, so just reduce our shield amount
			if(dmg < 0) {
				TakeEnergyShield(to_take + dmg);
				dmg = 0;
				if(!CheckInventory("DnD_EshieldHitSoundCooldown")) {
					GiveInventory("DnD_EshieldHitSoundCooldown", 1);
					LocalAmbientSound("EShield/Hit", 100);
				}
			}
			else if(to_take < temp) {
				TakeEnergyShield(to_take);
				if(!CheckInventory("DnD_EshieldHitSoundCooldown")) {
					GiveInventory("DnD_EshieldHitSoundCooldown", 1);
					LocalAmbientSound("EShield/Hit", 100);
				}
			}
			else {
				SetEnergyShield(0);
				LocalAmbientSound("EShield/Break", 127);

				temp = PlayerModData[pnum].vals[PSTAT_EX_STARTESONDEPLETE];
				if(temp && random(1, 100) <= temp && (to_take = CanRegenEShield(pnum))) {
					GiveInventory("EShieldChargeNow", 1);
					ACS_NamedExecuteAlways("DnD Energy Shield Regen", 0, to_take, pnum);
				}
			}
		}
	}

	// check overleech
	if(HasClassPerk_Fast(DND_PLAYER_PUNISHER, 4)) {
		// check if we do have overleech
		temp = CheckInventory("LifeStealAmount") - GetLifestealCap(pnum);
		if(temp > 0) {
			// we got overleech, now we can consider taking damage off
			to_take = dmg * DND_PUNISHER_OVERLEECH_REDUCEFACTOR / 100;
			if(to_take > temp)
				to_take = temp;

			if(to_take > 0) {
				dmg -= to_take;

				// shouldn't need to check if dmg < 0, because to take is at most 33% anyway of the damage, and if that itself is more than we can afford, we clamp
				// should def be lower
				
				TakeInventory("LifeStealAmount", to_take);
			}
		}
	}

	return dmg;
}

void HandleMonsterDamageModChecks(int m_id, int monster_tid, int victim, int dmg, bool isDot) {
	// vampirism check
	int hp;
	if(HasMonsterTrait(m_id, DND_VAMPIRISM) && isActorAlive(monster_tid)) {
		// if this monster is trying to leech off of a bloodless monster, do not allow (we cant have all rules be against players... right?)
		if(IsMonster(victim) && HasMonsterTrait(victim - DND_MONSTERTID_BEGIN, DND_BLOODLESS))
			return;
		
		// ignite effects prevent vampirism healing
		if(CanHealMonsterTID(monster_tid)) {
			// 10% or 10 flat healing per hit, minimum
			hp = Max(dmg / 10, 10);

			HealMonsterTID(monster_tid, m_id, hp);
			ACS_NamedExecuteAlways("DnD Vampirism FX CS", 0, monster_tid);
		}
	}

	if(!isPlayer(victim) || !isActorAlive(victim))
		return;

	if(HasMonsterTrait(m_id, DND_EXHAUSTING) && !isDot) {
		HandleStaminaBarDraw(victim - P_TIDSTART);
		TakeStamina(DND_EXHAUSTING_STAMINATAKE);
	}

	if(HasMonsterTrait(m_id, DND_BLACKOUT))
		ACS_NamedExecuteAlways("DnD Blackout", 0, victim);
}

int HandlePetMonsterDamageScale(int this, int master, int victim, int dmg, int dmg_data, int flags) {
	// set pointer to owner
	SetActivator(master);

	int pnum = master - P_TIDSTART, temp;
	bool wantDmgNums = flags == -1;
	int dmgnum_flags = 0;

	// extract damage category from dmg_data stamina
	int dmg_category = MonsterDamageTypeToDamageCategory(dmg_data);
	
	// revived monsters have half stat gain -- flags for damage category things arent used for pets YET
	dmg += MapDamageCategoryToFlatBonus(pnum, dmg_category, 0);
	dmg = dmg * (100 + HandleStatBonus(pnum, 0, 0, DND_STAT_ATTUNEMENT_GAIN / 2, true)) / 100;

	if((dmg_category == DND_DAMAGECATEGORY_BULLET || dmg_category == DND_DAMAGECATEGORY_MELEE) && (temp = PlayerModData[pnum].vals[PSTAT_EX_PHYSDAMAGEPER_FLATHEALTH])) {
		temp = GetFlatHealthDamageFactor(temp);
		dmg = dmg * (100 + temp) / 100;
	}

	temp = MapDamageCategoryToPercentBonus(pnum, dmg_category, 0);
	if(temp)
		dmg = dmg * (100 + temp) / 100;
	
	// finally crit chance -- move to damagedeal for pets in damage event later
	if(CheckCritChance(pnum, -1, -1, false, -1)) {
		dmg = dmg * GetCritModifier(pnum, -1, -1) / 100;
		HandleHunterTalisman();
		dmgnum_flags |= DND_DAMAGETICFLAG_CRIT;
	}

	if(flags == -1)
		ACS_NamedExecuteWithResult("DnD Damage Numbers", victim, dmg, dmgnum_flags);

	SetActivator(this);

	return dmg;
}

int HandlePercentDamageFromEnemy(int victim, int dmg, int dmg_data) {
	// check inflictor momentarily
	if((!(dmg_data & DND_DAMAGETYPEFLAG_PERCENTHP) && !(dmg_data & DND_DAMAGETYPEFLAG_PERCENTHP_LOW)) || CheckActorInventory(victim, "DnD_PercentDamageHalt"))
		return 0;
	
	int pct = DND_MONSTER_PERCENTDAMAGEBASE_LOW;
	// big pct damage has a halt check here
	if(dmg_data & DND_DAMAGETYPEFLAG_PERCENTHP) {
		GiveActorInventory(victim, "DnD_PercentDamageHalt", 1);
		pct = DND_MONSTER_PERCENTDAMAGEBASE;
	}

	int res = (GetActorProperty(victim, APROP_HEALTH) * pct) / 100;
	if(res < 0)
		res = 1;

	return res;
}

void OnPlayerHit(int this, int pnum, int target, bool isMonster, bool isDot = false) {
	int m_id, val;

	if(CheckActorInventory(this, "HateCheck") && target != this && isMonster)
		GiveActorInventory(target, "HateWeakness", 1);
	
	// necro and lightning coil chance
	int temp = PlayerModData[pnum].vals[PSTAT_IMP_NECROARMOR];
	if(temp && !CheckActorInventory(this, "NecroSpikeCooldown") && random(1, 100) <= temp) {
		GiveActorInventory(this, "NecroSpikeShooter", 1);
		GiveActorInventory(this, "NecroSpikeCooldown", 1);
	}
	else if((temp = ReadPlayerModExtra(pnum, INV_IMP_ABSORBLIGHTNING)) && !CheckActorInventory(this, "LightningCoilCooldown") && random(1, 100) <= temp) {
		// 25% chance
		GiveActorInventory(this, "LightningCoilShooter", 1);
		GiveActorInventory(this, "LightningCoilCooldown", 1);
	}

	// check unstable power core
	temp = ReadPlayerModExtra(pnum, INV_IMP_UNSTABLECORE);
	if(temp && (m_id = CheckInventory("EShieldAmount")) && RunLuckBasedChance(pnum, temp)) {
		// explode for this amount now
		SpawnForced("UnstableExplosion", GetActorX(0), GetActorY(0), GetActorZ(0) + GetActorViewHeight(this) / 2, DND_UNSTABLEEXP_TID);
		SetActivator(DND_UNSTABLEEXP_TID);
		SetActorProperty(0, APROP_MASS, 256);
		//  damage scales from 25% to 50% depending on player's eshield %
		// if has % hp contribution mod, include it to base dmg too!
		val = PlayerModData[pnum].vals[PSTAT_EX_ESEXPLOSIONHPDMG];
		if(val) {
			m_id += GetActorProperty(this, APROP_HEALTH) * val / 100;
			SetActorProperty(0, APROP_HEALTH, 2);
		}

		// quantizing to whole percent first made a low roll deal ZERO: at temp 0.002
		// (5*temp*100)>>16 is 0, and even temp 0.01 delivered 4% instead of 5%
		m_id = FixedMul(m_id, UNSTABLE_DMG_MULT * temp);
		m_id &= NONWEP_DMG_MASK; // limit to 65536
		// encode damage type
		m_id |= DND_DAMAGETYPE_ENERGY << NONWEP_DMG_SHIFT;

		SetUserVariable(0, "user_expdmg", m_id);
		SetActorProperty(0, APROP_TARGETTID, this);
		SetPointer(AAPTR_TARGET, this);
		SetActivator(this);

		// take away all ES
		SetEnergyShield(0);

		// depletion check
		temp = PlayerModData[pnum].vals[PSTAT_EX_STARTESONDEPLETE];
		if(temp && random(1, 100) <= temp && (m_id = CanRegenEShield(pnum))) {
			GiveInventory("EShieldChargeNow", 1);
			ACS_NamedExecuteAlways("DnD Energy Shield Regen", 0, m_id, pnum);
		}
	}
	
	// player heal on hit check -- target is 0 if we are the target, but the extra check in there is for safety
	// extra is chance to proc, value is the heal %
	temp = ReadPlayerModExtra(pnum, INV_EX_CHANCE_HEALMISSINGONPAIN);
	if(temp && target && target != this && random(1, 100) <= temp && !CheckActorInventory(temp, "DnD_HealOnMissingCD")) {
		// heal for missing health
		GiveActorInventory(this, "VeilHealFXSpawner", 1); // use same fx as veil for now
		GiveActorInventory(this, "DnD_HealOnMissingCD", 1);
		SetActivator(this);
		HandleHealthPickup((PlayerModData[pnum].vals[PSTAT_EX_CHANCE_HEALMISSINGONPAIN] * GetMissingHealth()) / 100, 0, true, true);
		// restore ptr
		SetActivator(0, AAPTR_DAMAGE_TARGET);
	}
	
	// check perk25 for berserker with cooldown
	if(HasActorClassPerk_Fast(this, DND_PLAYER_BERSERKER, 2) && !CheckActorInventory(this, "Berserker_Perk20_CD")) {
		// basically make sure only one instance of this runs

		if(!CheckActorInventory(this, "Berserker_Perk80_Extension")) {
			if(!CheckActorInventory(this, "Berserker_DamageTimer"))
				ACS_NamedExecuteAlways("DnD Berserker Perk20", 0, this);
				
			SetActorInventory(this, "Berserker_DamageTimer", DND_BERSERKER_DAMAGETRACKTIME);
			GiveActorInventory(this, "Berserker_Perk20_CD", 1);
		}

		if(CheckActorInventory(this, "Berserker_DamageTracker") < DND_BERSERKER_PERK20_MAXSTACKS)
			GiveActorInventory(this, "Berserker_DamageTracker", 1);
	}
	
	// rest of the code doesn't work if we weren't hit by a monster
	if(!isMonster)
		return;
	
	// monster might be thief, check it
	m_id = target - DND_MONSTERTID_BEGIN;

	// basic dot from monsters shouldn't apply these effects
	if(!isDot) {
		if(HasMonsterTrait(m_id, DND_THIEF)) {
			// get current weapon's ammo and steal it if possible
			temp = random(0, 1);
			str cur_ammo = GetWeaponAmmoType(GetActorWeaponID(this), temp);
			
			// if we picked no ammo, flip to check the other one using negation
			if(cur_ammo == "")
				cur_ammo = GetWeaponAmmoType(GetActorWeaponID(this), !temp);
			
			// if it's something we can take ammo from		
			if(cur_ammo != "")
				TakeActorInventory(this, cur_ammo, CheckActorInventory(this, cur_ammo) * DND_ELITE_THIEFRATE / 100);
		}

		// energy leech
		if(HasMonsterTrait(m_id, DND_ENERGYLEECH)) {
			temp = CheckActorInventory(this, "EShieldAmount") * DND_ENERGYLEECH_PCT / 100;
			if(temp) {
				GiveActorInventory(target, "MonsterFortifyCount", temp);
				if(CheckActorInventory(target, "MonsterFortifyCount") > MonsterProperties[m_id].maxhp)
					SetActorInventory(target, "MonsterFortifyCount", MonsterProperties[m_id].maxhp);
				TakeActorEnergyShield(this, temp);
			}
		}
		
		// shocker check
		if(HasMonsterTrait(m_id, DND_SHOCKER))
			GiveActorInventory(this, "PlayerStopper", 1);
		
		// ruination check
		if(HasMonsterTrait(m_id, DND_RUINATION)) {
			if(!CheckActorInventory(this, "RuinationStacks")) {
				SetActivator(this);
				
				ACS_NamedExecuteAlways("DnD Ruination Ticker", 0);
				
				// restore ptr
				SetActivator(0, AAPTR_DAMAGE_TARGET);
			}
			GiveActorInventory(this, "RuinationStacks", 1);

			if(CheckActorInventory(this, "RuinationStacks") == RUINATION_MAX_STACKS)
				HandleRuination(this, target);
		}

		// the curse is applied if the player is not immune, the checks are delegated to curse items
		if(HasMonsterTrait(m_id, DND_HEXFUSION) && random(1, 100) <= DND_HEXFUSION_CHANCE)
			ApplyRandomCurse(this, target);
	}
}

bool HandleRipperHit(int shooter, int victim) {
	// increment id by 1 for each call, doesnt matter if it overflows
	static int ripper_count = 0;
	static int ripper_hits[MAX_RIPPERS_ACTIVE][MAX_RIPPER_HITS_STORED];

	int i;

	// Id stored +1 so the item's zero state is "no id yet". Victims are stored +1 for the same reason:
	// ripper_hits is a static, ie. a MAP array that zero fills on every map load, and a -1 keyed table
	// reads all of those zeros as real entries -- the scan then runs to the cap and every rip returns
	// "already hit", which is a RIPSONCE weapon silently dealing nothing. 0 has to BE the empty slot.
	int ripper_id = CheckInventory("DnD_RipperId") - 1;
	if(ripper_id < 0) {
		ripper_id = ripper_count;
		ripper_count = (ripper_count + 1) % MAX_RIPPERS_ACTIVE;

		for(i = 0; i < MAX_RIPPER_HITS_STORED; ++i)
			ripper_hits[ripper_id][i] = 0;

		SetInventory("DnD_RipperId", ripper_id + 1);
	}

	bool found = false;

	for(i = 0; i < MAX_RIPPER_HITS_STORED && ripper_hits[ripper_id][i]; ++i) {
		if(ripper_hits[ripper_id][i] == victim + 1) {
			found = true;
			break;
		}
	}

	// record it as added into the array and return true
	if(!found && i < MAX_RIPPER_HITS_STORED) {
		ripper_hits[ripper_id][i] = victim + 1;
		return false;
	}

	return true;
}

// should this projectile reflect back? -- takes dmg_data as flags
// 0 is yes, 1 is no
bool CheckReflect(int owner, int pnum, int flags) {
	return 	CheckFlag(0, "DONTREFLECT") || CheckUniquePropertyOnPlayer(pnum, PUP_HOMINGNOREFLECT, CheckFlag(0, "SEEKERMISSILE"), CheckFlag(0, "SCREENSEEKER")) ||
			((flags & DND_DAMAGEFLAG_ISEXPLOSIVE) && HasActorClassPerk_Fast(owner, DND_PLAYER_MARINE, 2));
}

// shooter is who fired initially and victim is the tid of the actor that got hit that'll now own the projectile
void HandleReflect(int shooter, int victim, str proj_name, int encoded_data, int dmg_data, int spd, bool useGravity, int hit_x, int hit_y, int hit_z) {
	int pnum = shooter - P_TIDSTART;

	// the hit location
	Vec3_T* v_Pos = GetVec3(hit_x, hit_y, hit_z);

	// velocity vector will be randomized ie. towards the shooter but with some randomness
	Vec3_T* v_Vel = Vec3To_Pos(shooter, hit_x, hit_y, hit_z);
	ToUnitVec3(v_Vel);
	ScaleVec3(v_Vel, spd);
	v_Vel.z = FixedMul(v_Vel.z, random(0.75, 1.25));
	RotateVector3(v_Vel, ANG_TO_DOOM(random(-45.0, 45.0)));

	int wid, dmg, dtype;
	int dmg_category = 0;
	if(IsPlayer(shooter)) {
		// wepid
		wid = encoded_data & ATK_WID_MASK;
		encoded_data >>= ATK_CACHE_SHIFT;

		// dmg cache id
		dmg = encoded_data & ATK_CACHE_MASK;
		encoded_data >>= ATK_DTYPE_SHIFT;

		// dmg type
		dtype = encoded_data & ATK_DTYPE_MASK;
		encoded_data >>= ATK_DPCT_SHIFT;

		SetActivator(shooter);
		dmg_category = GetDamageCategory(dtype, dmg_data);

		if(!(dmg_data & DND_DAMAGEFLAG_ISSPECIALAMMO))
			dmg = RetrieveWeaponDamage(pnum, wid, dmg, dmg_category, dmg_data, 0);
		else {
			encoded_data = CheckInventory("DnD_WeaponID");
			dmg = RetrieveWeaponDamage(pnum, encoded_data, dmg, dmg_category, dmg_data, wid + 1);
			wid = encoded_data;
		}
		dmg_category = DamageCategoryToMonsterDamageType(dmg_category);
		//printbold(s:"retrieved dmg ", d:dmg);
	}
	else {
		dmg = encoded_data;
	}
	SetActivator(victim);

	dtype = DND_ATF_DAMAGEINEXTRA | DND_ATF_ISREFLECTED | (useGravity ? DND_ATF_USEGRAVITY : 0);

	CreateProjectile(
		victim,
		PROJECTILE_HELPER_TID + pnum,
		proj_name,
		AngleOfVector3(v_Vel),
		0, // pitch doesn't matter for this
		spd >> 16,
		v_Vel,
		v_Pos,
		dtype,
		dmg,
		dmg_data,
		dmg_category
	);
	bcs::free(v_Pos);
	bcs::free(v_Vel);
}

bool IsDamageEventException(str dt) {
	// special doomguy order of events check here
	if(dt == "Execution") {
		SetActivator(0, AAPTR_DAMAGE_TARGET);
		GiveInventory("Doomguy_ValidExecute", 1);
		GiveInventory("MonsterKilledByPlayer", 1);

		// QoL for doomguy
		ACS_NamedExecuteWithResult("DnD Monster Trait Take", DND_SUBORDINATE);
		return true;
	}
	else if(dt == "NoRealDamage") {
		SetActivator(0, AAPTR_DAMAGE_TARGET);
		HealThing(1);
		return true;
	}
	else if(dt == "InstantDeath" || dt == "Telefrag") {
		SetActivator(0, AAPTR_DAMAGE_TARGET);
		GiveInventory("MonsterKilledByPlayer", 1);
		return true;
	}
	
	return 	dt == "Suicide" || dt == "Perish" || dt == "Special_NoPain" || dt == "SkipHandle" || dt == "ForcedPainBypass";
}

void HandlePlayerChill(int pnum, int m_id, int dmg_received, int dmg_data) {
	// PlayerHitGainedFlags carries the touch traits' contribution: our caller only ever had the
	// attack's own type flags, so a RIMETOUCH monster's cold would be invisible here without it.
	if(m_id != -1 && ((dmg_data & DND_DAMAGETYPEFLAG_ICE) || (PlayerHitGainedFlags[pnum] & DND_DAMAGETYPEFLAG_ICE) || HasMonsterTrait(m_id, DND_FRIGID))) {
		// Only the cold part of the hit gets to chill. 100 for a plain ice hit and for a FRIGID
		// monster whose attack carries no ice at all, so both keep the whole number as before.
		dmg_received = MulPercent_Exact(dmg_received, PlayerHitIceShare[pnum]);

		// considerations for chill and freeze
		int health_cap = CheckInventory("PlayerHealthCap");
		int stacks = CheckInventory("DnD_ChillStacks");
		
		// if hpdiff >= threshold
		if
		(
			GetPlayerElementalAvoidChance(pnum, DND_PAVOID_CHILLFREEZE) < random(1, 100) && 
			dmg_received >= health_cap * GetMonsterChillThreshold(m_id) / 100
		)
		{
			HandleRiskAversion();

			// add a new stack of chill if applicable
			if(!CheckInventory("DnD_ChillGainCooldown") && stacks < DND_BASE_CHILL_CAP) {
				GiveInventory("DnD_ChillStacks", 1);
				GiveInventory("DnD_ChillGainCooldown", 1);
				
				// give chill debuff
				ACS_NamedExecuteWithResult("DnD Give Buff", DND_DEBUFF_CHILL, DEBUFF_F_PLAYERISACTIVATOR | DEBUFF_F_OWNERISTARGET);
			}
			
			// freeze checks
			health_cap = GetMonsterFreezeChance(m_id, CheckInventory("DnD_ChillStacks"));
			if(random(1, 100) <= health_cap) {
				stacks = DND_BASE_FREEZETIMER;
				
				// set freeze timer and run script
				SetInventory("DnD_FreezeTimer", stacks);
				ACS_NamedExecuteWithResult("DnD Give Buff", DND_DEBUFF_FREEZE, DEBUFF_F_PLAYERISACTIVATOR | DEBUFF_F_OWNERISTARGET);
			}
		}
	}
}

Script "DnD Parry Weakness Time" (int tid, int amt) {
	SetActivator(tid);

	str pSound = GetActorProperty(0, APROP_PAINSOUND);
	SetActorProperty(0, APROP_PAINSOUND, "");

	while(isAlive() && CheckInventory("DnD_ParryWeaknessTimer")) {
		TakeInventory("DnD_ParryWeaknessTimer", 1);
		Delay(const:DND_PARRY_WEAKNESS_TICS);
	}

	SetActorProperty(0, APROP_PAINSOUND, pSound);
	TakeInventory("DnD_ParryWeakness", 1);
}

Script "DnD Event Handler" (int type, int arg1, int arg2) EVENT {
	// in monster shooting player case, temp holds accuracy stored in the projectile!
	int temp, dmg, m_id;
	int pnum;
	int victim;
	if(type == GAMEEVENT_ACTOR_DAMAGED) {
#ifdef ISDEBUGBUILD
		++damage_script_count;
#endif
		//printbold(s:"dmg type: ", s:arg2);
		// arg1 contains damage, arg2 contains damage type as a string
		// this causes A_KillChildren etc. to actually work...
		if(IsDamageEventException(arg2)) {
			if(arg2 == "Telefrag") {
				victim = ActivatorTID();
				SetActivator(0, AAPTR_DAMAGE_SOURCE);
				if(isPlayer(ActivatorTID()) && isPlayer(victim))
					arg1 = 0;
			}
			else if(arg2 == "Execution") {
				// The punch behind an execute carries a flat number out of DECORATE, and a flat
				// number stops being a kill the moment a monster's health passes it -- which is
				// exactly what a boss does, so the execute landed as an ordinary hit and left it
				// standing. An execute is meant to kill whatever it lands on, so the number comes
				// from the victim rather than from the weapon.
				SetActivator(0, AAPTR_DAMAGE_TARGET);

				// Only a target that was actually marked executable earns the guaranteed kill.
				// A_CustomPunch traces forward on its own and can land on a monster standing
				// between the player and the one that was checked; that one keeps the weapon's
				// own number instead of dying outright to a hit that was never aimed at it.
				if(CheckInventory("Doomguy_CanExecute")) {
					arg1 = GetActorProperty(0, APROP_HEALTH);

					// doubled for the reason the cull path doubles -- nothing applied downstream
					// of this return gets to leave the target standing on a sliver of health.
					// Guarded because at these health values the doubling is what overflows first.
					if(arg1 > bcs::INT_MAX / 2)
						arg1 = bcs::INT_MAX;
					else
						arg1 *= 2;
				}
			}
			SetResultValue(arg1);
			Terminate;
		}

		bool isRipper = false;
		int shooter = -1;
		int orig_dmg = arg1;

		SetActivator(0, AAPTR_DAMAGE_TARGET);
		victim = ActivatorTID();

		// damage inflictor (projectile etc.) -- reflected projectiles seem to have "None" as their class
		// poisonDOT or any DOT has this characteristic as well so we must check for those as exceptions here
		SetActivator(0, AAPTR_DAMAGE_INFLICTOR);
		if(arg2 == "Melee" && !GetActorProperty(0, APROP_TARGETTID))
			shooter = ActivatorTID(); // apparently the damagesource is 0 under melee case for some reason...
		//printbold(s:GetactorClass(0), s:" inflicts damage ", d:arg1, s:" type ", s:arg2);
		int dmg_data = GetActorProperty(0, APROP_STAMINA);
		//printbold(s:"dmg flag: ", d:dmg_data);
		int inflictor_class = GetActorClass(0);

		int ox = GetActorX(0);
		int oy = GetActorY(0);
		int oz = GetActorZ(0);
		int factor = 0;
		int dist_damage_bonus = 0;

		int actor_flags = ScanActorFlags();
		temp = GetActorProperty(0, APROP_ACCURACY);
		if(temp == DND_CRIT_TOKEN) {
			actor_flags |= DND_ACTORFLAG_CONFIRMEDCRIT;
			// printbold(s:"actor got crit confirm");
		}

		if(dmg_data & DND_DAMAGEFLAG_DISTANCEGIVESDAMAGE)
			dist_damage_bonus = GetUserVariable(0, DND_DISTANCEDAMAGE_VARIABLE);

		int isArmorPiercing = CheckFlag(0, "PIERCEARMOR");
		isRipper = CheckFlag(0, "RIPPER");

		// set activator to owner of this projectile for certain crediting
		SetActivator(0, AAPTR_DAMAGE_SOURCE);
		if(shooter == -1) {
			//printbold(s:"this ? ", d:ActivatorTID(), s: " ", s:GetActorClass(ActivatorTID()));
			if(!(dmg_data & DND_DAMAGETYPEFLAG_USEMASTER) || GetActorProperty(0, APROP_SPECIES) != "Spider2")
				shooter = ActivatorTID();
			else
				shooter = GetActorProperty(0, APROP_MASTERTID);
		}

		// this flag shares the same value as a damagetype for monsters, so we need to separate it
		SetActivator(0, AAPTR_DAMAGE_INFLICTOR);
		if((shooter == -1 || shooter == 0) && !IsMonster(shooter) && (dmg_data & DND_DAMAGEFLAG_USEMASTER)) {
			//printbold(s:"take shooter as ", d:GetActorProperty(0, APROP_SCORE));
			shooter = GetActorProperty(0, APROP_SCORE);
		}

		//printbold(s:"dmg source: ", s:GetActorClass(0), s: " ", s:GetActorClass(shooter));

		// if whatever fired this is a monster but original owner was player, it's reflected
		// NOTE: DONT USE TEMP UNTIL REFLECTION CHECK IS DONE
		int isReflected = GetActorProperty(0, APROP_SCORE);
		temp = GetActorProperty(0, APROP_PLAYERSOURCED);
		if
		(
			(
				IsPlayer(shooter) && 
				IsMonster(victim) && 
				!(dmg_data & DND_DAMAGEFLAG_ISHITSCAN) && 
				!(dmg_data & DND_DAMAGEFLAG_ISRADIUSDMG) && 
				HasTrait(victim - DND_MONSTERTID_BEGIN, DND_REFLECTIVE) &&
				!CheckReflect(shooter, shooter - P_TIDSTART, dmg_data)
			) ||
			(
				IsMonster(shooter) &&
				IsPlayer(victim) &&
				(dmg_data & DND_DAMAGETYPEFLAG_REFLECTABLE) &&
				!CheckFlag(0, "DONTREFLECT") &&
				CheckActorInventory(victim, "ReflectiveState")
			)
		)
		{
			// fire back at player the same thing that hurt the player
			Thing_ChangeTID(0, AUX_PROJ_TID + shooter - P_TIDSTART);

			isArmorPiercing = GetActorProperty(0, APROP_SPEED);
			isRipper = !CheckFlag(0, "NOGRAVITY");
			factor = GetActorClass(0);

			SetActivator(0, AAPTR_DAMAGE_TARGET);

			// handle reflection firing projectile code
			if(isPlayer(shooter))
				dmg_data |= DND_DAMAGEFLAG_ISREFLECTED;
			HandleReflect(shooter, ActivatorTID(), factor, arg1, dmg_data, isArmorPiercing, isRipper, ox, oy, oz);

			Thing_Remove(AUX_PROJ_TID + shooter - P_TIDSTART);
			SetResultValue(0);
			Terminate;
		}

		SetActivator(0, AAPTR_DAMAGE_SOURCE);
		if(dmg_data & DND_DAMAGEFLAG_ISHITSCAN) {
			ox = GetActorX(shooter);
			oy = GetActorY(shooter);
			oz = GetActorZ(shooter);
		}

		// if the inflictor had no damage data for some reason, try to look it up from the monster
		if(!dmg_data)
			dmg_data = GetActorProperty(0, APROP_STAMINA);
		isArmorPiercing |= CheckFlag(shooter, "PIERCEARMOR");

		// set the activator to us now
		SetActivator(0, AAPTR_DAMAGE_TARGET);
		//printbold(s:"Victim is ", s:GetActorClass(0));
		
		dmg = arg1;

		//printbold(s:"Dmg Data ", d:dmg_data, s:" ", d:dmg, s:" ", d:shooter, s: " ", d:IsPlayer(shooter));

		// FROM HERE ON WHOEVER TOOK DAMAGE IS THE ACTIVATOR, PLAYER OR MONSTER!
		bool isDot = false;
		if(IsMonster(shooter)) {
			m_id = shooter - DND_MONSTERTID_BEGIN;
			isArmorPiercing |= HasMonsterTrait(m_id, DND_PIERCE);
		
			// if victim was a monster, check for infight situation
			// BOTH VICTIM AND SHOOTER ARE MONSTERS HERE
			//printbold(s:GetActorProperty(victim, APROP_SPECIES), s: " ", s:GetActorProperty(shooter, APROP_SPECIES));
			if
			(
				// added condition for friendlies
				(IsPlayer(victim) && GetActorProperty(shooter, APROP_SPECIES) == "Player") ||
				(
					IsMonster(victim) && 
					victim != shooter && 
					GetActorProperty(victim, APROP_SPECIES) == GetActorProperty(shooter, APROP_SPECIES) && !(dmg_data & DND_DAMAGETYPEFLAG_HURTSPECIES)
				)
			)
			{
				// no damage dealt from same species, makes damage things much easier to keep track of	
				SetResultValue(0);
				Terminate;
			}

			// DoT shouldn't double dip
			isDot = IsDamageStringDOT(arg2);
			if(!isDot) {
				// dont scale reflected damage by this -- reflected damage will have score property as damage of proj, if it's not a player tid its reflected
				//printbold(s:"here?? ", d:isReflected, s: " ", d:dmg_data & DND_DAMAGEFLAG_ISREFLECTED);
				// DONT USE TEMP UNTIL HERE!!!!!
				//printbold(s:"incoming damage: ", d:dmg, s: " reflect ambush flag: ", d:temp, s: " reflect dmg flag: ", d:dmg_data & DND_DAMAGEFLAG_ISREFLECTED);
				if(!temp && !(dmg_data & DND_DAMAGEFLAG_ISREFLECTED)) {
					// special bonuses
					factor  = 	(MonsterProperties[m_id].level > 1) * MonsterProperties[m_id].dmg_bonus;
					dmg = dmg * (100 + factor) / 100;

					// % damage effects -- this is same for all monsters which is 10% of player's maximum health added as damage
					dmg += HandlePercentDamageFromEnemy(victim, dmg, dmg_data);
				}
				else {
					// unpack regular weapon dmg from isReflected variable
					arg1 = isReflected;
					dmg = arg1;
					arg2 = "Reflection";
					//printbold(s:"player reflected damage ", d:dmg);
					isReflected = true;
				}
			}

			if(isRipper)
				dmg >>= 1;
				
			// halved by demon sealer effect if any
			if(CheckActorInventory(shooter, "DemonSealDamageDebuff"))
				dmg >>= 1;

			// if this was a player, factor their resists in
			// resists of player now will factor in after we've calculated the damage accurately
			// phasing also negates non-radius and hitscan damage
			if(IsPlayer(victim)) {
				pnum = victim - P_TIDSTART;

				if
				(
					CheckInventory("DnD_CountdownProtection") ||
					(
						!(dmg_data & (DND_DAMAGETYPEFLAG_EXPLOSIVE | DND_DAMAGETYPEFLAG_HITSCAN | DND_DAMAGETYPEFLAG_DOT)) && !isDot && arg2 != "Melee" && HasPlayerBuff(pnum, BTI_PHASING)
					) ||
					(
						GetDodgeChance(pnum) >= random(0.01, 100.0) && !(dmg_data & DND_DAMAGETYPEFLAG_EXPLOSIVE)
					)
				)
				{
					SetResultValue(0);
					Terminate;
				}
				else if(!isDot && CheckActorInventory(victim, "DnD_Parrying")) {
					// spawn parry aoe
					isReflected = GetActorAngle(victim);
					ox = cos(isReflected);
					oy = sin(isReflected);

					SpawnForced(
						"ParryAoE", 
						GetActorX(victim) + DND_PARRY_SPAWNDIST * ox,
						GetActorY(victim) + DND_PARRY_SPAWNDIST * oy,
						GetActorZ(victim) + 64.0,
						AUX_PARRY_TID + pnum
					);

					SetActivator(AUX_PARRY_TID + pnum);
					SetActorProperty(0, APROP_TARGETTID, victim);
					SetPointer(AAPTR_TARGET, victim);
					Thing_ChangeTID(AUX_PARRY_TID + pnum, 0);

					GiveActorInventory(victim, "DnD_ParryDamageReduction", 1);

					// remove the projectile or attack that hit us if it's not a monster!!
					SetActivator(0, AAPTR_DAMAGE_INFLICTOR);
					ox = ActivatorTID();
					if(!ox) {
						Thing_ChangeTID(0, AUX_PROJ_TID);
						Thing_Remove(AUX_PROJ_TID);
					}

					SetResultValue(0);
					Terminate;
				}

				// out of combat hit timer, 3 seconds
				if(!HasActorClassPerk_Fast(victim, DND_PLAYER_CYBORG, 2) || random(0, 1.0) <= DND_CYBORG_REGENCONTCHANCE)
					GiveInventory("DnD_Hit_CombatTimer", 1);
				
				if(!CheckInventory("DnD_Hit_Cooldown")) {
					OnPlayerHit(victim, pnum, shooter, true, isDot);
					GiveInventory("DnD_Hit_Cooldown", 1);
				}
				
				// hate shard reflection
				if(CheckInventory("HateCheck")) {
					// this is needed for kill credit
					HandleMonsterDeathConfirm(shooter, dmg);
					Thing_Damage2(shooter, dmg, "Reflection");
					ACS_NamedExecuteWithResult("DnD Damage Numbers", shooter, dmg, 0);
				}
				
				// check for special reduced damage factors
				// store damage before reductions to apply to armor later
				dmg = HandlePlayerResists(pnum, dmg, arg2, dmg_data, isReflected, inflictor_class, m_id);

				// A touch trait genuinely put another element into this hit, and everything below
				// still keys off the attack's ORIGINAL flags -- dmg_data is by value, so the OR that
				// HandlePlayerResists does to it never came back here. Without this the Hellfire
				// Amulet misses an Ember Touch hit's fire and a Rime Touch hit's cold, and Lightning
				// Coil does not absorb against Storm Touch, even though the damage really is that
				// type and already met that type's resist inside the call above.
				dmg_data |= PlayerHitGainedFlags[pnum];

				dmg = HandlePlayerOnHitBuffs(victim, shooter, dmg, dmg_data, arg2);
				// finally apply player armor
				dmg = HandlePlayerArmor(pnum, dmg, arg2, dmg_data, isArmorPiercing);

				// berserker damage reduction
				temp = CheckInventory("Berserker_DamageTracker");
				if(temp)
					dmg = MulPercent_Exact(dmg, 100 - temp * DND_BERSERKER_PERK20_REDUCTION);

				// parry dmg reduction
				if(CheckInventory("DnD_ParryDamageReduction"))
					dmg = dmg * (100 - DND_PARRY_DAMAGEREDUCTION) / 100;

				// doomguy damage reduction
				if(HasClassPerk_Fast(DND_PLAYER_DOOMGUY, 1) && CheckActorInventory(shooter, "Doomguy_CanExecute"))
					dmg = MulPercent_Exact(dmg, 100 - DND_DOOMGUY_DMGREDUCE_PERCENT - (HasClassPerk_Fast(DND_PLAYER_DOOMGUY, 3)) * DND_DOOMGUY_DMGREDUCE_PERK3BONUS);

				// final check, if damage is less than 10% of it, cap it at 10%
				temp = arg1 / 10;
				if(temp <= 0)
					temp = 1;

				if(dmg < temp)
					dmg = temp;

				// this is the real damage going to player regardless of their eshield
				HandlePlayerChill(pnum, m_id, dmg, dmg_data);

				// the real final check vs eshield
				dmg = ApplyTrueDamageDeductions(pnum, dmg, arg2, dmg_data);

				if(dmg) {
					// add to player stat
					IncrementStatistic(DND_STATISTIC_DAMAGETAKEN, dmg, victim);
					//GiveInventory("DnD_DamageReceived", dmg);
					PlayerScriptsCheck[DND_SCRIPT_DAMAGETAKENTIC][pnum] = dmg;
					PlayerScriptsCheck[DND_SCRIPT_BLEND][pnum] = false;

					if(HasClassPerk_Fast(DND_PLAYER_MARINE, 4) && !CheckInventory("Marine_Perk50_Cooldown"))
						GiveInventory("Marine_Perk50_DamageTaken", dmg);
					
					if(HasClassPerk_Fast(DND_PLAYER_TRICKSTER, 3) && !CheckInventory("Trickster_ShadowCooldown") && GetActorProperty(0, APROP_HEALTH) - dmg <= CheckInventory("PlayerHealthCap") * DND_TRICKSTER_PERK40_THRESHOLD / 100)
						HandleShadowClone(pnum, victim, shooter);
				}

				// these are on monsters only, dont have much to do with us beyond this point
				HandleMonsterDamageModChecks(m_id, shooter, victim, dmg, isDot);

				if(CheckMapEvent(DND_MAPEVENT_CULLINGMONSTERS) && CheckCullRangeVsPlayer(shooter, victim, dmg))
					dmg = 99999;
			}
			else {
				temp = GetActorProperty(shooter, APROP_MASTERTID);
				if(IsPlayer(temp)) {
					dmg = HandlePetMonsterDamageScale(shooter, temp, victim, dmg, dmg_data, -1);

					// these are on monsters only, dont have much to do with us beyond this point
					HandleMonsterDamageModChecks(m_id, shooter, victim, dmg, isDot);
					
					if(GetActorProperty(victim, APROP_HEALTH) <= dmg)
						GiveActorInventory(victim, "MonsterKilledByPlayer", 1);
				}
			}
			
			SetResultValue(dmg);
		}
		else if(IsPlayer(shooter) && !IsPlayer(victim)) {
			// PLAYER HURTING MONSTERS CODE HERE
			// extract the encoded damage data, and proceed
			// stamina contains any special flags we might need
			// variable swap here to fix a bug with radius damage projectiles that also rip once
			//printbold(s:"dmg ", d:dmg, s:" victim ", d:victim, s:" ", d:shooter, s:" ", d:dmg_data, s:" ", d:isReflected, s:" ", d:IsPlayer(isReflected));
			if(!(dmg_data & DND_DAMAGEFLAG_ISREFLECTED) && (!isReflected || IsPlayer(isReflected))) {
				factor = arg1;
				//printbold(s:"factor = ", d:arg1);
				SetActivator(0, AAPTR_DAMAGE_INFLICTOR);
				if(dmg_data & DND_DAMAGEFLAG_RIPSONCE) {
					// insert victim to a temporary array and check if it exists in there before continuing
					// this marks that we had a ripper case that we need to handle later that we hit, we havent got access to weapon id at this point so we cant know until later
					if(!HandleRipperHit(shooter, victim)) {
						isArmorPiercing = true;

						// we really care about this if the ripper hits like this, not explosion portion at all
						if((temp = GetUserVariable(0, "user_expdmg")))
							arg1 = temp;
					}
					else {
						// ignore this event
						SetResultValue(0);
						Terminate;
					}
				}

				if(dmg_data & DND_DAMAGEFLAG_ISRADIUSDMG) {
					// explosions do not hit monsters under these conditions
					if(!CheckFlag(victim, "SHOOTABLE") || (CheckFlag(victim, "GHOST") && (actor_flags & DND_ACTORFLAG_THRUGHOST))) {
						SetResultValue(0);
						Terminate;
					}

					// save the percentage of damage from radius falloff into somewhere (arg1 base is 100, so we can use it as percentage)
					//printbold(s:arg2, s: " aoe dmg ", d:GetUserVariable(0, "user_expdmg"), s: " factor ", d:factor);
					arg2 = factor;
					arg1 = GetUserVariable(0, "user_expdmg");
					dmg_data |= GetUserVariable(0, "user_expflags");
				}
				
				SetActivator(shooter);

				pnum = shooter - P_TIDSTART;

				// spells get raw damage in here, they don't encode weapon data or anything
				if(dmg_data & DND_DAMAGEFLAG_NONWEAPON) {
					// non-weapon sources that aren't spells
					dmg = arg1 & NONWEP_DMG_MASK; // dmg
					temp = arg1 >> NONWEP_DMG_SHIFT; // dmg_type
					m_id = -1;

					if(temp == DND_DAMAGETYPE_PARRY && !CheckFlag(victim, "NOPAIN")) {
						SetActorInventory(victim, "DnD_ParryWeaknessTimer", GetPlayerParryWeakenTimer(pnum, victim));
						if(!CheckActorInventory(victim, "DnD_ParryWeakness")) {
							GiveActorInventory(victim, "DnD_ParryWeakness", 1);
							ACS_NamedExecuteWithResult("DnD Parry Weakness Time", victim);
						}
					}

					//printbold(s:"dmg = ", d:dmg, s: " flags = ", d:temp);
				}
				else if(dmg_data & DND_DAMAGEFLAG_ISSPELL) {
					// first 16 bits is spell damage, next is damage type and last is spell id
					dmg = arg1 & SPELLDMG_MASK;
					dmg += dist_damage_bonus; // depending on distance increasing damage modifier this can be non-zero

					arg1 >>= SPELL_DMG_SHIFT;
					temp = arg1 & SPELLDTYPE_MASK;

					arg1 >>= SPELL_DTYPE_SHIFT;
					m_id = arg1 & SPELLID_MASK;

					//printbold(s:"spell ", d:dmg);
				}
				else {
					// regular weapon dmg
					// wepid
					m_id = arg1 & ATK_WID_MASK;
					arg1 >>= ATK_CACHE_SHIFT;

					// dmg cache id
					dmg = arg1 & ATK_CACHE_MASK;
					arg1 >>= ATK_DTYPE_SHIFT;

					// dmg type
					temp = arg1 & ATK_DTYPE_MASK;
					arg1 >>= ATK_DPCT_SHIFT;

					// weapon check for sedrin staff
					if(m_id == DND_WEAPON_SEDRINSTAFF && IsActorFullRobotic(victim)) {
						ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, 0, 0, DND_HITBEEP_IMMUNITY);
						SetResultValue(0);
						Terminate;
					}
					else if(m_id == DND_WEAPON_ADMINPISTOL) {
						dmg = 2 * GetActorProperty(victim, APROP_HEALTH);
						GiveActorInventory(victim, "MonsterKilledByPlayer", 1);
						SetResultValue(dmg);
						Terminate;
					}

					// dmg still holds the dmgid at this point and is about to be overwritten with the
					// rolled damage -- the components need it further down, keyed the same way, and
					// the category with it so the staged split can be matched back to this hit
					int added_dmgid = dmg;
					int added_category = GetDamageCategory(temp, dmg_data);

					// the one call site that can emit components, hence the only one that asks for them
					if(!(dmg_data & DND_DAMAGEFLAG_ISSPECIALAMMO))
						dmg = RetrieveWeaponDamage(pnum, m_id, dmg, added_category, dmg_data, 0, true);
					else {
						// special ammo correction
						factor = CheckInventory("DnD_WeaponID");
						dmg = RetrieveWeaponDamage(pnum, factor, dmg, added_category, dmg_data, m_id + 1, true);
						m_id = factor;
					}

					//printbold(s:"retrieved dmg ", d:dmg);

					dmg += dist_damage_bonus; // depending on distance increasing damage modifier this can be non-zero
					// % adjustment factor -- extract after the flat addition to reuse variables
					factor = arg1 & ATK_DPCT_MASK;

					if(dmg_data & DND_DAMAGEFLAG_LOSEDAMAGEPERHIT)
						dmg = ACS_NamedExecuteWithResult("DnD Adjust Impact Damage", dmg_data, dmg, shooter);

					// setup the flags and factor
					if(factor != 100)
						dmg = dmg * factor / 100;
				}

				if(dmg_data & DND_DAMAGEFLAG_ISRADIUSDMG) {
					//printbold(d:dmg, s: " ", d:arg2, s:" ", d:dmg * arg2 / 100);
					dmg = dmg * arg2 / 100;

					if(!dmg) {
						SetResultValue(0);
						Terminate;
					}
				}

				if((dmg_data & DND_DAMAGEFLAG_FOILINVUL) || (HasPlayerFlag(pnum, PFLAG_MELEEIGNORESHIELD) && (IsMeleeDamage(temp) || (dmg_data & DND_DAMAGEFLAG_COUNTSASMELEE)))) {
					actor_flags |= DND_ACTORFLAG_FOILINVUL;
					dmg_data ^= DND_DAMAGEFLAG_FOILINVUL;
				}
				
				if(dmg_data & DND_DAMAGEFLAG_COUNTSASMELEE) {
					actor_flags |= DND_ACTORFLAG_COUNTSASMELEE;
					dmg_data ^= DND_DAMAGEFLAG_COUNTSASMELEE;
				}

				if(isArmorPiercing)
					HandleRipperHitSound(victim, shooter, m_id);

				// damage boost on overheating things
				// factor variable isnt used below for anything saved prior so we can use it too
				isArmorPiercing = PlayerModData[pnum].vals[PSTAT_EX_MOREDMGPEROVERHEAT];
				factor = GetCurrentWeaponID();
				if(isArmorPiercing && CanWeaponOverheat(factor)) {
					// add the extra damage as "more" on top --- ammo2 is always the overheat on overheating weapons
					factor = CheckInventory(Weapons_Data[factor].ammo_name2);
					dmg = dmg * (((100.0 + isArmorPiercing * factor) >> 16)) / 100;
				}

				// Class effects here -- isArmorPiercing holds if wepid is negative or not
				isArmorPiercing = (m_id < 0 || (dmg_data & (DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISSPECIALAMMO)));
				if(!isArmorPiercing) {
					// berserker perk50 dmg increase portion and other melee increases
					if((IsMeleeWeapon(m_id) || (actor_flags & DND_ACTORFLAG_COUNTSASMELEE))) {
						if(HasClassPerk_Fast(DND_PLAYER_BERSERKER, 4)) {
							SetInventory("Berserker_HitTimer", DND_BERSERKER_PERK60_TIMER);
							if
							(
								!CheckInventory("Berserker_Perk60_HitCooldown") &&
								(factor = CheckInventory("Berserker_HitTracker")) < DND_BERSERKER_PERK60_MAXSTACKS && 
								(arg2 = CheckInventory("Berserker_Perk60_HitCounter")) < DND_BERSERKER_PERK60_MAXHITS &&
								!CheckInventory("Berserker_Perk80_Extension")
							)
							{
								GiveInventory("Berserker_HitTracker", 1);

								GiveInventory("Berserker_Perk60_HitCounter", 1);
								if(arg2 + 1 >= DND_BERSERKER_PERK60_MAXHITS) {
									// now that we hit cooldown time, reset the counter
									GiveInventory("Berserker_Perk60_HitCooldown", 1);
									SetInventory("Berserker_Perk60_HitCounter", 0);
								}

								if(!factor)
									ACS_NamedExecuteAlways("DnD Berserker Perk50 Timer", 0, shooter);
							}

							if(factor + 1 >= DND_BERSERKER_PERK60_MAXSTACKS) {
								if(!CheckInventory("Berserker_NoRoar"))
									HandleBerserkerRoar(shooter);
								GiveInventory("Berserker_Perk60_Speed", 1);
							}
							dmg = dmg * (100 + (factor + 1) * DND_BERSERKER_PERK60_DMGINCREASE) / 100;
						}

						if(CheckInventory("DnD_StaminaDepleted"))
							dmg = dmg * (100 - DND_DEPLETEDSTAMINA_FACTOR) / 100;
						else if(IsOnLowStamina())
							dmg = dmg * (100 - DND_LOWSTAMINA_FACTOR) / 100;
						
						dmg = dmg * (
							100 + 
							PlayerModData[pnum].vals[PSTAT_MELEEDAMAGE] + 
							CheckActorInventory(victim, "DnD_ParryWeakness") * DND_PARRY_DAMAGEWEAKNESS
						) / 100;

						if(CheckActorInventory(victim, "DnD_CrushingBlow"))
							dmg = dmg * (100 + DND_CRUSHINGBLOW_PCT) / 100;

						factor = PlayerModData[pnum].vals[PSTAT_CRUSHINGBLOW];
						if(factor && random(1, 100) <= factor) {
							ACS_NamedExecuteAlways("DnD Special Fx Spawner", 0, victim, INV_CRUSHINGBLOW);
							GiveActorInventory(victim, "DnD_CrushingBlow", 1);
						}

						factor = PlayerModData[pnum].vals[PSTAT_DEEPCUTS];
						if(factor && random(1, 100) <= factor) {
							ACS_NamedExecuteAlways("DnD Special Fx Spawner", 0, victim, INV_DEEPCUTS);
							GiveActorInventory(victim, "DnD_DeepCuts", 1);
						}

						factor = PlayerModData[pnum].vals[PSTAT_OPENWOUNDS];
						if(factor && random(1, 100) <= factor) {
							ACS_NamedExecuteAlways("DnD Special Fx Spawner", 0, victim, INV_OPENWOUNDS);
							GiveActorInventory(victim, "DnD_OpenWounds", 1);
						}
					}
					
					// Flayer magic or undead check -- explosive flag check to prevent it from calling itself
					if(m_id == DND_WEAPON_CROSSBOW && IsActorMagicOrUndead(victim) && !(dmg_data & (DND_DAMAGEFLAG_ISEXPLOSIVE | DND_DAMAGEFLAG_ISRADIUSDMG)))
						ACS_NamedExecuteWithResult("DnD Crossbow Explosion", victim, shooter);
				}

				// cyborg perk50
				if(!isArmorPiercing && HasClassPerk_Fast(DND_PLAYER_CYBORG, 3) && IsTechWeapon(m_id)) {
					factor = CheckInventory("Cyborg_InstabilityStack");

					SetInventory("Cyborg_Instability_Timer", DND_CYBORG_INSTABILITY_TIMER + HasClassPerk_Fast(DND_PLAYER_CYBORG, 4) * DND_CYBORG_INSTABILITY_BONUS);
					if(!factor)
						ACS_NamedExecuteAlways("DnD Cyborg Instability Timer", 0);
					else if(factor == DND_MAXCYBORG_INSTABILITY - 1 && !CheckInventory("Cyborg_NoAnim")) {
						// we check -1 above because we'll give 1 already
						PlaySound(shooter, "Cyborg/Unstable", CHAN_BODY, 1.0);
						GiveInventory("Cyborg_NoAnim", 1);
						ACS_NamedExecuteAlways("DnD Cyborg Visor Anim", 0);
					}

					if(!CheckInventory("Cyborg_Instability_StackGainCD")) {
						GiveInventory("Cyborg_InstabilityStack", 1);
						GiveInventory("Cyborg_Instability_StackGainCD", 1);
					}
				}
			}
			else {
				SetActivator(shooter);
				m_id = -1;
				dmg = isReflected;
			}

			// 25% less damage taken
			if(CheckActorInventory(victim, "WarmasterProtect"))
				dmg = 3 * dmg / 4;

			if(isRipper) {
				SetActivator(0, AAPTR_DAMAGE_INFLICTOR);
				isArmorPiercing = ActivatorTID();
				if(isArmorPiercing != shooter) {
					GiveInventory("DnD_RipCount", 1);
					isReflected = CheckInventory("DnD_RipCount");
					isArmorPiercing = CheckInventory("DnD_RipLimit");
					// if we reach ripcount and we aren't a "super ripper"
					if(isArmorPiercing != MAX_RIPCOUNT && (HasMonsterTrait(victim - DND_MONSTERTID_BEGIN, DND_HARDENED_SKIN) || isReflected >= isArmorPiercing))
						GiveInventory("TakeRipperAway", 1);
					isArmorPiercing = PlayerModData[pnum].vals[PSTAT_RIPDAMAGE];
					dmg = dmg * (100 + isArmorPiercing * isReflected) / 100;
				}
				SetActivator(shooter);
			}
 
			// finally dealing the damage -- temp holds damage type, dont use temp above here!
			if(victim) {
				//printbold(s:"dmg deal to ", d:victim, s:" dmg ", d:dmg, s: " wepid ", d:m_id);

				// "+10 cold damage to attacks" and "50% of physical converted to cold", each as its
				// own typed instance so it hits its own resists and can inflict its own ailment. The
				// remainder is what the weapon's own type still deals.
				//
				// ATTACKS ONLY. This deliberately sits here rather than inside HandleDamageDeal: most
				// of that function's callers are DOT ticks, procs and spells, and a fire DOT ticking
				// several times a second would otherwise re-add its cold component on every tick. It
				// is also the last point that still knows the dmgid and category the split was keyed
				// on, and the first where every type-agnostic multiplier has already landed.
				dmg = DealDamageComponents(pnum, shooter, victim, m_id, added_dmgid, added_category, dmg, dmg_data, actor_flags);

				if(dmg > 0)
					dmg = HandleDamageDeal(shooter, victim, dmg, temp, m_id, dmg_data, ox, oy, oz, actor_flags, (m_id < 0) || (dmg_data & DND_DAMAGEFLAG_ISSPELL), 0);

				// failsafe -- hopefully not necessary anymore
				if(GetActorProperty(victim, APROP_HEALTH) > MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp) {
					if(m_id >= 0)
						Log(
							s:"Monster hp overflow on ", s:GetActorClass(victim), s:" hp: ", d:GetActorProperty(victim, APROP_HEALTH), s:" / ", d:MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp, 
							s: " with player weapon and dmg: ", s:Weapons_Data[m_id].name, s: " <> ", d:dmg, s:" m_id: ", d:m_id
						);
					else
						Log(
							s:"Monster hp overflow on ", s:GetActorClass(victim), s:" hp: ", d:GetActorProperty(victim, APROP_HEALTH), s:" / ", d:MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp, 
							s: " with player (weapon was neg) dmg: ", d:dmg, s:" m_id: ", d:m_id
						);
					SetActorProperty(victim, APROP_HEALTH, MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp);
				}

				if(dmg <= 0)
					dmg = 0;
				else if
				(
					(dmg_data & DND_DAMAGEFLAG_ISMELEE) && 
						(
							HasActorClassPerk_Fast(shooter, DND_PLAYER_BERSERKER, 3) || 
							(!IsOnLowStamina() && PlayerModData[pnum].vals[PSTAT_MELEESPLASH_NOTONLOWSTAMINA] >= random(1, 100)) ||
							(ox = CheckPlayerCleave(pnum))
						)
				)
				{
					// check if the berserker perk for splashing melees is there
					orig_dmg &= BITMASK_NOFACTOR;
					orig_dmg |= DND_BERSERKER_PERK40_SPLASHPCT << DPCT_SHIFT;

					// store potential cleave outcome
					isRipper = ox;
					if(isRipper)
						ACS_NamedExecuteAlways("DnD Special Fx Spawner", 0, shooter, INV_REAPINGCLEAVE);
					
					arg1 = GetActorProperty(0, APROP_STAMINA);
					SetActorProperty(0, APROP_STAMINA, dmg_data ^ DND_DAMAGEFLAG_ISMELEE);

					arg2 = GetPlayerMeleeRange(pnum, DND_BERSERKER_PERK40_SPLASHRADIUS);

					if(inflictor_class == "ThunderAxePuff") {
						inflictor_class = "ThunderAxePuff_NoChain";
						//printbold(s:"inflictor changed to ", s:inflictor_class);
					}

					// search if any monster fits the criteria for a splash
					for(temp = 0; temp < InformationInLevel[LEVELINFO_TID_MONSTER]; ++temp) {
						ox = UsedMonsterTIDs[temp];
						// to be affected by gravdis debuff
						oy = GetActorX(shooter) - GetActorX(ox);
						oz = GetActorY(shooter) - GetActorY(ox);
						if
						(
							ox != victim && isActorAlive(ox) && AproxDistance(oy, oz) <= arg2 &&
							(isRipper || MaxAngleDiff(shooter, ox, DND_BERSERKER_PERK40_SPLASHANGLE)) && CheckSight(shooter, ox, CSF_NOBLOCKALL)
						)
						{
							Thing_Damage2(ox, orig_dmg, "Player_MeleeSplash");
							SpawnForced(inflictor_class, GetActorX(ox), GetActorY(ox), GetActorZ(ox) + GetActorProperty(ox, APROP_HEIGHT) / 2);
						}
					}

					SetActorProperty(0, APROP_STAMINA, arg1);
				}

				SetResultValue(dmg);
				HandleOnHitEffects(shooter);
			}
			else {
				// simply deal the extracted damage, nothing to be done -- support for non-mod monsters and objects
				SetResultValue(dmg);
				HandleOnHitEffects(shooter);
			}
		}
		else if(IsPlayer(shooter) && IsPlayer(victim) && shooter != victim) {
			// if victim is a player, and shooter is also a player and its not us, make sure they take no damage! YOU NEVER KNOW!!
			SetResultValue(0);
		}
		else if(IsPet(victim) && IsPlayer(shooter)) {
			// players shouldnt hurt pets
			SetResultValue(0);
		}
		else if(IsPet(shooter) && shooter != victim) {
			// shooter is pet, it most likely attacked a monster, factor in things related to pets and put damage numbers!
			// make sure activator is the player themselves now
			SetActivator(GetActorProperty(shooter, APROP_MASTERTID));
			ACS_NamedExecuteWithResult("DnD Damage Numbers", victim, dmg, 0);
			SetResultValue(dmg);
		}
		else if(IsMonster(victim) && dmg_data) {
			// last option, player hurt monster in here --- we normally don't handle this here but for reflection we can
			// if we have dmg_data, currently it can only come from monster projectile
			SetActivator(GetActorProperty(shooter, APROP_MASTERTID));
			ACS_NamedExecuteWithResult("DnD Damage Numbers", victim, dmg, 0);

			// failsafe
			if(GetActorProperty(victim, APROP_HEALTH) > MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp)
				SetActorProperty(victim, APROP_HEALTH, MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp);

			SetResultValue(dmg);
			Terminate;
		}
		else if(!IsMonster(victim)) {
			if(CheckActorInventory(victim, "DnD_CountdownProtection")) {
				SetResultValue(0);
				Terminate;
			}

			// exception for map related hazards
			if(arg2 == "Slime" || arg2 == "Crush" || arg2 == "Drowning" || arg2 == "Telefrag" || arg2 == "Suicide" || arg2 == "InstantDeath" || arg2 == "Exit" || arg2 == "Trap") {
				// apply eshield to these only
				if(arg2 == "Slime" || arg2 == "Crush" || arg2 == "Drowning" || arg2 == "Trap") {
					// scale these up by player level -- more if its a trap
					dmg = dmg * (100 + GetBasicMonsterDMGScaling(GetActorLevel(victim), arg2 == "Trap")) / 100;
					
					dmg = ApplyTrueDamageDeductions(pnum, dmg, arg2, 0);
				}
				if(!HasActorClassPerk_Fast(victim, DND_PLAYER_CYBORG, 2) || random(0, 1.0) <= DND_CYBORG_REGENCONTCHANCE)
					GiveActorInventory(victim, "DnD_Hit_CombatTimer", 1);
				SetResultValue(dmg);
				PlayerScriptsCheck[DND_SCRIPT_DAMAGETAKENTIC][pnum] = arg1;
				Terminate;
			}

			// the above check was necessary
			// hurt self -- handleplayerselfdamage is ran in explosion side of things, we run additional stuff that isnt handled by that here, like resists and armor
			pnum = PlayerNumber();

			if(dmg_data & DND_DAMAGEFLAG_ISRADIUSDMG) {
				SetActivator(0, AAPTR_DAMAGE_INFLICTOR);

				// save the percentage of damage from radius falloff into somewhere (arg1 base is 100, so we can use it as percentage)
				inflictor_class = arg1;
				arg1 = GetUserVariable(0, "user_expdmg");
				dmg_data |= GetUserVariable(0, "user_expflags");

				// not meant to be hurting self
				if(!(dmg_data & DND_DAMAGEFLAG_BLASTSELF)) {
					SetResultValue(0);
					Terminate;
				}
			}

			SetActivator(shooter);

			if(!(dmg_data & DND_DAMAGEFLAG_NONWEAPON)) {
				// wepid
				m_id = arg1 & ATK_WID_MASK;
				arg1 >>= ATK_CACHE_SHIFT;

				// dmg cache id
				dmg = arg1 & ATK_CACHE_MASK;
				arg1 >>= ATK_DTYPE_SHIFT;

				// dmg type
				temp = arg1 & ATK_DTYPE_MASK;
				arg1 >>= ATK_DPCT_SHIFT;

				// % adjustment factor
				factor = arg1 & ATK_DPCT_MASK;

				if(!(dmg_data & DND_DAMAGEFLAG_ISSPECIALAMMO))
					dmg = RetrieveWeaponDamage(pnum, m_id, dmg, GetDamageCategory(temp, dmg_data), dmg_data, 0);
				else {
					// special ammo correction
					isRipper = CheckInventory("DnD_WeaponID");
					dmg = RetrieveWeaponDamage(pnum, isRipper, dmg, GetDamageCategory(temp, dmg_data), dmg_data, m_id + 1);
					m_id = isRipper;
				}

				// setup the flags and factor
				if(factor != 100)
					dmg = dmg * factor / 100;
			}
			else {
				// non-weapon sources that aren't spells
				dmg = arg1 & NONWEP_DMG_MASK; // dmg
				temp = arg1 >> NONWEP_DMG_SHIFT; // dmg_type
				m_id = -1;
			}

			// restore ptr
			SetActivator(0, AAPTR_DAMAGE_TARGET);

			if(dmg_data & DND_DAMAGEFLAG_ISRADIUSDMG) {
				dmg = dmg * inflictor_class / 100;
				dmg = HandlePlayerSelfDamage(pnum, dmg, temp, m_id, dmg_data, false);
				if(!dmg) {
					SetResultValue(0);
					Terminate;
				}
			}

			// a third to self
			if(dmg_data & DND_DAMAGEFLAG_HALFDMGSELF)
				dmg /= 3;

			if(!HasActorClassPerk_Fast(victim, DND_PLAYER_CYBORG, 2) || random(0, 1.0) <= DND_CYBORG_REGENCONTCHANCE)
				GiveActorInventory(victim, "DnD_Hit_CombatTimer", 1);

			if(!CheckActorInventory(victim, "DnD_Hit_Cooldown")) {
				OnPlayerHit(victim, pnum, shooter, false);
				GiveActorInventory(victim, "DnD_Hit_Cooldown", 1);
			}

			dmg = HandlePlayerOnHitBuffs(victim, shooter, dmg, dmg_data, arg2);
			dmg = HandlePlayerResists(pnum, dmg, arg2, dmg_data, !!isReflected || (dmg_data & DND_DAMAGEFLAG_ISREFLECTED), inflictor_class);
			dmg = HandlePlayerArmor(pnum, dmg, arg2, dmg_data, false);
			dmg = ApplyTrueDamageDeductions(pnum, dmg, arg2, dmg_data);
			//GiveInventory("DnD_DamageReceived", dmg);
			PlayerScriptsCheck[DND_SCRIPT_DAMAGETAKENTIC][pnum] = dmg;
			IncrementStatistic(DND_STATISTIC_DAMAGETAKEN, dmg, victim);

			if(HasClassPerk_Fast(DND_PLAYER_MARINE, 4) && !CheckInventory("Marine_Perk50_Cooldown"))
				GiveInventory("Marine_Perk50_DamageTaken", dmg);

			if(!(dmg_data & DND_DAMAGEFLAG_NOPUSH))
				HandleDamagePush(dmg * 4, ox, oy, oz, shooter);
			SetResultValue(dmg);
		}
		else if(temp && !shooter)
			SetResultValue(0);
	}
	else if(type == GAMEEVENT_ROUND_ABORTED) {
		// Same reason as the unloading path: ResetUsedTIDs is about to hand monster ids out from 0
		// again, and a latch left set is inherited by whatever monster takes that id next round.
		// Must run BEFORE the reset -- the sweep is bounded by the id counters that clears.
		FlushDamageTicResidue();

		// and refresh the list the respawning monsters will clear themselves against
		BuildActivePlayerList();

		SetupUndo(SETUP_STATE1, SETUP_CLEANINGMONSTERTIDS);
		ResetUsedTIDs();
	}
}

Script "DnD Block Prevention Timer" (int monster_tid) {
	SetActivator(monster_tid);

	ACS_NamedExecuteWithResult("DnD Block Prevention FX");

	// remove blocking status
	ACS_NamedExecuteWithResult("DnD Monster Trait Take Single", monster_tid, DND_ISBLOCKING);
	ACS_NamedExecuteWithResult("DnD Monster Trait Give Single", monster_tid, DND_GUARDBROKEN);

	while(isAlive() && CheckInventory("DnD_AntiBlockCounter")) {
		TakeInventory("DnD_AntiBlockCounter", 1);
		Delay(const:TICRATE);
	}

	SetInventory("DnD_AntiBlockCounter", 0);
	ACS_NamedExecuteWithResult("DnD Monster Trait Take Single", monster_tid, DND_GUARDBROKEN);

	SetResultValue(0);
}

Script "DnD Block Prevention FX" (void) CLIENTSIDE {
	PlaySound(0, "Elite/FortifyCrack", CHAN_VOICE, 1.0);
	GiveInventory("ShieldPreventionFXSpawner", 1);

	SetResultValue(0);
}

Script "DnD Special Fx Spawner" (int victim, int id) CLIENTSIDE {
	str actor = "";
	int ang;
	int offset = 8.0 + GetActorProperty(victim, APROP_RADIUS);
	switch(id) {
		case INV_DEEPCUTS:
			actor = "DnD_DeepCutFX";
			ang = AngleToFace(victim, ConsolePlayerNumber() + P_TIDSTART);
		break;
		case INV_CRUSHINGBLOW:
			actor = "DnD_CrushingBlowFX";
			ang = AngleToFace(victim, ConsolePlayerNumber() + P_TIDSTART);
		break;
		case INV_OPENWOUNDS:
			actor = "DnD_OpenWoundsFX";
			ang = AngleToFace(victim, ConsolePlayerNumber() + P_TIDSTART);
		break;
		case INV_DEADLYSTRIKE:
			actor = "DnD_DeadlyStrikeFX";
			ang = AngleToFace(victim, ConsolePlayerNumber() + P_TIDSTART);
		break;
		case INV_REAPINGCLEAVE:
			actor = "DnD_ReapingCleaveFX";
			ang = GetActorAngle(victim);
			offset += 8.0;
		break;
	}

	if(id) {
		int c = cos(ang);
		int s = sin(ang);

		int h = GetActorProperty(victim, APROP_HEIGHT) / 2;

		SpawnForced(actor, GetActorX(victim) + FixedMul(offset, c), GetActorY(victim) + FixedMul(offset, s), GetActorZ(victim) + h, 0);
	}
}

#endif
