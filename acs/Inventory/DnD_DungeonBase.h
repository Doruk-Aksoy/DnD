#ifndef DND_DUNGEONBASE_IN
#define DND_DUNGEONBASE_IN

enum {
	DND_DUNGEON_VOIDKEEP,
};
#define DND_FIRST_DUNGEONID DND_DUNGEON_VOIDKEEP

#define MAX_DUNGEONKEY_ATTRIB_DEFAULT 3

#define DND_DUNGEON_QUANTPERLVL 0.0075
#define DND_DUNGEON_INCURSION_ODDS 10

enum {
	DUN_UPSIDE_QUANT,
	DUN_UPSIDE_RARITY,
	DUN_UPSIDE_EXPANDCREDIT,
	DUN_UPSIDE_BUDGET,
	DUN_UPSIDE_RARERCHEST,
	DUN_UPSIDE_RARERORBS,
	DUN_UPSIDE_MERCHANTCHANCE,
	DUN_UPSIDE_ITEMLEVELBONUS,

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
	DUN_ATTR_EXTRASPEED,
	DUN_ATTR_AILMENTAVOID,
	DUN_ATTR_INFLICTAILMENT,

	DUN_ATTR_MAX
};
#define FIRST_DUNGEON_ATTRIBUTE DUN_ATTR_EXTRAHP

// Which ailment DUN_ATTR_INFLICTAILMENT names. This is rolled into attrib_val -- the only field a
// dungeon downside has, since attrib_extra is always the paired upside -- so the value IS the
// selector and carries no magnitude. That is also why the mod is a quality exception below: scaling
// an index by dungeon quality would just point it somewhere else.
//
// Starts at 1, not 0. GetDungeonModRangeWithTier turns a computed 0 into 1 on the way out, so a
// zero based list would lose its first entry to the entry above it.
//
// Ignite is deliberately absent: the design names these five.
enum {
	DUN_INFLICT_BLEED = 1,
	DUN_INFLICT_POISON,
	DUN_INFLICT_CHILL,
	DUN_INFLICT_FREEZE,
	DUN_INFLICT_OVERLOAD,

	DUN_INFLICT_MAX
};

