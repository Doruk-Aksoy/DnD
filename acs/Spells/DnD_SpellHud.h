#ifndef DND_SPELLHUD_IN
#define DND_SPELLHUD_IN

#include "DnD_SpellCast.h"

// The hotbar draws clientside, and SpellPlayerData is a global -- each side holds its own copy, so
// the client never sees what the server allocated. Two ammo items per slot carry across instead.
//
// P_HotbarN    : spell id + 1, zero for an empty slot.
// P_HotbarCdN  : the end tic, whole; P_HotbarTotN : PackHotbarTotal(total_tics). The client works the percentage
//                out from its own Timer(). Sharing one word, the end tic overflowed ~8.3 hours into a map.
#define DND_HOTBARCD_MASK 0x7FF         // a packed total's 11 bits
#define DND_HOTBARCD_LONG 1024          // totals from here on are kept in STEP tic units, up to 7.8 minutes
#define DND_HOTBARCD_STEP 16

int PackHotbarTotal(int tics) {
	if(tics < DND_HOTBARCD_LONG)
		return tics;
	return DND_HOTBARCD_LONG | Min((tics + DND_HOTBARCD_STEP - 1) / DND_HOTBARCD_STEP, DND_HOTBARCD_LONG - 1);
}

int UnpackHotbarTotal(int packed) {
	int f = packed & DND_HOTBARCD_MASK;
	if(f & DND_HOTBARCD_LONG)
		return (f & (DND_HOTBARCD_LONG - 1)) * DND_HOTBARCD_STEP;
	return f;
}

// The art is 64x64 and renders one unit per source pixel, so it is drawn in a 3x canvas to reach
// an effective 21 units -- the same trick and the same size as a tree node. At the old 320 wide hud
// an icon was a fifth of the screen.
#define DND_HOTBAR_SCALE 3
#define DND_HOTBAR_ICON 21
#define DND_HOTBAR_GAP 5
#define DND_HOTBAR_GLINTFRAMES 10       // SPLGLT01-10, a tic each

// Client side: what each slot showed last tic, so the tic a spell comes back can be caught.
typedef struct {
	int spell[MAX_HOTBAR_SLOTS], ready[MAX_HOTBAR_SLOTS], at[MAX_HOTBAR_SLOTS];
} hotbar_glint_T;

// A static behind an accessor, not a file scope array: those are stack storage. See dnd-menu-sync.md.
hotbar_glint_T module& GetHotbarGlint() {
	static hotbar_glint_T s;
	return s;
}

// Client side: what the spell HUD has on screen, so a layer is deleted once when it goes and an unchanged bar is not resent.
#define DND_HUDL_SLOT 1                 // the icon and key label
#define DND_HUDL_CD 2
#define DND_HUDL_CD2 4
#define DND_HUDL_GLINT 8
#define DND_HUDL_CHG 16
#define DND_HUDL_STACK 32

typedef struct {
	int shown[MAX_HOTBAR_SLOTS];
	bool mana_drawn, cast_drawn;
	int mana_cap, mana_usable, mana_cur, mana_fill, mana_ghost, mana_a, mana_x, mana_y, mana_w, mana_h;
} spell_hud_drawn_T;

spell_hud_drawn_T module& GetSpellHudDrawn() {
	static spell_hud_drawn_T s;
	return s;
}

void HideHotbarLayer(int slot, int layer, int id) {
	if(GetSpellHudDrawn().shown[slot] & layer) {
		DeleteText(id + slot);
		GetSpellHudDrawn().shown[slot] &= ~layer;
	}
}

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

// The 299x30 art is drawn in a 27/16 canvas, 177x18 units here: about 18% larger than the old 2x one.
// Up here because the hotbar stacks on its height.
#define DND_MANABAR_CNUM 27
#define DND_MANABAR_CDEN 16
#define DND_MANABAR_PXW 299
#define DND_MANABAR_PXH 30
#define DND_MANABAR_W (DND_MANABAR_PXW * DND_MANABAR_CDEN / DND_MANABAR_CNUM)
#define DND_MANABAR_H ((DND_MANABAR_PXH * DND_MANABAR_CDEN + DND_MANABAR_CNUM - 1) / DND_MANABAR_CNUM)

// Clearance over the panel. H_BGRIT is opaque from its very first row, so DND_AMMOPANEL_TOP lands the
// bar exactly on the panel's edge with nothing between them -- this is what separates the two.
#define DND_MANABAR_GAP 6

str GetHotbarSlotItem(int slot) {
	return StrParam(s:"P_Hotbar", d:slot + 1);
}

str GetHotbarCdItem(int slot) {
	return StrParam(s:"P_HotbarCd", d:slot + 1);
}

// The SECOND charge's cooldown word. Named CdB rather than Cd2 because P_HotbarCd2 is already
// slot 2's.
str GetHotbarCdBItem(int slot) {
	return StrParam(s:"P_HotbarCdB", d:slot + 1);
}

// How many charges the spell HAS. Without it the client cannot tell a ready second charge from a
// spell that never had one -- both read as a zero word.
str GetHotbarChgItem(int slot) {
	return StrParam(s:"P_HotbarChg", d:slot + 1);
}

// The slot's packed cooldown total, shared by both charge words.
str GetHotbarTotItem(int slot) {
	return StrParam(s:"P_HotbarTot", d:slot + 1);
}

