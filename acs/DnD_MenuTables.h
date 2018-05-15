#ifndef DND_MENUTABLES_IN
#define DND_MENUTABLES_IN

#include "DnD_MenuConstants.h"
#include "DnD_Defs.h"
#include "DnD_Orbs.h"

#define MAXATTRIBUTES 6

#define PRICE_CHARISMAREDUCE 2
#define SHOP_SCALE_MAX 10

#define CHARISMA_PERCENT 5 // percentage
#define CHARISMA_DIV 10 // for fractional
#define CHARISMA_REDUCE CHARISMA_DIV / CHARISMA_PERCENT
#define CHARISMA_REDUCE_AFTER100 CHARISMA_REDUCE * 2

#define TALENT_CAP 100

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

#define MAX_STACK_ELEMS 16
typedef struct menu_stack {
	int stack_elems[MAX_STACK_ELEMS];
	int stackptr;
	int cursize;
} menu_stack_T;

// Scroll defs

int ScrollPos = 0;

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

enum {
	MENU_MOVE_LEFT = 1,
	MENU_MOVE_RIGHT = 2
};

// Box enums end

#define SHOP_FIRSTAMMO_PAGE MENU_SHOP_AMMO1
#define SHOP_LASTAMMO_PAGE MENU_SHOP_AMMO_SPECIAL1

#define SHOP_MAXWEAPON_PAGES MENU_SHOP_WEAPON8 - MENU_SHOP_WEAPON1 + 1
#define SHOP_MAXAMMO_PAGES SHOP_LASTAMMO_PAGE - SHOP_FIRSTAMMO_PAGE + 1

int WeaponBeginIndexes[SHOP_MAXWEAPON_PAGES] = {
	SHOP_WEAPON1_BEGIN,
	SHOP_WEAPON2_BEGIN,
	SHOP_WEAPON31_BEGIN,
	SHOP_WEAPON32_BEGIN,
	SHOP_WEAPON4_BEGIN,
	SHOP_WEAPON5_BEGIN,
	SHOP_WEAPON6_BEGIN,
	SHOP_WEAPON7_BEGIN,
	SHOP_WEAPON8_BEGIN
};

int AmmoBeginIndexes[SHOP_MAXAMMO_PAGES] = {
	SHOP_FIRSTAMMO_INDEX,
	SHOP_FIRSTAMMO2_INDEX,
	SHOP_FIRSTAMMO3_INDEX,
	SHOP_FIRSTAMMOSPECIAL_INDEX
};

// Holds the players' current maximum page visit indexes
#define MENUMAXPAGES MENU_ABILITY + 1
#define MAX_MENU_BOXPAGES MENU_ABILITY + 1

rect_T BoxList[MENUMAXPAGES][MAX_MENU_BOXES];

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
   OBJ_USESCROLL = 512 // has some text on page that use scrolling
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
	TRADE_ARMOR_REPLACE = 256
};

// Popup Definitions
enum {
	POPUP_ERROR,
	POPUP_SELL,
	POPUP_QBUY_BIND
	//POPUP_FULLAMMO
};
	 
 // MENU IDS
enum {
	RPGMENUCURSORID = 224,
	RPGMENUPAGEID = 226,
	RPGMENUID,
	RPGMENULARRID,
	RPGMENURARRID,
	RPGMENURETARRID,
	RPGMENUITEMIDEND,
	RPGMENUITEMID = 640,
	RPGMENUHELPCORNERID,
	RPGMENUHELPCORNERIDMAIN,
	RPGMENUNAMEID,
	RPGMENUHELPID,
	RPGMENUINFOID,
	RPGMENUDAMAGETYPEID,
	RPGMENULISTID = 690,
	RPGMENUWEAPONPANELID = 719,
	RPGMENUBACKGROUNDID = 720
};
	 
