#ifndef DND_MONSTERS_IN
#define DND_MONSTERS_IN

#include "DnD_Common.h"
#include "DnD_CommonStat.h"
#include "DnD_MonsterData.h"

#define AVATAR_SOUL_TID 3565
#define AVATAR_CUBE_TID 6000
#define AVATAR_SOUL_COUNT 10

#define WAIT_MAX 18 // approx 2.5 seconds of flight max for jumping monsters (or lands on object and gets stuck, this is to unstuck it)

#define ZEALOT_SHIELD_TIME 4 * 35
#define ZEALOT_SHIELD_TID 3564 

#define DARKLICH_WARP_RANDMAX 160
#define DARKLICH_WARP_VELZMAX 12.0

#define FLESHWIZARD_PERCENT 10
#define FLESHWIZARD_TETHERRANGE_MIN 56.0
#define FLESHWIZARD_TETHERRANGE_MAX 256.0
#define FLESHWIZARD_TETHERRANGE_RADIUS 64.0
#define FLESHWIZARD_DAMAGE_PERIOD 17

#define BLOODSEEKER_BASESIZE 1.0
#define BLOODSEEKERSIZE_INC 0.25
#define BLOODSEEKERSIZE_TIME 24
#define BLOODSEEKER_DISTANCE 1576.0

#define WARMASTER_DIST 288.0

#define DRAUGR_TEMP_FX 700
#define DRAUGR_FX_COUNT 16
#define DRAUGR_R 96
#define DRAUGR_VEL_BASE 8

#define DND_GOLGOTH_YOFF1 43.7
#define DND_GOLGOTH_YOFF2 39.1
#define DND_GOLGOTH_ZOFF1 94.3
#define DND_GOLGOTH_ZOFF2 69.0
#define GOLGOTH_LASER_DENSITY 6
#define GOLGOTH_ORANGESOUL_SPEED 18.0
#define GOLGOTH_METEOR_SPEED 28.0
#define GOLGOTH_METEOR_Z1 82.8
#define GOLGOTH_METEOR_Z2 55.2
#define GOLGOTH_METEOR_OFFSET 27.6

#define DREAMER_CORPSEEXPLOSION_DIST 512.0

#define VAAJ_PULL_DIST 192.0

typedef struct {
	int time;
	int growtime;
	int size;
	int rbeg, rend;
	int gbeg, gend;
	int bbeg, bend;
} pentagramfx_T;

enum {
	DND_PENTAGRAMFX_THAMUZ
};

enum {
	DND_PENTAGRAMFX_TIME,
	DND_PENTAGRAMFX_GROWTIME,
	DND_PENTAGRAMFX_SIZE,
	DND_PENTAGRAMFX_RBEG,
	DND_PENTAGRAMFX_REND,
	DND_PENTAGRAMFX_GBEG,
	DND_PENTAGRAMFX_GEND,
	DND_PENTAGRAMFX_BBEG,
	DND_PENTAGRAMFX_BEND
};

#define MAX_PENTAGRAMFX_ELEMS DND_PENTAGRAMFX_THAMUZ + 1
pentagramfx_T MonsterPentagramFXList[MAX_PENTAGRAMFX_ELEMS] = {
	{ TICRATE * 3, TICRATE, 256, 0x13, 0x77, 0x23, 0xff, 0x0b, 0x6f }
};

int GetPentagramFXData(int mID, int what) {
	switch(what) {
		case DND_PENTAGRAMFX_TIME:
		return MonsterPentagramFXList[mID].time;
		case DND_PENTAGRAMFX_GROWTIME:
		return MonsterPentagramFXList[mID].growtime;
		case DND_PENTAGRAMFX_SIZE:
		return MonsterPentagramFXList[mID].size;
		case DND_PENTAGRAMFX_RBEG:
		return MonsterPentagramFXList[mID].rbeg;
		case DND_PENTAGRAMFX_REND:
		return MonsterPentagramFXList[mID].rend;
		case DND_PENTAGRAMFX_GBEG:
		return MonsterPentagramFXList[mID].gbeg;
		case DND_PENTAGRAMFX_GEND:
		return MonsterPentagramFXList[mID].gend;
		case DND_PENTAGRAMFX_BBEG:
		return MonsterPentagramFXList[mID].bbeg;
		case DND_PENTAGRAMFX_BEND:
		return MonsterPentagramFXList[mID].bend;
	}
	return 0;
}

enum {
	DND_REVENANT_COLLIDER
};

int DP_AnglesGeneric[5] = { 
	0.00, 
	0.18, 
	0.39, 
	0.58, 
	0.80
};

int GetHealthMaxActor(int t) {
	int caller = ActivatorTID();
	SetActivator(t);
	int cap = GetSpawnHealth();
	SetActivator(caller);
    return cap;
}

#define MAX_DND_COLLIDERS 1
str Colliders[MAX_DND_COLLIDERS] = {
	"GodSlayerCollider"
};

void SetXYSpeed(int speed) {
	SetActorVelocity(0, speed * cos(GetActorAngle(0)), speed * sin(GetActorAngle(0)), GetActorVelZ(0), 0, 0);
}

void AddXYSpeed(int speed) {
	SetActorVelocity(0, speed * cos(GetActorAngle(0)), speed * sin(GetActorAngle(0)), 0, 1, 0);
}

void TryChangeDirection() {
	int i, j;
	for(i = random(1, 7), j = 0; j < 8; ++i, ++j)
		if(DoCollisionCheck(0, 72 * cos(0.125 * (i % 8)), 72 * sin(0.125 * (i % 8)), 0.0)) {
			break;
	}
	i = 0.125 * (i % 8) + random(-0.025, 0.025);
	SetActorAngle(0, GetActorAngle(0) + i);
}

