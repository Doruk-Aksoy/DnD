#ifndef DND_INV_ATTRIBS_IN
#define DND_INV_ATTRIBS_IN

// formula for accuracy:
// every 10 points contribute 0.01%
// applied as follows: X = spread of weapon, X * (1.0 - accuracy * factor_per_point), capped at 80 000 accuracy (80%)

// chance to roll negative attribute: 40% - 10% * tier, perfect charms never roll negative attributes -- 0 based tier

#define DND_ACCURACY_CAP 80000
#define DND_ATTRIBUTEBONUS_CAP 1024

//#define MAX_ATTRIB_MODIFIER 0xFFFFFFFF

#define FACTOR_SMALLCHARM_RESOLUTION 1000

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
	INV_EXPLOSIVE_RESIST,
	
	INV_AMMOGAIN_CHANCE,
	INV_AMMOGAIN_INCREASE,
	INV_SHOPSTOCK_INCREASE,
	
	INV_REGENCAP_INCREASE,
	
	INV_CRITCHANCE_INCREASE,
	INV_CRITPERCENT_INCREASE,
	INV_CRITDAMAGE_INCREASE,
	
	INV_KNOCKBACK_RESIST,
	INV_DAMAGEPERCENT_INCREASE,
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
	
	// add new regular rollable attributes here (will require db reset otherwise desync)
	
	// essence attributes (only via. specific means)
	INV_ESS_VAAJ,
	INV_ESS_SSRATH,
	INV_ESS_OMNISIGHT,
	INV_ESS_OMNISIGHT2,
	INV_ESS_CHEGOVAX,
	INV_ESS_HARKIMONDE,
	INV_ESS_LESHRAC,
	INV_ESS_LESHRAC2,	// this is a two part bonus
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
	INV_EX_DAMAGEPER_FLATHEALTH, // only 1%
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
	// add new unique attributes here
};

// attributes below last_inv (normal rollables) are exotic
#define FIRST_INV_ATTRIBUTE INV_HP_INCREASE
#define LAST_INV_ATTRIBUTE INV_INCREASEDDOT
#define NORMAL_ATTRIBUTE_COUNT (LAST_INV_ATTRIBUTE - FIRST_INV_ATTRIBUTE + 1)
// modify the above to make it use the negative last
//#define NEGATIVE_ATTRIB_BEGIN INV_NEG_DAMAGE_DEALT
#define UNIQUE_ATTRIB_BEGIN INV_EX_CHANCE
#define UNIQUE_ATTRIB_END INV_EX_CURSEIMMUNITY
#define UNIQUE_ATTRIB_COUNT (UNIQUE_ATTRIB_END - UNIQUE_ATTRIB_BEGIN + 1)

#define FIRST_ESSENCE_ATTRIBUTE INV_ESS_VAAJ
#define LAST_ESSENCE_ATTRIBUTE INV_ESS_ERYXIA
#define ESSENCE_ATTRIBUTE_COUNT (LAST_ESSENCE_ATTRIBUTE - FIRST_ESSENCE_ATTRIBUTE + 1)

#define MAX_INV_ATTRIBUTE_TYPES (NORMAL_ATTRIBUTE_COUNT + ESSENCE_ATTRIBUTE_COUNT)
#define MAX_TOTAL_ATTRIBUTES (UNIQUE_ATTRIB_COUNT + NORMAL_ATTRIBUTE_COUNT + ESSENCE_ATTRIBUTE_COUNT)
#define UNIQUE_ATTRIB_MAPPER (LAST_ESSENCE_ATTRIBUTE - UNIQUE_ATTRIB_ID_BEGIN + 1) // maps the array indices proper

#define UNIQUE_MAP_MACRO(X) ((X) + UNIQUE_ATTRIB_MAPPER)

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
	INV_ATTR_TAG_STAT = 512
};

global str 62: Inv_Attribute_Checkers[MAX_TOTAL_ATTRIBUTES];

