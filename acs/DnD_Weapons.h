#ifndef DND_WEAPONS_IN
#define DND_WEAPONS_IN

#include "DnD_WeaponDefs.h"

#define MAX_WEAPON_STRINGS WEAPON_ICON64 + 1
str Weapons[MAXWEPS][MAX_WEAPON_STRINGS] = { 		     
	 { " Fists ",							"Fist",						"1",		" ",				" ",							"StartOnly",				"FISTICO",		"WEPICO1"	},
	 { " Chainsaw ",						"Chainsaw",					"1",		" ",				" ",							"GotChainsaw",				"CSAWA0",		"WEPICO2"	},
	 
	 { "Upgraded Chainsaw",					"Dual Chainsaw",			"1",		" ",				" ",							"GotChainsaw",				"CSW2A0",		"WEPICO3"	},
	 { "Katana",							"Katana",					"1",		" ",				" ",							"GotChainsaw",				"JFISZ0",		"WEPICO4"	},
	 { "Excalibat",							"Excalibat",				"1",		"BatCharge",		"Souls",						"GotChainsaw",				"EBATICO",		"WEPICO5"	},
	 { "ResMelee1",							"Inferno Sword",			"1",		"Souls",			" ",							"GotChainsaw",				"INFSICO1",		"WEPICO6"	},
	 { "Dusk Blade",						"Dusk Blade",				"1",		"SwordHitCharge",	"Souls",						"GotChainsaw",				"DUSKBICO",		"WEPICO7"	},
     { "Sickle",							"Necromancer's Sickle", 	"1",		" ",				" ",							"GotChainsaw",				"SICKLICO",  	"WEPICO8"	},
	 
	 { " Pistol ",							"Pistol",					"2",		"Clip",				" ",							"StartOnly",				"COLTICO",		"WEPICO9"	},
	 { " Akimbo Pistols ",					"Dual Pistols",				"2",		"Clip",				" ",							"GotAkimbo",				"AKIMICO",		"WEPICO10"	},
	 { "Magnum",							"Magnum",					"2",		"Clip",				"BulletSize_6",			        "GotMagnum",				"MAGNICO",		"WEPICO11"	},
	 { "Laser Pistol",						"Laser Pistol",				"2",		"LPistolCharge",	" ",							"GotLaserPistol",			"PBLTZ0", 		"WEPICO12"	},
	 { "ResPistol1",						"Assault Rifle",			"2",		"Clip",				"MGClip5",					    "GotAR",					"4AFLA0",		"WEPICO13"	},
	 { "ResPistol2",						"Viper Staff",				"2",		"ViperAmmo",		" ",							"GotViper",					"VIPAZ0",		"WEPICO14"	},
	 
	 { "RubyWand",							"Ruby Wand",				"2",		"RubyAmmo",			" ",							"GotRubyWand",				"WANDF0",		"WEPICO15"	},
	 { "ScatterGun",						"Scatter Pistol",			"2",		"Clip",				" ",							"GotScatterPistol",			"SPPPA0", 		"WEPICO16"	},
	 
	 { " Shotgun ",							"Shotgun",					"3",		"Shell",			"ShellSize_8N",			        "GotShotgun",				"SGN1ICO",		"WEPICO17"	},
	 { "Upgraded Shotgun",					"Purifier",					"3",		"Shell",			"ShellSize_8",			    	"GotShotgun",				"SGP2Z0",		"WEPICO18"	},
	 { "Upgraded Shotgun2",					"Auto Shotgun",				"3",		"Shell",			"ShellSize_10",			        "GotShotgun",				"UASGICO",		"WEPICO19"	},
	 { "Upgraded Shotgun3",					"Emerald Wand",				"3",		"EmeraldMana",		" ",							"GotShotgun",				"JADEC0",		"WEPICO20"	},
	 { "ResShotgun1",						"Deadlock",					"3",		"Shell",			"ShellSize_12",			        "GotShotgun",				"RW01X0",		"WEPICO21"	},
	 { "ResShotgun2",                       "Nitrogen Crossbow",        "3",        "NitrogenCanister", " ",                            "GotShotgun",               "WBOWA0",       "WEPICO22"	},
	 { "ResShotgun3",						"Wheel of Torment",			"3",		"DemonBlood",		" ",							"GotShotgun",				"WWOPA0",		"WEPICO23"	},
	 
	 { " Super Shotgun ",					"Super Shotgun",			"3",		"Shell",			" ",							"GotSSG",					"SHS1Z0",		"WEPICO24"	},
	 { "Upgraded Super Shotgun",			"H. S. Shotgun",			"3",		"Shell",			" ",							"GotSSG",					"SSGPA0",		"WEPICO25"	},
	 { "Upgraded Super Shotgun2",		    "Erasus",					"3",		"Shell",			"ShellSize_2",			    	"GotSSG",					"ERASICO",		"WEPICO26"	},
	 { "Upgraded Super Shotgun3",		    "Hell's Maw",				"3",		"HellsMawAmmo",		" ",			    			"GotSSG",					"HLMWX0",		"WEPICO27"	},
	 { "ResSSG1",							"Plasma Cannon",			"3",		"PCanAmmo",			"PCanClip",				    	"GotSSG",					"RW02]0",		"WEPICO28"	},
     { "ResSSG2",							"Shocker",					"3",		"Cell",				"IonOverheat",					"GotSSG",					"WSONC0",		"WEPICO29"	},
	 { "ResSSG3",							"Hades Shotgun",			"3",		"HadesAmmo",		"ShellSize_16",					"GotSSG",					"STRGX0",		"WEPICO30"	},
	 
	 { "Silver Gun",						"Silver Gun",				"3",		"ExplodingShell",	" ",							"GotSilver",				"EXSGX0",		"WEPICO31"	},
	 { "Slayer",							"Slayer",					"3",		"SlayerAmmo",		" ",							"GotSlayer",				"SLYPA0",		"WEPICO32"	},
	 
	 { " Machine gun ", 					"Machine Gun",				"4",		"Clip",				"MGClip",				    	"GotMG",					"WASGA0",		"WEPICO33"	},
	 { "Upgraded Machine Gun",			    "H. Machine Gun",			"4",		"Clip",				"MGClip2",				    	"GotMG",					"WSG2ICO", 		"WEPICO34"	},
	 { "Upgraded Machine Gun2",			    "Lead Spitter",				"4",		"Clip",				"MGClip3",				    	"GotMG",					"QWOPA0",		"WEPICO35"	},
	 { "Upgraded Machine Gun3",			    "Demon Sealer",				"4",		"DsealAmmo",		" ",				    		"GotMG",					"JPLAZ0",		"WEPICO36"	},
	 { "ResMG1",							"Templar MG",				"4",		"Clip",				"MGClip4",				    	"GotMG",					"RW03X0",		"WEPICO37"	},
	 { "ResMG2",                            "Riot Cannon",              "4",        "RiotgunShell",     "RiotgunClip",                  "GotMG",                    "RIOTI0",       "WEPICO38"	},
     { "ResMG3",							"Acid Rifle",				"4", 		"AcidAmmo",			"AcidClip",						"GotMG",					"RIF2Y0",		"WEPICO39"	},
     { "ResMG4",							"Fusion Blaster",			"4",		"FusionCell",		"MGClip6",						"GotMG",					"FUSPA0",		"WEPICO40"	},
	 
	 { "Desolator",							"Desolator Cannon",			"4",		"DesolatorAmmo",	"DesolatorOverheat",			"GotDesolator",				"DES4X0",		"WEPICO41"	},
	 { " Minigun ",							"Minigun",					"4",		"Clip",				" ",							"GotCG",					"CHNGX0",		"WEPICO42"	},
	 { "Ebony Cannon",						"Ebony Cannon",				"4",		"EbonyAmmo",		"EbonyAmmoX",			        "GotEbony",					"EBONICO",		"WEPICO43"	},
	 
	 { "Rocket Launcher",					"R. Launcher",				"5",		"RocketAmmo",		" ",							"GotRL",					"LAUNICO",		"WEPICO44"	},
	 { "Upgraded Rocket Launcher",		    "T. Launcher",				"5",		"RocketAmmo",		" ",							"GotRL",					"WPPKG0",		"WEPICO45"	},
	 { "Upgraded Rocket Launcher2",		    "Merc. Launcher",			"5",		"RocketAmmo",		" ",							"GotRL",					"MERCICO",		"WEPICO46"	},
	 { "Upgraded Rocket Launcher3",			"Vindicator",				"5",		"FlakShell",		"ShellSize_18",					"GotRL",					"VS9PA0",		"WEPICO47"	},
	 { "ResRL1",							"Meteor Launcher",			"5",		"MeteorAmmo",		" ",							"GotRL",					"RW06X0",		"WEPICO48"	},
	 { "ResRL2",							"Heavy Grenade Launcher",	"5",		"Grenades",			"HeavyGLCounter",				"GotRL",					"GLAUX0",		"WEPICO49"	},
	 { "ResRL3",							"Freezer Cannon",			"5",		"EverIce",			"FreezerOverheat",				"GotRL",					"FRZCZ0",		"WEPICO50"	},
	 
	 { "Grenade Launcher",					"G. Launcher",				"5",		"Grenades",			" ",							"GotGL",					"NGLPA0",		"WEPICO51"	},
	 { "Upgraded Grenade Launcher",		    "Rotary G. Launcher",		"5",		"Grenades",			" ",							"GotGL",					"RTGLICO",		"WEPICO52"	},
	 { "Heavy Missile Launcher",			"H. M. Launcher",			"5",		"MISAmmo",			" ",							"GotHML",					"WMLNA0",		"WEPICO53"	},
	 
	 { "Plasma Rifle",						"Plasma Rifle",				"6",		"Cell",				" ",							"GotPL",					"NRAIE0",		"WEPICO54"	},
	 { "Upgraded Plasma Rifle",				"Nuclear P. Rifle",			"6",		"Cell",				"PlasmaOverheat",		        "GotPL",					"QNPLX0",		"WEPICO55"	},
	 { "Upgraded Plasma Rifle2",			"Turel Cannon", 			"6",		"Cell",				" ",							"GotPL",					"TURLX0",		"WEPICO56"	},
	 { "Upgraded Plasma Rifle3",			"Frost Fang",				"6",		"EverIce",			" ",							"GotPL",					"FSTFICO",		"WEPICO57"	},
	 { "ResPlasma1",						"Flamethrower",				"6",		"Fuel",				"FuelClip",					    "GotPL",					"RW05X0",		"WEPICO58"	},
	 { "ResPlasma2",                        "Lightning Gun",            "6",        "LightningCell",    "LightningStacks",              "GotPL",                    "LTGSA0",       "WEPICO59"	},
	 { "ResPlasma3",						"Rebounder",				"6",		"Cell",				"RebounderOverheat",			"GotPL",					"DGAPA0",		"WEPICO60"	},
	 
	 { "RhinoRifle",						"Rhino Assault Rifle",		"6",		"Clip",				"MGClip7",						"GotRhino",					"MAXPA0",		"WEPICO61"	},
	 { "Nailgun",							"Nail Gun",					"6",		"NailgunAmmo",		" ",							"GotNail",					"NAIGA0",		"WEPICO62"	},
	 { "Basilisk",							"Basilisk",					"6",		"BasiliskAmmo",		"LoadedBasilisk",			    "GotBasilisk",				"BASICO",		"WEPICO63"	},
	 
	 { "BFG 9000",							"BFG 6000",					"7",		"Cell",				" ",							"GotBFG",					"PRDCX0",		"WEPICO64"	},
	 { "Upgraded BFG 9000",					"BFG 32768",				"7",		"Cell",				" ",							"GotBFG",					"DEBFA0",		"WEPICO65"	},
	 { "Devastator",						"Devastator 5000",			"7",		"DevastatorAmmo",	" ",							"GotBFG", 					"DEVAZ0",		"WEPICO66"	},
	 { "MFG",								"Destruction Generator",	"7",		"Cell",				" ",							"GotBFG",					"BFGPA0",		"WEPICO67"	},
	 { "ResBFG1",							"Ion Cannon",				"7",		"IonAmmo",			"IonOverHeat",			        "GotBFG",					"RW04X0",		"WEPICO68"	},
	 { "ResBFG2",							"Thunder Staff",			"7",		"ThunderAmmo",		"ThunderRangeCounter",			"GotBFG",					"APOPA0",		"WEPICO69"	},
	 
	 { "Gauss Rifle",						"Gauss Rifle",				"7",		"GaussRound",		" ",							"GotGauss",					"GAUSZ0",		"WEPICO70"	},
	 { "Rail gun",							"Rail Gun",					"7",		"Cell",				" ",							"GotRail",					"WRALA0",		"WEPICO71"	},
	 { "Death Ray",							"Death Ray",				"7",		"IonAmmo",			"DeathrayOverheat",				"GotDRay",					"ERWEX0",		"WEPICO88"	},
	 
	 { "Death Staff",						"Death Staff",				"8",		"Souls",			"Souls",						"GotDeath",					"LICICO",		"WEPICO72"	},
	 { "Razorfang",						    "Razorfang",				"8",		"Souls",			"Souls",						"GotRazor",					"WRAZA0",		"WEPICO73"	},
	 { "Sun Staff",							"Sun Staff",				"8",		"Souls",			"Souls",						"GotSun",					"SUNICO",		"WEPICO74"	},
	 { "Soul Reaver",						"Soul Reaver",				"8",		"Souls",			" ",							"GotReaver",				"REAVICO",		"WEPICO75"	},
	 
	 { "Sawedoff",							"Sawedoff",					"9",		"SawedoffShell",	"SawedoffCounter",		        "GotSawedoff",				"SAWNICO",		"WEPICO76"	},
	 { "Soul Render",						"Soul Render",				"9",		"BladeHits",		"BladeCharge",				    "GotSoul",					"SWOICO",		"WEPICO77"	},
	 { "SMG",								"SMG",						"9",		"SMGAmmo",			" ",							"GotSMG",					"SMGICO",  		"WEPICO78"	},
	 { "Hellforge Cannon",				    "Hellforge Cannon",			"9",		"IronBalls",		"IronBalls",					"GotHell",					"WHFCA0",  		"WEPICO79"	},
	 { "Bloodfiend Spine",				    "Bloodfiend Spine",			"9",		"BloodAmmo",		"BloodAmmo",			        "GotGore",					"WGORA0",  		"WEPICO80"	},
	 { "Enforcer Rifle",					"Laser Rifle",				"9",		"LaserAmmo",		"LaserAmmo",			        "GotRifle",					"ENRPA0",  		"WEPICO81"	},
	 { "Venom",								"Venom",					"9",		"VenomAmmo",		"VenomAmmo",			        "GotVenom",					"VENOICO", 		"WEPICO82"	},
	 { "Demon Heart",						"Demon Heart",				"9",		"HeartAmmo",		" ",							"GotHeart",					"HARTICO", 		"WEPICO83"	},
	 { "DarkServantGloves",                 "Dark Servant Gloves",      "9",        "DarkServantEnergy"," ",                            "GotGloves",                "DSGNTICO",     "WEPICO84"	},
	 { "Nailgun2",                          "Heavy Nailgun",            "9",        "BigNail",          " ",                            "GotNailgun2",              "NLCGF0",       "WEPICO85"	},
	 { "Berettas",                          "Akimbo Berettas",          "9",        "BerettaAmmo",      " ",                            "GotBerettas",              "PAKMX0",       "WEPICO86"	},
	 { "Plasma Bolter",						"Plasma Bolter",			"9",		"PlasmaBolterAmmo",	" ",							"GotPlasmaBolter",			"PBL9X0",		"WEPICO87"	},
	 { "Ripper Cannon",						"Ripper Cannon",			"9",		"RipperCannonAmmo",	" ",							"GotRipperCannon",			"TWP0X0",		"WEPICO89"	},
};

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
#define WEPCHECK_SLOT8L "P_Slot8Luxury"

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
	WEPCHECK_SLOT8L
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

