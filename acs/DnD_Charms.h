#ifndef DND_CHARMS_IN
#define DND_CHARMS_IN

#include "DnD_InvAttribs.h"
#include "DnD_Common.h"
#include "DnD_Inventory.h"

#define MAX_SMALL_CHARMS_USED 4
#define MAX_MEDIUM_CHARMS_USED 2
#define MAX_LARGE_CHARMS_USED 1
#define MAX_CHARMS_EQUIPPABLE MAX_SMALL_CHARMS_USED + MAX_MEDIUM_CHARMS_USED + MAX_LARGE_CHARMS_USED

#define SMALL_CHARM_ATTRIB_MAX 2

#define NULL_CHARM -1

#define DND_BASE_CHARMRATE 0.0075

#define DND_CHARM_BASEHEIGHT 1
#define DND_CHARM_BASEWIDTH 1

enum {
	DND_CHARM_SMALL,
	DND_CHARM_MEDIUM,
	DND_CHARM_LARGE
};

#define MAX_CHARM_TYPES DND_CHARM_LARGE + 1
str Charm_TypeName[MAX_CHARM_TYPES] = {
	"Small",
	"Medium",
	"Grand"
};

int Charm_MaxAffixes[MAX_CHARM_TYPES] = {
	2,
	3,
	5
};

int Charm_MaxUsable[MAX_CHARM_TYPES] = {
	4,
	2,
	1
};

// level 100 = perfect
str Charm_Tiers[MAX_CHARM_AFFIXTIERS + 1] = {
	"\c[C8]Whispering",
	"\c[C3]Fine",
	"\c[C1]Supreme",
	"\c[C5]Majestic",
	"\c[L7]Perfect"
};

// holds indexes to charms used that are on players
global inventory_T 12: Charms_Used[MAXPLAYERS][MAX_CHARMS_EQUIPPABLE];

void CopyCharmInfo_FieldToPlayer(int field_index, int player_index) {
	int pos = GetFreeSpotForItem(field_index, player_index, DND_SYNC_ITEMSOURCE_FIELD);
	if(pos == -1)
		return;
	// copy now
	CopyItem(true, field_index, player_index, pos);
}

void RollCharmInfo(int charm_pos, int charm_tier, bool onField) {
	// roll random attributes for the charm
	int charm_type = random(DND_CHARM_SMALL, DND_CHARM_LARGE);
	int count = random(2, 2 * (charm_type + 1)), i, roll;
	Inventories_On_Field[charm_pos].item_level = charm_tier;
	Inventories_On_Field[charm_pos].item_stack = 0; // charms have no stack
	Inventories_On_Field[charm_pos].item_type = DND_ITEM_CHARM;
	Inventories_On_Field[charm_pos].item_subtype = charm_type;
	Inventories_On_Field[charm_pos].width = DND_CHARM_BASEWIDTH;
	Inventories_On_Field[charm_pos].height = DND_CHARM_BASEHEIGHT + charm_type;
	
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
			roll = random(0, LAST_INV_ATTRIBUTE);
		} while(CheckItemAttribute(charm_pos, roll, onField, -1, count));
		AddAttributeToCharm(charm_pos, roll);
		++i;
	}
}

// can only add attributes to charms that are about to be created ie. on field dropped from monster
void AddAttributeToCharm(int charm_pos, int attrib) {
	if(Inventories_On_Field[charm_pos].attrib_count < Charm_MaxAffixes[Inventories_On_Field[charm_pos].item_subtype]) {
		int temp = Inventories_On_Field[charm_pos].attrib_count++;
		Inventories_On_Field[charm_pos].attributes[temp].attrib_id = attrib;
		Inventories_On_Field[charm_pos].attributes[temp].attrib_val = random(Inv_Attribute_Info[attrib].attrib_low, Inv_Attribute_Info[attrib].attrib_high) * (1 + (Inv_Attribute_Info[attrib].attrib_level_modifier * Inventories_On_Field[charm_pos].item_level) / CHARM_ATTRIBLEVEL_SEPERATOR);
	}
}

void AddAttributeToItem(int item_pos, int attrib) {
	int pnum = PlayerNumber();
	int temp = PlayerInventoryList[pnum][item_pos].attrib_count++;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_id = attrib;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_val = random(Inv_Attribute_Info[attrib].attrib_low, Inv_Attribute_Info[attrib].attrib_high) * (1 + (Inv_Attribute_Info[attrib].attrib_level_modifier * PlayerInventoryList[pnum][item_pos].item_level) / CHARM_ATTRIBLEVEL_SEPERATOR);
}

