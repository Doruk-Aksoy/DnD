#ifndef DND_AMMO_IN
#define DND_AMMO_IN

#include "DnD_Stat.h"

#define DND_MAX_BACKPACK 55
#define DND_BACKPACK_RATIO 5

enum {
	AMMO_CLIP,
	AMMO_SHELL,
	AMMO_ROCKET,
	AMMO_CELL,
	AMMO_GRENADE,
	AMMO_HMISSILE,
	AMMO_NAIL,
	AMMO_EXSHELL,
	AMMO_SOUL,
	AMMO_EBONY1,
	AMMO_EBONY2,
	AMMO_LAVA,
	AMMO_GAUSS,
	AMMO_SLAYER,
	AMMO_PCANNON,
	AMMO_ION,
	AMMO_FUEL,
	AMMO_METEOR,
    AMMO_LIGHTNING,
    AMMO_NITROGENCANISTER,
    AMMO_RIOT,
	AMMO_ACID,
	AMMO_EVERICE,
	AMMO_RUBY,
	AMMO_THUNDER,
	AMMO_VIPER,
	AMMO_DEMONSEAL,
	AMMO_FUSIONCELL
};

#define MAXAMMOTYPES AMMO_FUSIONCELL + 1

str AmmoTypes[MAXAMMOTYPES] = { 
	"Clip",    
	"Shell",    
	"RocketAmmo", 
	"Cell",    
	"Grenades",   
	"MISAmmo", 
	"NailgunAmmo", 
	"ExplodingShell",    
	"Souls",   
	"EbonyAmmo", 
	"EbonyAmmoX", 
	"BasiliskAmmo", 
	"GaussRound", 
	"SlayerAmmo", 
	"PCanAmmo", 
	"IonAmmo", 
	"Fuel", 
	"MeteorAmmo", 
	"LightningCell", 
	"NitrogenCanister", 
	"RiotgunShell",
	"AcidAmmo",
	"EverIce",
	"RubyAmmo",
	"ThunderAmmo",
	"ViperAmmo",
	"DSealAmmo",
	"FusionCell"
};

int InitialCapacity[MAXAMMOTYPES] = { 
	200, 
	50, 
	50, 
	300, 
	50, 
	80, 
	175, 
	40, 
	75, 
	60, 
	30, 
	250, 
	75, 
	40,
	35,
	180,
	250,
	40,
    375,
    40,
    160,
	160,
	240,
	175,
	200,
	80,
	200,
	240
};

// ratios based on initial capacities
int AmmoContainerValues[MAXAMMOTYPES] = {
	50,
	20,
	5,
	100,
	5,
	6,
	40,
	15,
	0,
	12,
	6,
	16,
	10,
	15,
	15,
	60,
	75,
	4,
	105,
	15,
	40,
	40,
	80,
	43,
	66,
	20,
	50,
	48
};

enum {
	DND_AMMOSLOT_CLIP,
	DND_AMMOSLOT_SHELL,
	DND_AMMOSLOT_ROCKET,
	DND_AMMOSLOT_CELL
};

#define MAX_SLOTS DND_AMMOSLOT_CELL + 1
#define MAX_AMMOTYPES_PER_SLOT 9
int SlotAmmos[MAX_SLOTS][MAX_AMMOTYPES_PER_SLOT] = {
	{ AMMO_CLIP, AMMO_EBONY1, AMMO_EBONY2, AMMO_RIOT, AMMO_ACID, AMMO_RUBY, AMMO_VIPER, AMMO_DEMONSEAL, AMMO_FUSIONCELL },
	{ AMMO_SHELL, AMMO_EXSHELL, AMMO_SLAYER, AMMO_PCANNON, AMMO_NITROGENCANISTER, -1 },
	{ AMMO_ROCKET, AMMO_GRENADE, AMMO_HMISSILE, AMMO_METEOR, -1 },
	{ AMMO_CELL, AMMO_NAIL, AMMO_LAVA, AMMO_GAUSS, AMMO_ION, AMMO_FUEL, AMMO_LIGHTNING, AMMO_EVERICE, AMMO_THUNDER }
};

#define MAXCLIPAMMOTYPES 28
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
	for(int j = 0; j < MAXAMMOTYPES; ++j)
		SetAmmoCapacity(AmmoTypes[j], AmmoCapWithBonuses(InitialCapacity[j] + CheckInventory("BackpackCounter") * InitialCapacity[j] / DND_BACKPACK_RATIO));
}

// when setting ammo capacity, this must be called at all costs!
int AmmoCapWithBonuses(int base) {
	// include any and all bonuses here
	base *= 1.0 + Player_Bonuses[PlayerNumber()].holding + GetDataFromOrbBonus(PlayerNumber(), OBI_HOLDING, -1);
	base >>= 16;
	return base;
}

int GetMaximumAmmoCapacity(int t) {
	int res = InitialCapacity[t] + InitialCapacity[t] * DND_MAX_BACKPACK / DND_BACKPACK_RATIO;
	// include holding bonuses
	res *= 1.0 + Player_Bonuses[PlayerNumber()].holding + GetDataFromOrbBonus(PlayerNumber(), OBI_HOLDING, -1);
	res >>= 16;
	return res;
}

bool CheckAmmoPickup(int slot, bool simple) {
	bool res = CheckInventory(AmmoTypes[SlotAmmos[slot][0]]) == GetAmmoCapacity(AmmoTypes[SlotAmmos[slot][0]]);
	if(!simple) {
		for(int i = 0; i < MAX_AMMOTYPES_PER_SLOT && SlotAmmos[slot][i] != -1; ++i)
			res = res && CheckInventory(AmmoTypes[SlotAmmos[slot][i]]) == GetAmmoCapacity(AmmoTypes[SlotAmmos[slot][i]]);
	}
	return res;
}

void HandleAmmoContainerPickup(int ctype) {
	int amt = 0, index = 0;
	for(int i = 0; i < MAX_AMMOTYPES_PER_SLOT && SlotAmmos[ctype][i] != -1; ++i) {
		index = SlotAmmos[ctype][i];
		amt = AmmoContainerValues[index];
		amt += (CheckInventory("Perk_Munitionist") * amt * DND_MUNITION_GAIN) / 100;
		if(!amt)
			amt = 1;
		
		GiveInventory(AmmoTypes[index], amt);
	}
}

#endif