enum {
	TYPE_WEAPON,
	TYPE_AMMO,
	TYPE_ABILITY,
	TYPE_TALENT,
	TYPE_ARTI,
	TYPE_ARMOR
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

#define PERK_LABEL 0
#define PERK_ITEM 1
str PerkLabels[DND_PERKS][2] = {
	{ "Sharpshooting", "Perk_Sharpshooting" },
	{ "Endurance", "Perk_Endurance" },
	{ "Wisdom", "Perk_Wisdom" },
	{ "Greed", "Perk_Greed" },
	{ "Medic", "Perk_Medic" },
	{ "Munitionist", "Perk_Munitionist" },
	{ "Deadliness", "Perk_Deadliness" },
	{ "Savagery", "Perk_Savagery" },
	{ "Luck", "Perk_Luck" }
};

#define SHOPINFO_PRICE 0
#define SHOPINFO_MAX 1
int ShopInfo[MAXSHOPITEMS][2] = 
	{
		// Weapons
		// 1
		{ 3000,	 1 },
		{ 3750,  1 },
		{ 3750,  1 },
		{ 8000,  1 },
		{ 8500,  1 },
		{ 6500,  1 },
		
		// 2
		{ 2000,  1 },
		{ 3500,  1 },
		{ 3000,  1 },
		{ 3000,	 1 },
		{ 4000,  1 },
		{ 3750,  1 },
		{ 3250,  1 },
		
		// 3
		{ 4000,  1 },
		{ 4500,  1 },
		{ 4350,  1 },
        { 4500,  1 },
		{ 5000,  1 },
		{ 5250,  1 },
		{ 5500,  1 },
		{ 5750,  1 },
		
		{ 5250,  1 },
		{ 5250,  1 },
		
		// 4
		{ 4000,  1 },
		{ 4500,  1 },
		{ 6000,  1 },
		{ 5750,  1 },
        { 6500,  1 },
		{ 7000,  1 },
		{ 5250,	 1 },
		{ 4800,  1 },
		
		// 5
		{ 3500,  1 },
		{ 4800,  1 },
		{ 5500,  1 },
		{ 5750,  1 },
		{ 5500,  1 },
		{ 1750,  1 },
		{ 4000,  1 },
		{ 5000,  1 },
		
		// 6
		{ 4000,  1 },
		{ 4500,  1 },
		{ 6500,  1 },
		{ 4800,  1 },
        { 5250,  1 },
		{ 5000,  1 },
		{ 5500,  1 },
		
		// 7
		{ 15000,  1 },
		{ 16000,  1 },
		{ 16000,  1 },
		{ 16000,  1 },
		{ 18000,  1 },
		{ 7500,  1 },
		{ 7500,  1 },
		
		// 8
		{ 25000, 1 },
		{ 25000, 1 },
		{ 25000, 1 },
		{ 25000, 1 },
		
		// Ammunition
		// Basic Ammunition
		{ 20,		1 },
		{ 30,		1 },
		{ 40,		1 },
		{ 50,		1 },
		{ 80,       1 },
		{ 50,       1 },
		{ 100,      1 },
		{ 100,      1 },
		{ 25,       1 },
		{ 65,       1 },
		
		// Ammo Page 2
		{ 100,      1 },
		{ 100,      1 },
		{ 90,       1 },
		{ 80,		1 },
        { 65,       1 },
        { 70,       1 },
        { 105,      1 },
        { 80,       1 },
        { 90,       1 },
        { 70,       1 },
		
		// Ammo Page 3
        { 150,      1 },
		{ 110,		1 },
		{ 100,		1 },
		{ 75,		1 },
		{ 275,		1 },
		{ 65,       1 },
		
		// Special Ammunition
		{ 475,	    1 },
		{ 825,	    1 },
		{ 925,	    1 },
        { 875,      1 },
		{ 925,      1 },
		{ 1250,	    1 },
		
		
		// Abilities - 1
		{ 12000,  1 },
		{ 12000,  1 },
		{ 19000,  1 },
		{ 14500,  1 },
		{ 12000,  1 },
		{ 12000,  1 },
		{ 14000,  1 },
		{ 17000,  1 },
		
		// Abilities - 2
		{ 14000,  1 },
		{ 19500,  1 },
		{ 2000,  1 },
		
		// Talent costs
		{ 1500,	 1 },
		{ 1500,	 1 },
		{ 1500,	 1 },
		{ 1500,	 1 },
		{ 1500,	 1 },
		{ 1500,	 1 },
		
		// Armor costs
		{ 4500,	 1 },
		{ 9000,  1 },
		{ 13500, 1 },
		{ 18000, 1 },
		
		{ 10000, 1 },
		{ 10000, 1 },
		{ 10000, 1 },
		{ 10000, 1 },
        { 10000, 1 },
        
        { 22500, 1 },
		{ 18500, 1 },
		{ 8500,  1 },
		{ 15000, 1 },
		{ 17000, 1 },
		{ 12000, 1 },

		// Artifacts, here for convenience. Index = MAXSHOPITEMS - MAXARTIFACTS
		{ 1250,          FIELDKITMAX  		},
		{ 18000,         SOLVEMAX     		},
		{ 12000,         STASISMAX    		},
		{ 20000,         BLOODRUNEMAX 	    },
		{ 20000,         TRIPLEMAX   		},
		{ 20000,         VORTEXMAX   		},
		{ 17500,	     BOOKMAX      		},
		{ 27500,         1			   		},
		{ 50000,         1					},
		{ 10000,         RESETMAX 			}
};

#define MAX_RESEARCH_REQUIREMENTS 3
// Basis for multi-research requiring stuff is here
int ItemResearchRequirements[MAXSHOPITEMS][MAX_RESEARCH_REQUIREMENTS] = {
	
	// wep slot 1
		{ -1, -1, -1 },
		{ RES_SLOT1OCCULT, -1, -1 },
		{ -1, -1, -1 },
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
		
	// wep slot 3
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT3UPG1, -1, -1 },
		{ RES_SLOT3UPG2, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT3SSGUPG1, -1, -1 },
		{ RES_SLOT3SSGUPG2, -1, -1 },
		
	// wep slot 3 - 2
		{ RES_SLOT3LUXURY, -1, -1 },
		{ RES_SLOT3LUXURY, -1, -1 },
		
