#ifndef DND_DUNGEONS_IN
#define DND_DUNGEONS_IN

void RollDungeonKeyInfo(int item_pos, int keytype, bool onField) {
	// roll random attributes for the charm
	Inventories_On_Field[item_pos].item_level = 1;
#ifdef ISDEBUGBUILD
	Inventories_On_Field[item_pos].item_stack = 100; 
#else
	Inventories_On_Field[item_pos].item_stack = 1;
#endif
	Inventories_On_Field[item_pos].item_type = DND_ITEM_DUNGEONKEY;
	Inventories_On_Field[item_pos].item_subtype = keytype;
	Inventories_On_Field[item_pos].width = 1;
	Inventories_On_Field[item_pos].height = 1;
	Inventories_On_Field[item_pos].attrib_count = 0;
	Inventories_On_Field[item_pos].item_image = ITEM_IMAGE_DUNGEONKEY_BEGIN + keytype;
}

int PickDungeon() {
    return 0;
}

void SpawnDungeonKey(int pnum) {
	int res = 0;
	int c = CreateItemSpot();
	if(c != -1) {
		int w = random(0, 1.0);
		
        res = PickDungeon();

		// c is the index on the field now
		RollDungeonKeyInfo(c, res, true);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(GetInventoryName(res + DUNGEONKEY_BEGIN), 24.0, 16, pnum + 1, c);
	}
}

#endif