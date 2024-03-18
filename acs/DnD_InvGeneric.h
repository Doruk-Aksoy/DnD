#ifndef DND_INV_GEN_IN
#define DND_INV_GEN_IN

void SpawnArmor(int pnum, int rarity_boost, int tiers = 0, bool noRandomVelXY = false) {
    int c = CreateItemSpot();
	if(c != -1) {
        int type = RollArmorInfo(c, RollItemLevel(), pnum, tiers);
        // depending on armor type rolled, spawn its appropriate actor
        SpawnDrop(GetArmorDropClass(type), 16.0, 16, pnum + 1, c, noRandomVelXY);

		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_BODYARMOR);
	}
}

void SpawnBoot(int pnum, int rarity_boost, int unused = 0, bool noRandomVelXY = false) {
    int c = CreateItemSpot();
	if(c != -1) {
        int type = RollBootInfo(c, RollItemLevel(), pnum);
        // depending on armor type rolled, spawn its appropriate actor
        SpawnDrop(GetBootDropClass(type), 16.0, 16, pnum + 1, c, noRandomVelXY);

		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_BOOT);
	}
}

// monsters dropping charms
void SpawnCharm(int pnum, int rarity_boost, int unused = 0, bool noRandomVelXY = false) {
	int c = CreateItemSpot();
	if(c != -1) {
		// c is the index on the field now
		#ifndef ISDEBUGBUILD
			if((GetCVar("dnd_ignore_dropweights") && random(0, 1)) || RunDefaultDropChance(pnum, UNIQUE_DROPCHANCE * (100 + rarity_boost) / 100))
		#else
			if(random(0,1))
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

void SpawnPowercore(int pnum, int rarity_boost, int unused = 0, bool noRandomVelXY = false) {
    int c = CreateItemSpot();
	if(c != -1) {
        int type = RollPowercoreInfo(c, RollItemLevel(), pnum);

        // depending on armor type rolled, spawn its appropriate actor
		SpawnDrop(GetPowercoreDropClass(type), 24.0, 16, pnum + 1, c, noRandomVelXY);

		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_POWERCORE);
	}
}

void SpawnToken(int pnum, int unused1 = 0, int unused2 = 0, bool noRandomVelXY = false) {
	int c = CreateItemSpot();
	if(c != -1) {
		// c is the index on the field now
		// only current token is repair token, so just assume that instead
		int i = 0, w = random(1, MAX_TOKEN_WEIGHT);
		
		for(; i < MAX_TOKENS && TokenWeights[i] < w; ++i);
		
		RollTokenInfo(c, i, true);
		SpawnDrop( GetInventoryName(i + TOKEN_BEGIN), 24.0, 16, pnum + 1, c, noRandomVelXY);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_TOKEN);
	}
}

void SpawnItemForAll(int type, int repeats = 1) {
    void function(int, int, int, bool)& f = SpawnArmor;
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
			if(PlayerInGame(j) && !PlayerIsSpectator(j))
				f(j, 0, 0, 0);
		}
	}
}

#endif