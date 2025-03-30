#ifndef DND_BUFFTABLE_IN
#define DND_BUFFTABLE_IN

enum {
    BTI_KILLSTORM,
    BTI_MINIGUN,
    BTI_MPPB,
    BTI_RIOTGUN,
    BTI_HAMMER,
    BTI_DEVASTATOR,

    // add all debuffs below this one
    BTI_OTHERWORDLYGRIP,

    BTI_REDLICH,
    BTI_PALADIN,
    BTI_GURU,
    BTI_HIEROPHANT_SLOW,
    BTI_HIEROPHANT_DAMAGETAKEN,
    BTI_FLESHWIZARD_SLOW1,
    BTI_FLESHWIZARD_SLOW2,
    BTI_FLESHWIZARD_WEAKEN,
    BTI_FLESHWIZARD_SNARE,
    BTI_STOMPSLOW,
    BTI_LICHICECURSE,
    BTI_LICHVISION,
    BTI_LICHDEGEN,

    BTI_SSRATHSTUN,
    BTI_ERYXIASLOW,

    BTI_TORRASQUE_SNARE,
    BTI_GOLGOTH_SLOW,
    BTI_GOLGOTH_WEAKEN
};
#define DND_BTI_DEBUFF_BEGIN BTI_OTHERWORDLYGRIP

enum {
    BTI_F_USETARGET         = 0b1,
    BTI_F_REMOVE            = 0b10,
    BTI_F_USEINITIALSOURCE  = 0b100,
    BTI_F_ISCURSE           = 0b1000,
};

// by default assumes the source of buff to be activator of the script calling this, initiator may not always be activator of script
// returns duration for blends
int HandlePlayerBuffAssignment(int pnum, int initiator, int buff_table_index, int script_flags = 0, int update = 0) {
    int ptid = ActivatorTID();

    int bsource = 0;
    int btype = 0;
    int bvalue = 0;                 // make sure to send negative values if they are detrimental effects (detrimental to the buff label)
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
            bflags |= BUFF_F_WEAPONSOURCE | BUFF_F_NODUPLICATE;
            bvalue = -0.25;
        break;
        case BTI_HAMMER:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_WEAPONSOURCE | BUFF_F_NODUPLICATE;
            bvalue = -0.5;
        break;

        // curses
        case BTI_OTHERWORDLYGRIP:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE;
            bvalue = -0.67;
            bduration = 2;
            tic_duration = bduration * TICRATE;
        break;

        case BTI_REDLICH:
            btype = BUFF_DAMAGETAKEN;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE;
            bvalue = 0.5;
            bduration = 30;
            tic_duration = bduration * TICRATE;
        break;
        case BTI_PALADIN:
            btype = BUFF_ARMORINCREASE;
            bflags |= BUFF_F_MONSTERSOURCE;
            bvalue = -0.1;
            bduration = 10;
            tic_duration = bduration * TICRATE;
        break;
        case BTI_GURU:
            btype = BUFF_DAMAGEDEALT;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE;
            bvalue = -0.7; // 70% less
            bduration = 3;
            tic_duration = bduration * TICRATE;
        break;
        case BTI_HIEROPHANT_SLOW:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE;
            bvalue = -0.5;
            bduration = 5;
        break;
        case BTI_HIEROPHANT_DAMAGETAKEN:
            // don't use detrimental here, as it'll flip damagetaken to be damagereduced
            btype = BUFF_DAMAGETAKEN;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE;
            bvalue = 0.5;
            bduration = 5;
            tic_duration = TICRATE * bduration + 10; // the last 10 helps clarify visual
        break;
        case BTI_FLESHWIZARD_SLOW1:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE;
            bvalue = -0.25;
            bduration = 2;
            tic_duration = bduration * TICRATE;
        break;
        case BTI_FLESHWIZARD_SLOW2:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE | BUFF_F_DURATIONINTICS;
            bvalue = -0.5;
            bduration = 52;
            tic_duration = bduration;
        break;
        case BTI_FLESHWIZARD_WEAKEN:
            btype = BUFF_DAMAGEDEALT;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS;
            bvalue = -0.25;
            bduration = 52;
            tic_duration = bduration;
        break;
        case BTI_FLESHWIZARD_SNARE:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE | BUFF_F_DURATIONINTICS;
            bvalue = -1.0;
            bduration = 17;
            tic_duration = bduration;
        break;
        case BTI_STOMPSLOW:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE;
            bvalue = -0.33;
            bduration = 4;
            tic_duration = bduration * TICRATE;
        break;
        case BTI_LICHICECURSE:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_MORETYPE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS;
            bvalue = -0.2;
            bduration = 35;
            tic_duration = 25 * 5; // the curse inflicts itself over 5 times of 25 tics so the blend should match
        break;
        case BTI_LICHVISION:
            btype = BUFF_VISIONLOSS;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_MORETYPE;
            bvalue = 1.0;
            bduration = 2;
            tic_duration = bduration * TICRATE + 8;
        break;
        case BTI_LICHDEGEN:
            btype = BUFF_HEALTHREGEN;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE;
            bvalue = -0.175;
            bduration = 5;
            tic_duration = bduration * TICRATE + 8;
        break;

        case BTI_SSRATHSTUN:
            btype = BUFF_STUN;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE | BUFF_F_DURATIONINTICS;
            bvalue = -1.0; // -1 to make speed 0
            bduration = 17;
            tic_duration = bduration + 7;
        break;
        case BTI_ERYXIASLOW:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE | BUFF_F_DURATIONINTICS;
            bvalue = -0.33;
            bduration = 56;
            tic_duration = bduration + 8;
        break;

        case BTI_TORRASQUE_SNARE:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE | BUFF_F_DURATIONINTICS;
            bvalue = -1.0;
            bduration = 45;
            tic_duration = bduration;
        break;
        case BTI_GOLGOTH_SLOW:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE | BUFF_F_DURATIONINTICS;
            bvalue = -0.5;
            bduration = 8;
            tic_duration = bduration;
        break;
        case BTI_GOLGOTH_WEAKEN:
            btype = BUFF_DAMAGEDEALT;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS;
            bvalue = -0.75;
            bduration = 6;
            tic_duration = bduration * TICRATE;
        break;
    }

    bduration = SetDuration(bduration, bflags & BUFF_F_DURATIONINTICS);

    if(script_flags & BTI_F_ISCURSE) {
        bduration = bduration * (100 - GetPlayerAttributeValue(pnum, INV_REDUCEDCURSEDURATION)) / 100;
        if(bduration <= 0)
            return 0;
        tic_duration = tic_duration * (100 - GetPlayerAttributeValue(pnum, INV_REDUCEDCURSEDURATION)) / 100;
    }

    if(buff_table_index >= DND_BTI_DEBUFF_BEGIN && CheckActorInventory(ptid, "Wanderer_Perk5")) {
        bduration = bduration * (100 - DND_WANDERER_PERK5_DEBUFFREDUCE) / 100;
        tic_duration = tic_duration * (100 - DND_WANDERER_PERK5_DEBUFFREDUCE) / 100;
    }

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

    HandlePlayerBuffAssignment(pnum, initiator, buff_table_index, script_flags, update);

    SetResultValue(0);
}

#endif