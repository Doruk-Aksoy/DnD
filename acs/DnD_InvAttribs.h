#ifndef DND_INV_ATTRIBS_IN
#define DND_INV_ATTRIBS_IN

#include "DnD_InvInfo.h"

#define MAX_CHARM_AFFIXTIERS 10
#define CHARM_ATTRIBLEVEL_SEPERATOR 10 // just leave this as is... its basically every 10 levels a new tier is named for it

#define CHARMSTR_COLORCODE 0
#define CHARMSTR_TIERTAG 1

// level 100 = perfect
str Charm_Strings[MAX_CHARM_AFFIXTIERS + 1][2] = {
	{ "\c[C8]", "DND_CHARMTIER1" },
	{ "\c[D5]", "DND_CHARMTIER2" },
	{ "\c[A3]", "DND_CHARMTIER3" },
	{ "\c[C3]", "DND_CHARMTIER4" },
	{ "\c[E9]", "DND_CHARMTIER5" },
	{ "\c[K9]", "DND_CHARMTIER6" },
	{ "\c[E2]", "DND_CHARMTIER7" },
	{ "\c[C5]", "DND_CHARMTIER8" },
	{ "\c[W9]", "DND_CHARMTIER9" },
	{ "\c[Y9]", "DND_CHARMTIER10" },
	{ "\c[L7]", "DND_CHARMTIER11" },
};

// formula for accuracy:
// every 10 points contribute 0.01%
// applied as follows: X = spread of weapon, X * (1.0 - accuracy * factor_per_point), capped at 100 000 accuracy (100%)

#define DND_ATTRIBUTEBONUS_CAP 1024

//#define MAX_ATTRIB_MODIFIER 0xFFFFFFFF

// these two imply we have 1000 regular charm mods potentially, 1000 essence and unlimited unique mods
// reason we have these generous ranges is so when a new mod is added, database resets do not need to happen
#define ESSENCE_ATTRIB_ID_BEGIN 1000
#define UNIQUE_ATTRIB_ID_BEGIN 2000
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
	INV_FLATEXP_DAMAGE,
	INV_FLATMAGIC_DAMAGE,
	INV_FLATELEM_DAMAGE,
	
	INV_PERCENTPHYS_DAMAGE,
	INV_PERCENTENERGY_DAMAGE,
	INV_PERCENTEXP_DAMAGE,
	INV_PERCENTMAGIC_DAMAGE,
	INV_PERCENTELEM_DAMAGE,
	
	INV_SLOT1_DAMAGE,
	INV_SLOT2_DAMAGE,
	INV_SLOT3_DAMAGE,
	INV_SLOT4_DAMAGE,
	INV_SLOT5_DAMAGE,
	INV_SLOT6_DAMAGE,
	INV_SLOT7_DAMAGE,
	INV_SLOT8_DAMAGE,
	INV_TEMPWEP_DAMAGE,
	
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
	INV_STAT_BULKINESS,
	INV_STAT_CHARISMA,
	INV_STAT_VITALITY,
	INV_STAT_INTELLECT,
	
	INV_DMGREDUCE_ELEM,
	INV_DMGREDUCE_PHYS,
	INV_DMGREDUCE_REFL,
	
	INV_PEN_PHYSICAL,
	INV_PEN_ENERGY,
	INV_PEN_EXPLOSIVE,
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
	
	INV_DMGREDUCE_HITSCAN,
	INV_DMGREDUCE_ENERGY,
	INV_DMGREDUCE_EXPLOSION,
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
	// add new regular rollable attributes here
	
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
	
	// below here are exotic attributes not found in normal items, if you add new attributes do so to above and change MAX_INV_ATTRIBUTE_TYPES
	INV_EX_CHANCE = UNIQUE_ATTRIB_ID_BEGIN, // this is the generic "chance to do X" thing, the starter attribute, any effect that use this will come immediately after it
	INV_EX_CHANCE_CASTELEMSPELLONATK,
	INV_EX_KNOCKBACK_IMMUNITY,
	INV_EX_FACTOR_SMALLCHARM,
	INV_EX_ALLSTATS,
	INV_EX_CHANCE_HEALMISSINGONPAIN,
	INV_EX_DMGINCREASE_LIGHTNING,
	INV_EX_MORECRIT_LIGHTNING,
	INV_EX_DMGINCREASE_SHOTGUNS,
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
	INV_EX_LESSHEALING
	// add new unique attributes here
};

// attributes below last_inv (normal rollables) are exotic
#define FIRST_INV_ATTRIBUTE INV_HP_INCREASE
#define LAST_INV_ATTRIBUTE INV_CHANCE_AILMENTIGNORE
#define NORMAL_ATTRIBUTE_COUNT (LAST_INV_ATTRIBUTE - FIRST_INV_ATTRIBUTE + 1)
// modify the above to make it use the negative last
//#define NEGATIVE_ATTRIB_BEGIN INV_NEG_DAMAGE_DEALT
#define UNIQUE_ATTRIB_BEGIN INV_EX_CHANCE
#define UNIQUE_ATTRIB_END INV_EX_LESSHEALING
#define UNIQUE_ATTRIB_COUNT (UNIQUE_ATTRIB_END - UNIQUE_ATTRIB_BEGIN + 1)

#define FIRST_ESSENCE_ATTRIBUTE INV_ESS_VAAJ
#define LAST_ESSENCE_ATTRIBUTE INV_ESS_ERYXIA
#define ESSENCE_ATTRIBUTE_COUNT (LAST_ESSENCE_ATTRIBUTE - FIRST_ESSENCE_ATTRIBUTE + 1)
#define ESSENCE_MAP_MACRO(X) ((X) - FIRST_ESSENCE_ATTRIBUTE + 1)

#define MAX_INV_ATTRIBUTE_TYPES (NORMAL_ATTRIBUTE_COUNT + ESSENCE_ATTRIBUTE_COUNT)
#define UNIQUE_MAP_MACRO(X) ((X) - UNIQUE_ATTRIB_ID_BEGIN + 1)
#define MAX_TOTAL_ATTRIBUTES 3000 // 1000 for each of: regular, essence, unique mods.

