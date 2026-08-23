#ifndef DND_SYNC_IN
#define DND_SYNC_IN

global bool 52: PlayerWeaponDataNeedsSync[MAXPLAYERS][MAXWEPS];

void MarkWeaponDataSync(int pnum, int wepid, bool s) {
	PlayerWeaponDataNeedsSync[pnum][wepid] = s;
}

bool WeaponNeedsDataSync(int pnum, int wepid) {
	return PlayerWeaponDataNeedsSync[pnum][wepid];
}

enum {
	DND_SYNC_WEPMOD_CRIT,
	DND_SYNC_WEPMOD_CRITDMG,
	DND_SYNC_WEPMOD_CRITPERCENT,
	DND_SYNC_WEPMOD_DMG,
	DND_SYNC_WEPMOD_POWERSET1,
	
	DND_SYNC_ITEMTOPLEFTBOX,
	DND_SYNC_ITEMTYPE,
	DND_SYNC_ITEMSUBTYPE,
	DND_SYNC_ITEMWIDTH,
	DND_SYNC_ITEMHEIGHT,
	DND_SYNC_ITEMIMAGE,
	DND_SYNC_ITEMLEVEL,
	DND_SYNC_ITEMSTACK,
	DND_SYNC_ITEMCORRUPTED,
	DND_SYNC_ITEMQUALITY,
	DND_SYNC_ITEMBASE,
	// add non attribute related things from above attrib count

	// implicit attribute stuff
	DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID,
	DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL,
	DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER,
	DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA,

	DND_SYNC_ITEMSATTRIBCOUNT,
	DND_SYNC_ITEMATTRIBUTES_ID,
	DND_SYNC_ITEMATTRIBUTES_VAL,
	DND_SYNC_ITEMATTRIBUTES_TIER,
	DND_SYNC_ITEMATTRIBUTES_EXTRA,
	DND_SYNC_ITEMATTRIBUTES_FRACTURE,
	// add attribute related things from below here
};
#define DND_LAST_SYNC_TYPE DND_SYNC_ITEMATTRIBUTES_FRACTURE

#define DND_SYNC_ITEMBEGIN DND_SYNC_ITEMTOPLEFTBOX
#define DND_SYNC_ITEMEND DND_SYNC_ITEMATTRIBUTES_TIER

#define FIRST_WEPMOD_SYNC (DND_SYNC_WEPMOD_CRIT)
#define MAX_SYNC_VARS (DND_SYNC_WEPMOD_POWERSET1 + 1)

enum {
	DND_SYNC_ITEMSOURCE_ITEMSUSED,
	DND_SYNC_ITEMSOURCE_PLAYERINVENTORY,
	DND_SYNC_ITEMSOURCE_FIELD,
	DND_SYNC_ITEMSOURCE_TRADEVIEW,		// trade view array
	DND_SYNC_ITEMSOURCE_STASH
};

int GetItemSyncValue(int pnum, int which, int extra, int sub, int source) {
	auto item = AcquireItemFromSource(pnum, extra, source);

	switch(which) {
		case DND_SYNC_ITEMWIDTH:
		return item.width;
		case DND_SYNC_ITEMHEIGHT:
		return item.height;
		case DND_SYNC_ITEMIMAGE:
		return item.item_image;
		case DND_SYNC_ITEMTYPE:
		return item.item_type;
		case DND_SYNC_ITEMSUBTYPE:
		return item.item_subtype;
		case DND_SYNC_ITEMLEVEL:
		return item.item_level;
		case DND_SYNC_ITEMTOPLEFTBOX:
		return item.topleftboxid;
		case DND_SYNC_ITEMSATTRIBCOUNT:
		return item.attrib_count;
		case DND_SYNC_ITEMSTACK:
		return item.item_stack;
		case DND_SYNC_ITEMCORRUPTED:
		return item.corrupted;
		case DND_SYNC_ITEMQUALITY:
		return item.quality;
		case DND_SYNC_ITEMBASE:
		return item.item_base;

		case DND_SYNC_ITEMATTRIBUTES_ID:
		return item.attributes[sub].attrib_id;
		case DND_SYNC_ITEMATTRIBUTES_VAL:
		return item.attributes[sub].attrib_val;
		case DND_SYNC_ITEMATTRIBUTES_TIER:
		return item.attributes[sub].attrib_tier;
		case DND_SYNC_ITEMATTRIBUTES_FRACTURE:
		return item.attributes[sub].fractured;
		case DND_SYNC_ITEMATTRIBUTES_EXTRA:
		return item.attributes[sub].attrib_extra;

		case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID:
		return item.implicit[sub].attrib_id;
		case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL:
		return item.implicit[sub].attrib_val;
		case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER:
		return item.implicit[sub].attrib_tier;
		case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA:
		return item.implicit[sub].attrib_extra;
	}

	return 0;
}

