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

Vec2_T* GetVec2(int x = 0, int y = 0) {
	Vec2_T* v = bcs::malloc(SIZEOF_INT * 2);

	v.x = x;
	v.y = y;
	
	return v;
}

Vec2_T* CopyVec2(Vec2_T* from) {
	Vec2_T* res = GetVec2(from.x, from.y);
	return res;
}

Vec3_T* GetVec3(int x = 0, int y = 0, int z = 0) {
	Vec3_T* v = bcs::malloc(SIZEOF_INT * 3);

	v.x = x;
	v.y = y;
	v.z = z;
	
	return v;
}

Vec3_T* CopyVec3(Vec3_T* from) {
	Vec3_T* res = GetVec3(from.x, from.y, from.z);
	return res;
}

// Functions with C suffix mean they create a unique copy, if they dont have it they will overwrite the result into first vector
// To prevent user error, those that overwrite return nothing
void SetVec2(Vec2_T* v, int x, int y) {
	v.x = x;
	v.y = y;
}

void AddVec2(Vec2_T* v1, Vec2_T* v2) {
	v1.x += v2.x;
	v1.y += v2.y;
}

void SubVec2(Vec2_T* v1, Vec2_T* v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
}

void ScaleVec2(Vec2_T* v, int f) {
	v.x = FixedMul(v.x, f);
	v.y = FixedMul(v.y, f);
}

Vec2_T* AddVec2C(Vec2_T* v1, Vec2_T* v2) {
	Vec2_T* res = CopyVec2(v1);
	res.x += v2.x;
	res.y += v2.y;
	return res;
}

Vec2_T* SubVec2C(Vec2_T* v1, Vec2_T* v2) {
	Vec2_T* res = CopyVec2(v1);
	res.x -= v2.x;
	res.y -= v2.y;
	return res;
}

Vec2_T* ScaleVec2C(Vec2_T* v, int f) {
	Vec2_T* res = CopyVec2(v);
	res.x = FixedMul(res.x, f);
	res.y = FixedMul(res.y, f);
	return res;
}

int DotProductVec2(Vec2_T* v1, Vec2_T* v2) {
	int v1x_16 = v1.x >> 16;		int v2x_16 = v2.x >> 16;
	int v1y_16 = v1.y >> 16;		int v2y_16 = v2.y >> 16;
	int vdot = v1x_16 * v2x_16 + v1y_16 * v2y_16;
	
	if(abs(vdot) > FIXED_LIM)
		return vdot << 16;
	return FixedMul(v1.x, v2.x) + FixedMul(v1.y, v2.y);
}

bool IsUnitVec2(Vec2_T* v) {
	// check sqrt to really confirm
	//printbold(s:"unit val: ", f:abs(LengthVec2(v) - 1.0), s: " vs epsilon: ", f:F_EPSILON);
	return abs(LengthVec2(v) - 1.0) < F_EPSILON;
}

int LengthVec2(Vec2_T* v) {
	// we have to accept some accuracy loss, but it'll be extremely minimal (lack of fractional parts basically)
	int vx_16 = v.x >> 16;
	int vy_16 = v.y >> 16;
	int vl = vx_16 * vx_16 + vy_16 * vy_16;
	if(abs(vl) > FIXED_LIM)
		return sqrt_z(vl) << 16;
	return fsqrt(FixedMul(v.x, v.x) + FixedMul(v.y, v.y));
}

void ToUnitVec2(Vec2_T* v) {
	int len = LengthVec2(v);
	v.x = FixedDiv(v.x, len);
	v.y = FixedDiv(v.y, len);
}

Vec2_T* ToUnitVec2C(Vec2_T* v) {
	Vec2_T* res = CopyVec2(v);
	int len = LengthVec2(res);
	res.x = FixedDiv(res.x, len);
	res.y = FixedDiv(res.y, len);
	return res;
}

void ShowVec2(str prefix, Vec2_T* v) {
	printbold(s:prefix, s:": (", f:v.x, s: ", ", f:v.y, s:")"); 
}

void RotateVector2(Vec2_T* v, int angle) {
	int c = cos(angle);
	int s = sin(angle);
	
	int xtemp = v.x;
	v.x = FixedMul(c, v.x) - FixedMul(s, v.y);
	v.y = FixedMul(s, xtemp) + FixedMul(c, v.y);
}

