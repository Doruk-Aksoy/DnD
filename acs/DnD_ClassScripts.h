#ifndef DND_CLASS_SC_IN
#define DND_CLASS_SC_IN

Script "DnD Check Execution" (void) {
	int this = ActivatorTID();
	SetActivatorToTarget(0);
	int tid = ActivatorTID();
	SetActivator(this);
	SetResultValue(tid && !!CheckActorInventory(tid, "Doomguy_CanExecute"));
}

// monster is assumed activator of this function
void HandleDoomguyExecute(int ptid, int mon_tid) {
	int pnum = ptid - P_TIDSTART;
	if(HasActorClassPerk_Fast(ptid, "Doomguy", 1) && CheckInventory("Doomguy_CanExecute")) {
		if(CheckInventory("Doomguy_ValidExecute")) {
			TakeInventory("Doomguy_CanExecute", 1);
			TakeInventory("Doomguy_ValidExecute", 1);

			int hp_to_give = GetSpawnHealth(false, pnum) * DND_DOOMGUY_PERK5HEAL / 100;
			
			// give extra health to player
			if(HasActorClassPerk_Fast(ptid, "Doomguy", 2)) {
				// give the better damage boost
				GiveActorInventory(ptid, "Doomguy_Perk20_Damage_Execute", 1);
				GiveActorInventory(ptid, "Doomguy_DemonSoul_PickMsg", 1);

				int temp = MonsterProperties[mon_tid - DND_MONSTERTID_BEGIN].maxhp;
				temp = Clamp_Between(temp / DND_DOOMGUY_HEALDIV, 1, DND_DOOMGUY_MAXHEAL);
				hp_to_give += temp;
			}

			// heal
			ACS_NamedExecuteAlways("DnD Health Pickup", 0, hp_to_give, 6, 1);

			// slayer special interaction
			hp_to_give = GetPlayerAttributeValue(pnum, INV_EX_REFILLAMMOONMELEEKILL);
			if(hp_to_give) {
				SetActivator(ptid);
				GiveOwnedWeaponsAmmo(hp_to_give);
				SetActivator(mon_tid);
			}

			if(GetPlayerAttributeValue(pnum, INV_EX_SWAPFROMMELEECRIT))
				ACS_NamedExecuteAlways("DnD Swapped From Melee", 0, ptid);
		}
		ACS_NamedExecuteWithResult("DnD Doomguy Execute Translation", mon_tid, 1, MonsterProperties[mon_tid - DND_MONSTERTID_BEGIN].id);
	}
}

