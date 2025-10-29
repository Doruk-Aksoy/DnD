#ifndef DND_INVENTORY_IN
#define DND_INVENTORY_IN

#include "DnD_InvInfo.h"
#include "DnD_UniqueItems.h"
#include "DnD_Hud.h"
#include "DnD_WeaponDefs.h"

#define DND_ITEMMOD_ADD FALSE
#define DND_ITEMMOD_REMOVE TRUE

#define MAX_ITEM_LEVEL 100

#define INVENTORY_INFO_ALPHA 0.666
#define INVENTORY_HOLDTIME 0.5
#define INVENTORY_FADETIME 0.5

#define DND_BASE_DISASSEMBLE_COST 350
#define DND_DISASSEMBLE_LEVEL_PERCENT 5
#define DND_DISASSEMBLE_IMPLICIT_PERCENT 10
#define DND_DISASSEMBLE_TIER_PERCENT 10
#define DND_DISASSEMBLE_FRACTURE_PERCENT 10 // 10% per fracture
#define DND_BASE_DISASSEMBLE_CHANCE 25 // 25%
#define DND_BASE_DISASSEMBLE_CHANCE_PERLUCK 5 // 5%
#define DND_BASE_FRACTURE_DISASSEMBLE_CHANCE 3
#define DND_BASE_CORRUPT_DISASSEMBLE_CHANCE 5
#define DND_BASE_ILVL_YIELD 10
#define DND_BASE_AVGMOD_YIELD 30
#define DND_BASE_FRACTURE_YIELD 50
#define DND_BASE_CORRUPT_YIELD 100

#define MAX_EXTRA_INVENTORY_PAGES 10

#define MAX_POWERCORE_ATTRIB_DEFAULT 2

#define MAXSTACKS_ORB 512
#define MAXSTACKS_CKEY 128
#define MAXSTACKS_TOKEN 128

#define HUD_DII_FIELD_MULT 10

#define MAX_EXPRESIST_VAL 100
#define MAX_ELEMRESIST_VAL 100
#define MAX_PHYSRESIST_VAL 100
#define MAX_REFLRESIST_VAL 1000 // 1000 because we use 1 as 0.1% so 1000 is 100%
#define MAX_WEAKEN_VAL 200

#define MAX_SMALL_CHARMS_USED 4
#define MAX_MEDIUM_CHARMS_USED 2
#define MAX_LARGE_CHARMS_USED 1
#define MAX_ARMORS_USED 3 // BOOT BODY HELM
#define MAX_POWERCORES_USED 1
#define MAX_ITEMS_EQUIPPABLE (MAX_SMALL_CHARMS_USED + MAX_MEDIUM_CHARMS_USED + MAX_LARGE_CHARMS_USED + MAX_ARMORS_USED + MAX_POWERCORES_USED)

enum {
	SMALLCHARM_INDEX1,
	SMALLCHARM_INDEX2,
	SMALLCHARM_INDEX3,
	SMALLCHARM_INDEX4,
	MEDIUMCHARM_INDEX1,
	MEDIUMCHARM_INDEX2,
	LARGECHARM_INDEX,
	HELM_INDEX,
	BODY_ARMOR_INDEX,
	POWERCORE_INDEX,
	BOOT_INDEX
};

enum {
	IPROCESS_ADD,
	IPROCESS_REMOVE
};

typedef struct imove {
	int width;
	int height;
	int dest_pos;
	bool state;
} imove_T;

imove_T ItemMoveList[MAXPLAYERS][MAX_INVENTORY_BOXES];

enum {
	IIMG_SC_1,
	IIMG_SC_2,
	IIMG_SC_3,
	
	IIMG_MC_1 = 20,
	IIMG_MC_2,
	IIMG_MC_3,
	
	IIMG_LC_1 = 40,
	IIMG_LC_2,
	IIMG_LC_3,
	
	// unique charm images
	IIMG_UCHRM_1 = 60,
	IIMG_UCHRM_2,
	IIMG_UCHRM_3,
	IIMG_UCHRM_4,
	IIMG_UCHRM_5,
	IIMG_UCHRM_6,
	IIMG_UCHRM_7,
	IIMG_UCHRM_8,
	IIMG_UCHRM_9,
	IIMG_UCHRM_10,
	IIMG_UCHRM_11,
	IIMG_UCHRM_12,
	IIMG_UCHRM_13,
	IIMG_UCHRM_14,
	IIMG_UCHRM_15,
	IIMG_UCHRM_16,
	IIMG_UCHRM_17,
	IIMG_UCHRM_18,
	IIMG_UCHRM_19,
	IIMG_UCHRM_20,
	IIMG_UCHRM_21,

	// drop only charms
	IIMG_UDCHRM_1 = 400,
	IIMG_UDCHRM_2,
	IIMG_UDCHRM_3,
	IIMG_UDCHRM_4,
	IIMG_UDCHRM_5,

	IIMG_UCORE_1 = 600,
	IIMG_UCORE_R2,
	IIMG_UCORE_R3,
	IIMG_UCORE_R4,
	IIMG_UCORE_R5,
	IIMG_UCORE_R6,

	IIMG_UBODY_1 = 700,
	IIMG_UBODY_2,
	IIMG_UBODY_3,
	IIMG_UBODY_R4,
	IIMG_UBODY_R5,
	IIMG_UBODY_R6,
	IIMG_UBODY_R7,
	IIMG_UBODY_R8,
	IIMG_UBODY_R9,
	IIMG_UBODY_R10,
	IIMG_UBODY_R11,
	IIMG_UBODY_R12,
	IIMG_UBODY_R13,
	IIMG_UBODY_R14,
	IIMG_UBODY_R15,
	IIMG_UBODY_R16,

	IIMG_UBOOT_R1 = 1000,
	IIMG_UBOOT_R2,
	IIMG_UBOOT_R3,
	IIMG_UBOOT_R4,
	IIMG_UBOOT_R5,
	IIMG_UBOOT_R6,
	IIMG_UBOOT_R7,
	IIMG_UBOOT_R8,
	IIMG_UBOOT_R9,
	IIMG_UBOOT_R10,
	IIMG_UBOOT_R11,
	IIMG_UBOOT_R12,
	IIMG_UBOOT_R13,
	IIMG_UBOOT_R14,
	IIMG_UBOOT_R15,
	IIMG_UBOOT_R16,

	IIMG_UHELM_R1 = 1300,
	IIMG_UHELM_R2,
	IIMG_UHELM_R3,
	IIMG_UHELM_R4,
	IIMG_UHELM_R5,
	IIMG_UHELM_R6,
	IIMG_UHELM_R7,
	IIMG_UHELM_R8,
	IIMG_UHELM_R9,
	IIMG_UHELM_R10,
	IIMG_UHELM_R11,
	IIMG_UHELM_R12,
	IIMG_UHELM_R13,
	IIMG_UHELM_R14,
	IIMG_UHELM_R15,
	IIMG_UHELM_R16,
	
	IIMG_ORB_1 = 1500,
	IIMG_ORB_2,
	IIMG_ORB_3,
	IIMG_ORB_4,
	IIMG_ORB_5,
	IIMG_ORB_6,
	IIMG_ORB_7,
	IIMG_ORB_8,
	IIMG_ORB_9,
	IIMG_ORB_10,
	IIMG_ORB_11,
	IIMG_ORB_12,
	IIMG_ORB_13,
	IIMG_ORB_14,
	IIMG_ORB_15,
	IIMG_ORB_16,
	IIMG_ORB_17,
	IIMG_ORB_18,
	IIMG_ORB_19,
	IIMG_ORB_20,
	IIMG_ORB_21,
	IIMG_ORB_22,
	IIMG_ORB_23,
	IIMG_ORB_24,
	IIMG_ORB_25,
	IIMG_ORB_26,
	IIMG_ORB_27,
	IIMG_ORB_28,
	IIMG_ORB_29,
	IIMG_ORB_30,
	IIMG_ORB_31,

	// monster specific orb drops
	IIMG_MORB_1 = 1640,
	IIMG_MORB_2,
	IIMG_MORB_3,
	IIMG_MORB_4,
	IIMG_MORB_5,
	IIMG_MORB_6,

	// armor
	IIMG_ARM_1 = 1800,
	IIMG_ARM_2,
	IIMG_ARM_3,
	IIMG_ARM_4,

	IIMG_ARM_5,
	IIMG_ARM_6,
	IIMG_ARM_7,
	IIMG_ARM_8,
	IIMG_ARM_9,

	IIMG_ARM_10,
	IIMG_ARM_11,
	IIMG_ARM_12,
	IIMG_ARM_13,
	IIMG_ARM_14,
	IIMG_ARM_15,
	IIMG_ARM_16,
	
	IIMG_ARM_17,
	IIMG_ARM_18,

	// boots
	IIMG_BOO_1 = 2000,
	IIMG_BOO_2,
	IIMG_BOO_3,
	IIMG_BOO_4,
	IIMG_BOO_5,
	IIMG_BOO_6,
	IIMG_BOO_7,
	IIMG_BOO_8,
	IIMG_BOO_9,
	IIMG_BOO_10,
	IIMG_BOO_11,
	
	// helm
	IIMG_HLM_1 = 2200,
	IIMG_HLM_2,
	IIMG_HLM_3,
	IIMG_HLM_4,
	IIMG_HLM_5,
	IIMG_HLM_6,
	IIMG_HLM_7,
	IIMG_HLM_8,

	// powercores
	IIMG_CORE_1 = 2400,
	IIMG_CORE_2,
	IIMG_CORE_3,
	IIMG_CORE_4,
	
	IIMG_CKEY_1 = 2600,
	IIMG_CKEY_2,
	IIMG_CKEY_3,
	
	IIMG_TOKEN_ARMORER = 2800,
	IIMG_TOKEN_GUNSMITH
};
#define MAX_ITEM_IMAGES (IIMG_TOKEN_GUNSMITH + 1)

enum {
	DND_TOKEN_ARMORER,
	DND_TOKEN_GUNSMITH
};
#define DND_MAX_TOKEN_KINDS (DND_TOKEN_GUNSMITH + 1)

// first bunch are orbs, the next are tokens
#define MAX_UNIQUE_CRAFTING_TYPES (DND_MAX_ORB_KINDS + DND_MAX_TOKEN_KINDS)
int UniqueCraftingItemList[MAX_UNIQUE_CRAFTING_TYPES];

void ResetUniqueCraftingItemList() {
	for(int i = 0; i < MAX_UNIQUE_CRAFTING_TYPES; ++i)
		UniqueCraftingItemList[i] = -1;
}

#define DND_SMALLCHARM_IMAGEBEGIN IIMG_SC_1
#define DND_SMALLCHARM_IMAGEEND IIMG_SC_3
#define DND_MEDIUMCHARM_IMAGEBEGIN IIMG_MC_1
#define DND_MEDIUMCHARM_IMAGEEND IIMG_MC_3
#define DND_LARGECHARM_IMAGEBEGIN IIMG_LC_1
#define DND_LARGECHARM_IMAGEEND IIMG_LC_3

#define ITEM_IMAGE_ORB_BEGIN IIMG_ORB_1
#define ITEM_IMAGE_KEY_BEGIN IIMG_CKEY_1
#define ITEM_IMAGE_TOKEN_BEGIN IIMG_TOKEN_ARMORER

#define ITEM_IMAGE_ARMOR_BEGIN IIMG_ARM_1
#define ITEM_IMAGE_ARMOR_END IIMG_ARM_18

#define ITEM_IMAGE_BOOT_BEGIN IIMG_BOO_1
#define ITEM_IMAGE_BOOT_END IIMG_BOO_11

#define ITEM_IMAGE_HELM_BEGIN IIMG_HLM_1
#define ITEM_IMAGE_HELM_END IIMG_HLM_8

#define ITEM_IMAGE_MONSTERORB_BEGIN IIMG_MORB_1
#define ITEM_IMAGE_POWERCORE_BEGIN IIMG_CORE_1

#define ITEM_IMAGE_CHARM_END IIMG_LC_3
#define ITEM_IMAGE_ORB_END IIMG_ORB_31
#define ITEM_IMAGE_MONSTERORB_END IIMG_MORB_6
#define ITEM_IMAGE_POWERCORE_END IIMG_CORE_4
#define ITEM_IMAGE_KEY_END IIMG_CKEY_3
#define ITEM_IMAGE_TOKEN_END IIMG_TOKEN_GUNSMITH

// uniques
#define ITEM_IMAGE_UCHARM_BEGIN IIMG_UCHRM_1
#define ITEM_IMAGE_UCHARM_END IIMG_UCHRM_21

#define ITEM_IMAGE_DROPONLY_UCHARM_BEGIN IIMG_UDCHRM_1
#define ITEM_IMAGE_DROPONLY_UCHARM_END IIMG_UDCHRM_5

#define ITEM_IMAGE_UCORE_BEGIN IIMG_UCORE_1
#define ITEM_IMAGE_UCORE_END IIMG_UCORE_1

#define ITEM_IMAGE_UBODYARM_BEGIN IIMG_UBODY_1
#define ITEM_IMAGE_UBODYARM_END IIMG_UBODY_3

#include "DnD_Armor.h"
#include "DnD_Powercore.h"
#include "DnD_InvGeneric.h"

// wide returns wider version
str GetItemImage(int id, bool wide = false) {
	str img_prefix = "";
	int suffix = 0;
	if(id <= ITEM_IMAGE_CHARM_END) {
		if(id >= DND_LARGECHARM_IMAGEBEGIN) {
			suffix = id - DND_LARGECHARM_IMAGEBEGIN + 1;
			img_prefix = "CG";
		}
		else if(id >= DND_MEDIUMCHARM_IMAGEBEGIN) {
			suffix = id - DND_MEDIUMCHARM_IMAGEBEGIN + 1;
			img_prefix = "CM";
		}
		else {
			suffix = id + 1;
			img_prefix = "CS";
		}
	}
	else if(id <= ITEM_IMAGE_UCHARM_END) {
		img_prefix = "UC";
		suffix = id - ITEM_IMAGE_UCHARM_BEGIN + 1;
	}
	else if(id <= ITEM_IMAGE_DROPONLY_UCHARM_END) {
		img_prefix = "UDC";
		suffix = id - ITEM_IMAGE_DROPONLY_UCHARM_BEGIN + 1;
	}
	else if(id <= ITEM_IMAGE_UCORE_END) {
		img_prefix = "UCOR";
		suffix = id - ITEM_IMAGE_UCORE_BEGIN + 1;
	}
	else if(id <= ITEM_IMAGE_UBODYARM_END) {
		img_prefix = "UARM";
		suffix = id - ITEM_IMAGE_UBODYARM_BEGIN + 1;
	}
	else if(id <= ITEM_IMAGE_ORB_END) {
		img_prefix = "O";
		suffix = id - ITEM_IMAGE_ORB_BEGIN + 1;
	}
	else if(id <= ITEM_IMAGE_MONSTERORB_END) {
		img_prefix = "MO";
		suffix = id - ITEM_IMAGE_MONSTERORB_BEGIN + 1;
	}
	else if(id <= ITEM_IMAGE_ARMOR_END) {
		img_prefix = "AR";
		suffix = id - ITEM_IMAGE_ARMOR_BEGIN + 1;
		if(wide)
			img_prefix = "ARW";
	}
	else if(id <= ITEM_IMAGE_BOOT_END)  {
		img_prefix = "BO";
		suffix = id - ITEM_IMAGE_BOOT_BEGIN + 1;
	}
	else if(id <= ITEM_IMAGE_HELM_END) {
		img_prefix = "HL";
		suffix = id - ITEM_IMAGE_HELM_BEGIN + 1;
	}
	else if(id <= ITEM_IMAGE_POWERCORE_END) {
		img_prefix = "PC";
		suffix = id - ITEM_IMAGE_POWERCORE_BEGIN + 1;
	}
	else if(id <= ITEM_IMAGE_KEY_END) {
		img_prefix = "K";
		suffix = id - ITEM_IMAGE_KEY_BEGIN + 1;
	}
	else {
		img_prefix = "T";
		suffix = id - ITEM_IMAGE_TOKEN_BEGIN + 1;
	}
	//Log(l:StrParam(d:id, s:" ==>", s:"DND_", s:img_prefix, s:"IMG", d:suffix));
	return StrParam(l:StrParam(s:"DND_", s:img_prefix, s:"IMG", d:suffix));
}

#define ITEMLEVEL_VARIANCE_LOWER 15
#define ITEMLEVEL_VARIANCE_HIGHER 7

// holds inventories of all players
global inventory_T 11: PlayerInventoryList[MAXPLAYERS][MAX_INVENTORY_BOXES];

#define MAX_TEMP_INVENTORIES 4
global inventory_T 23: TemporaryInventoryList[MAXPLAYERS][MAX_TEMP_INVENTORIES];

enum {
	PTR_FREEITEMWORLD
};
#define MAX_POINTERS (PTR_FREEITEMWORLD + 1)

global int 24: PointerIndexTable[MAX_POINTERS];

#define MAX_INVENTORIES_ON_FIELD 8192
// holds indexes to items used that are on players like charms or armors
global inventory_T 12: Items_Used[MAXPLAYERS][MAX_ITEMS_EQUIPPABLE];
global inventory_T 13: Inventories_On_Field[MAX_INVENTORIES_ON_FIELD];
global inventory_T 14: TradeViewList[MAXPLAYERS][MAX_INVENTORY_BOXES];
global inventory_T 15: PlayerStashList[MAXPLAYERS][MAX_EXTRA_INVENTORY_PAGES][MAX_INVENTORY_BOXES];

#define INVSOURCE_PLAYER PlayerInventoryList
#define INVSOURCE_ITEMSUSED Items_Used

// Creates an item on the game field
int CreateItemSpot() {
	//Naive but very fast: Will just replace old item if index goes back to it - have a bigger array to prevent the negative effect.
	//Just having a loop here creates an error so avoid looping at all costs.
	//Remember, the floor gets cleared on a new map, so most likely the older items are useless for the players anyways, except on 4k mob slaugher maps.
	// Note: This table for free items index must be zero'd at the end of every map... otherwise we will try to give index to something already on field next map...
	//Log(s:"curr index: ", d:PointerIndexTable[PTR_FREEITEMWORLD]);
	if ((++PointerIndexTable[PTR_FREEITEMWORLD]) >= MAX_INVENTORIES_ON_FIELD)
		PointerIndexTable[PTR_FREEITEMWORLD] = 0;
		
	// clear properties of this item before creating it -- fixes garbage data leftovers
	//Log(s:"clear id ", d:PointerIndexTable[PTR_FREEITEMWORLD]);
	RemoveItemFromWorld(PointerIndexTable[PTR_FREEITEMWORLD]);
	//Log(s:"use id ", d:PointerIndexTable[PTR_FREEITEMWORLD]);
	return PointerIndexTable[PTR_FREEITEMWORLD];
}

void RemoveItemFromWorld(int fieldpos) {
	//Log(s:"remove item id ", d:fieldpos);
	Inventories_On_Field[fieldpos].width = 1;
	Inventories_On_Field[fieldpos].height = 1;
	Inventories_On_Field[fieldpos].item_type = DND_ITEM_NULL;
	Inventories_On_Field[fieldpos].item_subtype = 0;
	Inventories_On_Field[fieldpos].item_image = 0;
	Inventories_On_Field[fieldpos].item_level = 0;
	Inventories_On_Field[fieldpos].item_stack = 0;

	Inventories_On_Field[fieldpos].corrupted = 0;
	Inventories_On_Field[fieldpos].quality = 0;

	int k;
	for(k = 0; k < MAX_ITEM_IMPLICITS; ++k) {
		Inventories_On_Field[fieldpos].implicit[k].attrib_id = -1;
		Inventories_On_Field[fieldpos].implicit[k].attrib_val = 0;
		Inventories_On_Field[fieldpos].implicit[k].attrib_tier = 0;
		Inventories_On_Field[fieldpos].implicit[k].attrib_extra = 0;
	}

	for(k = 0; k < Inventories_On_Field[fieldpos].attrib_count; ++k) {
		Inventories_On_Field[fieldpos].attributes[k].attrib_id = 0;
		Inventories_On_Field[fieldpos].attributes[k].attrib_val = 0;
		Inventories_On_Field[fieldpos].attributes[k].attrib_tier = 0;
		Inventories_On_Field[fieldpos].attributes[k].fractured = 0;
		Inventories_On_Field[fieldpos].attributes[k].attrib_extra = 0;
	}
	Inventories_On_Field[fieldpos].attrib_count = 0;

	Inventories_On_Field[fieldpos].isDirty = true;
	Inventories_On_Field[fieldpos].textID = 0;
}

// Deletes an item, essentially
void FreeItem(int pnum, int item_index, int source, bool dontSync) {
	//if(source == DND_SYNC_ITEMSOURCE_FIELD)
	//	Log(s:"free item id ", d:item_index);
	int i, j;
	int temp;
	int wtemp = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, item_index, -1, source);
	int htemp = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, item_index, -1, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, item_index, -1, 0, source);

	SetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_index, -1, 0, source);

	for(j = 0; j < MAX_ITEM_IMPLICITS; ++j) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, j, -1, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, j, 0, source);
	}
	
	temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	for(j = 0; j < temp; ++j) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, item_index, j, 0, source);
	}
	SetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, 0, source);
	
	if(IsSourceInventoryView(source)) {
		for(j = 0; j < htemp; ++j)
			for(i = 0; i < wtemp; ++i) {
				SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, item_index + j * MAXINVENTORYBLOCKS_VERT + i, -1, 0, source);
				SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_index + j * MAXINVENTORYBLOCKS_VERT + i, -1, DND_ITEM_NULL, source);
			}
	}
	else {
		SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_index, -1, DND_ITEM_NULL, source);
	}
		
	SetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, item_index, -1, 0, source);
	if(!dontSync)
		SyncItemData_Null(pnum, item_index, source, wtemp, htemp);
}

