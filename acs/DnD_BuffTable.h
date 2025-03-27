#ifndef DND_BUFFTABLE_IN
#define DND_BUFFTABLE_IN

enum {
    BTI_KILLSTORM,
    BTI_MINIGUN,
    BTI_MPPB,
    BTI_RIOTGUN,
    BTI_HAMMER,
    BTI_DEVASTATOR,

    // add all curses below this one
    BTI_GURU,
    BTI_HIEROPHANT_SLOW,
    BTI_HIEROPHANT_DAMAGETAKEN,
    BTI_LICHICECURSE
};
#define DND_CURSE_BEGIN BTI_GURU

enum {
    BTI_F_USETARGET         = 0b1,
    BTI_F_REMOVE            = 0b10,
    BTI_F_USEINITIALSOURCE  = 0b100,
};

// by default assumes the source of buff to be activator of the script calling this, initiator may not always be activator of script
// returns duration for blends
int HandlePlayerBuffAssignment(int pnum, int initiator, int buff_table_index, int script_flags = 0, int update = 0) {
    int ptid = ActivatorTID();

    int bsource = 0;
    int btype = 0;
    int bvalue = 0;
    int bflags = 0;
    int bduration = 0;

    int tic_duration = 0;

    if(script_flags & BTI_F_USEINITIALSOURCE)
        bsource = initiator;
    else
        bsource = ptid;

    switch(buff_table_index) {
        case BTI_KILLSTORM:
        case BTI_MINIGUN:
        case BTI_MPPB:
        case BTI_RIOTGUN:
        case BTI_DEVASTATOR:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_WEAPONSOURCE | BUFF_F_NODUPLICATE | BUFF_F_DETRIMENTAL;
            bvalue = 0.25;
        break;
        case BTI_HAMMER:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_WEAPONSOURCE | BUFF_F_NODUPLICATE | BUFF_F_DETRIMENTAL;
            bvalue = 0.5;
        break;

        // curses
        case BTI_GURU:
            btype = BUFF_DAMAGEDEALT;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE | BUFF_F_DETRIMENTAL;
            bvalue = 0.7; // 70% less
            bduration = 2;
            tic_duration = bduration * TICRATE;
        break;
        case BTI_LICHICECURSE:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_MORETYPE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS | BUFF_F_DETRIMENTAL;
            bvalue = 0.2;
            bduration = 35;
            tic_duration = 25 * 5; // the curse inflicts itself over 5 times of 25 tics so the blend should match
        break;
    }

    bduration = SetDuration(bduration, bflags & BUFF_F_DURATIONINTICS);

    if(!(script_flags & BTI_F_REMOVE)) {
        if(update) {
            // update with this if there is
            update = update * 1.0 / 100;
            bflags |= BUFF_F_REPLACEMENTVALUE;
        }
        GivePlayerBuff(pnum, bsource, btype, bvalue, bflags, bduration, update);
    }
    else {
        if(update) {
            // replace value here for removal
            bvalue = update * 1.0 / 100;
            bflags |= BUFF_F_REPLACEMENTVALUE;
        }
        RemoveBuffMatching(pnum, bsource, btype, bvalue, bflags);
    }

    return tic_duration;
}

Script "DnD Player Buff" (int buff_table_index, int script_flags, int update) {
    int initiator = ActivatorTID();

    // set the target up
    if(script_flags & BTI_F_USETARGET)
        SetActivatorToTarget(0);

    int pnum = PlayerNumber();

    if(!IsPlayerBuffStateOK(pnum))
        Terminate;

    HandlePlayerBuffAssignment(pnum, initiator, buff_table_index, update);

    SetResultValue(0);
}

#endif