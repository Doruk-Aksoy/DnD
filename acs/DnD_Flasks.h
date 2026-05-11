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
    DND_FLASK_LIFE_EXQUISITE,
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

	int chance_on_hit;				// chance to gain charge on hit
	int chance_on_crit;				// chance to gain on crit
};

#define MAX_FLASK_SLOTS 2
global flaskData_T 34: FlaskData[MAXPLAYERS][MAX_FLASK_SLOTS];

void ResetFlask(int pnum, int i) {
	FlaskData[pnum][i].flask_type = -1;
	FlaskData[pnum][i].max_charges = 0;
	FlaskData[pnum][i].curr_charges = 0;
	FlaskData[pnum][i].charges_used = 0;
	FlaskData[pnum][i].effect_duration = 0;
	FlaskData[pnum][i].curr_tics = 0;
	FlaskData[pnum][i].quality = 0;
	FlaskData[pnum][i].chance_on_hit = 0;
	FlaskData[pnum][i].chance_on_crit = 0;
}

// initialization
void ResetPlayerFlaskData(int pnum) {
	for(int i = 0; i < MAX_FLASK_SLOTS; ++i)
		ResetFlask(pnum, i);
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

int GetFlaskChargeUseEffects(int pnum, int flask_id) {
	int base = -GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_REDUCEDCHARGEUSE);
	base += GetFlaskAttributeExtra(pnum, flask_id, INV_FLASK_INCCHARGERECOVERY);
	return base;
}

int GetFlaskRecoveryEffects(int pnum, int flask_id) {
	int base = GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_INCCHARGERECOVERY);
	base -= GetFlaskAttributeExtra(pnum, flask_id, INV_FLASK_INSTANTONLOWLIFE);
	return base;
}

int GetFlaskData(int pnum, int flask_id, int flask_type, int data_type) {
	int res = 0;
	int i;
	int temp;
	if(IsLifeFlask(flask_type)) {
		switch(data_type) {
			case FLASK_DATA_MAXCHARGES:
				for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
					if(Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_id != -1 && Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_id == INV_FLASK_IMP_CHARGECOUNT)
						res = Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_val;
				}

				if((temp = GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_INCCHARGES)))
					res = res * (100 + temp) / 100;
			break;
			case FLASK_DATA_CHARGEUSE:
				for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
					if(Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_id != -1 && Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_id == INV_FLASK_IMP_CHARGECOUNT)
						res = Items_Used[pnum][FLASK1_INDEX + flask_id].implicit[i].attrib_extra;
				}

				if((temp = GetFlaskChargeUseEffects(pnum, flask_id)))
					res = res * (100 + temp) / 100;
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
			
				if((temp = GetFlaskRecoveryEffects(pnum, flask_id)))
					res = res * (100 + temp) / 100;
			break;
		}
	}
	else {

	}

	return res;
}

// Reads equipped item data to gather information about the flasks
void UpdatePlayerFlaskData(int pnum, bool charLoad = false) {
	int tid = pnum + P_TIDSTART;
	for(int i = 0; i < MAX_FLASK_SLOTS; ++i) {
		if(Items_Used[pnum][FLASK1_INDEX + i].item_type == DND_ITEM_NULL) {
			ResetFlask(pnum, i);
			SetActorInventory(tid, StrParam(s:"Flask", d:i + 1, s:"_Type"), 0);
			SetActorInventory(tid, StrParam(s:"Flask", d:i + 1, s:"_CurrentCharges"), 0);
			SetActorInventory(tid, StrParam(s:"Flask", d:i + 1, s:"_TicCounter"), 0);
			continue;
		}

		FlaskData[pnum][i].flask_type = Items_Used[pnum][FLASK1_INDEX + i].item_subtype;

		FlaskData[pnum][i].max_charges = GetFlaskData(pnum, i, FlaskData[pnum][i].flask_type, FLASK_DATA_MAXCHARGES);

		// if we aren't loading fresh set to 0, player just put this in place of something else in the world
		FlaskData[pnum][i].curr_charges = charLoad ? FlaskData[pnum][i].max_charges : 0;
		FlaskData[pnum][i].effect_duration = GetFlaskData(pnum, i, FlaskData[pnum][i].flask_type, FLASK_DATA_EFFECTDURATION);
		FlaskData[pnum][i].charges_used = GetFlaskData(pnum, i, FlaskData[pnum][i].flask_type, FLASK_DATA_CHARGEUSE);

		// not active
		FlaskData[pnum][i].curr_tics = 0;
		FlaskData[pnum][i].quality = Items_Used[pnum][FLASK1_INDEX + i].quality;

		FlaskData[pnum][i].chance_on_hit = GetFlaskAttributeVal(pnum, i, INV_FLASK_CHANCEGAINONHIT);
		FlaskData[pnum][i].chance_on_crit = GetFlaskAttributeVal(pnum, i, INV_FLASK_CHANCEGAINCRIT);

		// send +1 here as 0 doesnt make sense here for SBARINFO from the enums
		SetActorInventory(tid, StrParam(s:"Flask", d:i + 1, s:"_Type"), FlaskData[pnum][i].flask_type + 1);
		SetActorInventory(tid, StrParam(s:"Flask", d:i + 1, s:"_CurrentCharges"), FlaskData[pnum][i].curr_charges);
		SetActorInventory(tid, StrParam(s:"Flask", d:i + 1, s:"_TicCounter"), 0);

		SetAmmoCapacity(StrParam(s:"Flask", d:i + 1, s:"_CurrentCharges"), FlaskData[pnum][i].max_charges);

		// the /2 is needed to match the code that gives tics + make it look right on the hud
		SetAmmoCapacity(StrParam(s:"Flask", d:i + 1, s:"_TicCounter"), FlaskData[pnum][i].effect_duration / FLASK_RECOVERY_TICRATE);
	}
}

