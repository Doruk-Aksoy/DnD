#ifndef DND_INV_ATTRIBS_IN
#define DND_INV_ATTRIBS_IN

#include "DnD_InvInfo.h"
#include "DnD_WeaponDefs.h"

#define MAX_CHARM_AFFIXTIERS 10
#define CHARM_ATTRIBLEVEL_SEPERATOR 10 // just leave this as is... its basically every 10 levels a new tier is named for it
#define MAX_ATTRIBUTE_TIERS 9

int GetItemTier(int level) {
	int res = level / CHARM_ATTRIBLEVEL_SEPERATOR;
	if(res > MAX_ATTRIBUTE_TIERS)
		res = MAX_ATTRIBUTE_TIERS;
	return res;
}

#define CHARMSTR_COLORCODE 0
#define CHARMSTR_TIERTAG 1

// level 100 = perfect
str Charm_Strings[MAX_CHARM_AFFIXTIERS][2] = {
	{ "\c[C8]", "DND_CHARMTIER1" },
	{ "\c[D5]", "DND_CHARMTIER2" },
	{ "\c[A3]", "DND_CHARMTIER3" },
	{ "\c[C3]", "DND_CHARMTIER4" },
	{ "\c[E9]", "DND_CHARMTIER5" },
	{ "\c[K9]", "DND_CHARMTIER6" },
	{ "\c[E2]", "DND_CHARMTIER7" },
	{ "\c[C5]", "DND_CHARMTIER8" },
	{ "\c[W9]", "DND_CHARMTIER9" },
	{ "\c[Y9]", "DND_CHARMTIER10" }
};

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

global attrib_sync_T 48: PlayerAttributeSyncs[MAXPLAYERS];

bool IsAttributeQueuedForSync(int pnum, int attrib_id) {
	int amt = PlayerAttributeSyncs[pnum].count;
	for(int i = 0; i < amt; ++i)
		if(PlayerAttributeSyncs[pnum].arr[i] == attrib_id)
			return true;
	return false;
}

bool IsAttributeExtraQueuedForSync(int pnum, int attrib_id) {
	int amt = PlayerAttributeSyncs[pnum].extras;
	for(int i = 0; i < amt; ++i)
		if(PlayerAttributeSyncs[pnum].arr_extra[i] == attrib_id)
			return true;
	return false;
}

void PushToPlayerAttributeSync(int pnum, int attrib_id) {
	if(PlayerAttributeSyncs[pnum].count < DND_MAX_ATTRIBUTE_SYNC_ELEMS && !IsAttributeQueuedForSync(pnum, attrib_id))
		PlayerAttributeSyncs[pnum].arr[PlayerAttributeSyncs[pnum].count++] = attrib_id;
}

void PushToPlayerAttributeExtraSync(int pnum, int attrib_id) {
	if(PlayerAttributeSyncs[pnum].extras < DND_MAX_ATTRIBUTE_SYNC_ELEMS && !IsAttributeExtraQueuedForSync(pnum, attrib_id))
		PlayerAttributeSyncs[pnum].arr_extra[PlayerAttributeSyncs[pnum].extras++] = attrib_id;
}

void ClearPlayerAttributeSync(int pnum) {
	int amt = PlayerAttributeSyncs[pnum].count;
	for(int i = 0; i < amt; ++i)
		PlayerAttributeSyncs[pnum].arr[i] = -1;
	PlayerAttributeSyncs[pnum].count = 0;
}

void ClearPlayerAttributeExtraSync(int pnum) {
	int amt = PlayerAttributeSyncs[pnum].extras;
	for(int i = 0; i < amt; ++i)
		PlayerAttributeSyncs[pnum].arr_extra[i] = -1;
	PlayerAttributeSyncs[pnum].extras = 0;
}

//#define MAX_ATTRIB_MODIFIER 0xFFFFFFFF

#define UNSTABLE_DMG_MULT 5 // x5 of chance as damage
#define UNSTABLE_PROC_PCT 0.05 // 5%

// these two imply we have 1000 regular charm mods potentially, 1000 essence and unlimited unique mods
// reason we have these generous ranges is so when a new mod is added, database resets do not need to happen
#define CORRUPT_ATTRIB_ID_BEGIN 1000
#define IMPLICIT_ATTRIB_ID_BEGIN 1250
#define ESSENCE_ATTRIB_ID_BEGIN 1500
#define INCURSION_ATTRIB_ID_BEGIN 2000
#define UNIQUE_ATTRIB_ID_BEGIN 2500
// self note: all status_buffs_X modifiers are handled as exceptions
enum {
	INV_HP_INCREASE,
	INV_ARMOR_INCREASE,
	INV_HPPERCENT_INCREASE,
	INV_ARMORPERCENT_INCREASE,
	INV_EXPGAIN_INCREASE,
	INV_CREDITGAIN_INCREASE,
	INV_DROPCHANCE_INCREASE,
	INV_LUCK_INCREASE,
	INV_AMMOCAP_INCREASE,
	INV_SPEED_INCREASE,
	INV_MAGAZINE_INCREASE,
	
	INV_FLATPHYS_DAMAGE,
	INV_FLATENERGY_DAMAGE,
	INV_FLATRADIUS_DAMAGE,
	INV_FLATMAGIC_DAMAGE,
	INV_FLATELEM_DAMAGE,
	
	INV_PERCENTPHYS_DAMAGE,
	INV_PERCENTENERGY_DAMAGE,
	INV_PERCENTRADIUS_DAMAGE,
	INV_PERCENTMAGIC_DAMAGE,
	INV_PERCENTELEM_DAMAGE,
	
	INV_HANDGUN_PERCENT,
	INV_SHOTGUN_PERCENT,
	INV_AUTOMATIC_PERCENT,
	INV_ARTILLERY_PERCENT,
	INV_PRECISION_PERCENT,

	INV_FLAT_HANDGUN,
	INV_FLAT_SHOTGUN,
	INV_FLAT_AUTOMATIC,
	INV_FLAT_ARTILLERY,

	INV_PELLET_INCREASE,
	
	INV_EXPLOSION_RADIUS,
	INV_SELFDMG_RESIST,
	
	INV_AMMOGAIN_CHANCE,
	INV_AMMOGAIN_INCREASE,
	INV_SHOPSTOCK_INCREASE,
	
	INV_REGENCAP_INCREASE,
	
	INV_CRITCHANCE_INCREASE,
	INV_CRITPERCENT_INCREASE,
	INV_CRITDAMAGE_INCREASE,
	
	INV_KNOCKBACK_RESIST,
	INV_DAMAGEPERCENT_MORE,
	INV_ACCURACY_INCREASE,
	
	INV_STAT_STRENGTH,
	INV_STAT_DEXTERITY,
	INV_STAT_INTELLECT,
	
	INV_DMGREDUCE_ELEM,
	INV_DMGREDUCE_PHYS,
	INV_DMGREDUCE_REFL,
	
	INV_PEN_PHYSICAL,
	INV_PEN_ENERGY,
	INV_CHANCE_IGNORERADIUSIMMUNITY,
	INV_PEN_OCCULT,
	INV_PEN_ELEMENTAL,
	
	INV_FLAT_FIREDMG,
	INV_FLAT_ICEDMG,
	INV_FLAT_LIGHTNINGDMG,
	INV_FLAT_POISONDMG,
	
	INV_LIFESTEAL,
	
	INV_POISON_TICRATE,
	INV_POISON_DURATION,
	INV_POISON_TICDMG,
	
	INV_BLOCKERS_MOREDMG,
	
	INV_FREEZECHANCE,
	INV_SLOWEFFECT,
	INV_CHILLTHRESHOLD,
	
	INV_IGNITECHANCE,
	INV_IGNITEDMG,
	INV_IGNITEDURATION,
	
	INV_OVERLOADCHANCE,
	INV_OVERLOAD_ZAPCOUNT,
	INV_OVERLOAD_DMGINCREASE,
	
	INV_CYBERNETIC,
	INV_MELEERANGE,
	INV_MELEEDAMAGE,
	INV_DOTMULTI,
	INV_INCREASEDDOT,
	
	INV_PROJSPEED,
	INV_DMGREDUCE_ENERGY,
	INV_PERCENT_KNOCKBACKRESIST,
	INV_DMGREDUCE_MAGIC,
	INV_DMGREDUCE_FIRE,
	INV_DMGREDUCE_ICE,
	INV_DMGREDUCE_LIGHTNING,
	INV_DMGREDUCE_POISON,
	
	INV_ADDEDMAXRESIST,
	INV_REGENRATE,
	
	INV_LIFESTEAL_RATE,
	INV_LIFESTEAL_RECOVERY,
	INV_LIFESTEAL_CAP,
	INV_LIFESTEAL_DAMAGE,
	
	INV_OVERLOAD_DURATION,
	
	INV_IGNITE_PROLIFCHANCE,
	INV_IGNITE_PROLIFCOUNT,
	INV_IGNITE_PROLIFRANGE,
	
	INV_CHANCE_AILMENTIGNORE,
	INV_CHANCE_FLATIGNITE,
	INV_CHANCE_FLATPROLIF,

	INV_SHIELD_INCREASE,
	INV_PERCENTSHIELD_INCREASE,
	INV_SHIELD_RECOVERYRATE,
	INV_SHIELD_RECHARGEDELAY,
	INV_MIT_INCREASE,
	INV_MITEFFECT_INCREASE,

	INV_PERCENTFIRE_DAMAGE,
	INV_PERCENTICE_DAMAGE,
	INV_PERCENTLIGHTNING_DAMAGE,
	INV_PERCENTPOISON_DAMAGE,

	INV_PEN_FIRE,
	INV_PEN_ICE,
	INV_PEN_LIGHTNING,
	INV_PEN_POISON,

	INV_MAGIC_NEGATION,

	INV_FLAT_TECH,
	INV_FLAT_PRECISION,
	INV_TECH_PERCENT,
	INV_FLAT_MAGIC,
	INV_MAGIC_PERCENT,

	INV_REDUCED_OVERHEAT,
	INV_DMGREDUCE_ALL,
	INV_ITEMRARITY,

	INV_RIPCOUNT,
	INV_RIPDAMAGE,
	INV_LOCKONAREA,
	INV_LOCKONRANGE,
	INV_INCKILLINGSPREE,
	INV_REDUCEDCURSEEFFECT,
	INV_REDUCEDCURSEDURATION,
	INV_AVOID_IGNITE,
	INV_AVOID_CHILLFREEZE,
	INV_AVOID_POISON,
	INV_AVOID_OVERLOAD,
	INV_AVOID_ELEAILMENTS,
	INV_AVOID_BLEED,

	INV_CHANCE_BLEED,
	INV_PERCENTDMG_BLEED,
	INV_BLEED_DURATION,
	// add new regular rollable attributes here

	// corrupted implicits -- add new ones here
	INV_CORR_WEAPONDMG = CORRUPT_ATTRIB_ID_BEGIN,
	INV_CORR_WEAPONCRIT,
	INV_CORR_WEAPONCRITDMG,
	INV_CORR_SPEED,
	INV_CORR_DROPCHANCE,
	INV_CORR_PERCENTSTATS,
	INV_CORR_WEAPONPCTDMG,
	INV_CORR_WEAPONPOISONPCT,
	INV_CORR_WEAPONFORCEPAIN,
	INV_CORR_CYBERNETIC,
	INV_CORR_DMGDOESNTSTOPREGEN,
	INV_CORR_INSTALEECHPCT,
	// add new corruption implicits here

	// implicits -- add new ones below here
	INV_IMP_INCARMOR = IMPLICIT_ATTRIB_ID_BEGIN,
	INV_IMP_INCSHIELD,
	INV_IMP_INCARMORSHIELD,
	INV_IMP_INCMIT,
	INV_IMP_INCMITARMOR,
	INV_IMP_INCMITSHIELD,
	INV_IMP_POWERCORE,
	INV_IMP_UNSTABLECORE,
	INV_IMP_INCMITARMORSHIELD,

	INV_IMP_RAVAGER,					// ravager armor
	INV_IMP_DOUBLEESHIELDRECOVERY,		// 
	INV_IMP_ABSORBLIGHTNING,			// lightning coil
	INV_IMP_KNIGHTARMOR,
	INV_IMP_HANDGUNBONUS,
	INV_IMP_NECROARMOR,
	INV_IMP_CANROLL_PHYS,
	INV_IMP_CANROLL_MAGIC,
	INV_IMP_CANROLL_EXPLOSIVE,
	INV_IMP_CANROLL_ENERGY,
	INV_IMP_CANROLL_ELEMENTAL,
	INV_IMP_HIGHREFLECTREDUCE,			// synth metal
	INV_IMP_ESHIELDBLOCKSALL,
	INV_IMP_LESSLIGHTNINGTAKEN,
	INV_IMP_FASTEROVERHEATDISS,
	INV_IMP_LESSPOISONTAKEN,
	INV_IMP_LESSFIRETAKEN,
	INV_IMP_LESSSELFDAMAGETAKEN,
	INV_IMP_BONUSPETCAP,
	INV_IMP_REDUCEDVISIONIMPAIR,		// synthmetal mask
	INV_IMP_MELEEIGNORESSHIELDS,
	INV_IMP_MOREDAMAGETOBOSSES,
	INV_IMP_RECOVERESONUNDEADKILL,
	INV_IMP_PRECISIONCRITBONUS,
	INV_IMP_QUALITYCAPFIFTY,
	// add new implicits here
	
	// essence attributes (only via. specific means)
	INV_ESS_VAAJ = ESSENCE_ATTRIB_ID_BEGIN,
	INV_ESS_SSRATH,
	INV_ESS_OMNISIGHT,
	INV_ESS_OMNISIGHT2,
	INV_ESS_CHEGOVAX,
	INV_ESS_HARKIMONDE,
	INV_ESS_LESHRAC,
	INV_ESS_KRULL,
	INV_ESS_THORAX,
	INV_ESS_ZRAVOG,
	INV_ESS_ERYXIA,
	// add new essences here

	INV_INC_DOUBLEHPBONUS = INCURSION_ATTRIB_ID_BEGIN,
	INV_INC_MAXPOISONSTACK,
	INV_INC_POISONSPREAD,
	INV_INC_CRITFORDOT,
	INV_INC_ALLOVERLOAD,
	INV_INC_ESHIELDNOINTERRUPT,
	INV_INC_PLUSPROJ,
	INV_INC_PLUSTWOPROJ,
	INV_INC_INSTANTLIFESTEAL,
	INV_INC_TWICEARMORDEFENSE,
	INV_INC_MITIGATIONTODODGE,
	INV_INC_ACCURACYFORPRECISION,
	INV_INC_HPREGENINTERRUPT,
	INV_INC_PASSIVEREGEN,
	INV_INC_ENEMYRIPCHANCE,
	INV_INC_BLOCKPREVENTION,
	INV_INC_RIPPERSEXPLODE,
	INV_INC_INVERTRESISTANCES,
	INV_INC_ACCURACYREVERSED,
	INV_INC_PROJREVERSE,
	INV_INC_EXCESSCRIT,
	// add new incursion mods here
	
