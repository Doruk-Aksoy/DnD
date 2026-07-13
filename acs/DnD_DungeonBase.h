#ifndef DND_DUNGEONBASE_IN
#define DND_DUNGEONBASE_IN

enum {
	DND_DUNGEON_VOIDKEEP,
};
#define DND_FIRST_DUNGEONID DND_DUNGEON_VOIDKEEP

#define MAX_DUNGEONKEY_ATTRIB_DEFAULT 3

enum {
	DUN_UPSIDE_QUANT,
	DUN_UPSIDE_RARITY,
	DUN_UPSIDE_EXPANDCREDIT,
	DUN_UPSIDE_BUDGET,
	DUN_UPSIDE_RARERCHEST,
	DUN_UPSIDE_RARERORBS,
	DUN_UPSIDE_MERCHANTCHANCE,

	DUN_UPSIDE_MAX
};
#define FIRST_DUNGEON_UPSIDE DUN_UPSIDE_QUANT

typedef struct {
	int dungeon_id;									// # of the dungeon map (DND0X etc)
	int quality;									// quality is a multiplier for the upsides and downsides
	int level;										// level of drops / monsters
	int attrib_count;
	attr_inf_T attributes[MAX_ITEM_ATTRIBUTES];		// attribute list of the dungeon (% increased elite chance etc. stuff like that)
	int upside_vals[DUN_UPSIDE_MAX];				// sums of upside values if any applicable
	str next_map;
} dungeon_data_T;

global dungeon_data_T 38: DungeonInformation;

enum {
	DUN_ATTR_EXTRAHP,
	DUN_ATTR_FORTIFIED,
	DUN_ATTR_MORETOUGHENEMIES,
	DUN_ATTR_MOREELITEENEMIES,
	DUN_ATTR_CULLENEMIES,
	DUN_ATTR_NOINFIGHT,
	DUN_ATTR_NOPAIN,
	DUN_ATTR_NORIP,
	DUN_ATTR_EXTRAFAST,
	DUN_ATTR_MOREDMG,
	DUN_ATTR_FASTPROJ,
	DUN_ATTR_GHOST,
	DUN_ATTR_INCREASEDRESISTS,

	DUN_ATTR_MAX
};
#define FIRST_DUNGEON_ATTRIBUTE DUN_ATTR_EXTRAHP

typedef struct {
	inv_attrib_T DungeonModTable[DUN_ATTR_MAX];
	inv_attrib_T DungeonUpsideModTable[DUN_UPSIDE_MAX];
} dungeon_mod_info_T;

global dungeon_mod_info_T 19: DungeonModData;

