#ifndef DND_DUNGEONBASE_IN
#define DND_DUNGEONBASE_IN

#define MAX_DUNGEONKEY_ATTRIB_DEFAULT 3

typedef struct {
	int dungeon_id;									// # of the dungeon map (DND0X etc)
	int quality;									// quality is a multiplier for drop rates
	int level;										// level of drops / monsters
	int attrib_count;
	attr_inf_T attributes[MAX_ITEM_ATTRIBUTES];		// attribute list of the dungeon (% increased elite chance etc. stuff like that)
	str next_map;
} dungeon_data_T;

global dungeon_data_T 38: DungeonInformation;

#endif