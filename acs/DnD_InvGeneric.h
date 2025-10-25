#ifndef DND_INV_GEN_IN
#define DND_INV_GEN_IN

#define QUALITY_ITEM_ADD_MIN 1
#define QUALITY_ITEM_ADD_MAX 4

#define DND_INCURSION_ITEMCHANCE 10 // 10% chance for an item to have an incursion mod on it

void SpawnArmor(int pnum, int rarity_boost, int tiers = 0, bool noRandomVelXY = false, int extra = -1) {
    int c = CreateItemSpot();
	//Log(s:"alloc armor id: ", d:c, s: " for player: ", d:pnum);
	int id = 0;
	if(c != -1) {
		int ilvl = RollItemLevel();
        int type = InitializeArmor(c, ilvl, pnum, tiers, extra);

		// tiers need to be 0 for it to be monster drops
#ifndef ISDEBUGBUILD
		if(!tiers && ((GetCVar("dnd_ignore_dropweights") && random(0, 1)) || RunDefaultDropChance(pnum, UNIQUE_ARMOR_DROPCHANCE * (100 + rarity_boost) / 100)))
#else
		if(random(0,1))
#endif
		{
			id = MakeUnique(c, DND_ITEM_BODYARMOR, pnum);
			SpawnDrop(StrParam(s:"UniqueArmor_", d:id - UNIQUE_BODYARMOR_BEGIN), 16.0, 16, pnum + 1, c, noRandomVelXY);
		}
		else {
			RollArmorInfo(c, ilvl, pnum, DND_ITEM_BODYARMOR, type, MAX_ARMOR_ATTRIB_DEFAULT);
			SpawnDrop(GetArmorDropClass(type), 16.0, 16, pnum + 1, c, noRandomVelXY);
		}

		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_BODYARMOR);
	}
}

void SpawnArmorWithMods(int pnum, int m1, int m2 = -1, int m3 = -1, bool noRepeat = false) {
	int c = CreateItemSpot();
	if(c != -1) {
		int ilvl = RollItemLevel();
        int type = InitializeArmor(c, ilvl, pnum, 0, -1);

		RollArmorInfoWithMods(c, ilvl, pnum, DND_ITEM_BODYARMOR, type, MAX_ARMOR_ATTRIB_DEFAULT, m1, m2, m3);
		SpawnDrop(GetArmorDropClass(type), 16.0, 16, pnum + 1, c, false);

		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_BODYARMOR);

		if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, STAT_LUCK) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
			SpawnArmorWithMods(pnum, m1, m2, m3, true);
	}
}

void SpawnArmorWithMods_ForAll(int m1, int m2 = -1, int m3 = -1, bool noRepeat = false) {
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && !PlayerIsSpectator(i))
			SpawnArmorWithMods(i, m1, m2, m3, noRepeat);
	}
}

void SpawnBoot(int pnum, int rarity_boost, int unused = 0, bool noRandomVelXY = false, int extra = -1) {
    int c = CreateItemSpot();
	if(c != -1) {
		int ilvl = RollItemLevel();
        int type = InitializeBoot(c, ilvl, pnum);
		RollArmorInfo(c, ilvl, pnum, DND_ITEM_BOOT, type, MAX_BOOT_ATTRIB_DEFAULT);
        // depending on armor type rolled, spawn its appropriate actor
        SpawnDrop(GetBootDropClass(type), 16.0, 16, pnum + 1, c, noRandomVelXY);

		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_BOOT);
	}
}

void SpawnBootWithMods(int pnum, int m1, int m2 = -1, int m3 = -1, bool noRepeat = false) {
	int c = CreateItemSpot();
	if(c != -1) {
		int ilvl = RollItemLevel();
		int type = InitializeBoot(c, ilvl, pnum);
		RollArmorInfoWithMods(c, ilvl, pnum, DND_ITEM_BOOT, type, MAX_ARMOR_ATTRIB_DEFAULT, m1, m2, m3);
		// depending on armor type rolled, spawn its appropriate actor
		SpawnDrop(GetBootDropClass(type), 16.0, 16, pnum + 1, c, false);

		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_BOOT);

		if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, STAT_LUCK) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
			SpawnBootWithMods(pnum, m1, m2, m3, true);
	}
}

