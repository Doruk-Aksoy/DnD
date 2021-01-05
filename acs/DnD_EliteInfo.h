#ifndef DND_ELITEINFO_IN
#define DND_ELITEINFO_IN

enum {
	// weaknesses
	DND_ENERGY_WEAKNESS,
	DND_SILVER_WEAKNESS,
	DND_FIRE_WEAKNESS,
	DND_ICE_WEAKNESS,
	DND_MAGIC_WEAKNESS,
	DND_ELEMENTAL_WEAKNESS,
	
	// resists
	DND_EXPLOSIVE_RESIST,
	DND_BULLET_RESIST,
	DND_ENERGY_RESIST,
	DND_MAGIC_RESIST,
	DND_ELEMENTAL_RESIST,
	
	// immune
	DND_EXPLOSIVE_IMMUNE,
	DND_EXPLOSIVE_NONE,
	DND_BULLET_IMMUNE,
	DND_ENERGY_IMMUNE,
	DND_MAGIC_IMMUNE,
	DND_ELEMENTAL_IMMUNE,
	
	// other
	DND_GHOST,
	DND_HARDENED_SKIN,
	DND_REFLECTIVE,
	DND_MINIONS,
	DND_CURSE,
	
	DND_HEAL,
	DND_BLOCK,
	DND_SPLIT,
	DND_RAISE,
	DND_TELEPORT,
	
	DND_RESURRECT,
	DND_MOBILITY,
	DND_HOMING,
	DND_POISON,
	DND_DEATH,

	DND_RAGE,
	DND_PIERCE,
	DND_AGGRESSIVE,
	DND_EXTRAFAST,
	DND_FASTREACTION,
	
	DND_NOPAIN,
	DND_EXTRASTRONG,
	DND_VITAL,
	DND_ARMORPEN,
	DND_PET,
	
	DND_ISBLOCKING,
	
	DND_SUMMONED,
	DND_REVIVED,
	DND_ICECREATURE,
	DND_FIRECREATURE,
	DND_STONECREATURE,
	DND_EARTHCREATURE,
	
	DND_BLOODLESS,
	DND_VIOLENTRETALIATION,
	DND_THIEF,
	DND_HEXFUSION,
	DND_REBIRTH,
	DND_VENOMANCER,
	DND_FRIGID,
	DND_SCORCHED,
	DND_INSULATED,
	
	DND_LEGENDARY
};
#define MAX_MONSTER_TRAITS (DND_LEGENDARY + 1)

#define MONSTER_TRAITID 1403
#define MONSTER_BARID (MONSTER_TRAITID + MAX_MONSTER_TRAITS + 1)
#define FIRST_MONSTER_TRAIT DND_ENERGY_WEAKNESS
#define LAST_MONSTER_TRAIT DND_LEGENDARY
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
	"Elemental Weakness",
	
	"Explosive Resist",
	"Physical Resist",
	"Energy Resist",
	"Magic Resist",
	"Elemental Resist",
	
	"High Explosive Resist",
	"Explosive Immune",
	"Physical Immune",
	"Energy Immune",
	"Magic Immune",
	"Elemental Immune",
	
	"Ghost",
	"Hardened Skin",					// ripper immune
	"Reflective",
	"Summon Minion",
	"Can Curse",						// monsters with curses
	
	"Can Heal",							// monsters with healing
	"Can Block",						// shielded monster
	"Can Split",
	"Can Raise",
	"Can Teleport",
	
	"Resurrect Fallen",					// arch vile style resurrection
	"Mobility",							// dashes, fast speed etc
	"Homing",
	"Poisonous",
	"Death Surprise",					// monsters with dangerous deaths
	
	"Rage", 							// monsters that get angry after a certain condition
	"Pierces Armor", 					// monsters that ignore player armor
	"Aggressive", 						// more aggression
	"Extra Fast", 						// nightmare fast
	"Fast Reaction", 					// quicktoretaliate

	"No Pain",
	"Extra Strong",
	"Vital",
	"Armor Penetration",
	"Pet Creature",
	
	"Blocking Attacks",
	
	"Summoned Creature",
	"Revived",
	"Ice Creature",
	"Fire Creature",
	"Stone Creature",
	"Earth Creature",
	
	"Bloodless",
	"Violent Retaliation",
	"Thief",
	"Hexfusion",
	"Rebirth",
	"Venomancer",
	"Frigid",
	"Scorched",
	"Insulated",
	
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

