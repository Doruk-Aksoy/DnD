#ifndef DND_SPELLHUD_IN
#define DND_SPELLHUD_IN

#include "DnD_SpellCast.h"

// The hotbar draws clientside, and SpellPlayerData is a global -- each side holds its own copy, so
// the client never sees what the server allocated. Two ammo items per slot carry across instead.
//
// P_HotbarN   : spell id + 1, zero for an empty slot.
// P_HotbarCdN : (end_tic << DND_HOTBARCD_SHIFT) | total_tics. The client works the percentage out
//               from its own Timer() rather than being told it every tic.
#define DND_HOTBARCD_SHIFT 11
#define DND_HOTBARCD_MASK ((1 << DND_HOTBARCD_SHIFT) - 1)

// The art is 64x64 and renders one unit per source pixel, so it is drawn in a 3x canvas to reach
// an effective 21 units -- the same trick and the same size as a tree node. At the old 320 wide hud
// an icon was a fifth of the screen.
#define DND_HOTBAR_SCALE 3
#define DND_HOTBAR_ICON 21
#define DND_HOTBAR_GAP 5

// Two anchors, neither of them a guess about the player's setup.
//
// ACROSS: GetHudRight, not the hud WIDTH. The hud space is 3:2, so on a 16:9 screen it is
// pillarboxed and its own 480 is nowhere near the monitor's edge -- GetHudRight(480) is 561 there.
// That is what the whole ScreenResOffsets machinery is for. It is linear in the width, so
// GetHudRight(HUDMAX_X * n) == n * GetHudRight(HUDMAX_X) and the scaled canvas just multiplies.
//
// DOWN: a measured offset up from the bottom of the screen, and statusbar = 0. SBARINFO declares
// `height 0`, so the status bar reserves no space at all -- statusbar = 1 bought nothing but the full
// screen, and the whole stack sat ON the ammo panel. That panel is H_BGRIT, a 239x80 image drawn at
// -244,-85 in SBARINFO's own 640x400 fullscreenoffsets space, and that space scales WITH the screen.
// So the panel's top edge is a fixed FRACTION of the screen height rather than a fixed pixel count,
// which is exactly what lets a constant here hold at any resolution. DrawDashCharges anchors the same
// way.
//
// That scale works out to exactly 2, putting the panel's top edge at screen y 910 of 1080 and its
// left edge at 1432 -- both confirmed by sampling a capture for the panel's cool grey against the warm
// level geometry. Sampling its CONTENT rather than its background is what produced an earlier, too low
// figure of 38: the "50" and the red ammo bar start well below the panel itself.
#define DND_AMMOPANEL_TOP 50

// The panel is 160 units wide here and clears the right edge by about 3, which is what the mana bar
// is drawn to match -- see DND_MANABAR_W.
#define DND_HOTBAR_MARGIN 3
#define DND_HOTBAR_GAPY 3		// between the slot numbers and the mana bar under them
#define DND_HOTBAR_LABELH 10	// the slot number, which hangs below the icon

// The mana bar's art is 299x30 and renders one unit per source pixel, so at 1x it would be 299 units
// -- nearly twice the ammo panel's width, running off the right of the screen. It is drawn in a 2x
// canvas instead, which lands it at 149x15: about the width of the plain bar it replaced, so it still
// sits over the panel without overhanging it. Declared up here because the hotbar stacks on it and
// needs the height.
#define DND_MANABAR_SCALE 2
#define DND_MANABAR_PXW 299
#define DND_MANABAR_PXH 30
#define DND_MANABAR_W (DND_MANABAR_PXW / DND_MANABAR_SCALE)
#define DND_MANABAR_H (DND_MANABAR_PXH / DND_MANABAR_SCALE)

// Clearance over the panel. H_BGRIT is opaque from its very first row, so DND_AMMOPANEL_TOP lands the
// bar exactly on the panel's edge with nothing between them -- this is what separates the two.
#define DND_MANABAR_GAP 6

str GetHotbarSlotItem(int slot) {
	return StrParam(s:"P_Hotbar", d:slot + 1);
}

str GetHotbarCdItem(int slot) {
	return StrParam(s:"P_HotbarCd", d:slot + 1);
}

// Server side. Called when a binding changes and when a cast starts, never on a timer.
void SyncHotbarSlot(int pnum, int slot) {
	int tid = pnum + P_TIDSTART;
	int spell = GetHotbarSpell(pnum, slot);

	SetActorInventory(tid, GetHotbarSlotItem(slot), spell + 1);

	if(spell < 0) {
		SetActorInventory(tid, GetHotbarCdItem(slot), 0);
		return;
	}

	int total = Min(GetSpellCooldownTics(pnum, spell), DND_HOTBARCD_MASK);
	SetActorInventory(tid, GetHotbarCdItem(slot), (GetSpellCooldowns().at[pnum][spell] << DND_HOTBARCD_SHIFT) | total);
}

