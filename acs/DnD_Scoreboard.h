#ifndef DND_SCOREBOARD_IN
#define DND_SCOREBOARD_IN

enum {
	DND_SCBRD_PLAYEREXITED1,
	DND_SCBRD_PLAYEREXITED2,
	DND_SCBRD_PLAYEREXITCOUNT,
	DND_SCBRD_TIMER,
	DND_SCBRD_ALIVEPLAYERCOUNT,
	DND_SCBRD_SPECPLAYERCOUNT,
	DND_SCBRD_DEADPLAYERCOUNT,
};
#define MAX_SCOREBOARD_DATA (DND_SCBRD_DEADPLAYERCOUNT + 1)

#define MAX_INTERMISSION_SONGS 4
#define MAX_INTERMISSION_PLAYERS_SHOWN 29 // we can fit 29 players currently
#define MAX_INTERMISSION_PNAMELEN 25
#define INTERMISSION_CHAR_SCALE 2.1 // this is a scale found by experimenting with the current resolution
#define INTERMISSION_SCROLLFACTOR_INT 4
#define INTERMISSION_SCROLLFACTOR 4.0
#define INTERMISSION_SCROLLSCALE (0.4166 * INTERMISSION_SCROLLFACTOR_INT) // 320 / 768 = 0.4166
#define INTERMISSION_BOX_YCHANGE 7.5
#define INTERMISSION_INITIAL_BUTTONS 3

#define SCOREBOARD_WRAPX 272.0
#define SCOREBOARD_WRAPY 152.0
#define SCOREBOARD_HOLDTIME 0.1

int ScoreboardData[MAX_SCOREBOARD_DATA];

#define MAXINTERMISSIONRECTS (MAXPLAYERS + INTERMISSION_INITIAL_BUTTONS)
typedef struct mt {
	rect_T MenuRectangles[MAXINTERMISSIONRECTS];
	int cursize;
} menu_inter_T;

global menu_inter_T 19: IntermissionPane;

// this order is followed during display
enum {
	INTER_PSTATE_ALIVE,
	INTER_PSTATE_SPECTATING,
	INTER_PSTATE_DEAD
};
global int 20: PInterGroups[][MAXPLAYERS];

void ResetIntermissionPane() {
	IntermissionPane.cursize = 0;
	
	// re-add the fixed boxes
	AddBoxToInterPane_Points(0, 0, 0, 0);
	AddBoxToInterPane_Points(0, 0, 0, 0);
	AddBoxToInterPane_Points(0, 0, 0, 0);
}

void AddBoxToInterPane_Points(int tx, int ty, int bx, int by) {
	if(IntermissionPane.cursize < MAXINTERMISSIONRECTS) {
		int s = IntermissionPane.cursize;
		IntermissionPane.MenuRectangles[s].topleft_x = tx;
		IntermissionPane.MenuRectangles[s].topleft_y = ty;
		IntermissionPane.MenuRectangles[s].botright_x = bx;
		IntermissionPane.MenuRectangles[s].botright_y = by;
		//Log(s:"add box ", f:tx, s: " ", f:ty, s: " ", f:bx, s: " ", f:by);
		++IntermissionPane.cursize;
	}
	else
		Log(s:"Menu box limit exceeded.");
}

int GetTriggeredBoxOnIntermissionPane(int mx, int my, int yoff = 0) {
	// temporary soln, I'll put some button or some shit there maybe
	if(my < 90.0)
		return MAINBOX_NONE;
	for(int i = 0; i < IntermissionPane.cursize; ++i) {
		if
		(
			point_in_points(
				IntermissionPane.MenuRectangles[i].topleft_x,
				IntermissionPane.MenuRectangles[i].topleft_y,
				IntermissionPane.MenuRectangles[i].botright_x,
				IntermissionPane.MenuRectangles[i].botright_y,
				mx, my, yoff
			)
		)
			return i + 1;
	}
	return MAINBOX_NONE;
}

bool PlayerTriggeredExit(int pnum) {
	if(pnum > 31)
		return IsSet(ScoreboardData[DND_SCBRD_PLAYEREXITED2], pnum - 32);
	return IsSet(ScoreboardData[DND_SCBRD_PLAYEREXITED1], pnum);
}

