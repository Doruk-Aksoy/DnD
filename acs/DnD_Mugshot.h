#ifndef DND_MUGS_IN
#define DND_MUGS_IN

#include "DnD_Hud.h"

#define VID_REFRESH_TIME (TICRATE * 3);
#define DND_MUGIDLE_TIME 17
#define DND_MUGPAIN_TIME 35
#define DND_MUGGRIN_TIME (TICRATE * 3)
#define DND_MUGXDTH_TIME 3
#define DND_MUGGOD_TIME 40
#define DND_MUGGOD_TIME2 3
#define DND_MUGGOD_TIME3 6

#define DND_OUCH_PERCENT 25 // something hits you for more than 25% of your max hp you're gonna go ouch!
#define DND_MUGSHOT_HP_PERCENT 20
#define DND_XDEATH_HP_PERCENT 20 // if hp goes to negative 20% or lower of your max hp you are gibbed
#define DND_MUGSHOT_XDEATHFRAMES 4 // stops at frame 4
#define DND_MUGSHOT_GRITTIME (TICRATE * 5 / 2) // holding fire or altfire for 2.5 seconds will make mugshot grit teeth
#define DND_MUGSHOT_GRITTICS (TICRATE * 10)

#define HUD_MUGSIZEX 480
#define HUD_MUGSIZEY (3 * HUD_MUGSIZEX / 4)

/*
	-- all pain frames last 35 tics
	-- xdeath is 3 tics each frame for 5 frames
	
	-- god follows 40, 3, 6, 3
	
	Another note: Since the baseline value for the staring mugshot is used on x offset, anything that isn't that needs to be moved with sprite offsets!
	Ex: Stare image size is 24, but looking right frame is 28, move looking right by 4 units right in sprite offset!
*/

// only clientside data -- if this has something in the range of a monster tid that means we got ouchied by it, storing its tid here
enum {
	PMUG_CLASS,
	PMUG_STR,
	PMUG_STATE,
	PMUG_FRAME,
	PMUG_TIC,
	PMUG_TRIGGER_TIC,
	PMUG_ISRUNNING
};
#define MAX_MUGSHOT_DATA (PMUG_ISRUNNING + 1)
int PlayerMugshotData[MAX_MUGSHOT_DATA] = { DND_PLAYER_DOOMGUY, 0, DND_MUGSTATE_IDLE, 0, DND_MUGIDLE_TIME, 0, 0 };

Script "DnD Player MugshotData Update" (int mstate, int frame, int val, int ignoreInvul) CLIENTSIDE {
	bool isInvul = (!ignoreInvul && PlayerIsInvulnerable());

	if(ConsolePlayerNumber() != PlayerNumber()) {
		SetResultValue(0);
		Terminate;
	}
	
	// dont have other updates interrupt over dying, ex: weapon pickups
	if(mstate == DND_MUGSTATE_IDLE || (mstate != DND_MUGSTATE_DEATH && mstate != DND_MUGSTATE_XDEATH && isAlive())) {
		PlayerMugshotData[PMUG_STATE] = mstate;
		PlayerMugshotData[PMUG_FRAME] = frame;
		PlayerMugshotData[PMUG_TIC] = val;
	}
	
	SetResultValue(0);
}

Script "DnD Player Mugshot Class Update" (int pclass) CLIENTSIDE {
	if(ConsolePlayerNumber() == PlayerNumber()) {
		PlayerMugshotData[PMUG_CLASS] = pclass;
		PlayerMugshotData[PMUG_STR] = GetMugshotString(pclass);
	}
	SetResultValue(0);
}

void HandleDeathMugshot(int curhp) {
	if(-curhp < CheckInventory("PlayerHealthCap") * DND_XDEATH_HP_PERCENT / 100) {
		// if damage - curhp difference is less than the xdeath threshold, normal death
		// ex: you had 20 hp, you took 100 damage. its 80 vs xdeath percent of hp cap. if health cap is 100 you go to xdeath. But if it's 400+ you don't.
		PlayerMugshotData[PMUG_TIC] = 0;
		PlayerMugshotData[PMUG_FRAME] = 0;
		PlayerMugshotData[PMUG_STATE] = DND_MUGSTATE_DEATH;
	}
	else {
		// xdeath
		PlayerMugshotData[PMUG_TIC] = 3;
		PlayerMugshotData[PMUG_FRAME] = 0;
		PlayerMugshotData[PMUG_STATE] = DND_MUGSTATE_XDEATH;
	}
}

