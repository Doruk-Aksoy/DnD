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
Script "DnD Can Fire Weapon" (void) {
	int pnum = PlayerNumber();

	//int pc = 0;
	while(IsAlive() && !IsSetupComplete(SETUP_STATE1, SETUP_PLAYERDATAFINISHED)) {
		int wepid = GetCurrentWeaponID();
		buffData_T module& pbuffs = GetPlayerBuffData(pnum);

		//pc = (pc + 1) % 15;
		//if(!pc)
		//	printbold(s:"loop running on ", d:wepid);

		int flags = 0;
		bool canFire = false;
		bool canAltFire = false;
		bool canReload = false;
		bool isOverheatingWeapon = false;
		bool hasOverheatCooldown = false;

		int amt1 = Weapons_Data[wepid].ammo_use1;
		str ammo1 = Weapons_Data[wepid].ammo_name1;

		int amt2 = Weapons_Data[wepid].ammo_use2;
		str ammo2 = Weapons_Data[wepid].ammo_name2;

		switch(wepid) {
			case DND_WEAPON_SICKLE:
				canFire = true;
				flags = DND_CFW_ALTFIRECHECK;
				canAltFire = !CheckInventory("SickleCooldown");
			break;

			case DND_WEAPON_AKIMBOPISTOL:
				if(CheckInventory("AkimboFireMode") == 3)
					ammo2 = "AkimboClipRight";
				else
					ammo2 = "AkimboClipLeft";

				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt1, flags);

				amt2 = GetMagazineCap(pnum, DND_MAG_AKIMBOLEFT);

				canReload = (CheckInventory(WeaponMagazineList[DND_MAG_AKIMBOLEFT]) < amt2 || CheckInventory(WeaponMagazineList[DND_MAG_AKIMBORIGHT]) < amt2) &&
							CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
			break;
			case DND_WEAPON_SCATTERGUN:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1 * 3, flags);
				canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
			break;

			case DND_WEAPON_PURIFIER:
				flags |= CheckInventory("DnD_FiringFlags");
			case DND_WEAPON_SHOTGUN:
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
				canFire = canAltFire && (GetPlayerAttributeValue(pnum, INV_EX_CANFIREOVERHEATED) || CheckInventory("ShockerOverheat") <= 80);
				
				canAltFire &= CheckInventory("ShockerOverheat") >= 10;

				isOverheatingWeapon = true;
			break;
			case DND_WEAPON_HADES:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt1, flags);
				canAltFire = canFire && !CheckInventory("HadesBurstFired") && !CheckInventory("HadesBurstCooldown");
				canReload = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
			break;

			// special case where weapons use same ammo to check for reload and fire
			case DND_WEAPON_HEAVYSUPERSHOTGUN:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				canReload = canFire;
			break;

			case DND_WEAPON_DESOLATOR:
				flags |= CheckInventory("DnD_FiringFlags");

				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				hasOverheatCooldown = CheckInventory("DesolatorCooldown");

				canFire &= GetPlayerAttributeValue(pnum, INV_EX_CANFIREOVERHEATED) || (!hasOverheatCooldown && CheckInventory("DesolatorOverheat") < 100);

				if(flags & DND_CFW_HOLDFIREORRELOADCHECK)
					canFire &= GetPlayerInput(-1, INPUT_BUTTONS) & BT_ATTACK;

				isOverheatingWeapon = true;
			break;
			case DND_WEAPON_MINIGUN:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1 * 5, flags);
				canAltFire = canFire && GetPlayerInput(-1, INPUT_BUTTONS) & BT_ATTACK;
			break;
			case DND_WEAPON_MPPB:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				canAltFire = canFire && GetPlayerInput(-1, INPUT_BUTTONS) & BT_ATTACK;
			break;

			case DND_WEAPON_RIOTCANNON:
				flags = DND_CFW_DONTCHECKEQUALITY;
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

			// weapons that have ammo check requirement on ammo2 to fire m1, and ammo1 is morely internal either cosmetic to weapon mechanics or something else
			case DND_WEAPON_HAMMER:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt2, flags);
				canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt2, flags);
			break;

			// weapons that require only ammo1 to fire both primary and alt
			case DND_WEAPON_SNIPER:
			case DND_WEAPON_THUNDERSTAFF:
			case DND_WEAPON_RAILGUN:
			case DND_WEAPON_DARKGLOVES:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt2, flags);
			break;

			// standard weapons with no special alt fire that use the ammos, which can reload with mags
			case DND_WEAPON_MAGNUM:
				// magnum case is different					
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt2, flags);
				canReload = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
			break;
			case DND_WEAPON_ASSAULTRIFLE:
			case DND_WEAPON_KILLSTORM:
			case DND_WEAPON_PLASMACANNON:
			case DND_WEAPON_MACHINEGUN:
			case DND_WEAPON_HEAVYMACHINEGUN:
			case DND_WEAPON_LEADSPITTER:
			case DND_WEAPON_TEMPLARMG:
			case DND_WEAPON_VINDICATOR:
			case DND_WEAPON_RHINORIFLE:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt1, flags);
				canReload = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
			break;

			// weapons with different cost altfires, that have clips and use the main ammo as means to reload
			case DND_WEAPON_ERASUS:
			case DND_WEAPON_ACIDRIFLE:
			case DND_WEAPON_FUSIONBLASTER:
			case DND_WEAPON_FLAMETHROWER:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt1, flags);
				canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt2, flags);
				canReload = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
			break;

			case DND_WEAPON_HEAVYGL:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1 + 1, flags);
			break;
			case DND_WEAPON_FREEZER:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				hasOverheatCooldown = CheckInventory("FreezerCooldown");
				
				canAltFire = GetPlayerAttributeValue(pnum, INV_EX_CANFIREOVERHEATED) || (!hasOverheatCooldown && CheckInventory("FreezerOverheat") < 100);
				canFire &= canAltFire;
				
				canAltFire &= CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1 / 2, flags);

				isOverheatingWeapon = true;
			break;
			case DND_WEAPON_VOIDCANNON:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				hasOverheatCooldown = CheckInventory("VoidCannonCooldown");

				canFire &= GetPlayerAttributeValue(pnum, INV_EX_CANFIREOVERHEATED) || (!hasOverheatCooldown && CheckInventory("VoidCannonOverHeat") < 81);

				isOverheatingWeapon = true;
			break;

			case DND_WEAPON_NUCLEARPLASMARIFLE:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				hasOverheatCooldown = CheckInventory("PlasmaOverheatCooldown");

				canFire &= GetPlayerAttributeValue(pnum, INV_EX_CANFIREOVERHEATED) || (!hasOverheatCooldown && CheckInventory("PlasmaOverheat") < 100);

				isOverheatingWeapon = true;
			break;
			case DND_WEAPON_FROSTFANG:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1 * 5, flags);
				canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt2, flags);
			break;
			case DND_WEAPON_LIGHTNINGGUN:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1 * 3, flags);
			break;
			case DND_WEAPON_REBOUNDER:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				hasOverheatCooldown = CheckInventory("RebounderCooldown");
				isOverheatingWeapon = GetPlayerAttributeValue(pnum, INV_EX_CANFIREOVERHEATED);

				canFire &= isOverheatingWeapon || (!hasOverheatCooldown && CheckInventory("RebounderOverheat") < 98);
				canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt2, flags);
				canAltFire &= isOverheatingWeapon || (!hasOverheatCooldown && CheckInventory("RebounderOverheat") < 83);

				isOverheatingWeapon = true;
			break;
			case DND_WEAPON_BASILISK:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags) || CheckInventory("LoadedBasilisk");
				canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt2, flags);
			break;

			case DND_WEAPON_BFG32768:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				hasOverheatCooldown = CheckInventory("BFG32768Cooldown");

				canFire &= GetPlayerAttributeValue(pnum, INV_EX_CANFIREOVERHEATED) || (!hasOverheatCooldown && CheckInventory("BFG32768Overheat") < 100);

				isOverheatingWeapon = true;
			break;
			case DND_WEAPON_DEATHRAY:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				hasOverheatCooldown = CheckInventory("DeathRayCooldown");

				canFire &= GetPlayerAttributeValue(pnum, INV_EX_CANFIREOVERHEATED) || (!hasOverheatCooldown && CheckInventory("DeathRayOverheat") < 100);

				isOverheatingWeapon = true;
			break;
			case DND_WEAPON_IONCANNON:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
				hasOverheatCooldown = CheckInventory("IonCooldown");

				canFire &= GetPlayerAttributeValue(pnum, INV_EX_CANFIREOVERHEATED) || (!hasOverheatCooldown && CheckInventory("IonOverheat") < 100);
				canAltFire = canFire && GetPlayerInput(-1, INPUT_BUTTONS) & BT_ATTACK;

				isOverheatingWeapon = true;
			break;

			case DND_WEAPON_SAWEDOFF:
				canFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt1, flags);
				canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt2, flags);
				canReload = CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);
			break;

			// melee weapons with no ammo requirements
			case DND_WEAPON_FIST:
			case DND_WEAPON_KATANA:
			case DND_WEAPON_CHAINSAW:
			case DND_WEAPON_DOUBLECHAINSAW:
				canFire = true;
				flags = DND_CFW_ALTFIRECHECK;
			break;
			case DND_WEAPON_EXCALIBAT:
				canFire = true;
				flags = DND_CFW_ALTFIRECHECK;
				canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt2, flags);
			break;
			case DND_WEAPON_DUSKBLADE:
			case DND_WEAPON_INFERNOSWORD:
			case DND_WEAPON_AXE:
				flags = DND_CFW_ALTFIRECHECK;
			default:
				canFire = ammo1 == "" || CanTakeAmmoFromPlayer(pnum, wepid, ammo1, amt1, flags);

				if(ammo2 == "")
					ammo2 = ammo1;
				canAltFire = CanTakeAmmoFromPlayer(pnum, wepid, ammo2, amt2, flags);
			break;
		}

		if(CheckInventory("ArtemisCheck") && (IsSlot8Weapon(wepid) || wepid == DND_WEAPON_DEMONHEART)) {
			canFire = false;
			canAltFire = false;
			canReload = false;
		}

		if(!isOverheatingWeapon && GetPlayerAttributeValue(pnum, INV_EX_CANTFIRENONOVERHEAT) && HasRunningOverheatCooldown(pnum + P_TIDSTART)) {
			// can't fire weapons that dont overheat if there's overheat cooldown active if the HeatBreaker charm is equipped
			canFire = false;
			canAltFire = false;
		}

		if((flags & DND_CFW_ALTFIRECHECK) && IsMeleeWeapon(wepid))
			canAltFire &= CheckInventory("Ability_Kick");

		if(pbuffs.buff_net_values[BUFF_STUN].multiplicative != 1.0) {
			canFire = false;
			canAltFire = false;
			canReload = false;
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

	SetResultValue(0);
}

