#ifndef DND_MERCHANT_IN
#define DND_MERCHANT_IN

#define DND_MIN_MERCHANTITEMS 3
#define DND_MAX_MERCHANTITEMS 5
#define DND_MERCHANT_LIMITITEMS 10
#define DND_MERCHANT_ITEMSPERLVL 20

#define DND_MERCHANT_ITEMCHANCE 0.4
#define DND_MERCHANT_ORBCHANCE 0.9
#define DND_MERCHANT_TOKENCHANCE 1.0

#define DND_MERCHANT_ARMORCHANCE 0.175
#define DND_MERCHANT_HELMCHANCE 0.375
#define DND_MERCHANT_BOOTCHANCE 0.575
#define DND_MERCHANT_SPECIALTYCHANCE 0.675
#define DND_MERCHANT_CHARMCHANCE 1.0

#define DND_BASE_MERCHANT_STACKABLECOST 600

#define DND_BASE_MERCHANT_ITEMCOST 2000
#define DND_MERCHANT_LEVEL_PERCENT 7
#define DND_MERCHANT_IMPLICIT_PERCENT 20
#define DND_MERCHANT_TIER_PERCENT 10
#define DND_MERCHANT_FRACTURE_PERCENT 50
#define DND_MERCHANT_UNIQUEFACTOR 30 // 30% more cost if its unique

#define DND_MERCHANT_MEDIUMTIERORB_FACTOR 3
#define DND_MERCHANT_HIGHTIERORB_FACTOR 6
#define DND_MERCHANT_STACKABLE_ILVLCONTRIB 20
#define DND_MERCHANT_STACKABLE_ILVLFACTOR 10 // 10% per ilvl

#define DND_MERCHANT_QUALITYCHANCE 0.25
#define DND_MERCHANT_LOWQUALITY 0.6
#define DND_MERCHANT_MEDQUALITY 0.9
#define DND_MERCHANT_HIGHQUALITY 1.0

#define DND_MERCHANT_ITEMCORRUPTCHANCE 0.1 // 10% corrupted
#define DND_MERCHANT_ITEMFRACTURECHANCE 0.025 // 2.5% to offer fractured

#define DND_MERCHANT_SYNERGYITEM_CHANCE 0.1 // 10% chance for an item to have really synergistic mods (mods of same mod pool)

#define DND_MERCHANT_UNIQUECHANCE 0.1 // 10% to offer uniques

void ConstructUniqueOnMerchant(int item_pos, int unique_id) {
	TradeViewList[MAXPLAYERS][item_pos].width = UniqueItemList[unique_id].width;
	TradeViewList[MAXPLAYERS][item_pos].height = UniqueItemList[unique_id].height;
	TradeViewList[MAXPLAYERS][item_pos].item_type = UniqueItemList[unique_id].item_type;
	TradeViewList[MAXPLAYERS][item_pos].item_image = UniqueItemList[unique_id].item_image;
	TradeViewList[MAXPLAYERS][item_pos].item_subtype = UniqueItemList[unique_id].item_subtype;
	TradeViewList[MAXPLAYERS][item_pos].item_level = UniqueItemList[unique_id].item_level;
	TradeViewList[MAXPLAYERS][item_pos].item_stack = UniqueItemList[unique_id].item_stack;
	TradeViewList[MAXPLAYERS][item_pos].attrib_count = UniqueItemList[unique_id].attrib_count;

	// this can set images sometimes, so just moved item_image below here
	SetupItemImplicit(item_pos, TradeViewList[MAXPLAYERS][item_pos].item_type & 0xFFFF, TradeViewList[MAXPLAYERS][item_pos].item_subtype, TradeViewList[MAXPLAYERS][item_pos].item_level, true);

	TradeViewList[MAXPLAYERS][item_pos].corrupted = 0;
	TradeViewList[MAXPLAYERS][item_pos].quality = 0;

	for(int i = 0; i < TradeViewList[MAXPLAYERS][item_pos].attrib_count; ++i) {
		TradeViewList[MAXPLAYERS][item_pos].attributes[i].attrib_id = UniqueItemList[unique_id].attrib_id_list[i];
		TradeViewList[MAXPLAYERS][item_pos].attributes[i].attrib_tier = 0;
		
		TradeViewList[MAXPLAYERS][item_pos].attributes[i].attrib_val = random(UniqueItemList[unique_id].rolls[i].attrib_low, UniqueItemList[unique_id].rolls[i].attrib_high);
		TradeViewList[MAXPLAYERS][item_pos].attributes[i].attrib_extra = random(UniqueItemList[unique_id].rolls[i].attrib_extra_low, UniqueItemList[unique_id].rolls[i].attrib_extra_high);
	}
}

