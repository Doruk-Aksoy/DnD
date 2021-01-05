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
	
	DND_SYNC_WEPBONUS_CRIT,
	DND_SYNC_WEPBONUS_CRITDMG,
	DND_SYNC_WEPBONUS_CRITPERCENT,
	DND_SYNC_WEPBONUS_DMG,
	
	DND_SYNC_ITEMTOPLEFTBOX,
	DND_SYNC_ITEMTYPE,
	DND_SYNC_ITEMSUBTYPE,
	DND_SYNC_ITEMWIDTH,
	DND_SYNC_ITEMHEIGHT,
	DND_SYNC_ITEMIMAGE,
	DND_SYNC_ITEMLEVEL,
	DND_SYNC_ITEMSTACK,
	DND_SYNC_ITEMSATTRIBCOUNT,
	DND_SYNC_ITEMATTRIBUTES_ID,
	DND_SYNC_ITEMATTRIBUTES_VAL
};

#define MAX_SYNC_VARS DND_SYNC_WEPBONUS_DMG + 1

enum {
	DND_SYNC_ITEMSOURCE_CHARMUSED,
	DND_SYNC_ITEMSOURCE_PLAYERINVENTORY,
	DND_SYNC_ITEMSOURCE_FIELD,
	DND_SYNC_ITEMSOURCE_TRADEVIEW,		// trade view array
	DND_SYNC_ITEMSOURCE_STASH
};

int GetItemSyncValue(int which, int extra, int sub, int source) {
	int pnum = PlayerNumber();
	int page = 0;
	if(sub > 65535) { // alternate use for sub
		pnum = (sub >> 16) - 1;
		sub &= 0xFFFF;
	}
	
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
		return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_BULLET);
		case DND_SYNC_DAMAGEENERGY:
		return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_ENERGY);
		case DND_SYNC_DAMAGEEXPLOSIVE:
		return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_EXPLOSIVE);
		case DND_SYNC_DAMAGEMELEE:
		return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_MELEE);
		case DND_SYNC_DAMAGEELEMENTAL:
		return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_ELEMENTAL);
		case DND_SYNC_DAMAGEOCCULT:
		return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_OCCULT);
		case DND_SYNC_WEPBONUS_CRIT:
		return GetDataFromOrbBonus(pnum, OBI_WEAPON_CRIT, extra);
		case DND_SYNC_WEPBONUS_CRITDMG:
		return GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, extra);
		case DND_SYNC_WEPBONUS_CRITPERCENT:
		return GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITPERCENT, extra);
		case DND_SYNC_WEPBONUS_DMG:
		return GetDataFromOrbBonus(pnum, OBI_WEAPON_DMG, extra);
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
		return Player_Elixir_Bonuses[pnum].damage_type_bonus[TALENT_BULLET];
		case DND_SYNC_DAMAGEENERGY:
		return Player_Elixir_Bonuses[pnum].damage_type_bonus[TALENT_ENERGY];
		case DND_SYNC_DAMAGEEXPLOSIVE:
		return Player_Elixir_Bonuses[pnum].damage_type_bonus[TALENT_EXPLOSIVE];
		case DND_SYNC_DAMAGEMELEE:
		return Player_Elixir_Bonuses[pnum].damage_type_bonus[TALENT_MELEE];
		case DND_SYNC_DAMAGEELEMENTAL:
		return Player_Elixir_Bonuses[pnum].damage_type_bonus[TALENT_ELEMENTAL];
		case DND_SYNC_DAMAGEOCCULT:
		return Player_Elixir_Bonuses[pnum].damage_type_bonus[TALENT_OCCULT];
		case DND_SYNC_LUCK:
		return Player_Elixir_Bonuses[pnum].luck = 0;
	}
	return 0;
}

void SetItemSyncValue(int which, int extra, int sub, int val, int source) {
	int pnum = PlayerNumber();
	int page = 0;
	if(sub > 65535) {
		pnum = (sub >> 16) - 1;
		sub &= 0xFFFF;
	}
	
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
		}
	}
}

void SetSyncValue_Orb(int pos, int val, int extra) {
	int pnum = PlayerNumber();
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
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_BULLET, val);
		break;
		case DND_SYNC_DAMAGEENERGY:
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_ENERGY, val);
		break;
		case DND_SYNC_DAMAGEEXPLOSIVE:
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_EXPLOSIVE, val);
		break;
		case DND_SYNC_DAMAGEMELEE:
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_MELEE, val);
		break;
		case DND_SYNC_DAMAGEELEMENTAL:
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_ELEMENTAL, val);
		break;
		case DND_SYNC_DAMAGEOCCULT:
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_OCCULT, val);
		break;
		case DND_SYNC_WEPBONUS_CRIT:
			SetDataToOrbBonus(pnum, OBI_WEAPON_CRIT, extra, val);
		break;
		case DND_SYNC_WEPBONUS_CRITDMG:
			SetDataToOrbBonus(pnum, OBI_WEAPON_CRITDMG, extra, val);
		break;
		case DND_SYNC_WEPBONUS_CRITPERCENT:
			SetDataToOrbBonus(pnum, OBI_WEAPON_CRITPERCENT, extra, val);
		break;
		case DND_SYNC_WEPBONUS_DMG:
			SetDataToOrbBonus(pnum, OBI_WEAPON_DMG, extra, val);
		break;
	}
}

