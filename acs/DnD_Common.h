#ifndef DND_COMMON_IN
#define DND_COMMON_IN

//#define ISDEBUGBUILD
//#define ISAPRILFIRST // enables memes... OH NO

// string tables should always follow icon + name if they have both
#define STRING_ICON 0
#define STRING_NAME 1

#define CHAOSMARK_DAMAGEBUFF 50

#define MAXLEVELS 100
#define DND_MAX_MONSTERLVL MAXLEVELS

#define FACTOR_FIXED_RESOLUTION 1000

#define MAXPLAYERS 64
#define TICRATE 35
#define TICRATE_F 35.0
#define HALF_TICRATE 17

#define DND_BASE_HEALTH 100

// moved here for better access everywhere, was necessary for monster resists
enum {
	DND_DAMAGECATEGORY_MELEE,
	DND_DAMAGECATEGORY_BULLET,
	DND_DAMAGECATEGORY_ENERGY,
	DND_DAMAGECATEGORY_EXPLOSIVES,
	DND_DAMAGECATEGORY_OCCULT,
	DND_DAMAGECATEGORY_FIRE,
	DND_DAMAGECATEGORY_ICE,
	DND_DAMAGECATEGORY_LIGHTNING,
	DND_DAMAGECATEGORY_POISON,
	DND_DAMAGECATEGORY_SOUL
};
#define DND_ELECATEGORY_BEGIN DND_DAMAGECATEGORY_FIRE
#define DND_ELECATEGORY_END DND_DAMAGECATEGORY_POISON
#define MAX_DAMAGE_CATEGORIES (DND_DAMAGECATEGORY_SOUL + 1)

#define DND_WEAKNESS_FACTOR 25 // 25% extra dmg
#define DND_SPECIFICELEWEAKNESS_FACTOR 50 // 50% extra dmg taken from specific elemental dmg
#define DND_RESIST_FACTOR 50 // 50% dmg taken
#define DND_IMMUNITY_FACTOR 95 // 5% dmg taken
#define DND_IMMUNITY_HARDCAP_FACTOR 99 // 1% damage taken

#define DND_DAMAGECATEGORY_BEGIN DND_DAMAGECATEGORY_MELEE
#define DND_DAMAGECATEGORY_END (DND_DAMAGECATEGORY_POISON + 1)

// this dumb number wasted weeks of our time, fuck you dumb number!
//#define DND_DROP_TID 343 // some dumb number

#define EPSILON 0.000001

#define DND_VIEWCHECK_DENSITY 8

#define DND_MONSTERBONUS_PERLVL 15 // 15% increase per level on gains granted by monsters

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

#define DND_PLAYER_RADIUS 16.0
#define DND_PLAYER_RADIUS_INT (DND_PLAYER_RADIUS >> 16)

#define DND_MARINE_SELFEXPLOSIVEREDUCE 25
#define DND_MARINE_EXPLOSIVEREDUCTION 33

#define DND_HOBO_SHOTGUNBONUS 30
#define DND_HOBO_SHOTGUNPELLETBONUS 0.5

#define DND_PUNISHER_SPREERECOVER 32 // 32%
#define DND_PUNISHER_SPREEPERBONUS 8
#define DND_PUNISHER_PERK3_KILLCOUNT 10
#define DND_PUNISHER_PERK3_MAX 100
#define DND_PUNISHER_DMGINCF 0.04
#define DND_PUNISHER_DMGINC 4 // 4%
#define DND_PUNISHER_DMG_MAX 1000

#define PUNISHERPERKID1 7010
#define PUNISHERPERKID2 7011

#define DND_WANDERER_POISONPERCENT 75
#define DND_WANDERER_SPELLEFFICIENCY 4
#define DND_WANDERER_COOLDOWNBONUS 4

#define DND_CYBORG_CYBER_MULT 3
#define DND_CYBORG_CYBER_DIV 10

#define DND_EXP_HUDID_FILL 266
#define DND_EXP_HUDID_BACK 267

#define DND_MAX_MONSTERS 12800

#define DND_MAX_TEMP_PROJ 200
#define DND_EMERALD_TIDADD 100
#define DND_TEMPORARY_TIDADD 200
#define DND_AVATAR_CUBESKIP 6000

#define MAX_SHARED_ITEMS 2048
#define SHARED_ITEM_TYPE_TID_MAX 19048

#define LIMITED_RESPAWN_TID_END 29049
#define LIMITED_RESPAWN_MAX 10000

#define THUNDERSTAFF_RING_SKIP 2048 // this is calculated in spawner code, but basically 32 x 64 = 2048

