#ifndef DND_HUD_IN
#define DND_HUD_IN

#define ASPECT_4_3 (4.0 / 3)
#define ASPECT_5_4 1.25
#define ASPECT_16_9 (16.0 / 9)
#define ASPECT_16_10 1.6

#define MAX_SCREENRES_OFFSETS 5
#define SCREEN_ASPECT_RATIO 4
int ScreenResOffsets[MAX_SCREENRES_OFFSETS] = { -1, -1, -1, -1, ASPECT_4_3 };

#define INVENTORYINFO_NORMALVIEW_WRAPX 272.0
#define INVENTORYINFO_NORMALVIEW_WRAPY 152.0

#define INVENTORYINFO_TRADEVIEW_WRAPX 272.0
#define INVENTORYINFO_TRADEVIEW_WRAPY 152.0

#define NOTIFBAK_X 160
#define NOTIFBAK_Y 128
#define NOTIFBAK_XF 160.0
#define NOTIFBAK_YF 128.0

#define QSTBOX_X 250
#define QSTBOX_Y 150
#define QSTBOX_XF 250.0
#define QSTBOX_YF 150.0

#define QSTBOX2_X 288
#define QSTBOX2_Y 173
#define QSTBOX2_XF 288.0
#define QSTBOX2_YF 173.0

#define MONSTERINFO_HOLDTIME 0.2

typedef struct coord {
	int x;
	int y;
} coord_T;

// x is scroll current pos, y is scroll limit
coord_T ScrollPos = { 0, 0 };

// MENU IDS
// Moved here because of dependencies
enum {
	RPGMENUCURSORID = 100,
	RPGMENUPOPUPID = 105,
	RPGMENUTRADECOUNTDOWNID,
	RPGMENUPAGEID = 120,
	RPGMENUHIGHLIGHTID,
	
	RPGMENUCLICKEDID = 849,
	
	RPGMENUINVENTORYID = 1749,
	RPGMENUID,
	RPGMENULARRID,
	RPGMENURARRID,
	RPGMENURETARRID,
	RPGMENUITEMIDEND,
	RPGMENUITEMID = 2050,
	RPGMENUITEMSUBID,
	RPGMENUHELPCORNERID,
	RPGMENUHELPCORNERIDMAIN,
	RPGMENUNAMEID,
	RPGMENUHELPID,
	RPGMENUINFOID,
	RPGMENUDAMAGETYPEID,
	RPGMENULISTID = 2110,
	RPGMENUWEAPONPANELID = 2150,
	RPGMENUBACKGROUNDID = 2151,
	
	// monster scanner hud id stuff
	MONSTER_TYPEICONID = 2300,
	MONSTER_TYPEICONID_RIGHT,
	MONSTER_NAMEID,
	MONSTER_TEXTID,
	MONSTER_BARFILLID,
	MONSTER_BARFILLOVERLAY,
	MONSTER_TRAITID = 2400
};

void ClearMonsterScanInfo() {
	DeleteTextRange(MONSTER_TYPEICONID, MONSTER_BARFILLOVERLAY);
	DeleteTextRange(MONSTER_TRAITID, MONSTER_TRAITID + MAX_MONSTER_TRAITS);
}

#define HUD_DII_MULT 14
#define MAXINVENTORYBLOCKS_HORIZ 5 // from top to bottom, 5 heights
#define MAXINVENTORYBLOCKS_VERT 9 // from left to right, 9 vertical slices
#define MAX_INVENTORY_BOXES (MAXINVENTORYBLOCKS_HORIZ * MAXINVENTORYBLOCKS_VERT)

/*
0 	1	2	3	4	5	6	7	8
9	10	11	12	13	14	15	16	17
18	19	20	21	22	23	24	25	26
27	28	29	30	31	32	33	34	35
36	37	38	39	40	41	42	43	44
*/

#define MAX_CRAFTING_ITEMBOXES 16
#define CRAFTING_IMAGE_COUNT 4
#define MATERIALBOX_OFFSET (MAX_CRAFTING_ITEMBOXES * CRAFTING_IMAGE_COUNT - 7)

#define MAX_CRAFTING_MATERIALBOXES 12

void CleanInventoryInfo(int id_begin = RPGMENUINVENTORYID) {
	// log(s:"cleaning up!");
	DeleteTextRange(id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES - 14, id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES);
}

// cleans up stuff in crafting material panel -- minus_bg is for preserving the background!
void CleanMaterialInfo(bool minus_bg) {
	// -2 for the two arrows in the material page
	DeleteTextRange(RPGMENUID - MATERIALBOX_OFFSET - 3 * MAX_CRAFTING_MATERIALBOXES - 2, RPGMENUID - minus_bg);
}