int MakeMerchantItemUnique(int item_pos) {
	int i, beg, end, w = 0;

	switch(TradeViewList[MAXPLAYERS][item_pos].item_type) {
		case DND_ITEM_CHARM:
			beg = UNIQUE_CHARM_BEGIN;
			end = UNIQUE_CHARM_REGULARDROP_END; // rolonly until the regular drop
			w = MAX_UNIQUE_WEIGHT;
		break;
		case DND_ITEM_SPECIALTY_CYBORG:
			beg = UNIQUE_POWERCORE_BEGIN;
			end = UNIQUE_POWERCORE_END;
			w = MAX_UNIQUE_PCORE_WEIGHT;
		break;
		case DND_ITEM_BODYARMOR:
			beg = UNIQUE_BODYARMOR_BEGIN;
			end = UNIQUE_BODYARMOR_END;
			w = MAX_UNIQUE_BODYARMOR_WEIGHT;
		break;
		case DND_ITEM_BOOT:
			beg = UNIQUE_BOOT_BEGIN;
			end = UNIQUE_BOOT_END;
			w = MAX_UNIQUE_BOOT_WEIGHT;
		break;
		default:
			beg = UNIQUE_CHARM_BEGIN;
			end = UNIQUE_CHARM_END;
			w = MAX_UNIQUE_WEIGHT;
		break;
	}

	int roll = random(1, w);
	for(i = beg; i <= end && roll > UniqueItemList[i].weight; ++i);

	// i is the unique id
	ConstructUniqueOnMerchant(item_pos, i);
	return i;
}

void AddAttributeToMerchant(int item_pos, int attrib, int max_affixes = 0) {
	if(!max_affixes)
		max_affixes = GetMaxItemAffixes(TradeViewList[MAXPLAYERS][item_pos].item_type, TradeViewList[MAXPLAYERS][item_pos].item_subtype);
	if(TradeViewList[MAXPLAYERS][item_pos].attrib_count < max_affixes) {
		int temp = TradeViewList[MAXPLAYERS][item_pos].attrib_count++;
		int lvl = TradeViewList[MAXPLAYERS][item_pos].item_level / CHARM_ATTRIBLEVEL_SEPERATOR;
		
		bool makeWellRolled = CheckWellRolled(MAXPLAYERS);
		
		lvl = GetItemTierRoll(lvl, makeWellRolled);

		TradeViewList[MAXPLAYERS][item_pos].attributes[temp].attrib_tier = lvl;
		TradeViewList[MAXPLAYERS][item_pos].attributes[temp].attrib_id = attrib;
		TradeViewList[MAXPLAYERS][item_pos].attributes[temp].fractured = random(0, 1.0) <= DND_MERCHANT_ITEMFRACTURECHANCE;

		// it basically adds the step value (val) and a +1 if we aren't 0, so our range is ex: 5-10 in tier 1 then 11-15 in tier 2 assuming +5 range per tier
		// luck adds a small chance for a charm to have well rolled modifier on it -- luck gain is 0.15, 0.05 x 10 = 0.5 max rank thats 50% chance for well rolled mods
		TradeViewList[MAXPLAYERS][item_pos].attributes[temp].attrib_val = RollAttributeValue(
			attrib, 
			lvl, 
			makeWellRolled,
			TradeViewList[MAXPLAYERS][item_pos].item_type,
			TradeViewList[MAXPLAYERS][item_pos].item_subtype
		);

		max_affixes = GetExtraForMod(MAXPLAYERS, attrib, lvl, TradeViewList[MAXPLAYERS][item_pos].item_type, TradeViewList[MAXPLAYERS][item_pos].item_subtype, makeWellRolled);
		if(max_affixes != -1)
			TradeViewList[MAXPLAYERS][item_pos].attributes[temp].attrib_extra = max_affixes;
	}
	CheckAttribEffects(MAXPLAYERS, item_pos, attrib, DND_SYNC_ITEMSOURCE_TRADEVIEW);
}

