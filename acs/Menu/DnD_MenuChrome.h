#ifndef DND_MENUCHROME_IN
#define DND_MENUCHROME_IN

// The redesigned menu canvas. The engine maps a hud's width onto the 4:3 area of the screen, so a 4:3 canvas is the
// only one drawn 1:1; the 480x320 design space it stands in for is 3.0 wide and 3.375 tall per unit at 1080p.
#define MCH_W 1440
#define MCH_H 1080
#define MCH_SCALE_X 3.0
#define MCH_SCALE_Y 3.375

#define MCH_FX -220
#define MCH_FY 10
#define MCH_FW 1880
#define MCH_FH 1060
// how far past the design space the cursor may go on these pages: the 16:9 width
#define MCH_CURSOR_OVER 80

// Lower ids draw in front; ClearMenuDisplay already sweeps this band when the menu closes.
#define MCH_ID_ROWTXT		1760
#define MCH_ID_PANETXT		1796
#define MCH_ID_PAGETXT		1820
#define MCH_ID_CHIPTXT		1832
#define MCH_ID_TABTXT		1842
#define MCH_ID_STATTXT		1859
// sub tabs: labels from MCH_ID_SUBTXT, plates from MCH_ID_SUBIMG
#define MCH_MAX_SUBS		11
#define MCH_ID_PANEIMG		1862
#define MCH_ID_ROWIMG		1892
#define MCH_ID_CHIPIMG		1916
#define MCH_ID_PAGEIMG		1926
#define MCH_ID_BADGEIMG		1934
#define MCH_ID_TABIMG		1938
#define MCH_ID_FIRST		MCH_ID_ROWTXT
#define MCH_ID_SUBTXT		1960
#define MCH_ID_SUBIMG		1972
#define MCH_ID_LAST			1983

// Chrome clicks travel as mainboxid MCH_NAV_BASE + element code; the server decodes the code against its own page.
#define MCH_NAV_BASE		256
#define MCH_EL_TAB			0
#define MCH_EL_SUB			32
#define MCH_EL_CHIP			64
#define MCH_EL_PAGER		96
#define MCH_EL_CLOSE		128
// the confirm popup's buttons never reach the server as nav: they turn into the old confirm / cancel clicks
#define MCH_EL_POPYES		160
#define MCH_EL_POPNO		161
// research list rows: picked on the client, never sent
#define MCH_EL_ROW			192
// crafting: a material into the chamber (+ its inventory box), a recipe into it (+ the orb), emptying it
#define MCH_EL_CRFILL		512
#define MCH_EL_CRRECIPE		576
#define MCH_EL_CREMPTY		704
#define MCH_RES_BTNX		634
#define MCH_RES_BTNY		852
#define MCH_RES_BTNSTEP		220
#define MCH_CODEX_TOPICS	9
// codex landing page: the options rows sit in the pane, MMODS' category rows too
#define MCH_CODEX_OPTY		626
#define MCH_CODEX_CATY		420

// the confirm popup draws in front of the whole menu, just behind the cursor; the menu's close sweep covers it
#define MCH_ID_POPTXT		108
#define MCH_ID_POPIMG		112
#define MCH_ID_POPFIRST		108
#define MCH_ID_POPLAST		115
#define MCH_POP_X			410
#define MCH_POP_Y			390
#define MCH_POP_W			620
#define MCH_POP_BTNY		612
#define MCH_POP_YESX		500
#define MCH_POP_NOX			740

// layout, canvas pixels
#define MCH_TAB_X			-184
#define MCH_TAB_Y			42
#define MCH_TAB_W			200
#define MCH_TAB_H			52
#define MCH_TAB_STEP		210
#define MCH_KEY_X			1358
#define MCH_KEY_W			50
#define MCH_KEY_STEP		56
#define MCH_CLOSE_X			1528
#define MCH_CLOSE_W			96
#define MCH_SUB_X			-184
#define MCH_SUB_Y			116
#define MCH_SUB_W			150
#define MCH_SUB_H			34
#define MCH_SUB_STEP		160
#define MCH_CHIP_X			210
#define MCH_CHIP_Y			200
#define MCH_CHIP_W			48
#define MCH_CHIP_H			42
#define MCH_CHIP_STEP		58
#define MCH_ROW_X			-146
#define MCH_ROW_Y			278
#define MCH_ROW_W			720
#define MCH_ROW_H			54
#define MCH_ROW_STEP		60
#define MCH_PAGER_Y			892
#define MCH_PANE_X			610
#define MCH_PANE_Y			278
#define MCH_STATUS_Y		1020
#define MCH_PANE_IX 634
#define MCH_PANE_TX 920
#define MCH_PANE_TEXTW 928
#define MCH_PANE_FOOT 868

enum {
	MCH_SEC_CHARACTER,
	MCH_SEC_SKILLS,
	MCH_SEC_GEAR,
	MCH_SEC_SHOP,
	MCH_SEC_RESEARCH,
	MCH_SEC_WORLD,
	MCH_SEC_CODEX,
	MCH_SEC_COUNT
};
#define MCH_SEC_NONE -1

enum {
	MCH_KIND_NONE,
	MCH_KIND_SHOPINDEX,
	MCH_KIND_SLOTLIST,
	MCH_KIND_WEAPONS,
	MCH_KIND_AMMOSELECT,
	MCH_KIND_AMMO,
	MCH_KIND_ACCOUNT,
	MCH_KIND_OVERVIEW,
	MCH_KIND_STATINDEX,
	MCH_KIND_STATS,
	MCH_KIND_PERKINDEX,
	MCH_KIND_PERKTREE,
	MCH_KIND_RESINDEX,
	MCH_KIND_RESGUNS,
	MCH_KIND_RESLIST,
	MCH_KIND_CODEXINDEX,
	MCH_KIND_CODEX,
	MCH_KIND_SKILLINDEX,
	MCH_KIND_GEARINDEX,
	MCH_KIND_GEARDISCARD,
	MCH_KIND_GEARACC,
	MCH_KIND_GEAREQUIP,
	MCH_KIND_GEARINV,
	MCH_KIND_GEARSTASH,
	MCH_KIND_GEARTRADE,
	MCH_KIND_DUNGEONS,
	MCH_KIND_HOME,
	MCH_KIND_CRAFT
};

bool IsChromePage(int page) {
	return (page >= MENU_SHOP && page <= MENU_SHOP_ACCOUNT) || (page >= MENU_STAT1 && page <= MENU_PERKTREE_LAST) || (page >= MENU_RESEARCH && page <= SHOP_RESPAGE_END) || (page >= MENU_HELP && page <= MENU_MMOD_HELP_END) || page == MENU_SPELL || page == MENU_LOAD || page == MENU_LOAD_INVENTORY || page == MENU_LOAD_STASH || page == MENU_LOAD_CHARM || page == MENU_LOAD_WEAPONDISCARD
		|| (page >= MENU_FIRST_TRADE && page <= MENU_LAST_TRADE) || page == MENU_LOAD_DUNGEONS || page == MENU_MAIN
		|| (page >= MENU_FIRST_ACCESSORY_PAGE && page <= MENU_LAST_ACCESSORY_PAGE) || IsChromeCraftPage(page);
}

// Pages that draw the frame around content of their own: every chrome page, and the skill pages.
bool IsChromeFramed(int page) {
	return IsChromePage(page) || IsSpellPage(page);
}

int GetChromeKind(int page) {
	if(page == MENU_SHOP)
		return MCH_KIND_SHOPINDEX;
	if(page == MENU_SHOP_WEAPON)
		return MCH_KIND_SLOTLIST;
	if(page >= MENU_SHOP_WEAPON1 && page <= MENU_SHOP_WEAPON8)
		return MCH_KIND_WEAPONS;
	if(page == MENU_SHOP_AMMOSELECT)
		return MCH_KIND_AMMOSELECT;
	if(page >= MENU_SHOP_AMMO_1 && page <= MENU_SHOP_AMMO_SPECIAL1)
		return MCH_KIND_AMMO;
	if(page == MENU_SHOP_ACCOUNT)
		return MCH_KIND_ACCOUNT;
	if(page == MENU_STAT1)
		return MCH_KIND_OVERVIEW;
	if(page == MENU_STAT2)
		return MCH_KIND_STATINDEX;
	if(page >= MENU_STAT2_OFFENSE1 && page <= MENU_STAT2_MISC)
		return MCH_KIND_STATS;
	if(page == MENU_PERK)
		return MCH_KIND_PERKINDEX;
	if(IsPerkTreePage(page))
		return MCH_KIND_PERKTREE;
	if(page == MENU_RESEARCH)
		return MCH_KIND_RESINDEX;
	if(page == MENU_RESEARCH_GUNS)
		return MCH_KIND_RESGUNS;
	if(page >= SHOP_RESPAGE_BEGIN && page <= SHOP_RESPAGE_END)
		return MCH_KIND_RESLIST;
	if(page == MENU_HELP)
		return MCH_KIND_CODEXINDEX;
	if(page > MENU_HELP && page <= MENU_MMOD_HELP_END)
		return MCH_KIND_CODEX;
	if(page == MENU_SPELL)
		return MCH_KIND_SKILLINDEX;
	if(page == MENU_LOAD)
		return MCH_KIND_GEARINDEX;
	if(page == MENU_LOAD_WEAPONDISCARD)
		return MCH_KIND_GEARDISCARD;
	if(page == MENU_LOAD_CHARM)
		return MCH_KIND_GEAREQUIP;
	if(page == MENU_LOAD_INVENTORY)
		return MCH_KIND_GEARINV;
	if(page == MENU_LOAD_STASH)
		return MCH_KIND_GEARSTASH;
	if(page >= MENU_FIRST_TRADE && page <= MENU_LAST_TRADE)
		return MCH_KIND_GEARTRADE;
	if(page == MENU_LOAD_DUNGEONS)
		return MCH_KIND_DUNGEONS;
	if(page == MENU_MAIN)
		return MCH_KIND_HOME;
	if(IsChromeCraftPage(page))
		return MCH_KIND_CRAFT;
	if(page >= MENU_FIRST_ACCESSORY_PAGE && page <= MENU_LAST_ACCESSORY_PAGE)
		return MCH_KIND_GEARACC;
	return MCH_KIND_NONE;
}

int GetChromeSection(int page) {
	if(page == MENU_LOAD_DUNGEONS)
		return MCH_SEC_WORLD;
	if(page >= MENU_SHOP && page <= MENU_SHOP_ACCOUNT)
		return MCH_SEC_SHOP;
	if(page >= MENU_STAT1 && page <= MENU_PERKTREE_LAST)
		return MCH_SEC_CHARACTER;
	if(page >= MENU_RESEARCH && page <= SHOP_RESPAGE_END)
		return MCH_SEC_RESEARCH;
	if(page >= MENU_HELP && page <= MENU_MMOD_HELP_END)
		return MCH_SEC_CODEX;
	if(page == MENU_SPELL || IsSpellPage(page))
		return MCH_SEC_SKILLS;
	if((page >= MENU_FIRST_TRADE && page <= MENU_LAST_TRADE) || page == MENU_LOAD || page == MENU_LOAD_INVENTORY || page == MENU_LOAD_STASH || page == MENU_LOAD_CHARM || page == MENU_LOAD_WEAPONDISCARD || (page >= MENU_FIRST_ACCESSORY_PAGE && page <= MENU_LAST_ACCESSORY_PAGE) || IsChromeCraftPage(page))
		return MCH_SEC_GEAR;
	return MCH_SEC_NONE;
}

str GetChromeTabLabel(int sec) {
	switch(sec) {
		case MCH_SEC_CHARACTER:
		return "DND_MCH_TAB_CHARACTER";
		case MCH_SEC_SKILLS:
		return "DND_MCH_TAB_SKILLS";
		case MCH_SEC_GEAR:
		return "DND_MCH_TAB_GEAR";
		case MCH_SEC_SHOP:
		return "DND_MCH_TAB_SHOP";
		case MCH_SEC_RESEARCH:
		return "DND_MCH_TAB_RESEARCH";
		case MCH_SEC_WORLD:
		return "DND_MCH_TAB_WORLD";
	}
	return "DND_MCH_TAB_CODEX";
}

// Where each tab lands until its section is redesigned.
int GetChromeTabTarget(int sec) {
	switch(sec) {
		case MCH_SEC_CHARACTER:
		return MENU_STAT1;
		case MCH_SEC_SKILLS:
		return MENU_SPELL;
		case MCH_SEC_GEAR:
		return MENU_LOAD;
		case MCH_SEC_SHOP:
		return MENU_SHOP_WEAPON1;
		case MCH_SEC_RESEARCH:
		return MENU_RESEARCH;
		case MCH_SEC_WORLD:
		return MENU_LOAD_DUNGEONS;
		case MCH_SEC_CODEX:
		return MENU_HELP;
	}
	return -1;
}

int GetChromeSubCount(int sec) {
	if(sec == MCH_SEC_SHOP || sec == MCH_SEC_CHARACTER)
		return 3;
	if(sec == MCH_SEC_RESEARCH)
		return 5;
	if(sec == MCH_SEC_SKILLS)
		return 3 + MAX_SKILL_TREES;
	if(sec == MCH_SEC_GEAR)
		return 8;
	return 0;
}

// the skill section's eleven sub tabs are narrower
int GetChromeSubW(int sec) {
	if(sec == MCH_SEC_SKILLS || sec == MCH_SEC_GEAR)
		return 128;
	return MCH_SUB_W;
}

str GetChromeSubLabel(int sec, int i) {
	if(sec == MCH_SEC_GEAR)
		return i ? StrParam(s:"DND_MCH_GSUB", d:i + 1) : "DND_MCH_SUB_OVERVIEW";
	if(sec == MCH_SEC_SKILLS) {
		if(!i)
			return "DND_MCH_SUB_OVERVIEW";
		if(i == 1)
			return "DND_MCH_SUB_HOTBAR";
		if(i == 2)
			return "DND_MCH_SUB_GEMS";
		return StrParam(s:"DND_SPELLTREE", d:i - 3);
	}
	if(sec == MCH_SEC_RESEARCH) {
		switch(i) {
			case 0:
			return "DND_MCH_SUB_OVERVIEW";
			case 1:
			return "DND_MCH_RES_BODY";
			case 2:
			return "DND_MCH_SUB_AMMO";
			case 3:
			return "DND_MCH_SUB_WEAPONS";
		}
		return "DND_MCH_STAT_UTIL";
	}
	if(sec == MCH_SEC_CHARACTER) {
		if(!i)
			return "DND_MCH_SUB_OVERVIEW";
		if(i == 1)
			return "DND_MCH_SUB_STATSHEET";
		return "DND_MCH_SUB_PERKS";
	}
	if(!i)
		return "DND_MCH_SUB_WEAPONS";
	if(i == 1)
		return "DND_MCH_SUB_AMMO";
	return "DND_MCH_SUB_ACCOUNT";
}

int GetChromeSubTarget(int sec, int i) {
	if(sec == MCH_SEC_GEAR) {
		if(i < 0 || i > 7)
			return -1;
		return i ? MchGearRowPage(i) : MENU_LOAD;
	}
	if(sec == MCH_SEC_SKILLS) {
		if(i < 0 || i >= 3 + MAX_SKILL_TREES)
			return -1;
		if(!i)
			return MENU_SPELL;
		if(i == 1)
			return MENU_SPELL_HOTBAR;
		if(i == 2)
			return MENU_SPELL_GEMS;
		return MENU_SPELLTREE_FIRST + i - 3;
	}
	if(sec == MCH_SEC_RESEARCH) {
		switch(i) {
			case 0:
			return MENU_RESEARCH;
			case 1:
			return MENU_RESEARCH_BODY;
			case 2:
			return MENU_RESEARCH_AMMO;
			case 3:
			return MENU_RESEARCH_SLOTGUNS;
			case 4:
			return MENU_RESEARCH_UTILITY;
		}
		return -1;
	}
	if(i < 0 || i > 2)
		return -1;
	if(sec == MCH_SEC_CHARACTER) {
		if(!i)
			return MENU_STAT1;
		if(i == 1)
			return MENU_STAT2_OFFENSE1;
		return MENU_PERK;
	}
	if(sec != MCH_SEC_SHOP)
		return -1;
	if(!i)
		return MENU_SHOP_WEAPON1;
	if(i == 1)
		return MENU_SHOP_AMMO_1;
	return MENU_SHOP_ACCOUNT;
}

int GetChromeSubActive(int page) {
	if(page == MENU_LOAD)
		return 0;
	if(page == MENU_LOAD_INVENTORY)
		return 1;
	if(page == MENU_LOAD_STASH)
		return 2;
	if(page >= MENU_FIRST_TRADE && page <= MENU_LAST_TRADE)
		return 7;
	if(page == MENU_LOAD_CHARM)
		return 3;
	if(page == MENU_LOAD_WEAPONDISCARD)
		return 4;
	if(IsChromeCraftPage(page))
		return 5;
	if(page >= MENU_FIRST_ACCESSORY_PAGE && page <= MENU_LAST_ACCESSORY_PAGE)
		return 6;
	if(page == MENU_SPELL)
		return 0;
	if(page == MENU_SPELL_HOTBAR)
		return 1;
	if(page == MENU_SPELL_GEMS)
		return 2;
	if(IsSpellTreePage(page))
		return 3 + page - MENU_SPELLTREE_FIRST;
	int k = GetChromeKind(page);
	if(k == MCH_KIND_SLOTLIST || k == MCH_KIND_WEAPONS)
		return 0;
	if(k == MCH_KIND_AMMOSELECT || k == MCH_KIND_AMMO)
		return 1;
	if(k == MCH_KIND_ACCOUNT)
		return 2;
	if(k == MCH_KIND_OVERVIEW)
		return 0;
	if(k == MCH_KIND_STATINDEX || k == MCH_KIND_STATS)
		return 1;
	if(k == MCH_KIND_PERKINDEX || k == MCH_KIND_PERKTREE)
		return 2;
	switch(page) {
		case MENU_RESEARCH:
		return 0;
		case MENU_RESEARCH_BODY:
		return 1;
		case MENU_RESEARCH_AMMO:
		return 2;
		case MENU_RESEARCH_GUNS:
		case MENU_RESEARCH_SLOTGUNS:
		case MENU_RESEARCH_LUXURYGUNS:
		return 3;
		case MENU_RESEARCH_UTILITY:
		return 4;
	}
	return -1;
}

int GetChromeChipCount(int page) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_SLOTLIST || k == MCH_KIND_WEAPONS || k == MCH_KIND_PERKINDEX || k == MCH_KIND_PERKTREE)
		return 8;
	if(k == MCH_KIND_AMMOSELECT || k == MCH_KIND_AMMO || k == MCH_KIND_STATINDEX || k == MCH_KIND_STATS)
		return 5;
	if(page == MENU_RESEARCH_GUNS || page == MENU_RESEARCH_SLOTGUNS || page == MENU_RESEARCH_LUXURYGUNS)
		return 2;
	if(MchIsMonsterModPage(page))
		return 7;
	if(k == MCH_KIND_CRAFT)
		return 3;
	return 0;
}

bool MchIsMonsterModPage(int page) {
	return page == MENU_HELP_MMODS || (page >= MENU_MMOD_HELP_BEGIN && page <= MENU_MMOD_HELP_END);
}

// stat category and perk archetype chips carry words, the rest digits
bool ChromeChipsAreWords(int page) {
	int k = GetChromeKind(page);
	return k == MCH_KIND_STATINDEX || k == MCH_KIND_STATS || k == MCH_KIND_PERKINDEX || k == MCH_KIND_PERKTREE || k == MCH_KIND_RESGUNS || k == MCH_KIND_RESLIST || MchIsMonsterModPage(page) || k == MCH_KIND_CRAFT;
}

int GetChromeChipW(int page) {
	if(ChromeChipsAreWords(page))
		return 130;
	return MCH_CHIP_W;
}

str GetChromeWordChipLabel(int page, int i) {
	if(MchIsMonsterModPage(page))
		return StrParam(s:"DND_MCH_MMOD", d:i + 1);
	if(GetChromeKind(page) == MCH_KIND_CRAFT)
		return StrParam(s:"DND_MCH_CR_CHIP", d:i);
	if(GetChromeKind(page) >= MCH_KIND_RESINDEX)
		return i ? "DND_MCH_RES_LUX" : "DND_MCH_RES_SLOT";
	if(GetChromeKind(page) >= MCH_KIND_PERKINDEX)
		return GetPerkArchLangKey(i);
	return GetChromeStatChipLabel(i);
}

str GetChromeStatChipLabel(int i) {
	switch(i) {
		case 0:
		return "DND_MCH_STAT_OFF";
		case 1:
		return "DND_MCH_STAT_DEF";
		case 2:
		return "DND_MCH_STAT_UTIL";
		case 3:
		return "DND_MCH_STAT_EXO";
	}
	return "DND_MCH_STAT_MISC";
}

int GetChromeChipTarget(int page, int i) {
	int k = GetChromeKind(page);
	if(i < 0 || i >= GetChromeChipCount(page))
		return -1;
	if(k == MCH_KIND_SLOTLIST || k == MCH_KIND_WEAPONS)
		return GetWeaponPage(MBOX_1 + i);
	if(k == MCH_KIND_STATINDEX || k == MCH_KIND_STATS) {
		if(!i)
			return MENU_STAT2_OFFENSE1;
		return MENU_STAT2_DEFENSE + i - 1;
	}
	if(k == MCH_KIND_RESGUNS || k == MCH_KIND_RESLIST)
		return i ? MENU_RESEARCH_LUXURYGUNS : MENU_RESEARCH_SLOTGUNS;
	if(MchIsMonsterModPage(page))
		return MENU_MMOD_HELP_BEGIN + i;
	if(k == MCH_KIND_CRAFT)
		return MENU_LOAD_CRAFTING_FIRST + i;
	// an archetype with nothing authored has no page to open
	if(k == MCH_KIND_PERKINDEX || k == MCH_KIND_PERKTREE) {
		EnsurePerkArchLists();
		if(!GetPerkArchList(i).count)
			return -1;
		return MENU_PERKTREE_FIRST + i;
	}
	switch(i) {
		case 0:
		return MENU_SHOP_AMMO_1;
		case 1:
		return MENU_SHOP_AMMO_2;
		case 2:
		return MENU_SHOP_AMMO_3;
		case 3:
		return MENU_SHOP_AMMO_4_1;
	}
	return MENU_SHOP_AMMO_SPECIAL1;
}

int GetChromeChipActive(int page) {
	if(IsPerkTreePage(page))
		return page - MENU_PERKTREE_FIRST;
	if(page == MENU_RESEARCH_SLOTGUNS)
		return 0;
	if(page == MENU_RESEARCH_LUXURYGUNS)
		return 1;
	if(page >= MENU_MMOD_HELP_BEGIN && page <= MENU_MMOD_HELP_END)
		return page - MENU_MMOD_HELP_BEGIN;
	if(IsChromeCraftPage(page))
		return page - MENU_LOAD_CRAFTING_FIRST;
	switch(page) {
		case MENU_SHOP_WEAPON1:
		case MENU_SHOP_AMMO_1:
		case MENU_SHOP_AMMO_1_2:
		case MENU_STAT2_OFFENSE1:
		case MENU_STAT2_OFFENSE2:
		return 0;
		case MENU_STAT2_DEFENSE:
		return 1;
		case MENU_STAT2_UTILITY:
		return 2;
		case MENU_STAT2_EXOTIC:
		return 3;
		case MENU_STAT2_MISC:
		return 4;
		case MENU_SHOP_WEAPON2:
		case MENU_SHOP_AMMO_2:
		case MENU_SHOP_AMMO_2_2:
		return 1;
		case MENU_SHOP_WEAPON3_1:
		case MENU_SHOP_WEAPON3_2:
		case MENU_SHOP_WEAPON3_3:
		case MENU_SHOP_AMMO_3:
		return 2;
		case MENU_SHOP_WEAPON4_1:
		case MENU_SHOP_WEAPON4_2:
		case MENU_SHOP_AMMO_4_1:
		case MENU_SHOP_AMMO_4_2:
		return 3;
		case MENU_SHOP_WEAPON5_1:
		case MENU_SHOP_WEAPON5_2:
		case MENU_SHOP_AMMO_SPECIAL1:
		return 4;
		case MENU_SHOP_WEAPON6_1:
		case MENU_SHOP_WEAPON6_2:
		return 5;
		case MENU_SHOP_WEAPON7:
		return 6;
		case MENU_SHOP_WEAPON8:
		return 7;
	}
	return -1;
}

// page number (1 based) and page count of a multi page weapon slot; 1 of 1 otherwise
int GetChromePageNumber(int page) {
	if(page >= MENU_FIRST_TRADE && page <= MENU_LAST_TRADE)
		return page - MENU_FIRST_TRADE + 1;
	if(page >= MENU_FIRST_ACCESSORY_PAGE && page <= MENU_LAST_ACCESSORY_PAGE)
		return page - MENU_FIRST_ACCESSORY_PAGE + 1;
	if(page == MENU_STAT2_OFFENSE2)
		return 2;
	return 1;
}

int GetChromePageCount(int page) {
	if(page >= MENU_FIRST_TRADE && page <= MENU_LAST_TRADE)
		return Clamp_Between((PlayerCount() - 2) / MENU_TRADE_PLAYERCOUNT + 1, 1, MENU_TRADE_PAGECOUNT);
	if(page >= MENU_FIRST_ACCESSORY_PAGE && page <= MENU_LAST_ACCESSORY_PAGE)
		return MENU_LAST_ACCESSORY_PAGE - MENU_FIRST_ACCESSORY_PAGE + 1;
	if(page == MENU_STAT2_OFFENSE1 || page == MENU_STAT2_OFFENSE2)
		return 2;
	return 1;
}

// Weapon pages page with boxes the server already numbers (next, then previous); ammo pages page through chrome nav.
bool ChromePagerIsBoxes(int page) {
	return GetChromeKind(page) == MCH_KIND_WEAPONS || GetChromeKind(page) == MCH_KIND_GEARACC;
}

int GetChromePagerTarget(int page, int dir) {
	if(GetChromePageCount(page) < 2)
		return -1;
	int n = GetChromePageNumber(page);
	if(dir < 0 && n > 1)
		return page - 1;
	if(dir > 0 && n < GetChromePageCount(page))
		return page + 1;
	return -1;
}

int GetChromePagerBoxes(int page) {
	if(!ChromePagerIsBoxes(page))
		return 0;
	return (GetChromePagerTarget(page, 1) != -1) + (GetChromePagerTarget(page, -1) != -1);
}

// Decodes a chrome click on the server against the page it is on.
int GetChromeElementTarget(int page, int code) {
	if(code >= MCH_EL_TAB && code < MCH_EL_TAB + MCH_SEC_COUNT)
		return GetChromeTabTarget(code - MCH_EL_TAB);
	if(code >= MCH_EL_SUB && code < MCH_EL_CHIP)
		return GetChromeSubTarget(GetChromeSection(page), code - MCH_EL_SUB);
	if(code >= MCH_EL_CHIP && code < MCH_EL_PAGER)
		return GetChromeChipTarget(page, code - MCH_EL_CHIP);
	if(code >= MCH_EL_ROW && code < MCH_EL_ROW + MCH_CODEX_TOPICS && GetChromeKind(page) == MCH_KIND_CODEX)
		return MENU_HELP_BEGIN + code - MCH_EL_ROW;
	if(code == MCH_EL_PAGER && !ChromePagerIsBoxes(page))
		return GetChromePagerTarget(page, -1);
	if(code == MCH_EL_PAGER + 1 && !ChromePagerIsBoxes(page))
		return GetChromePagerTarget(page, 1);
	return -1;
}

void HandleChromeNav(int pnum, int page, int code) {
	if(code == MCH_EL_CLOSE) {
		ACS_ExecuteWithResult(900, 0, 0);
		return;
	}
	if(page == MENU_LOAD_CRAFTING_TRANSMUTING) {
		if(code >= MCH_EL_CRFILL && code < MCH_EL_CRFILL + MAX_INVENTORY_BOXES) {
			MchTransmuteFill(pnum, code - MCH_EL_CRFILL);
			return;
		}
		if(code >= MCH_EL_CRRECIPE && code < MCH_EL_CRRECIPE + DND_MAX_ORB_KINDS) {
			MchTransmuteRecipe(pnum, code - MCH_EL_CRRECIPE);
			return;
		}
		if(code == MCH_EL_CREMPTY) {
			ResetTransmutingData(pnum);
			LocalAmbientSound("RPG/MenuChoose", 127);
			return;
		}
	}
	int target = GetChromeElementTarget(page, code);
	if(target <= 0)
		return;
	if(target == MENU_LOAD_CRAFTING_TRANSMUTING && CheckResearchStatus(RES_TRANSMUTING) != RES_DONE) {
		ShowPopup(POPUP_NEEDRESEARCH, false, 0);
		return;
	}
	// the same side effects the gear index rows have, so a tab lands on the page in the state its row leaves it
	SetInventory("DnD_SelectedInventoryBox", 0);
	SetInventory("DnD_UsedTwoItemRequirementMaterial", 0);
	if(target == MENU_LOAD_CRAFTING_TRANSMUTING)
		ResetTransmutingData(pnum);
	if(target == MENU_LOAD_INVENTORY)
		GiveInventory("DnD_InventoryView", 1);
	else if(target == MENU_LOAD_STASH)
		GiveInventory("DnD_StashView", 1);
	else if(target == MENU_LOAD_DUNGEONS)
		ResetDungeonSelectData(pnum);
	UpdateMenuPosition(target);
}