// which = what property of item
// extra = index in array
// sub = attribute index of item
// val = value to put
// source = source of inventory item (inventory, used charms, field etc.)
void SetItemSyncValue(int pnum, int which, int extra, int sub, int val, int source) {
	auto item = AcquireItemFromSource(pnum, extra, source);
	
	switch(which) {
		case DND_SYNC_ITEMWIDTH:
			item.width = val;
		break;
		case DND_SYNC_ITEMHEIGHT:
			item.height = val;
		break;
		case DND_SYNC_ITEMIMAGE:
			item.item_image = val;
		break;
		case DND_SYNC_ITEMTYPE:
			item.item_type = val;
		break;
		case DND_SYNC_ITEMSUBTYPE:
			item.item_subtype = val;
		break;
		case DND_SYNC_ITEMLEVEL:
			item.item_level = val;
		break;
		case DND_SYNC_ITEMTOPLEFTBOX:
			item.topleftboxid = val;
		break;
		case DND_SYNC_ITEMSATTRIBCOUNT:
			item.attrib_count = val;
		break;
		case DND_SYNC_ITEMSTACK:
			item.item_stack = val;
		break;
		case DND_SYNC_ITEMCORRUPTED:
			item.corrupted = val;
		break;
		case DND_SYNC_ITEMQUALITY:
			item.quality = val;
		break;
		case DND_SYNC_ITEMBASE:
			item.item_base = val;
		break;

		case DND_SYNC_ITEMATTRIBUTES_ID:
			item.attributes[sub].attrib_id = val;
		break;
		case DND_SYNC_ITEMATTRIBUTES_VAL:
			item.attributes[sub].attrib_val = val;
		break;
		case DND_SYNC_ITEMATTRIBUTES_TIER:
			item.attributes[sub].attrib_tier = val;
		break;
		case DND_SYNC_ITEMATTRIBUTES_FRACTURE:
			item.attributes[sub].fractured = val;
		break;
		case DND_SYNC_ITEMATTRIBUTES_EXTRA:
			item.attributes[sub].attrib_extra = val;
		break;

		case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID:
			item.implicit[sub].attrib_id = val;
		break;
		case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL:
			item.implicit[sub].attrib_val = val;
		break;
		case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER:
			item.implicit[sub].attrib_tier = val;
		break;
		case DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA:
			item.implicit[sub].attrib_extra = val;
		break;
	}
}

Script "DND Clientside Item Syncer" (int pnum, int var, int to, int extra) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	/*if(ConsolePlayerNumber() != pnum)
		Terminate;*/
	SetItemSyncValue(pnum, var & 0xFF, extra & 0xFFFF, extra >> 16, to, ((var & 0xFF00) >> 8) | (var & 0xFF0000));
	SetResultValue(0);
}

Script "DND Clientside Item Syncer Special" (int pnum, int var, int to, int extra) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;

	/*if(ConsolePlayerNumber() != pnum)
		Terminate;*/
	Delay(1);
	/*printbold(
		s:"calling sync value for pnum ", d:pnum, s:" cpnum: ", d:ConsolePlayerNumber(),
		s: " params: ", d:var & 0xFF, s: " ", d:extra & 0xFFFF, s: " ", d:extra >> 16, s:" ", d: to, s:" ", d: ((var & 0xFF00) >> 8) | (var & 0xFF0000)
	);*/
	SetItemSyncValue(pnum, var & 0xFF, extra & 0xFFFF, extra >> 16, to, ((var & 0xFF00) >> 8) | (var & 0xFF0000));
	SetResultValue(0);
}

Script "DND Clientside Item Syncer Field" (int var, int to, int extra) CLIENTSIDE {
	SetItemSyncValue(-1, var & 0xFF, extra & 0xFFFF, extra >> 16, to, ((var & 0xFF00) >> 8) | (var & 0xFF0000));
	SetResultValue(0);
}

