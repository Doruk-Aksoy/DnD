#ifndef DND_ACTIVITY_IN
#define DND_ACTIVITY_IN

/* 
	Hold any kind of player **ORB** activity here.
	
	List of activities to be recorded:
	
		* Exp gain
		* Credit gain
		* Budget gain from orb
		* Level gain
		* Free talent points
		* Perk X change
		* Attribute X change
		* Unused attribute points or perk points
		* Bonuses from orbs obtained
		* Player's inventory is saved after each change forcibly. No way around it. I don't want to keep two copies of the whole inventory...

	In the future, maybe record everything? Might be too resource demanding.
*/

#define MAX_ACCOUNTNAME_LEN 64
typedef struct {
	// max player account name len 64
	int player_account[MAX_ACCOUNTNAME_LEN];

	int exp;
	int credit;
	int budget;
	int level;
	int perk_change[DND_MAX_PERKS];
	int attribute_change[DND_MAX_ATTRIBUTES];
	int free_perks;
	int free_attributes;
	int discarded_weapons;

	int loot_penalty;
	int vote_skips;
	str visited_maps[MAX_MAPS_RECORDED];

	// these are not related to active things that are saved but they are forever kept while players are in the server
	int total_exp;
	
	int char_id;
	int stash_pages;
} player_activity_T;

global player_activity_T 18: PlayerActivities[MAXPLAYERS];

void SetPlayerExp(int pnum, int val) {
	PlayerActivities[pnum].total_exp = val;
}

int GetPlayerExp(int pnum) {
	return PlayerActivities[pnum].total_exp;
}

void AddPlayerExp(int pnum, int val) {
	PlayerActivities[pnum].total_exp += val;
}

Script "DnD Sync Player Exp" (int pnum, int val) CLIENTSIDE {
	PlayerActivities[pnum].total_exp = val;
	SetResultValue(0);
}

// copy stored characters into a string
str RecoverPlayerAccountName(int pnum) {
	str result = "";
	
	for(int i = 0; i < MAX_ACCOUNTNAME_LEN && PlayerActivities[pnum].player_account[i]; ++i) {
		result = StrParam(s:result, c:PlayerActivities[pnum].player_account[i]);
	}
	
	return result;
}

enum {
	DND_ACTIVITY_EXP,
	DND_ACTIVITY_CREDIT,
	DND_ACTIVITY_BUDGET,
	DND_ACTIVITY_LEVEL,
	DND_ACTIVITY_TALENT,
	DND_ACTIVITY_PERK,
	DND_ACTIVITY_ATTRIBUTE,
	DND_ACTIVITY_PERKPOINT,
	DND_ACTIVITY_ATTRIBUTEPOINT,
	DND_ACTIVITY_WEAPONDISCARD
};

void UpdateActivity(int pnum, int activity, int val, int extra) {
	switch(activity) {
		case DND_ACTIVITY_EXP:
			PlayerActivities[pnum].exp = val;
		break;
		case DND_ACTIVITY_CREDIT:
			PlayerActivities[pnum].credit += val;
		break;
		case DND_ACTIVITY_BUDGET:
			PlayerActivities[pnum].budget += val;
		break;
		case DND_ACTIVITY_LEVEL:
			PlayerActivities[pnum].level += val;
		break;
		case DND_ACTIVITY_PERK:
			PlayerActivities[pnum].perk_change[extra] += val;
			//printbold(s:"activity for perk ", d:extra, s: " became ", d:PlayerActivities[pnum].perk_change[extra]);
		break;
		case DND_ACTIVITY_ATTRIBUTE:
			PlayerActivities[pnum].attribute_change[extra] += val;
			//printbold(s:"activity for attribute ", d:extra, s: " became ", d:PlayerActivities[pnum].attribute_change[extra]);
		break;
		case DND_ACTIVITY_PERKPOINT:
			PlayerActivities[pnum].free_perks += val;
		break;
		case DND_ACTIVITY_ATTRIBUTEPOINT:
			PlayerActivities[pnum].free_attributes += val;
		break;
		case DND_ACTIVITY_WEAPONDISCARD:
			// expects 1 to slotmax + 1, not 0 based
			if(!extra) {
				// extra is 0 means we increment
				if(val < 0) {
					val = -val;
					--val;
					PlayerActivities[pnum].discarded_weapons &= ~(1 << val);
				}
				else {
					--val;
					PlayerActivities[pnum].discarded_weapons |= (1 << val);
				}
			}
			else if(extra == 1) {
				// just update it
				PlayerActivities[pnum].discarded_weapons = val;
			}
			else {
				// cycle temp weapon ids
				val = PlayerActivities[pnum].discarded_weapons >> 8;
				val = (val + 1) % (LAST_SLOT9_WEAPON - FIRST_SLOT9_WEAPON + 3);
				PlayerActivities[pnum].discarded_weapons = (PlayerActivities[pnum].discarded_weapons & 0xFF) | (val << 8);
			}
			// we want to sync this one to the client
			ACS_NamedExecuteWithResult("DnD Player Weapon Discard Sync", pnum, PlayerActivities[pnum].discarded_weapons);
		break;
	}
}

