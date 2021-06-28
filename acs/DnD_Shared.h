#ifndef DND_SHARED_IN
#define DND_SHARED_IN

#include "DnD_Common.h"

#define DND_SHARED_ITEM_SCRIPT 999
#define DND_SHARED_ITEM_MAXCOUNT (SHARED_ITEM_TYPE_TID_MAX - SHARED_ITEM_TID_BEGIN)
#define SHARED_ITEM_ACTIVITY_DELAY (TICRATE * 3)

#define LIMITED_RESPAWN_TID_BEGIN 19049
#define LIMITED_RESPAWN_TID_END 29049
#define LIMITED_RESPAWN_MAX (LIMITED_RESPAWN_TID_END - LIMITED_RESPAWN_TID_BEGIN + 1)

enum {
	SHI_STIMPACK,
	SHI_MEDKIT,
	SHI_GREENARMOR,
	SHI_YELLOWARMOR,
	SHI_BLUEARMOR,
	SHI_REDARMOR,
	SHI_BACKPACK,
	SHI_SYNTHMETALARMOR
};

#define MAX_SHARED_ITEM_TYPES (SHI_SYNTHMETALARMOR + 1)
str SharedItemMessage[MAX_SHARED_ITEM_TYPES] = { 
	"\ccHealth pickup : \c[Y5]Stimpack x 10\c-",
	"\ccHealth pickup : \c[Y5]Medikit x 25\c-",
	"\ccArmor pickup  : \c[Y5]Green Armor 33%\c-",
	"\ccArmor pickup  : \c[Y5]Yellow Armor 42%\c-",
	"\ccArmor pickup  : \c[Y5]Blue Armor 50%\c-",
	"\ccArmor Pickup  : \c[Y5]Red Armor 75%\c-",
	"\ccItem Pickup   : \c[Y5]Backpack\c-",
	"\ccArmor Pickup  : \c[Y5]Synth-metal Armor 65%\c-"
};

str SP_SharedItems[MAX_SHARED_ITEM_TYPES] = {
	"Stimpack_SP",
	"Medikit_SP",
	"GreenArmor_SP",
	"YellowArmor_SP",
	"BlueArmor_SP",
	"RedArmor_SP",
	"Backpack_SP",
	"Synthmetal_SP"
};

// these all can be grouped into a struct to save variable space
int shared_type_items_counter[MAX_SHARED_ITEM_TYPES] = {0,0,0,0,0,0,0,0};
bool Shared_Item_pickup_state[DND_SHARED_ITEM_MAXCOUNT][MAXPLAYERS];

int limitedrespawn_item_counter = 0;
int LimitedItem_Respawns[LIMITED_RESPAWN_MAX];

// allow max 16 items
int shared_item_queue_count = 0;
int shared_item_clientside_queue[16];

void ResetSharedVariables() {
	int i, j;
	for(i = 0; i < DND_SHARED_ITEM_MAXCOUNT; ++i)
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
			case SHI_REDARMOR:
				if(ACS_NamedExecuteWithResult("DND Armor Pickup Check", 300, DND_ARMOR_RED)) {
					ACS_NamedExecuteWithResult("DND Armor Give", DND_ARMOR_RED, 300);
					GiveInventory("RedArmorMsg", 1);
					pickedup = true;
				}
			break;
			case SHI_BLUEARMOR:
				if(ACS_NamedExecuteWithResult("DND Armor Pickup Check", 200, DND_ARMOR_BLUE)) {
					ACS_NamedExecuteWithResult("DND Armor Give", DND_ARMOR_BLUE, 200);
					GiveInventory("BlueArmorMsg", 1);
					pickedup = true;
				}
			break;
			case SHI_YELLOWARMOR:
				if(ACS_NamedExecuteWithResult("DND Armor Pickup Check", 150, DND_ARMOR_YELLOW)) {
					ACS_NamedExecuteWithResult("DND Armor Give", DND_ARMOR_YELLOW, 150);
					GiveInventory("YellowArmorMsg", 1);
					pickedup = true;
				}
			break;
			case SHI_GREENARMOR:
				if(ACS_NamedExecuteWithResult("DND Armor Pickup Check", 100, DND_ARMOR_GREEN)) {
					ACS_NamedExecuteWithResult("DND Armor Give", DND_ARMOR_GREEN, 100);
					GiveInventory("GreenArmorMsg", 1);
					pickedup = true;
				}
			break;
			case SHI_SYNTHMETALARMOR:
				if(ACS_NamedExecuteWithResult("DND Armor Pickup Check", 400, DND_ARMOR_SYNTHMETAL)) {
					ACS_NamedExecuteWithResult("DND Armor Give", DND_ARMOR_SYNTHMETAL, 400);
					GiveInventory("SynthmetalArmorMsg", 1);
					ACS_NamedExecuteAlways("DnD Give Research - Regular", 0, RES_SYNTHMETALARMOR, 1);
					pickedup = true;
				}
			break;
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
		if (DnD_TID_List[DND_TID_SHAREDITEMS] < SHARED_ITEM_TYPE_TID_MAX) {
			Thing_ChangeTID(0, DnD_TID_List[DND_TID_SHAREDITEMS]++);
			SetActorProperty(0, APROP_MASS, type);
		}
	}
	else {
		Spawn(SP_SharedItems[type], GetActorX(0), GetActorY(0), GetActorZ(0), 0);
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