void MarkPlayerAsExited(int pnum) {
	if(pnum > 31)
		ScoreboardData[DND_SCBRD_PLAYEREXITED2] = SetBit(ScoreboardData[DND_SCBRD_PLAYEREXITED2], pnum - 32);
	else
		ScoreboardData[DND_SCBRD_PLAYEREXITED1] = SetBit(ScoreboardData[DND_SCBRD_PLAYEREXITED1], pnum);
	++ScoreboardData[DND_SCBRD_PLAYEREXITCOUNT];
}

void UnmarkPlayerAsExited(int pnum) {
	if(pnum > 31)
		ScoreboardData[DND_SCBRD_PLAYEREXITED2] = ClearBit(ScoreboardData[DND_SCBRD_PLAYEREXITED2], pnum - 32);
	else
		ScoreboardData[DND_SCBRD_PLAYEREXITED1] = ClearBit(ScoreboardData[DND_SCBRD_PLAYEREXITED1], pnum);
	--ScoreboardData[DND_SCBRD_PLAYEREXITCOUNT];
}

bool PlayerHasClicked(int pnum) {
	if(pnum > 31)
		return IsSet(ScoreboardData[DND_SCBRD_PLAYEREXITED2], pnum - 32);
	return IsSet(ScoreboardData[DND_SCBRD_PLAYEREXITED1], pnum);
}

// serverside
Script 1690 (int isSecretExit, int forcedExit) {
	// this player hasn't triggered the exit before
	int this = ActivatorTID();
	int pnum = this - P_TIDSTART;
	// timer didnt begin yet
	if(!PlayerTriggeredExit(pnum) && !ScoreboardData[DND_SCBRD_TIMER]) {
		bool proceed = false;
		bool democracy_result = false;
		bool monpct_result = false;
		int democracy = GetCVar("dnd_exit_democratic");
		int monpct = GetCVar("dnd_exit_monsterpercent");
		int active_pcount = GetActivePlayerCount();
		
		if(!forcedExit) {
			if(democracy) {
				// mark this player as already exited
				MarkPlayerAsExited(pnum);
				
				// first 10 is used as a simple divisor, anything above is treated as an actual percentage
				// so this allows things like "half the players" or "one third players" exited thing much simpler
				if(democracy >= 1 && democracy <= 10) {
					if(ScoreboardData[DND_SCBRD_PLAYEREXITCOUNT] >= active_pcount / democracy)
						democracy_result = true;
					else {
						// error message
						ShowPopup(POPUP_PLAYERCONDNOTMET, 0, 0);
					}
				}
				else if(democracy > 10) {
					if(ScoreboardData[DND_SCBRD_PLAYEREXITCOUNT] >= active_pcount * democracy / 100)
						democracy_result = true;
					else {
						// error message
						ShowPopup(POPUP_PLAYERCONDNOTMET, 0, 0);
					}
				}
			}
			
			if(monpct) {
				if(GetLevelInfo(LEVELINFO_KILLED_MONSTERS) >= GetLevelInfo(LEVELINFO_TOTAL_MONSTERS) * monpct / 100)
					monpct_result = true;
				else {
					// error message
					ShowPopup(POPUP_MONKILLCONDNOTMET, 0, 0);
				}
			}
			
			proceed = (!democracy || democracy_result) && (!monpct || monpct_result);
		}
		
		// forcedExit is for romero head
		if(proceed || forcedExit) {
			// start the timer in the above function too, then after it expires / all players are ready we can exit
			ScoreboardData[DND_SCBRD_TIMER] = Clamp_Between(GetCVar("dnd_exit_timer"), 5, 30);
			
			// freeze monsters / players during this time
			GiveInventory("MenuFreeze", 1);
			GiveInventory("DnD_IntermissionState", 1);
			SetPlayerProperty(1, 1, PROP_TOTALLYFROZEN);
			SetPlayerProperty(1, 2, PROP_INVULNERABILITY);
			
			// make all players run this input loop
			for(int i = 0; i < MAXPLAYERS; ++i) {
				if(PlayerInGame(i)) {
					SetActivator(i + P_TIDSTART);
					ACS_NamedExecuteWithResult("DnD Scoreboard Input Loop", i);
				}
			}
			
			// set activator to world --- CS draws etc. are all broadcasted from here on to all players, but server checks for making the decision to
			// change the map
			SetActivator(-1);
			
			// display
			// get the same music id for everyone and make sure everyone in the game runs this script
			democracy = GetLevelInfo(LEVELINFO_TOTAL_MONSTERS);
			
			ACS_NamedExecuteWithResult("DnD Scoreboard Display", ScoreboardData[DND_SCBRD_TIMER], democracy);
			
			// clean up exit markers, we'll reuse them for click detection
			ScoreboardData[DND_SCBRD_PLAYEREXITED1] = 0;
			ScoreboardData[DND_SCBRD_PLAYEREXITED2] = 0;
			ScoreboardData[DND_SCBRD_PLAYEREXITCOUNT] = 0;
			
			// timer -- waits until times up or everyones clicked
			democracy = ScoreboardData[DND_SCBRD_TIMER] * TICRATE;
			while(democracy-- && ScoreboardData[DND_SCBRD_PLAYEREXITCOUNT] < GetActivePlayerCount())
				Delay(const:1);
			
			if(!isSecretExit)
				Exit_Normal(0);
			else
				Exit_Secret(0);
			Delay(1);
		}
	}
}

