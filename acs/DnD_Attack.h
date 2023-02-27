#ifndef DND_ATK_IN
#define DND_ATK_IN

#include "DnD_Vec.h"
#include "DnD_Skills.h"

// attack control flags that are added on top of pre-defined constants for each weapon's projectile here
#define DND_ATK_PRIMARY 0
#define DND_ATK_SECONDARY 1
#define DND_ATK_OTHER_DIR 4

#define ACCURACY_FACTOR 0.00001
enum {
	DND_ATF_NOAMMOTAKE 				= 0b1,					// does not take ammo
	DND_ATF_NOATTACKTRIGGER 		= 0b10,					// does not run attack check code ---- use for things that make multi proj attacks that are different proj
	DND_ATF_NOAMMOGAINCHECK 		= 0b100,				// broader variant of the above, skips ammo gain check (probably handled outside etc. for multi ammo taking weapons)
	DND_ATF_CANFIRECIRCLE 			= 0b1000,				// attack can be fired around a circle
	DND_ATF_USEGRAVITY 				= 0b10000,
};

enum {
	DND_MF_MAKETARGET				= 0b1,					// sets target pointer instead of master
};

// This function will create a projectile with given angles, pitch, direction vector, speed, xy dist and zdist
void CreateProjectile(int owner, int p_helper_tid, str projectile, int angle, int pitch, int velocity, int vPos, bool useGravity = 0) {
	// this is the actor that is responsible for firing the projectile because moving the player itself to the position temporarily jitters them... ty zandro you are really good
	SpawnForced(
		"ProjectileHelper",
		vec3[vPos].x,
		vec3[vPos].y,
		vec3[vPos].z,
		p_helper_tid
	);
	
	SetActivator(p_helper_tid);
	
	// make the proj itself
	SpawnProjectile(0, projectile, angle, 0, 0, useGravity, TEMPORARY_ATTACK_TID);
	
	// clear used tid
	Thing_ChangeTID(p_helper_tid, 0);

	// manipulate newly spawned projectiles
	SetActorAngle(TEMPORARY_ATTACK_TID, angle);
	SetActorPitch(TEMPORARY_ATTACK_TID, pitch);
	SetActorVelocity(TEMPORARY_ATTACK_TID, vec3[velocity].x, vec3[velocity].y, vec3[velocity].z, 0, 0);
	
	SetActorProperty(TEMPORARY_ATTACK_TID, APROP_TARGETTID, owner);
	
	// remove NOGRAVITY
	if(useGravity)
		GiveActorInventory(TEMPORARY_ATTACK_TID, "TakeFlight", 1);
	
	SetActivator(TEMPORARY_ATTACK_TID);

	// this is the line that makes the owner the true owner
	SetPointer(AAPTR_TARGET, owner);
	
	Thing_ChangeTID(TEMPORARY_ATTACK_TID, 0);
	
	// return script ownership back to owner
	SetActivator(owner);
}

// same with projectile above but fires a hitscan attack instead
void CreateHitscan(int owner, int p_helper_tid, str projectile, int angle, int pitch, int range, int vPos) {
	// create a helper actor to manipulate spawn pos that will act on our behalf
	SpawnForced(
		"ProjectileHelper",
		vec3[vPos].x,
		vec3[vPos].y,
		vec3[vPos].z,
		p_helper_tid
	);

	SetActivator(p_helper_tid);
	SetActorProperty(p_helper_tid, APROP_TARGETTID, owner);
	SetPointer(AAPTR_TARGET, owner);
	
	// make the proj itself
	LineAttack(
		owner,
		angle,
		pitch,
		0,
		projectile,
		"None",
		range,
		FHF_NORANDOMPUFFZ
	);
	
	// clear used tid
	Thing_ChangeTID(p_helper_tid, 0);
	
	// return script ownership back to owner
	SetActivator(owner);
}