// Server side. Called when a binding changes and when a cast starts, never on a timer.
void SyncHotbarSlot(int pnum, int slot) {
	int tid = pnum + P_TIDSTART;
	int spell = GetHotbarSpell(pnum, slot);

	SetActorInventory(tid, GetHotbarSlotItem(slot), spell + 1);

	// Cleared TOGETHER. A slot that loses its spell must drop the charge count too, or the hotbar
	// keeps drawing a number over an empty slot.
	if(spell < 0) {
		SetActorInventory(tid, GetHotbarCdItem(slot), 0);
		SetActorInventory(tid, GetHotbarCdBItem(slot), 0);
		SetActorInventory(tid, GetHotbarChgItem(slot), 0);
		SetActorInventory(tid, GetHotbarTotItem(slot), 0);
		return;
	}

	auto cd = GetSpellCooldowns();
	int charges = GetSpellMaxCharges(pnum, spell);

	SetActorInventory(tid, GetHotbarCdItem(slot), cd.at[pnum][spell]);
	SetActorInventory(tid, GetHotbarCdBItem(slot), charges > 1 ? cd.atb[pnum][spell] : 0);
	SetActorInventory(tid, GetHotbarChgItem(slot), charges);
	SetActorInventory(tid, GetHotbarTotItem(slot), PackHotbarTotal(GetSpellCooldownTics(pnum, spell)));
}

#define DND_CASTBAR_DRAIN 0x8000        // on P_CastSpell: the bar shows time left, for a channel's window
#define DND_CASTBAR_SPELLMASK 0x7FFF
#define DND_CASTBAR_TOTALSHIFT 16       // P_CastSpell carries the packed total above the spell and DRAIN

// P_CastSpell's word: the spell, DRAIN and the packed total. P_CastProgress holds the end tic, whole.
int MakeSpellCastWord(int spell, int tics, bool drain) {
	return (spell + 1) | (drain ? DND_CASTBAR_DRAIN : 0) | (PackHotbarTotal(tics) << DND_CASTBAR_TOTALSHIFT);
}

