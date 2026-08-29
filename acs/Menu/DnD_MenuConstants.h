#ifndef DND_MENUCONSTANTS_IN
#define DND_MENUCONSTANTS_IN

#include "../DnD_Ammo.h"
#include "../DnD_SpecialAmmo.h"
#include "../DnD_Artifacts.h"
#include "../DnD_Abilities.h"
#include "../Inventory/DnD_OrbDefs.h"

#define DND_QUICKBUY_POPUPID 0xFFFF
#define DND_LANGUAGE_LOOKUP true
#define DND_NOLOOKUP false

#define DND_BUDGET_CONVERSION_CREDIT 10000
#define DND_TRANSMUTE_COST 5000

#define DND_MENU_MAX_AILMENTDESCRIPTIONS 6

typedef struct {
	int val[MAX_TRANSMUTE_BOXES];
} transmute_data_T;

typedef struct {
	int id;
} sel_dungeon_data_T;

// holds topboxid from inventories in its val field
transmute_data_T module& GetTransmuteOrbData(int pnum) {
	static transmute_data_T TransmuteData[MAXPLAYERS];
	return TransmuteData[pnum];
}

sel_dungeon_data_T module& GetSelectedDungeonData(int pnum) {
	static sel_dungeon_data_T DungeonData[MAXPLAYERS];
	return DungeonData[pnum];
}

#define DND_MENU_BASEAMMOSTOCK 50

enum {
	DRAW_STAT_OFFENSE1,
	DRAW_STAT_OFFENSE2,
	DRAW_STAT_DEFENSE,
	DRAW_STAT_UTILITY,
	DRAW_STAT_MISC, // for charges and stuff on player
	DRAW_STAT_EXOTIC // unique item effects -- nothing here rolls on a regular item
};

// Page definitions
enum { 
	MENU_NULL,
	MENU_STAT1,
	MENU_STAT2,
	MENU_STAT2_OFFENSE1,
	MENU_STAT2_OFFENSE2,
	MENU_STAT2_DEFENSE,
	MENU_STAT2_UTILITY,
	MENU_STAT2_EXOTIC,
	MENU_STAT2_MISC,
	
	MENU_PERK,
	
	// One page per archetype, in PERK_ARCH_* order so the page IS the archetype:
	// curopt - MENU_PERKTREE_FIRST. Seven pages rather than one page with a variable, because the
	// pane is rebuilt on page CHANGE and a variable would swap archetypes without it ever noticing.
	MENU_PERK_ACRO,
	MENU_PERK_ASSN,
	MENU_PERK_CUN,
	MENU_PERK_END,
	MENU_PERK_MART,
	MENU_PERK_PERC,
	MENU_PERK_TORM,
	
	MENU_LOAD,
	MENU_LOAD1,
	MENU_LOAD_INVENTORY,
	MENU_LOAD_STASH,
	MENU_LOAD_CHARM,
	MENU_LOAD_WEAPONDISCARD,
	MENU_LOAD_CRAFTING,
	MENU_LOAD_CRAFTING_WEAPON,
	MENU_LOAD_CRAFTING_INVENTORY,
	MENU_LOAD_CRAFTING_TRANSMUTING,
	MENU_LOAD_ACC1,
	MENU_LOAD_ACC2,
	MENU_LOAD_ACC3,
	MENU_LOAD_ACC4,
	
	MENU_LOAD_TRADE1,
	MENU_LOAD_TRADE2,
	MENU_LOAD_TRADE3,
	MENU_LOAD_TRADE4,
	MENU_LOAD_TRADE5,

	MENU_LOAD_DUNGEONS,
	
	MENU_SHOP,
	MENU_SHOP_WEAPON,
	MENU_SHOP_WEAPON1,
	MENU_SHOP_WEAPON2,
	MENU_SHOP_WEAPON3_1,
	MENU_SHOP_WEAPON3_2,
	MENU_SHOP_WEAPON3_3,
	MENU_SHOP_WEAPON4_1,
	MENU_SHOP_WEAPON4_2,
	MENU_SHOP_WEAPON5_1,
	MENU_SHOP_WEAPON5_2,
	MENU_SHOP_WEAPON6_1,
	MENU_SHOP_WEAPON6_2,
	MENU_SHOP_WEAPON7,
	MENU_SHOP_WEAPON8,
	
