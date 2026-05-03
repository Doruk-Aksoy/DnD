#ifndef DND_FLASKS_IN
#define DND_FLASKS_IN

#define DND_FLASK_BASEWIDTH 1
#define DND_FLASK_BASEHEIGHT 2

#define MAX_FLASK_ATTRIB_DEFAULT 2

enum {
    DND_FLASK_LIFE_SMALL,
    DND_FLASK_LIFE_MEDIUM,
    DND_FLASK_LIFE_LARGE,
    DND_FLASK_LIFE_GRAND,
    DND_FLASK_LIFE_EXQUISITE
};

#define FLASK_LIFE_BEGIN DND_FLASK_LIFE_SMALL
#define FLASK_LIFE_END DND_FLASK_LIFE_EXQUISITE

#define FLASKS_BEGIN DND_FLASK_LIFE_SMALL
#define FLASKS_END DND_FLASK_LIFE_EXQUISITE

void SetupFlaskDropWeights() {
	// Body Armors
	int weight = 0;
	SET_ITEM_WEIGHT(DND_DROPPEDITEM_FLASK, DND_FLASK_LIFE_SMALL, 50);
	SET_ITEM_WEIGHT(DND_DROPPEDITEM_FLASK, DND_FLASK_LIFE_MEDIUM, 35);
	SET_ITEM_WEIGHT(DND_DROPPEDITEM_FLASK, DND_FLASK_LIFE_LARGE, 20);
	SET_ITEM_WEIGHT(DND_DROPPEDITEM_FLASK, DND_FLASK_LIFE_GRAND, 16);
    SET_ITEM_WEIGHT(DND_DROPPEDITEM_FLASK, DND_FLASK_LIFE_EXQUISITE, 8);
}

int GetFlaskLevelThreshold(int type) {
	switch(type) {
		case DND_FLASK_LIFE_SMALL:
		return 0;
		case DND_FLASK_LIFE_MEDIUM:
		return 15;
		case DND_FLASK_LIFE_LARGE:
		return 28;
		case DND_FLASK_LIFE_GRAND:
		return 41;
		case DND_FLASK_LIFE_EXQUISITE:
		return 55;
	}
}

int ConstructFlaskDataOnField(int item_pos, int item_tier, int pnum, int flask = -1) {
    // decide what type of armor to spawn here -- droppers have tiers not equal to zero, so they can determine some easy armors to drop
	int i;
	int res = 0;
	int plvl = GetActorLevel(pnum + P_TIDSTART);
	if(flask == -1) {
		res = random(1, MAX_FLASK_WEIGHT);
		for(i = 0; i <= FLASKS_END; ++i) {
			if(res <= ItemDropWeights[DND_DROPPEDITEM_FLASK][i]) {
				// return the previous acceptable flask if the level threshold isn't met... or the base of 0
				if(plvl < GetFlaskLevelThreshold(i))
					res = Max(i - 1, 0);
				res = i;
				break;
			}
		}
	}
	else
		res = flask;

	if(item_tier > GetCVar("dnd_maxmonsterlevel"))
		item_tier = GetCVar("dnd_maxmonsterlevel");

	Inventories_On_Field[item_pos].item_level = item_tier;
	Inventories_On_Field[item_pos].item_stack = 0;
	Inventories_On_Field[item_pos].item_type = DND_ITEM_FLASK;
	Inventories_On_Field[item_pos].item_subtype = res;
	Inventories_On_Field[item_pos].width = DND_FLASK_BASEWIDTH;
	Inventories_On_Field[item_pos].height = DND_FLASK_BASEHEIGHT;

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

int InitializeFlask(int item_pos, int item_tier, int pnum, int type = -1) {
	int flask_type = ConstructFlaskDataOnField(item_pos, item_tier, pnum, type);
	Inventories_On_Field[item_pos].item_image = ITEM_IMAGE_FLASK_BEGIN + flask_type;
	return flask_type;
}

void RollFlaskInfo(int item_pos, int item_tier, int pnum, int item_type, int flask_type, int max_attr) {
	// only for rolling body armors we access the array for item_tier, as that can be changed in ConstructArmorDataOnField to level this down for lower level players
	int i = 0, roll;
	int count = random(1, max_attr);
	int special_roll = SetupItemImplicit(item_pos, item_type, flask_type, item_tier);

	while(i < count) {
		do {
			roll = PickRandomAttribute(item_type, flask_type, special_roll, Inventories_On_Field[item_pos].implicit[0].attrib_id);
		} while(CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(item_pos, roll, pnum, count);
		++i;
	}
}

void RollFlaskInfoWithMods(int item_pos, int item_tier, int pnum, int item_type, int flask_type, int max_attr, int m1, int m2, int m3) {
	// only for rolling body armors we access the array for item_tier, as that can be changed in ConstructArmorDataOnField to level this down for lower level players
	int i = 0, roll;
	int count = random(1, max_attr) - 1 - (m2 != -1) - (m3 != -1);
	int special_roll = SetupItemImplicit(item_pos, item_type, flask_type, item_tier);

	AddAttributeToFieldItem(item_pos, m1, pnum);
	if(m2 != -1)
		AddAttributeToFieldItem(item_pos, m2, pnum);
	if(m3 != -1)
		AddAttributeToFieldItem(item_pos, m3, pnum);

	while(i < count) {
		do {
			roll = PickRandomAttribute(item_type, flask_type, special_roll, Inventories_On_Field[item_pos].implicit[0].attrib_id);
		} while(CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(item_pos, roll, pnum, count);
		++i;
	}
}

bool IsLifeFlask(int subtype) {
    return subtype >= FLASK_LIFE_BEGIN && subtype <= FLASK_LIFE_END;
}

str GetFlaskDropClass(int type) {
    if(IsLifeFlask(type))
	    return StrParam(s:"FlaskDrop_Life_", d:type);
    return "FlaskDrop";
}

void HandleFlaskUse(int pnum, int flask_index) {
    // player has flask
    if(!HasFlaskEquipped(pnum, flask_index))
        return;


}

Script "DnD Flask Use" (int flask_id) NET {
    // if dead or in intermission or spectating, don't allow
    int pnum = PlayerNumber();
    if(!IsAlive() || CheckInventory("DnD_IntermissionState") || PlayerIsSpectator(pnum))
        Terminate;

    HandleFlaskUse(pnum, flask_id);
}

Script "DnD Flask Message" (int id, int type) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	if(type > UNIQUE_BEGIN) {
		Log(s:StrParam(s:"\cc", l:"DND_PICKUP_FLASK", s:": \c[Y5]", l:GetUniqueItemName(type & 0xFFFF, (type >> UNIQUE_BITS) - 1), s:"!\c-"));
	}
	else
		Log(s:StrParam(s:"\cc", l:"DND_PICKUP_FLASK", s:": \c[Y5]", l:GetItemTagName(type, id), s:"!\c-"));
}

Script "DnD Flask Item Pickup" (int sp) {
    if((sp & 0xFFFF) == 255)
		SetActivatorToTarget(0);
	else
		SetActivator((sp & 0xFFFF) + P_TIDSTART);

	ACS_NamedExecuteAlways("DnD Flask Message", 0, Inventories_On_Field[sp >> 16].item_subtype, Inventories_On_Field[sp >> 16].item_type);
    GiveInventory("FlaskSoundPlayer", 1);
	
    HandleInventoryPickup(sp >> 16);
}

#endif