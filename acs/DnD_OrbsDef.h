#ifndef DND_ORBSDEF_IN
#define DND_ORBSDEF_IN

#include "DnD_WeaponDefs.h"

// kept seperate to reduce dependancy

typedef struct {
	pstat_T orb_stat_bonuses;
	wep_info_T weapon_stat_bonuses[MAXWEPS];
} orb_max_info_T;

global orb_max_info_T 8: Player_Orb_Data[MAXPLAYERS];

#endif