	MENU_SHOP_AMMOSELECT,
	// the following 4 are for ammo categories
	MENU_SHOP_AMMO_1,
	MENU_SHOP_AMMO_1_2,
	MENU_SHOP_AMMO_2,
	MENU_SHOP_AMMO_2_2,
	MENU_SHOP_AMMO_3,
	MENU_SHOP_AMMO_4_1,
	MENU_SHOP_AMMO_4_2,
	MENU_SHOP_AMMO_SPECIAL1,
	
	// The ability and artifact shop pages are gone. Everything they sold still exists and still
	// works when granted -- the actors, the SHOP_ABILITY_* / SHOP_ARTI_* ids, their table rows and
	// every hook that reads them. There is simply no counter selling them any more.
	
	MENU_SHOP_ACCOUNT,
	
	MENU_RESEARCH,
	MENU_RESEARCH_GUNS, // this leads to a subsection, add subsections below this
	MENU_RESEARCH_BODY,
	MENU_RESEARCH_AMMO,
	MENU_RESEARCH_SLOTGUNS,
	MENU_RESEARCH_LUXURYGUNS,
	MENU_RESEARCH_UTILITY,
	MENU_MAIN,
	
	MENU_HELP,
	MENU_HELP_CHARACTER,
	MENU_HELP_CLASSPERKS,
	MENU_HELP_WEAPONPROPS,
	MENU_HELP_RESEARCHES,
	MENU_HELP_DAMAGETYPES,
	MENU_HELP_AILMENTS,
	MENU_HELP_ORBS,
	MENU_HELP_MMODS,
	MENU_HELP_LEGMONS,
	
	MENU_HELP_MMODS_WEAKNESS,
	MENU_HELP_MMODS_RESIST,
	MENU_HELP_MMODS_IMMUNITY,
	MENU_HELP_MMODS_AGGRESSIVE,
	MENU_HELP_MMODS_DEFENSIVE,
	MENU_HELP_MMODS_UTILITY,
	MENU_HELP_MMODS_SPECIAL,

	MENU_ABILITY,
	
	#ifdef ISAPRILFIRST
	MENU_SHOP_NFT
	#endif
};

#define MAX_WEPS_PER_PAGE 8

#define SHOP_FIRSTWEAPON_PAGE MENU_SHOP_WEAPON1
#define SHOP_LASTWEAPON_PAGE MENU_SHOP_WEAPON8

#define SHOP_FIRSTAMMO_PAGE MENU_SHOP_AMMO_1
#define SHOP_FIRSTAMMO_INDEX SHOP_AMMO_CLIP

#define SHOP_FIRSTAMMOSPECIAL_INDEX SHOP_AMMO_FLECHETTE
#define SHOP_FIRSTAMMOSPECIAL_GRENADE_INDEX SHOP_AMMO_SONICGRENADE

#define SHOP_FIRSTARTI1_INDEX SHOP_ARTI_KIT
#define SHOP_FIRSTARTI2_INDEX SHOP_ARTI_RADSUIT

#define SHOP_ABILITY1_BEGIN SHOP_ABILITY_PARRY
#define SHOP_ABILITY2_BEGIN SHOP_ABILITY_TEMPORAL

#define SHOP_ACCOUNT_BEGIN SHOP_ACCOUNT_STASHTAB

