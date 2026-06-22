#ifndef DND_DUNGEONS_IN
#define DND_DUNGEONS_IN

#include "DnD_InvInfo.h"

enum {
	DND_DUNGEON_VOIDKEEP,
};
#define DND_FIRST_DUNGEONID DND_DUNGEON_VOIDKEEP

void RollDungeonKeyInfo(int item_pos, int keytype, bool onField) {
	// roll random attributes for the charm
	Inventories_On_Field[item_pos].item_level = RollItemLevel();
	Inventories_On_Field[item_pos].item_stack = 0;
	Inventories_On_Field[item_pos].item_type = DND_ITEM_DUNGEONKEY;
	Inventories_On_Field[item_pos].item_subtype = keytype;
	Inventories_On_Field[item_pos].width = 1;
	Inventories_On_Field[item_pos].height = 1;
	Inventories_On_Field[item_pos].attrib_count = 0;
	Inventories_On_Field[item_pos].item_image = ITEM_IMAGE_DUNGEONKEY_BEGIN + keytype;
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
		RollDungeonKeyInfo(c, res, true);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(GetInventoryName(res + DUNGEONKEY_BEGIN), 24.0, 16, pnum + 1, c);
	}
}

void ResetCurrentDungeonData() {
	DungeonInformation.quality = 0;
	DungeonInformation.level = -1;
	DungeonInformation.dungeon_id = -1;
	DungeonInformation.next_map = "MAP01";

	for(int i = 0; i < DungeonInformation.attrib_count; ++i) {
		DungeonInformation.attributes[i].attrib_id = -1;
		DungeonInformation.attributes[i].attrib_val = 0;
		DungeonInformation.attributes[i].attrib_tier = 0;
		DungeonInformation.attributes[i].attrib_extra = 0;
	}

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
	
	for(i = 0; i < DungeonInformation.attrib_count; ++i) {
		DungeonInformation.attributes[i].attrib_id = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_id;
		DungeonInformation.attributes[i].attrib_val = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_val;
		DungeonInformation.attributes[i].attrib_tier = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier;
		DungeonInformation.attributes[i].attrib_extra = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_extra;
	}
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