#ifndef DND_MATH_IN
#define DND_MATH_IN

#define FIXED_LIM 32768
#define F_EPSILON 0.0005
#define M_PI 3.14156

#define ANG_TO_DOOM(x) ((x) / 360)

#include "DnD_Vec.h"

// takes deltax and deltay as parameter for actor comparisons
int AproxDistance (int dx, int dy) {
	dx = abs(dx);
	dy = abs(dy);

	if (dx < dy)
		return dx + dy - (dx >> 1);

	return dx + dy - (dy >> 1);
}

// Copied from ZDWiki.
// Fixed-point version, only works up to 30,000 or so:
int fsqrt(int number) 
{ 
  int samples = 15; // Samples for accuracy

  if (number == 1.0) return 1.0; 
  if (number <= 0) return 0;
  int val = samples<<17 + samples<<19; // x*10 = x<<1 + x<<3
  for (int i=0; i<samples; ++i) 
    val = (val + FixedDiv(number, val)) >> 1;

  return val; 
}

int sqrt_z(int number)
{
	if(number <= 3)
	{
		if(number > 0)
		{
			return 1;
		}
		return 0;
	}
	
	int oldAns = number >> 1,                     
	    newAns = (oldAns + number / oldAns) >> 1; 
	
	while(newAns < oldAns)
	{
		oldAns = newAns;
		newAns = (oldAns + number / oldAns) >> 1;
	}

	return oldAns;
}

int Min(int x, int y) {
	if(x < y)
		return x;
	return y;
}


int Max(int x, int y) {
	if(x > y)
		return x;
	return y;
}

int magnitudeThree(int x, int y, int z)
{
    return sqrt_z(x*x + y*y + z*z);
}

int smart_mul(int x, int y) {
	if(x >= 1.0 && y >= 1.0)
		return FixedMul(x, y);
	return x * y;
}

int ftrunc(int x) {
	return ((x * 100 + 50) / 100);
}

int roundUp(int x, int mult) {
    if (!mult)
        return x;

    int remainder = x % mult;
    if (!remainder)
        return x;

    return x + mult - remainder;
}

int ConvertFixedToPrecise(int x) {
	// 1000 = 1.0, but we are talking percentages so 0.01 = 1%
	// the first >> 4 is so we get rid of unimportant last 4 bits of precision and can multiply with 1000 without worrying
	// essentially this increases maximum fixed we can clearly represent from 65.0 to 65.0 x 16 = 1024.0
	return (((x + 0.0005) >> 4) * FACTOR_FIXED_RESOLUTION) >> 12;
}

Str GetFixedRepresentation(int val, bool isPercentage) {
	val = ConvertFixedToPrecise(val);
	
	if(isPercentage)
		val *= 100;
	
	if(val > 1000) {
		if((val / 10) % 10)
			return StrParam(d:val / 1000, s:".", d:(val / 100) % 10, d:(val / 10) % 10);
		return StrParam(d:val / 1000, s:".", d:(val / 100) % 10);
	}
		
	if(val % 10)
		return StrParam(d:val / 1000, s:".", d:(val / 100) % 10, d:(val / 10) % 10, d:val % 10);
	if((val / 10) % 10)
		return StrParam(d:val / 1000, s:".", d:(val / 100) % 10, d:(val / 10) % 10);
	return StrParam(d:val / 1000, s:".", d:(val / 100) % 10);
}

int CancelMultiplicativeFactors(int f1, int f2) {
	return FixedDiv(1.0 + f1, 1.0 + f2);
}

int CombineMultiplicativeFactors(int f1, int f2) {
	return FixedMul(1.0 + f1, 1.0 + f2);
}

// converts a factor in fixed to a factor in int (equivalent) -- also used in below
int ConvertFixedFactorToInt(int factor) {
	return ((1.0 + factor) * 100) >> 16;
}

// this is a fixed value and represents a percentage, so convert it into one
int ApplyFixedFactorToInt(int val, int factor) {
	return val * (((1.0 + factor) * 100) >> 16) / 100;
}

int VectorPitch (Int t1, Int t2, int dx, int dy, int adj) {
	If(adj != 0)
		adj <<= 16;
	Return(VectorAngle(AproxDistance(dx, dy), GetActorZ(t1) - (GetActorZ(t2) - adj)));
}

/*
=====================
	INTERPOLATION
=====================
*/

int SmoothStop3(int x) {
	return 1.0 - FixedMul(1.0 - x, 1.0 - x);
}

int SmoothStart3(int x) {
	return FixedMul(x, x);
}

int SmoothSine(int x) {
	return 1.0 - sin(x);
}

int lerpDouble(int d1, int d2, int a) {
	return d1 + FixedMul(d2 - d1, a);
}

bool player_pickup_cubes_intersect(int player_tid, int object_tid) {
	//tid1 = doom player: height: 56, radius: 16.
	//tid2 = pickups: height: 16, radius: 20.

	//printbold(s:"actor_cubes_intersect: player tid: ", d:player_tid, s:", item tid: ", d:object_tid, s:", radius: ", f:radius, s:", check states: ", d:check_state_1, s:", ", d:check_state_2, s:", ", d:check_state_3_1, s:"; ", d:check_state_3_2, s:", player z:", f:getactorz(player_tid), s:", object z: ", f:getactorz(object_tid));
	
	return	((getactorx(player_tid)-16.0 <= getactorx(object_tid)+20.0) && (getactorx(player_tid)+16.0 >= getactorx(object_tid)-20.0)) &&
			((getactory(player_tid)-16.0 <= getactory(object_tid)+20.0) && (getactory(player_tid)+16.0 >= getactory(object_tid)-20.0)) &&
			((getactorz(player_tid) <= getactorz(object_tid)+16.0) && (getactorz(player_tid)+56.0 >= getactorz(object_tid)));
}

