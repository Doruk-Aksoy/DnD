#ifndef DND_ARMOR_IN
#define DND_ARMOR_IN

enum {
    // body armors
    BODYARMOR_GREEN,
    BODYARMOR_YELLOW,
    BODYARMOR_BLUE,
    BODYARMOR_RED,

	BODYARMOR_GUNSLINGER,
	BODYARMOR_OCCULT,
	BODYARMOR_DEMO,
	BODYARMOR_ENERGY,
	BODYARMOR_ELEMENTAL,

	BODYARMOR_MONOLITH,
	BODYARMOR_CYBER,
	BODYARMOR_DUELIST,
	BODYARMOR_NECRO,
	BODYARMOR_KNIGHT,
	BODYARMOR_RAVAGER,

	BODYARMOR_SYNTHMETAL,
	BODYARMOR_LIGHTNINGCOIL
};
#define BODYARMORS_BEGIN BODYARMOR_GREEN
#define BODYARMORS_END BODYARMOR_LIGHTNINGCOIL

#define DND_BODYARMOR_BASEWIDTH 2
#define DND_BODYARMOR_BASEHEIGHT 2

#define MAX_ARMOR_ATTRIB_DEFAULT 6

// returns type of charm as result
int ConstructArmorDataOnField(int item_pos, int item_tier, int tiers = 0) {
    // decide what type of armor to spawn here -- droppers have tiers not equal to zero, so they can determine some easy armors to drop
	int res;
	if(!tiers) {
		// pick with some weight here
		res = random(BODYARMORS_BEGIN, BODYARMORS_END);
		//res = BODYARMOR_CYBER;
	}
	else if(tiers == 1) {
		res = random(0, 2);
		// 1/3rd chance to be yellow spawn
		if(res)
			res = BODYARMOR_GREEN;
		else
			res = BODYARMOR_YELLOW;
	}
	else if(tiers == 2) {
		// 1/5 chance to be red
		res = random(0, 4);
		if(res)
			res = BODYARMOR_BLUE;
		else
			res = BODYARMOR_RED;
	}
	else if(tiers < 0)
		res = -tiers;

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

int RollArmorInfo(int item_pos, int item_tier, int pnum, int tiers = 0) {
	// roll random attributes for the charm
	int i = 0, roll;
	int armor_type = ConstructArmorDataOnField(item_pos, item_tier, tiers);
	int count = random(1, MAX_ARMOR_ATTRIB_DEFAULT);
	int special_roll = 0;
	
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
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 250, 0, item_tier, 150);
		break;

		case BODYARMOR_GUNSLINGER:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_5;
			special_roll = PPOWER_CANROLLPHYS;
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 100, PPOWER_CANROLLPHYS, item_tier, 75);
		break;
		case BODYARMOR_OCCULT:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_6;
			special_roll = PPOWER_CANROLLOCCULT;
			GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 80, PPOWER_CANROLLOCCULT, item_tier, 60);
		break;
		case BODYARMOR_DEMO:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_7;
			special_roll = PPOWER_CANROLLEXP;
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 100, PPOWER_CANROLLEXP, item_tier, 75);
		break;
		case BODYARMOR_ENERGY:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_8;
			special_roll = PPOWER_CANROLLENERGY;
			GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 80, PPOWER_CANROLLENERGY, item_tier, 60);
		break;
		case BODYARMOR_ELEMENTAL:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_9;
			special_roll = PPOWER_CANROLLELEMENTAL;
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 100, PPOWER_CANROLLELEMENTAL, item_tier, 75);
		break;

		case BODYARMOR_MONOLITH:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_10;
			GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 100, 0, item_tier, 50);
		break;
		case BODYARMOR_CYBER:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_11;
			GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 125, PPOWER_CYBER, item_tier, 75);
		break;
		case BODYARMOR_DUELIST:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_12;
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 125, PPOWER_HITSCANPROTECT, item_tier, 75);
		break;
		case BODYARMOR_NECRO:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_13;
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 200, PPOWER_SPIKES, item_tier, 100);
		break;
		case BODYARMOR_KNIGHT:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_14;
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 225, PPOWER_KNIGHTMELEEBONUS, item_tier, 125);
		break;
		case BODYARMOR_RAVAGER:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_15;
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 180, PPOWER_RAVAGER, item_tier, 90);
		break;

		case BODYARMOR_SYNTHMETAL:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_16;
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 200, PPOWER_LOWERREFLECT, item_tier, 125);
		break;
		case BODYARMOR_LIGHTNINGCOIL:
			Inventories_On_Field[item_pos].item_image = IIMG_ARM_17;
			GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 75, PPOWER_LIGHTNINGABSORB, item_tier, 35);
		break;
	}
	
	while(i < count) {
		do {
			roll = PickRandomAttribute(DND_ITEM_BODYARMOR, special_roll);
		} while(CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(item_pos, roll, pnum, count);
		++i;
	}

	return armor_type;
}