typedef struct {
	int attrib_low;
	int attrib_high;
	int attrib_level_modifier;
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
	INV_ATTR_TAG_MELEE = 4096
};

// indexing on this one is done by checking ranges, and then mapping appropriately
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
		case INV_DMGREDUCE_ELEM:
		case INV_DMGREDUCE_ENERGY:
		case INV_DMGREDUCE_EXPLOSION:
		case INV_DMGREDUCE_HITSCAN:
		case INV_DMGREDUCE_PHYS:
		case INV_DMGREDUCE_MAGIC:
		case INV_DMGREDUCE_FIRE:
		case INV_DMGREDUCE_ICE:
		case INV_DMGREDUCE_LIGHTNING:
		case INV_DMGREDUCE_POISON:
		case INV_DMGREDUCE_REFL:
		case INV_ADDEDMAXRESIST:
		case INV_OVERLOAD_DURATION:
		case INV_OVERLOAD_DMGINCREASE:
		case INV_LIFESTEAL:
		case INV_LIFESTEAL_DAMAGE:
		case INV_ESS_ERYXIA:
		case INV_EX_MORECRIT_LIGHTNING:
		return true;
	}
	return false;
}

void SetPlayerModValue(int pnum, int mod, int val, bool noSync = false) {
	//printbold(s:"mod: ", d:mod, s:" ", d:PlayerModValues[pnum][mod], s: " = ", d:val);
	PlayerModValues[pnum][mod] = val;
	
	if(!noSync)
		ACS_NamedExecuteWithResult("DnD Request Mod Sync", pnum, mod, PlayerModValues[pnum][mod]);
}

void IncPlayerModValue(int pnum, int mod, int val, bool noSync = false) {
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
	
	if(!noSync)
		ACS_NamedExecuteWithResult("DnD Request Mod Sync", pnum, mod, PlayerModValues[pnum][mod]);
}

void ResetPlayerModList(int pnum) {
	for(int i = 0; i < MAX_TOTAL_ATTRIBUTES; ++i)
		PlayerModValues[pnum][i] = 0;
	ACS_NamedExecuteWithResult("DnD Reset Player Mod List", pnum);
}

void SyncPlayerItemMods(int pnum) {
	for(int i = 0; i < MAX_TOTAL_ATTRIBUTES; ++i)
		if(PlayerModValues[pnum][i])
			ACS_NamedExecuteWithResult("DnD Request Mod Sync", pnum, i, PlayerModValues[pnum][i]);
}

// resets things clientside for the array
Script "DnD Reset Player Mod List" (int pnum) CLIENTSIDE {
	for(int i = 0; i < MAX_TOTAL_ATTRIBUTES; ++i)
		PlayerModValues[pnum][i] = 0;
}

// reason why this uses UNIQUE_ATTRIB_ID_BEGIN, it skips regular and essence mod indexes. This means, we have enough room without database reset for both regular
// and essence mods. This is good for future compatibility as well.
// Currently: 2000 mods, 0-1999
global Inv_attrib_T 62: ItemModTable[UNIQUE_ATTRIB_ID_BEGIN];