void SetupDungeonModTable() {
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAHP].attrib_low = 10;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAHP].attrib_high = 21;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAHP].attrib_extra_low = 5;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAHP].attrib_extra_high = 10;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAHP].attrib_level_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAHP].attrib_level_extra_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAHP].tags = INV_ATTR_TAG_LIFE;

	DungeonModData.DungeonModTable[DUN_ATTR_FORTIFIED].attrib_low = 5;
	DungeonModData.DungeonModTable[DUN_ATTR_FORTIFIED].attrib_high = 11;
	DungeonModData.DungeonModTable[DUN_ATTR_FORTIFIED].attrib_extra_low = 8;
	DungeonModData.DungeonModTable[DUN_ATTR_FORTIFIED].attrib_extra_high = 12;
	DungeonModData.DungeonModTable[DUN_ATTR_FORTIFIED].attrib_level_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_FORTIFIED].attrib_level_extra_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_FORTIFIED].tags = INV_ATTR_TAG_DEFENSE;

	DungeonModData.DungeonModTable[DUN_ATTR_MORETOUGHENEMIES].attrib_low = 5;
	DungeonModData.DungeonModTable[DUN_ATTR_MORETOUGHENEMIES].attrib_high = 15;
	DungeonModData.DungeonModTable[DUN_ATTR_MORETOUGHENEMIES].attrib_extra_low = 8;
	DungeonModData.DungeonModTable[DUN_ATTR_MORETOUGHENEMIES].attrib_extra_high = 12;
	DungeonModData.DungeonModTable[DUN_ATTR_MORETOUGHENEMIES].attrib_level_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_MORETOUGHENEMIES].attrib_level_extra_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_MORETOUGHENEMIES].tags = INV_ATTR_TAG_NONE;

	DungeonModData.DungeonModTable[DUN_ATTR_MOREELITEENEMIES].attrib_low = 5;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREELITEENEMIES].attrib_high = 15;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREELITEENEMIES].attrib_extra_low = 10;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREELITEENEMIES].attrib_extra_high = 15;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREELITEENEMIES].attrib_level_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREELITEENEMIES].attrib_level_extra_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREELITEENEMIES].tags = INV_ATTR_TAG_NONE;

	DungeonModData.DungeonModTable[DUN_ATTR_CULLENEMIES].attrib_low = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_CULLENEMIES].attrib_high = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_CULLENEMIES].attrib_extra_low = 20;
	DungeonModData.DungeonModTable[DUN_ATTR_CULLENEMIES].attrib_extra_high = 40;
	DungeonModData.DungeonModTable[DUN_ATTR_CULLENEMIES].attrib_level_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_CULLENEMIES].attrib_level_extra_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_CULLENEMIES].tags = INV_ATTR_TAG_ATTACK;

	DungeonModData.DungeonModTable[DUN_ATTR_NOINFIGHT].attrib_low = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_NOINFIGHT].attrib_high = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_NOINFIGHT].attrib_extra_low = 15;
	DungeonModData.DungeonModTable[DUN_ATTR_NOINFIGHT].attrib_extra_high = 25;
	DungeonModData.DungeonModTable[DUN_ATTR_NOINFIGHT].attrib_level_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_NOINFIGHT].attrib_level_extra_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_NOINFIGHT].tags = INV_ATTR_TAG_UTILITY;

	DungeonModData.DungeonModTable[DUN_ATTR_NOPAIN].attrib_low = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_NOPAIN].attrib_high = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_NOPAIN].attrib_extra_low = 15;
	DungeonModData.DungeonModTable[DUN_ATTR_NOPAIN].attrib_extra_high = 30;
	DungeonModData.DungeonModTable[DUN_ATTR_NOPAIN].attrib_level_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_NOPAIN].attrib_level_extra_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_NOPAIN].tags = INV_ATTR_TAG_DEFENSE;

	DungeonModData.DungeonModTable[DUN_ATTR_NORIP].attrib_low = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_NORIP].attrib_high = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_NORIP].attrib_extra_low = 10;
	DungeonModData.DungeonModTable[DUN_ATTR_NORIP].attrib_extra_high = 20;
	DungeonModData.DungeonModTable[DUN_ATTR_NORIP].attrib_level_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_NORIP].attrib_level_extra_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_NORIP].tags = INV_ATTR_TAG_ATTACK;

	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAFAST].attrib_low = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAFAST].attrib_high = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAFAST].attrib_extra_low = 20;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAFAST].attrib_extra_high = 50;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAFAST].attrib_level_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAFAST].attrib_level_extra_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRAFAST].tags = INV_ATTR_TAG_UTILITY;

	DungeonModData.DungeonModTable[DUN_ATTR_MOREDMG].attrib_low = 11;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREDMG].attrib_high = 20;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREDMG].attrib_extra_low = 8;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREDMG].attrib_extra_high = 14;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREDMG].attrib_level_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREDMG].attrib_level_extra_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREDMG].tags = INV_ATTR_TAG_DAMAGE;

	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].attrib_low = 5;
	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].attrib_high = 10;
	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].attrib_extra_low = 10;
	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].attrib_extra_high = 16;
	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].attrib_level_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].attrib_level_extra_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].tags = INV_ATTR_TAG_ATTACK;

	DungeonModData.DungeonModTable[DUN_ATTR_GHOST].attrib_low = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_GHOST].attrib_high = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_GHOST].attrib_extra_low = 15;
	DungeonModData.DungeonModTable[DUN_ATTR_GHOST].attrib_extra_high = 35;
	DungeonModData.DungeonModTable[DUN_ATTR_GHOST].attrib_level_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_GHOST].attrib_level_extra_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_GHOST].tags = INV_ATTR_TAG_UTILITY;

	DungeonModData.DungeonModTable[DUN_ATTR_INCREASEDRESISTS].attrib_low = 5;
	DungeonModData.DungeonModTable[DUN_ATTR_INCREASEDRESISTS].attrib_high = 10;
	DungeonModData.DungeonModTable[DUN_ATTR_INCREASEDRESISTS].attrib_extra_low = 16;
	DungeonModData.DungeonModTable[DUN_ATTR_INCREASEDRESISTS].attrib_extra_high = 20;
	DungeonModData.DungeonModTable[DUN_ATTR_INCREASEDRESISTS].attrib_level_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_INCREASEDRESISTS].attrib_level_extra_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_INCREASEDRESISTS].tags = INV_ATTR_TAG_DEFENSE;
	
	/////////////
	// upsides //
	/////////////

	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_QUANT].attrib_low = 5;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_QUANT].attrib_high = 10;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_QUANT].attrib_level_modifier = 0;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_QUANT].tags = INV_ATTR_TAG_NONE;

	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARITY].attrib_low = 5;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARITY].attrib_high = 10;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARITY].attrib_level_modifier = 0;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARITY].tags = INV_ATTR_TAG_NONE;

	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_EXPANDCREDIT].attrib_low = 5;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_EXPANDCREDIT].attrib_high = 20;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_EXPANDCREDIT].attrib_level_modifier = 0;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_EXPANDCREDIT].tags = INV_ATTR_TAG_NONE;

	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_BUDGET].attrib_low = 5;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_BUDGET].attrib_high = 15;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_BUDGET].attrib_level_modifier = 0;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_BUDGET].tags = INV_ATTR_TAG_NONE;

	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERCHEST].attrib_low = 1;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERCHEST].attrib_high = 8;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERCHEST].attrib_level_modifier = 0;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERCHEST].tags = INV_ATTR_TAG_NONE;

	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERORBS].attrib_low = 1;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERORBS].attrib_high = 6;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERORBS].attrib_level_modifier = 0;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERORBS].tags = INV_ATTR_TAG_NONE;

	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_MERCHANTCHANCE].attrib_low = 1;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_MERCHANTCHANCE].attrib_high = 10;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_MERCHANTCHANCE].attrib_level_modifier = 0;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_MERCHANTCHANCE].tags = INV_ATTR_TAG_NONE;
}

