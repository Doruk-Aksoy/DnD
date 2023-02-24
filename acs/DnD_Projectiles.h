#ifndef DND_PROJ_IN
#define DND_PROJ_IN

#define HITSCAN_RANGE_DEFAULT 8192.0

enum {
	DND_PROJ_HITSCAN 					= 0b1,
	DND_PROJ_HASGHOSTHITTER				= 0b10,
	DND_PROJ_ISMINION					= 0b100,
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
	
	DND_PROJ_DEVASTATOR
};
#define MAX_PROJECTILE_TYPES (DND_PROJ_DEVASTATOR + 1)

global proj_data_T 30: ProjectileInfo[MAX_PROJECTILE_TYPES];

// This fills the giant table of projectile information
void SetupProjectileData() {
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
	
	
	// SLOT 7
	ProjectileInfo[DND_PROJ_DEVASTATOR].name = "DevastatorRocket";
	ProjectileInfo[DND_PROJ_DEVASTATOR].flags = DND_PROJ_HASGHOSTHITTER;
	ProjectileInfo[DND_PROJ_DEVASTATOR].spd_range = 32;
}

#endif