void SetupInventoryAttributeTable() {
	// doing this so we populate the global arrays (acs wont allow just initializing them like local variables)
	// and these are global so we move from stack to heap and also only initialize this once when server is launched
	// and never again as opposed to start of every map with local variable method
	ItemModTable[INV_HP_INCREASE].attrib_low = 5;
	ItemModTable[INV_HP_INCREASE].attrib_high = 14;
	ItemModTable[INV_HP_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_HP_INCREASE].tags = INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_ARMOR_INCREASE].attrib_low = 5;
	ItemModTable[INV_ARMOR_INCREASE].attrib_high = 14;
	ItemModTable[INV_ARMOR_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_ARMOR_INCREASE].tags = INV_ATTR_TAG_DEFENSE;
	
	ItemModTable[INV_HPPERCENT_INCREASE].attrib_low = 1;
	ItemModTable[INV_HPPERCENT_INCREASE].attrib_high = 6;
	ItemModTable[INV_HPPERCENT_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_HPPERCENT_INCREASE].tags = INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_ARMORPERCENT_INCREASE].attrib_low = 1;
	ItemModTable[INV_ARMORPERCENT_INCREASE].attrib_high = 6;
	ItemModTable[INV_ARMORPERCENT_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_ARMORPERCENT_INCREASE].tags = INV_ATTR_TAG_DEFENSE;
	
	ItemModTable[INV_EXPGAIN_INCREASE].attrib_low = 0.05;
	ItemModTable[INV_EXPGAIN_INCREASE].attrib_high = 0.09;
	ItemModTable[INV_EXPGAIN_INCREASE].attrib_level_modifier = 0.05;
	ItemModTable[INV_EXPGAIN_INCREASE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_CREDITGAIN_INCREASE].attrib_low = 0.05;
	ItemModTable[INV_CREDITGAIN_INCREASE].attrib_high = 0.09;
	ItemModTable[INV_CREDITGAIN_INCREASE].attrib_level_modifier = 0.05;
	ItemModTable[INV_CREDITGAIN_INCREASE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_DROPCHANCE_INCREASE].attrib_low = 0.005;
	ItemModTable[INV_DROPCHANCE_INCREASE].attrib_high = 0.015;
	ItemModTable[INV_DROPCHANCE_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_DROPCHANCE_INCREASE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_LUCK_INCREASE].attrib_low = 0.025;
	ItemModTable[INV_LUCK_INCREASE].attrib_high = 0.05;
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
	ItemModTable[INV_MAGAZINE_INCREASE].attrib_high = 9;
	ItemModTable[INV_MAGAZINE_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_MAGAZINE_INCREASE].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_FLATPHYS_DAMAGE].attrib_low = 1;
	ItemModTable[INV_FLATPHYS_DAMAGE].attrib_high = 3;
	ItemModTable[INV_FLATPHYS_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_FLATPHYS_DAMAGE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_PHYSICAL;
	
	ItemModTable[INV_FLATENERGY_DAMAGE].attrib_low = 1;
	ItemModTable[INV_FLATENERGY_DAMAGE].attrib_high = 3;
	ItemModTable[INV_FLATENERGY_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_FLATENERGY_DAMAGE].tags = INV_ATTR_TAG_ATTACK	| INV_ATTR_TAG_ENERGY;
	
	ItemModTable[INV_FLATEXP_DAMAGE].attrib_low = 1;
	ItemModTable[INV_FLATEXP_DAMAGE].attrib_high = 3;
	ItemModTable[INV_FLATEXP_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_FLATEXP_DAMAGE].tags = INV_ATTR_TAG_ATTACK	| INV_ATTR_TAG_EXPLOSIVE;
	
	ItemModTable[INV_FLATMAGIC_DAMAGE].attrib_low = 1;
	ItemModTable[INV_FLATMAGIC_DAMAGE].attrib_high = 3;
	ItemModTable[INV_FLATMAGIC_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_FLATMAGIC_DAMAGE].tags = INV_ATTR_TAG_ATTACK	| INV_ATTR_TAG_OCCULT;
	
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
	
	ItemModTable[INV_PERCENTEXP_DAMAGE].attrib_low = 5;
	ItemModTable[INV_PERCENTEXP_DAMAGE].attrib_high = 15;
	ItemModTable[INV_PERCENTEXP_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTEXP_DAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_EXPLOSIVE;
	
	ItemModTable[INV_PERCENTMAGIC_DAMAGE].attrib_low = 5;
	ItemModTable[INV_PERCENTMAGIC_DAMAGE].attrib_high = 15;
	ItemModTable[INV_PERCENTMAGIC_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTMAGIC_DAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_OCCULT;
	
	ItemModTable[INV_PERCENTELEM_DAMAGE].attrib_low = 5;
	ItemModTable[INV_PERCENTELEM_DAMAGE].attrib_high = 15;
	ItemModTable[INV_PERCENTELEM_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_PERCENTELEM_DAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_ELEMENTAL;

	ItemModTable[INV_SLOT1_DAMAGE].attrib_low = 5;
	ItemModTable[INV_SLOT1_DAMAGE].attrib_high = 25;
	ItemModTable[INV_SLOT1_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_SLOT1_DAMAGE].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_SLOT2_DAMAGE].attrib_low = 5;
	ItemModTable[INV_SLOT2_DAMAGE].attrib_high = 25;
	ItemModTable[INV_SLOT2_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_SLOT2_DAMAGE].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_SLOT3_DAMAGE].attrib_low = 5;
	ItemModTable[INV_SLOT3_DAMAGE].attrib_high = 25;
	ItemModTable[INV_SLOT3_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_SLOT3_DAMAGE].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_SLOT4_DAMAGE].attrib_low = 5;
	ItemModTable[INV_SLOT4_DAMAGE].attrib_high = 25;
	ItemModTable[INV_SLOT4_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_SLOT4_DAMAGE].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_SLOT5_DAMAGE].attrib_low = 5;
	ItemModTable[INV_SLOT5_DAMAGE].attrib_high = 25;
	ItemModTable[INV_SLOT5_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_SLOT5_DAMAGE].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_SLOT6_DAMAGE].attrib_low = 5;
	ItemModTable[INV_SLOT6_DAMAGE].attrib_high = 25;
	ItemModTable[INV_SLOT6_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_SLOT6_DAMAGE].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_SLOT7_DAMAGE].attrib_low = 5;
	ItemModTable[INV_SLOT7_DAMAGE].attrib_high = 25;
	ItemModTable[INV_SLOT7_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_SLOT7_DAMAGE].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_SLOT8_DAMAGE].attrib_low = 5;
	ItemModTable[INV_SLOT8_DAMAGE].attrib_high = 25;
	ItemModTable[INV_SLOT8_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_SLOT8_DAMAGE].tags = INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_OCCULT;
	
	ItemModTable[INV_TEMPWEP_DAMAGE].attrib_low = 5;
	ItemModTable[INV_TEMPWEP_DAMAGE].attrib_high = 25;
	ItemModTable[INV_TEMPWEP_DAMAGE].attrib_level_modifier = 0;
	ItemModTable[INV_TEMPWEP_DAMAGE].tags = INV_ATTR_TAG_DAMAGE;
	
	ItemModTable[INV_PELLET_INCREASE].attrib_low = 0.02;
	ItemModTable[INV_PELLET_INCREASE].attrib_high = 0.04;
	ItemModTable[INV_PELLET_INCREASE].attrib_level_modifier = 0.02;
	ItemModTable[INV_PELLET_INCREASE].tags = INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_ATTACK;
	
	ItemModTable[INV_EXPLOSION_RADIUS].attrib_low = 1;
	ItemModTable[INV_EXPLOSION_RADIUS].attrib_high = 8;
	ItemModTable[INV_EXPLOSION_RADIUS].attrib_level_modifier = 0;
	ItemModTable[INV_EXPLOSION_RADIUS].tags = INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_EXPLOSIVE;
	
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
	
	ItemModTable[INV_REGENCAP_INCREASE].attrib_low = 5;
	ItemModTable[INV_REGENCAP_INCREASE].attrib_high = 14;
	ItemModTable[INV_REGENCAP_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_REGENCAP_INCREASE].tags = INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_CRITCHANCE_INCREASE].attrib_low = 0.005;
	ItemModTable[INV_CRITCHANCE_INCREASE].attrib_high = 0.01;
	ItemModTable[INV_CRITCHANCE_INCREASE].attrib_level_modifier = 0.006;
	ItemModTable[INV_CRITCHANCE_INCREASE].tags = INV_ATTR_TAG_CRIT;
	
	ItemModTable[INV_CRITPERCENT_INCREASE].attrib_low = 0.01;
	ItemModTable[INV_CRITPERCENT_INCREASE].attrib_high = 0.08;
	ItemModTable[INV_CRITPERCENT_INCREASE].attrib_level_modifier = 0.08;
	ItemModTable[INV_CRITPERCENT_INCREASE].tags = INV_ATTR_TAG_CRIT;
	
	ItemModTable[INV_CRITDAMAGE_INCREASE].attrib_low = 5;
	ItemModTable[INV_CRITDAMAGE_INCREASE].attrib_high = 14;
	ItemModTable[INV_CRITDAMAGE_INCREASE].attrib_level_modifier = 0;
	ItemModTable[INV_CRITDAMAGE_INCREASE].tags = INV_ATTR_TAG_CRIT;
	
	ItemModTable[INV_KNOCKBACK_RESIST].attrib_low = 50;
	ItemModTable[INV_KNOCKBACK_RESIST].attrib_high = 100;
	ItemModTable[INV_KNOCKBACK_RESIST].attrib_level_modifier = 100;
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
	
	ItemModTable[INV_STAT_BULKINESS].attrib_low = 1;
	ItemModTable[INV_STAT_BULKINESS].attrib_high = 4;
	ItemModTable[INV_STAT_BULKINESS].attrib_level_modifier = 0;
	ItemModTable[INV_STAT_BULKINESS].tags = INV_ATTR_TAG_STAT;
	
	ItemModTable[INV_STAT_CHARISMA].attrib_low = 1;
	ItemModTable[INV_STAT_CHARISMA].attrib_high = 4;
	ItemModTable[INV_STAT_CHARISMA].attrib_level_modifier = 0;
	ItemModTable[INV_STAT_CHARISMA].tags = INV_ATTR_TAG_STAT;
	
	ItemModTable[INV_STAT_VITALITY].attrib_low = 1;
	ItemModTable[INV_STAT_VITALITY].attrib_high = 4;
	ItemModTable[INV_STAT_VITALITY].attrib_level_modifier = 0;
	ItemModTable[INV_STAT_VITALITY].tags = INV_ATTR_TAG_STAT;
	
	ItemModTable[INV_STAT_INTELLECT].attrib_low = 1;
	ItemModTable[INV_STAT_INTELLECT].attrib_high = 4;
	ItemModTable[INV_STAT_INTELLECT].attrib_level_modifier = 0;
	ItemModTable[INV_STAT_INTELLECT].tags = INV_ATTR_TAG_STAT;
	
	ItemModTable[INV_DMGREDUCE_ELEM].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_ELEM].attrib_high = 4.0;
	ItemModTable[INV_DMGREDUCE_ELEM].attrib_level_modifier = 0;
	ItemModTable[INV_DMGREDUCE_ELEM].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_DMGREDUCE_PHYS].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_PHYS].attrib_high = 4.0;
	ItemModTable[INV_DMGREDUCE_PHYS].attrib_level_modifier = 0;
	ItemModTable[INV_DMGREDUCE_PHYS].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_PHYSICAL;
	
	ItemModTable[INV_DMGREDUCE_REFL].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_REFL].attrib_high = 4.0;
	ItemModTable[INV_DMGREDUCE_REFL].attrib_level_modifier = 0;
	ItemModTable[INV_DMGREDUCE_REFL].tags = INV_ATTR_TAG_DEFENSE;

	ItemModTable[INV_PEN_PHYSICAL].attrib_low = 1;
	ItemModTable[INV_PEN_PHYSICAL].attrib_high = 3;
	ItemModTable[INV_PEN_PHYSICAL].attrib_level_modifier = 0;
	ItemModTable[INV_PEN_PHYSICAL].tags = INV_ATTR_TAG_ATTACK	| INV_ATTR_TAG_PHYSICAL;
	
	ItemModTable[INV_PEN_ENERGY].attrib_low = 1;
	ItemModTable[INV_PEN_ENERGY].attrib_high = 3;
	ItemModTable[INV_PEN_ENERGY].attrib_level_modifier = 0;
	ItemModTable[INV_PEN_ENERGY].tags = INV_ATTR_TAG_ATTACK	| INV_ATTR_TAG_ENERGY;
	
	ItemModTable[INV_PEN_EXPLOSIVE].attrib_low = 1;
	ItemModTable[INV_PEN_EXPLOSIVE].attrib_high = 3;
	ItemModTable[INV_PEN_EXPLOSIVE].attrib_level_modifier = 0;
	ItemModTable[INV_PEN_EXPLOSIVE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_EXPLOSIVE;
	
	ItemModTable[INV_PEN_OCCULT].attrib_low = 1;
	ItemModTable[INV_PEN_OCCULT].attrib_high = 3;
	ItemModTable[INV_PEN_OCCULT].attrib_level_modifier = 0;
	ItemModTable[INV_PEN_OCCULT].tags = INV_ATTR_TAG_ATTACK	| INV_ATTR_TAG_OCCULT;
	
	ItemModTable[INV_PEN_ELEMENTAL].attrib_low = 1;
	ItemModTable[INV_PEN_ELEMENTAL].attrib_high = 3;
	ItemModTable[INV_PEN_ELEMENTAL].attrib_level_modifier = 0;
	ItemModTable[INV_PEN_ELEMENTAL].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL;

	ItemModTable[INV_FLAT_FIREDMG].attrib_low = 1;
	ItemModTable[INV_FLAT_FIREDMG].attrib_high = 4;
	ItemModTable[INV_FLAT_FIREDMG].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_FIREDMG].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_FLAT_ICEDMG].attrib_low = 1;
	ItemModTable[INV_FLAT_ICEDMG].attrib_high = 4;
	ItemModTable[INV_FLAT_ICEDMG].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_ICEDMG].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_FLAT_LIGHTNINGDMG].attrib_low = 1;
	ItemModTable[INV_FLAT_LIGHTNINGDMG].attrib_high = 4;
	ItemModTable[INV_FLAT_LIGHTNINGDMG].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_LIGHTNINGDMG].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_FLAT_POISONDMG].attrib_low = 1;
	ItemModTable[INV_FLAT_POISONDMG].attrib_high = 4;
	ItemModTable[INV_FLAT_POISONDMG].attrib_level_modifier = 0;
	ItemModTable[INV_FLAT_POISONDMG].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_LIFESTEAL].attrib_low = 0.05;
	ItemModTable[INV_LIFESTEAL].attrib_high = 0.125;
	ItemModTable[INV_LIFESTEAL].attrib_level_modifier = 0;
	ItemModTable[INV_LIFESTEAL].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_POISON_TICRATE].attrib_low = 5;
	ItemModTable[INV_POISON_TICRATE].attrib_high = 14;
	ItemModTable[INV_POISON_TICRATE].attrib_level_modifier = 0;
	ItemModTable[INV_POISON_TICRATE].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_POISON_DURATION].attrib_low = 5;
	ItemModTable[INV_POISON_DURATION].attrib_high = 14;
	ItemModTable[INV_POISON_DURATION].attrib_level_modifier = 0;
	ItemModTable[INV_POISON_DURATION].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_POISON_TICDMG].attrib_low = 5;
	ItemModTable[INV_POISON_TICDMG].attrib_high = 20;
	ItemModTable[INV_POISON_TICDMG].attrib_level_modifier = 0;
	ItemModTable[INV_POISON_TICDMG].tags = INV_ATTR_TAG_ELEMENTAL;
	
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
	ItemModTable[INV_SLOWEFFECT].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_CHILLTHRESHOLD].attrib_low = 1;
	ItemModTable[INV_CHILLTHRESHOLD].attrib_high = 5;
	ItemModTable[INV_CHILLTHRESHOLD].attrib_level_modifier = 0;
	ItemModTable[INV_CHILLTHRESHOLD].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_IGNITECHANCE].attrib_low = 5;
	ItemModTable[INV_IGNITECHANCE].attrib_high = 14;
	ItemModTable[INV_IGNITECHANCE].attrib_level_modifier = 0;
	ItemModTable[INV_IGNITECHANCE].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_IGNITEDMG].attrib_low = 5;
	ItemModTable[INV_IGNITEDMG].attrib_high = 14;
	ItemModTable[INV_IGNITEDMG].attrib_level_modifier = 0;
	ItemModTable[INV_IGNITEDMG].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_IGNITEDURATION].attrib_low = 4;
	ItemModTable[INV_IGNITEDURATION].attrib_high = 12;
	ItemModTable[INV_IGNITEDURATION].attrib_level_modifier = 0;
	ItemModTable[INV_IGNITEDURATION].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_OVERLOADCHANCE].attrib_low = 5;
	ItemModTable[INV_OVERLOADCHANCE].attrib_high = 14;
	ItemModTable[INV_OVERLOADCHANCE].attrib_level_modifier = 0;
	ItemModTable[INV_OVERLOADCHANCE].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_OVERLOAD_ZAPCOUNT].attrib_low = 1;
	ItemModTable[INV_OVERLOAD_ZAPCOUNT].attrib_high = 1;
	ItemModTable[INV_OVERLOAD_ZAPCOUNT].attrib_level_modifier = 1;
	ItemModTable[INV_OVERLOAD_ZAPCOUNT].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_OVERLOAD_DMGINCREASE].attrib_low = 0.01;
	ItemModTable[INV_OVERLOAD_DMGINCREASE].attrib_high = 0.04;
	ItemModTable[INV_OVERLOAD_DMGINCREASE].attrib_level_modifier = 0.04;
	ItemModTable[INV_OVERLOAD_DMGINCREASE].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_CYBERNETIC].attrib_low = 1;
	ItemModTable[INV_CYBERNETIC].attrib_high = 1;
	ItemModTable[INV_CYBERNETIC].attrib_level_modifier = 1;
	ItemModTable[INV_CYBERNETIC].tags = INV_ATTR_TAG_UTILITY;
	
	ItemModTable[INV_MELEERANGE].attrib_low = 2;
	ItemModTable[INV_MELEERANGE].attrib_high = 6;
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
	
	ItemModTable[INV_DMGREDUCE_HITSCAN].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_HITSCAN].attrib_high = 4.0;
	ItemModTable[INV_DMGREDUCE_HITSCAN].attrib_level_modifier = 0;
	ItemModTable[INV_DMGREDUCE_HITSCAN].tags = INV_ATTR_TAG_DEFENSE;
	
	ItemModTable[INV_DMGREDUCE_ENERGY].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_ENERGY].attrib_high = 4.0;
	ItemModTable[INV_DMGREDUCE_ENERGY].attrib_level_modifier = 0;
	ItemModTable[INV_DMGREDUCE_ENERGY].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ENERGY;
	
	ItemModTable[INV_DMGREDUCE_EXPLOSION].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_EXPLOSION].attrib_high = 4.0;
	ItemModTable[INV_DMGREDUCE_EXPLOSION].attrib_level_modifier = 0;
	ItemModTable[INV_DMGREDUCE_EXPLOSION].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_EXPLOSIVE;
	
	ItemModTable[INV_DMGREDUCE_MAGIC].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_MAGIC].attrib_high = 4.0;
	ItemModTable[INV_DMGREDUCE_MAGIC].attrib_level_modifier = 0;
	ItemModTable[INV_DMGREDUCE_MAGIC].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_OCCULT;
	
	ItemModTable[INV_DMGREDUCE_FIRE].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_FIRE].attrib_high = 4.0;
	ItemModTable[INV_DMGREDUCE_FIRE].attrib_level_modifier = 0;
	ItemModTable[INV_DMGREDUCE_FIRE].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_DMGREDUCE_ICE].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_ICE].attrib_high = 4.0;
	ItemModTable[INV_DMGREDUCE_ICE].attrib_level_modifier = 0;
	ItemModTable[INV_DMGREDUCE_ICE].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_DMGREDUCE_LIGHTNING].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_LIGHTNING].attrib_high = 4.0;
	ItemModTable[INV_DMGREDUCE_LIGHTNING].attrib_level_modifier = 0;
	ItemModTable[INV_DMGREDUCE_LIGHTNING].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_DMGREDUCE_POISON].attrib_low = 1.0;
	ItemModTable[INV_DMGREDUCE_POISON].attrib_high = 4.0;
	ItemModTable[INV_DMGREDUCE_POISON].attrib_level_modifier = 0;
	ItemModTable[INV_DMGREDUCE_POISON].tags = INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_ADDEDMAXRESIST].attrib_low = 0.25;
	ItemModTable[INV_ADDEDMAXRESIST].attrib_high = 0.5;
	ItemModTable[INV_ADDEDMAXRESIST].attrib_level_modifier = 0;
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
	
	ItemModTable[INV_LIFESTEAL_DAMAGE].attrib_low = 0.02;
	ItemModTable[INV_LIFESTEAL_DAMAGE].attrib_high = 0.08;
	ItemModTable[INV_LIFESTEAL_DAMAGE].attrib_level_modifier = 0.07;
	ItemModTable[INV_LIFESTEAL_DAMAGE].tags = INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_LIFE;
	
	ItemModTable[INV_OVERLOAD_DURATION].attrib_low = 0.1;
	ItemModTable[INV_OVERLOAD_DURATION].attrib_high = 0.5;
	ItemModTable[INV_OVERLOAD_DURATION].attrib_level_modifier = 0;
	ItemModTable[INV_OVERLOAD_DURATION].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_IGNITE_PROLIFCHANCE].attrib_low = 5;
	ItemModTable[INV_IGNITE_PROLIFCHANCE].attrib_high = 14;
	ItemModTable[INV_IGNITE_PROLIFCHANCE].attrib_level_modifier = 0;
	ItemModTable[INV_IGNITE_PROLIFCHANCE].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_IGNITE_PROLIFCOUNT].attrib_low = 1;
	ItemModTable[INV_IGNITE_PROLIFCOUNT].attrib_high = 1;
	ItemModTable[INV_IGNITE_PROLIFCOUNT].attrib_level_modifier = 1;
	ItemModTable[INV_IGNITE_PROLIFCOUNT].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_IGNITE_PROLIFRANGE].attrib_low = 5;
	ItemModTable[INV_IGNITE_PROLIFRANGE].attrib_high = 14;
	ItemModTable[INV_IGNITE_PROLIFRANGE].attrib_level_modifier = 0;
	ItemModTable[INV_IGNITE_PROLIFRANGE].tags = INV_ATTR_TAG_ELEMENTAL;
	
	ItemModTable[INV_CHANCE_AILMENTIGNORE].attrib_low = 5;
	ItemModTable[INV_CHANCE_AILMENTIGNORE].attrib_high = 9;
	ItemModTable[INV_CHANCE_AILMENTIGNORE].attrib_level_modifier = 0;
	ItemModTable[INV_CHANCE_AILMENTIGNORE].tags = INV_ATTR_TAG_ELEMENTAL;
	
	// essences from here on out

	ItemModTable[INV_ESS_VAAJ].attrib_low = 1;
	ItemModTable[INV_ESS_VAAJ].attrib_high = 1;
	ItemModTable[INV_ESS_VAAJ].attrib_level_modifier = 1;
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
}

