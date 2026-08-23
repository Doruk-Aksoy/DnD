#ifndef DND_INV_ATTRIBS_IN
#define DND_INV_ATTRIBS_IN

#include "DnD_InvInfo.h"
#include "DnD_ItemBase.h"
#include "../DnD_WeaponDefs.h"

// The base tier an item level grants, before GetItemTierRoll's +1/-N shuffle. This is the ONE place
// that conversion lives -- every site that assigns an attrib_tier goes through here (field drops,
// inventory adds, merchant stock) and GetHighestTierModNaturalOnItem derives the Potency ceiling from
// it. Two of those used to divide by CHARM_ATTRIBLEVEL_SEPERATOR by hand without the clamp, which let
// an ilvl 100 drop hand out a base tier of 10 while the same mod added later by an orb started at 9.
//
// MAX_ATTRIBUTE_TIERS (9) is the ceiling on purpose: it is the highest base an item level may grant.
// MAX_CHARM_AFFIXTIERS (10) is the absolute ceiling, and the last step onto it is meant to be earned
// -- the 10% up roll, or Orb of Potency.
int GetItemTier(int level) {
	int res = level / CHARM_ATTRIBLEVEL_SEPERATOR;
	if(res > MAX_ATTRIBUTE_TIERS)
		res = MAX_ATTRIBUTE_TIERS;
	return res;
}

#define DND_FLASK_RECOVERY_REDUCEEFFECT 25 // 25%

// formula for accuracy:
// every 10 points contribute 0.01%
// applied as follows: X = spread of weapon, X * (1.0 - accuracy * factor_per_point), capped at 100 000 accuracy (100%)

#define DND_MAX_ATTRIBUTE_SYNC_ELEMS 64
typedef struct {
	int count;
	int arr[DND_MAX_ATTRIBUTE_SYNC_ELEMS]; // 64 attributes should be enough

	int extras;
	int arr_extra[MAX_ITEM_IMPLICITS];
} attrib_sync_T;

attrib_sync_T module& GetPlayerAttributeSyncs(int pnum) {
	static attrib_sync_T PlayerAttributeSyncs[MAXPLAYERS];
	return PlayerAttributeSyncs[pnum];
}

bool IsAttributeQueuedForSync(int pnum, int attrib_id) {
	auto psync = GetPlayerAttributeSyncs(pnum);
	int amt = psync.count;
	for(int i = 0; i < amt; ++i)
		if(psync.arr[i] == attrib_id)
			return true;
	return false;
}

bool IsAttributeExtraQueuedForSync(int pnum, int attrib_id) {
	auto psync = GetPlayerAttributeSyncs(pnum);
	int amt = psync.extras;
	for(int i = 0; i < amt; ++i)
		if(psync.arr_extra[i] == attrib_id)
			return true;
	return false;
}

void PushToPlayerAttributeSync(int pnum, int attrib_id) {
	auto psync = GetPlayerAttributeSyncs(pnum);
	if(psync.count < DND_MAX_ATTRIBUTE_SYNC_ELEMS && !IsAttributeQueuedForSync(pnum, attrib_id))
		psync.arr[psync.count++] = attrib_id;
}

void PushToPlayerAttributeExtraSync(int pnum, int attrib_id) {
	auto psync = GetPlayerAttributeSyncs(pnum);
	if(psync.extras < DND_MAX_ATTRIBUTE_SYNC_ELEMS && !IsAttributeExtraQueuedForSync(pnum, attrib_id))
		psync.arr_extra[psync.extras++] = attrib_id;
}

void ClearPlayerAttributeSync(int pnum) {
	auto psync = GetPlayerAttributeSyncs(pnum);
	int amt = psync.count;
	for(int i = 0; i < amt; ++i)
		psync.arr[i] = -1;
	psync.count = 0;
}

void ClearPlayerAttributeExtraSync(int pnum) {
	auto psync = GetPlayerAttributeSyncs(pnum);
	int amt = psync.extras;
	for(int i = 0; i < amt; ++i)
		psync.arr_extra[i] = -1;
	psync.extras = 0;
}

//#define MAX_ATTRIB_MODIFIER 0xFFFFFFFF

#define UNSTABLE_DMG_MULT 5 // x5 of chance as damage
#define UNSTABLE_PROC_PCT 0.05 // 5%

#define DND_INC_ACCURACYFORPRECRATIO 25
#define DND_INC_BLOCKPREVENTIONTIME 10

#define DND_INC_SINGLEPROJ_NEGDMG 0.15
#define DND_INC_TWOPROJ_NEGDMG 0.3

#define DND_INC_POISONSPREAD_R 160.0
#define DND_INC_POISONSPREAD_COUNT 8

// The PPOWER_* bitmask enum that used to live here is gone. Its four powers are PFLAG_CYBER,
// PFLAG_ESHIELDBLOCKALL, PFLAG_MELEEIGNORESHIELD and PFLAG_LOWERREFLECT in DnD_Common.h, and they
// are INDICES now, not masks. Deleted rather than left as an alias: the two are the same shape and
// the same order, so a leftover PPOWER_ argument would have compiled happily and tested the wrong
// bit -- PPOWER_MELEEIGNORESHIELD is 16, which as an index is a flag that does not exist.

bool IsSpecialRollRuleAttribute(int id) {
	switch(id) {
		case INV_IMP_CANROLL_PHYS:
		case INV_IMP_CANROLL_MAGIC:
		case INV_IMP_CANROLL_EXPLOSIVE:
		case INV_IMP_CANROLL_ENERGY:
		case INV_IMP_CANROLL_ELEMENTAL:
		return true;
	}
	return false;
}

#include "DnD_InvAttribTable.h"
#include "DnD_ItemModPool.h"

enum {
	INV_ATTR_TAG_NONE,
	INV_ATTR_TAG_DAMAGE = 1,
	INV_ATTR_TAG_ATTACK = 2,
	INV_ATTR_TAG_LIFE = 4,
	INV_ATTR_TAG_DEFENSE = 8,
	INV_ATTR_TAG_UTILITY = 16,
	INV_ATTR_TAG_ELEMENTAL = 32,
	INV_ATTR_TAG_EXPLOSIVE = 64,
	INV_ATTR_TAG_OCCULT = 128,
	INV_ATTR_TAG_CRIT = 256,
	INV_ATTR_TAG_STAT = 512,
	INV_ATTR_TAG_PHYSICAL = 1024,
	INV_ATTR_TAG_ENERGY = 2048,
	INV_ATTR_TAG_MELEE = 4096,
	INV_ATTR_TAG_FIRE = 8192,
	INV_ATTR_TAG_ICE = 16384,
	INV_ATTR_TAG_POISON = 32768,
	INV_ATTR_TAG_LIGHTNING = 65536,
	INV_ATTR_TAG_STAMINA = 131072,
	INV_ATTR_TAG_FLASK = 262144,
	INV_ATTR_TAG_ARMOR = 524288,
	INV_ATTR_TAG_ESHIELD = 1048576,
	INV_ATTR_TAG_MITIGATION = 2097152,
	INV_ATTR_TAG_AILMENT = 4194304,

	// shorthands
	INV_ATTR_TAG_ALL = 	INV_ATTR_TAG_AILMENT | (INV_ATTR_TAG_AILMENT - 1),

	// bodyarmors
	INV_ATTR_TAG_OK_BODYARMOR = INV_ATTR_TAG_LIFE | INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_OCCULT | INV_ATTR_TAG_STAT | 
								INV_ATTR_TAG_PHYSICAL | INV_ATTR_TAG_ENERGY | INV_ATTR_TAG_FIRE | INV_ATTR_TAG_ICE | INV_ATTR_TAG_POISON |
								INV_ATTR_TAG_LIGHTNING | INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_AILMENT | INV_ATTR_TAG_ARMOR | INV_ATTR_TAG_ESHIELD | INV_ATTR_TAG_MITIGATION,

	INV_ATTR_TAG_EXCLUDED_BODYARMOR = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_CRIT | INV_ATTR_TAG_MELEE | INV_ATTR_TAG_FLASK,

	// helms
	INV_ATTR_TAG_OK_HELM = 	INV_ATTR_TAG_LIFE | INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_OCCULT | INV_ATTR_TAG_STAT | 
							INV_ATTR_TAG_PHYSICAL | INV_ATTR_TAG_ENERGY | INV_ATTR_TAG_FIRE | INV_ATTR_TAG_ICE | INV_ATTR_TAG_POISON |
							INV_ATTR_TAG_LIGHTNING | INV_ATTR_TAG_AILMENT | INV_ATTR_TAG_ARMOR | INV_ATTR_TAG_ESHIELD | INV_ATTR_TAG_MITIGATION |
							INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_CRIT,

	INV_ATTR_TAG_EXCLUDED_HELM = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_FLASK | INV_ATTR_TAG_AILMENT,

	// boots
	INV_ATTR_TAG_OK_BOOT = 	INV_ATTR_TAG_LIFE | INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_OCCULT | INV_ATTR_TAG_STAT | 
							INV_ATTR_TAG_PHYSICAL | INV_ATTR_TAG_ENERGY | INV_ATTR_TAG_FIRE | INV_ATTR_TAG_ICE | INV_ATTR_TAG_POISON |
							INV_ATTR_TAG_LIGHTNING | INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_MELEE | INV_ATTR_TAG_STAMINA | INV_ATTR_TAG_AILMENT |
							INV_ATTR_TAG_ARMOR | INV_ATTR_TAG_ESHIELD | INV_ATTR_TAG_MITIGATION,

	INV_ATTR_TAG_EXCLUDED_BOOT = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_CRIT | INV_ATTR_TAG_FLASK,
};