	// below here are exotic attributes not found in normal items, if you add new attributes do so to above and change MAX_INV_ATTRIBUTE_TYPES
	INV_EX_CHANCE_CASTELEMSPELLONATK = UNIQUE_ATTRIB_ID_BEGIN,
	INV_EX_KNOCKBACK_IMMUNITY,
	INV_EX_FACTOR_SMALLCHARM,
	INV_EX_ALLSTATS,
	INV_EX_CHANCE_HEALMISSINGONPAIN,
	INV_EX_DMGINCREASE_LIGHTNING,
	INV_EX_MORECRIT_LIGHTNING,
	INV_EX_SECONDEXPBONUS,
	INV_EX_DOUBLE_HEALTHCAP,
	INV_EX_PHYSDAMAGEPER_FLATHEALTH, // only 1%
	INV_EX_FORBID_ARMOR,
	INV_EX_BEHAVIOR_PELLETSFIRECIRCLE,
	INV_EX_FORSHOW_BURSTGETSPELLETBONUS,
	INV_EX_CHANCE_ONDEATH_RAISEZOMBIE,
	INV_EX_DMGREDUCE_SHAREWITHPETS,
	INV_EX_DMGINCREASE_TAKEN,
	INV_EX_FLATDMG_ALL,
	INV_EX_ONKILL_HEALMISSING,
	INV_EX_SOULWEPS_FULLDAMAGE,
	INV_EX_PICKUPS_MORESOUL,
	INV_EX_ABILITY_RALLY,
	INV_EX_BEHAVIOR_SPELLSFULLDAMAGE,
	INV_EX_ABILITY_MONSTERSRIP,
	INV_EX_FLATDOT,
	INV_EX_DOTDURATION,
	INV_EX_ABILITY_LUCKYCRIT,
	INV_EX_CRITIGNORERESCHANCE,
	INV_EX_CURSEIMMUNITY,
	INV_EX_LIMITEDSMALLCHARMS,
	INV_EX_FLATPERSHOTGUNOWNED,
	INV_EX_LESSHEALING,
	INV_EX_SOULWEPSPEN,
	INV_EX_DEADEYEBONUS,
	INV_EX_DAMAGPERMISSINGAMMO,
	INV_EX_MOREAMMOUSE,
	INV_EX_REDUCEDAMMOCAP,
	INV_EX_UNITY,
	INV_EX_UNITY_RES_BONUS,
	INV_EX_UNITY_PEN_BONUS,
	INV_EX_UNITY_NOBONUS,
	INV_EX_INTBONUSTOMELEE,
	INV_EX_STARTESONDEPLETE,
	INV_EX_ESCHARGE_DMGNOINTERRUPT,
	INV_EX_ESEXPLOSIONHPDMG,
	INV_EX_ESCHARGE_USEHP,
	INV_EX_HPTOESHIELD,
	INV_EX_ESHIELDFULLABSORB,
	INV_EX_HEALTHATONE,
	INV_EX_RESPERESHIELD,
	INV_EX_ESHIELDONLYBLOCKPCT,
	INV_EX_REFILLAMMOONMELEEKILL,
	INV_EX_SWAPFROMMELEECRIT,
	INV_EX_DAMAGELOWESTTAKENASPHYS,
	INV_EX_DEMONBARRIERS,
	INV_EX_SOULPICKUPSINFAMMO,
	INV_EX_SOULPICKUPSONLYAMMO,
	INV_EX_STREXTRABONUSTOMELEE,
	INV_EX_ELEPENHARMONY,
	INV_EX_CANFIREOVERHEATED,
	INV_EX_CANTFIRENONOVERHEAT,
	INV_EX_MOREDMGPEROVERHEAT,
	INV_EX_LESSDMGTAKENMAXOVERHEAT,
	INV_EX_WEAPONSUSEHEALTH,
	INV_EX_CANNOTPICKAMMO,
	INV_EX_AMMOCOSTMULTIPLIER,
	INV_EX_RIPPERSONETIMEONLY,
	INV_EX_RIPPERSRIPALL,
	INV_EX_MIRROROTHERMEDIUM,
	// add new unique attributes here
	INV_EX_PLAYERPOWERSET1, // holds certain powers that are just bitfields in one -- is not shown in item attrib list
};

#define DND_INC_ACCURACYFORPRECRATIO 25
#define DND_INC_BLOCKPREVENTIONTIME 10

#define DND_INC_SINGLEPROJ_NEGDMG 0.85
#define DND_INC_TWOPROJ_NEGDMG 0.7

#define DND_INC_POISONSPREAD_R 160.0
#define DND_INC_POISONSPREAD_COUNT 8

#define DND_INC_PASSIVEREGEN_REDUCEDLIFESTEAL 50

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

#define REDUCED_SELF_DMG_FACTOR 25 // 25%

// attributes below last_inv (normal rollables) are exotic
#define FIRST_INV_ATTRIBUTE INV_HP_INCREASE
#define LAST_INV_ATTRIBUTE INV_BLEED_DURATION
#define NORMAL_ATTRIBUTE_COUNT (LAST_INV_ATTRIBUTE - FIRST_INV_ATTRIBUTE + 1)
// modify the above to make it use the negative last
//#define NEGATIVE_ATTRIB_BEGIN INV_NEG_DAMAGE_DEALT
#define UNIQUE_ATTRIB_BEGIN INV_EX_CHANCE_CASTELEMSPELLONATK
#define UNIQUE_ATTRIB_END INV_EX_PLAYERPOWERSET1
#define UNIQUE_ATTRIB_COUNT (UNIQUE_ATTRIB_END - UNIQUE_ATTRIB_BEGIN + 1)

#define FIRST_CORRUPT_IMPLICIT INV_CORR_WEAPONDMG
#define LAST_CORRUPT_IMPLICIT INV_CORR_INSTALEECHPCT
#define MAX_CORRUPT_IMPLICITS (LAST_CORRUPT_IMPLICIT - FIRST_CORRUPT_IMPLICIT + 1)

#define FIRST_REGULAR_IMPLICIT INV_IMP_INCARMOR
#define LAST_REGULAR_IMPLICIT INV_IMP_QUALITYCAPFIFTY

#define FIRST_ESSENCE_ATTRIBUTE INV_ESS_VAAJ
#define LAST_ESSENCE_ATTRIBUTE INV_ESS_ERYXIA
#define ESSENCE_ATTRIBUTE_COUNT (LAST_ESSENCE_ATTRIBUTE - FIRST_ESSENCE_ATTRIBUTE + 1)
#define ESSENCE_MAP_MACRO(X) ((X) - FIRST_ESSENCE_ATTRIBUTE + 1)

#define FIRST_INCURSION_ATTRIBUTE INV_INC_DOUBLEHPBONUS
#define LAST_INCURSION_ATTRIBUTE INV_INC_EXCESSCRIT
#define INCURSION_ATTRIBUTE_COUNT (LAST_INCURSION_ATTRIBUTE - FIRST_INCURSION_ATTRIBUTE + 1)
#define INCURSION_MAP_MACRO(X) ((X) - FIRST_INCURSION_ATTRIBUTE + 1)

#define MAX_INV_ATTRIBUTE_TYPES (NORMAL_ATTRIBUTE_COUNT + ESSENCE_ATTRIBUTE_COUNT + INCURSION_ATTRIBUTE_COUNT)
#define UNIQUE_MAP_MACRO(X) ((X) - UNIQUE_ATTRIB_ID_BEGIN + 1)
#define MAX_TOTAL_ATTRIBUTES 3000 // 1000 for each of: regular, essence, unique mods.

typedef struct {
	int attrib_low;
	int attrib_high;
	int attrib_extra_low;
	int attrib_extra_high;
	int attrib_level_modifier;
	int attrib_level_extra_modifier;
	int tags;
} inv_attrib_T;

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
	INV_ATTR_TAG_LIGHTNING = 65536
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
	INV_ATTR_TAG_LIGHTNING_ID
};
#define DND_ATTRIB_TAG_ID_BEGIN INV_ATTR_TAG_DAMAGE_ID
#define DND_ATTRIB_TAG_ID_END INV_ATTR_TAG_LIGHTNING_ID
#define MAX_ATTRIB_TAG_GROUPS (DND_ATTRIB_TAG_ID_END + 1)
#define MAX_ATTRIB_TAG_GROUPCOUNT 64

// indexing on this one is done by checking ranges, and then mapping appropriately
global int 8: AttributeTagGroups[MAX_ATTRIB_TAG_GROUPS][MAX_CRAFTABLEITEMTYPES][MAX_ATTRIB_TAG_GROUPCOUNT];
global int 5: AttributeTagGroupCount[MAX_ATTRIB_TAG_GROUPS][MAX_CRAFTABLEITEMTYPES];
global int 57: PlayerModExtras[MAXPLAYERS][MAX_TOTAL_ATTRIBUTES];
global int 61: PlayerModValues[MAXPLAYERS][MAX_TOTAL_ATTRIBUTES];

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
		case INV_DAMAGEPERCENT_MORE:
		case INV_BLOCKERS_MOREDMG:
		case INV_PROJSPEED:
		case INV_DMGREDUCE_ELEM:
		case INV_DMGREDUCE_ENERGY:
		case INV_DMGREDUCE_PHYS:
		case INV_DMGREDUCE_MAGIC:
		case INV_DMGREDUCE_FIRE:
		case INV_DMGREDUCE_ICE:
		case INV_DMGREDUCE_LIGHTNING:
		case INV_DMGREDUCE_POISON:
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

		case INV_CORR_WEAPONDMG:
		case INV_CORR_DROPCHANCE:
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
int GetExtraForMod(int pnum, int mod, int tier = 0, int item_type = -1, int item_subtype = -1, bool isWellRolled = false) {
	int res = -1;
	switch(mod) {
		// extra is the weapon_id for these
		case INV_CORR_WEAPONDMG:
		case INV_CORR_WEAPONCRIT:
		case INV_CORR_WEAPONCRITDMG:
		case INV_CORR_WEAPONPCTDMG:
		case INV_CORR_WEAPONPOISONPCT:
		case INV_CORR_WEAPONFORCEPAIN:
			// pick one from a weapon the player owns
			res = PickRandomOwnedWeaponID(pnum);
		break;

		// mods that have natural extra values
		case INV_ESS_VAAJ:
		case INV_INC_DOUBLEHPBONUS:
		case INV_INC_PASSIVEREGEN:
		case INV_INC_INSTANTLIFESTEAL:
			res = RollAttributeExtra(mod, tier, isWellRolled, item_type, item_subtype);
		break;

		case INV_INC_PLUSPROJ:
			res = (PickRandomOwnedWeaponID_WithProj(pnum) << 16) | DND_INC_SINGLEPROJ_NEGDMG;
		break;
		case INV_INC_PLUSTWOPROJ:
			res = (PickRandomOwnedWeaponID_WithProj(pnum) << 16) | DND_INC_TWOPROJ_NEGDMG;
		break;
	}
	return res;
}

void SetPlayerModValue(int pnum, int mod, int val) {
	PlayerModValues[pnum][mod] = val;
	PushToPlayerAttributeSync(pnum, mod);
}

void SetPlayerModExtra(int pnum, int mod, int val) {
	//printbold(s:"mod: ", d:mod, s:" ", d:PlayerModValues[pnum][mod], s: " = ", d:val);
	PlayerModExtras[pnum][mod] = val;
	PushToPlayerAttributeExtraSync(pnum, mod);
}

void IncPlayerModValue(int pnum, int mod, int val) {
	//printbold(s:"mod: ", d:mod, s:" ", d:PlayerModValues[pnum][mod], s: " += ", d:val);
	// check if it's a "more" multiplier, they are multiplicative with each other
	if(!IsMoreMultiplierMod(mod)) {
		PlayerModValues[pnum][mod] += val;
	}
	else if(!PlayerModValues[pnum][mod]) {
		// if we are zero, simply replace with val
		PlayerModValues[pnum][mod] = val;
	}
	else if(val > 0) {
		// non-zero, multiply case -- we store things like 0.2 etc. here, but while we amplify it we need to consider 1.0 + val
		PlayerModValues[pnum][mod] = CombineMultiplicativeFactors(PlayerModValues[pnum][mod], val) - 1.0;
	}
	else if(val < 0) {
		// if negative we divide
		// if mod value == val, this means we need to set to zero (it's removed), otherwise just divide it
		if(PlayerModValues[pnum][mod] + val < EPSILON)
			PlayerModValues[pnum][mod] = 0;
		else
			PlayerModValues[pnum][mod] = CancelMultiplicativeFactors(PlayerModValues[pnum][mod], -val) - 1.0;
	}

	PushToPlayerAttributeSync(pnum, mod);
}

void IncPlayerModExtra(int pnum, int mod, int val) {
	//printbold(s:"mod: ", d:mod, s:" ", d:PlayerModValues[pnum][mod], s: " += ", d:val);
	// check if it's a "more" multiplier, they are multiplicative with each other
	if(!IsMoreMultiplierMod(mod)) {
		PlayerModExtras[pnum][mod] += val;
	}
	else if(!PlayerModExtras[pnum][mod]) {
		// if we are zero, simply replace with val
		PlayerModExtras[pnum][mod] = val;
	}
	else if(val > 0) {
		// non-zero, multiply case -- we store things like 0.2 etc. here, but while we amplify it we need to consider 1.0 + val
		PlayerModExtras[pnum][mod] = CombineMultiplicativeFactors(PlayerModExtras[pnum][mod], val) - 1.0;
	}
	else if(val < 0) {
		// if negative we divide
		// if mod value == val, this means we need to set to zero (it's removed), otherwise just divide it
		if(PlayerModExtras[pnum][mod] + val < EPSILON)
			PlayerModExtras[pnum][mod] = 0;
		else
			PlayerModExtras[pnum][mod] = CancelMultiplicativeFactors(PlayerModExtras[pnum][mod], -val) - 1.0;
	}
	
	PushToPlayerAttributeExtraSync(pnum, mod);
}

void ResetPlayerModList(int pnum) {
	for(int i = 0; i < MAX_TOTAL_ATTRIBUTES; ++i) {
		PlayerModValues[pnum][i] = 0;
		PlayerModExtras[pnum][i] = 0;
	}
	ACS_NamedExecuteWithResult("DnD Reset Player Mod List", pnum);
}

void SyncPlayerItemMods(int pnum) {
	for(int i = 0; i < MAX_TOTAL_ATTRIBUTES; ++i) {
		if(PlayerModValues[pnum][i])
			ACS_NamedExecuteWithResult("DnD Request Mod Sync", pnum, i, PlayerModValues[pnum][i]);
		if(PlayerModExtras[pnum][i])
			ACS_NamedExecuteWithResult("DnD Request Mod Extra Sync", pnum, i, PlayerModExtras[pnum][i]);
	}
}

// resets things clientside for the array
Script "DnD Reset Player Mod List" (int pnum) CLIENTSIDE {
	for(int i = 0; i < MAX_TOTAL_ATTRIBUTES; ++i) {
		PlayerModValues[pnum][i] = 0;
		PlayerModExtras[pnum][i] = 0;
	}
}

// reason why this uses UNIQUE_ATTRIB_ID_BEGIN, it skips regular and essence mod indexes. This means, we have enough room without database reset for both regular
// and essence mods. This is good for future compatibility as well.
// Currently: 2000 mods, 0-1999
global inv_attrib_T 62: ItemModTable[UNIQUE_ATTRIB_ID_BEGIN];

