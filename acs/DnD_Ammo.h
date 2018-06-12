#ifndef DND_AMMO_IN
#define DND_AMMO_IN

#include "DnD_Stat.h"

#define DND_MAX_BACKPACK 55
#define DND_BACKPACK_RATIO 5

enum {
	DND_AMMOSLOT_CLIP,
	DND_AMMOSLOT_SHELL,
	DND_AMMOSLOT_ROCKET,
	DND_AMMOSLOT_CELL,
	DND_AMMOSLOT_SOULS
};

#define MAX_SLOTS DND_AMMOSLOT_SOULS + 1
#define MAX_AMMOTYPES_PER_SLOT 10

enum {
	// category 2
	AMMO_CLIP = 0,
	AMMO_RUBY,
	AMMO_VIPER,
	AMMO_DEMONSEAL,
	AMMO_RIOT,
	AMMO_ACID,
	AMMO_FUSIONCELL,
	AMMO_DESOLATOR,
	AMMO_EBONY1,
	AMMO_EBONY2,
	
	// category 3
	AMMO_SHELL = 0,
	AMMO_PCANNON,
	AMMO_NITROGENCANISTER,
	AMMO_HADES,
	AMMO_EXSHELL,
	AMMO_SLAYER,
	
	// category 4
	AMMO_ROCKET = 0,
	AMMO_FLAKSHELL,
	AMMO_METEOR,
	AMMO_GRENADE,
	AMMO_HMISSILE,
	
	// category 5
	AMMO_CELL = 0,
	AMMO_EVERICE,
	AMMO_FUEL,
	AMMO_LIGHTNING,
	AMMO_NAIL,
	AMMO_LAVA,
	AMMO_ION,
	AMMO_THUNDER,
	AMMO_GAUSS,
	
	// category 6 (soul weapons -- only 1 ammo type here)
	AMMO_SOUL = 0
};

#define MAXAMMOTYPES AMMO_SOUL + 1

typedef struct {
	str ammo_icon;
	str ammo_name;
	int initial_capacity;
	int container_value;
} ammo_info_T;

ammo_info_T AmmoInfo[MAX_SLOTS][MAX_AMMOTYPES_PER_SLOT] = {
	// category 1 -- clip box
	{
		{ "CLIPA0",			"Clip",						200,		50		},
		{ "RBAMA0",			"RubyAmmo",					175,		43		},
		{ "C_H2A1",			"ViperAmmo",				80,			20		},
		{ "DEMSICO",		"DSealAmmo",				200,		50		},
		{ "RIOTJ0",         "RiotgunShell",				160, 		40		},
		{ "D97AA0",			"AcidAmmo",					160,		40		},
		{ "FBATA0",			"FusionCell",				240,		48		},
		{ "DES4Y0",			"DesolatorAmmo",			175,		42		},
		{ "EBOPA0",			"EbonyAmmo",				60,			12		},
		{ "EBOPE0",			"EbonyAmmoX",				30,			6		},
	},
	// category 2 -- shell box
	{
		{ "SHELA0",			"Shell",					50,			20		},
		{ "PCNIC1",         "PCanAmmo",                 35,			15		},
		{ "D97A1",          "NitrogenCanister",         40,			15		},
		{ "STRGY0",			"HadesAmmo",				32,			8		},
		{ "ESHAA0",			"ExplodingShell",			40,			15		},
		{ "SLAYAM01",		"SlayerAmmo",				40,			15		},
		{ "",				"",							-1,			0		}
	},
	// category 3 -- rocket box
	{
		{ "ROCKA0",			"RocketAmmo",				50,			5		},
		{ "FSHLA0",			"FlakShell",				40,			10		},
		{ "LAAM1",          "MeteorAmmo",               40,			4		},
		{ "GBUNA0",			"Grenades", 				50,			5		},
		{ "AHRLA0",			"MISAmmo",					80,			6		},
		{ "",				"",							-1,			0		}
	},
	// category 4 -- cell box
	{
		{ "CELLA0",			"Cell",						300,		100		},
		{ "EICEAM1",		"EverIce",					240,		80		},
		{ "FUAMA0",         "Fuel",                     250,		75		},
		{ "D98AB1",         "LightningCell",            375,		105		},
		{ "NPAKA0",			"NailgunAmmo",				175,		40		},
		{ "BASIA01",		"BasiliskAmmo",				250,		16		},
		{ "IONAMM1",        "IonAmmo",                 	180,		60		},
		{ "D95AA0",			"ThunderAmmo",				200,		66		},
		{ "GAUSICO",		"GaussRound",				75,			10		},
		{ "",				"",							-1,			0		}
	},
	// category 5 -- soul ammo
	{
		{ "AMM3A0",			"Souls",					75,			0		},
		{ "",				"",							-1,			0		}
	}
};

#define MAXCLIPAMMOTYPES 30
str ClipAmmoTypes[MAXCLIPAMMOTYPES] = {
	"SpecialAmmoMode_3",
	"SpecialAmmoMode_3X",
	"SpecialAmmoMode_4",
	"SpecialAmmoMode_5",
	"SpecialAmmoMode_5X",
	"GoreMode",
	"BladeCharge",
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
	"HeavyGLCounter"
};

void SetAllAmmoCapacities() {
	int cap;
	// last slot is for souls, we don't increase it here
	for(int i = 0; i < MAX_SLOTS - 1; ++i)
		for(int j = 0; j < MAXAMMOTYPES && AmmoInfo[i][j].initial_capacity != -1; ++j) {
			cap = AmmoInfo[i][j].initial_capacity;
			SetAmmoCapacity(AmmoInfo[i][j].ammo_name, AmmoCapWithBonuses(cap + CheckInventory("BackpackCounter") * cap / DND_BACKPACK_RATIO));
		}
}

// when setting ammo capacity, this must be called at all costs!
int AmmoCapWithBonuses(int base) {
	// include any and all bonuses here
	base *= 1.0 + Player_Bonuses[PlayerNumber()].holding + GetDataFromOrbBonus(PlayerNumber(), OBI_HOLDING, -1);
	base >>= 16;
	return base;
}

int GetMaximumAmmoCapacity(int slot, int t) {
	int cap = AmmoInfo[slot][t].initial_capacity;
	int res = cap + cap * DND_MAX_BACKPACK / DND_BACKPACK_RATIO;
	// include holding bonuses
	res *= 1.0 + Player_Bonuses[PlayerNumber()].holding + GetDataFromOrbBonus(PlayerNumber(), OBI_HOLDING, -1);
	res >>= 16;
	return res;
}

bool CheckAmmoPickup(int slot, bool simple) {
	bool res = CheckInventory(AmmoInfo[slot][0].ammo_name) == GetAmmoCapacity(AmmoInfo[slot][0].ammo_name);
	if(!simple) {
		for(int i = 0; i < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[slot][i].initial_capacity != -1; ++i)
			res = res && CheckInventory(AmmoInfo[slot][i].ammo_name) == GetAmmoCapacity(AmmoInfo[slot][i].ammo_name);
	}
	return res;
}

void HandleAmmoContainerPickup(int slot) {
	int amt = 0, index = 0;
	for(int i = 0; i < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[slot][i].initial_capacity != -1; ++i) {
		amt = AmmoInfo[slot][i].container_value;
		amt += (CheckInventory("Perk_Munitionist") * amt * DND_MUNITION_GAIN) / 100;
		if(!amt)
			amt = 1;
		
		GiveInventory(AmmoInfo[slot][i].ammo_name, amt);
	}
}

#endif