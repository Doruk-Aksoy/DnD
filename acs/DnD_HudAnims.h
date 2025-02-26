#ifndef HUDANIM_IN
#define HUDANIM_IN

enum {
    DND_ANIM_DOOMGUYPUNCH
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
    }
    HudMessage(s:""; HUDMSG_PLAIN, HUDANIMATED_ID, CR_WHITE, 0, 0, 1.0);
    SetResultValue(0);
}

#endif