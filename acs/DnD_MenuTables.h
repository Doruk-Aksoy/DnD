#ifndef DND_MENUTABLES_IN
#define DND_MENUTABLES_IN

#include "DnD_MenuConstants.h"
#include "DnD_Defs.h"
#include "DnD_Orbs.h"
#include "DnD_EliteInfo.h"
#include "DnD_ClassMenu.h"

#define PRICE_CHARISMAREDUCE 2
#define SHOP_SCALE_MAX 10

#define CHARISMA_PERCENT 5 // percentage
#define CHARISMA_DIV 10 // for fractional
#define CHARISMA_REDUCE (CHARISMA_DIV / CHARISMA_PERCENT)
#define CHARISMA_REDUCE_AFTER100 CHARISMA_REDUCE * 2

#define DND_MENU_LOADOUTWEPITEMS 6
#define DND_MENU_ACCESSORYITEMS 4

#define DND_MAX_ACK_ATTEMPT 35

#define VORTEXMAX 1
#define TRIPLEMAX 1
#define BLOODRUNEMAX 1
#define BOOKMAX 1
#define SOLVEMAX 1
#define STASISMAX 3
#define FIELDKITMAX 3
#define RESETMAX 1

#define ACCESSORY_PER_PAGE 5

#define INVENTORYBOX_BASEX ((HUDMAX_XF / 2) & 0xFFFF0000) + 128.4
#define INVENTORYBOX_BASEY HUDMAX_YF / 2 + 64.0

#define INVENTORYBOX_BASEX_TRADEUP ((2 * HUDMAX_XF / 3) & 0xFFFF0000) - 8.4
#define INVENTORYBOX_BASEY_TRADEUP 2 * HUDMAX_YF / 3 - 44.0

#define STASHBOX_BASEX_UP 2 * HUDMAX_XF / 3 + 260.4

#define INVENTORYBOX_BASEX_TRADEDOWN ((2 * HUDMAX_XF / 3) & 0xFFFF0000) - 8.4
#define INVENTORYBOX_BASEY_TRADEDOWN 2 * HUDMAX_YF / 3 + 196.0

#define INVENTORYBOX_BASEX_TRADEMID ((2 * HUDMAX_XF / 3) & 0xFFFF0000) + 340.4
#define INVENTORYBOX_BASEY_TRADEMID 2 * HUDMAX_YF / 3 + 96.0

#define INVENTORYBOX_ITEMBASEY HUDMAX_YF / 4 + 16.0

#define INVENTORYBOX_BASEX_RECT 384.0
#define INVENTORYBOX_BASEY_RECT 240.0

#define TRADE_RES_X HUDMAX_X * 3 / 2
#define TRADE_RES_Y HUDMAX_Y * 3 / 2

#define TRADEBOX_SKIP 2 * 32.0 / 3
#define INVENTORYBOXTRADE_TOP_BASEX_RECT 465.0
#define INVENTORYBOXTRADE_TOP_BASEY_RECT 293.0

#define STASHBOX_TOP_BASEX_RECT 275.0

#define HUD_DII_MULT 14

#define INVENTORYBOXTRADE_BOT_BASEX_RECT 465.0
#define INVENTORYBOXTRADE_BOT_BASEY_RECT 133.0

#define INVENTORYBOXTRADE_MID_BASEX_RECT 221.0
#define INVENTORYBOXTRADE_MID_BASEY_RECT 200.0

#define INVENTORYBOXTRADE_TRADECONFIRM_BASEX_RECT 467.0
#define INVENTORYBOXTRADE_TRADECONFIRM_BASEY_RECT 168.0

#define INVENTORYBOXTRADE_TRADEBOXSKIP_X 77.0
#define INVENTORYBOXTRADE_TRADEBOXSKIP_Y 16.0

#define INVENTORYBOXTRADE_TRADECANCEL_BASEX_RECT 350.0
#define INVENTORYBOXTRADE_TRADECANCEL_BASEY_RECT 168.0

#define TRADECONFIRM_BOXID 3 * MAX_INVENTORY_BOXES
#define TRADECANCEL_BOXID 3 * MAX_INVENTORY_BOXES + 1

#define TRADE_BOXOFFSET 2 * MAX_INVENTORY_BOXES
#define STASHBUTTON_BOXID_START 2 * MAX_INVENTORY_BOXES + 1

#define INVENTORYINFO_NORMALVIEW_WRAPX 272.0
#define INVENTORYINFO_NORMALVIEW_WRAPY 152.0

#define INVENTORYINFO_TRADEVIEW_WRAPX 272.0
#define INVENTORYINFO_TRADEVIEW_WRAPY 152.0

#define STASHTAB_BUTTON_TOPLEFT_X 226.0
#define STASHTAB_BUTTON_TOPLEFT_Y 186.0
#define STASHTAB_BUTTON_BOTRIGHT_X 216.0
#define STASHTAB_BUTTON_BOTRIGHT_Y 175.0

#define STASHTAB_BUTTON_INCREMENT_X 10.0

#define TRADEITEMOFFSETSCALE 4.0
#define TRADEITEMSKIP 3 * 32.0 / 2

// left right buttons at material side 
#define MAX_CRAFTING_ADDITIONALBUTTONS 4 + (MENU_LOAD_CRAFTING_LAST - MENU_LOAD_CRAFTING_FIRST + 1)

#define MAX_CRAFTING_ITEMBOXES 16
#define CRAFTING_IMAGE_COUNT 4
#define MATERIALBOX_OFFSET MAX_CRAFTING_ITEMBOXES * CRAFTING_IMAGE_COUNT - 7
#define MATERIALBOX_OFFSET_BOXID MAX_CRAFTING_ITEMBOXES

#define MAX_CRAFTING_NORMAL_BOXES MAX_CRAFTING_MATERIALBOXES + MAX_CRAFTING_ITEMBOXES
#define MAX_CRAFTING_BOXES MAX_CRAFTING_NORMAL_BOXES + MAX_CRAFTING_ADDITIONALBUTTONS
#define CRAFTINGARROWBOX_OFFSET MAX_CRAFTING_NORMAL_BOXES

#define MATERIALARROW_HUDID MATERIALBOX_OFFSET + MAX_CRAFTING_MATERIALBOXES
#define MATERIALARROW_ID MAX_CRAFTING_BOXES - 2

#define CRAFTING_PAGEARROW_ID MAX_CRAFTING_BOXES - 4

#define CRAFTING_PAGEARROWL_X 466.0
#define CRAFTING_PAGEARROWL_Y 36.0
#define CRAFTING_PAGEARROWR_X 142.0
#define CRAFTING_PAGEARROW_XSIZE 12.0
#define CRAFTING_PAGEARROW_YSIZE 8.0

#define CRAFTING_WEAPON_BOXID MAX_CRAFTING_NORMAL_BOXES
#define CRAFTING_INVENTORY_BOXID MAX_CRAFTING_NORMAL_BOXES + 1

#define CRAFTING_MATERIALBOX_X 92.0
#define CRAFTING_MATERIALBOX_Y 264.0
#define CRAFTING_MATERIALBOX_SKIPX 12.0
#define CRAFTING_MATERIALBOX_SKIPY 4.0

#define CRAFTING_WEAPONBOXDRAW_X 68.0
#define CRAFTING_WEAPONBOXDRAW_Y 60.0

#define CRAFTING_ITEMBOX_X 445.0
#define CRAFTING_ITEMBOX_Y 293.0
#define CRAFTING_ITEMBOX_SKIPX 12.0
#define CRAFTING_ITEMBOX_SKIPY 4.0

#define CRAFTING_LARR_X 98.0
#define CRAFTING_LARR_Y 40.0
#define CRAFTING_RARR_X 23.0
#define CRAFTING_ARROW_X_SKIP 12.0
#define CRAFTING_ARROW_Y_SKIP 8.0

#define DND_MENUFLOATYICON_TID 1664

#define DND_MENU_ITEMSAVEBITS1 8
#define DND_MENU_ITEMSAVEBITS1_MASK 0xFF
#define DND_MENU_ITEMCLEARMASK1 0xFFF00FF
#define DND_MENU_ITEMSAVEBITS2 16
#define DND_MENU_ITEMCLEARMASK2 0x000FFFF

enum {
	BOXLIT_STATE_OFF,
	BOXLIT_STATE_CURSORON,
	BOXLIT_STATE_CLICK
};

#define MAXLITBOXES 3 * MAX_INVENTORY_BOXES
int InventoryBoxLit[MAXLITBOXES];

typedef struct mi {
	rect_T MenuRectangles[MAX_INVENTORY_BOXES];
	int cursize;
} menu_inventory_T;

typedef struct mt {
	rect_T MenuRectangles[3 * MAX_INVENTORY_BOXES + 2];
	int cursize;
} menu_trade_T;

#define MAX_STACK_ELEMS 16
typedef struct menu_stack {
	int stack_elems[MAX_STACK_ELEMS];
	int stackptr;
	int cursize;
} menu_stack_T;

