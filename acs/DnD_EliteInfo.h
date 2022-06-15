#ifndef DND_ELITEINFO_IN
#define DND_ELITEINFO_IN

#define DND_FORTIFY_AMOUNT 3 // 30%

#define REPEL_REFRESH_RATE (3 * TICRATE / 2)
#define REPEL_COOLDOWN (TICRATE * 3)
#define REPEL_DISTANCE 160.0

#define CRIPPLE_REFRESH_RATE (TICRATE / 2)
#define CRIPPLE_DISTANCE 192.0

#define RUINATION_MAX_STACKS 7
#define RUINATION_TIC 7
#define RUINATION_MAX_TICS 15

enum {
	DND_TRAITCODE_WEAKNESS,
	DND_TRAITCODE_RESIST,
	DND_TRAITCODE_IMMUNITY,
	DND_TRAITCODE_AGGRESSIVE,
	DND_TRAITCODE_DEFENSIVE,
	DND_TRAITCODE_UTILITY
};

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
	DND_REJUVENATING,
	
	DND_HATRED,
	DND_SHOCKER,
	DND_VAMPIRISM,
	DND_FORTIFIED,
	DND_SUBORDINATE,
	DND_REPEL,
	DND_PHANTASM,
	DND_CRIPPLE,
	DND_RUINATION,
	DND_ETHEREAL,
	
	DND_MARKOFCHAOS,
	
	DND_LEGENDARY
};
#define MAX_MONSTER_TRAITS (DND_LEGENDARY + 1)
#define FIRST_MONSTER_TRAIT DND_ENERGY_WEAKNESS
#define LAST_MONSTER_TRAIT DND_LEGENDARY