// returns the amount to skip over the base range to map it into its appropriate tier
int GetModTierRangeMapper(int attr, int lvl) {
	int val = 0;
	if(!ItemModTable[attr].attrib_level_modifier)
		val = (ItemModTable[attr].attrib_high - ItemModTable[attr].attrib_low) * lvl;
	else
		val = (ItemModTable[attr].attrib_level_modifier * lvl);
	return val;
}

#define ITEM_MODRANGE_LOW 0
#define ITEM_MODRANGE_HIGH 1

// this uses a precalculated tier mapping to save time
int GetModRangeWithTier(int attr, int tier_mapping, bool which) {
	if(!which)
		return ItemModTable[attr].attrib_low + tier_mapping + (tier_mapping != 0);
	return ItemModTable[attr].attrib_high + tier_mapping + (tier_mapping != 0);
}

// this calculates the tier mapping for itself based on supplied level
int GetModRange(int attr, int tier, bool which) {
	return GetModRangeWithTier(attr, GetModTierRangeMapper(attr, tier), which);
}

// rolls an attribute's value depending on specified parameters
// if they are fixed, put the truncated value in to match (as closely as possible...) what the menu displays in stat gains
int RollAttributeValue(int attr, int tier, bool isWellRolled) {
	int tier_mapping = GetModTierRangeMapper(attr, tier);

	if(!isWellRolled)
		return random(GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW), GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH));
	int high = GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH);
	return random((GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW) + high) / 2, high);
}

