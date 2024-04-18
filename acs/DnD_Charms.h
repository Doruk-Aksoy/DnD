#ifndef DND_CHARMS_IN
#define DND_CHARMS_IN

#include "DnD_InvAttribs.h"
#include "DnD_Common.h"
#include "DnD_Inventory.h"

#define NULL_CHARM -1

#define DND_CHARM_BASEHEIGHT 1
#define DND_CHARM_BASEWIDTH 1

#define DND_CHARM_SIZEFACTOR 4 // 25%

str GetCharmTypeName(int id) {
	return StrParam(s:"DND_CHARMTYPE", d:id + 1);
}

// returns type of charm as result
int ConstructCharmDataOnField(int charm_pos, int charm_tier) {
	int res = random(DND_CHARM_SMALL, DND_CHARM_LARGE);
	Inventories_On_Field[charm_pos].item_level = charm_tier;
	Inventories_On_Field[charm_pos].item_stack = 0; // charms have no stack
	Inventories_On_Field[charm_pos].item_type = DND_ITEM_CHARM;
	Inventories_On_Field[charm_pos].item_subtype = res;
	Inventories_On_Field[charm_pos].width = DND_CHARM_BASEWIDTH;
	Inventories_On_Field[charm_pos].height = DND_CHARM_BASEHEIGHT + res;

	Inventories_On_Field[charm_pos].corrupted = false;
	Inventories_On_Field[charm_pos].quality = 0;
	Inventories_On_Field[charm_pos].implicit.attrib_id = -1;
	Inventories_On_Field[charm_pos].implicit.attrib_val = 0;
	Inventories_On_Field[charm_pos].implicit.attrib_tier = 0;
	Inventories_On_Field[charm_pos].implicit.attrib_extra = 0;
	
	Inventories_On_Field[charm_pos].attrib_count = 0;
	for(int i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		Inventories_On_Field[charm_pos].attributes[i].attrib_id = -1;
		
	return res;
}

void RollCharmInfo(int charm_pos, int charm_tier, int pnum) {
	// roll random attributes for the charm
	int i = 0, roll;
	int charm_type = ConstructCharmDataOnField(charm_pos, charm_tier);
	int count = GetMaxItemAffixes(DND_ITEM_CHARM, charm_type);
	
	switch(charm_type) {
		case DND_CHARM_SMALL:
			Inventories_On_Field[charm_pos].item_image = random(DND_SMALLCHARM_IMAGEBEGIN, DND_SMALLCHARM_IMAGEEND);
		break;
		case DND_CHARM_MEDIUM:
			Inventories_On_Field[charm_pos].item_image = random(DND_MEDIUMCHARM_IMAGEBEGIN, DND_MEDIUMCHARM_IMAGEEND);
		break;
		case DND_CHARM_LARGE:
			Inventories_On_Field[charm_pos].item_image = random(DND_LARGECHARM_IMAGEBEGIN, DND_LARGECHARM_IMAGEEND);
		break;
	}
	
	while(i < count) {
		do {
			roll = PickRandomAttribute(DND_ITEM_CHARM);
		} while(CheckItemAttribute(pnum, charm_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(charm_pos, roll, pnum);
		++i;
	}
}

void RollCharmInfoWithMods(int charm_pos, int charm_tier, int m1, int m2, int m3, int pnum) {
	// roll random attributes for the charm
	int i = 0, roll;
	int charm_type = ConstructCharmDataOnField(charm_pos, charm_tier);
	int count = random(2, 2 * (charm_type + 1)) - 1 - (m2 != -1) - (m3 != -1);
	
	switch(charm_type) {
		case DND_CHARM_SMALL:
			Inventories_On_Field[charm_pos].item_image = random(DND_SMALLCHARM_IMAGEBEGIN, DND_SMALLCHARM_IMAGEEND);
		break;
		case DND_CHARM_MEDIUM:
			Inventories_On_Field[charm_pos].item_image = random(DND_MEDIUMCHARM_IMAGEBEGIN, DND_MEDIUMCHARM_IMAGEEND);
		break;
		case DND_CHARM_LARGE:
			Inventories_On_Field[charm_pos].item_image = random(DND_LARGECHARM_IMAGEBEGIN, DND_LARGECHARM_IMAGEEND);
		break;
	}
	
	// add the guaranteed mods first, because rolling later will check for their presence if they are rolled
	AddAttributeToFieldItem(charm_pos, m1, pnum);
	if(m2 != -1)
		AddAttributeToFieldItem(charm_pos, m2, pnum);
	if(m3 != -1)
		AddAttributeToFieldItem(charm_pos, m3, pnum);
	
	// the guaranteed mods are subtracted from random rolls
	// rolling logic is as follows:
	// we extend the resolution of the range (the ends weren't being picked as evenly) example: range of 0 to 5 x 10 => 0 to 50, then we divide by 10. We extend possibility of picking the ends
	while(i < count) {
		do {
			roll = PickRandomAttribute(DND_ITEM_CHARM);
		} while(CheckItemAttribute(pnum, charm_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(charm_pos, roll, pnum);
		++i;
	}
}

// creates a charm with given mods as guaranteed
void SpawnCharmWithMods(int pnum, int m1, int m2 = -1, int m3 = -1, bool noRepeat = false) {
	int c = CreateItemSpot();
	if(c != -1) {
		RollCharmInfoWithMods(c, RollItemLevel(), m1, m2, m3, pnum);
		SpawnDrop("CharmDrop", 16.0, 16, pnum + 1, c);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_CHARM);
		
		if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, STAT_LUCK) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
			SpawnCharmWithMods(pnum, m1, m2, m3, true);
	}
}

void SpawnCharmWithMods_ForAll(int m1, int m2 = -1, int m3 = -1) {
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && !PlayerIsSpectator(i))
			SpawnCharmWithMods(i, m1, m2, m3);
	}
}

void SpawnCharmForAll(int rarity_boost) {
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && !PlayerIsSpectator(i))
			SpawnCharm(i, rarity_boost);
	}
}

