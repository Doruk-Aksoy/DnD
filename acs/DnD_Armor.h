#ifndef DND_ARMOR_IN
#define DND_ARMOR_IN

enum {
    // body armors
    BODYARMOR_GREEN,
    BODYARMOR_YELLOW,
    BODYARMOR_BLUE,
    BODYARMOR_RED
};
#define BODYARMORS_BEGIN BODYARMOR_GREEN
#define BODYARMORS_END BODYARMOR_RED

#define DND_BODYARMOR_BASEWIDTH 2
#define DND_BODYARMOR_BASEHEIGHT 2

#define MAX_ARMOR_ATTRIB_DEFAULT 6

// returns type of charm as result
int ConstructArmorDataOnField(int item_pos, int item_tier) {
    // decide what type of armor to spawn here
	int res = random(BODYARMORS_BEGIN, BODYARMORS_END);

	Inventories_On_Field[item_pos].item_level = item_tier;
	Inventories_On_Field[item_pos].item_stack = 0;
	Inventories_On_Field[item_pos].item_type = DND_ITEM_BODYARMOR;
	Inventories_On_Field[item_pos].item_subtype = res;
	Inventories_On_Field[item_pos].width = DND_BODYARMOR_BASEWIDTH;
	Inventories_On_Field[item_pos].height = DND_BODYARMOR_BASEHEIGHT;

	Inventories_On_Field[item_pos].corrupted = false;
	Inventories_On_Field[item_pos].quality = 0;
	Inventories_On_Field[item_pos].implicit.attrib_id = -1;
	Inventories_On_Field[item_pos].implicit.attrib_val = 0;
	Inventories_On_Field[item_pos].implicit.attrib_tier = 0;
	Inventories_On_Field[item_pos].implicit.attrib_extra = 0;
	
	Inventories_On_Field[item_pos].attrib_count = 0;
	for(int i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		Inventories_On_Field[item_pos].attributes[i].attrib_id = -1;

	return res;
}

void RollArmorInfo(int item_pos, int item_tier, int pnum) {
	// roll random attributes for the charm
	int i = 0, roll;
	int armor_type = ConstructArmorDataOnField(item_pos, item_tier);
	int count = random(1, MAX_ARMOR_ATTRIB_DEFAULT);
	
	// implicits that come along with the item always
	switch(armor_type) {
		case BODYARMOR_GREEN:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_1;
			// 100 armor, 33% damage reduction
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 100, 0, item_tier, 80);
		break;
		case BODYARMOR_YELLOW:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_2;
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 150, 0, item_tier, 100);
		break;
		case BODYARMOR_BLUE:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_3;
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 200, 0, item_tier, 125);
		break;
        case BODYARMOR_RED:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_4;
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 300, 0, item_tier, 150);
		break;
	}
	
	while(i < count) {
		do {
			roll = PickRandomAttribute(DND_ITEM_BODYARMOR);
		} while(CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(item_pos, roll, pnum, count);
		++i;
	}
}

void SpawnBodyArmor(int pnum, int rarity_boost, bool noRepeat = false) {
    int c = CreateItemSpot();
	if(c != -1) {
		// c is the index on the field now
        // no unique armors... yet
		/*#ifndef ISDEBUGBUILD
			if((GetCVar("dnd_ignore_dropweights") && random(0, 1)) || RunDefaultDropChance(pnum, UNIQUE_DROPCHANCE * (100 + rarity_boost) / 100))
		#else
			if(random(0,1))
		#endif
		{
			MakeUnique(c, DND_ITEM_CHARM, pnum);
			SpawnDrop("UniqueCharmDrop", 16.0, 16, pnum + 1, c);
		}
		else {
			RollCharmInfo(c, RollItemLevel(), pnum);
			SpawnDrop("CharmDrop", 16.0, 16, pnum + 1, c);
		}*/

        RollArmorInfo(c, RollItemLevel(), pnum);

        // depending on armor type rolled, spawn its appropriate actor
        SpawnDrop("CharmDrop", 16.0, 16, pnum + 1, c);

		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_BODYARMOR);
		
		if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, STAT_LUCK) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
			SpawnBodyArmor(pnum, rarity_boost, true);
	}
}

str GetArmorInventoryTag(int subt) {
	return StrParam(l:StrParam(s:"DND_ARMOR", d:subt + 1));
}

Script "DnD Armor Item Pickup" (int sp) {
    if((sp & 0xFFFF) == 255)
		SetActivatorToTarget(0);
	else
		SetActivator((sp & 0xFFFF) + P_TIDSTART);
	ACS_NamedExecuteAlways("DnD Armor Message", 0, Inventories_On_Field[sp >> 16].item_subtype, Inventories_On_Field[sp >> 16].item_type);
	
    GiveInventory("CharmSoundPlayer", 1);
	
    HandleInventoryPickup(sp >> 16);
}

#endif