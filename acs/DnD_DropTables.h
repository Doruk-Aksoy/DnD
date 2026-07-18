#ifndef DND_DROPTABLE_IN
#define DND_DROPTABLE_IN

enum {
    DND_MONSTERLOOT_CHARM,
    DND_MONSTERLOOT_BODYARMOR,
    DND_MONSTERLOOT_HELM,
    DND_MONSTERLOOT_BOOT,
    
    DND_MONSTERLOOT_SPECIALTY,

    DND_MONSTERLOOT_FLASK,
    DND_MONSTERLOOT_ORB,
    DND_MONSTERLOOT_CHESTKEY,

    DND_MONSTERLOOT_TOKEN,
    DND_MONSTERLOOT_DUNGEONKEY,

    DND_MONSTERLOOT_COUNT
};

typedef struct {
    int count;
    int[]* mod_list;
} mod_pool_T;

typedef struct {
    int monster_loot_weight_sum;
	alias_table_T* monster_drop_table;
} loot_tables_T;

global loot_tables_T 21: LootTables;

#define DND_LOOTWEIGHT_NOTHING 10000
#define DND_SPECIALTY_BIAS_CHANCE 66 // biased towards finding your own specialty item
int[] module& GetLootDropWeights() {
    static int loot_weights[DND_MONSTERLOOT_COUNT] = {
        200,
        150,
        100,
        120,

        60,

        32,
        160,
        60,

        50,
        20
    };

    return loot_weights;
}

void SetupMonsterDropTable() {
    auto weights = GetLootDropWeights();

    int i;
    LootTables.monster_loot_weight_sum = 0;
    LootTables.monster_drop_table = CreateAliasTable(DND_MONSTERLOOT_COUNT);

    for(i = 0; i < DND_MONSTERLOOT_COUNT; ++i) {
        LootTables.monster_drop_table.weights[i] = weights[i];
        LootTables.monster_loot_weight_sum += weights[i];
    }
}

int GetAdjustedNothingWeight(int drop_chance, int drop_boost = 100) {
    int tmp = (((drop_chance * 100 - 100.0) >> 16) * drop_boost / 100) - 100;
    // clamp it
    if(tmp < -50)
        tmp = -50;
    return DND_LOOTWEIGHT_NOTHING * 100 / (100 + tmp);
}

void TestMonsterDropTable(int pnum, int drop_boost = 100)  {
    int i = 1000;
    int vals[DND_MONSTERLOOT_COUNT] = { 0 };

    int p_chance = GetDropChance(pnum);
    int nothing_count = 0;
    int w = GetAdjustedNothingWeight(p_chance, drop_boost);

    Log(s:"Nothing weight and sum: ", d:w, s: " ", d:LootTables.monster_loot_weight_sum, s: " drop boost: ", d:drop_boost, s:" drop chance: ", f:p_chance);

    while(i--) {
        if(random(1, w + LootTables.monster_loot_weight_sum) > LootTables.monster_loot_weight_sum)
            ++nothing_count;
        else
            ++vals[PickFromAliasTable(LootTables.monster_drop_table)];
    }

    Log(s:"Nothing rolled: ", d:nothing_count);
    for(i = 0; i < DND_MONSTERLOOT_COUNT; ++i)
        Log(s:"Picked ", d:i, s:": ", d:vals[i]);
}

#endif