// checks to see if this attribute exists on the flask, returns 0 if it doesn't
int GetFlaskAttributeVal(int pnum, int flask_id, int attrib_to_check) {
	if(Items_Used[pnum][FLASK1_INDEX + flask_id].item_type == DND_ITEM_FLASK) {
		for(int i = 0; i < Items_Used[pnum][FLASK1_INDEX + flask_id].attrib_count; ++i) {
			if(Items_Used[pnum][FLASK1_INDEX + flask_id].attributes[i].attrib_id == attrib_to_check)
				return Items_Used[pnum][FLASK1_INDEX + flask_id].attributes[i].attrib_val;
		}
	}
	return 0;
}

int GetFlaskAttributeExtra(int pnum, int flask_id, int attrib_to_check) {
	if(Items_Used[pnum][FLASK1_INDEX + flask_id].item_type == DND_ITEM_FLASK) {
		for(int i = 0; i < Items_Used[pnum][FLASK1_INDEX + flask_id].attrib_count; ++i) {
			if(Items_Used[pnum][FLASK1_INDEX + flask_id].attributes[i].attrib_id == attrib_to_check)
				return Items_Used[pnum][FLASK1_INDEX + flask_id].attributes[i].attrib_extra;
		}
	}
	return 0;
}

void GiveSpecificFlaskCharges(int pnum, int amt, int flask_id) {
	// check for inc charges gained mod on flask
	int temp;
	if((temp = GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_INCCHARGERECOVERY)))
		amt = amt * (100 + temp) / 100;

	FlaskData[pnum][flask_id].curr_charges += amt;
	if(FlaskData[pnum][flask_id].curr_charges > FlaskData[pnum][flask_id].max_charges)
		FlaskData[pnum][flask_id].curr_charges = FlaskData[pnum][flask_id].max_charges;

	SetActorInventory(pnum + P_TIDSTART, StrParam(s:"Flask", d:flask_id + 1, s:"_CurrentCharges"), FlaskData[pnum][flask_id].curr_charges);
}

void GiveFlaskCharges(int pnum, int amt) {
	for(int i = 0; i < MAX_FLASK_SLOTS; ++i) {
		if(FlaskData[pnum][i].flask_type != -1)
			GiveSpecificFlaskCharges(pnum, amt, i);
	}
}

bool HasFlaskEquipped(int pnum, int flask_id) {
	return (Items_Used[pnum][FLASK1_INDEX + flask_id].item_type & 0xFFFF) == DND_ITEM_FLASK;
}

