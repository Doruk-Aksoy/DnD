#ifndef DND_WEAPONDEFS_IN
#define DND_WEAPONDEFS_IN

#define DND_THUNDERAXE_WEAKENPCT 50
#define CHAIN_LIGHTNING_DELAY 4

#define LESS_OVERHEAT_FACTOR 20

#define SNIPER_CRIT_MAX 100
#define SNIPER_CRIT_BOOST_PER (0.75 / SNIPER_CRIT_MAX)

#define THUNDERSTAFF_SELFDMG_DIST 160.0

#define DND_EXTRAPROJ_SPREADANG 6.25

int PlayerWeaponUsed[MAXPLAYERS] = { -1 };

enum {
	DND_CFW_ALTFIRECHECK = 1,
	DND_CFW_HOLDFIREORRELOADCHECK = 2,
	DND_CFW_DONTCHECKEQUALITY = 4,
	DND_CFW_NOAMMOCONSUMPTIONCHECKS = 8
};

enum {
	DND_RWF_NOTAKEAMMO = 1,
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
	WPROP_POISON = 256,
	WPROP_TECH = 512,
	WPROP_SHOTGUN = 1024,
	WPROP_PRECISION = 2048,
	WPROP_HANDGUN = 4096,
	WPROP_AUTOMATIC = 8192,
	WPROP_ARTILLERY = 16384,
	WPROP_MAGIC = 32768,
	WPROP_MELEE = 65536,

	// properties that aren't shown yet but used internally
	WPROP_HOMINGSCREEN = 131072,
	WPROP_FIRESPROJECTILES = 262144,
};
#define MAX_WEAPON_PROPERTIES 17

str WeaponPropertyImages[MAX_WEAPON_PROPERTIES] = {
	"WNOGHST",
	"WSLFDMG",
	"WOHEAT",
	"WOSHLD",
	"WOALT",
	"WORIP",
	"WOFULL",
	"WNOREFL",
	"WOPOIS",
	"WOTECH",
	"WSHOTKUN",
	"WPRECIS",
	"WHANDGN",
	"WOAUTOM",
	"WOARTILL",
	"WOMAGIC",
	"WOMELEE"
};

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
	DND_WEAPON_PURIFIER, // 17
	DND_WEAPON_KILLSTORM,
	DND_WEAPON_EMERALDWAND,
	DND_WEAPON_DEADLOCK,
	DND_WEAPON_NITROGENCROSSBOW,
	DND_WEAPON_WHEELOFTORMENT,
	DND_WEAPON_CHARONBLASTER,
	
	DND_WEAPON_SUPERSHOTGUN,
	DND_WEAPON_HEAVYSUPERSHOTGUN, // 25
	DND_WEAPON_ERASUS,
	DND_WEAPON_HELLSMAW,
	DND_WEAPON_AXE,
	DND_WEAPON_PLASMACANNON,
	DND_WEAPON_SHOCKER,
	DND_WEAPON_HADES,
	DND_WEAPON_CROSSBOW, // 32
	
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
	DND_WEAPON_HAMMER,
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
	DND_WEAPON_SNIPER, // 66
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
	DND_WEAPON_RIPPERCANNON,

	// special weapon
	DND_WEAPON_ASCENSION
};
#define MAXWEPS (DND_WEAPON_ASCENSION + 1)

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
	WEP_MOD_EXTRAPROJ,
	WEP_MOD_POWERSET1 // holds boolean style modifiers
};
#define MAX_WEP_MODS (WEP_MOD_POWERSET1 + 1) // max allowed mods

// source of weapon mod value, can come from either equipped items or weapon itself directly
enum {
	WMOD_ITEMS,
	WMOD_WEP
};
#define DND_MAX_WEAPONMODSOURCES (WMOD_WEP + 1)

