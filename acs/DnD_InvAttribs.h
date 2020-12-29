#ifndef DND_INV_ATTRIBS_IN
#define DND_INV_ATTRIBS_IN

// formula for accuracy:
// every 10 points contribute 0.01%
// applied as follows: X = spread of weapon, X * (1.0 - accuracy * factor_per_point), capped at 80 000 accuracy (80%)

// chance to roll negative attribute: 40% - 10% * tier, perfect charms never roll negative attributes -- 0 based tier

#define DND_ACCURACY_CAP 80000
#define DND_ATTRIBUTEBONUS_CAP 1024

#define MAX_ATTRIB_MODIFIER 0xFF

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
	
	INV_IGNITECHANCE,
	INV_IGNITEDMG,
	
	INV_OVERLOADCHANCE,
	INV_OVERLOAD_ZAPCOUNT,
	INV_OVERLOAD_DMGINCREASE,
	
	// add new attributes here, below the last normal item attributes to avoid weird problems regarding database saves
	/*
	INV_NEG_DAMAGE_DEALT,
	INV_NEG_DAMAGE_TAKEN, // remove the inv_ex_damage taken and replace it with this
	INV_NEG_HEALTHCAP,
	INV_NEG_ARMORCAP,
	INV_NEG_HEALTHCAP_PERCENT,
	INV_NEG_ARMORCAP_PERCENT,
	INV_NEG_SPEED,
	INV_NEG_CRIT_DAMAGE,
	INV_NEG_CRIT_PERCENT,
	INV_NEG_KNOCKBACK_RESIST,
	INV_NEG_REGEN_CAP,
	
	INV_NEG_FLATPHYS_DAMAGE,
	INV_NEG_FLATENERGY_DAMAGE,
	INV_NEG_FLATEXP_DAMAGE,
	INV_NEG_FLATMAGIC_DAMAGE,
	INV_NEG_FLATELEM_DAMAGE,
	
	INV_NEG_PERCENTPHYS_DAMAGE,
	INV_NEG_PERCENTENERGY_DAMAGE,
	INV_NEG_PERCENTEXP_DAMAGE,
	INV_NEG_PERCENTMAGIC_DAMAGE,
	INV_NEG_PERCENTELEM_DAMAGE,
	
	INV_NEG_SLOT1_DAMAGE,
	INV_NEG_SLOT2_DAMAGE,
	INV_NEG_SLOT3_DAMAGE,
	INV_NEG_SLOT4_DAMAGE,
	INV_NEG_SLOT5_DAMAGE,
	INV_NEG_SLOT6_DAMAGE,
	INV_NEG_SLOT7_DAMAGE,
	INV_NEG_SLOT8_DAMAGE,
	INV_NEG_TEMPWEP_DAMAGE,
	
	INV_NEG_DMGTAKEN_ELEM,
	INV_NEG_DMGTAKEN_PHYS,
	INV_NEG_DMGTAKEN_REFL,
	
	INV_NEG_ACCURACY,
	
	INV_NEG_EXPGAIN,
	INV_NEG_CREDITGAIN,
	
	INV_NEG_AMMOGAIN,
	INV_NEG_AMMOCAP,
	*/
	
	// below here are exotic attributes not found in normal items, if you add new attributes do so to above and change MAX_INV_ATTRIBUTE_TYPES
	INV_EX_CHANCE = UNIQUE_ATTRIB_ID_BEGIN, // this is the generic "chance to do X" thing, the starter attribute, any effect that use this will come immediately after it
	INV_EX_CHANCE_CASTELEMSPELLONATK,
	INV_EX_KNOCKBACK_IMMUNITY,
	INV_EX_FACTOR_SMALLCHARM,
	INV_EX_ALLSTATS,
	INV_EX_CHANCE_HEALMISSINGONPAIN,
	INV_EX_DMGINCREASE_LIGHTNING,
	INV_EX_ALWAYSCRIT_LIGHTNING,
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
	INV_EX_ABILITY_MONSTERSRIP
	// add new unique attributes here
};

