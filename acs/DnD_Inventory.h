#ifndef DND_INVENTORY_IN
#define DND_INVENTORY_IN

#define MAXINVENTORYBLOCKS_HORIZ 5
#define MAXINVENTORYBLOCKS_VERT 9

#define MAX_INVENTORY_BOXES MAXINVENTORYBLOCKS_HORIZ * MAXINVENTORYBLOCKS_VERT

#define MAX_ITEM_LEVEL 100
#define MAX_ITEM_AFFIXTIERS 4
#define ITEM_TIER_SEPERATOR 100 / MAX_ITEM_AFFIXTIERS

#define MAX_ITEM_ATTRIBUTES 9

#define DND_SYNC_ITEMBEGIN DND_SYNC_ITEMTOPLEFTBOX
#define DND_SYNC_ITEMEND DND_SYNC_ITEMATTRIBUTES_VAL

enum {
	DND_ITEM_NULL,
	DND_ITEM_CHARM,
	DND_ITEM_HELM,
	DND_ITEM_BOOT,
	DND_ITEM_GLOVE,
	DND_ITEM_NECKLACE,
	DND_ITEM_RING
};

typedef struct {
	int attrib_val;
	int attrib_id;
} attr_inf_T;

typedef struct it {
	int width;										// width in inventory space
	int height;										// height in inventory space
	int item_image;									// image of item from image list
	int item_type;									// what type of item it is
	int item_subtype;								// subtype for items that have it (charms etc)
	int item_level;									// what level this item is
	int topleftboxid;								// used to determine the owning pointer (-1 of this is the pointer)
	int attrib_count;								// count of attributes
	attr_inf_T attributes[MAX_ITEM_ATTRIBUTES];		// attribute list
} inventory_T;

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

#define MAX_ITEM_IMAGES IM_LCHRM3 + 1
str Item_Images[MAX_ITEM_IMAGES] = {
	"SCHRM1",
	"SCHRM2",
	"SCHRM3",
	
	"MCHRM1",
	"MCHRM2",
	"MCHRM3",
	
	"LCHRM1",
	"LCHRM2",
	"LCHRM3"
};

#define ITEMLEVEL_VARIANCE_LOWER 15
#define ITEMLEVEL_VARIANCE_HIGHER 7

#define MAX_INVENTORIES_ON_FIELD 256
inventory_T Inventories_On_Field[MAX_INVENTORIES_ON_FIELD];
// holds inventories of all players
global inventory_T 11: PlayerInventoryList[MAXPLAYERS][MAX_INVENTORY_BOXES];

#define INVSOURCE_PLAYER PlayerInventoryList
#define INVSOURCE_CHARMUSED Charms_Used

// Creates an item on the game field
int CreateItemSpot() {
	int pos = -1, i;
	for(i = 0; i < MAX_INVENTORIES_ON_FIELD; ++i)
		if(Inventories_On_Field[i].item_type == DND_ITEM_NULL) {
			pos = i;
			break;
		}
	return pos;
}

void FreeItem(int pnum, int item_index) {
	int i, j;
	int wtemp = PlayerInventoryList[pnum][item_index].width;
	int htemp = PlayerInventoryList[pnum][item_index].height;
	PlayerInventoryList[pnum][item_index].item_subtype = 0;
	PlayerInventoryList[pnum][item_index].item_image = 0;
	PlayerInventoryList[pnum][item_index].item_level = 0;
	for(j = 0; j < PlayerInventoryList[pnum][item_index].attrib_count; ++j) {
		PlayerInventoryList[pnum][item_index].attributes[j].attrib_id = 0;
		PlayerInventoryList[pnum][item_index].attributes[j].attrib_val = 0;
	}
	PlayerInventoryList[pnum][item_index].attrib_count = 0;
	for(j = 0; j < htemp; ++j)
		for(i = 0; i < wtemp; ++i) {
			PlayerInventoryList[pnum][item_index + j * MAXINVENTORYBLOCKS_VERT + i].topleftboxid = 0;
			PlayerInventoryList[pnum][item_index + j * MAXINVENTORYBLOCKS_VERT + i].item_type = DND_ITEM_NULL;
		}
	PlayerInventoryList[pnum][item_index].width = 0;
	PlayerInventoryList[pnum][item_index].height = 0;
	SyncItemData_Null(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, wtemp, htemp);
}

