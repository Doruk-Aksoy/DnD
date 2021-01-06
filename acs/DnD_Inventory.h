#ifndef DND_INVENTORY_IN
#define DND_INVENTORY_IN

#include "DnD_InvInfo.h"
#include "DnD_UniqueItems.h"

#define MAXINVENTORYBLOCKS_HORIZ 5
#define MAXINVENTORYBLOCKS_VERT 9

#define MAX_INVENTORY_BOXES MAXINVENTORYBLOCKS_HORIZ * MAXINVENTORYBLOCKS_VERT

#define MAX_ITEM_LEVEL 100
#define MAX_ITEM_AFFIXTIERS 4
#define ITEM_TIER_SEPERATOR 100 / MAX_ITEM_AFFIXTIERS

#define DND_SYNC_ITEMBEGIN DND_SYNC_ITEMTOPLEFTBOX
#define DND_SYNC_ITEMEND DND_SYNC_ITEMATTRIBUTES_VAL

#define INVENTORY_INFO_ALPHA 0.666
#define INVENTORY_HOLDTIME 0.5
#define INVENTORY_FADETIME 0.5

#define MAX_CHARM_AFFIXTIERS 4
#define CHARM_ATTRIBLEVEL_SEPERATOR 25 // just leave this as is...

#define MAX_EXTRA_INVENTORY_PAGES 10

#define MAXSTACKS_ORB 128
#define MAXSTACKS_CKEY 32
#define MAXSTACKS_ELIXIR 25

#define HUD_DII_FIELD_MULT 10

#define MAX_EXPRESIST_VAL 100
#define MAX_ELEMRESIST_VAL 100
#define MAX_PHYSRESIST_VAL 100
#define MAX_REFLRESIST_VAL 1000 // 1000 because we use 1 as 0.1% so 1000 is 100%
#define MAX_WEAKEN_VAL 200

#define MAX_SMALL_CHARMS_USED 4
#define MAX_MEDIUM_CHARMS_USED 2
#define MAX_LARGE_CHARMS_USED 1
#define MAX_CHARMS_EQUIPPABLE MAX_SMALL_CHARMS_USED + MAX_MEDIUM_CHARMS_USED + MAX_LARGE_CHARMS_USED

enum {
	IPROCESS_ADD,
	IPROCESS_REMOVE
};

// MENU IDS
// Moved here because of dependencies
enum {
	RPGMENUINVENTORYID = 699,
};

typedef struct imove {
	int width;
	int height;
	bool state;
} imove_T;

imove_T ItemMoveList[MAXPLAYERS][MAX_INVENTORY_BOXES];

// so far only charms count here
enum {
	IM_SCHRM1,
	IM_SCHRM2,
	IM_SCHRM3,
	
	IM_MCHRM1,
	IM_MCHRM2,
	IM_MCHRM3,
	
	IM_LCHRM1,
	IM_LCHRM2,
	IM_LCHRM3,
};

#define DND_SMALLCHARM_IMAGEBEGIN IM_SCHRM1
#define DND_SMALLCHARM_IMAGEEND IM_SCHRM3
#define DND_MEDIUMCHARM_IMAGEBEGIN IM_MCHRM1
#define DND_MEDIUMCHARM_IMAGEEND IM_MCHRM3
#define DND_LARGECHARM_IMAGEBEGIN IM_LCHRM1
#define DND_LARGECHARM_IMAGEEND IM_LCHRM3

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
	
	IIMG_CKEY_1,
	IIMG_CKEY_2,
	IIMG_CKEY_3,
	
	IIMG_ELIX_HEALTH,
	IIMG_ELIX_ARMOR,
	IIMG_ELIX_HPARMOR,
	IIMG_ELIX_HPPERCENT,
	IIMG_ELIX_ARMORPERCENT,
	IIMG_ELIX_HPARMORPERCENT,
	IIMG_ELIX_SPEED,
	IIMG_ELIX_DAMAGE,
	IIMG_ELIX_LUCK
};

#define ITEM_IMAGE_ORB_BEGIN IIMG_ORB_1
#define ITEM_IMAGE_KEY_BEGIN IIMG_CKEY_1
#define ITEM_IMAGE_ELIXIR_BEGIN IIMG_ELIX_HEALTH

#define MAX_ITEM_IMAGES IIMG_ELIX_LUCK + 1
str Item_Images[MAX_ITEM_IMAGES] = {
	// charms
	"SCHRM1",
	"SCHRM2",
	"SCHRM3",
	
	"MCHRM1",
	"MCHRM2",
	"MCHRM3",
	
	"LCHRM1",
	"LCHRM2",
	"LCHRM3",
	
	// unique charms
	"UCHRM1",
	"UCHRM2",
	"UCHRM3",
	"UCHRM4",
	"UCHRM5",
	"UCHRM6",
	"UCHRM7",
	"UCHRM8",
	"UCHRM9",
	"UCHRM10",
	"UCHRM11",
	"UCHRM12",
	"UCHRM13",
	"UCHRM14",
	
	// orbs
	"ORB1D0",
	"ORB1B0",
	"ORB1I0",
	"ORB1Z0",
	"ORB3A0",
	"ORB1N0",
	"ORB1S0",
	"ORB2J0",
	"ORB3E0",
	"ORB2O0",
	"ORB2V0",
	"ORB2E0",
	"ORB3G0",
	"ORB3I0",
	"ORB3K0",
	"ORB3M0",
	"ORB3O0",
	
	"SBKGA0",
	"SBKGB0",
	"SBKGC0",
	
	"ELIX01",
	"ELIX02",
	"ELIX03",
	"ELIX04",
	"ELIX05",
	"ELIX06",
	"ELIX07",
	"ELIX08",
	"ELIX09"
};

#define IOFFSET_X 0
#define IOFFSET_Y 1
// this is used to properly adjust images of items that don't get fit in trade/stash views
int Item_ImageOffsets[MAX_ITEM_IMAGES][2] = {
	{ 0.0, 0.0 },
	{ 0.0, 0.0 },
	{ 0.0, 0.0 },
	
	{ 0.0, 0.0 },
	{ 0.0, 0.0 },
	{ 0.0, 0.0 },
	
	{ 0.0, 0.0 },
	{ 0.0, 0.0 },
	{ 0.0, 0.0 },
	
	// unique charms
	{ 0.0, 8.0 },
	{ 0.0, 3.0 },
	{ 0.0, 5.0 },
	{ 0.0, 5.0 },
	{ 0.0, 3.0 },
	{ 0.0, 3.0 },
	{ 0.0, 8.0 },
	{ 0.0, 5.0 },
	{ 0.0, 6.0 },
	{ 0.0, 8.0 },
	{ 0.0, 4.0 },
	{ 0.0, 0.0 },
	{ 0.0, 0.0 },
	{ 0.0, 0.0 },
	
	// orbs
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	
	// chest keys
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	{ 7.0, 7.0 },
	
	// elixirs
	{ 0.0, 10.0 },
	{ 0.0, 10.0 },
	{ 0.0, 10.0 },
	{ 0.0, 10.0 },
	{ 0.0, 10.0 },
	{ 0.0, 10.0 },
	{ 0.0, 10.0 },
	{ 0.0, 10.0 },
	{ 0.0, 10.0 }
};

enum {
	DND_INVDROP_CHARM,
	DND_INVDROP_UNIQUECHARM,
};
#define MAX_DND_INVDROPACTORS DND_INVDROP_UNIQUECHARM + 1
str InventoryDropActors[MAX_DND_INVDROPACTORS] = {
	"CharmDrop",
	"UniqueCharmDrop"
};

#define ITEMLEVEL_VARIANCE_LOWER 15
#define ITEMLEVEL_VARIANCE_HIGHER 7

// holds inventories of all players
global inventory_T 11: PlayerInventoryList[MAXPLAYERS][MAX_INVENTORY_BOXES];

#define MAX_INVENTORIES_ON_FIELD 16384
global inventory_T 13: Inventories_On_Field[MAX_INVENTORIES_ON_FIELD];
global inventory_T 14: TradeViewList[MAXPLAYERS][MAX_INVENTORY_BOXES];
global inventory_T 15: PlayerStashList[MAXPLAYERS][MAX_EXTRA_INVENTORY_PAGES][MAX_INVENTORY_BOXES];

#define INVSOURCE_PLAYER PlayerInventoryList
#define INVSOURCE_CHARMUSED Charms_Used

int last_created_inventory = 0;

// Creates an item on the game field
int CreateItemSpot() {
	//Naive but very fast: Will just replace old item if index goes back to it - have a bigger array to prevent the negative effect.
	//Just having a loop here creates an error so avoid looping at all costs.
	//Remember, the floor gets cleared on a new map, so most likely the older items are useless for the players anyways, except on 4k mob slaugher maps.
	if ((++last_created_inventory) >= MAX_INVENTORIES_ON_FIELD)
		last_created_inventory = 0;
		
	// clear properties of this item before creating it -- fixes garbage data leftovers
	RemoveItemFromWorld(last_created_inventory);
		
	return last_created_inventory;
}

void RemoveItemFromWorld(int fieldpos) {
	Inventories_On_Field[fieldpos].width = 1;
	Inventories_On_Field[fieldpos].height = 1;
	Inventories_On_Field[fieldpos].item_type = DND_ITEM_NULL;
	Inventories_On_Field[fieldpos].item_subtype = 0;
	Inventories_On_Field[fieldpos].item_image = 0;
	Inventories_On_Field[fieldpos].item_level = 0;
	Inventories_On_Field[fieldpos].item_stack = 0;
	for(int k = 0; k < Inventories_On_Field[fieldpos].attrib_count; ++k) {
		Inventories_On_Field[fieldpos].attributes[k].attrib_id = 0;
		Inventories_On_Field[fieldpos].attributes[k].attrib_val = 0;
	}
	Inventories_On_Field[fieldpos].attrib_count = 0;
}

void FreeItem(int item_index, int source, bool dontSync) {
	int i, j;
	int temp;
	int wtemp = GetItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, -1, source);
	int htemp = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, -1, source);
	SetItemSyncValue(DND_SYNC_ITEMSUBTYPE, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMIMAGE, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMLEVEL, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMSTACK, item_index, -1, 0, source);
	temp = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	for(j = 0; j < temp; ++j) {
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, item_index, i, 0, source);
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, item_index, i, 0, source);
	}
	SetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, 0, source);
	for(j = 0; j < htemp; ++j)
		for(i = 0; i < wtemp; ++i) {
			SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, item_index + j * MAXINVENTORYBLOCKS_VERT + i, -1, 0, source);
			SetItemSyncValue(DND_SYNC_ITEMTYPE, item_index + j * MAXINVENTORYBLOCKS_VERT + i, -1, DND_ITEM_NULL, source);
		}
	SetItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, -1, 0, source);
	if(!dontSync)
		SyncItemData_Null(item_index, source, wtemp, htemp);
}

