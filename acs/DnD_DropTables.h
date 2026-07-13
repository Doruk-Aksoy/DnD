#ifndef DND_DROPTABLE_IN
#define DND_DROPTABLE_IN

enum {
    DND_MONSTERLOOT_NOTHING,

    DND_MONSTERLOOT_CHARM,
    DND_MONSTERLOOT_BODYARMOR,
    DND_MONSTERLOOT_HELM,
    DND_MONSTERLOOT_BOOT,
    
    DND_MONSTERLOOT_SPECIALTY_DOOMGUY,
	DND_MONSTERLOOT_SPECIALTY_MARINE,
	DND_MONSTERLOOT_SPECIALTY_HOBO,
	DND_MONSTERLOOT_SPECIALTY_PUNISHER,
	DND_MONSTERLOOT_SPECIALTY_WANDERER,
	DND_MONSTERLOOT_SPECIALTY_CYBORG,
	DND_MONSTERLOOT_SPECIALTY_BERSERKER,
	DND_MONSTERLOOT_SPECIALTY_TRICKSTER,

    DND_MONSTERLOOT_FLASK,
    DND_MONSTERLOOT_ORB,
    DND_MONSTERLOOT_CHESTKEY,

    DND_MONSTERLOOT_TOKEN,

    DND_MONSTERLOOT_COUNT
};

typedef struct {
	alias_table_T* monster_drop_table;
} loot_tables_T;

global loot_tables_T 21: LootTables;

int[] module& GetLootDropWeights() {
    static int loot_weights[DND_MONSTERLOOT_COUNT] = {
        10000,
        100,
        75,
        50,
        60,

        4,
        4,
        4,
        4,
        4,
        4,
        4,
        4,

        15,
        80,
        30,

        25
    };

    return loot_weights;
}

void SetupMonsterDropTable() {
    auto weights = GetLootDropWeights();

    int i;
	LootTables.monster_drop_table = CreateAliasTable(DND_MONSTERLOOT_COUNT);

    for(i = 0; i < DND_MONSTERLOOT_COUNT; ++i)
        LootTables.monster_drop_table.weights[i] = weights[i];
}

// sets it up to the same weight
void ResetMonsterDropTable() {
    auto weights = GetLootDropWeights();
    for(int i = 0; i < DND_MONSTERLOOT_COUNT; ++i)
        LootTables.monster_drop_table.weights[i] = weights[i];
}

// reducing weight of nothing dropping is same as increasing everything else, this is the most performant option here
void UpdateMonsterDropTable(int increase, int class_of_player) {
    auto weights = GetLootDropWeights();
    LootTables.monster_drop_table.weights[DND_MONSTERLOOT_NOTHING] = weights[DND_MONSTERLOOT_NOTHING] * 100 / (100 + increase);

    // player class bonus for player to be more likely to find specialty item of themselves
    LootTables.monster_drop_table.weights[DND_MONSTERLOOT_SPECIALTY_DOOMGUY + class_of_player] = weights[DND_MONSTERLOOT_SPECIALTY_DOOMGUY + class_of_player] * 4;

    LootTables.monster_drop_table.isDirty = true;
}

#endif