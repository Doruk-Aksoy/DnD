#ifndef DND_INV_ATTRIBS_IN
#define DND_INV_ATTRIBS_IN

#include "DnD_InvInfo.h"
#include "DnD_ItemBase.h"
#include "../DnD_WeaponDefs.h"

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

#define DND_INC_SINGLEPROJ_NEGDMG 0.85
#define DND_INC_TWOPROJ_NEGDMG 0.7

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

	// shorthands
	INV_ATTR_TAG_ALL = 	INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_LIFE | INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_UTILITY |
						INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_EXPLOSIVE | INV_ATTR_TAG_OCCULT | INV_ATTR_TAG_CRIT | INV_ATTR_TAG_STAT |
						INV_ATTR_TAG_PHYSICAL | INV_ATTR_TAG_ENERGY | INV_ATTR_TAG_MELEE | INV_ATTR_TAG_FIRE | INV_ATTR_TAG_ICE |
						INV_ATTR_TAG_POISON | INV_ATTR_TAG_LIGHTNING | INV_ATTR_TAG_STAMINA | INV_ATTR_TAG_FLASK | INV_ATTR_TAG_ARMOR |
						INV_ATTR_TAG_ESHIELD | INV_ATTR_TAG_MITIGATION,

	// bodyarmors
	INV_ATTR_TAG_OK_BODYARMOR = INV_ATTR_TAG_LIFE | INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_OCCULT | INV_ATTR_TAG_STAT | 
								INV_ATTR_TAG_PHYSICAL | INV_ATTR_TAG_ENERGY | INV_ATTR_TAG_FIRE | INV_ATTR_TAG_ICE | INV_ATTR_TAG_POISON |
								INV_ATTR_TAG_LIGHTNING,

	INV_ATTR_TAG_EXCLUDED_BODYARMOR = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_CRIT | INV_ATTR_TAG_MELEE | INV_ATTR_TAG_FLASK,

	// helms
	INV_ATTR_TAG_OK_HELM = 	INV_ATTR_TAG_LIFE | INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_OCCULT | INV_ATTR_TAG_STAT | 
							INV_ATTR_TAG_PHYSICAL | INV_ATTR_TAG_ENERGY | INV_ATTR_TAG_FIRE | INV_ATTR_TAG_ICE | INV_ATTR_TAG_POISON |
							INV_ATTR_TAG_LIGHTNING,

	INV_ATTR_TAG_EXCLUDED_HELM = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_FLASK,

	// boots
	INV_ATTR_TAG_OK_BOOT = 	INV_ATTR_TAG_LIFE | INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_OCCULT | INV_ATTR_TAG_STAT | 
							INV_ATTR_TAG_PHYSICAL | INV_ATTR_TAG_ENERGY | INV_ATTR_TAG_FIRE | INV_ATTR_TAG_ICE | INV_ATTR_TAG_POISON |
							INV_ATTR_TAG_LIGHTNING | INV_ATTR_TAG_STAMINA,
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

	// these arent "real" tags, but more like sub tags --- dont add them to the below group otherwise they get grouped in their own tag group, which doesnt make sense for these
	INV_ATTR_TAG_STAMINA_ID,
	INV_ATTR_TAG_FLASK_ID,
	INV_ATTR_TAG_ARMOR_ID,
	INV_ATTR_TAG_ESHIELD_ID,
	INV_ATTR_TAG_MITIGATION_ID,
};
#define DND_ATTRIB_TAG_ID_BEGIN INV_ATTR_TAG_DAMAGE_ID
#define DND_ATTRIB_TAG_ID_END INV_ATTR_TAG_LIGHTNING_ID
#define MAX_ATTRIB_TAG_GROUPS (DND_ATTRIB_TAG_ID_END + 1)
#define MAX_ATTRIB_TAG_GROUPCOUNT 64

// indexing on this one is done by checking ranges, and then mapping appropriately
global int 8: AttributeTagGroups[MAX_ATTRIB_TAG_GROUPS][MAX_CRAFTABLEITEMTYPES][MAX_ATTRIB_TAG_GROUPCOUNT];
global int 5: AttributeTagGroupCount[MAX_ATTRIB_TAG_GROUPS][MAX_CRAFTABLEITEMTYPES];

