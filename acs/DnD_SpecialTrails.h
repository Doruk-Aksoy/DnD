#ifndef DND_SPECIALTRAILS_IN
#define DND_SPECIALTRAILS_IN

enum {
	DND_TRAILACTOR_RUBYFLAME,
	DND_TRAILACTOR_THUNDERZAP,
	DND_TRAILACTOR_OVERLOADZAP,
	DND_TRAILACTOR_CHARON1,
	DND_TRAILACTOR_CHARON2,
	DND_TRAILACTOR_CHARON3
};
#define MAXTRAILACTORS (DND_TRAILACTOR_CHARON3 + 1)

typedef struct traildef {
	str actor;
	int waittime;
	int maxdiff;
	int offset_z;
} traildef_T;

traildef_T DnD_TrailDef[MAXTRAILACTORS] = {
	{ "RubyFlameTrail", 2, 0, 0 },
	{ "SmallZapPuff", 0, 16.0, 0 },
	{ "OverloadZap", 0, 16.0, 32.0 },
	{"CharonParticle_Orange", 0, 8.0, 4.0 },
	{"CharonParticle_Blue", 0, 8.0, 4.0 },
	{"CharonParticle_Green", 0, 8.0, 4.0 }
};

#define LIGHTNING_ATTACK_POINTS 24
#define OVERLOAD_LGDISTX 8
#define OVERLOAD_LGDISTY 32
#define OVERLOAD_LGSWAY 160
#define OVERLOAD_LGSWAYF 160.0
#define OVERLOAD_LGJAGG (1.0 / OVERLOAD_LGSWAY)
#define OVERLOAD_DENSITY 4
#define OVERLOAD_TZ 36.0

#define MAX_MEM_BLOCKS 128
bool mem24_use[MAX_MEM_BLOCKS];
// if you notice any glitches with lightning based attacks, this could be it and the reference needs to be put in a struct
int mref;

void FreeMem24(int i) {
	mem24_use[i] = 0;
}

int[]& GetMem_24() {
	static int mem[MAX_MEM_BLOCKS][24];
	for(int i = 0; i < MAX_MEM_BLOCKS; ++i)
		if(!mem24_use[i]) {
			mem24_use[i] = 1;
			mref = i;
			return mem[i];
		}
	return mem[0];
}

void bubblesort(int[]& list) {
   int left = lengthof(list);
   while (left) {
      int i = 0;
      int k = left - 1;
      while (i < k) {
         if (list[i] > list[i + 1]) {
            int temp = list[i + 1];
            list[i + 1] = list[i];
            list[i] = temp;
         }
         ++i;
      }
      --left;
   }
}

void LineDraw(int actortype, int sx, int sy, int sz, int dx, int dy, int dz, int density) {
    int nX;  int nY;   int nZ;
    int bX;  int bY;   int bZ;
    int magnitude;
    int pointCount;
    int pointOffset;
    dx -= sx;
	dy -= sy;
	dz -= sz;

    magnitude = magnitudeThree(dx >> 16, dy >> 16, dz >> 16);
	if(!magnitude)
		magnitude = 1;

    pointCount  = magnitude / density;
    pointOffset = magnitude - (pointCount * density);

    if (magnitude != 0) {
        nX = dx / magnitude; nY = dy / magnitude; nZ = dz / magnitude;

        int i; int j;
        for (i = 1; i < pointCount; ++i) {
            j = (i * density) + pointOffset;

            bX = (nX * j) + sx;
            bY = (nY * j) + sy;
            bZ = (nZ * j) + sz;

			SpawnForced(DnD_TrailDef[actortype].actor, bX, bY, bZ);
        }
    }
}

Script "DnD Overload Zap FX" (int source, int dest) CLIENTSIDE {
	// first generate some points
	int i;
	auto points = GetMem_24();
	int curmref = mref;
	for(i = 0; i < LIGHTNING_ATTACK_POINTS; ++i)
		points[i] = random(0, 1.0);
	
	// sort points
	bubblesort(points);
	
	// we need to create the lightning-like fx from source to dest
	int sr = GetActorProperty(source, APROP_RADIUS);
	
	int sx = GetActorX(source) + FixedMul(sr, cos(GetActorAngle(source)));
	int dx = GetActorX(dest);
	
	int sy = GetActorY(source) + FixedMul(sr, sin(GetActorAngle(source)));
	int dy = GetActorY(dest);
	
	int sz = GetActorZ(source) + GetActorProperty(source, APROP_HEIGHT) / 2 + 16.0;
	int dz = GetActorZ(dest) + GetActorProperty(dest, APROP_HEIGHT) / 2 + 16.0;
	int target_z = dz;
	// prev point, base
	int px = sx;
	int py = sy;
	int pz = sz;
	// get dir
	int tx = dx - sx;
	int ty = dy - sy;
	int mag = magnitudeThree(tx >> 16, ty >> 16, (dz - sz) >> 16);
	
	// for each point of that we will be making the line segments
	int disp = 0, prevdisp = 0, pos = 0;
	int scale, envelope;
	for(i = 1; i < LIGHTNING_ATTACK_POINTS; ++i) {
		pos = points[i];
		// prevents sharp angles, smooths them
		scale = FixedMul(mag, OVERLOAD_LGJAGG);
		scale = FixedMul(scale, pos - points[i - 1]);
		
		envelope = (pos > 0.95) ? OVERLOAD_LGDISTY * (1.0 - pos) : 1.0;
		//envelope = (pos > 0.15 && pos < 0.85) ? OVERLOAD_LGDISTY * (1.0 - pos) : pos;
		
		disp = random(-OVERLOAD_LGSWAYF, OVERLOAD_LGSWAYF);
		disp -= FixedMul(disp - prevdisp, 1.0 - scale);
		disp = FixedMul(disp, envelope);
		// d are new points
		dx = sx + FixedMul(pos, tx);
		dy = sy + FixedMul(pos, ty);
		dz = sz + FixedMul(pos, target_z - sz + random(-OVERLOAD_TZ, OVERLOAD_TZ));
		
		// draw line from previous point to current point (px, py, pz) to (dx, dy, dz)
		LineDraw(DND_TRAILACTOR_OVERLOADZAP, px, py, pz, dx, dy, dz, OVERLOAD_DENSITY);
		
		// previous are now current
		px = dx;
		py = dy;
		pz = dz;
		prevdisp = disp;
	}
	FreeMem24(curmref);
	// last point
	dx = GetActorX(dest);
	dy = GetActorY(dest);
	LineDraw(DND_TRAILACTOR_OVERLOADZAP, px, py, pz, dx, dy, target_z, OVERLOAD_DENSITY);
}

#endif
