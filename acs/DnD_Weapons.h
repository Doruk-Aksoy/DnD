#ifndef DND_WEAPONS_IN
#define DND_WEAPONS_IN

#include "DnD_WeaponDefs.h"

#define MAX_WEAPON_STRINGS (WEAPON_ICON + 1)
str Weapons_Data[MAXWEPS][MAX_WEAPON_STRINGS] = { 		     
	 { " Fists ",							" ",				" ",							"WEPICO1"	},
	 { " Chainsaw ",						" ",				" ",							"WEPICO2"	},
	 
	 { "Upgraded Chainsaw",					" ",				" ",							"WEPICO3"	},
	 { "Katana",							" ",				" ",							"WEPICO4"	},
	 { "Excalibat",							"BatCharge",		"Souls",						"WEPICO5"	},
	 { "ResMelee1",							"Souls",			" ",							"WEPICO6"	},
	 { "Dusk Blade",						"SwordHitCharge",	"Souls",						"WEPICO7"	},
     { "Sickle",							" ",				" ",							"WEPICO8"	},
	 
	 { " Pistol ",							"Clip",				" ",							"WEPICO9"	},
	 { " Akimbo Pistols ",					"Clip",				" ",							"WEPICO10"	},
	 { "Magnum",							"Clip",				"BulletSize_6",			        "WEPICO11"	},
	 { "Laser Pistol",						"LPistolCharge",	" ",							"WEPICO12"	},
	 { "ResPistol1",						"Clip",				"MGClip5",					    "WEPICO13"	},
	 { "ResPistol2",						"ViperAmmo",		" ",							"WEPICO14"	},
	 
	 { "RubyWand",							"RubyAmmo",			" ",							"WEPICO15"	},
	 { "ScatterGun",						"Clip",				" ",							"WEPICO16"	},
	 
	 { " Shotgun ",							"Shell",			"ShellSize_8N",			        "WEPICO17"	},
	 { "Upgraded Shotgun",					"Shell",			"ShellSize_8",			    	"WEPICO18"	},
	 { "Upgraded Shotgun2",					"Shell",			"ShellSize_10",			        "WEPICO19"	},
	 { "Upgraded Shotgun3",					"EmeraldMana",		" ",							"WEPICO20"	},
	 { "ResShotgun1",						"Shell",			"ShellSize_12",			        "WEPICO21"	},
	 { "ResShotgun2",                       "NitrogenCanister", " ",                            "WEPICO22"	},
	 { "ResShotgun3",						"DemonBlood",		" ",							"WEPICO23"	},
	 { "ResShotgun4",						"CharonAmmo",		"CharonStacks",					"WEPICO93"	},
	 
	 { " Super Shotgun ",					"Shell",			" ",							"WEPICO24"	},
	 { "Upgraded Super Shotgun",			"Shell",			" ",							"WEPICO25"	},
	 { "Upgraded Super Shotgun2",		    "Shell",			"ShellSize_2",			    	"WEPICO26"	},
	 { "Upgraded Super Shotgun3",		    "HellsMawAmmo",		" ",			    			"WEPICO27"	},
	 { "ResSSG1",							"PCanAmmo",			"PCanClip",				    	"WEPICO28"	},
     { "ResSSG2",							"Cell",				"IonOverheat",					"WEPICO29"	},
	 { "ResSSG3",							"HadesAmmo",		"ShellSize_16",					"WEPICO30"	},
	 { "ResSSG4",							"FlayerAmmo",		" ",							"WEPICO90"  },
	 
	 { "Silver Gun",						"ExplodingShell",	" ",							"WEPICO31"	},
	 { "Slayer",							"SlayerAmmo",		" ",							"WEPICO32"	},
	 
	 { " Machine gun ", 					"Clip",				"MGClip",				    	"WEPICO33"	},
	 { "Upgraded Machine Gun",			    "Clip",				"MGClip2",				    	"WEPICO34"	},
	 { "Upgraded Machine Gun2",			    "Clip",				"MGClip3",				    	"WEPICO35"	},
	 { "Upgraded Machine Gun3",			    "DsealAmmo",		" ",				    		"WEPICO36"	},
	 { "ResMG1",							"Clip",				"MGClip4",				    	"WEPICO37"	},
	 { "ResMG2",                            "RiotgunShell",     "RiotgunClip",                  "WEPICO38"	},
     { "ResMG3",							"AcidAmmo",			"AcidClip",						"WEPICO39"	},
     { "ResMG4",							"FusionCell",		"MGClip6",						"WEPICO40"	},
	 { "ResMG5",							"IncineratorAmmo",	"IncineratorStacks",			"WEPICO94"	},
	 
	 { "Desolator",							"DesolatorAmmo",	"DesolatorOverheat",			"WEPICO41"	},
	 { " Minigun ",							"Clip",				" ",							"WEPICO42"	},
	 { "Ebony Cannon",						"EbonyAmmo",		"EbonyAmmoX",			        "WEPICO43"	},
	 { "MPPB",								"Cell",				" ",							"WEPICO95"	},
	 
	 { "Rocket Launcher",					"RocketAmmo",		" ",							"WEPICO44"	},
	 { "Upgraded Rocket Launcher",		    "RocketAmmo",		" ",							"WEPICO45"	},
	 { "Grenade Launcher",					"Grenades",			" ",							"WEPICO51"	},
	 { "Upgraded Rocket Launcher3",			"FlakShell",		"ShellSize_18",					"WEPICO47"	},
	 { "ResRL1",							"MeteorAmmo",		" ",							"WEPICO48"	},
	 { "ResRL2",							"HeavyGrenades",	"HeavyGLCounter",				"WEPICO49"	},
	 { "ResRL3",							"EverIce",			"FreezerOverheat",				"WEPICO50"	},
	 { "ResRL4",							"GravdisAmmo",		" ",							"WEPICO91"	},
	 { "ResRL5",							"Cell",				" ",							"WEPICO96"	},
	 
	 { "Upgraded Rocket Launcher2",		    "RocketAmmo",		" ",							"WEPICO46"	},
	 { "Upgraded Grenade Launcher",		    "Grenades",			" ",							"WEPICO52"	},
	 { "Heavy Missile Launcher",			"MISAmmo",			" ",							"WEPICO53"	},
	 { "Sedrin Staff",						"SedrinAmmo",		" ",							"WEPICO92"	},
	 
	 { "Plasma Rifle",						"Cell",				" ",							"WEPICO54"	},
	 { "Upgraded Plasma Rifle",				"Cell",				"PlasmaOverheat",		        "WEPICO55"	},
	 { "Upgraded Plasma Rifle2",			"Cell",				" ",							"WEPICO56"	},
	 { "Upgraded Plasma Rifle3",			"EverIce",			" ",							"WEPICO57"	},
	 { "ResPlasma1",						"Fuel",				"FuelClip",					    "WEPICO58"	},
	 { "ResPlasma2",                        "LightningCell",    "LightningStacks",              "WEPICO59"	},
	 { "ResPlasma3",						"Cell",				"RebounderOverheat",			"WEPICO60"	},
	 { "ResPlasma4",						"DemonBlood",		"LanceStacks",					"WEPICO97"	},
	 
	 { "RhinoRifle",						"Clip",				"MGClip7",						"WEPICO61"	},
	 { "Nailgun",							"NailgunAmmo",		" ",							"WEPICO62"	},
	 { "Basilisk",							"BasiliskAmmo",		"LoadedBasilisk",			    "WEPICO63"	},
	 
	 { "BFG 9000",							"Cell",				" ",							"WEPICO64"	},
	 { "Upgraded BFG 9000",					"Cell",				" ",							"WEPICO65"	},
	 { "Devastator",						"DevastatorAmmo",	" ",							"WEPICO66"	},
	 { "MFG",								"Cell",				" ",							"WEPICO67"	},
	 { "ResBFG1",							"IonAmmo",			"IonOverHeat",			        "WEPICO68"	},
	 { "ResBFG2",							"ThunderAmmo",		"ThunderRangeCounter",			"WEPICO69"	},
	 
	 { "Gauss Rifle",						"GaussRound",		" ",							"WEPICO70"	},
	 { "Rail gun",							"Cell",				" ",							"WEPICO71"	},
	 { "Death Ray",							"IonAmmo",			"DeathrayOverheat",				"WEPICO88"	},
	 
	 { "Death Staff",						"Souls",			"Souls",						"WEPICO72"	},
	 { "Razorfang",						    "Souls",			"Souls",						"WEPICO73"	},
	 { "Sun Staff",							"Souls",			"Souls",						"WEPICO74"	},
	 { "Soul Reaver",						"Souls",			" ",							"WEPICO75"	},
	 
	 { "Sawedoff",							"SawedoffShell",	"SawedoffCounter",		        "WEPICO76"	},
	 { "Soul Render",						"BladeHits",		"BladeCharge",				    "WEPICO77"	},
	 { "SMG",								"SMGAmmo",			" ",							"WEPICO78"	},
	 { "Hellforge Cannon",				    "IronBalls",		"IronBalls",					"WEPICO79"	},
	 { "Bloodfiend Spine",				    "BloodAmmo",		"BloodAmmo",			        "WEPICO80"	},
	 { "Enforcer Rifle",					"LaserAmmo",		"LaserAmmo",			        "WEPICO81"	},
	 { "Venom",								"VenomAmmo",		"VenomAmmo",			        "WEPICO82"	},
	 { "Demon Heart",						"HeartAmmo",		" ",							"WEPICO83"	},
	 { "DarkServantGloves",                 "DarkServantEnergy"," ",                            "WEPICO84"	},
	 { "Nailgun2",                          "BigNail",          " ",                            "WEPICO85"	},
	 { "Berettas",                          "BerettaAmmo",      " ",                            "WEPICO86"	},
	 { "Plasma Bolter",						"PlasmaBolterAmmo",	" ",							"WEPICO87"	},
	 { "Ripper Cannon",						"RipperCannonAmmo",	" ",							"WEPICO89"	},
};

