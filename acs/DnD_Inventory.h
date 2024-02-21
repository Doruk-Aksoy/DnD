#ifndef DND_INVENTORY_IN
#define DND_INVENTORY_IN

#include "DnD_InvInfo.h"
#include "DnD_UniqueItems.h"
#include "DnD_Hud.h"
#include "DnD_WeaponDefs.h"

#define DND_ITEMMOD_ADD FALSE
#define DND_ITEMMOD_REMOVE TRUE

#define MAX_ITEM_LEVEL 100
#define MAX_ITEM_AFFIXTIERS 4
#define ITEM_TIER_SEPERATOR (100 / MAX_ITEM_AFFIXTIERS)

#define DND_SYNC_ITEMBEGIN DND_SYNC_ITEMTOPLEFTBOX
#define DND_SYNC_ITEMEND DND_SYNC_ITEMATTRIBUTES_TIER

#define INVENTORY_INFO_ALPHA 0.666
#define INVENTORY_HOLDTIME 0.5
#define INVENTORY_FADETIME 0.5

#define DND_BASE_DISASSEMBLE_COST 1000
#define DND_DISASSEMBLE_LEVEL_PERCENT 25
#define DND_DISASSEMBLE_TIER_PERCENT 30
#define DND_BASE_DISASSEMBLE_CHANCE 25 // 25%
#define DND_BASE_DISASSEMBLE_CHANCE_PERLUCK 10 // 20%
#define DND_BASE_FRACTURE_DISASSEMBLE_CHANCE 3
#define DND_BASE_CORRUPT_DISASSEMBLE_CHANCE 5
#define DND_BASE_ILVL_YIELD 10
#define DND_BASE_AVGMOD_YIELD 30
#define DND_BASE_FRACTURE_YIELD 50
#define DND_BASE_CORRUPT_YIELD 100

#define MAX_EXTRA_INVENTORY_PAGES 10

#define MAXSTACKS_ORB 128
#define MAXSTACKS_CKEY 32
#define MAXSTACKS_TOKEN 30

#define HUD_DII_FIELD_MULT 10

#define MAX_EXPRESIST_VAL 100
#define MAX_ELEMRESIST_VAL 100
#define MAX_PHYSRESIST_VAL 100
#define MAX_REFLRESIST_VAL 1000 // 1000 because we use 1 as 0.1% so 1000 is 100%
#define MAX_WEAKEN_VAL 200

#define MAX_SMALL_CHARMS_USED 4
#define MAX_MEDIUM_CHARMS_USED 2
#define MAX_LARGE_CHARMS_USED 1
#define MAX_ARMORS_USED 3 // BOOT BODY NECKLACE
#define MAX_ITEMS_EQUIPPABLE (MAX_SMALL_CHARMS_USED + MAX_MEDIUM_CHARMS_USED + MAX_LARGE_CHARMS_USED + MAX_ARMORS_USED)

enum {
	SMALLCHARM_INDEX1,
	SMALLCHARM_INDEX2,
	SMALLCHARM_INDEX3,
	SMALLCHARM_INDEX4,
	MEDIUMCHARM_INDEX1,
	MEDIUMCHARM_INDEX2,
	LARGECHARM_INDEX,
	BODY_ARMOR_INDEX,
	BOOT_INDEX
};

enum {
	IPROCESS_ADD,
	IPROCESS_REMOVE
};

typedef struct imove {
	int width;
	int height;
	int dest_pos;
	bool state;
} imove_T;

imove_T ItemMoveList[MAXPLAYERS][MAX_INVENTORY_BOXES];

enum {
	IIMG_SC_1,
	IIMG_SC_2,
	IIMG_SC_3,
	
	IIMG_MC_1,
	IIMG_MC_2,
	IIMG_MC_3,
	
	IIMG_LC_1,
	IIMG_LC_2,
	IIMG_LC_3,
	
	// unique charm images
	IIMG_UCHRM_1,
	IIMG_UCHRM_2,
	IIMG_UCHRM_3,
	IIMG_UCHRM_4,
	IIMG_UCHRM_5,
	IIMG_UCHRM_6,
	IIMG_UCHRM_7,
	IIMG_UCHRM_8,
	IIMG_UCHRM_9,
	IIMG_UCHRM_10,
	IIMG_UCHRM_11,
	IIMG_UCHRM_12,
	IIMG_UCHRM_13,
	IIMG_UCHRM_14,
	
	IIMG_ORB_1,
	IIMG_ORB_2,
	IIMG_ORB_3,
	IIMG_ORB_4,
	IIMG_ORB_5,
	IIMG_ORB_6,
	IIMG_ORB_7,
	IIMG_ORB_8,
	IIMG_ORB_9,
	IIMG_ORB_10,
	IIMG_ORB_11,
	IIMG_ORB_12,
	IIMG_ORB_13,
	IIMG_ORB_14,
	IIMG_ORB_15,
	IIMG_ORB_16,
	IIMG_ORB_17,
	IIMG_ORB_18,
	IIMG_ORB_19,
	IIMG_ORB_20,
	IIMG_ORB_21,
	IIMG_ORB_22,
	IIMG_ORB_23,
	IIMG_ORB_24,
	IIMG_ORB_25,
	IIMG_ORB_26,
	IIMG_ORB_27,

	// monster specific orb drops
	IIMG_MORB_1,
	IIMG_MORB_2,
	IIMG_MORB_3,

	// armor
	IIMG_ARM_1,
	IIMG_ARM_2,
	IIMG_ARM_3,
	IIMG_ARM_4,

	IIMG_ARM_5,
	IIMG_ARM_6,
	IIMG_ARM_7,
	IIMG_ARM_8,
	IIMG_ARM_9,

	IIMG_ARM_10,
	IIMG_ARM_11,
	IIMG_ARM_12,
	IIMG_ARM_13,
	IIMG_ARM_14,
	IIMG_ARM_15,

	IIMG_ARM_16,
	IIMG_ARM_17,
	
	IIMG_CKEY_1,
	IIMG_CKEY_2,
	IIMG_CKEY_3,
	
	IIMG_TOKEN_REPAIR,
	IIMG_TOKEN_SCOUR
};
#define MAX_ITEM_IMAGES (IIMG_TOKEN_SCOUR + 1)

#define DND_SMALLCHARM_IMAGEBEGIN IIMG_SC_1
#define DND_SMALLCHARM_IMAGEEND IIMG_SC_3
#define DND_MEDIUMCHARM_IMAGEBEGIN IIMG_MC_1
#define DND_MEDIUMCHARM_IMAGEEND IIMG_MC_3
#define DND_LARGECHARM_IMAGEBEGIN IIMG_LC_1
#define DND_LARGECHARM_IMAGEEND IIMG_LC_3

#define ITEM_IMAGE_ORB_BEGIN IIMG_ORB_1
#define ITEM_IMAGE_KEY_BEGIN IIMG_CKEY_1
#define ITEM_IMAGE_TOKEN_BEGIN IIMG_TOKEN_REPAIR

#define ITEM_IMAGE_ARMOR_BEGIN IIMG_ARM_1
#define ITEM_IMAGE_ARMOR_END IIMG_ARM_17
#define ITEM_IMAGE_UCHARM_BEGIN IIMG_UCHRM_1
#define ITEM_IMAGE_MONSTERORB_BEGIN IIMG_MORB_1

#define ITEM_IMAGE_CHARM_END IIMG_LC_3
#define ITEM_IMAGE_UCHARM_END IIMG_UCHRM_14
#define ITEM_IMAGE_ORB_END IIMG_ORB_27
#define ITEM_IMAGE_MONSTERORB_END IIMG_MORB_3
#define ITEM_IMAGE_KEY_END IIMG_CKEY_3
#define ITEM_IMAGE_TOKEN_END IIMG_TOKEN_SCOUR

#include "DnD_Armor.h"

// wide returns wider version
str GetItemImage(int id, bool wide = false) {
	str img_prefix = "";
	int suffix = 0;
	if(id <= ITEM_IMAGE_CHARM_END) {
		img_prefix = "C";
		suffix = id + 1;
	}
	else if(id <= ITEM_IMAGE_UCHARM_END) {
		img_prefix = "UC";
		suffix = id - ITEM_IMAGE_UCHARM_BEGIN + 1;
	}
	else if(id <= ITEM_IMAGE_ORB_END) {
		img_prefix = "O";
		suffix = id - ITEM_IMAGE_ORB_BEGIN + 1;
	}
	else if(id <= ITEM_IMAGE_MONSTERORB_END) {
		img_prefix = "MO";
		suffix = id - ITEM_IMAGE_MONSTERORB_BEGIN + 1;
	}
	else if(id <= ITEM_IMAGE_ARMOR_END) {
		img_prefix = "AR";
		suffix = id - ITEM_IMAGE_ARMOR_BEGIN + 1;
		if(wide)
			img_prefix = "ARW";
	}
	else if(id <= ITEM_IMAGE_KEY_END) {
		img_prefix = "K";
		suffix = id - ITEM_IMAGE_KEY_BEGIN + 1;
	}
	else {
		img_prefix = "T";
		suffix = id - ITEM_IMAGE_TOKEN_BEGIN + 1;
	}
	//Log(l:StrParam(s:"DND_", s:img_prefix, s:"IMG", d:suffix));
	return StrParam(l:StrParam(s:"DND_", s:img_prefix, s:"IMG", d:suffix));
}

#define ITEMLEVEL_VARIANCE_LOWER 15
#define ITEMLEVEL_VARIANCE_HIGHER 7

// holds inventories of all players
global inventory_T 11: PlayerInventoryList[MAXPLAYERS][MAX_INVENTORY_BOXES];

#define MAX_TEMP_INVENTORIES 4
global inventory_T 23: TemporaryInventoryList[MAXPLAYERS][MAX_TEMP_INVENTORIES];

enum {
	PTR_FREEITEMWORLD
};
#define MAX_POINTERS (PTR_FREEITEMWORLD + 1)

global int 24: PointerIndexTable[MAX_POINTERS];

#define MAX_INVENTORIES_ON_FIELD 8192
// holds indexes to items used that are on players like charms or armors
global inventory_T 12: Items_Used[MAXPLAYERS][MAX_ITEMS_EQUIPPABLE];
global inventory_T 13: Inventories_On_Field[MAX_INVENTORIES_ON_FIELD];
global inventory_T 14: TradeViewList[MAXPLAYERS][MAX_INVENTORY_BOXES];
global inventory_T 15: PlayerStashList[MAXPLAYERS][MAX_EXTRA_INVENTORY_PAGES][MAX_INVENTORY_BOXES];

#define INVSOURCE_PLAYER PlayerInventoryList
#define INVSOURCE_ITEMSUSED Items_Used

// Creates an item on the game field
int CreateItemSpot() {
	//Naive but very fast: Will just replace old item if index goes back to it - have a bigger array to prevent the negative effect.
	//Just having a loop here creates an error so avoid looping at all costs.
	//Remember, the floor gets cleared on a new map, so most likely the older items are useless for the players anyways, except on 4k mob slaugher maps.
	if ((++PointerIndexTable[PTR_FREEITEMWORLD]) >= MAX_INVENTORIES_ON_FIELD)
		PointerIndexTable[PTR_FREEITEMWORLD] = 0;
		
	// clear properties of this item before creating it -- fixes garbage data leftovers
	RemoveItemFromWorld(PointerIndexTable[PTR_FREEITEMWORLD]);
		
	return PointerIndexTable[PTR_FREEITEMWORLD];
}

void RemoveItemFromWorld(int fieldpos) {
	Inventories_On_Field[fieldpos].width = 1;
	Inventories_On_Field[fieldpos].height = 1;
	Inventories_On_Field[fieldpos].item_type = DND_ITEM_NULL;
	Inventories_On_Field[fieldpos].item_subtype = 0;
	Inventories_On_Field[fieldpos].item_image = 0;
	Inventories_On_Field[fieldpos].item_level = 0;
	Inventories_On_Field[fieldpos].item_stack = 0;

	Inventories_On_Field[fieldpos].corrupted = 0;
	Inventories_On_Field[fieldpos].quality = 0;
	Inventories_On_Field[fieldpos].implicit.attrib_id = -1;
	Inventories_On_Field[fieldpos].implicit.attrib_val = 0;
	Inventories_On_Field[fieldpos].implicit.attrib_tier = 0;
	Inventories_On_Field[fieldpos].implicit.attrib_extra = 0;

	for(int k = 0; k < Inventories_On_Field[fieldpos].attrib_count; ++k) {
		Inventories_On_Field[fieldpos].attributes[k].attrib_id = 0;
		Inventories_On_Field[fieldpos].attributes[k].attrib_val = 0;
		Inventories_On_Field[fieldpos].attributes[k].attrib_tier = 0;
		Inventories_On_Field[fieldpos].attributes[k].fractured = 0;
		Inventories_On_Field[fieldpos].attributes[k].attrib_extra = 0;
	}
	Inventories_On_Field[fieldpos].attrib_count = 0;
}

// Deletes an item, essentially
void FreeItem(int pnum, int item_index, int source, bool dontSync) {
	int i, j;
	int temp;
	int wtemp = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, item_index, -1, source);
	int htemp = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, item_index, -1, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, item_index, -1, 0, source);

	SetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, -1, 0, source);
	
	temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	for(j = 0; j < temp; ++j) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, item_index, j, 0, source);
	}
	SetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, 0, source);
	
	if(IsSourceInventoryView(source)) {
		for(j = 0; j < htemp; ++j)
			for(i = 0; i < wtemp; ++i) {
				SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, item_index + j * MAXINVENTORYBLOCKS_VERT + i, -1, 0, source);
				SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_index + j * MAXINVENTORYBLOCKS_VERT + i, -1, DND_ITEM_NULL, source);
			}
	}
	else {
		SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_index, -1, DND_ITEM_NULL, source);
	}
		
	SetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, item_index, -1, 0, source);
	if(!dontSync)
		SyncItemData_Null(pnum, item_index, source, wtemp, htemp);
}

void FreeSpot(int pnum, int item_index, int source) {
	int j, temp;
	SetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, item_index, -1, 0, source);

	SetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, -1, 0, source);
	
	temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	for(j = 0; j < temp; ++j) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, item_index, j, 0, source);
	}
	SetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_index, -1, DND_ITEM_NULL, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, item_index, -1, 0, source);
}

// move this from field to player's inventory
int HandleInventoryPickup(int item_index) {
	int pcharm_index = GetFreeSpotForItem(item_index, PlayerNumber(), DND_SYNC_ITEMSOURCE_FIELD, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	CopyItemFromFieldToPlayer(item_index, PlayerNumber(), pcharm_index);
	GiveInventory("DnD_RefreshRequest", 1);
	return pcharm_index;
}

int GetStackValue(int type) {
	switch (type) {
		case DND_ITEM_ORB:
		return MAXSTACKS_ORB;
		case DND_ITEM_CHESTKEY:
		return MAXSTACKS_CKEY;
		case DND_ITEM_TOKEN:
		return MAXSTACKS_TOKEN;
	}
	return 0;
}

bool IsStackedItem(int type) {
	switch (type) {
		case DND_ITEM_ORB:
		case DND_ITEM_CHESTKEY:
		case DND_ITEM_TOKEN:
		return true;
	}
	return false;
}

int FindInventoryOfType(int player_index, int item_type, int item_subtype) {
	int i = 0, j = 0, bid = 0;
	
	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			bid = j * MAXINVENTORYBLOCKS_VERT + i;
			// notice we can return bid here, we don't check like below
			if(PlayerInventoryList[player_index][bid].item_type == item_type && PlayerInventoryList[player_index][bid].item_subtype == item_subtype)
				return bid;
		}
	}
	
	return -1;
}

