#ifndef DND_SKILLWHEEL_IN
#define DND_SKILLWHEEL_IN

// The skill wheel: hold to open, steer with the mouse, release to cast the hovered hotbar slot where you looked.

#define DND_SKILLWHEEL_RADIUS 176           // icon ring, in the weapon wheel's canvas
#define DND_SKILLWHEEL_LABELRADIUS 108      // slot numbers, between the middle and the icons
#define DND_SKILLWHEEL_DEADZONE 32          // the cursor this close to the middle picks nothing

// HUD ids, shared with the weapon wheel and the menu, which never show at the same time as this.
#define SKILLWHEEL_ICON_ID (RPGMENUITEMID)
#define SKILLWHEEL_LABEL_ID (RPGMENUITEMID - MAX_HOTBAR_SLOTS)

// Client side: the slot under the cursor, -1 for none.
typedef struct {
	int slot;
} skill_wheel_T;

skill_wheel_T module& GetSkillWheel() {
	static skill_wheel_T w;
	return w;
}

// Slot 1 at the top, the rest clockwise; HUD y runs down, so a growing angle turns clockwise.
int GetSkillWheelSlotAngle(int slot, int count) {
	return 0.75 + slot * 1.0 / count;
}

// The slot whose sector holds this offset from the middle, or -1 inside the dead zone.
int GetSkillWheelSlotAt(int dx, int dy, int count) {
	int px = dx >> 16, py = dy >> 16;
	if(px * px + py * py <= DND_SKILLWHEEL_DEADZONE * DND_SKILLWHEEL_DEADZONE)
		return -1;

	int rel = VectorAngle(dx, dy) - 0.75 + 0.5 / count;
	while(rel < 0)
		rel += 1.0;
	while(rel >= 1.0)
		rel -= 1.0;
	return Min((rel * count) >> 16, count - 1);
}

void DrawSkillWheelSlot(int slot, int count, int cx, int cy) {
	int ang = GetSkillWheelSlotAngle(slot, count);
	int spell = CheckInventory(GetHotbarSlotItem(slot)) - 1;

	if(spell >= 0) {
		SetFont(GetSpellIcon(spell, GetHotbarCooldownPercent(slot) > 0));
		HudMessage(s:"A"; HUDMSG_PLAIN, SKILLWHEEL_ICON_ID - slot, -1,
			cx + GetIntegerBits(DND_SKILLWHEEL_RADIUS * cos(ang)), cy + GetIntegerBits(DND_SKILLWHEEL_RADIUS * sin(ang)), 0.0, 0.0);
	}
	else
		DeleteText(SKILLWHEEL_ICON_ID - slot);

	SetFont("SMALLFONT");
	HudMessage(s:spell >= 0 ? "\c[Y5]" : "\c[B1]", d:slot + 1; HUDMSG_PLAIN, SKILLWHEEL_LABEL_ID - slot, -1,
		cx + GetIntegerBits(DND_SKILLWHEEL_LABELRADIUS * cos(ang)), cy + GetIntegerBits(DND_SKILLWHEEL_LABELRADIUS * sin(ang)), 0.0, 0.0);
}