#define MAXWEPUPGRADES 9
#define MAXNORMALWEPSLOTS 8
str SlotWeapons[MAXNORMALWEPSLOTS][MAXWEPUPGRADES] = {
	{ " Chainsaw ", "Upgraded Chainsaw", "Katana", "Excalibat", "ResMelee1", "", "", "", "" },
	{ "Magnum", " Akimbo Pistols ", "Laser Pistol", "ResPistol1", "ResPistol2", "", "", "", "" },
	{ " Shotgun ", "Upgraded Shotgun", "Upgraded Shotgun2", "Upgraded Shotgun3", "ResShotgun1", "ResShotgun2", "ResShotgun3", "", "" },
	{ " Super Shotgun ", "Upgraded Super Shotgun", "Upgraded Super Shotgun2", "Upgraded Super Shotgun3", "ResSSG1", "ResSSG2", "ResSSG3", "", "" },
	{ " Machine Gun ", "Upgraded Machine Gun", "Upgraded Machine Gun2", "Upgraded Machine Gun3", "ResMG1", "ResMG2", "ResMG3", "ResMG4", "" },
	{ "Rocket Launcher", "Upgraded Rocket Launcher", "Upgraded Rocket Launcher2", "Upgraded Rocket Launcher2", "ResRL1", "ResRL2", "ResRL3", "", "" },
	{ "Plasma Rifle", "Upgraded Plasma Rifle", "Upgraded Plasma Rifle2", "Upgraded Plasma Rifle3", "ResPlasma1", "ResPlasma2", "ResPlasma3", "" },
	{ "BFG 9000", "Upgraded BFG 9000", "Devastator", "MFG", "ResBFG1", "ResBFG2", "", "", "" },
};

