#ifndef DND_UNIQUE_IN
#define DND_UNIQUE_IN

#define UNIQUE_DROPCHANCE 0.005
#define UNIQUE_BITS 16

// s, m and g indicate charm type
enum {
	// M - 50% increased armor cap, 10-25% reduced elemental damage taken, 2-5% chance of casting a random elemental spell while firing
	UITEM_ELEMENTALBULWARK,
	// G - 5-30 flat physical damage, 10-25 bulkiness, 10-25% reduced physical damage taken, 75-200 regen cap, knockback immunity
	UITEM_IRONBARK,
	// G - Doubles effects of all small charms
	UITEM_WELLOFPOWER,
	// S - 5-10 to all stats, 3-5% chance to be healed for 6-10% of your maximum health on being hit
	UITEM_ANCIENTGEMSTONE,
	// S - 5-25% increased lightning damage, Lightning type attacks always crit
	UITEM_DEATHSPARK,
	// S - 15-35% increased pellets, Shotgun type weapons deal 25-50% more damage
	UITEM_SHELLSHOCK,
	// M - Your health cap is doubled, Can't use armor, Gain 1% damage increase every 75 - 50 max health
	UITEM_OAKHEART,
	// M - 40-50% more pellets, 100-200% slot 3 damage, Pellets fire in a circle around you regardless of accuracy
	UITEM_PELLETSTORM,
	// G - 15-25 Intellect, Slain enemies have 10-15% chance to raise a zombie (Max 5, Lasts 15 seconds), Damage taken is shared between all summoned creatures, Take 25-10% more damage
	UITEM_GRAVECALLER,
	// M - 6-12% speed, 3-10 flat damage to all attacks, Killing enemies heals for 1-5% missing health 
	UITEM_LIFELEECH,
	// S - Soul type weapons do full damage and can hit ghosts, Soul pickups give 25-50% more ammo
	UITEM_EYEBEHOLDER,
	// G - 250-400 health cap, 250-400 armor cap, Gives item that grants 25% damage and 15% speed for 8 seconds (30 sec cd), Slain enemies rest in peace
	UITEM_DEADKINGBANNER
};

#define LAST_UNIQUE_ITEM UITEM_DEADKINGBANNER
#define MAX_UNIQUE_ITEMS LAST_UNIQUE_ITEM + 1

str UniqueItemNames[MAX_UNIQUE_ITEMS] = {
	"Elemental Bulwark",
	"Ironbark",
	"Well of Power",
	"Ancient Gemstone",
	"Deathspark",
	"Shellshock",
	"Oakheart",
	"Pellet Storm",
	"Grave Caller",
	"Life Leech",
	"Eye of Beholder",
	"Dead King's Banner"
};

#define MAX_UNIQUE_WEIGHT 1000
// chances
/*
18
15
3
5
6
13
9
8
6
6
8
3
*/
int UniqueItemDropWeight[MAX_UNIQUE_ITEMS] = {
	180,
	330,
	360,
	410,
	470,
	600,
	690,
	770,
	830,
	890,
	970,
	1000
};

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

