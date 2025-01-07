#ifndef DND_INVINFO_IN
#define DND_INVINFO_IN

#include "DnD_OrbDefs.h"

// put common inventory information here
#define MAX_ITEM_ATTRIBUTES 9

// only orbs and tokens
#define MAX_CRAFTITEMTYPES 2

#define DND_CYBERNETIC_FACTOR 3 // 33%

#define DND_MAX_ITEM_QUALITY 25
#define DND_MAX_CHARM_QUALITY 25

// for hybrid armor implicits
#define DND_ARMOR_TO_MIT_RATIO 25 // armor mit or shield mit implicits will only give this factor of the rating provided as mitigation %
#define DND_SHIELD_TO_MIT_RATIO 15 // same as above but for es

#define MAX_ITEM_LEVEL 100

// main item types
enum {
	DND_ITEM_NULL,
	DND_ITEM_TEMPORARY,
	DND_ITEM_CHARM,
	DND_ITEM_HELM,
	DND_ITEM_BOOT,
	DND_ITEM_BODYARMOR,
	DND_ITEM_POWERCORE,
	DND_ITEM_ORB,
	DND_ITEM_CHESTKEY,
	DND_ITEM_WEAPON,
	DND_ITEM_TOKEN
};

enum {
	DND_STACKEDITEM_ORB,
	DND_STACKEDITEM_CHESTKEY,
	DND_STACKEDITEM_TOKEN
};

enum {
	DND_CRAFTABLEID_CHARM,
	DND_CRAFTABLEID_BODYARMOR,
	DND_CRAFTABLEID_BOOT,
	DND_CRAFTABLEID_HELM,
	DND_CRAFTABLEID_POWERCORE
};
#define MAX_CRAFTABLEITEMTYPES (DND_CRAFTABLEID_POWERCORE + 1)

enum {
	DND_CHARM_SMALL,
	DND_CHARM_MEDIUM,
	DND_CHARM_LARGE
};
#define MAX_CHARM_TYPES (DND_CHARM_LARGE + 1)

typedef struct {
	int attrib_val;
	int attrib_id;
	int attrib_tier;
	int attrib_extra;								// this one can hold an extra information regarding the attribute, or corrupted implicit
	bool fractured;
} attr_inf_T;
#define ATTRIB_DATA_COUNT 5

typedef struct it {
	int width;										// width in inventory space
	int height;										// height in inventory space
	int item_image;									// image of item from image list
	int item_type;									// what type of item it is (>65535 implies this item is a unique, >> 16 - 1 gives unique id)
	int item_subtype;								// subtype for items that have it (charms etc)
	int item_level;									// what level this item is
	int item_stack;									// the stack of the item (if applicable)
	int topleftboxid;								// used to determine the owning pointer (-1 of this is the pointer)

	bool corrupted;									// is the item corrupted?
	attr_inf_T implicit;							// id of the implicit attribute -- can be a corrupted implicit too
	int quality;

	int attrib_count;								// count of attributes
	attr_inf_T attributes[MAX_ITEM_ATTRIBUTES];		// attribute list
} inventory_T;

// The following are seperated to fitting categories, they are used this way in many places
enum {
	DND_CHESTTYPE_BRONZE,
	DND_CHESTTYPE_SILVER,
	DND_CHESTTYPE_GOLD
};

// these are used for accessing common info table below
enum {
	DND_ISUBT_CHESTTYPE_BRONZE,
	DND_ISUBT_CHESTTYPE_SILVER,
	DND_ISUBT_CHESTTYPE_GOLD,
	
	DND_ISUBT_ORB_0,
	DND_ISUBT_ORB_1,
	DND_ISUBT_ORB_2,
	DND_ISUBT_ORB_3,
	DND_ISUBT_ORB_4,
	DND_ISUBT_ORB_5,
	DND_ISUBT_ORB_6,
	DND_ISUBT_ORB_7,
	DND_ISUBT_ORB_8,
	DND_ISUBT_ORB_9,
	DND_ISUBT_ORB_10,
	DND_ISUBT_ORB_11,
	DND_ISUBT_ORB_12,
	DND_ISUBT_ORB_13,
	DND_ISUBT_ORB_14,
	DND_ISUBT_ORB_15,
	DND_ISUBT_ORB_16,
	DND_ISUBT_ORB_17,
	DND_ISUBT_ORB_18,
	DND_ISUBT_ORB_19,
	DND_ISUBT_ORB_20,
	DND_ISUBT_ORB_21,
	DND_ISUBT_ORB_22,
	DND_ISUBT_ORB_23,
	DND_ISUBT_ORB_24,
	DND_ISUBT_ORB_25,
	DND_ISUBT_ORB_26,
	
