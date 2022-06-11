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
	
	DND_SYNC_DAMAGEBULLET,
	DND_SYNC_DAMAGEMELEE,
	DND_SYNC_DAMAGEOCCULT,
	DND_SYNC_DAMAGEEXPLOSIVE,
	DND_SYNC_DAMAGEENERGY,
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
	// add non attribute related things from above attrib count
	DND_SYNC_ITEMSATTRIBCOUNT,
	DND_SYNC_ITEMATTRIBUTES_ID,
	DND_SYNC_ITEMATTRIBUTES_VAL,
	DND_SYNC_ITEMATTRIBUTES_TIER
	// add attribute related things from below here
};

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
			case DND_SYNC_ITEMATTRIBUTES_ID:
			return Charms_Used[pnum][extra].attributes[sub].attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
			return Charms_Used[pnum][extra].attributes[sub].attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
			return Charms_Used[pnum][extra].attributes[sub].attrib_tier;
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
			case DND_SYNC_ITEMATTRIBUTES_ID:
			return Inventories_On_Field[extra].attributes[sub].attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
			return Inventories_On_Field[extra].attributes[sub].attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
			return Inventories_On_Field[extra].attributes[sub].attrib_tier;
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
			case DND_SYNC_ITEMATTRIBUTES_ID:
			return PlayerInventoryList[pnum][extra].attributes[sub].attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
			return PlayerInventoryList[pnum][extra].attributes[sub].attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
			return PlayerInventoryList[pnum][extra].attributes[sub].attrib_tier;
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
			case DND_SYNC_ITEMATTRIBUTES_ID:
			return TradeViewList[pnum][extra].attributes[sub].attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
			return TradeViewList[pnum][extra].attributes[sub].attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
			return TradeViewList[pnum][extra].attributes[sub].attrib_tier;
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
			case DND_SYNC_ITEMATTRIBUTES_ID:
			return PlayerStashList[pnum][page][extra].attributes[sub].attrib_id;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
			return PlayerStashList[pnum][page][extra].attributes[sub].attrib_val;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
			return PlayerStashList[pnum][page][extra].attributes[sub].attrib_tier;
		}
	}
	return 0;
}

int GetPlayerSyncValue_Orb(int pos, int extra) {
	int pnum = PlayerNumber();
	switch(pos) {
		case DND_SYNC_WEAPONENHANCE:
		return GetDataFromOrbBonus(pnum, OBI_WEAPON_ENCHANT, extra);
		case DND_SYNC_SPEED:
		return GetDataFromOrbBonus(pnum, OBI_SPEED, -1);
		case DND_SYNC_DROPCHANCE:
		return GetDataFromOrbBonus(pnum, OBI_DROPCHANCE, -1);
		case DND_SYNC_HPFLAT_BONUS:
		return GetDataFromOrbBonus(pnum, OBI_HPFLAT, -1);
		case DND_SYNC_ARMORFLAT_BONUS:
		return GetDataFromOrbBonus(pnum, OBI_ARMORFLAT, -1);
		case DND_SYNC_HPPERCENT_BONUS:
		return GetDataFromOrbBonus(pnum, OBI_HPPERCENT, -1);
		case DND_SYNC_ARMORPERCENT_BONUS:
		return GetDataFromOrbBonus(pnum, OBI_ARMORPERCENT, -1);
		case DND_SYNC_GREEDPERCENT_BONUS:
		return GetDataFromOrbBonus(pnum, OBI_GREEDPERCENT, -1);
		case DND_SYNC_WISDOMPERCENT_BONUS:
		return GetDataFromOrbBonus(pnum, OBI_WISDOMPERCENT, -1);
		case DND_SYNC_HOLDING:
		return GetDataFromOrbBonus(pnum, OBI_HOLDING, -1);
		case DND_SYNC_DAMAGEBULLET:
		return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, DND_DAMAGECATEGORY_BULLET);
		case DND_SYNC_DAMAGEENERGY:
		return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, DND_DAMAGECATEGORY_ENERGY);
		case DND_SYNC_DAMAGEEXPLOSIVE:
		return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, DND_DAMAGECATEGORY_EXPLOSIVES);
		case DND_SYNC_DAMAGEMELEE:
		return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, DND_DAMAGECATEGORY_MELEE);
		case DND_SYNC_DAMAGEELEMENTAL:
		return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, DND_DAMAGECATEGORY_ELEMENTAL);
		case DND_SYNC_DAMAGEOCCULT:
		return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, DND_DAMAGECATEGORY_OCCULT);
		case DND_SYNC_WEPMOD_CRIT:
		return GetDataFromOrbBonus(pnum, OBI_WEAPON_CRIT, extra);
		case DND_SYNC_WEPMOD_CRITDMG:
		return GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, extra);
		case DND_SYNC_WEPMOD_CRITPERCENT:
		return GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITPERCENT, extra);
		case DND_SYNC_WEPMOD_DMG:
		return GetDataFromOrbBonus(pnum, OBI_WEAPON_DMG, extra);
		case DND_SYNC_WEPMOD_POWERSET1:
		return GetDataFromOrbBonus(pnum, OBI_WEAPON_POWERSET1, extra);
	}
	return 0;
}