// attributes below last_inv (normal rollables) are exotic
#define FIRST_INV_ATTRIBUTE INV_HP_INCREASE
#define LAST_INV_ATTRIBUTE INV_OVERLOAD_DMGINCREASE 
// modify the above to make it use the negative last
//#define NEGATIVE_ATTRIB_BEGIN INV_NEG_DAMAGE_DEALT
#define UNIQUE_ATTRIB_BEGIN INV_EX_CHANCE
#define UNIQUE_ATTRIB_END INV_EX_ABILITY_MONSTERSRIP
#define NORMAL_ATTRIBUTE_COUNT (LAST_INV_ATTRIBUTE - FIRST_INV_ATTRIBUTE + 1)
#define UNIQUE_ATTRIB_COUNT (UNIQUE_ATTRIB_END - UNIQUE_ATTRIB_BEGIN + 1)
#define MAX_INV_ATTRIBUTE_TYPES (LAST_INV_ATTRIBUTE + 1)
#define MAX_TOTAL_ATTRIBUTES (UNIQUE_ATTRIB_COUNT + NORMAL_ATTRIBUTE_COUNT)
#define UNIQUE_ATTRIB_MAPPER (LAST_INV_ATTRIBUTE - UNIQUE_ATTRIB_ID_BEGIN + 1) // maps the array indices proper

#define UNIQUE_MAP_MACRO(X) ((X) +  UNIQUE_ATTRIB_MAPPER)

typedef struct {
	int attrib_low;
	int attrib_high;
	int attrib_level_modifier;
} inv_attrib_T;