// note to self: height is => horizontal, moving heights => x * MAXINVENTORYBLOCKS_VERT, width is vertical, just + x
int GetFreeSpotForItem(int item_index, int player_index, int item_source, int dest_source, int source_player = -1) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	int w, h;
	
	// extended check for potential player source change
	if(source_player == -1) {
		w = GetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, item_index, -1, item_source);
		h = GetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, item_index, -1, item_source);
	}
	else {
		w = GetItemSyncValue(source_player, DND_SYNC_ITEMWIDTH, item_index, -1, item_source);
		h = GetItemSyncValue(source_player, DND_SYNC_ITEMHEIGHT, item_index, -1, item_source);
	}
	
	bool unfit = false;

	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			for(hcheck = 0; !unfit && hcheck < h && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < w && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if(bid >= MAX_INVENTORY_BOXES || GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, bid, -1, dest_source) != DND_ITEM_NULL)
						unfit = true;
				}
			}
			// we return top left corner box id
			if(wcheck == w && hcheck == h && !unfit) {
				return j * MAXINVENTORYBLOCKS_VERT + i;
			}
		}
	}
	//printbold(s:"found no spot");
	return -1;
}

int GetFreeSpotForItemWithStack(int item_index, int player_index, int item_source, int dest_source, bool check_stack = true, int source_player = -1) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	int w, h;
	bool unfit = false;
	
	// extended check for potential player source change
	if(source_player == -1) {
		w = GetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, item_index, -1, item_source);
		h = GetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, item_index, -1, item_source);
	}
	else {
		w = GetItemSyncValue(source_player, DND_SYNC_ITEMWIDTH, item_index, -1, item_source);
		h = GetItemSyncValue(source_player, DND_SYNC_ITEMHEIGHT, item_index, -1, item_source);
	}
	
	// first search for any spot on our inventory for a stack item of this type
	int type = GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, item_index, -1, item_source);
	int sub = GetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, item_index, -1, item_source);
	int maxstack = GetStackValue(type);
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if
		(
			GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, i, -1, dest_source) == type &&
			GetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, i, -1, dest_source) == sub &&
			(
				(!check_stack && GetItemSyncValue(player_index, DND_SYNC_ITEMSTACK, i, -1, dest_source) != maxstack) || 
				(GetItemSyncValue(player_index, DND_SYNC_ITEMSTACK, i, -1, dest_source) + GetItemSyncValue(player_index, DND_SYNC_ITEMSTACK, item_index, -1, item_source) < maxstack)
			)
		)
			return i;
	}
	
	// didn't work, find new spot
	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			for(hcheck = 0; !unfit && hcheck < h && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < w && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if(bid >= MAX_INVENTORY_BOXES || GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, bid, -1, dest_source) != DND_ITEM_NULL)
						unfit = true;
				}
			}
			// we return top left corner box id
			if(wcheck == w && hcheck == h && !unfit)
				return j * MAXINVENTORYBLOCKS_VERT + i;
		}
	}
	return -1;
}

int GetFreeSpotForSingleSpotItem(int player_index, int type, int sub) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	bool unfit = false;
	
	// first search for any spot on our inventory for a stack item of this type
	int maxstack = GetStackValue(type);
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i)
		if(PlayerInventoryList[player_index][i].item_type == type && PlayerInventoryList[player_index][i].item_subtype == sub && PlayerInventoryList[player_index][i].item_stack < maxstack)
			return i;
			
	// didn't work, find new spot
	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			for(hcheck = 0; !unfit && hcheck < 1 && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < 1 && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if(bid >= MAX_INVENTORY_BOXES || PlayerInventoryList[player_index][bid].item_type != DND_ITEM_NULL)
						unfit = true;
				}
			}
			// we return top left corner box id
			if(wcheck == 1 && hcheck == 1 && !unfit)
				return j * MAXINVENTORYBLOCKS_VERT + i;
		}
	}
	return -1;
}

int GetFreeSpotForItem_Trade(int item_index, int source_player, int player_index, int source) {
	if(!IsStackedItem(GetItemSyncValue(source_player, DND_SYNC_ITEMTYPE, item_index, -1, source)))
		return GetFreeSpotForItem(item_index, player_index, source, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, source_player);
	return GetFreeSpotForItemWithStack(item_index, player_index, source, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, true, source_player);
}

// Check if we have enough space for all the items offered, in our inventory
bool ConfirmSpaceForOfferings(int pnum, int tradee) {
	int bid, pos;
	int i, j, h, w, hcomp, wcomp;
	// for every possible item in the trade list of this player, get free position
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			bid = j + i * MAXINVENTORYBLOCKS_VERT;
			// care about the items only once, so use topleftboxid == bid
			if(TradeViewList[tradee][bid].topleftboxid - 1 == bid) {
				pos = GetFreeSpotForItem_Trade(bid, tradee, pnum, DND_SYNC_ITEMSOURCE_TRADEVIEW);
				if(pos != -1) {
					// mark as occupied so getfreespot wont return them
					hcomp = TradeViewList[tradee][bid].height;
					wcomp = TradeViewList[tradee][bid].width;
					for(h = 0; h < hcomp; ++h)
						for(w = 0; w < wcomp; ++w) {
							// hack to avoid finding the same spot as empty
							if(PlayerInventoryList[pnum][pos + w + h * MAXINVENTORYBLOCKS_VERT].item_type == DND_ITEM_NULL)
								PlayerInventoryList[pnum][pos + w + h * MAXINVENTORYBLOCKS_VERT].item_type = DND_ITEM_TEMPORARY;
							ItemMoveList[pnum][bid + w + h * MAXINVENTORYBLOCKS_VERT].state = true;
							ItemMoveList[pnum][bid + w + h * MAXINVENTORYBLOCKS_VERT].width = wcomp;
							ItemMoveList[pnum][bid + w + h * MAXINVENTORYBLOCKS_VERT].height = hcomp;
						}
					//printbold(s:"send player ", d:pnum, s:"'s item at ", d:bid, s:" to ", d:pos);
					ItemMoveList[pnum][bid].dest_pos = pos;
				}
				else {
					// clean up whatever was used up, we had no space
					for(pos = 0; pos < MAX_INVENTORY_BOXES; ++pos) {
						if(ItemMoveList[pnum][pos].state) {
							// cleanup our hack
							if(PlayerInventoryList[pnum][pos].item_type == DND_ITEM_TEMPORARY)
								PlayerInventoryList[pnum][pos].item_type = DND_ITEM_NULL;
							ItemMoveList[pnum][pos].state = false;
							ItemMoveList[pnum][pos].width = 0;
							ItemMoveList[pnum][pos].height = 0;
							ItemMoveList[pnum][pos].dest_pos = -1;
						}
					}
					return false;
				}
			}
		}
	}
	
	// unmark all marked spots as unoccupied (real marking happens when we move items)
	// do not touch itemmovelist, we will use this as the place to move new items so we avoid checking again
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(ItemMoveList[pnum][i].state) {
			// unmark as occupied
			wcomp = ItemMoveList[pnum][i].width;
			hcomp = ItemMoveList[pnum][i].height;
			
			// note that the itemmovelist contains "Item of Player in tradebox was to move to dest_pos in player inventory" info
			pos = ItemMoveList[pnum][i].dest_pos;
			for(h = 0; h < hcomp; ++h)
				for(w = 0; w < wcomp; ++w) {
					// cleanup our hack
					if(PlayerInventoryList[pnum][pos + w + h * MAXINVENTORYBLOCKS_VERT].item_type == DND_ITEM_TEMPORARY)
						PlayerInventoryList[pnum][pos + w + h * MAXINVENTORYBLOCKS_VERT].item_type = DND_ITEM_NULL;
				}
		}
	}
	
	
	return true;
}

bool IsWearingBodyArmor(int pnum) {
	return Items_Used[pnum][BODY_ARMOR_INDEX].item_type == DND_ITEM_BODYARMOR;
}

bool IsPlayerInventoryItemUnique(int pnum, int pos) {
	return PlayerInventoryList[pnum][pos].item_type > UNIQUE_BEGIN;
}

int MakeItemUsed(int pnum, int use_id, int item_index, int item_type, int target_type) {
	int i, j;
	
	// type mismatch, popup
	if(item_type == DND_ITEM_CHARM && target_type != PlayerInventoryList[pnum][item_index].item_subtype)
		return POPUP_CHARMMISMATCH;
		
	// too high level
	if(PlayerInventoryList[pnum][item_index].item_level > GetStat(STAT_LVL))
		return POPUP_ITEMLVLTOOHIGH;
		
	// no duplicate uniques
	if(DoUniqueCheck(pnum, use_id, item_index, target_type))
		return POPUP_ONLYONEUNIQUE;
	
	// special condition checks
	bool isUnique = IsPlayerInventoryItemUnique(pnum, item_index);
	// tried to put well of power, but have too many small charms -- its always attribute id 2 on well of power
	// we have to check for that specifically because technically its not equipped yet, so player has no tokens of it on them
	if
	(
		isUnique && 
		(PlayerInventoryList[pnum][item_index].item_type >> UNIQUE_BITS) - 1 == UITEM_WELLOFPOWER &&
		CountPlayerSmallCharms(pnum) > PlayerInventoryList[pnum][item_index].attributes[2].attrib_val
	)
		return POPUP_NOMORESMALLCHARMS;
	
	// or tried to put small charm when well of power is there and would exceed limit
	if(target_type == DND_CHARM_SMALL && (i = GetPlayerAttributeValue(pnum, INV_EX_LIMITEDSMALLCHARMS)) && i == CountPlayerSmallCharms(pnum))
		return POPUP_NOMORESMALLCHARMS;

	// if has forbid armor but has equipped body armor, don't allow that item to be put, and vice versa if has no armor and has forbid armor etc.
	if
	(
		isUnique &&
		(PlayerInventoryList[pnum][item_index].item_type >> UNIQUE_BITS) - 1 == UITEM_OAKHEART &&
		IsWearingBodyArmor(pnum)
	)
		return POPUP_CANTWEARBODYARMOR;

	if(item_type == DND_ITEM_BODYARMOR && GetPlayerAttributeValue(pnum, INV_EX_FORBID_ARMOR))
		return POPUP_CANTPUTONBODYARMOR;

	// proceed to equip the item now
	// request damage cache recalculation
	ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, PlayerNumber());
	
	// this means we must swap items
	if(Items_Used[pnum][use_id].item_type != DND_ITEM_NULL) {
		ApplyItemFeatures(pnum, use_id, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_REMOVE);
		SwapItems(pnum, use_id, item_index, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
		ApplyItemFeatures(pnum, use_id, DND_SYNC_ITEMSOURCE_ITEMSUSED);
	}
	else {
		// just zero the stuff in inventory, and copy them into items used
		Items_Used[pnum][use_id].width = PlayerInventoryList[pnum][item_index].width;
		Items_Used[pnum][use_id].height = PlayerInventoryList[pnum][item_index].height;
		Items_Used[pnum][use_id].item_type = PlayerInventoryList[pnum][item_index].item_type;
		Items_Used[pnum][use_id].item_subtype = PlayerInventoryList[pnum][item_index].item_subtype;
		Items_Used[pnum][use_id].item_image = PlayerInventoryList[pnum][item_index].item_image;
		Items_Used[pnum][use_id].item_level = PlayerInventoryList[pnum][item_index].item_level;
		Items_Used[pnum][use_id].item_stack = PlayerInventoryList[pnum][item_index].item_stack;
		Items_Used[pnum][use_id].attrib_count = PlayerInventoryList[pnum][item_index].attrib_count;
		Items_Used[pnum][use_id].topleftboxid = use_id + 1;

		Items_Used[pnum][use_id].corrupted = PlayerInventoryList[pnum][item_index].corrupted;
		Items_Used[pnum][use_id].quality = PlayerInventoryList[pnum][item_index].quality;
		Items_Used[pnum][use_id].implicit.attrib_id = PlayerInventoryList[pnum][item_index].implicit.attrib_id;
		Items_Used[pnum][use_id].implicit.attrib_val = PlayerInventoryList[pnum][item_index].implicit.attrib_val;
		Items_Used[pnum][use_id].implicit.attrib_tier = PlayerInventoryList[pnum][item_index].implicit.attrib_tier;
		Items_Used[pnum][use_id].implicit.attrib_extra = PlayerInventoryList[pnum][item_index].implicit.attrib_extra;

		for(i = 0; i < Items_Used[pnum][use_id].attrib_count; ++i) {
			Items_Used[pnum][use_id].attributes[i].attrib_id = PlayerInventoryList[pnum][item_index].attributes[i].attrib_id;
			Items_Used[pnum][use_id].attributes[i].attrib_val = PlayerInventoryList[pnum][item_index].attributes[i].attrib_val;
			Items_Used[pnum][use_id].attributes[i].attrib_tier = PlayerInventoryList[pnum][item_index].attributes[i].attrib_tier;
			Items_Used[pnum][use_id].attributes[i].attrib_extra = PlayerInventoryList[pnum][item_index].attributes[i].attrib_extra;
			Items_Used[pnum][use_id].attributes[i].fractured = PlayerInventoryList[pnum][item_index].attributes[i].fractured;
		}

		// the leftover spot is a null item
		int wtemp = PlayerInventoryList[pnum][item_index].width;
		int htemp = PlayerInventoryList[pnum][item_index].height;
		FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
		//SyncItemData(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, wtemp, htemp);
		SyncItemData(pnum, use_id, DND_SYNC_ITEMSOURCE_ITEMSUSED, -1, -1);
		ApplyItemFeatures(pnum, use_id, DND_SYNC_ITEMSOURCE_ITEMSUSED);
	}
	return -1;
}

// based on average player level
int RollItemLevel() {
	int res = PlayerInformationInLevel[PLAYERLEVELINFO_COUNTATSTART];
	if(!res)
		res = 1;
	
	// return average player level, +- some value
	int pavg = PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] / res;
	if(pavg > 2 * ITEMLEVEL_VARIANCE_LOWER) {
		res = pavg + random(-ITEMLEVEL_VARIANCE_LOWER, ITEMLEVEL_VARIANCE_HIGHER);
		res = Clamp_Between(res, 1, MAX_ITEM_LEVEL);
		return res;
	}
	return pavg + random((-pavg + 1) / 2, ITEMLEVEL_VARIANCE_HIGHER);
}

int CheckItemAttribute(int pnum, int item_pos, int attrib_index, int source, int count) {
	int i;
	for(i = 0; i < count; ++i)
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_pos, i, source) == attrib_index)
			return i;
	return -1;
}

// specialized version of the above function for playerinventorylist -- keeps it cleaner arguments wise I guess
int IsAttribInItem(int pnum, int item_pos, int attrib_id) {
	int count = PlayerInventoryList[pnum][item_pos].attrib_count;
	for(int i = 0; i < count; ++i)
		if(PlayerInventoryList[pnum][item_pos].attributes[i].attrib_id == attrib_id)
			return true;
	return false;
}

// find the item that has a min, if basis isn't -1 then we must exclude this from inclusion to min
int FindMinOnUsedCharmsForAttribute(int pnum, int attrib_index, int basis) {
	int res = -1, temp, compare = INT_MAX;
	for(int i = 0; i < MAX_ITEMS_EQUIPPABLE; ++i) {
		if(i == basis)
			continue;
		if(Items_Used[pnum][i].item_type != DND_ITEM_NULL) {
			temp = CheckItemAttribute(pnum, i, attrib_index, DND_SYNC_ITEMSOURCE_ITEMSUSED, Items_Used[pnum][i].attrib_count);
			// means this exists
			if(temp != -1) {
				if(Items_Used[pnum][i].attributes[temp].attrib_val < compare) {
					compare = Items_Used[pnum][i].attributes[temp].attrib_val;
					SetInventory("DamagePerFlatHPBuffer", compare);
					res = i;
				}
			}
		}
	}
	return res;
}