Script "DnD Doomguy Execute Translation" (int mon_tid, int mode, int m_id) CLIENTSIDE {
	// if this guy isnt doomguy, terminate
	// testing running it in any case to see if it helps fix the bug of these guys sometimes having their shit not taken away
	//Log(d:ConsolePlayerNumber() + P_TIDSTART, s:" ", d:CheckActorInventory(ConsolePlayerNumber() + P_TIDSTART, "Doomguy_Perk5"));
	if(!HasActorClassPerk_Fast(ConsolePlayerNumber() + P_TIDSTART, "Doomguy", 1))
		Terminate;

	if(!mode) {
		Thing_SetTranslation(mon_tid, DND_EXECUTE_TRANSLATION);
		GiveActorInventory(mon_tid, "Doomguy_CanExecute", 1);
	}
	else {
		switch(m_id) {
			case MONSTER_ZOMBIEQUAKE2:
				Thing_SetTranslation(mon_tid, DND_QUAKEZOMBIESG_TRANSLATION);
			break;

			case MONSTER_ZOMBIESMG:
				Thing_SetTranslation(mon_tid, DND_SMGGUY_TRANSLATION);
			break;
			case MONSTER_ZOMBIEQUAKE3:
				Thing_SetTranslation(mon_tid, DND_QUAKEZOMBIECG_TRANSLATION);
			break;

			case MONSTER_NETHERDARKIMP:
				Thing_SetTranslation(mon_tid, DND_NETHERDARKIMP_TRANSLATION);
			break;

			case MONSTER_FLAMEDEMON:
				Thing_SetTranslation(mon_tid, DND_FLAMEDEMON_TRANSLATION);
			break;
			case MONSTER_SCAVENGER:
				Thing_SetTranslation(mon_tid, DND_SCAVENGER_TRANSLATION);
			break;
			case MONSTER_NHUMCIGN:
				Thing_SetTranslation(mon_tid, DND_NHUMCIGN_TRANSLATION);
			break;
			case MONSTER_NIGHTMAREDEMON:
				Thing_SetTranslation(mon_tid, DND_NIGHTMARESPECTRE_TRANSLATION);
			break;
			case MONSTER_GRAVEDIGGER:
				Thing_SetTranslation(mon_tid, DND_GRAVEDIGGER_TRANSLATION);
			break;
			case MONSTER_DEVOURER:
				Thing_SetTranslation(mon_tid, DND_DEVOURER_TRANSLATION);
			break;
			case MONSTER_RAVAGER:
				if(GetActorClass(mon_tid) == "RavagerGhost")
					Thing_SetTranslation(mon_tid, TRANSLATION_ICE);
				else
					Thing_SetTranslation(mon_tid, DND_NO_TRANSLATION);
			break;

			case MONSTER_ENHANCEDCACO:
				Thing_SetTranslation(mon_tid, DND_ENHANCEDCACO_TRANSLATION);
			break;

			case MONSTER_DEFILER:
				Thing_SetTranslation(mon_tid, DND_DEFILER_TRANSLATION);
			break;

			case MONSTER_ICEFATSO:
				Thing_SetTranslation(mon_tid, DND_ICEFATSO_TRANSLATION);
			break;

			case MONSTER_CADAVER:
				Thing_SetTranslation(mon_tid, DND_CADAVER_TRANSLATION);
			break;

			case MONSTER_DREADKNIGHT:
				Thing_SetTranslation(mon_tid, DND_DREADKNIGHT_TRANSLATION);
			break;

			case MONSTER_BLOODSATYR:
				Thing_SetTranslation(mon_tid, DND_BLOODSATYR_TRANSLATION);
			break;
			case MONSTER_MOONSATYR:
				Thing_SetTranslation(mon_tid, DND_MOONSATYR_TRANSLATION);
			break;
			case MONSTER_WARLORD:
				Thing_SetTranslation(mon_tid, DND_WARLORDHELL_TRANSLATION);
			break;
			case MONSTER_LORDOFHERESY:
				Thing_SetTranslation(mon_tid, DND_LORDHERESY_TRANSLATION);
			break;
			case MONSTER_BARBATOS:
				Thing_SetTranslation(mon_tid, DND_BARBATOS_TRANSLATION);
			break;
			case MONSTER_KJAROCH:
				Thing_SetTranslation(mon_tid, DND_KJAROCH_TRANSLATION);
			break;

			case MONSTER_HORSHACKER:
				Thing_SetTranslation(mon_tid, DND_HORSHACKER_TRANSLATION);
			break;

			default:
				Thing_SetTranslation(mon_tid, DND_NO_TRANSLATION);
			break;
		}
		
		TakeActorInventory(mon_tid, "Doomguy_CanExecute", 1);
	}
	SetResultValue(0);
}

Script "DnD Marine Perk 50 Checker" (void) {
	int max_hp;
	bool proc;
	int counter;
	static int i = 0;
	++i;
    while(isAlive()) {
		// prev hp recorded 2 seconds ago
		max_hp = GetSpawnHealth();
		counter = 0;
		proc = false;
		do {
			Delay(const:2);
			proc = CheckInventory("Marine_Perk50_DamageTaken") >= Max(1, max_hp * 2 / 5);
		} while(!proc && ++counter < MARINE_PROC_WINDOW);

        // if hp difference in the second is > 40% of our health cap, we can trigger
		SetInventory("Marine_Perk50_DamageTaken", 0);
        
        if(proc) {
            SetInventory("Marine_DamageReduction_Timer", DND_MARINE_DMGREDUCE_TIMER);

            ACS_NamedExecuteAlways("DnD Marine Shield Anim", 0);

            do {
                Delay(const:1);
                TakeInventory("Marine_DamageReduction_Timer", 1);
            } while(isAlive() && CheckInventory("Marine_DamageReduction_Timer"));

            // per 500 give hp
            max_hp = CheckInventory("Marine_Perk50_DamageDealt") / DND_MARINE_PERK50_HPPER;
            if(max_hp) {
                HandleHealthPickup(max_hp, 0, 0);
                // fx
                Spawn("DarkHealEffectSpawner", GetActorX(0), GetActorY(0), GetActorZ(0), 0);
                LocalAmbientSound("Items/fieldkit", 127);
            }

            SetInventory("Marine_Perk50_DamageDealt", 0);

            // go on a long cooldown now
            SetInventory("Marine_Perk50_Cooldown", DND_MARINE_PERK50_COOLDOWN);
            do {
                Delay(const:1);
                TakeInventory("Marine_Perk50_Cooldown", 1);
            } while(isAlive() && CheckInventory("Marine_Perk50_Cooldown"));

			SetInventory("Marine_Perk50_DamageTaken", 0);
        }
    }
}