// Scroll defs

int ScrollPos = 0;

enum {
	INVENTORY_SETUP_BIT = 0,
	TRADEVIEW_SETUP_BIT = 1,
	STASH_SETUP_BIT = 2,
	CRAFTING_SETUP_BIT = 4,
};
int PaneSetup = 0;

#define FIRST_CLICKABLE_BOXID MAINBOX_LARR
#define LAST_CLICKABLE_BOXID MAINBOX_RARR
#define MAX_TIMED_BUTTONS (LAST_CLICKABLE_BOXID - FIRST_CLICKABLE_BOXID + 1)
str ButtonTimers[MAX_TIMED_BUTTONS] = { 
	"DnD_LeftArrowButton_Timer",
	"DnD_ReturnArrowButton_Timer", 
	"DnD_RightArrowButton_Timer" 
};

int ButtonFrameCounts[MAX_TIMED_BUTTONS] = { 3, 3, 3 };
// Box enums end

#define SHOP_MAXWEAPON_PAGES (MENU_SHOP_WEAPON8 - MENU_SHOP_WEAPON1 + 1)
#define SHOP_MAXAMMO_PAGES (SHOP_LASTAMMO_PAGE - SHOP_FIRSTAMMO_PAGE + 1)

int WeaponBeginIndexes[SHOP_MAXWEAPON_PAGES] = {
	SHOP_WEAPON1_BEGIN,
	SHOP_WEAPON2_BEGIN,
	SHOP_WEAPON31_BEGIN,
	SHOP_WEAPON32_BEGIN,
	SHOP_WEAPON41_BEGIN,
	SHOP_WEAPON42_BEGIN,
	SHOP_WEAPON51_BEGIN,
	SHOP_WEAPON52_BEGIN,
	SHOP_WEAPON61_BEGIN,
	SHOP_WEAPON62_BEGIN,
	SHOP_WEAPON7_BEGIN,
	SHOP_WEAPON8_BEGIN
};

// Holds the players' current maximum page visit indexes
#define MENUMAXPAGES MENU_ABILITY + 1
#define MAX_MENU_BOXPAGES MENU_ABILITY + 1

// Input Listener Flags
enum { 
   LISTEN_LEFT = 1,
   LISTEN_RIGHT = 2,
   LISTEN_SCROLL = 4,
   LISTEN_FASTLR = 8
};

// Object definitions for shop
enum { 
   OBJ_WEP = 1,
   OBJ_AMMO = 2,
   OBJ_ABILITY = 4,
   OBJ_ARTI = 8,
   OBJ_ARMOR = 32, // color those that can only be had 1 in a category as a different color
   OBJ_HASCHOICE = 64,
   OBJ_RESEARCH = 128, // requires some research to be available,
   OBJ_RESEARCH_ATLEASTONE = 256, // only requires one of the research reqs to be met
   OBJ_USESCROLL = 512, // has some text on page that use scrolling
   OBJ_ACCOUNT = 1024,
   OBJ_SHOTGUN = 2048
};

// Trade options
enum {
	TRADE_BUY = 1,
	TRADE_SELL = 2,
	TRADE_WEAPON = 4,
	TRADE_AMMO = 8,
	TRADE_ABILITY = 16,
	TRADE_ARTIFACT = 32,
	TRADE_ARMOR = 128,
	TRADE_ARMOR_REPLACE = 256,
	TRADE_ACCOUNT = 512
};

// Popup Definitions
enum {
	POPUP_ERROR,
	POPUP_SELL,
	POPUP_QBUY_BIND
	//POPUP_FULLAMMO
};
	 
enum {
	TYPE_WEAPON,
	TYPE_AMMO,
	TYPE_ABILITY,
	TYPE_ARTI,
	TYPE_ARMOR,
	TYPE_ACCOUNT
};
		
struct coord {
	int x;
	int y;
};

// for drawtoggledimage func
struct draw_info {
	int flags;
	int res_id;
};

#define SHOPINFO_PRICE 0
#define SHOPINFO_MAX 1
#define SHOPINFO_STOCK 2
int ShopInfo[MAXSHOPITEMS][3] = 
	{
		// Weapons
		// 1
		{ 3000,	 1,		1 },
		{ 3750,  1,		1 },
		{ 3750,  1,		1 },
		{ 8000,  1,		1 },
		{ 8500,  1,		1 },
		{ 6500,  1,		1 },
		
		// 2
		{ 2000,  1,		1 },
		{ 3500,  1,		1 },
		{ 3000,  1,		1 },
		{ 3000,	 1,		1 },
		{ 4000,  1,		1 },
		{ 3750,  1,		1 },
		{ 3250,  1,		1 },
		
		// 3 - 1
		{ 4000,  1,		1 },
		{ 4500,  1,		1 },
		{ 5000,  1,		1 },
		{ 4350,  1,		1 },
        { 4500,  1,		1 },
		{ 5250,  1,		1 },
		{ 5000,  1,		1 },
		{ 6000,	 1,		1 },
		
		// 3 - 2
		{ 5250,  1,		1 },
		{ 5500,  1,		1 },
		{ 6000,	 1,		1 },
		{ 5750,  1,		1 },
		{ 6500,  1,		1 },
		{ 6000,  1,     1 },
		{ 5250,  1,		1 },
		{ 5250,  1,		1 },
		
		// 4 - 1
		{ 4000,  1,		1 },
		{ 4500,  1,		1 },
		{ 6000,  1,		1 },
		{ 5750,  1,		1 },
        { 6500,  1,		1 },
		{ 7000,  1,		1 },
		{ 8250,  1,		1 },
		{ 7250,  1,		1 },
		
		// 4 - 2
		{ 6750,  1,		1 },
		{ 5250,	 1,		1 },
		{ 4800,  1,		1 },
		{ 7000,  1,		1 },
		
		// 5 - 1
		{ 3500,  1,		1 },
		{ 4800,  1,		1 },
		{ 6750,  1,		1 },
		{ 5500,  1,		1 },
		{ 5750,  1,		1 },
		{ 5500,  1,		1 },
		{ 7000,  1,     1 },
		{ 6250,  1,		1 },
		
		// 5 - 2
		{ 1750,  1,		1 },
		{ 4000,  1,		1 },
		{ 5000,  1,		1 },
		{ 5500,	 1,		1 },
		
		// 6 - 1
		{ 4000,  1,		1 },
		{ 4500,  1,		1 },
		{ 6500,  1,		1 },
		{ 4800,  1,		1 },
        { 5250,  1,		1 },
		{ 6000,  1,		1 },
		{ 7500,	 1,		1 },
		
		// 6 - 2
		{ 7000,  1,		1 },
		{ 5000,  1,		1 },
		{ 5500,  1,		1 },
		
		// 7
		{ 15000,  1,	1 },
		{ 16000,  1,	1 },
		{ 16000,  1,	1 },
		{ 16000,  1,	1 },
		{ 18000,  1,	1 },
		
		{ 9500,   1,	1 },
		{ 10000,  1,	1 },
		{ 12500,  1,	1 },
		
		// 8
		{ 25000, 1,		1 },
		{ 25000, 1,		1 },
		{ 25000, 1,		1 },
		{ 25000, 1,		1 },
		
		// Ammunition
		// Ammo 1 --- -1 stock means use cap of the ammo type
		{ 40,			1, 		-1 },
		{ 60,			1, 		-1 },
		{ 80,			1, 		-1 },
		{ 100,			1, 		-1 },
		{ 160,       	1, 		-1 },
		{ 200,       	1, 		-1 },
		{ 200,      	1, 		-1 },
		{ 200,      	1, 		-1 },
		{ 75,       	1, 		-1 },
		{ 130,       	1, 		-1 },
		
		// Ammo 2
		{ 200,      	1, 		-1 },
		{ 200,      	1, 		-1 },
		{ 180,       	1, 		-1 },
		{ 160,			1, 		-1 },
        { 130,       	1, 		-1 },
        { 140,       	1, 		-1 },
        { 210,      	1, 		-1 },
        { 160,       	1, 		-1 },
        { 180,       	1, 		-1 },
        { 140,       	1, 		-1 },
		
		// Ammo 3
        { 300,      	1, 		-1 },
		{ 220,			1, 		-1 },
		{ 200,			1, 		-1 },
		{ 150,			1, 		-1 },
		{ 550,			1, 		-1 },
		{ 130,       	1, 		-1 },
		{ 180,			1, 		-1 },
		{ 200,			1, 		-1 },
		{ 230,			1, 		-1 },
		{ 200,			1, 		-1 },
		
		// Ammo 4
		{ 500,		1, 		-1 },
		{ 170,		1, 		-1 },
		{ 180,		1, 		-1 },
		{ 250,		1, 		-1 },
		{ 190,		1, 		-1 },
		{ 175,		1,		-1 },
		{ 250,		1,		-1 },
		{ 125,		1,		-1 },
		{ 115,		1,		-1 },
		{ 100,		1,		-1 },
		
		// Special Ammunition
		{ 500,	    1, 		-1 },
		{ 850,	    1, 		-1 },
		{ 950,	    1, 		-1 },
        { 900,      1, 		-1 },
		{ 1250,		1, 		-1 },
		
		{ 950,      1, 		-1 },
		{ 1250,	    1, 		-1 },
		
		
		// Abilities - 1
		{ 12000,  1,		1 },
		{ 12000,  1,		1 },
		{ 19000,  1,		1 },
		{ 14500,  1,		1 },
		{ 12000,  1,		1 },
		{ 12000,  1,		1 },
		{ 14000,  1,		1 },
		{ 17000,  1,		1 },
		
		// Abilities - 2
		{ 14000,  1,		1 },
		{ 19500,  1,		1 },
		
		// Armor costs
		{ 4500,	 1,			1 },
		{ 9000,  1,			1 },
		{ 13500, 1,			1 },
		{ 18000, 1,			1 },
		
		{ 10000, 1,			1 },
		{ 10000, 1,			1 },
		{ 10000, 1,			1 },
		{ 10000, 1,			1 },
        { 10000, 1,			1 },
        
        { 22500, 1,			1 },
		{ 18500, 1,			1 },
		{ 8500,  1,			1 },
		{ 15000, 1,			1 },
		{ 17000, 1,			1 },
		{ 12000, 1,			1 },
		{ 19000, 1,			1 },
		
		// Account
		{ 1500000, MAX_EXTRA_INVENTORY_PAGES,		5 },

		// Artifacts, here for convenience. Index = MAXSHOPITEMS - MAXARTIFACTS
		{ 1250,          FIELDKITMAX,				3   },
		{ 18000,         SOLVEMAX,					1   },
		{ 12000,         STASISMAX,					1   },
		{ 20000,         BLOODRUNEMAX,				1 	},
		{ 20000,         TRIPLEMAX,					1   },
		{ 20000,         VORTEXMAX,					1   },
		{ 17500,	     BOOKMAX,					1   },
		{ 27500,         1,							1	},
		{ 50000,         1,							1	},
		{ 10000,         RESETMAX,					1 	}
};