// Supporting 8 such properties. See: RPGMENUBACKGROUNDID for increasing this
enum {
	WPROP_NONE,
	WPROP_CANTHITGHOST = 1,
	WPROP_SELFDMG = 2,
	WPROP_OVERHEAT = 4,
	WPROP_IGNORESHIELD = 8,
	WPROP_ALTAMMO = 16,
	WPROP_RIPPER = 32,
	WPROP_IRREDUCIBLE = 64,
	WPROP_NOREFLECT = 128,
	WPROP_POISON = 256
};
#define MAX_WEAPON_PROPERTIES 9

str WeaponPropertyImages[MAX_WEAPON_PROPERTIES] = {
	"WNOGHST",
	"WSLFDMG",
	"WOHEAT",
	"WOSHLD",
	"WOALT",
	"WORIP",
	"WOFULL",
	"WNOREFL",
	"WOPOIS"
};

// holds can hit ghost etc. kind of information
int WeaponProperties[MAXWEPS] = {
	// fist & csaw
	WPROP_NONE,
	WPROP_CANTHITGHOST,

	// 1
	WPROP_CANTHITGHOST,
	WPROP_NONE,
	WPROP_CANTHITGHOST | WPROP_IGNORESHIELD,
	WPROP_CANTHITGHOST,
	WPROP_NONE,
	WPROP_IRREDUCIBLE,
	
	// 2
	WPROP_NONE,
	WPROP_NONE,
	WPROP_IGNORESHIELD,
	WPROP_IGNORESHIELD,
	WPROP_NONE,
	WPROP_IGNORESHIELD | WPROP_POISON,
	WPROP_CANTHITGHOST,
	WPROP_IGNORESHIELD | WPROP_IRREDUCIBLE,
	
	// 3 - 1
	WPROP_NONE,
	WPROP_ALTAMMO,
	WPROP_NONE,
	WPROP_CANTHITGHOST | WPROP_POISON,
	WPROP_ALTAMMO,
	WPROP_NONE,
	WPROP_NONE,
	WPROP_IGNORESHIELD,
	WPROP_RIPPER,
	
	// 3 - 2
	WPROP_NONE,
	WPROP_NONE,
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_IGNORESHIELD | WPROP_NOREFLECT,
	WPROP_IGNORESHIELD | WPROP_OVERHEAT,
	WPROP_RIPPER | WPROP_CANTHITGHOST,
	WPROP_RIPPER | WPROP_NOREFLECT,
	WPROP_SELFDMG | WPROP_IGNORESHIELD,
	WPROP_SELFDMG | WPROP_CANTHITGHOST | WPROP_RIPPER,
	
	// 4 - 1
	WPROP_NONE,
	WPROP_NONE,
	WPROP_NONE,
	WPROP_NONE,
	WPROP_ALTAMMO,
	WPROP_ALTAMMO,
	WPROP_POISON,
	WPROP_IGNORESHIELD | WPROP_RIPPER,
	WPROP_IGNORESHIELD | WPROP_RIPPER | WPROP_SELFDMG | WPROP_NOREFLECT,
	
	// 4 - 2
	WPROP_OVERHEAT | WPROP_POISON,
	WPROP_CANTHITGHOST,
	WPROP_IGNORESHIELD | WPROP_SELFDMG,
	WPROP_RIPPER | WPROP_NOREFLECT | WPROP_CANTHITGHOST,
	
	// 5 - 1
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_NOREFLECT,
	WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_ALTAMMO,
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_IGNORESHIELD | WPROP_SELFDMG,
	WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_RIPPER,
	WPROP_RIPPER | WPROP_OVERHEAT,
	WPROP_IGNORESHIELD | WPROP_NOREFLECT,
	WPROP_RIPPER | WPROP_IGNORESHIELD | WPROP_NOREFLECT,
	
	// 5 - 2
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_SELFDMG | WPROP_POISON,
	
	// 6 - 1
	WPROP_NONE,
	WPROP_IGNORESHIELD | WPROP_OVERHEAT | WPROP_SELFDMG,
	WPROP_RIPPER,
	WPROP_RIPPER | WPROP_CANTHITGHOST,
	WPROP_SELFDMG,
	WPROP_NONE,
	WPROP_OVERHEAT | WPROP_NOREFLECT,
	WPROP_NOREFLECT | WPROP_IGNORESHIELD,
	
	// 6 - 2
	WPROP_ALTAMMO,
	WPROP_RIPPER | WPROP_CANTHITGHOST | WPROP_NOREFLECT,
	WPROP_IGNORESHIELD,
	
	// 7
	WPROP_IGNORESHIELD | WPROP_NOREFLECT,
	WPROP_IGNORESHIELD | WPROP_NOREFLECT,
	WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_CANTHITGHOST,
	WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_NOREFLECT,
	WPROP_OVERHEAT | WPROP_IGNORESHIELD | WPROP_CANTHITGHOST,
	WPROP_SELFDMG | WPROP_IGNORESHIELD,
	
	WPROP_SELFDMG | WPROP_IGNORESHIELD,
	WPROP_IGNORESHIELD | WPROP_RIPPER,
	WPROP_IGNORESHIELD | WPROP_OVERHEAT,
	
	// 8
	WPROP_IGNORESHIELD | WPROP_SELFDMG,
	WPROP_RIPPER,
	WPROP_IGNORESHIELD | WPROP_RIPPER,
	WPROP_IGNORESHIELD | WPROP_RIPPER,
	
	// temps
	WPROP_NONE,
	WPROP_RIPPER | WPROP_IGNORESHIELD,
	WPROP_NONE,
	WPROP_RIPPER | WPROP_IGNORESHIELD,
	WPROP_CANTHITGHOST,
	WPROP_IGNORESHIELD,
	WPROP_RIPPER | WPROP_CANTHITGHOST,
	WPROP_IGNORESHIELD,
	WPROP_NONE,
	WPROP_RIPPER,
	WPROP_NONE,
	WPROP_RIPPER | WPROP_NOREFLECT,
	WPROP_RIPPER
};