Script "DnD Marine Shield Anim" (void) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

    LocalAmbientSound("Marine/Shielded", 127);
    
    int width = (800 * FixedDiv(1.0, ScreenResOffsets[SCREEN_ASPECT_RATIO])) >> 16;
	int height = (450 * FixedDiv(1.0, ASPECT_4_3)) >> 16;

    SetHudSize(width, height, 1);

	width = (width << 15) + 0.4;
	height <<= 15;
    height = GetIntegerBits(height);

	SetFont("FOCUSBG");
    for(int i = 0; i < DND_MARINE_DMGREDUCE_TIMER; ++i) {
        int a = abs(sin(1.0 * (i % 60) / 60));
        if(a > 0.8)
            a = 0.8;

	    HudMessage (s:"A"; HUDMSG_FADEINOUT | HUDMSG_ALPHA, HUD_GRAPHIC_ID, CR_UNTRANSLATED, width, height, 0.875, 0, 0.25, a);
        Delay(const:1);
    }
}

Script "DnD Hobo Frenzy Anim" (void) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

    int width = (800 * FixedDiv(1.0, ScreenResOffsets[SCREEN_ASPECT_RATIO])) >> 16;
	int height = (450 * FixedDiv(1.0, ASPECT_4_3)) >> 16;

    SetHudSize(width, height, 1);

	width = (width << 15) + 0.4;
	height <<= 15;
    height = GetIntegerBits(height);

	int anim_count = 0;
    for(int i = 0; IsAlive() && i < 72 * 3; ++i) {
        int a = abs(sin(1.0 * (i % 72) / 72));
        if(a > 0.6)
            a = 0.6;
		
		++anim_count;
		SetFont("SHGFRZ1");
		if(anim_count == 4) {
			anim_count = 0;
			SetFont("SHGFRZ2");
		}
	    HudMessage (s:"A"; HUDMSG_FADEINOUT | HUDMSG_ALPHA, HUD_GRAPHIC_ID, CR_UNTRANSLATED, width, height, 0.333, 0, 0.125, a);
        Delay(const:1);
    }
}

Script "DnD Hobo Frenzy Timer" (void) {
	PlaySound(0, "Hobo/Frenzy", CHAN_6, 1.0);

	ACS_NamedExecuteAlways("DnD Hobo Frenzy Anim", 0);

    while(isAlive() && CheckInventory("Hobo_ShotgunFrenzyTimer")) {
        TakeInventory("Hobo_ShotgunFrenzyTimer", 1);
        Delay(const:1);
    }
    CalculatePlayerAccuracy(PlayerNumber());
}

void HandleBerserkerRoar(int tid) {
	GiveActorInventory(tid, "Berserker_NoRoar", 1);
	ACS_NamedExecuteAlways("DnD Berserker Roar", 0, tid);
}

Script "DnD Berserker Perk20" (void) {
	while(CheckInventory("Berserker_DamageTimer")) {
		Delay(const:1);
		TakeInventory("Berserker_DamageTimer", 1);
	}
	
	// heal for max hp 10% if was at full stack
	if(isAlive() && CheckInventory("Berserker_DamageTracker") == DND_BERSERKER_PERK20_MAXSTACKS) {
        HandleHealthPickup(GetSpawnHealth() / DND_BERSERKER_PERK20_HEALPERCENT, 0, 0);
	
		// fx
		Spawn("DarkHealEffectSpawner", GetActorX(0), GetActorY(0), GetActorZ(0), 0);
		LocalAmbientSound("Berserker/Heal", 127);
	}
	SetInventory("Berserker_DamageTracker", 0);
}