// store players mods etc.
typedef struct {
	int quality;
	wep_mod_T wep_mods[MAX_WEP_MODS][DND_MAX_WEAPONMODSOURCES];
	bool needs_save; // whether this should be written to db or not
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

#define DND_LIGHTNINGGUN_DMGPERSTACK 2
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
enum {
	DND_MAG_SAWEDOFF,
	DND_MAG_AKIMBOLEFT,
	DND_MAG_AKIMBORIGHT,
	DND_MAG_BULLET6,
	DND_MAG_SHELL2,
	DND_MAG_SHELL8,
	DND_MAG_SHELL8N,
	DND_MAG_SHELL10,
	DND_MAG_SHELL12,
	DND_MAG_SHELL16,
	DND_MAG_SHELL18,
	DND_MAG_MGCLIP,
	DND_MAG_MGCLIP2,
	DND_MAG_MGCLIP3,
	DND_MAG_MGCLIP4,
	DND_MAG_MGCLIP5,
	DND_MAG_MGCLIP6,
	DND_MAG_MGCLIP7,
	DND_MAG_LOADEDBASILISK,
	DND_MAG_PCAN,
	DND_MAG_RIOTGUN,
	DND_MAG_ACIDGUN,
	DND_MAG_HEAVYGL,
	DND_MAG_FUEL,

	DND_MAX_MAGAZINES
};

str WeaponMagazineList[DND_MAX_MAGAZINES] = {
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
int WeaponMagazineCaps[DND_MAX_MAGAZINES] = {
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

int GetMagazineCap(int pnum, int mag_id) {
	return WeaponMagazineCaps[mag_id] * (100 + GetPlayerAttributeValue(pnum, INV_MAGAZINE_INCREASE)) / 100;
}

#include "DnD_TempWeps.h"

#define MAX_WEAPON_STRINGS (WEAPON_ICON + 1)

typedef struct wep_data {
	int ammo_use1;
	int ammo_use2;
	int properties;
	int attunement[DND_MAX_ATTRIBUTES]; // which stat attunement does this weapon have -- out of the maximum 5%, these will be integers -- this is also a fixed number now
	str name;
	str ammo_name1;
	str ammo_name2;
	str icon;
} wep_data_T;

global wep_data_T 31: Weapons_Data[MAXWEPS];

#define DND_SICKLE_RESTIME_BASE 10
#define DND_SICKLE_RESTIME_FACTOR 15

void SetupWeaponData() {
	// SLOT 1
	Weapons_Data[DND_WEAPON_FIST].name = " Fists ";
	Weapons_Data[DND_WEAPON_FIST].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_FIST].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_FIST].icon = "WEPICO1";
	Weapons_Data[DND_WEAPON_FIST].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_FIST].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_FIST].properties = WPROP_MELEE;
	Weapons_Data[DND_WEAPON_FIST].attunement[STAT_STR] = 0.03;
	Weapons_Data[DND_WEAPON_FIST].attunement[STAT_DEX] = 0.01;
	
	Weapons_Data[DND_WEAPON_CHAINSAW].name = " Chainsaw ";
	Weapons_Data[DND_WEAPON_CHAINSAW].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_CHAINSAW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_CHAINSAW].icon = "WEPICO2";
	Weapons_Data[DND_WEAPON_CHAINSAW].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_CHAINSAW].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_CHAINSAW].properties = WPROP_CANTHITGHOST | WPROP_MELEE;
	Weapons_Data[DND_WEAPON_CHAINSAW].attunement[STAT_STR] = 0.03;
	Weapons_Data[DND_WEAPON_CHAINSAW].attunement[STAT_DEX] = 0.01;
	
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].name = "Upgraded Chainsaw";
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].icon = "WEPICO3";
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].properties = WPROP_CANTHITGHOST | WPROP_MELEE;
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].attunement[STAT_STR] = 0.03;
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].attunement[STAT_DEX] = 0.01;
	
	Weapons_Data[DND_WEAPON_KATANA].name = "Katana";
	Weapons_Data[DND_WEAPON_KATANA].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_KATANA].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_KATANA].icon = "WEPICO4";
	Weapons_Data[DND_WEAPON_KATANA].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_KATANA].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_KATANA].properties = WPROP_RIPPER | WPROP_MELEE | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_KATANA].attunement[STAT_STR] = 0.015;
	Weapons_Data[DND_WEAPON_KATANA].attunement[STAT_DEX] = 0.025;
	
	Weapons_Data[DND_WEAPON_EXCALIBAT].name = "Excalibat";
	Weapons_Data[DND_WEAPON_EXCALIBAT].ammo_name1 = "BatCharge";
	Weapons_Data[DND_WEAPON_EXCALIBAT].ammo_name2 = "Souls";
	Weapons_Data[DND_WEAPON_EXCALIBAT].icon = "WEPICO5";
	Weapons_Data[DND_WEAPON_EXCALIBAT].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_EXCALIBAT].ammo_use2 = 3;
	Weapons_Data[DND_WEAPON_EXCALIBAT].properties = WPROP_CANTHITGHOST | WPROP_IGNORESHIELD | WPROP_MELEE | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_EXCALIBAT].attunement[STAT_STR] = 0.025;
	Weapons_Data[DND_WEAPON_EXCALIBAT].attunement[STAT_INT] = 0.015;
	
	Weapons_Data[DND_WEAPON_INFERNOSWORD].name = "ResMelee1";
	Weapons_Data[DND_WEAPON_INFERNOSWORD].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_INFERNOSWORD].ammo_name2 = "Souls";
	Weapons_Data[DND_WEAPON_INFERNOSWORD].icon = "WEPICO6";
	Weapons_Data[DND_WEAPON_INFERNOSWORD].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_INFERNOSWORD].ammo_use2 = 5;
	Weapons_Data[DND_WEAPON_INFERNOSWORD].properties = WPROP_CANTHITGHOST | WPROP_MELEE | WPROP_MAGIC | WPROP_SELFDMG | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_INFERNOSWORD].attunement[STAT_STR] = 0.025;
	Weapons_Data[DND_WEAPON_INFERNOSWORD].attunement[STAT_INT] = 0.015;
	
	Weapons_Data[DND_WEAPON_DUSKBLADE].name = "Dusk Blade";
	Weapons_Data[DND_WEAPON_DUSKBLADE].ammo_name1 = "SwordHitCharge";
	Weapons_Data[DND_WEAPON_DUSKBLADE].ammo_name2 = "Souls";
	Weapons_Data[DND_WEAPON_DUSKBLADE].icon = "WEPICO7";
	Weapons_Data[DND_WEAPON_DUSKBLADE].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_DUSKBLADE].ammo_use2 = 11;
	Weapons_Data[DND_WEAPON_DUSKBLADE].properties = WPROP_MELEE | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_DUSKBLADE].attunement[STAT_STR] = 0.015;
	Weapons_Data[DND_WEAPON_DUSKBLADE].attunement[STAT_DEX] = 0.01;
	Weapons_Data[DND_WEAPON_DUSKBLADE].attunement[STAT_INT] = 0.015;
	
	Weapons_Data[DND_WEAPON_SICKLE].name = "Sickle";
	Weapons_Data[DND_WEAPON_SICKLE].ammo_name1 = "SickleCooldown";
	Weapons_Data[DND_WEAPON_SICKLE].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SICKLE].icon = "WEPICO8";
	Weapons_Data[DND_WEAPON_SICKLE].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_SICKLE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SICKLE].properties = WPROP_IRREDUCIBLE | WPROP_MELEE | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_SICKLE].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_SICKLE].attunement[STAT_DEX] = 0.01;
	Weapons_Data[DND_WEAPON_SICKLE].attunement[STAT_INT] = 0.02;
	
	// SLOT 2
	Weapons_Data[DND_WEAPON_PISTOL].name = " Pistol ";
	Weapons_Data[DND_WEAPON_PISTOL].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_PISTOL].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_PISTOL].icon = "WEPICO9";
	Weapons_Data[DND_WEAPON_PISTOL].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PISTOL].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_PISTOL].properties = WPROP_PRECISION | WPROP_HANDGUN;
	Weapons_Data[DND_WEAPON_PISTOL].attunement[STAT_DEX] = 0.04;
	
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].name = " Akimbo Pistols ";
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].icon = "WEPICO10";
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].properties = WPROP_HANDGUN;
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].attunement[STAT_DEX] = 0.03;
	
	Weapons_Data[DND_WEAPON_MAGNUM].name = "Magnum";
	Weapons_Data[DND_WEAPON_MAGNUM].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_MAGNUM].ammo_name2 = "BulletSize_6";
	Weapons_Data[DND_WEAPON_MAGNUM].icon = "WEPICO11";
	Weapons_Data[DND_WEAPON_MAGNUM].ammo_use1 = 6;
	Weapons_Data[DND_WEAPON_MAGNUM].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_MAGNUM].properties = WPROP_IGNORESHIELD | WPROP_PRECISION | WPROP_HANDGUN;
	Weapons_Data[DND_WEAPON_MAGNUM].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_MAGNUM].attunement[STAT_DEX] = 0.03;
	
	Weapons_Data[DND_WEAPON_LASERPISTOL].name = "Laser Pistol";
	Weapons_Data[DND_WEAPON_LASERPISTOL].ammo_name1 = "LPistolCharge";
	Weapons_Data[DND_WEAPON_LASERPISTOL].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_LASERPISTOL].icon = "WEPICO12";
	Weapons_Data[DND_WEAPON_LASERPISTOL].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_LASERPISTOL].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_LASERPISTOL].properties = WPROP_IGNORESHIELD | WPROP_TECH | WPROP_PRECISION | WPROP_HANDGUN;
	Weapons_Data[DND_WEAPON_LASERPISTOL].attunement[STAT_DEX] = 0.03;
	Weapons_Data[DND_WEAPON_LASERPISTOL].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].name = "ResPistol1";
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].ammo_name2 = "MGClip5";
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].icon = "WEPICO13";
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].properties =  WPROP_PRECISION | WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].attunement[STAT_DEX] = 0.03;
	
	Weapons_Data[DND_WEAPON_VIPERSTAFF].name = "ResPistol2";
	Weapons_Data[DND_WEAPON_VIPERSTAFF].ammo_name1 = "ViperAmmo";
	Weapons_Data[DND_WEAPON_VIPERSTAFF].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_VIPERSTAFF].icon = "WEPICO14";
	Weapons_Data[DND_WEAPON_VIPERSTAFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_VIPERSTAFF].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_VIPERSTAFF].properties = WPROP_IGNORESHIELD | WPROP_POISON | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_VIPERSTAFF].attunement[STAT_DEX] = 0.01;
	Weapons_Data[DND_WEAPON_VIPERSTAFF].attunement[STAT_INT] = 0.03;
	
	Weapons_Data[DND_WEAPON_RUBYWAND].name = "RubyWand";
	Weapons_Data[DND_WEAPON_RUBYWAND].ammo_name1 = "RubyAmmo";
	Weapons_Data[DND_WEAPON_RUBYWAND].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_RUBYWAND].icon = "WEPICO15";
	Weapons_Data[DND_WEAPON_RUBYWAND].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RUBYWAND].ammo_use2 = 7;
	Weapons_Data[DND_WEAPON_RUBYWAND].properties = WPROP_CANTHITGHOST | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_RUBYWAND].attunement[STAT_INT] = 0.04;
	
	Weapons_Data[DND_WEAPON_SCATTERGUN].name = "ScatterGun";
	Weapons_Data[DND_WEAPON_SCATTERGUN].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_SCATTERGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SCATTERGUN].icon = "WEPICO16";
	Weapons_Data[DND_WEAPON_SCATTERGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SCATTERGUN].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_SCATTERGUN].properties = WPROP_IGNORESHIELD | WPROP_IRREDUCIBLE | WPROP_TECH | WPROP_HANDGUN | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_SCATTERGUN].attunement[STAT_DEX] = 0.03;
	Weapons_Data[DND_WEAPON_SCATTERGUN].attunement[STAT_INT] = 0.01;

	
	// SLOT 3 - SHOTGUNS
	Weapons_Data[DND_WEAPON_SHOTGUN].name = " Shotgun ";
	Weapons_Data[DND_WEAPON_SHOTGUN].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_SHOTGUN].ammo_name2 = "ShellSize_8N";
	Weapons_Data[DND_WEAPON_SHOTGUN].icon = "WEPICO17";
	Weapons_Data[DND_WEAPON_SHOTGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SHOTGUN].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_SHOTGUN].properties = WPROP_SHOTGUN;
	Weapons_Data[DND_WEAPON_SHOTGUN].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_SHOTGUN].attunement[STAT_DEX] = 0.03;
	
	Weapons_Data[DND_WEAPON_PURIFIER].name = "Upgraded Shotgun";
	Weapons_Data[DND_WEAPON_PURIFIER].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_PURIFIER].ammo_name2 = "ShellSize_8";
	Weapons_Data[DND_WEAPON_PURIFIER].icon = "WEPICO18";
	Weapons_Data[DND_WEAPON_PURIFIER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PURIFIER].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_PURIFIER].properties = WPROP_ALTAMMO | WPROP_SHOTGUN;
	Weapons_Data[DND_WEAPON_PURIFIER].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_PURIFIER].attunement[STAT_DEX] = 0.03;
	
	Weapons_Data[DND_WEAPON_KILLSTORM].name = "Upgraded Shotgun2";
	Weapons_Data[DND_WEAPON_KILLSTORM].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_KILLSTORM].ammo_name2 = "ShellSize_9";
	Weapons_Data[DND_WEAPON_KILLSTORM].icon = "WEPICO19";
	Weapons_Data[DND_WEAPON_KILLSTORM].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_KILLSTORM].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_KILLSTORM].properties = WPROP_SHOTGUN | WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_KILLSTORM].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_KILLSTORM].attunement[STAT_DEX] = 0.03;
	
	Weapons_Data[DND_WEAPON_EMERALDWAND].name = "Upgraded Shotgun3";
	Weapons_Data[DND_WEAPON_EMERALDWAND].ammo_name1 = "EmeraldMana";
	Weapons_Data[DND_WEAPON_EMERALDWAND].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_EMERALDWAND].icon = "WEPICO20";
	Weapons_Data[DND_WEAPON_EMERALDWAND].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_EMERALDWAND].ammo_use2 = 8;
	Weapons_Data[DND_WEAPON_EMERALDWAND].properties = WPROP_CANTHITGHOST | WPROP_POISON | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_EMERALDWAND].attunement[STAT_INT] = 0.04;
	
	Weapons_Data[DND_WEAPON_DEADLOCK].name = "ResShotgun1";
	Weapons_Data[DND_WEAPON_DEADLOCK].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_DEADLOCK].ammo_name2 = "ShellSize_12";
	Weapons_Data[DND_WEAPON_DEADLOCK].icon = "WEPICO21";
	Weapons_Data[DND_WEAPON_DEADLOCK].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEADLOCK].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_DEADLOCK].properties = WPROP_ALTAMMO | WPROP_SHOTGUN;
	Weapons_Data[DND_WEAPON_DEADLOCK].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_DEADLOCK].attunement[STAT_DEX] = 0.03;
	
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].name = "ResShotgun2";
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].ammo_name1 = "NitrogenCanister";
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].icon = "WEPICO22";
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].properties = WPROP_TECH | WPROP_PRECISION;
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].attunement[STAT_DEX] = 0.02;
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].name = "ResShotgun3";
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].ammo_name1 = "DemonBlood";
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].icon = "WEPICO23";
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].properties = WPROP_IGNORESHIELD | WPROP_MAGIC | WPROP_MELEE;
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].attunement[STAT_INT] = 0.03;
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].attunement[STAT_STR] = 0.01;
	
	Weapons_Data[DND_WEAPON_CHARONBLASTER].name = "ResShotgun4";
	Weapons_Data[DND_WEAPON_CHARONBLASTER].ammo_name1 = "CharonAmmo";
	Weapons_Data[DND_WEAPON_CHARONBLASTER].ammo_name2 = "CharonStacks";
	Weapons_Data[DND_WEAPON_CHARONBLASTER].icon = "WEPICO93";
	Weapons_Data[DND_WEAPON_CHARONBLASTER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_CHARONBLASTER].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_CHARONBLASTER].properties =  WPROP_TECH | WPROP_HOMINGSCREEN | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_CHARONBLASTER].attunement[STAT_INT] = 0.01;
	Weapons_Data[DND_WEAPON_CHARONBLASTER].attunement[STAT_DEX] = 0.03;
	
	// SLOT 3 - SUPER SHOTGUNS
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].name = " Super Shotgun ";
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].icon = "WEPICO24";
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].properties = WPROP_SHOTGUN;
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].attunement[STAT_STR] = 0.015;
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].attunement[STAT_DEX] = 0.025;
	
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].name = "Upgraded Super Shotgun";
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].icon = "WEPICO25";
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].properties = WPROP_RIPPER | WPROP_SHOTGUN;
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].attunement[STAT_STR] = 0.015;
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].attunement[STAT_DEX] = 0.025;
	
	Weapons_Data[DND_WEAPON_ERASUS].name = "Upgraded Super Shotgun2";
	Weapons_Data[DND_WEAPON_ERASUS].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_ERASUS].ammo_name2 = "ShellSize_2";
	Weapons_Data[DND_WEAPON_ERASUS].icon = "WEPICO26";
	Weapons_Data[DND_WEAPON_ERASUS].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ERASUS].ammo_use2 = 2;
	Weapons_Data[DND_WEAPON_ERASUS].properties = WPROP_SHOTGUN;
	Weapons_Data[DND_WEAPON_ERASUS].attunement[STAT_STR] = 0.025;
	Weapons_Data[DND_WEAPON_ERASUS].attunement[STAT_DEX] = 0.015;
	
	Weapons_Data[DND_WEAPON_HELLSMAW].name = "Upgraded Super Shotgun3";
	Weapons_Data[DND_WEAPON_HELLSMAW].ammo_name1 = "HellsMawAmmo";
	Weapons_Data[DND_WEAPON_HELLSMAW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_HELLSMAW].icon = "WEPICO27";
	Weapons_Data[DND_WEAPON_HELLSMAW].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HELLSMAW].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_HELLSMAW].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_HELLSMAW].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_HELLSMAW].attunement[STAT_INT] = 0.03;

	Weapons_Data[DND_WEAPON_AXE].name = "TheAxe";
	Weapons_Data[DND_WEAPON_AXE].ammo_name1 = "AxeMana";
	Weapons_Data[DND_WEAPON_AXE].ammo_name2 = "AxeMana";
	Weapons_Data[DND_WEAPON_AXE].icon = "WEPICO99";
	Weapons_Data[DND_WEAPON_AXE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_AXE].ammo_use2 = 3;
	Weapons_Data[DND_WEAPON_AXE].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT | WPROP_MELEE | WPROP_MAGIC;
	Weapons_Data[DND_WEAPON_AXE].attunement[STAT_STR] = 0.025;
	Weapons_Data[DND_WEAPON_AXE].attunement[STAT_DEX] = 0.015;
	
	Weapons_Data[DND_WEAPON_PLASMACANNON].name = "ResSSG1";
	Weapons_Data[DND_WEAPON_PLASMACANNON].ammo_name1 = "PCanAmmo";
	Weapons_Data[DND_WEAPON_PLASMACANNON].ammo_name2 = "PCanClip";
	Weapons_Data[DND_WEAPON_PLASMACANNON].icon = "WEPICO28";
	Weapons_Data[DND_WEAPON_PLASMACANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PLASMACANNON].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_PLASMACANNON].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT | WPROP_TECH | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_PLASMACANNON].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_PLASMACANNON].attunement[STAT_DEX] = 0.02;
	Weapons_Data[DND_WEAPON_PLASMACANNON].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_SHOCKER].name = "ResSSG2";
	Weapons_Data[DND_WEAPON_SHOCKER].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_SHOCKER].ammo_name2 = "IonOverheat";
	Weapons_Data[DND_WEAPON_SHOCKER].icon = "WEPICO29";
	Weapons_Data[DND_WEAPON_SHOCKER].ammo_use1 = 8;
	Weapons_Data[DND_WEAPON_SHOCKER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SHOCKER].properties = WPROP_IGNORESHIELD | WPROP_OVERHEAT | WPROP_TECH | WPROP_SHOTGUN;
	Weapons_Data[DND_WEAPON_SHOCKER].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_SHOCKER].attunement[STAT_DEX] = 0.02;
	Weapons_Data[DND_WEAPON_SHOCKER].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_HADES].name = "ResSSG3";
	Weapons_Data[DND_WEAPON_HADES].ammo_name1 = "HadesAmmo";
	Weapons_Data[DND_WEAPON_HADES].ammo_name2 = "ShellSize_16";
	Weapons_Data[DND_WEAPON_HADES].icon = "WEPICO30";
	Weapons_Data[DND_WEAPON_HADES].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_HADES].ammo_use2 = 2;
	Weapons_Data[DND_WEAPON_HADES].properties = WPROP_CANTHITGHOST | WPROP_SHOTGUN | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_HADES].attunement[STAT_STR] = 0.02;
	Weapons_Data[DND_WEAPON_HADES].attunement[STAT_DEX] = 0.02;
	
	Weapons_Data[DND_WEAPON_CROSSBOW].name = "ResSSG4";
	Weapons_Data[DND_WEAPON_CROSSBOW].ammo_name1 = "FlayerAmmo";
	Weapons_Data[DND_WEAPON_CROSSBOW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_CROSSBOW].icon = "WEPICO90";
	Weapons_Data[DND_WEAPON_CROSSBOW].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_CROSSBOW].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_CROSSBOW].properties = WPROP_RIPPER | WPROP_NOREFLECT | WPROP_PRECISION | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_CROSSBOW].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_CROSSBOW].attunement[STAT_DEX] = 0.03;
	
	Weapons_Data[DND_WEAPON_SILVERGUN].name = "Silver Gun";
	Weapons_Data[DND_WEAPON_SILVERGUN].ammo_name1 = "ExplodingShell";
	Weapons_Data[DND_WEAPON_SILVERGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SILVERGUN].icon = "WEPICO31";
	Weapons_Data[DND_WEAPON_SILVERGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SILVERGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SILVERGUN].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_SHOTGUN | WPROP_ARTILLERY;
	Weapons_Data[DND_WEAPON_SILVERGUN].attunement[STAT_STR] = 0.015;
	Weapons_Data[DND_WEAPON_SILVERGUN].attunement[STAT_DEX] = 0.025;
	
	Weapons_Data[DND_WEAPON_SLAYER].name = "Slayer";
	Weapons_Data[DND_WEAPON_SLAYER].ammo_name1 = "SlayerAmmo";
	Weapons_Data[DND_WEAPON_SLAYER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SLAYER].icon = "WEPICO32";
	Weapons_Data[DND_WEAPON_SLAYER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SLAYER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SLAYER].properties = WPROP_SELFDMG | WPROP_CANTHITGHOST | WPROP_RIPPER | WPROP_MAGIC | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_SLAYER].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_SLAYER].attunement[STAT_INT] = 0.03;
	
	// SLOT 4
	Weapons_Data[DND_WEAPON_MACHINEGUN].name = " Machine gun ";
	Weapons_Data[DND_WEAPON_MACHINEGUN].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_MACHINEGUN].ammo_name2 = "MGClip";
	Weapons_Data[DND_WEAPON_MACHINEGUN].icon = "WEPICO33";
	Weapons_Data[DND_WEAPON_MACHINEGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_MACHINEGUN].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_MACHINEGUN].properties = WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_MACHINEGUN].attunement[STAT_DEX] = 0.04;
	
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].name = "Upgraded Machine Gun";
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].ammo_name2 = "MGClip2";
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].icon = "WEPICO34";
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].properties = WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].attunement[STAT_DEX] = 0.03;
	
	Weapons_Data[DND_WEAPON_LEADSPITTER].name = "Upgraded Machine Gun2";
	Weapons_Data[DND_WEAPON_LEADSPITTER].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_LEADSPITTER].ammo_name2 = "MGClip3";
	Weapons_Data[DND_WEAPON_LEADSPITTER].icon = "WEPICO35";
	Weapons_Data[DND_WEAPON_LEADSPITTER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_LEADSPITTER].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_LEADSPITTER].properties = WPROP_TECH | WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_LEADSPITTER].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_LEADSPITTER].attunement[STAT_DEX] = 0.02;
	Weapons_Data[DND_WEAPON_LEADSPITTER].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_DEMONSEALER].name = "Upgraded Machine Gun3";
	Weapons_Data[DND_WEAPON_DEMONSEALER].ammo_name1 = "DsealAmmo";
	Weapons_Data[DND_WEAPON_DEMONSEALER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DEMONSEALER].icon = "WEPICO36";
	Weapons_Data[DND_WEAPON_DEMONSEALER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEMONSEALER].ammo_use2 = 10;
	Weapons_Data[DND_WEAPON_DEMONSEALER].properties = WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_DEMONSEALER].attunement[STAT_DEX] = 0.01;
	Weapons_Data[DND_WEAPON_DEMONSEALER].attunement[STAT_INT] = 0.03;
	
	Weapons_Data[DND_WEAPON_TEMPLARMG].name = "ResMG1";
	Weapons_Data[DND_WEAPON_TEMPLARMG].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_TEMPLARMG].ammo_name2 = "MGClip4";
	Weapons_Data[DND_WEAPON_TEMPLARMG].icon = "WEPICO37";
	Weapons_Data[DND_WEAPON_TEMPLARMG].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_TEMPLARMG].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_TEMPLARMG].properties = WPROP_ALTAMMO | WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_TEMPLARMG].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_TEMPLARMG].attunement[STAT_DEX] = 0.03;
	
	Weapons_Data[DND_WEAPON_RIOTCANNON].name = "ResMG2";
	Weapons_Data[DND_WEAPON_RIOTCANNON].ammo_name1 = "RiotgunShell";
	Weapons_Data[DND_WEAPON_RIOTCANNON].ammo_name2 = "RiotgunClip";
	Weapons_Data[DND_WEAPON_RIOTCANNON].icon = "WEPICO38";
	Weapons_Data[DND_WEAPON_RIOTCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RIOTCANNON].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_RIOTCANNON].properties = WPROP_ALTAMMO | WPROP_SHOTGUN | WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_RIOTCANNON].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_RIOTCANNON].attunement[STAT_DEX] = 0.03;
	
	Weapons_Data[DND_WEAPON_ACIDRIFLE].name = "ResMG3";
	Weapons_Data[DND_WEAPON_ACIDRIFLE].ammo_name1 = "AcidAmmo";
	Weapons_Data[DND_WEAPON_ACIDRIFLE].ammo_name2 = "AcidClip";
	Weapons_Data[DND_WEAPON_ACIDRIFLE].icon = "WEPICO39";
	Weapons_Data[DND_WEAPON_ACIDRIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ACIDRIFLE].ammo_use2 = 16;
	Weapons_Data[DND_WEAPON_ACIDRIFLE].properties = WPROP_POISON | WPROP_TECH | WPROP_AUTOMATIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_ACIDRIFLE].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_ACIDRIFLE].attunement[STAT_DEX] = 0.02;
	Weapons_Data[DND_WEAPON_ACIDRIFLE].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].name = "ResMG4";
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].ammo_name1 = "FusionCell";
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].ammo_name2 = "MGClip6";
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].icon = "WEPICO40";
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].ammo_use2 = 20;
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].properties = WPROP_IGNORESHIELD | WPROP_RIPPER | WPROP_TECH | WPROP_AUTOMATIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].attunement[STAT_DEX] = 0.02;
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_INCINERATOR].name = "ResMG5";
	Weapons_Data[DND_WEAPON_INCINERATOR].ammo_name1 = "IncineratorAmmo";
	Weapons_Data[DND_WEAPON_INCINERATOR].ammo_name2 = "IncineratorStacks";
	Weapons_Data[DND_WEAPON_INCINERATOR].icon = "WEPICO94";
	Weapons_Data[DND_WEAPON_INCINERATOR].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_INCINERATOR].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_INCINERATOR].properties = WPROP_IGNORESHIELD | WPROP_RIPPER | WPROP_SELFDMG | WPROP_NOREFLECT | WPROP_TECH | WPROP_SHOTGUN | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_INCINERATOR].attunement[STAT_STR] = 0.015;
	Weapons_Data[DND_WEAPON_INCINERATOR].attunement[STAT_DEX] = 0.015;
	Weapons_Data[DND_WEAPON_INCINERATOR].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_DESOLATOR].name = "Desolator";
	Weapons_Data[DND_WEAPON_DESOLATOR].ammo_name1 = "DesolatorAmmo";
	Weapons_Data[DND_WEAPON_DESOLATOR].ammo_name2 = "DesolatorOverheat";
	Weapons_Data[DND_WEAPON_DESOLATOR].icon = "WEPICO41";
	Weapons_Data[DND_WEAPON_DESOLATOR].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DESOLATOR].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DESOLATOR].properties = WPROP_OVERHEAT | WPROP_POISON | WPROP_TECH | WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_DESOLATOR].attunement[STAT_STR] = 0.025;
	Weapons_Data[DND_WEAPON_DESOLATOR].attunement[STAT_DEX] = 0.015;
	
	Weapons_Data[DND_WEAPON_MINIGUN].name = " Minigun ";
	Weapons_Data[DND_WEAPON_MINIGUN].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_MINIGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_MINIGUN].icon = "WEPICO42";
	Weapons_Data[DND_WEAPON_MINIGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_MINIGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_MINIGUN].properties = WPROP_CANTHITGHOST | WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_MINIGUN].attunement[STAT_STR] = 0.03;
	Weapons_Data[DND_WEAPON_MINIGUN].attunement[STAT_DEX] = 0.01;
	
	Weapons_Data[DND_WEAPON_EBONYCANNON].name = "Ebony Cannon";
	Weapons_Data[DND_WEAPON_EBONYCANNON].ammo_name1 = "EbonyAmmo";
	Weapons_Data[DND_WEAPON_EBONYCANNON].ammo_name2 = "EbonyAmmoX";
	Weapons_Data[DND_WEAPON_EBONYCANNON].icon = "WEPICO43";
	Weapons_Data[DND_WEAPON_EBONYCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_EBONYCANNON].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_EBONYCANNON].properties = WPROP_IGNORESHIELD | WPROP_SELFDMG | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_EBONYCANNON].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_EBONYCANNON].attunement[STAT_INT] = 0.3;
	
	Weapons_Data[DND_WEAPON_MPPB].name = "MPPB";
	Weapons_Data[DND_WEAPON_MPPB].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_MPPB].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_MPPB].icon = "WEPICO95";
	Weapons_Data[DND_WEAPON_MPPB].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_MPPB].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_MPPB].properties = WPROP_RIPPER | WPROP_NOREFLECT | WPROP_CANTHITGHOST | WPROP_TECH | WPROP_AUTOMATIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_MPPB].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_MPPB].attunement[STAT_DEX] = 0.02;
	Weapons_Data[DND_WEAPON_MPPB].attunement[STAT_INT] = 0.01;
	
	// SLOT 5
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].name = "Rocket Launcher";
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].ammo_name1 = "RocketAmmo";
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].icon = "WEPICO44";
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].attunement[STAT_STR] = 0.02;
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].attunement[STAT_DEX] = 0.02;
	
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].name = "Upgraded Rocket Launcher";
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].ammo_name1 = "RocketAmmo";
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].icon = "WEPICO45";
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_NOREFLECT | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].attunement[STAT_STR] = 0.02;
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].attunement[STAT_DEX] = 0.02;
	
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].name = "Grenade Launcher";
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_name1 = "Grenades";
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].icon = "WEPICO51";
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_ALTAMMO | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].attunement[STAT_STR] = 0.015;
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].attunement[STAT_DEX] = 0.025;
	
	Weapons_Data[DND_WEAPON_VINDICATOR].name = "Upgraded Rocket Launcher3";
	Weapons_Data[DND_WEAPON_VINDICATOR].ammo_name1 = "FlakShell";
	Weapons_Data[DND_WEAPON_VINDICATOR].ammo_name2 = "ShellSize_18";
	Weapons_Data[DND_WEAPON_VINDICATOR].icon = "WEPICO47";
	Weapons_Data[DND_WEAPON_VINDICATOR].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_VINDICATOR].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_VINDICATOR].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_SHOTGUN;
	Weapons_Data[DND_WEAPON_VINDICATOR].attunement[STAT_STR] = 0.015;
	Weapons_Data[DND_WEAPON_VINDICATOR].attunement[STAT_DEX] = 0.025;

	Weapons_Data[DND_WEAPON_HAMMER].name = "TheHammer";
	Weapons_Data[DND_WEAPON_HAMMER].ammo_name1 = "HammerThrowStrength";
	Weapons_Data[DND_WEAPON_HAMMER].ammo_name2 = "HammerAmmo";
	Weapons_Data[DND_WEAPON_HAMMER].icon = "WEPICO98";
	Weapons_Data[DND_WEAPON_HAMMER].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_HAMMER].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_HAMMER].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT | WPROP_MELEE | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_HAMMER].attunement[STAT_STR] = 0.03;
	Weapons_Data[DND_WEAPON_HAMMER].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].name = "ResRL1";
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].ammo_name1 = "MeteorAmmo";
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].icon = "WEPICO48";
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].properties = WPROP_IGNORESHIELD | WPROP_SELFDMG | WPROP_ARTILLERY | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].attunement[STAT_INT] = 0.03;
	
	Weapons_Data[DND_WEAPON_HEAVYGL].name = "ResRL2";
	Weapons_Data[DND_WEAPON_HEAVYGL].ammo_name1 = "HeavyGrenades";
	Weapons_Data[DND_WEAPON_HEAVYGL].ammo_name2 = "HeavyGLCounter";
	Weapons_Data[DND_WEAPON_HEAVYGL].icon = "WEPICO49";
	Weapons_Data[DND_WEAPON_HEAVYGL].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HEAVYGL].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HEAVYGL].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_RIPPER | WPROP_TECH | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_HEAVYGL].attunement[STAT_STR] = 0.03;
	Weapons_Data[DND_WEAPON_HEAVYGL].attunement[STAT_DEX] = 0.01;
	
	Weapons_Data[DND_WEAPON_FREEZER].name = "ResRL3";
	Weapons_Data[DND_WEAPON_FREEZER].ammo_name1 = "EverIce";
	Weapons_Data[DND_WEAPON_FREEZER].ammo_name2 = "FreezerOverheat";
	Weapons_Data[DND_WEAPON_FREEZER].icon = "WEPICO50";
	Weapons_Data[DND_WEAPON_FREEZER].ammo_use1 = 10;
	Weapons_Data[DND_WEAPON_FREEZER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_FREEZER].properties = WPROP_RIPPER | WPROP_OVERHEAT | WPROP_TECH | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_FREEZER].attunement[STAT_STR] = 0.015;
	Weapons_Data[DND_WEAPON_FREEZER].attunement[STAT_INT] = 0.025;
	
	Weapons_Data[DND_WEAPON_GRAVDIS].name = "ResRL4";
	Weapons_Data[DND_WEAPON_GRAVDIS].ammo_name1 = "GravdisAmmo";
	Weapons_Data[DND_WEAPON_GRAVDIS].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_GRAVDIS].icon = "WEPICO91";
	Weapons_Data[DND_WEAPON_GRAVDIS].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_GRAVDIS].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_GRAVDIS].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT | WPROP_TECH | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_GRAVDIS].attunement[STAT_STR] = 0.03;
	Weapons_Data[DND_WEAPON_GRAVDIS].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_VOIDCANNON].name = "ResRL5";
	Weapons_Data[DND_WEAPON_VOIDCANNON].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_VOIDCANNON].ammo_name2 = "VoidCannonOverheat";
	Weapons_Data[DND_WEAPON_VOIDCANNON].icon = "WEPICO96";
	Weapons_Data[DND_WEAPON_VOIDCANNON].ammo_use1 = 30;
	Weapons_Data[DND_WEAPON_VOIDCANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_VOIDCANNON].properties = WPROP_RIPPER | WPROP_IGNORESHIELD | WPROP_NOREFLECT | WPROP_TECH | WPROP_OVERHEAT | WPROP_SELFDMG | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_VOIDCANNON].attunement[STAT_STR] = 0.025;
	Weapons_Data[DND_WEAPON_VOIDCANNON].attunement[STAT_INT] = 0.015;

	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].name = "Upgraded Rocket Launcher2";
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].ammo_name1 = "RocketAmmo";
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].icon = "WEPICO46";
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_ARTILLERY | WPROP_HOMINGSCREEN | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].attunement[STAT_STR] = 0.015;
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].attunement[STAT_DEX] = 0.025;
	
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].name = "Upgraded Grenade Launcher";
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].ammo_name1 = "Grenades";
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].icon = "WEPICO52";
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].attunement[STAT_STR] = 0.025;
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].attunement[STAT_DEX] = 0.015;
	
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].name = "Heavy Missile Launcher";
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].ammo_name1 = "MISAmmo";
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].icon = "WEPICO53";
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_TECH | WPROP_ARTILLERY | WPROP_HOMINGSCREEN | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].attunement[STAT_STR] = 0.03;
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].attunement[STAT_DEX] = 0.01;
	
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].name = "Sedrin Staff";
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].ammo_name1 = "SedrinAmmo";
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].icon = "WEPICO92";
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].properties = WPROP_SELFDMG | WPROP_POISON | WPROP_MAGIC | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].attunement[STAT_DEX] = 0.02;
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].attunement[STAT_INT] = 0.02;
	
	// SLOT 6
	Weapons_Data[DND_WEAPON_PLASMARIFLE].name = "Plasma Rifle";
	Weapons_Data[DND_WEAPON_PLASMARIFLE].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_PLASMARIFLE].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_PLASMARIFLE].icon = "WEPICO54";
	Weapons_Data[DND_WEAPON_PLASMARIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PLASMARIFLE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_PLASMARIFLE].properties = WPROP_TECH | WPROP_AUTOMATIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_PLASMARIFLE].attunement[STAT_DEX] = 0.03;
	Weapons_Data[DND_WEAPON_PLASMARIFLE].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].name = "Upgraded Plasma Rifle";
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].ammo_name2 = "PlasmaOverheat";
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].icon = "WEPICO55";
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].properties = WPROP_IGNORESHIELD | WPROP_OVERHEAT | WPROP_SELFDMG | WPROP_TECH | WPROP_AUTOMATIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].attunement[STAT_DEX] = 0.03;
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_TURELCANNON].name = "Upgraded Plasma Rifle2";
	Weapons_Data[DND_WEAPON_TURELCANNON].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_TURELCANNON].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_TURELCANNON].icon = "WEPICO56";
	Weapons_Data[DND_WEAPON_TURELCANNON].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_TURELCANNON].ammo_use2 = 4;
	Weapons_Data[DND_WEAPON_TURELCANNON].properties = WPROP_RIPPER | WPROP_TECH | WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_TURELCANNON].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_TURELCANNON].attunement[STAT_DEX] = 0.02;
	Weapons_Data[DND_WEAPON_TURELCANNON].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_FROSTFANG].name = "Upgraded Plasma Rifle3";
	Weapons_Data[DND_WEAPON_FROSTFANG].ammo_name1 = "EverIce";
	Weapons_Data[DND_WEAPON_FROSTFANG].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_FROSTFANG].icon = "WEPICO57";
	Weapons_Data[DND_WEAPON_FROSTFANG].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_FROSTFANG].ammo_use2 = 30;
	Weapons_Data[DND_WEAPON_FROSTFANG].properties = WPROP_RIPPER | WPROP_CANTHITGHOST | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_FROSTFANG].attunement[STAT_DEX] = 0.01;
	Weapons_Data[DND_WEAPON_FROSTFANG].attunement[STAT_INT] = 0.03;

	Weapons_Data[DND_WEAPON_SNIPER].name = "Upgraded Plasma Rifle4";
	Weapons_Data[DND_WEAPON_SNIPER].ammo_name1 = "SniperRounds";
	Weapons_Data[DND_WEAPON_SNIPER].ammo_name2 = "SniperZoomTimer";
	Weapons_Data[DND_WEAPON_SNIPER].icon = "WEPIC100";
	Weapons_Data[DND_WEAPON_SNIPER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SNIPER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SNIPER].properties = WPROP_IGNORESHIELD | WPROP_TECH | WPROP_PRECISION;
	Weapons_Data[DND_WEAPON_SNIPER].attunement[STAT_STR] = 0.015;
	Weapons_Data[DND_WEAPON_SNIPER].attunement[STAT_DEX] = 0.025;
	
	Weapons_Data[DND_WEAPON_FLAMETHROWER].name = "ResPlasma1";
	Weapons_Data[DND_WEAPON_FLAMETHROWER].ammo_name1 = "Fuel";
	Weapons_Data[DND_WEAPON_FLAMETHROWER].ammo_name2 = "FuelClip";
	Weapons_Data[DND_WEAPON_FLAMETHROWER].icon = "WEPICO58";
	Weapons_Data[DND_WEAPON_FLAMETHROWER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_FLAMETHROWER].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_FLAMETHROWER].properties = WPROP_SELFDMG | WPROP_TECH | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_FLAMETHROWER].attunement[STAT_DEX] = 0.03;
	Weapons_Data[DND_WEAPON_FLAMETHROWER].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].name = "ResPlasma2";
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].ammo_name1 = "LightningCell";
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].ammo_name2 = "LightningStacks";
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].icon = "WEPICO59";
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].properties = WPROP_TECH | WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].attunement[STAT_DEX] = 0.03;
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_REBOUNDER].name = "ResPlasma3";
	Weapons_Data[DND_WEAPON_REBOUNDER].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_REBOUNDER].ammo_name2 = "RebounderOverheat";
	Weapons_Data[DND_WEAPON_REBOUNDER].icon = "WEPICO60";
	Weapons_Data[DND_WEAPON_REBOUNDER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_REBOUNDER].ammo_use2 = 20;
	Weapons_Data[DND_WEAPON_REBOUNDER].properties = WPROP_OVERHEAT | WPROP_NOREFLECT | WPROP_TECH | WPROP_AUTOMATIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_REBOUNDER].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_REBOUNDER].attunement[STAT_DEX] = 0.02;
	Weapons_Data[DND_WEAPON_REBOUNDER].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_DARKLANCE].name = "ResPlasma4";
	Weapons_Data[DND_WEAPON_DARKLANCE].ammo_name1 = "DemonBlood";
	Weapons_Data[DND_WEAPON_DARKLANCE].ammo_name2 = "LanceStacks";
	Weapons_Data[DND_WEAPON_DARKLANCE].icon = "WEPICO97";
	Weapons_Data[DND_WEAPON_DARKLANCE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DARKLANCE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DARKLANCE].properties = WPROP_NOREFLECT | WPROP_IGNORESHIELD | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_DARKLANCE].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_DARKLANCE].attunement[STAT_INT] = 0.03;
	
	Weapons_Data[DND_WEAPON_RHINORIFLE].name = "RhinoRifle";
	Weapons_Data[DND_WEAPON_RHINORIFLE].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_RHINORIFLE].ammo_name2 = "MGClip7";
	Weapons_Data[DND_WEAPON_RHINORIFLE].icon = "WEPICO61";
	Weapons_Data[DND_WEAPON_RHINORIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RHINORIFLE].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_RHINORIFLE].properties = WPROP_ALTAMMO | WPROP_PRECISION | WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_RHINORIFLE].attunement[STAT_STR] = 0.02;
	Weapons_Data[DND_WEAPON_RHINORIFLE].attunement[STAT_DEX] = 0.02;
	
	Weapons_Data[DND_WEAPON_NAILGUN].name = "Nailgun";
	Weapons_Data[DND_WEAPON_NAILGUN].ammo_name1 = "NailgunAmmo";
	Weapons_Data[DND_WEAPON_NAILGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_NAILGUN].icon = "WEPICO62";
	Weapons_Data[DND_WEAPON_NAILGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_NAILGUN].ammo_use2 = 2;
	Weapons_Data[DND_WEAPON_NAILGUN].properties = WPROP_RIPPER | WPROP_CANTHITGHOST | WPROP_NOREFLECT | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_NAILGUN].attunement[STAT_DEX] = 0.02;
	Weapons_Data[DND_WEAPON_NAILGUN].attunement[STAT_INT] = 0.02;
	
	Weapons_Data[DND_WEAPON_BASILISK].name = "Basilisk";
	Weapons_Data[DND_WEAPON_BASILISK].ammo_name1 = "BasiliskAmmo";
	Weapons_Data[DND_WEAPON_BASILISK].ammo_name2 = "LoadedBasilisk";
	Weapons_Data[DND_WEAPON_BASILISK].icon = "WEPICO63";
	Weapons_Data[DND_WEAPON_BASILISK].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_BASILISK].ammo_use2 = 8;
	Weapons_Data[DND_WEAPON_BASILISK].properties = WPROP_IGNORESHIELD | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_BASILISK].attunement[STAT_STR] = 0.02;
	Weapons_Data[DND_WEAPON_BASILISK].attunement[STAT_INT] = 0.02;
	
	// SLOT 7
	Weapons_Data[DND_WEAPON_BFG6000].name = "BFG 9000";
	Weapons_Data[DND_WEAPON_BFG6000].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_BFG6000].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_BFG6000].icon = "WEPICO64";
	Weapons_Data[DND_WEAPON_BFG6000].ammo_use1 = 50;
	Weapons_Data[DND_WEAPON_BFG6000].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_BFG6000].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT | WPROP_TECH | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_BFG6000].attunement[STAT_STR] = 0.02;
	Weapons_Data[DND_WEAPON_BFG6000].attunement[STAT_DEX] = 0.01;
	Weapons_Data[DND_WEAPON_BFG6000].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_BFG32768].name = "Upgraded BFG 9000";
	Weapons_Data[DND_WEAPON_BFG32768].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_BFG32768].ammo_name2 = "BFG32768Overheat";
	Weapons_Data[DND_WEAPON_BFG32768].icon = "WEPICO65";
	Weapons_Data[DND_WEAPON_BFG32768].ammo_use1 = 75;
	Weapons_Data[DND_WEAPON_BFG32768].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_BFG32768].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT | WPROP_TECH | WPROP_OVERHEAT | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_BFG32768].attunement[STAT_STR] = 0.02;
	Weapons_Data[DND_WEAPON_BFG32768].attunement[STAT_DEX] = 0.01;
	Weapons_Data[DND_WEAPON_BFG32768].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_DEVASTATOR].name = "Devastator";
	Weapons_Data[DND_WEAPON_DEVASTATOR].ammo_name1 = "DevastatorAmmo";
	Weapons_Data[DND_WEAPON_DEVASTATOR].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DEVASTATOR].icon = "WEPICO66";
	Weapons_Data[DND_WEAPON_DEVASTATOR].ammo_use1 = 5;
	Weapons_Data[DND_WEAPON_DEVASTATOR].ammo_use2 = 60;
	Weapons_Data[DND_WEAPON_DEVASTATOR].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_CANTHITGHOST | WPROP_NOREFLECT | WPROP_TECH | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_DEVASTATOR].attunement[STAT_STR] = 0.015;
	Weapons_Data[DND_WEAPON_DEVASTATOR].attunement[STAT_INT] = 0.01;
	Weapons_Data[DND_WEAPON_DEVASTATOR].attunement[STAT_DEX] = 0.015;
	
	Weapons_Data[DND_WEAPON_MFG].name = "MFG";
	Weapons_Data[DND_WEAPON_MFG].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_MFG].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_MFG].icon = "WEPICO67";
	Weapons_Data[DND_WEAPON_MFG].ammo_use1 = 60;
	Weapons_Data[DND_WEAPON_MFG].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_MFG].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_NOREFLECT | WPROP_TECH | WPROP_ARTILLERY | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_MFG].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_MFG].attunement[STAT_INT] = 0.01;
	Weapons_Data[DND_WEAPON_MFG].attunement[STAT_DEX] = 0.02;
	
	Weapons_Data[DND_WEAPON_IONCANNON].name = "ResBFG1";
	Weapons_Data[DND_WEAPON_IONCANNON].ammo_name1 = "IonAmmo";
	Weapons_Data[DND_WEAPON_IONCANNON].ammo_name2 = "IonOverHeat";
	Weapons_Data[DND_WEAPON_IONCANNON].icon = "WEPICO68";
	Weapons_Data[DND_WEAPON_IONCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_IONCANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_IONCANNON].properties = WPROP_OVERHEAT | WPROP_IGNORESHIELD | WPROP_CANTHITGHOST | WPROP_TECH | WPROP_ARTILLERY;
	Weapons_Data[DND_WEAPON_IONCANNON].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_IONCANNON].attunement[STAT_INT] = 0.01;
	Weapons_Data[DND_WEAPON_IONCANNON].attunement[STAT_DEX] = 0.02;
	
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].name = "ResBFG2";
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].ammo_name1 = "ThunderAmmo";
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].ammo_name2 = "ThunderRangeCounter";
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].icon = "WEPICO69";
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].ammo_use1 = 20;
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].ammo_use2 = 40;
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].attunement[STAT_INT] = 0.03;
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].attunement[STAT_DEX] = 0.01;
	
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].name = "Gauss Rifle";
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].ammo_name1 = "GaussRound";
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].icon = "WEPICO70";
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_TECH | WPROP_PRECISION;
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].attunement[STAT_DEX] = 0.03;
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_RAILGUN].name = "Rail gun";
	Weapons_Data[DND_WEAPON_RAILGUN].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_RAILGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_RAILGUN].icon = "WEPICO71";
	Weapons_Data[DND_WEAPON_RAILGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RAILGUN].ammo_use2 = 8;
	Weapons_Data[DND_WEAPON_RAILGUN].properties = WPROP_IGNORESHIELD | WPROP_RIPPER | WPROP_TECH | WPROP_PRECISION;
	Weapons_Data[DND_WEAPON_RAILGUN].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_RAILGUN].attunement[STAT_DEX] = 0.02;
	Weapons_Data[DND_WEAPON_RAILGUN].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_DEATHRAY].name = "Death Ray";
	Weapons_Data[DND_WEAPON_DEATHRAY].ammo_name1 = "IonAmmo";
	Weapons_Data[DND_WEAPON_DEATHRAY].ammo_name2 = "DeathrayOverheat";
	Weapons_Data[DND_WEAPON_DEATHRAY].icon = "WEPICO88";
	Weapons_Data[DND_WEAPON_DEATHRAY].ammo_use1 = 6;
	Weapons_Data[DND_WEAPON_DEATHRAY].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DEATHRAY].properties = WPROP_IGNORESHIELD | WPROP_OVERHEAT | WPROP_TECH | WPROP_PRECISION;
	Weapons_Data[DND_WEAPON_DEATHRAY].attunement[STAT_DEX] = 0.01;
	Weapons_Data[DND_WEAPON_DEATHRAY].attunement[STAT_INT] = 0.03;
	
	// SLOT 8
	Weapons_Data[DND_WEAPON_DEATHSTAFF].name = "Death Staff";
	Weapons_Data[DND_WEAPON_DEATHSTAFF].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_DEATHSTAFF].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DEATHSTAFF].icon = "WEPICO72";
	Weapons_Data[DND_WEAPON_DEATHSTAFF].ammo_use1 = 4;
	Weapons_Data[DND_WEAPON_DEATHSTAFF].ammo_use2 = 10;
	Weapons_Data[DND_WEAPON_DEATHSTAFF].properties = WPROP_IGNORESHIELD | WPROP_SELFDMG | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_DEATHSTAFF].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_DEATHSTAFF].attunement[STAT_INT] = 0.03;
	
	Weapons_Data[DND_WEAPON_RAZORFANG].name = "Razorfang";
	Weapons_Data[DND_WEAPON_RAZORFANG].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_RAZORFANG].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_RAZORFANG].icon = "WEPICO73";
	Weapons_Data[DND_WEAPON_RAZORFANG].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RAZORFANG].ammo_use2 = 4;
	Weapons_Data[DND_WEAPON_RAZORFANG].properties = WPROP_RIPPER | WPROP_HOMINGSCREEN | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_RAZORFANG].attunement[STAT_DEX] = 0.01;
	Weapons_Data[DND_WEAPON_RAZORFANG].attunement[STAT_INT] = 0.03;
	
	Weapons_Data[DND_WEAPON_SUNSTAFF].name = "Sun Staff";
	Weapons_Data[DND_WEAPON_SUNSTAFF].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_SUNSTAFF].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SUNSTAFF].icon = "WEPICO74";
	Weapons_Data[DND_WEAPON_SUNSTAFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SUNSTAFF].ammo_use2 = 5;
	Weapons_Data[DND_WEAPON_SUNSTAFF].properties = WPROP_RIPPER | WPROP_MAGIC;
	Weapons_Data[DND_WEAPON_SUNSTAFF].attunement[STAT_DEX] = 0.01;
	Weapons_Data[DND_WEAPON_SUNSTAFF].attunement[STAT_INT] = 0.03;
	
	Weapons_Data[DND_WEAPON_SOULREAVER].name = "Soul Reaver";
	Weapons_Data[DND_WEAPON_SOULREAVER].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_SOULREAVER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SOULREAVER].icon = "WEPICO75";
	Weapons_Data[DND_WEAPON_SOULREAVER].ammo_use1 = 75;
	Weapons_Data[DND_WEAPON_SOULREAVER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SOULREAVER].properties = WPROP_IGNORESHIELD | WPROP_RIPPER | WPROP_MAGIC;
	Weapons_Data[DND_WEAPON_SOULREAVER].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_SOULREAVER].attunement[STAT_DEX] = 0.01;
	Weapons_Data[DND_WEAPON_SOULREAVER].attunement[STAT_INT] = 0.02;
	
	// TEMP WEPS
	Weapons_Data[DND_WEAPON_SAWEDOFF].name = "Sawedoff";
	Weapons_Data[DND_WEAPON_SAWEDOFF].ammo_name1 = "SawedoffShell";
	Weapons_Data[DND_WEAPON_SAWEDOFF].ammo_name2 = "SawedoffCounter";
	Weapons_Data[DND_WEAPON_SAWEDOFF].icon = "WEPICO76";
	Weapons_Data[DND_WEAPON_SAWEDOFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SAWEDOFF].ammo_use2 = 2;
	Weapons_Data[DND_WEAPON_SAWEDOFF].properties = WPROP_SHOTGUN;
	Weapons_Data[DND_WEAPON_SAWEDOFF].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_SAWEDOFF].attunement[STAT_DEX] = 0.03;
	
	Weapons_Data[DND_WEAPON_SOULRENDER].name = "Soul Render";
	Weapons_Data[DND_WEAPON_SOULRENDER].ammo_name1 = "BladeHits";
	Weapons_Data[DND_WEAPON_SOULRENDER].ammo_name2 = "BladeCharge";
	Weapons_Data[DND_WEAPON_SOULRENDER].icon = "WEPICO77";
	Weapons_Data[DND_WEAPON_SOULRENDER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SOULRENDER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SOULRENDER].properties = WPROP_RIPPER | WPROP_IGNORESHIELD | WPROP_MAGIC | WPROP_MELEE | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_SOULRENDER].attunement[STAT_STR] = 0.02;
	Weapons_Data[DND_WEAPON_SOULRENDER].attunement[STAT_DEX] = 0.01;
	Weapons_Data[DND_WEAPON_SOULRENDER].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_SMG].name = "SMG";
	Weapons_Data[DND_WEAPON_SMG].ammo_name1 = "SMGAmmo";
	Weapons_Data[DND_WEAPON_SMG].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SMG].icon = "WEPICO78";
	Weapons_Data[DND_WEAPON_SMG].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SMG].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SMG].properties = WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_SMG].attunement[STAT_DEX] = 0.04;
	
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].name = "Hellforge Cannon";
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].ammo_name1 = "IronBalls";
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].ammo_name2 = "IronBalls";
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].icon = "WEPICO79";
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].ammo_use2 = 3;
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].properties = WPROP_RIPPER | WPROP_IGNORESHIELD | WPROP_ARTILLERY | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].attunement[STAT_STR] = 0.02;
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].attunement[STAT_INT] = 0.0;
	
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].name = "Bloodfiend Spine";
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].ammo_name1 = "BloodAmmo";
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].ammo_name2 = "BloodAmmo";
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].icon = "WEPICO80";
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].properties = WPROP_CANTHITGHOST | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].attunement[STAT_INT] = 0.03;

	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].name = "Enforcer Rifle";
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].ammo_name1 = "LaserAmmo";
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].icon = "WEPICO81";
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].ammo_use2 = 2;
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].properties = WPROP_IGNORESHIELD | WPROP_TECH | WPROP_AUTOMATIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].attunement[STAT_DEX] = 0.03;
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_VENOM].name = "Venom";
	Weapons_Data[DND_WEAPON_VENOM].ammo_name1 = "VenomAmmo";
	Weapons_Data[DND_WEAPON_VENOM].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_VENOM].icon = "WEPICO82";
	Weapons_Data[DND_WEAPON_VENOM].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_VENOM].ammo_use2 = 3;
	Weapons_Data[DND_WEAPON_VENOM].properties = WPROP_RIPPER | WPROP_CANTHITGHOST | WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_VENOM].attunement[STAT_DEX] = 0.01;
	Weapons_Data[DND_WEAPON_VENOM].attunement[STAT_INT] = 0.03;
	
	Weapons_Data[DND_WEAPON_DEMONHEART].name = "Demon Heart";
	Weapons_Data[DND_WEAPON_DEMONHEART].ammo_name1 = "HeartAmmo";
	Weapons_Data[DND_WEAPON_DEMONHEART].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DEMONHEART].icon = "WEPICO83";
	Weapons_Data[DND_WEAPON_DEMONHEART].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEMONHEART].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DEMONHEART].properties = WPROP_IGNORESHIELD | WPROP_MAGIC;
	Weapons_Data[DND_WEAPON_DEMONHEART].attunement[STAT_INT] = 0.04;
	
	Weapons_Data[DND_WEAPON_DARKGLOVES].name = "DarkServantGloves";
	Weapons_Data[DND_WEAPON_DARKGLOVES].ammo_name1 = "DarkServantEnergy";
	Weapons_Data[DND_WEAPON_DARKGLOVES].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DARKGLOVES].icon = "WEPICO84";
	Weapons_Data[DND_WEAPON_DARKGLOVES].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DARKGLOVES].ammo_use2 = 4;
	Weapons_Data[DND_WEAPON_DARKGLOVES].properties = WPROP_MAGIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_DARKGLOVES].attunement[STAT_STR] = 0.01;
	Weapons_Data[DND_WEAPON_DARKGLOVES].attunement[STAT_INT] = 0.03;
	
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].name = "Nailgun2";
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].ammo_name1 = "BigNail";
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].icon = "WEPICO85";
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].properties = WPROP_RIPPER | WPROP_AUTOMATIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].attunement[STAT_STR] = 0.03;
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].attunement[STAT_DEX] = 0.01;
	
	Weapons_Data[DND_WEAPON_BERETTAS].name = "Berettas";
	Weapons_Data[DND_WEAPON_BERETTAS].ammo_name1 = "BerettaAmmo";
	Weapons_Data[DND_WEAPON_BERETTAS].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_BERETTAS].icon = "WEPICO86";
	Weapons_Data[DND_WEAPON_BERETTAS].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_BERETTAS].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_BERETTAS].properties = WPROP_HANDGUN;
	Weapons_Data[DND_WEAPON_BERETTAS].attunement[STAT_DEX] = 0.04;
	
	Weapons_Data[DND_WEAPON_PLASMABOLTER].name = "Plasma Bolter";
	Weapons_Data[DND_WEAPON_PLASMABOLTER].ammo_name1 = "PlasmaBolterAmmo";
	Weapons_Data[DND_WEAPON_PLASMABOLTER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_PLASMABOLTER].icon = "WEPICO87";
	Weapons_Data[DND_WEAPON_PLASMABOLTER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PLASMABOLTER].ammo_use2 = 5;
	Weapons_Data[DND_WEAPON_PLASMABOLTER].properties = WPROP_RIPPER | WPROP_NOREFLECT | WPROP_TECH | WPROP_AUTOMATIC | WPROP_FIRESPROJECTILES;
	Weapons_Data[DND_WEAPON_PLASMABOLTER].attunement[STAT_DEX] = 0.03;
	Weapons_Data[DND_WEAPON_PLASMABOLTER].attunement[STAT_INT] = 0.01;
	
	Weapons_Data[DND_WEAPON_RIPPERCANNON].name = "Ripper Cannon";
	Weapons_Data[DND_WEAPON_RIPPERCANNON].ammo_name1 = "RipperCannonAmmo";
	Weapons_Data[DND_WEAPON_RIPPERCANNON].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_RIPPERCANNON].icon = "WEPICO89";
	Weapons_Data[DND_WEAPON_RIPPERCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RIPPERCANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_RIPPERCANNON].properties = WPROP_RIPPER | WPROP_AUTOMATIC;
	Weapons_Data[DND_WEAPON_RIPPERCANNON].attunement[STAT_STR] = 0.025;
	Weapons_Data[DND_WEAPON_RIPPERCANNON].attunement[STAT_DEX] = 0.015;

	// special weapons
	Weapons_Data[DND_WEAPON_ASCENSION].name = "Ethereal Form";
	Weapons_Data[DND_WEAPON_ASCENSION].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_ASCENSION].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_ASCENSION].icon = "TNT1A0";
	Weapons_Data[DND_WEAPON_ASCENSION].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_ASCENSION].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_ASCENSION].properties = 0;
	Weapons_Data[DND_WEAPON_ASCENSION].attunement[STAT_INT] = 0.04;
}

