#ifndef DND_TEMPWEAPONS_IN
#define DND_TEMPWEAPONS_IN

#include "DnD_Weapons.h"

#define TEMPBEGIN DND_WEAPON_SAWEDOFF
#define MAXTEMPWEPS MAXWEPS - TEMPBEGIN + 1
#define UNUSEDWEPS 0 // this is remaining slots out of 2^(nearest power of 2 to maxweps) - maxweps

str TemporaryWeapons[MAXTEMPWEPS] = { 
	"Sawedoff", 
	"Soul Render", 
	"SMG", 
	"Hellforge Cannon", 
	"Bloodfiend Spine", 
	"Enforcer Rifle", 
	"Venom", 
	"Demon Heart", 
	"DarkServantGloves", 
	"Nailgun2",
	"Berettas",
	"Plasma Bolter"
};

str TemporaryWeaponDrops[MAXTEMPWEPS] = { 
	"SawedoffPickup_D", 
	"SoulRenderPickup_D", 
	"SMGPickup_D", 
	"HellforgePickup_D", 
	"SpinePickup_D", 
	"LaserPickup_D", 
	"VenomPickup_D", 
	"DemonHeartPickup_D", 
	"DarkServantGlovesPickup_D", 
	"Nailgun2Pickup_D",
	"BerettasPickup_D",
	"PlasmaBolterPickup_D"
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
	DND_TEMPAMMO_PLASMABOLTERAMMO
};

#define MAXTEMPAMMO DND_TEMPAMMO_PLASMABOLTERAMMO + 1
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
	"PlasmaBolterAmmo"
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
	"\ccWeapon Pickup : \c[Y5]Plasma Bolter - 9\c-"
};

bool HasNoTempWeapon() {
	for(int i = 0; i < MAXTEMPWEPS; ++i)
		if(!CheckInventory(TemporaryWeapons[i]))
			return 1;
	return 0;
}

void CleanTempAmmo() {
	for(int i = 0; i < MAXTEMPWEPS; ++i) {
			SetInventory(TemporaryAmmos[i], 0);
	}
}

#endif