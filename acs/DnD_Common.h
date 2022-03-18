#ifndef DND_COMMON_IN
#define DND_COMMON_IN

//#define ISDEBUGBUILD

// string tables should always follow icon + name if they have both
#define STRING_ICON 0
#define STRING_NAME 1

#define DND_AFTER50_INCREMENT_DAMAGE 33
#define CHAOSMARK_DAMAGEBUFF 50

#define MAXLEVELS 100
#define DND_MAX_MONSTERLVL MAXLEVELS

#define MAXPLAYERS 64
#define P_TIDSTART 1000
#define TICRATE 35
#define HALF_TICRATE 17

#define DND_FROZENFX_TID 3000
#define DND_CHAOSMARKFX_TID 3001

#define SHARED_ITEM_TID_BEGIN 17000
#define SHARED_ITEM_TYPE_TID_MAX 19048
#define DND_DAMAGENUMBER_TID 30000
#define DND_SHOOTABLETID_BEGIN 50000
#define DND_PETTID_BEGIN 55000
#define DND_PICKUPTID_BEGIN 56001
#define DND_MONSTERTID_BEGIN 66000

#define DND_BASE_HEALTH 100

#define DND_DROP_TID 343 // some dumb number
#define DND_EMERALD_TIDADD 100

#define DND_VIEWCHECK_DENSITY 8

enum {
	DND_PLAYER_DOOMGUY,
	DND_PLAYER_MARINE,
	DND_PLAYER_HOBO,
	DND_PLAYER_PUNISHER,
	DND_PLAYER_WANDERER,
	DND_PLAYER_CYBORG,
	DND_PLAYER_BERSERKER
};

#define MAXPLAYERCLASSES (DND_PLAYER_BERSERKER + 1)

#define DND_MARINE_SELFEXPLOSIVEREDUCE 25
#define DND_MARINE_EXPLOSIVEREDUCTION 33

#define DND_HOBO_SHOTGUNBONUS 30
#define DND_HOBO_SHOTGUNPELLETBONUS 50

#define DND_PUNISHER_PERK2_MULT 3
#define DND_PUNISHER_PERK2_DIV 20
#define DND_PUNISHER_PERK3_KILLCOUNT 10
#define DND_PUNISHER_PERK3_MAX 100
#define PUNISHERPERKID1 7010
#define PUNISHERPERKID2 7011

#define DND_WANDERER_POISONPERCENT 75
#define DND_WANDERER_SPELLEFFICIENCY 4
#define DND_WANDERER_COOLDOWNBONUS 4

#define DND_CYBORG_ENERGYREDUCE 15
#define DND_CYBORG_CYBER_MULT 3
#define DND_CYBORG_CYBER_DIV 10

#define DND_EXP_HUDID_FILL 266
#define DND_EXP_HUDID_BACK 267

/*
////////////////
// TID RANGES //
////////////////

* 700 = Draugr Temporary FX
* 701 - 999 = Projectile temporary tid
* 1000 - 1063 = Players
* 1100 - 1163 = Emerald Death Actors
* 1200 - 1263 = Player temporary weapon drop ids (only at the moment of drop, cleared the next tic)
* 1500 - 1563 = Temporary tid assignment of pets
* 1600 - 1663 = Temporary tid assignment for spells
* 1664 - 1727 = Menu icon above people
* 2000 - 2063 = Deathray marker TID for players
* 2100 - 2163 = Dark Lance tids
* 2200 - 2263 = Dark Lance Shredder FX
* 2300 - 2363 = Kanji Trap tids
* 2400 - 2463 = Kanji Trap Explosion tid
* 2500 = Auxiliary unique monster tid
* 3000 = Frozen FX Temporary TID
* 3001 = Chaos Mark FX Temporary TID
* 3099 = Player shotgun puff removal tid
* 3100 - 3163 = Player Shotgun Puff Temporary TID
* 3564 = Dark Zealot Shield TID
* 3565 = Avatar soul projectiles TID
* 6000 - 12000 = Avatar Cubes TID
* 15000+ = Zealot Shield TID
* 17000 - 19048 = Shared item IDs
* 19049 - 29049 = Limited Respawn Ammos
* 30000 = Temporary tid for damage numbers
* 32768 = Special FX TID
* 32769 = Thunder Staff temporary damager tid
* 32770 = Ice chunk TID
* 40000 - 42048 = Thunder Staff Ring tid
* 42049 - Talisman Mark tid
* 50000 = Destructible object tid start
* 54000 - 54063 = Wanderer explosion tids
* 54100 - 54163 = Crossbow explosion tids
* 54165 = NPC TID
* 55000 - 56000 = pet tids
* 56001 - 59001 = other pickup tids of importance -- soulspheres, megas, invis, invul, radsuit etc.
* Anything above 66000 => any monster tid
*/

