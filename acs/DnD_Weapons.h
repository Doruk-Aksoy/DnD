#ifndef DND_WEAPONS_IN
#define DND_WEAPONS_IN

#include "DnD_WeaponDefs.h"

#define MAX_WEAPON_STRINGS (WEAPON_ICON64 + 1)
str Weapons_Data[MAXWEPS][MAX_WEAPON_STRINGS] = { 		     
	 { " Fists ",							" ",				" ",							"FISTICO",		"WEPICO1"	},
	 { " Chainsaw ",						" ",				" ",							"CSAWA0",		"WEPICO2"	},
	 
	 { "Upgraded Chainsaw",					" ",				" ",							"CSW2A0",		"WEPICO3"	},
	 { "Katana",							" ",				" ",							"JFISZ0",		"WEPICO4"	},
	 { "Excalibat",							"BatCharge",		"Souls",						"EBATICO",		"WEPICO5"	},
	 { "ResMelee1",							"Souls",			" ",							"INFSICO1",		"WEPICO6"	},
	 { "Dusk Blade",						"SwordHitCharge",	"Souls",						"DUSKBICO",		"WEPICO7"	},
     { "Sickle",							" ",				" ",							"SICKLICO",  	"WEPICO8"	},
	 
	 { " Pistol ",							"Clip",				" ",							"COLTICO",		"WEPICO9"	},
	 { " Akimbo Pistols ",					"Clip",				" ",							"AKIMICO",		"WEPICO10"	},
	 { "Magnum",							"Clip",				"BulletSize_6",			        "MAGNICO",		"WEPICO11"	},
	 { "Laser Pistol",						"LPistolCharge",	" ",							"PBLTZ0", 		"WEPICO12"	},
	 { "ResPistol1",						"Clip",				"MGClip5",					    "4AFLA0",		"WEPICO13"	},
	 { "ResPistol2",						"ViperAmmo",		" ",							"VIPAZ0",		"WEPICO14"	},
	 
	 { "RubyWand",							"RubyAmmo",			" ",							"WANDF0",		"WEPICO15"	},
	 { "ScatterGun",						"Clip",				" ",							"SPPPA0", 		"WEPICO16"	},
	 
	 { " Shotgun ",							"Shell",			"ShellSize_8N",			        "SGN1ICO",		"WEPICO17"	},
	 { "Upgraded Shotgun",					"Shell",			"ShellSize_8",			    	"SGP2Z0",		"WEPICO18"	},
	 { "Upgraded Shotgun2",					"Shell",			"ShellSize_10",			        "UASGICO",		"WEPICO19"	},
	 { "Upgraded Shotgun3",					"EmeraldMana",		" ",							"JADEC0",		"WEPICO20"	},
	 { "ResShotgun1",						"Shell",			"ShellSize_12",			        "RW01Z0",		"WEPICO21"	},
	 { "ResShotgun2",                       "NitrogenCanister", " ",                            "WBOWA0",       "WEPICO22"	},
	 { "ResShotgun3",						"DemonBlood",		" ",							"WWOPA0",		"WEPICO23"	},
	 { "ResShotgun4",						"CharonAmmo",		"CharonStacks",					"RW08Z0",		"RW08Z0"	},
	 
	 { " Super Shotgun ",					"Shell",			" ",							"SHS1Z0",		"WEPICO24"	},
	 { "Upgraded Super Shotgun",			"Shell",			" ",							"SSGPA0",		"WEPICO25"	},
	 { "Upgraded Super Shotgun2",		    "Shell",			"ShellSize_2",			    	"ERASICO",		"WEPICO26"	},
	 { "Upgraded Super Shotgun3",		    "HellsMawAmmo",		" ",			    			"HLMWX0",		"WEPICO27"	},
	 { "ResSSG1",							"PCanAmmo",			"PCanClip",				    	"RW02]0",		"WEPICO28"	},
     { "ResSSG2",							"Cell",				"IonOverheat",					"WSONC0",		"WEPICO29"	},
	 { "ResSSG3",							"HadesAmmo",		"ShellSize_16",					"STRGX0",		"WEPICO30"	},
	 { "ResSSG4",							"FlayerAmmo",		" ",							"CBOWZ0",		"WEPICO90"  },
	 
	 { "Silver Gun",						"ExplodingShell",	" ",							"EXSGX0",		"WEPICO31"	},
	 { "Slayer",							"SlayerAmmo",		" ",							"SLYPA0",		"WEPICO32"	},
	 
	 { " Machine gun ", 					"Clip",				"MGClip",				    	"WASGA0",		"WEPICO33"	},
	 { "Upgraded Machine Gun",			    "Clip",				"MGClip2",				    	"WSG2ICO", 		"WEPICO34"	},
	 { "Upgraded Machine Gun2",			    "Clip",				"MGClip3",				    	"QWOPA0",		"WEPICO35"	},
	 { "Upgraded Machine Gun3",			    "DsealAmmo",		" ",				    		"JPLAZ0",		"WEPICO36"	},
	 { "ResMG1",							"Clip",				"MGClip4",				    	"RW03X0",		"WEPICO37"	},
	 { "ResMG2",                            "RiotgunShell",     "RiotgunClip",                  "RIOTI0",       "WEPICO38"	},
     { "ResMG3",							"AcidAmmo",			"AcidClip",						"RIF2Y0",		"WEPICO39"	},
     { "ResMG4",							"FusionCell",		"MGClip6",						"FUSPA0",		"WEPICO40"	},
	 { "ResMG5",							"IncineratorAmmo",	"IncineratorStacks",			"RW09Z0",		"RW09Z0"	},
	 
	 { "Desolator",							"DesolatorAmmo",	"DesolatorOverheat",			"DES4X0",		"WEPICO41"	},
	 { " Minigun ",							"Clip",				" ",							"CHNGX0",		"WEPICO42"	},
	 { "Ebony Cannon",						"EbonyAmmo",		"EbonyAmmoX",			        "EBONICO",		"WEPICO43"	},
	 { "MPPB",								"Cell",				" ",							"MPPBZ0",		"MPPBZ0"	},
	 
	 { "Rocket Launcher",					"RocketAmmo",		" ",							"LAUNICO",		"WEPICO44"	},
	 { "Upgraded Rocket Launcher",		    "RocketAmmo",		" ",							"WPPKG0",		"WEPICO45"	},
	 { "Upgraded Rocket Launcher2",		    "RocketAmmo",		" ",							"MERCICO",		"WEPICO46"	},
	 { "Upgraded Rocket Launcher3",			"FlakShell",		"ShellSize_18",					"VS9PA0",		"WEPICO47"	},
	 { "ResRL1",							"MeteorAmmo",		" ",							"RW06X0",		"WEPICO48"	},
	 { "ResRL2",							"HeavyGrenades",	"HeavyGLCounter",				"GLAUX0",		"WEPICO49"	},
	 { "ResRL3",							"EverIce",			"FreezerOverheat",				"FRZCZ0",		"WEPICO50"	},
	 { "ResRL4",							"GravdisAmmo",		" ",							"BRSGX0",		"WEPICO91"	},
	 { "ResRL5",							"Cell",				" ",							"RW07X0",		"RW07X0"	},
	 
	 { "Grenade Launcher",					"Grenades",			" ",							"NGLPA0",		"WEPICO51"	},
	 { "Upgraded Grenade Launcher",		    "Grenades",			" ",							"RTGLICO",		"WEPICO52"	},
	 { "Heavy Missile Launcher",			"MISAmmo",			" ",							"WMLNA0",		"WEPICO53"	},
	 { "Sedrin Staff",						"SedrinAmmo",		" ",							"SEDRX0",		"WEPICO92"	},
	 
	 { "Plasma Rifle",						"Cell",				" ",							"NRAIE0",		"WEPICO54"	},
	 { "Upgraded Plasma Rifle",				"Cell",				"PlasmaOverheat",		        "QNPLX0",		"WEPICO55"	},
	 { "Upgraded Plasma Rifle2",			"Cell",				" ",							"TURLX0",		"WEPICO56"	},
	 { "Upgraded Plasma Rifle3",			"EverIce",			" ",							"FSTFICO",		"WEPICO57"	},
	 { "ResPlasma1",						"Fuel",				"FuelClip",					    "RW05X0",		"WEPICO58"	},
	 { "ResPlasma2",                        "LightningCell",    "LightningStacks",              "LTGSA0",       "WEPICO59"	},
	 { "ResPlasma3",						"Cell",				"RebounderOverheat",			"DGAPA0",		"WEPICO60"	},
	 { "ResPlasma4",						"DemonBlood",		"LanceStacks",					"RW10Z0",		"RW10Z0"	},
	 
	 { "RhinoRifle",						"Clip",				"MGClip7",						"MAXPA0",		"WEPICO61"	},
	 { "Nailgun",							"NailgunAmmo",		" ",							"NAIGA0",		"WEPICO62"	},
	 { "Basilisk",							"BasiliskAmmo",		"LoadedBasilisk",			    "BASICO",		"WEPICO63"	},
	 
	 { "BFG 9000",							"Cell",				" ",							"PRDCX0",		"WEPICO64"	},
	 { "Upgraded BFG 9000",					"Cell",				" ",							"DEBFA0",		"WEPICO65"	},
	 { "Devastator",						"DevastatorAmmo",	" ",							"DEVAZ0",		"WEPICO66"	},
	 { "MFG",								"Cell",				" ",							"BFGPA0",		"WEPICO67"	},
	 { "ResBFG1",							"IonAmmo",			"IonOverHeat",			        "RW04X0",		"WEPICO68"	},
	 { "ResBFG2",							"ThunderAmmo",		"ThunderRangeCounter",			"APOPA0",		"WEPICO69"	},
	 
	 { "Gauss Rifle",						"GaussRound",		" ",							"GAUSZ0",		"WEPICO70"	},
	 { "Rail gun",							"Cell",				" ",							"WRALA0",		"WEPICO71"	},
	 { "Death Ray",							"IonAmmo",			"DeathrayOverheat",				"ERWEX0",		"WEPICO88"	},
	 
	 { "Death Staff",						"Souls",			"Souls",						"LICICO",		"WEPICO72"	},
	 { "Razorfang",						    "Souls",			"Souls",						"WRAZA0",		"WEPICO73"	},
	 { "Sun Staff",							"Souls",			"Souls",						"SUNICO",		"WEPICO74"	},
	 { "Soul Reaver",						"Souls",			" ",							"REAVICO",		"WEPICO75"	},
	 
	 { "Sawedoff",							"SawedoffShell",	"SawedoffCounter",		        "SAWNICO",		"WEPICO76"	},
	 { "Soul Render",						"BladeHits",		"BladeCharge",				    "SWOICO",		"WEPICO77"	},
	 { "SMG",								"SMGAmmo",			" ",							"SMGICO",  		"WEPICO78"	},
	 { "Hellforge Cannon",				    "IronBalls",		"IronBalls",					"WHFCA0",  		"WEPICO79"	},
	 { "Bloodfiend Spine",				    "BloodAmmo",		"BloodAmmo",			        "WGORA0",  		"WEPICO80"	},
	 { "Enforcer Rifle",					"LaserAmmo",		"LaserAmmo",			        "ENRPA0",  		"WEPICO81"	},
	 { "Venom",								"VenomAmmo",		"VenomAmmo",			        "VENOICO", 		"WEPICO82"	},
	 { "Demon Heart",						"HeartAmmo",		" ",							"HARTICO", 		"WEPICO83"	},
	 { "DarkServantGloves",                 "DarkServantEnergy"," ",                            "DSGNTICO",     "WEPICO84"	},
	 { "Nailgun2",                          "BigNail",          " ",                            "NLCGF0",       "WEPICO85"	},
	 { "Berettas",                          "BerettaAmmo",      " ",                            "PAKMX0",       "WEPICO86"	},
	 { "Plasma Bolter",						"PlasmaBolterAmmo",	" ",							"PBL9X0",		"WEPICO87"	},
	 { "Ripper Cannon",						"RipperCannonAmmo",	" ",							"TWP0X0",		"WEPICO89"	},
};

