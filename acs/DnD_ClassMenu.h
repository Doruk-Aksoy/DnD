#ifndef DND_CLASSMENU
#define DND_CLASSMENU

#include "Database/DnD_DatabaseRows.h"

// NOTE: DnD_SelectedInventoryBox is used to tell the system player wants to "overwrite" the slot, ie. setchar call
// DnD_SelectedCharmBox is used to load a picked character as-is

typedef struct {
	int classid;
	int level;
	int exp;
	int hp;
	int credit;
	int budget;
} loaded_pdata_T;
loaded_pdata_T LoadedPlayerData[DND_MAX_CHARS]; // this is going to be used clientside only, no need for other players to be aware of its value so we can store it on each player's own client

bool HasChosenClass() {
	return CheckInventory("DnD_Character");
}

Script "DnD Class Select" (int isSinglePlayer) NET CLIENTSIDE {
	// close the previous menu
	ConsoleCommand("closemenu");

	int cpnum = ConsolePlayerNumber(), r;

	// don't allow duplicates to be run
	if(GetUserCVar(cpnum, "dnd_onclassmenu"))
		Terminate;

	SetUserCVar(cpnum, "dnd_onclassmenu", true);
	CselStart(GetUserCVar(cpnum, "dnd_playerclass"));

	while(PlayerIsSpectator(cpnum) || isSinglePlayer) {
		r = ClassSelectTic(GetPlayerInput(-1, INPUT_BUTTONS), GetPlayerInput(-1, INPUT_OLDBUTTONS));
		if(r > 0) {
			LocalAmbientSound("RPG/MenuChoose", 127);
			SetUserCVar(cpnum, "dnd_playerclass", GetCsel().cur);
			if(!isSinglePlayer)
				ConsoleCommand("join");
			break;
		}
		if(r < 0) {
			LocalAmbientSound("RPG/MenuClose", 127);
			break;
		}
		DrawClassSelect();
		Delay(const:1);
	}

	SetUserCVar(cpnum, "dnd_onclassmenu", false);
	ACS_NamedExecuteAlways("DnD Class Select Cleanup", 0);

	if(isSinglePlayer)
		NamedRequestScriptPuke("DnD Set Class", GetCVar("dnd_playerclass"), -1, -1);
}

Script "DnD Set Class" (int class_id, int cpnum) NET {
	if(cpnum == -1)
		cpnum = 0;
		
	while(!ActivatorTID())
		Delay(const:1);
		
	int tid = ActivatorTID();

	bool notSet = CheckActorInventory(tid, "DnD_Character") != class_id + 1;
	SetActorInventory(tid, "DnD_Character", class_id + 1);
	DoPreClassAdjustments();
	
	if(notSet)
		SetActorState(tid, "Spawn");
		
	// sync this to clients
	ACS_NamedExecuteWithResult("DnD Sync Set Class", tid, class_id, notSet);
}

Script "DnD Sync Set Class" (int tid, int class_id, int notSet) CLIENTSIDE {
	SetActorInventory(tid, "DnD_Character", class_id + 1);
}

str GetClassPerkDisplay(str cprefix, bool isMenu) {
	static str color_codes[4] = { "\cd", "\ck", "\ci", "\cg" };
	str toShow = StrParam(s:"\cq", l:GetClassLabel(cprefix, DND_CLASS_LABEL_PERK1), s:" - ", l:"CLASS_INNATE", s:"\n\cq--------------------\n", l:GetClassLabel(cprefix, DND_CLASS_LABEL_PERK1_DESC), s:"\n\n");
	int lvl = isMenu ? CheckInventory("Level") : 0;
	for(int i = 1; i <= 4; ++i) {
		if(!isMenu)
			toShow = StrParam(s:toShow, s:color_codes[i - 1], l:GetClassLabel(cprefix, DND_CLASS_LABEL_PERK1 + i), s:" - ", l:"DND_STAT18", s:" ", d:i * DND_PERK_REGULARTHRESHOLD, s:"\n", s:color_codes[i - 1], s:"--------------------\n", l:GetClassLabel(cprefix, DND_CLASS_LABEL_PERK1_DESC + i), s:"\n\n");
		else {
			str col = color_codes[i - 1];
			str txt_col = "";
			if(lvl < i * DND_PERK_REGULARTHRESHOLD) {
				col = "\cm";
				txt_col = "\cm";
			}
			toShow = StrParam(s:toShow, s:col, l:GetClassLabel(cprefix, DND_CLASS_LABEL_PERK1 + i), s:" - ", l:"DND_STAT18", s:" ", d:i * DND_PERK_REGULARTHRESHOLD, s:"\n", s:col, s:"--------------------\n", s:txt_col, l:GetClassLabel(cprefix, DND_CLASS_LABEL_PERK1_DESC + i), s:"\n\n");
		}	
	}
	return toShow;
}