void FreeSpot(int pnum, int item_index, int source) {
	int j, temp;
	SetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, item_index, -1, 0, source);

	SetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_index, -1, 0, source);

	for(j = 0; j < MAX_ITEM_IMPLICITS; ++j) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, j, -1, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, j, 0, source);
	}
	
	temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	for(j = 0; j < temp; ++j) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, j, 0, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, item_index, j, 0, source);
	}
	SetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_index, -1, DND_ITEM_NULL, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, item_index, -1, 0, source);
	SetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, item_index, -1, 0, source);
}

// move this from field to player's inventory
int HandleInventoryPickup(int item_index) {
	int pnum = PlayerNumber();
	int pcharm_index = GetFreeSpotForItem(item_index, pnum, DND_SYNC_ITEMSOURCE_FIELD, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	CopyItemFromFieldToPlayer(item_index, pnum, pcharm_index);
	ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
	return pcharm_index;
}

int GetStackValue(int type) {
	switch (type) {
		case DND_ITEM_ORB:
		return MAXSTACKS_ORB;
		case DND_ITEM_CHESTKEY:
		return MAXSTACKS_CKEY;
		case DND_ITEM_TOKEN:
		return MAXSTACKS_TOKEN;
	}
	return 0;
}

bool IsStackedItem(int type) {
	switch (type) {
		case DND_ITEM_ORB:
		case DND_ITEM_CHESTKEY:
		case DND_ITEM_TOKEN:
		return true;
	}
	return false;
}

int GetMaxItemAffixes(int item_type, int item_subtype = -1) {
	int res = 0;

	if(item_type > UNIQUE_BEGIN)
		item_type &= 0xFFFF;

	switch(item_type) {
		case DND_ITEM_CHARM:
			if(item_subtype == DND_CHARM_SMALL)
				res = 2;
			else if(item_subtype == DND_CHARM_MEDIUM)
				res = 4;
			else
				res = 6;
		break;
		case DND_ITEM_BODYARMOR:
			res = MAX_ARMOR_ATTRIB_DEFAULT;
		break;
		case DND_ITEM_HELM:
			res = MAX_HELM_ATTRIB_DEFAULT;
		break;
		case DND_ITEM_BOOT:
			res = MAX_BOOT_ATTRIB_DEFAULT;
		break;


		case DND_ITEM_SPECIALTY_DOOMGUY:
		case DND_ITEM_SPECIALTY_MARINE:
		case DND_ITEM_SPECIALTY_HOBO:
		case DND_ITEM_SPECIALTY_PUNISHER:
		case DND_ITEM_SPECIALTY_WANDERER:
		case DND_ITEM_SPECIALTY_CYBORG:
		case DND_ITEM_SPECIALTY_BERSERKER:
		case DND_ITEM_SPECIALTY_TRICKSTER:
			res = MAX_POWERCORE_ATTRIB_DEFAULT;
		break;
	}
	return res;
}

int FindInventoryOfType(int player_index, int item_type, int item_subtype) {
	int i = 0, j = 0, bid = 0;
	
	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			bid = j * MAXINVENTORYBLOCKS_VERT + i;
			// notice we can return bid here, we don't check like below
			if(PlayerInventoryList[player_index][bid].item_type == item_type && PlayerInventoryList[player_index][bid].item_subtype == item_subtype)
				return bid;
		}
	}
	
	return -1;
}

// note to self: height is => horizontal, moving heights => x * MAXINVENTORYBLOCKS_VERT, width is vertical, just + x
int GetFreeSpotForItem(int item_index, int player_index, int item_source, int dest_source, int source_player = -1) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	int w, h;

	int temp = item_index;
	if(IsSourceInventoryView(item_source))
		temp = GetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, temp, -1, item_source) - 1;
	
	// extended check for potential player source change
	if(source_player == -1) {
		w = GetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, temp, -1, item_source);
		h = GetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, temp, -1, item_source);
	}
	else {
		w = GetItemSyncValue(source_player, DND_SYNC_ITEMWIDTH, temp, -1, item_source);
		h = GetItemSyncValue(source_player, DND_SYNC_ITEMHEIGHT, temp, -1, item_source);
	}
	
	bool unfit = false;

	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			bool rowStart = !(i % MAXINVENTORYBLOCKS_VERT);
			for(hcheck = 0; !unfit && hcheck < h && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < w && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if
					(
						bid >= MAX_INVENTORY_BOXES || 
						(!rowStart && !(bid % 9)) ||
						GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, bid, -1, dest_source) != DND_ITEM_NULL
					)
						unfit = true;
				}
			}
			// we return top left corner box id
			if(wcheck == w && hcheck == h && !unfit) {
				return j * MAXINVENTORYBLOCKS_VERT + i;
			}
		}
	}
	//printbold(s:"found no spot");
	return -1;
}

int GetFreeSpotForItemWithStack(int item_index, int player_index, int item_source, int dest_source, bool check_stack = true, int source_player = -1) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	int w, h;
	bool unfit = false;
	
	// extended check for potential player source change
	if(source_player == -1) {
		w = GetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, item_index, -1, item_source);
		h = GetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, item_index, -1, item_source);
	}
	else {
		w = GetItemSyncValue(source_player, DND_SYNC_ITEMWIDTH, item_index, -1, item_source);
		h = GetItemSyncValue(source_player, DND_SYNC_ITEMHEIGHT, item_index, -1, item_source);
	}
	
	// first search for any spot on our inventory for a stack item of this type
	int type = GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, item_index, -1, item_source);
	int sub = GetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, item_index, -1, item_source);
	int maxstack = GetStackValue(type);
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if
		(
			GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, i, -1, dest_source) == type &&
			GetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, i, -1, dest_source) == sub &&
			(
				(!check_stack && GetItemSyncValue(player_index, DND_SYNC_ITEMSTACK, i, -1, dest_source) != maxstack) || 
				(GetItemSyncValue(player_index, DND_SYNC_ITEMSTACK, i, -1, dest_source) + GetItemSyncValue(player_index, DND_SYNC_ITEMSTACK, item_index, -1, item_source) < maxstack)
			)
		)
			return i;
	}
	
	// didn't work, find new spot
	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			for(hcheck = 0; !unfit && hcheck < h && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < w && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if(bid >= MAX_INVENTORY_BOXES || GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, bid, -1, dest_source) != DND_ITEM_NULL)
						unfit = true;
				}
			}
			// we return top left corner box id
			if(wcheck == w && hcheck == h && !unfit)
				return j * MAXINVENTORYBLOCKS_VERT + i;
		}
	}
	return -1;
}

int GetFreeSpotForSingleSpotItem(int player_index, int type, int sub) {
	int i = 0, j = 0;
	int bid = 0, wcheck = 0, hcheck = 0;
	bool unfit = false;
	
	// first search for any spot on our inventory for a stack item of this type
	int maxstack = GetStackValue(type);
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i)
		if(PlayerInventoryList[player_index][i].item_type == type && PlayerInventoryList[player_index][i].item_subtype == sub && PlayerInventoryList[player_index][i].item_stack < maxstack)
			return i;
			
	// didn't work, find new spot
	// try every line
	for(i = 0; i < MAXINVENTORYBLOCKS_VERT; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_HORIZ; ++j) {
			// if width matches, try height from here on then and if unfit, restart at a new coordinate
			unfit = false;
			for(hcheck = 0; !unfit && hcheck < 1 && hcheck + j < MAXINVENTORYBLOCKS_HORIZ; ++hcheck) {
				for(wcheck = 0; !unfit && wcheck < 1 && wcheck + i < MAXINVENTORYBLOCKS_VERT; ++wcheck) {
					bid = (j + hcheck) * MAXINVENTORYBLOCKS_VERT + i + wcheck;
					if(bid >= MAX_INVENTORY_BOXES || PlayerInventoryList[player_index][bid].item_type != DND_ITEM_NULL)
						unfit = true;
				}
			}
			// we return top left corner box id
			if(wcheck == 1 && hcheck == 1 && !unfit)
				return j * MAXINVENTORYBLOCKS_VERT + i;
		}
	}
	return -1;
}

int GetFreeSpotForItem_Trade(int item_index, int source_player, int player_index, int source) {
	if(!IsStackedItem(GetItemSyncValue(source_player, DND_SYNC_ITEMTYPE, item_index, -1, source)))
		return GetFreeSpotForItem(item_index, player_index, source, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, source_player);
	return GetFreeSpotForItemWithStack(item_index, player_index, source, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, true, source_player);
}

// Check if we have enough space for all the items offered, in our inventory
bool ConfirmSpaceForOfferings(int pnum, int tradee) {
	int bid, pos;
	int i, j, h, w, hcomp, wcomp;
	// for every possible item in the trade list of this player, get free position
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			bid = j + i * MAXINVENTORYBLOCKS_VERT;
			// care about the items only once, so use topleftboxid == bid
			if(TradeViewList[tradee][bid].topleftboxid - 1 == bid) {
				pos = GetFreeSpotForItem_Trade(bid, tradee, pnum, DND_SYNC_ITEMSOURCE_TRADEVIEW);
				if(pos != -1) {
					// mark as occupied so getfreespot wont return them
					hcomp = TradeViewList[tradee][bid].height;
					wcomp = TradeViewList[tradee][bid].width;
					for(h = 0; h < hcomp; ++h)
						for(w = 0; w < wcomp; ++w) {
							// hack to avoid finding the same spot as empty
							if(PlayerInventoryList[pnum][pos + w + h * MAXINVENTORYBLOCKS_VERT].item_type == DND_ITEM_NULL)
								PlayerInventoryList[pnum][pos + w + h * MAXINVENTORYBLOCKS_VERT].item_type = DND_ITEM_TEMPORARY;
							ItemMoveList[pnum][bid + w + h * MAXINVENTORYBLOCKS_VERT].state = true;
							ItemMoveList[pnum][bid + w + h * MAXINVENTORYBLOCKS_VERT].width = wcomp;
							ItemMoveList[pnum][bid + w + h * MAXINVENTORYBLOCKS_VERT].height = hcomp;
						}
					//printbold(s:"send player ", d:pnum, s:"'s item at ", d:bid, s:" to ", d:pos);
					ItemMoveList[pnum][bid].dest_pos = pos;
				}
				else {
					// clean up whatever was used up, we had no space
					for(pos = 0; pos < MAX_INVENTORY_BOXES; ++pos) {
						if(ItemMoveList[pnum][pos].state) {
							// cleanup our hack
							if(PlayerInventoryList[pnum][pos].item_type == DND_ITEM_TEMPORARY)
								PlayerInventoryList[pnum][pos].item_type = DND_ITEM_NULL;
							ItemMoveList[pnum][pos].state = false;
							ItemMoveList[pnum][pos].width = 0;
							ItemMoveList[pnum][pos].height = 0;
							ItemMoveList[pnum][pos].dest_pos = -1;
						}
					}
					return false;
				}
			}
		}
	}
	
	// unmark all marked spots as unoccupied (real marking happens when we move items)
	// do not touch itemmovelist, we will use this as the place to move new items so we avoid checking again
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(ItemMoveList[pnum][i].state) {
			// unmark as occupied
			wcomp = ItemMoveList[pnum][i].width;
			hcomp = ItemMoveList[pnum][i].height;
			
			// note that the itemmovelist contains "Item of Player in tradebox was to move to dest_pos in player inventory" info
			pos = ItemMoveList[pnum][i].dest_pos;
			for(h = 0; h < hcomp; ++h)
				for(w = 0; w < wcomp; ++w) {
					// cleanup our hack
					if(PlayerInventoryList[pnum][pos + w + h * MAXINVENTORYBLOCKS_VERT].item_type == DND_ITEM_TEMPORARY)
						PlayerInventoryList[pnum][pos + w + h * MAXINVENTORYBLOCKS_VERT].item_type = DND_ITEM_NULL;
				}
		}
	}
	
	return true;
}

bool IsWearingBodyArmor(int pnum) {
	return (Items_Used[pnum][BODY_ARMOR_INDEX].item_type & 0xFFFF) == DND_ITEM_BODYARMOR;
}

bool IsPlayerInventoryItemUnique(int pnum, int pos) {
	return PlayerInventoryList[pnum][pos].item_type > UNIQUE_BEGIN;
}

int MakeItemUsed(int pnum, int use_id, int item_index, int item_type, int target_type) {
	int i, j;
	// type mismatch, popup
	if(item_type == DND_ITEM_CHARM && target_type != PlayerInventoryList[pnum][item_index].item_subtype)
		return POPUP_CHARMMISMATCH;
		
	// too high level
	if(PlayerInventoryList[pnum][item_index].item_level > GetLevel())
		return POPUP_ITEMLVLTOOHIGH;
		
	// no duplicate uniques
	if(DoUniqueCheck(pnum, use_id, item_index, target_type))
		return POPUP_ONLYONEUNIQUE;
	
	// special condition checks
	bool isUnique = IsPlayerInventoryItemUnique(pnum, item_index);
	// tried to put well of power, but have too many small charms -- its always attribute id 2 on well of power
	// we have to check for that specifically because technically its not equipped yet, so player has no tokens of it on them
	if
	(
		isUnique && 
		(PlayerInventoryList[pnum][item_index].item_type >> UNIQUE_BITS) - 1 == UITEM_WELLOFPOWER &&
		CountPlayerSmallCharms(pnum) > PlayerInventoryList[pnum][item_index].attributes[1].attrib_val
	)
		return POPUP_NOMORESMALLCHARMS;
	
	// or tried to put small charm when well of power is there and would exceed limit
	if(target_type == DND_CHARM_SMALL && (i = GetPlayerAttributeValue(pnum, INV_EX_LIMITEDSMALLCHARMS)) && i != MAX_SMALL_CHARMS_USED && i == CountPlayerSmallCharms(pnum))
		return POPUP_NOMORESMALLCHARMS;

	// if has forbid armor but has equipped body armor, don't allow that item to be put, and vice versa if has no armor and has forbid armor etc.
	if
	(
		isUnique &&
		(PlayerInventoryList[pnum][item_index].item_type >> UNIQUE_BITS) - 1 == UITEM_OAKHEART &&
		IsWearingBodyArmor(pnum)
	)
		return POPUP_CANTWEARBODYARMOR;

	if(item_type == DND_ITEM_BODYARMOR && GetPlayerAttributeValue(pnum, INV_EX_FORBID_ARMOR))
		return POPUP_CANTPUTONBODYARMOR;
	// proceed to equip the item now

	// this means we must swap items
	if(Items_Used[pnum][use_id].item_type != DND_ITEM_NULL) {
		ApplyItemFeatures(pnum, use_id, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_REMOVE, true);
		SwapItems(pnum, use_id, item_index, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false, true);
		ApplyItemFeatures(pnum, use_id, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_ADD);
	}
	else {
		// just zero the stuff in inventory, and copy them into items used
		Items_Used[pnum][use_id].width = PlayerInventoryList[pnum][item_index].width;
		Items_Used[pnum][use_id].height = PlayerInventoryList[pnum][item_index].height;
		Items_Used[pnum][use_id].item_type = PlayerInventoryList[pnum][item_index].item_type;
		Items_Used[pnum][use_id].item_subtype = PlayerInventoryList[pnum][item_index].item_subtype;
		Items_Used[pnum][use_id].item_image = PlayerInventoryList[pnum][item_index].item_image;
		Items_Used[pnum][use_id].item_level = PlayerInventoryList[pnum][item_index].item_level;
		Items_Used[pnum][use_id].item_stack = PlayerInventoryList[pnum][item_index].item_stack;
		Items_Used[pnum][use_id].attrib_count = PlayerInventoryList[pnum][item_index].attrib_count;
		Items_Used[pnum][use_id].topleftboxid = use_id + 1;

		Items_Used[pnum][use_id].corrupted = PlayerInventoryList[pnum][item_index].corrupted;
		Items_Used[pnum][use_id].quality = PlayerInventoryList[pnum][item_index].quality;

		for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
			Items_Used[pnum][use_id].implicit[i].attrib_id = PlayerInventoryList[pnum][item_index].implicit[i].attrib_id;
			Items_Used[pnum][use_id].implicit[i].attrib_val = PlayerInventoryList[pnum][item_index].implicit[i].attrib_val;
			Items_Used[pnum][use_id].implicit[i].attrib_tier = PlayerInventoryList[pnum][item_index].implicit[i].attrib_tier;
			Items_Used[pnum][use_id].implicit[i].attrib_extra = PlayerInventoryList[pnum][item_index].implicit[i].attrib_extra;
		}

		Items_Used[pnum][use_id].isDirty = true;
		Items_Used[pnum][use_id].textID = 0;

		for(i = 0; i < Items_Used[pnum][use_id].attrib_count; ++i) {
			Items_Used[pnum][use_id].attributes[i].attrib_id = PlayerInventoryList[pnum][item_index].attributes[i].attrib_id;
			Items_Used[pnum][use_id].attributes[i].attrib_val = PlayerInventoryList[pnum][item_index].attributes[i].attrib_val;
			Items_Used[pnum][use_id].attributes[i].attrib_tier = PlayerInventoryList[pnum][item_index].attributes[i].attrib_tier;
			Items_Used[pnum][use_id].attributes[i].attrib_extra = PlayerInventoryList[pnum][item_index].attributes[i].attrib_extra;
			Items_Used[pnum][use_id].attributes[i].fractured = PlayerInventoryList[pnum][item_index].attributes[i].fractured;
		}

		// the leftover spot is a null item
		int wtemp = PlayerInventoryList[pnum][item_index].width;
		int htemp = PlayerInventoryList[pnum][item_index].height;
		FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
		//SyncItemData(item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, wtemp, htemp);
		SyncItemData(pnum, use_id, DND_SYNC_ITEMSOURCE_ITEMSUSED, -1, -1);
		ApplyItemFeatures(pnum, use_id, DND_SYNC_ITEMSOURCE_ITEMSUSED);
	}

	// request damage cache recalculation
	ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, PlayerNumber());

	return -1;
}

// based on average player level
int RollItemLevel() {
	int res = PlayerInformationInLevel[PLAYERLEVELINFO_COUNTATSTART];
	if(!res)
		res = 1;
	
	// return average player level, +- some value -- 33% chance to just roll something between min player level and max player level regardless
	// if that 33% chance rolls then there's a 50-50 chance that it'll be either min - max level players or something much closer to lower lvl player
	if(!random(0, 2)) {
		if(random(0, 1))
			return random(PlayerInformationInLevel[PLAYERLEVELINFO_MINLEVEL], PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL]);
		res = random(PlayerInformationInLevel[PLAYERLEVELINFO_MINLEVEL], PlayerInformationInLevel[PLAYERLEVELINFO_MINLEVEL] + 2 * ITEMLEVEL_VARIANCE_HIGHER);
		res = Clamp_Between(res, 1, MAX_ITEM_LEVEL);
		return res;
	}

	int pavg = PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] / res;
	if(pavg > 2 * ITEMLEVEL_VARIANCE_LOWER) {
		res = pavg + random(-ITEMLEVEL_VARIANCE_LOWER, ITEMLEVEL_VARIANCE_HIGHER);
		res = Clamp_Between(res, 1, MAX_ITEM_LEVEL);
		return res;
	}
	return pavg + random((-pavg + 1) / 2, ITEMLEVEL_VARIANCE_HIGHER);
}

// attrib_id is the raw attribute, like INV_INCHP etc. not an index in the item!
int CheckItemAttribute(int pnum, int item_pos, int attrib_id, int source, int count) {
	int i;
	for(i = 0; i < count; ++i) {
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_pos, i, source) == attrib_id)
			return i;
	}
	return -1;
}

// specialized version of the above function for playerinventorylist -- keeps it cleaner arguments wise I guess
int IsAttribInItem(int pnum, int item_pos, int attrib_id) {
	int count = PlayerInventoryList[pnum][item_pos].attrib_count;
	for(int i = 0; i < count; ++i)
		if(PlayerInventoryList[pnum][item_pos].attributes[i].attrib_id == attrib_id)
			return true;
	return false;
}

// find the item that has a min, if basis isn't -1 then we must exclude this from inclusion to min
int FindMinOnUsedCharmsForAttribute(int pnum, int attrib_index, int basis) {
	int res = -1, temp, compare = INT_MAX;
	for(int i = 0; i < MAX_ITEMS_EQUIPPABLE; ++i) {
		if(i == basis)
			continue;
		if(Items_Used[pnum][i].item_type != DND_ITEM_NULL) {
			temp = CheckItemAttribute(pnum, i, attrib_index, DND_SYNC_ITEMSOURCE_ITEMSUSED, Items_Used[pnum][i].attrib_count);
			// means this exists
			if(temp != -1) {
				if(Items_Used[pnum][i].attributes[temp].attrib_val < compare) {
					compare = Items_Used[pnum][i].attributes[temp].attrib_val;
					SetInventory("DamagePerFlatHPBuffer", compare);
					res = i;
				}
			}
		}
	}
	return res;
}