void CleanTradeInfo() {
	DeleteTextRange(RPGMENUINVENTORYID - 12 * MAX_INVENTORY_BOXES - 12, RPGMENUINVENTORYID);
}

void CleanCraftingInfo() {
	DeleteTextRange(RPGMENUID - MATERIALBOX_OFFSET - 3 * MAX_CRAFTING_MATERIALBOXES - 2, RPGMENUID);
}

void ClearMenuDisplay() {
	DeleteText(RPGMENUBACKGROUNDID);
	DeleteText(RPGMENULARRID);
	DeleteText(RPGMENURETARRID);
	DeleteText(RPGMENURARRID);
	DeleteText(RPGMENUHELPID);
	DeleteText(RPGMENUNAMEID);
	DeleteText(RPGMENUHELPCORNERID);
	DeleteText(RPGMENUHELPCORNERIDMAIN);
	DeleteText(RPGMENUPAGEID);
	DeleteText(RPGMENUPAGEID - 1);
	DeleteText(RPGMENUHIGHLIGHTID);
	DeleteTextRange(RPGMENULISTID - 1, RPGMENULISTID);
	
	DeleteTextRange(RPGMENUCURSORID, RPGMENUINVENTORYID);
	DeleteTextRange(RPGMENUINFOID, RPGMENUWEAPONPANELID);
	DeleteTextRange(RPGMENUITEMIDEND, RPGMENUITEMSUBID);

	// login screen for hardcore
	DeleteText(DND_HARDCORE_SCREENID);
	CleanInventoryInfo();
	CleanMaterialInfo(false);
}

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
	POPUP_YOUARENTALLOWED,
	POPUP_PLAYERCONDNOTMET,
	POPUP_MONKILLCONDNOTMET,
	POPUP_ITEMLVLTOOHIGH,
	POPUP_NOMORESMALLCHARMS,
	POPUP_ONLYONEUNIQUE,
	POPUP_ALREADYFOUND,
	POPUP_MAXINVESTMENT,
	POPUP_NARESEARCH,
	POPUP_NOTENOUGHSPACE,
	
	#ifdef ISAPRILFIRST
	POPUP_OWNNFT
	#endif
};

#ifndef ISAPRILFIRST
	#define MAX_POPUPS (POPUP_NOTENOUGHSPACE + 1)
#else
	#define MAX_POPUPS (POPUP_OWNNFT + 1)
#endif

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
		case POPUP_PLAYERCONDNOTMET:
		return "DND_POPUP_PLAYERCONDNOTMET";
		case POPUP_MONKILLCONDNOTMET:
		return "DND_POPUP_MONKILLCONDNOTMET";
		case POPUP_ITEMLVLTOOHIGH:
		return "DND_POPUP_ITEMLVLTOOHIGH";
		case POPUP_NOMORESMALLCHARMS:
		return "DND_POPUP_NOMORESMALLCHARMS";
		case POPUP_ONLYONEUNIQUE:
		return "DND_POPUP_ONLYONEUNIQUE";
		case POPUP_ALREADYFOUND:
		return "DND_POPUP_ALREADYFOUND";
		case POPUP_MAXINVESTMENT:
		return "DND_POPUP_MAXINVESTMENT";
		case POPUP_NARESEARCH:
		return "DND_POPUP_NARESEARCH";
		case POPUP_NOTENOUGHSPACE:
		return "DND_POPUP_NOTENOUGHSPACE";
		
		#ifdef ISAPRILFIRST
		case POPUP_OWNNFT:
		return "DND_POPUP_OWNNFT";
		#endif
	}
	return "";
}

void ShowPopup(int popupid, bool isSell, int activebox) {
	ACS_NamedExecuteAlways("DnD Menu Popup", 0, Playernumber() | (popupid << 16), isSell, activebox);
}

void ShowActorPopup(int pnum, int popupid, bool isSell, int activebox) {
	ACS_NamedExecuteAlways("DnD Menu Popup", 0, pnum | (popupid << 16), isSell, activebox);
}

#define MONSTER_BARID (MONSTER_TRAITID + MAX_MONSTER_TRAITS + 1)

// topleft corner 1:1 bottom right 0:0
#define HUDMAX_X 480
#define HUDMAX_Y 320
#define HUDMAX_XF 480.0
#define HUDMAX_YF 320.0

#define HUDTEXTMAX_X 720
#define HUDTEXTMAX_Y 480
#define HUDTEXTMAX_XF 720.0
#define HUDTEXTMAX_YF 480.0