int CheckMerchantItemSynergy(int synergy_roll, int item_pos) {
	static int tags_found[MAX_ATTRIB_TAG_GROUPS];

	if(synergy_roll == -2 && random(0, 1.0) <= DND_MERCHANT_SYNERGYITEM_CHANCE) {
		// pick one of the random mods on the existing item to be the target mod tag to go after
		synergy_roll = random(0, TradeViewList[MAXPLAYERS][item_pos].attrib_count - 1);

		// pick a random tag if its got multiple then give it +1, as it expects +1 of it
		synergy_roll = TradeViewList[MAXPLAYERS][item_pos].attributes[synergy_roll].attrib_id;
		synergy_roll = ItemModTable[synergy_roll].tags;

		int i, j, temp;
		if(synergy_roll & (synergy_roll - 1)) {
			// not power of two so its got multiple tags in it, process it to pick a random one
			for(i = 0; i < MAX_ATTRIB_TAG_GROUPS; ++i)
				tags_found[i] = 0;

			i = 0;			// holds tags found
			j = 0;			// counter
			while(synergy_roll) {
				if(synergy_roll & 1)
					tags_found[i++] = j + 1; // we store +1 here already
				synergy_roll >>= 1;
				++j;
			}

			// pick random bit from the obtained total
			synergy_roll = tags_found[random(0, i - 1)];
		}
		else {
			j = 0;
			while(synergy_roll) {
				synergy_roll >>= 1;
				++j;
			}
			synergy_roll = j;
		}
	}
	//printbold(s:"item ", d:item_pos, s:" picked synergy id ", d:synergy_roll - 1);
	return synergy_roll;
}

void RollArmorInfoOnMerchant(int item_pos, int item_tier, int item_type, int armor_type, int max_attr) {
	// only for rolling body armors we access the array for item_tier, as that can be changed in ConstructArmorDataOnField to level this down for lower level players
	int i = 0, roll;
	int count = random(1, max_attr);
	int special_roll = SetupItemImplicit(item_pos, item_type, armor_type, item_tier, true);
	int synergy_roll = -2;
	int max_tries = 10;

	while(i < count) {
		do {
			roll = PickRandomAttribute(item_type, armor_type, special_roll, TradeViewList[MAXPLAYERS][item_pos].implicit[0].attrib_id, synergy_roll);
			if(max_tries-- < 0)
				synergy_roll = -2;
		} while(CheckItemAttribute(MAXPLAYERS, item_pos, roll, DND_SYNC_ITEMSOURCE_TRADEVIEW, count) != -1);
		AddAttributeToMerchant(item_pos, roll, count);

		synergy_roll = CheckMerchantItemSynergy(synergy_roll, item_pos);

		++i;
	}
}

void ConstructArmorDataOnMerchant(int item_pos, int ilvl) {
    // decide what type of armor to spawn here -- droppers have tiers not equal to zero, so they can determine some easy armors to drop
	int res, i;

    // pick armor type here

    // pick with some weight here -- <= here for index, size is that +1
    res = random(1, 100);
    for(i = 0; i <= BODYARMORS_REGULAREND; ++i) {
        if(res <= ArmorDropWeights[DND_DROPPEDARMOR_BODY][i]) {
            if(i == BODYARMOR_TANGLEDRIBCAGE && ilvl < ARMOR_TANGLED_DROPLVL) {
                // repeat if this happened
                res = random(1, 100);
                i = -1;
                continue;
            }
            res = i;
            break;
        }
    }

	TradeViewList[MAXPLAYERS][item_pos].item_level = ilvl;
	TradeViewList[MAXPLAYERS][item_pos].item_stack = 0;
	TradeViewList[MAXPLAYERS][item_pos].item_type = DND_ITEM_BODYARMOR;
	TradeViewList[MAXPLAYERS][item_pos].item_subtype = res;
	TradeViewList[MAXPLAYERS][item_pos].width = DND_BODYARMOR_BASEWIDTH;
	TradeViewList[MAXPLAYERS][item_pos].height = DND_BODYARMOR_BASEHEIGHT;

	TradeViewList[MAXPLAYERS][item_pos].corrupted = false;
	TradeViewList[MAXPLAYERS][item_pos].quality = 0;

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_id = -1;
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_val = 0;
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_tier = 0;
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_extra = 0;
	}
	
	TradeViewList[MAXPLAYERS][item_pos].attrib_count = 0;
	for(i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		TradeViewList[MAXPLAYERS][item_pos].attributes[i].attrib_id = -1;

    TradeViewList[MAXPLAYERS][item_pos].item_image = IIMG_ARM_1 + res;

	if(random(0, 1.0) <= DND_MERCHANT_UNIQUECHANCE)
		MakeMerchantItemUnique(item_pos);
	else
    	RollArmorInfoOnMerchant(item_pos, ilvl, DND_ITEM_BODYARMOR, res, MAX_ARMOR_ATTRIB_DEFAULT);
}

