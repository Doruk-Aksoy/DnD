#ifndef DND_SPECIALTY_IN
#define DND_SPECIALTY_IN

// specialty items
enum {
	// doomguy
	SLAYERCARD_STR = 0,
	SLAYERCARD_DEX,
	SLAYERCARD_INT,

	// marine
	DOGTAG_PRIVATE = 0,
	DOGTAG_SGT,
	DOGTAG_CORPORAL,

	// hobo
	SUNGLASS_BLACK = 0,
	SUNGLASS_PINK,
	SUNGLASS_GREEN,

	// punisher
	CIGAR_LIGHT = 0,
	CIGAR_MID,
	CIGAR_HEAVY,

	// wanderer
	POWERRING_GREEN = 0,
	POWERRING_RED,
	POWERRING_BLUE,

	// cyborg
    PCORE_COPPER = 0,
    PCORE_GOLD,
	PCORE_UNSTABLE,
    PCORE_MOLYBDENUM,

	// berserker
	BELT_SASH = 0,
	BELT_HEAVY,
	BELT_LIGHT,

	// trickster
	CLAW_RAKE = 0,
	CLAW_CESTUS,
	CLAW_KATAR
};

#define MAX_SPECIALTY_ATTRIBS 3

int ConstructSpecialtyDataOnField(int item_pos, int item_tier, int item_type) {
    // decide what type of armor to spawn here -- droppers have tiers not equal to zero, so they can determine some easy armors to drop
	int res, i;
	res = random(1, 100);

	Inventories_On_Field[item_pos].width = 1;
	Inventories_On_Field[item_pos].height = 1;

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
			Inventories_On_Field[item_pos].height = 2;
			i = ITEM_IMAGE_DOGTAG_BEGIN;
			if(res <= 40)
				res = DOGTAG_PRIVATE;
			else if(res <= 60)
				res = DOGTAG_SGT;
			else
				res = DOGTAG_CORPORAL;
		break;

		case DND_ITEM_SPECIALTY_HOBO:
			Inventories_On_Field[item_pos].width = 2;
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
			Inventories_On_Field[item_pos].width = 2;
			i = ITEM_IMAGE_BELT_BEGIN;
			if(res <= 33)
				res = BELT_SASH;
			else if(res <= 66)
				res = BELT_HEAVY;
			else
				res = BELT_LIGHT;
		break;

		case DND_ITEM_SPECIALTY_TRICKSTER:
			Inventories_On_Field[item_pos].height = 2;
			i = ITEM_IMAGE_CLAW_BEGIN;
			if(res <= 40)
				res = CLAW_RAKE;
			else if(res <= 80)
				res = CLAW_CESTUS;
			else
				res = CLAW_KATAR;
		break;
	}

	Inventories_On_Field[item_pos].item_image = i + res;

	if(item_tier > GetCVar("dnd_maxmonsterlevel"))
		item_tier = GetCVar("dnd_maxmonsterlevel");

	Inventories_On_Field[item_pos].item_level = item_tier;
	Inventories_On_Field[item_pos].item_stack = 0;
	Inventories_On_Field[item_pos].item_type = item_type;
	Inventories_On_Field[item_pos].item_subtype = res;

	Inventories_On_Field[item_pos].corrupted = false;
	Inventories_On_Field[item_pos].quality = 0;

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		Inventories_On_Field[item_pos].implicit[i].attrib_id = -1;
		Inventories_On_Field[item_pos].implicit[i].attrib_val = 0;
		Inventories_On_Field[item_pos].implicit[i].attrib_tier = 0;
		Inventories_On_Field[item_pos].implicit[i].attrib_extra = 0;
	}
	
	Inventories_On_Field[item_pos].attrib_count = 0;
	for(i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		Inventories_On_Field[item_pos].attributes[i].attrib_id = -1;

	return res;
}

