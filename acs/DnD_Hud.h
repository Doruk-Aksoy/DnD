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

#define NEXT_LINE_LEN 35
#define NEXT_LINE_LEN_ATTR 30

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
#define EXIT_HOLDTIME 0.5

#define MILLION_MINUS_ONE 999999
#define THOUSAND_MINUS_ONE 999

#define TIPBOX_TOPLEN 70.0
#define TIPBOX_MIDLEN 39.0

#define ITEMINFOBG_TOPLEN 17.0
#define ITEMINFOBG_MIDLEN 16.0
#define ITEMINFOBG_MAXMIDS 12
#define ITEMINFO_CLIPWIDTH 360

typedef struct coord {
	int x;
	int y;
} coord_T;

// x is scroll current pos, y is scroll limit
coord_T ScrollPos = { 0, 0 };
//coord_T HudScale = { 1.0, 1.0 };

#define BOSSDATA_TID 0 // unique boss tid, this is typically dungeon boss or one off boss encounters in maps
#define BOSSDATA_HP 1 // unique boss hp, monster health isnt synced to clients
#define BOSSDATA_FORT 2
#define BOSSDATA_DMGTAKEN 3
#define BOSSDATA_DMGTRIGGER 4
#define BOSSDATA_AWAKENED 5
int DungeonBossData[6] = { 0, 0, 0, 0, 0, 0 };

// MENU IDS
// Moved here because of dependencies
enum {
	RPGMENUCURSORID = 100,
	RPGMENUPOPUPID = 105,
	RPGMENUTRADECOUNTDOWNID,
	RPGMENUPAGEID = 120,
	RPGMENUHIGHLIGHTID,

	// this is used as basis for the character select menu id
	RPGMENUCHARSELID = 800,

	RPGMENUINVENTORYID = 849,
	
	RPGMENUCLICKEDID = 1749,
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
	MONSTER_BARFILLOVERLAY2,
	MONSTER_BARFILLOVERLAY3,
	MONSTER_BARFILLOVERLAY4,
	MONSTER_TRAITID = 2400,
	
	HUD_GRAPHIC_ID = 2600,

	BLACKOUT_ID = 2699,
	ESHIELD_LEFT_ID,
	ESHIELD_RIGHT_ID,

	PLAYER_MUGSHOTID = 2800
};

#define MAX_CYBORG_VISOR_FRAMES 39

void ClearMonsterScanInfo() {
	DeleteTextRange(MONSTER_TYPEICONID, MONSTER_BARFILLOVERLAY4);
	DeleteTextRange(MONSTER_TRAITID, MONSTER_TRAITID + MAX_MONSTER_TRAITS_SHOWN);
}

/*int GetUIScale(int hud_scale, int width, int height) {
	int scale = 1;
	int htemp, wtemp;
	if(hud_scale > 0)
		scale = hud_scale;
	else {
		// default scale to 640x400
		htemp = height / 400;
		wtemp = width / 640;
		scale = max(1, min(height, width));
	}
	
	htemp = height / 200;
	wtemp = width / 320;
	
	return max(1, min(scale, max(htemp, wtemp)));
}

void CalculateHudScale(int width, int height, bool isForcedScale) {
	int hud_scale = GetCVar("hud_scale");
	Log(s:"hud cvar: ", d:hud_scale);
	if(hud_scale < 0 || isForcedScale) {
		HudScale.x = 1.0;
		HudScale.y = 1.0;
		return;
	}
		
	int scale = GetUIScale(hud_scale, width, height);
	scale = max(1.0, 320.0 * scale / height);
		
	HudScale.x = scale;
	HudScale.y = FixedMul(scale, GetCVar("hud_aspectscale") ? 1.2 : 1.0);
}*/

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
	DeleteTextRange(id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES - 20 - ITEMINFOBG_MAXMIDS, id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES);
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
	CleanInventoryInfo(RPGMENUCLICKEDID);
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
	POPUP_CANTWEARBODYARMOR,
	POPUP_CANTPUTONBODYARMOR,
	POPUP_NOTENOUGHSPACE,
	POPUP_SCAVENGECONFIRM,
	POPUP_NOTENOUGHMATERIALS,
	POPUP_CANTUSEPOWERCORE,
	
	#ifdef ISAPRILFIRST
	POPUP_OWNNFT
	#endif
};