#define HUD_ITEMBAK_X 209
#define HUD_ITEMBAK_Y 168
#define HUD_ITEMBAK_XF 209.0
#define HUD_ITEMBAK_YF 168.0

// stash hud
#define HUDMAX_X_STASH (HUDMAX_X * 3 / 2)
#define HUDMAX_Y_STASH (HUDMAX_Y * 3 / 2)
#define HUDMAX_XF_STASH (HUDMAX_XF * 3 / 2)
#define HUDMAX_YF_STASH (HUDMAX_YF * 3 / 2)

#define MMASK (-1.0)

#define X_MULTIPLIER (4800.0)
#define Y_MULTIPLIER (3200.0)

#define MOUSE_INPUT_X 1
#define MOUSE_INPUT_Y 2

#define HUDMAX_X_PROMPT (HUDMAX_X * 2)
#define HUDMAX_Y_PROMPT (HUDMAX_Y * 2)
#define HUDMAX_XF_PROMPT (HUDMAX_XF * 2)
#define HUDMAX_YF_PROMPT (HUDMAX_YF * 2)

enum {
	DND_MENUINPUT_LCLICK = 1,
	DND_MENUINPUT_RCLICK,
	DND_MENUINPUT_PREVBUTTON,
	DND_MENUINPUT_NEXTBUTTON,
	DND_MENUINPUT_USEBUTTON
};

#define DND_MENU_INPUTDELAYTICS 4
#define DND_MENUINPUT 0
#define DND_MENUINPUT_DELAY 1
#define DND_MENUINPUT_PAYLOAD 2
#define DND_MENUINPUT_PLAYERCRAFTCLICK 3
#define DND_MENUINPUT_LRPOS 4
int MenuInputData[MAXPLAYERS][5];

typedef struct rect {
	int topleft_x;
	int topleft_y;
	int botright_x;
	int botright_y;
} rect_T;

typedef struct idragdata {
	int size_x;
	int size_y;
	int topboxid;
	int source;
	int click_box;				// the box that we had clicked initially
} idragdata_T;

typedef struct cursor {
	int posx;
	int posy;
	int owner_pnum;				// player number of the owner of the item being hovered on -- This doesnt necessarily have to be us! It can be another player (trade case)
	
	int itemHovered;			// spot of the item in player's inventory, or weapon's id
	int itemHoveredType;		// is this an orb, etc. or a weapon
	int itemHoveredSource;		// for inventory stuff, player inventory, stash etc.
	vec2_T itemHoveredDim;		// dim for draving things
	bool hoverNeedsReset;		// need reset on hover data
	
	int itemDragged;			// this holds the image id, not the item id (We dont need it)
	bool itemDraggedStashSize;	// draw resized to fit stash view
	idragdata_T itemDragInfo;	// x,y used for dimensions of item, z is used for topbox so its not drawn until itemDragged is -1
	
	// just like hover but for items user had clicked previously
	int itemClicked;
	int itemClickedType;		// is this an orb, etc. or a weapon
	int itemClickedSource;		// for inventory stuff, player inventory, stash etc.
	vec2_T itemClickedPos;
	bool clickNeedsReset;
} cursor_T;
cursor_T PlayerCursorData;

#define MAX_MENU_BOXES 20
typedef struct mp {
	rect_T MenuRectangles[MAX_MENU_BOXES];
	int cursize;
} menu_pane_T;

menu_pane_T& GetPane() {
	static menu_pane_T pane;
	return pane;
}

void ResetPane(menu_pane_T& p) {
	p.cursize = 0;
}

void AddBoxToPane_Points(menu_pane_T& p, int tx, int ty, int bx, int by) {
	if(p.cursize < MAX_MENU_BOXES) {
		p.MenuRectangles[p.cursize].topleft_x = tx;
		p.MenuRectangles[p.cursize].topleft_y = ty;
		p.MenuRectangles[p.cursize].botright_x = bx;
		p.MenuRectangles[p.cursize].botright_y = by;
		p.cursize++;
	}
	else
		Log(s:"Menu box limit exceeded.");
}

// since we use top left corner as 1:1, directions are changed
bool point_in_box(rect_T? box, int mx, int my, int yoffset) {
	return (mx <= box.topleft_x && mx >= box.botright_x && my <= box.topleft_y - yoffset && my >= box.botright_y - yoffset);
}

bool point_in_points(int ux, int uy, int lx, int ly, int mx, int my, int yoffset) {
	return (mx <= ux && mx >= lx && my <= uy - yoffset && my >= ly - yoffset);
}