void SetupInventoryAttributeStrings() {
	Inv_Attribute_Checkers[INV_HP_INCREASE] = "IATTR_FlatHP";
	Inv_Attribute_Checkers[INV_ARMOR_INCREASE] = "IATTR_FlatArmor";
	Inv_Attribute_Checkers[INV_HPPERCENT_INCREASE] = "IATTR_HPPercent";
	Inv_Attribute_Checkers[INV_ARMORPERCENT_INCREASE] = "IATTR_ArmorPercent";
	Inv_Attribute_Checkers[INV_EXPGAIN_INCREASE] = "IATTR_ExpBonus";
	Inv_Attribute_Checkers[INV_CREDITGAIN_INCREASE] = "IATTR_CreditBonus";
	Inv_Attribute_Checkers[INV_DROPCHANCE_INCREASE] = "IATTR_DropChanceBonus";
	Inv_Attribute_Checkers[INV_LUCK_INCREASE] = "IATTR_LuckBonus";
	Inv_Attribute_Checkers[INV_AMMOCAP_INCREASE] = "IATTR_AmmoCapBonus";
	Inv_Attribute_Checkers[INV_SPEED_INCREASE] = "IATTR_SpeedBonus";
	Inv_Attribute_Checkers[INV_MAGAZINE_INCREASE] = "IATTR_MagazineIncrease";
	
	Inv_Attribute_Checkers[INV_FLATPHYS_DAMAGE] = "IATTR_FlatDamageBonus_Physical";
	Inv_Attribute_Checkers[INV_FLATENERGY_DAMAGE] = "IATTR_FlatDamageBonus_Energy";
	Inv_Attribute_Checkers[INV_FLATEXP_DAMAGE] = "IATTR_FlatDamageBonus_Explosive";
	Inv_Attribute_Checkers[INV_FLATMAGIC_DAMAGE] = "IATTR_FlatDamageBonus_Magic";
	Inv_Attribute_Checkers[INV_FLATELEM_DAMAGE] = "IATTR_FlatDamageBonus_Elemental";
	
	Inv_Attribute_Checkers[INV_PERCENTPHYS_DAMAGE] = "IATTR_PercentDamageBonus_Physical";
	Inv_Attribute_Checkers[INV_PERCENTENERGY_DAMAGE] = "IATTR_PercentDamageBonus_Energy";
	Inv_Attribute_Checkers[INV_PERCENTEXP_DAMAGE] = "IATTR_PercentDamageBonus_Explosive";
	Inv_Attribute_Checkers[INV_PERCENTMAGIC_DAMAGE] = "IATTR_PercentDamageBonus_Magic";
	Inv_Attribute_Checkers[INV_PERCENTELEM_DAMAGE] = "IATTR_PercentDamageBonus_Elemental";
	
	Inv_Attribute_Checkers[INV_SLOT1_DAMAGE] = "IATTR_SlotDamageBonus_1";
	Inv_Attribute_Checkers[INV_SLOT2_DAMAGE] = "IATTR_SlotDamageBonus_2";
	Inv_Attribute_Checkers[INV_SLOT3_DAMAGE] = "IATTR_SlotDamageBonus_3";
	Inv_Attribute_Checkers[INV_SLOT4_DAMAGE] = "IATTR_SlotDamageBonus_4";
	Inv_Attribute_Checkers[INV_SLOT5_DAMAGE] = "IATTR_SlotDamageBonus_5";
	Inv_Attribute_Checkers[INV_SLOT6_DAMAGE] = "IATTR_SlotDamageBonus_6";
	Inv_Attribute_Checkers[INV_SLOT7_DAMAGE] = "IATTR_SlotDamageBonus_7";
	Inv_Attribute_Checkers[INV_SLOT8_DAMAGE] = "IATTR_SlotDamageBonus_8";
	Inv_Attribute_Checkers[INV_TEMPWEP_DAMAGE] = "IATTR_SlotDamageBonus_9";
	
	Inv_Attribute_Checkers[INV_PELLET_INCREASE] = "IATTR_PelletIncrease";
	
	Inv_Attribute_Checkers[INV_EXPLOSION_RADIUS] = "IATTR_ExplosionRadiusIncrease";
	Inv_Attribute_Checkers[INV_EXPLOSIVE_RESIST] = "IATTR_ExplosionResist";

	Inv_Attribute_Checkers[INV_AMMOGAIN_CHANCE] = "IATTR_AmmoGainChance";
	Inv_Attribute_Checkers[INV_AMMOGAIN_INCREASE] = "IATTR_AmmoPickupIncrease";
	Inv_Attribute_Checkers[INV_SHOPSTOCK_INCREASE] = "IATTR_ShopStockIncrease";
	
	Inv_Attribute_Checkers[INV_REGENCAP_INCREASE] = "IATTR_RegenCap";
	
	Inv_Attribute_Checkers[INV_CRITCHANCE_INCREASE] = "IATTR_CritChance";
	Inv_Attribute_Checkers[INV_CRITPERCENT_INCREASE] = "IATTR_CritChancePercent";
	Inv_Attribute_Checkers[INV_CRITDAMAGE_INCREASE] = "IATTR_CritDamage";
	
	Inv_Attribute_Checkers[INV_KNOCKBACK_RESIST] = "IATTR_KnockbackResist";
	Inv_Attribute_Checkers[INV_DAMAGEPERCENT_INCREASE] = "IATTR_DamagePercent";
	Inv_Attribute_Checkers[INV_ACCURACY_INCREASE] = "IATTR_Accuracy";
	
	Inv_Attribute_Checkers[INV_STAT_STRENGTH] = "IATTR_StatBonus_STR";
	Inv_Attribute_Checkers[INV_STAT_DEXTERITY] = "IATTR_StatBonus_DEX";
	Inv_Attribute_Checkers[INV_STAT_BULKINESS] = "IATTR_StatBonus_BUL";
	Inv_Attribute_Checkers[INV_STAT_CHARISMA] = "IATTR_StatBonus_CHR";
	Inv_Attribute_Checkers[INV_STAT_VITALITY] = "IATTR_StatBonus_VIT";
	Inv_Attribute_Checkers[INV_STAT_INTELLECT] = "IATTR_StatBonus_INT";
	
	Inv_Attribute_Checkers[INV_DMGREDUCE_ELEM] = "IATTR_ElementalResist";
	Inv_Attribute_Checkers[INV_DMGREDUCE_PHYS] = "IATTR_PhysicalResist";
	Inv_Attribute_Checkers[INV_DMGREDUCE_REFL] = "IATTR_ReflectResist";
	
	Inv_Attribute_Checkers[INV_PEN_PHYSICAL] = "IATTR_PhysPen";
	Inv_Attribute_Checkers[INV_PEN_ENERGY] = "IATTR_EnergyPen";
	Inv_Attribute_Checkers[INV_PEN_EXPLOSIVE] = "IATTR_ExplosivePen";
	Inv_Attribute_Checkers[INV_PEN_OCCULT] = "IATTR_OccultPen";
	Inv_Attribute_Checkers[INV_PEN_ELEMENTAL] = "IATTR_ElementalPen";
	
	Inv_Attribute_Checkers[INV_FLAT_FIREDMG] = "IATTR_FlatFireDmg";
	Inv_Attribute_Checkers[INV_FLAT_ICEDMG] = "IATTR_FlatIceDmg";
	Inv_Attribute_Checkers[INV_FLAT_LIGHTNINGDMG] = "IATTR_FlatLightningDmg";
	Inv_Attribute_Checkers[INV_FLAT_POISONDMG] = "IATTR_FlatPoisonDmg";
	
	Inv_Attribute_Checkers[INV_LIFESTEAL] = "IATTR_Lifesteal";
	
	Inv_Attribute_Checkers[INV_POISON_TICRATE] = "IATTR_PoisonTicrate";
	Inv_Attribute_Checkers[INV_POISON_DURATION] = "IATTR_PoisonDuration";
	Inv_Attribute_Checkers[INV_POISON_TICDMG] = "IATTR_PoisonTicDmg";
	
	Inv_Attribute_Checkers[INV_BLOCKERS_MOREDMG] = "IATTR_BlockDmg";
	
	Inv_Attribute_Checkers[INV_FREEZECHANCE] = "IATTR_FreezeChance";
	Inv_Attribute_Checkers[INV_SLOWEFFECT] = "IATTR_SlowEffect";
	Inv_Attribute_Checkers[INV_CHILLTHRESHOLD] = "IATTR_ChillThreshold";
	
	Inv_Attribute_Checkers[INV_IGNITECHANCE] = "IATTR_IgniteChance";
	Inv_Attribute_Checkers[INV_IGNITEDMG] = "IATTR_IgniteDmg";
	Inv_Attribute_Checkers[INV_IGNITEDURATION] = "IATTR_IgniteDuration";
	
	Inv_Attribute_Checkers[INV_OVERLOADCHANCE] = "IATTR_OverloadChance";
	Inv_Attribute_Checkers[INV_OVERLOAD_ZAPCOUNT] = "IATTR_OverloadZapCount";
	Inv_Attribute_Checkers[INV_OVERLOAD_DMGINCREASE] = "IATTR_OverloadZapDmg";
	
	Inv_Attribute_Checkers[INV_CYBERNETIC] = "";
	Inv_Attribute_Checkers[INV_MELEERANGE] = "IATTR_MeleeRange";
	Inv_Attribute_Checkers[INV_MELEEDAMAGE] = "IATTR_MeleeDamage";
	Inv_Attribute_Checkers[INV_DOTMULTI] = "IATTR_DamageOverTimeMult";
	Inv_Attribute_Checkers[INV_INCREASEDDOT] = "IATTR_DamageOverTimeIncrease";
	
	// essences
	Inv_Attribute_Checkers[INV_ESS_VAAJ] = "IATTR_StatusBuffs_1";
	Inv_Attribute_Checkers[INV_ESS_SSRATH] = "IATTR_SoulPenetration";
	Inv_Attribute_Checkers[INV_ESS_OMNISIGHT] = "IATTR_Accuracy";
	Inv_Attribute_Checkers[INV_ESS_OMNISIGHT2] = "IATTR_AccuracyPercent";
	Inv_Attribute_Checkers[INV_ESS_CHEGOVAX] = "IATTR_IgniteDamageEachTic";
	Inv_Attribute_Checkers[INV_ESS_HARKIMONDE] = "IATTR_ChanceIgnoreShield";
	Inv_Attribute_Checkers[INV_ESS_LESHRAC] = "IATTR_ReducedPoisonTaken";
	Inv_Attribute_Checkers[INV_ESS_LESHRAC2] = "IATTR_StatusBuffs_1";
	Inv_Attribute_Checkers[INV_ESS_KRULL] = "IATTR_ExplosionAgainChance";
	Inv_Attribute_Checkers[INV_ESS_THORAX] = "IATTR_StatusBuffs_1";
	Inv_Attribute_Checkers[INV_ESS_ZRAVOG] = "IATTR_OccultReducePer";
	Inv_Attribute_Checkers[INV_ESS_ERYXIA] = "IATTR_FrozenDamage";
	
	// exotic stuff is handled differently here
	int idmap = UNIQUE_MAP_MACRO(UNIQUE_ATTRIB_BEGIN);
	
	Inv_Attribute_Checkers[idmap++] = "";
	Inv_Attribute_Checkers[idmap++] = "IATTR_ChanceToCastElementalSpell";
	Inv_Attribute_Checkers[idmap++] = "IATTR_StatusBuffs_1";
	Inv_Attribute_Checkers[idmap++] = "IATTR_StatusBuffs_1";
	Inv_Attribute_Checkers[idmap++] = "";
	Inv_Attribute_Checkers[idmap++] = "IATTR_HealMissingHealthOnPain";
	Inv_Attribute_Checkers[idmap++] = "IATTR_DamageIncrease_Lightning";
	Inv_Attribute_Checkers[idmap++] = "IATTR_IncreasedCritLightning";
	Inv_Attribute_Checkers[idmap++] = "IATTR_DamageIncrease_Shotguns";
	Inv_Attribute_Checkers[idmap++] = "IATTR_HPPercent";
	Inv_Attribute_Checkers[idmap++] = "IATTR_DamagePerFlatHP";
	Inv_Attribute_Checkers[idmap++] = "IATTR_StatusBuffs_1";
	Inv_Attribute_Checkers[idmap++] = "IATTR_StatusBuffs_1";
	Inv_Attribute_Checkers[idmap++] = "";
	Inv_Attribute_Checkers[idmap++] = "IATTR_ChanceToRaiseZombieFromKills";
	Inv_Attribute_Checkers[idmap++] = "IATTR_StatusBuffs_1";
	Inv_Attribute_Checkers[idmap++] = "IATTR_IncreasedDamageTaken";
	Inv_Attribute_Checkers[idmap++] = "";
	Inv_Attribute_Checkers[idmap++] = "IATTR_HealOnKill";
	Inv_Attribute_Checkers[idmap++] = "IATTR_StatusBuffs_1";
	Inv_Attribute_Checkers[idmap++] = "IATTR_SoulAmmoIncrease";
	Inv_Attribute_Checkers[idmap++] = "IATTR_CastRally";
	Inv_Attribute_Checkers[idmap++] = "IATTR_StatusBuffs_1";
	Inv_Attribute_Checkers[idmap++] = "IATTR_StatusBuffs_1";
	Inv_Attribute_Checkers[idmap++] = "IATTR_FlatDotDamage";
	Inv_Attribute_Checkers[idmap++] = "IATTR_DotDuration";
	Inv_Attribute_Checkers[idmap++] = "IATTR_StatusBuffs_1";
	Inv_Attribute_Checkers[idmap++] = "IATTR_CritIgnoreRes";
	Inv_Attribute_Checkers[idmap++] = "IATTR_StatusBuffs_1";
}