Script "DND Clientside Weapon Mod Sync" (int wepid, int mod, int val, int tier) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;

	int pnum = wepid >> 16;
	wepid &= 0xFFFF;

	int source = mod >> 16;
	mod &= 0xFFFF;
	
	/*if(ConsolePlayerNumber() != pnum)
		Terminate;*/
	
	Player_Weapon_Infos[pnum][wepid].wep_mods[mod][source].val = val;
	Player_Weapon_Infos[pnum][wepid].wep_mods[mod][source].tier = tier;
	SetResultValue(0);
}

// add more things from wep_info_T in WeaponsDef here later
Script "DnD Clientside Weapon Property Sync" (int wepid, int pnum, int prop, int val) CLIENTSIDE {
	// do a switch-case for properties here
	Player_Weapon_Infos[pnum][wepid].quality = val;
	SetResultValue(0);
}

void SyncClientsideVariable_WeaponProperties(int pnum, int wepid) {
	// do a for loop for all properties we might add here to wep_info_T
	ACS_NamedExecuteWithResult("DnD Clientside Weapon Property Sync", wepid, pnum, 0, Player_Weapon_Infos[pnum][wepid].quality);
}

void SyncClientsideVariable_WeaponMods(int pnum, int wepid) {
	for(int i = 0; i < MAX_WEP_MODS; ++i) {
		for(int j = 0; j < DND_MAX_WEAPONMODSOURCES; ++j)
			ACS_NamedExecuteWithResult(
				"DND Clientside Weapon Mod Sync", 
				wepid | (pnum << 16), 
				i | (j << 16), 
				Player_Weapon_Infos[pnum][wepid].wep_mods[i][j].val,
				Player_Weapon_Infos[pnum][wepid].wep_mods[i][j].tier
			);
	}
}

void SyncItemData(int pnum, int itemid, int source, int wprev, int hprev, bool source_inv_except = false) {
	int i, j, h, bid;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	// synchronize the topleftboxid for all adjacent ones
	if(!source_inv_except && IsSourceInventoryView(raw_source)) {
		int w;
		// we must know previous height/width for proper sync
		if(wprev != -1)
			w = wprev;
		else
			w = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, itemid, -1, source);
		if(hprev != -1)
			h = hprev;
		else
			h = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, itemid, -1, source);
		for(i = 0; i < h; ++i)
			for(j = 0; j < w; ++j) {
				bid = itemid + j + i * MAXINVENTORYBLOCKS_VERT;
				ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), bid);
				ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), bid);
			}
	}
	else {
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), itemid);
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), itemid);
	}

	//Log(s:"syncing item at field pos ", d:itemid, s:" type ", d:GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), s:" for player ", d:pnum);
	
	// skip top left box and item type, we handled it
	for(i = DND_SYNC_ITEMBEGIN + 2; i <= DND_SYNC_ITEMBASE ; ++i) {
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, i | payload, GetItemSyncValue(pnum, i, itemid, -1, source), itemid);
	}

	// sync implicits
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID; j <= DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
	}
	
	// sync attributes
	h = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source);
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMSATTRIBCOUNT | payload, h, itemid);
	for(i = 0; i < h; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
	}

	MarkVSyncItemDirty();
}

void SyncItemData_Special(int pnum, int itemid, int source) {
	int i, j, bid;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	
	int w = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, itemid, -1, source);
	int h = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, itemid, -1, source);
	
	// synchronize the topleftboxid for all adjacent ones
	if(IsSourceInventoryView(raw_source)) {
		for(i = 0; i < h; ++i)
			for(j = 0; j < w; ++j) {
				bid = itemid + j + i * MAXINVENTORYBLOCKS_VERT;
				ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), bid);
				ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), bid);
			}
	}
	else {
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), itemid);
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), itemid);
	}

	for(i = DND_SYNC_ITEMBEGIN + 2; i <= DND_SYNC_ITEMBASE ; ++i)
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, i | payload, GetItemSyncValue(pnum, i, itemid, -1, source), itemid);

	// sync implicits
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID; j <= DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
	}
	
	// sync attributes
	h = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source);
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMSATTRIBCOUNT | payload, h, itemid);
	for(i = 0; i < h; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
	}

	MarkVSyncItemDirty();
}