Script "DnD Class Select Cleanup" (void) CLIENTSIDE {
	if(PlayerNumber() != ConsolePlayerNumber())
		Terminate;
	ClearClassSelect();
}

Script "DnD Character Select Cleanup" (void) CLIENTSIDE {
	if(PlayerNumber() != ConsolePlayerNumber())
		Terminate;
	ClearCharLoad();
}

void RunClassLoadScripts() {
	ACS_NamedExecuteAlways("DnD Character Select Animated", 0);
	ACS_NamedExecuteAlways("DnD Character Load Inputs", 0);
}

// the load timer and the players still loading, shown by the load screen's draw
Script "DnD Character Select Animated" (void) CLIENTSIDE {
	int cpnum = ConsolePlayerNumber();
	int pnum = PlayerNumber();
	if(cpnum != pnum)
		Terminate;

	int lim = TICRATE * Clamp_Between(GetCVar("dnd_loadtime"), DND_HARDCORE_LOADTIME, 100);
	int counter = 0;
	int temp;

	while(counter < lim && PlayerInGame(pnum)) {
		temp = CheckInventory("DnD_PlayersNotReady_Sync");
		if(!CheckInventory("CanLoad")) {
			if(temp > 1)
				GetCloadMsg().msg = StrParam(l:"DND_TEXT_WAITINGFOR", s:" \cd", d:temp, s:"\c- ", l:"DND_TEXT_PLAYERSTOLOAD");
			else if(temp == 1)
				GetCloadMsg().msg = StrParam(l:"DND_TEXT_WAITINGFOR", s:" \cd", d:temp, s:"\c- ", l:"DND_TEXT_PLAYERTOLOAD");
			else
				break;
		}
		else if (CheckInventory("PlayerIsLoggedIn"))
			GetCloadMsg().msg = StrParam(l:"DND_TEXT_YOUHAVE", s:" \cd", d:(lim - counter) / TICRATE, s:"\c- ", l:"DND_TEXT_LOADCHARWARNING");
		else
			GetCloadMsg().msg = StrParam(l:"DND_TEXT_YOUHAVE", s:" \cd", d:(lim - counter) / TICRATE, s:"\c- ", l:"DND_TEXT_LOADLOGINWARNING");
		Delay(const:1);
		++counter;
	}
}

Script "DnD Character Load Inputs" (void) CLIENTSIDE {
	int cpnum = ConsolePlayerNumber();
	int pnum = PlayerNumber();
	if(cpnum != pnum)
		Terminate;

	int boxid = MAINBOX_NONE, boxid_prev = MAINBOX_NONE;
	int i;
	bool sendInput = false;
	bool shown_first = false;

	CloadStart();

	while(PlayerInGame(pnum) && CheckInventory("PlayersNotReady")) {
		// the slots answer clicks once logged in; Load is off once a load went through
		boxid = CloadBox(CloadTic(GetPlayerInput(-1, INPUT_BUTTONS), GetPlayerInput(-1, INPUT_OLDBUTTONS)));
		if(boxid == MBOX_1 && CheckInventory("DnD_RemoveBoxSignal"))
			boxid = MAINBOX_NONE;
		if(boxid != boxid_prev && boxid != MAINBOX_NONE)
			LocalAmbientSound("RPG/MenuMove", 127);
		boxid_prev = boxid;
		SetInventory("ActivePopupBox", boxid);

		// on login, open on the first saved character
		if(!shown_first && CheckInventory("PlayerIsLoggedIn")) {
			i = 0;
			while(i < DND_MAX_CHARS && !LoadedPlayerData[i].classid)
				++i;
			if(i < DND_MAX_CHARS)
				CloadShow(i);
			shown_first = true;
		}
		DrawCharLoad(boxid);

		// check inputs
		ListenMouseInput();
		sendInput = CheckInventory("MenuInput") != 0;
		if(sendInput) {
			// server gets a few extra info in boxid
			if(!MenuInputData[pnum][DND_MENUINPUT_PAYLOAD])
				MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] = (boxid | MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK]);
			i = PlayerNumber() | (CheckInventory("MenuInput") << 16);
			// guarantee nonzero input
			if(i)
				NamedRequestScriptPuke("DND Server Box Receive - CharLoad", i, MenuInputData[pnum][DND_MENUINPUT_PAYLOAD]);
		}

		Delay(const:1);

		// retry ack
		if(CheckInventory("DND_ACK")) {
			SetInventory("MenuInput", 0);
			MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] = 0;
			MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = 0;
		}
	}

	SetInventory("ActivePopupBox", 0);

	ACS_NamedExecuteAlways("DnD Character Select Cleanup", 0);
}

