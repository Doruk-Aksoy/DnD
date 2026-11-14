#ifndef DND_SPELLTREE_IN
#define DND_SPELLTREE_IN

#include "DnD_SpellHud.h"
#include "DnD_SpellTreeRoutes.h"

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
#define DND_SPELLTREE_GAPY 12
#define DND_SPELLTREE_COLW (DND_SPELLNODE + DND_SPELLTREE_GAPX)
#define DND_SPELLTREE_ROWH (DND_SPELLNODE + DND_SPELLTREE_GAPY)

// The board the grid is centred in, rather than a fixed origin. Trees are not the same shape --
// Fire is 8 columns by 7 rows, Cold 6 by 6 -- so a shared origin left the smaller ones hugging the
// top left corner with the whole right half of the board empty.
#define DND_SPELLTREE_LEFT 16
#define DND_SPELLTREE_BOARDW 322
#define DND_SPELLTREE_TOP 53
#define DND_SPELLTREE_BOARDH 233

// Connector line thickness box: the strips are this tall or wide, the joint tiles this square.
#define DND_SPELLLINE_JOINT 7

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

// The pocket's text cursor, and which trees already warned about a stale route table.
typedef struct {
	int panel_y, panel_id;
	int stale_warned;
} spell_draw_T;

spell_draw_T module& GetSpellDraw() {
	static spell_draw_T s;
	return s;
}

// Same hash as the router: positions and requirement slots, so a table routed from another shape is never drawn.
int GetSpellTreeSignature(int tree) {
	int i, k, s, sig = tree + 1;
	for(i = 0; i < DND_SPELLS_PER_TREE; ++i) {
		s = GetTreeFirstSpell(tree) + i;
		if(!IsSpellDefined(s))
			continue;
		sig = (sig * 31 + i + 1) % 1000003;
		sig = (sig * 31 + SpellDefs[s].tx) % 1000003;
		sig = (sig * 31 + SpellDefs[s].ty) % 1000003;
		for(k = 0; k < DND_MAX_SKILL_REQ; ++k)
			sig = (sig * 31 + SpellDefs[s].req_spell[k]) % 1000003;
	}
	return sig;
}

// Strips (kinds 0-1) clip to the span. Single pictures cannot be tinted, so a coloured piece is the same art as a SPLLINES glyph.
str GetSpellLineTile(int kind) {
	switch(kind) {
		case 0: return "SPLLHL";
		case 1: return "SPLLVL";
		case 2: return "SPLLCUL";
		case 3: return "SPLLCUR";
		case 4: return "SPLLCDL";
		case 5: return "SPLLCDR";
		case 6: return "SPLLTL";
		case 7: return "SPLLTR";
		case 8: return "SPLLTU";
		case 9: return "SPLLTD";
	}
	return "SPLLX";
}

void DrawSpellLinePiece(int kind, int x, int y, int len, int id, str col) {
	if(kind == 0)
		MenuClipRect(x, y, len, DND_SPELLLINE_JOINT);
	else if(kind == 1)
		MenuClipRect(x, y, DND_SPELLLINE_JOINT, len);
	if(StrLen(col)) {
		SetFont("SPLLINES");
		HudMessage(s:col, c:'A' + kind; HUDMSG_PLAIN, id, CR_UNTRANSLATED, MenuX((x << 16) + 0.1), MenuY((y << 16) + 0.1), 0.0, 0.0);
	}
	else {
		SetFont(GetSpellLineTile(kind));
		HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_UNTRANSLATED, MenuX((x << 16) + 0.1), MenuY((y << 16) + 0.1), 0.0, 0.0);
	}
	if(kind < 2)
		MenuClipRect(0, 0, 0, 0);
}

// Each requirement bus is its own network; only the highlighted spell's own paths draw cyan. Returns the next free id.
int DrawRoutedConnectors(int pnum, int tree, int hl) {
	int i, a, b, spell, pass, id = SPELLTREE_CROSS_ID;
	int first = GetSpellRouteInfo(tree, 1), last = first + GetSpellRouteInfo(tree, 2);
	bool lit;

	hl -= GetTreeFirstSpell(tree);
	if(hl < 0 || hl >= DND_SPELLS_PER_TREE)
		hl = -1;

	// Joints first, so the lower ids put them in front of the strips they cap.
	for(pass = 0; pass < 2; ++pass) {
		for(i = first; i < last; ++i) {
			a = GetSpellRoutes()[2 * i];
			if(((a & 15) < 2) != (pass == 1))
				continue;
			b = GetSpellRoutes()[2 * i + 1];
			spell = GetTreeFirstSpell(tree) + ((b >> 6) & 63);

			// A parent's leg lights once it meets any child of the bus, everything else once its child can open.
			if(b & (1 << 12))
				lit = GetSpellAllocatedRank(pnum, spell) >= (b >> 13);
			else
				lit = SpellReqsMet(pnum, spell);

			DrawSpellLinePiece(a & 15, (a >> 4) & 511, (a >> 13) & 511, (a >> 22) & 511, id++,
				(hl != -1 && (GetSpellRouteHot()[2 * i + (hl >> 5)] >> (hl & 31)) & 1) ? "\c[C1]" : lit ? "\c[C3]" : "");
		}
	}
	return id;
}