// returns false if it should not destroy item, true if it should
bool CopyItemFromFieldToPlayer(int fieldpos, int player_index, int item_index, int stacked_item_type = -1) {
	int i, j, k, wtemp, htemp;
	// handle the box management
	// is this a stack item and does it already contain an item of this type?
	// ex type = orb, subtype = enhancement
	int max_stack = GetStackValue(Inventories_On_Field[fieldpos].item_type);
	if(
		Inventories_On_Field[fieldpos].item_stack && 
		Inventories_On_Field[fieldpos].item_type == PlayerInventoryList[player_index][item_index].item_type && 
		Inventories_On_Field[fieldpos].item_subtype == PlayerInventoryList[player_index][item_index].item_subtype &&
		PlayerInventoryList[player_index][item_index].item_stack + Inventories_On_Field[fieldpos].item_stack <= max_stack
	) {
		// just add to the stack
		PlayerInventoryList[player_index][item_index].item_stack += Inventories_On_Field[fieldpos].item_stack;
		SyncItemData(player_index, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[player_index][item_index].width, PlayerInventoryList[player_index][item_index].height);
	}
	else {
		// no? -- check to dump as much of the stack from this into the other item, and create new spot with leftover stack
		if(PlayerInventoryList[player_index][item_index].item_stack != max_stack && PlayerInventoryList[player_index][item_index].item_stack + Inventories_On_Field[fieldpos].item_stack > max_stack) {
			Inventories_On_Field[fieldpos].item_stack -= max_stack - PlayerInventoryList[player_index][item_index].item_stack;
			PlayerInventoryList[player_index][item_index].item_stack = max_stack;
			SyncItemData(player_index, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[player_index][item_index].width, PlayerInventoryList[player_index][item_index].height);
			SyncItemData(player_index, fieldpos, DND_SYNC_ITEMSOURCE_FIELD, Inventories_On_Field[fieldpos].width, Inventories_On_Field[fieldpos].height);
			
			// create new item and place it, if can't find place to put it leave it on the ground with leftover stacks
			int new_pos = GetFreeSpotForItemWithStack(fieldpos, player_index, DND_SYNC_ITEMSOURCE_FIELD, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			if(new_pos != -1)
				return CopyItemFromFieldToPlayer(fieldpos, player_index, new_pos);
			
			ACS_NamedExecuteAlways("DnD Inventory Full CS", 0, player_index);
			return false;
		}

		// look to place it
		wtemp = Inventories_On_Field[fieldpos].width;
		htemp = Inventories_On_Field[fieldpos].height;
		PlayerInventoryList[player_index][item_index].width = wtemp;
		PlayerInventoryList[player_index][item_index].height = htemp;
		PlayerInventoryList[player_index][item_index].item_subtype = Inventories_On_Field[fieldpos].item_subtype;
		PlayerInventoryList[player_index][item_index].item_image = Inventories_On_Field[fieldpos].item_image;
		PlayerInventoryList[player_index][item_index].item_level = Inventories_On_Field[fieldpos].item_level;
		PlayerInventoryList[player_index][item_index].item_stack = Inventories_On_Field[fieldpos].item_stack;

		PlayerInventoryList[player_index][item_index].corrupted = Inventories_On_Field[fieldpos].corrupted;
		PlayerInventoryList[player_index][item_index].quality = Inventories_On_Field[fieldpos].quality;
		PlayerInventoryList[player_index][item_index].implicit.attrib_id = Inventories_On_Field[fieldpos].implicit.attrib_id;
		PlayerInventoryList[player_index][item_index].implicit.attrib_val = Inventories_On_Field[fieldpos].implicit.attrib_val;
		PlayerInventoryList[player_index][item_index].implicit.attrib_tier = Inventories_On_Field[fieldpos].implicit.attrib_tier;
		PlayerInventoryList[player_index][item_index].implicit.attrib_extra = Inventories_On_Field[fieldpos].implicit.attrib_extra;

		PlayerInventoryList[player_index][item_index].attrib_count = Inventories_On_Field[fieldpos].attrib_count;
		for(k = 0; k < PlayerInventoryList[player_index][item_index].attrib_count; ++k) {
			PlayerInventoryList[player_index][item_index].attributes[k].attrib_id = Inventories_On_Field[fieldpos].attributes[k].attrib_id;
			PlayerInventoryList[player_index][item_index].attributes[k].attrib_val = Inventories_On_Field[fieldpos].attributes[k].attrib_val;
			PlayerInventoryList[player_index][item_index].attributes[k].attrib_tier = Inventories_On_Field[fieldpos].attributes[k].attrib_tier;
			PlayerInventoryList[player_index][item_index].attributes[k].attrib_extra = Inventories_On_Field[fieldpos].attributes[k].attrib_extra;
			PlayerInventoryList[player_index][item_index].attributes[k].fractured = Inventories_On_Field[fieldpos].attributes[k].fractured;
		}
		for(i = 0; i < htemp; ++i)
			for(j = 0; j < wtemp; ++j) {
				PlayerInventoryList[player_index][item_index + i * MAXINVENTORYBLOCKS_VERT + j].item_type = Inventories_On_Field[fieldpos].item_type;
				PlayerInventoryList[player_index][item_index + i * MAXINVENTORYBLOCKS_VERT + j].topleftboxid = item_index + 1;
			}
		SyncItemData(player_index, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, wtemp, htemp);
	}
	// the leftover spot is a null item
	FreeItem(-1, fieldpos, DND_SYNC_ITEMSOURCE_FIELD, false);
	return true;
}

// clones an item on this player's inventory, if no spot is found it won't bother
int CloneItem(int pnum, int item_index, int source, bool dontSync) {
	int temp;
	int c = GetFreeSpotForItem(item_index, pnum, source, source);
	if(c != -1) {
		int wtemp = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, item_index, -1, source);
		int htemp = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, item_index, -1, source);
		int i, j, k;
		SetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, c, -1, wtemp, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, c, -1, htemp, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, item_index, -1, source), source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, item_index, -1, source), source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, item_index, -1, source), source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, item_index, -1, source), source);

		SetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, item_index, -1, source), source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_index, -1, source), source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, -1, source), source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, -1, source), source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, item_index, -1, source), source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, -1, source), source);

		temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, c, -1, temp, source);
		for(k = 0; k < temp; ++k) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, k, source), source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, k, source), source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, k, source), source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, k, source), source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, item_index, k, source), source);
		}
		for(i = 0; i < htemp; ++i)
			for(j = 0; j < wtemp; ++j) {
				temp = c + i * MAXINVENTORYBLOCKS_VERT + j;
				SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_index, -1, source), source);
				SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, temp, -1, c + 1, source);
			}
		if(!dontSync)
			SyncItemData(pnum, c, source, -1, -1);
		return c;
	}
	return -1;
}

// check if clicked spot is free for the item we want to put
bool IsFreeSpot(int pnum, int itempos, int emptypos, int itemsource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, int emptysource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) {
	int temp = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, itemsource) - 1;
	int w = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, temp, -1, itemsource);
	int h = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, temp, -1, itemsource);
	int offset = temp - itempos;
	int bid = 0;
	int tb = 0;

	bool rowStart = !((emptypos + offset) % MAXINVENTORYBLOCKS_VERT);
	
	for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
			bid = emptypos + offset + j + i * MAXINVENTORYBLOCKS_VERT;
			// vertical range check
			if(bid >= MAX_INVENTORY_BOXES || bid < 0)
				return false;

			// horiz range check -- this one's a bit more involved
			// check if we go from a non-multiple of 9 to a multiple of 9, that's when you get on a new line
			if(!rowStart && !(bid % 9))
				return false;

			// if not empty and it's not us
			tb = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, emptysource);
			if(tb && tb - 1 != temp)
				return false;
		}
	}
	return true;
}

bool IsValidBox(int beg, int off) {
	return beg + off < MAX_INVENTORY_BOXES && beg + off >= 0;
}

bool IsSourceInventoryView(int source) {
	// just in case we receive this with page numbers in it
	source &= 0xFFFF;
	return source == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY || source == DND_SYNC_ITEMSOURCE_TRADEVIEW || source == DND_SYNC_ITEMSOURCE_STASH;
}

// assumes both spots have an item
// if ipos1 has enough space to acommodate ipos2
bool CanSwapItems(int pnum, int ipos1, int ipos2, int offset1, int offset2, int source1, int source2) {
	int i, j;
	int w1, w2, h1, h2;
	int bid = 0, tb1, tb2;
	// from ipos2 to ipos1
	tb1 = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, source1) - 1;
	tb2 = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, source2) - 1;
	w2 = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, tb2, -1, source2);
	h2 = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, tb2, -1, source2);
	for(i = 0; i < h2; ++i) {
		for(j = 0; j < w2; ++j) {
			bid = ipos1 + offset2 + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0)
				return false;
			if(GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source1) - 1 != tb1 && GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, bid, -1, source1) != DND_ITEM_NULL)
				return false;
		}
	}
	// from ipos1 to ipos2
	w1 = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, tb1, -1, source1);
	h1 = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, tb1, -1, source1);
	for(i = 0; i < h1; ++i) {
		for(j = 0; j < w1; ++j) {
			bid = ipos2 + offset1 + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0)
				return false;
			if(GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source2) - 1 != tb2 && GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, bid, -1, source2) != DND_ITEM_NULL)
				return false;
		}
	}
	return true;
}

// accepts topboxid as item_index
void MoveItemToTemporary(int player_index, int item_index, int temp_pos, int source) {
	int i;
	TemporaryInventoryList[player_index][temp_pos].width = GetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].height = GetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].item_type = GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].item_subtype = GetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].item_image = GetItemSyncValue(player_index, DND_SYNC_ITEMIMAGE, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].item_level = GetItemSyncValue(player_index, DND_SYNC_ITEMLEVEL, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].item_stack = GetItemSyncValue(player_index, DND_SYNC_ITEMSTACK, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].attrib_count = GetItemSyncValue(player_index, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);

	TemporaryInventoryList[player_index][temp_pos].corrupted = GetItemSyncValue(player_index, DND_SYNC_ITEMCORRUPTED, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].quality = GetItemSyncValue(player_index, DND_SYNC_ITEMQUALITY, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].implicit.attrib_id = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, i, source);
	TemporaryInventoryList[player_index][temp_pos].implicit.attrib_val = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, i, source);
	TemporaryInventoryList[player_index][temp_pos].implicit.attrib_tier = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, item_index, i, source);
	TemporaryInventoryList[player_index][temp_pos].implicit.attrib_extra = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, i, source);

	TemporaryInventoryList[player_index][temp_pos].topleftboxid = 0;
	for(i = 0; i < TemporaryInventoryList[player_index][temp_pos].attrib_count; ++i) {
		TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_id = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_ID, item_index, i, source);
		TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_val = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, i, source);
		TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_tier = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, i, source);
		TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_extra = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, i, source);
		TemporaryInventoryList[player_index][temp_pos].attributes[i].fractured = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_FRACTURE, item_index, i, source);
	}
}

// copies item from temporary list to player inventory
void CopyItemFromTemporary(int player_index, int item_index, int temp_pos, int source) {
	int i;
	int w = TemporaryInventoryList[player_index][temp_pos].width, h = TemporaryInventoryList[player_index][temp_pos].height;
	SetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, item_index, -1, w, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, item_index, -1, h, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, item_index, -1, TemporaryInventoryList[player_index][temp_pos].item_subtype, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, TemporaryInventoryList[player_index][temp_pos].attrib_count, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMSTACK, item_index, -1, TemporaryInventoryList[player_index][temp_pos].item_stack, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMLEVEL, item_index, -1, TemporaryInventoryList[player_index][temp_pos].item_level, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMIMAGE, item_index, -1, TemporaryInventoryList[player_index][temp_pos].item_image, source);

	SetItemSyncValue(player_index, DND_SYNC_ITEMCORRUPTED, item_index, -1, TemporaryInventoryList[player_index][temp_pos].corrupted, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMQUALITY, item_index, -1, TemporaryInventoryList[player_index][temp_pos].quality, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, i, TemporaryInventoryList[player_index][temp_pos].implicit.attrib_id, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, i, TemporaryInventoryList[player_index][temp_pos].implicit.attrib_val, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, item_index, i, TemporaryInventoryList[player_index][temp_pos].implicit.attrib_tier, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, i, TemporaryInventoryList[player_index][temp_pos].implicit.attrib_extra, source);
	
	if(IsSourceInventoryView(source)) {
		for(i = 0; i < h; ++i) {
			for(int j = 0; j < w; ++j) {
				int bid = item_index + j + i * MAXINVENTORYBLOCKS_VERT;
				SetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, item_index + 1, source);
				SetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, bid, -1, TemporaryInventoryList[player_index][temp_pos].item_type, source);
			}
		}
	}
	else {
		SetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, item_index + 1, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, item_index, -1, TemporaryInventoryList[player_index][temp_pos].item_type, source);
	}
	
	for(i = 0; i < TemporaryInventoryList[player_index][temp_pos].attrib_count; ++i) {
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_ID, item_index, i, TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_id, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, i, TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_val, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, i, TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_tier, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, i, TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_extra, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_FRACTURE, item_index, i, TemporaryInventoryList[player_index][temp_pos].attributes[i].fractured, source);
	}
}

// for swapping items -- assumes neither are null
void SwapItems(int pnum, int ipos1, int ipos2, int source1, int source2, bool dontSync) {
	// check if there's enough space
	// find if there is some sort of offset we must take care of, this matters only if we are swapping in inventory
	int offset1 = 0, offset2 = 0;
	int i, j, h1p, h2p, w1p, w2p;
	if(IsSourceInventoryView(source1))
		offset1 = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, source1) - 1 - ipos1;
	if(IsSourceInventoryView(source2))
		offset2 = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, source2) - 1 - ipos2;
		
	//printbold(s:"item positions: ", d:ipos1, s: ", ", d:ipos2, s:" -- offsets: ", d:offset1, s: ", ", d:offset2);
		
	// if both items are stack items and are of same types, add stack on top of the other
	h1p = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos1 + offset1, -1, source1);
	h2p = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos2 + offset2, -1, source2);
	w1p = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, ipos1 + offset1, -1, source1);
	if(
		h1p && h2p &&
		w1p == GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, ipos2 + offset2, -1, source2) &&
		GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, ipos1 + offset1, -1, source1) == GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, ipos2 + offset2, -1, source2)
	)
	{
		w2p = GetStackValue(w1p);
		// add stack of ipos2 to ipos1
		if(h1p + h2p <= w2p) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos1 + offset1, -1, h1p + h2p, source1);
			SyncItemStack(pnum, ipos1 + offset1, source1);
			// dispose of 2nd item
			FreeItem(pnum, ipos2 + offset2, source2, false);
		}
		else {
			// set stack of h1p to max, then set the stack of ipos2 to whatever is left
			SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos2 + offset2, -1, h2p - w2p + h1p, source2);
			SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos1 + offset1, -1, w2p, source1);
			SyncItemStack(pnum, ipos1 + offset1, source1);
			SyncItemStack(pnum, ipos2 + offset2, source2);
		}
	}
	else if(CanSwapItems(pnum, ipos1, ipos2, offset1, offset2, source1, source2)) {
		//printbold(s:"can swap");
		MoveItemToTemporary(pnum, ipos1 + offset1, 0, source1);
		MoveItemToTemporary(pnum, ipos2 + offset2, 1, source2);
		
		FreeItem(pnum, ipos1 + offset1, source1, false);
		FreeItem(pnum, ipos2 + offset2, source2, false);
		
		CopyItemFromTemporary(pnum, ipos1 + offset2, 1, source1);
		CopyItemFromTemporary(pnum, ipos2 + offset1, 0, source2);
		
		// special version calls scripts with 1 tic delay, because apparently script execution order in zandronum is so SHIT, the ones in FreeItem aren't even
		// guaranteed to run before the scripts called from below
		SyncItemData_Special(pnum, ipos1 + offset2, source1);
		SyncItemData_Special(pnum, ipos2 + offset1, source2);
		
		// update orb item pointer
		if(Player_MostRecent_Orb[pnum].p_tempwep == ipos1)
			Player_MostRecent_Orb[pnum].p_tempwep = ipos2;
		else if(Player_MostRecent_Orb[pnum].p_tempwep == ipos2)
			Player_MostRecent_Orb[pnum].p_tempwep = ipos1;
	}
}

/*
	0  1  2  3  4
	9 10 11 12 13
   18 19 20 21 22
   27 28 29 30 31
*/

