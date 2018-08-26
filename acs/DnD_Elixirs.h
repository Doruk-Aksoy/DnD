#ifndef DND_ELIXIR_IN
#define DND_ELIXIR_IN

#include "DnD_Stat.h"

#define DND_ELIXIR_CHESTDROPRATE 0.25
#define DND_ELIXIR_DROPRATE 0.05

#define SIMPLE_ELIXIR_AMOUNT 5
#define PERCENT_ELIXIR_AMOUNT 1
#define SIMPLE_ELIXIR_AMOUNT_F 0.01

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

void HandleElixirUse(int eid) {
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
			Player_Bonuses[pnum].luck += SIMPLE_ELIXIR_AMOUNT << 16;
			SyncClientsideVariable(DND_SYNC_LUCK, -1, DND_SYNC_NONORB);
		break;
	}
}

void SpawnElixir(int pnum) {
	int c = CreateItemSpot();
	if(c != -1) {
		int w = random(1, ELIXIR_MAXWEIGHT), i = 0;
		for(; i < MAX_ELIXIRS && ElixirDropWeights[i] < w; ++i);
		RollElixirInfo(c, i, true);
		SyncItemData(c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(StrParam(s:ElixirList[i][ELIXIR_NAME], s:"_Drop"), 24.0, 16, pnum + 1, c);
	}
}

void HandleElixirDrop(bool isChest) {
	int chance = isChest ? DND_ELIXIR_CHESTDROPRATE : DND_ELIXIR_DROPRATE;
	for(int i = 0; i < MAXPLAYERS; ++i) {
		// run each player's chance, drop for corresponding player only
		if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART) && RunDefaultDropChance(i, false, chance))
			SpawnElixir(i);
	}
}

void RollElixirInfo(int item_pos, int etype, bool onField) {
	// roll random attributes for the charm
	Inventories_On_Field[item_pos].item_level = 1;
	Inventories_On_Field[item_pos].item_stack = 1; // orbs have default stack of 1
	Inventories_On_Field[item_pos].item_type = DND_ITEM_ELIXIR;
	Inventories_On_Field[item_pos].item_subtype = etype;
	Inventories_On_Field[item_pos].width = 1;
	Inventories_On_Field[item_pos].height = 1;
	Inventories_On_Field[item_pos].attrib_count = 0;
	Inventories_On_Field[item_pos].item_image = ITEM_IMAGE_ELIXIR_BEGIN + etype;
}

#endif