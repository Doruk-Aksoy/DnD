#ifndef DND_SPELLTREE_IN
#define DND_SPELLTREE_IN

#include "DnD_SpellHud.h"

// The page borrows CRFTVW, the crafting backdrop: a wide board on the left for the tree and a tall
// pocket on the right for text. It is drawn exactly the way the crafting page draws it -- enlarged
// canvas for the BACKDROP ONLY, restored immediately, everything else in the default HUDMAX space.
//
// That split is not a style choice. Node hit boxes go through AddBoxToPane_Points, which is always
// in the default space whatever SetHudSize the drawing used. Drawing content in a second space meant
// boxes and icons came from different numbers: the boxes worked and the icons never appeared, so
// hovering played its sound over a blank page. One space for everything that is not the backdrop.
#define DND_SPELLTREE_BACKW 384
#define DND_SPELLTREE_BACKH 200

// The art is 64x64 and a graphic renders ONE UNIT PER SOURCE PIXEL, so at menu scale an icon is 64
// units across -- eight of those is 512, and the whole board is about 322. They are drawn in a 2x
// canvas instead, which halves them to an effective 32 units and needs no new art.
//
// DND_SPELLNODE is that EFFECTIVE size, in menu units. Everything on the board derives from it, and
// only the icon draw itself switches canvas. Safe now that the id z-order is fixed: a second space
// was never the reason nothing rendered.
// At 32 units the grid IS the board -- 7 rows leave a 1 unit gap, and a connector needs somewhere to
// run. A 3x canvas takes the icons to 21 units and buys 13 units between rows and 15 between
// columns, which is what the lines are drawn in. Raise ICONSCALE back to 2 for bigger icons and the
// connectors have nowhere to go.
#define DND_SPELLNODE 21
#define DND_SPELLTREE_ICONSCALE 3

// The board, left of the pocket: usable is roughly x 16..338 by y 46..285. Fire is the widest tree
// at 8 columns by 7 rows, and at this icon size that is the whole board -- the gaps are what is
// left over, not a choice.
#define DND_SPELLTREE_GAPX 15
#define DND_SPELLTREE_GAPY 13
#define DND_SPELLTREE_COLW (DND_SPELLNODE + DND_SPELLTREE_GAPX)
#define DND_SPELLTREE_ROWH (DND_SPELLNODE + DND_SPELLTREE_GAPY)

// The board the grid is centred in, rather than a fixed origin. Trees are not the same shape --
// Fire is 8 columns by 7 rows, Cold 6 by 6 -- so a shared origin left the smaller ones hugging the
// top left corner with the whole right half of the board empty.
#define DND_SPELLTREE_LEFT 16
#define DND_SPELLTREE_BOARDW 322
#define DND_SPELLTREE_TOP 46
#define DND_SPELLTREE_BOARDH 239

// Connector tiles, drawn as single-picture fonts: SPLLINEV is 5x8 and stacks downward, SPLLINEH is
// 8x5 and runs sideways, SPLLJOIN is a 5x5 plug for the corner where the two meet. The steps are the
// tile sizes along the run, and the cap is a guard on the repeat strings.
// Joints are 7x7; the straights are 14 long so a span needs half as many tiles, which is what
// keeps the whole tree inside the hud id budget.
#define DND_SPELLLINE_JOINT 7
#define DND_SPELLLINE_LEN 14

// Per edge: 2 stubs, then up to 4 + 4 vertical and 10 horizontal tiles. The corners went to the
// junction pass, so their two ids came back here.
//
// MAXH must COVER the widest edge in any tree or the crossbar stops in mid air with its legs left
// hanging -- the widest is 108 units, which is 8 tiles, and the old cap of 6 cut it by two.
//
// Fire is the busiest tree at 27 edges, so the band runs 1150 + 27*20 = 1690, clear of RPGMENUID.
#define DND_SPELLLINE_IDS 20
#define DND_SPELLLINE_MAXV 4
#define DND_SPELLLINE_MAXH 10

// How far the tree reaches, so it can be centred. Authored positions, so this is the tree's shape
// and not the player's progress.
// These walk the tree's own block rather than the whole id space, and skip the empty slots in it --
// a zeroed def would otherwise read as a Fire spell at (0,0).
int GetTreeMaxTX(int tree) {
	int i, s, m = 0;
	for(i = 0; i < DND_SPELLS_PER_TREE; ++i) {
		s = GetTreeFirstSpell(tree) + i;
		if(IsSpellDefined(s) && SpellDefs[s].tx > m)
			m = SpellDefs[s].tx;
	}
	return m;
}

int GetTreeMaxTY(int tree) {
	int i, s, m = 0;
	for(i = 0; i < DND_SPELLS_PER_TREE; ++i) {
		s = GetTreeFirstSpell(tree) + i;
		if(IsSpellDefined(s) && SpellDefs[s].ty > m)
			m = SpellDefs[s].ty;
	}
	return m;
}

int GetSpellNodeX(int spell) {
	int tree = SpellDefs[spell].tree;
	int w = GetTreeMaxTX(tree) * DND_SPELLTREE_COLW + DND_SPELLNODE;

	return DND_SPELLTREE_LEFT + (DND_SPELLTREE_BOARDW - w) / 2 +
		SpellDefs[spell].tx * DND_SPELLTREE_COLW;
}

int GetSpellNodeY(int spell) {
	int tree = SpellDefs[spell].tree;
	int h = GetTreeMaxTY(tree) * DND_SPELLTREE_ROWH + DND_SPELLNODE;

	return DND_SPELLTREE_TOP + (DND_SPELLTREE_BOARDH - h) / 2 +
		SpellDefs[spell].ty * DND_SPELLTREE_ROWH;
}

// Boxes always live in the default HUDMAX space whatever SetHudSize the drawing used, and both axes
// run from the far edge -- see the note above AddBoxToPane_Points.
//
// The box has to match where the icon LANDS. A GRAPHIC anchors at its TOP LEFT on both axes, so the
// icon occupies (x, y) to (x + DND_SPELLNODE, y + DND_SPELLNODE) and its centre is half a node in on
// each. Centring the box on the draw coordinate instead put it half a node up and to the left, which
// is why the right third of an icon selected its neighbour and its bottom third selected the row
// below.
//
// Note this differs from TEXT, which the menu's own rows show centred on y -- a label drawn at
// (192.1, 88) owns the box y 82..94. Do not carry one convention over to the other.
void AddSpellBoxAt(menu_pane_T module& p, int x, int y) {
	int h = DND_SPELLNODE / 2;
	int cx = x + h;
	int cy = y + h;

	AddBoxToPane_Points(p,
		(HUDMAX_X - (cx - h)) << 16, (HUDMAX_Y - (cy - h)) << 16,
		(HUDMAX_X - (cx + h)) << 16, (HUDMAX_Y - (cy + h)) << 16);
}

void AddSpellNodeBox(menu_pane_T module& p, int spell) {
	int h = DND_SPELLNODE / 2;
	int cx = GetSpellNodeX(spell) + h;
	int cy = GetSpellNodeY(spell) + h;

	AddBoxToPane_Points(p,
		(HUDMAX_X - (cx - h)) << 16, (HUDMAX_Y - (cy - h)) << 16,
		(HUDMAX_X - (cx + h)) << 16, (HUDMAX_Y - (cy + h)) << 16);
}

// Connectors are drawn from the requirement data rather than authored, so an edge can never
// disagree with the rule that produced it.
//
// Every piece is a 7x7 tile from one set, so the routing only ever has to say WHICH tile goes where:
//
//   SPLLND   stub leaving the parent's bottom edge      SPLLNU   stub meeting the child's top edge
//   SPLLNV   straight vertical run                      SPLLNH   straight horizontal run
//   SPLLCUL  from above, turning left                   SPLLCUR  from above, turning right
//   SPLLCDL  from the left, turning down                SPLLCDR  from the right, turning down
//
// The stubs are what stop a line from simply running under an icon -- it visibly meets the edge and
// stops there.
// Where two runs genuinely pass through each other a CROSS belongs, not two bands cutting each
// other's core. Nothing about one edge can know that, so each edge logs the segments it drew and a
// second pass over the log places the crosses.
//
// Map arrays, so they cost nothing between maps. Fire is the busiest tree at 27 edges, which is at
// most 54 verticals and 27 horizontals.
#define DND_SPELLTREE_MAXSEG 64
#define DND_SPELLTREE_MAXCROSS 48
#define DND_SPELLTREE_MAXJOINT 96

// One accessor for everything the tree draw scribbles on, function static rather than module scope
// -- the house rule for anything array sized. The segment log, the junction candidates it produces,
// and the pocket's text cursor all live and die inside a single draw, so they share a home.
typedef struct {
	int vx[DND_SPELLTREE_MAXSEG], vy0[DND_SPELLTREE_MAXSEG], vy1[DND_SPELLTREE_MAXSEG];
	int hy[DND_SPELLTREE_MAXSEG], hx0[DND_SPELLTREE_MAXSEG], hx1[DND_SPELLTREE_MAXSEG];
	int vcount, hcount;

	int jx[DND_SPELLTREE_MAXJOINT], jy[DND_SPELLTREE_MAXJOINT], jcount;

	int panel_y, panel_id;
} spell_draw_T;

spell_draw_T module& GetSpellDraw() {
	static spell_draw_T s;
	return s;
}

void ResetTreeSegments() {
	GetSpellDraw().vcount = 0;
	GetSpellDraw().hcount = 0;
}

void LogSegmentV(int x, int y0, int y1) {
	if(GetSpellDraw().vcount >= DND_SPELLTREE_MAXSEG)
		return;
	GetSpellDraw().vx[GetSpellDraw().vcount] = x;
	GetSpellDraw().vy0[GetSpellDraw().vcount] = y0;
	GetSpellDraw().vy1[GetSpellDraw().vcount] = y1;
	++GetSpellDraw().vcount;
}

void LogSegmentH(int y, int x0, int x1) {
	if(GetSpellDraw().hcount >= DND_SPELLTREE_MAXSEG)
		return;
	GetSpellDraw().hy[GetSpellDraw().hcount] = y;
	GetSpellDraw().hx0[GetSpellDraw().hcount] = x0;
	GetSpellDraw().hx1[GetSpellDraw().hcount] = x1;
	++GetSpellDraw().hcount;
}

// Every piece is placed INDIVIDUALLY at a computed top-left. A multi glyph run's true length is
// the font's glyph advance times the count, which this code cannot know -- every version built on
// runs either left gaps at the corners or staggered the tiles apart. One HudMessage per tile costs
// ids and buys exact placement.
//
// Both coordinates use the .1 suffix, so each is the tile's TOP LEFT. Nothing here relies on an
// alignment mode.
void DrawLineTileAt(str tile, int x, int y, int id, str col) {
	SetFont(tile);
	HudMessage(s:col, s:"A"; HUDMSG_PLAIN, id, CR_UNTRANSLATED,
		(x << 16) + 0.1, (y << 16) + 0.1, 0.0, 0.0);
}

// Fills y0..y1 exactly: whole tiles while they fit, then one flush to the end. The overlap that
// leaves in the middle is invisible; a gap at either end is not.
void DrawLineSpanV(int cx, int y0, int y1, int id, int maxn, str col) {
	int n = 0, y = y0;
	int x = cx - DND_SPELLLINE_JOINT / 2;

	while(y + DND_SPELLLINE_LEN <= y1 && n < maxn) {
		DrawLineTileAt("SPLLNV", x, y, id + n, col);
		y += DND_SPELLLINE_LEN;
		++n;
	}

	// The flush tile is only legal when a WHOLE one still fits inside the span. Between neighbouring
	// rows the gap is 13 and a tile is 14, so y1 - LEN landed ABOVE y0 and the line grew backwards out
	// of the parent. A span that short needs no tile at all -- the two stubs already meet.
	if(y < y1 && n < maxn && y1 - DND_SPELLLINE_LEN >= y0) {
		DrawLineTileAt("SPLLNV", x, y1 - DND_SPELLLINE_LEN, id + n, col);
		++n;
	}

	DeleteTextRange(id + n, id + maxn - 1);
}

void DrawLineSpanH(int cy, int x0, int x1, int id, int maxn, str col) {
	int n = 0, x = x0;
	int y = cy - DND_SPELLLINE_JOINT / 2;

	while(x + DND_SPELLLINE_LEN <= x1 && n < maxn) {
		DrawLineTileAt("SPLLNH", x, y, id + n, col);
		x += DND_SPELLLINE_LEN;
		++n;
	}
	if(x < x1 && n < maxn && x1 - DND_SPELLLINE_LEN >= x0) {
		DrawLineTileAt("SPLLNH", x1 - DND_SPELLLINE_LEN, y, id + n, col);
		++n;
	}

	DeleteTextRange(id + n, id + maxn - 1);
}

