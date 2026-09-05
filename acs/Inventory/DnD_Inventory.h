#ifndef DND_INVENTORY_IN
#define DND_INVENTORY_IN

#define MAX_SMALL_CHARMS_USED 4
#define MAX_MEDIUM_CHARMS_USED 2
#define MAX_LARGE_CHARMS_USED 1
#define MAX_ARMORS_USED 3 // BOOT BODY HELM
#define MAX_POWERCORES_USED 1
// The STORAGE cap, not what a player can use. Slots past the base two are unlocked by research
// -- see GetPlayerFlaskSlots -- and the extra headroom is here so a later source can grant a
// fourth without moving anything. FLASK*_INDEX is the LAST group in the equipped slot enum, so
// growing this appends indices and shifts nothing above it.
#define MAX_FLASK_SLOTS 4
#define BASE_FLASK_SLOTS 2
#define MAX_ITEMS_EQUIPPABLE (MAX_SMALL_CHARMS_USED + MAX_MEDIUM_CHARMS_USED + MAX_LARGE_CHARMS_USED + MAX_ARMORS_USED + MAX_POWERCORES_USED + MAX_FLASK_SLOTS)

#define MAX_EXTRA_INVENTORY_PAGES 10
#define PAGEID_STASHTAB_ORBS MAX_EXTRA_INVENTORY_PAGES

// how long a sort waits for the shared scratch grid before taking it anyway
#define DND_SORT_MAXWAIT 35

#include "DnD_InvInfo.h"
#include "DnD_UniqueItems.h"
#include "../DnD_Hud.h"
#include "../DnD_WeaponDefs.h"

#define MAX_TEMP_INVENTORIES 4

enum {
	PTR_FREEITEMWORLD
};
#define MAX_POINTERS (PTR_FREEITEMWORLD + 1)
#define MAX_INVENTORIES_ON_FIELD 4096

typedef struct {
	int PointerIndexTable[MAX_POINTERS];

	inventory_T[]* TemporaryInventoryList[MAXPLAYERS]; //[MAXPLAYERS][MAX_TEMP_INVENTORIES];

	inventory_T[]* Items_Used[MAXPLAYERS]; //[MAXPLAYERS][MAX_ITEMS_EQUIPPABLE];

	inventory_T[]* Inventories_On_Field; //[MAX_INVENTORIES_ON_FIELD];

	inventory_T[]* PlayerInventoryList[MAXPLAYERS]; //[MAXPLAYERS][MAX_INVENTORY_BOXES];			// holds inventories of all players

	inventory_T[]* TradeViewList[MAXPLAYERS + 1]; //[MAXPLAYERS + 1][MAX_INVENTORY_BOXES]; 			// merchant's item list is on MAXPLAYERS index of this

	inventory_T[]* PlayerStashList[MAXPLAYERS][MAX_EXTRA_INVENTORY_PAGES + 1];//[MAX_INVENTORY_BOXES];

	// Staging grid for auto sort. One shared copy rather than one per player: a sort holds the busy
	// flag from start to finish, so two can never overlap, and per player would be
	// MAX_INVENTORY_BOXES * INVENTORY_T_INTS * MAXPLAYERS ints for something used a few times a map.
	inventory_T[]* SortScratchList; //[MAX_INVENTORY_BOXES];
} global_item_storage_T;

// holds indexes to items used that are on players like charms or armors
global global_item_storage_T 20: GlobalItemStorage;

// While an item sits in a player's trade view, this remembers the inventory box it was staked from,
// keyed by the item's top left box in the trade grid. Stored +1 so 0 reads as "no record".
//
// Handing it back to that exact box is only sound because the inventory cannot change shape while a
// trade is open: pickup is blocked with the menu up, the trade view refuses moves inside the
// inventory grid, and stakes never split or merge stacks. So a staked item's home box is still free
// and still the right shape when the trade falls through. Map scope on purpose -- a trade never
// outlives the map, and this has no business anywhere near global 20's layout.
int[] module& GetTradeItemOrigins(int pnum) {
	static int TradeItemOrigin[MAXPLAYERS][MAX_INVENTORY_BOXES];
	return TradeItemOrigin[pnum];
}

bool IsPlayerTradeGrid(int pnum, int source) {
	return source == DND_SYNC_ITEMSOURCE_TRADEVIEW && pnum >= 0 && pnum < MAXPLAYERS;
}

// -1 if there is no record
int GetTradeItemOrigin(int pnum, int tradebox) {
	auto origins = GetTradeItemOrigins(pnum);
	return origins[tradebox] - 1;
}

void SetTradeItemOrigin(int pnum, int tradebox, int invbox) {
	auto origins = GetTradeItemOrigins(pnum);
	origins[tradebox] = invbox + 1;
}

void ClearTradeItemOrigins(int pnum) {
	auto origins = GetTradeItemOrigins(pnum);
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i)
		origins[i] = 0;
}

#include "DnD_InventoryFuncs.h"

#define DND_ITEMMOD_ADD FALSE
#define DND_ITEMMOD_REMOVE TRUE

#define MAX_ITEM_LEVEL 100

#define INVENTORY_INFO_ALPHA 0.666
#define INVENTORY_HOLDTIME 0.5
#define INVENTORY_HOLDTIME_CRAFT (INVENTORY_HOLDTIME * 4)
#define INVENTORY_FADETIME 0.5

#define DND_BASE_DISASSEMBLE_COST 350
#define DND_DISASSEMBLE_LEVEL_PERCENT 5
#define DND_DISASSEMBLE_IMPLICIT_PERCENT 10
#define DND_DISASSEMBLE_TIER_PERCENT 10
#define DND_DISASSEMBLE_FRACTURE_PERCENT 10 // 10% per fracture
#define DND_BASE_DISASSEMBLE_CHANCE 40 // 40%
#define DND_BASE_DISASSEMBLE_CHANCE_PERLUCK 5 // 5%
#define DND_BASE_FRACTURE_DISASSEMBLE_CHANCE 3
#define DND_BASE_CORRUPT_DISASSEMBLE_CHANCE 5
#define DND_BASE_ILVL_YIELD 10
#define DND_BASE_AVGMOD_YIELD 30
#define DND_BASE_FRACTURE_YIELD 50
#define DND_BASE_CORRUPT_YIELD 100

#define MAX_POWERCORE_ATTRIB_DEFAULT 2

#define MAXSTACKS_ORB 128
#define MAXSTACKS_ORB_MORE 8192
#define MAXSTACKS_CKEY 256
#define MAXSTACKS_TOKEN 256

#define HUD_DII_FIELD_MULT 10

#define MAX_EXPRESIST_VAL 100
#define MAX_ELEMRESIST_VAL 100
#define MAX_PHYSRESIST_VAL 100
#define MAX_REFLRESIST_VAL 1000 // 1000 because we use 1 as 0.1% so 1000 is 100%
#define MAX_WEAKEN_VAL 200

#define DND_SYNERGYITEM_CHANCE 0.125

enum {
	SMALLCHARM_INDEX1,
	SMALLCHARM_INDEX2,
	SMALLCHARM_INDEX3,
	SMALLCHARM_INDEX4,
	MEDIUMCHARM_INDEX1,
	MEDIUMCHARM_INDEX2,
	LARGECHARM_INDEX,
	HELM_INDEX,
	BODY_ARMOR_INDEX,
	POWERCORE_INDEX,
	BOOT_INDEX,

	FLASK1_INDEX,
	FLASK2_INDEX,
	FLASK3_INDEX,
	FLASK4_INDEX,

	INV_ICON_INDEX // not an item just the icon index
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

imove_T[] module& GetItemMoveList(int pnum) {
	static imove_T ItemMoveList[MAXPLAYERS][MAX_INVENTORY_BOXES];
	return ItemMoveList[pnum];
}

// first bunch are orbs, the next are tokens
#define MAX_UNIQUE_CRAFTING_TYPES (DND_MAX_ORB_KINDS + DND_MAX_TOKEN_KINDS)
int UniqueCraftingItemList[MAX_UNIQUE_CRAFTING_TYPES];

void ResetUniqueCraftingItemList() {
	for(int i = 0; i < MAX_UNIQUE_CRAFTING_TYPES; ++i)
		UniqueCraftingItemList[i] = -1;
}

#include "DnD_InventoryImg.h"
#include "DnD_InventoryWeights.h"
#include "DnD_Armor.h"
#include "DnD_Flasks.h"
#include "DnD_SpecialtyItem.h"
#include "DnD_InvGeneric.h"

#define ITEMLEVEL_VARIANCE_LOWER 20
#define ITEMLEVEL_VARIANCE_HIGHER 7

// Creates an item on the game field
int CreateItemSpot() {
	//Naive but very fast: Will just replace old item if index goes back to it - have a bigger array to prevent the negative effect.
	//Just having a loop here creates an error so avoid looping at all costs.
	//Remember, the floor gets cleared on a new map, so most likely the older items are useless for the players anyways, except on 4k mob slaugher maps.
	// Note: This table for free items index must be zero'd at the end of every map... otherwise we will try to give index to something already on field next map...
	//Log(s:"curr index: ", d:GlobalItemStorage.PointerIndexTable[PTR_FREEITEMWORLD]);
	if ((++GlobalItemStorage.PointerIndexTable[PTR_FREEITEMWORLD]) >= MAX_INVENTORIES_ON_FIELD)
		GlobalItemStorage.PointerIndexTable[PTR_FREEITEMWORLD] = 0;
		
	// clear properties of this item before creating it -- fixes garbage data leftovers
	//Log(s:"clear id ", d:GlobalItemStorage.PointerIndexTable[PTR_FREEITEMWORLD]);
	RemoveItemFromWorld(GlobalItemStorage.PointerIndexTable[PTR_FREEITEMWORLD]);
	//Log(s:"use id ", d:PointerIndexTable[PTR_FREEITEMWORLD]);
	return GlobalItemStorage.PointerIndexTable[PTR_FREEITEMWORLD];
}

void RemoveItemFromWorld(int fieldpos) {
	ClearInventoryItem(GlobalItemStorage.Inventories_On_Field[fieldpos]);
}

// move this from field to player's inventory
int HandleInventoryPickup(int item_index) {
	int pnum = PlayerNumber();
	int pcharm_index = GetFreeSpotForItem(item_index, pnum, DND_SYNC_ITEMSOURCE_FIELD, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	CopyItemFromFieldToPlayer(item_index, pnum, pcharm_index);
	ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
	return pcharm_index;
}

int GetStackValue(int type, int source = 0) {
	switch (type) {
		case DND_ITEM_ORB:
		if((source >> 16) != PAGEID_STASHTAB_ORBS)
			return MAXSTACKS_ORB;
		return MAXSTACKS_ORB_MORE;
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

int GetMaxItemAffixes(int item_type, int item_subtype = -1) {
	int res = 0;

	if(item_type > UNIQUE_BEGIN)
		item_type &= 0xFFFF;

	switch(item_type) {
		case DND_ITEM_CHARM:
			if(item_subtype == DND_CHARM_SMALL)
				res = 2;
			else if(item_subtype == DND_CHARM_MEDIUM)
				res = 4;
			else
				res = 6;
		break;
		case DND_ITEM_BODYARMOR:
			res = MAX_ARMOR_ATTRIB_DEFAULT;
		break;
		case DND_ITEM_HELM:
			res = MAX_HELM_ATTRIB_DEFAULT;
		break;
		case DND_ITEM_BOOT:
			res = MAX_BOOT_ATTRIB_DEFAULT;
		break;
		case DND_ITEM_FLASK:
			res = MAX_FLASK_ATTRIB_DEFAULT;
		break;

		case DND_ITEM_DUNGEONKEY:
			res = MAX_DUNGEONKEY_ATTRIB_DEFAULT;
		break;


		case DND_ITEM_SPECIALTY_DOOMGUY:
		case DND_ITEM_SPECIALTY_MARINE:
		case DND_ITEM_SPECIALTY_HOBO:
		case DND_ITEM_SPECIALTY_PUNISHER:
		case DND_ITEM_SPECIALTY_WANDERER:
		case DND_ITEM_SPECIALTY_CYBORG:
		case DND_ITEM_SPECIALTY_BERSERKER:
		case DND_ITEM_SPECIALTY_TRICKSTER:
			res = MAX_POWERCORE_ATTRIB_DEFAULT;
		break;
	}
	return res;
}

int FindInventoryOfType(int player_index, int item_type, int item_subtype) {
	int i = 0, j = 0, bid = 0;
	
	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			bid = j * MAXINVENTORYBLOCKS_VERT + i;
			// notice we can return bid here, we don't check like below
			auto item = GetPlayerInventoryItem(player_index, bid);
			if(item.item_type == item_type && item.item_subtype == item_subtype)
				return bid;
		}
	}
	
	return -1;
}

// note to self: height is => horizontal, moving heights => x * MAXINVENTORYBLOCKS_VERT, width is vertical, just + x
// The scan half of GetFreeSpotForItem, split out so a caller that already knows the size can ask
// directly instead of needing the item to live in a source first. Auto sort needs that -- its items
// are parked in the scratch list while the grid is being repacked. One implementation on purpose:
// the sorter decides where things land with the very same placer that runs everywhere else.
int FindFreeSpotForSize(int player_index, int w, int h, int dest_source) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	bool unfit = false;

	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			bool rowStart = !(i % MAXINVENTORYBLOCKS_VERT);
			for(hcheck = 0; !unfit && hcheck < h && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < w && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if
					(
						bid >= MAX_INVENTORY_BOXES || 
						(!rowStart && !(bid % MAXINVENTORYBLOCKS_VERT)) ||
						GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, bid, -1, dest_source) != DND_ITEM_NULL
					)
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

// Left as its own copy of the scan rather than a wrapper over FindFreeSpotForSize. Every existing
// caller runs through here, so it stays byte for byte what it was before auto sort existed.
int GetFreeSpotForItem(int item_index, int player_index, int item_source, int dest_source, int source_player = -1, bool source_inv_except = false) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	int w, h;

	int temp = item_index;
	if(!source_inv_except && IsSourceInventoryView(item_source))
		temp = GetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, temp, -1, item_source) - 1;

	// extended check for potential player source change
	if(source_player == -1) {
		w = GetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, temp, -1, item_source);
		h = GetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, temp, -1, item_source);
	}
	else {
		w = GetItemSyncValue(source_player, DND_SYNC_ITEMWIDTH, temp, -1, item_source);
		h = GetItemSyncValue(source_player, DND_SYNC_ITEMHEIGHT, temp, -1, item_source);
	}

	//printbold(s:"comp with w and h: ", d:w, s: " ", d:h, s: " ", d:item_index, s: " ", d:temp, s: " source: ", d:source_player, s: " player_index: ", d:player_index);

	bool unfit = false;

	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			bool rowStart = !(i % MAXINVENTORYBLOCKS_VERT);
			for(hcheck = 0; !unfit && hcheck < h && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < w && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if
					(
						bid >= MAX_INVENTORY_BOXES ||
						(!rowStart && !(bid % MAXINVENTORYBLOCKS_VERT)) ||
						GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, bid, -1, dest_source) != DND_ITEM_NULL
					)
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
	int w, h, type, sub;
	bool unfit = false;
	
	// extended check for potential player source change
	if(source_player == -1) {
		w = GetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, item_index, -1, item_source);
		h = GetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, item_index, -1, item_source);
		type = GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, item_index, -1, item_source);
		sub = GetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, item_index, -1, item_source);
	}
	else {
		w = GetItemSyncValue(source_player, DND_SYNC_ITEMWIDTH, item_index, -1, item_source);
		h = GetItemSyncValue(source_player, DND_SYNC_ITEMHEIGHT, item_index, -1, item_source);
		type = GetItemSyncValue(source_player, DND_SYNC_ITEMTYPE, item_index, -1, item_source);
		sub = GetItemSyncValue(source_player, DND_SYNC_ITEMSUBTYPE, item_index, -1, item_source);
	}
	
	// first search for any spot on our inventory for a stack item of this type
	int maxstack = GetStackValue(type, dest_source);
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
	int maxstack = GetStackValue(type, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto item = GetPlayerInventoryItem(player_index, i);
		if(item.item_type == type && item.item_subtype == sub && item.item_stack < maxstack)
			return i;
	}
			
	// didn't work, find new spot
	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			for(hcheck = 0; !unfit && hcheck < 1 && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < 1 && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					item = GetPlayerInventoryItem(player_index, bid);
					if(bid >= MAX_INVENTORY_BOXES || item.item_type != DND_ITEM_NULL)
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

int GetFreeSpotForItem_Trade(int item_index, int source_player, int player_index, int source, bool source_inv_except = false) {
	if(!IsStackedItem(GetItemSyncValue(source_player, DND_SYNC_ITEMTYPE, item_index, -1, source)))
		return GetFreeSpotForItem(item_index, player_index, source, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, source_player, source_inv_except);
	return GetFreeSpotForItemWithStack(item_index, player_index, source, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, true, source_player);
}

// Check if we have enough space for all the items offered, in our inventory
bool ConfirmSpaceForOfferings(int pnum, int tradee) {
	int bid, pos;
	int i, j, h, w, hcomp, wcomp;

	auto item_move_list = GetItemMoveList(pnum);

	// for every possible item in the trade list of this player, get free position
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			bid = j + i * MAXINVENTORYBLOCKS_VERT;
			// care about the items only once, so use topleftboxid == bid
			auto item = GetTradeItem(tradee, bid);
			if(item.topleftboxid - 1 == bid) {
				pos = GetFreeSpotForItem_Trade(bid, tradee, pnum, DND_SYNC_ITEMSOURCE_TRADEVIEW, true);
				if(pos != -1) {
					// mark as occupied so getfreespot wont return them
					hcomp = item.height;
					wcomp = item.width;
					for(h = 0; h < hcomp; ++h)
						for(w = 0; w < wcomp; ++w) {
							// hack to avoid finding the same spot as empty
							auto p_item = GetPlayerInventoryItem(pnum, pos + w + h * MAXINVENTORYBLOCKS_VERT);
							if(p_item.item_type == DND_ITEM_NULL)
								p_item.item_type = DND_ITEM_TEMPORARY;
							item_move_list[bid + w + h * MAXINVENTORYBLOCKS_VERT].state = true;
							item_move_list[bid + w + h * MAXINVENTORYBLOCKS_VERT].width = wcomp;
							item_move_list[bid + w + h * MAXINVENTORYBLOCKS_VERT].height = hcomp;
						}
					//printbold(s:"send player ", d:pnum, s:"'s item at ", d:bid, s:" to ", d:pos);
					item_move_list[bid].dest_pos = pos;
				}
				else {
					// clean up whatever was used up, we had no space
					for(pos = 0; pos < MAX_INVENTORY_BOXES; ++pos) {
						if(item_move_list[pos].state) {
							// cleanup our hack
							p_item = GetPlayerInventoryItem(pnum, pos);
							if(p_item.item_type == DND_ITEM_TEMPORARY)
								p_item.item_type = DND_ITEM_NULL;
							item_move_list[pos].state = false;
							item_move_list[pos].width = 0;
							item_move_list[pos].height = 0;
							item_move_list[pos].dest_pos = -1;
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
		if(item_move_list[i].state) {
			// unmark as occupied
			wcomp = item_move_list[i].width;
			hcomp = item_move_list[i].height;
			
			// note that the itemmovelist contains "Item of Player in tradebox was to move to dest_pos in player inventory" info
			pos = item_move_list[i].dest_pos;
			for(h = 0; h < hcomp; ++h)
				for(w = 0; w < wcomp; ++w) {
					// cleanup our hack
					p_item = GetPlayerInventoryItem(pnum, pos + w + h * MAXINVENTORYBLOCKS_VERT);
					if(p_item.item_type == DND_ITEM_TEMPORARY)
						p_item.item_type = DND_ITEM_NULL;
				}
		}
	}
	
	return true;
}

bool IsWearingBodyArmor(int pnum) {
	return (GetUsedItem(pnum, BODY_ARMOR_INDEX).item_type & 0xFFFF) == DND_ITEM_BODYARMOR;
}

bool IsPlayerInventoryItemUnique(int pnum, int pos) {
	return GetPlayerInventoryItem(pnum, pos).item_type > UNIQUE_BEGIN;
}

int MakeItemUsed(int pnum, int use_id, int item_index, int item_type, int target_type) {
	int i, j;

	auto item = GetPlayerInventoryItem(pnum, item_index);

	// type mismatch, popup
	if(item_type == DND_ITEM_CHARM && target_type != item.item_subtype)
		return POPUP_CHARMMISMATCH;
		
	// too high level
	if(item.item_level > GetLevel())
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
		(item.item_type >> UNIQUE_BITS) - 1 == UITEM_WELLOFPOWER &&
		CountPlayerSmallCharms(pnum) > item.attributes[1].attrib_val
	)
		return POPUP_NOMORESMALLCHARMS;
	
	// or tried to put small charm when well of power is there and would exceed limit
	if(target_type == DND_CHARM_SMALL && (i = PlayerModData[pnum].vals[PSTAT_EX_LIMITEDSMALLCHARMS]) && i != MAX_SMALL_CHARMS_USED && i == CountPlayerSmallCharms(pnum))
		return POPUP_NOMORESMALLCHARMS;

	// A slot past what this player has unlocked is not a slot yet.
	if(item_type == DND_ITEM_FLASK && use_id >= FLASK1_INDEX + GetPlayerFlaskSlots(pnum))
		return POPUP_ONLYONEFLASK;

	// check if player is trying to equip another of the same utility flask if it is utility
	if(item_type == DND_ITEM_FLASK && IsUtilityFlask(item.item_subtype)) {
		// Every OTHER slot, not just the one opposite. This used to name FLASK1/FLASK2 outright,
		// which silently stopped covering everything the moment a third slot existed.
		for(i = 0; i < MAX_FLASK_SLOTS; ++i) {
			if(FLASK1_INDEX + i == use_id)
				continue;
			if(GlobalItemStorage.Items_Used[pnum][FLASK1_INDEX + i].item_subtype == item.item_subtype)
				return POPUP_ONLYONEFLASK;
		}
	}

	// if has forbid armor but has equipped body armor, don't allow that item to be put, and vice versa if has no armor and has forbid armor etc.
	if
	(
		isUnique &&
		(item.item_type >> UNIQUE_BITS) - 1 == UITEM_OAKHEART &&
		IsWearingBodyArmor(pnum)
	)
		return POPUP_CANTWEARBODYARMOR;

	if(item_type == DND_ITEM_BODYARMOR && PlayerModData[pnum].vals[PSTAT_EX_FORBID_ARMOR])
		return POPUP_CANTPUTONBODYARMOR;
	// proceed to equip the item now

	// this means we must swap items
	auto used_item = GetUsedItem(pnum, use_id);
	if(used_item.item_type != DND_ITEM_NULL) {
		ApplyItemFeatures(pnum, use_id, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_REMOVE, true);
		SwapItems(pnum, use_id, item_index, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false, true);
		ApplyItemFeatures(pnum, use_id, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_ADD);
	}
	else {
		// just zero the stuff in inventory, and copy them into items used
		SetItemToAnother(used_item, item);

		used_item.topleftboxid = use_id + 1;

		// the leftover spot is a null item
		int wtemp = item.width;
		int htemp = item.height;
		FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
		//SyncItemData(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, wtemp, htemp);
		SyncItemData(pnum, use_id, DND_SYNC_ITEMSOURCE_ITEMSUSED, -1, -1);
		ApplyItemFeatures(pnum, use_id, DND_SYNC_ITEMSOURCE_ITEMSUSED);
	}

	// request damage cache recalculation
	ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, PlayerNumber());

	return -1;
}

// based on average player level
int RollItemLevel(int max_lvl = MAX_REGULAR_ILVL) {
	int res = InformationInLevel[LEVELINFO_PLAYERCOUNTATSTART];
	if(!res)
		res = 1;

	int pavg;
	if(InformationInLevel[LEVELINFO_ISDUNGEON] && (pavg = HasDungeonUpside(DUN_UPSIDE_ITEMLEVELBONUS))) {
		max_lvl += pavg;
		if(max_lvl > MAX_ITEM_LEVEL)
			max_lvl = MAX_ITEM_LEVEL;
	}
	
	// return average player level, +- some value -- 33% chance to just roll something between min player level and max player level regardless
	// if that 33% chance rolls then there's a 50-50 chance that it'll be either min - max level players or something much closer to lower lvl player
	if(DungeonInformation.level == -1 && !random(0, 2)) {
		if(random(0, 1)) {
			return Clamp_Between(
				random(InformationInLevel[LEVELINFO_MINPLAYERLEVEL], InformationInLevel[LEVELINFO_MAXPLAYERLEVEL]),
				1,
				max_lvl
			);
		}
		res = random(InformationInLevel[LEVELINFO_MINPLAYERLEVEL], InformationInLevel[LEVELINFO_MINPLAYERLEVEL] + 2 * ITEMLEVEL_VARIANCE_HIGHER);
		res = Clamp_Between(res, 1, max_lvl);
		return res;
	}

	pavg = 0;
	if(DungeonInformation.level == -1)
		pavg = InformationInLevel[LEVELINFO_PLAYERLEVEL] / res;
	else
		pavg = DungeonInformation.level;

	if(pavg > 2 * ITEMLEVEL_VARIANCE_LOWER) {
		res = pavg + random(-ITEMLEVEL_VARIANCE_LOWER, ITEMLEVEL_VARIANCE_HIGHER);
		res = Clamp_Between(res, 1, max_lvl);
		return res;
	}

	return pavg + random((-pavg + 1) / 2, ITEMLEVEL_VARIANCE_HIGHER);
}

// attrib_id is the raw attribute, like INV_INCHP etc. not an index in the item!
int CheckItemAttribute(int pnum, int item_pos, int attrib_id, int source, int count) {
	int i;
	for(i = 0; i < count; ++i) {
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_pos, i, source) == attrib_id)
			return i;
	}
	return -1;
}

