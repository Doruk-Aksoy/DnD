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
	
	SSAM_40MMSONIC,
	SSAM_40MMHE
};

#define MAX_SPECIAL_AMMOS SSAM_40MMHE + 1
#define MAX_SPECIALAMMO_DAMAGEINDEX 3

str SpecialAmmoInfo_Str[MAX_SPECIAL_AMMOS][2] = {
	{	"SAM1A0",			"FlechetteShell"		},
	{	"SAM3A0",			"PiercingShell"			},
	{	"SAM2A0",			"ElectricShell"			},
	{	"SAM4A0",         	"NitroShell"			},
	{	"SAM5A0",			"SlugShell"				},
		
	{	"GAM1A0",			"A40MMSonicGrenade"		},
	{	"GAM2A0",			"A40MMHEGrenade"		}
};

ammo_info_T SpecialAmmoInfo[MAX_SPECIAL_AMMOS] = {
	{ 	40,			8		},
	{ 	40,			8		},
	{ 	40,			8		},
	{   40,			8		},
	{ 	40,			8		},
	
	{ 	25,			5		},
	{ 	25,			5		}
};

pdmg_T DamageValues[MAX_SPECIAL_AMMOS][MAX_SPECIALAMMO_DAMAGEINDEX] = {
	{ { 25, 0, 0 }, { 6, 0, 0 }, { 3, 0, 0 } },
	{ { 35, 0, 0 }, { -1, -1, -1 }, { -1, -1, -1 } },
	{ { 25, 0, 0 }, { 10, 0, 0 }, { -1, -1, -1 } },
	{ { 2, 0, 0 }, { -1, -1, -1 }, { -1, -1, -1 } },
	{ { 300, 0, 0 }, { -1, -1, -1 }, { -1, -1, -1 } },
	
	{ { 288, 0, 0 }, { 1, 8, 16 }, { 10, 0, 0 } },
	{ { 144, 0, 0 }, { -1, -1, -1 }, { -1, -1, -1 } }
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
#define MAXSPECIALAMMOTYPES AMMO_SLUGSHELL + 1
#define SPECIALAMMO_NAME 0
#define SPECIALAMMO_TAG 1
str SpecialAmmoNames[MAXSPECIALAMMOTYPES][2] = {
	{		"Shell",					"Normal Shells"						},
	{ 		"FlechetteShell",			"\cdFlechette Shells"				},
	{ 		"PiercingShell",			"\cfMagnum Shells"					},
	{		"ElectricShell",			"\cvElectric Shells"				},
	
	{		"Clip",						"Bullets"							},
	{		"Grenades",					"40mm Grenades"						},
	{		"A40mmSonicGrenade",		"\cu40mm Sonic Grenades"			},
	{		"A40mmHEGrenade",			"\cr40mm HEGrenades"				},
    
    {       "RiotgunShell",             "\cgRiot Shells"                    },
    {       "NitroShell",               "\c[E3]Nitrogen Shells"             },
    {       "ExplodingShell",           "\cuExplosive Shells"               },
	{		"SlugShell",				"\crSlug Shells"					}
};

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
	if(StrLen(Weapons[wepid][WEAPON_AMMO2]) > 1)
		return CheckInventory(Weapons[wepid][WEAPON_AMMO2]);
	return 0;
}

bool HasAmmoForSpecialMode(int mode, int ammo_category, int wepid) {
	// if I have the right special ammo, or, in order to switch back, I got the magazine amount even if I dont have the ammo
	return CheckInventory(SpecialAmmoNames[SpecialAmmoRanges[ammo_category][mode]][SPECIALAMMO_NAME]) || (!mode && HasAmmoOnMagazine(wepid));
}

int HasSpecialAmmoForWeapon(int ammo_category) {
	for(int i = 1; i != -1 && i < SPECIALAMMO_PERWEAPON_MAX; ++i)
		if(CheckInventory(SpecialAmmoNames[SpecialAmmoRanges[ammo_category][i]][SPECIALAMMO_NAME]))
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