#ifndef DND_CLASSMENU
#define DND_CLASSMENU

#include "DnD_Mugshot.h"
#include "DnD_DatabaseRows.h"

#define DND_CLASSMENU_BUTTON_DELAY 4
#define DND_MAX_CHARACTERSEL_ITEMS 24

enum {
	DND_CLASSMENU_LEFTBUTTON_BIT = 0b0001,
	DND_CLASSMENU_RIGHTBUTTON_BIT = 0b0010
};

typedef struct {
	int classid;
	int level;
	int exp;
	int hp;
	int credit;
	int budget;
} loaded_pdata_T;
loaded_pdata_T LoadedPlayerData[DND_MAX_CHARS]; // this is going to be used clientside only, no need for other players to be aware of its value so we can store it on each player's own client

void ResetLoadedPlayerDisplayData() {
	for(int i = 0; i < DND_MAX_CHARS; ++i) {
		LoadedPlayerData[i].classid = 0;
		LoadedPlayerData[i].level = 0;
		LoadedPlayerData[i].exp = 0;
		LoadedPlayerData[i].hp = 0;
		LoadedPlayerData[i].credit = 0;
		LoadedPlayerData[i].budget = 0;
	}
}

enum {
	DND_CLASSMENU_SELECTID = 1,
	DND_CLASSMENU_CLASSID,
	DND_CLASSMENU_CLASSEXPID,
	DND_CLASSMENU_LEFTIMG,
	DND_CLASSMENU_RIGHTIMG,
	DND_CLASSMENU_MUGIMG,
	DND_CLASSMENU_PREVCLASS_IMGID,
	DND_CLASSMENU_NEXTCLASS_IMGID,
	DND_CLASSMENU_BOTTOMTEXTID,
	DND_CLASSMENU_CLASSPERK5ID,
	DND_CLASSMENU_CLASSPERK25ID,
	DND_CLASSMENU_CLASSPERK50ID,
	DND_CLASSMENU_CLASSCURSOR_LEFTID,
	DND_CLASSMENU_CLASSCURSOR_RIGHTID,
	DND_CLASSMENU_BACKGROUND = 300
};

#define DND_CLASSMENU_IDCOUNT (DND_CLASSMENU_CLASSCURSOR_RIGHTID + 1)
#define DND_MAX_CLASSPERKS (DND_CLASSMENU_CLASSPERK50ID - DND_CLASSMENU_CLASSPERK5ID + 1)

enum {
	DND_CLASS_LABEL_ACTOR,
	DND_CLASS_LABEL_NAME,
	DND_CLASS_LABEL_LEFTIMG,
	DND_CLASS_LABEL_RIGHTIMG,
	DND_CLASS_LABEL_MUGSHOT,
	DND_CLASS_LABEL_TEXT,
	DND_CLASS_LABEL_PERK1,
	DND_CLASS_LABEL_PERK2,
	DND_CLASS_LABEL_PERK3
};

str GetClassLabel(str class_prefix, int label) {
	switch(label) {
		case DND_CLASS_LABEL_ACTOR:
		return StrParam(s:class_prefix, s:"_ACTOR");
		
		case DND_CLASS_LABEL_NAME:
		return StrParam(s:class_prefix, s:"_NAME");
		
		case DND_CLASS_LABEL_LEFTIMG:
		return StrParam(s:class_prefix, s:"_LEFTIMG");
		
		case DND_CLASS_LABEL_RIGHTIMG:
		return StrParam(s:class_prefix, s:"_RIGHTIMG");
		
		case DND_CLASS_LABEL_MUGSHOT:
		return StrParam(s:class_prefix, s:"_MUGSHOT");
		
		case DND_CLASS_LABEL_TEXT:
		return StrParam(s:class_prefix, s:"_TEXT");
		
		case DND_CLASS_LABEL_PERK1:
		return StrParam(s:class_prefix, s:"_PERK1");
		
		case DND_CLASS_LABEL_PERK2:
		return StrParam(s:class_prefix, s:"_PERK2");
		
		case DND_CLASS_LABEL_PERK3:
		return StrParam(s:class_prefix, s:"_PERK3");
	}
	return "";
}

str GetClassName(int id) {
	return StrParam(s:"CLASS", d:id, s:"_NAME");
}

