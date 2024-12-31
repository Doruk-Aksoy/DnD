#ifndef DND_PROMPT_IN
#define DND_PROMPT_IN

#include "DnD_Common.h"

enum {
	DW_GREET_FIRST_TIME1,
	DW_GREET_FIRST_TIME2,
	DW_GREET_FIRST_TIME3,
	DW_GREET1,
	DW_GREET2,
	DW_GREET3,
	DW_CHALLENGE1,
	DW_CHALLENGE2,
	DW_CHALLENGE3,
	DW_CHALLENGE_DUNGEON
};
#define MAX_PROMPTED_LINES (DW_CHALLENGE_DUNGEON + 1)
#define DW_CHALLENGE_BEGIN DW_CHALLENGE1

enum {
	NPC_NA,
	NPC_OFFER_1,
	NPC_OFFER_2,
	NPC_OFFER_3,
	NPC_OFFER_DUNGEON
};

#define WANDERER_MARK_MIN 5
#define WANDERER_MARK_MAX 50

enum {
	DND_NPC_DARKWANDERER,
};
#define MAX_DND_NPCS (DND_NPC_DARKWANDERER + 1)
#define NPC_APPEAR_CHANCE 0.075 // 7.5%

enum {
	NPC_STATE_IDLE,
	NPC_STATE_VOTE_ONGOING,
	NPC_STATE_VOTE_DECLINE,
	NPC_STATE_VOTE_ACCEPT
};

typedef struct npc_info {
	bool voting;
	int vote_count;
	int offer;
	int dialog;
	int offer_progress;
	int time;
	int n_state;
	int voters[MAXPLAYERS];		// index is player_id, value is what vote they cast, 1 for accept -1 for decline
} npc_info_T;

npc_info_T NPC_States[MAX_DND_NPCS] = { { false, 0, 0, 0, 0, 0, 0 } };

#define DND_VOTE_TIME 30 // seconds

void MarkNPCMet(int npc) {
	SetInventory("DnD_NPC_Meet", CheckInventory("DnD_NPC_Meet") | (1 << (npc + 1)));
}

bool HasMetNPC(int npc) {
	return CheckInventory("DnD_NPC_Meet") & (1 << (npc + 1));
}

void NPC_Setup() {
	int pcount = PlayerInformationInLevel[PLAYERLEVELINFO_COUNTATSTART];

	// check averages -- later on if there are more random npcs added, consider checking which npc to place instead of assuming its dark wanderer
	if(1) {
	//if(PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] / pcount >= GetCVar("dnd_npc_appear_level") && random(0, 1.0) <= NPC_APPEAR_CHANCE && !NPC_States[DND_NPC_DARKWANDERER]) {
		NPC_States[DND_NPC_DARKWANDERER].offer = NPC_OFFER_3;//random(NPC_OFFER_1, NPC_OFFER_3);
		NPC_States[DND_NPC_DARKWANDERER].dialog = random(DW_GREET1, DW_GREET3);
		// find a player and spawn this near them at start
		int tid = 0;
		int i;
		for(i = 0; i < MAXPLAYERS; ++i) {
			tid = i + P_TIDSTART;
			
			// try a radius around this player first -- 4 degrees inc with 180 degree offset
			if(PlayerInGame(i) && isActorAlive(tid) && SpawnAreaTID(tid, 96.0, 4.0, "DarkWanderer", DND_NPC_TID, 0.125, 0)) {
				// this part assumes only a single npc will be available per map
				SetThingSpecial(DND_NPC_TID, ACS_ExecuteAlways, 895, 0, 0, DND_NPC_TID);
				FaceActor(DND_NPC_TID, tid);
				break;
			}
		}
		
		for(i = 0; i < MAXPLAYERS; ++i)
			NPC_States[DND_NPC_DARKWANDERER].voters[i] = 0;
		ACS_NamedExecuteWithResult("DnD NPC Vote Sync", -1);
	}
}

str GetPromptText(int id) {
	return StrParam(s:"DND_PROMPT", d:id + 1);
}