enum {
	DND_PELLETACTOR_HSSG
};

#define MAX_PELLET_ACTORS DND_PELLETACTOR_HSSG + 1
str WeaponPelletActors[MAX_PELLET_ACTORS] = {
	"HeavySSGRailPellet"
};

void ResetWeaponMods(int pnum) {
	int i, j;
	for(j = 0; j < MAXWEPS; ++j) {
		Player_Weapon_Infos[pnum][j].enchants = 0;
		// not saved right now as they aren't added yet
		for(i = 0; i < MAX_WEP_MODS; ++i) {
			Player_Weapon_Infos[pnum][j].wep_mods[i].mod_id = 0;
			Player_Weapon_Infos[pnum][j].wep_mods[i].tier = 0;
			Player_Weapon_Infos[pnum][j].wep_mods[i].low = 0;
			Player_Weapon_Infos[pnum][j].wep_mods[i].high = 0;
		}
		for(i = 0; i < MAX_WEP_BONUSES; ++i) {
			Player_Weapon_Infos[pnum][j].wep_bonuses[i].amt = 0;
		}
	}
}

int CheckSlotWeapon(int slot) {
	for(int i = 0; i < MAXWEPUPGRADES; ++i) {
		if(!StrLen(SlotWeapons[slot - 1][i]))
			return 0;
		if(CheckInventory(SlotWeapons[slot - 1][i]))
			return 1;
	}
	return 0;
}

int GetWeaponPosFromTable() {
	return CheckInventory("DnD_WeaponID");
}

int CheckSuperWeaponPickup (void) {
	if(CheckInventory("Death Staff") || CheckInventory("Razorfang") || 
	   CheckInventory("Sun Staff")  || CheckInventory("Soul Reaver"))
		  return 1;
	return 0;
}

int GetCurrentWeaponID (void) {
	int res = 0;
	for(int i = 0; i < MAXWEPS; ++i) {
		if(CheckWeapon(Weapons[i][WEAPON_NAME])) {
			res = i;
			break;
		}
	}
	return res;
}

str GetWeaponAmmoType (int wepid, int which) {
	if(!which)
		return Weapons[wepid][WEAPON_AMMO1];
	else
		return Weapons[wepid][WEAPON_AMMO2];
}

bool IsTemporaryWeapon(int id) {
	return id >= DND_WEAPON_SAWEDOFF;
}

int GetWeaponCount() {
	int res = 0;
	for(int i = 0; i < MAXWEPS; ++i)
		if(CheckInventory(Weapons[i][WEAPON_NAME]))
			++res;
	return res;
}

#endif