// initializes all uniques
Script "DnD Load Uniques" OPEN {
	// construct unique list to copy from
	int id = UITEM_ELEMENTALBULWARK;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = 0;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_ARMORPERCENT_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_DMGREDUCE_ELEM;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_CHANCE_CASTELEMSPELL;
	UniqueItemList[id].rolls[0].attrib_low = 50;
	UniqueItemList[id].rolls[0].attrib_high = 50;
	UniqueItemList[id].rolls[1].attrib_low = 10;
	UniqueItemList[id].rolls[1].attrib_high = 25;
	UniqueItemList[id].rolls[2].attrib_low = 2;
	UniqueItemList[id].rolls[2].attrib_high = 5;
	id = UITEM_IRONBARK;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = 0;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 5;
	UniqueItemList[id].attrib_id_list[0] = INV_FLATPHYS_DAMAGE;
	UniqueItemList[id].attrib_id_list[1] = INV_STAT_BULKINESS;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_DMGREDUCE_PHYS;
	UniqueItemList[id].attrib_id_list[3] = INV_REGENCAP_INCREASE;
	UniqueItemList[id].attrib_id_list[4] = INV_EX_KNOCKBACK_IMMUNITY;
	UniqueItemList[id].rolls[0].attrib_low = 5;
	UniqueItemList[id].rolls[0].attrib_high = 30;
	UniqueItemList[id].rolls[1].attrib_low = 10;
	UniqueItemList[id].rolls[1].attrib_high = 25;
	UniqueItemList[id].rolls[2].attrib_low = 10;
	UniqueItemList[id].rolls[2].attrib_high = 25;
	UniqueItemList[id].rolls[3].attrib_low = 75;
	UniqueItemList[id].rolls[3].attrib_high = 200;
	UniqueItemList[id].rolls[4].attrib_low = 1;
	UniqueItemList[id].rolls[4].attrib_high = 1;
	id = UITEM_WELLOFPOWER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = 0;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 1;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_DOUBLE_SMALLCHARM;
	UniqueItemList[id].rolls[0].attrib_low = 2;
	UniqueItemList[id].rolls[0].attrib_high = 2;
	id = UITEM_ANCIENTGEMSTONE;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = 0;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_ALLSTATS;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_DOUBLE_SMALLCHARM;
	UniqueItemList[id].rolls[0].attrib_low = 5;
	UniqueItemList[id].rolls[0].attrib_high = 10;
	UniqueItemList[id].rolls[1].attrib_low = 3 | (6 << 16); // secondary portion
	UniqueItemList[id].rolls[1].attrib_high = 5 | (10 << 16);
	id = UITEM_DEATHSPARK;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = 0;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_DMGINCREASE_LIGHTNING;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_ALWAYSCRIT_LIGHTNING;
	UniqueItemList[id].rolls[0].attrib_low = 5;
	UniqueItemList[id].rolls[0].attrib_high = 25;
	UniqueItemList[id].rolls[1].attrib_low = 1;
	UniqueItemList[id].rolls[1].attrib_high = 1;
	id = UITEM_SHELLSHOCK;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = 0;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_PELLET_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_DMGINCREASE_SHOTGUNS;
	UniqueItemList[id].rolls[0].attrib_low = 15;
	UniqueItemList[id].rolls[0].attrib_high = 35;
	UniqueItemList[id].rolls[1].attrib_low = 25;
	UniqueItemList[id].rolls[1].attrib_high = 50;
	id = UITEM_OAKHEART;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = 0;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_DOUBLE_HEALTHCAP;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_FORBID_ARMOR;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_DAMAGEPER_FLATHEALTH;
	UniqueItemList[id].rolls[0].attrib_low = 1;
	UniqueItemList[id].rolls[0].attrib_high = 1;
	UniqueItemList[id].rolls[1].attrib_low = 1;
	UniqueItemList[id].rolls[1].attrib_high = 1;
	UniqueItemList[id].rolls[2].attrib_low = 50;
	UniqueItemList[id].rolls[2].attrib_high = 75;
	id = UITEM_PELLETSTORM;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = 0;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_PELLET_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_SLOT3_DAMAGE;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_BEHAVIOR_PELLETSFIRECIRCLE;
	UniqueItemList[id].rolls[0].attrib_low = 40;
	UniqueItemList[id].rolls[0].attrib_high = 50;
	UniqueItemList[id].rolls[1].attrib_low = 100;
	UniqueItemList[id].rolls[1].attrib_high = 200;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 1;
	id = UITEM_GRAVECALLER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = 0;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 4;
	UniqueItemList[id].attrib_id_list[0] = INV_STAT_INTELLECT;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_CHANCE_ONDEATH_RAISEZOMBIE;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_DMGREDUCE_SHAREWITHPETS;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_DMGINCREASE_TAKEN;
	UniqueItemList[id].rolls[0].attrib_low = 15;
	UniqueItemList[id].rolls[0].attrib_high = 25;
	UniqueItemList[id].rolls[1].attrib_low = 10;
	UniqueItemList[id].rolls[1].attrib_high = 15;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 1;
	UniqueItemList[id].rolls[3].attrib_low = 10;
	UniqueItemList[id].rolls[3].attrib_high = 25;
	id = UITEM_LIFELEECH;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = 0;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_SPEED_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_FLATDMG_ALL;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_ONKILL_HEALPERCENT;
	UniqueItemList[id].rolls[0].attrib_low = 6;
	UniqueItemList[id].rolls[0].attrib_high = 12;
	UniqueItemList[id].rolls[1].attrib_low = 3;
	UniqueItemList[id].rolls[1].attrib_high = 10;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 5;
	id = UITEM_EYEBEHOLDER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = 0;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_SOULWEPS_FULLDAMAGE;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_PICKUPS_MORESOUL;
	UniqueItemList[id].rolls[0].attrib_low = 1;
	UniqueItemList[id].rolls[0].attrib_high = 1;
	UniqueItemList[id].rolls[1].attrib_low = 25;
	UniqueItemList[id].rolls[1].attrib_high = 50;
	id = UITEM_DEADKINGBANNER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = 0;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 4;
	UniqueItemList[id].attrib_id_list[0] = INV_HP_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_ARMOR_INCREASE;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_ABILITY_RALLY;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_ABILITY_MONSTERSRIP;
	UniqueItemList[id].rolls[0].attrib_low = 250;
	UniqueItemList[id].rolls[0].attrib_high = 400;
	UniqueItemList[id].rolls[1].attrib_low = 250;
	UniqueItemList[id].rolls[1].attrib_high = 400;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 1;
	UniqueItemList[id].rolls[3].attrib_low = 1;
	UniqueItemList[id].rolls[3].attrib_high = 1;
}

#endif