// put them in this place if they are luxury weapons
bool IsLuxuryWeapon(int id) {
	switch(id) {
		case DND_WEAPON_DUSKBLADE:
		case DND_WEAPON_SICKLE:
		
		case DND_WEAPON_RUBYWAND:
		case DND_WEAPON_SCATTERGUN:
		
		case DND_WEAPON_SILVERGUN:
		case DND_WEAPON_SLAYER:
		
		case DND_WEAPON_DESOLATOR:
		case DND_WEAPON_MINIGUN:
		case DND_WEAPON_EBONYCANNON:
		case DND_WEAPON_MPPB:
		
		case DND_WEAPON_ROTARYGRENADELAUNCHER:
		case DND_WEAPON_HEAVYMISSILELAUNCHER:
		case DND_WEAPON_SEDRINSTAFF:
			
		case DND_WEAPON_RHINORIFLE:
		case DND_WEAPON_NAILGUN:
		case DND_WEAPON_BASILISK:
			
		case DND_WEAPON_GAUSSRIFLE:
		case DND_WEAPON_RAILGUN:
		case DND_WEAPON_DEATHRAY:
			
		// these shouldnt be luxury weapons, there are no regular weapons for this slot anyway!
		/*case DND_WEAPON_DEATHSTAFF:
		case DND_WEAPON_RAZORFANG:
		case DND_WEAPON_SUNSTAFF:
		case DND_WEAPON_SOULREAVER:*/
		return true;
	}
	return false;
}