int RollSpecialtyItemInfo(int item_pos, int item_tier, int pnum, int itype) {
	// roll random attributes for the charm
	int i = 0, roll;
	int sub_type = ConstructSpecialtyDataOnField(item_pos, item_tier, itype);
	int count = random(1, MAX_SPECIALTY_ATTRIBS);
	int special_roll = 0;

	SetupItemImplicit(item_pos, itype, sub_type, item_tier);
	
	while(i < count) {
		do {
			roll = PickRandomAttribute(itype, special_roll, Inventories_On_Field[item_pos].implicit[0].attrib_id);
		} while(CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(item_pos, roll, pnum, count);
		++i;
	}

	return sub_type;
}

str GetSpecialtyDropClass(int type, int subtype) {
	switch(type) {
		case DND_ITEM_SPECIALTY_DOOMGUY:
		return StrParam(s:"SlayerCardDrop_", d:subtype);
		case DND_ITEM_SPECIALTY_MARINE:
		return StrParam(s:"DogTagDrop_", d:subtype);
		case DND_ITEM_SPECIALTY_HOBO:
		return StrParam(s:"SunglassDrop_", d:subtype);
		case DND_ITEM_SPECIALTY_PUNISHER:
		return StrParam(s:"CigarDrop_", d:subtype);
		case DND_ITEM_SPECIALTY_WANDERER:
		return StrParam(s:"PowerringDrop_", d:subtype);
		case DND_ITEM_SPECIALTY_CYBORG:
		return StrParam(s:"PowercoreDrop_", d:subtype);
		case DND_ITEM_SPECIALTY_BERSERKER:
		return StrParam(s:"BeltDrop_", d:subtype);
		case DND_ITEM_SPECIALTY_TRICKSTER:
		return StrParam(s:"ClawDrop_", d:subtype);
	}
	return StrParam(s:"PowercoreDrop_", d:subtype);
}

Script "DnD Specialty Item Pickup" (int sp) {
    if((sp & 0xFFFF) == 255)
		SetActivatorToTarget(0);
	else
		SetActivator((sp & 0xFFFF) + P_TIDSTART);

	ACS_NamedExecuteAlways("DnD Specialty Message", 0, Inventories_On_Field[sp >> 16].item_subtype, Inventories_On_Field[sp >> 16].item_type);
    GiveInventory("PowercoreSoundPlayer", 1);
	
    HandleInventoryPickup(sp >> 16);
}

Script "DnD Specialty Message" (int id, int type) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	str item_pickup = "";
	switch(type) {
		case DND_ITEM_SPECIALTY_DOOMGUY:
			item_pickup = "DND_PICKUP_SLAYERCARD";
		break;
		case DND_ITEM_SPECIALTY_MARINE:
			item_pickup = "DND_PICKUP_DOGTAG";
		break;
		case DND_ITEM_SPECIALTY_HOBO:
			item_pickup = "DND_PICKUP_SUNGLASS";
		break;
		case DND_ITEM_SPECIALTY_PUNISHER:
			item_pickup = "DND_PICKUP_CIGAR";
		break;
		case DND_ITEM_SPECIALTY_WANDERER:
			item_pickup = "DND_PICKUP_POWERRING";
		break;
		case DND_ITEM_SPECIALTY_CYBORG:
			item_pickup = "DND_PICKUP_POWERCORE";
		break;
		case DND_ITEM_SPECIALTY_BERSERKER:
			item_pickup = "DND_PICKUP_BELT";
		break;
		case DND_ITEM_SPECIALTY_TRICKSTER:
			item_pickup = "DND_PICKUP_CLAW";
		break;
	}

	if(type > UNIQUE_BEGIN) {
		type = (type >> UNIQUE_BITS) - 1;
		Log(s:StrParam(s:"\cc", l:item_pickup, s:": \c[Y5]", l:GetUniqueItemName(type, type), s:"!\c-"));
	}
	else
		Log(s:StrParam(s:"\cc", l:item_pickup, s:": \c[Y5]", l:GetItemTagName(type, id), s:"!\c-"));
}

#endif