bool InventoryBoxContainsPoint(int point, int boxleft, int w, int h) {
	// for every line, check if point falls into a linear range
	for(int i = 0; i < h; ++i)
		if(boxleft + i * MAXINVENTORYBLOCKS_VERT <= point && boxleft + i * MAXINVENTORYBLOCKS_VERT + w - 1 >= point)
			return true;
	return false;
}

void MoveItem(int pnum, int itempos, int emptypos) {
	int tb = PlayerInventoryList[pnum][itempos].topleftboxid - 1;
	int offset = tb - itempos;
	
	int i, j, bid;
	
	int w = PlayerInventoryList[pnum][tb].width;
	int h = PlayerInventoryList[pnum][tb].height;

	// these two mark box ids that have been modified, ie. need updates
	// if the boxes aren't in range of itempos, they will need to be nulled because we no longer need to preserve the data in that spot
	int set1 = 0;
	int set2 = 0;
	
	int temp = emptypos + offset;
	
	for(i = 0; i < h; ++i) 
		for(j = 0; j < w; ++j) {
			bid = tb + j + i * MAXINVENTORYBLOCKS_VERT;
			if(!InventoryBoxContainsPoint(bid, temp, w, h)) {
				if(bid < 32)
					set1 |= 1 << bid;
				else
					set2 |= 1 << (bid - 32);
			}
		}
	
	PlayerInventoryList[pnum][temp].width = w;
	PlayerInventoryList[pnum][temp].height = h;
	PlayerInventoryList[pnum][temp].item_subtype = PlayerInventoryList[pnum][tb].item_subtype;
	PlayerInventoryList[pnum][temp].item_image = PlayerInventoryList[pnum][tb].item_image;
	PlayerInventoryList[pnum][temp].item_level = PlayerInventoryList[pnum][tb].item_level;
	PlayerInventoryList[pnum][temp].item_stack = PlayerInventoryList[pnum][tb].item_stack;
	PlayerInventoryList[pnum][temp].attrib_count = PlayerInventoryList[pnum][tb].attrib_count;

	PlayerInventoryList[pnum][temp].corrupted = PlayerInventoryList[pnum][tb].corrupted;
	PlayerInventoryList[pnum][temp].quality = PlayerInventoryList[pnum][tb].quality;
	PlayerInventoryList[pnum][temp].implicit.attrib_id = PlayerInventoryList[pnum][tb].implicit.attrib_id;
	PlayerInventoryList[pnum][temp].implicit.attrib_val = PlayerInventoryList[pnum][tb].implicit.attrib_val;
	PlayerInventoryList[pnum][temp].implicit.attrib_tier = PlayerInventoryList[pnum][tb].implicit.attrib_tier;
	PlayerInventoryList[pnum][temp].implicit.attrib_extra = PlayerInventoryList[pnum][tb].implicit.attrib_extra;

	for(i = 0; i < PlayerInventoryList[pnum][temp].attrib_count; ++i) {
		PlayerInventoryList[pnum][temp].attributes[i].attrib_id = PlayerInventoryList[pnum][tb].attributes[i].attrib_id;
		PlayerInventoryList[pnum][temp].attributes[i].attrib_val = PlayerInventoryList[pnum][tb].attributes[i].attrib_val;
		PlayerInventoryList[pnum][temp].attributes[i].attrib_tier = PlayerInventoryList[pnum][tb].attributes[i].attrib_tier;
		PlayerInventoryList[pnum][temp].attributes[i].attrib_extra = PlayerInventoryList[pnum][tb].attributes[i].attrib_extra;
		PlayerInventoryList[pnum][temp].attributes[i].fractured = PlayerInventoryList[pnum][tb].attributes[i].fractured;
	}
	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			PlayerInventoryList[pnum][temp + i * MAXINVENTORYBLOCKS_VERT + j].item_type = PlayerInventoryList[pnum][tb].item_type;
			PlayerInventoryList[pnum][temp + i * MAXINVENTORYBLOCKS_VERT + j].topleftboxid = temp + 1;
		}

	if(set1 || set2) {
		for(i = 0; i < 32; ++i)
			if(IsSet(set1, i)) {
				//printbold(s:"null req on ", d:i);
				FreeSpot(pnum, i, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			}
				
		for(i = 0; i < MAX_INVENTORY_BOXES - 32; ++i)
			if(IsSet(set2, i)) {
				//printbold(s:"null req on ", d:i+32);
				FreeSpot(pnum, i + 32, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			}
		SyncItemData_Null(pnum, tb, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, w, h);
	}
	/*else {
		// Simply null the leftover spot, no collision happened
		FreeItem(pnum, tb, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	}*/
	
	// update orb item pointer
	if(Player_MostRecent_Orb[pnum].p_tempwep == itempos)
		Player_MostRecent_Orb[pnum].p_tempwep = emptypos;
	
	SyncItemData_Special(pnum, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

// this is made specifically for trade view, the one above is optimized for normal inventory
// also used for moving items from one source to another, ie. inventory to stash etc.
void MoveItemTrade(int pnum, int itempos, int emptypos, int itemsource, int emptysource) {
	int tb = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, itemsource) - 1;
	int offset = tb - itempos;
	
	int i, j, bid;
	
	int w = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, tb, -1, itemsource);
	int h = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, tb, -1, itemsource);

	// these two mark box ids that have been modified, ie. need updates
	// if the boxes aren't in range of itempos, they will need to be nulled
	int set1 = 0;
	int set2 = 0;
	
	int temp = emptypos + offset;
	
	// only check for this if they have the same source (overlap can only happen if so)
	// this also fixes a corner case bug
	if(itemsource == emptysource) {
		for(i = 0; i < h; ++i) 
			for(j = 0; j < w; ++j) {
				bid = tb + j + i * MAXINVENTORYBLOCKS_VERT;
				if(!InventoryBoxContainsPoint(bid, temp, w, h)) {
					if(tb + j + i * MAXINVENTORYBLOCKS_VERT < 32)
						set1 |= 1 << (bid);
					else
						set2 |= 1 << (bid - 32);
				}
			}
	}

	SetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, temp, -1, w, emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, temp, -1, h, emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, tb, -1, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, tb, -1, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, tb, -1, itemsource), emptysource);

	SetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, tb, -1, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, tb, -1, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, tb, i, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, tb, i, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, tb, i, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, tb, i, itemsource), emptysource);

	bid = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, emptysource);
	for(i = 0; i < bid; ++i) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, tb, i, itemsource), emptysource);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, tb, i, itemsource), emptysource);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, tb, i, itemsource), emptysource);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, tb, i, itemsource), emptysource);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, tb, i, itemsource), emptysource);
	}
	bid = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, tb, -1, itemsource);
	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, bid, emptysource);
			SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, temp + 1, emptysource);
		}

	if(set1 || set2) {
		for(i = 0; i < 32; ++i)
			if(IsSet(set1, i))
				FreeSpot(pnum, i, itemsource);
			
		for(i = 0; i < MAX_INVENTORY_BOXES - 32; ++i)
			if(IsSet(set2, i))
				FreeSpot(pnum, i + 32, itemsource);
				
		SyncItemData_Null(pnum, tb, itemsource, w, h);
	}
	else {
		// Simply null the leftover spot, no collision happened
		FreeItem(pnum, tb, itemsource, false);
	}
	
	// as soon as this item is offered for a trade it can't be edited
	if(Player_MostRecent_Orb[pnum].p_tempwep == itempos && itemsource == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY)
		Player_MostRecent_Orb[pnum].p_tempwep = 0;
	
	SyncItemData_Special(pnum, temp, emptysource);
}

// this simply carries an item from another player's place to another, like moveitem but has player inputs
// from p_item to p_empty
void CarryItemTo(int itempos, int emptypos, int itemsource, int emptysource, int p_item, int p_empty) {
	int tb = GetItemSyncValue(p_item, DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, itemsource) - 1;
	int offset = tb - itempos;
	
	int i, j, bid;
	
	int w = GetItemSyncValue(p_item, DND_SYNC_ITEMWIDTH, tb, -1, itemsource);
	int h = GetItemSyncValue(p_item, DND_SYNC_ITEMHEIGHT, tb, -1, itemsource);
	
	int temp = emptypos + offset;
	
	//printbold(s:"carry item of player ", d:p_item, s: " to pos ", d:emptypos);
	
	SetItemSyncValue(p_empty, DND_SYNC_ITEMWIDTH, temp, -1, w, emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMHEIGHT, temp, -1, h, emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMSUBTYPE, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMSUBTYPE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMIMAGE, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMIMAGE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMLEVEL, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMLEVEL, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMSTACK, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMSTACK, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMSATTRIBCOUNT, tb, -1, itemsource), emptysource);

	SetItemSyncValue(p_empty, DND_SYNC_ITEMCORRUPTED, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMCORRUPTED, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMQUALITY, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMQUALITY, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, tb, i, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, tb, i, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, tb, i, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, tb, i, itemsource), emptysource);

	bid = GetItemSyncValue(p_empty, DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, emptysource);
	for(i = 0; i < bid; ++i) {
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_ID, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_ID, tb, i, itemsource), emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_VAL, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_VAL, tb, i, itemsource), emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_TIER, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_TIER, tb, i, itemsource), emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_EXTRA, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_EXTRA, tb, i, itemsource), emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_FRACTURE, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_FRACTURE, tb, i, itemsource), emptysource);
	}
	bid = GetItemSyncValue(p_item, DND_SYNC_ITEMTYPE, tb, -1, itemsource);
	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			SetItemSyncValue(p_empty, DND_SYNC_ITEMTYPE, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, bid, emptysource);
			SetItemSyncValue(p_empty, DND_SYNC_ITEMTOPLEFTBOX, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, temp + 1, emptysource);
		}	

	FreeItem(p_item, tb, itemsource, false);
	SyncItemData(p_empty, temp, emptysource, -1, -1);
}

void TransferTradeItems(int from, int to) {
	int bid;
	int i, j, h, w;
	// for every possible item in the trade list of this player, get free position
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			bid = j + i * MAXINVENTORYBLOCKS_VERT;
			// care about the items only once, so use topleftboxid == bid
			if(TradeViewList[from][bid].topleftboxid - 1 == bid) {
				//printbold(s:"carry item to ", d:ItemMoveList[to][bid].dest_pos, s: " from player ", d:from, s:"'s movelist to player ", d:to);
				CarryItemTo(bid, ItemMoveList[to][bid].dest_pos, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, from, to);
			}
		}
	}
	
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		ItemMoveList[to][i].state = false;
		ItemMoveList[to][i].width = 0;
		ItemMoveList[to][i].height = 0;
		ItemMoveList[to][i].dest_pos = -1;
	}
}

int GetInventoryInfoOffset(int itype) {
	switch(itype) {
		case DND_ITEM_CHESTKEY:
		return CHESTKEY_BEGIN;
		case DND_ITEM_ORB:
		return ORBS_BEGIN;
		case DND_ITEM_TOKEN:
		return TOKEN_BEGIN;
	}
	return 0;
}

// outside source implies this was called from the menu, not in the game
void DrawInventoryInfo_Field(int pnum, int topboxid, int source, int yoff, bool isOutsideSource) {
	DeleteTextRange(RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 14, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES);
	
	// dont draw in custom intermission
	if(CheckInventory("DnD_IntermissionState"))
		return;
	
	int stack, itype = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, topboxid, -1, source), left = 0;
	if(topboxid != -1 && itype != DND_ITEM_NULL) {
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
		
		// adjust pos
		left = GetHudLeft(HUDMAX_X) << 16;
		int bx = GetIntegerBits(left) + 0.1;
		int by = isOutsideSource ? 0.1 : GetIntegerBits(HUDMAX_YF / 2) - yoff;
		int img_off = 0;
		
		// if attribute count + potential implicit existing is less than 3, it will fit on this rectangle
		int attr_count = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, topboxid, -1, source);
		bool lessThanFour = attr_count + (GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, topboxid, -1, source) != -1) < 4;
		if(lessThanFour) {
			SetFont("LDTITINS");
			HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
			by += 24.0;
			img_off = 96.2;
		}
		else {
			SetFont("LDTITINF");
			HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
			img_off = 140.2;
		}

		// center text by making it 0.4 and 0.1 on y
		bx += GetIntegerBits(HUD_ITEMBAK_XF / 2) + 0.3;
		
		if(!isOutsideSource)
			by = by - 70.9;
		else
			by += 16.0;
		
		int isubt = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, topboxid, -1, source);
		
		// show item details -- image and stack
		SetFont(GetItemImage(GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, topboxid, -1, source)));
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 1, CR_WHITE, bx, GetIntegerBits(by) + img_off, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		stack = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, topboxid, -1, source);
		if(stack) {
			SetFont("SMALLFONT");
			HudMessage(d:stack; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 14, CR_GREEN, bx + GetIntegerBits(HUD_ITEMBAK_XF / 2) - 7.2, by + 3.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
		
		// resize to keep ratios
		SetHudSize(HUDTEXTMAX_X, HUDTEXTMAX_Y, 1);
		left = GetHudLeft(HUDTEXTMAX_X);
		bx = ((left + (HUD_ITEMBAK_X * HUDTEXTMAX_X / (2 * HUDMAX_X))) << 16) + 0.4;
		by = isOutsideSource ? 20.1 : GetIntegerBits(HUDTEXTMAX_YF / 2 - HUD_ITEMBAK_YF / 2 - yoff - 30.0) + 0.1;

		if(lessThanFour && !isOutsideSource)
			by += 36.0;

		SetHudClipRect(left + 15, (by >> 16), 4 * HUD_ITEMBAK_X / 3 + 9, 288, 4 * HUD_ITEMBAK_X / 3 + 9);
		DrawInventoryText(topboxid, source, pnum, bx, by, itype, isubt, RPGMENUINVENTORYID, HUD_DII_FIELD_MULT, attr_count, !isOutsideSource);
		SetHudClipRect(0, 0, 0, 0, 0);
	}
}