bool IsSoulWeapon(int wepid) {
	switch(wepid) {
		case DND_WEAPON_DEATHSTAFF:
		case DND_WEAPON_RAZORFANG:
		case DND_WEAPON_SUNSTAFF:
		case DND_WEAPON_SOULREAVER:
		case DND_WEAPON_DUSKBLADE:
		case DND_WEAPON_INFERNOSWORD:
		return true;
	}
	return false;
}

bool IsSuperWeapon(int wepid) {
	return wepid >= FIRST_SLOT7_WEAPON && wepid <= LAST_SLOT8_WEAPON;
}

#define WEPCHECK_SLOT1 "P_Slot1Replaced"
#define WEPCHECK_SLOT1L "P_Slot1Luxury"
#define WEPCHECK_SLOT2 "P_Slot2Replaced"
#define WEPCHECK_SLOT2L "P_Slot2Luxury"
#define WEPCHECK_SLOT3 "P_Slot3Replaced"
#define WEPCHECK_SLOT3X "P_Slot3XReplaced"
#define WEPCHECK_SLOT3L "P_Slot3Luxury"
#define WEPCHECK_SLOT4 "P_Slot4Replaced"
#define WEPCHECK_SLOT4L "P_Slot4Luxury"
#define WEPCHECK_SLOT5 "P_Slot5Replaced"
#define WEPCHECK_SLOT5L "P_Slot5Luxury"
#define WEPCHECK_SLOT6 "P_Slot6Replaced"
#define WEPCHECK_SLOT6L "P_Slot6Luxury"
#define WEPCHECK_SLOT7 "P_Slot7Replaced"
#define WEPCHECK_SLOT7L "P_Slot7Luxury"
#define WEPCHECK_SLOT8 "P_Slot8Luxury"	// this says luxury here but theres really no point, they all share the same slot regardless and simplifies checking algorithm