// specialized version of the above function for playerinventorylist -- keeps it cleaner arguments wise I guess
int IsAttribInItem(int pnum, int item_pos, int attrib_id) {
	auto item = GetPlayerInventoryItem(pnum, item_pos);
	int count = item.attrib_count;
	for(int i = 0; i < count; ++i)
		if(item.attributes[i].attrib_id == attrib_id)
			return true;
	return false;
}

// find the item that has a min, if basis isn't -1 then we must exclude this from inclusion to min
int FindMinOnUsedCharmsForAttribute(int pnum, int attrib_index, int basis) {
	int res = -1, temp, compare = bcs::INT_MAX;
	for(int i = 0; i < MAX_ITEMS_EQUIPPABLE; ++i) {
		if(i == basis)
			continue;

		auto item = GetUsedItem(pnum, i);

		if(item.item_type != DND_ITEM_NULL) {
			temp = CheckItemAttribute(pnum, i, attrib_index, DND_SYNC_ITEMSOURCE_ITEMSUSED, item.attrib_count);
			// means this exists
			if(temp != -1) {
				if(item.attributes[temp].attrib_val < compare) {
					compare = item.attributes[temp].attrib_val;
					SetInventory("DamagePerFlatHPBuffer", compare);
					res = i;
				}
			}
		}
	}
	return res;
}

// returns false if it should not destroy item, true if it should
bool CopyItemFromFieldToPlayer(int fieldpos, int player_index, int item_index, int stacked_item_type = -1, int previous_spot = -1) {
	int i, j, wtemp, htemp;
	// handle the box management
	// is this a stack item and does it already contain an item of this type?
	// ex type = orb, subtype = enhancement
	auto field_item = GetFieldItem(fieldpos);
	auto p_item = GetPlayerInventoryItem(player_index, item_index);

	int max_stack = GetStackValue(field_item.item_type, 0);
	int new_pos = previous_spot;
	if(
		field_item.item_stack && 
		field_item.item_type == p_item.item_type && 
		field_item.item_subtype == p_item.item_subtype &&
		p_item.item_stack + field_item.item_stack <= max_stack
	) {
		// just add to the stack
		p_item.item_stack += field_item.item_stack;
		SyncItemStack(player_index, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	}
	else {
		// no? -- check to dump as much of the stack from this into the other item, and create new spot with leftover stack
		//Log(s:"conditions: ", d:p_item.item_stack, s: " vs ", d:max_stack, s: ", ", d:p_item.item_stack, s: " + ", d:field_item.item_stack, s: " > ", d:max_stack);
		if(stacked_item_type != -1 && p_item.item_stack != max_stack && p_item.item_stack + field_item.item_stack > max_stack) {
			if(p_item.item_type == DND_ITEM_NULL) {
				// copy it here if its an empty slot
				SetItemToAnother(p_item, field_item);
				p_item.item_stack = 0;
				p_item.topleftboxid = item_index + 1;
			}
			
			field_item.item_stack -= max_stack - p_item.item_stack;
			p_item.item_stack = max_stack;
			//Log(s:"stacks moving here ", d:p_item.item_stack, s:" ", d:field_item.item_stack);
			SyncItemData(player_index, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, p_item.width, p_item.height);
			SyncItemData(player_index, fieldpos, DND_SYNC_ITEMSOURCE_FIELD, field_item.width, field_item.height);
			
			// create new item and place it, if can't find place to put it leave it on the ground with leftover stacks
			new_pos = GetFreeSpotForItemWithStack(fieldpos, player_index, DND_SYNC_ITEMSOURCE_FIELD, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			//Log(s:"new spot: ", d:new_pos, s: " vs target earlier ", d:item_index);
			if(new_pos != -1)
				return CopyItemFromFieldToPlayer(fieldpos, player_index, new_pos, stacked_item_type, item_index);
			
			ACS_NamedExecuteAlways("DnD Inventory Full CS", 0, player_index);
			return false;
		}

		//Log(s:"indices new and item ", d:new_pos, s:" ", d:item_index);

		if(new_pos == item_index) {
			item_index = GetFreeSpotForItemWithStack(fieldpos, player_index, DND_SYNC_ITEMSOURCE_FIELD, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			if(item_index == -1)
				return false;
			//Log(s:"updated index ", d:item_index);
			p_item = GetPlayerInventoryItem(player_index, item_index);
		}

		// look to place it
		wtemp = field_item.width;
		htemp = field_item.height;

		SetItemToAnother(p_item, field_item);

		//Log(s:"after setting items to one another: ", d:p_item.item_stack, s:" ", d:field_item.item_stack);

		for(i = 0; i < htemp; ++i)
			for(j = 0; j < wtemp; ++j) {
				p_item = GetPlayerInventoryItem(player_index, item_index + i * MAXINVENTORYBLOCKS_VERT + j);
				p_item.item_type = field_item.item_type;
				p_item.topleftboxid = item_index + 1;
			}

		//Log(s:"item type: ", d:(p_item.item_type >> 16) - 1, s: " ", d:field_item.item_type);
		
		SyncItemData(player_index, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, wtemp, htemp);
	}
	// the leftover spot is a null item
	RemoveItemFromWorld(fieldpos);
	//FreeItem(-1, fieldpos, DND_SYNC_ITEMSOURCE_FIELD, false);
	return true;
}

// clones an item on this player's inventory, if no spot is found it won't bother
int CloneItem(int pnum, int item_index, int source, bool dontSync, int dest_source = -1) {
	int temp;
	if(dest_source == -1)
		dest_source = source;

	int c = GetFreeSpotForItem(item_index, pnum, source, dest_source);
	if(c != -1) {
		int wtemp = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, item_index, -1, source);
		int htemp = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, item_index, -1, source);
		int i, j, k;
		SetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, c, -1, wtemp, dest_source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, c, -1, htemp, dest_source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, item_index, -1, source), dest_source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, item_index, -1, source), dest_source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, item_index, -1, source), dest_source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, item_index, -1, source), dest_source);

		SetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, item_index, -1, source), dest_source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_index, -1, source), dest_source);

		for(k = 0; k < MAX_ITEM_IMPLICITS; ++k) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, k, source), dest_source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, k, source), dest_source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, item_index, k, source), dest_source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, k, source), dest_source);
		}

		temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, c, -1, temp, dest_source);
		for(k = 0; k < temp; ++k) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, k, source), dest_source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, k, source), dest_source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, k, source), dest_source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, k, source), dest_source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, item_index, k, source), dest_source);
		}
		for(i = 0; i < htemp; ++i)
			for(j = 0; j < wtemp; ++j) {
				temp = c + i * MAXINVENTORYBLOCKS_VERT + j;
				SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_index, -1, source), dest_source);
				SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, temp, -1, c + 1, dest_source);
			}
		if(!dontSync)
			SyncItemData(pnum, c, dest_source, -1, -1);
		return c;
	}
	return -1;
}

int CloneItemToSpot(int pnum, int item_index, int source, int dest_spot, bool dontSync, int dest_source = -1) {
	int temp;
	if(dest_source == -1)
		dest_source = source;

	// this doesnt retrieve a free spot, assumes the dest_spot is a free spot already
	int wtemp = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, item_index, -1, source);
	int htemp = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, item_index, -1, source);
	int i, j, k;
	SetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, dest_spot, -1, wtemp, dest_source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, dest_spot, -1, htemp, dest_source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, dest_spot, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, item_index, -1, source), dest_source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, dest_spot, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, item_index, -1, source), dest_source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, dest_spot, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, item_index, -1, source), dest_source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, dest_spot, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, item_index, -1, source), dest_source);

	SetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, dest_spot, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, item_index, -1, source), dest_source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, dest_spot, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_index, -1, source), dest_source);

	for(k = 0; k < MAX_ITEM_IMPLICITS; ++k) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, dest_spot, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, k, source), dest_source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, dest_spot, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, k, source), dest_source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, dest_spot, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, item_index, k, source), dest_source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, dest_spot, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, k, source), dest_source);
	}

	temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, dest_spot, -1, temp, dest_source);
	for(k = 0; k < temp; ++k) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, dest_spot, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, k, source), dest_source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, dest_spot, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, k, source), dest_source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, dest_spot, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, k, source), dest_source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, dest_spot, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, k, source), dest_source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, dest_spot, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, item_index, k, source), dest_source);
	}
	for(i = 0; i < htemp; ++i)
		for(j = 0; j < wtemp; ++j) {
			temp = dest_spot + i * MAXINVENTORYBLOCKS_VERT + j;
			SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_index, -1, source), dest_source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, temp, -1, dest_spot + 1, dest_source);
		}
	if(!dontSync)
		SyncItemData(pnum, dest_spot, dest_source, -1, -1);
	return dest_spot;
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

			// if not empty and it's not us -- the "if its not us" part only matters if the sources are the same
			// because only then it shouldn't overlap, if sources are different different topboxids are irrelevant!
			tb = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, emptysource);
			if(tb && (itemsource != emptysource || tb - 1 != temp))
				return false;
		}
	}
	return true;
}

bool IsValidBox(int beg) {
	return beg < MAX_INVENTORY_BOXES && beg >= 0;
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

	// these hold 64 box spots (45 only needed but still) to make sure we are good on the overlapping areas
	int overlap1 = 0, overlap2 = 0;
	bool rowStart = false;
	// from ipos2 to ipos1
	tb1 = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, source1) - 1;
	tb2 = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, source2) - 1;
	w2 = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, tb2, -1, source2);
	h2 = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, tb2, -1, source2);
				
	rowStart = !(((ipos1 + offset2)) % MAXINVENTORYBLOCKS_VERT);
	for(i = 0; i < h2; ++i) {
		for(j = 0; j < w2; ++j) {
			bid = ipos1 + offset2 + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0 || (!rowStart && !(bid % 9)))
				return false;

			if(IsSourceInventoryView(source1) && GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source1) - 1 != tb1 && GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, bid, -1, source1) != DND_ITEM_NULL)
				return false;

			if(bid < 32)
				overlap1 |= 1 << bid;
			else
				overlap2 |= 1 << (bid - 32);
		}
	}

	// from ipos1 to ipos2
	w1 = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, tb1, -1, source1);
	h1 = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, tb1, -1, source1);
	rowStart = !(((ipos2 + offset1)) % MAXINVENTORYBLOCKS_VERT);
	for(i = 0; i < h1; ++i) {
		for(j = 0; j < w1; ++j) {
			bid = ipos2 + offset1 + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0 || (!rowStart && !(bid % 9)))
				return false;

			if(IsSourceInventoryView(source2) && GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source2) - 1 != tb2 && GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, bid, -1, source2) != DND_ITEM_NULL)
				return false;

			// check potential overlaps now -- can only happen if same source
			if(source1 == source2 && ((bid < 32 && (overlap1 & (1 << bid))) || (overlap2 & (1 << (bid - 32)))))
				return false;
		}
	}
	return true;
}

// accepts topboxid as item_index
void MoveItemToTemporary(int player_index, int item_index, int temp_pos, int source) {
	int i;
	auto temp_item = GetTemporaryItem(player_index, temp_pos);
	auto p_item = AcquireItemFromSource(player_index, item_index, source);

	SetItemToAnother(temp_item, p_item);

	temp_item.topleftboxid = 0;
}

// copies item from temporary list to player inventory
void CopyItemFromTemporary(int player_index, int item_index, int temp_pos, int source) {
	auto temp_item = GetTemporaryItem(player_index, temp_pos);
	int i;
	int w = temp_item.width, h = temp_item.height;

	auto p_item = AcquireItemFromSource(player_index, item_index, source);

	SetItemToAnother(p_item, temp_item);
	
	if(IsSourceInventoryView(source)) {
		for(i = 0; i < h; ++i) {
			for(int j = 0; j < w; ++j) {
				int bid = item_index + j + i * MAXINVENTORYBLOCKS_VERT;
				SetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, item_index + 1, source);
				SetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, bid, -1, temp_item.item_type, source);
			}
		}
	}
	else {
		SetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, item_index + 1, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, item_index, -1, temp_item.item_type, source);
	}
}

