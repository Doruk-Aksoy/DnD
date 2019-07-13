#ifndef DND_SHARED_IN
#define DND_SHARED_IN

#include "DnD_Common.h"

#define DND_SHARED_ITEM_SCRIPT 999
#define SHARED_ITEM_ACTIVITY_DELAY (TICRATE * 3)
#define SHARED_ITEM_TID_BEGIN 17000
#define SHARED_ITEM_TYPE_TIDS_MAX 2048

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
	SHI_TALENTCAP
};

#define MAX_SHARED_ITEM_TYPES (SHI_TALENTCAP + 1)
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

int shared_item_counter = 0;
int shared_type_items_counter[MAX_SHARED_ITEM_TYPES] = {0,0,0,0,0,0,0,0};
bool Shared_Item_pickup_state[MAX_SHARED_ITEM_TYPES][SHARED_ITEM_TYPE_TIDS_MAX][MAXPLAYERS];

int limitedrespawn_item_counter = 0;
int LimitedItem_Respawns[LIMITED_RESPAWN_MAX];

// allow max 16 items
int shared_item_queue_count = 0;
int shared_item_clientside_queue[16];

#endif