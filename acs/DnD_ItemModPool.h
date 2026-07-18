#ifndef DND_ITEMMODPOOL_IN
#define DND_ITEMMODPOOL_IN

mod_pool_T* CreateModPool(int size) {
    mod_pool_T* res = bcs::malloc(SIZEOF_INT * 2);
    res.mod_list = bcs::arrNew(size);
    res.count = 0;
    return res;
}

void FreeModPool(mod_pool_T* m) {
    bcs::arrDelete(m.mod_list);
    bcs::free(m);
}

// Check if the slot this mod for is appropriate, as well as the implicit properties of the item
bool CanRollMod(int item_base_mask, int item_implicit_prop, int mod) {
    return  (ItemModTable[mod].allowed_slots & item_base_mask) && 
            (ItemModTable[mod].required_properties & item_implicit_prop) == ItemModTable[mod].required_properties;
}

void BuildModPool(mod_pool_T* m, int item_base_mask, int item_implicit_prop) {
    for(int i = FIRST_INV_ATTRIBUTE; i <= LAST_INV_ATTRIBUTE; ++i) {
        if(!CanRollMod(item_base_mask, item_implicit_prop, i))
            continue;
        m.mod_list[m.count++] = i;
    }
}

#endif