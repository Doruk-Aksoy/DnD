#ifndef DND_SHARED_IN
#define DND_SHARED_IN

#include "DnD_Common.h"

#define DND_SHARED_ITEM_SCRIPT 999
#define SHARED_ITEM_ACTIVITY_DELAY (TICRATE * 3)

enum {
	SHI_STIMPACK,
	SHI_MEDKIT,
	SHI_BACKPACK
};
#define MAX_SHARED_ITEM_TYPES (SHI_BACKPACK + 1)

void HandleSharedItemPickupMessage(int id) {
	str header = "";
	str text = "";
	str extra = "";
	switch(id) {
		case SHI_STIMPACK:
			header = "DND_HEALTHPICKUP";
			text = "DND_STIMPACK";
			extra = " x 10%";
		break;
		case SHI_MEDKIT:
			header = "DND_HEALTHPICKUP";
			text = "DND_MEDIKIT";
			extra = " x 25%";
		break;
		case SHI_BACKPACK:
			header = "DND_ITEMPICKUP";
			text = "DND_BACKPACK";
		break;
	}
	
	Log(s:"\cc", l:header, s:": \c[Y5]", l:text, s:extra);
} 

str SP_SharedItems[MAX_SHARED_ITEM_TYPES] = {
	"Stimpack_SP",
	"Medikit_SP",
	"Backpack_SP"
};

// these all can be grouped into a struct to save variable space
int shared_type_items_counter[MAX_SHARED_ITEM_TYPES] = {0,0,0};
bool Shared_Item_pickup_state[MAX_SHARED_ITEMS][MAXPLAYERS];

int limitedrespawn_item_counter = 0;
int LimitedItem_Respawns[LIMITED_RESPAWN_MAX];

// allow max 16 items
int shared_item_queue_count = 0;
int shared_item_clientside_queue[16];

void ResetSharedVariables() {
	int i, j;
	for(i = 0; i < MAX_SHARED_ITEMS; ++i)
		for(j = 0; j < MAXPLAYERS; ++j)
			Shared_Item_pickup_state[i][j] = 0;
				
	for(i = 0; i < LIMITED_RESPAWN_MAX; ++i)
		LimitedItem_Respawns[i] = 0;
}

Script DND_SHARED_ITEM_SCRIPT (int tid) {
	int type = GetActorProperty(tid, APROP_MASS);
	int pnum = PlayerNumber();
	bool pickedup = false;
	
	// temporarily disable this so others can be attempted
	// put here so it's done ASAP
	ACS_NamedExecuteAlways("DnD Disable Pickup", 0, tid);
	
	if(!Shared_Item_pickup_state[tid - SHARED_ITEM_TID_BEGIN][pnum]) {
		switch(type) {
			case SHI_BACKPACK:
				GiveInventory("NewBackpack", 1);
				GiveInventory("BackpackPickMSG", 1);
				pickedup = true;
			break;
			case SHI_STIMPACK:
				//printBold(s:"before stimpack", d:closest);
				if(!CheckInventory("TaltosUp") && ACS_NamedExecuteWithResult("DnD Can Pick Health Item", 0)) {
					//printBold(s:"after stimpack", d:closest);
					ACS_NamedExecuteWithResult("DnD Health Pickup", 10);
					GiveInventory("StimpackMsg", 1);
					pickedup = true;
				}
			break;
			case SHI_MEDKIT:
				if(!CheckInventory("TaltosUp") && ACS_NamedExecuteWithResult("DnD Can Pick Health Item", 0)) {
					ACS_NamedExecuteWithResult("DnD Health Pickup", 25);
					GiveInventory("MedikitMsg", 1);
					pickedup = true;
				}
			break;
		}
		if(pickedup) {
			Shared_Item_pickup_state[tid - SHARED_ITEM_TID_BEGIN][pnum] = 1;
			ACS_NamedExecuteAlways("DnD Shared Item Queue", 0, tid);
		}
	}
}

Script "DND Shared Item Init" (int type) {
	if(GameType() != GAME_SINGLE_PLAYER) {
		// If it goes over limits, just ignore its existance.
		if (DnD_TID_Counter[DND_TID_SHAREDITEMS] < MAX_SHARED_ITEMS) {
			GiveSharedItemTID();
			SetActorProperty(0, APROP_MASS, type);
		}
	}
	else {
		SpawnForced(SP_SharedItems[type], GetActorX(0), GetActorY(0), GetActorZ(0), 0);
		Thing_Remove(0);
	}
}

Script "DnD Shared Item Toggle" ENTER CLIENTSIDE {
	while(1) {
		if(PlayerNumber() == ConsolePlayerNumber()) {
			for(int i = 0; i < shared_item_queue_count; ++i) {
				SetActorProperty(shared_item_clientside_queue[i], APROP_ALPHA, 0.25);
				SetActorState(shared_item_clientside_queue[i], "IdlePicked", 0);
			}
			shared_item_queue_count = 0;
		}
		Delay(5);
	}
}

Script "DnD Shared Item Queue" (int tid) CLIENTSIDE {
	if(PlayerNumber() == ConsolePlayerNumber())
		shared_item_clientside_queue[shared_item_queue_count++] = tid;
}

Script "DND Limited Respawn Item Init" (void) {
	if(!GetCVar("dnd_enable_limiteditems") || GameType() == GAME_SINGLE_PLAYER)
		Terminate;
	if(!ActivatorTID()) {
		Thing_ChangeTID(0, LIMITED_RESPAWN_TID_BEGIN + limitedrespawn_item_counter);
		LimitedItem_Respawns[limitedrespawn_item_counter++] = GetCVar("dnd_limitedrespawn_amount");
	}
	else {
		if(!LimitedItem_Respawns[ActivatorTID() - LIMITED_RESPAWN_TID_BEGIN])
			Thing_Remove(ActivatorTID());
		else
			LimitedItem_Respawns[ActivatorTID() - LIMITED_RESPAWN_TID_BEGIN]--;
	}
}

#endif