// ---------------------------------------------------------------- geometry

// The old equipment page's layout (480x320 units) at 2.4x, the charm and armor groups centred as a pair.
#define MCH_EQ_CHARMX 327
#define MCH_EQ_ARMORX 872
#define MCH_EQ_Y 322
#define MCH_EQ_BTNX 1370
#define MCH_EQ_BTNY 868
// the dungeon key socket, centred in the list column, Activate under it
#define MCH_DUN_X 154
#define MCH_DUN_Y 380

int MchEqX(int x) {
	if(x < 290)
		return MCH_EQ_CHARMX + (x - 188) * 12 / 5;
	return MCH_EQ_ARMORX + (x - 296) * 12 / 5;
}

int MchEqY(int y) {
	return MCH_EQ_Y + (y - 38) * 12 / 5;
}

// where the old page centres equipped slot i, in its own units
int MchEqSlotX(int i) {
	switch(i) {
		case SMALLCHARM_INDEX1:
		case SMALLCHARM_INDEX3:
		case MEDIUMCHARM_INDEX1:
		return 208;
		case SMALLCHARM_INDEX2:
		case SMALLCHARM_INDEX4:
		case MEDIUMCHARM_INDEX2:
		return 264;
		case LARGECHARM_INDEX:
		return 236;
		case POWERCORE_INDEX:
		return 338;
		case HELM_INDEX:
		case BODY_ARMOR_INDEX:
		case BOOT_INDEX:
		return 394;
	}
	return ((i - FLASK1_INDEX) & 1) ? 348 : 312;
}

int MchEqSlotY(int i) {
	switch(i) {
		case SMALLCHARM_INDEX1:
		case SMALLCHARM_INDEX2:
		return 208;
		case SMALLCHARM_INDEX3:
		case SMALLCHARM_INDEX4:
		return 256;
		case MEDIUMCHARM_INDEX1:
		case MEDIUMCHARM_INDEX2:
		return 154;
		case POWERCORE_INDEX:
		case BODY_ARMOR_INDEX:
		return 152;
		case LARGECHARM_INDEX:
		return 80;
		case HELM_INDEX:
		return 88;
		case BOOT_INDEX:
		return 216;
	}
	return ((i - FLASK1_INDEX) & 2) ? 247 : 207;
}

str MchEqWell(int i) {
	if(i <= SMALLCHARM_INDEX4)
		return "MCEQS";
	if(i <= MEDIUMCHARM_INDEX2)
		return "MCEQM";
	if(i == LARGECHARM_INDEX)
		return "MCEQG";
	if(i <= BOOT_INDEX)
		return "MCEQA";
	return "MCEQF";
}

int MchEqWellW(int i) {
	if(i >= FLASK1_INDEX)
		return 76;
	return (i >= HELM_INDEX) ? 120 : 96;
}

int MchEqWellH(int i) {
	if(i == LARGECHARM_INDEX)
		return 202;
	if(i == MEDIUMCHARM_INDEX1 || i == MEDIUMCHARM_INDEX2)
		return 134;
	return MchEqWellW(i);
}

// each slot of a kind has its own worn well (variant 0 is the plain name), so neighbours never repeat
str MchWellPic(str base, int slot, bool hover) {
	int v = slot - SMALLCHARM_INDEX1;
	if(slot >= FLASK1_INDEX)
		v = slot - FLASK1_INDEX;
	else if(slot >= HELM_INDEX)
		v = slot - HELM_INDEX;
	else if(slot == LARGECHARM_INDEX)
		v = 0;
	else if(slot >= MEDIUMCHARM_INDEX1)
		v = slot - MEDIUMCHARM_INDEX1;
	if(v)
		return StrParam(s:base, d:v, s:hover ? "H" : "");
	return StrParam(s:base, s:hover ? "H" : "");
}

int MchCursorX() {
	return FixedMul(HUDMAX_XF - (PlayerCursorData.posx & MMASK), MCH_SCALE_X) >> 16;
}

int MchCursorY() {
	return FixedMul(HUDMAX_YF - (PlayerCursorData.posy & MMASK), MCH_SCALE_Y) >> 16;
}

bool MchIn(int cx, int cy, int x, int y, int w, int h) {
	return cx >= x && cx < x + w && cy >= y && cy < y + h;
}

// a canvas rect as a pane box: the cursor runs mirrored in the 480x320 design space
void MchAddBox(menu_pane_T module& p, int x, int y, int w, int h) {
	AddBoxToPane_Points(
		p,
		HUDMAX_XF - FixedDiv(x << 16, MCH_SCALE_X), HUDMAX_YF - FixedDiv(y << 16, MCH_SCALE_Y),
		HUDMAX_XF - FixedDiv((x + w) << 16, MCH_SCALE_X), HUDMAX_YF - FixedDiv((y + h) << 16, MCH_SCALE_Y)
	);
}

int MchPagerX(int dir) {
	if(dir < 0)
		return MCH_ROW_X;
	return MCH_ROW_X + MCH_ROW_W - MCH_KEY_W;
}

// The old row's boxes this page shows as rows; the rest are navigation the chrome now offers itself.
int GetChromeRowCount(int page, int count) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_OVERVIEW)
		return 3;
	if(k == MCH_KIND_STATINDEX)
		return 5;
	if(k == MCH_KIND_STATS || k == MCH_KIND_PERKTREE || IsChromeShopList(page))
		return 0;
	return count - GetChromePagerBoxes(page);
}

// the dash toggle sits apart from the archetype rows above it
#define MCH_PERK_TOGGLEGAP 24

int GetChromeRowY(int page, int i) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_OVERVIEW)
		return 560 + MCH_ROW_STEP * i;
	if(k == MCH_KIND_PERKINDEX && i >= PERK_ARCH_COUNT)
		return MCH_ROW_Y + MCH_ROW_STEP * i + MCH_PERK_TOGGLEGAP;
	if(k == MCH_KIND_GEARDISCARD && i >= 8)
		return MCH_ROW_Y + MCH_ROW_STEP * i + MCH_PERK_TOGGLEGAP;
	if(k == MCH_KIND_RESINDEX && i >= 4)
		return MCH_ROW_Y + MCH_ROW_STEP * i + MCH_PERK_TOGGLEGAP;
	if(k == MCH_KIND_SKILLINDEX && i >= MAX_SKILL_TREES)
		return MCH_ROW_Y + MCH_ROW_STEP * i + MCH_PERK_TOGGLEGAP;
	if(k == MCH_KIND_CODEXINDEX && i >= MCH_CODEX_TOPICS)
		return MCH_CODEX_OPTY + MCH_ROW_STEP * (i - MCH_CODEX_TOPICS);
	if(page == MENU_HELP_MMODS)
		return MCH_CODEX_CATY + MCH_ROW_STEP * i;
	return MCH_ROW_Y + MCH_ROW_STEP * i;
}

int GetChromeRowX(int page, int i) {
	if((GetChromeKind(page) == MCH_KIND_CODEXINDEX && i >= MCH_CODEX_TOPICS) || page == MENU_HELP_MMODS)
		return MCH_PANE_IX;
	return MCH_ROW_X;
}

// Same box count and order as the old row of this page: the rows, then next, then previous, then the ones
// parked where the cursor cannot reach, so the numbering the server knows never shifts.
void LoadChromePane(menu_pane_T module& p, int page) {
	int count = p.cursize, rows = GetChromeRowCount(page, count), i;
	p.cursize = 0;
	// a research entry's two boxes are the pane's Research and Invest buttons
	if(GetChromeKind(page) == MCH_KIND_RESLIST) {
		MchAddBox(p, MCH_RES_BTNX, MCH_RES_BTNY, MCH_TAB_W, MCH_TAB_H);
		MchAddBox(p, MCH_RES_BTNX + MCH_RES_BTNSTEP, MCH_RES_BTNY, MCH_TAB_W, MCH_TAB_H);
		return;
	}
	if(GetChromeKind(page) == MCH_KIND_GEAREQUIP) {
		for(i = 0; i < INV_ICON_INDEX; ++i)
			MchAddBox(p, MchEqX(MchEqSlotX(i)) - MchEqWellW(i) / 2, MchEqY(MchEqSlotY(i)) - MchEqWellH(i) / 2, MchEqWellW(i), MchEqWellH(i));
		MchAddBox(p, MCH_EQ_BTNX, MCH_EQ_BTNY, MCH_TAB_W, MCH_TAB_H);
		rows = 0;
	}
	// the inventory page's boxes are its grid, which the view pane owns
	// the key socket, then Activate; the old back arrow stays parked, the frame's keys go back
	if(GetChromeKind(page) == MCH_KIND_DUNGEONS) {
		MchAddBox(p, MCH_DUN_X, MCH_DUN_Y, 120, 120);
		MchAddBox(p, MCH_DUN_X + 60 - MCH_TAB_W / 2, MCH_DUN_Y + 150, MCH_TAB_W, MCH_TAB_H);
		rows = 0;
	}
	// these pages' boxes are their grids, which the view pane owns, or rows built as they draw
	if(GetChromeKind(page) == MCH_KIND_GEARINV || GetChromeKind(page) == MCH_KIND_GEARSTASH || GetChromeKind(page) == MCH_KIND_GEARTRADE || GetChromeKind(page) == MCH_KIND_CRAFT)
		rows = 0;
	if(GetChromeKind(page) == MCH_KIND_GEARINDEX) {
		MchAddBox(p, -5000, -5000, 1, 1);
		for(i = 1; i < rows; ++i)
			MchAddBox(p, MCH_ROW_X, GetChromeRowY(page, i - 1), MCH_ROW_W, MCH_ROW_H);
		rows = 0;
	}
	for(i = 0; i < rows; ++i)
		MchAddBox(p, GetChromeRowX(page, i), GetChromeRowY(page, i), MCH_ROW_W, MCH_ROW_H);
	if(ChromePagerIsBoxes(page)) {
		if(GetChromePagerTarget(page, 1) != -1)
			MchAddBox(p, MchPagerX(1), MCH_PAGER_Y, MCH_KEY_W, MCH_TAB_H);
		if(GetChromePagerTarget(page, -1) != -1)
			MchAddBox(p, MchPagerX(-1), MCH_PAGER_Y, MCH_KEY_W, MCH_TAB_H);
	}
	for(i = p.cursize; i < count; ++i)
		MchAddBox(p, -5000, -5000, 1, 1);
}

// What the cursor is over among the chrome's own controls, as a mainboxid.
int GetTriggeredChromeBox(int page) {
	int cx = MchCursorX(), cy = MchCursorY(), i;
	if(cy >= MCH_TAB_Y && cy < MCH_TAB_Y + MCH_TAB_H) {
		for(i = 0; i < MCH_SEC_COUNT; ++i)
			if(MchIn(cx, cy, MCH_TAB_X + MCH_TAB_STEP * i, MCH_TAB_Y, MCH_TAB_W, MCH_TAB_H))
				return MCH_NAV_BASE + MCH_EL_TAB + i;
		for(i = 0; i < 3; ++i)
			if(MchIn(cx, cy, MCH_KEY_X + MCH_KEY_STEP * i, MCH_TAB_Y, MCH_KEY_W, MCH_TAB_H))
				return MAINBOX_LARR + i;
		if(MchIn(cx, cy, MCH_CLOSE_X, MCH_TAB_Y, MCH_CLOSE_W, MCH_TAB_H))
			return MCH_NAV_BASE + MCH_EL_CLOSE;
		return MAINBOX_NONE;
	}
	int n = GetChromeSubCount(GetChromeSection(page)), sw = GetChromeSubW(GetChromeSection(page));
	for(i = 0; i < n; ++i)
		if(MchIn(cx, cy, MCH_SUB_X + (sw + 10) * i, MCH_SUB_Y, sw, MCH_SUB_H))
			return MCH_NAV_BASE + MCH_EL_SUB + i;
	n = GetChromeChipCount(page);
	int cw = GetChromeChipW(page);
	for(i = 0; i < n; ++i)
		if(MchIn(cx, cy, MCH_CHIP_X + (cw + 10) * i, MCH_CHIP_Y, cw, MCH_CHIP_H))
			return MCH_NAV_BASE + MCH_EL_CHIP + i;
	if(GetChromeKind(page) == MCH_KIND_RESLIST) {
		i = MchResRowAt(page, cx, cy);
		if(i != -1)
			return MCH_NAV_BASE + MCH_EL_ROW + i;
	}
	if(GetChromeKind(page) == MCH_KIND_CODEX)
		for(i = 0; i < MCH_CODEX_TOPICS; ++i)
			if(MchIn(cx, cy, MCH_ROW_X, MCH_ROW_Y + MCH_ROW_STEP * i, MCH_ROW_W, MCH_ROW_H))
				return MCH_NAV_BASE + MCH_EL_ROW + i;
	if(!ChromePagerIsBoxes(page) && cy >= MCH_PAGER_Y && cy < MCH_PAGER_Y + MCH_TAB_H) {
		if(GetChromePagerTarget(page, -1) != -1 && MchIn(cx, cy, MchPagerX(-1), MCH_PAGER_Y, MCH_KEY_W, MCH_TAB_H))
			return MCH_NAV_BASE + MCH_EL_PAGER;
		if(GetChromePagerTarget(page, 1) != -1 && MchIn(cx, cy, MchPagerX(1), MCH_PAGER_Y, MCH_KEY_W, MCH_TAB_H))
			return MCH_NAV_BASE + MCH_EL_PAGER + 1;
	}
	return MAINBOX_NONE;
}

// ---------------------------------------------------------------- drawing primitives

void MchPic(str pic, int id, int x, int y) {
	SetFont(pic);
	HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_UNTRANSLATED, MenuX((x << 16) + 0.1), MenuY((y << 16) + 0.1), 0.0);
}

// A picture made for a smaller canvas, drawn k times larger and centred on (x, y).
void MchPicScaled(str pic, int id, int x, int y, int k) {
	MenuHudSize(FixedDiv(MCH_W << 16, k) >> 16, FixedDiv(MCH_H << 16, k) >> 16);
	SetFont(pic);
	HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_UNTRANSLATED, MenuX(FixedDiv(x << 16, k) & MMASK), MenuY(FixedDiv(y << 16, k) & MMASK), 0.0);
	MenuHudSize(MCH_W, MCH_H);
}

// align: 0 centred, 1 left middle, 2 right middle, 3 left top
void MchText(str font, str txt, int id, int x, int y, int align) {
	int fx = x << 16, fy = y << 16;
	if(align == 1)
		fx += 0.1;
	else if(align == 2)
		fx += 0.2;
	else if(align == 3) {
		fx += 0.1;
		fy += 0.1;
	}
	SetFont(font);
	HudMessage(s:txt; HUDMSG_PLAIN, id, CR_WHITE, MenuX(fx), MenuY(fy), 0.0);
}

// ---------------------------------------------------------------- scroll bars

// A bar the mouse can drag: the draw records where it is, MchScrollDrag reads presses against that.
#define MCH_SBARS 4
#define MCH_SB_W 8
#define MCH_SB_THUMB 80
#define MCH_SB_PAD 12
#define MCH_SB_POCKET 3

typedef struct {
	int x;
	int y;
	int h;
	int max;
	int pos;
	int grab;
	bool grabbed;
	bool lit;
} mch_sbar_T;

mch_sbar_T module& GetMchSbar(int i) {
	static mch_sbar_T b[MCH_SBARS];
	return b[i];
}

// a page change: no bar is where it was
void MchSbarReset() {
	for(int i = 0; i < MCH_SBARS; ++i) {
		GetMchSbar(i).max = 0;
		GetMchSbar(i).grabbed = false;
		GetMchSbar(i).lit = false;
	}
}

// a chrome page's redraw: its bars register again as they draw
void MchSbarClear() {
	for(int i = 0; i < MCH_SB_POCKET; ++i)
		GetMchSbar(i).max = 0;
}

bool MchSbarGrabbed() {
	for(int i = 0; i < MCH_SBARS; ++i)
		if(GetMchSbar(i).grabbed)
			return true;
	return false;
}

int MchSbarThumbY(int slot) {
	mch_sbar_T module& b = GetMchSbar(slot);
	return b.y + (b.h - MCH_SB_THUMB) * b.pos / b.max;
}

bool MchOnSbar(int slot, int cx, int cy) {
	mch_sbar_T module& b = GetMchSbar(slot);
	return b.max > 0 && cx >= b.x - MCH_SB_PAD && cx < b.x + MCH_SB_W + MCH_SB_PAD && cy >= b.y && cy < b.y + b.h;
}

// pos of max on a track h tall at (x, y); id is the thumb, id + 1 the track
void MchScrollBar(int slot, int id, int x, int y, int h, int pos, int max) {
	mch_sbar_T module& b = GetMchSbar(slot);
	b.x = x;
	b.y = y;
	b.h = h;
	b.max = max;
	if(max <= 0) {
		b.grabbed = false;
		DeleteTextRange(id, id + 1);
		return;
	}
	b.pos = Clamp_Between(pos, 0, max);
	MenuClipRect(x, y, MCH_SB_W, h);
	MchPic("MCSBTRK", id + 1, x, y);
	MchPic(b.lit ? "MCSBTHH" : "MCSBTHM", id, x, MchSbarThumbY(slot));
	MenuClipRect(0, 0, 0, 0, 0);
}

// The bar's position after this tic's mouse: a press on the thumb drags it, a press on the track jumps there.
int MchScrollDrag(int slot, int pos) {
	mch_sbar_T module& b = GetMchSbar(slot);
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS), cy = MchCursorY();
	if(b.max <= 0 || CheckInventory("DnD_SellConfirm") || !IsButtonHeld(bpress, BT_ATTACK)) {
		b.grabbed = false;
		return pos;
	}
	if(!b.grabbed) {
		if(!IsButtonPressed(bpress, GetPlayerInput(-1, INPUT_OLDBUTTONS), BT_ATTACK) || !MchOnSbar(slot, MchCursorX(), cy))
			return pos;
		b.pos = Clamp_Between(pos, 0, b.max);
		int ty = MchSbarThumbY(slot);
		b.grab = (cy >= ty && cy < ty + MCH_SB_THUMB) ? cy - ty : MCH_SB_THUMB / 2;
		b.grabbed = true;
		LocalAmbientSound("RPG/MenuMove", 127);
	}
	int travel = b.h - MCH_SB_THUMB;
	return Clamp_Between(((cy - b.grab - b.y) * b.max + travel / 2) / travel, 0, b.max);
}

// lit while grabbed or under the cursor; true when any bar changed
bool MchSbarHover() {
	bool r = false;
	int cx = MchCursorX(), cy = MchCursorY();
	for(int i = 0; i < MCH_SBARS; ++i) {
		bool lit = GetMchSbar(i).max > 0 && (GetMchSbar(i).grabbed || MchOnSbar(i, cx, cy));
		if(lit != GetMchSbar(i).lit) {
			GetMchSbar(i).lit = lit;
			r = true;
		}
	}
	return r;
}

// a skill page's instructions, on the board's bottom edge
void DrawChromeSpellHint(str lump) {
	MenuHudSize(MCH_W, MCH_H);
	MchText("MENUSUB", StrParam(s:"\c[UIGrey]", l:lump), SPELLTREE_HEAD_ID + 1, 476, MCH_PANE_FOOT + 54, 0);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
	SetFont("NMENUFNT");
}

// the skill pages' info pocket, drawn from their 480x320 hud
void MchSpellPocketBar(int pos, int max) {
	MenuHudSize(MCH_W, MCH_H);
	MchScrollBar(MCH_SB_POCKET, SPELLTREE_BAR_ID, 1588, 307, 648, pos, max);
	DeleteTextRange(SPELLTREE_BAR_ID + 2, SPELLTREE_BAR_ID + 3);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

// the pocket's drag onto the shared bar's position, which counts down from 0
bool MchSpellPocketDrag() {
	bool r = MchSbarHover();
	int p = -GetScrollBarPos(DND_SPELLBAR_PANEL), v = MchScrollDrag(MCH_SB_POCKET, p);
	if(v != p) {
		SetScrollBarPos(DND_SPELLBAR_PANEL, -v);
		r = true;
	}
	return r;
}

// A stack count short enough for a well corner: 999, 1.2k, 12k.
str MchStackText(int v) {
	if(v < 1000)
		return StrParam(d:v);
	if(v < 10000 && (v / 100) % 10)
		return StrParam(d:v / 1000, s:".", d:(v / 100) % 10, s:"k");
	return StrParam(d:v / 1000, s:"k");
}

str MchMoney(int v) {
	str out = "";
	bool neg = v < 0;
	if(neg)
		v = -v;
	do {
		int part = v % 1000;
		v /= 1000;
		if(v)
			out = StrParam(s:",", s:(part < 100) ? "0" : "", s:(part < 10) ? "0" : "", d:part, s:out);
		else
			out = StrParam(d:part, s:out);
	} while(v);
	return StrParam(s:neg ? "-$" : "$", s:out);
}

// ---------------------------------------------------------------- the frame, tabs and status strip

void DrawChromeFrame(int pnum, int page, int mainboxid) {
	int i, x, n, sec = GetChromeSection(page);
	str st, col;
	MenuHudSize(MCH_W, MCH_H);
	MchPic("MCFRAME", RPGMENUBACKGROUNDID, MCH_FX - 360, MCH_FY - 10);

	for(i = 0; i < MCH_SEC_COUNT; ++i) {
		x = MCH_TAB_X + MCH_TAB_STEP * i;
		st = "O";
		col = "\c[UIGrey]";
		if(i == sec) {
			st = "A";
			col = "\c[UIGold]";
		}
		else if(mainboxid == MCH_NAV_BASE + MCH_EL_TAB + i) {
			st = "H";
			col = "\c[UIWhite]";
		}
		MchPic(StrParam(s:"MCT", s:st, d:i % 4), MCH_ID_TABIMG + i, x, MCH_TAB_Y);
		MchText("MENUTAB", StrParam(s:col, l:GetChromeTabLabel(i)), MCH_ID_TABTXT + i, x + MCH_TAB_W / 2, MCH_TAB_Y + 26, 0);
	}

	// unspent points light up their tab
	for(i = 0; i < 2; ++i) {
		if(!i)
			n = CheckInventory("AttributePoint") + CheckInventory("PerkPoint");
		else
			n = CheckInventory("SpellPoint");
		x = MCH_TAB_X + MCH_TAB_STEP * (i ? MCH_SEC_SKILLS : MCH_SEC_CHARACTER) + MCH_TAB_W - 26;
		if(n) {
			MchPic("MCBADGE", MCH_ID_BADGEIMG + i, x, MCH_TAB_Y - 8);
			MchText("MENUSUB", StrParam(s:"\c[UIInk]", d:n), MCH_ID_TABTXT + 7 + i, x + 17, MCH_TAB_Y + 4, 0);
		}
		else {
			DeleteText(MCH_ID_BADGEIMG + i);
			DeleteText(MCH_ID_TABTXT + 7 + i);
		}
	}

	// history keys share the old buttons' ids, so the server and their click animation are unchanged
	for(i = 0; i < 3; ++i) {
		x = MCH_KEY_X + MCH_KEY_STEP * i;
		st = "O";
		if(CheckInventory(ButtonTimers[i]))
			st = "P";
		else if(mainboxid == MAINBOX_LARR + i)
			st = "H";
		MchPic(StrParam(s:"MCK", s:st, s:"0"), MCH_ID_TABIMG + 10 + i, x, MCH_TAB_Y);
		if(!i)
			col = "MCIBK";
		else if(i == 1)
			col = "MCIHM";
		else
			col = "MCIFW";
		MchPic(StrParam(s:col, d:mainboxid == MAINBOX_LARR + i), MCH_ID_TABIMG + 7 + i, x + 13, MCH_TAB_Y + 14);
	}
	bool hot = mainboxid == MCH_NAV_BASE + MCH_EL_CLOSE;
	MchPic(hot ? "MCWH0" : "MCWO0", MCH_ID_TABIMG + 13, MCH_CLOSE_X, MCH_TAB_Y);
	MchText("MENUSUB", StrParam(s:hot ? "\c[UIWhite]" : "\c[UIGrey]", l:"DND_MCH_CLOSE"), MCH_ID_TABTXT + 9, MCH_CLOSE_X + MCH_CLOSE_W / 2, MCH_TAB_Y + 26, 0);

	// sub tabs of this section
	DeleteTextRange(MCH_ID_SUBTXT, MCH_ID_SUBTXT + MCH_MAX_SUBS - 1);
	DeleteTextRange(MCH_ID_SUBIMG, MCH_ID_SUBIMG + MCH_MAX_SUBS - 1);
	n = GetChromeSubCount(sec);
	int act = GetChromeSubActive(page);
	int sw = GetChromeSubW(sec);
	for(i = 0; i < n; ++i) {
		x = MCH_SUB_X + (sw + 10) * i;
		st = "O";
		col = "\c[UIGrey]";
		if(i == act) {
			st = "A";
			col = "\c[UIGold]";
		}
		else if(mainboxid == MCH_NAV_BASE + MCH_EL_SUB + i) {
			st = "H";
			col = "\c[UIWhite]";
		}
		MchPic(StrParam(s:(sw == MCH_SUB_W) ? "MCS" : "MCN", s:st, d:(i + 1) % 4), MCH_ID_SUBIMG + i, x, MCH_SUB_Y);
		MchText("MENUSUB", StrParam(s:col, l:GetChromeSubLabel(sec, i)), MCH_ID_SUBTXT + i, x + sw / 2, MCH_SUB_Y + 16, 0);
	}

	DrawChromeStatus(pnum, page);
}

void DrawChromeStatus(int pnum, int page) {
	int lvl = CheckInventory("Level");
	str extra = "";
	if(GetChromeSection(page) == MCH_SEC_SKILLS)
		extra = StrParam(s:"\c[UIGrey]", l:"DND_MCH_SKILLPTS", s:"  ", s:GetSpellPoints(pnum) ? "\c[UIGold]" : "\c[UIWhite]", d:GetSpellPoints(pnum), s:"      ");
	MchText("MENUTAB", StrParam(s:"\c[UIWhite]", l:"DND_MCH_LV", s:" ", d:lvl, s:"    \c[UIGreen]",
		l:GetClassLabel(StrParam(s:"CLASS", d:CheckInventory("DnD_Character") - 1), DND_CLASS_LABEL_NAME)), MCH_ID_STATTXT, MCH_FX + 54, MCH_STATUS_Y, 1);

	int pct = 0;
	if(lvl > 0 && lvl <= MAXLEVELS)
		pct = Clamp_Between(GetPlayerExp(pnum) / Max(LevelCurve[lvl - 1] / 100, 1), 0, 100);
	MchPic("MCXPBG", MCH_ID_PAGEIMG + 7, MCH_FX + 330, MCH_STATUS_Y - 8);
	if(pct) {
		MenuClipRect(MCH_FX + 333, MCH_STATUS_Y - 4, 414 * pct / 100, 8);
		MchPic("MCXPFL", MCH_ID_PAGEIMG + 6, MCH_FX + 333, MCH_STATUS_Y - 4);
		MenuClipRect(0, 0, 0, 0, 0);
	}
	else
		DeleteText(MCH_ID_PAGEIMG + 6);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", d:pct, s:"%"), MCH_ID_STATTXT + 1, MCH_FX + 766, MCH_STATUS_Y, 1);

	MchText("MENUTAB", StrParam(s:extra, s:"\c[UIGrey]", l:"DND_MENU_CREDITS", s:"  \c[UIGold]", s:MchMoney(GetPlayerCredit(pnum)),
		s:"      \c[UIGrey]", l:"DND_MENU_BUDGET", s:"  \c[UIGold]$", d:CheckInventory("Budget"), s:"K"), MCH_ID_STATTXT + 2, MCH_FX + MCH_FW - 36, MCH_STATUS_Y, 2);
}

// Takes down what the old frame drew outside the page band when a redesigned page takes over.
void ClearOldMenuFrame() {
	DeleteTextRange(RPGMENULISTID - 6, RPGMENULISTID);
	DeleteText(RPGMENUHIGHLIGHTID);
	DeleteText(RPGMENUNAMEID);
	DeleteTextRange(RPGMENULARRID, RPGMENURETARRID);
	DeleteTextRange(RPGMENUWEAPONPANELID - MAX_WEAPON_PROPERTIES, RPGMENUWEAPONPANELID);
	DeleteText(RPGMENUITEMSUBID);
	DeleteText(RPGMENUHELPID);
	ClearInfoPanel();
}

// ---------------------------------------------------------------- shop rows

typedef struct {
	bool known;
	bool owned;
	int price;
	int stock;
	str col;
	str pcol;
} mch_shoprow_T;

mch_shoprow_T module& GetMchRow() {
	static mch_shoprow_T r;
	return r;
}

// The same states and precedence as DrawToggledImage, as colours of the new palette.
void MchFillShopRow(int pnum, int itemid, int flags, str choicename, int choicecount) {
	mch_shoprow_T module& r = GetMchRow();
	r.known = !(flags & OBJ_RESEARCH) || CheckItemRequirements(itemid, RES_KNOWN, flags);
	r.owned = false;
	r.price = 0;
	r.stock = GlobalData.ShopStockRemaining[pnum][itemid];
	r.col = "\c[UIDim]";
	r.pcol = "\c[UIDim]";
	if(!r.known)
		return;

	bool done = CheckItemRequirements(itemid, RES_DONE, flags);
	if(!(flags & OBJ_AMMO))
		r.price = GetShopPrice(itemid);
	else
		r.price = GetShopPrice(itemid, PRICE_INCREASE_STOCK_LOW);
	bool cant = r.price > GetPlayerCredit(pnum) || r.stock <= 0;

	r.col = "\c[UIWhite]";
	r.pcol = "\c[UIGold]";
	if(flags & OBJ_RESEARCH && !done) {
		r.col = "\c[UISlate]";
		r.pcol = "\c[UISlate]";
	}
	else if(!(flags & (OBJ_AMMO | OBJ_ARMOR))) {
		if(!(flags & OBJ_ACCOUNT) && CheckInventory(GetItemName(itemid))) {
			r.owned = true;
			r.col = "\c[UIGreen]";
			r.pcol = "\c[UIGreen]";
		}
		else if(cant) {
			r.col = "\c[UIGrey]";
			r.pcol = "\c[UIGrey]";
		}
		else if(!(HasClassPerk_Fast(DND_PLAYER_BERSERKER, 1) && itemid <= SHOP_WEAPON_SLOT1END) && CheckInventory(choicename) == choicecount) {
			r.col = "\c[UIRed]";
			r.pcol = "\c[UIRed]";
		}
	}
	else if(cant) {
		r.col = "\c[UIGrey]";
		r.pcol = "\c[UIGrey]";
	}
}

typedef struct {
	int slot;
	int first;
	int last;
	bool spec;
} mch_ammopage_T;

mch_ammopage_T module& GetMchAmmoPage(int page) {
	static mch_ammopage_T a;
	a.spec = false;
	switch(page) {
		case MENU_SHOP_AMMO_1:
		case MENU_SHOP_AMMO_1_2:
			a.slot = DND_AMMOSLOT_CLIP;
			a.first = AMMOSLOT_CLIP_BEGIN;
			a.last = AMMOSLOT_CLIP2_END;
		break;
		case MENU_SHOP_AMMO_2:
		case MENU_SHOP_AMMO_2_2:
			a.slot = DND_AMMOSLOT_SHELL;
			a.first = AMMOSLOT_SHELL_BEGIN;
			a.last = AMMOSLOT_SHELL2_END;
		break;
		case MENU_SHOP_AMMO_3:
			a.slot = DND_AMMOSLOT_ROCKET;
			a.first = AMMOSLOT_ROCKET_BEGIN;
			a.last = AMMOSLOT_ROCKET_END;
		break;
		case MENU_SHOP_AMMO_4_1:
		case MENU_SHOP_AMMO_4_2:
			a.slot = DND_AMMOSLOT_CELL;
			a.first = AMMOSLOT_CELL_BEGIN;
			a.last = AMMOSLOT_CELL2_END;
		break;
		default:
			a.slot = -1;
			a.first = 0;
			a.last = MAX_SPECIAL_AMMOS_FOR_SHOP - 1;
			a.spec = true;
		break;
	}
	return a;
}

// ---------------------------------------------------------------- the shop's slot lists

// A weapon or ammo slot is one list on its first page, scrolled rather than paged.
#define MCH_LIST_ROWS 10

bool IsChromeShopList(int page) {
	int k = GetChromeKind(page);
	return k == MCH_KIND_WEAPONS || k == MCH_KIND_AMMO;
}

int MchWeaponSlotFirst(int page) {
	if(page >= MENU_SHOP_WEAPON3_1 && page <= MENU_SHOP_WEAPON3_3)
		return SHOP_WEAPON31_BEGIN;
	if(page == MENU_SHOP_WEAPON4_1 || page == MENU_SHOP_WEAPON4_2)
		return SHOP_WEAPON41_BEGIN;
	if(page == MENU_SHOP_WEAPON5_1 || page == MENU_SHOP_WEAPON5_2)
		return SHOP_WEAPON51_BEGIN;
	if(page == MENU_SHOP_WEAPON6_1 || page == MENU_SHOP_WEAPON6_2)
		return SHOP_WEAPON61_BEGIN;
	return GetWeaponBeginIndexFromOption(page);
}

int MchWeaponSlotLast(int page) {
	if(page >= MENU_SHOP_WEAPON3_1 && page <= MENU_SHOP_WEAPON3_3)
		return SHOP_WEAPON_SLOT3END;
	if(page == MENU_SHOP_WEAPON4_1 || page == MENU_SHOP_WEAPON4_2)
		return SHOP_WEAPON_SLOT42END;
	if(page == MENU_SHOP_WEAPON5_1 || page == MENU_SHOP_WEAPON5_2)
		return SHOP_WEAPON_SLOT52END;
	if(page == MENU_SHOP_WEAPON6_1 || page == MENU_SHOP_WEAPON6_2)
		return SHOP_WEAPON_SLOT62END;
	return GetWeaponEndIndexFromOption(page);
}

int MchShopListCount(int page) {
	if(GetChromeKind(page) == MCH_KIND_WEAPONS)
		return MchWeaponSlotLast(page) - MchWeaponSlotFirst(page) + 1;
	mch_ammopage_T module& a = GetMchAmmoPage(page);
	return a.last - a.first + 1;
}

// the row under the cursor as the box the server knows: the list index + 1
int MchShopListHit(int page) {
	mch_state_T module& s = GetMchState();
	int cx = MchCursorX(), cy = MchCursorY() - MCH_ROW_Y;
	if(s.page != page || cx < MCH_ROW_X || cx >= MCH_ROW_X + MCH_ROW_W || cy < 0 || cy % MCH_ROW_STEP >= MCH_ROW_H)
		return MAINBOX_NONE;
	int i = cy / MCH_ROW_STEP;
	if(i >= MCH_LIST_ROWS || s.scroll + i >= MchShopListCount(page))
		return MAINBOX_NONE;
	return s.scroll + i + 1;
}

// The shop item of row r on a page, -1 for rows that are navigation.
int GetChromeRowItem(int page, int r) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_WEAPONS) {
		if(MchWeaponSlotFirst(page) + r <= MchWeaponSlotLast(page))
			return MchWeaponSlotFirst(page) + r;
	}
	else if(k == MCH_KIND_AMMO) {
		mch_ammopage_T module& a = GetMchAmmoPage(page);
		if(a.spec)
			return SHOP_FIRSTAMMOSPECIAL_INDEX + r;
		return GetMenuAmmoIndexMap(a.slot, a.first + r);
	}
	else if(k == MCH_KIND_ACCOUNT)
		return SHOP_ACCOUNT_BEGIN + r;
	return -1;
}

