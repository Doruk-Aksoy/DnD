#ifndef DND_ATK_IN
#define DND_ATK_IN

#include "DnD_Vec.h"
#include "DnD_Skills.h"

// attack control flags that are added on top of pre-defined constants for each weapon's projectile here
#define DND_ATK_PRIMARY 1
#define DND_ATK_SECONDARY 2
#define DND_ATK_OTHER_DIR 4

#define ACCURACY_FACTOR 0.00001
enum {
	DND_ATF_NOAMMOTAKE 				= 0b1,					// does not take ammo
	DND_ATF_NOATTACKTRIGGER 		= 0b10,					// does not run attack check code ---- use for things that make multi proj attacks that are different proj
	DND_ATF_NOAMMOGAINCHECK 		= 0b100,				// broader variant of the above, skips ammo gain check (probably handled outside etc. for multi ammo taking weapons)
	DND_ATF_CANFIRECIRCLE 			= 0b1000,				// attack can be fired around a circle
	DND_ATF_USEGRAVITY 				= 0b10000,
	DND_ATF_ISHITSCAN				= 0b100000,				// treated as hitscan for named attacks
	DND_ATF_NOHELPER				= 0b1000000,			// dont create helper actor to adjust position of attacks
	DND_ATF_WIDESWING				= 0b10000000,			// melee wide swing attack


	DND_ATF_INSTABILITY				= 0b100000000000000000000000000000,
	DND_ATF_NOINSTABILITY			= 0b1000000000000000000000000000000
};

enum {
	DND_MF_MAKETARGET				= 0b1,					// sets target pointer instead of master
};

// thunderstaff info things
#define DND_THUNDERSTAFF_MAXTARGETS 5
#define DND_THUNDER_RADIUSPERCOUNT 18
#define DND_THUNDERSTAFF_BASERANGE 128
#define DND_THUNDERSTAFF_LIMIT 30
typedef struct dist_tid_pair {
	int dist;
	int tid;
} dist_tid_pair_T;
#define DND_MAXSCANTRACER 256

enum {
	DND_SCANNER_BFG,
	DND_SCANNER_BFGUPGRADED,
	DND_SCANNER_HEART,
	DND_SCANNER_BOOK
};

#define MAX_SCANNER_PARTICLES (DND_SCANNER_BOOK + 1)
str ScannerAttackParticles[MAX_SCANNER_PARTICLES] = {
	"BFGExtra2",
	"BFGExtraUpgraded",
	"HeartAttackPuff",
	"BookPuff"
};

typedef struct scan_data {
	int max_dist;
	int fov;
	int spawn_offZ;
} scan_data_T;

scan_data_T ScanAttackData[MAX_SCANNER_PARTICLES] = {
	{ 1024.0, 			0.1875, 			24.0 },
	{ 1024.0,		 	0.1875, 			24.0 },
	{ 2048.0,		 	0.16, 				32.0 },
	{ 4096.0,			0.25,				32.0 }
};

int Scan_to_WeaponID(int scan_id) {
	int ret = DND_WEAPON_BFG6000;
	switch(scan_id) {
		case DND_SCANNER_BFGUPGRADED:
			ret = DND_WEAPON_BFG32768;
		break;
		case DND_SCANNER_HEART:
			ret = DND_WEAPON_DEMONHEART;
		break;
		case DND_SCANNER_BOOK:
			ret = -1;
		break;
	}
	return ret;
}

// needed to make current reflect systems work properly
Script "DnD Readjust Speed" (int spd) {
	SetActorProperty(0, APROP_SPEED, spd << 16);
	SetResultValue(0);
}

