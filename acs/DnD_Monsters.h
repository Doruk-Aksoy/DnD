#ifndef DND_MONSTERS_IN
#define DND_MONSTERS_IN

#include "DnD_Common.h"
#include "DnD_Orbs.h"
#include "DnD_Stat.h"


#define AVATAR_SOUL_TID 2000
#define AVATAR_CUBE_TID 6000
#define AVATAR_SOUL_COUNT 10

#define WAIT_MAX 18 // approx 2.5 seconds of flight max for jumping monsters (or lands on object and gets stuck, this is to unstuck it)

#define ZEALOT_SHIELD_TIME 4 * 35
#define ZEALOT_SHIELD_TIDADD 1000 

#define DARKLICH_WARP_RANDMAX 160
#define DARKLICH_WARP_VELZMAX 12.0

#define FLESHWIZARD_PERCENT 10
#define FLESHWIZARD_TETHERRANGE_MIN 56.0
#define FLESHWIZARD_TETHERRANGE_MAX 256.0
#define FLESHWIZARD_TETHERRANGE_RADIUS 64.0
#define FLESHWIZARD_DAMAGE_PERIOD 17

#define DND_TORRASQUE_MACEID 800

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

int DP_AnglesGeneric[5] = { 
	0.00, 
	0.18, 
	0.39, 
	0.58, 
	0.80
};

int GetHealthCapActor(int t) {
	return DND_VIT_INCREASE * CheckActorInventory(t, "PSTAT_Vitality");
}

int GetHealthMaxActor(int t) {
    return 100 + GetHealthCapActor(t);
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

#endif