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

// Everything SyncItemData_Null or _ClearFields would send field by field, rebuilt here from one
// command. w x h is the region whose topleft and type go null (0 x 0 for ClearFields).
void ApplyItemSyncClear(int pnum, int itemid, int source, int w, int h, int attribs) {
	int i, j;
	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itemid + j + i * MAXINVENTORYBLOCKS_VERT, -1, 0, source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid + j + i * MAXINVENTORYBLOCKS_VERT, -1, DND_ITEM_NULL, source);
		}

	for(i = DND_SYNC_ITEMBEGIN + 2; i <= DND_SYNC_ITEMBASE; ++i)
		SetItemSyncValue(pnum, i, itemid, -1, 0, source);

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, itemid, i, -1, source);
		for(j = DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL; j <= DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA; ++j)
			SetItemSyncValue(pnum, j, itemid, i, 0, source);
	}

	if(attribs > MAX_ITEM_ATTRIBUTES)
		attribs = MAX_ITEM_ATTRIBUTES;
	for(i = 0; i < attribs; ++i)
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			SetItemSyncValue(pnum, j, itemid, i, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, 0, source);
}

// var carries the source and page like the syncer's, with the attribute count in the low byte
Script "DND Clientside Item Clear" (int pnum, int var, int itemid, int dims) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	ApplyItemSyncClear(pnum, itemid, ((var & 0xFF00) >> 8) | (var & 0xFF0000), dims & 0xFF, (dims >> 8) & 0xFF, var & 0xFF);
	SetResultValue(0);
}

// -1 only on a server. Offline multiplayer runs "clientside" scripts on the server's own memory,
// where a sync deferred to next tic could land on top of newer data, so only a real server defers.
bool IsOnlineServer() {
	return ConsolePlayerNumber() == -1;
}

// To the owner's client alone when online, otherwise the old immediate run. Clients get here too --
// DECORATE starts server scripts on them (orb pickup) -- and NamedExecuteClientScript errors there.
void SendOwnerSync(str sname, int pnum, int a0 = 0, int a1 = 0, int a2 = 0, int a3 = 0) {
	if(IsOnlineServer())
		NamedExecuteClientScript(sname, pnum, a0, a1, a2, a3);
	else
		ACS_NamedExecuteWithResult(sname, a0, a1, a2, a3);
}

// Same, for a call site that used ACS_NamedExecuteAlways (three script args at most).
void SendOwnerScript(str sname, int pnum, int a0 = 0, int a1 = 0, int a2 = 0) {
	if(IsOnlineServer())
		NamedExecuteClientScript(sname, pnum, a0, a1, a2);
	else
		ACS_NamedExecuteAlways(sname, 0, a0, a1, a2);
}

// Inventory and stash go to their owner alone. Other clients read two grids: a trade offer (the
// partner) and equipped items (viewplayer, script 1006). Pseudo owners (merchant, ultimatum) are not
// players; all of these keep the broadcast.
void SendItemSync(str sname, int pnum, int var, int to, int extra) {
	int raw_source = (var >> 8) & 0xFF;
	if(pnum >= 0 && pnum < MAXPLAYERS && raw_source != DND_SYNC_ITEMSOURCE_TRADEVIEW && raw_source != DND_SYNC_ITEMSOURCE_ITEMSUSED)
		SendOwnerSync(sname, pnum, pnum, var, to, extra);
	else
		ACS_NamedExecuteWithResult(sname, pnum, var, to, extra);
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

// One command for a weapon whose mods are all zero, instead of one per mod and source.
Script "DND Clientside Weapon Mod Clear" (int wepid) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;

	int pnum = wepid >> 16;
	wepid &= 0xFFFF;

	for(int i = 0; i < MAX_WEP_MODS; ++i)
		for(int j = 0; j < DND_MAX_WEAPONMODSOURCES; ++j) {
			Player_Weapon_Infos[pnum][wepid].wep_mods[i][j].val = 0;
			Player_Weapon_Infos[pnum][wepid].wep_mods[i][j].tier = 0;
		}
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
	SendOwnerSync("DnD Clientside Weapon Property Sync", pnum, wepid, pnum, 0, Player_Weapon_Infos[pnum][wepid].quality);
}