void ClosePrompt() {
	TakeInventory("NPC_Offer_Accepted", 1);
	TakeInventory("NPC_Offer_Declined", 1);
	TakeInventory("ShowingPrompt", 1);
	GiveInventory("NPC_Trigger_Cooldown", 1);
	LocalAmbientSound("RPG/MenuOpen", 127);
	SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
	TakeInventory("P_Frozen", 1);
	ACS_NamedExecuteAlways("DND Menu Cleanup", 0);
}

void ConcludeVoting(int npc_id, int result) {
	NPC_States[npc_id].n_state = result;
	NPC_States[npc_id].voting = false;
	TakeInventory("NPC_Offer_Accepted", 1);
	TakeInventory("NPC_Offer_Declined", 1);
	
	// reset vote list if voting has concluded
	ACS_NamedExecuteWithResult("DnD NPC Vote Sync", -1, -1, npc_id);

	// close prompts of players that had voted
	NPC_States[npc_id].vote_count = 0;
	int curr_activator = ActivatorTID();
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(NPC_States[npc_id].voters[i]) {
			SetActivator(i + P_TIDSTART);
			ACS_NamedExecuteAlways("DnD Close Prompt Delayed", i, npc_id);
		}
	}
	
	// set npc state to go back state
	if(result == NPC_STATE_VOTE_DECLINE)
		SetActorState(DND_NPC_TID, "GoBack", false);
		
	// restore activator
	SetActivator(curr_activator);
}

void ApplyMarkOfChaos(int m_id) {
	int tid = m_id + DND_MONSTERTID_BEGIN;
	
	ACS_NamedExecuteWithResult("DnD Monster Trait Give Single", tid, DND_MARKOFCHAOS);

	// clear all weaknesses
	ACS_NamedExecuteWithResult("DnD Monster Trait Take - TID", tid, DND_ENERGY_WEAKNESS, DND_SILVER_WEAKNESS, DND_FIRE_WEAKNESS);
	ACS_NamedExecuteWithResult("DnD Monster Trait Take - TID", tid, DND_ICE_WEAKNESS, DND_MAGIC_WEAKNESS, DND_ELEMENTAL_WEAKNESS);
	
	// provide the benefits of this trait - doubled health, 50% more damage, 50% faster, extra aggressive, chaos imbued attacks
	int base = MonsterProperties[m_id].maxhp;
	int add = base * 2;
	base = Clamp_Between(base + add, 1, INT_MAX);
	MonsterProperties[m_id].basehp = base;
	MonsterProperties[m_id].maxhp = base;
	
	SetActorProperty(tid, APROP_HEALTH, base);
	SetActorProperty(tid, APROP_SPEED, 3 * GetActorProperty(tid, APROP_SPEED) / 2);
	GiveActorInventory(tid, "MakeAggressive", 1);
	
	GiveActorInventory(tid, "ChaosMark_Script_Run", 1);
}

Script "DnD Chaos Mark Script" (void) {
	ACS_NamedExecuteWithResult("DnD Chaos Mark FX", ActivatorTID());
}

Script "DnD Chaos Mark FX" (int tid) CLIENTSIDE {
	int h = GetActorProperty(tid, APROP_HEIGHT);
	SpawnForced("ChaosMarkFX", GetActorX(tid), GetActorY(tid), GetActorZ(tid) + 16.0, DND_CHAOSMARKFX_TID);
	SetActorProperty(DND_CHAOSMARKFX_TID, APROP_MASTERTID, tid);
	// 127 and 121 are sprite dimensions
	SetActorProperty(DND_CHAOSMARKFX_TID, APROP_SCALEX, (GetActorProperty(tid, APROP_RADIUS) + 16.0) / 51);
	SetActorProperty(DND_CHAOSMARKFX_TID, APROP_SCALEY, (h + 16.0) / 52);
	
	SetActivator(DND_CHAOSMARKFX_TID);
	Thing_ChangeTID(0, 0);
	
	// always face viewer
	int tics = 0;
	bool reverse = false;
	while(isActorAlive(tid)) {
		int ang = AngleToFace(tid, ConsolePlayerNumber() + P_TIDSTART);
		SetActorPosition(0, GetActorX(tid) + 8 * cos(ang), GetActorY(tid) + 8 * sin(ang), GetActorZ(tid) + h / 2, 0);
		if(!reverse)
			SetActorProperty(0, APROP_ALPHA, 1.0 - tics * 0.0125);
		else
			SetActorProperty(0, APROP_ALPHA, 0.625 + tics * 0.0125);
		Delay(const:1);
		++tics;
		if(tics == 31) {
			tics = 0;
			reverse = !reverse;
		}
	}
	SetActorState(0, "Vanish", false);
	SetResultValue(0);
}

