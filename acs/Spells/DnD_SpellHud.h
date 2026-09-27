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

#define DND_HOTBAR_ICON 16
#define DND_HOTBAR_GAP 2
#define DND_HOTBAR_RIGHT 314
#define DND_HOTBAR_Y 128

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
	return DND_HOTBAR_RIGHT - (count - slot) * (DND_HOTBAR_ICON + DND_HOTBAR_GAP);
}

Script "DnD Spell Hotbar" ENTER CLIENTSIDE {
	int pnum = PlayerNumber();
	if(ConsolePlayerNumber() != pnum)
		Terminate;

	int i, count, spell, x, pct, drawn = 0;

	while(PlayerInGame(pnum)) {
		count = GetHotbarSlotCount(pnum);

		if(isAlive() && !CheckInventory("ShowingMenu")) {
			SetHudSize(320, 200, 1);

			for(i = 0; i < count; ++i) {
				spell = CheckInventory(GetHotbarSlotItem(i)) - 1;
				x = GetHotbarSlotX(i, count);

				SetFont("SPLBOX");
				HudMessage(s:"A"; HUDMSG_PLAIN, HOTBAR_SLOT_ID + i, CR_UNTRANSLATED,
					(x << 16) + 0.1, (DND_HOTBAR_Y << 16) + 0.1, 0.1);

				if(spell < 0) {
					DeleteText(HOTBAR_ICON_ID + i);
					DeleteText(HOTBAR_CD_ID + i);
					continue;
				}

				SetFont(GetSpellIcon(spell, false));
				HudMessage(s:"A"; HUDMSG_PLAIN, HOTBAR_ICON_ID + i, CR_UNTRANSLATED,
					(x << 16) + 0.1, (DND_HOTBAR_Y << 16) + 0.1, 0.1);

				// The black square is clipped to the share of the cooldown still to run and anchored
				// at the top, so it wipes downward off the icon as the cooldown finishes.
				pct = GetHotbarCooldownPercent(i);
				if(pct) {
					SetHudClipRect(x - DND_HOTBAR_ICON / 2, DND_HOTBAR_Y - DND_HOTBAR_ICON / 2,
						DND_HOTBAR_ICON, DND_HOTBAR_ICON * pct / 100, DND_HOTBAR_ICON);
					SetFont("SPLCDBLK");
					HudMessage(s:"A"; HUDMSG_PLAIN, HOTBAR_CD_ID + i, CR_UNTRANSLATED,
						(x << 16) + 0.1, (DND_HOTBAR_Y << 16) + 0.1, 0.1);
					SetHudClipRect(0, 0, 0, 0, 0);
				}
				else
					DeleteText(HOTBAR_CD_ID + i);
			}

			drawn = count;
		}
		else if(drawn) {
			DeleteTextRange(HOTBAR_SLOT_ID, HOTBAR_CD_ID + MAX_HOTBAR_SLOTS);
			drawn = 0;
		}

		Delay(const:1);
	}
}

#endif
