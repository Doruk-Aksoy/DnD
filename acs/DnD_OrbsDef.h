#ifndef DND_ORBSDEF_IN
#define DND_ORBSDEF_IN

#include "DnD_WeaponDefs.h"

// kept seperate to reduce dependancy

typedef struct {
	int hp_flat_bonus;
	int armor_flat_bonus;
	
	int hp_percent_bonus;
	int armor_percent_bonus;
	
	int greed_percent_bonus;
	int wisdom_percent_bonus;
	
	int speed_bonus;
	int drop_chance;
	int holding;
	
	int luck; // this is in fixed!
	
	int damage_type_bonus[MAX_TALENTS];
} orb_stat_T;

typedef struct {
	orb_stat_T orb_stat_bonuses;
	wep_info_T weapon_stat_bonuses[MAXWEPS];
} orb_max_info_T;

global orb_max_info_T 8: Player_Orb_Data[MAXPLAYERS];

#endif