bool IsDungeonAttributeQualityException(int attr) {
	// these don't have scaling values
	switch(attr) {
		case DUN_ATTR_CULLENEMIES:
		case DUN_ATTR_NOINFIGHT:
		case DUN_ATTR_NOPAIN:
		case DUN_ATTR_NORIP:
		case DUN_ATTR_EXTRAFAST:
		case DUN_ATTR_GHOST:
		return true;
	}
	return false;
}

bool IsDungeonAttributeExtraException(int attr) {
	/*switch(attr) {
		return true;
	}*/
	return false;
}

bool IsFixedPointDungeonMod(int mod) {
	/*switch(mod) {

	}*/
	return false;
}

int GetDungeonModTierRangeMapper(int attr, int lvl) {
	int val = 0;
	if(DungeonModData.DungeonModTable[attr].attrib_level_modifier == -1)
		return -1;

	if(!DungeonModData.DungeonModTable[attr].attrib_level_modifier)
		val = (DungeonModData.DungeonModTable[attr].attrib_high - DungeonModData.DungeonModTable[attr].attrib_low + 1) * lvl;
	else
		val = (DungeonModData.DungeonModTable[attr].attrib_level_modifier * lvl);
	return val;
}

int GetDungeonModTierRangeMapperExtra(int attr, int lvl) {
	int val = 0;
	if(DungeonModData.DungeonModTable[attr].attrib_level_extra_modifier == -1)
		return -1;

	if(!DungeonModData.DungeonModTable[attr].attrib_level_extra_modifier)
		val = (DungeonModData.DungeonModTable[attr].attrib_extra_high - DungeonModData.DungeonModTable[attr].attrib_extra_low + 1) * lvl;
	else
		val = (DungeonModData.DungeonModTable[attr].attrib_level_extra_modifier * lvl);
	return val;
}

