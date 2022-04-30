#ifndef DND_INVENTORY_IN
#define DND_INVENTORY_IN

#include "DnD_InvInfo.h"
#include "DnD_UniqueItems.h"
#include "DnD_Hud.h"

#define MAX_ITEM_LEVEL 100
#define MAX_ITEM_AFFIXTIERS 4
#define ITEM_TIER_SEPERATOR (100 / MAX_ITEM_AFFIXTIERS)

#define DND_SYNC_ITEMBEGIN DND_SYNC_ITEMTOPLEFTBOX
#define DND_SYNC_ITEMEND DND_SYNC_ITEMATTRIBUTES_TIER

#define INVENTORY_INFO_ALPHA 0.666
#define INVENTORY_HOLDTIME 0.5
#define INVENTORY_FADETIME 0.5

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
#define MAX_CHARMS_EQUIPPABLE (MAX_SMALL_CHARMS_USED + MAX_MEDIUM_CHARMS_USED + MAX_LARGE_CHARMS_USED)

enum {
	IPROCESS_ADD,
	IPROCESS_REMOVE
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
	IIMG_ORB_18,
	IIMG_ORB_19,
	IIMG_ORB_20,
	
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
	IIMG_ELIX_LUCK,
	
	IIMG_TOKEN_REPAIR,
	IIMG_TOKEN_SCOUR
};

#define ITEM_IMAGE_ORB_BEGIN IIMG_ORB_1
#define ITEM_IMAGE_KEY_BEGIN IIMG_CKEY_1
#define ITEM_IMAGE_ELIXIR_BEGIN IIMG_ELIX_HEALTH
#define ITEM_IMAGE_TOKEN_BEGIN IIMG_TOKEN_REPAIR

#define MAX_ITEM_IMAGES (IIMG_TOKEN_SCOUR + 1)
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
	"ORB3Q0",
	"ORB3Z0",
	"ORB3S0",
	
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
	"ELIX09",
	
	"REPTOKN",
	"SCRTOKN"
};

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
global inventory_T 13: Inventories_On_Field[MAX_INVENTORIES_ON_FIELD];

global inventory_T 14: TradeViewList[MAXPLAYERS][MAX_INVENTORY_BOXES];
global inventory_T 15: PlayerStashList[MAXPLAYERS][MAX_EXTRA_INVENTORY_PAGES][MAX_INVENTORY_BOXES];

#define INVSOURCE_PLAYER PlayerInventoryList
#define INVSOURCE_CHARMUSED Charms_Used

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
	for(int k = 0; k < Inventories_On_Field[fieldpos].attrib_count; ++k) {
		Inventories_On_Field[fieldpos].attributes[k].attrib_id = 0;
		Inventories_On_Field[fieldpos].attributes[k].attrib_val = 0;
		Inventories_On_Field[fieldpos].attributes[k].attrib_tier = 0;
	}
	Inventories_On_Field[fieldpos].attrib_count = 0;
}

void FreeItem(int pnum, int item_index, int source, bool dontSync) {
	int i, j;
	int temp;
	int wtemp = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, item_index, -1, source);
	int htemp = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, item_index, -1, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, item_index, -1, 0, source);
	
	temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	for(j = 0; j < temp; ++j) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, j, 0, source);
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
	
	temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	for(j = 0; j < temp; ++j) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, j, 0, source);
	}
	SetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_index, -1, DND_ITEM_NULL, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, item_index, -1, 0, source);
}

int GetStackValue(int type) {
	switch (type) {
		case DND_ITEM_ORB:
		return MAXSTACKS_ORB;
		case DND_ITEM_CHESTKEY:
		return MAXSTACKS_CKEY;
		case DND_ITEM_ELIXIR:
		case DND_ITEM_TOKEN:
		return MAXSTACKS_ELIXIR;
	}
	return 0;
}