Script "DnD Class Select" (int isSinglePlayer) NET CLIENTSIDE {
	// these aren't needed for net clientside
	//if(PlayerNumber() != ConsolePlayerNumber())
	//	Terminate;

	// close the previous menu
	ConsoleCommand("closemenu");

	int cpnum = ConsolePlayerNumber();
	
	//Log(s:"cpnum ", d:ConsolePlayerNumber(), s: " vs pnum ", d:PlayerNumber());
	
	// don't allow duplicates to be run
	if(GetUserCVar(cpnum, "dnd_onclassmenu"))
		Terminate;
	
	int ctype = GetUserCVar(cpnum, "dnd_playerclass");
	
	SetUserCVar(cpnum, "dnd_classmenu_button", 0);
	SetUserCVar(cpnum, "dnd_onclassmenu", true);
	
	ACS_NamedExecuteAlways("DnD Class Select Screen", 0);
	ACS_NamedExecuteAlways("DnD Class Select Info", 0, ctype);
	ACS_NamedExecuteAlways("DnD Class Select Animated", 0);
	
	int bpress, obpress, dir = 0;
	while(PlayerIsSpectator(cpnum) || isSinglePlayer) {
		bpress = GetPlayerInput(-1, INPUT_BUTTONS);
		obpress = GetPlayerInput(-1, INPUT_OLDBUTTONS);

		// left move
		if(isButtonPressed(bpress, obpress, GetMovementButton(MVMT_BT_LEFT))) {
			dir = -1;
			ACS_NamedExecuteAlways("DnD Class Select Arrow Press", 0, DND_CLASSMENU_LEFTBUTTON_BIT);
		}
		if(isButtonPressed(bpress, obpress, GetMovementButton(MVMT_BT_RIGHT))) {
			dir = 1;
			ACS_NamedExecuteAlways("DnD Class Select Arrow Press", 0, DND_CLASSMENU_RIGHTBUTTON_BIT);
		}
		if(isButtonPressed(bpress, obpress, BT_USE | BT_ATTACK)) {
			// terminate this, we made our choice
			LocalAmbientSound("RPG/MenuChoose", 127);
			SetUserCVar(cpnum, "dnd_playerclass", ctype);
			//ConsoleCommand("menu_joinclassidx 0");
			//ConsoleCommand("menu_joingamewithclass");
			if(!isSinglePlayer)
				ConsoleCommand("join");
			break;
		}
		if(isButtonPressed(bpress, obpress, BT_ALTATTACK)) {
			// clean this, we can exit
			LocalAmbientSound("RPG/MenuClose", 127);
			break;
		}
		
		if(dir != 0) {
			LocalAmbientSound("RPG/MenuMove", 127);
			ctype += dir;
			
			if(ctype < 0)
				ctype = MAXPLAYERCLASSES - 1;
			else if(ctype > MAXPLAYERCLASSES - 1)
				ctype = 0;
			
			// update
			ACS_NamedExecuteAlways("DnD Class Select Info", 0, ctype);
		}
		
		// reset
		dir = 0;
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
	
	if(notSet)
		SetActorState(tid, "Spawn");
		
	// sync this to clients
	ACS_NamedExecuteWithResult("DnD Sync Set Class", tid, class_id, notSet);
}

Script "DnD Sync Set Class" (int tid, int class_id, int notSet) CLIENTSIDE {
	SetActorInventory(tid, "DnD_Character", class_id + 1);
}

Script "DnD Class Select Arrow Press" (int mode) CLIENTSIDE {
	int cpnum = ConsolePlayerNumber();
	int bn = GetUserCVar(cpnum, "dnd_classmenu_button");
	if(bn & mode)
		Terminate;
	bn |= mode;
	SetUserCVar(cpnum, "dnd_classmenu_button", bn);
}

Script "DnD Class Select Screen" (void) CLIENTSIDE {
	SetHUDSize(600, 450, 1);
	SetFont("BIGFONT");
	HudMessage(s:"\cj", l:"CLASS_SELECT"; HUDMSG_PLAIN, DND_CLASSMENU_SELECTID, -1, 300.0, 80.0, 0.0);
		
	SetHUDSize(320, 240, 1);
	setFont("DNDCHRSL");
	HudMessage(s:"A"; HUDMSG_PLAIN, DND_CLASSMENU_BACKGROUND, -1, 160.0, 120.0, 0.0);
}

Script "DnD Class Select Animated" (void) CLIENTSIDE {
	int cpnum = ConsolePlayerNumber();
	bool larr_inc = false, rarr_inc = false;
	int larr_timer = 0, rarr_timer = 0;
	int select_alpha = 0;
	
	while(GetUserCVar(cpnum, "dnd_onclassmenu")) {
		// handle button press detection
		if(GetUserCVar(cpnum, "dnd_classmenu_button") & DND_CLASSMENU_LEFTBUTTON_BIT) {
			larr_timer = 0;
			larr_inc = true;
			SetUserCVar(cpnum, "dnd_classmenu_button", GetUserCVar(cpnum, "dnd_classmenu_button") ^ DND_CLASSMENU_LEFTBUTTON_BIT);
		}
		if(GetUserCVar(cpnum, "dnd_classmenu_button") & DND_CLASSMENU_RIGHTBUTTON_BIT) {
			rarr_timer = 0;
			rarr_inc = true;
			SetUserCVar(cpnum, "dnd_classmenu_button", GetUserCVar(cpnum, "dnd_classmenu_button") ^ DND_CLASSMENU_RIGHTBUTTON_BIT);
		}
	
		SetHudSize(320, 240, 1);
		
		// display arrows
		if(larr_inc)
			SetFont("CHSLARR2");
		else
			SetFont("CHSLARR1");
		HudMessage(s:"A"; HUDMSG_PLAIN, DND_CLASSMENU_CLASSCURSOR_LEFTID, -1, 34.0, 66.0, 0.0);
		
		if(rarr_inc)
			SetFont("CHSRARR2");
		else
			SetFont("CHSRARR1");
		HudMessage(s:"A"; HUDMSG_PLAIN, DND_CLASSMENU_CLASSCURSOR_RIGHTID, -1, 284.0, 66.0, 0.0);
		
		SetHudSize(600, 450, 1);
		SetFont("SMALLFONT");
		
		// display press w.e keys text
		HudMessage(
			s:"\c[L7]", l:"CLASS_PRESS", s:" \ci", k:"+use", s:" \c[L7]", l:"DND_OR", s:" \ci", k:"+attack", s: " \c[L7]", l:"CLASS_TOSELECT", s:"!",
			s:"\n\c[L7]", l:"CLASS_PRESS", s:" \ci", k:"+altattack", s:" \c[L7]", l:"CLASS_TOCLOSE", s:"!";
			HUDMSG_PLAIN | HUDMSG_ALPHA, DND_CLASSMENU_BOTTOMTEXTID, -1, 300.4, 368.1, 0.0, abs(sin(select_alpha * 1.0 / 360))
		);
		
		Delay(const:1);
		
		// alpha increment
		select_alpha = (select_alpha + 4) % 360;

		// button timers
		if(larr_inc) {
			larr_timer++;
			if(larr_timer >= DND_CLASSMENU_BUTTON_DELAY) {
				larr_timer = 0;
				larr_inc = false;
			}
		}
		
		if(rarr_inc) {
			rarr_timer++;
			if(rarr_timer >= DND_CLASSMENU_BUTTON_DELAY) {
				rarr_timer = 0;
				rarr_inc = false;
			}
		}
	}
}

Script "DnD Class Select Info" (int ctype) CLIENTSIDE {
	str cprefix = StrParam(s:"CLASS", d:ctype);

	SetHUDSize(600, 450, 1);
	SetFont("SMALLFONT");
	SetHudClipRect(100, 84, 384, 384, 336);
	
	HudMessage(s:"\c[J7]", l:GetClassLabel(cprefix, DND_CLASS_LABEL_NAME); HUDMSG_PLAIN, DND_CLASSMENU_CLASSID, -1, 300.4, 96.1, 0.0);

	str toShow = StrParam(l:GetClassLabel(cprefix, DND_CLASS_LABEL_TEXT));
	HudMessage(s:"\cj", s:toShow; HUDMSG_PLAIN, DND_CLASSMENU_CLASSEXPID, -1, 128.1, 184.1, 0.0);
	int len = GetRawLength(toShow) & 0xFFFF;
	
	// perks
	toShow = StrParam(s:"+ L5: ", l:GetClassLabel(cprefix, DND_CLASS_LABEL_PERK1));
	HudMessage(s:toShow; HUDMSG_PLAIN, DND_CLASSMENU_CLASSPERK5ID, CR_GREEN, 128.1, 192.1 + 8.0 * (len / 35), 0.0);
	len += GetRawLength(toShow) & 0xFFFF;

	toShow = StrParam(s:"+ L25: ", l:GetClassLabel(cprefix, DND_CLASS_LABEL_PERK2));
	HudMessage(s:toShow; HUDMSG_PLAIN, DND_CLASSMENU_CLASSPERK25ID, CR_ORANGE, 128.1, 208.1 + 8.0 * (len / 36), 0.0);
	len += GetRawLength(toShow) & 0xFFFF;

	HudMessage(s:"+ L50: ", l:GetClassLabel(cprefix, DND_CLASS_LABEL_PERK3); HUDMSG_PLAIN, DND_CLASSMENU_CLASSPERK50ID, CR_RED, 128.1, 224.1 + 8.0 * (len / 36), 0.0);
	
	SetHudClipRect(0, 0, 0, 0, 0);
	
	// show the class images -- offsets aligned
	SetFont(StrParam(l:GetClassLabel(cprefix, DND_CLASS_LABEL_MUGSHOT)));
	HudMessage(s:"A"; HUDMSG_PLAIN, DND_CLASSMENU_MUGIMG, -1, 281.4 + GetIntegerBits(GetMugshotXOffset(ctype)), 112.1, 0.0);
	SetFont(StrParam(l:GetClassLabel(cprefix, DND_CLASS_LABEL_LEFTIMG)));
	HudMessage(s:"A"; HUDMSG_PLAIN, DND_CLASSMENU_LEFTIMG, -1, 80.4, 208.1, 0.0);
	SetFont(StrParam(l:GetClassLabel(cprefix, DND_CLASS_LABEL_RIGHTIMG)));
	HudMessage(s:"A"; HUDMSG_PLAIN, DND_CLASSMENU_RIGHTIMG, -1, 512.4, 208.1, 0.0);
	
	// prev and next class images
	cprefix = StrParam(s:"CLASS", d:(ctype - 1 < 0) ? MAXPLAYERCLASSES - 1 : ctype - 1);
	SetFont(StrParam(l:GetClassLabel(cprefix, DND_CLASS_LABEL_LEFTIMG)));
	HudMessage(s:"A"; HUDMSG_PLAIN, DND_CLASSMENU_PREVCLASS_IMGID, -1, 112.4, 96.1, 0.0);
	
	cprefix = StrParam(s:"CLASS", d:(ctype + 1 > MAXPLAYERCLASSES - 1) ? 0 : ctype + 1);
	SetFont(StrParam(l:GetClassLabel(cprefix, DND_CLASS_LABEL_RIGHTIMG)));
	HudMessage(s:"A"; HUDMSG_PLAIN, DND_CLASSMENU_NEXTCLASS_IMGID, -1, 480.4, 96.1, 0.0);
}

Script "DnD Class Select Cleanup" (void) CLIENTSIDE {
	HudMessage(s:""; HUDMSG_PLAIN, DND_CLASSMENU_BACKGROUND, -1, 0, 0, 1.0);
	for(int i = 0; i < DND_CLASSMENU_IDCOUNT; ++i)
		HudMessage(s:""; HUDMSG_PLAIN, DND_CLASSMENU_SELECTID + i, -1, 0, 0, 1.0);
}

Script "DnD Character Select Cleanup" (void) CLIENTSIDE {
	for(int i = 0; i < DND_MAX_CHARACTERSEL_ITEMS; ++i)
		HudMessage(s:""; HUDMSG_PLAIN, RPGMENUCHARSELID - i, -1, 0, 0, 1.0);
}

Script "DnD Character Select Screen" (void) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;
	
	SetHUDSize(600, 450, 1);
	SetFont("BIGFONT");
	HudMessage(s:"\cj", l:"CLASS_LOAD"; HUDMSG_PLAIN, RPGMENUCHARSELID - 1, -1, 300.0, 80.0, 0.0);
		
	SetHUDSize(320, 240, 1);
	setFont("DNDCHRLD");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUCHARSELID, -1, 160.4, 120.0, 0.0);
}