Script "DnD Scoreboard Input Loop" (int pnum) {
	// this is fine because after map terminates so does this script
	while(true) {
		if(!PlayerHasClicked(pnum) && (GetPlayerInput(pnum, INPUT_BUTTONS) & (BT_ATTACK | BT_USE | BT_ALTATTACK))) {
			MarkPlayerAsExited(pnum);
			GiveInventory("DnD_ScoreboardTick", 1);
			
			// give every player the refresh token
			for(int i = 0; i < MAXPLAYERS; ++i)
				if(PlayerInGame(i))
					GiveActorInventory(i + P_TIDSTART, "DnD_ScoreboardRefresher", 1);
		}
		Delay(const:1);
	}
}

enum {
	DND_SCBRDID_HOVERBG_WHO = 1800,
	DND_SCBRDID_HOVERBG_CLASS,
	DND_SCBRDID_HOVER_LIFE,
	DND_SCBRDID_HOVER_ARMOR,
	DND_SCBRDID_HOVER_EXP,
	DND_SCBRDID_HOVER_CREDIT,
	DND_SCBRDID_HOVERBG,
	DND_SCBRDID_MAPCOMPLETE,
	DND_SCBRDID_STATUSICON,
	DND_SCBRDID_TICKICON,
	DND_SCBRDID_CONTINUE,
	DND_SCBRDID_PLAYER,
	DND_SCBRDID_PLAYERUNDERLINE,
	DND_SCBRDID_KILLS,
	DND_SCBRDID_KILLSUNDERLINE,
	DND_SCBRDID_DAMAGEDEALT,
	DND_SCBRDID_DAMAGEDEALTUNDERLINE,
	DND_SCBRDID_DAMAGETAKEN,
	DND_SCBRDID_DAMAGETAKENUNDERLINE,
	DND_SCBRDID_PDATA,
	
	DND_SCBRDID_BG = 2800
};
#define DND_SCBRD_PDATA_THINGS 6 // status, name, damage dealt, damage taken, kill pct, tick icon

// ghetto af solution
Script "DnD Scoreboard Sound Player" (int sound) CLIENTSIDE {
	if(!sound)
		PlaySound(0, StrParam(s:"Intermission", d:random(1, MAX_INTERMISSION_SONGS)), CHAN_AUTO | CHAN_NOPAUSE, 1.0, true, ATTN_NONE);
	else
		PlaySound(0, "RPG/MenuMove", CHAN_AUTO | CHAN_NOPAUSE, 1.0, false, ATTN_NONE);
	SetResultValue(0);
}

// group players into categories
void GroupScoreboardPlayers() {
	ScoreboardData[DND_SCBRD_ALIVEPLAYERCOUNT] = 0;
	ScoreboardData[DND_SCBRD_DEADPLAYERCOUNT] = 0;
	ScoreboardData[DND_SCBRD_SPECPLAYERCOUNT] = 0;
	
	int i;
	for(i = 0; i < MAXPLAYERS; ++i) {
		// reset to -1
		PInterGroups[INTER_PSTATE_ALIVE][i] = -1;
		PInterGroups[INTER_PSTATE_DEAD][i] = -1;
		PInterGroups[INTER_PSTATE_SPECTATING][i] = -1;
	}

	// grouping works on first come first serve in player id -- the index 0 may be player 13 for example on spectating players
	int lim = GetPlayerCountAny();
	int pcounter = 0;
	for(i = 0; i < MAXPLAYERS && pcounter < lim; ++i) {
		if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART)) {
			PInterGroups[INTER_PSTATE_ALIVE][ScoreboardData[DND_SCBRD_ALIVEPLAYERCOUNT]++] = i;
			++pcounter;
		}
		else {
			int spec = PlayerIsSpectator(i);
			if(spec == INTER_PSTATE_DEAD) {
				PInterGroups[INTER_PSTATE_DEAD][ScoreboardData[DND_SCBRD_DEADPLAYERCOUNT]++] = i;
				++pcounter;
			}
			else if(spec == INTER_PSTATE_SPECTATING) {
				PInterGroups[INTER_PSTATE_SPECTATING][ScoreboardData[DND_SCBRD_SPECPLAYERCOUNT]++] = i;
				++pcounter;
			}
		}
	}
}