void DoWeaponDamageCache(int pnum, int dmg_id, int dmg, int dmg_rand, int wepid) {
	// we recalc accuracy in case a particular weapon has bonuses to it
	bool firingLoopRun = false;
	if(!CheckInventory("DnD_WeaponSwapCheck")) {
		GiveInventory("DnD_WeaponSwapCheck", 1);
		firingLoopRun = true;
		
		// weapon swap related things can go here
		// we have access to the old/previous weapon at this point until below SetInventory
		if(GetPlayerAttributeValue(pnum, INV_EX_SWAPFROMMELEECRIT) && IsMeleeWeapon(CheckInventory("DnD_WeaponID")))
			GiveInventory("DnD_SwappedFromMelee", 1);

		// we are now committed to this weapon
		SetInventory("DnD_WeaponID", wepid);
		
		CalculatePlayerAccuracy(pnum, wepid);
	}

	//printbold(s:"cached id and wep ", d:dmg_id, s:" ", d: wepid);
	CachePlayerDamage(pnum, dmg, wepid, dmg_id, dmg_rand);
}

Script "DnD Weapon Damage Cache" (int wepid) {
	int pnum = PlayerNumber();

	if(PlayerIsLoggedIn(pnum) && IsSetupComplete(SETUP_STATE1, SETUP_PLAYERDATAFINISHED))
		Terminate;

	switch(wepid) {
		case DND_WEAPON_FIST:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 10, 1 | (3 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 15, 12 | (16 << 16), wepid);
		break;
		case DND_WEAPON_CHAINSAW:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 4, 5 | (10 << 16), wepid);
		break;
		case DND_WEAPON_DOUBLECHAINSAW:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 4, 5 | (10 << 16), wepid);
		break;
		case DND_WEAPON_KATANA:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 20, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 12, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 125, 0, wepid);
		break;
		case DND_WEAPON_EXCALIBAT:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 5, 4 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 25, 4 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 128, 0, wepid);
		break;
		case DND_WEAPON_INFERNOSWORD:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 60, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 48, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 20, 2 | (4 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_3, 8, 8 | (10 << 16), wepid);
		break;
		case DND_WEAPON_DUSKBLADE:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 25, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 96, 0, wepid);
		break;
		case DND_WEAPON_SICKLE:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 5, 8 | (12 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 5, 15 | (18 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 80, 0, wepid);
		break;
		
		case DND_WEAPON_PISTOL:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 15, 0, wepid);
		break;
		case DND_WEAPON_AKIMBOPISTOL:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 30, 0, wepid);
		break;
		case DND_WEAPON_MAGNUM:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 25, 4 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 50, 0, wepid);
		break;
		case DND_WEAPON_LASERPISTOL:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 5, 3 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 10, 3 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 15, 3 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_3, 20, 3 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_4, 25, 3 | (6 << 16), wepid);
		break;
		case DND_WEAPON_RUBYWAND:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 5, 3 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 25, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 8, 4 | (8 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_3, 24, 0, wepid);
		break;
		case DND_WEAPON_ASSAULTRIFLE:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 30, 0, wepid);
		break;
		case DND_WEAPON_VIPERSTAFF:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 40, 3 | (5 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 10, 15 | (25 << 16), wepid);
		break;
		case DND_WEAPON_SCATTERGUN:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 5, 2 | (4 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 3, 2 | (4 << 16), wepid);
		break;
		
		case DND_WEAPON_SHOTGUN:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 5, 2 | (3 << 16), wepid);
		break;
		case DND_WEAPON_PURIFIER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 15, 0, wepid);
		break;
		case DND_WEAPON_KILLSTORM:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 18, 0, wepid);
		break;
		case DND_WEAPON_EMERALDWAND:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 15, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 150, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 8, 1 | (4 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_3, 50, 0, wepid);
		break;
		case DND_WEAPON_DEADLOCK:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 15, 0, wepid);
		break;
		case DND_WEAPON_NITROGENCROSSBOW:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 210, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 75, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 5, 0, wepid);
		break;
		case DND_WEAPON_WHEELOFTORMENT:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 160, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 192, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 10, 1 | (2 << 16), wepid);
		break;
		case DND_WEAPON_CHARONBLASTER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 6, 10 | (12 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 2, 5 | (8 << 16), wepid);
		break;
		
		case DND_WEAPON_SUPERSHOTGUN:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 5, 2 | (3 << 16), wepid);
		break;
		case DND_WEAPON_HEAVYSUPERSHOTGUN:
		case DND_WEAPON_ERASUS:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 15, 0, wepid);
		break;
		case DND_WEAPON_HELLSMAW:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 75, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 45, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 30, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_3, 25, 0, wepid);
		break;
		case DND_WEAPON_AXE:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 25, 6 | (9 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 125, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 100, 0, wepid);
		break;
		case DND_WEAPON_PLASMACANNON:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 40, 0, wepid);
		break;
		case DND_WEAPON_SHOCKER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 18, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 10, 4 | (5 << 16), wepid);
		break;
		case DND_WEAPON_HADES:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 1, 2 | (3 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 10, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 18, 0, wepid);
		break;
		case DND_WEAPON_CROSSBOW:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 400, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 200, 0, wepid);
		break;
		
		case DND_WEAPON_SILVERGUN:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 15, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 8, 4 | (6 << 16), wepid);
		break;
		case DND_WEAPON_SLAYER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 10, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 50, 0, wepid);
		break;
		
		case DND_WEAPON_MACHINEGUN:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 5, 3 | (4 << 16), wepid);
		break;
		case DND_WEAPON_HEAVYMACHINEGUN:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 25, 0, wepid);
		break;
		case DND_WEAPON_LEADSPITTER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 16, 0, wepid);
		break;
		case DND_WEAPON_DEMONSEALER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 18, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 150, 0, wepid);
		break;
		case DND_WEAPON_TEMPLARMG:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 20, 0, wepid);
			// compat with gl
			DoWeaponDamageCache(pnum, DND_DMGID_1, 128, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 80, 0, wepid);
		break;
		case DND_WEAPON_RIOTCANNON:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 12, 0, wepid);
		break;
		case DND_WEAPON_ACIDRIFLE:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 18, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 5, 2 | (3 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 10, 4 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_3, 96, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_4, 3, 1 | (2 << 16), wepid);
		break;
		case DND_WEAPON_FUSIONBLASTER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 30, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 200, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 6, 0, wepid);
		break;
		case DND_WEAPON_INCINERATOR:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 4, 5 | (7 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 10, 12 | (16 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 128, 0, wepid);
		break;
		
		case DND_WEAPON_DESOLATOR:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 1, 25 | (30 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 10, 15 | (20 << 16), wepid);
		break;
		case DND_WEAPON_MINIGUN:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 4, 4 | (7 << 16), wepid);
		break;
		case DND_WEAPON_EBONYCANNON:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 96, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 8, 4 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 1, 4 | (8 << 16), wepid);
		break;
		case DND_WEAPON_MPPB:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 7, 6 | (8 << 16), wepid);
		break;
		
		case DND_WEAPON_ROCKETLAUNCHER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 45, 2 | (4 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 128, 0, wepid);
		break;
		case DND_WEAPON_TORPEDOLAUNCHER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 20, 15 | (20 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 224, 0, wepid);
		break;
		case DND_WEAPON_GRENADELAUNCHER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 128, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 80, 0, wepid);
		break;
		case DND_WEAPON_VINDICATOR:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 5, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 12, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 16, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_3, 20, 0, wepid);
		break;
		case DND_WEAPON_HAMMER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 25, 3 | (5 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 96, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 180, 0, wepid);
		break;
		case DND_WEAPON_METEORLAUNCHER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 200, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 192, 0, wepid);
		break;
		case DND_WEAPON_HEAVYGL:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 128, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 6, 1 | (3 << 16), wepid);
		break;
		case DND_WEAPON_FREEZER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 150, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 3, 1 | (3 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 36, 0, wepid);
		break;
		case DND_WEAPON_GRAVDIS:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 400, 0, wepid);
		break;
		case DND_WEAPON_VOIDCANNON:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 100, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 300, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 30, 0, wepid);
		break;
		case DND_WEAPON_MERCURYLAUNCHER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 16, 16 | (20 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 24, 0, wepid);
		break;
		case DND_WEAPON_ROTARYGRENADELAUNCHER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 192, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 384, 0, wepid);
		break;
		case DND_WEAPON_HEAVYMISSILELAUNCHER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 192, 0, wepid);
		break;
		case DND_WEAPON_SEDRINSTAFF:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 5, 15 | (20 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 96, 0, wepid);
		break;
		
		case DND_WEAPON_PLASMARIFLE:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 5, 4 | (8 << 16), wepid);
		break;
		case DND_WEAPON_NUCLEARPLASMARIFLE:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 6, 6 | (10 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 10, 1 | (3 << 16), wepid);
		break;
		case DND_WEAPON_TURELCANNON:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 80, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 135, 0, wepid);
		break;
		case DND_WEAPON_FROSTFANG:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 5, 3 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 3, 2 | (3 << 16), wepid);
		break;
		case DND_WEAPON_SNIPER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 10, 35 | (40 << 16), wepid);
		break;
		case DND_WEAPON_FLAMETHROWER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 1, 1 | (4 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 2, 0, wepid);
		break;
		case DND_WEAPON_LIGHTNINGGUN:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 1, 1 | (10 << 16), wepid);
		break;
		case DND_WEAPON_REBOUNDER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 35, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 150, 0, wepid);
		break;
		case DND_WEAPON_DARKLANCE:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 25, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 50, 0, wepid);
		break;
		case DND_WEAPON_RHINORIFLE:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 75, 0, wepid);
		break;
		case DND_WEAPON_NAILGUN:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 1, 13 | (21 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 20, 0, wepid);
		break;
		case DND_WEAPON_BASILISK:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 6, 3 | (8 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 12, 6 | (12 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 96, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_3, 32, 0, wepid);
		break;
		
		case DND_WEAPON_BFG6000:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 100, 1 | (8 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 2000, 0, wepid);
		break;
		case DND_WEAPON_BFG32768:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 150, 4 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 2500, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 224, 0, wepid);
		break;
		case DND_WEAPON_DEVASTATOR:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 6, 8 | (10 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 48, 0, wepid);
		break;
		case DND_WEAPON_MFG:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 425, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 200, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 32, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_3, 128, 0, wepid);
		break;
		case DND_WEAPON_IONCANNON:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 160, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 144, 0, wepid);
		break;
		case DND_WEAPON_THUNDERSTAFF:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 50, 5 | (10 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 1, 20 | (80 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 250, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_3, 50, 5 | (15 << 16), wepid);
		break;
		case DND_WEAPON_GAUSSRIFLE:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 200, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 96, 0, wepid);
		break;
		case DND_WEAPON_RAILGUN:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 80, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 160, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 240, 0, wepid);
		break;
		case DND_WEAPON_DEATHRAY:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 450, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 150, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 550, 0, wepid);
		break;
		
		case DND_WEAPON_DEATHSTAFF:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 128, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 200, 2 | (3 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 8, 16 | (24 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_3, 1, 6 | (12 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_4, 1, 0, wepid);
		break;
		case DND_WEAPON_RAZORFANG:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 12, 4 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 2, 6 | (9 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 1, 16 | (20 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_3, 64, 0, wepid);
		break;
		case DND_WEAPON_SUNSTAFF:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 50, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 48, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 36, 0, wepid);
		break;
		case DND_WEAPON_SOULREAVER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 6, 4 | (6 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 128, 0, wepid);
		break;
		
		case DND_WEAPON_SAWEDOFF:
		case DND_WEAPON_SMG:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 15, 0, wepid);
		break;
		case DND_WEAPON_SOULRENDER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 40, 3 | (4 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 40, 6 | (8 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 20, 1 | (8 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_3, 75, 0, wepid);
		break;
		case DND_WEAPON_HELLFORGECANNON:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 10, 1 | (8 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 64, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 1, 3 | (6 << 16), wepid);
		break;
		case DND_WEAPON_BLOODFIENDSPINE:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 1, 16 | (32 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 36, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 5, 1 | (3 << 16), wepid);
		break;
		case DND_WEAPON_ENFORCERRIFLE:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 5, 4 | (9 << 16), wepid);
		break;
		case DND_WEAPON_VENOM:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 1, 3 | (5 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 5, 1 | (3 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 6, 0, wepid);
		break;
		case DND_WEAPON_DEMONHEART:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 32, 28 | (36 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 48, 0, wepid);
		break;
		case DND_WEAPON_DARKGLOVES:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 10, 6 | (9 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 48, 0, wepid);
		break;
		case DND_WEAPON_HEAVYNAILGUN:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 4, 4 | (8 << 16), wepid);
		break;
		case DND_WEAPON_BERETTAS:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 10, 0, wepid);
		break;
		case DND_WEAPON_PLASMABOLTER:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 8, 3 | (5 << 16), wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 35, 0, wepid);
		break;
		case DND_WEAPON_RIPPERCANNON:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 25, 0, wepid);
		break;

		// special
		case DND_WEAPON_ASCENSION:
			DoWeaponDamageCache(pnum, DND_DMGID_0, 20, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_1, 300, 0, wepid);
			DoWeaponDamageCache(pnum, DND_DMGID_2, 200, 0, wepid);
		break;
	}

	SetResultValue(0);
}

