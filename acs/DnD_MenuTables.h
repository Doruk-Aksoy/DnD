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

#define X_MULTIPLIER (4800.0)
#define Y_MULTIPLIER (3200.0)
#define MMASK (-1.0)

#define MOUSE_INPUT_X 1
#define MOUSE_INPUT_Y 2

// topleft corner 1:1 bottom right 0:0
#define HUDMAX_X 480
#define HUDMAX_Y 320
#define HUDMAX_XF 480.0
#define HUDMAX_YF 320.0

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

// MENU IDS
// Moved here because of dependencies
enum {
	RPGMENUCURSORID = 100,
	RPGMENUPOPUPID = 105,
	RPGMENUTRADECOUNTDOWNID,
	RPGMENUPAGEID,
	RPGMENUHIGHLIGHTID,
	RPGMENUID = 700,
	RPGMENULARRID,
	RPGMENURARRID,
	RPGMENURETARRID,
	RPGMENUITEMIDEND,
	RPGMENUITEMID = 1000,
	RPGMENUHELPCORNERID,
	RPGMENUHELPCORNERIDMAIN,
	RPGMENUNAMEID,
	RPGMENUHELPID,
	RPGMENUINFOID,
	RPGMENUDAMAGETYPEID,
	RPGMENULISTID = 1060,
	RPGMENUWEAPONPANELID = 1100,
	RPGMENUBACKGROUNDID = 1101
};

#define DND_MENU_ITEMSAVEBITS1 8
#define DND_MENU_ITEMSAVEBITS1_MASK 0xFF
#define DND_MENU_ITEMCLEARMASK1 0xFFF00FF
#define DND_MENU_ITEMSAVEBITS2 16
#define DND_MENU_ITEMCLEARMASK2 0x000FFFF

#define DND_MENU_INPUTDELAYTICS 4
#define DND_MENUINPUT 0
#define DND_MENUINPUT_DELAY 1
#define DND_MENUINPUT_PAYLOAD 2
#define DND_MENUINPUT_PLAYERCRAFTCLICK 3
#define DND_MENUINPUT_LRPOS 4
int MenuInputData[MAXPLAYERS][5];

enum {
	DND_MENUINPUT_LCLICK = 1,
	DND_MENUINPUT_RCLICK,
	DND_MENUINPUT_PREVBUTTON,
	DND_MENUINPUT_NEXTBUTTON
};

enum {
	BOXLIT_STATE_OFF,
	BOXLIT_STATE_CURSORON,
	BOXLIT_STATE_CLICK
};

#define MAXLITBOXES 3 * MAX_INVENTORY_BOXES
int InventoryBoxLit[MAXLITBOXES];

typedef struct rect {
	int topleft_x;
	int topleft_y;
	int botright_x;
	int botright_y;
} rect_T;

#define MAX_MENU_BOXES 20
typedef struct mp {
	rect_T MenuRectangles[MAX_MENU_BOXES];
	int cursize;
} menu_pane_T;

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
#define MAX_TIMED_BUTTONS LAST_CLICKABLE_BOXID - FIRST_CLICKABLE_BOXID + 1
str ButtonTimers[MAX_TIMED_BUTTONS] = { 
	"DnD_LeftArrowButton_Timer",
	"DnD_ReturnArrowButton_Timer", 
	"DnD_RightArrowButton_Timer" 
};

int ButtonFrameCounts[MAX_TIMED_BUTTONS] = { 6, 6, 6 };

str MainBoxTexts[LAST_CLICKABLE_BOXID + 1] = {
	"",
	"Stat\nScreen",
	"Perk\nScreen",
	"Current\nItems",
	"The\nShop",
	"Research\nScreen",
	"Abilities\nLearned",
	"Extra\nHelp",
	"Previous\nPage",
	"Main\nMenu",
	"Next\nPage"
};

// Box enums end

#define SHOP_MAXWEAPON_PAGES MENU_SHOP_WEAPON8 - MENU_SHOP_WEAPON1 + 1
#define SHOP_MAXAMMO_PAGES SHOP_LASTAMMO_PAGE - SHOP_FIRSTAMMO_PAGE + 1

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
   OBJ_TALENT = 16,
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
	TRADE_TALENT = 64,
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
	TYPE_TALENT,
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
		{ 5250,  1,		1 },
		{ 5500,  1,		1 },
		
		// 3 - 2
		{ 6000,	 1,		1 },
		{ 5750,  1,		1 },
		{ 6500,  1,		1 },
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
		
		// 4 - 2
		{ 6750,  1,		1 },
		{ 5250,	 1,		1 },
		{ 4800,  1,		1 },
		
		// 5 - 1
		{ 3500,  1,		1 },
		{ 4800,  1,		1 },
		{ 6750,  1,		1 },
		{ 5500,  1,		1 },
		{ 5750,  1,		1 },
		{ 5500,  1,		1 },
		
		// 5 - 2
		{ 1750,  1,		1 },
		{ 4000,  1,		1 },
		{ 5000,  1,		1 },
		
		// 6 - 1
		{ 4000,  1,		1 },
		{ 4500,  1,		1 },
		{ 6500,  1,		1 },
		{ 4800,  1,		1 },
        { 5250,  1,		1 },
		{ 6000,  1,		1 },
		
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
		
		// Talent costs
		{ 1500,	 1,			1 },
		{ 1500,	 1,			1 },
		{ 1500,	 1,			1 },
		{ 1500,	 1,			1 },
		{ 1500,	 1,			1 },
		{ 1500,	 1,			1 },
		
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
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		
	// wep slot 3 - 2
		{ -1, -1, -1 },
		{ RES_SLOT3SSGUPG1, -1, -1 },
		{ RES_SLOT3SSGUPG2, -1, -1 },
		{ RES_SLOT3SSGUPG3, -1, -1 },
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
		
	// wep slot 4 - 2
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
		
	// wep slot 5 - 2
		{ RES_SLOT5GL, -1, -1 },
		{ RES_SLOT5LUXURY, -1, -1 },
		{ RES_SLOT5LUXURY, -1, -1 },
		
	// wep slot 6 - 1
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT6UPG1, -1, -1 },
		{ RES_SLOT6UPG2, -1, -1 },
		{ RES_SLOT6UPG3, -1, -1 },
		
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
		{ RES_SLOT3UPG3, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT5LUXURY, -1, -1 },
		
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
		
	// talents
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		
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

#define TALENT_COST_INCREASE 1000
#define TALENT_SCALE_DOUBLER_MARK 25

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