int GetPlayerSyncValue_Elixir(int pos, int extra) {
	int pnum = PlayerNumber();
	switch(pos) {
		case DND_SYNC_SPEED:
		return Player_Elixir_Bonuses[pnum].speed_bonus;
		case DND_SYNC_HPFLAT_BONUS:
		return Player_Elixir_Bonuses[pnum].hp_flat_bonus;
		case DND_SYNC_ARMORFLAT_BONUS:
		return Player_Elixir_Bonuses[pnum].armor_flat_bonus;
		case DND_SYNC_HPPERCENT_BONUS:
		return Player_Elixir_Bonuses[pnum].hp_percent_bonus;
		case DND_SYNC_ARMORPERCENT_BONUS:
		return Player_Elixir_Bonuses[pnum].armor_percent_bonus;
		case DND_SYNC_DAMAGEBULLET:
		return Player_Elixir_Bonuses[pnum].damage_type_bonus[DND_DAMAGECATEGORY_BULLET];
		case DND_SYNC_DAMAGEENERGY:
		return Player_Elixir_Bonuses[pnum].damage_type_bonus[DND_DAMAGECATEGORY_ENERGY];
		case DND_SYNC_DAMAGEEXPLOSIVE:
		return Player_Elixir_Bonuses[pnum].damage_type_bonus[DND_DAMAGECATEGORY_EXPLOSIVES];
		case DND_SYNC_DAMAGEMELEE:
		return Player_Elixir_Bonuses[pnum].damage_type_bonus[DND_DAMAGECATEGORY_MELEE];
		case DND_SYNC_DAMAGEELEMENTAL:
		return Player_Elixir_Bonuses[pnum].damage_type_bonus[DND_DAMAGECATEGORY_ELEMENTAL];
		case DND_SYNC_DAMAGEOCCULT:
		return Player_Elixir_Bonuses[pnum].damage_type_bonus[DND_DAMAGECATEGORY_OCCULT];
		case DND_SYNC_LUCK:
		return Player_Elixir_Bonuses[pnum].luck = 0;
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
			case DND_SYNC_ITEMATTRIBUTES_ID:
				Charms_Used[pnum][extra].attributes[sub].attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
				Charms_Used[pnum][extra].attributes[sub].attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
				Charms_Used[pnum][extra].attributes[sub].attrib_tier = val;
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
			case DND_SYNC_ITEMATTRIBUTES_ID:
				Inventories_On_Field[extra].attributes[sub].attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
				Inventories_On_Field[extra].attributes[sub].attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
				Inventories_On_Field[extra].attributes[sub].attrib_tier = val;
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
			case DND_SYNC_ITEMATTRIBUTES_ID:
				PlayerInventoryList[pnum][extra].attributes[sub].attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
				PlayerInventoryList[pnum][extra].attributes[sub].attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
				PlayerInventoryList[pnum][extra].attributes[sub].attrib_tier = val;
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
			case DND_SYNC_ITEMATTRIBUTES_ID:
				TradeViewList[pnum][extra].attributes[sub].attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
				TradeViewList[pnum][extra].attributes[sub].attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
				TradeViewList[pnum][extra].attributes[sub].attrib_tier = val;
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
			case DND_SYNC_ITEMATTRIBUTES_ID:
				PlayerStashList[pnum][page][extra].attributes[sub].attrib_id = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_VAL:
				PlayerStashList[pnum][page][extra].attributes[sub].attrib_val = val;
			break;
			case DND_SYNC_ITEMATTRIBUTES_TIER:
				PlayerStashList[pnum][page][extra].attributes[sub].attrib_tier = val;
			break;
		}
	}
}

