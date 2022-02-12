#ifndef DND_SCOREBOARD_IN
#define DND_SCOREBOARD_IN

enum {
	DND_SCBRD_PLAYEREXITED1,
	DND_SCBRD_PLAYEREXITED2,
	DND_SCBRD_PLAYEREXITCOUNT,
	DND_SCBRD_TIMER
};
#define MAX_SCOREBOARD_DATA (DND_SCBRD_TIMER + 1)

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
	INTER_PSTATE_DEAD,
	INTER_PSTATE_SPECTATING
};
global bool 20: PInterGroups[][MAXPLAYERS];

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
		
		if(!forcedExit) {
			if(democracy) {
				// mark this player as already exited
				MarkPlayerAsExited(pnum);
				
				// first 10 is used as a simple divisor, anything above is treated as an actual percentage
				// so this allows things like "half the players" or "one third players" exited thing much simpler
				if(democracy >= 1 && democracy <= 10) {
					if(ScoreboardData[DND_SCBRD_PLAYEREXITCOUNT] >= GetActivePlayerCount() / democracy)
						democracy_result = true;
					else {
						// error message
					}
				}
				else if(democracy > 10) {
					if(ScoreboardData[DND_SCBRD_PLAYEREXITCOUNT] >= GetActivePlayerCount() * democracy / 100)
						democracy_result = true;
					else {
						// error message
					}
				}
			}
			
			if(monpct) {
				if(GetLevelInfo(LEVELINFO_KILLED_MONSTERS) >= GetLevelInfo(LEVELINFO_TOTAL_MONSTERS) * monpct / 100)
					monpct_result = true;
				else {
					// error message
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
			SetPlayerProperty(1, 1, PROP_TOTALLYFROZEN);
			SetPlayerProperty(1, 2, PROP_INVULNERABILITY);
			
			// set activator to world
			SetActivator(-1);
			
			// display
			// get the same music id for everyone and make sure everyone in the game runs this script
			democracy = GetLevelInfo(LEVELINFO_TOTAL_MONSTERS);
			
			ACS_NamedExecuteWithResult("DnD Scoreboard Display", ScoreboardData[DND_SCBRD_TIMER], democracy);
			
			// timer
			democracy = ScoreboardData[DND_SCBRD_TIMER];
			while(democracy--) {
				Delay(const:TICRATE);
			}
			
			if(!isSecretExit)
				Exit_Normal(0);
			else
				Exit_Secret(0);
			Delay(1);
		}
	}
}

enum {
	DND_SCBRDID_HOVERBG_WHO = 1500,
	DND_SCBRDID_HOVER_LIFE,
	DND_SCBRDID_HOVER_ARMOR,
	DND_SCBRDID_HOVER_EXP,
	DND_SCBRDID_HOVER_CREDIT,
	DND_SCBRDID_HOVERBG,
	DND_SCBRDID_MAPCOMPLETE,
	DND_SCBRDID_STATUSICON,
	DND_SCBRDID_PLAYER,
	DND_SCBRDID_PLAYERUNDERLINE,
	DND_SCBRDID_KILLS,
	DND_SCBRDID_KILLSUNDERLINE,
	DND_SCBRDID_DAMAGEDEALT,
	DND_SCBRDID_DAMAGEDEALTUNDERLINE,
	DND_SCBRDID_DAMAGETAKEN,
	DND_SCBRDID_DAMAGETAKENUNDERLINE,
	DND_SCBRDID_PDATA,
	
	DND_SCBRDID_BG = 2500
};
#define DND_SCBRD_PDATA_THINGS 5 // status, name, damage dealt, damage taken, kill pct

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
	int lim = MAXPLAYERS;//GetPlayerCountAny();
	for(int i = 0; i < lim; ++i) {
		// reset to 0
		PInterGroups[INTER_PSTATE_ALIVE][i] = false;
		PInterGroups[INTER_PSTATE_DEAD][i] = false;
		PInterGroups[INTER_PSTATE_SPECTATING][i] = false;
		
		if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART))
			PInterGroups[INTER_PSTATE_ALIVE][i] = true;
		else {
			int spec = PlayerIsSpectator(i);
			if(spec == 2)
				PInterGroups[INTER_PSTATE_DEAD][i] = true;
			else if(spec == 1)
				PInterGroups[INTER_PSTATE_SPECTATING][i] = true;
		}
	}
}

void BuildScoreboardBoxes() {
	ResetIntermissionPane();
	
	
}