void ConstructBootDataOnMerchant(int item_pos, int ilvl) {
    // decide what type of armor to spawn here -- droppers have tiers not equal to zero, so they can determine some easy armors to drop
	int i;
	int res = random(1, MAX_BOOT_WEIGHT);
	for(i = 0; i <= BOOTS_END; ++i)
		if(res <= ArmorDropWeights[DND_DROPPEDARMOR_BOOT][i]) {
			res = i;
			break;
		}

	TradeViewList[MAXPLAYERS][item_pos].item_level = ilvl;
	TradeViewList[MAXPLAYERS][item_pos].item_stack = 0;
	TradeViewList[MAXPLAYERS][item_pos].item_type = DND_ITEM_BOOT;
	TradeViewList[MAXPLAYERS][item_pos].item_subtype = res;
	TradeViewList[MAXPLAYERS][item_pos].width = DND_BOOT_BASEWIDTH;
	TradeViewList[MAXPLAYERS][item_pos].height = DND_BOOT_BASEHEIGHT;

	TradeViewList[MAXPLAYERS][item_pos].corrupted = false;
	TradeViewList[MAXPLAYERS][item_pos].quality = 0;

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_id = -1;
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_val = 0;
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_tier = 0;
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_extra = 0;
	}
	
	TradeViewList[MAXPLAYERS][item_pos].attrib_count = 0;
	for(i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		TradeViewList[MAXPLAYERS][item_pos].attributes[i].attrib_id = -1;

	TradeViewList[MAXPLAYERS][item_pos].item_image = IIMG_BOO_1 + res;

	if(random(0, 1.0) <= DND_MERCHANT_UNIQUECHANCE)
		MakeMerchantItemUnique(item_pos);
	else
    	RollArmorInfoOnMerchant(item_pos, ilvl, DND_ITEM_BOOT, res, MAX_BOOT_ATTRIB_DEFAULT);
}

void ConstructHelmDataOnMerchant(int item_pos, int ilvl, int helm = -1) {
    // decide what type of armor to spawn here -- droppers have tiers not equal to zero, so they can determine some easy armors to drop
	int i;
	int res = 0;
	if(helm == -1) {
		res = random(1, MAX_HELM_WEIGHT);
		for(i = 0; i <= HELMS_END; ++i) {
			if(res <= ArmorDropWeights[DND_DROPPEDARMOR_HELM][i]) {
				res = i;
				break;
			}
		}
	}
	else
		res = helm;

	TradeViewList[MAXPLAYERS][item_pos].item_level = ilvl;
	TradeViewList[MAXPLAYERS][item_pos].item_stack = 0;
	TradeViewList[MAXPLAYERS][item_pos].item_type = DND_ITEM_HELM;
	TradeViewList[MAXPLAYERS][item_pos].item_subtype = res;
	TradeViewList[MAXPLAYERS][item_pos].width = DND_HELM_BASEWIDTH;
	TradeViewList[MAXPLAYERS][item_pos].height = DND_HELM_BASEHEIGHT;

	TradeViewList[MAXPLAYERS][item_pos].corrupted = false;
	TradeViewList[MAXPLAYERS][item_pos].quality = 0;

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_id = -1;
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_val = 0;
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_tier = 0;
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_extra = 0;
	}
	
	TradeViewList[MAXPLAYERS][item_pos].attrib_count = 0;
	for(i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		TradeViewList[MAXPLAYERS][item_pos].attributes[i].attrib_id = -1;

	TradeViewList[MAXPLAYERS][item_pos].item_image = IIMG_HLM_1 + res;

    RollArmorInfoOnMerchant(item_pos, ilvl, DND_ITEM_HELM, res, MAX_HELM_ATTRIB_DEFAULT);
}