void DrawInventoryText(int topboxid, int source, int pnum, int bx, int by, int itype, int isubt, int id_begin, int id_mult, int attr_count = 0, bool isOutsideSource = false) {
	int i, j;
	int val, temp, lvl;
	int yoff = 0.0;
	bool showModTiers = GetCVar("dnd_detailedmods");
	bool isUnique = false;

	str tmp_text;
	
	// potential delete of quality in case we hover over an item that doesn't have it, we don't want it lingering!
	DeleteText(id_begin - id_mult * MAX_INVENTORY_BOXES - 18);

	SetFont("SMALLFONT");
	if(IsStackedItem(itype)) {
		temp = isubt + GetInventoryInfoOffset(itype);
		HudMessage(
				s:"\c[Y5]", l:GetInventoryTag(temp), s:"\n\n", l:GetInventoryText(temp);
				HUDMSG_PLAIN | HUDMSG_FADEOUT,
				id_begin - id_mult * MAX_INVENTORY_BOXES - 3,
				CR_WHITE, 
				bx, 
				by + 32.0 * (source == DND_SYNC_ITEMSOURCE_FIELD) - 32.0 * isOutsideSource, 
				INVENTORY_HOLDTIME, 
				INVENTORY_FADETIME, 
				INVENTORY_INFO_ALPHA
		);
		return;
	}
	else {
		// this holds charm's tier id
		lvl = GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, topboxid, -1, source);
		
		if(itype == DND_ITEM_CHARM) {
			temp = GetItemTier(lvl);
			HudMessage(s:Charm_Strings[temp][CHARMSTR_COLORCODE], l:Charm_Strings[temp][CHARMSTR_TIERTAG], s: " ", l:GetCharmTypeName(isubt), s:" ", l:"DND_ITEM_CHARM"; 
				HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 2, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
			);
		}
		else if(itype == DND_ITEM_BODYARMOR) {
			HudMessage(s:"\c[Y5]", s:GetArmorInventoryTag(isubt); 
				HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 2, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
			);
		}
		else if(itype > UNIQUE_BEGIN) {
			temp = itype & 0xFFFF;
			itype >>= UNIQUE_BITS;
			--itype;
			isUnique = true;

			HudMessage(s:"\c[A1]", l:GetUniqueItemName(itype); HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 2, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
			HudMessage(s:"\c[D1]", l:"DND_ITEM_UNIQUE", s:" ", l:GetCharmTypeName(isubt), s:" ", l:"DND_ITEM_CHARM"; HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 3, CR_WHITE, bx, by + 8.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
		
		val = GetStat(STAT_LVL) < lvl ? CR_RED : CR_WHITE;
		HudMessage(l:"DND_LEVEL_HEADER", s:": ", d:lvl; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 4, val, GetIntegerBits(bx - HUD_ITEMBAK_XF / 2 - 36.0) + 0.1, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);

		// quality -- non-stacking item so
		HudMessage(s:"\c[Q2]Q: ", d:GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, topboxid, -1, source), s:"%";
			HUDMSG_PLAIN | HUDMSG_FADEOUT, 
			id_begin - id_mult * MAX_INVENTORY_BOXES - 18, 
			CR_GREEN, 
			GetIntegerBits(bx + ((2 * HUD_ITEMBAK_X / 3) << 16)) + 8.2, 
			by, 
			INVENTORY_HOLDTIME, 
			INVENTORY_FADETIME, 
			INVENTORY_INFO_ALPHA
		);

		// implicit
		val = GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, topboxid, -1, source);
		temp = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, topboxid, -1, source);
		if(temp != -1) {
			if(isUnique)
				yoff = 6.0;
			
			tmp_text = GetItemAttributeText(
				temp, 
				itype, 
				isubt, 
				GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, topboxid, j, source), 
				0, 
				GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, topboxid, j, source), 
				showModTiers, 
				GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, topboxid, j, source), 
				false,
				val
			);

			HudMessage(
				s:tmp_text;
				HUDMSG_PLAIN | HUDMSG_FADEOUT, 
				id_begin - id_mult * MAX_INVENTORY_BOXES - 5, CR_WHITE, bx, by + 12.0 + yoff, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
			);

			// add 1 for each newline... they count :p
			temp = GetRawLength(tmp_text);
			i = temp >> 16;
			temp &= 0xFFFF;
			temp = temp / NEXT_LINE_LEN + i;
			yoff = 12.0 + 8.0 * temp;
		}

		by += 12.0 + 8.0 * isUnique;

		SetFont("IMPSEPR");
		HudMessage(s:"A"; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 6, val, GetIntegerBits(bx) + 0.4, by + yoff, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);

		SetFont("SMALLFONT");
		for(j = 0; j < attr_count; ++j) {
			temp = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, topboxid, j, source);
			lvl = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, topboxid, j, source);
			if(isUnique)
				lvl = itype;

			tmp_text = GetItemAttributeText(
				temp, 
				itype, 
				isubt, 
				GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, topboxid, j, source), 
				GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, topboxid, j, source), 
				lvl, 
				showModTiers, 
				!isUnique ? -1 : j, 
				GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, topboxid, j, source),
				val
			);

			HudMessage(
				s:tmp_text; 
				HUDMSG_PLAIN | HUDMSG_FADEOUT, 
				id_begin - id_mult * MAX_INVENTORY_BOXES - 7 - j, CR_WHITE, bx, by + 8.0 + 12.0 * j + yoff, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
			);

			yoff += 10.0 * ((GetRawLength(tmp_text) & 0xFFFF) / NEXT_LINE_LEN);
		}
	}

	// corrupted label and seperator
	if(GetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, topboxid, -1, source)) {
		SetFont("IMPSEPRC");
		HudMessage(s:"A"; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 8 - attr_count, val, GetIntegerBits(bx) + 0.4, by + 8.0 + 12.0 * attr_count + yoff, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);

		SetFont("SMALLFONT");
		HudMessage(
			s:"\cgCORRUPTED"; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, 
			id_begin - id_mult * MAX_INVENTORY_BOXES - 9 - attr_count, CR_WHITE, bx, by + 16.0 + 12.0 * attr_count + yoff, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);
	}
}

void CopyItemToField(int fieldpos, int player_index, int item_index, int source) {
	int i, wtemp;
	wtemp = GetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, source) - 1;
	Inventories_On_Field[fieldpos].width = GetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, wtemp, -1, source);
	Inventories_On_Field[fieldpos].height = GetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_type = GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_subtype = GetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_image = GetItemSyncValue(player_index, DND_SYNC_ITEMIMAGE, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_level = GetItemSyncValue(player_index, DND_SYNC_ITEMLEVEL, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_stack = GetItemSyncValue(player_index, DND_SYNC_ITEMSTACK, wtemp, -1, source);
	Inventories_On_Field[fieldpos].attrib_count = GetItemSyncValue(player_index, DND_SYNC_ITEMSATTRIBCOUNT, wtemp, -1, source);

	Inventories_On_Field[fieldpos].corrupted = GetItemSyncValue(player_index, DND_SYNC_ITEMCORRUPTED, wtemp, -1, source);
	Inventories_On_Field[fieldpos].quality = GetItemSyncValue(player_index, DND_SYNC_ITEMQUALITY, wtemp, -1, source);
	Inventories_On_Field[fieldpos].implicit.attrib_id = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, wtemp, i, source);
	Inventories_On_Field[fieldpos].implicit.attrib_val = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, wtemp, i, source);
	Inventories_On_Field[fieldpos].implicit.attrib_tier = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, wtemp, i, source);
	Inventories_On_Field[fieldpos].implicit.attrib_extra = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, wtemp, i, source);

	Inventories_On_Field[fieldpos].topleftboxid = 0;
	for(i = 0; i < Inventories_On_Field[fieldpos].attrib_count; ++i) {
		Inventories_On_Field[fieldpos].attributes[i].attrib_id = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_ID, wtemp, i, source);
		Inventories_On_Field[fieldpos].attributes[i].attrib_val = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_VAL, wtemp, i, source);
		Inventories_On_Field[fieldpos].attributes[i].attrib_tier = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_TIER, wtemp, i, source);
		Inventories_On_Field[fieldpos].attributes[i].attrib_extra = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_EXTRA, wtemp, i, source);
		Inventories_On_Field[fieldpos].attributes[i].fractured = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_FRACTURE, wtemp, i, source);
	}

	// the leftover spot is a null item
	FreeItem(player_index, wtemp, source, false);
	SyncItemData_Field(fieldpos);
}

void DropItemToField(int player_index, int pitem_index, bool forAll, int source) {
	int c = CreateItemSpot();
	
	int itype = GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, pitem_index, -1, source);
	int stype = GetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, pitem_index, -1, source);
	// copy now
	CopyItemToField(c, player_index, pitem_index, source);
	str droptype = "CharmDrop";
	if(itype > UNIQUE_BEGIN)
		droptype = "UniqueCharmDrop";
	else if(itype == DND_ITEM_ORB)
		droptype = GetInventoryName(stype + ORBS_BEGIN);
	else if(itype == DND_ITEM_CHESTKEY)
		droptype = GetInventoryName(stype + CHESTKEY_BEGIN);
	else if(itype == DND_ITEM_TOKEN)
		droptype = GetInventoryName(stype + TOKEN_BEGIN);
	else if(itype == DND_ITEM_BODYARMOR)
		droptype = GetArmorDropClass(stype);
	forAll ? SpawnDropFacing(droptype, 16.0, 16, 256, c) : SpawnDropFacing(droptype, 16.0, 16, player_index + 1, c);
}

void StackedItemPickupCS(int item_index, int type) {
	if(type == DND_STACKEDITEM_ORB) {
		ACS_NamedExecuteAlways("DnD Orb Message", 0, Inventories_On_Field[item_index].item_subtype);
		GiveInventory("OrbSoundPlayer", 1);
	}
	else if(type == DND_STACKEDITEM_CHESTKEY) {
		ACS_NamedExecuteAlways("DnD Chestkey Message", 0, Inventories_On_Field[item_index].item_subtype);
		GiveInventory("ChestkeySoundPlayer", 1);
	}
	else if(type == DND_STACKEDITEM_TOKEN) {
		ACS_NamedExecuteAlways("DnD Token Message", 0, Inventories_On_Field[item_index].item_subtype);
		GiveInventory("TokenSoundPlayer", 1);
	}
}

// move this from field to player's inventory
int HandleStackedPickup(int item_index, int type) {
	// make sure this item actually gets placed on top of an item that has some stack, if any
	// print message first, if its moved to inv it's gone from there
	StackedItemPickupCS(item_index, type);
	
	int porb_index = GetFreeSpotForItemWithStack(item_index, PlayerNumber(), DND_SYNC_ITEMSOURCE_FIELD, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	GiveInventory("DnD_RefreshRequest", 1);
	return CopyItemFromFieldToPlayer(item_index, PlayerNumber(), porb_index, type);
}

// checks players inventory for the given item precisely with its subtype matching
int CheckPlayerInventoryList(int pnum, int itemtype, int subtype) {
	int i;
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i)
		if(PlayerInventoryList[pnum][i].item_type == itemtype && PlayerInventoryList[pnum][i].item_subtype == subtype)
			return i;
	return -1;
}

bool IsTwoSelectionItem(int pnum, int item_index) {
	return PlayerInventoryList[pnum][item_index].item_type == DND_ITEM_ORB && PlayerInventoryList[pnum][item_index].item_subtype == DND_ORB_ASSIMILATION;
}

// can only use items in inventory
// returns true if item expired after use
bool UsePlayerItem(int pnum, int item_index, bool countTokens) {
	if(IsUsableItem(PlayerInventoryList[pnum][item_index].item_type)) {
		GiveInventory("DnD_RefreshPane", 1);
		--PlayerInventoryList[pnum][item_index].item_stack;
		if(PlayerInventoryList[pnum][item_index].item_stack) {
			SyncItemStack(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			return false;
		}
		FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
		GiveInventory("DnD_CleanCraftingRequest", 1);
		// if this is freed, we might have to auto-adjust the player's page -- need exact equality here
		int mcount = CountCraftingMaterials(countTokens);
		if(mcount == MAX_CRAFTING_MATERIALBOXES * CheckInventory("DnD_Crafting_MaterialPage"))
			TakeInventory("DnD_Crafting_MaterialPage", 1);
		return true;
	}
	return false;
}

// Consumes a stack off a stackable item
void ConsumePlayerItem(int pnum, int item_index) {
	if(IsStackedItem(PlayerInventoryList[pnum][item_index].item_type)) {
		GiveInventory("DnD_RefreshPane", 1);
		--PlayerInventoryList[pnum][item_index].item_stack;
		if(PlayerInventoryList[pnum][item_index].item_stack)
			SyncItemStack(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		else
			FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	}
}

void UsePlayerItem_Count(int pnum, int item_index, int count) {
	PlayerInventoryList[pnum][item_index].item_stack = Clamp_Between(PlayerInventoryList[pnum][item_index].item_stack - count, 0, PlayerInventoryList[pnum][item_index].item_stack);
	if(PlayerInventoryList[pnum][item_index].item_stack)
		SyncItemStack(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	else {
		FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	}
}

// uses items from stash (needed for certain functions)
void UsePlayerStashItem_Count(int pnum, int page, int item_index, int count) {
	PlayerStashList[pnum][page][item_index].item_stack = Clamp_Between(PlayerStashList[pnum][page][item_index].item_stack - count, 0, PlayerStashList[pnum][page][item_index].item_stack);
	if(PlayerStashList[pnum][page][item_index].item_stack)
		SyncItemStack(pnum, item_index, DND_SYNC_ITEMSOURCE_STASH | (page << 16));
	else
		FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_STASH | (page << 16), false);
}

// a safeguard pretty much for use player item
bool isUsableItem(int itype) {
	switch(itype) {
		case DND_ITEM_ORB:
		case DND_ITEM_CHESTKEY:
		case DND_ITEM_TOKEN:
		return true;
	}
	return false;
}

// we only have orbs as crafting material atm
bool IsCraftingItem(int itype) {
	switch(itype) {
		case DND_ITEM_ORB:
		case DND_ITEM_TOKEN:
		return true;
	}
	return false;
}

bool IsCraftableItem(int itype) {
	// just so that this is recognized
	if(itype > UNIQUE_BEGIN)
		itype &= 0xFFFF; // lower 16 bits contain item type
	switch(itype) {
		case DND_ITEM_CHARM:
		case DND_ITEM_BOOT:
		case DND_ITEM_HELM:
		case DND_ITEM_NECKLACE:
		case DND_ITEM_WEAPON:
		case DND_ITEM_BODYARMOR:
		return true;
	}
	return false;
}

bool IsUsableOnInventory(int itype) {
	// just so that this is recognized
	if(itype > UNIQUE_BEGIN)
		itype &= 0xFFFF; // lower 16 bits contain item type
	switch(itype) {
		case DND_ITEM_CHARM:
		case DND_ITEM_BOOT:
		case DND_ITEM_HELM:
		case DND_ITEM_NECKLACE:
		return true;
	}
	return false;
}

bool IsInventoryCorrupted(int pnum, int item_id) {
	return PlayerInventoryList[pnum][item_id].corrupted;
}

// will count crafting materials the player has currently in their inventory
int CountCraftingMaterials(bool countTokens) {
	int pnum = PlayerNumber();
	int res = 0;
	int unique_orbs = 0;
	int unique_tokens = 0;
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(IsCraftingItem(PlayerInventoryList[pnum][i].item_type)) {
			if(PlayerInventoryList[pnum][i].item_type == DND_ITEM_ORB && !IsSet(unique_orbs, PlayerInventoryList[pnum][i].item_subtype)) {
				++res;
				unique_orbs = SetBit(unique_orbs, PlayerInventoryList[pnum][i].item_subtype);
			}
			else if(countTokens && PlayerInventoryList[pnum][i].item_type == DND_ITEM_TOKEN && !IsSet(unique_tokens, PlayerInventoryList[pnum][i].item_subtype)) {
				++res;
				unique_tokens = SetBit(unique_tokens, PlayerInventoryList[pnum][i].item_subtype);
			}
		}
	}
	return res;
}

// problem is this: say 8 orbs 5 elixirs, first page draws 12, next page requires next current is 12
// but this counts per item type, and per item type count will never make it to 12...
// soln: just count everything regardless, level order in inventory
int GetNextUniqueCraftingMaterial(int current, bool returnTokens) {
	int pnum = PlayerNumber();
	int res = 0, i;
	int unique_orbs = 0;
	int unique_tokens = 0;
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(IsCraftingItem(PlayerInventoryList[pnum][i].item_type)) {
			if(PlayerInventoryList[pnum][i].item_type == DND_ITEM_ORB && !IsSet(unique_orbs, PlayerInventoryList[pnum][i].item_subtype)) {
				++res;
				unique_orbs = SetBit(unique_orbs, PlayerInventoryList[pnum][i].item_subtype);
			}
			else if(returnTokens && PlayerInventoryList[pnum][i].item_type == DND_ITEM_TOKEN && !IsSet(unique_tokens, PlayerInventoryList[pnum][i].item_subtype)) {
				++res;
				unique_tokens = SetBit(unique_tokens, PlayerInventoryList[pnum][i].item_subtype);
			}
			// return the item's index
			if(res > current)
				return i;
		}
	}
	return -1;
}

/*int GetNextUniqueCraftableMaterial(int current) {
	int pnum = PlayerNumber();
	int res = 0, i;
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(IsCraftableItem(PlayerInventoryList[pnum][i].item_type) && PlayerInventoryList[pnum][i].height) {
			++res;
			// return the item's index
			if(res > current)
				return i;
		}
	}
	return -1;
}*/

int GetTotalStackOfMaterial(int itemid) {
	int pnum = PlayerNumber();
	int res = 0;
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i)
		if(PlayerInventoryList[pnum][i].item_type == PlayerInventoryList[pnum][itemid].item_type && PlayerInventoryList[pnum][i].item_subtype == PlayerInventoryList[pnum][itemid].item_subtype)
			res += PlayerInventoryList[pnum][i].item_stack;
	return res;
}

