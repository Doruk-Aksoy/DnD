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
		if(id < UNIQUE_CHARM_DROPONLY_BEGIN)
			return StrParam(s:"DND_UNIQUE", d:id + 1);
		return StrParam(s:"DND_DROP_UNIQUE", d:id - UNIQUE_CHARM_DROPONLY_BEGIN + 1);

		case DND_ITEM_SPECIALTY_CYBORG:
		return StrParam(s:"DND_UCORE_NAME", d:id - UNIQUE_POWERCORE_BEGIN + 1);

		case DND_ITEM_BODYARMOR:
		return StrParam(s:"DND_UARM_NAME", d:id - UNIQUE_BODYARMOR_BEGIN + 1);
	}
	return "NULL";
}

str GetUniqueCreatorName(int itype, int id) {
	str res = "";
	switch(itype) {
		case DND_ITEM_CHARM:
			if(id == UITEM_SLAYERSPECIAL)
				return "Lawrence";
	}
	return res;
}

#define START_UNIQUE_DATA(X) id = X
#define UNIQUE_DATA_ENTRY(WEIGHT, WIDTH, HEIGHT, SUBTYPE, LEVEL, ATTRIBS) \
		UniqueItemList[id].weight = weight + WEIGHT; \
		UniqueItemList[id].width = WIDTH; \
		UniqueItemList[id].height = HEIGHT; \
		UniqueItemList[id].item_image = img + IIMG_UCHRM_1; \
		UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS); \
		UniqueItemList[id].item_subtype = SUBTYPE; \
		UniqueItemList[id].item_level = LEVEL; \
		UniqueItemList[id].item_stack = 0; \
		UniqueItemList[id].attrib_count = ATTRIBS; \
		++img; \
		weight += WEIGHT; \
		attr_id = 0

#define UNIQUE_ATTR_ENTRY(MOD, LOW, HIGH, EXTRALOW, EXTRAHIGH) \
		UniqueItemList[id].attrib_id_list[attr_id] = MOD; \
		UniqueItemList[id].rolls[attr_id].attrib_low = LOW; \
		UniqueItemList[id].rolls[attr_id].attrib_high = HIGH; \
		UniqueItemList[id].rolls[attr_id].attrib_extra_low = EXTRALOW; \
		UniqueItemList[id].rolls[attr_id].attrib_extra_high = EXTRAHIGH; \
		++attr_id

#define MAX_UNIQUE_WEIGHT UniqueItemList[UNIQUE_CHARM_REGULARDROP_END].weight