int GetChromeRowFlags(int page, int item, int r) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_WEAPONS)
		return GetWeaponDrawInfo(item - SHOP_WEAPON_BEGIN).flags;
	if(k == MCH_KIND_AMMO) {
		if(GetMchAmmoPage(page).spec)
			return GetAmmoDrawInfo(r + MAXSHOPNORMALAMMOS).flags;
		return GetAmmoDrawInfo(item - SHOP_FIRSTAMMO_INDEX).flags;
	}
	return OBJ_RESEARCH | OBJ_ACCOUNT;
}

void MchFillRowFor(int pnum, int page, int item, int r) {
	int k = GetChromeKind(page), flags = GetChromeRowFlags(page, item, r);
	if(k == MCH_KIND_WEAPONS)
		MchFillShopRow(pnum, item, flags, GetWeaponCondition(item), 1);
	else if(k == MCH_KIND_AMMO)
		MchFillShopRow(pnum, item, flags, "", 1);
	else
		MchFillShopRow(pnum, item, flags, GetItemName(item), GetShopItemInfo(item, DND_SHOPINFO_MAX));
}

str GetChromeRowName(int page, int item, int r) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_WEAPONS)
		return StrParam(l:GetWeaponTag(ShopTableIdToWeaponTableId(item)));
	if(k == MCH_KIND_AMMO)
		return StrParam(l:GetAmmoTag(item));
	if(k == MCH_KIND_ACCOUNT)
		return StrParam(l:GetAccountPurchaseTag(item - SHOP_ACCOUNT_BEGIN));
	if(k == MCH_KIND_SHOPINDEX) {
		if(!r)
			return StrParam(l:"DND_MCH_SUB_WEAPONS");
		if(r == 1)
			return StrParam(l:"DND_MCH_SUB_AMMO");
		if(r == 2)
			return StrParam(l:"DND_MCH_SUB_ACCOUNT");
		return StrParam(l:"DND_MENU_NFT");
	}
	if(k == MCH_KIND_SLOTLIST)
		return StrParam(l:"DND_MENU_SLOT", s:" ", d:r + 1);
	if(r < MAX_SLOTS - 1)
		return StrParam(l:"DND_MENU_AMMOCATEGORY", s:" ", d:r + 1);
	return StrParam(l:"DND_MENU_SPECIALAMMO");
}

str GetChromeTitle(int page) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_SHOPINDEX)
		return "DND_MENU_HEAD_SHOP";
	if(k == MCH_KIND_SLOTLIST || k == MCH_KIND_WEAPONS)
		return "DND_MENU_HEAD_WEAPONS";
	if(k == MCH_KIND_ACCOUNT)
		return "DND_MENU_HEAD_ACCOUNT";
	if(k == MCH_KIND_OVERVIEW)
		return "DND_MENU_HEAD_STATS";
	if(k == MCH_KIND_PERKINDEX)
		return "DND_MENU_HEAD_PERKS";
	if(k == MCH_KIND_PERKTREE)
		return GetPerkArchLangKey(page - MENU_PERKTREE_FIRST);
	if(k == MCH_KIND_RESINDEX)
		return "DND_MENU_RESEARCH";
	if(k == MCH_KIND_RESGUNS)
		return "DND_MCH_SUB_WEAPONS";
	if(k == MCH_KIND_RESLIST)
		return GetChromeSubLabel(MCH_SEC_RESEARCH, GetChromeSubActive(page));
	if(k == MCH_KIND_CODEXINDEX)
		return "DND_MCH_TAB_CODEX";
	if(k == MCH_KIND_SKILLINDEX)
		return "DND_MCH_TAB_SKILLS";
	if(k == MCH_KIND_GEARINDEX)
		return "DND_MCH_TAB_GEAR";
	if(k == MCH_KIND_GEARDISCARD)
		return "DND_MENU_WEAPONDISCARD";
	if(k == MCH_KIND_GEARACC)
		return "DND_MENU_ACCESSORIES";
	if(k == MCH_KIND_GEAREQUIP)
		return "DND_MENU_EQUIPMENTS";
	if(k == MCH_KIND_GEARINV)
		return "DND_MENU_INVENTORY";
	if(k == MCH_KIND_GEARSTASH)
		return "DND_MENU_STASH";
	if(k == MCH_KIND_GEARTRADE)
		return "DND_MENU_TRADING";
	if(k == MCH_KIND_DUNGEONS)
		return "DND_MENU_DUNGEONS";
	if(k == MCH_KIND_HOME)
		return "DND_MCH_HOME_TITLE";
	if(k == MCH_KIND_CRAFT)
		return "DND_MENU_CRAFTING";
	if(k == MCH_KIND_CODEX)
		return StrParam(s:"DND_MCH_CODEX", d:MchCodexTopic(page) + 1);
	if(k == MCH_KIND_STATINDEX)
		return "DND_MENU_HEAD_GAINS";
	if(k == MCH_KIND_STATS) {
		if(page == MENU_STAT2_OFFENSE1 || page == MENU_STAT2_OFFENSE2)
			return "DND_MENU_GAIN_OFFENSE";
		return StrParam(s:"DND_MENU_GAIN_", s:(page == MENU_STAT2_DEFENSE) ? "DEFENSE" : (page == MENU_STAT2_UTILITY) ? "UTILITY" : (page == MENU_STAT2_EXOTIC) ? "EXOTIC" : "MISC");
	}
	return "DND_MENU_HEAD_AMMO";
}

typedef struct {
	int page;
	int row;
	int scroll;
	int scroll_max;
	int scroll_tic;
	int pscroll;
	int pscroll_max;
	int lastpos;
	bool popanswered;
} mch_state_T;

mch_state_T module& GetMchState() {
	static mch_state_T s;
	return s;
}

// ---------------------------------------------------------------- the page

void DrawChromePage(int pnum, int page, int boxid, int mainboxid, menu_pane_T module& p) {
	int i, x, item, k = GetChromeKind(page), count = p.cursize;
	str st;
	MenuHudSize(MCH_W, MCH_H);
	MchSbarClear();
	// the page's own bands only: the frame's labels and the status strip sit between them
	DeleteTextRange(MCH_ID_ROWTXT, MCH_ID_TABTXT - 1);
	DeleteTextRange(MCH_ID_PANEIMG, MCH_ID_PAGEIMG + 5);

	MchText("MENUHEAD", StrParam(s:"\c[UICyan]", l:GetChromeTitle(page)), MCH_ID_PAGETXT, MCH_ROW_X, 196, 3);

	int n = GetChromeChipCount(page), act = GetChromeChipActive(page), cw = GetChromeChipW(page);
	for(i = 0; i < n; ++i) {
		x = MCH_CHIP_X + (cw + 10) * i;
		st = "N";
		if(i == act)
			st = "S";
		else if(mainboxid == MCH_NAV_BASE + MCH_EL_CHIP + i)
			st = "H";
		if(ChromeChipsAreWords(page)) {
			MchPic(StrParam(s:"MCCW", s:st), MCH_ID_CHIPIMG + i, x - 6, MCH_CHIP_Y - 6);
			MchText("MENUSUB", StrParam(s:(i == act) ? "\c[UIGold]" : (GetChromeChipTarget(page, i) == -1) ? "\c[UIDim]" : "\c[UIGrey]", l:GetChromeWordChipLabel(page, i)),
				MCH_ID_CHIPTXT + i, x + cw / 2, MCH_CHIP_Y + 20, 0);
			continue;
		}
		MchPic(StrParam(s:"MCCHIP", s:st), MCH_ID_CHIPIMG + i, x - 6, MCH_CHIP_Y - 6);
		if(k == MCH_KIND_AMMO || k == MCH_KIND_AMMOSELECT) {
			if(i == 4)
				MchText("MENUSUB", StrParam(s:(i == act) ? "\c[UIGold]" : "\c[UIGrey]", l:"DND_MCH_SPECIALCHIP"), MCH_ID_CHIPTXT + i, x + MCH_CHIP_W / 2, MCH_CHIP_Y + 20, 0);
			else
				MchText("MENUTAB", StrParam(s:(i == act) ? "\c[UIGold]" : "\c[UIGrey]", d:i + 1), MCH_ID_CHIPTXT + i, x + MCH_CHIP_W / 2, MCH_CHIP_Y + 20, 0);
		}
		else
			MchText("MENUTAB", StrParam(s:(i == act) ? "\c[UIGold]" : "\c[UIGrey]", d:i + 1), MCH_ID_CHIPTXT + i, x + MCH_CHIP_W / 2, MCH_CHIP_Y + 20, 0);
	}

	if(k == MCH_KIND_SLOTLIST || k == MCH_KIND_WEAPONS)
		MchText("MENUBDYS", StrParam(s:"\c[UIDim]R  \c[UIGrey]", l:"DND_MENU_REPLACESWEP", s:"      \c[UIDim]L  \c[UIGrey]", l:"DND_MENU_LUXURYWEP"),
			MCH_ID_PAGETXT + 1, MCH_PANE_X + 976, 221, 2);
	MchPic("MCDIV", MCH_ID_PAGEIMG, MCH_ROW_X, 258);
	if(k == MCH_KIND_SKILLINDEX) {
		DrawChromeSkillIndex(pnum, boxid);
		return;
	}
	if(k == MCH_KIND_GEARINDEX) {
		DrawChromeGearIndex(pnum, boxid);
		return;
	}
	if(k == MCH_KIND_GEARDISCARD) {
		DrawChromeGearDiscard(pnum, boxid);
		return;
	}
	if(k == MCH_KIND_GEARACC) {
		DrawChromeGearAcc(pnum, page, boxid, mainboxid, count);
		return;
	}
	if(k == MCH_KIND_GEAREQUIP) {
		DrawChromeGearEquip(pnum, boxid);
		return;
	}
	if(k == MCH_KIND_GEARINV) {
		DrawChromeGearInv(pnum);
		return;
	}
	if(k == MCH_KIND_GEARTRADE) {
		DrawChromeGearTrade(pnum, page, boxid, mainboxid, p);
		return;
	}
	if(k == MCH_KIND_DUNGEONS) {
		DrawChromeDungeons(pnum, boxid);
		return;
	}
	if(k == MCH_KIND_HOME) {
		DrawChromeHome();
		return;
	}
	if(k == MCH_KIND_CRAFT) {
		DrawChromeCraft(pnum, page, boxid);
		return;
	}
	if(k == MCH_KIND_GEARSTASH) {
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_INV"), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);
		return;
	}
	if(k >= MCH_KIND_CODEXINDEX) {
		DrawChromeCodex(pnum, page, boxid, mainboxid);
		return;
	}
	if(k >= MCH_KIND_RESINDEX) {
		DrawChromeResearch(pnum, page, boxid, mainboxid);
		return;
	}
	if(k >= MCH_KIND_PERKINDEX) {
		DrawChromePerks(pnum, page, boxid, p);
		return;
	}
	if(k >= MCH_KIND_OVERVIEW) {
		DrawChromeCharacter(pnum, page, boxid, mainboxid);
		return;
	}

	// the rows, and which one the pane describes: the hovered one, else the last one hovered
	int rows = count - GetChromePagerBoxes(page), first = 0, total = rows;
	bool list = IsChromeShopList(page);
	mch_state_T module& s = GetMchState();
	if(s.page != page) {
		s.page = page;
		s.row = 0;
		s.scroll = 0;
	}
	if(list) {
		total = MchShopListCount(page);
		s.scroll_max = Max(total - MCH_LIST_ROWS, 0);
		s.scroll = Clamp_Between(s.scroll, 0, s.scroll_max);
		first = s.scroll;
		rows = Min(total - first, MCH_LIST_ROWS);
	}
	if(boxid != MAINBOX_NONE && boxid <= total)
		s.row = boxid - 1;

	for(int ri = 0; ri < rows; ++ri) {
		int y = MCH_ROW_Y + MCH_ROW_STEP * ri;
		i = first + ri;
		item = GetChromeRowItem(page, i);
		str name, price = "", col = "\c[UIWhite]", pcol = "\c[UIGold]";
		bool locked = false, tagged = false, lux = false;
		if(item != -1) {
			MchFillRowFor(pnum, page, item, i);
			mch_shoprow_T module& r = GetMchRow();
			if(!r.known) {
				locked = true;
				name = StrParam(l:"DND_MCH_NA");
				price = "$ ???";
			}
			else {
				name = GetChromeRowName(page, item, i);
				price = MchMoney(r.price);
			}
			col = r.col;
			pcol = r.pcol;
			if(k == MCH_KIND_WEAPONS && r.known) {
				tagged = true;
				lux = IsLuxuryWeapon(ShopTableIdToWeaponTableId(item));
			}
		}
		else
			name = GetChromeRowName(page, -1, i);

		if(boxid == i + 1)
			st = "S";
		else if(locked)
			st = "L";
		else
			st = "N";
		MchPic(StrParam(s:"MCROW", s:st), MCH_ID_ROWIMG + 12 + ri, MCH_ROW_X - 8, y - 8);
		x = MCH_ROW_X + 30;
		if(tagged) {
			MchPic("MCTAGBOX", MCH_ID_ROWIMG + ri, MCH_ROW_X + 24, y + 13);
			MchText("MENUSUB", lux ? "\c[UICyan]L" : "\c[UIWhite]R", MCH_ID_ROWTXT + 3 * ri + 2, MCH_ROW_X + 38, y + 27, 0);
			x = MCH_ROW_X + 68;
		}
		MchText("MENUBODY", StrParam(s:col, s:name), MCH_ID_ROWTXT + 3 * ri, x, y + 27, 1);
		if(StrLen(price))
			MchText("MENUBODY", StrParam(s:pcol, s:price), MCH_ID_ROWTXT + 3 * ri + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
	}

	if(list) {
		MchScrollBar(0, MCH_ID_PANEIMG + 26, MCH_ROW_X + MCH_ROW_W + 10, MCH_ROW_Y, MCH_ROW_STEP * MCH_LIST_ROWS - 6, s.scroll, s.scroll_max);
		if(s.scroll_max)
			MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_SHOWING", s:" ", d:first + 1, s:"-", d:first + rows, s:" / ", d:total, s:"    \c[UIDim]", l:"DND_MCH_SCROLLHINT"),
				MCH_ID_PAGETXT + 2, MCH_ROW_X + MCH_ROW_W / 2, MCH_PAGER_Y + 26, 0);
	}
	else
		DrawChromePager(page, boxid, mainboxid, rows);
	DrawChromePane(pnum, page, Clamp_Between(s.row, 0, Max(total - 1, 0)));
}

void DrawChromePager(int page, int boxid, int mainboxid, int rows) {
	int pages = GetChromePageCount(page);
	if(pages < 2)
		return;
	for(int d = -1; d <= 1; d += 2) {
		if(GetChromePagerTarget(page, d) == -1)
			continue;
		bool hot;
		if(ChromePagerIsBoxes(page))
			hot = boxid == rows + 1 + (d < 0 && GetChromePagerTarget(page, 1) != -1);
		else
			hot = mainboxid == MCH_NAV_BASE + MCH_EL_PAGER + (d > 0);
		int x = MchPagerX(d);
		MchPic(StrParam(s:"MCK", s:hot ? "H" : "O", s:"0"), MCH_ID_PAGEIMG + 3 + (d > 0), x, MCH_PAGER_Y);
		MchPic(StrParam(s:(d < 0) ? "MCIBK" : "MCIFW", d:hot), MCH_ID_PAGEIMG + 1 + (d > 0), x + 13, MCH_PAGER_Y + 14);
	}
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_PAGE", s:" ", d:GetChromePageNumber(page), s:" / ", d:pages),
		MCH_ID_PAGETXT + 2, MCH_ROW_X + MCH_ROW_W / 2, MCH_PAGER_Y + 26, 0);
}

// ---------------------------------------------------------------- the detail pane


void DrawChromePane(int pnum, int page, int row) {
	int k = GetChromeKind(page), item = GetChromeRowItem(page, row), y;
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);

	if(item == -1) {
		str head = GetChromeRowName(page, -1, row), body = "";
		if(k == MCH_KIND_SHOPINDEX && row < 3)
			body = StrParam(l:StrParam(s:"DND_MCH_SHOPDESC", d:row + 1));
		else if(k == MCH_KIND_SLOTLIST)
			body = StrParam(l:"DND_MCH_PICKSLOT", s:"\n\n\c[UIGrey]R  ", l:"DND_MENU_REPLACESWEP", s:".\n\c[UICyan]L  \c[UIGrey]", l:"DND_MENU_LUXURYWEP", s:". ", l:"DND_MENU_CARRYLIMIT", s:".");
		else if(k == MCH_KIND_AMMOSELECT)
			body = StrParam(l:"DND_MCH_PICKCAT");
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", s:head), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 500, MCH_PANE_TEXTW);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", s:body), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MenuClipRect(0, 0, 0, 0, 0);
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_OPEN"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
		return;
	}

	MchFillRowFor(pnum, page, item, row);
	mch_shoprow_T module& r = GetMchRow();
	MchPic("MCWELL", MCH_ID_PANEIMG + 29, MCH_PANE_IX, MCH_PANE_Y + 24);
	if(!r.known) {
		MchText("MENUHEAD", StrParam(s:"\c[UIDim]", l:"DND_MCH_NA"), MCH_ID_PANETXT, MCH_PANE_TX, MCH_PANE_Y + 26, 3);
		MchText("MENUHEAD", "\c[UIDim]$ ???", MCH_ID_PANETXT + 5, MCH_PANE_IX, MCH_PANE_FOOT, 3);
		return;
	}

	MchText("MENUHEAD", StrParam(s:r.col, s:GetChromeRowName(page, item, row)), MCH_ID_PANETXT, MCH_PANE_TX, MCH_PANE_Y + 26, 3);
	int desc_y = MCH_PANE_Y + 212;
	str desc = "", hint = "DND_MCH_HINT_ACCOUNT";
	if(k == MCH_KIND_WEAPONS) {
		int real = ShopTableIdToWeaponTableId(item);
		draw_info_T module& info = GetWeaponDrawInfo(item);
		str icon = GetTextWithResearch(GetWeaponShopIcon(item), "", info.res_id, RES_KNOWN, info.flags);
		if(StrLen(icon))
			MchPicScaled(icon, MCH_ID_PANEIMG, MCH_PANE_IX + 130, MCH_PANE_Y + 109, 3.0);
		if(IsLuxuryWeapon(real))
			MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_SLOT", s:" ", d:GetChromeChipActive(page) + 1, s:"  -  \c[UICyan]", l:"DND_MENU_LUXURYWEP",
				s:"\c[UIGrey]. ", l:"DND_MENU_CARRYLIMIT"), MCH_ID_PANETXT + 1, MCH_PANE_TX, MCH_PANE_Y + 78, 3);
		else
			MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_SLOT", s:" ", d:GetChromeChipActive(page) + 1, s:"  -  ", l:"DND_MENU_REPLACESWEP"),
				MCH_ID_PANETXT + 1, MCH_PANE_TX, MCH_PANE_Y + 78, 3);

		int i, j = 0;
		if(CheckItemRequirements(item, RES_KNOWN, info.flags)) {
			int dt = GetShopWeaponDamageTypes(item);
			for(i = 0; i < MAX_DAMAGE_TYPES_MENU; ++i)
				if(IsSet(dt, i)) {
					MchPicScaled(GetDamageTypeIcon(i), MCH_ID_PANEIMG + 1 + j, MCH_PANE_TX + 32 + 70 * j, MCH_PANE_Y + 146, 2.0);
					++j;
				}
			j = 0;
			for(i = 0; i < MAX_WEAPON_PROPERTIES; ++i)
				if(IsSet(Weapons_Data[real].properties, i)) {
					MchPicScaled(WeaponPropertyImages[i], MCH_ID_PANEIMG + 9 + j, MCH_PANE_IX + 52 + 108 * (j % 8), MCH_PANE_Y + 254 + 84 * (j / 8), 1.5);
					++j;
				}
		}
		desc_y = MCH_PANE_Y + 216 + 84 * ((j + 7) / 8);
		desc = GetWeaponExplanation(item);
		hint = "DND_MCH_HINT_WEAPON";
	}
	else if(k == MCH_KIND_AMMO) {
		mch_ammopage_T module& a = GetMchAmmoPage(page);
		str aicon;
		if(a.spec) {
			int sidx = row;
			if(sidx >= SSAM_EXSHELL)
				++sidx;
			aicon = GetTextWithResearch(GetSpecialAmmoStr(sidx, AMMOINFO_ICON), "", GetAmmoDrawInfo(MAXSHOPNORMALAMMOS + row).res_id, RES_KNOWN, GetAmmoDrawInfo(MAXSHOPNORMALAMMOS + row).flags);
		}
		else
			aicon = GetTextWithResearch(GetAmmoInfo(a.slot, a.first + row).icon, "", GetAmmoDrawInfo(item - SHOP_FIRSTAMMO_INDEX).res_id, RES_KNOWN, GetAmmoDrawInfo(item - SHOP_FIRSTAMMO_INDEX).flags);
		if(StrLen(aicon))
			MchPicScaled(aicon, MCH_ID_PANEIMG, MCH_PANE_IX + 130, MCH_PANE_Y + 109, 3.0);
		desc = GetChromeAmmoText(item);
		desc_y = MCH_PANE_Y + 212;
		int bulk = GetBulkPriceForAmmo(item);
		if(bulk)
			MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_INBULK", s:": \c[UIGold]", s:MchMoney(bulk)), MCH_ID_PANETXT + 6, MCH_PANE_IX + 560, MCH_PANE_FOOT + 18, 1);
		hint = "DND_MCH_HINT_AMMO";
	}
	else
		desc = StrParam(l:GetAccountPurchaseText(item - SHOP_ACCOUNT_BEGIN));

	MenuClipRect(MCH_PANE_IX, desc_y, MCH_PANE_TEXTW, MCH_PANE_FOOT - 12 - desc_y, MCH_PANE_TEXTW);
	MchText("MENUBDYS", StrParam(s:"\c[UIWhite]", s:desc), MCH_ID_PANETXT + 2, MCH_PANE_IX, desc_y, 3);
	MenuClipRect(0, 0, 0, 0, 0);

	MchText("MENUHEAD", StrParam(s:r.pcol, s:MchMoney(r.price)), MCH_ID_PANETXT + 5, MCH_PANE_IX, MCH_PANE_FOOT, 3);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_STOCK", s:": \c[UIWhite]", d:r.stock), MCH_ID_PANETXT + 3, MCH_PANE_IX + 300, MCH_PANE_FOOT + 18, 1);
	if(r.owned)
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_SELLSFOR", s:": \c[UIGreen]", s:MchMoney(GetShopPrice(item) / 2)), MCH_ID_PANETXT + 4, MCH_PANE_IX + 560, MCH_PANE_FOOT + 18, 1);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:hint), MCH_ID_PANETXT + 7, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// DrawAmmoExplanation's text, without its drawing.
str GetChromeAmmoText(int itemid) {
	int ammo_id = itemid - SHOP_FIRSTAMMO_INDEX;
	int ammo_ref = GetMenuAmmoCount(ammo_id, AMMOID_WEPREF);
	str base = StrParam(l:"DND_MENU_GIVES", s:" \c[UIGold]", d:GetAmmoToGive(itemid), s:"\c[UIWhite] ", l:StrParam(s:"DND_MENU_AMMOTEXT", d:ammo_id + 1));
	if(ammo_ref < 65536) {
		if(itemid <= SHOP_AMMO_CELL || itemid >= SHOP_AMMO_FLECHETTE)
			return base;
		return StrParam(s:base, s:" ", l:"DND_MENU_FOR", s:"\c[UICyan]", l:GetWeaponTag(ammo_ref), s:"\c[UIWhite].");
	}
	return StrParam(s:base, s:" ", l:"DND_MENU_FOR", s:"\c[UICyan]", l:GetWeaponTag(ammo_ref & 0xFFFF), s:"\c[UIWhite]", l:"DND_MENU_AND",
		s:"\c[UICyan]", l:GetWeaponTag(ammo_ref >> 16), s:"\c[UIWhite].");
}