// for swapping items -- assumes neither are null
void SwapItems(int pnum, int ipos1, int ipos2, int source1, int source2, bool dontSync, bool forcedSwap = false) {
	// check if there's enough space
	// find if there is some sort of offset we must take care of, this matters only if we are swapping in inventory
	int offset1 = 0, offset2 = 0;
	int h1p, h2p, w1p, w2p, i2_type;
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
		w2p = GetStackValue(w1p, source1);
		// add stack of ipos2 to ipos1
		if(h1p + h2p <= w2p) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos1 + offset1, -1, h1p + h2p, source1);
			SyncItemStack(pnum, ipos1 + offset1, source1);
			// dispose of 2nd item
			FreeItem(pnum, ipos2 + offset2, source2, false);
			if(IsPlayerTradeGrid(pnum, source2))
				SetTradeItemOrigin(pnum, ipos2 + offset2, -1);
		}
		else {
			// set stack of h1p to max, then set the stack of ipos2 to whatever is left
			if(h1p != w2p && h2p != w2p) {
				SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos2 + offset2, -1, h2p - w2p + h1p, source2);
				SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos1 + offset1, -1, w2p, source1);
			}
			else if(h1p <= GetStackValue(w1p, source2) && h2p <= w2p) {
				// one of them is max stack of the source that can handle! just swap around
				SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos2 + offset2, -1, h1p, source2);
				SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos1 + offset1, -1, h2p, source1);
			}
			SyncItemStack(pnum, ipos1 + offset1, source1);
			SyncItemStack(pnum, ipos2 + offset2, source2);
		}
	}
	else if(forcedSwap || CanSwapItems(pnum, ipos1, ipos2, offset1, offset2, source1, source2)) {
		MoveItemToTemporary(pnum, ipos1 + offset1, 0, source1);
		MoveItemToTemporary(pnum, ipos2 + offset2, 1, source2);
		
		FreeItem(pnum, ipos1 + offset1, source1, false);
		FreeItem(pnum, ipos2 + offset2, source2, false);
		
		CopyItemFromTemporary(pnum, ipos1 + offset2, 1, source1);
		CopyItemFromTemporary(pnum, ipos2 + offset1, 0, source2);

		// a swap is the one move that doesn't run through MoveItemTrade, so the trade notes are
		// handed over here too. item1 landed in source2, item2 landed in source1.
		if(IsPlayerTradeGrid(pnum, source1) || IsPlayerTradeGrid(pnum, source2)) {
			int org1 = IsPlayerTradeGrid(pnum, source1) ? GetTradeItemOrigin(pnum, ipos1 + offset1) : -1;
			int org2 = IsPlayerTradeGrid(pnum, source2) ? GetTradeItemOrigin(pnum, ipos2 + offset2) : -1;
			if(IsPlayerTradeGrid(pnum, source1)) {
				SetTradeItemOrigin(pnum, ipos1 + offset1, -1);
				SetTradeItemOrigin(pnum, ipos1 + offset2, org2);
			}
			if(IsPlayerTradeGrid(pnum, source2)) {
				SetTradeItemOrigin(pnum, ipos2 + offset2, -1);
				SetTradeItemOrigin(pnum, ipos2 + offset1, org1);
			}
		}

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
	int tb = GlobalItemStorage.PlayerInventoryList[pnum][itempos].topleftboxid - 1;
	int offset = tb - itempos;
	
	int i, j, bid;

	auto item_from = GetPlayerInventoryItem(pnum, tb);
	
	int w = item_from.width;
	int h = item_from.height;

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

	auto item_to = GetPlayerInventoryItem(pnum, temp);

	SetItemToAnother(item_to, item_from);

	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			item_to = GetPlayerInventoryItem(pnum, temp + i * MAXINVENTORYBLOCKS_VERT + j);
			item_to.item_type = item_from.item_type;
			item_to.topleftboxid = temp + 1;
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

int AutoMoveItem(int pnum, int boxid, int isource, int ssource, bool noSync = false) {
	int res = 0;

	int tpbid = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, boxid, -1, isource) - 1;
	int i, j;
	int try_pos = -1;

	// if this is a stacked item, scan the destination for any of this item to add on top, if we can
	// reduce stack count if we couldn't, and if we could, we're done
	int itype = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, tpbid, -1, isource);
	int isubtype = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, tpbid, -1, isource);
	int maxstacks = GetStackValue(itype, ssource), istacks = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, tpbid, -1, isource);
	int stype;
	if(IsStackedItem(itype)) {
		for(j = 0; !res && j < MAXINVENTORYBLOCKS_VERT; ++j) {
			for(i = 0; !res && i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
				try_pos = j + i * MAXINVENTORYBLOCKS_VERT;
				// check destination for same item to potentially put it onto it
				stype = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, try_pos, -1, ssource);
				if
				(
					stype == itype &&
					GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, try_pos, -1, ssource) == isubtype
				) 
				{
					// move stacks of what we got to this one and end the operation
					// check if we have reached max, if we did, then we must proceed further below
					stype = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, try_pos, -1, ssource);
					if(stype + istacks <= maxstacks) {
						SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, try_pos, -1, stype + istacks, ssource);

						if(!noSync)
							SyncItemStack(pnum, try_pos, ssource);

						FreeItem(pnum, tpbid, isource, false);
						return try_pos;
					}
					else if(stype != maxstacks) {
						// set target one to max, and ours max - what we had
						SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, try_pos, -1, maxstacks, ssource);

						if(!noSync)
							SyncItemStack(pnum, try_pos, ssource);
						
						// dont sync this, we'll move it later anyways then the real sync will occur
						SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, tpbid, -1, istacks - (maxstacks - stype), isource);
						res = 1;
					}
				}
			}
		}
	}

	// if this is the special orb stash page and we are dealing with more than the max limits while auto moving, handle it here
	if(!res && itype == DND_ITEM_ORB && (isource >> 16) == PAGEID_STASHTAB_ORBS && istacks > maxstacks) {
		// we are auto pulling from orb stash tab to our inventory, only ever allow stacks of max stacks on regular pages
		res = CloneItem(pnum, tpbid, isource, true, ssource);
		if(res != -1) {
			i = GetStackValue(DND_ITEM_ORB);
			SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, tpbid, -1, istacks - i, isource);
			SyncItemStack(pnum, tpbid, isource);

			SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, res, -1, i, ssource);

			if(!noSync)
				SyncItemData(pnum, res, ssource, -1, -1);

			return res;
		}
		return -1;
	}

	// scan from vertical positions to horizontal first to find a good spot
	for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
		for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
			try_pos = j + i * MAXINVENTORYBLOCKS_VERT;
			if(IsFreeSpot(pnum, tpbid, try_pos, isource, ssource)) {
				// move item to here now
				MoveItemTrade(pnum, tpbid, try_pos, isource, ssource, noSync);
				return try_pos;
			}
		}
	}

	return -1;
}

// auto dump functionality from inventory of player to stash
void AutoDumpItems(int pnum, int stackableOnly = 0) {
	// for each item the player has, attempt to AutoMoveItem them to stash
	// first store item ids in an array, sorted from biggest to shortest (biggest first in list)
	static int marked_tbids[MAX_INVENTORY_BOXES];
	static int is_orb[MAX_INVENTORY_BOXES];

	int i, j, k, count = 0;
	auto item_move_list = GetItemMoveList(pnum);
	int curr_page = CheckInventory("DnD_PlayerCurrentPage") - 1;

	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		marked_tbids[i] = 0;
		is_orb[i] = 0;
		item_move_list[i].dest_pos = -1;
	}

	// ItemMoveList contains topboxids of items to be moved in order of largest to smallest size occupying (w * h)
	// insert sorted
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto item = GetPlayerInventoryItem(pnum, i);
		if(item.item_type != DND_ITEM_NULL && (!stackableOnly || IsStackedItem(item.item_type)) && !marked_tbids[i]) {
			if(curr_page == PAGEID_STASHTAB_ORBS && item.item_type != DND_ITEM_ORB)
				continue;

			k = 0;
			j = 0;
			while(item_move_list[k].dest_pos != -1) {
				if(item_move_list[k].width * item_move_list[k].height < item.width * item.height) {
					// need to shift items right starting from k
					j = 1;
					break;
				}
				++k;
			}

			// add it here, but check if this current location is actually occupied!
			if(j) {
				// shift then add to where k is
				//printbold(s:"shifting item at ", d:k, s: " right");
				for(j = count - 1; j >= k; --j) {
					//printbold(s:"move ", d:j, s: " to ", d:j + 1, s: " item dest_pos: ", d:ItemMoveList[pnum][j + 1].dest_pos, s: " to ", d:ItemMoveList[pnum][j].dest_pos);
					item_move_list[j + 1].dest_pos = item_move_list[j].dest_pos;
					item_move_list[j + 1].width = item_move_list[j].width;
					item_move_list[j + 1].height = item_move_list[j].height;
				}
			}

			// add here
			//printbold(s:"insert to movelist ", d:k, s: " item at ", d:i, s:" w and h: ", d:PlayerInventoryList[pnum][i].width, s: " ", d:PlayerInventoryList[pnum][i].height);
			item_move_list[k].dest_pos = item.topleftboxid;
			item_move_list[k].width = item.width;
			item_move_list[k].height = item.height;

			++count;

			// mark them for quick checks so we dont keep checking these
			for(k = 0; k < item.width; ++k) 
				for(j = 0; j < item.height; ++j)
					marked_tbids[i + k  + j * MAXINVENTORYBLOCKS_VERT] = true;
		}
	}

	//printbold(s:"count of items to move: ", d:count);

	// we formed the list of items to be send to stash, sorted wrt size, now just send them over
	for(i = 0; i < count; ++i) {
		//printbold(s:"item: ", d:i, s: " w and h: ", d:ItemMoveList[pnum][i].width, s: " ", d:ItemMoveList[pnum][i].height, s:" move item pos: ", d:ItemMoveList[pnum][i].dest_pos - 1);
		item = GetPlayerInventoryItem(pnum, item_move_list[i].dest_pos - 1);
		
		if(stackableOnly && item.item_type == DND_ITEM_ORB) {
			curr_page = PAGEID_STASHTAB_ORBS;
			is_orb[i] = 1;
		}
		else
			curr_page = CheckInventory("DnD_PlayerCurrentPage") - 1;

		marked_tbids[i] = AutoMoveItem(pnum, item_move_list[i].dest_pos - 1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, DND_SYNC_ITEMSOURCE_STASH | (curr_page << 16), true);

		// if somehow we got no space on the dedicated page, try dumping to the current page instead, if that fails it fails
		if(marked_tbids[i] == -1 && is_orb[i]) {
			is_orb[i] = 0;
			curr_page = CheckInventory("DnD_PlayerCurrentPage") - 1;
			marked_tbids[i] = AutoMoveItem(pnum, item_move_list[i].dest_pos - 1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, DND_SYNC_ITEMSOURCE_STASH | (curr_page << 16), true);
		}
	}

	// sync them all after if movement was successful (this preserves later updates such as multiple same kinds of stacked items retain the sum)
	for(i = 0; i < count; ++i) {
		if(marked_tbids[i] == -1)
			continue;

		if(stackableOnly && is_orb[i])
			curr_page = PAGEID_STASHTAB_ORBS;
		else
			curr_page = CheckInventory("DnD_PlayerCurrentPage") - 1;

		SyncItemData(pnum, marked_tbids[i], DND_SYNC_ITEMSOURCE_STASH | (curr_page << 16), -1, -1);
	}
}

// ---- auto sort -------------------------------------------------------------------------------

typedef struct isort {
	int origin_box;		// where it sat before, so a pack that cannot finish can put it all back
	int area;
	int item_type;
	int item_subtype;
	int item_level;
} isort_T;

isort_T[] module& GetSortList() {
	static isort_T SortList[MAX_INVENTORY_BOXES];
	return SortList;
}

typedef struct isortstate {
	int busy;
} isortstate_T;

// Guards the one shared scratch grid. If this ever leaks set, sorting is dead for everyone until
// the map ends, so the waiter takes it anyway once its patience runs out -- a leak has to degrade
// to one skipped sort, never to a button that stopped working.
isortstate_T module& GetSortState() {
	static isortstate_T SortState;
	return SortState;
}

// Consolidates stacks of the same thing, so the packer gets fewer and fuller items to place and a
// half stack never sits next to its own twin.
void MergeStacksInGrid(int pnum, int source) {
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto item = AcquireItemFromSource(pnum, i, source);
		if(item.item_type == DND_ITEM_NULL || item.topleftboxid != i + 1 || !IsStackedItem(item.item_type))
			continue;

		int cap = GetStackValue(item.item_type, source);
		for(int j = i + 1; j < MAX_INVENTORY_BOXES && item.item_stack < cap; ++j) {
			auto other = AcquireItemFromSource(pnum, j, source);
			if(other.topleftboxid != j + 1 || other.item_type != item.item_type || other.item_subtype != item.item_subtype)
				continue;

			int move = cap - item.item_stack;
			if(move > other.item_stack)
				move = other.item_stack;

			item.item_stack += move;
			other.item_stack -= move;

			if(!other.item_stack)
				FreeItem(pnum, j, source, true);
		}
	}
}

// Writes a parked item back into the grid at item_index and re-marks every cell it covers.
void PlaceItemFromSortScratch(int pnum, int item_index, int scratch_pos, int source) {
	auto s_item = GetSortScratchItem(scratch_pos);
	auto p_item = AcquireItemFromSource(pnum, item_index, source);

	SetItemToAnother(p_item, s_item);

	for(int i = 0; i < s_item.height; ++i)
		for(int j = 0; j < s_item.width; ++j) {
			auto cell = AcquireItemFromSource(pnum, item_index + j + i * MAXINVENTORYBLOCKS_VERT, source);
			cell.topleftboxid = item_index + 1;
			cell.item_type = s_item.item_type;
		}
}

// Repacks one grid in place. source is a whole source value, so a stash page arrives as
// DND_SYNC_ITEMSOURCE_STASH | (page << 16). Returns how many items were placed, or -1 if the pack
// could not finish, in which case the grid is left exactly as it was found.
// The caller owns the busy flag and the sync pass -- both want to yield and functions cannot.
int SortInventoryGrid(int pnum, int source) {
	int i, j, k;
	auto sort_list = GetSortList();
	int count = 0;

	MergeStacksInGrid(pnum, source);

	// decide the order first, without touching anything
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto item = AcquireItemFromSource(pnum, i, source);
		if(item.item_type == DND_ITEM_NULL || item.topleftboxid != i + 1)
			continue;

		int area = item.width * item.height;

		// biggest first so the awkward shapes get the open grid, then type, subtype and tier so
		// identical sizes still land next to their own kind instead of wherever they fell
		for(k = 0; k < count; ++k) {
			if(area != sort_list[k].area) {
				if(area > sort_list[k].area)
					break;
				continue;
			}
			if(item.item_type != sort_list[k].item_type) {
				if(item.item_type < sort_list[k].item_type)
					break;
				continue;
			}
			if(item.item_subtype != sort_list[k].item_subtype) {
				if(item.item_subtype < sort_list[k].item_subtype)
					break;
				continue;
			}
			if(item.item_level > sort_list[k].item_level)
				break;
		}

		for(j = count; j > k; --j) {
			sort_list[j].origin_box = sort_list[j - 1].origin_box;
			sort_list[j].area = sort_list[j - 1].area;
			sort_list[j].item_type = sort_list[j - 1].item_type;
			sort_list[j].item_subtype = sort_list[j - 1].item_subtype;
			sort_list[j].item_level = sort_list[j - 1].item_level;
		}

		sort_list[k].origin_box = i;
		sort_list[k].area = area;
		sort_list[k].item_type = item.item_type;
		sort_list[k].item_subtype = item.item_subtype;
		sort_list[k].item_level = item.item_level;

		++count;
	}

	// lift everything out in placement order, then empty the real grid
	for(i = 0; i < count; ++i)
		SetItemToAnother(GetSortScratchItem(i), AcquireItemFromSource(pnum, sort_list[i].origin_box, source));

	for(i = 0; i < count; ++i)
		FreeItem(pnum, sort_list[i].origin_box, source, true);

	// place with the same placer the rest of the menu uses, so this cannot drift from it
	for(i = 0; i < count; ++i) {
		auto s_item = GetSortScratchItem(i);
		int spot = FindFreeSpotForSize(pnum, s_item.width, s_item.height, source);

		// Biggest first is a heuristic, not a proof: a hand packed grid can be an arrangement it
		// cannot reproduce. Put everything back rather than leave the player an item short.
		if(spot == -1) {
			for(j = 0; j < MAX_INVENTORY_BOXES; ++j)
				FreeItem(pnum, j, source, true);
			for(j = 0; j < count; ++j)
				PlaceItemFromSortScratch(pnum, sort_list[j].origin_box, j, source);
			return -1;
		}

		PlaceItemFromSortScratch(pnum, spot, i, source);
	}

	return count;
}

// this is made specifically for trade view, the one above is optimized for normal inventory
// also used for moving items from one source to another, ie. inventory to stash etc.
void MoveItemTrade(int pnum, int itempos, int emptypos, int itemsource, int emptysource, bool noSync = false) {
	int tb = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, itemsource) - 1;
	int offset = tb - itempos;
	
	//printbold(s:"will move tbid ", d:tb, s: " offset ", d:offset, s:" to loc: ", d:emptypos, s: "isrc: ", d:itemsource, s: " esrc: ", d:emptysource);

	int i, j, bid;

	auto item_from = AcquireItemFromSource(pnum, tb, itemsource);
	
	int w = item_from.width;
	int h = item_from.height;

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
					if(bid < 32)
						set1 |= 1 << (bid);
					else
						set2 |= 1 << (bid - 32);
				}
			}
	}

	auto item_to = AcquireItemFromSource(pnum, temp, emptysource);

	SetItemToAnother(item_to, item_from);
	
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
	
	// carry the note saying where this item came from, so a trade that falls through can hand it
	// back to the same box -- read it before clearing, a move inside the trade grid is both sides
	if(IsPlayerTradeGrid(pnum, itemsource) || IsPlayerTradeGrid(pnum, emptysource)) {
		int origin = -1;
		if(IsPlayerTradeGrid(pnum, itemsource)) {
			origin = GetTradeItemOrigin(pnum, tb);
			SetTradeItemOrigin(pnum, tb, -1);
		}
		else if(itemsource == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY)
			origin = tb;

		if(IsPlayerTradeGrid(pnum, emptysource))
			SetTradeItemOrigin(pnum, temp, origin);
	}

	// as soon as this item is offered for a trade it can't be edited
	if(Player_MostRecent_Orb[pnum].p_tempwep == itempos && itemsource == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY)
		Player_MostRecent_Orb[pnum].p_tempwep = 0;
	
	if(!noSync)
		SyncItemData_Special(pnum, temp, emptysource);
}

// this simply carries an item from another player's place to another, like moveitem but has player inputs
// from p_item to p_empty
void CarryItemTo(int itempos, int emptypos, int itemsource, int emptysource, int p_item, int p_empty, bool regular_free = true, bool no_wh_check = false, bool dontSync = false) {
	int tb = GetItemSyncValue(p_item, DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, itemsource) - 1;
	int offset = tb - itempos;
	
	int i, j, bid;
	
	int w = GetItemSyncValue(p_item, DND_SYNC_ITEMWIDTH, tb, -1, itemsource);
	int h = GetItemSyncValue(p_item, DND_SYNC_ITEMHEIGHT, tb, -1, itemsource);
	
	int temp = emptypos + offset;
	
	//printbold(s:"carry item ", d:itempos, s: " to pos ", d:emptypos);

	auto item_to = AcquireItemFromSource(p_empty, temp, emptysource);
	auto item_from = AcquireItemFromSource(p_item, tb, itemsource);
	
	// if stacked item, add it on top
	if(item_to.item_type != DND_ITEM_NULL && item_to.item_stack && item_from.item_stack && item_to.item_type == item_from.item_type && item_to.item_subtype == item_from.item_subtype) {
		// cap check should be done from earlier in case it can't accommodate it
		item_to.item_stack += item_from.item_stack;

		if(regular_free)
			FreeItem(p_item, tb, itemsource, dontSync);
		else
			FreeSpot(p_item, tb, itemsource, dontSync);

		SyncItemStack(p_empty, temp, emptysource);

		return;
	}
	else
		SetItemToAnother(item_to, item_from);

	bid = GetItemSyncValue(p_item, DND_SYNC_ITEMTYPE, tb, -1, itemsource);

	if(no_wh_check) {
		SetItemSyncValue(p_empty, DND_SYNC_ITEMTYPE, temp, -1, item_from.item_type, emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMTOPLEFTBOX, temp, -1, emptypos + 1, emptysource);

		//printbold(s:"assign topleftbox to ", d:temp, s:": ", d:emptypos + 1);
	}
	else {
		for(i = 0; i < h; ++i)
			for(j = 0; j < w; ++j) {
				SetItemSyncValue(p_empty, DND_SYNC_ITEMTYPE, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, bid, emptysource);
				SetItemSyncValue(p_empty, DND_SYNC_ITEMTOPLEFTBOX, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, temp + 1, emptysource);
			}
	}

	if(regular_free)
		FreeItem(p_item, tb, itemsource, dontSync);
	else
		FreeSpot(p_item, tb, itemsource, dontSync);

	if(!dontSync)
		SyncItemData(p_empty, temp, emptysource, -1, -1);
}

void TransferTradeItems(int from, int to) {
	int bid;
	int i, j, h, w;
	auto item_move_list = GetItemMoveList(to);

	// for every possible item in the trade list of this player, get free position
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			bid = j + i * MAXINVENTORYBLOCKS_VERT;
			// care about the items only once, so use topleftboxid == bid
			if(GlobalItemStorage.TradeViewList[from][bid].topleftboxid - 1 == bid) {
				//printbold(s:"carry item to ", d:ItemMoveList[to][bid].dest_pos, s: " from player ", d:from, s:"'s movelist to player ", d:to);
				CarryItemTo(bid, item_move_list[bid].dest_pos, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, from, to);
			}
		}
	}
	
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		item_move_list[i].state = false;
		item_move_list[i].width = 0;
		item_move_list[i].height = 0;
		item_move_list[i].dest_pos = -1;
	}

	// the offer went through, so "put it back where it was" means nothing for these anymore
	ClearTradeItemOrigins(from);
}