int RollUniqueAttributeValue(int unique_id, int attr, bool isWellRolled) {
	if(!isWellRolled)
		return random(UniqueItemList[unique_id].rolls[attr].attrib_low, UniqueItemList[unique_id].rolls[attr].attrib_high);
	return random((UniqueItemList[unique_id].rolls[attr].attrib_low + UniqueItemList[unique_id].rolls[attr].attrib_high) / 2, UniqueItemList[unique_id].rolls[attr].attrib_high);
}

int PickRandomAttribute() {
	int bias = Timer();
	int val = random(FIRST_INV_ATTRIBUTE + bias, LAST_INV_ATTRIBUTE + bias) - bias;
	// this is a last resort random here, in case there was an overflow... shouldn't, but might
	// this random really didn't want to pick the edge values for some reason so we use the shifted one above...
	if(val < 0)
		val = random(FIRST_INV_ATTRIBUTE, LAST_INV_ATTRIBUTE);
	return val;
}

Str GetFixedRepresentation(int val, bool isPercentage) {
	val = ConvertFixedToPrecise(val);
	
	if(isPercentage)
		val *= 100;
	
	if(val > 1000) {
		if((val / 10) % 10)
			return StrParam(d:val / 1000, s:".", d:(val / 100) % 10, d:(val / 10) % 10);
		return StrParam(d:val / 1000, s:".", d:(val / 100) % 10);
	}
		
	if(val % 10)
		return StrParam(d:val / 1000, s:".", d:(val / 100) % 10, d:(val / 10) % 10, d:val % 10);
	if((val / 10) % 10)
		return StrParam(d:val / 1000, s:".", d:(val / 100) % 10, d:(val / 10) % 10);
	return StrParam(d:val / 1000, s:".", d:(val / 100) % 10);
}