// note to self: height is => horizontal, moving heights => x * MAXINVENTORYBLOCKS_VERT, width is vertical, just + x
int GetFreeSpotForItem(int item_index, int player_index) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	int w = Inventories_On_Field[item_index].width, h = Inventories_On_Field[item_index].height;
	bool unfit = false;

	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			for(hcheck = 0; !unfit && hcheck < h && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < w && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if(PlayerInventoryList[player_index][bid].item_type != DND_ITEM_NULL)
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

// based on average player level
int RollItemLevel() {
	int res = 0;
	// return average player level, +- some value
	int pavg = total_level / PlayerCount();
	if(pavg > 2 * ITEMLEVEL_VARIANCE_LOWER) {
		res = pavg + random(-ITEMLEVEL_VARIANCE_LOWER, ITEMLEVEL_VARIANCE_HIGHER);
		if(res > MAX_ITEM_LEVEL)
			res = MAX_ITEM_LEVEL;
		return res;
	}
	return pavg + random((-pavg + 1) / 2, ITEMLEVEL_VARIANCE_HIGHER);
}

bool CheckItemAttribute(int item_pos, int attrib_index, bool onField, int pnum, int count) {
	int i;
	if(onField) {
		for(i = 0; i < count; ++i)
			if(Inventories_On_Field[item_pos].attributes[i].attrib_id == attrib_index)
				return true;
		return false;
	}
	else {
		for(i = 0; i < count; ++i)
			if(PlayerInventoryList[pnum][item_pos].attributes[i].attrib_id == attrib_index)
				return true;
		return false;
	}
	return false;
}

void CopyItem(bool fieldToPlayer, int fieldpos, int player_index, int item_index) {
	int i, j, k, wtemp, htemp;
	if(!fieldToPlayer) {
		Inventories_On_Field[fieldpos].width = PlayerInventoryList[player_index][item_index].width;
		Inventories_On_Field[fieldpos].height = PlayerInventoryList[player_index][item_index].height;
		Inventories_On_Field[fieldpos].item_subtype = PlayerInventoryList[player_index][item_index].item_subtype;
		Inventories_On_Field[fieldpos].item_image = PlayerInventoryList[player_index][item_index].item_image;
		Inventories_On_Field[fieldpos].item_level = PlayerInventoryList[player_index][item_index].item_level;
		Inventories_On_Field[fieldpos].attrib_count = PlayerInventoryList[player_index][item_index].attrib_count;
		Inventories_On_Field[fieldpos].topleftboxid = 0;
		for(i = 0; i < Inventories_On_Field[fieldpos].attrib_count; ++i) {
			Inventories_On_Field[fieldpos].attributes[i].attrib_id = PlayerInventoryList[player_index][item_index].attributes[i].attrib_id;
			Inventories_On_Field[fieldpos].attributes[i].attrib_val = PlayerInventoryList[player_index][item_index].attributes[i].attrib_val;
		}

		// the leftover spot is a null charm
		FreeItem(player_index, item_index);
	}
	else {
		// handle the box management
		wtemp = Inventories_On_Field[fieldpos].width;
		htemp = Inventories_On_Field[fieldpos].height;
		PlayerInventoryList[player_index][item_index].width = wtemp;
		PlayerInventoryList[player_index][item_index].height = htemp;
		PlayerInventoryList[player_index][item_index].item_subtype = Inventories_On_Field[fieldpos].item_subtype;
		PlayerInventoryList[player_index][item_index].item_image = Inventories_On_Field[fieldpos].item_image;
		PlayerInventoryList[player_index][item_index].item_level = Inventories_On_Field[fieldpos].item_level;
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
		// the leftover spot is a null charm
		Inventories_On_Field[fieldpos].width = 1;
		Inventories_On_Field[fieldpos].height = 1;
		Inventories_On_Field[fieldpos].item_type = DND_ITEM_NULL;
		Inventories_On_Field[fieldpos].item_subtype = 0;
		Inventories_On_Field[fieldpos].item_image = 0;
		Inventories_On_Field[fieldpos].item_level = 0;
		for(k = 0; k < Inventories_On_Field[fieldpos].attrib_count; ++k) {
			Inventories_On_Field[fieldpos].attributes[k].attrib_id = 0;
			Inventories_On_Field[fieldpos].attributes[k].attrib_val = 0;
		}
		Inventories_On_Field[fieldpos].attrib_count = 0;
	}
	SyncItemData(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, wtemp, htemp);
}

// if ipos1 has enough space to acommodate ipos2
bool CanSwapItems(int ipos1, int ipos2) {
	int pnum = PlayerNumber();
	int i, j;
	int w1, w2, h1, h2;
	int bid = 0, temp;
	// from ipos2 to ipos1
	// if null don't check, this is a free spot
	if(PlayerInventoryList[pnum][ipos2].item_type != DND_ITEM_NULL) {
		temp = PlayerInventoryList[pnum][ipos2].topleftboxid - 1;
		w2 = PlayerInventoryList[pnum][temp].width;
		h2 = PlayerInventoryList[pnum][temp].height;
		for(i = 0; i < h2; ++i) {
			for(j = 0; j < w2; ++j) {
				bid = ipos1 + j + i * MAXINVENTORYBLOCKS_VERT;
				if(bid >= MAX_INVENTORY_BOXES)
					return false;
				// if the pointer here doesn't point to us or not empty
				if(PlayerInventoryList[pnum][bid].topleftboxid - 1 != ipos1 && PlayerInventoryList[pnum][bid].item_type != DND_ITEM_NULL)
					return false;
			}
		}
	}
	// from ipos1 to ipos2
	if(PlayerInventoryList[pnum][ipos1].item_type != DND_ITEM_NULL) {
		temp = PlayerInventoryList[pnum][ipos1].topleftboxid - 1;
		w1 = PlayerInventoryList[pnum][temp].width;
		h1 = PlayerInventoryList[pnum][temp].height;
		for(i = 0; i < h1; ++i) {
			for(j = 0; j < w1; ++j) {
				bid = ipos2 + j + i * MAXINVENTORYBLOCKS_VERT;
				if(bid >= MAX_INVENTORY_BOXES)
					return false;
				// if the pointer here doesn't point to us or not empty
				if(PlayerInventoryList[pnum][bid].topleftboxid - 1 != ipos2 && PlayerInventoryList[pnum][bid].item_type != DND_ITEM_NULL)
					return false;
			}
		}
	}
	return true;
}

void SwapItemProperty(int ipos1, int ipos2, int property, int source1, int source2) {
	int pnum = PlayerNumber();
	int temp;
	int i;
	int ht, wt;
	
	int h1, h2, w1, w2;
	
	switch(property) {
		case DND_SYNC_ITEMTOPLEFTBOX:
			h1 = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, ipos1, -1, source1);
			h2 = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, ipos2, -1, source2);
			w1 = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, ipos1, -1, source1);
			w2 = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, ipos2, -1, source2);
			// reset pointers
			for(ht = 0; ht < h1; ++ht)
				for(wt = 0; wt < w1; ++wt)
					SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, 0, source1);
			
			for(ht = 0; ht < h2; ++ht)
				for(wt = 0; wt < w2; ++wt)
					SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, 0, source2);
					
			// handle the leftover pointers
			// update pointers for new scale
			// ipos + 1 because -1 of this is used as pointer
			if(source1 == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) {
				for(ht = 0; ht < h2; ++ht) {
					for(wt = 0; wt < w2; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, ipos1 + 1, source1);
				}
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, ipos1 + 1, source1);
				
			if(source2 == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) {
				for(ht = 0; ht < h1; ++ht) {
					for(wt = 0; wt < w1; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, ipos2 + 1, source2);
				}
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, ipos2 + 1, source2);
		break;
		case DND_SYNC_ITEMTYPE:
			h1 = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, ipos1, -1, source1);
			h2 = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, ipos2, -1, source2);
			w1 = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, ipos1, -1, source1);
			w2 = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, ipos2, -1, source2);
			int itype1 = GetPlayerItemSyncValue(DND_SYNC_ITEMTYPE, ipos1, -1, source1);
			int itype2 = GetPlayerItemSyncValue(DND_SYNC_ITEMTYPE, ipos2, -1, source2);
			// reset pointers
			for(ht = 0; ht < h1; ++ht)
				for(wt = 0; wt < w1; ++wt)
					SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos1 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, DND_ITEM_NULL, source1);
			
			for(ht = 0; ht < h2; ++ht)
				for(wt = 0; wt < w2; ++wt)
					SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos2 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, DND_ITEM_NULL, source2);
					
			// handle the leftover pointers
			// update pointers for new scale
			if(source1 == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) {
				for(ht = 0; ht < h2; ++ht) {
					for(wt = 0; wt < w2; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos1 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, itype2, source1);
				}
			}
			else
				SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos1, -1, itype2, source1);
				
			if(source2 == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) {
				for(ht = 0; ht < h1; ++ht) {
					for(wt = 0; wt < w1; ++wt)
						SetItemSyncValue(DND_SYNC_ITEMTYPE, ipos2 + wt + ht * MAXINVENTORYBLOCKS_VERT, -1, itype1, source2);
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
		case DND_SYNC_ITEMSATTRIBCOUNT:
			temp = GetPlayerItemSyncValue(property, ipos1, -1, source1);
			SetItemSyncValue(property, ipos1, -1, GetPlayerItemSyncValue(property, ipos2, -1, source2), source1);
			SetItemSyncValue(property, ipos2, -1, temp, source2);
		break;
		case DND_SYNC_ITEMATTRIBUTES_ID:
		case DND_SYNC_ITEMATTRIBUTES_VAL:
			wt = GetPlayerItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, ipos1, -1, source1);
			ht = GetPlayerItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, ipos2, -1, source2);
			// if my attrib count now is higher, I need to take from whoever had the higher one before
			if(wt > ht) {
				for(i = 0; i < wt; ++i) {
					// ipos2 had more attributes, it covers a larger space, take them all
					temp = GetPlayerItemSyncValue(property, ipos2, i, source2);
					SetItemSyncValue(property, ipos2, i, GetPlayerItemSyncValue(property, ipos1, i, source1), source2);
					SetItemSyncValue(property, ipos1, i, temp, source1);
				}
			}
			else {
				for(i = 0; i < ht; ++i) {
					// ipos1 had more attributes, it covers a larger space, take them all
					temp = GetPlayerItemSyncValue(property, ipos1, i, source1);
					SetItemSyncValue(property, ipos1, i, GetPlayerItemSyncValue(property, ipos2, i, source2), source1);
					SetItemSyncValue(property, ipos2, i, temp, source2);
				}
			}
		break;
	}
}