void HandleNPC(int npc_id) {
	int mc = 0;
	int temp = 0;
	int count = 0;
	int i, j, k;
	
	static int slots_occupied[64];
	int slot_count = 0;
	
	switch(npc_id) {
		case DND_NPC_DARKWANDERER:
			// dark wanderer's offer must affect the map now
			switch(NPC_States[DND_NPC_DARKWANDERER].offer) {
				case NPC_OFFER_1:
					// kill marked monsters -- min of 5, max of 50 -- scaling with map difficulty
					mc = DnD_TID_Counter[DND_TID_MONSTER];
					temp = random(5 + 5 * MapDifficulty, 10 + 5 * MapDifficulty);

					// we dont care about uniqueness
					for(i = 0; i < temp; ++i) {
						// dont give this mark to elite monsters!
						do {
							j = random(0, mc - 1);
						} while(MonsterProperties[j].isElite);
						
						// give it the mark of chaos
						if(!MonsterProperties[j].trait_list[DND_MARKOFCHAOS]) {
							ApplyMarkOfChaos(j);
							++count;
						}
					}
					
					// we know the unique monster count that received the mark now
					// save this, we'll use it to track progress
					NPC_States[DND_NPC_DARKWANDERER].offer_progress = count;
				break;
				case NPC_OFFER_2:
					// retrieve artifacts -- place the artifacts
					mc = DnD_TID_Counter[DND_TID_PICKUPS] - DND_PICKUPTID_BEGIN;
					temp = random(2 + MapDifficulty, 2 + 2 * MapDifficulty);
					NPC_States[DND_NPC_DARKWANDERER].offer_progress = temp;
					
					// we need to find places to spawn these, prioritize high profile powerups, then health, then radsuits etc.
					if(mc >= temp) {
						// there're enough powerups to place things around
						do {
							k = random(DND_PICKUPTID_BEGIN, mc + DND_PICKUPTID_BEGIN - 1);
							for(j = 0; j < slot_count; ++j) {
								if(slots_occupied[j] == k) {
									j = -1;
									break;
								}
							}
							if(j != -1) {
								slots_occupied[j] = k;
								++slot_count;
								if(Spawn("DarkWanderer_Artifact", GetActorX(k) + random(-20.0, 20.0), GetActorY(k) + random(-20.0, 20.0), GetActorZ(k)))
									--temp;
							}
						} while(temp);
					}
					else if(mc) {
						// we have some powerups, but not enough, so we'll most likely need shared items
						count = DnD_TID_Counter[DND_TID_SHAREDITEMS];
						do {
							// 50% from shared items and the other from powerups
							if(random(0, 1)) {
								k = random(SHARED_ITEM_TID_BEGIN, count - 1);
								i = 8.0;
							}
							else {
								k = random(DND_PICKUPTID_BEGIN, mc + DND_PICKUPTID_BEGIN - 1);
								i = 20.0;
							}
							for(j = 0; j < slot_count; ++j) {
								if(slots_occupied[j] == k) {
									j = -1;
									break;
								}
							}
							if(j != -1) {
								slots_occupied[j] = k;
								++slot_count;
								if(Spawn("DarkWanderer_Artifact", GetActorX(k) + random(-i, i), GetActorY(k) + random(-i, i), GetActorZ(k))) {
									--temp;
									printbold(s:GetActorClass(k), s: " ", f:GetActorX(k), s: " ", f:GetActorY(k));
								}
							}
						} while(temp);
					}
					else {
						// we have no powerups, simply loop through the shared items
						count = DnD_TID_Counter[DND_TID_SHAREDITEMS];
						do {
							k = random(SHARED_ITEM_TID_BEGIN, count - 1);
							for(j = 0; j < slot_count; ++j) {
								if(slots_occupied[j] == k) {
									j = -1;
									break;
								}
							}
							if(j != -1) {
								slots_occupied[j] = k;
								++slot_count;
								if(Spawn("DarkWanderer_Artifact", GetActorX(k) + random(-8.0, 8.0), GetActorY(k) + random(-8.0, 8.0), GetActorZ(k)))
									--temp;
							}
						} while(temp);
					}
				break;
				case NPC_OFFER_3:
					// super powered monster
				break;
			}
		break;
	}
	NPC_States[DND_NPC_DARKWANDERER].time = GetDarkWandererChallengeTime(NPC_States[DND_NPC_DARKWANDERER].offer);
	
	if(NPC_States[DND_NPC_DARKWANDERER].offer != NPC_OFFER_DUNGEON)
		ACS_NamedExecuteAlways("DnD Dark Wanderer Challenge Track", 0);
}

