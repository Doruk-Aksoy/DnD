#ifndef DND_MATH_IN
#define DND_MATH_IN

#define FIXED_LIM 32768
#define F_EPSILON 0.0005

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

// Fixed point square root by the engine; 0 for anything not positive.
int fsqrt(int number) {
	if(number <= 0)
		return 0;
	return FixedSqrt(number);
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

int magnitudeTwo(int x, int y)
{
    return sqrt_z(x*x + y*y);
}

int magnitudeThree(int x, int y, int z)
{
    return sqrt_z(x*x + y*y + z*z);
}

int ftrunc(int x) {
	return ((x * 100 + 50) / 100);
}

int ConvertFixedToPrecise(int x) {
	// 1000 = 1.0, but we are talking percentages so 0.01 = 1%
	// the first >> 4 is so we get rid of unimportant last 4 bits of precision and can multiply with 1000 without worrying
	// essentially this increases maximum fixed we can clearly represent from 65.0 to 65.0 x 16 = 1024.0
	return (((x + 0.0005) >> 4) * FACTOR_FIXED_RESOLUTION) >> 12;
}

Str GetFixedRepresentation(int val, bool isPercentage) {
	str isNeg = "";
	if(val < 0) {
		isNeg = "-";
		val = -val;
	}
	
	val = ConvertFixedToPrecise(val);
	
	if(isPercentage)
		val *= 100;
	
	if(val > 1000) {
		if((val / 10) % 10)
			return StrParam(s:isNeg, d:val / 1000, s:".", d:(val / 100) % 10, d:(val / 10) % 10);
		return StrParam(s:isNeg, d:val / 1000, s:".", d:(val / 100) % 10);
	}
		
	if(val % 10)
		return StrParam(s:isNeg, d:val / 1000, s:".", d:(val / 100) % 10, d:(val / 10) % 10, d:val % 10);
	if((val / 10) % 10)
		return StrParam(s:isNeg, d:val / 1000, s:".", d:(val / 100) % 10, d:(val / 10) % 10);
	return StrParam(s:isNeg, d:val / 1000, s:".", d:(val / 100) % 10);
}

int CancelMultiplicativeFactors(int f1, int f2) {
	return FixedDiv(1.0 + f1, 1.0 + f2);
}

int CombineMultiplicativeFactors(int f1, int f2) {
	return FixedMul(1.0 + f1, 1.0 + f2);
}

int CombineFactors(int f1, int f2) {
	return FixedMul(f1, 1.0 + f2);
}

int HandleMultiplicativeFactors(int f1, int f2) {
	if(!f1) {
		// if we are zero, simply replace with val
		return f2;
	}
	else if(f2 > 0) {
		// non-zero, multiply case -- we store things like 0.2 etc. here, but while we amplify it we need to consider 1.0 + val
		return FixedMul(f1, f2);
	}
	else if(f2 < 0) {
		// if negative we divide
		// if mod value == val, this means we need to set to zero (it's removed), otherwise just divide it
		f1 = FixedMul(f1, FixedDiv(1.0, -f2));
		if((f1 > 1.0 && f1 - 1.0 < F_EPSILON) || (f1 < 1.0 && 1.0 - f1 < F_EPSILON))
			f1 = 0;
	}

	return f1;
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

// bcs::INT_MAX has 10 digits
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

int fpow(int x, int n) {
	int y = 1.0;
	while(n-- > 0)
		y = FixedMul(y, x);
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

int normalize360(int x) {
	return (x + 1.0) % 1.0;
}

// log2 of a positive int, 16.16.
int FixedLog2Int(int x) {
	if(x <= 0)
		return 0;
	int n = 0, t = x;
	while(t > 1) {
		t >>= 1;
		++n;
	}
	// the mantissa in [1, 2), then one bit of the fraction per squaring
	int m = FixedDiv(x, 1 << n), r = n << 16;
	for(int i = 15; i >= 0; --i) {
		m = FixedMul(m, m);
		if(m >= 2.0) {
			m >>= 1;
			r |= 1 << i;
		}
	}
	return r;
}

// 2 to a 16.16 power >= 0, 16.16; the fraction is a product of 2^(1/2^k) factors.
int FixedExp2(int f) {
	int ip = f >> 16, fp = f & 0xFFFF, r = 1.0, s = 2.0;
	for(int i = 15; i >= 0; --i) {
		s = fsqrt(s);
		if(fp & (1 << i))
			r = FixedMul(r, s);
	}
	return r << ip;
}

// A positive int to a 16.16 power >= 0, 16.16. Not for hot paths: sixteen square roots a call.
int IntPowFixed(int x, int e) {
	return FixedExp2(FixedMul(e, FixedLog2Int(x)));
}

#endif