bool IsSelfUsableItem(int itype, int isubtype) {
	switch(itype) {
		// do all exceptions in here
		case DND_ITEM_ORB:
			switch(isubtype) {
				case DND_ORB_ENHANCE:
				case DND_ORB_REFINEMENT:
				case DND_ORB_PRISMATIC:
				case DND_ORB_CORRUPT:
				case DND_ORB_DESTRUCTION:
				case DND_ORB_VIOLENCE:
				case DND_ORB_FORTITUDE:
				case DND_ORB_PROSPERITY:
				case DND_ORB_TINKERER:
				case DND_ORB_SIN:
				case DND_ORB_SCULPTING:
				case DND_ORB_ELEVATION:
				case DND_ORB_HOLLOW:
				case DND_ORB_PHANTASMAL:
				case DND_ORB_NULLIFICATION:
				case DND_ORB_TURMOIL:
				case DND_ORB_TREMORS:
				case DND_ORB_HEXES:
				case DND_ORB_GROWTH:
				case DND_ORB_POTENCY:
				case DND_ORB_CRACKLING:
				case DND_ORB_BRUTE:
				case DND_ORB_JAGGED:
				case DND_ORB_ALCHEMIST:
				case DND_ORB_EVOKER:
				case DND_ORB_SAVAGERY:
				return false;
			}
		break;
		case DND_ITEM_TOKEN:
		return false;
	}
	return true;
}

void HandleEShieldChange(int pnum, bool remove) {
	int i = GetPlayerEnergyShieldCap(pnum);
	UpdateEnergyShieldVisual(i);

	if(remove && CheckInventory("EShieldAmount") > i)
		SetEnergyShield(i);
}

enum {
	REQ_SYNC_ACC = 1
};

int ProcessItemFeature(int pnum, int item_index, int source, int aindex, bool remove, bool has_cybernetic, bool noSync = false) {
	int atype = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, aindex, source);
	int aval = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, aindex, source);
	int aextra = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, aindex, source);
	int asubtype = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, item_index, aindex, source);
	int i, temp;
	int sync_required = 0;
	int cap;
	
	// Well of power factor
	temp = GetPlayerAttributeValue(pnum, INV_EX_FACTOR_SMALLCHARM);
	if(temp && asubtype == DND_CHARM_SMALL) {
		// don't multiply first in case of fixed point attributes, these are big numbers
		if(aval > 100 * FACTOR_FIXED_RESOLUTION) {
			aval /= FACTOR_FIXED_RESOLUTION;
			aval *= temp;
		}
		else {
			aval *= temp;
			aval /= FACTOR_FIXED_RESOLUTION; // our scale to lower it down from integer mult
		}

		aextra *= temp;
		aextra /= FACTOR_FIXED_RESOLUTION;
	}

	temp = GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_index, -1, source);
	if(temp) {
		// percent based
		temp += 100;
		// don't multiply first in case of fixed point attributes, these are big numbers
		if(aval > 100000) {
			aval /= 100;
			aval *= temp;
		}
		else {
			aval *= temp;
			aval /= 100;
		}
		aextra *= temp;
		aextra /= 100;
	}
	
	// cybernetic check
	if(has_cybernetic && CheckInventory("Cyborg_Perk25")) {
		aval *= DND_CYBERNETIC_FACTOR_MUL;
		aval /= DND_CYBERNETIC_FACTOR_DIV;
	}
	
	if(remove)
		aval = -aval;
	
	switch(atype) {
		// this is handled differently
		case INV_CYBERNETIC:
		break;
	
		// first cases with exceptions to our generic formula
		case INV_MAGAZINE_INCREASE:
			IncPlayerModValue(pnum, atype, aval, noSync);
			// add onto the base capacities, not current capacities
			for(i = 0; i < MAX_MAGAZINES; ++i)
				SetAmmoCapacity(WeaponMagazineList[i], (WeaponMagazineCaps[i] * (100 + GetPlayerAttributeValue(pnum, atype))) / 100);
		break;
		case INV_EXPLOSION_RADIUS:
			IncPlayerModValue(pnum, atype, aval, noSync);
			SetActorProperty(0, APROP_SCORE, GetPlayerAttributeValue(pnum, atype));
		break;
		
		// these are all accuracy mod groups
		case INV_ACCURACY_INCREASE:
		case INV_ESS_OMNISIGHT:
		case INV_ESS_OMNISIGHT2:
			IncPlayerModValue(pnum, atype, aval, noSync);
			sync_required = REQ_SYNC_ACC;
		break;
		
		// exotic stuff -- reason most of these dont have syncs is that they arent meant to be shown in stat menu page, so no need for client to be aware
		case INV_EX_FORSHOW_BURSTGETSPELLETBONUS:
			// by itself these do nothing
		break;

		// things that have EXTRA field used!
		case INV_EX_CHANCE_HEALMISSINGONPAIN:
			IncPlayerModValue(pnum, atype, aval, noSync);
			IncPlayerModExtra(pnum, atype, aextra, noSync);
		break;

		case INV_EX_KNOCKBACK_IMMUNITY:
			IncPlayerModValue(pnum, atype, aval, noSync);
			UpdatePlayerKnockbackResist();
		break;
		case INV_EX_FACTOR_SMALLCHARM:
			if(!remove) {
				// we now need to re-apply all other features of small charms we have equipped
				// first 4 are small charms
				for(i = 0; i < 4; ++i)
					if(Items_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_REMOVE, true);
				
				// now give the item and re-apply
				IncPlayerModValue(pnum, atype, aval, true);
								
				for(i = 0; i < 4; ++i)
					if(Items_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_ADD, true);
								
				// sync all at once at the end here for well of power...
				SyncPlayerItemMods(pnum);
			}
			else if(PlayerModValues[pnum][atype]) {
				// just take the attribute off and remove features and reapply
				for(i = 0; i < 4; ++i)
					if(Items_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_REMOVE, true);
										
				// little note: aval can be negative if we are removing, so just + is enough to subtract it
				IncPlayerModValue(pnum, atype, aval, true);
								
				// reapply with this gone
				for(i = 0; i < 4; ++i)
					if(Items_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_ADD, true);
						
				// sync all at once for well of power
				SyncPlayerItemMods(pnum);
			}
		break;
		case INV_EX_ALLSTATS:
			for(i = INV_STAT_STRENGTH; i <= INV_STAT_INTELLECT; ++i)
				IncPlayerModValue(pnum, i, aval, noSync);
			
			// for str
			UpdatePlayerKnockbackResist();

			// for vit
			i = GetActorProperty(0, APROP_HEALTH) - GetSpawnHealth();
			if(remove) {
				temp = GetSpawnHealth();
				if(GetActorProperty(0, APROP_HEALTH) > temp) {
					// set health to new cap, add the extra to player
					if(i > 0)
						SetActorProperty(0, APROP_HEALTH, temp + i);
					else
						SetActorProperty(0, APROP_HEALTH, temp);
				}
			}
		break;
		case INV_EX_DOUBLE_HEALTHCAP:
			IncPlayerModValue(pnum, INV_HPPERCENT_INCREASE, aval, noSync);
			i = GetActorProperty(0, APROP_HEALTH) - GetSpawnHealth();
			if(remove) {
				temp = GetSpawnHealth();
				if(GetActorProperty(0, APROP_HEALTH) > temp) {
					// set health to new cap, add the extra to player
					if(i > 0)
						SetActorProperty(0, APROP_HEALTH, temp + i);
					else
						SetActorProperty(0, APROP_HEALTH, temp);
				}
			}
		break;
		case INV_EX_PHYSDAMAGEPER_FLATHEALTH:
			// first check all sources, see if they contain this and are lower than this source
			temp = FindMinOnUsedCharmsForAttribute(pnum, INV_EX_PHYSDAMAGEPER_FLATHEALTH, item_index);
			// we got a new min
			if(temp != -1) {
				// update to use the new min if our comparison is better -- dont care otherwise
				if(!remove)
					SetPlayerModValue(pnum, atype, Min(aval, PlayerModValues[pnum][atype]), noSync);
			}
			else {
				// no new min was found
				if(remove)
					SetPlayerModValue(pnum, atype, 0, noSync);
				else
					SetPlayerModValue(pnum, atype, aval, noSync);
			}
		break;
		case INV_EX_FLATDMG_ALL:
			for(i = INV_FLATPHYS_DAMAGE; i <= INV_FLATELEM_DAMAGE; ++i)
				IncPlayerModValue(pnum, i, aval, noSync);
		break;
		case INV_EX_ABILITY_RALLY:
			IncPlayerModValue(pnum, atype, aval, noSync);
			if(PlayerModValues[pnum][atype])
				GiveInventory("CastRally", 1);
			else
				TakeInventory("CastRally", 1);
		break;
		case INV_HP_INCREASE:
		case INV_HPPERCENT_INCREASE:
		case INV_STAT_VITALITY:
			i = GetActorProperty(0, APROP_HEALTH) - GetSpawnHealth();
			IncPlayerModValue(pnum, atype, aval, noSync);
			if(remove) {
				temp = GetSpawnHealth();
				if(GetActorProperty(0, APROP_HEALTH) > temp) {
					// set health to new cap, add the extra to player
					if(i > 0)
						SetActorProperty(0, APROP_HEALTH, temp + i);
					else
						SetActorProperty(0, APROP_HEALTH, temp);
				}
			}
		break;
		
		case INV_STAT_BULKINESS:
			IncPlayerModValue(pnum, atype, aval, noSync);
			UpdatePlayerKnockbackResist();
		break;
		case INV_SPEED_INCREASE:
			IncPlayerModValue(pnum, atype, aval, noSync);
			SetActorProperty(0, APROP_SPEED, GetPlayerSpeed(pnum));
		break;
		case INV_AMMOCAP_INCREASE:
			IncPlayerModValue(pnum, atype, aval, noSync);
			// make sure to update ammo caps
			SetAllAmmoCapacities();
		break;
		case INV_EX_CURSEIMMUNITY:
			IncPlayerModValue(pnum, atype, aval, noSync);
			if(PlayerModValues[pnum][atype])
				GiveInventory("CurseImmunity", 1);
			else
				HandleCurseImmunityRemoval();
		break;
		case INV_STAT_STRENGTH:
			IncPlayerModValue(pnum, atype, aval, noSync);
			UpdatePlayerKnockbackResist();
		break;

		case INV_SHIELD_INCREASE:
		case INV_PERCENTSHIELD_INCREASE:
			IncPlayerModValue(pnum, atype, aval, noSync);

			HandleEShieldChange(pnum, remove);
		break;
		
		// anything that fits our generic formula
		default:
			IncPlayerModValue(pnum, atype, aval, noSync);
		break;
	}

	return sync_required;
}

bool IsAttributeExtraException(int attr) {
	switch(attr) {
		case INV_IMP_INCARMOR:
		case INV_IMP_INCARMORSHIELD:
		case INV_IMP_INCSHIELD:
		return true;
	}
	return false;
}

void ProcessItemImplicit(int pnum, int item_index, int source, bool remove, bool has_cybernetic, bool noSync = false) {
	int atype = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, -1, source);
	int aval = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, -1, source);
	int asubtype = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, item_index, -1, source);
	int aextra = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, -1, source);
	int i, cap;

	// Well of power factor
	int temp = GetPlayerAttributeValue(pnum, INV_EX_FACTOR_SMALLCHARM);
	if(temp && asubtype == DND_CHARM_SMALL) {
		// don't multiply first in case of fixed point attributes, these are big numbers
		if(aval > 100 * FACTOR_FIXED_RESOLUTION) {
			aval /= FACTOR_FIXED_RESOLUTION;
			aval *= temp;
		}
		else {
			aval *= temp;
			aval /= FACTOR_FIXED_RESOLUTION; // our scale to lower it down from integer mult
		}

		aextra *= temp;
		aextra /= 100;
	}

	temp = GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_index, -1, source);
	if(temp) {
		// percent based
		temp += 100;
		// don't multiply first in case of fixed point attributes, these are big numbers
		if(aval > 100000) {
			aval /= 100;
			aval *= temp;
		}
		else {
			aval *= temp;
			aval /= 100;
		}

		if(!IsAttributeExtraException(atype)) {
			aextra *= temp;
			aextra /= 100;
		}
	}
	
	// cybernetic check
	if(has_cybernetic && CheckInventory("Cyborg_Perk25")) {
		aval *= DND_CYBERNETIC_FACTOR_MUL;
		aval /= DND_CYBERNETIC_FACTOR_DIV;
	}
	
	if(remove)
		aval = -aval;

	switch(atype) {
		// standard implicits
		case INV_IMP_INCARMOR:
			IncPlayerModValue(pnum, INV_ARMOR_INCREASE, aval, noSync);
			if(aextra) {
				if(!remove)
					aextra = GetPlayerAttributeValue(pnum, INV_EX_PLAYERPOWERSET1) | aextra;
				else
					aextra = GetPlayerAttributeValue(pnum, INV_EX_PLAYERPOWERSET1) & ~(1 << aextra);

				SetPlayerModValue(pnum, INV_EX_PLAYERPOWERSET1, aextra, noSync);
			}
		break;
		case INV_IMP_INCSHIELD:
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval, noSync);

			HandleEShieldChange(pnum, remove);

			if(aextra) {
				if(!remove)
					aextra = GetPlayerAttributeValue(pnum, INV_EX_PLAYERPOWERSET1) | aextra;
				else
					aextra = GetPlayerAttributeValue(pnum, INV_EX_PLAYERPOWERSET1) & ~(1 << aextra);

				SetPlayerModValue(pnum, INV_EX_PLAYERPOWERSET1, aextra, noSync);
			}
		break;
		case INV_IMP_INCARMORSHIELD:
			IncPlayerModValue(pnum, INV_ARMOR_INCREASE, aval, noSync);
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval, noSync);

			HandleEShieldChange(pnum, remove);

			if(aextra) {
				if(!remove)
					aextra = GetPlayerAttributeValue(pnum, INV_EX_PLAYERPOWERSET1) | aextra;
				else
					aextra = GetPlayerAttributeValue(pnum, INV_EX_PLAYERPOWERSET1) & ~(1 << aextra);

				SetPlayerModValue(pnum, INV_EX_PLAYERPOWERSET1, aextra, noSync);
			}
		break;

		// corrupted implicits
		// non-weapon mods
		case INV_CORR_SPEED:
			IncPlayerModValue(pnum, INV_SPEED_INCREASE, aval, noSync);
			SetActorProperty(0, APROP_SPEED, GetPlayerSpeed(pnum));
		break;
		case INV_CORR_DROPCHANCE:
			IncPlayerModValue(pnum, INV_DROPCHANCE_INCREASE, aval, noSync);
		break;
		case INV_CORR_PERCENTSTAT:
			IncPlayerModValue(pnum, atype, aval, noSync);

			// for str
			UpdatePlayerKnockbackResist();

			// for vit
			i = GetActorProperty(0, APROP_HEALTH) - GetSpawnHealth();
			if(remove) {
				temp = GetSpawnHealth();
				if(GetActorProperty(0, APROP_HEALTH) > temp) {
					// set health to new cap, add the extra to player
					if(i > 0)
						SetActorProperty(0, APROP_HEALTH, temp + i);
					else
						SetActorProperty(0, APROP_HEALTH, temp);
				}
			}
		break;

		// weapon mods
		case INV_CORR_WEAPONCRIT:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_CRITPERCENT][WMOD_ITEMS].val += aval;
			SyncClientsideVariable_WeaponMods(pnum, aextra);
		break;
		case INV_CORR_WEAPONCRITDMG:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_CRITDMG][WMOD_ITEMS].val += aval;
			SyncClientsideVariable_WeaponMods(pnum, aextra);
		break;
		case INV_CORR_WEAPONDMG:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_DMG][WMOD_ITEMS].val += aval;
			SyncClientsideVariable_WeaponMods(pnum, aextra);
		break;
		case INV_CORR_WEAPONFORCEPAIN:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_FORCEPAINCHANCE][WMOD_ITEMS].val += aval;
			SyncClientsideVariable_WeaponMods(pnum, aextra);
		break;
		case INV_CORR_WEAPONPCTDMG:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_PERCENTDAMAGE][WMOD_ITEMS].val += aval;
			SyncClientsideVariable_WeaponMods(pnum, aextra);
		break;
		case INV_CORR_WEAPONPOISONPCT:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_POISONFORPERCENTDAMAGE][WMOD_ITEMS].val += aval;
			SyncClientsideVariable_WeaponMods(pnum, aextra);
		break;
	}
}