// Server side, bracketing the cast delay. Returns the end tic written, for EndSpellCastBar.
int StartSpellCastBar(int pnum, int spell, int tics, bool drain = false) {
	int tid = pnum + P_TIDSTART;
	int end = Timer() + tics;

	SetActorInventory(tid, "P_CastSpell", MakeSpellCastWord(spell, tics, drain));
	SetActorInventory(tid, "P_CastProgress", end);
	return end;
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

// Server side, once a second while the player is in: the clock the client reads end tics against.
Script "DnD Server Clock" (int pnum) {
	while(PlayerInGame(pnum)) {
		SetActorInventory(pnum + P_TIDSTART, "P_ServerTime", Timer());
		Delay(const:TICRATE);
	}
}

// ---- clientside ------------------------------------------------------------------------------

typedef struct {
	int val, at;
} server_clock_T;

server_clock_T module& GetServerClock() {
	static server_clock_T s;
	return s;
}

// The server's Timer() as this client knows it: its last push plus the client's tics since. Timer() offline.
int ServerTimer() {
	int v = CheckInventory("P_ServerTime");
	if(GameType() == GAME_SINGLE_PLAYER || !v)
		return Timer();
	if(v != GetServerClock().val) {
		GetServerClock().val = v;
		GetServerClock().at = Timer();
	}
	return v + Timer() - GetServerClock().at;
}

// Silencer: the share of the silence still to run, 0 to 100, off its end tic and the server's clock.
int GetSilencePercent() {
	int left = CheckInventory("P_SilenceEnd") - ServerTimer();
	if(left <= 0)
		return 0;
	return Min(100, left * 100 / DND_SILENCER_TICS);
}

// 0 when ready, up to 100 the instant it starts. Read off the packed word so the client needs no
// per tic traffic to animate the overlay.
int GetHotbarWordPercent(int slot, str word) {
	int end = CheckInventory(word);
	if(!end)
		return 0;

	int total = UnpackHotbarTotal(CheckInventory(GetHotbarTotItem(slot)));
	if(total <= 0)
		return 0;

	int left = end - ServerTimer();
	if(left <= 0)
		return 0;

	return Min(100, left * 100 / total);
}

// A silence shades every slot like a cooldown of its own, so the slot reads as ready only once both are over.
int GetHotbarCooldownPercent(int slot) {
	return Max(GetHotbarWordPercent(slot, GetHotbarCdItem(slot)), GetSilencePercent());
}

// The second charge, on the same arithmetic. Zero when the spell holds only one.
int GetHotbarCooldownBPercent(int slot) {
	if(CheckInventory(GetHotbarChgItem(slot)) < 2)
		return 0;
	return Max(GetHotbarWordPercent(slot, GetHotbarCdBItem(slot)), GetSilencePercent());
}

// Charges still castable. Worked out from the same two words the squares are drawn from, so the
// number and the shading can never disagree.
int GetHotbarChargesReady(int slot) {
	int charges = CheckInventory(GetHotbarChgItem(slot));
	if(charges < 2)
		return 0;

	return (GetHotbarCooldownPercent(slot) ? 0 : 1) + (GetHotbarCooldownBPercent(slot) ? 0 : 1);
}

// ---- stack scale -----------------------------------------------------------------------------

// The hotbar and mana bar scale like the fullscreen status bar under them: by hud_scale's clean factor, else 1.
// A unit is 27/16 screen px per step, so 1080p with hud_scale on is the 480x320 the layout was drawn in.
#define DND_HUDSTACK_NUM 27
#define DND_HUDSTACK_DEN 16

// This frame's canvas: its size, and the screen's right and bottom edges in it; key holds the inputs it came from.
typedef struct {
	int w, h, right, bottom;
	int key_w, key_h, key_cvars;
} hud_stack_T;

hud_stack_T module& GetHudStack() {
	static hud_stack_T s;
	return s;
}

// CheckRatio's pick (0 4:3, 1 16:9, 2 16:10, 3 17:10, 4 5:4, 6 21:9), with vid_aspect and vid_nowidescreen.
int GetEngineScreenRatio(int w, int h) {
	if(GetCVar("vid_nowidescreen"))
		return (GetCVar("vid_tft") && h * 5 / 4 == w) ? 4 : 0;
	switch(GetCVar("vid_aspect")) {
		case 1: return 1;
		case 2: return 2;
		case 3: return 0;
		case 4: return 4;
		case 5: return 3;
		case 6: return 6;
	}

	// nearest aspect, ties going to the earlier one as in the engine's list
	int a = (w << 16) / h, best = 6, bestd = abs(a - 152917), d;
	if((d = abs(a - 116508)) < bestd) { best = 1; bestd = d; }
	if((d = abs(a - 111411)) < bestd) { best = 3; bestd = d; }
	if((d = abs(a - 104858)) < bestd) { best = 2; bestd = d; }
	if((d = abs(a - 87381)) < bestd) { best = 0; bestd = d; }
	if((d = abs(a - 81920)) < bestd)
		best = 4;
	return best;
}

// BaseRatioSizes' base width and clean multiplier columns.
int GetRatioBaseWidth(int r) {
	switch(r) {
		case 1: return 1280;
		case 2: return 1152;
		case 3: return 1224;
		case 6: return 1707;
	}
	return 960;
}

int GetRatioCleanMul(int r) {
	switch(r) {
		case 1: return 36;
		case 2: return 40;
		case 3: return 37;
		case 4: return 45;
		case 6: return 27;
	}
	return 48;
}

// V_CalcCleanFacs for SBARINFO's 640x400; the y factor, which sets the status bar's height.
int GetHudCleanFactor(int w, int h, int r) {
	int cw = w, ch = h;
	if(r == 4)
		ch = h * GetRatioCleanMul(r) / 48;
	else
		cw = w * GetRatioCleanMul(r) / 48;

	int cx = Max(cw / 640, 1), cy = Max(ch / 400, 1), cx2 = Max(w / 640, 1), cy2 = Max(h / 400, 1);
	if(abs(cx - cy) > abs(cx2 - cy2)) {
		cx = cx2;
		cy = cy2;
	}
	if(cx > 1 && cy > 1 && cx != cy)
		return Min(cx, cy);
	return cy;
}

// Clientside, once per draw. VirtualToRealCoords maps a canvas onto a 4:3 box: 21:9 uses the 16:10 row, 5:4 letterboxes.
void UpdateHudStack(void) {
	int w = GetScreenWidth(), h = GetScreenHeight();
	if(w <= 0 || h <= 0) {
		w = 640;
		h = 480;
	}

	// the canvas only moves with the screen size or these four cvars (the + 1 keeps a first run from matching zeros)
	int cvars = 1 + (!!GetCVar("vid_nowidescreen") | (!!GetCVar("vid_tft") << 1) | (!!GetCVar("hud_scale") << 2) | ((GetCVar("vid_aspect") & 0xFF) << 3));
	hud_stack_T module& st = GetHudStack();
	if(st.key_w == w && st.key_h == h && st.key_cvars == cvars)
		return;
	st.key_w = w;
	st.key_h = h;
	st.key_cvars = cvars;

	int r = GetEngineScreenRatio(w, h);
	int s = GetCVar("hud_scale") ? GetHudCleanFactor(w, h, r) : 1;
	if(r == 6)
		r = 2;
	int bw = (r == 0 || r == 4) ? w : w * 960 / GetRatioBaseWidth(r);
	int bh = r == 4 ? h * 600 / 640 : h;

	GetHudStack().h = bh * DND_HUDSTACK_DEN / (DND_HUDSTACK_NUM * s);
	GetHudStack().w = GetHudStack().h * HUDMAX_X / HUDMAX_Y;
	GetHudStack().right = GetHudStack().w / 2 + w * GetHudStack().w / (2 * bw);
	GetHudStack().bottom = GetHudStack().h / 2 + h * GetHudStack().h / (2 * bh);
}

// Flush right and growing leftward: slot 0 is leftmost, so gaining a slot shifts the rest left and
// never renumbers a bind the player already learned.
int GetHotbarSlotX(int slot, int count) {
	return GetHudStack().right - DND_HOTBAR_MARGIN -
		(count - slot) * (DND_HOTBAR_ICON + DND_HOTBAR_GAP);
}

// Bottom to top the stack reads: ammo panel, mana bar, slot numbers, icons.
int GetHotbarSlotY() {
	return GetHudStack().bottom - DND_AMMOPANEL_TOP - DND_MANABAR_GAP - DND_MANABAR_H -
		DND_HOTBAR_GAPY - DND_HOTBAR_LABELH - DND_HOTBAR_ICON;
}

// Crackling Lance's intensity, Overcharge's charges, Surgecast's seconds left, the Souls Soulstorm feeds on.
int GetHotbarBadge(int spell) {
	switch(spell) {
		case SPL_SOULSTORM: return CheckInventory("Souls");
		case SPL_CRACKLINGLANCE: return CheckInventory("P_LanceStacks");
		case SPL_OVERCHARGE: return CheckInventory("P_OverchargeCharges");
		case SPL_SURGECAST: return CheckInventory("P_SurgecastTime");
		case SPL_BALEFULINFUSION: return CheckInventory("P_BalefulTime");
		case SPL_ROCKARMOR: return CheckInventory("P_RockArmor");
		case SPL_TELEKINESIS: return CheckInventory("P_TelekinesisTime");
	}
	return 0;
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
			if(GetSpellHudDrawn().shown[i]) {
				DeleteText(HOTBAR_SLOT_ID + i);
				DeleteText(HOTBAR_ICON_ID + i);
				DeleteText(HOTBAR_CD_ID + i);
				DeleteText(HOTBAR_CD2_ID + i);
				DeleteText(HOTBAR_CHG_ID + i);
				DeleteText(HOTBAR_GLINT_ID + i);
				DeleteText(HOTBAR_STACK_ID + i);
				GetSpellHudDrawn().shown[i] = 0;
			}
			GetHotbarGlint().spell[i] = 0;
			continue;
		}
		GetSpellHudDrawn().shown[i] |= DND_HUDL_SLOT;

		SetHudSize(GetHudStack().w * sc, GetHudStack().h * sc, 0);
		SetFont(GetSpellIcon(spell, false));
		HudMessage(s:"A"; HUDMSG_PLAIN, HOTBAR_ICON_ID + i, CR_UNTRANSLATED,
			((x * sc) << 16) + 0.1, ((GetHotbarSlotY() * sc) << 16) + 0.1, 0.1);

		// The black square is clipped to the share of the cooldown still to run and anchored at the
		// BOTTOM of the icon, so the shade drains downward -- its top edge walks down the icon as the
		// cooldown ticks and the art is revealed from the top. Anchoring at the top instead made it
		// recede upward, which reads as the cooldown running backwards. The clip is in the same canvas
		// as the draw, so it scales with it.
		int cdh;

		pct = GetHotbarCooldownPercent(i);
		if(pct) {
			cdh = DND_HOTBAR_ICON * sc * pct / 100;
			SetHudClipRect(x * sc, GetHotbarSlotY() * sc + DND_HOTBAR_ICON * sc - cdh,
				DND_HOTBAR_ICON * sc, cdh);
			SetFont("SPLCDBLK");
			HudMessage(s:"A"; HUDMSG_PLAIN, HOTBAR_CD_ID + i, CR_UNTRANSLATED,
				((x * sc) << 16) + 0.1, ((GetHotbarSlotY() * sc) << 16) + 0.1, 0.1);
			SetHudClipRect(0, 0, 0, 0);
			GetSpellHudDrawn().shown[i] |= DND_HUDL_CD;
		}
		else
			HideHotbarLayer(i, DND_HUDL_CD, HOTBAR_CD_ID);

		// The second charge, as a square of its own laid over the first. Both down therefore reads as
		// a visibly darker icon, and each drains on its own clock -- the two squares come back at
		// different times, which is what tells the player a charge has returned.
		pct = GetHotbarCooldownBPercent(i);
		if(pct) {
			cdh = DND_HOTBAR_ICON * sc * pct / 100;
			SetHudClipRect(x * sc, GetHotbarSlotY() * sc + DND_HOTBAR_ICON * sc - cdh,
				DND_HOTBAR_ICON * sc, cdh);
			SetFont("SPLCDBLK");
			HudMessage(s:"A"; HUDMSG_PLAIN, HOTBAR_CD2_ID + i, CR_UNTRANSLATED,
				((x * sc) << 16) + 0.1, ((GetHotbarSlotY() * sc) << 16) + 0.1, 0.1);
			SetHudClipRect(0, 0, 0, 0);
			GetSpellHudDrawn().shown[i] |= DND_HUDL_CD2;
		}
		else
			HideHotbarLayer(i, DND_HUDL_CD2, HOTBAR_CD2_ID);

		// The glint: the cooldown or a charge coming back starts it; a slot that just changed spell does not.
		int ready = CheckInventory(GetHotbarChgItem(i)) > 1 ? GetHotbarChargesReady(i) :
			(GetHotbarCooldownPercent(i) ? 0 : 1);
		if(GetHotbarGlint().spell[i] == spell + 1 && ready > GetHotbarGlint().ready[i])
			GetHotbarGlint().at[i] = Timer();
		GetHotbarGlint().spell[i] = spell + 1;
		GetHotbarGlint().ready[i] = ready;

		int g = Timer() - GetHotbarGlint().at[i];
		if(GetHotbarGlint().at[i] && g >= 0 && g < DND_HOTBAR_GLINTFRAMES) {
			SetFont(StrParam(s:"SPLGLT", s:g < 9 ? "0" : "", d:g + 1));
			HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ADDBLEND, HOTBAR_GLINT_ID + i, CR_UNTRANSLATED,
				((x * sc) << 16) + 0.1, ((GetHotbarSlotY() * sc) << 16) + 0.1, 0.1);
			GetSpellHudDrawn().shown[i] |= DND_HUDL_GLINT;
		}
		else
			HideHotbarLayer(i, DND_HUDL_GLINT, HOTBAR_GLINT_ID);

		// Charges left, top right of the icon. Only for a spell that holds more than one -- on
		// everything else it would be a permanent "1" saying nothing.
		SetHudSize(GetHudStack().w, GetHudStack().h, 0);
		SetFont("SMALLFONT");
		if(CheckInventory(GetHotbarChgItem(i)) > 1) {
			HudMessage(s:"\c[Y5]", d:GetHotbarChargesReady(i); HUDMSG_PLAIN, HOTBAR_CHG_ID + i, CR_WHITE,
				((x + DND_HOTBAR_ICON - 2) << 16) + 0.4, ((GetHotbarSlotY() + 1) << 16) + 0.1, 0.1);
			GetSpellHudDrawn().shown[i] |= DND_HUDL_CHG;
		}
		else
			HideHotbarLayer(i, DND_HUDL_CHG, HOTBAR_CHG_ID);

		// A count bottom right of the icon while above zero.
		int badge = GetHotbarBadge(spell);
		if(badge) {
			HudMessage(s:"\cn", d:badge; HUDMSG_PLAIN, HOTBAR_STACK_ID + i, CR_WHITE,
				((x + DND_HOTBAR_ICON - 2) << 16) + 0.4, ((GetHotbarSlotY() + DND_HOTBAR_ICON - 8) << 16) + 0.1, 0.1);
			GetSpellHudDrawn().shown[i] |= DND_HUDL_STACK;
		}
		else
			HideHotbarLayer(i, DND_HUDL_STACK, HOTBAR_STACK_ID);

		// Which key casts it, under the icon.
		HudMessage(s:"\c[Y5]", d:i + 1; HUDMSG_PLAIN, HOTBAR_SLOT_ID + i, CR_WHITE,
			((x + DND_HOTBAR_ICON / 2) << 16) + 0.4,
			((GetHotbarSlotY() + DND_HOTBAR_ICON + 1) << 16) + 0.1, 0.1);
	}

}


