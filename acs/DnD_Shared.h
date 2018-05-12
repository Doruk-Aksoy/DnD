#ifndef DND_SHARED_IN
#define DND_SHARED_IN

#include "DnD_Common.h"

#define SHARED_ITEM_TID_BEGIN 17000
#define SHARED_ITEM_TID_MAX 2048

#define LIMITED_RESPAWN_TID_BEGIN 19049
#define LIMITED_RESPAWN_TID_END 29049
#define LIMITED_RESPAWN_MAX LIMITED_RESPAWN_TID_END - LIMITED_RESPAWN_TID_BEGIN + 1 

enum {
	SHI_STIMPACK,
	SHI_MEDKIT,
	SHI_GREENARMOR,
	SHI_YELLOWARMOR,
	SHI_BLUEARMOR,
	SHI_REDARMOR,
	SHI_BACKPACK,
	SHI_TALENTCAP
};

#define MAX_SHARED_ITEM_TYPES SHI_TALENTCAP + 1
str SharedItemMessage[MAX_SHARED_ITEM_TYPES] = { 
	"\ccHealth pickup : \c[Y5]Stimpack x 10\c-",
	"\ccHealth pickup : \c[Y5]Medikit x 25\c-",
	"\ccArmor pickup  : \c[Y5]Green Armor 33%\c-",
	"\ccArmor pickup  : \c[Y5]Yellow Armor 42%\c-",
	"\ccArmor pickup  : \c[Y5]Blue Armor 50%\c-",
	"\ccArmor Pickup  : \c[Y5]Red Armor 75%\c-",
	"\ccItem Pickup   : \c[Y5]Backpack\c-",
	"\ccBonus pickup  : \c[Y5]Talent Capsule\c-"
};

str SP_SharedItems[MAX_SHARED_ITEM_TYPES] = {
	"Stimpack_SP",
	"Medikit_SP",
	"GreenArmor_SP",
	"YellowArmor_SP",
	"BlueArmor_SP",
	"RedArmor_SP",
	"Backpack_SP",
	"TalentCapsule_SP"
};

struct sif {
	bool semaphore[MAXPLAYERS];
	bool pickup_state[MAXPLAYERS];
};

int shared_item_counter = 0;
int limitedrespawn_item_counter = 0;
struct sif Shared_Item_Info[SHARED_ITEM_TID_MAX];
int LimitedItem_Respawns[LIMITED_RESPAWN_MAX];

// allow max 16 items
int shared_item_queue_count = 0;
int shared_item_clientside_queue[16];

// expects player tid for now
int GetClosestSharedItem(int tid) {
	int closest = -1;
	int mindist = 40.0, curdist = 0, i = 0;
	for(i = 0; i < shared_item_counter; ++i) {
		if(Shared_Item_Info[i].pickup_state[tid - P_TIDSTART])
			continue;
		curdist = fdistance(tid, SHARED_ITEM_TID_BEGIN + i);
		if(curdist <= mindist) {
			mindist = curdist;
			closest = i;
		}
	}
	// if this is picked, search again for another candidate on the same dist (handles corner case: stacked item)
	/*if(Shared_Item_Info[closest].pickup_state[tid - P_TIDSTART]) {
		closest = -1;
		for(i = 0; i < shared_item_counter; ++i) {
			if(Shared_Item_Info[i].pickup_state[tid - P_TIDSTART])
				continue;
			curdist = fdistance(tid, SHARED_ITEM_TID_BEGIN + i);
			if(curdist == mindist) {
				closest = i;
				break;
			}
		}
	}*/
	return closest;
}

#endif