void ConstructSpecialtyItemOnMerchant(int item_pos, int ilvl, int item_type) {
    // decide what type of armor to spawn here -- droppers have tiers not equal to zero, so they can determine some easy armors to drop
	int res, i;
	res = random(1, 100);

	TradeViewList[MAXPLAYERS][item_pos].width = 1;
	TradeViewList[MAXPLAYERS][item_pos].height = 1;

	switch(item_type) {
		case DND_ITEM_SPECIALTY_DOOMGUY:
			i = ITEM_IMAGE_SLAYERCARD_BEGIN;
			if(res <= 33)
				res = SLAYERCARD_STR;
			else if(res <= 66)
				res = SLAYERCARD_DEX;
			else
				res = SLAYERCARD_INT;
		break;

		case DND_ITEM_SPECIALTY_MARINE:
			TradeViewList[MAXPLAYERS][item_pos].height = 2;
			i = ITEM_IMAGE_DOGTAG_BEGIN;
			if(res <= 40)
				res = DOGTAG_PRIVATE;
			else if(res <= 60)
				res = DOGTAG_SGT;
			else
				res = DOGTAG_CORPORAL;
		break;

		case DND_ITEM_SPECIALTY_HOBO:
			TradeViewList[MAXPLAYERS][item_pos].width = 2;
			i = ITEM_IMAGE_SUNGLASSES_BEGIN;
			if(res <= 20)
				res = SUNGLASS_BLACK;
			else if(res <= 70)
				res = SUNGLASS_PINK;
			else
				res = SUNGLASS_GREEN;
		break;

		case DND_ITEM_SPECIALTY_PUNISHER:
			i = ITEM_IMAGE_CIGAR_BEGIN;
			if(res <= 20)
				res = CIGAR_LIGHT;
			else if(res <= 60)
				res = CIGAR_MID;
			else
				res = CIGAR_HEAVY;
		break;

		case DND_ITEM_SPECIALTY_WANDERER:
			i = ITEM_IMAGE_POWERRING_BEGIN;
			if(res <= 40)
				res = POWERRING_GREEN;
			else if(res <= 80)
				res = POWERRING_BLUE;
			else
				res = POWERRING_RED;
		break;

		case DND_ITEM_SPECIALTY_CYBORG:
			i = ITEM_IMAGE_POWERCORE_BEGIN;
			if(res <= 35)
				res = PCORE_COPPER;
			else if(res <= 75)
				res = PCORE_GOLD;
			else if(res <= 90)
				res = PCORE_UNSTABLE;
			else
				res = PCORE_MOLYBDENUM;
		break;

		case DND_ITEM_SPECIALTY_BERSERKER:
			TradeViewList[MAXPLAYERS][item_pos].width = 2;
			i = ITEM_IMAGE_BELT_BEGIN;
			if(res <= 33)
				res = BELT_SASH;
			else if(res <= 66)
				res = BELT_HEAVY;
			else
				res = BELT_LIGHT;
		break;

		case DND_ITEM_SPECIALTY_TRICKSTER:
			TradeViewList[MAXPLAYERS][item_pos].height = 2;
			i = ITEM_IMAGE_CLAW_BEGIN;
			if(res <= 40)
				res = CLAW_RAKE;
			else if(res <= 80)
				res = CLAW_CESTUS;
			else
				res = CLAW_KATAR;
		break;
	}

	TradeViewList[MAXPLAYERS][item_pos].item_image = i + res;

	TradeViewList[MAXPLAYERS][item_pos].item_level = ilvl;
	TradeViewList[MAXPLAYERS][item_pos].item_stack = 0;
	TradeViewList[MAXPLAYERS][item_pos].item_type = item_type;
	TradeViewList[MAXPLAYERS][item_pos].item_subtype = res;

	TradeViewList[MAXPLAYERS][item_pos].corrupted = false;
	TradeViewList[MAXPLAYERS][item_pos].quality = 0;

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_id = -1;
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_val = 0;
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_tier = 0;
		TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_extra = 0;
	}
	
	TradeViewList[MAXPLAYERS][item_pos].attrib_count = 0;
	for(i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		TradeViewList[MAXPLAYERS][item_pos].attributes[i].attrib_id = -1;

	RollSpecialtyItemInfoOnMerchant(item_pos, ilvl, item_type);
}

int RollSpecialtyItemInfoOnMerchant(int item_pos, int ilvl, int itype) {
	// roll random attributes for the charm
	int i = 0, roll;
	int sub_type = ConstructSpecialtyDataOnField(item_pos, ilvl, itype);
	int count = random(1, MAX_SPECIALTY_ATTRIBS);
	int special_roll = 0;

	SetupItemImplicit(item_pos, itype, sub_type, ilvl, true);
	
	while(i < count) {
		do {
			roll = PickRandomAttribute(itype, special_roll, TradeViewList[MAXPLAYERS][item_pos].implicit[0].attrib_id);
		} while(CheckItemAttribute(MAXPLAYERS, item_pos, roll, DND_SYNC_ITEMSOURCE_TRADEVIEW, count) != -1);
		AddAttributeToMerchant(item_pos, roll, count);
		++i;
	}

	return sub_type;
}

void RollCharmInfoOnMerchant(int charm_pos, int charm_type, int charm_tier) {
	// roll random attributes for the charm
	int i = 0, roll;
	int count = GetMaxItemAffixes(DND_ITEM_CHARM, charm_type);
	int synergy_roll = -2;
	
	switch(charm_type) {
		case DND_CHARM_SMALL:
			TradeViewList[MAXPLAYERS][charm_pos].item_image = random(DND_SMALLCHARM_IMAGEBEGIN, DND_SMALLCHARM_IMAGEEND);
		break;
		case DND_CHARM_MEDIUM:
			TradeViewList[MAXPLAYERS][charm_pos].item_image = random(DND_MEDIUMCHARM_IMAGEBEGIN, DND_MEDIUMCHARM_IMAGEEND);
		break;
		case DND_CHARM_LARGE:
			TradeViewList[MAXPLAYERS][charm_pos].item_image = random(DND_LARGECHARM_IMAGEBEGIN, DND_LARGECHARM_IMAGEEND);
		break;
	}
	
	while(i < count) {
		int max_tries = 10;
		do {
			roll = PickRandomAttribute(DND_ITEM_CHARM, charm_type, 0, -1, synergy_roll);

			// in case there's synergy with not enough mods available for a tag or unlucky rolls, opt out of synergy roll immediately
			--max_tries;
			if(max_tries <= 0)
				synergy_roll = 0;
		} while(CheckItemAttribute(MAXPLAYERS, charm_pos, roll, DND_SYNC_ITEMSOURCE_TRADEVIEW, count) != -1);
		AddAttributeToMerchant(charm_pos, roll);

		synergy_roll = CheckMerchantItemSynergy(synergy_roll, charm_pos);

		++i;
	}
}

