#ifndef DND_VEC_IN
#define DND_VEC_IN

/*
=========================
	VECTOR DEFINITIONS
=========================
*/

typedef struct vector2 {
	int x;
	int y;
} Vec2_T;

typedef struct vector3 {
	int x;
	int y;
	int z;
} Vec3_T;

#include "DnD_Mem.h"

// Functions with C suffix mean they create a unique copy, if they dont have it they will overwrite the result into first vector
// To prevent user error, those that overwrite return nothing
void AddVec2(int v1, int v2) {
	vec2[v1].x += vec2[v2].x;
	vec2[v1].y += vec2[v2].y;
}

void SubVec2(int v1, int v2) {
	vec2[v1].x -= vec2[v2].x;
	vec2[v1].y -= vec2[v2].y;
}

void ScaleVec2(int v, int f) {
	vec2[v].x = FixedMul(vec2[v].x, f);
	vec2[v].y = FixedMul(vec2[v].y, f);
}

int AddVec2C(int v1, int v2) {
	int res = CopyVec2(v1);
	vec2[res].x += vec2[v2].x;
	vec2[res].y += vec2[v2].y;
	return res;
}

int SubVec2C(int v1, int v2) {
	int res = CopyVec2(v1);
	vec2[res].x -= vec2[v2].x;
	vec2[res].y -= vec2[v2].y;
	return res;
}

int ScaleVec2C(int v, int f) {
	int res = CopyVec2(v);
	vec2[res].x = FixedMul(vec2[res].x, f);
	vec2[res].y = FixedMul(vec2[res].y, f);
	return res;
}

int DotProductVec2(int v1, int v2) {
	int v1x_16 = vec2[v1].x >> 16;	int v2x_16 = vec2[v2].x >> 16;
	int v1y_16 = vec2[v1].y >> 16;	int v2y_16 = vec2[v2].y >> 16;
	int vdot = v1x_16 * v2x_16 + v1y_16 * v2y_16;
	
	if(abs(vdot) > FIXED_LIM)
		return vdot << 16;
	return FixedMul(vec2[v1].x, vec2[v2].x) + FixedMul(vec2[v1].y, vec2[v2].y);
}

bool IsUnitVec2(int v) {
	// check sqrt to really confirm
	//printbold(s:"unit val: ", f:abs(LengthVec2(v) - 1.0), s: " vs epsilon: ", f:F_EPSILON);
	return abs(LengthVec2(v) - 1.0) < F_EPSILON;
}

int LengthVec2(int v) {
	// we have to accept some accuracy loss, but it'll be extremely minimal (lack of fractional parts basically)
	int vx_16 = vec2[v].x >> 16;
	int vy_16 = vec2[v].y >> 16;
	int vl = vx_16 * vx_16 + vy_16 * vy_16;
	if(abs(vl) > FIXED_LIM)
		return sqrt_z(vl) << 16;
	return fsqrt(FixedMul(vec2[v].x, vec2[v].x) + FixedMul(vec2[v].y, vec2[v].y));
}

void ToUnitVec2(int v) {
	int len = LengthVec2(v);
	vec2[v].x = FixedDiv(vec2[v].x, len);
	vec2[v].y = FixedDiv(vec2[v].y, len);
}

int ToUnitVec2C(int v) {
	int res = CopyVec2(v);
	int len = LengthVec2(res);
	vec2[res].x = FixedDiv(vec2[res].x, len);
	vec2[res].y = FixedDiv(vec2[res].y, len);
	return res;
}

void ShowVec2(str prefix, int v) {
	printbold(s:prefix, s:": (", f:vec2[v].x, s: ", ", f:vec2[v].y, s:")"); 
}

void RotateVector2(int v, int angle) {
	int c = cos(angle);
	int s = sin(angle);
	
	int xtemp = vec3[v].x;
	vec2[v].x = FixedMul(c, vec2[v].x) - FixedMul(s, vec2[v].y);
	vec2[v].y = FixedMul(s, xtemp) + FixedMul(c, vec2[v].y);
}