// this is a sync function that syncs data to all players
void SyncItemData_Field(int itemid) {
	int i, j, h;
	int payload = (DND_SYNC_ITEMSOURCE_FIELD << 8);
	// topleftboxid is 0 for field items, it doesnt make sense for them to have one
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer Field", DND_SYNC_ITEMTOPLEFTBOX | payload, 0, itemid);
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer Field", DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(-1, DND_SYNC_ITEMTYPE, itemid, -1, DND_SYNC_ITEMSOURCE_FIELD), itemid);


	for(i = DND_SYNC_ITEMBEGIN + 2; i <= DND_SYNC_ITEMBASE ; ++i)
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer Field", i | payload, GetItemSyncValue(-1, i, itemid, -1, DND_SYNC_ITEMSOURCE_FIELD), itemid);

	// sync implicits
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID; j <= DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer Field", j | payload, GetItemSyncValue(-1, j, itemid, i, DND_SYNC_ITEMSOURCE_FIELD), itemid | (i << 16));
	}
	
	// sync attributes
	h = GetItemSyncValue(-1, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, DND_SYNC_ITEMSOURCE_FIELD);
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer Field", DND_SYNC_ITEMSATTRIBCOUNT | payload, h, itemid);
	for(i = 0; i < h; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer Field", j | payload, GetItemSyncValue(-1, j, itemid, i, DND_SYNC_ITEMSOURCE_FIELD), itemid | (i << 16));
	}
}

void SyncItemStack(int pnum, int itemid, int source) {
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMSTACK | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, itemid, -1, source), itemid);
	
	MarkVSyncItemDirty();
}

void SyncItemStack_Delayed(int pnum, int itemid, int source) {
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMSTACK | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, itemid, -1, source), itemid);
	
	MarkVSyncItemDirty();
}

void SyncItemData_Null(int pnum, int itemid, int source, int wprev, int hprev, bool source_inv_except = false) {
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	
	if(!source_inv_except && IsSourceInventoryView(raw_source)) {
		int h, w;
		if(wprev != -1)
			w = wprev;
		else
			w = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, itemid, -1, source);
		if(hprev != -1)
			h = hprev;
		else
			h = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, itemid, -1, source);
		
		for(int i = 0; i < h; ++i)
			for(int j = 0; j < w; ++j) {
				ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, 0, itemid + j + i * MAXINVENTORYBLOCKS_VERT);
				ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTYPE | payload, DND_ITEM_NULL, itemid + j + i * MAXINVENTORYBLOCKS_VERT);
			}
	}
	else {
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, 0, itemid);
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTYPE | payload, DND_ITEM_NULL, itemid);
	}
	
	for(i = DND_SYNC_ITEMBEGIN + 2; i <= DND_SYNC_ITEMBASE ; ++i)
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, i | payload, 0, itemid);

	// sync implicits
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		// sync this with -1
		ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID | payload, -1, itemid | (i << 16));
		for(j = DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL; j <= DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, j | payload, 0, itemid | (i << 16));
	}
	
	// sync attributes
	h = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source);
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMSATTRIBCOUNT | payload, 0, itemid);
	for(i = 0; i < h; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, j | payload, 0, itemid | (i << 16));
	}
}

void SyncItemAttributes(int pnum, int itemid, int source) {
	int i, j;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	int temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source);

	ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMSATTRIBCOUNT | payload, temp, itemid);
	
	// we now sync ilvl too
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMLEVEL | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, itemid, -1, source), itemid);

	for(i = 0; i < temp; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
	}

	MarkVSyncItemDirty();
}

void SyncItemQuality(int pnum, int itemid, int source) {
	int i, j, temp;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMQUALITY | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, itemid, -1, source), itemid);
	
	MarkVSyncItemDirty();
}

void SyncItemImplicits(int pnum, int itemid, int source) {
	int i;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);

	ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMCORRUPTED | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, itemid, -1, source), itemid);

	for(int j = 0; j < MAX_ITEM_IMPLICITS; ++j) {
		for(i = DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID; i <= DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA ; ++i) {
			ACS_NamedExecuteWithResult("DND Clientside Item Syncer", pnum, i | payload, GetItemSyncValue(pnum, i, itemid, j, source), itemid | (j << 16));
		}
	}

	MarkVSyncItemDirty();
}