// Plain elbows, one per requirement, for a tree whose route table is stale.
int DrawPlainConnectors(int pnum, int tree, int id) {
	int i, k, req, s, px, py, cx, cy, midy, jt = DND_SPELLLINE_JOINT / 2;
	str col;

	for(i = 0; i < DND_SPELLS_PER_TREE; ++i) {
		s = GetTreeFirstSpell(tree) + i;
		if(!IsSpellDefined(s))
			continue;
		for(k = 0; k < DND_MAX_SKILL_REQ; ++k) {
			req = SpellDefs[s].req_spell[k] - 1;
			if(req < 0 || !IsSpellDefined(req) || SpellDefs[req].tree != tree)
				continue;

			col = GetSpellAllocatedRank(pnum, req) >= SpellDefs[s].req_rank[k] ? "\c[C3]" : "";
			px = GetSpellNodeX(req) + DND_SPELLNODE / 2;
			py = GetSpellNodeY(req) + DND_SPELLNODE;
			cx = GetSpellNodeX(s) + DND_SPELLNODE / 2;
			cy = GetSpellNodeY(s);
			midy = (py + cy) / 2;
			DrawSpellLinePiece(1, px - jt, py - 1, midy - py + 2, id++, col);
			DrawSpellLinePiece(1, cx - jt, midy - 1, cy - midy + 2, id++, col);
			if(px != cx)
				DrawSpellLinePiece(0, Min(px, cx) - 1, midy - jt, Max(px, cx) - Min(px, cx) + 3, id++, col);
		}
	}
	return id;
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

// The binds strip under Gem Sockets, a window down to the panel's floor that the wrapped text scrolls in.
#define DND_SPELLINDEX_HELP_Y (116 + MAX_SKILL_TREES * 16)
#define DND_SPELLINDEX_HELP_H (280 - DND_SPELLINDEX_HELP_Y)
#define DND_SPELLINDEX_HELP_STEP 9		// one line of the menu font, so the window never shows half a line
#define DND_SPELLINDEX_HELP_SPAN 6		// nine wrapped lines past the window's four, plus one for a longer bind name

// The eight trees as rows. Only the two with content are selectable; the rest are drawn dim
// rather than hidden, so the shape of what is coming is visible.
void HandleSpellIndexDraw(int pnum, int boxid) {
	HudMessage(s:"--- ", l:"DND_MENU_HEAD_SPELLS", s:" ---"; HUDMSG_PLAIN, RPGMENUHELPID, CR_CYAN, MenuX(316.4), MenuY(44.0), 0.0, 0.0);
	HudMessage(s:"\c[Y5]", l:"DND_MENU_SPELLPOINTS", s:": \c-", d:GetSpellPoints(pnum);
		HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, MenuX(192.1), MenuY(60.0), 0.0, 0.0);

	int i, y;
	for(i = 0; i < MAX_SKILL_TREES; ++i) {
		y = 80 + i * 16;
		DrawBoxText(StrParam(l:StrParam(s:"DND_SPELLTREE", d:i)), DND_NOLOOKUP, boxid, MBOX_1 + i,
			RPGMENUITEMID - 1 - i, 192.1, y << 16, "\c[B1]", GetTreeSpellCount(i) ? "\c-" : "\c[K5]");
	}

	HudMessage(s:"\c[Y5]-------------------------"; HUDMSG_PLAIN, RPGMENUITEMID - 3 - MAX_SKILL_TREES, CR_WHITE, MenuX(192.1), MenuY(80.0 + 16.0 * MAX_SKILL_TREES), 0.0, 0.0);

	// Last, so entry n stays box n however many trees get content.
	DrawBoxText("DND_SPELLTREE_HOTBAR", DND_LANGUAGE_LOOKUP, boxid, MBOX_1 + MAX_SKILL_TREES,
		RPGMENUITEMID - 1 - MAX_SKILL_TREES, 192.1, (88 + MAX_SKILL_TREES * 16) << 16, "\c[B1]", "\c-");
	DrawBoxText("DND_SPELLTREE_GEMS", DND_LANGUAGE_LOOKUP, boxid, MBOX_2 + MAX_SKILL_TREES,
		RPGMENUITEMID - 2 - MAX_SKILL_TREES, 192.1, (104 + MAX_SKILL_TREES * 16) << 16, "\c[B1]", "\c-");

	// The trees' binds, told before entering one, as the perk index does.
	MenuClipRect(192, DND_SPELLINDEX_HELP_Y, 256, DND_SPELLINDEX_HELP_H, 256);
	HudMessage(
		s:"\cv*\cu ", l:"DND_SPELLINDEX_HELP1", s:"\n\cv*\cu ", l:"DND_SPELLINDEX_HELP2", s:"\n\cv*\cu ",
		l:"DND_MENU_PERK_PINHELP1", s:" \ci", k:"+jump", s:"\cu ", l:"DND_MENU_PERK_PINHELP2", s:"\n\cv*\cu ",
		l:"DND_SPELLINDEX_REFUND1", s:" \ci", k:"+jump", s:"\cu ", l:"DND_SPELLINDEX_REFUND2";
		HUDMSG_PLAIN, RPGMENUITEMID - 4 - MAX_SKILL_TREES, CR_WHITE,
		MenuX(192.1), MenuY(((DND_SPELLINDEX_HELP_Y + DND_SPELLINDEX_HELP_STEP * ScrollPos.x) << 16) + 0.1), 0.0, 0.0
	);
	MenuClipRect(0, 0, 0, 0, 0);
}

// The binds strip's bar, on a short track beside the strip rather than down the whole page.
bool HandleSpellIndexScroll() {
	return ListenScroll(-DND_SPELLINDEX_HELP_SPAN, 0, DND_SPELLINDEX_HELP_STEP, DND_SPELLINDEX_HELP_H,
		0, DND_SPELLINDEX_HELP_Y, DND_SPELLINDEX_HELP_H);
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
#define DND_SPELLPANEL_Y 91
#define DND_SPELLPANEL_HEADY 67	// the heading's first line, centred in the board with the pocket art
// The pocket runs to the RIGHT EDGE OF THE SCREEN -- the art continues past 480 but nothing can be
// drawn or clicked there. 384..480 is all there is.
//
// SMALLFONT is about 8.7 units per character, so 21 of them wanted 183 units in a 96 unit pocket:
// the text was always overflowing, and clipping it only made that visible. NSMOLFNT is the smaller
// face and is what this is sized for.
#define DND_SPELLPANEL_W 148
#define DND_SPELLPANEL_H 192
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
			MenuX((DND_SPELLPANEL_X << 16) + 0.1), MenuY((GetSpellDraw().panel_y << 16) + 0.1), 0.0);
		GetSpellDraw().panel_y += DND_SPELLPANEL_LINEH;

		start = brk;
		while(start < len && GetChar(text, start) == ' ')
			++start;
	}
}

// Characters the player sees, colour escapes not counted.
int PanelVisibleLength(str text) {
	int len = StrLen(text), k = 0, seen = 0;
	while(k < len) {
		if(GetChar(text, k) == 28) {
			++k;
			if(k < len && GetChar(text, k) == '[')
				while(k < len && GetChar(text, k) != ']')
					++k;
		}
		else
			++seen;
		++k;
	}
	return seen;
}