int RotateVector2C(int v, int angle) {
	int res = CopyVec2(v);
	int c = cos(angle);
	int s = sin(angle);
	
	int xtemp = vec3[res].x;
	vec2[res].x = FixedMul(c, vec2[res].x) - FixedMul(s, vec2[res].y);
	vec2[res].y = FixedMul(s, xtemp) + FixedMul(c, vec2[res].y);
	return res;
}

// gets a vector that points to a target
int Vec2To(int target_tid) {
	int res = GetVec3();
	vec3[res].x = GetActorX(target_tid) - GetActorX(0);
	vec3[res].y = GetActorY(target_tid) - GetActorY(0);
	return res;
}

int AngleOfVector2(int v) {
	return VectorAngle(vec2[v].x, vec2[v].y);
}

int AngleBetweenVectors2(int v1, int v2) {
	int v1len = LengthVec2(v1) >> 16;
	int v2len = LengthVec2(v2) >> 16;
	
	if(!v1len)
		v1len = 1;
	if(!v2len)
		v2len = 1;
	
	return acos(DotProductVec2(v1, v2) / (v1len * v2len));
}

/*
=========================
	VECTOR3 FUNCTIONS
=========================
*/

void AssignVec3(int v, int to) {
	vec3[v].x = vec3[to].x;
	vec3[v].y = vec3[to].y;
	vec3[v].z = vec3[to].z;
}

void SetVec3XYZ(int v, int x, int y, int z) {
	vec3[v].x = x;
	vec3[v].y = y;
	vec3[v].z = z;
}

void AddVec3(int v1, int v2) {
	vec3[v1].x += vec3[v2].x;
	vec3[v1].y += vec3[v2].y;
	vec3[v1].z += vec3[v2].z;
}

void SubVec3(int v1, int v2) {
	vec3[v1].x -= vec3[v2].x;
	vec3[v1].y -= vec3[v2].y;
	vec3[v1].z -= vec3[v2].z;
}

void ScaleVec3(int v, int f) {
	vec3[v].x = FixedMul(vec3[v].x, f);
	vec3[v].y = FixedMul(vec3[v].y, f);
	vec3[v].z = FixedMul(vec3[v].z, f);
}

void ScaleVec3_Int(int v, int s) {
	vec3[v].x *= s;
	vec3[v].y *= s;
	vec3[v].z *= s;
}

int AddVec3C(int v1, int v2) {
	int res = CopyVec3(v1);
	vec3[res].x += vec3[v2].x;
	vec3[res].y += vec3[v2].y;
	vec3[res].z += vec3[v2].z;
	return res;
}

int SubVec3C(int v1, int v2) {
	int res = CopyVec3(v1);
	vec3[res].x -= vec3[v2].x;
	vec3[res].y -= vec3[v2].y;
	vec3[res].z -= vec3[v2].z;
	return res;
}

int ScaleVec3C(int v, int f) {
	int res = CopyVec2(v);
	vec3[res].x = FixedMul(vec3[res].x, f);
	vec3[res].y = FixedMul(vec3[res].y, f);
	vec3[res].z = FixedMul(vec3[res].z, f);
	return res;
}

int DotProductVec3(int v1, int v2) {
	int v1x_16 = vec3[v1].x >> 16;	int v2x_16 = vec3[v2].x >> 16;
	int v1y_16 = vec3[v1].y >> 16;	int v2y_16 = vec3[v2].y >> 16;
	int v1z_16 = vec3[v1].z >> 16;	int v2z_16 = vec3[v2].z >> 16;
	int vdot = v1x_16 * v2x_16 + v1y_16 * v2y_16 + v1z_16 * v2z_16;
	
	if(abs(vdot) > FIXED_LIM)
		return vdot << 16;
	return FixedMul(vec3[v1].x, vec3[v2].x) + FixedMul(vec3[v1].y, vec3[v2].y) + FixedMul(vec3[v1].z, vec3[v2].z);
}

bool IsUnitVec3(int v) {
	// check sqrt to really confirm
	//printbold(s:"unit val: ", f:abs(LengthVec2(v) - 1.0), s: " vs epsilon: ", f:F_EPSILON);
	return abs(LengthVec3(v) - 1.0) < F_EPSILON;
}