#define INVATTR_CHECKER 0
#define INVATTR_TEXT 1
str Inv_Attribute_Names[MAX_TOTAL_ATTRIBUTES][2] = {
	{ "IATTR_FlatHP", " Health" },
	{ "IATTR_FlatArmor", " Armor" },
	{ "IATTR_HPPercent", "% Health" },
	{ "IATTR_ArmorPercent", "% Armor" },
	{ "IATTR_ExpBonus", "% more exp gain" },
	{ "IATTR_CreditBonus", "% more credit gain" },
	{ "IATTR_DropChanceBonus", "% more loot" },
	{ "IATTR_LuckBonus", "% increased chance of finding loot" },
	{ "IATTR_AmmoCapBonus", "% increased ammo capacities" },
	{ "IATTR_SpeedBonus", "% Speed" },
	{ "IATTR_MagazineIncrease", "% magazine size" },
	
	{ "IATTR_FlatDamageBonus_Physical", " physical damage to such attacks" },
	{ "IATTR_FlatDamageBonus_Energy", " energy damage to such attacks" },
	{ "IATTR_FlatDamageBonus_Explosive", " explosive damage to such attacks" },
	{ "IATTR_FlatDamageBonus_Magic", " magic damage to such attacks" },
	{ "IATTR_FlatDamageBonus_Elemental", " elemental damage to such attacks" },
	
	{ "IATTR_PercentDamageBonus_Physical", "% increased physical damage" },
	{ "IATTR_PercentDamageBonus_Energy", "% increased  energy damage" },
	{ "IATTR_PercentDamageBonus_Explosive", "% increased explosive damage" },
	{ "IATTR_PercentDamageBonus_Magic", "% increased magic damage" },
	{ "IATTR_PercentDamageBonus_Elemental", "% increased elemental damage" },
	
	{ "IATTR_SlotDamageBonus_1", "% slot 1 weapon damage" },
	{ "IATTR_SlotDamageBonus_2", "% slot 2 weapon damage" },
	{ "IATTR_SlotDamageBonus_3", "% slot 3 weapon damage" },
	{ "IATTR_SlotDamageBonus_4", "% slot 4 weapon damage" },
	{ "IATTR_SlotDamageBonus_5", "% slot 5 weapon damage" },
	{ "IATTR_SlotDamageBonus_6", "% slot 6 weapon damage" },
	{ "IATTR_SlotDamageBonus_7", "% slot 7 weapon damage" },
	{ "IATTR_SlotDamageBonus_8", "% slot 8 weapon damage" },
	{ "IATTR_SlotDamageBonus_9", "% temporary weapon damage" },
	
	{ "IATTR_PelletIncrease", "% more pellets" },
	
	{ "IATTR_ExplosionRadiusIncrease", "% increased explosion radius" },
	{ "IATTR_ExplosionResist", "% reduced self explosion damage" },
	
	{ "IATTR_AmmoGainChance", "% chance to gain ammo back on firing" },
	{ "IATTR_AmmoPickupIncrease", "% ammo gain from pickups" },
	{ "IATTR_ShopStockIncrease", "% increased shop stock" },
	
	{ "IATTR_RegenCap", " regeneration cap" },
	
	{ "IATTR_CritChance", "% crit chance" },
	{ "IATTR_CritChancePercent", "% increased crit chance" },
	{ "IATTR_CritDamage", "% additional crit damage" },
	
	{ "IATTR_KnockbackResist", " to knockback resist" },
	{ "IATTR_DamagePercent", "% increased damage" },
	{ "IATTR_Accuracy", " to accuracy rating" },
	
	{ "IATTR_StatBonus_STR", " to strength" },
	{ "IATTR_StatBonus_DEX", " to dexterity" },
	{ "IATTR_StatBonus_BUL", " to bulkiness" },
	{ "IATTR_StatBonus_CHR", " to charisma" },
	{ "IATTR_StatBonus_VIT", " to vitality" },
	{ "IATTR_StatBonus_INT", " to intellect" },
	
	{ "IATTR_ElementalResist", "% reduced elemental damage taken" },
	{ "IATTR_PhysicalResist", "% reduced physical damage taken" },
	{ "IATTR_ReflectResist", "% reduced reflected damage taken" },
	
	{ "IATTR_PhysPen", "% physical resist penetration" },
	{ "IATTR_EnergyPen", "% energy resist penetration" },
	{ "IATTR_ExplosivePen", "% explosive resist penetration" },
	{ "IATTR_OccultPen", "% occult resist penetration" },
	{ "IATTR_ElementalPen", "% elemental resist penetration" },
	
	{ "IATTR_FlatFireDmg", " to fire damage" },
	{ "IATTR_FlatIceDmg", " to ice damage" },
	{ "IATTR_FlatLightningDmg", " to lightning damage" },
	{ "IATTR_FlatPoisonDmg", " to poison damage" },
	
	{ "IATTR_Lifesteal", "% lifesteal" },
	
	{ "IATTR_PoisonTicrate", "% increased ticrate for poison" },
	{ "IATTR_PoisonDuration", "% increased poison stack duration" },
	{ "IATTR_PoisonTicDmg", "% increased damage per tic for poison" },
	
	{ "IATTR_BlockDmg", "% more damage against blocking enemies" },
	
	{ "IATTR_FreezeChance", "% increased chance to freeze for ice attacks" },
	{ "IATTR_SlowEffect", "% increased chill effectiveness for ice attacks" },
	
	{ "IATTR_IgniteChance", "% increased chance to ignite for fire atttacks" },
	{ "IATTR_IgniteDmg", "% increased ignite damage" },
	
	{ "IATTR_OverloadChance", "% chance to overload for lightning attacks", },
	{ "IATTR_OverloadZapCount", " additional zapped targets for overload", },
	{ "IATTR_OverloadZapDmg", "% increased overload zap damage", },
	
	// exotic ones
	{ "", "% chance to " },
	{ "IATTR_ChanceToCastElementalSpell", "% chance to cast random elemental spell on attack" },
	{ "IATTR_StatusBuffs_1", "Knockback Immunity" },
	{ "IATTR_StatusBuffs_1", "Effects of small charms multiplied by " },
	{ "", " to all attributes" },
	{ "IATTR_HealMissingHealthOnPain", "be healed for " },
	{ "IATTR_DamageIncrease_Lightning", "% increased lightning damage" },
	{ "IATTR_StatusBuffs_1", "Lightning damage always crits" },
	{ "IATTR_DamageIncrease_Shotguns", "% increased damage to shotguns" },
	{ "IATTR_HPPercent", "Double Health Cap" },
	{ "IATTR_DamagePerFlatHP", "Gain 1% damage increase every " },
	{ "IATTR_StatusBuffs_1", "Can't use armor" },
	{ "IATTR_StatusBuffs_1", "Burst type attacks fire in a circle around you" },
	{ "", "Burst type attacks also benefit from pellet bonuses" },
	{ "IATTR_ChanceToRaiseZombieFromKills", "% chance to raise a zombie from slain enemies" },
	{ "IATTR_StatusBuffs_1", "Damage taken is divided per each pet (Max 90% reduction)" },
	{ "IATTR_IncreasedDamageTaken", "% increased damage taken" },
	{ "", " flat damage to all attacks" },
	{ "IATTR_HealOnKill", "Killing enemies heal for " },
	{ "IATTR_StatusBuffs_1", "Soul type weapons do irreducible damage" },
	{ "IATTR_SoulAmmoIncrease", "% more ammo from soul pickups" },
	{ "IATTR_CastRally", "Gain ability to cast level " },
	{ "IATTR_StatusBuffs_1", "Spell damage is irreducible" },
	{ "IATTR_StatusBuffs_1", "Slain enemies rest in peace" }
};