Script "DnD Retry Sending UntiL ACK - CharLoad" (int payload1, int payload2) CLIENTSIDE {
	if(!payload1 || CheckInventory("DND_ACKLoop") || ConsolePlayerNumber() != PlayerNumber())
		Terminate;
	while(!CheckInventory("DnD_ACK")) {
		//Log(s:"running till ack received with ", d:payload1, s: " ", d:payload2, s: " ", d:mainboxid);
		//Log(s:"trying to send prev item ", d:payload2 >> 16);
		NamedRequestScriptPuke("DND Server Box Receive - CharLoad", payload1, payload2);
		Delay(const:1);
	}
	TakeInventory("DND_ACKLoop", 1);
}

Script "DND Server Box Receive - CharLoad" (int pnum, int boxid) NET {
	// don't let garbage data slip in
	if(!pnum)
		Terminate;
	int temp = pnum >> 16;
	pnum &= 0xFFFF;

	if(!IsRequestFromPlayer(pnum) || !CheckInventory("CanLoad"))
		Terminate;
		
	if(!MenuInputData[pnum][DND_MENUINPUT_DELAY]) {
		SetActivator(pnum + P_TIDSTART);
		MenuInputData[pnum][DND_MENUINPUT_DELAY] = DND_MENU_INPUTDELAYTICS;
		MenuInputData[pnum][DND_MENUINPUT] = temp;
		GiveInventory("DND_ACK", 1);
		
		if(HasLeftClicked(pnum)) {
			if(boxid == MBOX_1 && !CheckInventory("DnD_RemoveBoxSignal")) { 
				// confirmation box
				// we only allow clicks here to register if player clicked a character id prior
				int loadExistingOrCreate = CheckInventory("DnD_SelectedCharmBox");
				int replaceExisting = CheckInventory("DnD_SelectedInventoryBox");
				str pacc = GetPlayerAccountName(pnum);
				
				if(loadExistingOrCreate || replaceExisting) {
					// proceed with confirmation, call the related functions to load this player
					LocalAmbientSound("RPG/MenuChoose", 127);
					
					if(loadExistingOrCreate) {
						// if we have a character there, load it
						if(GetDBEntry(GetCharField(DND_DB_CLASSID, loadExistingOrCreate - 2), pacc)) {
							loadExistingOrCreate = ACS_NamedExecuteWithResult("DnD Load Char", loadExistingOrCreate - 2);
						}
						else {
							// setchar to this instead and create fresh
							loadExistingOrCreate = ACS_NamedExecuteWithResult("DnD Set Char", loadExistingOrCreate - 2);
						}
						GiveInventory("DnD_RemoveBoxSignal", 1);
					}
					else {
						// setchar to this instead and create fresh
						temp = ACS_NamedExecuteWithResult("DnD Set Char", replaceExisting - 2);
						if(temp != DND_LOGIN_CREATECHAROK) {
							temp = ACS_NamedExecuteWithResult("DnD Load Char", replaceExisting - 2);
							GiveInventory("DnD_RemoveBoxSignal", 1);
						}
					}
				}
			}
			else if(boxid != MAINBOX_NONE) {
				// any character select box, ie. Char 2 etc.
				LocalAmbientSound("RPG/MenuChoose", 127);
				SetInventory("DnD_SelectedCharmBox", boxid);
				SetInventory("DnD_SelectedInventoryBox", 0);
				SendOwnerScript("DnD Character Data Display On Click", PlayerNumber(), boxid - 2);
			}
		}
		else if(HasRightClicked(pnum)) {
			// right click is to overwrite an existing character -- dont let this be CONFIRM box
			if(boxid > MBOX_1) {
				LocalAmbientSound("RPG/MenuChoose", 127);
				SetInventory("DnD_SelectedInventoryBox", boxid);
				SetInventory("DnD_SelectedCharmBox", 0);
				SendOwnerScript("DnD Character Data Display On Click", PlayerNumber(), boxid - 2);
			}
		}
		
		ClearPlayerInput(pnum, true);
		
		Delay(const:DND_MENU_INPUTDELAYTICS);
		MenuInputData[pnum][DND_MENUINPUT_DELAY] = 0;
	}
}

