#ifndef DND_AMMO_IN
#define DND_AMMO_IN

#define DND_MAX_BACKPACK 30
#define DND_BACKPACK_RATIO 10

#define DND_MUNITION_GAIN 10

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
#define MAX_AMMOTYPES_PER_SLOT 16

enum {
	DND_AMMO_SECONDARYISCLIP = 1
};

// Note to self: UPDATE THE MAX INDEX DEFINES BELOW
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
	AMMO_AXE,
	
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
	AMMO_SNIPER,
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
	DND_MAGAZINE_SHELLSIZE_9,
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

#define AMMOSLOT_CLIP_BEGIN AMMO_CLIP
#define AMMOSLOT_CLIP_END AMMO_EBONY1
#define AMMOSLOT_CLIP2_BEGIN AMMO_EBONY2
#define AMMOSLOT_CLIP2_END AMMO_EBONY2
#define AMMOSLOT_CLIP_REALEND AMMO_EBONY2

#define AMMOSLOT_SHELL_BEGIN AMMO_SHELL
#define AMMOSLOT_SHELL_END AMMO_EXSHELL
#define AMMOSLOT_SHELL2_BEGIN AMMO_SLAYER
#define AMMOSLOT_SHELL2_END AMMO_AXE
#define AMMOSLOT_SHELL_REALEND AMMO_AXE

#define AMMOSLOT_ROCKET_BEGIN AMMO_ROCKET
#define AMMOSLOT_ROCKET_END AMMO_HAMMER

#define AMMOSLOT_CELL_BEGIN AMMO_CELL
#define AMMOSLOT_CELL_END AMMO_THUNDER

#define AMMOSLOT_CELL2_BEGIN AMMO_GAUSS
#define AMMOSLOT_CELL2_END AMMO_GAUSS
#define AMMOSLOT_CELL2_REALEND AMMO_GAUSS

int GetAmmoSlotMaxIndex(int ammo_slot) {
	int res = 1;

	switch(ammo_slot) {
		case DND_AMMOSLOT_CLIP:
			res = AMMOSLOT_CLIP_REALEND + 1;
		break;
		case DND_AMMOSLOT_SHELL:
			res = AMMOSLOT_SHELL_REALEND + 1;
		break;
		case DND_AMMOSLOT_ROCKET:
			res = AMMOSLOT_ROCKET_END + 1;
		break;
		case DND_AMMOSLOT_CELL:
			res = AMMOSLOT_CELL2_REALEND + 1;
		break;
	}

	return res;
}

// don't make tables with strings and ints... weird fuckups happen
// replaced container value => it's calculated as follows:
// CLIPS => 25%, SHELL => 20%, ROCKETS => 10%, CELL => 33% -- min 1
typedef struct {
	int initial_capacity;
	str icon;
	str name;
} ammo_info_T;

// +1 for magazines, in-game max of MAX_SLOTS is used for most arithmetic and ammo cap setting, but for lookups we have this +1 (saves array id)
// +32 is for the mag stuff
global ammo_info_T 16: AmmoInfo[MAX_SLOTS + 1][MAX_AMMOTYPES_PER_SLOT + 32];

// slot it belongs to
// ammo id
// cap of ammo
// icon and name are strings
#define DEFINE_AMMO_INFO(SLOT, AMMO_ID, CAP, ICON, NAME) 	AmmoInfo[SLOT][AMMO_ID].initial_capacity = CAP; \
															AmmoInfo[SLOT][AMMO_ID].icon = ICON; \
															AmmoInfo[SLOT][AMMO_ID].name = NAME

