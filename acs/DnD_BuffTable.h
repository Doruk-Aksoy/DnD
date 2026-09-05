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

    // powerup-sourced "more" damage buffs -- the buff ticker owns their lifetime.
    // KEEP THESE ABOVE THE DEBUFF MARKER: anything >= DND_BTI_DEBUFF_BEGIN gets its
    // duration cut by Wanderer perk 1.
    BTI_DOOMGUY_ORB,
    BTI_TRIPLEDAMAGE,
    BTI_RAVAGER_POWER,
    BTI_RALLY,
    BTI_RALLY_SPEED,
    BTI_LEECHINGDAMAGE,
    BTI_PUNISHER_DAMAGE,
    BTI_MARINE_DAMAGEREDUCTION,

    // sigil element powers -- keep contiguous and in DND_DAMAGECATEGORY order
    // (that order is the conversion ladder, so poison precedes lightning and fire is last)
    BTI_ELEMENTPOWER_ICE,
    BTI_ELEMENTPOWER_POISON,
    BTI_ELEMENTPOWER_LIGHTNING,
    BTI_ELEMENTPOWER_FIRE,

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
    BTI_GOLGOTH_WEAKEN,

    // KEEP LAST -- sizes the bt_index presence mask. Add new entries ABOVE this,
    // and above the debuff marker if the entry is a buff rather than a debuff.
    BTI_MAX
};
#define DND_BTI_DEBUFF_BEGIN BTI_OTHERWORDLYGRIP

enum {
    BTI_F_USETARGET         = 0b1,
    BTI_F_REMOVE            = 0b10,
    BTI_F_USEINITIALSOURCE  = 0b100,
    BTI_F_ISCURSE           = 0b1000,

    // Archangel Beacon. Set on a shared copy. This is the recursion guard: without it two wearers standing
    // together would hand the same buff back and forth forever, each application spawning another.
    BTI_F_ISSHARED          = 0b10000,
};

#define DND_GRANITE_ARMORBUFF 1000

// Buff durations, in seconds unless the buff sets BUFF_F_DURATIONINTICS.
#define DND_DOOMGUY_ORB_DURATION        10
#define DND_DOOMGUY_ORB_EXEC_DURATION   20
#define DND_TRIPLEDAMAGE_DURATION       (15 * TICRATE)
#define DND_TRIPLEDAMAGE2_DURATION      787
#define DND_TRIPLEDAMAGE_TIER2_EFFECT   75                  // 2.0 * 175/100 = 3.5 => x4.5
#define DND_RAVAGER_POWER_DURATION      4
#define DND_RALLY_BUFF_DURATION         8                   // must match RALLY_DURATION
#define DND_ELEMENTPOWER_DURATION       20
#define DND_BASALT_ARMORBUFF 0.3
#define DND_BISMUTH_BUFF 35.0
#define DND_INSULAR_BUFF 35.0
#define DND_OAK_BUFF 35.0
#define DND_ARCANE_BUFF 35.0
#define DND_DIAMOND_BUFF 0.7
#define DND_SULPHUR_BUFF 0.4
#define DND_QUICKSILVER_BUFF 0.33
#define DND_QUARTZ_BUFF 0.2

// by default assumes the source of buff to be activator of the script calling this, initiator may not always be activator of script
// returns duration for blends
// Archangel Beacon. Cost is the whole design constraint here: this sits on the path every buff in the
// game takes, and buffs are gained constantly.
//
//  - Nothing below runs unless the wearer actually holds the charm. For everyone else the cost is a
//    single array read at the call site, which is why the gate lives there and not in here.
//  - Single player leaves before the loop; there is nobody to share with.
//  - The walk is over MAXPLAYERS -- 64 ints -- not over actors, and every cheap test comes before
//    the distance measurement, which is the only expensive one.
//  - The copies carry BTI_F_ISSHARED, so a copy never shares again. That is what stops two wearers
//    standing together from bouncing one buff between them forever.
void ShareBuffToNearby(int pnum, int buff_table_index, int script_flags, int update) {
	if(GameType() == GAME_SINGLE_PLAYER)
		return;

	int potency = PlayerModData[pnum].vals[PSTAT_EX_SHAREDBUFF_POTENCY];
	int ptid = pnum + P_TIDSTART;
	int range = ScalePlayerAoERadius(pnum, DND_ARCHANGELBEACON_RANGE, DND_AOESRC_NONWEAPON);

	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(i == pnum || !PlayerInGame(i) || PlayerIsSpectator(i))
			continue;

		int otid = i + P_TIDSTART;
		if(!isActorAlive(otid) || fdistance(ptid, otid) > range)
			continue;

		HandlePlayerBuffAssignment(i, ptid, buff_table_index,
			script_flags | BTI_F_ISSHARED | BTI_F_USEINITIALSOURCE, update, 0, 0, potency);
	}
}