#ifndef ISAPRILFIRST
	#define MAX_POPUPS (POPUP_CANTUSEPOWERCORE + 1)
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
		case POPUP_CANTWEARBODYARMOR:
		return "DND_POPUP_CANTUSEBODYARMOR";
		case POPUP_CANTPUTONBODYARMOR:
		return "DND_POPUP_CANTPUTONBODYARMOR";
		case POPUP_NOTENOUGHSPACE:
		return "DND_POPUP_NOTENOUGHSPACE";
		case POPUP_SCAVENGECONFIRM:
		return "DND_POPUP_SCAVENGECONFIRM";
		case POPUP_NOTENOUGHMATERIALS:
		return "DND_POPUP_NOTENOUGHMATERIALS";
		case POPUP_CANTUSEPOWERCORE:
		return "DND_POPUP_CANTUSEPOWERCORE";
		
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

void ShowNotif(int popupid, int activebox, int extra) {
	ACS_NamedExecuteAlways("DnD Menu Notif", 0, Playernumber() | (popupid << 16), extra, activebox);
}

void ShowActorPopup(int pnum, int popupid, bool isSell, int activebox) {
	ACS_NamedExecuteAlways("DnD Menu Popup", 0, pnum | (popupid << 16), isSell, activebox);
}

#define MONSTER_BARID (MONSTER_TRAITID + MAX_MONSTER_TRAITS_SHOWN + 1)

// topleft corner 1:1 bottom right 0:0
#define HUDMAX_X 480
#define HUDMAX_Y 320
#define HUDMAX_XF 480.0
#define HUDMAX_YF 320.0

#define HUDTEXTMAX_X 720
#define HUDTEXTMAX_Y 480
#define HUDTEXTMAX_XF 720.0
#define HUDTEXTMAX_YF 480.0

#define HUD_ITEMBAK_X 212
#define HUD_ITEMBAK_Y 168
#define HUD_ITEMBAK_XF 212.0
#define HUD_ITEMBAK_YF 168.0
#define HUD_ITEMBAK_WIDTH (4 * HUD_ITEMBAK_X / 3 + 16)

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

void ListenMouseInput() {
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
// holds amounts of newlines << 16
int GetRawLength(str name) {
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
		else if(c != 10)
			++real_len;
		else
			real_len += 1 << 16;
	}
	return real_len;
}

// this is the refined version of the above function where it counts how many new lines we'd need given a big string that can potentially have many line breaks
int CountNewLinesInText(str name, int char_count_per_line) {
	int len = Strlen(name);
	int real_len = 0;
	int line_count = 0;
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
		else if(c != 10) {
			// not new line
			++real_len;
			if(real_len >= char_count_per_line) {
				real_len = 0;
				++line_count;
			}
		}
		else {
			++line_count;
			real_len = 0;
		}
	}
	return line_count;
}