enum {
	INV_ATTR_TAG_DAMAGE_ID,
	INV_ATTR_TAG_ATTACK_ID,
	INV_ATTR_TAG_LIFE_ID,
	INV_ATTR_TAG_DEFENSE_ID,
	INV_ATTR_TAG_UTILITY_ID,
	INV_ATTR_TAG_ELEMENTAL_ID,
	INV_ATTR_TAG_EXPLOSIVE_ID,
	INV_ATTR_TAG_OCCULT_ID,
	INV_ATTR_TAG_CRIT_ID,
	INV_ATTR_TAG_STAT_ID,
	INV_ATTR_TAG_PHYSICAL_ID,
	INV_ATTR_TAG_ENERGY_ID,
	INV_ATTR_TAG_MELEE_ID,
	INV_ATTR_TAG_FIRE_ID,
	INV_ATTR_TAG_ICE_ID,
	INV_ATTR_TAG_POISON_ID,
	INV_ATTR_TAG_LIGHTNING_ID,
	INV_ATTR_TAG_STAMINA_ID,
	INV_ATTR_TAG_FLASK_ID,
	INV_ATTR_TAG_ARMOR_ID,
	INV_ATTR_TAG_ESHIELD_ID,
	INV_ATTR_TAG_MITIGATION_ID,
	INV_ATTR_TAG_AILMENT_ID,
};
// Which tag group a "can roll X" implicit opens, or -1 when the implicit is not a widening at all.
// Returning the tag ID rather than the implicit means a new widening is one line here plus the
// implicit itself -- nothing in the pool key or the pool builder has to know it exists.
int GetWideningTagId(int implicit_id) {
	switch(implicit_id) {
		case INV_IMP_CANROLL_PHYS:		return INV_ATTR_TAG_PHYSICAL_ID;
		case INV_IMP_CANROLL_MAGIC:		return INV_ATTR_TAG_OCCULT_ID;
		case INV_IMP_CANROLL_EXPLOSIVE:	return INV_ATTR_TAG_EXPLOSIVE_ID;
		case INV_IMP_CANROLL_ENERGY:	return INV_ATTR_TAG_ENERGY_ID;
		case INV_IMP_CANROLL_ELEMENTAL:	return INV_ATTR_TAG_ELEMENTAL_ID;
	}
	return DND_MODPOOL_NO_TAG;
}

#define DND_ATTRIB_TAG_ID_BEGIN INV_ATTR_TAG_DAMAGE_ID
#define DND_ATTRIB_TAG_ID_END INV_ATTR_TAG_LIGHTNING_ID
#define MAX_ATTRIB_TAG_GROUPS (DND_ATTRIB_TAG_ID_END + 1)


// Mods whose attribute value is the STRONGEST single source rather than the sum of all of them.
// Indexed densely rather than by attribute id: the storage below is a row of equipment slots per
// entry, and a MAX_TOTAL_ATTRIBUTES tall table of those would be a lot of global for two mods.
enum {
	DND_HIGHESTSRC_MOREHPBONUS,
	DND_HIGHESTSRC_CRITFORDOT,

	DND_HIGHESTSRC_COUNT
};

// One column per equipment slot. Every ApplyItemFeatures call passes DND_SYNC_ITEMSOURCE_ITEMSUSED,
// so ProcessAttribute's item_index IS the slot, which is what these rows are keyed on.
//
// Sized against MAX_ITEMS_EQUIPPABLE, which is 13 and lives in DnD_Inventory.h -- included after
// this header, so the number cannot be referenced here. Deliberately a few over: a spare entry costs
// nothing, and the bounds check in SetHighestModSource turns a slot count that outgrows this into a
// dropped source rather than a write past the end of the struct.
#define DND_MAX_HIGHESTSRC_SLOTS 16

// The two MAX_TOTAL_ATTRIBUTES arrays that used to head this struct are GONE. They were 3000 slots
// each per player -- 6000 ints for 360 attribute ids, of which only ~315 ever held a player scoped
// value and only 13 ever held an extra. Everything now lives in the dense members below, addressed by
// slot rather than by attribute id.
//
// Removing them was gated on one property, checked mechanically rather than assumed: EVERY attribute
// that anything reads is mapped by MapAttributeToPStat, MapAttributeToPFlag or MapAttributeToPExtra.
// Three mods are deliberately unmapped -- INV_CRITPERCENT_FORWEPTYPE, INV_INC_PLUSPROJ and
// INV_INC_PLUSTWOPROJ -- because they have no player scope storage at all: they fan into
// Player_Weapon_Infos and are read from there. Writes for an unmapped mod are now dropped on the
// floor, which is correct only while that property holds, so re-check it before adding a read.
typedef struct {
	// The DND_HIGHESTSRC_* mods take their strongest source rather than adding them up, so the
	// individual contributions are kept here, indexed by the slot they came from. A running total
	// cannot do it: dropping the strongest one has to fall back to the second strongest, and a sum
	// has already forgotten what the parts were.
	int highest_sources[DND_HIGHESTSRC_COUNT][DND_MAX_HIGHESTSRC_SLOTS];

	// Dense per-stat storage, one slot per STAT rather than per attribute id. See the long note on
	// PSTAT_* in DnD_Common.h. This is the whole player scope value side now.
	int f[PSTAT_COUNT];

	// Second number for the 13 mods that carry one. See MapAttributeToPExtra.
	int x[DND_PEXTRA_COUNT];

	// Boolean mods, one bit each, plus the packed source counts that make removal correct.
	int pflags[DND_PFLAG_WORDS];
	int pflag_rc[DND_PFLAG_RCWORDS];
} player_item_mod_data_T;

global player_item_mod_data_T 57: PlayerModData[MAXPLAYERS];

