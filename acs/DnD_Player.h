#ifndef DND_PLAYER_IN
#define DND_PLAYER_IN

typedef struct {
    alias_table_T? orb_table;
    alias_table_T? item_attributes_table;

    bool tables_exist;
} player_alias_tables_T;

// this will store ALL of alias tables that are needed for a player to roll things with specific modifiers applied to certain weights still in O(1). +1 for Merchant
global player_alias_tables_T 20: PlayerAliasTables[MAXPLAYERS + 1];

void SetupPlayerAliasTables(int pnum) {
    int i;

    // this is more of a safety measure
    if(!PlayerAliasTables[pnum].tables_exist) {
        PlayerAliasTables[pnum].orb_table = CreateAliasTable(DND_NORMAL_DROP_ORBCOUNT);

        // copy orb weights into this -- include global modifier here as early as possible
        for(i = 0; i < DND_NORMAL_DROP_ORBCOUNT; ++i)
            PlayerAliasTables[pnum].orb_table.weights[i] = GetOrbDropWeight(i);

        PlayerAliasTables[pnum].item_attributes_table = CreateAliasTable(NORMAL_ATTRIBUTE_COUNT);

        // copy attribute weights into this
        for(i = 0; i < NORMAL_ATTRIBUTE_COUNT; ++i)
            PlayerAliasTables[pnum].item_attributes_table.weights[i] = ItemModTable[i].weight;

        PlayerAliasTables[pnum].tables_exist = true;
    }
}

void ClearPlayerAliasTables(int pnum) {
    // this is more of a safety measure
    if(PlayerAliasTables[pnum].tables_exist) {
        //Log(s:"Clearing table for player ", d:pnum);
        FreeAliasTable(PlayerAliasTables[pnum].orb_table);
        FreeAliasTable(PlayerAliasTables[pnum].item_attributes_table);
        PlayerAliasTables[pnum].tables_exist = false;
    }
}

int PickPlayerOrb(int pnum) {
    return PickFromAliasTable(PlayerAliasTables[pnum].orb_table);
}

int PickPlayerAttribute(int pnum) {
    return PickFromAliasTable(PlayerAliasTables[pnum].item_attributes_table);
}

// this is for improving weights of rarer orbs after tables have been built
void ReadjustOrbWeightsForRarerOnes(int pnum) {
    // include anything that the player themselves might have and anything the map modifiers affect --- exclude MAXPLAYERS pnum from player stuff checking
    int boost = HasDungeonUpside(DUN_UPSIDE_RARERORBS);
    int i;

    if(boost) {
        //Log(s:"Boosting orb weights by ", d:boost, s:"%");
        for(i = 0; i < DND_NORMAL_DROP_ORBCOUNT; ++i) {
            if(IsLowTierOrb(i))
                continue;

            PlayerAliasTables[pnum].orb_table.weights[i] = GetOrbDropWeight(i) * (100 + boost) / 100;
            //Log(s:"New weight on ", l:GetInventoryTag(DND_ISUBT_ORB_0 + i), s:" is: ", d:PlayerAliasTables[pnum].orb_table.weights[i]);
        }

        MarkAliasTableDirty(PlayerAliasTables[pnum].orb_table);
    }
}

void MarkAllPlayerOrbTablesDirty() {
    for(int i = 0; i < MAXPLAYERS; ++i) {
        if(PlayerInGame(i) && !PlayerIsSpectator(i)) {
            // reset the weights
            // copy orb weights into this -- include global modifier here as early as possible
            for(int j = 0; j < DND_NORMAL_DROP_ORBCOUNT; ++j)
                PlayerAliasTables[i].orb_table.weights[j] = GetOrbDropWeight(j);

            MarkAliasTableDirty(PlayerAliasTables[i].orb_table);
        }
    }
}

void TestAliasTable(int pnum)  {
    int i = 200;
    int orbs[DND_NORMAL_DROP_ORBCOUNT] = { 0 };
    while(i--) {
        ++orbs[PickPlayerOrb(pnum)];
    }

    for(i = 0; i < DND_NORMAL_DROP_ORBCOUNT; ++i)
        Log(s:"Picked ", s:GetInventoryName(i + ORBS_BEGIN), s:": ", d:orbs[i]);
}

#endif