	// wep slot 4
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT4UPG1, -1, -1 },
		{ RES_SLOT4UPG2, -1, -1 },
		{ RES_SLOT4UPG3, -1, -1 },
		{ RES_SLOT4LUXURY, -1, -1 },
		{ RES_SLOT4LUXURY, -1, -1 },
		
	// wep slot 5
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT5UPG1, -1, -1 },
		{ RES_SLOT5UPG2, -1, -1 },
		{ RES_SLOT5UPG3, -1, -1 },
		{ RES_SLOT5GL, -1, -1 },
		{ RES_SLOT5LUXURY, -1, -1 },
		{ RES_SLOT5LUXURY, -1, -1 },
		
	// wep slot 6
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ RES_SLOT6UPG1, -1, -1 },
		{ RES_SLOT6UPG2, -1, -1 },
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
		{ RES_SLOT3LUXURY, -1, -1 },
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
		
	// ammo special
		{ RES_FLECHETTE, -1, -1 },
		{ RES_PIERCING, -1, -1 },
		{ RES_ELECTRIC, -1, -1 },
		{ RES_NITRO, -1, -1 },
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
		{ -1, -1, -1 },
		
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
	{ "Excalibat",							"Excalibat",						WEPCHECK_SLOT1,			"0"		    },
	{ "Katana",								"Katana",							WEPCHECK_SLOT1,			"0"			},
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
	{ "ResShotgun1",						"Deadlock",							WEPCHECK_SLOT3,			"0"		    },
	{ "ResShotgun2",                        "Nitrogen Crossbow",                WEPCHECK_SLOT3,     	"0"         },
	{ "Upgraded Super Shotgun",			    "Heavy SSG",						WEPCHECK_SLOT3X,		"0" 		},
	{ "Upgraded Super Shotgun2",		    "Erasus",							WEPCHECK_SLOT3X,		"0" 		},
	{ "ResSSG1",							"Plasma Cannon",					WEPCHECK_SLOT3X,		"0"		    },
	{ "ResSSG2",							"Shocker",							WEPCHECK_SLOT3X,		"0"			},
	
	{ "Silver Gun",						    "Silver Gun",						WEPCHECK_SLOT3L,		"1"	        },
	{ "Slayer",								"Slayer",							WEPCHECK_SLOT3L,		"1"		    },
		
	{ "Upgraded Machine Gun",				"Heavy Machine Gun",				WEPCHECK_SLOT4,  		"0"     	},
	{ "Upgraded Machine Gun2",			    "Lead Spitter",						WEPCHECK_SLOT4, 		"0"		    },
	{ "Upgraded Machine Gun3",			    "Demon Sealer",						WEPCHECK_SLOT4, 		"0"		    },
	{ "ResMG1",								"Templar MG",						WEPCHECK_SLOT4,			"0"		    },
	{ "ResMG2",                             "Riot Cannon",                      WEPCHECK_SLOT4,      	"0"         },
	{ "ResMG3",								"Acid Rifle",						WEPCHECK_SLOT4,			"0"			},
	{ " Minigun ",							"Minigun",							WEPCHECK_SLOT4L,   		"1"		    },
	{ "Ebony Cannon",						"Ebony Cannon",						WEPCHECK_SLOT4L,   		"1"		    },
	
	{ "Upgraded Rocket Launcher",		    "Torpedo Launcher",				    WEPCHECK_SLOT5,  		"0"		    },
	{ "Upgraded Rocket Launcher2",		    "Mercury Launcher",				    WEPCHECK_SLOT5,  		"0"		    },
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
	{ "Nailgun",							"Nailgun",							WEPCHECK_SLOT6L,		"1"	        },
	{ "Basilisk",							"Basilisk",							WEPCHECK_SLOT6L,		"1"	        },
	
	{ "Upgraded BFG 9000",					"BFG 32768",						WEPCHECK_SLOT7,			"0"		    },
	{ "Devastator",							"Devastator 5000",					WEPCHECK_SLOT7,			"0"     	},
	{ "MFG",								"Destr. Generator",					WEPCHECK_SLOT7,			"0"		    },
	{ "ResBFG1",							"Ion Cannon",						WEPCHECK_SLOT7,			"0"		    },
	{ "ResBFG2",							"Thunder Staff",					WEPCHECK_SLOT7,			"0"			},
	{ "Gauss Rifle",						"Gauss Rifle",						WEPCHECK_SLOT7L,		"1"     	},
	{ "Rail Gun",							"Railgun",							WEPCHECK_SLOT7L,		"1"     	},
	
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
	
	{ "FlechetteShell",					    "Flechette Shells",					"",						"0"		    },
	{ "PiercingShell",						"Magnum Shells",					"",						"0"		    },
	{ "ElectricShell",						"Shock Shells",						"",						"0"		    },
	{ "NitroShell",                         "Nitrogen Shells",                  "",                     "0"         },
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
	{ "Ability_MonsterInfo",				"Monster Scan",						"",						"0"		    },
	
	{ "Talent_Ballistic",					"Ballistic Talent",					"",						"0"		    },
	{ "Talent_Melee",						"Melee Talent",						"",						"0"		    },
	{ "Talent_Occult",						"Occult Talent",					"",						"0"		    },
	{ "Talent_Explosive",					"Explosive Talent",					"",						"0"		    },
	{ "Talent_Energy",						"Energy Talent",					"",						"0"		    },
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
	DTYPE_MELEE,
	DTYPE_MELEE | DTYPE_OCCULT,
	DTYPE_MELEE,
	DTYPE_MELEE | DTYPE_ELEMENTAL,
	DTYPE_MELEE | DTYPE_OCCULT,
	DTYPE_MELEE | DTYPE_OCCULT,
	
	DTYPE_BULLET,
	DTYPE_BULLET,
	DTYPE_ENERGY,
	DTYPE_BULLET,
	DTYPE_OCCULT,
	DTYPE_ELEMENTAL,
	DTYPE_BULLET,
	
	DTYPE_SHELL,
	DTYPE_SHELL,
	DTYPE_SHELL,
	DTYPE_ELEMENTAL,
	DTYPE_SHELL,
	DTYPE_SHELL,
	DTYPE_ENERGY,
	DTYPE_ELEMENTAL,
	
	DTYPE_SHELL | DTYPE_EXPLOSIVE,
	DTYPE_OCCULT,
	
	DTYPE_BULLET,
	DTYPE_ENERGY,
	DTYPE_OCCULT,
	DTYPE_BULLET,
	DTYPE_SHELL,
	DTYPE_ELEMENTAL,
	DTYPE_BULLET,
	DTYPE_OCCULT,
	
	DTYPE_EXPLOSIVE,
	DTYPE_EXPLOSIVE,
	DTYPE_OCCULT,
	DTYPE_EXPLOSIVE | DTYPE_SHELL,
	DTYPE_ELEMENTAL,
	DTYPE_EXPLOSIVE,
	DTYPE_EXPLOSIVE,
	DTYPE_EXPLOSIVE,
	
	DTYPE_ENERGY,
	DTYPE_ENERGY,
	DTYPE_ELEMENTAL,
	DTYPE_ELEMENTAL,
	DTYPE_ELEMENTAL,
	DTYPE_OCCULT,
	DTYPE_OCCULT,
	
	DTYPE_ENERGY,
	DTYPE_EXPLOSIVE,
	DTYPE_ENERGY,
	DTYPE_ENERGY,
	DTYPE_ELEMENTAL,
	DTYPE_ENERGY | DTYPE_EXPLOSIVE,
	DTYPE_ENERGY,
	
	DTYPE_OCCULT,
	DTYPE_OCCULT,
	DTYPE_OCCULT,
	DTYPE_OCCULT
};