// This function will create a projectile with given angles, pitch, direction vector, speed, xy dist and zdist
void CreateProjectile(int owner, int p_helper_tid, str projectile, int angle, int pitch, int spd, int velocity, int vPos, int flags = 0) {
	// this is the actor that is responsible for firing the projectile because moving the player itself to the position temporarily jitters them... ty zandro you are really good
	int g = (flags & DND_ATF_USEGRAVITY) ? 800.0 : 0;
	
	SpawnForced(
		"ProjectileHelper",
		vec3[vPos].x,
		vec3[vPos].y,
		vec3[vPos].z,
		p_helper_tid
	);
	
	SetActivator(p_helper_tid);
	
	// make the proj itself -- needs byte angle here
	SpawnProjectile(0, projectile, angle >> 8, 0, 0, g, TEMPORARY_ATTACK_TID);
	
	// clear used tid
	Thing_ChangeTID(p_helper_tid, 0);

	// manipulate newly spawned projectiles
	SetActorAngle(TEMPORARY_ATTACK_TID, angle);
	SetActorPitch(TEMPORARY_ATTACK_TID, pitch);
	SetActorVelocity(TEMPORARY_ATTACK_TID, vec3[velocity].x, vec3[velocity].y, vec3[velocity].z, 0, 0);
	
	// remove NOGRAVITY
	if(flags & DND_ATF_USEGRAVITY)
		GiveActorInventory(TEMPORARY_ATTACK_TID, "TakeFlight", 1);
	
	SetActivator(TEMPORARY_ATTACK_TID);

	// this is the line that makes the owner the true owner
	SetPointer(AAPTR_TARGET, owner);
	SetActorProperty(0, APROP_TARGETTID, owner);

	// this is needed so that reflecting works
	SetActorProperty(0, APROP_SPEED, spd << 16);
	
	Thing_ChangeTID(TEMPORARY_ATTACK_TID, 0);
	
	// return script ownership back to owner
	SetActivator(owner);
}

// same with projectile above but fires a hitscan attack instead
void CreateHitscan(int owner, int p_helper_tid, str projectile, int angle, int pitch, int range, int vPos, int dmg = 0) {
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
		dmg,
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

void Do_Attack_Circle(int owner, int pnum, int proj_id, int wepid, int count, int spd, int flags, int hitscan_id = -1) {
	// ghost check
	int a = GetActorAngle(owner);
	int p = Clamp_Between(GetActorPitch(owner), -0.248, 0.248);
	int p_helper_tid = PROJECTILE_HELPER_TID + pnum;
	
	// ghost power check
	bool makeGhostHitter = HasWeaponPower(pnum, wepid, WEP_POWER_GHOSTHIT) && (ProjectileInfo[proj_id].flags & DND_PROJ_HASGHOSTHITTER);
	str proj_name = ProjectileInfo[proj_id].name;
	if(makeGhostHitter)
		proj_name = StrParam(s:proj_name, s:"_GhostHitter");
		
	// vector of owner pos
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + GetActorViewHeight(owner) - 5.0);

	int i, proj_ang;
	if(!(ProjectileInfo[proj_id].flags & DND_PROJ_HITSCAN)) {
		int cosp = cos(p);
		int vProj = GetVec3();
		
		for(i = 0; i < count; ++i) {
			proj_ang = (a + (1.0 / count) * i) % 1.0;
			
			// these make our direction vector
			vec3[vProj].x = spd * FixedMul(cos(proj_ang), cosp);
			vec3[vProj].y = spd * FixedMul(sin(proj_ang), cosp);
			vec3[vProj].z = -sin(p) * spd;
			
			CreateProjectile(owner, p_helper_tid, proj_name, proj_ang, p, spd, vProj, vPos, flags);
		}
	}
	else {
		// hitscan version of the above -- spd becomes range for us in this case
		// since we take speed as an integer here (we discard upper 16 for special flag) we must turn it into fixed
		if(!spd)
			spd = 2048.0;
		else
			spd <<= 16;

		if(hitscan_id != -1)
			hitscan_id = HitscanDamageData[hitscan_id];
		
		for(i = 0; i < count; ++i) {
			proj_ang = (a + (1.0 / count) * i) % 1.0;
			CreateHitscan(owner, p_helper_tid, proj_name, proj_ang, p, spd, vPos, hitscan_id);
		}
	}
	
	FreeVec3(vPos);
}