#define MAX_SHOOTABLE_TIDS 4000
#define MAX_NPCS 512
#define MAX_PETS 1600 // max of 1600 pets stored here because => 25 max pets per player x 64 = 1600
#define MAX_PICKUPS 3000

enum {
	// dont change order of these, add below the last!
	STAT_STR,
	STAT_DEX,
	STAT_INT,
	
	// same for these
	STAT_SHRP,
	STAT_BRUT,
	STAT_END,
	STAT_WIS,
	STAT_GRE,
	STAT_MED,
	STAT_MUN,
    STAT_DED,
	STAT_SAV,
	STAT_LUCK,
	
	STAT_EXP,
	STAT_LVLEXP,
	STAT_LVL,
	STAT_CRED,
	STAT_LVLCRED
};

#define DND_ATTRIB_BEGIN STAT_STR
#define DND_ATTRIB_END STAT_INT
#define DND_MAX_ATTRIBUTES (DND_ATTRIB_END - DND_ATTRIB_BEGIN + 1)

#define DND_STAT_ATTUNEMENT_GAIN 0.05

enum {
	// from 1 to DND_MAX_MONSTERS + 1 is the amount of monsters we support -- we'll do tid - 1 to access array loc
	DND_MONSTERTID_BEGIN = 1,
	
	// 12801 - 12864 player tid range
	P_TIDSTART = DND_MONSTERTID_BEGIN + DND_MAX_MONSTERS,
	// emerald death actors add +100 to here, temp weapons use +200
	
	// temporary proj tid for use with scripts firing custom proj
	// also skips emerald death tid add
	DND_TEMP_PROJTID = P_TIDSTART + MAXPLAYERS + DND_TEMPORARY_TIDADD,
	
	// draugr teleport fx
	DRAUGR_TEMP_FX = DND_TEMP_PROJTID + DND_MAX_TEMP_PROJ,
	
	// 64 player temp tid range
	TEMPORARY_PET_TID,
	
	// 64 player temp tid range
	TEMPORARY_SPELL_TID = TEMPORARY_PET_TID + MAXPLAYERS,
	
	// 64 players temp tid range
	TEMPORARY_DATADUMMY_TID = TEMPORARY_SPELL_TID + MAXPLAYERS,

	PROJECTILE_HELPER_TID = TEMPORARY_DATADUMMY_TID + MAXPLAYERS,
	
	// circle attacks from burst attack modifiers
	TEMPORARY_ATTACK_TID = PROJECTILE_HELPER_TID + MAXPLAYERS,
	
	// 64 player temp tid range
	DND_MENUFLOATYICON_TID = TEMPORARY_ATTACK_TID + MAXPLAYERS,
	
	// 64 player temp tid range
	DEATHRAY_MARKER_TID = DND_MENUFLOATYICON_TID + MAXPLAYERS,
	
	// 64 player temp tid range
	DARKLANCE_TID = DEATHRAY_MARKER_TID + MAXPLAYERS,
	DARKLANCE_SHREDDER = DARKLANCE_TID + MAXPLAYERS,
	
	// 64 player temp tid range
	KANJI_TRAP_TID = DARKLANCE_SHREDDER + MAXPLAYERS,
	KANJI_TRAPEXP_TID = KANJI_TRAP_TID + MAXPLAYERS,
	
	UNIQUE_MON_AUX_TID,
	DND_FROZENFX_TID,
	DND_CHAOSMARKFX_TID,
	DND_SHOTGUNPUFF_REMOVETID,
	
	// 64 player temp tid range
	DND_SHOTGUNPUFF_TID,
	
	ZEALOT_SHIELD_TID = DND_SHOTGUNPUFF_TID + MAXPLAYERS,
	
	AVATAR_SOUL_TID,
	
	// has 6000 skip here, supports max 2000 avatars x 3 = 6000
	AVATAR_CUBE_TID,
	
	// max 2048
	SHARED_ITEM_TID_BEGIN = AVATAR_CUBE_TID + DND_AVATAR_CUBESKIP,
	
	// limited respawn skips MAX_SHARED_ITEMS from above, and has 10k skip for next
	LIMITED_RESPAWN_TID_BEGIN = SHARED_ITEM_TID_BEGIN + MAX_SHARED_ITEMS,
	
	DND_DAMAGENUMBER_TID = LIMITED_RESPAWN_TID_BEGIN + LIMITED_RESPAWN_MAX,
	
	DND_THUNDERSTAFF_DAMAGERTID,
	DND_ICECHUNK_TID,
	DND_TEMP_PLAYERPROJTID,
	
