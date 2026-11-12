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
// sub tabs: labels TABTXT + 10.., plates TABIMG + 14.., MCH_MAX_SUBS of each
#define MCH_MAX_SUBS		6
#define MCH_ID_PANEIMG		1862
#define MCH_ID_ROWIMG		1892
#define MCH_ID_CHIPIMG		1916
#define MCH_ID_PAGEIMG		1926
#define MCH_ID_BADGEIMG		1934
#define MCH_ID_TABIMG		1938
#define MCH_ID_FIRST		MCH_ID_ROWTXT
#define MCH_ID_LAST			1959

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
#define MCH_RES_BTNX		634
#define MCH_RES_BTNY		852
#define MCH_RES_BTNSTEP		220

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
	MCH_KIND_RESLIST
};

bool IsChromePage(int page) {
	return (page >= MENU_SHOP && page <= MENU_SHOP_ACCOUNT) || (page >= MENU_STAT1 && page <= MENU_PERKTREE_LAST) || (page >= MENU_RESEARCH && page <= SHOP_RESPAGE_END);
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
	return MCH_KIND_NONE;
}

int GetChromeSection(int page) {
	if(page >= MENU_SHOP && page <= MENU_SHOP_ACCOUNT)
		return MCH_SEC_SHOP;
	if(page >= MENU_STAT1 && page <= MENU_PERKTREE_LAST)
		return MCH_SEC_CHARACTER;
	if(page >= MENU_RESEARCH && page <= SHOP_RESPAGE_END)
		return MCH_SEC_RESEARCH;
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
	return 0;
}

str GetChromeSubLabel(int sec, int i) {
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
	return 0;
}

// stat category and perk archetype chips carry words, the rest digits
bool ChromeChipsAreWords(int page) {
	int k = GetChromeKind(page);
	return k == MCH_KIND_STATINDEX || k == MCH_KIND_STATS || k == MCH_KIND_PERKINDEX || k == MCH_KIND_PERKTREE || k == MCH_KIND_RESGUNS || k == MCH_KIND_RESLIST;
}

int GetChromeChipW(int page) {
	if(ChromeChipsAreWords(page))
		return 130;
	return MCH_CHIP_W;
}

str GetChromeWordChipLabel(int page, int i) {
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
	switch(page) {
		case MENU_SHOP_WEAPON3_2:
		case MENU_SHOP_WEAPON4_2:
		case MENU_SHOP_WEAPON5_2:
		case MENU_SHOP_WEAPON6_2:
		case MENU_SHOP_AMMO_1_2:
		case MENU_SHOP_AMMO_2_2:
		case MENU_SHOP_AMMO_4_2:
		case MENU_STAT2_OFFENSE2:
		return 2;
		case MENU_SHOP_WEAPON3_3:
		return 3;
	}
	return 1;
}

int GetChromePageCount(int page) {
	switch(page) {
		case MENU_SHOP_WEAPON3_1:
		case MENU_SHOP_WEAPON3_2:
		case MENU_SHOP_WEAPON3_3:
		return 3;
		case MENU_SHOP_WEAPON4_1:
		case MENU_SHOP_WEAPON4_2:
		case MENU_SHOP_WEAPON5_1:
		case MENU_SHOP_WEAPON5_2:
		case MENU_SHOP_WEAPON6_1:
		case MENU_SHOP_WEAPON6_2:
		case MENU_SHOP_AMMO_1:
		case MENU_SHOP_AMMO_1_2:
		case MENU_SHOP_AMMO_2:
		case MENU_SHOP_AMMO_2_2:
		case MENU_SHOP_AMMO_4_1:
		case MENU_SHOP_AMMO_4_2:
		case MENU_STAT2_OFFENSE1:
		case MENU_STAT2_OFFENSE2:
		return 2;
	}
	return 1;
}

// Weapon pages page with boxes the server already numbers (next, then previous); ammo pages page through chrome nav.
bool ChromePagerIsBoxes(int page) {
	return GetChromeKind(page) == MCH_KIND_WEAPONS;
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
	int target = GetChromeElementTarget(page, code);
	if(target > 0)
		UpdateMenuPosition(target);
}

