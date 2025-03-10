#ifndef DND_INV_GEN_IN
#define DND_INV_GEN_IN

void SpawnArmor(int pnum, int rarity_boost, int tiers = 0, bool noRandomVelXY = false, int extra = -1) {
    int c = CreateItemSpot();
	//Log(s:"alloc armor id: ", d:c, s: " for player: ", d:pnum);
	int id = 0;
	if(c != -1) {
        int type = RollArmorInfo(c, RollItemLevel(), pnum, tiers, extra);

		// tiers need to be 0 for it to be monster drops
//#ifndef ISDEBUGBUILD
		if(!tiers && ((GetCVar("dnd_ignore_dropweights") && random(0, 1)) || RunDefaultDropChance(pnum, UNIQUE_ARMOR_DROPCHANCE * (100 + rarity_boost) / 100)))
//#else
		//if(random(0,1))
//#endif
		{
			id = MakeUnique(c, DND_ITEM_BODYARMOR, pnum);
			SpawnDrop(StrParam(s:"UniqueArmor_", d:id - UNIQUE_BODYARMOR_BEGIN), 16.0, 16, pnum + 1, c, noRandomVelXY);
		}
		else
			SpawnDrop(GetArmorDropClass(type), 16.0, 16, pnum + 1, c, noRandomVelXY);

		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_BODYARMOR);
	}
}

void SpawnBoot(int pnum, int rarity_boost, int unused = 0, bool noRandomVelXY = false, int extra = -1) {
    int c = CreateItemSpot();
	if(c != -1) {
        int type = RollBootInfo(c, RollItemLevel(), pnum);
        // depending on armor type rolled, spawn its appropriate actor
        SpawnDrop(GetBootDropClass(type), 16.0, 16, pnum + 1, c, noRandomVelXY);

		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_BOOT);
	}
}

void SpawnHelm(int pnum, int rarity_boost, int pre_id = -1, bool noRandomVelXY = false, int extra = -1) {
    int c = CreateItemSpot();
	if(c != -1) {
        int type = RollHelmInfo(c, RollItemLevel(), pnum, pre_id);
        // depending on armor type rolled, spawn its appropriate actor
        SpawnDrop(GetHelmDropClass(type), 16.0, 16, pnum + 1, c, noRandomVelXY);

		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_HELM);
	}
}

// monsters dropping charms
void SpawnCharm(int pnum, int rarity_boost, int unused = 0, bool noRandomVelXY = false, int extra = -1) {
	int c = CreateItemSpot();
	if(c != -1) {
		// c is the index on the field now
		#ifndef ISDEBUGBUILD
			if((GetCVar("dnd_ignore_dropweights") && random(0, 1)) || RunDefaultDropChance(pnum, UNIQUE_DROPCHANCE * (100 + rarity_boost) / 100))
		#else
			if(!random(0,3))
		#endif
		{
			MakeUnique(c, DND_ITEM_CHARM, pnum);
			SpawnDrop("UniqueCharmDrop", 16.0, 16, pnum + 1, c, noRandomVelXY);
		}
		else {
			RollCharmInfo(c, RollItemLevel(), pnum);
			SpawnDrop("CharmDrop", 16.0, 16, pnum + 1, c);
		}
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_CHARM);
	}
}

void SpawnUniqueCharm(int pnum, int charm_id, bool noRandomVelXY = false) {
	int c = CreateItemSpot();
	MakeUnique(c, DND_ITEM_CHARM, pnum, charm_id);
	SpawnDrop("UniqueCharmDrop", 16.0, 16, pnum + 1, c, noRandomVelXY);
	SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
	ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_CHARM);
}

void SpawnPowercore(int pnum, int rarity_boost, int unused = 0, bool noRandomVelXY = false, int extra = -1) {
    int c = CreateItemSpot();
	int id = 0;
	if(c != -1) {
		#ifndef ISDEBUGBUILD
			if((GetCVar("dnd_ignore_dropweights") && random(0, 1)) || RunDefaultDropChance(pnum, UNIQUE_ARMOR_DROPCHANCE * (100 + rarity_boost) / 100))
		#else
			if(random(0,1))
		#endif
		{
			id = MakeUnique(c, DND_ITEM_POWERCORE, pnum);
			SpawnDrop(StrParam(s:"PowercoreDrop_Unique", d:id - UNIQUE_POWERCORE_BEGIN), 16.0, 16, pnum + 1, c, noRandomVelXY);
		}
		else
			SpawnDrop(GetPowercoreDropClass(RollPowercoreInfo(c, RollItemLevel(), pnum)), 24.0, 16, pnum + 1, c, noRandomVelXY);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_POWERCORE);
	}
}

