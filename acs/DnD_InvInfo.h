#ifndef DND_INVINFO_IN
#define DND_INVINFO_IN

// put common inventory information here
#define MAX_ITEM_ATTRIBUTES 9

// only orbs
#define MAX_CRAFTITEMTYPES 2

#define AIF_ONLYEXOTIC 1

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

#define DND_SYNC_NONORB 0
#define DND_SYNC_ORB 1

enum {
	DND_CHESTTYPE_BRONZE,
	DND_CHESTTYPE_SILVER,
	DND_CHESTTYPE_GOLD,
	
	DND_ELIXIR_HEALTH,
	DND_ELIXIR_ARMOR,
	DND_ELIXIR_HPARMOR,
	DND_ELIXIR_HPPERCENT,
	DND_ELIXIR_ARMORPERCENT,
	DND_ELIXIR_HPARMORPERCENT,
	DND_ELIXIR_SPEED,
	DND_ELIXIR_DAMAGE,
	DND_ELIXIR_LUCK,
	
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
	DND_ORB_ELEVATION
};

#define CHESTKEY_BEGIN DND_CHESTTYPE_BRONZE
#define ELIXIR_BEGIN DND_ELIXIR_HEALTH
#define ORBS_BEGIN DND_ORB_ENHANCE

#define CHESTKEY_END DND_CHESTTYPE_GOLD
#define ELIXIR_END DND_ELIXIR_LUCK
#define ORBS_END DND_ORB_ELEVATION

#define MAX_CHESTKEYS CHESTKEY_END - CHESTKEY_BEGIN + 1
#define MAX_ELIXIRS ELIXIR_END - ELIXIR_BEGIN + 1
#define MAX_ORBS ORBS_END - ORBS_BEGIN + 1

#define MAX_COMMON_INVENTORY DND_ORB_ELEVATION + 1

#define SITEM_NAME 0
#define SITEM_TAG 1
#define SITEM_DESC 2
str InventoryInfo[MAX_COMMON_INVENTORY][3] = {
	{ "BronzeChestKey", "Bronze Chest Key", "\c[Y5]Bronze Chest Key\nOpens a mysterious bronze chest." },
	{ "SilverChestKey", "Silver Chest Key", "\c[Y5]Silver Chest Key\nOpens a mysterious silver chest." },
	{ "GoldChestKey", "Gold Chest Key", "\c[Y5]Gold Chest Key\nOpens a mysterious gold chest." },
	
	{ "ElixirOfHealth", "Elixir of Health", "\c[Y5]Elixir of Health\c-\nAdds 5 to your health cap." },
	{ "ElixirOfArmor", "Elixir of Armor", "\c[Y5]Elixir of Armor\c-\nAdds 5 to your armor cap." },
	{ "ElixirOfProsperity", "Elixir of Prosperity", "\c[Y5]Elixir of Prosperity\c-\nAdds 5 to your health and armor caps." },
	{ "ElixirOfLife", "Elixir of Life", "\c[Y5]Elixir of Life\c-\nIncreases health cap by 1%." },
	{ "ElixirOfStrongness", "Elixir of Strongness", "\c[Y5]Elixir of Strongness\c-\nIncreases armor cap by 1%." },
	{ "ElixirOfFortitude", "Elixir of Fortitude", "\c[Y5]Elixir of Fortitude\c-\nIncreases health and armor caps by 1%." },
	{ "ElixirOfSpeed", "Elixir of Speed", "\c[Y5]Elixir of Speed\c-\nIncreases your speed by 1%."  },
	{ "ElixirOfDamage", "Elixir of Damage", "\c[Y5]Elixir of Damage\c-\nAdds 1% damage to all damage types." },
	{ "ElixirOfLuck", "Elixir of Luck", "\c[Y5]Elixir of Luck\c-\nGives 5% increased drop rate." },
	
	{ "OrbOfEnhancement", "Orb of Enhancement", "\c[Y5]Orb of Enhancement\nAllows the user to improve the quality of the currently selected weapon upon use, increasing its damage. Increases damage of selected weapon by \cd1%\c-, up to \cg25%\c- total." },
	{ "OrbOfCorruption", "Orb of Corruption", "\c[Y5]Orb of Corruption\nThis orb can do many things. Half the time it'll do nothing. More often than not it'll do something bad. However it can do some awesome things as well... Dare to find out?" },
	{ "OrbOfSpirit", "Orb of Spirit", "\c[Y5]Orb of Spirit\nGrants the user a random stat upon use. Stats can go above \cd100\c- with this orb. Charisma has lowest weight, followed by Vitality and Bulkiness. Rest share the highest weight." },
	{ "OrbofRepentance", "Orb of Repentance", "\c[Y5]Orb of Repentance\nAllows you to undo the effects of the most recently used orb. You can't undo the effects undone by this orb or the \c[Y5]Orb of Riches\c-." },
	{ "OrbofAffluence", "Orb of Affluence", "\c[Y5]Orb of Affluence\nDoubles the effect of the next orb. This effect can stack with itself up to a multiplier of \cg16\c-." },
	{ "OrbofCalamity", "Orb of Calamity", "\c[Y5]Orb of Calamity\nTurns a random orb into another. It won't work if you have no other orbs or if you only have these orbs." },
	{ "OrbofProsperity", "Orb of Prosperity", "\c[Y5]Orb of Prosperity\nIncreases health and armor caps by \cd1\c- up to a maximum of \cg500\c-." },
	{ "OrbofWisdom", "Orb of Wisdom", "\c[Y5]Orb of Wisdom\nIncreases experience gain from monsters by \cd1%\c- up to a maximum of \cg250%\c-." },
	{ "OrbofGreed", "Orb of Greed", "\c[Y5]Orb of Greed\nIncreases credit gain from monsters by \cd1%\c- up to a maximum of \cg250%\c-." },
	{ "OrbofViolence", "Orb of Violence", "\c[Y5]Orb of Violence\nIncreases a random damage type's damage by \cd1%\c- up to a maximum of \cg300%\c- for each category." },
	{ "OrbofFortitude", "Orb of Fortitude", "\c[Y5]Orb of Fortitude\nIncreases health and armor caps by \cd1%\c- up to a maximum of \cg200%\c-." },
	{ "OrbofSin", "Orb of Sin", "\c[Y5]Orb of Sin\nA pact with the devil itself, trading anywhere from \cg1\c- to \cg8\c- stat points for random benefits. Can give some stat points, critical chance, or even a perk point!" },
	{ "OrbofRiches", "Orb of Riches", "\c[Y5]Orb of Riches\nGrants a random resource on use. Can grant experience, credit or budget. Base values are \cd5%\c- of your current level's experience, \cd2000\c- and \cd5k\c- respectively." },
	{ "OrbofHolding", "Orb of Holding", "\c[Y5]Orb of Holding\nIncreases your ammo capacities by \cd1%\c- up to a maximum of \cg100%\c-. Doesn't increase the capacity of \cusouls\c- or \cstemporary weapons." },
	{ "OrbofRefinement", "Orb of Refinement", "\c[Y5]Orb of Refinement\nRerolls the attribute values of the selected item." },
	{ "OrbofSculpting", "Orb of Sculpting", "\c[Y5]Orb of Sculpting\nRemoves a random attribute entirely from the selected item." },
	{ "OrbofElevation", "Orb of Elevation", "\c[Y5]Orb of Elevation\nAdds a random attribute that's not already present to the selected item, if it has capacity." }
};

#endif