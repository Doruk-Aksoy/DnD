#ifndef DND_INVINFO_IN
#define DND_INVINFO_IN

// put common inventory information here
#define MAX_ITEM_ATTRIBUTES 9
#define MAX_CRAFTING_MATERIALBOXES 12

// only orbs and elixirs
#define MAX_CRAFTITEMTYPES 2

#define DND_CYBERNETIC_FACTOR_MUL 13 // +30% = x1.3
#define DND_CYBERNETIC_FACTOR_DIV 10

enum {
	DND_INVCATEGORY_WORN,
	DND_INVCATEGORY_CHARM
};
#define MAX_INVENTORY_CATEGORIES DND_INVCATEGORY_CHARM + 1

#define CHARMSTR_COLORCODE 0
#define CHARMSTR_TIERTAG 1

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

#endif