// ---- mana bar --------------------------------------------------------------------------------

#define DND_MANABAR_LIQ0 31         // the liquid's first art column and its width; the rest is the gold frame
#define DND_MANABAR_LIQW 236
#define DND_MANABAR_EDGEW 16        // SPLMANAE, centred on the fill's end
#define DND_MANABAR_LOWFROM 30      // the low mana look fades in from this percent of the usable pool...
#define DND_MANABAR_LOWFULL 20      // ...and is full from here down
#define DND_MANABAR_RAMP 7          // steps the surge and low looks ramp through; the shaders decode 0..7

#define DND_LEECHGLIDE_PCT 15       // while leeching, a bar's rise closes this percent of the gap a tic...
#define DND_LEECHGLIDE_HOLD 35      // ...until this long after the pool emptied, so its last pulse glides too

// Clientside: how far the surge look has ramped in, the fill as drawn (16.16 liquid columns) and the leech glide.
typedef struct {
	int surge;
	int fill;
	int leech;
} manabar_fx_T;

manabar_fx_T module& GetManaBarFx() {
	static manabar_fx_T s;
	return s;
}

// A mana flask drinking right now, so the surge lasts exactly as long as the drink. Literal names: this runs every tic.
bool IsManaFlaskRunning(void) {
	return (CheckInventory("Flask1_TicCounter") && IsManaFlask(CheckInventory("Flask1_Type") - 1)) ||
		(CheckInventory("Flask2_TicCounter") && IsManaFlask(CheckInventory("Flask2_Type") - 1)) ||
		(CheckInventory("Flask3_TicCounter") && IsManaFlask(CheckInventory("Flask3_Type") - 1)) ||
		(CheckInventory("Flask4_TicCounter") && IsManaFlask(CheckInventory("Flask4_Type") - 1));
}

