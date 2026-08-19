#ifndef DND_CHARMS_IN
#define DND_CHARMS_IN

#include "DnD_InvAttribs.h"
#include "DnD_Inventory.h"

#define NULL_CHARM -1

#define DND_CHARM_BASEHEIGHT 1
#define DND_CHARM_BASEWIDTH 1

#define DND_CHARM_SIZEFACTOR 4 // 25%

#define DND_CHARM_BAGCOLORSMAX 3
#define DND_CHARM_SPECIALTYBOOST_BASE 10 // 10%

str GetCharmDropLabel(int worth) {
	if(worth > DND_CHARM_BAGCOLORSMAX)
		worth = DND_CHARM_BAGCOLORSMAX;

	if(!worth)
		return "CharmDrop";
	return StrParam(s:"CharmDrop_Synergy", d:worth);
}

str GetCharmBoxLabel(int charm_type, bool isSelected) {
	switch(charm_type) {
		case DND_CHARM_SMALL:
			if(!isSelected)
				return "SCHNOR";
			return "SCHSEL";
		case DND_CHARM_MEDIUM:
			if(!isSelected)
				return "MCHNOR";
			return "MCHSEL";
		case DND_CHARM_LARGE:
			if(!isSelected)
				return "LCHNOR";
			return "LCHSEL";
	}
	return "SCHNOR";
}

// returns type of charm as result
int ConstructCharmDataOnField(int charm_pos, int charm_tier) {
	if(charm_tier > GetCVar("dnd_maxmonsterlevel"))
		charm_tier = GetCVar("dnd_maxmonsterlevel");

	int res = random(DND_CHARM_SMALL, DND_CHARM_LARGE);
	auto item = GetFieldItem(charm_pos);
	item.item_level = charm_tier;
	item.item_stack = 0; // charms have no stack
	item.item_type = DND_ITEM_CHARM;
	item.item_subtype = res;
	item.width = DND_CHARM_BASEWIDTH;
	item.height = DND_CHARM_BASEHEIGHT + res;

	item.corrupted = false;
	item.quality = 0;

	int i;
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		item.implicit[i].attrib_id = -1;
		item.implicit[i].attrib_val = 0;
		item.implicit[i].attrib_tier = 0;
		item.implicit[i].attrib_extra = 0;
	}
	
	item.attrib_count = 0;
	for(i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		item.attributes[i].attrib_id = -1;

	switch(res) {
		case DND_CHARM_SMALL:
			item.item_image = random(DND_SMALLCHARM_IMAGEBEGIN, DND_SMALLCHARM_IMAGEEND);
		break;
		case DND_CHARM_MEDIUM:
			item.item_image = random(DND_MEDIUMCHARM_IMAGEBEGIN, DND_MEDIUMCHARM_IMAGEEND);
		break;
		case DND_CHARM_LARGE:
			item.item_image = random(DND_LARGECHARM_IMAGEBEGIN, DND_LARGECHARM_IMAGEEND);
		break;
	}
		
	return res;
}

int RollCharmInfo(int charm_pos, int charm_tier, int pnum, int synergy_boost = -1) {
	// roll random attributes for the charm
	int i = 0, roll;
	int charm_type = ConstructCharmDataOnField(charm_pos, charm_tier);
	int count = random(1, GetMaxItemAffixes(DND_ITEM_CHARM, charm_type));

	int synergy_roll = -2;

	int worth = 0;

	int max_tries = 10;
	
	while(i < count) {
		do {
			roll = PickRandomAttribute(DND_ITEM_CHARM, charm_type, 0, -1, synergy_roll, DND_ITEMBASE_CHARM);

			if(max_tries-- < 0)
				synergy_roll = -2;
			else if(synergy_roll != -2)
				++worth;

		} while(CheckItemAttribute(pnum, charm_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(charm_pos, roll, pnum);

		synergy_roll = CheckItemSynergy(synergy_roll, charm_pos, synergy_boost);

		++i;
	}

	return worth;
}

void RollCharmInfoWithMods(int charm_pos, int charm_tier, int m1, int m2, int m3, int pnum) {
	// roll random attributes for the charm
	int i = 0, roll;
	int charm_type = ConstructCharmDataOnField(charm_pos, charm_tier);
	int count = random(0, GetMaxItemAffixes(DND_ITEM_CHARM, charm_type) - 1 - (m2 != -1) - (m3 != -1));
	
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
			roll = PickRandomAttribute(DND_ITEM_CHARM, DND_CHARM_SMALL, 0, -1, -2, DND_ITEMBASE_CHARM);
		} while(roll != -1 && CheckItemAttribute(pnum, charm_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);

		if(roll == -1)
			break;

		AddAttributeToFieldItem(charm_pos, roll, pnum);
		++i;
	}
}

// creates a charm with given mods as guaranteed
void SpawnCharmWithMods(int pnum, int m1, int m2 = -1, int m3 = -1, int max_lvl = MAX_REGULAR_ILVL) {
	int c = CreateItemSpot();
	if(c != -1) {
		RollCharmInfoWithMods(c, RollItemLevel(max_lvl), m1, m2, m3, pnum);
		SpawnDrop("CharmDrop", 16.0, 16, pnum + 1, c);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_CHARM);
	}
}

void SpawnCharmWithMods_ForAll(int m1, int m2 = -1, int m3 = -1, int max_lvl = MAX_REGULAR_ILVL) {
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && !PlayerIsSpectator(i))
			SpawnCharmWithMods(i, m1, m2, m3, max_lvl);
	}
}

void SpawnCharmForAll(int rarity_boost, int synergy_boost = -1, int max_lvl = MAX_REGULAR_ILVL) {
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && !PlayerIsSpectator(i))
			SpawnCharm(i, rarity_boost, max_lvl, false, synergy_boost);
	}
}

int CountPlayerSmallCharms(int pnum) {
	int res = 0;
	
	// first 4 boxes are for small charms, so id 0-3 are
	for(int i = 0; i < MAX_SMALL_CHARMS_USED; ++i)
		res += GlobalItemStorage.Items_Used[pnum][i].item_type != DND_ITEM_NULL;
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
		return GlobalItemStorage.Items_Used[pnum][id].item_type > UNIQUE_BEGIN && GlobalItemStorage.PlayerInventoryList[pnum][item_index].item_type == GlobalItemStorage.Items_Used[pnum][id].item_type;
	}
	
	// small charm
	for(int i = 0; i < MAX_SMALL_CHARMS_USED; ++i) {
		if(i == use_id)
			continue;
		if(GlobalItemStorage.Items_Used[pnum][i].item_type > UNIQUE_BEGIN && GlobalItemStorage.PlayerInventoryList[pnum][item_index].item_type == GlobalItemStorage.Items_Used[pnum][i].item_type)
			return true;
	}
	return false;
}

#endif