// A heading line at y: past DND_SPELLPANEL_WRAP characters it goes on a canvas widened to match, so it shrinks to fit.
void PanelHeading(str text, int id, int y) {
	int n = Max(DND_SPELLPANEL_WRAP, PanelVisibleLength(text));
	SetFont(DND_SPELLPANEL_FONT);
	MenuHudSize(HUDMAX_X * n / DND_SPELLPANEL_WRAP, HUDMAX_Y * n / DND_SPELLPANEL_WRAP);
	HudMessage(s:text; HUDMSG_PLAIN, id, CR_WHITE,
		MenuX(((DND_SPELLPANEL_X * n << 16) / DND_SPELLPANEL_WRAP) + 0.1), MenuY((y * n / DND_SPELLPANEL_WRAP) << 16), 0.0, 0.0);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

// A string split on its line breaks, each line wrapped by PanelText; an empty line is a half-line spacer.
void PanelTextLines(str text, str col = "") {
	int len = StrLen(text), start = 0;
	for(int k = 0; k <= len; ++k) {
		if(k == len || GetChar(text, k) == 10) {	// \n
			PanelText(StrMid(text, start, k - start), col);
			start = k + 1;
		}
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

// "lo-hi", or one number when they match.
str GetRangeText(int lo, int hi) {
	return lo == hi ? StrParam(d:lo) : StrParam(d:lo, s:"-", d:hi);
}

// "STR 0.15%, INT 0.25%": damage per point of each attuned attribute.
str GetSpellAttunementText(int spell) {
	str res = "";
	int v;
	for(int i = STAT_STR; i <= STAT_INT; ++i) {
		v = GetSpellAttunementRaw(spell, i);
		if(!v)
			continue;
		res = StrParam(s:res, s:StrLen(res) ? ", " : "", l:GetAttributeLabel_Short(i), s:" ", d:v / 100, s:".", d:(v / 10) % 10, d:v % 10, s:"%");
	}
	return res;
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
	MenuClipRect(DND_SPELLPANEL_X - 4, DND_SPELLPANEL_Y - 4, DND_SPELLPANEL_W, DND_SPELLPANEL_H);

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
			PanelText(StrParam(s:"\c[Y5]", l:GetSpellFieldLumpFor(spell, i), s:": \c-", d:GetSpellValue(pnum, spell, lo_field, preview), s:"-", d:temp));
		else
			PanelText(StrParam(s:"\c[Y5]", l:GetSpellFieldLumpFor(spell, i), s:": \c-", d:temp));

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

	// A summon's live health and hits, as its scripts compute them, before crits.
	if(GetSummonPetKind(spell) != -1) {
		int lo, hi, nlo, nhi;
		for(i = 0; StrLen(GetSummonRowLump(spell, i)); ++i) {
			lo = GetSummonRowValue(pnum, spell, i, preview, false);
			hi = GetSummonRowValue(pnum, spell, i, preview, true);
			PanelText(StrParam(s:"\c[Y5]", l:GetSummonRowLump(spell, i), s:" \c-", s:GetRangeText(lo, hi)));

			if(alloc && alloc < DND_SPELL_RANKCAP) {
				nlo = GetSummonRowValue(pnum, spell, i, alloc + 1, false);
				nhi = GetSummonRowValue(pnum, spell, i, alloc + 1, true);
				if(nhi > hi)
					PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_NEXTRANK", s:" \c-", s:GetRangeText(nlo, nhi), s:" (+", d:nhi - hi, s:")"));
			}
		}
	}

	// Paid in life at the rate PaySpellCost takes it.
	bool blood = HasPlayerFlag(pnum, PFLAG_BLOODMAGIC);
	int lifecost = GetSpellLifeCostRate(pnum, spell);
	str payres = lifecost ? " life" : " mana";

	temp = GetSpellValue(pnum, spell, SPELLVAL_COST, preview) >> 16;
	if(SpellDefs[spell].flags & SPLF_RESERVES)
		PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_RESERVE", s:" \c-", d:temp, s:blood ? "% life" : "% mana"));
	else if(temp) {
		if(lifecost)
			temp = temp * lifecost / 100;
		PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_COST", s:" \c-", d:temp, s:payres));
		if(alloc && alloc < DND_SPELL_RANKCAP) {
			int nc = GetSpellValue(pnum, spell, SPELLVAL_COST, alloc + 1) >> 16;
			if(lifecost)
				nc = nc * lifecost / 100;
			if(nc > temp)
				PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_NEXTRANK", s:" \c-", d:nc,
					s:payres, s:" (+", d:nc - temp, s:")"));
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

	// Only where it matters: a damage field, a summon, or a profile of its own like Righteous Fire's.
	if(SpellDefs[spell].scale_mask || GetSummonPetKind(spell) != -1 || GetSpellAttunementOverride(spell))
		PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_ATTUNE", s:" \c-", s:GetSpellAttunementText(spell)));

	// On a tree, the only page that takes refunds: what Jump + right click would charge for a rank, or why it cannot.
	if(alloc && IsSpellTreePage(CheckInventory("MenuOption"))) {
		PanelText("");
		temp = GetSpellRefundStatus(pnum, spell);
		if(temp == SPELLREFUND_NONE)
			PanelText(StrParam(s:"\c-", l:"DND_SPLPANEL_REFUND1", s:" \ci", k:"+jump", s:"\c- ", l:"DND_SPLPANEL_REFUND2",
				s:" \cd$", d:GetSpellRespecCost(pnum, spell), s:" \c-", l:"DND_MENU_CREDITS"), "\c-");
		else if(temp == SPELLREFUND_POOR)
			PanelText(StrParam(s:"\c[D4]", l:"DND_SPLPANEL_REFUNDPOOR", s:" (\cg$", d:GetSpellRespecCost(pnum, spell), s:"\c[D4])"), "\c[D4]");
		else if(temp == SPELLREFUND_LOCKS)
			PanelText(StrParam(s:"\c[D4]", l:"DND_SPLPANEL_REFUNDLOCKS", s:" \c-", l:StrParam(s:"DND_SPLNAME", d:GetSpellRefundBlocker(pnum, spell))), "\c[D4]");
		else if(temp == SPELLREFUND_GEMS)
			PanelText(StrParam(s:"\c[D4]", l:"DND_SPLPANEL_REFUNDGEMS"), "\c[D4]");
		else if(temp == SPELLREFUND_RUNNING)
			PanelText(StrParam(s:"\c[D4]", l:"DND_SPLPANEL_REFUNDRUNNING"), "\c[D4]");
	}

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
					IsSpellFieldPoisonTenths(spell, SpellSynergies[i].field) ?
					FixedToTenths((SpellSynergies[i].per_rank << 16) / 10) :
					IsSpellFieldFixedPoint(SpellSynergies[i].field) ?
					FixedToHundredths(SpellSynergies[i].per_rank) :
					StrParam(d:SpellSynergies[i].per_rank),
				s:" ", l:GetSpellFieldLumpFor(spell, SpellSynergies[i].field), s:GetSynergyStepText(i)));
		else
			PanelText(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_SYNERGY", s:" - \c-", l:GetSpellNameLump(temp),
				s:": ", s:FixedToTenths(SpellSynergies[i].per_rank), s:"% more ",
				l:GetSpellFieldLumpFor(spell, SpellSynergies[i].field), s:GetSynergyStepText(i)));
	}

	// Measured rather than counted: blank spacers are half a line, so a line count overstated the
	// content and the bar stopped short of the last row.
	GetSpellScroll().panel_px = GetSpellDraw().panel_y - panel_top;

	// Whatever the previous hover left behind, when this one is shorter.
	DeleteTextRange(GetSpellDraw().panel_id, SPELLTREE_DESC_ID + DND_SPELLPANEL_MAXLINES);
	MenuClipRect(0, 0, 0, 0, 0);
}