void DrawTreeConnector(int fromspell, int tospell, int id, str col) {
	int h = DND_SPELLNODE / 2;
	int jt = DND_SPELLLINE_JOINT;

	int px = GetSpellNodeX(fromspell) + h;
	int py = GetSpellNodeY(fromspell) + DND_SPELLNODE;	// parent's bottom edge
	int cx = GetSpellNodeX(tospell) + h;
	int cy = GetSpellNodeY(tospell);					// child's top edge
	int midy = (py + cy) / 2;

	// Joints take the LOW ids of the block so they sit in front of the spans they cap. The icons are
	// lower still, so anything reaching into a node goes behind it.
	// No corners here. An edge cannot see the others, and at a shared parent its "corner" is really
	// a tee -- DrawTreeJunctions decides every joint from the arms actually present.
	//
	// The stubs take the lowest ids so they cap each leg in front of it, which is what stops a line
	// running under an icon.
	DrawLineTileAt("SPLLND", px - jt / 2, py, id, col);
	DrawLineTileAt("SPLLNU", cx - jt / 2, cy - jt, id + 1, col);

	DrawLineSpanV(px, py, midy, id + 2, DND_SPELLLINE_MAXV, col);
	DrawLineSpanV(cx, midy, cy, id + 6, DND_SPELLLINE_MAXV, col);

	if(px != cx)
		DrawLineSpanH(midy, Min(px, cx), Max(px, cx), id + 10, DND_SPELLLINE_MAXH, col);
	else
		DeleteTextRange(id + 10, id + 10 + DND_SPELLLINE_MAXH - 1);

	// Logged for the crossing pass, which runs once the whole tree is down.
	LogSegmentV(px, py, midy);
	LogSegmentV(cx, midy, cy);
	if(px != cx)
		LogSegmentH(midy, Min(px, cx), Max(px, cx));
}

// Which arms a point actually has, asked of EVERY segment rather than of the one edge that happened
// to draw it. Two spells hanging off one parent put a corner and a passing run at the same spot, and
// an L there is simply the wrong piece -- it is a tee.
#define JARM_U 1
#define JARM_D 2
#define JARM_L 4
#define JARM_R 8

void AddJoint(int x, int y) {
	for(int i = 0; i < GetSpellDraw().jcount; ++i)
		if(GetSpellDraw().jx[i] == x && GetSpellDraw().jy[i] == y)
			return;

	if(GetSpellDraw().jcount >= DND_SPELLTREE_MAXJOINT)
		return;

	GetSpellDraw().jx[GetSpellDraw().jcount] = x;
	GetSpellDraw().jy[GetSpellDraw().jcount] = y;
	++GetSpellDraw().jcount;
}

int JointArms(int x, int y) {
	int i, m = 0;

	for(i = 0; i < GetSpellDraw().vcount; ++i) {
		if(GetSpellDraw().vx[i] != x)
			continue;
		if(GetSpellDraw().vy0[i] < y && y <= GetSpellDraw().vy1[i])	m |= JARM_U;
		if(GetSpellDraw().vy0[i] <= y && y < GetSpellDraw().vy1[i])	m |= JARM_D;
	}

	for(i = 0; i < GetSpellDraw().hcount; ++i) {
		if(GetSpellDraw().hy[i] != y)
			continue;
		if(GetSpellDraw().hx0[i] < x && x <= GetSpellDraw().hx1[i])	m |= JARM_L;
		if(GetSpellDraw().hx0[i] <= x && x < GetSpellDraw().hx1[i])	m |= JARM_R;
	}

	return m;
}

// Straight-through and dead ends need no piece: the run already covers them.
str JointTile(int m) {
	switch(m) {
		case JARM_U | JARM_L:					return "SPLLCUL";
		case JARM_U | JARM_R:					return "SPLLCUR";
		case JARM_D | JARM_L:					return "SPLLCDL";
		case JARM_D | JARM_R:					return "SPLLCDR";
		case JARM_U | JARM_D | JARM_L:			return "SPLLTL";
		case JARM_U | JARM_D | JARM_R:			return "SPLLTR";
		case JARM_L | JARM_R | JARM_U:			return "SPLLTU";
		case JARM_L | JARM_R | JARM_D:			return "SPLLTD";
		case JARM_U | JARM_D | JARM_L | JARM_R:	return "SPLLX";
	}
	return "";
}

void DrawTreeJunctions(str col) {
	int i, j, n = 0;
	int jt = DND_SPELLLINE_JOINT;
	str tile;

	GetSpellDraw().jcount = 0;

	// Every segment end is a candidate, and so is every genuine crossing.
	for(i = 0; i < GetSpellDraw().vcount; ++i) {
		AddJoint(GetSpellDraw().vx[i], GetSpellDraw().vy0[i]);
		AddJoint(GetSpellDraw().vx[i], GetSpellDraw().vy1[i]);
	}
	for(i = 0; i < GetSpellDraw().hcount; ++i) {
		AddJoint(GetSpellDraw().hx0[i], GetSpellDraw().hy[i]);
		AddJoint(GetSpellDraw().hx1[i], GetSpellDraw().hy[i]);
	}
	for(i = 0; i < GetSpellDraw().hcount; ++i)
		for(j = 0; j < GetSpellDraw().vcount; ++j)
			if(GetSpellDraw().hy[i] > GetSpellDraw().vy0[j] && GetSpellDraw().hy[i] < GetSpellDraw().vy1[j] &&
				GetSpellDraw().vx[j] > GetSpellDraw().hx0[i] && GetSpellDraw().vx[j] < GetSpellDraw().hx1[i])
				AddJoint(GetSpellDraw().vx[j], GetSpellDraw().hy[i]);

	for(i = 0; i < GetSpellDraw().jcount && n < DND_SPELLTREE_MAXCROSS; ++i) {
		tile = JointTile(JointArms(GetSpellDraw().jx[i], GetSpellDraw().jy[i]));
		if(!StrLen(tile))
			continue;

		DrawLineTileAt(tile, GetSpellDraw().jx[i] - jt / 2, GetSpellDraw().jy[i] - jt / 2, SPELLTREE_CROSS_ID + n, col);
		++n;
	}

	DeleteTextRange(SPELLTREE_CROSS_ID + n, SPELLTREE_CROSS_ID + DND_SPELLTREE_MAXCROSS - 1);
}

// Spells belonging to this tree, in id order. The index is also the box index, so a click resolves
// back through the same walk.
int GetTreeSpellByIndex(int tree, int index) {
	int i, s, n = 0;
	for(i = 0; i < DND_SPELLS_PER_TREE; ++i) {
		s = GetTreeFirstSpell(tree) + i;
		if(!IsSpellDefined(s))
			continue;
		if(n == index)
			return s;
		++n;
	}
	return -1;
}

int GetTreeSpellCount(int tree) {
	int i, n = 0;
	for(i = 0; i < DND_SPELLS_PER_TREE; ++i)
		if(IsSpellDefined(GetTreeFirstSpell(tree) + i))
			++n;
	return n;
}

// The eight trees as rows. Only the two with content are selectable; the rest are drawn dim
// rather than hidden, so the shape of what is coming is visible.
void HandleSpellIndexDraw(int pnum, int boxid) {
	HudMessage(s:"--- ", l:"DND_MENU_HEAD_SPELLS", s:" ---"; HUDMSG_PLAIN, RPGMENUHELPID, CR_CYAN, 316.4, 44.0, 0.0, 0.0);
	HudMessage(s:"\c[Y5]", l:"DND_MENU_SPELLPOINTS", s:": \c-", d:GetSpellPoints(pnum);
		HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 192.1, 60.0, 0.0, 0.0);

	int i, y;
	for(i = 0; i < MAX_SKILL_TREES; ++i) {
		y = 80 + i * 16;
		DrawBoxText(StrParam(l:StrParam(s:"DND_SPELLTREE", d:i)), DND_NOLOOKUP, boxid, MBOX_1 + i,
			RPGMENUITEMID - 1 - i, 192.1, y << 16, "\c[B1]", GetTreeSpellCount(i) ? "\c-" : "\c[K5]");
	}

	HudMessage(s:"\c[Y5]-------------------------"; HUDMSG_PLAIN, RPGMENUITEMID - 3 - MAX_SKILL_TREES, CR_WHITE, 192.1, 80.0 + 16.0 * MAX_SKILL_TREES, 0.0, 0.0);

	// Last, so entry n stays box n however many trees get content.
	DrawBoxText("DND_SPELLTREE_HOTBAR", DND_LANGUAGE_LOOKUP, boxid, MBOX_1 + MAX_SKILL_TREES,
		RPGMENUITEMID - 1 - MAX_SKILL_TREES, 192.1, (88 + MAX_SKILL_TREES * 16) << 16, "\c[B1]", "\c-");
	DrawBoxText("DND_SPELLTREE_GEMS", DND_LANGUAGE_LOOKUP, boxid, MBOX_2 + MAX_SKILL_TREES,
		RPGMENUITEMID - 2 - MAX_SKILL_TREES, 192.1, (104 + MAX_SKILL_TREES * 16) << 16, "\c[B1]", "\c-");
}

// ---- hover panel -------------------------------------------------------------------------------
// Sits right of the tree in the same enlarged canvas. The text lumps say what a spell DOES; every
// NUMBER here is read live from SpellDefs and SpellSynergies, so the panel cannot drift from what a
// cast actually resolves.

// CRFTVW2's right pocket -- the same pocket the crafting page fills with materials, widened. The
// crafting constants are what these are derived from: CRAFTING_MATERIALBOX_X 92.0 is a FAR EDGE
// coordinate, so its boxes sit at screen x 344..420, and the pocket heading is drawn at 424.0.
//
// If the text sits wrong against the art, these four are the only knobs -- nothing else measures
// the pocket.
#define DND_SPELLPANEL_X 384
#define DND_SPELLPANEL_Y 56
// The pocket runs to the RIGHT EDGE OF THE SCREEN -- the art continues past 480 but nothing can be
// drawn or clicked there. 384..480 is all there is.
//
// SMALLFONT is about 8.7 units per character, so 21 of them wanted 183 units in a 96 unit pocket:
// the text was always overflowing, and clipping it only made that visible. NSMOLFNT is the smaller
// face and is what this is sized for.
#define DND_SPELLPANEL_W 148
#define DND_SPELLPANEL_H 222
// SMALLFONT, not NSMOLFNT: that font is declared in FONTDEFS but has no glyph lumps at all
// (nothing named DNSMR*), so SetFont on it silently fell back here and "using a smaller face"
// changed nothing for several rounds.
#define DND_SPELLPANEL_FONT "SMALLFONT"
#define DND_SPELLPANEL_LINEH 11	// SMALLFONT's step. 8 was sized for a font that does not exist

// Measured off the render rather than assumed: a line came out about 8 units per character, so 18
// of them overran the pocket by half again and the clip ate the tail of every line.
#define DND_SPELLPANEL_WRAP 16		// VISIBLE characters per line -- escapes do not count

// The pocket CLIPS, so this only has to stay inside the id band: DESC starts at 900 and HEAD is at
// 980, so forty is comfortably clear.
// One HUD message id per line, from SPELLTREE_DESC_ID. 40 silently DROPPED every line past it,
// which is what cut Annihilus and Molten Boulder short -- the panel scrolls, so this was never a
// pixel limit. The next id range starts at 980 and this one at 900, so 72 is the headroom there
// has always been, with 8 ids to spare.
#define DND_SPELLPANEL_MAXLINES 72
#define DND_SPELLBAR_PANEL 1		// shares the perk panel's bar; the pages are never up together
// The pocket's right column. This is past x 480, which the cursor only reaches because the page
// asks for DND_SPELLCURSOR_OVERREACH -- see the GetCursorPos call in the menu loop.
#define DND_SPELLBAR_X 528

// What the pocket is describing, kept across the cursor LEAVING the icon. Moving down to the bar is
// the same gesture as un-hovering the node, so a pocket that followed hover would empty exactly as
// the player reached for the scrollbar. Jump + click pins it; a plain click releases.
typedef struct {
	int panel_px;		// measured while drawing, for the bar's range
	int shown_spell;
	int shown_tree;		// so a page change drops it
	bool pinned;

	// Which hotbar slot the assignment page is filling, or -1 when it is just showing the slots.
	// The page's boxes ARE the picker grid while this is set, so the click handler reads it to know
	// what a box number means.
	int picking_slot;
	int picking_page;

	// The gem page: the socket being filled as spell * DND_SPELL_MAXSOCKETS + slot + 1, 0 when none,
	// and how far the last draw reached in each id band, so the next one deletes only what it left.
	int gem_pick;
	int gem_nodes_drawn, gem_lines_drawn;
} spell_scroll_T;

spell_scroll_T module& GetSpellScroll() {
	static spell_scroll_T s;
	return s;
}

// The panel cursor. Only ever live inside one HandleSpellHoverPanel call, and the draw is clientside
// so each client is filling its own.