Script "DnD Scoreboard Display" (int time, int total_mons) CLIENTSIDE {
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
	
	/* 
		foreground stuff
		
		STATUSICON - PLAYERNAME (LEVEL - CLASSNAME) ---------- SURVIVED / DEAD / SPECTATING
		
		if too many players exist, draw a scrollbar to the rightmost side of the screen to allow scrolling + allow scroll with prev/next wep
		or forward/backward input
	*/
	
	// build the boxes and group players into categories
	GroupScoreboardPlayers();
	BuildScoreboardBoxes();
	
	Delay(const:1);
	ACS_NamedExecuteWithResult("DnD Scoreboard Loop", time, total_mons);
	
	SetResultValue(0);
}

void DrawScoreboard(int time, int ScrollPos, int total_mons, bool rebuild_boxes) {
	int temp, pct, col = CR_RED, tid, y;
	bool triggered = false;
	str text;
	
	SetHudSize(640, 480, 1);
	
	// map name in center of bg image --- make these fall into place here
	SetFont("INTERFONT");
	HudMessage(
		s:"\cj", n:PRINTNAME_LEVEL, s:": \cf", n:PRINTNAME_LEVELNAME, s:"\n\n\c[M3]", l:"DND_COMPLETED";
		HUDMSG_PLAIN, DND_SCBRDID_MAPCOMPLETE, CR_GOLD,
		240.4, 360.1, 0
	);
	
	// setup the clipping rectangle
	
	// playerlist
	SetHudSize(1024, 768, 1);
	SetHudClipRect(0, 36, 1024, 520, 1024);
	
	if(rebuild_boxes)
		ResetIntermissionPane();
	
	for(int i = 0; i < MAXPLAYERS; ++i) {
		//if(!PlayerInGame(i))
		//	continue;
		
		triggered = false;
		tid = i + P_TIDSTART;
		y = 36.1 + 18.0 * i + INTERMISSION_SCROLLFACTOR * ScrollPos;
		col = CR_UNTRANSLATED;
		
		if(PlayerCursorData.itemHovered - INTERMISSION_INITIAL_BUTTONS - 1 == i) {
			col = CR_GOLD;
			triggered = true;
		}
	
		// icon
		temp = PlayerIsSpectator(i);
		if(temp == 2)
			SetFont("DEADICO");
		else if(temp == 1)
			SetFont("SPECICO");
		else
			SetFont("LIVEICO");
		HudMessage(
			s:"A"; 
			HUDMSG_PLAIN, DND_SCBRDID_PDATA + DND_SCBRD_PDATA_THINGS * i, CR_UNTRANSLATED, 
			16.1, y, 0
		);
		
		SetFont("INTERFONT");
		
		// name
		/*if(!PlayerIsSpectator(i)) {
			HudMessage(
				n:(i + 1), s:" \c[Q9](Lvl ", d:GetActorStat(tid, STAT_LVL), s: " \cu", l:GetClassName(GetActorPlayerClass(tid)), s:"\c[Q9])"; 
				HUDMSG_PLAIN, DND_SCBRDID_PDATA + (DND_SCBRD_PDATA_THINGS * i + 1), CR_UNTRANSLATED, 
				35.1, y, 0
			);
		}
		else {*/
		text = StrParam(n:i + 1);
		temp = StrLen(text);
		// don't clutter the intermission board
		if(temp > MAX_INTERMISSION_PNAMELEN)
			text = StrParam(s:StrLeft(text, MAX_INTERMISSION_PNAMELEN), s:"...");
			temp = MAX_INTERMISSION_PNAMELEN + 3; // 3 for "..."
			HudMessage(
				s:text; 
				HUDMSG_PLAIN, DND_SCBRDID_PDATA + (DND_SCBRD_PDATA_THINGS * i + 1), col, 
				35.1, y, 0
			);
			
		// create the hoverable player name box
		if(rebuild_boxes)
			AddBoxToInterPane_Points(464.0, 306.0 - INTERMISSION_BOX_YCHANGE * i, 464.0 - temp * INTERMISSION_CHAR_SCALE, 300.0 - INTERMISSION_BOX_YCHANGE * i);
		
		// damage dealt
		HudMessage(
			s:"\c[Q9]", s:GetStatistic(DND_STATISTIC_DAMAGEDEALT, tid); 
			HUDMSG_PLAIN, DND_SCBRDID_PDATA + (DND_SCBRD_PDATA_THINGS * i + 2), col, 
			468.4, y, 0
		);
		
		// damage taken
		HudMessage(
			s:"\c[Q9]", s:GetStatistic(DND_STATISTIC_DAMAGETAKEN, tid); 
			HUDMSG_PLAIN, DND_SCBRDID_PDATA + (DND_SCBRD_PDATA_THINGS * i + 3), col, 
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
			HUDMSG_PLAIN, DND_SCBRDID_PDATA + (DND_SCBRD_PDATA_THINGS * i + 4), col, 
			964.4, y, 0
		);
	}
	
	SetHudClipRect(0, 0, 0, 0, 0);
}

