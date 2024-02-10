#ifndef DND_SYNC_IN
#define DND_SYNC_IN

#include "DnD_Charms.h"

enum {
	DND_SYNC_WEAPONENHANCE,
	
	DND_SYNC_SPEED,
	
	DND_SYNC_DROPCHANCE,
	
	DND_SYNC_HPFLAT_BONUS,
	DND_SYNC_ARMORFLAT_BONUS,
	DND_SYNC_HPPERCENT_BONUS,
	DND_SYNC_ARMORPERCENT_BONUS,
	
	DND_SYNC_GREEDPERCENT_BONUS,
	DND_SYNC_WISDOMPERCENT_BONUS,
	
	DND_SYNC_HOLDING,
	DND_SYNC_LUCK,
	
	DND_SYNC_DAMAGEMELEE,
	DND_SYNC_DAMAGEBULLET,
	DND_SYNC_DAMAGEENERGY,
	DND_SYNC_DAMAGEEXPLOSIVE,
	DND_SYNC_DAMAGEOCCULT,
	DND_SYNC_DAMAGEELEMENTAL,
	
	DND_SYNC_WEPMOD_CRIT,
	DND_SYNC_WEPMOD_CRITDMG,
	DND_SYNC_WEPMOD_CRITPERCENT,
	DND_SYNC_WEPMOD_DMG,
	DND_SYNC_WEPMOD_POWERSET1,
	
	DND_SYNC_ITEMTOPLEFTBOX,
	DND_SYNC_ITEMTYPE,
	DND_SYNC_ITEMSUBTYPE,
	DND_SYNC_ITEMWIDTH,
	DND_SYNC_ITEMHEIGHT,
	DND_SYNC_ITEMIMAGE,
	DND_SYNC_ITEMLEVEL,
	DND_SYNC_ITEMSTACK,
	DND_SYNC_ITEMCORRUPTED,
	DND_SYNC_ITEMQUALITY,

	// add non attribute related things from above attrib count

	// implicit attribute stuff
	DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID,
	DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL,
	DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER,
	DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA,

	DND_SYNC_ITEMSATTRIBCOUNT,
	DND_SYNC_ITEMATTRIBUTES_ID,
	DND_SYNC_ITEMATTRIBUTES_VAL,
	DND_SYNC_ITEMATTRIBUTES_TIER,
	DND_SYNC_ITEMATTRIBUTES_EXTRA,
	DND_SYNC_ITEMATTRIBUTES_FRACTURE,
	// add attribute related things from below here
};
#define DND_LAST_SYNC_TYPE DND_SYNC_ITEMATTRIBUTES_FRACTURE

#define FIRST_WEPMOD_SYNC (DND_SYNC_WEPMOD_CRIT)
#define MAX_SYNC_VARS (DND_SYNC_WEPMOD_POWERSET1 + 1)

enum {
	DND_SYNC_ITEMSOURCE_CHARMUSED,
	DND_SYNC_ITEMSOURCE_PLAYERINVENTORY,
	DND_SYNC_ITEMSOURCE_FIELD,
	DND_SYNC_ITEMSOURCE_TRADEVIEW,		// trade view array
	DND_SYNC_ITEMSOURCE_STASH
};