void FreeItem_Player(int item_index, int source, bool dontSync, int pnum) {
	int i, j;
	int temp;
	int pn = (pnum + 1) << 16;
	int wtemp = GetItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, pn, source);
	int htemp = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, pn, source);
	SetItemSyncValue(DND_SYNC_ITEMSUBTYPE, item_index, pn, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMIMAGE, item_index, pn, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMLEVEL, item_index, pn, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMSTACK, item_index, pn, 0, source);
	temp = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, pn, source);
	for(j = 0; j < temp; ++j) {
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, item_index, i | pn, 0, source);
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, item_index, i | pn, 0, source);
	}
	SetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, pn, 0, source);
	for(j = 0; j < htemp; ++j)
		for(i = 0; i < wtemp; ++i) {
			SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, item_index + j * MAXINVENTORYBLOCKS_VERT + i, pn, 0, source);
			SetItemSyncValue(DND_SYNC_ITEMTYPE, item_index + j * MAXINVENTORYBLOCKS_VERT + i, pn, DND_ITEM_NULL, source);
		}
	SetItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, pn, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, pn, 0, source);
	if(!dontSync)
		SyncItemData_Null_Player(item_index, source, wtemp, htemp, pnum);
}

void FreeSpot(int item_index, int source) {
	int j, temp;
	SetItemSyncValue(DND_SYNC_ITEMSUBTYPE, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMIMAGE, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMLEVEL, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMSTACK, item_index, -1, 0, source);
	
	temp = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	for(j = 0; j < temp; ++j) {
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, item_index, j, 0, source);
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, item_index, j, 0, source);
	}
	SetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMTYPE, item_index, -1, DND_ITEM_NULL, source);
	SetItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, -1, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, -1, 0, source);
}

void FreeSpot_Player(int item_index, int source, int pnum) {
	int j, temp;
	int pn = (pnum + 1) << 16;
	SetItemSyncValue(DND_SYNC_ITEMSUBTYPE, item_index, pn, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMIMAGE, item_index, pn, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMLEVEL, item_index, pn, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMSTACK, item_index, pn, 0, source);
	
	temp = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, pn, source);
	for(j = 0; j < temp; ++j) {
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, item_index, j | pn, 0, source);
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, item_index, j | pn, 0, source);
	}
	SetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, pn, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, item_index, pn, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMTYPE, item_index, pn, DND_ITEM_NULL, source);
	SetItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, pn, 0, source);
	SetItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, pn, 0, source);
}

int GetStackValue(int type) {
	switch (type) {
		case DND_ITEM_ORB:
		return MAXSTACKS_ORB;
		case DND_ITEM_CHESTKEY:
		return MAXSTACKS_CKEY;
		case DND_ITEM_ELIXIR:
		return MAXSTACKS_ELIXIR;
	}
	return 0;
}

// note to self: height is => horizontal, moving heights => x * MAXINVENTORYBLOCKS_VERT, width is vertical, just + x
int GetFreeSpotForItem(int item_index, int player_index, int source) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	int w = GetItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, (player_index + 1) << 16, source);
	int h = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, (player_index + 1) << 16, source);
	bool unfit = false;

	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			for(hcheck = 0; !unfit && hcheck < h && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < w && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if(PlayerInventoryList[player_index][bid].item_type != DND_ITEM_NULL || bid >= MAX_INVENTORY_BOXES)
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

int GetFreeSpotForItemWithStack(int item_index, int player_index, int source) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	int w = GetItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, (player_index + 1) << 16, source);
	int h = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, (player_index + 1) << 16, source);
	bool unfit = false;
	
	// first search for any spot on our inventory for a stack item of this type
	int type = GetItemSyncValue(DND_SYNC_ITEMTYPE, item_index, (player_index + 1) << 16, source);
	int sub = GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, item_index, (player_index + 1) << 16, source);
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
			for(hcheck = 0; !unfit && hcheck < h && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < w && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if(PlayerInventoryList[player_index][bid].item_type != DND_ITEM_NULL || bid >= MAX_INVENTORY_BOXES)
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
					if(PlayerInventoryList[player_index][bid].item_type != DND_ITEM_NULL || bid >= MAX_INVENTORY_BOXES)
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
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	int w = GetItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, (source_player + 1) << 16, source);
	int h = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, (source_player + 1) << 16, source);
	bool unfit = false;

	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			for(hcheck = 0; !unfit && hcheck < h && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < w && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if(PlayerInventoryList[player_index][bid].item_type != DND_ITEM_NULL || bid >= MAX_INVENTORY_BOXES) {
						unfit = true;
					}
				}
			}
			// we return top left corner box id
			if(wcheck == w && hcheck == h && !unfit)
				return j * MAXINVENTORYBLOCKS_VERT + i;
		}
	}
	return -1;
}

// this actually checks for free spots in the given source
int GetFreeSpotForItem_InPlace(int item_index, int player_index, int source) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	int w = GetItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, (player_index + 1) << 16, source);
	int h = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, (player_index + 1) << 16, source);
	bool unfit = false;

	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			for(hcheck = 0; !unfit && hcheck < h && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < w && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if(GetItemSyncValue(DND_SYNC_ITEMTYPE, bid, (player_index + 1) << 16, source) != DND_ITEM_NULL || bid >= MAX_INVENTORY_BOXES)
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
							PlayerInventoryList[pnum][pos + w + h * MAXINVENTORYBLOCKS_VERT].item_type = DND_ITEM_TEMPORARY;
							ItemMoveList[pnum][pos + w + h * MAXINVENTORYBLOCKS_VERT].state = true;
							ItemMoveList[pnum][pos + w + h * MAXINVENTORYBLOCKS_VERT].width = wcomp;
							ItemMoveList[pnum][pos + w + h * MAXINVENTORYBLOCKS_VERT].height = hcomp;
						}
				}
				else {
					// clean up whatever was used up until this point
					bid = i * MAXINVENTORYBLOCKS_VERT + j;
					for(pos = 0; pos < bid; ++pos) {
						PlayerInventoryList[pnum][pos].item_type = DND_ITEM_NULL;
						ItemMoveList[pnum][pos].state = false;
						ItemMoveList[pnum][pos].width = 0;
						ItemMoveList[pnum][pos].height = 0;
					}
					return false;
				}
			}
		}
	}
	
	// unmark all marked spots as unoccupied (real marking happens when we move items)
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(ItemMoveList[pnum][i].state) {
			// unmark as occupied
			wcomp = ItemMoveList[pnum][i].width;
			hcomp = ItemMoveList[pnum][i].height;
			for(h = 0; h < hcomp; ++h)
				for(w = 0; w < wcomp; ++w) {
					PlayerInventoryList[pnum][i + w + h * MAXINVENTORYBLOCKS_VERT].item_type = DND_ITEM_NULL;
					ItemMoveList[pnum][i + w + h * MAXINVENTORYBLOCKS_VERT].state = false;
					ItemMoveList[pnum][i + w + h * MAXINVENTORYBLOCKS_VERT].width = 0;
					ItemMoveList[pnum][i + w + h * MAXINVENTORYBLOCKS_VERT].height = 0;
				}
		}
	}
	
	return true;
}

// based on average player level
int RollItemLevel() {
	int res = 0;
	// return average player level, +- some value
	int pavg = PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] / PlayerInformationInLevel[PLAYERLEVELINFO_COUNTATSTART];
	if(pavg > 2 * ITEMLEVEL_VARIANCE_LOWER) {
		res = pavg + random(-ITEMLEVEL_VARIANCE_LOWER, ITEMLEVEL_VARIANCE_HIGHER);
		if(res > MAX_ITEM_LEVEL)
			res = MAX_ITEM_LEVEL;
		return res;
	}
	return pavg + random((-pavg + 1) / 2, ITEMLEVEL_VARIANCE_HIGHER);
}

int CheckItemAttribute(int item_pos, int attrib_index, int source, int count) {
	int i;
	for(i = 0; i < count; ++i)
		if(GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, item_pos, i, source) == attrib_index)
			return i;
	return -1;
}

// find the item that has a min, if basis isn't -1 then we must exclude this from inclusion to min
int FindMinOnUsedCharmsForAttribute(int pnum, int attrib_index, int basis) {
	int res = -1, temp, compare = INT_MAX;
	for(int i = 0; i < MAX_CHARMS_EQUIPPABLE; ++i) {
		if(i == basis)
			continue;
		if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL) {
			temp = CheckItemAttribute(i, attrib_index, DND_SYNC_ITEMSOURCE_CHARMUSED, Charms_Used[pnum][i].attrib_count);
			// means this exists
			if(temp != -1) {
				if(Charms_Used[pnum][i].attributes[temp].attrib_val < compare) {
					compare = Charms_Used[pnum][i].attributes[temp].attrib_val;
					SetInventory("DamagePerFlatHPBuffer", compare);
					res = i;
				}
			}
		}
	}
	return res;
}

