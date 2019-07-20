#ifndef DND_ELITEPOW_IN
#define DND_ELITEPOW_IN

enum {
	// Traitset 1
	
	DND_MELEE_RESIST, // obsolete
	
	DND_ENERGY_WEAKNESS,
	DND_SILVER_WEAKNESS,
	DND_FIRE_WEAKNESS = DND_SILVER_WEAKNESS << 1,
	DND_ICE_WEAKNESS = DND_FIRE_WEAKNESS << 1,
	DND_MAGIC_WEAKNESS = DND_ICE_WEAKNESS << 1,
	
	DND_EXPLOSIVE_RESIST = DND_MAGIC_WEAKNESS << 1,
	DND_EXPLOSIVE_IMMUNE = DND_EXPLOSIVE_RESIST << 1,
	DND_EXPLOSIVE_NONE = DND_EXPLOSIVE_IMMUNE << 1,
	DND_BULLET_RESIST = DND_EXPLOSIVE_NONE << 1,
	DND_GHOST = DND_BULLET_RESIST << 1,
	DND_HARDENED_SKIN = DND_GHOST << 1,
	DND_REFLECTIVE = DND_HARDENED_SKIN << 1,
	DND_MINIONS = DND_REFLECTIVE << 1,
	DND_CURSE = DND_MINIONS << 1,
	DND_HEAL = DND_CURSE << 1,
	DND_BLOCK = DND_HEAL << 1,
	DND_SPLIT = DND_BLOCK << 1,
	DND_RAISE = DND_SPLIT << 1,
	DND_TELEPORT = DND_RAISE << 1,
	DND_RESURRECT = DND_TELEPORT << 1,
	DND_MOBILITY = DND_RESURRECT << 1,
	DND_HOMING = DND_MOBILITY << 1,
	DND_POISON = DND_HOMING << 1,
	DND_DEATH = DND_POISON << 1,
	DND_RAGE = DND_DEATH << 1,
	DND_PIERCE = DND_RAGE << 1,
	DND_AGGRESSIVE = DND_PIERCE << 1,
	DND_EXTRAFAST = DND_AGGRESSIVE << 1,
	DND_FASTREACTION = DND_EXTRAFAST << 1,
	DND_NOPAIN = DND_FASTREACTION << 1,
	
	// Traitset 2
	
	DND_BULLET_IMMUNE = 1,
	DND_ENERGY_RESIST = DND_BULLET_IMMUNE << 1,
	DND_ENERGY_IMMUNE = DND_ENERGY_RESIST << 1,
	DND_MAGIC_RESIST = DND_ENERGY_IMMUNE << 1,
	DND_MAGIC_IMMUNE = DND_MAGIC_RESIST	<< 1,
	DND_ELEMENTAL_RESIST = DND_MAGIC_IMMUNE << 1,
	DND_ELEMENTAL_IMMUNE = DND_ELEMENTAL_RESIST << 1,
	DND_EXTRASTRONG = DND_ELEMENTAL_IMMUNE << 1,
	DND_VITAL = DND_EXTRASTRONG << 1,
	DND_ARMORPEN = DND_VITAL << 1,
	DND_PET = DND_ARMORPEN << 1,
	DND_SUMMONED = DND_PET << 1,
	
	DND_LEGENDARY = DND_SUMMONED << 1,
	DND_REVIVED = DND_LEGENDARY << 1
};

enum {
	DND_ENERGY_WEAKNESS_POW,
	DND_SILVER_WEAKNESS_POW,
	DND_FIRE_WEAKNESS_POW,
	DND_ICE_WEAKNESS_POW,
	DND_MAGIC_WEAKNESS_POW,
	
	DND_EXPLOSIVE_RESIST_POW,
	DND_EXPLOSIVE_IMMUNE_POW,
	DND_EXPLOSIVE_NONE_POW,
	DND_BULLET_RESIST_POW,
	DND_GHOST_POW,
	DND_HARDENED_SKIN_POW,
	DND_REFLECTIVE_POW,
	DND_MINIONS_POW,
	DND_CURSE_POW,
	DND_HEAL_POW,
	DND_BLOCK_POW,
	DND_SPLIT_POW,
	DND_RAISE_POW,
	DND_TELEPORT_POW,
	DND_RESURRECT_POW,
	DND_MOBILITY_POW,
	DND_HOMING_POW,
	DND_POISON_POW,
	DND_DEATH_POW,
	DND_RAGE_POW,
	DND_PIERCE_POW,
	DND_AGGRESSIVE_POW,
	DND_EXTRAFAST_POW,
	DND_FASTREACTION_POW,
	DND_NOPAIN_POW,
	
	// Traitset 2
	
	DND_BULLET_IMMUNE_POW = 0,
	DND_ENERGY_RESIST_POW,
	DND_ENERGY_IMMUNE_POW,
	DND_MAGIC_RESIST_POW,
	DND_MAGIC_IMMUNE_POW,
	DND_ELEMENTAL_RESIST_POW,
	DND_ELEMENTAL_IMMUNE_POW,
	DND_EXTRASTRONG_POW,
	DND_VITAL_POW,
	DND_ARMORPEN_POW,
	DND_PET_POW,
	DND_SUMMONED_POW,
	
	DND_LEGENDARY_POW,
	DND_REVIVED_POW
};

#define MAX_MONSTER_TRAITS 31
#define MONSTER_TRAITID 1403
#define MONSTER_BARID MONSTER_TRAITID + MAX_MONSTER_TRAITS + 1
// up to 1102 is occupied by barfillid, 1102 included
#define MONSTER_BARFILLID 1301
#define MONSTER_TEXTID 1300
#define MONSTER_NAMEID 1299
#define MONSTER_TYPEICONID 1298