bool IsStackedItem(int type) {
	switch (type) {
		case DND_ITEM_ORB:
		case DND_ITEM_CHESTKEY:
		case DND_ITEM_ELIXIR:
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
int GetFreeSpotForItem(int item_index, int player_index, int source) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	int w = GetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, item_index, -1, source);
	int h = GetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, item_index, -1, source);
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
	int w = GetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, item_index, -1, source);
	int h = GetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, item_index, -1, source);
	bool unfit = false;
	
	// first search for any spot on our inventory for a stack item of this type
	int type = GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, item_index, -1, source);
	int sub = GetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, item_index, -1, source);
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
	int w = GetItemSyncValue(source_player, DND_SYNC_ITEMWIDTH, item_index, -1, source);
	int h = GetItemSyncValue(source_player, DND_SYNC_ITEMHEIGHT, item_index, -1, source);
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
	int w = GetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, item_index, -1, source);
	int h = GetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, item_index, -1, source);
	bool unfit = false;

	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			for(hcheck = 0; !unfit && hcheck < h && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < w && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if(GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, bid, -1, source) != DND_ITEM_NULL || bid >= MAX_INVENTORY_BOXES)
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

// find the item that has a min, if basis isn't -1 then we must exclude this from inclusion to min
int FindMinOnUsedCharmsForAttribute(int pnum, int attrib_index, int basis) {
	int res = -1, temp, compare = INT_MAX;
	for(int i = 0; i < MAX_CHARMS_EQUIPPABLE; ++i) {
		if(i == basis)
			continue;
		if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL) {
			temp = CheckItemAttribute(pnum, i, attrib_index, DND_SYNC_ITEMSOURCE_CHARMUSED, Charms_Used[pnum][i].attrib_count);
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

void CopyItemFromFieldToPlayer(int fieldpos, int player_index, int item_index) {
	int i, j, k, wtemp, htemp;
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
			PlayerInventoryList[player_index][item_index].attributes[k].attrib_tier = Inventories_On_Field[fieldpos].attributes[k].attrib_tier;
		}
		for(i = 0; i < htemp; ++i)
			for(j = 0; j < wtemp; ++j) {
				PlayerInventoryList[player_index][item_index + i * MAXINVENTORYBLOCKS_VERT + j].item_type = Inventories_On_Field[fieldpos].item_type;
				PlayerInventoryList[player_index][item_index + i * MAXINVENTORYBLOCKS_VERT + j].topleftboxid = item_index + 1;
			}
	}
	// the leftover spot is a null item
	FreeItem(-1, fieldpos, DND_SYNC_ITEMSOURCE_FIELD, false);
	SyncItemData(player_index, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, wtemp, htemp);
}