struct draw_info WeaponDrawInfo[MAXSHOPWEAPONS] = {
	{ OBJ_WEP | OBJ_HASCHOICE, 												-1 						},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_EXCALIBAT		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL,								-1						},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_INFERNOSWORD	},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_DUSKBLADE		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_SICKLE			},
	
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_AKIMBOPISTOL	},
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_MAGNUM			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL,								SHOP_WEP_LASERPISTOL	},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESPIS1		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESPIS2		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RUBYWAND		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_SCATTERPISTOL	},
	
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_PURIFIER		},
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_AUTOSG			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESSG1			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESSG2			},
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_HSSG			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL,								SHOP_WEP_ERASUS			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESSSG1		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESSSG2		},
	
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_SILVER			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_SLAYER			},
	
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_HMG			},
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_LEAD			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL,								SHOP_WEP_DSEAL			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESMG1			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESMG2			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESMG3			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_MINIGUN		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_EBONY			},
	
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_TORPEDO		},
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_MERC			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESRL1			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESRL2			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESRL3			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_GRENADE		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_ROTARYGL		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_HEAVYML		},
	
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_NUCLEARPL		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL,								SHOP_WEP_TUREL			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL,								SHOP_WEP_FROSTFANG		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESPL1			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESPL2			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_NAIL			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_BASILISK		},
	
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_BFG			},
	{ OBJ_WEP | OBJ_HASCHOICE,												SHOP_WEP_DEVA			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_USESCROLL,								SHOP_WEP_MFG			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RESBFG1		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_RESBFG2		},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH | OBJ_USESCROLL,				SHOP_WEP_GAUSS			},
	{ OBJ_WEP | OBJ_HASCHOICE | OBJ_RESEARCH,								SHOP_WEP_RAIL			},
	
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
	WPROP_NONE,
	WPROP_IGNORESHIELD,
	WPROP_NONE,
	WPROP_CANTHITGHOST,
	WPROP_NONE,
	WPROP_IRREDUCIBLE,
	
	WPROP_NONE,
	WPROP_IGNORESHIELD,
	WPROP_IGNORESHIELD,
	WPROP_NONE,
	WPROP_IGNORESHIELD,
	WPROP_CANTHITGHOST,
	WPROP_IGNORESHIELD | WPROP_IRREDUCIBLE,
	
	WPROP_ALTAMMO,
	WPROP_NONE,
	WPROP_ALTAMMO,
	WPROP_NONE,
	WPROP_RIPPER,
	WPROP_NONE,
	WPROP_NONE,
	WPROP_IGNORESHIELD,
	
	WPROP_SELFDMG | WPROP_IGNORESHIELD,
	WPROP_SELFDMG | WPROP_CANTHITGHOST,
	
	WPROP_NONE,
	WPROP_IGNORESHIELD,
	WPROP_NONE,
	WPROP_ALTAMMO,
	WPROP_ALTAMMO,
	WPROP_NONE,
	WPROP_CANTHITGHOST,
	WPROP_IGNORESHIELD,
	
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_IGNORESHIELD | WPROP_SELFDMG,
	WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_RIPPER,
	WPROP_RIPPER | WPROP_OVERHEAT,
	WPROP_CANTHITGHOST | WPROP_ALTAMMO | WPROP_SELFDMG,
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	WPROP_CANTHITGHOST | WPROP_SELFDMG,
	
	WPROP_IGNORESHIELD | WPROP_OVERHEAT | WPROP_SELFDMG,
	WPROP_RIPPER,
	WPROP_RIPPER | WPROP_CANTHITGHOST,
	WPROP_SELFDMG,
	WPROP_NONE,
	WPROP_RIPPER | WPROP_CANTHITGHOST,
	WPROP_IGNORESHIELD,
	
	WPROP_IGNORESHIELD,
	WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_CANTHITGHOST,
	WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_IRREDUCIBLE,
	WPROP_OVERHEAT | WPROP_IGNORESHIELD | WPROP_CANTHITGHOST,
	WPROP_SELFDMG | WPROP_IGNORESHIELD,
	WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_IGNORESHIELD,
	WPROP_IGNORESHIELD | WPROP_RIPPER,
	
	WPROP_IGNORESHIELD | WPROP_SELFDMG,
	WPROP_RIPPER,
	WPROP_IGNORESHIELD | WPROP_RIPPER,
	WPROP_IGNORESHIELD | WPROP_RIPPER
};
						 
