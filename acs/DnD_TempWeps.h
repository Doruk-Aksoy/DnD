#ifndef DND_TEMPWEAPONS_IN
#define DND_TEMPWEAPONS_IN

#include "DnD_WeaponDefs.h"

#define TEMPBEGIN DND_WEAPON_SAWEDOFF
#define MAXTEMPWEPS (MAXWEPS - TEMPBEGIN + 1)
#define UNUSEDWEPS 0 // this is remaining slots out of 2^(nearest power of 2 to maxweps) - maxweps

#define TEMPWEP_BLOODFIENDSPINE_DROPCHANCE 0.6
#define TEMPWEP_VENOM_DROPCHANCE 0.5
#define TEMPWEP_NAILGUN_DROPCHANCE 0.33
#define TEMPWEP_SOULRENDER_DROPCHANCE 0.25
#define TEMPWEP_HFCANNON_DROPCHANCE 0.3
#define TEMPWEP_DARKGLOVES_DROPCHANCE 0.375

enum {
	DND_TEMPWEP_SAWEDOFF,
	DND_TEMPWEP_SOULRENDER,
	DND_TEMPWEP_SMG,
	DND_TEMPWEP_HELLFORGECANNON,
	DND_TEMPWEP_BLOODFIENDSPINE,
	DND_TEMPWEP_ENFORCERRIFLE,
	DND_TEMPWEP_VENOM,
	DND_TEMPWEP_DEMONHEART,
	DND_TEMPWEP_DARKGLOVES,
	DND_TEMPWEP_HEAVYNAILGUN,
	DND_TEMPWEP_BERETTAS,
	DND_TEMPWEP_PLASMABOLTER,
	DND_TEMPWEP_RIPPERCANNON
};

#define TEMPWEP_NAME 0
#define TEMPWEP_DROP 1
str TemporaryWeapons[MAXTEMPWEPS][2] = { 
	{ "Sawedoff", "SawedoffPickup" },
	{ "Soul Render", "SoulRenderPickup" },
	{ "SMG", "SMGPickup" },
	{ "Hellforge Cannon", "HellforgePickup" },
	{ "Bloodfiend Spine", "SpinePickup" },
	{ "Enforcer Rifle", "LaserPickup" },
	{ "Venom", "VenomPickup" },
	{ "Demon Heart", "DemonHeartPickup" },
	{ "DarkServantGloves", "DarkServantGlovesPickup" },
	{ "Nailgun2", "Nailgun2Pickup" },
	{ "Berettas", "BerettasPickup" },
	{ "Plasma Bolter", "PlasmaBolterPickup" },
	{ "Ripper Cannon", "RipperCannonPickup" }
};

enum {
	DND_TEMPAMMO_SAWEDOFFSHELL,
	DND_TEMPAMMO_BLADEHITS, 
	DND_TEMPAMMO_BLADECHARGE,
	DND_TEMPAMMO_SMGAMMO, 
	DND_TEMPAMMO_IRONBALLS, 
	DND_TEMPAMMO_BLOODAMMO, 
	DND_TEMPAMMO_LASERAMMO, 
	DND_TEMPAMMO_VENOMAMMO, 
	DND_TEMPAMMO_HEARTAMMO, 
	DND_TEMPAMMO_DARKSERVANTENERGY, 
	DND_TEMPAMMO_BIGNAIL,
	DND_TEMPAMMO_BERETTAAMMO,
	DND_TEMPAMMO_PLASMABOLTERAMMO,
	DND_TEMPAMMO_RIPPERCANNONAMMO
};

#define MAXTEMPAMMO DND_TEMPAMMO_RIPPERCANNONAMMO + 1
str TemporaryAmmos[MAXTEMPAMMO] = { 
	"SawedoffShell",
	"BladeHits", 
	"BladeCharge",
	"SMGAmmo", 
	"IronBalls", 
	"BloodAmmo", 
	"LaserAmmo", 
	"VenomAmmo", 
	"HeartAmmo", 
	"DarkServantEnergy", 
	"BigNail",
	"BerettaAmmo",
	"PlasmaBolterAmmo",
	"RipperCannonAmmo"
};

str TemporaryWeaponMsg[MAXTEMPWEPS] = { 
	"\ccWeapon Pickup : \c[Y5]Sawedoff - 9\c-",
	"\ccWeapon Pickup : \c[Y5]Soul Render - 9\c-",
	"\ccWeapon Pickup : \c[Y5]Submachine Gun - 9\c-",
	"\ccWeapon Pickup : \c[Y5]Hellforge Cannon - 9\c-",
	"\ccWeapon Pickup : \c[Y5]Bloodfiend Spine - 9\c-",
	"\ccWeapon Pickup : \c[Y5]Enforcer Laser Rifle - 9\c-",
	"\ccWeapon Pickup : \c[Y5]Venom - 9\c-",
	"\ccWeapon Pickup : \c[Y5]Demon Heart - 9\c-",
	"\ccWeapon Pickup : \c[Y5]Dark Servant Gloves - 9\c-",
	"\ccWeapon Pickup : \c[Y5]Heavy Nailgun - 9\c-",
	"\ccWeapon Pickup : \c[Y5]Akimbo Berettas - 9\c-",
	"\ccWeapon Pickup : \c[Y5]Plasma Bolter - 9\c-",
	"\ccWeapon Pickup : \c[Y5]Ripper Cannon - 9\c-"
};

bool HasNoTempWeapon() {
	for(int i = 0; i < MAXTEMPWEPS; ++i)
		if(!CheckInventory(TemporaryWeapons[i][TEMPWEP_NAME]))
			return 1;
	return 0;
}

void CleanTempAmmo() {
	for(int i = 0; i < MAXTEMPWEPS; ++i) {
			SetInventory(TemporaryAmmos[i], 0);
	}
}

#endif