// Server side, bracketing the cast delay. P_CastProgress is packed exactly as P_HotbarCd is, so the
// client reads both with the same arithmetic. Returns the word written, for EndSpellCastBar.
int StartSpellCastBar(int pnum, int spell, int tics) {
	int tid = pnum + P_TIDSTART;
	int packed = ((Timer() + tics) << DND_HOTBARCD_SHIFT) | Min(tics, DND_HOTBARCD_MASK);

	SetActorInventory(tid, "P_CastSpell", spell + 1);
	SetActorInventory(tid, "P_CastProgress", packed);
	return packed;
}

// Only clears the bar if the cast that started it is still the one on it. Casts can overlap -- the
// effect script is an ExecuteAlways -- and without this the first to finish wiped the second's bar.
void EndSpellCastBar(int pnum, int packed) {
	int tid = pnum + P_TIDSTART;
	if(CheckActorInventory(tid, "P_CastProgress") != packed)
		return;

	SetActorInventory(tid, "P_CastProgress", 0);
	SetActorInventory(tid, "P_CastSpell", 0);
}

void SyncHotbarAll(int pnum) {
	for(int i = 0; i < MAX_HOTBAR_SLOTS; ++i)
		SyncHotbarSlot(pnum, i);
}

// Every slot holding this spell, because the same spell may sit on several binds.
void SyncHotbarForSpell(int pnum, int spell) {
	for(int i = 0; i < MAX_HOTBAR_SLOTS; ++i)
		if(GetHotbarSpell(pnum, i) == spell)
			SyncHotbarSlot(pnum, i);
}

// ---- clientside ------------------------------------------------------------------------------

// 0 when ready, up to 100 the instant it starts. Read off the packed word so the client needs no
// per tic traffic to animate the overlay.
int GetHotbarCooldownPercent(int slot) {
	int packed = CheckInventory(GetHotbarCdItem(slot));
	if(!packed)
		return 0;

	int total = packed & DND_HOTBARCD_MASK;
	if(total <= 0)
		return 0;

	int left = (packed >> DND_HOTBARCD_SHIFT) - Timer();
	if(left <= 0)
		return 0;

	return Min(100, left * 100 / total);
}

// Flush right and growing leftward: slot 0 is leftmost, so gaining a slot shifts the rest left and
// never renumbers a bind the player already learned.
int GetHotbarSlotX(int slot, int count) {
	return GetHudRight(HUDMAX_X) - DND_HOTBAR_MARGIN -
		(count - slot) * (DND_HOTBAR_ICON + DND_HOTBAR_GAP);
}

// Bottom to top the stack reads: ammo panel, mana bar, slot numbers, icons.
int GetHotbarSlotY() {
	return HUDMAX_Y - DND_AMMOPANEL_TOP - DND_MANABAR_GAP - DND_MANABAR_H -
		DND_HOTBAR_GAPY - DND_HOTBAR_LABELH - DND_HOTBAR_ICON;
}