int GetInventoryInfoOffset(int itype) {
	switch(itype) {
		case DND_ITEM_CHESTKEY:
		return CHESTKEY_BEGIN;
		case DND_ITEM_ORB:
		return ORBS_BEGIN;
		case DND_ITEM_TOKEN:
		return TOKEN_BEGIN;
		case DND_ITEM_DUNGEONKEY:
		return DUNGEONKEY_BEGIN;
	}
	return 0;
}

// outside source implies this was called from the menu, not in the game
void DrawInventoryInfo_Field(int pnum, int topboxid, int source, int yoff, bool isOutsideSource) {
	DeleteTextRange(RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 18 + ITEMID_SKIP, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES);
	
	// dont draw in custom intermission
	if(CheckInventory("DnD_IntermissionState"))
		return;
	
	int stack, itype = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, topboxid, -1, source), left = 0;
	if(topboxid != -1 && itype != DND_ITEM_NULL) {
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
		
		// adjust pos
		left = GetHudLeft(HUDMAX_X) << 16;
		int bg_x = GetIntegerBits(left) + 0.1;
		int bg_y = isOutsideSource ? 0.1 : GetIntegerBits(100.0 - yoff) + 0.1;
		int img_off = 0;
		
		// if attribute count + potential implicit existing is less than 3, it will fit on this rectangle
		int attr_count = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, topboxid, -1, source);

		// center text by making it 0.4 and 0.1 on y
		int bx = bg_x + GetIntegerBits(HUD_ITEMBAK_XF / 2) + 0.3;
		int by = isOutsideSource ? 0.1 : GetIntegerBits(HUDMAX_YF / 2) - yoff;
		
		if(!isOutsideSource)
			by = by - 70.9;
		else
			by += 16.0;
		
		int isubt = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, topboxid, -1, source);
		
		// show item details -- image and stack --- show image to the right side of the box on outside
		SetFont(GetItemImage(GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, topboxid, -1, source)));
		HudMessage(
			s:"A"; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 1 + ITEMID_SKIP, CR_WHITE, 
			!isOutsideSource ? (GetIntegerBits(bg_x) + HUD_ITEMBAK_XF + 0.1) : bx, 
			!isOutsideSource ? bg_y : (GetIntegerBits(by) + 64.2), 
			INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);
		stack = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, topboxid, -1, source);
		if(stack) {
			SetFont("NSMOLFNT");
			HudMessage(d:stack; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 14 + ITEMID_SKIP, CR_GREEN, bx + GetIntegerBits(HUD_ITEMBAK_XF / 2) - 7.2, by + 24.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
		
		// resize to keep ratios
		SetHudSize(HUDTEXTMAX_X, HUDTEXTMAX_Y, 1);
		left = GetHudLeft(HUDTEXTMAX_X);
		bx = ((left + (HUD_ITEMBAK_X * HUDTEXTMAX_X / (2 * HUDMAX_X))) << 16) + 0.4;

		// bg_y is a HUDMAX coordinate and this is a HUDTEXTMAX one, so yoff has to cross the 3/2
		// with it. Subtracting it raw in both spaces slid the text against its own panel as the box
		// moved, which is why the same item looked differently spaced depending on the row.
		by = isOutsideSource ? (ITEMINFO_TEXTTOP + 0.1) : (GetIntegerBits(3 * (100.0 - yoff) / 2 + ITEMINFO_TEXTTOP) + 0.1);

		SetHudClipRect(left + ITEMINFO_CLIPX, (by >> 16), HUD_ITEMBAK_WIDTH, 288, HUD_ITEMBAK_WIDTH);
		DrawInventoryText(topboxid, source, pnum, bx, by, itype, isubt, RPGMENUINVENTORYID, HUD_DII_FIELD_MULT, HUDMAX_X, HUDMAX_Y, bg_x, bg_y, attr_count, !isOutsideSource);
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	}
}

void DrawInventoryText(
	int topboxid, 
	int source, 
	int pnum, 
	int bx, int by, 
	int itype, int isubt, 
	int id_begin, int id_mult, int hx, int hy, 
	int bg_posx, int bg_posy, 
	int attr_count = 0, bool isOutsideSource = false,
	int craftMaterialIdx = -1, int holdTime = INVENTORY_HOLDTIME
)
{
	int i, j;
	int val, temp, lvl;
	int yoff = 0.0;
	bool showModTiers = GetCVar("dnd_detailedmods");
	bool showModTags = GetCVar("dnd_showmodtags");
	bool isUnique = false;

	str tmp_text;
	str unique_creator = "";

	// potential delete of quality in case we hover over an item that doesn't have it, we don't want it lingering!
	DeleteText(id_begin - id_mult * MAX_INVENTORY_BOXES - 18);

	auto item_vsync_data = GetItemVSyncData(DND_SYNCINDEX_STACKABLE);

	SetFont("NSMOLFNT");
	if(IsStackedItem(itype)) {
		if(topboxid != item_vsync_data.topleftboxid || source != item_vsync_data.source) {
			item_vsync_data.isDirty = true;
			item_vsync_data.topleftboxid = topboxid;
			item_vsync_data.source = source;
			item_vsync_data.attr_lines_count = 0;
		}

		temp = isubt + GetInventoryInfoOffset(itype);
		if(item_vsync_data.isDirty) {
			tmp_text = StrParam(s:"\c[Y5]", l:GetInventoryTag(temp), s:"\n\n", l:GetInventoryText(temp));
			item_vsync_data.isDirty = false;
			item_vsync_data.textID = tmp_text;
			item_vsync_data.attr_lines_count = CountNewLinesInText(tmp_text, HUD_ITEMBAK_WIDTH);
		}
		else
			tmp_text = item_vsync_data.textID;

		HudMessage(
			s:tmp_text;
			HUDMSG_PLAIN | HUDMSG_FADEOUT,
			id_begin - id_mult * MAX_INVENTORY_BOXES - 3 + ITEMID_SKIP,
			CR_WHITE, 
			bx, 
			by + 32.0 * (source == DND_SYNC_ITEMSOURCE_FIELD) - 32.0 * isOutsideSource, 
			holdTime, 
			INVENTORY_FADETIME, 
			INVENTORY_INFO_ALPHA
		);
		// stacked items are a title and a paragraph with none of the offsets below, so this one
		// stays on the old segment-per-line rule rather than TextExtentToMids
		DrawItemInfoBackground(id_begin - id_mult * MAX_INVENTORY_BOXES, hx, hy, bg_posx, bg_posy, item_vsync_data.attr_lines_count, -1, holdTime);
		return;
	}
	else {
		item_vsync_data = GetItemVSyncData(DND_SYNCINDEX_ITEM);
		if(topboxid != item_vsync_data.topleftboxid || source != item_vsync_data.source) {
			item_vsync_data.isDirty = true;
			item_vsync_data.topleftboxid = topboxid;
			item_vsync_data.source = source;
		}

		if(craftMaterialIdx != -1) {
			i = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, craftMaterialIdx, -1, source);
			j = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, craftMaterialIdx, -1, source);
			if(i == DND_ITEM_ORB) {
				craftMaterialIdx = CanUseOrb(j, topboxid, itype);
				if(craftMaterialIdx) {
					GetOrbAffectedIds(j, pnum, topboxid, source);
					craftMaterialIdx = i | (j << 16);
				}
				else
					hovered_orb_craft_result.effect_type = -1; // reset if craft fails anyways
			}
			else if(i == DND_ITEM_TOKEN) {
				craftMaterialIdx = CanUseToken(j, topboxid, itype);
			}
			else {
				// reset if its not a crafting item being hovered (ex: item itself being hovered, we dont care)
				craftMaterialIdx = -1;
				hovered_orb_craft_result.effect_type = -1; // reset just in case
			}
		}
		else
			hovered_orb_craft_result.effect_type = -1; // reset just in case
		// after this point, if user made a selection to craft an item, and is hovering a craftable item, craftMaterialIdx will hold if its appropriate to use or not
		// it will also hold a non-zero value to indicate the "tags" of attributes that will be affected

		// this holds charm's tier id
		lvl = GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, topboxid, -1, source);
		
		if(itype == DND_ITEM_CHARM) {
			temp = GetItemTier(lvl);
			HudMessage(s:GetCharmString(temp, CHARMSTR_COLORCODE), l:GetCharmString(temp, CHARMSTR_TIERTAG), s: " ", l:GetItemTagName(DND_ITEM_CHARM, isubt); 
				HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 2 + ITEMID_SKIP, CR_WHITE, bx, by, holdTime, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
			);
		}
		else if(itype <= UNIQUE_BEGIN) {
			HudMessage(s:"\c[Y5]", l:GetItemTagName(itype, isubt); 
				HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 2 + ITEMID_SKIP, CR_WHITE, bx, by, holdTime, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
			);
		}
		else {
			// unique item case
			temp = itype & 0xFFFF;
			itype >>= UNIQUE_BITS;
			--itype;
			isUnique = true;

			unique_creator = GetUniqueCreatorName(temp, itype);
			HudMessage(s:"\c[A1]", l:GetUniqueItemName(temp, itype); HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 2 + ITEMID_SKIP, CR_WHITE, bx, by, holdTime, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
			HudMessage(s:"\c[D1]", l:"DND_ITEM_UNIQUE", s:" ", l:GetItemTagName(temp, isubt); HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 3 + ITEMID_SKIP, CR_WHITE, bx, by + 8.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
		
		val = GetLevel() < lvl ? CR_RED : CR_WHITE;
		HudMessage(l:"DND_LEVEL_HEADER", s:": ", d:lvl; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 4 + ITEMID_SKIP, val, GetIntegerBits(bx - HUD_ITEMBAK_XF / 2 - 36.0) + 0.1, by, holdTime, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);

		// quality -- non-stacking item so
		val = GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, topboxid, -1, source);
		HudMessage(s:"\c[Q2]Q: ", d:val, s:"%";
			HUDMSG_PLAIN | HUDMSG_FADEOUT, 
			id_begin - id_mult * MAX_INVENTORY_BOXES - 18 + ITEMID_SKIP, 
			CR_GREEN, 
			GetIntegerBits(bx + ((2 * HUD_ITEMBAK_X / 3) << 16)) + 8.2, 
			by, 
			holdTime, 
			INVENTORY_FADETIME, 
			INVENTORY_INFO_ALPHA
		);

		// implicit
		if
		(
			item_vsync_data.isDirty || 
			item_vsync_data.last_text_mode != showModTiers ||
			item_vsync_data.last_craft_vals != craftMaterialIdx
		)
		{
			tmp_text = "";
			for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
				temp = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, topboxid, i, source);
				if(temp != -1) {
					tmp_text = StrParam(s:tmp_text,
						s:GetItemAttributeText(
							temp, 
							itype, 
							isubt, 
							GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, topboxid, i, source), 
							0, 
							GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, topboxid, i, source), 
							false, // we dont want detailed info on implicits, it doesn't make much sense currently
							GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, topboxid, i, source), 
							false,
							val
						),
						s:(i != MAX_ITEM_IMPLICITS - 1) && GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, topboxid, i + 1, source) != -1 ? "\n" : ""
					);
				}
			}
			item_vsync_data.implicit_textID = tmp_text;
			// the true count -- this is a text offset at ITEMINFO_LINEH, nothing else reads it now
			item_vsync_data.implicit_lines_count = CountNewLinesInText(tmp_text, HUD_ITEMBAK_WIDTH);
			//Log(s:"implicit lines: ", d:item_vsync_data.implicit_lines_count);
		}
		else
			tmp_text = item_vsync_data.implicit_textID;

		// yoff walks down the block from here on. Every gap it steps over is named, so the panel
		// can be measured off it at the end instead of guessed at from a line count.
		yoff = by + ITEMINFO_LINEHF + ITEMINFO_LINEHF * isUnique;

		if(tmp_text != "") {
			yoff += ITEMINFO_TITLEGAP;
			HudMessage(
				s:tmp_text;
				HUDMSG_PLAIN | HUDMSG_FADEOUT,
				id_begin - id_mult * MAX_INVENTORY_BOXES - 5 + ITEMID_SKIP, CR_WHITE, bx, yoff, holdTime, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
			);

			yoff += ITEMINFO_LINEHF * item_vsync_data.implicit_lines_count;
		}

		yoff += ITEMINFO_SEPGAP;

		SetFont("IMPSEPR");
		HudMessage(
			s:"A";
			HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 6 + ITEMID_SKIP, val, GetIntegerBits(bx) + 0.4, yoff, holdTime, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);

		yoff += ITEMINFO_SEPH + ITEMINFO_ATTRGAP;

		// optimization for the potentially busy section with strparam spam
		if
		(
			item_vsync_data.isDirty || 
			item_vsync_data.last_text_mode != showModTiers ||
			item_vsync_data.last_tag_mode != showModTags ||
			item_vsync_data.last_craft_vals != craftMaterialIdx
		)
		{
			//Log(s:"obtain attrib text");
			tmp_text = "";
			for(j = 0; j < attr_count; ++j) {
				temp = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, topboxid, j, source);
				lvl = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, topboxid, j, source);
				if(isUnique)
					lvl = itype;

				i = 0;
				if(craftMaterialIdx > 0 && hovered_orb_craft_result.count) {
					for(i = 0; i < hovered_orb_craft_result.count; ++i)
						if(hovered_orb_craft_result.id_list[i] == j) {
							i = -1;
							break;
						}
				}
	
				tmp_text = StrParam(s:tmp_text,
					s:GetItemAttributeText(
						temp, 
						itype, 
						isubt, 
						GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, topboxid, j, source), 
						GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, topboxid, j, source), 
						lvl, 
						showModTiers, 
						!isUnique ? -1 : j, 
						GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, topboxid, j, source),
						val,
						i == -1 ? (1 | (hovered_orb_craft_result.effect_type << 8)) : 0
					),
					s:showModTags ? GetModTagText(temp) : "",
					s: j != attr_count - 1 ? "\n" : ""
				);

				if(j == attr_count - 1 && hovered_orb_craft_result.effect_type == DND_ORBEFFECT_NEWATTRIBUTE)
					tmp_text = StrParam(s:tmp_text, s:"\n\ck>>>\t???\t<<<\n");
			}
	
			// corrupted label and seperator -- the blank lines these add are paid for by the
			// measured extent now, so none of them need a hand tuned offset any more
			if(GetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, topboxid, -1, source))
				tmp_text = StrParam(s:tmp_text, s:"\n\n\cgCORRUPTED");

			if(unique_creator != "")
				tmp_text = StrParam(s:tmp_text, s:"\n\n", l:"DND_MADE_BY", s:": \cd", s:unique_creator);

			// this check is important, unique items have itype as the id of the unique at this point and it can just so happen for it to be equal to DND_ITEM_DUNGEONKEY
			if(!isUnique && itype == DND_ITEM_DUNGEONKEY) {
				tmp_text = StrParam(
					s:tmp_text, s:"\n\n\c[K9]", l:StrParam(s:"DND_DUNGEONKEYTEXT", d:isubt + 1),
					s:"\n", s:GetDungeonMonsterTypeString(isubt)
				);
			}

			item_vsync_data.isDirty = false;
			item_vsync_data.last_text_mode = showModTiers;
			item_vsync_data.last_tag_mode = showModTags;
			item_vsync_data.last_craft_vals = craftMaterialIdx;
			item_vsync_data.textID = tmp_text;
			item_vsync_data.attr_lines_count = CountNewLinesInText(tmp_text, HUD_ITEMBAK_WIDTH);
		}
		else
			tmp_text = item_vsync_data.textID;

		SetFont("NSMOLFNT");
		HudMessage(
			s:tmp_text;
			HUDMSG_PLAIN | HUDMSG_FADEOUT,
			id_begin - id_mult * MAX_INVENTORY_BOXES - 7 + ITEMID_SKIP, CR_WHITE, bx, yoff, holdTime, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);

		// how far the whole block reaches past the title row -- that is what the panel has to cover
		yoff += ITEMINFO_LINEHF * item_vsync_data.attr_lines_count + ITEMINFO_BOTTOMPAD - by;

		//log(s:"final lines: ", d:item_vsync_data.lines_count);
	}

	DrawItemInfoBackground(
		id_begin - id_mult * MAX_INVENTORY_BOXES,
		hx, hy, bg_posx, bg_posy,
		TextExtentToMids(yoff),
		craftMaterialIdx, holdTime
	);
}

// text_h is the height of the text block in HUDTEXTMAX pixels, measured from the title row.
int TextExtentToMids(int text_h) {
	text_h = (text_h >> 16) - ITEMINFOBG_CAPTEXTLEN;

	if(text_h <= 0)
		return 0;

	// doubled against the half pixel in a mid, and ceil so the last line always has art under it
	return (2 * text_h + ITEMINFOBG_MIDTEXT2 - 1) / ITEMINFOBG_MIDTEXT2;
}

void DrawItemInfoBackground(
	int hudid_begin, 
	int hx, int hy, 
	int bg_posx, int bg_posy,
	int mids, int craft_status = -1, int holdTime = INVENTORY_HOLDTIME
)
{
	SetHudClipRect(0, 0, 0, 0, 0);

	// finally draw the background -- use hx and hy to set hudsize
	SetHudSize(hx, hy, 1);

	if(mids > ITEMINFOBG_MAXMIDS)
		mids = ITEMINFOBG_MAXMIDS;

	str mid_img = "LDTITMID";
	if(craft_status) {
		if(hovered_orb_craft_result.effect_type != DND_ORBEFFECT_ENTIREITEM)
			SetFont("LDTITTOP");
		else {
			SetFont("LDTITTOO");
			mid_img = "LDTITMIO";
		}
	}
	else {
		SetFont("LDTITTOR");
		mid_img = "LDTITMIR";
	}
	HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_FADEOUT, hudid_begin, CR_WHITE, bg_posx, bg_posy, holdTime, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);

	for(int i = 0; i < mids; ++i) {
		SetFont(mid_img);
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_FADEOUT, hudid_begin - 1 - i, CR_WHITE, bg_posx, bg_posy + ITEMINFOBG_TOPLEN + i * ITEMINFOBG_MIDLEN, holdTime, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
	}

	if(craft_status) {
		if(hovered_orb_craft_result.effect_type != DND_ORBEFFECT_ENTIREITEM)
			SetFont("LDTITBOT");
		else
			SetFont("LDTITBOO");
	}
	else
		SetFont("LDTITBOR");
	HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_FADEOUT, hudid_begin - 2 - i, CR_WHITE, bg_posx, bg_posy + ITEMINFOBG_TOPLEN + i * ITEMINFOBG_MIDLEN, holdTime, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
}

void CopyItemToField(int fieldpos, int player_index, int item_index, int source) {
	int i, wtemp;
	wtemp = GetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, source) - 1;

	auto item_from = AcquireItemFromSource(player_index, wtemp, source);
	auto item_to = GetFieldItem(fieldpos);

	SetItemToAnother(item_to, item_from);
	item_to.topleftboxid = 0;

	// the leftover spot is a null item
	FreeItem(player_index, wtemp, source, false);
	SyncItemData_Field(fieldpos);
}

