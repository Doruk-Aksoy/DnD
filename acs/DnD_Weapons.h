#ifndef DND_WEAPONS_IN
#define DND_WEAPONS_IN

#define DND_BASE_CRITMODIFIER 200

enum {
	DND_WEAPON_FIST,
	DND_WEAPON_CHAINSAW,
	DND_WEAPON_DOUBLECHAINSAW,
	DND_WEAPON_EXCALIBAT,
	DND_WEAPON_KATANA,
	DND_WEAPON_INFERNOSWORD,
	DND_WEAPON_DUSKBLADE,
	DND_WEAPON_SICKLE,
	
	DND_WEAPON_PISTOL,
	DND_WEAPON_AKIMBOPISTOL,
	DND_WEAPON_MAGNUM,
	DND_WEAPON_LASERPISTOL,
	DND_WEAPON_ASSAULTRIFLE,
	DND_WEAPON_VIPERSTAFF,
	DND_WEAPON_RUBYWAND,
	DND_WEAPON_SCATTERGUN,
	
	DND_WEAPON_SHOTGUN,
	DND_WEAPON_PURIFIER,
	DND_WEAPON_KILLSTORM,
	DND_WEAPON_DEADLOCK,
	DND_WEAPON_NITROGENCROSSBOW,
	
	DND_WEAPON_SUPERSHOTGUN,
	DND_WEAPON_HEAVYSUPERSHOTGUN,
	DND_WEAPON_ERASUS,
	DND_WEAPON_PLASMACANNON,
	DND_WEAPON_SHOCKER,
	DND_WEAPON_STINGRAY,
	
	DND_WEAPON_SILVERGUN,
	DND_WEAPON_SLAYER,
	
	DND_WEAPON_MACHINEGUN,
	DND_WEAPON_HEAVYMACHINEGUN,
	DND_WEAPON_LEADSPITTER,
	DND_WEAPON_DEMONSEALER,
	DND_WEAPON_TEMPLARMG,
	DND_WEAPON_RIOTCANNON,
	DND_WEAPON_ACIDRIFLE,
	DND_WEAPON_FUSIONBLASTER,
	
	DND_WEAPON_DESOLATOR,
	DND_WEAPON_MINIGUN,
	DND_WEAPON_EBONYCANNON,
	
	DND_WEAPON_ROCKETLAUNCHER,
	DND_WEAPON_TORPEDOLAUNCHER,
	DND_WEAPON_MERCURYLAUNCHER,
	DND_WEAPON_VINDICATOR,
	DND_WEAPON_METEORLAUNCHER,
	DND_WEAPON_HEAVYGL,
	DND_WEAPON_FREEZER,
	DND_WEAPON_GRENADELAUNCHER,
	DND_WEAPON_ROTARYGRENADELAUNCHER,
	DND_WEAPON_HEAVYMISSILELAUNCHER,
	
	DND_WEAPON_PLASMARIFLE,
	DND_WEAPON_NUCLEARPLASMARIFLE,
	DND_WEAPON_TURELCANNON,
	DND_WEAPON_FROSTFANG,
	DND_WEAPON_FLAMETHROWER,
	DND_WEAPON_LIGHTNINGGUN,
	DND_WEAPON_REBOUNDER,
	
	DND_WEAPON_RHINORIFLE,
	DND_WEAPON_NAILGUN,
	DND_WEAPON_BASILISK,
	
	DND_WEAPON_BFG6000,
	DND_WEAPON_BFG32768,
	DND_WEAPON_DEVASTATOR,
	DND_WEAPON_MFG,
	DND_WEAPON_IONCANNON,
	DND_WEAPON_THUNDERSTAFF,
	DND_WEAPON_GAUSSRIFLE,
	DND_WEAPON_RAILGUN,
	
	DND_WEAPON_DEATHSTAFF,
	DND_WEAPON_RAZORFANG,
	DND_WEAPON_SUNSTAFF,
	DND_WEAPON_SOULREAVER,
	
