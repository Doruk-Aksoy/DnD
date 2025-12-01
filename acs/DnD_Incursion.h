#ifndef DND_INCURSION_IN
#define DND_INCURSION_IN

#define DND_INCURSION_CHANCE 0.2
#define DND_INCURSION_ITEMCHANCE 0.05 // 5% chance for an item to have an incursion mod on it

bool RollIncursionItemChance() {
    return random(0, 1.0) <= DND_INCURSION_ITEMCHANCE;
}

enum {
    DND_INCURSION_FAILED = -1,
    DND_INCURSION_NA = 0,
    DND_INCURSION_ON_CHEX,
};
#define DND_INCURSION_THEME_BEGIN DND_INCURSION_ON_CHEX
#define DND_INCURSION_THEME_END DND_INCURSION_ON_CHEX

int RollIncursionTheme() {
    return random(DND_INCURSION_THEME_BEGIN, DND_INCURSION_THEME_END);
}

bool IsIncursionActive() {
    return MapData[DND_MAPDATA_INCURSIONSTATE] >= DND_INCURSION_ON_CHEX;
}

str GetIncursionSpawnerLabel() {
    switch(MapData[DND_MAPDATA_INCURSIONSTATE]) {
        case DND_INCURSION_ON_CHEX:
        return "Incursion_Chex";
    }

    // not valid, so vanilla
    return "Vanilla";
}

#define DND_INCURSION_MARKER_MERGECOUNT 4 // if this many markers have spawned within the radius, they'll merge to form a portal -- this is technically 3, as the marker itself counts as 1 too
#define DND_INCURSION_MERGERADIUS 640.0 // the maximum radius in which incursion markers can be merged
#define DND_INCURSION_BASE_MARKERCHANCE 0.75 // 17.5%
#define DND_INCURSION_MIN_MARKERCHANCE 0.075
#define DND_INCURSION_BONUSMULT 0.33 // up to 33% increase the more it processes the monster list (to help even the odds and avoid "drought" situations)
#define DND_INCURSION_IMPROVETHRESHOLD 500
#define DND_INCURSION_REDUCETHRESHOLD 700
#define DND_INCURSION_IMPROVEPER 250
#define DND_INCURSION_MARKERIMPROVEPCT 0.025
#define DND_INCURSION_SPAWNRADIUS 384.0

enum {
    DND_INCURSION_NONE,
    DND_INCURSION_CHEX,

    DND_INCURSION_LIMIT
};
#define DND_INCURSION_BEGIN DND_INCURSION_CHEX
#define DND_INCURSION_END DND_INCURSION_CHEX

// if the map isn't very dense, increased odds, if the map is dense the odds are reduced (slaughter map cases)
int GetBaseMarkerSpawnChance(int max_monster_count) {
    int base = DND_INCURSION_BASE_MARKERCHANCE;

    if(max_monster_count < DND_INCURSION_IMPROVETHRESHOLD) {
        // shouldn't ever happen but you know...
        if(!max_monster_count)
            max_monster_count = 1;
        base += DND_INCURSION_MARKERIMPROVEPCT * (DND_INCURSION_IMPROVETHRESHOLD / max_monster_count);
    }
    else if(max_monster_count > DND_INCURSION_REDUCETHRESHOLD) {
        // for every 100 monster above the reduction count reduce by this much
        base -= DND_INCURSION_MARKERIMPROVEPCT * ((max_monster_count - DND_INCURSION_REDUCETHRESHOLD) / 100);
        if(base < DND_INCURSION_MIN_MARKERCHANCE)
            base = DND_INCURSION_MIN_MARKERCHANCE;
    }

    return base;
}

// Doesn't check for summoned or revived conditions because the kill script later down the line won't go through for those monsters where this would matter
// the further it goes down the list the chances improve slightly
bool CanMonsterSpawnIncursionMarker(int m_id, int base_chance, int curr_count, int max_monster_count) {
    int mul = 1.0 + DND_INCURSION_BONUSMULT * curr_count / max_monster_count;

    // bonus factor to be added
    int factor = 0;
    switch(MonsterProperties[m_id].class) {
        case MONSTERCLASS_ZOMBIEMAN:
        case MONSTERCLASS_SHOTGUNGUY:
        case MONSTERCLASS_WOLFENSS:
            factor = -0.05;
        break;

        // no increase or decrease
        case MONSTERCLASS_LOSTSOUL:
        case MONSTERCLASS_CHAINGUNGUY:
        case MONSTERCLASS_DEMON:
        case MONSTERCLASS_SPECTRE:
        case MONSTERCLASS_IMP:
            factor = 0;
        break;

        case MONSTERCLASS_CACODEMON:
        case MONSTERCLASS_PAINELEMENTAL:
        case MONSTERCLASS_REVENANT:
            factor = 0.0125;
        break;


        case MONSTERCLASS_HELLKNIGHT:
            factor = 0.02;
        break;
        case MONSTERCLASS_BARON:
            factor = 0.025;
        break;

        case MONSTERCLASS_FATSO:
        case MONSTERCLASS_ARACHNOTRON:
            factor = 0.03;
        break;

        case MONSTERCLASS_ARCHVILE:
            factor = 0.05;
        break;

        case MONSTERCLASS_SPIDERMASTERMIND:
        case MONSTERCLASS_CYBERDEMON:
            factor = 0.1;
        break;
    }

    return random(0, 1.0) <= FixedMul(factor + DND_INCURSION_BASE_MARKERCHANCE, mul);
}