void SpawnBootWithMods_ForAll(int m1, int m2 = -1, int m3 = -1, bool noRepeat = false) {
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && !PlayerIsSpectator(i))
			SpawnBootWithMods(i, m1, m2, m3, noRepeat);
	}
}

void SpawnHelm(int pnum, int rarity_boost, int pre_id = -1, bool noRandomVelXY = false, int extra = -1) {
    int c = CreateItemSpot();
	if(c != -1) {
		int ilvl = RollItemLevel();
        int type = InitializeHelm(c, ilvl, pnum, pre_id);
		RollArmorInfo(c, ilvl, pnum, DND_ITEM_HELM, type, MAX_HELM_ATTRIB_DEFAULT);
        // depending on armor type rolled, spawn its appropriate actor
        SpawnDrop(GetHelmDropClass(type), 16.0, 16, pnum + 1, c, noRandomVelXY);

		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_HELM);
	}
}

void SpawnHelmWithMods(int pnum, int m1, int m2 = -1, int m3 = -1, bool noRepeat = false) {
	int c = CreateItemSpot();
	if(c != -1) {
		int ilvl = RollItemLevel();
		int type = InitializeHelm(c, ilvl, pnum, -1);
		RollArmorInfoWithMods(c, ilvl, pnum, DND_ITEM_HELM, type, MAX_ARMOR_ATTRIB_DEFAULT, m1, m2, m3);
		// depending on armor type rolled, spawn its appropriate actor
		SpawnDrop(GetBootDropClass(type), 16.0, 16, pnum + 1, c, false);

		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_HELM);

		if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, STAT_LUCK) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
			SpawnBootWithMods(pnum, m1, m2, m3, true);
	}
}

void SpawnHelmWithMods_ForAll(int m1, int m2 = -1, int m3 = -1, bool noRepeat = false) {
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && !PlayerIsSpectator(i))
			SpawnHelmWithMods(i, m1, m2, m3, noRepeat);
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
			id = MakeUnique(c, DND_ITEM_SPECIALTY_CYBORG, pnum);
			SpawnDrop(StrParam(s:"PowercoreDrop_Unique", d:id - UNIQUE_POWERCORE_BEGIN), 16.0, 16, pnum + 1, c, noRandomVelXY);
		}
		else
			SpawnDrop(GetPowercoreDropClass(RollPowercoreInfo(c, RollItemLevel(), pnum)), 24.0, 16, pnum + 1, c, noRandomVelXY);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_SPECIALTY_CYBORG);
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
		case DND_ITEM_HELM:
			f = SpawnHelm;
		break;
        case DND_ITEM_SPECIALTY_CYBORG:
            f = SpawnPowercore;
        break;
        case DND_ITEM_TOKEN:
            f = SpawnToken;
        break;
		default:
		return;
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

// won't spawn powercores
void SpawnItemForAll_WithMods(int type, int repeats = 1, int extra = -1, int m1 = -1, int m2 = -1, int m3 = -1) {
    void function(int, int, int, bool)& f = SpawnArmorWithMods_ForAll;
    switch(type) {
        case DND_ITEM_CHARM:
            f = SpawnCharmWithMods_ForAll;
        break;
        case DND_ITEM_BODYARMOR:
            f = SpawnArmorWithMods_ForAll;
        break;
		case DND_ITEM_BOOT:
			f = SpawnBootWithMods_ForAll;
		break;
		case DND_ITEM_HELM:
			f = SpawnHelmWithMods_ForAll;
		break;
		default:
		return;
    }

	for(int k = 0; k < repeats; ++k)
		f(m1, m2, m3, false);
}

