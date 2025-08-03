#ifndef DND_CURSES_IN
#define DND_CURSES_IN

// this is poorly named but it basically is a mapper for buff table

enum {
    DND_BUFF_PHASING,
    DND_BUFF_PHASING_DASH,
    DND_BUFF_AMPHETAMINE,

    DND_DEBUFF_OTHERWORDLYGRIP,

    DND_DEBUFF_CHILL,
    DND_DEBUFF_FREEZE,
    DND_DEBUFF_OVERLOAD,

    DND_DEBUFF_REDLICH,
    DND_DEBUFF_PALADIN,
    DND_DEBUFF_GURU,
    DND_DEBUFF_HIEROPHANT,
    DND_DEBUFF_FLESHWIZARD_SLOW,
    DND_DEBUFF_FLESHWIZARD_WEAKEN,
    DND_DEBUFF_FLESHWIZARD_SNARE,
    DND_DEBUFF_STOMPSLOW,
    DND_DEBUFF_LICHICE,
    DND_DEBUFF_LICHVISION,
    DND_DEBUFF_LICHDEGEN,

    DND_DEBUFF_SSRATHSTUN,
    DND_DEBUFF_ERYXIASLOW,

    DND_DEBUFF_TORRASQUE_SNARE,
    DND_DEBUFF_GOLGOTH_SLOW,
    DND_DEBUFF_GOLGOTH_WEAKEN
};
#define DND_DEBUFF_BEGIN DND_DEBUFF_OTHERWORDLYGRIP
#define DND_DEBUFF_END DND_DEBUFF_GOLGOTH_WEAKEN

enum {
    DEBUFF_F_OWNERISTARGET = 1,
    DEBUFF_F_PLAYERISTRACER = 2,
    DEBUFF_F_PLAYERISACTIVATOR = 4,
};

bool IsCurse(int debuff_id) {
    switch(debuff_id) {
        case DND_DEBUFF_OTHERWORDLYGRIP:
        case DND_DEBUFF_REDLICH:
        case DND_DEBUFF_PALADIN:
        case DND_DEBUFF_GURU:
        case DND_DEBUFF_HIEROPHANT:
        case DND_DEBUFF_FLESHWIZARD_SLOW:
        case DND_DEBUFF_FLESHWIZARD_WEAKEN:
        case DND_DEBUFF_FLESHWIZARD_SNARE:
        case DND_DEBUFF_LICHICE:
        case DND_DEBUFF_LICHVISION:
        case DND_DEBUFF_LICHDEGEN:
        case DND_DEBUFF_GOLGOTH_SLOW:
        case DND_DEBUFF_GOLGOTH_WEAKEN:
        return true;
    }
    return false;
}

void HandleCurseFade(int player_tid, int origin_tid, int r, int g, int b, int intensity, int duration, int curse_effect) {
    // curse has a color component attached
    if(intensity && !CheckActorInventory(player_tid, "IsBlinded")) {
        int temp = GetPlayerAttributeValue(player_tid - P_TIDSTART, INV_IMP_REDUCEDVISIONIMPAIR);
        if(temp)
            intensity = (100 - temp) / 100;

        if(intensity <= 0)
            return;

        duration = duration * curse_effect / 100;

        SetActivator(player_tid);
        FadeRange(r, g, b, intensity, 0, 0, 0, 0, duration * 1.0 / TICRATE);
        SetActivator(origin_tid);
    }
}