str WeaponExplanation[MAXSHOPWEAPONS] = {
	"Double the blade, double the fun! Twice as much damage, same firing rate. Forces pain.",
	"Does 80 - 120 damage per swing. Melee does triple damage against undead or magical enemies. Alt fire charges and releases 17 baseballs each doing 100 - 150 on impact and 128 damage in a 128 unit radius. Alt fire requires Melee Expertise ability.",
	"Unsheathing does 140 damage, normal firing does 84. Altfire allows swinging combos to do massive damage. Normal swings block projectiles. Altfire requires Melee Expertise ability.",
	"60 - 240 damage per swing with 48 - 192 additional damage in a 96 unit radius. Alt fire shoots 5 flames doing 40 - 80 on hit and 192 - 240 damage in a 160 unit radius. Alt fire requires Melee Expertise ability.",
	"Does 100 - 400 damage per hit depending on charge . Alt fire makes you shoot a wave doing 24 damage in a 104 unit radius, run 15% faster but can't change weapons. Alt fire requires Melee Expertise ability.",
	"Sickle steals life from enemies on hit. Does 40 - 60 damage 3 times. Alt fire swings for irreducable 75 - 90 damage 3 times. Altfire requires Melee Expertise ability.",
	
	"Akimbo longslides. Does 35 damage per shot in a 1.25 by 0.5 spread.",
	"Magnum is a true classic. Each bullet does 150 - 225 damage. Has a capacity of 6. Alt fire reloads.",
	"Laser Pistol is the fresh invention of UAC. Shoots lasers doing 15 - 30 damage in a 2.0 by 1.25 spread. Alt fire charges to do up to a x5 damage rail. Doesn't use ammo.",
	"Assault Rifle does 25 damage per bullet in a 3.6 by 2.4 spread. Magazine capacity of 31. Alt fire zooms, allowing more precise shots.",
	"Summons mobile viper traps, jumping on enemies doing 120-200 damage in 128 unit radius. They expire after 12 seconds. Alt fire shoots slithering vipers doing 150-250 damage.",
	"Casts 3 flames doing 10-40 damage each. Alt fire casts a flame circle at most 384 units away doing 25 initial damage and creating 8 flames doing 32-64 damage on hit and 24 radius damage in 24 units.",
	"Scatter Pistol shoots 3 pellets each doing 10 - 20 damage. Pellets scatter to 6 tiny pellets doing 6 - 12 damage. Alt fire shoots one pellet.",
	
	"Purifier shoots 15 pellets each doing 15 damage in a 3.6 by 3.6 and a shell capacity of 8.",
	"Killstorm is an automatic shotgun, shooting 12 pellets each doing 18 damage in a 7.2 by 5.2 spread. Has a shell capacity of 10.",
	"Deadlock fires 16 pellets doing 15 damage in a 7.0 by 5.2 spread. Has a shell capacity of 12.",
	"Fires shots that do 210 ice damage. Alt fire shoots a blast of nitrogen 384 units ahead, creating 4 series of gas streams doing 5 damage.",
	"Heavy Super Shotgun shoots 28 pellets doing 15 damage in a 9.6 by 5.8 spread. Half of these rip through targets.",
	"Erasus shotgun shoots highly ballistic shells with 18 pellets each doing 15 damage. Has to reload after shooting twice. Alt fire shoots both shells in the chamber, or reloads.",
	"Fires 24 plasma balls in a circular fashion, each doing 20 damage. Has a clip size of 5.",
	"Shoots 18 shells each doing 15 damage and forcing pain. Overheats when used. Altfire releases a portion of it, dealing 108-180 damage in 96 unit radius.",
	
	"Silver Gun fires 9 pellets each doing 15 on hit. Each pellet also does 32 - 48 explosion damage in a small area. Does self damage.",
	"Slayer creates 6 blades each doing 10 damage and rip through. Alt fire detonates blades at will for 100 damage in a 108 unit radius. Blades return to you after travelling a bit.",
	
	"Finest machine guns UAC has to offer. Bullets do 25 damage in a 1.6 by 0.8 spread. Has a clip size of 60. Can zoom.",
	"Lead Spitter is a super sonic bullet shooter shooting 2 bullets doing 18 damage in a 6.4 by 4.8 spread. Clip size is 75.",
	"Japanese technology brings the finest demon hunting tool. Shoots magic cards that deal 15 damage, reduce monster damage and resistance by 50%. If the monster is below 10% health, culls the monster.",
	"Templar fires silver bullets doing 20 damage in a 4.4 by 2.8 spread. Bullets deal x3 damage to undead and magical enemies. Clip size of 40.",
	"Fires 7 pellets doing 12 damage in a 3.6 by 3.6 spread. Alt fire makes it full auto, but twice as inaccurate. Reload when full to use other ammo.",
	"Fires bullets doing 15 damage on hit and 5-15 damage in a 40 unit radius. Alt fire shoots a bolt that sticks to enemies, detonating after 3 seconds for 64 damage and release toxic cloud doing 5-15 damage in 96 unit radius.",
	"Stronger, faster and better than ever! Poor accuracy, shoots tracers that do 16 - 28 damage each. Alt fire to spin.",
	"The ebony cannon shoots bouncing balls of death. 16 - 40 damage with 48 explosion damage in 64 units. Alt fire shoots scatter bombs.",
	
	"The Torpedo Launcher shoots fast torpedos each doing 300 - 500 damage on impact and 224 damage in a 128 unit radius.",
	"Mercury Launcher fires accelerating and heat seeking mercury missiles doing 256 - 320 damage on hit and 192 damage in a 160 unit radius over 2 seconds.",
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
	"Shoots nails which do 13 - 21 damage and rips through. Alt fire shoots explosive lava nails that \cfignores shields.",
	"Shoots fire balls doing 18 - 48 damage. If loaded, shoots meteors doing 72 - 144 on impact and 96 explosion damage.",
	
	"The newest BFG model 32768 devastates with 600 - 900 damage on impact and 384 damage in a 160 unit radius. Shoots 64 tracers.",
	"Devastator launches four mini rockets each doing 32 to 80 with 32 radius damage in 72 units.",
	"Fires a destructive orb doing 425 damage on impact and 384 damage in a 768 unit radius. Creates 6 smaller explosions doing 200 damage in a 256 unit radius. Does self damage.",
	"Fires ionized energy doing 125 impact and 128 area damage in 160 unit radius.",
	"Launches a ball of lightning that zaps 5 nearest enemies for 115 damage in 420 units. On impact deals 250-500 and 250 radius damage in 96 units. Altfire zaps all enemies in a large radius.",
	"Gauss Rifle fires a magnetic pulse dealing 100 on hit and 192 radius damage in a 96 unit radius. Alt fire zooms, amplifying the damage for each zoom.",
	"This baby can rip through concrete with ease. Each shot does multiples of 92 damage. Alt fire charges up the next shot, up to 2 times.",
	
	"Fires meteors of magic bursting on impact. Alt fire fires three columns of fire on the floor and ceiling that travel and explode in flames.",
	"Fires magical bone shards that rip through. Alt fire switches the mode to shoot three demon shredders that seek demons.",
	"Fires sun beams to burn enemies. Alt fire channels the very essence of sun causing a massive meltdown.",
	"Creates portals that lead to hungry ghosts to devour your enemies. Hold to gain a deflective shield."
};