void BuildScoreboardBoxes() {
	ResetIntermissionPane();
	
	// we will construct the boxes with priority order of:
	// Alive first, Dead second and spectating last
	int box_count = 0;
	int i;
	bool foundDead = false;
	bool foundAlive = false;
	int y_off = 0;
	for(i = 0; i < MAXPLAYERS && PInterGroups[INTER_PSTATE_ALIVE][i] != -1; ++i) {
		foundAlive = true;
		AddBoxToInterPane_Points(464.0, 306.0 - INTERMISSION_BOX_YCHANGE * box_count, 400.0, 300.0 - INTERMISSION_BOX_YCHANGE * box_count);
		++box_count;
	}
	y_off = foundAlive * 7.5;
	// dead
	for(i = 0; i < MAXPLAYERS && PInterGroups[INTER_PSTATE_DEAD][i] != -1; ++i) {
		foundDead = true;
		AddBoxToInterPane_Points(464.0, 306.0 - INTERMISSION_BOX_YCHANGE * box_count - y_off, 400.0, 300.0 - INTERMISSION_BOX_YCHANGE * box_count - y_off);
		++box_count;
	}
	y_off += foundDead * 7.5;
	// spec
	for(i = 0; i < MAXPLAYERS && PInterGroups[INTER_PSTATE_SPECTATING][i] != -1; ++i) {
		AddBoxToInterPane_Points(464.0, 306.0 - INTERMISSION_BOX_YCHANGE * box_count - y_off, 400.0, 300.0 - INTERMISSION_BOX_YCHANGE * box_count - y_off);
		++box_count;
	}
}

Script "DnD Scoreboard Display" (int time, int total_mons) CLIENTSIDE {
	// we dont want scan info overlaid
	ClearMonsterScanInfo();

	SpawnForced("ScoreBoardSongPlayer", 0, 0, 0);
	// hopefully to clear the chat
	Log(s:"\n\n\n\n\n\n\n\n\n\n");

	// bg
	SetFont("INTERPCW");
	SetHudSize(640, 480, 1);
	HudMessage(s:"A"; HUDMSG_PLAIN, DND_SCBRDID_BG, CR_UNTRANSLATED, 320.0, 240.0, 0);
	
	// player header
	SetFont("INTERFONT");
	HudMessage(
		l:"DND_PLAYER"; 
		HUDMSG_PLAIN, DND_SCBRDID_PLAYER, CR_UNTRANSLATED, 
		20.1, 0.1, 0
	);
	
	SetFont("UNDERLNS");
	HudMessage(
		s:"A"; 
		HUDMSG_PLAIN, DND_SCBRDID_PLAYERUNDERLINE, CR_UNTRANSLATED, 
		8.1, 16.1, 0
	);
	
	// damage dealt header
	SetFont("INTERFONT");
	HudMessage(
		l:"DND_DMGDEALT"; 
		HUDMSG_PLAIN, DND_SCBRDID_DAMAGEDEALT, CR_UNTRANSLATED, 
		224.1, 0.1, 0
	);
	
	SetFont("UNDERLIN");
	HudMessage(
		s:"A"; 
		HUDMSG_PLAIN, DND_SCBRDID_DAMAGEDEALTUNDERLINE, CR_UNTRANSLATED, 
		224.1, 16.1, 0
	);
	
	// damage taken header
	SetFont("INTERFONT");
	HudMessage(
		l:"DND_DMGTAKEN"; 
		HUDMSG_PLAIN, DND_SCBRDID_DAMAGETAKEN, CR_UNTRANSLATED, 
		392.1, 0.1, 0
	);
	
	SetFont("UNDERLIN");
	HudMessage(
		s:"A"; 
		HUDMSG_PLAIN, DND_SCBRDID_DAMAGETAKENUNDERLINE, CR_UNTRANSLATED, 
		392.1, 16.1, 0
	);
	
	// kills header
	SetFont("INTERFONT");
	HudMessage(
		l:"DND_KILLS"; 
		HUDMSG_PLAIN, DND_SCBRDID_KILLS, CR_UNTRANSLATED, 
		600.4, 0.1, 0
	);
	
	SetFont("UNDERLNS");
	HudMessage(
		s:"A"; 
		HUDMSG_PLAIN, DND_SCBRDID_KILLSUNDERLINE, CR_UNTRANSLATED, 
		600.4, 16.1, 0
	);
	
	// map complete
	// map name in center of bg image --- make these fall into place here
	SetFont("INTERFONT");
	HudMessage(
		s:"\cj", n:PRINTNAME_LEVEL, s:": \cf", n:PRINTNAME_LEVELNAME, s:"\n\n\c[M3]", l:"DND_COMPLETED";
		HUDMSG_PLAIN, DND_SCBRDID_MAPCOMPLETE, CR_GOLD,
		240.4, 360.1, 0
	);
	
	/* 
		foreground stuff
		
		STATUSICON - PLAYERNAME (LEVEL - CLASSNAME) ---------- SURVIVED / DEAD / SPECTATING
		
		if too many players exist, draw a scrollbar to the rightmost side of the screen to allow scrolling + allow scroll with prev/next wep
		or forward/backward input
	*/
	
	Delay(const:1);
	ACS_NamedExecuteWithResult("DnD Scoreboard Loop", time, total_mons);
	
	SetResultValue(0);
}