// start_vels and vPos are vectors
void CreateMinion(int owner, str actor, int start_vels, int vPos, int unique_tid = 0, int flags = 0) {
	if(!unique_tid)
		unique_tid = TEMPORARY_ATTACK_TID;

	int spawned = Spawn(
		actor,
		vec3[vPos].x,
		vec3[vPos].y,
		vec3[vPos].z,
		unique_tid
	);

	if(spawned) {
		SetActivator(unique_tid);
		
		if(flags & DND_MF_MAKETARGET) {
			SetActorProperty(unique_tid, APROP_TARGETTID, owner);
			SetPointer(AAPTR_TARGET, owner);
		}
		else {
			SetActorProperty(unique_tid, APROP_MASTERTID, owner);
			SetPointer(AAPTR_MASTER, owner);
		}
		
		SetActorVelocity(unique_tid, vec3[start_vels].x, vec3[start_vels].y, vec3[start_vels].z, 0, 0);
		SetActorAngle(unique_tid, VectorAngle(vec3[start_vels].x, vec3[start_vels].y));
		
		// clear used tid
		Thing_ChangeTID(unique_tid, 0);
		
		// return script ownership back to owner
		SetActivator(owner);
	}
}

void Do_Attack_Circle(int owner, int pnum, int proj_id, int wepid, int count, int spd, int flags) {
	// ghost check
	int a = GetActorAngle(owner);
	int p = Clamp_Between(GetActorPitch(0), -0.248, 0.248);
	int p_helper_tid = PROJECTILE_HELPER_TID + PlayerNumber();
	
	bool useGravity = false;
	
	// ghost power check
	bool makeGhostHitter = HasWeaponPower(PlayerNumber(), wepid, WEP_POWER_GHOSTHIT) && (ProjectileInfo[proj_id].flags & DND_PROJ_HASGHOSTHITTER);
	str proj_name = ProjectileInfo[proj_id].name;
	if(makeGhostHitter)
		proj_name = StrParam(s:proj_name, s:"_GhostHitter");
		
	// vector of owner pos
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + 36.0);

	int i, proj_ang;
	if(!(ProjectileInfo[proj_id].flags & DND_PROJ_HITSCAN)) {
		int cosp = cos(pnum);
		int vProj = GetVec3();
		
		for(i = 0; i < count; ++i) {
			proj_ang = (a + (1.0 / count) * i) % 1.0;
			
			// these make our direction vector
			vec3[vProj].x = FixedMul(cos(proj_ang), cosp);
			vec3[vProj].y = FixedMul(sin(proj_ang), cosp);
			vec3[vProj].z = -sin(pnum);
			
			CreateProjectile(owner, p_helper_tid, proj_name, proj_ang, p, vProj, vPos, useGravity);
		}
	}
	else {
		// hitscan version of the above -- spd becomes range for us in this case
		// since we take speed as an integer here (we discard upper 16 for special flag) we must turn it into fixed
		if(!spd)
			spd = 2048.0;
		else
			spd <<= 16;
		
		for(i = 0; i < count; ++i) {
			proj_ang = (a + (1.0 / count) * i) % 1.0;

			CreateHitscan(owner, p_helper_tid, proj_name, proj_ang, p, spd, vPos);
		}
	}
	
	FreeVec3(vPos);
}

