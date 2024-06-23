#ifndef DND_WEAPON_SC_IN
#define DND_WEAPON_SC_IN

Script "DnD Prep Explosion" (void) {
	SetActorProperty(0, APROP_STAMINA, GetActorProperty(0, APROP_STAMINA) | DND_DAMAGEFLAG_ISRADIUSDMG);
	SetResultValue(0);
}

Script "DnD Pellet Count" (int base, int flags) {
	if(flags & DND_WDMG_USETARGET)
		SetactivatorToTarget(0);
	
	int pnum = PlayerNumber();
	SetResultValue(GetPelletCount(pnum, base)); 
}

// 0 is ammo1, 1 is ammo2
// returns 1 if it can fire
// int wepid, int ammo_which, int base_mult, int flags
Script "DnD Can Fire Weapon" (int wepid) {
	int pnum = PlayerNumber();

	while(IsAlive() && !CheckInventory("ResetCanFireLoop")) {
		int flags = CheckInventory("DnD_FiringFlags");
		bool canFire = false;
		bool canAltFire = false;
		bool canReload = false;

		if(!CheckInventory("ArtemisCheck")) {
			int amt1 = Weapons_Data[wepid].ammo_use1;
			str ammo1 = Weapons_Data[wepid].ammo_name1;

			int amt2 = Weapons_Data[wepid].ammo_use2;
			str ammo2 = Weapons_Data[wepid].ammo_name2;

			switch(wepid) {
				case DND_WEAPON_AKIMBOPISTOL:
					if(CheckInventory("AkimboFireMode") == 3)
						ammo2 = "AkimboClipRight";
					else
						ammo2 = "AkimboClipLeft";

					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt1, flags);
					canReload = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				break;

				// modify this to allow reload checking?
				case DND_WEAPON_SHOTGUN:
				case DND_WEAPON_PURIFIER:
				case DND_WEAPON_DEADLOCK:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt1, flags);
					canReload = CheckInventory(GetSpecialAmmoString(CheckInventory("SpecialAmmoMode_3"), AMMOINFO_NAME)) > CheckInventory(ammo2);
					if(flags & DND_CFW_HOLDFIREORRELOADCHECK) {
						amt1 = GetPlayerInput(-1, INPUT_BUTTONS);
						canReload &= (!CheckInventory(ammo2) && (amt1 & BT_ATTACK)) || (amt1 & BT_RELOAD);
					}
				break;
				case DND_WEAPON_SHOCKER:
					// reusing ammo variable here
					canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
					canFire = canAltFire && (IsQuestComplete(0, QUEST_KILLGODSLAYER) || CheckInventory("ShockerOverheat") <= 80);
					
					canAltFire &= CheckInventory("ShockerOverheat") >= 10;
				break;
				case DND_WEAPON_HADES:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt1, flags);
					canAltFire = canFire && !CheckInventory("HadesBurstFired") && !CheckInventory("HadesBurstCooldown");
					canReload = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				break;

				case DND_WEAPON_DESOLATOR:
					//if(ammo_which)
					//	res &= GetPlayerInput(-1, INPUT_BUTTONS) & BT_ATTACK;
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
					canFire &= IsQuestComplete(0, QUEST_KILLGODSLAYER) || (!CheckInventory("DesolatorCooldown") && CheckInventory("DesolatorOverheat") < 100);
				break;
				/*case DND_WEAPON_MINIGUN:
					if(ammo_which)
						res &= GetPlayerInput(-1, INPUT_BUTTONS) & BT_ATTACK;
				break;
				case DND_WEAPON_MPPB:
					if(ammo_which)
						res &= GetPlayerInput(-1, INPUT_BUTTONS) & BT_ATTACK;
				break;*/

				case DND_WEAPON_RIOTCANNON:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt1, flags);
					canReload = CheckInventory(GetSpecialAmmoString(CheckInventory("SpecialAmmoMode_4"), AMMOINFO_NAME)) > CheckInventory(ammo2);

					/*if(ammo_which == 1) {
						ammo = Weapons_Data[wepid].ammo_name2;
						amt = Weapons_Data[wepid].ammo_use2;
					}
					else if(ammo_which == 2) {
						ammo = Weapons_Data[wepid].ammo_name2;
						amt = Weapons_Data[wepid].ammo_use2;
						res &= GetPlayerInput(-1, INPUT_BUTTONS) & BT_ATTACK;
					}
					else if(ammo_which == 3) {
						res &= CheckInventory(GetSpecialAmmoString(CheckInventory("SpecialAmmoMode_4"), AMMOINFO_NAME)) > CheckInventory("RiotgunClip");
						SetResultValue(res);
						Terminate;
					}
					else if(ammo_which == 4) {
						res &= CheckInventory(GetSpecialAmmoString(CheckInventory("SpecialAmmoMode_4"), AMMOINFO_NAME)) > CheckInventory("RiotgunClip");
						SetResultValue(res);
						Terminate;
					}*/
				break;

				// standard weapons with no special alt fire that use the ammos, which can reload with mags
				case DND_WEAPON_MACHINEGUN:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt1, flags);
					canReload = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				break;

				// weapons with different cost altfires, that have clips and use the main ammo as means to reload
				case DND_WEAPON_ACIDRIFLE:
				case DND_WEAPON_FUSIONBLASTER:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt1, flags);
					canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt2, flags);
					canReload = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				break;

				case DND_WEAPON_FREEZER:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
					canFire &= IsQuestComplete(0, QUEST_KILLGODSLAYER) || (!CheckInventory("FreezerCooldown") && CheckInventory("FreezerOverheat") < 100);
					//if(ammo_which == 1)
					//	amt /= 2;
				break;
				case DND_WEAPON_VOIDCANNON:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
					canFire &= IsQuestComplete(0, QUEST_KILLGODSLAYER) || (!CheckInventory("VoidCannonCooldown") && CheckInventory("VoidCannonOverHeat") < 81);
				break;

				case DND_WEAPON_NUCLEARPLASMARIFLE:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
					canFire &= IsQuestComplete(0, QUEST_KILLGODSLAYER) || (!CheckInventory("PlasmaOverheatCooldown") && CheckInventory("PlasmaOverheat") < 100);
				break;
				case DND_WEAPON_FROSTFANG:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1 * 5, flags);
					canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt2, flags);

					/*if(ammo_which == 1)
						amt = Weapons_Data[wepid].ammo_use2;
					else if(ammo_which == 2)
						res &= GetPlayerInput(-1, INPUT_BUTTONS) & BT_ATTACK;*/
				break;
				case DND_WEAPON_REBOUNDER:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
					canFire &= IsQuestComplete(0, QUEST_KILLGODSLAYER) || (!CheckInventory("RebounderCooldown") && CheckInventory("RebounderOverheat") < 98);
					canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt2, flags);
					canAltFire &= IsQuestComplete(0, QUEST_KILLGODSLAYER) || (!CheckInventory("RebounderCooldown") && CheckInventory("RebounderOverheat") < 83);
				break;
				case DND_WEAPON_BASILISK:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
					canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt2, flags);
				break;

				case DND_WEAPON_BFG32768:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
					canFire &= IsQuestComplete(0, QUEST_KILLGODSLAYER) || (!CheckInventory("BFG32768Cooldown") && CheckInventory("BFG32768Overheat") < 100);
					/*if(ammo_which == 1)
						res &= GetPlayerInput(-1, INPUT_BUTTONS) & BT_ATTACK;
					res &= IsQuestComplete(0, QUEST_KILLGODSLAYER) || (!CheckInventory("BFG32768Cooldown") && CheckInventory("BFG32768Overheat") < 100);*/
				break;
				case DND_WEAPON_DEATHRAY:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
					canFire &= IsQuestComplete(0, QUEST_KILLGODSLAYER) || (!CheckInventory("DeathRayCooldown") && CheckInventory("DeathRayOverheat") < 100);
				break;
				case DND_WEAPON_IONCANNON:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
					canFire &= IsQuestComplete(0, QUEST_KILLGODSLAYER) || (!CheckInventory("IonCooldown") && CheckInventory("IonOverheat") < 100);
				break;

				case DND_WEAPON_SAWEDOFF:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt1, flags);
					canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt2, flags);
					canReload = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				break;
				case DND_WEAPON_DARKGLOVES:
					canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
					canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt2, flags);

					/*if(ammo_which == 1) {
						if(Weapons_Data[wepid].ammo_name2 != "")
							ammo = Weapons_Data[wepid].ammo_name2;
						amt = Weapons_Data[wepid].ammo_use2;
					}
					else if(ammo_which == 2)
						res &= GetPlayerInput(-1, INPUT_BUTTONS) & BT_ATTACK;*/
				break;

				default:
					canFire = ammo1 != "" && CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
					canAltFire = ammo2 != "" && CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt2, flags);
				break;
			}
		}
		else if(IsSlot8Weapon(wepid) || wepid == DND_WEAPON_DEMONHEART) {
			canFire = false;
			canAltFire = false;
			canReload = false;
		}
		else {
			canFire = true;
			canAltFire = true;
			canReload = true;
		}

		if(flags & DND_CFW_ALTFIRECHECK) {
			if(IsMeleeWeapon(wepid))
				canAltFire &= CheckInventory("Ability_Kick");
		}

		// these inventories get checked in weapon code
		if(canFire)
			GiveInventory("DnD_CanFire", 1);
		else
			TakeInventory("DnD_CanFire", 1);

		if(canAltFire)
			GiveInventory("DnD_CanAltFire", 1);
		else
			TakeInventory("DnD_CanAltFire", 1);

		if(canReload)
			GiveInventory("DnD_CanReload", 1);
		else
			TakeInventory("DnD_CanReload", 1);

		Delay(const:1);
	}

	TakeInventory("ResetCanFireLoop", 1);
}