void SetSyncValue_Elixir(int pos, int val, int extra) {
	int pnum = PlayerNumber();
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
			Player_Elixir_Bonuses[pnum].damage_type_bonus[TALENT_BULLET] = val;
		break;
		case DND_SYNC_DAMAGEENERGY:
			Player_Elixir_Bonuses[pnum].damage_type_bonus[TALENT_ENERGY] = val;
		break;
		case DND_SYNC_DAMAGEEXPLOSIVE:
			Player_Elixir_Bonuses[pnum].damage_type_bonus[TALENT_EXPLOSIVE] = val;
		break;
		case DND_SYNC_DAMAGEMELEE:
			Player_Elixir_Bonuses[pnum].damage_type_bonus[TALENT_MELEE] = val;
		break;
		case DND_SYNC_DAMAGEELEMENTAL:
			Player_Elixir_Bonuses[pnum].damage_type_bonus[TALENT_ELEMENTAL] = val;
		break;
		case DND_SYNC_DAMAGEOCCULT:
			Player_Elixir_Bonuses[pnum].damage_type_bonus[TALENT_OCCULT] = val;
		break;
		case DND_SYNC_LUCK:
			Player_Elixir_Bonuses[pnum].luck = val;
		break;
	}
}

void SyncClientsideVariable_Orb(int var, int extra) {
	if(var == DND_SYNC_WEAPONENHANCE ||(var >= DND_SYNC_WEPBONUS_CRIT && var <= DND_SYNC_WEPBONUS_DMG))
		ACS_NamedExecuteAlways("DND Clientside Orb Syncer", 0, var, GetPlayerSyncValue_Orb(var, extra), extra);
	else
		ACS_NamedExecuteAlways("DND Clientside Orb Syncer", 0, var, GetPlayerSyncValue_Orb(var, 0), 0);
}

void SyncClientsideVariable_Elixir(int var, int extra) {
	if(var == DND_SYNC_WEAPONENHANCE ||(var >= DND_SYNC_WEPBONUS_CRIT && var <= DND_SYNC_WEPBONUS_DMG))
		ACS_NamedExecuteAlways("DND Clientside Elixir Syncer", 0, var, GetPlayerSyncValue_Elixir(var, extra), extra);
	else
		ACS_NamedExecuteAlways("DND Clientside Elixir Syncer", 0, var, GetPlayerSyncValue_Elixir(var, 0), 0);
}

void SyncItemData(int itemid, int source, int wprev, int hprev) {
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
			w = GetItemSyncValue(DND_SYNC_ITEMWIDTH, itemid, -1, source);
		if(hprev != -1)
			h = hprev;
		else
			h = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, itemid, -1, source);
	
		for(i = 0; i < h; ++i)
			for(j = 0; j < w; ++j) {
				bid = itemid + j + i * MAXINVENTORYBLOCKS_VERT;
				ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source), bid);
				ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(DND_SYNC_ITEMTYPE, bid, -1, source), bid);
			}
	}
	else {
		ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), itemid);
		ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(DND_SYNC_ITEMTYPE, itemid, -1, source), itemid);
	}
	
	// skip top left box and item type, we handled it
	for(i = DND_SYNC_ITEMBEGIN + 2; i <= DND_SYNC_ITEMSATTRIBCOUNT ; ++i)
		ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, i | payload, GetItemSyncValue(i, itemid, -1, source), itemid);
	
	h = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source);
	for(i = 0; i < h; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_SYNC_ITEMATTRIBUTES_VAL; ++j)
			ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, j | payload, GetItemSyncValue(j, itemid, i, source), itemid | (i << 16));
	}
}

void SyncItemStack(int itemid, int source) {
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMSTACK | payload, GetItemSyncValue(DND_SYNC_ITEMSTACK, itemid, -1, source), itemid);
}