int LengthVec3(int v) {
	// we have to accept some accuracy loss, but it'll be extremely minimal (lack of fractional parts basically)
	int vx_16 = vec3[v].x >> 16;
	int vy_16 = vec3[v].y >> 16;
	int vz_16 = vec3[v].z >> 16;
	int vl = vx_16 * vx_16 + vy_16 * vy_16 + vz_16 * vz_16;
	if(abs(vl) > FIXED_LIM)
		return sqrt_z(vl) << 16;
	return fsqrt(FixedMul(vec3[v].x, vec3[v].x) + FixedMul(vec3[v].y, vec3[v].y) + FixedMul(vec3[v].z, vec3[v].z));
}

void ToUnitVec3(int v) {
	int len = LengthVec3(v);
	//printbold(s:"Vector Length: ", f:len);
	vec3[v].x = FixedDiv(vec3[v].x, len);
	vec3[v].y = FixedDiv(vec3[v].y, len);
	vec3[v].z = FixedDiv(vec3[v].z, len);
}

int ToUnitVec3C(int v) {
	int res = CopyVec3(v);
	int len = LengthVec3(res);
	vec3[res].x = FixedDiv(vec3[res].x, len);
	vec3[res].y = FixedDiv(vec3[res].y, len);
	vec3[res].z = FixedDiv(vec3[res].z, len);
	return res;
}

void ShowVec3(str prefix, int v) {
	printbold(s:prefix, s:": (", f:vec3[v].x, s: ", ", f:vec3[v].y, s: ", ", f:vec3[v].z, s:")"); 
}

// gets a vector that points to a given actor tid (assumes 0 as us)
int Vec3To(int target_tid) {
	int res = GetVec3(
		GetActorX(target_tid) - GetActorX(0),
		GetActorY(target_tid) - GetActorY(0),
		GetActorZ(target_tid) + GetActorProperty(target_tid, APROP_HEIGHT) / 2 - (GetActorZ(0) + GetActorViewHeight(0))
	);
	return res;
}

int Vec3To_Pos(int target_tid, int xd, int yd, int zd) {
	int res = GetVec3(
		GetActorX(target_tid) - xd,
		GetActorY(target_tid) - yd,
		GetActorZ(target_tid) + GetActorProperty(target_tid, APROP_HEIGHT) / 2 - zd
	);
	return res;
}

void RotateVector3(int v, int angle) {
	int c = cos(angle);
	int s = sin(angle);
	
	int xtemp = vec3[v].x;
	vec3[v].x = FixedMul(c, vec3[v].x) - FixedMul(s, vec3[v].y);
	vec3[v].y = FixedMul(s, xtemp) + FixedMul(c, vec3[v].y);
}

int RotateVector3C(int v, int angle) {
	int res = CopyVec3(v);
	int c = cos(angle);
	int s = sin(angle);
	
	int xtemp = vec3[res].x;
	vec3[res].x = FixedMul(c, vec3[res].x) - FixedMul(s, vec3[res].y);
	vec3[res].y = FixedMul(s, xtemp) + FixedMul(c, vec3[res].y);
	return res;
}

int AngleOfVector3(int v) {
	return VectorAngle(vec3[v].x, vec3[v].y);
}

int AngleBetweenVectors3(int v1, int v2) {
	int v1len = LengthVec3(v1) >> 16;
	int v2len = LengthVec3(v2) >> 16;
	
	if(!v1len)
		v1len = 1;
	if(!v2len)
		v2len = 1;
	
	return acos(DotProductVec3(v1, v2) / (v1len * v2len));
}

// gets a direction vector for a given actor tid
int GetDirectionVector(int tid, int a_offset = 0.0) {
	if(!tid)
		tid = ActivatorTID();

	int res = GetVec3();
	
	int a = GetActorAngle(tid) + a_offset;
	// the clamp below ensures we never get a vector perpendicular to our up vector, its important otherwise projectiles mess up
	int p = Clamp_Between(GetActorPitch(tid), -0.248, 0.248);
	int cosp = cos(p);
	
	vec3[res].x = FixedMul(cos(a), cosp);
	vec3[res].y = FixedMul(sin(a), cosp);
	vec3[res].z = -sin(p);
	
	// this is already a unit vector
	// ToUnitVec3(res);

	return res;
}