void SyncAllItemData(int pnum, int source) {
	int i, j;
	if(source == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) {
		for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
			if(GlobalItemStorage.PlayerInventoryList[pnum][i].item_type != DND_ITEM_NULL)
				SyncItemData(pnum, i, source, 1, 1);
			else
				SyncItemData_Null(pnum, i, source, 1, 1);
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_ITEMSUSED) {
		for(i = 0; i < MAX_ITEMS_EQUIPPABLE; ++i) {
			if(GlobalItemStorage.Items_Used[pnum][i].item_type != DND_ITEM_NULL)
				SyncItemData(pnum, i, source, 1, 1);
			else
				SyncItemData_Null(pnum, i, source, 1, 1);
		}
	}
	else if(source == DND_SYNC_ITEMSOURCE_STASH) {
		for(i = 0; i < CheckInventory("DnD_PlayerInventoryPages"); ++i) {
			for(j = 0; j < MAX_INVENTORY_BOXES; ++j) {
				if(GlobalItemStorage.PlayerStashList[pnum][i][j].item_type != DND_ITEM_NULL)
					SyncItemData(pnum, j, source | (i << 16), 1, 1);
				else
					SyncItemData_Null(pnum, j, source | (i << 16), 1, 1);
			}
		}

		// sync the orbs page
		i = PAGEID_STASHTAB_ORBS;
		for(j = 0; j < MAX_INVENTORY_BOXES; ++j) {
			if(GlobalItemStorage.PlayerStashList[pnum][i][j].item_type != DND_ITEM_NULL)
				SyncItemData(pnum, j, source | (i << 16), 1, 1);
			else
				SyncItemData_Null(pnum, j, source | (i << 16), 1, 1);
		}
	}

	MarkVSyncItemDirty();
}

void SyncAllClientsideVariables(int pnum) {
	int i, j;
	// sync weapon mods
	for(i = 0; i < MAXWEPS; ++i) {
		SyncClientsideVariable_WeaponProperties(pnum, i);
		SyncClientsideVariable_WeaponMods(pnum, i);
	}
}

Script "DnD Request Mod Sync" (int pnum, int mod, int val) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	// Log(s:"cs set mod ", d:mod, s: " to val ", d:val);
	// Routed, not a raw value[] write: the server sent an attribute id and the client has to land it
	// in whichever storage that id lives in, or a migrated mod would read back zero on every client.
	WritePlayerModValue(pnum, mod, val);
	SetResultValue(0);
}

// Migrated stats are addressed by SLOT. A slot shared by two mods has no single attribute id that
// names it, so the id-keyed script above could not carry one even in principle.
Script "DnD Request Stat Sync" (int pnum, int slot, int val) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	PlayerModData[pnum].f[slot] = val;
	SetResultValue(0);
}

// Slot-keyed extra, the counterpart to "DnD Request Stat Sync". Used by the full resync, which walks
// storage rather than the attribute id space now that there is no id-keyed array left to walk.
Script "DnD Request Extra Sync" (int pnum, int slot, int val) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	PlayerModData[pnum].x[slot] = val;
	SetResultValue(0);
}

// The whole word, not one bit. Sending the word makes the client's copy a mirror of the server's
// rather than something it has to reconstruct, so a dropped update self corrects on the next one.
// The refcounts stay server side -- the client only ever needs to know whether a flag is on.
Script "DnD Request Flag Sync" (int pnum, int word, int val) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	PlayerModData[pnum].pflags[word] = val;
	SetResultValue(0);
}

Script "DnD Request Mod Sync (Special)" (int pnum, int mod, int val) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	// Log(s:"cs set mod ", d:mod, s: " to val ", d:val);
	Delay(const:1);
	WritePlayerModValue(pnum, mod, val);
	SetResultValue(0);
}

Script "DnD Request Mod Extra Sync" (int pnum, int mod, int val) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	// Log(s:"cs set mod ", d:mod, s: " to val ", d:val);
	WritePlayerModExtra(pnum, mod, val);
	SetResultValue(0);
}

Script "DnD Request Mod Extra Sync (Special)" (int pnum, int mod, int val) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	// Log(s:"cs set mod ", d:mod, s: " to val ", d:val);
	Delay(const:1);
	WritePlayerModExtra(pnum, mod, val);
	SetResultValue(0);
}

Script "DnD Handle Attribute Sync" (int pnum) {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;

	Delay(const:1);

	auto psync = GetPlayerAttributeSyncs(pnum);
	int cnt = psync.count;
	int mod, i;
	for(i = 0; i < cnt; ++i) {
		mod = psync.arr[i];
		ACS_NamedExecuteWithResult("DnD Request Mod Sync", pnum, mod, ReadPlayerModValue(pnum, mod));
	}

	ClearPlayerAttributeSync(pnum);

	cnt = psync.extras;
	if(cnt) {
		Delay(const:1);

		for(i = 0; i < cnt; ++i) {
			mod = psync.arr_extra[i];
			ACS_NamedExecuteWithResult("DnD Request Mod Extra Sync", pnum, mod, ReadPlayerModExtra(pnum, mod));
		}

		ClearPlayerAttributeExtraSync(pnum);
	}

	SetResultValue(0);
}

#endif