str AttributeExplanation[MAXATTRIBUTES] = {
	"Increases \cuMelee Damage\c- by 9%, \cdArmor\c- and \cgHealth\c- Caps by 0.5% and \qKnockback Resist\c- by 50.",
	"Increases \cfNon-Magical Talent Bonuses\c- by 0.1%.",
	"Increases \cdArmor\c- Efficiency by 0.6%, \cdArmor\c- Cap by 4 and \qKnockback Resist\c- by 25.",
	"Reduces \cishop prices\c- by 0.5%.",
	"Increases \cgHealth\c- Cap by 4.",
	"Increases \cmOccult Talent Bonus\c- by 0.1%."
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
	 
str AccessoryNames[MAX_ACCESSORY] = { 
	"Wisdom Talisman", 
	"Greed Talisman", 
	"Demon Bane", 
	"Amulet of Hellfire", 
	"Angelic Ankh", 
	"Reflection Pendant", 
	"Nether Mask", 
	"Taltos Ring", 
	"Hate Shard",
	"Hand of Artemis",
	"Scroll of Agamotto",
	"Gryphon Boots",
	"Lich Arm",
	"Sigil of Elements",
	"Celestial Relic",
	"Veil of the Assassin",
	"Hunter's Talisman"
};

str AccessoryList[MAX_ACCESSORY] = {
	"Accessory_1",
	"Accessory_2",
	"Accessory_3",
	"Accessory_4",
	"Accessory_5",
	"Accessory_6",
	"Accessory_7",
	"Accessory_8",
	"Accessory_9",
	"Accessory_10",
	"Accessory_11",
	"Accessory_12",
	"Accessory_13",
	"Accessory_14",
	"Accessory_15",
	"Accessory_16",
	"Accessory_17"
};

// make this automated later
str AccessoryImages[MAX_ACCESSORY] = { 
	"ACC1B0", 
	"ACC2B0", 
	"ACC3B0", 
	"ACC4B0", 
	"ACC5B0", 
	"ACC6B0", 
	"ACC7B0", 
	"ACC8B0", 
	"ACC9B0",
	"AC10B0",
	"AC11B0",
	"AC12B0",
	"AC13B0",
	"AC14B0",
	"AC15B0",
	"AC16B0",
	"AC17B0"
};

str AccessoryBenefits[MAX_ACCESSORY] = {
	"Increases exp gain by 50%.",
	"Increases credit gain by 50%.",
	"Increases damage of occult weapons by 100%.",
	"Protects from all fire damage and hazards by 75%.",
	"When about to die, the ankh will intervene, making you invulnerable for 8 seconds. Works once very map.",
	"When hit, 20% chance to go invulnerable and reflective for 5 seconds. Can happen only 5 times every map.",
	"All your weapons can hit ghosts and do irreducable damage.",
	"Recover health from melee attacks and become a ghost.",
	"Reflect damage back to enemies. Attackers take x2 more damage for 5 seconds.",
	"Your weapons do not require any ammo. Does not affect \cstemporary\c- weapons.",
	"Do 150% more damage while standing.",
	"Gives 25% movement speed, immunity to curses and fall damage, and knockback immunity.",
	"Soul capacity becomes 150 and ultimate weapons do x2.5 damage.",
	"Gives x2 elemental damage to a single element for 15 seconds. This works in a rotation.",
	"Gives 25% increased health and armor capacity.",
	"Your critical hits mark nearby monsters once every 20 seconds, making them deal 33% less damage they replenish 3% of your max health on kill.",
	"On a critical hit, your next crit's damage is doubled. Expending this crit heals you for 10% of missing health. Lasts for 5 seconds or until you expend the crit."
};

str AccessoryNegatives[MAX_ACCESSORY] = {
	"Reduces credit gain by 50%.",
	"Reduces experience gain by 50%.",
	"Reduces other damages by 75%.",
	"Increases ice damage taken by 75%.",
	"Your health cap is halved. (Never below 100)",
	"While active, move 25% slower.",
	"Take 50% more damage.",
	"Health items are unpickable. Only Megasphere or Berserk can be picked that give health.",
	"Non-melee damage reduced by 50%.",
	"Do 50% less damage.",
	"Take 30% more damage while moving.",
	"Can't regenerate.",
	"Any other weapon does 90% reduced damage.",
	"Other elements do 50% less damage.",
	"Slows you down by 20%.",
	"Your overall crit damage is halved.",
	"On expending your doubled crit, never crit for 10 seconds."
};

struct draw_info AmmoDrawInfo[MAXSHOPAMMOS] = {
	{ OBJ_AMMO,													SHOP_AMMO_CLIP 				},
	{ OBJ_AMMO,													SHOP_AMMO_SHELL				},
	{ OBJ_AMMO,													SHOP_AMMO_ROCKET			},
	{ OBJ_AMMO,													SHOP_AMMO_CELL				},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_EXPSHELL			},
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
	
	// special ammos
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_FLECHETTE			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_PIERCING			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_ELECTRIC			},
	{ OBJ_AMMO | OBJ_RESEARCH,									SHOP_AMMO_NITROSHELL		},
	
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
	
	"flechette shells.",
	"magnum shells.",
	"electric shells.",
	"nitrogen shells.",
	
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
	"33% protection, 150 armor. Extra 100% hitscan resist.",
	"60% protection, 400 armor. 33% chance on hit to release spikes. Melee talent and bulkiness increase the damage.",
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
	"Gain poison resistance.",
	"Your explosives hurt you 50% less.",
	"Demons drop their hearts on death 10% of the time. Can use as a temporary weapon.",
	"Heal till 20 plus twice your vitality every second. Getting hurt delays the timer.",
	"Temporary weapons give 50% more ammo, and at the beginning of each map gain a fully loaded random temporary weapon.",
	"Demons give souls on kill.",
	"Can see monster health, mods and level.",
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
	{ OBJ_ABILITY | OBJ_RESEARCH, 				SHOP_ABILITY_SOUL		 						},
	{ OBJ_ABILITY, 								SHOP_ABILITY_MONSTERINFO	 					}
};
									 
str ShopWeaponTake[MAXSHOPWEAPONS] = {
	" Chainsaw ",
	" Chainsaw ",
	" Chainsaw ",
	" Chainsaw ",
	" ",
	" ",
	
	" Pistol ",
	" Pistol ",
	" Pistol ",
	" Pistol ",
	" Pistol ",
	" ",
	" ",
	
	" Shotgun ",
	" Shotgun ",
	" Shotgun ",
	" Shotgun ",
	
	" Super Shotgun ",
	" Super Shotgun ",
	" Super Shotgun ",
	" Super Shotgun ",
	
	" ",
	" ",
	
	" Machine Gun ",
	" Machine Gun ",
	" Machine Gun ",
	" Machine Gun ",
	" Machine Gun ",
	" Machine Gun ",
	" ",
	" ",
	
	"Rocket Launcher",
	"Rocket Launcher",
	"Rocket Launcher",
	"Rocket Launcher",
	"Rocket Launcher",
	" ",
	" ",
	" ",
	
	"Plasma Rifle",
	"Plasma Rifle",
	"Plasma Rifle",
	"Plasma Rifle",
	"Plasma Rifle",
	" ",
	" ",
	
	"BFG 9000",
	"BFG 9000",
	"BFG 9000",
	"BFG 9000",
	"BFG 9000",
	" ",
	" ",
	
	" ",
	" ",
	" ",
	" "
};

int ResearchEntryNumbers[MAX_RESEARCHES] = {
	4301,
	4300,
	3531,
	6914,
	
	2302,
	2303,
	2304,
    2309,
	
	2411,
	2412,
	
	2440,
	
	611,
	640,
	783,
	831,
	895,
	899,
	909,
	
	6709,
	
	6880,
	7982,
	
	7991,
    8015,
	
	7995,
	8001,
	
	8010,
    9507,
	8763,
	
	8277,
	8216,
	8219,
	7066,
	
	8433,
    4811,
	
	8566,
	9104,
	
	5779,
	9999,
	7744,
	3616,
	4569
};

int ResearchCosts[MAX_RESEARCHES] = {
	60,
	80,
	55,
	105,
	
	40,
	45,
	45,
    45,
	
	55,
	55,
	
	75,
	
	40,
	40,
	45,
	45,
	45,
	45,
	55,
	
	50,
	45,
	
	40,
	50,
	
	45,
    50,
	
	50,
	55,
	
    45,
    60,
	55,
	
	50,
	10,
	55,
	50,
	
	50,
    55,
	
	60,
	65,
	
	105,
	140,
	85,
	75
};

