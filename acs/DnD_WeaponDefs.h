#ifndef DND_WEAPONDEFS_IN
#define DND_WEAPONDEFS_IN

#include "DnD_Common.h"

int PlayerWeaponUsed[MAXPLAYERS] = { -1 };

enum {
	DND_WEAPON_FIST,
	DND_WEAPON_CHAINSAW,
	DND_WEAPON_DOUBLECHAINSAW,
	DND_WEAPON_KATANA,
	DND_WEAPON_EXCALIBAT,
	DND_WEAPON_INFERNOSWORD,
	DND_WEAPON_DUSKBLADE,
	DND_WEAPON_SICKLE,
	
	DND_WEAPON_PISTOL,
	DND_WEAPON_AKIMBOPISTOL,
	DND_WEAPON_MAGNUM,
	DND_WEAPON_LASERPISTOL,
	DND_WEAPON_RUBYWAND,
	DND_WEAPON_ASSAULTRIFLE,
	DND_WEAPON_VIPERSTAFF,
	DND_WEAPON_SCATTERGUN,
	
	DND_WEAPON_SHOTGUN,
	DND_WEAPON_PURIFIER,
	DND_WEAPON_KILLSTORM,
	DND_WEAPON_EMERALDWAND,
	DND_WEAPON_DEADLOCK,
	DND_WEAPON_NITROGENCROSSBOW,
	DND_WEAPON_WHEELOFTORMENT,
	DND_WEAPON_CHARONBLASTER,
	
	DND_WEAPON_SUPERSHOTGUN,
	DND_WEAPON_HEAVYSUPERSHOTGUN,
	DND_WEAPON_ERASUS,
	DND_WEAPON_HELLSMAW,
	DND_WEAPON_PLASMACANNON,
	DND_WEAPON_SHOCKER,
	DND_WEAPON_HADES,
	DND_WEAPON_CROSSBOW,
	
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
	DND_WEAPON_INCINERATOR,
	
	DND_WEAPON_DESOLATOR,
	DND_WEAPON_MINIGUN,
	DND_WEAPON_EBONYCANNON,
	DND_WEAPON_MPPB,
	
	DND_WEAPON_ROCKETLAUNCHER,
	DND_WEAPON_TORPEDOLAUNCHER,
	DND_WEAPON_GRENADELAUNCHER,
	DND_WEAPON_VINDICATOR,
	DND_WEAPON_METEORLAUNCHER,
	DND_WEAPON_HEAVYGL,
	DND_WEAPON_FREEZER,
	DND_WEAPON_GRAVDIS,
	DND_WEAPON_VOIDCANNON,
	
	DND_WEAPON_MERCURYLAUNCHER,
	DND_WEAPON_ROTARYGRENADELAUNCHER,
	DND_WEAPON_HEAVYMISSILELAUNCHER,
	DND_WEAPON_SEDRINSTAFF,
	
	DND_WEAPON_PLASMARIFLE,
	DND_WEAPON_NUCLEARPLASMARIFLE,
	DND_WEAPON_TURELCANNON,
	DND_WEAPON_FROSTFANG,
	DND_WEAPON_FLAMETHROWER,
	DND_WEAPON_LIGHTNINGGUN,
	DND_WEAPON_REBOUNDER,
	DND_WEAPON_DARKLANCE,
	
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
	DND_WEAPON_DEATHRAY,
	
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
	DND_WEAPON_PLASMABOLTER,
	DND_WEAPON_RIPPERCANNON
};
#define MAXWEPS (DND_WEAPON_RIPPERCANNON + 1)

// weapon mod data, mod_id contains the modifier, low and high are the rolled values
typedef struct {
	int tier;
	int val;
} wep_mod_T;

enum {
	WEP_MOD_CRIT,
	WEP_MOD_CRITDMG,
	WEP_MOD_CRITPERCENT,
	WEP_MOD_DMG,
	WEP_MOD_PERCENTDAMAGE,
	WEP_MOD_POISONFORPERCENTDAMAGE,
	WEP_MOD_FORCEPAINCHANCE,
	WEP_MOD_POWERSET1 // holds boolean style modifiers
};
#define MAX_WEP_MODS (WEP_MOD_POWERSET1 + 1) // max allowed mods

// store players mods etc.
typedef struct {
	int quality;
	wep_mod_T wep_mods[MAX_WEP_MODS];
} wep_info_T;

global wep_info_T 2: Player_Weapon_Infos[MAXPLAYERS][MAXWEPS];

#include "DnD_WeaponMods.h"

#define FIRST_SLOT0_WEAPON DND_WEAPON_FIST
#define LAST_SLOT0_WEAPON DND_WEAPON_SICKLE
#define LAST_SLOT0_NONLUXURYWEAPON DND_WEAPON_INFERNOSWORD

#define FIRST_SLOT1_WEAPON DND_WEAPON_PISTOL
#define LAST_SLOT1_WEAPON DND_WEAPON_SCATTERGUN

#define FIRST_SLOT2_WEAPON DND_WEAPON_SHOTGUN
#define LAST_SLOT2_WEAPON DND_WEAPON_CHARONBLASTER

#define FIRST_SLOT3_WEAPON DND_WEAPON_SUPERSHOTGUN
#define LAST_SLOT3_WEAPON DND_WEAPON_SLAYER

#define FIRST_SLOT4_WEAPON DND_WEAPON_MACHINEGUN
#define LAST_SLOT4_WEAPON DND_WEAPON_MPPB

#define FIRST_SLOT5_WEAPON DND_WEAPON_ROCKETLAUNCHER
#define LAST_SLOT5_WEAPON DND_WEAPON_SEDRINSTAFF

#define FIRST_SLOT6_WEAPON DND_WEAPON_PLASMARIFLE
#define LAST_SLOT6_WEAPON DND_WEAPON_BASILISK

