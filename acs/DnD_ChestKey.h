#ifndef DND_CKEY_IN
#define DND_CKEY_IN

#include "DnD_Inventory.h"

str ChestKeyText[MAX_CHESTKEYS] = {
	"\ccNotice        : You need a \ceBronze\c- Chest Key to open this!",
	"\ccNotice        : You need a \cuSilver\c- Chest Key to open this!",
	"\ccNotice        : You need a \cfGold\c- Chest Key to open this!"
};

bool SpawnedChests = 0;

void RollChestkeyInfo(int item_pos, int keytype, bool onField) {
	// roll random attributes for the charm
	Inventories_On_Field[item_pos].item_level = 1;
	Inventories_On_Field[item_pos].item_stack = 1; // orbs have default stack of 1
	Inventories_On_Field[item_pos].item_type = DND_ITEM_CHESTKEY;
	Inventories_On_Field[item_pos].item_subtype = keytype;
	Inventories_On_Field[item_pos].width = 1;
	Inventories_On_Field[item_pos].height = 1;
	Inventories_On_Field[item_pos].attrib_count = 0;
	Inventories_On_Field[item_pos].item_image = ITEM_IMAGE_KEY_BEGIN + keytype;
}

void HandleChestKeyDrop(bool isElite) {
#ifdef ISDEBUGBUILD
	SpawnChestKey(0, true);
#endif

	for(int i = 0; i < MAXPLAYERS; ++i) {
		// run each player's chance, drop for corresponding player only
		if(PlayerInGame(i) && (GetCVar("dnd_ignore_dropweights") || (IsActorAlive(i + P_TIDSTART) && RunDefaultDropChance(i, isElite, DND_CHESTKEY_DROPRATE)))) {
			SpawnChestKey(i, isElite);
			
			// check for luck mastery on players
			if(HasActorMasteredPerk(i + P_TIDSTART, STAT_LUCK) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
				SpawnChestKey(i, isElite);
		}
	}
}

void SpawnChestKey(int pnum, bool isElite) {
	int res = 0;
	int c = CreateItemSpot();
	if(c != -1) {
		if(RunDefaultDropChance(pnum, isElite, DND_GOLDCHESTKEY_DROPRATE))
			res = DND_CHESTTYPE_GOLD;
		else if(RunDefaultDropChance(pnum, isElite, DND_SILVERCHESTKEY_DROPRATE))
			res = DND_CHESTTYPE_SILVER;
		else
			res = DND_CHESTTYPE_BRONZE;

		// c is the index on the field now
		RollChestkeyInfo(c, res, true);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(InventoryInfo[res + CHESTKEY_BEGIN], 24.0, 16, pnum + 1, c);
	}
}

#endif
