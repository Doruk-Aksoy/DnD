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
	RPGMENUID = 1250,
	RPGMENULARRID,
	RPGMENURARRID,
	RPGMENURETARRID,
	RPGMENUITEMIDEND,
	RPGMENUITEMID = 1550,
	RPGMENUITEMSUBID,
	RPGMENUHELPCORNERID,
	RPGMENUHELPCORNERIDMAIN,
	RPGMENUNAMEID,
	RPGMENUHELPID,
	RPGMENUINFOID,
	RPGMENUDAMAGETYPEID,
	RPGMENULISTID = 1610,
	RPGMENUWEAPONPANELID = 1650,
	RPGMENUBACKGROUNDID = 1651,
	
	// monster scanner hud id stuff
	MONSTER_TYPEICONID = 1800,
	MONSTER_TYPEICONID_RIGHT,
	MONSTER_NAMEID,
	MONSTER_TEXTID,
	MONSTER_BARFILLID,
	MONSTER_BARFILLOVERLAY,
	MONSTER_TRAITID = 1900
};

void ClearMonsterScanInfo() {
	DeleteTextRange(MONSTER_TYPEICONID, MONSTER_BARFILLOVERLAY);
	DeleteTextRange(MONSTER_TRAITID, MONSTER_TRAITID + MAX_MONSTER_TRAITS);
}

#define HUD_DII_MULT 14
#define MAXINVENTORYBLOCKS_HORIZ 5
#define MAXINVENTORYBLOCKS_VERT 9

#define MAX_INVENTORY_BOXES (MAXINVENTORYBLOCKS_HORIZ * MAXINVENTORYBLOCKS_VERT)

#define MAX_CRAFTING_ITEMBOXES 16
#define CRAFTING_IMAGE_COUNT 4
#define MATERIALBOX_OFFSET (MAX_CRAFTING_ITEMBOXES * CRAFTING_IMAGE_COUNT - 7)

#define MAX_CRAFTING_MATERIALBOXES 12

void CleanInventoryInfo() {
	// log(s:"cleaning up!");
	DeleteTextRange(RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 14, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES);
}

// cleans up stuff in crafting material panel -- minus_bg is for preserving the background!
void CleanMaterialInfo(bool minus_bg) {
	// -2 for the two arrows in the material page
	DeleteTextRange(RPGMENUID - MATERIALBOX_OFFSET - 3 * MAX_CRAFTING_MATERIALBOXES - 2, RPGMENUID - minus_bg);
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
	DeleteText(PUNISHERPERKID1);
	DeleteText(PUNISHERPERKID2);
	
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
	POPUP_MONKILLCONDNOTMET
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
		case POPUP_PLAYERCONDNOTMET:
		return "DND_POPUP_PLAYERCONDNOTMET";
		case POPUP_MONKILLCONDNOTMET:
		return "DND_POPUP_MONKILLCONDNOTMET";
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

typedef struct cursor {
	int posx;
	int posy;
	int itemHovered;			// spot of the item in player's inventory, or weapon's id
	int itemHoveredType;		// is this an orb, etc. or a weapon
	int itemHoveredSource;		// for inventory stuff, player inventory, stash etc.
	vec2_T itemHoveredDim;		// dim for draving things
	int itemDragged;			// this holds the image id, not the item id (We dont need it)
	bool itemDraggedStashSize;	// draw resized to fit stash view
	vec3_T itemDragInfo;		// x,y used for dimensions of item, z is used for topbox so its not drawn until itemDragged is -1
	bool hoverNeedsReset;		// need reset on hover data
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

int GetTriggeredBoxOnPane(menu_pane_T& p, int mx, int my, int xlim = 348.0, int ylim = 270.0) {
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

// Meant to be used entirely clientside only, for scrolling up and down (used when server doesnt need to know about this)
bool ListenScroll(int condx_min, int condx_max) {
	bool redraw = false;
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS);
	// up is 1, down is 2
	// opposite buttons because view should go up
	if(IsButtonHeld(bpress, settings[0][0])) {
		if(ScrollPos.x < condx_max) {
			++ScrollPos.x;
			redraw = true;
		}
		SetInventory("MenuUD", 1);
	}
	if(IsButtonHeld(bpress, settings[2][0])) {
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
