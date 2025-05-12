#ifndef DND_CKEY_IN
#define DND_CKEY_IN

#include "DnD_Inventory.h"

#define DND_CHESTKEY_DROPRATE 0.00875 // overall chestkey droprate
#define DND_GOLDCHESTKEY_DROPRATE 0.1
#define DND_SILVERCHESTKEY_DROPRATE 0.3

#define MAX_BASE_CHESTCOUNT 10
#define CHEST_DROPWEIGHT 0.1
#define LOOTBOX_DROPWEIGHT 0.175
#define SILVERCHEST_DROPWEIGHT 0.37
#define GOLDCHEST_DROPWEIGHT 0.50

str ChestKeyText[MAX_CHESTKEYS] = {
	"\ccNotice        : You need a \ceBronze\c- Chest Key to open this!",
	"\ccNotice        : You need a \cuSilver\c- Chest Key to open this!",
	"\ccNotice        : You need a \cfGold\c- Chest Key to open this!"
};

bool SpawnedChests = 0;

void RollChestkeyInfo(int item_pos, int keytype, bool onField) {
	// roll random attributes for the charm
	Inventories_On_Field[item_pos].item_level = 1;
#ifdef ISDEBUGBUILD
	Inventories_On_Field[item_pos].item_stack = 100; 
#else
	Inventories_On_Field[item_pos].item_stack = 1;
#endif
	Inventories_On_Field[item_pos].item_type = DND_ITEM_CHESTKEY;
	Inventories_On_Field[item_pos].item_subtype = keytype;
	Inventories_On_Field[item_pos].width = 1;
	Inventories_On_Field[item_pos].height = 1;
	Inventories_On_Field[item_pos].attrib_count = 0;
	Inventories_On_Field[item_pos].item_image = ITEM_IMAGE_KEY_BEGIN + keytype;
}

void SpawnChestKey(int pnum) {
	int res = 0;
	int c = CreateItemSpot();
	if(c != -1) {
		// Todo: Item rarity / quant affecting this
		int w = random(0, 1.0);
		if(w < DND_GOLDCHESTKEY_DROPRATE)
			res = DND_CHESTTYPE_GOLD;
		else if(w < DND_SILVERCHESTKEY_DROPRATE)
			res = DND_CHESTTYPE_SILVER;
		else
			res = DND_CHESTTYPE_BRONZE;

		// c is the index on the field now
		RollChestkeyInfo(c, res, true);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(GetInventoryName(res + CHESTKEY_BEGIN), 24.0, 16, pnum + 1, c);
	}
}

#endif
