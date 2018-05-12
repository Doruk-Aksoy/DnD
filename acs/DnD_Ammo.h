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
	AMMO_DEMONSEAL
};

#define MAXAMMOTYPES AMMO_DEMONSEAL + 1

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
	"DSealAmmo"
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
	200
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
	{ AMMO_CLIP, AMMO_EBONY1, AMMO_EBONY2, AMMO_RIOT, AMMO_ACID, AMMO_RUBY, AMMO_VIPER, AMMO_DEMONSEAL, -1 },
	{ AMMO_SHELL, AMMO_EXSHELL, AMMO_SLAYER, AMMO_PCANNON, AMMO_NITROGENCANISTER, -1 },
	{ AMMO_ROCKET, AMMO_GRENADE, AMMO_HMISSILE, AMMO_METEOR, -1 },
	{ AMMO_CELL, AMMO_NAIL, AMMO_LAVA, AMMO_GAUSS, AMMO_ION, AMMO_FUEL, AMMO_LIGHTNING, AMMO_EVERICE, AMMO_THUNDER }
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

#endif