void CopyItem(bool fieldToPlayer, int fieldpos, int player_index, int item_index) {
	int i, j, k, wtemp, htemp;
	if(!fieldToPlayer) {
		wtemp = PlayerInventoryList[player_index][item_index].topleftboxid - 1;
		Inventories_On_Field[fieldpos].width = PlayerInventoryList[player_index][wtemp].width;
		Inventories_On_Field[fieldpos].height = PlayerInventoryList[player_index][wtemp].height;
		Inventories_On_Field[fieldpos].item_type = PlayerInventoryList[player_index][wtemp].item_type;
		Inventories_On_Field[fieldpos].item_subtype = PlayerInventoryList[player_index][wtemp].item_subtype;
		Inventories_On_Field[fieldpos].item_image = PlayerInventoryList[player_index][wtemp].item_image;
		Inventories_On_Field[fieldpos].item_level = PlayerInventoryList[player_index][wtemp].item_level;
		Inventories_On_Field[fieldpos].item_stack = PlayerInventoryList[player_index][wtemp].item_stack;
		Inventories_On_Field[fieldpos].attrib_count = PlayerInventoryList[player_index][wtemp].attrib_count;
		Inventories_On_Field[fieldpos].topleftboxid = 0;
		for(i = 0; i < Inventories_On_Field[fieldpos].attrib_count; ++i) {
			Inventories_On_Field[fieldpos].attributes[i].attrib_id = PlayerInventoryList[player_index][wtemp].attributes[i].attrib_id;
			Inventories_On_Field[fieldpos].attributes[i].attrib_val = PlayerInventoryList[player_index][wtemp].attributes[i].attrib_val;
		}

		// the leftover spot is a null charm
		FreeItem(wtemp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	}
	else {
		// handle the box management
		// is this a stack item and does it already contain an item of this type?
		// ex type = orb, subtype = enhancement
		if(
			Inventories_On_Field[fieldpos].item_stack && 
			Inventories_On_Field[fieldpos].item_type == PlayerInventoryList[player_index][item_index].item_type && 
			Inventories_On_Field[fieldpos].item_subtype == PlayerInventoryList[player_index][item_index].item_subtype &&
			PlayerInventoryList[player_index][item_index].item_stack < GetStackValue(Inventories_On_Field[fieldpos].item_type)
		) {
			// just add to the stack
			++PlayerInventoryList[player_index][item_index].item_stack;
		}
		else {
			// no?
			wtemp = Inventories_On_Field[fieldpos].width;
			htemp = Inventories_On_Field[fieldpos].height;
			PlayerInventoryList[player_index][item_index].width = wtemp;
			PlayerInventoryList[player_index][item_index].height = htemp;
			PlayerInventoryList[player_index][item_index].item_subtype = Inventories_On_Field[fieldpos].item_subtype;
			PlayerInventoryList[player_index][item_index].item_image = Inventories_On_Field[fieldpos].item_image;
			PlayerInventoryList[player_index][item_index].item_level = Inventories_On_Field[fieldpos].item_level;
			PlayerInventoryList[player_index][item_index].item_stack = Inventories_On_Field[fieldpos].item_stack;
			PlayerInventoryList[player_index][item_index].attrib_count = Inventories_On_Field[fieldpos].attrib_count;
			for(k = 0; k < PlayerInventoryList[player_index][item_index].attrib_count; ++k) {
				PlayerInventoryList[player_index][item_index].attributes[k].attrib_id = Inventories_On_Field[fieldpos].attributes[k].attrib_id;
				PlayerInventoryList[player_index][item_index].attributes[k].attrib_val = Inventories_On_Field[fieldpos].attributes[k].attrib_val;
			}
			for(i = 0; i < htemp; ++i)
				for(j = 0; j < wtemp; ++j) {
					PlayerInventoryList[player_index][item_index + i * MAXINVENTORYBLOCKS_VERT + j].item_type = Inventories_On_Field[fieldpos].item_type;
					PlayerInventoryList[player_index][item_index + i * MAXINVENTORYBLOCKS_VERT + j].topleftboxid = item_index + 1;
				}
		}
		// the leftover spot is a null item
		RemoveItemFromWorld(fieldpos);
		SyncItemData(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, wtemp, htemp);
	}
}

// clones an item on this player's inventory, if no spot is found it won't bother
int CloneItem(int item_index, int source, bool dontSync) {
	int temp;
	int c = GetFreeSpotForItem_InPlace(item_index, PlayerNumber(), source);
	if(c != -1) {
		int wtemp = GetItemSyncValue(DND_SYNC_ITEMWIDTH, item_index, -1, source);
		int htemp = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, item_index, -1, source);
		int i, j, k;
		SetItemSyncValue(DND_SYNC_ITEMWIDTH, c, -1, wtemp, source);
		SetItemSyncValue(DND_SYNC_ITEMHEIGHT, c, -1, htemp, source);
		SetItemSyncValue(DND_SYNC_ITEMSUBTYPE, c, -1, GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, item_index, -1, source), source);
		SetItemSyncValue(DND_SYNC_ITEMIMAGE, c, -1, GetItemSyncValue(DND_SYNC_ITEMIMAGE, item_index, -1, source), source);
		SetItemSyncValue(DND_SYNC_ITEMLEVEL, c, -1, GetItemSyncValue(DND_SYNC_ITEMLEVEL, item_index, -1, source), source);
		SetItemSyncValue(DND_SYNC_ITEMSTACK, c, -1, GetItemSyncValue(DND_SYNC_ITEMSTACK, item_index, -1, source), source);
		temp = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
		SetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, c, -1, temp, source);
		for(k = 0; k < temp; ++k) {
			SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, c, k, GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, item_index, k, source), source);
			SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, c, k, GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, item_index, k, source), source);
		}
		for(i = 0; i < htemp; ++i)
			for(j = 0; j < wtemp; ++j) {
				temp = c + i * MAXINVENTORYBLOCKS_VERT + j;
				SetItemSyncValue(DND_SYNC_ITEMTYPE, temp, -1, GetItemSyncValue(DND_SYNC_ITEMTYPE, item_index, -1, source), source);
				SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, temp, -1, c + 1, source);
			}
		if(!dontSync)
			SyncItemData(c, source, -1, -1);
		return c;
	}
	return -1;
}

// check if clicked spot is free for the item we want to put
bool IsFreeSpot(int itempos, int emptypos) {
	int pnum = PlayerNumber();
	int temp = PlayerInventoryList[pnum][itempos].topleftboxid - 1;
	int w = PlayerInventoryList[pnum][temp].width;
	int h = PlayerInventoryList[pnum][temp].height;
	int offset = temp - itempos;
	int bid = 0;
	
	for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
			bid = emptypos + offset + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0)
				return false;
			// if not empty and it's not us
			if(PlayerInventoryList[pnum][bid].topleftboxid && PlayerInventoryList[pnum][bid].topleftboxid - 1 != temp)
				return false;
		}
	}
	return true;
}

bool IsFreeSpot_Trade(int itempos, int emptypos, int itemsource, int emptysource) {
	int temp = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, itemsource) - 1;
	int w = GetItemSyncValue(DND_SYNC_ITEMWIDTH, temp, -1, itemsource);
	int h = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, temp, -1, itemsource);
	int offset = temp - itempos;
	int bid = 0;
	int tb;
	
	for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
			bid = emptypos + offset + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0)
				return false;
			// if not empty and it's not us
			tb = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, bid, -1, emptysource);
			if(tb && tb - 1 != temp)
				return false;
		}
	}
	return true;
}

bool IsSourceInventoryView(int source) {
	// just in case we receive this with page numbers in it
	source &= 0xFFFF;
	return source == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY || source == DND_SYNC_ITEMSOURCE_TRADEVIEW || source == DND_SYNC_ITEMSOURCE_STASH;
}

// assumes both spots have an item
// if ipos1 has enough space to acommodate ipos2
bool CanSwapItems(int ipos1, int ipos2, int offset1, int offset2, int source1, int source2) {
	int pnum = PlayerNumber();
	int i, j;
	int w1, w2, h1, h2;
	int bid = 0, tb1, tb2;
	// from ipos2 to ipos1
	tb1 = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, source1) - 1;
	tb2 = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, source2) - 1;
	w2 = GetItemSyncValue(DND_SYNC_ITEMWIDTH, tb2, -1, source2);
	h2 = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, tb2, -1, source2);
	for(i = 0; i < h2; ++i) {
		for(j = 0; j < w2; ++j) {
			bid = ipos1 + offset2 + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0)
				return false;
			if(GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source1) - 1 != tb1 && GetItemSyncValue(DND_SYNC_ITEMTYPE, bid, -1, source1))
				return false;
		}
	}
	// from ipos1 to ipos2
	w1 = GetItemSyncValue(DND_SYNC_ITEMWIDTH, tb1, -1, source1);
	h1 = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, tb1, -1, source1);
	for(i = 0; i < h1; ++i) {
		for(j = 0; j < w1; ++j) {
			bid = ipos2 + offset1 + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0)
				return false;
			if(GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source2) - 1 != tb2 && GetItemSyncValue(DND_SYNC_ITEMTYPE, bid, -1, source2) != DND_ITEM_NULL)
				return false;
		}
	}
	return true;
}

void SwapItemProperty(int ipos1, int ipos2, int property, int source1, int source2, int offset1, int offset2) {
	int pnum = PlayerNumber();
	int temp;
	int i;
	int ht, wt;
	
	int h1, h2, w1, w2;
	int itype1, itype2;
	
	int tb1, tb2;
	
	switch(property) {
		case DND_SYNC_ITEMTOPLEFTBOX:
			tb1 = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, source1) - 1;
			tb2 = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, source2) - 1;
			h1 = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, tb1, -1, source1);
			h2 = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, tb2, -1, source2);
			w1 = GetItemSyncValue(DND_SYNC_ITEMWIDTH, tb1, -1, source1);
			w2 = GetItemSyncValue(DND_SYNC_ITEMWIDTH, tb2, -1, source2);
			// reset pointers
			if(IsSourceInventoryView(source1)) {
				for(ht = 0; ht < h1; ++ht)
					for(wt = 0; wt < w1; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, tb1 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, 0, source1);
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, 0, source1);
			
			if(IsSourceInventoryView(source2)) {
				for(ht = 0; ht < h2; ++ht)
					for(wt = 0; wt < w2; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, tb2 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, 0, source2);
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, 0, source2);
			// handle the leftover pointers
			// update pointers for new scale
			// ipos + 1 because -1 of this is used as pointer
			if(IsSourceInventoryView(source1)) {
				for(ht = 0; ht < h2; ++ht) {
					for(wt = 0; wt < w2; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1 + offset2 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, ipos1 + offset2 + 1, source1);
				}
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, ipos1 + offset1 + 1, source1);
				
			if(IsSourceInventoryView(source2)) {
				for(ht = 0; ht < h1; ++ht) {
					for(wt = 0; wt < w1; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2 + offset1 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, ipos2 + offset1 + 1, source2);
				}
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, ipos2 + offset2 + 1, source2);
		break;
		case DND_SYNC_ITEMTYPE:
			tb1 = ipos1 + offset1;
			tb2 = ipos2 + offset2;
			h1 = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, tb1, -1, source1);
			h2 = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, tb2, -1, source2);
			w1 = GetItemSyncValue(DND_SYNC_ITEMWIDTH, tb1, -1, source1);
			w2 = GetItemSyncValue(DND_SYNC_ITEMWIDTH, tb2, -1, source2);
			itype1 = GetItemSyncValue(DND_SYNC_ITEMTYPE, tb1, -1, source1);
			itype2 = GetItemSyncValue(DND_SYNC_ITEMTYPE, tb2, -1, source2);
			// reset pointers
			if(IsSourceInventoryView(source1)) {
				for(ht = 0; ht < h1; ++ht)
					for(wt = 0; wt < w1; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTYPE, tb1 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, DND_ITEM_NULL, source1);
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos1, -1, DND_ITEM_NULL, source1);
			
			if(IsSourceInventoryView(source2)) {
				for(ht = 0; ht < h2; ++ht)
					for(wt = 0; wt < w2; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTYPE, tb2 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, DND_ITEM_NULL, source2);
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos2, -1, DND_ITEM_NULL, source2);
			
			// handle the leftover pointers
			// update pointers for new scale
			if(IsSourceInventoryView(source1)) {
				for(ht = 0; ht < h2; ++ht) {
					for(wt = 0; wt < w2; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos1 + offset2 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, itype2, source1);
				}
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos1, -1, itype2, source1);
				
			if(IsSourceInventoryView(source2)) {
				for(ht = 0; ht < h1; ++ht) {
					for(wt = 0; wt < w1; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos2 + offset1 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, itype1, source2);
				}
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos2, -1, itype1, source2);
		break;
		case DND_SYNC_ITEMWIDTH:
		case DND_SYNC_ITEMHEIGHT:
		case DND_SYNC_ITEMIMAGE:
		case DND_SYNC_ITEMSUBTYPE:
		case DND_SYNC_ITEMLEVEL:
		case DND_SYNC_ITEMSTACK:
		case DND_SYNC_ITEMSATTRIBCOUNT:
			tb1 = ipos1 + offset1;
			tb2 = ipos2 + offset2;
			temp = GetItemSyncValue(property, tb1, -1, source1);
			SetItemSyncValue(property, ipos1 + offset2, -1, GetItemSyncValue(property, tb2, -1, source2), source1);
			SetItemSyncValue(property, ipos2 + offset1, -1, temp, source2);
		break;
		case DND_SYNC_ITEMATTRIBUTES_ID:
		case DND_SYNC_ITEMATTRIBUTES_VAL:
			tb1 = ipos1 + offset1;
			tb2 = ipos2 + offset2;
			wt = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, tb1, -1, source1);
			ht = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, tb2, -1, source2);
			// if my attrib count now is higher, I need to take from whoever had the higher one before
			if(wt > ht) {
				for(i = 0; i < wt; ++i) {
					// ipos2 had more attributes, it covers a larger space, take them all
					temp = GetItemSyncValue(property, tb2, i, source2);
					SetItemSyncValue(property, ipos2 + offset1, i, GetItemSyncValue(property, tb1, i, source1), source2);
					SetItemSyncValue(property, ipos1 + offset2, i, temp, source1);
				}
			}
			else {
				for(i = 0; i < ht; ++i) {
					// ipos1 had more attributes, it covers a larger space, take them all
					temp = GetItemSyncValue(property, tb1, i, source1);
					SetItemSyncValue(property, ipos1 + offset2, i, GetItemSyncValue(property, tb2, i, source2), source1);
					SetItemSyncValue(property, ipos2 + offset1, i, temp, source2);
				}
			}
		break;
	}
}