	DND_WEAPON_SAWEDOFF,
	DND_WEAPON_SOULRENDER,
	DND_WEAPON_SMG,
	DND_WEAPON_HELLFORGECANNON,
	DND_WEAPON_BLOODFIENDSPINE,
	DND_WEAPON_ENFORCERRIFLE,
	DND_WEAPON_VENOM,
	DND_WEAPON_DEMONHEART,
	DND_WEAPON_DARKGLOVES,
	DND_WEAPON_HEAVYNAILGUN,
	DND_WEAPON_BERETTAS,
	DND_WEAPON_PLASMABOLTER
};

#define DND_LIGHTNINGGUN_DMGPERSTACK 4
#define DND_DUSKBLADE_DMGPERSTACK 3
#define MAXWEPS DND_WEAPON_PLASMABOLTER + 1

enum {  
	WEAPON_NAME,
	WEAPON_TAG,
	WEAPON_SLOT,
	WEAPON_AMMO1,
	WEAPON_AMMO2,
	WEAPON_GOT,
	WEAPON_ICON
};

str Weapons[MAXWEPS][7] = { 		     
	 { " Fists ",							"Fist",						"1",		" ",				" ",							"StartOnly",				"FISTICO"				},
	 { " Chainsaw ",						"Chainsaw",					"1",		" ",				" ",							"GotChainsaw",				"CSAWA0"				},
	 
	 { "Upgraded Chainsaw",					"Dual Chainsaw",			"1",		" ",				" ",							"GotChainsaw",				"CSW2A0"				},
	 { "Katana",							"Katana",					"1",		" ",				" ",							"GotChainsaw",				"JFISZ0"				},
	 { "Excalibat",							"Excalibat",				"1",		"BatCharge",		"Souls",						"GotChainsaw",				"EBATICO"				},
	 { "ResMelee1",							"Inferno Sword",			"1",		"Souls",			" ",							"GotChainsaw",				"INFSICO1"				},
	 { "Dusk Blade",						"Dusk Blade",				"1",		"SwordHitCharge",	"Souls",						"GotChainsaw",				"DUSKBICO"				},
     { "Sickle",							"Necromancer's Sickle", 	"1",		" ",				" ",							"GotChainsaw",				"SICKLICO"  			},
	 
	 { " Pistol ",							"Pistol",					"2",		"Clip",				" ",							"StartOnly",				"COLTICO"				},
	 { " Akimbo Pistols ",					"Dual Pistols",				"2",		"Clip",				" ",							"GotAkimbo",				"AKIMICO"				},
	 { "Magnum",							"Magnum",					"2",		"Clip",				"BulletSize_6",			        "GotMagnum",				"MAGNICO"			    },
	 { "Laser Pistol",						"Laser Pistol",				"2",		"LPistolCharge",	" ",							"GotLaserPistol",			"PBLTZ0" 				},
	 { "ResPistol1",						"Assault Rifle",			"2",		"Clip",				"MGClip5",					    "GotAR",					"4AFLA0"				},
	 { "ResPistol2",						"Viper Staff",				"2",		"ViperAmmo",		" ",							"GotViper",					"VIPAZ0"				},
	 
	 { "RubyWand",							"Ruby Wand",				"2",		"RubyAmmo",			" ",							"GotRubyWand",				"WANDF0"				},
	 { "ScatterGun",						"Scatter Pistol",			"2",		"Clip",				" ",							"GotScatterPistol",			"SPPPA0" 				},
	 
	 { " Shotgun ",							"Shotgun",					"3",		"Shell",			"ShellSize_8N",			        "GotShotgun",				"SGN1ICO"				},
	 { "Upgraded Shotgun",					"Purifier",					"3",		"Shell",			"ShellSize_8",			    	"GotShotgun",				"SGP2Z0"				},
	 { "Upgraded Shotgun2",					"Auto Shotgun",				"3",		"Shell",			"ShellSize_10",			        "GotShotgun",				"UASGICO"				},
	 { "ResShotgun1",						"Deadlock",					"3",		"Shell",			"ShellSize_12",			        "GotShotgun",				"RW01X0"				},
	 { "ResShotgun2",                       "Nitrogen Crossbow",        "3",        "NitrogenCanister", " ",                            "GotShotgun",               "WBOWA0"                },
	 
	 { " Super Shotgun ",					"Super Shotgun",			"3",		"Shell",			" ",							"GotSSG",					"SHS1Z0"				},
	 { "Upgraded Super Shotgun",			"H. S. Shotgun",			"3",		"Shell",			" ",							"GotSSG",					"SSGPA0"				},
	 { "Upgraded Super Shotgun2",		    "Erasus",					"3",		"Shell",			"ShellSize_2",			    	"GotSSG",					"ERASICO"		   		},
	 { "ResSSG1",							"Plasma Cannon",			"3",		"PCanAmmo",			"PCanClip",				    	"GotSSG",					"RW02]0"				},
     { "ResSSG2",							"Shocker",					"3",		"Cell",				"IonOverheat",					"GotSSG",					"WSONC0"				},
	 { "ResSSG3",							"Hades Shotgun",			"3",		"HadesAmmo",		"ShellSize_16",					"GotSSG",					"STRGX0"				},
	 
	 { "Silver Gun",						"Silver Gun",				"3",		"ExplodingShell",	" ",							"GotSilver",				"EXSGX0"				},
	 { "Slayer",							"Slayer",					"3",		"SlayerAmmo",		" ",							"GotSlayer",				"SLYPA0"				},
	 
	 { " Machine gun ", 					"Machine Gun",				"4",		"Clip",				"MGClip",				    	"GotMG",					"WASGA0"				},
	 { "Upgraded Machine Gun",			    "H. Machine Gun",			"4",		"Clip",				"MGClip2",				    	"GotMG",					"WSG2ICO" 			    },
	 { "Upgraded Machine Gun2",			    "Lead Spitter",				"4",		"Clip",				"MGClip3",				    	"GotMG",					"QWOPA0"				},
	 { "Upgraded Machine Gun3",			    "Demon Sealer",				"4",		"DsealAmmo",		" ",				    		"GotMG",					"JPLAZ0"				},
	 { "ResMG1",							"Templar MG",				"4",		"Clip",				"MGClip4",				    	"GotMG",					"RW03X0"				},
	 { "ResMG2",                            "Riot Cannon",              "4",        "RiotgunShell",     "RiotgunClip",                  "GotMG",                    "RIOTI0"                },
     { "ResMG3",							"Acid Rifle",				"4", 		"AcidAmmo",			"AcidClip",						"GotMG",					"RIF2Y0"				},
     { "ResMG4",							"Fusion Blaster",			"4",		"FusionCell",		"MGClip6",						"GotMG",					"FUSPA0"				},
	 
	 { "Desolator",							"Desolator Cannon",			"4",		"DesolatorAmmo",	"DesolatorOverheat",			"GotDesolator",				"DES4X0"				},
	 { " Minigun ",							"Minigun",					"4",		"Clip",				" ",							"GotCG",					"CHNGX0"				},
	 { "Ebony Cannon",						"Ebony Cannon",				"4",		"EbonyAmmo",		"EbonyAmmoX",			        "GotEbony",					"EBONICO"				},
	 
	 { "Rocket Launcher",					"R. Launcher",				"5",		"RocketAmmo",		" ",							"GotRL",					"LAUNICO"				},
	 { "Upgraded Rocket Launcher",		    "T. Launcher",				"5",		"RocketAmmo",		" ",							"GotRL",					"WPPKG0"				},
	 { "Upgraded Rocket Launcher2",		    "Merc. Launcher",			"5",		"RocketAmmo",		" ",							"GotRL",					"MERCICO"				},
	 { "Upgraded Rocket Launcher3",			"Vindicator",				"5",		"FlakShell",		"ShellSize_18",					"GotRL",					"VS9PA0"				},
	 { "ResRL1",							"Meteor Launcher",			"5",		"MeteorAmmo",		" ",							"GotRL",					"RW06X0"				},
	 { "ResRL2",							"Heavy Grenade Launcher",	"5",		"Grenades",			"HeavyGLCounter",				"GotRL",					"GLAUX0"				},
	 { "ResRL3",							"Freezer Cannon",			"5",		"EverIce",			"FreezerOverheat",				"GotRL",					"FRZCZ0"				},
	 
	 { "Grenade Launcher",					"G. Launcher",				"5",		"Grenades",			" ",							"GotGL",					"NGLPA0"				},
	 { "Upgraded Grenade Launcher",		    "Rotary G. Launcher",		"5",		"Grenades",			" ",							"GotGL",					"RTGLICO"				},
	 { "Heavy Missile Launcher",			"H. M. Launcher",			"5",		"MISAmmo",			" ",							"GotHML",					"WMLNA0"				},
	 
	 { "Plasma Rifle",						"Plasma Rifle",				"6",		"Cell",				" ",							"GotPL",					"NRAIE0"				},
	 { "Upgraded Plasma Rifle",				"Nuclear P. Rifle",			"6",		"Cell",				"PlasmaOverheat",		        "GotPL",					"QNPLX0"				},
	 { "Upgraded Plasma Rifle2",			"Turel Cannon", 			"6",		"Cell",				" ",							"GotPL",					"TURLX0"				},
	 { "Upgraded Plasma Rifle3",			"Frost Fang",				"6",		"EverIce",			" ",							"GotPL",					"FSTFICO"				},
	 { "ResPlasma1",						"Flamethrower",				"6",		"Fuel",				"FuelClip",					    "GotPL",					"RW05X0"				},
	 { "ResPlasma2",                        "Lightning Gun",            "6",        "LightningCell",    "LightningStacks",              "GotPL",                    "LTGSA0"                },
	 { "ResPlasma3",						"Rebounder",				"6",		"Cell",				"RebounderOverheat",			"GotPL",					"DGAPA0"				},
	 
	 { "RhinoRifle",						"Rhino Assault Rifle",		"6",		"Clip",				"MGClip7",						"GotRhino",					"MAXPA0"				},
	 { "Nailgun",							"Nail Gun",					"6",		"NailgunAmmo",		" ",							"GotNail",					"NAIGA0"				},
	 { "Basilisk",							"Basilisk",					"6",		"BasiliskAmmo",		"LoadedBasilisk",			    "GotBasilisk",				"BASICO"				},
	 
	 { "BFG 9000",							"BFG 6000",					"7",		"Cell",				" ",							"GotBFG",					"PRDCX0"				},
	 { "Upgraded BFG 9000",					"BFG 32768",				"7",		"Cell",				" ",							"GotBFG",					"DEBFA0"				},
	 { "Devastator",						"Devastator 5000",			"7",		"Cell",				" ",							"GotBFG", 					"DEVAZ0"				},
	 { "MFG",								"Destruction Generator",	"7",		"Cell",				" ",							"GotBFG",					"BFGPA0"				},
	 { "ResBFG1",							"Ion Cannon",				"7",		"IonAmmo",			"IonOverHeat",			        "GotBFG",					"RW04X0"				},
	 { "ResBFG2",							"Thunder Staff",			"7",		"ThunderAmmo",		"ThunderRangeCounter",			"GotBFG",					"APOPA0"				},
	 { "Gauss Rifle",						"Gauss Rifle",				"7",		"GaussRound",		" ",							"GotGauss",					"GAUSZ0"				},
	 { "Rail gun",							"Rail Gun",					"7",		"Cell",				" ",							"GotRail",					"WRALA0"				},
	 
	 { "Death Staff",						"Death Staff",				"8",		"Souls",			"Souls",						"GotDeath",					"LICICO"				},
	 { "Razorfang",						    "Razorfang",				"8",		"Souls",			"Souls",						"GotRazor",					"WRAZA0"				},
	 { "Sun Staff",							"Sun Staff",				"8",		"Souls",			"Souls",						"GotSun",					"SUNICO"				},
	 { "Soul Reaver",						"Soul Reaver",				"8",		"Souls",			" ",							"GotReaver",				"REAVICO"				},
	 
	 { "Sawedoff",							"Sawedoff",					"9",		"SawedoffShell",	"SawedoffCounter",		        "GotSawedoff",				"SAWNICO"			    },
	 { "Soul Render",						"Soul Render",				"9",		"BladeHits",		"BladeCharge",				    "GotSoul",					"SWOICO"			    },
	 { "SMG",								"SMG",						"9",		"SMGAmmo",			" ",							"GotSMG",					"SMGICO"  			    },
	 { "Hellforge Cannon",				    "Hellforge Cannon",			"9",		"IronBalls",		"IronBalls",					"GotHell",					"WHFCA0"  			    },
	 { "Bloodfiend Spine",				    "Bloodfiend Spine",			"9",		"BloodAmmo",		"BloodAmmo",			        "GotGore",					"WGORA0"  			    },
	 { "Enforcer Rifle",					"Laser Rifle",				"9",		"LaserAmmo",		"LaserAmmo",			        "GotRifle",					"ENRPA0"  			    },
	 { "Venom",								"Venom",					"9",		"VenomAmmo",		"VenomAmmo",			        "GotVenom",					"VENOICO" 			    },
	 { "Demon Heart",						"Demon Heart",				"9",		"HeartAmmo",		" ",							"GotHeart",					"HARTICO" 			    },
	 { "DarkServantGloves",                 "Dark Servant Gloves",      "9",        "DarkServantEnergy"," ",                            "GotGloves",                "DSGNTICO"              },
	 { "Nailgun2",                          "Heavy Nailgun",            "9",        "BigNail",          " ",                            "GotNailgun2",              "NLCGF0"                },
	 { "Berettas",                          "Akimbo Berettas",          "9",        "BerettaAmmo",      " ",                            "GotBerettas",              "PAKMX0"                },
	 { "Plasma Bolter",						"Plasma Bolter",			"9",		"PlasmaBolterAmmo",	" ",							"GotPlasmaBolter",			"PBL9X0"				}
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
	DND_OVERHEAT_DESOLATOR
};