void SetupInventoryAttributeTable() {
	// doing this so we populate the global arrays (acs wont allow just initializing them like local variables)
	// and these are global so we move from stack to heap and also only initialize this once when server is launched
	// and never again as opposed to start of every map with local variable method
	ItemModTable[INV_HP_INCREASE].attrib_low = 5;
	ItemModTable[INV_HP_INCREASE].attrib_high = 14;
	ItemModTable[INV_HP_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_HP_INCREASE].tags = INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_ARMOR_INCREASE].attrib_low = 5;
	ItemModTable[INV_ARMOR_INCREASE].attrib_high = 24;
	ItemModTable[INV_ARMOR_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_ARMOR_INCREASE].tags = INV_ATTR_TAG_DEFENSE;
	
	ItemModTable[INV_HPPERCENT_INCREASE].attrib_low = 1;
	ItemModTable[INV_HPPERCENT_INCREASE].attrib_high = 6;
	ItemModTable[INV_HPPERCENT_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_HPPERCENT_INCREASE].tags = INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_ARMORPERCENT_INCREASE].attrib_low = 1;
	ItemModTable[INV_ARMORPERCENT_INCREASE].attrib_high = 9;
	ItemModTable[INV_ARMORPERCENT_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_ARMORPERCENT_INCREASE].tags = INV_ATTR_TAG_DEFENSE;
	
	ItemModTable[INV_EXPGAIN_INCREASE].attrib_low = 0.02;
	ItemModTable[INV_EXPGAIN_INCREASE].attrib_high = 0.04;
	ItemModTable[INV_EXPGAIN_INCREASE].attrib_level_modifier = 0.03;
	ItemModTable[INV_EXPGAIN_INCREASE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_CREDITGAIN_INCREASE].attrib_low = 0.02;
	ItemModTable[INV_CREDITGAIN_INCREASE].attrib_high = 0.04;
	ItemModTable[INV_CREDITGAIN_INCREASE].attrib_level_modifier = 0.03;
	ItemModTable[INV_CREDITGAIN_INCREASE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_DROPCHANCE_INCREASE].attrib_low = 0.025;
	ItemModTable[INV_DROPCHANCE_INCREASE].attrib_high = 0.05;
	ItemModTable[INV_DROPCHANCE_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_DROPCHANCE_INCREASE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_LUCK_INCREASE].attrib_low = 0.005;
	ItemModTable[INV_LUCK_INCREASE].attrib_high = 0.015;
	ItemModTable[INV_LUCK_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_LUCK_INCREASE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_AMMOCAP_INCREASE].attrib_low = 5;
	ItemModTable[INV_AMMOCAP_INCREASE].attrib_high = 14;
	ItemModTable[INV_AMMOCAP_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_AMMOCAP_INCREASE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_SPEED_INCREASE].attrib_low = 0.01;
	ItemModTable[INV_SPEED_INCREASE].attrib_high = 0.03;
	ItemModTable[INV_SPEED_INCREASE].attrib_level_modifier = 0.03;
	ItemModTable[INV_SPEED_INCREASE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_MAGAZINE_INCREASE].attrib_low = 1;
	ItemModTable[INV_MAGAZINE_INCREASE].attrib_high = 10;
	ItemModTable[INV_MAGAZINE_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_MAGAZINE_INCREASE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_FLATPHYS_DAMAGE].attrib_low = 1;
	ItemModTable[INV_FLATPHYS_DAMAGE].attrib_high = 3;
	ItemModTable[INV_FLATPHYS_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_FLATPHYS_DAMAGE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_PHYSICAL;
	
	ItemModTable[INV_FLATENERGY_DAMAGE].attrib_low = 1;
	ItemModTable[INV_FLATENERGY_DAMAGE].attrib_high = 3;
	ItemModTable[INV_FLATENERGY_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_FLATENERGY_DAMAGE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ENERGY;
	
	ItemModTable[INV_FLATRADIUS_DAMAGE].attrib_low = 1;
	ItemModTable[INV_FLATRADIUS_DAMAGE].attrib_high = 2;
	ItemModTable[INV_FLATRADIUS_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_FLATRADIUS_DAMAGE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_EXPLOSIVE;
	
	ItemModTable[INV_FLATMAGIC_DAMAGE].attrib_low = 1;
	ItemModTable[INV_FLATMAGIC_DAMAGE].attrib_high = 3;
	ItemModTable[INV_FLATMAGIC_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_FLATMAGIC_DAMAGE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_OCCULT;
	
	ItemModTable[INV_FLATELEM_DAMAGE].attrib_low = 1;
	ItemModTable[INV_FLATELEM_DAMAGE].attrib_high = 3;
	ItemModTable[INV_FLATELEM_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_FLATELEM_DAMAGE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_PERCENTPHYS_DAMAGE].attrib_low = 5;
	ItemModTable[INV_PERCENTPHYS_DAMAGE].attrib_high = 15;
	ItemModTable[INV_PERCENTPHYS_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTPHYS_DAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_PHYSICAL;
	
	ItemModTable[INV_PERCENTENERGY_DAMAGE].attrib_low = 5;
	ItemModTable[INV_PERCENTENERGY_DAMAGE].attrib_high = 15;
	ItemModTable[INV_PERCENTENERGY_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTENERGY_DAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_ENERGY;
	
	ItemModTable[INV_PERCENTRADIUS_DAMAGE].attrib_low = 2;
	ItemModTable[INV_PERCENTRADIUS_DAMAGE].attrib_high = 7;
	ItemModTable[INV_PERCENTRADIUS_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTRADIUS_DAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_EXPLOSIVE;
	
	ItemModTable[INV_PERCENTMAGIC_DAMAGE].attrib_low = 5;
	ItemModTable[INV_PERCENTMAGIC_DAMAGE].attrib_high = 15;
	ItemModTable[INV_PERCENTMAGIC_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTMAGIC_DAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_OCCULT;
	
	ItemModTable[INV_PERCENTELEM_DAMAGE].attrib_low = 5;
	ItemModTable[INV_PERCENTELEM_DAMAGE].attrib_high = 15;
	ItemModTable[INV_PERCENTELEM_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTELEM_DAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_ELEMENTAL;

	ItemModTable[INV_HANDGUN_PERCENT].attrib_low = 3;
	ItemModTable[INV_HANDGUN_PERCENT].attrib_high = 12;
	ItemModTable[INV_HANDGUN_PERCENT].attrib_level_modifier = 0;
	ItemModTable[INV_HANDGUN_PERCENT].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_SHOTGUN_PERCENT].attrib_low = 3;
	ItemModTable[INV_SHOTGUN_PERCENT].attrib_high = 12;
	ItemModTable[INV_SHOTGUN_PERCENT].attrib_level_modifier = 0;
	ItemModTable[INV_SHOTGUN_PERCENT].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_AUTOMATIC_PERCENT].attrib_low = 3;
	ItemModTable[INV_AUTOMATIC_PERCENT].attrib_high = 12;
	ItemModTable[INV_AUTOMATIC_PERCENT].attrib_level_modifier = 0;
	ItemModTable[INV_AUTOMATIC_PERCENT].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_ARTILLERY_PERCENT].attrib_low = 3;
	ItemModTable[INV_ARTILLERY_PERCENT].attrib_high = 12;
	ItemModTable[INV_ARTILLERY_PERCENT].attrib_level_modifier = 0;
	ItemModTable[INV_ARTILLERY_PERCENT].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_EXPLOSIVE;
	
	ItemModTable[INV_PRECISION_PERCENT].attrib_low = 3;
	ItemModTable[INV_PRECISION_PERCENT].attrib_high = 12;
	ItemModTable[INV_PRECISION_PERCENT].attrib_level_modifier = 0;
	ItemModTable[INV_PRECISION_PERCENT].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_FLAT_HANDGUN].attrib_low = 1;
	ItemModTable[INV_FLAT_HANDGUN].attrib_high = 3;
	ItemModTable[INV_FLAT_HANDGUN].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_HANDGUN].tags = INV_ATTR_TAG_ATTACK;
	
	ItemModTable[INV_FLAT_SHOTGUN].attrib_low = 1;
	ItemModTable[INV_FLAT_SHOTGUN].attrib_high = 3;
	ItemModTable[INV_FLAT_SHOTGUN].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_SHOTGUN].tags = INV_ATTR_TAG_ATTACK;
	
	ItemModTable[INV_FLAT_AUTOMATIC].attrib_low = 1;
	ItemModTable[INV_FLAT_AUTOMATIC].attrib_high = 3;
	ItemModTable[INV_FLAT_AUTOMATIC].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_AUTOMATIC].tags = INV_ATTR_TAG_ATTACK;
	
	ItemModTable[INV_FLAT_ARTILLERY].attrib_low = 1;
	ItemModTable[INV_FLAT_ARTILLERY].attrib_high = 3;
	ItemModTable[INV_FLAT_ARTILLERY].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_ARTILLERY].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_EXPLOSIVE;
	
	ItemModTable[INV_PELLET_INCREASE].attrib_low = 0.02;
	ItemModTable[INV_PELLET_INCREASE].attrib_high = 0.04;
	ItemModTable[INV_PELLET_INCREASE].attrib_level_modifier = 0.02;
	ItemModTable[INV_PELLET_INCREASE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ATTACK;
	
	ItemModTable[INV_EXPLOSION_RADIUS].attrib_low = 1;
	ItemModTable[INV_EXPLOSION_RADIUS].attrib_high = 8;
	ItemModTable[INV_EXPLOSION_RADIUS].attrib_level_modifier = 0;
	ItemModTable[INV_EXPLOSION_RADIUS].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_EXPLOSIVE;
	
	ItemModTable[INV_SELFDMG_RESIST].attrib_low = 1;
	ItemModTable[INV_SELFDMG_RESIST].attrib_high = 8;
	ItemModTable[INV_SELFDMG_RESIST].attrib_level_modifier = 0;
	ItemModTable[INV_SELFDMG_RESIST].tags = INV_ATTR_TAG_DEFENSE;
	
	ItemModTable[INV_AMMOGAIN_CHANCE].attrib_low = 4;
	ItemModTable[INV_AMMOGAIN_CHANCE].attrib_high = 8;
	ItemModTable[INV_AMMOGAIN_CHANCE].attrib_level_modifier = 0;
	ItemModTable[INV_AMMOGAIN_CHANCE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_AMMOGAIN_INCREASE].attrib_low = 5;
	ItemModTable[INV_AMMOGAIN_INCREASE].attrib_high = 10;
	ItemModTable[INV_AMMOGAIN_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_AMMOGAIN_INCREASE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_SHOPSTOCK_INCREASE].attrib_low = 1;
	ItemModTable[INV_SHOPSTOCK_INCREASE].attrib_high = 10;
	ItemModTable[INV_SHOPSTOCK_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_SHOPSTOCK_INCREASE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_REGENCAP_INCREASE].attrib_low = 1;
	ItemModTable[INV_REGENCAP_INCREASE].attrib_high = 4;
	ItemModTable[INV_REGENCAP_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_REGENCAP_INCREASE].tags = INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_CRITCHANCE_INCREASE].attrib_low = 0.005;
	ItemModTable[INV_CRITCHANCE_INCREASE].attrib_high = 0.008;
	ItemModTable[INV_CRITCHANCE_INCREASE].attrib_level_modifier = 0.004;
	ItemModTable[INV_CRITCHANCE_INCREASE].tags = INV_ATTR_TAG_CRIT;
	
	ItemModTable[INV_CRITPERCENT_INCREASE].attrib_low = 0.01;
	ItemModTable[INV_CRITPERCENT_INCREASE].attrib_high = 0.05;
	ItemModTable[INV_CRITPERCENT_INCREASE].attrib_level_modifier = 0.05;
	ItemModTable[INV_CRITPERCENT_INCREASE].tags = INV_ATTR_TAG_CRIT;
	
	ItemModTable[INV_CRITDAMAGE_INCREASE].attrib_low = 4;
	ItemModTable[INV_CRITDAMAGE_INCREASE].attrib_high = 9;
	ItemModTable[INV_CRITDAMAGE_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_CRITDAMAGE_INCREASE].tags = INV_ATTR_TAG_CRIT;
	
	ItemModTable[INV_KNOCKBACK_RESIST].attrib_low = 50;
	ItemModTable[INV_KNOCKBACK_RESIST].attrib_high = 500;
	ItemModTable[INV_KNOCKBACK_RESIST].attrib_level_modifier = 0;
	ItemModTable[INV_KNOCKBACK_RESIST].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_DAMAGEPERCENT_MORE].attrib_low = 0.01;
	ItemModTable[INV_DAMAGEPERCENT_MORE].attrib_high = 0.025;
	ItemModTable[INV_DAMAGEPERCENT_MORE].attrib_level_modifier = 0.015;
	ItemModTable[INV_DAMAGEPERCENT_MORE].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_ACCURACY_INCREASE].attrib_low = 25;
	ItemModTable[INV_ACCURACY_INCREASE].attrib_high = 124;
	ItemModTable[INV_ACCURACY_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_ACCURACY_INCREASE].tags = INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_STAT_STRENGTH].attrib_low = 1;
	ItemModTable[INV_STAT_STRENGTH].attrib_high = 4;
	ItemModTable[INV_STAT_STRENGTH].attrib_level_modifier = 0;
	ItemModTable[INV_STAT_STRENGTH].tags = INV_ATTR_TAG_STAT;
	
	ItemModTable[INV_STAT_DEXTERITY].attrib_low = 1;
	ItemModTable[INV_STAT_DEXTERITY].attrib_high = 4;
	ItemModTable[INV_STAT_DEXTERITY].attrib_level_modifier = 0;
	ItemModTable[INV_STAT_DEXTERITY].tags = INV_ATTR_TAG_STAT;
	
	ItemModTable[INV_STAT_INTELLECT].attrib_low = 1;
	ItemModTable[INV_STAT_INTELLECT].attrib_high = 4;
	ItemModTable[INV_STAT_INTELLECT].attrib_level_modifier = 0;
	ItemModTable[INV_STAT_INTELLECT].tags = INV_ATTR_TAG_STAT;
	
	ItemModTable[INV_DMGREDUCE_ELEM].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_ELEM].attrib_high = 5.0;
	ItemModTable[INV_DMGREDUCE_ELEM].attrib_level_modifier = 5.0;
	ItemModTable[INV_DMGREDUCE_ELEM].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_DMGREDUCE_PHYS].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_PHYS].attrib_high = 5.0;
	ItemModTable[INV_DMGREDUCE_PHYS].attrib_level_modifier = 5.0;
	ItemModTable[INV_DMGREDUCE_PHYS].tags = INV_ATTR_TAG_PHYSICAL;
	
	ItemModTable[INV_DMGREDUCE_REFL].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_REFL].attrib_high = 5.0;
	ItemModTable[INV_DMGREDUCE_REFL].attrib_level_modifier = 5.0;
	ItemModTable[INV_DMGREDUCE_REFL].tags = INV_ATTR_TAG_DEFENSE;

	ItemModTable[INV_PEN_PHYSICAL].attrib_low = 1;
	ItemModTable[INV_PEN_PHYSICAL].attrib_high = 4;
	ItemModTable[INV_PEN_PHYSICAL].attrib_level_modifier = 0;
	ItemModTable[INV_PEN_PHYSICAL].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_PHYSICAL;
	
	ItemModTable[INV_PEN_ENERGY].attrib_low = 1;
	ItemModTable[INV_PEN_ENERGY].attrib_high = 4;
	ItemModTable[INV_PEN_ENERGY].attrib_level_modifier = 0;
	ItemModTable[INV_PEN_ENERGY].tags = INV_ATTR_TAG_ATTACK	| INV_ATTR_TAG_ENERGY;
	
	ItemModTable[INV_CHANCE_IGNORERADIUSIMMUNITY].attrib_low = 1;
	ItemModTable[INV_CHANCE_IGNORERADIUSIMMUNITY].attrib_high = 5;
	ItemModTable[INV_CHANCE_IGNORERADIUSIMMUNITY].attrib_level_modifier = 0;
	ItemModTable[INV_CHANCE_IGNORERADIUSIMMUNITY].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_EXPLOSIVE;
	
	ItemModTable[INV_PEN_OCCULT].attrib_low = 1;
	ItemModTable[INV_PEN_OCCULT].attrib_high = 4;
	ItemModTable[INV_PEN_OCCULT].attrib_level_modifier = 0;
	ItemModTable[INV_PEN_OCCULT].tags = INV_ATTR_TAG_ATTACK	| INV_ATTR_TAG_OCCULT;
	
	ItemModTable[INV_PEN_ELEMENTAL].attrib_low = 1;
	ItemModTable[INV_PEN_ELEMENTAL].attrib_high = 3;
	ItemModTable[INV_PEN_ELEMENTAL].attrib_level_modifier = 0;
	ItemModTable[INV_PEN_ELEMENTAL].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL;

	ItemModTable[INV_FLAT_FIREDMG].attrib_low = 1;
	ItemModTable[INV_FLAT_FIREDMG].attrib_high = 4;
	ItemModTable[INV_FLAT_FIREDMG].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_FIREDMG].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_FIRE;
	
	ItemModTable[INV_FLAT_ICEDMG].attrib_low = 1;
	ItemModTable[INV_FLAT_ICEDMG].attrib_high = 4;
	ItemModTable[INV_FLAT_ICEDMG].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_ICEDMG].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_ICE;
	
	ItemModTable[INV_FLAT_LIGHTNINGDMG].attrib_low = 1;
	ItemModTable[INV_FLAT_LIGHTNINGDMG].attrib_high = 4;
	ItemModTable[INV_FLAT_LIGHTNINGDMG].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_LIGHTNINGDMG].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_LIGHTNING;
	
	ItemModTable[INV_FLAT_POISONDMG].attrib_low = 1;
	ItemModTable[INV_FLAT_POISONDMG].attrib_high = 4;
	ItemModTable[INV_FLAT_POISONDMG].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_POISONDMG].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_POISON;
	
	ItemModTable[INV_LIFESTEAL].attrib_low = 0.125;
	ItemModTable[INV_LIFESTEAL].attrib_high = 0.5;
	ItemModTable[INV_LIFESTEAL].attrib_level_modifier = 0.375;
	ItemModTable[INV_LIFESTEAL].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_POISON_TICRATE].attrib_low = 5;
	ItemModTable[INV_POISON_TICRATE].attrib_high = 14;
	ItemModTable[INV_POISON_TICRATE].attrib_level_modifier = 0;
	ItemModTable[INV_POISON_TICRATE].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_POISON;
	
	ItemModTable[INV_POISON_DURATION].attrib_low = 5;
	ItemModTable[INV_POISON_DURATION].attrib_high = 14;
	ItemModTable[INV_POISON_DURATION].attrib_level_modifier = 0;
	ItemModTable[INV_POISON_DURATION].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_POISON;
	
	ItemModTable[INV_POISON_TICDMG].attrib_low = 5;
	ItemModTable[INV_POISON_TICDMG].attrib_high = 20;
	ItemModTable[INV_POISON_TICDMG].attrib_level_modifier = 0;
	ItemModTable[INV_POISON_TICDMG].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_POISON;
	
	ItemModTable[INV_BLOCKERS_MOREDMG].attrib_low = 0.05;
	ItemModTable[INV_BLOCKERS_MOREDMG].attrib_high = 0.19;
	ItemModTable[INV_BLOCKERS_MOREDMG].attrib_level_modifier = 0.15;
	ItemModTable[INV_BLOCKERS_MOREDMG].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_FREEZECHANCE].attrib_low = 5;
	ItemModTable[INV_FREEZECHANCE].attrib_high = 9;
	ItemModTable[INV_FREEZECHANCE].attrib_level_modifier = 0;
	ItemModTable[INV_FREEZECHANCE].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_SLOWEFFECT].attrib_low = 2;
	ItemModTable[INV_SLOWEFFECT].attrib_high = 6;
	ItemModTable[INV_SLOWEFFECT].attrib_level_modifier = 0;
	ItemModTable[INV_SLOWEFFECT].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_ICE;
	
	ItemModTable[INV_CHILLTHRESHOLD].attrib_low = 1;
	ItemModTable[INV_CHILLTHRESHOLD].attrib_high = 4;
	ItemModTable[INV_CHILLTHRESHOLD].attrib_level_modifier = 0;
	ItemModTable[INV_CHILLTHRESHOLD].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_ICE;
	
	ItemModTable[INV_IGNITECHANCE].attrib_low = 5;
	ItemModTable[INV_IGNITECHANCE].attrib_high = 14;
	ItemModTable[INV_IGNITECHANCE].attrib_level_modifier = 0;
	ItemModTable[INV_IGNITECHANCE].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_FIRE;
	
	ItemModTable[INV_IGNITEDMG].attrib_low = 5;
	ItemModTable[INV_IGNITEDMG].attrib_high = 14;
	ItemModTable[INV_IGNITEDMG].attrib_level_modifier = 0;
	ItemModTable[INV_IGNITEDMG].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_FIRE;
	
	ItemModTable[INV_IGNITEDURATION].attrib_low = 4;
	ItemModTable[INV_IGNITEDURATION].attrib_high = 12;
	ItemModTable[INV_IGNITEDURATION].attrib_level_modifier = 0;
	ItemModTable[INV_IGNITEDURATION].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_FIRE;
	
	ItemModTable[INV_OVERLOADCHANCE].attrib_low = 5;
	ItemModTable[INV_OVERLOADCHANCE].attrib_high = 14;
	ItemModTable[INV_OVERLOADCHANCE].attrib_level_modifier = 0;
	ItemModTable[INV_OVERLOADCHANCE].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_LIGHTNING;
	
	ItemModTable[INV_OVERLOAD_ZAPCOUNT].attrib_low = 1;
	ItemModTable[INV_OVERLOAD_ZAPCOUNT].attrib_high = 1;
	ItemModTable[INV_OVERLOAD_ZAPCOUNT].attrib_level_modifier = 1;
	ItemModTable[INV_OVERLOAD_ZAPCOUNT].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_LIGHTNING;
	
	ItemModTable[INV_OVERLOAD_DMGINCREASE].attrib_low = 0.01;
	ItemModTable[INV_OVERLOAD_DMGINCREASE].attrib_high = 0.04;
	ItemModTable[INV_OVERLOAD_DMGINCREASE].attrib_level_modifier = 0.04;
	ItemModTable[INV_OVERLOAD_DMGINCREASE].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_LIGHTNING;
	
	ItemModTable[INV_CYBERNETIC].attrib_low = 1;
	ItemModTable[INV_CYBERNETIC].attrib_high = 1;
	ItemModTable[INV_CYBERNETIC].attrib_level_modifier = 1;
	ItemModTable[INV_CYBERNETIC].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_MELEERANGE].attrib_low = 5;
	ItemModTable[INV_MELEERANGE].attrib_high = 10;
	ItemModTable[INV_MELEERANGE].attrib_level_modifier = 0;
	ItemModTable[INV_MELEERANGE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_MELEE;
	
	ItemModTable[INV_MELEEDAMAGE].attrib_low = 5;
	ItemModTable[INV_MELEEDAMAGE].attrib_high = 15;
	ItemModTable[INV_MELEEDAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_MELEEDAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_MELEE;
	
	ItemModTable[INV_DOTMULTI].attrib_low = 1;
	ItemModTable[INV_DOTMULTI].attrib_high = 8;
	ItemModTable[INV_DOTMULTI].attrib_level_modifier = 0;
	ItemModTable[INV_DOTMULTI].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_INCREASEDDOT].attrib_low = 5;
	ItemModTable[INV_INCREASEDDOT].attrib_high = 15;
	ItemModTable[INV_INCREASEDDOT].attrib_level_modifier = 0;
	ItemModTable[INV_INCREASEDDOT].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_PROJSPEED].attrib_low = 0.01;
	ItemModTable[INV_PROJSPEED].attrib_high = 0.03;
	ItemModTable[INV_PROJSPEED].attrib_level_modifier = 0;
	ItemModTable[INV_PROJSPEED].tags = INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_ATTACK;
	
	ItemModTable[INV_DMGREDUCE_ENERGY].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_ENERGY].attrib_high = 5.0;
	ItemModTable[INV_DMGREDUCE_ENERGY].attrib_level_modifier = 5.0;
	ItemModTable[INV_DMGREDUCE_ENERGY].tags = INV_ATTR_TAG_ENERGY;
	
	ItemModTable[INV_PERCENT_KNOCKBACKRESIST].attrib_low = 1;
	ItemModTable[INV_PERCENT_KNOCKBACKRESIST].attrib_high = 10;
	ItemModTable[INV_PERCENT_KNOCKBACKRESIST].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENT_KNOCKBACKRESIST].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_DMGREDUCE_MAGIC].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_MAGIC].attrib_high = 5.0;
	ItemModTable[INV_DMGREDUCE_MAGIC].attrib_level_modifier = 5.0;
	ItemModTable[INV_DMGREDUCE_MAGIC].tags = INV_ATTR_TAG_OCCULT;
	
	ItemModTable[INV_DMGREDUCE_FIRE].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_FIRE].attrib_high = 5.0;
	ItemModTable[INV_DMGREDUCE_FIRE].attrib_level_modifier = 5.0;
	ItemModTable[INV_DMGREDUCE_FIRE].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_FIRE;
	
	ItemModTable[INV_DMGREDUCE_ICE].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_ICE].attrib_high = 5.0;
	ItemModTable[INV_DMGREDUCE_ICE].attrib_level_modifier = 5.0;
	ItemModTable[INV_DMGREDUCE_ICE].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_ICE;
	
	ItemModTable[INV_DMGREDUCE_LIGHTNING].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_LIGHTNING].attrib_high = 5.0;
	ItemModTable[INV_DMGREDUCE_LIGHTNING].attrib_level_modifier = 5.0;
	ItemModTable[INV_DMGREDUCE_LIGHTNING].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_LIGHTNING;
	
	ItemModTable[INV_DMGREDUCE_POISON].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_POISON].attrib_high = 5.0;
	ItemModTable[INV_DMGREDUCE_POISON].attrib_level_modifier = 5.0;
	ItemModTable[INV_DMGREDUCE_POISON].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_POISON;
	
	ItemModTable[INV_ADDEDMAXRESIST].attrib_low = 0.25;
	ItemModTable[INV_ADDEDMAXRESIST].attrib_high = 0.5;
	ItemModTable[INV_ADDEDMAXRESIST].attrib_level_modifier = 0.26;
	ItemModTable[INV_ADDEDMAXRESIST].tags = INV_ATTR_TAG_DEFENSE;
	
	ItemModTable[INV_REGENRATE].attrib_low = 5;
	ItemModTable[INV_REGENRATE].attrib_high = 10;
	ItemModTable[INV_REGENRATE].attrib_level_modifier = 0;
	ItemModTable[INV_REGENRATE].tags = INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_LIFESTEAL_RATE].attrib_low = 4;
	ItemModTable[INV_LIFESTEAL_RATE].attrib_high = 8;
	ItemModTable[INV_LIFESTEAL_RATE].attrib_level_modifier = 0;
	ItemModTable[INV_LIFESTEAL_RATE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_LIFESTEAL_RECOVERY].attrib_low = 5;
	ItemModTable[INV_LIFESTEAL_RECOVERY].attrib_high = 10;
	ItemModTable[INV_LIFESTEAL_RECOVERY].attrib_level_modifier = 0;
	ItemModTable[INV_LIFESTEAL_RECOVERY].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_LIFESTEAL_CAP].attrib_low = 5;
	ItemModTable[INV_LIFESTEAL_CAP].attrib_high = 10;
	ItemModTable[INV_LIFESTEAL_CAP].attrib_level_modifier = 0;
	ItemModTable[INV_LIFESTEAL_CAP].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_LIFESTEAL_DAMAGE].attrib_low = 0.01;
	ItemModTable[INV_LIFESTEAL_DAMAGE].attrib_high = 0.03;
	ItemModTable[INV_LIFESTEAL_DAMAGE].attrib_level_modifier = 0.025;
	ItemModTable[INV_LIFESTEAL_DAMAGE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_OVERLOAD_DURATION].attrib_low = 0.1;
	ItemModTable[INV_OVERLOAD_DURATION].attrib_high = 0.5;
	ItemModTable[INV_OVERLOAD_DURATION].attrib_level_modifier = 0.5;
	ItemModTable[INV_OVERLOAD_DURATION].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_LIGHTNING;
	
	ItemModTable[INV_IGNITE_PROLIFCHANCE].attrib_low = 5;
	ItemModTable[INV_IGNITE_PROLIFCHANCE].attrib_high = 14;
	ItemModTable[INV_IGNITE_PROLIFCHANCE].attrib_level_modifier = 0;
	ItemModTable[INV_IGNITE_PROLIFCHANCE].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_FIRE;
	
	ItemModTable[INV_IGNITE_PROLIFCOUNT].attrib_low = 1;
	ItemModTable[INV_IGNITE_PROLIFCOUNT].attrib_high = 1;
	ItemModTable[INV_IGNITE_PROLIFCOUNT].attrib_level_modifier = 1;
	ItemModTable[INV_IGNITE_PROLIFCOUNT].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_FIRE;
	
	ItemModTable[INV_IGNITE_PROLIFRANGE].attrib_low = 0.05;
	ItemModTable[INV_IGNITE_PROLIFRANGE].attrib_high = 0.14;
	ItemModTable[INV_IGNITE_PROLIFRANGE].attrib_level_modifier = 0.10;
	ItemModTable[INV_IGNITE_PROLIFRANGE].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_FIRE;
	
	ItemModTable[INV_CHANCE_AILMENTIGNORE].attrib_low = 5;
	ItemModTable[INV_CHANCE_AILMENTIGNORE].attrib_high = 9;
	ItemModTable[INV_CHANCE_AILMENTIGNORE].attrib_level_modifier = 0;
	ItemModTable[INV_CHANCE_AILMENTIGNORE].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_CHANCE_FLATIGNITE].attrib_low = 1;
	ItemModTable[INV_CHANCE_FLATIGNITE].attrib_high = 2;
	ItemModTable[INV_CHANCE_FLATIGNITE].attrib_level_modifier = 0;
	ItemModTable[INV_CHANCE_FLATIGNITE].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_FIRE;
	
	ItemModTable[INV_CHANCE_FLATPROLIF].attrib_low = 1;
	ItemModTable[INV_CHANCE_FLATPROLIF].attrib_high = 2;
	ItemModTable[INV_CHANCE_FLATPROLIF].attrib_level_modifier = 0;
	ItemModTable[INV_CHANCE_FLATPROLIF].tags = INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_FIRE;

	ItemModTable[INV_SHIELD_INCREASE].attrib_low = 1;
	ItemModTable[INV_SHIELD_INCREASE].attrib_high = 4;
	ItemModTable[INV_SHIELD_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_SHIELD_INCREASE].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ENERGY;

	ItemModTable[INV_PERCENTSHIELD_INCREASE].attrib_low = 1;
	ItemModTable[INV_PERCENTSHIELD_INCREASE].attrib_high = 9;
	ItemModTable[INV_PERCENTSHIELD_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTSHIELD_INCREASE].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ENERGY;

	ItemModTable[INV_SHIELD_RECOVERYRATE].attrib_low = 5;
	ItemModTable[INV_SHIELD_RECOVERYRATE].attrib_high = 9;
	ItemModTable[INV_SHIELD_RECOVERYRATE].attrib_level_modifier = 0;
	ItemModTable[INV_SHIELD_RECOVERYRATE].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ENERGY;

	ItemModTable[INV_SHIELD_RECHARGEDELAY].attrib_low = 5;
	ItemModTable[INV_SHIELD_RECHARGEDELAY].attrib_high = 10;
	ItemModTable[INV_SHIELD_RECHARGEDELAY].attrib_level_modifier = 0;
	ItemModTable[INV_SHIELD_RECHARGEDELAY].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ENERGY;

	ItemModTable[INV_MIT_INCREASE].attrib_low = 0.5;
	ItemModTable[INV_MIT_INCREASE].attrib_high = 1.25;
	ItemModTable[INV_MIT_INCREASE].attrib_level_modifier = 1.5;
	ItemModTable[INV_MIT_INCREASE].tags = INV_ATTR_TAG_DEFENSE;

	ItemModTable[INV_MITEFFECT_INCREASE].attrib_low = 0.15;
	ItemModTable[INV_MITEFFECT_INCREASE].attrib_high = 0.275;
	ItemModTable[INV_MITEFFECT_INCREASE].attrib_level_modifier = 0.16;
	ItemModTable[INV_MITEFFECT_INCREASE].tags = INV_ATTR_TAG_DEFENSE;

	ItemModTable[INV_PERCENTFIRE_DAMAGE].attrib_low = 5;
	ItemModTable[INV_PERCENTFIRE_DAMAGE].attrib_high = 15;
	ItemModTable[INV_PERCENTFIRE_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTFIRE_DAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_FIRE;

	ItemModTable[INV_PERCENTICE_DAMAGE].attrib_low = 5;
	ItemModTable[INV_PERCENTICE_DAMAGE].attrib_high = 15;
	ItemModTable[INV_PERCENTICE_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTICE_DAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_ICE;

	ItemModTable[INV_PERCENTPOISON_DAMAGE].attrib_low = 5;
	ItemModTable[INV_PERCENTPOISON_DAMAGE].attrib_high = 15;
	ItemModTable[INV_PERCENTPOISON_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTPOISON_DAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_POISON;

	ItemModTable[INV_PERCENTLIGHTNING_DAMAGE].attrib_low = 5;
	ItemModTable[INV_PERCENTLIGHTNING_DAMAGE].attrib_high = 15;
	ItemModTable[INV_PERCENTLIGHTNING_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTLIGHTNING_DAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_LIGHTNING;

	ItemModTable[INV_PEN_FIRE].attrib_low = 1;
	ItemModTable[INV_PEN_FIRE].attrib_high = 4;
	ItemModTable[INV_PEN_FIRE].attrib_level_modifier = 0;
	ItemModTable[INV_PEN_FIRE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_FIRE;

	ItemModTable[INV_PEN_ICE].attrib_low = 1;
	ItemModTable[INV_PEN_ICE].attrib_high = 4;
	ItemModTable[INV_PEN_ICE].attrib_level_modifier = 0;
	ItemModTable[INV_PEN_ICE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_ICE;

	ItemModTable[INV_PEN_LIGHTNING].attrib_low = 1;
	ItemModTable[INV_PEN_LIGHTNING].attrib_high = 4;
	ItemModTable[INV_PEN_LIGHTNING].attrib_level_modifier = 0;
	ItemModTable[INV_PEN_LIGHTNING].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_LIGHTNING;

	ItemModTable[INV_PEN_POISON].attrib_low = 1;
	ItemModTable[INV_PEN_POISON].attrib_high = 4;
	ItemModTable[INV_PEN_POISON].attrib_level_modifier = 0;
	ItemModTable[INV_PEN_POISON].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL | INV_ATTR_TAG_POISON;

	ItemModTable[INV_MAGIC_NEGATION].attrib_low = 1;
	ItemModTable[INV_MAGIC_NEGATION].attrib_high = 5;
	ItemModTable[INV_MAGIC_NEGATION].attrib_level_modifier = 0;
	ItemModTable[INV_MAGIC_NEGATION].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_OCCULT;

	ItemModTable[INV_FLAT_TECH].attrib_low = 1;
	ItemModTable[INV_FLAT_TECH].attrib_high = 3;
	ItemModTable[INV_FLAT_TECH].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_TECH].tags = INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_FLAT_PRECISION].attrib_low = 1;
	ItemModTable[INV_FLAT_PRECISION].attrib_high = 3;
	ItemModTable[INV_FLAT_PRECISION].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_PRECISION].tags = INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_TECH_PERCENT].attrib_low = 3;
	ItemModTable[INV_TECH_PERCENT].attrib_high = 12;
	ItemModTable[INV_TECH_PERCENT].attrib_level_modifier = 0;
	ItemModTable[INV_TECH_PERCENT].tags = INV_ATTR_TAG_DAMAGE;

	ItemModTable[INV_FLAT_MAGIC].attrib_low = 1;
	ItemModTable[INV_FLAT_MAGIC].attrib_high = 3;
	ItemModTable[INV_FLAT_MAGIC].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_MAGIC].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_OCCULT;

	ItemModTable[INV_MAGIC_PERCENT].attrib_low = 3;
	ItemModTable[INV_MAGIC_PERCENT].attrib_high = 12;
	ItemModTable[INV_MAGIC_PERCENT].attrib_level_modifier = 0;
	ItemModTable[INV_MAGIC_PERCENT].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_OCCULT;

	ItemModTable[INV_REDUCED_OVERHEAT].attrib_low = 1;
	ItemModTable[INV_REDUCED_OVERHEAT].attrib_high = 4;
	ItemModTable[INV_REDUCED_OVERHEAT].attrib_level_modifier = 0;
	ItemModTable[INV_REDUCED_OVERHEAT].tags = INV_ATTR_TAG_UTILITY;

	ItemModTable[INV_DMGREDUCE_ALL].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_ALL].attrib_high = 2.5;
	ItemModTable[INV_DMGREDUCE_ALL].attrib_level_modifier = 1.5;
	ItemModTable[INV_DMGREDUCE_ALL].tags = INV_ATTR_TAG_DEFENSE;

	ItemModTable[INV_ITEMRARITY].attrib_low = 0.015;
	ItemModTable[INV_ITEMRARITY].attrib_high = 0.035;
	ItemModTable[INV_ITEMRARITY].attrib_level_modifier = 0;
	ItemModTable[INV_ITEMRARITY].tags = INV_ATTR_TAG_UTILITY;

	ItemModTable[INV_RIPCOUNT].attrib_low = 5;
	ItemModTable[INV_RIPCOUNT].attrib_high = 15;
	ItemModTable[INV_RIPCOUNT].attrib_level_modifier = 0;
	ItemModTable[INV_RIPCOUNT].tags = INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_RIPDAMAGE].attrib_low = 1;
	ItemModTable[INV_RIPDAMAGE].attrib_high = 4;
	ItemModTable[INV_RIPDAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_RIPDAMAGE].tags = INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_LOCKONAREA].attrib_low = 5;
	ItemModTable[INV_LOCKONAREA].attrib_high = 10;
	ItemModTable[INV_LOCKONAREA].attrib_level_modifier = 0;
	ItemModTable[INV_LOCKONAREA].tags = INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_LOCKONRANGE].attrib_low = 5;
	ItemModTable[INV_LOCKONRANGE].attrib_high = 10;
	ItemModTable[INV_LOCKONRANGE].attrib_level_modifier = 0;
	ItemModTable[INV_LOCKONRANGE].tags = INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_INCKILLINGSPREE].attrib_low = 5;
	ItemModTable[INV_INCKILLINGSPREE].attrib_high = 15;
	ItemModTable[INV_INCKILLINGSPREE].attrib_level_modifier = 0;
	ItemModTable[INV_INCKILLINGSPREE].tags = INV_ATTR_TAG_UTILITY;

	ItemModTable[INV_REDUCEDCURSEEFFECT].attrib_low = 1;
	ItemModTable[INV_REDUCEDCURSEEFFECT].attrib_high = 3;
	ItemModTable[INV_REDUCEDCURSEEFFECT].attrib_level_modifier = 0;
	ItemModTable[INV_REDUCEDCURSEEFFECT].tags = INV_ATTR_TAG_UTILITY;

	ItemModTable[INV_REDUCEDCURSEDURATION].attrib_low = 1;
	ItemModTable[INV_REDUCEDCURSEDURATION].attrib_high = 4;
	ItemModTable[INV_REDUCEDCURSEDURATION].attrib_level_modifier = 0;
	ItemModTable[INV_REDUCEDCURSEDURATION].tags = INV_ATTR_TAG_UTILITY;

	ItemModTable[INV_AVOID_IGNITE].attrib_low = 1;
	ItemModTable[INV_AVOID_IGNITE].attrib_high = 6;
	ItemModTable[INV_AVOID_IGNITE].attrib_level_modifier = 0;
	ItemModTable[INV_AVOID_IGNITE].tags = INV_ATTR_TAG_FIRE | INV_ATTR_TAG_ELEMENTAL;

	ItemModTable[INV_AVOID_CHILLFREEZE].attrib_low = 1;
	ItemModTable[INV_AVOID_CHILLFREEZE].attrib_high = 6;
	ItemModTable[INV_AVOID_CHILLFREEZE].attrib_level_modifier = 0;
	ItemModTable[INV_AVOID_CHILLFREEZE].tags = INV_ATTR_TAG_ICE | INV_ATTR_TAG_ELEMENTAL;

	ItemModTable[INV_AVOID_POISON].attrib_low = 1;
	ItemModTable[INV_AVOID_POISON].attrib_high = 6;
	ItemModTable[INV_AVOID_POISON].attrib_level_modifier = 0;
	ItemModTable[INV_AVOID_POISON].tags = INV_ATTR_TAG_POISON | INV_ATTR_TAG_ELEMENTAL;

	ItemModTable[INV_AVOID_OVERLOAD].attrib_low = 1;
	ItemModTable[INV_AVOID_OVERLOAD].attrib_high = 6;
	ItemModTable[INV_AVOID_OVERLOAD].attrib_level_modifier = 0;
	ItemModTable[INV_AVOID_OVERLOAD].tags = INV_ATTR_TAG_LIGHTNING | INV_ATTR_TAG_ELEMENTAL;

	ItemModTable[INV_AVOID_ELEAILMENTS].attrib_low = 1;
	ItemModTable[INV_AVOID_ELEAILMENTS].attrib_high = 3;
	ItemModTable[INV_AVOID_ELEAILMENTS].attrib_level_modifier = 0;
	ItemModTable[INV_AVOID_ELEAILMENTS].tags = INV_ATTR_TAG_ELEMENTAL;

	ItemModTable[INV_AVOID_BLEED].attrib_low = 1;
	ItemModTable[INV_AVOID_BLEED].attrib_high = 6;
	ItemModTable[INV_AVOID_BLEED].attrib_level_modifier = 0;
	ItemModTable[INV_AVOID_BLEED].tags = INV_ATTR_TAG_PHYSICAL;
	
	ItemModTable[INV_CHANCE_BLEED].attrib_low = 1;
	ItemModTable[INV_CHANCE_BLEED].attrib_high = 9;
	ItemModTable[INV_CHANCE_BLEED].attrib_level_modifier = 0;
	ItemModTable[INV_CHANCE_BLEED].tags = INV_ATTR_TAG_PHYSICAL | INV_ATTR_TAG_ATTACK;
	
	ItemModTable[INV_PERCENTDMG_BLEED].attrib_low = 5;
	ItemModTable[INV_PERCENTDMG_BLEED].attrib_high = 10;
	ItemModTable[INV_PERCENTDMG_BLEED].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTDMG_BLEED].tags = INV_ATTR_TAG_PHYSICAL | INV_ATTR_TAG_DAMAGE;

	ItemModTable[INV_BLEED_DURATION].attrib_low = 1;
	ItemModTable[INV_BLEED_DURATION].attrib_high = 10;
	ItemModTable[INV_BLEED_DURATION].attrib_level_modifier = 0;
	ItemModTable[INV_BLEED_DURATION].tags = INV_ATTR_TAG_PHYSICAL;
	
	/////////////////////////
	// corrupted implicits //
	/////////////////////////
	ItemModTable[INV_CORR_WEAPONDMG].attrib_low = 1.15;
	ItemModTable[INV_CORR_WEAPONDMG].attrib_high = 1.33;
	ItemModTable[INV_CORR_WEAPONDMG].attrib_level_modifier = 0;
	ItemModTable[INV_CORR_WEAPONDMG].tags = INV_ATTR_TAG_DAMAGE;

	ItemModTable[INV_CORR_WEAPONCRIT].attrib_low = 0.5;
	ItemModTable[INV_CORR_WEAPONCRIT].attrib_high = 1.0;
	ItemModTable[INV_CORR_WEAPONCRIT].attrib_level_modifier = 0;
	ItemModTable[INV_CORR_WEAPONCRIT].tags = INV_ATTR_TAG_CRIT;

	ItemModTable[INV_CORR_WEAPONCRITDMG].attrib_low = 50;
	ItemModTable[INV_CORR_WEAPONCRITDMG].attrib_high = 200;
	ItemModTable[INV_CORR_WEAPONCRITDMG].attrib_level_modifier = 0;
	ItemModTable[INV_CORR_WEAPONCRITDMG].tags = INV_ATTR_TAG_CRIT;

	ItemModTable[INV_CORR_SPEED].attrib_low = 0.1;
	ItemModTable[INV_CORR_SPEED].attrib_high = 0.25;
	ItemModTable[INV_CORR_SPEED].attrib_level_modifier = 0;
	ItemModTable[INV_CORR_SPEED].tags = INV_ATTR_TAG_UTILITY;

	ItemModTable[INV_CORR_DROPCHANCE].attrib_low = 0.1;
	ItemModTable[INV_CORR_DROPCHANCE].attrib_high = 0.2;
	ItemModTable[INV_CORR_DROPCHANCE].attrib_level_modifier = 0;
	ItemModTable[INV_CORR_DROPCHANCE].tags = INV_ATTR_TAG_UTILITY;

	ItemModTable[INV_CORR_PERCENTSTATS].attrib_low = 10;
	ItemModTable[INV_CORR_PERCENTSTATS].attrib_high = 33;
	ItemModTable[INV_CORR_PERCENTSTATS].attrib_level_modifier = 0;
	ItemModTable[INV_CORR_PERCENTSTATS].tags = INV_ATTR_TAG_STAT;

	ItemModTable[INV_CORR_WEAPONPCTDMG].attrib_low = 1;
	ItemModTable[INV_CORR_WEAPONPCTDMG].attrib_high = 5;
	ItemModTable[INV_CORR_WEAPONPCTDMG].attrib_level_modifier = 0;
	ItemModTable[INV_CORR_WEAPONPCTDMG].tags = INV_ATTR_TAG_DAMAGE;

	ItemModTable[INV_CORR_WEAPONPOISONPCT].attrib_low = 5;
	ItemModTable[INV_CORR_WEAPONPOISONPCT].attrib_high = 10;
	ItemModTable[INV_CORR_WEAPONPOISONPCT].attrib_level_modifier = 0;
	ItemModTable[INV_CORR_WEAPONPOISONPCT].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_CORR_WEAPONFORCEPAIN].attrib_low = 1;
	ItemModTable[INV_CORR_WEAPONFORCEPAIN].attrib_high = 20;
	ItemModTable[INV_CORR_WEAPONFORCEPAIN].attrib_level_modifier = 0;
	ItemModTable[INV_CORR_WEAPONFORCEPAIN].tags = INV_ATTR_TAG_UTILITY;

	ItemModTable[INV_CORR_CYBERNETIC].attrib_low = 1;
	ItemModTable[INV_CORR_CYBERNETIC].attrib_high = 1;
	ItemModTable[INV_CORR_CYBERNETIC].attrib_level_modifier = 0;
	ItemModTable[INV_CORR_CYBERNETIC].tags = INV_ATTR_TAG_UTILITY;

	ItemModTable[INV_CORR_DMGDOESNTSTOPREGEN].attrib_low = 1;
	ItemModTable[INV_CORR_DMGDOESNTSTOPREGEN].attrib_high = 1;
	ItemModTable[INV_CORR_DMGDOESNTSTOPREGEN].attrib_level_modifier = 0;
	ItemModTable[INV_CORR_DMGDOESNTSTOPREGEN].tags = INV_ATTR_TAG_UTILITY;

	ItemModTable[INV_CORR_INSTALEECHPCT].attrib_low = 5;
	ItemModTable[INV_CORR_INSTALEECHPCT].attrib_high = 10;
	ItemModTable[INV_CORR_INSTALEECHPCT].attrib_level_modifier = 0;
	ItemModTable[INV_CORR_INSTALEECHPCT].tags = INV_ATTR_TAG_LIFE;

	///////////////////////
	// regular implicits //
	///////////////////////

	// we dont care what they have, they get loaded in hardcoded values from outside
	for(int i = FIRST_REGULAR_IMPLICIT; i <= LAST_REGULAR_IMPLICIT; ++i) {
		ItemModTable[i].attrib_low = 100000;
		ItemModTable[i].attrib_high = -1;
		ItemModTable[i].attrib_level_modifier = 0;
		ItemModTable[i].tags = INV_ATTR_TAG_DEFENSE;
	}

	///////////////////////////////
	// essences from here on out //
	///////////////////////////////
	ItemModTable[INV_ESS_VAAJ].attrib_low = 1;
	ItemModTable[INV_ESS_VAAJ].attrib_high = 4;
	ItemModTable[INV_ESS_VAAJ].attrib_extra_low = 1;
	ItemModTable[INV_ESS_VAAJ].attrib_extra_high = 2;
	ItemModTable[INV_ESS_VAAJ].attrib_level_modifier = 0;
	ItemModTable[INV_ESS_VAAJ].attrib_level_extra_modifier = 0;
	ItemModTable[INV_ESS_VAAJ].tags = INV_ATTR_TAG_NONE;
	
	ItemModTable[INV_ESS_SSRATH].attrib_low = 3;
	ItemModTable[INV_ESS_SSRATH].attrib_high = 5;
	ItemModTable[INV_ESS_SSRATH].attrib_level_modifier = 0;
	ItemModTable[INV_ESS_SSRATH].tags = INV_ATTR_TAG_NONE;
	
	ItemModTable[INV_ESS_OMNISIGHT].attrib_low = 500;
	ItemModTable[INV_ESS_OMNISIGHT].attrib_high = 1000;
	ItemModTable[INV_ESS_OMNISIGHT].attrib_level_modifier = 0;
	ItemModTable[INV_ESS_OMNISIGHT].tags = INV_ATTR_TAG_NONE;
	
	ItemModTable[INV_ESS_OMNISIGHT2].attrib_low = 2;
	ItemModTable[INV_ESS_OMNISIGHT2].attrib_high = 5;
	ItemModTable[INV_ESS_OMNISIGHT2].attrib_level_modifier = 0;
	ItemModTable[INV_ESS_OMNISIGHT2].tags = INV_ATTR_TAG_NONE;
	
	ItemModTable[INV_ESS_CHEGOVAX].attrib_low = 1;
	ItemModTable[INV_ESS_CHEGOVAX].attrib_high = 4;
	ItemModTable[INV_ESS_CHEGOVAX].attrib_level_modifier = 0;
	ItemModTable[INV_ESS_CHEGOVAX].tags = INV_ATTR_TAG_NONE;
	
	ItemModTable[INV_ESS_HARKIMONDE].attrib_low = 3;
	ItemModTable[INV_ESS_HARKIMONDE].attrib_high = 6;
	ItemModTable[INV_ESS_HARKIMONDE].attrib_level_modifier = 0;
	ItemModTable[INV_ESS_HARKIMONDE].tags = INV_ATTR_TAG_NONE;
	
	ItemModTable[INV_ESS_LESHRAC].attrib_low = 1;
	ItemModTable[INV_ESS_LESHRAC].attrib_high = 1;
	ItemModTable[INV_ESS_LESHRAC].attrib_level_modifier = 1;
	ItemModTable[INV_ESS_LESHRAC].tags = INV_ATTR_TAG_NONE;
	
	ItemModTable[INV_ESS_KRULL].attrib_low = 5;
	ItemModTable[INV_ESS_KRULL].attrib_high = 9;
	ItemModTable[INV_ESS_KRULL].attrib_level_modifier = 0;
	ItemModTable[INV_ESS_KRULL].tags = INV_ATTR_TAG_NONE;
	
	ItemModTable[INV_ESS_THORAX].attrib_low = 1;
	ItemModTable[INV_ESS_THORAX].attrib_high = 1;
	ItemModTable[INV_ESS_THORAX].attrib_level_modifier = 1;
	ItemModTable[INV_ESS_THORAX].tags = INV_ATTR_TAG_NONE;
	
	ItemModTable[INV_ESS_ZRAVOG].attrib_low = 1;
	ItemModTable[INV_ESS_ZRAVOG].attrib_high = 2;
	ItemModTable[INV_ESS_ZRAVOG].attrib_level_modifier = 0;
	ItemModTable[INV_ESS_ZRAVOG].tags = INV_ATTR_TAG_NONE;
	
	ItemModTable[INV_ESS_ERYXIA].attrib_low = 0.05;
	ItemModTable[INV_ESS_ERYXIA].attrib_high = 0.075;
	ItemModTable[INV_ESS_ERYXIA].attrib_level_modifier = 0.025;
	ItemModTable[INV_ESS_ERYXIA].tags = INV_ATTR_TAG_NONE;

	////////////////////////////////
	// incursion from here on out //
	////////////////////////////////
	ItemModTable[INV_INC_DOUBLEHPBONUS].attrib_low = 1;
	ItemModTable[INV_INC_DOUBLEHPBONUS].attrib_high = 1;
	ItemModTable[INV_INC_DOUBLEHPBONUS].attrib_level_modifier = 0;
	ItemModTable[INV_INC_DOUBLEHPBONUS].attrib_extra_low = 300;
	ItemModTable[INV_INC_DOUBLEHPBONUS].attrib_extra_high = 300;
	ItemModTable[INV_INC_DOUBLEHPBONUS].attrib_level_extra_modifier = -1;
	ItemModTable[INV_INC_DOUBLEHPBONUS].tags = INV_ATTR_TAG_LIFE;

	ItemModTable[INV_INC_MAXPOISONSTACK].attrib_low = 1;
	ItemModTable[INV_INC_MAXPOISONSTACK].attrib_high = 1;
	ItemModTable[INV_INC_MAXPOISONSTACK].attrib_level_modifier = 0;
	ItemModTable[INV_INC_MAXPOISONSTACK].tags = INV_ATTR_TAG_POISON | INV_ATTR_TAG_ELEMENTAL;

	ItemModTable[INV_INC_POISONSPREAD].attrib_low = 1;
	ItemModTable[INV_INC_POISONSPREAD].attrib_high = 3;
	ItemModTable[INV_INC_POISONSPREAD].attrib_level_modifier = 0;
	ItemModTable[INV_INC_POISONSPREAD].tags = INV_ATTR_TAG_POISON | INV_ATTR_TAG_ELEMENTAL;

	ItemModTable[INV_INC_CRITFORDOT].attrib_low = 1;
	ItemModTable[INV_INC_CRITFORDOT].attrib_high = 1;
	ItemModTable[INV_INC_CRITFORDOT].attrib_level_modifier = 0;
	ItemModTable[INV_INC_CRITFORDOT].tags = INV_ATTR_TAG_CRIT | INV_ATTR_TAG_DAMAGE;

	ItemModTable[INV_INC_ALLOVERLOAD].attrib_low = 25;
	ItemModTable[INV_INC_ALLOVERLOAD].attrib_high = 40;
	ItemModTable[INV_INC_ALLOVERLOAD].attrib_level_modifier = -1;
	ItemModTable[INV_INC_ALLOVERLOAD].tags = INV_ATTR_TAG_LIGHTNING | INV_ATTR_TAG_ELEMENTAL;

	ItemModTable[INV_INC_ESHIELDNOINTERRUPT].attrib_low = 1;
	ItemModTable[INV_INC_ESHIELDNOINTERRUPT].attrib_high = 1;
	ItemModTable[INV_INC_ESHIELDNOINTERRUPT].attrib_level_modifier = 0;
	ItemModTable[INV_INC_ESHIELDNOINTERRUPT].tags = INV_ATTR_TAG_DEFENSE;

	ItemModTable[INV_INC_PLUSPROJ].attrib_low = 1;
	ItemModTable[INV_INC_PLUSPROJ].attrib_high = 1;
	ItemModTable[INV_INC_PLUSPROJ].attrib_level_modifier = -1;
	ItemModTable[INV_INC_PLUSPROJ].tags = INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_INC_PLUSTWOPROJ].attrib_low = 2;
	ItemModTable[INV_INC_PLUSTWOPROJ].attrib_high = 2;
	ItemModTable[INV_INC_PLUSTWOPROJ].attrib_level_modifier = -1;
	ItemModTable[INV_INC_PLUSTWOPROJ].tags = INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_INC_INSTANTLIFESTEAL].attrib_low = 1;
	ItemModTable[INV_INC_INSTANTLIFESTEAL].attrib_high = 2;
	ItemModTable[INV_INC_INSTANTLIFESTEAL].attrib_level_modifier = 0;
	ItemModTable[INV_INC_INSTANTLIFESTEAL].attrib_extra_low = 100;
	ItemModTable[INV_INC_INSTANTLIFESTEAL].attrib_extra_high = 100;
	ItemModTable[INV_INC_INSTANTLIFESTEAL].attrib_level_extra_modifier = -1;
	ItemModTable[INV_INC_INSTANTLIFESTEAL].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_LIFE;

	ItemModTable[INV_INC_TWICEARMORDEFENSE].attrib_low = 1;
	ItemModTable[INV_INC_TWICEARMORDEFENSE].attrib_high = 2;
	ItemModTable[INV_INC_TWICEARMORDEFENSE].attrib_level_modifier = 0;
	ItemModTable[INV_INC_TWICEARMORDEFENSE].tags = INV_ATTR_TAG_DEFENSE;

	ItemModTable[INV_INC_MITIGATIONTODODGE].attrib_low = 1;
	ItemModTable[INV_INC_MITIGATIONTODODGE].attrib_high = 1;
	ItemModTable[INV_INC_MITIGATIONTODODGE].attrib_level_modifier = 0;
	ItemModTable[INV_INC_MITIGATIONTODODGE].tags = INV_ATTR_TAG_DEFENSE;

	ItemModTable[INV_INC_ACCURACYFORPRECISION].attrib_low = 1;
	ItemModTable[INV_INC_ACCURACYFORPRECISION].attrib_high = 2;
	ItemModTable[INV_INC_ACCURACYFORPRECISION].attrib_level_modifier = -1;
	ItemModTable[INV_INC_ACCURACYFORPRECISION].tags = INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_INC_HPREGENINTERRUPT].attrib_low = 1;
	ItemModTable[INV_INC_HPREGENINTERRUPT].attrib_high = 1;
	ItemModTable[INV_INC_HPREGENINTERRUPT].attrib_level_modifier = 0;
	ItemModTable[INV_INC_HPREGENINTERRUPT].tags = INV_ATTR_TAG_LIFE;

	ItemModTable[INV_INC_PASSIVEREGEN].attrib_low = 0.01;
	ItemModTable[INV_INC_PASSIVEREGEN].attrib_high = 0.03;
	ItemModTable[INV_INC_PASSIVEREGEN].attrib_extra_low = 100;
	ItemModTable[INV_INC_PASSIVEREGEN].attrib_extra_high = 100;
	ItemModTable[INV_INC_PASSIVEREGEN].attrib_level_modifier = 0.02;
	ItemModTable[INV_INC_PASSIVEREGEN].attrib_level_extra_modifier = -1;
	ItemModTable[INV_INC_PASSIVEREGEN].tags = INV_ATTR_TAG_LIFE;

	ItemModTable[INV_INC_ENEMYRIPCHANCE].attrib_low = 1;
	ItemModTable[INV_INC_ENEMYRIPCHANCE].attrib_high = 3;
	ItemModTable[INV_INC_ENEMYRIPCHANCE].attrib_level_modifier = 0;
	ItemModTable[INV_INC_ENEMYRIPCHANCE].tags = INV_ATTR_TAG_UTILITY;

	ItemModTable[INV_INC_BLOCKPREVENTION].attrib_low = 1;
	ItemModTable[INV_INC_BLOCKPREVENTION].attrib_high = 8;
	ItemModTable[INV_INC_BLOCKPREVENTION].attrib_level_modifier = 0;
	ItemModTable[INV_INC_BLOCKPREVENTION].tags = INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_INC_RIPPERSEXPLODE].attrib_low = 1;
	ItemModTable[INV_INC_RIPPERSEXPLODE].attrib_high = 2;
	ItemModTable[INV_INC_RIPPERSEXPLODE].attrib_level_modifier = 0;
	ItemModTable[INV_INC_RIPPERSEXPLODE].tags = INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_INC_INVERTRESISTANCES].attrib_low = 1;
	ItemModTable[INV_INC_INVERTRESISTANCES].attrib_high = 2;
	ItemModTable[INV_INC_INVERTRESISTANCES].attrib_level_modifier = 0;
	ItemModTable[INV_INC_INVERTRESISTANCES].tags = INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_INC_ACCURACYREVERSED].attrib_low = 1;
	ItemModTable[INV_INC_ACCURACYREVERSED].attrib_high = 1;
	ItemModTable[INV_INC_ACCURACYREVERSED].attrib_level_modifier = -1;
	ItemModTable[INV_INC_ACCURACYREVERSED].tags = INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_INC_PROJREVERSE].attrib_low = 0.1;
	ItemModTable[INV_INC_PROJREVERSE].attrib_high = 2.0;
	ItemModTable[INV_INC_PROJREVERSE].attrib_level_modifier = -1;
	ItemModTable[INV_INC_PROJREVERSE].tags = INV_ATTR_TAG_ATTACK;

	ItemModTable[INV_INC_EXCESSCRIT].attrib_low = 0.2;
	ItemModTable[INV_INC_EXCESSCRIT].attrib_high = 0.2;
	ItemModTable[INV_INC_EXCESSCRIT].attrib_level_modifier = -1;
	ItemModTable[INV_INC_EXCESSCRIT].tags = INV_ATTR_TAG_CRIT;
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