void DropItemToField(int player_index, int pitem_index, bool forAll, int source) {
	int c = CreateItemSpot();
	
	// note to self here: subtype should always be looked up on TOPLEFTBOX property... that one index only has the fully synced info, rest dont
	int itype = GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, pitem_index, -1, source);
	int topboxid = GetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, pitem_index, -1, source) - 1;
	int stype = GetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, topboxid, -1, source);

	// copy now
	CopyItemToField(c, player_index, pitem_index, source);
	str droptype = "CharmDrop";
	if(itype > UNIQUE_BEGIN) {
		int utype = (itype >> 16) - 1;
		itype &= 0xFFFF;
		if(itype == DND_ITEM_CHARM)
			droptype = "UniqueCharmDrop";
		else if(itype == DND_ITEM_BODYARMOR)
			droptype = StrParam(s:"UniqueArmor_", d:utype - UNIQUE_BODYARMOR_BEGIN);
		else if(itype == DND_ITEM_BOOT)
			droptype = StrParam(s:"UniqueBoot_", d:utype - UNIQUE_BOOT_BEGIN);
		else if(itype == DND_ITEM_HELM)
			droptype = StrParam(s:"UniqueHelm_", d:utype - UNIQUE_HELM_BEGIN);
		else if(itype == DND_ITEM_SPECIALTY_CYBORG)
			droptype = StrParam(s:"PowercoreDrop_Unique", d:utype - UNIQUE_POWERCORE_BEGIN);
	}
	else if(itype == DND_ITEM_ORB)
		droptype = GetInventoryName(stype + ORBS_BEGIN);
	else if(itype == DND_ITEM_CHESTKEY)
		droptype = GetInventoryName(stype + CHESTKEY_BEGIN);
	else if(itype == DND_ITEM_TOKEN)
		droptype = GetInventoryName(stype + TOKEN_BEGIN);
	else if(itype == DND_ITEM_BODYARMOR)
		droptype = GetArmorDropClass(stype);
	else if(itype == DND_ITEM_BOOT)
		droptype = GetBootDropClass(stype);
	else if(itype == DND_ITEM_HELM)
		droptype = GetHelmDropClass(stype);
	else if(IsSpecialtyItemType(itype))
		droptype = GetSpecialtyDropClass(itype, stype);
	else if(itype == DND_ITEM_FLASK)
		droptype = GetFlaskDropClass(stype);
	else if(itype == DND_ITEM_DUNGEONKEY)
		droptype = GetInventoryName(stype + DUNGEONKEY_BEGIN);
	forAll ? SpawnDropFacing(droptype, 16.0, 16, 256, c) : SpawnDropFacing(droptype, 16.0, 16, player_index + 1, c);
}

void StackedItemPickupCS(int item_index, int type) {
	if(type == DND_STACKEDITEM_ORB)
		ACS_NamedExecuteAlways("DnD Orb Message", 0, GlobalItemStorage.Inventories_On_Field[item_index].item_subtype);
	else if(type == DND_STACKEDITEM_CHESTKEY)
		ACS_NamedExecuteAlways("DnD Chestkey Message", 0, GlobalItemStorage.Inventories_On_Field[item_index].item_subtype);
	else if(type == DND_STACKEDITEM_TOKEN)
		ACS_NamedExecuteAlways("DnD Token Message", 0, GlobalItemStorage.Inventories_On_Field[item_index].item_subtype);
	else if(type == DND_STACKEDITEM_DUNGEONKEY)
		ACS_NamedExecuteAlways("DnD Dungeon Key Message", 0, GlobalItemStorage.Inventories_On_Field[item_index].item_subtype);
}

// move this from field to player's inventory
int HandleStackedPickup(int item_index, int type) {
	// make sure this item actually gets placed on top of an item that has some stack, if any
	// print message first, if its moved to inv it's gone from there
	int pnum = PlayerNumber();
	StackedItemPickupCS(item_index, type);
	
	int porb_index = GetFreeSpotForItemWithStack(item_index, pnum, DND_SYNC_ITEMSOURCE_FIELD, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
	return CopyItemFromFieldToPlayer(item_index, pnum, porb_index, type);
}

// checks players inventory for the given item precisely with its subtype matching
int CheckPlayerInventoryList(int pnum, int itemtype, int subtype) {
	int i;
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto item = GetPlayerInventoryItem(pnum, i);
		if(item.item_type == itemtype && item.item_subtype == subtype)
			return i;
	}
	return -1;
}

bool IsTwoSelectionItem(int pnum, int item_index) {
	auto item = GetPlayerInventoryItem(pnum, item_index);
	return item.item_type == DND_ITEM_ORB && item.item_subtype == DND_ORB_ASSIMILATION;
}

// can only use items in inventory
// returns true if item expired after use
bool UsePlayerItem(int pnum, int item_index, bool countTokens) {
	auto item = GetPlayerInventoryItem(pnum, item_index);
	if(IsUsableItem(item.item_type)) {
		GiveInventory("DnD_RefreshPane", 1);
		--item.item_stack;
		if(item.item_stack) {
			SyncItemStack(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			return false;
		}
		FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
		GiveInventory("DnD_CleanCraftingRequest", 1);
		// if this is freed, we might have to auto-adjust the player's page -- if theres nothing left on this page go back
		if(!CountCraftingMaterials(countTokens))
			TakeInventory("DnD_Crafting_MaterialPage", 1);
		return true;
	}
	return false;
}

// Consumes a stack off a stackable item
void ConsumePlayerItem(int pnum, int item_index) {
	auto item = GetPlayerInventoryItem(pnum, item_index);
	if(IsStackedItem(item.item_type)) {
		GiveInventory("DnD_RefreshPane", 1);
		--item.item_stack;
		if(item.item_stack)
			SyncItemStack(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		else
			FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	}
}

void UsePlayerItem_Count(int pnum, int item_index, int count) {
	auto item = GetPlayerInventoryItem(pnum, item_index);
	item.item_stack = Clamp_Between(item.item_stack - count, 0, item.item_stack);
	if(item.item_stack)
		SyncItemStack(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	else {
		FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
		GiveInventory("DnD_RefreshPane", 1);
	}
}

// uses items from stash (needed for certain functions)
void UsePlayerStashItem_Count(int pnum, int page, int item_index, int count) {
	auto item = GetPlayerStashItem(pnum, page, item_index);
	item.item_stack = Clamp_Between(item.item_stack - count, 0, item.item_stack);
	if(item.item_stack)
		SyncItemStack(pnum, item_index, DND_SYNC_ITEMSOURCE_STASH | (page << 16));
	else
		FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_STASH | (page << 16), false);
		GiveInventory("DnD_RefreshPane", 1);
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
		case DND_ITEM_WEAPON:
		case DND_ITEM_BODYARMOR:
		case DND_ITEM_FLASK:
		case DND_ITEM_DUNGEONKEY:
		case DND_ITEM_SPECIALTY_DOOMGUY:
		case DND_ITEM_SPECIALTY_MARINE:
		case DND_ITEM_SPECIALTY_HOBO:
		case DND_ITEM_SPECIALTY_PUNISHER:
		case DND_ITEM_SPECIALTY_WANDERER:
		case DND_ITEM_SPECIALTY_CYBORG:
		case DND_ITEM_SPECIALTY_BERSERKER:
		case DND_ITEM_SPECIALTY_TRICKSTER:
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
		case DND_ITEM_BODYARMOR:
		case DND_ITEM_BOOT:
		case DND_ITEM_HELM:
		case DND_ITEM_FLASK:
		case DND_ITEM_DUNGEONKEY:
		case DND_ITEM_SPECIALTY_DOOMGUY:
		case DND_ITEM_SPECIALTY_MARINE:
		case DND_ITEM_SPECIALTY_HOBO:
		case DND_ITEM_SPECIALTY_PUNISHER:
		case DND_ITEM_SPECIALTY_WANDERER:
		case DND_ITEM_SPECIALTY_CYBORG:
		case DND_ITEM_SPECIALTY_BERSERKER:
		case DND_ITEM_SPECIALTY_TRICKSTER:
		return true;
	}
	return false;
}

bool IsInventoryCorrupted(int pnum, int item_id) {
	return GlobalItemStorage.PlayerInventoryList[pnum][item_id].corrupted;
}

// will count crafting materials the player has currently in their inventory
// this populates an array of fixed order crafting materials for use
int CountCraftingMaterials(bool countTokens) {
	int pnum = PlayerNumber();
	int res = 0;
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto item = GetPlayerInventoryItem(pnum, i);
		if(IsCraftingItem(item.item_type)) {
			if(item.item_type == DND_ITEM_ORB && UniqueCraftingItemList[item.item_subtype] == -1) {
				++res;
				UniqueCraftingItemList[item.item_subtype] = i;
			}
			else if(countTokens && item.item_type == DND_ITEM_TOKEN && UniqueCraftingItemList[DND_MAX_ORB_KINDS + item.item_subtype] == -1) {
				++res;
				UniqueCraftingItemList[DND_MAX_ORB_KINDS + item.item_subtype] = i;
			}
		}
	}
	return res;
}

// problem is this: say 8 orbs 5 elixirs, first page draws 12, next page requires next current is 12
// but this counts per item type, and per item type count will never make it to 12...
// soln: just count everything regardless, level order in inventory
int GetNextUniqueCraftableMaterial(int current, bool countTokens) {
	int pnum = PlayerNumber();
	int res = 0;
	int lim = DND_MAX_ORB_KINDS + countTokens * DND_MAX_TOKEN_KINDS;
	for(int i = 0; i < lim; ++i) {
		if(UniqueCraftingItemList[i] != -1) {
			++res;
			// return the item's index
			if(res > current)
				return UniqueCraftingItemList[i];
		}
	}
	return -1;
}

int GetTotalStackOfMaterial(int itemid) {
	int pnum = PlayerNumber();
	int res = 0;
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto item = GetPlayerInventoryItem(pnum, i);
		auto item_comp = GetPlayerInventoryItem(pnum, itemid);
		if(item.item_type == item_comp.item_type && item.item_subtype == item_comp.item_subtype)
			res += item.item_stack;
	}
	return res;
}

bool IsSelfUsableItem(int itype, int isubtype) {
	switch(itype) {
		// do all exceptions in here
		case DND_ITEM_ORB:
			switch(isubtype) {
				case DND_ORB_REPENT:
				case DND_ORB_AFFLUENCE:
				case DND_ORB_CALAMITY:
				case DND_ORB_ASSIMILATION:
				case DND_ORB_DESTINY:
				case DND_ORB_ORDER:
				case DND_ORB_REVERANCE:
				return true;
			}
		return false;
		case DND_ITEM_TOKEN:
		return false;
	}
	return false;
}

// standard attribute processing (not implicits)
void ProcessAttribute(int pnum, int atype, int aval, int aextra, int item_index, bool remove) {
	int i, temp;
	int cap;

	switch(atype) {
		// this is handled differently
		case INV_CYBERNETIC:
		break;
	
		// first cases with exceptions to our generic formula
		case INV_MAGAZINE_INCREASE:
			IncPlayerModValue(pnum, atype, aval);
			// add onto the base capacities, not current capacities
			for(i = 0; i < DND_MAX_MAGAZINES; ++i)
				SetAmmoCapacity(WeaponMagazineList[i], GetMagazineCap(pnum, i));
		break;
		case INV_WEAPONAOE_RADIUS:
		case INV_ALLAOE_RADIUS:
			IncPlayerModValue(pnum, atype, aval);
			SetActorProperty(0, APROP_SCORE, GetPlayerAoEIncrease(pnum, DND_AOESRC_WEAPON));
		break;
		
		// these are all accuracy mod groups
		case INV_ACCURACY_INCREASE:
		case INV_ESS_OMNISIGHT:
		case INV_ESS_OMNISIGHT2:
			IncPlayerModValue(pnum, atype, aval);
		break;
		
		// exotic stuff -- reason most of these dont have syncs is that they arent meant to be shown in stat menu page, so no need for client to be aware
		case INV_EX_FORSHOW_BURSTGETSPELLETBONUS:
			// by itself these do nothing
		break;

		// things that have EXTRA field used!
		case INV_EX_CHANCE_HEALMISSINGONPAIN:
		case INV_EX_SECONDEXPBONUS:
		case INV_ESS_VAAJ:
		case INV_INC_PASSIVEREGEN:
		case INV_INC_INSTANTLIFESTEAL:
			IncPlayerModValue(pnum, atype, aval);
			IncPlayerModExtra(pnum, atype, aextra);
		break;
		// this one is like the above but only has ONE (maximum) value as the real value
		case INV_INC_MOREHPBONUS:
			// Highest source wins rather than the sources adding up, so this cannot go through
			// IncPlayerModValue: taking one off has to fall back to the next highest instead of
			// subtracting, which means the individual contributions have to be tracked. Keyed on the
			// slot, so a remove does not have to recognise the value it added -- see the Well of
			// Power note on SetHighestModSource. aval is ignored entirely on a remove, which also
			// sidesteps the fact that it arrives negated.
			SetHighestModSource(pnum, atype, item_index, remove ? 0 : aval);

			// the flat health still stacks
			IncPlayerModExtra(pnum, atype, aextra);
		break;

		// same highest-source-only rule, no extra field on this one
		case INV_INC_CRITFORDOT:
			// The value is read as both the switch (GetCritModifier zeroing crit damage, and the
			// ailment gate in "DnD Damage Accumulate") and the magnitude the crit damage is handed to
			// the DoT at, so a second copy must be able to raise the magnitude but never add to it.
			SetHighestModSource(pnum, atype, item_index, remove ? 0 : aval);
		break;

		// Stored as a flag now, but it keeps the case it always had -- the recompute below is the
		// whole reason this mod was not on the generic accumulate path to begin with.
		case INV_EX_KNOCKBACK_IMMUNITY:
			SetPlayerFlag(pnum, PFLAG_KNOCKBACK_IMMUNITY, remove);
			UpdatePlayerKnockbackResist();
		break;

		case INV_EX_BEHAVIOR_PELLETSFIRECIRCLE:
			SetPlayerFlag(pnum, PFLAG_PELLETS_FIRE_CIRCLE, remove);
		break;
		case INV_EX_FACTOR_SMALLCHARM:
			if(!remove) {
				// we now need to re-apply all other features of small charms we have equipped
				// first 4 are small charms
				for(i = 0; i < 4; ++i)
					if(GlobalItemStorage.Items_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_REMOVE, true);
				
				// now give the item and re-apply
				IncPlayerModValue(pnum, atype, aval);
								
				for(i = 0; i < 4; ++i)
					if(GlobalItemStorage.Items_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_ADD);
								
			}
			else if(ReadPlayerModValue(pnum, atype)) {
				// just take the attribute off and remove features and reapply
				for(i = 0; i < 4; ++i)
					if(GlobalItemStorage.Items_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_REMOVE, true);
										
				// little note: aval can be negative if we are removing, so just + is enough to subtract it
				IncPlayerModValue(pnum, atype, aval);
								
				// reapply with this gone
				for(i = 0; i < 4; ++i)
					if(GlobalItemStorage.Items_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_ADD);
			}
		break;
		case INV_EX_MIRROROTHERMEDIUM:
			// reapply the other medium charm
			// find the index of the other medium charm
			temp = -1;
			for(i = 0; i < 2; ++i) {
				if
				(
					GlobalItemStorage.Items_Used[pnum][i + MEDIUMCHARM_INDEX1].item_type != DND_ITEM_NULL &&
					(GlobalItemStorage.Items_Used[pnum][i + MEDIUMCHARM_INDEX1].item_type >> 16) - 1 != UITEM_MIRROROFETERNITY
				)
				{
					// this holds the other charm's index
					temp = i + MEDIUMCHARM_INDEX1;
					break;
				}
			}

			// we got a valid index
			if(temp != -1) {
				ApplyItemFeatures(pnum, temp, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_REMOVE);
				IncPlayerModValue(pnum, atype, aval);
				ApplyItemFeatures(pnum, temp, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_ADD);
				SyncPlayerItemMods(pnum);
			}
			else // just give the thing now otherwise
				IncPlayerModValue(pnum, atype, aval);
		break;
		case INV_EX_ALLSTATS:
			for(i = INV_STAT_STRENGTH; i <= INV_STAT_INTELLECT; ++i)
				IncPlayerModValue(pnum, i, aval);
			
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

			HandleEShieldChange(pnum, remove);
			CalculateUnity(pnum);
		break;
		case INV_EX_PHYSDAMAGEPER_FLATHEALTH:
			// first check all sources, see if they contain this and are lower than this source
			temp = FindMinOnUsedCharmsForAttribute(pnum, INV_EX_PHYSDAMAGEPER_FLATHEALTH, item_index);
			// we got a new min
			if(temp != -1) {
				// update to use the new min if our comparison is better -- dont care otherwise
				if(!remove)
					SetPlayerModValue(pnum, atype, Min(aval, ReadPlayerModValue(pnum, atype)));
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
			if(ReadPlayerModValue(pnum, atype))
				GiveInventory("CastRally", 1);
			else
				TakeInventory("CastRally", 1);
		break;
		case INV_HP_INCREASE:
		case INV_HPPERCENT_INCREASE:
			temp = GetSpawnHealth();
			i = GetActorProperty(0, APROP_HEALTH) - temp;
			IncPlayerModValue(pnum, atype, aval);
			if(remove) {
				if(GetActorProperty(0, APROP_HEALTH) > temp) {
					// set health to new cap, add the extra to player
					if(i > 0)
						SetActorProperty(0, APROP_HEALTH, temp + i);
					else
						SetActorProperty(0, APROP_HEALTH, temp);
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
			SetAllAmmoCapacities(pnum);
		break;
		case INV_INCKILLINGSPREE:
			IncPlayerModValue(pnum, atype, aval);
			// make sure to update ammo caps
			UpdatePlayerSpreeTimer(pnum);
		break;
		case INV_EX_CURSEIMMUNITY:
			IncPlayerModValue(pnum, atype, aval);
			if(ReadPlayerModValue(pnum, atype))
				GiveInventory("CurseImmunity", 1);
			else
				HandleCurseImmunityRemoval();
		break;
		case INV_STAT_STRENGTH:
			IncPlayerModValue(pnum, atype, aval);
			UpdatePlayerKnockbackResist();

			temp = GetSpawnHealth();
			i = GetActorProperty(0, APROP_HEALTH) - temp;
			if(remove) {
				if(GetActorProperty(0, APROP_HEALTH) > temp) {
					// set health to new cap, add the extra to player
					if(i > 0)
						SetActorProperty(0, APROP_HEALTH, temp + i);
					else
						SetActorProperty(0, APROP_HEALTH, temp);
				}
			}
			CalculateUnity(pnum);
		break;
		case INV_STAT_DEXTERITY:
			IncPlayerModValue(pnum, atype, aval);
			CalculateUnity(pnum);
		break;
		case INV_STAT_INTELLECT:
			IncPlayerModValue(pnum, atype, aval);
			HandleEShieldChange(pnum, remove);
			CalculateUnity(pnum);
		break;

		case INV_SHIELD_INCREASE:
		case INV_PERCENTSHIELD_INCREASE:
			IncPlayerModValue(pnum, atype, aval);

			HandleEShieldChange(pnum, remove);
		break;

		case INV_EX_REDUCEDAMMOCAP:
			IncPlayerModValue(pnum, atype, aval);
			SetAllAmmoCapacities(pnum);
		break;

		case INV_EX_HEALTHATONE:
			IncPlayerModValue(pnum, atype, aval);

			// Recompute instead of assuming: on the way in this is 1, on the way out it is the real
			// cap again. APROP_SPAWNHEALTH is the part that was missing -- nothing writes it when an
			// item is equipped. It is only set on character load, on respawn and on a strength
			// change, so equipping this mid-map left the engine still handing out the pre-armor
			// maximum, and the player kept their old health capacity until one of those three
			// happened to fire. GetSpawnHealth refreshes PlayerHealthCap on the way past too.
			temp = GetSpawnHealth();
			SetActorProperty(0, APROP_SPAWNHEALTH, temp);

			if(!remove)
				SetActorProperty(0, APROP_HEALTH, 1);
			else if(GetActorProperty(0, APROP_HEALTH) > temp) {
				// taking the armor off restores the cap, so only clamp down -- the 1 hp the player
				// is sitting on is theirs to heal back. this used to pin them at 1 permanently by
				// running the set above on removal as well.
				SetActorProperty(0, APROP_HEALTH, temp);
			}
		break;

		case INV_EX_UNITY:
			IncPlayerModValue(pnum, atype, aval);

			// for str
			UpdatePlayerKnockbackResist();

			// for str
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

			// for int
			HandleEShieldChange(pnum, remove);
		break;

		case INV_INC_PLUSPROJ:
		case INV_INC_PLUSTWOPROJ:
			// make sure this stays positive even for removal!!!
			if(remove) {
				temp = -(atype - INV_INC_PLUSPROJ + 1);
				aval = -(1.0 + aval);
			}
			else {
				temp = (atype - INV_INC_PLUSPROJ + 1);
				aval = 1.0 - aval;
			}

			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_DMG][WMOD_ITEMS].val = HandleMultiplicativeFactors(Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_DMG][WMOD_ITEMS].val, aval);
			
			// this will add 1 or two depending on the mod type in question
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_EXTRAPROJ][WMOD_ITEMS].val += temp;

			//Log(s:"extra proj is now: ", d:Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_EXTRAPROJ][WMOD_ITEMS].val);

			MarkWeaponDataSync(pnum, aextra, true);
		break;

		case INV_CRITPERCENT_FORWEPTYPE:
			for(i = 0; i < MAXWEPS; ++i ) {
				if(IsWeaponType(i, aextra)) {
					Player_Weapon_Infos[pnum][i].wep_mods[WEP_MOD_CRITPERCENT][WMOD_ITEMS].val += aval;
					MarkWeaponDataSync(pnum, i, true);
				}
			}
		break;

		case INV_INC_STAMINA:
			IncPlayerModValue(pnum, atype, aval);
			SetAmmoCapacity("DnD_Stamina", DND_BASE_STAMINA * (100 + PlayerModData[pnum].vals[PSTAT_INC_STAMINA]) / 100);
			ACS_NamedExecuteWithResult("DnD Start Stamina Recovery");
		break;

		case INV_EX_COUNTASHAVINGMAXCHARGEOF:
			// even though this has attribute extra exception, should still negate it here!
			IncPlayerModValue(pnum, atype, aval);
			IncPlayerModExtra(pnum, atype, remove ? -aextra : aextra);

			if(remove)
				RemoveBuffWithTableIndex(pnum, BTI_FRENZYCHARGE + aextra);
			else
				HandlePlayerBuffAssignment(pnum, 0, BTI_FRENZYCHARGE + aextra, 0);
		break;
		
		// Stored as a flag, not a value -- every one of its read sites tests it for truth, four of
		// them inside GetResistPenetration on the damage path. MapAttributeToPFlag keeps it readable
		// by id so the exotic stat page still lists it.
		//
		// It belongs HERE and not in ProcessItemImplicit: that one only ever sees INV_IMP_* ids, so a
		// case for a unique attribute placed there compiles, reads correctly, and never once runs.
		case INV_EX_ELEPENHARMONY:
			SetPlayerFlag(pnum, PFLAG_ELEPENHARMONY, remove);
		break;

		// Same treatment, same reason: 1..1 rolls tested for truth at every site.
		case INV_INC_MITIGATIONTODODGE:
			SetPlayerFlag(pnum, PFLAG_MITIGATION_TO_DODGE, remove);
		break;

		case INV_INC_ESHIELDNOINTERRUPT:
			SetPlayerFlag(pnum, PFLAG_ESHIELD_NOINTERRUPT, remove);
		break;

		case INV_EX_ESCHARGE_DMGNOINTERRUPT:
			SetPlayerFlag(pnum, PFLAG_ESCHARGE_NOINTERRUPT, remove);
		break;

		case INV_EX_ABILITY_LUCKYCRIT:
			SetPlayerFlag(pnum, PFLAG_LUCKYCRIT, remove);
		break;

		// Wanderlust and Cinderstep. Here and not in ProcessItemImplicit, for the reason given on
		// INV_EX_ELEPENHARMONY above: that one only ever sees INV_IMP_* ids, so a unique attribute
		// cased there would compile, read correctly, and never once run.
		case INV_EX_CANNOTBEFROZEN:
			SetPlayerFlag(pnum, PFLAG_CANNOTBEFROZEN, remove);
		break;

		case INV_EX_CANNOTBEIGNITED:
			SetPlayerFlag(pnum, PFLAG_CANNOTBEIGNITED, remove);
		break;

		// Choir of Ashes.
		case INV_EX_IGNITE_CHAINS_ONSPREAD:
			SetPlayerFlag(pnum, PFLAG_IGNITE_CHAINS, remove);
		break;

		case INV_EX_IGNITE_CANNOT_REFRESH:
			SetPlayerFlag(pnum, PFLAG_IGNITE_NOREFRESH, remove);
		break;

		// Sightless Vigil.
		case INV_EX_ACCURACY_NOSPREAD:
			SetPlayerFlag(pnum, PFLAG_ACCURACY_NOSPREAD, remove);
		break;

		// Crown of Suffering.
		case INV_EX_POISON_SHAREDSTACKS:
			SetPlayerFlag(pnum, PFLAG_POISON_SHAREDSTACKS, remove);
		break;


		case INV_EX_POISON_CLEARS_ONKILL:
			SetPlayerFlag(pnum, PFLAG_POISON_CLEARONKILL, remove);
		break;

		// Faraday Halo.
		case INV_EX_OVERLOAD_CHAINS_TOSELF:
			SetPlayerFlag(pnum, PFLAG_OVERLOAD_CHAINSTOSELF, remove);
		break;

		case INV_EX_OVERLOAD_ONANYELEMENT:
			SetPlayerFlag(pnum, PFLAG_OVERLOAD_ANYELEMENT, remove);
		break;

		// Vaultstride.
		case INV_EX_DASH_RESETS_ONKILL:
			SetPlayerFlag(pnum, PFLAG_DASH_KILLREFUND, remove);
		break;

		// Undertow.
		case INV_EX_DASH_ANCHOR:
			SetPlayerFlag(pnum, PFLAG_DASH_ANCHOR, remove);
		break;

		// Emberwake.
		case INV_EX_TRAIL_SCALES_WITHSPEED:
			SetPlayerFlag(pnum, PFLAG_TRAIL_SPEEDSCALES, remove);
		break;
		case INV_EX_BURNS_WHILE_STILL:
			SetPlayerFlag(pnum, PFLAG_TRAIL_BURNSSTILL, remove);
		break;

		// Feeds the same conversion table the corruption mod does, so the ladder, Avatar of Fire and
		// every fire multiplier pick it up for free. Two rungs because physical is two categories --
		// one attribute value covering both is the whole point of not reusing INV_CORR_DAMAGECONVERSION,
		// which names a single pair in its extra. aval is already negated on removal.
		case INV_EX_CONVERT_PHYSTOFIRE:
			IncPlayerConversionPercent(pnum, DND_DAMAGECATEGORY_MELEE, DND_DAMAGECATEGORY_FIRE, aval);
			IncPlayerConversionPercent(pnum, DND_DAMAGECATEGORY_BULLET, DND_DAMAGECATEGORY_FIRE, aval);
		break;

		case INV_INC_ACCURACYREVERSED:
			SetPlayerFlag(pnum, PFLAG_ACCURACY_REVERSED, remove);
		break;

		// anything that fits our generic formula
		default:
			IncPlayerModValue(pnum, atype, aval);
		break;
	}
}