// for swapping items -- assumes neither are null
void SwapItems(int ipos1, int ipos2, int source1, int source2, bool dontSync) {
	// check if there's enough space
	// find if there is some sort of offset we must take care of, this matters only if we are swapping in inventory
	int offset1 = 0, offset2 = 0;
	int i, j, h1p, h2p, w1p, w2p;
	if(IsSourceInventoryView(source1))
		offset1 = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, source1) - 1 - ipos1;
	if(IsSourceInventoryView(source2))
		offset2 = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, source2) - 1 - ipos2;
		
	// if both items are stack items and are of same types, add stack on top of the other
	h1p = GetItemSyncValue(DND_SYNC_ITEMSTACK, ipos1 + offset1, -1, source1);
	h2p = GetItemSyncValue(DND_SYNC_ITEMSTACK, ipos2 + offset2, -1, source2);
	w1p = GetItemSyncValue(DND_SYNC_ITEMTYPE, ipos1 + offset1, -1, source1);
	if(
		h1p && h2p &&
		w1p == GetItemSyncValue(DND_SYNC_ITEMTYPE, ipos2 + offset2, -1, source2) &&
		GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, ipos1 + offset1, -1, source1) == GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, ipos2 + offset2, -1, source2)
	)
	{
		w2p = GetStackValue(w1p);
		// add stack of ipos2 to ipos1
		if(h1p + h2p <= w2p) {
			SetItemSyncValue(DND_SYNC_ITEMSTACK, ipos1 + offset1, -1, h1p + h2p, source1);
			SyncItemStack(ipos1 + offset1, source1);
			// dispose of 2nd item
			FreeItem(ipos2 + offset2, source2, false);
		}
		else {
			// set stack of h1p to max, then set the stack of ipos2 to whatever is left
			SetItemSyncValue(DND_SYNC_ITEMSTACK, ipos2 + offset2, -1, h2p - w2p + h1p, source2);
			SetItemSyncValue(DND_SYNC_ITEMSTACK, ipos1 + offset1, -1, w2p, source1);
			SyncItemStack(ipos1 + offset1, source1);
			SyncItemStack(ipos2 + offset2, source2);
		}
	}
	else if(CanSwapItems(ipos1, ipos2, offset1, offset2, source1, source2)) {
		// we need difference in height and width to the topboxid of this item, then we can anchor it properly
		// divide by max_vert to get height
		h1p = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, ipos1 + offset1, -1, source1);
		h2p = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, ipos2 + offset2, -1, source2);
		w1p = GetItemSyncValue(DND_SYNC_ITEMWIDTH, ipos1 + offset1, -1, source1); 
		w2p = GetItemSyncValue(DND_SYNC_ITEMWIDTH, ipos2 + offset2, -1, source2);
		
		for(i = DND_SYNC_ITEMBEGIN; i <= DND_SYNC_ITEMEND; ++i)
			SwapItemProperty(ipos1, ipos2, i, source1, source2, offset1, offset2);
		
		// for large swaps, a big sync at the end is way more favorable
		if(!dontSync) {
			if(GetItemSyncValue(DND_SYNC_ITEMTYPE, ipos1, -1, source1) == DND_ITEM_NULL) {
				SyncItemData_Null(ipos1 + offset1, source1, w1p, h1p);
			}
			else {
				SyncItemPointers(ipos1 + offset1, source1, w1p, h1p);
				SyncItemData(ipos1 + offset2, source1, -1, -1);
			}
				
			if(GetItemSyncValue(DND_SYNC_ITEMTYPE, ipos2, -1, source2) == DND_ITEM_NULL) {
				SyncItemData_Null(ipos2 + offset2, source2, w2p, h2p);
			}
			else {
				SyncItemPointers(ipos2 + offset2, source2, w2p, h2p);
				SyncItemData(ipos2 + offset1, source2, -1, -1);
			}
		}
		
		// update orb item pointer
		if(Player_MostRecent_Orb[PlayerNumber()].p_tempwep == ipos1)
			Player_MostRecent_Orb[PlayerNumber()].p_tempwep = ipos2;
		else if(Player_MostRecent_Orb[PlayerNumber()].p_tempwep == ipos2)
			Player_MostRecent_Orb[PlayerNumber()].p_tempwep = ipos1;
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

void MoveItem(int itempos, int emptypos) {
	int pnum = PlayerNumber();
	int tb = PlayerInventoryList[pnum][itempos].topleftboxid - 1;
	int offset = tb - itempos;
	
	int i, j, bid;
	
	int w = PlayerInventoryList[pnum][tb].width;
	int h = PlayerInventoryList[pnum][tb].height;

	// these two mark box ids that have been modified, ie. need updates
	// if the boxes aren't in range of itempos, they will need to be nulled
	bool set1 = false;
	bool set2 = false;
	
	int temp = emptypos + offset;
	
	for(i = 0; i < h; ++i) 
		for(j = 0; j < w; ++j) {
			bid = temp + j + i * MAXINVENTORYBLOCKS_VERT;
			if(!InventoryBoxContainsPoint(tb + j + i * MAXINVENTORYBLOCKS_VERT, temp, w, h)) {
				if(tb + j + i * MAXINVENTORYBLOCKS_VERT < 32)
					set1 |= 1 << (tb + j + i * MAXINVENTORYBLOCKS_VERT);
				else
					set2 |= 1 << (tb + j + i * MAXINVENTORYBLOCKS_VERT);
			}
		}
	
	PlayerInventoryList[pnum][temp].width = w;
	PlayerInventoryList[pnum][temp].height = h;
	PlayerInventoryList[pnum][temp].item_subtype = PlayerInventoryList[pnum][tb].item_subtype;
	PlayerInventoryList[pnum][temp].item_image = PlayerInventoryList[pnum][tb].item_image;
	PlayerInventoryList[pnum][temp].item_level = PlayerInventoryList[pnum][tb].item_level;
	PlayerInventoryList[pnum][temp].item_stack = PlayerInventoryList[pnum][tb].item_stack;
	PlayerInventoryList[pnum][temp].attrib_count = PlayerInventoryList[pnum][tb].attrib_count;
	for(i = 0; i < PlayerInventoryList[pnum][temp].attrib_count; ++i) {
		PlayerInventoryList[pnum][temp].attributes[i].attrib_id = PlayerInventoryList[pnum][tb].attributes[i].attrib_id;
		PlayerInventoryList[pnum][temp].attributes[i].attrib_val = PlayerInventoryList[pnum][tb].attributes[i].attrib_val;
	}
	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			PlayerInventoryList[pnum][temp + i * MAXINVENTORYBLOCKS_VERT + j].item_type = PlayerInventoryList[pnum][tb].item_type;
			PlayerInventoryList[pnum][temp + i * MAXINVENTORYBLOCKS_VERT + j].topleftboxid = temp + 1;
		}

	if(set1 || set2) {
		for(i = 0; i < 32; ++i)
			if(IsSet(set1, i))
				FreeSpot(i, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			
		for(i = 0; i < MAX_INVENTORY_BOXES - 32; ++i)
			if(IsSet(set2, i))
				FreeSpot(i + 32, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		SyncItemPointers(tb, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, w, h);
	}
	else {
		// Simply null the leftover spot, no collision happened
		FreeItem(tb, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	}
	
	// update orb item pointer
	if(Player_MostRecent_Orb[pnum].p_tempwep == itempos)
		Player_MostRecent_Orb[pnum].p_tempwep = emptypos;
	
	SyncItemData(temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, -1, -1);
}

// this is made specifically for trade view, the one above is optimized for normal inventory
void MoveItemTrade(int itempos, int emptypos, int itemsource, int emptysource) {
	int tb = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, itemsource) - 1;
	int offset = tb - itempos;
	
	int i, j, bid;
	
	int w = GetItemSyncValue(DND_SYNC_ITEMWIDTH, tb, -1, itemsource);
	int h = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, tb, -1, itemsource);

	// these two mark box ids that have been modified, ie. need updates
	// if the boxes aren't in range of itempos, they will need to be nulled
	bool set1 = false;
	bool set2 = false;
	
	int temp = emptypos + offset;
	
	// only check for this if they have the same source (overlap can only happen if so)
	// this also fixes a corner case bug
	if(itemsource == emptysource) {
		for(i = 0; i < h; ++i) 
			for(j = 0; j < w; ++j) {
				bid = temp + j + i * MAXINVENTORYBLOCKS_VERT;
				if(!InventoryBoxContainsPoint(tb + j + i * MAXINVENTORYBLOCKS_VERT, temp, w, h)) {
					if(tb + j + i * MAXINVENTORYBLOCKS_VERT < 32)
						set1 |= 1 << (tb + j + i * MAXINVENTORYBLOCKS_VERT);
					else
						set2 |= 1 << (tb + j + i * MAXINVENTORYBLOCKS_VERT);
				}
			}
	}

	SetItemSyncValue(DND_SYNC_ITEMWIDTH, temp, -1, w, emptysource);
	SetItemSyncValue(DND_SYNC_ITEMHEIGHT, temp, -1, h, emptysource);
	SetItemSyncValue(DND_SYNC_ITEMSUBTYPE, temp, -1, GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(DND_SYNC_ITEMIMAGE, temp, -1, GetItemSyncValue(DND_SYNC_ITEMIMAGE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(DND_SYNC_ITEMLEVEL, temp, -1, GetItemSyncValue(DND_SYNC_ITEMLEVEL, tb, -1, itemsource), emptysource);
	SetItemSyncValue(DND_SYNC_ITEMSTACK, temp, -1, GetItemSyncValue(DND_SYNC_ITEMSTACK, tb, -1, itemsource), emptysource);
	SetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, tb, -1, itemsource), emptysource);
	bid = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, emptysource);
	for(i = 0; i < bid; ++i) {
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, temp, i, GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, tb, i, itemsource), emptysource);
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, temp, i, GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, tb, i, itemsource), emptysource);
	}
	bid = GetItemSyncValue(DND_SYNC_ITEMTYPE, tb, -1, itemsource);
	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			SetItemSyncValue(DND_SYNC_ITEMTYPE, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, bid, emptysource);
			SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, temp + 1, emptysource);
		}

	if(set1 || set2) {
		for(i = 0; i < 32; ++i)
			if(IsSet(set1, i))
				FreeSpot(i, itemsource);
			
		for(i = 0; i < MAX_INVENTORY_BOXES - 32; ++i)
			if(IsSet(set2, i))
				FreeSpot(i + 32, itemsource);
		SyncItemPointers(tb, itemsource, w, h);
	}
	else {
		// Simply null the leftover spot, no collision happened
		FreeItem(tb, itemsource, false);
	}
	
	// as soon as this item is offered for a trade it can't be edited
	if(Player_MostRecent_Orb[PlayerNumber()].p_tempwep == itempos && itemsource == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY)
		Player_MostRecent_Orb[PlayerNumber()].p_tempwep = 0;
	
	SyncItemData(temp, emptysource, -1, -1);
}

