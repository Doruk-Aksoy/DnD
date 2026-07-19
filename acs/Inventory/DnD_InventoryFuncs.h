#ifndef DND_INV_FUNCS_IN
#define DND_INV_FUNCS_IN

void ClearInventoryItem(inventory_T& item) {
	item.width = 1;
	item.height = 1;
	item.item_type = DND_ITEM_NULL;
	item.item_subtype = 0;
	item.item_image = 0;
	item.item_level = 0;
	item.item_stack = 0;

	item.corrupted = 0;
	item.quality = 0;
	item.item_base = 0;
	item.item_tags.allowed_tags = 0;
	item.item_tags.excluded_tags = 0;

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

inventory_T& GetFieldItem(int pos) {
    return GlobalItemStorage.Inventories_On_Field[pos];
}

inventory_T global:20& GetUsedItem(int pnum, int slot) {
    return GlobalItemStorage.Items_Used[pnum][slot];
}

inventory_T global:20& GetPlayerFlask(int pnum, int id) {
    return GlobalItemStorage.Items_Used[pnum][FLASK1_INDEX + id];
}

#endif