str GetMonsterTraitLabel(int id) {
	// this is faster than using strparam, and the places that'd call this would potentially call it often so we want to avoid strparam
	// also removes color code seperation and combines them together
	// the entire reason we do this is to get rid of storing strings in stack and move them to language lump
	switch(id) {
		// weaknesses
		case DND_ENERGY_WEAKNESS:
		return "DND_EMOD_ENERGYWEAKNESS";
		case DND_SILVER_WEAKNESS:
		return "DND_EMOD_SILVERWEAKNESS";
		case DND_FIRE_WEAKNESS:
		return "DND_EMOD_FIREWEAKNESS";
		case DND_ICE_WEAKNESS:
		return "DND_EMOD_ICEWEAKNESS";
		case DND_MAGIC_WEAKNESS:
		return "DND_EMOD_MAGICWEAKNESS";
		case DND_ELEMENTAL_WEAKNESS:
		return "DND_EMOD_ELEWEAKNESS";
		
		// resists
		case DND_EXPLOSIVE_RESIST:
		return "DND_EMOD_EXPRESIST";
		case DND_BULLET_RESIST:
		return "DND_EMOD_PHYSRESIST";
		case DND_ENERGY_RESIST:
		return "DND_EMOD_ENERGYRESIST";
		case DND_MAGIC_RESIST:
		return "DND_EMOD_MAGICRESIST";
		case DND_ELEMENTAL_RESIST:
		return "DND_EMOD_ELERESIST";
		
		// immune
		case DND_EXPLOSIVE_IMMUNE:
		return "DND_EMOD_HIGHEXPRESIST";
		case DND_EXPLOSIVE_NONE:
		return "DND_EMOD_EXPIMMUNE";
		case DND_BULLET_IMMUNE:
		return "DND_EMOD_PHYSIMMUNE";
		case DND_ENERGY_IMMUNE:
		return "DND_EMOD_ENERGYIMMUNE";
		case DND_MAGIC_IMMUNE:
		return "DND_EMOD_MAGICIMMUNE";
		case DND_ELEMENTAL_IMMUNE:
		return "DND_EMOD_ELEIMMUNE";
		
		// other
		case DND_GHOST:
		return "DND_EMOD_GHOST";
		case DND_HARDENED_SKIN:
		return "DND_EMOD_HARDSKIN";
		case DND_REFLECTIVE:
		return "DND_EMOD_REFLECTIVE";
		case DND_MINIONS:
		return "DND_EMOD_SUMMONMINION";
		case DND_CURSE:
		return "DND_EMOD_CANCURSE";
		
		case DND_HEAL:
		return "DND_EMOD_CANHEAL";
		case DND_BLOCK:
		return "DND_EMOD_CANBLOCK";
		case DND_SPLIT:
		return "DND_EMOD_CANSPLIT";
		case DND_RAISE:
		return "DND_EMOD_CANRAISE";
		case DND_TELEPORT:
		return "DND_EMOD_CANTELEPORT";
		
		case DND_RESURRECT:
		return "DND_EMOD_RESFALLEN";
		case DND_MOBILITY:
		return "DND_EMOD_MOBILITY";
		case DND_HOMING:
		return "DND_EMOD_HOMING";
		case DND_POISON:
		return "DND_EMOD_POISONOUS";
		case DND_DEATH:
		return "DND_EMOD_DEATHSURP";

		case DND_RAGE:
		return "DND_EMOD_RAGE";
		case DND_PIERCE:
		return "DND_EMOD_PIERCEARMOR";
		case DND_AGGRESSIVE:
		return "DND_EMOD_AGGRESSIVE";
		case DND_EXTRAFAST:
		return "DND_EMOD_EXTRAFAST";
		case DND_FASTREACTION:
		return "DND_EMOD_FASTREACT";
		
		case DND_NOPAIN:
		return "DND_EMOD_NOPAIN";
		case DND_EXTRASTRONG:
		return "DND_EMOD_EXTRASTRONG";
		case DND_VITAL:
		return "DND_EMOD_VITAL";
		case DND_ARMORPEN:
		return "DND_EMOD_ARMORPEN";
		case DND_PET:
		return "DND_EMOD_PET";
		
		case DND_ISBLOCKING:
		return "DND_EMOD_ISBLOCKING";
		
		case DND_SUMMONED:
		return "DND_EMOD_SUMMONED";
		case DND_REVIVED:
		return "DND_EMOD_REVIVED";
		case DND_ICECREATURE:
		return "DND_EMOD_ICECREATURE";
		case DND_FIRECREATURE:
		return "DND_EMOD_FIRECREATURE";
		case DND_STONECREATURE:
		return "DND_EMOD_STONECREATURE";
		case DND_EARTHCREATURE:
		return "DND_EMOD_EARTHCREATURE";
		
		case DND_BLOODLESS:
		return "DND_EMOD_BLOODLESS";
		case DND_VIOLENTRETALIATION:
		return "DND_EMOD_VIOLENTRET";
		case DND_THIEF:
		return "DND_EMOD_THIEF";
		case DND_HEXFUSION:
		return "DND_EMOD_HEXFUSION";
		case DND_REBIRTH:
		return "DND_EMOD_REBIRTH";
		case DND_VENOMANCER:
		return "DND_EMOD_VENOMANCER";
		case DND_FRIGID:
		return "DND_EMOD_FRIGID";
		case DND_SCORCHED:
		return "DND_EMOD_SCORCHED";
		case DND_INSULATED:
		return "DND_EMOD_INSULATED";
		case DND_REJUVENATING:
		return "DND_EMOD_REJUVENATE";
		
		case DND_HATRED:
		return "DND_EMOD_HATRED";
		case DND_SHOCKER:
		return "DND_EMOD_SHOCKER";
		case DND_VAMPIRISM:
		return "DND_EMOD_VAMPIRISM";
		case DND_FORTIFIED:
		return "DND_EMOD_FORTIFIED";
		case DND_SUBORDINATE:
		return "DND_EMOD_SUBORDINATE";
		case DND_REPEL:
		return "DND_EMOD_REPEL";
		case DND_PHANTASM:
		return "DND_EMOD_PHANTASM";
		case DND_CRIPPLE:
		return "DND_EMOD_CRIPPLE";
		case DND_RUINATION:
		return "DND_EMOD_RUINATION";
		case DND_ETHEREAL:
		return "DND_EMOD_ETHEREAL";
		
		case DND_MARKOFCHAOS:
		return "DND_EMOD_MARKOFCHAOS";
		
		case DND_LEGENDARY:
		return "DND_EMOD_LEGENDARY";
	}
	return "";
}

#endif
