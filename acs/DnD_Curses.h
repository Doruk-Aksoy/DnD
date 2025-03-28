#ifndef DND_CURSES_IN
#define DND_CURSES_IN

enum {
    DND_CURSE_OTHERWORDLYGRIP,
    DND_CURSE_REDLICH,
    DND_CURSE_PALADIN,
    DND_CURSE_GURU,
    DND_CURSE_HIEROPHANT,
    DND_CURSE_FLESHWIZARD_SLOW,
    DND_CURSE_FLESHWIZARD_WEAKEN,
    DND_CURSE_FLESHWIZARD_SNARE,
    DND_CURSE_STOMPSLOW,
    DND_CURSE_LICHICE,
    DND_CURSE_LICHVISION,
    DND_CURSE_LICHDEGEN,

    DND_CURSE_SSRATHSTUN,
    DND_CURSE_ERYXIASLOW,

    DND_CURSE_TORRASQUE_SNARE,
    DND_CURSE_GOLGOTH_SLOW,
    DND_CURSE_GOLGOTH_WEAKEN
};
#define DND_CURSE_BEGIN DND_CURSE_OTHERWORDLYGRIP
#define DND_CURSE_END DND_CURSE_GOLGOTH_WEAKEN

enum {
    CURSE_F_OWNERISTARGET = 1,
    CURSE_F_PLAYERISTRACER = 2,
    CURSE_F_PLAYERISACTIVATOR = 4,
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

// player is target, and owner is activator by default
Script "DnD Monster Curse" (int curse_id, int curse_flags) {
    int player_tid = 0;
    if(curse_flags & CURSE_F_PLAYERISTRACER)
        player_tid = GetActorProperty(0, APROP_TRACERTID);
    else if(curse_flags & CURSE_F_PLAYERISACTIVATOR)
        player_tid = ActivatorTID();
    else
        player_tid = GetActorProperty(0, APROP_TARGETTID);

    int pnum = player_tid - P_TIDSTART;

    //Log(s:"pnum ", d:pnum, s: " ", d:player_tid);

    if(!IsPlayer(player_tid) || !IsPlayerBuffStateOK(pnum))
        Terminate;

    if(curse_flags & CURSE_F_OWNERISTARGET)
        SetActivatorToTarget(0);

    int i, temp = 0;
    int this = ActivatorTID();

    //printbold(s:"isplayer ", d:IsPlayer(player_tid), s: " ", d:CheckUniquePropertyOnPlayer(pnum, PUP_CURSEIMMUNITY));

    int curse_effect = 100; // player's curse effect thing goes here

	if(!CheckUniquePropertyOnPlayer(pnum, PUP_CURSEIMMUNITY)) {
        int intensity = 0;
        int duration = 0;
        int r = 0, g = 0, b = 0;
        buffData_T module& pbuffs = GetPlayerBuffData(pnum);

        switch(curse_id) {
            case DND_CURSE_OTHERWORDLYGRIP:
                HandlePlayerBuffAssignment(pnum, this, BTI_OTHERWORDLYGRIP);
            break;
            case DND_CURSE_REDLICH:
                PlaySound(player_tid, "CurseHit", 6);
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_REDLICH);
                intensity = 0.33;
                r = 200;
                g = 32;
                b = 32;
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_CURSE_PALADIN:               
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_PALADIN);
                intensity = 0.5;
                r = 117;
                g = 21;
                b = 21;
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_CURSE_GURU:
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_GURU);
                intensity = 0.5;
                r = 102;
                g = 102;
                b = 102;
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_CURSE_HIEROPHANT:
                intensity = 0.75;
                r = 136;
                g = 22;
                b = 136;
                PlaySound(player_tid, "CurseHit", 6);
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_HIEROPHANT_DAMAGETAKEN);
                HandlePlayerBuffAssignment(pnum, this, BTI_HIEROPHANT_SLOW);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_CURSE_FLESHWIZARD_SLOW:
                intensity = 0.1875;
                r = 176;
                g = 33;
                b = 33;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_FLESHWIZARD_SLOW1);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_CURSE_FLESHWIZARD_WEAKEN:
                intensity = 0.1875;
                r = 176;
                g = 33;
                b = 33;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_FLESHWIZARD_WEAKEN);
                HandlePlayerBuffAssignment(pnum, this, BTI_FLESHWIZARD_SLOW2);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_CURSE_FLESHWIZARD_SNARE:
                intensity = 0.1875;
                r = 176;
                g = 33;
                b = 33;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_FLESHWIZARD_SNARE);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_CURSE_STOMPSLOW:
                intensity = 0.1875;
                r = 68;
                g = 68;
                b = 68;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_STOMPSLOW);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_CURSE_LICHICE:
                intensity = 0.8;
                r = 208;
                g = 214;
                b = 254;
                PlaySound(player_tid, "Lich/IceCurse", 6);
                for(i = 0; i < 5; i++) {
                    temp = -20 * (i + 1);
                    duration = HandlePlayerBuffAssignment(pnum, this, BTI_LICHICECURSE, 0, temp);
                    // only proc on first instance
                    if(!i)
                        HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
                    Delay(25 * curse_effect / 100);
                }
                HandlePlayerBuffAssignment(pnum, this, BTI_LICHICECURSE, BTI_F_REMOVE, temp);
            break;
            case DND_CURSE_LICHVISION:
                intensity = 0.9;
                r = 17;
                g = 35;
                b = 88;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_LICHVISION);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_CURSE_LICHDEGEN:
                intensity = 0.8;
                r = 49;
                g = 184;
                b = 79;
                PlaySound(0, "Lich/PoisonCurse", 6);
                HandlePlayerBuffAssignment(pnum, this, BTI_LICHDEGEN);

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

            case DND_CURSE_SSRATHSTUN:
                intensity = 0.2;
                r = 100;
                g = 100;
                b = 18;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_SSRATHSTUN);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_CURSE_ERYXIASLOW:
                intensity = 0.2375;
                r = 15;
                g = 15;
                b = 136;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_ERYXIASLOW);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;

            case DND_CURSE_TORRASQUE_SNARE:
                intensity = 0.15;
                r = 64;
                g = 64;
                b = 64;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_TORRASQUE_SNARE);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_CURSE_GOLGOTH_SLOW:
                intensity = 0.275;
                r = 96;
                g = 21;
                b = 69;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_GOLGOTH_SLOW);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
            case DND_CURSE_GOLGOTH_WEAKEN:
                intensity = 0.275;
                r = 96;
                g = 21;
                b = 69;
                duration = HandlePlayerBuffAssignment(pnum, this, BTI_GOLGOTH_WEAKEN);
                HandleCurseFade(player_tid, this, r, g, b, intensity, duration, curse_effect);
            break;
        }
	}
    SetResultValue(0);
}

#endif