bool WeaponHasAnyMods(int pnum, int wepid) {
	for(int i = 0; i < MAX_WEP_MODS; ++i)
		for(int j = 0; j < DND_MAX_WEAPONMODSOURCES; ++j)
			if(Player_Weapon_Infos[pnum][wepid].wep_mods[i][j].val || Player_Weapon_Infos[pnum][wepid].wep_mods[i][j].tier)
				return true;
	return false;
}

void SyncClientsideVariable_WeaponMods(int pnum, int wepid) {
	if(!WeaponHasAnyMods(pnum, wepid)) {
		SendOwnerSync("DND Clientside Weapon Mod Clear", pnum, wepid | (pnum << 16), 0, 0, 0);
		return;
	}

	for(int i = 0; i < MAX_WEP_MODS; ++i) {
		for(int j = 0; j < DND_MAX_WEAPONMODSOURCES; ++j)
			SendOwnerSync(
				"DND Clientside Weapon Mod Sync",
				pnum,
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
				SendItemSync("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), bid);
				SendItemSync("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), bid);
			}
	}
	else {
		SendItemSync("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), itemid);
		SendItemSync("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), itemid);
	}

	//Log(s:"syncing item at field pos ", d:itemid, s:" type ", d:GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), s:" for player ", d:pnum);
	
	// skip top left box and item type, we handled it
	for(i = DND_SYNC_ITEMBEGIN + 2; i <= DND_SYNC_ITEMBASE ; ++i) {
		SendItemSync("DND Clientside Item Syncer", pnum, i | payload, GetItemSyncValue(pnum, i, itemid, -1, source), itemid);
	}

	// sync implicits
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID; j <= DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA; ++j)
			SendItemSync("DND Clientside Item Syncer", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
	}
	
	// sync attributes
	h = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source);
	SendItemSync("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMSATTRIBCOUNT | payload, h, itemid);
	for(i = 0; i < h; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			SendItemSync("DND Clientside Item Syncer", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
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
				SendItemSync("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), bid);
				SendItemSync("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), bid);
			}
	}
	else {
		SendItemSync("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMTOPLEFTBOX | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itemid, -1, source), itemid);
		SendItemSync("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMTYPE | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, itemid, -1, source), itemid);
	}

	for(i = DND_SYNC_ITEMBEGIN + 2; i <= DND_SYNC_ITEMBASE ; ++i)
		SendItemSync("DND Clientside Item Syncer Special", pnum, i | payload, GetItemSyncValue(pnum, i, itemid, -1, source), itemid);

	// sync implicits
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID; j <= DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA; ++j)
			SendItemSync("DND Clientside Item Syncer Special", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
	}
	
	// sync attributes
	h = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source);
	SendItemSync("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMSATTRIBCOUNT | payload, h, itemid);
	for(i = 0; i < h; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			SendItemSync("DND Clientside Item Syncer Special", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
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
	SendItemSync("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMSTACK | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, itemid, -1, source), itemid);
	
	MarkVSyncItemDirty();
}

void SyncItemStack_Delayed(int pnum, int itemid, int source) {
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	SendItemSync("DND Clientside Item Syncer Special", pnum, DND_SYNC_ITEMSTACK | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, itemid, -1, source), itemid);
	
	MarkVSyncItemDirty();
}

// One "DND Clientside Item Clear" command in place of ~24 per-field ones.
void SyncItemData_Null(int pnum, int itemid, int source, int wprev, int hprev, bool source_inv_except = false) {
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int w = 1, h = 1;

	if(!source_inv_except && IsSourceInventoryView(raw_source)) {
		if(wprev != -1)
			w = wprev;
		else
			w = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, itemid, -1, source);
		if(hprev != -1)
			h = hprev;
		else
			h = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, itemid, -1, source);
	}

	int attribs = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source) & 0xFF;
	SendItemSync("DND Clientside Item Clear", pnum, (raw_source << 8) | (page << 16) | attribs, itemid, (w & 0xFF) | ((h & 0xFF) << 8));
}

// Everything a box owns ITSELF, zeroed -- but not topleftboxid or item_type, which for a cell in
// the middle of a multi-cell item belong to the item's top left box and are set by ITS sync.
//
// This exists for the one case the bulk resyncs skip: a box that is neither an item's top left nor
// empty. Its topleft and type get corrected by the owner, but width, height, image, subtype, level
// and the attribute list are per box and would keep whatever used to live there. A stale height is
// the dangerous one -- the crafting view lists a box when its type is craftable AND its height is
// non-zero, so a leftover height makes a middle cell look like a whole item, drawn with the previous
// occupant's image under the current owner's name.
// A 0 x 0 region: the clear leaves topleft and type to the owner's sync.
void SyncItemData_ClearFields(int pnum, int itemid, int source) {
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int attribs = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source) & 0xFF;
	SendItemSync("DND Clientside Item Clear", pnum, (raw_source << 8) | (page << 16) | attribs, itemid, 0);
}

void SyncItemAttributes(int pnum, int itemid, int source) {
	int i, j;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	int temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, itemid, -1, source);

	SendItemSync("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMSATTRIBCOUNT | payload, temp, itemid);
	
	// we now sync ilvl too
	SendItemSync("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMLEVEL | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, itemid, -1, source), itemid);

	for(i = 0; i < temp; ++i) {
		for(j = DND_SYNC_ITEMATTRIBUTES_ID; j <= DND_LAST_SYNC_TYPE; ++j)
			SendItemSync("DND Clientside Item Syncer", pnum, j | payload, GetItemSyncValue(pnum, j, itemid, i, source), itemid | (i << 16));
	}

	MarkVSyncItemDirty();
}

void SyncItemQuality(int pnum, int itemid, int source) {
	int i, j, temp;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);
	SendItemSync("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMQUALITY | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, itemid, -1, source), itemid);
	
	MarkVSyncItemDirty();
}

void SyncItemImplicits(int pnum, int itemid, int source) {
	int i;
	int page = source >> 16;
	int raw_source = source & 0xFFFF;
	int payload = (raw_source << 8) | (page << 16);

	SendItemSync("DND Clientside Item Syncer", pnum, DND_SYNC_ITEMCORRUPTED | payload, GetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, itemid, -1, source), itemid);

	for(int j = 0; j < MAX_ITEM_IMPLICITS; ++j) {
		for(i = DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID; i <= DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA ; ++i) {
			SendItemSync("DND Clientside Item Syncer", pnum, i | payload, GetItemSyncValue(pnum, i, itemid, j, source), itemid | (j << 16));
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
	PlayerModData[pnum].vals[slot] = val;
	SetResultValue(0);
}

// Slot-keyed extra, the counterpart to "DnD Request Stat Sync". Used by the full resync, which walks
// storage rather than the attribute id space now that there is no id-keyed array left to walk.
Script "DnD Request Extra Sync" (int pnum, int slot, int val) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	PlayerModData[pnum].extras[slot] = val;
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

// Same shape as the flag sync and for the same reason. spent_in is deliberately NOT sent: it is
// derived from these words, so shipping it too would give the client two copies of one fact that
// could disagree. The client recounts instead.
Script "DnD Request Spell Sync" (int pnum, int word, int val) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	SpellPlayerData[pnum].ranks[word] = val;
	SetResultValue(0);
}

// The aura/passive on-off word. Same shape as the rank sync above.
Script "DnD Request Spell Toggle Sync" (int pnum, int word, int val) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	SpellPlayerData[pnum].aura_on[word] = val;
	SetResultValue(0);
}

Script "DnD Request Perk Sync" (int pnum, int word, int val) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	PlayerModData[pnum].perks_packed[word] = val;
	RecountPerkPoints(pnum);
	SetResultValue(0);
}

// The unspent pool. Its real home is the PerkPoint item, but a client cannot read another actor's
// inventory and player TIDs are only ever assigned server side, so the tree's "can I afford this"
// test reads this mirror instead of the item it shadows.
Script "DnD Request Perk Point Sync" (int pnum, int val) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	PlayerModData[pnum].unspent = val;
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

// ---- Sync check: does this client's copy of its own items match the server's? ----
// Grids: 0 equipped, 1 inventory, 2 + page for the stash (the orb page is page PAGEID_STASHTAB_ORBS).
#define DND_SYNCCHECK_EQUIPPED 0
#define DND_SYNCCHECK_INVENTORY 1
#define DND_SYNCCHECK_STASH 2
#define DND_SYNCCHECK_LAST (DND_SYNCCHECK_STASH + PAGEID_STASHTAB_ORBS)
#define DND_SYNCCHECK_SEED 0x2C1B3C6D

// report codes carried in the box argument
#define DND_SYNCCHECK_MATCH -1
#define DND_SYNCCHECK_DIFFERS -2
#define DND_SYNCCHECK_RESYNCED -3

int GetSyncCheckSource(int grid) {
	if(grid == DND_SYNCCHECK_EQUIPPED)
		return DND_SYNC_ITEMSOURCE_ITEMSUSED;
	if(grid == DND_SYNCCHECK_INVENTORY)
		return DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
	return DND_SYNC_ITEMSOURCE_STASH | ((grid - DND_SYNCCHECK_STASH) << 16);
}

int GetSyncCheckBoxCount(int grid) {
	return grid == DND_SYNCCHECK_EQUIPPED ? MAX_ITEMS_EQUIPPABLE : MAX_INVENTORY_BOXES;
}

str GetSyncCheckGridName(int grid) {
	if(grid == DND_SYNCCHECK_EQUIPPED)
		return "equipped";
	if(grid == DND_SYNCCHECK_INVENTORY)
		return "inventory";
	if(grid == DND_SYNCCHECK_LAST)
		return "orb page";
	return StrParam(s:"stash page ", d:grid - DND_SYNCCHECK_STASH + 1);
}

int SyncCheckMix(int h, int v) {
	return (h ^ v) * 16777619;
}

// Only what the syncers mirror. An empty box, or the middle cell of a multi-cell item, mirrors
// nothing past topleft and type, so the rest of it is never compared.
int GetItemBoxSyncHash(int pnum, int box, int source) {
	int i, f;
	int tl = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, box, -1, source);
	int type = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, box, -1, source);
	int h = SyncCheckMix(SyncCheckMix(DND_SYNCCHECK_SEED, tl), type);

	if(type == DND_ITEM_NULL || (IsSourceInventoryView(source) && tl != box + 1))
		return h;

	for(f = DND_SYNC_ITEMBEGIN + 2; f <= DND_SYNC_ITEMBASE; ++f)
		h = SyncCheckMix(h, GetItemSyncValue(pnum, f, box, -1, source));

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i)
		for(f = DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID; f <= DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA; ++f)
			h = SyncCheckMix(h, GetItemSyncValue(pnum, f, box, i, source));

	int n = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, box, -1, source);
	h = SyncCheckMix(h, n);
	if(n > MAX_ITEM_ATTRIBUTES)
		n = MAX_ITEM_ATTRIBUTES;
	for(i = 0; i < n; ++i)
		for(f = DND_SYNC_ITEMATTRIBUTES_ID; f <= DND_LAST_SYNC_TYPE; ++f)
			h = SyncCheckMix(h, GetItemSyncValue(pnum, f, box, i, source));
	return h;
}