Vec2_T* RotateVector2C(Vec2_T* v, int angle) {
	Vec2_T* res = CopyVec2(v);
	int c = cos(angle);
	int s = sin(angle);
	
	int xtemp = res.x;
	res.x = FixedMul(c, res.x) - FixedMul(s, res.y);
	res.y = FixedMul(s, xtemp) + FixedMul(c, res.y);
	return res;
}

// gets a vector that points to a target
Vec2_T* Vec2To(int target_tid) {
	Vec2_T* res = GetVec2();
	res.x = GetActorX(target_tid) - GetActorX(0);
	res.y = GetActorY(target_tid) - GetActorY(0);
	return res;
}

int AngleOfVector2(Vec2_T* v) {
	return VectorAngle(v.x, v.y);
}

int AngleBetweenVectors2(Vec2_T* v1, Vec2_T* v2) {
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

void AssignVec3(Vec3_T* v, Vec3_T* to) {
	v.x = to.x;
	v.y = to.y;
	v.z = to.z;
}

void SetVec3XYZ(Vec3_T* v, int x, int y, int z) {
	v.x = x;
	v.y = y;
	v.z = z;
}

void AddVec3(Vec3_T* v1, Vec3_T* v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
}

void SubVec3(Vec3_T* v1, Vec3_T* v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
}

void ScaleVec3(Vec3_T* v, int f) {
	v.x = FixedMul(v.x, f);
	v.y = FixedMul(v.y, f);
	v.z = FixedMul(v.z, f);
}

void MulVecVec3(Vec3_T* v1, Vec3_T* v2) {
	v1.x = FixedMul(v1.x, v2.x);
	v1.y = FixedMul(v1.y, v2.y);
	v1.z = FixedMul(v1.z, v2.z);
}

void ScaleVec3_Int(Vec3_T* v, int s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
}

Vec3_T* AddVec3C(Vec3_T* v1, Vec3_T* v2) {
	Vec3_T* res = CopyVec3(v1);
	res.x += v2.x;
	res.y += v2.y;
	res.z += v2.z;
	return res;
}

Vec3_T* SubVec3C(Vec3_T* v1, Vec3_T* v2) {
	Vec3_T* res = CopyVec3(v1);
	res.x -= v2.x;
	res.y -= v2.y;
	res.z -= v2.z;
	return res;
}

Vec3_T* ScaleVec3C(Vec3_T* v, int f) {
	Vec3_T* res = CopyVec3(v);
	res.x = FixedMul(res.x, f);
	res.y = FixedMul(res.y, f);
	res.z = FixedMul(res.z, f);
	return res;
}

int DotProductVec3(Vec3_T* v1, Vec3_T* v2) {
	int v1x_16 = v1.x >> 16;	int v2x_16 = v2.x >> 16;
	int v1y_16 = v1.y >> 16;	int v2y_16 = v2.y >> 16;
	int v1z_16 = v1.z >> 16;	int v2z_16 = v2.z >> 16;
	int vdot = v1x_16 * v2x_16 + v1y_16 * v2y_16 + v1z_16 * v2z_16;
	
	if(abs(vdot) > FIXED_LIM)
		return vdot << 16;
	return FixedMul(v1.x, v2.x) + FixedMul(v1.y, v2.y) + FixedMul(v1.z, v2.z);
}

bool IsUnitVec3(Vec3_T* v) {
	// check sqrt to really confirm
	//printbold(s:"unit val: ", f:abs(LengthVec2(v) - 1.0), s: " vs epsilon: ", f:F_EPSILON);
	return abs(LengthVec3(v) - 1.0) < F_EPSILON;
}

int LengthVec3(Vec3_T* v) {
	// we have to accept some accuracy loss, but it'll be extremely minimal (lack of fractional parts basically)
	int vx_16 = v.x >> 16;
	int vy_16 = v.y >> 16;
	int vz_16 = v.z >> 16;
	int vl = vx_16 * vx_16 + vy_16 * vy_16 + vz_16 * vz_16;
	if(abs(vl) > FIXED_LIM)
		return sqrt_z(vl) << 16;
	return fsqrt(FixedMul(v.x, v.x) + FixedMul(v.y, v.y) + FixedMul(v.z, v.z));
}

void ToUnitVec3(Vec3_T* v) {
	int len = LengthVec3(v);
	//printbold(s:"Vector Length: ", f:len);
	v.x = FixedDiv(v.x, len);
	v.y = FixedDiv(v.y, len);
	v.z = FixedDiv(v.z, len);
}

Vec3_T* ToUnitVec3C(Vec3_T* v) {
	Vec3_T* res = CopyVec3(v);
	int len = LengthVec3(res);
	res.x = FixedDiv(res.x, len);
	res.y = FixedDiv(res.y, len);
	res.z = FixedDiv(res.z, len);
	return res;
}

void ShowVec3(str prefix, Vec3_T* v) {
	printbold(s:prefix, s:": (", f:v.x, s: ", ", f:v.y, s: ", ", f:v.z, s:")"); 
}

// gets a vector that points to a given actor tid (assumes 0 as us)
Vec3_T* Vec3To(int target_tid) {
	Vec3_T* res = GetVec3(
		GetActorX(target_tid) - GetActorX(0),
		GetActorY(target_tid) - GetActorY(0),
		GetActorZ(target_tid) + GetActorProperty(target_tid, APROP_HEIGHT) / 2 - (GetActorZ(0) + GetActorViewHeight(0))
	);
	return res;
}

Vec3_T* Vec3To_Pos(int target_tid, int xd, int yd, int zd) {
	Vec3_T* res = GetVec3(
		GetActorX(target_tid) - xd,
		GetActorY(target_tid) - yd,
		GetActorZ(target_tid) + GetActorViewHeight(target_tid) - zd
	);
	return res;
}

void RotateVector3(Vec3_T* v, int angle) {
	int c = cos(angle);
	int s = sin(angle);
	
	int xtemp = v.x;
	v.x = FixedMul(c, v.x) - FixedMul(s, v.y);
	v.y = FixedMul(s, xtemp) + FixedMul(c, v.y);
}

Vec3_T* RotateVector3C(Vec3_T* v, int angle) {
	Vec3_T* res = CopyVec3(v);
	int c = cos(angle);
	int s = sin(angle);
	
	int xtemp = res.x;
	res.x = FixedMul(c, res.x) - FixedMul(s, res.y);
	res.y = FixedMul(s, xtemp) + FixedMul(c, res.y);
	return res;
}

int AngleOfVector3(Vec3_T* v) {
	return VectorAngle(v.x, v.y);
}

int AngleBetweenVectors3(Vec3_T* v1, Vec3_T* v2) {
	int v1len = LengthVec3(v1) >> 16;
	int v2len = LengthVec3(v2) >> 16;
	
	if(!v1len)
		v1len = 1;
	if(!v2len)
		v2len = 1;
	
	return acos(DotProductVec3(v1, v2) / (v1len * v2len));
}

// gets a direction vector for a given actor tid
Vec3_T* GetDirectionVector(int tid, int a_offset = 0.0) {
	if(!tid)
		tid = ActivatorTID();

	Vec3_T* res = GetVec3();
	
	int a = GetActorAngle(tid) + a_offset;
	// the clamp below ensures we never get a vector perpendicular to our up vector, its important otherwise projectiles mess up
	int p = Clamp_Between(GetActorPitch(tid), -0.248, 0.248);
	int cosp = cos(p);
	
	res.x = FixedMul(cos(a), cosp);
	res.y = FixedMul(sin(a), cosp);
	res.z = -sin(p);
	
	// this is already a unit vector
	// ToUnitVec3(res);

	return res;
}

void LoadDirectionVectorInto(Vec3_T* v, int tid, int a_offset = 0.0) {
	if(!tid)
		tid = ActivatorTID();
	
	int a = GetActorAngle(tid) + a_offset;
	// the clamp below ensures we never get a vector perpendicular to our up vector, its important otherwise projectiles mess up
	int p = Clamp_Between(GetActorPitch(tid), -0.248, 0.248);
	int cosp = cos(p);
	
	v.x = FixedMul(cos(a), cosp);
	v.y = FixedMul(sin(a), cosp);
	v.z = -sin(p);
}

Vec3_T* GetRightVector(Vec3_T* forward, Vec3_T* up) {
	Vec3_T* res = CrossVector3C(forward, up);
	ToUnitVec3(res);
	return res;
}

Vec3_T* GetUpVector(Vec3_T* forward) {
	// we need to assure we never get the same vector here somehow as this up vector... things break otherwise!
	Vec3_T* temp = GetVec3(0, 0, 1.0);
	Vec3_T* res = CrossVector3C(forward, temp);
	CrossVector3(res, forward);
	ToUnitVec3(res);
	bcs::free(temp);
	
	return res;
}

// Modifies v1
void CrossVector3(Vec3_T* v1, Vec3_T* v2) {
	Vec3_T* temp = CopyVec3(v1);
	
	v1.x = FixedMul(temp.y, v2.z) - FixedMul(temp.z, v2.y);
	v1.y = FixedMul(temp.z, v2.x) - FixedMul(temp.x, v2.z);
	v1.z = FixedMul(temp.x, v2.y) - FixedMul(temp.y, v2.x);

	bcs::free(temp);
}

Vec3_T* CrossVector3C(Vec3_T* v1, Vec3_T* v2) {
	Vec3_T* res = GetVec3();
	
	res.x = FixedMul(v1.y, v2.z) - FixedMul(v1.z, v2.y);
	res.y = FixedMul(v1.z, v2.x) - FixedMul(v1.x, v2.z);
	res.z = FixedMul(v1.x, v2.y) - FixedMul(v1.y, v2.x);
	
	return res;
}

Vec3_T* GetRandomSphericalVector() {
    int x, y, z, d;
	
	do {
		x = random(0, 1.0) * 2 - 1.0;
		y = random(0, 1.0) * 2 - 1.0;
		z = random(0, 1.0) * 2 - 1.0;
		d = FixedMul(x, x) + FixedMul(y, y) + FixedMul(z, z);
	} while(d > 1.0);
	
	Vec3_T* res = GetVec3(x, y, z);
	return res;
}

Vec3_T* RandomUnitVec3() {
	Vec3_T* res = GetVec3(random(-1.0, 1.0), random(-1.0, 1.0), random(-1.0, 1.0));
	ToUnitVec3(res);
	return res;
}

Vec3_T* GetRandomSpreadDir(Vec3_T* vDir, Vec3_T* vUp, Vec3_T* vRight, int speed, int angle_offset, int pitch_offset) {
	int w = FixedMul(random(-1.0, 1.0), angle_offset);
	int h = FixedMul(random(-1.0, 1.0), pitch_offset);
	
	Vec3_T* vDir_temp = CopyVec3(vDir);
	Vec3_T* vUp_temp = CopyVec3(vUp);
	Vec3_T* vRight_temp = CopyVec3(vRight);
	
	ScaleVec3(vDir_temp, speed);
	ScaleVec3(vUp_temp, h);
	ScaleVec3(vRight_temp, w);
	AddVec3(vDir_temp, vUp_temp);
	AddVec3(vDir_temp, vRight_temp);
	
	bcs::free(vUp_temp);
	bcs::free(vRight_temp);
	
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
void Vel3DFromAnglePitch(Vec3_T* v, int angle, int pitch, int speed = 1) {
	int cosp = cos(pitch);
	v.x = speed * FixedMul(cosp, cos(angle));
	v.y = speed * FixedMul(cosp, sin(angle));
	v.z = speed * sin(-pitch);
}

// basically does v1 + (v2 - v1) * a
Vec2_T* lerpVec2(Vec2_T* v1, Vec2_T* v2, int a) {
	Vec2_T* res = CopyVec2(v1);
	Vec2_T* sub = SubVec2C(v2, v1);
	
	ScaleVec2(sub, a);
	
	AddVec2(res, sub);
	
	bcs::free(sub);
	
	return res;
}

// basically does v1 + (v2 - v1) * a
Vec3_T* lerpVec3(Vec3_T* v1, Vec3_T* v2, int a) {
	Vec3_T* res = CopyVec3(v1);
	Vec3_T* sub = SubVec3C(v2, v1);
	
	ScaleVec3(sub, a);
	
	AddVec3(res, sub);
	
	bcs::free(sub);
	
	return res;
}

// returns angle and pitch to match uniformly distributed points in an ellipse (circle if ry is omitted)
// better than rectangular spread of decorate, but still has problem around polars collapsing
Vec2_T* GetEllipticSpread(int rx, int ry = 0.0) {
	int u = random(0, 1.0);
	int alpha = random(0, 1.0); // 0 - 360
	
	if(ry == 0.0)
		ry = rx;
		
	Vec2_T* v = GetVec2(FixedMul(FixedMul(fsqrt(u), rx), cos(alpha)), FixedMul(FixedMul(fsqrt(u), ry), sin(alpha)));
	
	return v;
}

// returns vec2 with xy and z spreads
Vec2_T* BulletAngle(int angle, int pitch, int rx, int ry = 0) {
	// if no spread return these as they are
	Vec2_T* res = GetVec2(angle, pitch);
	if(rx == 0 && ry == 0)
		return res;
	
	// rx exists but not ry, assign same val
	if(ry == 0)
		ry = rx;
		
	Vec3_T* v = GetVec3(0, 0, 0);
	rx = sin(rx);
	ry = sin(ry);

	while ((!v.x && !v.y && !v.z) || LengthVec3(v) > ((rx + ry) / 2)) {
		v.x = random(-rx, rx);
		v.y = random(-rx, rx);
		v.z = random(-ry, ry);
	}

	// add axis to it from the player
	v.x += FixedMul(Cos(pitch), Cos(angle));
	v.y += FixedMul(Cos(pitch), Sin(angle));
	v.z += -Sin(pitch);

	// Extract angle and pitch from trajectory
	res.x = VectorAngle(v.x, v.y);
	res.y = -asin(FixedDiv(v.z, fsqrt(FixedMul(v.x, v.x) + FixedMul(v.y, v.y) + FixedMul(v.z, v.z))));

	// free our temp vector v
	bcs::free(v);
	
	return res;
}

// returns vec2 with xy and z spreads
void SetBulletAngle(Vec2_T* res, int angle, int pitch, int rx, int ry = 0) {
	// if no spread return these as they are
	if(rx == 0 && ry == 0)
		return;
	
	// rx exists but not ry, assign same val
	if(ry == 0)
		ry = rx;
		
	Vec3_T* v = GetVec3(0, 0, 0);
	rx = sin(rx);
	ry = sin(ry);

	while ((!v.x && !v.y && !v.z) || LengthVec3(v) > ((rx + ry) / 2)) {
		v.x = random(-rx, rx);
		v.y = random(-rx, rx);
		v.z = random(-ry, ry);
	}

	// add axis to it from the player
	v.x += FixedMul(Cos(pitch), Cos(angle));
	v.y += FixedMul(Cos(pitch), Sin(angle));
	v.z += -Sin(pitch);

	// Extract angle and pitch from trajectory
	res.x = VectorAngle(v.x, v.y);
	res.y = -asin(FixedDiv(v.z, fsqrt(FixedMul(v.x, v.x) + FixedMul(v.y, v.y) + FixedMul(v.z, v.z))));

	// free our temp vector v
	bcs::free(v);
}

// returns the resulting offset vector with spread calculated to properly set velocities, one has to set angle from VectorAngle however
Vec3_T* BulletAngleVec3(int angle, int pitch, Vec3_T* vDefaultDir, int rx, int ry = 0) {
	// if no spread return these as they are
	Vec3_T* res = CopyVec3(vDefaultDir);
	if(rx == 0 && ry == 0)
		return res;
	
	// rx exists but not ry, assign same val
	if(ry == 0)
		ry = rx;
	
	rx = sin(rx);
	ry = sin(ry);

	while ((!res.x && !res.y && !res.z) || LengthVec3(res) > ((rx + ry) / 2)) {
		res.x = random(-rx, rx);
		res.y = random(-rx, rx);
		res.z = random(-ry, ry);
	}

	// add axis to it from the player
	res.x += FixedMul(Cos(pitch), Cos(angle));
	res.y += FixedMul(Cos(pitch), Sin(angle));
	res.z += -Sin(pitch);
	
	return res;
}

void SetBulletAngleVec3(Vec3_T* res, int angle, int pitch, Vec3_T* vDefaultDir, int rx, int ry = 0) {
	// if no spread return these as they are
	if(rx == 0 && ry == 0)
		return;
	
	// rx exists but not ry, assign same val
	if(ry == 0)
		ry = rx;
	
	rx = sin(rx);
	ry = sin(ry);

	while ((!res.x && !res.y && !res.z) || LengthVec3(res) > ((rx + ry) / 2)) {
		res.x = random(-rx, rx);
		res.y = random(-rx, rx);
		res.z = random(-ry, ry);
	}

	// add axis to it from the player
	res.x += FixedMul(Cos(pitch), Cos(angle));
	res.y += FixedMul(Cos(pitch), Sin(angle));
	res.z += -Sin(pitch);
}

#endif