// Tics a leeching bar's rise still glides: refilled while the pool holds anything.
int TickLeechGlide(int tics, int pool) {
	return pool ? DND_LEECHGLIDE_HOLD : Max(tics - 1, 0);
}

// The drawn value catches up to the true one: a rise glides while leeching, anything else lands at once.
int GlideLeechFill(int shown, int target, bool leeching, int least) {
	if(!leeching || target <= shown)
		return target;
	return Min(shown + Max((target - shown) * DND_LEECHGLIDE_PCT / 100, least), target);
}

int GetManaBarX() {
	return GetHudStack().right - DND_HOTBAR_MARGIN - DND_MANABAR_W;
}

// Riding just above the top of the ammo panel.
int GetManaBarY() {
	return GetHudStack().bottom - DND_AMMOPANEL_TOP - DND_MANABAR_GAP - DND_MANABAR_H;
}

// Clientside. Three numbers cross as ammo amounts because PlayerModData does not: current mana, the
// true cap, and the part of it no aura is holding. The bar spans the TRUE cap end to end and draws the
// reserved share as a dead tail on the right, so switching an aura on visibly eats the pool instead of
// silently rescaling the bar under the player.
void DrawManaBar(int pnum) {
	// No pool (Blood Magic): its messages are persistent, so a bar drawn before has to be taken down.
	spell_hud_drawn_T module& d = GetSpellHudDrawn();
	int cap = CheckInventory("P_ManaCap");
	if(cap <= 0) {
		if(d.mana_drawn) {
			DeleteTextRange(MANABAR_TEXT_ID, MANABAR_BACK_ID);
			d.mana_drawn = false;
		}
		return;
	}

	int usable = Min(CheckInventory("ManaVisual"), cap);
	int cur = Min(CheckInventory("Mana"), usable);
	int x = GetManaBarX(), y = GetManaBarY();

	int surge = Clamp_Between(GetManaBarFx().surge + (IsManaFlaskRunning() ? 1 : -1), 0, DND_MANABAR_RAMP);
	GetManaBarFx().surge = surge;
	int low = DND_MANABAR_RAMP;
	if(usable > 0)
		low = Clamp_Between((DND_MANABAR_LOWFROM * usable - 100 * cur) * DND_MANABAR_RAMP /
			((DND_MANABAR_LOWFROM - DND_MANABAR_LOWFULL) * usable), 0, DND_MANABAR_RAMP);

	// Both looks ride the alpha, which ManaFlow.fp and ManaEdge.fp decode: 1 - (surge * 8 + low) / 64.
	int a = 1.0 - (surge * 8 + low) * 1024;

	// A pending leech or mana flask: the ghost reaches as far as they will fill the pool (full mana ends it), and the
	// fill glides up into it as they pay. Both in liquid columns.
	manabar_fx_T module& fx = GetManaBarFx();
	int pool = CheckInventory("ManaLeechAmount") + CheckInventory("FlaskManaPending");
	fx.leech = TickLeechGlide(fx.leech, pool);
	fx.fill = GlideLeechFill(fx.fill, FixedDiv(DND_MANABAR_LIQW * cur, cap), fx.leech, 0.25);
	int fill = fx.fill >> 16;
	int ghost = DND_MANABAR_LIQW * Min(cur + pool, usable) / cap;

	// Every layer below is persistent, so a bar whose inputs did not change is left on screen as it is.
	if(d.mana_drawn && d.mana_cap == cap && d.mana_usable == usable && d.mana_cur == cur && d.mana_fill == fill &&
		d.mana_ghost == ghost && d.mana_a == a && d.mana_x == x && d.mana_y == y && d.mana_w == GetHudStack().w &&
		d.mana_h == GetHudStack().h)
		return;
	d.mana_drawn = true;
	d.mana_cap = cap;
	d.mana_usable = usable;
	d.mana_cur = cur;
	d.mana_fill = fill;
	d.mana_ghost = ghost;
	d.mana_a = a;
	d.mana_x = x;
	d.mana_y = y;
	d.mana_w = GetHudStack().w;
	d.mana_h = GetHudStack().h;

	// Laid out in stack units above, drawn in the bar's canvas where one unit is one art pixel, so
	// the clips below are in art pixels too.
	SetHudSize(GetHudStack().w * DND_MANABAR_CNUM / DND_MANABAR_CDEN, GetHudStack().h * DND_MANABAR_CNUM / DND_MANABAR_CDEN, 0);
	int cx = x * DND_MANABAR_CNUM / DND_MANABAR_CDEN, cy = y * DND_MANABAR_CNUM / DND_MANABAR_CDEN;

	SetFont("SPLMANAB");
	HudMessage(s:"A"; HUDMSG_PLAIN, MANABAR_BACK_ID, CR_UNTRANSLATED,
		(cx << 16) + 0.1, (cy << 16) + 0.1, 0.0);

	// Every segment draws the whole graphic and lets the clip decide what shows. The lengths map onto
	// the liquid alone, or the end caps would make the first and last tenth of the pool invisible.
	int w = DND_MANABAR_LIQW * (cap - usable) / cap;
	if(w > 0) {
		SetHudClipRect(cx + DND_MANABAR_LIQ0 + DND_MANABAR_LIQW - w, cy, w + DND_MANABAR_PXW - DND_MANABAR_LIQ0 - DND_MANABAR_LIQW, DND_MANABAR_PXH);
		SetFont("SPLMANAR");
		HudMessage(s:"A"; HUDMSG_PLAIN, MANABAR_RESERVED_ID, CR_UNTRANSLATED,
			(cx << 16) + 0.1, (cy << 16) + 0.1, 0.0);
		SetHudClipRect(0, 0, 0, 0);
	}
	else
		DeleteText(MANABAR_RESERVED_ID);

	// The leech ghost, from the fill's drawn end to the ghost's.
	if(ghost > fill) {
		SetHudClipRect(cx + DND_MANABAR_LIQ0 + fill, cy, ghost - fill, DND_MANABAR_PXH);
		SetFont("SPLMANAG");
		HudMessage(s:"A"; HUDMSG_PLAIN, MANABAR_GHOST_ID, CR_UNTRANSLATED,
			(cx << 16) + 0.1, (cy << 16) + 0.1, 0.0);
		SetHudClipRect(0, 0, 0, 0);
	}
	else
		DeleteText(MANABAR_GHOST_ID);

	w = fill;
	if(w > 0) {
		SetHudClipRect(cx, cy, DND_MANABAR_LIQ0 + w, DND_MANABAR_PXH);
		SetFont("SPLMANAF");
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA, MANABAR_FILL_ID, CR_UNTRANSLATED,
			(cx << 16) + 0.1, (cy << 16) + 0.1, 0.0, a);

		SetHudClipRect(cx + DND_MANABAR_LIQ0 - 1, cy, DND_MANABAR_LIQW + 2, DND_MANABAR_PXH);
		SetFont("SPLMANAE");
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_ADDBLEND, MANABAR_EDGE_ID, CR_UNTRANSLATED,
			((cx + DND_MANABAR_LIQ0 + w - DND_MANABAR_EDGEW / 2) << 16) + 0.1, (cy << 16) + 0.1, 0.0, a);
		SetHudClipRect(0, 0, 0, 0);
	}
	else {
		DeleteText(MANABAR_FILL_ID);
		DeleteText(MANABAR_EDGE_ID);
	}

	// The readout is the SPENDABLE pair, not the true cap: the reservation already shows as the tail.
	// Back in the stack canvas, so the font is not scaled along with the art.
	SetHudSize(GetHudStack().w, GetHudStack().h, 0);
	SetFont("SMALLFONT");
	// Both fractions are .4: .1 on the y is TOP alignment, which hung the whole readout below the bar's
	// midline instead of centring it on the bar.
	HudMessage(d:cur, s:"\c[D4]/\c-", d:usable; HUDMSG_PLAIN, MANABAR_TEXT_ID, CR_WHITE,
		((x + DND_MANABAR_W / 2) << 16) + 0.4, ((y + DND_MANABAR_H / 2) << 16) + 0.4, 0.0);
}

