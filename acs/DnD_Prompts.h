#ifndef DND_PROMPT_IN
#define DND_PROMPT_IN

#define DND_NPC_TID 54165

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
};

enum {
	DND_NPC_DARKWANDERER,
};
#define MAX_DND_NPCS (DND_NPC_DARKWANDERER + 1)
#define NPC_APPEAR_CHANCE 0.02

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
	int n_state;
	int voters[MAXPLAYERS];		// index is player_id, value is what vote they cast, 1 for accept -1 for decline
} npc_info_T;

npc_info_T NPC_States[MAX_DND_NPCS] = { { false, 0, 0, 0 } };

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
	if(1){
	//if(PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] / pcount >= GetCVar("dnd_npc_appear_level") && random(0, 1.0) <= NPC_APPEAR_CHANCE && !NPC_States[DND_NPC_DARKWANDERER]) {
		NPC_States[DND_NPC_DARKWANDERER].offer = random(NPC_OFFER_1, NPC_OFFER_3);
		// find a player and spawn this near them at start
		int tid = 0;
		int i;
		for(i = 0; i < MAXPLAYERS; ++i) {
			tid = i + P_TIDSTART;
			
			// try a radius around this player first -- 4 degrees inc with 180 degree offset
			if(PlayerInGame(i) && isActorAlive(tid) && SpawnAreaTID(tid, 96.0, 4.0, "DarkWanderer", DND_NPC_TID, 0.5, 0)) {
				// this part assumes only a single npc will be available per map
				SetThingSpecial(DND_NPC_TID, ACS_ExecuteAlways, 895, 0, 0, DND_NPC_TID);
				break;
			}
		}
		
		for(i = 0; i < MAXPLAYERS; ++i)
			NPC_States[DND_NPC_DARKWANDERER].voters[i] = 0;
		ACS_NamedExecuteWithResult("DnD NPC Vote Sync", -1);
	}
}

str PromptLines[MAX_PROMPTED_LINES] = {
	"    Greetings warrior! You have many questions, yes? Amongst them, who I am or why I'm not trying to destroy you probably take precedence.",
	"    Many can't see me, those that do see me only briefly. You, however, are different. I sense great power in you. I travel distant worlds, places unimaginable, trying to find the most interesting challenger.",
	"    The talents you have put to display here, show promise and entice me. Henceforth, I'll make offers to you anytime I'm visible to you. The offers I make require a sacrifice. Sometimes in blood, sometimes in other ways. The choice is left to the powers which I serve.",
	"    We meet once again, great warrior. Tell me, are you ready for another challenge?",
	"    Random hand of fate guides me to you, yet again. This time, it may come to pass that you lose.",
	"    My masters believe you are ready for another sacrifice. Let's see if you can live up to the challenge.",
	"Several monsters have spawned that bear the mark of my masters. Slay them, and return to me. Failure is unacceptable.",
	"Objects of immense power have been deployed randomly across the land. Their return shall bring great wealth and power to you.",
	"A being of untold power has been granted further cosmic powers. Destroy it, and a fraction of its powers will be yours.",
	"You have surpassed expectations. And so, your final challenge will be me. My masters are intrigued at this opportunity. What say you, great warrior? Are you ready?"
};

void ClosePrompt() {
	TakeInventory("ShowingPrompt", 1);
	LocalAmbientSound("RPG/MenuOpen", 127);
	SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
	TakeInventory("P_Frozen", 1);
	ACS_NamedExecuteAlways("DND Menu Cleanup", 0);
}

#endif
