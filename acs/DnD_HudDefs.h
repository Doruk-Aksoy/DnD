#ifndef DND_HUDDEFS_IN
#define DND_HUDDEFS_IN

#include "DnD_EliteInfo.h"

#define ASPECT_4_3 (4.0 / 3)
#define ASPECT_5_4 1.25
#define ASPECT_16_9 (16.0 / 9)
#define ASPECT_16_10 1.6

#define MAX_SCREENRES_OFFSETS 5
#define SCREEN_ASPECT_RATIO 4
int ScreenResOffsets[MAX_SCREENRES_OFFSETS] = { -1, -1, -1, -1, ASPECT_4_3 };

#define INVENTORYINFO_NORMALVIEW_WRAPX 272.0
#define INVENTORYINFO_NORMALVIEW_WRAPY 152.0

#define INVENTORYINFO_TRADEVIEW_WRAPX 304.0
#define INVENTORYINFO_TRADEVIEW_WRAPY 200.0

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

// Fillable width of the two monster health bars, each in the hud size its own drawing sets up --
// 720x600 for the target bar, 800x600 for the dungeon boss one. These are what the fill is scaled
// to, so they have to match the artwork's bar interior.
#define DND_MONSTERBAR_WIDTH 200
#define DND_BOSSBAR_WIDTH 450
#define EXIT_HOLDTIME 0.5

#define MILLION_MINUS_ONE 999999
#define THOUSAND_MINUS_ONE 999

#define TIPBOX_TOPLEN 70.0
#define TIPBOX_MIDLEN 39.0

#define ITEMINFOBG_TOPLEN 17.0
#define ITEMINFOBG_MIDLEN 9.0
#define ITEMINFOBG_MAXMIDS 10
#define ITEMINFO_CLIPWIDTH 360

// Item info panel text layout. These are HUDTEXTMAX pixels -- the panel art is drawn in HUDMAX, so
// the two never mix without the 3/2 between them.
#define ITEMINFO_LINEH 8			// NSMOLFNT height, ie. the engine's line step
#define ITEMINFO_LINEHF 8.0
#define ITEMINFO_TEXTTOP 20.0		// panel top -> title row
#define ITEMINFO_TITLEGAP 2.0		// title bottom -> implicit top
#define ITEMINFO_SEPGAP 4.0			// implicit bottom -> separator top
#define ITEMINFO_SEPH 7.0			// IMPSEPR
#define ITEMINFO_ATTRGAP 5.0		// separator bottom -> attribute top
#define ITEMINFO_BOTTOMPAD 6.0		// last attribute line -> where the panel may end

// Panel art: LDTITTOR is 17 rows ending in 3 of black interior, LDTITBOR is 21 opening with 9,
// LDTITMIR is 9 and all interior. In HUDTEXTMAX pixels the interior therefore starts
// (17 - 3) * 3 / 2 = 21 below the panel top and is (3 + 9) * 3 / 2 = 18 tall before any mids. Text
// starts at ITEMINFO_TEXTTOP, so 21 + 18 - 20 of it rides on the caps alone.
#define ITEMINFOBG_CAPTEXTLEN 19

// A mid is ITEMINFOBG_MIDLEN * 3 / 2 = 13.5 text pixels, so mids are counted in halves to stay
// in integers.
#define ITEMINFOBG_MIDTEXT2 27

// The text is centred on the panel and wraps at HUD_ITEMBAK_WIDTH, so the clip has to start where
// a full width line does -- anything to the right of that shaves the leading character off.
#define ITEMINFO_CLIPX ((HUD_ITEMBAK_X * HUDTEXTMAX_X / (2 * HUDMAX_X)) - HUD_ITEMBAK_WIDTH / 2)

#define ITEMID_SKIP (-10 - ITEMINFOBG_MAXMIDS)

typedef struct coord {
	int x;
	int y;
} coord_T;

// x is scroll current pos, y is scroll limit
coord_T ScrollPos = { 0, 0 };
//coord_T HudScale = { 1.0, 1.0 };

// Everything the scroll bar knows, in one place. All of it is clientside and menu lifetime only.
struct scrollbar_T {
	// What the page in view reported to ListenScroll this tic. range x is the lowest position it
	// will take (bottom of the content) and y the highest (top); content x is how many pixels one
	// position moves that page and y is the height of the window it draws inside. There is nowhere
	// else any of this lives -- a page states it in the ListenScroll call and nowhere else.
	coord_T range;
	coord_T content;