#define ITEM_MODRANGE_LOW 0
#define ITEM_MODRANGE_HIGH 1

#define DND_POWERCORE_ATTRFACTOR -50
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

		// these store the weapon in extra field!
		case INV_CORR_WEAPONDMG:
		case INV_CORR_WEAPONCRIT:
		case INV_CORR_WEAPONCRITDMG:
		case INV_CORR_WEAPONPCTDMG:
		case INV_CORR_WEAPONPOISONPCT:
		case INV_CORR_WEAPONFORCEPAIN:

		// incursion things that use extra field

		return true;
	}
	return false;
}

bool CanRerollAttributeExtra(int attr) {
	switch(attr) {
		case INV_ESS_VAAJ:

		case INV_INC_PLUSPROJ:
		case INV_INC_PLUSTWOPROJ:
		return true;
	}
	return false;
}

bool IsAttributeQualityException(int attr) {
	switch(attr) {
		case INV_EX_LIMITEDSMALLCHARMS:
		
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

str GetDetailedModRange(int attr, int item_type, int item_subtype, int tier, int trunc_factor = 0, int extra = -1, bool isPercentage = false) {
	if(extra != -1)
		return GetDetailedModRange_Unique(tier, trunc_factor, extra, isPercentage);
	
	// limit this to here at t10...
	str col_tag = Charm_Strings[Clamp_Between(tier, 0, 9)][CHARMSTR_COLORCODE];
	int tier_mapping = GetModTierRangeMapper(attr, tier);

	// visually change the attribute values depending on item scale factors
	int f = GetItemAttributeFactor(item_type, item_subtype);
	
	if(!trunc_factor) {
		return StrParam(
			s:"\c-(",
			s:col_tag, d:GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW, f),
			s:"\c--",
			s:col_tag, d:GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH, f), s:"\c-)"
		);
	}
	return StrParam(
		s:"\c-(",
		s:col_tag, s:GetFixedRepresentation(GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW, f), isPercentage),
		s:"\c--",
		s:col_tag, s:GetFixedRepresentation(GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH, f), isPercentage), s:"\c-)"
	);
}