Script "DnD Take Ammo From Weapon" (int wepid, int ammo_which, int amt) {
	int pnum = PlayerNumber();

	if(!CheckInventory("ArtemisCheck")) {
		str ammo = Weapons_Data[wepid].ammo_name1;

		switch(wepid) {
			default:
				if(ammo_which)
					ammo = Weapons_Data[wepid].ammo_name2;
			break;
		}

		TakeAmmoFromPlayer(pnum, wepid, ammo, amt);
	}
}

Script "DnD Handle Reload" (int wepid, int extra, int flags) {
	str toTake = "";
	str toGive = "";

	int pnum = PlayerNumber();
	int base = 0;

	switch(wepid) {
		case DND_WEAPON_AKIMBOPISTOL:
			toTake = "Clip";
			base = GetAmmoCapacity("AkimboClipLeft");
			if(!extra)
				toGive = "AkimboClipLeft";
			else
				toGive = "AkimboClipRight";
		break;
		case DND_WEAPON_ASSAULTRIFLE:
			toTake = "Clip";
			toGive = "MGClip5";
			base = GetAmmoCapacity("MGClip5");
		break;

		case DND_WEAPON_KILLSTORM:
			toTake = "Shell";
			toGive = "ShellSize_10";
			base = GetAmmoCapacity("ShellSize_10");
		break;

		case DND_WEAPON_ERASUS:
			toTake = "Shell";
			toGive = "ShellSize_2";
			base = GetAmmoCapacity("ShellSize_2");
		break;
		case DND_WEAPON_PLASMACANNON:
			toTake = "PCanAmmo";
			toGive = "PCanClip";
			base = GetAmmoCapacity("PCanClip");
		break;
		case DND_WEAPON_HADES:
			toTake = "HadesAmmo";
			toGive = "Shellsize_16";
			base = GetAmmoCapacity("Shellsize_16");
		break;

		case DND_WEAPON_MACHINEGUN:
			toTake = "Clip";
			toGive = "MGClip";
			base = GetAmmoCapacity("MGClip");
		break;
		case DND_WEAPON_HEAVYMACHINEGUN:
			toTake = "Clip";
			toGive = "MGClip2";
			base = GetAmmoCapacity("MGClip2");
		break;
		case DND_WEAPON_LEADSPITTER:
			toTake = "Clip";
			toGive = "MGClip3";
			base = GetAmmoCapacity("MGClip3");
		break;
		case DND_WEAPON_TEMPLARMG:
			toTake = "Clip";
			toGive = "MGClip4";
			base = GetAmmoCapacity("MGClip4");
		break;
		case DND_WEAPON_RIOTCANNON:
			if(!extra)
				toTake = "RiotgunShell";
			else if(extra == 1)
				toTake = "ExplodingShell";
			else
				toTake = "NitroShell";
			toGive = "RiotgunClip";
			base = GetAmmoCapacity("RiotgunClip");
		break;
		case DND_WEAPON_FUSIONBLASTER:
			toTake = "FusionCell";
			toGive = "MGClip6";
			base = GetAmmoCapacity("MGClip6");
		break;

		case DND_WEAPON_VINDICATOR:
			toTake = "FlakShell";
			toGive = "ShellSize_18";
			base = GetAmmoCapacity("ShellSize_18");
		break;

		case DND_WEAPON_RHINORIFLE:
			toTake = "Clip";
			toGive = "MGClip7";
			base = GetAmmoCapacity("MGClip7");
		break;

		case DND_WEAPON_FLAMETHROWER:
			toTake = "Fuel";
			toGive = "FuelClip";
			base = GetAmmoCapacity("FuelClip");
		break;

		case DND_WEAPON_SAWEDOFF:
			toTake = "SawedoffShell";
			toGive = "SawedoffCounter";
			base = GetAmmoCapacity("SawedoffCounter");
		break;
	}

	int tmp = CheckInventory(toGive);
	int amt = base - tmp;
	
	if(CheckInventory(toTake) < base)
		GiveInventory(toGive, CheckInventory(toTake));
	else
		GiveInventory(toGive, amt);

	if(!(flags & DND_RWF_NOTAKEAMMO))
		TakeAmmoFromPlayer(pnum, wepid, toTake, amt);
}

Script "DnD Overheat Reduction" (int index, int rate) {
	int d = rate >> 16;
	rate &= 0x0000FFFF;

	int pnum = PlayerNumber();

	if(HasPlayerPowerSet(pnum, PPOWER_OVERHEATGOFAST)) {
		int temp = d * (100 - OVERHEAT_DISS_FACTOR) / 100;
		if(!temp) {
			rate = rate * (100 + OVERHEAT_DISS_FACTOR) / 100;
			// if not 1 then we also reduce the delay if the difference is that small
			if(d != 1)
				d = 1;
		}
		else
			d = temp;
	}

	// prevent multiple copies, also dont keep this busy if we got no overheat bonus
	if(!IsSet(PlayerRunsOverheat[pnum], index) && !IsQuestComplete(0, QUEST_KILLGODSLAYER)) {
		PlayerRunsOverheat[pnum] = SetBit(PlayerRunsOverheat[pnum], index);
		while(CheckInventory(WeaponOverheatItems[index])) {
			if(CheckInventory("DnD_OverheatCanReduce"))
				TakeInventory(WeaponOverheatItems[index], rate);
			Delay(d);
		}
		PlayerRunsOverheat[pnum] = ClearBit(PlayerRunsOverheat[pnum], index);
	}
}