int GetItemSyncValue(int pnum, int which, int extra, int sub, int source) {
	int page = 0;
	
	if(source > 65535) {
		page = source >> 16;
		source &= 0xFFFF;
	}
	
	if(source == DND_SYNC_ITEMSOURCE_CHARMUSED) {
		// safety
		if(extra >= MAX_CHARMS_EQUIPPABLE || extra < 0)
			return 0;
		switch(which) {
			case DND_SYNC_ITEMWIDTH:
			return Charms_Used[pnum][extra].width;
			case DND_SYNC_ITEMHEIGHT:
			return Charms_Used[pnum][extra].height;
			case DND_SYNC_ITEMIMAGE:
			return Charms_Used[pnum][extra].item_image;
			case DND_SYNC_ITEMTYPE:
			return Charms_Used[pnum][extra].item_type;
			case DND_SYNC_ITEMSUBTYPE:
			return Charms_Used[pnum][extra].item_subtype;
			case DND_SYNC_ITEMLEVEL:
			return Charms_Used[pnum][extra].item_level;
			case DND_SYNC_ITEMTOPLEFTBOX:
			return Charms_Used[pnum][extra].topleftboxid;
			case DND_SYNC_ITEMSATTRIBCOUNT:
			return Charms_Used[pnum][extra].attrib_count;
			case DND_SYNC_ITEMCORRUPTED:
			return Charms_Used[pnum][extra].corrupted;
			case DND_SYNC_ITEMQUALITY:
			return Charms_Used[pnum][extra].quality;

			case DND_SYNC_ITEMATTRIBUTES_ID:
			return Charms_Used[pnum][extra].attributes[sub].attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
			return Charms_Used[pnum][extra].attributes[sub].attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
			return Charms_Used[pnum][extra].attributes[sub].attrib_tier;
			case DND_SYNC_ITEMATTRIBUTES_FRACTURE:
			return Charms_Used[pnum][extra].attributes[sub].fractured;
			case DND_SYNC_ITEMATTRIBUTES_EXTRA:
			return Charms_Used[pnum][extra].attributes[sub].attrib_extra;

			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID:
			return Charms_Used[pnum][extra].implicit.attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL:
			return Charms_Used[pnum][extra].implicit.attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER:
			return Charms_Used[pnum][extra].implicit.attrib_tier;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA:
			return Charms_Used[pnum][extra].implicit.attrib_extra;
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_FIELD){
		if(extra >= MAX_INVENTORIES_ON_FIELD || extra < 0)
			return 0;
		switch(which) {
			case DND_SYNC_ITEMWIDTH:
			return Inventories_On_Field[extra].width;
			case DND_SYNC_ITEMHEIGHT:
			return Inventories_On_Field[extra].height;
			case DND_SYNC_ITEMIMAGE:
			return Inventories_On_Field[extra].item_image;
			case DND_SYNC_ITEMTYPE:
			return Inventories_On_Field[extra].item_type;
			case DND_SYNC_ITEMSUBTYPE:
			return Inventories_On_Field[extra].item_subtype;
			case DND_SYNC_ITEMLEVEL:
			return Inventories_On_Field[extra].item_level;
			case DND_SYNC_ITEMSTACK:
			return Inventories_On_Field[extra].item_stack;
			case DND_SYNC_ITEMTOPLEFTBOX:
			return Inventories_On_Field[extra].topleftboxid;
			case DND_SYNC_ITEMSATTRIBCOUNT:
			return Inventories_On_Field[extra].attrib_count;
			case DND_SYNC_ITEMCORRUPTED:
			return Inventories_On_Field[extra].corrupted;
			case DND_SYNC_ITEMQUALITY:
			return Inventories_On_Field[extra].quality;

			case DND_SYNC_ITEMATTRIBUTES_ID:
			return Inventories_On_Field[extra].attributes[sub].attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
			return Inventories_On_Field[extra].attributes[sub].attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
			return Inventories_On_Field[extra].attributes[sub].attrib_tier;
			case DND_SYNC_ITEMATTRIBUTES_FRACTURE:
			return Inventories_On_Field[extra].attributes[sub].fractured;
			case DND_SYNC_ITEMATTRIBUTES_EXTRA:
			return Inventories_On_Field[extra].attributes[sub].attrib_extra;

			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID:
			return Inventories_On_Field[extra].implicit.attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL:
			return Inventories_On_Field[extra].implicit.attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER:
			return Inventories_On_Field[extra].implicit.attrib_tier;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA:
			return Inventories_On_Field[extra].implicit.attrib_extra;
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) {
		if(extra >= MAX_INVENTORY_BOXES || extra < 0)
			return 0;
		switch(which) {
			case DND_SYNC_ITEMWIDTH:
			return PlayerInventoryList[pnum][extra].width;
			case DND_SYNC_ITEMHEIGHT:
			return PlayerInventoryList[pnum][extra].height;
			case DND_SYNC_ITEMIMAGE:
			return PlayerInventoryList[pnum][extra].item_image;
			case DND_SYNC_ITEMTYPE:
			return PlayerInventoryList[pnum][extra].item_type;
			case DND_SYNC_ITEMSUBTYPE:
			return PlayerInventoryList[pnum][extra].item_subtype;
			case DND_SYNC_ITEMLEVEL:
			return PlayerInventoryList[pnum][extra].item_level;
			case DND_SYNC_ITEMSTACK:
			return PlayerInventoryList[pnum][extra].item_stack;
			case DND_SYNC_ITEMTOPLEFTBOX:
			return PlayerInventoryList[pnum][extra].topleftboxid;
			case DND_SYNC_ITEMSATTRIBCOUNT:
			return PlayerInventoryList[pnum][extra].attrib_count;
			case DND_SYNC_ITEMCORRUPTED:
			return PlayerInventoryList[pnum][extra].corrupted;
			case DND_SYNC_ITEMQUALITY:
			return PlayerInventoryList[pnum][extra].quality;

			case DND_SYNC_ITEMATTRIBUTES_ID:
			return PlayerInventoryList[pnum][extra].attributes[sub].attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
			return PlayerInventoryList[pnum][extra].attributes[sub].attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
			return PlayerInventoryList[pnum][extra].attributes[sub].attrib_tier;
			case DND_SYNC_ITEMATTRIBUTES_FRACTURE:
			return PlayerInventoryList[pnum][extra].attributes[sub].fractured;
			case DND_SYNC_ITEMATTRIBUTES_EXTRA:
			return PlayerInventoryList[pnum][extra].attributes[sub].attrib_extra;

			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID:
			return PlayerInventoryList[pnum][extra].implicit.attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL:
			return PlayerInventoryList[pnum][extra].implicit.attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER:
			return PlayerInventoryList[pnum][extra].implicit.attrib_tier;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA:
			return PlayerInventoryList[pnum][extra].implicit.attrib_extra;
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_TRADEVIEW){
		if(extra >= MAX_INVENTORY_BOXES || extra < 0)
			return 0;
		switch(which) {
			case DND_SYNC_ITEMWIDTH:
			return TradeViewList[pnum][extra].width;
			case DND_SYNC_ITEMHEIGHT:
			return TradeViewList[pnum][extra].height;
			case DND_SYNC_ITEMIMAGE:
			return TradeViewList[pnum][extra].item_image;
			case DND_SYNC_ITEMTYPE:
			return TradeViewList[pnum][extra].item_type;
			case DND_SYNC_ITEMSUBTYPE:
			return TradeViewList[pnum][extra].item_subtype;
			case DND_SYNC_ITEMLEVEL:
			return TradeViewList[pnum][extra].item_level;
			case DND_SYNC_ITEMSTACK:
			return TradeViewList[pnum][extra].item_stack;
			case DND_SYNC_ITEMTOPLEFTBOX:
			return TradeViewList[pnum][extra].topleftboxid;
			case DND_SYNC_ITEMSATTRIBCOUNT:
			return TradeViewList[pnum][extra].attrib_count;
			case DND_SYNC_ITEMCORRUPTED:
			return TradeViewList[pnum][extra].corrupted;
			case DND_SYNC_ITEMQUALITY:
			return TradeViewList[pnum][extra].quality;

			case DND_SYNC_ITEMATTRIBUTES_ID:
			return TradeViewList[pnum][extra].attributes[sub].attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
			return TradeViewList[pnum][extra].attributes[sub].attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
			return TradeViewList[pnum][extra].attributes[sub].attrib_tier;
			case DND_SYNC_ITEMATTRIBUTES_FRACTURE:
			return TradeViewList[pnum][extra].attributes[sub].fractured;
			case DND_SYNC_ITEMATTRIBUTES_EXTRA:
			return TradeViewList[pnum][extra].attributes[sub].attrib_extra;

			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID:
			return TradeViewList[pnum][extra].implicit.attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL:
			return TradeViewList[pnum][extra].implicit.attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER:
			return TradeViewList[pnum][extra].implicit.attrib_tier;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA:
			return TradeViewList[pnum][extra].implicit.attrib_extra;
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_STASH){
		if(extra >= MAX_INVENTORY_BOXES || extra < 0)
			return 0;
		switch(which) {
			case DND_SYNC_ITEMWIDTH:
			return PlayerStashList[pnum][page][extra].width;
			case DND_SYNC_ITEMHEIGHT:
			return PlayerStashList[pnum][page][extra].height;
			case DND_SYNC_ITEMIMAGE:
			return PlayerStashList[pnum][page][extra].item_image;
			case DND_SYNC_ITEMTYPE:
			return PlayerStashList[pnum][page][extra].item_type;
			case DND_SYNC_ITEMSUBTYPE:
			return PlayerStashList[pnum][page][extra].item_subtype;
			case DND_SYNC_ITEMLEVEL:
			return PlayerStashList[pnum][page][extra].item_level;
			case DND_SYNC_ITEMSTACK:
			return PlayerStashList[pnum][page][extra].item_stack;
			case DND_SYNC_ITEMTOPLEFTBOX:
			return PlayerStashList[pnum][page][extra].topleftboxid;
			case DND_SYNC_ITEMSATTRIBCOUNT:
			return PlayerStashList[pnum][page][extra].attrib_count;
			case DND_SYNC_ITEMCORRUPTED:
			return PlayerStashList[pnum][page][extra].corrupted;
			case DND_SYNC_ITEMQUALITY:
			return PlayerStashList[pnum][page][extra].quality;

			case DND_SYNC_ITEMATTRIBUTES_ID:
			return PlayerStashList[pnum][page][extra].attributes[sub].attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
			return PlayerStashList[pnum][page][extra].attributes[sub].attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
			return PlayerStashList[pnum][page][extra].attributes[sub].attrib_tier;
			case DND_SYNC_ITEMATTRIBUTES_FRACTURE:
			return PlayerStashList[pnum][page][extra].attributes[sub].fractured;
			case DND_SYNC_ITEMATTRIBUTES_EXTRA:
			return PlayerStashList[pnum][page][extra].attributes[sub].attrib_extra;

			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID:
			return PlayerStashList[pnum][page][extra].implicit.attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL:
			return PlayerStashList[pnum][page][extra].implicit.attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER:
			return PlayerStashList[pnum][page][extra].implicit.attrib_tier;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA:
			return PlayerStashList[pnum][page][extra].implicit.attrib_extra;
		}
	}
	return 0;
}

// which = what property of item
// extra = index in array
// sub = attribute index of item
// val = value to put
// source = source of inventory item (inventory, used charms, field etc.)
void SetItemSyncValue(int pnum, int which, int extra, int sub, int val, int source) {
	int page = 0;
	
	if(source > 65535) {
		page = source >> 16;
		source &= 0xFFFF;
	}
	
	if(source == DND_SYNC_ITEMSOURCE_CHARMUSED) {
		switch(which) {
			case DND_SYNC_ITEMWIDTH:
				Charms_Used[pnum][extra].width = val;
			break;
			case DND_SYNC_ITEMHEIGHT:
				Charms_Used[pnum][extra].height = val;
			break;
			case DND_SYNC_ITEMIMAGE:
				Charms_Used[pnum][extra].item_image = val;
			break;
			case DND_SYNC_ITEMTYPE:
				Charms_Used[pnum][extra].item_type = val;
			break;
			case DND_SYNC_ITEMSUBTYPE:
				Charms_Used[pnum][extra].item_subtype = val;
			break;
			case DND_SYNC_ITEMLEVEL:
				Charms_Used[pnum][extra].item_level = val;
			break;
			case DND_SYNC_ITEMTOPLEFTBOX:
				Charms_Used[pnum][extra].topleftboxid = val;
			break;
			case DND_SYNC_ITEMSATTRIBCOUNT:
				Charms_Used[pnum][extra].attrib_count = val;
			break;
			case DND_SYNC_ITEMCORRUPTED:
				Charms_Used[pnum][extra].corrupted = val;
			break;
			case DND_SYNC_ITEMQUALITY:
				Charms_Used[pnum][extra].quality = val;
			break;

			case DND_SYNC_ITEMATTRIBUTES_ID:
				Charms_Used[pnum][extra].attributes[sub].attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
				Charms_Used[pnum][extra].attributes[sub].attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
				Charms_Used[pnum][extra].attributes[sub].attrib_tier = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_FRACTURE:
				Charms_Used[pnum][extra].attributes[sub].fractured = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_EXTRA:
				Charms_Used[pnum][extra].attributes[sub].attrib_extra = val;
			break;

			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID:
				Charms_Used[pnum][extra].implicit.attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL:
				Charms_Used[pnum][extra].implicit.attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER:
				Charms_Used[pnum][extra].implicit.attrib_tier = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA:
				Charms_Used[pnum][extra].implicit.attrib_extra = val;
			break;
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_FIELD) {
		switch(which) {
			case DND_SYNC_ITEMWIDTH:
				Inventories_On_Field[extra].width = val;
			break;
			case DND_SYNC_ITEMHEIGHT:
				Inventories_On_Field[extra].height = val;
			break;
			case DND_SYNC_ITEMIMAGE:
				Inventories_On_Field[extra].item_image = val;
			break;
			case DND_SYNC_ITEMTYPE:
				Inventories_On_Field[extra].item_type = val;
			break;
			case DND_SYNC_ITEMSUBTYPE:
				Inventories_On_Field[extra].item_subtype = val;
			break;
			case DND_SYNC_ITEMLEVEL:
				Inventories_On_Field[extra].item_level = val;
			break;
			case DND_SYNC_ITEMSTACK:
				Inventories_On_Field[extra].item_stack = val;
			break;
			case DND_SYNC_ITEMTOPLEFTBOX:
				Inventories_On_Field[extra].topleftboxid = val;
			break;
			case DND_SYNC_ITEMSATTRIBCOUNT:
				Inventories_On_Field[extra].attrib_count = val;
			break;
			case DND_SYNC_ITEMCORRUPTED:
				Inventories_On_Field[extra].corrupted = val;
			break;
			case DND_SYNC_ITEMQUALITY:
				Inventories_On_Field[extra].quality = val;
			break;

			case DND_SYNC_ITEMATTRIBUTES_ID:
				Inventories_On_Field[extra].attributes[sub].attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
				Inventories_On_Field[extra].attributes[sub].attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
				Inventories_On_Field[extra].attributes[sub].attrib_tier = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_FRACTURE:
				Inventories_On_Field[extra].attributes[sub].fractured = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_EXTRA:
				Inventories_On_Field[extra].attributes[sub].attrib_extra = val;
			break;

			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID:
				Inventories_On_Field[extra].implicit.attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL:
				Inventories_On_Field[extra].implicit.attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER:
				Inventories_On_Field[extra].implicit.attrib_tier = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA:
				Inventories_On_Field[extra].implicit.attrib_extra = val;
			break;
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) {
		switch(which) {
			case DND_SYNC_ITEMWIDTH:
				PlayerInventoryList[pnum][extra].width = val;
			break;
			case DND_SYNC_ITEMHEIGHT:
				PlayerInventoryList[pnum][extra].height = val;
			break;
			case DND_SYNC_ITEMIMAGE:
				PlayerInventoryList[pnum][extra].item_image = val;
			break;
			case DND_SYNC_ITEMTYPE:
				PlayerInventoryList[pnum][extra].item_type = val;
			break;
			case DND_SYNC_ITEMSUBTYPE:
				PlayerInventoryList[pnum][extra].item_subtype = val;
			break;
			case DND_SYNC_ITEMLEVEL:
				PlayerInventoryList[pnum][extra].item_level = val;
			break;
			case DND_SYNC_ITEMSTACK:
				PlayerInventoryList[pnum][extra].item_stack = val;
			break;
			case DND_SYNC_ITEMTOPLEFTBOX:
				PlayerInventoryList[pnum][extra].topleftboxid = val;
			break;
			case DND_SYNC_ITEMSATTRIBCOUNT:
				PlayerInventoryList[pnum][extra].attrib_count = val;
			break;
			case DND_SYNC_ITEMCORRUPTED:
				PlayerInventoryList[pnum][extra].corrupted = val;
			break;
			case DND_SYNC_ITEMQUALITY:
				PlayerInventoryList[pnum][extra].quality = val;
			break;
			
			case DND_SYNC_ITEMATTRIBUTES_ID:
				PlayerInventoryList[pnum][extra].attributes[sub].attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
				PlayerInventoryList[pnum][extra].attributes[sub].attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
				PlayerInventoryList[pnum][extra].attributes[sub].attrib_tier = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_FRACTURE:
				PlayerInventoryList[pnum][extra].attributes[sub].fractured = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_EXTRA:
				PlayerInventoryList[pnum][extra].attributes[sub].attrib_extra = val;
			break;

			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID:
				PlayerInventoryList[pnum][extra].implicit.attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL:
				PlayerInventoryList[pnum][extra].implicit.attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER:
				PlayerInventoryList[pnum][extra].implicit.attrib_tier = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA:
				PlayerInventoryList[pnum][extra].implicit.attrib_extra = val;
			break;
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_TRADEVIEW) {
		switch(which) {
			case DND_SYNC_ITEMWIDTH:
				TradeViewList[pnum][extra].width = val;
			break;
			case DND_SYNC_ITEMHEIGHT:
				TradeViewList[pnum][extra].height = val;
			break;
			case DND_SYNC_ITEMIMAGE:
				TradeViewList[pnum][extra].item_image = val;
			break;
			case DND_SYNC_ITEMTYPE:
				TradeViewList[pnum][extra].item_type = val;
			break;
			case DND_SYNC_ITEMSUBTYPE:
				TradeViewList[pnum][extra].item_subtype = val;
			break;
			case DND_SYNC_ITEMLEVEL:
				TradeViewList[pnum][extra].item_level = val;
			break;
			case DND_SYNC_ITEMSTACK:
				TradeViewList[pnum][extra].item_stack = val;
			break;
			case DND_SYNC_ITEMTOPLEFTBOX:
				TradeViewList[pnum][extra].topleftboxid = val;
			break;
			case DND_SYNC_ITEMSATTRIBCOUNT:
				TradeViewList[pnum][extra].attrib_count = val;
			break;
			case DND_SYNC_ITEMCORRUPTED:
				TradeViewList[pnum][extra].corrupted = val;
			break;
			case DND_SYNC_ITEMQUALITY:
				TradeViewList[pnum][extra].quality = val;
			break;

			case DND_SYNC_ITEMATTRIBUTES_ID:
				TradeViewList[pnum][extra].attributes[sub].attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
				TradeViewList[pnum][extra].attributes[sub].attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
				TradeViewList[pnum][extra].attributes[sub].attrib_tier = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_FRACTURE:
				TradeViewList[pnum][extra].attributes[sub].fractured = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_EXTRA:
				TradeViewList[pnum][extra].attributes[sub].attrib_extra = val;
			break;

			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID:
				TradeViewList[pnum][extra].implicit.attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL:
				TradeViewList[pnum][extra].implicit.attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER:
				TradeViewList[pnum][extra].implicit.attrib_tier = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA:
				TradeViewList[pnum][extra].implicit.attrib_extra = val;
			break;
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_STASH) {
		switch(which) {
			case DND_SYNC_ITEMWIDTH:
				PlayerStashList[pnum][page][extra].width = val;
			break;
			case DND_SYNC_ITEMHEIGHT:
				PlayerStashList[pnum][page][extra].height = val;
			break;
			case DND_SYNC_ITEMIMAGE:
				PlayerStashList[pnum][page][extra].item_image = val;
			break;
			case DND_SYNC_ITEMTYPE:
				PlayerStashList[pnum][page][extra].item_type = val;
			break;
			case DND_SYNC_ITEMSUBTYPE:
				PlayerStashList[pnum][page][extra].item_subtype = val;
			break;
			case DND_SYNC_ITEMLEVEL:
				PlayerStashList[pnum][page][extra].item_level = val;
			break;
			case DND_SYNC_ITEMSTACK:
				PlayerStashList[pnum][page][extra].item_stack = val;
			break;
			case DND_SYNC_ITEMTOPLEFTBOX:
				PlayerStashList[pnum][page][extra].topleftboxid = val;
			break;
			case DND_SYNC_ITEMSATTRIBCOUNT:
				PlayerStashList[pnum][page][extra].attrib_count = val;
			break;
			case DND_SYNC_ITEMCORRUPTED:
				PlayerStashList[pnum][page][extra].corrupted = val;
			break;
			case DND_SYNC_ITEMQUALITY:
				PlayerStashList[pnum][page][extra].quality = val;
			break;
			
			case DND_SYNC_ITEMATTRIBUTES_ID:
				PlayerStashList[pnum][page][extra].attributes[sub].attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
				PlayerStashList[pnum][page][extra].attributes[sub].attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
				PlayerStashList[pnum][page][extra].attributes[sub].attrib_tier = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_FRACTURE:
				PlayerStashList[pnum][page][extra].attributes[sub].fractured = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_EXTRA:
				PlayerStashList[pnum][page][extra].attributes[sub].attrib_extra = val;
			break;

			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID:
				PlayerStashList[pnum][page][extra].implicit.attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL:
				PlayerStashList[pnum][page][extra].implicit.attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER:
				PlayerStashList[pnum][page][extra].implicit.attrib_tier = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA:
				PlayerStashList[pnum][page][extra].implicit.attrib_extra = val;
			break;
		}
	}
}

Script "DND Clientside Item Syncer" (int pnum, int var, int to, int extra) CLIENTSIDE {
	/*if(ConsolePlayerNumber() != pnum)
		Terminate;*/
	SetItemSyncValue(pnum, var & 0xFF, extra & 0xFFFF, extra >> 16, to, ((var & 0xFF00) >> 8) | (var & 0xFF0000));
	SetResultValue(0);
}

Script "DND Clientside Item Syncer Special" (int pnum, int var, int to, int extra) CLIENTSIDE {
	/*if(ConsolePlayerNumber() != pnum)
		Terminate;*/
	Delay(1);
	/*printbold(
		s:"calling sync value for pnum ", d:pnum, s:" cpnum: ", d:ConsolePlayerNumber(),
		s: " params: ", d:var & 0xFF, s: " ", d:extra & 0xFFFF, s: " ", d:extra >> 16, s:" ", d: to, s:" ", d: ((var & 0xFF00) >> 8) | (var & 0xFF0000)
	);*/
	SetItemSyncValue(pnum, var & 0xFF, extra & 0xFFFF, extra >> 16, to, ((var & 0xFF00) >> 8) | (var & 0xFF0000));
	SetResultValue(0);
}

Script "DND Clientside Item Syncer Field" (int var, int to, int extra) CLIENTSIDE {
	SetItemSyncValue(-1, var & 0xFF, extra & 0xFFFF, extra >> 16, to, ((var & 0xFF00) >> 8) | (var & 0xFF0000));
	SetResultValue(0);
}

Script "DND Clientside Weapon Mod Sync" (int wepid, int mod, int val, int tier) CLIENTSIDE {
	int pnum = PlayerNumber();
	
	/*if(ConsolePlayerNumber() != pnum)
		Terminate;*/
	
	Player_Weapon_Infos[pnum][wepid].wep_mods[mod].val = val;
	Player_Weapon_Infos[pnum][wepid].wep_mods[mod].tier = tier;
	SetResultValue(0);
}

// add more things from wep_info_T in WeaponsDef here later
Script "DnD Clientside Weapon Property Sync" (int wepid, int prop, int val) CLIENTSIDE {
	// do a switch-case for properties here
	int pnum = PlayerNumber();
	Player_Weapon_Infos[pnum][wepid].quality = val;
	SetResultValue(0);
}

void SyncClientsideVariable_WeaponProperties(int pnum, int wepid) {
	// do a for loop for all properties we might add here to wep_info_T
	ACS_NamedExecuteWithResult("DnD Clientside Weapon Property Sync", wepid, 0, Player_Weapon_Infos[pnum][wepid].quality);
}

void SyncClientsideVariable_WeaponMods(int pnum, int wepid) {
	for(int i = 0; i < MAX_WEP_MODS; ++i) {
		ACS_NamedExecuteWithResult("DND Clientside Weapon Mod Sync", wepid, i, Player_Weapon_Infos[pnum][wepid].wep_mods[i].val, Player_Weapon_Infos[pnum][wepid].wep_mods[i].tier);
	}
}

void SyncItemData(int pnum, int itemid, int source, int wprev, int hprev) {
	int i, j, h, bid;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	// synchronize the topleftboxid for all adjacent ones
	if(IsSourceInventoryView(raw_source)) {
		int w;
		// we must know previous height/width for proper sync
		if(wprev != -1)
			w = wprev;
		else
			w = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, itemid, -1, source);
		if(hprev != -1)
			h = hprev;
		else
			h = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, itemid, -1, source);
		for(i = 0; i < h; ++i)
			for(j = 0; j < w; ++j) {
				bid = itemid + j + i * MAXINVENTORYBLOCKS_VERT;
				ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), bid);
				ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), bid);
			}
	}
	else {
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), itemid);
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), itemid);
	}
	
	// skip top left box and item type, we handled it
	for(i = DND_SYNC_ITEMBEGIN + 2; i <= DND_SYNC_ITEMSATTRIBCOUNT ; ++i)
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, i | payload, GetItemSyncValue(pnum, i, itemid, -1, source), itemid);
	
	h = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source);
	for(i = 0; i < h; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
	}
}

