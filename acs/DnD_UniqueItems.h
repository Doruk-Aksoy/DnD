#ifndef DND_UNIQUE_IN
#define DND_UNIQUE_IN

#define UNIQUE_DROPCHANCE 0.05
#define UNIQUE_ARMOR_DROPCHANCE 0.0275
#define UNIQUE_BITS 16
#define UNIQUE_BEGIN 65535

#define REKINDLE_REDUCE 33

#define DND_DEADEYE_BONUS 1
#define DND_DEADEYE_BONUSF 0.01
#define DND_DEADEYE_PLUSPER 20
#define DND_DEADEYE_MINUSPER 45

#define DND_RES_PER_PRISMGUARD 0.5

str GetUniqueItemName(int itype, int id) {
	switch(itype) {
		case DND_ITEM_CHARM:
		return StrParam(s:"DND_UNIQUE", d:id + 1);

		case DND_ITEM_POWERCORE:
		return StrParam(s:"DND_UCORE_NAME", d:id - UNIQUE_POWERCORE_BEGIN + 1);

		case DND_ITEM_BODYARMOR:
		return StrParam(s:"DND_UARM_NAME", d:id - UNIQUE_BODYARMOR_BEGIN + 1);
	}
	return "NULL";
}

#define MAX_UNIQUE_WEIGHT 1000