str Research_Images[MAX_RESEARCHES] = {
    "RESBAK1",
    "RESBAK2",
    "RESBAK3",
    "RESBAK4",
    
    "RESBAK5",
    "RESBAK6",
    "RESBAK7",
    "RESBAK33",
    
    "RESBAK8",
    "RESBAK9",
    
    "RESBAK10",
    
    "RESBAK11",
    "RESBAK12",
    "RESBAK13",
    "RESBAK14",
    "RESBAK15",
    "RESBAK16",
    "RESBAK17",
    
	"RESBAK34",
	"RESBAK35",
	
    "RESBAK18",
	"RESBAK41",
	
    "RESBAK19",
    "RESBAK31",
	
    "RESBAK20",
	"RESBAK37",
	
    "RESBAK21",
    "RESBAK32",
	"RESBAK36",
	
    "RESBAK22",
    "RESBAK23",
	"RESBAK39",
	"RESBAK40",
	
    "RESBAK24",
    "RESBAK25",
	
    "RESBAK26",
	"RESBAK38",
    
    "RESBAK27",
    "RESBAK28",
    "RESBAK29",
    "RESBAK30"
};

str ResearchDescription[MAX_RESEARCHES] = {
	"By using parts from fallen enemies, we can manufacture armors of varying properties.",
	"With this breakthrough technology, we can start distributing state of the art armors to the marines, blocking 100% of the damage. Unlocks Monolith Armor.",
	"Instead of wasting medikits, by using this new technology we can allow marines to store the medikits picked up permanently. \cgMedic\c- perk increases the cap by 15% each.",
	"Unlocking the occult secrets of demonic energy, we can allow marines to use ancient trophies they come across. Allows use of accessories.",

	"Our scientists managed to create a highly ballistic shell that can scatter around and pierce targets, \cfignoring shields\c-.",
	"We can now utilize the same bullets used by magnum handguns on shotguns. Magnum shells penetrate any target.",
	"When you need some extra zap to go with your shell burst, we can use these now. They constantly lock enemies in their pain state and \cfignore shields\c-.",
	"Always nice to greet your adversaries with some ice. Allows Riot Cannon to use Nitrogen Shells.",
    
	"For when normal grenades are insufficient, try a Heavy Explosive one. Unlocks HE Grenades.",
	"Sometimes it is not raw power you need, but some sustained damage. Unlocks Sonic Grenades.",
	
	"New generation backpacks are super lightweight, marines can carry these to double the capacity of their special ammo types.",
	
	"Investigating the corpses of the various demons killed helped us unlock the secrets of their occult powers, allowing use of occult melee weaponry. Unlocks Excalibat and Necromancer's Scythe.",
	"Some can say pistols are useless but we beg to differ. Some pistols can be quite potent. Unlocks Scatter Pistol and Ruby Wand.",
	"Through scientific research we came up with stronger tiers of slot 3 weaponry. Unlocks Silver Gun and Slayer.",
	"Ever wanted to use more dangerous machineguns? Now you can! Unlocks Minigun and Ebony Cannon.",
	"Our scientists never cease to amaze us! We have new toys to blow things up with! Unlocks Rotary GL and Heavy Missile Launcher.",
	"Destorying things has never been this fun! Demons sure know how to kill things... Unlocks Nailgun and Basilisk.",
	"If you think you need a bit of sniping, try these! Unlocks Railgun and Gauss Rifle.",
	
	"A fine discovery buried in hell. Dusk blade hits real hard and lets its user experience the dusk mode. Unlocks Dusk Blade. (1)",
	"Buried deep in the bowels of the earth, inferno sword is everything a pyromaniac would want! Shoot flames, cut enemies down! Unlocks Inferno Sword.(1)",
	
	"Some of the zombies were using quite improved versions of your weaponry. Now we can utilize this for your slot 2 weapons. Unlocks Assault Rifle. (2)",
	"An intriguing weapon, capable of summoning mobile snake traps or just shoot them directly! Unlocks Viper Staff (2).",
	
	"Through many investments we can now utilize better shotgun mechanics. Unlocks Deadlock. (3)",
	"Sometimes it's better to just cool things down when things get heated up. Unlocks Nitrogen Crossbow. (3)",
   
    "Our scientists were obsessed with creating energy in fixed bursts and now they can! Unlocks Plasma Cannon. (3)",
	"Destroy your enemies with a good style. Put holes in them and burn them after with heat! Unlocks Shocker. (3)",
	
	"Finally an answer to the undead menace, this silver bullet shooting machine gun will make quick work of undeads and magical creatures alike! Unlocks Templar MG. (4)",
	"A combination of chaingun and shotgun, this fierce weapon is going to make demon paste. Can also use alternate shells. Unlocks Riot Cannon (4).",
    "We now have perfect material to contain powerful acid to use against demons! A very potent rifle. Unlocks Acid Rifle (4).",
	
	"Using energies of demons we can now create meteors at will and so can you! Unlocks Meteor Launcher. (5)",
	"It always occured to us, why don't we have a Grenade Launcher when we have a Rocket Launcher? Yeah, now we have both. (5)",
	"A grenade launcher on steroids! Shoots shrapnel filled grenades! Unlocks Heavy Grenade Launcher. (5)",
	"When you're desperate to get some ice for your drink, there's always a solution! Unlocks Freezer Cannon. (5)",
	
	"Burning enemies to crisps is never boring! Unlocks Flame Thrower. (6)",
	"For when you want to make a shocking entrance. Unlocks Lightning Gun. (6)",
    
	"We have miniguns, laser cannons and other various toys but not an Ion Cannon... Now we do! (7)",
	"An ominous staff, capable of electrocuting large groups of enemies at once. Unlocks Thunder Staff. (7)",
	
	"With this groundbreaking research, we can now utilize powers of the demons to empower ourselves! Unlocks certain abilities.",
	"Powerful demons can teach us a lot of things. Now we can utilize their immense power for even better weapons! Unlocks slot 8 weapons.",
	"Nano-technology is finally here! Unlocks certain abilities.",
	"Artifacts have always been an elusive aspect. However with demon technology we can harness even more! Unlocks certain artifacts."
	
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
	POPUP_NOTALENTPOINT
};

#define MAX_POPUPS POPUP_NOTALENTPOINT + 1
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
	"You need to acquire\ntalent points!"
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