void SyncItemData_Special(int pnum, int itemid, int source) {
	int i, j, bid;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	
	int w = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, itemid, -1, source);
	int h = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, itemid, -1, source);
	
	// synchronize the topleftboxid for all adjacent ones
	if(IsSourceInventoryView(raw_source)) {
		for(i = 0; i < h; ++i)
			for(j = 0; j < w; ++j) {
				bid = itemid + j + i * MAXINVENTORYBLOCKS_VERT;
				ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), bid);
				ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), bid);
			}
	}
	else {
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), itemid);
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), itemid);
	}
	
	// skip top left box and item type, we handled it
	for(i = DND_SYNC_ITEMBEGIN + 2; i <= DND_SYNC_ITEMSATTRIBCOUNT ; ++i)
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, i | payload, GetItemSyncValue(pnum, i, itemid, -1, source), itemid);
	
	h = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source);
	for(i = 0; i < h; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
	}
}

// this is a sync function that syncs data to all players
void SyncItemData_Field(int itemid) {
	int i, j, h;
	int payload = (DND_SYNC_ITEMSOURCE_FIELD << 8);
	// topleftboxid is 0 for field items, it doesnt make sense for them to have one
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer Field", DND_SYNC_ITEMTOPLEFTBOX | payload, 0, itemid);
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer Field", DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(-1, DND_SYNC_ITEMTYPE, itemid, -1, DND_SYNC_ITEMSOURCE_FIELD), itemid);
	
	// skip top left box and item type, we handled it
	for(i = DND_SYNC_ITEMBEGIN + 2; i <= DND_SYNC_ITEMSATTRIBCOUNT ; ++i)
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer Field", i | payload, GetItemSyncValue(-1, i, itemid, -1, DND_SYNC_ITEMSOURCE_FIELD), itemid);
	
	h = GetItemSyncValue(-1, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, DND_SYNC_ITEMSOURCE_FIELD);
	for(i = 0; i < h; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer Field", j | payload, GetItemSyncValue(-1, j, itemid, i, DND_SYNC_ITEMSOURCE_FIELD), itemid | (i << 16));
	}
}

