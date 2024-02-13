#ifndef DND_CHARMS_IN
#define DND_CHARMS_IN

#include "DnD_InvAttribs.h"
#include "DnD_Common.h"
#include "DnD_Inventory.h"

#define NULL_CHARM -1

#define DND_BASE_CHARMRATE 0.01

#define DND_CHARM_BASEHEIGHT 1
#define DND_CHARM_BASEWIDTH 1

#define DND_CHARM_SIZEFACTOR 4 // 25%

str GetCharmTypeName(int id) {
	return StrParam(s:"DND_CHARMTYPE", d:id + 1);
}

int Charm_MaxAffixes[MAX_CHARM_TYPES] = {
	2,
	4,
	6
};
#define MAX_CHARM_AFFIXES 6

int Charm_MaxUsable[MAX_CHARM_TYPES] = {
	4,
	2,
	1
};

// holds indexes to charms used that are on players
global inventory_T 12: Charms_Used[MAXPLAYERS][MAX_CHARMS_EQUIPPABLE];

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

int RollCharmMaxAttribCount(int charm_type) {
	return random(2, 2 * (charm_type + 1));
}

void RollCharmInfo(int charm_pos, int charm_tier, int pnum) {
	// roll random attributes for the charm
	int i = 0, roll;
	int charm_type = ConstructCharmDataOnField(charm_pos, charm_tier);
	int count = RollCharmMaxAttribCount(charm_type);
	
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

// monsters dropping charms
void SpawnCharm(int pnum, int rarity_boost, bool noRepeat = false) {
	int c = CreateItemSpot();
	if(c != -1) {
		// c is the index on the field now
		#ifndef ISDEBUGBUILD
			if((GetCVar("dnd_ignore_dropweights") && random(0, 1)) || RunDefaultDropChance(pnum, UNIQUE_DROPCHANCE * (100 + rarity_boost) / 100))
		#else
			if(random(0,1))
		#endif
		{
			MakeUnique(c, DND_ITEM_CHARM, pnum);
			SpawnDrop("UniqueCharmDrop", 16.0, 16, pnum + 1, c);
		}
		else {
			RollCharmInfo(c, RollItemLevel(), pnum);
			SpawnDrop("CharmDrop", 16.0, 16, pnum + 1, c);
		}
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_CHARM);
		
		if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, STAT_LUCK) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
			SpawnCharm(pnum, rarity_boost, true);
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

void SpawnCharm_ForAll() {
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && !PlayerIsSpectator(i))
			SpawnCharm(i, true, false);
	}
}

int MakeCharmUsed(int pnum, int use_id, int item_index, int target_type) {
	int i, j;
	
	// type mismatch, popup
	if(target_type != PlayerInventoryList[pnum][item_index].item_subtype)
		return POPUP_CHARMMISMATCH;
		
	// too high level
	if(PlayerInventoryList[pnum][item_index].item_level > GetStat(STAT_LVL))
		return POPUP_ITEMLVLTOOHIGH;
		
	// no duplicate uniques
	if(DoUniqueCheck(pnum, use_id, item_index, target_type))
		return POPUP_ONLYONEUNIQUE;
	
	// tried to put well of power, but have too many small charms -- its always attribute id 2 on well of power
	// we have to check for that specifically because technically its not equipped yet, so player has no tokens of it on them
	if
	(
		PlayerInventoryList[pnum][item_index].item_type > UNIQUE_BEGIN && 
		(PlayerInventoryList[pnum][item_index].item_type >> UNIQUE_BITS) - 1 == UITEM_WELLOFPOWER &&
		CountPlayerSmallCharms(pnum) > PlayerInventoryList[pnum][item_index].attributes[2].attrib_val
	)
		return POPUP_NOMORESMALLCHARMS;
	
	// or tried to put small charm when well of power is there and would exceed limit
	if(target_type == DND_CHARM_SMALL && (i = GetPlayerAttributeValue(pnum, INV_EX_LIMITEDSMALLCHARMS)) && i == CountPlayerSmallCharms(pnum))
		return POPUP_NOMORESMALLCHARMS;
		
	// request damage cache recalculation
	ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, PlayerNumber());
	
	// this means we must swap charms
	if(Charms_Used[pnum][use_id].item_type != DND_ITEM_NULL) {
		ApplyItemFeatures(pnum, use_id, DND_SYNC_ITEMSOURCE_CHARMUSED, DND_ITEMMOD_REMOVE);
		SwapItems(pnum, use_id, item_index, DND_SYNC_ITEMSOURCE_CHARMUSED, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
		ApplyItemFeatures(pnum, use_id, DND_SYNC_ITEMSOURCE_CHARMUSED);
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

		Charms_Used[pnum][use_id].corrupted = PlayerInventoryList[pnum][item_index].corrupted;
		Charms_Used[pnum][use_id].quality = PlayerInventoryList[pnum][item_index].quality;
		Charms_Used[pnum][use_id].implicit.attrib_id = PlayerInventoryList[pnum][item_index].implicit.attrib_id;
		Charms_Used[pnum][use_id].implicit.attrib_val = PlayerInventoryList[pnum][item_index].implicit.attrib_val;
		Charms_Used[pnum][use_id].implicit.attrib_tier = PlayerInventoryList[pnum][item_index].implicit.attrib_tier;
		Charms_Used[pnum][use_id].implicit.attrib_extra = PlayerInventoryList[pnum][item_index].implicit.attrib_extra;

		for(i = 0; i < Charms_Used[pnum][use_id].attrib_count; ++i) {
			Charms_Used[pnum][use_id].attributes[i].attrib_id = PlayerInventoryList[pnum][item_index].attributes[i].attrib_id;
			Charms_Used[pnum][use_id].attributes[i].attrib_val = PlayerInventoryList[pnum][item_index].attributes[i].attrib_val;
			Charms_Used[pnum][use_id].attributes[i].attrib_tier = PlayerInventoryList[pnum][item_index].attributes[i].attrib_tier;
			Charms_Used[pnum][use_id].attributes[i].attrib_extra = PlayerInventoryList[pnum][item_index].attributes[i].attrib_extra;
			Charms_Used[pnum][use_id].attributes[i].fractured = PlayerInventoryList[pnum][item_index].attributes[i].fractured;
		}

		// the leftover spot is a null charm
		int wtemp = PlayerInventoryList[pnum][item_index].width;
		int htemp = PlayerInventoryList[pnum][item_index].height;
		FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
		//SyncItemData(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, wtemp, htemp);
		SyncItemData(pnum, use_id, DND_SYNC_ITEMSOURCE_CHARMUSED, -1, -1);
		ApplyItemFeatures(pnum, use_id, DND_SYNC_ITEMSOURCE_CHARMUSED);
	}
	return -1;
}

