#ifndef DND_SPECIALAMMO_IN
#define DND_SPECIALAMMO_IN

#include "DnD_DamageCacheDefs.h"
#include "DnD_Weapons.h"
#include "DnD_Ammo.h"

enum {
	SSAM_FLECHETTE,
	SSAM_MAGNUM,
	SSAM_SHOCK,
	SSAM_NITROSHELL,
	SSAM_SLUG,
	
	SSAM_40MM, // this is ignored for shop
	SSAM_40MMSONIC,
	SSAM_40MMHE
};

#define MAX_SPECIAL_AMMOS (SSAM_40MMHE + 1)
#define MAX_SPECIAL_AMMOS_FOR_SHOP (MAX_SPECIAL_AMMOS - 1) // we need to ignore the regular grenade here
#define MAX_SPECIALAMMO_DAMAGEINDEX 3

str SpecialAmmoInfo_Str[MAX_SPECIAL_AMMOS_FOR_SHOP][2] = {
	{	"SAM1A0",			"FlechetteShell"		},
	{	"SAM3A0",			"PiercingShell"			},
	{	"SAM2A0",			"ElectricShell"			},
	{	"SAM4A0",         	"NitroShell"			},
	{	"SAM5A0",			"SlugShell"				},
	
	{	"GAM1A0",			"A40MMSonicGrenade"		},
	{	"GAM2A0",			"A40MMHEGrenade"		}
};

ammo_info_T SpecialAmmoInfo[MAX_SPECIAL_AMMOS_FOR_SHOP] = {
	{ 	40,			8		},
	{ 	40,			8		},
	{ 	40,			8		},
	{   40,			8		},
	{ 	40,			8		},
	
	{ 	25,			5		},
	{ 	25,			5		}
};

pdmg_T DamageValues[MAX_SPECIAL_AMMOS][MAX_SPECIALAMMO_DAMAGEINDEX] = {
	{ { 18, 0, 0 }, { 6, 0, 0 }, { 3, 0, 0 } },
	{ { 30, 0, 0 }, { -1, -1, -1 }, { -1, -1, -1 } },
	{ { 20, 0, 0 }, { 10, 0, 0 }, { -1, -1, -1 } },
	{ { 2, 0, 0 }, { -1, -1, -1 }, { -1, -1, -1 } },
	{ { 300, 0, 0 }, { -1, -1, -1 }, { -1, -1, -1 } },
	
	{ { 128, 0, 0 }, { 80, 0, 0 }, { -1, -1, -1 } }, // we don't ignore grenade here
	{ { 144, 0, 0 }, { -1, -1, -1 }, { -1, -1, -1 } },
	{ { 288, 0, 0 }, { 1, 8, 16 }, { 10, 0, 0 } }
};

int GetSpecialAmmoDamage(int id1, int id2) {
	int dmg = DamageValues[id1][id2].dmg;
	if(DamageValues[id1][id2].dmg_low && DamageValues[id1][id2].dmg_high)
		return dmg * random(DamageValues[id1][id2].dmg_low, DamageValues[id1][id2].dmg_high);
	return dmg;
}

enum {
	SPWEP_SG,
	SPWEP_SSG,
	SPWEP_MG,
	SPWEP_GL,
	SPWEP_SLOT6
};

// add whatever new ammo comes to below the last!!! Important!
enum {
	AMMO_BASICSHELL,
	AMMO_FLECHETTE,
	AMMO_PIERCING,
	AMMO_ELECTRIC,
	
	AMMO_BULLET,
	AMMO_BASICGRENADE,
	AMMO_40MMSONIC,
	AMMO_40MMHEGRENADE,
    
    AMMO_RIOTSHELL,
    AMMO_NITROGENSHELL,
    AMMO_EXPLOSIVESHELL,
	AMMO_SLUGSHELL
};

enum {
	AMMO_TYPE_SHELL,
	AMMO_TYPE_MGGRENADE,
	AMMO_TYPE_GRENADE,
    AMMO_TYPE_SHELL2,
	AMMO_TYPE_ONLYBIGNADE,
	AMMO_TYPE_HEAVYSHELLS
};

