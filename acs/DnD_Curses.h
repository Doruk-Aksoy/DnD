#ifndef DND_CURSES_IN
#define DND_CURSES_IN

enum {
    DND_CURSE_GURU,
    DND_CURSE_HIEROPHANT,
    DND_CURSE_LICHICE
};

enum {
    CURSE_F_OWNERISTARGET = 1,
    CURSE_F_PLAYERISTRACER = 2
};

void HandleCurseFade(int player_tid, int origin_tid, int r, int g, int b, int intensity, int duration, int curse_effect) {
    // curse has a color component attached
    if(intensity && !CheckActorInventory(player_tid, "IsBlinded")) {
        if(HasPlayerPowerset(player_tid - P_TIDSTART, PPOWER_SYNTHMETALMASK))
            intensity /= DND_SYNTHMASK_EFFECT;

        duration = duration * curse_effect / 100;

        SetActivator(player_tid);
        FadeRange(r, g, b, intensity, 0, 0, 0, 0, duration * 1.0 / TICRATE);
        SetActivator(origin_tid);
    }
}

Script "DnD Monster Curse" (int curse_id, int curse_flags) {
    int player_tid = 0;
    if(curse_flags & CURSE_F_PLAYERISTRACER)
        player_tid = GetActorProperty(0, APROP_TRACERTID);
    else
        player_tid = GetActorProperty(0, APROP_TARGETTID);

    int pnum = player_tid - P_TIDSTART;

    //Log(s:"pnum ", d:pnum, s: " ", d:player_tid);

    if(!IsPlayerBuffStateOK(pnum))
        Terminate;

    if(curse_flags & CURSE_F_OWNERISTARGET)
        SetActivatorToTarget(0);

    int i, temp = 0;
    int this = ActivatorTID();

    //printbold(s:"isplayer ", d:IsPlayer(player_tid), s: " ", d:CheckUniquePropertyOnPlayer(pnum, PUP_CURSEIMMUNITY));

    int curse_effect = 100; // player's curse effect thing goes here

	if(IsPlayer(player_tid) && !CheckUniquePropertyOnPlayer(pnum, PUP_CURSEIMMUNITY)) {
        int intensity = 0;
        int duration = 0;
        int r = 0, g = 0, b = 0;
        switch(curse_id) {
            case DND_CURSE_GURU:
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_GURU);
                intensity = 0.5;
                r = 102;
                g = 102;
                b = 102;
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_CURSE_LICHICE:
                intensity = 0.8;
                r = 208;
                g = 214;
                b = 254;
                PlaySound(player_tid, "Lich/IceCurse", 6);
                for(i = 0; i < 5; i++) {
                    temp = 20 * (i + 1);
                    duration = HandlePlayerBuffAssignment(pnum, this, BTI_LICHICECURSE, 0, temp);
                    // only proc on first instance
                    if(!i)
                        HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
                    Delay(25 * curse_effect / 100);
                }
                HandlePlayerBuffAssignment(pnum, this, BTI_LICHICECURSE, BTI_F_REMOVE, temp);
            break;
        }
	}
    SetResultValue(0);
}

#endif