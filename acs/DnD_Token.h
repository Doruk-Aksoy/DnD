#ifndef DND_TOKENINFO_IN
#define DND_TOKENINFO_IN

enum {
	DND_TOKEN_ARMORER,
	DND_TOKEN_GUNSMITH
};

bool CanUseToken(int token_type, int item_id, int item_type) {
    bool res = false;
    int pnum = PlayerNumber();

    switch(token_type) {
        case DND_TOKEN_ARMORER:
            res = (item_type == DND_ITEM_BODYARMOR || item_type == DND_ITEM_BOOT || item_type == DND_ITEM_HELM || item_type == DND_ITEM_POWERCORE) && PlayerInventoryList[pnum][item_id].quality < DND_MAX_ITEM_QUALITY;
        break;
        case DND_TOKEN_GUNSMITH:
            // if we have quality OR power to hit ghost from modifications made directly to a weapon
            res = item_type == DND_ITEM_WEAPON && 
            (
                Player_Weapon_Infos[pnum][item_id].quality > 0 || 
                    (HasWeaponPower(pnum, item_id, WEP_POWER_GHOSTHIT) && (Weapons_Data[item_id].properties & WPROP_CANTHITGHOST))
            );
        break;
    }
    return res;
}

void RollTokenInfo(int item_pos, int token_type, bool onField) {
	// roll random attributes for the charm
	Inventories_On_Field[item_pos].item_level = 1;
	
#ifdef ISDEBUGBUILD
	Inventories_On_Field[item_pos].item_stack = 100;
#else
	Inventories_On_Field[item_pos].item_stack = 1; // stackables have default stack of 1
#endif

	Inventories_On_Field[item_pos].item_type = DND_ITEM_TOKEN;
	Inventories_On_Field[item_pos].item_subtype = token_type;
	Inventories_On_Field[item_pos].width = 1;
	Inventories_On_Field[item_pos].height = 1;
	Inventories_On_Field[item_pos].attrib_count = 0;
	Inventories_On_Field[item_pos].item_image = ITEM_IMAGE_TOKEN_BEGIN + token_type;
}

// if we add more mods that can be directly applied through use of orbs on the weapon itself, modify here!
void ApplyGunsmithToken(int pnum, int wepid) {
	int i;
	Player_Weapon_Infos[pnum][wepid].quality = 0;

    // zero only the ghost hitting power -- remove weapon affecting sources for this, as the orb affecting this is the "phantasmal" orb applied onto the weapon directly!
    SetWeaponModPowerset(pnum, wepid, WEP_POWER_GHOSTHIT, 0, 1);
}

void HandleTokenUse(int pnum, int token_type, int item_id) {
    switch(token_type) {
        case DND_TOKEN_ARMORER:
			// just increment quality
			++PlayerInventoryList[pnum][item_id].quality;
			SyncItemQuality(pnum, item_id, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
        break;
        case DND_TOKEN_GUNSMITH:
            // zero all mods and quality
			ApplyGunsmithToken(pnum, item_id);
            SyncClientsideVariable_WeaponProperties(pnum, item_id);
			SyncClientsideVariable_WeaponMods(pnum, item_id);
        break;
    }
    ACS_NamedExecuteAlways("DND Token Use Message", 0, token_type, item_id);
}

void HandleTokenUseMessage(int token_type, int item_id) {
    switch(token_type) {
        case DND_TOKEN_ARMORER:
            Log(s:"\cj", l:"TOK_USE1");
        break;
        case DND_TOKEN_GUNSMITH:
            Log(s:"\cj", l:"TOK_USE2", s:" \cv", l:GetWeaponTag(item_id), s:"\c-.");
        break;
    }
}

Script "DND Token Use" (int token_type, int item_id) {
	int pnum = PlayerNumber();
	HandleTokenUse(pnum, token_type, item_id);
	LocalAmbientSound("Items/TokenPick", 127);
	ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, pnum);
}

Script "DND Token Use Message" (int type, int result, int affluence) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;
	HandleTokenUseMessage(type, result);
}

#endif