// Drawn from the shared per player HUD loop rather than a script of its own -- see
// "DnD Player HUD". The gate (alive, not in the menu) belongs to that loop, so this just
// draws.
void DrawSpellHotbar(int pnum) {
	int i, count = GetHotbarSlotCount(pnum), spell, x, pct;

	int sc = DND_HOTBAR_SCALE;

	for(i = 0; i < count; ++i) {
		spell = CheckInventory(GetHotbarSlotItem(i)) - 1;
		x = GetHotbarSlotX(i, count);

		// An unassigned slot draws NOTHING. It used to draw SPLBOX, a lump that does not exist,
		// so SetFont fell back and the slot came out as a literal "A".
		if(spell < 0) {
			DeleteText(HOTBAR_SLOT_ID + i);
			DeleteText(HOTBAR_ICON_ID + i);
			DeleteText(HOTBAR_CD_ID + i);
			continue;
		}

		SetHudSize(HUDMAX_X * sc, HUDMAX_Y * sc, 0);
		SetFont(GetSpellIcon(spell, false));
		HudMessage(s:"A"; HUDMSG_PLAIN, HOTBAR_ICON_ID + i, CR_UNTRANSLATED,
			((x * sc) << 16) + 0.1, ((GetHotbarSlotY() * sc) << 16) + 0.1, 0.1);

		// The black square is clipped to the share of the cooldown still to run and anchored at the
		// BOTTOM of the icon, so the shade drains downward -- its top edge walks down the icon as the
		// cooldown ticks and the art is revealed from the top. Anchoring at the top instead made it
		// recede upward, which reads as the cooldown running backwards. The clip is in the same canvas
		// as the draw, so it scales with it.
		pct = GetHotbarCooldownPercent(i);
		if(pct) {
			int cdh = DND_HOTBAR_ICON * sc * pct / 100;
			SetHudClipRect(x * sc, GetHotbarSlotY() * sc + DND_HOTBAR_ICON * sc - cdh,
				DND_HOTBAR_ICON * sc, cdh);
			SetFont("SPLCDBLK");
			HudMessage(s:"A"; HUDMSG_PLAIN, HOTBAR_CD_ID + i, CR_UNTRANSLATED,
				((x * sc) << 16) + 0.1, ((GetHotbarSlotY() * sc) << 16) + 0.1, 0.1);
			SetHudClipRect(0, 0, 0, 0);
		}
		else
			DeleteText(HOTBAR_CD_ID + i);

		// Which key casts it, under the icon.
		SetHudSize(HUDMAX_X, HUDMAX_Y, 0);
		SetFont("SMALLFONT");
		HudMessage(s:"\c[Y5]", d:i + 1; HUDMSG_PLAIN, HOTBAR_SLOT_ID + i, CR_WHITE,
			((x + DND_HOTBAR_ICON / 2) << 16) + 0.4,
			((GetHotbarSlotY() + DND_HOTBAR_ICON + 1) << 16) + 0.1, 0.1);
	}

}


// ---- mana bar --------------------------------------------------------------------------------

int GetManaBarX() {
	return GetHudRight(HUDMAX_X) - DND_HOTBAR_MARGIN - DND_MANABAR_W;
}

// Riding just above the top of the ammo panel.
int GetManaBarY() {
	return HUDMAX_Y - DND_AMMOPANEL_TOP - DND_MANABAR_GAP - DND_MANABAR_H;
}

// Clientside. Three numbers cross as ammo amounts because PlayerModData does not: current mana, the
// true cap, and the part of it no aura is holding. The bar spans the TRUE cap end to end and draws the
// reserved share as a dead tail on the right, so switching an aura on visibly eats the pool instead of
// silently rescaling the bar under the player.
void DrawManaBar(int pnum) {
	int cap = CheckInventory("P_ManaCap");
	if(cap <= 0)
		return;

	int usable = Min(CheckInventory("ManaVisual"), cap);
	int cur = Min(CheckInventory("Mana"), usable);
	int x = GetManaBarX(), y = GetManaBarY(), sc = DND_MANABAR_SCALE;

	// Laid out in unscaled units above, then multiplied into the canvas the art is drawn in -- the
	// same trick the hotbar icons use, and the clip has to live in that canvas too or it would cut at
	// half the intended place.
	SetHudSize(HUDMAX_X * sc, HUDMAX_Y * sc, 0);

	SetFont("SPLMANAB");
	HudMessage(s:"A"; HUDMSG_PLAIN, MANABAR_BACK_ID, CR_UNTRANSLATED,
		((x * sc) << 16) + 0.1, ((y * sc) << 16) + 0.1, 0.0);

	// Every segment draws the whole graphic at the same spot and lets the clip decide what shows, so
	// nothing has to be redrawn at a different size as the numbers move.
	int w = DND_MANABAR_W * (cap - usable) / cap;
	if(w > 0) {
		SetHudClipRect((x + DND_MANABAR_W - w) * sc, y * sc, w * sc, DND_MANABAR_H * sc);
		SetFont("SPLMANAR");
		HudMessage(s:"A"; HUDMSG_PLAIN, MANABAR_RESERVED_ID, CR_UNTRANSLATED,
			((x * sc) << 16) + 0.1, ((y * sc) << 16) + 0.1, 0.0);
		SetHudClipRect(0, 0, 0, 0);
	}
	else
		DeleteText(MANABAR_RESERVED_ID);

	w = DND_MANABAR_W * cur / cap;
	if(w > 0) {
		SetHudClipRect(x * sc, y * sc, w * sc, DND_MANABAR_H * sc);
		SetFont("SPLMANAF");
		HudMessage(s:"A"; HUDMSG_PLAIN, MANABAR_FILL_ID, CR_UNTRANSLATED,
			((x * sc) << 16) + 0.1, ((y * sc) << 16) + 0.1, 0.0);
		SetHudClipRect(0, 0, 0, 0);
	}
	else
		DeleteText(MANABAR_FILL_ID);

	// The readout is the SPENDABLE pair, not the true cap: the reservation is already on screen as the
	// tail, and a player checking whether they can pay for a cast wants the number they can reach.
	// Back in the unscaled canvas, so the font is not doubled along with the art.
	SetHudSize(HUDMAX_X, HUDMAX_Y, 0);
	SetFont("SMALLFONT");
	// Both fractions are .4: .1 on the y is TOP alignment, which hung the whole readout below the bar's
	// midline instead of centring it on the bar.
	HudMessage(d:cur, s:"\c[D4]/\c-", d:usable; HUDMSG_PLAIN, MANABAR_TEXT_ID, CR_WHITE,
		((x + DND_MANABAR_W / 2) << 16) + 0.4, ((y + DND_MANABAR_H / 2) << 16) + 0.4, 0.0);
}