void Do_Attack_Circle_Named(int owner, int pnum, str proj_name, int wepid, int count, int spd, int flags, int hitscan_id = -1) {
	// ghost check
	int a = GetActorAngle(owner);
	int p = Clamp_Between(GetActorPitch(owner), -0.248, 0.248);
	int p_helper_tid = PROJECTILE_HELPER_TID + pnum;
		
	// vector of owner pos
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + GetActorViewHeight(owner) - 5.0);

	int i, proj_ang;
	if(!(flags & DND_ATF_ISHITSCAN)) {
		int cosp = cos(p);
		int vProj = GetVec3();
		
		for(i = 0; i < count; ++i) {
			proj_ang = (a + (1.0 / count) * i) % 1.0;
			
			// these make our direction vector
			vec3[vProj].x = spd * FixedMul(cos(proj_ang), cosp);
			vec3[vProj].y = spd * FixedMul(sin(proj_ang), cosp);
			vec3[vProj].z = -sin(p) * spd;
			
			CreateProjectile(owner, p_helper_tid, proj_name, proj_ang, p, spd, vProj, vPos, flags);
		}
	}
	else {
		// hitscan version of the above -- spd becomes range for us in this case
		// since we take speed as an integer here (we discard upper 16 for special flag) we must turn it into fixed
		if(!spd)
			spd = 2048.0;
		else
			spd <<= 16;

		if(hitscan_id != -1)
			hitscan_id = HitscanDamageData[hitscan_id];
		
		for(i = 0; i < count; ++i) {
			proj_ang = (a + (1.0 / count) * i) % 1.0;

			CreateHitscan(owner, p_helper_tid, proj_name, proj_ang, p, spd, vPos, hitscan_id);
		}
	}
	
	FreeVec3(vPos);
}

// Does a hitscan attack, depending on special weapon behavior it can fire additional things --- we check for those using wepid, proj_id and flags
void Do_Hitscan_Attack(int owner, int pnum, int proj_id, int wepid, int count, int range, int spread_x, int spread_y, int flags, int hitscan_id = -1) {
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
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + GetActorViewHeight(owner) - 5.0);
	int acc = GetActorProperty(owner, APROP_ACCURACY);
	
	int sp_x = ANG_TO_DOOM(FixedMul(spread_x, (1.0 - ACCURACY_FACTOR * acc)));
	int sp_y = ANG_TO_DOOM(FixedMul(spread_y, (1.0 - ACCURACY_FACTOR * acc)));
	
	//printbold(f:sp_x, s: " ", f:sp_y);
	
	if(ProjectileInfo[proj_id].flags & DND_PROJ_MELEEBONUSES)
		range = GetPlayerMeleeRange(pnum, range);

	// hitscan_id now holds dmg from here on
	if(hitscan_id != -1)
		hitscan_id = HitscanDamageData[hitscan_id];
	
	for(int i = 0; i < count; ++i) {
		// use conical spread
		int vtemp = BulletAngle(a, p, sp_x, sp_y);
		
		CreateHitscan(owner, p_helper_tid, proj_name, vec2[vtemp].x, vec2[vtemp].y, range, vPos, hitscan_id);
		
		//printbold(f:vec2[vtemp].x, s: " ", f:vec2[vtemp].y, s: " ", s:proj_name, s: " ", f:range);
		
		FreeVec2(vtemp);
	}

	FreeVec3(vPos);
}

void Do_Hitscan_Attack_Named(int owner, int pnum, str proj_name, int wepid, int count, int range, int spread_x, int spread_y, int flags, int proj_id = 0, int hitscan_id = -1) {
	// ghost check
	int a = GetActorAngle(owner);
	int p = GetActorPitch(owner);
	int p_helper_tid = PROJECTILE_HELPER_TID + pnum;
	
	// ghost power check
	bool makeGhostHitter = (HasWeaponPower(pnum, wepid, WEP_POWER_GHOSTHIT) || CheckActorInventory(owner, "NetherCheck")) && (ProjectileInfo[proj_id].flags & DND_PROJ_HASGHOSTHITTER);
	if(makeGhostHitter)
		proj_name = StrParam(s:proj_name, s:"_GhostHitter");
		
	// vector of owner pos
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + GetActorViewHeight(owner) - 5.0);
	int acc = GetActorProperty(owner, APROP_ACCURACY);
	
	int sp_x = ANG_TO_DOOM(FixedMul(spread_x, (1.0 - ACCURACY_FACTOR * acc)));
	int sp_y = ANG_TO_DOOM(FixedMul(spread_y, (1.0 - ACCURACY_FACTOR * acc)));

	if(hitscan_id != -1)
		hitscan_id = HitscanDamageData[hitscan_id];
	
	for(int i = 0; i < count; ++i) {
		// use conical spread
		int vtemp = BulletAngle(a, p, sp_x, sp_y);
		
		CreateHitscan(owner, p_helper_tid, proj_name, vec2[vtemp].x, vec2[vtemp].y, range, vPos, hitscan_id);
		
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
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + GetActorViewHeight(owner) - 5.0);
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
	else
		flags |= DND_ATF_NOHELPER;
	
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
		
		CreateProjectile(owner, p_helper_tid, proj_name, vec2[proj_ang_vec].x, vec2[proj_ang_vec].y, ProjectileInfo[proj_id].spd_range, vFireDir, vPos, flags);
		
		FreeVec2(proj_ang_vec);
		
		FreeVec3(vFireDir);
	}

	FreeVec3(vPos);
	FreeVec3(vUp);
	FreeVec3(vDir);
	FreeVec3(vRight);
}