// clones an item on this player's inventory, if no spot is found it won't bother
int CloneItem(int pnum, int item_index, int source, bool dontSync) {
	int temp;
	int c = GetFreeSpotForItem_InPlace(item_index, pnum, source);
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
		temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, c, -1, temp, source);
		for(k = 0; k < temp; ++k) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, k, source), source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, k, source), source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, k, source), source);
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
bool IsFreeSpot(int pnum, int itempos, int emptypos) {
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

bool IsFreeSpot_Trade(int pnum, int itempos, int emptypos, int itemsource, int emptysource) {
	int temp = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, itemsource) - 1;
	int w = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, temp, -1, itemsource);
	int h = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, temp, -1, itemsource);
	int offset = temp - itempos;
	int bid = 0;
	int tb;
	
	for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
			bid = emptypos + offset + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0)
				return false;
			// if not empty and it's not us
			tb = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, emptysource);
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
			if(GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source1) - 1 != tb1 && GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, bid, -1, source1))
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
	TemporaryInventoryList[player_index][temp_pos].topleftboxid = 0;
	for(i = 0; i < TemporaryInventoryList[player_index][temp_pos].attrib_count; ++i) {
		TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_id = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_ID, item_index, i, source);
		TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_val = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, i, source);
		TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_tier = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, i, source);
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
	
	for(i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
			int bid = item_index + j + i * MAXINVENTORYBLOCKS_VERT;
			SetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, item_index + 1, source);
			SetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, bid, -1, TemporaryInventoryList[player_index][temp_pos].item_type, source);
		}
	}
	
	for(i = 0; i < TemporaryInventoryList[player_index][temp_pos].attrib_count; ++i) {
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_ID, item_index, i, TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_id, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, i, TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_val, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, i, TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_tier, source);
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
	// if the boxes aren't in range of itempos, they will need to be nulled
	int set1 = 0;
	int set2 = 0;
	
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
		PlayerInventoryList[pnum][temp].attributes[i].attrib_tier = PlayerInventoryList[pnum][tb].attributes[i].attrib_tier;
	}
	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			PlayerInventoryList[pnum][temp + i * MAXINVENTORYBLOCKS_VERT + j].item_type = PlayerInventoryList[pnum][tb].item_type;
			PlayerInventoryList[pnum][temp + i * MAXINVENTORYBLOCKS_VERT + j].topleftboxid = temp + 1;
		}

	if(set1 || set2) {
		for(i = 0; i < 32; ++i)
			if(IsSet(set1, i))
				FreeSpot(pnum, i, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
				
		for(i = 0; i < MAX_INVENTORY_BOXES - 32; ++i)
			if(IsSet(set2, i))
				FreeSpot(pnum, i + 32, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		SyncItemData_Null(pnum, tb, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, w, h);
	}
	else {
		// Simply null the leftover spot, no collision happened
		FreeItem(pnum, tb, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	}
	
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
				bid = temp + j + i * MAXINVENTORYBLOCKS_VERT;
				if(!InventoryBoxContainsPoint(tb + j + i * MAXINVENTORYBLOCKS_VERT, temp, w, h)) {
					if(tb + j + i * MAXINVENTORYBLOCKS_VERT < 32)
						set1 |= 1 << (tb + j + i * MAXINVENTORYBLOCKS_VERT);
					else
						set2 |= 1 << (tb + j + i * MAXINVENTORYBLOCKS_VERT);
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
	bid = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, emptysource);
	for(i = 0; i < bid; ++i) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, tb, i, itemsource), emptysource);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, tb, i, itemsource), emptysource);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, tb, i, itemsource), emptysource);
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
	
	SetItemSyncValue(p_empty, DND_SYNC_ITEMWIDTH, temp, -1, w, emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMHEIGHT, temp, -1, h, emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMSUBTYPE, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMSUBTYPE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMIMAGE, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMIMAGE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMLEVEL, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMLEVEL, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMSTACK, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMSTACK, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMSATTRIBCOUNT, tb, -1, itemsource), emptysource);
	bid = GetItemSyncValue(p_empty, DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, emptysource);
	for(i = 0; i < bid; ++i) {
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_ID, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_ID, tb, i, itemsource), emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_VAL, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_VAL, tb, i, itemsource), emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_TIER, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_TIER, tb, i, itemsource), emptysource);
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
		
		SetFont("LDTITINF");
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		// center text by making it 0.4 and 0.1 on y
		bx += GetIntegerBits(HUD_ITEMBAK_XF / 2) + 0.3;
		
		if(!isOutsideSource)
			by = by - 70.9;
		else
			by += 16.0;
		
		int isubt = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, topboxid, -1, source);
		
		// show item details
		SetFont(Item_Images[GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, topboxid, -1, source)]);
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 1, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
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

		SetHudClipRect(left + 15, (by >> 16), 4 * HUD_ITEMBAK_X / 3 + 9, 288, 4 * HUD_ITEMBAK_X / 3 + 9);
		DrawInventoryText(topboxid, source, pnum, bx, by, itype, isubt, HUD_DII_FIELD_MULT);
		SetHudClipRect(0, 0, 0, 0, 0);
	}
}