// initializes all uniques
void SetupUniqueItems() {
	// construct unique list to copy from
	int id, weight = 0;
	int img = 0;
	int attr_id = 0;

	START_UNIQUE_DATA(UITEM_ELEMENTALBULWARK);
	UNIQUE_DATA_ENTRY(100, 1, 2, DND_CHARM_MEDIUM, 21, 3);
	UNIQUE_ATTR_ENTRY(INV_ARMORPERCENT_INCREASE, 25, 50, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_DMGREDUCE_ELEM, 10.0, 25.0, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_CHANCE_CASTELEMSPELLONATK, 2, 5, 0, 0);

	START_UNIQUE_DATA(UITEM_IRONBARK);
	UNIQUE_DATA_ENTRY(95, 1, 3, DND_CHARM_LARGE, 18, 5);
	UNIQUE_ATTR_ENTRY(INV_FLATPHYS_DAMAGE, 5, 30, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_STAT_STRENGTH, 10, 25, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_DMGREDUCE_PHYS, 10.0, 25.0, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_REGENCAP_INCREASE, 75, 200, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_KNOCKBACK_IMMUNITY, 1, 1, 0, 0);

	START_UNIQUE_DATA(UITEM_WELLOFPOWER);
	UNIQUE_DATA_ENTRY(30, 1, 3, DND_CHARM_LARGE, 30, 2);
	UNIQUE_ATTR_ENTRY(INV_EX_FACTOR_SMALLCHARM, 1250, 2000, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_LIMITEDSMALLCHARMS, 1, 4, 0, 0);

	START_UNIQUE_DATA(UITEM_ANCIENTGEMSTONE);
	UNIQUE_DATA_ENTRY(50, 1, 1, DND_CHARM_SMALL, 30, 2);
	UNIQUE_ATTR_ENTRY(INV_EX_ALLSTATS, 5, 20, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_CHANCE_HEALMISSINGONPAIN, 6, 10, 3, 8);

	START_UNIQUE_DATA(UITEM_DEATHSPARK);
	UNIQUE_DATA_ENTRY(60, 1, 1, DND_CHARM_SMALL, 25, 2);
	UNIQUE_ATTR_ENTRY(INV_EX_DMGINCREASE_LIGHTNING, 5, 60, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_MORECRIT_LIGHTNING, 0.5, 1.25, 0, 0);

	START_UNIQUE_DATA(UITEM_SHELLSHOCK);
	UNIQUE_DATA_ENTRY(120, 1, 1, DND_CHARM_SMALL, 20, 4);
	UNIQUE_ATTR_ENTRY(INV_PELLET_INCREASE, 0.15, 0.35, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_SHOTGUN_PERCENT, 40, 75, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_FLATPERSHOTGUNOWNED, 1, 3, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_MOREAMMOUSE, 100, 300, 0, 0);

	START_UNIQUE_DATA(UITEM_OAKHEART);
	UNIQUE_DATA_ENTRY(110, 1, 2, DND_CHARM_MEDIUM, 25, 4);
	UNIQUE_ATTR_ENTRY(INV_EX_DOUBLE_HEALTHCAP, 100, 100, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_FORBID_ARMOR, 1, 1, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_PHYSDAMAGEPER_FLATHEALTH, 40, 65, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_LESSHEALING, 75, 50, 0, 0);

	START_UNIQUE_DATA(UITEM_PELLETSTORM);
	UNIQUE_DATA_ENTRY(80, 1, 2, DND_CHARM_MEDIUM, 25, 4);
	UNIQUE_ATTR_ENTRY(INV_PELLET_INCREASE, 0.1, 1.0, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_SHOTGUN_PERCENT, 1, 100, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_BEHAVIOR_PELLETSFIRECIRCLE, 1, 1, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_FORSHOW_BURSTGETSPELLETBONUS, 1, 1, 0, 0);

	START_UNIQUE_DATA(UITEM_GRAVECALLER);
	UNIQUE_DATA_ENTRY(30, 1, 3, DND_CHARM_LARGE, 30, 4);
	UNIQUE_ATTR_ENTRY(INV_STAT_INTELLECT, 15, 25, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_CHANCE_ONDEATH_RAISEZOMBIE, 5, 15, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_DMGREDUCE_SHAREWITHPETS, 1, 1, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_DMGINCREASE_TAKEN, 25, 50, 0, 0);

	START_UNIQUE_DATA(UITEM_ESSENCEEATER);
	UNIQUE_DATA_ENTRY(35, 1, 2, DND_CHARM_MEDIUM, 30, 3);
	UNIQUE_ATTR_ENTRY(INV_SPEED_INCREASE, 0.06, 0.175, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_FLATDMG_ALL, 3, 12, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_ONKILL_HEALMISSING, 1, 5, 0, 0);

	START_UNIQUE_DATA(UITEM_EYEBEHOLDER);
	UNIQUE_DATA_ENTRY(40, 1, 1, DND_CHARM_SMALL, 20, 3);
	UNIQUE_ATTR_ENTRY(INV_EX_SOULWEPSPEN, 15, 25, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_PICKUPS_MORESOUL, 50, 100, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_BEHAVIOR_SPELLSFULLDAMAGE, 1, 1, 0, 0);

	START_UNIQUE_DATA(UITEM_DEADKINGBANNER);
	UNIQUE_DATA_ENTRY(60, 1, 3, DND_CHARM_LARGE, 20, 5);
	UNIQUE_ATTR_ENTRY(INV_HP_INCREASE, 80, 160, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_ARMOR_INCREASE, 100, 200, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_ALLSTATS, 4, 15, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_ABILITY_RALLY, 1, 10, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_ABILITY_MONSTERSRIP, 1, 1, 0, 0);
	
	START_UNIQUE_DATA(UITEM_PAINMASTER);
	UNIQUE_DATA_ENTRY(80, 1, 1, DND_CHARM_SMALL, 20, 3);
	UNIQUE_ATTR_ENTRY(INV_EX_FLATDOT, 8, 16, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_DOTDURATION, 50, 100, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_CHANCE_AILMENTIGNORE, 30, 50, 0, 0);

	START_UNIQUE_DATA(UITEM_VOIDEMBLEM);
	UNIQUE_DATA_ENTRY(30, 1, 2, DND_CHARM_MEDIUM, 20, 2);
	UNIQUE_ATTR_ENTRY(INV_EX_ABILITY_LUCKYCRIT, 1, 1, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_CRITIGNORERESCHANCE, 20, 50, 0, 0);

	START_UNIQUE_DATA(UITEM_REKINDLEDSPARKS);
	UNIQUE_DATA_ENTRY(20, 1, 1, DND_CHARM_SMALL, 45, 2);
	UNIQUE_ATTR_ENTRY(INV_ESS_KRULL, 10, 25, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_SECONDEXPBONUS, 15, 40, 25, 25);

	START_UNIQUE_DATA(UITEM_DEADEYEGLARE);
	UNIQUE_DATA_ENTRY(40, 1, 2, DND_CHARM_MEDIUM, 40, 2);
	UNIQUE_ATTR_ENTRY(INV_ESS_OMNISIGHT2, 15, 36, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_DEADEYEBONUS, 1, 1, 0, 0);

	START_UNIQUE_DATA(UITEM_UNITY);
	UNIQUE_DATA_ENTRY(20, 1, 3, DND_CHARM_LARGE, 50, 4);
	UNIQUE_ATTR_ENTRY(INV_EX_UNITY, 1, 1, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_UNITY_RES_BONUS, 1, 3, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_UNITY_PEN_BONUS, 1, 3, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_UNITY_NOBONUS, 1, 1, 0, 0);

	START_UNIQUE_DATA(UITEM_MINDFORGE);
	UNIQUE_DATA_ENTRY(20, 1, 2, DND_CHARM_MEDIUM, 35, 2);
	UNIQUE_ATTR_ENTRY(INV_STAT_INTELLECT, 10, 40, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_INTBONUSTOMELEE, 1.5, 2.5, 0, 0);

	START_UNIQUE_DATA(UITEM_SLAYERSPECIAL);
	UNIQUE_DATA_ENTRY(25, 1, 1, DND_CHARM_SMALL, 18, 3);
	UNIQUE_ATTR_ENTRY(INV_EX_REFILLAMMOONMELEEKILL, 5, 10, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_SWAPFROMMELEECRIT, 0.35, 1.0, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_ALLSTATS, 1, 5, 0, 0);

	START_UNIQUE_DATA(UITEM_DRAGONFANG);
	UNIQUE_DATA_ENTRY(25, 1, 2, DND_CHARM_MEDIUM, 36, 4);
	UNIQUE_ATTR_ENTRY(INV_DMGREDUCE_ALL, 5.0, 10.0, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_FLAT_AUTOMATIC, 1, 10, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_RIPPERSRIPALL, 1, 1, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_RIPPERSONETIMEONLY, 1, 1, 0, 0);

	START_UNIQUE_DATA(UITEM_MIRROROFETERNITY);
	UNIQUE_DATA_ENTRY(1, 1, 2, DND_CHARM_MEDIUM, 40, 1);
	UNIQUE_ATTR_ENTRY(INV_EX_MIRROROTHERMEDIUM, 1, 1, 0, 0);
	
	START_UNIQUE_DATA(UITEM_KINGMAKER);
	UNIQUE_DATA_ENTRY(35, 1, 2, DND_CHARM_MEDIUM, 30, 3);
	UNIQUE_ATTR_ENTRY(INV_EX_CHANCEGAINXCHARGE, 1, 12, DND_CHARGE_FRENZY, DND_CHARGE_POWER);
	UNIQUE_ATTR_ENTRY(INV_EX_MOREDAMAGEPERCHARGE, 1, 5, 0, 0);
	UNIQUE_ATTR_ENTRY(INV_EX_CHARGEDURATIONHALVED, 1, 1, 0, 0);

	// ADD GENERAL DROP POOL UNIQUE CHARMS ABOVE
	// DROP ONLY UNIQUE CHARMS
	// Note: Maybe create new macros for these... only the ones above are factored into overall drop pool and weighting
	id = UITEM_ELEMENTALHARMONY;
	UniqueItemList[id].weight = -1;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UDCHRM_1;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 33;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 3;
	UniqueItemList[id].attrib_id_list[0] = INV_ARMORPERCENT_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_ELEPENHARMONY;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_CHANCE_CASTELEMSPELLONATK;
	UniqueItemList[id].rolls[0].attrib_low = 25;
	UniqueItemList[id].rolls[0].attrib_high = 50;
	UniqueItemList[id].rolls[1].attrib_low = 1;
	UniqueItemList[id].rolls[1].attrib_high = 1;
	UniqueItemList[id].rolls[2].attrib_low = 2;
	UniqueItemList[id].rolls[2].attrib_high = 5;
	id = UITEM_STEELBARK;
	UniqueItemList[id].weight = -1;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 3;
	UniqueItemList[id].item_image = IIMG_UDCHRM_2;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_LARGE;
	UniqueItemList[id].item_level = 36;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 5;
	UniqueItemList[id].attrib_id_list[0] = INV_FLATPHYS_DAMAGE;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_STREXTRABONUSTOMELEE;
	UniqueItemList[id].attrib_id_list[2] = INV_DMGREDUCE_PHYS;
	UniqueItemList[id].attrib_id_list[3] = INV_REGENCAP_INCREASE;
	UniqueItemList[id].attrib_id_list[4] = INV_EX_KNOCKBACK_IMMUNITY;
	UniqueItemList[id].rolls[0].attrib_low = 5;
	UniqueItemList[id].rolls[0].attrib_high = 30;
	UniqueItemList[id].rolls[1].attrib_low = 0.005;
	UniqueItemList[id].rolls[1].attrib_high = 0.015;
	UniqueItemList[id].rolls[2].attrib_low = 10.0;
	UniqueItemList[id].rolls[2].attrib_high = 25.0;
	UniqueItemList[id].rolls[3].attrib_low = 75;
	UniqueItemList[id].rolls[3].attrib_high = 200;
	UniqueItemList[id].rolls[4].attrib_low = 1;
	UniqueItemList[id].rolls[4].attrib_high = 1;
	id = UITEM_DREAMINGGODIRE;
	UniqueItemList[id].weight = -1;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UDCHRM_3;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_SMALL;
	UniqueItemList[id].item_level = 40;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 4;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_SOULWEPSPEN;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_SOULPICKUPSINFAMMO;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_SOULPICKUPSONLYAMMO;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_BEHAVIOR_SPELLSFULLDAMAGE;
	UniqueItemList[id].rolls[0].attrib_low = 15;
	UniqueItemList[id].rolls[0].attrib_high = 25;
	UniqueItemList[id].rolls[1].attrib_low = 3;
	UniqueItemList[id].rolls[1].attrib_high = 8;
	UniqueItemList[id].rolls[2].attrib_low = 8;
	UniqueItemList[id].rolls[2].attrib_high = 15;
	UniqueItemList[id].rolls[3].attrib_low = 1;
	UniqueItemList[id].rolls[3].attrib_high = 1;
	id = UITEM_HEATBREAKER;
	UniqueItemList[id].weight = -1;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UDCHRM_4;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 38;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 4;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_CANFIREOVERHEATED;
	UniqueItemList[id].attrib_id_list[1] = INV_EX_LESSDMGTAKENMAXOVERHEAT;
	UniqueItemList[id].attrib_id_list[2] = INV_EX_CANTFIRENONOVERHEAT;
	UniqueItemList[id].attrib_id_list[3] = INV_IMP_FASTEROVERHEATDISS;
	UniqueItemList[id].rolls[0].attrib_low = 1;
	UniqueItemList[id].rolls[0].attrib_high = 1;
	UniqueItemList[id].rolls[1].attrib_low = 5;
	UniqueItemList[id].rolls[1].attrib_high = 10;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 1;
	UniqueItemList[id].rolls[3].attrib_low = -300;
	UniqueItemList[id].rolls[3].attrib_high = -200;
	id = UITEM_THORNVEIN;
	UniqueItemList[id].weight = -1;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UDCHRM_5;
	UniqueItemList[id].item_type = DND_ITEM_CHARM | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = DND_CHARM_MEDIUM;
	UniqueItemList[id].item_level = 40;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 5;
	UniqueItemList[id].attrib_id_list[0] = INV_EX_WEAPONSUSEHEALTH;
	UniqueItemList[id].attrib_id_list[1] = INV_LIFESTEAL_RECOVERY;
	UniqueItemList[id].attrib_id_list[2] = INV_LIFESTEAL_CAP;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_CANNOTPICKAMMO;
	UniqueItemList[id].attrib_id_list[4] = INV_EX_AMMOCOSTMULTIPLIER;
	UniqueItemList[id].rolls[0].attrib_low = 1;
	UniqueItemList[id].rolls[0].attrib_high = 1;
	UniqueItemList[id].rolls[1].attrib_low = 50;
	UniqueItemList[id].rolls[1].attrib_high = 250;
	UniqueItemList[id].rolls[2].attrib_low = 25;
	UniqueItemList[id].rolls[2].attrib_high = 75;
	UniqueItemList[id].rolls[3].attrib_low = 1;
	UniqueItemList[id].rolls[3].attrib_high = 1;
	UniqueItemList[id].rolls[4].attrib_low = 3;
	UniqueItemList[id].rolls[4].attrib_high = 10;

	// powercores
	id = UITEM_TESSERACT;
	UniqueItemList[id].weight = 1000;
	UniqueItemList[id].width = 1;
	UniqueItemList[id].height = 1;
	UniqueItemList[id].item_image = IIMG_UCORE_1;
	UniqueItemList[id].item_type = DND_ITEM_SPECIALTY_CYBORG | ((id + 1) << UNIQUE_BITS);
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
	UniqueItemList[id].weight = 500;
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
	UniqueItemList[id].rolls[1].attrib_low = 75;
	UniqueItemList[id].rolls[1].attrib_high = 125;
	UniqueItemList[id].rolls[2].attrib_low = 1;
	UniqueItemList[id].rolls[2].attrib_high = 1;
	UniqueItemList[id].rolls[3].attrib_low = 1;
	UniqueItemList[id].rolls[3].attrib_high = 1;
	id = UITEM_PRISMGUARD;
	UniqueItemList[id].weight = 750;
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
	id = UITEM_BARONSDEN;
	UniqueItemList[id].weight = 1000;
	UniqueItemList[id].width = 2;
	UniqueItemList[id].height = 2;
	UniqueItemList[id].item_image = IIMG_UBODY_3;
	UniqueItemList[id].item_type = DND_ITEM_BODYARMOR | ((id + 1) << UNIQUE_BITS);
	UniqueItemList[id].item_subtype = BODYARMOR_BLUE;
	UniqueItemList[id].item_level = 58;
	UniqueItemList[id].item_stack = 0;
	UniqueItemList[id].attrib_count = 5;
	UniqueItemList[id].attrib_id_list[0] = INV_ARMORPERCENT_INCREASE;
	UniqueItemList[id].attrib_id_list[1] = INV_HP_INCREASE;
	UniqueItemList[id].attrib_id_list[2] = INV_DMGREDUCE_PHYS;
	UniqueItemList[id].attrib_id_list[3] = INV_EX_DAMAGELOWESTTAKENASPHYS;
	UniqueItemList[id].attrib_id_list[4] = INV_EX_DEMONBARRIERS;
	UniqueItemList[id].rolls[0].attrib_low = 30;
	UniqueItemList[id].rolls[0].attrib_high = 75;
	UniqueItemList[id].rolls[1].attrib_low = 60;
	UniqueItemList[id].rolls[1].attrib_high = 125;
	UniqueItemList[id].rolls[2].attrib_low = 15.0;
	UniqueItemList[id].rolls[2].attrib_high = 35.0;
	UniqueItemList[id].rolls[3].attrib_low = 10;
	UniqueItemList[id].rolls[3].attrib_high = 33;
	UniqueItemList[id].rolls[4].attrib_low = 1;
	UniqueItemList[id].rolls[4].attrib_high = 1;

}

// These are necessary to sync the global variables + unique data
Script "DnD Load Inventory Attributes" OPEN {
	if(!isSetupComplete(SETUP_STATE1, SETUP_ITEMTABLES)) {
		SetupArmorDropWeights();
		Delay(const:10);
		SetupInventoryAttributeTable();
		Delay(const:10);
		SetupInventoryTagGroups();
		Delay(const:10);
		SetupUniqueItems();
		Delay(10);
		ACS_NamedExecuteAlways("DnD Setup Menu Vars", 0); // leave this last here
	}
}

Script "DnD Load Inventory Attributes - CS" OPEN CLIENTSIDE {
	if(!isSetupComplete(SETUP_STATE1, SETUP_ITEMTABLES)) {
		SetupArmorDropWeights();
		Delay(const:10);
		SetupInventoryAttributeTable();
		Delay(const:10);
		SetupUniqueItems();
		Delay(10);
		ACS_NamedExecuteAlways("DnD Setup Menu Vars - CS", 0);
	}
}

#endif