Script "DnD Sickle Check Res" (int base) {
	int res = 0;
	int hit = GetActorProperty(0, APROP_TRACERTID);

	if(IsMonster(hit) && !IsActorAlive(hit)) {
		SetActivatorToTarget(0);
		base = (base << 16) / 100;
		res = RunLuckBasedChance(PlayerNumber(), base, DND_LUCK_OUTCOME_GAIN);
	}

	SetResultValue(res);
}

Script "DnD Sickle Mass Res" (int base, int dist) {
	int owner = GetActorProperty(0, APROP_TARGETTID);
	int pnum = owner - P_TIDSTART;

	dist <<= 16;
	base = (base << 16) / 100;

	for(int mn = 0; mn < DnD_TID_Counter[DND_TID_MONSTER]; ++mn) {
		int i = UsedMonsterTIDs[mn];
		if(!isActorAlive(i) && RunLuckBasedChance(pnum, base, DND_LUCK_OUTCOME_GAIN) && fdistance(0, i) <= dist) {
			ACS_NamedExecuteAlways("DnD Resurrect Checker", 0, TICRATE, i);
		}
	}
}

Script "DnD Inferno Debuff FX" (void) CLIENTSIDE {
	int r = GetActorProperty(0, APROP_RADIUS);
	int h = GetActorProperty(0, APROP_HEIGHT);
	GiveInventory("InfernoSwordDebuff", 1);
	while(isAlive() && CheckInventory("InfernoSwordDebuff")) {
		for(int i = 0; i < 3; ++i)
			SpawnForced("InfernoDebuffFX", GetActorX(0) + random(-r / 2, r / 2), GetActorY(0) + random(-r / 2, r / 2), GetActorZ(0) + random(16.0, 3 * h / 4));
		Delay(const:3);
	}
}

Script "DnD Take Inferno Debuff" (void) {
	int tid = GetActorProperty(0, APROP_TRACERTID);
	if(tid) {
		SetActivator(tid);
		TakeInventory("InfernoSwordDebuff", 1);
		ACS_NamedExecuteWithResult("DnD Take Inferno Debuff CS");
	}
	SetResultValue(0);
}

Script "DnD Take Inferno Debuff CS" (void) CLIENTSIDE {
	TakeInventory("InfernoSwordDebuff", 1);
	SetResultValue(0);
}

Script "DND Acid Rifle Bolt Stick" (int type) {
	int res = 0;
	int tracer = GetActorProperty(0, APROP_TRACERTID);
	if(!type)
		res = GetActorProperty(tracer, APROP_HEIGHT) >> 16;
	else
		res = IsActorAlive(tracer);
	SetResultValue(res);
}

Script "DND Demon Seal FX Spawn" (int tracer) CLIENTSIDE {
	Spawn("DemonSealDebuffFXSpawner", GetActorX(tracer), GetActorY(tracer), GetActorZ(tracer), 0);
}

Script "DND Demon Seal FX" (void) {
	int tracer = ActivatorTID();
	if(!CheckActorInventory(tracer, "DemonSealDamageDebuff")) {
		GiveActorInventory(tracer, "DemonSealDamageDebuff", 1);
		GiveActorInventory(tracer, "DemonSealResistDebuff", 1);
		if(isActorAlive(tracer)) {
			PlaySound(tracer, "DemonSeal/debuff", 6, 0.9, 0, 1.0);
			// loops at 4.8 seconds, zandro doesnt know how to stop looping this sound for whatever reason...
			//PlaySound(tracer, "DemonSeal/debuffloop", 7, 0.5, 0, 1.3);
		}
		while(isActorAlive(tracer) && CheckActorInventory(tracer, "DemonSealDamageDebuff")) {
			ACS_NamedExecuteAlways("DND Demon Seal FX Spawn", 0, tracer);
			Delay(1);
		}
		//StopSound(tracer, 7);
	}
}

Script "DND Desolator FX Spawn" (int tracer) CLIENTSIDE {
	Spawn("DesolatorDebuffFXSpawner", GetActorX(tracer), GetActorY(tracer), GetActorZ(tracer), 0);
}

Script "DND Desolator Debuff FX" (int tracer) {
	while(isActorAlive(tracer) && CheckActorInventory(tracer, "DesolatorStackTimer")) {
		ACS_NamedExecuteAlways("DND Desolator FX Spawn", 0, tracer);
		TakeActorInventory(tracer, "DesolatorStackTimer", 1);
		Delay(const:1);
	}
	TakeActorInventory(tracer, "DesolatorStackCounter", 99);
}

Script "DnD Trail Spawner" (int actortype, int density, int target) {
	int ptid = !target ? GetActorProperty(0, APROP_TARGETTID) : target;
	ACS_NamedExecuteWithResult("DND Trail Spawner CS", actortype, density, ptid);
}

Script "DND Trail Spawner CS" (int actortype, int density, int ptid) CLIENTSIDE {
	//printbold(d:ptid, s: " ", s:GetActorClass(ptid), s: " ", s:GetActorClass(0));
	if(DnD_TrailDef[actortype].waittime)
		Delay(DnD_TrailDef[actortype].waittime);
	
    int plX; int plY;  int plZ;
    int grX; int grY;  int grZ;
    int vX;  int vY;   int vZ;
    int nX;  int nY;   int nZ;
    int bX;  int bY;   int bZ;
    int magnitude;
    int pointCount;
    int pointOffset;
	
    plX = GetActorX(0); 
	grX = GetActorX(ptid);
	
    plY = GetActorY(0);
	grY  = GetActorY(ptid);
    
	plZ = GetActorZ(0) + (GetActorProperty(0, APROP_HEIGHT) >> 1) + DnD_TrailDef[actortype].offset_z; 
	grZ  = GetActorZ(ptid) + (GetActorProperty(ptid, APROP_HEIGHT) >> 1);
	
    vX   = grX - plX;       vY   = grY - plY;       vZ   = grZ - plZ;

    magnitude = magnitudeThree(vX >> 16, vY >> 16, vZ >> 16);

    pointCount  = magnitude / density;
    pointOffset = magnitude - (pointCount * density);

	int maxdiff = DnD_TrailDef[actortype].maxdiff;
    if (magnitude != 0) {
        nX = vX / magnitude; nY = vY / magnitude; nZ = vZ / magnitude;

        int i; int j;
        for (i = 1; i < pointCount; i++) {
            j = (i * density) + pointOffset;

			bX = (nX * j) + plX;
			bY = (nY * j) + plY;
			bZ = (nZ * j) + plZ;
			
			if(maxdiff) {
				bX += (random(0, 1) ? random(-maxdiff, maxdiff) : 0);
				bY += (random(0, 1) ? random(-maxdiff, maxdiff) : 0);
				bZ += (random(0, 1) ? random(-maxdiff, maxdiff) : 0);
			}

			Spawn(DnD_TrailDef[actortype].actor, bX, bY, bZ, 0, VectorAngle(-vX, -vY) >> 8);
        }
    }
	SetResultValue(0);
}