void DrawPlayerOnScoreboard(int interstate, int pnum, int draw_count, int total_mons, int p_highlight, int y_off) {
	int temp, pct, col = CR_RED, tid, y;
	bool triggered = false;
	str text;
	
	triggered = false;
	tid = pnum + P_TIDSTART;
	y = 36.1 + 18.0 * draw_count + INTERMISSION_SCROLLFACTOR * ScrollPos.x + y_off;
	col = CR_UNTRANSLATED;
	
	if(pnum == p_highlight) {
		col = CR_GOLD;
		triggered = true;
	}

	// icon
	if(interstate == INTER_PSTATE_DEAD)
		SetFont("DEADICO");
	else if(interstate == INTER_PSTATE_SPECTATING)
		SetFont("SPECICO");
	else
		SetFont("LIVEICO");
	HudMessage(
		s:"A"; 
		HUDMSG_PLAIN, DND_SCBRDID_PDATA + DND_SCBRD_PDATA_THINGS * draw_count, CR_UNTRANSLATED, 
		16.1, y, 0
	);
	
	if(interstate != INTER_PSTATE_SPECTATING && CheckActorInventory(pnum + P_TIDSTART, "DnD_ScoreboardTick")) {
		SetFont("TICKED");
		HudMessage(
			s:"A"; 
			HUDMSG_PLAIN, DND_SCBRDID_PDATA + DND_SCBRD_PDATA_THINGS * draw_count + 1, CR_UNTRANSLATED, 
			0.1, y, 0
		);
	}
	
	SetFont("INTERFONT");
	
	text = StrParam(n:pnum + 1);
	temp = GetPlayernameRawLength(text);
	// don't clutter the intermission board
	if(temp > MAX_INTERMISSION_PNAMELEN)
		text = StrParam(s:StrLeft(text, MAX_INTERMISSION_PNAMELEN), s:"...");
		temp = MAX_INTERMISSION_PNAMELEN + 3; // 3 for "..."
		HudMessage(
			s:text; 
			HUDMSG_PLAIN, DND_SCBRDID_PDATA + (DND_SCBRD_PDATA_THINGS * draw_count + 2), col, 
			35.1, y, 0
		);
	
	if(interstate == INTER_PSTATE_ALIVE) {
		// these stats use a special green color, so I check for this color availability and if so we recolor manually...
		// could prob be written better honestly
		str col_prefix = "";
		if(col != CR_GOLD)
			col_prefix = "\c[Q9]";
		
		// damage dealt
		HudMessage(
			s:col_prefix, s:GetStatistic(DND_STATISTIC_DAMAGEDEALT, tid); 
			HUDMSG_PLAIN, DND_SCBRDID_PDATA + (DND_SCBRD_PDATA_THINGS * draw_count + 3), col, 
			468.4, y, 0
		);
		
		// damage taken
		HudMessage(
			s:col_prefix, s:GetStatistic(DND_STATISTIC_DAMAGETAKEN, tid); 
			HUDMSG_PLAIN, DND_SCBRDID_PDATA + (DND_SCBRD_PDATA_THINGS * draw_count + 4), col, 
			736.4, y, 0
		);
		
		// kill display
		temp = CheckActorInventory(tid, "DnD_Kills");
		if(!total_mons)
			pct = 0;
		else
			pct = (temp * 100) / total_mons;
		
		if(col != CR_GOLD) {
			col = CR_RED;
			if(pct > 75)
				col = CR_GREEN;
			else if(pct > 50)
				col = CR_YELLOW;
			else if(pct > 25)
				col = CR_ORANGE;
		}
		
		HudMessage(
			d:temp, s: " (", d: pct, s:"%)"; 
			HUDMSG_PLAIN, DND_SCBRDID_PDATA + (DND_SCBRD_PDATA_THINGS * draw_count + 5), col, 
			964.4, y, 0
		);
	}
	else {
		// 5 to 3
		pct = DND_SCBRDID_PDATA + (DND_SCBRD_PDATA_THINGS * draw_count + 1);
		DeleteText(pct);
		DeleteTextRange(pct + 4, pct + 2);
	}
}