void RunClassLoadScripts() {
	ACS_NamedExecuteAlways("DnD Character Select Screen", 0);
	ACS_NamedExecuteAlways("DnD Character Select Animated", 0);
	ACS_NamedExecuteAlways("DnD Character Load Inputs", 0);
}

Script "DnD Character Select Animated" (void) CLIENTSIDE {
	int cpnum = ConsolePlayerNumber();
	int pnum = PlayerNumber();
	if(cpnum != pnum)
		Terminate;

	int select_alpha = 0;
	int lim = TICRATE * Clamp_Between(GetCVar("dnd_loadtime"), DND_HARDCORE_LOADTIME, 100);
	int counter = 0;
	
	while(counter < lim && PlayerInGame(pnum) && CheckInventory("CanLoad")) {
		SetHudSize(600, 450, 1);
		SetFont("SMALLFONT");
		
		// display press w.e keys text
		HudMessage(
			s:"\c[L7]", l:"CLASS_PRESS", s:" \ci", k:"+use", s:" \c[L7]", l:"DND_OR", s:" \ci", k:"+attack", s: " \c[L7]", l:"CLASS_TOSELECT", s:"!",
			s:"\n\c[L7]", l:"CLASS_PRESS", s:" \ci", k:"+altattack", s:" \c[L7]", l:"CLASS_TOCLOSE", s:"!";
			HUDMSG_PLAIN | HUDMSG_ALPHA, RPGMENUCHARSELID - 4, -1, 300.4, 368.1, 0.0, abs(sin(select_alpha * 1.0 / 360))
		);

		SetFont("BIGFONT");
		SetHudSize(960, 720, 0);
		SetHudClipRect(120, 500, 640, 120, 640);
		if (CheckInventory("PlayerIsLoggedIn"))
			HudMessage(l:"DND_TEXT_YOUHAVE", s:" \cd", d:(lim - counter) / TICRATE, s:"\c- ", l:"DND_TEXT_LOADCHARWARNING"; HUDMSG_PLAIN, RPGMENUCHARSELID - 5, CR_RED, 480.4, 580.2, 0);
		else
			HudMessage(l:"DND_TEXT_YOUHAVE", s:" \cd", d:(lim - counter) / TICRATE, s:"\c- ", l:"DND_TEXT_LOADLOGINWARNING"; HUDMSG_PLAIN, RPGMENUCHARSELID - 5, CR_RED, 480.4, 580.2, 0);
		SetHudClipRect(0, 0, 0, 0, 0);
		Delay(const:1);
		++counter;
		
		// alpha increment
		select_alpha = (select_alpha + 4) % 360;

		// button timers
		SetHudSize(600, 450, 1);
		SetFont("TRADBTN");
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUCHARSELID - 6, CR_WHITE, 300.2, 300.1, 0);
	}
}