// HudMessage does not word wrap, so long text is broken on spaces here. An empty string is a spacer.
// col is re-stated on EVERY wrapped line. A colour escape only lasts to the end of the string it is
// in, so a two line entry came out coloured on the first line and default on the second.
void PanelText(str text, str col = "") {
	int len = StrLen(text);
	if(!len) {
		GetSpellDraw().panel_y += DND_SPELLPANEL_LINEH / 2;
		return;
	}

	int start = 0, brk;
	while(start < len && GetSpellDraw().panel_id < SPELLTREE_DESC_ID + DND_SPELLPANEL_MAXLINES) {
		// Walk forward counting only what the player can SEE. A colour escape is 3 or 6 characters of
		// string and zero characters of line, and counting them broke "Cost: 5 mana" after the 5.
		int seen = 0, k = start;
		while(k < len && seen < DND_SPELLPANEL_WRAP) {
			if(GetChar(text, k) == 28) {		// \c
				++k;
				if(k < len && GetChar(text, k) == '[') {
					while(k < len && GetChar(text, k) != ']')
						++k;
				}
				++k;
				continue;
			}
			++k;
			++seen;
		}
		brk = k;

		if(brk < len) {
			// back off to the last space that fits, so words stay whole
			int b = brk;
			while(b > start && GetChar(text, b) != ' ')
				--b;
			if(b > start)
				brk = b;						// otherwise one unbroken word, hard break at brk
		}

		// CR_WHITE, not CR_UNTRANSLATED: untranslated leaves SMALLFONT's own red, which is what the
		// whole pocket came out as.
		SetFont(DND_SPELLPANEL_FONT);
		HudMessage(s:col, s:StrMid(text, start, brk - start);
			HUDMSG_PLAIN, GetSpellDraw().panel_id++, CR_WHITE,
			(DND_SPELLPANEL_X << 16) + 0.1, (GetSpellDraw().panel_y << 16) + 0.1, 0.0);
		GetSpellDraw().panel_y += DND_SPELLPANEL_LINEH;

		start = brk;
		while(start < len && GetChar(text, start) == ' ')
			++start;
	}
}

// Tics as seconds to one decimal. f: on a tic count reads as noise.
str TicsToSeconds(int tics) {
	return StrParam(d:tics / TICRATE, s:".", d:(tics * 10 / TICRATE) % 10);
}

// A 16.16 percent to one decimal, because 2.5% is a real value in the synergy table.
str FixedToTenths(int v) {
	return StrParam(d:v >> 16, s:".", d:((v * 10) >> 16) % 10);
}

// Two places, for values a tenth cannot describe -- 0.25 a rank reads as "0.2" through the above.
str FixedToHundredths(int v) {
	return StrParam(d:v >> 16, s:".", d:((v * 10) >> 16) % 10, d:((v * 100) >> 16) % 10);
}

// Built from the def rather than authored, so a requirement line cannot disagree with the rule that
// actually gates the spell. at_rank is the rank the player would buy next, so the level shown is
// the one standing in their way rather than rank 1's, which they already cleared.
str BuildSpellReqText(int spell, int at_rank = 1) {
	str res = "";
	int i, req, n = 0;
	bool any = SpellDefs[spell].flags & SPLF_REQ_ANY;

	for(i = 0; i < DND_MAX_SKILL_REQ; ++i) {
		req = SpellDefs[spell].req_spell[i];
		if(!req)
			continue;

		if(n)
			res = StrParam(s:res, s:any ? " or " : ", ");
		res = StrParam(s:res, l:GetSpellNameLump(req - 1), s:" (", d:SpellDefs[spell].req_rank[i], s:")");
		++n;
	}

	int lvl = GetSpellRankLevelReq(spell, at_rank);
	if(lvl > 1) {
		if(n)
			res = StrParam(s:res, s:", ");
		res = StrParam(s:res, l:"DND_SPLPANEL_LEVEL", s:" ", d:lvl);
		++n;
	}

	return n ? res : "";
}

void HandleSpellHoverPanel(int pnum, int spell) {
	int alloc = GetSpellAllocatedRank(pnum, spell);
	int rank = GetSpellRank(pnum, spell, true);

	// A locked spell has no rank to read values off, so it previews what rank 1 would give.
	int preview = alloc ? 0 : 1;
	int i, temp;
	str col;

	// Clipped to the pocket so long text stops at the art instead of running out of the bottom of
	// it, and offset by the bar so the rest can be scrolled to. Positions are negative, as everywhere
	// else that scrolls here.
	// FOUR arguments. The fifth is a WRAP WIDTH, and passing one made ZDoom re-wrap lines PanelText
	// had already wrapped -- each overflow then landed on top of the next message.
	SetHudClipRect(DND_SPELLPANEL_X - 4, DND_SPELLPANEL_Y - 4, DND_SPELLPANEL_W, DND_SPELLPANEL_H);

	int panel_top = DND_SPELLPANEL_Y + GetScrollBarPos(DND_SPELLBAR_PANEL) * DND_SPELLPANEL_LINEH;
	GetSpellDraw().panel_y = panel_top;
	GetSpellDraw().panel_id = SPELLTREE_DESC_ID;

	// The name and rank are the pocket's heading now, drawn outside this and not scrolled, so the
	// body opens straight into the description.
	PanelText(StrParam(s:"\c-", l:GetSpellDescLump(spell)));
	PanelText("");

	// Damage is read LIVE, like cost and cooldown beside it. It used to live only in the authored
	// DND_SPLPER prose, which cannot express a compounding curve -- "+60 damage per rank" stopped
	// being true the moment ranks started multiplying. Only fields the def marked as real damage
	// are shown, so a percent or a health figure never gets printed as "damage".
	// A ranged field prints as low-high; its low end is skipped as a line of its own.
	int lo_field;
	for(i = 0; i < SPELLVAL_MAX; ++i) {
		if(!(SpellDefs[spell].scale_mask & (1 << i)) || i == SPELLVAL_DAMAGEMIN || i == SPELLVAL_DAMAGE2MIN)
			continue;

		lo_field = GetSpellMinField(spell, i);

		temp = GetSpellValue(pnum, spell, i, preview);
		if(lo_field != -1)
			PanelText(StrParam(s:"\c[Y5]", l:GetSpellFieldLump(i), s:": \c-", d:GetSpellValue(pnum, spell, lo_field, preview), s:"-", d:temp));
		else
			PanelText(StrParam(s:"\c[Y5]", l:GetSpellFieldLump(i), s:": \c-", d:temp));

		// What the next point actually buys, since the step is no longer the flat per_rank value.
		if(alloc && alloc < DND_SPELL_RANKCAP) {
			int nxt = GetSpellValue(pnum, spell, i, alloc + 1);
			if(nxt > temp && lo_field != -1)
				PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_NEXTRANK", s:" \c-", d:GetSpellValue(pnum, spell, lo_field, alloc + 1), s:"-", d:nxt,
					s:" (+", d:nxt - temp, s:")"));
			else if(nxt > temp)
				PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_NEXTRANK", s:" \c-", d:nxt,
					s:" (+", d:nxt - temp, s:")"));
		}
	}

	temp = GetSpellValue(pnum, spell, SPELLVAL_COST, preview) >> 16;
	if(SpellDefs[spell].flags & SPLF_RESERVES)
		PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_RESERVE", s:" \c-", d:temp, s:"% mana"));
	else if(temp) {
		PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_COST", s:" \c-", d:temp, s:" mana"));
		if(alloc && alloc < DND_SPELL_RANKCAP) {
			int nc = GetSpellValue(pnum, spell, SPELLVAL_COST, alloc + 1) >> 16;
			if(nc > temp)
				PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_NEXTRANK", s:" \c-", d:nc,
					s:" mana (+", d:nc - temp, s:")"));
		}
	}

	// Whether the effect is actually running, for the spells that can be switched off. Directly under
	// the cost, so an aura's reservation and whether it is being paid read as one thought. Only once
	// learned -- there is nothing to switch on a spell the player does not have.
	if(IsSpellToggleable(spell) && alloc) {
		PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_STATE", s:" ",
			s:IsSpellToggledOff(pnum, spell) ? "\c[A0]" : "\cd",
			l:IsSpellToggledOff(pnum, spell) ? "DND_SPLPANEL_OFF" : "DND_SPLPANEL_ON"));
		PanelText(StrParam(s:"\c-", l:"DND_SPLPANEL_TOGGLEHINT"), "\c-");
	}

	// Shown AFTER everything that shortens them, so the panel matches the hotbar.
	temp = alloc ? GetSpellCooldownTics(pnum, spell) : (GetSpellValue(pnum, spell, SPELLVAL_COOLDOWN, 1) * TICRATE) >> 16;
	if(temp)
		PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_COOLDOWN", s:" \c-", s:TicsToSeconds(temp), s:"s"));

	temp = alloc ? GetSpellCastTics(pnum, spell) : (GetSpellValue(pnum, spell, SPELLVAL_CASTTIME, 1) * TICRATE) >> 16;
	PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_CASTTIME", s:" \c-",
		s:temp ? StrParam(s:TicsToSeconds(temp), s:"s") : StrParam(l:"DND_SPLPANEL_INSTANT")));

	PanelText("");
	// Empty for any spell whose per rank gain is now entirely in the live lines above. Skipped
	// rather than printed blank, so the pocket does not grow a stray heading with nothing under it.
	str per = StrParam(l:GetSpellPerRankLump(spell));
	if(StrLen(per))
		PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_PERRANK", s:" \c-", s:per));

	// Body is plain white whether or not the threshold is met -- the dim grey read as purple
	// against this backdrop. The label alone carries the colour.
	PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_RANK5", s:" \c-", l:GetSpellThresholdLump(spell, DND_SPELL_THRESH_LOW)), "\c-");

	PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_RANK10", s:" \c-", l:GetSpellThresholdLump(spell, DND_SPELL_THRESH_HIGH)), "\c-");

	// The rank they would buy next, so "Requires: Level N" is the gate in front of them. A maxed
	// spell has no next rank, so it keeps showing the last one it needed.
	int next = alloc < DND_SPELL_RANKCAP ? alloc + 1 : DND_SPELL_RANKCAP;
	str req = BuildSpellReqText(spell, next);
	if(StrLen(req)) {
		PanelText("");
		PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_REQUIRES", s:" \c-", s:req), "\c-");
	}

	bool first = true;
	for(i = 0; i < MAX_SPELL_SYNERGIES && SpellSynergies[i].target; ++i) {
		if(SpellSynergies[i].target - 1 != spell)
			continue;

		if(first) {
			PanelText("");
			first = false;
		}

		temp = SpellSynergies[i].source;
		if(SpellSynergies[i].flags & SYNF_FLAT)
			// Printed in the FIELD's unit. A flat bonus is a raw row value, and the rows are not all the
			// same kind -- a duration one is fixed point and showed its raw 16384.
			PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_SYNERGY", s:" - \c-", l:GetSpellNameLump(temp),
				s:": +", s:IsSpellFieldTenthsPercent(spell, SpellSynergies[i].field) ?
					StrParam(s:FixedToTenths((SpellSynergies[i].per_rank << 16) / 10), s:"%") :
					IsSpellFieldFixedPoint(SpellSynergies[i].field) ?
					FixedToHundredths(SpellSynergies[i].per_rank) :
					StrParam(d:SpellSynergies[i].per_rank),
				s:" ", l:GetSpellFieldLumpFor(spell, SpellSynergies[i].field), s:GetSynergyStepText(i)));
		else
			PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_SYNERGY", s:" - \c-", l:GetSpellNameLump(temp),
				s:": ", s:FixedToTenths(SpellSynergies[i].per_rank), s:"% more ",
				l:GetSpellFieldLump(SpellSynergies[i].field), s:GetSynergyStepText(i)));
	}

	// Measured rather than counted: blank spacers are half a line, so a line count overstated the
	// content and the bar stopped short of the last row.
	GetSpellScroll().panel_px = GetSpellDraw().panel_y - panel_top;

	// Whatever the previous hover left behind, when this one is shorter.
	DeleteTextRange(GetSpellDraw().panel_id, SPELLTREE_DESC_ID + DND_SPELLPANEL_MAXLINES);
	SetHudClipRect(0, 0, 0, 0, 0);
}

// The pocket's bar. Range comes from what the last draw actually emitted, so it can never disagree
// with the text it scrolls.
// The shared bar is used for its STATE only -- range, position, grab -- and its own drawing is
// left to paint harmlessly behind the backdrop. This is the copy that is actually seen, at low ids
// and at the pocket's right edge rather than the global DND_SCROLLBAR_X, which lands mid-pocket.
void DrawSpellPocketBar() {
	// The same routine the shared bar uses, into ids that sit in FRONT of this page's backdrop. The
	// hand-rolled version this replaces drew a track and no thumb, because it skipped the cap and
	// grip passes the real one needs.
	DrawScrollBarBody(DND_SPELLBAR_PANEL, DND_SPELLBAR_X,
		SPELLTREE_BAR_ID, SPELLTREE_BAR_ID + 1, SPELLTREE_BAR_ID + 2, SPELLTREE_BAR_ID + 3);
}