Script "DnD Player Weapon Discard Sync" (int pnum, int val) CLIENTSIDE {
	if(pnum != ConsolePlayerNumber())
		Terminate;
	PlayerActivities[pnum].discarded_weapons = val;
	SetResultValue(0);
}

bool HasPlayerDiscardedSlot(int pnum, int slot) {
	// expects 0 indexed
	return PlayerActivities[pnum].discarded_weapons & (1 << slot);
}

int GetPlayerPreferredTempWeapon(int pnum) {
	return (PlayerActivities[pnum].discarded_weapons >> 8) - 2;
}

void ResetPlayerActivities(int pnum, bool hardReset) {
	int i, j;
	PlayerActivities[pnum].exp = 0;
	PlayerActivities[pnum].credit = 0;
	PlayerActivities[pnum].budget = 0;
	PlayerActivities[pnum].level = 0;
	PlayerActivities[pnum].free_perks = 0;
	PlayerActivities[pnum].free_attributes = 0;
	
	if(hardReset) {
		// only if someone quits or spectates should the hard reset be resetting these
		PlayerActivities[pnum].char_id = 0;
		PlayerActivities[pnum].discarded_weapons = 0;
		ACS_NamedExecuteWithResult("DnD Player Weapon Discard Sync", pnum, 0);
		PlayerActivities[pnum].stash_pages = DND_BASE_STASH_PAGES;
		PlayerActivities[pnum].total_exp = 0; // DONT CHANGE THIS THING'S LOCATION!

		PlayerActivities[pnum].vote_skips = 0;
		for(i = 0; i < MAX_MAPS_RECORDED; ++i)
			PlayerActivities[pnum].visited_maps[i] = "";

		ACS_NamedExecuteAlways("DnD Loot Penalty Text", 0, pnum, 0);

		for(i = 0; i < MAX_ACCOUNTNAME_LEN; ++i)
			PlayerActivities[pnum].player_account[i] = 0;
	}
	
	for(i = 0; i < DND_MAX_PERKS; ++i)
		PlayerActivities[pnum].perk_change[i] = 0;
	for(i = 0; i < DND_MAX_ATTRIBUTES; ++i)
		PlayerActivities[pnum].attribute_change[i] = 0;
}

void InsertMapVisited(int pnum, str mapname) {
	// check if this map occurs on the list, dont fill it up otherwise
	int i;
	str s;

	//Log(s:"check maplist");
	//for(i = 0; i < MAX_MAPS_RECORDED; ++i)
	//	Log(s:PlayerActivities[pnum].visited_maps[i]);

	for(i = 0; i < MAX_MAPS_RECORDED; ++i) {
		s = PlayerActivities[pnum].visited_maps[i];
		// i hope people dont name their maps 1 Letter... or number...
		if(StrLen(s) > 1 && !StrCmp(s, mapname)) {
			PlayerActivities[pnum].loot_penalty = 1;
			Log(s:"Map repeated: ", s:mapname, s:"! Loot is being reduced!");
			return;
		}
	}

	// shift all to right and insert newest to id 0
	for(i = MAX_MAPS_RECORDED - 1; i > 0; --i)
		PlayerActivities[pnum].visited_maps[i] = PlayerActivities[pnum].visited_maps[i - 1];
	PlayerActivities[pnum].visited_maps[0] = mapname;

	//Log(s:"current list");
	//for(i = 0; i < MAX_MAPS_RECORDED; ++i)
	//	Log(s:PlayerActivities[pnum].visited_maps[i]);
}

#endif