// The pocket's bar. Range comes from what the last draw actually emitted, so it can never disagree
// with the text it scrolls.
// The shared bar is used for its STATE only -- range, position, grab -- and its own drawing is
// left to paint harmlessly behind the backdrop. This is the copy that is actually seen, at low ids
// and at the pocket's right edge rather than the global DND_SCROLLBAR_X, which lands mid-pocket.
void DrawSpellPocketBar() {
	scrollbar_T module& bar = GetScrollBar(DND_SPELLBAR_PANEL);
	MchSpellPocketBar(bar.range.y - GetScrollBarPos(DND_SPELLBAR_PANEL), CanShowScrollBar(bar) ? bar.range.y - bar.range.x : 0);
}

bool HandleSpellPageScroll(int tree, int boxid) {
	spell_scroll_T module& ss = GetSpellScroll();

	// One line of slack, or the final row sits half under the clip's bottom edge. An empty pocket measures 0.
	int over = ss.panel_px + DND_SPELLPANEL_LINEH - DND_SPELLPANEL_H;
	if(over < 0)
		over = 0;

	// the chrome bar takes the mouse, so the shared bar's own grab sits off screen
	bool r = MchSpellPocketDrag();
	bool k = ListenScroll(-(over / DND_SPELLPANEL_LINEH), 0, DND_SPELLPANEL_LINEH, DND_SPELLPANEL_H,
		DND_SPELLBAR_PANEL, DND_SPELLPANEL_Y, DND_SPELLPANEL_H, !MchSbarGrabbed(), -2000);
	return k || r;
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
		ss.panel_px = 0;
		DrawSpellPocketBar();
		return;
	}

	int hrank = GetSpellRank(pnum, ss.shown_spell, true);
	int halloc = GetSpellAllocatedRank(pnum, ss.shown_spell);

	PanelHeading(StrParam(s:"\c[Y5]", l:GetSpellNameLump(ss.shown_spell)), SPELLTREE_HEAD_ID + 2, DND_SPELLPANEL_HEADY);

	// Effective rank in brackets only where gear is actually adding to it.
	if(halloc && hrank != halloc)
		PanelHeading(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_LEVEL", s:": \c-", d:halloc, s:" (", d:hrank, s:")"), SPELLTREE_HEAD_ID + 3, DND_SPELLPANEL_HEADY + 12);
	else
		PanelHeading(StrParam(s:"\c[Y5]", l:"DND_SPLPANEL_LEVEL", s:": \c-", d:halloc, s:"/", d:DND_SPELL_RANKCAP), SPELLTREE_HEAD_ID + 3, DND_SPELLPANEL_HEADY + 12);

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

	MenuHudSize(HUDMAX_X * sc, HUDMAX_Y * sc);

	// SPLBACK is what an empty slot looks like; SPLSLCT is what HOVER looks like. One id either way:
	// a cell is an icon or an empty frame, never both.
	SetFont(spell >= 0 ? GetSpellIcon(spell, false) : "SPLBACK");
	HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_UNTRANSLATED,
		MenuX(((x * sc) << 16) + 0.1), MenuY(((y * sc) << 16) + 0.1), 0.0);

	if(lit || chosen) {
		SetFont("SPLSLCT");
		HudMessage(s:"A"; HUDMSG_PLAIN, chosen ? SPELLTREE_PICKSEL_ID : SPELLTREE_HOVER_ID,
			CR_UNTRANSLATED, MenuX(((x * sc) << 16) + 0.1), MenuY(((y * sc) << 16) + 0.1), 0.0);
	}

	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

void HandleSpellHotbarDraw(int pnum, int boxid, menu_pane_T module& p) {
	spell_scroll_T module& ss = GetSpellScroll();
	int i, n, spell, x, y, hovered = -1;
	bool chosen = false;
	int slots = GetHotbarSlotCount(pnum);
	int bindable = GetBindableCount(pnum);
	bool framed = false;

	ResetPane(p);

	// the menu frame is the backdrop now; the page name is its sub tab
	DrawChromeSpellPocket();
	SetFont("NMENUFNT");
	DeleteText(SPELLTREE_HEAD_ID);

	DrawChromeSpellHint(ss.picking_slot == -1 ? "DND_SPLPANEL_PICKSLOT" : "DND_SPLPANEL_PICKSPELL");
	DeleteText(SPELLTREE_HEAD_ID + 7);

	// box one stays, parked: the frame's tabs go back now, and the boxes after it keep their numbers
	AddBoxToPane_Points(p, -1, -1, -1, -1);
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
			MenuX(((x + DND_SPELLNODE / 2) << 16) + 0.4),
			MenuY(((y + DND_SPELLNODE + 2) << 16) + 0.1), 0.0);

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
				HUDMSG_PLAIN, SPELLTREE_HEAD_ID + 6, CR_WHITE, MenuX((mid << 16) + 0.4), MenuY(DND_SPELLPICK_PAGEY << 16), 0.0, 0.0);
		}
	}
	if(ss.picking_slot == -1 || pages == 1)
		DeleteTextRange(SPELLTREE_HEAD_ID + 4, SPELLTREE_HEAD_ID + 6);

	// Whatever the last, longer layout left behind: the rest of the node ids, which end where the
	// crossings begin. Never past them -- sweeping the whole id space wiped the tree's connectors.
	DeleteTextRange(SPELLTREE_NODE_ID + drawn, SPELLTREE_CROSS_ID - 1);
	// + 1: the labels start at RANK_ID + 1, because box one is the parked old back arrow. Sweeping from
	// RANK_ID + slots deleted the LAST slot's number every frame.
	DeleteTextRange(SPELLTREE_RANK_ID + slots + 1, SPELLTREE_RANK_ID + DND_SPELLS_PER_TREE - 1);

	// Not `hovered`: an EMPTY slot under the cursor is framed but has no spell, and keying the frame
	// off hovered deleted it the moment it was drawn.
	if(!framed)
		DeleteText(SPELLTREE_HOVER_ID);

	// Survives the cursor moving off the slot and onto a spell -- that is the whole point of it.
	if(!chosen)
		DeleteText(SPELLTREE_PICKSEL_ID);

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
// The bind itself is puked to "DnD Bind Hotbar" with the spell id, which the server re-checks.
//
// Returns whether the page owes a redraw.
bool HandleSpellHotbarPick(int pnum, int boxid, int input) {
	spell_scroll_T module& ss = GetSpellScroll();
	int slots = GetHotbarSlotCount(pnum);

	// Right click on a filled slot empties it: the bind takes a negative slot as "clear".
	if(input == DND_MENUINPUT_RCLICK) {
		int s = boxid - MBOX_1 - 1;
		if(boxid < MBOX_1 || s < 0 || s >= slots || !CheckActorInventory(pnum + P_TIDSTART, GetHotbarSlotItem(s)))
			return false;
		NamedRequestScriptPuke("DnD Bind Hotbar", -s - 1);
		ss.picking_slot = -1;
		ss.picking_page = 0;
		return true;
	}

	if(input != DND_MENUINPUT_LCLICK && input != DND_MENUINPUT_JUMPCLICK)
		return false;

	// Outside every box: closes the picker, nothing else.
	if(boxid < MBOX_1) {
		if(ss.picking_slot == -1)
			return false;
		ss.picking_slot = -1;
		ss.picking_page = 0;
		return true;
	}

	// Box one is parked (the old back arrow): nothing to pick there.
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
	NamedRequestScriptPuke("DnD Bind Hotbar", ss.picking_slot, GetBindableByIndex(pnum, ss.picking_page * DND_SPELLPICK_PERPAGE + idx));

	ss.picking_slot = -1;
	ss.picking_page = 0;
	return true;
}