// ---- life and shield bars --------------------------------------------------------------------

// Clientside: the status bar's health and shield fills as drawn, the life leech and heal glides, the leech pool last
// seen and whether its payout fills the shield (chosen when it starts, as "DnD Lifesteal Script" does).
typedef struct {
	int hp, es;
	int leech, flask, pool;
	bool to_shield;
	int last_hval, last_sval, last_hcap, last_scap, last_hpool, last_spool;
	bool written;
} leech_bars_T;

leech_bars_T module& GetLeechBars() {
	static leech_bars_T s;
	return s;
}

// Capacity first, or the amount is cut to the old one; nothing is sent when both already match.
void SetBarAmmo(str item, int amt, int cap) {
	amt = Min(amt, cap);
	if(GetAmmoCapacity(item) != cap)
		SetAmmoCapacity(item, cap);
	if(CheckInventory(item) != amt)
		SetInventory(item, amt);
}

// Clientside, every tic: the fills SBARINFO draws for the health and shield slots, and the ghosts of the pending life
// leech, life flasks and Essence Theft under the ones they fill. Their numbers stay on P_HealthShown and P_ShieldShown from DnD SBAR.
void UpdateLeechBars(void) {
	leech_bars_T module& s = GetLeechBars();
	// Sanguine Covenant and Cyborg's 5th perk send the leech to the shield, and the Covenant shows the shield in the
	// health slot, so the slot under the ghost follows the Covenant while the payout keeps its pool.
	bool covenant = CheckInventory("DnD_CovenantActive");
	int pool = CheckInventory("LifeStealAmount"), flask = CheckInventory("FlaskLifePending") + CheckInventory("EssenceHealPending");
	if(pool && !s.pool)
		s.to_shield = covenant || CheckInventory(GetClassPerk(DND_PLAYER_CYBORG, 5));
	s.pool = pool;
	s.leech = TickLeechGlide(s.leech, pool);
	s.flask = TickLeechGlide(s.flask, flask);

	// A flask or Essence Theft fills the heal pool, the shield under the Covenant, so it always lands in the health slot.
	int hp = Max(GetActorProperty(0, APROP_HEALTH), 0), es = CheckInventory("EShieldAmount");
	int hval = covenant ? es : hp, sval = covenant ? hp : es;
	bool leech_health = !s.to_shield == !covenant;
	int hpool = flask + (leech_health ? pool : 0), spool = leech_health ? 0 : pool;
	s.hp = GlideLeechFill(s.hp, hval, (s.leech && leech_health) || s.flask, 1);
	s.es = GlideLeechFill(s.es, sval, s.leech && !leech_health, 1);

	// The health slot's capacity grows with overhealth, as P_HealthShown's does; the ghost stops at the true cap.
	int hcap = CheckInventory("P_HealthCap"), cap = Max(hcap, hval), scap = GetAmmoCapacity("P_ShieldShown");

	// Settled (no glide left) on the same numbers as last tic: the four bars already hold them.
	if(s.written && s.hp == hval && s.es == sval && s.last_hval == hval && s.last_sval == sval && s.last_hcap == hcap &&
		s.last_scap == scap && s.last_hpool == hpool && s.last_spool == spool)
		return;
	s.written = true;
	s.last_hval = hval;
	s.last_sval = sval;
	s.last_hcap = hcap;
	s.last_scap = scap;
	s.last_hpool = hpool;
	s.last_spool = spool;

	SetBarAmmo("P_HealthBar", s.hp, cap);
	SetBarAmmo("P_HealthGhost", Max(s.hp, Min(hval + hpool, cap)), cap);

	cap = scap;
	SetBarAmmo("P_ShieldBar", s.es, cap);
	SetBarAmmo("P_ShieldGhost", Max(s.es, Min(sval + spool, cap)), cap);
}