#define MAX_WEPCONDITION_CHECKERS 16
str WeaponConditionCheckers[MAX_WEPCONDITION_CHECKERS] = {
	WEPCHECK_SLOT1,
	WEPCHECK_SLOT1L,
	WEPCHECK_SLOT2,
	WEPCHECK_SLOT2L,
	WEPCHECK_SLOT3,
	WEPCHECK_SLOT3X,
	WEPCHECK_SLOT3L,
	WEPCHECK_SLOT4,
	WEPCHECK_SLOT4L,
	WEPCHECK_SLOT5,
	WEPCHECK_SLOT5L,
	WEPCHECK_SLOT6,
	WEPCHECK_SLOT6L,
	WEPCHECK_SLOT7,
	WEPCHECK_SLOT7L,
	WEPCHECK_SLOT8
};

enum {
	DND_OVERHEAT_SHOCKER,
	DND_OVERHEAT_FREEZER,
	DND_OVERHEAT_NUCLEARPR,
	DND_OVERHEAT_ION,
	DND_OVERHEAT_REBOUNDER,
	DND_OVERHEAT_DESOLATOR,
	DND_OVERHEAT_DEATHRAY
};

#define MAXOVERHEATWEPS DND_OVERHEAT_DEATHRAY + 1
str WeaponOverheatItems[MAXOVERHEATWEPS] = {
	"ShockerOverheat",
	"FreezerOverheat",
	"PlasmaOverheat",
	"IonOverheat",
	"RebounderOverheat",
	"DesolatorOverheat",
	"DeathRayOverheat"
};

// 32 overheat weapons supported
int PlayerRunsOverheat[MAXPLAYERS] = { 0 };

void ResetWeaponMods(int pnum) {
	int i, j;
	for(j = 0; j < MAXWEPS; ++j) {
		Player_Weapon_Infos[pnum][j].quality = 0;
		for(i = 0; i < MAX_WEP_MODS; ++i) {
			Player_Weapon_Infos[pnum][j].wep_mods[i].tier = 0;
			Player_Weapon_Infos[pnum][j].wep_mods[i].val = 0;
		}
	}
}