void ProcessItemFeature(int pnum, int item_index, int source, int aindex, bool remove, int multiplier = 100) {
	int atype = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, aindex, source);
	int aval = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, aindex, source);
	int aextra = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, aindex, source);

	if(multiplier != 100) {
		if(!IsAttributeQualityException(atype)) {
			if(aval > bcs::INT_MAX / multiplier) {
				aval /= 100;
				aval *= multiplier;
			}
			else {
				aval *= multiplier;
				aval /= 100;
			}
		}

		if(!IsAttributeExtraException(atype)) {
			if(atype != INV_INC_PLUSPROJ && atype != INV_INC_PLUSTWOPROJ) {
				if(aextra > 100000) {
					aextra /= 100;
					aextra *= multiplier;
				}
				else {
					aextra *= multiplier;
					aextra /= 100;
				}
			}
			else {
				// these two have special treatment, only the first 16 bits scale
				int temp = aextra & 0xFFFF;
				aextra &= 0xFFFF0000;
				temp = 1.0 - (1.0 - temp) * multiplier / 100;
				aextra |= temp;
			}
		}
	}
	
	if(remove) {
		aval = -aval;
		if(!IsAttributeExtraException(atype))
			aextra = -aextra;
	}
	
	ProcessAttribute(pnum, atype, aval, aextra, item_index, remove);
}

bool ItemIsCybernetic(int pnum, int item_index, int attrib_count, int source) {
	int i;
	for(i = 0; i < attrib_count; ++i) {
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, i, source) == INV_CYBERNETIC) {
			return true;
		}
	}

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		attrib_count = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, i, source);
		if(attrib_count == INV_CORR_CYBERNETIC || attrib_count == INV_CYBERNETIC)
			return true;
	}
	return false;
}

// SetPlayerFlag carries a source count per flag, so the bit only clears once the LAST item granting
// it comes off. Plain set/clear cannot tell "two equipped items grant Cyber" from "one does", and
// would strip the power off a player still wearing the other one.
void HandleAttributePowerset(int pnum, int flag, bool remove) {
	SetPlayerFlag(pnum, flag, remove);
}

bool ProcessItemImplicit(int pnum, int item_index, int source, int implicit_id, bool remove, int multiplier = 100) {
	int atype = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, implicit_id, source);
	if(atype == -1)
		return false;

	int aval = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, implicit_id, source);
	int aextra = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, implicit_id, source);

	int i, temp;

	if(multiplier != 100) {
		if(!IsAttributeQualityException(atype)) {
			if(aval > bcs::INT_MAX / multiplier) {
				aval /= 100;
				aval *= multiplier;
			}
			else {
				aval *= multiplier;
				aval /= 100;
			}
		}

		if(!IsAttributeExtraException(atype)) {
			if(aextra > 100000) {
				aextra /= 100;
				aextra *= multiplier;
			}
			else {
				aextra *= multiplier;
				aextra /= 100;
			}
		}
	}
	
	if(remove) {
		aval = -aval;
		if(!IsAttributeExtraException(atype))
			aextra = -aextra;
	}

	// delegate regular attribute as implicit responsibility to this function instead
	if(atype < FIRST_CORRUPT_IMPLICIT || atype > LAST_REGULAR_IMPLICIT) {
		ProcessAttribute(pnum, atype, aval, aextra, item_index, remove);
		return false;
	}

	switch(atype) {
		// standard implicits
		case INV_IMP_INCARMOR:
			IncPlayerModValue(pnum, INV_ARMOR_INCREASE, aval);
		break;
		case INV_IMP_INCSHIELD:
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval);

			HandleEShieldChange(pnum, remove);
		break;
		case INV_IMP_INCMIT:
			//Log(s:"process mit ", f:aval, s:" player ", n:pnum + 1);
			IncPlayerModValue(pnum, INV_MIT_INCREASE, aval);
		break;
		case INV_IMP_INCARMORSHIELD:
			IncPlayerModValue(pnum, INV_ARMOR_INCREASE, aval);
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval);

			HandleEShieldChange(pnum, remove);
		break;
		case INV_IMP_INCMITSHIELD:
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval);
			IncPlayerModValue(pnum, INV_MIT_INCREASE, ((aval << 16) / DND_SHIELD_TO_MIT_RATIO));
			
			HandleEShieldChange(pnum, remove);
		break;
		case INV_IMP_INCMITARMOR:
			IncPlayerModValue(pnum, INV_ARMOR_INCREASE, aval);
			IncPlayerModValue(pnum, INV_MIT_INCREASE, ((aval << 16) / DND_ARMOR_TO_MIT_RATIO));
		break;
		case INV_IMP_POWERCORE:
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval);
			IncPlayerModValue(pnum, INV_MAGIC_NEGATION, aextra);
			
			HandleEShieldChange(pnum, remove);
		break;
		case INV_IMP_UNSTABLECORE:
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval);
			IncPlayerModExtra(pnum, INV_IMP_UNSTABLECORE, aextra);

			HandleEShieldChange(pnum, remove);
		break;
		case INV_IMP_INCMITARMORSHIELD:
			IncPlayerModValue(pnum, INV_ARMOR_INCREASE, aval);
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval);
			IncPlayerModValue(pnum, INV_MIT_INCREASE, ((aval << 16) / DND_ARMOR_TO_MIT_RATIO));
		break;

		// value and extra implicits
		case INV_IMP_RAVAGER:
		case INV_IMP_KNIGHTARMOR:
		case INV_IMP_ABSORBLIGHTNING:
		case INV_IMP_STAMINAONKILL:
			IncPlayerModValue(pnum, atype, aval);
			IncPlayerModExtra(pnum, atype, aextra);
		break;

		// powerset things
		case INV_IMP_DOUBLEESHIELDRECOVERY:
			HandleAttributePowerset(pnum, PFLAG_CYBER, remove);
		break;

		// single value implicits
		case INV_IMP_HANDGUNBONUS:
		case INV_IMP_NECROARMOR:
		case INV_IMP_LESSLIGHTNINGTAKEN:
		case INV_IMP_LESSPOISONTAKEN:
		case INV_IMP_LESSFIRETAKEN:
		case INV_IMP_LESSSELFDAMAGETAKEN:
		case INV_IMP_FASTEROVERHEATDISS:
		case INV_IMP_BONUSPETCAP:
		case INV_IMP_REDUCEDVISIONIMPAIR:
		case INV_IMP_MOREDAMAGETOBOSSES:
		case INV_IMP_RECOVERESONUNDEADKILL:
		case INV_IMP_PRECISIONCRITBONUS:
			IncPlayerModValue(pnum, atype, aval);
		break;
		
		// for these, do nothing, they are unique to the item itself (not player) and will be checked on the item itself, not player
		case INV_IMP_CANROLL_PHYS:
		case INV_IMP_CANROLL_MAGIC:
		case INV_IMP_CANROLL_EXPLOSIVE:
		case INV_IMP_CANROLL_ENERGY:
		case INV_IMP_CANROLL_ELEMENTAL:
		case INV_IMP_QUALITYCAPFIFTY:
		break;

		case INV_IMP_HIGHREFLECTREDUCE:
			HandleAttributePowerset(pnum, PFLAG_LOWERREFLECT, remove);
		break;

		case INV_IMP_ESHIELDBLOCKSALL:
			HandleAttributePowerset(pnum, PFLAG_ESHIELDBLOCKALL, remove);
		break;
		
		case INV_IMP_MELEEIGNORESSHIELDS:
			HandleAttributePowerset(pnum, PFLAG_MELEEIGNORESHIELD, remove);
		break;

		// corrupted implicits
		// non-weapon mods
		case INV_CORR_SPEED:
			IncPlayerModValue(pnum, INV_SPEED_INCREASE, aval);
			SetActorProperty(0, APROP_SPEED, GetPlayerSpeed(pnum));
		break;
		case INV_CORR_DROPCHANCE:
			IncPlayerModValue(pnum, INV_DROPCHANCE_INCREASE, aval);
		break;
		case INV_CORR_PERCENTSTATS:
			IncPlayerModValue(pnum, atype, aval);

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

			// for int
			HandleEShieldChange(pnum, remove);
			CalculateUnity(pnum);
		break;

		// weapon mods
		case INV_CORR_WEAPONCRIT:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_CRITPERCENT][WMOD_ITEMS].val += aval;
			MarkWeaponDataSync(pnum, aextra, true);
		break;
		case INV_CORR_WEAPONCRITDMG:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_CRITDMG][WMOD_ITEMS].val += aval;
			MarkWeaponDataSync(pnum, aextra, true);
		break;
		case INV_CORR_WEAPONDMG:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_DMG][WMOD_ITEMS].val = HandleMultiplicativeFactors(Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_DMG][WMOD_ITEMS].val, aval);
			MarkWeaponDataSync(pnum, aextra, true);
		break;
		case INV_CORR_WEAPONFORCEPAIN:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_FORCEPAINCHANCE][WMOD_ITEMS].val += aval;
			MarkWeaponDataSync(pnum, aextra, true);
		break;
		case INV_CORR_WEAPONPOISONPCT:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_POISONFORPERCENTDAMAGE][WMOD_ITEMS].val += aval;
			MarkWeaponDataSync(pnum, aextra, true);
		break;
		case INV_CORR_WEPCULL:
			SetWeaponModPowerset(pnum, aextra, WEP_POWER_CULL, !remove, WMOD_WEP);
			MarkWeaponDataSync(pnum, aextra, true);
		break;

		case INV_CORR_WEAPONPLUSPROJ:
			aextra >>= 16;

			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_EXTRAPROJ][WMOD_ITEMS].val += aval;

			//Log(s:"extra proj is now: ", d:Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_EXTRAPROJ][WMOD_ITEMS].val);

			MarkWeaponDataSync(pnum, aextra, true);
		break;

		case INV_CORR_MAXFRENZY:
			IncPlayerModValue(pnum, atype, aval);

			if(PlayerModData[pnum].vals[PSTAT_EX_COUNTASHAVINGMAXCHARGEOF] && (aextra = ReadPlayerModExtra(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF)) == DND_CHARGE_FRENZY) {
				RemoveBuffWithTableIndex(pnum, BTI_FRENZYCHARGE);
				HandlePlayerBuffAssignment(pnum, 0, BTI_FRENZYCHARGE);
			}
		break;
		case INV_CORR_MAXENDURANCE:
			IncPlayerModValue(pnum, atype, aval);

			if(PlayerModData[pnum].vals[PSTAT_EX_COUNTASHAVINGMAXCHARGEOF] && (aextra = ReadPlayerModExtra(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF)) == DND_CHARGE_ENDURANCE) {
				RemoveBuffWithTableIndex(pnum, BTI_ENDURANCECHARGE);
				HandlePlayerBuffAssignment(pnum, 0, BTI_ENDURANCECHARGE);
			}
		break;
		case INV_CORR_MAXPOWER:
			IncPlayerModValue(pnum, atype, aval);

			if(PlayerModData[pnum].vals[PSTAT_EX_COUNTASHAVINGMAXCHARGEOF] && (aextra = ReadPlayerModExtra(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF)) == DND_CHARGE_POWER) {
				RemoveBuffWithTableIndex(pnum, BTI_POWERCHARGE);
				HandlePlayerBuffAssignment(pnum, 0, BTI_POWERCHARGE);
			}
		break;

		// Deliberately NOT IncPlayerModValue: every one of these names its own source and
		// destination in aextra, so a single summed attribute value could not tell two of them
		// apart. The conversion table is the accumulator instead, and it is reset alongside
		// PlayerModData in ResetPlayerModList.
		//
		// aval is already negated on removal; aextra is not, because this mod is on the
		// IsAttributeExtraException list, so the pair survives an unequip intact.
		case INV_CORR_DAMAGECONVERSION:
			IncPlayerConversionPercent(pnum, aextra & DND_DAMAGECONVERSION_MASK, aextra >> DND_DAMAGECONVERSION_BITS, aval);
		break;

		// same pair, same ladder -- the only difference is that this one does not take the damage
		// away from the source, so it is uncapped and the source keeps dealing everything it had
		case INV_CORR_DAMAGEGAINEDAS:
			IncPlayerDamageGainPercent(pnum, aextra & DND_DAMAGECONVERSION_MASK, aextra >> DND_DAMAGECONVERSION_BITS, aval);
		break;

		default:
			IncPlayerModValue(pnum, atype, aval);
		break;
	}

	return true;
}

// Applies item stats to player -- can remove or add
void ApplyItemFeatures(int pnum, int item_index, int source, bool remove = false, bool noSync = false) {
	int ac = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	
	// check cybernetic and put it as bool
	bool has_cybernetic = ItemIsCybernetic(pnum, item_index, ac, source);
	int i = 0;

	// power cores are inherently cybernetic
	i = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, 0, source);
	has_cybernetic |= i == INV_IMP_POWERCORE;
	
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i)
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, i, source) == INV_CYBERNETIC) {
			has_cybernetic = true;
			break;
		}

	int multiplier = 100;
	auto item = GetUsedItem(pnum, item_index);

	// if player has mirror of eternity and this is a medium charm that is NOT the mirror, multiply magnitude by 2
	if
	(
		PlayerModData[pnum].vals[PSTAT_EX_MIRROROTHERMEDIUM] && 
		(item.item_type & 0xFFFF) == DND_ITEM_CHARM &&
		item.item_subtype == DND_CHARM_MEDIUM &&
		(item.item_type >> 16) - 1 != UITEM_MIRROROFETERNITY
	)
	{
		multiplier *= 2;
	}

	// quality check
	int temp = GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_index, -1, source);
	if(temp) {
		// quality is percent based
		multiplier = multiplier * (temp + 100) / 100;
	}
	
	// cybernetic check
	if(has_cybernetic && HasClassPerk_Fast(DND_PLAYER_CYBORG, 1))
		multiplier = multiplier * (DND_CYBERNETIC_FACTOR + 100) / 100;
		
	// implicits dont get well of power factor
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i)
		ProcessItemImplicit(pnum, item_index, source, i, remove, multiplier);

	// Well of power factor -- the item type has to be part of this test. DND_CHARM_SMALL is 0, and
	// every other equippable keeps an unrelated index in item_subtype (boot/armor/helm base, flask
	// kind), so a subtype-only check hands the factor to whichever of those happens to be base 0.
	temp = PlayerModData[pnum].vals[PSTAT_EX_FACTOR_SMALLCHARM];
	if(temp && (item.item_type & 0xFFFF) == DND_ITEM_CHARM && item.item_subtype == DND_CHARM_SMALL)
		multiplier = multiplier * temp / FACTOR_FIXED_RESOLUTION;

	for(i = 0; i < ac; ++i)
		ProcessItemFeature(pnum, item_index, source, i, remove, multiplier);

	ACS_NamedExecuteWithResult("DnD Handle Attribute Sync", pnum);

	if(noSync)
		return;

	for(i = 0; i < MAXWEPS; ++i) {
		if(WeaponNeedsDataSync(pnum, i)) {
			SyncClientsideVariable_WeaponMods(pnum, i);
			MarkWeaponDataSync(pnum, i, false);
		}
	}
}

