#ifndef DND_INVINFO_IN
#define DND_INVINFO_IN

// put common inventory information here
#define MAX_ITEM_ATTRIBUTES 9

// only orbs and elixirs
#define MAX_CRAFTITEMTYPES 2

#define DND_CYBERNETIC_FACTOR_MUL 13 // +30% = x1.3
#define DND_CYBERNETIC_FACTOR_DIV 10

enum {
	DND_INVCATEGORY_WORN,
	DND_INVCATEGORY_CHARM
};
#define MAX_INVENTORY_CATEGORIES DND_INVCATEGORY_CHARM + 1

enum {
	DND_ITEM_NULL,
	DND_ITEM_TEMPORARY,
	DND_ITEM_CHARM,
	DND_ITEM_HELM,
	DND_ITEM_BOOT,
	DND_ITEM_GLOVE,
	DND_ITEM_NECKLACE,
	DND_ITEM_RING,
	DND_ITEM_ORB,
	DND_ITEM_CHESTKEY,
	DND_ITEM_ELIXIR,
	DND_ITEM_WEAPON,
	DND_ITEM_TOKEN
};

int CraftItemTypes[MAX_CRAFTITEMTYPES] = {
	DND_ITEM_ORB,
	DND_ITEM_ELIXIR
};

#define MAX_CRAFTABLEITEMTYPES 4
int CraftableItemTypes[MAX_CRAFTABLEITEMTYPES] = {
	DND_ITEM_CHARM,
	DND_ITEM_BOOT,
	DND_ITEM_HELM,
	DND_ITEM_NECKLACE
};

typedef struct {
	int attrib_val;
	int attrib_id;
	int attrib_tier;
} attr_inf_T;

typedef struct it {
	int width;										// width in inventory space
	int height;										// height in inventory space
	int item_image;									// image of item from image list
	int item_type;									// what type of item it is (>65535 implies this item is a unique, >> 16 - 1 gives unique id)
	int item_subtype;								// subtype for items that have it (charms etc)
	int item_level;									// what level this item is
	int item_stack;									// the stack of the item (if applicable)
	int topleftboxid;								// used to determine the owning pointer (-1 of this is the pointer)
	int attrib_count;								// count of attributes
	attr_inf_T attributes[MAX_ITEM_ATTRIBUTES];		// attribute list
} inventory_T;

// The following are seperated to fitting categories, they are used this way in many places
enum {
	DND_CHESTTYPE_BRONZE,
	DND_CHESTTYPE_SILVER,
	DND_CHESTTYPE_GOLD
};

enum {
	DND_ELIXIR_HEALTH,
	DND_ELIXIR_ARMOR,
	DND_ELIXIR_HPARMOR,
	DND_ELIXIR_HPPERCENT,
	DND_ELIXIR_ARMORPERCENT,
	DND_ELIXIR_HPARMORPERCENT,
	DND_ELIXIR_SPEED,
	DND_ELIXIR_DAMAGE,
	DND_ELIXIR_LUCK
};

enum {
	DND_ORB_ENHANCE,
	DND_ORB_CORRUPT,
	DND_ORB_SPIRIT,
	DND_ORB_REPENT,
	DND_ORB_AFFLUENCE,
	DND_ORB_CALAMITY,
	DND_ORB_PROSPERITY,
	DND_ORB_WISDOM,
	DND_ORB_GREED,
	DND_ORB_VIOLENCE,
	DND_ORB_FORTITUDE,
	DND_ORB_SIN,
	DND_ORB_RICHES,
	DND_ORB_HOLDING,
	DND_ORB_REFINEMENT,
	DND_ORB_SCULPTING,
	DND_ORB_ELEVATION,
	DND_ORB_HOLLOW,
	DND_ORB_PHANTASMAL,
	DND_ORB_ASSIMILATION
};

enum {
	DND_TOKEN_REPAIR,
	DND_TOKEN_SCOUR
};

// these are used for accessing common info table below
enum {
	DND_ISUBT_CHESTTYPE_BRONZE,
	DND_ISUBT_CHESTTYPE_SILVER,
	DND_ISUBT_CHESTTYPE_GOLD,
	