// This basically checks eligibility of weapon pickups if we have that slot's weapon or not
bool CheckSlotWeapon(int slot) {
	int beg = 0, end = 0;
	
	switch(slot) {
		case 0:
			beg = DND_WEAPON_CHAINSAW;
			end = LAST_SLOT0_WEAPON;
		break;
		case 1:
			beg = FIRST_SLOT1_WEAPON;
			end = LAST_SLOT1_WEAPON;
		break;
		case 2:
			beg = FIRST_SLOT2_WEAPON;
			end = LAST_SLOT2_WEAPON;
		break;
		case 3:
			beg = FIRST_SLOT3_WEAPON;
			end = LAST_SLOT3_WEAPON;
		break;
		case 4:
			beg = FIRST_SLOT4_WEAPON;
			end = LAST_SLOT4_WEAPON;
		break;
		case 5:
			beg = FIRST_SLOT5_WEAPON;
			end = LAST_SLOT5_WEAPON;
		break;
		case 6:
			beg = FIRST_SLOT6_WEAPON;
			end = LAST_SLOT6_WEAPON;
		break;
		case 7:
			beg = FIRST_SLOT7_WEAPON;
			end = LAST_SLOT7_WEAPON;
		break;
	}
	
	for(int i = beg; i <= end; ++i) {
		if(IsLuxuryWeapon(i))
			return false;
		if(CheckInventory(Weapons_Data[i][WEAPON_NAME]))
			return true;
	}
	return false;
}

int GetWeaponPosFromTable() {
	return CheckInventory("DnD_WeaponID");
}

int GetActorWeaponID(int actor) {
	return CheckActorInventory(actor, "DnD_WeaponID");
}

int PickRandomOwnedWeaponID(void) {
	int count = 0, i, pnum = PlayerNumber();
	static int owned_weapons[MAXPLAYERS][MAXWEPS];
	// setup the list
	for(i = 0; i < MAXWEPS; ++i) {
		if(CheckInventory(Weapons_Data[i][WEAPON_NAME])) {
			owned_weapons[pnum][count] = i;
			++count;
		}
	}
	// pick from random weapons owned
	return owned_weapons[pnum][random(0, count - 1)];
}

str GetWeaponAmmoType(int wepid, int which) {
	if(!which)
		return Weapons_Data[wepid][WEAPON_AMMO1];
	return Weapons_Data[wepid][WEAPON_AMMO2];
}

bool IsBoomstick(int id) {
	switch(id) {
		case DND_WEAPON_SHOTGUN:
		case DND_WEAPON_PURIFIER:
		case DND_WEAPON_KILLSTORM:
		case DND_WEAPON_DEADLOCK:
		case DND_WEAPON_SUPERSHOTGUN:
		case DND_WEAPON_HEAVYSUPERSHOTGUN:
		case DND_WEAPON_ERASUS:
		case DND_WEAPON_SHOCKER:
		case DND_WEAPON_HADES:
		case DND_WEAPON_SILVERGUN:
		case DND_WEAPON_INCINERATOR:
		case DND_WEAPON_VINDICATOR:
		case DND_WEAPON_SAWEDOFF:
		return true;
	}
	return false;
}

int CountShotgunWeaponsOwned() {
	// optimized to not loop over all weapons, but newer shotguns must be added here
	return 	CheckInventory(Weapons_Data[DND_WEAPON_SHOTGUN][WEAPON_NAME]) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_PURIFIER][WEAPON_NAME]) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_KILLSTORM][WEAPON_NAME]) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_DEADLOCK][WEAPON_NAME]) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_SUPERSHOTGUN][WEAPON_NAME]) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN][WEAPON_NAME]) 		+
			CheckInventory(Weapons_Data[DND_WEAPON_ERASUS][WEAPON_NAME]) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_SHOCKER][WEAPON_NAME]) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_HADES][WEAPON_NAME]) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_SILVERGUN][WEAPON_NAME]) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_INCINERATOR][WEAPON_NAME]) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_VINDICATOR][WEAPON_NAME]) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_SAWEDOFF][WEAPON_NAME]);
}

bool IsTemporaryWeapon(int id) {
	return id >= DND_WEAPON_SAWEDOFF;
}

int GetWeaponCount() {
	int res = 0;
	for(int i = 0; i < MAXWEPS; ++i)
		if(CheckInventory(Weapons_Data[i][WEAPON_NAME]))
			++res;
	return res;
}