// this runs a specialized way of sending the player number in the input as well
void SyncItemData_Player(int itemid, int source, int wprev, int hprev, int pnum) {
	int i, j, h, bid;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = ((pnum + 1) << 8) | (raw_source << 16) | (page << 24);
	// synchronize the topleftboxid for all adjacent ones
	if(IsSourceInventoryView(raw_source)) {
		int w;
		// we must know previous height/width for proper sync
		if(wprev != -1)
			w = wprev;
		else
			w = GetItemSyncValue(DND_SYNC_ITEMWIDTH, itemid, (pnum + 1) << 16, source);
		if(hprev != -1)
			h = hprev;
		else
			h = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, itemid, (pnum + 1) << 16, source);
	
		for(i = 0; i < h; ++i)
			for(j = 0; j < w; ++j) {
				bid = itemid + j + i * MAXINVENTORYBLOCKS_VERT;
				ACS_NamedExecuteAlways("DND Clientside Item Syncer Player", 0, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, bid, (pnum + 1) << 16, source), bid);
				ACS_NamedExecuteAlways("DND Clientside Item Syncer Player", 0, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(DND_SYNC_ITEMTYPE, bid, (pnum + 1) << 16, source), bid);
			}
	}
	else {
		ACS_NamedExecuteAlways("DND Clientside Item Syncer Player", 0, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, itemid, (pnum + 1) << 16, source), itemid);
		ACS_NamedExecuteAlways("DND Clientside Item Syncer Player", 0, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(DND_SYNC_ITEMTYPE, itemid, (pnum + 1) << 16, source), itemid);
	}
	
	// skip top left box and item type, we handled it
	for(i = DND_SYNC_ITEMBEGIN + 2; i <= DND_SYNC_ITEMSATTRIBCOUNT ; ++i)
		ACS_NamedExecuteAlways("DND Clientside Item Syncer Player", 0, i | payload, GetItemSyncValue(i, itemid, (pnum + 1) << 16, source), itemid);
	
	h = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, itemid, (pnum + 1) << 16, source);
	for(i = 0; i < h; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_SYNC_ITEMATTRIBUTES_VAL; ++j)
			ACS_NamedExecuteAlways("DND Clientside Item Syncer Player", 0, j | payload, GetItemSyncValue(j, itemid, i | ((pnum + 1) << 16), source), itemid | (i << 16));
	}
}

void SyncItemData_Null(int itemid, int source, int wprev, int hprev) {
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	if(IsSourceInventoryView(raw_source)) {
		int h, w;
		if(wprev != -1)
			w = wprev;
		else
			w = GetItemSyncValue(DND_SYNC_ITEMWIDTH, itemid, -1, source);
		if(hprev != -1)
			h = hprev;
		else
			h = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, itemid, -1, source);
	
		for(int i = 0; i < h; ++i)
			for(int j = 0; j < w; ++j) {
				ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMTOPLEFTBOX | payload, 0, itemid + j + i * MAXINVENTORYBLOCKS_VERT);
				ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMTYPE | payload, DND_ITEM_NULL, itemid + j + i * MAXINVENTORYBLOCKS_VERT);
			}
	}
	else {
		ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMTOPLEFTBOX | payload, 0, itemid);
		ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMTYPE | payload, DND_ITEM_NULL, itemid);
	}
}

void SyncItemData_Null_Player(int itemid, int source, int wprev, int hprev, int pnum) {
	int h, w;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = ((pnum + 1) << 8) | (raw_source << 16) | (page << 24);
	if(wprev != -1)
		w = wprev;
	else
		w = GetItemSyncValue(DND_SYNC_ITEMWIDTH, itemid, (pnum + 1) << 16, source);
	if(hprev != -1)
		h = hprev;
	else
		h = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, itemid, (pnum + 1) << 16, source);
	
	for(int i = 0; i < h; ++i)
		for(int j = 0; j < w; ++j) {
			ACS_NamedExecuteAlways("DND Clientside Item Syncer Player", 0, DND_SYNC_ITEMTOPLEFTBOX | payload, 0, itemid + j + i * MAXINVENTORYBLOCKS_VERT);
			ACS_NamedExecuteAlways("DND Clientside Item Syncer Player", 0, DND_SYNC_ITEMTYPE | payload, DND_ITEM_NULL, itemid + j + i * MAXINVENTORYBLOCKS_VERT);
		}
}

// a more efficient syncer for the server
void SyncItemPointers(int itemid, int source, int wprev, int hprev) {
	int i, j, bid;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	// synchronize the topleftboxid for all adjacent ones
	if(IsSourceInventoryView(raw_source)) {
		int w, h;
		// we must know previous height/width for proper sync
		if(wprev != -1)
			w = wprev;
		else
			w = GetItemSyncValue(DND_SYNC_ITEMWIDTH, itemid, -1, source);
		if(hprev != -1)
			h = hprev;
		else
			h = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, itemid, -1, source);
	
		for(i = 0; i < h; ++i)
			for(j = 0; j < w; ++j) {
				bid = itemid + j + i * MAXINVENTORYBLOCKS_VERT;
				ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source), bid);
				ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(DND_SYNC_ITEMTYPE, bid, -1, source), bid);
			}
	}
	else {
		ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), itemid);
		ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(DND_SYNC_ITEMTYPE, itemid, -1, source), itemid);
	}
}