int GetItemGridSyncHash(int pnum, int grid) {
	int source = GetSyncCheckSource(grid);
	int n = GetSyncCheckBoxCount(grid);
	int h = DND_SYNCCHECK_SEED;
	for(int i = 0; i < n; ++i)
		h = SyncCheckMix(h, GetItemBoxSyncHash(pnum, i, source));
	return h;
}

// Console: pukename "DnD Sync Check". Run it idle -- a sync still in flight reads as a difference.
Script "DnD Sync Check" (void) NET CLIENTSIDE {
	int pnum = PlayerNumber();
	if(pnum != ConsolePlayerNumber())
		Terminate;

	if(GameType() == GAME_SINGLE_PLAYER) {
		Log(s:"Sync check: single player keeps one copy, nothing to compare.");
		Terminate;
	}

	Log(s:"Sync check: asking the server...");
	NamedRequestScriptPuke("DnD Sync Check Grid", DND_SYNCCHECK_EQUIPPED, GetItemGridSyncHash(pnum, DND_SYNCCHECK_EQUIPPED));
	NamedRequestScriptPuke("DnD Sync Check Grid", DND_SYNCCHECK_INVENTORY, GetItemGridSyncHash(pnum, DND_SYNCCHECK_INVENTORY));

	int pages = CheckInventory("DnD_PlayerInventoryPages");
	for(int i = 0; i < pages && i < PAGEID_STASHTAB_ORBS; ++i)
		NamedRequestScriptPuke("DnD Sync Check Grid", DND_SYNCCHECK_STASH + i, GetItemGridSyncHash(pnum, DND_SYNCCHECK_STASH + i));
	NamedRequestScriptPuke("DnD Sync Check Grid", DND_SYNCCHECK_LAST, GetItemGridSyncHash(pnum, DND_SYNCCHECK_LAST));
}