#define SHOPNAME_ITEM 0
#define SHOPNAME_TAG 1
#define SHOPNAME_CONDITION 2
#define SHOPNAME_TYPE 3
// rework these later reducing weapon names from two cols
str ShopItemNames[MAXSHOPITEMS][4] = {
	{ "Upgraded Chainsaw",					"Dual Chainsaw",					WEPCHECK_SLOT1,			"0"  	    },
	{ "Katana",								"Katana",							WEPCHECK_SLOT1,			"0"			},
	{ "Excalibat",							"Excalibat",						WEPCHECK_SLOT1,			"0"		    },
	{ "ResMelee1",							"Inferno Sword",					WEPCHECK_SLOT1,			"0"			},
	{ "Dusk Blade",							"Dusk Blade",						WEPCHECK_SLOT1L,		"1"			},
	{ "Sickle",								"Necro's Sickle",					WEPCHECK_SLOT1L,		"1"		    },
	
	{ " Akimbo Pistols ",					"Akimbo Longslides",				WEPCHECK_SLOT2,			"0"  	    },
	{ "Magnum", 							"Magnum",							WEPCHECK_SLOT2,			"0"  	    },
	{ "Laser Pistol",						"Laser Pistol",						WEPCHECK_SLOT2,			"0"		    },
	{ "ResPistol1",							"Assault Rifle",					WEPCHECK_SLOT2,			"0"		    },
	{ "ResPistol2",							"Viper Staff",						WEPCHECK_SLOT2,			"0"			},
	{ "RubyWand",							"Ruby Wand",						WEPCHECK_SLOT2L,		"1"			},
	{ "ScatterGun",							"Scatter Pistol",					WEPCHECK_SLOT2L,		"1"		    },
	
	{ "Upgraded Shotgun",					"Purifier",							WEPCHECK_SLOT3,			"0"  		},
	{ "Upgraded Shotgun2", 					"Killstorm",						WEPCHECK_SLOT3,			"0" 		},
	{ "Upgraded Shotgun3", 					"Emerald Wand",						WEPCHECK_SLOT3,			"0" 		},
	{ "ResShotgun1",						"Deadlock",							WEPCHECK_SLOT3,			"0"		    },
	{ "ResShotgun2",                        "Nitrogen Crossbow",                WEPCHECK_SLOT3,     	"0"         },
	{ "ResShotgun3",                        "Wheel of Torment",                	WEPCHECK_SLOT3,     	"0"         },
	{ "Upgraded Super Shotgun",			    "Heavy SSG",						WEPCHECK_SLOT3X,		"0" 		},
	{ "Upgraded Super Shotgun2",		    "Erasus",							WEPCHECK_SLOT3X,		"0" 		},
	
	{ "Upgraded Super Shotgun3",		    "Hell's Maw",						WEPCHECK_SLOT3X,		"0" 		},
	{ "ResSSG1",							"Plasma Cannon",					WEPCHECK_SLOT3X,		"0"		    },
	{ "ResSSG2",							"Shocker",							WEPCHECK_SLOT3X,		"0"			},
	{ "ResSSG3",							"Hades Shotgun",					WEPCHECK_SLOT3X,		"0"			},
	{ "Silver Gun",						    "White Death",						WEPCHECK_SLOT3L,		"1"	        },
	{ "Slayer",								"Slayer",							WEPCHECK_SLOT3L,		"1"		    },
		
	{ "Upgraded Machine Gun",				"Heavy Machine Gun",				WEPCHECK_SLOT4,  		"0"     	},
	{ "Upgraded Machine Gun2",			    "Lead Spitter",						WEPCHECK_SLOT4, 		"0"		    },
	{ "Upgraded Machine Gun3",			    "Demon Sealer",						WEPCHECK_SLOT4, 		"0"		    },
	{ "ResMG1",								"Templar MG",						WEPCHECK_SLOT4,			"0"		    },
	{ "ResMG2",                             "Riot Cannon",                      WEPCHECK_SLOT4,      	"0"         },
	{ "ResMG3",								"Acid Rifle",						WEPCHECK_SLOT4,			"0"			},
	{ "ResMG4",								"Fusion Blaster",					WEPCHECK_SLOT4,			"0"			},
	
	{ "Desolator",							"Desolator Cannon",					WEPCHECK_SLOT4L,		"1"			},
	{ " Minigun ",							"Minigun",							WEPCHECK_SLOT4L,   		"1"		    },
	{ "Ebony Cannon",						"Ebony Cannon",						WEPCHECK_SLOT4L,   		"1"		    },
	
	{ "Upgraded Rocket Launcher",		    "Torpedo Launcher",				    WEPCHECK_SLOT5,  		"0"		    },
	{ "Upgraded Rocket Launcher2",		    "Mercury Launcher",				    WEPCHECK_SLOT5,  		"0"		    },
	{ "Upgraded Rocket Launcher3",		    "Vindicator",				    	WEPCHECK_SLOT5,  		"0"		    },
	{ "ResRL1",								"Meteor Launcher",					WEPCHECK_SLOT5,			"0"		    },
	{ "ResRL2",								"Heavy G. Launcher",				WEPCHECK_SLOT5,			"0"		    },
	{ "ResRL3",								"Freezer Cannon",					WEPCHECK_SLOT5,			"0"		    },
	
	{ "Grenade Launcher",					"Grenade Launcher",				    WEPCHECK_SLOT5L,    	"1"		    },
	{ "Upgraded Grenade Launcher",		    "Rotary G. Launcher",				WEPCHECK_SLOT5L,  		"1"		    },
	{ "Heavy Missile Launcher",				"H. Missile Launcher",				WEPCHECK_SLOT5L,   		"1"		    },
	
	{ "Upgraded Plasma Rifle",				"Nuclear P. R.",					WEPCHECK_SLOT6, 		"0"		    },
	{ "Upgraded Plasma Rifle2",				"Turel Cannon",						WEPCHECK_SLOT6,			"0"		    },
	{ "Upgraded Plasma Rifle3",				"Frost Fang",						WEPCHECK_SLOT6,			"0"		    },
	{ "ResPlasma1",							"Flamethrower",						WEPCHECK_SLOT6,			"0"		    },
	{ "ResPlasma2",                         "Lightning Gun",                    WEPCHECK_SLOT6,      	"0"         },
	{ "ResPlasma3",							"Rebounder",						WEPCHECK_SLOT6,			"0"			},
	
	{ "RhinoRifle",							"Rhino AR",							WEPCHECK_SLOT6L,		"1"			},
	{ "Nailgun",							"Nailgun",							WEPCHECK_SLOT6L,		"1"	        },
	{ "Basilisk",							"Basilisk",							WEPCHECK_SLOT6L,		"1"	        },
	
	{ "Upgraded BFG 9000",					"BFG 32768",						WEPCHECK_SLOT7,			"0"		    },
	{ "Devastator",							"Devastator 5000",					WEPCHECK_SLOT7,			"0"     	},
	{ "MFG",								"Destr. Generator",					WEPCHECK_SLOT7,			"0"		    },
	{ "ResBFG1",							"Ion Cannon",						WEPCHECK_SLOT7,			"0"		    },
	{ "ResBFG2",							"Thunder Staff",					WEPCHECK_SLOT7,			"0"			},
	{ "Gauss Rifle",						"Gauss Rifle",						WEPCHECK_SLOT7L,		"1"     	},
	{ "Rail Gun",							"Railgun",							WEPCHECK_SLOT7L,		"1"     	},
	{ "Death Ray",							"Death Ray",						WEPCHECK_SLOT7L,		"1"			},
	
	{ "Death Staff",						"Death Staff",						WEPCHECK_SLOT8L,		"1"		    },
	{ "RazorFang",							"Razorfang",						WEPCHECK_SLOT8L,		"1"    	    },
	{ "Sun Staff",							"Sun Staff",						WEPCHECK_SLOT8L,		"1"     	},
	{ "Soul Reaver",						"Soul Reaver",						WEPCHECK_SLOT8L,		"1"     	},
	
	{ "Clip",								"Bullets",							"",						"0"		    },
	{ "Shell",								"Shells",							"",						"0"		    },
	{ "RocketAmmo",							"Rockets",							"",						"0"		    },
	{ "Cell",								"Cells",							"",						"0"		    },
	{ "ExplodingShell",						"Explosive Shells",					"",						"0"	        },
	{ "EbonyAmmo",							"Cannon Balls",						"",						"0"	        },
	{ "EbonyAmmoX",							"Shrapnel Balls",					"",						"0"	        },
	{ "MISAmmo",							"Heavy Missile",					"",						"0"	        },
	{ "Grenades",							"Grenades",							"",						"0"	        },
	{ "NailgunAmmo",						"Nails",							"",						"0"	        },
	
	{ "BasiliskAmmo",						"Lava Cell",						"",						"0"	        },
	{ "GaussRound",							"Gauss Rounds",						"",						"0"	        },
	{ "SlayerAmmo",							"Slayer Essence",					"",						"0"		    },
	{ "RubyAmmo",							"Ruby Crystal",						"",						"0"			},
	{ "PCanAmmo",                           "Plasma Battery",                   "",                     "0"         },
	{ "RiotgunShell",                       "Riot Cannon Shell",                "",                     "0"         },
	{ "MeteorAmmo",                         "Meteor Sphere",                    "",                     "0"         },
	{ "Fuel",                               "Fuel Tank",                        "",                     "0"         },
	{ "LightningCell",                      "Lightning Cell",                   "",                     "0"         },
	{ "NitrogenCanister",                   "Nitrogen Canister",                "",                     "0"         },
	
	{ "IonAmmo",                            "Ion Cell",                         "",                     "0"         },
	{ "AcidAmmo",							"Acid Ammo",						"",						"0"			},
	{ "EverIce",							"Ever Ice",							"",						"0"			},
	{ "ViperAmmo",							"Viper Mana",						"",						"0"		 	},
	{ "ThunderAmmo",						"Thunder Mana",						"",						"0"			},
	{ "DSealAmmo",							"Dark Seals",						"",						"0"			},
	{ "FusionCell",							"Fusion Cell",						"",						"0"			},
	{ "FlakShell",							"Flak Shell",						"",						"0"			},
	{ "DesolatorAmmo",						"Desolator Rounds",					"",						"0"			},
	{ "HadesAmmo",							"Hades Shells",						"",						"0"			},
	
	{ "DemonBlood",							"Vial of Demon Blood",				"",						"0"			},
	{ "EmeraldMana",						"Emerald Mana",						"",						"0"			},
	{ "HellsMawAmmo",						"Hell Fire",						"",						"0"			},
	{ "DevastatorAmmo",						"Devastator Ammo",					"",						"0"			},
	{ "HeavyGrenades",						"Heavy Grenades",					"",						"0"			},
	
	{ "FlechetteShell",					    "Flechette Shells",					"",						"0"		    },
	{ "PiercingShell",						"Magnum Shells",					"",						"0"		    },
	{ "ElectricShell",						"Shock Shells",						"",						"0"		    },
	{ "NitroShell",                         "Nitrogen Shells",                  "",                     "0"         },
	{ "SlugShell",							"Slug Shells",						"",						"0"			},
	
	{ "A40mmSonicGrenade",				    "Sonic Grenades",					"",						"0"		    },
	{ "A40mmHEGrenade",					    "HE Grenades",						"",						"0"		    },
	
	{ "Ability_Kick",						"Melee Expertise",				    "",						"0"	        },
	{ "Ability_Reloader",					"Fast Reloading",					"",						"0"	        },
	{ "Ability_Dash",						"Mobility",							"",						"0"	        },
	{ "Ability_Arcanery",					"Arcanery",							"",						"0"	        },
	{ "Ability_AntiPoison",					"Toxicology",						"",						"0"	        },
	{ "Ability_ExplosionMastery",			"Demolitions",						"",						"0"	        },
	{ "Ability_HeartSeeker",				"Heart Seeking",			   		"",						"0"	        },
	{ "Ability_Regeneration",				"Regeneration",			    		"",						"0"	        },
	{ "Ability_Temporal",					"Temporal Mastery",				    "",						"0"	        },
	{ "Ability_SoulStealer",				"Soul Stealing",				    "",						"0"	        },
	
	{ "Talent_Ballistic",					"Ballistic Talent",					"",						"0"		    },
	{ "Talent_Melee",						"Melee Talent",						"",						"0"		    },
	{ "Talent_Energy",						"Energy Talent",					"",						"0"		    },
	{ "Talent_Explosive",					"Explosive Talent",					"",						"0"		    },
	{ "Talent_Occult",						"Occult Talent",					"",						"0"		    },
	{ "Talent_Elemental",                   "Elemental Talent",                 "",                     "0"         },
	
	{ "NewGreenArmor",						"Green Armor",						"",						"0"		    },
	{ "YellowArmor",						"Yellow Armor",						"",						"0"		    },
	{ "NewBlueArmor",					    "Blue Armor",						"",						"0"		    },
	{ "TheRedArmor",						"Red Armor",						"",						"0"		    },
	{ "GunSlingerArmor",					"Gunslinger Armor",				    "",						"0"		    },
	{ "OccultArmor",						"Occult Armor",						"",						"0"		    },
	{ "DemoArmor",							"Demo Armor",						"",						"0"		    },
	{ "EnergyArmor",						"Energy Armor",						"",						"0"		    },
	{ "ElementalArmor",                     "Elemental Armor",                  "",                     "0"         },
	
	{ "SuperArmor",                         "Monolith Armor",                   "",                     "0"         },
	{ "CyberneticArmor",					"Cybernetic Armor",				    "",						"0"		    },
	{ "DuelistArmor",						"Duelist Armor",					"",						"0"		    },
	{ "NecroArmor",							"Necro Armor",						"",						"0"		    },
	{ "KnightArmor",						"Knight Armor",						"",						"0"		    },
	{ "RavagerArmor",						"Ravager Armor",					"",						"0"		    },
	
	{ "DnD_PlayerInventoryPages",			"Extra Stash Tab",					"",						"0"			},

	{ "FieldKit",							"Field Kit",						"",						"0"		    },
	{ "SalvationSphere",					"Salvation Sphere",					"",						"0"		    },
	{ "PortableShield",						"Portable Shield",					"",						"0"		    },
	{ "BloodRune",							"Blood Rune",						"",						"0"		    },
	{ "TripleDamage",						"Triple Damage",					"",						"0"    	    },
	{ "BladeVortex",						"Blade Vortex",						"",						"0"		    },
	{ "BookOfTheDead",						"Book of the Dead",				    "",						"0"		    },
	{ "AllmapGiver",						"Auto Map",							"",						"0"		    },
	{ "NewBackPack",						"Back Pack",						"",						"0"		    },
	{ "StatReset",							"Stat Reset",						"",						"0"		    }
};