int GetCraftableItemCount() {
	int res = 0, pnum = PlayerNumber();
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto item = GetPlayerInventoryItem(pnum, i);
		if(IsCraftableItem(item.item_type) && item.height)
			++res;
	}
	return res;
}

// Orb of Assimilation is the only caller: it copies a mod off the donor at the tier the DONOR rolled
// it, which can be above anything the host's own level could have produced.
void InsertAttributeToItem(int pnum, int item_pos, int a_id, int a_val, int a_tier, int a_extra = 0, bool a_fracture = false) {
	auto item = GetPlayerInventoryItem(pnum, item_pos);
	int temp = item.attrib_count++;
	item.attributes[temp].attrib_id = a_id;
	item.attributes[temp].attrib_val = a_val;
	item.attributes[temp].attrib_tier = a_tier;
	item.attributes[temp].attrib_extra = a_extra;
	item.attributes[temp].fractured = a_fracture;

	// set ilvl to the min requirement of this tier
	a_tier = GetModTierRequirement(a_id, item.item_type, a_tier);

	// ...but only so far. A tier 9 or 10 mod needs ilvl 90/100 to exist naturally, so without this an
	// ordinary item handed one by assimilation walked straight up to boss item level -- a level range
	// only boss drops are supposed to reach, and one the host would then demand of whoever wears it.
	// An item that was ALREADY at boss level keeps the wider ceiling, since it earned that on the drop.
	//
	// Read off the live item_level, not a level captured before the first mod: assimilation inserts
	// several in a row, and the cap of the item as it now stands is the one that should apply to each.
	// A host above its own ceiling (a boss item at 95) is left where it is -- this only ever raises,
	// so the clamp can lower what a mod ASKS for but never what the item already has.
	int ilvl_ceiling = item.item_level >= MAX_BOSS_ILVL ? MAX_BOSS_ILVL : MAX_REGULAR_ILVL;
	if(a_tier > ilvl_ceiling)
		a_tier = ilvl_ceiling;

	if(item.item_level < a_tier)
		item.item_level = a_tier;

	CheckAttribEffects(pnum, item_pos, a_id, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

void CheckAttribEffects(int pnum, int item_pos, int attrib, int source) {
	int itype = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_pos, -1, source);
	if(itype == DND_ITEM_CHARM && attrib == INV_CYBERNETIC) {
		// check quality and zero it if its non-zero
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_pos, -1, source) > 0) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_pos, -1, 0, source);
			SyncItemQuality(pnum, item_pos, source);
		}
	}
}

// can only add attributes to items that are about to be created ie. on field dropped from monster
void AddAttributeToFieldItem(int item_pos, int attrib, int pnum, int max_affixes = 0) {
	auto item = GetFieldItem(item_pos);
	if(!max_affixes)
		max_affixes = GetMaxItemAffixes(item.item_type, item.item_subtype);
	if(item.attrib_count < max_affixes) {
		int temp = item.attrib_count++;
		int lvl = GetModTierForLevel(attrib, item.item_type, item.item_level);
		
		bool makeWellRolled = CheckWellRolled(pnum);
		
		lvl = RollModTier(attrib, item.item_type, lvl, makeWellRolled);

		item.attributes[temp].attrib_tier = lvl;
		item.attributes[temp].attrib_id = attrib;
		item.attributes[temp].fractured = false;

		// it basically adds the step value (val) and a +1 if we aren't 0, so our range is ex: 5-10 in tier 1 then 11-15 in tier 2 assuming +5 range per tier
		// luck adds a small chance for a charm to have well rolled modifier on it -- luck gain is 0.15, 0.05 x 10 = 0.5 max rank thats 50% chance for well rolled mods
		if(item.item_type != DND_ITEM_DUNGEONKEY) {
			item.attributes[temp].attrib_val = RollAttributeValue(
				attrib, 
				lvl, 
				makeWellRolled,
				item.item_type,
				item.item_subtype
			);
		}
		else {
			item.attributes[temp].attrib_val = RollDungeonAttributeValue(
				attrib, 
				lvl, 
				makeWellRolled
			);
		}

		max_affixes = GetExtraForMod(
			pnum, attrib, lvl, 
			item.item_type, item.item_subtype, 
			makeWellRolled,
			item.attributes[temp].attrib_val
		);
		if(max_affixes != -1)
			item.attributes[temp].attrib_extra = max_affixes;
	}
	CheckAttribEffects(pnum, item_pos, attrib, DND_SYNC_ITEMSOURCE_FIELD);
}

// adds attribute to existing item in player inventory
void AddAttributeToItem(int pnum, int item_pos, int attrib, bool isWellRolled = false) {
	auto item = GetPlayerInventoryItem(pnum, item_pos);
	int temp = item.attrib_count++;
	int lvl = GetModTierForLevel(attrib, item.item_type, item.item_level);
	
	// 10% chance to roll a tier up or down for the modifier on the charm
	lvl = RollModTier(attrib, item.item_type, lvl, isWellRolled);
	
	item.attributes[temp].attrib_tier = lvl;
	item.attributes[temp].attrib_id = attrib;
	item.attributes[temp].attrib_extra = 0; // set this to 0, if the rollattributevalue needs to assign the extra it will
	item.attributes[temp].fractured = false;
	
	// roll the attribute
	item.attributes[temp].attrib_val = RollAttributeValue(
		attrib, 
		lvl,
		isWellRolled,
		item.item_type,
		item.item_subtype
	);

	lvl = GetExtraForMod(
		pnum, attrib, lvl, 
		item.item_type, item.item_subtype, 
		isWellRolled,
		item.attributes[temp].attrib_val
	);
	if(lvl != -1)
		item.attributes[temp].attrib_extra = lvl;

	// if attribute is CYBERNETIC, make sure it resets quality of the item to 0 in case its a charm
	CheckAttribEffects(pnum, item_pos, attrib, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

// can add implicits up to 3
void GiveImplicitToField(int item_pos, int attr, int val, int extra = -1, int tier = 0, int tier_mapping = 0, int item_base = 0) {
	auto item = GetFieldItem(item_pos);
	int imp_pos = 0;
	for(imp_pos = 0; imp_pos < MAX_ITEM_IMPLICITS && item.implicit[imp_pos].attrib_id != -1; ++imp_pos);

	if(imp_pos == MAX_ITEM_IMPLICITS)
		return;

	if(extra != -1)
		item.implicit[imp_pos].attrib_extra = extra;

	item.implicit[imp_pos].attrib_id = attr;
	item.implicit[imp_pos].attrib_tier = tier;

	if(!tier)
		item.implicit[imp_pos].attrib_val = val;
	else {
		extra = GetItemTier(tier);
		if(tier_mapping)
			item.implicit[imp_pos].attrib_val = random(val + extra * tier_mapping, val + (extra + 1) * tier_mapping);
		else
			item.implicit[imp_pos].attrib_val = val * (extra + 1);
	}

	// Only the first implicit names the base. Every later one passes 0 for it, and
	// DND_ITEMBASE_CHARM is 0, so writing unconditionally reset the base to charm on any item
	// carrying more than one implicit -- which is most of them.
	if(!imp_pos)
		item.item_base = item_base;
}

void GiveImplicitToMerchant(int item_pos, int attr, int val, int extra = -1, int tier = 0, int tier_mapping = 0, int item_base = 0) {
	auto item = GetMerchantItem(item_pos);
	int imp_pos = 0;
	for(imp_pos = 0; imp_pos < MAX_ITEM_IMPLICITS && item.implicit[imp_pos].attrib_id != -1; ++imp_pos);

	if(imp_pos == MAX_ITEM_IMPLICITS)
		return;

	if(extra != -1)
		item.implicit[imp_pos].attrib_extra = extra;

	item.implicit[imp_pos].attrib_id = attr;
	item.implicit[imp_pos].attrib_tier = tier;

	if(!tier)
		item.implicit[imp_pos].attrib_val = val;
	else {
		int temp = GetItemTier(tier);
		if(tier_mapping)
			item.implicit[imp_pos].attrib_val = random(val + temp * tier_mapping, val + (temp + 1) * tier_mapping);
		else
			item.implicit[imp_pos].attrib_val = val * (temp + 1);
	}

	// see GiveImplicitToField -- only implicit 0 names the base
	if(!imp_pos)
		item.item_base = item_base;
}

void GiveCorruptionEffect(int pnum, int item_pos) {
	// pick between random effects to corruption implicits, with equal weight, and decide from there
	// roll between 0 to MAX_CORRUPTION_WEIRD_OUTCOMES + MAX_CORRUPT_IMPLICITS - 1
	// if > than MAX_CORRUPTION_WEIRD_OUTCOMES subtract it to get corrupt implicit
	// NEW: Corruption ALWAYS replaces the very first implicit!
	auto item = GetPlayerInventoryItem(pnum, item_pos);
	item.corrupted = true;

#ifndef ISDEBUGBUILD
	int corr_outcome = random(0, MAX_CORRUPTION_WEIRD_OUTCOMES + MAX_CORRUPT_IMPLICITS - 1);
#else
	int corr_outcome = random(0, MAX_CORRUPTION_WEIRD_OUTCOMES + MAX_CORRUPT_IMPLICITS - 1);//MAX_CORRUPTION_WEIRD_OUTCOMES;
#endif

	if(corr_outcome >= MAX_CORRUPTION_WEIRD_OUTCOMES) {
#ifdef ISDEBUGBUILD
		int corr_mod = FIRST_CORRUPT_IMPLICIT + corr_outcome - MAX_CORRUPTION_WEIRD_OUTCOMES;//random(INV_CORR_DAMAGECONVERSION, INV_CORR_DAMAGEGAINEDAS);
#else
		int corr_mod = FIRST_CORRUPT_IMPLICIT + corr_outcome - MAX_CORRUPTION_WEIRD_OUTCOMES;
#endif
		int extra = GetExtraForMod(pnum, corr_mod);

		if(extra != -1)
			item.implicit[0].attrib_extra = extra;

		item.implicit[0].attrib_id = corr_mod;
		item.implicit[0].attrib_tier = 0;

		// roll the value for this now
		item.implicit[0].attrib_val = random(ItemModTable[corr_mod].attrib_low, ItemModTable[corr_mod].attrib_high);

		return;
	}

	// we are within the initial [0, MAX_CORRUPTION_WEIRD_OUTCOMES) range so we can apply the customized change here

	switch(corr_outcome) {
		case DND_CORR_OUTCOME_QUALITY:
			// don't let it hit negative -- if destiny is used, zero the negative component and make it 1
			extra = CheckInventory("DestinyUsed");
			item.quality += random(-DND_QUALITY_CORRUPTION_CHANGE * (1 - extra) + extra, DND_QUALITY_CORRUPTION_CHANGE);
			if(item.quality < 0)
				item.quality = 0;
			SyncItemQuality(pnum, item_pos, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		break;
	}
}

int GetItemFracturedModCount(int pnum, int item_pos) {
	int fc = 0;
	auto item = GetPlayerInventoryItem(pnum, item_pos);
	for(int i = 0; i < item.attrib_count; ++i)
		fc += item.attributes[i].fractured;
	return fc;
}

// Leaves nothing but fractured mods and implicits on it
int ScourItem(int pnum, int item_pos) {
	// scan for fractured mods
	int min_count = GetItemFracturedModCount(pnum, item_pos);
	int frac_id = 0;
	int i;
	auto item = GetPlayerInventoryItem(pnum, item_pos);

	// completely reset the item attribs
	for(i = 0; i < item.attrib_count; ++i) {
		// is this fractured
		if(item.attributes[i].fractured) {
			// if fractured mods are on top, ordered, do not erase or do anything
			if(frac_id == i) {
				// increment this too because we are technically moving over it!
				++frac_id;
				continue;
			}
			// move the fractured mod to the beginning
			item.attributes[frac_id].attrib_val = item.attributes[i].attrib_val;
			item.attributes[frac_id].attrib_tier = item.attributes[i].attrib_tier;
			item.attributes[frac_id].attrib_id = item.attributes[i].attrib_id;
			item.attributes[frac_id].attrib_extra = item.attributes[i].attrib_extra;
			item.attributes[frac_id].fractured = true;
			++frac_id;
		}

		// erase the current attribute slot
		item.attributes[i].attrib_val = 0;
		item.attributes[i].attrib_tier = 0;
		item.attributes[i].attrib_id = 0;
		item.attributes[i].attrib_extra = 0;
		item.attributes[i].fractured = 0;
	}

	item.attrib_count = min_count;

	// return the new minimum count, can be 0 or non-zero if fractured mods exist
	return min_count;
}

// 0 means nothing exists of this sort
int GetSpecialRollAttribute(int pnum, int item_pos) {
	int special_roll;
	// check if any of the implicits allow for special roll rules
	auto item = GetPlayerInventoryItem(pnum, item_pos);
	for(special_roll = 0; special_roll < MAX_ITEM_IMPLICITS; ++special_roll) {
		if(IsSpecialRollRuleAttribute(item.implicit[special_roll].attrib_id)) {
			special_roll = item.implicit[special_roll].attrib_id;
			break;
		}
	}

	if(special_roll == MAX_ITEM_IMPLICITS)
		special_roll = 0;

	return special_roll;
}

// The mod pool answers "what may ROLL on this base". This asks the same question about a mod that
// already exists and is being MOVED onto an item, which orb of assimilation is the only path that
// does. It runs the pool's own admission test rather than a second copy of the rules, so the two
// cannot drift: the slot, the base's effective tags and the item's own widening implicit all apply
// exactly as they would at drop time.
bool CanModLiveOnItem(int pnum, int item_pos, int mod) {
	// not a pooled mod (unique-only ids run past the end of ItemModTable), so there is nothing to
	// judge it against -- assimilation refuses uniques anyway, this is just a guard on the index
	if(mod < FIRST_INV_ATTRIBUTE || mod > LAST_INV_ATTRIBUTE)
		return true;

	auto item = GetPlayerInventoryItem(pnum, item_pos);
	return CanRollModWidened(
		ItemBaseToModBaseFlag(item.item_base),
		GetItemBaseEffectiveTags(item.item_base),
		ModPoolTagIdToMask(GetWideningTagId(GetSpecialRollAttribute(pnum, item_pos))),
		mod
	);
}

// Dungeon key mods live in their own id space: DUN_ATTR_EXTRAHP is 0, so key mod ids run
// 0..DUN_ATTR_MAX-1 and sit directly on top of INV_ ids. Nothing keyed by mod id may be reached with
// one -- not the mod pools, not the tier profiles -- so both gate on this.
//
// Flasks were lumped in here for both. They should not have been: their ids start at
// FLASK_ATTRIB_ID_BEGIN and collide with nothing, and they now have real ladders and real pools.
bool HasSeparateModIdSpace(int item_type) {
	return item_type == DND_ITEM_DUNGEONKEY;
}

// special roll rule holds PPOWER_CANROLLXXXX and it checks what is possible based on that
// last field is checking for Orb of Order use, if it's not -2 then we must check for its use
int PickRandomAttribute(int item_type = DND_ITEM_CHARM, int item_subtype = DND_CHARM_SMALL, int special_roll_rule = 0, int implicit_id = -1, int respect_order_orb = -2, int item_base = -1, int ilvl = -1) {
	// Flasks write an item base now, but one saved before they did carries whatever was in the field --
	// a leftover 0 is DND_ITEMBASE_CHARM and would quietly draw from the charm pool. The subtype is the
	// authority on which of the two flask pools applies, so derive rather than trust.
	if(item_type == DND_ITEM_FLASK)
		item_base = GetFlaskItemBase(item_subtype);

	// Dungeon keys draw from their own mod id range and have no item base, so they keep the flat pick
	// below. Everything else, flasks included, comes from a weighted pool.
	if(!HasSeparateModIdSpace(item_type) && item_base >= 0) {
		// respect_order_orb stores tag + 1, with -2 meaning "no orb" and 0 "orb but no tag stored".
		int forced_tag_id = (respect_order_orb == -2 || !respect_order_orb) ? DND_MODPOOL_NO_TAG : respect_order_orb - 1;

		// The base names the slot and the tagset, a widening implicit adds its tag group on top,
		// and a guarantee narrows to one tag. All of that was settled when the pool was built, so
		// this is one weighted draw -- no tag walk, no exception checks, no retry budget.
		mod_pool_T* pool = GetModPool(item_base, GetWideningTagId(special_roll_rule), forced_tag_id);

		// An empty pool means the guarantee asked for a tag this base cannot roll. Nothing to pick,
		// so the caller drops this mod rather than substituting one -- same outcome the old code
		// reached by exhausting its retry budget, just without spending the retries.
		if(pool == null || !pool.count)
			return -1;

		// The item level gate is rejection, not a filtered pool: banding the pool cache by level would
		// multiply the alias tables well past the heap. Rejection keeps the weights right among what is
		// eligible. ilvl < 0 means the caller does not gate.
		int res;
		int tries = DND_MAX_ORB_REROLL_ATTEMPTS;
		do {
			res = PickModFromPool(pool);
		} while(ilvl >= 0 && !CanModRollAtLevel(res, item_type, ilvl) && tries-- > 0);

		// budget spent on a base whose eligible mods all outlevel this item
		if(ilvl >= 0 && !CanModRollAtLevel(res, item_type, ilvl))
			return -1;

		return res;
	}

	// Dungeon keys only. They are unlevelled and unweighted, so this stays a flat draw -- the life
	// and utility split that used to be filtered here is now the two flask bases' own pools.
	int bias = Timer() & 0xFFFF;
	int val = random(FIRST_DUNGEON_ATTRIBUTE + bias, DUN_ATTR_MAX - 1 + bias) - bias;
	// this is a last resort random here, in case there was an overflow... shouldn't, but might
	if(val < 0)
		val = random(FIRST_DUNGEON_ATTRIBUTE, DUN_ATTR_MAX - 1);

	return val;
}

void AssignAttributes(int pnum, int item_pos, int itype, int attr_count, int respect_order_orb = -2) {
	auto item = GetPlayerInventoryItem(pnum, item_pos);
	int isubt = item.item_subtype;
	int special_roll = GetSpecialRollAttribute(pnum, item_pos);
	
	int i = 0, roll, max_attempts = 0;

	while(i < attr_count) {
		do {
			roll = PickRandomAttribute(itype, isubt, special_roll, item.implicit[0].attrib_id, respect_order_orb, item.item_base, item.item_level);
			++max_attempts;
		} while(roll != -1 && max_attempts < DND_MAX_ORB_REROLL_ATTEMPTS && CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, item.attrib_count) != -1);

		// -1 means the pool had nothing left to offer, most often a guarantee for a tag this base
		// cannot roll. Either way there is no mod to add.
		if(roll == -1 || max_attempts >= DND_MAX_ORB_REROLL_ATTEMPTS)
			break;

		AddAttributeToItem(pnum, item_pos, roll);
		++i;
	}
}

void ReforgeItem(int pnum, int item_pos) {
	auto item = GetPlayerInventoryItem(pnum, item_pos);
	int itype = item.item_type;

	int min_count = ScourItem(pnum, item_pos);
	
	// subtract the fractured mods on it from what it can max have
	int max_natural = GetMaxItemAffixes(itype, item.item_subtype);
	int attr_count = random(1, max_natural) - min_count;

	bool hasOrder = CheckInventory("OrderStored");

	if(attr_count > 0) {
		AssignAttributes(pnum, item_pos, itype, attr_count, hasOrder);
		TakeInventory("ReveranceUsed", 1);
	}

	if(hasOrder)
		SetInventory("OrderStored", 0);
}

void RemoveAttributeFromItem(int pnum, int item_id, int to_remove) {
	auto item = GetPlayerInventoryItem(pnum, item_id);
	// all attributes must be shifted left from the position of the deleted attribute now
	for(int i = to_remove; i < item.attrib_count - 1; ++i) {
		item.attributes[i].attrib_id = item.attributes[i + 1].attrib_id;
		item.attributes[i].attrib_val = item.attributes[i + 1].attrib_val;
		item.attributes[i].attrib_tier = item.attributes[i + 1].attrib_tier;
		item.attributes[i].attrib_extra = item.attributes[i + 1].attrib_extra;
		item.attributes[i].fractured = item.attributes[i + 1].fractured;
	}
	--item.attrib_count;
}

// Gives an attribute of a tag group guaranteed, and completely reforges the attribs
void ReforgeWithOneTagGuaranteed(int pnum, int item_pos, int tag_id, int affluence = 1, bool isWellRolled = false) {
	auto item = GetPlayerInventoryItem(pnum, item_pos);
	int itype = item.item_type;

	int min_count = ScourItem(pnum, item_pos);
	int attr_count = GetMaxItemAffixes(itype, item.item_subtype) - min_count;

	// in case this is a fully fractured mod item
	if(attr_count <= 0)
		return;

	// cap at 2
	if(affluence > 2)
		affluence = 2;

	// The guarantee is the base's own pool narrowed to one tag, so it already respects the slot,
	// the base's exclusions and any widening implicit. A tag the base cannot roll leaves the pool
	// empty, and the guarantee is simply skipped -- the reforge below still runs.
	mod_pool_T* pool = GetModPool(item.item_base, GetWideningTagId(GetSpecialRollAttribute(pnum, item_pos)), tag_id);

	if(pool != null && pool.count) {
		int rand_attr;
		int max_tries = 30;

		while(affluence > 0 && attr_count > 0 && max_tries-- > 0) {
			rand_attr = PickModFromPool(pool);

			// if this isn't already present on the item in question, and the item is high enough level
			if(CanModRollAtLevel(rand_attr, itype, item.item_level) && CheckItemAttribute(pnum, item_pos, rand_attr, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, item.attrib_count) == -1) {
				AddAttributeToItem(pnum, item_pos, rand_attr, isWellRolled);
				TakeInventory("ReveranceUsed", 1);
				--attr_count;
				--affluence;
			}
		}
	}
	else
		TakeInventory("ReveranceUsed", 1);

	// rest of the mods
	AssignAttributes(pnum, item_pos, itype, attr_count);
}

int PickUniqueItem(int item_type, int unique_id = -1) {
	int i, beg, end, w;

	switch(item_type) {
		case DND_ITEM_CHARM:
			beg = UNIQUE_CHARM_BEGIN;
			end = UNIQUE_CHARM_REGULARDROP_END; // roll only until the regular drop
			w = MAX_UNIQUE_WEIGHT;
		break;
		case DND_ITEM_SPECIALTY_CYBORG:
			beg = UNIQUE_POWERCORE_BEGIN;
			end = UNIQUE_POWERCORE_END;
			w = MAX_UNIQUE_PCORE_WEIGHT;
		break;
		case DND_ITEM_BODYARMOR:
			beg = UNIQUE_BODYARMOR_BEGIN;
			end = UNIQUE_BODYARMOR_END;
			w = MAX_UNIQUE_BODYARMOR_WEIGHT;
		break;
		case DND_ITEM_BOOT:
			beg = UNIQUE_BOOT_BEGIN;
			end = UNIQUE_BOOT_END;
			w = MAX_UNIQUE_BOOT_WEIGHT;
		break;
		case DND_ITEM_HELM:
			beg = UNIQUE_HELM_BEGIN;
			end = UNIQUE_HELM_END;
			w = MAX_UNIQUE_HELM_WEIGHT;
		break;
		default:
			beg = UNIQUE_CHARM_BEGIN;
			end = UNIQUE_CHARM_END;
			w = MAX_UNIQUE_WEIGHT;
		break;
	}

	if(unique_id == -1) {
		w = random(1, w);
		for(i = beg; i <= end && w > UniqueItemList[i].weight; ++i);

		#ifdef ISDEBUGBUILD
			if(item_type == DND_ITEM_CHARM) {
				if(random(0, 1))
					end = UNIQUE_CHARM_REGULARDROP_END;
				else {
					beg = UNIQUE_CHARM_DROPONLY_BEGIN;
					end = UNIQUE_CHARM_END;
				}
				
				w = Timer() & 0xFFFF;
				i = random(w + beg, w + end) - w;
				//i = random(UITEM_ELEMENTALHARMONY, UITEM_THORNVEIN);
				i = UITEM_DRAGONFANG;
				//i = random(UITEM_UNITY, UITEM_MINDFORGE);
			}
			else if(item_type == DND_ITEM_BOOT)
				i = UNIQUE_BOOT_END;
			else
				i = random(beg, end);
		#endif
	}
	else
		i = unique_id;
	return i;
}

void ConstructUniqueOnField(int fieldpos, int unique_id, int pnum) {
	auto item = GetFieldItem(fieldpos);
	item.width = UniqueItemList[unique_id].width;
	item.height = UniqueItemList[unique_id].height;
	item.item_type = UniqueItemList[unique_id].item_type;
	item.item_image = UniqueItemList[unique_id].item_image;
	item.item_subtype = UniqueItemList[unique_id].item_subtype;
	item.item_level = UniqueItemList[unique_id].item_level;
	item.item_stack = UniqueItemList[unique_id].item_stack;
	item.attrib_count = UniqueItemList[unique_id].attrib_count;
	item.topleftboxid = 0;

	// this can set images sometimes, so just moved item_image below here
	SetupItemImplicit(fieldpos, item.item_type & 0xFFFF, item.item_subtype, item.item_level);

	item.corrupted = 0;
	item.quality = 0;

	for(int i = 0; i < item.attrib_count; ++i) {
		item.attributes[i].attrib_id = UniqueItemList[unique_id].attrib_id_list[i];
		item.attributes[i].attrib_tier = 0;
		
		// we must roll the value once dropped
		bool makeWellRolled = CheckWellRolled(pnum);
		if(!makeWellRolled) {
			item.attributes[i].attrib_val = random(UniqueItemList[unique_id].rolls[i].attrib_low, UniqueItemList[unique_id].rolls[i].attrib_high);
			item.attributes[i].attrib_extra = random(UniqueItemList[unique_id].rolls[i].attrib_extra_low, UniqueItemList[unique_id].rolls[i].attrib_extra_high);
		}
		else {
			item.attributes[i].attrib_val = random((UniqueItemList[unique_id].rolls[i].attrib_low + UniqueItemList[unique_id].rolls[i].attrib_high) / 2, UniqueItemList[unique_id].rolls[i].attrib_high);
			item.attributes[i].attrib_extra = random((UniqueItemList[unique_id].rolls[i].attrib_extra_low + UniqueItemList[unique_id].rolls[i].attrib_extra_high) / 2, UniqueItemList[unique_id].rolls[i].attrib_extra_high);
		}
	}
}

void ResetPlayerInventory(int pnum) {
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto item = GetPlayerInventoryItem(pnum, i);
		if(item.topleftboxid - 1 == i)
			SyncItemData_Null(pnum, i, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, item.width, item.height);
		ClearInventoryItem(item);
	}
}

void ResetTradeViewList(int pnum) {
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto item = GetTradeItem(pnum, i);
		if(item.topleftboxid - 1 == i)
			SyncItemData_Null(pnum, i, DND_SYNC_ITEMSOURCE_TRADEVIEW, item.width, item.height);
		ClearInventoryItem(item);
	}
	ClearTradeItemOrigins(pnum);
}