void DrawScoreboard(int time, int ScrollPos, int total_mons, int p_highlight) {		
	// setup the clipping rectangle
	
	// playerlist
	SetHudSize(1024, 768, 1);
	SetHudClipRect(0, 36, 1024, 520, 1024);
	
	// alive
	int draw_count = 0;
	int i;
	int y_off = 0;
	
	bool foundAlive = false;
	bool foundDead = false;
	
	// clear the potentially previously drawn things due to refresh
	DeleteTextRange(DND_SCBRDID_PDATA, DND_SCBRDID_PDATA + DND_SCBRD_PDATA_THINGS * (MAXPLAYERS + 1));
	
	for(i = 0; PInterGroups[INTER_PSTATE_ALIVE][i] != -1; ++i) {
		foundAlive = true;
		DrawPlayerOnScoreboard(INTER_PSTATE_ALIVE, PInterGroups[INTER_PSTATE_ALIVE][i], draw_count++, total_mons, p_highlight, 0);
	}
	
	// dead
	y_off = foundAlive * 18.0;
	for(i = 0; PInterGroups[INTER_PSTATE_DEAD][i] != -1; ++i) {
		foundDead = true;
		DrawPlayerOnScoreboard(INTER_PSTATE_DEAD, PInterGroups[INTER_PSTATE_DEAD][i], draw_count++, total_mons, p_highlight, y_off);
	}
		
	// spec
	y_off += foundDead * 18.0;
	for(i = 0; i < MAXPLAYERS && PInterGroups[INTER_PSTATE_SPECTATING][i] != -1; ++i)
		DrawPlayerOnScoreboard(INTER_PSTATE_SPECTATING, PInterGroups[INTER_PSTATE_SPECTATING][i], draw_count++, total_mons, p_highlight, y_off);
	
	SetHudClipRect(0, 0, 0, 0, 0);
}

bool HandleIntermissionInputs(int min_y) {
	bool res = ListenScroll(min_y, 0);
	
	return res;
}