void SpawnToken(int pnum, int stack = 0, int unused2 = 0, bool noRandomVelXY = false, int extra = -1) {
	int c = CreateItemSpot();
	if(c != -1) {
		// c is the index on the field now
		// only current token is repair token, so just assume that instead
		int i = 0, w = random(1, MAX_TOKEN_WEIGHT);
		
		for(; i < MAX_TOKENS && TokenWeights[i] < w; ++i);

		if(!stack)
			stack = 1;
		
		RollTokenInfo(c, i, true, stack);
		SpawnDrop(GetInventoryName(i + TOKEN_BEGIN), 24.0, 16, pnum + 1, c, noRandomVelXY);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_TOKEN);
	}
}

void SpawnItemForAll(int type, int repeats = 1, int extra = -1) {
    void function(int, int, int, bool, int)& f = SpawnArmor;
    switch(type) {
        case DND_ITEM_CHARM:
            f = SpawnCharm;
        break;
        case DND_ITEM_BODYARMOR:
            f = SpawnArmor;
        break;
		case DND_ITEM_BOOT:
			f = SpawnBoot;
		break;
        case DND_ITEM_POWERCORE:
            f = SpawnPowercore;
        break;
        case DND_ITEM_TOKEN:
            f = SpawnToken;
        break;
    }

	for(int k = 0; k < repeats; ++k) {
		for(int j = 0; j < MAXPLAYERS; ++j) {
			if(PlayerInGame(j) && !PlayerIsSpectator(j)) {
				if(PlayerActivities[j].loot_penalty)
					continue;
				f(j, 0, 0, 0, extra);
			}
		}
	}
}

str GetItemTagName(int itype, int isubt) {
	switch(itype) {
		case DND_ITEM_CHARM:
		return StrParam(l:StrParam(s:"DND_CHARMTYPE", d:isubt + 1), s:" ", l:"DND_ITEM_CHARM");

		case DND_ITEM_POWERCORE:
		return StrParam(l:StrParam(s:"DND_PCORE", d:isubt + 1));

		case DND_ITEM_BODYARMOR:
		return StrParam(l:StrParam(s:"DND_ARMOR", d:isubt + 1));

		case DND_ITEM_BOOT:
		return StrParam(l:StrParam(s:"DND_BOOT", d:isubt + 1));

		case DND_ITEM_HELM:
		return StrParam(l:StrParam(s:"DND_HELM", d:isubt + 1));
	}
	return "NULL";
}

