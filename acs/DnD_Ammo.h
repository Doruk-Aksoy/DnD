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
#define MAX_AMMOTYPES_PER_SLOT 16

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
#define AMMOSLOT_SHELL2_END AMMO_AXE
#define AMMOSLOT_ROCKET_BEGIN AMMO_ROCKET
#define AMMOSLOT_ROCKET_END AMMO_HAMMER
#define AMMOSLOT_CELL_BEGIN AMMO_CELL
#define AMMOSLOT_CELL_END AMMO_GAUSS

// don't make tables with strings and ints... weird fuckups happen
// replaced container value => it's calculated as follows:
// CLIPS => 25%, SHELL => 20%, ROCKETS => 10%, CELL => 33% -- min 1
typedef struct {
	int initial_capacity;
	str icon;
	str name;
} ammo_info_T;

#define AMMOINFO_ICON 0
#define AMMOINFO_NAME 1

global ammo_info_T 16: AmmoInfo[MAX_SLOTS][MAX_AMMOTYPES_PER_SLOT];

void SetupAmmoInfos() {
	// clip
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_CLIP].initial_capacity = 200;
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_CLIP].icon = "CLIPA0";
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_CLIP].name = "Clip";

	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_RUBY].initial_capacity = 175;
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_RUBY].icon = "RBAMA0";
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_RUBY].name = "RubyAmmo";

	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_VIPER].initial_capacity = 50;
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_VIPER].icon = "C_H2A1";
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_VIPER].name = "ViperAmmo";

	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_DEMONSEAL].initial_capacity = 200;
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_DEMONSEAL].icon = "DEMSICO";
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_DEMONSEAL].name = "DSealAmmo";

	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_RIOT].initial_capacity = 160;
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_RIOT].icon = "RIOTJ0";
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_RIOT].name = "RiotgunShell";

	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_ACID].initial_capacity = 160;
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_ACID].icon = "D97AA0";
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_ACID].name = "AcidAmmo";

	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_FUSIONCELL].initial_capacity = 240;
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_FUSIONCELL].icon = "FBATA0";
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_FUSIONCELL].name = "FusionCell";

	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_INCINERATOR].initial_capacity = 72;
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_INCINERATOR].icon = "INCRAMMO";
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_INCINERATOR].name = "IncineratorAmmo";

	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_DESOLATOR].initial_capacity = 125;
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_DESOLATOR].icon = "DES4Y0";
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_DESOLATOR].name = "DesolatorAmmo";

	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_EBONY1].initial_capacity = 60;
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_EBONY1].icon = "EBOPA0";
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_EBONY1].name = "EbonyAmmo";

	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_EBONY2].initial_capacity = 30;
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_EBONY2].icon = "EBOPE0";
	AmmoInfo[DND_AMMOSLOT_CLIP][AMMO_EBONY2].name = "EbonyAmmoX";

	// shell
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_SHELL].initial_capacity = 50;
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_SHELL].icon = "SHELA0";
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_SHELL].name = "Shell";

	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_EMERALDMANA].initial_capacity = 90;
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_EMERALDMANA].icon = "EMEAA0";
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_EMERALDMANA].name = "EmeraldMana";

	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_HELLSMAWAMMO].initial_capacity = 60;
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_HELLSMAWAMMO].icon = "HMAWA1";
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_HELLSMAWAMMO].name = "HellsMawAmmo";

	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_PCANNON].initial_capacity = 35;
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_PCANNON].icon = "PCNIC1";
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_PCANNON].name = "PCanAmmo";

	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_NITROGENCANISTER].initial_capacity = 40;
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_NITROGENCANISTER].icon = "D97A1";
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_NITROGENCANISTER].name = "NitrogenCanister";

	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_DEMONBLOOD].initial_capacity = 60;
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_DEMONBLOOD].icon = "DBLUD1";
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_DEMONBLOOD].name = "DemonBlood";

	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_CHARON].initial_capacity = 60;
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_CHARON].icon = "CHARAMMO";
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_CHARON].name = "CharonAmmo";

	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_HADES].initial_capacity = 32;
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_HADES].icon = "STRGY0";
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_HADES].name = "HadesAmmo";

	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_FLAYER].initial_capacity = 36;
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_FLAYER].icon = "CBWAA0";
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_FLAYER].name = "FlayerAmmo";

	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_EXSHELL].initial_capacity = 40;
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_EXSHELL].icon = "ESHAA0";
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_EXSHELL].name = "ExplodingShell";

	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_SLAYER].initial_capacity = 40;
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_SLAYER].icon = "SLAYAM01";
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_SLAYER].name = "SlayerAmmo";

	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_AXE].initial_capacity = 60;
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_AXE].icon = "AXEAMM";
	AmmoInfo[DND_AMMOSLOT_SHELL][AMMO_AXE].name = "AxeMana";

	// rocket
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_ROCKET].initial_capacity = 50;
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_ROCKET].icon = "ROCKA0";
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_ROCKET].name = "RocketAmmo";

	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_FLAKSHELL].initial_capacity = 40;
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_FLAKSHELL].icon = "FSHLA0";
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_FLAKSHELL].name = "FlakShell";

	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_METEOR].initial_capacity = 40;
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_METEOR].icon = "LAAM1";
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_METEOR].name = "MeteorAmmo";

	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_HEAVYGRENADE].initial_capacity = 50;
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_HEAVYGRENADE].icon = "HEGRA0";
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_HEAVYGRENADE].name = "HeavyGrenades";

	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_GRAVDIS].initial_capacity = 25;
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_GRAVDIS].icon = "GRAVD0";
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_GRAVDIS].name = "GravdisAmmo";

	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_GRENADE].initial_capacity = 50;
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_GRENADE].icon = "GBUNA0";
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_GRENADE].name = "Grenades";

	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_HMISSILE].initial_capacity = 80;
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_HMISSILE].icon = "AHRLA0";
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_HMISSILE].name = "MISAmmo";

	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_SEDRIN].initial_capacity = 45;
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_SEDRIN].icon = "SEDCRY";
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_SEDRIN].name = "SedrinAmmo";

	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_HAMMER].initial_capacity = 1;
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_HAMMER].icon = "WEPICO98";
	AmmoInfo[DND_AMMOSLOT_ROCKET][AMMO_HAMMER].name = "HammerAmmo";

	// cell
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_CELL].initial_capacity = 300;
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_CELL].icon = "CELLA0";
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_CELL].name = "Cell";

	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_DEVASTATOR].initial_capacity = 270;
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_DEVASTATOR].icon = "DEVIAM0";
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_DEVASTATOR].name = "DevastatorAmmo";

	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_EVERICE].initial_capacity = 240;
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_EVERICE].icon = "EICEAM1";
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_EVERICE].name = "EverIce";

	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_FUEL].initial_capacity = 250;
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_FUEL].icon = "FUAMA0";
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_FUEL].name = "Fuel";

	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_LIGHTNING].initial_capacity = 375;
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_LIGHTNING].icon = "D98AB1";
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_LIGHTNING].name = "LightningCell";

	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_NAIL].initial_capacity = 175;
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_NAIL].icon = "NPAKA0";
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_NAIL].name = "NailgunAmmo";

	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_LAVA].initial_capacity = 250;
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_LAVA].icon = "BASIA01";
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_LAVA].name = "BasiliskAmmo";

	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_ION].initial_capacity = 180;
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_ION].icon = "IONAMM1";
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_ION].name = "IonAmmo";

	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_THUNDER].initial_capacity = 200;
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_THUNDER].icon = "D95AA0";
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_THUNDER].name = "ThunderAmmo";

	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_GAUSS].initial_capacity = 75;
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_GAUSS].icon = "GAUSICO";
	AmmoInfo[DND_AMMOSLOT_CELL][AMMO_GAUSS].name = "GaussRound";

	// soul
	AmmoInfo[DND_AMMOSLOT_SOULS][AMMO_SOUL].initial_capacity = 75;
	AmmoInfo[DND_AMMOSLOT_SOULS][AMMO_SOUL].icon = "AMM3A0";
	AmmoInfo[DND_AMMOSLOT_SOULS][AMMO_SOUL].name = "Souls";
}