// ---------------------------------------------------------------- the character pages

#define MCH_STAT_LINEH 27
#define MCH_STAT_TOP (MCH_PANE_Y + 22)
#define MCH_STAT_VIEW 600

// lines a text takes when wrapped at cols characters, colour codes not counted
int MchCountLines(str t, int cols) {
	int lines = 0, col = 0, n = StrLen(t);
	for(int i = 0; i < n; ++i) {
		int c = GetChar(t, i);
		if(c == 28) {
			if(i + 1 < n && GetChar(t, i + 1) == '[') {
				while(i < n && GetChar(t, i) != ']')
					++i;
			}
			else
				++i;
			continue;
		}
		if(c == '\n') {
			++lines;
			col = 0;
			continue;
		}
		if(++col >= cols) {
			++lines;
			col = 0;
		}
	}
	return lines + (col > 0);
}

int GetChromeStatCategory(int page) {
	switch(page) {
		case MENU_STAT2_OFFENSE1:
		return DRAW_STAT_OFFENSE1;
		case MENU_STAT2_OFFENSE2:
		return DRAW_STAT_OFFENSE2;
		case MENU_STAT2_DEFENSE:
		return DRAW_STAT_DEFENSE;
		case MENU_STAT2_UTILITY:
		return DRAW_STAT_UTILITY;
		case MENU_STAT2_EXOTIC:
		return DRAW_STAT_EXOTIC;
	}
	return DRAW_STAT_MISC;
}

str GetChromeStatName(int i) {
	if(!i)
		return "DND_MENU_GAIN_OFFENSE";
	if(i == 1)
		return "DND_MENU_GAIN_DEFENSE";
	if(i == 2)
		return "DND_MENU_GAIN_UTILITY";
	if(i == 3)
		return "DND_MENU_GAIN_EXOTIC";
	return "DND_MENU_GAIN_MISC";
}

// map difficulty, kills and lives, as the old gains pages showed them
str GetChromeRunSummary() {
	str out = StrParam(s:"\c[UIGrey]", l:"DND_MENU_MAPDIFF", s:": \c[UIWhite]", l:MapDifficultyLabel(MapData[DND_MAPDATA_DIFFICULTY]),
		s:"\n\c[UIGrey]", l:"DND_MENU_TOTALKILLS", s:": \c[UIWhite]", s:GetPlayerLifetimeKills());
	if(GetCVar("survival"))
		out = StrParam(s:out, s:"\n\c[UIGrey]", l:"DND_MENU_LIVESLEFT", s:": \c[UIWhite]", d:GetPlayerLivesLeft(PlayerNumber()));
	return out;
}

void DrawChromeCharacter(int pnum, int page, int boxid, int mainboxid) {
	int k = GetChromeKind(page), i, y;
	str st;
	mch_state_T module& s = GetMchState();
	if(s.page != page) {
		s.page = page;
		s.row = 0;
		s.scroll = 0;
		s.scroll_max = 0;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);

	if(k == MCH_KIND_OVERVIEW) {
		int lvl = CheckInventory("Level"), pct = 0;
		bool unity = PlayerModData[pnum].vals[PSTAT_EX_UNITY];
		MchText("MENUHEAD", StrParam(s:"\c[UIWhite]", l:"DND_STAT18", s:" ", d:lvl, s:"   \c[UIGreen]",
			l:GetClassLabel(StrParam(s:"CLASS", d:CheckInventory("DnD_Character") - 1), DND_CLASS_LABEL_NAME)), MCH_ID_ROWTXT + 30, MCH_ROW_X, 282, 3);
		if(lvl > 0 && lvl <= MAXLEVELS)
			pct = Clamp_Between(GetPlayerExp(pnum) / Max(LevelCurve[lvl - 1] / 100, 1), 0, 100);
		MchPic("MCXPBG", MCH_ID_PANEIMG + 21, MCH_ROW_X, 338);
		if(pct) {
			MenuClipRect(MCH_ROW_X + 3, 342, 414 * pct / 100, 8);
			MchPic("MCXPFL", MCH_ID_PANEIMG + 20, MCH_ROW_X + 3, 342);
			MenuClipRect(0, 0, 0, 0, 0);
		}
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_STAT16", s:": \c[UIWhite]", d:GetPlayerExp(pnum), s:" / ", d:(lvl > 0 && lvl <= MAXLEVELS) ? LevelCurve[lvl - 1] : 0),
			MCH_ID_ROWTXT + 31, MCH_ROW_X + 440, 346, 1);
		MchText("MENUBODY", StrParam(s:"\c[UIGrey]", l:"DND_STAT19", s:": \c[UIGold]", s:MchMoney(GetPlayerCredit(pnum)),
			s:"\n\c[UIGrey]", l:"DND_MENU_BUDGET", s:": \c[UIGold]$", d:CheckInventory("Budget"), s:"K",
			s:"\n\c[UIGrey]", l:"DND_ATTRIBUTEPTS", s:": ", s:CheckInventory("AttributePoint") ? "\c[UIGold]" : "\c[UIWhite]", d:CheckInventory("AttributePoint"),
			s:"\n\c[UIGrey]", l:"DND_MCH_LOOTPENALTY", s:": \c[UIWhite]", d:PlayerActivities[pnum].vote_skips),
			MCH_ID_ROWTXT + 32, MCH_ROW_X, 380, 3);

		if(boxid != MAINBOX_NONE && boxid <= 3)
			s.row = boxid - 1;
		for(i = 0; i < 3; ++i) {
			y = 560 + MCH_ROW_STEP * i;
			MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
			int base = 0, bonus = 0;
			if(!unity) {
				if(!i) {
					base = STAT_BASE + CheckInventory("PSTAT_Strength");
					bonus = GetStrength(pnum);
				}
				else if(i == 1) {
					base = STAT_BASE + CheckInventory("PSTAT_Dexterity");
					bonus = GetDexterity(pnum);
				}
				else {
					base = STAT_BASE + CheckInventory("PSTAT_Intellect");
					bonus = GetIntellect(pnum);
				}
			}
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_ATTR", d:i + 1)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", d:base, s:"  \c[UIGrey](\c[UIGreen]", d:bonus, s:"\c[UIGrey])"), MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
		}
		if(unity)
			MchText("MENUBODY", StrParam(s:"\c[UIGold]", l:"UNITY", s:": \c[UIWhite]", d:GetUnity()), MCH_ID_ROWTXT + 33, MCH_ROW_X + 30, 560 + MCH_ROW_STEP * 3 + 27, 1);

		i = Clamp_Between(s.row, 0, 2);
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:StrParam(s:"DND_MCH_ATTR", d:i + 1)), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:GetAttributeText(i), s:"\n\n\c[UIGrey]", l:"DND_MENU_ATTRKNEE"), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MenuClipRect(0, 0, 0, 0, 0);
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_ATTR"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 22, 1);
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MENU_STAT_REFUND1", s:" \c[UIGrey]", k:"+jump", s:"\c[UIDim] ", l:"DND_MENU_STAT_REFUND2",
			s:"\c[UIGold]$", d:GetAttributeRefundCost(pnum)), MCH_ID_PANETXT + 3, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
		return;
	}

	if(k == MCH_KIND_STATINDEX) {
		if(boxid != MAINBOX_NONE && boxid <= 5)
			s.row = boxid - 1;
		for(i = 0; i < 5; ++i) {
			y = MCH_ROW_Y + MCH_ROW_STEP * i;
			MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:GetChromeStatName(i)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
		}
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:GetChromeStatName(Clamp_Between(s.row, 0, 4))), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MchText("MENUBODY", GetChromeRunSummary(), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_OPEN"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
		return;
	}

	// a stat category: the run summary on the left, the stat list in the pane
	MchText("MENUBODY", GetChromeRunSummary(), MCH_ID_ROWTXT, MCH_ROW_X, MCH_ROW_Y + 4, 3);
	DrawChromePager(page, boxid, mainboxid, 0);

	pstat_text_T? pstat_text = GetPlayerStatText();
	BuildPlayerStatsText(pnum, GetChromeStatCategory(page));
	str txt = pstat_text.text;
	if(!StrLen(txt)) {
		MenuClipRect(MCH_PANE_IX, MCH_STAT_TOP, MCH_PANE_TEXTW, MCH_STAT_VIEW, MCH_PANE_TEXTW);
		MchText("MENUBDYS", StrParam(s:"\c[UIDim]", l:"DND_MCH_NOSTATS"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_STAT_TOP, 3);
		MenuClipRect(0, 0, 0, 0, 0);
		s.scroll_max = 0;
		return;
	}
	s.scroll_max = Max(MchCountLines(txt, 74) - MCH_STAT_VIEW / MCH_STAT_LINEH, 0);
	s.scroll = Clamp_Between(s.scroll, 0, s.scroll_max);
	MenuClipRect(MCH_PANE_IX, MCH_STAT_TOP, MCH_PANE_TEXTW - 24, MCH_STAT_VIEW, MCH_PANE_TEXTW - 24);
	MchText("MENUBDYS", StrParam(s:"\c[UIWhite]", s:txt), MCH_ID_PANETXT, MCH_PANE_IX, MCH_STAT_TOP - MCH_STAT_LINEH * s.scroll, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchScrollBar(0, MCH_ID_PANEIMG + 22, MCH_PANE_X + 954, MCH_STAT_TOP, 620, s.scroll, s.scroll_max);
	if(s.scroll_max) {
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_SCROLLHINT"), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
	}
}

// ---------------------------------------------------------------- the research pages

#define MCH_RES_ROWS 10

typedef struct {
	int total;
	int done;
	int found;
} mch_rescount_T;

// how far a research page (RESPAGE_*) has come
mch_rescount_T module& MchCountResearch(int respage) {
	static mch_rescount_T c;
	c.total = 0;
	c.done = 0;
	c.found = 0;
	for(int i = 0; i < MENU_MAXRES_PERPAGE; ++i) {
		int id = GetResearchInfo(respage, i).res_id;
		if(id == -1)
			break;
		++c.total;
		int st = CheckResearchStatus(id);
		if(st == RES_DONE)
			++c.done;
		else if(st == RES_KNOWN)
			++c.found;
	}
	return c;
}

int MchResEntryCount(int respage) {
	return MchCountResearch(respage).total;
}

// the entry under the cursor on a research list, -1 if none
int MchResRowAt(int page, int cx, int cy) {
	mch_state_T module& s = GetMchState();
	if(s.page != page || cx < MCH_ROW_X || cx >= MCH_ROW_X + MCH_ROW_W || cy < MCH_ROW_Y)
		return -1;
	int r = (cy - MCH_ROW_Y) / MCH_ROW_STEP;
	if(r >= MCH_RES_ROWS || (cy - MCH_ROW_Y) % MCH_ROW_STEP >= MCH_ROW_H)
		return -1;
	r += s.scroll;
	if(r >= MchResEntryCount(page - SHOP_RESPAGE_BEGIN))
		return -1;
	return r;
}

str MchResName(int respage, int i) {
	int id = GetResearchInfo(respage, i).res_id;
	if(CheckResearchStatus(id) == RES_NA && (ResearchFlags[id].res_flags & RESF_NODROP))
		return StrParam(l:"DND_MCH_RES_UNKNOWN");
	return StrParam(l:StrParam(s:"RES_LABEL", d:id));
}

str MchResProgress(int respage) {
	mch_rescount_T module& c = MchCountResearch(respage);
	return StrParam(s:"\c[UIGreen]", d:c.done, s:"\c[UIGrey] / ", d:c.total);
}

void DrawChromeResearch(int pnum, int page, int boxid, int mainboxid) {
	int k = GetChromeKind(page), i, y;
	mch_state_T module& s = GetMchState();
	if(s.page != page) {
		s.page = page;
		s.row = 0;
		s.scroll = 0;
		s.lastpos = -1;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	if(k == MCH_KIND_RESLIST) {
		DrawChromeResearchList(pnum, page, boxid, mainboxid);
		return;
	}

	// the categories, the donation below a gap; the weapons page splits into slot weapons and bundles
	int rows = (k == MCH_KIND_RESINDEX) ? 5 : 2;
	if(boxid != MAINBOX_NONE && boxid <= rows)
		s.row = boxid - 1;
	for(i = 0; i < rows; ++i) {
		y = GetChromeRowY(page, i);
		MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		str name, right = "";
		if(k == MCH_KIND_RESGUNS) {
			name = StrParam(l:i ? "DND_MCH_RES_LUX" : "DND_MCH_RES_SLOT");
			right = MchResProgress(RESPAGE_SLOTGUNS + i);
		}
		else if(i == 4) {
			name = StrParam(l:"DND_MENU_DONATE");
			right = StrParam(s:"\c[UIGold]", s:MchMoney(DND_BUDGET_CONVERSION_CREDIT), s:"\c[UIGrey]  =  \c[UIGold]1K");
		}
		else {
			name = StrParam(l:GetChromeSubLabel(MCH_SEC_RESEARCH, i + 1));
			if(i == 2) {
				mch_rescount_T module& a = MchCountResearch(RESPAGE_SLOTGUNS);
				int d = a.done, t = a.total;
				mch_rescount_T module& b = MchCountResearch(RESPAGE_LUXURYGUNS);
				right = StrParam(s:"\c[UIGreen]", d:d + b.done, s:"\c[UIGrey] / ", d:t + b.total);
			}
			else
				right = MchResProgress((i == 3) ? RESPAGE_UTILITY : i);
		}
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", s:name), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
		MchText("MENUBODY", right, MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
	}

	i = Clamp_Between(s.row, 0, rows - 1);
	str head, body, hint = "DND_MCH_HINT_OPEN";
	if(k == MCH_KIND_RESGUNS) {
		head = i ? "DND_MCH_RES_LUX" : "DND_MCH_RES_SLOT";
		body = StrParam(l:StrParam(s:"DND_MCH_RESDESC", d:5 + i));
	}
	else if(i == 4) {
		head = "DND_MENU_DONATE";
		body = StrParam(l:"DND_MCH_RESDONATE", s:"\n\n\c[UIGrey]", l:"DND_MENU_CONVRATE", s:": \c[UIGold]", s:MchMoney(DND_BUDGET_CONVERSION_CREDIT), s:"\c[UIGrey] = \c[UIGold]1K");
		hint = "DND_MCH_HINT_DONATE";
	}
	else {
		head = GetChromeSubLabel(MCH_SEC_RESEARCH, i + 1);
		body = StrParam(l:StrParam(s:"DND_MCH_RESDESC", d:i + 1));
	}
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:head), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
	MchText("MENUBODY", StrParam(s:"\c[UIWhite]", s:body), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:hint), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// One page of entries, picked by a click or stepped with the keys (MenuPosX, which every click sends along).
void DrawChromeResearchList(int pnum, int page, int boxid, int mainboxid) {
	int respage = page - SHOP_RESPAGE_BEGIN, n = MchResEntryCount(respage), i, y;
	mch_state_T module& s = GetMchState();
	int pos = Clamp_Between(CheckInventory("MenuPosX"), 0, Max(n - 1, 0));
	s.scroll_max = Max(n - MCH_RES_ROWS, 0);
	if(pos != s.lastpos) {
		s.lastpos = pos;
		if(pos < s.scroll)
			s.scroll = pos;
		else if(pos >= s.scroll + MCH_RES_ROWS)
			s.scroll = pos - MCH_RES_ROWS + 1;
	}
	s.scroll = Clamp_Between(s.scroll, 0, s.scroll_max);

	for(int r = 0; r < MCH_RES_ROWS && s.scroll + r < n; ++r) {
		i = s.scroll + r;
		y = MCH_ROW_Y + MCH_ROW_STEP * r;
		res_info_T module& rd = GetResearchInfo(respage, i);
		int st = CheckResearchStatus(rd.res_id);
		bool unknown = st == RES_NA && (ResearchFlags[rd.res_id].res_flags & RESF_NODROP);
		bool hot = mainboxid == MCH_NAV_BASE + MCH_EL_ROW + i;
		str plate = "N", col = "\c[UIWhite]", right;
		if(i == pos)
			plate = "S";
		else if(hot)
			plate = "H";
		else if(unknown)
			plate = "L";
		MchPic(StrParam(s:"MCROW", s:plate), MCH_ID_ROWIMG + 12 + r, MCH_ROW_X - 8, y - 8);
		MchPicScaled(unknown ? "RESNONE" : GetResearchIcon(respage, i), MCH_ID_ROWIMG + r, MCH_ROW_X + 36, y + 27, 0.8);
		if(st == RES_DONE) {
			col = "\c[UIGreen]";
			right = StrParam(s:"\c[UIGreen]", l:"DND_MCH_RES_DONESHORT");
		}
		else if(st == RES_KNOWN)
			right = StrParam(s:"\c[UIGold]", d:rd.res_cost, s:"K");
		else if(unknown) {
			col = "\c[UIDim]";
			right = "\c[UIDim]-";
		}
		else {
			col = "\c[UIGrey]";
			right = StrParam(s:"\c[UIGrey]", l:"DND_MCH_RES_NOTFOUND");
		}
		// dim text would vanish on the highlight
		if(i == pos && st != RES_DONE)
			col = "\c[UIWhite]";
		MchText("MENUBODY", StrParam(s:col, s:MchResName(respage, i)), MCH_ID_ROWTXT + 3 * r, MCH_ROW_X + 70, y + 27, 1);
		MchText("MENUBODY", right, MCH_ID_ROWTXT + 3 * r + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
	}
	MchScrollBar(0, MCH_ID_PANEIMG + 24, MCH_ROW_X + MCH_ROW_W + 10, MCH_ROW_Y, 620, s.scroll, s.scroll_max);
	if(s.scroll_max) {
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_SHOWING", s:" ", d:s.scroll + 1, s:"-", d:Min(s.scroll + MCH_RES_ROWS, n), s:" / ", d:n, s:"    \c[UIDim]", l:"DND_MCH_SCROLLHINT"),
			MCH_ID_PAGETXT + 2, MCH_ROW_X + MCH_ROW_W / 2, MCH_PAGER_Y + 26, 0);
	}
	if(!n)
		return;

	// the picked entry
	res_info_T module& e = GetResearchInfo(respage, pos);
	int status = CheckResearchStatus(e.res_id);
	bool nodrop = ResearchFlags[e.res_id].res_flags & RESF_NODROP;
	bool hidden = status == RES_NA && nodrop;
	MchPic("MCWELL", MCH_ID_PANEIMG + 20, MCH_PANE_IX, MCH_PANE_Y + 24);
	MchPicScaled(hidden ? "RESNONE" : GetResearchIcon(respage, pos), MCH_ID_PANEIMG, MCH_PANE_IX + 130, MCH_PANE_Y + 109, 2.6);
	str hcol = "\c[UIWhite]", sline = StrParam(s:"\c[UIRed]", l:"DND_MCH_RES_NA");
	if(status == RES_DONE) {
		hcol = "\c[UIGreen]";
		sline = StrParam(s:"\c[UIGreen]", l:"DND_MCH_RES_DONE");
	}
	else if(status == RES_KNOWN) {
		hcol = "\c[UIGold]";
		sline = StrParam(s:"\c[UIGold]", l:"DND_MCH_RES_KNOWN");
	}
	else if(hidden)
		hcol = "\c[UIDim]";
	MchText("MENUHEAD", StrParam(s:hcol, s:MchResName(respage, pos)), MCH_ID_PANETXT, MCH_PANE_TX, MCH_PANE_Y + 26, 3);
	if(hidden)
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_ENTRY", s:" \c[UIWhite]", l:"DND_MENU_NA", s:"      \c[UIGrey]", l:"DND_MENU_BUDGET", s:" ", l:"DND_MENU_COST", s:" \c[UIWhite]", l:"DND_MENU_NA"),
			MCH_ID_PANETXT + 1, MCH_PANE_TX, MCH_PANE_Y + 78, 3);
	else
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_ENTRY", s:" \c[UIWhite]#", d:e.res_number, s:"      \c[UIGrey]", l:"DND_MENU_BUDGET", s:" ", l:"DND_MENU_COST", s:" \c[UIGold]", d:e.res_cost, s:"K"),
			MCH_ID_PANETXT + 1, MCH_PANE_TX, MCH_PANE_Y + 78, 3);
	MchText("MENUBDYS", sline, MCH_ID_PANETXT + 2, MCH_PANE_TX, MCH_PANE_Y + 112, 3);
	if(status == RES_NA && !nodrop)
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_DISCOVERYCHANCE", s:" \c[UIWhite]", s:GetFixedRepresentation(GetResearchDropRate(pnum, e.res_id, true), false), s:"%",
			s:"      \c[UIGrey]", l:"DND_MCH_RES_INVESTED", s:" \c[UIWhite]", d:ResearchInvestments[pnum][e.res_id]), MCH_ID_PANETXT + 3, MCH_PANE_TX, MCH_PANE_Y + 146, 3);

	str desc = "";
	if(!hidden)
		desc = StrParam(s:"\c[UIWhite]", l:GetResearchDescription(e.res_id));
	if(status == RES_NA && !nodrop)
		desc = StrParam(s:desc, s:"\n\n\c[UIGrey]", l:"DND_INVESTMENT_EXPLANATION");
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 212, MCH_PANE_TEXTW, MCH_RES_BTNY - 16 - MCH_PANE_Y - 212, MCH_PANE_TEXTW);
	MchText("MENUBDYS", desc, MCH_ID_PANETXT + 4, MCH_PANE_IX, MCH_PANE_Y + 212, 3);
	MenuClipRect(0, 0, 0, 0, 0);

	// the two actions; a button that cannot act now is drawn dim but still answers, as the old boxes did
	for(i = 0; i < 2; ++i) {
		bool live = i ? (status == RES_NA && !nodrop) : status == RES_KNOWN;
		bool over = boxid == MBOX_1 + i;
		int bx = MCH_RES_BTNX + MCH_RES_BTNSTEP * i;
		MchPic(StrParam(s:over ? "MCTH" : "MCTO", d:i), MCH_ID_PANEIMG + 21 + i, bx, MCH_RES_BTNY);
		str lab;
		if(i)
			lab = StrParam(l:"DND_MENU_INVEST", s:"  ", s:MchMoney(GetInvestmentCost(pnum, respage, pos, e.res_id)));
		else
			lab = StrParam(l:"DND_MENU_RESEARCH", s:"  ", d:e.res_cost, s:"K");
		MchText("MENUTAB", StrParam(s:over ? "\c[UIWhite]" : live ? "\c[UIGold]" : "\c[UIDim]", s:lab), MCH_ID_PANETXT + 5 + i, bx + MCH_TAB_W / 2, MCH_RES_BTNY + 26, 0);
	}
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_RES"), MCH_ID_PANETXT + 7, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- the codex

#define MCH_CODEX_TOP (MCH_PANE_Y + 24)
#define MCH_CODEX_BOT (MCH_PANE_FOOT + 32)
#define MCH_CODEX_SLOTS 14

// the topic a codex page belongs to; the monster mod categories are under monster mods
int MchCodexTopic(int page) {
	if(page >= MENU_MMOD_HELP_BEGIN)
		return MENU_HELP_MMODS - MENU_HELP_BEGIN;
	return page - MENU_HELP_BEGIN;
}

typedef struct {
	str icon;
	int k;
	str text;
} mch_codexblock_T;

mch_codexblock_T module& GetMchCodexBlock() {
	static mch_codexblock_T b;
	return b;
}

int MchLegendaryCount() {
	int n = 0;
	for(int i = 0; i < MAX_LEGMONS_TEXT; ++i)
		n += HasKilledLegendary(i);
	return n;
}

// legendary monsters show only once one has been killed
int MchLegendaryAt(int b) {
	for(int i = 0; i < MAX_LEGMONS_TEXT; ++i)
		if(HasKilledLegendary(i) && !b--)
			return i;
	return -1;
}

int MchCodexBlockCount(int page) {
	switch(page) {
		case MENU_HELP_WEAPONPROPS:
		return MAX_WEAPON_PROPERTIES;
		case MENU_HELP_DAMAGETYPES:
		return MAX_DAMAGE_TYPES_MENU;
		case MENU_HELP_AILMENTS:
		return DND_MENU_MAX_AILMENTDESCRIPTIONS;
		case MENU_HELP_ORBS:
		return MAX_ORBS + 1;
		case MENU_HELP_LEGMONS:
		return Max(MchLegendaryCount(), 1);
		case MENU_HELP_MMODS:
		return 1;
	}
	if(page >= MENU_MMOD_HELP_BEGIN)
		return GetMonsterModMenuCount((page == MENU_HELP_MMODS_SPECIAL) ? -1 : page - MENU_MMOD_HELP_BEGIN);
	return 1;
}

str MchCodexDamageText(int i) {
	switch(i) {
		case 0:
		return "DND_MENU_BULLETTEXT";
		case 1:
		return "DND_MENU_MELEETEXT";
		case 2:
		return "DND_MENU_OCCULTTEXT";
		case 3:
		return "DND_MENU_ENERGYTEXT";
		case 4:
		return "DND_MENU_FIRETEXT";
		case 5:
		return "DND_MENU_ICETEXT";
		case 6:
		return "DND_MENU_POISONTEXT";
	}
	return "DND_MENU_LIGHTNINGTEXT";
}

str MchLegendaryIcon(int i) {
	switch(i) {
		case 0:
		return "LEG_DRMR";
		case 1:
		return "LEG_TORR";
		case 2:
		return "LEG_MORD";
		case 3:
		return "LEG_GODS";
	}
	return "LEG_GOLG";
}

// The same words the old pages showed, one block per entry: an icon (or none) and its text.
mch_codexblock_T module& MchCodexBlock(int page, int b) {
	mch_codexblock_T module& c = GetMchCodexBlock();
	int i;
	str cls;
	c.icon = "";
	c.k = 2.0;
	switch(page) {
		case MENU_HELP_CHARACTER:
			c.text = "";
			for(i = 1; i <= 9; ++i)
				c.text = StrParam(s:c.text, s:"\c[UIGold]*  \c[UIWhite]", l:StrParam(s:"DND_MENU_CHARHELPTEXT", d:i), s:(i < 9) ? "\n\n" : "");
		break;
		case MENU_HELP_RESEARCHES:
			c.text = "";
			for(i = 1; i <= 5; ++i)
				c.text = StrParam(s:c.text, s:"\c[UIGold]*  \c[UIWhite]", l:StrParam(s:"DND_MENU_RESHELPTEXT", d:i), s:(i < 5) ? "\n\n" : "");
		break;
		case MENU_HELP_CLASSPERKS:
			cls = StrParam(s:"CLASS", d:CheckInventory("DnD_Character") - 1);
			c.text = StrParam(s:"\c[UIGrey]", l:"DND_CLASS", s:": \c[UIGreen]", l:GetClassLabel(cls, DND_CLASS_LABEL_NAME), s:"\n\n\c-", s:GetClassPerkDisplay(cls, true));
		break;
		case MENU_HELP_WEAPONPROPS:
			c.icon = WeaponPropertyImages[b];
			c.k = 1.5;
			c.text = StrParam(s:"\c[UIGold]", l:StrParam(s:"DND_WEPPROP_", d:b + 1), s:"\n\c[UIWhite]", l:StrParam(s:"DND_MENU_WEPHELPTEXT", d:b + 1));
		break;
		case MENU_HELP_DAMAGETYPES:
			c.icon = GetDamageTypeIcon(b);
			c.text = StrParam(l:MchCodexDamageText(b));
		break;
		case MENU_HELP_AILMENTS:
			c.text = StrParam(s:"\c[UIGold]", l:StrParam(s:"DND_MENU_AILMENT", d:b + 1), s:"\n\c[UIWhite]", l:StrParam(s:"DND_MENU_AILMENT", d:b + 1, s:"_EXP"));
		break;
		case MENU_HELP_ORBS:
			if(!b) {
				c.text = StrParam(s:"\c[UIWhite]", l:"DND_MENU_ORBHELPTEXT");
				break;
			}
			i = b - 1;
			c.icon = GetItemImage(i + ITEM_IMAGE_ORB_BEGIN);
			c.text = StrParam(s:"\c[UIGold]", l:GetInventoryTag(i + ORBS_BEGIN), s:"\n\c[UIWhite]", l:GetInventoryText(i + ORBS_BEGIN));
			if(CheckOrbRecipe(i))
				c.text = StrParam(s:c.text, s:"\n", s:GetOrbRecipeText(i));
		break;
		case MENU_HELP_LEGMONS:
			i = MchLegendaryAt(b);
			if(i == -1) {
				c.text = StrParam(s:"\c[UIDim]", l:"DND_MCH_NOLEGENDARY");
				break;
			}
			c.icon = MchLegendaryIcon(i);
			c.k = 1.5;
			c.text = StrParam(l:StrParam(s:"DND_MENU_LEGHELPTEXT", d:i + 1));
		break;
		case MENU_HELP_MMODS:
			c.text = StrParam(s:"\c[UIWhite]", l:"DND_MCH_MMODINTRO");
		break;
		default:
			c.text = StrParam(s:"\c[UIWhite]", s:GetMonsterModMenuEntry((page == MENU_HELP_MMODS_SPECIAL) ? -1 : page - MENU_MMOD_HELP_BEGIN, b));
		break;
	}
	return c;
}