	// monster specific
	DND_ISUBT_MORB_0,
	DND_ISUBT_MORB_1,
	DND_ISUBT_MORB_2,
	
	DND_ISUBT_TOKEN_ARMORER,
	DND_ISUBT_TOKEN_GUNSMITH
};

#define CHESTKEY_BEGIN DND_ISUBT_CHESTTYPE_BRONZE
#define ORBS_BEGIN DND_ISUBT_ORB_0
#define MONSTER_ORBS_BEGIN DND_ISUBT_MORB_0
#define TOKEN_BEGIN DND_ISUBT_TOKEN_ARMORER

#define CHESTKEY_END DND_ISUBT_CHESTTYPE_GOLD
#define ORBS_END DND_ISUBT_ORB_26
#define MONSTER_ORBS_END DND_ISUBT_MORB_2
#define TOKEN_END DND_ISUBT_TOKEN_GUNSMITH

#define MAX_CHESTKEYS (CHESTKEY_END - CHESTKEY_BEGIN + 1)
#define MAX_ORBS (MONSTER_ORBS_END - ORBS_BEGIN + 1) // notice the monster drops were below regular and we use their index as final here
#define MAX_TOKENS (TOKEN_END - TOKEN_BEGIN + 1)

int TokenWeights[MAX_TOKENS] = {
	8,
	10
};
#define MAX_TOKEN_WEIGHT 10

#define MAX_COMMON_INVENTORY (TOKEN_END + 1)

str GetInventoryName(int inv) {
	str label = "";
	if(inv <= CHESTKEY_END)
		label = StrParam(s:"DND_KEYN", d:inv + 1);
	else if(inv <= ORBS_END)
		label = StrParam(s:"DND_ORBN", d:inv + 1 - ORBS_BEGIN);
	else if(inv <= MONSTER_ORBS_END)
		label = StrParam(s:"DND_ORBNM", d:inv + 1 - MONSTER_ORBS_BEGIN);
	else
		label = StrParam(s:"DND_TOKENN", d:inv + 1 - TOKEN_BEGIN);
	return StrParam(l:label);
}

// the below two are for when only text or tag are needed, 3rd is for the entire thing being needed (a tiny optimization)
// draw tag with \c[Y5] color code, then \n then text
str GetInventoryTag(int id) {
	if(id <= CHESTKEY_END)
		return StrParam(s:"DND_CHESTKEY", d:id - CHESTKEY_BEGIN + 1);
	if(id <= ORBS_END)
		return StrParam(s:"DND_ORB", d:id - ORBS_BEGIN + 1);
	if(id <= MONSTER_ORBS_END)
		return StrParam(s:"DND_ORB_MON", d:id - MONSTER_ORBS_BEGIN + 1);
	return StrParam(s:"DND_TOKEN", d:id - TOKEN_BEGIN + 1);
}