// player is target, and owner is activator by default
Script "DnD Give Buff" (int debuff_id, int debuff_flags) {
    int player_tid = 0;
    if(debuff_flags & DEBUFF_F_PLAYERISTRACER)
        player_tid = GetActorProperty(0, APROP_TRACERTID);
    else if(debuff_flags & DEBUFF_F_PLAYERISACTIVATOR)
        player_tid = ActivatorTID();
    else
        player_tid = GetActorProperty(0, APROP_TARGETTID);

    int pnum = player_tid - P_TIDSTART;

    //Log(s:"pnum ", d:pnum, s: " ", d:player_tid);

    if(!IsPlayer(player_tid) || !IsPlayerBuffStateOK(pnum))
        Terminate;

    if(debuff_flags & DEBUFF_F_OWNERISTARGET)
        SetActivatorToTarget(0);

    int i;
    int this = ActivatorTID();

    //printbold(s:"isplayer ", d:IsPlayer(player_tid), s: " ", d:CheckUniquePropertyOnPlayer(pnum, PUP_CURSEIMMUNITY));

    i = IsCurse(debuff_id);
	if(!CheckUniquePropertyOnPlayer(pnum, PUP_CURSEIMMUNITY) || !i) {
        int curse_effect = 100;
        int sc_flags = 0;
        if(i) {
            // player's curse effect thing goes here
            curse_effect = curse_effect * (100 - GetPlayerAttributeValue(pnum, INV_REDUCEDCURSEEFFECT)) / 100;
            if(curse_effect <= 0) {
                SetResultValue(0);
                Terminate;
            }
            sc_flags |= BTI_F_ISCURSE;
        }

        int intensity = 0;
        int duration = 0;
        int r = 0, g = 0, b = 0;
        buffData_T module& pbuffs = GetPlayerBuffData(pnum);

        switch(debuff_id) {
            case DND_BUFF_PHASING:
                intensity = 0.33;
                r = 112;
                g = 112;
                b = 112;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_PHASING, sc_flags);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
                GiveActorInventory(player_tid, "DnD_HasPhasing", 1);
                ACS_NamedExecuteAlways("DnD Phasing Anim", 0, player_tid);
            break;
            case DND_BUFF_PHASING_DASH:
                intensity = 0.33;
                r = 112;
                g = 112;
                b = 112;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_PHASING, sc_flags, 0, 2); // this lasts 2 seconds
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
                GiveActorInventory(player_tid, "DnD_HasPhasing", 1);
                ACS_NamedExecuteAlways("DnD Phasing Anim", 0, player_tid);
            break;

            case DND_BUFF_AMPHETAMINE:
                intensity = 0.225;
                r = 33;
                g = 33;
                b = 132;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_AMPHETAMINE, sc_flags) / 6;
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
                GiveActorInventory(player_tid, "DnD_HasAmphetamine", 1);
            break;

            case DND_DEBUFF_OTHERWORDLYGRIP:
                HandlePlayerBuffAssignment(pnum, this, BTI_OTHERWORDLYGRIP, sc_flags);
            break;
            case DND_DEBUFF_CHILL:
                HandlePlayerBuffAssignment(pnum, this, BTI_CHILL, sc_flags);

                if(!CheckActorInventory(player_tid, "DnD_ChillFXRunning")) {
                    GiveActorInventory(player_tid, "DnD_ChillFXRunning", 1);
                    while(HasPlayerBuff(pnum, BTI_CHILL)) {
                        ACS_NamedExecuteAlways("DnD Monster Chill FX", 0, player_tid);
                        Delay(const:TICRATE);
                    }
                    SetActorInventory(player_tid, "DnD_ChillStacks", 0);
                    SetActorInventory(player_tid, "DnD_ChillFXRunning", 0);
                }
            break;
            case DND_DEBUFF_FREEZE:
                HandlePlayerBuffAssignment(pnum, this, BTI_FREEZE, sc_flags);
                i = 0;
                if(!CheckActorInventory(player_tid, "DnD_FreezeFXRunning")) {
                    GiveActorInventory(player_tid, "DnD_FreezeFXRunning", 1);
                    while(HasPlayerBuff(pnum, BTI_FREEZE)) {
                        // create freeze fx and adjust it every 5 tics
                        ACS_NamedExecuteWithResult("DnD Monster Freeze Adjust", player_tid, i, i >= 2, CheckActorInventory(player_tid, "DnD_FreezeTimer") == 1);
                        Delay(const:5);
                        TakeActorInventory(player_tid, "DnD_FreezeTimer", 1);
                        i = (i + 1) % 4;
                    }
                    SetActorInventory(player_tid, "DnD_FreezeTimer", 0);
                    SetActorInventory(player_tid, "DnD_FreezeFXRunning", 0);
                }
            break;
            case DND_DEBUFF_OVERLOAD:
                HandlePlayerBuffAssignment(pnum, this, BTI_OVERLOAD, sc_flags);
                if(!CheckActorInventory(player_tid, "DnD_OverloadFXRunning")) {
                    GiveActorInventory(player_tid, "DnD_OverloadFXRunning", 1);
                    while(HasPlayerBuff(pnum, BTI_OVERLOAD)) {
                        ACS_NamedExecuteWithResult("DnD Monster Overload Particles", player_tid);
                        TakeActorInventory(player_tid, "DnD_OverloadTimer", 1);
                        Delay(const:DND_BASE_OVERLOADTICK);
                        GiveActorInventory(player_tid, "Overload_SoundStopper", 1);
                    }
                    SetActorInventory(player_tid, "DnD_OverloadTimer", 0);
                    SetActorInventory(player_tid, "DnD_OverloadFXRunning", 0);
                }
            break;

            case DND_DEBUFF_REDLICH:
                PlaySound(player_tid, "CurseHit", 6);
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_REDLICH, sc_flags);
                intensity = 0.33;
                r = 200;
                g = 32;
                b = 32;
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_DEBUFF_PALADIN:               
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_PALADIN, sc_flags);
                intensity = 0.5;
                r = 117;
                g = 21;
                b = 21;
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_DEBUFF_GURU:
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_GURU, sc_flags);
                intensity = 0.5;
                r = 102;
                g = 102;
                b = 102;
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_DEBUFF_HIEROPHANT:
                intensity = 0.75;
                r = 136;
                g = 22;
                b = 136;
                PlaySound(player_tid, "CurseHit", 6);
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_HIEROPHANT_DAMAGETAKEN, sc_flags);
                HandlePlayerBuffAssignment(pnum, this, BTI_HIEROPHANT_SLOW);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_DEBUFF_FLESHWIZARD_SLOW:
                intensity = 0.1875;
                r = 176;
                g = 33;
                b = 33;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_FLESHWIZARD_SLOW1, sc_flags);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_DEBUFF_FLESHWIZARD_WEAKEN:
                intensity = 0.1875;
                r = 176;
                g = 33;
                b = 33;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_FLESHWIZARD_WEAKEN, sc_flags);
                HandlePlayerBuffAssignment(pnum, this, BTI_FLESHWIZARD_SLOW2);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_DEBUFF_FLESHWIZARD_SNARE:
                intensity = 0.1875;
                r = 176;
                g = 33;
                b = 33;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_FLESHWIZARD_SNARE, sc_flags);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_DEBUFF_STOMPSLOW:
                intensity = 0.1875;
                r = 68;
                g = 68;
                b = 68;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_STOMPSLOW, sc_flags);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_DEBUFF_LICHICE:
                intensity = 0.8;
                r = 208;
                g = 214;
                b = 254;
                PlaySound(player_tid, "Lich/IceCurse", 6);
                for(i = 0; i < 5; i++) {
                    duration = HandlePlayerBuffAssignment(pnum, this, BTI_LICHICECURSE, sc_flags, -20 * (i + 1));
                    // only proc on first instance
                    if(!i)
                        HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
                    Delay(25 * curse_effect / 100);
                }
                HandlePlayerBuffAssignment(pnum, this, BTI_LICHICECURSE, BTI_F_REMOVE | sc_flags, -100);
            break;
            case DND_DEBUFF_LICHVISION:
                intensity = 0.9;
                r = 17;
                g = 35;
                b = 88;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_LICHVISION, sc_flags);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_DEBUFF_LICHDEGEN:
                intensity = 0.8;
                r = 49;
                g = 184;
                b = 79;
                PlaySound(0, "Lich/PoisonCurse", 6);
                HandlePlayerBuffAssignment(pnum, this, BTI_LICHDEGEN, sc_flags);

                Delay(const:4);
                r = pbuffs.buff_net_values[BUFF_HEALTHREGEN].additive;
                if(r < 0) {
                    r = -r;
                    r = (r * 100) >> 16;
                    g = GetActorProperty(player_tid, APROP_HEALTH) * r / 100;
    
                    r = pbuffs.buff_net_values[BUFF_HEALTHREGEN].multiplicative;
                    if(r != 1.0) {
                        r = (r * 100) >> 16;
                        g = g * r / 100;
                    }
    
                    if(g <= 0)
                        g = 1;
                    Thing_Damage2(player_tid, g, "PoisonDOT");
                }
            break;

            case DND_DEBUFF_SSRATHSTUN:
                intensity = 0.2;
                r = 100;
                g = 100;
                b = 18;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_SSRATHSTUN, sc_flags);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_DEBUFF_ERYXIASLOW:
                intensity = 0.2375;
                r = 15;
                g = 15;
                b = 136;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_ERYXIASLOW, sc_flags);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;

            case DND_DEBUFF_TORRASQUE_SNARE:
                intensity = 0.15;
                r = 64;
                g = 64;
                b = 64;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_TORRASQUE_SNARE, sc_flags);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_DEBUFF_GOLGOTH_SLOW:
                intensity = 0.275;
                r = 96;
                g = 21;
                b = 69;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_GOLGOTH_SLOW, sc_flags);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_DEBUFF_GOLGOTH_WEAKEN:
                intensity = 0.275;
                r = 96;
                g = 21;
                b = 69;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_GOLGOTH_WEAKEN, sc_flags);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
        }
	}
    SetResultValue(0);
}

#endif