int MapClassicWeaponToTableID(int id) {
	int ret = DND_WEAPON_FIST;
	switch (id) {
		case 0:
			ret = DND_WEAPON_CHAINSAW;
		break;
		case 1:
			ret = DND_WEAPON_SHOTGUN;
		break;
		case 2:
			ret = DND_WEAPON_SUPERSHOTGUN;
		break;
		case 3:
			ret = DND_WEAPON_MACHINEGUN;
		break;
		case 4:
			ret = DND_WEAPON_ROCKETLAUNCHER;
		break;
		case 5:
			ret = DND_WEAPON_PLASMARIFLE;
		break;
		case 6:
			ret = DND_WEAPON_BFG6000;
		break;
	}
	return ret;
}

bool IsWeaponLightningType(int wepid, int extra, bool isSpecial) {
	switch(wepid) {
		case DND_WEAPON_SHOCKER:
		case DND_WEAPON_LIGHTNINGGUN:
		case DND_WEAPON_THUNDERSTAFF:
		return true;
	}
	return isSpecial && extra == SSAM_SHOCK;
}

bool IsMeleeWeapon(int wepid) {
	switch(wepid) {
		case DND_WEAPON_FIST:
		case DND_WEAPON_CHAINSAW:
		case DND_WEAPON_DOUBLECHAINSAW:
		case DND_WEAPON_KATANA:
		case DND_WEAPON_EXCALIBAT:
		case DND_WEAPON_INFERNOSWORD:
		case DND_WEAPON_DUSKBLADE:
		case DND_WEAPON_SICKLE:
		case DND_WEAPON_SOULRENDER:
		return true;
	}
	return false;
}

void DoWeaponTip(int curweap) {
	if(!CheckInventory("TipBoxOpen")) {
		ACS_ExecuteAlways(977, 0, 0, curweap);
		GiveInventory("TipBoxOpen", 1);
	}
}

// This needs a better name for real, but it returns indexing of slot for tabular format, not in-game slot of weapons
// this will never change so I dont want to waste an array spot for this
int GetSlotOfWeapon(int wepid) {
	if(wepid <= LAST_SLOT0_WEAPON)
		return 0;
	if(wepid <= LAST_SLOT1_WEAPON)
		return 1;
	if(wepid <= LAST_SLOT2_WEAPON)
		return 2;
	if(wepid <= LAST_SLOT3_WEAPON)
		return 3;
	if(wepid <= LAST_SLOT4_WEAPON)
		return 4;
	if(wepid <= LAST_SLOT5_WEAPON)
		return 5;
	if(wepid <= LAST_SLOT6_WEAPON)
		return 6;
	if(wepid <= LAST_SLOT7_WEAPON)
		return 7;
	if(wepid <= LAST_SLOT8_WEAPON)
		return 8;
	return 9;
}

// This gets the in-game slot of the weapon
int GetGameSlotOfWeapon(int wepid) {
	int slot = GetSlotOfWeapon(wepid);
	// 0 is melee, 1 is pistols, 2 is shotguns, 3 is super shotguns and 3 and above is fine, we need to add +1 to things below 2
	if(slot <= 2)
		return slot + 1;
	return slot;
}

// Retrieves a weapon's order within the slot cluster
int GetWeaponOrderSlot(int wepid, int slot) {
	switch(slot) {
		case 0:
		return wepid - FIRST_SLOT0_WEAPON;
		case 1:
		return wepid - FIRST_SLOT1_WEAPON;
		case 2:
		return wepid - FIRST_SLOT2_WEAPON;
		case 3:
		return wepid - FIRST_SLOT3_WEAPON;
		case 4:
		return wepid - FIRST_SLOT4_WEAPON;
		case 5:
		return wepid - FIRST_SLOT5_WEAPON;
		case 6:
		return wepid - FIRST_SLOT6_WEAPON;
		case 7:
		return wepid - FIRST_SLOT7_WEAPON;
		case 8:
		return wepid - FIRST_SLOT8_WEAPON;
		case 9:
		return wepid - FIRST_SLOT9_WEAPON;
	}
	return 0;
}

str GetWeaponTipText(int wepid) {
	int slot = GetSlotOfWeapon(wepid);
	int subid = GetWeaponOrderSlot(wepid, slot);
	return StrParam(s:"WEPTIP_", d:slot, s:"_", d:subid);
}

str GetWeaponTag(int wepid) {
	return StrParam(s:"WEP_", d:wepid, s:"_TAG");
}

#endif