// keeps track of how much is left for all players
int ShopStockRemaining[MAXPLAYERS][MAXSHOPITEMS];

#define MAX_RESEARCH_REQUIREMENTS 3
// Basis for multi-research requiring stuff is here
int ItemResearchRequirements[MAXSHOPITEMS][MAX_RESEARCH_REQUIREMENTS] = {
	// wep slot 1
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT1OCCULT, -1, -1 },
		{ RES_SLOT1UPG2, -1, -1 },
		{ RES_SLOT1UPG1, -1, -1 },
		{ RES_SLOT1OCCULT, -1, -1 },
		
	// wep slot 2
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT2UPG1, -1, -1 },
		{ RES_SLOT2UPG2, -1, -1 },
		{ RES_SLOT2LUXURY, -1, -1 },
		{ RES_SLOT2LUXURY, -1, -1 },
		
	// wep slot 3 - 1
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT3UPG1, -1, -1 },
		{ RES_SLOT3UPG2, -1, -1 },
		{ RES_SLOT3UPG3, -1, -1 },
		{ RES_SLOT3UPG4, -1, -1 },
		{ -1, -1, -1 },
		
	// wep slot 3 - 2
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT3SSGUPG1, -1, -1 },
		{ RES_SLOT3SSGUPG2, -1, -1 },
		{ RES_SLOT3SSGUPG3, -1, -1 },
		{ RES_SLOT3SSGUPG4, -1, -1 },
		{ RES_SLOT3LUXURY, -1, -1 },
		{ RES_SLOT3LUXURY, -1, -1 },
		
	// wep slot 4 - 1
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT4UPG1, -1, -1 },
		{ RES_SLOT4UPG2, -1, -1 },
		{ RES_SLOT4UPG3, -1, -1 },
		{ RES_SLOT4UPG4, -1, -1 },
		{ RES_SLOT4UPG5, -1, -1 },
		
	// wep slot 4 - 2
		{ RES_SLOT4LUXURY, -1, -1 },
		{ RES_SLOT4LUXURY, -1, -1 },
		{ RES_SLOT4LUXURY, -1, -1 },
		{ RES_SLOT4LUXURY, -1, -1 },
		
	// wep slot 5 - 1
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT5UPG1, -1, -1 },
		{ RES_SLOT5UPG2, -1, -1 },
		{ RES_SLOT5UPG3, -1, -1 },
		{ RES_SLOT5UPG4, -1, -1 },
		{ RES_SLOT5UPG5, -1, -1 },
		
	// wep slot 5 - 2
		{ RES_SLOT5GL, -1, -1 },
		{ RES_SLOT5LUXURY, -1, -1 },
		{ RES_SLOT5LUXURY, -1, -1 },
		{ RES_SLOT5LUXURY, -1, -1 },
		
	// wep slot 6 - 1
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT6UPG1, -1, -1 },
		{ RES_SLOT6UPG2, -1, -1 },
		{ RES_SLOT6UPG3, -1, -1 },
		{ RES_SLOT6UPG4, -1, -1 },
		
	// wep slot 6 - 2
		{ RES_SLOT6LUXURY, -1, -1 },
		{ RES_SLOT6LUXURY, -1, -1 },
		{ RES_SLOT6LUXURY, -1, -1 },
		
	// wep slot 7
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT7UPG1, -1, -1 },
		{ RES_SLOT7UPG2, -1, -1 },
		{ RES_SLOT7LUXURY, -1, -1 },
		{ RES_SLOT7LUXURY, -1, -1 },
		{ RES_SLOT7LUXURY, -1, -1 },
		
	// wep slot 8
		{ RES_SLOT8REVEAL, -1, -1 },
		{ RES_SLOT8REVEAL, -1, -1 },
		{ RES_SLOT8REVEAL, -1, -1 },
		{ RES_SLOT8REVEAL, -1, -1 },
		
	// ammo 1
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT3LUXURY, RES_SLOT4UPG2, -1 },
		{ RES_SLOT4LUXURY, -1, -1 },
		{ RES_SLOT4LUXURY, -1, -1 },
		{ RES_SLOT5LUXURY, -1, -1 },
		{ RES_SLOT5GL, RES_SLOT5UPG2, -1 },
		{ RES_SLOT6LUXURY, -1, -1 },
		
	// ammo 2
		{ RES_SLOT6LUXURY, -1, -1 },
		{ RES_SLOT7LUXURY, -1, -1 },
		{ RES_SLOT3LUXURY, -1, -1 },
		{ RES_SLOT2LUXURY, -1, -1 },
		{ RES_SLOT3SSGUPG1, -1, -1 },
		{ RES_SLOT4UPG2, -1, -1 },
		{ RES_SLOT5UPG1, -1, -1 },
		{ RES_SLOT6UPG1, -1, -1 },
		{ RES_SLOT6UPG2, -1, -1 },
		{ RES_SLOT3UPG2, -1, -1 },
		
	// ammo 3
		{ RES_SLOT7UPG1, -1, -1 },
		{ RES_SLOT4UPG3, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT2UPG2, -1, -1 },
		{ RES_SLOT7UPG2, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT4UPG4, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT4LUXURY, -1, -1 },
		{ RES_SLOT3SSGUPG3, -1, -1 },
		
	// ammo 4
		{ RES_SLOT3UPG3, RES_SLOT6UPG4, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT5LUXURY, -1, -1 },
		{ RES_SLOT3SSGUPG4, -1, -1 },
		{ RES_SLOT5UPG4, -1, -1 },
		{ RES_SLOT5LUXURY, -1, -1 },
		{ RES_SLOT3UPG4, -1, -1 },
		{ RES_SLOT4UPG5, -1, -1 },
		
	// ammo special
		{ RES_FLECHETTE, -1, -1 },
		{ RES_PIERCING, -1, -1 },
		{ RES_ELECTRIC, -1, -1 },
		{ RES_NITRO, -1, -1 },
		{ RES_SLUGSHELL, -1, -1 },
		{ RES_SONICGRENADE, -1, -1 },
		{ RES_HEGRENADE, -1, -1 },
		
	// abilities
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_OCCULTABILITY, -1, -1 },
		{ RES_NANOTECH, -1, -1 },
		{ RES_NANOTECH, -1, -1 },
		{ RES_OCCULTABILITY, -1, -1 },
		{ RES_NANOTECH, -1, -1 },
		{ RES_OCCULTABILITY, -1, -1 },
		{ RES_OCCULTABILITY, -1, -1 },
		
	// armor 1
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_RAREARMOR, -1, -1 },
		{ RES_RAREARMOR, -1, -1 },
		{ RES_RAREARMOR, -1, -1 },
		{ RES_RAREARMOR, -1, -1 },
		{ RES_RAREARMOR, -1, -1 },
	
	// armor 2
		{ RES_SUPERARMOR, -1, -1 },
		{ RES_RAREARMOR, -1, -1 },
		{ RES_RAREARMOR, -1, -1 },
		{ RES_RAREARMOR, -1, -1 },
		{ RES_RAREARMOR, -1, -1 },
		{ RES_RAREARMOR, -1, -1 },
		{ RES_SYNTHMETALARMOR, -1, -1},
		
	// account
		{ RES_STASHTAB, -1, -1 },
		
	// arti
		{ -1, -1, -1 },
		{ RES_OCCULTARTIFACT, -1, -1 },
		{ -1, -1, -1 },
		{ RES_OCCULTARTIFACT, -1, -1 },
		{ RES_OCCULTARTIFACT, -1, -1 },
		{ RES_OCCULTARTIFACT, -1, -1 },
		{ RES_OCCULTARTIFACT, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 }
};