str GetDetailedModRangeExtra(int attr, int item_type, int item_subtype, int tier, int trunc_factor = 0, bool isPercentage = false) {
	// limit this to here at t10...
	str col_tag = Charm_Strings[Clamp_Between(tier, 0, 9)][CHARMSTR_COLORCODE];
	int tier_mapping = GetModTierRangeMapperExtra(attr, tier);

	// visually change the attribute values depending on item scale factors
	int f = GetItemAttributeFactor(item_type, item_subtype);
	
	if(!trunc_factor) {
		return StrParam(
			s:"\c-(",
			s:col_tag, d:GetModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_LOW, f),
			s:"\c--",
			s:col_tag, d:GetModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_HIGH, f), s:"\c-)"
		);
	}
	return StrParam(
		s:"\c-(",
		s:col_tag, s:GetFixedRepresentation(GetModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_LOW, f), isPercentage),
		s:"\c--",
		s:col_tag, s:GetFixedRepresentation(GetModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_HIGH, f), isPercentage), s:"\c-)"
	);
}

str GetDetailedImplicitModRange(int attr, int item_type, int item_subtype, int trunc_factor = 0, bool isPercentage = false) {
	str col_tag = "\cg";
	
	if(!trunc_factor) {
		return StrParam(
			s:"\c-(",
			s:col_tag, d:ItemModTable[attr].attrib_low,
			s:"\c--",
			s:col_tag, d:ItemModTable[attr].attrib_high, s:"\c-)"
		);
	}
	return StrParam(
		s:"\c-(",
		s:col_tag, s:GetFixedRepresentation(ItemModTable[attr].attrib_low, isPercentage),
		s:"\c--",
		s:col_tag, s:GetFixedRepresentation(ItemModTable[attr].attrib_high, isPercentage), s:"\c-)"
	);
}