// ---------------------------------------------------------------- geometry

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
	if(k == MCH_KIND_STATS || k == MCH_KIND_PERKTREE)
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
	if(k == MCH_KIND_RESINDEX && i >= 4)
		return MCH_ROW_Y + MCH_ROW_STEP * i + MCH_PERK_TOGGLEGAP;
	return MCH_ROW_Y + MCH_ROW_STEP * i;
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
	for(i = 0; i < rows; ++i)
		MchAddBox(p, MCH_ROW_X, GetChromeRowY(page, i), MCH_ROW_W, MCH_ROW_H);
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
	int n = GetChromeSubCount(GetChromeSection(page));
	for(i = 0; i < n; ++i)
		if(MchIn(cx, cy, MCH_SUB_X + MCH_SUB_STEP * i, MCH_SUB_Y, MCH_SUB_W, MCH_SUB_H))
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
	DeleteTextRange(MCH_ID_TABIMG + 14, MCH_ID_TABIMG + 13 + MCH_MAX_SUBS);
	DeleteTextRange(MCH_ID_TABTXT + 10, MCH_ID_TABTXT + 9 + MCH_MAX_SUBS);
	n = GetChromeSubCount(sec);
	int act = GetChromeSubActive(page);
	for(i = 0; i < n; ++i) {
		x = MCH_SUB_X + MCH_SUB_STEP * i;
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
		MchPic(StrParam(s:"MCS", s:st, d:(i + 1) % 4), MCH_ID_TABIMG + 14 + i, x, MCH_SUB_Y);
		MchText("MENUSUB", StrParam(s:col, l:GetChromeSubLabel(sec, i)), MCH_ID_TABTXT + 10 + i, x + MCH_SUB_W / 2, MCH_SUB_Y + 16, 0);
	}

	DrawChromeStatus(pnum);
}