Script "DnD Berserker Perk50 Timer" (int this) {
	if(this)
		SetActivator(this);
	// will simply count these by one while we have them, per the berserker timer
	while(CheckInventory("Berserker_HitTracker")) {
		while(CheckInventory("Berserker_HitTimer")) {
			if(!isAlive()) {
				SetInventory("Berserker_HitTracker", 0);
				SetInventory("Berserker_HitTimer", 0);
				Terminate;
			}
		
			TakeInventory("Berserker_HitTimer", 1);
			delay(const:1);
		}
		TakeInventory("Berserker_Perk60_Speed", 1);
		TakeInventory("Berserker_HitTracker", 1);
		SetInventory("Berserker_HitTimer", DND_BERSERKER_PERK60_TIMER);
	}
	// get rid of no roar at end of timer
	TakeInventory("Berserker_NoRoar", 1);
	SetInventory("Berserker_HitTimer", 0);
}

Script "DnD Berserker Perk5 Check" (void) {
	// berserker perk5 fail-safe checks for super weapons
	if(HasClassPerk_Fast("Berserker", 1)) {
		for(int i = DND_WEAPON_BFG32768; i <= DND_WEAPON_SOULREAVER; ++i) {
			if(CheckInventory(Weapons_Data[i].name)) {
				TakeInventory(Weapons_Data[i].name, 1);
				TakeInventory("H_WeaponSlot7", 1);
				break;
			}
		}
	}
}

Script "DnD Berserker Perk5 Check (Melee)" (void) {
	// berserker perk5 fail-safe checks for class change from berserker to any other
	// do not include luxury weapons as part of this
	bool has_melee = false;
	if(!HasClassPerk_Fast("Berserker", 1)) {
		for(int i = DND_WEAPON_DOUBLECHAINSAW; i <= LAST_SLOT0_NONLUXURYWEAPON; ++i) {
			if(CheckInventory(Weapons_Data[i].name)) {
				// basically, if we had a melee weapon found, and we find more afterwards, we will take all subsequent ones away
				if(has_melee) {
					TakeInventory(Weapons_Data[i].name, 1);
					TakeInventory("H_WeaponSlot1", 1);
					GiveInventory(Weapons_Data[DND_WEAPON_CHAINSAW].name, 1);
				}
				
				has_melee = true;
			}
		}
	}
}

Script "DnD Berserker Roar" (int this) CLIENTSIDE {
	if(this)
		SetActivator(this);
	
	PlaySound(0, "Berserker/Berserk", CHAN_6, 1.0);
	PlaySound(0, "Berserker/GoMad", CHAN_7, 1.0);
	GiveInventory("Berserker_ThunderFX_Spawner", 1);
	
	if(ScreenResOffsets[SCREEN_ASPECT_RATIO] == ASPECT_16_9 || ScreenResOffsets[SCREEN_ASPECT_RATIO] == ASPECT_16_10) {
		SetFont("BERSERKW");
		SetHudSize(1024, 768, 0);
		HudMessage(s:"A"; HUDMSG_FADEINOUT, 90, -1, 512.0, 384.0, 0.25, 0.3, 0.3, 0.8);
	}
	else {
		SetFont("BERSERK");
		SetHudSize(640, 480, 0);
		HudMessage(s:"A"; HUDMSG_FADEINOUT, 90, -1, 320.0, 240.0, 0.25, 0.3, 0.3, 0.8);
	}
}

Script "DnD Berserker GoW" (void) {
	GiveInventory("Berserker_Perk80_Extension", 1);

	// damage reduction perk
	SetInventory("Berserker_DamageTracker", DND_BERSERKER_PERK20_MAXSTACKS);

	// damage boost perk
	SetInventory("Berserker_HitTracker", DND_BERSERKER_PERK60_MAXSTACKS);

	ACS_NamedExecuteAlways("DnD Berserker Roar", 0, 0);

	Delay(TICRATE * DND_BERSERKER_PERK80_EXTENSION + 10);

	if(!CheckInventory("Berserker_Perk80_Extension") && isAlive()) {
		if(CheckInventory("Berserker_DamageTracker"))
			ACS_NamedExecuteAlways("DnD Berserker Perk20", 0);

		if(CheckInventory("Berserker_HitTracker"))
			ACS_NamedExecuteAlways("DnD Berserker Perk50 Timer", 0, 0);
	}
}