Script "DnD Player MugshotData Ouchies" (int attacker_tid, int damage) CLIENTSIDE {
	if(PlayerIsInvulnerable() || ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	int curhp = GetActorProperty(0, APROP_HEALTH);
	if(curhp > 0) {
		PlayerMugshotData[PMUG_TIC] = DND_MUGPAIN_TIME;
		PlayerMugshotData[PMUG_FRAME] = 0;
		
		// big damage check
		if(damage > CheckInventory("PlayerHealthCap") * DND_OUCH_PERCENT / 100) {
			PlayerMugshotData[PMUG_STATE] = DND_MUGSTATE_OUCH;
			Terminate;
		}
		
		// if we hurt ourselves only look forward
		if(attacker_tid == ActivatorTID())
			PlayerMugshotData[PMUG_STATE] = DND_MUGSTATE_PAIN;
		else {
			// determine relative position of us vs whoever attacked us, find out angle to face
			int ang = AngleToFace(ActivatorTID(), attacker_tid) - GetActorAngle(0);
			
			// 45 degree diff
			if(ang < -0.125)
				PlayerMugshotData[PMUG_STATE] = DND_MUGSTATE_OUCHRIGHT;
			else if(ang > 0.125)
				PlayerMugshotData[PMUG_STATE] = DND_MUGSTATE_OUCHLEFT;
			else
				PlayerMugshotData[PMUG_STATE] = DND_MUGSTATE_PAIN;
		}
	}
	else
		HandleDeathMugshot(curhp);
}

enum {
	DND_MUGSTATE_EVILGRIN,
	DND_MUGSTATE_GOD,
	DND_MUGSTATE_KILL,
	DND_MUGSTATE_PAIN,
	DND_MUGSTATE_OUCH,
	DND_MUGSTATE_IDLE,
	DND_MUGSTATE_OUCHLEFT,
	DND_MUGSTATE_OUCHRIGHT,
	DND_MUGSTATE_DEATH,
	DND_MUGSTATE_XDEATH
};

void ResetMugstate(bool ignore_inv = false) {
	ACS_NamedExecuteWithResult("DnD Player MugshotData Update", DND_MUGSTATE_IDLE, 0, DND_MUGIDLE_TIME, ignore_inv);
}

str GetMugshotString(int pclass) {
	switch(pclass) {
		case DND_PLAYER_DOOMGUY:
		return "STF";
		
		case DND_PLAYER_MARINE:
		return "MAR";
		
		case DND_PLAYER_HOBO:
		return "REN";
		
		case DND_PLAYER_PUNISHER:
		return "BAD";
		
		case DND_PLAYER_WANDERER:
		return "CRV";
		
		case DND_PLAYER_CYBORG:
		return "CYG";
		
		case DND_PLAYER_BERSERKER:
		return "BER";
	}
	
	return "";
}

// this is based on image dimension
int GetMugshotXOffset(int pclass) {
	switch(pclass) {
		case DND_PLAYER_DOOMGUY:
		return 12.0;
		
		case DND_PLAYER_MARINE:
		return 17.0;
		
		case DND_PLAYER_HOBO:
		return 12.0;
		
		case DND_PLAYER_PUNISHER:
		return 12.0;
		
		case DND_PLAYER_WANDERER:
		return 18.0;
		
		case DND_PLAYER_CYBORG:
		return 12.0;
		
		case DND_PLAYER_BERSERKER:
		return 21.0;
	}
	return "";
}

// returns something of the form mugstr + frame + offset as a string that corresponds to a mugshot image
// we use PlayerHealthCap inventory item to check for the cap here, as it always gets called at least once, if not we assume it's 100%
str GetMugshotGraphic(str mugstr) {
	int hp_pct;
	
	switch(PlayerMugshotData[PMUG_STATE]) {
		case DND_MUGSTATE_EVILGRIN:
			hp_pct = GetHealthPercent();
		return StrParam(s:mugstr, s:"EVL", d:5 - (hp_pct + DND_MUGSHOT_HP_PERCENT - 1) / DND_MUGSHOT_HP_PERCENT);
		
		case DND_MUGSTATE_GOD:
		return StrParam(s:mugstr, s:"GOD", d:PlayerMugshotData[PMUG_FRAME]);
		
		// they are new states for discerning purposes of animation times and state transitions, they are otherwise equivalent
		case DND_MUGSTATE_KILL:
		case DND_MUGSTATE_PAIN:
			hp_pct = GetHealthPercent();
		return StrParam(s:mugstr, s:"KILL", d:5 - (hp_pct + DND_MUGSHOT_HP_PERCENT - 1) / DND_MUGSHOT_HP_PERCENT);
		
		case DND_MUGSTATE_OUCH:
			hp_pct = GetHealthPercent();
		return StrParam(s:mugstr, s:"OUCH", d:5 - (hp_pct + DND_MUGSHOT_HP_PERCENT - 1) / DND_MUGSHOT_HP_PERCENT);
		
		case DND_MUGSTATE_IDLE:
			hp_pct = GetHealthPercent();
		return StrParam(s:mugstr, s:"ST", d:5 - (hp_pct + DND_MUGSHOT_HP_PERCENT - 1) / DND_MUGSHOT_HP_PERCENT, d:PlayerMugshotData[PMUG_FRAME]);
		
		case DND_MUGSTATE_OUCHLEFT:
			hp_pct = GetHealthPercent();
		return StrParam(s:mugstr, s:"TL", d:5 - (hp_pct + DND_MUGSHOT_HP_PERCENT - 1) / DND_MUGSHOT_HP_PERCENT, d:0);
		
		case DND_MUGSTATE_OUCHRIGHT:
			hp_pct = GetHealthPercent();
		return StrParam(s:mugstr, s:"TR", d:5 - (hp_pct + DND_MUGSHOT_HP_PERCENT - 1) / DND_MUGSHOT_HP_PERCENT, d:0);
		
		case DND_MUGSTATE_DEATH:
		return StrParam(s:mugstr, s:"DEAD", d:0);
		
		case DND_MUGSTATE_XDEATH:
		return StrParam(s:mugstr, s:"XDTH", d:PlayerMugshotData[PMUG_FRAME]);
	}

	return "STFST00";
}

void HandleMugshotState(int width, int height, str mugstr, int xoffset, int aspect) {
	// get mugshot frame and offset
	str mugframe = GetMugshotGraphic(mugstr);
	
	//if(GetActorProperty(0, APROP_HEALTH) <= 0)
	//	Log(s:mugframe);
	
	width = ((HUD_MUGSIZEX * width / 800) * FixedDiv(1.0, aspect)) >> 16;
	height = ((HUD_MUGSIZEY * height / 600) * FixedDiv(1.0, ASPECT_4_3)) >> 16;
		
	SetFont(mugframe);
	SetHudSize(width, height, true);
	
	// in case of rounding errors we need to properly get rid of the other parts...
	xoffset += GetHudLeft(width) << 16;
	xoffset &= 0xFFFF0000;
	
	if(xoffset < 0)
		xoffset += 0.6;
	else {
		xoffset += 0.40001;
	}
	
	//Log(f:xoffset);
	
	// > 10 means theres no point for mugshot
	if(GetCVar("screenblocks") > 10)
		HudMessage(s:""; HUDMSG_PLAIN | HUDMSG_NOTWITHFULLMAP, PLAYER_MUGSHOTID, CR_UNTRANSLATED, xoffset, (height << 16) - 22.0, 0.0);
	else
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_NOTWITHFULLMAP, PLAYER_MUGSHOTID, CR_UNTRANSLATED, xoffset, (height << 16) - 22.0, 0.0);
}

