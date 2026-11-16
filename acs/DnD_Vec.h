#ifndef DND_VEC_IN
#define DND_VEC_IN

/*
=========================
	VECTOR HELPERS
=========================
*/

// Vectors are zt-bcc's built-in vec2 and vec3: plain values with + - * /, dot, cross, .Length(),
// .Unit(), .Angle() and .Rotate(), so nothing here allocates or needs freeing.
// DnD is not strict, so `int` is raw and `v * f` scales as fixed point; whole-number scaling
// goes through ScaleVec3_Int.

void ScaleVec3_Int(vec3& v, int s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
}

bool isMoving() {
	return GetActorVelX(0) != 0.0 || GetActorVelY(0) != 0.0;
}

// gets a vector that points to a target, from our view height to its middle
// (DnD's own: zt-bcc's built-in Vec3To(from, to) goes between the actors' origins)
vec3 Vec3To(int target_tid) {
	return (
		GetActorX(target_tid) - GetActorX(0),
		GetActorY(target_tid) - GetActorY(0),
		GetActorZ(target_tid) + GetActorProperty(target_tid, APROP_HEIGHT) / 2 - (GetActorZ(0) + GetActorViewHeight(0))
	);
}

vec3 Vec3To_Pos(int target_tid, int xd, int yd, int zd) {
	return (
		GetActorX(target_tid) - xd,
		GetActorY(target_tid) - yd,
		GetActorZ(target_tid) + GetActorViewHeight(target_tid) - zd
	);
}

vec2 Vec2To(int target_tid) {
	return (GetActorX(target_tid) - GetActorX(0), GetActorY(target_tid) - GetActorY(0));
}

// gets a direction vector for a given actor tid
vec3 GetDirectionVector(int tid, int a_offset = 0.0) {
	if(!tid)
		tid = ActivatorTID();

	int a = GetActorAngle(tid) + a_offset;
	// the clamp below ensures we never get a vector perpendicular to our up vector, its important otherwise projectiles mess up
	int p = Clamp_Between(GetActorPitch(tid), -0.248, 0.248);
	int cosp = cos(p);

	// this is already a unit vector
	return (FixedMul(cos(a), cosp), FixedMul(sin(a), cosp), -sin(p));
}

vec3 GetRightVector(const vec3& forward, const vec3& up) {
	return (forward cross up).Unit();
}

vec3 GetUpVector(const vec3& forward) {
	// we need to assure we never get the same vector here somehow as this up vector... things break otherwise!
	// forward x (0, 0, 1) is exactly (y, -x, 0), so the up axis needs no vector of its own
	return ((forward.y, -forward.x, 0) cross forward).Unit();
}

vec3 GetRandomSphericalVector() {
	int x, y, z, d;

	do {
		x = random(0, 1.0) * 2 - 1.0;
		y = random(0, 1.0) * 2 - 1.0;
		z = random(0, 1.0) * 2 - 1.0;
		d = FixedMul(x, x) + FixedMul(y, y) + FixedMul(z, z);
	} while(d > 1.0);

	return (x, y, z);
}

vec3 RandomUnitVec3() {
	vec3 res = (random(-1.0, 1.0), random(-1.0, 1.0), random(-1.0, 1.0));
	return res.Unit();
}

vec3 GetRandomSpreadDir(const vec3& vDir, const vec3& vUp, const vec3& vRight, int speed, int angle_offset, int pitch_offset) {
	int w = FixedMul(random(-1.0, 1.0), angle_offset);
	int h = FixedMul(random(-1.0, 1.0), pitch_offset);
	return vDir * speed + vUp * h + vRight * w;
}

// returns angle and pitch to match uniformly distributed points in an ellipse (circle if ry is omitted)
// better than rectangular spread of decorate, but still has problem around polars collapsing
vec2 GetEllipticSpread(int rx, int ry = 0.0) {
	int u = random(0, 1.0);
	int alpha = random(0, 1.0); // 0 - 360

	if(ry == 0.0)
		ry = rx;

	return (FixedMul(FixedMul(fsqrt(u), rx), cos(alpha)), FixedMul(FixedMul(fsqrt(u), ry), sin(alpha)));
}

// returns angle and pitch after xy and z spreads
vec2 BulletAngle(int angle, int pitch, int rx, int ry = 0) {
	// if no spread return these as they are
	if(rx == 0 && ry == 0)
		return (angle, pitch);

	// rx exists but not ry, assign same val
	if(ry == 0)
		ry = rx;

	vec3 v = (0, 0, 0);
	rx = sin(rx);
	ry = sin(ry);

	while ((!v.x && !v.y && !v.z) || v.Length() > ((rx + ry) / 2)) {
		v = (random(-rx, rx), random(-rx, rx), random(-ry, ry));
	}

	// add axis to it from the player
	v += (FixedMul(Cos(pitch), Cos(angle)), FixedMul(Cos(pitch), Sin(angle)), -Sin(pitch));

	// Extract angle and pitch from trajectory
	return (v.Angle(), -asin(FixedDiv(v.z, v.Length())));
}

// One bullet's angle and pitch after spread, read from GetBulletDir(): a map static, so a shot takes no heap block.
typedef struct {
	int x, y;
} bullet_dir_T;

bullet_dir_T module& GetBulletDir() {
	static bullet_dir_T s;
	return s;
}

void RollBulletDir(int angle, int pitch, int rx, int ry = 0) {
	bullet_dir_T module& res = GetBulletDir();
	// if no spread return these as they are
	if(rx == 0 && ry == 0) {
		res.x = angle;
		res.y = pitch;
		return;
	}

	// rx exists but not ry, assign same val
	if(ry == 0)
		ry = rx;

	vec3 v = (0, 0, 0);
	rx = sin(rx);
	ry = sin(ry);

	while ((!v.x && !v.y && !v.z) || v.Length() > ((rx + ry) / 2)) {
		v = (random(-rx, rx), random(-rx, rx), random(-ry, ry));
	}

	// add axis to it from the player
	v += (FixedMul(Cos(pitch), Cos(angle)), FixedMul(Cos(pitch), Sin(angle)), -Sin(pitch));

	// Extract angle and pitch from trajectory
	res.x = v.Angle();
	res.y = -asin(FixedDiv(v.z, v.Length()));
}

// returns the resulting offset vector with spread calculated to properly set velocities, one has to set angle from VectorAngle however
vec3 BulletAngleVec3(int angle, int pitch, const vec3& vDefaultDir, int rx, int ry = 0) {
	vec3 res = vDefaultDir;
	SetBulletAngleVec3(res, angle, pitch, vDefaultDir, rx, ry);
	return res;
}

// res keeps its value when it already lies within the spread, as the heap version did
void SetBulletAngleVec3(vec3& res, int angle, int pitch, const vec3& vDefaultDir, int rx, int ry = 0) {
	// if no spread return these as they are
	if(rx == 0 && ry == 0) {
		res = vDefaultDir;
		return;
	}

	// rx exists but not ry, assign same val
	if(ry == 0)
		ry = rx;

	rx = sin(rx);
	ry = sin(ry);

	while ((!res.x && !res.y && !res.z) || res.Length() > ((rx + ry) / 2)) {
		res = (random(-rx, rx), random(-rx, rx), random(-ry, ry));
	}

	// add axis to it from the player
	res += (FixedMul(Cos(pitch), Cos(angle)), FixedMul(Cos(pitch), Sin(angle)), -Sin(pitch));
}

#endif