/*str Weapons[MAXWEPS][MAX_WEAPON_STRINGS] = { 		     
	 { " Fists ",							"Fist",						"1",		" ",				" ",							"FISTICO",		"WEPICO1"	},
	 { " Chainsaw ",						"Chainsaw",					"1",		" ",				" ",							"CSAWA0",		"WEPICO2"	},
	 
	 { "Upgraded Chainsaw",					"Dual Chainsaw",			"1",		" ",				" ",							"CSW2A0",		"WEPICO3"	},
	 { "Katana",							"Katana",					"1",		" ",				" ",							"JFISZ0",		"WEPICO4"	},
	 { "Excalibat",							"Excalibat",				"1",		"BatCharge",		"Souls",						"EBATICO",		"WEPICO5"	},
	 { "ResMelee1",							"Inferno Sword",			"1",		"Souls",			" ",							"INFSICO1",		"WEPICO6"	},
	 { "Dusk Blade",						"Dusk Blade",				"1",		"SwordHitCharge",	"Souls",						"DUSKBICO",		"WEPICO7"	},
     { "Sickle",							"Necromancer's Sickle", 	"1",		" ",				" ",							"SICKLICO",  	"WEPICO8"	},
	 
	 { " Pistol ",							"Pistol",					"2",		"Clip",				" ",							"COLTICO",		"WEPICO9"	},
	 { " Akimbo Pistols ",					"Akimbo Longslides",		"2",		"Clip",				" ",							"AKIMICO",		"WEPICO10"	},
	 { "Magnum",							"Magnum",					"2",		"Clip",				"BulletSize_6",			        "MAGNICO",		"WEPICO11"	},
	 { "Laser Pistol",						"Laser Pistol",				"2",		"LPistolCharge",	" ",							"PBLTZ0", 		"WEPICO12"	},
	 { "ResPistol1",						"Assault Rifle",			"2",		"Clip",				"MGClip5",					    "4AFLA0",		"WEPICO13"	},
	 { "ResPistol2",						"Viper Staff",				"2",		"ViperAmmo",		" ",							"VIPAZ0",		"WEPICO14"	},
	 
	 { "RubyWand",							"Ruby Wand",				"2",		"RubyAmmo",			" ",							"WANDF0",		"WEPICO15"	},
	 { "ScatterGun",						"Scatter Pistol",			"2",		"Clip",				" ",							"SPPPA0", 		"WEPICO16"	},
	 
	 { " Shotgun ",							"Shotgun",					"3",		"Shell",			"ShellSize_8N",			        "SGN1ICO",		"WEPICO17"	},
	 { "Upgraded Shotgun",					"Purifier",					"3",		"Shell",			"ShellSize_8",			    	"SGP2Z0",		"WEPICO18"	},
	 { "Upgraded Shotgun2",					"KillStorm",				"3",		"Shell",			"ShellSize_10",			        "UASGICO",		"WEPICO19"	},
	 { "Upgraded Shotgun3",					"Emerald Wand",				"3",		"EmeraldMana",		" ",							"JADEC0",		"WEPICO20"	},
	 { "ResShotgun1",						"Deadlock",					"3",		"Shell",			"ShellSize_12",			        "RW01Z0",		"WEPICO21"	},
	 { "ResShotgun2",                       "Nitrogen Crossbow",        "3",        "NitrogenCanister", " ",                            "WBOWA0",       "WEPICO22"	},
	 { "ResShotgun3",						"Wheel of Torment",			"3",		"DemonBlood",		" ",							"WWOPA0",		"WEPICO23"	},
	 { "ResShotgun4",						"Charon Blaster",			"3",		"CharonAmmo",		"CharonStacks",					"RW08Z0",		"RW08Z0"	},
	 
	 { " Super Shotgun ",					"Super Shotgun",			"3",		"Shell",			" ",							"SHS1Z0",		"WEPICO24"	},
	 { "Upgraded Super Shotgun",			"Heavy Super Shotgun",		"3",		"Shell",			" ",							"SSGPA0",		"WEPICO25"	},
	 { "Upgraded Super Shotgun2",		    "Erasus",					"3",		"Shell",			"ShellSize_2",			    	"ERASICO",		"WEPICO26"	},
	 { "Upgraded Super Shotgun3",		    "Hell's Maw",				"3",		"HellsMawAmmo",		" ",			    			"HLMWX0",		"WEPICO27"	},
	 { "ResSSG1",							"Plasma Cannon",			"3",		"PCanAmmo",			"PCanClip",				    	"RW02]0",		"WEPICO28"	},
     { "ResSSG2",							"Shocker",					"3",		"Cell",				"IonOverheat",					"WSONC0",		"WEPICO29"	},
	 { "ResSSG3",							"Hades Shotgun",			"3",		"HadesAmmo",		"ShellSize_16",					"STRGX0",		"WEPICO30"	},
	 { "ResSSG4",							"Flayer Crossbow",			"3",		"FlayerAmmo",		" ",							"CBOWZ0",		"WEPICO90"  },
	 
	 { "Silver Gun",						"White Death",				"3",		"ExplodingShell",	" ",							"EXSGX0",		"WEPICO31"	},
	 { "Slayer",							"Slayer",					"3",		"SlayerAmmo",		" ",							"SLYPA0",		"WEPICO32"	},
	 
	 { " Machine gun ", 					"Machine Gun",				"4",		"Clip",				"MGClip",				    	"WASGA0",		"WEPICO33"	},
	 { "Upgraded Machine Gun",			    "Heavy Machine Gun",		"4",		"Clip",				"MGClip2",				    	"WSG2ICO", 		"WEPICO34"	},
	 { "Upgraded Machine Gun2",			    "Lead Spitter",				"4",		"Clip",				"MGClip3",				    	"QWOPA0",		"WEPICO35"	},
	 { "Upgraded Machine Gun3",			    "Demon Sealer",				"4",		"DsealAmmo",		" ",				    		"JPLAZ0",		"WEPICO36"	},
	 { "ResMG1",							"Templar Machine Gun",		"4",		"Clip",				"MGClip4",				    	"RW03X0",		"WEPICO37"	},
	 { "ResMG2",                            "Riot Cannon",              "4",        "RiotgunShell",     "RiotgunClip",                  "RIOTI0",       "WEPICO38"	},
     { "ResMG3",							"Acid Rifle",				"4", 		"AcidAmmo",			"AcidClip",						"RIF2Y0",		"WEPICO39"	},
     { "ResMG4",							"Fusion Blaster",			"4",		"FusionCell",		"MGClip6",						"FUSPA0",		"WEPICO40"	},
	 { "ResMG5",							"Incinerator Shotgun",		"4",		"IncineratorAmmo",	"IncineratorStacks",			"RW09Z0",		"RW09Z0"	},
	 
	 { "Desolator",							"Desolator Cannon",			"4",		"DesolatorAmmo",	"DesolatorOverheat",			"DES4X0",		"WEPICO41"	},
	 { " Minigun ",							"Minigun",					"4",		"Clip",				" ",							"CHNGX0",		"WEPICO42"	},
	 { "Ebony Cannon",						"Ebony Cannon",				"4",		"EbonyAmmo",		"EbonyAmmoX",			        "EBONICO",		"WEPICO43"	},
	 { "MPPB",								"MP. Photon Blaster",		"4",		"Cell",				" ",							"MPPBZ0",		"MPPBZ0"	},
	 
	 { "Rocket Launcher",					"Rocket Launcher",			"5",		"RocketAmmo",		" ",							"LAUNICO",		"WEPICO44"	},
	 { "Upgraded Rocket Launcher",		    "Torpedo Launcher",			"5",		"RocketAmmo",		" ",							"WPPKG0",		"WEPICO45"	},
	 { "Upgraded Rocket Launcher2",		    "Mercury Launcher",			"5",		"RocketAmmo",		" ",							"MERCICO",		"WEPICO46"	},
	 { "Upgraded Rocket Launcher3",			"Vindicator",				"5",		"FlakShell",		"ShellSize_18",					"VS9PA0",		"WEPICO47"	},
	 { "ResRL1",							"Meteor Launcher",			"5",		"MeteorAmmo",		" ",							"RW06X0",		"WEPICO48"	},
	 { "ResRL2",							"Heavy Grenade Launcher",	"5",		"HeavyGrenades",	"HeavyGLCounter",				"GLAUX0",		"WEPICO49"	},
	 { "ResRL3",							"Freezer Cannon",			"5",		"EverIce",			"FreezerOverheat",				"FRZCZ0",		"WEPICO50"	},
	 { "ResRL4",							"Gravdis 5000",				"5",		"GravdisAmmo",		" ",							"BRSGX0",		"WEPICO91"	},
	 { "ResRL5",							"Void Cannon",				"5",		"Cell",				" ",							"RW07X0",		"RW07X0"	},
	 
	 { "Grenade Launcher",					"Grenade Launcher",			"5",		"Grenades",			" ",							"NGLPA0",		"WEPICO51"	},
	 { "Upgraded Grenade Launcher",		    "Rotary Grenade Launcher",	"5",		"Grenades",			" ",							"RTGLICO",		"WEPICO52"	},
	 { "Heavy Missile Launcher",			"Heavy Missile Launcher",	"5",		"MISAmmo",			" ",							"WMLNA0",		"WEPICO53"	},
	 { "Sedrin Staff",						"Sedrin Staff",				"5",		"SedrinAmmo",		" ",							"SEDRX0",		"WEPICO92"	},
	 
	 { "Plasma Rifle",						"Plasma Rifle",				"6",		"Cell",				" ",							"NRAIE0",		"WEPICO54"	},
	 { "Upgraded Plasma Rifle",				"Nuclear Plasma Rifle",		"6",		"Cell",				"PlasmaOverheat",		        "QNPLX0",		"WEPICO55"	},
	 { "Upgraded Plasma Rifle2",			"Turel Cannon", 			"6",		"Cell",				" ",							"TURLX0",		"WEPICO56"	},
	 { "Upgraded Plasma Rifle3",			"Frost Fang",				"6",		"EverIce",			" ",							"FSTFICO",		"WEPICO57"	},
	 { "ResPlasma1",						"Flamethrower",				"6",		"Fuel",				"FuelClip",					    "RW05X0",		"WEPICO58"	},
	 { "ResPlasma2",                        "Lightning Gun",            "6",        "LightningCell",    "LightningStacks",              "LTGSA0",       "WEPICO59"	},
	 { "ResPlasma3",						"Rebounder",				"6",		"Cell",				"RebounderOverheat",			"DGAPA0",		"WEPICO60"	},
	 { "ResPlasma4",						"Dark Lance",				"6",		"DemonBlood",		"LanceStacks",					"RW10Z0",		"RW10Z0"	},
	 
	 { "RhinoRifle",						"Rhino Assault Rifle",		"6",		"Clip",				"MGClip7",						"MAXPA0",		"WEPICO61"	},
	 { "Nailgun",							"Nail Gun",					"6",		"NailgunAmmo",		" ",							"NAIGA0",		"WEPICO62"	},
	 { "Basilisk",							"Basilisk",					"6",		"BasiliskAmmo",		"LoadedBasilisk",			    "BASICO",		"WEPICO63"	},
	 
	 { "BFG 9000",							"BFG 6000",					"7",		"Cell",				" ",							"PRDCX0",		"WEPICO64"	},
	 { "Upgraded BFG 9000",					"BFG 32768",				"7",		"Cell",				" ",							"DEBFA0",		"WEPICO65"	},
	 { "Devastator",						"Devastator 5000",			"7",		"DevastatorAmmo",	" ",							"DEVAZ0",		"WEPICO66"	},
	 { "MFG",								"Destruction Generator",	"7",		"Cell",				" ",							"BFGPA0",		"WEPICO67"	},
	 { "ResBFG1",							"Ion Cannon",				"7",		"IonAmmo",			"IonOverHeat",			        "RW04X0",		"WEPICO68"	},
	 { "ResBFG2",							"Thunder Staff",			"7",		"ThunderAmmo",		"ThunderRangeCounter",			"APOPA0",		"WEPICO69"	},
	 
	 { "Gauss Rifle",						"Gauss Rifle",				"7",		"GaussRound",		" ",							"GAUSZ0",		"WEPICO70"	},
	 { "Rail gun",							"Rail Gun",					"7",		"Cell",				" ",							"WRALA0",		"WEPICO71"	},
	 { "Death Ray",							"Death Ray",				"7",		"IonAmmo",			"DeathrayOverheat",				"ERWEX0",		"WEPICO88"	},
	 
	 { "Death Staff",						"Death Staff",				"8",		"Souls",			"Souls",						"LICICO",		"WEPICO72"	},
	 { "Razorfang",						    "Razorfang",				"8",		"Souls",			"Souls",						"WRAZA0",		"WEPICO73"	},
	 { "Sun Staff",							"Sun Staff",				"8",		"Souls",			"Souls",						"SUNICO",		"WEPICO74"	},
	 { "Soul Reaver",						"Soul Reaver",				"8",		"Souls",			" ",							"REAVICO",		"WEPICO75"	},
	 
	 { "Sawedoff",							"Sawed-off Super Shotgun",	"9",		"SawedoffShell",	"SawedoffCounter",		        "SAWNICO",		"WEPICO76"	},
	 { "Soul Render",						"Soul Render",				"9",		"BladeHits",		"BladeCharge",				    "SWOICO",		"WEPICO77"	},
	 { "SMG",								"Submachine Gun",			"9",		"SMGAmmo",			" ",							"SMGICO",  		"WEPICO78"	},
	 { "Hellforge Cannon",				    "Hellforge Cannon",			"9",		"IronBalls",		"IronBalls",					"WHFCA0",  		"WEPICO79"	},
	 { "Bloodfiend Spine",				    "Bloodfiend Spine",			"9",		"BloodAmmo",		"BloodAmmo",			        "WGORA0",  		"WEPICO80"	},
	 { "Enforcer Rifle",					"Laser Rifle",				"9",		"LaserAmmo",		"LaserAmmo",			        "ENRPA0",  		"WEPICO81"	},
	 { "Venom",								"Venom",					"9",		"VenomAmmo",		"VenomAmmo",			        "VENOICO", 		"WEPICO82"	},
	 { "Demon Heart",						"Demon Heart",				"9",		"HeartAmmo",		" ",							"HARTICO", 		"WEPICO83"	},
	 { "DarkServantGloves",                 "Dark Servant Gloves",		"9",        "DarkServantEnergy"," ",                            "DSGNTICO",     "WEPICO84"	},
	 { "Nailgun2",                          "Heavy Nail Gun",			"9",        "BigNail",          " ",                            "NLCGF0",       "WEPICO85"	},
	 { "Berettas",                          "Akimbo Berettas",			"9",        "BerettaAmmo",      " ",                            "PAKMX0",       "WEPICO86"	},
	 { "Plasma Bolter",						"Plasma Bolter",			"9",		"PlasmaBolterAmmo",	" ",							"PBL9X0",		"WEPICO87"	},
	 { "Ripper Cannon",						"Ripper Cannon",			"9",		"RipperCannonAmmo",	" ",							"TWP0X0",		"WEPICO89"	},
};*/

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
	{ " Shotgun ", "Upgraded Shotgun", "Upgraded Shotgun2", "Upgraded Shotgun3", "ResShotgun1", "ResShotgun2", "ResShotgun3", "ResShotgun4", "" },
	{ " Super Shotgun ", "Upgraded Super Shotgun", "Upgraded Super Shotgun2", "Upgraded Super Shotgun3", "ResSSG1", "ResSSG2", "ResSSG3", "ResSSG4", "" },
	{ " Machine Gun ", "Upgraded Machine Gun", "Upgraded Machine Gun2", "Upgraded Machine Gun3", "ResMG1", "ResMG2", "ResMG3", "ResMG4", "ResMG5" },
	{ "Rocket Launcher", "Upgraded Rocket Launcher", "Upgraded Rocket Launcher2", "Upgraded Rocket Launcher3", "ResRL1", "ResRL2", "ResRL3", "ResRL4", "ResRL5" },
	{ "Plasma Rifle", "Upgraded Plasma Rifle", "Upgraded Plasma Rifle2", "Upgraded Plasma Rifle3", "ResPlasma1", "ResPlasma2", "ResPlasma3", "ResPlasma4" },
	{ "BFG 9000", "Upgraded BFG 9000", "Devastator", "MFG", "ResBFG1", "ResBFG2", "", "", "" },
};

void ResetWeaponMods(int pnum) {
	int i, j;
	for(j = 0; j < MAXWEPS; ++j) {
		Player_Weapon_Infos[pnum][j].wep_bonus.enchants = 0;
		for(i = 0; i < MAX_WEP_MODS; ++i) {
			Player_Weapon_Infos[pnum][j].wep_mods[i].tier = 0;
			Player_Weapon_Infos[pnum][j].wep_mods[i].val = 0;
		}
		for(i = 0; i < MAX_WEP_BONUSES; ++i)
			Player_Weapon_Infos[pnum][j].wep_bonus.bonus_list[i] = 0;
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

int CheckSuperWeaponPickup(void) {
	if(CheckInventory("Death Staff") || CheckInventory("Razorfang") || 
	   CheckInventory("Sun Staff")  || CheckInventory("Soul Reaver"))
		  return 1;
	return 0;
}

/*int GetCurrentWeaponID(void) {
	int res = 0;
	for(int i = 0; i < MAXWEPS; ++i) {
		if(CheckWeapon(Weapons[i][WEAPON_NAME])) {
			res = i;
			break;
		}
	}
	return res;
}*/

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
		return true;
		
		default:
		return false;
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