// The chance is flat rather than rolled, because attrib_val is spent naming the ailment.
#define DND_DUNGEON_INFLICTAILMENT_CHANCE 20

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
	DungeonModData.DungeonModTable[DUN_ATTR_CULLENEMIES].attrib_extra_high = 50;
	DungeonModData.DungeonModTable[DUN_ATTR_CULLENEMIES].attrib_level_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_CULLENEMIES].attrib_level_extra_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_CULLENEMIES].tags = INV_ATTR_TAG_ATTACK;

	DungeonModData.DungeonModTable[DUN_ATTR_NOINFIGHT].attrib_low = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_NOINFIGHT].attrib_high = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_NOINFIGHT].attrib_extra_low = 15;
	DungeonModData.DungeonModTable[DUN_ATTR_NOINFIGHT].attrib_extra_high = 30;
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
	DungeonModData.DungeonModTable[DUN_ATTR_NORIP].attrib_extra_high = 30;
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
	DungeonModData.DungeonModTable[DUN_ATTR_MOREDMG].attrib_extra_high = 20;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREDMG].attrib_level_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREDMG].attrib_level_extra_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_MOREDMG].tags = INV_ATTR_TAG_DAMAGE;

	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].attrib_low = 5;
	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].attrib_high = 10;
	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].attrib_extra_low = 10;
	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].attrib_extra_high = 15;
	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].attrib_level_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].attrib_level_extra_modifier = 0;
	DungeonModData.DungeonModTable[DUN_ATTR_FASTPROJ].tags = INV_ATTR_TAG_ATTACK;

	DungeonModData.DungeonModTable[DUN_ATTR_GHOST].attrib_low = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_GHOST].attrib_high = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_GHOST].attrib_extra_low = 15;
	DungeonModData.DungeonModTable[DUN_ATTR_GHOST].attrib_extra_high = 40;
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

	DungeonModData.DungeonModTable[DUN_ATTR_EXTRASPEED].attrib_low = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRASPEED].attrib_high = 1;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRASPEED].attrib_extra_low = 10;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRASPEED].attrib_extra_high = 25;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRASPEED].attrib_level_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRASPEED].attrib_level_extra_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_EXTRASPEED].tags = INV_ATTR_TAG_UTILITY;
	
	DungeonModData.DungeonModTable[DUN_ATTR_AILMENTAVOID].attrib_low = 40;
	DungeonModData.DungeonModTable[DUN_ATTR_AILMENTAVOID].attrib_high = 70;
	DungeonModData.DungeonModTable[DUN_ATTR_AILMENTAVOID].attrib_extra_low = 15;
	DungeonModData.DungeonModTable[DUN_ATTR_AILMENTAVOID].attrib_extra_high = 33;
	DungeonModData.DungeonModTable[DUN_ATTR_AILMENTAVOID].attrib_level_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_AILMENTAVOID].attrib_level_extra_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_AILMENTAVOID].tags = INV_ATTR_TAG_DEFENSE;

	// low..high spans the ailment list rather than a magnitude. Both level modifiers are -1 so the
	// dungeon's level cannot shift the selector either.
	DungeonModData.DungeonModTable[DUN_ATTR_INFLICTAILMENT].attrib_low = DUN_INFLICT_BLEED;
	DungeonModData.DungeonModTable[DUN_ATTR_INFLICTAILMENT].attrib_high = DUN_INFLICT_MAX - 1;
	DungeonModData.DungeonModTable[DUN_ATTR_INFLICTAILMENT].attrib_extra_low = 20;
	DungeonModData.DungeonModTable[DUN_ATTR_INFLICTAILMENT].attrib_extra_high = 50;
	DungeonModData.DungeonModTable[DUN_ATTR_INFLICTAILMENT].attrib_level_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_INFLICTAILMENT].attrib_level_extra_modifier = -1;
	DungeonModData.DungeonModTable[DUN_ATTR_INFLICTAILMENT].tags = INV_ATTR_TAG_ATTACK;

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

	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERCHEST].attrib_low = 3;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERCHEST].attrib_high = 12;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERCHEST].attrib_level_modifier = 0;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERCHEST].tags = INV_ATTR_TAG_NONE;

	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERORBS].attrib_low = 2;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERORBS].attrib_high = 10;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERORBS].attrib_level_modifier = 0;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_RARERORBS].tags = INV_ATTR_TAG_NONE;

	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_MERCHANTCHANCE].attrib_low = 1;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_MERCHANTCHANCE].attrib_high = 10;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_MERCHANTCHANCE].attrib_level_modifier = 0;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_MERCHANTCHANCE].tags = INV_ATTR_TAG_NONE;

	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_ITEMLEVELBONUS].attrib_low = 1;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_ITEMLEVELBONUS].attrib_high = 2;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_ITEMLEVELBONUS].attrib_level_modifier = 0;
	DungeonModData.DungeonUpsideModTable[DUN_UPSIDE_ITEMLEVELBONUS].tags = INV_ATTR_TAG_NONE;
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
		case DUN_ATTR_EXTRASPEED:
		// not a magnitude at all -- scaling it would name a different ailment
		case DUN_ATTR_INFLICTAILMENT:
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
			res = (DungeonModData.DungeonUpsideModTable[attr].attrib_low + tier_mapping + (tier_mapping != 0)) * (100 + attr_factor) / 100;
		else
			res = (DungeonModData.DungeonUpsideModTable[attr].attrib_high + tier_mapping + (tier_mapping != 0)) * (100 + attr_factor) / 100;
	}
	else {
		if(!which)
			res = DungeonModData.DungeonUpsideModTable[attr].attrib_low * (100 + attr_factor) / 100;
		else
			res = DungeonModData.DungeonUpsideModTable[attr].attrib_high * (100 + attr_factor) / 100;
	}
	
	if(!res)
		res = 1;
	return res;
}

// this calculates the tier mapping for itself based on supplied level
int GetDungeonModRange(int attr, int tier, bool which, int attr_factor) {
	return GetDungeonModRangeWithTier(attr, GetDungeonModTierRangeMapper(attr, tier), which, attr_factor);
}

