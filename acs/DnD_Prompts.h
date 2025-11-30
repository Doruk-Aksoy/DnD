#ifndef DND_PROMPT_IN
#define DND_PROMPT_IN

#include "DnD_Common.h"
#include "DnD_Incursion.h"

#define DND_ARTIFACT_BASETIME 60
#define DND_ARTIFACT_TIME_PER 30

#define DND_SUPERDEMON_HPBUFF 4 // 4 times hp
#define DND_SUPERDEMON_NONBOSSBUFF 2
#define DND_SUPERDEMON_DIFFBONUS 20 // 20%

#define DND_ASMODEUS_RESISTBUFF 33 // 33%

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
	DW_CHALLENGE4,

	MAX_PROMPTED_LINES
};
#define DW_CHALLENGE_BEGIN DW_CHALLENGE1

enum {
	NPC_OFFER_NA,
	NPC_OFFER_SLAYCHAOSMARK,
	NPC_OFFER_COLLECTARTIFACT,
	NPC_OFFER_SUPERDEMON,
	NPC_OFFER_DUNGEON
};

#define WANDERER_MARK_MIN 5
#define WANDERER_MARK_MAX 50

enum {
	DND_NPC_DARKWANDERER,
};
#define MAX_DND_NPCS (DND_NPC_DARKWANDERER + 1)
#define NPC_APPEAR_CHANCE 0.15 // 15%

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
	int aux_data;
	int interaction_count;
	int voters[MAXPLAYERS];		// index is player_id, value is what vote they cast, 1 for accept -1 for decline
} npc_info_T;

npc_info_T NPC_States[MAX_DND_NPCS] = { { false, 0, 0, 0, 0, 0, 0, 0 } };

#define DND_VOTE_TIME 30 // seconds

void MarkNPCMet(int npc) {
	SetInventory("DnD_NPC_Meet", CheckInventory("DnD_NPC_Meet") | (1 << (npc + 1)));
}

bool HasMetNPC(int npc) {
	return CheckInventory("DnD_NPC_Meet") & (1 << (npc + 1));
}

bool CanDarkWandererOfferSuperMonster() {
	return 	MapData[DND_MAPDATA_BARONCOUNT] || MapData[DND_MAPDATA_FATSOCOUNT] || MapData[DND_MAPDATA_ARACHNOCOUNT] || 
			MapData[DND_MAPDATA_ARCHVILECOUNT] || MapData[DND_MAPDATA_SPIDERMASTERMINDCOUNT] || MapData[DND_MAPDATA_CYBERDEMONCOUNT];
}