int GetAmmoContainerValue(int ammo_slot, int ammo) {
	int res = AmmoInfo[ammo_slot][ammo].initial_capacity;
	switch(ammo_slot) {
		case DND_AMMOSLOT_CLIP:
			res /= 4;
		break;
		case DND_AMMOSLOT_SHELL:
			res /= 5;
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
			SetAmmoCapacity(AmmoInfo[i][j].name, (AmmoInfo[i][j].initial_capacity * factor) / 100);
}

void SetAllAmmoCapacitiesToDefault() {
	// last slot is for souls, we don't increase it here
	for(int i = 0; i < MAX_SLOTS - 1; ++i)
		for(int j = 0; j < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[i][j].initial_capacity != -1; ++j)
			SetAmmoCapacity(AmmoInfo[i][j].name, AmmoInfo[i][j].initial_capacity);
}

bool CheckAmmoPickup(int slot, bool simple) {
	bool res = CheckInventory(AmmoInfo[slot][0].name) >= GetAmmoCapacity(AmmoInfo[slot][0].name);
	if(!simple) {
		// start from 1, we already included 0 above
		for(int i = 1; i < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[slot][i].initial_capacity != -1; ++i)
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
		if (basic_kind <= 2)
			amt /= 5; // small pack
		if (basic_kind == 1)
			amt /= 2; // dropped pack (clip only)
		GiveAmmo(amt, slot, 0);
	}
	else {
		for(int i = 0; i < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[slot][i].initial_capacity != -1; ++i)
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

#endif
