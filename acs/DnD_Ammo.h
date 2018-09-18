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
	DND_AMMOSLOT_SOULS,
	DND_AMMOSLOT_MAGAZINE,
	DND_AMMOSLOT_SPECIAL,
	DND_AMMOSLOT_TEMPORARY
};

#define MAX_SLOTS DND_AMMOSLOT_SOULS + 1
#define MAX_AMMOTYPES_PER_SLOT 10

enum {
	DND_AMMO_SECONDARYISCLIP = 1
};

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
	AMMO_EMERALDMANA,
	AMMO_HELLSMAWAMMO,
	AMMO_PCANNON,
	AMMO_NITROGENCANISTER,
	AMMO_DEMONBLOOD,
	AMMO_HADES,
	AMMO_EXSHELL,
	AMMO_SLAYER,
	
	// category 4
	AMMO_ROCKET = 0,
	AMMO_FLAKSHELL,
	AMMO_METEOR,
	AMMO_HEAVYGRENADE,
	AMMO_GRENADE,
	AMMO_HMISSILE,
	
	// category 5
	AMMO_CELL = 0,
	AMMO_DEVASTATOR,
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

// don't make tables with strings and ints... weird fuckups happen
typedef struct {
	int initial_capacity;
	int container_value;
} ammo_info_T;

#define AMMOINFO_ICON 0
#define AMMOINFO_NAME 1
str AmmoInfo_Str[MAX_SLOTS][MAX_AMMOTYPES_PER_SLOT][2] = {
	// category 1 -- clip box
	{
		{		"CLIPA0",			"Clip"					},
		{		"RBAMA0",			"RubyAmmo"				},
		{		"C_H2A1",			"ViperAmmo"				},			
		{		"DEMSICO",			"DSealAmmo"				},			
		{		"RIOTJ0",			"RiotgunShell"			},
		{		"D97AA0",			"AcidAmmo"				},
		{		"FBATA0",			"FusionCell"			},
		{		"DES4Y0",			"DesolatorAmmo"			},
		{		"EBOPA0",			"EbonyAmmo"				},
		{		"EBOPE0",			"EbonyAmmoX"			}
	},
	// category 2 -- shell box
	{
		{		"SHELA0",			"Shell"					},
		{		"EMEAA0",			"EmeraldMana"			},
		{		"HMAWA1",			"HellsMawAmmo"			},
		{		"PCNIC1",			"PCanAmmo"				},
		{		"D97A1",			"NitrogenCanister"		},
		{		"DBLUD1",			"DemonBlood"			},
		{		"STRGY0",			"HadesAmmo"				},
		{		"ESHAA0",			"ExplodingShell"		},
		{		"SLAYAM01",			"SlayerAmmo"			},
		{		"",					""						}
	},
	// category 3 -- rocket box
	{
		{		"ROCKA0",			"RocketAmmo"			},
		{		"FSHLA0",			"FlakShell"				},
		{		"LAAM1",			"MeteorAmmo"			},
		{		"HEGRA0",			"HeavyGrenades"			},
		{		"GBUNA0",			"Grenades"				},
		{		"AHRLA0",			"MISAmmo"				},
		{		"",					"",						}
	},
	// category 4 -- cell box
	{
		{		"CELLA0",			"Cell"					},
		{		"DEVIAM0",			"DevastatorAmmo"		},
		{		"EICEAM1",			"EverIce"				},
		{		"FUAMA0",			"Fuel"					},
		{		"D98AB1",			"LightningCell"			},
		{		"NPAKA0",			"NailgunAmmo"			},
		{		"BASIA01",			"BasiliskAmmo"			},
		{		"IONAMM1",			"IonAmmo"				},
		{		"D95AA0",			"ThunderAmmo"			},
		{		"GAUSICO",			"GaussRound"			}
	},
	// category 5 -- soul ammo
	{
		{		"AMM3A0",			"Souls"					},
		{		"",					""						}
	}
};

ammo_info_T AmmoInfo[MAX_SLOTS][MAX_AMMOTYPES_PER_SLOT] = {
	// category 1 -- clip box
	{
		{ 		200,		50 		},
		{ 		175,		43		},
		{ 		80,			20		},
		{ 		200,		50		},
		{ 		160, 		40		},
		{ 		160,		40		},
		{ 		240,		48		},
		{ 		75,			42		},
		{ 		60,			12		},
		{ 		30,			6		}
	},
	// category 2 -- shell box
	{
		{ 		50,			20		},
		{ 		90,			36		},
		{ 		60,			24		},
		{ 		35,			15		},
		{ 		40,			15		},
		{ 		60,			25		},
		{ 		32,			8		},
		{ 		40,			15		},
		{ 		40,			15		},
		{ 		-1,			0		}
	},
	// category 3 -- rocket box
	{
		{ 		50,			5		},
		{ 		40,			10		},
		{ 		40,			4		},
		{  		50,			5		},
		{  		50,			5		},
		{ 		80,			6		},
		{ 		-1,			0		}
	},
	// category 4 -- cell box
	{
		{		300,		100		},
		{ 		270,		90		},
		{ 		240,		80		},
		{ 		250,		75		},
		{ 		375,		105		},
		{ 		175,		40		},
		{ 		250,		16		},
		{ 		180,		60		},
		{ 		200,		66		},
		{ 		75,			10		}
	},
	// category 5 -- soul ammo
	{
		{ 		75,			0		},
		{ 		-1,			0		}
	}
};

