#ifndef DND_WEAPONWHEEL_IN
#define DND_WEAPONWHEEL_IN

#define DND_MAX_OWNEDWEPS 16 // max 16
#define DND_WHEEL_HUDRADIUS 192
#define DND_WHEEL_HUDRADIUS_SQ (DND_WHEEL_HUDRADIUS * DND_WHEEL_HUDRADIUS)
#define DND_WHEEL_ALPHA 0.666
#define DND_WHEEL_LINE_STEP 6
#define DND_WHEEL_LINE_LEN 32

Script "DnD Create Weapon Wheel" (int pnum, int cleanup) CLIENTSIDE {
    if(ConsolePlayerNumber() != pnum)
        Terminate;

    int cpn = ConsolePlayerNumber();

    if(!cleanup) {
        int i, j, k, count, ang, temp;
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

        for(i = 0; i < count; ++i) {
            j = owned_guns[i];
            ang = i * 1.0 / count;
            hx = width + GetIntegerBits(DND_WHEEL_HUDRADIUS * cos(ang));
            hy = height + GetIntegerBits(DND_WHEEL_HUDRADIUS * sin(ang));

            SetFont(Weapons_Data[j].icon);
            HudMessage(s:"A";  HUDMSG_PLAIN, RPGMENUITEMID - i, -1, hx, hy, 0.0, 0.0);

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
        while(CheckInventory("ShowingWeaponWheel") && isAlive()) {
            // cursor handling
            PlayerCursorData.posx = GetCursorPos(GetPlayerInput(cpn, INPUT_YAW), MOUSE_INPUT_X, o_width, o_height);
            PlayerCursorData.posy = GetCursorPos(GetPlayerInput(cpn, INPUT_PITCH), MOUSE_INPUT_Y, o_width, o_height);

            DrawCursor(o_width << 16, o_height << 16);

            Delay(const:1);

            k = -1;

            temp = VectorAngle(PlayerCursorData.posx - width - 0.4, PlayerCursorData.posy - height);

            px = PlayerCursorData.posx >> 16;
            px -= o_width / 2;
            py = PlayerCursorData.posy >> 16;
            py -= o_height / 2;

            px *= px;
            py *= py;
            px += py;

            for(i = 0; i < count; ++i) {
                j = 0.5 / count; // step +- from ang
                ang = i * 1.0 / count;
                Log(f:temp, s: " ", f:ang + j, s: " ", f:ang - j);
                if
                (
                    px <= DND_WHEEL_HUDRADIUS_SQ &&
                    temp <= ang + j && temp >= ang - j
                )
                {
                    k = i;
                    break;
                }
            }

            if(k != -1) {
                SetFont("SMALLFONT");
                Log(s:"located on wep ", d:k, s: ": ", s:Weapons_Data[k].name);
            }
            else
                DeleteText(RPGMENUWEAPONPANELID - 1);
        }
    }
    else {
        DeleteText(RPGMENUCURSORID);
        DeleteText(RPGMENUBACKGROUNDID);
        DeleteText(RPGMENUWEAPONPANELID - 1);
        DeleteText(RPGMENUWEAPONPANELID);
        DeleteTextRange(RPGMENUITEMID - 16, RPGMENUITEMID);
        DeleteTextRange(RPGMENUITEMID - 16 * DND_WHEEL_LINE_LEN, RPGMENUITEMID - 17);
    }

    SetResultValue(0);
}

Script "DnD Weapon Wheel" (void) NET {
    int pnum = PlayerNumber();

    if(CheckInventory("ShowingWeaponWheel")) {
        TakeInventory("ShowingWeaponWheel", 1);
        ACS_NamedExecuteWithResult("DnD Create Weapon Wheel", pnum, 1);
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