#define DND_SHOTGUNPUFF_REMOVETID 3099
#define DND_SHOTGUNPUFF_TID 3100
#define DND_ICECHUNK_TID 32770

enum {
	DND_MAPINFO_MAPCHANGED,
	DND_MAPINFO_HASDOOMGUY
};

typedef struct vec2 {
	int x;
	int y;
} vec2_T;

typedef struct vec3 {
	int x;
	int y;
	int z;
} vec3_T;

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

int active_quest_id = -1;

#define DND_TID_MONSTER 0
#define DND_TID_PET 1
#define DND_TID_SHOOTABLE 2
#define DND_TID_PICKUPS 3
#define DND_TID_SHAREDITEMS 4
// keeps at what tid we are left off
int DnD_TID_List[5] = { DND_MONSTERTID_BEGIN, DND_PETTID_BEGIN, DND_SHOOTABLETID_BEGIN, DND_PICKUPTID_BEGIN, SHARED_ITEM_TID_BEGIN };

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

void GiveShootableTID() {
	Thing_ChangeTID(0, DnD_TID_List[DND_TID_SHOOTABLE]++);
}

void GivePickupTID() {
	Thing_ChangeTID(0, DnD_TID_List[DND_TID_PICKUPS]++);
}

bool isEnemyTID(int tid) {
	return tid >= DND_MONSTERTID_BEGIN;
}