	DND_ISUBT_ELIXIR_HEALTH,
	DND_ISUBT_ELIXIR_ARMOR,
	DND_ISUBT_ELIXIR_HPARMOR,
	DND_ISUBT_ELIXIR_HPPERCENT,
	DND_ISUBT_ELIXIR_ARMORPERCENT,
	DND_ISUBT_ELIXIR_HPARMORPERCENT,
	DND_ISUBT_ELIXIR_SPEED,
	DND_ISUBT_ELIXIR_DAMAGE,
	DND_ISUBT_ELIXIR_LUCK,
	
	DND_ISUBT_ORB_ENHANCE,
	DND_ISUBT_ORB_CORRUPT,
	DND_ISUBT_ORB_SPIRIT,
	DND_ISUBT_ORB_REPENT,
	DND_ISUBT_ORB_AFFLUENCE,
	DND_ISUBT_ORB_CALAMITY,
	DND_ISUBT_ORB_PROSPERITY,
	DND_ISUBT_ORB_WISDOM,
	DND_ISUBT_ORB_GREED,
	DND_ISUBT_ORB_VIOLENCE,
	DND_ISUBT_ORB_FORTITUDE,
	DND_ISUBT_ORB_SIN,
	DND_ISUBT_ORB_RICHES,
	DND_ISUBT_ORB_HOLDING,
	DND_ISUBT_ORB_REFINEMENT,
	DND_ISUBT_ORB_SCULPTING,
	DND_ISUBT_ORB_ELEVATION,
	DND_ISUBT_ORB_HOLLOW,
	DND_ISUBT_ORB_PHANTASMAL,
	DND_ISUBT_ORB_ASSIMILATION,
	
	DND_ISUBT_TOKEN_REPAIR,
	DND_ISUBT_TOKEN_SCOUR
};

#define CHESTKEY_BEGIN DND_ISUBT_CHESTTYPE_BRONZE
#define ELIXIR_BEGIN DND_ISUBT_ELIXIR_HEALTH
#define ORBS_BEGIN DND_ISUBT_ORB_ENHANCE
#define TOKEN_BEGIN DND_ISUBT_TOKEN_REPAIR

#define CHESTKEY_END DND_ISUBT_CHESTTYPE_GOLD
#define ELIXIR_END DND_ISUBT_ELIXIR_LUCK
#define ORBS_END DND_ISUBT_ORB_ASSIMILATION
#define TOKEN_END DND_ISUBT_TOKEN_SCOUR

#define MAX_CHESTKEYS (CHESTKEY_END - CHESTKEY_BEGIN + 1)
#define MAX_ELIXIRS (ELIXIR_END - ELIXIR_BEGIN + 1)
#define MAX_ORBS (ORBS_END - ORBS_BEGIN + 1)
#define MAX_TOKENS (TOKEN_END - TOKEN_BEGIN + 1)

int TokenWeights[MAX_TOKENS] = {
	8,
	10
};
#define MAX_TOKEN_WEIGHT 10

#define MAX_COMMON_INVENTORY (TOKEN_END + 1)

str InventoryInfo[MAX_COMMON_INVENTORY] = {
	"BronzeChestKey",
	"SilverChestKey",
	"GoldChestKey",
	
	"ElixirOfHealth",
	"ElixirOfArmor",
	"ElixirOfProsperity",
	"ElixirOfLife",
	"ElixirOfStrongness",
	"ElixirOfFortitude",
	"ElixirOfSpeed",
	"ElixirOfDamage",
	"ElixirOfLuck",
	
	"OrbOfEnhancement",
	"OrbOfCorruption",
	"OrbOfSpirit",
	"OrbofRepentance",
	"OrbofAffluence",
	"OrbofCalamity",
	"OrbofProsperity",
	"OrbofWisdom",
	"OrbofGreed",
	"OrbofViolence",
	"OrbofFortitude",
	"OrbofSin",
	"OrbofRiches",
	"OrbofHolding",
	"OrbofRefinement",
	"OrbofSculpting",
	"OrbofElevation",
	"OrbofHollow",
	"OrbofPhantasmal",
	"OrbofAssimilation",

	"RepairToken",
	"ScouringToken"
};

// the below two are for when only text or tag are needed, 3rd is for the entire thing being needed (a tiny optimization)
// draw tag with \c[Y5] color code, then \n then text
str GetInventoryTag(int id) {
	if(id <= CHESTKEY_END)
		return StrParam(s:"DND_CHESTKEY", d:id - CHESTKEY_BEGIN + 1);
	if(id <= ELIXIR_END)
		return StrParam(s:"DND_ELIXIR", d:id - ELIXIR_BEGIN + 1);
	if(id <= ORBS_END)
		return StrParam(s:"DND_ORB", d:id - ORBS_BEGIN + 1);
	return StrParam(s:"DND_TOKEN", d:id - TOKEN_BEGIN + 1);
}

