#ifndef DND_AMMO_IN
#define DND_AMMO_IN

#include "DnD_Stat.h"

#define DND_MAX_BACKPACK 25
#define DND_BACKPACK_RATIO 20

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

#define MAX_SLOTS (DND_AMMOSLOT_SOULS + 1)
#define MAX_AMMOTYPES_PER_SLOT 11

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
	AMMO_INCINERATOR,
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
	AMMO_CHARON,
	AMMO_HADES,
	AMMO_FLAYER,
	AMMO_EXSHELL,
	AMMO_SLAYER,
	
	// category 4
	AMMO_ROCKET = 0,
	AMMO_FLAKSHELL,
	AMMO_METEOR,
	AMMO_HEAVYGRENADE,
	AMMO_GRAVDIS,
	AMMO_GRENADE,
	AMMO_HMISSILE,
	AMMO_SEDRIN,
	AMMO_HAMMER,
	
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

#define AMMOSLOT_CLIP_BEGIN AMMO_CLIP
#define AMMOSLOT_CLIP_END AMMO_EBONY1
#define AMMOSLOT_CLIP2_BEGIN AMMO_EBONY2
#define AMMOSLOT_CLIP2_END AMMO_EBONY2
#define AMMOSLOT_SHELL_BEGIN AMMO_SHELL
#define AMMOSLOT_SHELL_END AMMO_EXSHELL
#define AMMOSLOT_SHELL2_BEGIN AMMO_SLAYER
#define AMMOSLOT_SHELL2_END AMMO_SLAYER
#define AMMOSLOT_ROCKET_BEGIN AMMO_ROCKET
#define AMMOSLOT_ROCKET_END AMMO_HAMMER
#define AMMOSLOT_CELL_BEGIN AMMO_CELL
#define AMMOSLOT_CELL_END AMMO_GAUSS

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
		{		"INCRAMMO",			"IncineratorAmmo"		},
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
		{		"CHARAMMO",			"CharonAmmo"			},
		{		"STRGY0",			"HadesAmmo"				},
		{		"CBWAA0",			"FlayerAmmo"			},
		{		"ESHAA0",			"ExplodingShell"		},
		{		"SLAYAM01",			"SlayerAmmo"			}
	},
	// category 3 -- rocket box
	{
		{		"ROCKA0",			"RocketAmmo"			},
		{		"FSHLA0",			"FlakShell"				},
		{		"LAAM1",			"MeteorAmmo"			},
		{		"HEGRA0",			"HeavyGrenades"			},
		{		"GRAVD0",			"GravdisAmmo"			},
		{		"GBUNA0",			"Grenades"				},
		{		"AHRLA0",			"MISAmmo"				},
		{ 		"SEDCRY",			"SedrinAmmo"			},
		{ 		"WEPICO98",			"HammerAmmo"			},
		{		"",					""						}
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
		{		"GAUSICO",			"GaussRound"			},
		{		"",					""						}
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
		{ 		50,			12		},
		{ 		200,		50		},
		{ 		160, 		40		},
		{ 		160,		40		},
		{ 		240,		48		},
		{ 		72,			18		},
		{ 		75,			25		},
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
		{		60,			25		},
		{ 		32,			8		},
		{ 		36,			9		},
		{ 		40,			15		},
		{ 		40,			15		}
	},
	// category 3 -- rocket box
	{
		{ 		50,			5		},
		{ 		40,			10		},
		{ 		40,			4		},
		{  		50,			5		},
		{ 		25,			2		},
		{  		50,			5		},
		{ 		80,			6		},
		{ 		45,			3		},
		{ 		1,			1		},
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
		{ 		75,			10		},
		{		-1,			0		}
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
	DND_MAGAZINE_HEAVYGLCOUNTER,
	DND_MAGAZINE_INCINERATOR, // not really a mag, just put here for db save
	DND_MAGAZINE_DARKLANCE // not really a mag, just put here for db save
};

#define MAXCLIPAMMOTYPES (DND_MAGAZINE_DARKLANCE + 1)
#define MAXCLIPAMMO_FIRINGMODES 6 // specialammomode_3 to goremode hold firing mode info -- thats 6
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
	"HeavyGLCounter",
	"IncineratorStacks",
	"LanceStacks"
};

// gets you the percentage to increase by
int GetAmmoCapIncrease() {
	int bpcount = CheckInventory("BackpackCounter");
	return (((100 + bpcount * DND_BACKPACK_RATIO)) * (100 + (GetPlayerAttributeValue(PlayerNumber(), INV_AMMOCAP_INCREASE)))) / 100;
}

bool IsBackpackLimitReached() {
	return CheckInventory("BackpackCounter") >= DND_MAX_BACKPACK;
}

