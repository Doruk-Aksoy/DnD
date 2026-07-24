#ifndef DND_TOKENINFO_IN
#define DND_TOKENINFO_IN

enum {
	DND_TOKEN_ARMORER,
	DND_TOKEN_GUNSMITH,
	DND_TOKEN_ARTISAN,
    DND_TOKEN_CARTOGRAPHER,

    DND_MAX_TOKEN_KINDS
};

void SetupTokenDropWeights() {
	// Body Armors
	INIT_ITEM_WEIGHTS;
	SET_ITEM_WEIGHT(DND_DROPPEDITEM_TOKEN, DND_TOKEN_ARMORER, 16);
	SET_ITEM_WEIGHT(DND_DROPPEDITEM_TOKEN, DND_TOKEN_GUNSMITH, 20);
	SET_ITEM_WEIGHT(DND_DROPPEDITEM_TOKEN, DND_TOKEN_ARTISAN, 8);
    SET_ITEM_WEIGHT(DND_DROPPEDITEM_TOKEN, DND_TOKEN_CARTOGRAPHER, 4);
}

bool CanUseToken(int token_type, int item_id, int item_type) {
    bool res = false;
    int pnum = PlayerNumber();

    auto item = GetPlayerInventoryItem(pnum, item_id);

    switch(token_type) {
        case DND_TOKEN_ARMORER:
            if(item_type > UNIQUE_BEGIN)
                item_type &= 0xFFFF;
            res = (
                item_type == DND_ITEM_BODYARMOR || item_type == DND_ITEM_BOOT || item_type == DND_ITEM_HELM || 
                (item_type >= FIRST_SPECIALTY_ITEM_TYPE && item_type <= LAST_SPECIALTY_ITEM_TYPE)
            ) && item.quality < GetItemMaxQuality(pnum, item_id);
            res &= !IsInventoryCorrupted(pnum, item_id);
        break;
        case DND_TOKEN_GUNSMITH:
            // if we have quality OR power to hit ghost from modifications made directly to a weapon
            res = item_type == DND_ITEM_WEAPON && 
            (
                Player_Weapon_Infos[pnum][item_id].quality > 0 || 
                    (HasWeaponPower(pnum, item_id, WEP_POWER_GHOSTHIT) && (Weapons_Data[item_id].properties & WPROP_CANTHITGHOST))
            );
        break;
        case DND_TOKEN_ARTISAN:
            res = item_type == DND_ITEM_FLASK && item.quality < GetItemMaxQuality(pnum, item_id);
        break;
        case DND_TOKEN_CARTOGRAPHER:
            res = item_type == DND_ITEM_DUNGEONKEY && item.quality < GetItemMaxQuality(pnum, item_id);
        break;
    }
    return res;
}

void RollTokenInfo(int item_pos, int token_type, bool onField, int stack = 1) {
	// roll random attributes for the charm
    auto item = GetFieldItem(item_pos);
	item.item_level = 1;
	
#ifdef ISDEBUGBUILD
	item.item_stack = 100;
#else
	item.item_stack = stack; // stackables have default stack of 1
#endif

	item.item_type = DND_ITEM_TOKEN;
	item.item_subtype = token_type;
	item.width = 1;
	item.height = 1;
	item.attrib_count = 0;
	item.item_image = ITEM_IMAGE_TOKEN_BEGIN + token_type;
}

// if we add more mods that can be directly applied through use of orbs on the weapon itself, modify here!
void ApplyGunsmithToken(int pnum, int wepid) {
	int i;
	Player_Weapon_Infos[pnum][wepid].quality = 0;

    // zero only the ghost hitting power -- remove weapon affecting sources for this, as the orb affecting this is the "phantasmal" orb applied onto the weapon directly!
    SetWeaponModPowerset(pnum, wepid, WEP_POWER_GHOSTHIT, 0, WMOD_WEP);
}

void HandleTokenUse(int pnum, int token_type, int item_id) {
    int temp;
    auto item = GetPlayerInventoryItem(pnum, item_id);
    int amt = 0;
    switch(token_type) {
        case DND_TOKEN_ARMORER:
        case DND_TOKEN_ARTISAN:
        case DND_TOKEN_CARTOGRAPHER:
			// just increment quality
            amt = random(QUALITY_ITEM_ADD_MIN, QUALITY_ITEM_ADD_MAX);
			item.quality += amt;
            
            temp = GetItemMaxQuality(pnum, item_id);
            if(item.quality > temp)
                item.quality = temp;

			SyncItemQuality(pnum, item_id, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
        break;
        case DND_TOKEN_GUNSMITH:
            // zero all mods and quality
			ApplyGunsmithToken(pnum, item_id);
            SyncClientsideVariable_WeaponProperties(pnum, item_id);
			SyncClientsideVariable_WeaponMods(pnum, item_id);
        break;
    }
    ACS_NamedExecuteAlways("DND Token Use Message", 0, token_type, item_id, amt);
}

void HandleTokenUseMessage(int token_type, int item_id, int amt) {
    switch(token_type) {
        case DND_TOKEN_ARMORER:
            Log(s:"\cj", l:"TOK_USE1", s:" \cd", d:amt, s:"%\c-.");
        break;
        case DND_TOKEN_GUNSMITH:
            Log(s:"\cj", l:"TOK_USE2", s:" \cv", l:GetWeaponTag(item_id), s:"\c-.");
        break;
        case DND_TOKEN_ARTISAN:
            Log(s:"\cj", l:"TOK_USE3", s:" \cd", d:amt, s:"%\c-.");
        break;
        case DND_TOKEN_CARTOGRAPHER:
            Log(s:"\cj", l:"TOK_USE4", s:" \cd", d:amt, s:"%\c-.");
        break;
    }
}

Script "DND Token Use" (int token_type, int item_id, int amt) {
	int pnum = PlayerNumber();
	HandleTokenUse(pnum, token_type, item_id);
	LocalAmbientSound("Items/TokenPick", 127);
	ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, pnum);
}

Script "DND Token Use Message" (int type, int result, int amt) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;
	HandleTokenUseMessage(type, result, amt);
}

#endif