typedef struct {
	int extra[MAX_TOTAL_ATTRIBUTES];
	int value[MAX_TOTAL_ATTRIBUTES];
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
		case INV_CORR_SPEED:

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
			case INV_CRITPERCENT_FORWEPTYPE:
				// pick one from a weapon the player owns
				if(pnum != MAXPLAYERS)
					res = PickRandomOwnedWeaponID(pnum);
				else
					res = random(FIRST_SLOT0_WEAPON, LAST_SLOT9_WEAPON);
			break;

			// mods that have natural extra values
			case INV_ESS_VAAJ:
			case INV_INC_DOUBLEHPBONUS:
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
					res = PickRandomOwnedWeaponID_WithProj(pnum) << 16;
				else
					res = random(FIRST_SLOT0_WEAPON, LAST_SLOT9_WEAPON) << 16;
				
				// corruption has no reduced damage
				if(mod != INV_CORR_WEAPONPLUSPROJ)
					res |=  DND_INC_SINGLEPROJ_NEGDMG;
			break;
			case INV_INC_PLUSTWOPROJ:
				if(pnum != MAXPLAYERS)
					res = (PickRandomOwnedWeaponID_WithProj(pnum) << 16);
				else
					res = random(FIRST_SLOT0_WEAPON, LAST_SLOT9_WEAPON) << 16;
				res |= DND_INC_TWOPROJ_NEGDMG;
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

bool IsTagArmorException(int tag, int armor_type) {
	// generic stamina rule -- only allowed on boots or berserker item
	if(armor_type != DND_CRAFTABLEID_BOOT && armor_type != DND_CRAFTABLEID_SPECIALTY_BERSERKER && (tag & INV_ATTR_TAG_STAMINA))
		return true;

	if(armor_type == DND_CRAFTABLEID_BODYARMOR || armor_type == DND_CRAFTABLEID_BOOT)
		return tag & (INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_CRIT | INV_ATTR_TAG_MELEE | INV_ATTR_TAG_FLASK);
	else if(armor_type == DND_CRAFTABLEID_HELM)
		return tag & (INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_FLASK);
	else if(armor_type == DND_CRAFTABLEID_SPECIALTY_WANDERER)
		return tag & (INV_ATTR_TAG_PHYSICAL | INV_ATTR_TAG_FLASK);
	else if(armor_type == DND_CRAFTABLEID_SPECIALTY_BERSERKER)
		return tag & (INV_ATTR_TAG_EXPLOSIVE | INV_ATTR_TAG_ENERGY);
	else if(armor_type == DND_CRAFTABLEID_SPECIALTY_TRICKSTER)
		return tag & (INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_LIFE | INV_ATTR_TAG_FLASK);

	// generic cant have occult
	return tag & (INV_ATTR_TAG_OCCULT | INV_ATTR_TAG_FLASK);
}

bool IsAttributeArmorException(int attr, int armor_type) {
	return IsTagArmorException(ItemModTable[attr].tags, armor_type);
}

// given an item type, return the generic attribute tags its allowed to roll
int GetItemBaseAllowedTags(int item_type) {
	switch(item_type) {
		case DND_ITEM_CHARM:
		return INV_ATTR_TAG_ALL;

		case DND_ITEM_HELM:
		return 0;
		case DND_ITEM_BOOT:
		return 0;
		case DND_ITEM_BODYARMOR:
		return 0;

		case DND_ITEM_FLASK:
		return 0;

		case DND_ITEM_SPECIALTY_DOOMGUY:
		return 0;
		case DND_ITEM_SPECIALTY_MARINE:
		return 0;
		case DND_ITEM_SPECIALTY_HOBO:
		return 0;
		case DND_ITEM_SPECIALTY_PUNISHER:
		return 0;
		case DND_ITEM_SPECIALTY_WANDERER:
		return 0;
		case DND_ITEM_SPECIALTY_CYBORG:
		return 0;
		case DND_ITEM_SPECIALTY_BERSERKER:
		return 0;
		case DND_ITEM_SPECIALTY_TRICKSTER:
		return 0;
	}

	return 0;
}

void SetupInventoryTagGroups() {
	// charms can roll every mod, so we iterate and build a table for them as opposed to by hand
	// move each groups tags looping through all attribs, extracting their bits
	int i, j;
	int tag, tag_id;
	// init to 0
	for(j = 0; j < MAX_ATTRIB_TAG_GROUPS; ++j) {
		for(i = 0; i < MAX_CRAFTABLEITEMTYPES; ++i)
			AttributeTagGroupCount[j][i] = 0;
	}
	
	// scan all attribs and place them in appropriate slots for charms
	for(i = FIRST_INV_ATTRIBUTE; i <= LAST_INV_ATTRIBUTE; ++i) {
		tag = ItemModTable[i].tags;
		tag_id = 0;
		// can have multiple
		while(tag > 0) {
			if(tag & 1) {
				AttributeTagGroups[tag_id][DND_CRAFTABLEID_CHARM][AttributeTagGroupCount[tag_id][DND_CRAFTABLEID_CHARM]] = i;
				++AttributeTagGroupCount[tag_id][DND_CRAFTABLEID_CHARM];
				//Log(s:"tag ", d:tag_id, s: " attr: ", d:i);
			}
			tag >>= 1;
			++tag_id;
		}
	}

	for(i = FIRST_INV_ATTRIBUTE; i <= LAST_INV_ATTRIBUTE; ++i) {
		for(j = DND_CRAFTABLEID_BODYARMOR; j < MAX_CRAFTABLEITEMTYPES; ++j) {
			tag = ItemModTable[i].tags;

			if(IsTagArmorException(tag, j))
				continue;

			tag_id = 0;
			// can have multiple
			while(tag > 0) {
				if(tag & 1) {
					AttributeTagGroups[tag_id][j][AttributeTagGroupCount[tag_id][j]] = i;
					++AttributeTagGroupCount[tag_id][j];
				}
				tag >>= 1;
				++tag_id;
			}
		}
	}
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