int DoCollisionCheck(int collider, int xoff, int yoff, int zoff) {
	return Spawn(Colliders[collider], GetActorX(0) + FixedMul(xoff, cos(GetActorAngle(0))), GetActorY(0) + FixedMul(yoff, sin(GetActorAngle(0))), GetActorZ(0) + zoff);
}

// The following 3 are by ZZYZX
void DrawPentagramPart(int fx, int fy, int fz, int size, int alpha, int ang1, int ang2, int fxID) {
    int x1 = fx + size * cos(ang1);
    int y1 = fy + size * sin(ang1);
    int x2 = fx + size * cos(ang2);
    int y2 = fy + size * sin(ang2);
    int len = VectorLength(x2 - x1, y2 - y1);
    int dx = FixedDiv(x2 - x1, len);
    int dy = FixedDiv(y2 - y1, len);
	int r1 = GetPentagramFXData(fxID, DND_PENTAGRAMFX_RBEG);
	int g1 = GetPentagramFXData(fxID, DND_PENTAGRAMFX_GBEG);
	int b1 = GetPentagramFXData(fxID, DND_PENTAGRAMFX_BBEG);
	int r2 = GetPentagramFXData(fxID, DND_PENTAGRAMFX_REND);
	int g2 = GetPentagramFXData(fxID, DND_PENTAGRAMFX_GEND);
	int b2 = GetPentagramFXData(fxID, DND_PENTAGRAMFX_BEND);
	int partsize = 5;
    int cnt = len >> 16;
    for (int i = 0; i < cnt; i += 2) {
		int dstmax = cnt / 2;
		int dstc = Abs(dstmax - i);
		int rdstc = dstmax - dstc;
		int r = r1 * dstc / dstmax + r2 * rdstc / dstmax;
		int g = g1 * dstc / dstmax + g2 * rdstc / dstmax;
		int b = b1 * dstc / dstmax + b2 * rdstc / dstmax;
		// this script is CLIENTSIDE. means we can do this.
	    // I wonder what happens if you have vid_renderer at improper values in ZDoom...
		// well, anyway, this is because particles arent displayed the same way in software and OpenGL.
		if (!GetCVar("vid_renderer"))
			partsize = 10;
        SpawnParticle((r << 16) | (g << 8) | b, true, 1, partsize, x1 + i * dx, y1 + i * dy, fz, 0, 0, 0, 0, 0, 0, alpha * rdstc / (dstmax / 2), 0);
    }
}

void DrawPentagram(int fx, int fy, int fz, int size, int alpha, int angle, int fxID) {
    DrawPentagramPart(fx, fy, fz, size, alpha, DP_AnglesGeneric[0] + angle, DP_AnglesGeneric[2] + angle, fxID);
    DrawPentagramPart(fx, fy, fz, size, alpha, DP_AnglesGeneric[2] + angle, DP_AnglesGeneric[4] + angle, fxID);
    DrawPentagramPart(fx, fy, fz, size, alpha, DP_AnglesGeneric[4] + angle, DP_AnglesGeneric[1] + angle, fxID);
    DrawPentagramPart(fx, fy, fz, size, alpha, DP_AnglesGeneric[1] + angle, DP_AnglesGeneric[3] + angle, fxID);
    DrawPentagramPart(fx, fy, fz, size, alpha, DP_AnglesGeneric[3] + angle, DP_AnglesGeneric[0] + angle, fxID);
}

void GolgothLaserTrail(int yoff, int zoff, int target, bool isFake) {
	int oX = GetActorX(0);
	int oY = GetActorY(0);
	int oZ = GetActorZ(0) + 16.0;

    int grX; int grY;  int grZ;
    int vX;  int vY;   int vZ;
    int nX;  int nY;   int nZ;
    int bX;  int bY;   int bZ;
    int magnitude;
    int pointCount;
    int pointOffset;
    grX = GetActorX(target);
    grY = GetActorY(target);
    grZ = GetActorZ(target) + zoff;
	
    vX   = grX - oX;
	vY   = grY - oY;
	vZ   = grZ - oZ;
	
	// get vectorangle for the facing, we need this for relative
	grX = GetActorAngle(target);
	//vX += FixedMul(yoff, cos(grX));
	vX += FixedMul(yoff, sin(grX));
	vY -= FixedMul(yoff, cos(grX));
	
    magnitude = magnitudeThree(vX >> 16, vY >> 16, vZ >> 16);
    pointCount  = magnitude / GOLGOTH_LASER_DENSITY;
    pointOffset = magnitude - (pointCount * GOLGOTH_LASER_DENSITY);
	
    if (magnitude != 0) {
        nX = vX / magnitude; nY = vY / magnitude; nZ = vZ / magnitude;

        int i; int j;
        for (i = 1; i < pointCount; i++) {
            j = (i * GOLGOTH_LASER_DENSITY) + pointOffset;
            bX = (nX * j) + oX + FixedMul(random(-6.0, 6.0), sin(grX));
            bY = (nY * j) + oY + FixedMul(random(-6.0, 6.0), cos(grX));
            bZ = (nZ * j) + oZ + random(-6.0, 6.0);

			if(isFake)
				Spawn("GolgothRailFX_Tracer", bX, bY, bZ);
			else
				Spawn("GolgothRailFX", bX, bY, bZ);
        }
    }
}

#endif