void SetupAmmoInfos() {
	// clip category
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CLIP, AMMO_CLIP, 200, "CLIPA0", "Clip");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CLIP, AMMO_RUBY, 175, "RBAMA0", "RubyAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CLIP, AMMO_VIPER, 50, "C_H2A1", "ViperAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CLIP, AMMO_DEMONSEAL, 200, "DEMSICO", "DSealAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CLIP, AMMO_RIOT, 160, "RIOTJ0", "RiotgunShell");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CLIP, AMMO_ACID, 160, "D97AA0", "AcidAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CLIP, AMMO_FUSIONCELL, 240, "FBATA0", "FusionCell");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CLIP, AMMO_INCINERATOR, 72, "INCRAMMO", "IncineratorAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CLIP, AMMO_DESOLATOR, 125, "DES4Y0", "DesolatorAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CLIP, AMMO_EBONY1, 60, "EBOPA0", "EbonyAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CLIP, AMMO_EBONY2, 30, "EBOPE0", "EbonyAmmoX");

	// shell category
	DEFINE_AMMO_INFO(DND_AMMOSLOT_SHELL, AMMO_SHELL, 50, "SHELA0", "Shell");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_SHELL, AMMO_EMERALDMANA, 90, "EMEAA0", "EmeraldMana");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_SHELL, AMMO_HELLSMAWAMMO, 60, "HMAWA1", "HellsMawAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_SHELL, AMMO_PCANNON, 35, "PCNIC1", "PCanAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_SHELL, AMMO_NITROGENCANISTER, 40, "D97A1", "NitrogenCanister");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_SHELL, AMMO_DEMONBLOOD, 60, "DBLUD1", "DemonBlood");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_SHELL, AMMO_CHARON, 60, "CHARAMMO", "CharonAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_SHELL, AMMO_HADES, 32, "STRGY0", "HadesAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_SHELL, AMMO_FLAYER, 36, "CBWAA0", "FlayerAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_SHELL, AMMO_EXSHELL, 40, "ESHAA0", "ExplodingShell");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_SHELL, AMMO_SLAYER, 40, "SLAYAM01", "SlayerAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_SHELL, AMMO_AXE, 60, "AXEAMM", "AxeMana");

	// rocket category
	DEFINE_AMMO_INFO(DND_AMMOSLOT_ROCKET, AMMO_ROCKET, 50, "ROCKA0", "RocketAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_ROCKET, AMMO_FLAKSHELL, 40, "FSHLA0", "FlakShell");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_ROCKET, AMMO_METEOR, 40, "LAAM1", "MeteorAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_ROCKET, AMMO_HEAVYGRENADE, 50, "HEGRA0", "HeavyGrenades");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_ROCKET, AMMO_GRAVDIS, 25, "GRAVD0", "GravdisAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_ROCKET, AMMO_GRENADE, 50, "GBUNA0", "Grenades");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_ROCKET, AMMO_HMISSILE, 80, "AHRLA0", "MISAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_ROCKET, AMMO_SEDRIN, 45, "SEDCRY", "SedrinAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_ROCKET, AMMO_HAMMER, 1, "WEPICO98", "HammerAmmo");

	// cell category
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CELL, AMMO_CELL, 300, "CELLA0", "Cell");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CELL, AMMO_DEVASTATOR, 270, "DEVIAM0", "DevastatorAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CELL, AMMO_EVERICE, 240, "EICEAM1", "EverIce");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CELL, AMMO_SNIPER, 40, "SNIAMM", "SniperRounds");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CELL, AMMO_FUEL, 250, "FUAMA0", "Fuel");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CELL, AMMO_LIGHTNING, 375, "D98AB1", "LightningCell");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CELL, AMMO_NAIL, 175, "NPAKA0", "NailgunAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CELL, AMMO_LAVA, 250, "BASIA01", "BasiliskAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CELL, AMMO_ION, 180, "IONAMM1", "IonAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CELL, AMMO_THUNDER, 200, "D95AA0", "ThunderAmmo");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_CELL, AMMO_GAUSS, 75, "GAUSICO", "GaussRound");

	// soul
	DEFINE_AMMO_INFO(DND_AMMOSLOT_SOULS, AMMO_SOUL, 75, "AMM3A0", "Souls");

	// magazines / clips --- secondary ammo of reloading weapons
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SPECIALAMMOMODE_3, 0, "", "SpecialAmmoMode_3");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SPECIALAMMOMODE_3X, 0, "", "SpecialAmmoMode_3X");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SPECIALAMMOMODE_4, 0, "", "SpecialAmmoMode_4");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SPECIALAMMOMODE_5, 0, "", "SpecialAmmoMode_5");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SPECIALAMMOMODE_5X, 0, "", "SpecialAmmoMode_5X");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_GOREMODE, 0, "", "GoreMode");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_BLADECHARGE, 0, "", "BladeCharge");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SAWEDOFFCOUNTER, 0, "", "SawedoffCounter");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_AKIMBOCLIPLEFT, 0, "", "AkimboClipLeft");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_AKIMBOCLIPRIGHT, 0, "", "AkimboClipRight");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_BULLETSIZE_6, 0, "", "BulletSize_6");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SHELLSIZE_2, 0, "", "ShellSize_2");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SHELLSIZE_8, 0, "", "ShellSize_8");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SHELLSIZE_8N, 0, "", "ShellSize_8N");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SHELLSIZE_9, 0, "", "ShellSize_9");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SHELLSIZE_10, 0, "", "ShellSize_10");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SHELLSIZE_12, 0, "", "ShellSize_12");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SHELLSIZE_16, 0, "", "ShellSize_16");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_SHELLSIZE_18, 0, "", "ShellSize_18");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_MGCLIP, 0, "", "MGClip");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_MGCLIP2, 0, "", "MGClip2");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_MGCLIP3, 0, "", "MGClip3");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_MGCLIP4, 0, "", "MGClip4");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_MGCLIP5, 0, "", "MGClip5");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_MGCLIP6, 0, "", "MGClip6");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_MGCLIP7, 0, "", "MGClip7");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_FUELCLIP, 0, "", "FuelClip");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_LOADEDBASILISK, 0, "", "LoadedBasilisk");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_PCANCLIP, 0, "", "PCanClip");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_RIOTGUNCLIP, 0, "", "RiotgunClip");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_ACIDCLIP, 0, "", "AcidClip");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_HEAVYGLCOUNTER, 0, "", "HeavyGLCounter");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_INCINERATOR, 0, "", "IncineratorStacks");
	DEFINE_AMMO_INFO(DND_AMMOSLOT_MAGAZINE, DND_MAGAZINE_DARKLANCE, 0, "", "LanceStacks");
}

