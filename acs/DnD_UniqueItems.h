#ifndef DND_UNIQUE_IN
#define DND_UNIQUE_IN

#define UNIQUE_DROPCHANCE 0.0875
#define UNIQUE_BITS 16
#define UNIQUE_BEGIN 65535

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
	UITEM_LIFELEECH,
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
	"Eye of the Beholder",
	"Dead King's Banner",
	"Pain Master",
	"Void Emblem"
};

#define MAX_UNIQUE_WEIGHT 1000
// chances -- no longer valid -- need to come up with some weighted formula later
/*
15
15
4
6
6
13
9
8
6
6
8
4
*/
int UniqueItemDropWeight[MAX_UNIQUE_ITEMS] = {
	150,
	300,
	340,
	400,
	460,
	590,
	680,
	760,
	800,
	825,
	850,
	880,
	960,
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
	UniqueItemList[id].item_image = IIMG_UCHRM_1;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_ARMORPERCENT_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_DMGREDUCE_ELEM;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_CHANCE_CASTELEMSPELLONATK;
	UniqueItemList[id].rolls[0].attrib_low = 50;
	UniqueItemList[id].rolls[0].attrib_high = 50;
	UniqueItemList[id].rolls[1].attrib_low = 10;
	UniqueItemList[id].rolls[1].attrib_high = 25;
	UniqueItemList[id].rolls[2].attrib_low = 2;
	UniqueItemList[id].rolls[2].attrib_high = 5;
	id = UITEM_IRONBARK;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = IIMG_UCHRM_2;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 5;
	UniqueItemList[id].attrib_id_list[0] = INV_FLATPHYS_DAMAGE;
	UniqueItemList[id].attrib_id_list[1] = INV_STAT_BULKINESS;
	UniqueItemList[id].attrib_id_list[2] = INV_DMGREDUCE_PHYS;
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
	UniqueItemList[id].rolls[4].attrib_low = 0;
	UniqueItemList[id].rolls[4].attrib_high = 0;
	id = UITEM_WELLOFPOWER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = IIMG_UCHRM_3;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_FACTOR_SMALLCHARM;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_DMGINCREASE_TAKEN;
	UniqueItemList[id].rolls[0].attrib_low = 1100; // will be divided by 100 after mult (1.1 to 2)
	UniqueItemList[id].rolls[0].attrib_high = 2000;
	UniqueItemList[id].rolls[1].attrib_low = 15;
	UniqueItemList[id].rolls[1].attrib_high = 75;
	id = UITEM_ANCIENTGEMSTONE;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_4;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_ALLSTATS;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_CHANCE;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_CHANCE_HEALMISSINGONPAIN;
	UniqueItemList[id].rolls[0].attrib_low = 5;
	UniqueItemList[id].rolls[0].attrib_high = 10;
	UniqueItemList[id].rolls[1].attrib_low = 3; // chance to proc
	UniqueItemList[id].rolls[1].attrib_high = 5;
	UniqueItemList[id].rolls[2].attrib_low = 6; // heal %
	UniqueItemList[id].rolls[2].attrib_high = 10;
	id = UITEM_DEATHSPARK;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_5;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_DMGINCREASE_LIGHTNING;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_MORECRIT_LIGHTNING;
	UniqueItemList[id].rolls[0].attrib_low = 5;
	UniqueItemList[id].rolls[0].attrib_high = 45;
	UniqueItemList[id].rolls[1].attrib_low = 0.5;
	UniqueItemList[id].rolls[1].attrib_high = 1.25;
	id = UITEM_SHELLSHOCK;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_6;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_PELLET_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_DMGINCREASE_SHOTGUNS;
	UniqueItemList[id].rolls[0].attrib_low = 25;
	UniqueItemList[id].rolls[0].attrib_high = 50;
	UniqueItemList[id].rolls[1].attrib_low = 40;
	UniqueItemList[id].rolls[1].attrib_high = 75;
	id = UITEM_OAKHEART;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UCHRM_7;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_DOUBLE_HEALTHCAP;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_FORBID_ARMOR;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_DAMAGEPER_FLATHEALTH;
	UniqueItemList[id].rolls[0].attrib_low = 100;
	UniqueItemList[id].rolls[0].attrib_high = 100;
	UniqueItemList[id].rolls[1].attrib_low = 0;
	UniqueItemList[id].rolls[1].attrib_high = 0;
	UniqueItemList[id].rolls[2].attrib_low = 50;
	UniqueItemList[id].rolls[2].attrib_high = 75;
	id = UITEM_PELLETSTORM;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UCHRM_8;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 4;
	UniqueItemList[id].attrib_id_list[0] = INV_PELLET_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_SLOT3_DAMAGE;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_BEHAVIOR_PELLETSFIRECIRCLE;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_FORSHOW_BURSTGETSPELLETBONUS;
	UniqueItemList[id].rolls[0].attrib_low = 50;
	UniqueItemList[id].rolls[0].attrib_high = 100;
	UniqueItemList[id].rolls[1].attrib_low = 150;
	UniqueItemList[id].rolls[1].attrib_high = 250;
	UniqueItemList[id].rolls[2].attrib_low = 0;
	UniqueItemList[id].rolls[2].attrib_high = 0;
	UniqueItemList[id].rolls[3].attrib_low = 0;
	UniqueItemList[id].rolls[3].attrib_high = 0;
	id = UITEM_GRAVECALLER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = IIMG_UCHRM_9;
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
	UniqueItemList[id].rolls[1].attrib_low = 5;
	UniqueItemList[id].rolls[1].attrib_high = 15;
	UniqueItemList[id].rolls[2].attrib_low = 0;
	UniqueItemList[id].rolls[2].attrib_high = 0;
	UniqueItemList[id].rolls[3].attrib_low = 25;
	UniqueItemList[id].rolls[3].attrib_high = 50;
	id = UITEM_LIFELEECH;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UCHRM_10;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_SPEED_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_FLATDMG_ALL;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_ONKILL_HEALMISSING;
	UniqueItemList[id].rolls[0].attrib_low = 0.06;
	UniqueItemList[id].rolls[0].attrib_high = 0.12;
	UniqueItemList[id].rolls[1].attrib_low = 3;
	UniqueItemList[id].rolls[1].attrib_high = 10;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 5;
	id = UITEM_EYEBEHOLDER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_11;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_SOULWEPS_FULLDAMAGE;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_PICKUPS_MORESOUL;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_BEHAVIOR_SPELLSFULLDAMAGE;
	UniqueItemList[id].rolls[0].attrib_low = 0;
	UniqueItemList[id].rolls[0].attrib_high = 0;
	UniqueItemList[id].rolls[1].attrib_low = 50;
	UniqueItemList[id].rolls[1].attrib_high = 100;
	UniqueItemList[id].rolls[2].attrib_low = 0;
	UniqueItemList[id].rolls[2].attrib_high = 0;
	id = UITEM_DEADKINGBANNER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = IIMG_UCHRM_12;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 5;
	UniqueItemList[id].attrib_id_list[0] = INV_HP_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_ARMOR_INCREASE;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_ALLSTATS;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_ABILITY_RALLY;
	UniqueItemList[id].attrib_id_list[4] = INV_EX_ABILITY_MONSTERSRIP;
	UniqueItemList[id].rolls[0].attrib_low = 250;
	UniqueItemList[id].rolls[0].attrib_high = 425;
	UniqueItemList[id].rolls[1].attrib_low = 250;
	UniqueItemList[id].rolls[1].attrib_high = 425;
	UniqueItemList[id].rolls[2].attrib_low = 4;
	UniqueItemList[id].rolls[2].attrib_high = 15;
	UniqueItemList[id].rolls[3].attrib_low = 1;
	UniqueItemList[id].rolls[3].attrib_high = 10;
	UniqueItemList[id].rolls[4].attrib_low = 0;
	UniqueItemList[id].rolls[4].attrib_high = 0;
	id = UITEM_PAINMASTER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_13;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_FLATDOT;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_DOTDURATION;
	UniqueItemList[id].rolls[0].attrib_low = 5;
	UniqueItemList[id].rolls[0].attrib_high = 10;
	UniqueItemList[id].rolls[1].attrib_low = 50;
	UniqueItemList[id].rolls[1].attrib_high = 100;
	id = UITEM_VOIDEMBLEM;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UCHRM_14;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 100;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_ABILITY_LUCKYCRIT;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_CRITIGNORERESCHANCE;
	UniqueItemList[id].rolls[0].attrib_low = 0;
	UniqueItemList[id].rolls[0].attrib_high = 0;
	UniqueItemList[id].rolls[1].attrib_low = 20;
	UniqueItemList[id].rolls[1].attrib_high = 35;
}

#endif