void DrawInventoryText(int topboxid, int source, int pnum, int bx, int by, int itype, int isubt, int id_mult) {
	int i, j;
	int val, temp, lvl;
	bool showModTiers = GetCVar("dnd_detailedmods");
	
	SetFont("SMALLFONT");
	if(itype == DND_ITEM_CHARM) {
		// temp holds charm's tier id
		lvl = GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, topboxid, -1, source);
		temp = lvl / CHARM_ATTRIBLEVEL_SEPERATOR;
		HudMessage(s:Charm_Strings[temp][CHARMSTR_COLORCODE], l:Charm_Strings[temp][CHARMSTR_TIERTAG], s: " ", l:GetCharmTypeName(isubt), s:" ", l:"DND_ITEM_CHARM"; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - id_mult * MAX_INVENTORY_BOXES - 2, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);
		
		val = GetStat(STAT_LVL) < lvl ? CR_RED : CR_WHITE;
		HudMessage(l:"DND_LEVEL_HEADER", s:": ", d:lvl; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - id_mult * MAX_INVENTORY_BOXES - 3, val, GetIntegerBits(bx - HUD_ITEMBAK_XF / 2 - 36.0) + 0.1, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);
		
		i = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, topboxid, -1, source);
		for(j = 0; j < i; ++j) {
			temp = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, topboxid, j, source);
			val = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, topboxid, j, source);
			lvl = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, topboxid, j, source);
			
			HudMessage(s:GetItemAttributeText(temp, val, 0, lvl, showModTiers); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - id_mult * MAX_INVENTORY_BOXES - 4 - j, CR_WHITE, bx, by + 16.0 + 24.0 * j, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
	}
	else if(IsStackedItem(itype)) {
		temp = isubt + GetInventoryInfoOffset(itype);
		HudMessage(s:"\c[Y5]", l:GetInventoryTag(temp), s:"\n\n", l:GetInventoryText(temp); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - id_mult * MAX_INVENTORY_BOXES - 3, CR_WHITE, bx, by + 40.0 * (source == DND_SYNC_ITEMSOURCE_FIELD), INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
	}
	else if(itype > UNIQUE_BEGIN) {
		temp = itype & 0xFFFF;
		itype >>= UNIQUE_BITS;
		--itype;
		
		lvl = GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, topboxid, -1, source);
		val = GetStat(STAT_LVL) < lvl ? CR_RED : CR_WHITE;
		HudMessage(l:"DND_LEVEL_HEADER", s:": ", d:lvl; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - id_mult * MAX_INVENTORY_BOXES - 4, val, GetIntegerBits(bx - HUD_ITEMBAK_XF / 2 - 36.0) + 0.1, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);
		
		// itype holds unique position, temp is the actual item type -- so does lvl
		lvl = itype;
		HudMessage(s:"\c[A1]", l:GetUniqueItemName(itype); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - id_mult * MAX_INVENTORY_BOXES - 2, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		HudMessage(s:"\c[D1]", l:"DND_ITEM_UNIQUE", s:" ", l:GetCharmTypeName(isubt), s:" ", l:"DND_ITEM_CHARM"; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - id_mult * MAX_INVENTORY_BOXES - 3, CR_WHITE, bx, by + 8.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		i = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, topboxid, -1, source);
		
		// itype will count the skipped properties (the helper attributes)
		itype = 0;
		for(j = 0; j < i; ++j) {
			temp = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, topboxid, j, source);
			val = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, topboxid, j, source);
			if(val > 0) {
				// dont show this, skip to next attribute's detail
				if(temp == INV_EX_CHANCE) {
					++j;
					++itype;
					HudMessage(
						s:GetItemAttributeText(GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, topboxid, j, source), val, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, topboxid, j, source), lvl, showModTiers, j);
						HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - id_mult * MAX_INVENTORY_BOXES - 5 - (j - itype), CR_WHITE,
						bx, by + 24.0 + 24.0 * (j - itype), INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
					);
				}
				else {
					HudMessage(
						s: GetItemAttributeText(temp, val, 0, lvl, showModTiers, j);
						HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - id_mult * MAX_INVENTORY_BOXES - 5 - (j - itype), CR_WHITE,
						bx, by + 24.0 + 24.0 * (j - itype), INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
					);
				}
			}
			else if(!val) {
				// unique item doesn't have numeric attribute to show
				HudMessage(
					s:GetItemAttributeText(temp, val, 0);
					HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - id_mult * MAX_INVENTORY_BOXES - 5 - (j - itype), CR_WHITE,
					bx, by + 24.0 + 24.0 * (j - itype), INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
				);
			}
			else {
				HudMessage(
					s:"- ", s:GetItemAttributeText(temp, val, 0, lvl, showModTiers, j);
					HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - id_mult * MAX_INVENTORY_BOXES - 5 -  (j - itype), CR_WHITE,
					bx, by + 24.0 + 24.0 * (j - itype), INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
				);
			}
		}
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
	Inventories_On_Field[fieldpos].topleftboxid = 0;
	for(i = 0; i < Inventories_On_Field[fieldpos].attrib_count; ++i) {
		Inventories_On_Field[fieldpos].attributes[i].attrib_id = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_ID, wtemp, i, source);
		Inventories_On_Field[fieldpos].attributes[i].attrib_val = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_VAL, wtemp, i, source);
		Inventories_On_Field[fieldpos].attributes[i].attrib_tier = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_TIER, wtemp, i, source);
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
		droptype = InventoryInfo[stype + ORBS_BEGIN];
	else if(itype == DND_ITEM_CHESTKEY)
		droptype = InventoryInfo[stype + CHESTKEY_BEGIN];
	else if(itype == DND_ITEM_ELIXIR)
		droptype = InventoryInfo[stype + ELIXIR_BEGIN];
	else if(itype == DND_ITEM_TOKEN)
		droptype = InventoryInfo[stype + TOKEN_BEGIN];
	forAll ? SpawnDropFacing(droptype, 16.0, 16, 256, c) : SpawnDropFacing(droptype, 16.0, 16, player_index + 1, c);
}