bool IsSpellTreeEmpty(int tree) {
	int first = GetTreeFirstSpell(tree);
	for(int j = 0; j < DND_SPELLS_PER_TREE; ++j)
		if(IsSpellDefined(first + j))
			return false;
	return true;
}

void HandleSpellTreeDraw(int pnum, int tree, int boxid, menu_pane_T module& p) {
	int i, j, spell, rank, alloc, n = 0, line, hovered = -1;
	bool unlocked;

	ResetPane(p);
	if(IsSpellTreeEmpty(tree)) {
		DeleteText(RPGMENUID);
		return;
	}

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

	// the menu frame is the backdrop; the info panel keeps a pane at RPGMENUID, in front of the frame
	DrawChromeSpellPocket();
	SetFont("NMENUFNT");

	// The tree's name is its sub tab, and points and credits sit on the frame's status strip.
	DeleteTextRange(SPELLTREE_HEAD_ID, SPELLTREE_HEAD_ID + 1);
	DeleteText(SPELLTREE_HEAD_ID + 7);

	// Edges before nodes so every icon sits on top of the lines reaching it; the hovered or pinned spell's buses light.
	int first = GetTreeFirstSpell(tree);
	if(GetSpellRouteInfo(tree, 0) == GetSpellTreeSignature(tree))
		line = DrawRoutedConnectors(pnum, tree, boxid >= MBOX_1 ? GetTreeSpellByIndex(tree, boxid - MBOX_1) : ss.pinned ? ss.shown_spell : -1);
	else {
		if(!(GetSpellDraw().stale_warned & (1 << tree))) {
			GetSpellDraw().stale_warned |= 1 << tree;
			Log(s:"Spell tree ", d:tree, s:": route table is stale, drawing plain connectors. Regenerate DnD_SpellTreeRoutes.h.");
		}
		line = DrawPlainConnectors(pnum, tree, SPELLTREE_CROSS_ID);
	}
	DeleteTextRange(line, SPELLPAGE_ID_LAST - 1);

	for(j = 0; j < DND_SPELLS_PER_TREE; ++j) {
		i = first + j;
		if(!IsSpellDefined(i))
			continue;

		spell = i;
		alloc = GetSpellAllocatedRank(pnum, spell);
		unlocked = alloc > 0;

		// The only thing on the page drawn outside the default space. Coordinates scale with it, and
		// the hit box in AddSpellNodeBox stays in menu units -- that is the space boxes always use.
		MenuHudSize(HUDMAX_X * DND_SPELLTREE_ICONSCALE, HUDMAX_Y * DND_SPELLTREE_ICONSCALE);
		SetFont(GetSpellIcon(spell, !unlocked));
		HudMessage(s:"A"; HUDMSG_PLAIN, SPELLTREE_NODE_ID + n, CR_UNTRANSLATED,
			MenuX(((GetSpellNodeX(spell) * DND_SPELLTREE_ICONSCALE) << 16) + 0.1),
			MenuY(((GetSpellNodeY(spell) * DND_SPELLTREE_ICONSCALE) << 16) + 0.1), 0.0);

		// The selection frame is 64x64 like the icons, so it rides the same canvas and the same
		// coordinates -- one id, because only one node is ever hovered.
		if(boxid == MBOX_1 + n) {
			SetFont("SPLSLCT");
			HudMessage(s:"A"; HUDMSG_PLAIN, SPELLTREE_HOVER_ID, CR_UNTRANSLATED,
				MenuX(((GetSpellNodeX(spell) * DND_SPELLTREE_ICONSCALE) << 16) + 0.1),
				MenuY(((GetSpellNodeY(spell) * DND_SPELLTREE_ICONSCALE) << 16) + 0.1), 0.0);
		}

		MenuHudSize(HUDMAX_X, HUDMAX_Y);

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
				MenuX(((GetSpellNodeX(spell) + DND_SPELLNODE) << 16) + 0.2),
				MenuY(((GetSpellNodeY(spell) + 1) << 16) + 0.1), 0.0);
		else
			HudMessage(s:"\c[Y5]", d:alloc;
				HUDMSG_PLAIN, SPELLTREE_RANK_ID + n, CR_WHITE,
				MenuX(((GetSpellNodeX(spell) + DND_SPELLNODE) << 16) + 0.2),
				MenuY(((GetSpellNodeY(spell) + 1) << 16) + 0.1), 0.0);

		if(boxid == MBOX_1 + n)
			hovered = spell;

		AddSpellNodeBox(p, spell);
		++n;
	}

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

	// the old back arrow's box, parked after every node so entry n stays box n
	AddBoxToPane_Points(p, -1, -1, -1, -1);
	DeleteText(RPGMENUPAGEID - 1);
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