int SetupItemImplicit(int item_pos, int type, int subtype, int item_tier) {
	int special_roll = 0;
	switch(type) {
        case DND_ITEM_BODYARMOR:
			// implicits that come along with the item always
			switch(subtype) {
				case BODYARMOR_GREEN:
					// 100 armor, 33% damage reduction
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 100, 0, item_tier, 50);
				break;
				case BODYARMOR_YELLOW:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 150, 0, item_tier, 75);
				break;
				case BODYARMOR_BLUE:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 200, 0, item_tier, 100);
				break;
				case BODYARMOR_RED:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 300, 0, item_tier, 100);
				break;

				case BODYARMOR_GUNSLINGER:
					special_roll = PPOWER_CANROLLPHYS;
					GiveImplicitToField(item_pos, INV_IMP_INCMIT, 20.0, PPOWER_CANROLLPHYS, item_tier, 1.75);
				break;
				case BODYARMOR_OCCULT:
					special_roll = PPOWER_CANROLLOCCULT;
					GiveImplicitToField(item_pos, INV_IMP_INCMITSHIELD, 85, PPOWER_CANROLLOCCULT, item_tier, 30);
				break;
				case BODYARMOR_DEMO:
					special_roll = PPOWER_CANROLLEXP;
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 175, PPOWER_CANROLLEXP, item_tier, 100);
				break;
				case BODYARMOR_ENERGY:
					special_roll = PPOWER_CANROLLENERGY;
					GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 85, PPOWER_CANROLLENERGY, item_tier, 30);
				break;
				case BODYARMOR_ELEMENTAL:
					special_roll = PPOWER_CANROLLELEMENTAL;
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 125, PPOWER_CANROLLELEMENTAL, item_tier, 60);
				break;

				case BODYARMOR_MONOLITH:
					GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 115, 0, item_tier, 40);
				break;
				case BODYARMOR_CYBER:
					GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 100, PPOWER_CYBER, item_tier, 40);
				break;
				case BODYARMOR_DUELIST:
					GiveImplicitToField(item_pos, INV_IMP_INCMIT, 20.0, PPOWER_HITSCANPROTECT, item_tier, 2.25);
				break;
				case BODYARMOR_NECRO:
					GiveImplicitToField(item_pos, INV_IMP_INCMITARMOR, 150, PPOWER_SPIKES, item_tier, 60);
				break;
				case BODYARMOR_KNIGHT:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 250, PPOWER_KNIGHTMELEEBONUS, item_tier, 100);
				break;
				case BODYARMOR_RAVAGER:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 200, PPOWER_RAVAGER, item_tier, 80);
				break;
				case BODYARMOR_TANGLEDRIBCAGE:
					GiveImplicitToField(item_pos, INV_IMP_INCMITARMORSHIELD, 35, PPOWER_MAXQUALITYHIGH, item_tier, 10);
				break;

				case BODYARMOR_SYNTHMETAL:
					GiveImplicitToField(item_pos, INV_IMP_INCMITARMOR, 180, PPOWER_LOWERREFLECT, item_tier, 80);
				break;
				case BODYARMOR_LIGHTNINGCOIL:
					GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 80, PPOWER_LIGHTNINGABSORB, item_tier, 25);
				break;
			}
        break;
		case DND_ITEM_BOOT:
			// implicits that come along with the item always
			switch(subtype) {
				case BOOTS_SILVER:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 50, PPOWER_INCMAGICRES, item_tier, 20);
				break;
				case BOOTS_ENGINEER:
					GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 16, PPOWER_INCENERGYRES, item_tier, 5);
				break;
				case BOOTS_INSULATED:
					GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 10, PPOWER_REDUCEDLIGHTNINGTAKEN, item_tier, 5);
				break;
				case BOOTS_PLATED:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 75, 0, item_tier, 25);
				break;
				case BOOTS_POWER:
					GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 25, PPOWER_OVERHEATGOFAST, item_tier, 8);
				break;
				case BOOTS_ENERGY:
					GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 12, PPOWER_REDUCEDSELFDMG, item_tier, 6);
				break;
				case BOOTS_TACTICAL:
					GiveImplicitToField(item_pos, INV_IMP_INCMITARMOR, 45, 0, item_tier, 25);
				break;
				case BOOTS_FUSION:
					GiveImplicitToField(item_pos, INV_IMP_INCMITSHIELD, 20, PPOWER_LESSOVERHEAT, item_tier, 8);
				break;
				case BOOTS_LEATHER:
					GiveImplicitToField(item_pos, INV_IMP_INCMIT, 8.0, 0, item_tier, 1.0);
				break;
				case BOOTS_SNAKESKIN:
					GiveImplicitToField(item_pos, INV_IMP_INCMIT, 4.5, PPOWER_REDUCEDPOISONTAKEN, item_tier, 1.0);
				break;
				case BOOTS_DRAKESKIN:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 35, PPOWER_REDUCEDFIRETAKEN, item_tier, 10);
				break;
			}
		break;
		case DND_ITEM_HELM:
			// implicits that come along with the item always
			switch(subtype) {
				case HELMS_LICH:
					GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 45, PPOWER_PETCAP, item_tier, 18);
				break;
				case HELMS_WARRIOR:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 80, PPOWER_MELEEDAMAGE, item_tier, 40);
				break;
				case HELMS_SYNTHMETAL:
					GiveImplicitToField(item_pos, INV_IMP_INCMIT, 10.0, PPOWER_SYNTHMETALMASK, item_tier, 1.75);
				break;
				case HELMS_KNIGHT:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 75, PPOWER_MELEEIGNORESHIELD, item_tier, 40);
				break;
				case HELMS_PREDATOR:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 80, PPOWER_BOSSTAKEMOREDMG, item_tier, 30);
				break;
				case HELMS_CRUSADER:
					GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 55, PPOWER_UNDEADRECOVERES, item_tier, 25);
				break;
				case HELMS_TACTICAL:
					GiveImplicitToField(item_pos, INV_IMP_INCMITARMOR, 72, PPOWER_PRECISIONCRIT, item_tier, 24);
				break;
				case HELMS_ROBE:
					GiveImplicitToField(item_pos, INV_IMP_INCMITSHIELD, 36, PPOWER_MAGICNEGATION, item_tier, 16);
				break;
			}
		break;
        case DND_ITEM_POWERCORE:
			// implicits that come along with the item always
			switch(subtype) {
				case PCORE_COPPER:
					GiveImplicitToField(item_pos, INV_IMP_POWERCORE, 10, 25, item_tier, 5);
				break;
				case PCORE_GOLD:
					GiveImplicitToField(item_pos, INV_IMP_POWERCORE, 20, 20, item_tier, 7);
				break;
				case PCORE_UNSTABLE:
					GiveImplicitToField(item_pos, INV_IMP_UNSTABLECORE, 15, 0.05 + 0.0275 * item_tier / MAX_CHARM_AFFIXTIERS, item_tier, 8);
				break;
				case PCORE_MOLYBDENUM:
					GiveImplicitToField(item_pos, INV_IMP_POWERCORE, 30, 15, item_tier, 10);
				break;
			}
        break;
	}
	return special_roll;
}

bool IsArmorTypeItem(int type) {
	switch(type) {
		case DND_ITEM_BODYARMOR:
		case DND_ITEM_HELM:
		case DND_ITEM_BOOT:
		case DND_ITEM_POWERCORE:
		return true;
	}
	return false;
}

#endif