void SetupFlaskDropWeights() {
	// Body Armors
	INIT_ITEM_WEIGHTS;
	SET_ITEM_WEIGHT(DND_DROPPEDITEM_FLASK, DND_FLASK_LIFE_SMALL, 50);
	SET_ITEM_WEIGHT(DND_DROPPEDITEM_FLASK, DND_FLASK_LIFE_MEDIUM, 35);
	SET_ITEM_WEIGHT(DND_DROPPEDITEM_FLASK, DND_FLASK_LIFE_LARGE, 20);
	SET_ITEM_WEIGHT(DND_DROPPEDITEM_FLASK, DND_FLASK_LIFE_GRAND, 16);
    SET_ITEM_WEIGHT(DND_DROPPEDITEM_FLASK, DND_FLASK_LIFE_EXQUISITE, 6);
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
				if(plvl < GetFlaskLevelThreshold(i)) {
					do {
						--i;
					} while(plvl < GetFlaskLevelThreshold(i));
				}
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

bool IsAttributeFlaskException(int flask_type, int id) {
	if(IsLifeFlask(flask_type)) {
		switch(id) {
			case INV_FLASK_INCEFFECT:
			case INV_FLASK_INCDURATION:
			return true;
		}
	}
	else {
		switch(id) {
			case INV_FLASK_INCAMOUNTRECOVER:
			case INV_FLASK_INSTANTRECOVERY:
			case INV_FLASK_INSTANTONLOWLIFE:
    		case INV_FLASK_MORERECOVERYONLOWLIFE:
			case INV_FLASK_INCRECOVERYRATE:
			return true;
		}
	}
	return false;
}

void RollFlaskInfo(int item_pos, int item_tier, int pnum, int flask_type, int max_attr) {
	// only for rolling body armors we access the array for item_tier, as that can be changed in ConstructArmorDataOnField to level this down for lower level players
	int i = 0, roll;
	int count = random(1, max_attr);
	int special_roll = SetupItemImplicit(item_pos, DND_ITEM_FLASK, flask_type, item_tier);

	while(i < count) {
		do {
			roll = PickRandomAttribute(DND_ITEM_FLASK, flask_type, special_roll, Inventories_On_Field[item_pos].implicit[0].attrib_id);
		} while(CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(item_pos, roll, pnum, count);
		++i;
	}
}

void RollFlaskInfoWithMods(int item_pos, int item_tier, int pnum, int flask_type, int max_attr, int m1, int m2, int m3) {
	// only for rolling body armors we access the array for item_tier, as that can be changed in ConstructArmorDataOnField to level this down for lower level players
	int i = 0, roll;
	int count = random(1, max_attr) - 1 - (m2 != -1) - (m3 != -1);
	int special_roll = SetupItemImplicit(item_pos, DND_ITEM_FLASK, flask_type, item_tier);

	AddAttributeToFieldItem(item_pos, m1, pnum);
	if(m2 != -1)
		AddAttributeToFieldItem(item_pos, m2, pnum);
	if(m3 != -1)
		AddAttributeToFieldItem(item_pos, m3, pnum);

	while(i < count) {
		do {
			roll = PickRandomAttribute(DND_ITEM_FLASK, flask_type, special_roll, Inventories_On_Field[item_pos].implicit[0].attrib_id);
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
	return !isAlive() || !PlayerInGame(pnum) || PlayerIsSpectator(pnum) || CheckInventory("DnD_IntermissionState");
}

int GetReducedFlaskEffectAmount(int pnum, int flask_id) {
	// this modifier has -25%
	int base = GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_INCCHARGERECOVERY);
	if(base)
		base = DND_FLASK_RECOVERY_REDUCEEFFECT;
	return base;
}

void HandleCommonFlaskActivationEffects(int pnum, int flask_id) {
	int tid = pnum + P_TIDSTART;
	if(GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_IMMUNE_BLEED))
		GiveActorInventory(tid, "RemoveBleed", 1);
	if(GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_IMMUNE_CHILLFREEZE))
		GiveActorInventory(tid, "RemoveChillFreeze", 1);
	if(GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_IMMUNE_POISON))
		GiveActorInventory(tid, "RemovePoison", 1);
	if(GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_IMMUNE_SHOCK))
		GiveActorInventory(tid, "RemoveShock", 1);
	if(GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_IMMUNE_IGNITE))
		GiveActorInventory(tid, "RemoveIgnite", 1);
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

		HandleCommonFlaskActivationEffects(pnum, flask_id);

		// update current charges and SBARINFO hook
		FlaskData[pnum][flask_id].curr_charges -= FlaskData[pnum][flask_id].charges_used;
		SetInventory(StrParam(s:"Flask", d:flask_id + 1, s:"_CurrentCharges"), FlaskData[pnum][flask_id].curr_charges);

		// the duration contains xTICRATE in it
		int total_time = FlaskData[pnum][flask_id].effect_duration / FLASK_RECOVERY_TICRATE;

		// this is the amount that would be given over this duration -- also consider quality effect here
		int toGive_total = GetFlaskData(pnum, flask_id, FlaskData[pnum][flask_id].flask_type, FLASK_DATA_GIVEAMOUNT);
		int rem = GetReducedFlaskEffectAmount(pnum, flask_id);

		if(FlaskData[pnum][flask_id].quality)
			toGive_total = toGive_total * (100 + FlaskData[pnum][flask_id].quality - rem) / 100;

		if((rem = GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_MORERECOVERYONLOWLIFE)) && IsLowLife())
			toGive_total = toGive_total * (100 + rem) / 100;

		// instant recovery check
		if((GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_INSTANTONLOWLIFE) && IsLowLife()) || GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_INSTANTRECOVERY))
			total_time = 1;
		else if((rem = GetFlaskAttributeVal(pnum, flask_id, INV_FLASK_INCRECOVERYRATE)))
			total_time = total_time * 100 / (100 + rem);

		// quality increases amount recovered
		if(Items_Used[pnum][FLASK1_INDEX + flask_id].quality)
			toGive_total = toGive_total * (100 + Items_Used[pnum][FLASK1_INDEX + flask_id].quality) / 100;

		int toGive = toGive_total / total_time;
		int currGiven = 0;
		
		rem = toGive_total % total_time;

		if(!toGive)
			toGive = 1;

		// sbarinfo hook for current tics
		str flask_tics_item = StrParam(s:"Flask", d:flask_id + 1, s:"_TicCounter");
		SetInventory(flask_tics_item, total_time * FLASK_RECOVERY_TICRATE);

		int error = 0;

		while(!CheckFlaskScriptConditions(pnum) && GetActorProperty(0, APROP_HEALTH) < (cap = CheckInventory("PlayerHealthCap")) && FlaskData[pnum][flask_id].curr_tics < total_time) {
			int tmp_give = toGive;

			// balance out if we should give toGive or toGive + 1 per step
			error += rem;
			if(error >= total_time) {
				++tmp_give;
				error -= total_time;
			}

			// max amount to give has been depleted
			currGiven += tmp_give;

			if(GetActorProperty(0, APROP_HEALTH) + tmp_give > cap) {
				// limit reached, stop
				GiveInventory("HealthBonusX", cap - GetActorProperty(0, APROP_HEALTH));
				break;
			}
			else
				GiveInventory("HealthBonusX", tmp_give);

			if(currGiven >= toGive_total)
				break;

			++FlaskData[pnum][flask_id].curr_tics;
			TakeInventory(flask_tics_item, 1);
			delay(const:FLASK_RECOVERY_TICRATE);
		}
		
		FlaskData[pnum][flask_id].curr_tics = 0;

		if(CheckFlaskScriptConditions(pnum))
			Terminate;

		SetInventory(flask_tics_item, 0);

		//printbold(s:"Given ", d:currGiven, s: " out of ", d:toGive_total);
	}
	else {
		HandleCommonFlaskActivationEffects(pnum, flask_id);
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
	int base = 1;
	if(MonsterProperties[m_id].flags & DND_MONFLAG_ISELITE)
		base = 5;
	else if(MonsterProperties[m_id].flags & DND_MONFLAG_ISMAGIC)
		base = 3;
	else if(IsUniqueMonster(MonsterProperties[m_id].id))
		base = 10;

	if(IsMonsterIdBoss(MonsterProperties[m_id].id) || isLegendaryMonster(MonsterProperties[m_id].id))
		base <<= 1;

	GiveFlaskCharges(pnum, base);
}

void CheckFlasksOnHitGain(int pnum) {
	for(int i = 0; i < MAX_FLASK_SLOTS; ++i) {
		int temp = FlaskData[pnum][i].chance_on_hit;
		int amt = 0;
		while(temp > 0) {
			amt += random(1, 100) <= temp;
			temp -= 100;
		}

		if(amt)
			GiveSpecificFlaskCharges(pnum, amt, i);
	}
}

// hook for player doing crit to check the flask state
void CheckFlasksOnCritGain(int pnum) {
	int tid = pnum + P_TIDSTART;
	for(int i = 0; i < MAX_FLASK_SLOTS; ++i) {
		str flask_cd_item = StrParam(s:"Flask", d:i + 1, s:"_CritCooldown");
		if(CheckActorInventory(tid, flask_cd_item))
			continue;
		int temp = FlaskData[pnum][i].chance_on_crit;
		int amt = 0;
		while(temp > 0) {
			amt += random(1, 100) <= temp;
			temp -= 100;
		}

		if(amt) {
			GiveSpecificFlaskCharges(pnum, amt, i);
			GiveActorInventory(tid, flask_cd_item, 1);
		}
	}
}

#endif