	// Where the cursor sat and what the view read when a drag began. Both are needed because the
	// drag is relative: measuring from the cursor's absolute position instead would snap the thumb
	// under the cursor on the first frame no matter where along the thumb it was grabbed.
	int grab_y;
	int grab_pos;

	// Where this bar's track is drawn, in screen pixels. A page states it in the same ListenScroll
	// call that states its range, because a bar that spans the whole page while scrolling a strip at
	// the bottom of it is not a control, it is a riddle -- which is exactly how the perk tree's
	// detail panel read before it got a track of its own.
	int track_y;
	int track_h;

	// Where this bar is scrolled to. Bar 0 does NOT use this: its position stays in ScrollPos.x,
	// which every existing page reads directly when it draws and writes directly when it resets.
	// Moving that would have meant touching all of them; see GetScrollBarPos.
	int pos;

	// listened separates "this page scrolls, and the two above describe it" from "this page has no
	// scroll at all", which is the difference between drawing a bar and not drawing one.
	bool listened;
	bool grabbed;

	// What the frame before settled on for the two things the bar draws differently. Neither is
	// part of the every-tic draw, so the frame either changes on has to ask for the redraw itself
	// or nothing will.
	bool drawn;
	bool lit;
};

// Two bars, because one page needs two independent ones. Every other page uses bar 0 and never
// mentions the second, which stands down on its own -- a bar nothing called ListenScroll for has
// listened == false and is not drawn.
#define DND_SCROLLBAR_COUNT 2

scrollbar_T module& GetScrollBar(int id = 0) {
	static scrollbar_T bars[DND_SCROLLBAR_COUNT];
	return bars[id];
}

// The bar sits in the strip between where the page's text stops and where the panel's right hand
// frame begins. Every page wraps its text 256 wide from x 192, so nothing it draws can reach past
// 448 -- starting at 449 means the bar never lands on a line of text no matter how long the line
// runs, which picking a column inside the content area could not promise. All of these are in the
// HUDMAX_X x HUDMAX_Y space the menu draws its text in.
#define DND_SCROLLBAR_X 449
#define DND_SCROLLBAR_W 7
#define DND_SCROLLBAR_Y 40
#define DND_SCROLLBAR_H 240

// Rows in SCRLTHMB.png, which is stacked bands rather than one strip because the handle's height
// is produced by clipping and anything at a fixed distance from one end would shear off the other.
// The draw anchors the art once per band to land each where it belongs. These mirror the row
// numbers in the art, so the two have to move together.
#define DND_SCROLLBAR_THUMBCAP 0		// bottom shadow, onto the handle's last row
#define DND_SCROLLBAR_THUMBGRIP 4		// notches, centred on the handle
#define DND_SCROLLBAR_THUMBGRIPH 9
#define DND_SCROLLBAR_THUMBBODY 16		// top highlight, then uniform body below it

// below this the notches crowd the whole handle instead of reading as a grip on it
#define DND_SCROLLBAR_THUMBGRIPMIN 28

// what the thumb falls back to when the page hasn't declared its content size
#define DND_SCROLLBAR_THUMBDEFAULT 32
// small enough to still mean "there is a lot below", large enough to still be a mouse target
#define DND_SCROLLBAR_THUMBMIN 16
// a thumb that filled its track would leave nowhere to drag it to. Against the BAR's track rather
// than the default one, because a short bar's thumb has to be short enough to move within it.
#define DND_SCROLLBAR_THUMBMAX(track_h) ((track_h) - 8)

#define DND_SCROLLBAR_IDSTRIDE (RPGMENUSCROLLGRIPID2 - RPGMENUSCROLLGRIPID)

#define DND_SCROLLBAR_XF (DND_SCROLLBAR_X << 16)
#define DND_SCROLLBAR_YF (DND_SCROLLBAR_Y << 16)

// how far either side of the bar a click still counts as grabbing it -- 7 pixels is a small
// target to hit with a cursor that moves at mouse speed
#define DND_SCROLLBAR_GRABPAD 5

// The cursor runs the other way to the screen (top left is 1:1, see point_in_box), so the bar's
// screen rectangle has to be flipped into cursor space before anything can be tested against it. The
// y pair takes the bar's own track, since two bars share the column and only their y separates them.
#define DND_SCROLLBAR_CURSOR_XMAX ((HUDMAX_X - DND_SCROLLBAR_X + DND_SCROLLBAR_GRABPAD) << 16)
#define DND_SCROLLBAR_CURSOR_XMIN ((HUDMAX_X - DND_SCROLLBAR_X - DND_SCROLLBAR_W - DND_SCROLLBAR_GRABPAD) << 16)
#define DND_SCROLLBAR_CURSOR_YMAX(track_y) ((HUDMAX_Y - (track_y)) << 16)
#define DND_SCROLLBAR_CURSOR_YMIN(track_y, track_h) ((HUDMAX_Y - (track_y) - (track_h)) << 16)