// returns false if it should not destroy item, true if it should
bool CopyItemFromFieldToPlayer(int fieldpos, int player_index, int item_index, int stacked_item_type = -1) {
	int i, j, k, wtemp, htemp;
	// handle the box management
	// is this a stack item and does it already contain an item of this type?
	// ex type = orb, subtype = enhancement
	int max_stack = GetStackValue(Inventories_On_Field[fieldpos].item_type);
	if(
		Inventories_On_Field[fieldpos].item_stack && 
		Inventories_On_Field[fieldpos].item_type == PlayerInventoryList[player_index][item_index].item_type && 
		Inventories_On_Field[fieldpos].item_subtype == PlayerInventoryList[player_index][item_index].item_subtype &&
		PlayerInventoryList[player_index][item_index].item_stack + Inventories_On_Field[fieldpos].item_stack <= max_stack
	) {
		// just add to the stack
		PlayerInventoryList[player_index][item_index].item_stack += Inventories_On_Field[fieldpos].item_stack;
		SyncItemData(player_index, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[player_index][item_index].width, PlayerInventoryList[player_index][item_index].height);
	}
	else {
		// no? -- check to dump as much of the stack from this into the other item, and create new spot with leftover stack
		if(PlayerInventoryList[player_index][item_index].item_stack != max_stack && PlayerInventoryList[player_index][item_index].item_stack + Inventories_On_Field[fieldpos].item_stack > max_stack) {
			Inventories_On_Field[fieldpos].item_stack -= max_stack - PlayerInventoryList[player_index][item_index].item_stack;
			PlayerInventoryList[player_index][item_index].item_stack = max_stack;
			SyncItemData(player_index, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[player_index][item_index].width, PlayerInventoryList[player_index][item_index].height);
			SyncItemData(player_index, fieldpos, DND_SYNC_ITEMSOURCE_FIELD, Inventories_On_Field[fieldpos].width, Inventories_On_Field[fieldpos].height);
			
			// create new item and place it, if can't find place to put it leave it on the ground with leftover stacks
			int new_pos = GetFreeSpotForItemWithStack(fieldpos, player_index, DND_SYNC_ITEMSOURCE_FIELD, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			if(new_pos != -1)
				return CopyItemFromFieldToPlayer(fieldpos, player_index, new_pos);
			
			ACS_NamedExecuteAlways("DnD Inventory Full CS", 0, player_index);
			return false;
		}

		// look to place it
		wtemp = Inventories_On_Field[fieldpos].width;
		htemp = Inventories_On_Field[fieldpos].height;
		PlayerInventoryList[player_index][item_index].width = wtemp;
		PlayerInventoryList[player_index][item_index].height = htemp;
		PlayerInventoryList[player_index][item_index].item_subtype = Inventories_On_Field[fieldpos].item_subtype;
		PlayerInventoryList[player_index][item_index].item_image = Inventories_On_Field[fieldpos].item_image;
		PlayerInventoryList[player_index][item_index].item_level = Inventories_On_Field[fieldpos].item_level;
		PlayerInventoryList[player_index][item_index].item_stack = Inventories_On_Field[fieldpos].item_stack;

		PlayerInventoryList[player_index][item_index].corrupted = Inventories_On_Field[fieldpos].corrupted;
		PlayerInventoryList[player_index][item_index].quality = Inventories_On_Field[fieldpos].quality;

		for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
			PlayerInventoryList[player_index][item_index].implicit[i].attrib_id = Inventories_On_Field[fieldpos].implicit[i].attrib_id;
			PlayerInventoryList[player_index][item_index].implicit[i].attrib_val = Inventories_On_Field[fieldpos].implicit[i].attrib_val;
			PlayerInventoryList[player_index][item_index].implicit[i].attrib_tier = Inventories_On_Field[fieldpos].implicit[i].attrib_tier;
			PlayerInventoryList[player_index][item_index].implicit[i].attrib_extra = Inventories_On_Field[fieldpos].implicit[i].attrib_extra;
		}

		PlayerInventoryList[player_index][item_index].isDirty = true;

		PlayerInventoryList[player_index][item_index].attrib_count = Inventories_On_Field[fieldpos].attrib_count;
		for(k = 0; k < PlayerInventoryList[player_index][item_index].attrib_count; ++k) {
			PlayerInventoryList[player_index][item_index].attributes[k].attrib_id = Inventories_On_Field[fieldpos].attributes[k].attrib_id;
			PlayerInventoryList[player_index][item_index].attributes[k].attrib_val = Inventories_On_Field[fieldpos].attributes[k].attrib_val;
			PlayerInventoryList[player_index][item_index].attributes[k].attrib_tier = Inventories_On_Field[fieldpos].attributes[k].attrib_tier;
			PlayerInventoryList[player_index][item_index].attributes[k].attrib_extra = Inventories_On_Field[fieldpos].attributes[k].attrib_extra;
			PlayerInventoryList[player_index][item_index].attributes[k].fractured = Inventories_On_Field[fieldpos].attributes[k].fractured;
		}
		for(i = 0; i < htemp; ++i)
			for(j = 0; j < wtemp; ++j) {
				PlayerInventoryList[player_index][item_index + i * MAXINVENTORYBLOCKS_VERT + j].item_type = Inventories_On_Field[fieldpos].item_type;
				PlayerInventoryList[player_index][item_index + i * MAXINVENTORYBLOCKS_VERT + j].topleftboxid = item_index + 1;
			}
		SyncItemData(player_index, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, wtemp, htemp);
	}
	// the leftover spot is a null item
	RemoveItemFromWorld(fieldpos);
	//FreeItem(-1, fieldpos, DND_SYNC_ITEMSOURCE_FIELD, false);
	return true;
}

// clones an item on this player's inventory, if no spot is found it won't bother
int CloneItem(int pnum, int item_index, int source, bool dontSync) {
	int temp;
	int c = GetFreeSpotForItem(item_index, pnum, source, source);
	if(c != -1) {
		int wtemp = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, item_index, -1, source);
		int htemp = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, item_index, -1, source);
		int i, j, k;
		SetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, c, -1, wtemp, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, c, -1, htemp, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, item_index, -1, source), source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, item_index, -1, source), source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, item_index, -1, source), source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, item_index, -1, source), source);

		SetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, item_index, -1, source), source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, c, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_index, -1, source), source);

		for(k = 0; k < MAX_ITEM_IMPLICITS; ++k) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, k, source), source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, k, source), source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, item_index, k, source), source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, k, source), source);
		}

		SetItemSyncValue(pnum, DND_SYNC_ISDIRTY, c, -1, true, source);

		temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
		SetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, c, -1, temp, source);
		for(k = 0; k < temp; ++k) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, k, source), source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, k, source), source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, k, source), source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, k, source), source);
			SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, c, k, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, item_index, k, source), source);
		}
		for(i = 0; i < htemp; ++i)
			for(j = 0; j < wtemp; ++j) {
				temp = c + i * MAXINVENTORYBLOCKS_VERT + j;
				SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_index, -1, source), source);
				SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, temp, -1, c + 1, source);
			}
		if(!dontSync)
			SyncItemData(pnum, c, source, -1, -1);
		return c;
	}
	return -1;
}

// check if clicked spot is free for the item we want to put
bool IsFreeSpot(int pnum, int itempos, int emptypos, int itemsource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, int emptysource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) {
	int temp = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, itemsource) - 1;
	int w = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, temp, -1, itemsource);
	int h = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, temp, -1, itemsource);
	int offset = temp - itempos;
	int bid = 0;
	int tb = 0;

	bool rowStart = !((emptypos + offset) % MAXINVENTORYBLOCKS_VERT);
	
	for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
			bid = emptypos + offset + j + i * MAXINVENTORYBLOCKS_VERT;
			// vertical range check
			if(bid >= MAX_INVENTORY_BOXES || bid < 0)
				return false;

			// horiz range check -- this one's a bit more involved
			// check if we go from a non-multiple of 9 to a multiple of 9, that's when you get on a new line
			if(!rowStart && !(bid % 9))
				return false;

			// if not empty and it's not us -- the "if its not us" part only matters if the sources are the same
			// because only then it shouldn't overlap, if sources are different different topboxids are irrelevant!
			tb = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, emptysource);
			if(tb && (itemsource != emptysource || tb - 1 != temp))
				return false;
		}
	}
	return true;
}

bool IsValidBox(int beg) {
	return beg < MAX_INVENTORY_BOXES && beg >= 0;
}

bool IsSourceInventoryView(int source) {
	// just in case we receive this with page numbers in it
	source &= 0xFFFF;
	return source == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY || source == DND_SYNC_ITEMSOURCE_TRADEVIEW || source == DND_SYNC_ITEMSOURCE_STASH;
}

// assumes both spots have an item
// if ipos1 has enough space to acommodate ipos2
bool CanSwapItems(int pnum, int ipos1, int ipos2, int offset1, int offset2, int source1, int source2) {
	int i, j;
	int w1, w2, h1, h2;
	int bid = 0, tb1, tb2;

	// these hold 64 box spots (45 only needed but still) to make sure we are good on the overlapping areas
	int overlap1 = 0, overlap2 = 0;
	bool rowStart = false;
	// from ipos2 to ipos1
	tb1 = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, source1) - 1;
	tb2 = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, source2) - 1;
	w2 = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, tb2, -1, source2);
	h2 = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, tb2, -1, source2);
				
	rowStart = !(((ipos1 + offset2)) % MAXINVENTORYBLOCKS_VERT);
	for(i = 0; i < h2; ++i) {
		for(j = 0; j < w2; ++j) {
			bid = ipos1 + offset2 + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0 || (!rowStart && !(bid % 9)))
				return false;

			if(IsSourceInventoryView(source1) && GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source1) - 1 != tb1 && GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, bid, -1, source1) != DND_ITEM_NULL)
				return false;

			if(bid < 32)
				overlap1 |= 1 << bid;
			else
				overlap2 |= 1 << (bid - 32);
		}
	}

	// from ipos1 to ipos2
	w1 = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, tb1, -1, source1);
	h1 = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, tb1, -1, source1);
	rowStart = !(((ipos2 + offset1)) % MAXINVENTORYBLOCKS_VERT);
	for(i = 0; i < h1; ++i) {
		for(j = 0; j < w1; ++j) {
			bid = ipos2 + offset1 + j + i * MAXINVENTORYBLOCKS_VERT;
			if(bid >= MAX_INVENTORY_BOXES || bid < 0 || (!rowStart && !(bid % 9)))
				return false;

			if(IsSourceInventoryView(source2) && GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source2) - 1 != tb2 && GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, bid, -1, source2) != DND_ITEM_NULL)
				return false;

			// check potential overlaps now -- can only happen if same source
			if(source1 == source2 && ((bid < 32 && (overlap1 & (1 << bid))) || (overlap2 & (1 << (bid - 32)))))
				return false;
		}
	}
	return true;
}

// accepts topboxid as item_index
void MoveItemToTemporary(int player_index, int item_index, int temp_pos, int source) {
	int i;
	TemporaryInventoryList[player_index][temp_pos].width = GetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].height = GetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].item_type = GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].item_subtype = GetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].item_image = GetItemSyncValue(player_index, DND_SYNC_ITEMIMAGE, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].item_level = GetItemSyncValue(player_index, DND_SYNC_ITEMLEVEL, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].item_stack = GetItemSyncValue(player_index, DND_SYNC_ITEMSTACK, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].attrib_count = GetItemSyncValue(player_index, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);

	TemporaryInventoryList[player_index][temp_pos].corrupted = GetItemSyncValue(player_index, DND_SYNC_ITEMCORRUPTED, item_index, -1, source);
	TemporaryInventoryList[player_index][temp_pos].quality = GetItemSyncValue(player_index, DND_SYNC_ITEMQUALITY, item_index, -1, source);

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		TemporaryInventoryList[player_index][temp_pos].implicit[i].attrib_id = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, i, source);
		TemporaryInventoryList[player_index][temp_pos].implicit[i].attrib_val = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, i, source);
		TemporaryInventoryList[player_index][temp_pos].implicit[i].attrib_tier = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, item_index, i, source);
		TemporaryInventoryList[player_index][temp_pos].implicit[i].attrib_extra = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, i, source);
	}

	TemporaryInventoryList[player_index][temp_pos].isDirty = true;
	TemporaryInventoryList[player_index][temp_pos].textID = 0;

	TemporaryInventoryList[player_index][temp_pos].topleftboxid = 0;
	for(i = 0; i < TemporaryInventoryList[player_index][temp_pos].attrib_count; ++i) {
		TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_id = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_ID, item_index, i, source);
		TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_val = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, i, source);
		TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_tier = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, i, source);
		TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_extra = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, i, source);
		TemporaryInventoryList[player_index][temp_pos].attributes[i].fractured = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_FRACTURE, item_index, i, source);
	}
}

// copies item from temporary list to player inventory
void CopyItemFromTemporary(int player_index, int item_index, int temp_pos, int source) {
	int i;
	int w = TemporaryInventoryList[player_index][temp_pos].width, h = TemporaryInventoryList[player_index][temp_pos].height;
	SetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, item_index, -1, w, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, item_index, -1, h, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, item_index, -1, TemporaryInventoryList[player_index][temp_pos].item_subtype, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, TemporaryInventoryList[player_index][temp_pos].attrib_count, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMSTACK, item_index, -1, TemporaryInventoryList[player_index][temp_pos].item_stack, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMLEVEL, item_index, -1, TemporaryInventoryList[player_index][temp_pos].item_level, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMIMAGE, item_index, -1, TemporaryInventoryList[player_index][temp_pos].item_image, source);

	SetItemSyncValue(player_index, DND_SYNC_ITEMCORRUPTED, item_index, -1, TemporaryInventoryList[player_index][temp_pos].corrupted, source);
	SetItemSyncValue(player_index, DND_SYNC_ITEMQUALITY, item_index, -1, TemporaryInventoryList[player_index][temp_pos].quality, source);

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, i, TemporaryInventoryList[player_index][temp_pos].implicit[i].attrib_id, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, i, TemporaryInventoryList[player_index][temp_pos].implicit[i].attrib_val, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, item_index, i, TemporaryInventoryList[player_index][temp_pos].implicit[i].attrib_tier, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, i, TemporaryInventoryList[player_index][temp_pos].implicit[i].attrib_extra, source);
	}
	
	SetItemSyncValue(player_index, DND_SYNC_ISDIRTY, item_index, -1, true, source);
	SetItemSyncValue(player_index, DND_SYNC_TEXTID, item_index, -1, 0, source);

	if(IsSourceInventoryView(source)) {
		for(i = 0; i < h; ++i) {
			for(int j = 0; j < w; ++j) {
				int bid = item_index + j + i * MAXINVENTORYBLOCKS_VERT;
				SetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, item_index + 1, source);
				SetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, bid, -1, TemporaryInventoryList[player_index][temp_pos].item_type, source);
			}
		}
	}
	else {
		SetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, item_index + 1, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, item_index, -1, TemporaryInventoryList[player_index][temp_pos].item_type, source);
	}
	
	for(i = 0; i < TemporaryInventoryList[player_index][temp_pos].attrib_count; ++i) {
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_ID, item_index, i, TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_id, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, i, TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_val, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_TIER, item_index, i, TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_tier, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, i, TemporaryInventoryList[player_index][temp_pos].attributes[i].attrib_extra, source);
		SetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_FRACTURE, item_index, i, TemporaryInventoryList[player_index][temp_pos].attributes[i].fractured, source);
	}
}

// for swapping items -- assumes neither are null
void SwapItems(int pnum, int ipos1, int ipos2, int source1, int source2, bool dontSync, bool forcedSwap = false) {
	// check if there's enough space
	// find if there is some sort of offset we must take care of, this matters only if we are swapping in inventory
	int offset1 = 0, offset2 = 0;
	int i, j, h1p, h2p, w1p, w2p;
	if(IsSourceInventoryView(source1))
		offset1 = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, ipos1, -1, source1) - 1 - ipos1;
	if(IsSourceInventoryView(source2))
		offset2 = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, ipos2, -1, source2) - 1 - ipos2;
		
	// if both items are stack items and are of same types, add stack on top of the other
	h1p = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos1 + offset1, -1, source1);
	h2p = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos2 + offset2, -1, source2);
	w1p = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, ipos1 + offset1, -1, source1);
	if(
		h1p && h2p &&
		w1p == GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, ipos2 + offset2, -1, source2) &&
		GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, ipos1 + offset1, -1, source1) == GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, ipos2 + offset2, -1, source2)
	)
	{
		w2p = GetStackValue(w1p);
		// add stack of ipos2 to ipos1
		if(h1p + h2p <= w2p) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos1 + offset1, -1, h1p + h2p, source1);
			SyncItemStack(pnum, ipos1 + offset1, source1);
			// dispose of 2nd item
			FreeItem(pnum, ipos2 + offset2, source2, false);
		}
		else {
			// set stack of h1p to max, then set the stack of ipos2 to whatever is left
			SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos2 + offset2, -1, h2p - w2p + h1p, source2);
			SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos1 + offset1, -1, w2p, source1);
			SyncItemStack(pnum, ipos1 + offset1, source1);
			SyncItemStack(pnum, ipos2 + offset2, source2);
		}
	}
	else if(forcedSwap || CanSwapItems(pnum, ipos1, ipos2, offset1, offset2, source1, source2)) {
		MoveItemToTemporary(pnum, ipos1 + offset1, 0, source1);
		MoveItemToTemporary(pnum, ipos2 + offset2, 1, source2);
		
		FreeItem(pnum, ipos1 + offset1, source1, false);
		FreeItem(pnum, ipos2 + offset2, source2, false);
		
		CopyItemFromTemporary(pnum, ipos1 + offset2, 1, source1);
		CopyItemFromTemporary(pnum, ipos2 + offset1, 0, source2);
		
		// special version calls scripts with 1 tic delay, because apparently script execution order in zandronum is so SHIT, the ones in FreeItem aren't even
		// guaranteed to run before the scripts called from below
		SyncItemData_Special(pnum, ipos1 + offset2, source1);
		SyncItemData_Special(pnum, ipos2 + offset1, source2);
		
		// update orb item pointer
		if(Player_MostRecent_Orb[pnum].p_tempwep == ipos1)
			Player_MostRecent_Orb[pnum].p_tempwep = ipos2;
		else if(Player_MostRecent_Orb[pnum].p_tempwep == ipos2)
			Player_MostRecent_Orb[pnum].p_tempwep = ipos1;
	}
}

/*
	0  1  2  3  4
	9 10 11 12 13
   18 19 20 21 22
   27 28 29 30 31
*/

bool InventoryBoxContainsPoint(int point, int boxleft, int w, int h) {
	// for every line, check if point falls into a linear range
	for(int i = 0; i < h; ++i)
		if(boxleft + i * MAXINVENTORYBLOCKS_VERT <= point && boxleft + i * MAXINVENTORYBLOCKS_VERT + w - 1 >= point)
			return true;
	return false;
}