// the topics down the left; on the landing page they are boxes, elsewhere chrome nav the server decodes
void DrawChromeCodexTopics(int page, int boxid, int mainboxid) {
	int cur = (page == MENU_HELP) ? -1 : MchCodexTopic(page);
	for(int i = 0; i < MCH_CODEX_TOPICS; ++i) {
		int y = MCH_ROW_Y + MCH_ROW_STEP * i;
		bool hot = (page == MENU_HELP) ? boxid == i + 1 : mainboxid == MCH_NAV_BASE + MCH_EL_ROW + i;
		str plate = "N";
		if(i == cur || (hot && page == MENU_HELP))
			plate = "S";
		else if(hot)
			plate = "H";
		MchPic(StrParam(s:"MCROW", s:plate), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_CODEX", d:i + 1)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
	}
}

void DrawChromeCodex(int pnum, int page, int boxid, int mainboxid) {
	mch_state_T module& s = GetMchState();
	if(s.page != page) {
		s.page = page;
		s.row = 0;
		s.pscroll = 0;
		s.pscroll_max = 0;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	DrawChromeCodexTopics(page, boxid, mainboxid);
	if(page == MENU_HELP) {
		DrawChromeCodexIndex(pnum, boxid);
		return;
	}

	int n = MchCodexBlockCount(page), i, y = MCH_CODEX_TOP - MCH_STAT_LINEH * s.pscroll, slot = 0, total = 0;
	bool icons = page == MENU_HELP_WEAPONPROPS || page == MENU_HELP_DAMAGETYPES || page == MENU_HELP_ORBS || page == MENU_HELP_LEGMONS;
	int tx = MCH_PANE_IX + (icons ? 130 : 0), cols = icons ? 68 : 80;
	for(i = 0; i < n; ++i) {
		mch_codexblock_T module& c = MchCodexBlock(page, i);
		int h = MchCountLines(c.text, cols) * MCH_STAT_LINEH;
		if(StrLen(c.icon))
			h = Max(h, 96);
		// only what reaches the view is drawn; the rest only counts toward the scroll range
		if(y + total + h > MCH_CODEX_TOP && y + total < MCH_CODEX_BOT && slot < MCH_CODEX_SLOTS) {
			// a scaled icon changes the hud size under the clip, so it is drawn only when wholly in view
			if(StrLen(c.icon) && y + total >= MCH_CODEX_TOP && y + total + 96 <= MCH_CODEX_BOT)
				MchPicScaled(c.icon, MCH_ID_PANEIMG + slot, MCH_PANE_IX + 56, y + total + 44, c.k);
			MenuClipRect(MCH_PANE_IX, MCH_CODEX_TOP, MCH_PANE_TEXTW - 24, MCH_CODEX_BOT - MCH_CODEX_TOP, MCH_PANE_TEXTW - 24 - (tx - MCH_PANE_IX) * !!StrLen(c.icon));
			MchText("MENUBDYS", c.text, MCH_ID_PANETXT + slot, StrLen(c.icon) ? tx : MCH_PANE_IX, y + total, 3);
			MenuClipRect(0, 0, 0, 0, 0);
			++slot;
		}
		total += h + MCH_STAT_LINEH;
	}
	int view = (MCH_CODEX_BOT - MCH_CODEX_TOP) / MCH_STAT_LINEH;
	s.pscroll_max = Max((total + MCH_STAT_LINEH - 1) / MCH_STAT_LINEH - view, 0);
	if(s.pscroll > s.pscroll_max)
		s.pscroll = s.pscroll_max;
	MchScrollBar(1, MCH_ID_PANEIMG + 22, MCH_PANE_X + 954, MCH_CODEX_TOP, MCH_CODEX_BOT - MCH_CODEX_TOP, s.pscroll, s.pscroll_max);
	if(s.pscroll_max) {
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_SCROLLHINT"), MCH_ID_PAGETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
	}

	// the monster mod categories are this page's own boxes, as rows in the pane
	if(page == MENU_HELP_MMODS)
		for(i = 0; i < 7; ++i) {
			y = MCH_CODEX_CATY + MCH_ROW_STEP * i;
			MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : "N"), MCH_ID_PANEIMG + 14 + i, MCH_PANE_IX - 8, y - 8);
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_MMOD", d:i + 1)), MCH_ID_PANETXT + 14 + i, MCH_PANE_IX + 30, y + 27, 1);
		}
}

// The landing page: the hovered topic up top, the game options (boxes 10-13) below it.
void DrawChromeCodexIndex(int pnum, int boxid) {
	mch_state_T module& s = GetMchState();
	int i, y;
	if(boxid != MAINBOX_NONE && boxid <= MCH_CODEX_TOPICS)
		s.row = boxid - 1;
	i = Clamp_Between(s.row, 0, MCH_CODEX_TOPICS - 1);
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:StrParam(s:"DND_MCH_CODEX", d:i + 1)), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 180, MCH_PANE_TEXTW);
	MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_CODEXDESC", d:i + 1)), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
	MenuClipRect(0, 0, 0, 0, 0);

	// the option toggles flip their cvar here, on the client, when the server hands back the pulse item
	if(CheckInventory("SetMapInfo")) {
		SetCVar("dnd_showmapinfo", !GetCVar("dnd_showmapinfo"));
		SetInventory("ShowMapInfo", !CheckInventory("ShowMapInfo"));
	}
	if(CheckInventory("SetModTierInfo"))
		SetCVar("dnd_detailedmods", !GetCVar("dnd_detailedmods"));
	if(CheckInventory("SetTempWepIgnore"))
		SetCVar("dnd_ignoretempweps", !GetCVar("dnd_ignoretempweps"));

	MenuClipRect(MCH_PANE_IX, MCH_CODEX_OPTY - 70, MCH_PANE_TEXTW, 2);
	MchPic("MCDIV", MCH_ID_PAGEIMG + 1, MCH_PANE_IX, MCH_CODEX_OPTY - 70);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_OPTIONS"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_CODEX_OPTY - 34, 1);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_AVGMONLEVEL", s:" \c[UIGold]", d:CheckInventory("DnD_PTotalLevel") / Max(PlayerCount(), 1)),
		MCH_ID_PANETXT + 3, MCH_PANE_IX + MCH_ROW_W, MCH_CODEX_OPTY - 34, 2);
	for(i = 0; i < 4; ++i) {
		y = MCH_CODEX_OPTY + MCH_ROW_STEP * i;
		MchPic(StrParam(s:"MCROW", s:(boxid == MCH_CODEX_TOPICS + 1 + i) ? "S" : "N"), MCH_ID_PANEIMG + 14 + i, MCH_PANE_IX - 8, y - 8);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_OPT", d:i + 1)), MCH_ID_PANETXT + 4 + 2 * i, MCH_PANE_IX + 30, y + 27, 1);
		str v;
		if(i == 3)
			v = StrParam(s:"\c[UIGold]", l:StrParam(s:"DND_MCH_FILTER_", d:GetCVar("dnd_pickup_filter")));
		else {
			bool on;
			if(!i)
				on = CheckInventory("ShowMapInfo");
			else if(i == 1)
				on = GetCVar("dnd_detailedmods");
			else
				on = GetCVar("dnd_ignoretempweps");
			v = StrParam(s:on ? "\c[UIGreen]" : "\c[UIGrey]", l:on ? "DND_MCH_ON" : "DND_MCH_OFF");
		}
		MchText("MENUBODY", v, MCH_ID_PANETXT + 5 + 2 * i, MCH_PANE_IX + MCH_ROW_W - 20, y + 27, 2);
	}
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_CODEX"), MCH_ID_PAGETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- skills

int MchTreeLearned(int pnum, int tree) {
	int n = 0, c = GetTreeSpellCount(tree);
	for(int j = 0; j < c; ++j)
		n += GetSpellAllocatedRank(pnum, GetTreeSpellByIndex(tree, j)) > 0;
	return n;
}

// The trees, the hotbar and the gems; each tree opens its own page, which keeps its board.
void DrawChromeSkillIndex(int pnum, int boxid) {
	mch_state_T module& s = GetMchState();
	int i, y, rows = MAX_SKILL_TREES + 2;
	if(s.page != MENU_SPELL) {
		s.page = MENU_SPELL;
		s.row = 0;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	if(boxid != MAINBOX_NONE && boxid <= rows)
		s.row = boxid - 1;
	for(i = 0; i < rows; ++i) {
		y = GetChromeRowY(MENU_SPELL, i);
		bool empty = i < MAX_SKILL_TREES && !GetTreeSpellCount(i);
		MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : empty ? "L" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		str name;
		if(i < MAX_SKILL_TREES) {
			name = StrParam(l:StrParam(s:"DND_SPELLTREE", d:i));
			if(!empty) {
				int got = MchTreeLearned(pnum, i);
				MchText("MENUBODY", StrParam(s:got ? "\c[UIGold]" : "\c[UIGrey]", d:got, s:"\c[UIGrey] / ", d:GetTreeSpellCount(i)),
					MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
			}
		}
		else
			name = StrParam(l:(i == MAX_SKILL_TREES) ? "DND_MCH_SUB_HOTBAR" : "DND_MCH_SUB_GEMS");
		MchText("MENUBODY", StrParam(s:empty ? "\c[UIDim]" : "\c[UIWhite]", s:name), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
	}

	i = Clamp_Between(s.row, 0, rows - 1);
	str head, body;
	if(i < MAX_SKILL_TREES) {
		head = StrParam(s:"DND_SPELLTREETITLE", d:i);
		if(GetTreeSpellCount(i))
			body = StrParam(s:"\c[UIGrey]", l:"DND_MCH_LEARNED", s:" \c[UIWhite]", d:MchTreeLearned(pnum, i), s:" / ", d:GetTreeSpellCount(i),
				s:"\n\n\c[UIWhite]", l:"DND_SPELLINDEX_HELP1", s:"\n", l:"DND_SPELLINDEX_HELP2",
				s:"\n", l:"DND_MENU_PERK_PINHELP1", s:" \c[UIGold]", k:"+jump", s:"\c[UIWhite] ", l:"DND_MENU_PERK_PINHELP2",
				s:"\n", l:"DND_SPELLINDEX_REFUND1", s:" \c[UIGold]", k:"+jump", s:"\c[UIWhite] ", l:"DND_SPELLINDEX_REFUND2");
		else
			body = StrParam(s:"\c[UIDim]", l:"DND_MCH_TREEEMPTY");
	}
	else {
		head = (i == MAX_SKILL_TREES) ? "DND_SPELLTREE_HOTBAR" : "DND_SPELLTREE_GEMS";
		body = StrParam(s:"\c[UIWhite]", l:(i == MAX_SKILL_TREES) ? "DND_MCH_HOTBARDESC" : "DND_MCH_GEMSDESC");
	}
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:head), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 520, MCH_PANE_TEXTW);
	MchText("MENUBODY", body, MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_OPEN"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// The spell pages' own backdrop gave way to the frame; their info panel keeps a pane behind it.
void DrawChromeSpellPocket() {
	MenuHudSize(MCH_W, MCH_H);
	MchPic("MCPOCKET", RPGMENUID, 1140, 194);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

// ---------------------------------------------------------------- gear

// The equipment page's own layout (480x320 units) drawn 2x in the pane, so every slot sits where players know it;
// the charm and armor groups each keep their own shape but stand further apart than on that page.
#define MCH_GEAR_CHARMX (MCH_PANE_IX + 145)
#define MCH_GEAR_ARMORX (MCH_GEAR_CHARMX + 392)
#define MCH_GEAR_Y 440

int MchGearX(int x) {
	if(x < 290)
		return MCH_GEAR_CHARMX + (x - 188) * 2;
	return MCH_GEAR_ARMORX + (x - 296) * 2;
}

int MchGearY(int y) {
	return MCH_GEAR_Y + (y - 38) * 2;
}

// the page a gear row opens, as the server's own row handling does; -1 is the gear index
int MchGearRowPage(int i) {
	switch(i) {
		case -1:
		return MENU_LOAD;
		case 6:
		return MENU_LOAD_ACC1;
		case 7:
		return MENU_LOAD_TRADE1;
		case 8:
		return MENU_LOAD_DUNGEONS;
		case 5:
		return MENU_LOAD_CRAFTING_WEAPON;
	}
	return MENU_LOAD + i + 1;
}

str MchGearRowName(int i) {
	switch(i) {
		case 1:
		return "DND_MENU_INVENTORY";
		case 2:
		return "DND_MENU_STASH";
		case 3:
		return "DND_MENU_EQUIPMENTS";
		case 4:
		return "DND_MENU_WEAPONDISCARD";
		case 5:
		return "DND_MENU_CRAFTING";
		case 6:
		return "DND_MENU_ACCESSORIES";
		case 7:
		return "DND_MENU_TRADING";
	}
	return "DND_MENU_DUNGEONS";
}

// One equipment slot centred where the equipment page has it: its well, and the equipped item's picture at the same 2x.
void MchGearSlot(int pnum, int slot, int n, int x, int y, str well, int w, int h, bool locked, str empty) {
	int cx = MchGearX(x), cy = MchGearY(y);
	MchPic(MchWellPic(locked ? "MCSLFL" : well, slot, false), (n < 12) ? MCH_ID_ROWIMG + n : MCH_ID_ROWIMG + 9 + n, cx - w / 2, cy - h / 2);
	if(locked)
		return;
	auto item = GetUsedItem(pnum, slot);
	if(item.item_type != DND_ITEM_NULL)
		MchPicScaled(GetItemImage(item.item_image), MCH_ID_PANEIMG + n, cx, cy, 2.0);
	// an empty slot shows the same shadowed picture as the equipment page, so each slot says what goes in it
	else if(StrLen(empty))
		MchPicScaled(empty, MCH_ID_PANEIMG + n, cx, cy, 2.0);
}

// The gear pages down the left; the pane shows the hovered one over what is equipped, all four flask slots included.
void DrawChromeGearIndex(int pnum, int boxid) {
	mch_state_T module& s = GetMchState();
	int i, y, n = 0;
	if(s.page != MENU_LOAD) {
		s.page = MENU_LOAD;
		s.row = 1;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	if(boxid > MBOX_1 && boxid <= 9)
		s.row = boxid - 1;
	// row 0 was the artifacts page, which the menu no longer has
	for(i = 1; i < 9; ++i) {
		y = MCH_ROW_Y + MCH_ROW_STEP * (i - 1);
		MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:MchGearRowName(i)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
	}

	i = Clamp_Between(s.row, 1, 8);
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:MchGearRowName(i)), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 90, MCH_PANE_TEXTW);
	MchText("MENUBDYS", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_GEARDESC", d:i + 1)), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
	MenuClipRect(0, 0, 0, 0, 0);

	MenuClipRect(MCH_PANE_IX, MCH_GEAR_Y - 44, MCH_PANE_TEXTW, 2);
	MchPic("MCDIV", MCH_ID_PAGEIMG + 1, MCH_PANE_IX, MCH_GEAR_Y - 44);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_GEARROW2"), MCH_ID_PANETXT + 2, MchGearX(236), MCH_GEAR_Y - 20, 0);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_GEARROW1"), MCH_ID_PANETXT + 3, MchGearX(394), MCH_GEAR_Y - 20, 0);

	// charms: the large one on top, the two medium under it, the four small in a square
	MchGearSlot(pnum, LARGECHARM_INDEX, n++, 236, 80, "MCSLG", 80, 168, false, "");
	MchGearSlot(pnum, MEDIUMCHARM_INDEX1, n++, 208, 154, "MCSLM", 80, 112, false, "");
	MchGearSlot(pnum, MEDIUMCHARM_INDEX2, n++, 264, 154, "MCSLM", 80, 112, false, "");
	MchGearSlot(pnum, SMALLCHARM_INDEX1, n++, 208, 208, "MCSLS", 80, 80, false, "");
	MchGearSlot(pnum, SMALLCHARM_INDEX2, n++, 264, 208, "MCSLS", 80, 80, false, "");
	MchGearSlot(pnum, SMALLCHARM_INDEX3, n++, 208, 256, "MCSLS", 80, 80, false, "");
	MchGearSlot(pnum, SMALLCHARM_INDEX4, n++, 264, 256, "MCSLS", 80, 80, false, "");
	// armor: helm over body, the class specialty left of the body, boots under it
	MchGearSlot(pnum, HELM_INDEX, n++, 394, 88, "MCSLA", 100, 100, false, GetArmorSlotSilhouette(DND_ITEM_HELM));
	MchGearSlot(pnum, BODY_ARMOR_INDEX, n++, 394, 152, "MCSLA", 100, 100, false, GetArmorSlotSilhouette(DND_ITEM_BODYARMOR));
	MchGearSlot(pnum, POWERCORE_INDEX, n++, 338, 152, "MCSLA", 100, 100, false, GetArmorSlotSilhouette(DND_ITEM_SPECIALTY_DOOMGUY + GetPlayerClass()));
	MchGearSlot(pnum, BOOT_INDEX, n++, 394, 216, "MCSLA", 100, 100, false, GetArmorSlotSilhouette(DND_ITEM_BOOT));
	// flasks: the 2x2 block left of the boots, all four shown, the ones not unlocked yet as locked
	int open = GetPlayerFlaskSlots(pnum);
	for(i = 0; i <= FLASK4_INDEX - FLASK1_INDEX; ++i)
		MchGearSlot(pnum, FLASK1_INDEX + i, n++, (i & 1) ? 348 : 312, (i & 2) ? 247 : 207, "MCSLF", 64, 64, i >= open, "FLASKBAK");

	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_OPEN"), MCH_ID_PAGETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- gear: equipment

// the shadowed picture an empty armor slot shows
str MchEqSilhouette(int i) {
	if(i == POWERCORE_INDEX)
		return GetArmorSlotSilhouette(DND_ITEM_SPECIALTY_DOOMGUY + GetPlayerClass());
	if(i == HELM_INDEX)
		return GetArmorSlotSilhouette(DND_ITEM_HELM);
	if(i == BOOT_INDEX)
		return GetArmorSlotSilhouette(DND_ITEM_BOOT);
	return GetArmorSlotSilhouette(DND_ITEM_BODYARMOR);
}

// Every equipped slot where the old page had it, larger; a click opens the inventory over the page as before.
void DrawChromeGearEquip(int pnum, int boxid) {
	bool view = CheckInventory("DnD_InventoryView");
	int i, hov = (!view && boxid != MAINBOX_NONE) ? boxid - 1 : -1, open = GetPlayerFlaskSlots(pnum);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_GEARROW2"), MCH_ID_PANETXT + 2, MchEqX(236), MCH_EQ_Y - 24, 0);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_GEARROW1"), MCH_ID_PANETXT + 3, MchEqX(394), MCH_EQ_Y - 24, 0);
	for(i = 0; i < INV_ICON_INDEX; ++i) {
		int cx = MchEqX(MchEqSlotX(i)), cy = MchEqY(MchEqSlotY(i)), w = MchEqWellW(i), h = MchEqWellH(i);
		if(i >= FLASK1_INDEX + open) {
			MchPic(MchWellPic("MCEQFL", i, false), MCH_ID_PANEIMG + 15 + i, cx - w / 2, cy - h / 2);
			continue;
		}
		MchPic(MchWellPic(MchEqWell(i), i, i == hov), MCH_ID_PANEIMG + 15 + i, cx - w / 2, cy - h / 2);
		// item pictures stay 2x so they sit crisp inside the wells
		auto item = GetUsedItem(pnum, i);
		if(item.item_type != DND_ITEM_NULL)
			MchPicScaled(GetItemImage(item.item_image), MCH_ID_PANEIMG + i, cx, cy, 2.0);
		else if(i >= FLASK1_INDEX)
			MchPicScaled("FLASKBAK", MCH_ID_PANEIMG + i, cx, cy, 2.0);
		else if(i >= HELM_INDEX)
			MchPicScaled(MchEqSilhouette(i), MCH_ID_PANEIMG + i, cx, cy, 2.0);
	}
	bool over = !view && boxid == INV_ICON_INDEX + 1;
	MchPic(over ? "MCTH2" : "MCTO2", MCH_ID_PAGEIMG + 2, MCH_EQ_BTNX, MCH_EQ_BTNY);
	MchText("MENUTAB", StrParam(s:over ? "\c[UIWhite]" : "\c[UIGold]", l:"DND_MENU_INVENTORY"), MCH_ID_PANETXT + 4, MCH_EQ_BTNX + MCH_TAB_W / 2, MCH_EQ_BTNY + 26, 0);
	str hint = "DND_MCH_HINT_EQUIP";
	if(view)
		hint = CheckInventory("DnD_SelectedCharmBox") ? "DND_MCH_HINT_EQUIPPICK" : "DND_MCH_HINT_EQUIPINV";
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:hint), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);

	// the hovered item's card as the old page showed it; an empty slot leaves none behind
	if(view)
		return;
	if(hov >= 0 && hov < INV_ICON_INDEX && GetUsedItem(pnum, hov).item_type != DND_ITEM_NULL)
		UpdateCursorHoverData(hov, DND_SYNC_ITEMSOURCE_ITEMSUSED, 0, pnum, 0, HUDMAX_X, HUDMAX_Y);
	else if(PlayerCursorData.itemHovered != -1)
		ResetCursorHoverData();
}

// ---------------------------------------------------------------- gear: inventory grid

// The plain inventory grid keeps its cells where the hit boxes are (32 menu units each); only the art is the frame's.
#define MCH_INV_X 288
#define MCH_INV_Y 270

// Stash and inventory side by side, each on its own panel: heading and sort on top, the grid, then the stash's page
// tabs or the inventory's space; the cells are 64 x 72 (32 units of the stash hud space).
#define MCH_STASH_X 84
#define MCH_STASH_IX 804
#define MCH_STASH_Y 342
#define MCH_STASH_TABY 722
#define MCH_STASH_SORTY 280
// The trade view: their offer, our offer and our inventory side by side across the frame, buttons under our offer.
#define MCH_TRADE_X0 -160
#define MCH_TRADE_X1 432
#define MCH_TRADE_X2 1024
#define MCH_TRADE_Y 333
#define MCH_TRADE_BTNY 714
#define MCH_TRADE_ROWSTEP 38

void MchInventoryPanel(int id) {
	MenuHudSize(MCH_W, MCH_H);
	MchPic("MCINVPN", id, MCH_INV_X - 36, MCH_INV_Y - 34);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

// the plate state letter for the old cell picture, empty for a bare cell
str MchCellState(str old) {
	if(!StrCmp(old, "LDTBOXO") || !StrCmp(old, "LDTBOXCN"))
		return "O";
	if(!StrCmp(old, "LDTBOXR"))
		return "R";
	if(!StrCmp(old, "LDTBOXS"))
		return "S";
	if(!StrCmp(old, "LDTBOXC"))
		return "C";
	if(!StrCmp(old, "LDTBOXN"))
		return "N";
	return "";
}

// a neighbour joins this cell's plate when it shows the same highlight, or the same item
// a confirmed trade marks every cell of that side; on the frame an item there stays an item and an empty cell plain
str MchCellPicNorm(int pnum, str old, int bid, int source) {
	if(StrCmp(old, "LDTBOXCN"))
		return old;
	return GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source) ? "LDTBOXO" : "LDTBOX";
}

int MchGridX(int grid) {
	switch(grid) {
		case 1:
		return MCH_STASH_X;
		case 2:
		return MCH_STASH_IX;
		case 3:
		return MCH_TRADE_X0;
		case 4:
		return MCH_TRADE_X1;
		case 5:
		return MCH_TRADE_X2;
	}
	return MCH_INV_X;
}

bool MchCellJoins(int pnum, str old, int bid, int nb, int boff, int source) {
	if(StrCmp(MchCellPicNorm(pnum, GetInventoryCellPic(pnum, nb, boff, source), nb, source), old))
		return false;
	if(!StrCmp(old, "LDTBOXO") || !StrCmp(old, "LDTBOXR") || !StrCmp(old, "LDTBOXCN"))
		return GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source) == GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, nb, -1, source);
	return true;
}

// The frame's plate for a grid cell: items and highlights merge across their cells into one plate (MCIP<state><mask>).
void MchInventoryCell(int pnum, str old, int id, int row, int col, int boff, int source, int grid) {
	int bid = row * MAXINVENTORYBLOCKS_VERT + col, mask = 0;
	old = MchCellPicNorm(pnum, old, bid, source);
	str st = MchCellState(old), pic = "MCIP";
	if(StrLen(st)) {
		if(col > 0 && MchCellJoins(pnum, old, bid, bid - 1, boff, source))
			mask |= 1;
		if(col < MAXINVENTORYBLOCKS_VERT - 1 && MchCellJoins(pnum, old, bid, bid + 1, boff, source))
			mask |= 2;
		if(row > 0 && MchCellJoins(pnum, old, bid, bid - MAXINVENTORYBLOCKS_VERT, boff, source))
			mask |= 4;
		if(row < MAXINVENTORYBLOCKS_HORIZ - 1 && MchCellJoins(pnum, old, bid, bid + MAXINVENTORYBLOCKS_VERT, boff, source))
			mask |= 8;
		pic = StrParam(s:grid ? "MCIQ" : "MCIP", s:st, x:mask);
	}
	else if(grid)
		pic = "MCIQ";
	MenuHudSize(MCH_W, MCH_H);
	if(grid) {
		MchPic(pic, id, MchGridX(grid) + 64 * col, ((grid < 3) ? MCH_STASH_Y : MCH_TRADE_Y) + 72 * row);
		MenuHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH);
		return;
	}
	MchPic(pic, id, MCH_INV_X + 96 * col, MCH_INV_Y + 108 * row);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

// the sort plate over the old sort box, the label on the id after the plate's
void MchInventorySort(bool over, int id) {
	MenuHudSize(MCH_W, MCH_H);
	MchPic(over ? "MCSRTH" : "MCSRTO", id, 660, 818);
	MchText("MENUTAB", StrParam(s:CheckInventory("DnD_SortCooldown") ? "\c[UIDim]" : over ? "\c[UIWhite]" : "\c[UIGold]", l:"DND_SORT"), id - 1, 720, 844, 0);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

int MchInventoryCellsUsed(int pnum) {
	int n = 0;
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i)
		n += GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, i, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) != DND_ITEM_NULL;
	return n;
}

// The inventory page: the grid draws itself over the page; the page adds how full it is and how to use it.
void DrawChromeGearInv(int pnum) {
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_CELLSUSED", s:"  \c[UIGold]", d:MchInventoryCellsUsed(pnum), s:" / ", d:MAX_INVENTORY_BOXES),
		MCH_ID_PAGETXT + 1, MCH_PANE_X + 976, 221, 2);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_INV"), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- gear: stash

// canvas rect of stash view box id: 0 x, 1 y, 2 w, 3 h
int MchStashBox(int id, int what) {
	int x, y, w, h;
	if(id < 2 * MAX_INVENTORY_BOXES) {
		x = ((id < MAX_INVENTORY_BOXES) ? MCH_STASH_X : MCH_STASH_IX) + 64 * (id % MAXINVENTORYBLOCKS_VERT);
		y = MCH_STASH_Y + 72 * ((id % MAX_INVENTORY_BOXES) / MAXINVENTORYBLOCKS_VERT);
		w = 64;
		h = 72;
	}
	else if(id < 2 * MAX_INVENTORY_BOXES + MAX_EXTRA_INVENTORY_PAGES) {
		x = MCH_STASH_X + 48 * (id - 2 * MAX_INVENTORY_BOXES);
		y = MCH_STASH_TABY;
		w = 44;
		h = 42;
	}
	else if(id == 2 * MAX_INVENTORY_BOXES + PAGEID_STASHTAB_ORBS) {
		x = MCH_STASH_X + 484;
		y = MCH_STASH_TABY;
		w = 92;
		h = 42;
	}
	else {
		x = ((id == STASHSORT_BOXID_STASH - 1) ? MCH_STASH_X : MCH_STASH_IX) + 456;
		y = MCH_STASH_SORTY;
		w = 120;
		h = 52;
	}
	if(what == 0)
		return x;
	if(what == 1)
		return y;
	return (what == 2) ? w : h;
}