// These are necessary to sync the global variables
Script "DnD Load Inventory Attributes" OPEN {
	SetupInventoryAttributeStrings();
}

Script "DnD Load Inventory Attributes - CS" OPEN CLIENTSIDE {
	SetupInventoryAttributeStrings();
}

// if 3rd argument is 0 that means simply use the difference + 1 as increment
Inv_attrib_T Inv_Attribute_Info[MAX_INV_ATTRIBUTE_TYPES] = {
	{ 	5, 		19, 		0,		INV_ATTR_TAG_LIFE 										},
	{ 	5, 		19, 		0,		INV_ATTR_TAG_DEFENSE 									},
	{ 	1, 		6, 			0,		INV_ATTR_TAG_LIFE										},
	{ 	1, 		6, 			0,		INV_ATTR_TAG_DEFENSE									},
	{ 	5, 		9, 			7,		INV_ATTR_TAG_UTILITY 									},
	{ 	5, 		9, 			7, 		INV_ATTR_TAG_UTILITY									},
	{ 	0.005, 	0.015, 		0, 		INV_ATTR_TAG_UTILITY									},
	{ 	0.025, 	0.05, 		0, 		INV_ATTR_TAG_UTILITY									},
	{ 	5, 		14, 		0, 		INV_ATTR_TAG_UTILITY									},
	{ 	0.01, 	0.03, 		0.04,	INV_ATTR_TAG_UTILITY									},
	{ 	1, 		9, 			0,		INV_ATTR_TAG_UTILITY									},
	
	{ 	1, 		4, 			0,		INV_ATTR_TAG_ATTACK 									},
	{ 	1, 		4, 			0, 		INV_ATTR_TAG_ATTACK										},
	{ 	1, 		4, 			0, 		INV_ATTR_TAG_ATTACK	| INV_ATTR_TAG_EXPLOSIVE			},
	{ 	1, 		4, 			0, 		INV_ATTR_TAG_ATTACK	| INV_ATTR_TAG_OCCULT				},
	{ 	1, 		4, 			0,		INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL			},
	
	{ 	5, 		10, 		0,		INV_ATTR_TAG_DAMAGE										},
	{ 	5, 		10, 		0,		INV_ATTR_TAG_DAMAGE										},
	{ 	5, 		10, 		0,		INV_ATTR_TAG_DAMAGE	| INV_ATTR_TAG_EXPLOSIVE			},
	{ 	5, 		10, 		0,		INV_ATTR_TAG_DAMAGE	| INV_ATTR_TAG_OCCULT				},
	{ 	5, 		10, 		0,		INV_ATTR_TAG_DAMAGE | INV_ATTR_TAG_ELEMENTAL			},
	
	{ 	5, 		15, 		0,		INV_ATTR_TAG_DAMAGE 									},
	{ 	5, 		15, 		0,		INV_ATTR_TAG_DAMAGE										},
	{ 	5, 		15, 		0,		INV_ATTR_TAG_DAMAGE										},
	{ 	5, 		15, 		0,		INV_ATTR_TAG_DAMAGE										},
	{ 	5, 		15, 		0,		INV_ATTR_TAG_DAMAGE										},
	{ 	5, 		15, 		0,		INV_ATTR_TAG_DAMAGE										},
	{ 	5, 		15, 		0,		INV_ATTR_TAG_DAMAGE										},
	{ 	5, 		15, 		0,		INV_ATTR_TAG_DAMAGE	| INV_ATTR_TAG_OCCULT				},
	{ 	5, 		15, 		0,		INV_ATTR_TAG_DAMAGE										},
	
	{ 	5, 		10, 		10,		INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_ATTACK				},
	
	{ 	1, 		10, 		0,		INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_EXPLOSIVE			},
	{ 	1, 		5, 			0,		INV_ATTR_TAG_EXPLOSIVE | INV_ATTR_TAG_ATTACK 			},
	
	{ 	4, 		8, 			0,		INV_ATTR_TAG_UTILITY									},
	{ 	5, 		10, 		0,		INV_ATTR_TAG_UTILITY									},
	{ 	1, 		10, 		0,		INV_ATTR_TAG_UTILITY									},
	
	{ 	10, 	24, 		0,		INV_ATTR_TAG_UTILITY | INV_ATTR_TAG_LIFE				},
	
	{ 	1, 		5, 			0,		INV_ATTR_TAG_CRIT										},
	{ 	1, 		10, 		0,		INV_ATTR_TAG_CRIT										},
	{ 	5, 		14, 		0,		INV_ATTR_TAG_CRIT										},
	
	{ 	50, 	100, 		100,	INV_ATTR_TAG_UTILITY									},
	{ 	1, 		5, 			0,		INV_ATTR_TAG_DAMAGE										},
	{ 	25, 	124, 		0,		INV_ATTR_TAG_ATTACK										},
	
	{ 	2, 		6, 			0,		INV_ATTR_TAG_STAT										},
	{ 	2, 		6, 			0,		INV_ATTR_TAG_STAT										},
	{ 	2, 		6, 			0,		INV_ATTR_TAG_STAT										},
	{ 	2, 		6, 			0,		INV_ATTR_TAG_STAT										},
	{ 	2, 		6, 			0,		INV_ATTR_TAG_STAT										},
	{ 	2, 		6, 			0,		INV_ATTR_TAG_STAT										},
	
	{ 	1, 		2, 			0,		INV_ATTR_TAG_DEFENSE | INV_ATTR_TAG_ELEMENTAL			},
	{ 	1, 		2, 			0,		INV_ATTR_TAG_DEFENSE									},
	{ 	10, 	90, 		0,		INV_ATTR_TAG_DEFENSE									},
	
	{ 	1, 		5, 			0,		INV_ATTR_TAG_ATTACK										},
	{ 	1, 		5, 			0,		INV_ATTR_TAG_ATTACK										},
	{ 	1, 		5, 			0,		INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_EXPLOSIVE			},
	{ 	1, 		5, 			0,		INV_ATTR_TAG_ATTACK	| INV_ATTR_TAG_OCCULT				},
	{ 	1, 		5, 			0,		INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL			},
	
	{ 	2, 		6, 			0,		INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL			},
	{ 	2, 		6, 			0,		INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL			},
	{ 	2, 		6, 			0,		INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL			},
	{ 	2, 		6, 			0,		INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_ELEMENTAL			},
	
	// lifesteal
	{ 	0.05, 	0.125, 		0,		INV_ATTR_TAG_ATTACK										},
	
	{ 	5, 		10, 		0,		INV_ATTR_TAG_ELEMENTAL									},
	{ 	5, 		14, 		0,		INV_ATTR_TAG_ELEMENTAL									},
	{ 	5, 		14, 		0,		INV_ATTR_TAG_ELEMENTAL									},
	
	{ 	5, 		14, 		0,		INV_ATTR_TAG_DAMAGE										},

	{ 	2, 		5, 			0,		INV_ATTR_TAG_ELEMENTAL									},
	{ 	2, 		6, 			0,		INV_ATTR_TAG_ELEMENTAL									},
	{ 	1, 		5, 			0,		INV_ATTR_TAG_ELEMENTAL									},
	
	{ 	1, 		5, 			0,		INV_ATTR_TAG_ELEMENTAL									},
	{ 	5, 		14, 		0,		INV_ATTR_TAG_ELEMENTAL									},
	{ 	4, 		12, 		0,		INV_ATTR_TAG_ELEMENTAL									},
	
	{ 	1, 		5, 			0,		INV_ATTR_TAG_ELEMENTAL									},
	{ 	1, 		1, 			1,		INV_ATTR_TAG_ELEMENTAL									},
	{ 	5, 		9, 			0,		INV_ATTR_TAG_ELEMENTAL									},
	
	// cybernetic
	{ 	1, 		1, 			1,		INV_ATTR_TAG_UTILITY									},
	{ 	2, 		6, 			0,		INV_ATTR_TAG_ATTACK | INV_ATTR_TAG_UTILITY				},
	{ 	5, 		15, 		0,		INV_ATTR_TAG_DAMAGE										},
	{ 	1, 		8, 			0,		INV_ATTR_TAG_DAMAGE										},
	{ 	5, 		19, 		0,		INV_ATTR_TAG_DAMAGE										},
	
	// essences
	{ 	1, 		1, 			1, 		INV_ATTR_TAG_NONE 										}, // vaaj
	{ 	3,		5,			0,		INV_ATTR_TAG_NONE										}, // ssrath
	{ 	1001,	1500,		0,		INV_ATTR_TAG_NONE										}, // omnisight 1
	{ 	2,		5,			0,		INV_ATTR_TAG_NONE										}, // omnisight 2
	{ 	1,		4,			0,		INV_ATTR_TAG_NONE										}, // chegovax
	{ 	3,		6,			0,		INV_ATTR_TAG_NONE										}, // harkimonde
	{ 	5,		9,			0,		INV_ATTR_TAG_NONE										}, // leshrac 1
	{ 	1,		1,			1,		INV_ATTR_TAG_NONE										}, // leshrac 2
	{ 	5,		9,			0,		INV_ATTR_TAG_NONE										}, // krull
	{ 	1,		1,			1,		INV_ATTR_TAG_NONE										}, // thorax
	{ 	1,		2,			0,		INV_ATTR_TAG_NONE										}, // zravog
	{ 	10,		20,			0,		INV_ATTR_TAG_NONE										}  // eryxia
};