#define MAXOVERHEATWEPS DND_OVERHEAT_DESOLATOR + 1
str WeaponOverheatItems[MAXOVERHEATWEPS] = {
	"ShockerOverheat",
	"FreezerOverheat",
	"PlasmaOverheat",
	"IonOverheat",
	"RebounderOverheat",
	"DesolatorOverheat"
};

// 32 overheat weapons supported
int PlayerRunsOverheat[MAXPLAYERS] = { 0 };

#define MAXWEPUPGRADES 9
#define MAXNORMALWEPSLOTS 8
str SlotWeapons[MAXNORMALWEPSLOTS][MAXWEPUPGRADES] = {
	{ " Chainsaw ", "Upgraded Chainsaw", "Katana", "Excalibat", "ResMelee1", "", "", "", "" },
	{ "Magnum", " Akimbo Pistols ", "Laser Pistol", "ResPistol1", "ResPistol2", "", "", "", "" },
	{ " Shotgun ", "Upgraded Shotgun", "Upgraded Shotgun2", "ResShotgun1", "ResShotgun2", "", "", "", "" },
	{ " Super Shotgun ", "Upgraded Super Shotgun", "Upgraded Super Shotgun2", "ResSSG1", "ResSSG2", "", "", "", "" },
	{ " Machine Gun ", "Upgraded Machine Gun", "Upgraded Machine Gun2", "Upgraded Machine Gun3", "ResMG1", "ResMG2", "ResMG3", "ResMG4", "" },
	{ "Rocket Launcher", "Upgraded Rocket Launcher", "Upgraded Rocket Launcher2", "Upgraded Rocket Launcher2", "ResRL1", "ResRL2", "ResRL3", "", "" },
	{ "Plasma Rifle", "Upgraded Plasma Rifle", "Upgraded Plasma Rifle2", "Upgraded Plasma Rifle3", "ResPlasma1", "ResPlasma2", "ResPlasma3", "" },
	{ "BFG 9000", "Upgraded BFG 9000", "Devastator", "MFG", "ResBFG1", "ResBFG2", "", "", "" },
};

// weapon mod data, mod_id contains the modifier, low and high are the rolled values
typedef struct {
	int mod_id;
	int tier;
	int low;
	int high;
} wep_mod_T;

enum {
	WEP_BONUS_CRIT,
	WEP_BONUS_CRITDMG,
	WEP_BONUS_CRITPERCENT,
	WEP_BONUS_DMG
};

typedef struct {
	int amt;
} wep_bonus_T;

#define MAX_WEP_MODS 4 // max allowed mods
#define MAX_WEP_BONUSES WEP_BONUS_DMG + 1
// store players mods etc.
typedef struct {
	int enchants;
	wep_bonus_T wep_bonuses[MAX_WEP_BONUSES];
	wep_mod_T wep_mods[MAX_WEP_MODS];
} wep_info_T;

global wep_info_T 2: Player_Weapon_Infos[MAXPLAYERS][MAXWEPS];

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

#endif