void MoveItem(int pnum, int itempos, int emptypos) {
	int tb = PlayerInventoryList[pnum][itempos].topleftboxid - 1;
	int offset = tb - itempos;
	
	int i, j, bid;
	
	int w = PlayerInventoryList[pnum][tb].width;
	int h = PlayerInventoryList[pnum][tb].height;

	// these two mark box ids that have been modified, ie. need updates
	// if the boxes aren't in range of itempos, they will need to be nulled because we no longer need to preserve the data in that spot
	int set1 = 0;
	int set2 = 0;
	
	int temp = emptypos + offset;
	
	for(i = 0; i < h; ++i) 
		for(j = 0; j < w; ++j) {
			bid = tb + j + i * MAXINVENTORYBLOCKS_VERT;
			if(!InventoryBoxContainsPoint(bid, temp, w, h)) {
				if(bid < 32)
					set1 |= 1 << bid;
				else
					set2 |= 1 << (bid - 32);
			}
		}
	
	PlayerInventoryList[pnum][temp].width = w;
	PlayerInventoryList[pnum][temp].height = h;
	PlayerInventoryList[pnum][temp].item_subtype = PlayerInventoryList[pnum][tb].item_subtype;
	PlayerInventoryList[pnum][temp].item_image = PlayerInventoryList[pnum][tb].item_image;
	PlayerInventoryList[pnum][temp].item_level = PlayerInventoryList[pnum][tb].item_level;
	PlayerInventoryList[pnum][temp].item_stack = PlayerInventoryList[pnum][tb].item_stack;
	PlayerInventoryList[pnum][temp].attrib_count = PlayerInventoryList[pnum][tb].attrib_count;

	PlayerInventoryList[pnum][temp].corrupted = PlayerInventoryList[pnum][tb].corrupted;
	PlayerInventoryList[pnum][temp].quality = PlayerInventoryList[pnum][tb].quality;

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		PlayerInventoryList[pnum][temp].implicit[i].attrib_id = PlayerInventoryList[pnum][tb].implicit[i].attrib_id;
		PlayerInventoryList[pnum][temp].implicit[i].attrib_val = PlayerInventoryList[pnum][tb].implicit[i].attrib_val;
		PlayerInventoryList[pnum][temp].implicit[i].attrib_tier = PlayerInventoryList[pnum][tb].implicit[i].attrib_tier;
		PlayerInventoryList[pnum][temp].implicit[i].attrib_extra = PlayerInventoryList[pnum][tb].implicit[i].attrib_extra;
	}

	PlayerInventoryList[pnum][temp].isDirty = PlayerInventoryList[pnum][tb].isDirty;
	PlayerInventoryList[pnum][temp].textID = PlayerInventoryList[pnum][tb].textID;

	for(i = 0; i < PlayerInventoryList[pnum][temp].attrib_count; ++i) {
		PlayerInventoryList[pnum][temp].attributes[i].attrib_id = PlayerInventoryList[pnum][tb].attributes[i].attrib_id;
		PlayerInventoryList[pnum][temp].attributes[i].attrib_val = PlayerInventoryList[pnum][tb].attributes[i].attrib_val;
		PlayerInventoryList[pnum][temp].attributes[i].attrib_tier = PlayerInventoryList[pnum][tb].attributes[i].attrib_tier;
		PlayerInventoryList[pnum][temp].attributes[i].attrib_extra = PlayerInventoryList[pnum][tb].attributes[i].attrib_extra;
		PlayerInventoryList[pnum][temp].attributes[i].fractured = PlayerInventoryList[pnum][tb].attributes[i].fractured;
	}
	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			PlayerInventoryList[pnum][temp + i * MAXINVENTORYBLOCKS_VERT + j].item_type = PlayerInventoryList[pnum][tb].item_type;
			PlayerInventoryList[pnum][temp + i * MAXINVENTORYBLOCKS_VERT + j].topleftboxid = temp + 1;
		}

	if(set1 || set2) {
		for(i = 0; i < 32; ++i)
			if(IsSet(set1, i)) {
				//printbold(s:"null req on ", d:i);
				FreeSpot(pnum, i, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			}
				
		for(i = 0; i < MAX_INVENTORY_BOXES - 32; ++i)
			if(IsSet(set2, i)) {
				//printbold(s:"null req on ", d:i+32);
				FreeSpot(pnum, i + 32, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			}
		SyncItemData_Null(pnum, tb, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, w, h);
	}
	/*else {
		// Simply null the leftover spot, no collision happened
		FreeItem(pnum, tb, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	}*/
	
	// update orb item pointer
	if(Player_MostRecent_Orb[pnum].p_tempwep == itempos)
		Player_MostRecent_Orb[pnum].p_tempwep = emptypos;
	
	SyncItemData_Special(pnum, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

bool AutoMoveItem(int pnum, int boxid, int isource, int ssource, bool noSync = false) {
	bool res = false;

	int tpbid = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, boxid, -1, isource) - 1;
	int i, j;
	int try_pos = -1;

	// if this is a stacked item, scan the destination for any of this item to add on top, if we can
	// reduce stack count if we couldn't, and if we could, we're done
	int itype = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, tpbid, -1, isource);
	int isubtype = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, tpbid, -1, isource);
	int maxstacks = GetStackValue(itype), istacks = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, tpbid, -1, isource);
	int stype;
	if(IsStackedItem(itype)) {
		for(j = 0; !res && j < MAXINVENTORYBLOCKS_VERT; ++j) {
			for(i = 0; !res && i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
				try_pos = j + i * MAXINVENTORYBLOCKS_VERT;
				// check destination for same item to potentially put it onto it
				stype = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, try_pos, -1, ssource);
				if
				(
					stype == itype &&
					GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, try_pos, -1, ssource) == isubtype
				) 
				{
					// move stacks of what we got to this one and end the operation
					// check if we have reached max, if we did, then we must proceed further below
					stype = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, try_pos, -1, ssource);
					if(stype + istacks <= maxstacks) {
						SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, try_pos, -1, stype + istacks, ssource);
						SyncItemStack(pnum, try_pos, ssource);
						FreeItem(pnum, tpbid, isource, false);
						return true;
					}
					else if(stype != maxstacks) {
						// set target one to max, and ours max - what we had
						SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, try_pos, -1, maxstacks, ssource);
						SyncItemStack(pnum, try_pos, ssource);

						// dont sync this, we'll move it later anyways then the real sync will occur
						SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, tpbid, -1, istacks - (maxstacks - stype), isource);
						res = true;
					}
				}
			}
		}
	}

	// if res was true before, make it false on the stacked item case for 2nd condition occuring
	res = false;

	// scan from vertical positions to horizontal first to find a good spot
	for(j = 0; !res && j < MAXINVENTORYBLOCKS_VERT; ++j) {
		for(i = 0; !res && i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
			try_pos = j + i * MAXINVENTORYBLOCKS_VERT;
			if(IsFreeSpot(pnum, tpbid, try_pos, isource, ssource)) {
				// move item to here now
				MoveItemTrade(pnum, tpbid, try_pos, isource, ssource, noSync);
				res = true;
			}
		}
	}

	return res;
}

// auto dump functionality from inventory of player to stash
void AutoDumpItems(int pnum, int orbsOnly = 0) {
	// for each item the player has, attempt to AutoMoveItem them to stash
	// first store item ids in an array, sorted from biggest to shortest (biggest first in list)
	static bool marked_tbids[MAX_INVENTORY_BOXES];

	int i, j, k, count = 0;

	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		marked_tbids[i] = false;
		ItemMoveList[pnum][i].dest_pos = -1;
	}

	// ItemMoveList contains topboxids of items to be moved in order of largest to smallest size occupying (w * h)
	// insert sorted
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(PlayerInventoryList[pnum][i].item_type != DND_ITEM_NULL && (!orbsOnly || PlayerInventoryList[pnum][i].item_type == DND_ITEM_ORB) && !marked_tbids[i]) {
			k = 0;
			j = 0;
			while(ItemMoveList[pnum][k].dest_pos != -1) {
				if(ItemMoveList[pnum][k].width * ItemMoveList[pnum][k].height < PlayerInventoryList[pnum][i].width * PlayerInventoryList[pnum][i].height) {
					// need to shift items right starting from k
					j = 1;
					break;
				}
				++k;
			}

			// add it here, but check if this current location is actually occupied!
			if(j) {
				// shift then add to where k is
				//printbold(s:"shifting item at ", d:k, s: " right");
				for(j = count - 1; j >= k; --j) {
					//printbold(s:"move ", d:j, s: " to ", d:j + 1, s: " item dest_pos: ", d:ItemMoveList[pnum][j + 1].dest_pos, s: " to ", d:ItemMoveList[pnum][j].dest_pos);
					ItemMoveList[pnum][j + 1].dest_pos = ItemMoveList[pnum][j].dest_pos;
					ItemMoveList[pnum][j + 1].width = ItemMoveList[pnum][j].width;
					ItemMoveList[pnum][j + 1].height = ItemMoveList[pnum][j].height;
				}
			}

			// add here
			//printbold(s:"insert to movelist ", d:k, s: " item at ", d:i, s:" w and h: ", d:PlayerInventoryList[pnum][i].width, s: " ", d:PlayerInventoryList[pnum][i].height);
			ItemMoveList[pnum][k].dest_pos = PlayerInventoryList[pnum][i].topleftboxid;
			ItemMoveList[pnum][k].width = PlayerInventoryList[pnum][i].width;
			ItemMoveList[pnum][k].height = PlayerInventoryList[pnum][i].height;

			++count;

			// mark them for quick checks so we dont keep checking these
			for(k = 0; k < PlayerInventoryList[pnum][i].width; ++k) 
				for(j = 0; j < PlayerInventoryList[pnum][i].height; ++j)
					marked_tbids[i + k  + j * MAXINVENTORYBLOCKS_VERT] = true;
		}
	}

	//printbold(s:"count of items to move: ", d:count);

	// we formed the list of items to be send to stash, sorted wrt size, now just send them over
	for(i = 0; i < count; ++i) {
		//printbold(s:"item: ", d:i, s: " w and h: ", d:ItemMoveList[pnum][i].width, s: " ", d:ItemMoveList[pnum][i].height, s:" move item pos: ", d:ItemMoveList[pnum][i].dest_pos - 1);
		AutoMoveItem(pnum, ItemMoveList[pnum][i].dest_pos - 1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, DND_SYNC_ITEMSOURCE_STASH | ((CheckInventory("DnD_PlayerCurrentPage") - 1) << 16));
	}

	// sync entire inventory and stash after success
}

// this is made specifically for trade view, the one above is optimized for normal inventory
// also used for moving items from one source to another, ie. inventory to stash etc.
void MoveItemTrade(int pnum, int itempos, int emptypos, int itemsource, int emptysource, bool noSync = false) {
	int tb = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, itemsource) - 1;
	int offset = tb - itempos;
	
	//printbold(s:"will move tbid ", d:tb, s: " offset ", d:offset, s:" to loc: ", d:emptypos, s: "isrc: ", d:itemsource, s: " esrc: ", d:emptysource);

	int i, j, bid;
	
	int w = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, tb, -1, itemsource);
	int h = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, tb, -1, itemsource);

	// these two mark box ids that have been modified, ie. need updates
	// if the boxes aren't in range of itempos, they will need to be nulled
	int set1 = 0;
	int set2 = 0;
	
	int temp = emptypos + offset;
	
	// only check for this if they have the same source (overlap can only happen if so)
	// this also fixes a corner case bug
	if(itemsource == emptysource) {
		for(i = 0; i < h; ++i) 
			for(j = 0; j < w; ++j) {
				bid = tb + j + i * MAXINVENTORYBLOCKS_VERT;
				if(!InventoryBoxContainsPoint(bid, temp, w, h)) {
					if(bid < 32)
						set1 |= 1 << (bid);
					else
						set2 |= 1 << (bid - 32);
				}
			}
	}

	SetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, temp, -1, w, emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, temp, -1, h, emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, tb, -1, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, tb, -1, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, tb, -1, itemsource), emptysource);

	SetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, tb, -1, itemsource), emptysource);
	SetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, temp, -1, GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, tb, -1, itemsource), emptysource);

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, tb, i, itemsource), emptysource);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, tb, i, itemsource), emptysource);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, tb, i, itemsource), emptysource);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, tb, i, itemsource), emptysource);
	}

	SetItemSyncValue(pnum, DND_SYNC_ISDIRTY, temp, -1, true/*GetItemSyncValue(pnum, DND_SYNC_ISDIRTY, tb, -1, itemsource)*/, emptysource);
	SetItemSyncValue(pnum, DND_SYNC_TEXTID, temp, -1, GetItemSyncValue(pnum, DND_SYNC_TEXTID, tb, -1, itemsource), emptysource);

	bid = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, emptysource);
	for(i = 0; i < bid; ++i) {
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, tb, i, itemsource), emptysource);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, tb, i, itemsource), emptysource);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, tb, i, itemsource), emptysource);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, tb, i, itemsource), emptysource);
		SetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, temp, i, GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, tb, i, itemsource), emptysource);
	}
	bid = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, tb, -1, itemsource);
	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, bid, emptysource);
			SetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, temp + 1, emptysource);
		}

	if(set1 || set2) {
		for(i = 0; i < 32; ++i)
			if(IsSet(set1, i))
				FreeSpot(pnum, i, itemsource);
			
		for(i = 0; i < MAX_INVENTORY_BOXES - 32; ++i)
			if(IsSet(set2, i))
				FreeSpot(pnum, i + 32, itemsource);
				
		SyncItemData_Null(pnum, tb, itemsource, w, h);
	}
	else {
		// Simply null the leftover spot, no collision happened
		FreeItem(pnum, tb, itemsource, false);
	}
	
	// as soon as this item is offered for a trade it can't be edited
	if(Player_MostRecent_Orb[pnum].p_tempwep == itempos && itemsource == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY)
		Player_MostRecent_Orb[pnum].p_tempwep = 0;
	
	if(!noSync)
		SyncItemData_Special(pnum, temp, emptysource);
}

// this simply carries an item from another player's place to another, like moveitem but has player inputs
// from p_item to p_empty
void CarryItemTo(int itempos, int emptypos, int itemsource, int emptysource, int p_item, int p_empty) {
	int tb = GetItemSyncValue(p_item, DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, itemsource) - 1;
	int offset = tb - itempos;
	
	int i, j, bid;
	
	int w = GetItemSyncValue(p_item, DND_SYNC_ITEMWIDTH, tb, -1, itemsource);
	int h = GetItemSyncValue(p_item, DND_SYNC_ITEMHEIGHT, tb, -1, itemsource);
	
	int temp = emptypos + offset;
	
	//printbold(s:"carry item of player ", d:p_item, s: " to pos ", d:emptypos);
	
	SetItemSyncValue(p_empty, DND_SYNC_ITEMWIDTH, temp, -1, w, emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMHEIGHT, temp, -1, h, emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMSUBTYPE, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMSUBTYPE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMIMAGE, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMIMAGE, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMLEVEL, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMLEVEL, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMSTACK, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMSTACK, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMSATTRIBCOUNT, tb, -1, itemsource), emptysource);

	SetItemSyncValue(p_empty, DND_SYNC_ITEMCORRUPTED, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMCORRUPTED, tb, -1, itemsource), emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_ITEMQUALITY, temp, -1, GetItemSyncValue(p_item, DND_SYNC_ITEMQUALITY, tb, -1, itemsource), emptysource);

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, tb, i, itemsource), emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, tb, i, itemsource), emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, tb, i, itemsource), emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, tb, i, itemsource), emptysource);
	}

	SetItemSyncValue(p_empty, DND_SYNC_ISDIRTY, temp, -1, true, emptysource);
	SetItemSyncValue(p_empty, DND_SYNC_TEXTID, temp, -1, 0, emptysource);

	bid = GetItemSyncValue(p_empty, DND_SYNC_ITEMSATTRIBCOUNT, temp, -1, emptysource);
	for(i = 0; i < bid; ++i) {
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_ID, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_ID, tb, i, itemsource), emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_VAL, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_VAL, tb, i, itemsource), emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_TIER, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_TIER, tb, i, itemsource), emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_EXTRA, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_EXTRA, tb, i, itemsource), emptysource);
		SetItemSyncValue(p_empty, DND_SYNC_ITEMATTRIBUTES_FRACTURE, temp, i, GetItemSyncValue(p_item, DND_SYNC_ITEMATTRIBUTES_FRACTURE, tb, i, itemsource), emptysource);
	}
	bid = GetItemSyncValue(p_item, DND_SYNC_ITEMTYPE, tb, -1, itemsource);
	for(i = 0; i < h; ++i)
		for(j = 0; j < w; ++j) {
			SetItemSyncValue(p_empty, DND_SYNC_ITEMTYPE, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, bid, emptysource);
			SetItemSyncValue(p_empty, DND_SYNC_ITEMTOPLEFTBOX, temp + i * MAXINVENTORYBLOCKS_VERT + j, -1, temp + 1, emptysource);
		}	

	FreeItem(p_item, tb, itemsource, false);
	SyncItemData(p_empty, temp, emptysource, -1, -1);
}

void TransferTradeItems(int from, int to) {
	int bid;
	int i, j, h, w;
	// for every possible item in the trade list of this player, get free position
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			bid = j + i * MAXINVENTORYBLOCKS_VERT;
			// care about the items only once, so use topleftboxid == bid
			if(TradeViewList[from][bid].topleftboxid - 1 == bid) {
				//printbold(s:"carry item to ", d:ItemMoveList[to][bid].dest_pos, s: " from player ", d:from, s:"'s movelist to player ", d:to);
				CarryItemTo(bid, ItemMoveList[to][bid].dest_pos, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, from, to);
			}
		}
	}
	
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		ItemMoveList[to][i].state = false;
		ItemMoveList[to][i].width = 0;
		ItemMoveList[to][i].height = 0;
		ItemMoveList[to][i].dest_pos = -1;
	}
}

int GetInventoryInfoOffset(int itype) {
	switch(itype) {
		case DND_ITEM_CHESTKEY:
		return CHESTKEY_BEGIN;
		case DND_ITEM_ORB:
		return ORBS_BEGIN;
		case DND_ITEM_TOKEN:
		return TOKEN_BEGIN;
	}
	return 0;
}

// outside source implies this was called from the menu, not in the game
void DrawInventoryInfo_Field(int pnum, int topboxid, int source, int yoff, bool isOutsideSource) {
	DeleteTextRange(RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 18 - ITEMINFOBG_MAXMIDS, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES);
	
	// dont draw in custom intermission
	if(CheckInventory("DnD_IntermissionState"))
		return;
	
	int stack, itype = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, topboxid, -1, source), left = 0;
	if(topboxid != -1 && itype != DND_ITEM_NULL) {
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
		
		// adjust pos
		left = GetHudLeft(HUDMAX_X) << 16;
		int bg_x = GetIntegerBits(left) + 0.1;
		int bg_y = isOutsideSource ? 0.1 : GetIntegerBits(100.0 - yoff) + 0.1;
		int img_off = 0;
		
		// if attribute count + potential implicit existing is less than 3, it will fit on this rectangle
		int attr_count = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, topboxid, -1, source);

		// center text by making it 0.4 and 0.1 on y
		int bx = bg_x + GetIntegerBits(HUD_ITEMBAK_XF / 2) + 0.3;
		int by = isOutsideSource ? 0.1 : GetIntegerBits(HUDMAX_YF / 2) - yoff;
		
		if(!isOutsideSource)
			by = by - 70.9;
		else
			by += 16.0;
		
		int isubt = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, topboxid, -1, source);
		
		// show item details -- image and stack --- show image to the right side of the box on outside
		SetFont(GetItemImage(GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, topboxid, -1, source)));
		HudMessage(
			s:"A"; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 1 - ITEMINFOBG_MAXMIDS, CR_WHITE, 
			!isOutsideSource ? (GetIntegerBits(bg_x) + HUD_ITEMBAK_XF + 0.1) : bx, 
			!isOutsideSource ? bg_y : (GetIntegerBits(by) + 64.2), 
			INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);
		stack = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, topboxid, -1, source);
		if(stack) {
			SetFont("NSMOLFNT");
			HudMessage(d:stack; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_FIELD_MULT * MAX_INVENTORY_BOXES - 14 - ITEMINFOBG_MAXMIDS, CR_GREEN, bx + GetIntegerBits(HUD_ITEMBAK_XF / 2) - 7.2, by + 24.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
		
		// resize to keep ratios
		SetHudSize(HUDTEXTMAX_X, HUDTEXTMAX_Y, 1);
		left = GetHudLeft(HUDTEXTMAX_X);
		bx = ((left + (HUD_ITEMBAK_X * HUDTEXTMAX_X / (2 * HUDMAX_X))) << 16) + 0.4;
		by = isOutsideSource ? 20.1 : GetIntegerBits(HUDTEXTMAX_YF / 2 - HUD_ITEMBAK_YF / 2 - yoff - 30.0) + 0.1;

		if(/*lessThanFour && */!isOutsideSource)
			by += 36.0;

		SetHudClipRect(left + 12, (by >> 16), HUD_ITEMBAK_WIDTH, 288, HUD_ITEMBAK_WIDTH);
		DrawInventoryText(topboxid, source, pnum, bx, by, itype, isubt, RPGMENUINVENTORYID, HUD_DII_FIELD_MULT, HUDMAX_X, HUDMAX_Y, bg_x, bg_y, attr_count, !isOutsideSource);
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	}
}