int fdistance_delta(int dx, int dy, int dz) {
	int len;
	int ang = vectorangle(dx, dy);
	if(((ang+0.125)%0.5) > 0.25) len = fixeddiv(dy, sin(ang));
	else len = fixeddiv(dx, cos(ang));

	ang = vectorangle(len, dz);
	if(((ang+0.125)%0.5) > 0.25) len = fixeddiv(dz, sin(ang));
	else len = fixeddiv(len, cos(ang));

	return len;
}

bool MaxAngleDiff (int m1, int m2, int maxdiff) {
	int x = GetActorX(m2) - GetActorX(m1);
	int y = GetActorY(m2) - GetActorY(m1);
	int cone_len = VectorLength(x, y);
	x = FixedDiv(x, cone_len);
	y = FixedDiv(y, cone_len);

	// comparison angle to our fov
	int cone_x = cos(GetActorAngle(0));
	int cone_y = sin(GetActorAngle(0));
	cone_len = VectorLength(cone_x, cone_y);
	
	int dot = FixedMul(x, FixedDiv(cone_x, cone_len)) + FixedMul(y, FixedDiv(cone_y, cone_len));
	return dot > cos(maxdiff);
}

bool MaxAngleDiff_Projection (int m1, int m2, int maxdiff, int projected_angle) {
	int x = GetActorX(m2) - GetActorX(m1);
	int y = GetActorY(m2) - GetActorY(m1);
	int cone_len = VectorLength(x, y);
	x = FixedDiv(x, cone_len);
	y = FixedDiv(y, cone_len);

	// comparison angle to our fov
	int cone_x = cos(projected_angle);
	int cone_y = sin(projected_angle);
	cone_len = VectorLength(cone_x, cone_y);
	
	int dot = FixedMul(x, FixedDiv(cone_x, cone_len)) + FixedMul(y, FixedDiv(cone_y, cone_len));
	return dot > cos(maxdiff);
}

int LinearMap(int val, int o_min, int o_max, int n_min, int n_max) {
	return (val - o_min) * (n_max - n_min) / (o_max - o_min) + n_min;
}

int LinearMap_Fixed(int val, int o_min, int o_max, int n_min, int n_max) {
	return FixedDiv(FixedMul(val - o_min, n_max - n_min), o_max - o_min) + n_min;
}

// INT_MAX has 10 digits
int PowersOf10[10] = {
	1,
	10,
	100,
	1000,
	10000,
	100000,
	1000000,
	10000000,
	100000000,
	1000000000
};

int digitcount(int x) {
	int res = 1;
	while((x /= 10))
		++res;
	return res;
}

int fdistance (int tid1, int tid2) {
	int len;
	int y = getactory(tid1) - getactory(tid2);
	int x = getactorx(tid1) - getactorx(tid2);
	int z = getactorz(tid1) - getactorz(tid2);

	int ang = vectorangle(x,y);
	if(((ang+0.125)%0.5) > 0.25) len = fixeddiv(y, sin(ang));
	else len = fixeddiv(x, cos(ang));

	ang = vectorangle(len, z);
	if(((ang+0.125)%0.5) > 0.25) len = fixeddiv(z, sin(ang));
	else len = fixeddiv(len, cos(ang));

	return len;
}

int GetIntegerBits(int x) {
	return x & 0xFFFF0000;
}

int XorBit(int x, int n) {
	return x ^ (1 << n);
}

int SetBit(int x, int n) {
	return x | (1 << n);
}

int ClearBit(int x, int n) {
	return x & ~(1 << n);
}

bool IsSet(int x, int n) {
	return x & (1 << n);
}

int ResetBits(int val, int begin, int end) {
	for(int i = begin; i < end + 1; ++i)
		val &= ~(1 << i);
	return val;
}

// only positive
int getpow2 (int x) {
	int res = 0;
	while((x >>= 1))
		++res;
	return res;
}

int fpow(int x, int n) {
	int y = 1;
	while(n-- > 0)
		y = FixedMul(y, x);
	return y;
}

int pow (int x, int n) {
	int y = 1;
	while (n-- > 0) y *= x;
	return y;
}

int abs (int x) {
	if(x < 0)
		return -x;
	return x;
}

int sign (int x) {
	return (x > 0) - (x < 0);
}

int atan(int x) {
    return VectorAngle(1.0, x);
}

// consider generating a global array table of X (work out number later--guessing 65536?) elements for faster lookup of this later? It's 15 iterations per fsqrt at the very least with FixedDiv on each
// it might be useful later! If the optimization is required of course!
int asin(int x) {
	return atan(FixedDiv(x, fsqrt(1.0 - FixedMul(x, x))));
}

// 0.5 * pi = asinx + acosx
// pi is 0.5 in doom angles
int acos(int x) {
	return 0.25 - asin(x);
}

// map to -0.5 to 0.5
int normalize180(int x) {
	x = x % 1.0;
	x = (x + 1.0) % 1.0;
	if(x > 0.5)
		x -= 1.0;
	return x;
}

int normalize360(int x) {
	return (x + 1.0) % 1.0;
}

int delta_angle(int x, int y) {
	return normalize180(y - x);
}

int randsign() {
	return 1 - 2 * random(0, 1);
}

int degToFixed(int x) {
	return x / 360;
}

int degToFixed_Pitch(int x) {
	return x / 90;
}

#endif