// JUMP + right click: one rank back for credits, through the same gate the panel shows.
bool HandleSpellTreeRefund(int pnum, int tree, int boxid) {
	if(boxid < MBOX_1)
		return false;

	int spell = GetTreeSpellByIndex(tree, boxid - MBOX_1);
	return spell != -1 && RespecSpellPoint(pnum, spell);
}

// ---- gem sockets: the mechanics --------------------------------------------------------------
bool SpellHasRow(int spell, int field) {
	return SpellDefs[spell].base[field] > 0 || SpellDefs[spell].per_rank[field] > 0;
}

// Damage the player's own casts deal: not a support spell's, not a summon's (its minions' hits are not the spell's).
bool SpellDealsOwnDamage(int spell) {
	return !(SpellDefs[spell].flags & (SPLF_SUPPORT | SPLF_SUMMON | SPLF_NODAMAGE));
}

// Hits the player's own casts land; damage over time never crits, leeches or rolls the hit-only ailments.
bool SpellLandsOwnHits(int spell) {
	return SpellDealsOwnDamage(spell) && !(SpellDefs[spell].flags & SPLF_NOHIT);
}

// A summon whose minions land hits of their own, for the hit gems that roll on them (not the Ghast: its attack is your
// Dark Wind, which takes Dark Wind's; not the portal, which deals no damage).
bool IsMinionHitSpell(int spell) {
	return (SpellDefs[spell].flags & SPLF_SUMMON) && spell != SPL_GHAST && spell != SPL_DARKPORTAL;
}

// Whether Quicktongue's repeat can add anything: not on a curse or a mark (re-applied, the strongest wins), a flock or corpses
// a recast replaces, a channel that refuses a second, or a summon capped at one of its kind (the repeat evicts it).
bool CanSpellRepeat(int spell) {
	int f = SpellDefs[spell].flags;
	if(f & (SPLF_CURSE | SPLF_TOGGLE) || spell == SPL_MARKPREY || spell == SPL_CARRIONCROWS || spell == SPL_UNLEASHEARTH || spell == SPL_DESECRATE)
		return false;
	return !(f & SPLF_SUMMON) || GetSummonKindMaxCap(spell) > 1;
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
		case GEMSTAT_CRITCHANCE:
		case GEMSTAT_CRITMULT:
		case GEMSTAT_MORE_HITDAMAGE:
		return SpellLandsOwnHits(spell);
	}
	// damage and ailments: the player's own damage
	return SpellDealsOwnDamage(spell);
}

// A colour does something on this spell when any of its terms can.
bool IsGemColourActive(int colour, int spell) {
	for(int k = 0; k < GetGemColourTermCount(colour); ++k)
		if(IsGemStatActiveOnSpell(GetGemColourTermStat(colour, k), spell))
			return true;
	return false;
}

// A curse whose values go through GetSpellCurseEffect, so Witch has something to raise.
bool HasSpellCurseEffect(int spell) {
	switch(spell) {
		case SPL_FLAMMABILITY:
		case SPL_FROSTBITE:
		case SPL_CONDUCTIVITY:
		case SPL_VULNERABILITY:
		case SPL_DESPAIR:
		case SPL_NOX:
		case SPL_TEMPORALCHAINS:
		case SPL_ENFEEBLE:
		case SPL_PAINAMP:
		return true;
	}
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
		case DND_GEMROLE_NECROMANCER:
		return !!(f & SPLF_SUMMON);
		// Minion damage reaches the spells minions cast (the Ghast's Dark Wind); the portal deals no damage.
		case DND_GEMROLE_COMMANDER:
		return !!(f & SPLF_SUMMON) && spell != SPL_DARKPORTAL;
		// A minion's cast crits as the spell it is, so minion crit has nothing on the Ghast.
		case DND_GEMROLE_VORACIOUS:
		case DND_GEMROLE_ALPHA:
		return !!(f & SPLF_SUMMON) && spell != SPL_GHAST && spell != SPL_DARKPORTAL;
		case DND_GEMROLE_PRIEST:
		return !!(f & SPLF_HEAL);
		case DND_GEMROLE_HEXMASTER:
		return !!(f & SPLF_CURSE);
		case DND_GEMROLE_WITCH:
		return !!(f & SPLF_CURSE) && HasSpellCurseEffect(spell);
		// It lingers in the curse's area, so a single-target curse has none to linger in.
		case DND_GEMROLE_RITUALIST:
		return !!(f & SPLF_CURSE) && spell != SPL_DELIRIUM && spell != SPL_BANISH && spell != SPL_TRANSMOGRIFY;
		case DND_GEMROLE_SWIFTCASTER:
		return SpellHasRow(spell, SPELLVAL_CASTTIME);
		case DND_GEMROLE_QUICKTONGUE:
		return SpellHasRow(spell, SPELLVAL_CASTTIME) && CanSpellRepeat(spell);
		case DND_GEMROLE_MAGUS:
		case DND_GEMROLE_HAEMOCRAFTER:
		return IsGemStatActiveOnSpell(GEMSTAT_MORE_COST, spell);
		case DND_GEMROLE_BLADEDANCER:
		case DND_GEMROLE_FIGHTER:
		return !(f & (SPLF_PASSIVE | SPLF_AURA));
		// The lightning pairs take both: the matching one into their own count, the other off their first target.
		case DND_GEMROLE_CHAINER:
		case DND_GEMROLE_FORKER:
		return !!(f & SPLF_STRIKE) || spell == SPL_CHAINLIGHTNING || spell == SPL_FORKEDLIGHTNING;
		// on a crit, or off a hit (leech, Archmage's added hit, the bleed and poison rolls skip damage over time)
		case DND_GEMROLE_MARKSMAN:
		case DND_GEMROLE_WIZARD:
		case DND_GEMROLE_ASSASSIN:
		case DND_GEMROLE_POWERFUL:
		case DND_GEMROLE_ARCHMAGE:
		case DND_GEMROLE_VAMPIRE:
		case DND_GEMROLE_ARCHON:
		return SpellLandsOwnHits(spell);
		// The ailment and finishing roles also roll on a summon's minions' own hits.
		case DND_GEMROLE_SANGUINOMANCER:
		return SpellLandsOwnHits(spell) || IsMinionHitSpell(spell);
		// A poison spell's hits already poison; Venom Gyre rolls its own.
		case DND_GEMROLE_VENOMANCER:
		return (SpellLandsOwnHits(spell) && GetSpellDamageType(spell) != DND_DAMAGETYPE_POISON) || IsMinionHitSpell(spell);
		case DND_GEMROLE_PYROMANCER:
		case DND_GEMROLE_CRYOMANCER:
		case DND_GEMROLE_ELECTROMANCER:
		case DND_GEMROLE_MERCILESS:
		case DND_GEMROLE_EXECUTIONER:
		case DND_GEMROLE_DESTROYER:
		return SpellDealsOwnDamage(spell) || IsMinionHitSpell(spell);
		// Doomguard's landing stun rolls it (the user's call); no other minion does.
		case DND_GEMROLE_RESOLUTE:
		return SpellDealsOwnDamage(spell) || spell == SPL_DOOMGUARD;
	}
	// The other hit roles: damage, ailments, penetration, culling, charges.
	return SpellDealsOwnDamage(spell);
}