#define FIRST_SLOT7_WEAPON DND_WEAPON_BFG6000
#define LAST_SLOT7_WEAPON DND_WEAPON_DEATHRAY

#define FIRST_SLOT8_WEAPON DND_WEAPON_DEATHSTAFF
#define LAST_SLOT8_WEAPON DND_WEAPON_SOULREAVER

#define FIRST_SLOT9_WEAPON DND_WEAPON_SAWEDOFF
#define LAST_SLOT9_WEAPON DND_WEAPON_RIPPERCANNON

#define DND_LIGHTNINGGUN_DMGPERSTACK 4
#define DND_DUSKBLADE_DMGPERSTACK 3
#define MAXWEPQUALITY 50
#define MAXWEPSLOTS 9

enum {  
	WEAPON_NAME,
	WEAPON_AMMO1,
	WEAPON_AMMO2,
	WEAPON_ICON
};

// keep here for now for ammo save chance
#define MAX_MAGAZINES 24
str WeaponMagazineList[MAX_MAGAZINES] = {
	"SawedoffCounter",
	"AkimboClipLeft",
	"AkimboClipRight",
	"BulletSize_6",
	"ShellSize_2",
	"ShellSize_8",
	"ShellSize_8N",
	"ShellSize_10",
	"ShellSize_12",
	"ShellSize_16",
	"ShellSize_18",
	"MGClip",
	"MGClip2",
	"MGClip3",
	"MGClip4",
	"MGClip5",
	"MGClip6",
	"MGClip7",
	"LoadedBasilisk",
	"PCanClip",
	"RiotgunClip",
	"AcidClip",
	"HeavyGLCounter",
	"FuelClip"
};

// Used for base when increasing these using magazine cap increase
int WeaponMagazineCaps[MAX_MAGAZINES] = {
	2,
	12,
	12,
	6,
	2,
	8,
	8,
	10,
	12,
	16,
	18,
	50,
	60,
	75,
	40,
	31,
	40,
	75,
	10,
	5,
	8,
	32,
	3,
	75
};

#include "DnD_TempWeps.h"

#define MAX_WEAPON_STRINGS (WEAPON_ICON + 1)

typedef struct wep_data {
	int ammo_use1;
	int ammo_use2;
	int properties;
	str name;
	str ammo_name1;
	str ammo_name2;
	str icon;
} wep_data_T;

global wep_data_T 31: Weapons_Data[MAXWEPS];