void NPC_Setup() {
	// dont show npc in lobby maps or unpopulated maps
	if(IsLobbyMap(StrParam(n:PRINTNAME_LEVEL)) || !MapData[DND_MAPDATA_MONSTERTOTAL])
		return;

	int pcount = PlayerInformationInLevel[PLAYERLEVELINFO_COUNTATSTART];
	bool NPCOK = random(0, 1.0) <= NPC_APPEAR_CHANCE;

	// check averages -- later on if there are more random npcs added, consider checking which npc to place instead of assuming its dark wanderer
#ifdef ISDEBUGBUILD
	if(1) {
#else
	if(PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] / pcount >= GetCVar("dnd_npc_appear_level") && NPCOK && NPC_States[DND_NPC_DARKWANDERER].offer == NPC_OFFER_NA) {
#endif
		// check if offers can be OK for this particular map
		NPC_States[DND_NPC_DARKWANDERER].aux_data = 0;
		do {
			NPC_States[DND_NPC_DARKWANDERER].offer = random(NPC_OFFER_SLAYCHAOSMARK, NPC_OFFER_SUPERDEMON);
		} while(NPC_States[DND_NPC_DARKWANDERER].offer == NPC_OFFER_SUPERDEMON && !CanDarkWandererOfferSuperMonster());

		NPC_States[DND_NPC_DARKWANDERER].dialog = random(DW_GREET1, DW_GREET3);
		// find a player and spawn this near them at start
		int tid = 0;
		int i;
		for(i = 0; i < MAXPLAYERS; ++i) {
			tid = i + P_TIDSTART;

			// try a radius around this player first -- 4 degrees inc with 180 degree offset
			if(PlayerInGame(i) && isActorAlive(tid)) {
				// this part assumes only a single npc will be available per map
				ACS_NamedExecuteWithResult("DnD Try Spawn Area", tid, "DarkWanderer", DND_NPC_TID, 96 | (4 << 16) | (22 << 24));
				break;
			}
		}
		
		for(i = 0; i < MAXPLAYERS; ++i)
			NPC_States[DND_NPC_DARKWANDERER].voters[i] = 0;
		ACS_NamedExecuteWithResult("DnD NPC Vote Sync", -1, -1, DND_NPC_DARKWANDERER);
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
	LocalAmbientSound("RPG/MenuClose", 127);
	SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
	TakeInventory("P_Frozen", 1);
	ACS_NamedExecuteAlways("DND Menu Cleanup", 0, PlayerNumber());
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
			ACS_NamedExecuteAlways("DnD Close Prompt Delayed", 0, i, npc_id);
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

void ApplyMarkOfAsmodeus(int m_id, int m_class) {
	int tid = m_id + DND_MONSTERTID_BEGIN;

	// this monster is the one, buff it
	ACS_NamedExecuteWithResult("DnD Monster Trait Give Single", tid, DND_MARKOFASMODEUS);

	// clear all weaknesses
	ACS_NamedExecuteWithResult("DnD Monster Trait Take - TID", tid, DND_ENERGY_WEAKNESS, DND_SILVER_WEAKNESS, DND_FIRE_WEAKNESS);
	ACS_NamedExecuteWithResult("DnD Monster Trait Take - TID", tid, DND_ICE_WEAKNESS, DND_MAGIC_WEAKNESS, DND_ELEMENTAL_WEAKNESS);

	int base = MonsterProperties[m_id].maxhp;
	int add = base * DND_SUPERDEMON_HPBUFF;
	if(m_class != MONSTERCLASS_CYBERDEMON && m_class != MONSTERCLASS_SPIDERMASTERMIND)
		add *= DND_SUPERDEMON_NONBOSSBUFF;

	base = Clamp_Between(base + add, 1, INT_MAX);
	MonsterProperties[m_id].basehp = base;
	MonsterProperties[m_id].maxhp = base;
	SetActorProperty(tid, APROP_HEALTH, base);

	GiveActorInventory(tid, "AsmodeusMark_Script_Run", 1);

	for(int i = 0; i < MAX_DAMAGE_CATEGORIES; ++i)
		MonsterProperties[m_id].resists[i] = Min(MonsterProperties[m_id].resists[i] + DND_ASMODEUS_RESISTBUFF, DND_IMMUNITY_FACTOR);
}

Script "DnD Chaos Mark Script" (void) {
	ACS_NamedExecuteWithResult("DnD Chaos Mark FX", ActivatorTID());
}

Script "DnD Asmodeus Mark Script" (void) {
	ACS_NamedExecuteAlways("DND Spawn Attachment", 0, ActivatorTID(), DND_SPECIALFX_ASMODEUSCIRCLE);
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

bool IsChallengeTrackException() {
	switch(NPC_States[DND_NPC_DARKWANDERER].offer) {
		case NPC_OFFER_SUPERDEMON:
		case NPC_OFFER_DUNGEON:
		return true;
	}
	return false;
}

void HandleNPC(int npc_id) {
	int mc = 0;
	int temp = 0;
	int count = 0;
	int i, j, k;
	
	static int slots_occupied[64];
	for(i = 0; i < 64; ++i)
		slots_occupied[i] = 0;

	int slot_count = 0;
	
	switch(npc_id) {
		case DND_NPC_DARKWANDERER:
			// dark wanderer's offer must affect the map now
			switch(NPC_States[DND_NPC_DARKWANDERER].offer) {
				case NPC_OFFER_SLAYCHAOSMARK:
					// kill marked monsters -- min of 5, max of 50 -- scaling with map difficulty
					mc = DnD_TID_Counter[DND_TID_MONSTER];
					temp = random(5 + 5 * MapData[DND_MAPDATA_DIFFICULTY], 10 + 5 * MapData[DND_MAPDATA_DIFFICULTY]);

					// we dont care about uniqueness
					for(i = 0; i < temp; ++i) {
						// dont give this mark to elite monsters!
						do {
							j = random(0, mc - 1);
							j = UsedMonsterTIDs[j] - DND_MONSTERTID_BEGIN;
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
				case NPC_OFFER_COLLECTARTIFACT:
					// retrieve artifacts -- place the artifacts
					mc = DnD_TID_Counter[DND_TID_PICKUPS];
					temp = random(2 + MapData[DND_MAPDATA_DIFFICULTY], 2 + 2 * MapData[DND_MAPDATA_DIFFICULTY]);
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
								k = random(SHARED_ITEM_TID_BEGIN, SHARED_ITEM_TID_BEGIN + count - 1);
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
									//printbold(s:GetActorClass(k), s: " ", f:GetActorX(k), s: " ", f:GetActorY(k));
								}
							}
						} while(temp);
					}
					else {
						// we have no powerups, simply loop through the shared items
						count = DnD_TID_Counter[DND_TID_SHAREDITEMS];
						do {
							k = random(SHARED_ITEM_TID_BEGIN, SHARED_ITEM_TID_BEGIN + count - 1);
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
				case NPC_OFFER_SUPERDEMON:
					// super powered monster -- find a random monster with matching random id and pick it to be our monster
					for(i = 0; i < 6; ++i)
						slots_occupied[i] = MapData[DND_MAPDATA_BARONCOUNT + i];

					for(mc = 0; mc < DnD_TID_Counter[DND_TID_MONSTER]; ++mc) {
						i = UsedMonsterTIDs[mc] - DND_MONSTERTID_BEGIN;
						temp = MonsterProperties[i].class >= MONSTERCLASS_SPIDERMASTERMIND ? 0.25 : 0; // bias to bosses
						if
						(
							MonsterProperties[i].class >= MONSTERCLASS_BARON && MonsterProperties[i].class <= MONSTERCLASS_CYBERDEMON &&
							random(0, 1.0) <= temp + 1.0 / Max(slots_occupied[MonsterProperties[i].class - MONSTERCLASS_BARON]--, 1)
						)
						{
							ApplyMarkOfAsmodeus(i, MonsterProperties[i].class);
							NPC_States[DND_NPC_DARKWANDERER].offer_progress = 1;
							break;
						}
					}
				break;
				/*case NPC_OFFER_INCURSION:
					// cycle through entire list of monsters, roll rng and see if they are "destined" to drop an incursion marker on death
					temp = GetBaseMarkerSpawnChance(DnD_TID_Counter[DND_TID_MONSTER]);
					for(mc = 0; mc < DnD_TID_Counter[DND_TID_MONSTER]; ++mc) {
						i = UsedMonsterTIDs[mc] - DND_MONSTERTID_BEGIN;
						if(CanMonsterSpawnIncursionMarker(i, temp, mc, DnD_TID_Counter[DND_TID_MONSTER])) {
							MonsterProperties[i].spawnsIncursionMarker = true;
							//Log(s:"Confirmed marker on ", d:i, s:" ", s:GetActorClass(UsedMonsterTIDs[mc]), s:" with bchance: ", f:temp);
						}
					}

					NPC_States[DND_NPC_DARKWANDERER].aux_data = random(DND_INCURSION_BEGIN, DND_INCURSION_END);
				break;*/
			}
		break;
	}

	NPC_States[DND_NPC_DARKWANDERER].time = GetDarkWandererChallengeTime(NPC_States[DND_NPC_DARKWANDERER].offer);
	
	if(!IsChallengeTrackException())
		ACS_NamedExecuteAlways("DnD Dark Wanderer Challenge Track", 0);
}

int GetDarkWandererChallengeTime(int offer_id) {
	int x;
	switch(offer_id) {
		case NPC_OFFER_SLAYCHAOSMARK:
			// kill marked monsters
			x = NPC_States[DND_NPC_DARKWANDERER].offer_progress;
			if(x < 25)
				return x * 30;
			else if(x < 35)
				return x * 25 + 120;
			return x * 20 + 140;
		case NPC_OFFER_COLLECTARTIFACT:
			// retrieve artifacts
			return NPC_States[DND_NPC_DARKWANDERER].offer_progress * DND_ARTIFACT_BASETIME + MapData[DND_MAPDATA_DIFFICULTY] * DND_ARTIFACT_TIME_PER;
		case NPC_OFFER_SUPERDEMON:
			// super powered monster
			switch(NPC_States[DND_NPC_DARKWANDERER].offer_progress) {
				case DND_CYBERDEMONID:
					x = 180;
				break;
				case DND_SPIDERMASTERMINDID:
					x = 150;
				break;
				default:
					x = 120;
				break;
			}
			return x * (100 + MapData[DND_MAPDATA_DIFFICULTY] * DND_SUPERDEMON_DIFFBONUS) / 100;
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
	while(NPC_States[DND_NPC_DARKWANDERER].time && NPC_States[DND_NPC_DARKWANDERER].offer_progress && GetGameModeState() != GAMESTATE_INRESULTSEQUENCE && !IsSetupComplete(SETUP_STATE1, SETUP_PLAYERDATAFINISHED)) {
		// global display
		if(NPC_States[DND_NPC_DARKWANDERER].time < 30)
			color = CR_RED;
		else if(NPC_States[DND_NPC_DARKWANDERER].time < 90)
			color = CR_ORANGE;
		else if(NPC_States[DND_NPC_DARKWANDERER].time < 150)
			color = CR_YELLOW;
			
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		SetFont("DBIGFONT");
		HudMessageBold(
			s:TimeZeroPadding(NPC_States[DND_NPC_DARKWANDERER].time / 60), d:NPC_States[DND_NPC_DARKWANDERER].time / 60, 
			s:" : ", 
			s:TimeZeroPadding(NPC_States[DND_NPC_DARKWANDERER].time % 60), d:NPC_States[DND_NPC_DARKWANDERER].time % 60;
			HUDMSG_PLAIN,
			RPGMENUBACKGROUNDID + 1,
			color,
			480.4,
			32.0,
			2.0
		);
		
		Delay(const:TICRATE);
		--NPC_States[DND_NPC_DARKWANDERER].time;
	}
	
	if(NPC_States[DND_NPC_DARKWANDERER].offer_progress <= 0 && NPC_States[DND_NPC_DARKWANDERER].time) {
		// success
		SetFont("DBIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(l:"DND_WELLDONE"; HUDMSG_FADEOUT, RPGMENUBACKGROUNDID + 1, CR_GREEN, 240.4, 16.0, 1.0, 1.0);
		AmbientSound("DarkWanderer/ChallengeWin", 127);

		str reward_chest = GetDarkWandererReward();

		// hand out rewards to survivors
		for(int i = 0; i < MAXPLAYERS; ++i) {
			int tid = i + P_TIDSTART;
			if(PlayerInGame(i) && isActorAlive(tid)) {
				SpawnDropAtActor(tid, reward_chest, 0, 0, i + 1, 0);
			}
		}
	}
	else {
		// fail, this means either we have progress left or there is no time, but we know theres no time from the loop above so...
		SetFont("DBIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(l:"DND_FAILEDIT"; HUDMSG_FADEOUT, RPGMENUBACKGROUNDID + 1, CR_RED, 240.4, 16.0, 1.0, 1.0);
		AmbientSound("DarkWanderer/ChallengeFail", 127);
	}
	
	Delay(const:TICRATE);

	NPC_States[DND_NPC_DARKWANDERER].offer_progress = 0;
}

str GetDarkWandererReward() {
	switch(NPC_States[DND_NPC_DARKWANDERER].offer) {
		case NPC_OFFER_SLAYCHAOSMARK:
		return "LootChest_ForPlayer_Reverance";
		case NPC_OFFER_COLLECTARTIFACT:
		return "LootChest_ForPlayer_Destiny";
		case NPC_OFFER_SUPERDEMON:
		return "LootChest_ForPlayer_Order";
	}
	return "LootChest_ForPlayer";
}

Script "DnD NPC Artifact Pickup" (void) {
	--NPC_States[DND_NPC_DARKWANDERER].offer_progress;
	NPC_States[DND_NPC_DARKWANDERER].time += DND_ARTIFACT_BASETIME + DND_ARTIFACT_TIME_PER * MapData[DND_MAPDATA_DIFFICULTY];
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
	
	menu_pane_T module& CurrentPane = GetPane();
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

		voting_ongoing = n_state != NPC_STATE_VOTE_DECLINE && n_state != NPC_STATE_VOTE_ACCEPT;
		
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
		
		// users can make only one valid click (accept or decline, no spam)
		if(!CheckInventory("DnD_PromptLocked")) {
			// check inputs
			ListenMouseInput();
			sendInput = CheckInventory("MenuInput") != 0;
			if(sendInput && !CheckInventory("DnD_ClickTicker")) {
				GiveInventory("DnD_ClickTicker", 1);
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
			if(CheckInventory("DND_ACK")) {
				sendInput = false;
				SetInventory("MenuInput", 0);
				//Log(s:"reset input data");
				MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] = 0;
				MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = 0;
			}
		}
		else
			Delay(const:1);
	}
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
				GiveInventory("DnD_PromptLocked", 1);
			}
			else if(boxid == MBOX_2) {
				// decline -- mark our vote as a no, wait for voting to conclude optionally
				ACS_NamedExecuteWithResult("DnD NPC Vote Register", -1, npc_id, pnum);
				LocalAmbientSound("RPG/MenuChoose", 127);
				GiveInventory("NPC_Offer_Declined", 1);
				TakeInventory("NPC_Offer_Accepted", 1);
				GiveInventory("DnD_PromptLocked", 1);
			}
		}
		
		ClearPlayerInput(pnum, true);
		
		Delay(const:DND_MENU_INPUTDELAYTICS);
		MenuInputData[pnum][DND_MENUINPUT_DELAY] = 0;
	}
}

Script "DnD NPC Vote Register" (int vote, int npc_id, int pnum) {
	// if concluded accept no further votes
	if(NPC_States[npc_id].n_state == NPC_STATE_VOTE_ACCEPT || NPC_States[npc_id].n_state == NPC_STATE_VOTE_DECLINE)
		Terminate;

	if(!NPC_States[npc_id].voting) {
		NPC_States[npc_id].voting = true;
		NPC_States[npc_id].n_state = NPC_STATE_VOTE_ONGOING;
		ACS_NamedExecuteWithResult("DnD NPC Voting", npc_id);
	}
	
	// register vote
	if(NPC_States[npc_id].voters[pnum] != vote) {
		NPC_States[npc_id].vote_count += vote;
		NPC_States[npc_id].voters[pnum] = vote;
	}
	else
		Terminate;
		
	// check if majority is met -- if our (yes vote count) - (current player count / 2) > 0 that means we can skip further checks, we have majority
	int majority = PlayerCount() / 2 + (PlayerCount() > 1 && (PlayerCount() & 1));
	int curr = NPC_States[npc_id].vote_count - majority;
	int i;
	if(curr > 0) {
		// settle the vote as majority voting yes
		SetFont("DBIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(l:"DND_VOTEPASSED", s:"!"; HUDMSG_FADEOUT, RPGMENUBACKGROUNDID + 1, CR_GREEN, 240.4, 16.0, 1.0, 1.0);
		
		ConcludeVoting(npc_id, NPC_STATE_VOTE_ACCEPT);
		
		// handle npc completion
		while(!SpawnedChests)
			Delay(const:10);

		// send the npc away, no more interaction
		SetActorState(DND_NPC_TID, "GoBack", false);

		HandleNPC(npc_id);
	}
	else {
		// check if majority actually declined instead of accepted, so we can close voting early
		curr = 0;
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(PlayerInGame(i) && NPC_States[npc_id].voters[i] == -1)
				++curr;
		}

		// conclude voting
		if(curr >= majority) {
			HudMessageBold(s:""; HUDMSG_PLAIN, RPGMENUBACKGROUNDID + 1, CR_WHITE, 240.4, 16.0, 1.0, 0.0);
			SetFont("DBIGFONT");
			SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
			HudMessageBold(l:"DND_VOTEFAILED", s:"!"; HUDMSG_FADEOUT, RPGMENUHELPCORNERID, CR_RED, 240.4, 16.0, 1.0, 1.0);
			ConcludeVoting(npc_id, NPC_STATE_VOTE_DECLINE);
		}
	}

	ACS_NamedExecuteWithResult("DnD NPC Vote Sync", pnum, vote, npc_id);
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
		str col_tag = "\cd";
		int time_to_show = time / TICRATE;
		if(time_to_show < 2 * DND_VOTE_TIME / 3 && time_to_show > DND_VOTE_TIME / 2)
			col_tag = "\ck";
		else if(time_to_show < DND_VOTE_TIME / 3)
			col_tag = "\ci";
		else if(time_to_show < DND_VOTE_TIME / 5)
			col_tag = "\cg";

		SetFont("DBIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(s:col_tag, d:time_to_show, s:"\c- ", l:"DND_TOVOTE", s:"!"; HUDMSG_PLAIN, RPGMENUBACKGROUNDID + 1, CR_WHITE, 240.4, 16.0, 2.0, 0.0);
		
		Delay(const:1);
		--time;
	}
	
	if(NPC_States[npc_id].n_state == NPC_STATE_VOTE_ONGOING) {
		HudMessageBold(s:""; HUDMSG_PLAIN, RPGMENUBACKGROUNDID + 1, CR_WHITE, 240.4, 16.0, 1.0, 0.0);
		SetFont("DBIGFONT");
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

// this => monster's TID
void HandleNPCChallenges(int this, int m_id) {

}

#endif
