#ifndef DND_FLASKS_IN
#define DND_FLASKS_IN

#define DND_FLASK_BASEWIDTH 1
#define DND_FLASK_BASEHEIGHT 2

#define MAX_FLASK_ATTRIB_DEFAULT 2

#define FLASK_RECOVERY_TICRATE 2

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

struct flaskData_T {
	int flask_type;
	int max_charges;
	int curr_charges;
	int charges_used;				// charges to be taken when used
	int effect_duration;			// total time it takes for it to finish
	int curr_tics;					// curr tics on the effect duration if active
	int quality;					// quality of the flask transferred over from the inventory data
};

#define MAX_FLASK_SLOTS 2
global flaskData_T 34: FlaskData[MAXPLAYERS][MAX_FLASK_SLOTS];

// initialization
void ResetPlayerFlaskData(int pnum) {
	for(int i = 0; i < MAX_FLASK_SLOTS; ++i) {
		FlaskData[pnum][i].flask_type = -1;
		FlaskData[pnum][i].max_charges = 0;
		FlaskData[pnum][i].curr_charges = 0;
		FlaskData[pnum][i].charges_used = 0;
		FlaskData[pnum][i].effect_duration = 0;
		FlaskData[pnum][i].curr_tics = 0;
		FlaskData[pnum][i].quality = 0;
	}
}

enum {
	FLASK_DATA_MAXCHARGES,
	FLASK_DATA_CHARGEUSE,
	FLASK_DATA_EFFECTDURATION,
	FLASK_DATA_GIVEAMOUNT
};

bool IsLifeFlask(int subtype) {
    return subtype >= FLASK_LIFE_BEGIN && subtype <= FLASK_LIFE_END;
}

int GetFlaskData(int pnum, int flask_id, int flask_type, int data_type) {
	int res = 0;
	int i;
	if(IsLifeFlask(flask_type)) {
		switch(data_type) {
			case FLASK_DATA_MAXCHARGES:
				for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
					if(Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_id != -1 && Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_id == INV_FLASK_IMP_CHARGECOUNT)
						res = Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_val;
				}
			break;
			case FLASK_DATA_CHARGEUSE:
				for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
					if(Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_id != -1 && Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_id == INV_FLASK_IMP_CHARGECOUNT)
						res = Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_extra;
				}
			break;
			case FLASK_DATA_EFFECTDURATION:
				for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
					if(Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_id != -1 && Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_id == INV_FLASK_IMP_LIFE)
						res = Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_extra;
				}
			break;
			case FLASK_DATA_GIVEAMOUNT:
				for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
					if(Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_id != -1 && Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_id == INV_FLASK_IMP_LIFE)
						res = Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_val;
				}
			break;
		}
	}
	else {

	}

	return res;
}

// Reads equipped item data to gather information about the flasks
void UpdatePlayerFlaskData(int pnum, bool charLoad = false) {
	for(int i = 0; i < MAX_FLASK_SLOTS; ++i) {
		if(Items_Used[pnum][FLASK1_INDEX + i].item_type == -1)
			continue;

		FlaskData[pnum][i].flask_type = Items_Used[pnum][FLASK1_INDEX + i].item_subtype;

		FlaskData[pnum][i].max_charges = GetFlaskData(pnum, i, FlaskData[pnum][i].flask_type, FLASK_DATA_MAXCHARGES);

		// if we aren't loading fresh set to 0, player just put this in place of something else in the world
		FlaskData[pnum][i].curr_charges = charLoad ? FlaskData[pnum][i].max_charges : 0;
		FlaskData[pnum][i].effect_duration = GetFlaskData(pnum, i, FlaskData[pnum][i].flask_type, FLASK_DATA_EFFECTDURATION);
		FlaskData[pnum][i].charges_used = GetFlaskData(pnum, i, FlaskData[pnum][i].flask_type, FLASK_DATA_CHARGEUSE);

		// not active
		FlaskData[pnum][i].curr_tics = 0;
		FlaskData[pnum][i].quality = Items_Used[pnum][FLASK1_INDEX + i].quality;
	}
}