Script "DnD Character Load Inputs" (void) CLIENTSIDE {
	int cpnum = ConsolePlayerNumber();
	int pnum = PlayerNumber();
	if(cpnum != pnum)
		Terminate;

	PlayerCursorData.posx = HUDMAX_XF / 2;
	PlayerCursorData.posy = HUDMAX_YF / 2;

	int boxid = MAINBOX_NONE, boxid_prev = MAINBOX_NONE;
	int i, j, k;
	bool sendInput = false;
	int class_id;

	bool draw_character_info = false;

	menu_pane_T& CurrentPane = GetPane();
	ResetPane(CurrentPane);
	AddBoxToPane_Points(CurrentPane, 1, 1, 1, 1);

	while(PlayerInGame(pnum) && CheckInventory("PlayersNotReady")) {
		PlayerCursorData.posx = GetCursorPos(GetPlayerInput(cpnum, INPUT_YAW), MOUSE_INPUT_X);
		PlayerCursorData.posy = GetCursorPos(GetPlayerInput(cpnum, INPUT_PITCH), MOUSE_INPUT_Y);

		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
		DrawCursor();

		// button id recognition
		boxid = GetTriggeredBoxOnPane(CurrentPane, PlayerCursorData.posx, PlayerCursorData.posy,  404.0);
		if(boxid != boxid_prev && boxid != MAINBOX_NONE)
			LocalAmbientSound("RPG/MenuMove", 127);
			
		boxid_prev = boxid;
		SetInventory("ActivePopupBox", boxid);

		Delay(const:1);
		
		if(!draw_character_info && CheckInventory("PlayerIsLoggedIn")) {
			j = 0;
			for(i = 0; i < DND_MAX_CHARS; ++i) {
				class_id = LoadedPlayerData[i].classid;
				if(class_id) {
					if(!j)
						DrawCharacterLoadInfo(i);
					draw_character_info = true;
					AddBoxToPane_Points(CurrentPane, 404.0, 161.0 - 11.0 * j, 240.0, 155.0 - 11.0 * j);
					++j;
				}
			}
		}

		if(draw_character_info)
			DrawAllCharactersLoadInfo(boxid);
	}

	SetInventory("ActivePopupBox", 0);

	ACS_NamedExecuteAlways("DnD Character Select Cleanup", 0);
}