// this simply carries an item from another player's place to another, like moveitem but has player inputs
void CarryItemTo(int itempos, int emptypos, int itemsource, int emptysource, int p_item, int p_empty) {
	int tb = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, itempos, (p_item + 1) << 16, itemsource) - 1;
	int offset = tb - itempos;
	
	int i, j, bid;
	
	int w = GetItemSyncValue(DND_SYNC_ITEMWIDTH, tb, (p_item + 1) << 16, itemsource);
	int h = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, tb, (p_item + 1) << 16, itemsource);
	
	int temp = emptypos + offset;
	
	SetItemSyncValue(DND_SYNC_ITEMWIDTH, temp, (p_empty + 1) << 16, w, emptysource);
	SetItemSyncValue(DND_SYNC_ITEMHEIGHT, temp, (p_empty + 1) << 16, h, emptysource);
	SetItemSyncValue(DND_SYNC_ITEMSUBTYPE, temp, (p_empty + 1) << 16, GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, tb, (p_item + 1) << 16, itemsource), emptysource);
	SetItemSyncValue(DND_SYNC_ITEMIMAGE, temp, (p_empty + 1) << 16, GetItemSyncValue(DND_SYNC_ITEMIMAGE, tb, (p_item + 1) << 16, itemsource), emptysource);
	SetItemSyncValue(DND_SYNC_ITEMLEVEL, temp, (p_empty + 1) << 16, GetItemSyncValue(DND_SYNC_ITEMLEVEL, tb, (p_item + 1) << 16, itemsource), emptysource);
	SetItemSyncValue(DND_SYNC_ITEMSTACK, temp, (p_empty + 1) << 16, GetItemSyncValue(DND_SYNC_ITEMSTACK, tb, (p_item + 1) << 16, itemsource), emptysource);
	SetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, temp, (p_empty + 1) << 16, GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, tb, (p_item + 1) << 16, itemsource), emptysource);
	bid = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, temp, (p_empty + 1) << 16, emptysource);
	for(i = 0; i < bid; ++i) {
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, temp, i | ((p_empty + 1) << 16), GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, tb, i | ((p_item + 1) << 16), itemsource), emptysource);
		SetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, temp, i | ((p_empty + 1) << 16), GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, tb, i | ((p_item + 1) << 16), itemsource), emptysource);
	}
	bid = GetItemSyncValue(DND_SYNC_ITEMTYPE, tb, (p_item + 1) << 16, itemsource);
	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			SetItemSyncValue(DND_SYNC_ITEMTYPE, temp + i * MAXINVENTORYBLOCKS_VERT + j, (p_empty + 1) << 16, bid, emptysource);
			SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, temp + i * MAXINVENTORYBLOCKS_VERT + j, (p_empty + 1) << 16, temp + 1, emptysource);
		}

	FreeItem_Player(tb, itemsource, false, p_item);
	SyncItemData_Player(temp, emptysource, -1, -1, p_empty);
}

void TransferTradeItems(int from, int to) {
	int bid, pos;
	int i, j, h, w;
	// for every possible item in the trade list of this player, get free position
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			bid = j + i * MAXINVENTORYBLOCKS_VERT;
			// care about the items only once, so use topleftboxid == bid
			if(TradeViewList[from][bid].topleftboxid - 1 == bid) {
				pos = GetFreeSpotForItem_Trade(bid, from, to, DND_SYNC_ITEMSOURCE_TRADEVIEW);
				if(pos != -1) {
					CarryItemTo(bid, pos, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, from, to);
				}
			}
		}
	}
}

int GetInventoryInfoOffset(int itype) {
	switch(itype) {
		case DND_ITEM_CHESTKEY:
		return CHESTKEY_BEGIN;
		case DND_ITEM_ELIXIR:
		return ELIXIR_BEGIN;
		case DND_ITEM_ORB:
		return ORBS_BEGIN;
	}
	return 0;
}

// outside source implies this was called from the menu, not in the game
void DrawInventoryInfo_Field(int topboxid, int source, int bx, int by, bool isOutsideSource) {
	int pnum = PlayerNumber();
	int stack, itype = GetItemSyncValue(DND_SYNC_ITEMTYPE, topboxid, -1, source), offset = 0.0;
	DeleteTextRange(RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 14, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES);
	if(topboxid != -1 && itype != DND_ITEM_NULL) {
		SetHudSize(480, 320, 1);
		SetFont("LDTITINF");
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES, CR_WHITE, bx + ScreenResOffsets[2], by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		by += 10.0;
		// show item details
		SetFont(Item_Images[GetItemSyncValue(DND_SYNC_ITEMIMAGE, topboxid, -1, source)]);
		if(itype == DND_ITEM_ORB)
			offset = 6.0;
		else if(itype == DND_ITEM_CHESTKEY)
			offset = 3.0;
		else if(itype == DND_ITEM_ELIXIR)
			offset = 8.0;
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 1, CR_WHITE, bx + ScreenResOffsets[2], by + offset, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		stack = GetItemSyncValue(DND_SYNC_ITEMSTACK, topboxid, -1, source);
		if(stack) {
			SetFont("SMALLFONT");
			HudMessage(d:stack; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 14, CR_GREEN, bx + ScreenResOffsets[2] + 96.2, by + 6.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
		SetHudSize(720, 480, 1);
		bx *= 3; by *= 3;
		bx /= 2; by /= 2;
		bx &= 0xFFFF0000;
		by &= 0xFFFF0000;
		bx += 0.4;
		by += 48.1;
		offset = 0;
		if(isOutsideSource) {
			offset = -64;
			by -= 8.0;
		}
		if (GetAspectRatio() == ASPECT_4_3)
			SetHudClipRect(-96 + 72, 80 + offset, 264 + 72, 224, 264 + 72, 1);
		else if (GetAspectRatio() == ASPECT_16_10)
			SetHudClipRect(-96 + 45, 80 + offset, 264, 224, 264, 1);
		else
			SetHudClipRect(-96, 80 + offset, 264, 224, 264, 1);
		DrawInventoryText_Field(topboxid, source, bx, by, itype);
		SetHudClipRect(0, 0, 0, 0, 0);
	}
}

void DrawInventoryText_Field(int topboxid, int source, int bx, int by, int itype) {
	int i, j;
	int val, temp;
	SetFont("SMALLFONT");
	if(itype == DND_ITEM_CHARM) {
		HudMessage(s:Charm_Tiers[GetItemSyncValue(DND_SYNC_ITEMLEVEL, topboxid, -1, source) / CHARM_ATTRIBLEVEL_SEPERATOR], s: " ", s:Charm_TypeName[GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, topboxid, -1, source)], s:" Charm"; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 2, CR_WHITE, bx + ScreenResOffsets[3], by - 40.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);
		i = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, topboxid, -1, source);
		for(j = 0; j < i; ++j) {
			temp = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, topboxid, j, source);
			val = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, topboxid, j, source);
			HudMessage(s:ItemAttributeString(temp, val); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 3 - j, CR_WHITE, bx + ScreenResOffsets[3], by + 24.0 * j, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
	}
	else if(itype == DND_ITEM_ORB || itype == DND_ITEM_CHESTKEY || itype == DND_ITEM_ELIXIR) {
		temp = GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, topboxid, -1, source) + GetInventoryInfoOffset(itype);
		HudMessage(s:InventoryInfo[temp][SITEM_DESC]; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 3, CR_WHITE, bx + ScreenResOffsets[2], by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
	}
	else if(itype > UNIQUE_BEGIN) {
		temp = itype & 0xFFFF;
		itype >>= UNIQUE_BITS;
		--itype;
		// itype holds unique position, temp is the actual item type
		HudMessage(s:"\c[A1]", s:UniqueItemNames[itype]; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 2, CR_WHITE, bx + ScreenResOffsets[3], by - 40.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		HudMessage(s:"\c[D1]Unique ", s:Charm_TypeName[GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, topboxid, -1, source)], s:" Charm"; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 3, CR_WHITE, bx + ScreenResOffsets[3], by - 24.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		i = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, topboxid, -1, source);
		// itype will count the skipped properties (the helper attributes)
		itype = 0;
		for(j = 0; j < i; ++j) {
			temp = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, topboxid, j, source);
			val = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, topboxid, j, source);
			if(val > 0) {
				// dont show this, skip to next attribute's detail
				if(temp == INV_EX_CHANCE) {
					++j;
					++itype;
					HudMessage(s:ExoticAttributeString(GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, topboxid, j, source), val, GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, topboxid, j, source)); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 4 - (j - itype), CR_WHITE, bx + ScreenResOffsets[3], by + 24.0 * (j - itype), INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
				}
				else
					HudMessage(s:ExoticAttributeString(temp, val, 0); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 4 - (j - itype), CR_WHITE, bx + ScreenResOffsets[3], by + 24.0 * (j - itype), INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
			}
			else if(!val) {
				// unique item doesn't have numeric attribute to show
				HudMessage(s:ExoticAttributeString(temp, val, 0); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 4 - (j - itype), CR_WHITE, bx + ScreenResOffsets[3], by + 24.0 * (j - itype), INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
			}
			else
				HudMessage(s:"- ", s:ExoticAttributeString(temp, val, 0); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 4 -  (j - itype), CR_WHITE, bx + ScreenResOffsets[3], by + 24.0 * (j - itype), INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
	}
}

void CopyItemSource(int fieldpos, int player_index, int item_index, int source) {
	int i, j, k, wtemp, htemp;
	wtemp = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, source) - 1;
	Inventories_On_Field[fieldpos].width = GetItemSyncValue(DND_SYNC_ITEMWIDTH, wtemp, -1, source);
	Inventories_On_Field[fieldpos].height = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_type = GetItemSyncValue(DND_SYNC_ITEMTYPE, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_subtype = GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_image = GetItemSyncValue(DND_SYNC_ITEMIMAGE, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_level = GetItemSyncValue(DND_SYNC_ITEMLEVEL, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_stack = GetItemSyncValue(DND_SYNC_ITEMSTACK, wtemp, -1, source);
	Inventories_On_Field[fieldpos].attrib_count = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, wtemp, -1, source);
	Inventories_On_Field[fieldpos].topleftboxid = 0;
	for(i = 0; i < Inventories_On_Field[fieldpos].attrib_count; ++i) {
		Inventories_On_Field[fieldpos].attributes[i].attrib_id = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, wtemp, i, source);
		Inventories_On_Field[fieldpos].attributes[i].attrib_val = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, wtemp, i, source);
	}

	// the leftover spot is a null item
	FreeItem(wtemp, source, false);
}