void DrawChromeStatus(int pnum) {
	int lvl = CheckInventory("Level");
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

	MchText("MENUTAB", StrParam(s:"\c[UIGrey]", l:"DND_MENU_CREDITS", s:"  \c[UIGold]", s:MchMoney(GetPlayerCredit(pnum)),
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
			a.slot = DND_AMMOSLOT_CLIP;
			a.first = AMMOSLOT_CLIP_BEGIN;
			a.last = AMMOSLOT_CLIP_END;
		break;
		case MENU_SHOP_AMMO_1_2:
			a.slot = DND_AMMOSLOT_CLIP;
			a.first = AMMOSLOT_CLIP2_BEGIN;
			a.last = AMMOSLOT_CLIP2_END;
		break;
		case MENU_SHOP_AMMO_2:
			a.slot = DND_AMMOSLOT_SHELL;
			a.first = AMMOSLOT_SHELL_BEGIN;
			a.last = AMMOSLOT_SHELL_END;
		break;
		case MENU_SHOP_AMMO_2_2:
			a.slot = DND_AMMOSLOT_SHELL;
			a.first = AMMOSLOT_SHELL2_BEGIN;
			a.last = AMMOSLOT_SHELL2_END;
		break;
		case MENU_SHOP_AMMO_3:
			a.slot = DND_AMMOSLOT_ROCKET;
			a.first = AMMOSLOT_ROCKET_BEGIN;
			a.last = AMMOSLOT_ROCKET_END;
		break;
		case MENU_SHOP_AMMO_4_1:
			a.slot = DND_AMMOSLOT_CELL;
			a.first = AMMOSLOT_CELL_BEGIN;
			a.last = AMMOSLOT_CELL_END;
		break;
		case MENU_SHOP_AMMO_4_2:
			a.slot = DND_AMMOSLOT_CELL;
			a.first = AMMOSLOT_CELL2_BEGIN;
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

// The shop item of row r on a page, -1 for rows that are navigation.
int GetChromeRowItem(int page, int r) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_WEAPONS) {
		int b = GetWeaponBeginIndexFromOption(page);
		if(b + r <= GetWeaponEndIndexFromOption(page))
			return b + r;
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
	int rows = count - GetChromePagerBoxes(page);
	mch_state_T module& s = GetMchState();
	if(s.page != page) {
		s.page = page;
		s.row = 0;
	}
	if(boxid != MAINBOX_NONE && boxid <= rows)
		s.row = boxid - 1;

	for(i = 0; i < rows; ++i) {
		int y = MCH_ROW_Y + MCH_ROW_STEP * i;
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
		MchPic(StrParam(s:"MCROW", s:st), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		x = MCH_ROW_X + 30;
		if(tagged) {
			MchPic("MCTAGBOX", MCH_ID_ROWIMG + i, MCH_ROW_X + 24, y + 13);
			MchText("MENUSUB", lux ? "\c[UICyan]L" : "\c[UIWhite]R", MCH_ID_ROWTXT + 3 * i + 2, MCH_ROW_X + 38, y + 27, 0);
			x = MCH_ROW_X + 68;
		}
		MchText("MENUBODY", StrParam(s:col, s:name), MCH_ID_ROWTXT + 3 * i, x, y + 27, 1);
		if(StrLen(price))
			MchText("MENUBODY", StrParam(s:pcol, s:price), MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
	}

	DrawChromePager(page, boxid, mainboxid, rows);
	DrawChromePane(pnum, page, Clamp_Between(s.row, 0, Max(rows - 1, 0)));
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

#define MCH_PANE_IX 634
#define MCH_PANE_TX 920
#define MCH_PANE_TEXTW 928
#define MCH_PANE_FOOT 868

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
	if(s.scroll_max) {
		MchPic("MCSBTRK", MCH_ID_PANEIMG + 23, MCH_PANE_X + 954, MCH_STAT_TOP);
		MchPic("MCSBTHM", MCH_ID_PANEIMG + 22, MCH_PANE_X + 954, MCH_STAT_TOP + (620 - 80) * s.scroll / s.scroll_max);
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
	if(pos < s.scroll)
		s.scroll = pos;
	else if(pos >= s.scroll + MCH_RES_ROWS)
		s.scroll = pos - MCH_RES_ROWS + 1;
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
	if(s.scroll_max) {
		MchPic("MCSBTRK", MCH_ID_PANEIMG + 25, MCH_ROW_X + MCH_ROW_W + 10, MCH_ROW_Y);
		MchPic("MCSBTHM", MCH_ID_PANEIMG + 24, MCH_ROW_X + MCH_ROW_W + 10, MCH_ROW_Y + (620 - 80) * s.scroll / s.scroll_max);
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

// ---------------------------------------------------------------- the sell confirm popup

int GetChromePopupBox() {
	int cx = MchCursorX(), cy = MchCursorY();
	if(MchIn(cx, cy, MCH_POP_YESX, MCH_POP_BTNY, MCH_TAB_W, MCH_TAB_H))
		return MCH_NAV_BASE + MCH_EL_POPYES;
	if(MchIn(cx, cy, MCH_POP_NOX, MCH_POP_BTNY, MCH_TAB_W, MCH_TAB_H))
		return MCH_NAV_BASE + MCH_EL_POPNO;
	return MAINBOX_NONE;
}

// what the server is sent as mainboxid: popup buttons are not navigation
int GetChromeSendMain(int mainboxid) {
	if(mainboxid == MCH_NAV_BASE + MCH_EL_POPYES || mainboxid == MCH_NAV_BASE + MCH_EL_POPNO)
		return MAINBOX_NONE;
	return mainboxid;
}

// Clicks the client answers itself: a research row picks the entry the keys would step to, and is not sent.
int HandleChromeClientInput(int page, int input, int mainboxid) {
	if(CheckInventory("DnD_SellConfirm"))
		return HandleChromeSellPopupInput(input, mainboxid);
	if(mainboxid >= MCH_NAV_BASE + MCH_EL_ROW && mainboxid < MCH_NAV_BASE + MCH_EL_ROW + MENU_MAXRES_PERPAGE) {
		if(input == DND_MENUINPUT_LCLICK && GetChromeKind(page) == MCH_KIND_RESLIST) {
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
	return CheckInventory("DnD_SellConfirm") && CheckInventory("DnD_ShowSellPopup") && !s.popanswered;
}

void DrawChromeSellPopup(int mainboxid) {
	int item = CheckInventory("DnD_PopupID");
	MenuHudSize(MCH_W, MCH_H);
	MchPicScaled("MCDIM", MCH_ID_POPIMG + 3, MCH_W / 2, MCH_H / 2, 120.0);
	MchPic("MCPOPUP", MCH_ID_POPIMG + 2, MCH_POP_X, MCH_POP_Y);
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:"DND_MCH_SELLTITLE"), MCH_ID_POPTXT, MCH_POP_X + MCH_POP_W / 2, MCH_POP_Y + 56, 0);
	MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MCH_SELLASK", s:" \c[UICyan]", l:GetWeaponTag(ShopTableIdToWeaponTableId(item)), s:"\c[UIWhite]?\n\c[UIGrey]",
		l:"DND_MCH_SELLGET", s:" \c[UIGold]", s:MchMoney(GetShopPrice(item) / 2)), MCH_ID_POPTXT + 1, MCH_POP_X + MCH_POP_W / 2, MCH_POP_Y + 148, 0);

	bool yes = mainboxid == MCH_NAV_BASE + MCH_EL_POPYES, no = mainboxid == MCH_NAV_BASE + MCH_EL_POPNO;
	MchPic(yes ? "MCTH0" : "MCTO0", MCH_ID_POPIMG, MCH_POP_YESX, MCH_POP_BTNY);
	MchText("MENUTAB", StrParam(s:yes ? "\c[UIWhite]" : "\c[UIGold]", l:"DND_MCH_SELLYES"), MCH_ID_POPTXT + 2, MCH_POP_YESX + MCH_TAB_W / 2, MCH_POP_BTNY + 26, 0);
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

	if(s.scroll_max) {
		MchPic("MCSBTRK", MCH_ID_PANEIMG + 25, MCH_ROW_X + MCH_ROW_W + 10, MCH_ROW_Y);
		MchPic("MCSBTHM", MCH_ID_PANEIMG + 24, MCH_ROW_X + MCH_ROW_W + 10, MCH_ROW_Y + (620 - 80) * s.scroll / s.scroll_max);
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
	if(s.pscroll_max) {
		MenuClipRect(MCH_PANE_X + 954, MCH_PERK_BODYTOP, 8, view * MCH_STAT_LINEH);
		MchPic("MCSBTRK", MCH_ID_PANEIMG + 23, MCH_PANE_X + 954, MCH_PERK_BODYTOP);
		MchPic("MCSBTHM", MCH_ID_PANEIMG + 22, MCH_PANE_X + 954, MCH_PERK_BODYTOP + (view * MCH_STAT_LINEH - 80) * s.pscroll / s.pscroll_max);
		MenuClipRect(0, 0, 0, 0, 0);
	}

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
	if(k != MCH_KIND_STATS && k != MCH_KIND_PERKTREE && k != MCH_KIND_RESLIST)
		return false;
	mch_state_T module& s = GetMchState();
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS), d = 0;
	if(IsButtonHeld(bpress, BT_FORWARD))
		d = -1;
	else if(IsButtonHeld(bpress, BT_BACK))
		d = 1;
	if(!d) {
		s.scroll_tic = 0;
		return false;
	}
	if(s.scroll_tic-- > 0)
		return false;
	s.scroll_tic = 2;
	// a perk tree scrolls whichever side the cursor is on: the list, or the pane's description
	if(k == MCH_KIND_PERKTREE && MchCursorX() >= MCH_PANE_X) {
		int m = Clamp_Between(s.pscroll + d, 0, s.pscroll_max);
		if(m == s.pscroll)
			return false;
		s.pscroll = m;
		return true;
	}
	int n = Clamp_Between(s.scroll + d, 0, s.scroll_max);
	if(n == s.scroll)
		return false;
	s.scroll = n;
	return true;
}

#endif