enum {
	DTYPE_BULLET = 1,
	DTYPE_SHELL = 2,
	DTYPE_MELEE = 4,
	DTYPE_OCCULT = 8,
	DTYPE_EXPLOSIVE = 16,
	DTYPE_ENERGY = 32,
	DTYPE_ELEMENTAL = 64
};

#define MAX_DAMAGE_TYPES 7 // 64
str DamageTypeIcons[MAX_DAMAGE_TYPES] = { 
	"DT_BULL",
	"DT_SHEL",
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
	DTYPE_BULLET,
	DTYPE_BULLET,
	DTYPE_ENERGY,
	DTYPE_BULLET,
	DTYPE_OCCULT,
	DTYPE_ELEMENTAL,
	DTYPE_BULLET,
	
	// 3 - 1
	DTYPE_SHELL,
	DTYPE_SHELL,
	DTYPE_ELEMENTAL,
	DTYPE_SHELL,
	DTYPE_ELEMENTAL,
	DTYPE_OCCULT | DTYPE_MELEE,
	DTYPE_SHELL,
	DTYPE_SHELL,
	
	// 3 - 2
	DTYPE_EXPLOSIVE | DTYPE_OCCULT,
	DTYPE_ENERGY,
	DTYPE_ELEMENTAL,
	DTYPE_SHELL | DTYPE_ELEMENTAL,
	DTYPE_SHELL | DTYPE_EXPLOSIVE,
	DTYPE_OCCULT,
	
	// 4 - 1
	DTYPE_BULLET,
	DTYPE_ENERGY,
	DTYPE_OCCULT,
	DTYPE_BULLET,
	DTYPE_SHELL,
	DTYPE_ELEMENTAL,
	DTYPE_ENERGY,
	
	// 4 - 2
	DTYPE_ELEMENTAL,
	DTYPE_BULLET,
	DTYPE_OCCULT,
	
	// 5 - 1
	DTYPE_EXPLOSIVE,
	DTYPE_EXPLOSIVE,
	DTYPE_SHELL | DTYPE_EXPLOSIVE,
	DTYPE_OCCULT,
	DTYPE_EXPLOSIVE | DTYPE_SHELL,
	DTYPE_ELEMENTAL,
	
	// 5 - 2
	DTYPE_EXPLOSIVE,
	DTYPE_EXPLOSIVE,
	DTYPE_EXPLOSIVE,
	
	// 6 - 1
	DTYPE_ENERGY,
	DTYPE_ENERGY,
	DTYPE_ELEMENTAL,
	DTYPE_ELEMENTAL,
	DTYPE_ELEMENTAL,
	DTYPE_ENERGY,
	
	// 6 - 2
	DTYPE_BULLET,
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
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_SHOTGUN,								SHOP_WEP_HSSG			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL | OBJ_SHOTGUN,				SHOP_WEP_ERASUS			},
	
	// 3 - 2
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL | OBJ_SHOTGUN,				SHOP_WEP_HELLSMAW		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_SHOTGUN,					SHOP_WEP_RESSSG1		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_SHOTGUN,					SHOP_WEP_RESSSG2		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL | OBJ_SHOTGUN,	SHOP_WEP_RESSSG3		},
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
	
	// 4 - 2
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_DESOLATOR		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_MINIGUN		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_EBONY			},
	
	// 5 - 1
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_TORPEDO		},
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_MERC			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL | OBJ_SHOTGUN,				SHOP_WEP_VINDICATOR		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESRL1			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESRL2			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESRL3			},
	
	// 5 - 2
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_GRENADE		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_ROTARYGL		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_HEAVYML		},
	
	// 6 - 1
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_NUCLEARPL		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL,								SHOP_WEP_TUREL			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL,								SHOP_WEP_FROSTFANG		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESPL1			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESPL2			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESPL3			},
	
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

// Supporting 8 such properties. See: RPGMENUBACKGROUNDID for increasing this
enum {
	WPROP_NONE,
	WPROP_CANTHITGHOST = 1,
	WPROP_SELFDMG = 2,
	WPROP_OVERHEAT = 4,
	WPROP_IGNORESHIELD = 8,
	WPROP_ALTAMMO = 16,
	WPROP_RIPPER = 32,
	WPROP_IRREDUCIBLE = 64
};
#define MAX_WEAPON_PROPERTIES 7

str WeaponPropertyImages[MAX_WEAPON_PROPERTIES] = {
	"WNOGHST",
	"WSLFDMG",
	"WOHEAT",
	"WOSHLD",
	"WOALT",
	"WORIP",
	"WOFULL"
};