// ---- cast bar --------------------------------------------------------------------------------

// Bottom centre. Centred off the real screen edges rather than the hud width, since the hud space is
// pillarboxed on a widescreen and its own midpoint is not the monitor's.
//
// As low as it can go without a clash. The bottom centre is already spoken for: "DnD Stamina Bar
// Draw" puts its bar across the bottom 68 pixels and its parry readout at about 962 of 1080, so this
// bar's lower edge stops just above that. Everything else down there is off to one side -- the EXP
// row ends well left of it and the metronome glyph sits right of centre, over toward the ammo panel.
// The mana bar and hotbar are bottom RIGHT and never reach this far in.
#define DND_CASTBAR_W 160
#define DND_CASTBAR_H 12
#define DND_CASTBAR_BOTTOM 51
#define DND_CASTBAR_LABEL 12

int GetCastBarX() {
	return (GetHudLeft(HUDMAX_X) + GetHudRight(HUDMAX_X) - DND_CASTBAR_W) / 2;
}

// -1 when nothing is being cast, else how far ELAPSED the cast is, 0 to 100. Same packed word and the
// same reasoning as GetHotbarCooldownPercent: the client animates it off its own Timer().
int GetSpellCastPercent() {
	int packed = CheckInventory("P_CastProgress");
	if(!packed)
		return -1;

	int total = packed & DND_HOTBARCD_MASK;
	if(total <= 0)
		return -1;

	int left = (packed >> DND_HOTBARCD_SHIFT) - Timer();
	if(left <= 0)
		return -1;

	return Max(0, Min(100, 100 - left * 100 / total));
}

void ClearSpellCastBar() {
	DeleteTextRange(CASTBAR_TEXT_ID, CASTBAR_BACK_ID);
}

// Clientside. Starts empty and fills to the right edge as the cast runs out, with the spell being cast
// named over it.
void DrawSpellCastBar(int pnum) {
	int pct = GetSpellCastPercent();
	if(pct < 0) {
		ClearSpellCastBar();
		return;
	}

	int x = GetCastBarX(), y = HUDMAX_Y - DND_CASTBAR_BOTTOM;
	int spell = CheckInventory("P_CastSpell") - 1;

	SetHudSize(HUDMAX_X, HUDMAX_Y, 0);

	SetFont("SPLCSTB");
	HudMessage(s:"A"; HUDMSG_PLAIN, CASTBAR_BACK_ID, CR_UNTRANSLATED,
		(x << 16) + 0.1, (y << 16) + 0.1, 0.0);

	int w = DND_CASTBAR_W * pct / 100;
	if(w > 0) {
		SetHudClipRect(x, y, w, DND_CASTBAR_H);
		SetFont("SPLCSTF");
		HudMessage(s:"A"; HUDMSG_PLAIN, CASTBAR_FILL_ID, CR_UNTRANSLATED,
			(x << 16) + 0.1, (y << 16) + 0.1, 0.0);
		SetHudClipRect(0, 0, 0, 0);
	}
	else
		DeleteText(CASTBAR_FILL_ID);

	if(spell >= 0 && spell < MAX_SPELL_IDS) {
		SetFont("SMALLFONT");
		HudMessage(s:"\c[Y5]", l:GetSpellNameLump(spell); HUDMSG_PLAIN, CASTBAR_TEXT_ID, CR_WHITE,
			((x + DND_CASTBAR_W / 2) << 16) + 0.4, ((y - DND_CASTBAR_LABEL) << 16) + 0.1, 0.0);
	}
	else
		DeleteText(CASTBAR_TEXT_ID);
}

// Everything the spell HUD draws, in one sweep: the bars sit just under the hotbar's three runs and
// nothing else lives in that stretch of ids.
void ClearSpellHotbar() {
	DeleteTextRange(MANABAR_TEXT_ID, HOTBAR_SLOT_ID + MAX_HOTBAR_SLOTS);
}

#endif