void SetSyncValue_Orb(int pnum, int pos, int val, int extra) {
	switch(pos) {
		case DND_SYNC_WEAPONENHANCE:
			SetDataToOrbBonus(pnum, OBI_WEAPON_ENCHANT, extra, val);
		break;
		case DND_SYNC_SPEED:
			SetDataToOrbBonus(pnum, OBI_SPEED, -1, val);
		break;
		case DND_SYNC_DROPCHANCE:
			SetDataToOrbBonus(pnum, OBI_DROPCHANCE, -1, val);
		break;
		case DND_SYNC_HPFLAT_BONUS:
			SetDataToOrbBonus(pnum, OBI_HPFLAT, -1, val);
		break;
		case DND_SYNC_ARMORFLAT_BONUS:
			SetDataToOrbBonus(pnum, OBI_ARMORFLAT, -1, val);
		break;
		case DND_SYNC_HPPERCENT_BONUS:
			SetDataToOrbBonus(pnum, OBI_HPPERCENT, -1, val);
		break;
		case DND_SYNC_ARMORPERCENT_BONUS:
			SetDataToOrbBonus(pnum, OBI_ARMORPERCENT, -1, val);
		break;
		case DND_SYNC_GREEDPERCENT_BONUS:
			SetDataToOrbBonus(pnum, OBI_GREEDPERCENT, -1, val);
		break;
		case DND_SYNC_WISDOMPERCENT_BONUS:
			SetDataToOrbBonus(pnum, OBI_WISDOMPERCENT, -1, val);
		break;
		case DND_SYNC_HOLDING:
			SetDataToOrbBonus(pnum, OBI_HOLDING, -1, val);
		break;
		case DND_SYNC_DAMAGEBULLET:
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, DND_DAMAGECATEGORY_BULLET, val);
		break;
		case DND_SYNC_DAMAGEENERGY:
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, DND_DAMAGECATEGORY_ENERGY, val);
		break;
		case DND_SYNC_DAMAGEEXPLOSIVE:
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, DND_DAMAGECATEGORY_EXPLOSIVES, val);
		break;
		case DND_SYNC_DAMAGEMELEE:
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, DND_DAMAGECATEGORY_MELEE, val);
		break;
		case DND_SYNC_DAMAGEELEMENTAL:
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, DND_DAMAGECATEGORY_ELEMENTAL, val);
		break;
		case DND_SYNC_DAMAGEOCCULT:
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, DND_DAMAGECATEGORY_OCCULT, val);
		break;
		case DND_SYNC_WEPMOD_CRIT:
			SetDataToOrbBonus(pnum, OBI_WEAPON_CRIT, extra, val);
		break;
		case DND_SYNC_WEPMOD_CRITDMG:
			SetDataToOrbBonus(pnum, OBI_WEAPON_CRITDMG, extra, val);
		break;
		case DND_SYNC_WEPMOD_CRITPERCENT:
			SetDataToOrbBonus(pnum, OBI_WEAPON_CRITPERCENT, extra, val);
		break;
		case DND_SYNC_WEPMOD_DMG:
			SetDataToOrbBonus(pnum, OBI_WEAPON_DMG, extra, val);
		break;
		case DND_SYNC_WEPMOD_POWERSET1:
			// last param is a sign telling it to just overwrite it not bitset
			SetDataToOrbBonus(pnum, OBI_WEAPON_POWERSET1, extra, val, true);
		break;
	}
}

void SetSyncValue_Elixir(int pnum, int pos, int val, int extra) {
	switch(pos) {
		case DND_SYNC_SPEED:
			Player_Elixir_Bonuses[pnum].speed_bonus = val;
		break;
		case DND_SYNC_HPFLAT_BONUS:
			Player_Elixir_Bonuses[pnum].hp_flat_bonus = val;
		break;
		case DND_SYNC_ARMORFLAT_BONUS:
			Player_Elixir_Bonuses[pnum].armor_flat_bonus = val;
		break;
		case DND_SYNC_HPPERCENT_BONUS:
			Player_Elixir_Bonuses[pnum].hp_percent_bonus = val;
		break;
		case DND_SYNC_ARMORPERCENT_BONUS:
			Player_Elixir_Bonuses[pnum].armor_percent_bonus = val;
		break;
		case DND_SYNC_DAMAGEBULLET:
			Player_Elixir_Bonuses[pnum].damage_type_bonus[DND_DAMAGECATEGORY_BULLET] = val;
		break;
		case DND_SYNC_DAMAGEENERGY:
			Player_Elixir_Bonuses[pnum].damage_type_bonus[DND_DAMAGECATEGORY_ENERGY] = val;
		break;
		case DND_SYNC_DAMAGEEXPLOSIVE:
			Player_Elixir_Bonuses[pnum].damage_type_bonus[DND_DAMAGECATEGORY_EXPLOSIVES] = val;
		break;
		case DND_SYNC_DAMAGEMELEE:
			Player_Elixir_Bonuses[pnum].damage_type_bonus[DND_DAMAGECATEGORY_MELEE] = val;
		break;
		case DND_SYNC_DAMAGEELEMENTAL:
			Player_Elixir_Bonuses[pnum].damage_type_bonus[DND_DAMAGECATEGORY_ELEMENTAL] = val;
		break;
		case DND_SYNC_DAMAGEOCCULT:
			Player_Elixir_Bonuses[pnum].damage_type_bonus[DND_DAMAGECATEGORY_OCCULT] = val;
		break;
		case DND_SYNC_LUCK:
			Player_Elixir_Bonuses[pnum].luck = val;
		break;
	}
}