// holds can hit ghost etc. kind of information
int WeaponProperties[MAXSHOPWEAPONS] = {
	// 1
	WPROP_NONE,
	WPROP_NONE,
	WPROP_IGNORESHIELD,
	WPROP_CANTHITGHOST,
	WPROP_NONE,
	WPROP_IRREDUCIBLE,
	
	// 2
	WPROP_NONE,
	WPROP_IGNORESHIELD,
	WPROP_IGNORESHIELD,
	WPROP_NONE,
	WPROP_IGNORESHIELD,
	WPROP_CANTHITGHOST,
	WPROP_IGNORESHIELD | WPROP_IRREDUCIBLE,
	
	// 3 - 1
	WPROP_ALTAMMO,
	WPROP_NONE,
	WPROP_CANTHITGHOST,
	WPROP_ALTAMMO,
	WPROP_NONE,
	WPROP_NONE,
	WPROP_RIPPER,
	WPROP_NONE,
	
	// 3 - 2
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_IGNORESHIELD,
	WPROP_IGNORESHIELD | WPROP_OVERHEAT,
	WPROP_RIPPER | WPROP_CANTHITGHOST,
	WPROP_SELFDMG | WPROP_IGNORESHIELD,
	WPROP_SELFDMG | WPROP_CANTHITGHOST | WPROP_RIPPER,
	
	// 4 - 1
	WPROP_NONE,
	WPROP_NONE,
	WPROP_NONE,
	WPROP_ALTAMMO,
	WPROP_ALTAMMO,
	WPROP_NONE,
	WPROP_IGNORESHIELD | WPROP_RIPPER,
	
	// 4 - 2
	WPROP_OVERHEAT,
	WPROP_CANTHITGHOST,
	WPROP_IGNORESHIELD,
	
	// 5 - 1
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_IGNORESHIELD | WPROP_SELFDMG,
	WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_RIPPER,
	WPROP_RIPPER | WPROP_OVERHEAT,
	
	// 5 - 2
	WPROP_CANTHITGHOST | WPROP_ALTAMMO | WPROP_SELFDMG,
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	
	// 6 - 1
	WPROP_IGNORESHIELD | WPROP_OVERHEAT | WPROP_SELFDMG,
	WPROP_RIPPER,
	WPROP_RIPPER | WPROP_CANTHITGHOST,
	WPROP_SELFDMG,
	WPROP_NONE,
	WPROP_OVERHEAT,
	
	// 6 - 2
	WPROP_ALTAMMO,
	WPROP_RIPPER | WPROP_CANTHITGHOST,
	WPROP_IGNORESHIELD,
	
	// 7
	WPROP_IGNORESHIELD,
	WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_CANTHITGHOST,
	WPROP_SELFDMG | WPROP_IGNORESHIELD,
	WPROP_OVERHEAT | WPROP_IGNORESHIELD | WPROP_CANTHITGHOST,
	WPROP_SELFDMG | WPROP_IGNORESHIELD,
	WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_IGNORESHIELD,
	WPROP_IGNORESHIELD | WPROP_RIPPER,
	WPROP_IGNORESHIELD | WPROP_OVERHEAT,
	
	// 8
	WPROP_IGNORESHIELD | WPROP_SELFDMG,
	WPROP_RIPPER,
	WPROP_IGNORESHIELD | WPROP_RIPPER,
	WPROP_IGNORESHIELD | WPROP_RIPPER
};
						 
str WeaponExplanation[MAXSHOPWEAPONS] = {
	"Double the blade, double the fun! Twice as much damage, same firing rate. Forces pain.",
	"Unsheathing does 140 damage, normal firing does 84. Altfire allows swinging combos to do massive damage. Normal swings block projectiles. Altfire requires Melee Expertise ability.",
	"Does 80 - 120 damage per swing. Melee does triple damage against undead or magical enemies. Alt fire charges and releases 17 baseballs each doing 100 - 150 on impact and 128 damage in a 128 unit radius. Alt fire requires Melee Expertise ability.",
	"60 - 240 damage per swing with 48 - 192 additional damage in a 96 unit radius. Alt fire shoots 5 flames doing 40 - 80 on hit and 192 - 240 damage in a 160 unit radius. Alt fire requires Melee Expertise ability.",
	"Does 100 - 400 damage per hit depending on charge . Alt fire makes you shoot a wave doing 24 damage in a 104 unit radius, run 15% faster but can't change weapons. Alt fire requires Melee Expertise ability.",
	"Sickle steals life from enemies on hit. Does 40 - 60 damage 3 times. Alt fire swings for irreducable 75 - 90 damage 3 times. Altfire requires Melee Expertise ability.",
	
	"Akimbo longslides. Does 35 damage per shot in a 1.25 by 0.5 spread.",
	"Magnum is a true classic. Each bullet does 150 - 225 damage. Has a capacity of 6. Alt fire reloads.",
	"Laser Pistol is the fresh invention of UAC. Shoots lasers doing 15 - 30 damage in a 2.0 by 1.25 spread. Alt fire charges to do up to a x5 damage rail. Doesn't use ammo.",
	"Assault Rifle does 25 damage per bullet in a 3.6 by 2.4 spread. Magazine capacity of 31. Alt fire zooms, allowing more precise shots.",
	"Summons mobile viper traps, jumping on enemies doing 120-200 damage in 128 unit radius. They expire after 12 seconds. Alt fire shoots slithering vipers doing 150-250 damage. Inflicts \cqpoison.",
	"Casts 3 flames doing 15-30 damage each. Alt fire casts a flame circle at most 384 units away doing 25 initial damage and creating 8 flames doing 32-64 damage on hit and 24 radius damage in 32 units.",
	"Scatter Pistol shoots 3 pellets each doing 10 - 20 damage. Pellets scatter to 6 tiny pellets doing 6 - 12 damage. Alt fire shoots one pellet.",
	
	"Purifier shoots 15 pellets each doing 15 damage in a 3.6 by 3.6 and a shell capacity of 8.",
	"Killstorm is an automatic shotgun, shooting 12 pellets each doing 18 damage in a 7.2 by 5.2 spread. Has a shell capacity of 10.",
	"Creates 5 blasts doing 20 damage each in a 6 by 4.25 spread. Altfire fires a projectile that deals 150 damage, and releases an acid rain, doing 8-32 damage each. These can't hit \cughosts\c-. Victims explode on death doing 100 damage in 96 unit radius. Inflicts \cqpoison.",
	"Deadlock fires 16 pellets doing 15 damage in a 7.0 by 5.2 spread. Has a shell capacity of 12.",
	"Fires shots that do 210 ice damage. Alt fire shoots a blast of nitrogen 384 units ahead, creating 4 series of gas streams doing 5 damage.",
	"An artifact that does 160 damage up to 1024 units, sending a healing bolt. If a \cgdemon\c- was hit, does an explosion in 160 unit radius doing 192 damage. Altfire does 10-20 melee damage. If a \cgdemon\c- is hit, gives 1 ammo.",
	"Heavy Super Shotgun shoots 28 pellets doing 15 damage in a 9.6 by 5.8 spread. Half of these rip through targets.",
	"Erasus shotgun shoots highly ballistic shells with 18 pellets each doing 15 damage. Has to reload after shooting twice. Alt fire shoots both shells in the chamber, or reloads.",
	
	"Shoots a missile and 3 mini missiles. Missile does 45, mini missiles do 15 and explode for 20 in 32 unit radius, not hitting \cughosts\c-. Main missile can scatter after travelling a bit. If it hits an object, explodes for 30 in 64 unit radius. Altfire fires the other side.",
	"Fires 12 plasma balls in a circular fashion, each doing 40 damage. Has a clip size of 5.",
	"Shoots 18 particles each doing 15 damage and forcing pain. Altfire releases heat, dealing 192-240 damage in 108 unit radius.",
	"Fires 15 shells doing 13 damage in a 11.6 and 9.0 spread, releasing embers on hit doing 2 damage, ripping through enemies. Altfire shoots a chunk of embers doing 30 damage on hit. Pressing altfire while on flight splits it into 15 embers doing 18 damage. Embers can't hit \cughosts\c-.",
	"White Death fires 9 pellets each doing 15 on hit. Each pellet also does 32 - 48 explosion damage in a small area. Does self damage.",
	"Slayer creates 6 blades each doing 10 damage and rip through. Alt fire detonates blades at will for 100 damage in a 108 unit radius. Blades return to you after travelling a bit.",
	
	"Finest machine guns UAC has to offer. Bullets do 25 damage in a 1.6 by 0.8 spread. Has a clip size of 60. Can zoom.",
	"Lead Spitter is a super sonic bullet shooter shooting 2 bullets doing 18 damage in a 6.4 by 4.8 spread. Clip size is 75.",
	"Japanese technology brings the finest demon hunting tool. Shoots magic cards that deal 15 damage, reduce monster damage and resistance by 50%. If the monster is below 10% health, culls the monster.",
	"Templar fires silver bullets doing 20 damage in a 4.4 by 2.8 spread. Bullets deal x3 damage to undead and magical enemies. Clip size of 40.",
	"Fires 7 pellets doing 12 damage in a 3.6 by 3.6 spread. Alt fire makes it full auto, but twice as inaccurate. Reload when full to use other ammo.",
	"Fires acid rounds doing 18 damage on hit and 5-15 damage in a 48 unit radius. Alt fire shoots a bolt that sticks to enemies, detonating after 3 seconds for 96 damage and release toxic cloud doing 10-15 damage in 96 unit radius. Inflicts \cqpoison.",
	"Fires in bursts of 5 each doing 28 damage in a 6.8 by 4.2 spread. Altfire shoots a fusion grenade doing 150 damage and releasing rippers doing 5 damage around. Attacks do more damage with more distance. \cfIgnores shields.\c-",
	
	"Desolator fires highly toxic rounds doing 30 damage. Every subsequent hit to a target makes them take 10% more damage from elemental attacks, stacks maximum 5 times. At max stacks, rounds cause a toxic explosion doing 80 damage in 128 unit radius. Inflicts \cqpoison.",
	"Stronger, faster and better than ever! Poor accuracy, shoots tracers that do 16 - 28 damage each. Alt fire to spin.",
	"The ebony cannon shoots bouncing balls of death. 32 - 48 damage with 48 explosion damage in 64 units. Alt fire shoots scatter bombs.",
	
	"The Torpedo Launcher shoots fast torpedos each doing 300 - 500 damage on impact and 224 damage in a 128 unit radius.",
	"Mercury Launcher fires accelerating and heat seeking mercury missiles doing 256 - 320 damage on hit and 192 damage in a 160 unit radius over 2 seconds.",
	"Shoots 10 flak shells in 10.4 by 7.8 doing 5 impact damage and 20 explosion damage in 96 unit radius. The shells explode 320 units ahead to into scatter 3 explosive particles each doing 16 damage in 96 unit radius. Alt fire can zoom to improve accuracy by 50%.",
	"Fires a meteor doing 200 on impact and 192 in a 192 unit radius. The meteor then splits into smaller pieces, and those pieces as well.",
	"Fires grenades doing 128 on impact and 128 in a 128 unit radius. The grenade explodes into shrapnels ripping through doing 6-18 damage. Altfire loads more grenades in the chamber.",
	"Launches a ball of ice that does 150 damage on impact. After some time it'll stop and explode doing 150 damage in 176 unit radius, releasing many ice particles around each doing 3-9 damage, ripping through enemies. They also explode and do 36 damage in 64 unit radius.",
	
	"Useful for when you can't reach around corners. Does 80 damage on impact and 128 damage in a 144 unit radius.",
	"The Rotary Grenade Launcher does 384 damage on impact and 192 damage on a 192 unit radius. Can't hit \cughosts",
	"Top of the food chain for rockets. Shoots two homing rockets each doing 192 damage both on impact and explosion.",
	
	"Improved with a nuclear reactor. Does 36 - 60 on hit and 10 - 30 explosion damage in a 48 unit radius.",
	"Turel Cannon fires highly ionized particles doing 125 damage ripping through everything. Continuous fire is less accurate and does 80 damage. Has a range of 768 units.",
	"Launches 5 ice shards doing 15 - 30 damage in a 7.5 by 7.5 spread. Alt fire launches a glacial orb launching ice shards all around with itself doing 6-9 damage and rips through enemies.",
	"Flamethrower does what it says and throws flames doing 1 - 8 damage. When they hit, they leave a trail of flame doing 5 damage every 2 tics. Fuel size of 75.",
	"UAC offers this shockingly deadly weapon that can shoot lightning doing 9-12 damage. Alt fire shoots forked lightning. Keep firing and damage increases by 4% per stack. Stacks additively.",
	"Fires projectiles doing 25 damage. For every 24 point mark on your overheat, each hit rebounds to 2 other projectiles. Altfire shoots a special projectile doing 600 damage in 256 radius to enemies with \cuHardened Skin\c- or \cfShield\c-.",
	
	"Rhino AR fires heavy rounds dealing 75 damage in a 7.2 by 4.8 spread. Alt fire zooms, allowing for higher precision but loss of rapid fire. Can use \cialternate\c- ammo. Reload when full to use other ammo.",
	"Shoots nails which do 13 - 21 damage and rips through. Alt fire shoots explosive lava nails that \cfignores shields.",
	"Shoots fire balls doing 18 - 48 damage. If loaded, shoots meteors doing 72 - 144 on impact and 96 explosion damage.",
	
	"The newest BFG model 32768 devastates with 600 - 900 damage on impact and 384 damage in a 160 unit radius. Shoots 64 tracers.",
	"Devastator launches five mini rockets each doing 64 to 80 with 32 radius damage in 96 units.",
	"Fires a destructive orb doing 425 damage on impact and 448 damage in a 768 unit radius. Creates 6 smaller explosions doing 200 damage in a 256 unit radius.",
	"Fires ionized energy doing 125 impact and 160 area damage in 160 unit radius, then 80 in 80 and 53 in 60.",
	"Launches a ball of lightning that zaps 5 nearest enemies for 115 damage in 420 units. On impact deals 250-500 and 250 radius damage in 96 units. Altfire zaps all enemies in a large radius.",
	"Gauss Rifle fires a magnetic pulse dealing 200 on hit and 96 radius damage in a 128 unit radius. Alt fire zooms, amplifying the damage for each zoom.",
	"This baby can rip through concrete with ease. Each shot does multiples of 400 damage. Alt fire charges up the next shot, up to 2 times.",
	"Fires energy particles doing 300 damage on impact and 75 area damage in 96 unit radius. Using altfire while particles are midflight causes a laser to be emitted to target area, doing 250 damage both on impact and in a 160 unit radius.",
	
	"Fires meteors of magic bursting on impact. Alt fire fires three columns of fire on the floor and ceiling that travel and explode in flames.",
	"Fires magical bone shards that rip through. Alt fire switches the mode to shoot three demon shredders that seek demons.",
	"Fires sun beams to burn enemies. Alt fire channels the very essence of sun causing a massive meltdown.",
	"Creates portals that lead to hungry ghosts to devour your enemies. Hold to gain a deflective shield."
};