// move this from field to player's inventory
int HandleStackedPickup(int item_index) {
	// make sure this item actually gets placed on top of an item that has some stack, if any
	int porb_index = GetFreeSpotForItemWithStack(item_index, PlayerNumber(), DND_SYNC_ITEMSOURCE_FIELD);
	CopyItemFromFieldToPlayer(item_index, PlayerNumber(), porb_index);
	return porb_index;
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
bool UsePlayerItem(int pnum, int item_index) {
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
		int mcount = CountCraftingMaterials();
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
				case DND_ORB_SCULPTING:
				case DND_ORB_ELEVATION:
				case DND_ORB_HOLLOW:
				case DND_ORB_PHANTASMAL:
				return false;
			}
		break;
	}
	return true;
}

void ProcessItemFeature(int pnum, int item_index, int source, int aindex, bool remove, bool has_cybernetic) {
	int atype = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, aindex, source);
	int aval = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, aindex, source);
	int asubtype = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, item_index, aindex, source);
	int i, temp;
	int cap;
	
	str inv;
	
	// Well of power factor
	if(CheckInventory("StatbuffCounter_DoubleSmallCharm") && asubtype == DND_CHARM_SMALL) {
		aval *= CheckInventory("StatbuffCounter_DoubleSmallCharm");
		aval /= FACTOR_SMALLCHARM_RESOLUTION; // our scale to lower it down from integer mult
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
			IncPlayerModValue(pnum, atype, aval);
			// add onto the base capacities, not current capacities
			for(i = 0; i < MAX_MAGAZINES; ++i)
				SetAmmoCapacity(WeaponMagazineList[i], (WeaponMagazineCaps[i] * (100 + GetPlayerAttributeValue(pnum, atype))) / 100);
		break;
		case INV_EXPLOSION_RADIUS:
			IncPlayerModValue(pnum, atype, aval);
			// accuracy is held in a 32bit integer (tested) so it adheres to the limits of it
			SetActorProperty(0, APROP_SCORE, GetPlayerAttributeValue(pnum, atype));
		break;
		
		// these are all accuracy mod groups
		case INV_ACCURACY_INCREASE:
		case INV_ESS_OMNISIGHT:
		case INV_ESS_OMNISIGHT2:
			IncPlayerModValue(pnum, atype, aval);
			// accuracy is held in a 32bit integer (tested) so it adheres to the limits of it
			CalculatePlayerAccuracy(pnum);
		break;
		
		// exotic stuff -- reason most of these dont have syncs is that they arent meant to be shown in stat menu page, so no need for client to be aware
		case INV_EX_CHANCE:
		case INV_EX_FORSHOW_BURSTGETSPELLETBONUS:
			// by itself these do nothing
		break;
		case INV_EX_KNOCKBACK_IMMUNITY:
			IncPlayerModValue(pnum, atype, aval);
			UpdatePlayerKnockbackResist();
		break;
		case INV_EX_FACTOR_SMALLCHARM:
			if(!remove) {
				// we now need to re-apply all other features of small charms we have equipped
				// first 4 are small charms
				for(i = 0; i < 4; ++i)
					if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL)
						RemoveItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_CHARMUSED);
						
				// now give the item and re-apply
				IncPlayerModValue(pnum, atype, aval);
				
				for(i = 0; i < 4; ++i)
					if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_CHARMUSED);
			}
			else if(PlayerModValues[pnum][atype]) {
				// just take the attribute off and remove features and reapply
				for(i = 0; i < 4; ++i)
					if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL)
						RemoveItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_CHARMUSED);
						
				// little note: aval can be negative if we are removing, so just + is enough to subtract it
				IncPlayerModValue(pnum, atype, aval);
				
				// reapply with this gone
				for(i = 0; i < 4; ++i)
					if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_CHARMUSED);
			}
		break;
		case INV_EX_ALLSTATS:
			for(i = INV_STAT_STRENGTH; i <= INV_STAT_INTELLECT; ++i)
				IncPlayerModValue(pnum, i, aval);
			UpdatePlayerKnockbackResist();
			UpdateArmorVisual();
		break;
		case INV_EX_CHANCE_HEALMISSINGONPAIN:
			// -1 of aindex is used to retrieve chance
			// i will hold the chance of this to happen
			i = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, aindex - 1, source);
			temp = GetPlayerAttributeValue(pnum, INV_EX_FACTOR_SMALLCHARM);
			if(temp && asubtype == DND_CHARM_SMALL) {
				i *= temp;
				i /= FACTOR_SMALLCHARM_RESOLUTION; // our scale to lower it down from integer mult
			}
			temp = GetPlayerAttributeValue(pnum, atype);
			if(!remove)
				temp = ((temp & 0xFFFF) + aval) | (((temp >> 16) + i) << 16);
			else
				temp = ((temp & 0xFFFF) - aval) | (((temp >> 16) - i) << 16);
			SetPlayerModValue(pnum, atype, temp);
		break;
		case INV_EX_DOUBLE_HEALTHCAP:
			IncPlayerModValue(pnum, INV_HPPERCENT_INCREASE, aval);
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
					SetPlayerModValue(pnum, atype, Min(aval, PlayerModValues[pnum][atype]));
			}
			else {
				// no new min was found
				if(remove)
					SetPlayerModValue(pnum, atype, 0);
				else
					SetPlayerModValue(pnum, atype, aval);
			}
		break;
		case INV_EX_FLATDMG_ALL:
			for(i = INV_FLATPHYS_DAMAGE; i <= INV_FLATELEM_DAMAGE; ++i)
				IncPlayerModValue(pnum, i, aval);
		break;
		case INV_EX_ABILITY_RALLY:
			IncPlayerModValue(pnum, atype, aval);
			if(PlayerModValues[pnum][atype])
				GiveInventory("CastRally", 1);
			else
				TakeInventory("CastRally", 1);
		break;
		case INV_HP_INCREASE:
		case INV_HPPERCENT_INCREASE:
		case INV_STAT_VITALITY:
			i = GetActorProperty(0, APROP_HEALTH) - GetSpawnHealth();
			IncPlayerModValue(pnum, atype, aval);
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
			IncPlayerModValue(pnum, atype, aval);
			UpdatePlayerKnockbackResist();
			UpdateArmorVisual();
			
			i = GetArmorAmount();
			if(i) {
				cap = GetArmorCapFromID(GetArmorID());
				i = i - cap;
				if(remove) {
					temp = cap;
					if(GetArmorAmount() > temp) {
						// set health to new cap, add the extra to player
						if(i > 0)
							SetArmorAmount(temp + i);
						else
							SetArmorAmount(temp);
					}
				}
			}
		break;
		case INV_ARMOR_INCREASE:
		case INV_ARMORPERCENT_INCREASE:
			IncPlayerModValue(pnum, atype, aval);
			i = GetArmorAmount();
			if(i) {
				cap = GetArmorCapFromID(GetArmorID());
				i = i - cap;
				if(remove) {
					temp = cap;
					if(GetArmorAmount() > temp) {
						// set health to new cap, add the extra to player
						if(i > 0)
							SetArmorAmount(temp + i);
						else
							SetArmorAmount(temp);
					}
				}
			}
		break;
		case INV_SPEED_INCREASE:
			IncPlayerModValue(pnum, atype, aval);
			SetActorProperty(0, APROP_SPEED, GetPlayerSpeed(pnum));
		break;
		case INV_AMMOCAP_INCREASE:
			IncPlayerModValue(pnum, atype, aval);
			// make sure to update ammo caps
			SetAllAmmoCapacities();
		break;
		case INV_EX_CURSEIMMUNITY:
			IncPlayerModValue(pnum, atype, aval);
			if(PlayerModValues[pnum][atype])
				GiveInventory("CurseImmunity", 1);
			else
				HandleCurseImmunityRemoval();
		break;
		case INV_STAT_STRENGTH:
			IncPlayerModValue(pnum, atype, aval);
			UpdatePlayerKnockbackResist();
		break;
		
		// anything that fits our generic formula
		default:
			IncPlayerModValue(pnum, atype, aval);
		break;
	}
}