void HudMessageOnActor(int tid, int msgID, int hudX = 640, int hudY = 480, int xOffset = 0, int yOffset = 0, int range = 128, str sprite = "TNT1A0", str text = "Null", int holdTime = 0.1, int colour = CR_RED) {
	int dist, angle, vang, pitch, x, y;

	if(sprite != -1) {
		SetFont(sprite);
		text = "A";
		//offset = 0.1;
	}

	SetHudSize(hudX, hudY, 1);
	x = GetActorX(tid) - GetActorX(0);
	y = GetActorY(tid) - GetActorY(0);

	vang = VectorAngle(x,y);
	angle = (vang - GetActorAngle(0) + 1.0) % 1.0;

	if(((vang + 0.125) % 0.5) > 0.25)
		dist = FixedDiv(y, sin(vang));
	else
		dist = FixedDiv(x, cos(vang));

	if ((angle < 0.2 || angle > 0.8) && (dist >> 16) < range) {
		if (GetActorPitch(0) >= -0.25 && GetActorPitch(0) <= 0.25) {

			pitch = VectorAngle(dist, GetActorZ(tid) - (GetActorZ(0) + 41.0));
			pitch = (pitch + GetActorPitch(0) + 1.0) % 1.0;

			// Fixes divide by zero
			if ((hudX / 2) * sin(angle) != 0 && cos(angle) != 0 && (hudX / 2) * sin(pitch) != 0 && cos(pitch) != 0) {
				x = hudX / 2 - ((hudX / 2) * sin(angle) / cos(angle));
				y = hudY / 2 - ((HUDX / 2) * sin(pitch) / cos(pitch));

				x += xOffset;
				y += yOffset;

				HudMessage(s:text; HUDMSG_PLAIN, msgID, colour, (x << 16), (y << 16), holdTime);
			}
		}
	}
}

// assumes 999, 999, 999 is total max amount to be shown
str GetIntegerCommaView(int x) {
	if(x > MILLION_MINUS_ONE) {
		return StrParam(d:x / 1000000, s:", ", d:(x / 100000) % 10, d:(x / 10000) % 10, d:(x / 1000) % 10, s:", ", d:(x / 100) % 10, d:(x / 10) % 10, d:x % 10);
	}
	else if(x > THOUSAND_MINUS_ONE)
		return StrParam(d:x / 1000, s:", ", d:(x / 100) % 10, d:(x / 10) % 10, d:x % 10);
	return StrParam(d:x);
}