// Does a hitscan attack, depending on special weapon behavior it can fire additional things --- we check for those using wepid, proj_id and flags
void Do_Hitscan_Attack(int owner, int pnum, int proj_id, int wepid, int count, int range, int spread_x, int spread_y, int flags) {
	// ghost check
	int a = GetActorAngle(owner);
	int p = GetActorPitch(owner);
	int p_helper_tid = PROJECTILE_HELPER_TID + pnum;
	
	// ghost power check
	bool makeGhostHitter = (HasWeaponPower(pnum, wepid, WEP_POWER_GHOSTHIT) || CheckActorInventory(owner, "NetherCheck")) && (ProjectileInfo[proj_id].flags & DND_PROJ_HASGHOSTHITTER);
	str proj_name = ProjectileInfo[proj_id].name;
	if(makeGhostHitter)
		proj_name = StrParam(s:proj_name, s:"_GhostHitter");
		
	// vector of owner pos
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + 36.0);
	int acc = GetActorProperty(owner, APROP_ACCURACY);
	
	int sp_x = ANG_TO_DOOM(FixedMul(spread_x, (1.0 - ACCURACY_FACTOR * acc)));
	int sp_y = ANG_TO_DOOM(FixedMul(spread_y, (1.0 - ACCURACY_FACTOR * acc)));
	
	//printbold(f:sp_x, s: " ", f:sp_y);
	
	if(ProjectileInfo[proj_id].flags & DND_PROJ_MELEEBONUSES)
		range += (range * GetPlayerMeleeRange(pnum)) / 100;
	
	for(int i = 0; i < count; ++i) {
		// use conical spread
		int vtemp = BulletAngle(a, p, sp_x, sp_y);
		
		CreateHitscan(owner, p_helper_tid, proj_name, vec2[vtemp].x, vec2[vtemp].y, range, vPos);
		
		//printbold(f:vec2[vtemp].x, s: " ", f:vec2[vtemp].y, s: " ", s:proj_name, s: " ", f:range);
		
		FreeVec2(vtemp);
	}

	FreeVec3(vPos);
}

void Do_Hitscan_Attack_Named(int owner, int pnum, str proj_name, int wepid, int count, int range, int spread_x, int spread_y, int flags) {
	// ghost check
	int a = GetActorAngle(owner);
	int p = GetActorPitch(owner);
	int p_helper_tid = PROJECTILE_HELPER_TID + pnum;
	
	// add if needs to hit ghosts later here
		
	// vector of owner pos
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + 36.0);
	int acc = GetActorProperty(owner, APROP_ACCURACY);
	
	int sp_x = ANG_TO_DOOM(FixedMul(spread_x, (1.0 - ACCURACY_FACTOR * acc)));
	int sp_y = ANG_TO_DOOM(FixedMul(spread_y, (1.0 - ACCURACY_FACTOR * acc)));
	
	for(int i = 0; i < count; ++i) {
		// use conical spread
		int vtemp = BulletAngle(a, p, sp_x, sp_y);
		
		CreateHitscan(owner, p_helper_tid, proj_name, vec2[vtemp].x, vec2[vtemp].y, range, vPos);
		
		//printbold(f:vec2[vtemp].x, s: " ", f:vec2[vtemp].y, s: " ", s:proj_name, s: " ", f:range);
		
		FreeVec2(vtemp);
	}

	FreeVec3(vPos);
}

void Do_Railgun_Attack(str rail_helper, int count) {
	// railgun attacks dont exist in ACS, so we just give a "helper inventory" to fire it... we can't adjust its angle or anything but it is what it is...
	// at least we get to decide what would be fired here, so there's that...
	for(int i = 0; i < count; ++i)
		GiveInventory(rail_helper, 1);
}