Script "DnD Create Skill Wheel" (int pnum, int cleanup, int wantCast) CLIENTSIDE {
	if(ConsolePlayerNumber() != pnum)
		Terminate;

	skill_wheel_T module& w = GetSkillWheel();

	if(cleanup) {
		if(wantCast && w.slot != -1 && CheckInventory(GetHotbarSlotItem(w.slot))) {
			LocalAmbientSound("WeaponWheel/Select", 127);
			NamedRequestScriptPuke("DnD Skill Wheel Cast", w.slot, GetWheelView().angle, GetWheelView().pitch);
		}
		else if(wantCast)
			LocalAmbientSound("WeaponWheel/Exit", 127);

		// A tic for the draw loop to see the wheel closed before its text goes.
		Delay(const:2);
		if(!IsWheelShowing())
			ClearWheels();
		SetResultValue(0);
		Terminate;
	}

	LocalAmbientSound("WeaponWheel/Open", 127);

	int o_width = (HUDMAX_X_PROMPT * FixedDiv(1.0, ScreenResOffsets[SCREEN_ASPECT_RATIO])) >> 16;
	int o_height = (HUDMAX_Y_PROMPT * FixedDiv(1.0, ASPECT_4_3)) >> 16;
	int cx = (o_width << 15) + 0.4, cy = o_height << 15;
	int count = GetHotbarSlotCount(pnum), i, j, spell, prev = -2, tic = 0;

	w.slot = -1;

	ClearWheels();
	LockWheelView(pnum);
	SetHudSize(o_width, o_height, 1);

	SetFont("WHEELBAK");
	HudMessage(s:"A"; HUDMSG_ALPHA, RPGMENUBACKGROUNDID, -1, cx, cy, 0.0, DND_WHEEL_ALPHA);
	SetFont("WHEELMID");
	HudMessage(s:"A"; HUDMSG_PLAIN, WHEEL_RING_ID, -1, cx, cy, 0.0);

	for(i = 0; i < count; ++i)
		DrawWheelBorder(i, count, true, false, cx, cy);

	PlayerCursorData.posx = o_width << 15;
	PlayerCursorData.posy = o_height << 15;

	while(CheckInventory("ShowingSkillWheel") && IsAlive()) {
		HoldWheelView(pnum);

		PlayerCursorData.posx = GetCursorPos(GetPlayerInput(pnum, INPUT_YAW), MOUSE_INPUT_X, o_width, o_height);
		PlayerCursorData.posy = GetCursorPos(GetPlayerInput(pnum, INPUT_PITCH), MOUSE_INPUT_Y, o_width, o_height);

		SetHudSize(o_width, o_height, 1);
		DrawCursor(o_width << 16, o_height << 16);

		// Cooldowns change while it is open, so the ring redraws a few times a second.
		if(!(tic++ % 4))
			for(i = 0; i < count; ++i)
				DrawSkillWheelSlot(i, count, cx, cy);

		// The cursor is stored mirrored, so the offset from the middle is centre minus position.
		w.slot = GetSkillWheelSlotAt((o_width << 15) - PlayerCursorData.posx, (o_height << 15) - PlayerCursorData.posy, count);
		spell = w.slot != -1 ? CheckInventory(GetHotbarSlotItem(w.slot)) - 1 : -1;

		// The name redraws every tic a spell is hovered, so its cooldown counts down under it.
		if(spell >= 0) {
			j = CheckInventory(GetHotbarCdItem(w.slot)) - Timer();
			SetFont("SMALLFONT");
			if(j > 0)
				HudMessage(s:"\c[H9]", l:GetSpellNameLump(spell), s:"\n\c[B1]", s:TicsToSeconds(j), s:"s"; HUDMSG_PLAIN, RPGMENUINVENTORYID, -1, cx, cy + 40.1, 0.0);
			else
				HudMessage(s:"\c[H9]", l:GetSpellNameLump(spell); HUDMSG_PLAIN, RPGMENUINVENTORYID, -1, cx, cy + 40.1, 0.0);
		}

		if(w.slot != prev) {
			if(prev >= 0)
				DrawWheelSectorBorders(prev, count, true, false, cx, cy);
			if(w.slot != -1)
				DrawWheelSectorBorders(w.slot, count, true, true, cx, cy);

			if(spell >= 0) {
				LocalAmbientSound("WeaponWheel/Hover", 127);
				SetFont("WHEELMDS");
				HudMessage(s:"A"; HUDMSG_PLAIN, WHEEL_RING_ID, -1, cx, cy, 0.0);
				SetFont(GetSpellIcon(spell, false));
				HudMessage(s:"A"; HUDMSG_PLAIN, WHEEL_CENTRE_ID, -1, cx, cy, 0.0, 0.0);
			}
			else {
				SetFont("WHEELMID");
				HudMessage(s:"A"; HUDMSG_PLAIN, WHEEL_RING_ID, -1, cx, cy, 0.0);
				DeleteText(WHEEL_CENTRE_ID);
				if(w.slot != -1) {
					SetFont("SMALLFONT");
					HudMessage(s:"\c[B1]", l:"DND_SKILLWHEEL_EMPTY"; HUDMSG_PLAIN, RPGMENUINVENTORYID, -1, cx, cy + 40.1, 0.0);
				}
				else
					DeleteText(RPGMENUINVENTORYID);
			}
			prev = w.slot;
		}

		Delay(const:1);
	}

	// the weapon wheel may have taken over the camera already
	if(!IsWheelShowing() || !IsAlive())
		UnlockWheelView(pnum);

	// Let go or closed by another screen: that call clears the HUD. Death has none, so it clears here.
	if(!CheckInventory("ShowingSkillWheel") && IsAlive())
		Terminate;
	w.slot = -1;
	ClearWheels();
	SetResultValue(0);
}

// Server side. Shuts the wheel without casting, for anything that takes the screen over.
void CloseSkillWheel(int pnum) {
	if(CheckActorInventory(pnum + P_TIDSTART, "ShowingSkillWheel")) {
		TakeActorInventory(pnum + P_TIDSTART, "ShowingSkillWheel", 1);
		ACS_NamedExecuteWithResult("DnD Create Skill Wheel", pnum, 1, 0);
	}
}

// The bind is held: open on press, cast the hovered slot on release.
Script "DnD Skill Wheel" (int open) NET {
	int pnum = PlayerNumber();
	if(pnum < 0 || !PlayerInGame(pnum) || PlayerIsSpectator(pnum))
		Terminate;

	if(!open) {
		if(CheckInventory("ShowingSkillWheel")) {
			TakeInventory("ShowingSkillWheel", 1);
			ACS_NamedExecuteWithResult("DnD Create Skill Wheel", pnum, 1, 1);
		}
		Terminate;
	}

	if(CheckInventory("ShowingSkillWheel") || CheckInventory("ShowingMenu") || CheckInventory("ShowingPrompt") ||
		CheckInventory("P_Frozen") || !HasChosenClass() || !IsAlive())
		Terminate;

	if(CheckInventory("ShowingWeaponWheel")) {
		TakeInventory("ShowingWeaponWheel", 1);
		ACS_NamedExecuteWithResult("DnD Create Weapon Wheel", pnum, 1);
	}

	GiveInventory("ShowingSkillWheel", 1);
	HideWheelName();
	ACS_NamedExecuteWithResult("DnD Create Skill Wheel", pnum, 0, 0);
}

// The client's pick, aimed where the locked view looked.
Script "DnD Skill Wheel Cast" (int slot, int angle, int pitch) NET {
	int pnum = PlayerNumber();
	if(pnum < 0 || !PlayerInGame(pnum) || PlayerIsSpectator(pnum) || !IsAlive())
		Terminate;

	SetActorAngle(0, angle);
	SetActorPitch(0, pitch);
	TryCastHotbarSlot(pnum, slot);
}

#endif
