#ifndef DND_SPECIALAMMO_IN
#define DND_SPECIALAMMO_IN

#include "DnD_DamageCache.h"
#include "DnD_Weapons.h"

enum {
	SSAM_FLECHETTE,
	SSAM_MAGNUM,
	SSAM_SHOCK,
	
	SSAM_40MMHE,
	SSAM_40MMSONIC,
    
    SSAM_NITROSHELL
};

#define MAX_SPECIAL_AMMOS SSAM_NITROSHELL + 1
#define MAX_SPECIALAMMO_DAMAGEINDEX 3
pdmg_T DamageValues[MAX_SPECIAL_AMMOS][MAX_SPECIALAMMO_DAMAGEINDEX] = {
	{ { 25, 0, 0 }, { 6, 0, 0 }, { 3, 0, 0 } },
	{ { 35, 0, 0 }, { -1, -1, -1 }, { -1, -1, -1 } },
	{ { 25, 0, 0 }, { 10, 0, 0 }, { -1, -1, -1 } },
	
	{ { 288, 0, 0 }, { 1, 8, 16 }, { 10, 0, 0 } },
	{ { 144, 0, 0 }, { -1, -1, -1 }, { -1, -1, -1 } },
	
	{ { 2, 0, 0 }, { -1, -1, -1 }, { -1, -1, -1 } }
};

int GetSpecialAmmoDamage(int id1, int id2) {
	id2 >>= 16;
	int dmg = DamageValues[id1][id2].dmg;
	if(DamageValues[id1][id2].dmg_low && DamageValues[id1][id2].dmg_high)
		return dmg * random(DamageValues[id1][id2].dmg_low, DamageValues[id1][id2].dmg_high);
	return dmg;
}

enum {
	SPWEP_SG,
	SPWEP_SSG,
	SPWEP_MG,
	SPWEP_GL
};

str SuperSpecialAmmos[MAX_SPECIAL_AMMOS] = {
	"FlechetteShell", 
	"PiercingShell",
	"ElectricShell",
	
	"A40mmSonicGrenade",
	"A40mmHEGrenade",
    
    "NitroShell"
};

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
    AMMO_EXPLOSIVESHELL
};

enum {
	AMMO_TYPE_SHELL,
	AMMO_TYPE_MGGRENADE,
	AMMO_TYPE_GRENADE,
    AMMO_TYPE_SHELL2
};

#define SPECIALAMMO_TYPE_MAX AMMO_TYPE_SHELL2 + 1
int SpecialAmmoBase[SPECIALAMMO_TYPE_MAX] = { AMMO_BASICSHELL, AMMO_BULLET, AMMO_BASICGRENADE, AMMO_RIOTSHELL };
int SpecialAmmoLimits[SPECIALAMMO_TYPE_MAX] = { AMMO_ELECTRIC, AMMO_40MMHEGRENADE, AMMO_40MMHEGRENADE, AMMO_EXPLOSIVESHELL };

#define MAXSPECIALAMMOCATEGORY 2
#define MAXSPECIALAMMOTYPES AMMO_EXPLOSIVESHELL + 1
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
    {       "ExplodingShell",           "\cuExplosive Shells"               }
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
	return CheckInventory(SpecialAmmoNames[mode][SPECIALAMMO_NAME]) || (mode == SpecialAmmoBase[ammo_category] && HasAmmoOnMagazine(wepid));
}

int HasSpecialAmmoForWeapon(int ammo_category) {
	for(int i = SpecialAmmoBase[ammo_category] + 1; i <= SpecialAmmoLimits[ammo_category]; ++i)
		if(CheckInventory(SpecialAmmoNames[i][SPECIALAMMO_NAME]))
			return i;
	return SpecialAmmoBase[ammo_category];
}

int GetSpecialAmmoMode(int ammo_category, int weptype) {
	str suffix = GetSpecialAmmoSuffix(weptype);
	return CheckInventory(StrParam(s:"SpecialAmmoMode", s:suffix));
}

bool CanSwitchMode(int ammo_category, int weptype) {
	int curweap = CheckInventory("DnD_WeaponID");
	int mode = GetSpecialAmmoMode(ammo_category, weptype);
	int prevmode = mode;
	// while can cycle through on next, if no ammo keep searching and take mod
	do {
		mode = (mode + 1) % (SpecialAmmoLimits[ammo_category] + 1);
		if(!mode)
			mode = SpecialAmmoBase[ammo_category];
	} while(!HasAmmoForSpecialMode(mode, ammo_category, curweap));
	return prevmode != mode;
}

// if this runs when player has no ammo, big trouble. Make sure decorate of weapons dont allow it!
void SetSpecialAmmoMode(int ammo_category, int weptype) {
	int curweap = CheckInventory("DnD_WeaponID");
	int mode = GetSpecialAmmoMode(ammo_category, weptype);
	// while can cycle through on next, if no ammo keep searching and take mod
	do {
		mode = (mode + 1) % (SpecialAmmoLimits[ammo_category] + 1);
		if(!mode)
			mode = SpecialAmmoBase[ammo_category];
	} while(!HasAmmoForSpecialMode(mode, ammo_category, curweap));
	
	str suffix = GetSpecialAmmoSuffix(weptype);
	SetInventory(StrParam(s:"SpecialAmmoMode", s:suffix), mode);
	SetInventory("AmmoChangeMessage", mode);
}

void DoubleSpecialAmmoCapacity() {
	for(int i = 0; i < MAX_SPECIAL_AMMOS; ++i)
		SetAmmoCapacity(SuperSpecialAmmos[i], GetAmmoCapacity(SuperSpecialAmmos[i]) * 2);
}

#endif