// Does a projectile attack, depending on special weapon behavior it can fire additional things --- we check for those using wepid, proj_id and flags
// angle_vec is (angle, pitch) offsets
// offset_vec is (x,y,z) offsets of the projectile to fire
void Do_Projectile_Attack(int owner, int pnum, int proj_id, int wepid, int count, int angle_vec, int offset_vec, int spread_x, int spread_y, int flags) {
	int p_helper_tid = PROJECTILE_HELPER_TID + pnum;
	
	// ghost power check
	bool makeGhostHitter = (HasWeaponPower(pnum, wepid, WEP_POWER_GHOSTHIT) || CheckActorInventory(owner, "NetherCheck")) && (ProjectileInfo[proj_id].flags & DND_PROJ_HASGHOSTHITTER);
	str proj_name = ProjectileInfo[proj_id].name;
	if(makeGhostHitter)
		proj_name = StrParam(s:proj_name, s:"_GhostHitter");
		
	// vectors of owner
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + 36.0);
	int vDir = GetDirectionVector(owner);
	int vUp = GetUpVector(vDir);
	int vRight = GetRightVector(vDir, vUp);
	
	// do angle and pitch offsets -- negative on angle to flip it to decorate direction (- is left + is right)
	int vTemp = GetVec3();
	if(vec2[angle_vec].x) {
		AssignVec3(vTemp, vRight);
		ScaleVec3(vTemp, sin(ANG_TO_DOOM(-vec2[angle_vec].x)));
		AddVec3(vDir, vTemp);
	}
	
	if(vec2[angle_vec].y) {
		AssignVec3(vTemp, vUp);
		ScaleVec3(vTemp, sin(ANG_TO_DOOM(-vec2[angle_vec].y)));
		AddVec3(vDir, vTemp);
	}
	
	// angle and pitch corrections
	int a = AngleOfVector3(vDir);
	// if there's any spread do this otherwise don't, it fucks up pinpoint projectiles
	if(spread_x || spread_y)
		RotateVector3(vDir, -a);
	int p = -VectorAngle(vec3[vDir].x, vec3[vDir].z);
	
	// offset adjustment
	if(vec3[offset_vec].x || vec3[offset_vec].y || vec3[offset_vec].z) {
		AssignVec3(vTemp, vDir);
		ScaleVec3(vTemp, vec3[offset_vec].x);
		AddVec3(vPos, vTemp);
		
		AssignVec3(vTemp, vRight);
		ScaleVec3(vTemp, vec3[offset_vec].y);
		AddVec3(vPos, vTemp);
		
		AssignVec3(vTemp, vUp);
		ScaleVec3(vTemp, vec3[offset_vec].z);
		AddVec3(vPos, vTemp);
	}
	
	// we dont use it beyond here
	FreeVec3(vTemp);
	
	int acc = GetActorProperty(owner, APROP_ACCURACY);
	int sp_x = ANG_TO_DOOM(FixedMul(spread_x, (1.0 - ACCURACY_FACTOR * acc)));
	int sp_y = ANG_TO_DOOM(FixedMul(spread_y, (1.0 - ACCURACY_FACTOR * acc)));
	
	for(int i = 0; i < count; ++i) {
		// use conical spread to obtain velocities of firing the projectile
		int vFireDir = BulletAngleVec3(a, p, vDir, sp_x, sp_y);
		
		int proj_ang_vec = GetVec2(
			VectorAngle(vec3[vFireDir].x, vec3[vFireDir].y),
			-asin(FixedDiv(vec3[vFireDir].z, fsqrt(FixedMul(vec3[vFireDir].x, vec3[vFireDir].x) + FixedMul(vec3[vFireDir].y, vec3[vFireDir].y) + FixedMul(vec3[vFireDir].z, vec3[vFireDir].z))))
		);
		
		// proper scaling now that we got our direction vector
		ScaleVec3_Int(vFireDir, ProjectileInfo[proj_id].spd_range);
		
		CreateProjectile(owner, p_helper_tid, proj_name, vec2[proj_ang_vec].x, vec2[proj_ang_vec].y, vFireDir, vPos, flags & DND_ATF_USEGRAVITY);
		
		FreeVec2(proj_ang_vec);
		
		FreeVec3(vFireDir);
	}

	FreeVec3(vPos);
	FreeVec3(vUp);
	FreeVec3(vDir);
	FreeVec3(vRight);
}