void Do_Projectile_Attack_Named(int owner, int pnum, str proj_name, int wepid, int count, int speed, int angle_vec, int offset_vec, int spread_x, int spread_y, int flags, int proj_id = 0) {
	int p_helper_tid = PROJECTILE_HELPER_TID + pnum;
	
	// ghost power check
	bool makeGhostHitter = (HasWeaponPower(pnum, wepid, WEP_POWER_GHOSTHIT) || CheckActorInventory(owner, "NetherCheck")) && (ProjectileInfo[proj_id].flags & DND_PROJ_HASGHOSTHITTER);
	if(makeGhostHitter)
		proj_name = StrParam(s:proj_name, s:"_GhostHitter");
		
	// vectors of owner
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + GetActorViewHeight(owner) - 5.0);
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
	else
		flags |= DND_ATF_NOHELPER;
	
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
		
		CreateProjectile(owner, p_helper_tid, proj_name, vec2[proj_ang_vec].x, vec2[proj_ang_vec].y, speed, vFireDir, vPos, flags);
		
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
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + GetActorViewHeight(owner) - 5.0);
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

void Do_Melee_Attack(int owner, int pnum, int wepid, int count, str proj_name, int proj_id, int angle_offset, int pitch_offset, int flags, int range_offset = 0, int hitscan_id = -1) {
	// just like before, we fire hitscans in player's facing direction +- spread depending on the frame
	int range = GetPlayerMeleeRange(pnum, ProjectileInfo[proj_id].spd_range + range_offset);
	
	// ghost check
	int a = (GetActorAngle(owner) + ANG_TO_DOOM(angle_offset)) % 1.0;
	int p = Clamp_Between(GetActorPitch(owner) + ANG_TO_DOOM(pitch_offset), -0.248, 0.248);
	int p_helper_tid = PROJECTILE_HELPER_TID + pnum;
	
	// ghost power check
	bool makeGhostHitter = (HasWeaponPower(pnum, wepid, WEP_POWER_GHOSTHIT) || CheckActorInventory(owner, "NetherCheck")) && (ProjectileInfo[proj_id].flags & DND_PROJ_HASGHOSTHITTER);
	if(makeGhostHitter)
		proj_name = StrParam(s:proj_name, s:"_GhostHitter");
		
	// vector of owner pos
	int vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + GetActorViewHeight(owner) - 9.0);
	
	if(hitscan_id != -1)
		hitscan_id = HitscanDamageData[hitscan_id];

	for(int i = 0; i < count; ++i)
		CreateHitscan(owner, p_helper_tid, proj_name, a, p, range, vPos, hitscan_id);
	
	FreeVec3(vPos);
}