#define DEMONSEAL_DMGTAKEN_DEBUFF 50 // 50%

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

bool IsSlot8Weapon(int wepid) {
	return wepid >= FIRST_SLOT8_WEAPON && wepid <= LAST_SLOT8_WEAPON;
}

bool IsWeaponFiringProjectiles(int wepid) {
	return !!(Weapons_Data[wepid].properties & WPROP_FIRESPROJECTILES);
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
	DND_OVERHEAT_BFG32768,
	DND_OVERHEAT_ION,
	DND_OVERHEAT_REBOUNDER,
	DND_OVERHEAT_DESOLATOR,
	DND_OVERHEAT_DEATHRAY,
	DND_OVERHEAT_VOIDCANNON,

	DND_OVERHEAT_LAST
};

#define WEAPON_OVERHEATID 0
#define WEAPON_OVERHEATCOOLDOWNID 1
#define MAXOVERHEATWEPS DND_OVERHEAT_LAST
str WeaponOverheatItems[MAXOVERHEATWEPS][MAXOVERHEATWEPS] = {
	{ "ShockerOverheat", "" },
	{ "FreezerOverheat", "FreezerCooldown" },
	{ "PlasmaOverheat", "PlasmaOverheatCooldown" },
	{ "BFG32768Overheat", "BFG32768Cooldown" },
	{ "IonOverheat", "IonCooldown" },
	{ "RebounderOverheat", "RebounderCooldown" },
	{ "DesolatorOverheat", "DesolatorCooldown" },
	{ "DeathRayOverheat", "DeathrayCooldown" },
	{ "VoidCannonOverheat", "VoidCannonCooldown" }
};

