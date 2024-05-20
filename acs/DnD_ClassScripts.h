#ifndef DND_CLASS_SC_IN
#define DND_CLASS_SC_IN

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
    for(int i = 0; IsAlive() && CheckInventory("Hobo_ShotgunFrenzyTimer"); ++i) {
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

Script "DnD Berserker Perk25" (void) {
	while(CheckInventory("Berserker_DamageTimer")) {
		delay(const:1);
		TakeInventory("Berserker_DamageTimer", 1);
	}
	
	// heal for max hp 10% if was at full stack
	if(isAlive() && CheckInventory("Berserker_DamageTracker") == DND_BERSERKER_PERK25_MAXSTACKS) {
        HandleHealthPickup(GetSpawnHealth() / DND_BERSERKER_PERK25_HEALPERCENT, 0, 0);
	
		// fx
		Spawn("DarkHealEffectSpawner", GetActorX(0), GetActorY(0), GetActorZ(0), 0);
		LocalAmbientSound("Berserker/Heal", 127);
	}
	SetInventory("Berserker_DamageTracker", 0);
}

Script "DnD Berserker Perk50 Timer" (int this) {
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
		TakeInventory("Berserker_Perk50_Speed", 1);
		TakeInventory("Berserker_HitTracker", 1);
		SetInventory("Berserker_HitTimer", DND_BERSERKER_PERK50_TIMER);
	}
	// get rid of no roar at end of timer
	TakeInventory("Berserker_NoRoar", 1);
	SetInventory("Berserker_HitTimer", 0);
}

Script "DnD Berserker Perk5 Check" (void) {
	// berserker perk5 fail-safe checks for super weapons
	if(CheckInventory("Berserker_Perk5")) {
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
	if(!CheckInventory("Berserker_Perk5")) {
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

Script "DnD Cyborg Instability Timer" (void) {
	while(IsAlive() && CheckInventory("Cyborg_Instability_Timer")) {
		delay(const:1);
		TakeInventory("Cyborg_Instability_Timer", 1);
	}
	SetInventory("Cyborg_NoAnim", 0);
	SetInventory("Cyborg_InstabilityStack", 0);
	LocalAmbientSound("Instability/End", 127);
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
	int hpdamage = MonsterProperties[this - DND_MONSTERTID_BEGIN].maxhp / DND_WANDERER_EXP_DAMAGE;
	int exptid = DND_WANDERER_EXP_TID + target - P_TIDSTART;
	if(hpdamage <= 0)
		hpdamage = 1;
	
	SpawnForced("Wanderer_Explosion", GetActorX(this), GetActorY(this), GetActorZ(this) + GetActorProperty(this, APROP_HEIGHT) / 2 + 24.0, exptid);
	SetUserVariable(exptid, "user_dmg", hpdamage);
	SetActivator(exptid);
	SetPointer(AAPTR_TARGET, target);
	Thing_ChangeTID(exptid, 0);
}

#endif