Script "DND Charon Trail Spawner" (int plX, int plY, int plZ, int target) CLIENTSIDE {
	int actortype = target >> 16;
	target &= 0xFFFF;
	int ptid = !target ? GetActorProperty(0, APROP_TARGETTID) : target;
	
    int grX; int grY;  int grZ;
    int vX;  int vY;   int vZ;
    int nX;  int nY;   int nZ;
    int bX;  int bY;   int bZ;
    int magnitude;
    int pointCount;
    int pointOffset;
 
	grX = GetActorX(ptid);
	grY = GetActorY(ptid);
	grZ = GetActorZ(ptid) + (GetActorProperty(ptid, APROP_HEIGHT) >> 1) + DnD_TrailDef[actortype].offset_z;
	
	plX <<= 16;
	plY <<= 16;
	plZ <<= 16;
	
    vX   = grX - plX;       vY   = grY - plY;       vZ   = grZ - plZ;

    magnitude = magnitudeThree(vX >> 16, vY >> 16, vZ >> 16);

    pointCount  = magnitude / CHARON_PARTICLE_DENSITY;
    pointOffset = magnitude - (pointCount * CHARON_PARTICLE_DENSITY);

	int maxdiff = DnD_TrailDef[actortype].maxdiff;
    if (magnitude != 0) {
        nX = vX / magnitude; nY = vY / magnitude; nZ = vZ / magnitude;

        int i; int j;
        for (i = 1; i < pointCount - 3; i++) {
            j = (i * CHARON_PARTICLE_DENSITY) + pointOffset;

			bX = (nX * j) + plX;
			bY = (nY * j) + plY;
			bZ = (nZ * j) + plZ;
			
			if(maxdiff) {
				bX += (random(0, 1) ? random(-maxdiff, maxdiff) : 0);
				bY += (random(0, 1) ? random(-maxdiff, maxdiff) : 0);
				bZ += (random(0, 1) ? random(-maxdiff, maxdiff) : 0);
			}

			Spawn(DnD_TrailDef[actortype].actor, bX, bY, bZ, 0, VectorAngle(-vX, -vY) >> 8);
        }
    }
}

Script "DnD Dark Lance Aligner" (int xd, int yd, int zd, int tics) CLIENTSIDE {
	int owner = GetActorProperty(0, APROP_TARGETTID);
	
	for(int i = 0; i < tics; ++i) {
		// get direction vector from owner constantly
		int a = GetActorAngle(owner);
		int p = GetActorPitch(owner);
		int vx = FixedMul(cos(p), cos(a));
		int vy = FixedMul(cos(p), sin(a));
		int vz = -sin(p);
		
		int x = GetActorX(owner);
		int y = GetActorY(owner);
		int z = GetActorZ(owner);
				
		SetActorPosition(0, x + vx * xd + vy * yd, y + vy * xd - vx * yd, z + 32.0 + vz * zd, 0);
		Delay(const:1);
	}
	SetResultValue(0);
}

Script "DnD Lance Stack Buffer" (void) {
	SetInventory("LanceBuffer", CheckInventory("LanceStacks"));
	SetInventory("LanceStacks", 0);
	SetResultValue(0);
}

Script "DnD Dark Lance Shred" (int dmg, int owner, int stacks, int victim) {
	// first time engage
	if(!victim)
		victim = GetActorProperty(0, APROP_TRACERTID);
	
	if(isMonster(victim)) {
		// start procedure --- if owner is left blank that means this is initiated the first time from us
		if(!owner)
			owner = GetActorProperty(0, APROP_TARGETTID);
			
		SetActivator(owner);
		
		int pnum = owner - P_TIDSTART;
		
		// special fx script -- since dot has duration it scales off of dot duration increase
		int time_lim = DARKLANCE_ALT_DURATION_TICS * (100 + GetPlayerAttributeValue(pnum, INV_EX_DOTDURATION)) / 100;
		ACS_NamedExecuteWithResult("DnD Dark Lance Shred FX", victim, pnum, time_lim);
		time_lim /= DARKLANCE_TICKS;
		
		// the affliction does 15 ticks, each stack provides 1% more damage. 15 * (50 * (100 + stacks) / 100) = 1500 max damage default at max stacks 
		if(!stacks)
			stacks = CheckInventory("LanceBuffer");

		if(!dmg)
			dmg = RetrieveWeaponDamage(pnum, DND_WEAPON_DARKLANCE, DND_DMGID_1, DND_DAMAGECATEGORY_OCCULT, DND_DAMAGEFLAG_ISDAMAGEOVERTIME, 0);
		
		// apply stacks and dot multi
		int calc_dmg = dmg * (100 + stacks) / 100;
		calc_dmg = calc_dmg * (100 + GetPlayerAttributeValue(pnum, INV_DOTMULTI)) / 100;
			
		PlaySound(victim, "Darklance/Affliction", CHAN_ITEM, 1.0);
		
		bool terminated = false;
		int i;
		for(i = 0; !terminated && i < time_lim; ++i) {
			// flat dot damage added after stacks are multiplied, because otherwise it's too overpowered...
			int tmp_dmg;
			if(isActorAlive(victim) && (tmp_dmg = HandleDamageDeal(owner, victim, calc_dmg, DND_DAMAGETYPE_OCCULT, DND_WEAPON_DARKLANCE, DND_DAMAGEFLAG_ISDAMAGEOVERTIME | DND_DAMAGEFLAG_NOPUSH, -1, -1, -1, DND_ACTORFLAG_FOILINVUL)) > 0)
				Thing_Damage2(victim, tmp_dmg, "SkipHandle");
			else
				terminated = true;
			
			Delay(const:DARKLANCE_TICKS);
		}
		
		// find nearest monster to bounce to
		if(terminated) {
			// phase out the previous victim
			ACS_NamedExecuteAlways("DnD Monster RIP", 0, victim);
			
			GiveActorInventory(owner, "DemonBlood", 1);
		
			// find new victim
			int dist = 0;
			int prev_victim = victim;
			int curr_dist = INT_MAX;
			victim = -1;
			
			for(int mn = 0; mn < DnD_TID_Counter[DND_TID_MONSTER]; ++mn) {
				i = UsedMonsterTIDs[mn];
				if(IsActorAlive(i) && CheckFlag(i, "SHOOTABLE") && IsMonster(i)) {
					dist = fdistance(prev_victim, i);
					if(dist < DARKLANCE_BOUNCE_DIST && dist < curr_dist && CheckSight(prev_victim, i, CSF_NOBLOCKALL)) {
						victim = i;
						curr_dist = dist;
					}
				}
			}
			
			// found a monster suitable for jump, so go to this one now
			if(victim != -1)
				ACS_NamedExecuteWithResult("DnD Dark Lance Shred", dmg, owner, stacks, victim);
		}
	}

	SetResultValue(0);
}

Script "DnD Dark Lance Shred FX" (int tid, int pnum, int time_lim) CLIENTSIDE {
	SpawnForced("DarkLanceShredder_FX", GetActorX(tid), GetActorY(tid), GetActorZ(tid), DARKLANCE_SHREDDER + pnum);
	SetActivator(DARKLANCE_SHREDDER + pnum);
	Thing_ChangeTID(0, 0);
	
	SetActorProperty(0, APROP_SCALEX, (GetActorProperty(tid, APROP_RADIUS) + 32.0) / 128);
	SetActorProperty(0, APROP_SCALEY, (GetActorProperty(tid, APROP_HEIGHT) + 32.0) / 128);
	
	int h = GetActorProperty(tid, APROP_HEIGHT);
	
	for(int i = 0; isActorAlive(tid) && i < time_lim; ++i) {
		SetActorPosition(0, GetActorX(tid), GetActorY(tid), GetActorZ(tid) + h / 2, 0);
		Delay(const:1);
	}
	
	// if dead, vanish with special fx otherwise just remove
	if(!isActorAlive(tid)) {
		SetActorState(0, "Vanish", 0);
		SpawnForced("DarkLanceConsumeFX", GetActorX(tid), GetActorY(tid), GetActorZ(tid) + h / 2, 0);
	}
	else
		Thing_Remove(0);
	
	SetResultValue(0);
}

