#ifndef DND_COMMON_IN
#define DND_COMMON_IN

//#define ISDEBUGBUILD

#define DND_AFTER50_INCREMENT_DAMAGE 0.33

#define MAXLEVELS 100
#define DND_MAX_MONSTERLVL MAXLEVELS

#define MAXPLAYERS 64
#define P_TIDSTART 1000
#define TICRATE 35

#define DND_PETTID_BEGIN 55000
#define DND_MONSTERTID_BEGIN 66000

#define DND_BASE_HEALTH 100

#define DND_DROP_TID 343 // some dumb number

#define ZEALOT_BASE_TID 14000
#define AVATAR_BASE_TID 5000
#define PURP_DEM_TID 13000

#define DND_EMERALD_TIDADD 100

#define ASPECT_4_3 (4.0 / 3)
#define ASPECT_5_4 1.25
#define ASPECT_16_9 (16.0 / 9)
#define ASPECT_16_10 1.6

enum {
	DND_PLAYER_DOOMGUY,
	DND_PLAYER_MARINE,
	DND_PLAYER_HOBO,
	DND_PLAYER_PUNISHER,
	DND_PLAYER_WANDERER,
	DND_PLAYER_CYBORG
};

#define MAXPLAYERCLASSES (DND_PLAYER_CYBORG + 1)

#define DND_DOOMGUY_DROPCHANCE 0.15

#define DND_HOBO_SHOTGUNBONUS 30
#define DND_HOBO_SHOTGUNPELLETBONUS 50

#define DND_PUNISHER_PERK2_MULT 3
#define DND_PUNISHER_PERK2_DIV 20
#define DND_PUNISHER_PERK3_KILLCOUNT 50
#define DND_PUNISHER_PERK3_MAX 100

/*
////////////////
// TID RANGES //
////////////////

* 1000 - 1063 = Players
* 1100 - 1163 = Emerald Death Actors
* 1200 - 1263 = Player temporary weapon drop ids (only at the moment of drop, cleared the next tic)
* 1500 - 1563 = Temporary tid assignment of pets
* 1600 - 1663 = Temporary tid assignment for spells
* 1664 - 1727 = Menu icon above people
* 2000 - 2063 = Deathray marker TID for players
* 3000 - 3063 = Initial railgun trail ID
* 3500 - 3563 = In-between trails ID for railgun
* 3564 = Dark Zealot Shield TID
* 3565 = Avatar soul projectiles TID
* 6000 - 12000 = Avatar Cubes TID
* 15000+ = Zealot Shield TID
* 17000 - 19048 = Shared item IDs
* 19049 - 29049 = Limited Respawn Ammos
* 32768 = Special FX TID
* 32769 = Thunder Staff temporary damager tid
* 40000 - 42048 = Thunder Staff Ring tid
* 42049 - Talisman Mark tid
* 55000 - 65999 = pet tids
* Anything above 66000 => any monster tid
*/

enum {
	DND_MAPINFO_MAPCHANGED,
	DND_MAPINFO_HASDOOMGUY
};

// save for later
global bool 0: MapInfo[32];
global int 5: HardcoreSet;
global bool 7: PlayerDied[MAXPLAYERS];

enum {
	DND_MODE_NORMAL,
	DND_MODE_SOFTCORE,
	DND_MODE_HARDCORE
};

int settings[4][2] = {
	{ BT_FORWARD,					0.00 	},
	{ BT_MOVELEFT | BT_LEFT,		0.25	},
	{ BT_BACK,						0.50	},
	{ BT_MOVERIGHT | BT_RIGHT,	    0.75	}
};

int IsButtonPressed (int input, int oldInput, int mask) {
	return (input & ~oldInput) & mask;
}

int IsButtonHeld (int input, int mask) {
	return input & mask;
}

#define MAX_SCREENRES_OFFSETS 4
int ScreenResOffsets[MAX_SCREENRES_OFFSETS] = { -1, -1, -1, -1 };
int active_quest_id = -1;

#define DND_TID_MONSTER 0
#define DND_TID_PET 1
// keeps at what tid we are left off
int DnD_TID_List[2] = { DND_MONSTERTID_BEGIN, DND_PETTID_BEGIN };

#define PLAYERLEVELINFO_LEVEL 0
#define PLAYERLEVELINFO_MINLEVEL 1
#define PLAYERLEVELINFO_MAXLEVEL 2
#define PLAYERLEVELINFO_COUNTATSTART 3
int PlayerInformationInLevel[4] = {
	0, INT_MAX, INT_MIN, 0
};

void GiveMonsterTID () {
	Thing_ChangeTID(0, DnD_TID_List[DND_TID_MONSTER]++);
}

void GivePetTID() {
	Thing_ChangeTID(0, DnD_TID_List[DND_TID_PET]++);
}

enum {
    LEVEL_TOTAL,
    LEVEL_MIN,
    LEVEL_MAX
};

int XorBit(int x, int n) {
	return x ^ (1 << n);
}

int SetBit(int x, int n) {
	return x | (1 << n);
}

int ClearBit(int x, int n) {
	return x & ~(1 << n);
}