// The ONE place an attribute id becomes a stat slot. Everything not listed here still lives in
// value[] and is looked up by id, which is what makes the migration incremental: a family moves by
// gaining cases here and having its read sites rewritten to touch f[] directly, and nothing else in
// the mod has to know it moved.
//
// Two mods may share a slot when they are a new source of the same stat -- that is the point of the
// design. Two rules bound it:
//   - Never share a slot between mods IsMoreMultiplierMod says are multiplicative. The cancel path
//     in IncPlayerModValue is only exact while one mod owns the accumulator; two of them in one slot
//     cannot be unequipped independently.
//   - Never share a slot between mods with different conditions. Two unconditional accuracy mods
//     share; one that only applies to a weapon type does not.
// Thirty mods are DELIBERATELY absent from this switch. Each one has no player-scope value of its
// own -- their ProcessAttribute / ProcessItemImplicit cases either write into OTHER attributes
// (INV_IMP_INCARMOR feeds INV_ARMOR_INCREASE, INV_EX_ALLSTATS feeds the three stats), or into
// Player_Weapon_Infos (the INV_CORR_WEAPON* family), or into the conversion tables, or do nothing
// at all because they are item-generation markers read off item data (INV_CYBERNETIC, the
// INV_IMP_CANROLL_* set, INV_IMP_QUALITYCAPFIFTY).
//
// Giving them slots would be harmless but dishonest: the slot could never be non-zero, and it
// would imply storage that does not exist. Unmapped reads return 0, which is the same answer the
// old value[] array gave for them, so nothing changes behaviourally either way.
// Some of them DO carry an extra -- INV_IMP_UNSTABLECORE for one -- which lives in
// MapAttributeToPExtra and is unaffected by their absence here.
int MapAttributeToPStat(int mod) {
	switch(mod) {
		// Flat added damage. The slot IS the damage category it is dealt as -- physical resolves to
		// BULLET because an added component is never the weapon's own swing.
		case INV_ADDED_PHYSDMG:			return PSTAT_ADDEDFLAT_BASE + DND_DAMAGECATEGORY_BULLET;
		case INV_ADDED_ENERGYDMG:		return PSTAT_ADDEDFLAT_BASE + DND_DAMAGECATEGORY_ENERGY;
		case INV_ADDED_MAGICDMG:		return PSTAT_ADDEDFLAT_BASE + DND_DAMAGECATEGORY_OCCULT;
		case INV_ADDED_FIREDMG:			return PSTAT_ADDEDFLAT_BASE + DND_DAMAGECATEGORY_FIRE;
		case INV_ADDED_COLDDMG:			return PSTAT_ADDEDFLAT_BASE + DND_DAMAGECATEGORY_ICE;
		case INV_ADDED_LIGHTNINGDMG:	return PSTAT_ADDEDFLAT_BASE + DND_DAMAGECATEGORY_LIGHTNING;
		case INV_ADDED_POISONDMG:		return PSTAT_ADDEDFLAT_BASE + DND_DAMAGECATEGORY_POISON;

		// Player resistances. REFL rides in the same run because it is passed through the same
		// parameter as the rest, even though nothing rolls it as a damage type.
		case INV_DMGREDUCE_PHYS:		return PSTAT_RESIST_BASE + DND_PRESIST_PHYS;
		case INV_DMGREDUCE_MAGIC:		return PSTAT_RESIST_BASE + DND_PRESIST_MAGIC;
		case INV_DMGREDUCE_ENERGY:		return PSTAT_RESIST_BASE + DND_PRESIST_ENERGY;
		case INV_DMGREDUCE_ELEM:		return PSTAT_RESIST_BASE + DND_PRESIST_ELEM;
		case INV_DMGREDUCE_REFL:		return PSTAT_RESIST_BASE + DND_PRESIST_REFL;
		case INV_DMGREDUCE_ALL:			return PSTAT_RESIST_ALL;

		// Resist penetration.
		case INV_PEN_PHYSICAL:			return PSTAT_PEN_BASE + DND_PPEN_PHYS;
		case INV_PEN_ENERGY:			return PSTAT_PEN_BASE + DND_PPEN_ENERGY;
		case INV_PEN_OCCULT:			return PSTAT_PEN_BASE + DND_PPEN_OCCULT;
		case INV_PEN_FIRE:				return PSTAT_PEN_BASE + DND_PPEN_FIRE;
		case INV_PEN_ICE:				return PSTAT_PEN_BASE + DND_PPEN_ICE;
		case INV_PEN_LIGHTNING:			return PSTAT_PEN_BASE + DND_PPEN_LIGHTNING;
		case INV_PEN_POISON:			return PSTAT_PEN_BASE + DND_PPEN_POISON;
		case INV_PEN_ELEMENTAL:			return PSTAT_PEN_BASE + DND_PPEN_ELEMENTAL;

		// A corruption implicit that pierces everything. A second SOURCE of the same stat rather than
		// a stat of its own, which is exactly the case this design exists to make free -- it shares
		// the slot and no formula downstream knows it arrived.
		case INV_CORR_ALLPIERCE:		return PSTAT_PEN_BASE + DND_PPEN_ALL;

		// Ailment avoidance.
		case INV_AVOID_IGNITE:			return PSTAT_AVOID_BASE + DND_PAVOID_IGNITE;
		case INV_AVOID_CHILLFREEZE:		return PSTAT_AVOID_BASE + DND_PAVOID_CHILLFREEZE;
		case INV_AVOID_POISON:			return PSTAT_AVOID_BASE + DND_PAVOID_POISON;
		case INV_AVOID_OVERLOAD:		return PSTAT_AVOID_BASE + DND_PAVOID_OVERLOAD;
		case INV_AVOID_BLEED:			return PSTAT_AVOID_BASE + DND_PAVOID_BLEED;
		case INV_AVOID_ELEAILMENTS:		return PSTAT_AVOID_ELEALL;

		// Ignite.
		case INV_IGNITECHANCE:			return PSTAT_IGN_CHANCE_PCT;
		case INV_CHANCE_FLATIGNITE:		return PSTAT_IGN_CHANCE_FLAT;
		case INV_IGNITEDMG:				return PSTAT_IGN_DMG;
		case INV_IGNITEDURATION:		return PSTAT_IGN_DURATION;
		case INV_IGNITE_PROLIFCHANCE:	return PSTAT_IGN_PROLIF_CHANCE_PCT;
		case INV_CHANCE_FLATPROLIF:		return PSTAT_IGN_PROLIF_CHANCE_FLAT;
		case INV_IGNITE_PROLIFCOUNT:	return PSTAT_IGN_PROLIF_COUNT;
		case INV_IGNITE_PROLIFRANGE:	return PSTAT_IGN_PROLIF_RANGE;

		// Poison.
		case INV_POISON_TICRATE:		return PSTAT_POIS_TICRATE;
		case INV_POISON_DURATION:		return PSTAT_POIS_DURATION;
		case INV_POISON_TICDMG:			return PSTAT_POIS_TICDMG;

		// Bleed.
		case INV_CHANCE_BLEED:			return PSTAT_BLEED_CHANCE;
		case INV_PERCENTDMG_BLEED:		return PSTAT_BLEED_DMG_PCT;
		case INV_BLEED_DURATION:		return PSTAT_BLEED_DURATION;

		// Chill and freeze.
		case INV_FREEZECHANCE:			return PSTAT_FREEZE_CHANCE;
		case INV_CHILLTHRESHOLD:		return PSTAT_CHILL_THRESHOLD;

		// Overload. INV_OVERLOAD_DMGINCREASE is an IsMoreMultiplierMod, so its slot must stay its
		// own -- the cancel path in IncPlayerModValue is only exact with one mod per accumulator.
		case INV_OVERLOAD_ZAPCOUNT:		return PSTAT_OVERLOAD_ZAPCOUNT;
		case INV_OVERLOAD_DMGINCREASE:	return PSTAT_OVERLOAD_DMGINCREASE;
		case INV_OVERLOAD_DURATION:		return PSTAT_OVERLOAD_DURATION;

		// Cross-ailment broadcasts.
		case INV_INCREASEDDOT:			return PSTAT_DOT_INCREASED;
		case INV_EX_FLATDOT:			return PSTAT_DOT_FLAT;
		case INV_EX_DOTDURATION:		return PSTAT_DOT_DURATION;
		case INV_CHANCE_AILMENTIGNORE:	return PSTAT_AILMENT_IGNORECHANCE;

		// Defense.
		case INV_HP_INCREASE:			return PSTAT_HP_FLAT;
		case INV_HPPERCENT_INCREASE:	return PSTAT_HP_PCT;
		case INV_ARMOR_INCREASE:		return PSTAT_ARMOR_FLAT;
		case INV_ARMORPERCENT_INCREASE:	return PSTAT_ARMOR_PCT;
		case INV_SHIELD_INCREASE:		return PSTAT_SHIELD_FLAT;
		case INV_PERCENTSHIELD_INCREASE:return PSTAT_SHIELD_PCT;
		case INV_SHIELD_RECHARGEDELAY:	return PSTAT_SHIELD_RECHARGEDELAY;
		case INV_SHIELD_RECOVERYRATE:	return PSTAT_SHIELD_RECOVERYRATE;
		case INV_MIT_INCREASE:			return PSTAT_MIT_CHANCE;
		case INV_MITEFFECT_INCREASE:	return PSTAT_MIT_EFFECT;
		case INV_ADDEDMAXRESIST:		return PSTAT_MAXRESIST_ADDED;
		case INV_SELFDMG_RESIST:		return PSTAT_SELFDMG_RESIST;
		case INV_MAGIC_NEGATION:		return PSTAT_MAGIC_NEGATION;
		case INV_INC_TWICEARMORDEFENSE:	return PSTAT_ARMOR_DOUBLEDEF;

		// Flat damage bonuses, one slot per damage category.
		case INV_FLATPHYS_DAMAGE:		return PSTAT_FLATDMG_BASE + DND_DAMAGECATEGORY_BULLET;
		case INV_FLATMAGIC_DAMAGE:		return PSTAT_FLATDMG_BASE + DND_DAMAGECATEGORY_OCCULT;
		case INV_FLATENERGY_DAMAGE:		return PSTAT_FLATDMG_BASE + DND_DAMAGECATEGORY_ENERGY;
		case INV_FLAT_FIREDMG:			return PSTAT_FLATDMG_BASE + DND_DAMAGECATEGORY_FIRE;
		case INV_FLAT_ICEDMG:			return PSTAT_FLATDMG_BASE + DND_DAMAGECATEGORY_ICE;
		case INV_FLAT_LIGHTNINGDMG:		return PSTAT_FLATDMG_BASE + DND_DAMAGECATEGORY_LIGHTNING;
		case INV_FLAT_POISONDMG:		return PSTAT_FLATDMG_BASE + DND_DAMAGECATEGORY_POISON;
		case INV_FLATELEM_DAMAGE:		return PSTAT_FLATDMG_ELEM;
		case INV_FLATRADIUS_DAMAGE:		return PSTAT_FLATDMG_RADIUS;

		// Percent damage bonuses, same layout.
		case INV_PERCENTPHYS_DAMAGE:	return PSTAT_PCTDMG_BASE + DND_DAMAGECATEGORY_BULLET;
		case INV_PERCENTMAGIC_DAMAGE:	return PSTAT_PCTDMG_BASE + DND_DAMAGECATEGORY_OCCULT;
		case INV_PERCENTENERGY_DAMAGE:	return PSTAT_PCTDMG_BASE + DND_DAMAGECATEGORY_ENERGY;
		case INV_PERCENTFIRE_DAMAGE:	return PSTAT_PCTDMG_BASE + DND_DAMAGECATEGORY_FIRE;
		case INV_PERCENTICE_DAMAGE:		return PSTAT_PCTDMG_BASE + DND_DAMAGECATEGORY_ICE;
		case INV_PERCENTLIGHTNING_DAMAGE:return PSTAT_PCTDMG_BASE + DND_DAMAGECATEGORY_LIGHTNING;
		case INV_PERCENTPOISON_DAMAGE:	return PSTAT_PCTDMG_BASE + DND_DAMAGECATEGORY_POISON;
		case INV_PERCENTELEM_DAMAGE:	return PSTAT_PCTDMG_ELEM;
		case INV_PERCENTRADIUS_DAMAGE:	return PSTAT_PCTDMG_RADIUS;

		// Crit, sustain and misc scalars. Six of these are IsMoreMultiplierMod, and each therefore
		// owns its slot outright -- the cancel path is only exact with one mod per accumulator.
		case INV_CRITCHANCE_INCREASE:         return PSTAT_CRITCHANCE_INCREASE;
		case INV_CRITDAMAGE_INCREASE:         return PSTAT_CRITDAMAGE_INCREASE;
		case INV_CRITPERCENT_INCREASE:        return PSTAT_CRITPERCENT_INCREASE;
		case INV_EX_CRITIGNORERESCHANCE:      return PSTAT_EX_CRITIGNORERESCHANCE;
		case INV_EX_MORECRIT_LIGHTNING:       return PSTAT_EX_MORECRIT_LIGHTNING;
		case INV_EX_SWAPFROMMELEECRIT:        return PSTAT_EX_SWAPFROMMELEECRIT;
		case INV_IMP_PRECISIONCRITBONUS:      return PSTAT_IMP_PRECISIONCRITBONUS;
		case INV_INC_CRITFORDOT:              return PSTAT_INC_CRITFORDOT;
		case INV_INC_EXCESSCRIT:              return PSTAT_INC_EXCESSCRIT;
		case INV_MELEECRIT_NOTONLOWSTAMINA:   return PSTAT_MELEECRIT_NOTONLOWSTAMINA;
		case INV_LIFESTEAL:                   return PSTAT_LIFESTEAL;
		case INV_LIFESTEAL_CAP:               return PSTAT_LIFESTEAL_CAP;
		case INV_LIFESTEAL_DAMAGE:            return PSTAT_LIFESTEAL_DAMAGE;
		case INV_LIFESTEAL_RATE:              return PSTAT_LIFESTEAL_RATE;
		case INV_LIFESTEAL_RECOVERY:          return PSTAT_LIFESTEAL_RECOVERY;
		case INV_INC_INSTANTLIFESTEAL:        return PSTAT_INC_INSTANTLIFESTEAL;
		case INV_LUCK_INCREASE:               return PSTAT_LUCK_INCREASE;
		case INV_ACCURACY_INCREASE:           return PSTAT_ACCURACY_INCREASE;
		case INV_INC_ACCURACYFORPRECISION:    return PSTAT_INC_ACCURACYFORPRECISION;
		case INV_CREDITGAIN_INCREASE:         return PSTAT_CREDITGAIN_INCREASE;
		case INV_EXPGAIN_INCREASE:            return PSTAT_EXPGAIN_INCREASE;
		case INV_SPEED_INCREASE:              return PSTAT_SPEED_INCREASE;
		case INV_PROJSPEED:                   return PSTAT_PROJSPEED;
		case INV_KNOCKBACK_RESIST:            return PSTAT_KNOCKBACK_RESIST;
		case INV_PERCENT_KNOCKBACKRESIST:     return PSTAT_PERCENT_KNOCKBACKRESIST;

		// Ammo and projectiles.
		case INV_AMMOCAP_INCREASE:              return PSTAT_AMMOCAP_INCREASE;
		case INV_AMMOGAIN_CHANCE:               return PSTAT_AMMOGAIN_CHANCE;
		case INV_AMMOGAIN_INCREASE:             return PSTAT_AMMOGAIN_INCREASE;
		case INV_EX_AMMOCOSTMULTIPLIER:         return PSTAT_EX_AMMOCOSTMULTIPLIER;
		case INV_EX_CANNOTPICKAMMO:             return PSTAT_EX_CANNOTPICKAMMO;
		case INV_EX_MOREAMMOUSE:                return PSTAT_EX_MOREAMMOUSE;
		case INV_EX_REDUCEDAMMOCAP:             return PSTAT_EX_REDUCEDAMMOCAP;
		case INV_EX_REFILLAMMOONMELEEKILL:      return PSTAT_EX_REFILLAMMOONMELEEKILL;
		case INV_EX_SOULPICKUPSINFAMMO:         return PSTAT_EX_SOULPICKUPSINFAMMO;
		case INV_EX_SOULPICKUPSONLYAMMO:        return PSTAT_EX_SOULPICKUPSONLYAMMO;
		case INV_IMP_AMMOGAIN_SHOTGUNS:         return PSTAT_IMP_AMMOGAIN_SHOTGUNS;
		case INV_INC_PROJREVERSE:               return PSTAT_INC_PROJREVERSE;
		case INV_PELLET_INCREASE:               return PSTAT_PELLET_INCREASE;
		case INV_EX_PICKUPS_MORESOUL:           return PSTAT_EX_PICKUPS_MORESOUL;

		// corruption implicits
		case INV_CORR_PERCENTSTATS:                 return PSTAT_CORR_PERCENTSTATS;
		case INV_CORR_CYBERNETIC:                   return PSTAT_CORR_CYBERNETIC;
		case INV_CORR_DMGDOESNTSTOPREGEN:           return PSTAT_CORR_DMGDOESNTSTOPREGEN;
		case INV_CORR_INSTALEECHPCT:                return PSTAT_CORR_INSTALEECHPCT;
		case INV_CORR_MOREAOE:                      return PSTAT_CORR_MOREAOE;
		case INV_CORR_MAXFRENZY:                    return PSTAT_CORR_MAXFRENZY;
		case INV_CORR_MAXENDURANCE:                 return PSTAT_CORR_MAXENDURANCE;
		case INV_CORR_MAXPOWER:                     return PSTAT_CORR_MAXPOWER;

		// regular implicits
		case INV_IMP_RAVAGER:                       return PSTAT_IMP_RAVAGER;
		case INV_IMP_ABSORBLIGHTNING:               return PSTAT_IMP_ABSORBLIGHTNING;
		case INV_IMP_KNIGHTARMOR:                   return PSTAT_IMP_KNIGHTARMOR;
		case INV_IMP_HANDGUNBONUS:                  return PSTAT_IMP_HANDGUNBONUS;
		case INV_IMP_NECROARMOR:                    return PSTAT_IMP_NECROARMOR;
		case INV_IMP_LESSLIGHTNINGTAKEN:            return PSTAT_IMP_LESSLIGHTNINGTAKEN;
		case INV_IMP_FASTEROVERHEATDISS:            return PSTAT_IMP_FASTEROVERHEATDISS;
		case INV_IMP_LESSPOISONTAKEN:               return PSTAT_IMP_LESSPOISONTAKEN;
		case INV_IMP_LESSFIRETAKEN:                 return PSTAT_IMP_LESSFIRETAKEN;
		case INV_IMP_LESSSELFDAMAGETAKEN:           return PSTAT_IMP_LESSSELFDAMAGETAKEN;
		case INV_IMP_BONUSPETCAP:                   return PSTAT_IMP_BONUSPETCAP;
		case INV_IMP_REDUCEDVISIONIMPAIR:           return PSTAT_IMP_REDUCEDVISIONIMPAIR;
		case INV_IMP_MOREDAMAGETOBOSSES:            return PSTAT_IMP_MOREDAMAGETOBOSSES;
		case INV_IMP_RECOVERESONUNDEADKILL:         return PSTAT_IMP_RECOVERESONUNDEADKILL;
		case INV_IMP_PERCENTSTR:                    return PSTAT_IMP_PERCENTSTR;
		case INV_IMP_PERCENTDEX:                    return PSTAT_IMP_PERCENTDEX;
		case INV_IMP_PERCENTINT:                    return PSTAT_IMP_PERCENTINT;
		case INV_IMP_REDUCEDSLOWSHOTGUNS:           return PSTAT_IMP_REDUCEDSLOWSHOTGUNS;
		case INV_IMP_ONKILL_FRENZY:                 return PSTAT_IMP_ONKILL_FRENZY;
		case INV_IMP_ONKILL_ENDURANCE:              return PSTAT_IMP_ONKILL_ENDURANCE;
		case INV_IMP_ONKILL_POWER:                  return PSTAT_IMP_ONKILL_POWER;
		case INV_IMP_PHASINGTIME:                   return PSTAT_IMP_PHASINGTIME;
		case INV_IMP_STAMINAONKILL:                 return PSTAT_IMP_STAMINAONKILL;

		// essences
		case INV_ESS_VAAJ:                          return PSTAT_ESS_VAAJ;
		case INV_ESS_SSRATH:                        return PSTAT_ESS_SSRATH;
		case INV_ESS_OMNISIGHT:                     return PSTAT_ESS_OMNISIGHT;
		case INV_ESS_OMNISIGHT2:                    return PSTAT_ESS_OMNISIGHT2;
		case INV_ESS_CHEGOVAX:                      return PSTAT_ESS_CHEGOVAX;
		case INV_ESS_HARKIMONDE:                    return PSTAT_ESS_HARKIMONDE;
		case INV_ESS_LESHRAC:                       return PSTAT_ESS_LESHRAC;
		case INV_ESS_KRULL:                         return PSTAT_ESS_KRULL;
		case INV_ESS_THORAX:                        return PSTAT_ESS_THORAX;
		case INV_ESS_ZRAVOG:                        return PSTAT_ESS_ZRAVOG;
		case INV_ESS_ERYXIA:                        return PSTAT_ESS_ERYXIA;

		// incursion mods
		case INV_INC_STAMINA:                       return PSTAT_INC_STAMINA;
		case INV_INC_STAMINARECOVERYRATE:           return PSTAT_INC_STAMINARECOVERYRATE;
		case INV_INC_STAMINAGAINED:                 return PSTAT_INC_STAMINAGAINED;
		case INV_INC_MOREHPBONUS:                   return PSTAT_INC_MOREHPBONUS;
		case INV_INC_MAXPOISONSTACK:                return PSTAT_INC_MAXPOISONSTACK;
		case INV_INC_POISONSPREAD:                  return PSTAT_INC_POISONSPREAD;
		case INV_INC_ALLOVERLOAD:                   return PSTAT_INC_ALLOVERLOAD;
		case INV_INC_HPREGENINTERRUPT:              return PSTAT_INC_HPREGENINTERRUPT;
		case INV_INC_PASSIVEREGEN:                  return PSTAT_INC_PASSIVEREGEN;
		case INV_INC_ENEMYRIPCHANCE:                return PSTAT_INC_ENEMYRIPCHANCE;
		case INV_INC_BLOCKPREVENTION:               return PSTAT_INC_BLOCKPREVENTION;
		case INV_INC_RIPPERSEXPLODE:                return PSTAT_INC_RIPPERSEXPLODE;
		case INV_INC_INVERTRESISTANCES:             return PSTAT_INC_INVERTRESISTANCES;

		// unique mods
		case INV_EX_CHANCE_CASTELEMSPELLONATK:      return PSTAT_EX_CHANCE_CASTELEMSPELLONATK;
		case INV_EX_FACTOR_SMALLCHARM:              return PSTAT_EX_FACTOR_SMALLCHARM;
		case INV_EX_CHANCE_HEALMISSINGONPAIN:       return PSTAT_EX_CHANCE_HEALMISSINGONPAIN;
		case INV_EX_DMGINCREASE_LIGHTNING:          return PSTAT_EX_DMGINCREASE_LIGHTNING;
		case INV_EX_SECONDEXPBONUS:                 return PSTAT_EX_SECONDEXPBONUS;
		case INV_EX_DOUBLE_HEALTHCAP:               return PSTAT_EX_DOUBLE_HEALTHCAP;
		case INV_EX_PHYSDAMAGEPER_FLATHEALTH:       return PSTAT_EX_PHYSDAMAGEPER_FLATHEALTH;
		case INV_EX_FORBID_ARMOR:                   return PSTAT_EX_FORBID_ARMOR;
		case INV_EX_CHANCE_ONDEATH_RAISEZOMBIE:     return PSTAT_EX_CHANCE_ONDEATH_RAISEZOMBIE;
		case INV_EX_DMGREDUCE_SHAREWITHPETS:        return PSTAT_EX_DMGREDUCE_SHAREWITHPETS;
		case INV_EX_DMGINCREASE_TAKEN:              return PSTAT_EX_DMGINCREASE_TAKEN;
		case INV_EX_ONKILL_HEALMISSING:             return PSTAT_EX_ONKILL_HEALMISSING;
		case INV_EX_SOULWEPS_FULLDAMAGE:            return PSTAT_EX_SOULWEPS_FULLDAMAGE;
		case INV_EX_ABILITY_RALLY:                  return PSTAT_EX_ABILITY_RALLY;
		case INV_EX_BEHAVIOR_SPELLSFULLDAMAGE:      return PSTAT_EX_BEHAVIOR_SPELLSFULLDAMAGE;
		case INV_EX_ABILITY_MONSTERSRIP:            return PSTAT_EX_ABILITY_MONSTERSRIP;
		case INV_EX_CURSEIMMUNITY:                  return PSTAT_EX_CURSEIMMUNITY;
		case INV_EX_LIMITEDSMALLCHARMS:             return PSTAT_EX_LIMITEDSMALLCHARMS;
		case INV_EX_FLATPERSHOTGUNOWNED:            return PSTAT_EX_FLATPERSHOTGUNOWNED;
		case INV_EX_LESSHEALING:                    return PSTAT_EX_LESSHEALING;
		case INV_EX_SOULWEPSPEN:                    return PSTAT_EX_SOULWEPSPEN;
		case INV_EX_DEADEYEBONUS:                   return PSTAT_EX_DEADEYEBONUS;
		case INV_EX_DAMAGPERMISSINGAMMO:            return PSTAT_EX_DAMAGPERMISSINGAMMO;
		case INV_EX_UNITY:                          return PSTAT_EX_UNITY;
		case INV_EX_UNITY_RES_BONUS:                return PSTAT_EX_UNITY_RES_BONUS;
		case INV_EX_UNITY_PEN_BONUS:                return PSTAT_EX_UNITY_PEN_BONUS;
		case INV_EX_UNITY_NOBONUS:                  return PSTAT_EX_UNITY_NOBONUS;
		case INV_EX_INTBONUSTOMELEE:                return PSTAT_EX_INTBONUSTOMELEE;
		case INV_EX_STARTESONDEPLETE:               return PSTAT_EX_STARTESONDEPLETE;
		case INV_EX_ESEXPLOSIONHPDMG:               return PSTAT_EX_ESEXPLOSIONHPDMG;
		case INV_EX_ESCHARGE_USEHP:                 return PSTAT_EX_ESCHARGE_USEHP;
		case INV_EX_HPTOESHIELD:                    return PSTAT_EX_HPTOESHIELD;
		case INV_EX_ESHIELDFULLABSORB:              return PSTAT_EX_ESHIELDFULLABSORB;
		case INV_EX_HEALTHATONE:                    return PSTAT_EX_HEALTHATONE;
		case INV_EX_RESPERESHIELD:                  return PSTAT_EX_RESPERESHIELD;
		case INV_EX_ESHIELDONLYBLOCKPCT:            return PSTAT_EX_ESHIELDONLYBLOCKPCT;
		case INV_EX_DAMAGELOWESTTAKENASPHYS:        return PSTAT_EX_DAMAGELOWESTTAKENASPHYS;
		case INV_EX_DEMONBARRIERS:                  return PSTAT_EX_DEMONBARRIERS;
		case INV_EX_STREXTRABONUSTOMELEE:           return PSTAT_EX_STREXTRABONUSTOMELEE;
		case INV_EX_CANFIREOVERHEATED:              return PSTAT_EX_CANFIREOVERHEATED;
		case INV_EX_CANTFIRENONOVERHEAT:            return PSTAT_EX_CANTFIRENONOVERHEAT;
		case INV_EX_MOREDMGPEROVERHEAT:             return PSTAT_EX_MOREDMGPEROVERHEAT;
		case INV_EX_LESSDMGTAKENMAXOVERHEAT:        return PSTAT_EX_LESSDMGTAKENMAXOVERHEAT;
		case INV_EX_WEAPONSUSEHEALTH:               return PSTAT_EX_WEAPONSUSEHEALTH;
		case INV_EX_RIPPERSONETIMEONLY:             return PSTAT_EX_RIPPERSONETIMEONLY;
		case INV_EX_RIPPERSRIPALL:                  return PSTAT_EX_RIPPERSRIPALL;
		case INV_EX_MIRROROTHERMEDIUM:              return PSTAT_EX_MIRROROTHERMEDIUM;
		case INV_EX_CHANCEGAINXCHARGE:              return PSTAT_EX_CHANCEGAINXCHARGE;
		case INV_EX_CHARGEDURATIONHALVED:           return PSTAT_EX_CHARGEDURATIONHALVED;
		case INV_EX_MOREDAMAGEPERCHARGE:            return PSTAT_EX_MOREDAMAGEPERCHARGE;
		case INV_EX_COUNTASHAVINGMAXCHARGEOF:       return PSTAT_EX_COUNTASHAVINGMAXCHARGEOF;
		case INV_EX_PLAYERPOWERSET1:                return PSTAT_EX_PLAYERPOWERSET1;

		// everything else (regular rollables)
		case INV_FLASKLIFERECOVERYRATE:             return PSTAT_FLASKLIFERECOVERYRATE;
		case INV_DROPCHANCE_INCREASE:               return PSTAT_DROPCHANCE_INCREASE;
		case INV_MAGAZINE_INCREASE:                 return PSTAT_MAGAZINE_INCREASE;
		case INV_HANDGUN_PERCENT:                   return PSTAT_HANDGUN_PERCENT;
		case INV_SHOTGUN_PERCENT:                   return PSTAT_SHOTGUN_PERCENT;
		case INV_AUTOMATIC_PERCENT:                 return PSTAT_AUTOMATIC_PERCENT;
		case INV_ARTILLERY_PERCENT:                 return PSTAT_ARTILLERY_PERCENT;
		case INV_PRECISION_PERCENT:                 return PSTAT_PRECISION_PERCENT;
		case INV_FLAT_HANDGUN:                      return PSTAT_FLAT_HANDGUN;
		case INV_FLAT_SHOTGUN:                      return PSTAT_FLAT_SHOTGUN;
		case INV_FLAT_AUTOMATIC:                    return PSTAT_FLAT_AUTOMATIC;
		case INV_FLAT_ARTILLERY:                    return PSTAT_FLAT_ARTILLERY;
		case INV_EXPLOSION_RADIUS:                  return PSTAT_EXPLOSION_RADIUS;
		case INV_SHOPSTOCK_INCREASE:                return PSTAT_SHOPSTOCK_INCREASE;
		case INV_REGENCAP_INCREASE:                 return PSTAT_REGENCAP_INCREASE;
		case INV_DAMAGEPERCENT_MORE:                return PSTAT_DAMAGEPERCENT_MORE;
		case INV_STAT_STRENGTH:                     return PSTAT_STAT_STRENGTH;
		case INV_STAT_DEXTERITY:                    return PSTAT_STAT_DEXTERITY;
		case INV_STAT_INTELLECT:                    return PSTAT_STAT_INTELLECT;
		case INV_CHANCE_IGNORERADIUSIMMUNITY:       return PSTAT_CHANCE_IGNORERADIUSIMMUNITY;
		case INV_BLOCKERS_MOREDMG:                  return PSTAT_BLOCKERS_MOREDMG;
		case INV_SLOWEFFECT:                        return PSTAT_SLOWEFFECT;
		case INV_OVERLOADCHANCE:                    return PSTAT_OVERLOADCHANCE;
		case INV_MELEERANGE:                        return PSTAT_MELEERANGE;
		case INV_MELEEDAMAGE:                       return PSTAT_MELEEDAMAGE;
		case INV_DOTMULTI:                          return PSTAT_DOTMULTI;
		case INV_DOTMULTI_FIRE:                     return PSTAT_DOTMULTI_FIRE;
		case INV_DOTMULTI_POISON:                   return PSTAT_DOTMULTI_POISON;
		case INV_DOTMULTI_BLEED:                    return PSTAT_DOTMULTI_BLEED;
		case INV_CHARGEDURATION:                    return PSTAT_CHARGEDURATION;
		case INV_REGENRATE:                         return PSTAT_REGENRATE;
		case INV_FLAT_TECH:                         return PSTAT_FLAT_TECH;
		case INV_FLAT_PRECISION:                    return PSTAT_FLAT_PRECISION;
		case INV_TECH_PERCENT:                      return PSTAT_TECH_PERCENT;
		case INV_FLAT_MAGIC:                        return PSTAT_FLAT_MAGIC;
		case INV_MAGIC_PERCENT:                     return PSTAT_MAGIC_PERCENT;
		case INV_REDUCED_OVERHEAT:                  return PSTAT_REDUCED_OVERHEAT;
		case INV_ITEMRARITY:                        return PSTAT_ITEMRARITY;
		case INV_RIPCOUNT:                          return PSTAT_RIPCOUNT;
		case INV_RIPDAMAGE:                         return PSTAT_RIPDAMAGE;
		case INV_LOCKONAREA:                        return PSTAT_LOCKONAREA;
		case INV_LOCKONRANGE:                       return PSTAT_LOCKONRANGE;
		case INV_INCKILLINGSPREE:                   return PSTAT_INCKILLINGSPREE;
		case INV_REDUCEDCURSEEFFECT:                return PSTAT_REDUCEDCURSEEFFECT;
		case INV_REDUCEDCURSEDURATION:              return PSTAT_REDUCEDCURSEDURATION;
		case INV_FRENZYCHARGE_ONSHATTER:            return PSTAT_FRENZYCHARGE_ONSHATTER;
		case INV_ENDURANCECHARGE_ONMELEE:           return PSTAT_ENDURANCECHARGE_ONMELEE;
		case INV_POWERCHARGE_ONOVERLOAD:            return PSTAT_POWERCHARGE_ONOVERLOAD;
		case INV_MELEESPLASH_NOTONLOWSTAMINA:       return PSTAT_MELEESPLASH_NOTONLOWSTAMINA;
		case INV_MELEE_ATKCDR:                      return PSTAT_MELEE_ATKCDR;
		case INV_CRUSHINGBLOW:                      return PSTAT_CRUSHINGBLOW;
		case INV_DEEPCUTS:                          return PSTAT_DEEPCUTS;
		case INV_OPENWOUNDS:                        return PSTAT_OPENWOUNDS;
		case INV_DEADLYSTRIKE:                      return PSTAT_DEADLYSTRIKE;
		case INV_REAPINGCLEAVE:                     return PSTAT_REAPINGCLEAVE;
		case INV_INCFLASKCHARGEGAINED:              return PSTAT_INCFLASKCHARGEGAINED;
	}
	return DND_PSTAT_UNMAPPED;
}

