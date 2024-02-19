#ifndef DND_ORBDEF_IN
#define DND_ORBDEF_IN

#define DND_MAX_ORB_REROLL_ATTEMPTS 100

enum {
	DND_ORB_ENHANCE,
	DND_ORB_CORRUPT,
	DND_ORB_PRISMATIC,
	DND_ORB_REPENT,
	DND_ORB_AFFLUENCE,

	DND_ORB_CALAMITY,
	DND_ORB_PROSPERITY,
	DND_ORB_NULLIFICATION,
	DND_ORB_DESTRUCTION,
	DND_ORB_VIOLENCE,

	DND_ORB_FORTITUDE,
	DND_ORB_SIN,
	DND_ORB_TREMORS,
	DND_ORB_TINKERER,
	DND_ORB_REFINEMENT,

	DND_ORB_SCULPTING,
	DND_ORB_ELEVATION,
	DND_ORB_TURMOIL,
	DND_ORB_HEXES,
	DND_ORB_GROWTH,

	DND_ORB_POTENCY,
	DND_ORB_CRACKLING,
	DND_ORB_BRUTE,
	DND_ORB_JAGGED,
	DND_ORB_ALCHEMIST,
	
	DND_ORB_EVOKER,
	DND_ORB_SAVAGERY,

	// unique drops from specific monsters
	DND_ORB_HOLLOW,
	DND_ORB_PHANTASMAL,
	DND_ORB_ASSIMILATION
};

int PickLowTierOrb() {
	int o = random(0, 11);
	switch(o) {
		case 0:
		case 1:
		case 2:
			o = DND_ORB_ENHANCE;
		break;
		case 3:
		case 4:
		case 5:
			o = DND_ORB_TURMOIL;
		break;
		case 6:
		case 7:
			o = DND_ORB_REFINEMENT;
		break;
		case 8:
		case 9:
			o = DND_ORB_SCULPTING;
		break;
		case 10:
			o = DND_ORB_EVOKER;
		break;
		case 11:
			o = DND_ORB_CALAMITY;
		break;
	}
	return o;
}

int PickMidTierOrb() {
	int o = random(0, 13);
	switch(o) {
		case 0:
			o = DND_ORB_PRISMATIC;
		break;
		case 1:
			o = DND_ORB_DESTRUCTION;
		break;
		case 2:
			o = DND_ORB_JAGGED;
		break;
		case 3:
			o = DND_ORB_CRACKLING;
		break;
		case 4:
			o = DND_ORB_BRUTE;
		break;
		case 5:
			o = DND_ORB_TREMORS;
		break;
		case 6:
			o = DND_ORB_TINKERER;
		break;
		case 7:
			o = DND_ORB_VIOLENCE;
		break;
		case 8:
			o = DND_ORB_PROSPERITY;
		break;
		case 9:
			o = DND_ORB_FORTITUDE;
		break;
		case 10:
			o = DND_ORB_HEXES;
		break;
		case 11:
			o = DND_ORB_GROWTH;
		break;
		case 12:
			o = DND_ORB_ELEVATION;
		break;
		case 13:
			o = DND_ORB_SAVAGERY;
		break;
	}
	return o;
}

int PickHighTierOrb() {
	int o = random(0, 11);
	switch(o) {
		case 0:
		case 1:
			o = DND_ORB_CORRUPT;
		break;
		case 2:
		case 3:
			o = DND_ORB_SIN;
		break;
		case 4:
		case 5:
			o = DND_ORB_ALCHEMIST;
		break;
		case 6:
			o = DND_ORB_AFFLUENCE;
		break;
		case 7:
			o = DND_ORB_REPENT;
		break;
		case 8:
		case 9:
			o = DND_ORB_NULLIFICATION;
		break;
		case 10:
		case 11:
			o = DND_ORB_POTENCY;
		break;
	}
	return o;
}

#endif