bool HandleSpellPageScroll(int tree, int boxid) {
	spell_scroll_T module& ss = GetSpellScroll();

	// One line of slack, or the final row sits half under the clip's bottom edge.
	int over = 0;
	if(ss.shown_spell != -1)
		over = ss.panel_px + DND_SPELLPANEL_LINEH - DND_SPELLPANEL_H;
	if(over < 0)
		over = 0;

	return ListenScroll(-(over / DND_SPELLPANEL_LINEH), 0, DND_SPELLPANEL_LINEH, DND_SPELLPANEL_H,
		DND_SPELLBAR_PANEL, DND_SPELLPANEL_Y, DND_SPELLPANEL_H, true, DND_SPELLBAR_X);
}

// Jump + click holds the pocket on the node under the cursor; any plain click releases it. Called
// straight after the input is produced -- see HandlePerkPin for why that placement is load bearing.
bool HandleSpellPin(int tree, int boxid, int input) {
	spell_scroll_T module& ss = GetSpellScroll();

	if(input == DND_MENUINPUT_JUMPCLICK) {
		if(boxid < MBOX_1)
			return false;

		int spell = GetTreeSpellByIndex(tree, boxid - MBOX_1);
		if(spell == -1)
			return false;

		// Rewind only for a DIFFERENT spell -- pinning the one already being read is the gesture.
		if(ss.shown_spell != spell)
			SetScrollBarPos(DND_SPELLBAR_PANEL, 0);

		ss.shown_spell = spell;
		ss.pinned = true;
		LocalAmbientSound("RPG/MenuChoose", 127);
		return true;
	}
	else if(input == DND_MENUINPUT_LCLICK && ss.pinned) {
		ss.pinned = false;
		return true;
	}

	return false;
}

// Heading, body and bar, off whatever the page last said it was describing. Both spell pages end
// with this, and the heading is drawn HERE rather than at the top of a page so it reads the hover
// the page has just resolved -- drawn earlier it was always one frame behind.
void DrawSpellPocket(int pnum) {
	spell_scroll_T module& ss = GetSpellScroll();

	if(ss.shown_spell == -1) {
		DeleteText(SPELLTREE_HEAD_ID + 2);
		DeleteText(SPELLTREE_HEAD_ID + 3);
		DeleteTextRange(SPELLTREE_DESC_ID, SPELLTREE_DESC_ID + DND_SPELLPANEL_MAXLINES);
		DrawSpellPocketBar();
		return;
	}

	int hrank = GetSpellRank(pnum, ss.shown_spell, true);
	int halloc = GetSpellAllocatedRank(pnum, ss.shown_spell);

	SetFont("SMALLFONT");
	HudMessage(s:"\c[Y5]", l:GetSpellNameLump(ss.shown_spell);
		HUDMSG_PLAIN, SPELLTREE_HEAD_ID + 2, CR_WHITE,
		(DND_SPELLPANEL_X << 16) + 0.1, 24.0, 0.0, 0.0);

	// Effective rank in brackets only where gear is actually adding to it.
	if(halloc && hrank != halloc)
		HudMessage(s:"\c[Y5]", l:"DND_SPLPANEL_LEVEL", s:": \c-", d:halloc, s:" (", d:hrank, s:")";
			HUDMSG_PLAIN, SPELLTREE_HEAD_ID + 3, CR_WHITE,
			(DND_SPELLPANEL_X << 16) + 0.1, 36.0, 0.0, 0.0);
	else
		HudMessage(s:"\c[Y5]", l:"DND_SPLPANEL_LEVEL", s:": \c-", d:halloc, s:"/", d:DND_SPELL_RANKCAP;
			HUDMSG_PLAIN, SPELLTREE_HEAD_ID + 3, CR_WHITE,
			(DND_SPELLPANEL_X << 16) + 0.1, 36.0, 0.0, 0.0);

	HandleSpellHoverPanel(pnum, ss.shown_spell);
	DrawSpellPocketBar();
}

// ---- hotbar assignment page --------------------------------------------------------------------
#define DND_SPELLHOTBAR_PITCH 36
#define DND_SPELLHOTBAR_Y 84
// The picker pages: 8 x 4 a page, so the slots and a full page stay inside the node ids (1050 + 6 + 32
// ends under SPELLTREE_CROSS_ID) and on the board, with the page arrows under the last row.
#define DND_SPELLPICK_COLS 8
#define DND_SPELLPICK_ROWS 4
#define DND_SPELLPICK_PERPAGE (DND_SPELLPICK_COLS * DND_SPELLPICK_ROWS)
#define DND_SPELLPICK_Y 118
#define DND_SPELLPICK_PAGEY 262
#define DND_SPELLPICK_ARROWX 44		// the page arrows sit this far either side of the board's middle

// Only what can actually sit on a bar: allocated, and not a passive or an aura. Those two work from
// allocation or a tree toggle and would be dead slots.
bool IsSpellBindable(int pnum, int spell) {
	return GetSpellAllocatedRank(pnum, spell) > 0 &&
		!(SpellDefs[spell].flags & (SPLF_PASSIVE | SPLF_AURA));
}

int GetBindableCount(int pnum) {
	int i, n = 0;
	for(i = 0; i < MAX_SPELL_IDS; ++i)
		if(IsSpellBindable(pnum, i))
			++n;
	return n;
}

// The nth bindable spell in id order -- the same walk the picker draws with, so an index means the
// same thing to the draw and to the click.
int GetBindableByIndex(int pnum, int index) {
	int i, n = 0;
	for(i = 0; i < MAX_SPELL_IDS; ++i) {
		if(!IsSpellBindable(pnum, i))
			continue;
		if(n == index)
			return i;
		++n;
	}
	return -1;
}

int GetPickerPageCount(int pnum) {
	return Max(1, (GetBindableCount(pnum) + DND_SPELLPICK_PERPAGE - 1) / DND_SPELLPICK_PERPAGE);
}

// A page arrow's box, in the pane's mirrored coordinates, around text centred at x, y.
void AddPickerArrowBox(menu_pane_T module& p, int x, int y) {
	AddBoxToPane_Points(p, (HUDMAX_X - x + 8) << 16, (HUDMAX_Y - y + 7) << 16, (HUDMAX_X - x - 8) << 16, (HUDMAX_Y - y - 7) << 16);
}

// x of the nth cell in a centred row of `count`, at DND_SPELLHOTBAR_PITCH.
int GetRowCellX(int index, int count) {
	int w = (count - 1) * DND_SPELLHOTBAR_PITCH + DND_SPELLNODE;
	return DND_SPELLTREE_LEFT + (DND_SPELLTREE_BOARDW - w) / 2 + index * DND_SPELLHOTBAR_PITCH;
}

// lit is the cursor; chosen is the slot already picked. Separate ids, because the picker's cells
// are drawn after the slots and a shared one let a hovered spell erase the chosen slot's frame.
void DrawSpellCell(int spell, int x, int y, int id, bool lit, bool chosen = false) {
	int sc = DND_SPELLTREE_ICONSCALE;

	SetHudSize(HUDMAX_X * sc, HUDMAX_Y * sc, 1);

	// SPLBACK is what an empty slot looks like; SPLSLCT is what HOVER looks like. One id either way:
	// a cell is an icon or an empty frame, never both.
	SetFont(spell >= 0 ? GetSpellIcon(spell, false) : "SPLBACK");
	HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_UNTRANSLATED,
		((x * sc) << 16) + 0.1, ((y * sc) << 16) + 0.1, 0.0);

	if(lit || chosen) {
		SetFont("SPLSLCT");
		HudMessage(s:"A"; HUDMSG_PLAIN, chosen ? SPELLTREE_PICKSEL_ID : SPELLTREE_HOVER_ID,
			CR_UNTRANSLATED, ((x * sc) << 16) + 0.1, ((y * sc) << 16) + 0.1, 0.0);
	}

	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

void HandleSpellHotbarDraw(int pnum, int boxid, menu_pane_T module& p) {
	spell_scroll_T module& ss = GetSpellScroll();
	int i, n, spell, x, y, hovered = -1;
	bool chosen = false;
	int slots = GetHotbarSlotCount(pnum);
	int bindable = GetBindableCount(pnum);
	bool framed = false;

	ResetPane(p);

	SetHudSize(DND_SPELLTREE_BACKW, DND_SPELLTREE_BACKH, 1);
	SetFont("CRFTVW2");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID, CR_CYAN, 176.0, 100.0, 0.0, 0.0);
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	SetFont("NMENUFNT");

	HudMessage(s:"--- ", l:"DND_SPELLTREE_HOTBAR", s:" ---";
		HUDMSG_PLAIN, SPELLTREE_HEAD_ID, CR_CYAN, 178.4, 22.0, 0.0, 0.0);
	HudMessage(s:"\c[Y5]", l:ss.picking_slot == -1 ? "DND_SPLPANEL_PICKSLOT" : "DND_SPLPANEL_PICKSPELL";
		HUDMSG_PLAIN, SPELLTREE_HEAD_ID + 1, CR_WHITE, 178.4, 34.0, 0.0, 0.0);

	// The back arrow is box ONE, added before anything else, because opening the picker changes how
	// many boxes follow it -- and the server, which cannot see the clientside picking_slot, would
	// otherwise have no fixed number to recognise it by. It used to be added last, so the picker's
	// first cell inherited its index and going back was what a grid click did.
	AddBoxToPane_Points(p, 466.0, 36.0, 454.0, 28.0);
	DrawBoxText("<=", DND_NOLOOKUP, boxid, MBOX_1, RPGMENUPAGEID - 1, 16.1, 288.0, "\c[B1]", "\c[Y5]");
	n = 1;

	// The slots. Read off the SYNCED inventory item rather than the global, because this draw is
	// clientside and the global copy here never sees what the server bound.
	for(i = 0; i < slots; ++i) {
		x = GetRowCellX(i, slots);
		y = DND_SPELLHOTBAR_Y;
		spell = CheckActorInventory(pnum + P_TIDSTART, GetHotbarSlotItem(i)) - 1;

		if(boxid == MBOX_1 + n)
			framed = true;

		if(ss.picking_slot == i)
			chosen = true;

		DrawSpellCell(spell, x, y, SPELLTREE_NODE_ID + n,
			boxid == MBOX_1 + n, ss.picking_slot == i);

		SetFont("SMALLFONT");
		HudMessage(s:"\c[Y5]", d:i + 1;
			HUDMSG_PLAIN, SPELLTREE_RANK_ID + n, CR_WHITE,
			((x + DND_SPELLNODE / 2) << 16) + 0.4,
			((y + DND_SPELLNODE + 2) << 16) + 0.1, 0.0);

		if(boxid == MBOX_1 + n && spell >= 0)
			hovered = spell;

		AddSpellBoxAt(p, x, y);
		++n;
	}

	// The picker, only while a slot is being filled. Its cells are boxes too, which is why the click
	// handler has to know whether it is open: a box number means a different thing either way.
	int pages = GetPickerPageCount(pnum);
	ss.picking_page = Clamp_Between(ss.picking_page, 0, pages - 1);
	// Where the icons end. The page arrows take box numbers after them but no node ids.
	int drawn = n;
	if(ss.picking_slot != -1) {
		int first = ss.picking_page * DND_SPELLPICK_PERPAGE;
		int shown = Min(bindable - first, DND_SPELLPICK_PERPAGE);
		for(i = 0; i < shown; ++i) {
			spell = GetBindableByIndex(pnum, first + i);

			// Each row is centred on its OWN count, so a trailing part row sits in the middle instead of
			// hanging off to the left under a full one.
			int row = i / DND_SPELLPICK_COLS;
			x = GetRowCellX(i % DND_SPELLPICK_COLS,
				Min(shown - row * DND_SPELLPICK_COLS, DND_SPELLPICK_COLS));
			y = DND_SPELLPICK_Y + row * DND_SPELLHOTBAR_PITCH;

			if(boxid == MBOX_1 + n)
				framed = true;

			DrawSpellCell(spell, x, y, SPELLTREE_NODE_ID + n, boxid == MBOX_1 + n);

			if(boxid == MBOX_1 + n)
				hovered = spell;

			AddSpellBoxAt(p, x, y);
			++n;
		}
		drawn = n;

		// Prev and next, wrapping round, as the two boxes after this page's cells.
		if(pages > 1) {
			int mid = DND_SPELLTREE_LEFT + DND_SPELLTREE_BOARDW / 2;
			SetFont("NMENUFNT");
			DrawBoxText("<", DND_NOLOOKUP, boxid, MBOX_1 + n, SPELLTREE_HEAD_ID + 4,
				((mid - DND_SPELLPICK_ARROWX) << 16) + 0.4, DND_SPELLPICK_PAGEY << 16, "\c[B1]", "\c[Y5]");
			AddPickerArrowBox(p, mid - DND_SPELLPICK_ARROWX, DND_SPELLPICK_PAGEY);
			++n;
			DrawBoxText(">", DND_NOLOOKUP, boxid, MBOX_1 + n, SPELLTREE_HEAD_ID + 5,
				((mid + DND_SPELLPICK_ARROWX) << 16) + 0.4, DND_SPELLPICK_PAGEY << 16, "\c[B1]", "\c[Y5]");
			AddPickerArrowBox(p, mid + DND_SPELLPICK_ARROWX, DND_SPELLPICK_PAGEY);
			++n;
			HudMessage(s:"\c[Y5]", d:ss.picking_page + 1, s:" / ", d:pages;
				HUDMSG_PLAIN, SPELLTREE_HEAD_ID + 6, CR_WHITE, (mid << 16) + 0.4, DND_SPELLPICK_PAGEY << 16, 0.0, 0.0);
		}
	}
	if(ss.picking_slot == -1 || pages == 1)
		DeleteTextRange(SPELLTREE_HEAD_ID + 4, SPELLTREE_HEAD_ID + 6);

	// Whatever the last, longer layout left behind: the rest of the node ids, which end where the
	// crossings begin. Never past them -- sweeping the whole id space wiped the tree's connectors.
	DeleteTextRange(SPELLTREE_NODE_ID + drawn, SPELLTREE_CROSS_ID - 1);
	// + 1: the labels start at RANK_ID + 1, because box one is the back arrow. Sweeping from
	// RANK_ID + slots deleted the LAST slot's number every frame.
	DeleteTextRange(SPELLTREE_RANK_ID + slots + 1, SPELLTREE_RANK_ID + DND_SPELLS_PER_TREE - 1);

	// Not `hovered`: an EMPTY slot under the cursor is framed but has no spell, and keying the frame
	// off hovered deleted it the moment it was drawn.
	if(!framed)
		DeleteText(SPELLTREE_HOVER_ID);

	// Survives the cursor moving off the slot and onto a spell -- that is the whole point of it.
	if(!chosen)
		DeleteText(SPELLTREE_PICKSEL_ID);

	// Published for the server: the bind script reads the spell from here, exactly as it does for the
	// hotbar keys, so assigning from this page reuses that path instead of inventing a second one.
	SetUserCVar(pnum, "dnd_hoveredspell", hovered);

	if(!ss.pinned && ss.shown_spell != hovered) {
		ss.shown_spell = hovered;
		SetScrollBarPos(DND_SPELLBAR_PANEL, 0);
	}

	DrawSpellPocket(pnum);
}

