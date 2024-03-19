#ifndef DND_WEAPONS_IN
#define DND_WEAPONS_IN

#include "DnD_WeaponDefs.h"
#include "DnD_Attack.h"
#include "DnD_Projectiles.h"

void HandleAmmoGainChance(int slot, int ammo, int amount, int owner = 0) {
	// if hand of artemis is equipped don't run this
	if(random(1, 100) <= GetPlayerAttributeValue(PlayerNumber(), INV_AMMOGAIN_CHANCE)) {
		if(slot == DND_AMMOSLOT_MAGAZINE)
			GiveInventory(ClipAmmoTypes[ammo], amount);
		else if(slot == DND_AMMOSLOT_SPECIAL) {
			// subtract one if we are above slug, we have grenade for an exception
			if(ammo > SSAM_SLUG)
				--ammo;
			GiveInventory(SpecialAmmoInfo_Str[ammo][AMMOINFO_NAME], amount);
		}
		else if(slot == DND_AMMOSLOT_TEMPORARY) {
			GiveInventory(TemporaryWeaponData[ammo][TEMPWEP_AMMO], amount);
			if(ammo == DND_TEMPWEP_SOULRENDER)
				GiveInventory("BladeCharge", 100);
		}
		else
			GiveInventory(AmmoInfo[slot][ammo].name, amount);
	}
}