str AttributeExplanation[DND_MAX_ATTRIBUTES] = {
	"\c[INT]Strength\c-\nIncreases \cuMelee Damage\c- by 9%, \cdArmor\c- and \cgHealth\c- Caps by 0.5% and \cqKnockback Resist\c- by 50.",
	"\c[INT]Dexterity\c-\nIncreases \cfNon-Magical Ranged Damage\c- by 3%.",
	"\c[INT]Bulkiness\c-\nIncreases \cdArmor\c- Efficiency by 0.6%, \cdArmor\c- Cap by 4 and \cqKnockback Resist\c- by 25.",
	"\c[INT]Charisma\c-\nReduces \cishop prices\c- by 0.5%, increases \cishop stock\c- by 1% and \ckpet\c- cap by 1 for every 20 points.",
	"\c[INT]Vitality\c-\nIncreases \cgHealth\c- Cap by 4.",
	"\c[INT]Intellect\c-\nIncreases \cmOccult Ranged Damage\c- by 3% and improves spells."
};
							
str ArtifactExplanation[MAXARTIFACTS] = {
	"Portable medikits, can carry 3.",
	"Teleports you to start, heals for 100 health.",
	"Invulnerability on use for 5 seconds. Duration stacks.",
	"Steal life from damage done for 30 seconds.",
	"Triples damage dealt for 15 seconds.",
	"Creates a ring of blades for 20 seconds, granting 50% damage resistance. Blades damage on collision.",
	"Unleashes power of the dead, granting souls for demon kills.",
	"Reveals the current map to you.",
	"Increases ammo capacity.",
	"Resets your stats back to zero. You can reassign."
};

str AccountPurchaseExplanation[MAXACCOUNTITEMS] = {
	"Adds an extra stash tab to your account."
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
		SHOP_AMMO_DESOLATOR,
		SHOP_AMMO_EBONY,
		SHOP_AMMO_EBONYX
	},
	// category 2
	{
		SHOP_AMMO_SHELL,
		SHOP_AMMO_EMERALDMANA,
		SHOP_AMMO_HELLSMAW,
		SHOP_AMMO_PCAN,
		SHOP_AMMO_NITROGEN,
		SHOP_AMMO_DEMONBLOOD,
		SHOP_AMMO_HADES,
		SHOP_AMMO_EXPSHELL,
		SHOP_AMMO_SLAYER,
		-1
	},
	// category 3
	{
		SHOP_AMMO_ROCKET,
		SHOP_AMMO_FLAK,
		SHOP_AMMO_METEOR,
		SHOP_AMMO_HEAVYGRENADE,
		SHOP_AMMO_GL,
		SHOP_AMMO_MIS,
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
		SHOP_AMMO_GAUSS
	},
	// category 5 (not buyable -- souls etc)
	{
		-1
	}
};

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
	
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_DEMONBLOOD		},
	{ OBJ_AMMO,													SHOP_AMMO_EMERALDMANA		},
	{ OBJ_AMMO,													SHOP_AMMO_HELLSMAW			},
	{ OBJ_AMMO,													SHOP_AMMO_DEVASTATOR		},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_HEAVYGRENADE		},
	
	// special ammos
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_FLECHETTE			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_PIERCING			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_ELECTRIC			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_NITROSHELL		},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_SLUGSHELL			},
	
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_SONICGRENADE		},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_HEGRENADE			}
};
								