#define DND_ARTI_BUYONCE_END ARTI_BOOK
enum {
	ARTI_FIELDKIT = 1,
	ARTI_SALVATION,
	ARTI_SHIELD,
	ARTI_BLOODRUNE,
	ARTI_TRIPLE,
	ARTI_BLADE,
	ARTI_BOOK,
	ARTI_MAP,
	ARTI_BACKPACK,
	ARTI_RESET,
};

// gets item type
int GetItemType(int id) {
	if(id <= SHOP_LASTWEP_INDEX)
		return TYPE_WEAPON;
	else if(id <= SHOP_LASTAMMO_SPECIALINDEX)
		return TYPE_AMMO;
	else if(id <= SHOP_LASTABILITY_INDEX)
		return TYPE_ABILITY;
	else if(id <= SHOP_LASTARMOR_INDEX)
		return TYPE_ARMOR;
	else if(id <= SHOP_ACCOUNT_END)
		return TYPE_ACCOUNT;
	else
		return TYPE_ARTI;
	return TYPE_WEAPON;
}

str GetItemName(int id) {
	int type = GetItemType(id);
	str name = "";
	
	switch(type) {
		case TYPE_WEAPON:
			name = Weapons_Data[ShopTableIdToWeaponTableId(id)][WEAPON_NAME];
		break;
		case TYPE_AMMO:
			if(id < SHOP_FIRSTAMMOSPECIAL_INDEX) {
				type = ShopAmmoIndexToRegularAmmoIndex(id - SHOP_FIRSTAMMO_INDEX);
				name = AmmoInfo_Str[type >> 16][type & 0xFFFF][AMMOINFO_NAME];
			}
			else
				name = SpecialAmmoInfo_Str[id - SHOP_FIRSTAMMOSPECIAL_INDEX][AMMOINFO_NAME];
		break; 
		case TYPE_ABILITY:
			name = AbilityInfo[id - SHOP_ABILITY1_BEGIN];
		break;
		case TYPE_ARMOR:
			name = ArmorStrings[id - SHOP_FIRSTARMOR_INDEX][ARTI_NAME];
		break;
		case TYPE_ARTI:
			name = ArtifactInfo[id - SHOP_FIRSTARTI_INDEX][ARTI_NAME];
		break;
		case TYPE_ACCOUNT:
			// there's only one for now
			name = "DnD_PlayerInventoryPages";
		break;
	}
	return name;
}

// put them in this place if they are luxury weapons
bool IsLuxuryWeapon(int id) {
	switch(id) {
		case SHOP_WEP_DUSKBLADE:
		case SHOP_WEP_SICKLE:
		
		case SHOP_WEP_RUBYWAND:
		case SHOP_WEP_SCATTERPISTOL:
		
		case SHOP_WEP_SILVER:
		case SHOP_WEP_SLAYER:
		
		case SHOP_WEP_DESOLATOR:
		case SHOP_WEP_MINIGUN:
		case SHOP_WEP_EBONY:
		case SHOP_WEP_MPPB:
		
		case SHOP_WEP_GRENADE:
		case SHOP_WEP_ROTARYGL:
		case SHOP_WEP_HEAVYML:
		case SHOP_WEP_SEDRIN:
		
		case SHOP_WEP_RHINO:
		case SHOP_WEP_NAIL:
		case SHOP_WEP_BASILISK:
		
		case SHOP_WEP_GAUSS:
		case SHOP_WEP_RAIL:
		case SHOP_WEP_DEATHRAY:
		
		case SHOP_WEP_DEATHSTAFF:
		case SHOP_WEP_RAZOR:
		case SHOP_WEP_SUN:
		case SHOP_WEP_REAVER:
		return true;
	}
	return false;
}

int ShopTableIdToWeaponTableId(int id) {
	// skip classic weapons
	int skip = 0;
	if(id >= SHOP_WEAPON1_BEGIN)
		skip += 2; // fist & chainsaw
	if(id >= SHOP_WEAPON2_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON31_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON3SSG_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON41_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON51_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON61_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON7_BEGIN)
		skip += 1;
	return id + skip;
}

str GetWeaponCondition(int id) {
	bool islux = IsLuxuryWeapon(id);
	int real_id = ShopTableIdToWeaponTableId(id);
	int slot = GetSlotOfWeapon(real_id);
	
	if(slot <= 1) 
		return WeaponConditionCheckers[2 * slot + islux];
	else if(slot == 2) // checker3, checker3x, checker3L so we skip 2 for lux check
		return WeaponConditionCheckers[2 * slot + islux * 2];
	// we have to count for the fact we have an extra checker at slot 3 so -1
	return WeaponConditionCheckers[2 * slot + islux - 1];
}

// I could probably make these much finer with one function and multiple checks in it or better yet an array to look up the strings themselves
// well, later on I probably should but I like this more as each has their unique purpose and its immediately obvious
str GetArmorTag(int id) {
	return StrParam(s:"DND_ARMOR", d:id + 1);
}

str GetArmorExplanation(int id) {
	return StrParam(s:"DND_MENU_ARMTEXT", d:id + 1);
}

// regular ammo, then special shells then special grenades
str GetAmmoTag(int id) {
	if(id < SHOP_FIRSTAMMOSPECIAL_INDEX) {
		int temp = ShopAmmoIndexToRegularAmmoIndex(id - SHOP_FIRSTAMMO_INDEX);
		return StrParam(s:"DND_AMMO_", d:temp >> 16, s:"_", d:temp & 0xFFFF);
	}
	if(id < SHOP_FIRSTAMMOSPECIAL_GRENADE_INDEX)
		return StrParam(s:"DND_AMMOSPECIAL", d:id - SHOP_FIRSTAMMOSPECIAL_INDEX + 1);
	return StrParam(s:"DND_AMMOSPECIALG", d:id - SHOP_FIRSTAMMOSPECIAL_GRENADE_INDEX + 1);
}

str GetAbilityTag(int id) {
	return StrParam(s:"DND_MENU_ABLTAG", d:id + 1);
}

str GetAbilityHelpText(int id) {
	return StrParam(s:"DND_MENU_ABLHELPTEXT", d:id + 1);
}

str GetArtifactTag(int id) {
	return StrParam(s:"DND_ARTI", d:id + 1);
}

str GetArtifactText(int id) {
	return StrParam(s:"DND_MENU_ARTHELPTEXT", d:id + 1);
}

str GetAttributeText(int id) {
	return StrParam(s:"DND_MENU_ATTRTEXT", d:id + 1);
}

str GetAccountPurchaseTag(int id) {
	return StrParam(s:"DND_MENU_ACCITEMTAG", d:id + 1);
}

str GetAccountPurchaseText(int id) {
	return StrParam(s:"DND_MENU_ACCITEMTEXT", d:id + 1);
}

enum {
	DTYPE_PHYSICAL = 1,
	DTYPE_MELEE = 2,
	DTYPE_OCCULT = 4,
	DTYPE_EXPLOSIVE = 8,
	DTYPE_ENERGY = 16,
	DTYPE_ELEMENTAL = 32
};

#define MAX_DAMAGE_TYPES 6 // 64
str DamageTypeIcons[MAX_DAMAGE_TYPES] = { 
	"DT_PHYS",
	"DT_MELE",
	"DT_OCCU",
	"DT_EXPL",
	"DT_ENER",
	"DT_ELEM"
};