// Chainer and Forker never work together: the role in an earlier socket that blocks this one, or -1.
int GetGemRoleBlocker(int pnum, int spell, int slot, int role) {
	if(role != DND_GEMROLE_CHAINER && role != DND_GEMROLE_FORKER)
		return DND_GEMROLE_NONE;
	int v, other;
	for(int i = 0; i < slot; ++i) {
		if(!(v = GetSpellSocket(pnum, spell, i)))
			continue;
		other = GetSocketGemRole(v);
		if((other == DND_GEMROLE_CHAINER || other == DND_GEMROLE_FORKER) && other != role)
			return other;
	}
	return DND_GEMROLE_NONE;
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
		case DND_GEMROLE_HEXMASTER:
			AddSpellGemMod(pnum, spell, GEMSTAT_HEXMASTER, 1);
			AddGemCostPenalty(pnum, spell, g ? 0.33 : 0.5);
		break;
		// The longest linger socketed wins with its own reduction; two of them do not add up.
		case DND_GEMROLE_RITUALIST:
			if(GetSpellGemMod(pnum, spell, GEMSTAT_LINGER) < (g ? 4 : 3) * TICRATE) {
				SetSpellGemMod(pnum, spell, GEMSTAT_LINGER, (g ? 4 : 3) * TICRATE);
				SetSpellGemMod(pnum, spell, GEMSTAT_LINGER_LESS, g ? 20 : 30);
			}
		break;
		// Counts add; a greater one's loss and floor win over a normal one's.
		case DND_GEMROLE_CHAINER:
			AddSpellGemMod(pnum, spell, GEMSTAT_CHAIN, g ? 5 : 3);
			if(g) {
				SetSpellGemMod(pnum, spell, GEMSTAT_CHAIN_LOSS, 15);
				SetSpellGemMod(pnum, spell, GEMSTAT_CHAIN_FLOOR, 60);
			}
			else if(!GetSpellGemMod(pnum, spell, GEMSTAT_CHAIN_LOSS))
				SetSpellGemMod(pnum, spell, GEMSTAT_CHAIN_LOSS, 10);
			AddGemCostPenalty(pnum, spell, g ? 0.33 : 0.2);
		break;
		case DND_GEMROLE_FORKER:
			AddSpellGemMod(pnum, spell, GEMSTAT_FORK, g ? 7 : 4);
			SetSpellGemMod(pnum, spell, GEMSTAT_FORK_LESS, 10);
			AddGemCostPenalty(pnum, spell, g ? 0.33 : 0.2);
		break;
		case DND_GEMROLE_VORACIOUS:
			AddSpellGemMod(pnum, spell, GEMSTAT_MINION_CRIT, g ? 5 : 3);
			AddGemCostPenalty(pnum, spell, g ? 0.5 : 0.33);
		break;
		case DND_GEMROLE_ALPHA:
			AddSpellGemMod(pnum, spell, GEMSTAT_MINION_CRITMULT, g ? 100 : 50);
			AddGemCostPenalty(pnum, spell, g ? 0.4 : 0.25);
		break;
		case DND_GEMROLE_ARCHON:
			AddSpellGemMod(pnum, spell, GEMSTAT_MANALEECH, g ? 30 : 15);
			AddGemCostPenalty(pnum, spell, g ? 0.35 : 0.2);
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

	ACS_NamedExecuteAlways(GetSpellSummonScript(spell), 0, pnum);
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
	MenuHudSize(HUDMAX_X * sc, HUDMAX_Y * sc);

	// A socket its research has not opened: the empty cell, dimmed and never lit.
	if(locked) {
		SetFont("LDTBOX");
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA, icon_id + 1, CR_UNTRANSLATED, MenuX(((x * sc) << 16) + 0.1), MenuY(((y * sc) << 16) + 0.1), 0.0, 0.3);
		DeleteText(icon_id);
		MenuHudSize(HUDMAX_X, HUDMAX_Y);
		return;
	}

	SetFont(lit ? "LDTBOXS" : chosen ? "LDTBOXC" : v ? "LDTBOXO" : "LDTBOX");
	HudMessage(s:"A"; HUDMSG_PLAIN, icon_id + 1, CR_UNTRANSLATED, MenuX(((x * sc) << 16) + 0.1), MenuY(((y * sc) << 16) + 0.1), 0.0);

	if(v) {
		SetFont(GetItemImage(GetGemImageId(GetSocketGemSubtype(v))));
		HudMessage(s:"A"; HUDMSG_PLAIN, icon_id, CR_UNTRANSLATED,
			MenuX(((x * 2 + DND_GEMPAGE_SOCKET) * sc / 2) << 16), MenuY(((y * 2 + DND_GEMPAGE_SOCKET) * sc / 2) << 16), 0.0);
	}
	else
		DeleteText(icon_id);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