str GetItemTagName(int itype, int isubt) {
	switch(itype) {
		case DND_ITEM_CHARM:
		return StrParam(l:StrParam(s:"DND_CHARMTYPE", d:isubt + 1), s:" ", l:"DND_ITEM_CHARM");

		case DND_ITEM_SPECIALTY_CYBORG:
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
					special_roll = INV_IMP_CANROLL_PHYS;
					GiveImplicitToField(item_pos, INV_IMP_INCMIT, 20.0, 0, item_tier, 1.75);
					GiveImplicitToField(item_pos, INV_IMP_CANROLL_PHYS, 1);
				break;
				case BODYARMOR_OCCULT:
					special_roll = INV_IMP_CANROLL_MAGIC;
					GiveImplicitToField(item_pos, INV_IMP_INCMITSHIELD, 85, 0, item_tier, 30);
					GiveImplicitToField(item_pos, INV_IMP_CANROLL_MAGIC, 1);
				break;
				case BODYARMOR_DEMO:
					special_roll = INV_IMP_CANROLL_EXPLOSIVE;
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 175, 0, item_tier, 100);
					GiveImplicitToField(item_pos, INV_IMP_CANROLL_EXPLOSIVE, 1);
				break;
				case BODYARMOR_ENERGY:
					special_roll = INV_IMP_CANROLL_ENERGY;
					GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 85, 0, item_tier, 30);
					GiveImplicitToField(item_pos, INV_IMP_CANROLL_ENERGY, 1);
				break;
				case BODYARMOR_ELEMENTAL:
					special_roll = INV_IMP_CANROLL_ELEMENTAL;
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 125, 0, item_tier, 60);
					GiveImplicitToField(item_pos, INV_IMP_CANROLL_ELEMENTAL, 1);
				break;

				case BODYARMOR_MONOLITH:
					GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 115, 0, item_tier, 40);
				break;
				case BODYARMOR_CYBER:
					GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 100, 0, item_tier, 40);
					GiveImplicitToField(item_pos, INV_IMP_DOUBLEESHIELDRECOVERY, 1);
					GiveImplicitToField(item_pos, INV_CYBERNETIC, 1);
				break;
				case BODYARMOR_DUELIST:
					GiveImplicitToField(item_pos, INV_IMP_INCMIT, 20.0, 0, item_tier, 2.25);
					GiveImplicitToField(item_pos, INV_IMP_HANDGUNBONUS, 0.12, 0, item_tier, 0.04);
				break;
				case BODYARMOR_NECRO:
					GiveImplicitToField(item_pos, INV_IMP_INCMITARMOR, 150, 0, item_tier, 60);
					GiveImplicitToField(item_pos, INV_IMP_NECROARMOR, 15, 0, item_tier, 3);
				break;
				case BODYARMOR_KNIGHT:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 250, 0, item_tier, 100);
					GiveImplicitToField(item_pos, INV_IMP_KNIGHTARMOR, 50, 20);
				break;
				case BODYARMOR_RAVAGER:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 200, 0, item_tier, 80);
					GiveImplicitToField(item_pos, INV_IMP_RAVAGER, 25, 15);
				break;
				case BODYARMOR_TANGLEDRIBCAGE:
					GiveImplicitToField(item_pos, INV_IMP_INCMITARMORSHIELD, 35, 0, item_tier, 10);
					GiveImplicitToField(item_pos, INV_IMP_QUALITYCAPFIFTY, 1);
				break;

				case BODYARMOR_SYNTHMETAL:
					GiveImplicitToField(item_pos, INV_IMP_INCMITARMOR, 180, 0, item_tier, 80);
					GiveImplicitToField(item_pos, INV_IMP_HIGHREFLECTREDUCE, 1);
				break;
				case BODYARMOR_LIGHTNINGCOIL:
					GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 80, 0, item_tier, 25);
					GiveImplicitToField(item_pos, INV_IMP_ABSORBLIGHTNING, 80, 25);
				break;
			}
        break;
		case DND_ITEM_BOOT:
			// implicits that come along with the item always
			switch(subtype) {
				case BOOTS_SILVER:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 50, 0, item_tier, 20);
					GiveImplicitToField(item_pos, INV_DMGREDUCE_MAGIC, 3.0, 0, item_tier, 1.0);
				break;
				case BOOTS_ENGINEER:
					GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 16, 0, item_tier, 5);
					GiveImplicitToField(item_pos, INV_DMGREDUCE_ENERGY, 3.0, 0, item_tier, 1.0);
				break;
				case BOOTS_INSULATED:
					GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 10, 0, item_tier, 5);
					GiveImplicitToField(item_pos, INV_IMP_LESSLIGHTNINGTAKEN, 10);
				break;
				case BOOTS_PLATED:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 75, 0, item_tier, 25);
				break;
				case BOOTS_POWER:
					GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 25, 0, item_tier, 8);
					GiveImplicitToField(item_pos, INV_IMP_FASTEROVERHEATDISS, 10, 0, item_tier, 5);
				break;
				case BOOTS_ENERGY:
					GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 12, 0, item_tier, 6);
					GiveImplicitToField(item_pos, INV_IMP_LESSSELFDAMAGETAKEN, 10);
				break;
				case BOOTS_TACTICAL:
					GiveImplicitToField(item_pos, INV_IMP_INCMITARMOR, 45, 0, item_tier, 25);
				break;
				case BOOTS_FUSION:
					GiveImplicitToField(item_pos, INV_IMP_INCMITSHIELD, 20, 0, item_tier, 8);
					GiveImplicitToField(item_pos, INV_REDUCED_OVERHEAT, 8, 0, item_tier, 4);
				break;
				case BOOTS_LEATHER:
					GiveImplicitToField(item_pos, INV_IMP_INCMIT, 8.0, 0, item_tier, 1.0);
				break;
				case BOOTS_SNAKESKIN:
					GiveImplicitToField(item_pos, INV_IMP_INCMIT, 4.5, 0, item_tier, 1.0);
					GiveImplicitToField(item_pos, INV_IMP_LESSPOISONTAKEN, 10);
				break;
				case BOOTS_DRAKESKIN:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 35, 0, item_tier, 10);
					GiveImplicitToField(item_pos, INV_IMP_LESSFIRETAKEN, 10);
				break;
			}
		break;
		case DND_ITEM_HELM:
			// implicits that come along with the item always
			switch(subtype) {
				case HELMS_LICH:
					GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 45, 0, item_tier, 18);
					GiveImplicitToField(item_pos, INV_IMP_BONUSPETCAP, 1);
				break;
				case HELMS_WARRIOR:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 80, 0, item_tier, 40);
					GiveImplicitToField(item_pos, INV_MELEEDAMAGE, 25);
					GiveImplicitToField(item_pos, INV_MELEERANGE, 15);
				break;
				case HELMS_SYNTHMETAL:
					GiveImplicitToField(item_pos, INV_IMP_INCMIT, 10.0, 0, item_tier, 1.75);
					GiveImplicitToField(item_pos, INV_IMP_REDUCEDVISIONIMPAIR, 40, 0, item_tier, 5);
				break;
				case HELMS_KNIGHT:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 75, 0, item_tier, 40);
					GiveImplicitToField(item_pos, INV_IMP_MELEEIGNORESSHIELDS, 1);
				break;
				case HELMS_PREDATOR:
					GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 80, 0, item_tier, 30);
					GiveImplicitToField(item_pos, INV_IMP_MOREDAMAGETOBOSSES, 5, 0, item_tier, 1);
				break;
				case HELMS_CRUSADER:
					GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 55, 0, item_tier, 25);
					GiveImplicitToField(item_pos, INV_IMP_RECOVERESONUNDEADKILL, 1);
				break;
				case HELMS_TACTICAL:
					GiveImplicitToField(item_pos, INV_IMP_INCMITARMOR, 72, 0, item_tier, 24);
					GiveImplicitToField(item_pos, INV_IMP_PRECISIONCRITBONUS, 0.02, 0, item_tier, 0.01);
				break;
				case HELMS_ROBE:
					GiveImplicitToField(item_pos, INV_IMP_INCMITSHIELD, 36, 0, item_tier, 16);
					GiveImplicitToField(item_pos, INV_MAGIC_NEGATION, 5, 0, item_tier, 2);
				break;
			}
		break;
        case DND_ITEM_SPECIALTY_CYBORG:
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
		case DND_ITEM_SPECIALTY_CYBORG:
		return true;
	}
	return false;
}

#endif