#define BOSSDATA_TID 0 // unique boss tid, this is typically dungeon boss or one off boss encounters in maps
#define BOSSDATA_HP 1 // unique boss hp, monster health isnt synced to clients
#define BOSSDATA_FORT 2
#define BOSSDATA_DMGTAKEN 3
#define BOSSDATA_DMGTRIGGER 4
#define BOSSDATA_AWAKENED 5
int DungeonBossData[6] = { 0, 0, 0, 0, 0, 0 };

// Box definitions for clickables
// 7 images on menu leftmost bar
#define MAX_MAIN_BOXES 10
#define MAX_TAGCHARS_SHOWN 17

// Mainbox Labels
enum {
	MAINBOX_NONE = 0,
	MAINBOX_STATS,
	MAINBOX_PERK,
	MAINBOX_LOAD,
	MAINBOX_SHOP,
	MAINBOX_RESEARCH,
	MAINBOX_ABILITY,
	MAINBOX_HELP,
	MAINBOX_LARR,
	MAINBOX_RET,
	MAINBOX_RARR
};

// Various box enums (I know this part is terrible, dk how to make it look better atm)
// Definition rule: L -> R, T -> B
enum {
	MBOX_1 = 1,
	MBOX_2,
	MBOX_3,
	MBOX_4,
	MBOX_5,
	MBOX_6,
	MBOX_7,
	MBOX_8,
	MBOX_9,
	MBOX_10,
	MBOX_11,
	MBOX_12,
	MBOX_13,
	MBOX_14,
	MBOX_15,
	MBOX_16
};

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

	// AttachMessage keeps its list sorted by id and draws the highest first, so a LOWER id lands
	// in front -- which is why the cursor is 100 and the panel background is the largest number
	// here. These two have to come in front of that background and behind nothing else the page
	// draws, so they go directly between the two, and the thumb has to sit below the track for
	// the same reason or the track paints over it.
	// Below RPGMENUBACKGROUNDID also keeps them clear of RPGMENUBACKGROUNDID + 1, which the
	// prompt and vote banners write to.
	// the handle's three bands, then its groove, each in front of the one after it
	// One group of four per bar, contiguous and in the same order, so bar n's ids are the first
	// group plus n * DND_SCROLLBAR_IDSTRIDE. The background follows the last group rather than
	// sitting at a fixed number, so adding a bar cannot quietly overlap it -- everything that
	// references it, including the prompt and vote banners at + 1, does so by name.
	RPGMENUSCROLLGRIPID = 2151,
	RPGMENUSCROLLCAPID,
	RPGMENUSCROLLTHUMBID,
	RPGMENUSCROLLTRACKID,

	RPGMENUSCROLLGRIPID2,
	RPGMENUSCROLLCAPID2,
	RPGMENUSCROLLTHUMBID2,
	RPGMENUSCROLLTRACKID2,

	RPGMENUBACKGROUNDID,

	// monster scanner hud id stuff
	MONSTER_TYPEICONID = 2300,
	MONSTER_TYPEICONID_RIGHT,
	MONSTER_NAMEID,
	MONSTER_TEXTID,
	MONSTER_BARFILL_OVERLAY1,
	MONSTER_BARFILL_OVERLAY2,
	MONSTER_BARFILLID,
	MONSTER_BARFILLOVERLAY,
	MONSTER_BARFILLOVERLAY2,
	MONSTER_BARFILLOVERLAY3,
	MONSTER_BARFILLOVERLAY4,
	MONSTER_TRAITID = 2400,
	
	HUD_GRAPHIC_ID = 2600,
	HUD_GRAPHIC_ID_2,

	HUD_STAMINA_FOREGROUND,
	HUD_STAMINA_PARRYCDID,
	HUD_STAMINA_PARRYDICID,
	HUD_STAMINA_ICON,

	BLACKOUT_ID = 2699,
	HUDANIMATED_ID,
	ESHIELD_LEFT_ID = 3000,
	ESHIELD_RIGHT_ID
};

void ClearMonsterScanInfo() {
	DeleteTextRange(MONSTER_TYPEICONID, MONSTER_BARFILLOVERLAY4);
	DeleteTextRange(MONSTER_TRAITID, MONSTER_TRAITID + MAX_MONSTER_TRAITS_SHOWN);
}

#endif