int GetDarkWandererChallengeTime(int offer_id) {
	int x;
	switch(offer_id) {
		case NPC_OFFER_1:
			// kill marked monsters
			x = NPC_States[DND_NPC_DARKWANDERER].offer_progress;
			if(x < 25)
				return x * 30;
			else if(x < 35)
				return x * 25 + 120;
			return x * 20 + 140;
		case NPC_OFFER_2:
			// retrieve artifacts
			return NPC_States[DND_NPC_DARKWANDERER].offer_progress * 60 + MapDifficulty * 30;
		case NPC_OFFER_3:
			// super powered monster
			
		break;
	}
	return 0;
}

str TimeZeroPadding(int t) {
	if(t < 10)
		return "0";
	return "";
}

Script "DnD Dark Wanderer Challenge Track" (void) {
	Delay(const:TICRATE);
	int color = CR_WHITE;
	// players did not all die check at the end
	while(NPC_States[DND_NPC_DARKWANDERER].time && NPC_States[DND_NPC_DARKWANDERER].offer_progress && GetGameModeState() != GAMESTATE_INRESULTSEQUENCE) {
		// global display
		if(NPC_States[DND_NPC_DARKWANDERER].time < 30)
			color = CR_RED;
		else if(NPC_States[DND_NPC_DARKWANDERER].time < 90)
			color = CR_ORANGE;
		else if(NPC_States[DND_NPC_DARKWANDERER].time < 150)
			color = CR_YELLOW;
			
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		SetFont("BIGFONT");
		HudMessageBold(
			s:TimeZeroPadding(NPC_States[DND_NPC_DARKWANDERER].time / 60), d:NPC_States[DND_NPC_DARKWANDERER].time / 60, 
			s:" : ", 
			s:TimeZeroPadding(NPC_States[DND_NPC_DARKWANDERER].time % 60), d:NPC_States[DND_NPC_DARKWANDERER].time % 60;
			HUDMSG_PLAIN,
			RPGMENUBACKGROUNDID + 1,
			color,
			240.4,
			16.0,
			2.0
		);
		
		Delay(const:TICRATE);
		--NPC_States[DND_NPC_DARKWANDERER].time;
	}
	
	if(!NPC_States[DND_NPC_DARKWANDERER].offer_progress && NPC_States[DND_NPC_DARKWANDERER].time) {
		// success
		SetFont("BIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(s:"WELL DONE!"; HUDMSG_FADEOUT, RPGMENUBACKGROUNDID + 1, CR_GREEN, 240.4, 16.0, 1.0, 1.0);
		AmbientSound("DarkWanderer/ChallengeWin", 127);
		// hand out rewards to survivors
		for(int i = 0; i < MAXPLAYERS; ++i) {
			int tid = i + P_TIDSTART;
			if(PlayerInGame(i) && isActorAlive(tid)) {
				
			}
		}
	}
	else {
		// fail, this means either we have progress left or there is no time, but we know theres no time from the loop above so...
		SetFont("BIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(s:"YOU FAILED!"; HUDMSG_FADEOUT, RPGMENUBACKGROUNDID + 1, CR_RED, 240.4, 16.0, 1.0, 1.0);
		AmbientSound("DarkWanderer/ChallengeFail", 127);
	}
	
	Delay(const:TICRATE);
	SetActorState(DND_NPC_TID, "GoBack", false);
}

Script "DnD NPC Artifact Pickup" (void) {
	--NPC_States[DND_NPC_DARKWANDERER].offer_progress;
	NPC_States[DND_NPC_DARKWANDERER].time += 45 + 15 * MapDifficulty;
}

Script "DnD Prompt Dark Wanderer" (int first_time, int offer_id, int n_state) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;
		
	// draw the background once
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	SetFont("THORM1");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID, -1, 400.0, 160.0, 0.0, 0.0);
	SetHudSize(HUDMAX_X_PROMPT, HUDMAX_Y_PROMPT, 1);
	SetFont("PROMPT");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUBACKGROUNDID, -1, 480.0, 320.0, 0.0, 0.0);

	// draw greetings text
	SetFont("SMALLFONT");
	SetHudClipRect(160, 128, 532, 600, 532);
	
	menu_pane_T& CurrentPane = GetPane();
	ResetPane(CurrentPane);
	
	// vote state isnt concluded
	bool voting_ongoing = n_state != NPC_STATE_VOTE_DECLINE && n_state != NPC_STATE_VOTE_ACCEPT;
	int yOff = 384.0;
	
	if(first_time) {
		if(n_state != NPC_STATE_VOTE_ACCEPT) {
			HudMessage(
				l:GetPromptText(DW_GREET_FIRST_TIME1), 
				s:"\n\n", l:GetPromptText(DW_GREET_FIRST_TIME2), 
				s:"\n\n", l:GetPromptText(DW_GREET_FIRST_TIME3),
				s:"\n\n\c[Y5]---------------------------------------\n\n",
				s:"\c[W3]", l:"DND_CHALLENGE", s:":\n\n", l:GetPromptText(DW_CHALLENGE_BEGIN + offer_id - 1);
				HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0
			);
		}
		else {
			HudMessage(
				l:GetPromptText(DW_GREET_FIRST_TIME1), 
				s:"\n\n", l:GetPromptText(DW_GREET_FIRST_TIME2), 
				s:"\n\n", l:GetPromptText(DW_GREET_FIRST_TIME3),
				s:"\n\n\c[Y5]---------------------------------------\n\n",
				s:"\c[W3]", l:"DND_CHALLENGE", s:" - \cd", l:"DND_ACCEPTED", s:"\c[W3]:\n\n", l:GetPromptText(DW_CHALLENGE_BEGIN + offer_id - 1);
				HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0
			);

		}
		if(voting_ongoing) {
			AddBoxToPane_Points(CurrentPane, 340.0, 132.0, 300.0, 124.0);
			AddBoxToPane_Points(CurrentPane, 260.0, 132.0, 220.0, 124.0);
		}
	}
	else if(n_state != NPC_STATE_VOTE_ACCEPT) {
		HudMessage(
			l:GetPromptText(CheckInventory("ReceivedDialogID")),
			s:"\n\n\c[Y5]---------------------------------------\n\n",
			s:"\c[W3]", l:"DND_CHALLENGE", s:":\n\n", l:GetPromptText(DW_CHALLENGE_BEGIN + offer_id - 1);
			HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0
		);
		
		if(voting_ongoing) {
			AddBoxToPane_Points(CurrentPane, 340.0, 196.0, 300.0, 188.0);
			AddBoxToPane_Points(CurrentPane, 260.0, 196.0, 220.0, 188.0);
		}
		
		yOff = 256.0;
	}
	else {
		HudMessage(
			s:"\c[W3]", l:"DND_CHALLENGE", s:" - \cd", l:"DND_ACCEPTED", s:"\c[W3]:\n\n", l:GetPromptText(DW_CHALLENGE_BEGIN + offer_id - 1);
			HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0
		);
		
		yOff = 192.0;
	}

	PlayerCursorData.posx = HUDMAX_XF / 2;
	PlayerCursorData.posy = HUDMAX_YF / 2;

	int boxid = MAINBOX_NONE, boxid_prev = MAINBOX_NONE;
	int i, j, k;
	int pnum = PlayerNumber();
	bool sendInput = false;
	while(CheckInventory("ShowingPrompt") && !CheckInventory("ShowingMenu")) {
		// cursor handling
		PlayerCursorData.posx = GetCursorPos(GetPlayerInput(ConsolePlayerNumber(), INPUT_YAW), MOUSE_INPUT_X);
		PlayerCursorData.posy = GetCursorPos(GetPlayerInput(ConsolePlayerNumber(), INPUT_PITCH), MOUSE_INPUT_Y);
		
		SetHudClipRect(0, 0, 0, 0);
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
		DrawCursor();
		SetHudSize(HUDMAX_X_PROMPT, HUDMAX_Y_PROMPT, 1);
		
		// button id recognition
		boxid = GetTriggeredBoxOnPane(CurrentPane, PlayerCursorData.posx, PlayerCursorData.posy);
		if(boxid != boxid_prev && boxid != MAINBOX_NONE)
			LocalAmbientSound("RPG/MenuMove", 127);
			
		boxid_prev = boxid;
		
		// accept
		if(voting_ongoing) {
			SetFont("SMALLFONT");
			if(CheckInventory("NPC_Offer_Accepted")) {
				HudMessage(l:"DND_ACCEPT"; HUDMSG_PLAIN, RPGMENUITEMID - 3, CR_GREEN, 320.4, yOff, 0.0, 0.0);
				SetFont("TRADBTNC");
			}
			else if(boxid == MBOX_1) {
				HudMessage(s:"\c[M3]", l:"DND_ACCEPT"; HUDMSG_PLAIN, RPGMENUITEMID - 3, CR_WHITE, 320.4, yOff, 0.0, 0.0);
				SetFont("TRADBTNH");
			}
			else {
				HudMessage(s:"\c[Y5]", l:"DND_ACCEPT"; HUDMSG_PLAIN, RPGMENUITEMID - 3, CR_WHITE, 320.4, yOff, 0.0, 0.0);
				SetFont("TRADBTN");
			}
			HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 1, CR_WHITE, 320.4, yOff, 0.0, 0.0);
			
			// decline
			SetFont("SMALLFONT");
			if(CheckInventory("NPC_Offer_Declined")) {
				HudMessage(l:"DND_DECLINE"; HUDMSG_PLAIN, RPGMENUITEMID - 4, CR_GREEN, 480.4, yOff, 0.0, 0.0);
				SetFont("TRADBTNC");
			}
			else if(boxid == MBOX_2) {
				HudMessage(s:"\c[M3]", l:"DND_DECLINE"; HUDMSG_PLAIN, RPGMENUITEMID - 4, CR_WHITE, 480.4, yOff, 0.0, 0.0);
				SetFont("TRADBTNH");
			}
			else {
				HudMessage(s:"\c[Y5]", l:"DND_DECLINE"; HUDMSG_PLAIN, RPGMENUITEMID - 4, CR_WHITE, 480.4, yOff, 0.0, 0.0);
				SetFont("TRADBTN");
			}
			HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 2, CR_WHITE, 480.4, yOff, 0.0, 0.0);
			
			// draw people voting yes and no here
			j = 0;
			k = 0;
			SetFont("SMALLFONT");
			for(i = 0; i < MAXPLAYERS; ++i) {
				if(NPC_States[DND_NPC_DARKWANDERER].voters[i] == 1) {
					HudMessage(n:i + 1; HUDMSG_PLAIN, RPGMENUITEMID - 5 - j - k, CR_UNTRANSLATED, 320.4, yOff + 16.0 * (j + 1), 0.0, 0.0);
					++j;
				}
				else if(NPC_States[DND_NPC_DARKWANDERER].voters[i] == -1) {
					HudMessage(n:i + 1; HUDMSG_PLAIN, RPGMENUITEMID - 5 - j - k, CR_UNTRANSLATED, 480.4, yOff + 16.0 * (k + 1), 0.0, 0.0);
					++j;
				}
			}
		}
		
		// check inputs
		ListenNPCInput();
		sendInput = CheckInventory("MenuInput") != 0;
		if(sendInput) {
			// server gets a few extra info in boxid
			if(!MenuInputData[pnum][DND_MENUINPUT_PAYLOAD])
				MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] = (boxid | MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK]);
			i = PlayerNumber() | (CheckInventory("MenuInput") << 16);
			// guarantee nonzero input
			if(i) {
				//Log(s:"trying to send prev item ", d:MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] >> 16, s: " vs ", d:MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] >> 16);
				NamedRequestScriptPuke("DND Server Box Receive - NPC", i, MenuInputData[pnum][DND_MENUINPUT_PAYLOAD], DND_NPC_DARKWANDERER);
			}
		}
		
		Delay(const:1);
		
		// retry ack
		if(!CheckInventory("DND_ACK")) {
			if(sendInput) {
				GiveInventory("DND_ACKLoop", 1);
				//temp = boxid | (CheckInventory("DnD_PlayerItemIndex") << DND_MENU_ITEMSAVEBITS1) | (CheckInventory("DnD_PlayerPrevItemIndex") << DND_MENU_ITEMSAVEBITS2);
				//Log(s:"trying to send prev item loop beg", d:MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] >> 16, s: " vs ", d:MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] >> 16);
				ACS_NamedExecuteAlways("DnD Retry Sending UntiL ACK - NPC", 0, PlayerNumber() | (CheckInventory("MenuInput") << 16), MenuInputData[pnum][DND_MENUINPUT_PAYLOAD], DND_NPC_DARKWANDERER);
			}
		}
		else {
			sendInput = false;
			SetInventory("MenuInput", 0);
			//Log(s:"reset input data");
			MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] = 0;
			MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = 0;
		}
	}
}