int GetRightVector(int forward, int up) {
	int res = CrossVector3C(forward, up);
	ToUnitVec3(res);
	return res;
}

int GetUpVector(int forward) {
	// we need to assure we never get the same vector here somehow as this up vector... things break otherwise!
	int temp = GetVec3(0, 0, 1.0);
	int res = CrossVector3C(forward, temp);
	CrossVector3(res, forward);
	ToUnitVec3(res);
	FreeVec3(temp);
	
	return res;
}

// Modifies v1
void CrossVector3(int v1, int v2) {
	int temp = CopyVec3(v1);
	
	vec3[v1].x = FixedMul(vec3[temp].y, vec3[v2].z) - FixedMul(vec3[temp].z, vec3[v2].y);
	vec3[v1].y = FixedMul(vec3[temp].z, vec3[v2].x) - FixedMul(vec3[temp].x, vec3[v2].z);
	vec3[v1].z = FixedMul(vec3[temp].x, vec3[v2].y) - FixedMul(vec3[temp].y, vec3[v2].x);

	FreeVec3(temp);
}

int CrossVector3C(int v1, int v2) {
	int res = GetVec3();
	
	vec3[res].x = FixedMul(vec3[v1].y, vec3[v2].z) - FixedMul(vec3[v1].z, vec3[v2].y);
	vec3[res].y = FixedMul(vec3[v1].z, vec3[v2].x) - FixedMul(vec3[v1].x, vec3[v2].z);
	vec3[res].z = FixedMul(vec3[v1].x, vec3[v2].y) - FixedMul(vec3[v1].y, vec3[v2].x);
	
	return res;
}

int GetRandomSphericalVector() {
    int x, y, z, d;
	
	do {
		x = random(0, 1.0) * 2 - 1.0;
		y = random(0, 1.0) * 2 - 1.0;
		z = random(0, 1.0) * 2 - 1.0;
		d = FixedMul(x, x) + FixedMul(y, y) + FixedMul(z, z);
	} while(d > 1.0);
	
	int res = GetVec3(x, y, z);
	return res;
}

int RandomUnitVec3() {
	int res = GetVec3(random(-1.0, 1.0), random(-1.0, 1.0), random(-1.0, 1.0));
	ToUnitVec3(res);
	return res;
}

int GetRandomSpreadDir(int vDir, int vUp, int vRight, int speed, int angle_offset, int pitch_offset) {
	int w = FixedMul(random(-1.0, 1.0), angle_offset);
	int h = FixedMul(random(-1.0, 1.0), pitch_offset);
	
	int vDir_temp = CopyVec3(vDir);
	int vUp_temp = CopyVec3(vUp);
	int vRight_temp = CopyVec3(vRight);
	
	ScaleVec3(vDir_temp, speed);
	ScaleVec3(vUp_temp, h);
	ScaleVec3(vRight_temp, w);
	AddVec3(vDir_temp, vUp_temp);
	AddVec3(vDir_temp, vRight_temp);
	
	FreeVec3(vUp_temp);
	FreeVec3(vRight_temp);
	
	return vDir_temp;
}

void StartLookDist(int part, int maxdist = 2048.0) {
	// tid parameter doesnt work on puffs for some fucking reason (zandro old)
	LineAttack(0, GetActorAngle(0), GetActorPitch(0), 0, "DistanceHelper", "None", maxdist, FHF_NORANDOMPUFFZ | FHF_NOIMPACTDECAL);
}

bool isMoving() {
	return GetActorVelX(0) != 0.0 || GetActorVelY(0) != 0.0;
}

bool isPressingMovementKeys() {
	int bt = GetPlayerInput(-1, INPUT_BUTTONS);
	return (bt & (BT_MOVELEFT | BT_MOVERIGHT | BT_FORWARD | BT_BACK));
}

int GetLateralSpeed() {
	int vx = GetActorVelX(0);
	int vy = GetActorVelY(0);
	return FixedMul(vx, vx) + FixedMul(vy, vy);
}