// CLIENTSIDE, called from the menu input loop beside HandlePerkPin -- NOT from the server's box
// receive. picking_slot decides what a box number means to the DRAW, and the draw is clientside, so
// a server side write never reaches it. That is why the first version did nothing at all.
//
// The BIND itself is authoritative, so that half is puked to "DnD Bind Hotbar", which reads the
// spell out of dnd_hoveredspell exactly as the hotbar keys do.
//
// Returns whether the page owes a redraw.
bool HandleSpellHotbarPick(int pnum, int boxid, int input) {
	if(input != DND_MENUINPUT_LCLICK && input != DND_MENUINPUT_JUMPCLICK)
		return false;

	spell_scroll_T module& ss = GetSpellScroll();
	int slots = GetHotbarSlotCount(pnum);

	// Outside every box: closes the picker, nothing else.
	if(boxid < MBOX_1) {
		if(ss.picking_slot == -1)
			return false;
		ss.picking_slot = -1;
		ss.picking_page = 0;
		return true;
	}

	// Box one is the back arrow, which the server handles.
	int idx = boxid - MBOX_1;
	if(!idx)
		return false;
	--idx;

	// A slot: start filling it, or move an open assignment to it.
	if(idx < slots) {
		if(ss.picking_slot == -1)
			ss.picking_page = 0;
		ss.picking_slot = idx;
		LocalAmbientSound("RPG/MenuChoose", 127);
		return true;
	}

	if(ss.picking_slot == -1)
		return false;

	// The page's cells, then its prev and next arrows when it has more than one page.
	idx -= slots;
	int pages = GetPickerPageCount(pnum);
	int shown = Min(GetBindableCount(pnum) - ss.picking_page * DND_SPELLPICK_PERPAGE, DND_SPELLPICK_PERPAGE);
	if(idx >= shown) {
		if(pages > 1 && idx - shown < 2) {
			ss.picking_page = (ss.picking_page + (idx - shown ? 1 : pages - 1)) % pages;
			LocalAmbientSound("RPG/MenuChoose", 127);
		}
		return true;
	}

	// No sound here: "DnD Bind Hotbar" makes one when the bind lands, and two fired together read as
	// one loud click rather than two.
	NamedRequestScriptPuke("DnD Bind Hotbar", ss.picking_slot);

	ss.picking_slot = -1;
	ss.picking_page = 0;
	return true;
}

void HandleSpellTreeDraw(int pnum, int tree, int boxid, menu_pane_T module& p) {
	int i, j, req, spell, rank, alloc, n = 0, line = SPELLTREE_LINE_ID, hovered = -1;
	bool unlocked;
	str col;

	ResetPane(p);

	spell_scroll_T module& ss = GetSpellScroll();

	// A page change drops what the pocket was describing and rewinds it: the spell belongs to the
	// tree just left, and the offset is measured against text no longer on screen.
	if(ss.shown_tree != tree) {
		ss.shown_tree = tree;
		ss.shown_spell = -1;
		ss.panel_px = 0;
		ss.pinned = false;
		ss.picking_slot = -1;
		ss.gem_pick = 0;
		SetScrollBarPos(DND_SPELLBAR_PANEL, 0);
	}

	// CRFTVW2 -- the crafting board with a wider right pocket. Drawn with the crafting page's own
	// call: same size, same position, same immediate restore.
	SetHudSize(DND_SPELLTREE_BACKW, DND_SPELLTREE_BACKH, 1);
	SetFont("CRFTVW2");
	// RPGMENUID, the crafting page's slot, and it has to be a LOW id: this backdrop is what covers
	// DND_STAB, the side bar and the game behind them. Putting it at RPGMENUBACKGROUNDID instead --
	// to get it behind the shared scrollbar -- replaced the full screen cover and let the side bar's
	// leftover text sit on top of the tree.
	//
	// Which means the SHARED bar (2151..2158) can never show through it, so the pocket draws its own
	// just below.
	// Exactly the crafting view's coordinate. Nudging it to compensate for CRFTVW2's own padding
	// only made the two views disagree in the other direction; if the art needs to move, it moves in
	// the art.
	//
	// Whole numbers only. The fraction of a HudMessage coordinate selects ALIGNMENT here -- .1 left,
	// .4 centre -- so 103.5 did not mean "half a unit lower", it picked another mode and threw the
	// backdrop across the screen.
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID, CR_CYAN, 176.0, 100.0, 0.0, 0.0);
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	SetFont("NMENUFNT");

	// The board's heading over the middle of the tree, and the points count under it.
	HudMessage(s:"--- ", l:StrParam(s:"DND_SPELLTREETITLE", d:tree), s:" ---";
		HUDMSG_PLAIN, SPELLTREE_HEAD_ID, CR_CYAN, 178.4, 22.0, 0.0, 0.0);
	HudMessage(s:"\c[Y5]", l:"DND_MENU_SPELLPOINTS", s:": \c-", d:GetSpellPoints(pnum);
		HUDMSG_PLAIN, SPELLTREE_HEAD_ID + 1, CR_WHITE, 178.4, 34.0, 0.0, 0.0);

	ResetTreeSegments();

	// Edges first so a node's icon always sits on top of anything reaching it.
	//
	// This walks the TREE'S BLOCK, not the whole id space, and skips its empty slots. Walking all of
	// MAX_SPELL_IDS and testing .tree drew every unused id as a Fire spell at (0,0) -- 234 invisible
	// nodes, each claiming a box, which overran the pane's box limit and took the real tree with it.
	int first = GetTreeFirstSpell(tree);
	for(j = 0; j < DND_SPELLS_PER_TREE; ++j) {
		i = first + j;
		if(!IsSpellDefined(i))
			continue;

		for(int k = 0; k < DND_MAX_SKILL_REQ; ++k) {
			req = SpellDefs[i].req_spell[k];
			if(!req || !IsSpellDefined(req - 1) || SpellDefs[req - 1].tree != tree)
				continue;

			// Lit when the requirement is satisfied, so the tree reads as a set of open paths.
			col = GetSpellAllocatedRank(pnum, req - 1) >= SpellDefs[i].req_rank[k] ? "\c[Y5]" : "\c[K5]";
			DrawTreeConnector(req - 1, i, line, col);
			line += DND_SPELLLINE_IDS;
		}
	}

	// Once every edge is down, and only then: a joint is a fact about the SET of them.
	DrawTreeJunctions("\c[K5]");

	for(j = 0; j < DND_SPELLS_PER_TREE; ++j) {
		i = first + j;
		if(!IsSpellDefined(i))
			continue;

		spell = i;
		alloc = GetSpellAllocatedRank(pnum, spell);
		unlocked = alloc > 0;

		// The only thing on the page drawn outside the default space. Coordinates scale with it, and
		// the hit box in AddSpellNodeBox stays in menu units -- that is the space boxes always use.
		SetHudSize(HUDMAX_X * DND_SPELLTREE_ICONSCALE, HUDMAX_Y * DND_SPELLTREE_ICONSCALE, 1);
		SetFont(GetSpellIcon(spell, !unlocked));
		HudMessage(s:"A"; HUDMSG_PLAIN, SPELLTREE_NODE_ID + n, CR_UNTRANSLATED,
			((GetSpellNodeX(spell) * DND_SPELLTREE_ICONSCALE) << 16) + 0.1,
			((GetSpellNodeY(spell) * DND_SPELLTREE_ICONSCALE) << 16) + 0.1, 0.0);

		// The selection frame is 64x64 like the icons, so it rides the same canvas and the same
		// coordinates -- one id, because only one node is ever hovered.
		if(boxid == MBOX_1 + n) {
			SetFont("SPLSLCT");
			HudMessage(s:"A"; HUDMSG_PLAIN, SPELLTREE_HOVER_ID, CR_UNTRANSLATED,
				((GetSpellNodeX(spell) * DND_SPELLTREE_ICONSCALE) << 16) + 0.1,
				((GetSpellNodeY(spell) * DND_SPELLTREE_ICONSCALE) << 16) + 0.1, 0.0);
		}

		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);

		// Only ALLOCATED spells carry a number on the board. At this icon size there is no room for a
		// line under every node -- forty of them ran together into one smear -- and an unspent tree
		// saying 0/10 forty times says nothing. The pocket carries the rank for whatever is hovered.
		rank = GetSpellRank(pnum, spell, true);
		SetFont("SMALLFONT");
		if(!unlocked)
			DeleteText(SPELLTREE_RANK_ID + n);
		// RIGHT aligned (.2) against the icon's right edge, so a two digit rank grows away from the
		// art instead of into it -- at 10 the left aligned version ran across the icon.
		else if(rank != alloc)
			HudMessage(s:"\c[Y5]", d:alloc, s:"(", d:rank, s:")";
				HUDMSG_PLAIN, SPELLTREE_RANK_ID + n, CR_WHITE,
				((GetSpellNodeX(spell) + DND_SPELLNODE) << 16) + 0.2,
				((GetSpellNodeY(spell) + 1) << 16) + 0.1, 0.0);
		else
			HudMessage(s:"\c[Y5]", d:alloc;
				HUDMSG_PLAIN, SPELLTREE_RANK_ID + n, CR_WHITE,
				((GetSpellNodeX(spell) + DND_SPELLNODE) << 16) + 0.2,
				((GetSpellNodeY(spell) + 1) << 16) + 0.1, 0.0);

		if(boxid == MBOX_1 + n)
			hovered = spell;

		AddSpellNodeBox(p, spell);
		++n;
	}

	// Published for the hotbar bind keys, which arrive on the server with no idea what the
	// cursor is over.
	SetUserCVar(pnum, "dnd_hoveredspell", hovered);

	if(hovered == -1)
		DeleteText(SPELLTREE_HOVER_ID);

	// Follows the cursor and CLEARS with it. Only a pin holds it, which is the whole point of the
	// pin: without one, walking the cursor off a node used to leave its text standing forever.
	//
	// A DIFFERENT spell starts at the top of its own text -- carrying the last one's offset over
	// opens a short entry half way down itself.
	if(!ss.pinned && ss.shown_spell != hovered) {
		ss.shown_spell = hovered;
		SetScrollBarPos(DND_SPELLBAR_PANEL, 0);
	}

	DrawSpellPocket(pnum);

	SetFont("NMENUFNT");

	// Added last on purpose: entry n stays box n however many spells the tree grows.
	// Exactly the crafting view's arrow: same box, same position, same art. Added AFTER every node
	// on purpose, so entry n stays box n however the tree grows.
	// Literals rather than CRAFTING_PAGEARROW*: those live in DnD_MenuTables.h, which is included
	// AFTER this file, and macros do not forward reference. Values are 466/36 with a 12x8 size.
	AddBoxToPane_Points(p, 466.0, 36.0, 454.0, 28.0);
	DrawBoxText("<=", DND_NOLOOKUP, boxid, MBOX_1 + n, RPGMENUPAGEID - 1, 16.1, 288.0, "\c[B1]", "\c[Y5]");
}