void ResetMarkerMonsterKillData(int marker_tid) {
    for(int i = 0; i < MAX_MONSTER_CATEGORIES; ++i) {
        str var_name = StrParam(s:"user_mclass_", d:i);
        SetUserVariable(
            marker_tid,
            var_name,
            0
        );
    }
}

void GiveMarkerMonsterKillData(int marker_tid, int mclass) {
    str var_name = StrParam(s:"user_mclass_", d:mclass);
    SetUserVariable(
        marker_tid,
        var_name,
        GetUserVariable(marker_tid, var_name) + 1
    );
    //Log(s:"incremented marker ", d:marker_tid, s:" on class ", d:mclass, s:" to now be ", d:GetUserVariable(marker_tid, var_name));
}

void PoolMarkerDataToPortal(int marker_tid, int portal_tid, int mclass) {
    str var_name = StrParam(s:"user_mclass_", d:mclass);
    SetUserVariable(
        portal_tid,
        var_name,
        GetUserVariable(portal_tid, var_name) + GetUserVariable(marker_tid, var_name)
    );
}

str GetIncursionMonsterSpawner(int class) {
    switch(class) {
        case MONSTERCLASS_ZOMBIEMAN:
        return "ZombiemanSpawner_Incursion";
        case MONSTERCLASS_SHOTGUNGUY:
        return "ShotgunguySpawner_Incursion";
        case MONSTERCLASS_CHAINGUNGUY:
        return "ChaingunguySpawner_Incursion";
        case MONSTERCLASS_DEMON:
        return "DemonSpawner_Incursion";
        case MONSTERCLASS_SPECTRE:
        return "SpectreSpawner_Incursion";
        case MONSTERCLASS_IMP:
        return "ImpSpawner_Incursion";
        case MONSTERCLASS_CACODEMON:
        return "CacoSpawner_Incursion";
        case MONSTERCLASS_PAINELEMENTAL:
        return "PainElementalSpawner_Incursion";
        case MONSTERCLASS_LOSTSOUL:
        return "LostSoulSpawner_Incursion";
        case MONSTERCLASS_REVENANT:
        return "RevenantSpawner_Incursion";
        case MONSTERCLASS_HELLKNIGHT:
        return "HellKnightSpawner_Incursion";
        case MONSTERCLASS_BARON:
        return "BaronSpawner_Incursion";
        case MONSTERCLASS_FATSO:
        return "FatsoSpawner_Incursion";
        case MONSTERCLASS_ARACHNOTRON:
        return "ArachnoSpawner_Incursion";
        case MONSTERCLASS_ARCHVILE:
        return "ArchVileSpawner_Incursion";
        case MONSTERCLASS_SPIDERMASTERMIND:
        return "SpiderBossSpawner_Incursion";
        case MONSTERCLASS_CYBERDEMON:
        return "CyberdemonSpawner_Incursion";
        case MONSTERCLASS_WOLFENSS:
        return "WolfenSS_Spawner_Incursion";
    }
    return "";
}

Script "DnD Incursion Spawn" (int incursion_theme) {
    // spawn monsters from this portal according to what has been killed around it... with some randomization (maybe not spawn some at all etc.)
    for(int i = 0; i < MAX_MONSTER_CATEGORIES; ++i) {
        str var_name = StrParam(s:"user_mclass_", d:i);

        // divide by DND_INCURSION_MARKER_MERGECOUNT - 1 because there will be duplicate signals on these markers in most cases
        int val = GetUserVariable(0, var_name);
        if(!val)
            continue;

        if(i != MONSTERCLASS_CYBERDEMON && i != MONSTERCLASS_SPIDERMASTERMIND)
            val = random(val / (DND_INCURSION_MARKER_MERGECOUNT - 1), val);
        else
            val /= DND_INCURSION_MARKER_MERGECOUNT - 1;

        str spawner = GetIncursionMonsterSpawner(i);
        bool spawned = false;

        for(int j = 0; j < val; ++j) {
            if(SpawnAreaRandomTID(0, DND_INCURSION_SPAWNRADIUS, spawner, DND_INCURSION_SPAWNER_AUX)) {
                //Log(s:"spawned!");
                //SetActorProperty(DND_INCURSION_SPAWNER_AUX, APROP_SCORE, incursion_theme);
                //Thing_ChangeTID(DND_INCURSION_SPAWNER_AUX, 0);
                spawned = true;
                Delay(const:2);
            }
        }

        if(spawned)
            Delay(const:HALF_TICRATE);
    }

    // disappear
    SetActorState(0, "Vanish", false);
}

#endif