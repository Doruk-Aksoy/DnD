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

enum {
	PPOWER_CYBER 						= 	0b1,
	PPOWER_ESHIELDBLOCKALL				=	0b100,
	PPOWER_MELEEIGNORESHIELD			=	0b10000,
	PPOWER_LOWERREFLECT					=	0b1000000
};

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

typedef struct {
	int extra[MAX_TOTAL_ATTRIBUTES];
	int value[MAX_TOTAL_ATTRIBUTES];

	// The DND_HIGHESTSRC_* mods take their strongest source rather than adding them up, so the
	// individual contributions are kept here, indexed by the slot they came from. A running total
	// cannot do it: dropping the strongest one has to fall back to the second strongest, and a sum
	// has already forgotten what the parts were.
	int highest_sources[DND_HIGHESTSRC_COUNT][DND_MAX_HIGHESTSRC_SLOTS];
} player_item_mod_data_T;

global player_item_mod_data_T 57: PlayerModData[MAXPLAYERS];

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
	PlayerModData[pnum].value[mod] = val;
	PushToPlayerAttributeSync(pnum, mod);
}

void SetPlayerModExtra(int pnum, int mod, int val) {
	PlayerModData[pnum].extra[mod] = val;
	PushToPlayerAttributeExtraSync(pnum, mod);
}

void IncPlayerModValue(int pnum, int mod, int val) {
	// check if it's a "more" multiplier, they are multiplicative with each other
	if(!IsMoreMultiplierMod(mod)) {
		PlayerModData[pnum].value[mod] += val;
	}
	else if(!PlayerModData[pnum].value[mod]) {
		// if we are zero, simply replace with val
		PlayerModData[pnum].value[mod] = val;
	}
	else if(val > 0) {
		// non-zero, multiply case -- we store things like 0.2 etc. here, but while we amplify it we need to consider 1.0 + val
		PlayerModData[pnum].value[mod] = CombineMultiplicativeFactors(PlayerModData[pnum].value[mod], val) - 1.0;
	}
	else if(val < 0) {
		// if negative we divide
		// if mod value == val, this means we need to set to zero (it's removed), otherwise just divide it
		if(PlayerModData[pnum].value[mod] + val < EPSILON)
			PlayerModData[pnum].value[mod] = 0;
		else
			PlayerModData[pnum].value[mod] = CancelMultiplicativeFactors(PlayerModData[pnum].value[mod], -val) - 1.0;
	}

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
	// check if it's a "more" multiplier, they are multiplicative with each other
	if(!IsMoreMultiplierMod(mod)) {
		PlayerModData[pnum].extra[mod] += val;
	}
	else if(!PlayerModData[pnum].extra[mod]) {
		// if we are zero, simply replace with val
		PlayerModData[pnum].extra[mod] = val;
	}
	else if(val > 0) {
		// non-zero, multiply case -- we store things like 0.2 etc. here, but while we amplify it we need to consider 1.0 + val
		PlayerModData[pnum].extra[mod] = CombineMultiplicativeFactors(PlayerModData[pnum].extra[mod], val) - 1.0;
	}
	else if(val < 0) {
		// if negative we divide
		// if mod value == val, this means we need to set to zero (it's removed), otherwise just divide it
		if(PlayerModData[pnum].extra[mod] + val < EPSILON)
			PlayerModData[pnum].extra[mod] = 0;
		else
			PlayerModData[pnum].extra[mod] = CancelMultiplicativeFactors(PlayerModData[pnum].extra[mod], -val) - 1.0;
	}
	
	PushToPlayerAttributeExtraSync(pnum, mod);
}

void ResetPlayerModList(int pnum) {
	for(int i = 0; i < MAX_TOTAL_ATTRIBUTES; ++i) {
		PlayerModData[pnum].value[i] = 0;
		PlayerModData[pnum].extra[i] = 0;
	}

	// The highest source rows are not derived from value[] and have to be cleared with it, or a
	// character reload leaves phantom sources behind that nothing will ever remove.
	for(i = 0; i < DND_HIGHESTSRC_COUNT; ++i)
		for(int j = 0; j < DND_MAX_HIGHESTSRC_SLOTS; ++j)
			PlayerModData[pnum].highest_sources[i][j] = 0;

	// Damage conversion accumulates outside PlayerModData -- one summed attribute cannot tell two
	// conversion mods apart when each names its own source and destination -- so it has to be reset
	// alongside it or a character reload doubles everything the player is wearing.
	ResetPlayerConversionTable(pnum);

	ACS_NamedExecuteWithResult("DnD Reset Player Mod List", pnum);
}

void SyncPlayerItemMods(int pnum) {
	for(int i = 0; i < MAX_TOTAL_ATTRIBUTES; ++i) {
		if(PlayerModData[pnum].value[i])
			ACS_NamedExecuteWithResult("DnD Request Mod Sync", pnum, i, PlayerModData[pnum].value[i]);
		if(PlayerModData[pnum].extra[i])
			ACS_NamedExecuteWithResult("DnD Request Mod Extra Sync", pnum, i, PlayerModData[pnum].extra[i]);
	}
}

// resets things clientside for the array
Script "DnD Reset Player Mod List" (int pnum) CLIENTSIDE {
	for(int i = 0; i < MAX_TOTAL_ATTRIBUTES; ++i) {
		PlayerModData[pnum].value[i] = 0;
		PlayerModData[pnum].extra[i] = 0;
	}

	// The highest source rows are not derived from value[] and have to be cleared with it, or a
	// character reload leaves phantom sources behind that nothing will ever remove.
	for(i = 0; i < DND_HIGHESTSRC_COUNT; ++i)
		for(int j = 0; j < DND_MAX_HIGHESTSRC_SLOTS; ++j)
			PlayerModData[pnum].highest_sources[i][j] = 0;
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