// 32 overheat weapons supported
int PlayerRunsOverheat[MAXPLAYERS] = { 0 };

void ResetAllWeaponMods(int pnum) {
	int i, j;
	for(j = 0; j < MAXWEPS; ++j) {
		Player_Weapon_Infos[pnum][j].quality = 0;
		Player_Weapon_Infos[pnum][j].needs_save = false;
		for(i = 0; i < MAX_WEP_MODS; ++i) {
			Player_Weapon_Infos[pnum][j].wep_mods[i][WMOD_ITEMS].tier = 0;
			Player_Weapon_Infos[pnum][j].wep_mods[i][WMOD_ITEMS].val = 0;

			Player_Weapon_Infos[pnum][j].wep_mods[i][WMOD_WEP].tier = 0;
			Player_Weapon_Infos[pnum][j].wep_mods[i][WMOD_WEP].val = 0;
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

int GetCurrentWeaponID() {
	return CheckInventory("DnD_WeaponID");
}

int GetActorWeaponID(int actor) {
	return CheckActorInventory(actor, "DnD_WeaponID");
}

int PickRandomOwnedWeaponID(int pnum) {
	int count = 0, i;
	static int owned_weapons[MAXPLAYERS][MAXWEPS];
	// setup the list
	for(i = 0; i < MAXWEPS; ++i) {
		if(CheckInventory(Weapons_Data[i].name)) {
			owned_weapons[pnum][count] = i;
			++count;
		}
	}

	// pick from random weapons owned
	if(count)
		return owned_weapons[pnum][random(0, count - 1)];
	return 0;
}

int PickRandomOwnedWeaponID_WithProj(int pnum) {
	int count = 0, i;
	static int owned_weapons[MAXPLAYERS][MAXWEPS];
	// setup the list
	for(i = 0; i < MAXWEPS; ++i) {
		if(CheckInventory(Weapons_Data[i].name) && IsWeaponFiringProjectiles(i)) {
			owned_weapons[pnum][count] = i;
			++count;
		}
	}

	// pick from random weapons owned
	if(count)
		return owned_weapons[pnum][random(0, count - 1)];
	return 0;
}

str GetWeaponAmmoType(int wepid, int which) {
	if(!which)
		return Weapons_Data[wepid].ammo_name1;
	return Weapons_Data[wepid].ammo_name2;
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

bool IsWeaponLightningType(int wepid) {
	switch(wepid) {
		case DND_WEAPON_SHOCKER:
		case DND_WEAPON_LIGHTNINGGUN:
		case DND_WEAPON_THUNDERSTAFF:
		return true;
	}
	return false;
	// SSAM_SHOCK == 2... THIS IS TERRIBLE, UNTANGLE DEPENDENCY HERE LATER!
	//return isSpecial && extra == 2;
}

bool IsSoulDroppingWeapon(int wepid) {
	switch(wepid) {
		case DND_WEAPON_INFERNOSWORD:
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

bool IsTechWeapon(int wepid) {
	return Weapons_Data[wepid].properties & WPROP_TECH;
}

bool IsHandgun(int wepid) {
	return Weapons_Data[wepid].properties & WPROP_HANDGUN;
}

bool IsBoomstick(int id) {
	return Weapons_Data[id].properties & WPROP_SHOTGUN;
}

bool IsAutomaticWeapon(int wepid) {
	return Weapons_Data[wepid].properties & WPROP_AUTOMATIC;
}

bool IsMeleeWeapon(int wepid) {
	return Weapons_Data[wepid].properties & WPROP_MELEE;
}

bool IsPrecisionWeapon(int wepid) {
	return Weapons_Data[wepid].properties & WPROP_PRECISION;
}

bool IsArtilleryWeapon(int wepid) {
	return Weapons_Data[wepid].properties & WPROP_ARTILLERY;
}

bool IsMagicalWeapon(int wepid) {
	return Weapons_Data[wepid].properties & WPROP_MAGIC;
}

int IsUsingMeleeWeapon() {
	int curwepid = GetCurrentWeaponID();
	return Weapons_Data[curwepid].properties & WPROP_MELEE;
}

bool IsHomingHudWeapon(int wepid) {
	return Weapons_Data[wepid].properties & WPROP_HOMINGSCREEN;
}

str GetWeaponTipText(int wepid) {
	int slot = GetSlotOfWeapon(wepid);
	int subid = GetWeaponOrderSlot(wepid, slot);
	str res = "";

	int tmp = Weapons_Data[wepid].attunement[STAT_STR];
	if(tmp)
		res = StrParam(l:"DND_MENU_ATTR_SHORT1", s: ": \cd", s:GetFixedRepresentation(tmp, true), s: "% ");

	tmp = Weapons_Data[wepid].attunement[STAT_DEX];
	if(tmp)
		res = StrParam(s:res, l:"DND_MENU_ATTR_SHORT2", s: ": \cd", s:GetFixedRepresentation(tmp, true), s: "% ");

	tmp = Weapons_Data[wepid].attunement[STAT_INT];
	if(tmp)
		res = StrParam(s:res, l:"DND_MENU_ATTR_SHORT3", s: ": \cd", s:GetFixedRepresentation(tmp, true), s: "%");

	return StrParam(s:res, s:"\n", l:StrParam(s:"WEPTIP_", d:slot, s:"_", d:subid));
}

str GetWeaponTag(int wepid) {
	int slot = GetSlotOfWeapon(wepid);
	int subid = GetWeaponOrderSlot(wepid, slot);
	return StrParam(s:"WEP_", d:slot, s:"_", d:subid, s:"_TAG");
}

void GiveOverheat(int pnum, str item, int amt, int wepid) {
	if((Weapons_Data[wepid].properties & WPROP_TECH) && HasClassPerk_Fast("Cyborg", 1))
		amt -= amt * 3 / 10;

	int reduce = GetPlayerAttributeValue(pnum, INV_REDUCED_OVERHEAT);
	if(reduce)
		amt = amt * (100 - reduce) / 100;

	// let players attain maximum overheat reduction
	if(amt > 0)
		GiveInventory(item, amt);
}

void HandleOverheating(int pnum, str overheatPercentItem, str overheatCooldownItem) {
	if(CheckInventory(overheatPercentItem) >= 100)
		GiveInventory(overheatCooldownItem, 1);
}

bool HasRunningOverheatCooldown(int p_tid) {
	for(int i = 0; i < MAXOVERHEATWEPS; ++i)
		if(WeaponOverheatItems[i][WEAPON_OVERHEATCOOLDOWNID] != "" && CheckActorInventory(p_tid, WeaponOverheatItems[i][WEAPON_OVERHEATCOOLDOWNID]))
			return true;
	return false;
}

bool CanWeaponOverheat(int id) {
	return Weapons_Data[id].properties & WPROP_OVERHEAT;
}

// TODO: If this ends up lagging the server, store player owned weapons in a specific array per weapon slot and pull data from there
void GiveOwnedWeaponsAmmo(int pct) {
	for(int i = 0; i < MAXWEPS; ++i) {
		if(!IsMeleeWeapon(i) && CheckInventory(Weapons_Data[i].name)) {
			str ammo = Weapons_Data[i].ammo_name1;
			int amt = 0;
			if(ammo != "") {
				amt = GetAmmoCapacity(ammo) * pct / 100;
				GiveInventory(ammo, amt);
			}
			
			ammo = Weapons_Data[i].ammo_name2;
			if(ammo != "") {
				amt = GetAmmoCapacity(ammo) * pct / 100;
				GiveInventory(ammo, amt);
			}
		}
	}
}

#endif