// returns type of charm as result
void ConstructCharmDataOnMerchant(int charm_pos, int ilvl) {
	int res = random(DND_CHARM_SMALL, DND_CHARM_LARGE);

	TradeViewList[MAXPLAYERS][charm_pos].item_level = ilvl;
	TradeViewList[MAXPLAYERS][charm_pos].item_stack = 0; // charms have no stack
	TradeViewList[MAXPLAYERS][charm_pos].item_type = DND_ITEM_CHARM;
	TradeViewList[MAXPLAYERS][charm_pos].item_subtype = res;
	TradeViewList[MAXPLAYERS][charm_pos].width = DND_CHARM_BASEWIDTH;
	TradeViewList[MAXPLAYERS][charm_pos].height = DND_CHARM_BASEHEIGHT + res;

	TradeViewList[MAXPLAYERS][charm_pos].corrupted = false;
	TradeViewList[MAXPLAYERS][charm_pos].quality = 0;

	int i;
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		TradeViewList[MAXPLAYERS][charm_pos].implicit[i].attrib_id = -1;
		TradeViewList[MAXPLAYERS][charm_pos].implicit[i].attrib_val = 0;
		TradeViewList[MAXPLAYERS][charm_pos].implicit[i].attrib_tier = 0;
		TradeViewList[MAXPLAYERS][charm_pos].implicit[i].attrib_extra = 0;
	}
	
	TradeViewList[MAXPLAYERS][charm_pos].attrib_count = 0;
	for(i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		TradeViewList[MAXPLAYERS][charm_pos].attributes[i].attrib_id = -1;

	if(random(0, 1.0) <= DND_MERCHANT_UNIQUECHANCE)
		MakeMerchantItemUnique(charm_pos);
	else
    	RollCharmInfoOnMerchant(charm_pos, res, ilvl);
}

void RollOrbInfoOnMerchant(int item_pos, int ilvl) {
    int w, orbtype = 0;

	int stack = (GetOrbDropStack(ilvl) * random(0, 1.0)) >> 16;
	if(stack < 1)
		stack = 1;

	do {
		w = random(1, ORB_MAXWEIGHT);
		for(orbtype = 0; orbtype < MAX_DROPPABLE_ORBS && OrbDropWeights[orbtype] < w; ++orbtype);
	} while(IsOrbDropException(orbtype));

	if(IsHighTierOrb(orbtype)) {
		stack /= 3;
		if(!stack)
			stack = 1;
	}
	else if(IsMidTierOrb(orbtype)) {
		stack /= 2;
		if(!stack)
			stack = 1;
	}

	// roll random attributes for the charm
	TradeViewList[MAXPLAYERS][item_pos].item_level = ilvl;
	TradeViewList[MAXPLAYERS][item_pos].item_stack = stack; // orbs have default stack of 1
	TradeViewList[MAXPLAYERS][item_pos].item_type = DND_ITEM_ORB;
	TradeViewList[MAXPLAYERS][item_pos].item_subtype = orbtype;
	TradeViewList[MAXPLAYERS][item_pos].width = 1;
	TradeViewList[MAXPLAYERS][item_pos].height = 1;
	TradeViewList[MAXPLAYERS][item_pos].attrib_count = 0;

	TradeViewList[MAXPLAYERS][item_pos].item_image = GetOrbItemImage(orbtype);
}

void RollTokenInfoOnMerchant(int item_pos, int ilvl) {
    int i = 0, w = random(1, MAX_TOKEN_WEIGHT);
    
    for(; i < MAX_TOKENS && TokenWeights[i] < w; ++i);

	// roll random attributes for the charm
	TradeViewList[MAXPLAYERS][item_pos].item_level = 1;

	w = (GetOrbDropStack(ilvl) * random(0, 1.0)) >> 16;
	if(w < 1)
		w = 1;
	
	TradeViewList[MAXPLAYERS][item_pos].item_stack = w;
	TradeViewList[MAXPLAYERS][item_pos].item_level = ilvl;
	TradeViewList[MAXPLAYERS][item_pos].item_type = DND_ITEM_TOKEN;
	TradeViewList[MAXPLAYERS][item_pos].item_subtype = i;
	TradeViewList[MAXPLAYERS][item_pos].width = 1;
	TradeViewList[MAXPLAYERS][item_pos].height = 1;
	TradeViewList[MAXPLAYERS][item_pos].attrib_count = 0;
	TradeViewList[MAXPLAYERS][item_pos].item_image = ITEM_IMAGE_TOKEN_BEGIN + i;
}