void SetAllAmmoCapacities() {
	// last slot is for souls, we don't increase it here
	int factor = GetAmmoCapIncrease();
	for(int i = 0; i < MAX_SLOTS - 1; ++i)
		for(int j = 0; j < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[i][j].initial_capacity != -1; ++j)
			SetAmmoCapacity(AmmoInfo_Str[i][j][AMMOINFO_NAME], (AmmoInfo[i][j].initial_capacity * factor) / 100); //Check backpack orbs.
}

void SetAllAmmoCapacitiesToDefault() {
	// last slot is for souls, we don't increase it here
	for(int i = 0; i < MAX_SLOTS - 1; ++i)
		for(int j = 0; j < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[i][j].initial_capacity != -1; ++j)
			SetAmmoCapacity(AmmoInfo_Str[i][j][AMMOINFO_NAME], AmmoInfo[i][j].initial_capacity);
}

bool CheckAmmoPickup(int slot, bool simple) {
	bool res = CheckInventory(AmmoInfo_Str[slot][0][AMMOINFO_NAME]) >= GetAmmoCapacity(AmmoInfo_Str[slot][0][AMMOINFO_NAME]);
	if(!simple) {
		// start from 1, we already included 0 above
		for(int i = 1; i < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[slot][i].initial_capacity != -1; ++i)
			res = res && CheckInventory(AmmoInfo_Str[slot][i][AMMOINFO_NAME]) >= GetAmmoCapacity(AmmoInfo_Str[slot][i][AMMOINFO_NAME]);
			
		// we got a few exceptions -- everice is used in rocket slot, and cell slot has clip using weapon
		// slot 6 has rhino ar which uses clip
		// slot 3 has shocker which uses cell
		if(slot == DND_AMMOSLOT_SHELL) {
			res = res && CheckInventory(AmmoInfo_Str[DND_AMMOSLOT_CELL][AMMO_CELL][AMMOINFO_NAME]) >= GetAmmoCapacity(AmmoInfo_Str[DND_AMMOSLOT_CELL][AMMO_CELL][AMMOINFO_NAME]);
		}
		else if(slot == DND_AMMOSLOT_CELL) {
			res = res && CheckInventory(AmmoInfo_Str[DND_AMMOSLOT_CLIP][AMMO_CLIP][AMMOINFO_NAME]) >= GetAmmoCapacity(AmmoInfo_Str[DND_AMMOSLOT_CLIP][AMMO_CLIP][AMMOINFO_NAME]);
		}
		else if(slot == DND_AMMOSLOT_ROCKET) {
			res = res && CheckInventory(AmmoInfo_Str[DND_AMMOSLOT_CELL][AMMO_EVERICE][AMMOINFO_NAME]) >= GetAmmoCapacity(AmmoInfo_Str[DND_AMMOSLOT_CELL][AMMO_EVERICE][AMMOINFO_NAME]);
		}
	}
	return res;
}

int GetAmmoGainFactor() {
	return (100 + GetPlayerAttributeValue(PlayerNumber(), INV_AMMOGAIN_INCREASE) + (CheckInventory("Perk_Munitionist") * DND_MUNITION_GAIN));
}

void HandleAmmoContainerPickup(int slot, int basic_kind) {
	int amt = 0;
	if (basic_kind > 0) {
		amt = AmmoInfo[slot][0].container_value; // large pack
		if (basic_kind <= 2)
			amt /= 5; // small pack
		if (basic_kind == 1)
			amt /= 2; // dropped pack (clip only)
		GiveAmmo(amt, slot, 0);
	}
	else {
		for(int i = 0; i < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[slot][i].initial_capacity != -1; ++i)
			GiveAmmo(AmmoInfo[slot][i].container_value, slot, i);
		
		if(slot == DND_AMMOSLOT_SHELL) {
			GiveAmmo(AmmoInfo[DND_AMMOSLOT_CELL][AMMO_CELL].container_value / 2, DND_AMMOSLOT_CELL, AMMO_CELL);
		}
		else if(slot == DND_AMMOSLOT_CELL) {
			GiveAmmo(AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_CLIP].container_value / 2, DND_AMMOSLOT_CLIP, AMMO_CLIP);
		}
		else if(slot == DND_AMMOSLOT_ROCKET) {
			GiveAmmo(AmmoInfo[DND_AMMOSLOT_CELL][AMMO_EVERICE].container_value / 2, DND_AMMOSLOT_CELL, AMMO_EVERICE);
		}
	}
}

void GiveAmmo(int amt, int slot, int t) {
	if(slot != DND_AMMOSLOT_SOULS)
		amt = amt * GetAmmoGainFactor() / 100;
	else
		amt = amt * (100 + GetPlayerAttributeValue(PlayerNumber(), INV_EX_PICKUPS_MORESOUL)) / 100;
	GiveInventory(AmmoInfo_Str[slot][t][AMMOINFO_NAME], amt);
}

#endif