// this uses a precalculated tier mapping to save time
int GetDungeonModRangeWithTier(int attr, int tier_mapping, bool which, int attr_factor) {
	int res = 0;

	if(tier_mapping != -1) {
		if(!which)
			res = (DungeonModData.DungeonModTable[attr].attrib_low + tier_mapping + (tier_mapping != 0)) * (100 + attr_factor) / 100;
		else
			res = (DungeonModData.DungeonModTable[attr].attrib_high + tier_mapping + (tier_mapping != 0)) * (100 + attr_factor) / 100;
	}
	else {
		if(!which)
			res = DungeonModData.DungeonModTable[attr].attrib_low * (100 + attr_factor) / 100;
		else
			res = DungeonModData.DungeonModTable[attr].attrib_high * (100 + attr_factor) / 100;
	}
	
	if(!res)
		res = 1;
	return res;
}

int GetDungeonModRangeWithTierExtra(int attr, int tier_mapping, bool which, int attr_factor) {
	int res = 0;

	if(tier_mapping != -1) {
		if(!which)
			res = (DungeonModData.DungeonModTable[attr].attrib_extra_low + tier_mapping + (tier_mapping != 0)) * (100 + attr_factor) / 100;
		else
			res = (DungeonModData.DungeonModTable[attr].attrib_extra_high + tier_mapping + (tier_mapping != 0)) * (100 + attr_factor) / 100;
	}
	else {
		if(!which)
			res = DungeonModData.DungeonModTable[attr].attrib_extra_low * (100 + attr_factor) / 100;
		else
			res = DungeonModData.DungeonModTable[attr].attrib_extra_high * (100 + attr_factor) / 100;
	}
	
	if(!res)
		res = 1;
	return res;
}

// this calculates the tier mapping for itself based on supplied level
int GetDungeonModRange(int attr, int tier, bool which, int attr_factor) {
	return GetDungeonModRangeWithTier(attr, GetDungeonModTierRangeMapper(attr, tier), which, attr_factor);
}

int GetDungeonModRangeExtra(int attr, int tier, bool which, int attr_factor) {
	return GetDungeonModRangeWithTierExtra(attr, GetDungeonModTierRangeMapperExtra(attr, tier), which, attr_factor);
}

str GetDetailedDungeonModRange(int attr, int tier, int trunc_factor = 0, int extra = -1, bool isPercentage = false) {
	if(extra != -1)
		return GetDetailedModRange_Unique(tier, trunc_factor, extra, isPercentage);
	
	// limit this to here at t10...
	str col_tag = GetCharmString(Clamp_Between(tier, 0, 9), CHARMSTR_COLORCODE);
	int tier_mapping = GetModTierRangeMapper(attr, tier);
	
	if(!trunc_factor) {
		return StrParam(
			s:"\c-(",
			s:col_tag, d:GetDungeonModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW, 0),
			s:"\c--",
			s:col_tag, d:GetDungeonModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH, 0), s:"\c-)"
		);
	}
	return StrParam(
		s:"\c-(",
		s:col_tag, s:GetFixedRepresentation(GetDungeonModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW, 0), isPercentage),
		s:"\c--",
		s:col_tag, s:GetFixedRepresentation(GetDungeonModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH, 0), isPercentage), s:"\c-)"
	);
}

int RollDungeonAttributeValue(int attr, int tier, bool isWellRolled) {
	int tier_mapping = GetDungeonModTierRangeMapper(attr, tier);
	int temp;

	bool revered = CheckInventory("ReveranceUsed");
	
	// the + 0.0005 is so the edge rolls can be achieved
	if(!isWellRolled && !revered) {
		temp = random(GetDungeonModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW, 0), GetDungeonModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH, 0));
		if(IsFixedPointDungeonMod(attr) && temp > 0.0005)
			temp += 0.0005;
		return temp;
	}
	
	// well rolled case
	temp = GetDungeonModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH, 0);

	if(!revered)
		temp = random((GetDungeonModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW, 0) + temp) / 2, temp);
	else
		temp = random(GetDungeonModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW, 0) / 4 + (temp * 3) / 4, temp);
	
	if(IsFixedPointDungeonMod(attr) && temp > 0.0005)
		temp += 0.0005;
	
	return temp;
}

