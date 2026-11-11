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
    DND_MONSTERLOOT_GEM,

    DND_MONSTERLOOT_COUNT
};

typedef struct {
    int monster_loot_weight_sum;
	alias_table_T* monster_drop_table;
} loot_tables_T;

global loot_tables_T 21: LootTables;

#define DND_SPECIALTY_BIAS_CHANCE 66 // biased towards finding your own specialty item
int[] module& GetLootDropWeights() {
    static int loot_weights[DND_MONSTERLOOT_COUNT] = {
        200,
        56,
        40,
        48,

        60,

        32,
        160,
        24,

        20,
        5,
        4       // gems, and only from monster level DND_GEM_MINLEVEL up
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

// 1000 kills of a monster worth value (whole value points) on this map: the items each would drop, by kind.
void TestMonsterDropTable(int pnum, int value = 10)  {
    int i = 1000, n, total = 0;
    int vals[DND_MONSTERLOOT_COUNT] = { 0 };
    int share = FixedMul(MapData[DND_MAPDATA_LOOTPAYOUT], value << 16);
    int e = FixedMul(FixedMul(share, GetDropChance(pnum)), GetPlayerDropQuantity(pnum));

    Log(s:"Budget ", d:MapData[DND_MAPDATA_LOOTBUDGET], s:" payout ", f:MapData[DND_MAPDATA_LOOTPAYOUT], s:" expected per kill ", f:e);
    while(i--)
        for(n = (e >> 16) + (random(0, 0xFFFF) < (e & 0xFFFF)); n > 0; --n) {
            ++vals[PickFromAliasTable(LootTables.monster_drop_table)];
            ++total;
        }

    Log(s:"Items from 1000 kills: ", d:total);
    for(i = 0; i < DND_MONSTERLOOT_COUNT; ++i)
        Log(s:"Picked ", d:i, s:": ", d:vals[i]);
}

#endif