void GiveFlaskCharges(int pnum, int amt) {
	for(int i = 0; i < MAX_FLASK_SLOTS; ++i) {
		if(FlaskData[pnum][i].flask_type != -1) {
			FlaskData[pnum][i].curr_charges += amt;
			if(FlaskData[pnum][i].curr_charges > FlaskData[pnum][i].max_charges)
				FlaskData[pnum][i].curr_charges = FlaskData[pnum][i].max_charges;
		}
	}
}

bool HasFlaskEquipped(int pnum, int flask_id) {
	return (Items_Used[pnum][FLASK1_INDEX + flask_id].item_type & 0xFFFF) == DND_ITEM_FLASK;
}

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
	return 0;
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

str GetFlaskDropClass(int type) {
    if(IsLifeFlask(type))
	    return StrParam(s:"FlaskDrop_Life_", d:type);
    return "FlaskDrop";
}

bool CheckFlaskScriptConditions(int pnum) {
	return !isAlive() || !PlayerInGame(pnum) || PlayerIsSpecator(pnum) || CheckInventory("DnD_IntermissionState");
}

Script "DnD Flask Use" (int flask_id) NET {
    // if dead or in intermission or spectating, don't allow
    int pnum = PlayerNumber();
    if(CheckFlaskScriptConditions(pnum))
        Terminate;

	// player has flask
    if(!HasFlaskEquipped(pnum, flask_id) || FlaskData[pnum][flask_id].curr_charges < FlaskData[pnum][flask_id].charges_used || FlaskData[pnum][flask_id].curr_tics) {
		// play some error sound
        Terminate;
	}

	int cap;
	// if its life flask only allow use when not max health already
    if(IsLifeFlask(FlaskData[pnum][flask_id].flask_type) && GetActorProperty(0, APROP_HEALTH) < (cap = CheckInventory("PlayerHealthCap"))) {
		// play the drink sound
		PlaySound(0, "Items/FlaskUse", CHAN_ITEM);
		FlaskData[pnum][flask_id].curr_charges -= FlaskData[pnum][flask_id].charges_used;

		int total_time = FlaskData[pnum][flask_id].effect_duration * TICRATE / FLASK_RECOVERY_TICRATE;

		// this is the amount that would be given over this duration -- also consider quality effect here
		int toGive_total = GetFlaskData(pnum, flask_id, FlaskData[pnum][flask_id].flask_type, FLASK_DATA_GIVEAMOUNT);
		if(FlaskData[pnum][flask_id].quality)
			toGive_total = toGive_total * (100 + FlaskData[pnum][flask_id].quality) / 100;

		int toGive = toGive_total / total_time;
		int currGiven = 0;
		if(!toGive)
			toGive = 1;

		// 50 life flask healed up to 49 not 50
		while(!CheckFlaskScriptConditions(pnum) && GetActorProperty(0, APROP_HEALTH) < (cap = CheckInventory("PlayerHealthCap")) && FlaskData[pnum][flask_id].curr_tics < total_time) {
			if(GetActorProperty(0, APROP_HEALTH) + toGive > cap) {
				// limit reached, stop
				GiveInventory("HealthBonusX", cap - toGive);
				break;
			}
			else
				GiveInventory("HealthBonusX", toGive);

			// max amount to give has been depleted
			currGiven += toGive;
			if(currGiven >= toGive_total)
				break;

			++FlaskData[pnum][flask_id].curr_tics;
			delay(const:FLASK_RECOVERY_TICRATE);
		}

		if(CheckFlaskScriptConditions(pnum))
			Terminate;

		// this is to cap off the remainder in case its an uneven division with time vs amount given
		if(currGiven + toGive < toGive_total)
			GiveInventory("HealthBonusX", toGive_total - (currGiven + toGive));

		FlaskData[pnum][flask_id].curr_tics = 0;
	}
	else {

	}
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

// Depending on monster killed player can gain charges, and if they have more charges gained mods or such they can gain even more
void HandleFlaskChargeGain(int pnum, int m_id) {

}

#endif