// ---- cast bar --------------------------------------------------------------------------------

// Bottom centre, in the 480x320 layout at 27/16 so one unit is one art pixel. It scales with the screen height like
// "DnD Stamina Bar Draw" under it, so the gap to that script's parry shield holds at every resolution.
#define DND_CASTBAR_CNUM 27
#define DND_CASTBAR_CDEN 16
#define DND_CASTBAR_PXW 339         // SPLCSTB; its trough runs DND_CASTBAR_LIQW columns from DND_CASTBAR_LIQ0
#define DND_CASTBAR_LIQ0 31
#define DND_CASTBAR_LIQW 276
#define DND_CASTBAR_TOP 434         // frame top; its lower crest stops just above the parry shield
#define DND_CASTBAR_FILLY 7         // SPLCSTF covers frame rows 7..22, a lip pair either side of the trough
#define DND_CASTBAR_FILLH 16
#define DND_CASTBAR_BLEED 16        // shown past the front for the shader's glow
#define DND_CASTBAR_CODES 220       // front steps in the alpha; FUA keeps alpha as a byte, so each sits on a byte bucket's centre
#define DND_CASTBAR_LABEL 10        // the spell name's top, in 480x320 units above the frame
#define DND_CASTBAR_REFUSAL 11      // a refused cast's reason, this far above the label

// -1 when nothing is being cast, else how far ELAPSED the cast is, 0 to 100. Same packed word and the
// same reasoning as GetHotbarCooldownPercent: the client animates it off ServerTimer().
int GetSpellCastPercent() {
	int end = CheckInventory("P_CastProgress");
	if(!end)
		return -1;

	int total = UnpackHotbarTotal(CheckInventory("P_CastSpell") >> DND_CASTBAR_TOTALSHIFT);
	if(total <= 0)
		return -1;

	int left = end - ServerTimer();
	if(left <= 0)
		return -1;

	return Max(0, Min(100, 100 - left * 100 / total));
}

void ClearSpellCastBar() {
	DeleteTextRange(CASTBAR_TEXT_ID, CASTBAR_BACK_ID);
	DeleteText(CASTBAR_ICON_ID);
}