// Applies item stats to player
void ApplyItemFeatures(int pnum, int item_index, int source) {
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
	
	for(i = 0; i < ac; ++i)
		ProcessItemFeature(pnum, item_index, source, i, false, has_cybernetic);
}

// Removes an applied list of item stats from player
void RemoveItemFeatures(int pnum, int item_index, int source) {
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
	
	for(i = 0; i < ac; ++i)
		ProcessItemFeature(pnum, item_index, source, i, true, has_cybernetic);
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

void AddAttributeToItem(int pnum, int item_pos, int attrib, bool isWellRolled = false) {
	int temp = PlayerInventoryList[pnum][item_pos].attrib_count++;
	int lvl = PlayerInventoryList[pnum][item_pos].item_level / CHARM_ATTRIBLEVEL_SEPERATOR;
	
	// 10% chance to roll a tier up or down for the modifier on the charm
	lvl = GetItemTierRoll(lvl, isWellRolled);
	
	// force within bounds
	lvl = Clamp_Between(lvl, 0, MAX_CHARM_AFFIXTIERS);
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_tier = lvl;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_id = attrib;
	
	// roll the attribute
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_val = RollAttributeValue(attrib, lvl, isWellRolled);
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
	//i = UITEM_LIFELEECH;
	#endif
	// i is the unique id
	ConstructUniqueOnField(item_pos, i, item_type, pnum);
}

// this too
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
	for(int i = 0; i < Inventories_On_Field[fieldpos].attrib_count; ++i) {
		Inventories_On_Field[fieldpos].attributes[i].attrib_id = UniqueItemList[unique_id].attrib_id_list[i];
		
		// we must roll the value once dropped
		bool makeWellRolled = CheckWellRolled(pnum);
		if(!makeWellRolled)
			Inventories_On_Field[fieldpos].attributes[i].attrib_val = random(UniqueItemList[unique_id].rolls[i].attrib_low, UniqueItemList[unique_id].rolls[i].attrib_high);
		else
			Inventories_On_Field[fieldpos].attributes[i].attrib_val = random((UniqueItemList[unique_id].rolls[i].attrib_low + UniqueItemList[unique_id].rolls[i].attrib_high) / 2, UniqueItemList[unique_id].rolls[i].attrib_high);
	}
}