Inv_attrib_T Inv_Attribute_Info[MAX_INV_ATTRIBUTE_TYPES] = {
	{ 5, 20, 5 },
	{ 5, 20, 5 },
	{ 2, 6, 2 },
	{ 2, 6, 2 },
	{ 1, 10, 5 },
	{ 1, 10, 5 },
	{ 0.005, 0.015, 1 },
	{ 0.025, 0.05, 2 },
	{ 5, 15, 3 },
	{ 0.01, 0.03, 1 },
	{ 2, 4, 2 },

	{ 1, 4, 1 },
	{ 1, 4, 1 },
	{ 1, 4, 1 },
	{ 1, 4, 1 },
	{ 1, 4, 1 },
	
	{ 2, 5, 8 },
	{ 2, 5, 8 },
	{ 2, 5, 8 },
	{ 2, 5, 8 },
	{ 2, 5, 8 },
	
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	
	{ 3, 8, 2 },
	
	{ 1, 4, 4 },
	{ 1, 5, 1 },
	
	{ 1, 5, 2 },
	{ 1, 3, 2 },
	{ 1, 10, 4 },
	
	{ 1, 25, 5 },
	
	{ 1, 5, 1 },
	{ 1, 5, 2 },
	{ 1, 10, 2 },
	
	{ 25, 100, 10 },
	{ 1, 5, 2 },
	{ 4, 125, 20 },
	
	{ 1, 5, 2 },
	{ 1, 5, 2 },
	{ 1, 5, 2 },
	{ 1, 5, 2 },
	{ 1, 5, 2 },
	{ 1, 5, 2 },
	
	{ 1, 2, 1 },
	{ 1, 2, 1 },
	{ 3, 10, 2 },
	
	{ 1, 5, 1 },
	{ 1, 5, 1 },
	{ 1, 5, 1 },
	{ 1, 5, 1 },
	{ 1, 5, 1 },
	
	{ 1, 5, 2 },
	{ 1, 5, 2 },
	{ 1, 5, 2 },
	{ 1, 5, 2 },
	
	// lifesteal
	{ 0.05, 0.1, 2 },
	
	{ 5, 10, 1 },
	{ 5, 10, 1 },
	{ 5, 10, 1 },
	
	{ 1, 15, 1 },

	{ 1, 5, 1 },
	{ 2, 5, 1 },
	
	{ 1, 5, 1 },
	{ 1, 10, 1 },
	
	{ 2, 5, 1 },
	{ 1, 2, 1 },
	{ 5, 10, 1 }
};