Script "DnD Cyborg Instability Timer" (void) {
	while(IsAlive() && CheckInventory("Cyborg_Instability_Timer")) {
		delay(const:1);
		TakeInventory("Cyborg_Instability_Timer", 1);
	}
	SetInventory("Cyborg_NoAnim", 0);
	SetInventory("Cyborg_InstabilityStack", 0);
	LocalAmbientSound("Instability/End", 127);

	// 50% chance to insta start es regen
	if(RunLuckBasedChance(PlayerNumber(), 0.5)) {
		SetInventory("DnD_Hit_CombatTimer", 0);
		GiveInventory("EShieldChargeNow", 1);
	}
}

Script "DnD Cyborg Visor Anim" (void) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	LocalAmbientSound("Instability/Load", 127);

	int width = (800 * FixedDiv(1.0, ScreenResOffsets[SCREEN_ASPECT_RATIO])) >> 16;
	int height = (450 * FixedDiv(1.0, ASPECT_4_3)) >> 16;

	SetHudSize(width, height, 1);

	width = (width << 15) + 0.4;
	height <<= 15;
    height = GetIntegerBits(height);

	SetFont("VISORB");
	HudMessage (s:"A"; HUDMSG_FADEINOUT | HUDMSG_ALPHA, HUD_GRAPHIC_ID, CR_UNTRANSLATED, width, height, 0.75, 0.5, 0.25, 0.9875);

	SetFont("VISBG");
	HudMessage (s:"A"; HUDMSG_FADEINOUT | HUDMSG_ALPHA, HUD_GRAPHIC_ID + 1, CR_UNTRANSLATED, width, height, 1.0, 0.5, 0.5, 0.75);

	int repeats = 0;
	int hx = 512.4, hy = 384.0;
	int ofx = 0, ofy = 0;
	do {
		for(int i = 1; i <= MAX_CYBORG_VISOR_FRAMES && isAlive(); ++i) {
			SetHudSize(1024, 768, 0);
			SetFont(StrParam(s:"HACKB", d:i));

			if(random(1, 100) <= 16) {
				ofx += GetIntegerBits(random(-8.0, 8.0));
				ofy += GetIntegerBits(random(-8.0, 8.0));
			}

			HudMessage (s:"A"; HUDMSG_FADEINOUT | HUDMSG_ALPHA | HUDMSG_ADDBLEND, HUD_GRAPHIC_ID + 2, CR_UNTRANSLATED, hx + ofx, hy + ofy, 0.05, 0.0, 0.25, 0.666);
			Delay(const:1);
		}
		++repeats;
	} while(repeats < 2 && isAlive());
}

Script "DnD Wanderer Explosions" (int this, int target) {
	int hpdamage = MonsterProperties[this - DND_MONSTERTID_BEGIN].maxhp;
	if(hpdamage > INT_MAX / DND_WANDERER_EXP_DAMAGE) {
		hpdamage /= 100;
		hpdamage *= DND_WANDERER_EXP_DAMAGE;
	}
	else
		hpdamage = hpdamage * DND_WANDERER_EXP_DAMAGE / 100;

	int exptid = DND_WANDERER_EXP_TID + target - P_TIDSTART;
	if(hpdamage <= 0)
		hpdamage = 1;
	
	SpawnForced("Wanderer_Explosion", GetActorX(this), GetActorY(this), GetActorZ(this) + GetActorProperty(this, APROP_HEIGHT) / 2 + 24.0, exptid);
	SetUserVariable(exptid, "user_dmg", hpdamage);
	SetActivator(exptid);
	SetPointer(AAPTR_TARGET, target);
	Thing_ChangeTID(exptid, 0);
}