	// we allocate each player proj tid to their own pnum
	DND_THUNDER_RING_TIDSTART = DND_TEMP_PLAYERPROJTID + MAXPLAYERS,
	
	// allocate enough room for max 4 hammers per player... idk why but why not
	DND_TALISMAN_MARK = DND_THUNDER_RING_TIDSTART + THUNDERSTAFF_RING_SKIP,
	
	DND_SHOOTABLETID_BEGIN,
	
	// 64 player temp tid range
	DND_WANDERER_EXP_TID = DND_SHOOTABLETID_BEGIN + MAX_SHOOTABLE_TIDS,
	
	// 64 player temp tid range
	DND_CROSSBOW_EXPLOSIONTID = DND_WANDERER_EXP_TID + MAXPLAYERS,
	
	DND_NPC_TID = DND_CROSSBOW_EXPLOSIONTID + MAXPLAYERS,
	
	DND_PETTID_BEGIN = DND_NPC_TID + MAX_NPCS,
	
	DND_PICKUPTID_BEGIN = DND_PETTID_BEGIN + MAX_PETS,
	
	DND_SUBORDINATE_TEMPTID = DND_PICKUPTID_BEGIN + MAX_PICKUPS,
	
	DND_DROP_TID = INT_MAX - 1,
	SPECIAL_FX_TID
};

#define DND_LASTMONSTER_TID (DND_MAX_MONSTERS + DND_MONSTERTID_BEGIN)
#define DND_PETTID_END (DND_PETTID_BEGIN + MAX_PETS)

enum {
	SETUP_STATE1
};

enum {
	// 32 per
	SETUP_HARDCORE,
	SETUP_SOFTCORE,
	SETUP_MONSTERS,
	SETUP_ITEMTABLES,
	SETUP_MAPCHANGED,
	SETUP_WEAPONDATA,
	SETUP_PLAYERINFO_MINMAXLEVELS,
};
global int 55: SetupStates[2];

void SetupComplete(int state, int flag) {
	SetupStates[state] |= 1 << flag;
}

void SetupUndo(int state, int flag) {
	SetupStates[state] &= ~(1 << flag);
}

bool isSetupComplete(int state, int flag) {
	return SetupStates[state] & (1 << flag);
}

bool isSoftorHardcore() {
	return isSetupComplete(SETUP_STATE1, SETUP_SOFTCORE) || isSetupComplete(SETUP_STATE1, SETUP_HARDCORE);
}

bool isHardcore() {
	return isSetupComplete(SETUP_STATE1, SETUP_HARDCORE);
}

global bool 6: PlayerLoaded[MAXPLAYERS];
global bool 7: PlayerDied[MAXPLAYERS];

// used for aux stuff -- 16 unique sections
enum {
	TARR_ORB1,
	TARR_ORB2,
	TARR_MENU
};
global int 58: TempArray[16][256];

enum {
	DND_MODE_NORMAL,
	DND_MODE_SOFTCORE,
	DND_MODE_HARDCORE
};

// These are actor inherited flags, like forcepain, foilinvul, painless etc.
// can store at most 16 bits, the last 16 are for wepid reference
enum {
	DND_ACTORFLAG_FOILINVUL				=			0b1,
	DND_ACTORFLAG_FORCEPAIN				=			0b10,
	DND_ACTORFLAG_PAINLESS				=			0b100,
	DND_ACTORFLAG_NOPUSH				=			0b1000,
	DND_ACTORFLAG_CONFIRMEDCRIT			=			0b10000,
	DND_ACTORFLAG_COUNTSASMELEE			=			0b100000,
	DND_ACTORFLAG_THRUGHOST				=			0b1000000,
	DND_ACTORFLAG_FORCERADIUSDMG		=			0b10000000,
	DND_ACTORFLAG_ISDAMAGEOVERTIME		=			0b100000000,
};

int ScanActorFlags() {
	return 	CheckFlag(0, "FOILINVUL") * DND_ACTORFLAG_FOILINVUL				|
			CheckFlag(0, "FORCEPAIN") * DND_ACTORFLAG_FORCEPAIN				|
			CheckFlag(0, "PAINLESS") * DND_ACTORFLAG_PAINLESS				|
			CheckFlag(0, "NODAMAGETHRUST") * DND_ACTORFLAG_NOPUSH			|
			CheckFlag(0, "THRUGHOST") * DND_ACTORFLAG_THRUGHOST				|
			CheckFlag(0, "FORCERADIUSDMG") * DND_ACTORFLAG_FORCERADIUSDMG;
}