str GetDetailedModRange(int attr, int tier, int trunc_factor = 0, int extra = -1, bool isPercentage = false) {
	if(extra != -1)
		return GetDetailedModRange_Unique(tier, trunc_factor, extra, isPercentage);
		
	str col_tag = Charm_Strings[tier][CHARMSTR_COLORCODE];
	int tier_mapping = GetModTierRangeMapper(attr, tier);
	if(!trunc_factor) {
		return StrParam(
			s:"\c-(",
			s:col_tag, d:GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW),
			s:"\c--",
			s:col_tag, d:GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH), s:"\c-)"
		);
	}
	return StrParam(
		s:"\c-(",
		s:col_tag, s:GetFixedRepresentation(GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW), isPercentage),
		s:"\c--",
		s:col_tag, s:GetFixedRepresentation(GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH), isPercentage), s:"\c-)"
	);
}

str GetDetailedModRange_Unique(int unique_id, int trunc_factor = 0, int unique_roll_id = 0, bool isPercentage = false) {
	if(!trunc_factor) {
		return StrParam(
			s:"\c-(",
			s:"\c[D1]", d:UniqueItemList[unique_id].rolls[unique_roll_id].attrib_low,
			s:"\c--",
			s:"\c[D1]", d:UniqueItemList[unique_id].rolls[unique_roll_id].attrib_high, s:"\c-)"
		);
	}
	
	if(unique_id != UITEM_WELLOFPOWER) {
		return StrParam(
			s:"\c-(",
			s:"\c[D1]", s:GetFixedRepresentation(UniqueItemList[unique_id].rolls[unique_roll_id].attrib_low, isPercentage),
			s:"\c--",
			s:"\c[D1]", s:GetFixedRepresentation(UniqueItemList[unique_id].rolls[unique_roll_id].attrib_high, isPercentage), s:"\c-)"
		);
	}

	// this item is a little odd, so we need to treat it as such
	// since ACS can't round floats to shit (bad representation, w.e) we need to do a custom one for weird numbers like this
	int low = UniqueItemList[unique_id].rolls[unique_roll_id].attrib_low;
	int hi = UniqueItemList[unique_id].rolls[unique_roll_id].attrib_high;
	
	return StrParam(
		s:"\c-(",
		s:"\c[D1]", d:low / 1000, s:".", d:(low / 100) % 10, d:(low / 10) % 10,
		s:"\c--",
		s:"\c[D1]", d:hi / 1000, s:".", d:(hi / 100) % 10, d:(hi / 10) % 10, s:"\c-)"
	);
}