#define SHOP_WEAPON_BEGIN SHOP_WEP_CSAW
#define SHOP_WEAPON1_BEGIN SHOP_WEP_CSAW
#define SHOP_WEAPON2_BEGIN SHOP_WEP_AKIMBOPISTOL
#define SHOP_WEAPON31_BEGIN SHOP_WEP_PURIFIER
#define SHOP_WEAPON32_BEGIN SHOP_WEP_ERASUS
#define SHOP_WEAPON33_BEGIN SHOP_WEP_SLAYER
#define SHOP_WEAPON41_BEGIN SHOP_WEP_HMG
#define SHOP_WEAPON42_BEGIN SHOP_WEP_DESOLATOR
#define SHOP_WEAPON51_BEGIN SHOP_WEP_TORPEDO
#define SHOP_WEAPON52_BEGIN SHOP_WEP_RESRL5
#define SHOP_WEAPON61_BEGIN SHOP_WEP_NUCLEARPL
#define SHOP_WEAPON62_BEGIN SHOP_WEP_RHINO
#define SHOP_WEAPON7_BEGIN SHOP_WEP_BFG
#define SHOP_WEAPON8_BEGIN SHOP_WEP_DEATHSTAFF

#define SHOP_WEAPON_SLOT1END SHOP_WEP_SICKLE
#define SHOP_WEAPON_SLOT2END SHOP_WEP_SCATTERPISTOL
#define SHOP_WEAPON_SLOT31END SHOP_WEP_HSSG
#define SHOP_WEAPON_SLOT32END SHOP_WEP_SILVER
#define SHOP_WEAPON_SLOT33END SHOP_WEP_SLAYER
#define SHOP_WEAPON_SLOT3END SHOP_WEP_SLAYER
#define SHOP_WEAPON_SLOT41END SHOP_WEP_RESMG5
#define SHOP_WEAPON_SLOT42END SHOP_WEP_MPPB
#define SHOP_WEAPON_SLOT51END SHOP_WEP_RESRL4
#define SHOP_WEAPON_SLOT52END SHOP_WEP_SEDRIN
#define SHOP_WEAPON_SLOT61END SHOP_WEP_RESPL4
#define SHOP_WEAPON_SLOT62END SHOP_WEP_BASILISK
#define SHOP_WEAPON_SLOT7END SHOP_WEP_DEATHRAY
#define SHOP_WEAPON_SLOT8END SHOP_WEP_REAVER

// Weapon slot stuff

#define SHOP_WEAPON1CSAW_BEGIN SHOP_WEP_CSAW
#define SHOP_WEAPON1CSAW_END SHOP_WEP_INFERNOSWORD
#define SHOP_WEAPON2PISTOL_BEGIN SHOP_WEP_AKIMBOPISTOL
#define SHOP_WEAPON2PISTOL_END SHOP_WEP_RESPIS2
#define SHOP_WEAPON3SG_BEGIN SHOP_WEP_PURIFIER
#define SHOP_WEAPON3SG_END SHOP_WEP_RESSG4
#define SHOP_WEAPON3SSG_BEGIN SHOP_WEP_HSSG
#define SHOP_WEAPON3SSG_END SHOP_WEP_RESSSG4
#define SHOP_WEAPON4CG_BEGIN SHOP_WEP_HMG
#define SHOP_WEAPON4CG_END SHOP_WEP_RESMG5
#define SHOP_WEAPON5RL_BEGIN SHOP_WEP_TORPEDO
#define SHOP_WEAPON5RL_END SHOP_WEP_MERC
#define SHOP_WEAPON6PL_BEGIN SHOP_WEP_NUCLEARPL
#define SHOP_WEAPON6PL_END SHOP_WEP_RESPL4
#define SHOP_WEAPON7BFG_BEGIN SHOP_WEP_BFG
#define SHOP_WEAPON7BFG_END SHOP_WEP_RESBFG2

#define SHOP_LASTAMMO_PAGE MENU_SHOP_AMMO_SPECIAL1
#define SHOP_LASTAMMO_SPECIALINDEX SHOP_AMMO_HEGRENADE
#define SHOP_LASTAMMO_NORMALINDEX SHOP_AMMO_SNIPER
#define SHOP_LASTWEP_INDEX SHOP_WEP_REAVER
#define SHOP_ABILITY1_END SHOP_ABILITY_REGEN
#define SHOP_LASTABILITY_INDEX SHOP_ABILITY_SOUL
#define SHOP_ARTIFACT1_END SHOP_ARTI_RESET
#define SHOP_LASTARTI_INDEX SHOP_ARTI_RADSUIT
#define SHOP_LASTDRAWNARTI_INDEX SHOP_ARTI_BOOK

