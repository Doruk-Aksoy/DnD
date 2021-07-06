#ifndef DND_CHARMS_IN
#define DND_CHARMS_IN

#include "DnD_InvAttribs.h"
#include "DnD_Common.h"
#include "DnD_Inventory.h"

#define NULL_CHARM -1

#define DND_BASE_CHARMRATE 0.015

#define DND_CHARM_BASEHEIGHT 1
#define DND_CHARM_BASEWIDTH 1

enum {
	DND_CHARM_SMALL,
	DND_CHARM_MEDIUM,
	DND_CHARM_LARGE
};
#define MAX_CHARM_TYPES DND_CHARM_LARGE + 1

str GetCharmTypeName(int id) {
	return StrParam(s:"DND_CHARMTYPE", d:id + 1);
}

int Charm_MaxAffixes[MAX_CHARM_TYPES] = {
	2,
	4,
	6
};

int Charm_MaxUsable[MAX_CHARM_TYPES] = {
	4,
	2,
	1
};

// level 100 = perfect
str Charm_Strings[MAX_CHARM_AFFIXTIERS + 1][2] = {
	{ "\c[C8]", "DND_CHARMTIER1" },
	{ "\c[C3]", "DND_CHARMTIER2" },
	{ "\c[C1]", "DND_CHARMTIER3" },
	{ "\c[C5]", "DND_CHARMTIER4" },
	{ "\c[L7]", "DND_CHARMTIER5" }
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

void RollCharmInfo(int charm_pos, int charm_tier) {
	// roll random attributes for the charm
	int charm_type = random(DND_CHARM_SMALL, DND_CHARM_LARGE);
	int count = random(2, 2 * (charm_type + 1));
	int i = 0, roll;
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
			roll = random(FIRST_INV_ATTRIBUTE, LAST_INV_ATTRIBUTE);
		} while(CheckItemAttribute(charm_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToCharm(charm_pos, roll);
		++i;
	}
}

void RollCharmInfoWithMods(int charm_pos, int charm_tier, int m1, int m2, int m3) {
	// roll random attributes for the charm
	int charm_type = random(DND_CHARM_SMALL, DND_CHARM_LARGE);
	int count = random(2, 2 * (charm_type + 1)) - 1 - (m2 != -1) - (m3 != -1);
	int i = 0, roll;
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
	
	// the guaranteed mods are subtracted from random rolls
	while(i < count) {
		do {
			#ifdef ISDEBUGBUILD
				roll = random(INV_POISON_TICRATE, LAST_INV_ATTRIBUTE);
			#else
				roll = random(FIRST_INV_ATTRIBUTE, LAST_INV_ATTRIBUTE);
			#endif
		} while(CheckItemAttribute(charm_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToCharm(charm_pos, roll);
		++i;
	}
	
	// add the guaranteed mods
	AddAttributeToCharm(charm_pos, m1);
	if(m2 != -1)
		AddAttributeToCharm(charm_pos, m2);
	if(m3 != -1)
		AddAttributeToCharm(charm_pos, m3);
}

// can only add attributes to charms that are about to be created ie. on field dropped from monster
void AddAttributeToCharm(int charm_pos, int attrib) {
	if(Inventories_On_Field[charm_pos].attrib_count < Charm_MaxAffixes[Inventories_On_Field[charm_pos].item_subtype]) {
		int temp = Inventories_On_Field[charm_pos].attrib_count++;
		int lvl = Inventories_On_Field[charm_pos].item_level;
		
		// 10% chance to roll a tier up or down
		if(!random(0, 9))
			++lvl;
		else if(!random(0, 9))
			--lvl;
		
		// force within bounds
		lvl = Clamp_Between(lvl, 0, MAX_CHARM_AFFIXTIERS);
		
		int val = (Inv_Attribute_Info[attrib].attrib_level_modifier * lvl) / CHARM_ATTRIBLEVEL_SEPERATOR;

		Inventories_On_Field[charm_pos].attributes[temp].attrib_id = attrib;
		
		// it basically adds the step value (val) and a +1 if we aren't 0, so our range is ex: 5-10 in tier 1 then 11-15 in tier 2 assuming +5 range per tier
		Inventories_On_Field[charm_pos].attributes[temp].attrib_val = random(Inv_Attribute_Info[attrib].attrib_low + val + (val != 0), Inv_Attribute_Info[attrib].attrib_high + val);
	}
}

void AddAttributeToItem(int item_pos, int attrib) {
	int pnum = PlayerNumber();
	int temp = PlayerInventoryList[pnum][item_pos].attrib_count++;
	int lvl = PlayerInventoryList[pnum][item_pos].item_level;
	
	// 10% chance to roll a tier up or down
	if(!random(0, 9))
		++lvl;
	else if(!random(0, 9))
		--lvl;
	
	// force within bounds
	lvl = Clamp_Between(lvl, 0, MAX_CHARM_AFFIXTIERS);
	
	int val = 0;
	
	if(!Inv_Attribute_Info[attrib].attrib_level_modifier)
		val = (Inv_Attribute_Info[attrib].attrib_high - Inv_Attribute_Info[attrib].attrib_low + 1) * lvl / CHARM_ATTRIBLEVEL_SEPERATOR;
	else
		val = (Inv_Attribute_Info[attrib].attrib_level_modifier * lvl) / CHARM_ATTRIBLEVEL_SEPERATOR;
	
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_id = attrib;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_val = random(Inv_Attribute_Info[attrib].attrib_low + val, Inv_Attribute_Info[attrib].attrib_high + val);
}

// monsters dropping charms
void SpawnCharm(int pnum, bool isElite) {
	int c = CreateItemSpot();
	if(c != -1) {
		// c is the index on the field now
		int addchance = 0;
		if(isElite)
			addchance = DND_ELITE_BASEDROP / 2;

		#ifndef ISDEBUGBUILD
			if((GetCVar("dnd_ignore_dropweights") && random(0, 1)) || (!GetCVar("dnd_ignore_dropweights") && RunDefaultDropChance(pnum, isElite, UNIQUE_DROPCHANCE + addchance)))
		#else
			if(random(0,1))
		#endif
		{
			MakeUnique(c, DND_ITEM_CHARM);
			SpawnDrop("UniqueCharmDrop", 16.0, 16, pnum + 1, c);
		}
		else {
			RollCharmInfo(c, RollItemLevel());
			SpawnDrop("CharmDrop", 16.0, 16, pnum + 1, c);
		}
		SyncItemData(c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_CHARM);
	}
}

// creates a charm with given mods as guaranteed
void SpawnCharmWithMods(int pnum, int m1, int m2 = -1, int m3 = -1) {
	int c = CreateItemSpot();
	if(c != -1) {
		RollCharmInfoWithMods(c, RollItemLevel(), m1, m2, m3);
		SpawnDrop("CharmDrop", 16.0, 16, pnum + 1, c);
		SyncItemData(c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_CHARM);
	}
}

void SpawnCharmWithMods_ForAll(int m1, int m2 = -1, int m3 = -1) {
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && !PlayerIsSpectator(i))
			SpawnCharmWithMods(i, m1, m2, m3);
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
		// request damage cache recalculation
		ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, PlayerNumber());
		// this means we must swap charms
		if(Charms_Used[pnum][use_id].item_type != DND_ITEM_NULL) {
			RemoveItemFeatures(use_id, DND_SYNC_ITEMSOURCE_CHARMUSED);
			SwapItems(use_id, item_index, DND_SYNC_ITEMSOURCE_CHARMUSED, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
			ApplyItemFeatures(use_id, DND_SYNC_ITEMSOURCE_CHARMUSED);
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
			ApplyItemFeatures(use_id, DND_SYNC_ITEMSOURCE_CHARMUSED);
		}
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