// Modifies input vector
void Vel3DFromAnglePitch(int v, int angle, int pitch, int speed = 1) {
	int cosp = cos(pitch);
	vec3[v].x = speed * FixedMul(cosp, cos(angle));
	vec3[v].y = speed * FixedMul(cosp, sin(angle));
	vec3[v].z = speed * sin(-pitch);
}

// basically does v1 + (v2 - v1) * a
int lerpVec2(int v1, int v2, int a) {
	int res = CopyVec2(v1);
	int sub = SubVec2C(v2, v1);
	
	ScaleVec2(sub, a);
	
	AddVec2(res, sub);
	
	FreeVec2(sub);
	
	return res;
}

// basically does v1 + (v2 - v1) * a
int lerpVec3(int v1, int v2, int a) {
	int res = CopyVec3(v1);
	int sub = SubVec3C(v2, v1);
	
	ScaleVec3(sub, a);
	
	AddVec3(res, sub);
	
	FreeVec3(sub);
	
	return res;
}

// returns angle and pitch to match uniformly distributed points in an ellipse (circle if ry is omitted)
// better than rectangular spread of decorate, but still has problem around polars collapsing
int GetEllipticSpread(int rx, int ry = 0.0) {
	int u = random(0, 1.0);
	int alpha = random(0, 1.0); // 0 - 360
	
	if(ry == 0.0)
		ry = rx;
		
	int v = GetVec2(FixedMul(FixedMul(fsqrt(u), rx), cos(alpha)), FixedMul(FixedMul(fsqrt(u), ry), sin(alpha)));
	
	return v;
}

// returns vec2 with xy and z spreads
int BulletAngle(int angle, int pitch, int rx, int ry = 0) {
	// if no spread return these as they are
	int res = GetVec2(angle, pitch);
	if(rx == 0 && ry == 0)
		return res;
	
	// rx exists but not ry, assign same val
	if(ry == 0)
		ry = rx;
		
	int v = GetVec3(0, 0, 0);
	rx = sin(rx);
	ry = sin(ry);

	while ((!vec3[v].x && !vec3[v].y && !vec3[v].z) || LengthVec3(v) > ((rx + ry) / 2)) {
		vec3[v].x = random(-rx, rx);
		vec3[v].y = random(-rx, rx);
		vec3[v].z = random(-ry, ry);
	}

	// add axis to it from the player
	vec3[v].x += FixedMul(Cos(pitch), Cos(angle));
	vec3[v].y += FixedMul(Cos(pitch), Sin(angle));
	vec3[v].z += -Sin(pitch);

	// Extract angle and pitch from trajectory
	vec2[res].x = VectorAngle(vec3[v].x, vec3[v].y);
	vec2[res].y = -asin(FixedDiv(vec3[v].z, fsqrt(FixedMul(vec3[v].x, vec3[v].x) + FixedMul(vec3[v].y, vec3[v].y) + FixedMul(vec3[v].z, vec3[v].z))));

	// free our temp vector v
	FreeVec3(v);
	
	return res;
}

// returns the resulting offset vector with spread calculated to properly set velocities, one has to set angle from VectorAngle however
int BulletAngleVec3(int angle, int pitch, int vDefaultDir, int rx, int ry = 0) {
	// if no spread return these as they are
	int res = CopyVec3(vDefaultDir);
	if(rx == 0 && ry == 0)
		return res;
	
	// rx exists but not ry, assign same val
	if(ry == 0)
		ry = rx;
	
	rx = sin(rx);
	ry = sin(ry);

	while ((!vec3[res].x && !vec3[res].y && !vec3[res].z) || LengthVec3(res) > ((rx + ry) / 2)) {
		vec3[res].x = random(-rx, rx);
		vec3[res].y = random(-rx, rx);
		vec3[res].z = random(-ry, ry);
	}

	// add axis to it from the player
	vec3[res].x += FixedMul(Cos(pitch), Cos(angle));
	vec3[res].y += FixedMul(Cos(pitch), Sin(angle));
	vec3[res].z += -Sin(pitch);
	
	return res;
}

#endif
