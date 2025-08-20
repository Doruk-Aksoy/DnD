#ifndef HUDANIM_IN
#define HUDANIM_IN

enum {
    DND_ANIM_DOOMGUYPUNCH,
    DND_ANIM_DOOMGUYKICK,
    DND_ANIM_WANDERERSPARKLES
};

Script "DnD Hud Animation" (int anim_id) CLIENTSIDE {
    if(PlayerNumber() != ConsolePlayerNumber())
        Terminate;

    int frame_id = 0;
    int frame_count = 0;
    str toShow = "";

    int hudx = 0, hudy = 0;
    int animx = 0, animy = 0;

    // done this way for now to not worry with array and storing the whole animation data, later I'll replace this with a more generic method when it'll be more common place
    switch(anim_id) {
        case DND_ANIM_DOOMGUYPUNCH:
            hudx = 320;
            hudy = 200;
            SetHudsize(hudx, hudy, 0);
            frame_count = 15;

            for(; frame_id < frame_count; ++frame_id) {
                animx = GetHudLeft(hudx) << 16;
                switch(frame_id) {
                    case 0:
                        toShow = "DGPUN0";
                    break;
                    case 1:
                        toShow = "DGPUN1";
                    break;
                    case 2:
                        toShow = "DGPUN2";
                    break;
                    case 3:
                        toShow = "DGPUN3";
                    break;
                    case 4:
                        toShow = "DGPUN4";
                    break;
                    case 5:
                    case 6:
                        toShow = "DGPUN5";
                    break;
                    case 7:
                    case 8:
                        toShow = "DGPUN6";
                    break;
                    case 9:
                        toShow = "DGPUN7";
                    break;
                    case 10:
                        toShow = "DGPUN8";
                    break;
                    case 11:
                        toShow = "DGPUN9";
                    break;
                    case 12:
                        toShow = "DGPUN10";
                    break;
                    case 13:
                        toShow = "DGPUN11";
                    break;
                    case 14:
                        toShow = "DGPUN12";
                    break;
                }
                SetFont(toShow);
                HudMessage(s:"A"; HUDMSG_PLAIN, HUDANIMATED_ID, CR_WHITE, animx + 100.4, animy + 36.1, 1.0);
                Delay(const:1);
            }
        break;

        case DND_ANIM_DOOMGUYKICK:
            hudx = 320;
            hudy = 200;
            SetHudsize(hudx, hudy, 0);
            frame_count = 15;
            animx = GetHudLeft(hudx) << 16;

            for(; frame_id < frame_count; ++frame_id) {
                switch(frame_id) {
                    case 0:
                        toShow = "DGKICK0";
                        animx -= 1.0;
                        animy -= 10.0;
                    break;
                    case 1:
                        toShow = "DGKICK1";
                        animx -= 2.0;
                        animy -= 11.0;
                    break;
                    case 2:
                        toShow = "DGKICK2";
                        animx -= 4.0;
                        animy -= 10.0;
                    break;
                    case 3:
                    case 4:
                        toShow = "DGKICK3";
                        animx -= 5.0;
                        animy -= 11.0;
                    break;
                    case 5:
                    case 6:
                        toShow = "DGKICK4";
                        animx -= 5.0;
                        animy -= 3.0;
                    break;
                    case 7:
                        toShow = "DGKICK4";
                        animx += 3.0;
                        animy += 2.0;
                    break;
                    case 8:
                        toShow = "DGKICK4";
                        animx += 1.0;
                        animy += 1.0;
                    break;
                    case 9:
                    case 10:
                        toShow = "DGKICK3";
                        animx += 5.0;
                        animy += 2.0;
                    break;
                    case 11:
                        toShow = "DGKICK2";
                        animx += 7.0;
                        animy += 3.0;
                    break;
                    case 12:
                        toShow = "DGKICK1";
                        animx += 6.0;
                        animy += 7.0;
                    break;
                    case 13:
                        toShow = "DGKICK0";
                        animx += 5.0;
                        animy += 9.0;
                    break;
                    case 14:
                        toShow = "DGKICK0";
                        animx += 4.0;
                        animy += 11.0;
                    break;
                }
                SetFont(toShow);
                HudMessage(s:"A"; HUDMSG_PLAIN, HUDANIMATED_ID, CR_WHITE, animx + 276.0, animy + 196.0, 1.0);
                Delay(const:1);
            }
        break;

        case DND_ANIM_WANDERERSPARKLES:
            hudx = 800;
            hudy = 600;
            frame_count = 10;

            animx = 8;
            
            for(; frame_id < frame_count; ++frame_id) {
                ACS_NamedExecuteWithResult("DnD Wanderer Sparkle", hudx, hudy, animx * frame_id);
                ACS_NamedExecuteWithResult("DnD Wanderer Sparkle", hudx, hudy, animx * frame_id + 1);
                ACS_NamedExecuteWithResult("DnD Wanderer Sparkle", hudx, hudy, animx * frame_id + 2);
                ACS_NamedExecuteWithResult("DnD Wanderer Sparkle", hudx, hudy, animx * frame_id + 3);

                ACS_NamedExecuteWithResult("DnD Wanderer Sparkle", hudx, hudy, animx * frame_id + 4, 1);
                ACS_NamedExecuteWithResult("DnD Wanderer Sparkle", hudx, hudy, animx * frame_id + 5, 1);
                ACS_NamedExecuteWithResult("DnD Wanderer Sparkle", hudx, hudy, animx * frame_id + 6, 1);
                ACS_NamedExecuteWithResult("DnD Wanderer Sparkle", hudx, hudy, animx * frame_id + 7, 1);
                Delay(const:1);
            }
        break;
    }

    HudMessage(s:""; HUDMSG_PLAIN, HUDANIMATED_ID, CR_WHITE, 0, 0, 1.0);
    SetResultValue(0);
}

Script "DnD Wanderer Sparkle" (int hudx, int hudy, int id_offset, int side) CLIENTSIDE {
    SetHudsize(hudx, hudy, 0);
    int frame_count = 15;

    int animx = random(120, 345) << 16;
    if(!side)
        animx += (GetHudLeft(hudx) << 16);
    else
        animx = (GetHudRight(hudx) << 16) - animx;

    int animy = (hudy << 16) + (random(-152, -18) << 16);
    
    for(int frame_id = 0; frame_id < frame_count; ++frame_id) {
        str img = StrParam(s:"SPRKL", d:frame_id / 3);
        int a = 0.33 + cos(0.05 * (frame_id + 1));
        SetFont(img);
        HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA, HUDANIMATED_ID + id_offset, CR_WHITE, animx + 0.4, animy, 0.1, a);
        Delay(const:1);
    }

    SetResultValue(0);
}

#endif