// Returns true when something changed and the page wants a redraw.
// LEFT click only. The toggle used to live here and fired on any click, which meant an aura could
// never be ranked up past the first point -- every attempt flipped it instead.
bool HandleSpellTreeClick(int pnum, int tree, int boxid) {
	if(boxid < MBOX_1)
		return false;

	int spell = GetTreeSpellByIndex(tree, boxid - MBOX_1);
	if(spell == -1)
		return false;

	return AllocateSpellPoint(pnum, spell);
}

// RIGHT click, on an aura or a passive the player has already learned: switches its effect off, or
// back on. Auras stop reserving their mana while off, which is the point -- a build may want the
// spell ranked for its synergies without paying the reservation.
bool HandleSpellTreeToggle(int pnum, int tree, int boxid) {
	if(boxid < MBOX_1)
		return false;

	int spell = GetTreeSpellByIndex(tree, boxid - MBOX_1);
	if(spell == -1 || !IsSpellToggleable(spell) || !GetSpellAllocatedRank(pnum, spell))
		return false;

	SetSpellToggledOff(pnum, spell, !IsSpellToggledOff(pnum, spell));

	// Before the sync: this may switch a DIFFERENT aura off when the reservations no longer fit, and
	// that one pushes its own word.
	ValidateAuraReservations(pnum);
	SyncSpellToggleWord(pnum, spell);
	return true;
}

// ---- gem sockets: the mechanics --------------------------------------------------------------
bool SpellHasRow(int spell, int field) {
	return SpellDefs[spell].base[field] > 0 || SpellDefs[spell].per_rank[field] > 0;
}

// Whether a term can do anything on this spell. A term that cannot is never added, so it cannot leak into
// a row a spell borrows for something else: Annihilus's DURATION is a channel cap, not an effect.
bool IsGemStatActiveOnSpell(int stat, int spell) {
	int f = SpellDefs[spell].flags;
	switch(stat) {
		case GEMSTAT_MORE_DURATION:
		return !!(f & SPLF_DURATION);
		case GEMSTAT_MORE_COST:
		return !(f & (SPLF_NOCOST | SPLF_RESERVES | SPLF_PASSIVE)) && SpellHasRow(spell, SPELLVAL_COST);
		case GEMSTAT_MORE_HASTE:
		case GEMSTAT_HASTE:
		return SpellHasRow(spell, SPELLVAL_CASTTIME);
		case GEMSTAT_CDR:
		return SpellHasRow(spell, SPELLVAL_COOLDOWN);
		case GEMSTAT_RESERVE_EFF:
		return !!(f & SPLF_RESERVES);
		case GEMSTAT_MORE_AREA:
		return !!(f & SPLF_AOE);
		case GEMSTAT_EXTRAPROJ:
		return !!(f & SPLF_PROJECTILE);
	}
	// damage, crit and ailments: anything that is not purely a support spell
	return !(f & SPLF_SUPPORT);
}

// A colour does something on this spell when any of its terms can.
bool IsGemColourActive(int colour, int spell) {
	for(int k = 0; k < GetGemColourTermCount(colour); ++k)
		if(IsGemStatActiveOnSpell(GetGemColourTermStat(colour, k), spell))
			return true;
	return false;
}

// Whether a role can do anything on this spell. An inactive role adds nothing, its cost penalty included.
bool IsGemRoleActiveOnSpell(int role, int spell) {
	int f = SpellDefs[spell].flags;
	switch(role) {
		case DND_GEMROLE_CONQUEROR:
		return !!(f & SPLF_AOE);
		case DND_GEMROLE_CONJURER:
		return !!(f & (SPLF_PROJECTILE | SPLF_CHAIN));
		case DND_GEMROLE_LEADER:
		return !!(f & SPLF_AURA);
		case DND_GEMROLE_CONSERVATIONIST:
		return !!(f & SPLF_RESERVES);
		case DND_GEMROLE_INFLUENCER:
		return !!(f & SPLF_PASSIVE);
		case DND_GEMROLE_SUMMONER:
		case DND_GEMROLE_COMMANDER:
		case DND_GEMROLE_NECROMANCER:
		return !!(f & SPLF_SUMMON);
		case DND_GEMROLE_PRIEST:
		return !!(f & SPLF_HEAL);
		case DND_GEMROLE_WITCH:
		return !!(f & SPLF_CURSE);
		case DND_GEMROLE_SWIFTCASTER:
		return SpellHasRow(spell, SPELLVAL_CASTTIME);
		case DND_GEMROLE_QUICKTONGUE:
		return SpellHasRow(spell, SPELLVAL_CASTTIME) && !(f & SPLF_TOGGLE);
		case DND_GEMROLE_MAGUS:
		case DND_GEMROLE_HAEMOCRAFTER:
		return IsGemStatActiveOnSpell(GEMSTAT_MORE_COST, spell);
		case DND_GEMROLE_BLADEDANCER:
		case DND_GEMROLE_FIGHTER:
		return !(f & (SPLF_PASSIVE | SPLF_AURA));
	}
	// The hit roles: damage, crit, ailments, leech, penetration, culling, charges.
	return !(f & SPLF_SUPPORT);
}

// Whether a gem does anything on this spell: its colour does, or its role does.
bool IsGemUsefulOnSpell(int subt, int role, int spell) {
	return IsGemColourActive(GetGemColour(subt), spell) || (role != DND_GEMROLE_NONE && IsGemRoleActiveOnSpell(role, spell));
}

// A role's mana cost penalty, as more mana cost; a spell without a cost has nothing to raise.
void AddGemCostPenalty(int pnum, int spell, int more) {
	if(IsGemStatActiveOnSpell(GEMSTAT_MORE_COST, spell))
		AddSpellGemMod(pnum, spell, GEMSTAT_MORE_COST, more);
}

// A role's terms, from the design's numbers; g picks the greater ones. MORE values are 16.16.
void AddGemRoleToSpellRow(int pnum, int spell, int role, bool g) {
	switch(role) {
		case DND_GEMROLE_MARKSMAN:
			AddSpellGemMod(pnum, spell, GEMSTAT_CRITMULT, g ? 80 : 40);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_WIZARD:
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_HITDAMAGE, g ? 0.4 : 0.25);
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_DOTDAMAGE, g ? -0.6 : -0.4);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_CONQUEROR:
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_AREA, g ? 0.36 : 0.2);
			AddGemCostPenalty(pnum, spell, g ? 0.3 : 0.2);
		break;
		case DND_GEMROLE_BLADEDANCER:
			AddSpellGemMod(pnum, spell, GEMSTAT_FREEPARRY, g ? 4 * TICRATE : 5 * TICRATE / 2);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_FIGHTER:
			AddSpellGemMod(pnum, spell, GEMSTAT_STAMINAREGEN, g ? 50 : 25);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_CONJURER:
			AddSpellGemMod(pnum, spell, GEMSTAT_EXTRAPROJ, g ? 2 : 1);
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_DAMAGE, g ? -0.25 : -0.125);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_LEADER:
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_AREA, g ? 0.5 : 0.3);
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_RESERVE, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_CONSERVATIONIST:
			AddSpellGemMod(pnum, spell, GEMSTAT_RESERVE_EFF, g ? 25 : 15);
		break;
		case DND_GEMROLE_MAGUS:
			AddSpellGemMod(pnum, spell, GEMSTAT_MAGUS, g ? 40 : 25);
			AddGemCostPenalty(pnum, spell, g ? 0.2 : 0.1);
		break;
		case DND_GEMROLE_SWIFTCASTER:
			AddSpellGemMod(pnum, spell, GEMSTAT_HASTE, g ? 40 : 20);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_SUMMONER:
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_MINIONLIFE, g ? 0.4 : 0.25);
			AddGemCostPenalty(pnum, spell, g ? 0.27 : 0.18);
		break;
		case DND_GEMROLE_PRIEST:
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_HEALING, g ? 0.4 : 0.2);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_WITCH:
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_CURSE, g ? 0.4 : 0.2);
			AddGemCostPenalty(pnum, spell, g ? 0.4 : 0.2);
		break;
		case DND_GEMROLE_CRUEL:
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_HITDAMAGE, g ? -0.6 : -0.4);
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_DOTDAMAGE, g ? 0.45 : 0.3);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_PYROMANCER:
			AddSpellGemMod(pnum, spell, GEMSTAT_IGNITECHANCE, g ? 50 : 25);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_CRYOMANCER:
			AddSpellGemMod(pnum, spell, GEMSTAT_CHILLCHANCE, g ? 50 : 25);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_VENOMANCER:
			AddSpellGemMod(pnum, spell, GEMSTAT_POISONCHANCE, g ? 50 : 25);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_ELECTROMANCER:
			AddSpellGemMod(pnum, spell, GEMSTAT_OVERLOADCHANCE, g ? 50 : 25);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_VAMPIRE:
			AddSpellGemMod(pnum, spell, GEMSTAT_LEECH, g ? 30 : 15);
			AddGemCostPenalty(pnum, spell, g ? 0.35 : 0.2);
		break;
		case DND_GEMROLE_INFLUENCER:
			AddSpellGemMod(pnum, spell, GEMSTAT_PASSIVE_EFFECT, g ? 20 : 10);
			AddSpellGemMod(pnum, spell, GEMSTAT_RESERVE_ADD, g ? 10 : 5);
		break;
		case DND_GEMROLE_COMMANDER:
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_MINIONDAMAGE, g ? 0.4 : 0.25);
			AddGemCostPenalty(pnum, spell, g ? 0.27 : 0.18);
		break;
		case DND_GEMROLE_QUICKTONGUE:
			AddSpellGemMod(pnum, spell, GEMSTAT_REPEAT, 1);
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_HASTE, g ? 0.3 : 0.2);
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_DAMAGE, g ? -0.25 : -0.35);
			AddGemCostPenalty(pnum, spell, g ? 0.35 : 0.2);
		break;
		case DND_GEMROLE_ARCHMAGE:
			AddSpellGemMod(pnum, spell, GEMSTAT_ARCHMAGE, g ? 12 : 8);
			AddGemCostPenalty(pnum, spell, g ? 0.5 : 0.3);
		break;
		case DND_GEMROLE_ASSASSIN:
			AddSpellGemMod(pnum, spell, GEMSTAT_ELUSIVE, g ? 20 : 10);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_NECROMANCER:
			AddSpellGemMod(pnum, spell, GEMSTAT_NECRO, g ? 50 : 25);
			AddGemCostPenalty(pnum, spell, g ? 0.5 : 0.3);
		break;
		case DND_GEMROLE_SANGUINOMANCER:
			AddSpellGemMod(pnum, spell, GEMSTAT_BLEEDCHANCE, g ? 50 : 25);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_DESTROYER:
			AddSpellGemMod(pnum, spell, GEMSTAT_PENETRATION, g ? 30 : 20);
			AddGemCostPenalty(pnum, spell, g ? 0.35 : 0.2);
		break;
		// The greater three keep the normal charge chance and add their damage per charge on top.
		case DND_GEMROLE_RESOLUTE:
			AddSpellGemMod(pnum, spell, GEMSTAT_ENDURANCECHANCE, 15);
			if(g)
				AddSpellGemMod(pnum, spell, GEMSTAT_ENDURANCEDAMAGE, 4);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_RAVAGER:
			AddSpellGemMod(pnum, spell, GEMSTAT_FRENZYCHANCE, 15);
			if(g)
				AddSpellGemMod(pnum, spell, GEMSTAT_FRENZYDAMAGE, 4);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_POWERFUL:
			AddSpellGemMod(pnum, spell, GEMSTAT_POWERCHANCE, 15);
			if(g)
				AddSpellGemMod(pnum, spell, GEMSTAT_POWERDAMAGE, 4);
			AddGemCostPenalty(pnum, spell, g ? 0.25 : 0.15);
		break;
		case DND_GEMROLE_MERCILESS:
			AddSpellGemMod(pnum, spell, GEMSTAT_MORE_LOWLIFE, g ? 0.54 : 0.33);
			AddGemCostPenalty(pnum, spell, g ? 0.35 : 0.2);
		break;
		case DND_GEMROLE_EXECUTIONER:
			AddSpellGemMod(pnum, spell, GEMSTAT_CANCULL, 1);
			if(g)
				AddSpellGemMod(pnum, spell, GEMSTAT_CULLRANGE, 5);
			AddGemCostPenalty(pnum, spell, g ? 0.35 : 0.2);
		break;
		// The cheapest multiplier socketed wins; two of them do not add up to 350% of the cost.
		case DND_GEMROLE_HAEMOCRAFTER:
			if(!GetSpellGemMod(pnum, spell, GEMSTAT_LIFECOST) || GetSpellGemMod(pnum, spell, GEMSTAT_LIFECOST) > (g ? 150 : 200))
				SetSpellGemMod(pnum, spell, GEMSTAT_LIFECOST, g ? 150 : 200);
		break;
	}
}