int GetPunisherTierRequirement(int tier, int cap = -1) {
	static int punisher_tier_values[DND_MAX_PUNISHER_PERK50_TIERS];
	if(cap != -1) {
		// halved
		cap <<= 15;

		// initialization step -- we have to calculate a base value
		if(cap > DND_MAX_PUNISHER_PERK50_CAP)
			cap = DND_MAX_PUNISHER_PERK50_CAP;

		// calculate the base value here
		int f = FixedDiv(DND_PUNISHER_PERK50_BONUS_PER_TIER, fpow(DND_PUNISHER_PERK50_BONUS_PER_TIER + 1.0, DND_MAX_PUNISHER_PERK50_TIERS) - 1.0);
		punisher_tier_values[0] = (FixedMul(cap, f)) & 0xFFFFF000;

		//int sum = punisher_tier_values[0];
		//Log(s:"Start punisher tier 0: ", f:punisher_tier_values[0], s:" initial score value was ", f:cap);
		for(int i = 1; i < DND_MAX_PUNISHER_PERK50_TIERS; ++i) {
			punisher_tier_values[i] = punisher_tier_values[i - 1] + (FixedMul(punisher_tier_values[0], fpow(1.0 + DND_PUNISHER_PERK50_BONUS_PER_TIER, i))) & 0xFFFFF000;
			//sum += punisher_tier_values[i];
			//Log(s:"Tier ", d:i, s:": ", f:punisher_tier_values[i], s: " running sum: ", f:sum);
		}
	}

	return punisher_tier_values[tier];
}

int GetPunisherTierKillBonus(int m_id) {
	static int factor_table[MAX_MONSTER_CATEGORIES] = {
		DND_ZOMBIE_CONTRIB,
		DND_SHOTGUNNER_CONTRIB,
		DND_CHAINGUNNER_CONTRIB,
		DND_DEMON_CONTRIB,
		DND_DEMON_CONTRIB,
		DND_IMP_CONTRIB,
		DND_CACO_CONTRIB,
		DND_PAIN_CONTRIB,
		DND_SOUL_CONTRIB,
		DND_REVENANT_CONTRIB,
		DND_HELLKNIGHT_CONTRIB,
		DND_BARON_CONTRIB,
		DND_FATSO_CONTRIB,
		DND_ARACHNO_CONTRIB,
		DND_VILE_CONTRIB,
		DND_SPIDERMASTERMIND_CONTRIB,
		DND_CYBERDEMON_CONTRIB,
		DND_SHOTGUNNER_CONTRIB
	};

	return (factor_table[MonsterProperties[m_id].class] << 16) & 0xFFFFF000;
}

// Get the tier from amount of item token
int GetPunisherTier() {
	int comp = CheckInventory("Punisher_Perk50_Counter");
	for(int i = 0; i < DND_MAX_PUNISHER_PERK50_TIERS; ++i) {
		if(comp < GetPunisherTierRequirement(i))
			return i;
	}
	return DND_MAX_PUNISHER_PERK50_TIERS;
}

void HandleShadowClone(int pnum, int victim, int shooter) {
	GiveInventory("Trickster_ShadowCooldown", DND_TRICKSTER_PERK40_COOLDOWN);
	ACS_NamedExecuteWithResult("DnD Give Buff", DND_BUFF_PHASING, DEBUFF_F_PLAYERISACTIVATOR);
	ACS_NamedExecuteAlways("DnD Trickster Cooldown", 0);
	SpawnForced("TricksterShadowClone", GetActorX(0), GetActorY(0), GetActorZ(0), DND_TRICKSTERCLONE_TID + pnum);
	Thing_SetTranslation(DND_TRICKSTERCLONE_TID + pnum, -1);
	SetActivator(DND_TRICKSTERCLONE_TID + pnum);
	SetPointer(AAPTR_TARGET, shooter);
	SetActorProperty(0, APROP_TARGETTID, shooter);
	Thing_ChangeTID(DND_TRICKSTERCLONE_TID + pnum, 0);
	SetActivator(victim);

	for(int i = 0; i < DND_TRICKSTER_RANDOMCLONES_COUNT && IsActorAlive(victim); ++i) {
		SpawnForced("TricksterShadowClone_Random", GetActorX(0), GetActorY(0), GetActorZ(0), DND_TRICKSTERCLONE_TID + pnum);
		Thing_SetTranslation(DND_TRICKSTERCLONE_TID + pnum, -1);
		SetActivator(DND_TRICKSTERCLONE_TID + pnum);
		SetPointer(AAPTR_TARGET, shooter);
		SetActorProperty(0, APROP_TARGETTID, shooter);
		Thing_ChangeTID(DND_TRICKSTERCLONE_TID + pnum, 0);
		SetActivator(victim);
	}
}