// the hit box edges of stash view box id in the mirrored cursor space: 0 top left x, 1 top left y, 2 bottom right x, 3 bottom right y
int MchStashRectEdge(int id, int edge) {
	if(edge == 0)
		return HUDMAX_XF - FixedDiv(MchStashBox(id, 0) << 16, MCH_SCALE_X);
	if(edge == 1)
		return HUDMAX_YF - FixedDiv(MchStashBox(id, 1) << 16, MCH_SCALE_Y);
	if(edge == 2)
		return HUDMAX_XF - FixedDiv((MchStashBox(id, 0) + MchStashBox(id, 2)) << 16, MCH_SCALE_X);
	return HUDMAX_YF - FixedDiv((MchStashBox(id, 1) + MchStashBox(id, 3)) << 16, MCH_SCALE_Y);
}

// the two panels, on the ids the old backdrops used
void MchStashPanels(int id) {
	MenuHudSize(MCH_W, MCH_H);
	MchPic("MCSTPN", id, MCH_STASH_X - 24, MCH_STASH_SORTY - 14);
	MchPic("MCSTPN", id - 1, MCH_STASH_IX - 24, MCH_STASH_SORTY - 14);
	MenuHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH);
}

// Headings, sort buttons, page tabs and the inventory's space, on the ids the old stash controls used.
void MchStashControls(int pnum, int boxid, int curpage) {
	int base = RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES, i, x;
	bool on, over;
	MenuHudSize(MCH_W, MCH_H);
	str page = (curpage == PAGEID_STASHTAB_ORBS) ? StrParam(l:"DND_MCH_ORBS") : StrParam(l:"DND_MCH_PAGE", s:" ", d:curpage + 1);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MENU_STASH", s:"  \c[UIGrey]", s:page), RPGMENUINVENTORYID - 10 * MAX_INVENTORY_BOXES - 4, MCH_STASH_X, MCH_STASH_SORTY + 26, 1);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MENU_INVENTORY"), RPGMENUINVENTORYID - 10 * MAX_INVENTORY_BOXES - 5, MCH_STASH_IX, MCH_STASH_SORTY + 26, 1);
	for(i = 0; i < 2; ++i) {
		over = boxid == (i ? STASHSORT_BOXID_INV : STASHSORT_BOXID_STASH);
		x = (i ? MCH_STASH_IX : MCH_STASH_X) + 456;
		MchPic(over ? "MCSRTH" : "MCSRTO", base - 2 * MAX_EXTRA_INVENTORY_PAGES - 2 - 2 * i, x, MCH_STASH_SORTY);
		MchText("MENUTAB", StrParam(s:CheckInventory("DnD_SortCooldown") ? "\c[UIDim]" : over ? "\c[UIWhite]" : "\c[UIGold]", l:"DND_SORT"),
			base - 2 * MAX_EXTRA_INVENTORY_PAGES - 3 - 2 * i, x + 60, MCH_STASH_SORTY + 26, 0);
	}
	for(i = 0; i <= MAX_EXTRA_INVENTORY_PAGES; ++i) {
		int id = 2 * MAX_INVENTORY_BOXES + i;
		bool orbs = i == PAGEID_STASHTAB_ORBS;
		if(!orbs && i >= CheckInventory("DnD_PlayerInventoryPages"))
			continue;
		on = curpage == i || CheckInventory(orbs ? "DnD_ButtonPress_OrbPage" : StrParam(s:"DnD_ButtonPress_", d:i + 1));
		over = boxid == STASHBUTTON_BOXID_START + i;
		str st = on ? "S" : over ? "H" : "N";
		MchPic(StrParam(s:orbs ? "MCCO" : "MCCS", s:st), base - 2 * i, MchStashBox(id, 0) - 6, MCH_STASH_TABY - 6);
		MchText("MENUSUB", StrParam(s:on ? "\c[UIGold]" : over ? "\c[UIWhite]" : "\c[UIGrey]", s:orbs ? StrParam(l:"DND_MCH_ORBS") : StrParam(d:i + 1)),
			base - 1 - 2 * i, MchStashBox(id, 0) + MchStashBox(id, 2) / 2, MCH_STASH_TABY + 21, 0);
	}
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_CELLSUSED", s:"  \c[UIGold]", d:MchInventoryCellsUsed(pnum), s:" / ", d:MAX_INVENTORY_BOXES),
		RPGMENUINVENTORYID - 10 * MAX_INVENTORY_BOXES - 6, MCH_STASH_IX, MCH_STASH_TABY + 21, 1);
	MenuHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH);
}

// ---------------------------------------------------------------- gear: trading

// canvas rect of trade view box id: 0 x, 1 y, 2 w, 3 h
int MchTradeBox(int id, int what) {
	int x, y, w = 64, h = 72;
	if(id < 3 * MAX_INVENTORY_BOXES) {
		x = MchGridX(3 + id / MAX_INVENTORY_BOXES) + 64 * (id % MAXINVENTORYBLOCKS_VERT);
		y = MCH_TRADE_Y + 72 * ((id % MAX_INVENTORY_BOXES) / MAXINVENTORYBLOCKS_VERT);
	}
	else {
		x = MCH_TRADE_X1 + 76 + 224 * (id - 3 * MAX_INVENTORY_BOXES);
		y = MCH_TRADE_BTNY;
		w = MCH_TAB_W;
		h = MCH_TAB_H;
	}
	if(what == 0)
		return x;
	if(what == 1)
		return y;
	return (what == 2) ? w : h;
}

// the hit box edges in the mirrored cursor space: 0 top left x, 1 top left y, 2 bottom right x, 3 bottom right y
int MchTradeRectEdge(int id, int edge) {
	if(edge == 0)
		return HUDMAX_XF - FixedDiv(MchTradeBox(id, 0) << 16, MCH_SCALE_X);
	if(edge == 1)
		return HUDMAX_YF - FixedDiv(MchTradeBox(id, 1) << 16, MCH_SCALE_Y);
	if(edge == 2)
		return HUDMAX_XF - FixedDiv((MchTradeBox(id, 0) + MchTradeBox(id, 2)) << 16, MCH_SCALE_X);
	return HUDMAX_YF - FixedDiv((MchTradeBox(id, 1) + MchTradeBox(id, 3)) << 16, MCH_SCALE_Y);
}

// Headings with each side's confirmation, the confirm and cancel buttons, and our space, on the old trade ids.
void MchTradeControls(int pnum, int box) {
	int base = RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES, tradee = GetTradee();
	bool theirs = CheckActorInventory(tradee + P_TIDSTART, "DnD_Trade_Confirmed"), ours = CheckInventory("DnD_Trade_Confirmed");
	MenuHudSize(MCH_W, MCH_H);
	// each offer's heading carries whether that side has confirmed
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", n:tradee + 1, s:"\c[UICyan]", l:"DND_MENU_THEIROFFER", s:theirs ? "   \c[UIGreen]" : "   \c[UIDim]",
		l:theirs ? "DND_MCH_TR_CONFIRMED" : "DND_MCH_TR_NOTCONFIRMED"), base - 4, MCH_TRADE_X0, MCH_TRADE_Y - 26, 1);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MENU_YOUROFFERING", s:ours ? "   \c[UIGreen]" : "   \c[UIDim]",
		l:ours ? "DND_MCH_TR_CONFIRMED" : "DND_MCH_TR_NOTCONFIRMED"), base - 5, MCH_TRADE_X1, MCH_TRADE_Y - 26, 1);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MENU_YOURINV"), base - 6, MCH_TRADE_X2, MCH_TRADE_Y - 26, 1);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_CELLSUSED", s:"  \c[UIGold]", d:MchInventoryCellsUsed(pnum), s:" / ", d:MAX_INVENTORY_BOXES),
		498, MCH_TRADE_X2, MCH_TRADE_BTNY + 26, 1);
	// confirm, then cancel: pressed, confirmed (confirm only), hovered, plain
	for(int i = 0; i < 2; ++i) {
		bool press = CheckInventory(i ? "DnD_Trade_CancelButtonPress" : "DnD_Trade_ConfirmButtonPress");
		bool on = !i && ours;
		bool over = box == (i ? TRADECANCEL_BOXID : TRADECONFIRM_BOXID);
		str st = press ? "P" : on ? "A" : over ? "H" : "O";
		int x = MchTradeBox(3 * MAX_INVENTORY_BOXES + i, 0);
		MchPic(StrParam(s:"MCT", s:st, d:i), base - 8 - i, x, MCH_TRADE_BTNY);
		MchText("MENUTAB", StrParam(s:(press || on) ? "\c[UIGreen]" : over ? "\c[UIWhite]" : "\c[UIGold]", l:i ? "DND_MENU_CANCEL" : "DND_MENU_CONFIRM"),
			base - 10 - i, x + MCH_TAB_W / 2, MCH_TRADE_BTNY + 26, 0);
	}
	MenuHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH);
}

// the status words a player's row carries
str MchTradeBadge(int pnum, int i) {
	str b = "";
	if(IsTradingWith(pnum, i))
		b = StrParam(s:CheckInventory("DnD_TradeAcceptWindow") ? "\c[UIGold]" : "\c[UICyan]", l:CheckInventory("DnD_TradeAcceptWindow") ? "DND_MCH_TR_WANTS" : "DND_MCH_TR_SENT");
	else if(IsTrading(i))
		b = StrParam(s:"\c[UIRed]", l:"DND_MCH_TR_BUSY");
	if(TradeMuteStatus(pnum, i))
		b = StrParam(s:b, s:StrLen(b) ? "   " : "", s:"\c[UIDim]", l:"DND_MCH_TR_MUTED");
	if(TradeMuteStatus(i, pnum))
		b = StrParam(s:b, s:StrLen(b) ? "   " : "", s:"\c[UIDim]", l:"DND_MCH_TR_MUTEDYOU");
	return b;
}