void SetupWeaponData() {
	// SLOT 1
	Weapons_Data[DND_WEAPON_FIST].name = " Fist ";
	Weapons_Data[DND_WEAPON_FIST].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_FIST].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_FIST].icon = "WEPICO1";
	Weapons_Data[DND_WEAPON_FIST].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_FIST].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_FIST].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_CHAINSAW].name = " Chainsaw ";
	Weapons_Data[DND_WEAPON_CHAINSAW].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_CHAINSAW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_CHAINSAW].icon = "WEPICO2";
	Weapons_Data[DND_WEAPON_CHAINSAW].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_CHAINSAW].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_CHAINSAW].properties = WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].name = "Upgraded Chainsaw";
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].icon = "WEPICO3";
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].properties = WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_KATANA].name = "Katana";
	Weapons_Data[DND_WEAPON_KATANA].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_KATANA].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_KATANA].icon = "WEPICO4";
	Weapons_Data[DND_WEAPON_KATANA].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_KATANA].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_KATANA].properties = WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_EXCALIBAT].name = "Excalibat";
	Weapons_Data[DND_WEAPON_EXCALIBAT].ammo_name1 = "BatCharge";
	Weapons_Data[DND_WEAPON_EXCALIBAT].ammo_name2 = "Souls";
	Weapons_Data[DND_WEAPON_EXCALIBAT].icon = "WEPICO5";
	Weapons_Data[DND_WEAPON_EXCALIBAT].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_EXCALIBAT].ammo_use2 = 3;
	Weapons_Data[DND_WEAPON_EXCALIBAT].properties = WPROP_CANTHITGHOST | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_INFERNOSWORD].name = "ResMelee1";
	Weapons_Data[DND_WEAPON_INFERNOSWORD].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_INFERNOSWORD].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_INFERNOSWORD].icon = "WEPICO6";
	Weapons_Data[DND_WEAPON_INFERNOSWORD].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_INFERNOSWORD].ammo_use2 = 5;
	Weapons_Data[DND_WEAPON_INFERNOSWORD].properties = WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_DUSKBLADE].name = "Dusk Blade";
	Weapons_Data[DND_WEAPON_DUSKBLADE].ammo_name1 = "SwordHitCharge";
	Weapons_Data[DND_WEAPON_DUSKBLADE].ammo_name2 = "Souls";
	Weapons_Data[DND_WEAPON_DUSKBLADE].icon = "WEPICO7";
	Weapons_Data[DND_WEAPON_DUSKBLADE].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_DUSKBLADE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DUSKBLADE].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_SICKLE].name = "Sickle";
	Weapons_Data[DND_WEAPON_SICKLE].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_SICKLE].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SICKLE].icon = "WEPICO8";
	Weapons_Data[DND_WEAPON_SICKLE].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_SICKLE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SICKLE].properties = WPROP_IRREDUCIBLE;
	
	// SLOT 2
	Weapons_Data[DND_WEAPON_PISTOL].name = " Pistol ";
	Weapons_Data[DND_WEAPON_PISTOL].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_PISTOL].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_PISTOL].icon = "WEPICO9";
	Weapons_Data[DND_WEAPON_PISTOL].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PISTOL].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_PISTOL].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].name = " Akimbo Pistols ";
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].icon = "WEPICO10";
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_MAGNUM].name = "Magnum";
	Weapons_Data[DND_WEAPON_MAGNUM].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_MAGNUM].ammo_name2 = "BulletSize_6";
	Weapons_Data[DND_WEAPON_MAGNUM].icon = "WEPICO11";
	Weapons_Data[DND_WEAPON_MAGNUM].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_MAGNUM].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_MAGNUM].properties = WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_LASERPISTOL].name = "Laser Pistol";
	Weapons_Data[DND_WEAPON_LASERPISTOL].ammo_name1 = "LPistolCharge";
	Weapons_Data[DND_WEAPON_LASERPISTOL].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_LASERPISTOL].icon = "WEPICO12";
	Weapons_Data[DND_WEAPON_LASERPISTOL].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_LASERPISTOL].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_LASERPISTOL].properties = WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].name = "ResPistol1";
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].ammo_name2 = "MGClip5";
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].icon = "WEPICO13";
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_VIPERSTAFF].name = "ResPistol2";
	Weapons_Data[DND_WEAPON_VIPERSTAFF].ammo_name1 = "ViperAmmo";
	Weapons_Data[DND_WEAPON_VIPERSTAFF].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_VIPERSTAFF].icon = "WEPICO14";
	Weapons_Data[DND_WEAPON_VIPERSTAFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_VIPERSTAFF].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_VIPERSTAFF].properties = WPROP_IGNORESHIELD | WPROP_POISON;
	
	Weapons_Data[DND_WEAPON_RUBYWAND].name = "RubyWand";
	Weapons_Data[DND_WEAPON_RUBYWAND].ammo_name1 = "RubyAmmo";
	Weapons_Data[DND_WEAPON_RUBYWAND].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_RUBYWAND].icon = "WEPICO15";
	Weapons_Data[DND_WEAPON_RUBYWAND].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RUBYWAND].ammo_use2 = 7;
	Weapons_Data[DND_WEAPON_RUBYWAND].properties = WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_SCATTERGUN].name = "ScatterGun";
	Weapons_Data[DND_WEAPON_SCATTERGUN].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_SCATTERGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SCATTERGUN].icon = "WEPICO16";
	Weapons_Data[DND_WEAPON_SCATTERGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SCATTERGUN].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_SCATTERGUN].properties = WPROP_IGNORESHIELD | WPROP_IRREDUCIBLE;
	
	// SLOT 3 - SHOTGUNS
	Weapons_Data[DND_WEAPON_SHOTGUN].name = " Shotgun ";
	Weapons_Data[DND_WEAPON_SHOTGUN].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_SHOTGUN].ammo_name2 = "ShellSize_8N";
	Weapons_Data[DND_WEAPON_SHOTGUN].icon = "WEPICO17";
	Weapons_Data[DND_WEAPON_SHOTGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SHOTGUN].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_SHOTGUN].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_PURIFIER].name = "Upgraded Shotgun";
	Weapons_Data[DND_WEAPON_PURIFIER].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_PURIFIER].ammo_name2 = "ShellSize_8";
	Weapons_Data[DND_WEAPON_PURIFIER].icon = "WEPICO18";
	Weapons_Data[DND_WEAPON_PURIFIER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PURIFIER].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_PURIFIER].properties = WPROP_ALTAMMO;
	
	Weapons_Data[DND_WEAPON_KILLSTORM].name = "Upgraded Shotgun2";
	Weapons_Data[DND_WEAPON_KILLSTORM].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_KILLSTORM].ammo_name2 = "ShellSize_10";
	Weapons_Data[DND_WEAPON_KILLSTORM].icon = "WEPICO19";
	Weapons_Data[DND_WEAPON_KILLSTORM].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_KILLSTORM].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_KILLSTORM].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_EMERALDWAND].name = "Upgraded Shotgun3";
	Weapons_Data[DND_WEAPON_EMERALDWAND].ammo_name1 = "EmeraldMana";
	Weapons_Data[DND_WEAPON_EMERALDWAND].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_EMERALDWAND].icon = "WEPICO20";
	Weapons_Data[DND_WEAPON_EMERALDWAND].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_EMERALDWAND].ammo_use2 = 8;
	Weapons_Data[DND_WEAPON_EMERALDWAND].properties = WPROP_CANTHITGHOST | WPROP_POISON;
	
	Weapons_Data[DND_WEAPON_DEADLOCK].name = "ResShotgun1";
	Weapons_Data[DND_WEAPON_DEADLOCK].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_DEADLOCK].ammo_name2 = "ShellSize_12";
	Weapons_Data[DND_WEAPON_DEADLOCK].icon = "WEPICO21";
	Weapons_Data[DND_WEAPON_DEADLOCK].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEADLOCK].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_DEADLOCK].properties = WPROP_ALTAMMO;
	
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].name = "ResShotgun2";
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].ammo_name1 = "NitrogenCanister";
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].icon = "WEPICO22";
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].name = "ResShotgun3";
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].ammo_name1 = "DemonBlood";
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].icon = "WEPICO23";
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].properties = WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_CHARONBLASTER].name = "ResShotgun4";
	Weapons_Data[DND_WEAPON_CHARONBLASTER].ammo_name1 = "CharonAmmo";
	Weapons_Data[DND_WEAPON_CHARONBLASTER].ammo_name2 = "CharonStacks";
	Weapons_Data[DND_WEAPON_CHARONBLASTER].icon = "WEPICO93";
	Weapons_Data[DND_WEAPON_CHARONBLASTER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_CHARONBLASTER].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_CHARONBLASTER].properties = WPROP_NONE;
	
	// SLOT 3 - SUPER SHOTGUNS
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].name = " Super Shotgun ";
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].icon = "WEPICO24";
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].name = "Upgraded Super Shotgun";
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].icon = "WEPICO25";
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].properties = WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_ERASUS].name = "Upgraded Super Shotgun2";
	Weapons_Data[DND_WEAPON_ERASUS].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_ERASUS].ammo_name2 = "ShellSize_2";
	Weapons_Data[DND_WEAPON_ERASUS].icon = "WEPICO26";
	Weapons_Data[DND_WEAPON_ERASUS].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ERASUS].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_ERASUS].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_HELLSMAW].name = "Upgraded Super Shotgun3";
	Weapons_Data[DND_WEAPON_HELLSMAW].ammo_name1 = "HellsMawAmmo";
	Weapons_Data[DND_WEAPON_HELLSMAW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_HELLSMAW].icon = "WEPICO27";
	Weapons_Data[DND_WEAPON_HELLSMAW].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HELLSMAW].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_HELLSMAW].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_PLASMACANNON].name = "ResSSG1";
	Weapons_Data[DND_WEAPON_PLASMACANNON].ammo_name1 = "PCanAmmo";
	Weapons_Data[DND_WEAPON_PLASMACANNON].ammo_name2 = "PCanClip";
	Weapons_Data[DND_WEAPON_PLASMACANNON].icon = "WEPICO28";
	Weapons_Data[DND_WEAPON_PLASMACANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PLASMACANNON].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_PLASMACANNON].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_SHOCKER].name = "ResSSG2";
	Weapons_Data[DND_WEAPON_SHOCKER].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_SHOCKER].ammo_name2 = "IonOverheat";
	Weapons_Data[DND_WEAPON_SHOCKER].icon = "WEPICO29";
	Weapons_Data[DND_WEAPON_SHOCKER].ammo_use1 = 5;
	Weapons_Data[DND_WEAPON_SHOCKER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SHOCKER].properties = WPROP_IGNORESHIELD | WPROP_OVERHEAT;
	
	Weapons_Data[DND_WEAPON_HADES].name = "ResSSG3";
	Weapons_Data[DND_WEAPON_HADES].ammo_name1 = "HadesAmmo";
	Weapons_Data[DND_WEAPON_HADES].ammo_name2 = "ShellSize_16";
	Weapons_Data[DND_WEAPON_HADES].icon = "WEPICO30";
	Weapons_Data[DND_WEAPON_HADES].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_HADES].ammo_use2 = 2;
	Weapons_Data[DND_WEAPON_HADES].properties = WPROP_RIPPER | WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_CROSSBOW].name = "ResSSG4";
	Weapons_Data[DND_WEAPON_CROSSBOW].ammo_name1 = "FlayerAmmo";
	Weapons_Data[DND_WEAPON_CROSSBOW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_CROSSBOW].icon = "WEPICO90";
	Weapons_Data[DND_WEAPON_CROSSBOW].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_CROSSBOW].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_CROSSBOW].properties = WPROP_RIPPER | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_SILVERGUN].name = "Silver Gun";
	Weapons_Data[DND_WEAPON_SILVERGUN].ammo_name1 = "ExplodingShell";
	Weapons_Data[DND_WEAPON_SILVERGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SILVERGUN].icon = "WEPICO31";
	Weapons_Data[DND_WEAPON_SILVERGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SILVERGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SILVERGUN].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_SLAYER].name = "Slayer";
	Weapons_Data[DND_WEAPON_SLAYER].ammo_name1 = "SlayerAmmo";
	Weapons_Data[DND_WEAPON_SLAYER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SLAYER].icon = "WEPICO32";
	Weapons_Data[DND_WEAPON_SLAYER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SLAYER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SLAYER].properties = WPROP_SELFDMG | WPROP_CANTHITGHOST | WPROP_RIPPER;
	
	// SLOT 4
	Weapons_Data[DND_WEAPON_MACHINEGUN].name = " Machine gun ";
	Weapons_Data[DND_WEAPON_MACHINEGUN].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_MACHINEGUN].ammo_name2 = "MGClip";
	Weapons_Data[DND_WEAPON_MACHINEGUN].icon = "WEPICO33";
	Weapons_Data[DND_WEAPON_MACHINEGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_MACHINEGUN].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_MACHINEGUN].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].name = "Upgraded Machine Gun";
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].ammo_name2 = "MGClip2";
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].icon = "WEPICO34";
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_LEADSPITTER].name = "Upgraded Machine Gun2";
	Weapons_Data[DND_WEAPON_LEADSPITTER].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_LEADSPITTER].ammo_name2 = "MGClip3";
	Weapons_Data[DND_WEAPON_LEADSPITTER].icon = "WEPICO35";
	Weapons_Data[DND_WEAPON_LEADSPITTER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_LEADSPITTER].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_LEADSPITTER].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_DEMONSEALER].name = "Upgraded Machine Gun3";
	Weapons_Data[DND_WEAPON_DEMONSEALER].ammo_name1 = "DsealAmmo";
	Weapons_Data[DND_WEAPON_DEMONSEALER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DEMONSEALER].icon = "WEPICO36";
	Weapons_Data[DND_WEAPON_DEMONSEALER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEMONSEALER].ammo_use2 = 10;
	Weapons_Data[DND_WEAPON_DEMONSEALER].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_TEMPLARMG].name = "ResMG1";
	Weapons_Data[DND_WEAPON_TEMPLARMG].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_TEMPLARMG].ammo_name2 = "MGClip4";
	Weapons_Data[DND_WEAPON_TEMPLARMG].icon = "WEPICO37";
	Weapons_Data[DND_WEAPON_TEMPLARMG].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_TEMPLARMG].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_TEMPLARMG].properties = WPROP_ALTAMMO;
	
	Weapons_Data[DND_WEAPON_RIOTCANNON].name = "ResMG2";
	Weapons_Data[DND_WEAPON_RIOTCANNON].ammo_name1 = "RiotgunShell";
	Weapons_Data[DND_WEAPON_RIOTCANNON].ammo_name2 = "RiotgunClip";
	Weapons_Data[DND_WEAPON_RIOTCANNON].icon = "WEPICO38";
	Weapons_Data[DND_WEAPON_RIOTCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RIOTCANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_RIOTCANNON].properties = WPROP_ALTAMMO;
	
	Weapons_Data[DND_WEAPON_ACIDRIFLE].name = "ResMG3";
	Weapons_Data[DND_WEAPON_ACIDRIFLE].ammo_name1 = "AcidAmmo";
	Weapons_Data[DND_WEAPON_ACIDRIFLE].ammo_name2 = "AcidClip";
	Weapons_Data[DND_WEAPON_ACIDRIFLE].icon = "WEPICO39";
	Weapons_Data[DND_WEAPON_ACIDRIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ACIDRIFLE].ammo_use2 = 8;
	Weapons_Data[DND_WEAPON_ACIDRIFLE].properties = WPROP_POISON;
	
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].name = "ResMG4";
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].ammo_name1 = "FusionCell";
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].ammo_name2 = "MGClip6";
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].icon = "WEPICO40";
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].ammo_use2 = 20;
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].properties = WPROP_IGNORESHIELD | WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_INCINERATOR].name = "ResMG5";
	Weapons_Data[DND_WEAPON_INCINERATOR].ammo_name1 = "IncineratorAmmo";
	Weapons_Data[DND_WEAPON_INCINERATOR].ammo_name2 = "IncineratorStacks";
	Weapons_Data[DND_WEAPON_INCINERATOR].icon = "WEPICO94";
	Weapons_Data[DND_WEAPON_INCINERATOR].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_INCINERATOR].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_INCINERATOR].properties = WPROP_IGNORESHIELD | WPROP_RIPPER | WPROP_SELFDMG | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_DESOLATOR].name = "Desolator";
	Weapons_Data[DND_WEAPON_DESOLATOR].ammo_name1 = "DesolatorAmmo";
	Weapons_Data[DND_WEAPON_DESOLATOR].ammo_name2 = "DesolatorOverheat";
	Weapons_Data[DND_WEAPON_DESOLATOR].icon = "WEPICO41";
	Weapons_Data[DND_WEAPON_DESOLATOR].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DESOLATOR].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DESOLATOR].properties = WPROP_OVERHEAT | WPROP_POISON;
	
	Weapons_Data[DND_WEAPON_MINIGUN].name = " Minigun ";
	Weapons_Data[DND_WEAPON_MINIGUN].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_MINIGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_MINIGUN].icon = "WEPICO42";
	Weapons_Data[DND_WEAPON_MINIGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_MINIGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_MINIGUN].properties = WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_EBONYCANNON].name = "Ebony Cannon";
	Weapons_Data[DND_WEAPON_EBONYCANNON].ammo_name1 = "EbonyAmmo";
	Weapons_Data[DND_WEAPON_EBONYCANNON].ammo_name2 = "EbonyAmmoX";
	Weapons_Data[DND_WEAPON_EBONYCANNON].icon = "WEPICO43";
	Weapons_Data[DND_WEAPON_EBONYCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_EBONYCANNON].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_EBONYCANNON].properties = WPROP_IGNORESHIELD | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_MPPB].name = "MPPB";
	Weapons_Data[DND_WEAPON_MPPB].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_MPPB].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_MPPB].icon = "WEPICO95";
	Weapons_Data[DND_WEAPON_MPPB].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_MPPB].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_MPPB].properties = WPROP_RIPPER | WPROP_NOREFLECT | WPROP_CANTHITGHOST;
	
	// SLOT 5
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].name = "Rocket Launcher";
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].ammo_name1 = "RocketAmmo";
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].icon = "WEPICO44";
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].name = "Upgraded Rocket Launcher";
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].ammo_name1 = "RocketAmmo";
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].icon = "WEPICO45";
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].name = "Grenade Launcher";
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_name1 = "Grenades";
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].icon = "WEPICO51";
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_ALTAMMO;
	
	Weapons_Data[DND_WEAPON_VINDICATOR].name = "Upgraded Rocket Launcher3";
	Weapons_Data[DND_WEAPON_VINDICATOR].ammo_name1 = "FlakShell";
	Weapons_Data[DND_WEAPON_VINDICATOR].ammo_name2 = "ShellSize_18";
	Weapons_Data[DND_WEAPON_VINDICATOR].icon = "WEPICO47";
	Weapons_Data[DND_WEAPON_VINDICATOR].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_VINDICATOR].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_VINDICATOR].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].name = "ResRL1";
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].ammo_name1 = "MeteorAmmo";
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].icon = "WEPICO48";
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].properties = WPROP_IGNORESHIELD | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_HEAVYGL].name = "ResRL2";
	Weapons_Data[DND_WEAPON_HEAVYGL].ammo_name1 = "HeavyGrenades";
	Weapons_Data[DND_WEAPON_HEAVYGL].ammo_name2 = "HeavyGLCounter";
	Weapons_Data[DND_WEAPON_HEAVYGL].icon = "WEPICO49";
	Weapons_Data[DND_WEAPON_HEAVYGL].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HEAVYGL].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HEAVYGL].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_FREEZER].name = "ResRL3";
	Weapons_Data[DND_WEAPON_FREEZER].ammo_name1 = "EverIce";
	Weapons_Data[DND_WEAPON_FREEZER].ammo_name2 = "FreezerOverheat";
	Weapons_Data[DND_WEAPON_FREEZER].icon = "WEPICO50";
	Weapons_Data[DND_WEAPON_FREEZER].ammo_use1 = 5;
	Weapons_Data[DND_WEAPON_FREEZER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_FREEZER].properties = WPROP_RIPPER | WPROP_OVERHEAT;
	
	Weapons_Data[DND_WEAPON_GRAVDIS].name = "ResRL4";
	Weapons_Data[DND_WEAPON_GRAVDIS].ammo_name1 = "GravdisAmmo";
	Weapons_Data[DND_WEAPON_GRAVDIS].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_GRAVDIS].icon = "WEPICO91";
	Weapons_Data[DND_WEAPON_GRAVDIS].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_GRAVDIS].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_GRAVDIS].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_VOIDCANNON].name = "ResRL5";
	Weapons_Data[DND_WEAPON_VOIDCANNON].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_VOIDCANNON].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_VOIDCANNON].icon = "WEPICO96";
	Weapons_Data[DND_WEAPON_VOIDCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_VOIDCANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_VOIDCANNON].properties = WPROP_RIPPER | WPROP_IGNORESHIELD | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].name = "Upgraded Rocket Launcher2";
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].ammo_name1 = "RocketAmmo";
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].icon = "WEPICO46";
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].name = "Upgraded Grenade Launcher";
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].ammo_name1 = "Grenades";
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].icon = "WEPICO52";
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].name = "Heavy Missile Launcher";
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].ammo_name1 = "MISAmmo";
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].icon = "WEPICO53";
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].name = "Sedrin Staff";
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].ammo_name1 = "SedrinAmmo";
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].icon = "WEPICO92";
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].properties = WPROP_SELFDMG | WPROP_POISON;
	
	// SLOT 6
	Weapons_Data[DND_WEAPON_PLASMARIFLE].name = "Plasma Rifle";
	Weapons_Data[DND_WEAPON_PLASMARIFLE].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_PLASMARIFLE].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_PLASMARIFLE].icon = "WEPICO54";
	Weapons_Data[DND_WEAPON_PLASMARIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PLASMARIFLE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_PLASMARIFLE].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].name = "Upgraded Plasma Rifle";
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].ammo_name2 = "PlasmaOverheat";
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].icon = "WEPICO55";
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].properties = WPROP_IGNORESHIELD | WPROP_OVERHEAT | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_TURELCANNON].name = "Upgraded Plasma Rifle2";
	Weapons_Data[DND_WEAPON_TURELCANNON].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_TURELCANNON].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_TURELCANNON].icon = "WEPICO56";
	Weapons_Data[DND_WEAPON_TURELCANNON].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_TURELCANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_TURELCANNON].properties = WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_FROSTFANG].name = "Upgraded Plasma Rifle3";
	Weapons_Data[DND_WEAPON_FROSTFANG].ammo_name1 = "EverIce";
	Weapons_Data[DND_WEAPON_FROSTFANG].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_FROSTFANG].icon = "WEPICO57";
	Weapons_Data[DND_WEAPON_FROSTFANG].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_FROSTFANG].ammo_use2 = 30;
	Weapons_Data[DND_WEAPON_FROSTFANG].properties = WPROP_RIPPER | WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_FLAMETHROWER].name = "ResPlasma1";
	Weapons_Data[DND_WEAPON_FLAMETHROWER].ammo_name1 = "Fuel";
	Weapons_Data[DND_WEAPON_FLAMETHROWER].ammo_name2 = "FuelClip";
	Weapons_Data[DND_WEAPON_FLAMETHROWER].icon = "WEPICO58";
	Weapons_Data[DND_WEAPON_FLAMETHROWER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_FLAMETHROWER].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_FLAMETHROWER].properties = WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].name = "ResPlasma2";
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].ammo_name1 = "LightningCell";
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].ammo_name2 = "LightningStacks";
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].icon = "WEPICO59";
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_REBOUNDER].name = "ResPlasma3";
	Weapons_Data[DND_WEAPON_REBOUNDER].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_REBOUNDER].ammo_name2 = "RebounderOverheat";
	Weapons_Data[DND_WEAPON_REBOUNDER].icon = "WEPICO60";
	Weapons_Data[DND_WEAPON_REBOUNDER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_REBOUNDER].ammo_use2 = 20;
	Weapons_Data[DND_WEAPON_REBOUNDER].properties = WPROP_OVERHEAT | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_DARKLANCE].name = "ResPlasma4";
	Weapons_Data[DND_WEAPON_DARKLANCE].ammo_name1 = "DemonBlood";
	Weapons_Data[DND_WEAPON_DARKLANCE].ammo_name2 = "LanceStacks";
	Weapons_Data[DND_WEAPON_DARKLANCE].icon = "WEPICO97";
	Weapons_Data[DND_WEAPON_DARKLANCE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DARKLANCE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DARKLANCE].properties = WPROP_NOREFLECT | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_RHINORIFLE].name = "Nailgun";
	Weapons_Data[DND_WEAPON_RHINORIFLE].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_RHINORIFLE].ammo_name2 = "MGClip7";
	Weapons_Data[DND_WEAPON_RHINORIFLE].icon = "WEPICO61";
	Weapons_Data[DND_WEAPON_RHINORIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RHINORIFLE].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_RHINORIFLE].properties = WPROP_ALTAMMO;
	
	Weapons_Data[DND_WEAPON_NAILGUN].name = "Nailgun";
	Weapons_Data[DND_WEAPON_NAILGUN].ammo_name1 = "NailgunAmmo";
	Weapons_Data[DND_WEAPON_NAILGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_NAILGUN].icon = "WEPICO62";
	Weapons_Data[DND_WEAPON_NAILGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_NAILGUN].ammo_use2 = 2;
	Weapons_Data[DND_WEAPON_NAILGUN].properties = WPROP_RIPPER | WPROP_CANTHITGHOST | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_BASILISK].name = "Basilisk";
	Weapons_Data[DND_WEAPON_BASILISK].ammo_name1 = "BasiliskAmmo";
	Weapons_Data[DND_WEAPON_BASILISK].ammo_name2 = "LoadedBasilisk";
	Weapons_Data[DND_WEAPON_BASILISK].icon = "WEPICO63";
	Weapons_Data[DND_WEAPON_BASILISK].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_BASILISK].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_BASILISK].properties = WPROP_IGNORESHIELD;
	
	// SLOT 7
	Weapons_Data[DND_WEAPON_BFG6000].name = "BFG 9000";
	Weapons_Data[DND_WEAPON_BFG6000].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_BFG6000].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_BFG6000].icon = "WEPICO64";
	Weapons_Data[DND_WEAPON_BFG6000].ammo_use1 = 50;
	Weapons_Data[DND_WEAPON_BFG6000].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_BFG6000].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_BFG32768].name = "Upgraded BFG 9000";
	Weapons_Data[DND_WEAPON_BFG32768].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_BFG32768].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_BFG32768].icon = "WEPICO65";
	Weapons_Data[DND_WEAPON_BFG32768].ammo_use1 = 75;
	Weapons_Data[DND_WEAPON_BFG32768].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_BFG32768].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_DEVASTATOR].name = "Devastator";
	Weapons_Data[DND_WEAPON_DEVASTATOR].ammo_name1 = "DevastatorAmmo";
	Weapons_Data[DND_WEAPON_DEVASTATOR].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DEVASTATOR].icon = "WEPICO66";
	Weapons_Data[DND_WEAPON_DEVASTATOR].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEVASTATOR].ammo_use2 = 60;
	Weapons_Data[DND_WEAPON_DEVASTATOR].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_CANTHITGHOST | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_MFG].name = "MFG";
	Weapons_Data[DND_WEAPON_MFG].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_MFG].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_MFG].icon = "WEPICO67";
	Weapons_Data[DND_WEAPON_MFG].ammo_use1 = 60;
	Weapons_Data[DND_WEAPON_MFG].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_MFG].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_IONCANNON].name = "ResBFG1";
	Weapons_Data[DND_WEAPON_IONCANNON].ammo_name1 = "IonAmmo";
	Weapons_Data[DND_WEAPON_IONCANNON].ammo_name2 = "IonOverHeat";
	Weapons_Data[DND_WEAPON_IONCANNON].icon = "WEPICO68";
	Weapons_Data[DND_WEAPON_IONCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_IONCANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_IONCANNON].properties = WPROP_OVERHEAT | WPROP_IGNORESHIELD | WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].name = "ResBFG2";
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].ammo_name1 = "ThunderAmmo";
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].ammo_name2 = "ThunderRangeCounter";
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].icon = "WEPICO69";
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].ammo_use1 = 20;
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].ammo_use2 = 40;
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].name = "Gauss Rifle";
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].ammo_name1 = "GaussRound";
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].icon = "WEPICO70";
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_RAILGUN].name = "Rail gun";
	Weapons_Data[DND_WEAPON_RAILGUN].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_RAILGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_RAILGUN].icon = "WEPICO71";
	Weapons_Data[DND_WEAPON_RAILGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RAILGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_RAILGUN].properties = WPROP_IGNORESHIELD | WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_DEATHRAY].name = "Death Ray";
	Weapons_Data[DND_WEAPON_DEATHRAY].ammo_name1 = "IonAmmo";
	Weapons_Data[DND_WEAPON_DEATHRAY].ammo_name2 = "DeathrayOverheat";
	Weapons_Data[DND_WEAPON_DEATHRAY].icon = "WEPICO88";
	Weapons_Data[DND_WEAPON_DEATHRAY].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEATHRAY].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DEATHRAY].properties = WPROP_IGNORESHIELD | WPROP_OVERHEAT;
	
	// SLOT 8
	Weapons_Data[DND_WEAPON_DEATHSTAFF].name = "Death Staff";
	Weapons_Data[DND_WEAPON_DEATHSTAFF].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_DEATHSTAFF].ammo_name2 = "Souls";
	Weapons_Data[DND_WEAPON_DEATHSTAFF].icon = "WEPICO72";
	Weapons_Data[DND_WEAPON_DEATHSTAFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEATHSTAFF].ammo_use2 = 9;
	Weapons_Data[DND_WEAPON_DEATHSTAFF].properties = WPROP_IGNORESHIELD | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_RAZORFANG].name = "Razorfang";
	Weapons_Data[DND_WEAPON_RAZORFANG].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_RAZORFANG].ammo_name2 = "Souls";
	Weapons_Data[DND_WEAPON_RAZORFANG].icon = "WEPICO73";
	Weapons_Data[DND_WEAPON_RAZORFANG].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RAZORFANG].ammo_use2 = 4;
	Weapons_Data[DND_WEAPON_RAZORFANG].properties = WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_SUNSTAFF].name = "Sun Staff";
	Weapons_Data[DND_WEAPON_SUNSTAFF].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_SUNSTAFF].ammo_name2 = "Souls";
	Weapons_Data[DND_WEAPON_SUNSTAFF].icon = "WEPICO74";
	Weapons_Data[DND_WEAPON_SUNSTAFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SUNSTAFF].ammo_use2 = 5;
	Weapons_Data[DND_WEAPON_SUNSTAFF].properties = WPROP_IGNORESHIELD | WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_SOULREAVER].name = "Soul Reaver";
	Weapons_Data[DND_WEAPON_SOULREAVER].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_SOULREAVER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SOULREAVER].icon = "WEPICO75";
	Weapons_Data[DND_WEAPON_SOULREAVER].ammo_use1 = 75;
	Weapons_Data[DND_WEAPON_SOULREAVER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SOULREAVER].properties = WPROP_IGNORESHIELD | WPROP_RIPPER;
	
	// TEMP WEPS
	Weapons_Data[DND_WEAPON_SAWEDOFF].name = "Sawedoff";
	Weapons_Data[DND_WEAPON_SAWEDOFF].ammo_name1 = "SawedoffShell";
	Weapons_Data[DND_WEAPON_SAWEDOFF].ammo_name2 = "SawedoffCounter";
	Weapons_Data[DND_WEAPON_SAWEDOFF].icon = "WEPICO76";
	Weapons_Data[DND_WEAPON_SAWEDOFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SAWEDOFF].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SAWEDOFF].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_SOULRENDER].name = "Soul Render";
	Weapons_Data[DND_WEAPON_SOULRENDER].ammo_name1 = "BladeHits";
	Weapons_Data[DND_WEAPON_SOULRENDER].ammo_name2 = "BladeCharge";
	Weapons_Data[DND_WEAPON_SOULRENDER].icon = "WEPICO77";
	Weapons_Data[DND_WEAPON_SOULRENDER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SOULRENDER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SOULRENDER].properties = WPROP_RIPPER | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_SMG].name = "SMG";
	Weapons_Data[DND_WEAPON_SMG].ammo_name1 = "SMGAmmo";
	Weapons_Data[DND_WEAPON_SMG].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SMG].icon = "WEPICO78";
	Weapons_Data[DND_WEAPON_SMG].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SMG].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SMG].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].name = "Hellforge Cannon";
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].ammo_name1 = "IronBalls";
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].ammo_name2 = "IronBalls";
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].icon = "WEPICO79";
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].properties = WPROP_RIPPER | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].name = "Bloodfiend Spine";
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].ammo_name1 = "BloodAmmo";
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].ammo_name2 = "BloodAmmo";
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].icon = "WEPICO80";
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].properties = WPROP_CANTHITGHOST;

	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].name = "Enforcer Rifle";
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].ammo_name1 = "LaserAmmo";
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].icon = "WEPICO81";
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].ammo_use2 = 2;
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].properties = WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_VENOM].name = "Venom";
	Weapons_Data[DND_WEAPON_VENOM].ammo_name1 = "VenomAmmo";
	Weapons_Data[DND_WEAPON_VENOM].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_VENOM].icon = "WEPICO82";
	Weapons_Data[DND_WEAPON_VENOM].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_VENOM].ammo_use2 = 3;
	Weapons_Data[DND_WEAPON_VENOM].properties = WPROP_RIPPER | WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_DEMONHEART].name = "Demon Heart";
	Weapons_Data[DND_WEAPON_DEMONHEART].ammo_name1 = "HeartAmmo";
	Weapons_Data[DND_WEAPON_DEMONHEART].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DEMONHEART].icon = "WEPICO83";
	Weapons_Data[DND_WEAPON_DEMONHEART].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEMONHEART].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DEMONHEART].properties = WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_DARKGLOVES].name = "DarkServantGloves";
	Weapons_Data[DND_WEAPON_DARKGLOVES].ammo_name1 = "DarkServantEnergy";
	Weapons_Data[DND_WEAPON_DARKGLOVES].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DARKGLOVES].icon = "WEPICO84";
	Weapons_Data[DND_WEAPON_DARKGLOVES].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DARKGLOVES].ammo_use2 = 4;
	Weapons_Data[DND_WEAPON_DARKGLOVES].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].name = "Nailgun2";
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].ammo_name1 = "BigNail";
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].icon = "WEPICO85";
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].properties = WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_BERETTAS].name = "Berettas";
	Weapons_Data[DND_WEAPON_BERETTAS].ammo_name1 = "BerettaAmmo";
	Weapons_Data[DND_WEAPON_BERETTAS].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_BERETTAS].icon = "WEPICO86";
	Weapons_Data[DND_WEAPON_BERETTAS].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_BERETTAS].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_BERETTAS].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_PLASMABOLTER].name = "Plasma Bolter";
	Weapons_Data[DND_WEAPON_PLASMABOLTER].ammo_name1 = "PlasmaBolterAmmo";
	Weapons_Data[DND_WEAPON_PLASMABOLTER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_PLASMABOLTER].icon = "WEPICO87";
	Weapons_Data[DND_WEAPON_PLASMABOLTER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PLASMABOLTER].ammo_use2 = 5;
	Weapons_Data[DND_WEAPON_PLASMABOLTER].properties = WPROP_RIPPER | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_RIPPERCANNON].name = "Ripper Cannon";
	Weapons_Data[DND_WEAPON_RIPPERCANNON].ammo_name1 = "RipperCannonAmmo";
	Weapons_Data[DND_WEAPON_RIPPERCANNON].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_RIPPERCANNON].icon = "WEPICO89";
	Weapons_Data[DND_WEAPON_RIPPERCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RIPPERCANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_RIPPERCANNON].properties = WPROP_RIPPER;
}
	

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