Script "DnD Trickster Cooldown" (void) {
	while(IsAlive()) {
		TakeInventory("Trickster_ShadowCooldown", 1);
		Delay(DND_TRICKSTER_PERK40_DELAY);
	}
	SetInventory("Trickster_ShadowCooldown", 0);
}

Script "DnD Trickster Pointer" (void) {
	int owner = GetActorProperty(0, APROP_TARGETTID);
	Thing_ChangeTID(0, owner - P_TIDSTART + DND_TRICKSTER_POINTERTID);
}

Script "DnD Spawn Target Clone" (void) {
	int owner = ActivatorTID();
	int pnum = ActivatorTID() - P_TIDSTART;
	int shooter = DND_TRICKSTER_POINTERTID + pnum;
	SpawnForced("TricksterShadowClone_Targeted", GetActorX(0), GetActorY(0), GetActorZ(0), DND_TRICKSTERCLONE_TID + pnum);
	Thing_SetTranslation(DND_TRICKSTERCLONE_TID + pnum, -1);
	SetActivator(DND_TRICKSTERCLONE_TID + pnum);
	SetPointer(AAPTR_TARGET, shooter);
	SetPointer(AAPTR_MASTER, owner);
	SetActorProperty(0, APROP_TARGETTID, shooter);
	Thing_ChangeTID(DND_TRICKSTERCLONE_TID + pnum, 0);
	SetResultValue(0);
}

Script "DnD Trickster Swap Checker" (void) {
	int owner = GetActorProperty(0, APROP_MASTERTID);
	while(isAlive() && isActorAlive(owner)) {
		if(CheckActorInventory(owner, "TricksterSwapSignal")) {
			// swap places
			int ox = GetActorX(0);
			int oy = GetActorY(0);
			int oz = GetActorZ(0);

			int tx = GetActorX(owner);
			int ty = GetActorY(owner);
			int tz = GetActorZ(owner);
			
			GiveActorInventory(owner, "Trickster_Swap", 1);
			TakeActorInventory(owner, "Trickster_SwapCount", 1);

			SetActorPosition(owner, ox, oy, oz, 0);
			SetActorPosition(0, tx, ty, tz, 0);

			ActivatorSound("Trickster/Poof", 127);

			TakeActorInventory(owner, "TricksterSwapSignal", 1);
		}
		Delay(const:2);
	}
}

void UndoWandererAscension() {
	TakeInventory("Wanderer_Ascended", 1);
	GiveInventory("UnmakePhysicalResist", 1);
	StopSound(0, CHAN_7);
}

Script "DnD Force Select Ascension" (int val) {
	SetWeapon("Wanderer_Ascended");
	GiveInventory("MakePhysicalResist", 1);
	GiveInventory("Wanderer_ReturnIndicator_Spawner", 1);

	Delay(const:DND_WANDERER_ASCENSION_TIME);

	UndoWandererAscension();
	
	SetResultValue(0);
}

Script "DnD Wanderer Dash" (void) {
	GiveInventory("AllowFlight", 1);
	GiveInventory("AllowThruObjects", 1);

	SetActorVelocity(0, 0, 0, 0, 0, 0);

	PlaySound(0, "Wanderer/DashBegin", 7);

	for(int i = 0; i < DND_WANDERER_DASH_TIME; ++i) {
		int v_dir = GetDirectionVector(0);
		SetActorVelocity(0, vec3[v_dir].x * DND_WANDERER_DASHVEL, vec3[v_dir].y * DND_WANDERER_DASHVEL, vec3[v_dir].z * DND_WANDERER_DASHVEL, 0, 0);
		Delay(const:1);
	}

	PlaySound(0, "Wanderer/DashFinish", 7);

	SetActorVelocity(0, 0, 0, 0, 0, 0);

	Delay(const:DND_WANDERER_DASH_DELAY);

	GiveInventory("TakeFlight", 1);
	GiveInventory("TakeThruObjects", 1);

	SetResultValue(0);
}

#endif