bool IsMerchantItemUnique(int pos) {
	return TradeViewList[MAXPLAYERS][pos].item_type > UNIQUE_BEGIN;
}

void CorruptMerchantItem(int item_pos) {
	// pick between random effects to corruption implicits, with equal weight, and decide from there
	// roll between 0 to MAX_CORRUPTION_WEIRD_OUTCOMES + MAX_CORRUPT_IMPLICITS - 1
	// if > than MAX_CORRUPTION_WEIRD_OUTCOMES subtract it to get corrupt implicit
	// NEW: Corruption ALWAYS replaces the very first implicit!
	TradeViewList[MAXPLAYERS][item_pos].isDirty = true;
	TradeViewList[MAXPLAYERS][item_pos].corrupted = true;

	int corr_outcome = random(0, MAX_CORRUPTION_WEIRD_OUTCOMES + MAX_CORRUPT_IMPLICITS - 1);

	if(corr_outcome >= MAX_CORRUPTION_WEIRD_OUTCOMES) {
		int corr_mod = FIRST_CORRUPT_IMPLICIT + corr_outcome - MAX_CORRUPTION_WEIRD_OUTCOMES;
		int extra = GetExtraForMod(MAXPLAYERS, corr_mod);

		if(extra != -1)
			TradeViewList[MAXPLAYERS][item_pos].implicit[0].attrib_extra = extra;

		TradeViewList[MAXPLAYERS][item_pos].implicit[0].attrib_id = corr_mod;
		TradeViewList[MAXPLAYERS][item_pos].implicit[0].attrib_tier = 0;

		// roll the value for this now
		TradeViewList[MAXPLAYERS][item_pos].implicit[0].attrib_val = random(ItemModTable[corr_mod].attrib_low, ItemModTable[corr_mod].attrib_high);

		return;
	}

	// we are within the initial [0, MAX_CORRUPTION_WEIRD_OUTCOMES) range so we can apply the customized change here

	switch(corr_outcome) {
		case DND_CORR_OUTCOME_QUALITY:
			// don't let it hit negative
			TradeViewList[MAXPLAYERS][item_pos].quality += random(-DND_QUALITY_CORRUPTION_CHANGE, DND_QUALITY_CORRUPTION_CHANGE);
			if(TradeViewList[MAXPLAYERS][item_pos].quality < 0)
				TradeViewList[MAXPLAYERS][item_pos].quality = 0;
		break;
	}
}

int GetMerchantItemPrice(int item_pos) {
	// we have a price band for the item, use the price the gauge what kind of orb(s) we can give to the player
	// make tier and amount of mods contribute highly to this too, so we need to do some of the steps we did again for this one
	int base = DND_BASE_MERCHANT_ITEMCOST;

	if(TradeViewList[MAXPLAYERS][item_pos].item_stack > 0) {
		base = DND_BASE_MERCHANT_STACKABLECOST;
		base *= TradeViewList[MAXPLAYERS][item_pos].item_stack;

		if(TradeViewList[MAXPLAYERS][item_pos].item_type == DND_ITEM_ORB) {
			if(IsHighTierOrb(TradeViewList[MAXPLAYERS][item_pos].item_subtype))
				base *= DND_MERCHANT_HIGHTIERORB_FACTOR + TradeViewList[MAXPLAYERS][item_pos].item_level / DND_MERCHANT_STACKABLE_ILVLCONTRIB;
			else if(IsMidTierOrb(TradeViewList[MAXPLAYERS][item_pos].item_subtype))
				base *= DND_MERCHANT_MEDIUMTIERORB_FACTOR + TradeViewList[MAXPLAYERS][item_pos].item_level / DND_MERCHANT_STACKABLE_ILVLCONTRIB;
		}

		base = base * (100 + DND_MERCHANT_STACKABLE_ILVLFACTOR * TradeViewList[MAXPLAYERS][item_pos].item_level) / 100;

		return base;
	}

	int ilvl = TradeViewList[MAXPLAYERS][item_pos].item_level;

	int avg_mod_tier = 0;
	int acount = TradeViewList[MAXPLAYERS][item_pos].attrib_count;
	int fracture_count = 0;
	int i;
	if(acount) {
		if(!IsMerchantItemUnique(item_pos)) {
			for(i = 0; i < acount; ++i) {
				avg_mod_tier += TradeViewList[MAXPLAYERS][item_pos].attributes[i].attrib_tier + 1;
				fracture_count += TradeViewList[MAXPLAYERS][item_pos].attributes[i].fractured;
			}
		
			avg_mod_tier /= acount;
		}
		else
			avg_mod_tier = MAX_CHARM_AFFIXTIERS / 2;
	}

	base = base * (100 + (ilvl - 1) * DND_MERCHANT_LEVEL_PERCENT + fracture_count * DND_MERCHANT_FRACTURE_PERCENT + DND_MERCHANT_TIER_PERCENT * avg_mod_tier) / 100;

	// if corrupted or has implicit, include that too
	fracture_count = 0;
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i)
		fracture_count += TradeViewList[MAXPLAYERS][item_pos].implicit[i].attrib_id != -1;

	base = base * (100 + DND_MERCHANT_IMPLICIT_PERCENT * fracture_count) / 100;
	
	if(TradeViewList[MAXPLAYERS][item_pos].corrupted) {
		// 50% increase
		base *= 3;
		base >>= 1;
	}

	if(TradeViewList[MAXPLAYERS][item_pos].item_type > UNIQUE_BEGIN)
		base = base * (100 + DND_MERCHANT_UNIQUEFACTOR) / 100;

	return base;
}