str ItemAttributeString(int attr, int val) {
	switch(attr) {
		case INV_DROPCHANCE_INCREASE:
		case INV_LUCK_INCREASE:
		case INV_SPEED_INCREASE:
			return StrParam(s:"+ ", f:ftrunc(val * 100), s:Inv_Attribute_Names[attr][INVATTR_TEXT]);
		
		case INV_DMGREDUCE_REFL:
			return StrParam(s:"+ ", f:ftrunc(val * 0.1), s:Inv_Attribute_Names[attr][INVATTR_TEXT]);
			
		case INV_LIFESTEAL:
			return StrParam(s:"+ ", f:ftrunc(val), s:Inv_Attribute_Names[attr][INVATTR_TEXT]);
		
		default:
			if(val > 0)
				return StrParam(s:"+ ", d:val, s:Inv_Attribute_Names[attr][INVATTR_TEXT]);
			else if(val < 0)
				StrParam(s:"- ", d:val, s:Inv_Attribute_Names[attr][INVATTR_TEXT]);
	}
	return "";
}

str ExoticAttributeString(int attr, int val1, int val2) {
	// treat it as normal inv attribute range
	if(attr <= LAST_INV_ATTRIBUTE)
		return ItemAttributeString(attr, val1);
	// we must map all exotic ones with the proper macro
	switch(attr) {
		case INV_EX_FACTOR_SMALLCHARM:
		return StrParam(s:Inv_Attribute_Names[UNIQUE_MAP_MACRO(INV_EX_FACTOR_SMALLCHARM)][INVATTR_TEXT], f:ftrunc2((val1 << 16) / FACTOR_SMALLCHARM_RESOLUTION));
		case INV_EX_CHANCE_HEALMISSINGONPAIN:
		return StrParam(d:val1, s:Inv_Attribute_Names[UNIQUE_MAP_MACRO(INV_EX_CHANCE)][INVATTR_TEXT], s:Inv_Attribute_Names[UNIQUE_MAP_MACRO(INV_EX_CHANCE_HEALMISSINGONPAIN)][INVATTR_TEXT], d:val2, s:"% missing health");
		case INV_EX_DAMAGEPER_FLATHEALTH:
		return StrParam(s:Inv_Attribute_Names[UNIQUE_MAP_MACRO(INV_EX_DAMAGEPER_FLATHEALTH)][INVATTR_TEXT], d:val1, s:" maximum health");
		case INV_EX_ONKILL_HEALMISSING:
		return StrParam(s:Inv_Attribute_Names[UNIQUE_MAP_MACRO(INV_EX_ONKILL_HEALMISSING)][INVATTR_TEXT], d:val1, s:"% missing health");
		case INV_EX_ABILITY_RALLY:
		return StrParam(s:Inv_Attribute_Names[UNIQUE_MAP_MACRO(INV_EX_ABILITY_RALLY)][INVATTR_TEXT], d:val1, s:" Rally");
		case INV_EX_DOUBLE_HEALTHCAP:
		return Inv_Attribute_Names[UNIQUE_MAP_MACRO(attr)][INVATTR_TEXT];
		default:
			if(val1)
				return StrParam(s:"+ ", d:val1, s:Inv_Attribute_Names[UNIQUE_MAP_MACRO(attr)][INVATTR_TEXT]);
			return Inv_Attribute_Names[UNIQUE_MAP_MACRO(attr)][INVATTR_TEXT];
	}
	return "";
}

#endif