Script "DnD Retry Sending UntiL ACK - NPC" (int payload1, int payload2, int npc_id) CLIENTSIDE {
	if(!payload1 || CheckInventory("DND_ACKLoop"))
		Terminate;
	while(!CheckInventory("DnD_ACK")) {
		//Log(s:"running till ack received with ", d:payload1, s: " ", d:payload2, s: " ", d:mainboxid);
		//Log(s:"trying to send prev item ", d:payload2 >> 16);
		NamedRequestScriptPuke("DND Server Box Receive - NPC", payload1, payload2, npc_id);
		Delay(const:1);
	}
	TakeInventory("DND_ACKLoop", 1);
}

Script "DND Server Box Receive - NPC" (int pnum, int boxid, int npc_id) NET {
	// don't let garbage data slip in
	if(!pnum)
		Terminate;
	int temp = pnum >> 16;
	pnum &= 0xFFFF;
		
	if(!MenuInputData[pnum][DND_MENUINPUT_DELAY]) {
		SetActivator(pnum + P_TIDSTART);
		MenuInputData[pnum][DND_MENUINPUT_DELAY] = DND_MENU_INPUTDELAYTICS;
		MenuInputData[pnum][DND_MENUINPUT] = temp;
		GiveInventory("DND_ACK", 1);
		
		if(temp == DND_MENUINPUT_USEBUTTON) {
			// force close the menu for us
			ClosePrompt();
		}
		else if(boxid != MAINBOX_NONE) {
			if(boxid == MBOX_1) {
				// in case of accept, declare a vote and wait majority or if only player in game, skip this phase
				ACS_NamedExecuteWithResult("DnD NPC Vote Register", 1, npc_id, pnum);
				LocalAmbientSound("RPG/MenuChoose", 127);
				GiveInventory("NPC_Offer_Accepted", 1);
				TakeInventory("NPC_Offer_Declined", 1);
			}
			else if(boxid == MBOX_2) {
				// decline -- mark our vote as a no, wait for voting to conclude optionally
				ACS_NamedExecuteWithResult("DnD NPC Vote Register", -1, npc_id, pnum);
				LocalAmbientSound("RPG/MenuChoose", 127);
				GiveInventory("NPC_Offer_Declined", 1);
				TakeInventory("NPC_Offer_Accepted", 1);
			}
		}
		
		ClearPlayerInput(pnum, true);
		
		Delay(const:DND_MENU_INPUTDELAYTICS);
		MenuInputData[pnum][DND_MENUINPUT_DELAY] = 0;
	}
}