void Do_Projectile_Attack_Named(int owner, int pnum, str proj_name, int wepid, int count, int speed, int angle_vec, int offset_vec, int spread_x, int spread_y, int flags) {
	int p_helper_tid = PROJECTILE_HELPER_TID + pnum;
		
	// vectors of owner
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + 36.0);
	int vDir = GetDirectionVector(owner);
	int vUp = GetUpVector(vDir);
	int vRight = GetRightVector(vDir, vUp);
	
	// do angle and pitch offsets -- negative on angle to flip it to decorate direction (- is left + is right)
	int vTemp = GetVec3();
	if(vec2[angle_vec].x) {
		AssignVec3(vTemp, vRight);
		ScaleVec3(vTemp, sin(ANG_TO_DOOM(-vec2[angle_vec].x)));
		AddVec3(vDir, vTemp);
	}
	
	if(vec2[angle_vec].y) {
		AssignVec3(vTemp, vUp);
		ScaleVec3(vTemp, sin(ANG_TO_DOOM(-vec2[angle_vec].y)));
		AddVec3(vDir, vTemp);
	}
	
	// angle and pitch corrections
	int a = AngleOfVector3(vDir);
	// if there's any spread do this otherwise don't, it fucks up pinpoint projectiles
	if(spread_x || spread_y)
		RotateVector3(vDir, -a);
	int p = -VectorAngle(vec3[vDir].x, vec3[vDir].z);
	
	// offset adjustment
	if(vec3[offset_vec].x || vec3[offset_vec].y || vec3[offset_vec].z) {
		AssignVec3(vTemp, vDir);
		ScaleVec3(vTemp, vec3[offset_vec].x);
		AddVec3(vPos, vTemp);
		
		AssignVec3(vTemp, vRight);
		ScaleVec3(vTemp, vec3[offset_vec].y);
		AddVec3(vPos, vTemp);
		
		AssignVec3(vTemp, vUp);
		ScaleVec3(vTemp, vec3[offset_vec].z);
		AddVec3(vPos, vTemp);
	}
	
	// we dont use it beyond here
	FreeVec3(vTemp);
	
	int acc = GetActorProperty(owner, APROP_ACCURACY);
	int sp_x = ANG_TO_DOOM(FixedMul(spread_x, (1.0 - ACCURACY_FACTOR * acc)));
	int sp_y = ANG_TO_DOOM(FixedMul(spread_y, (1.0 - ACCURACY_FACTOR * acc)));
	
	for(int i = 0; i < count; ++i) {
		// use conical spread to obtain velocities of firing the projectile
		int vFireDir = BulletAngleVec3(a, p, vDir, sp_x, sp_y);
		
		int proj_ang_vec = GetVec2(
			VectorAngle(vec3[vFireDir].x, vec3[vFireDir].y),
			-asin(FixedDiv(vec3[vFireDir].z, fsqrt(FixedMul(vec3[vFireDir].x, vec3[vFireDir].x) + FixedMul(vec3[vFireDir].y, vec3[vFireDir].y) + FixedMul(vec3[vFireDir].z, vec3[vFireDir].z))))
		);
		
		// proper scaling now that we got our direction vector
		ScaleVec3_Int(vFireDir, speed);
		
		CreateProjectile(owner, p_helper_tid, proj_name, vec2[proj_ang_vec].x, vec2[proj_ang_vec].y, vFireDir, vPos, flags & DND_ATF_USEGRAVITY);
		
		FreeVec2(proj_ang_vec);
		
		FreeVec3(vFireDir);
	}

	FreeVec3(vPos);
	FreeVec3(vUp);
	FreeVec3(vDir);
	FreeVec3(vRight);
}