// The players to trade with, sixteen a page in the order the server counts them (rows built as they draw); the
// pane describes the hovered one. While trading, the trade view draws itself over the page.
void DrawChromeGearTrade(int pnum, int page, int boxid, int mainboxid, menu_pane_T module& p) {
	mch_state_T module& s = GetMchState();
	if(CheckInventory("InTradeView") && IsTrading(pnum)) {
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_TRADEVIEW"), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);
		HandleInventoryViewTrade(boxid);
		return;
	}
	if(s.page != page) {
		s.page = page;
		s.row = -1;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	int skip = MENU_TRADE_PLAYERCOUNT * (page - MENU_FIRST_TRADE), j = 0, n = 0, i, y;
	p.cursize = 0;
	for(i = 0; i < MAXPLAYERS && j < skip + MENU_TRADE_PLAYERCOUNT; ++i) {
		if(!PlayerInGame(i) || i == pnum)
			continue;
		if(j++ < skip)
			continue;
		y = MCH_ROW_Y + MCH_TRADE_ROWSTEP * n;
		MchAddBox(p, MCH_ROW_X, y, MCH_ROW_W, 34);
		if(boxid == n + 1)
			s.row = i;
		MchPic((boxid == n + 1) ? "MCRWCS" : "MCRWCN", MCH_ID_ROWIMG + n, MCH_ROW_X - 8, y - 8);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", n:i + 1), MCH_ID_ROWTXT + 2 * n, MCH_ROW_X + 24, y + 17, 1);
		MchText("MENUBDYS", MchTradeBadge(pnum, i), MCH_ID_ROWTXT + 2 * n + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 17, 2);
		++n;
	}
	DrawChromePager(page, boxid, mainboxid, n);
	if(!n) {
		MchText("MENUBODY", StrParam(s:"\c[UIDim]", l:"DND_MCH_TR_NOONE"), MCH_ID_ROWTXT, MCH_ROW_X, MCH_ROW_Y + 17, 1);
		s.row = -1;
	}
	if(s.row >= 0 && PlayerInGame(s.row)) {
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", n:s.row + 1), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MchText("MENUBDYS", MchTradeBadge(pnum, s.row), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 78, 3);
	}
	else
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:"DND_MENU_HEAD_TRADE"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 130, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
	MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MCH_TR_HOW"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_Y + 132, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_TRADE"), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- home

// Bobby in his frame on the left, his welcome and how the frame works in the pane.
void DrawChromeHome() {
	MchPic("MCPORT", MCH_ID_PAGEIMG + 2, MCH_ROW_X + 195, MCH_PANE_Y);
	MchPicScaled("BBYSNC", MCH_ID_PAGEIMG + 1, MCH_ROW_X + 360, MCH_PANE_Y + 202, 1.5);
	MchText("MENUBDYS", StrParam(s:"\c[UIGold]Bobby \c[UIGrey]", l:"DND_MCH_HOME_BOBBY"), MCH_ID_PAGETXT + 3, MCH_ROW_X + 360, MCH_PANE_Y + 432, 0);
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 24, MCH_PANE_TEXTW, MCH_PANE_FOOT - MCH_PANE_Y - 24, MCH_PANE_TEXTW);
	MchText("MENUBODY", StrParam(s:"\c[UICyan]*\c[UIWhite] ", l:"DND_MENU_BOBBY1", s:"\n\n\c[UICyan]*\c[UIWhite] ", l:"DND_MENU_BOBBY2",
		s:"\n\n\c[UICyan]*\c[UIWhite] ", l:"DND_MENU_BOBBY3", s:"\n\n\c[UIGold]", l:"DND_MCH_HOME_HOW", s:"\n\c[UIGrey]", l:"DND_MCH_HOME_TIPS", s:"\n\c[UIGrey]", l:"DND_MCH_HOME_TIPS2"),
		MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(0, 0, 0, 0, 0);
}

// ---------------------------------------------------------------- world: dungeons

// The key socket and Activate on the left; the pane reads out the dungeon the key opens, as the old page listed it.
void DrawChromeDungeons(int pnum, int boxid) {
	bool view = CheckInventory("DnD_InventoryView");
	int k = GetSelectedDungeonData(pnum).id, j;
	bool hov = !view && boxid == MBOX_1;
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_DUN_KEY"), MCH_ID_PAGETXT + 3, MCH_DUN_X + 60, MCH_DUN_Y - 26, 0);
	MchPic(hov ? "MCEQAH" : "MCEQA", MCH_ID_PANEIMG + 20, MCH_DUN_X, MCH_DUN_Y);
	if(k != -1) {
		MchPicScaled(GetItemImage(GetDungeonKeyImageId(k)), MCH_ID_PANEIMG, MCH_DUN_X + 60, MCH_DUN_Y + 60, 2.0);
		if(hov)
			UpdateCursorHoverData(CheckInventory("DnD_SelectedDungeonBox") - 1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, 0, pnum, 0, HUDMAX_X, HUDMAX_Y);
	}
	else
		MchPicScaled("ORBBAKQ", MCH_ID_PANEIMG, MCH_DUN_X + 60, MCH_DUN_Y + 60, 2.0);
	if(!view && !hov && PlayerCursorData.itemHovered != -1)
		ResetCursorHoverData();
	bool over = !view && boxid == MBOX_2;
	int bx = MCH_DUN_X + 60 - MCH_TAB_W / 2;
	MchPic(over ? "MCTH3" : "MCTO3", MCH_ID_PANEIMG + 21, bx, MCH_DUN_Y + 150);
	MchText("MENUTAB", StrParam(s:over ? "\c[UIWhite]" : (k != -1) ? "\c[UIGold]" : "\c[UIDim]", l:"DND_ACTIVATE"), MCH_ID_PANETXT + 5, bx + MCH_TAB_W / 2, MCH_DUN_Y + 176, 0);

	// the dungeon in the socket, or the one already open
	if(DungeonInformation.attrib_count) {
		if(k == -1 && DungeonInformation.dungeon_id != -1)
			k = DungeonInformation.dungeon_id;
		str body = "";
		for(j = 0; j < DungeonInformation.attrib_count; ++j)
			body = StrParam(s:body, s:DungeonAttributeString(DungeonInformation.attributes[j].attrib_id, DungeonInformation.attributes[j].attrib_val, 0, false, -1, false, 0, -1));
		body = StrParam(s:body, s:"\n");
		for(j = 0; j < DUN_UPSIDE_MAX; ++j)
			if(DungeonInformation.upside_vals[j])
				body = StrParam(s:body, s:DungeonUpsideString(j, DungeonInformation.upside_vals[j]), s:"\n");
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:"DND_MENU_DUNGEONS"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		if(k != -1)
			MchText("MENUBDYS", GetDungeonMonsterTypeString(k), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 78, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 120, MCH_PANE_TEXTW, MCH_PANE_FOOT - MCH_PANE_Y - 120, MCH_PANE_TEXTW);
		MchText("MENUBDYS", body, MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_Y + 122, 3);
		MenuClipRect(0, 0, 0, 0, 0);
	}
	else {
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:"DND_MENU_DUNGEONS"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:(k != -1) ? "DND_MCH_DUN_READY" : "DND_MCH_DUN_HOW"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MenuClipRect(0, 0, 0, 0, 0);
	}
	str hint = view ? "DND_MCH_HINT_DUNPICK" : "DND_MCH_HINT_DUN";
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:hint), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- gear: weapon discard

// Each starting slot weapon can be kept or discarded, and the temporary weapon to prefer cycles; the same nine boxes.
void DrawChromeGearDiscard(int pnum, int boxid) {
	mch_state_T module& s = GetMchState();
	int i, y;
	if(s.page != MENU_LOAD_WEAPONDISCARD) {
		s.page = MENU_LOAD_WEAPONDISCARD;
		s.row = 0;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	if(boxid != MAINBOX_NONE && boxid <= 9)
		s.row = boxid - 1;
	for(i = 0; i < 9; ++i) {
		y = GetChromeRowY(MENU_LOAD_WEAPONDISCARD, i);
		MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		str right;
		if(i < 8) {
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_DSLOT", d:i + 1)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
			if(HasPlayerDiscardedSlot(pnum, i))
				right = StrParam(s:"\c[UIRed]", l:"DND_MCH_DISCARDED");
			else
				right = StrParam(s:"\c[UIGreen]", l:"DND_MCH_KEPT");
		}
		else {
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MCH_TEMPWEP"), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
			int pref = GetPlayerPreferredTempWeapon(pnum);
			if(pref >= 0)
				right = StrParam(s:"\c[UIGold]", l:GetWeaponTag(pref + FIRST_SLOT9_WEAPON));
			else
				right = StrParam(s:"\c[UIGold]", l:(pref == -2) ? "DND_MCH_ANY" : "DND_MCH_NONE");
		}
		MchText("MENUBODY", right, MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
	}

	i = Clamp_Between(s.row, 0, 8);
	str head = (i < 8) ? StrParam(s:"DND_MCH_DSLOT", d:i + 1) : "DND_MCH_TEMPWEP";
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:head), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
	MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:(i < 8) ? "DND_MENU_DISCARD_EXPLANATION" : "DND_MCH_TEMPWEPDESC"), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:(i < 8) ? "DND_MCH_HINT_DISCARD" : "DND_MCH_HINT_TEMPWEP"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- gear: accessories

int MchEquippedAccessories() {
	int n = 0, cur = CheckInventory("Accessory_Index");
	for(; cur; cur >>= 1)
		n += cur & 1;
	return n;
}

// Five accessories a page and the page arrows, the same boxes as before; the pane describes the hovered one.
void DrawChromeGearAcc(int pnum, int page, int boxid, int mainboxid, int count) {
	mch_state_T module& s = GetMchState();
	int rows = count - GetChromePagerBoxes(page), first = ACCESSORY_PER_PAGE * (page - MENU_FIRST_ACCESSORY_PAGE), i, y;
	bool open = CheckResearchStatus(RES_ACCESSORY) == RES_DONE;
	if(s.page != page) {
		s.page = page;
		s.row = 0;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_EQUIPPED", s:"  \c[UIGold]", d:MchEquippedAccessories(), s:" / ", d:DND_ACCESSORY_BASELIMIT),
		MCH_ID_PAGETXT + 1, MCH_PANE_X + 976, 221, 2);
	if(boxid != MAINBOX_NONE && boxid <= rows)
		s.row = boxid - 1;
	for(i = 0; i < rows; ++i) {
		int rid = first + i;
		bool owned = open && CheckInventory(GetAccessoryName(rid));
		y = MCH_ROW_Y + MCH_ROW_STEP * i;
		MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : owned ? "N" : "L"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		if(!owned) {
			MchText("MENUBODY", StrParam(s:(boxid == i + 1) ? "\c[UIWhite]" : "\c[UIDim]", l:open ? "DND_MCH_NOTFOUNDACC" : "DND_MCH_NEEDSRESEARCH"), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
			continue;
		}
		MchPicScaled(GetAccessoryIcon(rid), MCH_ID_ROWIMG + i, MCH_ROW_X + 40, y + 27, 1.0);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:GetAccessoryText(rid, ACCESSORY_TAG)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 80, y + 27, 1);
		if(IsAccessoryEquipped(0, 1 << rid))
			MchText("MENUBODY", StrParam(s:"\c[UIGreen]", l:"DND_MCH_EQUIPPEDONE"), MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
	}
	DrawChromePager(page, boxid, mainboxid, rows);

	i = Clamp_Between(s.row, 0, Max(rows - 1, 0));
	int id = first + i;
	if(!open) {
		MchText("MENUHEAD", StrParam(s:"\c[UIDim]", l:"DND_MENU_ACCESSORIES"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MCH_ACCLOCKED"), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MenuClipRect(0, 0, 0, 0, 0);
		return;
	}
	if(!CheckInventory(GetAccessoryName(id))) {
		MchText("MENUHEAD", StrParam(s:"\c[UIDim]", l:"DND_MCH_NOTFOUNDACC"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MCH_ACCFIND"), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MenuClipRect(0, 0, 0, 0, 0);
		return;
	}
	bool on = IsAccessoryEquipped(0, 1 << id);
	MchPic("MCWELL", MCH_ID_PANEIMG + 20, MCH_PANE_IX, MCH_PANE_Y + 24);
	MchPicScaled(GetAccessoryIcon(id), MCH_ID_PANEIMG, MCH_PANE_IX + 130, MCH_PANE_Y + 109, 2.5);
	MchText("MENUHEAD", StrParam(s:on ? "\c[UIGreen]" : "\c[UIGold]", l:GetAccessoryText(id, ACCESSORY_TAG)), MCH_ID_PANETXT, MCH_PANE_TX, MCH_PANE_Y + 26, 3);
	MchText("MENUBDYS", StrParam(s:on ? "\c[UIGreen]" : "\c[UIGrey]", l:on ? "DND_MCH_EQUIPPEDONE" : "DND_MCH_NOTEQUIPPED"), MCH_ID_PANETXT + 1, MCH_PANE_TX, MCH_PANE_Y + 78, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 212, MCH_PANE_TEXTW, MCH_PANE_FOOT - MCH_PANE_Y - 212, MCH_PANE_TEXTW);
	MchText("MENUBDYS", StrParam(s:"\c[UIGreen]+  \c[UIWhite]", l:GetAccessoryText(id, ACCESSORY_PRO), s:"\n\n\c[UIRed]-  \c[UIWhite]", l:GetAccessoryText(id, ACCESSORY_CON)),
		MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_Y + 212, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_ACC"), MCH_ID_PANETXT + 3, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- gear: crafting

// The crafting pages answer the cursor with their own codes (rows, wells, buttons) and turn a click into what the
// server's crafting input already reads: a box in its range plus the hovered and the selected item's indices.
#define MCH_CR_ITEM 100
#define MCH_CR_MAT 200
#define MCH_CR_RECIPE 300
#define MCH_CR_APPLY 400
#define MCH_CR_SALVAGE 401
#define MCH_CR_TRANSMUTE 402
#define MCH_CR_EMPTY 403
#define MCH_CR_SOCKET 410
#define MCH_CR_FILTER 420
#define MCH_CR_PANEL 499

#define MCH_CR_LX -146
#define MCH_CR_LW 470
#define MCH_CR_LY 278
#define MCH_CR_ROWS 10
#define MCH_CR_RROWS 8
#define MCH_CR_MX 364
#define MCH_CR_MCX 717
#define MCH_CR_IMCX 756
#define MCH_CR_BTNY 840
#define MCH_CR_PX 1090
#define MCH_CR_PW 496
#define MCH_CR_PY 278
#define MCH_CR_GX 1118
#define MCH_CR_GY 352
#define MCH_CR_GCOLS 4
#define MCH_CR_GROWS 3
#define MCH_CR_GSTEP 98
#define MCH_CR_DY 724
#define MCH_CR_DLINES 5
#define MCH_CR_LINEH 27
#define MCH_CR_SX (MCH_CR_MCX - 272)
#define MCH_CR_SY 380
#define MCH_CR_GRIDY 306

typedef struct {
	int page;
	int pick;
	int sel;
	int pend;
	int filter;
	int mscroll;
	int lscroll;
	int dscroll;
	int dscroll_max;
	int hov_mat;
	int send_box;
	int send_main;
	int scroll_tic;
} mch_craft_T;

mch_craft_T module& GetMchCraft() {
	static mch_craft_T c;
	return c;
}

bool IsChromeCraftPage(int page) {
	return page >= MENU_LOAD_CRAFTING_FIRST && page <= MENU_LOAD_CRAFTING_LAST;
}

// the page's own state starts over when the page changes
mch_craft_T module& MchCraft(int page) {
	mch_craft_T module& c = GetMchCraft();
	if(c.page != page) {
		c.page = page;
		c.pick = -1;
		c.sel = -1;
		c.pend = 0;
		c.filter = 0;
		c.mscroll = 0;
		c.lscroll = 0;
		c.dscroll = 0;
		c.dscroll_max = 0;
		c.hov_mat = -1;
		c.send_main = 0;
	}
	return c;
}

// the materials a page lists, filtered: 0 all, 1 orbs, 2 tokens (gems on transmuting)
typedef struct {
	int n;
	int idx[MAX_UNIQUE_CRAFTING_TYPES];
} mch_craftlist_T;

mch_craftlist_T module& MchCraftMats(int pnum, int page, int filter) {
	static mch_craftlist_T l;
	bool tokens = IsCraftingPageForTokens(page), gems = page == MENU_LOAD_CRAFTING_TRANSMUTING && CheckResearchStatus(RES_GEMCRAFTING) == RES_DONE;
	int i, t, ty, n;
	ResetUniqueCraftingItemList();
	n = CountCraftingMaterials(tokens, gems);
	l.n = 0;
	for(i = 0; i < n && l.n < MAX_UNIQUE_CRAFTING_TYPES; ++i) {
		t = GetNextUniqueCraftableMaterial(i, tokens, gems);
		if(t == -1)
			break;
		ty = GlobalItemStorage.PlayerInventoryList[pnum][t].item_type & 0xFFFF;
		if((filter == 1 && ty != DND_ITEM_ORB) || (filter == 2 && ty == DND_ITEM_ORB))
			continue;
		l.idx[l.n++] = t;
	}
	return l;
}

int MchCraftMatAt(int pnum, int page, int k) {
	mch_craftlist_T module& l = MchCraftMats(pnum, page, GetMchCraft().filter);
	if(k < 0 || k >= l.n)
		return -1;
	return l.idx[k];
}

// the k-th craftable weapon's id
int MchCraftWeaponAt(int k) {
	for(int i = 0; i < MAXWEPS; ++i)
		if(IsCraftingWeapon(i) && !k--)
			return i;
	return -1;
}

bool MchIsCraftableAt(int pnum, int box) {
	return box >= 0 && box < MAX_INVENTORY_BOXES && IsCraftableItem(GlobalItemStorage.PlayerInventoryList[pnum][box].item_type) && GlobalItemStorage.PlayerInventoryList[pnum][box].height;
}

// the orbs a transmute recipe makes, in order; the k-th one, or -1
int MchRecipeAt(int k) {
	for(int i = 0; i < DND_MAX_ORB_KINDS; ++i)
		if(GetOrbRecipe(i).components[0].id != -1 && !k--)
			return i;
	return -1;
}

int MchRecipeCount() {
	int n = 0;
	for(int i = 0; i < DND_MAX_ORB_KINDS; ++i)
		n += GetOrbRecipe(i).components[0].id != -1;
	return n;
}

str MchOrbName(int o) {
	return GetInventoryTag(ORBS_BEGIN + o);
}

// What the cursor is over on a crafting page, as one of the codes above.
int MchCraftHit(int pnum, int page) {
	mch_craft_T module& c = MchCraft(page);
	int cx = MchCursorX(), cy = MchCursorY(), i, n;
	if(c.pend)
		--c.pend;
	if(MchIn(cx, cy, MCH_CR_PX, MCH_CR_PY, MCH_CR_PW, 616)) {
		for(i = 0; i < 3; ++i)
			if(MchIn(cx, cy, MCH_CR_PX + MCH_CR_PW - 268 + 80 * i, MCH_CR_PY + 16, 80, 32))
				return MCH_CR_FILTER + i;
		if(MchIn(cx, cy, MCH_CR_GX, MCH_CR_GY, MCH_CR_GCOLS * MCH_CR_GSTEP, MCH_CR_GROWS * MCH_CR_GSTEP)
			&& (cx - MCH_CR_GX) % MCH_CR_GSTEP < 86 && (cy - MCH_CR_GY) % MCH_CR_GSTEP < 86) {
			i = ((cy - MCH_CR_GY) / MCH_CR_GSTEP + c.mscroll) * MCH_CR_GCOLS + (cx - MCH_CR_GX) / MCH_CR_GSTEP;
			if(MchCraftMatAt(pnum, page, i) != -1)
				return MCH_CR_MAT + i;
		}
		return MCH_CR_PANEL;
	}
	if(page == MENU_LOAD_CRAFTING_TRANSMUTING) {
		for(i = 0; i < MAX_TRANSMUTE_BOXES; ++i)
			if(MchIn(cx, cy, MCH_CR_SX + 130 * i, MCH_CR_SY, 120, 120))
				return MCH_CR_SOCKET + i;
		if(MchIn(cx, cy, MCH_CR_MCX - 212, 700, MCH_TAB_W, MCH_TAB_H))
			return MCH_CR_TRANSMUTE;
		if(MchIn(cx, cy, MCH_CR_MCX + 12, 700, MCH_TAB_W, MCH_TAB_H))
			return MCH_CR_EMPTY;
		n = MchRecipeCount();
		for(i = 0; i < MCH_CR_RROWS && i + c.lscroll < n; ++i)
			if(MchIn(cx, cy, MCH_CR_LX, MCH_CR_LY + 52 + 70 * i, MCH_CR_LW, 62))
				return MCH_CR_RECIPE + i + c.lscroll;
		return MAINBOX_NONE;
	}
	if(page == MENU_LOAD_CRAFTING_WEAPON) {
		if(MchIn(cx, cy, MCH_CR_MCX - 100, MCH_CR_BTNY, MCH_TAB_W, MCH_TAB_H))
			return MCH_CR_APPLY;
		n = GetWeaponCount();
		for(i = 0; i < MCH_CR_ROWS && i + c.lscroll < n; ++i)
			if(MchIn(cx, cy, MCH_CR_LX, MCH_CR_LY + 62 * i, MCH_CR_LW, 54))
				return MCH_CR_ITEM + i + c.lscroll;
		return MAINBOX_NONE;
	}
	// inventory crafting: a craftable item answers by its top left box
	if(MchIn(cx, cy, MCH_CR_IMCX - 212, MCH_CR_BTNY, MCH_TAB_W, MCH_TAB_H))
		return MCH_CR_APPLY;
	if(MchIn(cx, cy, MCH_CR_IMCX + 12, MCH_CR_BTNY, MCH_TAB_W, MCH_TAB_H))
		return MCH_CR_SALVAGE;
	if(MchIn(cx, cy, MCH_CR_LX, MCH_CR_GRIDY, 64 * MAXINVENTORYBLOCKS_VERT, 72 * MAXINVENTORYBLOCKS_HORIZ)) {
		i = ((cy - MCH_CR_GRIDY) / 72) * MAXINVENTORYBLOCKS_VERT + (cx - MCH_CR_LX) / 64;
		n = GlobalItemStorage.PlayerInventoryList[pnum][i].topleftboxid - 1;
		if(MchIsCraftableAt(pnum, n))
			return MCH_CR_ITEM + n;
	}
	return MAINBOX_NONE;
}

int MchCraftApply(int pnum, int mat) {
	mch_craft_T module& c = GetMchCraft();
	c.send_box = MATERIALBOX_OFFSET_BOXID + 1;
	MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = (mat << DND_MENU_ITEMSAVEBITS1) | (Max(c.sel, 0) << DND_MENU_ITEMSAVEBITS2);
	return DND_MENUINPUT_RCLICK;
}

// A click on a crafting page becomes what the server reads, or is answered here and not sent (0).
int MchCraftInput(int pnum, int page, int input, int boxid) {
	mch_craft_T module& c = MchCraft(page);
	int selbox = CheckInventory("DnD_SelectedInventoryBox"), other = (selbox == 1) ? 2 : 1, it;
	bool l = input == DND_MENUINPUT_LCLICK;
	c.send_box = boxid;
	c.send_main = 0;
	MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = 0;
	if(input != DND_MENUINPUT_LCLICK && input != DND_MENUINPUT_RCLICK) {
		c.send_box = MAINBOX_NONE;
		return input;
	}
	// the salvage confirm: its buttons already became a right click (salvage) or a left click (cancel)
	if(CheckInventory("DnD_SellConfirm")) {
		c.send_box = 1;
		return input;
	}
	// a left click into nothing lets the server drop its selection, as it always did; on the panel it drops the pick
	if(boxid == MAINBOX_NONE)
		return input;
	if(boxid == MCH_CR_PANEL) {
		if(l && c.pick != -1) {
			c.pick = -1;
			LocalAmbientSound("RPG/MenuChoose", 127);
		}
		return 0;
	}
	if(boxid >= MCH_CR_FILTER && boxid < MCH_CR_FILTER + 3) {
		if(l) {
			c.filter = (boxid == MCH_CR_FILTER + 2) ? 0 : boxid - MCH_CR_FILTER + 1;
			c.mscroll = 0;
			LocalAmbientSound("RPG/MenuChoose", 127);
		}
		return 0;
	}
	if(page == MENU_LOAD_CRAFTING_TRANSMUTING) {
		c.send_box = MAINBOX_NONE;
		if(boxid >= MCH_CR_MAT && boxid < MCH_CR_RECIPE) {
			it = MchCraftMatAt(pnum, page, boxid - MCH_CR_MAT);
			if(!l || it == -1)
				return 0;
			c.send_main = MCH_NAV_BASE + MCH_EL_CRFILL + it;
			return input;
		}
		if(boxid >= MCH_CR_RECIPE && boxid < MCH_CR_APPLY) {
			it = MchRecipeAt(boxid - MCH_CR_RECIPE);
			if(!l || it == -1 || !CheckOrbRecipe(it))
				return 0;
			c.send_main = MCH_NAV_BASE + MCH_EL_CRRECIPE + it;
			return input;
		}
		// either click on a socket takes its material back out
		if(boxid >= MCH_CR_SOCKET && boxid < MCH_CR_SOCKET + MAX_TRANSMUTE_BOXES) {
			c.send_box = boxid - MCH_CR_SOCKET + 1;
			return DND_MENUINPUT_RCLICK;
		}
		if(boxid == MCH_CR_TRANSMUTE && l) {
			c.send_box = MBOX_4;
			return input;
		}
		if(boxid == MCH_CR_EMPTY && l) {
			c.send_main = MCH_NAV_BASE + MCH_EL_CREMPTY;
			return input;
		}
		return 0;
	}
	if(boxid >= MCH_CR_MAT && boxid < MCH_CR_RECIPE) {
		it = MchCraftMatAt(pnum, page, boxid - MCH_CR_MAT);
		if(it == -1)
			return 0;
		// left picks (or drops) the material, right applies it straight away as before
		if(l) {
			c.pick = (c.pick == it) ? -1 : it;
			LocalAmbientSound("RPG/MenuChoose", 127);
			return 0;
		}
		return MchCraftApply(pnum, it);
	}
	if(boxid == MCH_CR_APPLY) {
		if(!l || c.pick == -1)
			return 0;
		return MchCraftApply(pnum, c.pick);
	}
	if(boxid == MCH_CR_SALVAGE) {
		if(!l || c.sel == -1)
			return 0;
		c.send_box = other;
		MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = c.sel << DND_MENU_ITEMSAVEBITS1;
		return DND_MENUINPUT_RCLICK;
	}
	if(boxid >= MCH_CR_ITEM && boxid < MCH_CR_MAT) {
		it = (page == MENU_LOAD_CRAFTING_WEAPON) ? MchCraftWeaponAt(boxid - MCH_CR_ITEM) : boxid - MCH_CR_ITEM;
		if(it == -1 || (!l && page == MENU_LOAD_CRAFTING_WEAPON))
			return 0;
		// a box other than the selected one, which the server would ignore; a left click on the selection keeps it
		c.send_box = (l && it == c.sel && selbox) ? selbox : other;
		MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = (it << DND_MENU_ITEMSAVEBITS1) | (Max(c.sel, 0) << DND_MENU_ITEMSAVEBITS2);
		if(l) {
			c.sel = it;
			c.pend = 35;
		}
		return input;
	}
	return 0;
}

// The material an item card previews: the picked one, else the one under the cursor.
int MchCraftPreview(int hovered) {
	if(!IsChromeCraftPage(CheckInventory("MenuOption")))
		return hovered;
	mch_craft_T module& c = GetMchCraft();
	if(c.pick != -1)
		return c.pick;
	return c.hov_mat;
}

// the selection follows the server's: dropped once it dropped it (a click having had time to land), or the item is gone
void MchCraftSyncSelection(int pnum, int page) {
	mch_craft_T module& c = GetMchCraft();
	if(c.pick != -1 && GlobalItemStorage.PlayerInventoryList[pnum][c.pick].item_type == DND_ITEM_NULL)
		c.pick = -1;
	if(c.sel == -1 || c.pend)
		return;
	if(!CheckInventory("DnD_SelectedInventoryBox") && !CheckInventory("DnD_UsedTwoItemRequirementMaterial"))
		c.sel = -1;
	else if(page == MENU_LOAD_CRAFTING_INVENTORY && !MchIsCraftableAt(pnum, c.sel))
		c.sel = -1;
	else if(page == MENU_LOAD_CRAFTING_WEAPON && !IsCraftingWeapon(c.sel))
		c.sel = -1;
}

// a scroll bar of h pixels for a list showing shown of total from first
void MchCraftScrollBar(int slot, int id, int x, int y, int h, int first, int shown, int total) {
	MchScrollBar(slot, id, x, y, h, first, total - shown);
}

void MchCraftButton(int id, int x, int y, str label, bool over, bool live, int k) {
	MchPic(StrParam(s:over ? "MCTH" : "MCTO", d:k), id, x, y);
	MchText("MENUTAB", StrParam(s:(over && live) ? "\c[UIWhite]" : live ? "\c[UIGold]" : "\c[UIDim]", l:label), id - 1, x + MCH_TAB_W / 2, y + 26, 0);
}

// The materials panel: filters, a scrolling grid of wells with the counts etched into their corners, and the
// picked or hovered material named and explained below.
void MchCraftMaterials(int pnum, int page, int boxid) {
	mch_craft_T module& c = GetMchCraft();
	mch_craftlist_T module& l = MchCraftMats(pnum, page, c.filter);
	int B = MCH_ID_CRAFT_FIRST, i, k, rows = (l.n + MCH_CR_GCOLS - 1) / MCH_CR_GCOLS;
	MchPic("MCCRPN", B + 199, MCH_CR_PX, MCH_CR_PY);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MENU_MATERIALS"), B + 170, MCH_CR_GX, MCH_CR_PY + 32, 1);
	for(i = 0; i < 3; ++i) {
		str label = "DND_MCH_CR_ALL";
		if(!i)
			label = "DND_MCH_ORBS";
		else if(i == 1)
			label = (page == MENU_LOAD_CRAFTING_TRANSMUTING) ? "DND_MCH_CR_GEMS" : "DND_MCH_CR_TOKENS";
		bool on = c.filter == ((i == 2) ? 0 : i + 1);
		MchText("MENUBDYS", StrParam(s:on ? "\c[UIGold]" : (boxid == MCH_CR_FILTER + i) ? "\c[UIWhite]" : "\c[UIGrey]", l:label), B + 171 + i, MCH_CR_PX + MCH_CR_PW - 228 + 80 * i, MCH_CR_PY + 32, 0);
	}
	c.mscroll = Clamp_Between(c.mscroll, 0, Max(rows - MCH_CR_GROWS, 0));
	c.hov_mat = -1;
	for(k = 0; k < MCH_CR_GCOLS * MCH_CR_GROWS; ++k) {
		i = k + c.mscroll * MCH_CR_GCOLS;
		if(i >= l.n)
			break;
		int idx = l.idx[i], x = MCH_CR_GX + (k % MCH_CR_GCOLS) * MCH_CR_GSTEP, y = MCH_CR_GY + (k / MCH_CR_GCOLS) * MCH_CR_GSTEP;
		bool hov = boxid == MCH_CR_MAT + i;
		if(hov)
			c.hov_mat = idx;
		str st = (idx == c.pick) ? "S" : hov ? "H" : "N";
		MchPic(StrParam(s:"MCMWB", s:st), B + 45 + k, x, y);
		MchPicScaled(GetItemImage(GlobalItemStorage.PlayerInventoryList[pnum][idx].item_image), B + 30 + k, x + 43, y + 43, 2.0);
		MchPic(StrParam(s:"MCMWF", s:st), B + 15 + k, x, y);
		MchText("MENUNUM", StrParam(s:"\c[UIWhite]", s:MchStackText(GetTotalStackOfMaterial(idx))), B + k, x + 79, y + 69, 2);
	}
	MchCraftScrollBar(1, B + 174, MCH_CR_GX + 410, MCH_CR_GY, MCH_CR_GROWS * MCH_CR_GSTEP - 12, c.mscroll, MCH_CR_GROWS, rows);
	MenuClipRect(MCH_CR_GX, 658, MCH_CR_PW - 56, 4);
	MchPic("MCDIV", B + 198, MCH_CR_GX, 660);
	MenuClipRect(0, 0, 0, 0, 0);
	if(!l.n)
		MchText("MENUBDYS", StrParam(s:"\c[UIDim]", l:"DND_MCH_CR_NOMATS"), B + 176, MCH_CR_GX, MCH_CR_GY + 30, 1);

	// what the shown material is and does
	int show = (c.hov_mat != -1) ? c.hov_mat : c.pick;
	if(show == -1) {
		c.dscroll_max = 0;
		MchText("MENUBDYS", StrParam(s:"\c[UIDim]", l:(page == MENU_LOAD_CRAFTING_TRANSMUTING) ? "DND_MCH_CR_PICKT" : "DND_MCH_CR_PICKM"), B + 177, MCH_CR_GX, 690, 1);
		return;
	}
	int ty = GlobalItemStorage.PlayerInventoryList[pnum][show].item_type & 0xFFFF, sub = GlobalItemStorage.PlayerInventoryList[pnum][show].item_subtype;
	str name, body = "";
	if(ty == DND_ITEM_GEM) {
		name = GetGemFullName(sub, GlobalItemStorage.PlayerInventoryList[pnum][show].implicit[0].attrib_id);
		body = StrParam(l:"DND_MCH_CR_GEMBODY");
	}
	else {
		name = StrParam(l:GetInventoryTag(sub + GetInventoryInfoOffset(ty, sub)));
		body = StrParam(l:GetInventoryText(sub + GetInventoryInfoOffset(ty, sub)));
	}
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", s:name), B + 176, MCH_CR_GX, 690, 1);
	// a long description scrolls with the forward / back keys while the cursor is on the panel
	c.dscroll_max = Max(MchCountLines(body, 33) - MCH_CR_DLINES, 0);
	c.dscroll = Clamp_Between(c.dscroll, 0, c.dscroll_max);
	MenuClipRect(MCH_CR_GX, MCH_CR_DY, 400, MCH_CR_DLINES * MCH_CR_LINEH, 400);
	MchText("MENUBDYS", StrParam(s:"\c[UIWhite]", s:body), B + 178, MCH_CR_GX, MCH_CR_DY - MCH_CR_LINEH * c.dscroll, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchCraftScrollBar(2, B + 179, MCH_CR_GX + 410, MCH_CR_DY, MCH_CR_DLINES * MCH_CR_LINEH, c.dscroll, MCH_CR_DLINES, c.dscroll_max + MCH_CR_DLINES);
}

// The selected item's card, which the menu draws every tic at the card spot in the middle column.
void MchCraftCard(int pnum, int page) {
	mch_craft_T module& c = GetMchCraft();
	// the card's panel sits 16 units right of and below the cursor point it is given
	int cx = HUDMAX_XF - FixedDiv(((page == MENU_LOAD_CRAFTING_INVENTORY) ? MCH_CR_IMCX - 318 : MCH_CR_MCX - 318) << 16, MCH_SCALE_X) + 16.0, cy = HUDMAX_YF - FixedDiv(MCH_CR_LY << 16, MCH_SCALE_Y) + 16.0;
	if(c.sel == -1) {
		if(PlayerCursorData.itemClicked != -1)
			ResetCursorClickData();
		MchText("MENUBODY", StrParam(s:"\c[UIDim]", l:(page == MENU_LOAD_CRAFTING_WEAPON) ? "DND_MCH_CR_PICKW" : "DND_MCH_CR_PICKI"),
			MCH_ID_CRAFT_FIRST + 182, (page == MENU_LOAD_CRAFTING_INVENTORY) ? MCH_CR_IMCX : MCH_CR_MCX, 420, 0);
		return;
	}
	if(page == MENU_LOAD_CRAFTING_WEAPON)
		UpdateCursorClickData(c.sel, 0, DND_ITEM_WEAPON, cx, cy);
	else
		UpdateCursorClickData(c.sel, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, GlobalItemStorage.PlayerInventoryList[pnum][c.sel].item_type, cx, cy);
	MchCraftUseLine(pnum, page);
}

// Over the buttons: what the picked material will be used on, or that it cannot be; Enhancement's quality range.
void MchCraftUseLine(int pnum, int page) {
	mch_craft_T module& c = GetMchCraft();
	if(c.pick == -1)
		return;
	int ty = GlobalItemStorage.PlayerInventoryList[pnum][c.pick].item_type & 0xFFFF, sub = GlobalItemStorage.PlayerInventoryList[pnum][c.pick].item_subtype;
	int x = (page == MENU_LOAD_CRAFTING_INVENTORY) ? MCH_CR_IMCX : MCH_CR_MCX, itype = DND_ITEM_WEAPON, q;
	bool ok = true;
	if(page == MENU_LOAD_CRAFTING_INVENTORY)
		itype = GlobalItemStorage.PlayerInventoryList[pnum][c.sel].item_type;
	if(ty == DND_ITEM_ORB)
		ok = CanUseOrb(sub, c.sel, itype);
	else if(ty == DND_ITEM_TOKEN && page == MENU_LOAD_CRAFTING_INVENTORY)
		ok = CanUseToken(sub, c.sel, itype);
	str mat = StrParam(l:GetInventoryTag(sub + GetInventoryInfoOffset(ty, sub)));
	if(!ok) {
		MchText("MENUBDYS", StrParam(s:"\c[UIRed]", s:mat, s:" ", l:"DND_MCH_CR_CANT"), MCH_ID_CRAFT_FIRST + 181, x, MCH_CR_BTNY - 36, 0);
		return;
	}
	str on = (page == MENU_LOAD_CRAFTING_WEAPON) ? StrParam(l:GetWeaponTag(c.sel)) : StrParam(l:"DND_MCH_CR_THEITEM");
	MchText("MENUBDYS", StrParam(s:"\c[UICyan]", s:mat, s:" \c[UIGrey]", l:"DND_MCH_CR_ON", s:" \c[UIWhite]", s:on), MCH_ID_CRAFT_FIRST + 181, x, MCH_CR_BTNY - 36, 0);
	if(ty != DND_ITEM_ORB || sub != DND_ORB_ENHANCE || page != MENU_LOAD_CRAFTING_WEAPON)
		return;
	q = GetPlayerWeaponQuality(pnum, c.sel);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_CR_QUALITY", s:" \c[UIWhite]+", d:q, s:"%  \c[UIGrey]>  \c[UIGold]+", d:Min(q + QUALITY_ITEM_ADD_MIN * GetAffluenceBonus(), ENHANCEORB_MAX),
		s:"% \c[UIGrey]", l:"DND_MCH_CR_TO", s:" \c[UIGold]+", d:Min(q + QUALITY_ITEM_ADD_MAX * GetAffluenceBonus(), ENHANCEORB_MAX), s:"%"), MCH_ID_CRAFT_FIRST + 180, x, MCH_CR_BTNY - 70, 0);
}

void MchCraftWeapons(int pnum, int boxid) {
	mch_craft_T module& c = GetMchCraft();
	int B = MCH_ID_CRAFT_FIRST, n = GetWeaponCount(), i, w, y, q;
	c.lscroll = Clamp_Between(c.lscroll, 0, Max(n - MCH_CR_ROWS, 0));
	for(i = 0; i < MCH_CR_ROWS && i + c.lscroll < n; ++i) {
		w = MchCraftWeaponAt(i + c.lscroll);
		y = MCH_CR_LY + 62 * i;
		bool hov = boxid == MCH_CR_ITEM + i + c.lscroll;
		MchPic((w == c.sel || hov) ? "MCCRRS" : "MCCRRN", B + 120 + i, MCH_CR_LX - 8, y - 8);
		MchPicScaled(Weapons_Data[w].icon, B + 100 + i, MCH_CR_LX + 48, y + 27, 1.5);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:GetWeaponTag(w)), B + 60 + i, MCH_CR_LX + 100, y + 27, 1);
		q = GetPlayerWeaponQuality(pnum, w);
		MchText("MENUBDYS", StrParam(s:(w == c.sel || hov) ? "\c[UIWhite]" : q ? "\c[UIGold]" : "\c[UIDim]", s:"+", d:q, s:"%"), B + 80 + i, MCH_CR_LX + MCH_CR_LW - 20, y + 27, 2);
		if(hov && w != c.sel)
			UpdateCursorHoverData(w, 0, DND_ITEM_WEAPON, pnum, 0);
	}
	MchCraftScrollBar(0, B + 186, MCH_CR_LX + MCH_CR_LW + 10, MCH_CR_LY, 62 * MCH_CR_ROWS - 8, c.lscroll, MCH_CR_ROWS, n);
	MchCraftButton(B + 161, MCH_CR_MCX - 100, MCH_CR_BTNY, "DND_MCH_CR_APPLY", boxid == MCH_CR_APPLY, c.pick != -1 && c.sel != -1, 0);
}

// a cell's plate: S the hovered item, C the selected one, O any other item, "" empty
str MchCraftCellState(int pnum, int box, int boxid) {
	mch_craft_T module& c = GetMchCraft();
	int top = GlobalItemStorage.PlayerInventoryList[pnum][box].topleftboxid - 1;
	if(top < 0)
		return "";
	if(top == c.sel)
		return "C";
	if(boxid == MCH_CR_ITEM + top)
		return "S";
	return "O";
}

void MchCraftInventory(int pnum, int boxid) {
	mch_craft_T module& c = GetMchCraft();
	int B = MCH_ID_CRAFT_FIRST, r, col, box, top, mask;
	for(box = 0; box < MAX_INVENTORY_BOXES; ++box) {
		r = box / MAXINVENTORYBLOCKS_VERT;
		col = box % MAXINVENTORYBLOCKS_VERT;
		str st = MchCraftCellState(pnum, box, boxid), pic = "MCIQ";
		top = GlobalItemStorage.PlayerInventoryList[pnum][box].topleftboxid;
		if(StrLen(st)) {
			mask = 0;
			if(col > 0 && GlobalItemStorage.PlayerInventoryList[pnum][box - 1].topleftboxid == top)
				mask |= 1;
			if(col < MAXINVENTORYBLOCKS_VERT - 1 && GlobalItemStorage.PlayerInventoryList[pnum][box + 1].topleftboxid == top)
				mask |= 2;
			if(r > 0 && GlobalItemStorage.PlayerInventoryList[pnum][box - MAXINVENTORYBLOCKS_VERT].topleftboxid == top)
				mask |= 4;
			if(r < MAXINVENTORYBLOCKS_HORIZ - 1 && GlobalItemStorage.PlayerInventoryList[pnum][box + MAXINVENTORYBLOCKS_VERT].topleftboxid == top)
				mask |= 8;
			pic = StrParam(s:"MCIQ", s:st, x:mask);
		}
		MchPic(pic, B + 105 + box, MCH_CR_LX + 64 * col, MCH_CR_GRIDY + 72 * r);
		if(top != box + 1)
			continue;
		// the picture once, as the stash grid places it; items crafting cannot touch are dimmed
		int w = GlobalItemStorage.PlayerInventoryList[pnum][box].width, h = GlobalItemStorage.PlayerInventoryList[pnum][box].height;
		bool craftable = MchIsCraftableAt(pnum, box);
		MenuHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH);
		SetFont(GetItemImage(GlobalItemStorage.PlayerInventoryList[pnum][box].item_image, true));
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA, B + 60 + box, CR_WHITE,
			MenuX(((MCH_CR_LX + 64 * col) << 15) + 16.4 + (w - 1) * ITEMOFFSETSCALE),
			MenuY(FixedDiv((MCH_CR_GRIDY + 72 * r) << 16, 2.25) + 16.0 + (h - 1) * ITEMOFFSETSCALE), 0.0, craftable ? 1.0 : 0.3);
		MenuHudSize(MCH_W, MCH_H);
		if(craftable && boxid == MCH_CR_ITEM + box && box != c.sel)
			UpdateCursorHoverData(box, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, GlobalItemStorage.PlayerInventoryList[pnum][box].item_type, pnum, 0);
	}
	MchCraftButton(B + 161, MCH_CR_IMCX - 212, MCH_CR_BTNY, "DND_MCH_CR_APPLY", boxid == MCH_CR_APPLY, c.pick != -1 && c.sel != -1, 0);
	MchCraftButton(B + 163, MCH_CR_IMCX + 12, MCH_CR_BTNY, "DND_MCH_CR_SALVAGE", boxid == MCH_CR_SALVAGE, c.sel != -1, 2);
}

// a chamber socket's picture: an orb, a gem still in its box, or the empty mark once that gem is gone
str MchSocketImage(int pnum, int v) {
	if(IsTransmuteGemSlot(v)) {
		int g = GetTransmuteSlotGem(pnum, v);
		return (g != -1) ? GetItemImage(GetGemImageId(GetPlayerInventoryItem(pnum, g).item_subtype)) : "ORBBAK";
	}
	return GetItemImage(GetOrbItemImage(v));
}

void MchCraftTransmute(int pnum, int boxid) {
	mch_craft_T module& c = GetMchCraft();
	int B = MCH_ID_CRAFT_FIRST, n = MchRecipeCount(), i, j, o, y;
	auto d = GetTransmuteOrbData(pnum);
	// the recipe book: what each orb is made from once found; picking one fills the chamber
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_CR_RECIPES"), B + 183, MCH_CR_LX, MCH_CR_LY + 20, 1);
	c.lscroll = Clamp_Between(c.lscroll, 0, Max(n - MCH_CR_RROWS, 0));
	for(i = 0; i < MCH_CR_RROWS && i + c.lscroll < n; ++i) {
		o = MchRecipeAt(i + c.lscroll);
		y = MCH_CR_LY + 52 + 70 * i;
		bool known = CheckOrbRecipe(o);
		MchPic((known && boxid == MCH_CR_RECIPE + i + c.lscroll) ? "MCCRQS" : "MCCRQN", B + 120 + i, MCH_CR_LX - 8, y - 8);
		MchPicScaled(GetItemImage(GetOrbItemImage(o)), B + 100 + i, MCH_CR_LX + 36, y + 31, 1.5);
		MchText("MENUBODY", StrParam(s:known ? "\c[UIWhite]" : "\c[UIGrey]", l:MchOrbName(o)), B + 60 + i, MCH_CR_LX + 76, y + 19, 1);
		str parts = "";
		if(known) {
			for(j = 0; j < MAX_TRANSMUTE_BOXES; ++j) {
				int cid = GetOrbRecipe(o).components[j].id;
				if(cid == -1)
					break;
				parts = StrParam(s:parts, s:j ? "  +  " : "", d:GetOrbRecipe(o).components[j].count, s:" x ", l:MchOrbName(cid));
			}
		}
		else
			parts = StrParam(l:"DND_MCH_CR_UNKNOWN");
		MchText("MENUSUB", StrParam(s:"\c[UIGrey]", s:parts), B + 80 + i, MCH_CR_LX + 76, y + 44, 1);
	}
	MchCraftScrollBar(0, B + 186, MCH_CR_LX + MCH_CR_LW + 10, MCH_CR_LY + 52, 70 * MCH_CR_RROWS - 8, c.lscroll, MCH_CR_RROWS, n);

	// the chamber: three sockets, then what they make
	MchPic("MCCRPM", B + 197, MCH_CR_MCX - 330, MCH_CR_PY);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_CR_CHAMBER"), B + 184, MCH_CR_MCX, MCH_CR_SY - 40, 0);
	for(i = 0; i < MAX_TRANSMUTE_BOXES; ++i) {
		int x = MCH_CR_SX + 130 * i;
		MchPic((boxid == MCH_CR_SOCKET + i) ? "MCEQAH" : "MCEQA", B + 154 + i, x, MCH_CR_SY);
		if(d.val[i] != -1)
			MchPicScaled(MchSocketImage(pnum, d.val[i]), B + 150 + i, x + 60, MCH_CR_SY + 60, 2.0);
	}
	o = IsValidTransmuteRecipe(pnum);
	MchText("MENUHEAD", StrParam(s:(o >= 0) ? "\c[UIGold]" : "\c[UIDim]", s:"="), B + 187, MCH_CR_SX + 412, MCH_CR_SY + 60, 0);
	MchPic((o >= 0) ? "MCEQAH" : "MCEQA", B + 157, MCH_CR_SX + 434, MCH_CR_SY);
	str out = "";
	j = MCH_CR_SX + 494;
	if(o == DND_GEM_TRANSMUTE) {
		MchPicScaled("ORBBAKQ", B + 153, j, MCH_CR_SY + 60, 2.0);
		out = IsGreaterGem(GetPlayerInventoryItem(pnum, d.val[0] & DND_TRANSMUTE_GEMBOXMASK).item_subtype) ? "DND_RANDOM_GREATERGEM" : "DND_RANDOM_GEM";
	}
	else if(o == DND_RANDOM_TAGGED_ORB_TRANSMUTE) {
		MchPicScaled("ORBBAKQ", B + 153, j, MCH_CR_SY + 60, 2.0);
		out = "DND_RANDOM_TAGGED_ORB";
	}
	else if(o >= 0) {
		o = (o == DND_ELEMENTAL_TO_PRISMATIC) ? DND_ORB_PRISMATIC : o;
		MchPicScaled(GetItemImage(GetOrbItemImage(o)), B + 153, j, MCH_CR_SY + 60, 2.0);
		out = MchOrbName(o);
	}
	if(StrLen(out))
		MchText("MENUBDYS", StrParam(s:"\c[UIGold]", l:out), B + 188, j, MCH_CR_SY + 150, 0);
	MchText("MENUBODY", StrParam(s:"\c[UIGrey]", l:"DND_MCH_CR_COST", s:"  \c[UIGold]", s:MchMoney(DND_TRANSMUTE_COST)), B + 189, MCH_CR_MCX, 640, 0);
	MchCraftButton(B + 161, MCH_CR_MCX - 212, 700, "DND_MCH_CR_TRANSMUTE", boxid == MCH_CR_TRANSMUTE, o >= 0, 0);
	MchCraftButton(B + 163, MCH_CR_MCX + 12, 700, "DND_MCH_CR_EMPTY", boxid == MCH_CR_EMPTY, d.val[0] != -1 || d.val[1] != -1 || d.val[2] != -1, 1);
}

// One crafting page: its list, grid or recipe book, the middle column and the materials panel.
void DrawChromeCraft(int pnum, int page, int boxid) {
	DeleteTextRange(MCH_ID_CRAFT_FIRST, MCH_ID_CRAFT_LAST);
	MchCraft(page);
	MchCraftSyncSelection(pnum, page);
	// a card under the cursor is only for items: the panel says what a material is
	if((boxid < MCH_CR_ITEM || boxid >= MCH_CR_MAT) && PlayerCursorData.itemHovered != -1)
		ResetCursorHoverData();
	MchCraftMaterials(pnum, page, boxid);
	if(page == MENU_LOAD_CRAFTING_WEAPON)
		MchCraftWeapons(pnum, boxid);
	else if(page == MENU_LOAD_CRAFTING_INVENTORY)
		MchCraftInventory(pnum, boxid);
	else
		MchCraftTransmute(pnum, boxid);
	if(page != MENU_LOAD_CRAFTING_TRANSMUTING)
		MchCraftCard(pnum, page);
	str hint = "DND_MCH_HINT_CRT";
	if(page == MENU_LOAD_CRAFTING_WEAPON)
		hint = "DND_MCH_HINT_CRW";
	else if(page == MENU_LOAD_CRAFTING_INVENTORY)
		hint = "DND_MCH_HINT_CRI";
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:hint), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);
}

// Forward / back scroll whichever part the cursor is on: the list, the materials grid or the description.
bool MchCraftListening(int page) {
	mch_craft_T module& c = MchCraft(page);
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS), d = 0, cx = MchCursorX(), cy = MchCursorY(), v;
	bool r = MchSbarHover();
	v = MchScrollDrag(0, c.lscroll);
	r |= v != c.lscroll;
	c.lscroll = v;
	v = MchScrollDrag(1, c.mscroll);
	r |= v != c.mscroll;
	c.mscroll = v;
	v = MchScrollDrag(2, c.dscroll);
	r |= v != c.dscroll;
	c.dscroll = v;
	if(MchSbarGrabbed())
		return r;
	if(IsButtonHeld(bpress, BT_FORWARD))
		d = -1;
	else if(IsButtonHeld(bpress, BT_BACK))
		d = 1;
	if(!d) {
		c.scroll_tic = 0;
		return r;
	}
	if(c.scroll_tic-- > 0)
		return r;
	c.scroll_tic = 3;
	if(cx >= MCH_CR_PX) {
		if(cy >= 660) {
			v = Clamp_Between(c.dscroll + d, 0, c.dscroll_max);
			if(v == c.dscroll)
				return r;
			c.dscroll = v;
			return true;
		}
		v = Max(c.mscroll + d, 0);
		if(v == c.mscroll)
			return r;
		c.mscroll = v;
		return true;
	}
	if(cx < MCH_CR_MX && page != MENU_LOAD_CRAFTING_INVENTORY) {
		v = Max(c.lscroll + d, 0);
		if(v == c.lscroll)
			return r;
		c.lscroll = v;
		return true;
	}
	return r;
}

// Server: a material into the first empty chamber socket, as picking it then a socket did.
void MchTransmuteFill(int pnum, int idx) {
	auto d = GetTransmuteOrbData(pnum);
	int s, temp, ty;
	if(idx < 0 || idx >= MAX_INVENTORY_BOXES)
		return;
	for(s = 0; s < MAX_TRANSMUTE_BOXES && d.val[s] != -1; ++s);
	if(s == MAX_TRANSMUTE_BOXES)
		return;
	ty = GlobalItemStorage.PlayerInventoryList[pnum][idx].item_type & 0xFFFF;
	temp = GlobalItemStorage.PlayerInventoryList[pnum][idx].item_subtype;
	if(ty == DND_ITEM_GEM) {
		if(CheckResearchStatus(RES_GEMCRAFTING) != RES_DONE)
			return;
		temp = DND_TRANSMUTE_GEMSLOT | idx;
	}
	else if(ty != DND_ITEM_ORB)
		return;
	d.val[s] = temp;
	SendOwnerScript("DnD Transmute Orb Sync", pnum, pnum, s, temp);
	LocalAmbientSound("RPG/MenuChoose", 127);
}

// Server: a found recipe's orbs into the chamber, in order; transmuting checks they are carried.
void MchTransmuteRecipe(int pnum, int orb) {
	if(orb < 0 || orb >= DND_MAX_ORB_KINDS || !CheckOrbRecipe(orb))
		return;
	auto d = GetTransmuteOrbData(pnum);
	int s = 0, j, k, cid, cnt;
	ResetTransmutingData(pnum);
	for(j = 0; j < MAX_TRANSMUTE_BOXES; ++j) {
		cid = GetOrbRecipe(orb).components[j].id;
		cnt = GetOrbRecipe(orb).components[j].count;
		if(cid == -1)
			break;
		for(k = 0; k < cnt && s < MAX_TRANSMUTE_BOXES; ++k) {
			d.val[s] = cid;
			SendOwnerScript("DnD Transmute Orb Sync", pnum, pnum, s, cid);
			++s;
		}
	}
	LocalAmbientSound("RPG/MenuChoose", 127);
}

// ---------------------------------------------------------------- the sell confirm popup

int GetChromePopupBox() {
	int cx = MchCursorX(), cy = MchCursorY();
	if(MchIn(cx, cy, MCH_POP_YESX, MCH_POP_BTNY, MCH_TAB_W, MCH_TAB_H))
		return MCH_NAV_BASE + MCH_EL_POPYES;
	if(MchIn(cx, cy, MCH_POP_NOX, MCH_POP_BTNY, MCH_TAB_W, MCH_TAB_H))
		return MCH_NAV_BASE + MCH_EL_POPNO;
	return MAINBOX_NONE;
}

// What the server is sent as mainboxid: the left / right keys never carry a button, so they reach the page wherever
// the cursor rests; chrome navigation only rides a left click; popup buttons are never navigation.
int GetChromeSendMain(int mainboxid, int input) {
	if(input == DND_MENUINPUT_PREVBUTTON || input == DND_MENUINPUT_NEXTBUTTON)
		return MAINBOX_NONE;
	if(mainboxid == MCH_NAV_BASE + MCH_EL_POPYES || mainboxid == MCH_NAV_BASE + MCH_EL_POPNO)
		return MAINBOX_NONE;
	if(mainboxid >= MCH_NAV_BASE && input != DND_MENUINPUT_LCLICK)
		return MAINBOX_NONE;
	return mainboxid;
}

// Clicks the client answers itself: a research row picks the entry the keys would step to, and is not sent.
int HandleChromeClientInput(int page, int input, int mainboxid) {
	if(CheckInventory("DnD_SellConfirm"))
		return HandleChromeSellPopupInput(input, mainboxid);
	if(GetChromeKind(page) == MCH_KIND_RESLIST && mainboxid >= MCH_NAV_BASE + MCH_EL_ROW && mainboxid < MCH_NAV_BASE + MCH_EL_ROW + MENU_MAXRES_PERPAGE) {
		if(input == DND_MENUINPUT_LCLICK) {
			SetInventory("MenuPosX", mainboxid - MCH_NAV_BASE - MCH_EL_ROW);
			LocalAmbientSound("RPG/MenuChoose", 127);
		}
		return 0;
	}
	return input;
}

// Yes becomes the right click that confirmed a sale before; any other left click cancels, as it always did.
int HandleChromeSellPopupInput(int input, int mainboxid) {
	if(input != DND_MENUINPUT_LCLICK)
		return 0;
	// hidden from the click on, so it does not flash back before the server's answer lands
	GetMchState().popanswered = true;
	if(mainboxid == MCH_NAV_BASE + MCH_EL_POPYES)
		return DND_MENUINPUT_RCLICK;
	return DND_MENUINPUT_LCLICK;
}

bool ChromeSellPopupShown() {
	mch_state_T module& s = GetMchState();
	if(!CheckInventory("DnD_SellConfirm"))
		s.popanswered = false;
	return CheckInventory("DnD_SellConfirm") && !s.popanswered
		&& (CheckInventory("DnD_ShowSellPopup") || (IsChromeCraftPage(CheckInventory("MenuOption")) && CheckInventory("DnD_PopupID") == POPUP_SCAVENGECONFIRM));
}

void DrawChromeSellPopup(int mainboxid) {
	int item = CheckInventory("DnD_PopupID");
	MenuHudSize(MCH_W, MCH_H);
	MchPicScaled("MCDIM", MCH_ID_POPIMG + 3, MCH_W / 2, MCH_H / 2, 120.0);
	MchPic("MCPOPUP", MCH_ID_POPIMG + 2, MCH_POP_X, MCH_POP_Y);
	bool salvage = IsChromeCraftPage(CheckInventory("MenuOption"));
	if(salvage) {
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:"DND_MCH_CR_SALVTITLE"), MCH_ID_POPTXT, MCH_POP_X + MCH_POP_W / 2, MCH_POP_Y + 56, 0);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MCH_CR_SALVASK", s:" \c[UIGold]", s:MchMoney(CheckInventory("DnD_ItemPriceTemp")), s:"\c[UIWhite]?\n\c[UIGrey]",
			l:"DND_DISASS_CHANCE", s:" \c[UICyan]", d:CheckInventory("DnD_ItemCursorMsg") >> 16, s:"%"), MCH_ID_POPTXT + 1, MCH_POP_X + MCH_POP_W / 2, MCH_POP_Y + 148, 0);
	}
	else {
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:"DND_MCH_SELLTITLE"), MCH_ID_POPTXT, MCH_POP_X + MCH_POP_W / 2, MCH_POP_Y + 56, 0);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MCH_SELLASK", s:" \c[UICyan]", l:GetWeaponTag(ShopTableIdToWeaponTableId(item)), s:"\c[UIWhite]?\n\c[UIGrey]",
			l:"DND_MCH_SELLGET", s:" \c[UIGold]", s:MchMoney(GetShopPrice(item) / 2)), MCH_ID_POPTXT + 1, MCH_POP_X + MCH_POP_W / 2, MCH_POP_Y + 148, 0);
	}

	bool yes = mainboxid == MCH_NAV_BASE + MCH_EL_POPYES, no = mainboxid == MCH_NAV_BASE + MCH_EL_POPNO;
	MchPic(yes ? "MCTH0" : "MCTO0", MCH_ID_POPIMG, MCH_POP_YESX, MCH_POP_BTNY);
	MchText("MENUTAB", StrParam(s:yes ? "\c[UIWhite]" : "\c[UIGold]", l:salvage ? "DND_MCH_CR_SALVAGE" : "DND_MCH_SELLYES"), MCH_ID_POPTXT + 2, MCH_POP_YESX + MCH_TAB_W / 2, MCH_POP_BTNY + 26, 0);
	MchPic(no ? "MCTH1" : "MCTO1", MCH_ID_POPIMG + 1, MCH_POP_NOX, MCH_POP_BTNY);
	MchText("MENUTAB", StrParam(s:no ? "\c[UIWhite]" : "\c[UIGrey]", l:"DND_MCH_CANCEL"), MCH_ID_POPTXT + 3, MCH_POP_NOX + MCH_TAB_W / 2, MCH_POP_BTNY + 26, 0);
}