// hp bar of monsters
void DrawMonsterHPBar(int mon_tid, int mmaxhp, int monhp, int monlevel, int monid, int m_id, int fortify_amt) {
	str barGraphic = "";
	str fortGraphic = "";
	int i, j;
	
	SetHudSize(800, 600, 0);
	SetFont(GetMonsterTypeIcon(GetMonsterType(monid, mon_tid)));	
	HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_TYPEICONID, CR_UNTRANSLATED, 270.0, 27.0, MONSTERINFO_HOLDTIME);
	HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_TYPEICONID_RIGHT, CR_UNTRANSLATED, 540.0, 27.0, MONSTERINFO_HOLDTIME);
	
	// inner text
	SetFont ("MONFONT");
	
	// do the hp drawing in-place to save usage of strparam spam
	HudMessage(l:"DND_STAT18_SHORT", s:": ", i:monlevel, s:"  ", s:GetIntegerCommaView(monhp), s:" / ", s:GetIntegerCommaView(mmaxhp); HUDMSG_FADEOUT, MONSTER_TEXTID, CR_WHITE, 400.0, 27.0, MONSTERINFO_HOLDTIME);
	
	bool is_unique = 0;
	bool mon_isPet = IsPet(mon_tid);
	
	if(!mon_isPet) {
		is_unique = IsUniqueMonster(monid);
		str prefix = "";
		if(CheckInventory("TargetIsElite"))
			prefix = StrParam(s:"\cf", l:"DND_ELITE", s:" ");
		if(MonsterProperties[m_id].trait_list[DND_LEGENDARY])
			HudMessage(s:prefix, s:GetActorProperty(mon_tid, APROP_NAMETAG); HUDMSG_FADEOUT, MONSTER_NAMEID, CR_RED, 404.4, 10.0, MONSTERINFO_HOLDTIME);
		else if(is_unique)
			HudMessage(s:prefix, s:GetActorProperty(mon_tid, APROP_NAMETAG); HUDMSG_FADEOUT, MONSTER_NAMEID, CR_RED, 404.4, 10.0, MONSTERINFO_HOLDTIME);
		else if(IsMonsterIdBoss(monid))
			HudMessage(s:prefix, s:GetActorProperty(mon_tid, APROP_NAMETAG); HUDMSG_FADEOUT, MONSTER_NAMEID, CR_ORANGE, 404.4, 10.0, MONSTERINFO_HOLDTIME);
		else
			HudMessage(s:prefix, s:GetActorProperty(mon_tid, APROP_NAMETAG); HUDMSG_FADEOUT, MONSTER_NAMEID, CR_WHITE, 404.4, 10.0, MONSTERINFO_HOLDTIME);
	}
	else
		HudMessage(s:prefix, s:GetActorProperty(mon_tid, APROP_NAMETAG); HUDMSG_FADEOUT, MONSTER_NAMEID, CR_WHITE, 404.4, 10.0, MONSTERINFO_HOLDTIME);
	
	// made this way to reduce variable dependancy and if statement use
	DeleteTextRange(MONSTER_BARFILLID, MONSTER_BARFILLOVERLAY4);
	if(CheckInventory("TargetHealthBarColor")) {
		if(!is_unique)
			SetFont("MNRHPBAR");
		else
			SetFont("MNUHPBAR");
		HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_BARID, CR_UNTRANSLATED, 400.0, 27.0, MONSTERINFO_HOLDTIME);
		
		if(monhp) {
			// clamp to 100 on certain monsters' overheal abilities (Warmaster for example)
			i = (monhp * 100 / mmaxhp);
			if(i > 100)
				i = 100;
			i <<= 1;
		
			if(fortify_amt) {
				j = (fortify_amt * 100) / mmaxhp;
				j <<= 1;
				
				SetHudClipRect(305, 17, j, 18, j);
				SetFont("FILLFORT");
				HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_BARFILLID, CR_GREEN, 305.1, 27.0, MONSTERINFO_HOLDTIME);
				
				if(i > 0) {
					SetHudClipRect(305, 17, i, 18, i);
					SetFont("FILLCRIT");
					HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_BARFILLOVERLAY, CR_GREEN, 305.1, 27.0, MONSTERINFO_HOLDTIME);
				}
				SetHudClipRect(0, 0, 0, 0, 0);
			}
			else if(i > 0) {
				SetHudClipRect(305, 17, i, 18, i);
				SetFont("FILLCRIT");
				HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_BARFILLID, CR_GREEN, 305.1, 27.0, MONSTERINFO_HOLDTIME);
				SetHudClipRect(0, 0, 0, 0, 0);
			}
		}
	}
	else {
		SetFont("MNGHPBAR");
		HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_BARID, CR_UNTRANSLATED, 400.0, 27.0, MONSTERINFO_HOLDTIME);
		
		i = (monhp * 100 / mmaxhp);
		if(i > 100)
			i = 100;
		i <<= 1;
		
		if(i > 0) {
			SetHudClipRect(305, 17, i, 18, i);
			SetFont("FILLNORM");
			HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_BARFILLID, CR_GREEN, 305.1, 27.0, MONSTERINFO_HOLDTIME);
			SetHudClipRect(0, 0, 0, 0, 0);
		}
	}

	i = j = 0;
	// show monster traits
	// legendary monsters show no traits at all
	// this delete is added to keep support with new dungeon boss + regular monsters -- hoping a monster gets max 12 traits to show...
	DeleteTextRange(MONSTER_TRAITID, MONSTER_TRAITID + 12);
	
	if(!mon_isPet) {
		if(MonsterProperties[m_id].trait_list[DND_LEGENDARY]) {
			SetFont ("MONFONT");
			HudMessage(s:"\c[D1]", l:"DND_EMOD_LEGENDARY_LONG"; HUDMSG_FADEOUT, MONSTER_TRAITID + j, CR_WHITE, 404.4, 44.0 + 8.0 * j, MONSTERINFO_HOLDTIME);
		}
		else if(MonsterProperties[m_id].trait_list[DND_MARKOFCHAOS]) {
			SetFont ("MONFONT");
			HudMessage(s:"\c[D1]", l:"DND_EMOD_MARKOFCHAOS_NOCOL"; HUDMSG_FADEOUT, MONSTER_TRAITID + j, CR_WHITE, 404.4, 44.0 + 8.0 * j, MONSTERINFO_HOLDTIME);
		}
		else if(MonsterProperties[m_id].hasTrait) {
			SetFont ("MONFONT");
			for(i = 0; i < MAX_MONSTER_TRAITS_SHOWN; ++i) {
				if(MonsterProperties[m_id].trait_list[i]) {
					HudMessage(l:GetMonsterTraitLabel(i); HUDMSG_FADEOUT, MONSTER_TRAITID + j, CR_WHITE, 404.4, 44.0 + 8.0 * j, MONSTERINFO_HOLDTIME);
					++j;
				}
			}
		}
	}
	else if(PetMonsterProperties[m_id].hasTrait) {
		SetFont ("MONFONT");
		for(i = 0; i < MAX_MONSTER_TRAITS_SHOWN; ++i) {
			if(PetMonsterProperties[m_id].trait_list[i]) {
				HudMessage(l:GetMonsterTraitLabel(i); HUDMSG_FADEOUT, MONSTER_TRAITID + j, CR_WHITE, 404.4, 44.0 + 8.0 * j, MONSTERINFO_HOLDTIME);
				++j;
			}
		}
	}
}