void DrawInventoryText(int topboxid, int source, int pnum, int bx, int by, int itype, int isubt, int id_begin, int id_mult, int hx, int hy, int bg_posx, int bg_posy, int attr_count = 0, bool isOutsideSource = false) {
	int i, j;
	int val, temp, lvl;
	int yoff = 0.0;
	bool showModTiers = GetCVar("dnd_detailedmods");
	bool isUnique = false;
	int lines_count = 0;

	str tmp_text;
	str unique_creator = "";
	
	// potential delete of quality in case we hover over an item that doesn't have it, we don't want it lingering!
	DeleteText(id_begin - id_mult * MAX_INVENTORY_BOXES - 18);

	SetFont("NSMOLFNT");
	if(IsStackedItem(itype)) {
		temp = isubt + GetInventoryInfoOffset(itype);
		if(GetItemSyncValue(pnum, DND_SYNC_ISDIRTY, topboxid, -1, source)) {
			tmp_text = StrParam(s:"\c[Y5]", l:GetInventoryTag(temp), s:"\n\n", l:GetInventoryText(temp));
			SetItemSyncValue(pnum, DND_SYNC_ISDIRTY, topboxid, -1, 0, source);
			SetItemSyncValue(pnum, DND_SYNC_TEXTID, topboxid, -1, tmp_text, source);
		}
		else
			tmp_text = GetItemSyncValue(pnum, DND_SYNC_TEXTID, topboxid, -1, source);

		HudMessage(
				s:tmp_text;
				HUDMSG_PLAIN | HUDMSG_FADEOUT,
				id_begin - id_mult * MAX_INVENTORY_BOXES - 3 - ITEMINFOBG_MAXMIDS,
				CR_WHITE, 
				bx, 
				by + 32.0 * (source == DND_SYNC_ITEMSOURCE_FIELD) - 32.0 * isOutsideSource, 
				INVENTORY_HOLDTIME, 
				INVENTORY_FADETIME, 
				INVENTORY_INFO_ALPHA
		);
		DrawItemInfoBackground(id_begin - id_mult * MAX_INVENTORY_BOXES, hx, hy, bg_posx, bg_posy, CountNewLinesInText(tmp_text, NEXT_LINE_LEN_ATTR + 8));
		return;
	}
	else {
		// this holds charm's tier id
		lvl = GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, topboxid, -1, source);
		
		if(itype == DND_ITEM_CHARM) {
			temp = GetItemTier(lvl);
			HudMessage(s:Charm_Strings[temp][CHARMSTR_COLORCODE], l:Charm_Strings[temp][CHARMSTR_TIERTAG], s: " ", l:GetItemTagName(DND_ITEM_CHARM, isubt); 
				HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 2 - ITEMINFOBG_MAXMIDS, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
			);
		}
		else if(itype <= UNIQUE_BEGIN) {
			HudMessage(s:"\c[Y5]", s:GetItemTagName(itype, isubt); 
				HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 2 - ITEMINFOBG_MAXMIDS, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
			);
		}
		else {
			// unique item case
			temp = itype & 0xFFFF;
			itype >>= UNIQUE_BITS;
			--itype;
			isUnique = true;

			unique_creator = GetUniqueCreatorName(temp, itype);
			HudMessage(s:"\c[A1]", l:GetUniqueItemName(temp, itype); HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 2 - ITEMINFOBG_MAXMIDS, CR_WHITE, bx, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
			HudMessage(s:"\c[D1]", l:"DND_ITEM_UNIQUE", s:" ", l:GetItemTagName(temp, isubt); HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 3 - ITEMINFOBG_MAXMIDS, CR_WHITE, bx, by + 8.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
		
		val = GetLevel() < lvl ? CR_RED : CR_WHITE;
		HudMessage(l:"DND_LEVEL_HEADER", s:": ", d:lvl; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 4 - ITEMINFOBG_MAXMIDS, val, GetIntegerBits(bx - HUD_ITEMBAK_XF / 2 - 36.0) + 0.1, by, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);

		// quality -- non-stacking item so
		val = GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, topboxid, -1, source);
		HudMessage(s:"\c[Q2]Q: ", d:val, s:"%";
			HUDMSG_PLAIN | HUDMSG_FADEOUT, 
			id_begin - id_mult * MAX_INVENTORY_BOXES - 18 - ITEMINFOBG_MAXMIDS, 
			CR_GREEN, 
			GetIntegerBits(bx + ((2 * HUD_ITEMBAK_X / 3) << 16)) + 8.2, 
			by, 
			INVENTORY_HOLDTIME, 
			INVENTORY_FADETIME, 
			INVENTORY_INFO_ALPHA
		);

		// implicit
		tmp_text = "";
		for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
			temp = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, topboxid, i, source);
			if(temp != -1) {
				if(isUnique)
					yoff = 6.0;

				if(tmp_text != "")
					tmp_text = StrParam(s:tmp_text, s:"\n");
				
				tmp_text = StrParam(s:tmp_text,
					s:GetItemAttributeText(
						temp, 
						itype, 
						isubt, 
						GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, topboxid, i, source), 
						0, 
						GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, topboxid, i, source), 
						false, // we dont want detailed info on implicits, it doesn't make much sense currently
						GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, topboxid, i, source), 
						false,
						val
					)
				);
			}
		}

		if(tmp_text != "") {
			HudMessage(
				s:tmp_text;
				HUDMSG_PLAIN | HUDMSG_FADEOUT, 
				id_begin - id_mult * MAX_INVENTORY_BOXES - 5 - ITEMINFOBG_MAXMIDS, CR_WHITE, bx, by + 10.0 + yoff, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
			);

			temp = CountNewLinesInText(tmp_text, NEXT_LINE_LEN_ATTR + 6); // was 10
			yoff = 10.0 + 8.0 * temp;
			lines_count += max(1, temp);
		}

		by += 12.0 + 8.0 * isUnique;

		SetFont("IMPSEPR");
		HudMessage(
			s:"A"; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, id_begin - id_mult * MAX_INVENTORY_BOXES - 6 - ITEMINFOBG_MAXMIDS, val, GetIntegerBits(bx) + 0.4, by + yoff, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);

		by += 8.0;

		// optimization for the potentially busy section with strparam spam
		if(GetItemSyncValue(pnum, DND_SYNC_ISDIRTY, topboxid, -1, source) || GetItemSyncValue(pnum, DND_SYNC_LASTSHOWNTEXTMODE, topboxid, -1, source) != showModTiers) {
			tmp_text = "";
			for(j = 0; j < attr_count; ++j) {
				temp = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, topboxid, j, source);
				lvl = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_TIER, topboxid, j, source);
				if(isUnique)
					lvl = itype;
	
				tmp_text = StrParam(s:tmp_text,
					s:GetItemAttributeText(
						temp, 
						itype, 
						isubt, 
						GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, topboxid, j, source), 
						GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, topboxid, j, source), 
						lvl, 
						showModTiers, 
						!isUnique ? -1 : j, 
						GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_FRACTURE, topboxid, j, source),
						val
					),
					s: (j != attr_count - 1) ? "\n" : ""
				);
			}
	
			// corrupted label and seperator
			isUnique = GetItemSyncValue(pnum, DND_SYNC_ITEMCORRUPTED, topboxid, -1, source);
			if(isUnique) {
				tmp_text = StrParam(s:tmp_text, s:"\n\n\cgCORRUPTED");
				++lines_count;
			}
	
			if(unique_creator != "") {
				tmp_text = StrParam(s:tmp_text, s:"\n\n", l:"DND_MADE_BY", s:": \cd", s:unique_creator);
				++lines_count;
			}

			SetItemSyncValue(pnum, DND_SYNC_ISDIRTY, topboxid, -1, false, source);
			SetItemSyncValue(pnum, DND_SYNC_LASTSHOWNTEXTMODE, topboxid, -1, showModTiers, source);
			SetItemSyncValue(pnum, DND_SYNC_TEXTID, topboxid, -1, tmp_text, source);
		}
		else
			tmp_text = GetItemSyncValue(pnum, DND_SYNC_TEXTID, topboxid, -1, source);

		SetFont("NSMOLFNT");
		HudMessage(
			s:tmp_text; 
			HUDMSG_PLAIN | HUDMSG_FADEOUT, 
			id_begin - id_mult * MAX_INVENTORY_BOXES - 7 - ITEMINFOBG_MAXMIDS, CR_WHITE, bx, by + yoff, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA
		);

		temp = CountNewLinesInText(tmp_text, NEXT_LINE_LEN_ATTR + 15);
		yoff += 8.0 * temp;
		lines_count += max(1, temp);
	}

	DrawItemInfoBackground(id_begin - id_mult * MAX_INVENTORY_BOXES, hx, hy, bg_posx, bg_posy, lines_count);
}

void DrawItemInfoBackground(int hudid_begin, int hx, int hy, int bg_posx, int bg_posy, int lines_count) {
	SetHudClipRect(0, 0, 0, 0, 0);

	// finally draw the background -- use hx and hy to set hudsize
	SetHudSize(hx, hy, 1);
	// per amount of this many lines, get a new midsection in
	int midcount = 1 + ((lines_count * 0.525) >> 16);

	SetFont("LDTITTOP");
	HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_FADEOUT, hudid_begin, CR_WHITE, bg_posx, bg_posy, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);

	for(int i = 0; i < midcount; ++i) {
		SetFont("LDTITMID");
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_FADEOUT, hudid_begin - 1 - i, CR_WHITE, bg_posx, bg_posy + ITEMINFOBG_TOPLEN + i * ITEMINFOBG_MIDLEN, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
	}

	SetFont("LDTITBOT");
	HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_FADEOUT, hudid_begin - 2 - i, CR_WHITE, bg_posx, bg_posy + ITEMINFOBG_TOPLEN + i * ITEMINFOBG_MIDLEN, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
}

void CopyItemToField(int fieldpos, int player_index, int item_index, int source) {
	int i, wtemp;
	wtemp = GetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, item_index, -1, source) - 1;
	Inventories_On_Field[fieldpos].width = GetItemSyncValue(player_index, DND_SYNC_ITEMWIDTH, wtemp, -1, source);
	Inventories_On_Field[fieldpos].height = GetItemSyncValue(player_index, DND_SYNC_ITEMHEIGHT, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_type = GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_subtype = GetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_image = GetItemSyncValue(player_index, DND_SYNC_ITEMIMAGE, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_level = GetItemSyncValue(player_index, DND_SYNC_ITEMLEVEL, wtemp, -1, source);
	Inventories_On_Field[fieldpos].item_stack = GetItemSyncValue(player_index, DND_SYNC_ITEMSTACK, wtemp, -1, source);
	Inventories_On_Field[fieldpos].attrib_count = GetItemSyncValue(player_index, DND_SYNC_ITEMSATTRIBCOUNT, wtemp, -1, source);

	Inventories_On_Field[fieldpos].corrupted = GetItemSyncValue(player_index, DND_SYNC_ITEMCORRUPTED, wtemp, -1, source);
	Inventories_On_Field[fieldpos].quality = GetItemSyncValue(player_index, DND_SYNC_ITEMQUALITY, wtemp, -1, source);

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		Inventories_On_Field[fieldpos].implicit[i].attrib_id = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, wtemp, i, source);
		Inventories_On_Field[fieldpos].implicit[i].attrib_val = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, wtemp, i, source);
		Inventories_On_Field[fieldpos].implicit[i].attrib_tier = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_TIER, wtemp, i, source);
		Inventories_On_Field[fieldpos].implicit[i].attrib_extra = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, wtemp, i, source);
	}

	Inventories_On_Field[fieldpos].isDirty = true;
	Inventories_On_Field[fieldpos].textID = 0;

	Inventories_On_Field[fieldpos].topleftboxid = 0;
	for(i = 0; i < Inventories_On_Field[fieldpos].attrib_count; ++i) {
		Inventories_On_Field[fieldpos].attributes[i].attrib_id = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_ID, wtemp, i, source);
		Inventories_On_Field[fieldpos].attributes[i].attrib_val = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_VAL, wtemp, i, source);
		Inventories_On_Field[fieldpos].attributes[i].attrib_tier = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_TIER, wtemp, i, source);
		Inventories_On_Field[fieldpos].attributes[i].attrib_extra = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_EXTRA, wtemp, i, source);
		Inventories_On_Field[fieldpos].attributes[i].fractured = GetItemSyncValue(player_index, DND_SYNC_ITEMATTRIBUTES_FRACTURE, wtemp, i, source);
	}

	// the leftover spot is a null item
	FreeItem(player_index, wtemp, source, false);
	SyncItemData_Field(fieldpos);
}

void DropItemToField(int player_index, int pitem_index, bool forAll, int source) {
	int c = CreateItemSpot();
	
	// note to self here: subtype should always be looked up on TOPLEFTBOX property... that one index only has the fully synced info, rest dont
	int itype = GetItemSyncValue(player_index, DND_SYNC_ITEMTYPE, pitem_index, -1, source);
	int topboxid = GetItemSyncValue(player_index, DND_SYNC_ITEMTOPLEFTBOX, pitem_index, -1, source) - 1;
	int stype = GetItemSyncValue(player_index, DND_SYNC_ITEMSUBTYPE, topboxid, -1, source);

	// copy now
	CopyItemToField(c, player_index, pitem_index, source);
	str droptype = "CharmDrop";
	if(itype > UNIQUE_BEGIN) {
		int utype = (itype >> 16) - 1;
		itype &= 0xFFFF;
		if(itype == DND_ITEM_CHARM)
			droptype = "UniqueCharmDrop";
		else if(itype == DND_ITEM_BODYARMOR)
			droptype = StrParam(s:"UniqueArmor_", d:utype - UNIQUE_BODYARMOR_BEGIN);
		else if(itype == DND_ITEM_SPECIALTY_CYBORG)
			droptype = StrParam(s:"PowercoreDrop_Unique", d:utype - UNIQUE_POWERCORE_BEGIN);
	}
	else if(itype == DND_ITEM_ORB)
		droptype = GetInventoryName(stype + ORBS_BEGIN);
	else if(itype == DND_ITEM_CHESTKEY)
		droptype = GetInventoryName(stype + CHESTKEY_BEGIN);
	else if(itype == DND_ITEM_TOKEN)
		droptype = GetInventoryName(stype + TOKEN_BEGIN);
	else if(itype == DND_ITEM_BODYARMOR)
		droptype = GetArmorDropClass(stype);
	else if(itype == DND_ITEM_BOOT)
		droptype = GetBootDropClass(stype);
	else if(itype == DND_ITEM_HELM)
		droptype = GetHelmDropClass(stype);
	else if(itype == DND_ITEM_SPECIALTY_CYBORG)
		droptype = GetPowercoreDropClass(stype);
	forAll ? SpawnDropFacing(droptype, 16.0, 16, 256, c) : SpawnDropFacing(droptype, 16.0, 16, player_index + 1, c);
}

void StackedItemPickupCS(int item_index, int type) {
	if(type == DND_STACKEDITEM_ORB)
		ACS_NamedExecuteAlways("DnD Orb Message", 0, Inventories_On_Field[item_index].item_subtype);
	else if(type == DND_STACKEDITEM_CHESTKEY)
		ACS_NamedExecuteAlways("DnD Chestkey Message", 0, Inventories_On_Field[item_index].item_subtype);
	else if(type == DND_STACKEDITEM_TOKEN)
		ACS_NamedExecuteAlways("DnD Token Message", 0, Inventories_On_Field[item_index].item_subtype);
}

// move this from field to player's inventory
int HandleStackedPickup(int item_index, int type) {
	// make sure this item actually gets placed on top of an item that has some stack, if any
	// print message first, if its moved to inv it's gone from there
	int pnum = PlayerNumber();
	StackedItemPickupCS(item_index, type);
	
	int porb_index = GetFreeSpotForItemWithStack(item_index, pnum, DND_SYNC_ITEMSOURCE_FIELD, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
	return CopyItemFromFieldToPlayer(item_index, pnum, porb_index, type);
}

// checks players inventory for the given item precisely with its subtype matching
int CheckPlayerInventoryList(int pnum, int itemtype, int subtype) {
	int i;
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i)
		if(PlayerInventoryList[pnum][i].item_type == itemtype && PlayerInventoryList[pnum][i].item_subtype == subtype)
			return i;
	return -1;
}

bool IsTwoSelectionItem(int pnum, int item_index) {
	return PlayerInventoryList[pnum][item_index].item_type == DND_ITEM_ORB && PlayerInventoryList[pnum][item_index].item_subtype == DND_ORB_ASSIMILATION;
}