// we get weapon id, primary or alt and flags only
// ammo_slot >> 16 contains subslot, first 16 contains main slot
Script "DnD Fire Weapon" (int wepid, int isAltfire, int ammo_slot, int flags) {
	int owner = ActivatorTID();
	int pnum = PlayerNumber();

	// hobo perk 50 lets you bypass ammo consumption on shotguns
	if(IsAccessoryEquipped(owner, DND_ACCESSORY_HANDARTEMIS) || (IsBoomstick(wepid) && CheckInventory("Hobo_ShotgunFrenzyTimer")))
		flags |= DND_ATF_NOAMMOTAKE;
	
	int ammo_sub_slot = ammo_slot >> 16;
	ammo_slot &= 0xFFFF;
	
	// Handles ammo gain checks, the chances etc.
	int ammo_take_amt;
	str ammo_type = "";
	str ammo_handler = "";
	if((isAltfire & DND_ATK_SECONDARY)) {
		// if left out use primary
		if(Weapons_Data[wepid].ammo_name2 == "")
			ammo_type = Weapons_Data[wepid].ammo_name1;
		else
			ammo_type = Weapons_Data[wepid].ammo_name2;
			
		// if left out use base
		ammo_take_amt = Weapons_Data[wepid].ammo_use2;
		if(!ammo_take_amt)
			ammo_take_amt = Weapons_Data[wepid].ammo_use1;
	}
	else {
		ammo_type = Weapons_Data[wepid].ammo_name1;
		ammo_take_amt = Weapons_Data[wepid].ammo_use1;
	}
	
	if(ammo_slot != DND_AMMOSLOT_SPECIAL)
		GiveInventory("DnD_UsedNonSpecial", 1);
	
	int proj_id = -1;				// id of proj to fire
	str proj_name_alt = "";			// alternative name
	int count = 1;					// proj count
	int sp_x = 0, sp_y = 0;			// spread values
	int angle_vec = GetVec2();		// negative is left, positive is right
	int offset_vec = GetVec3();
	bool use_default = false;		// default behavior

	// check for cyborg instability
	flags |= (IsTechWeapon(wepid) && CheckInventory("Cyborg_InstabilityStack") == DND_MAXCYBORG_INSTABILITY && !CheckInventory("Cyborg_Instability_CD") && RunLuckBasedChance(pnum, DND_CYBORG_INSTABILITY_CHANCE, DND_LUCK_OUTCOME_GAIN)) * DND_ATF_INSTABILITY;
	
	// we will scale count whenever the weapon would require it!
	// unused variables use their default values from above
	switch(wepid) {
		// SLOT 1
		case DND_WEAPON_FIST:
			use_default = true;
			proj_id = DND_PROJ_FIST;
			sp_x = 0;
			SetInventory("FistSide", !CheckInventory("FistSide"));
			proj_name_alt = ProjectileInfo[proj_id].name;
			if(CheckInventory("PowerStrength"))
				proj_name_alt = "FistPuffX";
		break;
		case DND_WEAPON_CHAINSAW:
			use_default = true;
			proj_id = DND_PROJ_CHAINSAW;
			proj_name_alt = ProjectileInfo[proj_id].name;
		break;
		case DND_WEAPON_DOUBLECHAINSAW:
			use_default = true;
			count = 2;
			proj_id = DND_PROJ_DOUBLECHAINSAW;
			if(isAltFire & DND_ATK_OTHER_DIR)
				proj_name_alt = "DChainsawPuff_NoSound";
			else
				proj_name_alt = ProjectileInfo[proj_id].name;
		break;
		case DND_WEAPON_SICKLE:
			use_default = true;
			proj_id = DND_PROJ_SICKLE;
			if(isAltFire & DND_ATK_SECONDARY) {
				proj_name_alt = "SicklePuff_X";
				if(isAltFire & DND_ATK_OTHER_DIR)
					Do_Projectile_Attack_Named(owner, pnum, "SickleGhostwave", wepid, 1, 48, angle_vec, offset_vec, 0, 0, flags);
			}
			else
				proj_name_alt = ProjectileInfo[proj_id].name;
		break;
		case DND_WEAPON_EXCALIBAT:
			use_default = true;
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				proj_id = DND_PROJ_EXCALIBAT;
				proj_name_alt = ProjectileInfo[proj_id].name;
				
				switch(CheckInventory("DnD_Weapon_FrameChecker")) {
					case 0:
						sp_x = -18.0;
					break;
					case 1:
						sp_x = 0.0;
					break;
					case 2:
						sp_x = 18.0;
					break;
					case 3:
						sp_x = 36.0;
					break;
				}
			}
			else {
				proj_id = DND_PROJ_EXCALIBAT2;
				sp_x = CheckInventory("BatCharge");
				
				// dont make the delay cause ammo from being taken late
				ammo_take_amt = max(1, (ammo_take_amt * sp_x) / 100);
				TakeInventory(ammo_type, ammo_take_amt);
				flags |= DND_ATF_NOAMMOTAKE;
				
				// angle increments of 4 degrees, start from -32 deg
				// max charges results in 17
				sp_y = 1 + (16 * sp_x) / 100;
				for(count = 0; count < sp_y; ++count) {
					vec2[angle_vec].x = -2.0 * (sp_y - 1) + count * 4.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0.0, 0.0, flags);
					Delay(const:1);
				}

				SetInventory("BatCharge", 0);
				sp_x = 0;
				sp_y = 0;
			}
		break;
		case DND_WEAPON_KATANA:
			use_default = false;
			proj_id = DND_PROJ_KATANA;
			
			// regular primary sheathe -- IaiSlashNormal
			if(!isAltFire) {
				switch(CheckInventory("DnD_Weapon_FrameChecker")) {
					case 0:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, 24.0, 12.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, 16.0, 8.0, flags);
					break;
					case 1:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, 8.0, 4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, 0.0, 0.0, flags);
					break;
					case 2:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, -8.0, -4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, -16.0, -8.0, flags);
					break;
					case 3:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, -24.0, -12.0, flags);
					break;
				}
			}
			else if(isAltFire == DND_ATK_PRIMARY) {
				// left slash -- LeftSlashNormal
				switch(CheckInventory("DnD_Weapon_FrameChecker")) {
					case 0:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, -24.0, -12.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, -16.0, -8.0, flags);
					break;
					case 1:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, -8.0, -4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, 0.0, 0.0, flags);
					break;
					case 2:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, 8.0, 4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, 16.0, 8.0, flags);
					break;
					case 3:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, 24.0, 12.0, flags);
					break;
				}
			}
			else if(isAltFire == DND_ATK_OTHER_DIR) {
				// right slash -- RightSlashNormal
					switch(CheckInventory("DnD_Weapon_FrameChecker")) {
					case 0:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, 24.0, -12.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, 16.0, -8.0, flags);
					break;
					case 1:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, 8.0, -4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, 0.0, 0.0, flags);
					break;
					case 2:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, -8.0, 4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, -16.0, 8.0, flags);
					break;
					case 3:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, -24.0, 12.0, flags);
					break;
				}
			}
			else if(isAltFire == DND_ATK_SECONDARY) {
				// altfire -- ContinueAltFire
				switch(CheckInventory("DnD_Weapon_FrameChecker")) {
					case 0:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, 24.0, 12.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, 16.0, 8.0, flags);
					break;
					case 1:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, 8.0, 4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, 0.0, 0.0, flags);
					break;
					case 2:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, -8.0, -4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, -16.0, -8.0, flags);
					break;
					case 3:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff3", proj_id, -24.0, -12.0, flags);
					break;
					
					// shooter 9-12
					case 4:
					case 12:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, 24.0, -12.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, 16.0, -8.0, flags);
					break;
					case 5:
					case 13:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, 8.0, -4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, 0.0, 0.0, flags);
					break;
					case 6:
					case 14:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, -8.0, 4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, -16.0, 8.0, flags);
					break;
					case 7:
					case 15:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, -24.0, 12.0, flags);
					break;
					
					// shooter 5-8
					case 8:
					case 16:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, -24.0, -12.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, -16.0, -8.0, flags);
					break;
					case 9:
					case 17:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, -8.0, -4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, 0.0, 0.0, flags);
					break;
					case 10:
					case 18:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, 8.0, 4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, 16.0, 8.0, flags);
					break;
					case 11:
					case 19:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, 24.0, 12.0, flags);
					break;
					
					// 13 - 14 - 13 shooters
					case 20:
					case 22:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff_Id0", proj_id, 0.0, 0.0, flags, 12.0);
					break;
					case 21:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff_Id0x4", proj_id, 0.0, 0.0, flags, 36.0);
						Do_Projectile_Attack(owner, pnum, DND_PROJ_KATANA2, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					break;
				}
			}
			else if(isAltFire & DND_ATK_OTHER_DIR) {
				// combo altfire -- ComboUnsheathed
				switch(CheckInventory("DnD_Weapon_FrameChecker")) {
					case 0:
					case 2:
					case 11:
					case 13:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff_Id0", proj_id, 0.0, 0.0, flags, 12.0);
					break;
					case 1:
					case 12:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff_Id0x4", proj_id, 0.0, 0.0, flags, 36.0);
						Do_Projectile_Attack(owner, pnum, DND_PROJ_KATANA2, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					break;
					
					case 3:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, 24.0, -12.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, 16.0, -8.0, flags);
					break;
					case 4:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, 8.0, -4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, 0.0, 0.0, flags);
					break;
					case 5:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, -8.0, 4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, -16.0, 8.0, flags);
					break;
					case 6:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff", proj_id, -24.0, 12.0, flags);
					break;
					
					case 7:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, -24.0, -12.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, -16.0, -8.0, flags);
					break;
					case 8:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, -8.0, -4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, 0.0, 0.0, flags);
					break;
					case 9:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, 8.0, 4.0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, 16.0, 8.0, flags);
					break;
					case 10:
						Do_Melee_Attack(owner, pnum, wepid, 1, "KatanaPuff2", proj_id, 24.0, 12.0, flags);
					break;
				}
			}
		break;
		case DND_WEAPON_DUSKBLADE:
			proj_id = DND_PROJ_DUSKBLADE1;
			use_default = false;
			switch(CheckInventory("DnD_Weapon_FrameChecker")) {
				case 0:
					if(isAltFire & DND_ATK_OTHER_DIR)
						Do_Melee_Attack(owner, pnum, wepid, 1, "DuskBladePuff_NoArmor", proj_id, 16.0, 5.0, flags);
					else {
						// fire the slashing projectile
						Do_Projectile_Attack(owner, pnum, DND_PROJ_DUSKBLADE2, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
						Do_Melee_Attack(owner, pnum, wepid, 1, "DuskBladePuff2", proj_id, 16.0, 5.0, flags);
					}
				break;
				case 1:
					if(isAltFire & DND_ATK_SECONDARY)
						Do_Melee_Attack(owner, pnum, wepid, 1, "DuskBladePuff2", proj_id, 8.0, 1.0, flags);
					else
						Do_Melee_Attack(owner, pnum, wepid, 1, "DuskBladePuff", proj_id, 8.0, 1.0, flags);
				break;
				case 2:
					if(isAltFire & DND_ATK_SECONDARY)
						Do_Melee_Attack(owner, pnum, wepid, 1, "DuskBladePuff2", proj_id, 0.0, -4.0, flags);
					else
						Do_Melee_Attack(owner, pnum, wepid, 1, "DuskBladePuff", proj_id, 0.0, -4.0, flags);
				break;
				case 3:
					if(isAltFire & DND_ATK_OTHER_DIR)
						Do_Melee_Attack(owner, pnum, wepid, 1, "DuskBladePuff_NoArmor", proj_id, -8.0, -9.0, flags);
					else
						Do_Melee_Attack(owner, pnum, wepid, 1, "DuskBladePuff2", proj_id, -8.0, -9.0, flags);
				break;
			}
		break;
		case DND_WEAPON_INFERNOSWORD:
			use_default = false;
			if(isAltFire & DND_ATK_PRIMARY) {
				proj_id = DND_PROJ_INFERNOSWORD1;
				Do_Melee_Attack(owner, pnum, wepid, 1, "InfernoSwordPuff", proj_id, -24.0 + 12.0 * CheckInventory("DnD_Weapon_FrameChecker"), 0.0, flags);
			}
			else {
				proj_id = DND_PROJ_INFERNOSWORD2;
				use_default = false;
				
				if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE)) {
					for(count = 0; count < 5; ++count) {
						vec2[angle_vec].x = -11.25 + count * 5.625;
						vec3[offset_vec].z = 10.0 - count * 5.0;
						if(count)
							Do_Projectile_Attack_Named(owner, pnum, StrParam(s:"InfernoSwordMissile_", d:count + 1), wepid, 1, 1, angle_vec, offset_vec, 0, 0, flags, proj_id);
						else
							Do_Projectile_Attack_Named(owner, pnum, StrParam(s:"InfernoSwordMissile_", d:count + 1), wepid, 1, 28, angle_vec, offset_vec, 0, 0, flags, proj_id);
					}
				}
				else {
					count = GetPelletCount(pnum, 5);
					Do_Attack_Circle(owner, pnum, proj_id, wepid, count, ProjectileInfo[proj_id].spd_range, flags);
				}
			}
		break;
		
		
		// SLOT 2
		case DND_WEAPON_PISTOL:
			proj_id = DND_PROJ_PISTOL;
			use_default = true;
			sp_x = 0.5;
		break;
		case DND_WEAPON_AKIMBOPISTOL:
			proj_id = DND_PROJ_AKIMBO;
			use_default = true;
			sp_x = 3.5;
			sp_y = 2.5;
		break;
		case DND_WEAPON_MAGNUM:
			proj_id = DND_PROJ_MAGNUMREVOLVER;
			use_default = true;
			sp_x = 0.1;
			sp_y = 0.1;
		break;
		case DND_WEAPON_LASERPISTOL:
			proj_id = DND_PROJ_LASERPISTOL;
			use_default = true;
			sp_x = 2.0;
			sp_y = 1.25;
		break;
		case DND_WEAPON_SCATTERGUN:
			proj_id = DND_PROJ_SCATTERGUN;
			use_default = true;
			// primary fire has spread
			if(!(isAltfire & DND_ATK_SECONDARY))
				sp_x = 3.0;
		break;
		case DND_WEAPON_RUBYWAND:
			// secondary attack fires one projectile
			if(isAltfire & DND_ATK_SECONDARY) {
				proj_id = DND_PROJ_RUBYWANDFLAME;
				use_default = true;
			}
			else if(isAltFire & DND_ATK_OTHER_DIR) {
				// other dir fires a single projectile
				proj_id = DND_PROJ_RUBYWAND;
				use_default = true;
			}
			else {
				// regular primary fire fires 3 projectiles with fixed angle, so we don't use default
				proj_id = DND_PROJ_RUBYWAND;
				
				if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE)) {
					use_default = false;
					
					// -3, 0, 3 deg angles
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, sp_x, sp_y, flags);
					vec2[angle_vec].x = -3.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, sp_x, sp_y, flags);
					vec2[angle_vec].x = 3.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, sp_x, sp_y, flags);
				}
				else {
					use_default = true;
					count = 3;
				}
			}
		break;
		case DND_WEAPON_ASSAULTRIFLE:
			if(isAltFire & DND_ATK_OTHER_DIR) {
				sp_x = 0.1;
				sp_y = 0.1;
			}
			else {
				sp_x = 3.6;
				sp_y = 2.4;
			}
			use_default = true;
			proj_id = DND_PROJ_ASSAULTRIFLE;
		break;
		case DND_WEAPON_VIPERSTAFF:
			if(isAltFire & DND_ATK_SECONDARY) {
				use_default = true;
				proj_id = DND_PROJ_VIPERMISSILE;
			}
			else {
				use_default = false;
				
				// this spawns a minion
				SetVec3XYZ(offset_vec, 24.0, 0.0, -12.0);
				Do_Minion_Summon(owner, ProjectileInfo[DND_PROJ_VIPERCRAWLER].name, offset_vec, ProjectileInfo[DND_PROJ_VIPERCRAWLER].spd_range);
			}
		break;
		
		// SLOT 3
		case DND_WEAPON_SHOTGUN:
			use_default = true;
			sp_x = 5.6;
			sp_y = 5.6;
			
			switch(CheckInventory("SpecialAmmoMode_3")) {
				case 3:
					count = 10;
					proj_id = DND_PROJ_ELECTRICSHELL;
					
					if(!(flags & DND_ATF_NOAMMOTAKE)) {
						HandleAmmoGainChance(DND_AMMOSLOT_SPECIAL, SSAM_SHOCK, Weapons_Data[wepid].ammo_use1, owner);
						TakeInventory(SpecialAmmoInfo_Str[SSAM_SHOCK][DND_SPECIALAMMO_NAME], Weapons_Data[wepid].ammo_use1);
					}
					PlaySound(owner, "Shells/Shock", CHAN_5);
					PlaySound(owner, "Shells/Shock2", CHAN_7);
					ammo_handler = "ShockShellHandler";
				break;
				case 2:
					count = 10;
					proj_id = DND_PROJ_MAGNUMSHELL;
					
					if(!(flags & DND_ATF_NOAMMOTAKE)) {
						HandleAmmoGainChance(DND_AMMOSLOT_SPECIAL, SSAM_MAGNUM, Weapons_Data[wepid].ammo_use1, owner);
						TakeInventory(SpecialAmmoInfo_Str[SSAM_MAGNUM][DND_SPECIALAMMO_NAME], Weapons_Data[wepid].ammo_use1);
					}
					PlaySound(owner, "Shells/Pierce", CHAN_5);
					ammo_handler = "MagnumShellHandler";
				break;
				case 1:
					count = 10;
					proj_id = DND_PROJ_FLECHETTE;
					
					if(!(flags & DND_ATF_NOAMMOTAKE)) {
						HandleAmmoGainChance(DND_AMMOSLOT_SPECIAL, SSAM_FLECHETTE, Weapons_Data[wepid].ammo_use1, owner);
						TakeInventory(SpecialAmmoInfo_Str[SSAM_FLECHETTE][DND_SPECIALAMMO_NAME], Weapons_Data[wepid].ammo_use1);
					}
					PlaySound(owner, "Shells/Flechette", CHAN_5);
					ammo_handler = "FlechetteHandler";
				break;
				case 0:
					count = 10;
					proj_id = DND_PROJ_SHOTGUN;
					
					if(!(flags & DND_ATF_NOAMMOTAKE)) {
						HandleAmmoGainChance(DND_AMMOSLOT_SHELL, AMMO_SHELL, Weapons_Data[wepid].ammo_use1, owner);
						TakeInventory(Weapons_Data[wepid].ammo_name1, Weapons_Data[wepid].ammo_use1);
					}
					PlaySound(owner, "weapons/sgun1fire", CHAN_WEAPON);
				break;
			}
		break;
		case DND_WEAPON_PURIFIER:
			proj_id = DND_PROJ_PURIFIER;
			use_default = true;
			sp_x = 3.6;
			sp_y = 3.6;
			
			switch(CheckInventory("SpecialAmmoMode_3")) {
				case 3:
					count = 10;
					proj_id = DND_PROJ_ELECTRICSHELL;
					
					if(!(flags & DND_ATF_NOAMMOTAKE)) {
						HandleAmmoGainChance(DND_AMMOSLOT_SPECIAL, SSAM_SHOCK, Weapons_Data[wepid].ammo_use1, owner);
						TakeInventory(SpecialAmmoInfo_Str[SSAM_SHOCK][DND_SPECIALAMMO_NAME], Weapons_Data[wepid].ammo_use1);
					}
					PlaySound(owner, "Shells/Shock", CHAN_5);
					PlaySound(owner, "Shells/Shock2", CHAN_7);
					ammo_handler = "ShockShellHandler";
				break;
				case 2:
					count = 10;
					proj_id = DND_PROJ_MAGNUMSHELL;
					
					if(!(flags & DND_ATF_NOAMMOTAKE)) {
						HandleAmmoGainChance(DND_AMMOSLOT_SPECIAL, SSAM_MAGNUM, Weapons_Data[wepid].ammo_use1, owner);
						TakeInventory(SpecialAmmoInfo_Str[SSAM_MAGNUM][DND_SPECIALAMMO_NAME], Weapons_Data[wepid].ammo_use1);
					}
					PlaySound(owner, "Shells/Pierce", CHAN_5);
					ammo_handler = "MagnumShellHandler";
				break;
				case 1:
					count = 10;
					proj_id = DND_PROJ_FLECHETTE;
					
					if(!(flags & DND_ATF_NOAMMOTAKE)) {
						HandleAmmoGainChance(DND_AMMOSLOT_SPECIAL, SSAM_FLECHETTE, Weapons_Data[wepid].ammo_use1, owner);
						TakeInventory(SpecialAmmoInfo_Str[SSAM_FLECHETTE][DND_SPECIALAMMO_NAME], Weapons_Data[wepid].ammo_use1);
					}
					PlaySound(owner, "Shells/Flechette", CHAN_5);
					ammo_handler = "FlechetteHandler";
				break;
				case 0:
					count = 15;
					proj_id = DND_PROJ_PURIFIER;
					
					if(!(flags & DND_ATF_NOAMMOTAKE)) {
						HandleAmmoGainChance(DND_AMMOSLOT_SHELL, AMMO_SHELL, Weapons_Data[wepid].ammo_use1, owner);
						TakeInventory(Weapons_Data[wepid].ammo_name1, Weapons_Data[wepid].ammo_use1);
					}
					PlaySound(owner, "weapons/shotgunfirenew", CHAN_WEAPON);
				break;
			}
		break;
		case DND_WEAPON_KILLSTORM:
			proj_id = DND_PROJ_KILLSTORM;
			use_default = true;
			count = 12;
			sp_x = 9.6;
			sp_y = 7.2;
		break;
		case DND_WEAPON_EMERALDWAND:
			use_default = true;
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				proj_id = DND_PROJ_EMERALDMAIN;
				count = 5;
				sp_x = 6.0;
				sp_y = 4.5;
			}
			else
				proj_id = DND_PROJ_EMERALDRAINER;
		break;
		case DND_WEAPON_SUPERSHOTGUN:
			proj_id = DND_PROJ_SSG;
			use_default = true;
			count = 20;
			sp_x = 11.2;
			sp_y = 7.1;
		break;
		case DND_WEAPON_HEAVYSUPERSHOTGUN:
			use_default = false;
			count = 14;
			sp_x = 9.6;
			sp_y = 5.8;
			
			// check if circle attack, fire in circle otherwise
			if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE)) {
				// this weapon fires half pellets as railgun, half as hitscan -- so we tuck it at the end
				Do_Hitscan_Attack(owner, pnum, DND_PROJ_HEAVYSSG, wepid, count, ProjectileInfo[DND_PROJ_HEAVYSSG].spd_range, sp_x, sp_y, flags);
				Do_Railgun_Attack("HeavySSG_RailHelper", count);
			}
			else {
				GiveInventory("HeavySSG_RailHelper_Circle", 1);
				count = GetPelletCount(pnum, count);
				Do_Attack_Circle(owner, pnum, DND_PROJ_HEAVYSSG, wepid, count, ProjectileInfo[DND_PROJ_HEAVYSSG].spd_range, flags);
			}
		break;
		case DND_WEAPON_ERASUS:
			proj_id = DND_PROJ_ERASUS;
			use_default = true;
			
			// use other dir to make it different from primary vs alt, and alt to use the magazine not main ammo pool
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				count = 18;
				sp_x = 9.0;
				sp_y = 6.4;
			}
			else {
				count = 36;
				sp_x = 14.8;
				sp_y = 10.2;
				
				// take twice ammo for altfire
				ammo_take_amt <<= 1;
			}
		break;
		case DND_WEAPON_HELLSMAW:
			use_default = false;
			
			// left or right option to check for here
			if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE)) {
				if(!(isAltFire & DND_ATK_OTHER_DIR)) {
					// left
					vec2[angle_vec].x = -1.0;
					vec3[offset_vec].y = -12.0;
					Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMAIN, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					
					SetVec3XYZ(offset_vec, 0.0, 4.0, 0.0);
					Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMINI, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					SetVec3XYZ(offset_vec, 0.0, -12.0, 16.0);
					Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMINI, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					SetVec3XYZ(offset_vec, 0.0, -28.0, 0.0);
					Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMINI, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				}
				else {
					// right
					vec2[angle_vec].x = 1.0;
					vec3[offset_vec].y = 12.0;
					Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMAIN, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					
					SetVec3XYZ(offset_vec, 0.0, -4.0, 0.0);
					Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMINI, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					SetVec3XYZ(offset_vec, 0.0, 12.0, 16.0);
					Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMINI, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					SetVec3XYZ(offset_vec, 0.0, 28.0, 0.0);
					Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMINI, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				}
			}
			else {
				Do_Attack_Circle(owner, pnum, DND_PROJ_HELLSMAWMAIN, wepid, GetPelletCount(pnum, 1), ProjectileInfo[DND_PROJ_HELLSMAWMAIN].spd_range, flags);
				Do_Attack_Circle(owner, pnum, DND_PROJ_HELLSMAWMINI, wepid, GetPelletCount(pnum, 3), ProjectileInfo[DND_PROJ_HELLSMAWMINI].spd_range, flags);
			}
		break;
		case DND_WEAPON_AXE:
			use_default = true;
			vec2[angle_vec].x = 0.875;
			if(isAltFire & DND_ATK_SECONDARY)
				proj_id = DND_PROJ_AXETHROWN;
			else {
				proj_id = DND_PROJ_AXEMELEE;
				if(!(isAltFire & DND_ATK_OTHER_DIR))
					proj_name_alt = ProjectileInfo[DND_PROJ_AXEMELEE].name;
				else
					proj_name_alt = "ThunderAxePuff_NoMana";
			}
		break;
		case DND_WEAPON_SILVERGUN:
			proj_id = DND_PROJ_WHITEDEATH;
			use_default = true;
			count = 10;
			sp_x = 6.4;
			sp_y = 4.8;
		break;
		case DND_WEAPON_SLAYER:
			use_default = false;
			proj_id = DND_PROJ_SLAYER;
		
			if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE)) {
				vec2[angle_vec].x = -3.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				vec2[angle_vec].x = 3.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				vec2[angle_vec].x = -6.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				vec2[angle_vec].x = 6.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				vec2[angle_vec].x = -1.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				vec2[angle_vec].x = 1.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
			}
			else {
				count = GetPelletCount(pnum, 6);
				Do_Attack_Circle(owner, pnum, proj_id, wepid, count, ProjectileInfo[proj_id].spd_range, flags);
			}
		break;
		case DND_WEAPON_DEADLOCK:
			use_default = true;
			count = 16;
			sp_x = 7.0;
			sp_y = 5.2;
			
			switch(CheckInventory("SpecialAmmoMode_3")) {
				case 3:
					count = 1;
					proj_id = DND_PROJ_SLUGSHELL;
					
					if(!(flags & DND_ATF_NOAMMOTAKE)) {
						HandleAmmoGainChance(DND_AMMOSLOT_SPECIAL, SSAM_SHOCK, Weapons_Data[wepid].ammo_use1, owner);
						TakeInventory(SpecialAmmoInfo_Str[SSAM_SHOCK][DND_SPECIALAMMO_NAME], Weapons_Data[wepid].ammo_use1);
					}
					PlaySound(owner, "Shells/Slug", CHAN_5);
					ammo_handler = "SlugShellHandler";
				break;
				case 2:
					count = 10;
					proj_id = DND_PROJ_MAGNUMSHELL;
					
					if(!(flags & DND_ATF_NOAMMOTAKE)) {
						HandleAmmoGainChance(DND_AMMOSLOT_SPECIAL, SSAM_MAGNUM, Weapons_Data[wepid].ammo_use1, owner);
						TakeInventory(SpecialAmmoInfo_Str[SSAM_MAGNUM][DND_SPECIALAMMO_NAME], Weapons_Data[wepid].ammo_use1);
					}
					PlaySound(owner, "Shells/Pierce", CHAN_5);
					ammo_handler = "MagnumShellHandler";
				break;
				case 1:
					count = 10;
					proj_id = DND_PROJ_FLECHETTE;
					
					if(!(flags & DND_ATF_NOAMMOTAKE)) {
						HandleAmmoGainChance(DND_AMMOSLOT_SPECIAL, SSAM_FLECHETTE, Weapons_Data[wepid].ammo_use1, owner);
						TakeInventory(SpecialAmmoInfo_Str[SSAM_FLECHETTE][DND_SPECIALAMMO_NAME], Weapons_Data[wepid].ammo_use1);
					}
					PlaySound(owner, "Shells/Flechette", CHAN_5);
					ammo_handler = "FlechetteHandler";
				break;
				case 0:
					count = 16;
					proj_id = DND_PROJ_DEADLOCK;
					
					if(!(flags & DND_ATF_NOAMMOTAKE)) {
						HandleAmmoGainChance(DND_AMMOSLOT_SHELL, AMMO_SHELL, Weapons_Data[wepid].ammo_use1, owner);
						TakeInventory(Weapons_Data[wepid].ammo_name1, Weapons_Data[wepid].ammo_use1);
					}
					PlaySound(owner, "Deadlock/Fire", CHAN_WEAPON);
				break;
			}
		break;
		case DND_WEAPON_NITROGENCROSSBOW:
			use_default = false;
			
			if(!(isAltFire & DND_ATK_SECONDARY))
				Do_Railgun_Attack("CryobowRailHelper_1", 1);
			else
				Do_Railgun_Attack("CryobowRailHelper_2", 1);
		break;
		case DND_WEAPON_WHEELOFTORMENT:
			use_default = true;
			
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				// primary
				proj_id = DND_PROJ_WHEELOFTORMENT_1;
			}
			else if(isAltFire & DND_ATK_OTHER_DIR) {
				// special 3rd proj that gives ammo
				proj_id = DND_PROJ_WHEELOFTORMENT_3;
			}
			else {
				// 2nd proj
				proj_id = DND_PROJ_WHEELOFTORMENT_2;
			}
		break;
		case DND_WEAPON_CHARONBLASTER:
			use_default = false;
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				proj_id = DND_PROJ_CHARON;
				if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE)) {
					vec2[angle_vec].x = -12.5;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = 0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = 12.5;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				}
				else {
					count = GetPelletCount(pnum, 3);
					Do_Attack_Circle(owner, pnum, proj_id, wepid, count, ProjectileInfo[proj_id].spd_range, flags);
				}
			}
			else {
				sp_x = CheckInventory("CharonBeamCounter");
				
				switch(sp_x) {
					case 0:
						Do_Hitscan_Attack_Named(owner, pnum, "CharonDamagingPuff_1", wepid, 1, 768.0, 0, 0, 0);
					break;
					case 1:
						Do_Hitscan_Attack_Named(owner, pnum, "CharonDamagingPuff_2", wepid, 1, 768.0, 0, 0, 0);
					break;
					case 2:
						Do_Hitscan_Attack_Named(owner, pnum, "CharonDamagingPuff_3", wepid, 1, 768.0, 0, 0, 0);
					break;
				}
				
				GiveInventory("CharonBeamCounter", 1);
				// we reset at every 3
				if(sp_x == 2)
					SetInventory("CharonBeamCounter", 0);
			}
		break;
		case DND_WEAPON_PLASMACANNON:
			use_default = false;
			proj_id = DND_PROJ_PLASMACANNON;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				// circle pattern -- check circle fire too
				if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE)) {
					count = 12;
					for(sp_x = 0; sp_x < 12; ++sp_x) {
						SetVec2XY(angle_vec, 4 * cos(ANG_TO_DOOM(30.0 * sp_x)), 4 * sin(ANG_TO_DOOM(30.0 * sp_x)));
						Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					}
				}
				else {
					count = GetPelletCount(pnum, 12);
					Do_Attack_Circle(owner, pnum, proj_id, wepid, count, ProjectileInfo[proj_id].spd_range, flags);
				}
			}
			else {
				// spread pattern -- check circle fire too				
				if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE)) {
					count = 6;
					vec2[angle_vec].x = 1.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = 4.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = 6.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = -1.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = -4.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = -6.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				}
				else {
					count = GetPelletCount(pnum, 6);
					Do_Attack_Circle(owner, pnum, proj_id, wepid, count, ProjectileInfo[proj_id].spd_range, flags);
				}
			}
		break;
		case DND_WEAPON_SHOCKER:
			// altfire fires an AoE but we dont need direction on that to be precise really, so it just fires pellets
			count = 18;
			proj_id = DND_PROJ_SHOCKER;
			use_default = true;
			sp_x = 8.4;
			sp_y = 6.2;
		break;
		case DND_WEAPON_HADES:
			use_default = true;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				proj_id = DND_PROJ_HADES;
				count = 15;
				sp_x = 11.6;
				sp_y = 9.0;
			}
			else {
				proj_id = DND_PROJ_HADES_2;
				count = 1;
			}
		break;
		case DND_WEAPON_CROSSBOW:
			use_default = false;
			sp_x = CheckInventory("Crossbow_PullLevel");
			
			switch(sp_x) {
				case 0:
					Do_Projectile_Attack_Named(owner, pnum, "CrossbowBolt_1", wepid, 1, 64, angle_vec, offset_vec, 0, 0, 0);
				break;
				case 1:
					Do_Projectile_Attack_Named(owner, pnum, "CrossbowBolt_2", wepid, 1, 80, angle_vec, offset_vec, 0, 0, 0);
				break;
				case 2:
					Do_Projectile_Attack_Named(owner, pnum, "CrossbowBolt_3", wepid, 1, 120, angle_vec, offset_vec, 0, 0, 0);
				break;
				case 3:
					Do_Projectile_Attack_Named(owner, pnum, "CrossbowBolt_4", wepid, 1, 160, angle_vec, offset_vec, 0, 0, 0);
				break;
				case 4:
					Do_Projectile_Attack_Named(owner, pnum, "CrossbowBolt_5", wepid, 1, 200, angle_vec, offset_vec, 0, 0, 0);
				break;
			}
		break;
		
		// SLOT 4
		case DND_WEAPON_MACHINEGUN:
			use_default = true;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				proj_id = DND_PROJ_MACHINEGUN;
				sp_x = 2.4;
				sp_y = 1.6;
			}
			else {
				// grenades
				flags |= DND_ATF_USEGRAVITY;
				switch(CheckInventory("SpecialAmmoMode_4")) {
					case AMMO_40MMHEGRENADE:
						proj_id = DND_PROJ_HEGRENADE;
						ammo_type = SpecialAmmoInfo_Str[SSAM_40MMHE - 1][DND_SPECIALAMMO_NAME];
						ammo_handler = "HEGrenadeHandler";
					break;
					case AMMO_40MMSONIC:
						proj_id = DND_PROJ_SONICGRENADE;
						ammo_type = SpecialAmmoInfo_Str[SSAM_40MMSONIC - 1][DND_SPECIALAMMO_NAME];
						ammo_handler = "SonicGrenadeHandler";
					break;
					case AMMO_BASICGRENADE:
						proj_id = DND_PROJ_GRENADESPECIAL;
						ammo_type = Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_name1;
						ammo_handler = "NormalGrenadeHandler";
					break;
				}
			}
		break;
		case DND_WEAPON_HEAVYMACHINEGUN:
			use_default = true;
			proj_id = DND_PROJ_HEAVYMACHINEGUN;
			
			if(!CheckInventory("WeaponZoom")) {
				sp_x = 4.0;
				sp_y = 2.0;
			}
			else {
				sp_x = 1.6;
				sp_y = 0.8;
			}
		break;
		case DND_WEAPON_LEADSPITTER:
			use_default = true;
			proj_id = DND_PROJ_LEADSPITTER;
			count = 2;
			sp_x = 7.2;
			sp_y = 5.2;
		break;
		case DND_WEAPON_DEMONSEALER:
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				// fire as normal projectile
				use_default = true;
				proj_id = DND_PROJ_DEMONSEALER;
			}
			else {
				use_default = false;
				
				// make as a minion that sets TARGET pointer instead of MASTER
				vec3[offset_vec].z = -36.0; // set it to feet level of player not above!
				Do_Minion_Summon(owner, "DemonSealTrap", offset_vec, 64, KANJI_TRAP_TID + pnum, DND_MF_MAKETARGET);
			}
		break;
		case DND_WEAPON_DESOLATOR:
			use_default = true;
			proj_id = DND_PROJ_DESOLATOR;
			sp_x = 8.4;
			sp_y = 6.2;

			GiveOverheat(pnum, "DesolatorOverheat", 2, DND_WEAPON_DESOLATOR);
		break;
		case DND_WEAPON_MINIGUN:
			use_default = true;
			proj_id = DND_PROJ_MINIGUN;
			count = 2; // actually fires 2 bullets for high bpm
			sp_x = 10.5;
			sp_y = 5.25;
		break;
		case DND_WEAPON_EBONYCANNON:
			use_default = false;
			
			if(!(isAltFire & DND_ATK_SECONDARY))
				proj_id = DND_PROJ_EBONY_1;
			else {
				proj_id = DND_PROJ_EBONY_2;
			}
			
			flags |= DND_ATF_USEGRAVITY;
			
			// 4 pattern of ebony balls or circle
			if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE)) {
				SetVec3XYZ(offset_vec, 0, 10.0, 8.0);
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				SetVec3XYZ(offset_vec, 0, -10.0, 8.0);
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				SetVec3XYZ(offset_vec, 0, 0, 18.0);
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				SetVec3XYZ(offset_vec, 0, 0, -2.0);
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
			}
			else {
				count = GetPelletCount(pnum, 4);
				Do_Attack_Circle(owner, pnum, proj_id, wepid, count, ProjectileInfo[proj_id].spd_range, flags);
			}
		break;
		case DND_WEAPON_MPPB:
			use_default = true;
			sp_x = 8.0;
			sp_y = 5.0;
			vec3[offset_vec].z = 2.0; // 6.0 z offset, we have default 36.0 from 32.0 so
			
			count = CheckInventory("MPPB_Phase");
			switch(count) {
				case 0:
					proj_id = DND_PROJ_MPPB_1;
				break;
				case 1:
					proj_id = DND_PROJ_MPPB_2;
				break;
				case 2:
					proj_id = DND_PROJ_MPPB_3;
				break;
			}
			
			if(count == 2)
				SetInventory("MPPB_Phase", 0);
			else
				GiveInventory("MPPB_Phase", 1);
			count = 1;
		break;
		case DND_WEAPON_TEMPLARMG:
			use_default = true;
			
			// regular bullets
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				proj_id = DND_PROJ_TEMPLARMG;
				sp_x = 4.4;
				sp_y = 2.8;
			}
			else {
				// grenades
				flags |= DND_ATF_USEGRAVITY;
				
				switch(CheckInventory("SpecialAmmoMode_4")) {
					case AMMO_40MMHEGRENADE:
						proj_id = DND_PROJ_HEGRENADE;
						ammo_type = SpecialAmmoInfo_Str[SSAM_40MMHE - 1][DND_SPECIALAMMO_NAME];
						ammo_handler = "HEGrenadeHandler";
					break;
					case AMMO_40MMSONIC:
						proj_id = DND_PROJ_SONICGRENADE;
						ammo_type = SpecialAmmoInfo_Str[SSAM_40MMSONIC - 1][DND_SPECIALAMMO_NAME];
						ammo_handler = "SonicGrenadeHandler";
					break;
					case AMMO_BASICGRENADE:
						proj_id = DND_PROJ_GRENADESPECIAL;
						ammo_type = Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_name1;
						ammo_handler = "NormalGrenadeHandler";
					break;
				}
			}
		break;
		case DND_WEAPON_RIOTCANNON:
			use_default = true;
			proj_id = DND_PROJ_RIOTGUN;
			count = 7;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				sp_x = 3.6;
				sp_y = 3.6;
			}
			else {
				sp_x = 7.2;
				sp_y = 7.2;
			}
			
			if(!(flags & DND_ATF_NOAMMOTAKE)) {
				HandleAmmoGainChance(DND_AMMOSLOT_CLIP, AMMO_RIOT, Weapons_Data[wepid].ammo_use1, owner);
				TakeInventory(Weapons_Data[wepid].ammo_name1, Weapons_Data[wepid].ammo_use1);
			}
		break;
		case DND_WEAPON_ACIDRIFLE:
			use_default = true;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				proj_id = DND_PROJ_ACIDBULLET;
				sp_x = 5.6;
				sp_y = 3.2;
				ammo_take_amt = Weapons_Data[DND_WEAPON_ACIDRIFLE].ammo_use1;
			}
			else {
				SetVec3XYZ(offset_vec, 0, 4.0, -12.0);
				proj_id = DND_PROJ_ACIDSTAKE;
			}
		break;
		case DND_WEAPON_FUSIONBLASTER:
			use_default = true;
			vec3[offset_vec].y = 8.0;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				proj_id = DND_PROJ_FUSIONBLASTER;
				sp_x = 2.4;
				sp_y = 1.6;
				ammo_take_amt = Weapons_Data[DND_WEAPON_FUSIONBLASTER].ammo_use1;
			}
			else {
				proj_id = DND_PROJ_FUSIONBLASTERGRENADE;
				vec2[angle_vec].x = 1.0;
				flags |= DND_ATF_USEGRAVITY;
			}
		break;
		case DND_WEAPON_INCINERATOR:
			use_default = true;
			proj_id = DND_PROJ_INCINERATOR;
			count = 11;
			sp_x = 9.0;
			sp_y = 4.8;
			flags |= DND_ATF_USEGRAVITY;
			
			GiveInventory("IncineratorStacks", 1);
			if(CheckInventory("IncineratorStacks") == 4) {
				//SetInventory("IncineratorStacks", 0);
				
				// fire the two extra lava balls
				vec3[offset_vec].y = -18.0;
				Do_Projectile_Attack_Named(owner, pnum, "IncineratorProjectile2", wepid, 1, 32, angle_vec, offset_vec, 0, 0, 0);
				vec3[offset_vec].y = 18.0;
				Do_Projectile_Attack_Named(owner, pnum, "IncineratorProjectile2", wepid, 1, 32, angle_vec, offset_vec, 0, 0, 0);
			}
			
			vec3[offset_vec].y = 0.0;
		break;
		
		// SLOT 5
		case DND_WEAPON_ROCKETLAUNCHER:
			use_default = true;
			proj_id = DND_PROJ_ROCKETLAUNCHER;
		break;
		case DND_WEAPON_TORPEDOLAUNCHER:
			use_default = true;
			proj_id = DND_PROJ_TORPEDOLAUNCHER;
		break;
		case DND_WEAPON_GRENADELAUNCHER:
			use_default = true;
			flags |= DND_ATF_USEGRAVITY;
			
			switch(CheckInventory("SpecialAmmoMode_5X")) {
				case AMMO_40MMHEGRENADE:
					proj_id = DND_PROJ_HEGRENADE;
					ammo_type = SpecialAmmoInfo_Str[SSAM_40MMHE - 1][DND_SPECIALAMMO_NAME];
					ammo_handler = "HEGrenadeHandler";
				break;
				case AMMO_40MMSONIC:
					proj_id = DND_PROJ_SONICGRENADE;
					ammo_type = SpecialAmmoInfo_Str[SSAM_40MMSONIC - 1][DND_SPECIALAMMO_NAME];
					ammo_handler = "SonicGrenadeHandler";
				break;
				case AMMO_BASICGRENADE:
					// GL uses its own grenade projectile, not special one!!
					proj_id = DND_PROJ_GRENADE;
				break;
			}
		break;
		case DND_WEAPON_ROTARYGRENADELAUNCHER:
			use_default = true;
			flags |= DND_ATF_USEGRAVITY;
			proj_id = DND_PROJ_ROTARYGRENADE;
		break;
		case DND_WEAPON_VINDICATOR:
			use_default = true;
			proj_id = DND_PROJ_VINDICATOR;
			count = 10;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				sp_x = 10.4;
				sp_y = 7.8;
			}
			else {
				sp_x = 5.2;
				sp_y = 3.9;
			}
		break;
		case DND_WEAPON_HAMMER:
			use_default = true;
			vec2[angle_vec].x = 0.875;
			if(isAltFire & DND_ATK_SECONDARY)
				proj_id = DND_PROJ_HAMMER;
			else {
				proj_id = DND_PROJ_HAMMERMELEE;
				proj_name_alt = ProjectileInfo[DND_PROJ_HAMMERMELEE].name;
			}
		break;
		case DND_WEAPON_HEAVYMISSILELAUNCHER:
			use_default = false;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				vec2[angle_vec].x = -4.0;
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HEAVYMISSILELAUNCHER_1, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
				vec2[angle_vec].x = 4.0;
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HEAVYMISSILELAUNCHER_1, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
			}
			else {
				vec3[offset_vec].y = -8.0;
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HEAVYMISSILELAUNCHER_2, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
				vec3[offset_vec].y = 8.0;
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HEAVYMISSILELAUNCHER_2, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
			}
		break;
		case DND_WEAPON_SEDRINSTAFF:
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				use_default = true;
				proj_id = DND_PROJ_SEDRIN_1;
			}
			else {
				use_default = false;
				proj_id = DND_PROJ_SEDRIN_2;
				
				vec2[angle_vec].x = -9.25;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
				vec2[angle_vec].x = -0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
				vec2[angle_vec].x = 9.25;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
			}
		break;
		case DND_WEAPON_MERCURYLAUNCHER:
			use_default = true;
			proj_id = DND_PROJ_MERCURY;
			vec2[angle_vec].x = 1.0;
		break;
		case DND_WEAPON_METEORLAUNCHER:
			use_default = true;
			proj_id = DND_PROJ_METEORLAUNCHER;
		break;
		case DND_WEAPON_HEAVYGL:
			use_default = true;
			proj_id = DND_PROJ_HEAVYGRENADE;
			flags |= DND_ATF_USEGRAVITY;
			count = CheckInventory("HeavyGLCounter") + 1;
			
			if(count > 1) {
				sp_x = 4.8;
				sp_y = 2.0;
				SetInventory("HeavyGLCounter", 0);
				
				// this is new
				flags |= DND_ATF_CANFIRECIRCLE;
			}
		break;
		case DND_WEAPON_FREEZER:
			use_default = true;
			proj_id = DND_PROJ_FREEZER;
			if(!(isAltFire & DND_ATK_OTHER_DIR))
				GiveOverheat(pnum, "FreezerOverheat", 5, DND_WEAPON_FREEZER);
			else {
				GiveOverheat(pnum, "FreezerOverheat", 10, DND_WEAPON_FREEZER);
				sp_x = 2.0;
				sp_y = 2.0;
			}
		break;
		case DND_WEAPON_GRAVDIS:
			use_default = true;
			proj_id = DND_PROJ_GRAVDIS;
		break;
		case DND_WEAPON_VOIDCANNON:
			use_default = true;
			proj_id = DND_PROJ_VOIDCANNON;
		break;
		
		// SLOT 6
		case DND_WEAPON_PLASMARIFLE:
			use_default = true;
			proj_id = DND_PROJ_PLASMA;
		break;
		case DND_WEAPON_NUCLEARPLASMARIFLE:
			use_default = true;
			proj_id = DND_PROJ_NUCLEARPLASMA;
			GiveOverheat(pnum, "PlasmaOverheat", 2, DND_WEAPON_NUCLEARPLASMARIFLE);
			if(CheckInventory("PlasmaOverheat") == 100)
				GiveInventory("PlasmaOverheatCooldown", 1);
		break;
		case DND_WEAPON_TURELCANNON:
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				if(isAltFire & DND_ATK_OTHER_DIR)
					Do_Railgun_Attack("TurelShooter2", 1);
				else
					Do_Railgun_Attack("TurelShooter1", 1);
			}
			else {
				Do_Railgun_Attack("TurelShooter3", 1);
				ammo_take_amt <<= 1; // double ammo on this one
			}
		break;
		case DND_WEAPON_NAILGUN:
			use_default = true;
			
			// proj type
			if(isAltFire & DND_ATK_SECONDARY)
				proj_id = DND_PROJ_NAILGUN_2;
			else
				proj_id = DND_PROJ_NAILGUN_1;
			
			// left right offset
			if(CheckInventory("NailCounter")) {
				SetInventory("NailCounter", 0);
				vec3[offset_vec].y = 6.0;
			}
			else {
				GiveInventory("NailCounter", 1);
				vec3[offset_vec].y = -6.0;
			}
		break;
		case DND_WEAPON_BASILISK:
			use_default = true;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				proj_id = DND_PROJ_BASILISK_1;
				sp_x = 2.5;
				sp_y = 2.0;
			}
			else {
				proj_id = DND_PROJ_BASILISK_2;
				sp_x = 3.25;
				sp_y = 2.75;
			}
		break;
		case DND_WEAPON_FROSTFANG:
			use_default = true;
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				proj_id = DND_PROJ_FROSTFANG_1;
				SetVec3XYZ(offset_vec, 0, random(-7.5, 7.5), random(-7.5, 7.5));
			}
			else
				proj_id = DND_PROJ_FROSTFANG_2;
		break;
		case DND_WEAPON_RHINORIFLE:
			use_default = true;
			proj_id = DND_PROJ_RHINO;
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				sp_x = 7.2;
				sp_y = 4.8;
			}
			else if(isAltFire & DND_ATK_SECONDARY) {
				sp_x = 0.75;
				sp_y = 0.75;
			}
			else {
				// only heavy grenades
				flags |= DND_ATF_USEGRAVITY;
				
				switch(CheckInventory("SpecialAmmoMode_6")) {
					case AMMO_40MMHEGRENADE:
						proj_id = DND_PROJ_HEGRENADE;
						ammo_type = SpecialAmmoInfo_Str[SSAM_40MMHE - 1][DND_SPECIALAMMO_NAME];
						ammo_handler = "HEGrenadeHandler";
					break;
				}
			}
		break;
		case DND_WEAPON_FLAMETHROWER:
			use_default = true;
			proj_id = DND_PROJ_FLAMETHROWER;
			sp_x = 4.0;
		break;
		case DND_WEAPON_LIGHTNINGGUN:
			use_default = false;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				Do_Hitscan_Attack_Named(owner, pnum, "LGPuffSEX", wepid, 1, 512.0, 0, 0, 0);
				Do_Railgun_Attack("LGShooter", 1);
			}
			else {
				Do_Hitscan_Attack_Named(owner, pnum, "LGAltPuff", wepid, 1, 384.0, 12.0, 5.5, 0);
				Do_Hitscan_Attack_Named(owner, pnum, "LGAltPuff_NoPain", wepid, 1, 384.0, 12.0, 5.5, 0);
				Do_Railgun_Attack("LGAltShooter", 1);
				
				// triple ammo consumption => 1 to 3
				ammo_take_amt *= 3;
			}
		break;
		case DND_WEAPON_REBOUNDER:
			use_default = false;
			vec2[angle_vec].x = 1.0;
			vec3[offset_vec].z = 5.0; // 32 + 9 = 41, we assume 36 so 5.0
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				GiveOverheat(pnum, "RebounderOverheat", 3, DND_WEAPON_REBOUNDER);
				count = CheckInventory("RebounderOverheat");
				// every 15 adds 1 up to 60, then its capped
				if(count < 15)
					Do_Projectile_Attack_Named(owner, pnum, "RebounderProjectile", wepid, 1, 32, angle_vec, offset_vec, 0, 0, 0);
				else
					Do_Projectile_Attack_Named(owner, pnum, StrParam(s:"RebounderProjectileB", d:min(count / 15, 4)), wepid, 1, 32, angle_vec, offset_vec, 0, 0, 0);
				count = 1;
			}
			else {
				// secondary of rebounder is in the array for HUD, we still use cells here
				ammo_type = Weapons_Data[wepid].ammo_name1;
				GiveOverheat(pnum, "RebounderOverheat", 10, DND_WEAPON_REBOUNDER);
				Do_Projectile_Attack_Named(owner, pnum, "RebounderProjectileAlt", wepid, 1, 48, angle_vec, offset_vec, 0, 0, 0);
			}
		break;
		case DND_WEAPON_DARKLANCE:
			// primary atk uses custom firing code
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				use_default = false;
				count = CheckInventory("LanceStacks");

				if(count < 20)
					Do_DarkLance_Shots(owner, pnum, 6, 18, 90, 0);
				else if(count < 40)
					Do_DarkLance_Shots(owner, pnum, 8, 18, 90, 0);
				else if(count < 60)
					Do_DarkLance_Shots(owner, pnum, 10, 18, 90, 1);
				else if(count < 80)
					Do_DarkLance_Shots(owner, pnum, 12, 18, 90, 1);
				else
					Do_DarkLance_Shots(owner, pnum, 14, 18, 90, 2);
				count = 1;
			}
			else {
				use_default = true;
				proj_id = DND_PROJ_DARKLANCE_SHRED;
			}
		break;
		
		// SLOT 7
		case DND_WEAPON_BFG6000:
			use_default = true;
			proj_id = DND_PROJ_BFG6000;
		break;
		case DND_WEAPON_BFG32768:
			use_default = true;
			proj_id = DND_PROJ_BFG32768;
		break;
		case DND_WEAPON_DEVASTATOR:
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				use_default = true;
				proj_id = DND_PROJ_DEVASTATOR;
				count = 5;
				sp_x = 4.8;
				sp_y = 2.8;
				
				if(isAltfire & DND_ATK_OTHER_DIR)
					SetVec3XYZ(offset_vec, 0.0, 12.0, 0.0);
				else
					SetVec3XYZ(offset_vec, 0.0, -12.0, 0.0);
			}
			else {
				// secondary uses a delayed script to match the firing anim, with delays corresponding to weapon frames in decorate
				// the entire burst timing is located here
				// offsets from left to right burst, so - to +
				// 3s in 1 tic delay, 3 times on each side with 5 total bursts
				for(sp_y = 0; sp_y < 5; ++sp_y) {
					for(sp_x = 0; sp_x < 3; ++sp_x) {
						vec3[offset_vec].y = -12.0;
						Do_Projectile_Attack_Named(owner, pnum, "DevastatorRocket_NR", wepid, 3, 32, angle_vec, offset_vec, 1.6, 0.825, 0);
						Delay(const:1);
					}
					
					for(sp_x = 0; sp_x < 3; ++sp_x) {
						vec3[offset_vec].y = 12.0;
						Do_Projectile_Attack_Named(owner, pnum, "DevastatorRocket_NR", wepid, 1, 32, angle_vec, offset_vec, 1.6, 0.825, 0);
						Delay(const:1);
					}
					Delay(const:1);
				}
			}
		break;
		case DND_WEAPON_MFG:
			use_default = true;
			proj_id = DND_PROJ_MFG;
		break;
		case DND_WEAPON_RAILGUN:
			use_default = false;
			count = CheckInventory("Railgun_Charge");
			switch(count) {
				case 0:
					Do_Railgun_Attack("RailgunShooter1", 1);
				break;
				case 1:
					Do_Railgun_Attack("RailgunShooter2", 1);
				break;
				case 2:
					Do_Railgun_Attack("RailgunShooter3", 1);
				break;
			}
			
			SetInventory("Railgun_Charge", 0);
		break;
		case DND_WEAPON_GAUSSRIFLE:
			use_default = false;
			count = CheckInventory("GaussZoom");
			
			switch(count) {
				case 0:
					Do_Hitscan_Attack_Named(owner, pnum, "GaussPuff1", wepid, 1, HITSCAN_RANGE_DEFAULT, 0, 0, 0);
					Do_Railgun_Attack("GaussShooter1", 1);
				break;
				case 1:
					Do_Hitscan_Attack_Named(owner, pnum, "GaussPuff2", wepid, 1, HITSCAN_RANGE_DEFAULT, 0, 0, 0);
					Do_Railgun_Attack("GaussShooter2", 1);
				break;
				case 2:
					Do_Hitscan_Attack_Named(owner, pnum, "GaussPuff3", wepid, 1, HITSCAN_RANGE_DEFAULT, 0, 0, 0);
					Do_Railgun_Attack("GaussShooter3", 1);
				break;
				case 3:
					Do_Hitscan_Attack_Named(owner, pnum, "GaussPuff4", wepid, 1, HITSCAN_RANGE_DEFAULT, 0, 0, 0);
					Do_Railgun_Attack("GaussShooter4", 1);
				break;
			}
		break;
		case DND_WEAPON_DEATHRAY:
			use_default = true;
			proj_id = DND_PROJ_DEATHRAY;
			GiveOverheat(pnum, "DeathRayOverheat", 40, DND_WEAPON_DEATHRAY);
		break;
		case DND_WEAPON_IONCANNON:
			use_default = true;
			proj_id = DND_PROJ_IONCANNON;
			sp_x = 7.2;
			sp_y = 5.6;
			GiveOverheat(pnum, "IonOverheat", 2, DND_WEAPON_IONCANNON);
		break;
		case DND_WEAPON_THUNDERSTAFF:
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				use_default = true;
				proj_id = DND_PROJ_THUNDERSTAFF;
			}
			else {
				use_default = false;
				ammo_take_amt = Weapons_Data[wepid].ammo_use2;
				ammo_type = Weapons_Data[wepid].ammo_name1;
				GiveInventory("ThunderRingMaker", 1);
			}
		break;
		
		// SLOT 8
		case DND_WEAPON_DEATHSTAFF:
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				use_default = true;
				proj_id = DND_PROJ_DEATHSTAFF;
			}
			else if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE)) {
				Do_Projectile_Attack_Named(owner, pnum, "HellfireLinkC", wepid, 1, 24, angle_vec, offset_vec, 0, 0, 0);
				Do_Projectile_Attack_Named(owner, pnum, "HellfireLinkF", wepid, 1, 24, angle_vec, offset_vec, 0, 0, 0);
			
				vec2[angle_vec].x = -18.0;
				Do_Projectile_Attack_Named(owner, pnum, "HellfireLinkFL", wepid, 1, 24, angle_vec, offset_vec, 0, 0, 0);
				Do_Projectile_Attack_Named(owner, pnum, "HellfireLinkCL", wepid, 1, 24, angle_vec, offset_vec, 0, 0, 0);
				
				vec2[angle_vec].x = 18.0;
				Do_Projectile_Attack_Named(owner, pnum, "HellfireLinkFR", wepid, 1, 24, angle_vec, offset_vec, 0, 0, 0);
				Do_Projectile_Attack_Named(owner, pnum, "HellfireLinkCR", wepid, 1, 24, angle_vec, offset_vec, 0, 0, 0);
			}
			else {
				// can fire circle, do so
				count = GetPelletCount(pnum, 3);
				Do_Attack_Circle_Named(owner, pnum, "HellfireLinkC", wepid, count, 24, 0);
				Do_Attack_Circle_Named(owner, pnum, "HellfireLinkF", wepid, count, 24, 0);
			}
		break;
		case DND_WEAPON_RAZORFANG:
			use_default = false;
			
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE)) {
					Do_Projectile_Attack_Named(owner, pnum, "RazorShot1", wepid, 1, 50, angle_vec, offset_vec, 0, 0, 0);
					
					for(sp_x = 1; sp_x <= 4; ++sp_x) {
						vec2[angle_vec].x = -3.0 * sp_x;
						Do_Projectile_Attack_Named(owner, pnum, "RazorShot1", wepid, 1, 50, angle_vec, offset_vec, 0, 0, 0);
						vec2[angle_vec].x = 3.0 * sp_x;
						Do_Projectile_Attack_Named(owner, pnum, "RazorShot1", wepid, 1, 50, angle_vec, offset_vec, 0, 0, 0);
					}
				}
				else {
					count = GetPelletCount(pnum, 9);
					Do_Attack_Circle_Named(owner, pnum, "RazorShot1", wepid, count, 50, 0);
				}
			}
			else if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE)) {
				// fire three skulls
				Do_Projectile_Attack_Named(owner, pnum, "RazorSkull", wepid, 1, 25, angle_vec, offset_vec, 0, 0, 0);
				vec2[angle_vec].x = -7.0;
				Do_Projectile_Attack_Named(owner, pnum, "RazorSkull", wepid, 1, 25, angle_vec, offset_vec, 0, 0, 0);
				vec2[angle_vec].x = 7.0;
				Do_Projectile_Attack_Named(owner, pnum, "RazorSkull", wepid, 1, 25, angle_vec, offset_vec, 0, 0, 0);
			}
			else {
				count = GetPelletCount(pnum, 3);
				Do_Attack_Circle_Named(owner, pnum, "RazorSkull", wepid, count, 25, 0);
			}
		break;
		case DND_WEAPON_SUNSTAFF:
			use_default = false;
			
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				if(CheckInventory("BeamCounter") == 3) {
					SetInventory("BeamCounter", 0);
					flags ^= DND_ATF_NOAMMOTAKE | DND_ATF_NOATTACKTRIGGER;
				}
				Do_Railgun_Attack("SunShooter", 1);
			}
			else if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE)) {
				Do_Projectile_Attack_Named(owner, pnum, "SunBeamSpawner", wepid, 1, 20, angle_vec, offset_vec, 0, 0, 0);
				vec2[angle_vec].x = -9.0;
				Do_Projectile_Attack_Named(owner, pnum, "SunBeamSpawner", wepid, 1, 25, angle_vec, offset_vec, 0, 0, 0);
				vec2[angle_vec].x = 9.0;
				Do_Projectile_Attack_Named(owner, pnum, "SunBeamSpawner", wepid, 1, 25, angle_vec, offset_vec, 0, 0, 0);
			}
			else {
				count = GetPelletCount(pnum, 3);
				Do_Attack_Circle_Named(owner, pnum, "SunBeamSpawner", wepid, count, 20, 0);
			}
		break;
		case DND_WEAPON_SOULREAVER:
			use_default = true;
			proj_id = DND_PROJ_SOULREAVER;
		break;
		
		// TEMP WEPS
		case DND_WEAPON_SAWEDOFF:
			use_default = true;
			proj_id = DND_PROJ_SAWEDOFF;
			
			count = 10;
			sp_x = 3.6;
			sp_y = 2.8;
			
			// altfire fires both shells at double spread
			if(isAltFire & DND_ATK_OTHER_DIR) {
				count <<= 1;
				sp_x <<= 1;
				sp_y <<= 1;
			}
		break;
		case DND_WEAPON_SOULRENDER:
			use_default = false;
			if(isAltFire & DND_ATK_PRIMARY) {
				// regular
				proj_id = DND_PROJ_SOULRENDER1;
				Do_Melee_Attack(owner, pnum, wepid, 1, "BladePuff1", proj_id, 0.0, 0.0, flags);
			}
			else if(isAltFire & DND_ATK_OTHER_DIR) {
				// drain attack
				proj_id = DND_PROJ_SOULRENDER1;
				Do_Melee_Attack(owner, pnum, wepid, 1, "BladePuff2", proj_id, 0.0, 0.0, flags);
			}
			else {
				// slash attack
				proj_id = DND_PROJ_SOULRENDER2;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				proj_id = DND_PROJ_SOULRENDER1;
				Do_Melee_Attack(owner, pnum, wepid, 1, "BladePuff3", proj_id, 0.0, 0.0, flags);
			}
		break;
		case DND_WEAPON_SMG:
			use_default = true;
			proj_id = DND_PROJ_SMG;
			sp_x = 8.0;
			sp_y = 1.2;
		break;
		case DND_WEAPON_HELLFORGECANNON:
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				use_default = true;
				proj_id = DND_PROJ_HELLFORGE_1;
			}
			else {
				use_default = false;
				proj_id = DND_PROJ_HELLFORGE_2;
				
				// dont make the delay cause ammo from being taken late
				TakeInventory(ammo_type, ammo_take_amt);
				flags |= DND_ATF_NOAMMOTAKE;
				
				// fires with 1 tic delay 11 times
				for(count = 0; count < 11; ++count) {
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 9.0, 2.0, flags);
					Delay(const:1);
				}
			}
		break;
		case DND_WEAPON_BLOODFIENDSPINE:
			use_default = true;
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				proj_id = DND_PROJ_SPINE_1;
				sp_x = 9.0;
				sp_y = 9.0;
				count = 20;
			}
			else
				proj_id = DND_PROJ_SPINE_2;
		break;
		case DND_WEAPON_ENFORCERRIFLE:
			proj_id = DND_PROJ_ENFORCERLASER;
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				use_default = true;
				vec2[angle_vec].x = 1.0;
			}
			else {
				SetVec3XYZ(offset_vec, 0, 2.0, 2.0);
				vec2[angle_vec].x = -2.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				vec2[angle_vec].x = 2.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
			}
		break;
		case DND_WEAPON_VENOM:
			use_default = true;
			count = 16;
			sp_x = 8.2;
			sp_y = 4.0;
			if(!(isAltfire & DND_ATK_SECONDARY))
				proj_id = DND_PROJ_VENOM_1;
			else {
				proj_id = DND_PROJ_VENOM_2;
				flags |= DND_ATF_USEGRAVITY;
			}
		break;
		case DND_WEAPON_DEMONHEART:
			use_default = false;
			Do_Scan_Attack(
				RetrieveWeaponDamage(
					pnum, 
					wepid, 
					DND_DMGID_0, 
					DND_DAMAGECATEGORY_OCCULT, 
					DND_WDMG_ISSLOT9, 
					false, 
					CheckInventory("DnD_CritToken") || GetPlayerWeaponCritState(pnum, DND_WEAPON_DEMONHEART)
				), 
				DND_DAMAGETYPE_OCCULT, 
				90 | (DND_SCANNER_HEART << 16), 
				DND_DAMAGEFLAG_DISTANCEGIVESDAMAGE | DND_DAMAGEFLAG_SELFORIGIN
			);
		break;
		case DND_WEAPON_DARKGLOVES:
			use_default = false;
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				if(isAltFire & DND_ATK_OTHER_DIR) {
					vec2[angle_vec].x = 2.0;
					vec3[offset_vec].y = 12.0;
				}
				else {
					vec2[angle_vec].x = -2.0;
					vec3[offset_vec].y = -12.0;
				}
				
				Do_Projectile_Attack_Named(owner, pnum, "DarkServantMissileSmall", wepid, 1, 32, angle_vec, offset_vec, 0, 0, 0);
			}
			else
				Do_Projectile_Attack_Named(owner, pnum, "DarkServantMissileBig", wepid, 1, 28, angle_vec, offset_vec, 0, 0, 0);
		break;
		case DND_WEAPON_HEAVYNAILGUN:
			use_default = true;
			proj_id = DND_PROJ_HEAVYNAIL;
		break;
		case DND_WEAPON_BERETTAS:
			use_default = true;
			proj_id = DND_PROJ_BERETTA;
			sp_x = 3.2;
			sp_y = 2.4;
		break;
		case DND_WEAPON_PLASMABOLTER:
			use_default = true;
			if(!(isAltFire & DND_ATK_SECONDARY))
				proj_id = DND_PROJ_PLASMABOLTER_1;
			else
				proj_id = DND_PROJ_PLASMABOLTER_2;
		break;
		case DND_WEAPON_RIPPERCANNON:
			use_default = false;
			Do_Railgun_Attack("RipperCannonShooter", 1);
		break;
	}
	
	// take away ammo if each shot isn't using ammo and we don't have the "no ammo take" flag
	if(!(flags & DND_ATF_NOAMMOTAKE) && ammo_type != "") {
		TakeInventory(ammo_type, ammo_take_amt);
		
		// ammo has been taken, handlers should check for running out of ammo here
		if(ammo_handler != "")
			GiveInventory(ammo_handler, 1);
	}
	
	// moved doesn't have artemis check to here for clarity
	if(!(flags & DND_ATF_NOAMMOGAINCHECK) && !(flags & DND_ATF_NOAMMOTAKE))
		HandleAmmoGainChance(ammo_slot, ammo_sub_slot, ammo_take_amt, owner);
		
	// NOTE: This rolls a crit chance to be used by the subsequent things below! Disabling it with the flag may remove crits!
	if(!(flags & DND_ATF_NOATTACKTRIGGER))
		HandleAttackEvent(wepid, ammo_slot == DND_AMMOSLOT_SPECIAL, ammo_sub_slot > SSAM_SLUG ? ammo_sub_slot - 1 : ammo_sub_slot);
	
	// for generic weapons that fire single shots these few lines will suffice, special spread weapons will have specifically crafted code for their attack
	if(proj_id != -1 && use_default) {
		// scale count if its a shotgun and/or we got pellets fire circle with non-zero count
		// every weapon that can fire in a circle is either a burst or a shotgun! -- this is the underlying assumption so far!
		if(count > 1 && (flags & DND_ATF_CANFIRECIRCLE))
			count = GetPelletCount(pnum, count);

		if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE) || !(flags & DND_ATF_CANFIRECIRCLE)) {
			// projectile, hitscan or melee: Check and do their corresponding attack function
			if(ProjectileInfo[proj_id].flags & DND_PROJ_HITSCAN)
				Do_Hitscan_Attack(owner, pnum, proj_id, wepid, count, ProjectileInfo[proj_id].spd_range, sp_x, sp_y, flags);
			else if(!(ProjectileInfo[proj_id].flags & DND_PROJ_MELEE))
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, count, angle_vec, offset_vec, sp_x, sp_y, flags);
			else // melee left as the sole option, not hitscan or proj
				Do_Melee_Attack(owner, pnum, wepid, count, proj_name_alt, proj_id, sp_x, sp_y, flags);
		}
		else {
			// let circle attack decide what kind of attack we need to do on its own
			Do_Attack_Circle(owner, pnum, proj_id, wepid, count, ProjectileInfo[proj_id].spd_range, flags);
		}
	}

	if(flags & DND_ATF_INSTABILITY) {
		GiveInventory("Cyborg_Instability_CD", 1);
		if(!CheckUniquePropertyOnPlayer(pnum, PUP_PELLETSFIRECIRCLE) || !(flags & DND_ATF_CANFIRECIRCLE)) {
			if(sp_x)
				sp_x = sp_x * 5 / 4;
			else
				sp_x = 3.2;

			if(sp_y)
				sp_y = sp_y * 5 / 4;
			else
				sp_y = 1.8;
			Do_Projectile_Attack(owner, pnum, DND_PROJ_INSTABILITY, wepid, count, angle_vec, offset_vec, sp_x, sp_y, flags);
		}
		else
			Do_Attack_Circle(owner, pnum, DND_PROJ_INSTABILITY, wepid, count, ProjectileInfo[DND_PROJ_INSTABILITY].spd_range, flags);
	}
	
	FreeVec2(angle_vec);
	FreeVec3(offset_vec);
	
	SetResultValue(0);
}

Script "DnD Load Weapon Information" OPEN {
	if(!isSetupComplete(SETUP_STATE1, SETUP_WEAPONDATA)) {
		SetupProjectileData();
		Delay(const:5);
		SetupWeaponData();
		Delay(const:5);
		SetupAmmoInfos();
		SetupComplete(SETUP_STATE1, SETUP_WEAPONDATA);
	}
}

Script "DnD Load Weapon Information CS" OPEN CLIENTSIDE {
	if(!isSetupComplete(SETUP_STATE1, SETUP_WEAPONDATA)) {
		// clients dont need proj data so far!
		//SetupProjectileData();
		SetupWeaponData();
		Delay(const:5);
		SetupAmmoInfos();
		SetupComplete(SETUP_STATE1, SETUP_WEAPONDATA);
	}
}

#include "DnD_WeaponScripts.h"

#endif
