#ifndef DND_WEAPONWHEEL_IN
#define DND_WEAPONWHEEL_IN

#define DND_MAX_OWNEDWEPS 16 // max 16
#define DND_WHEEL_INSIDERADIUS 25
#define DND_WHEEL_INSIDERADIUS_SQ (DND_WHEEL_INSIDERADIUS * DND_WHEEL_INSIDERADIUS)
#define DND_WHEEL_HUDRADIUS 192
#define DND_WHEEL_HUDCURSORDETECT_RADIUS 216 // the radius used to detect if cursor is in circular area on hud
#define DND_WHEEL_HUDRADIUS_SQ (DND_WHEEL_HUDCURSORDETECT_RADIUS * DND_WHEEL_HUDCURSORDETECT_RADIUS)
#define DND_WHEEL_ALPHA 0.666
#define DND_WHEEL_LINE_STEP 6
#define DND_WHEEL_LINE_LEN 32

#define DND_WHEEL_ENLARGETICS 8
#define DND_WHEEL_ENLARGEAMT -0.25 // 25%

int wep_scaling_state = 0;

bool IsWheelIconBeingScaled(int i) {
    return wep_scaling_state & (1 << i);
}

void SetWheelIconScaled(int i) {
    wep_scaling_state |= (1 << i);
}

void ClearWheelIconScaled(int i) {
    wep_scaling_state &= ~(1 << i);
}

