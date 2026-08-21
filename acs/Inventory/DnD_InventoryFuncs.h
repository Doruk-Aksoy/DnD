#ifndef DND_INV_FUNCS_IN
#define DND_INV_FUNCS_IN

void ClearInventoryItem(inventory_T* item) {
	item.width = 0;
	item.height = 0;
	item.topleftboxid = 0;
	item.item_type = DND_ITEM_NULL;
	item.item_subtype = 0;
	item.item_image = 0;
	item.item_level = 0;
	item.item_stack = 0;

	item.corrupted = 0;
	item.quality = 0;
	item.item_base = 0;

	int k;
	for(k = 0; k < MAX_ITEM_IMPLICITS; ++k) {
		item.implicit[k].attrib_id = -1;
		item.implicit[k].attrib_val = 0;
		item.implicit[k].attrib_tier = 0;
		item.implicit[k].attrib_extra = 0;
	}

	for(k = 0; k < item.attrib_count; ++k) {
		item.attributes[k].attrib_id = 0;
		item.attributes[k].attrib_val = 0;
		item.attributes[k].attrib_tier = 0;
		item.attributes[k].fractured = 0;
		item.attributes[k].attrib_extra = 0;
	}
	item.attrib_count = 0;
}

inventory_T global:20& GetFieldItem(int pos) {
	//Log(s:"request field id: ", d:pos);
    return GlobalItemStorage.Inventories_On_Field[pos];
}

inventory_T global:20& GetUsedItem(int pnum, int slot) {
    return GlobalItemStorage.Items_Used[pnum][slot];
}

inventory_T global:20& GetPlayerInventoryItem(int pnum, int id) {
    return GlobalItemStorage.PlayerInventoryList[pnum][id];
}

inventory_T global:20& GetPlayerFlask(int pnum, int id) {
    return GlobalItemStorage.Items_Used[pnum][FLASK1_INDEX + id];
}

inventory_T global:20& GetTradeItem(int pnum, int id) {
	return GlobalItemStorage.TradeViewList[pnum][id];
}

inventory_T global:20& GetTemporaryItem(int pnum, int id) {
	return GlobalItemStorage.TemporaryInventoryList[pnum][id];
}

inventory_T global:20& GetPlayerStashItem(int pnum, int page, int id) {
	return GlobalItemStorage.PlayerStashList[pnum][page][id];
}

inventory_T global:20& GetMerchantItem(int id) {
	return GlobalItemStorage.TradeViewList[MAXPLAYERS][id];
}

inventory_T global:20& GetSortScratchItem(int id) {
	return GlobalItemStorage.SortScratchList[id];
}

inventory_T global:20& AcquireItemFromSource(int pnum, int index, int source) {
	int page = 0;
	
	if(source > 65535) {
		page = source >> 16;
		source &= 0xFFFF;
	}

	switch(source) {
		case DND_SYNC_ITEMSOURCE_ITEMSUSED:
		return GlobalItemStorage.Items_Used[pnum][index];
		case DND_SYNC_ITEMSOURCE_FIELD:
		return GlobalItemStorage.Inventories_On_Field[index];
		case DND_SYNC_ITEMSOURCE_PLAYERINVENTORY:
		return GlobalItemStorage.PlayerInventoryList[pnum][index];
		case DND_SYNC_ITEMSOURCE_TRADEVIEW:
		return GlobalItemStorage.TradeViewList[pnum][index];
		case DND_SYNC_ITEMSOURCE_STASH:
		return GlobalItemStorage.PlayerStashList[pnum][page][index];
	}

	return GlobalItemStorage.Inventories_On_Field[index];
}

// Deletes an item, essentially
void FreeItem(int pnum, int item_index, int source, bool dontSync) {
	//if(source == DND_SYNC_ITEMSOURCE_FIELD)
	//	Log(s:"free item id ", d:item_index);
	int i, j;
	int temp;
	auto item = AcquireItemFromSource(pnum, item_index, source);
	int wtemp = item.width;
	int htemp = item.height;
	
	if(IsSourceInventoryView(source)) {
		for(j = 0; j < htemp; ++j)
			for(i = 0; i < wtemp; ++i) {
				auto item_ptr = AcquireItemFromSource(pnum, item_index + j * MAXINVENTORYBLOCKS_VERT + i, source);
				item_ptr.topleftboxid = 0;
				item_ptr.item_type = DND_ITEM_NULL;
			}
	}
	
	ClearInventoryItem(item);
	
	if(!dontSync)
		SyncItemData_Null(pnum, item_index, source, wtemp, htemp);
}

void FreeSpot(int pnum, int item_index, int source, bool dontSync = true) {
	auto item = AcquireItemFromSource(pnum, item_index, source);
	ClearInventoryItem(item);

	if(!dontSync)
		SyncItemData_Null(pnum, item_index, source, 1, 1);
}

void SetItemToAnother(inventory_T* to, inventory_T* from) {
	to.width = from.width;
	to.height = from.height;
	to.item_type = from.item_type;
	to.item_subtype = from.item_subtype;
	to.item_image = from.item_image;
	to.item_level = from.item_level;
	to.item_stack = from.item_stack;
	to.attrib_count = from.attrib_count;

	to.corrupted = from.corrupted;
	to.quality = from.quality;
	to.item_base = from.item_base;

	for(int i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		to.implicit[i].attrib_id = from.implicit[i].attrib_id;
		to.implicit[i].attrib_val = from.implicit[i].attrib_val;
		to.implicit[i].attrib_tier = from.implicit[i].attrib_tier;
		to.implicit[i].attrib_extra = from.implicit[i].attrib_extra;
	}

	for(i = 0; i < to.attrib_count; ++i) {
		to.attributes[i].attrib_id = from.attributes[i].attrib_id;
		to.attributes[i].attrib_val = from.attributes[i].attrib_val;
		to.attributes[i].attrib_tier = from.attributes[i].attrib_tier;
		to.attributes[i].attrib_extra = from.attributes[i].attrib_extra;
		to.attributes[i].fractured = from.attributes[i].fractured;
	}
}

void AllocateNonPlayerItemStorageMemory() {
	// merchant
	GlobalItemStorage.TradeViewList[MAXPLAYERS] = bcs::arrNew(MAX_INVENTORY_BOXES, INVENTORY_T_INTS);

	GlobalItemStorage.Inventories_On_Field = bcs::arrNew(MAX_INVENTORIES_ON_FIELD, INVENTORY_T_INTS);

	GlobalItemStorage.SortScratchList = bcs::arrNew(MAX_INVENTORY_BOXES, INVENTORY_T_INTS);
}

void AllocateItemStorageMemory(int i) {
	GlobalItemStorage.TemporaryInventoryList[i] = bcs::arrNew(MAX_TEMP_INVENTORIES, INVENTORY_T_INTS);
	GlobalItemStorage.Items_Used[i] = bcs::arrNew(MAX_ITEMS_EQUIPPABLE, INVENTORY_T_INTS);
	GlobalItemStorage.PlayerInventoryList[i] = bcs::arrNew(MAX_INVENTORY_BOXES, INVENTORY_T_INTS);
	GlobalItemStorage.TradeViewList[i] = bcs::arrNew(MAX_INVENTORY_BOXES, INVENTORY_T_INTS);

	for(int j = 0; j < MAX_EXTRA_INVENTORY_PAGES + 1; ++j)
		GlobalItemStorage.PlayerStashList[i][j] = bcs::arrNew(MAX_INVENTORY_BOXES, INVENTORY_T_INTS);
}

#endif