void DropItemToField(int player_index, int pitem_index, bool forAll, int source) {
	int c = CreateItemSpot();
	if(c != -1) {
		int itype = GetItemSyncValue(DND_SYNC_ITEMTYPE, pitem_index, -1, source);
		int stype = GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, pitem_index, -1, source);
		// copy now
		CopyItemSource(c, player_index, pitem_index, source);
		SyncItemData(c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		str droptype = InventoryDropActors[DND_INVDROP_CHARM];
		if(itype > UNIQUE_BEGIN)
			droptype = InventoryDropActors[DND_INVDROP_UNIQUECHARM];
		else if(itype == DND_ITEM_ORB)
			droptype = InventoryInfo[stype + ORBS_BEGIN][SITEM_NAME];
		else if(itype == DND_ITEM_CHESTKEY)
			droptype = InventoryInfo[stype + CHESTKEY_BEGIN][SITEM_NAME];
		else if(itype == DND_ITEM_ELIXIR)
			droptype = InventoryInfo[stype + ELIXIR_BEGIN][SITEM_NAME];
		forAll ? SpawnDropFacing(droptype, 16.0, 16, 256, c) : SpawnDropFacing(droptype, 16.0, 16, player_index + 1, c);
	}
}

// move this from field to player's inventory
int HandleStackedPickup(int item_index) {
	// make sure this item actually gets placed on top of an item that has some stack, if any
	int porb_index = GetFreeSpotForItemWithStack(item_index, PlayerNumber(), DND_SYNC_ITEMSOURCE_FIELD);
	CopyItem(true, item_index, PlayerNumber(), porb_index);
	return porb_index;
}

int CheckPlayerInventoryList(int pnum, int itemtype, int subtype) {
	int i;
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i)
		if(PlayerInventoryList[pnum][i].item_type == itemtype && PlayerInventoryList[pnum][i].item_subtype == subtype)
			return i;
	return -1;
}

// can only use items in inventory
void UsePlayerItem(int pnum, int item_index) {
	if(IsUsableItem(PlayerInventoryList[pnum][item_index].item_type)) {
		GiveInventory("DnD_RefreshPane", 1);
		--PlayerInventoryList[pnum][item_index].item_stack;
		if(PlayerInventoryList[pnum][item_index].item_stack)
			SyncItemStack(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		else {
			FreeItem_Player(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false, pnum);
			// if this is freed, we might have to auto-adjust the player's page -- need exact equality here
			int mcount = CountCraftingMaterials();
			if(mcount == MAX_CRAFTING_MATERIALBOXES * CheckInventory("DnD_Crafting_MaterialPage"))
				TakeInventory("DnD_Crafting_MaterialPage", 1);
		}
	}
}

void UsePlayerItem_Count(int pnum, int item_index, int count) {
	PlayerInventoryList[pnum][item_index].item_stack = Clamp_Between(PlayerInventoryList[pnum][item_index].item_stack - count, 0, PlayerInventoryList[pnum][item_index].item_stack);
	if(PlayerInventoryList[pnum][item_index].item_stack)
		SyncItemStack(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	else {
		FreeItem_Player(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false, pnum);
	}
}

// uses items from stash (needed for certain functions)
void UsePlayerStashItem_Count(int pnum, int page, int item_index, int count) {
	PlayerStashList[pnum][page][item_index].item_stack = Clamp_Between(PlayerStashList[pnum][page][item_index].item_stack - count, 0, PlayerStashList[pnum][page][item_index].item_stack);
	if(PlayerStashList[pnum][page][item_index].item_stack)
		SyncItemStack(item_index, DND_SYNC_ITEMSOURCE_STASH | (page << 16));
	else {
		FreeItem_Player(item_index, DND_SYNC_ITEMSOURCE_STASH | (page << 16), false, pnum);
	}
}

// a safeguard pretty much for use player item
bool isUsableItem(int itype) {
	switch(itype) {
		case DND_ITEM_ORB:
		case DND_ITEM_CHESTKEY:
		case DND_ITEM_ELIXIR:
		return true;
	}
	return false;
}

// we only have orbs as crafting material atm
bool IsCraftingItem(int itype) {
	switch(itype) {
		case DND_ITEM_ORB:
		case DND_ITEM_ELIXIR:
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
		case DND_ITEM_GLOVE:
		case DND_ITEM_WEAPON:
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
		case DND_ITEM_GLOVE:
		return true;
	}
	return false;
}

// will count crafting materials the player has currently in their inventory
int CountCraftingMaterials() {
	int pnum = PlayerNumber();
	int res = 0;
	bool unique_orbs = 0;
	bool unique_elixirs = 0;
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(IsCraftingItem(PlayerInventoryList[pnum][i].item_type)) {
			if(PlayerInventoryList[pnum][i].item_type == DND_ITEM_ORB && !IsSet(unique_orbs, PlayerInventoryList[pnum][i].item_subtype)) {
				++res;
				unique_orbs = SetBit(unique_orbs, PlayerInventoryList[pnum][i].item_subtype);
			}
			else if(PlayerInventoryList[pnum][i].item_type == DND_ITEM_ELIXIR && !IsSet(unique_elixirs, PlayerInventoryList[pnum][i].item_subtype)) {
				++res;
				unique_elixirs = SetBit(unique_elixirs, PlayerInventoryList[pnum][i].item_subtype);
			}
		}
	}
	return res;
}

// problem is this: say 8 orbs 5 elixirs, first page draws 12, next page requires next current is 12
// but this counts per item type, and per item type count will never make it to 12...
// soln: just count everything regardless, level order in inventory
int GetNextUniqueCraftingMaterial(int current) {
	int pnum = PlayerNumber();
	int res = 0, i;
	bool unique_orbs = 0;
	bool unique_elixirs = 0;
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(IsCraftingItem(PlayerInventoryList[pnum][i].item_type)) {
			if(PlayerInventoryList[pnum][i].item_type == DND_ITEM_ORB && !IsSet(unique_orbs, PlayerInventoryList[pnum][i].item_subtype)) {
				++res;
				unique_orbs = SetBit(unique_orbs, PlayerInventoryList[pnum][i].item_subtype);
			}
			else if(PlayerInventoryList[pnum][i].item_type == DND_ITEM_ELIXIR && !IsSet(unique_elixirs, PlayerInventoryList[pnum][i].item_subtype)) {
				++res;
				unique_elixirs = SetBit(unique_elixirs, PlayerInventoryList[pnum][i].item_subtype);
			}
			// return the item's index
			if(res > current)
				return i;
		}
	}
	return -1;
}

int GetNextUniqueCraftableMaterial(int current) {
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
}

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
				case DND_ORB_SCULPTING:
				case DND_ORB_ELEVATION:
				return false;
			}
		break;
	}
	return true;
}

