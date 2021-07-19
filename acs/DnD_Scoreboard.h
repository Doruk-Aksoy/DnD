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

int ScoreboardData[MAX_SCOREBOARD_DATA];

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

int GetActivePlayerCount() {
	int res = 0;
	
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && !PlayerIsSpectator(i))
			++res;
	}
	
	return res;
}

Script 1690 (int isSecretExit) {
	// this player hasn't triggered the exit before
	int this = ActivatorTID();
	int pnum = this - P_TIDSTART;
	if(!PlayerTriggeredExit(pnum)) {
		bool proceed = false;
		bool democracy_result = false;
		bool monpct_result = false;
		int democracy = GetCVar("dnd_exit_democratic");
		int monpct = GetCVar("dnd_exit_monsterpercent");
		
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
		
		if(proceed) {
			ACS_NamedExecuteAlways("DnD Scoreboard Display", 0);
			
			// start the timer in the above function too, then after it expires / all players are ready we can exit
			ScoreboardData[DND_SCBRD_TIMER] = Clamp_Between(GetCVar("dnd_exit_timer"), 5, 30);
			
			// freeze monsters / players during this time
			GiveInventory("MenuFreeze", 1);
			
			// display
			
			// get the same music id for everyone and make sure everyone in the game runs this script
			monpct = random(1, MAX_INTERMISSION_SONGS);
			for(int i = 0; i < MAXPLAYERS; ++i) {
				if(PlayerInGame(i)) {
					SetActivator(i + P_TIDSTART);
					ACS_NamedExecuteAlways("DnD Scoreboard Display", 0, ScoreboardData[DND_SCBRD_TIMER], GetLevelInfo(LEVELINFO_TOTAL_MONSTERS), monpct);
				}
			}
			
			// set it back to this one guy who triggered the exit condition
			SetActivator(this);
			
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

Script "DnD Scoreboard Display" (int time, int mus) CLIENTSIDE {
	PlaySound(0, StrParam(s:"Intermission", d:mus), CHAN_BODY | CHAN_NOPAUSE, 1.0, true, ATTN_NONE);
	
	// hopefully to clear the chat
	Log(s:"\n\n\n\n\n\n\n\n\n\n");

	// bg
	SetFont("INTERPCW");
	SetHudSize(640, 480, 1);
	HudMessage(s:"A"; HUDMSG_PLAIN, DND_SCBRDID_BG, CR_UNTRANSLATED, 320.0, 240.0, 0);
	
	/* 
		foreground stuff
		
		TICKBOX - PLAYERNAME (LEVEL - CLASSNAME) ---------- SURVIVED / DEAD / SPECTATING
		
		if too many players exist, draw a scrollbar to the rightmost side of the screen to allow scrolling + allow scroll with prev/next wep
		or forward/backward input
	*/
	
	ACS_NamedExecuteWithResult("DnD Scoreboard Loop", time);
}

enum {
	DND_SCBRDID_MAPCOMPLETE = 2,
	DND_SCBRDID_STATUSICON,
	DND_SCBRDID_PLAYER,
	DND_SCBRDID_KILLS,
	DND_SCBRDID_PDATA,
	
	DND_SCBRDID_BG = 250
};
#define DND_SCBRD_PDATA_THINGS 3 // status, name, kill pct

void DrawScoreboard(int time, int ScrollPos, int total_mons) {
	int temp, pct, col = CR_RED;
	
	SetHudSize(640, 480, 1);
	
	// map name in center of bg image --- make these fall into place here
	SetFont("INTERFONT");
	HudMessage(
		s:"\cj", n:PRINTNAME_LEVEL, s:": \cf", n:PRINTNAME_LEVELNAME, s:"\n\n\c[M3]", l:"DND_COMPLETED";
		HUDMSG_PLAIN, DND_SCBRDID_MAPCOMPLETE, CR_GOLD,
		240.4, 360.1, 0
	);
	
	// player header
	HudMessage(
		l:"DND_PLAYER"; 
		HUDMSG_PLAIN, DND_SCBRDID_PLAYER, CR_UNTRANSLATED, 
		32.1, 0.1, 0
	);
	
	// kills header
	HudMessage(
		l:"DND_KILLS"; 
		HUDMSG_PLAIN, DND_SCBRDID_KILLS, CR_UNTRANSLATED, 
		600.4, 0.1, 0
	);
	
	// setup the clipping rectangle
	
	// playerlist
	SetHudSize(1024, 768, 1);
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(!PlayerInGame(i))
			continue;
	
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
			32.1, 32.1 + 16.0 * i + 4.0 * ScrollPos, 0
		);
		
		SetFont("INTERFONT");
		
		// name
		HudMessage(
			n:(i + 1), s:" \c[Q9](Lvl ", d:CheckInventory("Level"), s: " \cu", l:GetClassName(GetPlayerClass()), s:"\c[Q9])"; 
			HUDMSG_PLAIN, DND_SCBRDID_PDATA + (DND_SCBRD_PDATA_THINGS * i + 1), CR_UNTRANSLATED, 
			51.1, 32.1 + 16.0 * i + 4.0 * ScrollPos, 0
		);
		
		// kill display
		temp = CheckInventory("DnD_Kills");
		if(!total_mons)
			pct = 0;
		else
			pct = temp * 100 / total_mons;
		
		if(pct > 75)
			col = CR_GREEN;
		else if(pct > 50)
			col = CR_YELLOW;
		else if(pct > 25)
			col = CR_ORANGE;
		
		HudMessage(
			d:temp, s: " (", d: pct, s:"%)"; 
			HUDMSG_PLAIN, DND_SCBRDID_PDATA + (DND_SCBRD_PDATA_THINGS * i + 2), col, 
			960.4, 32.1 + 16.0 * i + 4.0 * ScrollPos, 0
		);
	}
	
	SetHudClipRect(0, 0, 0, 0, 0);
}

Script "DnD Scoreboard Loop" (int time, int total_mons) CLIENTSIDE {
	bool redraw = false;
	int tics = time * 35;
	int ScrollPos = 0;

	// initial draw
	DrawScoreboard(time, 0, total_mons);
	
	while(tics) {
		redraw = false;
		
		// check if we should redraw here
		

		--tics;
		Delay(const:1);
		
		if(redraw)
			DrawScoreboard(time, ScrollPos, total_mons);
	}
	SetResultValue(0);
}

#endif