// A gem cell in the picker: the empty cell art, the gem centred on it.
void DrawGemCell(int subt, int x, int y, int icon_id, int frame_id, bool lit) {
	int sc = DND_SPELLTREE_ICONSCALE;
	MenuHudSize(HUDMAX_X * sc, HUDMAX_Y * sc);
	SetFont("SPLBACK");
	HudMessage(s:"A"; HUDMSG_PLAIN, frame_id, CR_UNTRANSLATED, MenuX(((x * sc) << 16) + 0.1), MenuY(((y * sc) << 16) + 0.1), 0.0);
	if(lit) {
		SetFont("SPLSLCT");
		HudMessage(s:"A"; HUDMSG_PLAIN, SPELLTREE_HOVER_ID, CR_UNTRANSLATED, MenuX(((x * sc) << 16) + 0.1), MenuY(((y * sc) << 16) + 0.1), 0.0);
	}

	sc = DND_GEMPAGE_GEMSC;
	MenuHudSize(HUDMAX_X * sc, HUDMAX_Y * sc);
	SetFont(GetItemImage(GetGemImageId(subt)));
	HudMessage(s:"A"; HUDMSG_PLAIN, icon_id, CR_UNTRANSLATED,
		MenuX(((x * 2 + DND_SPELLNODE) * sc / 2) << 16), MenuY(((y * 2 + DND_SPELLNODE) * sc / 2) << 16), 0.0);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

// The pocket for a gem: its name as the heading, then its colour's terms, greyed where the spell cannot use them.
// blocker is a role in an earlier socket that leaves this gem's role inert.
void DrawGemPocket(int subt, int role, int spell, int blocker = -1) {
	int c = GetGemColour(subt), k, st;
	bool g = IsGreaterGem(subt);

	PanelHeading(StrParam(s:"\c[Y5]", l:GetItemTagName(DND_ITEM_GEM, subt)), SPELLTREE_HEAD_ID + 2, DND_SPELLPANEL_HEADY);
	if(spell != -1)
		PanelHeading(StrParam(s:"\c[Y5]", l:"DND_GEM_IN", s:": \c-", l:GetSpellNameLump(spell)), SPELLTREE_HEAD_ID + 3, DND_SPELLPANEL_HEADY + 12);
	else
		DeleteText(SPELLTREE_HEAD_ID + 3);

	MenuClipRect(DND_SPELLPANEL_X - 4, DND_SPELLPANEL_Y - 4, DND_SPELLPANEL_W, DND_SPELLPANEL_H);
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
		PanelText(StrParam(l:GetGemRoleTextLump(role, g)), live && blocker == DND_GEMROLE_NONE ? "\c-" : "\c[K5]");
		if(!live)
			PanelText(StrParam(l:"DND_GEM_INACTIVE"), "\c[K5]");
		else if(blocker != DND_GEMROLE_NONE)
			PanelText(StrParam(l:"DND_GEM_BESIDE", s:" ", l:GetGemRoleNameLump(blocker), s:"."), "\c[K5]");
	}

	GetSpellScroll().panel_px = GetSpellDraw().panel_y - panel_top;
	DeleteTextRange(GetSpellDraw().panel_id, SPELLTREE_DESC_ID + DND_SPELLPANEL_MAXLINES);
	MenuClipRect(0, 0, 0, 0, 0);
	DrawSpellPocketBar();
}

// The gem page's pocket while nothing is hovered: DND_GEMPAGE_HELPTITLE, then DND_GEMPAGE_HELP split on its line breaks.
void DrawGemHelp() {
	PanelHeading(StrParam(s:"\c[Y5]", l:"DND_GEMPAGE_HELPTITLE"), SPELLTREE_HEAD_ID + 2, DND_SPELLPANEL_HEADY);
	DeleteText(SPELLTREE_HEAD_ID + 3);

	MenuClipRect(DND_SPELLPANEL_X - 4, DND_SPELLPANEL_Y - 4, DND_SPELLPANEL_W, DND_SPELLPANEL_H);
	int panel_top = DND_SPELLPANEL_Y + GetScrollBarPos(DND_SPELLBAR_PANEL) * DND_SPELLPANEL_LINEH;
	GetSpellDraw().panel_y = panel_top;
	GetSpellDraw().panel_id = SPELLTREE_DESC_ID;

	PanelTextLines(StrParam(l:"DND_GEMPAGE_HELP"), "\c-");

	GetSpellScroll().panel_px = GetSpellDraw().panel_y - panel_top;
	DeleteTextRange(GetSpellDraw().panel_id, SPELLTREE_DESC_ID + DND_SPELLPANEL_MAXLINES);
	MenuClipRect(0, 0, 0, 0, 0);
	DrawSpellPocketBar();
}

void HandleSpellGemsDraw(int pnum, int boxid, menu_pane_T module& p) {
	spell_scroll_T module& ss = GetSpellScroll();
	int i, k, n, spell, v, x, y, row, count, base;
	int hovered = -1, gem_subt = -1, gem_role = DND_GEMROLE_NONE, gem_spell = -1, gem_slot = 0, lines = 0;
	bool framed = false;

	ResetPane(p);

	// the menu frame is the backdrop now; the page name is its sub tab
	DrawChromeSpellPocket();
	SetFont("NMENUFNT");
	DeleteText(SPELLTREE_HEAD_ID);
	DeleteText(SPELLTREE_HEAD_ID + 7);

	// box one stays, parked: the frame's tabs go back now, and the boxes after it keep their numbers
	AddBoxToPane_Points(p, -1, -1, -1, -1);
	n = 1;

	// A pick on a spell that lost its sockets is dropped rather than drawn.
	if(ss.gem_pick && !GetSpellSocketCount(pnum, (ss.gem_pick - 1) / DND_SPELL_MAXSOCKETS))
		ss.gem_pick = 0;

	if(!ss.gem_pick) {
		count = GetGemEligibleCount(pnum);
		DrawChromeSpellHint(count ? "DND_GEMPAGE_PICKSOCKET" : "DND_GEMPAGE_NONE");

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
						gem_slot = k;
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
		DrawChromeSpellHint(count ? "DND_GEMPAGE_PICKGEM" : GetInventoryGemCount(pnum) ? "DND_GEMPAGE_NOFIT" : "DND_GEMPAGE_NOGEMS");

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
					gem_slot = k;
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
				gem_slot = pick_slot;
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
		DrawGemPocket(gem_subt, gem_role, gem_spell, GetGemRoleBlocker(pnum, gem_spell, gem_slot, gem_role));
		return;
	}

	if(!ss.pinned && ss.shown_spell != hovered) {
		ss.shown_spell = hovered;
		SetScrollBarPos(DND_SPELLBAR_PANEL, 0);
	}
	if(ss.shown_spell == -1)
		DrawGemHelp();
	else
		DrawSpellPocket(pnum);
}

// CLIENTSIDE, from the menu loop beside HandleSpellHotbarPick, for the same reason: the view decides what a box
// means, and the view is clientside. Left on a socket picks it, right on a filled one takes its gem out.
bool HandleSpellGemsPick(int pnum, int boxid, int input) {
	if(input != DND_MENUINPUT_LCLICK && input != DND_MENUINPUT_RCLICK && input != DND_MENUINPUT_JUMPCLICK)
		return false;

	spell_scroll_T module& ss = GetSpellScroll();

	// Outside every box closes the gem view; box one is parked (the old back arrow).
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