str GetModTierText(int tier, int extra) {
	if(extra != -1)
		return StrParam(s:"\c[D1]", s:"U");
	return StrParam(s:Charm_Strings[tier][CHARMSTR_COLORCODE], s:"T", d:tier);
}

str GetInventoryAttributeText(int attr) {
	if(attr <= LAST_INV_ATTRIBUTE)
		return StrParam(s:"IATTR_T", d:attr);
		
	// essences are mapped to 1 again for language file
	if(attr <= LAST_ESSENCE_ATTRIBUTE)
		return StrParam(s:"IATTR_TE", d:attr + 1 - FIRST_ESSENCE_ATTRIBUTE);
		
	// only option left is unique exotic attributes
	return StrParam(s:"IATTR_TX", d:UNIQUE_MAP_MACRO(attr));
}

str ItemAttributeString(int attr, int val, int tier = 0, bool showDetailedMods = false, int extra = -1) {
	str text = GetInventoryAttributeText(attr);
	
	switch(attr) {
		case INV_CYBERNETIC:
			return StrParam(l:text);
			
		// essences with specific writing
		case INV_ESS_SSRATH:
			if(showDetailedMods) {
				return StrParam(
					s:"\c[Q7]", l:text, d:val, s:GetDetailedModRange(attr, tier, 0, extra), s:"\c[Q7]% ", l:"IATTR_MAGICRES",
					s:"\c- - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"\c[Q7]", l:text, d:val, s:"% ", l:"IATTR_MAGICRES");
			
		case INV_ESS_CHEGOVAX:
			if(showDetailedMods) {
				return StrParam(
					s:"\c[Q7]", l:text, d:val, s:GetDetailedModRange(attr, tier, 0, extra), s:"\c[Q7]%",
					s:"\c- - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"\c[Q7]", l:text, d:val, s:"%");
			
		case INV_ESS_ZRAVOG:
			if(showDetailedMods) {
				return StrParam(
					s:"\c[Q7]", l:text, d:val, s:GetDetailedModRange(attr, tier, 0, extra), s: "\c[Q7]%",
					s:"\c- - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"\c[Q7]", l:text, d:val, s: "%");

		// essence with no numeric values
		case INV_ESS_VAAJ:
		case INV_ESS_LESHRAC:
		case INV_ESS_THORAX:
			return StrParam(s:"\c[Q7]", l:text);
			
		// essences with percentages in them
		case INV_ESS_OMNISIGHT2:
		case INV_ESS_HARKIMONDE:
		case INV_ESS_KRULL:
			if(showDetailedMods) {
				return StrParam(
					s:"\c[Q7]", s:"+ ", d:val, s:GetDetailedModRange(attr, tier, 0, extra), s:"\c[Q7]%", l:text,
					s:"\c- - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"\c[Q7]", s:"+ ", d:val, s:"%", l:text);
			
		// essences that are like regular mods, just have color code
		case INV_ESS_OMNISIGHT:
			if(showDetailedMods) {
				return StrParam(
					s:"\c[Q7]", s:"+ ", d:val, s:GetDetailedModRange(attr, tier, 0, extra), s:"\c[Q7]", l:text,
					s:"\c- - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"\c[Q7]", s:"+ ", d:val, l:text);
		
		// since percentages are handled in default case, we will handle all flat value attributes under here
		case INV_HP_INCREASE:
		case INV_ARMOR_INCREASE:
		case INV_FLATPHYS_DAMAGE:
		case INV_FLATENERGY_DAMAGE:
		case INV_FLATEXP_DAMAGE:
		case INV_FLATMAGIC_DAMAGE:
		case INV_FLATELEM_DAMAGE:	
		case INV_REGENCAP_INCREASE:
		case INV_KNOCKBACK_RESIST:
		case INV_ACCURACY_INCREASE:
		case INV_STAT_STRENGTH:
		case INV_STAT_DEXTERITY:
		case INV_STAT_BULKINESS:
		case INV_STAT_CHARISMA:
		case INV_STAT_VITALITY:
		case INV_STAT_INTELLECT:
		case INV_FLAT_FIREDMG:
		case INV_FLAT_ICEDMG:
		case INV_FLAT_LIGHTNINGDMG:
		case INV_FLAT_POISONDMG:
		case INV_OVERLOAD_ZAPCOUNT:
		case INV_IGNITE_PROLIFCOUNT:
			if(val > 0) {
				if(showDetailedMods) {
					return StrParam(
						s:"+ \c[Q9]", d:val, s:GetDetailedModRange(attr, tier, 0, extra), s:"\c- ", l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"+ \c[Q9]", d:val, s:"\c- ", l:text);
			}
			else if(val < 0)
				return StrParam(s:"- \cg", d:val, s:"\c- ", l:text);
				
		// fixed point stuff
		case INV_EXPGAIN_INCREASE:
		case INV_CREDITGAIN_INCREASE:
		case INV_DROPCHANCE_INCREASE:
		case INV_LUCK_INCREASE:
		case INV_SPEED_INCREASE:
		case INV_PELLET_INCREASE:
		case INV_CRITCHANCE_INCREASE:
		case INV_CRITPERCENT_INCREASE:
		case INV_DAMAGEPERCENT_MORE:
		case INV_BLOCKERS_MOREDMG:
		case INV_OVERLOAD_DMGINCREASE:
		case INV_LIFESTEAL_DAMAGE:
		case INV_ESS_ERYXIA:
			if(showDetailedMods) {
				return StrParam(s:"+ \c[Q9]", s:GetFixedRepresentation(val, true), s:GetDetailedModRange(attr, tier, FACTOR_FIXED_RESOLUTION, extra, true), s:"%\c- ", l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+ \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:text);
		
		// damage reduction attributes are shown as they are
		case INV_DMGREDUCE_ELEM:
		case INV_DMGREDUCE_PHYS:
		case INV_DMGREDUCE_REFL:
		case INV_DMGREDUCE_ENERGY:
		case INV_DMGREDUCE_EXPLOSION:
		case INV_DMGREDUCE_HITSCAN:
		case INV_DMGREDUCE_MAGIC:
		case INV_DMGREDUCE_FIRE:
		case INV_DMGREDUCE_LIGHTNING:
		case INV_DMGREDUCE_ICE:
		case INV_DMGREDUCE_POISON:
		case INV_ADDEDMAXRESIST:
		case INV_OVERLOAD_DURATION:
		case INV_LIFESTEAL:
			if(showDetailedMods) {
				return StrParam(s:"+ \c[Q9]", s:GetFixedRepresentation(val, false), s:GetDetailedModRange(attr, tier, FACTOR_FIXED_RESOLUTION, extra, false), s:"%\c- ", l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+ \c[Q9]", s:GetFixedRepresentation(val, false), s:"%\c- ", l:text);

		// default takes percentage values
		default:
			if(val > 0) {
				if(showDetailedMods) {
					return StrParam(
						s:"+ \c[Q9]", d:val, s:GetDetailedModRange(attr, tier, 0, extra), s:"%\c- ", l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"+ \c[Q9]", d:val, s:"%\c- ", l:text);
			}
			else if(val < 0) {
				if(showDetailedMods) {
					return StrParam(
						s:"- \c[Q9]", d:val, s:GetDetailedModRange(attr, tier, 0, extra), s:"%\c- ", l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"- \cg", d:val, s:"%\c- ", l:text);
			}
	}
	return "";
}

str GetItemAttributeText(int attr, int val1, int val2 = -1, int tier = 0, bool showDetailedMods = false, int extra = -1) {
	// treat it as normal inv attribute range
	// check last essence as its an all encompassing range except exotics
	if(attr <= LAST_ESSENCE_ATTRIBUTE)
		return ItemAttributeString(attr, val1, tier, showDetailedMods, extra);

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
					s:"\c[Q9]", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra - 1), s:"%\c- ",
					l:text, s:"\c[Q9]", d:val2, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_RECOVERHPHURT",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"\c[Q9]", d:val1, s:"%\c- ", l:GetInventoryAttributeText(INV_EX_CHANCE), l:text, s:"\c[Q9]", d:val2, s:"%\c- ", l:"IATTR_RECOVERHPHURT");
		
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
			if(showDetailedMods) {
				return StrParam(l:text, s:" - ", s:GetModTierText(tier, extra));
			}
			return StrParam(l:text);
		
		case INV_EX_LIMITEDSMALLCHARMS:
			if(showDetailedMods) {
				// singular matters here
				if(val1 > 1)
					return StrParam(
						l:text, s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s: "\c- ", l:"IATTR_TX30_2",
						s:" - ", s:GetModTierText(tier, extra)
					);
				return StrParam(
					l:text, s:"\c[Q9] ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s: "\c- ", l:"IATTR_TX30_2S",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			if(val1 > 1)
				return StrParam(l:text, s:"\c[Q9] ", d:val1, s: "\c- ", l:"IATTR_TX30_2");
			return StrParam(l:text, s:"\c[Q9] ", d:val1, s: "\c- ", l:"IATTR_TX30_2S");
		
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
						s:"- \c[Q9]", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"- \c[Q9]", d:val1, s:"%\c- ", l:text);
			}
			return StrParam(l:text);
			
		// negative effects are shown with different color -- these are % ones of those, these are positive numerically
		case INV_EX_DMGINCREASE_TAKEN:
			if(showDetailedMods) {
				return StrParam(s:"+ \c[D4]", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c[D4] ", l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+ \c[D4]", d:val1, s:"%\c[D4] ", l:text);
			
		// single text negative effects
		case INV_EX_FORBID_ARMOR:
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

#endif