int WeaponDamageTypes[MAXSHOPWEAPONS] = {
	// 1
	DTYPE_MELEE,
	DTYPE_MELEE,
	DTYPE_MELEE | DTYPE_OCCULT,
	DTYPE_MELEE | DTYPE_ELEMENTAL,
	DTYPE_MELEE | DTYPE_OCCULT,
	DTYPE_MELEE | DTYPE_OCCULT,
	
	// 2
	DTYPE_PHYSICAL,
	DTYPE_PHYSICAL,
	DTYPE_ENERGY,
	DTYPE_PHYSICAL,
	DTYPE_OCCULT,
	DTYPE_ELEMENTAL,
	DTYPE_PHYSICAL,
	
	// 3 - 1
	DTYPE_PHYSICAL,
	DTYPE_PHYSICAL,
	DTYPE_ELEMENTAL,
	DTYPE_PHYSICAL,
	DTYPE_ELEMENTAL,
	DTYPE_OCCULT | DTYPE_MELEE,
	DTYPE_ELEMENTAL,
	DTYPE_PHYSICAL,
	
	
	// 3 - 2
	DTYPE_PHYSICAL,
	DTYPE_EXPLOSIVE | DTYPE_OCCULT,
	DTYPE_ENERGY,
	DTYPE_ELEMENTAL,
	DTYPE_PHYSICAL | DTYPE_ELEMENTAL,
	DTYPE_PHYSICAL | DTYPE_OCCULT,
	DTYPE_PHYSICAL | DTYPE_EXPLOSIVE,
	DTYPE_OCCULT,
	
	// 4 - 1
	DTYPE_PHYSICAL,
	DTYPE_ENERGY,
	DTYPE_OCCULT,
	DTYPE_PHYSICAL,
	DTYPE_PHYSICAL,
	DTYPE_ELEMENTAL,
	DTYPE_ENERGY,
	DTYPE_ELEMENTAL,
	
	// 4 - 2
	DTYPE_ELEMENTAL,
	DTYPE_PHYSICAL,
	DTYPE_OCCULT,
	DTYPE_ENERGY,
	
	// 5 - 1
	DTYPE_EXPLOSIVE,
	DTYPE_EXPLOSIVE,
	DTYPE_PHYSICAL | DTYPE_EXPLOSIVE,
	DTYPE_OCCULT,
	DTYPE_EXPLOSIVE | DTYPE_PHYSICAL,
	DTYPE_ELEMENTAL,
	DTYPE_PHYSICAL,
	DTYPE_ENERGY,
	
	// 5 - 2
	DTYPE_EXPLOSIVE,
	DTYPE_EXPLOSIVE,
	DTYPE_EXPLOSIVE,
	DTYPE_ENERGY | DTYPE_ELEMENTAL,
	
	// 6 - 1
	DTYPE_ENERGY,
	DTYPE_ENERGY,
	DTYPE_ELEMENTAL,
	DTYPE_ELEMENTAL,
	DTYPE_ELEMENTAL,
	DTYPE_ENERGY,
	DTYPE_OCCULT,
	
	// 6 - 2
	DTYPE_PHYSICAL,
	DTYPE_OCCULT,
	DTYPE_OCCULT,
	
	// 7
	DTYPE_ENERGY,
	DTYPE_EXPLOSIVE,
	DTYPE_ENERGY,
	DTYPE_ENERGY,
	DTYPE_ELEMENTAL,
	DTYPE_ENERGY | DTYPE_EXPLOSIVE,
	DTYPE_ENERGY,
	DTYPE_ENERGY | DTYPE_EXPLOSIVE,
	
	// 8
	DTYPE_OCCULT,
	DTYPE_OCCULT,
	DTYPE_OCCULT,
	DTYPE_OCCULT
};

struct draw_info WeaponDrawInfo[MAXSHOPWEAPONS] = {
	// 1
	{ OBJ_WEP | OBJ_HASCHOICE, 												-1 						},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL,								-1						},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_EXCALIBAT		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_INFERNOSWORD	},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_DUSKBLADE		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_SICKLE			},
	
	// 2
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_AKIMBOPISTOL	},
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_MAGNUM			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL,								SHOP_WEP_LASERPISTOL	},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESPIS1		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESPIS2		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RUBYWAND		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_SCATTERPISTOL	},
	
	// 3 - 1
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_SHOTGUN,								SHOP_WEP_PURIFIER		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_SHOTGUN,								SHOP_WEP_AUTOSG			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL | OBJ_SHOTGUN,				SHOP_WEP_EMERALDWAND	},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_SHOTGUN,					SHOP_WEP_RESSG1			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_SHOTGUN,					SHOP_WEP_RESSG2			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL | OBJ_SHOTGUN,	SHOP_WEP_RESSG3			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESSG4			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_SHOTGUN,								SHOP_WEP_HSSG			},
	
	// 3 - 2
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_SHOTGUN,								SHOP_WEP_ERASUS			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL | OBJ_SHOTGUN,				SHOP_WEP_HELLSMAW		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_SHOTGUN,					SHOP_WEP_RESSSG1		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_SHOTGUN,					SHOP_WEP_RESSSG2		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL | OBJ_SHOTGUN,	SHOP_WEP_RESSSG3		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL | OBJ_SHOTGUN,	SHOP_WEP_RESSSG4		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_SHOTGUN,					SHOP_WEP_SILVER			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL | OBJ_SHOTGUN,	SHOP_WEP_SLAYER			},
	
	// 4 - 1
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_HMG			},
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_LEAD			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL,								SHOP_WEP_DSEAL			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESMG1			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESMG2			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESMG3			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESMG4			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESMG5			},
	
	// 4 - 2
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_DESOLATOR		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_MINIGUN		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_EBONY			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_MPPB			},
	
	// 5 - 1
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_TORPEDO		},
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_MERC			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL | OBJ_SHOTGUN,				SHOP_WEP_VINDICATOR		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESRL1			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESRL2			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESRL3			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESRL4			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESRL5			},
	
	// 5 - 2
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_GRENADE		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_ROTARYGL		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_HEAVYML		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_SEDRIN			},
	
	// 6 - 1
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_NUCLEARPL		},
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_TUREL			},
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_FROSTFANG		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESPL1			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESPL2			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESPL3			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESPL4			},
	
	// 6 - 2
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RHINO			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_NAIL			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_BASILISK		},
	
	// 7
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_BFG			},
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_DEVA			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL,								SHOP_WEP_MFG			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESBFG1		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESBFG2		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_GAUSS			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RAIL			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_DEATHRAY		},
	
	// 8
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_DEATHSTAFF		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RAZOR			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_SUN			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_REAVER			}
};

struct draw_info ArtifactDrawInfo[MAXARTIFACTS] = {
	{ OBJ_ARTI | OBJ_HASCHOICE, 					        -1 										    },
	{ OBJ_ARTI | OBJ_HASCHOICE | OBJ_RESEARCH, 		        SHOP_ARTI_SALVATE				            },
	{ OBJ_ARTI | OBJ_HASCHOICE,								-1											},
	{ OBJ_ARTI | OBJ_HASCHOICE | OBJ_RESEARCH,		        SHOP_ARTI_BLOOD				                },
	{ OBJ_ARTI | OBJ_HASCHOICE | OBJ_RESEARCH,		        SHOP_ARTI_TRIPLE					        },
	{ OBJ_ARTI | OBJ_HASCHOICE | OBJ_RESEARCH,		        SHOP_ARTI_VORTEX				            },
	{ OBJ_ARTI | OBJ_HASCHOICE | OBJ_RESEARCH,		        SHOP_ARTI_BOOK					            },
	{ OBJ_ARTI | OBJ_HASCHOICE,								-1											},
	{ OBJ_ARTI | OBJ_HASCHOICE,								-1											},
	{ OBJ_ARTI | OBJ_HASCHOICE,								-1											}
};

// This mapper is used when transitioning from ammo tables to menu tables
int MenuAmmoIndexMap[MAX_SLOTS][MAX_AMMOTYPES_PER_SLOT] = {
	// category 1
	{
		SHOP_AMMO_CLIP,
		SHOP_AMMO_RUBY,
		SHOP_AMMO_VIPER,
		SHOP_AMMO_DSEAL,
		SHOP_AMMO_RIOTSHELL,
		SHOP_AMMO_ACID,
		SHOP_AMMO_FUSION,
		SHOP_AMMO_INCINERATOR,
		SHOP_AMMO_DESOLATOR,
		SHOP_AMMO_EBONY,
		SHOP_AMMO_EBONYX,
	},
	// category 2
	{
		SHOP_AMMO_SHELL,
		SHOP_AMMO_EMERALDMANA,
		SHOP_AMMO_HELLSMAW,
		SHOP_AMMO_PCAN,
		SHOP_AMMO_NITROGEN,
		SHOP_AMMO_DEMONBLOOD,
		SHOP_AMMO_CHARON,
		SHOP_AMMO_HADES,
		SHOP_AMMO_FLAYER,
		SHOP_AMMO_EXPSHELL,
		SHOP_AMMO_SLAYER,
	},
	// category 3
	{
		SHOP_AMMO_ROCKET,
		SHOP_AMMO_FLAK,
		SHOP_AMMO_METEOR,
		SHOP_AMMO_HEAVYGRENADE,
		SHOP_AMMO_GRAVDIS,
		SHOP_AMMO_GL,
		SHOP_AMMO_MIS,
		SHOP_AMMO_SEDRIN,
		-1
	},
	// category 4
	{
		SHOP_AMMO_CELL,
		SHOP_AMMO_DEVASTATOR,
		SHOP_AMMO_EVERICE,
		SHOP_AMMO_FUEL,
		SHOP_AMMO_LG,
		SHOP_AMMO_NAIL,
		SHOP_AMMO_BASILISK,
		SHOP_AMMO_ION,
		SHOP_AMMO_THUNDER,
		SHOP_AMMO_GAUSS,
		-1
	},
	// category 5 (not buyable -- souls etc)
	{
		-1
	}
};