void ProcessItemFeature(int pnum, int item_index, int source, int aindex, bool remove) {
	int atype = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, item_index, aindex, source);
	int aval = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, item_index, aindex, source);
	int asubtype = GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, item_index, aindex, source);
	int i, temp;
	if(CheckInventory("StatbuffCounter_DoubleSmallCharm") && asubtype == DND_CHARM_SMALL) {
		aval *= CheckInventory("StatbuffCounter_DoubleSmallCharm");
		aval /= FACTOR_SMALLCHARM_RESOLUTION; // our scale to lower it down from integer mult
	}
	switch(atype) {
		// first cases with exceptions to our generic formula
		case INV_MAGAZINE_INCREASE:
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
			// add onto the base capacities, not current capacities
			for(i = 0; i < MAX_MAGAZINES; ++i)
				SetAmmoCapacity(WeaponMagazineList[i], (WeaponMagazineCaps[i] * (100 + GetPlayerAttributeValue(pnum, atype))) / 100);
		break;
		case INV_EXPLOSION_RADIUS:
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
			// accuracy is held in a 32bit integer (tested) so it adheres to the limits of it
			SetActorProperty(0, APROP_SCORE, GetPlayerAttributeValue(pnum, atype));
		break;
		/*case INV_EXPLOSIVE_RESIST:
			TakeInventory(StrParam(s:"ExplosionResist_", d:Clamp_Between(GetPlayerAttributeValue(pnum, atype), 1, MAX_EXPRESIST_VAL)), 1);
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
			if(GetPlayerAttributeValue(pnum, atype))
				GiveInventory(StrParam(s:"ExplosionResist_", d:Clamp_Between(GetPlayerAttributeValue(pnum, atype), 1, MAX_EXPRESIST_VAL)), 1);
		break;*/
		case INV_DMGREDUCE_ELEM:
			TakeInventory(StrParam(s:"ElementalResist_", d:Clamp_Between(GetPlayerAttributeValue(pnum, atype), 1, MAX_ELEMRESIST_VAL)), 1);
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
			if(GetPlayerAttributeValue(pnum, atype))
				GiveInventory(StrParam(s:"ElementalResist_", d:Clamp_Between(GetPlayerAttributeValue(pnum, atype), 1, MAX_ELEMRESIST_VAL)), 1);
		break;
		case INV_DMGREDUCE_PHYS:
			TakeInventory(StrParam(s:"PhysicalResist_", d:Clamp_Between(GetPlayerAttributeValue(pnum, atype), 1, MAX_PHYSRESIST_VAL)), 1);
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
			if(GetPlayerAttributeValue(pnum, atype))
				GiveInventory(StrParam(s:"PhysicalResist_", d:Clamp_Between(GetPlayerAttributeValue(pnum, atype), 1, MAX_PHYSRESIST_VAL)), 1);
		break;
		case INV_DMGREDUCE_REFL:
			TakeInventory(StrParam(s:"Reflect_Resist_", d:Clamp_Between(GetPlayerAttributeValue(pnum, atype), 1, MAX_REFLRESIST_VAL)), 1);
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
			if(GetPlayerAttributeValue(pnum, atype))
				GiveInventory(StrParam(s:"Reflect_Resist_", d:Clamp_Between(GetPlayerAttributeValue(pnum, atype), 1, MAX_REFLRESIST_VAL)), 1);
		break;
		case INV_EX_DMGINCREASE_TAKEN:
			TakeInventory(StrParam(s:"DamageTakenIncrease_", d:Clamp_Between(GetPlayerAttributeValue(pnum, atype), 1, MAX_WEAKEN_VAL)), 1);
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
			if(GetPlayerAttributeValue(pnum, atype))
				GiveInventory(StrParam(s:"DamageTakenIncrease_", d:Clamp_Between(GetPlayerAttributeValue(pnum, atype), 1, MAX_WEAKEN_VAL)), 1);
		break;
		case INV_ACCURACY_INCREASE:
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
			// accuracy is held in a 32bit integer (tested) so it adheres to the limits of it
			SetActorProperty(0, APROP_ACCURACY, Clamp_Between(GetPlayerAttributeValue(pnum, atype), 0, DND_ACCURACY_CAP));
		break;
		
		// exotic stuff -- reason most of these dont have syncs is that they arent meant to be shown in stat menu page, so no need for client to be aware
		case INV_EX_CHANCE:
		case INV_EX_FORSHOW_BURSTGETSPELLETBONUS:
			// by itself these do nothing
		break;
		case INV_EX_KNOCKBACK_IMMUNITY:
			i = UNIQUE_MAP_MACRO(atype);
			GiveOrTake("StatbuffCounter_KnockbackImmunity", 1, remove);
			temp = GetPlayerAttributeValue(pnum, i);
			if(CheckInventory("StatbuffCounter_KnockbackImmunity"))
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], SetBit(temp, DND_STATBUFF_KNOCKBACKIMMUNE));
			else
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], ClearBit(temp, DND_STATBUFF_KNOCKBACKIMMUNE));
		break;
		case INV_EX_FACTOR_SMALLCHARM:
			i = UNIQUE_MAP_MACRO(atype);
			temp = GetPlayerAttributeValue(pnum, i);
			if(!remove) {
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], SetBit(temp, DND_STATBUFF_DOUBLESMALLCHARM));
				// we now need to re-apply all other features of small charms we have equipped
				// first 4 are small charms
				for(i = 0; i < 4; ++i)
					if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL)
						RemoveItemFeatures(i, DND_SYNC_ITEMSOURCE_CHARMUSED);
				// now give the item and re-apply
				GiveInventory("StatbuffCounter_DoubleSmallCharm", aval);
				for(i = 0; i < 4; ++i)
					if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(i, DND_SYNC_ITEMSOURCE_CHARMUSED);
			}
			else if(CheckInventory("StatbuffCounter_DoubleSmallCharm")) {
				// just take the attribute off and remove features and reapply
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], ClearBit(temp, DND_STATBUFF_DOUBLESMALLCHARM));
				for(i = 0; i < 4; ++i)
					if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL)
						RemoveItemFeatures(i, DND_SYNC_ITEMSOURCE_CHARMUSED);
				SetInventory("StatbuffCounter_DoubleSmallCharm", 0);
				// reapply with this gone
				for(i = 0; i < 4; ++i)
					if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(i, DND_SYNC_ITEMSOURCE_CHARMUSED);
			}
		break;
		case INV_EX_ALLSTATS:
			for(i = INV_STAT_STRENGTH; i <= INV_STAT_INTELLECT; ++i)
				GiveOrTake(GetPlayerAttributeString(i), aval, remove);
		break;
		case INV_EX_CHANCE_HEALMISSINGONPAIN:
			// -1 of aindex is used to retrieve chance
			// i will hold the chance of this to happen
			i = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, item_index, aindex - 1, source);
			if(IsSet(CheckInventory("IATTR_StatusBuffs_1"), DND_STATBUFF_DOUBLESMALLCHARM) && asubtype == DND_CHARM_SMALL) {
				i *= CheckInventory("StatbuffCounter_DoubleSmallCharm");
				i /= FACTOR_SMALLCHARM_RESOLUTION; // our scale to lower it down from integer mult
			}
			temp = GetPlayerAttributeValue(pnum, atype);
			if(!remove)
				temp = ((temp & 0xFFFF) + aval) | (((temp >> 16) + i) << 16);
			else
				temp = ((temp & 0xFFFF) - aval) | (((temp >> 16) - i) << 16);
			SetInventory(GetPlayerAttributeValue(pnum, atype), aval);
		break;
		case INV_EX_ALWAYSCRIT_LIGHTNING:
			i = UNIQUE_MAP_MACRO(atype);
			GiveOrTake("StatbuffCounter_AlwaysCritLightning", 1, remove);
			temp = GetPlayerAttributeValue(pnum, i);
			if(CheckInventory("StatbuffCounter_AlwaysCritLightning"))
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], SetBit(temp, DND_STATBUFF_ALWAYSCRITLIGHTNING));
			else
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], ClearBit(temp, DND_STATBUFF_ALWAYSCRITLIGHTNING));
		break;
		case INV_EX_DOUBLE_HEALTHCAP:
			i = GetActorProperty(0, APROP_HEALTH) - GetSpawnHealth();
			GiveOrTake(GetPlayerAttributeString(INV_HPPERCENT_INCREASE), aval, remove);
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
		case INV_EX_FORBID_ARMOR:
			i = UNIQUE_MAP_MACRO(atype);
			GiveOrTake("StatbuffCounter_ForbidArmor", 1, remove);
			temp = GetPlayerAttributeValue(pnum, i);
			if(CheckInventory("StatbuffCounter_ForbidArmor")) {
				SetInventory("DnD_ArmorType", 0);
				SetInventory("Armor", 0);
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], SetBit(temp, DND_STATBUFF_FORBIDARMOR));
			}
			else
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], ClearBit(temp, DND_STATBUFF_FORBIDARMOR));
		break;
		case INV_EX_DAMAGEPER_FLATHEALTH:
			// first check all sources, see if they contain this and are lower than this source
			temp = FindMinOnUsedCharmsForAttribute(pnum, INV_EX_DAMAGEPER_FLATHEALTH, item_index);
			// we got a new min
			if(temp != -1) {
				// update to use the new min if our comparison is better
				if(!remove)
					SetInventory("IATTR_DamagePerFlatHP", Min(aval, CheckInventory("DamagePerFlatHPBuffer")));
				else
					SetInventory("IATTR_DamagePerFlatHP", CheckInventory("DamagePerFlatHPBuffer"));
			}
			else {
				// no new min was found
				if(remove)
					SetInventory("IATTR_DamagePerFlatHP", 0);
				else
					SetInventory("IATTR_DamagePerFlatHP", aval);
			}
		break;
		case INV_EX_BEHAVIOR_PELLETSFIRECIRCLE:
			i = UNIQUE_MAP_MACRO(atype);
			GiveOrTake("StatbuffCounter_PelletsInCircle", 1, remove);
			temp = GetPlayerAttributeValue(pnum, i);
			if(CheckInventory("StatbuffCounter_PelletsInCircle"))
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], SetBit(temp, DND_STATBUFF_PELLETSINCIRCLE));
			else
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], ClearBit(temp, DND_STATBUFF_PELLETSINCIRCLE));
		break;
		case INV_EX_DMGREDUCE_SHAREWITHPETS:
			i = UNIQUE_MAP_MACRO(atype);
			GiveOrTake("StatbuffCounter_PainSharedWithPets", 1, remove);
			temp = GetPlayerAttributeValue(pnum, i);
			if(CheckInventory("StatbuffCounter_PainSharedWithPets"))
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], SetBit(temp, DND_STATBUFF_PAINSHAREDWITHPETS));
			else
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], ClearBit(temp, DND_STATBUFF_PAINSHAREDWITHPETS));
		break;
		case INV_EX_FLATDMG_ALL:
			for(i = INV_FLATPHYS_DAMAGE; i <= INV_FLATELEM_DAMAGE; ++i)
				GiveOrTake(GetPlayerAttributeString(i), aval, remove);
		break;
		case INV_EX_SOULWEPS_FULLDAMAGE:
			i = UNIQUE_MAP_MACRO(atype);
			GiveOrTake("StatbuffCounter_SoulWepsDoFullDamage", 1, remove);
			temp = GetPlayerAttributeValue(pnum, i);
			if(CheckInventory("StatbuffCounter_SoulWepsDoFullDamage"))
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], SetBit(temp, DND_STATBUFF_SOULWEPSFULLDAMAGE));
			else
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], ClearBit(temp, DND_STATBUFF_SOULWEPSFULLDAMAGE));
		break;
		case INV_EX_ABILITY_RALLY:
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
			if(GetPlayerAttributeValue(pnum, atype))
				GiveInventory("CastRally", 1);
			else
				TakeInventory("CastRally", 1);
		break;
		case INV_EX_BEHAVIOR_SPELLSFULLDAMAGE:
			i = UNIQUE_MAP_MACRO(atype);
			GiveOrTake("StatbuffCounter_SpellsFullDamage", 1, remove);
			temp = GetPlayerAttributeValue(pnum, i);
			if(CheckInventory("StatbuffCounter_SpellsFullDamage"))
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], SetBit(temp, DND_STATBUFF_SPELLSDOFULLDAMAGE));
			else
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], ClearBit(temp, DND_STATBUFF_SPELLSDOFULLDAMAGE));
		break;
		case INV_EX_ABILITY_MONSTERSRIP:
			i = UNIQUE_MAP_MACRO(atype);
			GiveOrTake("StatbuffCounter_SlainMonstersRIP", 1, remove);
			temp = GetPlayerAttributeValue(pnum, i);
			if(CheckInventory("StatbuffCounter_SlainMonstersRIP"))
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], SetBit(temp, DND_STATBUFF_SLAINMONSTERSRIP));
			else
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], ClearBit(temp, DND_STATBUFF_SLAINMONSTERSRIP));
		break;
		case INV_HP_INCREASE:
		case INV_HPPERCENT_INCREASE:
		case INV_STAT_VITALITY:
			i = GetActorProperty(0, APROP_HEALTH) - GetSpawnHealth();
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
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
		case INV_ARMOR_INCREASE:
		case INV_ARMORPERCENT_INCREASE:
		case INV_STAT_BULKINESS:
				GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
			if(CheckInventory("Armor")) {
				i = CheckInventory("Armor") - GetArmorSpecificCap(ArmorBaseAmounts[CheckInventory("DnD_ArmorType") - 1]);
				if(remove) {
					temp = GetArmorSpecificCap(ArmorBaseAmounts[CheckInventory("DnD_ArmorType") - 1]);
					if(CheckInventory("Armor") > temp) {
						// set health to new cap, add the extra to player
						if(i > 0)
							SetInventory("Armor", temp + i);
						else
							SetInventory("Armor", temp);
					}
				}
			}
		break;
		case INV_STAT_STRENGTH:
			i = GetActorProperty(0, APROP_HEALTH) - GetSpawnHealth();
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
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
			if(CheckInventory("Armor")) {
				i = CheckInventory("Armor") - GetArmorSpecificCap(ArmorBaseAmounts[CheckInventory("DnD_ArmorType") - 1]);
				GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
				if(remove) {
					temp = GetArmorSpecificCap(ArmorBaseAmounts[CheckInventory("DnD_ArmorType") - 1]);
					if(CheckInventory("Armor") > temp) {
						// set health to new cap, add the extra to player
						if(i > 0)
							SetInventory("Armor", temp + i);
						else
							SetInventory("Armor", temp);
					}
				}
			}
		break;
		case INV_SPEED_INCREASE:
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
			SetActorProperty(0, APROP_SPEED, GetPlayerSpeed(PlayerNumber()));
		break;
		case INV_AMMOCAP_INCREASE:
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
			// make sure to update ammo caps
			SetAllAmmoCapacities();
		break;
		case INV_EX_ABILITY_LUCKYCRIT:
			i = UNIQUE_MAP_MACRO(atype);
			GiveOrTake("StatbuffCounter_LuckyCrit", 1, remove);
			temp = GetPlayerAttributeValue(pnum, i);
			if(CheckInventory("StatbuffCounter_LuckyCrit"))
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], SetBit(temp, DND_STATBUFF_SLAINMONSTERSRIP));
			else
				SetInventory(Inv_Attribute_Names[i][INVATTR_CHECKER], ClearBit(temp, DND_STATBUFF_SLAINMONSTERSRIP));
		break;
		
		// anything that fits our generic formula
		default:
			GiveOrTake(GetPlayerAttributeString(atype), aval, remove);
		break;
	}
}