Script "DnD Player Mugshot" (void) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber() || PlayerMugshotData[PMUG_ISRUNNING])
		Terminate;
		
	PlayerMugshotData[PMUG_ISRUNNING] = 1;

	// debugger script, enable for tests
	//ACS_NamedExecuteWithResult("DnD Player Mugshot Cycler", pclass);
	int vidrefresh_time = VID_REFRESH_TIME - 1;
	int pnum = PlayerNumber();
	int frame_inc = 1;
	int hud_scale_fac = 1;
	
	// immediately update aspect ratio
	int aspect = GetAspectRatio();
	ScreenResOffsets[SCREEN_ASPECT_RATIO] = aspect;
	
	// we want twice the size
	int width = getcvar("vid_defwidth");
	int height = getcvar("vid_defheight");
	
	if(GetCVar("hud_scale")) {
		hud_scale_fac = max(1, min(width / 640, height / 400));
		width /= hud_scale_fac;
		height /= hud_scale_fac;
	}
	
	int xoffset = GetMugshotXOffset(PlayerMugshotData[PMUG_CLASS]);
	//if(aspect == ASPECT_4_3)
	//	xoffset /= 2;
	//else if(GetCVar("hud_scale"))
	//	xoffset = GetIntegerBits(xoffset * 8 / 5);

	// this is removed on spectate / map reset anyway
	while(!CheckInventory("DnD_IntermissionState")) {
		HandleMugshotState(width, height, PlayerMugshotData[PMUG_STR], xoffset, aspect);
		vidrefresh_time = (vidrefresh_time + 1) % VID_REFRESH_TIME;
		
		if(!vidrefresh_time) {
			// update width/height every 5 sec in case player fiddles with them
			width = getcvar("vid_defwidth");
			height = getcvar("vid_defheight");
			
			// if aspect somehow changed, update it
			if(aspect != ScreenResOffsets[SCREEN_ASPECT_RATIO])
				aspect = ScreenResOffsets[SCREEN_ASPECT_RATIO];
			
			if(GetCVar("hud_scale")) {
				xoffset = GetMugshotXOffset(PlayerMugshotData[PMUG_CLASS]);
				
				hud_scale_fac = max(1, min(width / 640, height / 400));
				
				// idk anymore
				if(aspect == ASPECT_16_10 && width > 1024 && width < 1600 && height > 768 && height < 1200) {
					//hud_scale_fac = 2;
					
					if(xoffset < 30.0)
						xoffset = xoffset * 23 / 24;
					else
						xoffset = xoffset * 21 / 24;
				}
				//else if(aspect == ASPECT_4_3 && width >= 1280 && width <= 1920 && height >= 960 && height <= 1440)
				//	hud_scale_fac = 2;
					
				//if(aspect == ASPECT_4_3)
				//	xoffset /= 2;
				//else
				//	xoffset = GetIntegerBits(GetMugshotXOffset(PlayerMugshotData[PMUG_CLASS]) * 8 / 5);
				
				width /= hud_scale_fac;
				height /= hud_scale_fac;
			}
			else {
				xoffset = GetMugshotXOffset(PlayerMugshotData[PMUG_CLASS]);
				//if(aspect == ASPECT_4_3)
				//	xoffset /= 2;
				//else if(GetCVar("hud_scale"))
				//	xoffset = GetIntegerBits(xoffset * 8 / 5);
			}
		}
		
		// constantly decrement
		if(PlayerMugshotData[PMUG_TIC]) {
			//Log(s:"tic count ", d:PlayerMugshotData[PMUG_TIC]);
			--PlayerMugshotData[PMUG_TIC];
			
			// god state check
			if(PlayerIsInvulnerable() && PlayerMugshotData[PMUG_STATE] != DND_MUGSTATE_GOD) {
				PlayerMugshotData[PMUG_STATE] = DND_MUGSTATE_GOD;
				PlayerMugshotData[PMUG_TIC] = DND_MUGGOD_TIME;
				PlayerMugshotData[PMUG_FRAME] = 0;
				frame_inc = 1;
				
				//Log(s:"make god");
			}
			
			// trigger hold check -- only check god as death is a terminal states, they have 0 tic, we wont even enter here
			// we still check for xdeath though, its animated
			if(PlayerMugshotData[PMUG_STATE] != DND_MUGSTATE_GOD && PlayerMugshotData[PMUG_STATE] != DND_MUGSTATE_XDEATH && (GetPlayerInput(-1, INPUT_BUTTONS) & (BT_ATTACK | BT_ALTATTACK))) {
				++PlayerMugshotData[PMUG_TRIGGER_TIC];
				if(PlayerMugshotData[PMUG_TRIGGER_TIC] >= DND_MUGSHOT_GRITTIME) {
					PlayerMugshotData[PMUG_STATE] = DND_MUGSTATE_KILL;
					PlayerMugshotData[PMUG_TIC] = DND_MUGSHOT_GRITTICS;
				}
			}
			else {
				PlayerMugshotData[PMUG_TRIGGER_TIC] = 0;
				
				// return to idle
				if(PlayerMugshotData[PMUG_STATE] == DND_MUGSTATE_KILL) {
					PlayerMugshotData[PMUG_STATE] = DND_MUGSTATE_IDLE;
					PlayerMugshotData[PMUG_TIC] = DND_MUGIDLE_TIME;
					PlayerMugshotData[PMUG_FRAME] = random(0, 2); // idle has 0 to 2
				}
			}
			
			// timer ran out, new state decision
			if(!PlayerMugshotData[PMUG_TIC]) {
				// xdeath is animated
				if(PlayerMugshotData[PMUG_STATE] == DND_MUGSTATE_XDEATH) {
					PlayerMugshotData[PMUG_TIC] = 3;
					++PlayerMugshotData[PMUG_FRAME];
					
					// halt state
					if(PlayerMugshotData[PMUG_FRAME] == DND_MUGSHOT_XDEATHFRAMES)
						PlayerMugshotData[PMUG_TIC] = 0;
				}
				else if(PlayerMugshotData[PMUG_STATE] == DND_MUGSTATE_GOD) {
					//Log(s:"is god new frame time from ", d:PlayerMugshotData[PMUG_FRAME], s: " ", d:frame_inc);
					// if no longer in possession of the invul tokens, go away from this state
					if(!PlayerIsInvulnerable()) {
						PlayerMugshotData[PMUG_STATE] = DND_MUGSTATE_IDLE;
						PlayerMugshotData[PMUG_TIC] = DND_MUGIDLE_TIME;
						PlayerMugshotData[PMUG_FRAME] = random(0, 2); // idle has 0 to 2
					}
					else {
						PlayerMugshotData[PMUG_FRAME] += frame_inc;
						if(!PlayerMugshotData[PMUG_FRAME]) {
							PlayerMugshotData[PMUG_TIC] = DND_MUGGOD_TIME;
							frame_inc = 1;
						}
						else if(PlayerMugshotData[PMUG_FRAME] == 1) {
							PlayerMugshotData[PMUG_TIC] = DND_MUGGOD_TIME2;
						}
						else if(PlayerMugshotData[PMUG_FRAME] == 2) {
							PlayerMugshotData[PMUG_TIC] = DND_MUGGOD_TIME3;
							frame_inc = -1;
						}
					}
				}
				else {
					PlayerMugshotData[PMUG_STATE] = DND_MUGSTATE_IDLE;
					PlayerMugshotData[PMUG_TIC] = DND_MUGIDLE_TIME;
					PlayerMugshotData[PMUG_FRAME] = random(0, 2); // idle has 0 to 2
				}
			}
		}
		
		Delay(const:1);
	}
	
	DeleteText(PLAYER_MUGSHOTID);
	
	SetResultValue(0);
}