void ResetPlayerItemAttributes(int pnum, int itemid) {
	for(int j = 0; j < PlayerInventoryList[pnum][itemid].attrib_count; ++j) {
		PlayerInventoryList[pnum][itemid].attributes[j].attrib_id = 0;
		PlayerInventoryList[pnum][itemid].attributes[j].attrib_val = 0;
		PlayerInventoryList[pnum][itemid].attributes[j].attrib_tier = 0;
	}
	PlayerInventoryList[pnum][itemid].attrib_count = 0;
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
		for(int j = 0; j < PlayerInventoryList[pnum][i].attrib_count; ++j) {
			PlayerInventoryList[pnum][i].attributes[j].attrib_id = 0;
			PlayerInventoryList[pnum][i].attributes[j].attrib_val = 0;
			PlayerInventoryList[pnum][i].attributes[j].attrib_tier = 0;
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
		for(int j = 0; j < TradeViewList[pnum][i].attrib_count; ++j) {
			TradeViewList[pnum][i].attributes[j].attrib_id = 0;
			TradeViewList[pnum][i].attributes[j].attrib_val = 0;
			TradeViewList[pnum][i].attributes[j].attrib_tier = 0;
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
			Inventories_On_Field[i].attributes[j].attrib_tier = 0;
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
			for(int j = 0; j < PlayerStashList[pnum][p][i].attrib_count; ++j) {
				PlayerStashList[pnum][p][i].attributes[j].attrib_id = 0;
				PlayerStashList[pnum][p][i].attributes[j].attrib_val = 0;
				PlayerStashList[pnum][p][i].attributes[j].attrib_tier = 0;
			}
			PlayerStashList[pnum][p][i].attrib_count = 0;
		}
	}
}