void ResetPlayerCharmsUsed(int pnum) {
	for(int i = 0; i < MAX_ITEMS_EQUIPPABLE; ++i) {
		if(Items_Used[pnum][i].item_type != DND_ITEM_NULL)
			SyncItemData_Null(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, Items_Used[pnum][i].width, Items_Used[pnum][i].height);
		Items_Used[pnum][i].item_type = DND_ITEM_NULL;
		Items_Used[pnum][i].width = 0;
		Items_Used[pnum][i].height = 0;
		Items_Used[pnum][i].item_image = 0;
		Items_Used[pnum][i].item_subtype = 0;
		Items_Used[pnum][i].item_level = 0;
		Items_Used[pnum][i].item_stack = 0;
		Items_Used[pnum][i].topleftboxid = 0;

		Items_Used[pnum][i].corrupted = 0;
		Items_Used[pnum][i].quality = 0;
		Items_Used[pnum][i].implicit.attrib_id = -1;
		Items_Used[pnum][i].implicit.attrib_val = 0;
		Items_Used[pnum][i].implicit.attrib_tier = 0;
		Items_Used[pnum][i].implicit.attrib_extra = 0;

		for(int j = 0; j < Items_Used[pnum][i].attrib_count; ++j) {
			Items_Used[pnum][i].attributes[j].attrib_id = 0;
			Items_Used[pnum][i].attributes[j].attrib_val = 0;
			Items_Used[pnum][i].attributes[j].attrib_tier = 0;
			Items_Used[pnum][i].attributes[j].attrib_extra = 0;
			Items_Used[pnum][i].attributes[j].fractured = 0;
		}
		Items_Used[pnum][i].attrib_count = 0;
	}
}

int CountPlayerSmallCharms(int pnum) {
	int res = 0;
	
	// first 4 boxes are for small charms, so id 0-3 are
	for(int i = 0; i < MAX_SMALL_CHARMS_USED; ++i)
		res += Items_Used[pnum][i].item_type != DND_ITEM_NULL;
	return res;
}

bool DoUniqueCheck(int pnum, int use_id, int item_index, int target_type) {
	if(target_type == DND_CHARM_LARGE)
		return false;
	// 0 - 3 are small, 4-5 are medium and 6 is large
	int id;
	if(target_type == DND_CHARM_MEDIUM) {
		// check opposite index -- if we are unique on other index, and other index's type is equal to ours literally, that must mean its a unique match
		if(use_id == 4)
			id = 5;
		else
			id = 4;
		return Items_Used[pnum][id].item_type > UNIQUE_BEGIN && PlayerInventoryList[pnum][item_index].item_type == Items_Used[pnum][id].item_type;
	}
	
	// small charm
	for(int i = 0; i < MAX_SMALL_CHARMS_USED; ++i) {
		if(i == use_id)
			continue;
		if(Items_Used[pnum][i].item_type > UNIQUE_BEGIN && PlayerInventoryList[pnum][item_index].item_type == Items_Used[pnum][i].item_type)
			return true;
	}
	return false;
}

#endif