int AmmoCounts[MAXSHOPAMMOS] = {
	25,
	10,
	5,
	40,
	8,
	8,
	4,
	4,
	5,
	15,
	
	20,
	5,
	6,
	17,
    4,
    8,
    5,
    30,
    25,
    8,
	
    18,
	16,
	25,
	4,
	10,
	25,
	30,
	5,
	20,
	6,
	
	12,
	18,
	10,
	33,
	5,
	
	8,
	8,
	8,
    8,
	8,
	
	5,
	5
};
					
str AmmoExplanation[MAXSHOPAMMOS] = {
	"clips.",
	"shells.",
	"rockets.",
	"cells.",
	"explosive shells for the Silver Gun.",
	"cannon balls for the Ebony Cannon.",
	"shrapnel balls for the Ebony Cannon.",
	"heavy missiles for the Heavy Missile Launcher.",
	"grenades for the Grenade Launcher.",
	"nails for the Nail Gun.",
	
	"lava ammo for the Basilisk.",
	"gauss rounds for the Gauss Rifle.",
	"slayer essences for the Slayer.",
	"ruby crystals for the Ruby Wand.",
	"plasma charge for the Plasma Cannon.",
	"riot shells for the Riot Cannon.",
	"meteors for the Meteor Launcher.",
	"fuel for the Flame Thrower.",
	"lightning cells for the Lightning Gun.",
	"nitrogen canisters for the Nitrogen Crossbow.",
	
	"ion cells for the Ion Cannon.",
	"acid clips for the Acid Rifle.",
	"ever ice for the Frost Fang and Freezer Cannon.",
	"viper essence for the Viper Staff.",
	"thunder mana for the Thunder Staff.",
	"dark seals for the Demon Sealer.",
	"fusion cells for the Fusion Blaster.",
	"flak shells for the Vindicator.",
	"desolator rounds for the Desolator Cannon.",
	"hades shells for the Hades Auto Shotgun.",
	
	"vials of demon blood for the Wheel of Torment.",
	"emerald mana for the Emerald Wand.",
	"hell fire canisters for the Hell's Maw.",
	"rockets for the Devastator.",
	"heavy grenades for Heavy Grenade Launcher.",
	
	"flechette shells.",
	"magnum shells.",
	"electric shells.",
	"nitrogen shells.",
	"slug shells.",
	
	"sonic grenades.",
	"high-ex grenades."
};
									
str ArmorImages[MAXARMORS] = { 
	"ARM1A0", 
	"ARM3A0", 
	"ARM2A0", 
	"QRARA0",
	
	"ARM9A0",
	"AR10A0",
	"AR11A0",
	"AR12A1",
    "AR14A0",
    
    "AR13A0",
	"ARM4A1",
	"ARM5A0",
	"ARM6A0",
	"ARM7A0",
	"ARM8A0"
};

str ArmorExplanation[MAXARMORS] = {
	"33% protection, 100 armor.",
	"41.6% protection, 150 armor.",
	"50% protection, 200 armor.",
	"75% protection, 300 armor.",
	
	"33% protection, 200 armor. Extra 50% hitscan damage, 75% hitscan protection.",
	"33% protection, 200 armor. Extra 50% magic damage, 75% magic protection.",
	"33% protection, 200 armor. Extra 50% explosives damage, 75% explosive protection.",
	"33% protection, 200 armor. Extra 50% energy damage, 75% energy protection.",
    "33% protection, 200 armor. Extra 50% elemental damage, 75% elemental protection.",
    
    "100% protection, 400 armor.",
	"45% protection, 300 armor. If not damaged for 3 seconds, restores armor slowly.",
	"33% protection, 250 armor. Extra 100% hitscan resist.",
	"55% protection, 400 armor. 33% chance on hit to release spikes. Melee talent and bulkiness increase the damage.",
	"35% protection, 400 armor. 50% reduced damage from melee attacks. If a melee weapon is held extra 40% protection is bestowed.",
	"33% protection, 250 armor. On a kill spree, gain x2 extra damage and 25% damage reduction. If a killing spree is met restore armor completely.",
};

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
	{ OBJ_ARMOR | OBJ_RESEARCH, 		SHOP_ARMOR_RAVAGER		 						}
};