int RollDungeonAttributeExtra(int attr, int tier, bool isWellRolled) {
	int tier_mapping = GetDungeonModTierRangeMapperExtra(attr, tier);
	int temp;

	bool revered = CheckInventory("ReveranceUsed");
	
	// the + 0.0005 is so the edge rolls can be achieved
	if(!isWellRolled && !revered) {
		temp = random(GetDungeonModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_LOW, 0), GetDungeonModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_HIGH, 0));
		if(IsFixedPointDungeonMod(attr) && temp > 0.0005)
			temp += 0.0005;
		return temp;
	}
	
	// well rolled case
	temp = GetDungeonModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_HIGH, 0);

	if(!revered)
		temp = random((GetDungeonModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_LOW, 0) + temp) / 2, temp);
	else
		temp = random(GetDungeonModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_LOW, 0) / 4 + (temp * 3) / 4, temp);
	
	if(IsFixedPointDungeonMod(attr) && temp > 0.0005)
		temp += 0.0005;
	
	return temp;
}

str DungeonAttributeString(
	int attr, int val, int tier = 0, bool showDetailedMods = false, 
	int extra = -1, bool isFractured = false, int qual = 0, 
	int attr_extra = 0, int craftAffected = 0
)
{
	str text = StrParam(s:"DUNATTR_", d:attr);
	str col_tag = "\c[Q9]";
	str no_tag = "\c-";

	if(!(craftAffected & 0xFF)) {
		if(isFractured) {
			col_tag = "\c[E2]";
			no_tag = "\c[E2]";
		}
	}
	else {
		if((craftAffected >> 8) != DND_ORBEFFECT_NUMBER)
			no_tag = "\ck";
		col_tag = "\ck";
	}

	// extract id and value separately (we encode id in the value as part of limitation)
	int attr_extra_id = attr_extra & 0xFFFF;
	attr_extra >>= 16;

	if(qual) {
		if(!IsDungeonAttributeQualityException(attr)) {
			if(val < 100000) {
				val *= qual + 100;
				val /= 100;
			}
			else {
				val /= 100;
				val *= qual + 100;
			}
		}

		if(attr_extra && !IsDungeonAttributeExtraException(attr_extra_id)) {
			if(attr_extra > 100000) {
				attr_extra /= 100;
				attr_extra *= qual + 100;
			}
			else {
				attr_extra *= qual + 100;
				attr_extra /= 100;
			}
		}
	}

	switch(attr) {
		case DUN_ATTR_EXTRAHP:
		case DUN_ATTR_FORTIFIED:
		case DUN_ATTR_MORETOUGHENEMIES:
		case DUN_ATTR_MOREELITEENEMIES:
			if(showDetailedMods) {
				text = StrParam(s:"+", s:col_tag, d:val, s:GetDetailedDungeonModRange(attr, tier, 0, extra), s:"% ", s:no_tag, l:text, s:"\n");
			}
			else
				text =  StrParam(s:"+", s:col_tag, d:val, s:"% ", s:no_tag, l:text, s:"\n");
		break;

		// single line text, they dont have any change on text
		case DUN_ATTR_CULLENEMIES:
		case DUN_ATTR_NOINFIGHT:
		case DUN_ATTR_NOPAIN:
		case DUN_ATTR_NORIP:
		case DUN_ATTR_EXTRAFAST:
		case DUN_ATTR_GHOST:
			text = StrParam(s:no_tag, l:text, s:"\n");
		break;

		case DUN_ATTR_MOREDMG:
			if(showDetailedMods) {
				text = StrParam(s:no_tag, l:text, s:col_tag, d:val, s:GetDetailedDungeonModRange(attr, tier, 0, extra), s:"% ", s:no_tag, l:"DUNATTR_9X", s:"\n");
			}
			else
				text =  StrParam(s:no_tag, l:text, s:col_tag, d:val, s:"% ", s:no_tag, l:"DUNATTR_9X", s:"\n");
		break;

		case DUN_ATTR_FASTPROJ:
			if(showDetailedMods) {
				text = StrParam(s:no_tag, l:text, s:col_tag, d:val, s:GetDetailedDungeonModRange(attr, tier, 0, extra), s:"% ", s:no_tag, l:"DUNATTR_10X", s:"\n");
			}
			else
				text =  StrParam(s:no_tag, l:text, s:col_tag, d:val, s:"% ", s:no_tag, l:"DUNATTR_10X", s:"\n");
		break;

		case DUN_ATTR_INCREASEDRESISTS:
			if(showDetailedMods) {
				text = StrParam(s:no_tag, l:text, s:col_tag, d:val, s:GetDetailedDungeonModRange(attr, tier, 0, extra), s:"% ", s:no_tag, l:"DUNATTR_12X", s:"\n");
			}
			else
				text =  StrParam(s:no_tag, l:text, s:col_tag, d:val, s:"% ", s:no_tag, l:"DUNATTR_12X", s:"\n");
		break;
	}

	// use the assigned extra id as a separate thing to draw here, appended to text with a newline
	// for now we got no exceptions in the style of the upside text, so...
	if(attr_extra != -1) {
		if(showDetailedMods) {
			// append this at the end if mod tiers is requested, after the upside
			text = StrParam(
				s:text, s:"+", s:col_tag, d:attr_extra, s:"% ", s:no_tag, l:StrParam(s:"DUNATTR_UPSIDE_", d:attr_extra_id),
				s:" - ", s:GetModTierText(tier, extra)
			);
		}
		else {
			text = StrParam(s:text, s:"+", s:col_tag, d:attr_extra, s:"% ", s:no_tag, l:StrParam(s:"DUNATTR_UPSIDE_", d:attr_extra_id));
		}
	}

	return text;
}