void DrawBigBossHPBar(int mon_tid, int mmaxhp, int monhp, int monlevel, int monid, int m_id, int fortify_amt) {
	str barGraphic = "";
	str fortGraphic = "";
	
	SetHudSize(800, 600, 0);
	SetFont(GetMonsterTypeIcon(GetMonsterType(monid, mon_tid)));	
	HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_TYPEICONID, CR_UNTRANSLATED, 340.4, 24.0, MONSTERINFO_HOLDTIME);
	HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_TYPEICONID_RIGHT, CR_UNTRANSLATED, 453.4, 24.0, MONSTERINFO_HOLDTIME);
	
	// inner text
	SetFont ("MONFONT");
	HudMessage(l:"DND_STAT18_SHORT", s:": ", i:monlevel, s:"  ", s:GetIntegerCommaView(monhp), s:" / ", s:GetIntegerCommaView(mmaxhp); HUDMSG_FADEOUT, MONSTER_TEXTID, CR_WHITE, 400.4, 64.1, MONSTERINFO_HOLDTIME);
	
	HudMessage(s:GetActorProperty(mon_tid, APROP_NAMETAG); HUDMSG_FADEOUT, MONSTER_NAMEID, CR_RED, 400.4, 45.1, MONSTERINFO_HOLDTIME);
	
	SetFont("MNBHPBAR");
	HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_BARID, CR_UNTRANSLATED, 400.4, 0.1, MONSTERINFO_HOLDTIME);
	
	// clamp to 100 on certain monsters' overheal abilities (Warmaster for example)
	int hdisp = (monhp * 100 / MonsterProperties[m_id].maxhp);
	if(hdisp > 100)
		hdisp = 100;
	hdisp = 450 * hdisp / 100;
	
	if(monhp) {
		if(fortify_amt) {
			int fortify_disp = (fortify_amt * 450 / MonsterProperties[m_id].maxhp);
		
			SetFont("FILLARBR");
			SetHudClipRect(174, 60, fortify_disp, 18, fortify_disp);
			HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_BARFILLOVERLAY, CR_GREEN, 174.1, 61.1, MONSTERINFO_HOLDTIME);
			
			SetHudClipRect(174, 60, hdisp, 18, hdisp);
			if(hdisp > 0) {
				SetFont("FILLHPBR");
				HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_BARFILLOVERLAY4, CR_GREEN, 174.1, 61.1, MONSTERINFO_HOLDTIME);
				
				SetFont("HPBARFX");
				HudMessage(s:"a"; HUDMSG_FADEOUT | HUDMSG_ALPHA | HUDMSG_ADDBLEND, MONSTER_BARFILLOVERLAY3, CR_GREEN, 174.1, 61.1, MONSTERINFO_HOLDTIME, MONSTERINFO_HOLDTIME, 0.5);
			}
			SetHudClipRect(0, 0, 0, 0, 0);
		}
		else {
			SetFont("FILLHPBR");
			SetHudClipRect(174, 60, hdisp, 18, hdisp);
			HudMessage(s:"a"; HUDMSG_FADEOUT, MONSTER_BARFILLOVERLAY2, CR_GREEN, 174.1, 61.1, MONSTERINFO_HOLDTIME);
			
			SetFont("HPBARFX");
			HudMessage(s:"a"; HUDMSG_FADEOUT | HUDMSG_ALPHA | HUDMSG_ADDBLEND, MONSTER_BARFILLOVERLAY, CR_GREEN, 174.1, 61.1, MONSTERINFO_HOLDTIME, MONSTERINFO_HOLDTIME, 0.5);
			
			SetHudClipRect(0, 0, 0, 0, 0);
		}
	}
	
	DeleteTextRange(MONSTER_TRAITID, MONSTER_TRAITID + 12);

	// legendary monsters show no traits at all
	SetFont ("MONFONT");
	HudMessage(s:"\c[D1]", l:"DND_EMOD_DUNGEONBOSS"; HUDMSG_FADEOUT, MONSTER_TRAITID, CR_WHITE, 400.4, 84.1, MONSTERINFO_HOLDTIME);
}