// The flag equivalent of MapAttributeToPStat, for mods stored as a bit instead of a value.
//
// Its main job is not writing -- ProcessAttribute calls SetPlayerFlag directly for these -- it is
// READING. The exotic stat page walks the whole unique id range asking ReadPlayerModValue for a
// magnitude, so without this a mod moved to a flag would report 0 and quietly vanish off the page.
int MapAttributeToPFlag(int mod) {
	switch(mod) {
		case INV_IMP_DOUBLEESHIELDRECOVERY:	return PFLAG_CYBER;
		case INV_IMP_ESHIELDBLOCKSALL:		return PFLAG_ESHIELDBLOCKALL;
		case INV_IMP_MELEEIGNORESSHIELDS:	return PFLAG_MELEEIGNORESHIELD;
		case INV_IMP_HIGHREFLECTREDUCE:		return PFLAG_LOWERREFLECT;
		case INV_EX_ELEPENHARMONY:			return PFLAG_ELEPENHARMONY;
		case INV_INC_MITIGATIONTODODGE:		return PFLAG_MITIGATION_TO_DODGE;
		case INV_INC_ESHIELDNOINTERRUPT:	return PFLAG_ESHIELD_NOINTERRUPT;
		case INV_EX_ESCHARGE_DMGNOINTERRUPT:return PFLAG_ESCHARGE_NOINTERRUPT;
		case INV_EX_ABILITY_LUCKYCRIT:            return PFLAG_LUCKYCRIT;
		case INV_INC_ACCURACYREVERSED:            return PFLAG_ACCURACY_REVERSED;
		case INV_EX_KNOCKBACK_IMMUNITY:           return PFLAG_KNOCKBACK_IMMUNITY;
		case INV_EX_BEHAVIOR_PELLETSFIRECIRCLE:   return PFLAG_PELLETS_FIRE_CIRCLE;
	}
	return DND_PFLAG_UNMAPPED;
}