// can only use items in inventory
// returns true if item expired after use
bool UsePlayerItem(int pnum, int item_index, bool countTokens) {
	if(IsUsableItem(PlayerInventoryList[pnum][item_index].item_type)) {
		GiveInventory("DnD_RefreshPane", 1);
		--PlayerInventoryList[pnum][item_index].item_stack;
		if(PlayerInventoryList[pnum][item_index].item_stack) {
			SyncItemStack(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			return false;
		}
		FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
		GiveInventory("DnD_CleanCraftingRequest", 1);
		// if this is freed, we might have to auto-adjust the player's page -- if theres nothing left on this page go back
		if(!CountCraftingMaterials(countTokens))
			TakeInventory("DnD_Crafting_MaterialPage", 1);
		return true;
	}
	return false;
}

// Consumes a stack off a stackable item
void ConsumePlayerItem(int pnum, int item_index) {
	if(IsStackedItem(PlayerInventoryList[pnum][item_index].item_type)) {
		GiveInventory("DnD_RefreshPane", 1);
		--PlayerInventoryList[pnum][item_index].item_stack;
		if(PlayerInventoryList[pnum][item_index].item_stack)
			SyncItemStack(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		else
			FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	}
}

void UsePlayerItem_Count(int pnum, int item_index, int count) {
	PlayerInventoryList[pnum][item_index].item_stack = Clamp_Between(PlayerInventoryList[pnum][item_index].item_stack - count, 0, PlayerInventoryList[pnum][item_index].item_stack);
	if(PlayerInventoryList[pnum][item_index].item_stack)
		SyncItemStack(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	else {
		FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
		GiveInventory("DnD_RefreshPane", 1);
	}
}

// uses items from stash (needed for certain functions)
void UsePlayerStashItem_Count(int pnum, int page, int item_index, int count) {
	PlayerStashList[pnum][page][item_index].item_stack = Clamp_Between(PlayerStashList[pnum][page][item_index].item_stack - count, 0, PlayerStashList[pnum][page][item_index].item_stack);
	if(PlayerStashList[pnum][page][item_index].item_stack)
		SyncItemStack(pnum, item_index, DND_SYNC_ITEMSOURCE_STASH | (page << 16));
	else
		FreeItem(pnum, item_index, DND_SYNC_ITEMSOURCE_STASH | (page << 16), false);
		GiveInventory("DnD_RefreshPane", 1);
}

// a safeguard pretty much for use player item
bool isUsableItem(int itype) {
	switch(itype) {
		case DND_ITEM_ORB:
		case DND_ITEM_CHESTKEY:
		case DND_ITEM_TOKEN:
		return true;
	}
	return false;
}

// we only have orbs as crafting material atm
bool IsCraftingItem(int itype) {
	switch(itype) {
		case DND_ITEM_ORB:
		case DND_ITEM_TOKEN:
		return true;
	}
	return false;
}

bool IsCraftableItem(int itype) {
	// just so that this is recognized
	if(itype > UNIQUE_BEGIN)
		itype &= 0xFFFF; // lower 16 bits contain item type
	switch(itype) {
		case DND_ITEM_CHARM:
		case DND_ITEM_BOOT:
		case DND_ITEM_HELM:
		case DND_ITEM_SPECIALTY_CYBORG:
		case DND_ITEM_WEAPON:
		case DND_ITEM_BODYARMOR:
		return true;
	}
	return false;
}

bool IsUsableOnInventory(int itype) {
	// just so that this is recognized
	if(itype > UNIQUE_BEGIN)
		itype &= 0xFFFF; // lower 16 bits contain item type
	switch(itype) {
		case DND_ITEM_CHARM:
		case DND_ITEM_BODYARMOR:
		case DND_ITEM_BOOT:
		case DND_ITEM_HELM:
		case DND_ITEM_SPECIALTY_CYBORG:
		return true;
	}
	return false;
}

bool IsInventoryCorrupted(int pnum, int item_id) {
	return PlayerInventoryList[pnum][item_id].corrupted;
}

// will count crafting materials the player has currently in their inventory
// this populates an array of fixed order crafting materials for use
int CountCraftingMaterials(bool countTokens) {
	int pnum = PlayerNumber();
	int res = 0;
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(IsCraftingItem(PlayerInventoryList[pnum][i].item_type)) {
			if(PlayerInventoryList[pnum][i].item_type == DND_ITEM_ORB && UniqueCraftingItemList[PlayerInventoryList[pnum][i].item_subtype] == -1) {
				++res;
				UniqueCraftingItemList[PlayerInventoryList[pnum][i].item_subtype] = i;
			}
			else if(countTokens && PlayerInventoryList[pnum][i].item_type == DND_ITEM_TOKEN && UniqueCraftingItemList[DND_MAX_ORB_KINDS + PlayerInventoryList[pnum][i].item_subtype] == -1) {
				++res;
				UniqueCraftingItemList[DND_MAX_ORB_KINDS + PlayerInventoryList[pnum][i].item_subtype] = i;
			}
		}
	}
	return res;
}

// problem is this: say 8 orbs 5 elixirs, first page draws 12, next page requires next current is 12
// but this counts per item type, and per item type count will never make it to 12...
// soln: just count everything regardless, level order in inventory
int GetNextUniqueCraftableMaterial(int current, bool countTokens) {
	int pnum = PlayerNumber();
	int res = 0;
	int lim = DND_MAX_ORB_KINDS + countTokens * DND_MAX_TOKEN_KINDS;
	for(int i = 0; i < lim; ++i) {
		if(UniqueCraftingItemList[i] != -1) {
			++res;
			// return the item's index
			if(res > current)
				return UniqueCraftingItemList[i];
		}
	}
	return -1;
}

int GetTotalStackOfMaterial(int itemid) {
	int pnum = PlayerNumber();
	int res = 0;
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i)
		if(PlayerInventoryList[pnum][i].item_type == PlayerInventoryList[pnum][itemid].item_type && PlayerInventoryList[pnum][i].item_subtype == PlayerInventoryList[pnum][itemid].item_subtype)
			res += PlayerInventoryList[pnum][i].item_stack;
	return res;
}

bool IsSelfUsableItem(int itype, int isubtype) {
	switch(itype) {
		// do all exceptions in here
		case DND_ITEM_ORB:
			switch(isubtype) {
				case DND_ORB_REPENT:
				case DND_ORB_AFFLUENCE:
				case DND_ORB_CALAMITY:
				case DND_ORB_ASSIMILATION:
				case DND_ORB_DESTINY:
				case DND_ORB_ORDER:
				case DND_ORB_REVERANCE:
				return true;
			}
		return false;
		case DND_ITEM_TOKEN:
		return false;
	}
	return false;
}

// standard attribute processing (not implicits)
void ProcessAttribute(int pnum, int atype, int aval, int aextra, int item_index, bool remove) {
	int i, temp;
	int cap;

	switch(atype) {
		// this is handled differently
		case INV_CYBERNETIC:
		break;
	
		// first cases with exceptions to our generic formula
		case INV_MAGAZINE_INCREASE:
			IncPlayerModValue(pnum, atype, aval);
			// add onto the base capacities, not current capacities
			for(i = 0; i < DND_MAX_MAGAZINES; ++i)
				SetAmmoCapacity(WeaponMagazineList[i], GetMagazineCap(pnum, i));
		break;
		case INV_EXPLOSION_RADIUS:
			IncPlayerModValue(pnum, atype, aval);
			SetActorProperty(0, APROP_SCORE, GetPlayerAttributeValue(pnum, atype));
		break;
		
		// these are all accuracy mod groups
		case INV_ACCURACY_INCREASE:
		case INV_ESS_OMNISIGHT:
		case INV_ESS_OMNISIGHT2:
			IncPlayerModValue(pnum, atype, aval);
		break;
		
		// exotic stuff -- reason most of these dont have syncs is that they arent meant to be shown in stat menu page, so no need for client to be aware
		case INV_EX_FORSHOW_BURSTGETSPELLETBONUS:
			// by itself these do nothing
		break;

		// things that have EXTRA field used!
		case INV_EX_CHANCE_HEALMISSINGONPAIN:
		case INV_EX_SECONDEXPBONUS:
		case INV_ESS_VAAJ:
		case INV_INC_PASSIVEREGEN:
		case INV_INC_INSTANTLIFESTEAL:
		case INV_INC_DOUBLEHPBONUS:
			IncPlayerModValue(pnum, atype, aval);
			IncPlayerModExtra(pnum, atype, aextra);
		break;

		case INV_EX_KNOCKBACK_IMMUNITY:
			IncPlayerModValue(pnum, atype, aval);
			UpdatePlayerKnockbackResist();
		break;
		case INV_EX_FACTOR_SMALLCHARM:
			if(!remove) {
				// we now need to re-apply all other features of small charms we have equipped
				// first 4 are small charms
				for(i = 0; i < 4; ++i)
					if(Items_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_REMOVE, true);
				
				// now give the item and re-apply
				IncPlayerModValue(pnum, atype, aval);
								
				for(i = 0; i < 4; ++i)
					if(Items_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_ADD);
								
			}
			else if(PlayerModValues[pnum][atype]) {
				// just take the attribute off and remove features and reapply
				for(i = 0; i < 4; ++i)
					if(Items_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_REMOVE, true);
										
				// little note: aval can be negative if we are removing, so just + is enough to subtract it
				IncPlayerModValue(pnum, atype, aval);
								
				// reapply with this gone
				for(i = 0; i < 4; ++i)
					if(Items_Used[pnum][i].item_type != DND_ITEM_NULL)
						ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_ADD);
			}
		break;
		case INV_EX_MIRROROTHERMEDIUM:
			// reapply the other medium charm
			// find the index of the other medium charm
			temp = -1;
			for(i = 0; i < 2; ++i) {
				if
				(
					Items_Used[pnum][i + MEDIUMCHARM_INDEX1].item_type != DND_ITEM_NULL &&
					(Items_Used[pnum][i + MEDIUMCHARM_INDEX1].item_type >> 16) - 1 != UITEM_MIRROROFETERNITY
				)
				{
					// this holds the other charm's index
					temp = i + MEDIUMCHARM_INDEX1;
					break;
				}
			}

			// we got a valid index
			if(temp != -1) {
				ApplyItemFeatures(pnum, temp, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_REMOVE);
				IncPlayerModValue(pnum, atype, aval);
				ApplyItemFeatures(pnum, temp, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_ADD);
				SyncPlayerItemMods(pnum);
			}
			else // just give the thing now otherwise
				IncPlayerModValue(pnum, atype, aval);
		break;
		case INV_EX_ALLSTATS:
			for(i = INV_STAT_STRENGTH; i <= INV_STAT_INTELLECT; ++i)
				IncPlayerModValue(pnum, i, aval);
			
			// for str
			UpdatePlayerKnockbackResist();

			// for vit
			i = GetActorProperty(0, APROP_HEALTH) - GetSpawnHealth();
			if(remove) {
				temp = GetSpawnHealth();
				if(GetActorProperty(0, APROP_HEALTH) > temp) {
					// set health to new cap, add the extra to player
					if(i > 0)
						SetActorProperty(0, APROP_HEALTH, temp + i);
					else
						SetActorProperty(0, APROP_HEALTH, temp);
				}
			}

			HandleEShieldChange(pnum, remove);
			CalculateUnity(pnum);
		break;
		case INV_EX_PHYSDAMAGEPER_FLATHEALTH:
			// first check all sources, see if they contain this and are lower than this source
			temp = FindMinOnUsedCharmsForAttribute(pnum, INV_EX_PHYSDAMAGEPER_FLATHEALTH, item_index);
			// we got a new min
			if(temp != -1) {
				// update to use the new min if our comparison is better -- dont care otherwise
				if(!remove)
					SetPlayerModValue(pnum, atype, Min(aval, PlayerModValues[pnum][atype]));
			}
			else {
				// no new min was found
				if(remove)
					SetPlayerModValue(pnum, atype, 0);
				else
					SetPlayerModValue(pnum, atype, aval);
			}
		break;
		case INV_EX_FLATDMG_ALL:
			for(i = INV_FLATPHYS_DAMAGE; i <= INV_FLATELEM_DAMAGE; ++i)
				IncPlayerModValue(pnum, i, aval);
		break;
		case INV_EX_ABILITY_RALLY:
			IncPlayerModValue(pnum, atype, aval);
			if(PlayerModValues[pnum][atype])
				GiveInventory("CastRally", 1);
			else
				TakeInventory("CastRally", 1);
		break;
		case INV_HP_INCREASE:
		case INV_HPPERCENT_INCREASE:
			temp = GetSpawnHealth();
			i = GetActorProperty(0, APROP_HEALTH) - temp;
			IncPlayerModValue(pnum, atype, aval);
			if(remove) {
				if(GetActorProperty(0, APROP_HEALTH) > temp) {
					// set health to new cap, add the extra to player
					if(i > 0)
						SetActorProperty(0, APROP_HEALTH, temp + i);
					else
						SetActorProperty(0, APROP_HEALTH, temp);
				}
			}
		break;
		
		case INV_SPEED_INCREASE:
			IncPlayerModValue(pnum, atype, aval);
			SetActorProperty(0, APROP_SPEED, GetPlayerSpeed(pnum));
		break;
		case INV_AMMOCAP_INCREASE:
			IncPlayerModValue(pnum, atype, aval);
			// make sure to update ammo caps
			SetAllAmmoCapacities(pnum);
		break;
		case INV_INCKILLINGSPREE:
			IncPlayerModValue(pnum, atype, aval);
			// make sure to update ammo caps
			UpdatePlayerSpreeTimer(pnum);
		break;
		case INV_EX_CURSEIMMUNITY:
			IncPlayerModValue(pnum, atype, aval);
			if(PlayerModValues[pnum][atype])
				GiveInventory("CurseImmunity", 1);
			else
				HandleCurseImmunityRemoval();
		break;
		case INV_STAT_STRENGTH:
			IncPlayerModValue(pnum, atype, aval);
			UpdatePlayerKnockbackResist();

			temp = GetSpawnHealth();
			i = GetActorProperty(0, APROP_HEALTH) - temp;
			if(remove) {
				if(GetActorProperty(0, APROP_HEALTH) > temp) {
					// set health to new cap, add the extra to player
					if(i > 0)
						SetActorProperty(0, APROP_HEALTH, temp + i);
					else
						SetActorProperty(0, APROP_HEALTH, temp);
				}
			}
			CalculateUnity(pnum);
		break;
		case INV_STAT_DEXTERITY:
			IncPlayerModValue(pnum, atype, aval);
			CalculateUnity(pnum);
		break;
		case INV_STAT_INTELLECT:
			IncPlayerModValue(pnum, atype, aval);
			HandleEShieldChange(pnum, remove);
			CalculateUnity(pnum);
		break;

		case INV_SHIELD_INCREASE:
		case INV_PERCENTSHIELD_INCREASE:
			IncPlayerModValue(pnum, atype, aval);

			HandleEShieldChange(pnum, remove);
		break;

		case INV_EX_REDUCEDAMMOCAP:
			IncPlayerModValue(pnum, atype, aval);
			SetAllAmmoCapacities(pnum);
		break;

		case INV_EX_HEALTHATONE:
			IncPlayerModValue(pnum, atype, aval);
			SetActorProperty(0, APROP_HEALTH, 1);
		break;

		case INV_EX_UNITY:
			IncPlayerModValue(pnum, atype, aval);

			// for str
			UpdatePlayerKnockbackResist();

			// for str
			i = GetActorProperty(0, APROP_HEALTH) - GetSpawnHealth();
			if(remove) {
				temp = GetSpawnHealth();
				if(GetActorProperty(0, APROP_HEALTH) > temp) {
					// set health to new cap, add the extra to player
					if(i > 0)
						SetActorProperty(0, APROP_HEALTH, temp + i);
					else
						SetActorProperty(0, APROP_HEALTH, temp);
				}
			}

			// for int
			HandleEShieldChange(pnum, remove);
		break;

		case INV_INC_PLUSPROJ:
		case INV_INC_PLUSTWOPROJ:
			// make sure this stays positive even for removal!!!
			if(remove) {
				aextra = -aextra;
				temp = -(aextra & 0xFFFF);
			}
			else
				temp = aextra & 0xFFFF;
			aextra >>= 16;

			//Log(s:"processing plus proj ", d:aextra, s:" ", f:temp, s:" ", d:aval);

			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_DMG][WMOD_ITEMS].val = HandleMultiplicativeFactors(Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_DMG][WMOD_ITEMS].val, temp);
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_EXTRAPROJ][WMOD_ITEMS].val += aval;
			MarkWeaponDataSync(pnum, aextra, true);
		break;
		
		// anything that fits our generic formula
		default:
			IncPlayerModValue(pnum, atype, aval);
		break;
	}
}

void ProcessItemFeature(int pnum, int item_index, int source, int aindex, bool remove, int multiplier = 100) {
	int atype = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, aindex, source);
	int aval = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_VAL, item_index, aindex, source);
	int aextra = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_EXTRA, item_index, aindex, source);

	if(multiplier != 100) {
		if(!IsAttributeQualityException(atype)) {
			if(aval > INT_MAX / multiplier) {
				aval /= 100;
				aval *= multiplier;
			}
			else {
				aval *= multiplier;
				aval /= 100;
			}
		}

		if(!IsAttributeExtraException(atype)) {
			if(atype != INV_INC_PLUSPROJ && atype != INV_INC_PLUSTWOPROJ) {
				if(aextra > 100000) {
					aextra /= 100;
					aextra *= multiplier;
				}
				else {
					aextra *= multiplier;
					aextra /= 100;
				}
			}
			else {
				// these two have special treatment, only the first 16 bits scale
				int temp = aextra & 0xFFFF;
				aextra &= 0xFFFF0000;
				temp = 1.0 - (1.0 - temp) * multiplier / 100;
				aextra |= temp;
			}
		}
	}
	
	if(remove) {
		aval = -aval;
		aextra = -aextra;
	}
	
	ProcessAttribute(pnum, atype, aval, aextra, item_index, remove);
}

bool ItemIsCybernetic(int pnum, int item_index, int attrib_count, int source) {
	int i;
	for(i = 0; i < attrib_count; ++i) {
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_ID, item_index, i, source) == INV_CYBERNETIC) {
			return true;
		}
	}

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		attrib_count = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, i, source);
		if(attrib_count == INV_CORR_CYBERNETIC || attrib_count == INV_CYBERNETIC)
			return true;
	}
	return false;
}

void HandleAttributePowerset(int pnum, int val, int powerset, bool remove) {
	if(val) {
		if(!remove)
			val = GetPlayerAttributeValue(pnum, powerset) | val;
		else
			val = GetPlayerAttributeValue(pnum, powerset) & ~val;
		
		SetPlayerModValue(pnum, powerset, val);
	}
}

bool ProcessItemImplicit(int pnum, int item_index, int source, int implicit_id, bool remove, int multiplier = 100) {
	int atype = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, implicit_id, source);
	if(atype == -1)
		return false;

	int aval = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_VAL, item_index, implicit_id, source);
	int aextra = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_EXTRA, item_index, implicit_id, source);

	int i, temp;

	if(multiplier != 100) {
		if(aval > INT_MAX / multiplier) {
			aval /= 100;
			aval *= multiplier;
		}
		else {
			aval *= multiplier;
			aval /= 100;
		}

		if(!IsAttributeExtraException(atype)) {
			if(aextra > 100000) {
				aextra /= 100;
				aextra *= multiplier;
			}
			else {
				aextra *= multiplier;
				aextra /= 100;
			}
		}
	}
	
	if(remove) {
		aval = -aval;
		if(!IsAttributeExtraException(atype))
			aextra = -aextra;
	}

	// delegate regular attribute as implicit responsibility to this function instead
	if(atype < FIRST_CORRUPT_IMPLICIT || atype > LAST_REGULAR_IMPLICIT) {
		ProcessAttribute(pnum, atype, aval, aextra, item_index, remove);
		return false;
	}

	switch(atype) {
		// standard implicits
		case INV_IMP_INCARMOR:
			IncPlayerModValue(pnum, INV_ARMOR_INCREASE, aval);
		break;
		case INV_IMP_INCSHIELD:
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval);

			HandleEShieldChange(pnum, remove);
		break;
		case INV_IMP_INCMIT:
			//Log(s:"process mit ", f:aval, s:" player ", n:pnum + 1);
			IncPlayerModValue(pnum, INV_MIT_INCREASE, aval);
		break;
		case INV_IMP_INCARMORSHIELD:
			IncPlayerModValue(pnum, INV_ARMOR_INCREASE, aval);
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval);

			HandleEShieldChange(pnum, remove);
		break;
		case INV_IMP_INCMITSHIELD:
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval);
			IncPlayerModValue(pnum, INV_MIT_INCREASE, ((aval << 16) / DND_SHIELD_TO_MIT_RATIO));
			
			HandleEShieldChange(pnum, remove);
		break;
		case INV_IMP_INCMITARMOR:
			IncPlayerModValue(pnum, INV_ARMOR_INCREASE, aval);
			IncPlayerModValue(pnum, INV_MIT_INCREASE, ((aval << 16) / DND_ARMOR_TO_MIT_RATIO));
		break;
		case INV_IMP_POWERCORE:
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval);
			IncPlayerModValue(pnum, INV_MAGIC_NEGATION, aextra);
			
			HandleEShieldChange(pnum, remove);
		break;
		case INV_IMP_UNSTABLECORE:
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval);
			IncPlayerModExtra(pnum, INV_IMP_UNSTABLECORE, aextra);

			HandleEShieldChange(pnum, remove);
		break;
		case INV_IMP_INCMITARMORSHIELD:
			IncPlayerModValue(pnum, INV_ARMOR_INCREASE, aval);
			IncPlayerModValue(pnum, INV_SHIELD_INCREASE, aval);
			IncPlayerModValue(pnum, INV_MIT_INCREASE, ((aval << 16) / DND_ARMOR_TO_MIT_RATIO));
		break;

		// value and extra implicits
		case INV_IMP_RAVAGER:
		case INV_IMP_KNIGHTARMOR:
		case INV_IMP_ABSORBLIGHTNING:
			IncPlayerModValue(pnum, atype, aval);
			IncPlayerModExtra(pnum, atype, aextra);
		break;

		// powerset things
		case INV_IMP_DOUBLEESHIELDRECOVERY:
			HandleAttributePowerset(pnum, PPOWER_CYBER, INV_EX_PLAYERPOWERSET1, remove);
		break;

		// single value implicits
		case INV_IMP_HANDGUNBONUS:
		case INV_IMP_NECROARMOR:
		case INV_IMP_LESSLIGHTNINGTAKEN:
		case INV_IMP_LESSPOISONTAKEN:
		case INV_IMP_LESSFIRETAKEN:
		case INV_IMP_LESSSELFDAMAGETAKEN:
		case INV_IMP_FASTEROVERHEATDISS:
		case INV_IMP_BONUSPETCAP:
		case INV_IMP_REDUCEDVISIONIMPAIR:
		case INV_IMP_MOREDAMAGETOBOSSES:
		case INV_IMP_RECOVERESONUNDEADKILL:
		case INV_IMP_PRECISIONCRITBONUS:
			IncPlayerModValue(pnum, atype, aval);
		break;
		
		// for these, do nothing, they are unique to the item itself (not player) and will be checked on the item itself, not player
		case INV_IMP_CANROLL_PHYS:
		case INV_IMP_CANROLL_MAGIC:
		case INV_IMP_CANROLL_EXPLOSIVE:
		case INV_IMP_CANROLL_ENERGY:
		case INV_IMP_CANROLL_ELEMENTAL:
		case INV_IMP_QUALITYCAPFIFTY:
		break;

		case INV_IMP_HIGHREFLECTREDUCE:
			HandleAttributePowerset(pnum, PPOWER_LOWERREFLECT, INV_EX_PLAYERPOWERSET1, remove);
		break;

		case INV_IMP_ESHIELDBLOCKSALL:
			HandleAttributePowerset(pnum, PPOWER_ESHIELDBLOCKALL, INV_EX_PLAYERPOWERSET1, remove);
		break;
		case INV_IMP_MELEEIGNORESSHIELDS:
			HandleAttributePowerset(pnum, PPOWER_MELEEIGNORESHIELD, INV_EX_PLAYERPOWERSET1, remove);
		break;

		// corrupted implicits
		// non-weapon mods
		case INV_CORR_SPEED:
			IncPlayerModValue(pnum, INV_SPEED_INCREASE, aval);
			SetActorProperty(0, APROP_SPEED, GetPlayerSpeed(pnum));
		break;
		case INV_CORR_DROPCHANCE:
			IncPlayerModValue(pnum, INV_DROPCHANCE_INCREASE, aval);
		break;
		case INV_CORR_PERCENTSTATS:
			IncPlayerModValue(pnum, atype, aval);

			// for str
			UpdatePlayerKnockbackResist();

			// for vit
			i = GetActorProperty(0, APROP_HEALTH) - GetSpawnHealth();
			if(remove) {
				temp = GetSpawnHealth();
				if(GetActorProperty(0, APROP_HEALTH) > temp) {
					// set health to new cap, add the extra to player
					if(i > 0)
						SetActorProperty(0, APROP_HEALTH, temp + i);
					else
						SetActorProperty(0, APROP_HEALTH, temp);
				}
			}

			// for int
			HandleEShieldChange(pnum, remove);
			CalculateUnity(pnum);
		break;

		// weapon mods
		case INV_CORR_WEAPONCRIT:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_CRITPERCENT][WMOD_ITEMS].val += aval;
			MarkWeaponDataSync(pnum, aextra, true);
		break;
		case INV_CORR_WEAPONCRITDMG:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_CRITDMG][WMOD_ITEMS].val += aval;
			MarkWeaponDataSync(pnum, aextra, true);
		break;
		case INV_CORR_WEAPONDMG:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_DMG][WMOD_ITEMS].val = HandleMultiplicativeFactors(Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_DMG][WMOD_ITEMS].val, aval);
			MarkWeaponDataSync(pnum, aextra, true);
		break;
		case INV_CORR_WEAPONFORCEPAIN:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_FORCEPAINCHANCE][WMOD_ITEMS].val += aval;
			MarkWeaponDataSync(pnum, aextra, true);
		break;
		case INV_CORR_WEAPONPCTDMG:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_PERCENTDAMAGE][WMOD_ITEMS].val += aval;
			MarkWeaponDataSync(pnum, aextra, true);
		break;
		case INV_CORR_WEAPONPOISONPCT:
			Player_Weapon_Infos[pnum][aextra].wep_mods[WEP_MOD_POISONFORPERCENTDAMAGE][WMOD_ITEMS].val += aval;
			MarkWeaponDataSync(pnum, aextra, true);
		break;
	}

	return true;
}

// Applies item stats to player -- can remove or add
void ApplyItemFeatures(int pnum, int item_index, int source, bool remove = false, bool noSync = false) {
	int ac = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, item_index, -1, source);
	
	// check cybernetic and put it as bool
	bool has_cybernetic = ItemIsCybernetic(pnum, item_index, ac, source);
	int i = 0;

	// power cores are inherently cybernetic
	i = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, 0, source);
	has_cybernetic |= i == INV_IMP_POWERCORE;
	
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i)
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, item_index, i, source) == INV_CYBERNETIC) {
			has_cybernetic = true;
			break;
		}

	int multiplier = 100;

	// if player has mirror of eternity and this is a medium charm that is NOT the mirror, multiply magnitude by 2
	if
	(
		GetPlayerAttributeValue(pnum, INV_EX_MIRROROTHERMEDIUM) && 
		(Items_Used[pnum][item_index].item_type & 0xFFFF) == DND_ITEM_CHARM &&
		Items_Used[pnum][item_index].item_subtype == DND_CHARM_MEDIUM &&
		(Items_Used[pnum][item_index].item_type >> 16) - 1 != UITEM_MIRROROFETERNITY
	)
	{
		multiplier *= 2;
	}

	// quality check
	int temp = GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_index, -1, source);
	if(temp) {
		// quality is percent based
		multiplier = multiplier * (temp + 100) / 100;
	}
	
	// cybernetic check
	if(has_cybernetic && HasClassPerk_Fast("Cyborg", 1))
		multiplier = multiplier * (DND_CYBERNETIC_FACTOR + 100) / 100;
		
	// implicits dont get well of power factor
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i)
		ProcessItemImplicit(pnum, item_index, source, i, remove, multiplier);

	// Well of power factor
	temp = GetPlayerAttributeValue(pnum, INV_EX_FACTOR_SMALLCHARM);
	if(temp && GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, item_index, -1, source) == DND_CHARM_SMALL)
		multiplier = multiplier * temp / FACTOR_FIXED_RESOLUTION;

	for(i = 0; i < ac; ++i)
		ProcessItemFeature(pnum, item_index, source, i, remove, multiplier);

	ACS_NamedExecuteWithResult("DnD Handle Attribute Sync", pnum);

	if(noSync)
		return;

	for(i = 0; i < MAXWEPS; ++i) {
		if(WeaponNeedsDataSync(pnum, i)) {
			SyncClientsideVariable_WeaponMods(pnum, i);
			MarkWeaponDataSync(pnum, i, false);
		}
	}
}

int GetCraftableItemCount() {
	int res = 0, pnum = PlayerNumber();
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i)
		if(IsCraftableItem(PlayerInventoryList[pnum][i].item_type) && PlayerInventoryList[pnum][i].height)
			++res;
	return res;
}