#define SPECIALAMMO_TYPE_MAX AMMO_TYPE_HEAVYSHELLS + 1
// this is the maximum amount of special ammo types a single weapon can use
// The array below accepts the order of ammo types used for weapons, this is the accepted range of ammo types
#define SPECIALAMMO_PERWEAPON_MAX 4
int SpecialAmmoRanges[SPECIALAMMO_TYPE_MAX][SPECIALAMMO_PERWEAPON_MAX] = {
	{ AMMO_BASICSHELL, AMMO_FLECHETTE, AMMO_PIERCING, AMMO_ELECTRIC },
	{ AMMO_BULLET, AMMO_BASICGRENADE, AMMO_40MMSONIC, AMMO_40MMHEGRENADE },
	{ AMMO_BASICGRENADE, AMMO_40MMSONIC, AMMO_40MMHEGRENADE, -1 },
	{ AMMO_RIOTSHELL, AMMO_NITROGENSHELL, AMMO_EXPLOSIVESHELL, -1 },
	{ AMMO_BULLET, AMMO_40MMHEGRENADE, -1, -1 },
	{ AMMO_BASICSHELL, AMMO_FLECHETTE, AMMO_PIERCING, AMMO_SLUGSHELL }
};

#define MAXSPECIALAMMOCATEGORY 2
#define MAXSPECIALAMMOTYPES (AMMO_SLUGSHELL + 1)

// yeah these are ugly I know, but so is this special ammo system, maybe one day I'll rewrite it if I think of a clean way to do it... maybe
str GetSpecialAmmoString(int id, int which) {
	switch(id) {
		case AMMO_BASICSHELL:
		return AmmoInfo_Str[DND_AMMOSLOT_SHELL][AMMO_SHELL][which];
		case AMMO_FLECHETTE:
		return SpecialAmmoInfo_Str[SSAM_FLECHETTE][which];
		case AMMO_PIERCING:
		return SpecialAmmoInfo_Str[SSAM_MAGNUM][which];
		case AMMO_ELECTRIC:
		return SpecialAmmoInfo_Str[SSAM_SHOCK][which];
		
		case AMMO_BULLET:
		return AmmoInfo_Str[DND_AMMOSLOT_CLIP][AMMO_CLIP][which];
		case AMMO_BASICGRENADE:
		return AmmoInfo_Str[DND_AMMOSLOT_ROCKET][AMMO_GRENADE][which];
		case AMMO_40MMSONIC:
		return SpecialAmmoInfo_Str[SSAM_40MMSONIC - 1][which];
		case AMMO_40MMHEGRENADE:
		return SpecialAmmoInfo_Str[SSAM_40MMHE - 1][which];
		
		case AMMO_RIOTSHELL:
		return AmmoInfo_Str[DND_AMMOSLOT_CLIP][AMMO_RIOT][which];
		case AMMO_NITROGENSHELL:
		return SpecialAmmoInfo_Str[SSAM_NITROSHELL][which];
		case AMMO_EXPLOSIVESHELL:
		return AmmoInfo_Str[DND_AMMOSLOT_SHELL][AMMO_EXSHELL][which];
		case AMMO_SLUGSHELL:
		return SpecialAmmoInfo_Str[SSAM_SLUG][which];
	}
	return "";
}

// colorful tag display
str GetSpecialAmmoTag(int id) {
	switch(id) {
		case AMMO_BASICSHELL:
		return StrParam(l:"DND_AMMO_1_0");
		case AMMO_FLECHETTE:
		return StrParam(s:"\cd", l:"DND_AMMOSPECIAL1");
		case AMMO_PIERCING:
		return StrParam(s:"\cf", l:"DND_AMMOSPECIAL2");
		case AMMO_ELECTRIC:
		return StrParam(s:"\cv", l:"DND_AMMOSPECIAL3");
		
		case AMMO_BULLET:
		return StrParam(l:"DND_AMMO_0_0");
		case AMMO_BASICGRENADE:
		return StrParam(l:"DND_AMMO_2_5");
		case AMMO_40MMSONIC:
		return StrParam(s:"\cu", l:"DND_AMMOSPECIALG1");
		case AMMO_40MMHEGRENADE:
		return StrParam(s:"\cr", l:"DND_AMMOSPECIALG2");
		
		case AMMO_RIOTSHELL:
		return StrParam(s:"\cg", l:"DND_AMMO_0_4");
		case AMMO_NITROGENSHELL:
		return StrParam(s:"\c[E3]", l:"DND_AMMOSPECIAL4");
		case AMMO_EXPLOSIVESHELL:
		return StrParam(s:"\cu", l:"DND_AMMO_1_9");
		case AMMO_SLUGSHELL:
		return StrParam(s:"\cr", l:"DND_AMMOSPECIAL5");
	}
	return "";
}