str GetInventoryText(int id) {
	if(id <= CHESTKEY_END)
		return StrParam(s:"DND_CHESTKEYTEXT", d:id - CHESTKEY_BEGIN + 1);
	if(id <= ELIXIR_END)
		return StrParam(s:"DND_ELIXIRTEXT", d:id - ELIXIR_BEGIN + 1);
	if(id <= ORBS_END)
		return StrParam(s:"DND_ORBTEXT", d:id - ORBS_BEGIN + 1);
	return StrParam(s:"DND_TOKENTEXT", d:id - TOKEN_BEGIN + 1);
}

// Putting the uniques enum here due to dependency issues
// s, m and g indicate charm type
enum {
	// M - 50% increased armor cap, 10-25% reduced elemental damage taken, 2-5% chance of casting a random elemental spell while firing
	UITEM_ELEMENTALBULWARK,
	// G - 5-30 flat physical damage, 10-25 bulkiness, 10-25% reduced physical damage taken, 75-200 regen cap, knockback immunity
	UITEM_IRONBARK,
	// G - Doubles effects of all small charms
	UITEM_WELLOFPOWER,
	// S - 5-10 to all stats, 3-5% chance to be healed for 6-10% of your missing health on being hit
	UITEM_ANCIENTGEMSTONE,
	// S - 5-45% increased lightning damage, Lightning type attacks always crit
	UITEM_DEATHSPARK,
	// S - 25-50% increased pellets, Shotgun type weapons deal 40-75% more damage
	UITEM_SHELLSHOCK,
	// M - Your health cap is doubled, Can't use armor, Gain 1% damage increase every 75 - 50 max health
	UITEM_OAKHEART,
	// M - 50-100% more pellets, 150-250% slot 3 damage, Pellets fire in a circle around you regardless of accuracy
	UITEM_PELLETSTORM,
	// G - 15-25 Intellect, Slain enemies have 5-15% chance to raise a zombie (Max 5, Lasts 8 + INT / 10 seconds), Damage taken is shared between all summoned creatures, Take 50-25% more damage
	UITEM_GRAVECALLER,
	// M - 6-12% speed, 3-10 flat damage to all attacks, Killing enemies heals for 1-5% missing health 
	UITEM_ESSENCEEATER,
	// S - Soul type weapons do full damage and can hit ghosts, Soul pickups give 50-100% more ammo, Spells do irreducible damage
	UITEM_EYEBEHOLDER,
	// G - 250-425 health cap, 250-425 armor cap, 4 - 15 to all stats, Gives item that grants 35-75% damage and 8-18% speed for 8 seconds (20 sec cd, stats depend on level), Slain enemies rest in peace
	UITEM_DEADKINGBANNER,
	// S - + 5 - 10 flat damage to damage over time effects, 50 - 100% increased damage over time duration
	UITEM_PAINMASTER,
	// M - Crit chance is lucky, Critical hits have 20 - 35% chance to ignore all resists
	UITEM_VOIDEMBLEM
};

#define LAST_UNIQUE_ITEM UITEM_VOIDEMBLEM
#define MAX_UNIQUE_ITEMS (LAST_UNIQUE_ITEM + 1)

// this is used to construct items
typedef struct it_con {
	int width;										// width in inventory space
	int height;										// height in inventory space
	int item_image;									// image of item from image list
	int item_type;									// what type of item it is (>65535 implies this item is a unique, >> 16 - 1 gives unique id)
	int item_subtype;								// subtype for items that have it (charms etc)
	int item_level;									// what level this item is
	int item_stack;									// the stack of the item (if applicable)
	int attrib_count;								// count of attributes
	int attrib_id_list[MAX_ITEM_ATTRIBUTES];		// contains id list of corresponding attributes
	inv_attrib_T rolls[MAX_ITEM_ATTRIBUTES];		// contains roll information of the attributes (level modifier isn't used here)
} inventory_constructor_T;

global inventory_constructor_T 63: UniqueItemList[MAX_UNIQUE_ITEMS];

#endif