// Attribute id -> extra slot. Same shape as MapAttributeToPStat and consulted at equip time only.
int MapAttributeToPExtra(int mod) {
	switch(mod) {
		case INV_ESS_VAAJ:                          return PEXTRA_ESS_VAAJ;
		case INV_IMP_ABSORBLIGHTNING:               return PEXTRA_IMP_ABSORBLIGHTNING;
		case INV_IMP_KNIGHTARMOR:                   return PEXTRA_IMP_KNIGHTARMOR;
		case INV_IMP_RAVAGER:                       return PEXTRA_IMP_RAVAGER;
		case INV_IMP_STAMINAONKILL:                 return PEXTRA_IMP_STAMINAONKILL;
		case INV_IMP_UNSTABLECORE:                  return PEXTRA_IMP_UNSTABLECORE;
		case INV_INC_INSTANTLIFESTEAL:              return PEXTRA_INC_INSTANTLIFESTEAL;
		case INV_INC_MOREHPBONUS:                   return PEXTRA_INC_MOREHPBONUS;
		case INV_INC_PASSIVEREGEN:                  return PEXTRA_INC_PASSIVEREGEN;
		case INV_EX_CHANCEGAINXCHARGE:              return PEXTRA_EX_CHANCEGAINXCHARGE;
		case INV_EX_CHANCE_HEALMISSINGONPAIN:       return PEXTRA_EX_CHANCE_HEALMISSINGONPAIN;
		case INV_EX_COUNTASHAVINGMAXCHARGEOF:       return PEXTRA_EX_COUNTASHAVINGMAXCHARGEOF;
		case INV_EX_SECONDEXPBONUS:                 return PEXTRA_EX_SECONDEXPBONUS;
	}
	return DND_PEXTRA_UNMAPPED;
}