Script "DnD Weapon Select" (int wepid) {
	SetInventory("DnD_WeaponID", wepid);

	int pnum = PlayerNumber();
	if(IsHandgun(wepid) && GetPlayerAttributeValue(pnum, INV_IMP_HANDGUNBONUS)) {
		GiveInventory("DnD_HandgunMoreCritShots", 1);
	}
	else
		TakeInventory("DnD_HandgunMoreCritShots", 0);

	SetResultValue(0);
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
			toGive = "ShellSize_9";
			base = GetAmmoCapacity("ShellSize_9");
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
	int base_rate = rate;
	int pnum = PlayerNumber();

	// prevent multiple copies, also dont keep this busy if we got no overheat bonus
	if(!IsSet(PlayerRunsOverheat[pnum], index)) {
		PlayerRunsOverheat[pnum] = SetBit(PlayerRunsOverheat[pnum], index);
		while(CheckInventory(WeaponOverheatItems[index][WEAPON_OVERHEATID])) {
			// prevent snapshotting
			int temp = GetPlayerAttributeValue(pnum, INV_IMP_FASTEROVERHEATDISS);
			int d = base_rate >> 16;
			rate = base_rate & 0x0000FFFF;

			if(temp) {
				temp = d * (100 - temp) / 100;
				if(!temp) {
					rate = rate * (100 + temp) / 100;
					// if not 1 then we also reduce the delay if the difference is that small
					if(d != 1)
						d = 1;
				}
				else
					d = temp;
			}

			if(CheckInventory("DnD_OverheatCanReduce"))
				TakeInventory(WeaponOverheatItems[index][WEAPON_OVERHEATID], rate);
			Delay(d);
		}
		PlayerRunsOverheat[pnum] = ClearBit(PlayerRunsOverheat[pnum], index);
	}
}