void SyncItemStack(int pnum, int itemid, int source) {
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMSTACK | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, itemid, -1, source), itemid);
}

void SyncItemData_Null(int pnum, int itemid, int source, int wprev, int hprev) {
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	
	if(IsSourceInventoryView(raw_source)) {
		int h, w;
		if(wprev != -1)
			w = wprev;
		else
			w = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, itemid, -1, source);
		if(hprev != -1)
			h = hprev;
		else
			h = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, itemid, -1, source);
		
		for(int i = 0; i < h; ++i)
			for(int j = 0; j < w; ++j) {
				ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, 0, itemid + j + i * MAXINVENTORYBLOCKS_VERT);
				ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTYPE | payload, DND_ITEM_NULL, itemid + j + i * MAXINVENTORYBLOCKS_VERT);
			}
	}
	else {
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, 0, itemid);
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTYPE | payload, DND_ITEM_NULL, itemid);
	}
	
	h = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source);
	
	// skip top left box and item type, we handled it
	for(i = DND_SYNC_ITEMBEGIN + 2; i <= DND_SYNC_ITEMSATTRIBCOUNT ; ++i)
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, i | payload, 0, itemid);
		
	for(i = 0; i < h; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, j | payload, 0, itemid | (i << 16));
	}
}

void SyncItemAttributes(int pnum, int itemid, int source) {
	int i, j, temp;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMSATTRIBCOUNT | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source), itemid);
	
	temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source);
	for(i = 0; i < temp; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
	}
}