int HandlePlayerBuffAssignment(int pnum, int initiator, int buff_table_index, int script_flags = 0, int update = 0, int new_duration = 0, int inc_effect = 0, int shared_potency = 0) {
    // GivePlayerBuff and RemoveBuffMatching both WALK the buff list, and a virgin
    // pbuffs global has the dummy head linked to itself (see RemoveAllBuffs). The
    // "DnD Player Buff" script already gates on this, but the many direct callers
    // -- RestorePersistentBuffs, the Ravager spree, Punisher tiers, Rally, lifesteal,
    // the sigil powers -- do not, so the check belongs here too.
    if(!IsPlayerBuffStateOK(pnum))
        return 0;

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
                bduration = new_duration;

            tic_duration = PlayerModData[pnum].vals[PSTAT_IMP_PHASINGTIME];
            if(HasActorClassPerk_Fast(ptid, DND_PLAYER_TRICKSTER, 4))
                tic_duration += DND_TRICKSTER_ACROBAT_PHASINGBONUS;

            bduration = bduration * (100 + tic_duration) / 100;

            tic_duration = bduration;

            GiveActorInventory(ptid, "DnD_HasPhasing", 1);
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
            tic_duration = PlayerModData[pnum].vals[PSTAT_EX_COUNTASHAVINGMAXCHARGEOF];
            if(!tic_duration || (tic_duration && ReadPlayerModExtra(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF) != DND_CHARGE_FRENZY)) {
                bvalue = DND_FRENZYCHARGE_BONUS;
                bduration = GetPlayerChargeDuration(pnum, DND_CHARGE_FRENZY);
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

            tic_duration = PlayerModData[pnum].vals[PSTAT_EX_COUNTASHAVINGMAXCHARGEOF];
            if(!tic_duration || (tic_duration  && ReadPlayerModExtra(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF) != DND_CHARGE_ENDURANCE)) {
                bvalue = -DND_ENDURANCECHARGE_BONUS;
                bduration = GetPlayerChargeDuration(pnum, DND_CHARGE_ENDURANCE);
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

            tic_duration = PlayerModData[pnum].vals[PSTAT_EX_COUNTASHAVINGMAXCHARGEOF];
            if(!tic_duration || (tic_duration && ReadPlayerModExtra(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF) != DND_CHARGE_POWER)) {
                bvalue = DND_POWERCHARGE_BONUS;
                bduration = GetPlayerChargeDuration(pnum, DND_CHARGE_POWER);
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
            HandlePlayerBuffAssignment(pnum, initiator, BTI_PHASING, 0, 0, new_duration ? new_duration : 3, inc_effect);

            if(inc_effect)
                bvalue = bvalue * (100 + inc_effect) / 100;

            if(!new_duration)
                bduration = 3 * TICRATE;
            else // supplied durations MUST have TICRATE factor in them
                bduration = new_duration;

            bduration = bduration;
            tic_duration = bduration;
        break;

        // Powerup-sourced "more" damage buffs.
        // NODUPLICATE_STRICT => exactly one instance of each (a re-grant refreshes the
        // timer rather than stacking); MORETYPE => still combines multiplicatively with
        // every other buff.
        case BTI_DOOMGUY_ORB:
            btype = BUFF_DAMAGEDEALT;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_MORETYPE;
            bvalue = 0.25;                                  // DND_DOOMGUY_DMGBONUS 25 => x1.25
            // the plain orb is 10s, the execute variant 20s -- same value, so the strict
            // duplicate rule refreshes to the longer of the two
            if(!new_duration)
                bduration = DND_DOOMGUY_ORB_DURATION;
            else
                bduration = new_duration;
            tic_duration = bduration * TICRATE;
        break;

        case BTI_TRIPLEDAMAGE:
            btype = BUFF_DAMAGEDEALT;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_MORETYPE | BUFF_F_DURATIONINTICS;
            bvalue = 2.0;                                   // x3; tier 2 sends inc_effect 75 => 3.5 => x4.5
            // tier is fully determined by inc_effect, so the caller only needs one arg
            if(inc_effect) {
                bvalue = bvalue * (100 + inc_effect) / 100;
                bduration = DND_TRIPLEDAMAGE2_DURATION;
            }
            else
                bduration = DND_TRIPLEDAMAGE_DURATION;

            if(new_duration) // supplied durations MUST already be in tics
                bduration = new_duration;
            tic_duration = bduration;
        break;

        case BTI_RALLY:
            btype = BUFF_DAMAGEDEALT;
            // NODUPLICATE_STRICT is the whole point here: Rally is cast ON OTHER
            // PLAYERS, so several casters can land it on the same target. Strict keeps
            // exactly one node and lets a stronger cast overwrite a weaker one, while a
            // weaker cast on top of a stronger one is ignored. Equal strength just
            // refreshes the timer.
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_MORETYPE;

            // inc_effect carries BOTH halves of the spell, already resolved from the
            // caster's level: damage percent in the low 16 bits, speed percent in the
            // high 16. One int is all HandlePlayerBuffAssignment has spare, and packing
            // beats threading a second parameter through every call site. Resolving the
            // level curves at the call site also keeps DnD_SkillDef.h out of the buff
            // table, which is included well before it.
            bvalue = (inc_effect & 0xFFFF) * 1.0 / 100;

            bduration = DND_RALLY_BUFF_DURATION;
            tic_duration = bduration * TICRATE;

            // the speed half rides along as its own node, so it expires on the same
            // ticker and obeys the same strongest-caster-wins rule
            HandlePlayerBuffAssignment(pnum, initiator, BTI_RALLY_SPEED, script_flags, 0, 0, inc_effect);
        break;

        case BTI_LEECHINGDAMAGE:
            btype = BUFF_DAMAGEDEALT;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_MORETYPE;

            // inc_effect carries INV_LIFESTEAL_DAMAGE's raw 16.16 delta, so the value
            // keeps its sub-percent precision rather than rounding to whole percent
            bvalue = inc_effect;

            // bduration stays 0 on purpose: BUFF_F_TICKERREQUIRED is only set when a
            // duration exists, so this buff has no timer. "DnD Lifesteal Script" owns
            // its lifetime and drops it in its teardown; ResetPlayerBuffs is the
            // backstop on death and map change.
        break;

        // Sigil element powers. One case for all four -- they differ only in which
        // buff type they land in, and the BTI/BUFF orders are kept parallel so the
        // mapping is one subtraction.
        case BTI_ELEMENTPOWER_FIRE:
        case BTI_ELEMENTPOWER_ICE:
        case BTI_ELEMENTPOWER_LIGHTNING:
        case BTI_ELEMENTPOWER_POISON:
            btype = DND_FIRST_ELEMENTAL_DMGBUFF + (buff_table_index - BTI_ELEMENTPOWER_ICE);
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_MORETYPE;

            // DND_SIGIL_BUFF is 100, i.e. x2 on the matching element
            bvalue = DND_SIGIL_BUFF * 1.0 / 100;

            bduration = DND_ELEMENTPOWER_DURATION;
            tic_duration = bduration * TICRATE;
        break;

        case BTI_MARINE_DAMAGEREDUCTION:
            btype = BUFF_DAMAGETAKEN;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_MORETYPE;
            bvalue = DND_MARINE_50REDUCTION;
        break;

        case BTI_PUNISHER_DAMAGE:
            btype = BUFF_DAMAGEDEALT;
            // The tier only ever climbs (DnD.bcs guards with "if(is_demon < temp)")
            // and resets on death, so strongest-wins is exactly the tier-up rule: a
            // higher tier replaces the node, a stale lower one is ignored.
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_MORETYPE;

            // inc_effect carries the already-computed 16.16 delta
            bvalue = inc_effect;

            // no duration -- it lives until death or map change, where
            // ResetPlayerBuffs clears it alongside the HUD's tier item
        break;

        case BTI_RALLY_SPEED:
            btype = BUFF_SPEED;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_MORETYPE;

            // high half of the packed pair; +0.08 reproduces PowerSpeed "Speed 1.08"
            // exactly, since HandleBuffApplication FixedMuls the base speed by it
            bvalue = (inc_effect >> 16) * 1.0 / 100;

            bduration = DND_RALLY_BUFF_DURATION;
            tic_duration = bduration * TICRATE;
        break;

        case BTI_RAVAGER_POWER:
            btype = BUFF_DAMAGEDEALT;
            bflags |= BUFF_F_PLAYERSOURCE | BUFF_F_NODUPLICATE_STRICT | BUFF_F_MORETYPE;
            bvalue = 0.25;                                  // DND_RAVAGER_DMGBONUS 25 => x1.25
            bduration = DND_RAVAGER_POWER_DURATION;
            tic_duration = bduration * TICRATE;
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
        bduration = bduration * (100 - PlayerModData[pnum].vals[PSTAT_REDUCEDCURSEDURATION]) / 100;
        if(bduration <= 0)
            return 0;
        tic_duration = tic_duration * (100 - PlayerModData[pnum].vals[PSTAT_REDUCEDCURSEDURATION]) / 100;
    }
    
    if(buff_table_index >= DND_BTI_DEBUFF_BEGIN && HasActorClassPerk_Fast(ptid, DND_PLAYER_WANDERER, DND_CLASSPERK_1)) {
        bduration = bduration * (100 - DND_WANDERER_PERK5_DEBUFFREDUCE) / 100;
        tic_duration = tic_duration * (100 - DND_WANDERER_PERK5_DEBUFFREDUCE) / 100;
    }

    // Archangel Beacon. The wearer's own buffs run short. Not applied to a curse -- shortening those would
    // be an upside -- and not to a shared copy, which is somebody else's buff and pays its own way.
    if(!(script_flags & (BTI_F_ISCURSE | BTI_F_ISSHARED))) {
        int bw_short = PlayerModData[pnum].vals[PSTAT_EX_BUFF_SHORTER];
        if(bw_short > 0) {
            bduration = bduration * (100 - bw_short) / 100;
            tic_duration = tic_duration * (100 - bw_short) / 100;
        }
    }

    if(!(script_flags & BTI_F_REMOVE)) {
        if(update) {
            // update with this if there is
            update = update * 1.0 / 100;
            bflags |= BUFF_F_REPLACEMENTVALUE;
        }

        if(isShotgunSlow)
            bvalue = bvalue * (100 - PlayerModData[pnum].vals[PSTAT_IMP_REDUCEDSLOWSHOTGUNS]) / 100;

        // Archangel Beacon. A shared copy is weaker. Scaled here rather than through inc_effect because
        // that parameter is only read by a handful of the cases above -- this reaches every buff.
        if(shared_potency > 0)
            bvalue = bvalue * shared_potency / 100;

        GivePlayerBuff(pnum, bsource, btype, buff_table_index, bvalue, bflags, bduration, update);

        // Archangel Beacon. After the wearer's own copy has landed, and only for something that can be
        // shared safely:
        //
        //   BUFF_F_TICKERREQUIRED -- the buff expires ON ITS OWN. Everything without it is ended by
        //     an explicit RemoveBuffMatching when some condition lapses, and that call goes to the
        //     player whose condition it was. An ally who caught a copy and then walked away would
        //     never be told to drop it, so a charge or a stance would sit on them permanently.
        //   bduration > 0 -- belt and braces on the same point: a ticking buff with no time on it
        //     has nothing to count down.
        //   bvalue > 0 -- this table stores the weapon slows as NEGATIVE values, and handing an ally
        //     a movement slow every time the wearer fires a minigun is not support.
        if((bflags & BUFF_F_TICKERREQUIRED) && bduration > 0 && bvalue > 0
            && !(script_flags & (BTI_F_ISCURSE | BTI_F_ISSHARED))
            && PlayerModData[pnum].vals[PSTAT_EX_SHAREDBUFF_POTENCY] > 0)
            ShareBuffToNearby(pnum, buff_table_index, script_flags, update);
    }
    else {
        if(update) {
            // replace value here for removal
            bvalue = update * 1.0 / 100;
            bflags |= BUFF_F_REPLACEMENTVALUE;
        }

        if(isShotgunSlow)
            bvalue = bvalue * (100 - PlayerModData[pnum].vals[PSTAT_IMP_REDUCEDSLOWSHOTGUNS]) / 100;

        RemoveBuffMatching(pnum, bsource, btype, buff_table_index, bvalue, bflags);
    }

    return tic_duration;
}

// Load-time check that the bt_index presence mask in buffData_T is still wide enough
// for the BTI enum. The two cannot be tied together at compile time: this file is
// included at the BOTTOM of DnD_Buffs.h, so BTI_MAX does not exist yet when the struct
// is declared. Adding entries to the enum is routine, so this shouts rather than
// letting HasPlayerBuff silently start answering "no" for the entries past the end.
// Silent when correct.
void VerifyBuffMaskCapacity() {
    if(BTI_MAX > DND_BTI_MASK_BITS)
        Log(s:"DnD SETUP ERROR: BTI_MAX is ", d:BTI_MAX, s:" but the buff presence mask only holds ", d:DND_BTI_MASK_BITS, s:" bits -- raise DND_BTI_MASK_WORDS in DnD_Buffs.h to ", d:(BTI_MAX + 31) / 32);
}

// inc_effect is optional -- ACS defaults omitted args to 0, so the existing
// 1- and 2-arg callers are unaffected
Script "DnD Player Buff" (int buff_table_index, int script_flags, int update, int inc_effect) {
    int initiator = ActivatorTID();

    // set the target up
    if(script_flags & BTI_F_USETARGET)
        SetActivatorToTarget(0);

    int pnum = PlayerNumber();

    if(!IsPlayerBuffStateOK(pnum))
        Terminate;

    HandlePlayerBuffAssignment(pnum, initiator, buff_table_index, script_flags, update, 0, inc_effect);

    SetResultValue(0);
}

#endif