str GetDetailedModRange_Unique(int unique_id, int trunc_factor = 0, int unique_roll_id = 0, bool isPercentage = false, bool isExtra = false) {
	int low = UniqueItemList[unique_id].rolls[unique_roll_id].attrib_low;
	int high = UniqueItemList[unique_id].rolls[unique_roll_id].attrib_high;

	if(isExtra) {
		low = UniqueItemList[unique_id].rolls[unique_roll_id].attrib_extra_low;
		high = UniqueItemList[unique_id].rolls[unique_roll_id].attrib_extra_high;
	}

	if(!trunc_factor) {
		return StrParam(
			s:"\c-(",
			s:"\c[D1]", d:low,
			s:"\c--",
			s:"\c[D1]", d:high, s:"\c-)"
		);
	}
	
	if(unique_id != UITEM_WELLOFPOWER) {
		return StrParam(
			s:"\c-(",
			s:"\c[D1]", s:GetFixedRepresentation(low, isPercentage),
			s:"\c--",
			s:"\c[D1]", s:GetFixedRepresentation(high, isPercentage), s:"\c-)"
		);
	}

	// this item is a little odd, so we need to treat it as such
	// since ACS can't round floats to shit (bad representation, w.e) we need to do a custom one for weird numbers like this

	return StrParam(
		s:"\c-(",
		s:"\c[D1]", d:low / 1000, s:".", d:(low / 100) % 10, d:(low / 10) % 10,
		s:"\c--",
		s:"\c[D1]", d:high / 1000, s:".", d:(high / 100) % 10, d:(high / 10) % 10, s:"\c-)"
	);
}

str GetModTierText(int tier, int extra) {
	if(extra != -1)
		return StrParam(s:"\c[D1]", s:"U");
	return StrParam(s:Charm_Strings[Clamp_Between(tier, 0, 9)][CHARMSTR_COLORCODE], s:"T", d:tier);
}

str GetInventoryAttributeText(int attr) {
	if(attr <= LAST_INV_ATTRIBUTE)
		return StrParam(s:"IATTR_T", d:attr);

	if(attr <= LAST_CORRUPT_IMPLICIT)
		return StrParam(s:"IATTR_TC", d:attr + 1 - FIRST_CORRUPT_IMPLICIT);

	if(attr <= LAST_REGULAR_IMPLICIT)
		return StrParam(s:"IATTR_TI", d:attr + 1 - FIRST_REGULAR_IMPLICIT);
		
	// essences are mapped to 1 again for language file
	if(attr <= LAST_ESSENCE_ATTRIBUTE)
		return StrParam(s:"IATTR_TE", d:attr + 1 - FIRST_ESSENCE_ATTRIBUTE);

	if(attr <= LAST_INCURSION_ATTRIBUTE)
		return StrParam(s:"IATTR_TINC", d:attr + 1 - FIRST_INCURSION_ATTRIBUTE);
		
	// only option left is unique exotic attributes
	return StrParam(s:"IATTR_TX", d:UNIQUE_MAP_MACRO(attr));
}