int ReadPlayerModExtra(int pnum, int mod) {
	int slot = MapAttributeToPExtra(mod);
	if(slot != DND_PEXTRA_UNMAPPED)
		return PlayerModData[pnum].x[slot];
	return 0;
}

void WritePlayerModExtra(int pnum, int mod, int val) {
	int slot = MapAttributeToPExtra(mod);
	if(slot != DND_PEXTRA_UNMAPPED)
		PlayerModData[pnum].x[slot] = val;
}

// Id-keyed accessors. These exist for the paths that genuinely only have an id in hand -- item
// application, the client sync scripts and the stat pages -- and they pay the switch above for it.
// Gameplay code must not use them for a migrated mod: read the slot.
int ReadPlayerModValue(int pnum, int mod) {
	int slot = MapAttributeToPStat(mod);
	if(slot != DND_PSTAT_UNMAPPED)
		return PlayerModData[pnum].f[slot];

	// A flag has no magnitude, so 1 is the honest answer to "how much of this does the player have".
	// The stat pages test exactly that, which is what keeps a flagged mod visible on them.
	int flag = MapAttributeToPFlag(mod);
	if(flag != DND_PFLAG_UNMAPPED)
		return HasPlayerFlag(pnum, flag);

	// Unmapped means the mod has no player scope storage, which is a real answer and not a gap --
	// see the note on the struct.
	return 0;
}

void WritePlayerModValue(int pnum, int mod, int val) {
	int slot = MapAttributeToPStat(mod);
	if(slot != DND_PSTAT_UNMAPPED) {
		PlayerModData[pnum].f[slot] = val;
		return;
	}

	// Everything else has nowhere to go, and that is deliberate. A flag is owned by SetPlayerFlag and
	// its refcount; the three fan-out mods keep their numbers in Player_Weapon_Infos. Dropping the
	// write is what makes those cases honest instead of parking a number nothing can read back.
}

// ---- boolean flags ------------------------------------------------------------------------------
int GetPlayerFlagCount(int pnum, int flag) {
	return (PlayerModData[pnum].pflag_rc[flag >> 2] >> ((flag & 3) << 3)) & 0xFF;
}

void SetPlayerFlagCount(int pnum, int flag, int cnt) {
	int w = flag >> 2;
	int shift = (flag & 3) << 3;
	PlayerModData[pnum].pflag_rc[w] = (PlayerModData[pnum].pflag_rc[w] & ~(0xFF << shift)) | ((cnt & 0xFF) << shift);
}

// One equipped source granting or dropping a flag. The bit is on while the count is non-zero.
void SetPlayerFlag(int pnum, int flag, bool remove) {
	int cnt = GetPlayerFlagCount(pnum, flag);

	if(remove) {
		// Clamp rather than trust the caller. The INV_EX_FACTOR_SMALLCHARM handler strips and re-adds
		// every small charm around a factor change, so a remove with no matching add is reachable --
		// and an unclamped decrement would wrap to 255 and pin the flag on for the session.
		if(!cnt)
			return;
		--cnt;
	}
	else {
		if(cnt >= 0xFF)
			return;
		++cnt;
	}

	SetPlayerFlagCount(pnum, flag, cnt);

	if(cnt)
		PlayerModData[pnum].pflags[flag >> 5] |= 1 << (flag & 31);
	else
		PlayerModData[pnum].pflags[flag >> 5] &= ~(1 << (flag & 31));

	ACS_NamedExecuteWithResult("DnD Request Flag Sync", pnum, flag >> 5, PlayerModData[pnum].pflags[flag >> 5]);
}

// More multiplier mods are multiplied amongst themselves in case of having more than one source, and are all "FIXED POINT" values, not integers
bool IsMoreMultiplierMod(int mod) {
	switch(mod) {
		case INV_EXPGAIN_INCREASE:
		case INV_CREDITGAIN_INCREASE:
		case INV_LUCK_INCREASE:
		case INV_PELLET_INCREASE:
		case INV_DAMAGEPERCENT_MORE:
		case INV_BLOCKERS_MOREDMG:
		case INV_OVERLOAD_DMGINCREASE:
		case INV_LIFESTEAL_DAMAGE:
		case INV_ESS_ERYXIA:
		case INV_EX_MORECRIT_LIGHTNING:
		case INV_CORR_WEAPONDMG:
		case INV_INC_EXCESSCRIT:
		case INV_CORR_MOREAOE:
		return true;
	}
	return false;
}

// returns true if the mod is stored and used as fixed
bool IsFixedPointMod(int mod) {
	switch(mod) {
		case INV_EXPGAIN_INCREASE:
		case INV_CREDITGAIN_INCREASE:
		case INV_LUCK_INCREASE:
		case INV_SPEED_INCREASE:
		case INV_PELLET_INCREASE:
		case INV_CRITCHANCE_INCREASE:
		case INV_CRITPERCENT_INCREASE:
		case INV_CRITPERCENT_FORWEPTYPE:
		case INV_DAMAGEPERCENT_MORE:
		case INV_BLOCKERS_MOREDMG:
		case INV_PROJSPEED:
		case INV_DMGREDUCE_ELEM:
		case INV_DMGREDUCE_ENERGY:
		case INV_DMGREDUCE_PHYS:
		case INV_DMGREDUCE_MAGIC:
		case INV_DMGREDUCE_ALL:
		case INV_DMGREDUCE_REFL:
		case INV_ADDEDMAXRESIST:
		case INV_OVERLOAD_DURATION:
		case INV_OVERLOAD_DMGINCREASE:
		case INV_LIFESTEAL:
		case INV_LIFESTEAL_DAMAGE:

		case INV_IGNITE_PROLIFRANGE:

		case INV_MIT_INCREASE:
		case INV_MITEFFECT_INCREASE:

		case INV_MELEECRIT_NOTONLOWSTAMINA:

		case INV_CORR_WEAPONDMG:
		case INV_CORR_DROPCHANCE:
		case INV_CORR_MOREAOE:

		case INV_ESS_ERYXIA:

		case INV_INC_PASSIVEREGEN:
		case INV_INC_PROJREVERSE:

		case INV_EX_MORECRIT_LIGHTNING:
		return true;
	}
	return false;
}

bool IsStaticMod(int mod) {
	switch(mod) {
		case INV_INC_PLUSPROJ:
		case INV_INC_PLUSTWOPROJ:
		return true;
	}
	return false;
}

// Returns a weapon as extra field for the given corruption mod
int GetExtraForMod(int pnum, int mod, int tier = 0, int item_type = -1, int item_subtype = -1, bool isWellRolled = false, int attr_val = -1) {
	int res = -1;

	if(item_type != DND_ITEM_DUNGEONKEY) {
		switch(mod) {
			// extra is the weapon_id for these
			case INV_CORR_WEAPONDMG:
			case INV_CORR_WEAPONCRIT:
			case INV_CORR_WEAPONCRITDMG:
			case INV_CORR_WEAPONPOISONPCT:
			case INV_CORR_WEAPONFORCEPAIN:
			case INV_CORR_WEPCULL:
				// pick one from a weapon the player owns
				if(pnum != MAXPLAYERS)
					res = PickRandomOwnedWeaponID(pnum);
				else
					res = random(FIRST_SLOT0_WEAPON, LAST_SLOT9_WEAPON);
			break;
			case INV_CRITPERCENT_FORWEPTYPE:
				res = random(DND_WEPTYPE_TECH, DND_WEPTYPE_LAST);
			break;

			// mods that have natural extra values
			case INV_ESS_VAAJ:
			case INV_INC_MOREHPBONUS:
			case INV_INC_PASSIVEREGEN:
			case INV_INC_INSTANTLIFESTEAL:
			case INV_FLASK_INCAMOUNTRECOVER:
			case INV_FLASK_INSTANTRECOVERY:
			case INV_FLASK_INCEFFECT:
			case INV_FLASK_INSTANTONLOWLIFE:
				res = RollAttributeExtra(mod, tier, isWellRolled, item_type, item_subtype);
			break;
			case INV_FLASK_INCCHARGERECOVERY:
				res = DND_FLASK_RECOVERY_REDUCEEFFECT;
			break;

			case INV_INC_PLUSPROJ:
			case INV_CORR_WEAPONPLUSPROJ:
				if(pnum != MAXPLAYERS)
					res = PickRandomOwnedWeaponID_WithProj(pnum);
				else
					res = random(FIRST_SLOT0_WEAPON, LAST_SLOT9_WEAPON);
			break;
			case INV_INC_PLUSTWOPROJ:
				if(pnum != MAXPLAYERS)
					res = PickRandomOwnedWeaponID_WithProj(pnum);
				else
					res = random(FIRST_SLOT0_WEAPON, LAST_SLOT9_WEAPON);
			break;

			// gained-as rolls its pair exactly like conversion -- same ladder, same packing
			case INV_CORR_DAMAGEGAINEDAS:
			case INV_CORR_DAMAGECONVERSION:
				// pick first mod then mapped mod shift it by 8 to combine.
				// The source stops one short of the end: it is the LAST rung of the conversion
				// ladder, so rolling it leaves random() no destination above it and the pair comes
				// out as either a self-conversion or soul, both of which the ladder drops on the
				// floor. That would be a dead mod on a seventh of the rolls.
				res = random(DND_DAMAGECONVERSION_BEGIN, DND_DAMAGECONVERSION_END - 1);
				res |= random(res + 1, DND_DAMAGECONVERSION_END) << DND_DAMAGECONVERSION_BITS;
			break;
		}
	}
	else {
		// for now, assign one of the random upside modifier ids to it
		res = random(FIRST_DUNGEON_UPSIDE, DUN_UPSIDE_MAX - 1);
		res |= RollDungeonAttributeExtra(res, mod, tier, isWellRolled) << 16;
	}
	return res;
}

void SetPlayerModValue(int pnum, int mod, int val) {
	WritePlayerModValue(pnum, mod, val);
	PushToPlayerAttributeSync(pnum, mod);
}