int ShopAmmoIndexToRegularAmmoIndex(int id) {
	// we exclude soul ammo types
	for(int i = 0; i < MAX_SLOTS - 1; ++i) {
		for(int j = 0; j < MAX_AMMOTYPES_PER_SLOT; ++j) {
			if(MenuAmmoIndexMap[i][j] - SHOP_FIRSTAMMO_INDEX == id)
				return j | (i << 16);
		}
	}
	return -1;
}

struct draw_info AmmoDrawInfo[MAXSHOPAMMOS] = {
	{ OBJ_AMMO,													SHOP_AMMO_CLIP 				},
	{ OBJ_AMMO,													SHOP_AMMO_SHELL				},
	{ OBJ_AMMO,													SHOP_AMMO_ROCKET			},
	{ OBJ_AMMO,													SHOP_AMMO_CELL				},
	{ OBJ_AMMO | OBJ_RESEARCH | OBJ_RESEARCH_ATLEASTONE,		SHOP_AMMO_EXPSHELL			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_EBONY				},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_EBONYX			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_MIS				},
	{ OBJ_AMMO | OBJ_RESEARCH | OBJ_RESEARCH_ATLEASTONE,		SHOP_AMMO_GL				},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_NAIL				},
	
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_BASILISK			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_GAUSS				},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_SLAYER			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_RUBY				},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_PCAN				},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_RIOTSHELL			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_METEOR			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_FUEL				},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_LG				},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_NITROGEN			},
	
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_ION				},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_ACID				},
	{ OBJ_AMMO,													SHOP_AMMO_EVERICE			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_VIPER				},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_THUNDER			},
	{ OBJ_AMMO,													SHOP_AMMO_DSEAL				},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_FUSION			},
	{ OBJ_AMMO,													SHOP_AMMO_FLAK				},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_DESOLATOR			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_HADES				},
	
	{ OBJ_AMMO | OBJ_RESEARCH | OBJ_RESEARCH_ATLEASTONE,		SHOP_AMMO_DEMONBLOOD		},
	{ OBJ_AMMO,													SHOP_AMMO_EMERALDMANA		},
	{ OBJ_AMMO,													SHOP_AMMO_HELLSMAW			},
	{ OBJ_AMMO,													SHOP_AMMO_DEVASTATOR		},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_HEAVYGRENADE		},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_FLAYER			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_GRAVDIS			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_SEDRIN			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_CHARON			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_INCINERATOR		},
	
	// special ammos
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_FLECHETTE			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_PIERCING			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_ELECTRIC			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_NITROSHELL		},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_SLUGSHELL			},
	
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_SONICGRENADE		},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_HEGRENADE			}
};

#define AMMOID_COUNT 0
#define AMMOID_WEPREF 1			
int AmmoCounts[MAXSHOPAMMOS][2] = {
	{ 25, -1 },
	{ 10, -1 },
	{ 5, -1 },
	{ 40, -1 },
	
	{ 8, DND_WEAPON_SILVERGUN },
	{ 8, DND_WEAPON_EBONYCANNON },
	{ 4, DND_WEAPON_EBONYCANNON },
	{ 4, DND_WEAPON_HEAVYMISSILELAUNCHER },
	{ 5, DND_WEAPON_GRENADELAUNCHER },
	{ 15, DND_WEAPON_NAILGUN },
	
	{ 20, DND_WEAPON_BASILISK },
	{ 5, DND_WEAPON_GAUSSRIFLE },
	{ 6, DND_WEAPON_SLAYER },
	{ 17, DND_WEAPON_RUBYWAND },
    { 4, DND_WEAPON_PLASMACANNON },
    { 8, DND_WEAPON_RIOTCANNON },
    { 5, DND_WEAPON_METEORLAUNCHER },
    { 30, DND_WEAPON_FLAMETHROWER },
    { 25, DND_WEAPON_LIGHTNINGGUN },
    { 8, DND_WEAPON_NITROGENCROSSBOW },
	
    { 18, DND_WEAPON_IONCANNON },
	{ 16, DND_WEAPON_ACIDRIFLE },
	{ 25, DND_WEAPON_FROSTFANG | (DND_WEAPON_FREEZER << 16) },
	{ 4, DND_WEAPON_VIPERSTAFF },
	{ 10, DND_WEAPON_THUNDERSTAFF },
	{ 25, DND_WEAPON_DEMONSEALER },
	{ 30, DND_WEAPON_FUSIONBLASTER },
	{ 5, DND_WEAPON_VINDICATOR },
	{ 20, DND_WEAPON_DESOLATOR },
	{ 6, DND_WEAPON_HADES },
	
	{ 12, DND_WEAPON_WHEELOFTORMENT | (DND_WEAPON_DARKLANCE << 16) },
	{ 18, DND_WEAPON_EMERALDWAND },
	{ 10, DND_WEAPON_HELLSMAW },
	{ 33, DND_WEAPON_DEVASTATOR },
	{ 5, DND_WEAPON_HEAVYGL },
	{ 5, DND_WEAPON_CROSSBOW },
	{ 5, DND_WEAPON_GRAVDIS },
	{ 6, DND_WEAPON_SEDRINSTAFF },
	{ 10, DND_WEAPON_CHARONBLASTER },
	{ 8, DND_WEAPON_INCINERATOR },
	
	// special
	{ 8, -1 },
	{ 8, -1 },
	{ 8, -1 },
    { 8, -1 },
	{ 8, -1 },
	
	{ 5, -1 },
	{ 5, -1 }
};

void DrawAmmoExplanation(int itemid) {
	int ammo_id = itemid - SHOP_FIRSTAMMO_INDEX;
	int ammo_ref = AmmoCounts[ammo_id][AMMOID_WEPREF];
	
	// has 2 weapons -- we can extend this further in the future maybe with an array or something
	if(ammo_ref < 65536) {
		// exception for "no weapon specified"
		if(itemid <= SHOP_AMMO_CELL || itemid >= SHOP_AMMO_FLECHETTE)
			HudMessage(s:"\cd*\c- ", l:"DND_MENU_GIVES", s:" \cf", d:GetAmmoToGive(itemid), s:"\c- ", l:StrParam(s:"DND_MENU_AMMOTEXT", d:ammo_id + 1); HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 248.1, 0.0, 0.0);
		else
			HudMessage(s:"\cd*\c- ", l:"DND_MENU_GIVES", s:" \cf", d:GetAmmoToGive(itemid), s:"\c- ", l:StrParam(s:"DND_MENU_AMMOTEXT", d:ammo_id + 1), s: " ", l:"DND_MENU_FOR", s:"\cd", l:GetWeaponTag(ammo_ref), s:"\c-."; HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 248.1, 0.0, 0.0);
	}
	else
		HudMessage(
			s:"\cd*\c- ", l:"DND_MENU_GIVES", s:" \cf", d:GetAmmoToGive(itemid), s:"\c- ",
			l:StrParam(s:"DND_MENU_AMMOTEXT", d:ammo_id + 1), s: " ", 
			l:"DND_MENU_FOR", s:"\cd", l:GetWeaponTag(ammo_ref & 0xFFFF), s:"\c-", l:"DND_MENU_AND", s:"\cd", l:GetWeaponTag(ammo_ref >> 16), s:"\c-."; 
			HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 248.1, 0.0, 0.0
		);
}

struct draw_info ArmorDrawInfo[MAXARMORS] = {
	{ OBJ_ARMOR, 								-1 										},
	{ OBJ_ARMOR, 								-1										},
	{ OBJ_ARMOR, 								-1 										},
	{ OBJ_ARMOR, 								-1 										},
	{ OBJ_ARMOR | OBJ_RESEARCH, 		SHOP_ARMOR_GUNSLINGER	 					    },
	{ OBJ_ARMOR | OBJ_RESEARCH, 		SHOP_ARMOR_OCCULT	 							},
	{ OBJ_ARMOR | OBJ_RESEARCH, 		SHOP_ARMOR_DEMO	 								},
	{ OBJ_ARMOR | OBJ_RESEARCH, 		SHOP_ARMOR_ENERGY	 							},
    { OBJ_ARMOR | OBJ_RESEARCH,         SHOP_ARMOR_ELEMENTAL                            },
    
    { OBJ_ARMOR | OBJ_RESEARCH,         SHOP_ARMOR_MONOLITH                             },
	{ OBJ_ARMOR | OBJ_RESEARCH, 		SHOP_ARMOR_CYBERNETIC		 					},
	{ OBJ_ARMOR | OBJ_RESEARCH, 		SHOP_ARMOR_DUELIST	 							},
	{ OBJ_ARMOR | OBJ_RESEARCH, 		SHOP_ARMOR_NECRO		 						},
	{ OBJ_ARMOR | OBJ_RESEARCH, 		SHOP_ARMOR_KNIGHT	 							},
	{ OBJ_ARMOR | OBJ_RESEARCH, 		SHOP_ARMOR_RAVAGER		 						},
	{ OBJ_ARMOR | OBJ_RESEARCH, 		SHOP_ARMOR_SYNTHMETAL		 					}
};