int IsSet(int x, int n) {
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
		res++;
	return res;
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

bool isPlayer(int tid) {
	return tid >= P_TIDSTART && tid < P_TIDSTART + MAXPLAYERS;
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

bool player_pickup_cubes_intersect(int player_tid, int object_tid) {
	//tid1 = doom player: height: 56, radius: 16.
	//tid2 = pickups: height: 16, radius: 20.

	//printbold(s:"actor_cubes_intersect: player tid: ", d:player_tid, s:", item tid: ", d:object_tid, s:", radius: ", f:radius, s:", check states: ", d:check_state_1, s:", ", d:check_state_2, s:", ", d:check_state_3_1, s:"; ", d:check_state_3_2, s:", player z:", f:getactorz(player_tid), s:", object z: ", f:getactorz(object_tid));
	
	return	((getactorx(player_tid)-16.0 <= getactorx(object_tid)+20.0) && (getactorx(player_tid)+16.0 >= getactorx(object_tid)-20.0)) &&
			((getactory(player_tid)-16.0 <= getactory(object_tid)+20.0) && (getactory(player_tid)+16.0 >= getactory(object_tid)-20.0)) &&
			((getactorz(player_tid) <= getactorz(object_tid)+16.0) && (getactorz(player_tid)+56.0 >= getactorz(object_tid)));
}


// takes deltax and deltay as parameter for actor comparisons
int AproxDistance (int dx, int dy) {
	dx = abs(dx);
	dy = abs(dy);

	if (dx < dy)
		return dx + dy - (dx >> 1);

	return dx + dy - (dy >> 1);
}

int SetInventory (str item, int count) {
	int n = count - CheckInventory (item);
	if (n > 0)
		GiveInventory (item, n);
	else if (n < 0)
		TakeInventory (item, -n);
	return n;
}

int SetActorInventory (int tid, str item, int count) {
	int n = count - CheckActorInventory (tid, item);
	if (n > 0)
		GiveActorInventory (tid, item, n);
	else if (n < 0)
		TakeActorInventory (tid, item, -n);
	return n;
}

void GiveOrTake(str item, int count, bool mode) {
	if(!mode)
		GiveInventory(item, count);
	else
		TakeInventory(item, count);
}

int Clamp_Between(int x, int low, int high) {
	if(x < low)
		return low;
	if(x > high)
		return high;
	return x;
}

int IsAlive() {
	return GetActorProperty(0, APROP_HEALTH) > 0;
}

int IsActorAlive(int tid) {
	return GetActorProperty(tid, APROP_HEALTH) > 0;
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

int IsDigit(int c) {
	return c >= '0' && c <= '9';
}

// improve sens as parameter later?
int ftrunc(int x) {
	return (x + 0.05) & 0xFFFFF000;
}

// user must guarantee setspecial and setspecial2 are less than 65536
void SpawnDrop(str actor, int zoffset, int thrust, int setspecial, int setspecial2) {
	SpawnForced(actor, GetActorX(0), GetActorY(0), GetActorZ(0) + zoffset, DND_DROP_TID);
	ThrustThing(random(0, 255), random(2, 5), 0, DND_DROP_TID);
	ThrustThingZ(DND_DROP_TID, thrust, 0, 1);
	SetActorProperty(DND_DROP_TID, APROP_MASS, setspecial | (setspecial2 << 16));
	Thing_ChangeTID(DND_DROP_TID, 0);
}

void SpawnDropFacing(str actor, int zoffset, int thrust, int setspecial, int setspecial2) {
	SpawnForced(actor, GetActorX(0), GetActorY(0), GetActorZ(0) + zoffset, DND_DROP_TID);
	ThrustThing(GetActorAngle(0) >> 8, 6, 0, DND_DROP_TID);
	ThrustThingZ(DND_DROP_TID, thrust, 0, 1);
	SetActorProperty(DND_DROP_TID, APROP_MASS, setspecial | (setspecial2 << 16));
	Thing_ChangeTID(DND_DROP_TID, 0);
}

void DeleteText(int textid) {
	HudMessage(s:""; HUDMSG_PLAIN, textid, -1, 160.0, 100.0, 0.0, 0.0);
}

void DeleteTextRange(int r1, int r2) {
	for(int i = 0; i < r2 - r1 + 1; i++)
		HudMessage(s:""; HUDMSG_PLAIN, r1 + i, -1, 160.0, 100.0, 0.0, 0.0);
}

int VectorPitch (Int t1, Int t2, int dx, int dy, int adj) {
	If(adj != 0)
		adj = adj << 16;
	Return(VectorAngle(AproxDistance(dx, dy), GetActorZ(t1) - (GetActorZ(t2) - adj)));
}

int GetAspectRatio(void) {
	int width = getcvar("vid_defwidth");
	int height = getcvar("vid_defheight");
	int nowidescreen = getcvar("vid_nowidescreen");
	int tft = getcvar("vid_tft");
	int aspect = getcvar("vid_aspect");
	switch(aspect) {
		case 1:	return ASPECT_16_9;
		case 2:	return ASPECT_16_10;
		case 3:	return ASPECT_4_3;
		case 4:	return ASPECT_5_4;
	}
	if(nowidescreen) {
		if(!tft)
			return ASPECT_4_3;
		if(fixedmul(height<<16, fixeddiv(5.0, 4.0)) == width<<16)
			return ASPECT_5_4;
		else
			return ASPECT_4_3;
	}
	if(abs((abs(fixedmul(height<<16, fixeddiv(16.0, 9.0)))>>16) - width) < 10) {
		return ASPECT_16_9;
	}
	if(abs((abs(fixedmul(height<<16, fixeddiv(16.0, 10.0)))>>16) - width) < 60) {
		if((width == 320 && height == 200) || (width == 640 && height == 400))
			return ASPECT_4_3;
		return ASPECT_16_10;
	}
	if(fixedmul(height<<16, fixeddiv(5.0, 4.0))>>16 == width && tft) {
		return ASPECT_5_4;
	}
	return ASPECT_4_3;
}

bool isPlayerClass(int ctype) {
	return CheckInventory("DnD_Character") - 1 == ctype;
}

#endif