bool HandleIntermissionInputs(int min_y) {
	bool res = ListenScroll(min_y, 0);
	
	return res;
}

void DrawHoveredPlayerData() {
	int pnum = PlayerCursorData.itemHovered - INTERMISSION_INITIAL_BUTTONS - 1;
	if(pnum >= 0 && pnum <= MAXPLAYERS) {
		int tid = pnum + P_TIDSTART;
		int y_wrap = SCOREBOARD_WRAPY;
		// draw the box
		int spec_info = PlayerIsSpectator(pnum);
		bool inGame = PlayerInGame(pnum);
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
		HudMessage(s:"A"; HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVERBG, CR_WHITE, mx, my, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		
		SetFont("SMALLFONT");
		
		// text adjustment
		SetHudSize(HUDMAX_X * 3 / 2, HUDMAX_Y * 3 / 2, 1);
		mx *= 3; mx /= 2;
		my *= 3; my /= 2;
		mx &= MMASK;
		my &= MMASK;
		mx += 160.4;
		my += 36.1;
		
		SetHudClipRect(-96 + (mx >> 16), -48 + (my >> 16), 256, 288, 256, 1);
		
		if(inGame) {
			if(spec_info == 2) {
				// dead spectator
				HudMessage(s:"\cgDEAD"; HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVERBG_WHO, CR_WHITE, mx, my, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
				DeleteTextRange(DND_SCBRDID_HOVER_LIFE, DND_SCBRDID_HOVER_CREDIT);
			}
			else {
				// alive and well
				HudMessage(
					s:"\cd", l:"DND_STAT18", s:" \c-", d:CheckActorInventory(tid, "Level"),
					s:"\c[J7] ", l:GetClassLabel(StrParam(s:"CLASS", d:CheckActorInventory(tid, "DnD_Character") - 1), DND_CLASS_LABEL_NAME);
					HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVERBG_WHO, CR_WHITE, mx, my, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
				);
				
				// life
				HudMessage(
					s:"\cd", l:"IATTR_T0", s:": \c-", d:GetActorProperty(tid, APROP_HEALTH);
					HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVER_LIFE, CR_WHITE, mx, my + 16.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
				);
				
				// armor
				HudMessage(
					s:"\cd", l:"IATTR_T1", s:": \c-", d:CheckActorInventory(tid, "Armor");
					HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVER_ARMOR, CR_WHITE, mx, my + 32.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
				);
				
				// exp
				HudMessage(
					s:"\cd", l:"DND_STAT16", s:": \c-", d:GetActorStat(tid, STAT_EXP), s:" / ", d:LevelCurve[GetActorStat(tid, STAT_LVL) - 1];
					HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVER_EXP, CR_WHITE, mx, my + 48.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
				);

				// credit
				HudMessage(
					s:"\cd", l:"DND_STAT19", s:": \c-", d:GetActorStat(tid, STAT_CRED);
					HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVER_CREDIT, CR_WHITE, mx, my + 64.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
				);
			}
		}
		else {
			// spectator
			DeleteTextRange(DND_SCBRDID_HOVER_LIFE, DND_SCBRDID_HOVER_CREDIT);
			HudMessage(s:"\SPECTATING"; HUDMSG_FADEOUT | HUDMSG_ALPHA, DND_SCBRDID_HOVERBG_WHO, CR_WHITE, mx, my, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
		
		SetHudClipRect(0, 0, 0, 0, 0);
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	}
}

Script "DnD Scoreboard Loop" (int time, int total_mons) CLIENTSIDE {
	bool redraw = false;
	bool rebuild_boxes = false;
	int tics = time * TICRATE;
	int min_y = 0, prevmin_y = 0;
	int prevhover = MAINBOX_NONE;
	int temp = 0, prevplayers = 0;
	int cpn = ConsolePlayerNumber();
	
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
			temp = MAXPLAYERS;//GetPlayerCountAny();
			
			// player count changed so we need to recreate box list
			if(prevplayers != temp) {
				prevplayers = temp;
				redraw = true;
				rebuild_boxes = true;
			}
			
			if(temp > MAX_INTERMISSION_PLAYERS_SHOWN) {
				min_y = -((temp - MAX_INTERMISSION_PLAYERS_SHOWN - 1) * 9 / 2);
				redraw |= min_y != prevmin_y;
			}
		}
		
		DrawHoveredPlayerData();
		
		// check if we should redraw here
		redraw |= HandleIntermissionInputs(min_y);

		--tics;
		
		Delay(const:1);
		
		prevmin_y = min_y;
		
		if(redraw)
			DrawScoreboard(time, ScrollPos.x, total_mons, rebuild_boxes);
		
		prevhover = PlayerCursorData.itemHovered;
	}
	SetResultValue(0);
}

#endif