Script "DnD NPC Vote Register" (int vote, int npc_id, int pnum) {
	if(!NPC_States[npc_id].voting) {
		NPC_States[npc_id].voting = true;
		NPC_States[npc_id].n_state = NPC_STATE_VOTE_ONGOING;
		ACS_NamedExecuteWithResult("DnD NPC Voting", npc_id);
	}
	
	// register vote
	NPC_States[npc_id].vote_count += vote;
	NPC_States[npc_id].voters[pnum] = vote;
		
	// check if majority is met -- if our (yes vote count) - (current player count / 2) > 0 that means we can skip further checks, we have majority
	int majority = PlayerCount() / 2 + (PlayerCount() > 1 && (PlayerCount() & 1));
	int curr = NPC_States[npc_id].vote_count - majority;
	int i;
	if(curr > 0) {
		// settle the vote as majority voting yes
		SetFont("BIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(l:"DND_VOTEPASSED", s:"!"; HUDMSG_FADEOUT, RPGMENUBACKGROUNDID + 1, CR_GREEN, 240.4, 16.0, 1.0, 1.0);
		
		ConcludeVoting(npc_id, NPC_STATE_VOTE_ACCEPT);
		
		// handle npc completion
		while(!SpawnedChests)
			Delay(const:10);
		HandleNPC(npc_id);
	}
	else {
		// check if majority actually declined instead of accepted, so we can close voting early
		curr = 0;
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(NPC_States[npc_id].voters[i])
				++curr;
		}
		// conclude voting
		if(curr >= majority) {
			HudMessageBold(s:""; HUDMSG_PLAIN, RPGMENUBACKGROUNDID + 1, CR_WHITE, 240.4, 16.0, 1.0, 0.0);
			SetFont("BIGFONT");
			SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
			HudMessageBold(l:"DND_VOTEFAILED", s:"!"; HUDMSG_FADEOUT, RPGMENUHELPCORNERID, CR_RED, 240.4, 16.0, 1.0, 1.0);
			ConcludeVoting(npc_id, NPC_STATE_VOTE_DECLINE);
		}
		else
			ACS_NamedExecuteWithResult("DnD NPC Vote Sync", pnum, vote, npc_id);
	}
}