int GetAmmoContainerValue(int ammo_slot, int ammo) {
	int res = AmmoInfo[ammo_slot][ammo].initial_capacity;
	switch(ammo_slot) {
		case DND_AMMOSLOT_CLIP:
			res /= 4;
		break;
		case DND_AMMOSLOT_SHELL:
			// 40%
			res = 2 * res / 5;
		break;
		case DND_AMMOSLOT_ROCKET:
			res /= 10;
		break;
		case DND_AMMOSLOT_CELL:
			res /= 3;
		break;
	}

	if(!res)
		res = 1;

	return res;
}

// gets you the percentage to increase by
int GetAmmoCapIncrease(int pnum) {
	int bpcount = CheckInventory("BackpackCounter");
	bpcount = (((100 + bpcount * DND_BACKPACK_RATIO)) * (100 + (GetPlayerAttributeValue(PlayerNumber(), INV_AMMOCAP_INCREASE)))) / 100;

	int temp = GetPlayerAttributeValue(pnum, INV_EX_REDUCEDAMMOCAP);
	if(temp)
		bpcount = bpcount * (100 - temp) / 100;
	return bpcount;
}

bool IsBackpackLimitReached() {
	return CheckInventory("BackpackCounter") >= DND_MAX_BACKPACK;
}

void SetAllAmmoCapacities(int pnum) {
	// last slot is for souls, we don't increase it here
	int factor = GetAmmoCapIncrease(pnum);
	for(int i = 0; i < MAX_SLOTS - 1; ++i) {
		int temp = GetAmmoSlotMaxIndex(i);
		for(int j = 0; j < temp && AmmoInfo[i][j].initial_capacity; ++j) {
			// dont change hammer cap
			if(i != DND_AMMOSLOT_ROCKET || j != AMMO_HAMMER)
				SetAmmoCapacity(AmmoInfo[i][j].name, (AmmoInfo[i][j].initial_capacity * factor) / 100);
		}
	}
}