// put them in this place if they are luxury weapons
bool IsLuxuryWeapon(int id) {
	switch(id) {
		case DND_WEAPON_DUSKBLADE:
		case DND_WEAPON_SICKLE:
		
		//case DND_WEAPON_RUBYWAND:
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

void ResetAllWeaponMods(int pnum) {
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
		if(CheckInventory(Weapons_Data[i].name))
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
		if(CheckInventory(Weapons_Data[i].name)) {
			owned_weapons[pnum][count] = i;
			++count;
		}
	}
	// pick from random weapons owned
	return owned_weapons[pnum][random(0, count - 1)];
}

str GetWeaponAmmoType(int wepid, int which) {
	if(!which)
		return Weapons_Data[wepid].ammo_name1;
	return Weapons_Data[wepid].ammo_name2;
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
	return 	CheckInventory(Weapons_Data[DND_WEAPON_SHOTGUN].name) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_PURIFIER].name) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_KILLSTORM].name) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_DEADLOCK].name) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_SUPERSHOTGUN].name) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].name) 		+
			CheckInventory(Weapons_Data[DND_WEAPON_ERASUS].name) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_SHOCKER].name) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_HADES].name) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_SILVERGUN].name) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_INCINERATOR].name) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_VINDICATOR].name) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_SAWEDOFF].name);
}

bool IsTemporaryWeapon(int id) {
	return id >= DND_WEAPON_SAWEDOFF;
}

int GetWeaponCount() {
	int res = 0;
	for(int i = 0; i < MAXWEPS; ++i)
		if(CheckInventory(Weapons_Data[i].name))
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
	// SSAM_SHOCK == 2... THIS IS TERRIBLE, UNTANGLE DEPENDENCY HERE LATER!
	return isSpecial && extra == 2;
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