// Server side: answers a match with one line, a mismatch with its hash of every box in the grid.
Script "DnD Sync Check Grid" (int grid, int client_hash) NET {
	int pnum = PlayerNumber();
	if(pnum < 0 || grid < DND_SYNCCHECK_EQUIPPED || grid > DND_SYNCCHECK_LAST)
		Terminate;

	if(GetItemGridSyncHash(pnum, grid) == client_hash) {
		NamedExecuteClientScript("DnD Sync Check Report", pnum, grid, DND_SYNCCHECK_MATCH, 0);
		Terminate;
	}

	NamedExecuteClientScript("DnD Sync Check Report", pnum, grid, DND_SYNCCHECK_DIFFERS, 0);
	int source = GetSyncCheckSource(grid);
	int n = GetSyncCheckBoxCount(grid);
	for(int i = 0; i < n; ++i)
		NamedExecuteClientScript("DnD Sync Check Report", pnum, grid, i, GetItemBoxSyncHash(pnum, i, source));
}

Script "DnD Sync Check Report" (int grid, int box, int server_hash) CLIENTSIDE {
	int pnum = PlayerNumber();
	if(pnum != ConsolePlayerNumber())
		Terminate;

	str name = GetSyncCheckGridName(grid);
	if(box == DND_SYNCCHECK_RESYNCED)
		Log(s:"Resync: everything resent. Run \"DnD Sync Check\" again to confirm.");
	else if(box == DND_SYNCCHECK_MATCH)
		Log(s:"Sync check: ", s:name, s:" matches.");
	else if(box == DND_SYNCCHECK_DIFFERS)
		Log(s:"\cgSync check: ", s:name, s:" DIFFERS\c- -- mismatched boxes are listed (none listed: a sync was in flight, run it again). pukename \"DnD Resync\" repairs it.");
	else {
		int source = GetSyncCheckSource(grid);
		if(GetItemBoxSyncHash(pnum, box, source) != server_hash)
			Log(
				s:"\cgSync check: ", s:name, s:" box ", d:box, s:" differs\c- (client has type ",
				d:GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, box, -1, source), s:", topleft ",
				d:GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, box, -1, source), s:")"
			);
	}
	SetResultValue(0);
}

#endif