str GetSpecialAmmoSuffix(int weptype) {
	str suffix = "";
	if(weptype == SPWEP_SG)
		suffix = "_3";
	else if(weptype == SPWEP_SSG)
		suffix = "_3X";
	else if(weptype == SPWEP_MG)
		suffix = "_4";
	else if(weptype == SPWEP_GL)
		suffix = "_5X";
	else if(weptype == SPWEP_SLOT6)
		suffix = "_6";
	return suffix;
}

bool HasAmmoOnMagazine(int wepid) {
	// all weapons that have magazines go here
	// not " "
	if(StrLen(Weapons_Data[wepid][WEAPON_AMMO2]) > 1)
		return CheckInventory(Weapons_Data[wepid][WEAPON_AMMO2]);
	return 0;
}

bool HasAmmoForSpecialMode(int mode, int ammo_category, int wepid) {
	// if I have the right special ammo, or, in order to switch back, I got the magazine amount even if I dont have the ammo
	return CheckInventory(GetSpecialAmmoString(SpecialAmmoRanges[ammo_category][mode], AMMOINFO_NAME)) || (!mode && HasAmmoOnMagazine(wepid));
}

int HasSpecialAmmoForWeapon(int ammo_category) {
	for(int i = 1; i != -1 && i < SPECIALAMMO_PERWEAPON_MAX; ++i)
		if(CheckInventory(GetSpecialAmmoString(SpecialAmmoRanges[ammo_category][i], AMMOINFO_NAME)))
			return i;
	return SpecialAmmoRanges[ammo_category][0]; // return base if not found
}

int GetSpecialAmmoMode(int ammo_category, int weptype) {
	str suffix = GetSpecialAmmoSuffix(weptype);
	return CheckInventory(StrParam(s:"SpecialAmmoMode", s:suffix));
}

// pulls the index from the range of ammos this weapon can use
int GetSpecialAmmoRangeIndex(int ammo_category, int weptype) {
	int i = 0;
	int mode = GetSpecialAmmoMode(ammo_category, weptype);
	for(; i != -1 && i < SPECIALAMMO_TYPE_MAX; ++i)
		if(mode == SpecialAmmoRanges[ammo_category][i])
			return i;
	return SpecialAmmoRanges[ammo_category][0];
}

bool CanSwitchMode(int ammo_category, int weptype) {
	int curweap = CheckInventory("DnD_WeaponID");
	int mode = GetSpecialAmmoRangeIndex(ammo_category, weptype);
	int prevmode = mode;
	int failsafe = 0;
	// while can cycle through on next, if no ammo keep searching and take mod
	do {
		mode = (mode + 1) % SPECIALAMMO_PERWEAPON_MAX;
		++failsafe;
		if(failsafe == SPECIALAMMO_PERWEAPON_MAX)
			return false;
		if(SpecialAmmoRanges[ammo_category][mode] == -1)
			continue;
	} while(!HasAmmoForSpecialMode(mode, ammo_category, curweap));
	return prevmode != mode;
}

// if this runs when player has no ammo, big trouble. Make sure decorate of weapons dont allow it!
void SetSpecialAmmoMode(int ammo_category, int weptype) {
	int curweap = CheckInventory("DnD_WeaponID");
	int mode = GetSpecialAmmoRangeIndex(ammo_category, weptype);
	int failsafe = 0;
	// while can cycle through on next, if no ammo keep searching and take mod
	do {
		mode = (mode + 1) % SPECIALAMMO_PERWEAPON_MAX;
		++failsafe;
		if(failsafe == SPECIALAMMO_PERWEAPON_MAX)
			break;
		if(SpecialAmmoRanges[ammo_category][mode] == -1)
			continue;
	} while(!HasAmmoForSpecialMode(mode, ammo_category, curweap));
	if(SpecialAmmoRanges[ammo_category][mode] == -1)
		mode = 0;
	str suffix = GetSpecialAmmoSuffix(weptype);
	SetInventory(StrParam(s:"SpecialAmmoMode", s:suffix), SpecialAmmoRanges[ammo_category][mode]);
	SetInventory("AmmoChangeMessage", SpecialAmmoRanges[ammo_category][mode]);
}

#endif