// Applies item stats to player -- can remove or add
void ApplyItemFeatures(int pnum, int item_index, int source, bool remove = false, bool noSync = false) {
	int ac = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	
	// check cybernetic and put it as bool
	bool has_cybernetic = false;
	int i = 0;
	for(i = 0; i < ac; ++i) {
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, i, source) == INV_CYBERNETIC) {
			has_cybernetic = true;
			break;
		}
	}

	int sync_required = 0;
	for(i = 0; i < ac; ++i)
		sync_required |= ProcessItemFeature(pnum, item_index, source, i, remove, has_cybernetic, noSync);

	ProcessItemImplicit(pnum, item_index, source, remove, has_cybernetic, noSync);
}

int GetCraftableItemCount() {
	int res = 0, pnum = PlayerNumber();
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i)
		if(IsCraftableItem(PlayerInventoryList[pnum][i].item_type) && PlayerInventoryList[pnum][i].height)
			++res;
	return res;
}

int GetItemTierRoll(int lvl, bool isWellRolled) {
	// 10% chance to roll a tier up or down -- if well rolled 20%
	if(!random(0, 9 - 5 * isWellRolled))
		++lvl;
	else if(!random(0, 9 - 5 * isWellRolled))
		--lvl;
	return lvl;
}

void InsertAttributeToItem(int pnum, int item_pos, int a_id, int a_val, int a_tier, int a_extra = 0, bool a_fracture = false) {
	int temp = PlayerInventoryList[pnum][item_pos].attrib_count++;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_id = a_id;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_val = a_val;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_tier = a_tier;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_extra = a_extra;
	PlayerInventoryList[pnum][item_pos].attributes[temp].fractured = a_fracture;
}

// can only add attributes to items that are about to be created ie. on field dropped from monster
void AddAttributeToFieldItem(int item_pos, int attrib, int pnum, int max_affixes = 0) {
	if(!max_affixes)
		max_affixes = Charm_MaxAffixes[Inventories_On_Field[item_pos].item_subtype];
	if(Inventories_On_Field[item_pos].attrib_count < max_affixes) {
		int temp = Inventories_On_Field[item_pos].attrib_count++;
		int lvl = Inventories_On_Field[item_pos].item_level / CHARM_ATTRIBLEVEL_SEPERATOR;
		
		bool makeWellRolled = CheckWellRolled(pnum);
		
		lvl = GetItemTierRoll(lvl, makeWellRolled);

		// force within bounds
		lvl = Clamp_Between(lvl, 0, MAX_CHARM_AFFIXTIERS);
		Inventories_On_Field[item_pos].attributes[temp].attrib_tier = lvl;
		Inventories_On_Field[item_pos].attributes[temp].attrib_id = attrib;
		Inventories_On_Field[item_pos].attributes[temp].fractured = false;

		// it basically adds the step value (val) and a +1 if we aren't 0, so our range is ex: 5-10 in tier 1 then 11-15 in tier 2 assuming +5 range per tier
		// luck adds a small chance for a charm to have well rolled modifier on it -- luck gain is 0.15, 0.05 x 10 = 0.5 max rank thats 50% chance for well rolled mods
		Inventories_On_Field[item_pos].attributes[temp].attrib_val = RollAttributeValue(
			attrib, 
			lvl, 
			makeWellRolled,
			Inventories_On_Field[item_pos].item_type,
			Inventories_On_Field[item_pos].item_subtype
		);
	}
}

// adds attribute to existing item in player inventory
void AddAttributeToItem(int pnum, int item_pos, int attrib, bool isWellRolled = false) {
	int temp = PlayerInventoryList[pnum][item_pos].attrib_count++;
	int lvl = GetItemTier(PlayerInventoryList[pnum][item_pos].item_level);
	
	// 10% chance to roll a tier up or down for the modifier on the charm
	lvl = GetItemTierRoll(lvl, isWellRolled);
	
	// force within bounds
	lvl = Clamp_Between(lvl, 0, MAX_CHARM_AFFIXTIERS);
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_tier = lvl;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_id = attrib;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_extra = 0; // set this to 0, if the rollattributevalue needs to assign the extra it will
	PlayerInventoryList[pnum][item_pos].attributes[temp].fractured = false;
	
	// roll the attribute
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_val = RollAttributeValue(
		attrib, 
		lvl,
		isWellRolled,
		PlayerInventoryList[pnum][item_pos].item_type,
		PlayerInventoryList[pnum][item_pos].item_subtype
	);
}

void GiveImplicitToField(int item_pos, int attr, int val, int extra = -1, int tier = 0, int tier_mapping = 0) {
	if(extra != -1)
		Inventories_On_Field[item_pos].implicit.attrib_extra = extra;

	Inventories_On_Field[item_pos].implicit.attrib_id = attr;
	Inventories_On_Field[item_pos].implicit.attrib_tier = tier;

	if(!tier)
		Inventories_On_Field[item_pos].implicit.attrib_val = val;
	else {
		int temp = GetItemTier(tier);
		if(tier_mapping)
			Inventories_On_Field[item_pos].implicit.attrib_val = random(val + temp * tier_mapping, val + (temp + 1) * tier_mapping);
		else
			Inventories_On_Field[item_pos].implicit.attrib_val = val * (temp + 1);
	}
}

void GiveCorruptedImplicit(int pnum, int item_pos) {
	int corr_mod = random(FIRST_CORRUPT_IMPLICIT, LAST_CORRUPT_IMPLICIT);
	int extra = GetExtraForMod(corr_mod);

	if(extra != -1)
		PlayerInventoryList[pnum][item_pos].implicit.attrib_extra = extra;

	PlayerInventoryList[pnum][item_pos].implicit.attrib_id = corr_mod;
	PlayerInventoryList[pnum][item_pos].implicit.attrib_tier = 0;

	// roll the value for this now
	PlayerInventoryList[pnum][item_pos].implicit.attrib_val = random(ItemModTable[corr_mod].attrib_low, ItemModTable[corr_mod].attrib_high);
}

int GetItemFracturedModCount(int pnum, int item_pos) {
	int fc = 0;
	for(int i = 0; i < PlayerInventoryList[pnum][item_pos].attrib_count; ++i)
		fc += PlayerInventoryList[pnum][item_pos].attributes[i].fractured;
	return fc;
}

// Leaves nothing but fractured mods and implicits on it
int ScourItem(int pnum, int item_pos) {
	// scan for fractured mods
	int min_count = GetItemFracturedModCount(pnum, item_pos);
	int frac_id = 0;
	int i;

	// completely reset the item attribs
	for(i = 0; i < PlayerInventoryList[pnum][item_pos].attrib_count; ++i) {
		// is this fractured
		if(PlayerInventoryList[pnum][item_pos].attributes[i].fractured) {
			// if fractured mods are on top, ordered, do not erase or do anything
			if(frac_id == i)
				continue;
			// move the fractured mod to the beginning
			PlayerInventoryList[pnum][item_pos].attributes[frac_id].attrib_val = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_val;
			PlayerInventoryList[pnum][item_pos].attributes[frac_id].attrib_tier = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier;
			PlayerInventoryList[pnum][item_pos].attributes[frac_id].attrib_id = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_id;
			PlayerInventoryList[pnum][item_pos].attributes[frac_id].attrib_extra = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_extra;
			PlayerInventoryList[pnum][item_pos].attributes[frac_id].fractured = true;
			++frac_id;
		}

		// erase the current attribute slot
		PlayerInventoryList[pnum][item_pos].attributes[i].attrib_val = 0;
		PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier = 0;
		PlayerInventoryList[pnum][item_pos].attributes[i].attrib_id = 0;
		PlayerInventoryList[pnum][item_pos].attributes[i].attrib_extra = 0;
		PlayerInventoryList[pnum][item_pos].attributes[i].fractured = 0;
	}

	PlayerInventoryList[pnum][item_pos].attrib_count = min_count;

	// return the new minimum count, can be 0 or non-zero if fractured mods exist
	return min_count;
}

bool CanAllowModRollSpecial(int tag, int special_roll_rule) {
	return 	(tag == INV_ATTR_TAG_ELEMENTAL_ID && (special_roll_rule & PPOWER_CANROLLELEMENTAL)) ||
			(tag == INV_ATTR_TAG_OCCULT_ID && (special_roll_rule & PPOWER_CANROLLOCCULT)) ||
			(tag == INV_ATTR_TAG_PHYSICAL_ID && (special_roll_rule & PPOWER_CANROLLPHYS)) ||
			(tag == INV_ATTR_TAG_EXPLOSIVE_ID && (special_roll_rule & PPOWER_CANROLLEXP)) ||
			(tag == INV_ATTR_TAG_ENERGY_ID && (special_roll_rule & PPOWER_CANROLLENERGY));
}

bool IsImplicitException(int imp, int rolled_attr) {
	switch(imp) {
		// don't let eshield modifiers roll on armor base items
		case INV_IMP_INCARMOR:
		return rolled_attr == INV_SHIELD_INCREASE || rolled_attr == INV_SHIELD_RECHARGEDELAY || rolled_attr == INV_SHIELD_RECOVERYRATE;
	}
	return false;
}

// special roll rule holds PPOWER_CANROLLXXXX and it checks what is possible based on that
int PickRandomAttribute(int item_type = DND_ITEM_CHARM, int special_roll_rule = 0, int implicit_id = -1) {
	int bias = Timer() & 0xFFFF;
	int val;

	if(item_type == DND_ITEM_CHARM) {
		// unrestricted picking
		val = random(FIRST_INV_ATTRIBUTE + bias, LAST_INV_ATTRIBUTE + bias) - bias;
		// this is a last resort random here, in case there was an overflow... shouldn't, but might
		// this random really didn't want to pick the edge values for some reason so we use the shifted one above...
		if(val < 0)
			val = random(FIRST_INV_ATTRIBUTE, LAST_INV_ATTRIBUTE);
	}
	else {
		item_type = MapItemTypeToCraftableID(item_type);

		// find a random valid tag for this item
		int tag;
		do {
			do {
				tag = random(DND_ATTRIB_TAG_ID_BEGIN + bias, DND_ATTRIB_TAG_ID_END + bias) - bias;
				if(tag < 0)
					tag = random(DND_ATTRIB_TAG_ID_BEGIN, DND_ATTRIB_TAG_ID_END);

				// check rule exceptions -- compare vs charms for "cant roll" condition, charms can roll anything
				if(AttributeTagGroupCount[tag][item_type] < AttributeTagGroupCount[tag][DND_CRAFTABLEID_CHARM]) {
					// check potential special rolls
					if(CanAllowModRollSpecial(tag, special_roll_rule)) {
						// charms can roll everything possible, so we switch it to that, and then let it pick from that category
						item_type = DND_CRAFTABLEID_CHARM;
						break;
					}
				}
				// we check for "0" here because, if the above doesnt make it reroll into a wider pool, and if theres non-zero, that means we still get valid stuff here
			} while(!AttributeTagGroupCount[tag][item_type]);

			// finally roll the attrib at random from the group
			val = random(bias, AttributeTagGroupCount[tag][item_type] + bias) - bias;
			if(val < 0)
				val = random(0, AttributeTagGroupCount[tag][item_type]);
			val = AttributeTagGroups[tag][item_type][val];
			// finally check for implicit exception => Ex: Don't roll EShield on Armor base items!
		} while(IsImplicitException(implicit_id, val));
	}
	return val;
}

void AssignAttributes(int pnum, int item_pos, int itype, int attr_count) {
	int special_roll = 0;
	if(PlayerInventoryList[pnum][item_pos].implicit.attrib_id != -1 && itype == DND_ITEM_BODYARMOR) {
		special_roll = PlayerInventoryList[pnum][item_pos].implicit.attrib_extra;
	}
	
	int i = 0, roll;
	while(i < attr_count) {
		do {
			roll = PickRandomAttribute(itype, special_roll, PlayerInventoryList[pnum][item_pos].implicit.attrib_id);
		} while(CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[pnum][item_pos].attrib_count) != -1);
		AddAttributeToItem(pnum, item_pos, roll);
		++i;
	}
}

void ReforgeItem(int pnum, int item_pos) {
	int itype = PlayerInventoryList[pnum][item_pos].item_type;

	int min_count = ScourItem(pnum, item_pos);
	
	// subtract the fractured mods on it from what it can max have
	int attr_count = 0;
	if(itype == DND_ITEM_CHARM) {
		int max_natural = RollCharmMaxAttribCount(PlayerInventoryList[pnum][item_pos].item_subtype);
		attr_count = random(min(1, max_natural - min_count), max_natural - min_count);
	}
	else if(itype == DND_ITEM_BODYARMOR)
		attr_count = random(1, MAX_ARMOR_ATTRIB_DEFAULT) - min_count;

	if(attr_count > 0)
		AssignAttributes(pnum, item_pos, itype, attr_count);
}

void RemoveAttributeFromItem(int pnum, int item_id, int to_remove) {
	// all attributes must be shifted left from the position of the deleted attribute now
	for(int i = to_remove; i < PlayerInventoryList[pnum][item_id].attrib_count - 1; ++i) {
		PlayerInventoryList[pnum][item_id].attributes[i].attrib_id = PlayerInventoryList[pnum][item_id].attributes[i + 1].attrib_id;
		PlayerInventoryList[pnum][item_id].attributes[i].attrib_val = PlayerInventoryList[pnum][item_id].attributes[i + 1].attrib_val;
		PlayerInventoryList[pnum][item_id].attributes[i].attrib_tier = PlayerInventoryList[pnum][item_id].attributes[i + 1].attrib_tier;
		PlayerInventoryList[pnum][item_id].attributes[i].attrib_extra = PlayerInventoryList[pnum][item_id].attributes[i + 1].attrib_extra;
		PlayerInventoryList[pnum][item_id].attributes[i].fractured = PlayerInventoryList[pnum][item_id].attributes[i + 1].fractured;
	}
	--PlayerInventoryList[pnum][item_id].attrib_count;
}

// Gives an attribute of a tag group guaranteed, and completely reforges the attribs
void ReforgeWithOneTagGuaranteed(int pnum, int item_pos, int tag_id) {
	int itype = PlayerInventoryList[pnum][item_pos].item_type;
	int craftable_type;
	
	int min_count = ScourItem(pnum, item_pos);
	
	// charm group etc.
	int rand_attr = -1, attr_count = 0;
	if(itype == DND_ITEM_CHARM) {
		craftable_type = DND_CRAFTABLEID_CHARM;
		rand_attr = random(0, AttributeTagGroupCount[tag_id][craftable_type] - 1);
		attr_count = RollCharmMaxAttribCount(PlayerInventoryList[pnum][item_pos].item_subtype) - min_count;

		// in case this is a fully fractured mod item
		if(attr_count <= 0)
			return;

		AddAttributeToItem(pnum, item_pos, AttributeTagGroups[tag_id][craftable_type][rand_attr]);
		--attr_count;
	}
	else if(itype == DND_ITEM_BODYARMOR) {
		craftable_type = DND_CRAFTABLEID_BODYARMOR;
		attr_count = random(1, MAX_ARMOR_ATTRIB_DEFAULT) - min_count;

		// in case this is a fully fractured mod item or we rolled less than amount of fractures
		if(attr_count <= 0)
			return;

		// if no attributes of this type are allowed, but we have some special roll, include it and try again
		if(!AttributeTagGroupCount[tag_id][craftable_type]) {
			if(PlayerInventoryList[pnum][item_pos].implicit.attrib_id != -1 && CanAllowModRollSpecial(tag_id, PlayerInventoryList[pnum][item_pos].implicit.attrib_extra)) {
				craftable_type = DND_CRAFTABLEID_CHARM;
				rand_attr = random(0, AttributeTagGroupCount[tag_id][craftable_type] - 1);
			}
			else {
				// rest of the mods, we can't fit a guaranteed attribute here
				AssignAttributes(pnum, item_pos, itype, attr_count);
				return;
			}
		}
		else// we have an attribute of this type fitting, good, go ahead
			rand_attr = random(0, AttributeTagGroupCount[tag_id][craftable_type] - 1);

		AddAttributeToItem(pnum, item_pos, AttributeTagGroups[tag_id][craftable_type][rand_attr]);
		--attr_count;
	}

	// rest of the mods
	AssignAttributes(pnum, item_pos, itype, attr_count);
}