Script "DnD Character Data Display Store" (int pnum) {
	// fill in the display array since player logged in and sync it with client
	str pacc = GetPlayerAccountName(pnum);
	for(int i = 0; i < DND_MAX_CHARS; ++i) {
		int class_id = GetDBEntry(GetCharField(DND_DB_CLASSID, i), pacc);
		// player has a character on this char id with the classid = class_id, so store it for display now
		if(class_id) {
			int level = GetDBEntry(GetCharField(DND_DB_LEVEL, i), pacc);
			int exp = GetDBEntry(GetCharField(DND_DB_EXP, i), pacc);
			int hp = GetDBEntry(GetCharField(DND_DB_HEALTH, i), pacc);
			int credit = GetDBEntry(GetCharField(DND_DB_CREDIT, i), pacc);
			int budget = GetDBEntry(GetCharField(DND_DB_BUDGET, i), pacc);

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

	LoadedPlayerData[char_id].budget = budget;

	SetResultValue(0);
}

void DrawCharacterLoadInfo(int char_id) {
	int ctype = LoadedPlayerData[char_id].classid - 1;

	str cprefix = StrParam(s:"CLASS", d:ctype);

	SetHUDSize(600, 450, 1);
	SetFont("SMALLFONT");
	
	int id = 7;

	HudMessage(
		s:"\c[J7]", l:"DND_STAT18", s: " \cd", d:LoadedPlayerData[char_id].level, s:"\c[J7] ", l:GetClassLabel(cprefix, DND_CLASS_LABEL_NAME); 
		HUDMSG_PLAIN, RPGMENUCHARSELID - id, -1, 300.4, 96.1, 0.0
	);
	++id;

	HudMessage(s:"\c[J7]", l:"DND_STAT16", s:": \cd", d:LoadedPlayerData[char_id].exp, s:"\c[J7] / \cd", d:GetExpLimit_Level(LoadedPlayerData[char_id].level); HUDMSG_PLAIN, RPGMENUCHARSELID - id, -1, 224.1, 144.1, 0.0);
	++id;

	HudMessage(s:"\c[J7]", l:"DND_HEALTH", s:": \cj", d:LoadedPlayerData[char_id].hp; HUDMSG_PLAIN, RPGMENUCHARSELID - id, -1, 224.1, 160.1, 0.0);
	++id;
	
	HudMessage(s:"\c[J7]", l:"DND_STAT19", s:": \cj", d:LoadedPlayerData[char_id].credit; HUDMSG_PLAIN, RPGMENUCHARSELID - id, -1, 224.1, 176.1, 0.0);
	++id;
	
	HudMessage(s:"\c[J7]", l:"DND_MENU_BUDGET", s:": \cj", d:LoadedPlayerData[char_id].budget; HUDMSG_PLAIN, RPGMENUCHARSELID - id, -1, 224.1, 192.1, 0.0);
	++id;

	// show the class images -- offsets aligned
	SetFont(StrParam(l:GetClassLabel(cprefix, DND_CLASS_LABEL_MUGSHOT)));
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUCHARSELID - id, -1, 281.4 + GetIntegerBits(GetMugshotXOffset(ctype)), 112.1, 0.0);
}

void DrawAllCharactersLoadInfo(int boxid) {
	SetHUDSize(600, 450, 1);
	SetFont("SMALLFONT");
	int count = 0;
	int id = 13;
	str col = "\c[J7]";
	str sel_text = "";
	for(int i = 0; i < DND_MAX_CHARS; ++i) {
		if(LoadedPlayerData[i].classid) {
			// - 2 because -1 is the 1st button box
			if(boxid - 2 == count) {
				col = "\c[B1]";
				sel_text = " \cf<==";
			}
			else {
				col = "\c[J7]";
				sel_text = "";
			}

			str cprefix = StrParam(s:"CLASS", d:LoadedPlayerData[i].classid - 1);
			HudMessage(
				s:col, l:"DND_MENU_SLOT", s:" ", d:i, s:". \cj", l:GetClassLabel(cprefix, DND_CLASS_LABEL_NAME), s: " ", s:col, s:"- Level: \cd", d:LoadedPlayerData[i].level, s:sel_text; 
				HUDMSG_PLAIN, RPGMENUCHARSELID - id, -1, 96.1, 224.1 + 16.0 * count, 0.0
			);
			++id;
			++count;
		}
	}
}

#endif
