#ifndef DND_INVENTORY_IN
#define DND_INVENTORY_IN

#define MAXINVENTORYBLOCKS_HORIZ 5
#define MAXINVENTORYBLOCKS_VERT 9

#define MAX_INVENTORY_BOXES MAXINVENTORYBLOCKS_HORIZ * MAXINVENTORYBLOCKS_VERT

enum {
	DND_ITEM_CHARM,
	DND_ITEM_HELM,
	DND_ITEM_BOOT,
	DND_ITEM_GLOVE,
	DND_ITEM_NECKLACE,
	DND_ITEM_RING
};

typedef struct it {
	int width;			// width in inventory space
	int height;			// height in inventory space
	int item_type;		// what type of item it is
	int item_index;		// what index it is of the given type
} inventory_T;

#define MAX_INVENTORIES_ON_FIELD 256
inventory_T  Inventories_On_Field[MAX_INVENTORIES_ON_FIELD];
inventory_T PlayerInventory[MAXPLAYERS][MAX_INVENTORY_BOXES];

#endif