// this doesn't consider the item_type yet!
void MakeUnique(int item_pos, int item_type, int pnum) {
	int i;
	if(GetCVar("dnd_ignore_dropweights"))
		i = random(0, MAX_UNIQUE_ITEMS - 1);
	else {
		int roll = random(1, MAX_UNIQUE_WEIGHT);
		for(i = 0; i < MAX_UNIQUE_ITEMS && roll > UniqueItemDropWeight[i]; ++i);
	}
	#ifdef ISDEBUGBUILD
	i = random(0, MAX_UNIQUE_ITEMS - 1);
	//i = UITEM_ANCIENTGEMSTONE;
	#endif
	// i is the unique id
	ConstructUniqueOnField(item_pos, i, item_type, pnum);
}

void ConstructUniqueOnField(int fieldpos, int unique_id, int item_type, int pnum) {
	Inventories_On_Field[fieldpos].width = UniqueItemList[unique_id].width;
	Inventories_On_Field[fieldpos].height = UniqueItemList[unique_id].height;
	Inventories_On_Field[fieldpos].item_type = UniqueItemList[unique_id].item_type;
	Inventories_On_Field[fieldpos].item_subtype = UniqueItemList[unique_id].item_subtype;
	Inventories_On_Field[fieldpos].item_image = UniqueItemList[unique_id].item_image;
	Inventories_On_Field[fieldpos].item_level = UniqueItemList[unique_id].item_level;
	Inventories_On_Field[fieldpos].item_stack = UniqueItemList[unique_id].item_stack;
	Inventories_On_Field[fieldpos].attrib_count = UniqueItemList[unique_id].attrib_count;
	Inventories_On_Field[fieldpos].topleftboxid = 0;

	Inventories_On_Field[fieldpos].corrupted = 0;
	Inventories_On_Field[fieldpos].quality = 0;

	for(int i = 0; i < Inventories_On_Field[fieldpos].attrib_count; ++i) {
		Inventories_On_Field[fieldpos].attributes[i].attrib_id = UniqueItemList[unique_id].attrib_id_list[i];
		Inventories_On_Field[fieldpos].attributes[i].attrib_tier = 0;
		
		// we must roll the value once dropped
		bool makeWellRolled = CheckWellRolled(pnum);
		if(!makeWellRolled) {
			Inventories_On_Field[fieldpos].attributes[i].attrib_val = random(UniqueItemList[unique_id].rolls[i].attrib_low, UniqueItemList[unique_id].rolls[i].attrib_high);
			Inventories_On_Field[fieldpos].attributes[i].attrib_extra = random(UniqueItemList[unique_id].rolls[i].attrib_extra_low, UniqueItemList[unique_id].rolls[i].attrib_extra_high);
		}
		else {
			Inventories_On_Field[fieldpos].attributes[i].attrib_val = random((UniqueItemList[unique_id].rolls[i].attrib_low + UniqueItemList[unique_id].rolls[i].attrib_high) / 2, UniqueItemList[unique_id].rolls[i].attrib_high);
			Inventories_On_Field[fieldpos].attributes[i].attrib_extra = random((UniqueItemList[unique_id].rolls[i].attrib_extra_low + UniqueItemList[unique_id].rolls[i].attrib_extra_high) / 2, UniqueItemList[unique_id].rolls[i].attrib_extra_high);
		}
	}
}

void ResetPlayerInventory(int pnum) {
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(PlayerInventoryList[pnum][i].topleftboxid - 1 == i)
			SyncItemData_Null(pnum, i, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[pnum][i].width, PlayerInventoryList[pnum][i].height);
		PlayerInventoryList[pnum][i].item_type = DND_ITEM_NULL;
		PlayerInventoryList[pnum][i].width = 0;
		PlayerInventoryList[pnum][i].height = 0;
		PlayerInventoryList[pnum][i].item_image = 0;
		PlayerInventoryList[pnum][i].item_type = DND_ITEM_NULL;
		PlayerInventoryList[pnum][i].item_subtype = 0;
		PlayerInventoryList[pnum][i].item_level = 0;
		PlayerInventoryList[pnum][i].item_stack = 0;
		PlayerInventoryList[pnum][i].topleftboxid = 0;

		PlayerInventoryList[pnum][i].corrupted = 0;
		PlayerInventoryList[pnum][i].quality = 0;
		PlayerInventoryList[pnum][i].implicit.attrib_id = -1;
		PlayerInventoryList[pnum][i].implicit.attrib_val = 0;
		PlayerInventoryList[pnum][i].implicit.attrib_tier = 0;
		PlayerInventoryList[pnum][i].implicit.attrib_extra = 0;
		
		for(int j = 0; j < PlayerInventoryList[pnum][i].attrib_count; ++j) {
			PlayerInventoryList[pnum][i].attributes[j].attrib_id = 0;
			PlayerInventoryList[pnum][i].attributes[j].attrib_val = 0;
			PlayerInventoryList[pnum][i].attributes[j].attrib_tier = 0;
			PlayerInventoryList[pnum][i].attributes[j].attrib_extra = 0;
			PlayerInventoryList[pnum][i].attributes[j].fractured = 0;
		}
		PlayerInventoryList[pnum][i].attrib_count = 0;
	}
}

void ResetTradeViewList(int pnum) {
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(TradeViewList[pnum][i].topleftboxid - 1 == i)
			SyncItemData_Null(pnum, i, DND_SYNC_ITEMSOURCE_TRADEVIEW, TradeViewList[pnum][i].width, TradeViewList[pnum][i].height);
		TradeViewList[pnum][i].item_type = DND_ITEM_NULL;
		TradeViewList[pnum][i].width = 0;
		TradeViewList[pnum][i].height = 0;
		TradeViewList[pnum][i].item_image = 0;
		TradeViewList[pnum][i].item_type = 0;
		TradeViewList[pnum][i].item_subtype = 0;
		TradeViewList[pnum][i].item_level = 0;
		TradeViewList[pnum][i].item_stack = 0;
		TradeViewList[pnum][i].topleftboxid = 0;

		TradeViewList[pnum][i].corrupted = 0;
		TradeViewList[pnum][i].quality = 0;
		TradeViewList[pnum][i].implicit.attrib_id = -1;
		TradeViewList[pnum][i].implicit.attrib_val = 0;
		TradeViewList[pnum][i].implicit.attrib_tier = 0;
		TradeViewList[pnum][i].implicit.attrib_extra = 0;

		for(int j = 0; j < TradeViewList[pnum][i].attrib_count; ++j) {
			TradeViewList[pnum][i].attributes[j].attrib_id = 0;
			TradeViewList[pnum][i].attributes[j].attrib_val = 0;
			TradeViewList[pnum][i].attributes[j].attrib_tier = 0;
			TradeViewList[pnum][i].attributes[j].attrib_extra = 0;
			TradeViewList[pnum][i].attributes[j].fractured = 0;
		}
		TradeViewList[pnum][i].attrib_count = 0;
	}
}

void ResetFieldInventory() {
	for(int i = 0; i < MAX_INVENTORIES_ON_FIELD; ++i) {
		Inventories_On_Field[i].item_type = DND_ITEM_NULL;
		Inventories_On_Field[i].width = 0;
		Inventories_On_Field[i].height = 0;
		Inventories_On_Field[i].item_image = 0;
		Inventories_On_Field[i].item_type = DND_ITEM_NULL;
		Inventories_On_Field[i].item_subtype = 0;
		Inventories_On_Field[i].item_level = 0;
		Inventories_On_Field[i].item_stack = 0;
		Inventories_On_Field[i].topleftboxid = 0;

		Inventories_On_Field[i].corrupted = 0;
		Inventories_On_Field[i].quality = 0;
		Inventories_On_Field[i].implicit.attrib_id = -1;
		Inventories_On_Field[i].implicit.attrib_val = 0;
		Inventories_On_Field[i].implicit.attrib_tier = 0;
		Inventories_On_Field[i].implicit.attrib_extra = 0;

		for(int j = 0; j < Inventories_On_Field[i].attrib_count; ++j) {
			Inventories_On_Field[i].attributes[j].attrib_id = 0;
			Inventories_On_Field[i].attributes[j].attrib_val = 0;
			Inventories_On_Field[i].attributes[j].attrib_tier = 0;
			Inventories_On_Field[i].attributes[j].attrib_extra = 0;
			Inventories_On_Field[i].attributes[j].fractured = 0;
		}
		Inventories_On_Field[i].attrib_count = 0;
	}
}

void ResetPlayerStash(int pnum) {
	for(int p = 0; p < MAX_EXTRA_INVENTORY_PAGES; ++p) {
		for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
			if(PlayerStashList[pnum][p][i].topleftboxid - 1 == i)
				SyncItemData_Null(pnum, i, DND_SYNC_ITEMSOURCE_STASH | (p << 16), PlayerStashList[pnum][p][i].width, PlayerStashList[pnum][p][i].height);
			PlayerStashList[pnum][p][i].item_type = DND_ITEM_NULL;
			PlayerStashList[pnum][p][i].width = 0;
			PlayerStashList[pnum][p][i].height = 0;
			PlayerStashList[pnum][p][i].item_image = 0;
			PlayerStashList[pnum][p][i].item_type = DND_ITEM_NULL;
			PlayerStashList[pnum][p][i].item_subtype = 0;
			PlayerStashList[pnum][p][i].item_level = 0;
			PlayerStashList[pnum][p][i].item_stack = 0;
			PlayerStashList[pnum][p][i].topleftboxid = 0;

			PlayerStashList[pnum][p][i].corrupted = 0;
			PlayerStashList[pnum][p][i].quality = 0;
			PlayerStashList[pnum][p][i].implicit.attrib_id = -1;
			PlayerStashList[pnum][p][i].implicit.attrib_val = 0;
			PlayerStashList[pnum][p][i].implicit.attrib_tier = 0;
			PlayerStashList[pnum][p][i].implicit.attrib_extra = 0;

			for(int j = 0; j < PlayerStashList[pnum][p][i].attrib_count; ++j) {
				PlayerStashList[pnum][p][i].attributes[j].attrib_id = 0;
				PlayerStashList[pnum][p][i].attributes[j].attrib_val = 0;
				PlayerStashList[pnum][p][i].attributes[j].attrib_tier = 0;
				PlayerStashList[pnum][p][i].attributes[j].attrib_extra = 0;
				PlayerStashList[pnum][p][i].attributes[j].fractured = 0;
			}
			PlayerStashList[pnum][p][i].attrib_count = 0;
		}
	}
}

// underlying assumption that the item isn't null, as it's returned from crafting screen
int DisassembleItem_Price(int pnum, int item_pos) {
	// we have a price band for the item, use the price the gauge what kind of orb(s) we can give to the player
	// make tier and amount of mods contribute highly to this too, so we need to do some of the steps we did again for this one
	int base = DND_BASE_DISASSEMBLE_COST;
	int ilvl = PlayerInventoryList[pnum][item_pos].item_level;

	int avg_mod_tier = 0;
	int acount = PlayerInventoryList[pnum][item_pos].attrib_count;
	int fracture_count = 0;
	if(acount) {
		for(int i = 0; i < acount; ++i) {
			// uniques have tier 0
			if(PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier)
				avg_mod_tier += PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier;
			else
				avg_mod_tier += MAX_CHARM_AFFIXTIERS / 2;
			fracture_count += PlayerInventoryList[pnum][item_pos].attributes[i].fractured;
		}
		avg_mod_tier /= acount;
	}

	base = base * (100 + (ilvl - 1) * DND_DISASSEMBLE_LEVEL_PERCENT) / 100;
	base = base * (100 + DND_DISASSEMBLE_TIER_PERCENT * avg_mod_tier) / 100;

	// if corrupted or has implicit, include that too
	if(PlayerInventoryList[pnum][item_pos].implicit.attrib_id != -1) {
		// 50% increase
		base *= 3;
		base >>= 1;
	}
	
	if(PlayerInventoryList[pnum][item_pos].corrupted) {
		// 50% increase
		base *= 3;
		base >>= 1;
	}

	return base;
}

// returns the chance and a score rating how big the orb rarity and yield should be depending on how loaded the item is
int GetDissassembleChance(int pnum, int item_pos) {
	int ilvl = PlayerInventoryList[pnum][item_pos].item_level;

	int avg_mod_tier = 0;
	int acount = PlayerInventoryList[pnum][item_pos].attrib_count;
	int fracture_count = 0;
	if(acount) {
		for(int i = 0; i < acount; ++i) {
			// uniques have tier 0
			if(PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier)
				avg_mod_tier += PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier;
			else
				avg_mod_tier += MAX_CHARM_AFFIXTIERS / 2;
			fracture_count += PlayerInventoryList[pnum][item_pos].attributes[i].fractured;
		}
		avg_mod_tier /= acount;
	}

	// give more chance to succeed if we have the research related to it too
	bool hasResearch = CheckResearchStatus(RES_MOLECULARREC);
	int chance = 	DND_BASE_DISASSEMBLE_CHANCE + 
					DND_BASE_DISASSEMBLE_CHANCE_PERLUCK * GetStat(STAT_LUCK) +
					hasResearch * DND_DISASS_CHANCEBONUS_RESEARCH;

	// 10% of ilvl + 25% of avg mod tier + 3% flat per fractured mod and 5% if corrupted to fail
	chance -= ilvl / 10 + avg_mod_tier / 4 + DND_BASE_FRACTURE_DISASSEMBLE_CHANCE * fracture_count + DND_BASE_CORRUPT_DISASSEMBLE_CHANCE * PlayerInventoryList[pnum][item_pos].corrupted;

	int yields = 	ilvl * DND_BASE_ILVL_YIELD +
					avg_mod_tier * DND_BASE_AVGMOD_YIELD + 
					DND_BASE_FRACTURE_YIELD * fracture_count + 
					DND_BASE_CORRUPT_YIELD * PlayerInventoryList[pnum][item_pos].corrupted +
					hasResearch + DND_DISASS_CHANCEBONUS_YIELD;
	
	if(yields > 0xFFFF)
		yields = 0xFFFF;

	return (chance << 16) + yields;
}

void DisassembleItem(int pnum, int item_pos, int price, int chance) {
	TakeInventory("Credit", price);
	// give more chance to succeed if we have the research related to it too
	int yield = chance & 0xFFFF;
	chance >>= 16;

	bool result = chance >= random(1, 100);
	if(result) {
		// success, give user orbs depending on yield
		do {
			int orb = -1;
			int sub = random(75, 150);
			if(yield >= 1000) {
				orb = PickHighTierOrb();
				sub *= 5;
			}
			else if(yield >= 500) {
				orb = PickMidTierOrb();
				sub *= 3;
			}
			else
				orb = PickLowTierOrb();
			yield -= sub;
			
			ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, orb, 1);
		} while(yield > 0);
	}

	ACS_NamedExecuteAlways("DnD Disassemble CS", 0, result);

	// destroy item and give nothing to the user
	FreeItem(pnum, item_pos, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	GiveInventory("DnD_CleanCraftingRequest", 1);
	GiveInventory("DnD_RefreshPane", 1);
	GiveInventory("DnD_CursorDataClearRequest", 1);
}

Script "DnD Disassemble CS" (int result) CLIENTSIDE {
	if(result) {
		Log(s:"\c[Y5]", l:"DND_DISASS", s:": \cd", l:"DND_DISASS_WIN");
		LocalAmbientSound("Items/SuccessDisassemble", 127);
	}
	else {
		Log(s:"\c[Y5]", l:"DND_DISASS", s:": \cg", l:"DND_DISASS_LOSS");
		LocalAmbientSound("Items/FailDisassemble", 127);
	}
}

#include "DnD_Token.h"

#endif
