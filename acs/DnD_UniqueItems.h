#ifndef DND_UNIQUE_IN
#define DND_UNIQUE_IN

#define UNIQUE_DROPCHANCE 0.05
#define UNIQUE_BITS 16
#define UNIQUE_BEGIN 65535

str GetUniqueItemName(int id) {
	return StrParam(s:"DND_UNIQUE", d:id + 1);
}

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

// initializes all uniques
void SetupUniqueItems() {
	// construct unique list to copy from
	int id = UITEM_ELEMENTALBULWARK;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UCHRM_1;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 21;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_ARMORPERCENT_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_DMGREDUCE_ELEM;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_CHANCE_CASTELEMSPELLONATK;
	UniqueItemList[id].rolls[0].attrib_low = 25;
	UniqueItemList[id].rolls[0].attrib_high = 50;
	UniqueItemList[id].rolls[1].attrib_low = 10.0;
	UniqueItemList[id].rolls[1].attrib_high = 25.0;
	UniqueItemList[id].rolls[2].attrib_low = 2;
	UniqueItemList[id].rolls[2].attrib_high = 5;
	id = UITEM_IRONBARK;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = IIMG_UCHRM_2;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 18;
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
	UniqueItemList[id].rolls[2].attrib_low = 10.0;
	UniqueItemList[id].rolls[2].attrib_high = 25.0;
	UniqueItemList[id].rolls[3].attrib_low = 75;
	UniqueItemList[id].rolls[3].attrib_high = 200;
	UniqueItemList[id].rolls[4].attrib_low = 1;
	UniqueItemList[id].rolls[4].attrib_high = 1;
	id = UITEM_WELLOFPOWER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = IIMG_UCHRM_3;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 30;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_FACTOR_SMALLCHARM;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_DMGINCREASE_TAKEN;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_LIMITEDSMALLCHARMS;
	UniqueItemList[id].rolls[0].attrib_low = 1250; // will be divided by 100 after mult (1.25 to 2)
	UniqueItemList[id].rolls[0].attrib_high = 1800;
	UniqueItemList[id].rolls[1].attrib_low = 25;
	UniqueItemList[id].rolls[1].attrib_high = 40;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 3;
	id = UITEM_ANCIENTGEMSTONE;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_4;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 30;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_ALLSTATS;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_CHANCE;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_CHANCE_HEALMISSINGONPAIN;
	UniqueItemList[id].rolls[0].attrib_low = 5;
	UniqueItemList[id].rolls[0].attrib_high = 20;
	UniqueItemList[id].rolls[1].attrib_low = 3; // chance to proc
	UniqueItemList[id].rolls[1].attrib_high = 8;
	UniqueItemList[id].rolls[2].attrib_low = 6; // heal %
	UniqueItemList[id].rolls[2].attrib_high = 10;
	id = UITEM_DEATHSPARK;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_5;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 25;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_DMGINCREASE_LIGHTNING;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_MORECRIT_LIGHTNING;
	UniqueItemList[id].rolls[0].attrib_low = 5;
	UniqueItemList[id].rolls[0].attrib_high = 60;
	UniqueItemList[id].rolls[1].attrib_low = 0.5;
	UniqueItemList[id].rolls[1].attrib_high = 1.25;
	id = UITEM_SHELLSHOCK;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_6;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 20;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_PELLET_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_DMGINCREASE_SHOTGUNS;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_FLATPERSHOTGUNOWNED;
	UniqueItemList[id].rolls[0].attrib_low = 0.15;
	UniqueItemList[id].rolls[0].attrib_high = 0.35;
	UniqueItemList[id].rolls[1].attrib_low = 40;
	UniqueItemList[id].rolls[1].attrib_high = 75;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 3;
	id = UITEM_OAKHEART;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UCHRM_7;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 25;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 4;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_DOUBLE_HEALTHCAP;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_FORBID_ARMOR;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_PHYSDAMAGEPER_FLATHEALTH;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_LESSHEALING;
	UniqueItemList[id].rolls[0].attrib_low = 100;
	UniqueItemList[id].rolls[0].attrib_high = 100;
	UniqueItemList[id].rolls[1].attrib_low = 1;
	UniqueItemList[id].rolls[1].attrib_high = 1;
	UniqueItemList[id].rolls[2].attrib_low = 40;
	UniqueItemList[id].rolls[2].attrib_high = 65;
	UniqueItemList[id].rolls[3].attrib_low = 75;
	UniqueItemList[id].rolls[3].attrib_high = 50;
	id = UITEM_PELLETSTORM;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UCHRM_8;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 25;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 4;
	UniqueItemList[id].attrib_id_list[0] = INV_PELLET_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_SLOT3_DAMAGE;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_BEHAVIOR_PELLETSFIRECIRCLE;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_FORSHOW_BURSTGETSPELLETBONUS;
	UniqueItemList[id].rolls[0].attrib_low = 0.5;
	UniqueItemList[id].rolls[0].attrib_high = 1.25;
	UniqueItemList[id].rolls[1].attrib_low = 180;
	UniqueItemList[id].rolls[1].attrib_high = 300;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 1;
	UniqueItemList[id].rolls[3].attrib_low = 1;
	UniqueItemList[id].rolls[3].attrib_high = 1;
	id = UITEM_GRAVECALLER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = IIMG_UCHRM_9;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 30;
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
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 1;
	UniqueItemList[id].rolls[3].attrib_low = 25;
	UniqueItemList[id].rolls[3].attrib_high = 50;
	id = UITEM_ESSENCEEATER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UCHRM_10;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 30;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_SPEED_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_FLATDMG_ALL;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_ONKILL_HEALMISSING;
	UniqueItemList[id].rolls[0].attrib_low = 0.06;
	UniqueItemList[id].rolls[0].attrib_high = 0.175;
	UniqueItemList[id].rolls[1].attrib_low = 3;
	UniqueItemList[id].rolls[1].attrib_high = 12;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 5;
	id = UITEM_EYEBEHOLDER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_11;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 20;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_SOULWEPSPEN;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_PICKUPS_MORESOUL;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_BEHAVIOR_SPELLSFULLDAMAGE;
	UniqueItemList[id].rolls[0].attrib_low = 15;
	UniqueItemList[id].rolls[0].attrib_high = 25;
	UniqueItemList[id].rolls[1].attrib_low = 50;
	UniqueItemList[id].rolls[1].attrib_high = 100;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 1;
	id = UITEM_DEADKINGBANNER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = IIMG_UCHRM_12;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 20;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 5;
	UniqueItemList[id].attrib_id_list[0] = INV_HP_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_ARMOR_INCREASE;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_ALLSTATS;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_ABILITY_RALLY;
	UniqueItemList[id].attrib_id_list[4] = INV_EX_ABILITY_MONSTERSRIP;
	UniqueItemList[id].rolls[0].attrib_low = 80;
	UniqueItemList[id].rolls[0].attrib_high = 160;
	UniqueItemList[id].rolls[1].attrib_low = 100;
	UniqueItemList[id].rolls[1].attrib_high = 200;
	UniqueItemList[id].rolls[2].attrib_low = 4;
	UniqueItemList[id].rolls[2].attrib_high = 15;
	UniqueItemList[id].rolls[3].attrib_low = 1;
	UniqueItemList[id].rolls[3].attrib_high = 10;
	UniqueItemList[id].rolls[4].attrib_low = 1;
	UniqueItemList[id].rolls[4].attrib_high = 1;
	id = UITEM_PAINMASTER;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_13;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 18;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_FLATDOT;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_DOTDURATION;
	UniqueItemList[id].attrib_id_list[2] = INV_CHANCE_AILMENTIGNORE;
	UniqueItemList[id].rolls[0].attrib_low = 8;
	UniqueItemList[id].rolls[0].attrib_high = 16;
	UniqueItemList[id].rolls[1].attrib_low = 50;
	UniqueItemList[id].rolls[1].attrib_high = 100;
	UniqueItemList[id].rolls[2].attrib_low = 30;
	UniqueItemList[id].rolls[2].attrib_high = 50;
	id = UITEM_VOIDEMBLEM;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UCHRM_14;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 20;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_ABILITY_LUCKYCRIT;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_CRITIGNORERESCHANCE;
	UniqueItemList[id].rolls[0].attrib_low = 1;
	UniqueItemList[id].rolls[0].attrib_high = 1;
	UniqueItemList[id].rolls[1].attrib_low = 20;
	UniqueItemList[id].rolls[1].attrib_high = 50;
}

// These are necessary to sync the global variables + unique data
Script "DnD Load Inventory Attributes" OPEN {
	SetupInventoryAttributeTable();
	SetupUniqueItems();
}

Script "DnD Load Inventory Attributes - CS" OPEN CLIENTSIDE {
	SetupInventoryAttributeTable();
	SetupUniqueItems();
}

#endif