int GetItemTierRoll(int lvl, bool isWellRolled) {
	// 10% chance to roll a tier up or down -- if well rolled 20%
	if(!random(0, 9 - 5 * isWellRolled) || CheckInventory("ReveranceUsed"))
		++lvl;
	else // 0-1 do nothing, 2-3 is -1, 4-5 is -2 => if well rolled has only 33% chance for the tier to be a downgrade
		lvl -= random(0, 5 - 3 * isWellRolled) / 2;

	// clamp just in case
	lvl = Clamp_Between(lvl, 0, MAX_CHARM_AFFIXTIERS);
	return lvl;
}

void InsertAttributeToItem(int pnum, int item_pos, int a_id, int a_val, int a_tier, int a_extra = 0, bool a_fracture = false) {
	int temp = PlayerInventoryList[pnum][item_pos].attrib_count++;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_id = a_id;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_val = a_val;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_tier = a_tier;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_extra = a_extra;
	PlayerInventoryList[pnum][item_pos].attributes[temp].fractured = a_fracture;

	// use for checking ilvl diff
	a_tier *= CHARM_ATTRIBLEVEL_SEPERATOR;

	temp = a_tier - PlayerInventoryList[pnum][item_pos].item_level;
	if(temp > CHARM_ATTRIBLEVEL_SEPERATOR / 2) {
		temp /= CHARM_ATTRIBLEVEL_SEPERATOR;
		if(temp <= 0)
			temp = 1;

		PlayerInventoryList[pnum][item_pos].item_level += temp * random(MAX_CHARM_AFFIXTIERS / 2, 3 * MAX_CHARM_AFFIXTIERS / 4);
		if(PlayerInventoryList[pnum][item_pos].item_level > MAX_ITEM_LEVEL)
			PlayerInventoryList[pnum][item_pos].item_level = MAX_ITEM_LEVEL;
	}

	CheckAttribEffects(pnum, item_pos, a_id, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

void CheckAttribEffects(int pnum, int item_pos, int attrib, int source) {
	int itype = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_pos, -1, source);
	if(itype == DND_ITEM_CHARM && attrib == INV_CYBERNETIC) {
		// check quality and zero it if its non-zero
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_pos, -1, source) > 0) {
			SetItemSyncValue(pnum, DND_SYNC_ITEMQUALITY, item_pos, -1, 0, source);
			SyncItemQuality(pnum, item_pos, source);
		}
	}
}

// can only add attributes to items that are about to be created ie. on field dropped from monster
void AddAttributeToFieldItem(int item_pos, int attrib, int pnum, int max_affixes = 0) {
	if(!max_affixes)
		max_affixes = GetMaxItemAffixes(Inventories_On_Field[item_pos].item_type, Inventories_On_Field[item_pos].item_subtype);
	if(Inventories_On_Field[item_pos].attrib_count < max_affixes) {
		int temp = Inventories_On_Field[item_pos].attrib_count++;
		int lvl = Inventories_On_Field[item_pos].item_level / CHARM_ATTRIBLEVEL_SEPERATOR;
		
		bool makeWellRolled = CheckWellRolled(pnum);
		
		lvl = GetItemTierRoll(lvl, makeWellRolled);

		Inventories_On_Field[item_pos].attributes[temp].attrib_tier = lvl;
		Inventories_On_Field[item_pos].attributes[temp].attrib_id = attrib;
		Inventories_On_Field[item_pos].attributes[temp].fractured = false;

		// it basically adds the step value (val) and a +1 if we aren't 0, so our range is ex: 5-10 in tier 1 then 11-15 in tier 2 assuming +5 range per tier
		// luck adds a small chance for a charm to have well rolled modifier on it -- luck gain is 0.15, 0.05 x 10 = 0.5 max rank thats 50% chance for well rolled mods
		Inventories_On_Field[item_pos].attributes[temp].attrib_val = RollAttributeValue(
			attrib, 
			lvl, 
			makeWellRolled,
			Inventories_On_Field[item_pos].item_type,
			Inventories_On_Field[item_pos].item_subtype
		);

		max_affixes = GetExtraForMod(pnum, attrib, lvl, Inventories_On_Field[item_pos].item_type, Inventories_On_Field[item_pos].item_subtype, makeWellRolled);
		if(max_affixes != -1)
			Inventories_On_Field[item_pos].attributes[temp].attrib_extra = max_affixes;
	}
	CheckAttribEffects(pnum, item_pos, attrib, DND_SYNC_ITEMSOURCE_FIELD);
}

// adds attribute to existing item in player inventory
void AddAttributeToItem(int pnum, int item_pos, int attrib, bool isWellRolled = false) {
	int temp = PlayerInventoryList[pnum][item_pos].attrib_count++;
	int lvl = GetItemTier(PlayerInventoryList[pnum][item_pos].item_level);
	
	// 10% chance to roll a tier up or down for the modifier on the charm
	lvl = GetItemTierRoll(lvl, isWellRolled);
	
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_tier = lvl;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_id = attrib;
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_extra = 0; // set this to 0, if the rollattributevalue needs to assign the extra it will
	PlayerInventoryList[pnum][item_pos].attributes[temp].fractured = false;
	
	// roll the attribute
	PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_val = RollAttributeValue(
		attrib, 
		lvl,
		isWellRolled,
		PlayerInventoryList[pnum][item_pos].item_type,
		PlayerInventoryList[pnum][item_pos].item_subtype
	);

	lvl = GetExtraForMod(pnum, attrib, lvl, PlayerInventoryList[pnum][item_pos].item_type, PlayerInventoryList[pnum][item_pos].item_subtype, isWellRolled);
	if(lvl != -1)
		PlayerInventoryList[pnum][item_pos].attributes[temp].attrib_extra = lvl;

	// if attribute is CYBERNETIC, make sure it resets quality of the item to 0 in case its a charm
	CheckAttribEffects(pnum, item_pos, attrib, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);

	PlayerInventoryList[pnum][item_pos].isDirty = true;
}

// can add implicits up to 3
void GiveImplicitToField(int item_pos, int attr, int val, int extra = -1, int tier = 0, int tier_mapping = 0) {
	int imp_pos = 0;
	for(imp_pos = 0; imp_pos < MAX_ITEM_IMPLICITS && Inventories_On_Field[item_pos].implicit[imp_pos].attrib_id != -1; ++imp_pos);

	if(imp_pos == MAX_ITEM_IMPLICITS)
		return;

	if(extra != -1)
		Inventories_On_Field[item_pos].implicit[imp_pos].attrib_extra = extra;

	Inventories_On_Field[item_pos].implicit[imp_pos].attrib_id = attr;
	Inventories_On_Field[item_pos].implicit[imp_pos].attrib_tier = tier;

	if(!tier)
		Inventories_On_Field[item_pos].implicit[imp_pos].attrib_val = val;
	else {
		int temp = GetItemTier(tier);
		if(tier_mapping)
			Inventories_On_Field[item_pos].implicit[imp_pos].attrib_val = random(val + temp * tier_mapping, val + (temp + 1) * tier_mapping);
		else
			Inventories_On_Field[item_pos].implicit[imp_pos].attrib_val = val * (temp + 1);
	}
}

void GiveCorruptionEffect(int pnum, int item_pos) {
	// pick between random effects to corruption implicits, with equal weight, and decide from there
	// roll between 0 to MAX_CORRUPTION_WEIRD_OUTCOMES + MAX_CORRUPT_IMPLICITS - 1
	// if > than MAX_CORRUPTION_WEIRD_OUTCOMES subtract it to get corrupt implicit
	// NEW: Corruption ALWAYS replaces the very first implicit!
	PlayerInventoryList[pnum][item_pos].isDirty = true;

#ifndef ISDEBUGBUILD
	int corr_outcome = random(0, MAX_CORRUPTION_WEIRD_OUTCOMES + MAX_CORRUPT_IMPLICITS - 1);
#else
	int corr_outcome = MAX_CORRUPTION_WEIRD_OUTCOMES;
#endif

	if(corr_outcome >= MAX_CORRUPTION_WEIRD_OUTCOMES) {
#ifdef ISDEBUGBUILD
		int corr_mod = INV_CORR_WEAPONDMG;
#else
		int corr_mod = FIRST_CORRUPT_IMPLICIT + corr_outcome - MAX_CORRUPTION_WEIRD_OUTCOMES;
#endif
		int extra = GetExtraForMod(pnum, corr_mod);

		if(extra != -1)
			PlayerInventoryList[pnum][item_pos].implicit[0].attrib_extra = extra;

		PlayerInventoryList[pnum][item_pos].implicit[0].attrib_id = corr_mod;
		PlayerInventoryList[pnum][item_pos].implicit[0].attrib_tier = 0;

		// roll the value for this now
		PlayerInventoryList[pnum][item_pos].implicit[0].attrib_val = random(ItemModTable[corr_mod].attrib_low, ItemModTable[corr_mod].attrib_high);

		return;
	}

	// we are within the initial [0, MAX_CORRUPTION_WEIRD_OUTCOMES) range so we can apply the customized change here

	switch(corr_outcome) {
		case DND_CORR_OUTCOME_QUALITY:
			// don't let it hit negative
			PlayerInventoryList[pnum][item_pos].quality += random(-DND_QUALITY_CORRUPTION_CHANGE, DND_QUALITY_CORRUPTION_CHANGE);
			if(PlayerInventoryList[pnum][item_pos].quality < 0)
				PlayerInventoryList[pnum][item_pos].quality = 0;
			SyncItemQuality(pnum, item_pos, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		break;
	}
}

int GetItemFracturedModCount(int pnum, int item_pos) {
	int fc = 0;
	for(int i = 0; i < PlayerInventoryList[pnum][item_pos].attrib_count; ++i)
		fc += PlayerInventoryList[pnum][item_pos].attributes[i].fractured;
	return fc;
}

// Leaves nothing but fractured mods and implicits on it
int ScourItem(int pnum, int item_pos) {
	// scan for fractured mods
	int min_count = GetItemFracturedModCount(pnum, item_pos);
	int frac_id = 0;
	int i;

	// completely reset the item attribs
	for(i = 0; i < PlayerInventoryList[pnum][item_pos].attrib_count; ++i) {
		// is this fractured
		if(PlayerInventoryList[pnum][item_pos].attributes[i].fractured) {
			// if fractured mods are on top, ordered, do not erase or do anything
			if(frac_id == i) {
				// increment this too because we are technically moving over it!
				++frac_id;
				continue;
			}
			// move the fractured mod to the beginning
			PlayerInventoryList[pnum][item_pos].attributes[frac_id].attrib_val = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_val;
			PlayerInventoryList[pnum][item_pos].attributes[frac_id].attrib_tier = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier;
			PlayerInventoryList[pnum][item_pos].attributes[frac_id].attrib_id = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_id;
			PlayerInventoryList[pnum][item_pos].attributes[frac_id].attrib_extra = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_extra;
			PlayerInventoryList[pnum][item_pos].attributes[frac_id].fractured = true;
			++frac_id;
		}

		// erase the current attribute slot
		PlayerInventoryList[pnum][item_pos].attributes[i].attrib_val = 0;
		PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier = 0;
		PlayerInventoryList[pnum][item_pos].attributes[i].attrib_id = 0;
		PlayerInventoryList[pnum][item_pos].attributes[i].attrib_extra = 0;
		PlayerInventoryList[pnum][item_pos].attributes[i].fractured = 0;
	}

	PlayerInventoryList[pnum][item_pos].attrib_count = min_count;

	// return the new minimum count, can be 0 or non-zero if fractured mods exist
	return min_count;
}

bool CanAllowModRollSpecial(int tag, int special_roll_rule) {
	return 	(tag == INV_ATTR_TAG_ELEMENTAL_ID && (special_roll_rule == INV_IMP_CANROLL_ELEMENTAL)) ||
			(tag == INV_ATTR_TAG_OCCULT_ID && (special_roll_rule == INV_IMP_CANROLL_MAGIC)) ||
			(tag == INV_ATTR_TAG_PHYSICAL_ID && (special_roll_rule == INV_IMP_CANROLL_PHYS)) ||
			(tag == INV_ATTR_TAG_EXPLOSIVE_ID && (special_roll_rule == INV_IMP_CANROLL_EXPLOSIVE)) ||
			(tag == INV_ATTR_TAG_ENERGY_ID && (special_roll_rule == INV_IMP_CANROLL_ENERGY));
}

bool IsArmorAttributeException(int rolled_attr) {
	return 	rolled_attr == INV_ARMOR_INCREASE || 
			rolled_attr == INV_ARMORPERCENT_INCREASE;
}

bool IsEnergyShieldAttributeException(int rolled_attr) {
	return 	rolled_attr == INV_SHIELD_INCREASE || 
			rolled_attr == INV_SHIELD_RECHARGEDELAY || 
			rolled_attr == INV_SHIELD_RECOVERYRATE ||
			rolled_attr == INV_PERCENTSHIELD_INCREASE;
}

bool IsMitigationAttributeException(int rolled_attr) {
	return 	rolled_attr == INV_MIT_INCREASE ||
			rolled_attr == INV_MITEFFECT_INCREASE;
}

// assumes the implicit to look for is at implicit id 0!
bool IsImplicitException(int imp, int rolled_attr) {
	//printbold(s:"implicit ", d:imp, s: " rolled ", d:rolled_attr, s: " BAD: ", d:INV_MIT_INCREASE, s: ", ", d:INV_MITEFFECT_INCREASE, s:", ", d:INV_SHIELD_INCREASE, s:", ", d:INV_SHIELD_RECHARGEDELAY, s: ", ", d:INV_SHIELD_RECOVERYRATE);
	switch(imp) {
		// don't let eshield modifiers roll on armor base items etc.
		case INV_IMP_INCARMOR:
		return IsEnergyShieldAttributeException(rolled_attr) || IsMitigationAttributeException(rolled_attr);
		case INV_IMP_INCSHIELD:
		return IsArmorAttributeException(rolled_attr) || IsMitigationAttributeException(rolled_attr);
		case INV_IMP_INCMIT:
		return IsEnergyShieldAttributeException(rolled_attr) || IsArmorAttributeException(rolled_attr);

		case INV_IMP_INCARMORSHIELD:
		return IsMitigationAttributeException(rolled_attr);
		case INV_IMP_INCMITSHIELD:
		return IsArmorAttributeException(rolled_attr);
		case INV_IMP_INCMITARMOR:
		return IsEnergyShieldAttributeException(rolled_attr);
	}
	return false;
}

bool IsItemBaseException(int type, int subtype, int attr_id) {
	return attr_id == INV_CYBERNETIC && (type == DND_ITEM_SPECIALTY_CYBORG || (type == DND_ITEM_BODYARMOR && subtype == BODYARMOR_CYBER));
}

int GetHighestModTierOnItem(int pnum, int item_pos) {
	int count = PlayerInventoryList[pnum][item_pos].attrib_count;
	int t = 0;
	for(int i = 0; i < count; ++i)
		if(PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier > t)
			t = PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier;
	return t;
}

// 0 means nothing exists of this sort
int GetSpecialRollAttribute(int pnum, int item_pos) {
	int special_roll;
	// check if any of the implicits allow for special roll rules
	for(special_roll = 0; special_roll < MAX_ITEM_IMPLICITS; ++special_roll) {
		if(IsSpecialRollRuleAttribute(PlayerInventoryList[pnum][item_pos].implicit[special_roll].attrib_id)) {
			special_roll = PlayerInventoryList[pnum][item_pos].implicit[special_roll].attrib_id;
			break;
		}
	}

	if(special_roll == MAX_ITEM_IMPLICITS)
		special_roll = 0;

	return special_roll;
}

// special roll rule holds PPOWER_CANROLLXXXX and it checks what is possible based on that
// last field is checking for Orb of Order use, if it's not -2 then we must check for its use
int PickRandomAttribute(int item_type = DND_ITEM_CHARM, int item_subtype = DND_CHARM_SMALL, int special_roll_rule = 0, int implicit_id = -1, int respect_order_orb = -2) {
	int bias = Timer() & 0xFFFF;
	int val;
	int craftable_id = DND_CRAFTABLEID_CHARM;

	if(item_type == DND_ITEM_CHARM) {
		if(respect_order_orb == -2 || !respect_order_orb) {
			// unrestricted picking
			val = random(FIRST_INV_ATTRIBUTE + bias, LAST_INV_ATTRIBUTE + bias) - bias;
			// this is a last resort random here, in case there was an overflow... shouldn't, but might
			// this random really didn't want to pick the edge values for some reason so we use the shifted one above...
			if(val < 0)
				val = random(FIRST_INV_ATTRIBUTE, LAST_INV_ATTRIBUTE);
		}
		else {
			// we store +1
			--respect_order_orb;
			// its not -2 or 0, -2 is default behavior, 0 is "check for orb of order, but we dont have any effect stored"
			// so we have something here of a tag, just pick from its pool instead
			val = AttributeTagGroups[respect_order_orb][craftable_id][random(0, AttributeTagGroupCount[respect_order_orb][craftable_id] - 1)];
		}
	}
	else {
		craftable_id = MapItemTypeToCraftableID(item_type);

		// find a random valid tag for this item
		int tag;
		do {
			do {
				if(respect_order_orb == -2 || !respect_order_orb) {
					tag = random(DND_ATTRIB_TAG_ID_BEGIN + bias, DND_ATTRIB_TAG_ID_END + bias) - bias;
					if(tag < 0)
						tag = random(DND_ATTRIB_TAG_ID_BEGIN, DND_ATTRIB_TAG_ID_END);
				}
				else // tag is pre-picked by order orb -- we store +1 of the actual tag
					tag = respect_order_orb - 1;

				// check rule exceptions -- compare vs charms for "cant roll" condition, charms can roll anything
				if(AttributeTagGroupCount[tag][craftable_id] < AttributeTagGroupCount[tag][DND_CRAFTABLEID_CHARM]) {
					// check potential special rolls
					if(CanAllowModRollSpecial(tag, special_roll_rule)) {
						// charms can roll everything possible, so we switch it to that, and then let it pick from that category
						craftable_id = DND_CRAFTABLEID_CHARM;
						break;
					}
				}
				// we check for "0" here because, if the above doesnt make it reroll into a wider pool, and if theres non-zero, that means we still get valid stuff here
			} while(!AttributeTagGroupCount[tag][craftable_id]);

			// finally roll the attrib at random from the group
			val = random(bias, AttributeTagGroupCount[tag][craftable_id] + bias) - bias;
			if(val < 0)
				val = random(0, AttributeTagGroupCount[tag][craftable_id]);
			val = AttributeTagGroups[tag][craftable_id][val];
			// finally check for implicit exception => Ex: Don't roll EShield on Armor base items!
		} while(IsItemBaseException(item_type, item_subtype, val) || IsImplicitException(implicit_id, val));
	}
	return val;
}

void AssignAttributes(int pnum, int item_pos, int itype, int attr_count, int respect_order_orb = -2) {
	int isubt = PlayerInventoryList[pnum][item_pos].item_subtype;
	int special_roll = GetSpecialRollAttribute(pnum, item_pos);
	
	int i = 0, roll, max_attempts = 0;

	while(i < attr_count) {
		do {
			roll = PickRandomAttribute(itype, isubt, special_roll, PlayerInventoryList[pnum][item_pos].implicit[0].attrib_id, respect_order_orb);
			++max_attempts;
		} while(max_attempts < DND_MAX_ORB_REROLL_ATTEMPTS && CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[pnum][item_pos].attrib_count) != -1);
		
		// don't add more than one attribute of the same potentially
		if(max_attempts >= DND_MAX_ORB_REROLL_ATTEMPTS)
			break;

		AddAttributeToItem(pnum, item_pos, roll);
		++i;
	}

	PlayerInventoryList[pnum][item_pos].isDirty = true;
}

void ReforgeItem(int pnum, int item_pos) {
	int itype = PlayerInventoryList[pnum][item_pos].item_type;

	int min_count = ScourItem(pnum, item_pos);
	
	// subtract the fractured mods on it from what it can max have
	int max_natural = GetMaxItemAffixes(itype, PlayerInventoryList[pnum][item_pos].item_subtype);
	int attr_count = random(1, max_natural) - min_count;

	bool hasOrder = CheckInventory("OrderStored");

	if(attr_count > 0) {
		AssignAttributes(pnum, item_pos, itype, attr_count, hasOrder);
		TakeInventory("ReveranceUsed", 1);
	}

	if(hasOrder)
		SetInventory("OrderStored", 0);
}

void RemoveAttributeFromItem(int pnum, int item_id, int to_remove) {
	// all attributes must be shifted left from the position of the deleted attribute now
	for(int i = to_remove; i < PlayerInventoryList[pnum][item_id].attrib_count - 1; ++i) {
		PlayerInventoryList[pnum][item_id].attributes[i].attrib_id = PlayerInventoryList[pnum][item_id].attributes[i + 1].attrib_id;
		PlayerInventoryList[pnum][item_id].attributes[i].attrib_val = PlayerInventoryList[pnum][item_id].attributes[i + 1].attrib_val;
		PlayerInventoryList[pnum][item_id].attributes[i].attrib_tier = PlayerInventoryList[pnum][item_id].attributes[i + 1].attrib_tier;
		PlayerInventoryList[pnum][item_id].attributes[i].attrib_extra = PlayerInventoryList[pnum][item_id].attributes[i + 1].attrib_extra;
		PlayerInventoryList[pnum][item_id].attributes[i].fractured = PlayerInventoryList[pnum][item_id].attributes[i + 1].fractured;
	}
	--PlayerInventoryList[pnum][item_id].attrib_count;
	PlayerInventoryList[pnum][item_id].isDirty = true;
}