// takes DND_MF_XXX not the ATF! This is a minion summon!
void Do_Minion_Summon(int owner, str actor, int offset_vec, int speed = 0, int unique_tid = 0, int flags = 0) {
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + 36.0);
	int vDir = GetDirectionVector(owner);
	int vUp = GetUpVector(vDir);
	int vRight = GetRightVector(vDir, vUp);
	
	int velocity = CopyVec3(vDir);
	ScaleVec3_Int(velocity, speed);
	
	// offset adjustment
	if(vec3[offset_vec].x || vec3[offset_vec].y || vec3[offset_vec].z) {
		ScaleVec3(vDir, vec3[offset_vec].x);
		AddVec3(vPos, vDir);
		
		ScaleVec3(vRight, vec3[offset_vec].y);
		AddVec3(vPos, vRight);

		ScaleVec3(vUp, vec3[offset_vec].z);
		AddVec3(vPos, vUp);
	}
	
	CreateMinion(owner, actor, velocity, vPos, unique_tid, flags);

	FreeVec3(vPos);
	FreeVec3(vUp);
	FreeVec3(vDir);
	FreeVec3(vRight);
	FreeVec3(velocity);
}

void DoSlotWeaponQuestChecks(int wepid) {
	int pnum = PlayerNumber();
	// Simple Minded quest check
	if(PlayerWeaponUsed[pnum] == -1)
		PlayerWeaponUsed[pnum] = wepid;
	else if(!CheckInventory("DnD_WeaponFiredOther") && PlayerWeaponUsed[pnum] != wepid)
		GiveInventory(Quest_List[QUEST_ONLYONEWEAPON].qchecker, 1);

	// only slot 2 quest check
	if(
		active_quest_id == QUEST_ONLYPISTOLWEAPONS && !CheckInventory(Quest_List[QUEST_ONLYPISTOLWEAPONS].qchecker) &&
		!IsQuestComplete(ActivatorTID(), QUEST_ONLYPISTOLWEAPONS) && !CheckInventory("H_WeaponSlot2")
	  )
		GiveInventory(Quest_List[QUEST_ONLYPISTOLWEAPONS].qchecker, 1);
	
	// only boomstick quest check
	if(
		active_quest_id == QUEST_NOSHOTGUNS && !CheckInventory(Quest_List[QUEST_NOSHOTGUNS].qchecker) && 
		!IsQuestComplete(ActivatorTID(), QUEST_NOSHOTGUNS) && IsBoomstick(wepid)
	  )
		GiveInventory(Quest_List[QUEST_NOSHOTGUNS].qchecker, 1);
	
	// no superweapon quest check
	if(
		active_quest_id == QUEST_NOSUPERWEAPONS && !CheckInventory(Quest_List[QUEST_NOSUPERWEAPONS].qchecker) &&
		!IsQuestComplete(ActivatorTID(), QUEST_NOSUPERWEAPONS) &&
		(CheckInventory("H_WeaponSlot7") || CheckInventory("H_WeaponSlot8"))
	  )
		GiveInventory(Quest_List[QUEST_NOSUPERWEAPONS].qchecker, 1);
		
	// only energy quest check
	if(
		active_quest_id == QUEST_ONLYENERGY && !CheckInventory("DnD_UsingEnergy") && !CheckInventory(Quest_List[QUEST_ONLYENERGY].qchecker)
	  )
		GiveInventory(Quest_List[QUEST_ONLYENERGY].qchecker, 1);
}

void HandleAttackEvent(int isSpecial, int extra) {
	int pnum = PlayerNumber();
	// elemental bulwark check
	if(GetPlayerAttributeValue(pnum, INV_EX_CHANCE_CASTELEMSPELLONATK) >= random(1, 100) && !CheckInventory("RandomElementalSpellCooldown")) {
		GiveInventory("RandomElementalSpellCooldown", 1);
		CastRandomElementalSpell();
	}
	
	// roll a crit for this attack, and every projectile etc. to come out of it will use this pre-calculated crit chance here
	int wepid = CheckInventory("DnD_WeaponID");
	
	// reset crit roll before we check for crit on this attack again
	PlayerCritState[pnum][DND_CRITSTATE_CONFIRMED][wepid] = false;
	
	CheckCritChance(wepid, isSpecial, extra);
	
	// do the quest checks for slot guns being used
	DoSlotWeaponQuestChecks(wepid);
}

#endif
