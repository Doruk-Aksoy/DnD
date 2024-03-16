#ifndef DND_PCORE_IN
#define DND_PCORE_IN

enum {
    PCORE_COPPER,
    PCORE_GOLD,
    PCORE_MOLYBDENUM,
};
#define POWERCORES_BEGIN PCORE_COPPER
#define POWERCORES_END PCORE_GOLD

#define MAX_POWERCORE_ATTRIBS 2

int ConstructPowercoreDataOnField(int item_pos, int item_tier) {
    // decide what type of armor to spawn here -- droppers have tiers not equal to zero, so they can determine some easy armors to drop
	int res, i;
	res = random(1, 100);
    if(res <= 50)
        res = PCORE_COPPER;
    else if(res <= 90)
        res = PCORE_GOLD;
    else
        res = PCORE_MOLYBDENUM;

	Inventories_On_Field[item_pos].item_level = item_tier;
	Inventories_On_Field[item_pos].item_stack = 0;
	Inventories_On_Field[item_pos].item_type = DND_ITEM_POWERCORE;
	Inventories_On_Field[item_pos].item_subtype = res;
	Inventories_On_Field[item_pos].width = 1;
	Inventories_On_Field[item_pos].height = 1;

	Inventories_On_Field[item_pos].corrupted = false;
	Inventories_On_Field[item_pos].quality = 0;
	Inventories_On_Field[item_pos].implicit.attrib_id = INV_IMP_POWERCORE;
	Inventories_On_Field[item_pos].implicit.attrib_val = 0;
	Inventories_On_Field[item_pos].implicit.attrib_tier = 0;
	Inventories_On_Field[item_pos].implicit.attrib_extra = 0;
	
	Inventories_On_Field[item_pos].attrib_count = 0;
	for(i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		Inventories_On_Field[item_pos].attributes[i].attrib_id = -1;

	return res;
}

int RollPowercoreInfo(int item_pos, int item_tier, int pnum) {
	// roll random attributes for the charm
	int i = 0, roll;
	int core_type = ConstructPowercoreDataOnField(item_pos, item_tier);
	int count = random(1, MAX_POWERCORE_ATTRIBS);
	int special_roll = 0;
	
	// implicits that come along with the item always
	switch(core_type) {
		case PCORE_COPPER:
			Inventories_On_Field[item_pos].item_image = IIMG_CORE_1;
			// 100 armor, 33% damage reduction
			GiveImplicitToField(item_pos, INV_IMP_POWERCORE, 10, PPOWER_ESHIELDBLOCKALL, item_tier, 5);
		break;
		case PCORE_GOLD:
			Inventories_On_Field[item_pos].item_image = IIMG_CORE_2;
			GiveImplicitToField(item_pos, INV_IMP_POWERCORE, 20, PPOWER_ESHIELDBLOCKALL, item_tier, 10);
		break;
		case PCORE_MOLYBDENUM:
			Inventories_On_Field[item_pos].item_image = IIMG_CORE_3;
			GiveImplicitToField(item_pos, INV_IMP_POWERCORE, 50, PPOWER_ESHIELDBLOCKALL, item_tier, 25);
		break;
	}
	
	while(i < count) {
		do {
			roll = PickRandomAttribute(DND_ITEM_POWERCORE, special_roll);
		} while(CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(item_pos, roll, pnum, count);
		++i;
	}

	return core_type;
}

str GetPowercoreDropClass(int type) {
	return StrParam(s:"PowercoreDrop_", d:type);
}

str GetPowercoreInventoryTag(int subt) {
	return StrParam(l:StrParam(s:"DND_PCORE", d:subt + 1));
}

Script "DnD Powercore Item Pickup" (int sp) {
    if((sp & 0xFFFF) == 255)
		SetActivatorToTarget(0);
	else
		SetActivator((sp & 0xFFFF) + P_TIDSTART);

	ACS_NamedExecuteAlways("DnD Powercore Message", 0, Inventories_On_Field[sp >> 16].item_subtype, Inventories_On_Field[sp >> 16].item_type);
    GiveInventory("PowercoreSoundPlayer", 1);
	
    HandleInventoryPickup(sp >> 16);
}

Script "DnD Powercore Message" (int id, int type) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	if(type > UNIQUE_BEGIN) {
		type = (type >> UNIQUE_BITS) - 1;
		Log(s:StrParam(s:"\cc", l:"DND_PICKUP_POWERCORE", s:": \c[Y5]", l:GetUniqueItemName(type), s:"!\c-"));
	}
	else
		Log(s:StrParam(s:"\cc", l:"DND_PICKUP_POWERCORE", s:": \c[Y5]", l:GetPowercoreInventoryTag(id), s:"!\c-"));
}

#endif