#define SHOP_RESPAGE_BEGIN MENU_RESEARCH_BODY
#define SHOP_RESPAGE_END MENU_RESEARCH_UTILITY

#define MENU_FIRST_ACCESSORY_PAGE MENU_LOAD_ACC1
#define MENU_LAST_ACCESSORY_PAGE MENU_LOAD_ACC4

#define SHOP_ACCOUNT_END SHOP_ACCOUNT_STASHTAB

#define MENU_MAXRES_PAGES (SHOP_RESPAGE_END - SHOP_RESPAGE_BEGIN + 1)

#define MAXSHOPWEAPONS (SHOP_LASTWEP_INDEX - SHOP_WEAPON_BEGIN + 1)
#define MAXSHOPNORMALAMMOS (SHOP_LASTAMMO_NORMALINDEX - SHOP_FIRSTAMMO_INDEX + 1)
#define MAXSHOPAMMOS (SHOP_LASTAMMO_SPECIALINDEX - SHOP_FIRSTAMMO_INDEX + 1)

#define MENU_HELP_BEGIN MENU_HELP_CHARACTER
#define MENU_HELP_END MENU_HELP_LEGMONS

#define MENU_MMOD_HELP_BEGIN MENU_HELP_MMODS_WEAKNESS
#define MENU_MMOD_HELP_END MENU_HELP_MMODS_SPECIAL

#define AMMO_ICON 0
#define AMMO_NAME 1	
#define AMMO_PAGE2_BEGIN (SHOP_FIRSTAMMO2_INDEX - SHOP_FIRSTAMMO_INDEX)
#define AMMO_PAGE3_BEGIN (SHOP_FIRSTAMMO3_INDEX - SHOP_FIRSTAMMO_INDEX)
#define AMMO_PAGESPECIAL_BEGIN (SHOP_FIRSTAMMOSPECIAL_INDEX - SHOP_FIRSTAMMO_INDEX)

#define MENU_LAST_TRADE MENU_LOAD_TRADE5
#define MENU_FIRST_TRADE MENU_LOAD_TRADE1
#define MENU_TRADE_PAGECOUNT (MENU_LAST_TRADE - MENU_FIRST_TRADE + 1)
#define MENU_TRADE_PLAYERCOUNT 16

#define MENU_LOAD_CRAFTING_FIRST MENU_LOAD_CRAFTING_WEAPON
#define MENU_LOAD_CRAFTING_LAST MENU_LOAD_CRAFTING_TRANSMUTING

#define MAXACCOUNTITEMS (SHOP_ACCOUNT_END - SHOP_ACCOUNT_BEGIN + 1)

#define MAX_ACCESSORIES (DND_ACCESSORY_LICHARM + 1)

// The tree page's layout, in screen pixels. Here rather than beside the drawing code because the
// scroll switch near the top of DnD_MenuFuncs.h reads them, and a macro cannot be forward referenced
// in bcs the way a function can.
//
// The page has TWO scrolling regions: the list of perks, and the detail panel under it. They keep
// separate positions -- see perk_scroll_T -- because the panel's text runs to eight lines for the
// wordiest perks and reading it should not cost you your place in the tree.
// The text column. It has to STOP SHORT of the scroll bar, which stands at DND_SCROLLBAR_X and is
// drawn over whatever reaches it -- 192 + 248 lands at 440, nine pixels clear of it. This is the clip
// width and the wrap width both: wrapping alone would still let an unbreakable run overhang, and
// clipping alone would cut mid-word.
#define DND_PERKLIST_X       192
#define DND_PERKLIST_W       248

#define DND_PERKLIST_TOP     76
#define DND_PERKLIST_H       128    // exactly 8 rows, so the step below divides it
#define DND_PERKROW_H        16
#define DND_PERKINDENT       8

// One row per step, so a row is never left half inside the window. That in turn is what makes the
// clip pad safe: with a partial row possible, padding the top would show the bottom of the row above.
#define DND_PERKSCROLL_STEP  DND_PERKROW_H

