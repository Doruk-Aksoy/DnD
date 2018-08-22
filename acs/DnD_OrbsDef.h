#ifndef DND_ORBSDEF_IN
#define DND_ORBSDEF_IN

#include "DnD_WeaponDefs.h"

// kept seperate to reduce dependancy

typedef struct {
	pstat_T orb_stat_bonuses;
	wep_info_T weapon_stat_bonuses[MAXWEPS];
} orb_max_info_T;

global orb_max_info_T 8: Player_Orb_Data[MAXPLAYERS];

enum {
	DND_ORB_ENHANCE,
	DND_ORB_CORRUPT,
	DND_ORB_SPIRIT,
	DND_ORB_REPENT,
	DND_ORB_AFFLUENCE,
	DND_ORB_CALAMITY,
	DND_ORB_PROSPERITY,
	DND_ORB_WISDOM,
	DND_ORB_GREED,
	DND_ORB_VIOLENCE,
	DND_ORB_FORTITUDE,
	DND_ORB_SIN,
	DND_ORB_RICHES,
	DND_ORB_HOLDING,
	DND_ORB_REFINEMENT,
	DND_ORB_SCULPTING,
	DND_ORB_ELEVATION
};
#define MAX_ORBS DND_ORB_ELEVATION + 1

#endif