#ifndef DND_BUFFTABLE_IN
#define DND_BUFFTABLE_IN

enum {
    BTI_KILLSTORM,
    BTI_MINIGUN,
    BTI_MPPB,
    BTI_RIOTGUN,
    BTI_HAMMER,
    BTI_DEVASTATOR,

    // all buffs below this one
    BTI_PHASING,
    BTI_AMPHETAMINE,
    BTI_FRENZYCHARGE,
    BTI_ENDURANCECHARGE,
    BTI_POWERCHARGE,
    // flask sourced buffs
    BTI_ARMOR_FLASK,
    BTI_MOREARMOR_FLASK,
    BTI_ELEMENTALRES_FLASK,
    BTI_ENERGYRES_FLASK,
    BTI_PHYSRES_FLASK,
    BTI_MAGICRES_FLASK,
    BTI_CRITINCREASE_FLASK,
    BTI_SILVERIMBUE_FLASK,
    BTI_DMGINCREASE_FLASK,
    BTI_MOVESPEED_FLASK,
    BTI_MITIGATION_FLASK,

    // add all debuffs below this one
    BTI_OTHERWORDLYGRIP,
    BTI_CHILL,
    BTI_FREEZE,
    BTI_OVERLOAD,
    BTI_CRIPPLE,

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

#define DND_GRANITE_ARMORBUFF 1000
#define DND_BASALT_ARMORBUFF 0.3
#define DND_BISMUTH_BUFF 35.0
#define DND_INSULAR_BUFF 35.0
#define DND_OAK_BUFF 35.0
#define DND_ARCANE_BUFF 35.0
#define DND_DIAMOND_BUFF 0.7
#define DND_SULPHUR_BUFF 0.4
#define DND_QUICKSILVER_BUFF 0.2
#define DND_QUARTZ_BUFF 0.2

// by default assumes the source of buff to be activator of the script calling this, initiator may not always be activator of script
// returns duration for blends
int HandlePlayerBuffAssignment(int pnum, int initiator, int buff_table_index, int script_flags = 0, int update = 0, int new_duration = 0, int inc_effect = 0) {
    int ptid = pnum + P_TIDSTART;

    int bsource = 0;
    int btype = 0;
    int bvalue = 0;                 // make sure to send negative values if they are detrimental effects (detrimental to the buff label)
    int bflags = 0;
    int bduration = 0;

    int tic_duration = 0;

    bool isShotgunSlow = false;

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

            if(buff_table_index == BTI_KILLSTORM || buff_table_index == BTI_RIOTGUN)
                isShotgunSlow = true;
        break;
        case BTI_HAMMER:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_WEAPONSOURCE | BUFF_F_NODUPLICATE;
            bvalue = -0.5;
        break;

        // buffs
        case BTI_PHASING:
            btype = BUFF_PHASING;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS;
            bvalue = 0.1;

            if(!new_duration)
                bduration = 4 * TICRATE;
            else
                bduration = new_duration * TICRATE;

            tic_duration = GetPlayerAttributeValue(pnum, INV_IMP_PHASINGTIME);
            if(HasActorClassPerk_Fast(ptid, "Trickster", 4))
                tic_duration += DND_TRICKSTER_ACROBAT_PHASINGBONUS;

            bduration = bduration * (100 + tic_duration) / 100;

            tic_duration = bduration;
        break;
        case BTI_AMPHETAMINE:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS;
            bvalue = 0.33;
            bduration = 30;
            tic_duration = bduration * TICRATE;
        break;
        case BTI_FRENZYCHARGE:
            btype = BUFF_FRENZYCHARGE;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE | BUFF_F_ADDIFNODUPLICATE;

            // this check may seem redundant but its needed as the extra can be 0 which is DND_CHARGE_FRENZY
            tic_duration = GetPlayerAttributeValue(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF);
            if(!tic_duration || (tic_duration && GetPlayerAttributeExtra(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF) != DND_CHARGE_FRENZY)) {
                bvalue = DND_FRENZYCHARGE_BONUS;
                bduration = GetPlayerChargeDuration(pnum);
                tic_duration = bduration * TICRATE;

                // set bvalue to 0 for refreshing duration
                if(CanActorHaveFrenzyCharges(ptid, pnum))
                    GiveActorFrenzyCharge(ptid, 1);
                else
                    bvalue = 0;
            }
            else {
                bflags ^= BUFF_F_ADDIFNODUPLICATE | BUFF_F_NODUPLICATE_STRICT;
                bduration = 0;
                tic_duration = GetPlayerMaxFrenzyCharges(pnum);
                bvalue = DND_FRENZYCHARGE_BONUS * tic_duration;
                GiveActorFrenzyCharge(ptid, tic_duration, true);
                tic_duration = 0;
            }
        break;
        case BTI_ENDURANCECHARGE:
            btype = BUFF_ENDURANCECHARGE;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE | BUFF_F_ADDIFNODUPLICATE;

            tic_duration = GetPlayerAttributeValue(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF);
            if(!tic_duration || (tic_duration && GetPlayerAttributeValue(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF) && GetPlayerAttributeExtra(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF) != DND_CHARGE_ENDURANCE)) {
                bvalue = -DND_ENDURANCECHARGE_BONUS;
                bduration = GetPlayerChargeDuration(pnum);
                tic_duration = bduration * TICRATE;

                if(CanActorHaveEnduranceCharges(ptid, pnum))
                    GiveActorEnduranceCharge(ptid, 1);
                else
                    bvalue = 0;
            }
            else {
                bflags ^= BUFF_F_ADDIFNODUPLICATE | BUFF_F_NODUPLICATE_STRICT;
                bduration = 0;
                tic_duration = GetPlayerMaxEnduranceCharges(pnum);
                bvalue = -DND_ENDURANCECHARGE_BONUS * tic_duration;
                GiveActorEnduranceCharge(ptid, tic_duration, true);
                tic_duration = 0;
            }
        break;
        case BTI_POWERCHARGE:
            btype = BUFF_POWERCHARGE;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_UNIQUETOCLASS | BUFF_F_ADDIFNODUPLICATE;

            tic_duration = GetPlayerAttributeValue(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF);
            if(!tic_duration || (tic_duration && GetPlayerAttributeValue(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF) && GetPlayerAttributeExtra(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF) != DND_CHARGE_POWER)) {
                bvalue = DND_POWERCHARGE_BONUS;
                bduration = GetPlayerChargeDuration(pnum);
                tic_duration = bduration * TICRATE;

                if(CanActorHavePowerCharges(ptid, pnum))
                    GiveActorPowerCharge(ptid, 1);
                else
                    bvalue = 0;
            }
            else {
                bflags ^= BUFF_F_ADDIFNODUPLICATE | BUFF_F_NODUPLICATE_STRICT;
                bduration = 0;
                tic_duration = GetPlayerMaxPowerCharges(pnum);
                bvalue = DND_POWERCHARGE_BONUS * tic_duration;
                GiveActorPowerCharge(ptid, tic_duration, true);
                tic_duration = 0;
            }
        break;
        case BTI_ARMOR_FLASK:
            btype = BUFF_ARMORFLAT;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS;
            bvalue = DND_GRANITE_ARMORBUFF;

            if(inc_effect)
                bvalue = bvalue * (100 + inc_effect) / 100;

            if(!new_duration)
                bduration = 4 * TICRATE;
            else // supplied durations MUST have TICRATE factor in them
                bduration = new_duration;

            bduration = bduration * (100 + tic_duration) / 100;

            tic_duration = bduration;
        break;
        case BTI_MOREARMOR_FLASK:
            btype = BUFF_ARMORINCREASE;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS | BUFF_F_MORETYPE;
            bvalue = DND_BASALT_ARMORBUFF;

            if(inc_effect)
                bvalue = bvalue * (100 + inc_effect) / 100;

            if(!new_duration)
                bduration = 4 * TICRATE;
            else // supplied durations MUST have TICRATE factor in them
                bduration = new_duration;

            bduration = bduration * (100 + tic_duration) / 100;

            tic_duration = bduration;
        break;
        case BTI_ELEMENTALRES_FLASK:
            btype = BUFF_ELEMENTALRESIST;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS;
            bvalue = DND_BISMUTH_BUFF;

            if(inc_effect)
                bvalue = bvalue * (100 + inc_effect) / 100;

            if(!new_duration)
                bduration = 4 * TICRATE;
            else // supplied durations MUST have TICRATE factor in them
                bduration = new_duration;

            bduration = bduration * (100 + tic_duration) / 100;

            tic_duration = bduration;
        break;
        case BTI_ENERGYRES_FLASK:
            btype = BUFF_ENERGYRESIST;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS;
            bvalue = DND_INSULAR_BUFF;

            if(inc_effect)
                bvalue = bvalue * (100 + inc_effect) / 100;

            if(!new_duration)
                bduration = 4 * TICRATE;
            else // supplied durations MUST have TICRATE factor in them
                bduration = new_duration;

            bduration = bduration * (100 + tic_duration) / 100;

            tic_duration = bduration;
        break;
        case BTI_PHYSRES_FLASK:
            btype = BUFF_PHYSRESIST;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS;
            bvalue = DND_OAK_BUFF;

            if(inc_effect)
                bvalue = bvalue * (100 + inc_effect) / 100;

            if(!new_duration)
                bduration = 4 * TICRATE;
            else // supplied durations MUST have TICRATE factor in them
                bduration = new_duration;

            bduration = bduration * (100 + tic_duration) / 100;

            tic_duration = bduration;
        break;
        case BTI_MAGICRES_FLASK:
            btype = BUFF_MAGICRESIST;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS;
            bvalue = DND_ARCANE_BUFF;

            if(inc_effect)
                bvalue = bvalue * (100 + inc_effect) / 100;

            if(!new_duration)
                bduration = 4 * TICRATE;
            else // supplied durations MUST have TICRATE factor in them
                bduration = new_duration;

            bduration = bduration * (100 + tic_duration) / 100;

            tic_duration = bduration;
        break;
        case BTI_CRITINCREASE_FLASK:
            btype = BUFF_CRITPERCENT;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS;
            bvalue = DND_DIAMOND_BUFF;

            if(inc_effect)
                bvalue = bvalue * (100 + inc_effect) / 100;

            if(!new_duration)
                bduration = 4 * TICRATE;
            else // supplied durations MUST have TICRATE factor in them
                bduration = new_duration;

            bduration = bduration * (100 + tic_duration) / 100;

            tic_duration = bduration;
        break;
        case BTI_SILVERIMBUE_FLASK:
            btype = BUFF_SILVERIMBUE;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS;
            bvalue = 1;

            if(inc_effect)
                bvalue = bvalue * (100 + inc_effect) / 100;

            if(!new_duration)
                bduration = 4 * TICRATE;
            else // supplied durations MUST have TICRATE factor in them
                bduration = new_duration;

            bduration = bduration * (100 + tic_duration) / 100;

            tic_duration = bduration;
        break;
        case BTI_DMGINCREASE_FLASK:
            btype = BUFF_SULPHUR;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS;
            bvalue = DND_SULPHUR_BUFF;

            if(inc_effect)
                bvalue = bvalue * (100 + inc_effect) / 100;

            if(!new_duration)
                bduration = 4 * TICRATE;
            else // supplied durations MUST have TICRATE factor in them
                bduration = new_duration;

            bduration = bduration * (100 + tic_duration) / 100;

            tic_duration = bduration;
        break;
        case BTI_MOVESPEED_FLASK:
            btype = BUFF_SUPERMOVESPEED;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS;
            bvalue = DND_QUICKSILVER_BUFF;

            if(inc_effect)
                bvalue = bvalue * (100 + inc_effect) / 100;

            if(!new_duration)
                bduration = 4 * TICRATE;
            else // supplied durations MUST have TICRATE factor in them
                bduration = new_duration;

            bduration = bduration * (100 + tic_duration) / 100;

            tic_duration = bduration;
        break;
        case BTI_MITIGATION_FLASK:
            btype = BUFF_MITIGATION;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_DURATIONINTICS;
            bvalue = DND_QUARTZ_BUFF;

            // give the player the phasing buff alongside too
            HandlePlayerBuffAssignment(pnum, initiator, BTI_PHASING, 0, 0, new_duration ? new_duration : 4, inc_effect);

            if(inc_effect)
                bvalue = bvalue * (100 + inc_effect) / 100;

            if(!new_duration)
                bduration = 4 * TICRATE;
            else // supplied durations MUST have TICRATE factor in them
                bduration = new_duration;

            bduration = bduration * (100 + tic_duration) / 100;

            tic_duration = bduration;
        break;

        // curses
        case BTI_OTHERWORDLYGRIP:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE | BUFF_F_UNIQUETOCLASS | BUFF_F_MORETYPE;
            bvalue = -0.67;
            bduration = 2;
            tic_duration = bduration * TICRATE;
        break;

        case BTI_CHILL:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_ADDIFNODUPLICATE | BUFF_F_MORETYPE;
            bvalue = -0.15;
            bduration = 2;
            tic_duration = bduration * TICRATE;
        break;
        case BTI_FREEZE:
            btype = BUFF_STUN;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_MORETYPE;
            bvalue = -1.0;
            bduration = 2;
            tic_duration = bduration * TICRATE;
        break;
        case BTI_OVERLOAD:
            btype = BUFF_DAMAGETAKEN;
            bflags |= BUFF_F_MONSTERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_MORETYPE | BUFF_F_DURATIONINTICS;
            bvalue = 0.21;
            bduration = CheckActorInventory(ptid, "DnD_OverloadTimer");
            tic_duration = bduration;
        break;
        case BTI_CRIPPLE:
            GiveActorInventory(ptid, "CrippleToken", 1);

            btype = BUFF_SPEED;
            bflags |= BUFF_F_NODUPLICATE | BUFF_F_DURATIONINTICS;
            bvalue = -0.5;

            if(!new_duration)
                bduration = 2 * TICRATE;
            else
                bduration = new_duration * TICRATE;
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
            bduration = 42;
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
    if(bduration)
        bflags |= BUFF_F_TICKERREQUIRED;

    if(script_flags & BTI_F_ISCURSE) {
        bduration = bduration * (100 - GetPlayerAttributeValue(pnum, INV_REDUCEDCURSEDURATION)) / 100;
        if(bduration <= 0)
            return 0;
        tic_duration = tic_duration * (100 - GetPlayerAttributeValue(pnum, INV_REDUCEDCURSEDURATION)) / 100;
    }
    
    if(buff_table_index >= DND_BTI_DEBUFF_BEGIN && HasActorClassPerk_Fast(ptid, "Wanderer", DND_CLASSPERK_1)) {
        bduration = bduration * (100 - DND_WANDERER_PERK5_DEBUFFREDUCE) / 100;
        tic_duration = tic_duration * (100 - DND_WANDERER_PERK5_DEBUFFREDUCE) / 100;
    }

    if(!(script_flags & BTI_F_REMOVE)) {
        if(update) {
            // update with this if there is
            update = update * 1.0 / 100;
            bflags |= BUFF_F_REPLACEMENTVALUE;
        }

        if(isShotgunSlow)
            bvalue = bvalue * (100 - GetPlayerAttributeValue(pnum, INV_IMP_REDUCEDSLOWSHOTGUNS)) / 100;

        GivePlayerBuff(pnum, bsource, btype, buff_table_index, bvalue, bflags, bduration, update);
    }
    else {
        if(update) {
            // replace value here for removal
            bvalue = update * 1.0 / 100;
            bflags |= BUFF_F_REPLACEMENTVALUE;
        }

        if(isShotgunSlow)
            bvalue = bvalue * (100 - GetPlayerAttributeValue(pnum, INV_IMP_REDUCEDSLOWSHOTGUNS)) / 100;

        RemoveBuffMatching(pnum, bsource, btype, buff_table_index, bvalue, bflags);
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