str GetDetailedDungeonModRange(int attr, int tier, int trunc_factor = 0, int extra = -1, bool isPercentage = false) {
	if(extra != -1)
		return GetDetailedModRange_Unique(tier, trunc_factor, extra, isPercentage);
	
	str col_tag = GetCharmString(GetModTierColourIndex(attr, DND_ITEM_DUNGEONKEY, tier), CHARMSTR_COLORCODE);
	int tier_mapping = GetDungeonModTierRangeMapper(attr, tier);
	
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

str GetDetailedDungeonModRangeExtra(int attr, int base_mod, int tier, int trunc_factor = 0, int extra = -1, bool isPercentage = false) {
	if(extra != -1)
		return GetDetailedModRange_Unique(tier, trunc_factor, extra, isPercentage);
	
	str col_tag = GetCharmString(GetModTierColourIndex(attr, DND_ITEM_DUNGEONKEY, tier), CHARMSTR_COLORCODE);
	int tier_mapping = GetDungeonModTierRangeMapperExtra(base_mod, tier);
	int scale_factor_low = 0, scale_factor_high = 0;
	if(tier_mapping != -1) {
		scale_factor_low = DungeonModData.DungeonModTable[base_mod].attrib_extra_low + tier_mapping + (tier_mapping != 0);
		scale_factor_high = DungeonModData.DungeonModTable[base_mod].attrib_extra_high + tier_mapping + (tier_mapping != 0);
	}
	else {
		scale_factor_low = DungeonModData.DungeonModTable[base_mod].attrib_extra_low;
		scale_factor_high = DungeonModData.DungeonModTable[base_mod].attrib_extra_high;
	}
	
	if(!trunc_factor) {
		return StrParam(
			s:"\c-(",
			s:col_tag, d:GetDungeonModRangeWithTierExtra(attr, 0, ITEM_MODRANGE_LOW, scale_factor_low),
			s:"\c--",
			s:col_tag, d:GetDungeonModRangeWithTierExtra(attr, 0, ITEM_MODRANGE_HIGH, scale_factor_high), s:"\c-)"
		);
	}
	return StrParam(
		s:"\c-(",
		s:col_tag, s:GetFixedRepresentation(GetDungeonModRangeWithTierExtra(attr, 0, ITEM_MODRANGE_LOW, scale_factor_low), isPercentage),
		s:"\c--",
		s:col_tag, s:GetFixedRepresentation(GetDungeonModRangeWithTierExtra(attr, 0, ITEM_MODRANGE_HIGH, scale_factor_high), isPercentage), s:"\c-)"
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

// base_mod is the dungeon mod whose multiplier we will use on the attr's (the upside's) value
int RollDungeonAttributeExtra(int attr, int base_mod, int tier, bool isWellRolled) {
	int tier_mapping = GetDungeonModTierRangeMapperExtra(base_mod, tier);
	int temp;

	bool revered = CheckInventory("ReveranceUsed");

	int scale_factor = 0;
	if(tier_mapping != -1)
		scale_factor = random(DungeonModData.DungeonModTable[base_mod].attrib_extra_low, DungeonModData.DungeonModTable[base_mod].attrib_extra_high) + tier_mapping + (tier_mapping != 0);
	else
		scale_factor = random(DungeonModData.DungeonModTable[base_mod].attrib_extra_low, DungeonModData.DungeonModTable[base_mod].attrib_extra_high);

	// the + 0.0005 is so the edge rolls can be achieved
	if(!isWellRolled && !revered) {
		temp = random(GetDungeonModRangeWithTierExtra(attr, 0, ITEM_MODRANGE_LOW, scale_factor), GetDungeonModRangeWithTierExtra(attr, 0, ITEM_MODRANGE_HIGH, scale_factor));
		if(IsFixedPointDungeonMod(attr) && temp > 0.0005)
			temp += 0.0005;
		return temp;
	}
	
	// well rolled case
	temp = GetDungeonModRangeWithTierExtra(attr, 0, ITEM_MODRANGE_HIGH, scale_factor);

	if(!revered)
		temp = random((GetDungeonModRangeWithTierExtra(attr, 0, ITEM_MODRANGE_LOW, scale_factor) + temp) / 2, temp);
	else
		temp = random(GetDungeonModRangeWithTierExtra(attr, 0, ITEM_MODRANGE_LOW, scale_factor) / 4 + (temp * 3) / 4, temp);
	
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
		case DUN_ATTR_EXTRASPEED:
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

		case DUN_ATTR_AILMENTAVOID:
			if(showDetailedMods) {
				text = StrParam(s:no_tag, l:text, s:col_tag, d:val, s:GetDetailedDungeonModRange(attr, tier, 0, extra), s:"% ", s:no_tag, l:"DUNATTR_14X", s:"\n");
			}
			else
				text =  StrParam(s:no_tag, l:text, s:col_tag, d:val, s:"% ", s:no_tag, l:"DUNATTR_14X", s:"\n");
		break;

		// val names the ailment here, so there is no range to detail and no value to print -- the
		// chance is the same on every roll of this mod.
		case DUN_ATTR_INFLICTAILMENT:
			text = StrParam(s:no_tag, l:text, s:col_tag, d:DND_DUNGEON_INFLICTAILMENT_CHANCE, s:"% ", s:no_tag, l:"DUNATTR_15X", s:col_tag, l:GetDungeonInflictAilmentLabel(val), s:no_tag, s:"\n");
	}

	// use the assigned extra id as a separate thing to draw here, appended to text with a newline
	// for now we got no exceptions in the style of the upside text, so...
	if(attr_extra != -1) {
		if(showDetailedMods) {
			// append this at the end if mod tiers is requested, after the upside
			text = StrParam(
				s:text, s:DungeonUpsideString(attr_extra_id, attr_extra, true, attr, tier, extra)
			);
		}
		else {
			text = StrParam(s:text, s:DungeonUpsideString(attr_extra_id, attr_extra));
		}
	}

	return text;
}

// this is used for menu only really -- anything after showDetailedMods is only used if showDetailedMods is true!
str DungeonUpsideString(int id, int val, bool showDetailedMods = false, int attr = -1, int tier = -1, int extra = -1) {
	str col_tag = "\c[Q9]";
	str no_tag = "\c-";

	switch(id) {
		case DUN_UPSIDE_ITEMLEVELBONUS:
			if(!showDetailedMods)
				return StrParam(l:"DUNATTR_UPSIDE_7", s:" ", s:col_tag, s:"+", d:val, s:no_tag, s:" ", l:"DUNATTR_UPSIDE_7_2");
			
			return StrParam(
				l:"DUNATTR_UPSIDE_7", s:" ", s:col_tag, s:"+", d:val, s:GetDetailedDungeonModRangeExtra(id, attr, tier),
				s:no_tag, s:" ", l:"DUNATTR_UPSIDE_7_2", s:" - ", s:GetModTierText(attr, DND_ITEM_DUNGEONKEY, tier, extra)
			);
	}

	if(!showDetailedMods)
		return StrParam(s:"+", s:col_tag, d:val, s:"% ", s:no_tag, l:StrParam(s:"DUNATTR_UPSIDE_", d:id));
	
	return StrParam(
		s:"+", s:col_tag, d:val, s:GetDetailedDungeonModRangeExtra(id, attr, tier), s:"% ", s:no_tag, 
		l:StrParam(s:"DUNATTR_UPSIDE_", d:id), 
		s:" - ", s:GetModTierText(attr, DND_ITEM_DUNGEONKEY, tier, extra)
	);
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
		return DUNGEON_MONTYPE_DEMON | DUNGEON_MONTYPE_ELDRITCH | DUNGEON_MONTYPE_UNDEAD;
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

// Quality multiplies a rolled percentage, so a high AILMENTAVOID roll on a good key can pass 100 --
// and a chance that reaches 100 stops being a chance. It would switch every ailment build off for the
// map instead of taxing it, which no other dungeon downside does. Capped so something always lands.
#define DND_DUNGEON_MAXAILMENTAVOID 90

// The dungeon's own shrug, rolled per application. Deliberately separate from a monster's ailment
// immunity trait: the player's ailment ignore chance answers that trait, not the map, so stacking
// one stat cannot switch this modifier off.
bool DungeonAvoidsAilment() {
	int avoid = HasDungeonAttributeVal(DUN_ATTR_AILMENTAVOID);
	return avoid != -1 && random(1, 100) <= avoid;
}

// True when the dungeon names this ailment and its roll lands. This is an additional route to the
// ailment rather than a bonus on the existing one: it does not need the hit to carry the matching
// damage type or the monster to carry the matching trait, which is the point of the modifier.
// The player's own avoidance still answers it at every site.
bool DungeonInflictsAilment(int ailment) {
	return HasDungeonAttributeVal(DUN_ATTR_INFLICTAILMENT) == ailment &&
		random(1, 100) <= DND_DUNGEON_INFLICTAILMENT_CHANCE;
}

// DND_MENU_AILMENT<n> numbering, which is not contiguous with DUN_INFLICT_* because it carries
// ignite at 3 and this modifier does not roll it.
str GetDungeonInflictAilmentLabel(int id) {
	switch(id) {
		case DUN_INFLICT_CHILL:		return "DND_MENU_AILMENT1";
		case DUN_INFLICT_FREEZE:	return "DND_MENU_AILMENT2";
		case DUN_INFLICT_OVERLOAD:	return "DND_MENU_AILMENT4";
		case DUN_INFLICT_POISON:	return "DND_MENU_AILMENT5";
	}
	return "DND_MENU_AILMENT6";
}

// returns the value of extra when given the id that would be encoded here
int HasDungeonUpside(int id) {
	return DungeonInformation.upside_vals[id];
}

#endif