Script "DnD Boss HP FX Overlay" (int tid) CLIENTSIDE {
	int m_id = tid - DND_MONSTERTID_BEGIN;
	int counter = 0, alpha;
	int hdisp, fdisp;
	int draw_id = MONSTER_BARFILLID;
	while(DungeonBossData[BOSSDATA_TID]) {
		// get health percentage of boss and map it to a range of 0-450 from 0-100 instead
		hdisp = 450 * GetHealthPercentage(DungeonBossData[BOSSDATA_HP], MonsterProperties[m_id].maxhp) / 100;
		/*if(hdisp > 100)
			hdisp = 100;
		hdisp = 450 * hdisp / 100;*/
		
		fdisp = 450 * GetHealthPercentage(DungeonBossData[BOSSDATA_FORT], MonsterProperties[m_id].maxhp) / 100;
		
		alpha = Clamp_Between(DungeonBossData[BOSSDATA_DMGTAKEN] * 0.008, 0.0, 1.0);
		
		if(DungeonBossData[BOSSDATA_DMGTRIGGER]) {
			counter = 25;
			DungeonBossData[BOSSDATA_DMGTRIGGER] = 0;
		}
		
		if(!CheckInventory("TargetTID") || CheckInventory("TargetTID") == DungeonBossData[BOSSDATA_TID]) {
			SetHudSize(800, 600, 0);
			SetHudClipRect(174, 60, hdisp, 18, hdisp);
			SetFont("HPBARFXP");
				
			if(fdisp)
				draw_id = MONSTER_BARFILLOVERLAY2;
			else
				draw_id = MONSTER_BARFILLID;
			
			HudMessage(s:"a"; HUDMSG_FADEOUT | HUDMSG_ALPHA | HUDMSG_ADDBLEND, draw_id, CR_GREEN, 174.1, 61.1, MONSTERINFO_HOLDTIME, MONSTERINFO_HOLDTIME, alpha);
			SetHudClipRect(0, 0, 0, 0, 0);
		}
		
		Delay(const:1);
		
		if(counter) {
			--counter;
			DungeonBossData[BOSSDATA_DMGTAKEN] = (DungeonBossData[BOSSDATA_DMGTAKEN] * SmoothStop3(counter * 1.0 / 25)) >> 16;
		}
	}
	SetResultValue(0);
}