// debugger -- enable when working on new mugshot stuff
/*Script "DnD Player Mugshot Cycler" (int pclass) CLIENTSIDE {
	str pclass_mug = GetMugshotString(pclass);
	int vidrefresh_time = 0;
	
	// we want twice the size
	int width = getcvar("vid_defwidth");
	int height = getcvar("vid_defheight");
	
	// immediately update aspect ratio
	int aspect = GetAspectRatio();
	ScreenResOffsets[SCREEN_ASPECT_RATIO] = aspect;
	
	int xoffset = GetMugshotXOffset(pclass);
	if(aspect == ASPECT_4_3)
		xoffset /= 2;
	xoffset += 0.4;
	
	int debug_state = DND_MUGSTATE_EVILGRIN;
	
	while(isAlive()) {
		if(GetPlayerInput(-1, INPUT_BUTTONS) & BT_USE) {
			HandleMugshotState(width, height, pclass_mug, xoffset, aspect);
			PlayerMugshotData[PMUG_STATE] = debug_state;
			PlayerMugshotData[PMUG_FRAME] = 0;
			PlayerMugshotData[PMUG_TIC] = 17;
			
			debug_state = (debug_state + 1) % (DND_MUGSTATE_XDEATH + 1);
		}
		
		vidrefresh_time = (vidrefresh_time + 1) % VID_REFRESH_TIME;
		
		if(!vidrefresh_time) {
			// update width/height every 5 sec in case player fiddles with them
			width = getcvar("vid_defwidth");
			height = getcvar("vid_defheight");
			
			// if aspect somehow changed, update it
			if(aspect != ScreenResOffsets[SCREEN_ASPECT_RATIO]) {
				aspect = ScreenResOffsets[SCREEN_ASPECT_RATIO];
				if(ScreenResOffsets[SCREEN_ASPECT_RATIO] == ASPECT_4_3)
					xoffset = GetMugshotXOffset(pclass) / 2 + 0.4;
				else
					xoffset = GetMugshotXOffset(pclass) + 0.4;
			}
		}
		
		// constantly decrement
		if(PlayerMugshotData[PMUG_TIC]) {
			--PlayerMugshotData[PMUG_TIC];
			if(!PlayerMugshotData[PMUG_TIC]) {
				PlayerMugshotData[PMUG_STATE] = DND_MUGSTATE_IDLE;
				PlayerMugshotData[PMUG_TIC] = DND_MUGIDLE_TIME;
				PlayerMugshotData[PMUG_FRAME] = random(0, 2); // idle has 0 to 2
			}
		}
		
		Delay(const:3);
	}
	SetResultValue(0);
}*/

#endif