Script "DnD Create Weapon Wheel" (int pnum, int cleanup, int wantChange) CLIENTSIDE {
    if(ConsolePlayerNumber() != pnum)
        Terminate;

    int cpn = ConsolePlayerNumber();

    if(!cleanup) {
        LocalAmbientSound("WeaponWheel/Open", 127);

        int i, j, k, count, ang, temp, prev_wep;
        int px, py;
        static int owned_guns[DND_MAX_OWNEDWEPS];
        for(i = 0; i < DND_MAX_OWNEDWEPS; ++i)
            owned_guns[i] = 0;
        
        int width = (HUDMAX_X_PROMPT * FixedDiv(1.0, ScreenResOffsets[SCREEN_ASPECT_RATIO])) >> 16;
	    int height = (HUDMAX_Y_PROMPT * FixedDiv(1.0, ASPECT_4_3)) >> 16;

        int o_width = width;
        int o_height = height;

        SetHudSize(width, height, 1);

        // fixed point halved
        width = (width << 15) + 0.4;
        height <<= 15;

        // the below are all static elements
        SetFont("WHEELBAK");
        HudMessage(s:"A";  HUDMSG_ALPHA, RPGMENUBACKGROUNDID, -1, width, height, 0.0, DND_WHEEL_ALPHA);

        SetFont("WHEELMID");
        HudMessage(s:"A";  HUDMSG_PLAIN, RPGMENUWEAPONPANELID, -1, width, height, 0.0);

        count = 0;
        for(i = 0; i < MAXWEPS; ++i) {
            if(CheckInventory(Weapons_Data[i].name))
                owned_guns[count++] = i;
        }

        int hx = 0, hy = 0;

        // draw clockwise
        for(i = 0; i < count; ++i) {
            j = owned_guns[i];
            ang = i * 1.0 / count;
            hx = width + GetIntegerBits(DND_WHEEL_HUDRADIUS * cos(ang));
            hy = height + GetIntegerBits(DND_WHEEL_HUDRADIUS * sin(ang));

            SetFont(Weapons_Data[j].icon);
            HudMessage(s:"A";  HUDMSG_PLAIN, RPGMENUITEMID - i, -1, hx, hy, 0.0, 0.0);

            // lines appear directly "opposite" where the corresponding icon is
            SetFont("WHEELFIL");
            for(j = 0; j < DND_WHEEL_LINE_LEN; ++j) {
                ang = i * 1.0 / count;
                if(!(count % 2))
                    ang += 0.5 / count;
                hx = width + GetIntegerBits(DND_WHEEL_LINE_STEP * (j - 36) * cos(ang));
                hy = height + GetIntegerBits(DND_WHEEL_LINE_STEP * (j - 36) * sin(ang));
                ang = (j + 4) * 1.0 / DND_WHEEL_LINE_LEN;
                HudMessage(s:"A";  HUDMSG_ALPHA, RPGMENUITEMID - count - i * DND_WHEEL_LINE_LEN - j, -1, hx, hy, 0.0, ang);
            }
        }
        // static elements are finished from here on

        PlayerCursorData.posx = width - 0.4;
	    PlayerCursorData.posy = height;
        prev_wep = -1;
        while(CheckInventory("ShowingWeaponWheel") && isAlive()) {
            // cursor handling
            PlayerCursorData.posx = GetCursorPos(GetPlayerInput(cpn, INPUT_YAW), MOUSE_INPUT_X, o_width, o_height);
            PlayerCursorData.posy = GetCursorPos(GetPlayerInput(cpn, INPUT_PITCH), MOUSE_INPUT_Y, o_width, o_height);

            DrawCursor(o_width << 16, o_height << 16);

            k = -1;
            i = -1;

            Delay(const:1);

            hx = VectorAngle(PlayerCursorData.posx - width - 0.4, PlayerCursorData.posy - height);

            px = PlayerCursorData.posx >> 16;
            px -= o_width / 2;
            py = PlayerCursorData.posy >> 16;
            py -= o_height / 2;

            px *= px;
            py *= py;
            px += py;

            if(px > DND_WHEEL_INSIDERADIUS_SQ && px <= DND_WHEEL_HUDRADIUS_SQ) {
                // check for each point the sector of the circle we could be in now
                for(i = 0; i < count; ++i) {
                    // px and py are now the two sides of the sector
                    px = i * 1.0 / count;
                    py = (i + 1) * 1.0 / count;

                    // tilts the tile to be diagonal on even numbers
                    if(!(count % 2)) {
                        px += 0.5 / count;
                        py += 0.5 / count;
                    }

                    if(px > 1.0)
                        px -= 1.0;
                    if(py > 1.0)
                        py -= 1.0;

                    if(py < px) {
                        temp = py;
                        py = px;
                        px = temp;
                    }

                    //Log(f:hx, s: " vs limits on ", d:i, s:": ", f:px, s:" ", f:py);

                    if(hx >= px && hx < py) {
                        // this is needed because of the weird angle offset we impose on the shape...
                        i = (i + (count > 2) * (1 + count / 2)) % count;
                        k = owned_guns[i];
                        //Log(s:"picked ", d:k, s: " with i: ", d:i);
                        break;
                    }
                }

                if(k == -1) {
                    i = count - 1 - (count - 1) / 2;
                    k = owned_guns[i];
                }
            }

            if(k != -1) {
                // display the name of the weapon under it and put it on the middle, also enlarge the icon
                SetFont("WHEELMDS");
                HudMessage(s:"A";  HUDMSG_PLAIN, RPGMENUWEAPONPANELID, -1, width, height, 0.0);

                if(prev_wep != i && !IsWheelIconBeingScaled(i)) {
                    LocalAmbientSound("WeaponWheel/Hover", 127);
                    SetWheelIconScaled(i);
                    ACS_NamedExecuteWithResult("DnD Weapon Wheel Scale Icon", i, k, count);
                }

                SetInventory("WeaponWheelChangeId", k);

                SetFont(Weapons_Data[k].icon);
                HudMessage(s:"A";  HUDMSG_PLAIN, RPGMENUWEAPONPANELID - 1, -1, width, height, 0.0, 0.0);
                
                SetFont("SMALLFONT");
                HudMessage(s:"\c[H9]", l:GetWeaponTag(k);  HUDMSG_PLAIN, RPGMENUINVENTORYID, -1, width, height + 28.1, 0.0);
            }
            else {
                DeleteText(RPGMENUINVENTORYID);
                DeleteText(RPGMENUWEAPONPANELID - 1);
                SetInventory("WeaponWheelChangeId", INT_MAX);
            }
            
            if(k == -1 || prev_wep != i) {
                // if we had a weapon previously, shrink it back
                if(prev_wep != -1) {
                    // restore original draw
                    SetHudSize(o_width, o_height, 1);
                    
                    ang = prev_wep * 1.0 / count;
                    hx = GetIntegerBits(width + DND_WHEEL_HUDRADIUS * cos(ang)) + 0.4;
                    hy = GetIntegerBits(height + DND_WHEEL_HUDRADIUS * sin(ang));

                    SetFont(Weapons_Data[owned_guns[prev_wep]].icon);
                    HudMessage(s:"A";  HUDMSG_PLAIN, RPGMENUITEMID - prev_wep, -1, hx, hy, 0.0, 0.0);
                    ClearWheelIconScaled(prev_wep);
                }

                SetFont("WHEELMID");
                HudMessage(s:"A";  HUDMSG_PLAIN, RPGMENUWEAPONPANELID, -1, width, height, 0.0);
            }

            prev_wep = i;
        }
    }
    else {
        k = CheckInventory("WeaponWheelChangeId");
        if(wantChange) {
            if(k != INT_MAX) {
                LocalAmbientSound("WeaponWheel/Select", 127);
                NamedRequestScriptPuke("DND Weapon Wheel Change", cpn, k);
            }
            else
                LocalAmbientSound("WeaponWheel/Exit", 127);
        }

        Delay(const:1);

        wep_scaling_state = 0;

        DeleteText(RPGMENUCURSORID);
        DeleteText(RPGMENUBACKGROUNDID);
        DeleteText(RPGMENUINVENTORYID);
        DeleteText(RPGMENUWEAPONPANELID);
        DeleteText(RPGMENUWEAPONPANELID - 1);
        DeleteTextRange(RPGMENUITEMID - 16, RPGMENUITEMID);
        DeleteTextRange(RPGMENUITEMID - 16 * (DND_WHEEL_LINE_LEN + 1), RPGMENUITEMID - 17);
    }

    SetResultValue(0);
}