int DrawHoveredPlayerData() {
	int boxid = PlayerCursorData.itemHovered - INTERMISSION_INITIAL_BUTTONS;
	if(boxid <= MAINBOX_NONE)
		return -1;
	--boxid;
	
	int pnum = -1;
	int spec_info = 0;
	bool inGame = false;
	// from the boxid, discover which player it is
	if(boxid >= ScoreboardData[DND_SCBRD_ALIVEPLAYERCOUNT]) {
		if(boxid >= ScoreboardData[DND_SCBRD_ALIVEPLAYERCOUNT] + ScoreboardData[DND_SCBRD_DEADPLAYERCOUNT]) {
			// spectating range
			pnum = PInterGroups[INTER_PSTATE_SPECTATING][boxid - ScoreboardData[DND_SCBRD_DEADPLAYERCOUNT] - ScoreboardData[DND_SCBRD_ALIVEPLAYERCOUNT]];
			spec_info = INTER_PSTATE_SPECTATING;
		}
		else {
			// within dead player range
			pnum = PInterGroups[DND_SCBRD_DEADPLAYERCOUNT][boxid - ScoreboardData[DND_SCBRD_ALIVEPLAYERCOUNT]];
			inGame = true;
			spec_info = INTER_PSTATE_DEAD;
		}
	}
	else {
		// alive range
		pnum = PInterGroups[INTER_PSTATE_ALIVE][boxid];
		inGame = true;
	}
	
	if(pnum >= 0 && pnum <= MAXPLAYERS) {
		int tid = pnum + P_TIDSTART;
		int y_wrap = SCOREBOARD_WRAPY;
		
		// draw the box
		if(!inGame || spec_info) {
			y_wrap += 96.0;
			SetFont("SCBRDSM");
		}
		else
			SetFont("LDTITINF");

		int mx = HUDMAX_XF - (PlayerCursorData.posx & MMASK) + 16.1;
		int my = HUDMAX_YF - (PlayerCursorData.posy & MMASK) + 16.1;
		// to force them to appear in window
		if(mx > SCOREBOARD_WRAPX)
			mx = SCOREBOARD_WRAPX + 0.1;
		if(my > y_wrap)
			my = y_wrap + 0.1;
		HudMessage(s:"A"; HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVERBG, CR_WHITE, mx, my, SCOREBOARD_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		
		SetFont("SMALLFONT");
		
		// text adjustment
		SetHudSize(HUDMAX_X * 3 / 2, HUDMAX_Y * 3 / 2, 1);
		mx *= 3; mx /= 2;
		my *= 3; my /= 2;
		mx &= MMASK;
		my &= MMASK;
		mx += 160.4;
		my += 24.1;
		
		SetHudClipRect(-96 + (mx >> 16), -48 + (my >> 16), 256, 288, 256, 1);
		
		HudMessage(
			n:pnum + 1;
			HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVERBG_WHO, CR_WHITE, mx, my, SCOREBOARD_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);
		
		if(spec_info != INTER_PSTATE_SPECTATING) {
			if(spec_info == INTER_PSTATE_DEAD) {
				// dead spectator
				HudMessage(s:"\cgDEAD"; HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVERBG_CLASS, CR_WHITE, mx, my + 16.0, SCOREBOARD_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
				DeleteTextRange(DND_SCBRDID_HOVER_LIFE, DND_SCBRDID_HOVER_CREDIT);
			}
			else {
				int temp = CheckActorInventory(tid, "DnD_Character");
				// alive and well
				if(temp) {
					HudMessage(
						s:"\cd", l:"DND_STAT18", s:" \c-", d:CheckActorInventory(tid, "Level"),
						s:"\c[J7] ", l:GetClassLabel(StrParam(s:"CLASS", d:CheckActorInventory(tid, "DnD_Character") - 1), DND_CLASS_LABEL_NAME);
						HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVERBG_CLASS, CR_WHITE, mx, my + 16.0, SCOREBOARD_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
					);
				}
				
				// life
				HudMessage(
					s:"\cd", l:"IATTR_T0", s:": \c-", d:GetActorProperty(tid, APROP_HEALTH);
					HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVER_LIFE, CR_WHITE, mx, my + 32.0, SCOREBOARD_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
				);
				
				// armor
				HudMessage(
					s:"\cd", l:"IATTR_T1", s:": \c-", d:CheckActorInventory(tid, "Armor");
					HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVER_ARMOR, CR_WHITE, mx, my + 48.0, SCOREBOARD_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
				);
				
				// exp
				HudMessage(
					s:"\cd", l:"DND_STAT16", s:": \c-", d:GetActorStat(tid, STAT_EXP), s:" / ", d:LevelCurve[GetActorStat(tid, STAT_LVL) - 1];
					HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVER_EXP, CR_WHITE, mx, my + 64.0, SCOREBOARD_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
				);

				// credit
				HudMessage(
					s:"\cd", l:"DND_STAT19", s:": \c-", d:GetActorStat(tid, STAT_CRED);
					HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVER_CREDIT, CR_WHITE, mx, my + 80.0, SCOREBOARD_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
				);
			}
		}
		else {
			// spectator
			DeleteTextRange(DND_SCBRDID_HOVER_LIFE, DND_SCBRDID_HOVER_CREDIT);
			HudMessage(s:"\SPECTATING"; HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVERBG_CLASS, CR_WHITE, mx, my + 16.0, SCOREBOARD_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
		
		SetHudClipRect(0, 0, 0, 0, 0);
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	}
	
	return pnum;
}

Script "DnD Scoreboard Loop" (int time, int total_mons) CLIENTSIDE {
	bool redraw = false;
	bool rebuild_boxes = false;
	int tics = time * TICRATE;
	int min_y = 0, prevmin_y = 0;
	int prevhover = MAINBOX_NONE;
	int temp = 0, prevplayers = 0;
	int cpn = ConsolePlayerNumber();
	int alpha = 0;
	
	// initial draw and initialization steps
	PlayerCursorData.posx = HUDMAX_XF / 2;
	PlayerCursorData.posy = HUDMAX_YF / 2;
	
	// group players into dead, spectating or alive sorting using pnum
	
	while(tics) {
		rebuild_boxes = false;
		redraw = false;

		// cursor setup
		PlayerCursorData.posx = GetCursorPos(GetPlayerInput(cpn, INPUT_YAW), MOUSE_INPUT_X);
		PlayerCursorData.posy = GetCursorPos(GetPlayerInput(cpn, INPUT_PITCH), MOUSE_INPUT_Y);
		PlayerCursorData.itemHovered = GetTriggeredBoxOnIntermissionPane(PlayerCursorData.posx, PlayerCursorData.posy, INTERMISSION_SCROLLSCALE * ScrollPos.x);

		if(prevhover != PlayerCursorData.itemHovered) {
			if(PlayerCursorData.itemHovered != MAINBOX_NONE)
				SpawnForced("ScoreBoardHoverSoundPlayer", 0, 0, 0);
			redraw = true;
		}

		// draw the cursor
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
		DrawCursor();
		
		// initially always divisible by 5 (TICRATE) so we always enter here first
		if(!(tics % 5)) {
			temp = GetPlayerCountAny();

			int prev_alive = ScoreboardData[DND_SCBRD_ALIVEPLAYERCOUNT];
			int prev_dead = ScoreboardData[DND_SCBRD_DEADPLAYERCOUNT];
			int prev_spec = ScoreboardData[DND_SCBRD_SPECPLAYERCOUNT];
			
			// check by grouping players to see if any category count changed
			GroupScoreboardPlayers();
			
			// player count changed so we need to recreate box list
			if
			(
				prevplayers != temp ||
				prev_alive != ScoreboardData[DND_SCBRD_ALIVEPLAYERCOUNT] ||
				prev_dead != ScoreboardData[DND_SCBRD_DEADPLAYERCOUNT] ||
				prev_spec != ScoreboardData[DND_SCBRD_SPECPLAYERCOUNT]
			)
			{
				prevplayers = temp;
				redraw = true;
				rebuild_boxes = true;
			}
			
			if(temp > MAX_INTERMISSION_PLAYERS_SHOWN) {
				min_y = -((temp - MAX_INTERMISSION_PLAYERS_SHOWN - 1) * 9 / 2);
				redraw |= min_y != prevmin_y;
			}
		}
		
		int p_highlight = DrawHoveredPlayerData();
		
		// check if we should redraw here -- we check for actor as the owner of the script is not a player, but the server
		redraw |= HandleIntermissionInputs(min_y);
		redraw |= CheckActorInventory(cpn + P_TIDSTART, "DnD_ScoreboardRefresher");
		
		// display press key to continue text
		SetFont("INTERFONT");
		SetHudSize(640, 480, 1);
		HudMessage(
			s:"\c[L7]", l:"CLASS_PRESS", s:" \ci", k:"+use", s:"\c[L7], \ci", k:"+attack", s:" \c[L7]", l:"DND_OR", s:" \ci", k:"+altattack", s: " \c[L7]", l:"DND_TOREADY", s:"!";
			HUDMSG_PLAIN | HUDMSG_ALPHA, DND_SCBRDID_CONTINUE, -1, 240.4, 424.1, 0.0, abs(sin(alpha * 1.0 / 360))
		);

		--tics;
		
		Delay(const:1);
		
		// alpha increment
		alpha = (alpha + 4) % 360;
		
		prevmin_y = min_y;
		
		if(redraw) {
			// build the boxes and group players into categories
			if(rebuild_boxes)
				BuildScoreboardBoxes();
		
			DrawScoreboard(time, ScrollPos.x, total_mons, p_highlight);
		}
		
		prevhover = PlayerCursorData.itemHovered;
	}
	SetResultValue(0);
}

#endif