// this is used for menu only really
str DungeonUpsideString(int id, int val) {
	str col_tag = "\c[Q9]";
	str no_tag = "\c-";

	return StrParam(s:"+", s:col_tag, d:val, s:"% ", s:no_tag, l:StrParam(s:"DUNATTR_UPSIDE_", d:id));
}

enum {
	DUNGEON_MONTYPE_DEMON = 1,
	DUNGEON_MONTYPE_ELDRITCH = 2,
	DUNGEON_MONTYPE_ROBOT = 4,
	DUNGEON_MONTYPE_UNDEAD = 8,
	DUNGEON_MONTYPE_MAGICAL = 16
};

int GetDungeonMonsterTypes(int id) {
	switch(id) {
		case DND_DUNGEON_VOIDKEEP:
		return DUNGEON_MONTYPE_DEMON | DUNGEON_MONTYPE_ELDRITCH;
	}
	return 0;
}

str GetDungeonMonsterTypeString(int id) {
	int curr = 0;
	int types = GetDungeonMonsterTypes(id);
	str res = StrParam(s:"\c[Y5]", l:"DND_OPPOSITION", s:": \cj");
	while(types) {
		if(types & 1) {
			res = StrParam(s:res, l:StrParam(s:"DUNGEON_MTYPE_", d:curr + 1));

			if(types >> 1)
				res = StrParam(s:res, s:", ");
		}
		++curr;
		types >>= 1;
	}

	return res;
}

// return value if it exists, -1 if not
int HasDungeonAttributeVal(int attr) {
	for(int i = 0; i < DungeonInformation.attrib_count; ++i)
		if(DungeonInformation.attributes[i].attrib_id == attr)
			return DungeonInformation.attributes[i].attrib_val;
	return -1;
}

// returns the value of extra when given the id that would be encoded here
int HasDungeonUpside(int id) {
	return DungeonInformation.upside_vals[id];
}

#endif