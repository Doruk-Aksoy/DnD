#ifndef DND_DUNGEONS_IN
#define DND_DUNGEONS_IN

#include "DnD_InvInfo.h"

void RollDungeonKeyInfo(int item_pos, int keytype, int pnum) {
	// roll random attributes for the charm
	Inventories_On_Field[item_pos].item_level = RollItemLevel();
	Inventories_On_Field[item_pos].item_stack = 0;
	Inventories_On_Field[item_pos].item_type = DND_ITEM_DUNGEONKEY;
	Inventories_On_Field[item_pos].item_subtype = keytype;
	Inventories_On_Field[item_pos].width = 1;
	Inventories_On_Field[item_pos].height = 1;
	Inventories_On_Field[item_pos].item_image = ITEM_IMAGE_DUNGEONKEY_BEGIN + keytype;

	int count = random(1, MAX_DUNGEONKEY_ATTRIB_DEFAULT);

	// assign attributes
	int i = 0;
	while(i < count) {
		int roll = -1;
		do {
			roll = PickRandomAttribute(DND_ITEM_DUNGEONKEY, keytype);
		} while(CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		
		AddAttributeToFieldItem(item_pos, roll, pnum);
		++i;
	}
}

str GetDungeonName(int id) {
	return StrParam(s:"DND_DUNGEONNAME", d:id);
}

str GetDungeonMapLump(int id) {
	// lumps start from DND01 so 1 based index
	++id;
	if(id < 10)
		return StrParam(s:"DND0", d:id);
	return StrParam(s:"DND", d:id);
}

str GetNextMapLump() {
	str curr_name = StrParam(n:PRINTNAME_LEVEL);
	
	// add +1 to the id, see if we got it
	int len = StrLen(curr_name);
	int dig_1 = GetChar(curr_name, len - 2) - '0';
	int dig_2 = Getchar(curr_name, len - 1) - '0';

	++dig_2;
	if(dig_2 > 10) {
		dig_2 %= 10;
		++dig_1;
	}

	str next_name = StrParam(s:StrLeft(curr_name, len - 2), d:dig_1, d:dig_2);

	if(HasMapLump(next_name))
		return next_name;

	// no map found, check if VR exists otherwise map is MAP01

	if(HasMapLump("VR"))
		return "VR";

	return "MAP01";
}

int PickDungeon() {
    return DND_DUNGEON_VOIDKEEP;
}

void SpawnDungeonKey(int pnum) {
	int res = 0;
	int c = CreateItemSpot();
	if(c != -1) {
		int w = random(0, 1.0);
		
        res = PickDungeon();

		// c is the index on the field now
		RollDungeonKeyInfo(c, res, pnum);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(GetInventoryName(res + DUNGEONKEY_BEGIN), 24.0, 16, pnum + 1, c);
	}
}

void ResetCurrentDungeonData() {
	int i;
	DungeonInformation.quality = 0;
	DungeonInformation.level = -1;
	DungeonInformation.dungeon_id = -1;
	DungeonInformation.next_map = "MAP01";

	for(i = 0; i < DungeonInformation.attrib_count; ++i) {
		DungeonInformation.attributes[i].attrib_id = -1;
		DungeonInformation.attributes[i].attrib_val = 0;
		DungeonInformation.attributes[i].attrib_tier = 0;
		DungeonInformation.attributes[i].attrib_extra = 0;
	}

	for(i = 0; i < DUN_UPSIDE_MAX; ++i)
		DungeonInformation.upside_vals[i] = 0;

	DungeonInformation.attrib_count = 0;
}

void SetupCurrentDungeonData(int pnum, int item_pos, int sel_dungeon_id) {
	int i;

	// initialize this from player's item token used
	DungeonInformation.dungeon_id = sel_dungeon_id;
	DungeonInformation.quality = PlayerInventoryList[pnum][item_pos].quality;
	DungeonInformation.level = PlayerInventoryList[pnum][item_pos].item_level;
	DungeonInformation.next_map = GetNextMapLump();

	DungeonInformation.attrib_count = PlayerInventoryList[pnum][item_pos].attrib_count;

	ACS_NamedExecuteWithResult("DnD Sync Current Dungeon Data - 1", DungeonInformation.dungeon_id, DungeonInformation.quality, DungeonInformation.level, DungeonInformation.attrib_count);
	
	for(i = 0; i < DungeonInformation.attrib_count; ++i) {
		DungeonInformation.attributes[i].attrib_id = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_id;

		if(!IsDungeonAttributeQualityException(DungeonInformation.attributes[i].attrib_id))
			DungeonInformation.attributes[i].attrib_val = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_val * (100 + DungeonInformation.quality) / 100;
		else
			DungeonInformation.attributes[i].attrib_val = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_val;

		// the value of extra is in << 16
		DungeonInformation.attributes[i].attrib_extra = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_extra;
		int id = DungeonInformation.attributes[i].attrib_extra & 0xFFFF;
		int val = DungeonInformation.attributes[i].attrib_extra >> 16;
		if(!IsDungeonAttributeExtraException(id))
			DungeonInformation.attributes[i].attrib_extra = id | ((val * (100 + DungeonInformation.quality) / 100) << 16);
		else
			DungeonInformation.attributes[i].attrib_extra = id | (val << 16);

		DungeonInformation.attributes[i].attrib_tier = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier;

		ACS_NamedExecuteWithResult("DnD Sync Current Dungeon Data - 2", i, DungeonInformation.attributes[i].attrib_id, DungeonInformation.attributes[i].attrib_val, DungeonInformation.attributes[i].attrib_extra);

		// add the sum to the upsides array
		DungeonInformation.upside_vals[DungeonInformation.attributes[i].attrib_extra & 0xFFFF] += DungeonInformation.attributes[i].attrib_extra >> 16;
	}
}

void SyncCurrentDungeonInformation() {
	ACS_NamedExecuteWithResult("DnD Sync Current Dungeon Data - 1", DungeonInformation.dungeon_id, DungeonInformation.quality, DungeonInformation.level, DungeonInformation.attrib_count);
	for(int i = 0; i < DungeonInformation.attrib_count; ++i)
		ACS_NamedExecuteWithResult("DnD Sync Current Dungeon Data - 2", i, DungeonInformation.attributes[i].attrib_id, DungeonInformation.attributes[i].attrib_val, DungeonInformation.attributes[i].attrib_extra);
}

Script "DnD Sync Current Dungeon Data - 1" (int dung_id, int quality, int level, int attrib_count) CLIENTSIDE {
	DungeonInformation.dungeon_id = dung_id;
	DungeonInformation.quality = quality;
	DungeonInformation.level = level;
	DungeonInformation.attrib_count = attrib_count;
	
	SetResultValue(0);
}

Script "DnD Sync Current Dungeon Data - 2" (int id, int attrib_id, int val, int extra) CLIENTSIDE {
	DungeonInformation.attributes[id].attrib_id = attrib_id;
	DungeonInformation.attributes[id].attrib_val = val;
	DungeonInformation.attributes[id].attrib_extra = extra;
	DungeonInformation.upside_vals[extra & 0xFFFF] += extra >> 16;
	SetResultValue(0);
}

// pnum, item position top box id (includes -1), selected dungeon enum
Script "DnD Dungeon Portal Create" (int pnum, int item_pos, int sel_dungeon_id) {
	SetupCurrentDungeonData(pnum, item_pos, sel_dungeon_id);

	ACS_NamedExecuteAlways("DnD Dungeon Announce", 0, pnum, sel_dungeon_id);

	// take the inventory from the player
	FreeItem(pnum, item_pos, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);

	// create the portal actor and assign the current dungeon data to it
	SpawnForced("DungeonPortal", GetActorX(0), GetActorY(0), GetActorZ(0) + 40.0);

	SetResultValue(0);
}

Script "DnD Dungeon Portal Perish" (void) {
	ResetCurrentDungeonData();
	SetResultValue(0);
}

Script "DnD Dungeon Announce" (int owning_pnum, int sel_dungeon_id) {
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i)) {
			SetActivator(i + P_TIDSTART);
			ACS_NamedExecuteAlways("DnD Announcer", 0, DND_ANNOUNCER_DUNGEONPOP | (owning_pnum << 16), sel_dungeon_id | (DungeonInformation.level << 16));
		}
	}
}

#endif