str GetInventoryAttributeText(int attr) {
	if(attr <= LAST_INV_ATTRIBUTE)
		return StrParam(s:"IATTR_T", d:attr);
		
	// essences are mapped to 1 again for language file
	if(attr <= LAST_ESSENCE_ATTRIBUTE)
		return StrParam(s:"IATTR_TE", d:attr + 1 - FIRST_ESSENCE_ATTRIBUTE);
		
	// only option left is unique exotic attributes
	return StrParam(s:"IATTR_TX", d:UNIQUE_MAP_MACRO(attr) - LAST_ESSENCE_ATTRIBUTE);
}

str ItemAttributeString(int attr, int val) {
	str text = GetInventoryAttributeText(attr);
	
	switch(attr) {
		case INV_CYBERNETIC:
			return StrParam(l:text);
			
		// essences with specific writing
		case INV_ESS_SSRATH:
			return StrParam(s:"\c[Q7]", l:text, s:"+ ", d:val, s:"% ", l:"IATTR_MAGICRES");
		case INV_ESS_CHEGOVAX:
			return StrParam(s:"\c[Q7]", l:text, s:"+ ", d:val, s:"%");
		case INV_ESS_ZRAVOG:
			return StrParam(s:"\c[Q7]", l:text, d:val, s: "%");
		case INV_ESS_ERYXIA:
			return StrParam(s:"\c[Q7]", l:text, d:val, s:"% ", l:"IATTR_MOREDMG");
		// essence with no numeric values
		case INV_ESS_VAAJ:
		case INV_ESS_LESHRAC2:
		case INV_ESS_THORAX:
			return StrParam(s:"\c[Q7]", l:text);
		// essences with percentages in them
		case INV_ESS_OMNISIGHT2:
		case INV_ESS_LESHRAC:
		case INV_ESS_HARKIMONDE:
		case INV_ESS_KRULL:
			return StrParam(s:"\c[Q7]", s:"+ ", d:val, s:"%", l:text);
		// essences that are like regular mods, just have color code
		case INV_ESS_OMNISIGHT:
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
			if(val > 0)
				return StrParam(s:"+ \c[Q9]", d:val, s:"\c- ", l:text);
			else if(val < 0)
				return StrParam(s:"- \cg", d:val, s:"\c- ", l:text);
				
		case INV_DROPCHANCE_INCREASE:
		case INV_LUCK_INCREASE:
		case INV_SPEED_INCREASE:
			return StrParam(s:"+ \c[Q9]", f:ftrunc(val * 100), s:"%\c- ", l:text);
		
		case INV_DMGREDUCE_REFL:
			return StrParam(s:"+ \c[Q9]", f:ftrunc(val * 0.1), s:"%\c- ", l:text);
			
		case INV_LIFESTEAL:
			return StrParam(s:"+ \c[Q9]", f:ftrunc(val), s:"%\c- ", l:text);
		
		// default takes percentage values
		default:
			if(val > 0)
				return StrParam(s:"+ \c[Q9]", d:val, s:"%\c- ", l:text);
			else if(val < 0)
				return StrParam(s:"- \cg", d:val, s:"%\- ", l:text);
	}
	return "";
}