Script "DnD Sickle Cooldown" (void) {
	if(!CheckInventory("SickleCooldown")) {
		SetInventory("SickleCooldown", 100);

		while(isAlive() && CheckInventory("SickleCooldown")) {
			Delay(const:7);
			TakeInventory("SickleCooldown", 2);
		}

		SetInventory("SickleCooldown", 0);
	}
}

Script "DnD Sickle Check Res" (int base) {
	int res = 0;
	int hit = GetActorProperty(0, APROP_TRACERTID);

	if(IsMonster(hit) && !IsActorAlive(hit)) {
		SetActivatorToTarget(0);
		base = (base << 16) / 100;
		res = RunLuckBasedChance(PlayerNumber(), base, DND_LUCK_OUTCOME_GAIN);
		TakeInventory("SickleCooldown", 20);
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
		calc_dmg = GetGenericDoTDamage(pnum, calc_dmg, victim, DND_WEAPON_DARKLANCE);
			
		PlaySound(victim, "Darklance/Affliction", CHAN_ITEM, 1.0);
		
		bool terminated = false;
		int i;
		int orig_height = GetActorProperty(victim, APROP_HEIGHT);
		for(i = 0; !terminated && i < time_lim; ++i) {
			// flat dot damage added after stacks are multiplied, because otherwise it's too overpowered...
			int tmp_dmg;
			if(isActorAlive(victim) && (tmp_dmg = HandleDamageDeal(owner, victim, calc_dmg, DND_DAMAGETYPE_OCCULT, DND_WEAPON_DARKLANCE, DND_DAMAGEFLAG_ISDAMAGEOVERTIME | DND_DAMAGEFLAG_NOPUSH, -1, -1, -1, DND_ACTORFLAG_FOILINVUL)) > 0)
				Thing_Damage2(victim, tmp_dmg, "SkipHandle");
			else
				terminated = true;
			
			Delay(const:DARKLANCE_TICKS);
		}

		terminated = !isActorAlive(victim);
		
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

					time_lim = GetActorZ(prev_victim);
					SetActorPosition(prev_victim, GetActorX(prev_victim), GetActorY(prev_victim), GetActorZ(prev_victim) + orig_height - 24.0, false);
					if(dist < DARKLANCE_BOUNCE_DIST && dist < curr_dist && CheckSight(prev_victim, i, CSF_NOBLOCKALL)) {
						victim = i;
						curr_dist = dist;
					}
					SetActorPosition(prev_victim, GetActorX(prev_victim), GetActorY(prev_victim), time_lim, false);
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
	int newtid = tid + DND_THUNDER_RING_TIDSTART + 32 * ((tid + 1) % MAXPLAYERS);
	
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
	SetResultValue(0);
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
	SetActorVelocity(TEMPORARY_ATTACK_TID, FixedMul(cos(angle), cos(pitch)) * 640, FixedMul(sin(angle), cos(pitch)) * 640, sin(-pitch) * 640, 0, 0);
	
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
			HandleAmmoGainChance(ammo_slot, ammo_sub_slot, amt);
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
		if(!victim) {
			// try to pick one more time, and if it still yields 0, ignore this instance
			victim = zap_tids[pnum][random(0, cur_count - 1)];
			// if victim is somehow 0, terminate
			if(!victim)
				Terminate;
		}

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

Script "DnD Set Projectile RipCount" (int amt, int makeOneTimeRipper) {
	int owner = !isPet ? GetActorProperty(0, APROP_TARGETTID) : ACS_NamedExecuteWithResult("DnD Get Master of Target");
	if(!IsPlayer(owner)) {
		SetResultValue(0);
		Terminate;
	}

	int pnum = owner - P_TIDSTART;

	if(!GetPlayerAttributeValue(pnum, INV_EX_RIPPERSRIPALL))
		amt = amt * (100 + GetPlayerAttributeValue(pnum, INV_RIPCOUNT)) / 100;
	else
		amt = MAX_RIPCOUNT;

	if(GetPlayerAttributeValue(pnum, INV_EX_RIPPERSONETIMEONLY) || makeOneTimeRipper) {
		SetActorProperty(0, APROP_STAMINA, GetActorProperty(0, APROP_STAMINA) | DND_DAMAGEFLAG_RIPSONCE);

		// this trick was necessary for nitro shell to make it go through then damage properly...
		if(makeOneTimeRipper)
			GiveInventory("ReplaceThruActorsWithRipper", 1);
	}

	SetInventory("DnD_RipLimit", amt);
	SetResultValue(0);
}

#define LOCKON_LINE_LEN 54.0
#define LOCKON_HUD_X 1024
#define LOCKON_HUD_Y 768
#define LOCKON_HUD_XHALF (LOCKON_HUD_X / 2)
#define LOCKON_HUD_YHALF (LOCKON_HUD_Y / 2)

#define LOCKON_HUD_XHALF_F ((LOCKON_HUD_XHALF) << 16)
#define LOCKON_HUD_YHALF_F ((LOCKON_HUD_YHALF) << 16)

#define LOCKON_HUD_XF 1024.0
#define LOCKON_HUD_YF 768.0

enum {
	LOCKON_ID_DOWN = 2750,
	LOCKON_ID_UP,
	LOCKON_ID_MARKER,
};

int HandleTracerPicking(int owner, int lock_on_range, int width, int height, bool check_for_sight = true, bool draw_gfx = false, bool no_scale_wh = false) {
	// we will map the range of fcs to angles, and use the angletoface as a measure of determining if the target would fall into our fov
	// pitch comes to play in determining height
	// picks best candidate that is closest to us
	//printbold(s:"target pick");
	
	int result = 0;
	int cur_dist = INT_MAX;

	int x, y;
	int draw_x, draw_y;

	int vang;
	int angle_to_face, pitch;
	int dist;
	
	//we want to divide by 2 here
	width >>= 1;
	height >>= 1;

	if(!no_scale_wh) {
		vang = owner - P_TIDSTART;
		width = FixedMul(width, 1.0 + (GetPlayerAttributeValue(vang, INV_LOCKONAREA) << 16) / 100);
		height = FixedMul(height, 1.0 + (GetPlayerAttributeValue(vang, INV_LOCKONAREA) << 16) / 100);
		lock_on_range = FixedMul(lock_on_range, 1.0 + (GetPlayerAttributeValue(vang, INV_LOCKONRANGE) << 16) / 100);
	}

	//Log(s:"width height range ", f:width, s: " ", f:height, s: " ", f:lock_on_range);

	for(int i = 0; i < DnD_TID_Counter[DND_TID_MONSTER]; ++i) {
		int tid = UsedMonsterTIDs[i];
		if(!isActorAlive(tid))
			continue;
		
		// if we are in angle threshold of view
		x = GetActorX(tid) - GetActorX(owner);
		y = GetActorY(tid) - GetActorY(owner);
		
		vang = VectorAngle(x, y);
		angle_to_face = normalize360(vang - GetActorAngle(owner));
		
		dist = 1;
		// approx distance
		if(((vang + 0.125) % 0.5) > 0.25)
			dist = FixedDiv(y, sin(vang));
		else
			dist = FixedDiv(x, cos(vang));
		pitch = GetActorPitch(owner);
		if
		(
			(angle_to_face < 0.2 || angle_to_face > 0.8) && 
			dist < lock_on_range && dist < cur_dist &&
			pitch >= -0.5 && pitch <= 0.5
		)
		{
			// get the angle we make to face the actor and correct it to pixel ratio of doom (1.2)
			pitch = VectorAngle(dist, GetActorZ(tid) + GetActorProperty(tid, APROP_HEIGHT) / 2 - (GetActorZ(owner) + GetActorViewHeight(owner)));
			pitch = normalize360(pitch + GetActorPitch(owner));
	
			x = LOCKON_HUD_XHALF - (LOCKON_HUD_XHALF * FixedDiv(sin(angle_to_face), cos(angle_to_face))) >> 16;
			y = LOCKON_HUD_YHALF - (LOCKON_HUD_XHALF * FixedDiv(sin(pitch), cos(pitch))) >> 16;
			
			if(x <= width && x >= -width && y <= height && y >= -height && (!check_for_sight || CheckSight(owner, tid, 0))) {
				result = tid;
				cur_dist = dist;

				draw_x = x;
				draw_y = y;
			}
		}
	}

	if(result && draw_gfx) {
		SetHudSize(LOCKON_HUD_X, LOCKON_HUD_Y, 1);
		SetFont("HOMMARK");
		HudMessage(s:"A"; HUDMSG_PLAIN, LOCKON_ID_MARKER, CR_UNTRANSLATED, LOCKON_HUD_XHALF_F + (draw_x << 16), LOCKON_HUD_YHALF_F + (draw_y << 16), 0.5);
	}

	return result;
}

Script "DnD Homing Lock-on Draw" (int width, int height, int range) CLIENTSIDE {
	int pnum = PlayerNumber();
	if(ConsolePlayerNumber() != pnum)
		Terminate;

	width = width * (100 + GetPlayerAttributeValue(pnum, INV_LOCKONAREA)) / 100;
	height = height * (100 + GetPlayerAttributeValue(pnum, INV_LOCKONAREA)) / 100;
	range = range * (100 + GetPlayerAttributeValue(pnum, INV_LOCKONRANGE)) / 100;

	range <<= 16;

	int this = ActivatorTID();

	int r = 1.0 * LOCKON_HUD_Y / LOCKON_HUD_X;
	while(isAlive() && !CheckInventory("DnD_RemoveLockOn")) {
		int w = Clamp_Between(((width / 2) * r) & 0xFFFF0000, 0, (GetHudRight(getcvar("vid_defwidth") - (LOCKON_LINE_LEN >> 16)) << 16) / 2);
		int h = Clamp_Between(((height / 2) * r) & 0xFFFF0000, 0, LOCKON_HUD_YF / 2 - (FixedMul(LOCKON_LINE_LEN, r) & 0xFFFF0000));
	
		SetHudSize(LOCKON_HUD_X, LOCKON_HUD_Y, 1);
		// left corner
		SetFont("100pxD");
		HudMessage(
			s:"A";
			HUDMSG_PLAIN,
			LOCKON_ID_DOWN,
			CR_UNTRANSLATED,
			LOCKON_HUD_XF / 2 - w,
			LOCKON_HUD_YF / 2 + h,
			0
		);
		
		// right corner
		SetFont("100pxU");
		HudMessage(
			s:"A";
			HUDMSG_PLAIN,
			LOCKON_ID_UP,
			CR_UNTRANSLATED,
			LOCKON_HUD_XF / 2 + w,
			LOCKON_HUD_YF / 2 - h,
			0
		);

		// run this on server-side instead periodically and check an inventory item
		if(!HandleTracerPicking(this, range, width, height, true, true, true))
			HudMessage(s:""; HUDMSG_PLAIN, LOCKON_ID_MARKER, 0, 0, 0, 0.01);

		Delay(const:6);
	}
	HudMessage(s:""; HUDMSG_PLAIN, LOCKON_ID_DOWN, 0, 0, 0, 0.01);
	HudMessage(s:""; HUDMSG_PLAIN, LOCKON_ID_UP, 0, 0, 0, 0.01);
}

Script "DnD Clear Homing Lock-on" (void) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;
	HudMessage(s:""; HUDMSG_PLAIN, LOCKON_ID_DOWN, 0, 0, 0, 0.01);
	HudMessage(s:""; HUDMSG_PLAIN, LOCKON_ID_UP, 0, 0, 0, 0.01);
	HudMessage(s:""; HUDMSG_PLAIN, LOCKON_ID_MARKER, 0, 0, 0, 0.01);
}

Script "DnD Check Weapon Discard" (int slot, int useTarget) {
	if(useTarget)
		SetActivatorToTarget(0);
	if(slot < 8)
		SetResultValue(HasPlayerDiscardedSlot(PlayerNumber(), slot));
	else {
		useTarget = GetPlayerPreferredTempWeapon(PlayerNumber());
		slot -= FIRST_SLOT9_WEAPON;
		SetResultValue(useTarget != -2 && useTarget != slot);
	}
}

Script "DnD Swapped From Melee" (int ptid) {
	Delay(const:HALF_TICRATE);
	GiveActorInventory(ptid, "DnD_SwappedFromMelee", 1);
}

Script "DnD Ray of Disintegration Trails" (void) CLIENTSIDE {
	int v_dir = GetDirectionVector(0);
	int trail_amt = DND_WANDERER_RAY_BASEDIST / DND_WANDERER_RAY_DIST_PER;

	int viewZ = GetActorViewHeight(0) - 8.0;
	int ox = GetActorX(0);
	int oy = GetActorY(0);
	int oz = GetActorZ(0);

	for(int i = 0; i < trail_amt; ++i) {
		// just spawn these along a line
		int mx = ox + vec3[v_dir].x * ((i + 1) * DND_WANDERER_RAY_DIST_PER + DND_WANDERER_RAY_DIST_PER / 2);
		int my = oy + vec3[v_dir].y * ((i + 1) * DND_WANDERER_RAY_DIST_PER + DND_WANDERER_RAY_DIST_PER / 2);
		int mz = oz + viewZ + vec3[v_dir].z * ((i + 1) * DND_WANDERER_RAY_DIST_PER + DND_WANDERER_RAY_DIST_PER / 2);

		// if at any point it fails to spawn, good chance we hit a wall, ignore rest
		// this is lazy and idc, gzdoom solution is way better anyway
		if(!Spawn("WandererRayTrail", mx, my, mz, 0, 0))
			break;
	}

	SetResultValue(0);
}

#endif