void SetPlayerModExtra(int pnum, int mod, int val) {
	WritePlayerModExtra(pnum, mod, val);
	PushToPlayerAttributeExtraSync(pnum, mod);
}

void IncPlayerModValue(int pnum, int mod, int val) {
	// Read once, decide, write once -- the storage is behind a mapper now, so the old form that
	// touched PlayerModData[pnum].value[mod] five times in one function cannot be kept.
	int cur = ReadPlayerModValue(pnum, mod);

	// check if it's a "more" multiplier, they are multiplicative with each other
	if(!IsMoreMultiplierMod(mod)) {
		cur += val;
	}
	else if(!cur) {
		// if we are zero, simply replace with val
		cur = val;
	}
	else if(val > 0) {
		// non-zero, multiply case -- we store things like 0.2 etc. here, but while we amplify it we need to consider 1.0 + val
		cur = CombineMultiplicativeFactors(cur, val) - 1.0;
	}
	else if(val < 0) {
		// if negative we divide
		// if mod value == val, this means we need to set to zero (it's removed), otherwise just divide it
		if(cur + val < EPSILON)
			cur = 0;
		else
			cur = CancelMultiplicativeFactors(cur, -val) - 1.0;
	}

	WritePlayerModValue(pnum, mod, cur);
	PushToPlayerAttributeSync(pnum, mod);
}

// Which DND_HIGHESTSRC_* row a mod owns, or -1 for everything that adds up normally.
int GetHighestSourceRow(int mod) {
	switch(mod) {
		case INV_INC_MOREHPBONUS:
		return DND_HIGHESTSRC_MOREHPBONUS;
		case INV_INC_CRITFORDOT:
		return DND_HIGHESTSRC_CRITFORDOT;
	}
	return -1;
}

// The attribute value for one of these mods is always the largest live source, never their sum.
int GetHighestModSource(int pnum, int row) {
	int res = 0;

	for(int i = 0; i < DND_MAX_HIGHESTSRC_SLOTS; ++i)
		if(PlayerModData[pnum].highest_sources[row][i] > res)
			res = PlayerModData[pnum].highest_sources[row][i];

	return res;
}

// slot is ProcessAttribute's item_index; val of 0 clears the slot.
//
// Keyed on the slot rather than on the value, because the value a slot contributes is NOT stable
// while it is equipped. Well of Power (INV_EX_FACTOR_SMALLCHARM) scales small charm magnitudes, and
// neither of these mods is in IsAttributeQualityException, so the same charm is worth more with the
// Well on than off. A value keyed remove would go looking for a number nothing holds any more, find
// no match, and leave the source switched on for the rest of the session. Slot keying is also immune
// to the double apply in the INV_EX_FACTOR_SMALLCHARM handler, which strips and re-adds every small
// charm around the factor change -- writing the same slot twice is a no-op, pushing a second list
// entry would not have been.
//
// One slot holds one contribution, which is exactly right as long as a mod cannot appear twice on the
// same item -- the roll loops reject a mod the item already carries, so it cannot.
void SetHighestModSource(int pnum, int mod, int slot, int val) {
	int row = GetHighestSourceRow(mod);

	if(row < 0 || slot < 0 || slot >= DND_MAX_HIGHESTSRC_SLOTS)
		return;

	PlayerModData[pnum].highest_sources[row][slot] = val;
	SetPlayerModValue(pnum, mod, GetHighestModSource(pnum, row));
}

void IncPlayerModExtra(int pnum, int mod, int val) {
	// Read once, decide, write once -- same restructure IncPlayerModValue needed, same reason.
	int cur = ReadPlayerModExtra(pnum, mod);

	// check if it's a "more" multiplier, they are multiplicative with each other
	if(!IsMoreMultiplierMod(mod)) {
		cur += val;
	}
	else if(!cur) {
		// if we are zero, simply replace with val
		cur = val;
	}
	else if(val > 0) {
		// non-zero, multiply case -- we store things like 0.2 etc. here, but while we amplify it we need to consider 1.0 + val
		cur = CombineMultiplicativeFactors(cur, val) - 1.0;
	}
	else if(val < 0) {
		// if negative we divide
		// if mod value == val, this means we need to set to zero (it's removed), otherwise just divide it
		if(cur + val < EPSILON)
			cur = 0;
		else
			cur = CancelMultiplicativeFactors(cur, -val) - 1.0;
	}

	WritePlayerModExtra(pnum, mod, cur);
	
	PushToPlayerAttributeExtraSync(pnum, mod);
}

void ResetPlayerModList(int pnum) {
	for(int i = 0; i < DND_PEXTRA_COUNT; ++i)
		PlayerModData[pnum].x[i] = 0;

	// The highest source rows are not derived from value[] and have to be cleared with it, or a
	// character reload leaves phantom sources behind that nothing will ever remove.
	for(i = 0; i < DND_HIGHESTSRC_COUNT; ++i)
		for(int j = 0; j < DND_MAX_HIGHESTSRC_SLOTS; ++j)
			PlayerModData[pnum].highest_sources[i][j] = 0;

	// The dense stat slots and the flag words are not derived from value[] and have to be cleared
	// with it. Flags carry a refcount, so a leftover count would keep its bit on through a reload.
	for(i = 0; i < PSTAT_COUNT; ++i)
		PlayerModData[pnum].f[i] = 0;
	for(i = 0; i < DND_PFLAG_WORDS; ++i)
		PlayerModData[pnum].pflags[i] = 0;
	for(i = 0; i < DND_PFLAG_RCWORDS; ++i)
		PlayerModData[pnum].pflag_rc[i] = 0;

	// Damage conversion accumulates outside PlayerModData -- one summed attribute cannot tell two
	// conversion mods apart when each names its own source and destination -- so it has to be reset
	// alongside it or a character reload doubles everything the player is wearing.
	ResetPlayerConversionTable(pnum);

	ACS_NamedExecuteWithResult("DnD Reset Player Mod List", pnum);
}

void SyncPlayerItemMods(int pnum) {
	// Everything is keyed by SLOT now, so this walks the storage rather than the attribute id space:
	// 315 slots plus 13 extras plus one flag word, against the 6000 id probes this used to make. A
	// slot shared by two mods has no single id that could name it, so the id-keyed form could not
	// have carried it even in principle.
	for(int i = 0; i < DND_PEXTRA_COUNT; ++i)
		if(PlayerModData[pnum].x[i])
			ACS_NamedExecuteWithResult("DnD Request Extra Sync", pnum, i, PlayerModData[pnum].x[i]);

	for(i = 0; i < PSTAT_COUNT; ++i)
		if(PlayerModData[pnum].f[i])
			ACS_NamedExecuteWithResult("DnD Request Stat Sync", pnum, i, PlayerModData[pnum].f[i]);

	for(i = 0; i < DND_PFLAG_WORDS; ++i)
		ACS_NamedExecuteWithResult("DnD Request Flag Sync", pnum, i, PlayerModData[pnum].pflags[i]);
}

// resets things clientside for the array
Script "DnD Reset Player Mod List" (int pnum) CLIENTSIDE {
	for(int i = 0; i < DND_PEXTRA_COUNT; ++i)
		PlayerModData[pnum].x[i] = 0;

	// The highest source rows are not derived from value[] and have to be cleared with it, or a
	// character reload leaves phantom sources behind that nothing will ever remove.
	for(i = 0; i < DND_HIGHESTSRC_COUNT; ++i)
		for(int j = 0; j < DND_MAX_HIGHESTSRC_SLOTS; ++j)
			PlayerModData[pnum].highest_sources[i][j] = 0;

	// The dense stat slots and the flag words are not derived from value[] and have to be cleared
	// with it. Flags carry a refcount, so a leftover count would keep its bit on through a reload.
	for(i = 0; i < PSTAT_COUNT; ++i)
		PlayerModData[pnum].f[i] = 0;
	for(i = 0; i < DND_PFLAG_WORDS; ++i)
		PlayerModData[pnum].pflags[i] = 0;
	for(i = 0; i < DND_PFLAG_RCWORDS; ++i)
		PlayerModData[pnum].pflag_rc[i] = 0;
}

// returns the amount to skip over the base range to map it into its appropriate tier
int GetModTierRangeMapper(int attr, int lvl) {
	int val = 0;
	if(ItemModTable[attr].attrib_level_modifier == -1)
		return -1;

	if(!ItemModTable[attr].attrib_level_modifier)
		val = (ItemModTable[attr].attrib_high - ItemModTable[attr].attrib_low + 1) * lvl;
	else
		val = (ItemModTable[attr].attrib_level_modifier * lvl);
	return val;
}

int GetModTierRangeMapperExtra(int attr, int lvl) {
	int val = 0;
	if(ItemModTable[attr].attrib_level_extra_modifier == -1)
		return -1;

	if(!ItemModTable[attr].attrib_level_extra_modifier)
		val = (ItemModTable[attr].attrib_extra_high - ItemModTable[attr].attrib_extra_low + 1) * lvl;
	else
		val = (ItemModTable[attr].attrib_level_extra_modifier * lvl);
	return val;
}

#define DND_POWERCORE_ATTRFACTOR 0
#define DND_SMALLCHARM_ATTRFACTOR -50
#define DND_LARGECHARM_ATTRFACTOR 25
 
bool IsAttributeExtraException(int attr) {
	switch(attr) {
		case INV_IMP_INCARMOR:
		case INV_IMP_INCARMORSHIELD:
		case INV_IMP_INCSHIELD:
		case INV_IMP_INCMIT:
		case INV_IMP_INCMITARMOR:
		case INV_IMP_INCMITSHIELD:
		case INV_IMP_INCMITARMORSHIELD:
		case INV_IMP_POWERCORE:

		// flask implicits -- utility flask extra contains the duration, which increases with quality
		case INV_FLASK_IMP_LIFE:
		case INV_FLASK_IMP_CHARGECOUNT:

		// these store the weapon in extra field!
		case INV_CRITPERCENT_FORWEPTYPE:

		case INV_CORR_WEAPONDMG:
		case INV_CORR_WEAPONCRIT:
		case INV_CORR_WEAPONCRITDMG:
		case INV_CORR_WEAPONPLUSPROJ:
		case INV_CORR_WEAPONPOISONPCT:
		case INV_CORR_WEAPONFORCEPAIN:
		case INV_CORR_WEPCULL:
		case INV_CORR_DAMAGECONVERSION:
		case INV_CORR_DAMAGEGAINEDAS:

		// incursion things that use extra field
		case INV_INC_PLUSPROJ:
		case INV_INC_PLUSTWOPROJ:

		// unique things
		case INV_EX_CHANCEGAINXCHARGE:
		case INV_EX_COUNTASHAVINGMAXCHARGEOF:

		return true;
	}
	return false;
}

