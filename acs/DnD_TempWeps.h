#ifndef DND_TEMPWEAPONS_IN
#define DND_TEMPWEAPONS_IN

#define TEMPBEGIN DND_WEAPON_SAWEDOFF
// Counted off LAST_SLOT9_WEAPON, not MAXWEPS. MAXWEPS runs past the slot 9 range -- ASCENSION
// and ADMINPISTOL sit above RIPPERCANNON and are not temporary weapons -- and the old form
// added a spurious +1 on top of that, since MAXWEPS is already last+1. The result was 16 for
// a 13 row TemporaryWeaponData, so every loop over this ran three rows past the real data.
//
// 13 is what the table holds and what the discard preference cycle in DnD_Activity.h already
// assumes (its modulus is LAST_SLOT9 - FIRST + 3), so this brings the last of the three into
// agreement with the other two.
#define MAXTEMPWEPS (LAST_SLOT9_WEAPON - TEMPBEGIN + 1)
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
#define TEMPWEP_AMMO 2
#define TEMPWEP_TAG 3
str TemporaryWeaponData[MAXTEMPWEPS][4] = { 
	{ 
		"Sawedoff", 
		"SawedoffPickup", 
		"SawedoffShell", 
		"Sawedoff" 
	},
	{ 
		"Soul Render", 
		"SoulRenderPickup", 
		"BladeHits", 
		"Soul Render" 
	},
	{ 
		"SMG", 
		"SMGPickup", 
		"SMGAmmo", 
		"Submachine Gun" 
	},
	{ 
		"Hellforge Cannon", 
		"HellforgePickup", 
		"IronBalls", 
		"Hellforge Cannon" 
	},
	{ 
		"Bloodfiend Spine", 
		"SpinePickup", 
		"BloodAmmo", 
		"Bloodfiend Spine" 
	},
	{ 
		"Enforcer Rifle", 
		"LaserPickup", 
		"LaserAmmo", 
		"Enforcer Laser Rifle" 
	},
	{ 
		"Venom", 
		"VenomPickup", 
		"VenomAmmo", 
		"Venom" 
	},
	{ 
		"Demon Heart", 
		"DemonHeartPickup", 
		"HeartAmmo", 
		"Demon Heart" 
	},
	{ 
		"DarkServantGloves", 
		"DarkServantGlovesPickup", 
		"DarkServantEnergy", 
		"Dark Servant Gloves" 
	},
	{ 
		"Nailgun2", 
		"Nailgun2Pickup", 
		"BigNail", 
		"Heavy Nailgun" 
	},
	{ 
		"Berettas", 
		"BerettasPickup", 
		"BerettaAmmo", 
		"Akimbo Berettas"
	},
	{ 
		"Plasma Bolter", 
		"PlasmaBolterPickup", 
		"PlasmaBolterAmmo", 
		"Plasma Bolter" 
	},
	{ 
		"Ripper Cannon", 
		"RipperCannonPickup", 
		"RipperCannonAmmo", 
		"Ripper Cannon" 
	}
};

bool HasNoTempWeapon() {
	for(int i = 0; i < MAXTEMPWEPS; ++i)
		if(!CheckInventory(TemporaryWeaponData[i][TEMPWEP_NAME]))
			return 1;
	return 0;
}

void CleanTempAmmo() {
	for(int i = 0; i < MAXTEMPWEPS; ++i) {
		SetInventory(TemporaryWeaponData[i][TEMPWEP_AMMO], 0);
		
		// exception for soul render
		SetInventory("BladeCharge", 0);
	}
}

#endif