// negative factor enlarges, positive shrinks
Script "DnD Weapon Wheel Scale Icon" (int index, int wepid, int count) CLIENTSIDE {
    int o_width = (HUDMAX_X_PROMPT * FixedDiv(1.0, ScreenResOffsets[SCREEN_ASPECT_RATIO])) >> 16;
    int o_height = (HUDMAX_Y_PROMPT * FixedDiv(1.0, ASPECT_4_3)) >> 16;
    int ang = index * 1.0 / count;
    int width, height, hx, hy;

    for(int i = 0; i < DND_WHEEL_ENLARGETICS && CheckInventory("ShowingWeaponWheel") && isAlive() && IsWheelIconBeingScaled(index); ++i) {
        int f = (1.0 + DND_WHEEL_ENLARGEAMT * i / DND_WHEEL_ENLARGETICS);

        width = o_width * f;
        height = o_height * f; 

        SetHudSize(width >> 16, height >> 16, 1);

        hx = GetIntegerBits(width / 2 + FixedMul(DND_WHEEL_HUDRADIUS * cos(ang), f)) + 0.4;
        hy = GetIntegerBits(height / 2 + FixedMul(DND_WHEEL_HUDRADIUS * sin(ang), f));

        SetFont(Weapons_Data[wepid].icon);
        HudMessage(s:"A";  HUDMSG_PLAIN, RPGMENUITEMID - index, -1, hx, hy, 0.0, 0.0);

        Delay(const:1);
    }

    // cleanup just in case
    if(!CheckInventory("ShowingWeaponWheel") || !isAlive())
        DeleteText(RPGMENUITEMID - index);
    else if(!IsWheelIconBeingScaled(index)) {
        width = o_width << 15;
        height = o_height << 15;
        hx = GetIntegerBits(width + DND_WHEEL_HUDRADIUS * cos(ang)) + 0.4;
        hy = GetIntegerBits(height + DND_WHEEL_HUDRADIUS * sin(ang));

        SetHudSize(o_width, o_height, 1);
        SetFont(Weapons_Data[wepid].icon);
        HudMessage(s:"A";  HUDMSG_PLAIN, RPGMENUITEMID - index, -1, hx, hy, 0.0, 0.0);
    }

    ClearWheelIconScaled(index);

    SetResultValue(0);
}

Script "DND Weapon Wheel Change" (int pnum, int wepid) NET {
    if(!PlayerInGame(pnum) || PlayerIsSpectator(pnum))
        Terminate;
    SetActivator(pnum + P_TIDSTART);

    do {
        // in these cases we don't want a weapon swap to happen, it's meaningless
        if(!IsAlive() || !PlayerInGame(pnum) || PlayerIsSpectator(pnum))
            break;

        SetWeapon(Weapons_Data[wepid].name);
        Delay(const:7);
    } while(!CheckWeapon(Weapons_Data[wepid].name));
}

Script "DnD Weapon Wheel" (void) NET {
    int pnum = PlayerNumber();
    if(!PlayerInGame(pnum) || PlayerIsSpectator(pnum))
        Terminate;

    if(CheckInventory("ShowingWeaponWheel")) {
        TakeInventory("ShowingWeaponWheel", 1);
        ACS_NamedExecuteWithResult("DnD Create Weapon Wheel", pnum, 1, 1);
    }
    else if
    (
        !CheckInventory("ShowingMenu") &&
        !CheckInventory("ShowingPrompt") &&
        !CheckInventory("P_Frozen")
    )
    {
        GiveInventory("ShowingWeaponWheel", 1);
        ACS_NamedExecuteWithResult("DnD Create Weapon Wheel", pnum);
    }
}

#endif