int GetTriggeredBoxOnPane(menu_pane_T& p, int mx, int my, int xlim = 348.0, int ylim = 282.0) {
	if(mx >= xlim || my >= ylim)
		return MAINBOX_NONE;
	for(int i = 0; i < p.cursize; ++i) {
		if(point_in_box(p.MenuRectangles[i], mx, my, 0))
			return i + 1;
	}
	return MAINBOX_NONE;
}

bool HasPlayerClicked(int pnum) {
	return MenuInputData[pnum][DND_MENUINPUT] == DND_MENUINPUT_LCLICK || MenuInputData[pnum][DND_MENUINPUT] == DND_MENUINPUT_RCLICK;
}

bool HasLeftClicked(int pnum) {
	return MenuInputData[pnum][DND_MENUINPUT] == DND_MENUINPUT_LCLICK;
}

bool HasRightClicked(int pnum) {
	return MenuInputData[pnum][DND_MENUINPUT] == DND_MENUINPUT_RCLICK;
}

bool HasPressedLeft(int pnum) {
	return MenuInputData[pnum][DND_MENUINPUT] == DND_MENUINPUT_PREVBUTTON;
}

bool HasPressedRight(int pnum) {
	return MenuInputData[pnum][DND_MENUINPUT] == DND_MENUINPUT_NEXTBUTTON;
}

void ClearPlayerInput(int pnum, bool cleanLR) {
	MenuInputData[pnum][DND_MENUINPUT] = 0;
	if(cleanLR)
		MenuInputData[pnum][DND_MENUINPUT_LRPOS] = 0;
}

int GetCursorPos(int input, int mt) {
	int res = 0, speed, ds;
	switch(mt) {
		case MOUSE_INPUT_X:
			res = PlayerCursorData.posx;
			speed = FixedDiv(1.0, FixedMul(GetCVar("m_yaw"), GetCVar("mouse_sensitivity")));
			speed = FixedMul(speed * 2, HUDMAX_XF) / (HUDMAX_X * 100);
			ds = input * speed;
			res = Clamp_Between(res + ds, 0, HUDMAX_XF);
		break;
		case MOUSE_INPUT_Y:
			res = PlayerCursorData.posy;
			speed = FixedDiv(1.0, FixedMul(GetCVar("m_pitch"), GetCVar("mouse_sensitivity")));
			speed = FixedMul(speed * 2, HUDMAX_YF) / (HUDMAX_X / 2 * 100);
			if (GetCVar("invertmouse"))
				speed *= -1;
			ds = input * speed;
			res = Clamp_Between(res + ds, 0, HUDMAX_YF);
		break;
	}
	return res;
}

void DrawCursor() {
	static int cursor_anim = 0;
	if(cursor_anim < 8)
		SetFont("DND_CUR5");
	else
		SetFont(StrParam(s:"DND_CUR", d:cursor_anim / 4 - 1));
	cursor_anim = (cursor_anim + 1) % 24;
	
	//printbold(f:PlayerCursorData.posx, s: " ", f:PlayerCursorData.posy);

	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUCURSORID, -1, HUDMAX_XF - (PlayerCursorData.posx & MMASK) + 0.1, HUDMAX_YF - (PlayerCursorData.posy & MMASK) + 0.1, 0.2, 0.0);
}

void ListenNPCInput() {
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS);
	int obpress = GetPlayerInput(-1, INPUT_OLDBUTTONS);
	
	if(!CheckInventory("DnD_ClickTicker")) {
		if(IsButtonPressed(bpress, obpress, BT_ATTACK)) {
			GiveInventory("DnD_ClickTicker", 1);
			SetInventory("MenuInput", DND_MENUINPUT_LCLICK);
		}
		else if(IsButtonPressed(bpress, obpress, BT_ALTATTACK)) {
			GiveInventory("DnD_ClickTicker", 1);
			SetInventory("MenuInput", DND_MENUINPUT_RCLICK);
		}
		else if(IsButtonPressed(bpress ,obpress, BT_USE)) {
			GiveInventory("DnD_ClickTicker", 1);
			SetInventory("MenuInput", DND_MENUINPUT_USEBUTTON);
		}
	}
}

