#ifndef DND_ITEMMODPOOL_IN
#define DND_ITEMMODPOOL_IN

// The sampler lives with the pool it samples, so the two cannot drift apart. table is null only
// when count is 0 -- a base that no mod is eligible for.
typedef struct {
    int count;
    int[]* mod_list;
    alias_table_T* table;
} mod_pool_T;

#define SIZEOF_MODPOOL (SIZEOF_INT * 3)

// The item base already names the slot and the tagset together, so it IS the pool identity -- no
// interning needed. 35 bases fit in 6 bits, and the two tag fields hold a tag ID rather than an
// index into the CANROLL implicits, so any tag can become widenable or forceable without the key
// layout moving. Tag IDs line up with the bits (INV_ATTR_TAG_DAMAGE_ID 0 <-> INV_ATTR_TAG_DAMAGE
// 1), so a mask is just 1 << id.
#define DND_MODPOOL_ITEMBASE_BITS 6
#define DND_MODPOOL_TAGID_BITS 5
#define DND_MODPOOL_WIDEN_SHIFT DND_MODPOOL_ITEMBASE_BITS
#define DND_MODPOOL_FORCEDTAG_SHIFT (DND_MODPOOL_WIDEN_SHIFT + DND_MODPOOL_TAGID_BITS)
#define DND_MODPOOL_NO_TAG (-1)

#define ModPoolTagIdToMask(id) ((id) >= 0 ? (1 << (id)) : 0)

// maps: item base | widening tag + 1 | forced tag + 1 -> mod_pool_T*
LIBBCS_T_MAP_DEFN(CachedModPoolIdMap, int, int, LIBBCS_SIMPLE_CMP);

typedef struct {
    int curr_size;
    CachedModPoolIdMapT CachedModPools;
} mod_info_data_T;

global mod_info_data_T 35: ModPoolTables;

// forced_tag_id is reserved for the crafting-orb guarantee pools, which do not exist yet -- every
// caller passes the default. Keyed now so adding them later does not re-key the cache.
int MakeModPoolKey(int item_base, int widen_tag_id = DND_MODPOOL_NO_TAG, int forced_tag_id = DND_MODPOOL_NO_TAG) {
    return  item_base |
            ((widen_tag_id + 1) << DND_MODPOOL_WIDEN_SHIFT) |
            ((forced_tag_id + 1) << DND_MODPOOL_FORCEDTAG_SHIFT);
}

void FreeModPool(mod_pool_T* m) {
    // count gates both, because an empty pool allocates neither
    if(m.count) {
        FreeAliasTable(m.table);
        bcs::arrDelete(m.mod_list);
    }

    bcs::free(m);
}

// Check if the slot this mod is for is appropriate, as well as the implicit properties of the item
// first part solves "what items can roll this mod" problem
// second part solves "is this mod tag even eligible to appear" problem
// third part solves "is there an implicit conflict" problem, ie. Armor|Mitigation base trying to roll energy shield
bool CanRollMod(int item_base_mask, int item_allowed_tags, int mod) {
    return  (ItemModTable[mod].allowed_slots & item_base_mask) && 
            (ItemModTable[mod].tags & item_allowed_tags) == ItemModTable[mod].tags;
}

// A "can roll X" implicit overrules the base's exclusions rather than merely filling the gaps it
// left open: the implicit exists to be an exception to the base rule, so it hands the whole tag
// group charm-level reach. A helm that says it rolls ailments gets them even though its base
// excludes AILMENT.
bool CanRollModWidened(int slot_mask, int allowed_tags, int widen_mask, int mod) {
    if(CanRollMod(slot_mask, allowed_tags, mod))
        return true;

    return  widen_mask &&
            (ItemModTable[mod].tags & widen_mask) &&
            (ItemModTable[mod].allowed_slots & DND_MODBASE_CHARM);
}

// sets up mod pool for a given item base, with given mod tags that can be applied to it for eligibility (this is used as a cached pool for monster drops)
mod_pool_T* BuildModPool(int item_base, int widen_tag_id = DND_MODPOOL_NO_TAG, int forced_tag_id = DND_MODPOOL_NO_TAG) {
    int buf[NORMAL_ATTRIBUTE_COUNT];
    int count = 0;

    int slot_mask = ItemBaseToModBaseFlag(item_base);
    int allowed = GetItemBaseEffectiveTags(item_base);
    int widen_mask = ModPoolTagIdToMask(widen_tag_id);
    int forced_mask = ModPoolTagIdToMask(forced_tag_id);

    for(int i = FIRST_INV_ATTRIBUTE; i <= LAST_INV_ATTRIBUTE; ++i) {
        if(!CanRollModWidened(slot_mask, allowed, widen_mask, i))
            continue;

        // An orb that guarantees a tag narrows what is already eligible rather than reaching past
        // it, so a forced tag the base cannot roll leaves the pool empty and the caller skips.
        if(forced_mask && !(ItemModTable[i].tags & forced_mask))
            continue;

        buf[count++] = i;
    }

    mod_pool_T* m = bcs::malloc(SIZEOF_MODPOOL);
    m.count = count;
    m.table = null;

    // nothing is eligible for this base, so there is nothing to allocate or sample
    if(!count)
        return m;

    m.mod_list = bcs::arrNew(count);
    m.table = CreateAliasTable(count);

    for(i = 0; i < count; ++i) {
        m.mod_list[i] = buf[i];
        m.table.weights[i] = ItemModTable[buf[i]].weight;
    }

    // built here rather than lazily on first pick, so registration leaves nothing for a drop to pay
    BuildAliasTable(m.table);

    return m;
}

// index into the mod table, or -1 when no mod is eligible for this base at all
int PickModFromPool(mod_pool_T* m) {
    if(!m.count)
        return -1;

    return m.mod_list[PickFromAliasTable(m.table)];
}

// Builds on a miss and caches the result. Every unwidened pool is warmed at setup, so the only
// misses left are widened ones, which come from a handful of item subtypes -- that costs one build
// per (base, widening) pair for the whole session rather than one per drop. Warming all of those
// up front would mean bases * tags pools, nearly all of which no item can ever ask for.
mod_pool_T* GetModPool(int item_base, int widen_tag_id = DND_MODPOOL_NO_TAG, int forced_tag_id = DND_MODPOOL_NO_TAG) {
    int id = MakeModPoolKey(item_base, widen_tag_id, forced_tag_id);
    mod_pool_T* m = CachedModPoolIdMap_Get(ModPoolTables.CachedModPools, id);

    if(m == null) {
        m = BuildModPool(item_base, widen_tag_id, forced_tag_id);
        CachedModPoolIdMap_Put(ModPoolTables.CachedModPools, id, (int)m);
        ++ModPoolTables.curr_size;
    }

    return m;
}

void InitModPoolCache() {
    ModPoolTables.CachedModPools = CachedModPoolIdMap_New();
    ModPoolTables.curr_size = 0;
}

// The warmup itself lives in the setup script rather than here, because building every base is far
// too much work for one tic and only a script can Delay. See "DnD Load Inventory Attributes".
void LogModPoolCacheSize() {
#ifdef ISDEBUGBUILD
    Log(s:"Mod pools built: ", d:ModPoolTables.curr_size, s:" of ", d:DND_MAX_ITEMBASES, s:" item bases.");
#endif
}

#endif