void RollTokenInfo(int item_pos, int token_type, bool onField) {
	// roll random attributes for the charm
	Inventories_On_Field[item_pos].item_level = 1;
	Inventories_On_Field[item_pos].item_stack = 1; // stackables have default stack of 1
	Inventories_On_Field[item_pos].item_type = DND_ITEM_TOKEN;
	Inventories_On_Field[item_pos].item_subtype = token_type;
	Inventories_On_Field[item_pos].width = 1;
	Inventories_On_Field[item_pos].height = 1;
	Inventories_On_Field[item_pos].attrib_count = 0;
	Inventories_On_Field[item_pos].item_image = ITEM_IMAGE_TOKEN_BEGIN + token_type;
}

void SpawnToken(int pnum, bool sound, bool noRepeat = false) {
	int c = CreateItemSpot();
	if(c != -1) {
		// c is the index on the field now
		// only current token is repair token, so just assume that instead
		int i = 0, w = random(1, MAX_TOKEN_WEIGHT);
		
		for(; i < MAX_TOKENS && TokenWeights[i] < w; ++i);
		
		RollTokenInfo(c, i, true);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(InventoryInfo[i + TOKEN_BEGIN], 24.0, 16, pnum + 1, c);
		if(sound)
			ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_TOKEN);
		
		if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, STAT_LUCK) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
			SpawnToken(pnum, sound, true);
	}
}

void SpawnTokenForAll(int repeats) {
	for(int k = 0; k < repeats; ++k) {
		for(int j = 0; j < MAXPLAYERS; ++j) {
			SpawnToken(j, false);
		}
	}
}

#endif