// A socketed gem's terms into the spell's row: its colour's, then its role's when the role can act on the spell.
void AddGemToSpellRow(int pnum, int spell, int subt, int role) {
	int c = GetGemColour(subt), st;
	for(int k = 0; k < GetGemColourTermCount(c); ++k) {
		st = GetGemColourTermStat(c, k);
		if(IsGemStatActiveOnSpell(st, spell))
			AddSpellGemMod(pnum, spell, st, GemStatFromPercent(st, GetGemColourTermValue(c, k, IsGreaterGem(subt))));
	}

	if(role != DND_GEMROLE_NONE && IsGemRoleActiveOnSpell(role, spell))
		AddGemRoleToSpellRow(pnum, spell, role, IsGreaterGem(subt));
}

// A socket's gem back into the inventory, at box or the first free spot. False when it has nowhere to go.
bool ReturnGemToInventory(int pnum, int v, int box = -1) {
	int c = CreateItemSpot();
	if(c == -1)
		return false;

	// A role past the last one is "no role" to RollGemInfo, where -1 would roll one.
	int role = GetSocketGemRole(v);
	RollGemInfo(c, GetGemColour(GetSocketGemSubtype(v)), IsGreaterGem(GetSocketGemSubtype(v)), role == DND_GEMROLE_NONE ? DND_GEM_ROLES : role);

	if(box == -1)
		box = GetFreeSpotForItem(c, pnum, DND_SYNC_ITEMSOURCE_FIELD, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	if(box == -1) {
		RemoveItemFromWorld(c);
		return false;
	}

	CopyItemFromFieldToPlayer(c, pnum, box);
	return true;
}

// The gem at inventory box into a socket. A gem already there swaps into that box, so a full inventory never blocks it.
Script "DnD Gem Socket" (int spell, int slot, int box) NET {
	int pnum = PlayerNumber();
	if(pnum < 0 || spell < 0 || spell >= MAX_SPELL_IDS || slot < 0 || slot >= GetSpellSocketCount(pnum, spell) || box < 0 || box >= MAX_INVENTORY_BOXES)
		Terminate;

	auto item = GetPlayerInventoryItem(pnum, box);
	if(item.item_type != DND_ITEM_GEM || item.topleftboxid != box + 1 || !IsGemUsefulOnSpell(item.item_subtype, item.implicit[0].attrib_id, spell))
		Terminate;

	int v = MakeSocketValue(item.item_subtype, item.implicit[0].attrib_id);
	int old = GetSpellSocket(pnum, spell, slot);

	// A swap frees the box unsynced: the refill's own sync overwrites it, and a clear sent beside a fill can land last.
	FreeItem(pnum, box, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, !!old);
	if(old)
		ReturnGemToInventory(pnum, old, box);

	SetSpellSocket(pnum, spell, slot, v);
	RebuildSpellGems(pnum, spell);
	ValidateAuraReservations(pnum);
	UpdateManaVisualCap(pnum);
	LocalAmbientSound("Items/GemPick", 127);
	SendOwnerScript("DnD Refresh Request", pnum, pnum, 1);
}

// A socket's gem back to the inventory; refused, with the popup, when there is no room.
Script "DnD Gem Unsocket" (int spell, int slot) NET {
	int pnum = PlayerNumber();
	if(pnum < 0 || spell < 0 || spell >= MAX_SPELL_IDS || slot < 0 || slot >= DND_SPELL_MAXSOCKETS)
		Terminate;

	int v = GetSpellSocket(pnum, spell, slot);
	if(!v)
		Terminate;

	if(!ReturnGemToInventory(pnum, v)) {
		ShowPopup(POPUP_NOTENOUGHSPACE, false, 0);
		Terminate;
	}

	SetSpellSocket(pnum, spell, slot, 0);
	RebuildSpellGems(pnum, spell);
	ValidateAuraReservations(pnum);
	UpdateManaVisualCap(pnum);
	LocalAmbientSound("Items/GemDrop", 127);
	SendOwnerScript("DnD Refresh Request", pnum, pnum, 1);
}

// ---- the socket page's lists, clientside off the synced copies ----------------------------------
#define DND_GEMPAGE_MAXSHOWN 40		// 8 x 5, the same grid the hotbar picker uses

// Spells with open sockets, in id order: what the page lists and what its boxes index.
int GetGemEligibleSpell(int pnum, int index) {
	int n = 0;
	for(int i = 0; i < MAX_SPELL_IDS; ++i) {
		if(!GetSpellSocketCount(pnum, i))
			continue;
		if(n == index)
			return i;
		++n;
	}
	return -1;
}

int GetGemEligibleCount(int pnum) {
	int n = 0;
	for(int i = 0; i < MAX_SPELL_IDS; ++i)
		n += !!GetSpellSocketCount(pnum, i);
	return Min(n, DND_GEMPAGE_MAXSHOWN);
}

// The index-th gem in the inventory, as its top left box, or -1; with a spell, only gems that do something on it.
int GetInventoryGemBox(int pnum, int index, int spell = -1) {
	int n = 0;
	for(int b = 0; b < MAX_INVENTORY_BOXES; ++b) {
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, b, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) != DND_ITEM_GEM)
			continue;
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, b, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) != b + 1)
			continue;
		if(spell != -1 && !IsGemUsefulOnSpell(GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, b, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY),
			GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, b, 0, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY), spell))
			continue;
		if(n == index)
			return b;
		++n;
	}
	return -1;
}

int GetInventoryGemCount(int pnum, int spell = -1) {
	int n = 0;
	while(n < DND_GEMPAGE_MAXSHOWN && GetInventoryGemBox(pnum, n, spell) != -1)
		++n;
	return n;
}

// ---- windows a cast opens: Bladedancer's free parry, Fighter's faster stamina recovery ------------
#define DND_GEM_FIGHTER_TICS (4 * TICRATE)
#define DND_GEM_WINDOW_MAX (10 * TICRATE)	// no window runs longer; anything further off is left over from another map's Timer()

typedef struct {
	int parry_end[MAXPLAYERS];
	int regen_end[MAXPLAYERS];
	int regen_pct[MAXPLAYERS];
} gem_windows_T;

gem_windows_T module& GetGemWindows() {
	static gem_windows_T s;
	return s;
}

bool IsGemWindowOpen(int end) {
	int left = end - Timer();
	return left >= 0 && left <= DND_GEM_WINDOW_MAX;
}

void OpenGemCastWindows(int pnum, int spell) {
	auto w = GetGemWindows();
	int v;
	if((v = GetSpellGemMod(pnum, spell, GEMSTAT_FREEPARRY)))
		w.parry_end[pnum] = Timer() + v;
	if((v = GetSpellGemMod(pnum, spell, GEMSTAT_STAMINAREGEN))) {
		w.regen_end[pnum] = Timer() + DND_GEM_FIGHTER_TICS;
		w.regen_pct[pnum] = v;
	}
}

// Necromancer: a slain spell minion's chance to return after 3 seconds, summoned afresh by its own spell.
#define DND_GEM_REVIVE_TICS (3 * TICRATE)
Script "DnD Gem Minion Revive" (int pnum, int petid) {
	int spell = GetPetSummonSpell(petid);
	if(spell == -1)
		Terminate;

	int v = GetSpellGemMod(pnum, spell, GEMSTAT_NECRO);
	if(!v || random(1, 100) > v)
		Terminate;

	Delay(const:DND_GEM_REVIVE_TICS);
	if(!PlayerInGame(pnum) || !IsActorAlive(pnum + P_TIDSTART) || !GetSpellAllocatedRank(pnum, spell))
		Terminate;

	ACS_NamedExecuteAlways(spell == SPL_FIREDEMON ? "DnD Summon Fire Demon" : "DnD Summon Ice Golem", 0, pnum);
}

bool IsGemFreeParryOpen(int pnum) {
	return IsGemWindowOpen(GetGemWindows().parry_end[pnum]);
}

// Bladedancer: whether this parry is free, spending the window when it is -- it expires if not parried.
bool ConsumeGemFreeParry(int pnum) {
	auto w = GetGemWindows();
	if(!IsGemWindowOpen(w.parry_end[pnum]))
		return false;
	w.parry_end[pnum] = 0;
	return true;
}

// Fighter: percent faster stamina recovery while the window lasts.
int GetGemStaminaRegenBonus(int pnum) {
	auto w = GetGemWindows();
	return IsGemWindowOpen(w.regen_end[pnum]) ? w.regen_pct[pnum] : 0;
}

// ---- the gem socket page ----------------------------------------------------------------------
// Spells with their sockets, or a picked socket over the inventory's gems; clicks are read clientside and puked.
#define DND_GEMPAGE_Y 52				// the spell grid's first row
#define DND_GEMPAGE_ROWPITCH 36
#define DND_GEMPAGE_GEMY 100			// the gem grid's first row, under the picked spell
#define DND_GEMPAGE_GEMPITCH 30
#define DND_GEMPAGE_SOCKET 10			// a socket's side, two to a cell's width
#define DND_GEMPAGE_GEMSC 2				// the gem icon in a gem cell

int GetGemSocketX(int x, int slot) {
	return x + slot * (DND_GEMPAGE_SOCKET + 1);
}

int GetGemSocketY(int y) {
	return y + DND_SPELLNODE + 2;
}

void AddGemSocketBox(menu_pane_T module& p, int x, int y) {
	AddBoxToPane_Points(p,
		(HUDMAX_X - x) << 16, (HUDMAX_Y - y) << 16,
		(HUDMAX_X - (x + DND_GEMPAGE_SOCKET)) << 16, (HUDMAX_Y - (y + DND_GEMPAGE_SOCKET)) << 16);
}

// One socket in the inventory's own cell art, socket sized at this scale: green hovered, yellow picked, blue filled.
// A frame shrunk this far lost its border, so the socket states are the cell's colour instead.
void DrawGemSocket(int v, int x, int y, int icon_id, bool lit, bool chosen, bool locked = false) {
	int sc = DND_SPELLTREE_ICONSCALE;
	SetHudSize(HUDMAX_X * sc, HUDMAX_Y * sc, 1);

	// A socket its research has not opened: the empty cell, dimmed and never lit.
	if(locked) {
		SetFont("LDTBOX");
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA, icon_id + 1, CR_UNTRANSLATED, ((x * sc) << 16) + 0.1, ((y * sc) << 16) + 0.1, 0.0, 0.3);
		DeleteText(icon_id);
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
		return;
	}

	SetFont(lit ? "LDTBOXS" : chosen ? "LDTBOXC" : v ? "LDTBOXO" : "LDTBOX");
	HudMessage(s:"A"; HUDMSG_PLAIN, icon_id + 1, CR_UNTRANSLATED, ((x * sc) << 16) + 0.1, ((y * sc) << 16) + 0.1, 0.0);

	if(v) {
		SetFont(GetItemImage(GetGemImageId(GetSocketGemSubtype(v))));
		HudMessage(s:"A"; HUDMSG_PLAIN, icon_id, CR_UNTRANSLATED,
			((x * 2 + DND_GEMPAGE_SOCKET) * sc / 2) << 16, ((y * 2 + DND_GEMPAGE_SOCKET) * sc / 2) << 16, 0.0);
	}
	else
		DeleteText(icon_id);
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