// for swapping items
void SwapItems(int ipos1, int ipos2, int source1, int source2, bool dontSync) {
	// check if there's enough space
	if(CanSwapItems(ipos1, ipos2)) {
		printbold(s:"can swap");
		int i, j;
		int h1p = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, ipos1, -1, source1);
		int h2p = GetPlayerItemSyncValue(DND_SYNC_ITEMHEIGHT, ipos2, -1, source2);
		int w1p = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, ipos1, -1, source1); 
		int w2p = GetPlayerItemSyncValue(DND_SYNC_ITEMWIDTH, ipos2, -1, source2);
		
		for(i = DND_SYNC_ITEMBEGIN; i <= DND_SYNC_ITEMEND; ++i)
			SwapItemProperty(ipos1, ipos2, i, source1, source2);
		
		// for large swaps, a big sync at the end is way more favorable
		if(!dontSync) {
			if(GetPlayerItemSyncValue(DND_SYNC_ITEMTYPE, ipos1, -1, source1) == DND_ITEM_NULL)
				SyncItemData_Null(ipos1, source1, w1p, h1p);
			else {
				printbold(s:"item 1 ", d:w1p, s: " ", d:h1p);
				SyncItemPointers(ipos1, source1, w1p, h1p);
				SyncItemData(ipos1, source1, -1, -1);
			}
				
			if(GetPlayerItemSyncValue(DND_SYNC_ITEMTYPE, ipos2, -1, source2) == DND_ITEM_NULL)
				SyncItemData_Null(ipos2, source2, w2p, h2p);
			else {
				printbold(s:"item 2 ", d:w2p, s: " ", d:h2p);
				SyncItemPointers(ipos2, source2, w2p, h2p);
				SyncItemData(ipos2, source2, -1, -1);
			}
		}
	}
}

void ResetPlayerInventory(int pnum) {
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		PlayerInventoryList[pnum][i].item_type = DND_ITEM_NULL;
		PlayerInventoryList[pnum][i].width = 0;
		PlayerInventoryList[pnum][i].height = 0;
		PlayerInventoryList[pnum][i].item_image = 0;
		PlayerInventoryList[pnum][i].item_type = 0;
		PlayerInventoryList[pnum][i].item_subtype = 0;
		PlayerInventoryList[pnum][i].item_level = 0;
		PlayerInventoryList[pnum][i].topleftboxid = 0;
		for(int j = 0; j < PlayerInventoryList[pnum][i].attrib_count; ++j) {
			PlayerInventoryList[pnum][i].attributes[j].attrib_id = 0;
			PlayerInventoryList[pnum][i].attributes[j].attrib_val = 0;
		}
		PlayerInventoryList[pnum][i].attrib_count = 0;
	}
}

#endif