#ifndef DND_PROJ_IN
#define DND_PROJ_IN

#define HITSCAN_RANGE_DEFAULT 8192.0

enum {
	DND_PROJ_HITSCAN 					= 0b1,
	DND_PROJ_HASGHOSTHITTER				= 0b10,
	DND_PROJ_ISMINION					= 0b100,
	DND_PROJ_MELEE						= 0b1000,
	DND_PROJ_MELEEBONUSES				= 0b10000,				// melee bonuses apply (range etc)
};

enum {
	DND_PROJ_SPECIAL_DELAYEDFIRE		= 0b1,
};

typedef struct proj_data {
	int flags;
	int spd_range;
	str name;
} proj_data_T;

enum {
	DND_PROJ_FIST,
	DND_PROJ_CHAINSAW,
	DND_PROJ_DOUBLECHAINSAW,
	DND_PROJ_SICKLE,
	DND_PROJ_EXCALIBAT,
	DND_PROJ_EXCALIBAT2,
	DND_PROJ_KATANA,
	DND_PROJ_KATANA2,
	DND_PROJ_DUSKBLADE1,
	DND_PROJ_DUSKBLADE2,
	DND_PROJ_INFERNOSWORD1,
	DND_PROJ_INFERNOSWORD2,

	DND_PROJ_PISTOL,
	DND_PROJ_AKIMBO,
	DND_PROJ_MAGNUMREVOLVER,
	DND_PROJ_LASERPISTOL,
	DND_PROJ_SCATTERGUN,
	DND_PROJ_RUBYWAND,
	DND_PROJ_RUBYWANDFLAME,
	DND_PROJ_ASSAULTRIFLE,
	DND_PROJ_VIPERCRAWLER,
	DND_PROJ_VIPERMISSILE,
	
	DND_PROJ_SHOTGUN,
	DND_PROJ_PURIFIER,
	DND_PROJ_KILLSTORM,
	DND_PROJ_EMERALDMAIN,
	DND_PROJ_EMERALDRAINER,
	DND_PROJ_SSG,
	DND_PROJ_HEAVYSSG,
	DND_PROJ_ERASUS,
	DND_PROJ_HELLSMAWMAIN,
	DND_PROJ_HELLSMAWMINI,
	DND_PROJ_WHITEDEATH,
	DND_PROJ_SLAYER,
	DND_PROJ_DEADLOCK,
	DND_PROJ_WHEELOFTORMENT_1,
	DND_PROJ_WHEELOFTORMENT_2,
	DND_PROJ_WHEELOFTORMENT_3,
	DND_PROJ_CHARON,
	DND_PROJ_PLASMACANNON,
	DND_PROJ_SHOCKER,
	DND_PROJ_HADES,
	DND_PROJ_HADES_2,
	
	DND_PROJ_MACHINEGUN,
	DND_PROJ_HEAVYMACHINEGUN,
	DND_PROJ_LEADSPITTER,
	DND_PROJ_DEMONSEALER,
	DND_PROJ_DESOLATOR,
	DND_PROJ_MINIGUN,
	DND_PROJ_EBONY_1,
	DND_PROJ_EBONY_2,
	DND_PROJ_MPPB_1,
	DND_PROJ_MPPB_2,
	DND_PROJ_MPPB_3,
	DND_PROJ_TEMPLARMG,
	DND_PROJ_RIOTGUN,
	DND_PROJ_ACIDBULLET,
	DND_PROJ_ACIDSTAKE,
	DND_PROJ_FUSIONBLASTER,
	DND_PROJ_FUSIONBLASTERGRENADE,
	DND_PROJ_INCINERATOR,
	
	DND_PROJ_ROCKETLAUNCHER,
	DND_PROJ_TORPEDOLAUNCHER,
	DND_PROJ_VINDICATOR,
	DND_PROJ_GRENADE,
	DND_PROJ_ROTARYGRENADE,
	DND_PROJ_HEAVYMISSILELAUNCHER_1,
	DND_PROJ_HEAVYMISSILELAUNCHER_2,
	DND_PROJ_SEDRIN_1,
	DND_PROJ_SEDRIN_2,
	DND_PROJ_MERCURY,
	DND_PROJ_METEORLAUNCHER,
	DND_PROJ_HEAVYGRENADE,
	DND_PROJ_FREEZER,
	DND_PROJ_GRAVDIS,
	DND_PROJ_VOIDCANNON,
	