// traits #1 or #2
str GetTraitColorCode(int trait) {
	switch(trait) {
		case DND_ENERGY_WEAKNESS:
		case DND_SILVER_WEAKNESS:
		case DND_FIRE_WEAKNESS:
		case DND_ICE_WEAKNESS:
		case DND_MAGIC_WEAKNESS:
		case DND_ELEMENTAL_WEAKNESS:
		return EliteTraitColorCode[DND_TRAITCODE_WEAKNESS];
		
		case DND_EXPLOSIVE_RESIST:
		case DND_ENERGY_RESIST:
		case DND_MAGIC_RESIST:
		case DND_ELEMENTAL_RESIST:
		
		case DND_EXPLOSIVE_IMMUNE:
		case DND_BULLET_RESIST:
		return EliteTraitColorCode[DND_TRAITCODE_RESIST];
		
		case DND_EXPLOSIVE_NONE:
		return EliteTraitColorCode[DND_TRAITCODE_IMMUNITY];
		
		case DND_NOPAIN:
		case DND_POISON:
		case DND_MOBILITY:
		case DND_RESURRECT:
		case DND_TELEPORT:
		case DND_RAISE:
		case DND_CURSE:
		case DND_HEXFUSION:
		case DND_THIEF:
		return EliteTraitColorCode[DND_TRAITCODE_UTILITY];
		
		case DND_HEAL:
		case DND_BLOCK:
		case DND_DEATH:
		case DND_SPLIT:
		case DND_MINIONS:
		case DND_REFLECTIVE:
		case DND_HARDENED_SKIN:
		case DND_GHOST:
		case DND_ISBLOCKING:
		case DND_BLOODLESS:
		return EliteTraitColorCode[DND_TRAITCODE_DEFENSIVE];
		
		case DND_HOMING:
		case DND_RAGE:
		case DND_PIERCE:
		case DND_AGGRESSIVE:
		case DND_EXTRAFAST:
		case DND_FASTREACTION:
		case DND_VIOLENTRETALIATION:
		return EliteTraitColorCode[DND_TRAITCODE_AGGRESSIVE];

		return EliteTraitColorCode[DND_TRAITCODE_RESIST];
		
		case DND_BULLET_IMMUNE:
		case DND_ENERGY_IMMUNE:
		case DND_MAGIC_IMMUNE:
		case DND_ELEMENTAL_IMMUNE:
		case DND_SCORCHED:
		case DND_VENOMANCER:
		case DND_FRIGID:
		case DND_INSULATED:
		return EliteTraitColorCode[DND_TRAITCODE_IMMUNITY];
		
		case DND_EXTRASTRONG:
		case DND_ARMORPEN:
		return EliteTraitColorCode[DND_TRAITCODE_AGGRESSIVE];
		
		case DND_VITAL:
		return EliteTraitColorCode[DND_TRAITCODE_DEFENSIVE];
		
		case DND_PET:
		case DND_REVIVED:
		case DND_SUMMONED:
		return EliteTraitColorCode[DND_TRAITCODE_UTILITY];
		
		case DND_FIRECREATURE:
		case DND_ICECREATURE:
		case DND_STONECREATURE:
		case DND_EARTHCREATURE:
		return EliteTraitColorCode[DND_TRAITCODE_RESIST];
		
		case DND_LEGENDARY:
		return "\ci";
	}
	return EliteTraitColorCode[DND_TRAITCODE_NULL];
}

#endif