// this runs a specialized way of sending the player number in the input as well
void SyncItemPointers_Player(int itemid, int source, int wprev, int hprev, int pnum) {
	int i, j, h, w, bid;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = ((pnum + 1) << 8) | (raw_source << 16) | (page << 24);
	// we must know previous height/width for proper sync
	if(wprev != -1)
		w = wprev;
	else
		w = GetItemSyncValue(DND_SYNC_ITEMWIDTH, itemid, (pnum + 1) << 16, source);
	if(hprev != -1)
		h = hprev;
	else
		h = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, itemid, (pnum + 1) << 16, source);
		
	// synchronize the topleftboxid for all adjacent ones
	if(IsSourceInventoryView(raw_source)) {
		for(i = 0; i < h; ++i)
			for(j = 0; j < w; ++j) {
				bid = itemid + j + i * MAXINVENTORYBLOCKS_VERT;
				ACS_NamedExecuteAlways("DND Clientside Item Syncer Player", 0, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, bid, (pnum + 1) << 16, source), bid);
				ACS_NamedExecuteAlways("DND Clientside Item Syncer Player", 0, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(DND_SYNC_ITEMTYPE, bid, (pnum + 1) << 16, source), bid);
			}
	}
	else {
		ACS_NamedExecuteAlways("DND Clientside Item Syncer Player", 0, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, itemid, (pnum + 1) << 16, source), itemid);
		ACS_NamedExecuteAlways("DND Clientside Item Syncer Player", 0, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(DND_SYNC_ITEMTYPE, itemid, (pnum + 1) << 16, source), itemid);
	}
}

void SyncItemAttributes(int itemid, int source) {
	int i, j, temp;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, DND_SYNC_ITEMSATTRIBCOUNT | payload, GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source), itemid);
	
	temp = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source);
	for(i = 0; i < temp; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_SYNC_ITEMATTRIBUTES_VAL; ++j)
			ACS_NamedExecuteAlways("DND Clientside Item Syncer", 0, j | payload, GetItemSyncValue(j, itemid, i, source), itemid | (i << 16));
	}
}

void SyncAllItemData(int source) {
	int i, j, pnum = PlayerNumber();
	if(source == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) {
		for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
			if(PlayerInventoryList[pnum][i].item_type != DND_ITEM_NULL)
				SyncItemData(i, source, -1, -1);
			else
				SyncItemData_Null(i, source, 1, 1);
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_CHARMUSED) {
		for(i = 0; i < MAX_CHARMS_EQUIPPABLE; ++i) {
			if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL)
				SyncItemData(i, source, -1, -1);
			else
				SyncItemData_Null(i, source, 1, 1);
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_STASH) {
		for(i = 0; i < CheckInventory("DnD_PlayerInventoryPages"); ++i) {
			for(j = 0; j < MAX_INVENTORY_BOXES; ++j) {
				if(PlayerStashList[pnum][i][j].item_type != DND_ITEM_NULL)
					SyncItemData(j, source | (i << 16), -1, -1);
				else
					SyncItemData_Null(j, source | (i << 16), 1, 1);
			}
		}
	}
}

void SyncAllClientsideVariables() {
	int i, j;
	// sync orbs
	for(i = 0; i < MAX_SYNC_VARS; ++i) {
		if(i == DND_SYNC_WEAPONENHANCE || (i >= DND_SYNC_WEPBONUS_CRIT && i <= DND_SYNC_WEPBONUS_DMG)) {
			for(j = 0; j < MAXWEPS; ++j)
				ACS_NamedExecuteAlways("DND Clientside Orb Syncer", 0, i, GetPlayerSyncValue_Orb(i, j), j);
		}
		else
			ACS_NamedExecuteAlways("DND Clientside Orb Syncer", 0, i, GetPlayerSyncValue_Orb(i, 0), 0);
	}
	// sync elixirs
	for(i = 0; i < MAX_SYNC_VARS; ++i) {
		if(i == DND_SYNC_WEAPONENHANCE || (i >= DND_SYNC_WEPBONUS_CRIT && i <= DND_SYNC_WEPBONUS_DMG)) {
			for(j = 0; j < MAXWEPS; ++j)
				ACS_NamedExecuteAlways("DND Clientside Elixir Syncer", 0, i, GetPlayerSyncValue_Elixir(i, j), j);
		}
		else
			ACS_NamedExecuteAlways("DND Clientside Elixir Syncer", 0, i, GetPlayerSyncValue_Elixir(i, 0), 0);
	}
}

#endif