void ResetPlayerStash(int pnum) {
	for(int p = 0; p < MAX_EXTRA_INVENTORY_PAGES + 1; ++p) {
		for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
			auto item = GetPlayerStashItem(pnum, p, i);
			if(item.topleftboxid - 1 == i)
				SyncItemData_Null(pnum, i, DND_SYNC_ITEMSOURCE_STASH | (p << 16), item.width, item.height);
			ClearInventoryItem(item);
		}
	}
}

void ResetPlayerItemsUsed(int pnum) {
	for(int i = 0; i < MAX_ITEMS_EQUIPPABLE; ++i) {
		auto item = GetUsedItem(pnum, i);
		if(item.item_type != DND_ITEM_NULL)
			SyncItemData_Null(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, item.width, item.height);
		ClearInventoryItem(item);
	}
}

// underlying assumption that the item isn't null, as it's returned from crafting screen
int DisassembleItem_Price(int pnum, int item_pos) {
	// we have a price band for the item, use the price the gauge what kind of orb(s) we can give to the player
	// make tier and amount of mods contribute highly to this too, so we need to do some of the steps we did again for this one
	auto item = GetPlayerInventoryItem(pnum, item_pos);
	int base = DND_BASE_DISASSEMBLE_COST;
	int ilvl = item.item_level;

	int avg_mod_tier = 0;
	int acount = item.attrib_count;
	int fracture_count = 0;
	int i;
	if(acount) {
		if(!IsPlayerInventoryItemUnique(pnum, item_pos)) {
			for(i = 0; i < acount; ++i) {
				avg_mod_tier += GetModTierNormalized(item.attributes[i].attrib_id, item.item_type, item.attributes[i].attrib_tier) + 1;
				fracture_count += item.attributes[i].fractured;
			}
		
			avg_mod_tier /= acount;
		}
		else
			avg_mod_tier = MAX_CHARM_AFFIXTIERS / 2;
	}

	base = base * (100 + (ilvl - 1) * DND_DISASSEMBLE_LEVEL_PERCENT + fracture_count * DND_DISASSEMBLE_FRACTURE_PERCENT + DND_DISASSEMBLE_TIER_PERCENT * avg_mod_tier) / 100;

	// if corrupted or has implicit, include that too
	fracture_count = 0;
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i)
		fracture_count += item.implicit[i].attrib_id != -1;

	base = base * (100 + DND_DISASSEMBLE_IMPLICIT_PERCENT * fracture_count) / 100;
	
	if(item.corrupted) {
		// 50% increase
		base *= 3;
		base >>= 1;
	}

	return base;
}

// returns the chance and a score rating how big the orb rarity and yield should be depending on how loaded the item is
int GetDissassembleChance(int pnum, int item_pos) {
	auto item = GetPlayerInventoryItem(pnum, item_pos);
	int ilvl = item.item_level;

	int avg_mod_tier = 0;
	int acount = item.attrib_count;
	int fracture_count = 0;
	if(acount) {
		for(int i = 0; i < acount; ++i) {
			// uniques have tier 0
			if(item.attributes[i].attrib_tier)
				avg_mod_tier += GetModTierNormalized(item.attributes[i].attrib_id, item.item_type, item.attributes[i].attrib_tier);
			else
				avg_mod_tier += MAX_CHARM_AFFIXTIERS / 2;
			fracture_count += item.attributes[i].fractured;
		}
		avg_mod_tier /= acount;
	}

	// give more chance to succeed if we have the research related to it too
	bool hasResearch = CheckResearchStatus(RES_MOLECULARREC);
	int chance = 	DND_BASE_DISASSEMBLE_CHANCE + 
					hasResearch * DND_DISASS_CHANCEBONUS_RESEARCH;

	// 10% of ilvl + 25% of avg mod tier + 3% flat per fractured mod and 5% if corrupted to fail
	chance -= ilvl / 10 + avg_mod_tier / 4 + DND_BASE_FRACTURE_DISASSEMBLE_CHANCE * fracture_count + DND_BASE_CORRUPT_DISASSEMBLE_CHANCE * item.corrupted;

	int yields = 	ilvl * DND_BASE_ILVL_YIELD +
					avg_mod_tier * DND_BASE_AVGMOD_YIELD + 
					DND_BASE_FRACTURE_YIELD * fracture_count + 
					DND_BASE_CORRUPT_YIELD * item.corrupted +
					hasResearch + DND_DISASS_CHANCEBONUS_YIELD;
	
	if(yields > 0xFFFF)
		yields = 0xFFFF;

	// limit it
	if(chance > 100)
		chance = 100;

	return (chance << 16) + yields;
}

void DisassembleItem(int pnum, int item_pos, int price, int chance) {
	GiveInventory("DnD_CantSalvage", 1);
	TakeCredit(price);
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
	TakeInventory("DnD_CantSalvage", 1);
}

Script "DnD Disassemble CS" (int result) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	if(result) {
		Log(s:"\c[Y5]", l:"DND_DISASS", s:": \cd", l:"DND_DISASS_WIN");
		LocalAmbientSound("Items/SuccessDisassemble", 127);
	}
	else {
		Log(s:"\c[Y5]", l:"DND_DISASS", s:": \cg", l:"DND_DISASS_LOSS");
		LocalAmbientSound("Items/FailDisassemble", 127);
	}
}

bool GetItemMaxQuality(int pnum, int item_index) {
	for(int i = 0; i < MAX_ITEM_IMPLICITS; ++i)
		if(GlobalItemStorage.PlayerInventoryList[pnum][item_index].implicit[i].attrib_id == INV_IMP_QUALITYCAPFIFTY)
			return 2 * DND_MAX_CHARM_QUALITY;
	return DND_MAX_CHARM_QUALITY;
}

Script "DnD Check Item Collision" (void) {
	SetResultValue(Spawn("DnD_ItemCollisionChecker", GetActorX(0), GetActorY(0), GetActorZ(0)));
}

// This is ran on spawned items on field
int CheckItemSynergy(int synergy_roll, int item_pos, int synergy_boost) {
	static int tags_found[MAX_ATTRIB_TAG_GROUPS];

	int chance = DND_SYNERGYITEM_CHANCE;
	if(synergy_boost != -1)
		chance = chance * synergy_boost / 100;

	auto item = GetFieldItem(item_pos);

	if(synergy_roll == -2 && random(0, 1.0) <= chance) {
		// pick one of the random mods on the existing item to be the target mod tag to go after
		synergy_roll = random(0, item.attrib_count - 1);

		// pick a random tag if its got multiple then give it +1, as it expects +1 of it
		synergy_roll = item.attributes[synergy_roll].attrib_id;
		synergy_roll = ItemModTable[synergy_roll].tags;

		int i, j, temp;
		if(synergy_roll & (synergy_roll - 1)) {
			// not power of two so its got multiple tags in it, process it to pick a random one
			for(i = 0; i < MAX_ATTRIB_TAG_GROUPS; ++i)
				tags_found[i] = 0;

			i = 0;			// holds tags found
			j = 0;			// counter
			while(synergy_roll) {
				if(synergy_roll & 1)
					tags_found[i++] = j + 1; // we store +1 here already
				synergy_roll >>= 1;
				++j;
			}

			// pick random bit from the obtained total
			synergy_roll = tags_found[random(0, i - 1)];
		}
		else {
			j = 0;
			while(synergy_roll) {
				synergy_roll >>= 1;
				++j;
			}
			synergy_roll = j;
		}
	}
	//printbold(s:"item ", d:item_pos, s:" picked synergy id ", d:synergy_roll - 1);
	return synergy_roll;
}

#include "DnD_Token.h"
#include "../DnD_Sync.h"

// These are necessary to sync the global variables + unique data
Script "DnD Load Inventory Attributes" OPEN {
	if(!isSetupComplete(SETUP_STATE1, SETUP_ITEMTABLES)) {
		AllocateNonPlayerItemStorageMemory();
		Delay(const:1);
		for(int i = 0; i < MAXPLAYERS; ++i) {
			AllocateItemStorageMemory(i);
			if(!(i % 4))
				Delay(const:1);
		}
		Delay(const:5);
		SetupArmorDropWeights();
		Delay(const:5);
		SetupFlaskDropWeights();
		SetupTokenDropWeights();
		Delay(const:10);
		SetupModTierProfiles();
		SetupInventoryAttributeTable();
		Delay(const:8);
		InitModPoolCache();
		Delay(const:1);

		// One pool is a 161 entry eligibility scan plus an alias build, so all 35 in one tic is a
		// runaway. Spread like the storage allocation above.
		for(i = 0; i < DND_MAX_ITEMBASES; ++i) {
			GetModPool(i);
			if(!(i % 4))
				Delay(const:1);
		}

		Delay(const:1);
		LogModPoolCacheSize();
		Delay(const:1);

		// the two bands that actually gate mods -- see the level requirements in DnD_ModTiers.h
		LogModPoolLevelCoverage(1);
		Delay(const:1);
		LogModPoolLevelCoverage(20);
		Delay(const:1);
		SetupDungeonModTable();
		Delay(const:5);

		// A tic to itself on BOTH sides. SetupPerkTable is a table build in the same weight class as
		// SetupInventoryAttributeTable, and the delays are written here rather than left to whatever
		// its neighbours happen to use, so reordering this block cannot quietly pair two builds in one
		// tic. See the bytecode sizes in .claude/notes/dnd-perk-rework.md.
		Delay(const:1);
		SetupPerkTable();
		Delay(const:1);

		Delay(const:10);
		SetupUniqueItems();
		Delay(10);
		ACS_NamedExecuteAlways("DnD Setup Menu Vars", 0); // leave this last here
		Log(s:"Tables setup properly.");
	}
}

Script "DnD Load Inventory Attributes - CS" OPEN CLIENTSIDE {
	if(!isSetupComplete(SETUP_STATE1, SETUP_ITEMTABLES)) {
		AllocateNonPlayerItemStorageMemory();
		Delay(const:1);
		for(int i = 0; i < MAXPLAYERS; ++i) {
			AllocateItemStorageMemory(i);
			if(!(i % 4))
				Delay(const:1);
		}
		Delay(const:5);
		SetupArmorDropWeights();
		Delay(const:5);
		SetupFlaskDropWeights();
		SetupTokenDropWeights();
		Delay(const:10);
		SetupModTierProfiles();
		SetupInventoryAttributeTable();
		Delay(const:10);
		SetupDungeonModTable();
		Delay(const:5);

		// A tic to itself on BOTH sides. SetupPerkTable is a table build in the same weight class as
		// SetupInventoryAttributeTable, and the delays are written here rather than left to whatever
		// its neighbours happen to use, so reordering this block cannot quietly pair two builds in one
		// tic. See the bytecode sizes in .claude/notes/dnd-perk-rework.md.
		Delay(const:1);
		SetupPerkTable();

		// The server can push perk words before this table existed, and the archetype totals they feed
		// are counted AGAINST it -- a sync that landed early stored the lanes correctly and counted
		// nothing. Recount once the table is real; the lanes themselves were never in doubt.
		for(i = 0; i < MAXPLAYERS; ++i)
			RecountPerkPoints(i);
		Delay(const:1);

		SetupUniqueItems();
		Delay(10);
		ACS_NamedExecuteAlways("DnD Setup Menu Vars - CS", 0);
	}
}

// The perk table, for clients the block above never ran for. That block is gated on
// SETUP_ITEMTABLES, and a client already carrying the flag skips all of it -- which was fine while
// everything in there was also built serverside, but the perk tree menu is CLIENTSIDE and draws from
// this table. A client without it sees seven empty archetypes and cannot click a single row.
//
// A fallback, not a second path: it waits for the normal build and only acts if nothing arrived.
// Kept out of the block above deliberately, so it cannot inherit that gate.
Script "DnD Setup Perk Table - CS" OPEN CLIENTSIDE {
	int waited = 0;
	while(waited < DND_PERKTABLE_FALLBACK_WAIT && !IsPerkTableReady()) {
		Delay(const:5);
		waited += 5;
	}

	if(!IsPerkTableReady()) {
		Log(s:"Perk table was missing clientside -- building it here. The main setup block was skipped.");
		SetupPerkTable();
	}
}

#endif