void SetAllAmmoCapacitiesToDefault() {
	// last slot is for souls, we don't increase it here
	for(int i = 0; i < MAX_SLOTS - 1; ++i) {
		int temp = GetAmmoSlotMaxIndex(i);
		for(int j = 0; j < temp && AmmoInfo[i][j].initial_capacity; ++j)
			SetAmmoCapacity(AmmoInfo[i][j].name, AmmoInfo[i][j].initial_capacity);
	}
}

// returns true if you CANT pickup
bool CheckAmmoPickup(int slot, bool simple) {
	int pnum = PlayerNumber();
	if(GetPlayerAttributeValue(pnum, INV_EX_CANNOTPICKAMMO))
		return true;

	bool res = CheckInventory(AmmoInfo[slot][0].name) >= GetAmmoCapacity(AmmoInfo[slot][0].name);
	if(!simple) {
		// start from 1, we already included 0 above
		int temp = GetAmmoSlotMaxIndex(slot);
		for(int i = 1; i < temp && AmmoInfo[slot][i].initial_capacity; ++i)
			res = res && CheckInventory(AmmoInfo[slot][i].name) >= GetAmmoCapacity(AmmoInfo[slot][i].name);
			
		// we got a few exceptions -- everice is used in rocket slot, and cell slot has clip using weapon
		// slot 6 has rhino ar which uses clip
		// slot 3 has shocker which uses cell
		if(slot == DND_AMMOSLOT_SHELL) {
			res = res && CheckInventory(AmmoInfo[DND_AMMOSLOT_CELL][AMMO_CELL].name) >= GetAmmoCapacity(AmmoInfo[DND_AMMOSLOT_CELL][AMMO_CELL].name);
		}
		else if(slot == DND_AMMOSLOT_CELL) {
			res = res && CheckInventory(AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_CLIP].name) >= GetAmmoCapacity(AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_CLIP].name);
		}
		else if(slot == DND_AMMOSLOT_ROCKET) {
			res = res && CheckInventory(AmmoInfo[DND_AMMOSLOT_CELL][AMMO_EVERICE].name) >= GetAmmoCapacity(AmmoInfo[DND_AMMOSLOT_CELL][AMMO_EVERICE].name);
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
		amt = GetAmmoContainerValue(slot, 0); // large pack
		if (basic_kind == 2)
			amt /= 5; // small pack
		else if (basic_kind == 1)
			amt /= 10; // dropped pack (clip only)
		GiveAmmo(amt, slot, 0);
	}
	else {
		int temp = GetAmmoSlotMaxIndex(slot);
		for(int i = 0; i < temp && AmmoInfo[slot][i].initial_capacity; ++i)
			GiveAmmo(GetAmmoContainerValue(slot, i), slot, i);
		
		if(slot == DND_AMMOSLOT_SHELL) {
			GiveAmmo(GetAmmoContainerValue(DND_AMMOSLOT_CELL, AMMO_CELL) / 2, DND_AMMOSLOT_CELL, AMMO_CELL);
		}
		else if(slot == DND_AMMOSLOT_CELL) {
			GiveAmmo(GetAmmoContainerValue(DND_AMMOSLOT_CLIP, AMMO_CLIP) / 2, DND_AMMOSLOT_CLIP, AMMO_CLIP);
		}
		else if(slot == DND_AMMOSLOT_ROCKET) {
			GiveAmmo(GetAmmoContainerValue(DND_AMMOSLOT_CELL, AMMO_EVERICE) / 2, DND_AMMOSLOT_CELL, AMMO_EVERICE);
		}
	}
}

void GiveAmmo(int amt, int slot, int t) {
	if(slot != DND_AMMOSLOT_SOULS)
		amt = amt * GetAmmoGainFactor() / 100;
	else
		amt = amt * (100 + GetPlayerAttributeValue(PlayerNumber(), INV_EX_PICKUPS_MORESOUL)) / 100;
	GiveInventory(AmmoInfo[slot][t].name, amt);
}

// a fancy ammo pickup script to make it easier to add new ammo types to slots
Script "DND Ammo Container Pickup" (int ctype, int basic_kind) {
	HandleAmmoContainerPickup(ctype, basic_kind);
}

#endif