str GetItemAttributeText(int attr, int val1, int val2 = -1) {
	// treat it as normal inv attribute range
	// check last essence as its an all encompassing range except exotics
	if(attr <= LAST_ESSENCE_ATTRIBUTE)
		return ItemAttributeString(attr, val1);
		
	// we must map all exotic ones with the proper macro
	str text = GetInventoryAttributeText(attr);
	switch(attr) {
		case INV_EX_FACTOR_SMALLCHARM:
		return StrParam(l:text, s:"\c[Q9]", f:ftrunc2((val1 << 16) / FACTOR_SMALLCHARM_RESOLUTION), s:"%");
		
		case INV_EX_CHANCE_HEALMISSINGONPAIN:
		return StrParam(s:"\c[Q9]", d:val1, s:"%\c- ", l:GetInventoryAttributeText(INV_EX_CHANCE), l:text, s:"\c[Q9]", d:val2, s:"%\c- ", l:"IATTR_RECOVERHPHURT");
		
		case INV_EX_DAMAGEPER_FLATHEALTH:
		return StrParam(l:text, s:"\c[Q9]", d:val1, s:"\c- ", l:"IATTR_MAXHEALTH");
		
		case INV_EX_ONKILL_HEALMISSING:
		return StrParam(l:text, s:"\c[Q9]", d:val1, s:"%\c- ", l:"IATTR_PMISSHP");
		
		case INV_EX_ABILITY_RALLY:
		return StrParam(l:text, s:"\c[Q9]", d:val1, s:"\c- ", l:"IATTR_RALLY");
		
		case INV_EX_CRITIGNORERESCHANCE:
		return StrParam(l:text, s:"\c[Q9]", d:val1, s:"%\c- ", l:"IATTR_CHANCEIGNORERES");
		
		// float factor stuff
		case INV_EX_MORECRIT_LIGHTNING:
		return StrParam(s:"+ ", s:"\c[Q9]", f:ftrunc(val1 * 100), s:"%\c- ", l:text);
		
		case INV_EX_DOUBLE_HEALTHCAP:
		return StrParam(l:text);
		
		case INV_EX_ALLSTATS:
		case INV_EX_FLATDMG_ALL:
		case INV_EX_FLATDOT:
			return StrParam(s:"+ \c[Q9]", d:val1, s:"\c- ", l:text);
		
		// default are %
		default:
			if(val1)
				return StrParam(s:"+ \c[Q9]", d:val1, s:"%\c- ", l:text);
			return StrParam(l:text);
	}
	return "";
}

#endif