// ---------------------------------------------------------------- the perk pages

#define MCH_PERK_LISTBOT 880
#define MCH_PERK_CATGAP 16
#define MCH_PERK_ROWS 10
#define MCH_PERK_INDENT 56
#define MCH_PERK_BODYTOP (MCH_PANE_Y + 120)
#define MCH_PERK_COLS 80

// whether the list fits from entry first on, category gaps included
bool MchPerkListFits(perk_arch_list_T module& list, int first) {
	int y = MCH_ROW_Y, n = 0;
	for(int i = first; i < list.count; ++i) {
		if(list.newcat[i] && i > first)
			y += MCH_PERK_CATGAP;
		if(y + MCH_ROW_H > MCH_PERK_LISTBOT || ++n > MCH_PERK_ROWS)
			return false;
		y += MCH_ROW_STEP;
	}
	return true;
}

// whether another child at depth lv follows entry i under the same parent
bool MchPerkHasLaterSibling(perk_arch_list_T module& list, int i, int lv) {
	for(int j = i + 1; j < list.count; ++j) {
		if(list.newcat[j] || list.depth[j] < lv)
			return false;
		if(list.depth[j] == lv)
			return true;
	}
	return false;
}

int MchPerkScrollMax(perk_arch_list_T module& list) {
	int first = 0;
	while(first < list.count && !MchPerkListFits(list, first))
		++first;
	return first;
}

str MchPerkRankCol(int pts, int mx) {
	if(pts >= mx)
		return "\c[UIGreen]";
	if(pts)
		return "\c[UIGold]";
	return "\c[UIGrey]";
}

// the perks of an archetype that hold points, one per line
str MchPerkArchSummary(int pnum, int arch) {
	perk_arch_list_T module& list = GetPerkArchList(arch);
	str out = "";
	for(int i = 0; i < list.count; ++i) {
		int pk = list.perk[i], pts = GetPerkPoints(pnum, pk);
		if(pts)
			out = StrParam(s:out, s:"\c[UIWhite]", l:GetPerkLangKey(pk), s:"  ", s:MchPerkRankCol(pts, PerkTable[pk].max_points), d:pts, s:" / ", d:PerkTable[pk].max_points, s:"\n");
	}
	if(!StrLen(out))
		return StrParam(s:"\c[UIDim]", l:"DND_MCH_PERK_NONE");
	return out;
}

void MchPerkArchPane(int pnum, int arch) {
	perk_arch_list_T module& list = GetPerkArchList(arch);
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:GetPerkArchLangKey(arch)), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_PERKSSPENT", s:" \c[UIWhite]", d:GetPerkSpentIn(pnum, arch), s:" / ", d:list.maxpoints,
		s:"      \c[UIGrey]", l:"DND_MCH_PERK_AVAIL", s:" \c[UIGold]", d:PlayerModData[pnum].unspent), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 78, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PERK_BODYTOP, MCH_PANE_TEXTW, MCH_PANE_FOOT - 12 - MCH_PERK_BODYTOP, MCH_PANE_TEXTW);
	MchText("MENUBDYS", MchPerkArchSummary(pnum, arch), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PERK_BODYTOP, 3);
	MenuClipRect(0, 0, 0, 0, 0);
}

void DrawChromePerks(int pnum, int page, int boxid, menu_pane_T module& p) {
	EnsurePerkArchLists();
	mch_state_T module& s = GetMchState();
	if(s.page != page) {
		s.page = page;
		s.row = 0;
		s.scroll = 0;
		s.scroll_max = 0;
		s.pscroll = 0;
		s.pscroll_max = 0;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_PERK_AVAIL", s:"  \c[UIGold]", d:PlayerModData[pnum].unspent), MCH_ID_PAGETXT + 1, MCH_PANE_X + 976, 221, 2);
	if(GetChromeKind(page) == MCH_KIND_PERKTREE) {
		DrawChromePerkTree(pnum, page - MENU_PERKTREE_FIRST, boxid, p);
		return;
	}

	// the archetypes, then the dash toggle below a gap
	int i, y;
	if(boxid != MAINBOX_NONE && boxid <= PERK_ARCH_COUNT + 1)
		s.row = boxid - 1;
	for(i = 0; i <= PERK_ARCH_COUNT; ++i) {
		y = GetChromeRowY(page, i);
		bool empty = i < PERK_ARCH_COUNT && !GetPerkArchList(i).count;
		MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : empty ? "L" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		if(i == PERK_ARCH_COUNT) {
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MENU_DASH"), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
			if(CheckInventory("DashDisabled"))
				MchText("MENUBODY", StrParam(s:"\c[UIRed]", l:"DND_MCH_OFF"), MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
			else
				MchText("MENUBODY", StrParam(s:"\c[UIGreen]", l:"DND_MCH_ON"), MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
			continue;
		}
		MchText("MENUBODY", StrParam(s:empty ? "\c[UIDim]" : "\c[UIWhite]", l:GetPerkArchLangKey(i)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
		if(empty)
			MchText("MENUBODY", StrParam(s:"\c[UIDim]", l:"DND_MENU_PERK_EMPTY"), MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
		else {
			int spent = GetPerkSpentIn(pnum, i);
			MchText("MENUBODY", StrParam(s:spent ? "\c[UIGold]" : "\c[UIGrey]", d:spent, s:" / ", d:GetPerkArchList(i).maxpoints),
				MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
		}
	}

	i = Clamp_Between(s.row, 0, PERK_ARCH_COUNT);
	if(i == PERK_ARCH_COUNT) {
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:"DND_MENU_DASH"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MCH_DASHDESC"), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MenuClipRect(0, 0, 0, 0, 0);
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_TOGGLE"), MCH_ID_PANETXT + 3, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
		return;
	}
	MchPerkArchPane(pnum, i);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_OPEN"), MCH_ID_PANETXT + 3, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// Rebuilds the pane each redraw: entry n is box n (off the list it gets an empty rect), the back arrow last, parked.
void DrawChromePerkTree(int pnum, int arch, int boxid, menu_pane_T module& p) {
	perk_arch_list_T module& list = GetPerkArchList(arch);
	perk_scroll_T module& ps = GetPerkScroll();
	mch_state_T module& s = GetMchState();
	if(ps.shown_arch != arch) {
		ps.shown_arch = arch;
		ps.shown_perk = -1;
		ps.pinned = false;
	}
	s.scroll_max = MchPerkScrollMax(list);
	s.scroll = Clamp_Between(s.scroll, 0, s.scroll_max);

	bool blocked = CheckInventory("DnD_HighlightBlocker"), full = false;
	int i, y = MCH_ROW_Y, n = 0, hovered = -1, last = -1, conn = 0, cats = 0;
	ResetPane(p);
	for(i = 0; i < list.count; ++i) {
		if(i < s.scroll || full) {
			AddBoxToPane_Points(p, -1, -1, -1, -1);
			continue;
		}
		if(list.newcat[i] && i > s.scroll) {
			y += MCH_PERK_CATGAP;
			if(cats < 4 && y + MCH_ROW_H <= MCH_PERK_LISTBOT) {
				MenuClipRect(MCH_ROW_X, y - 14, MCH_ROW_W, 2);
				MchPic("MCDIV", MCH_ID_PAGEIMG + 1 + cats, MCH_ROW_X, y - 14);
				MenuClipRect(0, 0, 0, 0, 0);
				++cats;
			}
		}
		if(y + MCH_ROW_H > MCH_PERK_LISTBOT || n >= MCH_PERK_ROWS) {
			full = true;
			AddBoxToPane_Points(p, -1, -1, -1, -1);
			continue;
		}
		MchAddBox(p, MCH_ROW_X, y, MCH_ROW_W, MCH_ROW_H);

		int pk = list.perk[i], pts = GetPerkPoints(pnum, pk), mx = PerkTable[pk].max_points, blocker = GetPerkSpendBlocker(pnum, pk);
		bool locked = !pts && (blocker == PERKBLOCK_THRESHOLD || blocker == PERKBLOCK_REQUIRES);
		bool hot = boxid == i + 1 && !blocked;
		if(hot)
			hovered = i;
		// a plain list; the hovered row's plate starts at its name, so the arrow points into its notch
		int x = MCH_ROW_X + 36 + MCH_PERK_INDENT * list.depth[i];
		if(hot)
			MchPic(StrParam(s:"MCRWS", d:Min(list.depth[i], 5)), MCH_ID_ROWIMG + 12 + n, x - 30, y - 8);
		// tree lines: trunks of ancestors that still have children below, then this row's elbow or tee
		for(int lv = 1; lv <= list.depth[i]; ++lv) {
			bool more = MchPerkHasLaterSibling(list, i, lv);
			str pic = "MCTRV";
			if(lv == list.depth[i])
				pic = StrParam(s:more ? "MCTRT" : "MCTRL", d:pts || IsPerkRequirementMet(pnum, pk));
			else if(!more)
				continue;
			if(conn < 22)
				MchPic(pic, MCH_ID_PANEIMG + conn, MCH_ROW_X + 34 + MCH_PERK_INDENT * (lv - 1), y - 18);
			else if(conn < 34)
				MchPic(pic, MCH_ID_ROWIMG + conn - 22, MCH_ROW_X + 34 + MCH_PERK_INDENT * (lv - 1), y - 18);
			++conn;
		}
		// dim text would vanish on the highlight, so a hovered locked row reads white
		bool dim = locked && !hot;
		MchText("MENUBODY", StrParam(s:(pts >= mx) ? "\c[UIGreen]" : dim ? "\c[UIDim]" : "\c[UIWhite]", l:GetPerkLangKey(pk)), MCH_ID_ROWTXT + 3 * n, x, y + 27, 1);
		MchText("MENUBODY", StrParam(s:dim ? "\c[UIDim]" : locked ? "\c[UIWhite]" : MchPerkRankCol(pts, mx), d:pts, s:" / ", d:mx), MCH_ID_ROWTXT + 3 * n + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
		last = i;
		y += MCH_ROW_STEP;
		++n;
	}
	MchAddBox(p, -5000, -5000, 1, 1);

	MchScrollBar(0, MCH_ID_PANEIMG + 24, MCH_ROW_X + MCH_ROW_W + 10, MCH_ROW_Y, 620, s.scroll, s.scroll_max);
	if(s.scroll_max) {
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_SHOWING", s:" ", d:s.scroll + 1, s:"-", d:last + 1, s:" / ", d:list.count, s:"    \c[UIDim]", l:"DND_MCH_SCROLLHINT"),
			MCH_ID_PAGETXT + 2, MCH_ROW_X + MCH_ROW_W / 2, MCH_PAGER_Y + 26, 0);
	}

	// the pane keeps the last perk hovered so the cursor can travel to it; a pin holds it
	if(!ps.pinned && hovered != -1 && ps.shown_perk != list.perk[hovered]) {
		ps.shown_perk = list.perk[hovered];
		s.pscroll = 0;
	}
	if(ps.shown_perk == -1) {
		MchPerkArchPane(pnum, arch);
		s.pscroll_max = 0;
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_PERK_PICK"), MCH_ID_PANETXT + 4, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
		return;
	}
	DrawChromePerkPane(pnum, ps.shown_perk, ps.pinned);
}

void DrawChromePerkPane(int pnum, int pk, bool pinned) {
	mch_state_T module& s = GetMchState();
	int pts = GetPerkPoints(pnum, pk), mx = PerkTable[pk].max_points, blocker = GetPerkSpendBlocker(pnum, pk);
	str key = GetPerkLangKey(pk), col = "\c[UIWhite]", status = "";
	if(pts >= mx)
		col = "\c[UIGreen]";
	else if(!pts && (blocker == PERKBLOCK_THRESHOLD || blocker == PERKBLOCK_REQUIRES))
		col = "\c[UIDim]";
	MchText("MENUHEAD", StrParam(s:col, l:key), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);

	if(blocker == PERKBLOCK_MAXED)
		status = StrParam(s:"\c[UIGreen]", l:"DND_MENU_PERK_MAXED");
	else if(blocker == PERKBLOCK_THRESHOLD)
		status = StrParam(s:"\c[UIRed]", l:"DND_MENU_PERK_NEEDSPENT", s:": ", d:PerkTable[pk].threshold);
	else if(blocker == PERKBLOCK_REQUIRES)
		status = StrParam(s:"\c[UIRed]", l:"DND_MENU_PERK_LOCKED");
	else if(blocker == PERKBLOCK_NOPOINTS)
		status = StrParam(s:"\c[UIRed]", l:"DND_MENU_PERK_NOPOINTS");
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_PERK_RANK", s:" ", s:MchPerkRankCol(pts, mx), d:pts, s:" / ", d:mx, s:"      ", s:status),
		MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 78, 3);

	str body = StrParam(s:"\c[UIWhite]", l:StrParam(s:key, s:"_DESC"));
	if(mx > 1)
		body = StrParam(s:body, s:"\n\n\c[UIGold]", l:"DND_MENU_PERK_PERPOINT", s:": \c[UIWhite]", l:StrParam(s:key, s:"_PER"));
	if(PerkTable[pk].req_kind != PERK_REQ_NONE)
		body = StrParam(s:body, s:"\n\n\c[UIGold]", l:"DND_MENU_PERK_REQUIRES", s:": \c[UIWhite]", l:StrParam(s:key, s:"_REQ"));
	int view = (MCH_PANE_FOOT - 12 - MCH_PERK_BODYTOP) / MCH_STAT_LINEH;
	s.pscroll_max = Max(MchCountLines(body, MCH_PERK_COLS) - view, 0);
	s.pscroll = Clamp_Between(s.pscroll, 0, s.pscroll_max);
	MenuClipRect(MCH_PANE_IX, MCH_PERK_BODYTOP, MCH_PANE_TEXTW - 24, view * MCH_STAT_LINEH, MCH_PANE_TEXTW - 24);
	MchText("MENUBDYS", body, MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PERK_BODYTOP - MCH_STAT_LINEH * s.pscroll, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchScrollBar(1, MCH_ID_PANEIMG + 22, MCH_PANE_X + 954, MCH_PERK_BODYTOP, view * MCH_STAT_LINEH, s.pscroll, s.pscroll_max);

	str refund = "";
	int rs = GetPerkRefundStatus(pnum, pk);
	if(rs == PERKREFUND_NONE)
		refund = StrParam(s:"\c[UIGrey]", l:"DND_MCH_PERK_REFUNDCOST", s:" \c[UIGold]", s:MchMoney(GetPerkRespecCost(pnum, pk)));
	else if(rs == PERKREFUND_LOCKS)
		refund = StrParam(s:"\c[UIRed]", l:"DND_MENU_PERK_REFUNDLOCKS", s:" \c[UIWhite]", l:GetPerkLangKey(GetPerkRefundBlocker(pnum, pk)));
	else if(rs == PERKREFUND_POOR)
		refund = StrParam(s:"\c[UIRed]", l:"DND_MENU_PERK_REFUNDPOOR", s:" (", s:MchMoney(GetPerkRespecCost(pnum, pk)), s:")");
	MchText("MENUBDYS", refund, MCH_ID_PANETXT + 3, MCH_PANE_IX, MCH_PANE_FOOT + 18, 1);
	if(pinned)
		MchText("MENUSUB", StrParam(s:"\c[UICyan]", l:"DND_MENU_PERK_PINNED1", s:"  \c[UIDim]", l:"DND_MCH_PERK_UNPIN"), MCH_ID_PANETXT + 4, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
	else
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_PERK", s:"    ", l:"DND_MENU_PERK_PINHELP1", s:" \c[UIGrey]", k:"+jump", s:"\c[UIDim] ", l:"DND_MCH_PERK_PINHINT"),
			MCH_ID_PANETXT + 4, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// Forward / back scroll a redesigned page's long text, a line every few tics while held.
bool HandleChromeListening(int page, int boxid) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_CRAFT)
		return MchCraftListening(page);
	if(k != MCH_KIND_STATS && k != MCH_KIND_PERKTREE && k != MCH_KIND_RESLIST && k != MCH_KIND_CODEX && !IsChromeShopList(page))
		return false;
	mch_state_T module& s = GetMchState();
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS), d = 0;
	bool r = MchSbarHover();
	int v = MchScrollDrag(0, s.scroll);
	r |= v != s.scroll;
	s.scroll = v;
	v = MchScrollDrag(1, s.pscroll);
	r |= v != s.pscroll;
	s.pscroll = v;
	if(MchSbarGrabbed())
		return r;
	if(IsButtonHeld(bpress, BT_FORWARD))
		d = -1;
	else if(IsButtonHeld(bpress, BT_BACK))
		d = 1;
	if(!d) {
		s.scroll_tic = 0;
		return r;
	}
	if(s.scroll_tic-- > 0)
		return r;
	s.scroll_tic = 2;
	// a perk tree scrolls whichever side the cursor is on: the list, or the pane's description
	if(k == MCH_KIND_CODEX || (k == MCH_KIND_PERKTREE && MchCursorX() >= MCH_PANE_X)) {
		int m = Clamp_Between(s.pscroll + d, 0, s.pscroll_max);
		if(m == s.pscroll)
			return r;
		s.pscroll = m;
		return true;
	}
	int n = Clamp_Between(s.scroll + d, 0, s.scroll_max);
	if(n == s.scroll)
		return r;
	s.scroll = n;
	return true;
}

#endif