	DND_PROJ_PLASMA,
	DND_PROJ_NUCLEARPLASMA,
	DND_PROJ_NAILGUN_1,
	DND_PROJ_NAILGUN_2,
	DND_PROJ_BASILISK_1,
	DND_PROJ_BASILISK_2,
	DND_PROJ_FROSTFANG_1,
	DND_PROJ_FROSTFANG_2,
	DND_PROJ_RHINO,
	DND_PROJ_DARKLANCE_SHRED,
	DND_PROJ_FLAMETHROWER,
	
	DND_PROJ_BFG6000,
	DND_PROJ_BFG32768,
	DND_PROJ_DEVASTATOR,
	DND_PROJ_MFG,
	DND_PROJ_DEATHRAY,
	DND_PROJ_IONCANNON,
	DND_PROJ_THUNDERSTAFF,
	DND_PROJ_DEATHSTAFF,
	DND_PROJ_SOULREAVER,
	
	DND_PROJ_SAWEDOFF,
	DND_PROJ_SMG,
	DND_PROJ_SOULRENDER1,
	DND_PROJ_SOULRENDER2,
	DND_PROJ_HELLFORGE_1,
	DND_PROJ_HELLFORGE_2,
	DND_PROJ_SPINE_1,
	DND_PROJ_SPINE_2,
	DND_PROJ_ENFORCERLASER,
	DND_PROJ_VENOM_1,
	DND_PROJ_VENOM_2,
	DND_PROJ_HEAVYNAIL,
	DND_PROJ_BERETTA,
	DND_PROJ_PLASMABOLTER_1,
	DND_PROJ_PLASMABOLTER_2,
	
	DND_PROJ_FLECHETTE,
	DND_PROJ_MAGNUMSHELL,
	DND_PROJ_ELECTRICSHELL,
	DND_PROJ_SLUGSHELL,
	DND_PROJ_NITROSHELL,
	DND_PROJ_GRENADESPECIAL,
	DND_PROJ_HEGRENADE,
	DND_PROJ_SONICGRENADE
};
#define MAX_PROJECTILE_TYPES (DND_PROJ_SONICGRENADE + 1)

global proj_data_T 30: ProjectileInfo[MAX_PROJECTILE_TYPES];