str ItemAttributeString(int attr, int item_type, int item_subtype, int val, int tier = 0, bool showDetailedMods = false, int extra = -1, bool isFractured = false, int qual = 0, int attr_extra = 0) {
	str text = GetInventoryAttributeText(attr);
	str ess_tag = "\c[Q7]";
	str col_tag = "\c[Q9]";
	str no_tag = "\c- ";

	if(isFractured) {
		col_tag = "\c[E2]";
		ess_tag = "\c[E2]";
		no_tag = "\c[E2] ";
	}
	else if(attr >= FIRST_INCURSION_ATTRIBUTE && attr <= LAST_INCURSION_ATTRIBUTE) {
		ess_tag = "\c[Q0]";
		col_tag = "\c[Q9]";
		no_tag = "\c[Q0] ";
	}

	// dont draw essence mods as special if they are on unique items
	if(extra != -1)
		ess_tag = "";

	if(qual) {
		if(!IsAttributeQualityException(attr)) {
			if(val < 100000) {
				val *= qual + 100;
				val /= 100;
			}
			else {
				val /= 100;
				val *= qual + 100;
			}
		}

		if(attr_extra && !IsAttributeExtraException(attr)) {
			if(attr_extra > 100000) {
				attr_extra /= 100;
				attr_extra *= qual + 100;
			}
			else {
				attr_extra *= qual + 100;
				attr_extra /= 100;
			}
		}
	}
	
	switch(attr) {
		case INV_CORR_CYBERNETIC:
			if(!isFractured)
				col_tag = "\c[R5]";
			return StrParam(s:col_tag, l:text);

		case INV_CYBERNETIC:
			if(!isFractured)
				col_tag = "\c[R5]";
			if(showDetailedMods) {
				return StrParam(
					s:col_tag, l:text, s:no_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:col_tag, l:text);
			
		// essences with specific writing
		case INV_ESS_SSRATH:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"% ", s:ess_tag, l:"IATTR_MAGICRES",
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, l:text, s:col_tag, d:val, s:"% ", s:ess_tag, l:"IATTR_MAGICRES");
			
		// text + val%
		case INV_ESS_CHEGOVAX:
		case INV_ESS_ZRAVOG:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%",
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, l:text, s:col_tag, d:val, s:"%");

		// essence with no numeric values
		case INV_ESS_LESHRAC:
		case INV_ESS_THORAX:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, l:text);
			
		// essences with percentages in them
		case INV_ESS_OMNISIGHT2:
			// this ones used in a unique so
			if(extra == -1) {
				if(showDetailedMods) {
					return StrParam(
						s:ess_tag, s:"+ ", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:ess_tag, l:text,
						s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:ess_tag, s:"+ ", s:col_tag, d:val, s:"%", s:ess_tag, l:text);
			}
			if(showDetailedMods) {
				return StrParam(
					s:"+ ", s:col_tag, d:val, s:"\c-", s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:ess_tag, s:"%", l:text,
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+ ", s:col_tag, d:val, s:"%", s:"\c-", l:text);

		case INV_ESS_HARKIMONDE:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, s:"+ ", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:ess_tag, l:text,
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, s:"+ ", s:col_tag, d:val, s:"%", s:ess_tag, l:text);

		case INV_ESS_KRULL:
			if(showDetailedMods) {
				if(extra == -1) {
					return StrParam(
						s:ess_tag, s:"+ ", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:ess_tag, l:text,
						s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(
					s:"+ ", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%\c-", l:text,
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			if(extra == -1)
				return StrParam(s:ess_tag, s:"+ ", s:col_tag, d:val, s:"%", s:ess_tag, l:text);
			return StrParam(s:"+ ", s:col_tag, d:val, s:"%\c-", l:text);
			
		// essences that are like regular mods, just have color code
		case INV_ESS_OMNISIGHT:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, s:"+ ", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:ess_tag, l:text,
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, s:"+ ", s:col_tag, d:val, s:ess_tag, l:text);

		case INV_ESS_VAAJ:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, s:"+ ", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"% ", s:ess_tag, l:text,
					s:ess_tag, d:attr_extra, s:GetDetailedModRangeExtra(attr, item_type, item_subtype, tier, 0), s:col_tag, s:"% ", s:ess_tag, l:"IATTR_TE1S",
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, s:"+ ", s:col_tag, d:val, s:"% ", s:ess_tag, l:text, s:col_tag, d:attr_extra, s:"% ", s:ess_tag, l:"IATTR_TE1S");
		
		// since percentages are handled in default case, we will handle all flat value attributes under here
		case INV_HP_INCREASE:
		case INV_ARMOR_INCREASE:
		case INV_SHIELD_INCREASE:

		case INV_FLATPHYS_DAMAGE:
		case INV_FLATENERGY_DAMAGE:
		case INV_FLATMAGIC_DAMAGE:
		case INV_FLATELEM_DAMAGE:
		case INV_FLATRADIUS_DAMAGE:

		case INV_FLAT_ARTILLERY:
		case INV_FLAT_AUTOMATIC:
		case INV_FLAT_HANDGUN:
		case INV_FLAT_SHOTGUN:
		case INV_FLAT_PRECISION:
		case INV_FLAT_TECH:
		case INV_FLAT_MAGIC:

		case INV_PEN_ELEMENTAL:
		case INV_PEN_ENERGY:
		case INV_PEN_PHYSICAL:
		case INV_PEN_OCCULT:
		case INV_PEN_FIRE:
		case INV_PEN_ICE:
		case INV_PEN_LIGHTNING:
		case INV_PEN_POISON:

		case INV_KNOCKBACK_RESIST:
		case INV_ACCURACY_INCREASE:
		case INV_STAT_STRENGTH:
		case INV_STAT_DEXTERITY:
		case INV_STAT_INTELLECT:
		case INV_FLAT_FIREDMG:
		case INV_FLAT_ICEDMG:
		case INV_FLAT_LIGHTNINGDMG:
		case INV_FLAT_POISONDMG:
		case INV_OVERLOAD_ZAPCOUNT:
		case INV_IGNITE_PROLIFCOUNT:

		case INV_INC_MAXPOISONSTACK:
			if(val > 0) {
				if(showDetailedMods) {
					return StrParam(
						s:"+ ", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:no_tag, l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"+ ", s:col_tag, d:val, s:no_tag, l:text);
			}
			else if(val < 0)
				return StrParam(s:col_tag, s:"- \cg", d:val, s:no_tag, l:text);
				
		// corrupted implicits of certain kinds have +X% text TO (extra)
		case INV_CORR_WEAPONCRITDMG:
		case INV_CORR_WEAPONPCTDMG:
		case INV_CORR_WEAPONPOISONPCT:
			if(showDetailedMods) {
				return StrParam(
					s:"+ ", s:col_tag, d:val, s:GetDetailedImplicitModRange(attr, item_type, item_subtype, 0), s:"%", s:no_tag, l:text, s: " ", l:GetWeaponTag(extra)
				);
			}
			return StrParam(s:"+ ", s:col_tag, d:val, s:"%", s:no_tag, l:text, s: " ", l:GetWeaponTag(extra));

		// this doesn't have detailed mod explanation on purpose, each item has their own presets
		case INV_IMP_INCARMOR:
		case INV_IMP_INCSHIELD:
		case INV_IMP_INCARMORSHIELD:
			text = StrParam(s:"+ ", s:col_tag, d:val, s:no_tag, l:text);
		return text;
		case INV_IMP_INCMIT:
			text = StrParam(s:"+ ", s:col_tag, s:GetFixedRepresentation(val, false), s:"%", s:no_tag, l:text);
		return text;
		case INV_IMP_INCMITARMOR:
			text = StrParam(s:"+ ", s:col_tag, d:val, s:no_tag, l:text, s: " ", s:col_tag, s:GetFixedRepresentation((val << 16) / DND_ARMOR_TO_MIT_RATIO, false), s:"%", s:no_tag, l:"IATTR_TI4");
		return text;
		case INV_IMP_INCMITSHIELD:
			text = StrParam(s:"+ ", s:col_tag, d:val, s:no_tag, l:text, s: " ", s:col_tag, s:GetFixedRepresentation((val << 16) / DND_SHIELD_TO_MIT_RATIO, false), s:"%", s:no_tag, l:"IATTR_TI4");
		return text;
		case INV_IMP_POWERCORE:
			text = StrParam(s:"+ ", s:col_tag, d:extra, s:"%", s:no_tag, l:text, s: "\n", s:"+ ", s:col_tag, d:val, s:no_tag, l:"IATTR_T98", s:"\n\c[R5]", l:"IATTR_T72");
		return text;
		case INV_IMP_UNSTABLECORE:
			text = StrParam(
				s:"+ ", s:col_tag, d:val, s:no_tag, l:"IATTR_T98", s:"\n\cd",
				s:GetFixedRepresentation(extra, true), s:"%\c-", s:no_tag, l:text, s:" \cd", s:GetFixedRepresentation(extra * UNSTABLE_DMG_MULT, true), s:"%\c- ", l:"IATTR_TI8S", s:"\n\c[R5]",
				l:"IATTR_T72"
			);
		return text;
		case INV_IMP_INCMITARMORSHIELD:
			text = StrParam(s:"+ ", s:col_tag, d:val, s:no_tag, l:text, s: " +", s:col_tag, s:GetFixedRepresentation((val << 16) / DND_ARMOR_TO_MIT_RATIO, false), s:"%", s:no_tag, l:"IATTR_TI4");
		return text;

		case INV_IMP_RAVAGER:					// ravager armor
			text = StrParam(s:col_tag, d:val, s:"%\c-", l:text, s:col_tag, d:extra, s:"%\c- ", l:"IATTR_TI10S");
		return text;

		// just the wording
		case INV_IMP_DOUBLEESHIELDRECOVERY:		//
		case INV_IMP_CANROLL_PHYS:
		case INV_IMP_CANROLL_MAGIC:
		case INV_IMP_CANROLL_EXPLOSIVE:
		case INV_IMP_CANROLL_ENERGY:
		case INV_IMP_CANROLL_ELEMENTAL:
		case INV_IMP_HIGHREFLECTREDUCE:			// synth metal
		case INV_IMP_ESHIELDBLOCKSALL:
		case INV_IMP_BONUSPETCAP:
		case INV_IMP_MELEEIGNORESSHIELDS:
		case INV_IMP_RECOVERESONUNDEADKILL:
		case INV_IMP_QUALITYCAPFIFTY:
		case INV_CORR_DMGDOESNTSTOPREGEN:
		return StrParam(l:text);

		case INV_IMP_ABSORBLIGHTNING:			// lightning coil
			text = StrParam(s:col_tag, d:val, s:"%\c-", l:text, s:col_tag, d:extra, s:"%\c- ", l:"IATTR_TI12S");
		return text;
		case INV_IMP_KNIGHTARMOR:
			text = StrParam(s:col_tag, d:val, s:"%\c-", l:text, s:col_tag, d:extra, s:"%\c- ", l:"IATTR_TI13S");
		return text;
		case INV_IMP_HANDGUNBONUS:
			text = StrParam(l:text, s:col_tag, s:GetFixedRepresentation(val, true), s:"%\c-", l:"IATTR_TI14S");
		return text;

		// value and text only as percentage
		case INV_IMP_NECROARMOR:
		case INV_IMP_LESSLIGHTNINGTAKEN:
		case INV_IMP_FASTEROVERHEATDISS:
		case INV_IMP_LESSPOISONTAKEN:
		case INV_IMP_LESSFIRETAKEN:
		case INV_IMP_LESSSELFDAMAGETAKEN:
		case INV_IMP_REDUCEDVISIONIMPAIR:		// synthmetal mask
		case INV_CORR_INSTALEECHPCT:
			text = StrParam(s:col_tag, d:val, s:"%\c- ", l:text);
		return text;

		case INV_IMP_MOREDAMAGETOBOSSES:
			text = StrParam(l:text, s:col_tag, d:val, s:"%\c-", s:"\c- ", l:"IATTR_TI31S");
		return text;
		case INV_IMP_PRECISIONCRITBONUS:
			text = StrParam(l:text, s:col_tag, s:GetFixedRepresentation(val, true), s:"%\c-", l:"IATTR_TI33S");
		return text;

		case INV_CORR_WEAPONFORCEPAIN:
			return StrParam(l:GetWeaponTag(extra), s:" ", l:"DND_HAS", s:col_tag, s: " ", d:val, s:"%", s:no_tag, l:text);

		// fixed point corrupted
		case INV_CORR_DROPCHANCE:
		case INV_CORR_SPEED:
			if(showDetailedMods) {
				return StrParam(s:"+ ", s:col_tag, s:GetFixedRepresentation(val, true), s:GetDetailedImplicitModRange(attr, item_type, item_subtype, FACTOR_FIXED_RESOLUTION, true), s:"%", s:no_tag, l:text);
			}
			return StrParam(s:"+ ", s:col_tag, s:GetFixedRepresentation(val, true), s:"%", s:no_tag, l:text);

		// fixed point positive wep stuff
		case INV_CORR_WEAPONDMG:
			if(showDetailedMods) {
				return StrParam(
					s:"+ ", s:col_tag, s:GetFixedRepresentation(val - 1.0, true), s:GetDetailedImplicitModRange(attr, item_type, item_subtype, FACTOR_FIXED_RESOLUTION, true), s:"%", s:no_tag, l:text, s: " ", l:GetWeaponTag(extra)
				);
			}
			return StrParam(s:"+ ", s:col_tag, s:GetFixedRepresentation(val - 1.0, true), s:"%", s:no_tag, l:text, s: " ", l:GetWeaponTag(extra));

		// fixed point corrupted wep stuff
		case INV_CORR_WEAPONCRIT:
			if(showDetailedMods) {
				return StrParam(
					s:"+ ", s:col_tag, s:GetFixedRepresentation(val, true), s:GetDetailedImplicitModRange(attr, item_type, item_subtype, FACTOR_FIXED_RESOLUTION, true), s:"%", s:no_tag, l:text, s: " ", l:GetWeaponTag(extra)
				);
			}
			return StrParam(s:"+ ", s:col_tag, s:GetFixedRepresentation(val, true), s:"%", s:no_tag, l:text, s: " ", l:GetWeaponTag(extra));

		case INV_CORR_PERCENTSTATS:
			if(showDetailedMods) {
				return StrParam(s:"+ ", s:col_tag, d:val, s:GetDetailedImplicitModRange(attr, item_type, item_subtype, 0), s:"%", s:no_tag, l:text);
			}
			return StrParam(s:"+ ", s:col_tag, d:val, s:"%", s:no_tag, l:text);

		// fixed point stuff
		case INV_PROJSPEED:
		case INV_EXPGAIN_INCREASE:
		case INV_CREDITGAIN_INCREASE:
		case INV_DROPCHANCE_INCREASE:
		case INV_ITEMRARITY:
		case INV_LUCK_INCREASE:
		case INV_SPEED_INCREASE:
		case INV_PELLET_INCREASE:
		case INV_CRITCHANCE_INCREASE:
		case INV_CRITPERCENT_INCREASE:
		case INV_BLOCKERS_MOREDMG:
		case INV_OVERLOAD_DMGINCREASE:
		case INV_LIFESTEAL_DAMAGE:
		case INV_IGNITE_PROLIFRANGE:
		case INV_DAMAGEPERCENT_MORE:
			if(showDetailedMods) {
				return StrParam(s:"+ ", s:col_tag, s:GetFixedRepresentation(val, true), s:GetDetailedModRange(attr, item_type, item_subtype, tier, FACTOR_FIXED_RESOLUTION, extra, true), s:"%", s:no_tag, l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+ ", s:col_tag, s:GetFixedRepresentation(val, true), s:"%", s:no_tag, l:text);
		
		case INV_ESS_ERYXIA:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, s:GetFixedRepresentation(val, true), s:GetDetailedModRange(attr, item_type, item_subtype, tier, FACTOR_FIXED_RESOLUTION, extra, true), s:"%", s:ess_tag, s:" ", l:"IATTR_MOREDMG",
					s:no_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, l:text, s:col_tag, s:GetFixedRepresentation(val, true), s:"%", s:ess_tag, s:" ", l:"IATTR_MOREDMG");
		
		// damage reduction attributes are shown as they are
		case INV_DMGREDUCE_ELEM:
		case INV_DMGREDUCE_PHYS:
		case INV_DMGREDUCE_REFL:
		case INV_DMGREDUCE_ENERGY:
		case INV_DMGREDUCE_MAGIC:
		case INV_DMGREDUCE_FIRE:
		case INV_DMGREDUCE_LIGHTNING:
		case INV_DMGREDUCE_ICE:
		case INV_DMGREDUCE_POISON:
		case INV_DMGREDUCE_ALL:
		case INV_ADDEDMAXRESIST:
		case INV_OVERLOAD_DURATION:
		case INV_LIFESTEAL:
		case INV_MIT_INCREASE:
		case INV_MITEFFECT_INCREASE:
			if(showDetailedMods) {
				return StrParam(s:"+ ", s:col_tag, s:GetFixedRepresentation(val, false), s:GetDetailedModRange(attr, item_type, item_subtype, tier, FACTOR_FIXED_RESOLUTION, extra, false), s:"%", s:no_tag, l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+ ", s:col_tag, s:GetFixedRepresentation(val, false), s:"%", s:no_tag, l:text);

		// incursion special attribs
		case INV_INC_DOUBLEHPBONUS:
		return StrParam(s:"\cg-", d:attr_extra, s:"\c- ", s:ess_tag, l:text, s:"\n", s:ess_tag, l:"IATTR_TINC1S");

		case INV_INC_CRITFORDOT:
		return StrParam(s:ess_tag, l:text, s:"\n", s:ess_tag, l:"IATTR_TINC4S");
		case INV_INC_ALLOVERLOAD:
		return StrParam(s:ess_tag, l:text, s:col_tag, d:val, s:"%\n", s:ess_tag, l:"IATTR_TINC5S");
		case INV_INC_ESHIELDNOINTERRUPT:
		return StrParam(s:ess_tag, l:text, s:"\n", s:ess_tag, l:"IATTR_TINC6S");
		
		case INV_INC_PLUSPROJ:
		case INV_INC_PLUSTWOPROJ:
		return StrParam(s:ess_tag, l:text, s:col_tag, s:" ", l:GetWeaponTag(attr_extra >> 16), s:"\n", s:col_tag, d:((1.0 - (attr_extra & 0xFFFF)) * 100) >> 16, s:"% ", s:ess_tag, l:"IATTR_TINC7S", s:col_tag, l:GetWeaponTag(attr_extra >> 16));
		
		case INV_INC_INSTANTLIFESTEAL:
			if(showDetailedMods) {
				return StrParam(
					s:"+ ", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:no_tag, l:text,
					s:" - ", s:GetModTierText(tier, extra),
					s:"\n", s:col_tag, d:attr_extra, s:"% ", s:ess_tag, l:"IATTR_TINC9S"
				);
			}
			return StrParam(s:"+ ", s:col_tag, d:val, s:"% ", s:ess_tag, l:text, s:"\n", s:col_tag, d:attr_extra, s:"% ", s:ess_tag, l:"IATTR_TINC9S");
		
			case INV_INC_MITIGATIONTODODGE:
		return StrParam(s:ess_tag, l:text, s:"\n", s:ess_tag, l:"IATTR_TINC11S");

		// single text incursion
		case INV_INC_ACCURACYFORPRECISION:
		case INV_INC_HPREGENINTERRUPT:
		case INV_INC_ACCURACYREVERSED:
		return StrParam(s:ess_tag, l:text);
		
		case INV_INC_PASSIVEREGEN:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, s:GetFixedRepresentation(val, true), s:GetDetailedModRange(attr, item_type, item_subtype, tier, FACTOR_FIXED_RESOLUTION, extra, true), s:"% ", s:ess_tag, l:"IATTR_TINC14S1",
					s:ess_tag, s:" - ", s:GetModTierText(tier, extra),
					s:"\n", s:col_tag, d:attr_extra, s:"% ", s:ess_tag, l:"IATTR_TINC14S2"
				);
			}
			return StrParam(s:ess_tag, l:text, d:col_tag, s:GetFixedRepresentation(val, true), s:"% ", s:ess_tag, l:"IATTR_TINC14S1", s:"\n", s:col_tag, d:attr_extra, s:"% ", s:ess_tag, l:"IATTR_TINC14S2");
		
		case INV_INC_ENEMYRIPCHANCE:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"% ", s:ess_tag, l:"IATTR_TINC15S",
					s:ess_tag, s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, l:text, s:col_tag, d:val, s:"% ", s:ess_tag, l:"IATTR_TINC15S");
		case INV_INC_BLOCKPREVENTION:
		case INV_INC_INVERTRESISTANCES:
			if(showDetailedMods) {
				return StrParam(
					s:"+ ", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"% ", s:ess_tag, l:text,
					s:ess_tag, s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+ ", s:col_tag, d:val, s:"% ", s:ess_tag, l:text);
		case INV_INC_RIPPERSEXPLODE:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"% ", s:ess_tag, l:"IATTR_TINC17S",
					s:ess_tag, s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, l:text, s:col_tag, d:val, s:"% ", s:ess_tag, l:"IATTR_TINC17S");

		case INV_INC_PROJREVERSE:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, s:GetFixedRepresentation(val, false), s:GetDetailedModRange(attr, item_type, item_subtype, tier, FACTOR_FIXED_RESOLUTION, extra, false),
					s:ess_tag, l:"IATTR_TINC20S"
				);
			}
			return StrParam(s:ess_tag, l:text, s:col_tag, s:GetFixedRepresentation(val, false), s:ess_tag, l:"IATTR_TINC20S");

		case INV_INC_EXCESSCRIT:
		return StrParam(s:col_tag, s:GetFixedRepresentation(val, true), s:ess_tag, l:text, s:"\n", s:ess_tag, l:"IATTR_TINC21S");

		// default takes percentage values
		default:
			if(val > 0) {
				if(showDetailedMods) {
					return StrParam(
						s:"+ ", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:no_tag, l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"+ ", s:col_tag, d:val, s:"%", s:no_tag, l:text);
			}
			else if(val < 0) {
				if(showDetailedMods) {
					return StrParam(
						s:"- ", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:no_tag, l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"- \cg", d:val, s:"%", s:no_tag, l:text);
			}
	}
	return "";
}

// this definitely needs some better cleaning up for sure... in the future
str GetItemAttributeText(int attr, int item_type, int item_subtype, int val1, int val2 = -1, int tier = 0, bool showDetailedMods = false, int extra = -1, bool isFractured = false, int qual = 0) {
	// treat it as normal inv attribute range
	// check last essence as its an all encompassing range except exotics
	if(attr <= LAST_INCURSION_ATTRIBUTE)
		return ItemAttributeString(attr, item_type, item_subtype, val1, tier, showDetailedMods, extra, isFractured, qual, val2);

	if(qual) {
		if(!IsAttributeQualityException(attr)) {
			if(val1 < 100000) {
				val1 *= qual + 100;
				val1 /= 100;
			}
			else {
				val1 /= 100;
				val1 *= qual + 100;
			}

			if(val2 < 100000) {
				val2 *= qual + 100;
				val2 /= 100;
			}
			else {
				val2 /= 100;
				val2 *= qual + 100;
			}
		}
	}

	// if the item is unique extra is not -1
	str text = GetInventoryAttributeText(attr);
	switch(attr) {
		case INV_EX_FACTOR_SMALLCHARM:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:"\c[Q9]", d:val1 / 1000, s:".", d:(val1 / 100 % 10), d:(val1 / 10) % 10, s:GetDetailedModRange_Unique(tier, FACTOR_FIXED_RESOLUTION, extra, false), s:"\c-",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:"\c[Q9]", d:val1 / 1000, s:".", d:(val1 / 100 % 10), d:(val1 / 10) % 10);
		
		case INV_EX_CHANCE_HEALMISSINGONPAIN:
			if(showDetailedMods) {
				return StrParam(
					s:"\c[Q9]", d:val2, s:GetDetailedModRange_Unique(tier, 0, extra, false, true), s:"%\c- ",
					l:text, s:"\c[Q9]", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_RECOVERHPHURT",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"\c[Q9]", d:val2, s:"%\c- ", l:"IATTR_TX_CHANCE", l:text, s:"\c[Q9]", d:val1, s:"%\c- ", l:"IATTR_RECOVERHPHURT");
		
		case INV_EX_PHYSDAMAGEPER_FLATHEALTH:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:"\c[Q9]", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- ", l:"IATTR_MAXHEALTH",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:"\c[Q9]", d:val1, s:"\c- ", l:"IATTR_MAXHEALTH");
		
		case INV_EX_ONKILL_HEALMISSING:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:"\c[Q9]", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_PMISSHP",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:"\c[Q9]", d:val1, s:"%\c- ", l:"IATTR_PMISSHP");
		
		case INV_EX_ABILITY_RALLY:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:"\c[Q9]", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- ", l:"IATTR_RALLY",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:"\c[Q9]", d:val1, s:"\c- ", l:"IATTR_RALLY");
		
		case INV_EX_CRITIGNORERESCHANCE:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:"\c[Q9]", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_CHANCEIGNORERES",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:"\c[Q9]", d:val1, s:"%\c- ", l:"IATTR_CHANCEIGNORERES");
		
		// float factor stuff
		case INV_EX_MORECRIT_LIGHTNING:
			if(showDetailedMods) {
				return StrParam(
					s:"+ ", s:"\c[Q9]", s:GetFixedRepresentation(val1, true), s:GetDetailedModRange_Unique(tier, FACTOR_FIXED_RESOLUTION, extra, true), s:"%\c- ", l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+ ", s:"\c[Q9]", s:GetFixedRepresentation(val1, true), s:"%\c- ", l:text);

		// float factor that isnt a percentage by representation, but has % at the end
		case INV_EX_MOREDMGPEROVERHEAT:
			if(showDetailedMods) {
				return StrParam(
					s:"+ ", s:"\c[Q9]", s:GetFixedRepresentation(val1, false), s:GetDetailedModRange_Unique(tier, 1, extra, false), s:"%\c- ", l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+ ", s:"\c[Q9]", s:GetFixedRepresentation(val1, false), s:"%\c- ", l:text);

		case INV_EX_INTBONUSTOMELEE:
			if(showDetailedMods) {
				return StrParam(
					l:text, s: " ", s:"\c[Q9]x", s:GetFixedRepresentation(val1, false), s:GetDetailedModRange_Unique(tier, 1, extra, false),
					s:"\c- ", l:"IATTR_TX41S", s:"\n", l:"IATTR_TX41B", s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s: " ", s:"\c[Q9]x", s:GetFixedRepresentation(val1, false), s:"\c- ", l:"IATTR_TX41S", s:"\n", l:"IATTR_TX41B");
			
		case INV_EX_SOULWEPSPEN:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_MAGICRES",
					s:"\c- - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:"\c[Q9] ", d:val1, s:"%\c- ", l:"IATTR_MAGICRES");

		case INV_EX_SECONDEXPBONUS:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX8B", s:"\n",
					l:"IATTR_TX8S", s:"\cg", d:val2, s:"%\c-", l:"IATTR_TX8S2",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:"\c[Q9] ", d:val1, s:"%\c- ", l:"IATTR_TX8B", s:"\n", l:"IATTR_TX8S", s:"\cg", d:val2, s:"%\c-", l:"IATTR_TX8S2");

		case INV_EX_DEADEYEBONUS:
			if(showDetailedMods)
				return StrParam(l:text, s:"\n", l:"IATTR_TX33A", s:"\n", l:"IATTR_TX33B", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:text, s:"\n", l:"IATTR_TX33A", s:"\n", l:"IATTR_TX33B");

		case INV_EX_UNITY_RES_BONUS:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX_UNITY", s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX38", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX_UNITY", s:"\c[Q9] ", d:val1, s:"%\c- ", l:"IATTR_TX38");
		case INV_EX_UNITY_PEN_BONUS:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX_UNITY", s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX39", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX_UNITY", s:"\c[Q9] ", d:val1, s:"%\c- ", l:"IATTR_TX39");

		case INV_EX_ESEXPLOSIONHPDMG:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX44", s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX44S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX44", s:"\c[Q9] ", d:val1, s:"%\c- ", l:"IATTR_TX44S");
		case INV_EX_ESCHARGE_USEHP:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX45", s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX45S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX45", s:"\c[Q9] ", d:val1, s:"%\c- ", l:"IATTR_TX45S");

		case INV_EX_HPTOESHIELD:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX46", s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX46S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX46", s:"\c[Q9] ", d:val1, s:"%\c- ", l:"IATTR_TX46S");

		case INV_EX_RESPERESHIELD:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX49", s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- ", l:"IATTR_TX49S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX49", s:"\c[Q9] ", d:val1, s:"\c- ", l:"IATTR_TX49S");
		case INV_EX_ESHIELDONLYBLOCKPCT:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX50", s:"\cg ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\cg%\c- ", l:"IATTR_TX50S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX50", s:"\cg ", d:val1, s:"%\c- ", l:"IATTR_TX50S");

		case INV_EX_REFILLAMMOONMELEEKILL:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX51", s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX51", s:"\c[Q9] ", d:val1, s:"%");
		case INV_EX_SWAPFROMMELEECRIT:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX52", s:"\c[Q9] ", s:GetFixedRepresentation(val1, true), s:GetDetailedModRange_Unique(tier, FACTOR_FIXED_RESOLUTION, extra, true), s:"\c- ", l:"IATTR_TX52S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX52", s:"\c[Q9] ", s:GetFixedRepresentation(val1, true), s:"%\c- ", l:"IATTR_TX52S");

		case INV_EX_SOULPICKUPSINFAMMO:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX55", s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- ", l:"IATTR_TX55S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX55", s:"\c[Q9] ", d:val1, s:"\c- ", l:"IATTR_TX55S");

		case INV_EX_SOULPICKUPSONLYAMMO:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX56", s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- ", l:"IATTR_TX56S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX56", s:"\c[Q9] ", d:val1, s:"\c- ", l:"IATTR_TX56S");

		case INV_EX_STREXTRABONUSTOMELEE:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX57", s:"\c[Q9] ", s:GetFixedRepresentation(val1, false), s:GetDetailedModRange_Unique(tier, FACTOR_FIXED_RESOLUTION, extra, false), s:"\c- ", l:"IATTR_TX57S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX57", s:"\c[Q9] ", s:GetFixedRepresentation(val1, false), s:"\c- ", l:"IATTR_TX57S");

		case INV_EX_LESSDMGTAKENMAXOVERHEAT:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX62", s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX62S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX62", s:"\c[Q9] ", d:val1, s:"%\c- ", l:"IATTR_TX62S");

		// single text things, no mod ranges, just tier U
		case INV_EX_KNOCKBACK_IMMUNITY:
		case INV_EX_DOUBLE_HEALTHCAP:
		case INV_EX_BEHAVIOR_PELLETSFIRECIRCLE:
		case INV_EX_FORSHOW_BURSTGETSPELLETBONUS:
		case INV_EX_DMGREDUCE_SHAREWITHPETS:
		case INV_EX_SOULWEPS_FULLDAMAGE:
		case INV_EX_ABILITY_MONSTERSRIP:
		case INV_EX_BEHAVIOR_SPELLSFULLDAMAGE:
		case INV_EX_ABILITY_LUCKYCRIT:
		case INV_EX_CURSEIMMUNITY:
		case INV_EX_DAMAGPERMISSINGAMMO:
		case INV_EX_UNITY:
		case INV_EX_UNITY_NOBONUS:
		case INV_EX_ESCHARGE_DMGNOINTERRUPT:
		case INV_EX_HEALTHATONE:
		case INV_EX_ESHIELDFULLABSORB:
		case INV_EX_DEMONBARRIERS:
		case INV_EX_ELEPENHARMONY:
		case INV_EX_CANFIREOVERHEATED:
		case INV_EX_WEAPONSUSEHEALTH:
		case INV_EX_RIPPERSRIPALL:
		case INV_EX_MIRROROTHERMEDIUM:
			if(showDetailedMods)
				return StrParam(l:text, s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:text);
		
		case INV_EX_LIMITEDSMALLCHARMS:
			if(showDetailedMods) {
				// singular matters here
				if(val1 > 1)
					return StrParam(
						l:text, s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s: "\c- ", l:"IATTR_TX29_2",
						s:" - ", s:GetModTierText(tier, extra)
					);
				return StrParam(
					l:text, s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s: "\c- ", l:"IATTR_TX29_2S",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			if(val1 > 1)
				return StrParam(l:text, s:"\c[Q9] ", d:val1, s: "\c- ", l:"IATTR_TX29_2");
			return StrParam(l:text, s:"\c[Q9] ", d:val1, s: "\c- ", l:"IATTR_TX29_2S");
		
		case INV_EX_ALLSTATS:
		case INV_EX_FLATDMG_ALL:
		case INV_EX_FLATDOT:
		case INV_EX_FLATPERSHOTGUNOWNED:
			if(showDetailedMods) {
				return StrParam(
					s:"+ \c[Q9]", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- ", l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+ \c[Q9]", d:val1, s:"\c- ", l:text);
		
		// negative %
		case INV_EX_LESSHEALING:
			if(val1) {
				if(showDetailedMods) {
					return StrParam(
						s:"- \cg", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c[D4] ", l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"- \cg", d:val1, s:"%\c[D4] ", l:text);
			}
			return StrParam(l:text);
			
		// negative effects are shown with different color -- these are % ones of those, these are positive numerically
		case INV_EX_DMGINCREASE_TAKEN:
		case INV_EX_AMMOCOSTMULTIPLIER:
			if(showDetailedMods) {
				return StrParam(s:"+ \cg", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c[D4] ", l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+ \cg", d:val1, s:"%\c[D4] ", l:text);

		case INV_EX_MOREAMMOUSE:
			if(showDetailedMods)
				return StrParam(s:"\c[D4] ", l:text, s:" \cg", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c[D4] ", l:"IATTR_TX35S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(s:"\c[D4] ", l:text, s:" \cg", d:val1, s:"%\c[D4] ", l:"IATTR_TX35S");
			
		// single text negative effects
		case INV_EX_FORBID_ARMOR:
		case INV_EX_CANNOTPICKAMMO:
		case INV_EX_CANTFIRENONOVERHEAT:
		case INV_EX_RIPPERSONETIMEONLY:
			if(showDetailedMods) {
				return StrParam(s:"\c[D4]", l:text, s:"\c- - ", s:GetModTierText(tier, extra));
			}
			return StrParam(s:"\c[D4]", l:text);
		
		// default are %
		default:
			if(val1) {
				if(showDetailedMods) {
					return StrParam(
						s:"+ \c[Q9]", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"+ \c[Q9]", d:val1, s:"%\c- ", l:text);
			}
			return StrParam(l:text);
	}
	return "";
}

bool IsTagArmorException(int tag, int armor_type) {
	if(armor_type == DND_CRAFTABLEID_BODYARMOR || armor_type == DND_CRAFTABLEID_BOOT)
		return (tag & INV_ATTR_TAG_ATTACK) || (tag & INV_ATTR_TAG_DAMAGE) || (tag & INV_ATTR_TAG_CRIT) || (tag & INV_ATTR_TAG_MELEE);
	else if(armor_type == DND_CRAFTABLEID_HELM)
		return (tag & INV_ATTR_TAG_ATTACK) || (tag & INV_ATTR_TAG_DAMAGE);
	else if(armor_type == DND_CRAFTABLEID_SPECIALTY_WANDERER)
		return (tag & INV_ATTR_TAG_PHYSICAL);
	else if(armor_type == DND_CRAFTABLEID_SPECIALTY_BERSERKER)
		return (tag & INV_ATTR_TAG_EXPLOSIVE) || (tag & INV_ATTR_TAG_ENERGY);
	else if(armor_type == DND_CRAFTABLEID_SPECIALTY_TRICKSTER)
		return (tag & INV_ATTR_TAG_DEFENSE) || (tag & INV_ATTR_TAG_LIFE);

	// generic cant have occult
	return (tag & INV_ATTR_TAG_OCCULT);
}

bool IsAttributeArmorException(int attr, int armor_type) {
	return IsTagArmorException(ItemModTable[attr].tags, armor_type);
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

#endif