Script "DnD Character Data Display On Click" (int char_id) CLIENTSIDE {
	if(PlayerNumber() != ConsolePlayerNumber())
		Terminate;
	CloadShow(char_id);
}

Script "DnD Character Data Display Store" (int pnum) {
	// fill in the display array since player logged in and sync it with client
	int i;
	if(!PlayerIsLoggedIn(pnum)) {
		for(i = 0; i < DND_MAX_CHARS; ++i)
			ACS_NamedExecuteWithResult("DnD Character Data Display Sync - NULL", pnum, i);
		Terminate;
	}
	
	str pacc = GetPlayerAccountName(pnum);
	for(i = 0; i < DND_MAX_CHARS; ++i) {
		int class_id = GetDBEntry(GetCharField(DND_DB_CLASSID, i), pacc);
		// player has a character on this char id with the classid = class_id, so store it for display now
		if(class_id) {
			int level = GetDBEntry(GetCharField(DND_DB_LEVEL, i), pacc);
			int exp = GetDBEntry(GetCharField(DND_DB_EXP, i), pacc);
			int hp = GetDBEntry(GetCharField(DND_DB_HEALTH, i), pacc);
			int credit = GetDBEntry(GetCharField(DND_DB_CREDIT, i), pacc);
			int budget = GetDBEntry(GetCharField(DND_DB_BUDGET, i), pacc);

			//Log(s:"send data - charid and classid level exp hp credit ", d:i, s: " ", d:class_id, s: " ", d:level, s: " ", d:exp, s: " ", d:hp, s: " ", d:credit);

			ACS_NamedExecuteWithResult(
				"DnD Character Data Display Sync - 1", 
				pnum | (class_id << MAXPLAYERS_BITS) | (level << 10) | (i << 17),
				exp,
				hp,
				credit
			);

			ACS_NamedExecuteWithResult(
				"DnD Character Data Display Sync - 2", 
				pnum | (i << 17),
				budget
			);
		}
		else
			ACS_NamedExecuteWithResult("DnD Character Data Display Sync - NULL", pnum, i);
	}
	
	SetResultValue(0);
}

Script "DnD Character Data Display Sync - 1" (int payload, int exp, int hp, int credit) CLIENTSIDE {
	int pnum = payload & MAXPLAYERS_MASK;

	if(pnum != ConsolePlayerNumber())
		Terminate;
	
	int class_id = (payload >> MAXPLAYERS_BITS) & 0xF;
	int level = (payload >> (MAXPLAYERS_BITS + 4)) & 0x7F;
	int char_id = (payload >> (MAXPLAYERS_BITS + 11));

	//Log(s:"sync charid and classid level exp hp credit ", d:char_id, s: " ", d:class_id, s: " ", d:level, s: " ", d:exp, s: " ", d:hp, s: " ", d:credit);

	LoadedPlayerData[char_id].classid = class_id;
	LoadedPlayerData[char_id].exp = exp;
	LoadedPlayerData[char_id].hp = hp;
	LoadedPlayerData[char_id].level = level;
	LoadedPlayerData[char_id].credit = credit;

	SetResultValue(0);
}

Script "DnD Character Data Display Sync - 2" (int payload, int budget) CLIENTSIDE {
	int pnum = payload & MAXPLAYERS_MASK;

	if(pnum != ConsolePlayerNumber())
		Terminate;

	int char_id = (payload >> (MAXPLAYERS_BITS + 11));

	//Log(s:"sync charid part2 ", d:char_id);

	LoadedPlayerData[char_id].budget = budget;

	SetResultValue(0);
}

Script "DnD Character Data Display Sync - NULL" (int pnum, int char_id) CLIENTSIDE {
	if(pnum != ConsolePlayerNumber())
		Terminate;

	LoadedPlayerData[char_id].classid = 0;
	LoadedPlayerData[char_id].level = 0;
	LoadedPlayerData[char_id].exp = 0;
	LoadedPlayerData[char_id].hp = 0;
	LoadedPlayerData[char_id].credit = 0;
	LoadedPlayerData[char_id].budget = 0;

	SetResultValue(0);
}

#endif