#define MAX_HELPTEXT_ORBS MAX_ORBS
str HelpText_Orbs[MAX_HELPTEXT_ORBS] = {
	"\c[Y5]Orb of Enhancement\nAllows the user to improve the quality of the currently selected weapon upon use, increasing its damage. Increases damage of selected weapon by \cd1%\c-, up to \cg25%\c- total.",
	"\c[Y5]Orb of Corruption\nThis orb can do many things. Half the time it'll do nothing. More often than not it'll do something bad. However it can do some awesome things as well... Dare to find out?",
	"\c[Y5]Orb of Spirit\nGrants the user a random stat upon use. Stats can go above \cd100\c- with this orb. Charisma has lowest weight, followed by Vitality and Bulkiness. Rest share the highest weight.",
	"\c[Y5]Orb of Repentance\nAllows you to undo the effects of the most recently used orb. You can't undo the effects undone by this orb or the \c[Y5]Orb of Riches\c-.",
	"\c[Y5]Orb of Affluence\nDoubles the effect of the next orb. This effect can stack with itself up to a multiplier of \cg16\c-.",
	"\c[Y5]Orb of Calamity\nTurns a random orb into another. It won't work if you have no other orbs or if you only have these orbs.",
	"\c[Y5]Orb of Prosperity\nIncreases health and armor caps by \cd1\c- up to a maximum of \cg500\c-.",
	"\c[Y5]Orb of Wisdom\nIncreases experience gain from monsters by \cd1%\c- up to a maximum of \cg250%\c-.",
	"\c[Y5]Orb of Greed\nIncreases credit gain from monsters by \cd1%\c- up to a maximum of \cg250%\c-.",
	"\c[Y5]Orb of Violence\nIncreases a random damage type's damage by \cd1%\c- up to a maximum of \cg300%\c- for each category.",
	"\c[Y5]Orb of Fortitude\nIncreases health and armor caps by \cd1%\c- up to a maximum of \cg200%\c-.",
	"\c[Y5]Orb of Sin\nA pact with the devil itself, trading anywhere from \cg1 to 8\c- stat points for random benefits. Can give some stat points, critical chance, or even a perk point!",
	"\c[Y5]Orb of Riches\nGrants a random resource on use. Can grant experience, credit or budget. Base values are \d5%\c- of your current level's experience, \cd2000\c- and \cd5k\c- respectively.",
	"\c[Y5]Orb of Holding\nIncreases your ammo capacities by \cd1%\c- up to a maximum of \cg100%\c-. Doesn't increase the capacity of \cusouls\c- or \cstemporary weapons."
};

str OrbIcons[MAX_HELPTEXT_ORBS] = {
	"ORB1D0",
	"ORB1B0",
	"ORB1I0",
	"ORB1Z0",
	"ORB3A0",
	"ORB1N0",
	"ORB1S0",
	"ORB2J0",
	"ORB3E0",
	"ORB2O0",
	"ORB2V0",
	"ORB2E0",
	"ORB3G0",
	"ORB3I0"
};

#define MAX_HELPTEXT_MAIN 3
str HelpText_Main[MAX_HELPTEXT_MAIN] = {
	"This is Doom's first interactive super cool UAC Menu, ran by none other than \c[Y5]Bobby\c- \cj\"\c-\cmCombine\cqb\cjo\crb\cmnt\c-\" \c[Y5]Sanchez\c-! Through this interface, you're directly connected to the UAC Database! You can make your purchases of various goodies here.",
	"Use your mouse cursor and left click to do your selections. There are various pages with various items in them, so be sure to explore all of it! If you wish to sell a weapon, use altfire key to do so.",
	"I don't have all day, I have yet to finish my important projects such as \cdM\ctS\cfP\crD\c- so don't bother me for long! Buy your shit and leave!"
};

// LegendaryKills
#define MAX_LEGMONS_TEXT 4
str LegendaryMonsterText[MAX_LEGMONS_TEXT] = {
	"\c[Y5]The Dreaming God\nThe evil entity known only as the \"Dreaming God\" wanders realms unknown to many for as long as the universe, or at least that's what we think. Research shows this creature is immune to any magical or elemental attack.",
	"\c[Y5]Torrasque\nThis unwavering creature appears to be magically created by an immensely powerful wizard, long ago. Now it chose this realm to feast upon. We found that the creature is highly resistant to physical, energy and explosive attacks.",
	"\c[Y5]Mordecqai\nOnce the great ruler of the \"Realm of Suffering\", now a wanderer to consume and gain powers of whatever creature comes its way. It seems to be completely immune to fire and explosive attacks.",
	"\c[Y5]God Slayer\nA fearsome mech built by UAC to destroy the demonic menace, gone rogue by the evil powers of Hell. The robot will target only humans and has some interesting AI... Seems to be impervious to bullets. Explosives seem to do little, too."
};

str LegendaryMonsterIcons[MAX_LEGMONS_TEXT] = {
	"LEG_DRMR",
	"LEG_TORR",
	"LEG_MORD",
	"LEG_GODS"
};

#define MAX_MONSTER_MODS 35
str Help_EliteModLabel[MAX_MONSTER_MODS] = {
	"Explosive Resist",
	"High Explosive Resist",
	"Explosive Immune",
	"Physical Resist",
	"Ghost",
	"Hardened Skin",
	"Reflective",
	"Summon Minion",
	"Can Curse",
	"Can Heal",
	"Can Block",
	"Can Split",
	"Can Raise",
	"Can Teleport",
	"Resurrect Fallen",
	"Mobility",
	"Homing",
	"Poisonous",
	"Death Surprise",
	"Rage",
	"Pierces Armor",
	"Aggressive",
	"Extra Fast",
	"Fast Reaction",
	"No Pain",
	"Physical Immune",
	"Energy Resist",
	"Energy Immune",
	"Magic Resist",
	"Magic Immune",
	"Elemental Resist",
	"Elemental Immune",
	"Extra Strong",
	"Vital",
	"Armor Penetration"
};

str Help_EliteModExplanation[MAX_MONSTER_MODS] = {
	"Monster takes 50% reduced damage from explosives.",
	"Monster is immune to area damage.",
	"Monster is immune to explosives.",
	"Monster takes 50% reduced damage from physical melee attacks and 25% reduced damage from ballistic attacks.",
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
	"Monster is immune to any kind of ballistic attack.",
	"Monster takes 50% reduced damage from energy attacks.",
	"Monster is immune to energy attacks.",
	"Monster takes 50% reduced damage from magical attacks.",
	"Monster is immune to magical attacks.",
	"Monster takes 50% reduced damage from elemental attacks.",
	"Monster is immune to elemental attacks.",
	"Monster deals twice as much damage.",
	"Monster has twice as much health.",
	"Monster ignores armor of player with every attack."
};

#endif