// Glyphs sit a little above the y they are drawn at, so a clip that starts exactly at the first row
// shaves its top pixel off. Padding is added upward and taken out of the height, leaving the bottom
// edge where it was.
#define DND_PERKLIST_CLIPPAD 4

// How tall a row's click target is, centred on its text. Eight matches every hand-written bp[] row
// in the layout table -- a taller one reaches into the gap below and picks the wrong perk.
#define DND_PERKROW_HIT      8

// Which bar drives which region. The list takes bar 0 because that is the one whose position lives
// in ScrollPos.x, which is what the rest of the menu resets on a page change.
#define DND_PERKBAR_LIST     0
#define DND_PERKBAR_PANEL    1

// The archetype index runs out of rows at y 208 and the panel goes to 280, so the dash toggle lives
// in that gap behind a rule. It is not a perk, which is exactly why it is fenced off rather than
// dropped in as an eighth row -- and the box after the last archetype is the same "extras go last"
// convention the tree page uses for its back arrow.
#define DND_PERKINDEX_DIVIDER_Y 226.2
#define DND_PERKINDEX_TOGGLE_Y  240.2
#define DND_PERKINDEX_HINT_Y    280.2
#define DND_PERKINDEX_TOGGLEBOX (MBOX_1 + PERK_ARCH_COUNT)

#define DND_PERKDIVIDER_Y    206
#define DND_PERKPANEL_TOP    216
#define DND_PERKPANEL_H      64     // 8 lines, which is the wordiest perk in the tree
#define DND_PERKPANEL_LINE   8

// Rough characters per wrapped line. ACS cannot measure a rendered string, so the panel's extent is
// estimated from its length -- only the scroll bar's thumb depends on this being close.
#define DND_PERKPANEL_CPL    31

#define MENU_PERKTREE_FIRST MENU_PERK_ACRO
#define MENU_PERKTREE_LAST  MENU_PERK_TORM

bool IsPerkTreePage(int page) {
	return page >= MENU_PERKTREE_FIRST && page <= MENU_PERKTREE_LAST;
}

bool IsWeaponPage(int page) {
	return page >= SHOP_FIRSTWEAPON_PAGE && page <= SHOP_LASTWEAPON_PAGE;
}

bool IsAmmoPage(int page) {
	return page >= SHOP_FIRSTAMMO_PAGE && page <= SHOP_LASTAMMO_PAGE;
}

bool IsCraftingPageForTokens(int page) {
	return page == MENU_LOAD_CRAFTING_WEAPON || page == MENU_LOAD_CRAFTING_INVENTORY;
}

#define SHOP_SLOT1_END SHOP_WEP_SICKLE
#define SHOP_SLOT2_END SHOP_WEP_SCATTERPISTOL
#define SHOP_SLOT3_END SHOP_WEP_RESSG4
#define SHOP_SLOT3SSG_END SHOP_WEP_RESSSG4
#define SHOP_SLOT4_END SHOP_WEP_RESMG5
#define SHOP_SLOT5_END SHOP_WEP_RESRL5
#define SHOP_SLOT6_END SHOP_WEP_RESPL4
#define SHOP_SLOT7_END SHOP_WEP_RESBFG2
int GetDiscardSlotFromShopWeapon(int id) {
	if(id <= SHOP_SLOT1_END)
		return 0;
	if(id <= SHOP_SLOT2_END)
		return 1;
	if(id <= SHOP_SLOT3_END)
		return 2;
	if(id <= SHOP_SLOT3SSG_END)
		return 3;
	if(id <= SHOP_SLOT4_END)
		return 4;
	if(id <= SHOP_SLOT5_END)
		return 5;
	if(id <= SHOP_SLOT6_END)
		return 6;
	if(id <= SHOP_SLOT7_END)
		return 7;
	return -1;
}

#ifdef ISAPRILFIRST
	#define MAX_NFTS 8
	#define NFT_COST 1000000 // 1 million dollars!
#endif

#endif