bool CanRerollAttributeExtra(int attr) {
	switch(attr) {
		case INV_ESS_VAAJ:

		case INV_INC_PLUSPROJ:
		case INV_INC_PLUSTWOPROJ:
		case INV_CRITPERCENT_FORWEPTYPE:
		return true;
	}
	return false;
}

bool IsAttributeQualityException(int attr) {
	switch(attr) {
		case INV_FLASK_IMP_CHARGECOUNT:
		case INV_FLASK_IMP_GRANITE:
		case INV_FLASK_IMP_BASALT:
		case INV_FLASK_IMP_BISMUTH:
		case INV_FLASK_IMP_INSULAR:
		case INV_FLASK_IMP_OAK:
		case INV_FLASK_IMP_ARCANE:
		case INV_FLASK_IMP_DIAMOND:
		case INV_FLASK_IMP_SILVER:
		case INV_FLASK_IMP_SULPHUR:
		case INV_FLASK_IMP_QUICKSILVER:
		case INV_FLASK_IMP_QUARTZ:
		case INV_EX_LIMITEDSMALLCHARMS:
		case INV_EX_COUNTASHAVINGMAXCHARGEOF:
		return true;
	}
	return false;
}

// Quality scaling for an attribute's value and extra field.
//
// These exist so the tooltip and the mechanics cannot disagree. They used to be the same arithmetic
// written out in both places, and flasks only ever had the tooltip half: GetFlaskAttributeVal reads
// straight off the item, so every flask affix displayed a quality boosted number and then behaved as
// if the item had no quality at all.
//
// The odd looking branch is deliberate and must be kept: multiplying first overflows once the value
// is large, dividing first loses precision when it is small, so each case takes the order that is
// safe for it.
int ApplyQualityToAttribValue(int val, int qual, int attr) {
	if(!qual || IsAttributeQualityException(attr))
		return val;

	if(val < 100000) {
		val *= qual + 100;
		val /= 100;
	}
	else {
		val /= 100;
		val *= qual + 100;
	}
	return val;
}

int ApplyQualityToAttribExtra(int extra, int qual, int attr) {
	if(!qual || !extra || IsAttributeExtraException(attr))
		return extra;

	if(extra > 100000) {
		extra /= 100;
		extra *= qual + 100;
	}
	else {
		extra *= qual + 100;
		extra /= 100;
	}
	return extra;
}

bool IsUniqueModRerollException(int attr) {
	switch(attr) {
		case INV_EX_COUNTASHAVINGMAXCHARGEOF:
		return true;
	}
	return false;
}

// Add other item properties related to item quality here
int GetItemAttributeFactor(int item_type, int item_subtype) {
	if(item_type != DND_ITEM_CHARM && (item_type < FIRST_SPECIALTY_ITEM_TYPE || item_type > LAST_SPECIALTY_ITEM_TYPE))
		return 0;
	
	if(item_type == DND_ITEM_CHARM) {
		if(item_subtype == DND_CHARM_LARGE)
			return DND_LARGECHARM_ATTRFACTOR;
		else if(item_subtype == DND_CHARM_SMALL)
			return DND_SMALLCHARM_ATTRFACTOR;
		return 0;
	}
	return DND_POWERCORE_ATTRFACTOR;
}

// this uses a precalculated tier mapping to save time
int GetModRangeWithTier(int attr, int tier_mapping, bool which, int attr_factor) {
	int res = 0;

	if(tier_mapping != -1) {
		if(!which)
			res = (ItemModTable[attr].attrib_low + tier_mapping + (tier_mapping != 0)) * (100 + attr_factor) / 100;
		else
			res = (ItemModTable[attr].attrib_high + tier_mapping + (tier_mapping != 0)) * (100 + attr_factor) / 100;
	}
	else {
		if(!which)
			res = ItemModTable[attr].attrib_low * (100 + attr_factor) / 100;
		else
			res = ItemModTable[attr].attrib_high * (100 + attr_factor) / 100;
	}
	
	if(!res)
		res = 1;
	return res;
}

int GetModRangeWithTierExtra(int attr, int tier_mapping, bool which, int attr_factor) {
	int res = 0;

	if(tier_mapping != -1) {
		if(!which)
			res = (ItemModTable[attr].attrib_extra_low + tier_mapping + (tier_mapping != 0)) * (100 + attr_factor) / 100;
		else
			res = (ItemModTable[attr].attrib_extra_high + tier_mapping + (tier_mapping != 0)) * (100 + attr_factor) / 100;
	}
	else {
		if(!which)
			res = ItemModTable[attr].attrib_extra_low * (100 + attr_factor) / 100;
		else
			res = ItemModTable[attr].attrib_extra_high * (100 + attr_factor) / 100;
	}
	
	if(!res)
		res = 1;
	return res;
}

// this calculates the tier mapping for itself based on supplied level
int GetModRange(int attr, int tier, bool which, int attr_factor) {
	return GetModRangeWithTier(attr, GetModTierRangeMapper(attr, tier), which, attr_factor);
}

int GetModRangeExtra(int attr, int tier, bool which, int attr_factor) {
	return GetModRangeWithTierExtra(attr, GetModTierRangeMapperExtra(attr, tier), which, attr_factor);
}

// rolls an attribute's value depending on specified parameters
// if they are fixed, put the truncated value in to match (as closely as possible...) what the menu displays in stat gains
// item type and subtype store whether its a charm/other and type of charm if applicable
int RollAttributeValue(int attr, int tier, bool isWellRolled, int item_type, int item_subtype) {
	if(item_type == DND_ITEM_DUNGEONKEY)
		return RollDungeonAttributeValue(attr, tier, isWellRolled);

	int tier_mapping = GetModTierRangeMapper(attr, tier);
	int temp;
	
	int f = GetItemAttributeFactor(item_type, item_subtype);
	if(IsStaticMod(attr))
		f = 0;

	bool revered = CheckInventory("ReveranceUsed");
	
	// the + 0.0005 is so the edge rolls can be achieved
	if(!isWellRolled && !revered) {
		temp = random(GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW, f), GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH, f));
		if(IsFixedPointMod(attr) && temp > 0.0005)
			temp += 0.0005;
		return temp;
	}
	
	// well rolled case
	temp = GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH, f);

	if(!revered)
		temp = random((GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW, f) + temp) / 2, temp);
	else
		temp = random(GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW, f) / 4 + (temp * 3) / 4, temp);
	
	if(IsFixedPointMod(attr) && temp > 0.0005)
		temp += 0.0005;
	
	return temp;
}

int RollAttributeExtra(int attr, int tier, bool isWellRolled, int item_type, int item_subtype) {
	int tier_mapping = GetModTierRangeMapperExtra(attr, tier);
	int temp;
	
	int f = GetItemAttributeFactor(item_type, item_subtype);
	if(IsStaticMod(attr))
		f = 0;

	bool revered = CheckInventory("ReveranceUsed");
	
	// the + 0.0005 is so the edge rolls can be achieved
	if(!isWellRolled && !revered) {
		temp = random(GetModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_LOW, f), GetModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_HIGH, f));
		if(IsFixedPointMod(attr) && temp > 0.0005)
			temp += 0.0005;
		return temp;
	}
	
	// well rolled case
	temp = GetModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_HIGH, f);

	if(!revered)
		temp = random((GetModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_LOW, f) + temp) / 2, temp);
	else
		temp = random(GetModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_LOW, f) / 4 + (temp * 3) / 4, temp);
	
	if(IsFixedPointMod(attr) && temp > 0.0005)
		temp += 0.0005;
	
	return temp;
}

int RollUniqueAttributeValue(int unique_id, int attr, bool isWellRolled) {
	bool reverance = CheckInventory("ReveranceUsed");
	if(!isWellRolled && !reverance)
		return random(UniqueItemList[unique_id].rolls[attr].attrib_low, UniqueItemList[unique_id].rolls[attr].attrib_high);
	if(!reverance)
		return random((UniqueItemList[unique_id].rolls[attr].attrib_low + UniqueItemList[unique_id].rolls[attr].attrib_high) / 2, UniqueItemList[unique_id].rolls[attr].attrib_high);
	return random(UniqueItemList[unique_id].rolls[attr].attrib_low / 4 + 3 * UniqueItemList[unique_id].rolls[attr].attrib_high / 4, UniqueItemList[unique_id].rolls[attr].attrib_high);
}

int RollUniqueAttributeExtra(int unique_id, int attr, bool isWellRolled) {
	bool reverance = CheckInventory("ReveranceUsed");
	if(!isWellRolled && !reverance)
		return random(UniqueItemList[unique_id].rolls[attr].attrib_extra_low, UniqueItemList[unique_id].rolls[attr].attrib_extra_high);
	if(!reverance)
		return random((UniqueItemList[unique_id].rolls[attr].attrib_extra_low + UniqueItemList[unique_id].rolls[attr].attrib_extra_high) / 2, UniqueItemList[unique_id].rolls[attr].attrib_extra_high);
	return random(UniqueItemList[unique_id].rolls[attr].attrib_extra_low / 4 + 3 * UniqueItemList[unique_id].rolls[attr].attrib_extra_high / 4, UniqueItemList[unique_id].rolls[attr].attrib_extra_high);
}

int MapItemTypeToCraftableID(int type) {
	switch(type) {
		case DND_ITEM_BODYARMOR:
		return DND_CRAFTABLEID_BODYARMOR;
		case DND_ITEM_BOOT:
		return DND_CRAFTABLEID_BOOT;

		case DND_ITEM_HELM:
		case DND_ITEM_SPECIALTY_HOBO: // hobo ruleset is same as helms', its a facewear afterall...
		return DND_CRAFTABLEID_HELM;

		case DND_ITEM_SPECIALTY_DOOMGUY:
		case DND_ITEM_SPECIALTY_MARINE:
		case DND_ITEM_SPECIALTY_PUNISHER:
		case DND_ITEM_SPECIALTY_CYBORG:
		return DND_CRAFTABLEID_SPECIALTY_GENERIC;
		
		case DND_ITEM_SPECIALTY_WANDERER:
		return DND_CRAFTABLEID_SPECIALTY_WANDERER;
		case DND_ITEM_SPECIALTY_BERSERKER:
		return DND_CRAFTABLEID_SPECIALTY_BERSERKER;
		case DND_ITEM_SPECIALTY_TRICKSTER:
		return DND_CRAFTABLEID_SPECIALTY_TRICKSTER;
	}
	return DND_CRAFTABLEID_CHARM;
}

int PickRandomIncursionMod() {
	return random(FIRST_INCURSION_ATTRIBUTE, LAST_INCURSION_ATTRIBUTE);
}

#include "DnD_InvAttribText.h"

#endif