// currently does 90 degree scan from players fov
Script "DnD Do Scan Attack" (int dmg, int damage_type, int tracer_count, int flags) {
	Do_Scan_Attack(dmg, damage_type, tracer_count, flags);
}

Script "DND Thunderstaff Bolts" (void) {
	int dist = INT_MAX, i, j, k, temp;
	// scan all monsters for the item ThunderTarget -- that implies they are candidates having passed range check
	static dist_tid_pair_T tlist[MAXPLAYERS][DND_THUNDERSTAFF_MAXTARGETS];
	
	int owner = GetActorProperty(0, APROP_TARGETTID);
	int pnum = owner - P_TIDSTART;
	
	// init list
	for(i = 0; i < DND_THUNDERSTAFF_MAXTARGETS; ++i) {
		tlist[pnum][i].tid = 0;
		tlist[pnum][i].dist = INT_MAX;
	}
	int tcount = 0;
	int tidlim = 0;
	// pick N closest targets as a list of targets to hit
	for(int mn = 0; mn < DnD_TID_Counter[DND_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		if(CheckActorInventory(i, "ThunderTarget") && IsActorAlive(i) && CheckFlag(i, "SHOOTABLE")) {
			dist = fdistance(0, i);
			// insert sorted
			if(tcount < DND_THUNDERSTAFF_MAXTARGETS)
				temp = tcount;
			else
				temp = DND_THUNDERSTAFF_MAXTARGETS;
			// while our calc dist > alloc dist, keep going -- we add things to the end
			// if we come by a point where we are smaller, shift things
			for(j = 0; j < temp && dist > tlist[pnum][j].dist; ++j);

			// we know where to add, check if we must shift (if we should)
			if(j < temp) {
				// less, so that means we are in-between things
				// push everything for insertion
				// this is needed to move in 0 index shifts
				if(temp == DND_THUNDERSTAFF_MAXTARGETS)
					--temp;
				
				for(k = temp; k > j; --k) {
					// slide data
					tlist[pnum][k].dist = tlist[pnum][k - 1].dist;
					tlist[pnum][k].tid = tlist[pnum][k - 1].tid;
				}
			}
			
			tlist[pnum][j].dist = dist;
			tlist[pnum][j].tid = i;
			
			if(tcount < DND_THUNDERSTAFF_MAXTARGETS)
				++tcount;
		}
	}
	
	if(tcount) {
		int actor_flags = ScanActorFlags();
	
		for(i = 0; i < DND_THUNDERSTAFF_MAXTARGETS; ++i) {
			if(tlist[pnum][i].tid)
				ACS_NamedExecuteWithResult("DND Trail Spawner", DND_TRAILACTOR_THUNDERZAP, 16, tlist[pnum][i].tid);
		}
		
		int px = GetActorX(0);
		int py = GetActorY(0);
		int pz = GetActorZ(0);
		
		// damage credit
		SetActivatorToTarget(0);
		dist = RetrieveWeaponDamage(pnum, DND_WEAPON_THUNDERSTAFF, DND_DMGID_1, DND_DAMAGECATEGORY_LIGHTNING, 0, 0);

		for(i = 0; i < DND_THUNDERSTAFF_MAXTARGETS; ++i) {
			if(tlist[pnum][i].tid) {
				// store damage in tcount, in case we crit we need to restore it afterwards
				tcount = dist;
				
				// do not tokenize to not affect projectiles and clear crit flag after consumption
				if(actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT)
					actor_flags ^= DND_ACTORFLAG_CONFIRMEDCRIT;
				
				mn = HandleDamageDeal(owner, tlist[pnum][i].tid, dist, DND_DAMAGETYPE_LIGHTNING, DND_WEAPON_THUNDERSTAFF, 0, px, py, pz, actor_flags);
				if(mn > 0)
					Thing_Damage2(tlist[pnum][i].tid, mn, "SkipHandle");

				dist = tcount;
			}
		}
	}
}

Script "DND ThunderStaff FX Spawn" (int target) CLIENTSIDE {
	SpawnForced("ThunderStaffLightningFX", GetActorX(target), GetActorY(target), GetActorFloorZ(target));
}

Script "DND Thunder Ring" (int radius) CLIENTSIDE {
	int tid = ActivatorTID(), i; // Owner TID
	int newtid = tid + DND_THUNDER_RING_TIDSTART + 32 * ((tid + 1) % 100);
	
	for(i = 0; i < 32; ++i)
		Thing_Remove(newtid + i);
	
	int cx, cy, cz, r_add, angadd = 0;
	int pang = GetActorAngle(tid);
	cz = GetActorZ(tid);
	
	for(i = 0; i < 32; ++i) {
		cx = GetActorX(tid) + radius * cos(pang);
		cy = GetActorY(tid) + radius * sin(pang);
		pang += 1.0 / 32;
		Spawn("ThunderStaffRingFX", cx, cy, cz + 26.825, newtid + i, 0);
		SetActivator(newtid + i);
		SetPointer(AAPTR_TARGET, tid);
		SetActivator(tid);
	}
	Delay(const:1);
	while(CheckInventory("ThunderRangeCounter") && IsAlive() && !CheckInventory("ThunderStopRing")) {
		r_add = DND_THUNDER_RADIUSPERCOUNT * CheckInventory("ThunderRangeCounter");
		pang = 0;
		for(i = 0; i < 32; ++i) {
			pang += 1.0 / 32;
			if(GetActorFloorZ(newtid + i) > GetActorZ(tid))
				cz = GetActorFloorZ(newtid + i);
			cx = GetActorX(tid) + (radius + r_add) * cos(pang + angadd);
			cy = GetActorY(tid) + (radius + r_add) * sin(pang + angadd);
			SetActorPosition(newtid + i, cx, cy, cz + 26.825, 0);
		}
		angadd += 0.025;
		angadd %= 1.0;
		Delay(const:1);
	}
	for(i = 0; i < 32; ++i)
		SetActorState(newtid + i, "Fade");
}

Script "DND Thunderstaff Lightning" (void) {
	int i, this = ActivatorTID();
	int pnum = PlayerNumber();
	int dmg = RetrieveWeaponDamage(pnum, DND_WEAPON_THUNDERSTAFF, DND_DMGID_3, DND_DAMAGECATEGORY_LIGHTNING, 0, 0);
	int actor_flags = 0;
	int scan_amt = 0;
	int adist;
	
	for(int mn = 0; scan_amt < DND_THUNDERSTAFF_LIMIT && mn < DnD_TID_Counter[DND_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		if(CheckActorInventory(i, "ThunderStrike") && IsActorAlive(i) && CheckFlag(i, "SHOOTABLE")) {
			ACS_NamedExecuteAlways("DND ThunderStaff FX Spawn", 0, i);
			
			 if(actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT)
				actor_flags ^= DND_ACTORFLAG_CONFIRMEDCRIT;

			adist = HandleDamageDeal(this, i, dmg, DND_DAMAGETYPE_LIGHTNING, DND_WEAPON_THUNDERSTAFF, DND_DAMAGEFLAG_NOPUSH, 0, 0, 0, actor_flags);
			if(adist)
				Thing_Damage2(i, adist, "SkipHandle");
			
			// take the range checker
			TakeActorInventory(i, "ThunderStrike", 1);

			// prox hurt player based on distance to monster
			if(isActorAlive(this) && (adist = AproxDistance(GetActorX(0) - GetActorX(i), GetActorY(0) - GetActorY(i))) <= THUNDERSTAFF_SELFDMG_DIST) {
				// reduce base damage from a really high number to a rather tolerable one -- base is 750...
				int self_dmg = (dmg / 4) * ((THUNDERSTAFF_SELFDMG_DIST - adist) >> 16) / (THUNDERSTAFF_SELFDMG_DIST >> 16);
				
				// push with some greater force
				HandleDamagePush(self_dmg * 4, GetActorX(i), GetActorY(i), GetActorZ(i), this);

				// handle player's self explosion resists here
				self_dmg = HandlePlayerSelfDamage(pnum, self_dmg, DND_DAMAGETYPE_LIGHTNING, DND_WEAPON_THUNDERSTAFF, 0, 0);

				Thing_Damage2(this, self_dmg, "SkipHandle");
			}

			++scan_amt;
		}
	}
}

Script "DND Thunder Range" (void) {
	// adding a +18 to match the center radius of the special fx
	SetResultValue(DND_THUNDERSTAFF_BASERANGE + DND_THUNDER_RADIUSPERCOUNT * CheckInventory("ThunderRangeCounter") + 16);
}

Script "DnD DeathRay Marker TID" (void) {
	Thing_ChangeTID(0, GetActorProperty(0, APROP_TARGETTID) - P_TIDSTART + DEATHRAY_MARKER_TID);
}

Script "DnD DeathRay Laser Aim" (int adj) {
	int p_actor = GetActorProperty(0, APROP_TARGETTID);
	int target = p_actor - P_TIDSTART + DEATHRAY_MARKER_TID;
	if(!ThingCount(T_NONE, target))
		Terminate;
	int xdiff = GetActorX(target) - GetActorX(0);
	int ydiff = GetActorY(target) - GetActorY(0);
	int angle = VectorAngle(xdiff, ydiff);
	int pitch = VectorPitch(0, target, xdiff, ydiff, adj);
	
	// make the proj itself
	
	SpawnProjectile(0, "DeathRayLaser", angle, 0, 0, 0, TEMPORARY_ATTACK_TID);
	
	SetActorAngle(TEMPORARY_ATTACK_TID, angle);
	SetActorPitch(TEMPORARY_ATTACK_TID, pitch);
	SetActorVelocity(TEMPORARY_ATTACK_TID, FixedMul(cos(angle), cos(pitch)) * 512, FixedMul(sin(angle), cos(pitch)) * 512, sin(-pitch) * 512, 0, 0);
	
	SetActorProperty(TEMPORARY_ATTACK_TID, APROP_TARGETTID, p_actor);
	
	SetActorProperty(TEMPORARY_ATTACK_TID, APROP_ACCURACY, GetActorProperty(0, APROP_ACCURACY));
	SetUserVariable(TEMPORARY_ATTACK_TID, "user_x", GetActorX(0));
	SetUserVariable(TEMPORARY_ATTACK_TID, "user_y", GetActorY(0));
	SetUserVariable(TEMPORARY_ATTACK_TID, "user_z", GetActorZ(0));
	
	SetActivator(TEMPORARY_ATTACK_TID);

	// this is the line that makes the owner the true owner
	SetPointer(AAPTR_TARGET, p_actor);
	
	Thing_ChangeTID(TEMPORARY_ATTACK_TID, 0);
}

Script "DnD Deathray Laser Trail" (int density, int grX, int grY, int grZ) CLIENTSIDE {
	int oX = GetActorX(0);
	int oY = GetActorY(0);
	int oZ = GetActorZ(0);

    int vX;  int vY;   int vZ;
    int nX;  int nY;   int nZ;
    int bX;  int bY;   int bZ;
    int magnitude;
    int pointCount;
    int pointOffset;
	
    //grX <<= 16;
    //grY <<= 16;
    //grZ <<= 16;
	
    vX   = grX - oX;
	vY   = grY - oY;
	vZ   = grZ - oZ;
	
    magnitude = magnitudeThree(vX >> 16, vY >> 16, vZ >> 16);
    pointCount  = magnitude / density;
    pointOffset = magnitude - (pointCount * density);

    if (magnitude != 0) {
        nX = vX / magnitude; nY = vY / magnitude; nZ = vZ / magnitude;

        int i; int j;
        for (i = 1; i < pointCount; i++) {
            j = (i * density) + pointOffset;
            bX = (nX * j) + oX;
            bY = (nY * j) + oY;
            bZ = (nZ * j) + oZ;

            Spawn("DeathRayLaserTrail", bX, bY, bZ);
        }
    }
	
	SetResultValue(0);
}

Script "DND HeavyGL Check" (int x, int y, int z) {
	int res = 1;
	if (!(GetActorZ(0) - GetActorFloorZ(0) > 4.0))
		res = sqrt(x*x + y*y + z*z);
	SetResultValue(res);
}

Script "DnD Pull Towards Self" (int force, int pull_dist, int flags) {
	int x = GetActorX(0);
	int y = GetActorY(0);
	int z = GetActorZ(0);
	
	// make fixed
	bool adj_fixed = false;
	if(force < 1.0)
		force <<= 16;
	else
		adj_fixed = true;
		
	pull_dist <<= 16;
	for(int mn = 0; mn < DnD_TID_Counter[DND_TID_MONSTER]; ++mn) {
		int i = UsedMonsterTIDs[mn];
		int mass = GetActorProperty(i, APROP_MASS);
		
		// overflow check, we divide by x2 of mass so
		if(mass >= INT_MAX >> 1)
			continue;
		
		// flags has can pull dead, we dont need to check for alive or not
		if(((flags & DND_PULLF_CANPULLDEAD) || isActorAlive(i)) && fdistance(0, i) <= pull_dist && CheckSight(i, 0, CSF_NOBLOCKALL)) {
			// get angle to face and thrust it towards this
			int fx = x;
			int fy = y;
			
			if(flags & DND_PULLF_REVERSEDIR) {
				fx = GetActorX(i) - x;
				fy = GetActorY(i) - y;
			}
			else {
				fx -= GetActorX(i);
				fy -= GetActorY(i);
			}
			
			int ang = VectorAngle(fx, fy);
			int mforce = force / (2 * mass);
			if(adj_fixed && mforce < 1.0)
				mforce <<= 16;
			if(mforce > DND_MAX_PULLFORCE)
				mforce = DND_MAX_PULLFORCE;
			SetActorVelocity(i, FixedMul(mforce, cos(ang)), FixedMul(mforce, sin(ang)), 0, true, false);
		}
	}
	SetResultValue(0);
}

Script "DnD Gravdis Thrust" (int pull_count, int force) {
	int x = GetActorX(0);
	int y = GetActorY(0);
	//int z = GetActorZ(0);
	
	// make fixed
	force <<= 16;
	int j = pull_count;
	while(j) {
		for(int mn = 0; mn < DnD_TID_Counter[DND_TID_MONSTER]; ++mn) {
			int i = UsedMonsterTIDs[mn];
			if(CheckActorInventory(i, "GravdisDebuff") && isActorAlive(i) && !CheckActorInventory(i, "GravdisImmunity")) {
				// get angle to face and thrust it towards this
				int fx = x - GetActorX(i);
				int fy = y - GetActorY(i);
				int ang = VectorAngle(fx, fy);
				int mforce = Clamp_Between(force / (2 * GetActorProperty(i, APROP_MASS)), GRAVDIS_MINPULLVEL, GRAVDIS_MAXPULLVEL);
				SetActorVelocity(i, FixedMul(mforce, cos(ang)), FixedMul(mforce, sin(ang)), 0, true, false);
			}
		}
		Delay(const:2);
		--j;
	}
	SetResultValue(0);
}

Script "DnD Gravdis Debuff" (int base_dmg) {
	bool proj_crit = !ActivatorTID() && GetActorProperty(0, APROP_ACCURACY) == DND_CRIT_TOKEN;
	int owner = GetActorProperty(0, APROP_TARGETTID);
	int i, m_id;
	int dmg = RetrieveWeaponDamage(owner - P_TIDSTART, DND_WEAPON_GRAVDIS, DND_DMGID_0, DND_DAMAGECATEGORY_BULLET, 0, 0);
	bool got_one = false;
	
	for(int mn = 0; mn < DnD_TID_Counter[DND_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		// to be affected by gravdis debuff
		if(CheckActorInventory(i, "GravdisToken") && IsActorAlive(i) && !CheckActorInventory(i, "GravdisImmunity")) {
			TakeActorInventory(i, "GravdisToken", 1);
			
			m_id = MonsterProperties[i - DND_MONSTERTID_BEGIN].id;
			
			// dont affect teleporting / unshootable things or dungeon bosses --- no stun, no damage etc.
			if(!CheckFlag(i, "SHOOTABLE") || isUniqueBossMonster(m_id))
				continue;
			
			// this allows other people or even the same player to elevate monsters higher and keep stunning them for longer for more damage potential
			if(!CheckActorInventory(i, "GravdisDebuff"))
				ACS_NamedExecuteWithResult("DnD Gravdis Flinger", i, owner | (proj_crit << 17), dmg, base_dmg);
			else
				ACS_NamedExecuteWithResult("DnD Gravdis Elevate", i, dmg, base_dmg);
				
			// bosses arent as affected by this
			if(!IsMonsterIdBoss(m_id))
				SetActorInventory(i, "GravdisDebuff", GRAVDIS_DELAY_TIME);
			else
				SetActorInventory(i, "GravdisDebuff", GRAVDIS_DELAY_TIME / 5);
			SetActorState(i, "GravdisFling", 0);
			PlaySound(i, "Gravdis/Grab", 7, 1.0);
			got_one = true;
		}
	}
	
	if(got_one)
		GiveActorInventory(owner, "GravdisCanAlt", 1);

	SetResultValue(0);
}

Script "DnD Gravdis Flinger" (int victim, int dmg_source, int damage, int base_dmg) {
	//bool projcrit = victim >> 17;
	victim &= 0xFFFF;
	int m = GetActorProperty(victim, APROP_MASS);
	if(m < INT_MAX / damage * base_dmg)
		m = m * base_dmg / damage;
	m = Clamp_Between(m / 12, 16, 4096);
	bool was_flying = CheckFlag(victim, "NOGRAVITY");
	
	GiveActorInventory(victim, "AllowFlight", 1);
	if(was_flying)
		GiveActorInventory(victim, "TakeFloat", 1);
	
	for(int i = 0; i < HALF_TICRATE; ++i) {
		// adds vel on top
		SetActorVelocity(victim, 0, 0, GRAVDIS_FLING_FACTOR * cos((0.25 / (HALF_TICRATE - 1)) * i) / m, true, false);
		Delay(const:1);
	}
	
	SetActorVelocity(victim, 0, 0, 0, false, false);
	
	// damage portion of gravdis
	// wait for the effect
	while(CheckActorInventory(victim, "GravdisDebuff")) {
		TakeActorInventory(victim, "GravdisDebuff", 1);
		
		if(CheckActorInventory(dmg_source, "GravdisDunk")) {
			PlaySound(victim, "Gravdis/TimeOut", 7, 1.0);
			ACS_NamedExecuteWithResult("DnD Gravdis FX Spawner", victim, 0);
			Delay(const:7);
			SetActorInventory(victim, "GravdisDebuff", 0);
			break;
		}
		
		if(CheckActorInventory(victim, "GravdisDebuff") == 1) {
			PlaySound(victim, "Gravdis/TimeOut", 7, 1.0);
			ACS_NamedExecuteWithResult("DnD Gravdis FX Spawner", victim, 0);
		}
		Delay(const:7);
	}
	
	// pull down
	int height = GetActorZ(victim) - GetActorFloorZ(victim);
	SetActorVelocity(victim, 0, 0, -height / 2, false, false);
	
	// while actor is above ground, wait
	// no change for 2 tics can imply the unit landed somewhere and is no longer moving
	int prev_z;
	bool landed = false;
	do {
		prev_z = GetActorZ(victim);
		Delay(const:2);
		landed = prev_z == GetActorZ(victim) || prev_z - GetActorFloorZ(victim) <= 4.0;
	} while(!landed);
	
	PlaySound(victim, "Gravdis/Drop", 6, 1.0);
	
	if(!was_flying)
		GiveActorInventory(victim, "TakeFlight", 1);
	else {
		GiveActorInventory(victim, "AllowFloat", 1);
		// flying monsters need the line below, they dont move up or down otherwise for some reason
		SetActorVelocity(victim, 0, 0, 1, true, false);
	}
	
	// apply the damage
	int height_factor = (GRAVDIS_HEIGHT_FACTOR * height / GRAVDIS_HEIGHTADD_PER) >> 16;
	SetActivator(dmg_source);

	damage = HandleDamageDeal(dmg_source, victim, damage * (100 + height_factor) / 100, DND_DAMAGETYPE_PHYSICAL, DND_WEAPON_GRAVDIS, DND_DAMAGEFLAG_FOILINVUL | DND_DAMAGEFLAG_NOPUSH, 0, 0, 0, 0);
	if(damage > 0)
		Thing_Damage2(victim, damage, "SkipHandle");

	ACS_NamedExecuteWithResult("DnD Gravdis FX Spawner", victim, 1);
	
	SetResultValue(0);
}

Script "DnD Gravdis FX Spawner" (int id, int which) CLIENTSIDE {
	switch(which) {
		case 0:
			GiveActorInventory(id, "GravdisPullFX_Spawner", 1);
		break;
		case 1:
			GiveActorInventory(id, "GravdisLandFX_Spawner", 1);
		break;
	}
}

Script "DnD Gravdis Hit Fx" (int r) CLIENTSIDE {
	int p = 0;
	int x = GetActorX(0);
	int y = GetActorY(0);
	int z = GetActorZ(0);
	
	for(int i = 0; i < GRAVDIS_CIRCLE_PARTICLES; ++i) {
		p = 1.0 / (GRAVDIS_CIRCLE_PARTICLES - 1) * i;
		SpawnForced("GravdisBorderParticle", x + r * cos(p), y + r * sin(p), z, 0);
	}
}

// This script is used just to stack the elevation portion from multiple sources, but whoever initially triggered the first one will get all credit
Script "DnD Gravdis Elevate" (int victim, int damage, int base_dmg) {
	int m = Clamp_Between(GetActorProperty(victim, APROP_MASS) / (4 * damage / base_dmg), 16, 4096);
	
	GiveActorInventory(victim, "AllowFlight", 1);
	
	for(int i = 0; i < HALF_TICRATE; ++i) {
		SetActorVelocity(victim, 0, 0, GRAVDIS_FLING_FACTOR * cos((0.25 / (HALF_TICRATE - 1)) * i) / m, true, false);
		Delay(const:1);
	}
	
	SetActorVelocity(victim, 0, 0, 0, false, false);
	
	SetResultValue(0);
}

Script "DnD Hammer Return" (void) {
	while(CheckInventory("DnD_ActorWorking"))
		Delay(const:5);
	SetActivatorToTarget(0);
	TakeInventory("HammerFetched", 1);
	TakeInventory("HammerReturnSignal", 1);
	GiveInventory("HammerAmmo", 1);
}

// used in case something goes horribly wrong and the hammer gets to absurd speeds while bouncing and seeking
Script "DnD Hammer Speed Limiter" (int max_vel, int min_vel) {
	bool update_spd = false;
	int new_spd = 0;
	int owner_tid = GetActorProperty(0, APROP_TARGETTID);
	min_vel <<= 16;
	max_vel <<= 16;
	while(CheckInventory("DnD_ActorWorking")) {
		int vx = GetActorVelX(0);
		int vy = GetActorVelY(0);
		int vz = GetACtorVelZ(0);

		int len = VectorLength(vx, vy);
		len = VectorLength(vz, len);
		if(!len)
			len = 1;

		if(len > max_vel) {
			new_spd = max_vel;
			update_spd = true;
		}
		else if(len < min_vel) {
			new_spd = min_vel;
			update_spd = true;
		}

		if(update_spd && CheckInventory("DnD_Boolean")) {
			int tracer_tid = GetActorProperty(0, APROP_TRACERTID);
			if(tracer_tid && tracer_tid != owner_tid) {
				int v_dir = Vec3To(tracer_tid);
				ToUnitVec3(v_dir);

				update_spd = false;
				new_spd >>= 16;
				SetActorVelocity(
					0,
					vec3[v_dir].x * new_spd,
					vec3[v_dir].y * new_spd,
					vec3[v_dir].z * new_spd,
					0,
					0
				);
				GiveInventory("HammerSeekDelay", 1);
				FreeVec3(v_dir);

				// printbold(s:"is missile? ", d:CheckFlag(0, "MISSILE"), s: " ", s:GetActorClass(0), s: " vs ", s:GetActorClass(owner_tid));

				ACS_NamedExecuteWithResult("DnD Hammer Self Cooldown");
			}
		}
		//Log(s:GetActorClass(0), s: " ", d:ActivatorTID());

		//if(GetActorProperty(0, APROP_ACCURACY) != DND_CRIT_TOKEN && GetUserVariable(0, "user_bcount") >= crit_bounces)
		//	SetActorProperty(0, APROP_ACCURACY, DND_CRIT_TOKEN);

		Delay(const:1);
	}
}

Script "DnD Hammer Self Cooldown" (void) {
	GiveInventory("HammerThruActors", 1);
	Delay(const:5);
	// this check is necessary, from my tests, the projectile was so volatile and "engine-breaking" that by the time this ran it may not even be there
	// somehow the scripts and stuff in them assume its a bright idea to let the "owner" aka the player receive this shit...
	// so I had the player turn into a vegetable MISSILE that died and reanimated randomly... yeah...
	if(CheckInventory("DnD_ActorWorking") && GetActorClass(0) != "None")
		GiveInventory("HammerNoThruActors", 1);
	SetResultValue(0);
}

Script "DnD Puff Take Ammo" (int wepid, int amt, int ammo_slot, int secondary) {
	SetActivatorToTarget(0);

	int owner = ActivatorTID();
	str ammo_type = "";

	int ammo_sub_slot = ammo_slot >> 16;
	ammo_slot &= 0xFFFF;

	if(secondary)
		ammo_type = Weapons_Data[wepid].ammo_name2;
	else
		ammo_type = Weapons_Data[wepid].ammo_name1;

	if(ammo_type != "") {
		TakeInventory(ammo_type, amt);

		if(!IsAccessoryEquipped(owner, DND_ACCESSORY_HANDARTEMIS))
			HandleAmmoGainChance(ammo_slot, ammo_sub_slot, amt, owner);
	}

	SetResultValue(0);
}

// this is called by projectiles only
Script "DnD Update Melee ReactionTime" (int base) {
	int owner = GetActorProperty(0, APROP_TARGETTID);
	if(IsPlayer(owner)) {
		//printbold(d:base * (100 + GetMeleeRangeIncrease(owner)) / 100);
		base = GetPlayerMeleeRange(owner - P_TIDSTART, base << 16) >> 16;
		SetActorProperty(0, APROP_REACTIONTIME, base);
	}
	SetResultValue(0);
}

Script "DnD Get Melee Range Increase" (int safety_for_inv) {
	int owner = GetActorProperty(0, APROP_TARGETTID);
	int val = 0;
	if(IsPlayer(owner)) {
		val = GetPlayerMeleeRange(owner- P_TIDSTART, 100.0) >> 16;
		if(safety_for_inv)
			val = Clamp_Between(100 - val, 1, 100);
	}
	SetResultValue(val);
}

Script "DnD Thunder Axe Weaken" (void) {
	while(IsAlive() && CheckInventory("ThunderAxeWeakenTimer")) {
		Delay(const:7);
		TakeInventory("ThunderAxeWeakenTimer", 1);
	}
	SetInventory("ThunderAxeWeakenTimer", 0);
	
	SetResultValue(0);
}

Script "DnD Chain Lightning (Weapon)" (int dmg, int zap_count, int flags, int jump_dist) {
	int i, mn;

	int this = GetActorProperty(0, APROP_TRACERTID);
	int owner = GetActorProperty(0, APROP_TARGETTID);

	if(!this || !owner)
		Terminate;
	
	int pnum = owner - P_TIDSTART;
	int wepid = dmg & ATK_WID_MASK;
	dmg >>= ATK_CACHE_SHIFT;

	i = dmg & ATK_CACHE_MASK;
	dmg >>= ATK_DTYPE_SHIFT;

	int dmg_type = dmg & ATK_DTYPE_MASK;
	dmg >>= ATK_DPCT_SHIFT;

	int factor = dmg & ATK_DPCT_MASK;

	dmg = RetrieveWeaponDamage(pnum, wepid, i, GetDamageCategory(dmg_type, flags), flags, 0);
	int min_dmg = dmg / 10; // 10% of dmg

	if(factor != 100)
		dmg = dmg * factor / 100;

	// range inc
	jump_dist <<= 16;
	if(IsMeleeWeapon(wepid))
		jump_dist = GetPlayerMeleeRange(pnum, jump_dist);

	SetActivatorToTarget(0);

	Delay(const:1);

	static int zap_tids[MAXPLAYERS][DND_MAX_OVERLOADTARGETS];

	while(IsActorAlive(owner) && zap_count) {
		// init
		int cur_count = 0;
		for(i = 0; i < zap_count; ++i)
			zap_tids[pnum][i] = 0;

		for(mn = 0; mn < DnD_TID_Counter[DND_TID_MONSTER]; ++mn) {
			i = UsedMonsterTIDs[mn];
			if(i != this && isActorAlive(i) && CheckFlag(i, "SHOOTABLE") && fdistance(this, i) <= jump_dist && CheckSight(this, i, 0))
				zap_tids[pnum][cur_count++] = i;
		}

		// no candidates left, stop
		if(!cur_count)
			Terminate;

		int victim = zap_tids[pnum][random(0, cur_count - 1)];

		// fx
		ACS_NamedExecuteAlways("DnD Overload Zap FX", 0, this, victim);

		// dmg
		mn = HandleDamageDeal(owner, victim, dmg, dmg_type, wepid, flags, 0, 0, 0, ScanActorFlags());
		if(mn > 0) {
			Thing_Damage2(victim, mn, "SkipHandle");
		}

		PlaySound(victim, "Spell/LightningSpearBounce", CHAN_WEAPON, 1.0);

		// new origin and reduced damage per bounce by 10% (for now, we can make it configurable later)
		this = victim;
		dmg -= dmg / 10;
		if(dmg < min_dmg)
			dmg = min_dmg;
		
		Delay(const:CHAIN_LIGHTNING_DELAY);
		--zap_count;
	}
}

#endif