str GetArmorDropClass(int type) {
	return StrParam(s:"ArmorDrop_", d:type);
}

int GetArmorID(int pnum = -1) {
	if(pnum == -1)
		pnum = PlayerNumber();

	if(Items_Used[pnum][BODY_ARMOR_INDEX].item_type == DND_ITEM_NULL)
		return -1;

	return Items_Used[pnum][BODY_ARMOR_INDEX].item_subtype;
}

int GetActorArmorID(int tid) {
	return GetArmorID(tid - P_TIDSTART);
}

bool ActorHasNoArmor(int tid) {
	int pnum = tid - P_TIDSTART;

	return Items_Used[pnum][BODY_ARMOR_INDEX].item_type == DND_ITEM_NULL && Items_Used[pnum][BOOT_INDEX].item_type == DND_ITEM_NULL;
}

int DoArmorRatingEffect(int dmg, int rating) {
	// you will need 5 times the damage to gain half reduction
	return dmg - (dmg * rating) / (rating + 5 * dmg);
}

void SetEnergyShield(int val) {
	SetInventory("EShieldAmount", val);
	SetInventory("EShieldAmountVisual", val);
}

void AddEnergyShield(int val) {
	GiveInventory("EShieldAmount", val);
	GiveInventory("EShieldAmountVisual", val);
}

void TakeEnergyShield(int val) {
	TakeInventory("EShieldAmount", val);
	TakeInventory("EShieldAmountVisual", val);
}

void UpdateEnergyShieldVisual(int val) {
	SetAmmoCapacity("EShieldAmountVisual", val);
}

void SpawnBodyArmor(int pnum, int rarity_boost, bool noRepeat = false, int tiers = 0) {
    int c = CreateItemSpot();
	if(c != -1) {
        int type = RollArmorInfo(c, RollItemLevel(), pnum, tiers);

        // depending on armor type rolled, spawn its appropriate actor
        SpawnDrop(GetArmorDropClass(type), 16.0, 16, pnum + 1, c, true);

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
    GiveInventory("ArmorSoundPlayer", 1);
	
    HandleInventoryPickup(sp >> 16);
}

Script "DnD Armor Message" (int id, int type) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	if(type > UNIQUE_BEGIN) {
		type = (type >> UNIQUE_BITS) - 1;
		Log(s:StrParam(s:"\cc", l:"DND_PICKUP_ARMOR", s:": \c[Y5]", l:GetUniqueItemName(type), s:"!\c-"));
	}
	else
		Log(s:StrParam(s:"\cc", l:"DND_PICKUP_ARMOR", s:": \c[Y5]", l:GetArmorInventoryTag(id), s:"!\c-"));
}

Script "DnD Drop Random Basic Armor" (int higher_tier) {
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && !PlayerIsSpectator(i))
			SpawnBodyArmor(i, 0, true, higher_tier);
	}
}

#endif