void SyncItemQuality(int pnum, int itemid, int source) {
	int i, j, temp;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMQUALITY | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, itemid, -1, source), itemid);
}

void SyncItemImplicits(int pnum, int itemid, int source) {
	int i;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);

	ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMCORRUPTED | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, itemid, -1, source), itemid);
	
	for(i = DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID; i <= DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA ; ++i)
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, i | payload, GetItemSyncValue(pnum, i, itemid, -1, source), itemid);
}

void SyncAllItemData(int pnum, int source) {
	int i, j;
	if(source == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) {
		for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
			if(PlayerInventoryList[pnum][i].item_type != DND_ITEM_NULL)
				SyncItemData(pnum, i, source, 1, 1);
			else
				SyncItemData_Null(pnum, i, source, 1, 1);
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_CHARMUSED) {
		for(i = 0; i < MAX_CHARMS_EQUIPPABLE; ++i) {
			if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL)
				SyncItemData(pnum, i, source, 1, 1);
			else
				SyncItemData_Null(pnum, i, source, 1, 1);
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_STASH) {
		for(i = 0; i < CheckInventory("DnD_PlayerInventoryPages"); ++i) {
			for(j = 0; j < MAX_INVENTORY_BOXES; ++j) {
				if(PlayerStashList[pnum][i][j].item_type != DND_ITEM_NULL)
					SyncItemData(pnum, j, source | (i << 16), 1, 1);
				else
					SyncItemData_Null(pnum, j, source | (i << 16), 1, 1);
			}
		}
	}
}

void SyncAllClientsideVariables(int pnum) {
	int i, j;
	// sync weapon mods
	for(i = 0; i < MAXWEPS; ++i) {
		SyncClientsideVariable_WeaponProperties(pnum, i);
		SyncClientsideVariable_WeaponMods(pnum, i);
	}
}

Script "DnD Request Mod Sync" (int pnum, int mod, int val) CLIENTSIDE {
	// Log(s:"cs set mod ", d:mod, s: " to val ", d:val);
	PlayerModValues[pnum][mod] = val;
	SetResultValue(0);
}

Script "DnD Request Mod Extra Sync" (int pnum, int mod, int val) CLIENTSIDE {
	// Log(s:"cs set mod ", d:mod, s: " to val ", d:val);
	PlayerModExtras[pnum][mod] = val;
	SetResultValue(0);
}

#endif
