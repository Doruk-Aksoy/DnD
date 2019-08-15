#ifndef DND_ELITEINFO_IN
#define DND_ELITEINFO_IN

#include "DnD_ElitePow.h"

#define MAX_MONSTER_TRAITS 31
#define MONSTER_TRAITID 1403
#define MONSTER_BARID (MONSTER_TRAITID + MAX_MONSTER_TRAITS + 1)
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

#define MAX_MONSTER_TRAITS2 19
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
	"Revived",
	"Ice Creature",
	"Fire Creature",
	"Stone Creature",
	"Elemental Weakness",
	"Earth Creature",
	
	"Legendary"
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
			
			case DND_FIRECREATURE_POW:
			case DND_ICECREATURE_POW:
			case DND_STONECREATURE_POW:
			case DND_EARTHCREATURE_POW:
			return EliteTraitColorCode[DND_TRAITCODE_RESIST];
			
			case DND_ELEMENTAL_WEAKNESS_POW:
			return EliteTraitColorCode[DND_TRAITCODE_WEAKNESS];
		}
	}
	return EliteTraitColorCode[DND_TRAITCODE_NULL];
}

#endif