Script "DnD Close Prompt Delayed" (int pnum, int npc_id) {
	NPC_States[npc_id].voters[pnum] = 0;
	Delay(const:17);
	ClosePrompt();
}

Script "DnD NPC Voting" (int npc_id) {
	// this is the timer, when this runs out and a majority is not found, we auto decline
	int time = DND_VOTE_TIME * TICRATE;
	while(time > 0 && NPC_States[npc_id].voting) {
		// display the timer to everyone for vote countdown
		SetFont("BIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(d:time / TICRATE, s:" ", l:"DND_TOVOTE", s:"!"; HUDMSG_PLAIN, RPGMENUBACKGROUNDID + 1, CR_WHITE, 240.4, 16.0, 2.0, 0.0);
		
		Delay(const:1);
		--time;
	}
	
	if(NPC_States[npc_id].n_state == NPC_STATE_VOTE_ONGOING) {
		HudMessageBold(s:""; HUDMSG_PLAIN, RPGMENUBACKGROUNDID + 1, CR_WHITE, 240.4, 16.0, 1.0, 0.0);
		SetFont("BIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(l:"DND_VOTEFAILED", s:"!"; HUDMSG_FADEOUT, RPGMENUHELPCORNERID, CR_RED, 240.4, 16.0, 1.0, 1.0);
		
		ConcludeVoting(npc_id, NPC_STATE_VOTE_DECLINE);
	}
}

Script "DnD NPC Vote Sync" (int pnum, int vote, int npc_id) CLIENTSIDE {
	if(pnum == -1) {
		// reset
		NPC_States[npc_id].vote_count = 0;
		for(int i = 0; i < MAXPLAYERS; ++i)
			NPC_States[npc_id].voters[pnum] = 0;
	}
	else {
		// update
		NPC_States[npc_id].vote_count += vote;
		NPC_States[npc_id].voters[pnum] = vote;
	}
	SetResultValue(0);
}

#endif