enum {
	MVMT_BT_FORWARD,
	MVMT_BT_LEFT,
	MVMT_BT_BACK,
	MVMT_BT_RIGHT
};

int GetMovementButton(int b) {
	switch(b) {
		case MVMT_BT_FORWARD:
		return BT_FORWARD;
		case MVMT_BT_LEFT:
		return BT_MOVELEFT | BT_LEFT;
		case MVMT_BT_BACK:
		return BT_BACK;
		case MVMT_BT_RIGHT:
		return BT_MOVERIGHT | BT_RIGHT;
	}
	return BT_FORWARD;
}

// order follows above
int GetMovementAngle(int b) {
	return b * 0.25;
}

int IsButtonPressed (int input, int oldInput, int mask) {
	return (input & ~oldInput) & mask;
}

int IsButtonHeld (int input, int mask) {
	return input & mask;
}

// for now allocate slots for 10
#define MAX_SCRIPT_TRACK 10

enum {
	DND_SCRIPT_EXPTRACKER,
	DND_SCRIPT_BLEND,
	DND_SCRIPT_DAMAGETAKENTIC
};

global bool 17: PlayerScriptsCheck[MAX_SCRIPT_TRACK][MAXPLAYERS];

int active_quest_id = -1;

#define DND_MAX_PETS_PER_PLAYER 25
#define DND_MAX_PETS (DND_MAX_PETS_PER_PLAYER * MAXPLAYERS) // 25 pets per player x 64 players

#define DND_TID_MONSTER 0
#define DND_TID_SHOOTABLE 1
#define DND_TID_PICKUPS 2
#define DND_TID_SHAREDITEMS 3
// keeps at what tid we are left off
// { DND_MONSTERTID_BEGIN, DND_SHOOTABLETID_BEGIN, DND_PICKUPTID_BEGIN, SHARED_ITEM_TID_BEGIN };
int DnD_TID_Counter[4] = { 0, 0, 0, 0 };

// holds the monster tids that are in use -- arbitrary order
int UsedMonsterTIDs[DND_MAX_MONSTERS];

// Turned this to global to allow outside access for modders
#define PLAYERLEVELINFO_LEVEL 0
#define PLAYERLEVELINFO_MINLEVEL 1
#define PLAYERLEVELINFO_MAXLEVEL 2
#define PLAYERLEVELINFO_COUNTATSTART 3
#define PLAYERLEVELINFO_TIDMONSTER 4 // how many monster tids were skipped, this can happen if mappers allocated tids of their own
#define PLAYERLEVELINFO_LEVELATSTART 5 // level total of players at the start of the game
#define MAX_PLAYERLEVELINFO_DATA (PLAYERLEVELINFO_LEVELATSTART + 1)

bool pinfo_pending_reset = true;
global int 28: PlayerInformationInLevel[MAX_PLAYERLEVELINFO_DATA];

void ResetPlayerInformationLevel() {
	PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] = 0;
	PlayerInformationInLevel[PLAYERLEVELINFO_MINLEVEL] = INT_MAX;
	PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL] = INT_MIN;
	PlayerInformationInLevel[PLAYERLEVELINFO_COUNTATSTART] = 0;
	PlayerInformationInLevel[PLAYERLEVELINFO_TIDMONSTER] = 0;
	PlayerInformationInLevel[PLAYERLEVELINFO_LEVELATSTART] = 0;
	pinfo_pending_reset = false;

	SetupUndo(SETUP_STATE1, SETUP_PLAYERINFO_MINMAXLEVELS);
}

void GiveMonsterTID(int base_tid) {
	if(!base_tid) {
		int temp = DND_MONSTERTID_BEGIN + DnD_TID_Counter[DND_TID_MONSTER] + PlayerInformationInLevel[PLAYERLEVELINFO_TIDMONSTER];
		
		// we have to constantly check if we have run into a specific tid monster...
		while(ThingCount(0, temp)) {
			++temp;
			++PlayerInformationInLevel[PLAYERLEVELINFO_TIDMONSTER];
		}
		base_tid = temp;
		Thing_ChangeTID(0, base_tid);
	}
	
	UsedMonsterTIDs[DnD_TID_Counter[DND_TID_MONSTER]++] = base_tid;
}

void GivePetTID(int master_tid) {
	int master_pnum = master_tid - P_TIDSTART;
	int i;
	int pet_tid_beg = DND_PETTID_BEGIN + master_pnum * DND_MAX_PETS_PER_PLAYER;
	
	// check linearly while the pet tids are occupied, find one that isn't
	for(i = 0; i < DND_MAX_PETS_PER_PLAYER && ThingCount(T_NONE, pet_tid_beg + i); ++i);
	
	// we haven't skipped over lim which is beginning of pet tid for that player + max pet cap + 1
	if(i != pet_tid_beg + DND_MAX_PETS_PER_PLAYER + 1)
		Thing_ChangeTID(0, pet_tid_beg + i);
}