int GetIntegerBits(int x) {
	return x & 0xFFFF0000;
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

// naive search -- assumes lowercase key
bool StringContains(str s, str key) {
	int len = StrLen(s);
	int key_len = StrLen(key);
	
	int match_id = 0;
	for(int i = 0; i < len; ++i) {
		// force lowercase
		int c = GetChar(s, i);
		if(c < 'a')
			c += 32;
		if(c == GetChar(key, match_id)) {
			++match_id;
			if(match_id == key_len)
				return true;
		}
		else
			match_id = 0;
	}
	return false;
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

int AngleToFace(int this, int to) {
	int x = GetActorX(to) - GetActorX(this);
	int y = GetActorY(to) - GetActorY(this);
	return VectorAngle(x, y);
}

int LinearMap(int val, int o_min, int o_max, int n_min, int n_max) {
	return (val - o_min) * (n_max - n_min) / (o_max - o_min) + n_min;
}

int LinearMap_Fixed(int val, int o_min, int o_max, int n_min, int n_max) {
	return FixedDiv(FixedMul(val - o_min, n_max - n_min), o_max - o_min) + n_min;
}

void HealMonster(int mid, int amount) {
	int hp = GetActorProperty(0, APROP_HEALTH);
	amount = Clamp_Between(amount, 0, MonsterProperties[mid].maxhp - hp);
	SetActorProperty(0, APROP_HEALTH, hp + amount);
}

int PitchToFace(int m1, int m2) {
	int xdiff = GetActorX(m2) - GetActorX(m1);
	int ydiff = GetActorY(m2) - GetActorY(m1);
	int dist = AproxDistance(xdiff, ydiff);
	//dist >>= 8;
	int zdiff = GetActorZ(m2) - GetActorZ(m1);
	//zdiff >>= 8;
	return -VectorAngle(dist, zdiff);
}

int PitchToFace_Height(int m1, int m2) {
	int xdiff = GetActorX(m2) - GetActorX(m1);
	int ydiff = GetActorY(m2) - GetActorY(m1);
	int dist = AproxDistance(xdiff, ydiff);
	//dist >>= 8;
	int zdiff = (GetActorZ(m2) + GetActorProperty(m2, APROP_HEIGHT) / 2) - (GetActorZ(m1) + GetActorProperty(m1, APROP_HEIGHT) / 2);
	//zdiff >>= 8;
	return -VectorAngle(dist, zdiff);
}

void FaceActor(int this, int to) {
	int x = GetActorX(to) - GetActorX(this);
	int y = GetActorY(to) - GetActorY(this);
	SetActorAngle(this, VectorAngle(x, y));
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

int ftrunc2(int x) {
	return (x + 0.005) & 0xFFFFFC00;
}

int GetActivePlayerCount() {
	int res = 0;
	
	for(int i = 0; i < MAXPLAYERS; ++i)
		res += PlayerInGame(i) && !PlayerIsSpectator(i);
	
	return res;
}

int GetPlayerCountAny() {
	int res = 0;
	
	for(int i = 0; i < MAXPLAYERS; ++i)
		res += PlayerInGame(i) || PlayerIsSpectator(i);
	
	return res;
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

int SpawnAreaTID(int stid, int maxdist, int degree_inc, str actortype, int newtid, int ang_begin, int forced) {
	// tries to spawn an object in a circle around stid
	// tries halving radius if a full circular attempt failed until radius becomes 1
	int r = maxdist, tries = 0, circle_comp = 360 / (degree_inc >> 16), test = 0;
	int sang = GetActorAngle(stid) + ang_begin;
	bool finish = false;
	// convert to byte angle for this part
	degree_inc /= 360; 
	degree_inc <<= 8;
	while(r != 1.0 && !finish) {
		// try to spawn at this one point
		int cx = GetActorX(stid) + FixedMul(r, cos(sang + tries * degree_inc));
		int cy = GetActorY(stid) + FixedMul(r, sin(sang + tries * degree_inc));
		int cz = GetActorZ(stid);
		if(forced)
			test = SpawnForced(actortype, cx, cy, cz, newtid);
		else
			test = Spawn(actortype, cx, cy, cz, newtid);
			
		if(test)
			finish = true;
		else {
			++tries;
			if(tries == circle_comp) {
				tries = 0;
				r /= 2;
			}
		}
	}
	return finish;
}

void DeleteText(int textid) {
	HudMessage(s:""; HUDMSG_PLAIN, textid, -1, 160.0, 100.0, 0.1, 0.1);
}

// assumes r2 > r1
void DeleteTextRange(int r1, int r2) {
	for(int i = 0; i < r2 - r1 + 1; i++)
		HudMessage(s:""; HUDMSG_PLAIN, r1 + i, -1, 160.0, 100.0, 0.1, 0.1);
}

int VectorPitch (Int t1, Int t2, int dx, int dy, int adj) {
	If(adj != 0)
		adj = adj << 16;
	Return(VectorAngle(AproxDistance(dx, dy), GetActorZ(t1) - (GetActorZ(t2) - adj)));
}

int VectorLength3d(int x, int y, int z) {
	int len = VectorLength(x, y);
	len = VectorLength(z, len);
	return len;
}

bool isPlayerClass(int ctype) {
	return CheckInventory("DnD_Character") - 1 == ctype;
}

int GetPlayerClass() {
	return CheckInventory("DnD_Character") - 1;
}

int GetActorPlayerClass(int tid) {
	return CheckActorInventory(tid, "DnD_Character") - 1;
}

#endif