// Gives an attribute of a tag group guaranteed, and completely reforges the attribs
void ReforgeWithOneTagGuaranteed(int pnum, int item_pos, int tag_id, int affluence = 1, bool isWellRolled = false) {
	int itype = PlayerInventoryList[pnum][item_pos].item_type;
	int craftable_type;
	
	int min_count = ScourItem(pnum, item_pos);
	
	// charm group etc.
	int rand_attr = -1;
	int attr_count = GetMaxItemAffixes(itype, PlayerInventoryList[pnum][item_pos].item_subtype) - min_count;

	// in case this is a fully fractured mod item
	if(attr_count <= 0)
		return;

	// cap at 2
	int max_tries = 30;
	if(affluence > 2)
		affluence = 2;

	do {
		if(itype == DND_ITEM_CHARM) {
			craftable_type = DND_CRAFTABLEID_CHARM;
			rand_attr = AttributeTagGroups[tag_id][craftable_type][random(0, AttributeTagGroupCount[tag_id][craftable_type] - 1)];

			// if this isn't already present on the item in question
			if(CheckItemAttribute(pnum, item_pos, rand_attr, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[pnum][item_pos].attrib_count) == -1) {
				AddAttributeToItem(pnum, item_pos, rand_attr, isWellRolled);
				TakeInventory("ReveranceUsed", 1);
				--attr_count;
				--affluence;
			}
		}
		else {
			craftable_type = MapItemTypeToCraftableID(itype);
			min_count = 5;

			do {
				// pick an attribute if we have non-zero count, otherwise keep it at -1
				rand_attr = -1;
				if(AttributeTagGroupCount[tag_id][craftable_type])
					rand_attr = AttributeTagGroups[tag_id][craftable_type][random(0, AttributeTagGroupCount[tag_id][craftable_type] - 1)];

				// if no attributes of this type are allowed, but we have some special roll, include it and try again
				if(rand_attr == -1 || IsAttributeArmorException(rand_attr, craftable_type)) {
					// check if any of the implicits allow for special roll rules -- assumes multiple DONT exist
					int imp_id = GetSpecialRollAttribute(pnum, item_pos);

					if(imp_id && CanAllowModRollSpecial(tag_id, imp_id)) {
						craftable_type = DND_CRAFTABLEID_CHARM;
						rand_attr = AttributeTagGroups[tag_id][craftable_type][random(0, AttributeTagGroupCount[tag_id][craftable_type] - 1)];
					}
					else {
						// rest of the mods, we can't fit a guaranteed attribute here
						TakeInventory("ReveranceUsed", 1);
						AssignAttributes(pnum, item_pos, itype, attr_count);
						return;
					}
				}

				if
				(
					CheckItemAttribute(pnum, item_pos, rand_attr, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[pnum][item_pos].attrib_count) == -1 &&
					!IsItemBaseException(itype, PlayerInventoryList[pnum][item_pos].item_subtype, rand_attr) &&
					!IsImplicitException(PlayerInventoryList[pnum][item_pos].implicit[0].attrib_id, rand_attr)
				)
				{
					//printbold(s:"guaranteed add ", d:rand_attr);
					AddAttributeToItem(pnum, item_pos, rand_attr, isWellRolled);
					TakeInventory("ReveranceUsed", 1);
					--attr_count;
					--affluence;
					break;
				}
				--min_count;
			} while(min_count);
		}
	} while(affluence > 0 && attr_count > 0 && max_tries-- > 0);

	// rest of the mods
	AssignAttributes(pnum, item_pos, itype, attr_count);
}

int MakeUnique(int item_pos, int item_type, int pnum, int unique_id = -1) {
	int i, beg, end;

	switch(item_type) {
		case DND_ITEM_CHARM:
			beg = UNIQUE_CHARM_BEGIN;
			end = UNIQUE_CHARM_REGULARDROP_END; // roll only until the regular drop
		break;
		case DND_ITEM_SPECIALTY_CYBORG:
			beg = UNIQUE_POWERCORE_BEGIN;
			end = UNIQUE_POWERCORE_END;
		break;
		case DND_ITEM_BODYARMOR:
			beg = UNIQUE_BODYARMOR_BEGIN;
			end = UNIQUE_BODYARMOR_END;
		break;
		default:
			beg = UNIQUE_CHARM_BEGIN;
			end = UNIQUE_CHARM_END;
		break;
	}

	if(unique_id == -1) {
		if(GetCVar("dnd_ignore_dropweights")) {
			i = random(beg, end);
		}
		else {
			int roll = random(1, MAX_UNIQUE_WEIGHT);
			for(i = beg; i <= end && roll > UniqueItemList[i].weight; ++i);
		}

		#ifdef ISDEBUGBUILD
			if(item_type == DND_ITEM_CHARM) {
				if(random(0, 1))
					end = UNIQUE_CHARM_REGULARDROP_END;
				else {
					beg = UNIQUE_CHARM_DROPONLY_BEGIN;
					end = UNIQUE_CHARM_END;
				}
				
				int bias = Timer() & 0xFFFF;
				i = random(bias + beg, bias + end) - bias;
				//i = random(UITEM_ELEMENTALHARMONY, UITEM_THORNVEIN);
				i = UITEM_REKINDLEDSPARKS;
				//i = random(UITEM_UNITY, UITEM_MINDFORGE);
			}
		#endif
	}
	else
		i = unique_id;

	// i is the unique id
	ConstructUniqueOnField(item_pos, i, pnum);
	return i;
}

void ConstructUniqueOnField(int fieldpos, int unique_id, int pnum) {
	Inventories_On_Field[fieldpos].width = UniqueItemList[unique_id].width;
	Inventories_On_Field[fieldpos].height = UniqueItemList[unique_id].height;
	Inventories_On_Field[fieldpos].item_type = UniqueItemList[unique_id].item_type;
	Inventories_On_Field[fieldpos].item_image = UniqueItemList[unique_id].item_image;
	Inventories_On_Field[fieldpos].item_subtype = UniqueItemList[unique_id].item_subtype;
	Inventories_On_Field[fieldpos].item_level = UniqueItemList[unique_id].item_level;
	Inventories_On_Field[fieldpos].item_stack = UniqueItemList[unique_id].item_stack;
	Inventories_On_Field[fieldpos].attrib_count = UniqueItemList[unique_id].attrib_count;
	Inventories_On_Field[fieldpos].topleftboxid = 0;

	// this can set images sometimes, so just moved item_image below here
	SetupItemImplicit(fieldpos, Inventories_On_Field[fieldpos].item_type & 0xFFFF, Inventories_On_Field[fieldpos].item_subtype, Inventories_On_Field[fieldpos].item_level);

	Inventories_On_Field[fieldpos].corrupted = 0;
	Inventories_On_Field[fieldpos].quality = 0;

	for(int i = 0; i < Inventories_On_Field[fieldpos].attrib_count; ++i) {
		Inventories_On_Field[fieldpos].attributes[i].attrib_id = UniqueItemList[unique_id].attrib_id_list[i];
		Inventories_On_Field[fieldpos].attributes[i].attrib_tier = 0;
		
		// we must roll the value once dropped
		bool makeWellRolled = CheckWellRolled(pnum);
		if(!makeWellRolled) {
			Inventories_On_Field[fieldpos].attributes[i].attrib_val = random(UniqueItemList[unique_id].rolls[i].attrib_low, UniqueItemList[unique_id].rolls[i].attrib_high);
			Inventories_On_Field[fieldpos].attributes[i].attrib_extra = random(UniqueItemList[unique_id].rolls[i].attrib_extra_low, UniqueItemList[unique_id].rolls[i].attrib_extra_high);
		}
		else {
			Inventories_On_Field[fieldpos].attributes[i].attrib_val = random((UniqueItemList[unique_id].rolls[i].attrib_low + UniqueItemList[unique_id].rolls[i].attrib_high) / 2, UniqueItemList[unique_id].rolls[i].attrib_high);
			Inventories_On_Field[fieldpos].attributes[i].attrib_extra = random((UniqueItemList[unique_id].rolls[i].attrib_extra_low + UniqueItemList[unique_id].rolls[i].attrib_extra_high) / 2, UniqueItemList[unique_id].rolls[i].attrib_extra_high);
		}
	}
}

void ResetPlayerInventory(int pnum) {
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(PlayerInventoryList[pnum][i].topleftboxid - 1 == i)
			SyncItemData_Null(pnum, i, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[pnum][i].width, PlayerInventoryList[pnum][i].height);
		PlayerInventoryList[pnum][i].item_type = DND_ITEM_NULL;
		PlayerInventoryList[pnum][i].width = 0;
		PlayerInventoryList[pnum][i].height = 0;
		PlayerInventoryList[pnum][i].item_image = 0;
		PlayerInventoryList[pnum][i].item_type = DND_ITEM_NULL;
		PlayerInventoryList[pnum][i].item_subtype = 0;
		PlayerInventoryList[pnum][i].item_level = 0;
		PlayerInventoryList[pnum][i].item_stack = 0;
		PlayerInventoryList[pnum][i].topleftboxid = 0;

		PlayerInventoryList[pnum][i].corrupted = 0;
		PlayerInventoryList[pnum][i].quality = 0;

		int j;
		for(j = 0; j < MAX_ITEM_IMPLICITS; ++j) {
			PlayerInventoryList[pnum][i].implicit[j].attrib_id = -1;
			PlayerInventoryList[pnum][i].implicit[j].attrib_val = 0;
			PlayerInventoryList[pnum][i].implicit[j].attrib_tier = 0;
			PlayerInventoryList[pnum][i].implicit[j].attrib_extra = 0;
		}
		
		for(j = 0; j < PlayerInventoryList[pnum][i].attrib_count; ++j) {
			PlayerInventoryList[pnum][i].attributes[j].attrib_id = 0;
			PlayerInventoryList[pnum][i].attributes[j].attrib_val = 0;
			PlayerInventoryList[pnum][i].attributes[j].attrib_tier = 0;
			PlayerInventoryList[pnum][i].attributes[j].attrib_extra = 0;
			PlayerInventoryList[pnum][i].attributes[j].fractured = 0;
		}
		PlayerInventoryList[pnum][i].attrib_count = 0;
	}
}

void ResetTradeViewList(int pnum) {
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(TradeViewList[pnum][i].topleftboxid - 1 == i)
			SyncItemData_Null(pnum, i, DND_SYNC_ITEMSOURCE_TRADEVIEW, TradeViewList[pnum][i].width, TradeViewList[pnum][i].height);
		TradeViewList[pnum][i].item_type = DND_ITEM_NULL;
		TradeViewList[pnum][i].width = 0;
		TradeViewList[pnum][i].height = 0;
		TradeViewList[pnum][i].item_image = 0;
		TradeViewList[pnum][i].item_type = 0;
		TradeViewList[pnum][i].item_subtype = 0;
		TradeViewList[pnum][i].item_level = 0;
		TradeViewList[pnum][i].item_stack = 0;
		TradeViewList[pnum][i].topleftboxid = 0;

		TradeViewList[pnum][i].corrupted = 0;
		TradeViewList[pnum][i].quality = 0;

		int j;
		for(j = 0; j < MAX_ITEM_IMPLICITS; ++j) {
			TradeViewList[pnum][i].implicit[j].attrib_id = -1;
			TradeViewList[pnum][i].implicit[j].attrib_val = 0;
			TradeViewList[pnum][i].implicit[j].attrib_tier = 0;
			TradeViewList[pnum][i].implicit[j].attrib_extra = 0;
		}

		for(j = 0; j < TradeViewList[pnum][i].attrib_count; ++j) {
			TradeViewList[pnum][i].attributes[j].attrib_id = 0;
			TradeViewList[pnum][i].attributes[j].attrib_val = 0;
			TradeViewList[pnum][i].attributes[j].attrib_tier = 0;
			TradeViewList[pnum][i].attributes[j].attrib_extra = 0;
			TradeViewList[pnum][i].attributes[j].fractured = 0;
		}
		TradeViewList[pnum][i].attrib_count = 0;
	}
}

/*void ResetFieldInventory() {
	for(int i = 0; i < MAX_INVENTORIES_ON_FIELD; ++i) {
		Inventories_On_Field[i].item_type = DND_ITEM_NULL;
		Inventories_On_Field[i].width = 0;
		Inventories_On_Field[i].height = 0;
		Inventories_On_Field[i].item_image = 0;
		Inventories_On_Field[i].item_subtype = 0;
		Inventories_On_Field[i].item_level = 0;
		Inventories_On_Field[i].item_stack = 0;
		Inventories_On_Field[i].topleftboxid = 0;

		Inventories_On_Field[i].corrupted = 0;
		Inventories_On_Field[i].quality = 0;
		Inventories_On_Field[i].implicit.attrib_id = -1;
		Inventories_On_Field[i].implicit.attrib_val = 0;
		Inventories_On_Field[i].implicit.attrib_tier = 0;
		Inventories_On_Field[i].implicit.attrib_extra = 0;

		for(int j = 0; j < Inventories_On_Field[i].attrib_count; ++j) {
			Inventories_On_Field[i].attributes[j].attrib_id = 0;
			Inventories_On_Field[i].attributes[j].attrib_val = 0;
			Inventories_On_Field[i].attributes[j].attrib_tier = 0;
			Inventories_On_Field[i].attributes[j].attrib_extra = 0;
			Inventories_On_Field[i].attributes[j].fractured = 0;
		}
		Inventories_On_Field[i].attrib_count = 0;
	}
}*/

void ResetPlayerStash(int pnum) {
	for(int p = 0; p < MAX_EXTRA_INVENTORY_PAGES; ++p) {
		for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
			if(PlayerStashList[pnum][p][i].topleftboxid - 1 == i)
				SyncItemData_Null(pnum, i, DND_SYNC_ITEMSOURCE_STASH | (p << 16), PlayerStashList[pnum][p][i].width, PlayerStashList[pnum][p][i].height);
			PlayerStashList[pnum][p][i].item_type = DND_ITEM_NULL;
			PlayerStashList[pnum][p][i].width = 0;
			PlayerStashList[pnum][p][i].height = 0;
			PlayerStashList[pnum][p][i].item_image = 0;
			PlayerStashList[pnum][p][i].item_type = DND_ITEM_NULL;
			PlayerStashList[pnum][p][i].item_subtype = 0;
			PlayerStashList[pnum][p][i].item_level = 0;
			PlayerStashList[pnum][p][i].item_stack = 0;
			PlayerStashList[pnum][p][i].topleftboxid = 0;

			PlayerStashList[pnum][p][i].corrupted = 0;
			PlayerStashList[pnum][p][i].quality = 0;

			int j;
			for(j = 0; j < MAX_ITEM_IMPLICITS; ++j) {
				PlayerStashList[pnum][p][i].implicit[j].attrib_id = -1;
				PlayerStashList[pnum][p][i].implicit[j].attrib_val = 0;
				PlayerStashList[pnum][p][i].implicit[j].attrib_tier = 0;
				PlayerStashList[pnum][p][i].implicit[j].attrib_extra = 0;
			}

			for(j = 0; j < PlayerStashList[pnum][p][i].attrib_count; ++j) {
				PlayerStashList[pnum][p][i].attributes[j].attrib_id = 0;
				PlayerStashList[pnum][p][i].attributes[j].attrib_val = 0;
				PlayerStashList[pnum][p][i].attributes[j].attrib_tier = 0;
				PlayerStashList[pnum][p][i].attributes[j].attrib_extra = 0;
				PlayerStashList[pnum][p][i].attributes[j].fractured = 0;
			}
			PlayerStashList[pnum][p][i].attrib_count = 0;
		}
	}
}

// underlying assumption that the item isn't null, as it's returned from crafting screen
int DisassembleItem_Price(int pnum, int item_pos) {
	// we have a price band for the item, use the price the gauge what kind of orb(s) we can give to the player
	// make tier and amount of mods contribute highly to this too, so we need to do some of the steps we did again for this one
	int base = DND_BASE_DISASSEMBLE_COST;
	int ilvl = PlayerInventoryList[pnum][item_pos].item_level;

	int avg_mod_tier = 0;
	int acount = PlayerInventoryList[pnum][item_pos].attrib_count;
	int fracture_count = 0;
	int i;
	if(acount) {
		if(!IsPlayerInventoryItemUnique(pnum, item_pos)) {
			for(i = 0; i < acount; ++i) {
				avg_mod_tier += PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier + 1;
				fracture_count += PlayerInventoryList[pnum][item_pos].attributes[i].fractured;
			}
		
			avg_mod_tier /= acount;
		}
		else
			avg_mod_tier = MAX_CHARM_AFFIXTIERS / 2;
	}

	base = base * (100 + (ilvl - 1) * DND_DISASSEMBLE_LEVEL_PERCENT + fracture_count * DND_DISASSEMBLE_FRACTURE_PERCENT + DND_DISASSEMBLE_TIER_PERCENT * avg_mod_tier) / 100;

	// if corrupted or has implicit, include that too
	fracture_count = 0;
	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i)
		fracture_count += PlayerInventoryList[pnum][item_pos].implicit[i].attrib_id != -1;

	base = base * (100 + DND_DISASSEMBLE_IMPLICIT_PERCENT * fracture_count) / 100;
	
	if(PlayerInventoryList[pnum][item_pos].corrupted) {
		// 50% increase
		base *= 3;
		base >>= 1;
	}

	return base;
}

// returns the chance and a score rating how big the orb rarity and yield should be depending on how loaded the item is
int GetDissassembleChance(int pnum, int item_pos) {
	int ilvl = PlayerInventoryList[pnum][item_pos].item_level;

	int avg_mod_tier = 0;
	int acount = PlayerInventoryList[pnum][item_pos].attrib_count;
	int fracture_count = 0;
	if(acount) {
		for(int i = 0; i < acount; ++i) {
			// uniques have tier 0
			if(PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier)
				avg_mod_tier += PlayerInventoryList[pnum][item_pos].attributes[i].attrib_tier;
			else
				avg_mod_tier += MAX_CHARM_AFFIXTIERS / 2;
			fracture_count += PlayerInventoryList[pnum][item_pos].attributes[i].fractured;
		}
		avg_mod_tier /= acount;
	}

	// give more chance to succeed if we have the research related to it too
	bool hasResearch = CheckResearchStatus(RES_MOLECULARREC);
	int chance = 	DND_BASE_DISASSEMBLE_CHANCE + 
					DND_BASE_DISASSEMBLE_CHANCE_PERLUCK * GetPerk(STAT_LUCK) +
					hasResearch * DND_DISASS_CHANCEBONUS_RESEARCH;

	// 10% of ilvl + 25% of avg mod tier + 3% flat per fractured mod and 5% if corrupted to fail
	chance -= ilvl / 10 + avg_mod_tier / 4 + DND_BASE_FRACTURE_DISASSEMBLE_CHANCE * fracture_count + DND_BASE_CORRUPT_DISASSEMBLE_CHANCE * PlayerInventoryList[pnum][item_pos].corrupted;

	int yields = 	ilvl * DND_BASE_ILVL_YIELD +
					avg_mod_tier * DND_BASE_AVGMOD_YIELD + 
					DND_BASE_FRACTURE_YIELD * fracture_count + 
					DND_BASE_CORRUPT_YIELD * PlayerInventoryList[pnum][item_pos].corrupted +
					hasResearch + DND_DISASS_CHANCEBONUS_YIELD;
	
	if(yields > 0xFFFF)
		yields = 0xFFFF;

	// limit it
	if(chance > 100)
		chance = 100;

	return (chance << 16) + yields;
}

void DisassembleItem(int pnum, int item_pos, int price, int chance) {
	GiveInventory("DnD_CantSalvage", 1);
	TakeCredit(price);
	// give more chance to succeed if we have the research related to it too
	int yield = chance & 0xFFFF;
	chance >>= 16;

	bool result = chance >= random(1, 100);
	if(result) {
		// success, give user orbs depending on yield
		do {
			int orb = -1;
			int sub = random(75, 150);
			if(yield >= 1000) {
				orb = PickHighTierOrb();
				sub *= 5;
			}
			else if(yield >= 500) {
				orb = PickMidTierOrb();
				sub *= 3;
			}
			else
				orb = PickLowTierOrb();
			yield -= sub;
			
			ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, orb, 1);
		} while(yield > 0);
	}

	ACS_NamedExecuteAlways("DnD Disassemble CS", 0, result);

	// destroy item and give nothing to the user
	FreeItem(pnum, item_pos, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	GiveInventory("DnD_CleanCraftingRequest", 1);
	GiveInventory("DnD_RefreshPane", 1);
	GiveInventory("DnD_CursorDataClearRequest", 1);
	TakeInventory("DnD_CantSalvage", 1);
}

Script "DnD Disassemble CS" (int result) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	if(result) {
		Log(s:"\c[Y5]", l:"DND_DISASS", s:": \cd", l:"DND_DISASS_WIN");
		LocalAmbientSound("Items/SuccessDisassemble", 127);
	}
	else {
		Log(s:"\c[Y5]", l:"DND_DISASS", s:": \cg", l:"DND_DISASS_LOSS");
		LocalAmbientSound("Items/FailDisassemble", 127);
	}
}

bool GetItemMaxQuality(int pnum, int item_index) {
	for(int i = 0; i < MAX_ITEM_IMPLICITS; ++i)
		if(PlayerInventoryList[pnum][item_index].implicit[i].attrib_id == INV_IMP_QUALITYCAPFIFTY)
			return 2 * DND_MAX_CHARM_QUALITY;
	return DND_MAX_CHARM_QUALITY;
}

Script "DnD Check Item Collision" (void) {
	SetResultValue(Spawn("DnD_ItemCollisionChecker", GetActorX(0), GetActorY(0), GetActorZ(0)));
}

#include "DnD_Token.h"

#endif