enum {
	DND_MAGAZINE_SPECIALAMMOMODE_3,
	DND_MAGAZINE_SPECIALAMMOMODE_3X,
	DND_MAGAZINE_SPECIALAMMOMODE_4,
	DND_MAGAZINE_SPECIALAMMOMODE_5,
	DND_MAGAZINE_SPECIALAMMOMODE_5X,
	DND_MAGAZINE_GOREMODE,
	DND_MAGAZINE_BLADECHARGE,
	DND_MAGAZINE_SAWEDOFFCOUNTER,
	DND_MAGAZINE_AKIMBOCLIPLEFT,
	DND_MAGAZINE_AKIMBOCLIPRIGHT,
	DND_MAGAZINE_BULLETSIZE_6,
	DND_MAGAZINE_SHELLSIZE_2,
	DND_MAGAZINE_SHELLSIZE_8,
	DND_MAGAZINE_SHELLSIZE_8N,
	DND_MAGAZINE_SHELLSIZE_10,
	DND_MAGAZINE_SHELLSIZE_12,
	DND_MAGAZINE_SHELLSIZE_16,
	DND_MAGAZINE_SHELLSIZE_18,
	DND_MAGAZINE_MGCLIP,
	DND_MAGAZINE_MGCLIP2,
	DND_MAGAZINE_MGCLIP3,
	DND_MAGAZINE_MGCLIP4,
	DND_MAGAZINE_MGCLIP5,
	DND_MAGAZINE_MGCLIP6,
	DND_MAGAZINE_MGCLIP7,
	DND_MAGAZINE_FUELCLIP,
	DND_MAGAZINE_LOADEDBASILISK,
	DND_MAGAZINE_PCANCLIP,
	DND_MAGAZINE_RIOTGUNCLIP,
	DND_MAGAZINE_ACIDCLIP,
	DND_MAGAZINE_HEAVYGLCOUNTER
};

#define MAXCLIPAMMOTYPES DND_MAGAZINE_HEAVYGLCOUNTER + 1
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
	"FuelClip",
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
		for(int j = 0; j < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[i][j].initial_capacity != -1; ++j) {
			cap = AmmoInfo[i][j].initial_capacity;
			SetAmmoCapacity(AmmoInfo_Str[i][j][AMMOINFO_NAME], AmmoCapWithBonuses(cap + CheckInventory("BackpackCounter") * cap / DND_BACKPACK_RATIO));
		}
}

// when setting ammo capacity, this must be called at all costs!
int AmmoCapWithBonuses(int base) {
	// include any and all bonuses here
	base *= 1.0 + GetPlayerAttributeValue(PlayerNumber(), INV_AMMOCAP_INCREASE) + GetDataFromOrbBonus(PlayerNumber(), OBI_HOLDING, -1);
	base >>= 16;
	return base;
}

int GetMaximumAmmoCapacity(int slot, int t) {
	int cap = AmmoInfo[slot][t].initial_capacity;
	int res = cap + cap * DND_MAX_BACKPACK / DND_BACKPACK_RATIO;
	// include holding bonuses
	res *= 1.0 + GetPlayerAttributeValue(PlayerNumber(), INV_AMMOCAP_INCREASE) + GetDataFromOrbBonus(PlayerNumber(), OBI_HOLDING, -1);
	res >>= 16;
	return res;
}

bool CheckAmmoPickup(int slot, bool simple) {
	bool res = CheckInventory(AmmoInfo_Str[slot][0][AMMOINFO_NAME]) == GetAmmoCapacity(AmmoInfo_Str[slot][0][AMMOINFO_NAME]);
	if(!simple) {
		for(int i = 0; i < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[slot][i].initial_capacity != -1; ++i)
			res = res && CheckInventory(AmmoInfo_Str[slot][i][AMMOINFO_NAME]) == GetAmmoCapacity(AmmoInfo_Str[slot][i][AMMOINFO_NAME]);
	}
	return res;
}

void HandleAmmoContainerPickup(int slot) {
	int amt = 0, index = 0;
	for(int i = 0; i < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[slot][i].initial_capacity != -1; ++i) {
		amt = AmmoInfo[slot][i].container_value;
		amt += (amt * (GetPlayerAttributeValue(PlayerNumber(), INV_AMMOGAIN_INCREASE) + CheckInventory("Perk_Munitionist") * DND_MUNITION_GAIN)) / 100;
		if(!amt)
			amt = 1;
		
		GiveInventory(AmmoInfo_Str[slot][i][AMMOINFO_NAME], amt);
	}
}

void GiveAmmo(int amt, int slot, int t) {
	if(slot != DND_AMMOSLOT_SOULS)
		amt = amt + (amt * (GetPlayerAttributeValue(PlayerNumber(), INV_AMMOGAIN_INCREASE) + CheckInventory("Perk_Munitionist") * DND_MUNITION_GAIN)) / 100;
	else
		amt = amt + (amt * CheckInventory("IATTR_SoulAmmoIncrease")) / 100;
	GiveInventory(AmmoInfo_Str[slot][t][AMMOINFO_NAME], amt);
}

#endif