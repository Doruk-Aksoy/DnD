#ifndef DND_WEAPONDEFS_IN
#define DND_WEAPONDEFS_IN

#include "DnD_Common.h"
#include "DnD_WeaponMods.h"

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
	DND_WEAPON_ASSAULTRIFLE,
	DND_WEAPON_VIPERSTAFF,
	DND_WEAPON_RUBYWAND,
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
	DND_WEAPON_MERCURYLAUNCHER,
	DND_WEAPON_VINDICATOR,
	DND_WEAPON_METEORLAUNCHER,
	DND_WEAPON_HEAVYGL,
	DND_WEAPON_FREEZER,
	DND_WEAPON_GRAVDIS,
	DND_WEAPON_VOIDCANNON,
	
	DND_WEAPON_GRENADELAUNCHER,
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

#define FIRST_SLOT0_WEAPON DND_WEAPON_FIST
#define LAST_SLOT0_WEAPON DND_WEAPON_SICKLE

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
#define MAXWEPS (DND_WEAPON_RIPPERCANNON + 1)
#define MAXWEPQUALITY 50
#define MAXWEPSLOTS 9

enum {  
	WEAPON_NAME,
	WEAPON_AMMO1,
	WEAPON_AMMO2,
	WEAPON_ICON,
	WEAPON_ICON64
};

// store players mods etc.
typedef struct {
	int quality;
	wep_mod_T wep_mods[MAX_WEP_MODS];
} wep_info_T;

global wep_info_T 2: Player_Weapon_Infos[MAXPLAYERS][MAXWEPS];

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
	28,
	3,
	75
};

#endif