// initializes all uniques
void SetupUniqueItems() {
	// construct unique list to copy from
	int id = UITEM_ELEMENTALBULWARK;
	UniqueItemList[id].weight = 125;
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
	UniqueItemList[id].weight = 210;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = IIMG_UCHRM_2;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 18;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 5;
	UniqueItemList[id].attrib_id_list[0] = INV_FLATPHYS_DAMAGE;
	UniqueItemList[id].attrib_id_list[1] = INV_STAT_STRENGTH;
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
	UniqueItemList[id].weight = 250;
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
	UniqueItemList[id].weight = 300;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_4;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 30;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_ALLSTATS;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_CHANCE_HEALMISSINGONPAIN;
	UniqueItemList[id].rolls[0].attrib_low = 5;
	UniqueItemList[id].rolls[0].attrib_high = 20;
	UniqueItemList[id].rolls[1].attrib_extra_low = 3; // chance to proc
	UniqueItemList[id].rolls[1].attrib_extra_high = 8;
	UniqueItemList[id].rolls[1].attrib_low = 6; // heal %
	UniqueItemList[id].rolls[1].attrib_high = 10;
	id = UITEM_DEATHSPARK;
	UniqueItemList[id].weight = 360;
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
	UniqueItemList[id].weight = 480;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_6;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 20;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_PELLET_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_SHOTGUN_PERCENT;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_FLATPERSHOTGUNOWNED;
	UniqueItemList[id].rolls[0].attrib_low = 0.15;
	UniqueItemList[id].rolls[0].attrib_high = 0.35;
	UniqueItemList[id].rolls[1].attrib_low = 40;
	UniqueItemList[id].rolls[1].attrib_high = 75;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 3;
	id = UITEM_OAKHEART;
	UniqueItemList[id].weight = 570;
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
	UniqueItemList[id].weight = 650;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UCHRM_8;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 25;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 4;
	UniqueItemList[id].attrib_id_list[0] = INV_PELLET_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_SHOTGUN_PERCENT;
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
	UniqueItemList[id].weight = 680;
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
	UniqueItemList[id].weight = 715;
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
	UniqueItemList[id].weight = 740;
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
	UniqueItemList[id].weight = 770;
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
	UniqueItemList[id].weight = 820;
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
	UniqueItemList[id].weight = 850;
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
	id = UITEM_REKINDLEDSPARKS;
	UniqueItemList[id].weight = 870;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_15;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 45;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_ESS_KRULL;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_SECONDEXPBONUS;
	UniqueItemList[id].rolls[0].attrib_low = 10;
	UniqueItemList[id].rolls[0].attrib_high = 25;
	UniqueItemList[id].rolls[1].attrib_low = 15;
	UniqueItemList[id].rolls[1].attrib_high = 40;
	id = UITEM_DEADEYEGLARE;
	UniqueItemList[id].weight = 910;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UCHRM_16;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 40;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_ESS_OMNISIGHT2;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_DEADEYEBONUS;
	UniqueItemList[id].rolls[0].attrib_low = 15;
	UniqueItemList[id].rolls[0].attrib_high = 36;
	UniqueItemList[id].rolls[1].attrib_low = 1;
	UniqueItemList[id].rolls[1].attrib_high = 1;
	id = UITEM_UNITY;
	UniqueItemList[id].weight = 930;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = IIMG_UCHRM_17;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 50;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 4;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_UNITY;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_UNITY_RES_BONUS;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_UNITY_PEN_BONUS;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_UNITY_NOBONUS;
	UniqueItemList[id].rolls[0].attrib_low = 1;
	UniqueItemList[id].rolls[0].attrib_high = 1;
	UniqueItemList[id].rolls[1].attrib_low = 1;
	UniqueItemList[id].rolls[1].attrib_high = 2;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 2;
	UniqueItemList[id].rolls[3].attrib_low = 1;
	UniqueItemList[id].rolls[3].attrib_high = 1;
	id = UITEM_MINDFORGE;
	UniqueItemList[id].weight = 970;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UCHRM_18;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 35;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 2;
	UniqueItemList[id].attrib_id_list[0] = INV_STAT_INTELLECT;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_INTBONUSTOMELEE;
	UniqueItemList[id].rolls[0].attrib_low = 10;
	UniqueItemList[id].rolls[0].attrib_high = 40;
	UniqueItemList[id].rolls[1].attrib_low = 1.5;
	UniqueItemList[id].rolls[1].attrib_high = 2.5;
	id = UITEM_SLAYERSPECIAL;
	UniqueItemList[id].weight = 1000;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCHRM_19;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 18;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_REFILLAMMOONMELEEKILL;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_SWAPFROMMELEECRIT;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_ALLSTATS;
	UniqueItemList[id].rolls[0].attrib_low = 5;
	UniqueItemList[id].rolls[0].attrib_high = 10;
	UniqueItemList[id].rolls[1].attrib_low = 0.35;
	UniqueItemList[id].rolls[1].attrib_high = 1.0;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 5;

	// powercores
	id = UITEM_TESSERACT;
	UniqueItemList[id].weight = 1000;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCORE_1;
	UniqueItemList[id].item_type = DND_ITEM_POWERCORE | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = PCORE_UNSTABLE;
	UniqueItemList[id].item_level = 48;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 4;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_STARTESONDEPLETE;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_ESCHARGE_DMGNOINTERRUPT;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_ESEXPLOSIONHPDMG;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_ESCHARGE_USEHP;
	UniqueItemList[id].rolls[0].attrib_low = 5;
	UniqueItemList[id].rolls[0].attrib_high = 25;
	UniqueItemList[id].rolls[1].attrib_low = 1;
	UniqueItemList[id].rolls[1].attrib_high = 1;
	UniqueItemList[id].rolls[2].attrib_low = 5;
	UniqueItemList[id].rolls[2].attrib_high = 10;
	UniqueItemList[id].rolls[3].attrib_low = 20;
	UniqueItemList[id].rolls[3].attrib_high = 50;

	// body armors
	id = UITEM_PROTOSHELL;
	UniqueItemList[id].weight = 600;
	UniqueItemList[id].width = 2;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UBODY_1;
	UniqueItemList[id].item_type = DND_ITEM_BODYARMOR | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = BODYARMOR_CYBER;
	UniqueItemList[id].item_level = 50;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 4;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_HPTOESHIELD;
	UniqueItemList[id].attrib_id_list[1] = INV_SHIELD_RECOVERYRATE;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_ESHIELDFULLABSORB;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_HEALTHATONE;
	UniqueItemList[id].rolls[0].attrib_low = 15;
	UniqueItemList[id].rolls[0].attrib_high = 30;
	UniqueItemList[id].rolls[1].attrib_low = 100;
	UniqueItemList[id].rolls[1].attrib_high = 100;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 1;
	UniqueItemList[id].rolls[3].attrib_low = 1;
	UniqueItemList[id].rolls[3].attrib_high = 1;
	id = UITEM_PRISMGUARD;
	UniqueItemList[id].weight = 1000;
	UniqueItemList[id].width = 2;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UBODY_2;
	UniqueItemList[id].item_type = DND_ITEM_BODYARMOR | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = BODYARMOR_MONOLITH;
	UniqueItemList[id].item_level = 56;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 4;
	UniqueItemList[id].attrib_id_list[0] = INV_KNOCKBACK_RESIST;
	UniqueItemList[id].attrib_id_list[1] = INV_HP_INCREASE;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_RESPERESHIELD;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_ESHIELDONLYBLOCKPCT;
	UniqueItemList[id].rolls[0].attrib_low = 1000;
	UniqueItemList[id].rolls[0].attrib_high = 2000;
	UniqueItemList[id].rolls[1].attrib_low = 30;
	UniqueItemList[id].rolls[1].attrib_high = 100;
	UniqueItemList[id].rolls[2].attrib_low = 10;
	UniqueItemList[id].rolls[2].attrib_high = 20;
	UniqueItemList[id].rolls[3].attrib_low = 20;
	UniqueItemList[id].rolls[3].attrib_high = 33;
}

// These are necessary to sync the global variables + unique data
Script "DnD Load Inventory Attributes" OPEN {
	if(!isSetupComplete(SETUP_STATE1, SETUP_ITEMTABLES)) {
		SetupInventoryAttributeTable();
		Delay(const:2);
		SetupInventoryTagGroups();
		Delay(const:2);
		SetupUniqueItems();
		SetupComplete(SETUP_STATE1, SETUP_ITEMTABLES);
	}
}

Script "DnD Load Inventory Attributes - CS" OPEN CLIENTSIDE {
	if(!isSetupComplete(SETUP_STATE1, SETUP_ITEMTABLES)) {
		SetupInventoryAttributeTable();
		Delay(const:2);
		SetupUniqueItems();
		SetupComplete(SETUP_STATE1, SETUP_ITEMTABLES);
	}
}

#endif