// Applies item stats to player
void ApplyItemFeatures(int item_index, int source) {
	int ac = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	int pnum = PlayerNumber();
	for(int i = 0; i < ac; ++i)
		ProcessItemFeature(pnum, item_index, source, i, false);
}

// Removes an applied list of item stats from player
void RemoveItemFeatures(int item_index, int source) {
	int ac = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	int pnum = PlayerNumber();
	for(int i = 0; i < ac; ++i)
		ProcessItemFeature(pnum, item_index, source, i, true);
}

int GetCraftableItemCount() {
	int res = 0, pnum = PlayerNumber();
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i)
		if(IsCraftableItem(PlayerInventoryList[pnum][i].item_type) && PlayerInventoryList[pnum][i].height)
			++res;
	return res;
}

// this doesn't consider the item_type yet!
void MakeUnique(int item_pos, int item_type) {
	int i;
	if(GetCVar("dnd_ignore_dropweights"))
		i = random(0, MAX_UNIQUE_ITEMS - 1);
	else {
		int roll = random(1, MAX_UNIQUE_WEIGHT);
		for(i = 0; i < MAX_UNIQUE_ITEMS && roll > UniqueItemDropWeight[i]; ++i);
	}
	#ifdef ISDEBUGBUILD
	i = random(UITEM_PAINMASTER, UITEM_VOIDEMBLEM);
	#endif
	// i is the unique id
	ConstructUniqueOnField(item_pos, i, item_type);
}

// this too
void ConstructUniqueOnField(int fieldpos, int unique_id, int item_type) {
	Inventories_On_Field[fieldpos].width = UniqueItemList[unique_id].width;
	Inventories_On_Field[fieldpos].height = UniqueItemList[unique_id].height;
	Inventories_On_Field[fieldpos].item_type = UniqueItemList[unique_id].item_type;
	Inventories_On_Field[fieldpos].item_subtype = UniqueItemList[unique_id].item_subtype;
	Inventories_On_Field[fieldpos].item_image = UniqueItemList[unique_id].item_image;
	Inventories_On_Field[fieldpos].item_level = UniqueItemList[unique_id].item_level;
	Inventories_On_Field[fieldpos].item_stack = UniqueItemList[unique_id].item_stack;
	Inventories_On_Field[fieldpos].attrib_count = UniqueItemList[unique_id].attrib_count;
	Inventories_On_Field[fieldpos].topleftboxid = 0;
	for(int i = 0; i < Inventories_On_Field[fieldpos].attrib_count; ++i) {
		Inventories_On_Field[fieldpos].attributes[i].attrib_id = UniqueItemList[unique_id].attrib_id_list[i];
		// we must roll the value once dropped
		Inventories_On_Field[fieldpos].attributes[i].attrib_val = random(UniqueItemList[unique_id].rolls[i].attrib_low, UniqueItemList[unique_id].rolls[i].attrib_high);
	}
}

void ResetPlayerInventory(int pnum) {
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(PlayerInventoryList[pnum][i].topleftboxid - 1 == i)
			SyncItemData_Null(i, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[pnum][i].width, PlayerInventoryList[pnum][i].height);
		PlayerInventoryList[pnum][i].item_type = DND_ITEM_NULL;
		PlayerInventoryList[pnum][i].width = 0;
		PlayerInventoryList[pnum][i].height = 0;
		PlayerInventoryList[pnum][i].item_image = 0;
		PlayerInventoryList[pnum][i].item_type = DND_ITEM_NULL;
		PlayerInventoryList[pnum][i].item_subtype = 0;
		PlayerInventoryList[pnum][i].item_level = 0;
		PlayerInventoryList[pnum][i].item_stack = 0;
		PlayerInventoryList[pnum][i].topleftboxid = 0;
		for(int j = 0; j < PlayerInventoryList[pnum][i].attrib_count; ++j) {
			PlayerInventoryList[pnum][i].attributes[j].attrib_id = 0;
			PlayerInventoryList[pnum][i].attributes[j].attrib_val = 0;
		}
		PlayerInventoryList[pnum][i].attrib_count = 0;
	}
}

void ResetTradeViewList(int pnum) {
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(TradeViewList[pnum][i].topleftboxid - 1 == i)
			SyncItemData_Null(i, DND_SYNC_ITEMSOURCE_TRADEVIEW, TradeViewList[pnum][i].width, TradeViewList[pnum][i].height);
		TradeViewList[pnum][i].item_type = DND_ITEM_NULL;
		TradeViewList[pnum][i].width = 0;
		TradeViewList[pnum][i].height = 0;
		TradeViewList[pnum][i].item_image = 0;
		TradeViewList[pnum][i].item_type = 0;
		TradeViewList[pnum][i].item_subtype = 0;
		TradeViewList[pnum][i].item_level = 0;
		TradeViewList[pnum][i].item_stack = 0;
		TradeViewList[pnum][i].topleftboxid = 0;
		for(int j = 0; j < TradeViewList[pnum][i].attrib_count; ++j) {
			TradeViewList[pnum][i].attributes[j].attrib_id = 0;
			TradeViewList[pnum][i].attributes[j].attrib_val = 0;
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
		for(int j = 0; j < Inventories_On_Field[i].attrib_count; ++j) {
			Inventories_On_Field[i].attributes[j].attrib_id = 0;
			Inventories_On_Field[i].attributes[j].attrib_val = 0;
		}
		Inventories_On_Field[i].attrib_count = 0;
	}
}

void ResetPlayerStash(int pnum) {
	for(int p = 0; p < MAX_EXTRA_INVENTORY_PAGES; ++p) {
		for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
			if(PlayerStashList[pnum][p][i].topleftboxid - 1 == i)
				SyncItemData_Null(i, DND_SYNC_ITEMSOURCE_STASH | (p << 16), PlayerStashList[pnum][p][i].width, PlayerStashList[pnum][p][i].height);
			PlayerStashList[pnum][p][i].item_type = DND_ITEM_NULL;
			PlayerStashList[pnum][p][i].width = 0;
			PlayerStashList[pnum][p][i].height = 0;
			PlayerStashList[pnum][p][i].item_image = 0;
			PlayerStashList[pnum][p][i].item_type = DND_ITEM_NULL;
			PlayerStashList[pnum][p][i].item_subtype = 0;
			PlayerStashList[pnum][p][i].item_level = 0;
			PlayerStashList[pnum][p][i].item_stack = 0;
			PlayerStashList[pnum][p][i].topleftboxid = 0;
			for(int j = 0; j < PlayerStashList[pnum][p][i].attrib_count; ++j) {
				PlayerStashList[pnum][p][i].attributes[j].attrib_id = 0;
				PlayerStashList[pnum][p][i].attributes[j].attrib_val = 0;
			}
			PlayerStashList[pnum][p][i].attrib_count = 0;
		}
	}
}

#endif
