#ifndef DND_ELIXIR_IN
#define DND_ELIXIR_IN

#include "DnD_Stat.h"

#define DND_ELIXIR_CHESTDROPRATE 0.25
#define DND_ELIXIR_DROPRATE 0.05

enum {
	DND_ELIXIR_HEALTH,
	DND_ELIXIR_ARMOR,
	DND_ELIXIR_HPARMOR,
	DND_ELIXIR_HPPERCENT,
	DND_ELIXIR_ARMORPERCENT,
	DND_ELIXIR_HPARMORPERCENT,
	DND_ELIXIR_SPEED,
	DND_ELIXIR_DAMAGE,
	DND_ELIXIR_LUCK
};
#define MAX_ELIXIRS DND_ELIXIR_LUCK + 1

#define SIMPLE_ELIXIR_AMOUNT 5
#define PERCENT_ELIXIR_AMOUNT 1
#define SIMPLE_ELIXIR_AMOUNT_F 0.1

#define ELIXIR_NAME 0
#define ELIXIR_TAG 1
str ElixirList[MAX_ELIXIRS][2] = {
	{ "ElixirOfHealth", "Elixir of Health" },
	{ "ElixirOfArmor", "Elixir of Armor" },
	{ "ElixirOfProsperity", "Elixir of Prosperity" },
	{ "ElixirOfLife", "Elixir of Life" },
	{ "ElixirOfStrongness", "Elixir of Strongness" },
	{ "ElixirOfFortitude", "Elixir of Fortitude" },
	{ "ElixirOfSpeed", "Elixir of Speed" },
	{ "ElixirOfDamage", "Elixir of Damage" },
	{ "ElixirOfLuck", "Elixir of Luck" }
};

/*
percentages

25
25
12.5
12.5
12.5
7.5
2.5
2
0.5
*/

#define ELIXIR_MAXWEIGHT 1000
int ElixirDropWeights[MAX_ELIXIRS] = {
	250,
	500,
	625,
	750,
	875,
	950,
	975,
	995,
	1000
};

void HandleElixirPickup(int eid) {
	int pnum = PlayerNumber();
	switch(eid) {
		case DND_ELIXIR_HEALTH:
			Player_Bonuses[pnum].hp_flat_bonus += SIMPLE_ELIXIR_AMOUNT;
			SyncClientsideVariable(DND_SYNC_HPFLAT_BONUS, -1, DND_SYNC_NONORB);
		break;
		case DND_ELIXIR_ARMOR:
			Player_Bonuses[pnum].armor_flat_bonus += SIMPLE_ELIXIR_AMOUNT;
			SyncClientsideVariable(DND_SYNC_ARMORFLAT_BONUS, -1, DND_SYNC_NONORB);
		break;
		case DND_ELIXIR_HPARMOR:
			Player_Bonuses[pnum].hp_flat_bonus += SIMPLE_ELIXIR_AMOUNT;
			Player_Bonuses[pnum].armor_flat_bonus += SIMPLE_ELIXIR_AMOUNT;
			SyncClientsideVariable(DND_SYNC_HPFLAT_BONUS, -1, DND_SYNC_NONORB);
			SyncClientsideVariable(DND_SYNC_ARMORFLAT_BONUS, -1, DND_SYNC_NONORB);
		break;
		case DND_ELIXIR_HPPERCENT:
			Player_Bonuses[pnum].hp_percent_bonus += PERCENT_ELIXIR_AMOUNT;
			SyncClientsideVariable(DND_SYNC_HPPERCENT_BONUS, -1, DND_SYNC_NONORB);
		break;
		case DND_ELIXIR_ARMORPERCENT:
			Player_Bonuses[pnum].armor_percent_bonus += PERCENT_ELIXIR_AMOUNT;
			SyncClientsideVariable(DND_SYNC_ARMORPERCENT_BONUS, -1, DND_SYNC_NONORB);
		break;
		case DND_ELIXIR_HPARMORPERCENT:
			Player_Bonuses[pnum].hp_percent_bonus += PERCENT_ELIXIR_AMOUNT;
			Player_Bonuses[pnum].armor_percent_bonus += PERCENT_ELIXIR_AMOUNT;
			SyncClientsideVariable(DND_SYNC_HPPERCENT_BONUS, -1, DND_SYNC_NONORB);
			SyncClientsideVariable(DND_SYNC_ARMORPERCENT_BONUS, -1, DND_SYNC_NONORB);
		break;
		case DND_ELIXIR_SPEED:
			Player_Bonuses[pnum].speed_bonus += SIMPLE_ELIXIR_AMOUNT_F;
			SyncClientsideVariable(DND_SYNC_SPEED, -1, DND_SYNC_NONORB);
		break;
		case DND_ELIXIR_DAMAGE:
			for(int i = 0; i < MAX_TALENTS; ++i) {
				Player_Bonuses[pnum].damage_type_bonus[i] += PERCENT_ELIXIR_AMOUNT;
				SyncClientsideVariable(DND_SYNC_DAMAGEBULLET + i, -1, DND_SYNC_NONORB);
			}
		break;
		case DND_ELIXIR_LUCK:
			Player_Bonuses[pnum].luck += SIMPLE_ELIXIR_AMOUNT;
			SyncClientsideVariable(DND_SYNC_LUCK, -1, DND_SYNC_NONORB);
		break;
	}
}

void SpawnElixir(int pnum) {
	int w = random(1, ELIXIR_MAXWEIGHT), i = 0;
	for(; i < MAX_ELIXIRS && ElixirDropWeights[i] < w; ++i);
	SpawnDrop(StrParam(s:ElixirList[i][ELIXIR_NAME], s:"_Drop"), 24.0, 16, pnum + 1, i);
}

void HandleElixirDrop(bool isChest) {
	int chance = isChest ? DND_ELIXIR_CHESTDROPRATE : DND_ELIXIR_DROPRATE;
	for(int i = 0; i < MAXPLAYERS; ++i) {
		// run each player's chance, drop for corresponding player only
		if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART) && RunDefaultDropChance(i, false, chance))
			SpawnElixir(i);
	}
}

#endif