// ran when the boss awakes and a player dies (so that the above script doesn't stop functioning on their death when the boss is still active)
bool BossBarDrawnForPlayer[MAXPLAYERS];
Script "DnD Unique Boss Bar Draw SpecOnly" (int tid, int diedNoLives) CLIENTSIDE {
	int cpn = ConsolePlayerNumber();
	if(!PlayerIsSpectator(cpn) && !diedNoLives)
		Terminate;
	
	if(!BossBarDrawnForPlayer[cpn])
		ACS_NamedExecuteWithResult("DnD Boss HP FX Overlay", tid);
		
	BossBarDrawnForPlayer[cpn] = true;

	while(DungeonBossData[BOSSDATA_TID]) {
		int m_id = DungeonBossData[BOSSDATA_TID] - DND_MONSTERTID_BEGIN;
		DrawBigBossHPBar(
			DungeonBossData[BOSSDATA_TID],
			MonsterProperties[m_id].maxhp,
			DungeonBossData[BOSSDATA_HP],
			MonsterProperties[m_id].level,
			MonsterProperties[m_id].id,
			m_id,
			DungeonBossData[BOSSDATA_FORT]
		);
		Delay(const:2);
	}
	
	BossBarDrawnForPlayer[cpn] = false;
}

Script "DnD Homing Lock-on Draw" (int width, int height, int range) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	range <<= 16;

	int this = ActivatorTID();

	int r = 1.0 * LOCKON_HUD_Y / LOCKON_HUD_X;
	while(isAlive() && !CheckInventory("DnD_RemoveLockOn")) {
		int w = Clamp_Between(((width / 2) * r) & 0xFFFF0000, 0, (GetHudRight(getcvar("vid_defwidth") - (LOCKON_LINE_LEN >> 16)) << 16) / 2);
		int h = Clamp_Between(((height / 2) * r) & 0xFFFF0000, 0, LOCKON_HUD_YF / 2 - (FixedMul(LOCKON_LINE_LEN, r) & 0xFFFF0000));
	
		SetHudSize(LOCKON_HUD_X, LOCKON_HUD_Y, 1);
		// left corner
		SetFont("100pxD");
		HudMessage(
			s:"A";
			HUDMSG_PLAIN,
			LOCKON_ID_DOWN,
			CR_UNTRANSLATED,
			LOCKON_HUD_XF / 2 - w,
			LOCKON_HUD_YF / 2 + h,
			0
		);
		
		// right corner
		SetFont("100pxU");
		HudMessage(
			s:"A";
			HUDMSG_PLAIN,
			LOCKON_ID_UP,
			CR_UNTRANSLATED,
			LOCKON_HUD_XF / 2 + w,
			LOCKON_HUD_YF / 2 - h,
			0
		);

		// run this on server-side instead periodically and check an inventory item
		if(!HandleTracerPicking(this, range, width, height, true, true))
			HudMessage(s:""; HUDMSG_PLAIN, LOCKON_ID_MARKER, 0, 0, 0, 0.01);

		Delay(const:6);
	}
	HudMessage(s:""; HUDMSG_PLAIN, LOCKON_ID_DOWN, 0, 0, 0, 0.01);
	HudMessage(s:""; HUDMSG_PLAIN, LOCKON_ID_UP, 0, 0, 0, 0.01);
}

Script "DnD Clear Homing Lock-on" (void) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;
	HudMessage(s:""; HUDMSG_PLAIN, LOCKON_ID_DOWN, 0, 0, 0, 0.01);
	HudMessage(s:""; HUDMSG_PLAIN, LOCKON_ID_UP, 0, 0, 0, 0.01);
	HudMessage(s:""; HUDMSG_PLAIN, LOCKON_ID_MARKER, 0, 0, 0, 0.01);
}

#endif