void Do_Scan_Attack(int dmg, int damage_type, int tracer_count, int flags) {
	// scan field of view
	// check all monsters
	static dist_tid_pair_T tlist[MAXPLAYERS][DND_MAXSCANTRACER];
	
	int scan_id = tracer_count >> 16;
	tracer_count &= 0xFFFF;
	
	int scan_dist = ScanAttackData[scan_id].max_dist;
	int scan_fov = ScanAttackData[scan_id].fov;
	
	int dist = scan_dist, i, j, k, temp;
	
	int tcount = 0;
	int owner = (flags & DND_DAMAGEFLAG_SELFORIGIN) ? ActivatorTID() : GetActorProperty(0, APROP_TARGETTID);
	int pnum = owner - P_TIDSTART;
	
	// init list
	for(i = 0; i < DND_MAXSCANTRACER; ++i) {
		tlist[pnum][i].tid = 0;
		tlist[pnum][i].dist = scan_dist;
	}
	
	// face player to the originator temporarily to get projection to there
	int projection_angle = GetActorAngle(owner);
	// use angle of the ball
	if(!(flags & DND_DAMAGEFLAG_SELFORIGIN))
		projection_angle = GetActorAngle(0);
	
	// printbold(s:"starting scan from ", d:owner, s: " with dmg ", d:dmg, s: " scan id: ", d:scan_id);
	// pick tracer_count closest enemies
	for(int mn = 0; mn < DnD_TID_Counter[DND_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		if(IsActorAlive(i) && CheckFlag(i, "SHOOTABLE")) {
			dist = fdistance(owner, i);
			//printbold(s:"Checking ", s:GetActorClass(i));
			if(dist < scan_dist && MaxAngleDiff_Projection(owner, i, scan_fov, projection_angle) && CheckSight(owner, i, CSF_NOBLOCKALL)) {
				//printbold(s:"approved ", s:GetActorClass(i), s: " ", d:i);
				// insert sorted
				temp = tcount;
				// while our calc dist > alloc dist, keep going -- we add things to the end
				// if we come by a point where we are smaller, shift things
				for(j = 0; j < temp && dist > tlist[pnum][j].dist; ++j);

				// we know where to add, check if we must shift (if we should)
				if(j < temp) {
					// less, so that means we are in-between things
					// push everything for insertion
					// this is needed to move in 0 index shifts
					if(temp == DND_MAXSCANTRACER)
						--temp;
					
					for(k = temp; k > j; --k) {
						// slide data
						tlist[pnum][k].dist = tlist[pnum][k - 1].dist;
						tlist[pnum][k].tid = tlist[pnum][k - 1].tid;
					}
				}
				
				tlist[pnum][j].dist = dist;
				tlist[pnum][j].tid = i;
				
				if(tcount < tracer_count)
					++tcount;
			}
		}
	}
	
	if(tcount) {
		int actor_flags = ScanActorFlags();
		if(GetActorProperty(0, APROP_ACCURACY) == DND_CRIT_TOKEN)
			actor_flags |= DND_ACTORFLAG_CONFIRMEDCRIT;
		
		// damage credit
		SetActivator(owner);
		
		// re-use this as poison damage mod value if any
		int wepid = Scan_to_WeaponID(scan_id);
		
		for(i = 0, j = 0; i < DND_MAXSCANTRACER; ++i) {
			if(tlist[pnum][i].tid) {
				// distance reduces damage for scans, closer = better -- gives percentage
				// curr formula: factor = 100 * ((max_scan_dist - curr_dist) / max_scan_dist)^2 -- returns a percentage
				if(flags & DND_DAMAGEFLAG_DISTANCEGIVESDAMAGE) {
					tlist[pnum][i].dist = FixedDiv((scan_dist - tlist[pnum][i].dist), scan_dist);
					tlist[pnum][i].dist = FixedMul(tlist[pnum][i].dist, tlist[pnum][i].dist);
					tlist[pnum][i].dist *= 100;
					tlist[pnum][i].dist >>= 16;
					if(!tlist[pnum][i].dist)
						tlist[pnum][i].dist = 1;
					temp = (dmg * tlist[pnum][i].dist) / 100;
				}
				else
					temp = dmg;
				
				HandleDamageDeal(owner, tlist[pnum][i].tid, temp, damage_type, wepid, flags, GetActorX(owner), GetActorY(owner), GetActorZ(owner), actor_flags);
				SpawnForced(ScannerAttackParticles[scan_id], GetActorX(tlist[pnum][i].tid), GetActorY(tlist[pnum][i].tid), GetActorZ(tlist[pnum][i].tid) + ScanAttackData[scan_id].spawn_offZ, 0);
				
				//printbold(s:"do scan damage of ", d:temp, s: " dist: ", d:tlist[pnum][i].dist, s: " to ", s:GetActorClass(tlist[pnum][i].tid), s:" ", d:tlist[pnum][i].tid, s:" -- j = ", d:j, s: " / ", d:tcount, s: " index: ", d:i);
				// abort if we reached our tracer cap
				++j;
				if(j == tcount)
					break;
			}
		}
		//printbold(s:"complete? ", d:j, s: " vs ", d:tcount);
	}
}

void SpawnDarkLanceProjectile(int this, int a, int proj_tid, int x, int y, int z, int vx, int vy, int vz, int dist, int spd) {
	SpawnProjectile(this, "DarkLanceProjectile", a, 0, 0, 0, proj_tid);
	SetActivator(proj_tid);
	SetActorVelocity(0, vx * spd, vy * spd, vz * spd, 0, 0);
	SetActorPosition(0, x + vx * dist, y + vy * dist, z + GetActorViewHeight(this) - 9.0 + vz * dist, 0);
	SetPointer(AAPTR_TARGET, this);
	SetActorProperty(0, APROP_TARGETTID, this);
	SetActorProperty(0, APROP_SPEED, spd << 16);
	Thing_ChangeTID(proj_tid, 0);
	SetActivator(this);
}

void SpawnDarkLanceProjectile_Side(int this, int a, int proj_tid, int x, int y, int z, int vx, int vy, int vz, int xd, int yd, int zd, int spd) {
	SpawnProjectile(this, "DarkLanceProjectile", a, 0, 0, 0, proj_tid);
	SetActivator(proj_tid);
	SetActorVelocity(0, vx * spd, vy * spd, vz * spd, 0, 0);
	SetActorPosition(0, x + vx * xd + vy * yd, y + vy * xd - vx * yd, z + GetActorViewHeight(this) - 9.0 + vz * zd, 0);
	SetPointer(AAPTR_TARGET, this);
	SetActorProperty(0, APROP_TARGETTID, this);
	SetActorProperty(0, APROP_SPEED, spd << 16);
	Thing_ChangeTID(proj_tid, 0);
	SetActivator(this);
}

void Do_DarkLance_Shots(int owner, int pnum, int amt, int dist, int spd, int lvl) {
	int a = GetActorAngle(owner);
	int p = GetActorPitch(owner);
	int vx = FixedMul(cos(p), cos(a));
	int vy = FixedMul(cos(p), sin(a));
	int vz = -sin(p);
	
	int x = GetActorX(owner);
	int y = GetActorY(owner);
	int z = GetActorZ(owner);
	
	for(int i = 0; i < amt; ++i)
		SpawnDarkLanceProjectile(owner, a, DARKLANCE_TID + pnum, x, y, z, vx, vy, vz, i * dist, spd); 
	
	// side projectiles
	if(lvl > 0) {
		SpawnDarkLanceProjectile_Side(owner, a, DARKLANCE_TID + pnum, x, y, z, vx, vy, vz, 3, -16, 3, spd);
		SpawnDarkLanceProjectile_Side(owner, a, DARKLANCE_TID + pnum, x, y, z, vx, vy, vz, 11, -30, 11, spd);
		SpawnDarkLanceProjectile_Side(owner, a, DARKLANCE_TID + pnum, x, y, z, vx, vy, vz, 3, 16, 3, spd);
		SpawnDarkLanceProjectile_Side(owner, a, DARKLANCE_TID + pnum, x, y, z, vx, vy, vz, 11, 30, 11, spd);
	}
	
	// more side projectiles
	if(lvl > 1) {
		SpawnDarkLanceProjectile_Side(owner, a, DARKLANCE_TID + pnum, x, y, z, vx, vy, vz, 20, -39, 20, spd);
		SpawnDarkLanceProjectile_Side(owner, a, DARKLANCE_TID + pnum, x, y, z, vx, vy, vz, 28, -45, 28, spd);
		SpawnDarkLanceProjectile_Side(owner, a, DARKLANCE_TID + pnum, x, y, z, vx, vy, vz, 20, 39, 20, spd);
		SpawnDarkLanceProjectile_Side(owner, a, DARKLANCE_TID + pnum, x, y, z, vx, vy, vz, 28, 45, 28, spd);
	}
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

void HandleAttackEvent(int wepid, int isSpecial, int extra) {
	int pnum = PlayerNumber();
	// elemental bulwark check
	if(GetPlayerAttributeValue(pnum, INV_EX_CHANCE_CASTELEMSPELLONATK) >= random(1, 100) && !CheckInventory("RandomElementalSpellCooldown")) {
		GiveInventory("RandomElementalSpellCooldown", 1);
		CastRandomElementalSpell();
	}
	
	// reset crit roll before we check for crit on this attack again
	UnsetPlayerWeaponCritState(pnum, wepid);
	
	// do the quest checks for slot guns being used
	DoSlotWeaponQuestChecks(wepid);
}

#endif