struct draw_info AbilityDrawInfo[MAXABILITIES] = {
	{ OBJ_ABILITY, 								SHOP_ABILITY_KICK 								},
	{ OBJ_ABILITY, 								SHOP_ABILITY_RELOAD								},
	{ OBJ_ABILITY, 								SHOP_ABILITY_DASH 								},
	{ OBJ_ABILITY | OBJ_RESEARCH, 				SHOP_ABILITY_ARCANE	 					    	},
	{ OBJ_ABILITY | OBJ_RESEARCH, 				SHOP_ABILITY_POISON	 							},
	{ OBJ_ABILITY | OBJ_RESEARCH, 				SHOP_ABILITY_EXPLOSION	 						},
	{ OBJ_ABILITY | OBJ_RESEARCH, 				SHOP_ABILITY_HEART	 							},
    { OBJ_ABILITY | OBJ_RESEARCH,         		SHOP_ABILITY_REGEN                           	},
    { OBJ_ABILITY | OBJ_RESEARCH,         		SHOP_ABILITY_TEMPORAL                           },
	{ OBJ_ABILITY | OBJ_RESEARCH, 				SHOP_ABILITY_SOUL		 						}
};

typedef struct {
	int res_id;
	int res_number;
	int res_cost;
} res_info_T;

enum {
	RESPAGE_BODY,
	RESPAGE_AMMO,
	RESPAGE_SLOTGUNS,
	RESPAGE_LUXURYGUNS,
	RESPAGE_UTILITY,
};

#define MENU_MAXRES_PERPAGE 32
res_info_T ResearchInfo[MENU_MAXRES_PAGES][MENU_MAXRES_PERPAGE] = {
	// body
	{
		{
			RES_NANOTECH, 3616, 85
		},
		{
			RES_OCCULTABILITY, 9999, 105
		},
		{
			RES_BIO1, 7452, 50
		},
		{
			RES_BIO2, 7453, 85
		},
		{
			RES_BIO3, 7454, 120
		},
		{
			RES_EXO1, 7460, 50
		},
		{
			RES_EXO2, 7461, 85
		},
		{
			RES_EXO3, 7462, 120
		},
		{
			RES_IMP1, 8006, 45
		},
		{
			RES_IMP2, 8007, 75
		},
		{
			RES_IMP3, 8008, 105
		},
		{
			-1, -1, -1
		}
	},
	// special ammo types
	{
		{
			RES_FLECHETTE, 2302, 40
		},
		{
			RES_PIERCING, 2303, 45
		},
		{
			RES_ELECTRIC, 2304, 45
		},
		{
			RES_NITRO, 2309, 45
		},
		{
			RES_SLUGSHELL, 2316, 50
		},
		{
			RES_SONICGRENADE, 2411, 55
		},
		{
			RES_HEGRENADE, 2412, 55
		},
		{
			RES_DOUBLESPECIALCAP, 2440, 75
		},
		{
			-1, -1, -1
		}
	},
	// slot guns
	{
		{
			RES_SLOT1UPG1, 6709, 50
		},
		{
			RES_SLOT1UPG2, 6711, 45
		},
		{
			RES_SLOT2UPG1, 6880, 40
		},
		{
			RES_SLOT2UPG2, 7982, 50
		},
		{
			RES_SLOT3UPG1, 7991, 45
		},
		{
			RES_SLOT3UPG2, 8015, 50
		},
		{
			RES_SLOT3UPG3, 8278, 60
		},
		{
			RES_SLOT3UPG4, 8619, 65
		},
		{
			RES_SLOT3SSGUPG1, 7995, 50
		},
		{
			RES_SLOT3SSGUPG2, 8001, 55
		},
		{
			RES_SLOT3SSGUPG3, 8671, 65
		},
		{
			RES_SLOT3SSGUPG4, 9603, 60
		},
		{
			RES_SLOT4UPG1, 8010, 45
		},
		{
			RES_SLOT4UPG2, 9507, 60
		},
		{
			RES_SLOT4UPG3, 8763, 55
		},
		{
			RES_SLOT4UPG4, 8277, 60
		},
		{
			RES_SLOT4UPG5, 8419, 65
		},
		{
			RES_SLOT5UPG1, 8433, 50
		},
		{
			RES_SLOT5UPG2, 8219, 55
		},
		{
			RES_SLOT5UPG3, 7066, 50
		},
		{
			RES_SLOT5UPG4, 8519, 65
		},
		{
			RES_SLOT5UPG5, 9135, 60
		},
		{
			RES_SLOT5GL, 8216, 10
		},
		{
			RES_SLOT6UPG1, 4811, 50
		},
		{
			RES_SLOT6UPG2, 8566, 55
		},
		{
			RES_SLOT6UPG3, 9104, 60
		},
		{
			RES_SLOT6UPG4, 9464, 65
		},
		{
			RES_SLOT7UPG1, 5395, 60
		},
		{
			RES_SLOT7UPG2, 5411, 65
		},
		{
			-1, -1, -1
		}
	},
	// luxury guns
	{
		{
			RES_SLOT1OCCULT, 611, 40
		},
		{
			RES_SLOT2LUXURY, 640, 40
		},
		{
			RES_SLOT3LUXURY, 783, 45
		},
		{
			RES_SLOT4LUXURY, 831, 45
		},
		{
			RES_SLOT5LUXURY, 895, 45
		},
		{
			RES_SLOT6LUXURY, 899, 45
		},
		{
			RES_SLOT7LUXURY, 909, 55
		},
		{
			RES_SLOT8REVEAL, 7744, 140
		},
		{
			-1, -1, -1
		}
	},
	// utility
	{
		{
			RES_RAREARMOR, 4301, 60
		},
		{
			RES_SUPERARMOR, 4300, 80
		},
		{
			RES_SYNTHMETALARMOR, 4675, 75
		},
		{
			RES_SYNTHMASK, 4713, 50
		},
		{
			RES_MEDKITSTORE, 3531, 55
		},
		{
			RES_ACCESSORY, 6914, 105
		},
		{
			RES_OCCULTARTIFACT, 4569, 75
		},
		{
			RES_STASHTAB, 9704, 300
		},
		{
			-1, -1, -1
		}
	}
};

str ResearchIcons[MENU_MAXRES_PAGES][MENU_MAXRES_PERPAGE] = {
	// body
	{
		"RESBAK29",
		"RESBAK27",
		"RESBAK45",		
		"RESBAK46",
		"RESBAK47",
		"RESBAK48",
		"RESBAK49",
		"RESBAK50",
		"RESBAK51",
		"RESBAK52",
		"RESBAK53"
	},
	// special ammo types
	{
		"RESBAK5",
		"RESBAK6",
		"RESBAK7",
		"RESBAK33",
		"RESBAK54",
		"RESBAK9",
		"RESBAK8",
		"RESBAK10"
	},
	// slot guns
	{
		// slot 1
		"RESBAK34",
		"RESBAK35",
		
		// slot 2
		"RESBAK18",
		"RESBAK41",
		
		// slot 3 -- sg
		"RESBAK19",
		"RESBAK31",
		"RESBAK55",
		"RESBAK60",
		
		// slot 3 -- ssg
		"RESBAK20",
		"RESBAK37",
		"RESBAK44",
		"RESBAK57",
		
		// slot 4
		"RESBAK21",
		"RESBAK32",
		"RESBAK36",
		"RESBAK42",
		"RESBAK61",
		
		// slot 5
		"RESBAK22",
		"RESBAK39",
		"RESBAK40",
		"RESBAK58",
		"RESBAK59",
		"RESBAK23",
		
		// slot 6
		"RESBAK24",
		"RESBAK25",
		"RESBAK43",
		"RESBAK62",
		
		// slot 7
		"RESBAK26",
		"RESBAK38"
	},
	// luxury guns
	{
		"RESBAK11",
		"RESBAK12",
		"RESBAK13",
		"RESBAK14",
		"RESBAK15",
		"RESBAK16",
		"RESBAK17",
		"RESBAK28"
	},
	// utility
	{
		"RESBAK1",
		"RESBAK2",
		"RESBAK63",
		"RESBAK64",
		"RESBAK3",
		"RESBAK4",
		"RESBAK30",
		"RESBAK56"
	}
};