Script "DND Clientside Orb Syncer" (int pnum, int var, int to, int extra) CLIENTSIDE {
	/*if(ConsolePlayerNumber() != pnum)
		Terminate;*/
	SetSyncValue_Orb(pnum, var, to, extra);
	SetResultValue(0);
}

Script "DND Clientside Elixir Syncer" (int pnum, int var, int to, int extra) CLIENTSIDE {
	/*if(ConsolePlayerNumber() != pnum)
		Terminate;*/
	SetSyncValue_Elixir(pnum, var, to, extra);
	SetResultValue(0);
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

void SyncClientsideVariable_Orb(int pnum, int var, int extra) {
	if(var == DND_SYNC_WEAPONENHANCE || (var >= DND_SYNC_WEPMOD_CRIT && var < MAX_SYNC_VARS)) {
		/*if(extra == DND_WEAPON_PISTOL || extra == DND_WEAPON_FIST) {
			printbold(d:extra, s:". ", d:var, s:": ", d:GetPlayerSyncValue_Orb(var, extra));
		}*/
		ACS_NamedExecuteWithResult("DND Clientside Orb Syncer", pnum, var, GetPlayerSyncValue_Orb(var, extra), extra);
	}
	else
		ACS_NamedExecuteWithResult("DND Clientside Orb Syncer", pnum, var, GetPlayerSyncValue_Orb(var, 0), 0);
}

void SyncClientsideVariable_Elixir(int pnum, int var, int extra) {
	if(var == DND_SYNC_WEAPONENHANCE ||(var >= DND_SYNC_WEPMOD_CRIT && var <= DND_SYNC_WEPMOD_DMG))
		ACS_NamedExecuteWithResult("DND Clientside Elixir Syncer", pnum, var, GetPlayerSyncValue_Elixir(var, extra), extra);
	else
		ACS_NamedExecuteWithResult("DND Clientside Elixir Syncer", pnum, var, GetPlayerSyncValue_Elixir(var, 0), 0);
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
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_SYNC_ITEMATTRIBUTES_TIER; ++j)
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
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_SYNC_ITEMATTRIBUTES_TIER; ++j)
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
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_SYNC_ITEMATTRIBUTES_TIER; ++j)
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
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_SYNC_ITEMATTRIBUTES_TIER; ++j)
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
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_SYNC_ITEMATTRIBUTES_TIER; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
	}
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
	// sync orbs
	for(i = 0; i < MAX_SYNC_VARS; ++i) {
		if(i == DND_SYNC_WEAPONENHANCE || (i >= DND_SYNC_WEPMOD_CRIT && i < MAX_SYNC_VARS)) {
			for(j = 0; j < MAXWEPS; ++j) {
				/*if(j == DND_WEAPON_PISTOL || j == DND_WEAPON_FIST) {
					printbold(d:j, s:". ", d:i, s:": ", d:GetPlayerSyncValue_Orb(i, j));
				}*/
				ACS_NamedExecuteWithResult("DND Clientside Orb Syncer", pnum, i, GetPlayerSyncValue_Orb(i, j), j);
			}
		}
		else
			ACS_NamedExecuteWithResult("DND Clientside Orb Syncer", pnum, i, GetPlayerSyncValue_Orb(i, 0), 0);
	}
	// sync elixirs
	for(i = 0; i < MAX_SYNC_VARS; ++i) {
		if(i == DND_SYNC_WEAPONENHANCE || (i >= DND_SYNC_WEPMOD_CRIT && i <= DND_SYNC_WEPMOD_DMG)) {
			for(j = 0; j < MAXWEPS; ++j)
				ACS_NamedExecuteWithResult("DND Clientside Elixir Syncer", pnum, i, GetPlayerSyncValue_Elixir(i, j), j);
		}
		else
			ACS_NamedExecuteWithResult("DND Clientside Elixir Syncer", pnum, i, GetPlayerSyncValue_Elixir(i, 0), 0);
	}
	
	// sync weapon mods
	for(i = 0; i < MAXWEPS; ++i)
		SyncClientsideVariable_WeaponMods(pnum, i);
}

Script "DnD Request Mod Sync" (int pnum, int mod, int val) CLIENTSIDE {
	// Log(s:"cs set mod ", d:mod, s: " to val ", d:val);
	PlayerModValues[pnum][mod] = val;
	SetResultValue(0);
}

#endif