str MonsterTraits[MAX_MONSTER_TRAITS] = {
	"Energy Weakness",
	"Silver Weakness",
	"Fire Weakness",
	"Ice Weakness",
	"Magic Weakness",
	
	"Explosive Resist",
	"High Explosive Resist",
	"Explosive Immune",
	"Physical Resist",
	"Ghost",
	"Hardened Skin",	// ripper immune
	"Reflective",
	"Summon Minion",
	"Can Curse",		// monsters with curses
	"Can Heal",		// monsters with healing
	"Can Block",		// shielded monster
	"Can Split",
	"Can Raise",
	"Can Teleport",
	"Resurrect Fallen",	// arch vile
	"Mobility",	// dashes, fast speed etc
	"Homing",
	"Poisonous",
	"Death Surprise",	// monsters with dangerous deaths
	"Rage", // monsters that get angry after a certain condition
	"Pierces Armor", // monsters that ignore player armor
	"Aggressive", // more aggression
	"Extra Fast", // nightmare fast
	"Fast Reaction", // quicktoretaliate
	"No Pain",
	"Magic Resist"
};

#define MAX_MONSTER_TRAITS2 14
str MonsterTraits2[MAX_MONSTER_TRAITS2] = {
	"Physical Immune",
	"Energy Resist",
	"Energy Immune",
	"Magic Resist",
	"Magic Immune",
	"Elemental Resist",
	"Elemental Immune",
	"Extra Strong",
	"Vital",
	"Armor Penetration",
	"Pet Creature",
	"Summoned Creature",
	"Legendary",
	"Revived"
};

enum {
	DND_TRAITCODE_NULL,
	DND_TRAITCODE_WEAKNESS,
	DND_TRAITCODE_RESIST,
	DND_TRAITCODE_IMMUNITY,
	DND_TRAITCODE_AGGRESSIVE,
	DND_TRAITCODE_DEFENSIVE,
	DND_TRAITCODE_UTILITY
};

enum {
	DND_TRAITSIDE_1,
	DND_TRAITSIDE_2
};

#define MAX_ELITE_TRAIT_COLORCODES DND_TRAITCODE_UTILITY + 1
str EliteTraitColorCode[MAX_ELITE_TRAIT_COLORCODES] = {
	"\cj",
	"\cf",
	"\cu",
	"\cm",
	"\cg",
	"\cq",
	"\cv"
};

str GetTraitColorCode(int trait, int side) {
	if(!side) {
		switch(trait) {
			case DND_ENERGY_WEAKNESS_POW:
			case DND_SILVER_WEAKNESS_POW:
			case DND_FIRE_WEAKNESS_POW:
			case DND_ICE_WEAKNESS_POW:
			case DND_MAGIC_WEAKNESS_POW:
			return EliteTraitColorCode[DND_TRAITCODE_WEAKNESS];
			
			case DND_EXPLOSIVE_RESIST_POW:
			case DND_EXPLOSIVE_IMMUNE_POW:
			case DND_BULLET_RESIST_POW:
			return EliteTraitColorCode[DND_TRAITCODE_RESIST];
			
			case DND_EXPLOSIVE_NONE_POW:
			return EliteTraitColorCode[DND_TRAITCODE_IMMUNITY];
			
			case DND_NOPAIN_POW:
			case DND_POISON_POW:
			case DND_MOBILITY_POW:
			case DND_RESURRECT_POW:
			case DND_TELEPORT_POW:
			case DND_RAISE_POW:
			case DND_CURSE_POW:
			return EliteTraitColorCode[DND_TRAITCODE_UTILITY];
			
			case DND_HEAL_POW:
			case DND_BLOCK_POW:
			case DND_DEATH_POW:
			case DND_SPLIT_POW:
			case DND_MINIONS_POW:
			case DND_REFLECTIVE_POW:
			case DND_HARDENED_SKIN_POW:
			case DND_GHOST_POW:
			return EliteTraitColorCode[DND_TRAITCODE_DEFENSIVE];
			
			case DND_HOMING_POW:
			case DND_RAGE_POW:
			case DND_PIERCE_POW:
			case DND_AGGRESSIVE_POW:
			case DND_EXTRAFAST_POW:
			case DND_FASTREACTION_POW:
			return EliteTraitColorCode[DND_TRAITCODE_AGGRESSIVE];
		}
	}
	else {
		switch(trait) {
			case DND_ENERGY_RESIST_POW:
			case DND_MAGIC_RESIST_POW:
			case DND_ELEMENTAL_RESIST_POW:
			return EliteTraitColorCode[DND_TRAITCODE_RESIST];
			
			case DND_BULLET_IMMUNE_POW:
			case DND_ENERGY_IMMUNE_POW:
			case DND_MAGIC_IMMUNE_POW:
			case DND_ELEMENTAL_IMMUNE_POW:
			return EliteTraitColorCode[DND_TRAITCODE_IMMUNITY];
			
			case DND_EXTRASTRONG_POW:
			case DND_ARMORPEN_POW:
			return EliteTraitColorCode[DND_TRAITCODE_AGGRESSIVE];
			
			case DND_VITAL_POW:
			return EliteTraitColorCode[DND_TRAITCODE_DEFENSIVE];
			
			case DND_PET_POW:
			case DND_REVIVED_POW:
			case DND_SUMMONED_POW:
			return EliteTraitColorCode[DND_TRAITCODE_UTILITY];
		}
	}
	return EliteTraitColorCode[DND_TRAITCODE_NULL];
}

#endif