// This fills the giant table of projectile information
void SetupProjectileData() {
	// SLOT 1
	ProjectileInfo[DND_PROJ_FIST].name = "FistPuff";
	ProjectileInfo[DND_PROJ_FIST].flags = DND_PROJ_MELEE;
	ProjectileInfo[DND_PROJ_FIST].spd_range = 64.0;
	
	ProjectileInfo[DND_PROJ_CHAINSAW].name = "ChainsawPuff";
	ProjectileInfo[DND_PROJ_CHAINSAW].flags = DND_PROJ_MELEE | DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_CHAINSAW].spd_range = 65.0;
	
	ProjectileInfo[DND_PROJ_DOUBLECHAINSAW].name = "DChainsawPuff";
	ProjectileInfo[DND_PROJ_DOUBLECHAINSAW].flags = DND_PROJ_MELEE | DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_DOUBLECHAINSAW].spd_range = 65.0;
	
	ProjectileInfo[DND_PROJ_SICKLE].name = "SicklePuff";
	ProjectileInfo[DND_PROJ_SICKLE].flags = DND_PROJ_MELEE;
	ProjectileInfo[DND_PROJ_SICKLE].spd_range = 136.0;
	
	ProjectileInfo[DND_PROJ_EXCALIBAT].name = "ExcalibatPuff";
	ProjectileInfo[DND_PROJ_EXCALIBAT].flags = DND_PROJ_MELEE;
	ProjectileInfo[DND_PROJ_EXCALIBAT].spd_range = 100.0;
	
	ProjectileInfo[DND_PROJ_EXCALIBAT2].name = "ExcalibatBall";
	ProjectileInfo[DND_PROJ_EXCALIBAT2].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_EXCALIBAT2].spd_range = 32;
	
	ProjectileInfo[DND_PROJ_KATANA].name = "KatanaPuff";
	ProjectileInfo[DND_PROJ_KATANA].flags = DND_PROJ_MELEE;
	ProjectileInfo[DND_PROJ_KATANA].spd_range = 100.0;
	
	ProjectileInfo[DND_PROJ_KATANA2].name = "KatanaShockWave";
	ProjectileInfo[DND_PROJ_KATANA2].flags = DND_PROJ_MELEEBONUSES;
	ProjectileInfo[DND_PROJ_KATANA2].spd_range = 60;
	
	ProjectileInfo[DND_PROJ_DUSKBLADE1].name = "DuskBladePuff";
	ProjectileInfo[DND_PROJ_DUSKBLADE1].flags = DND_PROJ_MELEE;
	ProjectileInfo[DND_PROJ_DUSKBLADE1].spd_range = 96.0;
	
	ProjectileInfo[DND_PROJ_DUSKBLADE2].name = "DuskSlash";
	ProjectileInfo[DND_PROJ_DUSKBLADE2].flags = DND_PROJ_MELEEBONUSES;
	ProjectileInfo[DND_PROJ_DUSKBLADE2].spd_range = 28;
	
	ProjectileInfo[DND_PROJ_INFERNOSWORD1].name = "InfernoSwordPuff";
	ProjectileInfo[DND_PROJ_INFERNOSWORD1].flags = DND_PROJ_MELEE | DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_INFERNOSWORD1].spd_range = 88.0;
	
	ProjectileInfo[DND_PROJ_INFERNOSWORD2].name = "InfernoSwordMissile_1";
	ProjectileInfo[DND_PROJ_INFERNOSWORD2].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_INFERNOSWORD2].spd_range = 28;

	// SLOT 2
	ProjectileInfo[DND_PROJ_PISTOL].name = "PistolPuff";
	ProjectileInfo[DND_PROJ_PISTOL].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_PISTOL].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_AKIMBO].name = "AkimboPistolPuff";
	ProjectileInfo[DND_PROJ_AKIMBO].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_AKIMBO].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_MAGNUMREVOLVER].name = "MagnumPistolPuff";
	ProjectileInfo[DND_PROJ_MAGNUMREVOLVER].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_MAGNUMREVOLVER].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_LASERPISTOL].name = "LaserPistolPuff_0";
	ProjectileInfo[DND_PROJ_LASERPISTOL].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_LASERPISTOL].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_SCATTERGUN].name = "ScatterPShot";
	ProjectileInfo[DND_PROJ_SCATTERGUN].flags = 0;
	ProjectileInfo[DND_PROJ_SCATTERGUN].spd_range = 50;
	
	ProjectileInfo[DND_PROJ_RUBYWAND].name = "RubyShot";
	ProjectileInfo[DND_PROJ_RUBYWAND].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_RUBYWAND].spd_range = 30;
	
	ProjectileInfo[DND_PROJ_RUBYWANDFLAME].name = "RubyFlameMissile";
	ProjectileInfo[DND_PROJ_RUBYWANDFLAME].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_RUBYWANDFLAME].spd_range = 128;
	
	ProjectileInfo[DND_PROJ_ASSAULTRIFLE].name = "AssaultRiflePuff";
	ProjectileInfo[DND_PROJ_ASSAULTRIFLE].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_ASSAULTRIFLE].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_VIPERCRAWLER].name = "ViperCrawler";
	ProjectileInfo[DND_PROJ_VIPERCRAWLER].flags = DND_PROJ_ISMINION;
	ProjectileInfo[DND_PROJ_VIPERCRAWLER].spd_range = 24;
	
	ProjectileInfo[DND_PROJ_VIPERMISSILE].name = "ViperMissile";
	ProjectileInfo[DND_PROJ_VIPERMISSILE].flags = 0;
	ProjectileInfo[DND_PROJ_VIPERMISSILE].spd_range = 30;
	
	// SLOT 3
	ProjectileInfo[DND_PROJ_SHOTGUN].name = "ShotgunPuff";
	ProjectileInfo[DND_PROJ_SHOTGUN].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_SHOTGUN].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_PURIFIER].name = "PurifierPuff";
	ProjectileInfo[DND_PROJ_PURIFIER].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_PURIFIER].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_KILLSTORM].name = "KillstormPuff";
	ProjectileInfo[DND_PROJ_KILLSTORM].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_KILLSTORM].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_EMERALDMAIN].name = "EmeraldPuff";
	ProjectileInfo[DND_PROJ_EMERALDMAIN].flags = DND_PROJ_HITSCAN | DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_EMERALDMAIN].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_EMERALDRAINER].name = "EmeraldRainer";
	ProjectileInfo[DND_PROJ_EMERALDRAINER].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_EMERALDRAINER].spd_range = 30;
	
	ProjectileInfo[DND_PROJ_SSG].name = "SSGPuff";
	ProjectileInfo[DND_PROJ_SSG].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_SSG].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_HEAVYSSG].name = "HSSGPuff";
	ProjectileInfo[DND_PROJ_HEAVYSSG].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_HEAVYSSG].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_ERASUS].name = "ErasusPuff";
	ProjectileInfo[DND_PROJ_ERASUS].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_ERASUS].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_HELLSMAWMAIN].name = "HellsMawRocket";
	ProjectileInfo[DND_PROJ_HELLSMAWMAIN].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_HELLSMAWMAIN].spd_range = 35;
	
	ProjectileInfo[DND_PROJ_HELLSMAWMINI].name = "HellsMawMiniRocket";
	ProjectileInfo[DND_PROJ_HELLSMAWMINI].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_HELLSMAWMINI].spd_range = 35;

	ProjectileInfo[DND_PROJ_WHITEDEATH].name = "WhiteDeathPuff";
	ProjectileInfo[DND_PROJ_WHITEDEATH].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_WHITEDEATH].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_SLAYER].name = "SlayerHook";
	ProjectileInfo[DND_PROJ_SLAYER].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_SLAYER].spd_range = 28;
	
	ProjectileInfo[DND_PROJ_DEADLOCK].name = "DeadlockPuff";
	ProjectileInfo[DND_PROJ_DEADLOCK].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_DEADLOCK].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_WHEELOFTORMENT_1].name = "BloodFX2A";
	ProjectileInfo[DND_PROJ_WHEELOFTORMENT_1].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_WHEELOFTORMENT_1].spd_range = 1024.0;
	
	ProjectileInfo[DND_PROJ_WHEELOFTORMENT_2].name = "BloodFX3";
	ProjectileInfo[DND_PROJ_WHEELOFTORMENT_2].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_WHEELOFTORMENT_2].spd_range = 84.0;
	
	ProjectileInfo[DND_PROJ_WHEELOFTORMENT_3].name = "BloodDrainFX";
	ProjectileInfo[DND_PROJ_WHEELOFTORMENT_3].flags = DND_PROJ_HITSCAN | DND_PROJ_MELEEBONUSES;
	ProjectileInfo[DND_PROJ_WHEELOFTORMENT_3].spd_range = 84.0;
	
	ProjectileInfo[DND_PROJ_CHARON].name = "CharonBall";
	ProjectileInfo[DND_PROJ_CHARON].flags = 0;
	ProjectileInfo[DND_PROJ_CHARON].spd_range = 36;
	
	ProjectileInfo[DND_PROJ_PLASMACANNON].name = "PlasmaCannonBall";
	ProjectileInfo[DND_PROJ_PLASMACANNON].flags = 0;
	ProjectileInfo[DND_PROJ_PLASMACANNON].spd_range = 30;
	
	ProjectileInfo[DND_PROJ_SHOCKER].name = "ShockerPuff";
	ProjectileInfo[DND_PROJ_SHOCKER].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_SHOCKER].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_HADES].name = "HadesPuff";
	ProjectileInfo[DND_PROJ_HADES].flags = DND_PROJ_HITSCAN | DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_HADES].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_HADES_2].name = "HadesShot";
	ProjectileInfo[DND_PROJ_HADES_2].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_HADES_2].spd_range = 40;
	
	// slot 4
	ProjectileInfo[DND_PROJ_MACHINEGUN].name = "MGPuff";
	ProjectileInfo[DND_PROJ_MACHINEGUN].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_MACHINEGUN].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_HEAVYMACHINEGUN].name = "HMGPuff";
	ProjectileInfo[DND_PROJ_HEAVYMACHINEGUN].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_HEAVYMACHINEGUN].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_LEADSPITTER].name = "LeadBulletPuff";
	ProjectileInfo[DND_PROJ_LEADSPITTER].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_LEADSPITTER].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_DEMONSEALER].name = "DemonSealCard";
	ProjectileInfo[DND_PROJ_DEMONSEALER].flags = 0;
	ProjectileInfo[DND_PROJ_DEMONSEALER].spd_range = 48;
	
	ProjectileInfo[DND_PROJ_DESOLATOR].name = "DesolatorPuff";
	ProjectileInfo[DND_PROJ_DESOLATOR].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_DESOLATOR].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_MINIGUN].name = "ThrottlePuff";
	ProjectileInfo[DND_PROJ_MINIGUN].flags = DND_PROJ_HITSCAN | DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_MINIGUN].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_EBONY_1].name = "EbonyBalls";
	ProjectileInfo[DND_PROJ_EBONY_1].flags = 0;
	ProjectileInfo[DND_PROJ_EBONY_1].spd_range = 36;
	
	ProjectileInfo[DND_PROJ_EBONY_2].name = "EbonyBalls2";
	ProjectileInfo[DND_PROJ_EBONY_2].flags = 0;
	ProjectileInfo[DND_PROJ_EBONY_2].spd_range = 36;

	ProjectileInfo[DND_PROJ_MPPB_1].name = "MPPB_Projectile1";
	ProjectileInfo[DND_PROJ_MPPB_1].flags = 0;
	ProjectileInfo[DND_PROJ_MPPB_1].spd_range = 60;

	ProjectileInfo[DND_PROJ_MPPB_2].name = "MPPB_Projectile2";
	ProjectileInfo[DND_PROJ_MPPB_2].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_MPPB_2].spd_range = 60;
	
	ProjectileInfo[DND_PROJ_MPPB_3].name = "MPPB_Projectile3";
	ProjectileInfo[DND_PROJ_MPPB_3].flags = 0;
	ProjectileInfo[DND_PROJ_MPPB_3].spd_range = 60;
	
	ProjectileInfo[DND_PROJ_TEMPLARMG].name = "TemplarMGPuff";
	ProjectileInfo[DND_PROJ_TEMPLARMG].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_TEMPLARMG].spd_range = HITSCAN_RANGE_DEFAULT;

	ProjectileInfo[DND_PROJ_RIOTGUN].name = "RiotgunPuff";
	ProjectileInfo[DND_PROJ_RIOTGUN].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_RIOTGUN].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_ACIDBULLET].name = "AcidPuff";
	ProjectileInfo[DND_PROJ_ACIDBULLET].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_ACIDBULLET].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_ACIDSTAKE].name = "AcidBolt";
	ProjectileInfo[DND_PROJ_ACIDSTAKE].flags = 0;
	ProjectileInfo[DND_PROJ_ACIDSTAKE].spd_range = 56;
	
	ProjectileInfo[DND_PROJ_FUSIONBLASTER].name = "FusionBlasterProjectile";
	ProjectileInfo[DND_PROJ_FUSIONBLASTER].flags = 0;
	ProjectileInfo[DND_PROJ_FUSIONBLASTER].spd_range = 72;
	
	ProjectileInfo[DND_PROJ_FUSIONBLASTERGRENADE].name = "FusionBlasterGrenade";
	ProjectileInfo[DND_PROJ_FUSIONBLASTERGRENADE].flags = 0;
	ProjectileInfo[DND_PROJ_FUSIONBLASTERGRENADE].spd_range = 36;
	
	ProjectileInfo[DND_PROJ_INCINERATOR].name = "IncineratorProjectile1";
	ProjectileInfo[DND_PROJ_INCINERATOR].flags = 0;
	ProjectileInfo[DND_PROJ_INCINERATOR].spd_range = 52;
	
	// SLOT 5
	ProjectileInfo[DND_PROJ_ROCKETLAUNCHER].name = "RocketX";
	ProjectileInfo[DND_PROJ_ROCKETLAUNCHER].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_ROCKETLAUNCHER].spd_range = 20;
	
	ProjectileInfo[DND_PROJ_TORPEDOLAUNCHER].name = "NewRocket";
	ProjectileInfo[DND_PROJ_TORPEDOLAUNCHER].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_TORPEDOLAUNCHER].spd_range = 52;
	
	ProjectileInfo[DND_PROJ_VINDICATOR].name = "FlakShellPuff";
	ProjectileInfo[DND_PROJ_VINDICATOR].flags = DND_PROJ_HITSCAN | DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_VINDICATOR].spd_range = 320.0;
	
	ProjectileInfo[DND_PROJ_GRENADE].name = "NormalGrenade";
	ProjectileInfo[DND_PROJ_GRENADE].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_GRENADE].spd_range = 36;
	
	ProjectileInfo[DND_PROJ_ROTARYGRENADE].name = "NewGrenade";
	ProjectileInfo[DND_PROJ_ROTARYGRENADE].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_ROTARYGRENADE].spd_range = 36;
	
	ProjectileInfo[DND_PROJ_HEAVYMISSILELAUNCHER_1].name = "HomRocket1";
	ProjectileInfo[DND_PROJ_HEAVYMISSILELAUNCHER_1].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_HEAVYMISSILELAUNCHER_1].spd_range = 32;
	
	ProjectileInfo[DND_PROJ_HEAVYMISSILELAUNCHER_2].name = "HomRocket1NH";
	ProjectileInfo[DND_PROJ_HEAVYMISSILELAUNCHER_2].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_HEAVYMISSILELAUNCHER_2].spd_range = 32;
	
	ProjectileInfo[DND_PROJ_SEDRIN_1].name = "SedrinProjectile";
	ProjectileInfo[DND_PROJ_SEDRIN_1].flags = 0;
	ProjectileInfo[DND_PROJ_SEDRIN_1].spd_range = 36;
	
	ProjectileInfo[DND_PROJ_SEDRIN_2].name = "SedrinSplitProjectile";
	ProjectileInfo[DND_PROJ_SEDRIN_2].flags = 0;
	ProjectileInfo[DND_PROJ_SEDRIN_2].spd_range = 24;

	ProjectileInfo[DND_PROJ_MERCURY].name = "MercuryRocket";
	ProjectileInfo[DND_PROJ_MERCURY].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_MERCURY].spd_range = 16;	
	
	ProjectileInfo[DND_PROJ_METEORLAUNCHER].name = "MeteorMissile";
	ProjectileInfo[DND_PROJ_METEORLAUNCHER].flags = 0;
	ProjectileInfo[DND_PROJ_METEORLAUNCHER].spd_range = 30;	
	
	ProjectileInfo[DND_PROJ_HEAVYGRENADE].name = "HeavyGrenade";
	ProjectileInfo[DND_PROJ_HEAVYGRENADE].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_HEAVYGRENADE].spd_range = 24;
	
	ProjectileInfo[DND_PROJ_FREEZER].name = "FreezeBall";
	ProjectileInfo[DND_PROJ_FREEZER].flags = 0;
	ProjectileInfo[DND_PROJ_FREEZER].spd_range = 36;
	
	ProjectileInfo[DND_PROJ_GRAVDIS].name = "GravdisProjectile";
	ProjectileInfo[DND_PROJ_GRAVDIS].flags = 0;
	ProjectileInfo[DND_PROJ_GRAVDIS].spd_range = 50;
	
	ProjectileInfo[DND_PROJ_VOIDCANNON].name = "VoidMissile";
	ProjectileInfo[DND_PROJ_VOIDCANNON].flags = 0;
	ProjectileInfo[DND_PROJ_VOIDCANNON].spd_range = 40;
	
	ProjectileInfo[DND_PROJ_PLASMA].name = "PlasmaBallX";
	ProjectileInfo[DND_PROJ_PLASMA].flags = 0;
	ProjectileInfo[DND_PROJ_PLASMA].spd_range = 25;
	
	ProjectileInfo[DND_PROJ_NUCLEARPLASMA].name = "NewPlasmaBall";
	ProjectileInfo[DND_PROJ_NUCLEARPLASMA].flags = 0;
	ProjectileInfo[DND_PROJ_NUCLEARPLASMA].spd_range = 25;
	
	ProjectileInfo[DND_PROJ_NUCLEARPLASMA].name = "NewPlasmaBall";
	ProjectileInfo[DND_PROJ_NUCLEARPLASMA].flags = 0;
	ProjectileInfo[DND_PROJ_NUCLEARPLASMA].spd_range = 25;
	
	ProjectileInfo[DND_PROJ_NAILGUN_1].name = "NailProjectile";
	ProjectileInfo[DND_PROJ_NAILGUN_1].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_NAILGUN_1].spd_range = 40;
	
	ProjectileInfo[DND_PROJ_NAILGUN_2].name = "LavaNailProjectile";
	ProjectileInfo[DND_PROJ_NAILGUN_2].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_NAILGUN_2].spd_range = 40;
	
	ProjectileInfo[DND_PROJ_BASILISK_1].name = "BasiliskBall";
	ProjectileInfo[DND_PROJ_BASILISK_1].flags = 0;
	ProjectileInfo[DND_PROJ_BASILISK_1].spd_range = 96;
	
	ProjectileInfo[DND_PROJ_BASILISK_2].name = "BasiliskBall2";
	ProjectileInfo[DND_PROJ_BASILISK_2].flags = 0;
	ProjectileInfo[DND_PROJ_BASILISK_2].spd_range = 25;

	ProjectileInfo[DND_PROJ_FROSTFANG_1].name = "FrostFangShard";
	ProjectileInfo[DND_PROJ_FROSTFANG_1].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_FROSTFANG_1].spd_range = 40;

	ProjectileInfo[DND_PROJ_FROSTFANG_2].name = "FrostFangBall";
	ProjectileInfo[DND_PROJ_FROSTFANG_2].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_FROSTFANG_2].spd_range = 8;
	
	ProjectileInfo[DND_PROJ_RHINO].name = "RhinoPuff";
	ProjectileInfo[DND_PROJ_RHINO].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_RHINO].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_FLAMETHROWER].name = "FTFire";
	ProjectileInfo[DND_PROJ_FLAMETHROWER].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_FLAMETHROWER].spd_range = 16;
	
	ProjectileInfo[DND_PROJ_DARKLANCE_SHRED].name = "DarkLanceShredder";
	ProjectileInfo[DND_PROJ_DARKLANCE_SHRED].flags = 0;
	ProjectileInfo[DND_PROJ_DARKLANCE_SHRED].spd_range = 45;
	
	ProjectileInfo[DND_PROJ_BFG6000].name = "BFGBall2";
	ProjectileInfo[DND_PROJ_BFG6000].flags = 0;
	ProjectileInfo[DND_PROJ_BFG6000].spd_range = 25;
	
	ProjectileInfo[DND_PROJ_BFG32768].name = "BFGBallUpgraded";
	ProjectileInfo[DND_PROJ_BFG32768].flags = 0;
	ProjectileInfo[DND_PROJ_BFG32768].spd_range = 25;

	// SLOT 7
	ProjectileInfo[DND_PROJ_DEVASTATOR].name = "DevastatorRocket";
	ProjectileInfo[DND_PROJ_DEVASTATOR].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_DEVASTATOR].spd_range = 32;
	
	ProjectileInfo[DND_PROJ_MFG].name = "DNBFGBall";
	ProjectileInfo[DND_PROJ_MFG].flags = 0;
	ProjectileInfo[DND_PROJ_MFG].spd_range = 40;
	
	ProjectileInfo[DND_PROJ_DEATHRAY].name = "DeathRayProjectile";
	ProjectileInfo[DND_PROJ_DEATHRAY].flags = 0;
	ProjectileInfo[DND_PROJ_DEATHRAY].spd_range = 8;
	
	ProjectileInfo[DND_PROJ_IONCANNON].name = "IonPuff";
	ProjectileInfo[DND_PROJ_IONCANNON].flags = DND_PROJ_HITSCAN | DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_IONCANNON].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_THUNDERSTAFF].name = "BallLightning";
	ProjectileInfo[DND_PROJ_THUNDERSTAFF].flags = 0;
	ProjectileInfo[DND_PROJ_THUNDERSTAFF].spd_range = 10;
	
	ProjectileInfo[DND_PROJ_DEATHSTAFF].name = "VolcanoFireball5";
	ProjectileInfo[DND_PROJ_DEATHSTAFF].flags = 0;
	ProjectileInfo[DND_PROJ_DEATHSTAFF].spd_range = 40;
	
	ProjectileInfo[DND_PROJ_SOULREAVER].name = "SoulReaverSphere";
	ProjectileInfo[DND_PROJ_SOULREAVER].flags = 0;
	ProjectileInfo[DND_PROJ_SOULREAVER].spd_range = 40;
	
	// TEMP
	ProjectileInfo[DND_PROJ_SAWEDOFF].name = "SawedoffPuff";
	ProjectileInfo[DND_PROJ_SAWEDOFF].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_SAWEDOFF].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_SMG].name = "SMGPuff";
	ProjectileInfo[DND_PROJ_SMG].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_SMG].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_SOULRENDER1].name = "BladePuff1";
	ProjectileInfo[DND_PROJ_SOULRENDER1].flags = DND_PROJ_MELEE;
	ProjectileInfo[DND_PROJ_SOULRENDER1].spd_range = 96.0;
	
	ProjectileInfo[DND_PROJ_SOULRENDER2].name = "BladeSlash";
	ProjectileInfo[DND_PROJ_SOULRENDER2].flags = DND_PROJ_MELEEBONUSES;
	ProjectileInfo[DND_PROJ_SOULRENDER2].spd_range = 25;
	
	ProjectileInfo[DND_PROJ_HELLFORGE_1].name = "Ripper1";
	ProjectileInfo[DND_PROJ_HELLFORGE_1].flags = 0;
	ProjectileInfo[DND_PROJ_HELLFORGE_1].spd_range = 28;
	
	ProjectileInfo[DND_PROJ_HELLFORGE_2].name = "Ripper2Proj";
	ProjectileInfo[DND_PROJ_HELLFORGE_2].flags = 0;
	ProjectileInfo[DND_PROJ_HELLFORGE_2].spd_range = 24;

	ProjectileInfo[DND_PROJ_SPINE_1].name = "BloodFX";
	ProjectileInfo[DND_PROJ_SPINE_1].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_SPINE_1].spd_range = 384.0;
	
	ProjectileInfo[DND_PROJ_SPINE_2].name = "GoreShot1";
	ProjectileInfo[DND_PROJ_SPINE_2].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_SPINE_2].spd_range = 36;
	
	ProjectileInfo[DND_PROJ_ENFORCERLASER].name = "EnforcerPlasma";
	ProjectileInfo[DND_PROJ_ENFORCERLASER].flags = 0;
	ProjectileInfo[DND_PROJ_ENFORCERLASER].spd_range = 48;
	
	ProjectileInfo[DND_PROJ_VENOM_1].name = "Barbshot";
	ProjectileInfo[DND_PROJ_VENOM_1].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_VENOM_1].spd_range = 60;
	
	ProjectileInfo[DND_PROJ_VENOM_2].name = "Acidshot";
	ProjectileInfo[DND_PROJ_VENOM_2].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_VENOM_2].spd_range = 35;
	
	ProjectileInfo[DND_PROJ_HEAVYNAIL].name = "BigNailProj";
	ProjectileInfo[DND_PROJ_HEAVYNAIL].flags = 0;
	ProjectileInfo[DND_PROJ_HEAVYNAIL].spd_range = 44;

	ProjectileInfo[DND_PROJ_BERETTA].name = "BerettaPuff";
	ProjectileInfo[DND_PROJ_BERETTA].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_BERETTA].spd_range = HITSCAN_RANGE_DEFAULT;

	ProjectileInfo[DND_PROJ_PLASMABOLTER_1].name = "PlasmaBolterProj";
	ProjectileInfo[DND_PROJ_PLASMABOLTER_1].flags = 0;
	ProjectileInfo[DND_PROJ_PLASMABOLTER_1].spd_range = 32;
	
	ProjectileInfo[DND_PROJ_PLASMABOLTER_2].name = "PlasmaBolterProj2";
	ProjectileInfo[DND_PROJ_PLASMABOLTER_2].flags = 0;
	ProjectileInfo[DND_PROJ_PLASMABOLTER_2].spd_range = 20;
	
	// special ammo types
	ProjectileInfo[DND_PROJ_FLECHETTE].name = "FlechettePuff";
	ProjectileInfo[DND_PROJ_FLECHETTE].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_FLECHETTE].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_MAGNUMSHELL].name = "MagnumPuff";
	ProjectileInfo[DND_PROJ_MAGNUMSHELL].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_MAGNUMSHELL].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_ELECTRICSHELL].name = "ShockShellPuff";
	ProjectileInfo[DND_PROJ_ELECTRICSHELL].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_ELECTRICSHELL].spd_range = HITSCAN_RANGE_DEFAULT;

	ProjectileInfo[DND_PROJ_SLUGSHELL].name = "SlugShellPuff";
	ProjectileInfo[DND_PROJ_SLUGSHELL].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_SLUGSHELL].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_NITROSHELL].name = "NitroShellPuff";
	ProjectileInfo[DND_PROJ_NITROSHELL].flags = DND_PROJ_HITSCAN;
	ProjectileInfo[DND_PROJ_NITROSHELL].spd_range = HITSCAN_RANGE_DEFAULT;
	
	ProjectileInfo[DND_PROJ_GRENADESPECIAL].name = "NormalGrenade_Special";
	ProjectileInfo[DND_PROJ_GRENADESPECIAL].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_GRENADESPECIAL].spd_range = 28;
	
	ProjectileInfo[DND_PROJ_HEGRENADE].name = "HEGrenadeX";
	ProjectileInfo[DND_PROJ_HEGRENADE].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_HEGRENADE].spd_range = 24;
	
	ProjectileInfo[DND_PROJ_SONICGRENADE].name = "SonicGrenade";
	ProjectileInfo[DND_PROJ_SONICGRENADE].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_SONICGRENADE].spd_range = 32;
}

#endif
