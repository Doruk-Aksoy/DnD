#ifndef DND_BUFFTABLE_IN
#define DND_BUFFTABLE_IN

enum {
    BTI_KILLSTORM,
    BTI_MINIGUN,
    BTI_MPPB,
    BTI_RIOTGUN,
    BTI_RIOTGUN2,
    BTI_HAMMER,
    BTI_DEVASTATOR,
    BTI_DEVASTATOR2
};

enum {
    BTI_F_USETARGET         = 0b1,
    BTI_F_REMOVE            = 0b10,
};

Script "DnD Player Buff" (int buff_table_index, int script_flags) {
    if(script_flags & BTI_F_USETARGET)
        SetActivatorToTarget(0);

    int pnum = PlayerNumber();

    if(!IsPlayerBuffStateOK(pnum))
        Terminate;

    int ptid = ActivatorTID();

    int bsource = 0;
    int btype = 0;
    int bvalue = 0;
    int bflags = 0;
    int bduration = 0;

    switch(buff_table_index) {
        case BTI_KILLSTORM:
        case BTI_MINIGUN:
        case BTI_MPPB:
        case BTI_RIOTGUN:
        case BTI_DEVASTATOR:
            bsource = ptid;
            btype = BUFF_SLOW;
            bflags = BUFF_F_WEAPONSOURCE | BUFF_F_NODUPLICATE;
            bvalue = 0.25;
        break;
        case BTI_RIOTGUN2:
            bsource = ptid;
            btype = BUFF_SLOW;
            bflags = BUFF_F_WEAPONSOURCE | BUFF_F_NODUPLICATE;
            bvalue = 0.35;
        break;
        case BTI_HAMMER:
            bsource = ptid;
            btype = BUFF_SLOW;
            bflags = BUFF_F_WEAPONSOURCE | BUFF_F_NODUPLICATE;
            bvalue = 0.5;
        break;
        case BTI_DEVASTATOR2:
            bsource = ptid;
            btype = BUFF_SLOW;
            bflags = BUFF_F_WEAPONSOURCE | BUFF_F_NODUPLICATE;
            bvalue = 0.85;
        break;
    }

    if(!(script_flags & BTI_F_REMOVE))
        GivePlayerBuff(pnum, bsource, btype, bvalue, bflags, bduration);
    else
        RemoveBuffMatching(pnum, bsource, btype, bvalue, bflags);

    SetResultValue(0);
}

#endif