// A gem cell in the picker: the empty cell art, the gem centred on it.
void DrawGemCell(int subt, int x, int y, int icon_id, int frame_id, bool lit) {
	int sc = DND_SPELLTREE_ICONSCALE;
	SetHudSize(HUDMAX_X * sc, HUDMAX_Y * sc, 1);
	SetFont("SPLBACK");
	HudMessage(s:"A"; HUDMSG_PLAIN, frame_id, CR_UNTRANSLATED, ((x * sc) << 16) + 0.1, ((y * sc) << 16) + 0.1, 0.0);
	if(lit) {
		SetFont("SPLSLCT");
		HudMessage(s:"A"; HUDMSG_PLAIN, SPELLTREE_HOVER_ID, CR_UNTRANSLATED, ((x * sc) << 16) + 0.1, ((y * sc) << 16) + 0.1, 0.0);
	}

	sc = DND_GEMPAGE_GEMSC;
	SetHudSize(HUDMAX_X * sc, HUDMAX_Y * sc, 1);
	SetFont(GetItemImage(GetGemImageId(subt)));
	HudMessage(s:"A"; HUDMSG_PLAIN, icon_id, CR_UNTRANSLATED,
		((x * 2 + DND_SPELLNODE) * sc / 2) << 16, ((y * 2 + DND_SPELLNODE) * sc / 2) << 16, 0.0);
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

// The pocket for a gem: its name as the heading, then its colour's terms, greyed where the spell cannot use them.
void DrawGemPocket(int subt, int role, int spell) {
	int c = GetGemColour(subt), k, st;
	bool g = IsGreaterGem(subt);

	SetFont("SMALLFONT");
	HudMessage(s:"\c[Y5]", l:GetItemTagName(DND_ITEM_GEM, subt);
		HUDMSG_PLAIN, SPELLTREE_HEAD_ID + 2, CR_WHITE, (DND_SPELLPANEL_X << 16) + 0.1, 24.0, 0.0, 0.0);
	if(spell != -1)
		HudMessage(s:"\c[Y5]", l:"DND_GEM_IN", s:": \c-", l:GetSpellNameLump(spell);
			HUDMSG_PLAIN, SPELLTREE_HEAD_ID + 3, CR_WHITE, (DND_SPELLPANEL_X << 16) + 0.1, 36.0, 0.0, 0.0);
	else
		DeleteText(SPELLTREE_HEAD_ID + 3);

	SetHudClipRect(DND_SPELLPANEL_X - 4, DND_SPELLPANEL_Y - 4, DND_SPELLPANEL_W, DND_SPELLPANEL_H);
	int panel_top = DND_SPELLPANEL_Y + GetScrollBarPos(DND_SPELLBAR_PANEL) * DND_SPELLPANEL_LINEH;
	GetSpellDraw().panel_y = panel_top;
	GetSpellDraw().panel_id = SPELLTREE_DESC_ID;

	for(k = 0; k < GetGemColourTermCount(c); ++k) {
		st = GetGemColourTermStat(c, k);
		PanelText(GetGemStatText(st, GetGemColourTermValue(c, k, g)), spell == -1 || IsGemStatActiveOnSpell(st, spell) ? "\c-" : "\c[K5]");
	}
	if(spell != -1 && !IsGemColourActive(c, spell))
		PanelText(StrParam(l:"DND_GEM_INACTIVE"), "\c[K5]");

	if(role != DND_GEMROLE_NONE) {
		bool live = spell == -1 || IsGemRoleActiveOnSpell(role, spell);
		PanelText("");
		PanelText(StrParam(l:"DND_GEM_OFTHE", s:" ", l:GetGemRoleNameLump(role)), "\c[Y5]");
		PanelText(StrParam(l:GetGemRoleTextLump(role, g)), live ? "\c-" : "\c[K5]");
		if(!live)
			PanelText(StrParam(l:"DND_GEM_INACTIVE"), "\c[K5]");
	}

	GetSpellScroll().panel_px = GetSpellDraw().panel_y - panel_top;
	DeleteTextRange(GetSpellDraw().panel_id, SPELLTREE_DESC_ID + DND_SPELLPANEL_MAXLINES);
	SetHudClipRect(0, 0, 0, 0, 0);
	DrawSpellPocketBar();
}

void HandleSpellGemsDraw(int pnum, int boxid, menu_pane_T module& p) {
	spell_scroll_T module& ss = GetSpellScroll();
	int i, k, n, spell, v, x, y, row, count, base;
	int hovered = -1, gem_subt = -1, gem_role = DND_GEMROLE_NONE, gem_spell = -1, lines = 0;
	bool framed = false;

	ResetPane(p);

	SetHudSize(DND_SPELLTREE_BACKW, DND_SPELLTREE_BACKH, 1);
	SetFont("CRFTVW2");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID, CR_CYAN, 176.0, 100.0, 0.0, 0.0);
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	SetFont("NMENUFNT");

	HudMessage(s:"--- ", l:"DND_SPELLTREE_GEMS", s:" ---";
		HUDMSG_PLAIN, SPELLTREE_HEAD_ID, CR_CYAN, 178.4, 22.0, 0.0, 0.0);

	// Box one is the back arrow, first for the reason the hotbar page gives: the boxes after it change with the view.
	AddBoxToPane_Points(p, 466.0, 36.0, 454.0, 28.0);
	DrawBoxText("<=", DND_NOLOOKUP, boxid, MBOX_1, RPGMENUPAGEID - 1, 16.1, 288.0, "\c[B1]", "\c[Y5]");
	n = 1;

	// A pick on a spell that lost its sockets is dropped rather than drawn.
	if(ss.gem_pick && !GetSpellSocketCount(pnum, (ss.gem_pick - 1) / DND_SPELL_MAXSOCKETS))
		ss.gem_pick = 0;

	if(!ss.gem_pick) {
		count = GetGemEligibleCount(pnum);
		HudMessage(s:"\c[Y5]", l:count ? "DND_GEMPAGE_PICKSOCKET" : "DND_GEMPAGE_NONE";
			HUDMSG_PLAIN, SPELLTREE_HEAD_ID + 1, CR_WHITE, 178.4, 34.0, 0.0, 0.0);

		for(i = 0; i < count; ++i) {
			spell = GetGemEligibleSpell(pnum, i);
			row = i / DND_SPELLPICK_COLS;
			x = GetRowCellX(i % DND_SPELLPICK_COLS, Min(count - row * DND_SPELLPICK_COLS, DND_SPELLPICK_COLS));
			y = DND_GEMPAGE_Y + row * DND_GEMPAGE_ROWPITCH;

			framed |= boxid == MBOX_1 + n;
			if(boxid == MBOX_1 + n)
				hovered = spell;
			DrawSpellCell(spell, x, y, SPELLTREE_NODE_ID + 1 + i, boxid == MBOX_1 + n);
			AddSpellBoxAt(p, x, y);
			++n;

			for(k = 0; k < DND_SPELL_MAXSOCKETS; ++k) {
				v = GetSpellSocket(pnum, spell, k);
				if(boxid == MBOX_1 + n) {
					hovered = spell;
					if(v) {
						gem_subt = GetSocketGemSubtype(v);
						gem_role = GetSocketGemRole(v);
						gem_spell = spell;
					}
				}
				DrawGemSocket(v, GetGemSocketX(x, k), GetGemSocketY(y), SPELLTREE_LINE_ID + 4 * i + 2 * k, boxid == MBOX_1 + n, false, k >= GetSpellSocketCount(pnum, spell));
				AddGemSocketBox(p, GetGemSocketX(x, k), GetGemSocketY(y));
				++n;
			}
		}
		lines = 4 * count;
		k = 1 + count;
	}
	else {
		spell = (ss.gem_pick - 1) / DND_SPELL_MAXSOCKETS;
		int pick_slot = (ss.gem_pick - 1) % DND_SPELL_MAXSOCKETS;
		// Only the gems that can do something on this spell; the message tells none at all from none that fit.
		count = GetInventoryGemCount(pnum, spell);
		HudMessage(s:"\c[Y5]", l:count ? "DND_GEMPAGE_PICKGEM" : GetInventoryGemCount(pnum) ? "DND_GEMPAGE_NOFIT" : "DND_GEMPAGE_NOGEMS";
			HUDMSG_PLAIN, SPELLTREE_HEAD_ID + 1, CR_WHITE, 178.4, 34.0, 0.0, 0.0);

		x = GetRowCellX(0, 1);
		y = DND_GEMPAGE_Y;
		hovered = spell;
		DrawSpellCell(spell, x, y, SPELLTREE_NODE_ID + 1, false);

		// The picked spell's sockets are boxes 2 and 3: clicking the other one moves the pick to it.
		for(k = 0; k < DND_SPELL_MAXSOCKETS; ++k) {
			v = GetSpellSocket(pnum, spell, k);
			if(boxid == MBOX_1 + n) {
				if(v) {
					gem_subt = GetSocketGemSubtype(v);
					gem_role = GetSocketGemRole(v);
					gem_spell = spell;
				}
			}
			DrawGemSocket(v, GetGemSocketX(x, k), GetGemSocketY(y), SPELLTREE_LINE_ID + 2 * k, boxid == MBOX_1 + n, k == pick_slot, k >= GetSpellSocketCount(pnum, spell));
			AddGemSocketBox(p, GetGemSocketX(x, k), GetGemSocketY(y));
			++n;
		}

		DeleteTextRange(SPELLTREE_LINE_ID + 4, SPELLTREE_LINE_ID + 7);
		for(i = 0; i < count; ++i) {
			base = GetInventoryGemBox(pnum, i, spell);
			row = i / DND_SPELLPICK_COLS;
			x = GetRowCellX(i % DND_SPELLPICK_COLS, Min(count - row * DND_SPELLPICK_COLS, DND_SPELLPICK_COLS));
			y = DND_GEMPAGE_GEMY + row * DND_GEMPAGE_GEMPITCH;

			v = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, base, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			if(boxid == MBOX_1 + n) {
				framed = true;
				gem_subt = v;
				gem_role = GetItemSyncValue(pnum, DND_SYNC_ITEMATTRIBUTES_IMPLICIT_ID, base, 0, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
				gem_spell = spell;
			}
			DrawGemCell(v, x, y, SPELLTREE_NODE_ID + 2 + i, SPELLTREE_LINE_ID + 8 + i, boxid == MBOX_1 + n);
			AddSpellBoxAt(p, x, y);
			++n;
		}
		lines = 8 + count;
		k = 2 + count;
	}

	// Only what the last draw reached past this one, so a long list shrinking costs a few deletes, not hundreds.
	if(ss.gem_nodes_drawn > k)
		DeleteTextRange(SPELLTREE_NODE_ID + k, SPELLTREE_NODE_ID + ss.gem_nodes_drawn);
	if(ss.gem_lines_drawn > lines)
		DeleteTextRange(SPELLTREE_LINE_ID + lines, SPELLTREE_LINE_ID + ss.gem_lines_drawn);
	ss.gem_nodes_drawn = k;
	ss.gem_lines_drawn = lines;

	// The cell frame only; sockets show their state in their own colour, and nothing here uses the picked frame.
	if(!framed)
		DeleteText(SPELLTREE_HOVER_ID);
	DeleteText(SPELLTREE_PICKSEL_ID);

	if(gem_subt != -1) {
		DrawGemPocket(gem_subt, gem_role, gem_spell);
		return;
	}

	if(!ss.pinned && ss.shown_spell != hovered) {
		ss.shown_spell = hovered;
		SetScrollBarPos(DND_SPELLBAR_PANEL, 0);
	}
	DrawSpellPocket(pnum);
}

// CLIENTSIDE, from the menu loop beside HandleSpellHotbarPick, for the same reason: the view decides what a box
// means, and the view is clientside. Left on a socket picks it, right on a filled one takes its gem out.
bool HandleSpellGemsPick(int pnum, int boxid, int input) {
	if(input != DND_MENUINPUT_LCLICK && input != DND_MENUINPUT_RCLICK && input != DND_MENUINPUT_JUMPCLICK)
		return false;

	spell_scroll_T module& ss = GetSpellScroll();

	// Outside every box closes the gem view; box one is the back arrow, which the server handles.
	if(boxid <= MBOX_1) {
		if(boxid == MBOX_1 || !ss.gem_pick)
			return false;
		ss.gem_pick = 0;
		return true;
	}

	int idx = boxid - MBOX_1 - 1, spell, slot;
	if(!ss.gem_pick) {
		spell = GetGemEligibleSpell(pnum, idx / (1 + DND_SPELL_MAXSOCKETS));
		slot = idx % (1 + DND_SPELL_MAXSOCKETS) - 1;
		if(spell == -1 || slot < 0)
			return false;
	}
	else {
		spell = (ss.gem_pick - 1) / DND_SPELL_MAXSOCKETS;
		slot = idx;
	}

	// A socket, in either view. One its research has not opened does nothing.
	if(slot < DND_SPELL_MAXSOCKETS) {
		if(slot >= GetSpellSocketCount(pnum, spell))
			return false;
		if(input == DND_MENUINPUT_RCLICK) {
			if(GetSpellSocket(pnum, spell, slot))
				NamedRequestScriptPuke("DnD Gem Unsocket", spell, slot);
			return false;
		}
		ss.gem_pick = spell * DND_SPELL_MAXSOCKETS + slot + 1;
		LocalAmbientSound("RPG/MenuChoose", 127);
		return true;
	}

	// A gem in the picker: socket it. The view closes either way, as the hotbar picker does.
	int box = GetInventoryGemBox(pnum, slot - DND_SPELL_MAXSOCKETS, spell);
	if(box != -1 && input != DND_MENUINPUT_RCLICK)
		NamedRequestScriptPuke("DnD Gem Socket", spell, (ss.gem_pick - 1) % DND_SPELL_MAXSOCKETS, box);
	ss.gem_pick = 0;
	return true;
}

#endif
