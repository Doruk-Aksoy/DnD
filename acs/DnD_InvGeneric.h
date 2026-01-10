#ifndef DND_INV_GEN_IN
#define DND_INV_GEN_IN

#define QUALITY_ITEM_ADD_MIN 1
#define QUALITY_ITEM_ADD_MAX 4

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

		//if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, X) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
		//	SpawnArmorWithMods(pnum, m1, m2, m3, true);
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
	int id = 0;
	if(c != -1) {
		int ilvl = RollItemLevel();
        int type = InitializeBoot(c, ilvl, pnum);

#ifndef ISDEBUGBUILD
		if((GetCVar("dnd_ignore_dropweights") && random(0, 1)) || RunDefaultDropChance(pnum, UNIQUE_ARMOR_DROPCHANCE * (100 + rarity_boost) / 100))
#else
		if(random(0,2))
#endif
		{
			id = MakeUnique(c, DND_ITEM_BOOT, pnum);
			SpawnDrop(StrParam(s:"UniqueBoot_", d:id - UNIQUE_BOOT_BEGIN), 16.0, 16, pnum + 1, c, noRandomVelXY);
		}
		else {
			RollArmorInfo(c, ilvl, pnum, DND_ITEM_BOOT, type, MAX_BOOT_ATTRIB_DEFAULT);
			SpawnDrop(GetBootDropClass(type), 16.0, 16, pnum + 1, c, noRandomVelXY);
		}

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

		//if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, X) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
		//	SpawnBootWithMods(pnum, m1, m2, m3, true);
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

		//if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, X) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
		//	SpawnBootWithMods(pnum, m1, m2, m3, true);
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
			if(!random(0,1))
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

// extra field is the class
void SpawnSpecialtyItem(int pnum, int rarity_boost, int unused = 0, bool noRandomVelXY = false, int extra = -1) {
    int c = CreateItemSpot();
	int id = 0;
	if(c != -1) {
		if(extra == DND_ITEM_SPECIALTY_CYBORG && ((GetCVar("dnd_ignore_dropweights") && random(0, 1)) || RunDefaultDropChance(pnum, UNIQUE_ARMOR_DROPCHANCE * (100 + rarity_boost) / 100))) {
			id = MakeUnique(c, DND_ITEM_SPECIALTY_CYBORG, pnum);
			SpawnDrop(StrParam(s:"PowercoreDrop_Unique", d:id - UNIQUE_POWERCORE_BEGIN), 16.0, 16, pnum + 1, c, noRandomVelXY);
		}
		else {
			SpawnDrop(GetSpecialtyDropClass(extra, RollSpecialtyItemInfo(c, RollItemLevel(), pnum, extra)), 24.0, 16, pnum + 1, c, noRandomVelXY);
		}
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, extra);
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

		case DND_ITEM_SPECIALTY_DOOMGUY:
		case DND_ITEM_SPECIALTY_MARINE:
		case DND_ITEM_SPECIALTY_HOBO:
		case DND_ITEM_SPECIALTY_PUNISHER:
		case DND_ITEM_SPECIALTY_WANDERER:
		case DND_ITEM_SPECIALTY_CYBORG:
		case DND_ITEM_SPECIALTY_BERSERKER:
		case DND_ITEM_SPECIALTY_TRICKSTER:
			extra = type;
			f = SpawnSpecialtyItem;
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

		// specialty
		case DND_ITEM_SPECIALTY_DOOMGUY:
		return StrParam(l:StrParam(s:"DND_SCARD", d:isubt + 1));

		case DND_ITEM_SPECIALTY_MARINE:
		return StrParam(l:StrParam(s:"DND_DOGTAG", d:isubt + 1));

		case DND_ITEM_SPECIALTY_HOBO:
		return StrParam(l:StrParam(s:"DND_SUNGLASS", d:isubt + 1));

		case DND_ITEM_SPECIALTY_PUNISHER:
		return StrParam(l:StrParam(s:"DND_CIGAR", d:isubt + 1));

		case DND_ITEM_SPECIALTY_WANDERER:
		return StrParam(l:StrParam(s:"DND_POWERRING", d:isubt + 1));

		case DND_ITEM_SPECIALTY_CYBORG:
		return StrParam(l:StrParam(s:"DND_PCORE", d:isubt + 1));

		case DND_ITEM_SPECIALTY_BERSERKER:
		return StrParam(l:StrParam(s:"DND_BELT", d:isubt + 1));

		case DND_ITEM_SPECIALTY_TRICKSTER:
		return StrParam(l:StrParam(s:"DND_CLAW", d:isubt + 1));

		case DND_ITEM_BODYARMOR:
		return StrParam(l:StrParam(s:"DND_ARMOR", d:isubt + 1));

		case DND_ITEM_BOOT:
		return StrParam(l:StrParam(s:"DND_BOOT", d:isubt + 1));

		case DND_ITEM_HELM:
		return StrParam(l:StrParam(s:"DND_HELM", d:isubt + 1));
	}
	return "NULL";
}