void ResetPlayerCharmsUsed(int pnum) {
	for(int i = 0; i < MAX_CHARMS_EQUIPPABLE; ++i) {
		if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL)
			SyncItemData_Null(pnum, i, DND_SYNC_ITEMSOURCE_CHARMUSED, Charms_Used[pnum][i].width, Charms_Used[pnum][i].height);
		Charms_Used[pnum][i].item_type = DND_ITEM_NULL;
		Charms_Used[pnum][i].width = 0;
		Charms_Used[pnum][i].height = 0;
		Charms_Used[pnum][i].item_image = 0;
		Charms_Used[pnum][i].item_subtype = 0;
		Charms_Used[pnum][i].item_level = 0;
		Charms_Used[pnum][i].item_stack = 0;
		Charms_Used[pnum][i].topleftboxid = 0;

		Charms_Used[pnum][i].corrupted = 0;
		Charms_Used[pnum][i].quality = 0;
		Charms_Used[pnum][i].implicit.attrib_id = -1;
		Charms_Used[pnum][i].implicit.attrib_val = 0;
		Charms_Used[pnum][i].implicit.attrib_tier = 0;
		Charms_Used[pnum][i].implicit.attrib_extra = 0;

		for(int j = 0; j < Charms_Used[pnum][i].attrib_count; ++j) {
			Charms_Used[pnum][i].attributes[j].attrib_id = 0;
			Charms_Used[pnum][i].attributes[j].attrib_val = 0;
			Charms_Used[pnum][i].attributes[j].attrib_tier = 0;
			Charms_Used[pnum][i].attributes[j].attrib_extra = 0;
			Charms_Used[pnum][i].attributes[j].fractured = 0;
		}
		Charms_Used[pnum][i].attrib_count = 0;
	}
}

int CountPlayerSmallCharms(int pnum) {
	int res = 0;
	
	// first 4 boxes are for small charms, so id 0-3 are
	for(int i = 0; i < MAX_SMALL_CHARMS_USED; ++i)
		res += Charms_Used[pnum][i].item_type != DND_ITEM_NULL;
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
		return Charms_Used[pnum][id].item_type > UNIQUE_BEGIN && PlayerInventoryList[pnum][item_index].item_type == Charms_Used[pnum][id].item_type;
	}
	
	// small charm
	for(int i = 0; i < MAX_SMALL_CHARMS_USED; ++i) {
		if(i == use_id)
			continue;
		if(Charms_Used[pnum][i].item_type > UNIQUE_BEGIN && PlayerInventoryList[pnum][item_index].item_type == Charms_Used[pnum][i].item_type)
			return true;
	}
	return false;
}

#endif