str GetInventoryText(int id) {
	if(id <= CHESTKEY_END)
		return StrParam(s:"DND_CHESTKEYTEXT", d:id - CHESTKEY_BEGIN + 1);
	if(id <= ORBS_END)
		return StrParam(s:"DND_ORBTEXT", d:id - ORBS_BEGIN + 1);
	if(id <= MONSTER_ORBS_END)
		return StrParam(s:"DND_ORB_MONTEXT", d:id - MONSTER_ORBS_BEGIN + 1);
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
	// M - Your health cap is doubled, Can't use armor, Gain 1% phys damage increase every 75 - 50 max health
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
	UITEM_VOIDEMBLEM,
	UITEM_REKINDLEDSPARKS,
	UITEM_DEADEYEGLARE,
	UITEM_UNITY,
	UITEM_MINDFORGE,
	UITEM_SLAYERSPECIAL,
	// add new unique charms above to the general drop pool

	// specific monster drop unique charms below
	UITEM_ELEMENTALHARMONY,
	UITEM_STEELBARK,
	UITEM_DREAMINGGODIRE,
	UITEM_HEATBREAKER,
	UITEM_THORNVEIN,

	// add new powercores here
	UITEM_TESSERACT = 128,
	UITEM_PCORE_RES1,
	UITEM_PCORE_RES2,
	UITEM_PCORE_RES3,
	UITEM_PCORE_RES4,
	UITEM_PCORE_RES5,
	UITEM_PCORE_RES6,
	UITEM_PCORE_RES7,
	UITEM_PCORE_RES8,

	// add new body armors here
	UITEM_PROTOSHELL = 200,
	UITEM_PRISMGUARD,
	UITEM_BARONSDEN,
	UITEM_BODY_RESERVED4,
	UITEM_BODY_RESERVED5,
	UITEM_BODY_RESERVED6,
	UITEM_BODY_RESERVED7,
	UITEM_BODY_RESERVED8,
	UITEM_BODY_RESERVED9,
	UITEM_BODY_RESERVED10,
	UITEM_BODY_RESERVED11,
	UITEM_BODY_RESERVED12,
	UITEM_BODY_RESERVED13,
	UITEM_BODY_RESERVED14,
	UITEM_BODY_RESERVED15,
	UITEM_BODY_RESERVED16,

	// add new boots here
	UITEM_BOOT_RESERVED1 = 300,
	UITEM_BOOT_RESERVED2,
	UITEM_BOOT_RESERVED3,
	UITEM_BOOT_RESERVED4,
	UITEM_BOOT_RESERVED5,
	UITEM_BOOT_RESERVED6,
	UITEM_BOOT_RESERVED7,
	UITEM_BOOT_RESERVED8,
	UITEM_BOOT_RESERVED9,
	UITEM_BOOT_RESERVED10,
	UITEM_BOOT_RESERVED11,
	UITEM_BOOT_RESERVED12,
	UITEM_BOOT_RESERVED13,
	UITEM_BOOT_RESERVED14,
	UITEM_BOOT_RESERVED15,
	UITEM_BOOT_RESERVED16,

	// add new helms here
	UITEM_HELM_RESERVED1 = 400,
	UITEM_HELM_RESERVED2,
	UITEM_HELM_RESERVED3,
	UITEM_HELM_RESERVED4,
	UITEM_HELM_RESERVED5,
	UITEM_HELM_RESERVED6,
	UITEM_HELM_RESERVED7,
	UITEM_HELM_RESERVED8,
	UITEM_HELM_RESERVED9,
	UITEM_HELM_RESERVED10,
	UITEM_HELM_RESERVED11,
	UITEM_HELM_RESERVED12,
	UITEM_HELM_RESERVED13,
	UITEM_HELM_RESERVED14,
	UITEM_HELM_RESERVED15,
	UITEM_HELM_RESERVED16,	
};

#define UNIQUE_CHARM_BEGIN UITEM_ELEMENTALBULWARK
#define UNIQUE_CHARM_END UITEM_DREAMINGGODIRE
#define UNIQUE_CHARM_REGULARDROP_END UITEM_SLAYERSPECIAL

#define UNIQUE_POWERCORE_BEGIN UITEM_TESSERACT
#define UNIQUE_POWERCORE_END UITEM_TESSERACT

#define UNIQUE_BODYARMOR_BEGIN UITEM_PROTOSHELL
#define UNIQUE_BODYARMOR_END UITEM_BARONSDEN

#define UNIQUE_HELM_END UITEM_HELM_RESERVED16

// this is used to construct items
typedef struct it_con {
	int weight;										// drop weight
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

#define MAX_UNIQUE_ITEMS (UNIQUE_HELM_END - UNIQUE_CHARM_BEGIN + 1)
global inventory_constructor_T 63: UniqueItemList[MAX_UNIQUE_ITEMS];

#endif