// +1 because dash toggle is an exception
str AbilityHelpText[MAXABILITIES + 1] = {
	"Melee weapons gain altfires.",
	"You reload twice as fast.",
	"Gain dodge moves (tap the move keys to dash).",
	"Artifact power increases by 50%.",
	"\c[R5]CYBERNETIC \cj- Gain 50% poison resistance.",
	"\c[R5]CYBERNETIC \cj- Your explosives hurt you 50% less.",
	"Demons drop their hearts on death 10% of the time. Can use as a temporary weapon.",
	"\c[R5]CYBERNETIC \cj- Heal till 20 plus twice your vitality every second. Getting hurt delays the timer.",
	"Temporary weapons give 50% more ammo, and at the beginning of each map gain a fully loaded random temporary weapon.",
	"Demons drop souls when killed with an occult weapon.",
	"Toggles dashing."
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
						
// max 8 slots						
str ShopWeaponTake[MAXNORMALWEPSLOTS] = {
	" Chainsaw ",
	" Pistol ",
	" Shotgun ",
	" Super Shotgun ",
	" Machine Gun ",
	"Rocket Launcher",
	"Plasma Rifle",
	"BFG 9000"
};

typedef struct {
	int res_id;
	int res_number;
	int res_cost;
} res_info_T;

typedef struct {
	str res_icon;
	str res_desc;
} res_info_str_T;

enum {
	RESPAGE_BODY,
	RESPAGE_AMMO,
	RESPAGE_SLOTGUNS,
	RESPAGE_LUXURYGUNS,
	RESPAGE_UTILITY,
};

#define MENU_MAXRES_PERPAGE 24
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
			RES_SLOT3SSGUPG1, 7995, 50
		},
		{
			RES_SLOT3SSGUPG2, 8001, 55
		},
		{
			RES_SLOT3SSGUPG3, 8671, 65
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
			RES_SLOT4UPG4, 	8277, 60
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

res_info_str_T ResearchStringInfo[MENU_MAXRES_PAGES][MENU_MAXRES_PERPAGE] = {
	// body
	{
		{
			"RESBAK29",
			"Nano-technology is finally here! Unlocks certain abilities."
		},
		{
			"RESBAK27",
			"With this groundbreaking research, we can now utilize powers of the demons to empower ourselves! Unlocks certain abilities."
		},
		{
			"RESBAK45",
			"\c[R5]CYBERNETIC \cj- UAC has new bionic enhancements to allow you to withstand more punishment. Increases health capacity by 5%."
		},		
		{
			"RESBAK46",
			"\c[R5]CYBERNETIC \cj- Second enhancement to your body, with improved bionic elements. Increases health capacity by a further 6%."
		},
		{
			"RESBAK47",
			"\c[R5]CYBERNETIC \cj- Top notch bionic material with super-soldier injections to make you even stronger. Increases health capacity by a further 9%."
		},
		{
			"RESBAK48",
			"\c[R5]CYBERNETIC \cj- UAC can now field their soldiers with exoskeletons, making your armors better. Increases armor capacity by 5%."
		},
		{
			"RESBAK49",
			"\c[R5]CYBERNETIC \cj- Exoskeleton material is improved to be made even more durable. Increases armor capacity by a further 6%."
		},
		{
			"RESBAK50",
			"\c[R5]CYBERNETIC \cj- UAC found the ultimate material to create exoskeletons with. Increases armor capacity by a further 9%."
		},
		{
			"RESBAK51",
			"\c[R5]CYBERNETIC \cj- After studying the hide of several Cyberdemons you have sent, we can improve your body to make you take 10% less damage from your explosive attacks."
		},
		{
			"RESBAK52",
			"\c[R5]CYBERNETIC \cj- Studying further, we can now improve the previous enhancement to make you take 25% less damage from explosive attacks instead."
		},
		{
			"RESBAK53",
			"\c[R5]CYBERNETIC \cj- Superior impact protection. Improves previous enhancement to make you take 40% less damage from your explosive attacks instead."
		},
		{
			"", ""
		}
	},
	// special ammo types
	{
		{
			"RESBAK5",
			"Our scientists managed to create a highly ballistic shell that can scatter around and pierce targets, \cfignoring shields\c-."
		},
		{
			"RESBAK6",
			"We can now utilize the same bullets used by magnum handguns on shotguns. Magnum shells penetrate any target."
		},
		{
			"RESBAK7",
			"When you need some extra zap to go with your shell burst, we can use these now. They constantly lock enemies in their pain state and \cfignore shields\c-."
		},
		{
			"RESBAK33",
			"Always nice to greet your adversaries with some ice. Allows Riot Cannon to use Nitrogen Shells."
		},
		{
			"RESBAK54",
			"You can treat your shotgun like a sniper rifle now. Allows Deadlock to use Slug Shells, which can \cfignore shields\c-."
		},
		{
			"RESBAK9",
			"Sometimes it is not raw power you need, but some sustained damage. Unlocks Sonic Grenades."
		},
		{
			"RESBAK8",
			"For when normal grenades are insufficient, try a Heavy Explosive one. Unlocks HE Grenades."
		},
		{
			"RESBAK10",
			"New generation backpacks are super lightweight, marines can carry these to double the capacity of their special ammo types."
		},
		{
			"", ""
		}
	},
	// slot guns
	{
		{
			"RESBAK34",
			"A fine discovery buried in hell. Dusk blade hits real hard and lets its user experience the dusk mode. Unlocks Dusk Blade (1)."
		},
		{
			"RESBAK35",
			"Buried deep in the bowels of the earth, inferno sword is everything a pyromaniac would want! Shoot flames, cut enemies down! Unlocks Inferno Sword (1)."
		},
		{
			"RESBAK18",
			"Some of the zombies were using quite improved versions of your weaponry. Now we can utilize this for your slot 2 weapons. Unlocks Assault Rifle (2)."
		},
		{
			"RESBAK41",
			"An intriguing weapon, capable of summoning mobile snake traps or just shoot them directly! Unlocks Viper Staff (2)."
		},
		{
			"RESBAK19",
			"Through many investments we can now utilize better shotgun mechanics. Unlocks Deadlock (3)."
		},
		{
			"RESBAK31",
			"Sometimes it's better to just cool things down when things get heated up. Unlocks Nitrogen Crossbow (3)."
		},
		{
			"RESBAK55",
			"A strange artifact designed to inflict unimaginable torment on demons. Unlocks Wheel of Torment (3)."
		},
		{
			"RESBAK20",
			"Our scientists were obsessed with creating energy in fixed bursts and now they can! Unlocks Plasma Cannon (3)."
		},
		{
			"RESBAK37",
			"Destroy your enemies with a good style. Put holes in them and burn them after with heat! Unlocks Shocker (3)."
		},
		{
			"RESBAK44",
			"Hades... the god of underworld. Or so they say. His powers are now yours to command! Unlocks Hades Auto Shotgun (3)."
		},
		{
			"RESBAK21",
			"Finally an answer to the undead menace, this silver bullet shooting machine gun will make quick work of undeads and magical creatures alike! Unlocks Templar MG (4)."
		},
		{
			"RESBAK32",
			"A combination of chaingun and shotgun, this fierce weapon is going to make demon paste. Can also use alternate shells. Unlocks Riot Cannon (4)."
		},
		{
			"RESBAK36",
			"We now have perfect material to contain powerful acid to use against demons! A very potent rifle. Unlocks Acid Rifle (4)."
		},
		{
			"RESBAK42",
			"We can produce small scale fusion reactions at will to produce super powered energy blasts. Unlocks Fusion Blaster (4)."
		},
		{
			"RESBAK22",
			"Using energies of demons we can now create meteors at will and so can you! Unlocks Meteor Launcher (5)."
		},
		{
			"RESBAK39",
			"A grenade launcher on steroids! Shoots shrapnel filled grenades! Unlocks Heavy Grenade Launcher (5)."
		},
		{
			"RESBAK40",
			"When you're desperate to get some ice for your drink, there's always a solution! Unlocks Freezer Cannon (5)."
		},
		{
			"RESBAK23",
			"It always occured to us, why don't we have a Grenade Launcher when we have a Rocket Launcher? Yeah, now we have both (5)."
		},
		{
			"RESBAK24",
			"Burning enemies to crisps is never boring! Unlocks Flame Thrower (6)."
		},
		{
			"RESBAK25",
			"For when you want to make a shocking entrance. Unlocks Lightning Gun (6)."
		},
		{
			"RESBAK43",
			"Show your love for bouncing particles! Shoot to your hearts content with Rebounder (6)."
		},
		{
			"RESBAK26",
			"We have miniguns, laser cannons and other various toys but not an Ion Cannon (7)... Now we do!"
		},
		{
			"RESBAK38",
			"An ominous staff, capable of electrocuting large groups of enemies at once. Unlocks Thunder Staff (7)."
		},
		{
			"", ""
		}
	},
	// luxury guns
	{
		{
			"RESBAK11",
			"Investigating the corpses of the various demons killed helped us unlock the secrets of their occult powers, allowing use of occult melee weaponry. Unlocks Excalibat and Necromancer's Scythe (1)."
		},
		{
			"RESBAK12",
			"Some can say pistols are useless but we beg to differ. Some pistols can be quite potent. Unlocks Scatter Pistol and Ruby Wand (2)."
		},
		{
			"RESBAK13",
			"Through scientific research we came up with stronger tiers of slot 3 weaponry. Unlocks Silver Gun and Slayer (3)."
		},
		{
			"RESBAK14",
			"Ever wanted to use more dangerous machineguns? Now you can! Unlocks Desolator Cannon, Minigun and Ebony Cannon (4)."
		},
		{
			"RESBAK15",
			"Our scientists never cease to amaze us! We have new toys to blow things up with! Unlocks Rotary GL and Heavy Missile Launcher (5)."
		},
		{
			"RESBAK16",
			"Destroying things has never been this fun! Demons sure know how to kill things... Unlocks Rhino AR, Nailgun and Basilisk (6)."
		},
		{
			"RESBAK17",
			"If you think you need a bit of sniping, try these! Unlocks Railgun and Gauss Rifle (7)."
		},
		{
			"RESBAK28",
			"Powerful demons can teach us a lot of things. Now we can utilize their immense power for even better weapons! Unlocks slot 8 weapons."
		},
		{
			"", ""
		}
	},
	// utility
	{
		{
			"RESBAK1",
			"By using parts from fallen enemies, we can manufacture armors of varying properties."
		},
		{
			"RESBAK2",
			"With this breakthrough technology, we can start distributing state of the art armors to the marines, blocking 100% of the damage. Unlocks Monolith Armor."
		},
		{
			"RESBAK3",
			"Instead of wasting medikits, by using this new technology we can allow marines to store the medikits picked up permanently. \cgMedic\c- perk increases the cap by 15% each."
		},
		{
			"RESBAK4",
			"Unlocking the occult secrets of demonic energy, we can allow marines to use ancient trophies they come across. Allows use of accessories."
		},
		{
			"RESBAK30",
			"Artifacts have always been an elusive aspect. However with demon technology we can harness even more! Unlocks certain artifacts."
		},
		{
			"RESBAK56",
			"UAC can afford more space to teleport your personal belongings at the expense of more budget spent. Allows purchase of additional stash tabs."
		},
		{
			"", ""
		}
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
	POPUP_OUTOFSTOCK
};

#define MAX_POPUPS (POPUP_OUTOFSTOCK + 1)
str PopupText[MAX_POPUPS] = {
	"",
	"Insufficient funds.",
	"You can't buy this item.",
	"You already own this\nitem!",
	"You don't own this item.",
	"You need to research\nthis item!",
	"Insufficient budget!",
	"You need to discover\nthis research!",
	"You already did this\nresearch!",
	"Unequip some accesso-\nries first!",
	"You need to acquire\ntalent points!",
	"You don't have this\naccessory yet!",
	"Mismatching charm\ntype!",
	"No item selected!",
	"Selected player is\ntrading with someone\nelse!",
	"You can't trade while\nalready in a trade!",
	"Not enough space for\ntrade offerings!",
	"Targeted player has\nmuted you from\ntrading!",
	"Material requires\nanother item to\nbe used on!",
	"Conditions for the\nmaterial not met!",
	"Mismatching item\ntype!",
	"No free spot to\nplace the item!",
	"Can't replace armor\nwith same type! Try\nfill instead.",
	"Out of stock! Wait\nnext map to restock."
};

#define MAX_HELPTEXT_RESEARCH 5
str HelpText_Research[MAX_HELPTEXT_RESEARCH] = {
	"Researches help you unlock parts of the menu or certain abilities of your character. These can be weapons, abilities, artifacts, accessories and more!",
	"All researches require a research document. These documents contain top secret information that was discovered by UAC from the remains of your enemies.",
	"Through the information found in the research documents, you can have researches conducted and discover new features. The features use their own resource called \cv\"Budget\"\c-.",
	"\cvBudget\c- can only be gained by finishing maps. Upon completion of a map, you get \cvbudget\c- depending on the difficulty of the map. The harder the map the more budget you get.",
	"Once you obtain the required amount of \cvbudget\c-, you can go ahead and finish your research. When the research is concluded, you can use the feature the research enables."
};

#define MAX_HELPTEXT_CHARACTER 8
str HelpText_Character[MAX_HELPTEXT_CHARACTER] = {
	"Character loading is a feature only available in servers with \cvSoftcore\c- or \cgHardcore\c- modes enabled. You can do this using \c[Y5]\"sethardcore\"\c- or \c[Y5]\"setsoftcore\"\c- command or setting appropriate flags.",
	"There are numerous flags that require to be set in order for the above modes to work properly. A full list of them can be found on our discord at: https://discord.gg/Gffvznu",
	"You can skip the loading phase using \cf\"skipload\"\c- command in case you are starting a fresh character, or simply don't want to load.",
	"You should in general use the \cf\"setchar X\"\c- command to set your character slot if you are starting fresh.",
	"Avoid slot 1 as it's the default one, it can cause overwrites. Try to prefer slots 2 to 4 as much as possible.",
	"You can have a maximum of 4 characters total from slots 1 to 4. However, your selected character will be wiped out on death should you die in \cgHardcore\c- mode, so caution is advised!",
	"You'll be given a set amount of time before the game starts so that you can load your character safely. There are no monsters during this period.",
	"Loading procedure is as follows:\n1. Open Console\n2.Login using \"login username password\".\n3. Type \cf\"loadme X\"\c- where 'X' is your character slot number.\n3. Wait for others to load."
};

#define MAX_HELPTEXT_DAMAGETYPES MAX_DAMAGE_TYPES
str HelpText_DamageTypes[MAX_HELPTEXT_DAMAGETYPES] = {
	"\c[Y5]Bullet\n\nThe simplest of all types. You'll find this in many semi-auto or fully automatic weapons. Counts as \cuphysical\c- damage.",
	"\c[Y5]Shell\n\nAnother very common type. Mostly found in shotgun type weapons. Counts as \cuphysical\c- damage.",
	"\c[Y5]Melee\n\nFound specifically in close combat weapons. Magical weapons deal \cvmagic\c- damage with their melee, but otherwise counts as \cuphysical\c- damage. Only found in \c[Y5]Slot 1\c-.",
	"\c[Y5]Occult\n\nDemonic or holy weapons utilize this damage type. The most dominant weapons of this type are \c[Y5]Slot 8\c- weapons. Magical and undead monsters take extra damage from this type.",
	"\c[Y5]Explosive\n\nMany weapons that feature explosions use this damage type. Mostly found in \c[Y5]Slot 5\c- weapons.",
	"\c[Y5]Energy\n\nGeneral category of the plasma type weapons. Mostly found in \c[Y5]Slot 6\c- and \c[Y5]7\c- weapons. Mechanical enemies take extra damage from this type.",
	"\c[Y5]Elemental\n\nThis damage type is found in weapons that utilize any of the four elements in their attacks. This type is generally spread out in the weapon slots."
};

str HelpText_WeaponProp[MAX_WEAPON_PROPERTIES] = {
	"\c[Y5]Through Ghost\n\nCan't hit enemies with the \cughost\c- modifier.",
	"\c[Y5]Self Damage\n\nWeapon can inflict \ciself damage\c-.",
	"\c[Y5]Overheat\n\nWeapon will \cgoverheat\c- on continuous use.",
	"\c[Y5]Ignores Shields\n\nWeapon can \cfignore shields\c- or any kind of \cfinvulnerability\c- on monsters.",
	"\c[Y5]Alternate Ammo\n\nWeapon supports usage of \cvalternate ammo\c- through altfire.",
	"\c[Y5]Ripper\n\nWeapon has attacks that rip through enemies unless they have the \"Hardened Skin\" modifier.",
	"\c[Y5]Irreducable\n\nWeapon has attacks that can't be mitigated by any resistance.",
};

#define MAX_HELPTEXT_MAIN 3
str HelpText_Main[MAX_HELPTEXT_MAIN] = {
	"This is Doom's first interactive super cool UAC Menu, ran by none other than \c[Y5]Bobby\c- \cj\"\c-\cmCombine\cqb\cjo\crb\cmnt\c-\" \c[Y5]Sanchez\c-! Through this interface, you're directly connected to the UAC Database! You can make your purchases of various goodies here.",
	"Use your mouse cursor and left click to do your selections. There are various pages with various items in them, so be sure to explore all of it! If you wish to sell a weapon, use altfire key to do so.",
	"I don't have all day, I have yet to finish my important projects such as \cdM\ctS\cfP\crD\c- so don't bother me for long! Buy your shit and leave!"
};

// LegendaryKills
#define MAX_LEGMONS_TEXT 5
str LegendaryMonsterText[MAX_LEGMONS_TEXT] = {
	"\c[Y5]The Dreaming God\nThe evil entity known only as the \"Dreaming God\" wanders realms unknown to many for as long as the universe, or at least that's what we think. Research shows this creature is immune to any magical or elemental attack.",
	"\c[Y5]Torrasque\nThis unwavering creature appears to be magically created by an immensely powerful wizard, long ago. Now it chose this realm to feast upon. We found that the creature is highly resistant to physical, energy and explosive attacks.",
	"\c[Y5]Mordecqai\nOnce the great ruler of the \"Realm of Suffering\", now a wanderer to consume and gain powers of whatever creature comes its way. It seems to be completely immune to fire and explosive attacks.",
	"\c[Y5]God Slayer\nA fearsome mech built by UAC to destroy the demonic menace, gone rogue by the evil powers of Hell. The robot will target only humans and has some interesting AI... Seems to be impervious to bullets. Explosives seem to do little, too.",
	"\c[Y5]Golgoth\nAncient as time, this malignant being seeks only to torture living beings to empower it's domain. Seems to have manifested itself on Earth this time... Research shows it's explosive immune and magic resistant."
};

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
str Help_EliteModExplanation[MAX_MONSTER_MODS] = {
	"Monster takes 25% more damage from energy attacks.",
	"Monster takes 300% more damage from silver bullets.",
	"Monster takes 50% more damage from fire attacks.",
	"Monster takes 50% more damage from ice attacks.",
	"Monster takes 25% more damage from occult attacks.",
	"Monster takes 50% more damage from elemental attacks.",
	
	
	"Monster takes 50% reduced damage from explosives.",
	"Monster takes 50% reduced damage from melee and ballistic attacks.",
	"Monster takes 50% reduced damage from energy attacks.",
	"Monster takes 50% reduced damage from occult attacks.",
	"Monster takes 50% reduced damage from elemental attacks.",
	
	"Monster is immune to area damage.",
	"Monster is immune to explosives.",
	"Monster is immune to any kind of melee or ballistic attack.",
	"Monster is immune to energy attacks.",
	"Monster is immune to occult attacks.",
	"Monster is immune to elemental attacks.",
	
	"Monster is ghostly, most of the non-magical projectiles will go through the monster.",
	"Monster is impervious to ripping attacks.",
	"Monster reflects certain projectiles on hit.",
	"Monster can summon minions.",
	"Monster can curse the player.",
	
	"Monster can heal nearby monsters.",
	"Monster has means to block attacks from players. Weapons that \cfignore shields\c- are effective.",
	"Monster can split into other monsters.",
	"Monster has a chance to raise from the dead.",
	"Monster has means to teleport around.",
	
	"Monster can resurrect other dead monsters.",
	"Monster is capable of covering distances fast.",
	"Monster has homing attacks.",
	"Monster poisons player with certain attacks.",
	"Monster has a death activated ability.",
	
	"Monster can enter rage mode under certain conditions.",
	"Monster has attacks that ignores armor.",
	"Monster is extra aggressive.",
	"Monster is twice as fast.",
	"Monster reacts to whoever hits it immediately.",
	
	"Monster feels no pain.",
	"Monster deals twice as much damage.",
	"Monster has 75% more health.",
	"Monster ignores armor of player with every attack.",
	"Monster belongs to a player and is friendly.",
	
	"Monster is currently blocking attacks. Receives no damage unless attacks \cfignore shields\c-.",
	
	"Monster is summoned by another and gives nothing on kill.",
	"Monster is resurrected and doesn't give anything on kill.",
	"Monster is a being of ice, takes 50% more damage from fire attacks but immune to ice attacks.",
	"Monster is a being of fire, takes 50% more damage from ice attacks but immune to fire attacks.",
	"Monster is a being of stone, has ice weakness, fire resist, immunity to earth and lightning, and Hardened Skin.",
	"Monster is a being of earth, takes 50% more damage from lightning, but immune to earth attacks.",
	
	"You can't leech life from this the monster.",
	"Monster has 50% chance to throw explosive projectiles when hurt.",
	"Monster has 33% chance to steal 15% of current weapon's ammo from you with attacks.",
	"Monster attacks have 33% chance to inflict a random curse.",
	"Monster spawns 5 wraiths on death.",
	"Monster is immune to poison effects.",
	"Monster is immune to chill and freeze.",
	"Monster is immune to ignite.",
	"Monster is immune to overloading.",
	
	"Monster is unique."
};

str CharmBoxLabels[MAX_CHARM_TYPES][2] = {
	{ "SCHNOR", "SCHSEL" },
	{ "MCHNOR", "MCHSEL" },
	{ "LCHNOR", "LCHSEL" }
};

#endif