// build merchant's items, uses TradeViewList[MAXPLAYERS][item#] as a list
Script "DnD Merchant Items" (void) {
	int pcount =  PlayerInformationInLevel[PLAYERLEVELINFO_COUNTATSTART];
	pcount = PlayerInformationInLevel[PLAYERLEVELINFO_LEVELATSTART] / pcount;

	// just in case, we limit items here
	int item_count = random(DND_MIN_MERCHANTITEMS, DND_MAX_MERCHANTITEMS + pcount / DND_MERCHANT_ITEMSPERLVL);
	if(item_count > DND_MERCHANT_LIMITITEMS)
		item_count = DND_MERCHANT_LIMITITEMS;

	for(int pos = 0; pos < item_count; ++pos) {
        int ilvl = RollItemLevel();
		int temp = random(0, 1.0);

#ifdef ISDEBUGBUILD
		temp = DND_MERCHANT_ITEMCHANCE;
#endif

		if(temp <= DND_MERCHANT_ITEMCHANCE) {
			// roll items, charm or armor
			temp = random(0, 1.0);

			if(temp <= DND_MERCHANT_ARMORCHANCE) // armor
                ConstructArmorDataOnMerchant(pos, ilvl);
			else if(temp <= DND_MERCHANT_HELMCHANCE)
				ConstructHelmDataOnMerchant(pos, ilvl);
			else if(temp <= DND_MERCHANT_BOOTCHANCE)
				ConstructBootDataOnMerchant(pos, ilvl);
			else if(temp <= DND_MERCHANT_SPECIALTYCHANCE)
				ConstructSpecialtyItemOnMerchant(pos, ilvl, random(FIRST_SPECIALTY_ITEM_TYPE, LAST_SPECIALTY_ITEM_TYPE));
			else // charm
                ConstructCharmDataOnMerchant(pos, ilvl);

			// roll quality
			temp = random(0, 1.0);
			if(temp <= DND_MERCHANT_QUALITYCHANCE) {
				temp = random(0, 1.0);
				if(temp <= DND_MERCHANT_LOWQUALITY)
					temp = random(0, 7);
				else if(temp <= DND_MERCHANT_MEDQUALITY)
					temp = random(8, 15);
				else
					temp = random(16, DND_MAX_ITEM_QUALITY);
				TradeViewList[MAXPLAYERS][pos].quality = temp;
			}

			temp = random(0, 1.0);
			if(temp <= DND_MERCHANT_ITEMCORRUPTCHANCE)
				CorruptMerchantItem(pos);
		}
		else if(temp <= DND_MERCHANT_ORBCHANCE) {
			// roll some orbs
            RollOrbInfoOnMerchant(pos, ilvl);
		}
		else {
			// roll tokens
            RollTokenInfoOnMerchant(pos, ilvl);
		}

        TradeViewList[MAXPLAYERS][pos].topleftboxid = pos + 1;
        TradeViewList[MAXPLAYERS][pos].isDirty = true;
        SyncItemData(MAXPLAYERS, pos, DND_SYNC_ITEMSOURCE_TRADEVIEW, -1, -1, true);
	}

    SetResultValue(0);
}

void SpawnMerchant() {
	ACS_NamedExecuteWithResult("DnD Try Spawn Area", 0, "DnD_Merchant", DND_MERCHANT_TID, 32 | (4 << 16) | (22 << 24));
	ACS_NamedExecuteWithResult("DnD Merchant Items");
}

#endif