void GiveShootableTID() {
	Thing_ChangeTID(0, DND_SHOOTABLETID_BEGIN + DnD_TID_Counter[DND_TID_SHOOTABLE]);
	++DnD_TID_Counter[DND_TID_SHOOTABLE];
}

void GivePickupTID() {
	Thing_ChangeTID(0, DND_PICKUPTID_BEGIN + DnD_TID_Counter[DND_TID_PICKUPS]);
	++DnD_TID_Counter[DND_TID_PICKUPS];
}

void GiveSharedItemTID() {
	Thing_ChangeTID(0, SHARED_ITEM_TID_BEGIN + DnD_TID_Counter[DND_TID_SHAREDITEMS]);
	++DnD_TID_Counter[DND_TID_SHAREDITEMS];
}

enum {
    LEVEL_TOTAL,
    LEVEL_MIN,
    LEVEL_MAX
};

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

bool isPlayer(int tid) {
	return tid >= P_TIDSTART && tid < P_TIDSTART + MAXPLAYERS;
}

int AngleToFace(int this, int to) {
	int x = GetActorX(to) - GetActorX(this);
	int y = GetActorY(to) - GetActorY(this);
	return VectorAngle(x, y);
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

// contains overflow checks
int ApplyDamageFactor_Safe(int dmg, int factor, int div = 100) {
	// disabled overflow checks for now, see if there's any improvement in performance
	// if this turns out to be necessary, I'll enable this
	//if(dmg < INT_MAX / factor)
		return dmg * factor / div;
	//return INT_MAX;
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

int IsDigit(int c) {
	return c >= '0' && c <= '9';
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

bool PlayerIsInvulnerable() {
	return CheckInventory("P_Invulnerable") || CheckInventory("Invulnerable_Better") || GEtActorProperty(0, APROP_INVULNERABLE);
}

// primarily to be used for cases where the actor in question has health that can overflow when multiplied by 100
int GetHealthPercentage(int currhp, int maxhp) {
	// get the 1% of the max hp
	int one_pct = maxhp / 100;
	if(one_pct < 1)
		one_pct = 1;
		
	// find how many of this one pct we contain by dividing the curr hp again
	// forced exception to make sure the player knows they have only done 1% after they really have, and not just 99% displayed on the slightest dmg dealt
	if(currhp > maxhp - one_pct)
		return 100;

	// get the pct of the hp now
	return currhp / one_pct;
}

// user must guarantee setspecial and setspecial2 are less than 65536
void SpawnDrop(str actor, int zoffset, int thrust, int setspecial, int setspecial2, bool noRandomVelXY = false) {
	SpawnForced(actor, GetActorX(0), GetActorY(0), GetActorZ(0) + zoffset, DND_DROP_TID);
	if(!noRandomVelXY)
		ThrustThing(random(0, 255), random(3, 6), 0, DND_DROP_TID);
	ThrustThingZ(DND_DROP_TID, thrust, 0, 1);
	SetActorProperty(DND_DROP_TID, APROP_MASS, setspecial | (setspecial2 << 16));
	Thing_ChangeTID(DND_DROP_TID, 0);
}

void SpawnDropAtActor(int dest_tid, str actor, int zoffset, int thrust, int setspecial, int setspecial2, bool noRandomVelXY = false) {
	SpawnForced(actor, GetActorX(dest_tid), GetActorY(dest_tid), GetActorZ(dest_tid) + zoffset, DND_DROP_TID);
	if(!noRandomVelXY)
		ThrustThing(random(0, 255), random(3, 6), 0, DND_DROP_TID);
	ThrustThingZ(DND_DROP_TID, thrust, 0, 1);
	SetActorProperty(DND_DROP_TID, APROP_MASS, setspecial | (setspecial2 << 16));
	Thing_ChangeTID(DND_DROP_TID, 0);
}

void SpawnDropFacing(str actor, int zoffset, int thrust, int setspecial, int setspecial2) {
	SpawnForced(actor, GetActorX(0), GetActorY(0), GetActorZ(0) + zoffset, DND_DROP_TID);
	ThrustThing((GetActorAngle(0) >> 8) + random(-64, 64), 6, 0, DND_DROP_TID);
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

#include "DnD_Math.h"

#endif