// put only those that concern menu here
#define MAX_SPECIALAMMOFIX_WEAPONS 5
int SpecialAmmoFixWeapons[MAX_SPECIALAMMOFIX_WEAPONS][3] = {
    { SHOP_WEP_PURIFIER,            SPWEP_SG,           AMMO_TYPE_SHELL         },
    { SHOP_WEP_RESSG1,              SPWEP_SG,           AMMO_TYPE_SHELL         },
    { SHOP_WEP_RESMG1,              SPWEP_MG,           AMMO_TYPE_MGGRENADE     },
    { SHOP_WEP_RESMG2,              SPWEP_MG,           AMMO_TYPE_SHELL2        },
    { SHOP_WEP_GRENADE,             SPWEP_GL,           AMMO_TYPE_GRENADE       }
};

enum {
	POPUP_NONE,
	POPUP_NOFUNDS,
	POPUP_CANTBUY,
	POPUP_ALREADYOWN,
	POPUP_DONTOWN,
	POPUP_NEEDRESEARCH,
	POPUP_NOBUDGET,
	POPUP_NEEDDISCOVER,
	POPUP_ALREADYRESEARCHED,
	POPUP_MAXACCESSORYEQUIPPED,
	POPUP_NOTALENTPOINT,
	POPUP_ACCESSORYNOTFOUND,
	POPUP_CHARMMISMATCH,
	POPUP_NOITEMTHERE,
	POPUP_TARGETISTRADING,
	POPUP_YOUARETRADING,
	POPUP_NOSPACEFORTRADE,
	POPUP_YOUAREMUTED,
	POPUP_ITEMNEEDSTARGET,
	POPUP_MATERIALCANTUSE,
	POPUP_ITEMTYPEMISMATCH,
	POPUP_NOSPOTFORITEM,
	POPUP_POINTLESSARMOR,
	POPUP_OUTOFSTOCK,
	POPUP_NOARMORWORN,
	POPUP_ARMORISFULL,
	POPUP_NOREPAIRTOKENS,
	POPUP_YOUARENTALLOWED
};

#define MAX_POPUPS (POPUP_YOUARENTALLOWED + 1)

str GetPopupText(int id) {
	switch(id) {
		case POPUP_NOFUNDS:
		return "DND_POPUP_NOFUNDS";
		case POPUP_CANTBUY:
		return "DND_POPUP_CANTBUY";
		case POPUP_ALREADYOWN:
		return "DND_POPUP_ALREADYOWN";
		case POPUP_DONTOWN:
		return "DND_POPUP_DONTOWN";
		case POPUP_NEEDRESEARCH:
		return "DND_POPUP_NEEDRESEARCH";
		case POPUP_NOBUDGET:
		return "DND_POPUP_NOBUDGET";
		case POPUP_NEEDDISCOVER:
		return "DND_POPUP_NEEDDISCOVER";
		case POPUP_ALREADYRESEARCHED:
		return "DND_POPUP_ALREADYRESEARCHED";
		case POPUP_MAXACCESSORYEQUIPPED:
		return "DND_POPUP_MAXACCESSORYEQUIPPED";
		case POPUP_NOTALENTPOINT:
		return "DND_POPUP_NOTALENTPOINT";
		case POPUP_ACCESSORYNOTFOUND:
		return "DND_POPUP_ACCESSORYNOTFOUND";
		case POPUP_CHARMMISMATCH:
		return "DND_POPUP_CHARMMISMATCH";
		case POPUP_NOITEMTHERE:
		return "DND_POPUP_NOITEMTHERE";
		case POPUP_TARGETISTRADING:
		return "DND_POPUP_TARGETISTRADING";
		case POPUP_YOUARETRADING:
		return "DND_POPUP_YOUARETRADING";
		case POPUP_NOSPACEFORTRADE:
		return "DND_POPUP_NOSPACEFORTRADE";
		case POPUP_YOUAREMUTED:
		return "DND_POPUP_YOUAREMUTED";
		case POPUP_ITEMNEEDSTARGET:
		return "DND_POPUP_ITEMNEEDSTARGET";
		case POPUP_MATERIALCANTUSE:
		return "DND_POPUP_MATERIALCANTUSE";
		case POPUP_ITEMTYPEMISMATCH:
		return "DND_POPUP_ITEMTYPEMISMATCH";
		case POPUP_NOSPOTFORITEM:
		return "DND_POPUP_NOSPOTFORITEM";
		case POPUP_POINTLESSARMOR:
		return "DND_POPUP_POINTLESSARMOR";
		case POPUP_OUTOFSTOCK:
		return "DND_POPUP_OUTOFSTOCK";
		case POPUP_NOARMORWORN:
		return "DND_POPUP_NOARMORWORN";
		case POPUP_ARMORISFULL:
		return "DND_POPUP_ARMORISFULL";
		case POPUP_NOREPAIRTOKENS:
		return "DND_POPUP_NOREPAIRTOKENS";
		case POPUP_YOUARENTALLOWED:
		return "DND_POPUP_YOUARENTALLOWED";
	}
	return "";
}

// LegendaryKills
#define MAX_LEGMONS_TEXT 5

str LegendaryMonsterIcons[MAX_LEGMONS_TEXT] = {
	"LEG_DRMR",
	"LEG_TORR",
	"LEG_MORD",
	"LEG_GODS",
	"LEG_GOLG"
};

#define DND_TRAIT_STARTSHOW FIRST_MONSTER_TRAIT // first trait to show
#define DND_TRAIT_LASTSHOW LAST_MONSTER_TRAIT
#define MAX_MONSTER_MODS (DND_TRAIT_LASTSHOW - DND_TRAIT_STARTSHOW + 1)

// NOTE: UPDATE THESE VALUES WHEN ADDING NEW MODS
#define MAX_WEAKNESS_MODS 6

#define MAX_RESIST_MODS 9
#define RESMODOFFSET MAX_WEAKNESS_MODS

#define MAX_IMMUNITY_MODS 10
#define IMMMODOFFSET (RESMODOFFSET + MAX_RESIST_MODS)

#define MAX_AGGRESSIVE_MODS 12
#define AGGMODOFFSET (IMMMODOFFSET + MAX_IMMUNITY_MODS)

#define MAX_DEFENSE_MODS 17
#define DEFMODOFFSET (AGGMODOFFSET + MAX_AGGRESSIVE_MODS)

#define MAX_UTILITY_MODS 14
#define UTIMODOFFSET (DEFMODOFFSET + MAX_DEFENSE_MODS)

// input is menu based index, returns corresponding label index for the mod
int MonsterModGroupMapper[MAX_MONSTER_MODS] = {
	// weaknesses
	DND_ENERGY_WEAKNESS,
	DND_SILVER_WEAKNESS,
	DND_FIRE_WEAKNESS,
	DND_ICE_WEAKNESS,
	DND_MAGIC_WEAKNESS,
	DND_ELEMENTAL_WEAKNESS,
	
	// resists
	DND_EXPLOSIVE_RESIST,
	DND_BULLET_RESIST,
	DND_ENERGY_RESIST,
	DND_MAGIC_RESIST,
	DND_ELEMENTAL_RESIST,
	DND_ICECREATURE,
	DND_FIRECREATURE,
	DND_STONECREATURE,
	DND_EARTHCREATURE,
	
	// immune
	DND_EXPLOSIVE_IMMUNE,
	DND_EXPLOSIVE_NONE,
	DND_BULLET_IMMUNE,
	DND_ENERGY_IMMUNE,
	DND_MAGIC_IMMUNE,
	DND_ELEMENTAL_IMMUNE,
	DND_VENOMANCER,
	DND_FRIGID,
	DND_SCORCHED,
	DND_INSULATED,
	
	// aggression
	DND_RAGE,
	DND_PIERCE,
	DND_AGGRESSIVE,
	DND_EXTRAFAST,
	DND_FASTREACTION,
	DND_EXTRASTRONG,
	DND_HOMING,
	DND_ARMORPEN,
	DND_VIOLENTRETALIATION,
	DND_HATRED,
	DND_VAMPIRISM,
	DND_RUINATION,
	
	// defense
	DND_GHOST,
	DND_HARDENED_SKIN,
	DND_REFLECTIVE,
	DND_MINIONS,
	DND_HEAL,
	DND_BLOCK,
	DND_SPLIT,
	DND_DEATH,
	DND_VITAL,
	DND_ISBLOCKING,
	DND_BLOODLESS,
	DND_REBIRTH,
	DND_REJUVENATING,
	DND_FORTIFIED,
	DND_SUBORDINATE,
	DND_REPEL,
	DND_PHANTASM,
	
	// utility
	DND_CURSE,
	DND_RAISE,
	DND_TELEPORT,
	DND_RESURRECT,
	DND_MOBILITY,
	DND_POISON,
	DND_NOPAIN,
	DND_PET,
	DND_SUMMONED,
	DND_REVIVED,
	DND_THIEF,
	DND_HEXFUSION,
	DND_SHOCKER,
	DND_CRIPPLE,
	
	DND_MARKOFCHAOS,
	
	DND_LEGENDARY
};

str CharmBoxLabels[MAX_CHARM_TYPES][2] = {
	{ "SCHNOR", "SCHSEL" },
	{ "MCHNOR", "MCHSEL" },
	{ "LCHNOR", "LCHSEL" }
};

#endif