int SetupItemImplicit(int item_pos, int type, int subtype, int item_tier, bool isForMerchant = false) {
	int special_roll = 0;
	void function(int, int, int, int, int, int)& imp_func = GiveImplicitToField;
	if(isForMerchant)
		imp_func = GiveImplicitToMerchant;

	switch(type) {
        case DND_ITEM_BODYARMOR:
			// implicits that come along with the item always
			switch(subtype) {
				case BODYARMOR_GREEN:
					// 100 armor, 33% damage reduction
					imp_func(item_pos, INV_IMP_INCARMOR, 100, 0, item_tier, 50);
				break;
				case BODYARMOR_YELLOW:
					imp_func(item_pos, INV_IMP_INCARMOR, 150, 0, item_tier, 75);
				break;
				case BODYARMOR_BLUE:
					imp_func(item_pos, INV_IMP_INCARMOR, 200, 0, item_tier, 100);
				break;
				case BODYARMOR_RED:
					imp_func(item_pos, INV_IMP_INCARMOR, 300, 0, item_tier, 100);
				break;

				case BODYARMOR_GUNSLINGER:
					special_roll = INV_IMP_CANROLL_PHYS;
					imp_func(item_pos, INV_IMP_INCMIT, 20.0, 0, item_tier, 1.75);
					imp_func(item_pos, INV_IMP_CANROLL_PHYS, 1, -1, 0, 0);
				break;
				case BODYARMOR_OCCULT:
					special_roll = INV_IMP_CANROLL_MAGIC;
					imp_func(item_pos, INV_IMP_INCMITSHIELD, 85, 0, item_tier, 30);
					imp_func(item_pos, INV_IMP_CANROLL_MAGIC, 1, -1, 0, 0);
				break;
				case BODYARMOR_DEMO:
					special_roll = INV_IMP_CANROLL_EXPLOSIVE;
					imp_func(item_pos, INV_IMP_INCARMOR, 175, 0, item_tier, 100);
					imp_func(item_pos, INV_IMP_CANROLL_EXPLOSIVE, 1, -1, 0, 0);
				break;
				case BODYARMOR_ENERGY:
					special_roll = INV_IMP_CANROLL_ENERGY;
					imp_func(item_pos, INV_IMP_INCSHIELD, 85, 0, item_tier, 30);
					imp_func(item_pos, INV_IMP_CANROLL_ENERGY, 1, -1, 0, 0);
				break;
				case BODYARMOR_ELEMENTAL:
					special_roll = INV_IMP_CANROLL_ELEMENTAL;
					imp_func(item_pos, INV_IMP_INCARMOR, 125, 0, item_tier, 60);
					imp_func(item_pos, INV_IMP_CANROLL_ELEMENTAL, 1, -1, 0, 0);
				break;

				case BODYARMOR_MONOLITH:
					imp_func(item_pos, INV_IMP_INCARMORSHIELD, 115, 0, item_tier, 40);
				break;
				case BODYARMOR_CYBER:
					imp_func(item_pos, INV_IMP_INCSHIELD, 100, 0, item_tier, 40);
					imp_func(item_pos, INV_IMP_DOUBLEESHIELDRECOVERY, 1, -1, 0, 0);
					imp_func(item_pos, INV_CYBERNETIC, 1, -1, 0, 0);
				break;
				case BODYARMOR_DUELIST:
					imp_func(item_pos, INV_IMP_INCMIT, 20.0, 0, item_tier, 2.25);
					imp_func(item_pos, INV_IMP_HANDGUNBONUS, 0.12, 0, item_tier, 0.04);
				break;
				case BODYARMOR_NECRO:
					imp_func(item_pos, INV_IMP_INCMITARMOR, 150, 0, item_tier, 60);
					imp_func(item_pos, INV_IMP_NECROARMOR, 15, 0, item_tier, 3);
				break;
				case BODYARMOR_KNIGHT:
					imp_func(item_pos, INV_IMP_INCARMOR, 250, 0, item_tier, 100);
					imp_func(item_pos, INV_IMP_KNIGHTARMOR, 50, 20, 0, 0);
				break;
				case BODYARMOR_RAVAGER:
					imp_func(item_pos, INV_IMP_INCARMOR, 200, 0, item_tier, 80);
					imp_func(item_pos, INV_IMP_RAVAGER, 25, 15, 0, 0);
				break;
				case BODYARMOR_TANGLEDRIBCAGE:
					imp_func(item_pos, INV_IMP_INCMITARMORSHIELD, 35, 0, item_tier, 10);
					imp_func(item_pos, INV_IMP_QUALITYCAPFIFTY, 1, -1, 0, 0);
				break;

				case BODYARMOR_SYNTHMETAL:
					imp_func(item_pos, INV_IMP_INCMITARMOR, 180, 0, item_tier, 80);
					imp_func(item_pos, INV_IMP_HIGHREFLECTREDUCE, 1, -1, 0, 0);
				break;
				case BODYARMOR_LIGHTNINGCOIL:
					imp_func(item_pos, INV_IMP_INCARMORSHIELD, 80, 0, item_tier, 25);
					imp_func(item_pos, INV_IMP_ABSORBLIGHTNING, 80, 25, 0, 0);
				break;
			}
        break;
		case DND_ITEM_BOOT:
			// implicits that come along with the item always
			switch(subtype) {
				case BOOTS_SILVER:
					imp_func(item_pos, INV_IMP_INCARMOR, 50, 0, item_tier, 20);
					imp_func(item_pos, INV_DMGREDUCE_MAGIC, 3.0, 0, item_tier, 1.0);
				break;
				case BOOTS_ENGINEER:
					imp_func(item_pos, INV_IMP_INCARMORSHIELD, 16, 0, item_tier, 5);
					imp_func(item_pos, INV_DMGREDUCE_ENERGY, 3.0, 0, item_tier, 1.0);
				break;
				case BOOTS_INSULATED:
					imp_func(item_pos, INV_IMP_INCARMORSHIELD, 10, 0, item_tier, 5);
					imp_func(item_pos, INV_IMP_LESSLIGHTNINGTAKEN, 0.1, -1, 0, 0);
				break;
				case BOOTS_PLATED:
					imp_func(item_pos, INV_IMP_INCARMOR, 75, 0, item_tier, 25);
				break;
				case BOOTS_POWER:
					imp_func(item_pos, INV_IMP_INCSHIELD, 25, 0, item_tier, 8);
					imp_func(item_pos, INV_IMP_FASTEROVERHEATDISS, 10, 0, item_tier, 5);
				break;
				case BOOTS_ENERGY:
					imp_func(item_pos, INV_IMP_INCSHIELD, 12, 0, item_tier, 6);
					imp_func(item_pos, INV_IMP_LESSSELFDAMAGETAKEN, 10, -1, 0, 0);
				break;
				case BOOTS_TACTICAL:
					imp_func(item_pos, INV_IMP_INCMITARMOR, 45, 0, item_tier, 25);
				break;
				case BOOTS_FUSION:
					imp_func(item_pos, INV_IMP_INCMITSHIELD, 20, 0, item_tier, 8);
					imp_func(item_pos, INV_REDUCED_OVERHEAT, 8, 0, item_tier, 4);
				break;
				case BOOTS_LEATHER:
					imp_func(item_pos, INV_IMP_INCMIT, 8.0, 0, item_tier, 1.0);
				break;
				case BOOTS_SNAKESKIN:
					imp_func(item_pos, INV_IMP_INCMIT, 4.5, 0, item_tier, 1.0);
					imp_func(item_pos, INV_IMP_LESSPOISONTAKEN, 0.1, -1, 0, 0);
				break;
				case BOOTS_DRAKESKIN:
					imp_func(item_pos, INV_IMP_INCARMOR, 35, 0, item_tier, 10);
					imp_func(item_pos, INV_IMP_LESSFIRETAKEN, 0.1, -1, 0, 0);
				break;
				case BOOTS_WARRIORSABATON:
					imp_func(item_pos, INV_IMP_INCARMOR, 25, 0, item_tier, 15);
					imp_func(item_pos, INV_IMP_STAMINAONKILL, 10, 5, item_tier, 2);
				break;
			}
		break;
		case DND_ITEM_HELM:
			// implicits that come along with the item always
			switch(subtype) {
				case HELMS_LICH:
					imp_func(item_pos, INV_IMP_INCARMORSHIELD, 45, 0, item_tier, 18);
					imp_func(item_pos, INV_IMP_BONUSPETCAP, 1, -1, 0, 0);
				break;
				case HELMS_WARRIOR:
					imp_func(item_pos, INV_IMP_INCARMOR, 80, 0, item_tier, 40);
					imp_func(item_pos, INV_MELEEDAMAGE, 25, -1, 0, 0);
					imp_func(item_pos, INV_MELEERANGE, 15, -1, 0, 0);
				break;
				case HELMS_SYNTHMETAL:
					imp_func(item_pos, INV_IMP_INCMIT, 10.0, 0, item_tier, 1.75);
					imp_func(item_pos, INV_IMP_REDUCEDVISIONIMPAIR, 40, 0, item_tier, 5);
				break;
				case HELMS_KNIGHT:
					imp_func(item_pos, INV_IMP_INCARMOR, 75, 0, item_tier, 40);
					imp_func(item_pos, INV_IMP_MELEEIGNORESSHIELDS, 1, -1, 0, 0);
				break;
				case HELMS_PREDATOR:
					imp_func(item_pos, INV_IMP_INCARMOR, 80, 0, item_tier, 30);
					imp_func(item_pos, INV_IMP_MOREDAMAGETOBOSSES, 5, 0, item_tier, 1);
				break;
				case HELMS_CRUSADER:
					imp_func(item_pos, INV_IMP_INCARMORSHIELD, 55, 0, item_tier, 25);
					imp_func(item_pos, INV_IMP_RECOVERESONUNDEADKILL, 1, -1, 0, 0);
				break;
				case HELMS_TACTICAL:
					imp_func(item_pos, INV_IMP_INCMITARMOR, 72, 0, item_tier, 24);
					imp_func(item_pos, INV_IMP_PRECISIONCRITBONUS, 0.02, 0, item_tier, 0.01);
				break;
				case HELMS_ROBE:
					imp_func(item_pos, INV_IMP_INCMITSHIELD, 36, 0, item_tier, 16);
					imp_func(item_pos, INV_MAGIC_NEGATION, 5, 0, item_tier, 2);
				break;
				case HELMS_ELDER:
					special_roll = random(INV_CORR_MAXFRENZY, INV_CORR_MAXPOWER);

					// add these first so they can be corruption replaced
					if(special_roll == INV_CORR_MAXFRENZY)
						imp_func(item_pos, INV_IMP_INCMIT, 5.0, 0, item_tier, 1.25);
					else if(special_roll == INV_CORR_MAXENDURANCE)
						imp_func(item_pos, INV_IMP_INCARMOR, 50, 0, item_tier, 25);
					else
						imp_func(item_pos, INV_IMP_INCSHIELD, 25, 0, item_tier, 8);

					imp_func(item_pos, special_roll, 1, 0, item_tier, 0);
				break;
			}
		break;
		case DND_ITEM_SPECIALTY_DOOMGUY:
			switch(subtype) {
				case SLAYERCARD_STR:
					imp_func(item_pos, INV_IMP_INCARMORSHIELD, 25, 0, item_tier, 10);
					imp_func(item_pos, INV_IMP_PERCENTSTR, 5, 0, item_tier, 2);
				break;
				case SLAYERCARD_DEX:
					imp_func(item_pos, INV_IMP_INCARMORSHIELD, 25, 0, item_tier, 10);
					imp_func(item_pos, INV_IMP_PERCENTDEX, 5, 0, item_tier, 2);
				break;
				case SLAYERCARD_INT:
					imp_func(item_pos, INV_IMP_INCARMORSHIELD, 25, 0, item_tier, 10);
					imp_func(item_pos, INV_IMP_PERCENTINT, 5, 0, item_tier, 2);
				break;
			}
		break;

		case DND_ITEM_SPECIALTY_MARINE:
			switch(subtype) {
				case DOGTAG_PRIVATE:
					imp_func(item_pos, INV_IMP_INCARMOR, 45, 0, item_tier, 15);
					imp_func(item_pos, INV_IMP_LESSPOISONTAKEN, 0.05, 0, item_tier, 0.025);
				break;
				case DOGTAG_SGT:
					imp_func(item_pos, INV_IMP_INCARMOR, 45, 0, item_tier, 15);
					imp_func(item_pos, INV_DMGREDUCE_PHYS, 5.0, 0, item_tier, 2.5);
				break;
				case DOGTAG_CORPORAL:
					imp_func(item_pos, INV_IMP_INCARMOR, 45, 0, item_tier, 15);
					imp_func(item_pos, INV_IMP_LESSFIRETAKEN, 0.05, 0, item_tier, 0.025);
				break;
			}
		break;

		case DND_ITEM_SPECIALTY_HOBO:
			switch(subtype) {
				case SUNGLASS_BLACK:
					imp_func(item_pos, INV_IMP_INCMIT, 1.5, 0, item_tier, 0.75);
					imp_func(item_pos, INV_PELLET_INCREASE, 0.05, 0, item_tier, 0.015);
				break;
				case SUNGLASS_PINK:
					imp_func(item_pos, INV_IMP_INCMIT, 3.0, 0, item_tier, 0.75);
					imp_func(item_pos, INV_IMP_REDUCEDSLOWSHOTGUNS, 10, 0, item_tier, 5);
				break;
				case SUNGLASS_GREEN:
					imp_func(item_pos, INV_IMP_INCMIT, 3.0, 0, item_tier, 0.75);
					imp_func(item_pos, INV_IMP_AMMOGAIN_SHOTGUNS, 5, 0, item_tier, 1);
				break;
			}
		break;

		case DND_ITEM_SPECIALTY_PUNISHER:
			switch(subtype) {
				case CIGAR_LIGHT:
					imp_func(item_pos, INV_IMP_INCMIT, 2.0, 0, item_tier, 0.75);
					imp_func(item_pos, INV_LIFESTEAL, 1.0, 0, item_tier, 0.025);
				break;
				case CIGAR_MID:
					imp_func(item_pos, INV_SHIELD_INCREASE, 10, 0, item_tier, 5);
					imp_func(item_pos, INV_INCKILLINGSPREE, 10, 0, item_tier, 5);
				break;
				case CIGAR_HEAVY:
					imp_func(item_pos, INV_IMP_INCARMOR, 25, 0, item_tier, 5);
					imp_func(item_pos, INV_LIFESTEAL_RECOVERY, 10, 0, item_tier, 5);
				break;
			}
		break;

		case DND_ITEM_SPECIALTY_WANDERER:
			switch(subtype) {
				case POWERRING_GREEN:
					imp_func(item_pos, INV_DMGREDUCE_MAGIC, 5.0, 0, item_tier, 1.5);
					imp_func(item_pos, INV_IMP_ONKILL_FRENZY, 10, 0, item_tier, 1);
				break;
				case POWERRING_RED:
					imp_func(item_pos, INV_DMGREDUCE_MAGIC, 5.0, 0, item_tier, 1.5);
					imp_func(item_pos, INV_IMP_ONKILL_ENDURANCE, 10, 0, item_tier, 1);
				break;
				case POWERRING_BLUE:
					imp_func(item_pos, INV_DMGREDUCE_MAGIC, 5.0, 0, item_tier, 1.5);
					imp_func(item_pos, INV_IMP_ONKILL_POWER, 10, 0, item_tier, 1);
				break;
			}
		break;

        case DND_ITEM_SPECIALTY_CYBORG:
			switch(subtype) {
				case PCORE_COPPER:
					imp_func(item_pos, INV_IMP_POWERCORE, 10, 25, item_tier, 5);
				break;
				case PCORE_GOLD:
					imp_func(item_pos, INV_IMP_POWERCORE, 20, 20, item_tier, 7);
				break;
				case PCORE_UNSTABLE:
					imp_func(item_pos, INV_IMP_UNSTABLECORE, 15, 0.05 + 0.0275 * item_tier / MAX_CHARM_AFFIXTIERS, item_tier, 8);
				break;
				case PCORE_MOLYBDENUM:
					imp_func(item_pos, INV_IMP_POWERCORE, 30, 15, item_tier, 10);
				break;
			}
        break;

		case DND_ITEM_SPECIALTY_BERSERKER:
			switch(subtype) {
				case BELT_SASH:
					imp_func(item_pos, INV_IMP_INCMIT, 2.0, 0, item_tier, 0.75);
					imp_func(item_pos, INV_PERCENTPHYS_DAMAGE, 10, 0, item_tier, 8);
				break;
				case BELT_HEAVY:
					imp_func(item_pos, INV_IMP_INCARMOR, 40, 0, item_tier, 10);
					imp_func(item_pos, INV_PERCENT_KNOCKBACKRESIST, 15, 0, item_tier, 5);
				break;
				case BELT_LIGHT:
					imp_func(item_pos, INV_IMP_INCMITARMOR, 20, 0, item_tier, 5);
					imp_func(item_pos, INV_SPEED_INCREASE, 0.05, 0, item_tier, 0.02);
				break;
			}
        break;

		case DND_ITEM_SPECIALTY_TRICKSTER:
			switch(subtype) {
				case CLAW_RAKE:
					imp_func(item_pos, INV_IMP_INCMIT, 2.0, 0, item_tier, 0.75);
					imp_func(item_pos, INV_CRITDAMAGE_INCREASE, 10, 0, item_tier, 5);
				break;
				case CLAW_CESTUS:
					imp_func(item_pos, INV_IMP_INCARMOR, 40, 0, item_tier, 10);
					imp_func(item_pos, INV_MITEFFECT_INCREASE, 5.0, 0, item_tier, 1.0);
				break;
				case CLAW_KATAR:
					imp_func(item_pos, INV_IMP_INCMIT, 2.0, 0, item_tier, 0.75);
					imp_func(item_pos, INV_IMP_PHASINGTIME, 15, 0, item_tier, 2);
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