// monsters dropping charms
void SpawnCharm(int pnum) {
	int c = CreateItemSpot();
	if(c != -1) {
		// c is the index on the field now
		RollCharmInfo(c, RollItemLevel(), 1);
		SyncItemData(c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop("CharmDrop", 16.0, 16, pnum + 1, c);
	}
}

// move this from field to player's inventory
int HandleCharmPickup(int item_index) {
	int pcharm_index = GetFreeSpotForItem(item_index, PlayerNumber(), DND_SYNC_ITEMSOURCE_FIELD);
	CopyItem(true, item_index, PlayerNumber(), pcharm_index);
	return pcharm_index;
}

bool MakeCharmUsed(int use_id, int item_index, int target_type) {
	int pnum = PlayerNumber();
	int i, j;
	if(target_type != PlayerInventoryList[pnum][item_index].item_subtype) {
		// not matching, popup
		return false;
	}
	else {
		// this means we must swap charms
		if(Charms_Used[pnum][use_id].item_type != DND_ITEM_NULL) {
			RemoveItemFeatures(use_id, DND_SYNC_ITEMSOURCE_CHARMUSED);
			SwapItems(use_id, item_index, DND_SYNC_ITEMSOURCE_CHARMUSED, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
			ApplyItemFeatures(use_id, DND_SYNC_ITEMSOURCE_CHARMUSED, 0);
		}
		else {
			// just zero the stuff in inventory, and copy them into charms used
			Charms_Used[pnum][use_id].width = PlayerInventoryList[pnum][item_index].width;
			Charms_Used[pnum][use_id].height = PlayerInventoryList[pnum][item_index].height;
			Charms_Used[pnum][use_id].item_type = PlayerInventoryList[pnum][item_index].item_type;
			Charms_Used[pnum][use_id].item_subtype = PlayerInventoryList[pnum][item_index].item_subtype;
			Charms_Used[pnum][use_id].item_image = PlayerInventoryList[pnum][item_index].item_image;
			Charms_Used[pnum][use_id].item_level = PlayerInventoryList[pnum][item_index].item_level;
			Charms_Used[pnum][use_id].item_stack = PlayerInventoryList[pnum][item_index].item_stack;
			Charms_Used[pnum][use_id].attrib_count = PlayerInventoryList[pnum][item_index].attrib_count;
			Charms_Used[pnum][use_id].topleftboxid = use_id + 1;
			for(i = 0; i < Charms_Used[pnum][use_id].attrib_count; ++i) {
				Charms_Used[pnum][use_id].attributes[i].attrib_id = PlayerInventoryList[pnum][item_index].attributes[i].attrib_id;
				Charms_Used[pnum][use_id].attributes[i].attrib_val = PlayerInventoryList[pnum][item_index].attributes[i].attrib_val;
			}

			// the leftover spot is a null charm
			int wtemp = PlayerInventoryList[pnum][item_index].width;
			int htemp = PlayerInventoryList[pnum][item_index].height;
			FreeItem(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
			SyncItemData(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, wtemp, htemp);
			SyncItemData(use_id, DND_SYNC_ITEMSOURCE_CHARMUSED, -1, -1);
			ApplyItemFeatures(use_id, DND_SYNC_ITEMSOURCE_CHARMUSED, 0);
		}
		// save to database on use
		ACS_NamedExecuteAlways("DnD Save Player Item Data", PlayerNumber() | (CheckInventory("DnD_CharacterID") << 16), use_id, DND_SYNC_ITEMSOURCE_CHARMUSED);
		ACS_NamedExecuteAlways("DnD Save Player Item Data", PlayerNumber() | (CheckInventory("DnD_CharacterID") << 16), item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		return true;
	}
}

void ResetPlayerCharmsUsed(int pnum) {
	for(int i = 0; i < MAX_CHARMS_EQUIPPABLE; ++i) {
		if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL)
			SyncItemData_Null(i, DND_SYNC_ITEMSOURCE_CHARMUSED, Charms_Used[pnum][i].width, Charms_Used[pnum][i].height);
		Charms_Used[pnum][i].item_type = DND_ITEM_NULL;
		Charms_Used[pnum][i].width = 0;
		Charms_Used[pnum][i].height = 0;
		Charms_Used[pnum][i].item_image = 0;
		Charms_Used[pnum][i].item_type = 0;
		Charms_Used[pnum][i].item_subtype = 0;
		Charms_Used[pnum][i].item_level = 0;
		Charms_Used[pnum][i].item_stack = 0;
		Charms_Used[pnum][i].topleftboxid = 0;
		for(int j = 0; j < Charms_Used[pnum][i].attrib_count; ++j) {
			Charms_Used[pnum][i].attributes[j].attrib_id = 0;
			Charms_Used[pnum][i].attributes[j].attrib_val = 0;
		}
		Charms_Used[pnum][i].attrib_count = 0;
	}
}

#endif