int GetAspectRatio(void) {
	int width = getcvar("vid_defwidth");
	int height = getcvar("vid_defheight");
	int nowidescreen = getcvar("vid_nowidescreen");
	int tft = getcvar("vid_tft");
	int aspect = getcvar("vid_aspect");
	switch(aspect) {
		case 1:	return ASPECT_16_9;
		case 2:	return ASPECT_16_10;
		case 3:	return ASPECT_4_3;
		case 4:	return ASPECT_5_4;
	}
	if(nowidescreen) {
		if(!tft)
			return ASPECT_4_3;
		if(fixedmul(height<<16, fixeddiv(5.0, 4.0)) == width<<16)
			return ASPECT_5_4;
		else
			return ASPECT_4_3;
	}
	if(abs((abs(fixedmul(height<<16, fixeddiv(16.0, 9.0)))>>16) - width) < 10) {
		return ASPECT_16_9;
	}
	if(abs((abs(fixedmul(height<<16, fixeddiv(16.0, 10.0)))>>16) - width) < 60) {
		if((width == 320 && height == 200) || (width == 640 && height == 400))
			return ASPECT_4_3;
		return ASPECT_16_10;
	}
	if(fixedmul(height<<16, fixeddiv(5.0, 4.0))>>16 == width && tft) {
		return ASPECT_5_4;
	}
	return ASPECT_4_3;
}

// returns the edge locations of hud left and right
int GetHudRight(int width) {
    if(ScreenResOffsets[SCREEN_ASPECT_RATIO] == ASPECT_16_10)
        return width + (FixedDiv(width << 16, 10.0) >> 16);
    if(ScreenResOffsets[SCREEN_ASPECT_RATIO] == ASPECT_16_9)
        return width + (FixedDiv(width << 16, 5.9) >> 16);
    return width;
}

int GetHudLeft(int width) {
    return width - GetHudRight(width);
}

void SetupScreenOffsets() {
	ScreenResOffsets[SCREEN_ASPECT_RATIO] = GetAspectRatio();
	if (ScreenResOffsets[SCREEN_ASPECT_RATIO] == ASPECT_5_4) {
		ScreenResOffsets[0] = 0.0;
		ScreenResOffsets[1] = 0.0;
		ScreenResOffsets[2] = 0.0;
		ScreenResOffsets[3] = 0.0;
	}
	else if (ScreenResOffsets[SCREEN_ASPECT_RATIO] == ASPECT_4_3) {
		ScreenResOffsets[0] = 0.0;
		ScreenResOffsets[1] = 0.0;
		ScreenResOffsets[2] = 80.0;
		ScreenResOffsets[3] = 120.0;
	}
	else if (ScreenResOffsets[SCREEN_ASPECT_RATIO] == ASPECT_16_9) {
		ScreenResOffsets[0] = 108.0;
		ScreenResOffsets[1] = 110.0;
		ScreenResOffsets[2] = 0.0;
		ScreenResOffsets[3] = 0.0;
	}
	else if (GetAspectRatio() == ASPECT_16_10) {
		ScreenResOffsets[0] = 54.0;
		ScreenResOffsets[1] = 66.0;
		ScreenResOffsets[2] = 30.0;
		ScreenResOffsets[3] = 45.0;
	}
}

// Meant to be used entirely clientside only, for scrolling up and down (used when server doesnt need to know about this)
bool ListenScroll(int condx_min, int condx_max) {
	bool redraw = false;
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS);
	// up is 1, down is 2
	// opposite buttons because view should go up
	if(IsButtonHeld(bpress, BT_FORWARD)) {
		if(ScrollPos.x < condx_max) {
			++ScrollPos.x;
			redraw = true;
		}
		SetInventory("MenuUD", 1);
	}
	if(IsButtonHeld(bpress, BT_BACK)) {
		if(ScrollPos.x > condx_min) {
			--ScrollPos.x;
			redraw = true;
		}
		SetInventory("MenuUD", 2);
	}
	return redraw;
}

// we need this because the player name returned from acs functions currently includes color codes which may affect text length for trims
int GetPlayernameRawLength(str name) {
	int len = Strlen(name);
	int real_len = 0;
	int color_count = 0;
	bool in_ccode = false;
	bool in_bracketed_color = false;
	
	for(int i = 0; i < len; ++i) {
		int c = GetChar(name, i);
		
		// 28 is FS (File Seperator)
		if(c == 28)
			in_ccode = true;
		else if(in_ccode) {
			++color_count;
			if(c == '[')
				in_bracketed_color = true;
			else if((!in_bracketed_color && color_count == 1) || (in_bracketed_color && c == ']')) {
				// enter here if we counted the color character in zandro (\c + u into \cu example) or in newtextcolors bracket style (\c[M3] for example)
				in_ccode = false;
				in_bracketed_color = false;
				color_count = 0;
			}
		}
		else
			++real_len;
	}
	return real_len;
}

#endif