// Clientside. Starts empty and fills to the right edge as the cast runs out, with the spell being cast
// named over it; a channel's window (DND_CASTBAR_DRAIN) starts full and empties instead.
void DrawSpellCastBar(int pnum) {
	int pct = GetSpellCastPercent();

	// Silencer: with nothing being cast, the bar counts the silence down under its own label.
	bool silenced = false;
	if(pct < 0 && GetSilencePercent()) {
		pct = GetSilencePercent();
		silenced = true;
	}

	if(pct < 0) {
		if(GetSpellHudDrawn().cast_drawn) {
			ClearSpellCastBar();
			GetSpellHudDrawn().cast_drawn = false;
		}
		return;
	}
	GetSpellHudDrawn().cast_drawn = true;

	int spell = (CheckInventory("P_CastSpell") & DND_CASTBAR_SPELLMASK) - 1;
	if(!silenced && (CheckInventory("P_CastSpell") & DND_CASTBAR_DRAIN))
		pct = 100 - pct;

	int cw = HUDMAX_X * DND_CASTBAR_CNUM / DND_CASTBAR_CDEN;
	int x = (cw - DND_CASTBAR_PXW) / 2, y = DND_CASTBAR_TOP;
	SetHudSize(cw, HUDMAX_Y * DND_CASTBAR_CNUM / DND_CASTBAR_CDEN, 0);

	SetFont("SPLCSTB");
	HudMessage(s:"A"; HUDMSG_PLAIN, CASTBAR_BACK_ID, CR_UNTRANSLATED,
		(x << 16) + 0.1, (y << 16) + 0.1, 0.0);

	// The fill stays put on the trough and CastBar.fp finds the front in the alpha. SPLCSTS is the crimson silenced art.
	int w = DND_CASTBAR_LIQW * pct / 100;
	if(w > 0) {
		int code = w * DND_CASTBAR_CODES / DND_CASTBAR_LIQW;
		SetHudClipRect(x + DND_CASTBAR_LIQ0, y + DND_CASTBAR_FILLY, Min(w + DND_CASTBAR_BLEED, DND_CASTBAR_LIQW), DND_CASTBAR_FILLH);
		SetFont(silenced ? "SPLCSTS" : "SPLCSTF");
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA, CASTBAR_FILL_ID, CR_UNTRANSLATED,
			((x + DND_CASTBAR_LIQ0) << 16) + 0.1, ((y + DND_CASTBAR_FILLY) << 16) + 0.1, 0.0,
			1.0 - ((2 * code + 1) << 16) / 510);
		SetHudClipRect(0, 0, 0, 0);
	}
	else
		DeleteText(CASTBAR_FILL_ID);

	SetHudSize(HUDMAX_X, HUDMAX_Y, 0);
	int label = y * DND_CASTBAR_CDEN / DND_CASTBAR_CNUM - DND_CASTBAR_LABEL;
	if(silenced || (spell >= 0 && spell < MAX_SPELL_IDS)) {
		SetFont("SMALLFONT");
		HudMessage(s:silenced ? "\cg" : "\c[Y5]", l:silenced ? "DND_SPLHUD_SILENCED" : GetSpellNameLump(spell); HUDMSG_PLAIN, CASTBAR_TEXT_ID, CR_WHITE,
			((HUDMAX_X / 2) << 16) + 0.4, (label << 16) + 0.1, 0.0);
	}
	else
		DeleteText(CASTBAR_TEXT_ID);

	// Silencer: SILENCED at half size in a doubled canvas, centred over the bar, its bottom just above the label.
	if(silenced) {
		SetHudSize(HUDMAX_X * 2, HUDMAX_Y * 2, 0);
		SetFont("SILENCED");
		HudMessage(s:"A"; HUDMSG_PLAIN, CASTBAR_ICON_ID, CR_UNTRANSLATED,
			HUDMAX_X << 16, ((label * 2 - 4) << 16) + 0.2, 0.0);
		SetHudSize(HUDMAX_X, HUDMAX_Y, 0);
	}
	else
		DeleteText(CASTBAR_ICON_ID);
}

// Client side: the caster's own refused cast (no corpse, no minion), a line over where the cast bar names its spell.
Script "DnD Spell Refusal" (int pnum, int res) CLIENTSIDE {
	if(ConsolePlayerNumber() != pnum)
		Terminate;
	SetHudSize(HUDMAX_X, HUDMAX_Y, 0);
	SetFont("SMALLFONT");
	int y = DND_CASTBAR_TOP * DND_CASTBAR_CDEN / DND_CASTBAR_CNUM - DND_CASTBAR_LABEL - DND_CASTBAR_REFUSAL;
	HudMessage(l:res == CAST_NOCORPSE ? "DND_SPLNOCORPSE" : "DND_SPLNOMINION"; HUDMSG_FADEOUT, CASTBAR_REFUSAL_ID, CR_RED,
		((HUDMAX_X / 2) << 16) + 0.4, (y << 16) + 0.1, 1.5, 0.5);
	SetResultValue(0);
}

// Everything the spell HUD draws, in one sweep: the bars sit just under the hotbar's three runs and
// nothing else lives in that stretch of ids.
void ClearSpellHotbar() {
	DeleteTextRange(MANABAR_TEXT_ID, HOTBAR_SLOT_ID + MAX_HOTBAR_SLOTS);
	for(int i = 0; i < MAX_HOTBAR_SLOTS; ++i)
		GetSpellHudDrawn().shown[i] = 0;
	GetSpellHudDrawn().mana_drawn = false;
	GetSpellHudDrawn().cast_drawn = false;
}

#endif
