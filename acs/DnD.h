#include "DnD_Common.h"
#include "DnD_Elites.h"
#include "Database/DnD_Database.h"
#include "DnD_TempWeps.h"
#include "DnD_Ammo.h"
#include "DnD_Monsters.h"
#include "DnD_SpecialTrails.h"
#include "Inventory/DnD_ChestKey.h"
#include "DnD_Dungeons.h"
#include "DnD_Skills.h"
#include "DnD_Settings.h"
#include "DnD_ClassMenu.h"
#include "DnD_Research.h"
#include "DnD_Statistics.h"
#include "DnD_Scoreboard.h"
#include "DnD_Attachments.h"
#include "Menu/DnD_Menu.h"

enum {
	ITEMFILTER_STACKABLE = 1,
	ITEMFILTER_CHARM = 2,
	ITEMFILTER_ARMOR = 4
};

#define DND_EXP_BASEFACTOR 4
#define DND_CREDITGAIN_FACTOR 1 // divides the regular gain by 2 -- was 2, now 1, goes up by 1
#define DND_CREDITGAIN_PERLEVELREDUCE 10 // every 10 levels we reduce the gain
#define DND_RESEARCH_MAX_CHANCE 1.0

void SetupCVarTracking() {
	CVarValues[DND_CVAR_RESEARCHDROPRATE] = Clamp_Between(GetCVar("dnd_researchdroprate"), 0.0, DND_RESEARCH_MAX_CHANCE);
	CVarValues[DND_CVAR_ACCESSORYDROPRATE] = (Clamp_Between(GetCVar("dnd_accessory_droprate"), 0, 100) * 1.0) / 100;
	CVarValues[DND_CVAR_ACCESSORYLEVEL] = Clamp_Between(GetCVar("dnd_accessorylevel"), 1, 100);
}

void HandlePlayerPainSound(int pclass) {
	int curhp = GetActorProperty(0, APROP_HEALTH);
	int maxhp = GetSpawnHealth();
	int hpratio = (curhp * 100) / maxhp;
	switch(pclass) {
		case DND_PLAYER_DOOMGUY:
			PlaySound(0, "Doomguy/Pain1", CHAN_BODY, 1.0);
		break;
		case DND_PLAYER_MARINE:
			// marine -- has 2 sounds for each quarter like so: Player/Pain25
			if(hpratio <= 25)
				PlaySound(0, "Player/Pain25", CHAN_BODY, 1.0);
			else if(hpratio <= 50)
				PlaySound(0, "Player/Pain50", CHAN_BODY, 1.0);
			else if(hpratio <= 75)
				PlaySound(0, "Player/Pain75", CHAN_BODY, 1.0);
			else
				PlaySound(0, "Player/Pain100", CHAN_BODY, 1.0);
		break;
		case DND_PLAYER_HOBO:
			// hobo
			if(hpratio <= 50)
				PlaySound(0, "Hobo/HardPain", CHAN_BODY, 1.0);
			else
				PlaySound(0, "Hobo/SoftPain", CHAN_BODY, 1.0);
		break;
		case DND_PLAYER_PUNISHER:
			// punisher
			if(hpratio <= 50)
				PlaySound(0, "Punisher/Pain", CHAN_BODY, 1.0);
			else
				PlaySound(0, "Punisher/PainLow", CHAN_BODY, 1.0);
		break;
		case DND_PLAYER_WANDERER:
			PlaySound(0, "Wanderer/Pain", CHAN_BODY, 1.0);
		break;
		case DND_PLAYER_CYBORG:
			// cyborg
			if(hpratio <= 50)
				PlaySound(0, "CyborgPlayer/PainLow", CHAN_BODY, 1.0);
			else
				PlaySound(0, "CyborgPlayer/PainHigh", CHAN_BODY, 1.0);
		break;
		case DND_PLAYER_BERSERKER:
			PlaySound(0, "BerserkerPlayer/Pain", CHAN_BODY, 1.0);
		break;
		case DND_PLAYER_TRICKSTER:
			if(hpratio <= 25)
				PlaySound(0, "Trickster/Pain25", CHAN_BODY, 1.0);
			else if(hpratio <= 50)
				PlaySound(0, "Trickster/Pain50", CHAN_BODY, 1.0);
			else if(hpratio <= 75)
				PlaySound(0, "Trickster/Pain75", CHAN_BODY, 1.0);
			else
				PlaySound(0, "Trickster/Pain100", CHAN_BODY, 1.0);
		break;
	}
}

void HandlePlayerDeathSound(int pclass, bool isXDeath) {
	str snd = "Doomguy/Death";
	switch(pclass) {
		case DND_PLAYER_DOOMGUY:
			if(isXDeath)
				snd = "Doomguy/XDeath";
			else
				snd = "Doomguy/Death";
		break;
		case DND_PLAYER_MARINE:
			snd = "Player/Die";
		break;
		case DND_PLAYER_HOBO:
			snd = "Hobo/Die";
		break;
		case DND_PLAYER_PUNISHER:
			snd = "Punisher/Die";
		break;
		case DND_PLAYER_WANDERER:
			snd = "Wanderer/Die";
		break;
		case DND_PLAYER_CYBORG:
			snd = "CyborgPlayer/Die";
		break;
		case DND_PLAYER_BERSERKER:
			snd = "Berserker/Die";
		break;
		case DND_PLAYER_TRICKSTER:
			if(isXDeath)
				snd = "Trickster/XDeath";
			else
				snd = "Trickster/Death";
		break;
	}
	
	PlaySound(0, snd, CHAN_BODY, 1.0);
}

enum {
	PAINBLEND_RED,
	PAINBLEND_GREEN,
	PAINBLEND_DARKSLATEGRAY,
	PAINBLEND_MIDNIGHTBLUE,
	PAINBLEND_DARKSLATEBLUE,
	PAINBLEND_ORANGE,
	PAINBLEND_LIGHTGREEN,
	PAINBLEND_DARKGOLDENROD,
	PAINBLEND_LIGHTBLUE,
	PAINBLEND_LIGHTSLATEBLUE,
	PAINBLEND_MEDIUMPURPLE,
	PAINBLEND_BLUE,
	PAINBLEND_GRAY
};

#define DND_PAINBLEND_MINTICS 8
#define DND_PAINBLEND_MAXTICS 35
#define DND_PAINBLEND_MAXALPHA 75 // 75%

#define PERK_GAIN_RATE 5

#define DND_EXPBAR_SIZEX 132

// dash script defs, by KeksDose
#define SD_TIMEFRAME		7			// Tics during which you must double press
#define SD_DASHDELAY		27			// Tics before being able to dash again
#define SD_DASHSPEED		7.75	   	// Dash speed whilst in the air
#define SD_DASHJUMP			4.0	   	    // Adds a little z-velocity while in the air
#define SD_DASHSTRONG		12.0		// And dash speed whilst grounded
#define SD_SOUNDVOLUME		1.0			// How loud the dash sound is played

// Acrobacy / Tactical Dash. The ability dash uses DashDelay, a PowerProtection with a fixed 35 tic
// duration baked into DECORATE. The perk dash needs a cooldown that shrinks with points, so it gets
// its own counter driven from ACS rather than trying to vary a powerup's duration.
//
// A player holding BOTH keeps two independent cooldowns, which is intended: the ability recharges in
// a second and the perk version in six, so they are not the same dash wearing one timer.
#define DND_PERKDASH_COOLDOWN (6 * TICRATE)

// The dash cooldown in tics, in ONE place. Three things need this exact number -- the dash that arms
// the timer, the recharge that re-arms it, and the HUD bar that measures against it -- and when they
// were three copies of the expression, a mod that changed the length reached some and not others.
// Undertow made that a live problem: its longer cooldown would have stretched the timer while the
// bar still filled against the old length, then stalled full.
// Emberwake. The trail is spawned from ACS and does its damage from ACS, but the ACTOR is pure
// DECORATE -- see the layout note. The contract between them is three things: the trail carries
// DnD_TrailOwner (pnum + 1, so zero stays "nobody") and DnD_TrailDamage, and it calls
// "DnD Ember Trail Burn" on itself once per burn tick.
#define DND_EMBERTRAIL_TID      31000   // one contiguous block, clear of every other range
#define DND_EMBERTRAIL_PERPLAYER 12     // segments alive at once, per player -- a ring, oldest reused
#define DND_EMBERTRAIL_RATE     9       // tics between segments dropped while moving
// GetPVelocity returns map units per tic as a PLAIN INT, not fixed point -- it ends in >> 16. A
// running player reads about 16, a walk about 8. These were written as 3.0 and 12.0, which made the
// moving branch unreachable and every tick take the standing still path.
#define DND_EMBERTRAIL_MINSPEED 3       // below this you count as standing still
#define DND_EMBERTRAIL_FULLSPEED 12     // at or above this the segment burns at full strength
#define DND_EMBERTRAIL_BURNRATE 26      // tics between self burns, matching the FireDOT damage tick

// Undertow. One marker TID per player, above every range the mod already hands out. The actor is
// spawned and removed CLIENTSIDE, so these TIDs only ever exist on the machine that can see them.
#define DND_ANCHORMARKER_TID    30000

int GetPerkDashCooldown(int pnum) {
	int cd = DND_PERKDASH_COOLDOWN * (100 - PlayerModData[pnum].vals[PSTAT_DASH_COOLDOWNREDUCE]) / 100;

	// Applied AFTER the reduction, so Tactical Dash still buys something: the boots lengthen whatever
	// the perks arrived at rather than overwriting it.
	int longer = PlayerModData[pnum].vals[PSTAT_EX_DASH_COOLDOWN_LONG];
	if(longer > 0)
		cd = cd * (100 + longer) / 100;

	return Max(1, cd);
}
#define DND_PERKDASH_RECENT_TICS (4 * TICRATE)   // Swift Reflexes' window, and "recently" generally
#define DND_PERKDASH_EVADE_TICS (3 * TICRATE)    // Evasive Maneuvers' base window
#define DND_PERKDASH_KILLWINDOW TICRATE          // Unending Rush: "within the second of dashing"

// Acrobacy / Crash Course. The dash is a velocity impulse rather than a moving hitbox ACS can watch,
// so the collision is a short proximity sweep over the tics the impulse is actually carrying the
// player -- past that they are walking, and walking into a monster is not crashing into it.
#define DND_CRASHCOURSE_WATCHTICS 12
#define DND_CRASHCOURSE_RADIUS 64.0
#define DND_CRASHCOURSE_BASEDMG 300
#define DND_CRASHCOURSE_STRFACTOR 0.025
#define DND_CRASHCOURSE_DEXFACTOR 0.025

// Acrobacy fall-impact cluster. "A great height" is a landing speed, not a distance: the engine
// gives velocity for free every tic while a fall's origin would have to be tracked and would be
// wrong the moment a lift or a teleport moved the floor.
#define DND_THUMPER_MINFALLVEL -12.0
#define DND_THUMPER_BASEDMG 100
#define DND_THUMPER_BASERADIUS 160.0
#define DND_THUMPER_STRFACTOR 0.05
#define DND_ADRENALINE_COOLDOWN (10 * TICRATE)

// Cunning flask cluster.
#define DND_SPIKEDCONCOCTION_MAXSTACKS 4         // 5% a stack up to 20%
#define DND_OVERFLOWING_REFILLPCT 50

// Perception / Sharpshooter. The range the bonus reaches its full value at, from the notes.
#define DND_SHARPSHOOTER_MAXRANGE 2048.0

// Martialist / Swift & Precise. The notes cap the stack at 100% and give it three seconds.
#define DND_SWIFTPRECISE_MAXSTACK 10
#define DND_SWIFTPRECISE_TICS (3 * TICRATE)

// Martialist / Expose Weakness. Fifteen seconds, from the notes.
#define DND_EXPOSEWEAKNESS_TICS (15 * TICRATE)

// Assassination.
#define DND_QUICKGETAWAY_TICS (3 * TICRATE)
#define DND_PREPARATION_DROUGHT (5 * TICRATE)    // "haven't landed a crit in the last 5 seconds"
#define DND_PRESSUREPOINTS_TICS (5 * TICRATE)
#define DND_STEADYSHOT_MAXSECONDS 10             // the notes cap the ramp at 100% on a 10% base

// A target counts as facing away past this many degrees off the line to the attacker. 90 is the
// literal reading of "back is turned" -- the rear hemisphere, not a narrow cone behind them.
#define DND_BACKSTAB_ANGLE 90.0

// Endurance / Wind Dancer. "Recently" is the mod's usual four seconds, per the General Notes.
#define DND_WINDDANCER_TICS (4 * TICRATE)

// Acrobacy / Nimbleness. Its ramp is per THREE seconds and caps at 25%, both from the notes.
#define DND_NIMBLENESS_SECSPERSTACK 3
#define DND_NIMBLENESS_CAP 25.0

// Martialist / Riposte. Ten second window, capped at three stacks -- 10% each to the notes' 30%.
#define DND_RIPOSTE_TICS (10 * TICRATE)
#define DND_RIPOSTE_MAXSTACK 3

// Rage, per the General Notes: 1% more attack damage a point to a cap of 30, falling to nothing in
// three seconds once none is being gained. The cap lives on the DnD_Rage actor's maxamount; this is
// the decay, which drains the whole bar over DND_RAGE_DECAYTIME however full it was -- "rapidly goes
// down to 0 in 3 seconds" is a fixed emptying time, not a fixed rate.
#define DND_RAGE_DAMAGEPER 1
#define DND_RAGE_MAXSTACK 30
#define DND_RAGE_DECAYTIME (3 * TICRATE)
#define DND_RAGE_GRACE (1 * TICRATE)     // gained rage holds this long before the drain starts

// Elusive, per the General Notes: 30% movement speed and 25% avoidance, decaying from full effect to
// nothing, and unobtainable again until it has worn off.
#define DND_ELUSIVE_SPEED 30
#define DND_ELUSIVE_AVOID 25
#define DND_ELUSIVE_TICS (5 * TICRATE)

// "Overkill: Killing blow that did more than 30% of an enemy's maximum health."
#define DND_OVERKILL_PERCENT 30

// Perception / Plan B. Three seconds, from the notes.
#define DND_PLANB_TICS (3 * TICRATE)

// Martialist / Flash Parry. The shockwave is a forward sweep rather than a spawned actor: the
// ParryAoE beside it carries its own damage and could not be scaled per perk without the actor
// asking ACS what to do.
#define DND_FLASHPARRY_BASERANGE 256.0
#define DND_FLASHPARRY_ANGLE 45.0
#define DND_FLASHPARRY_HEALTHPCT 5

// Acrobacy / Tailwind. Polled far less often than the Crash Course sweep -- running past someone
// is a second-scale event, not a tic-scale one, and this runs for every player forever.
#define DND_TAILWIND_POLLRATE 10
#define DND_TAILWIND_RADIUS 128.0
#define DND_TAILWIND_TICS (3 * TICRATE)
#define DND_TAILWIND_BASECOOLDOWN (10 * TICRATE)

// Perception / Heatsinks. 45 seconds before another is handed out, shrinking with points.
#define DND_HEATSINK_BASECOOLDOWN (45 * TICRATE)
#define DND_HEATSINK_POLLRATE 17

// Perception / Essence Theft. "Recently" is the mod's four seconds, per the General Notes.
#define DND_ESSENCETHEFT_TICS (4 * TICRATE)

// Endurance / Dense Exoskeleton. Long enough to cover the rip that triggered it and the next tic or
// two of the same ripper passing through, short enough not to be blanket immunity.
#define DND_RIPIMMUNE_TICS 17

#define CHANCE_HEART 0.025

#define UPGRADETEXTID 6999
#define SURVIVEICO 7000
#define SURVIVETOPTEXTID 7001
#define SURVIVETEXTID 7002
#define SURVIVEID 7003
#define SURVIVETEXTID2 7010
#define SURVIVEBAKID 7011

#define INTERVENTION_DURATION TICRATE * 8

#define DND_EMERALD_TRANSLATIONID 7000

#define DND_BUDGET_BASE 3
#define DND_MAX_SHARE 4
#define DND_HALF_SPREE_PER (DND_SPREE_PER / 2)
#define DND_SPREE_TRIGGER (DND_SPREE_PER - 1) // -1 because current monster killed doesn't count
#define DND_SPREE_BASE 5 // 5%, this is dividing the exp
#define DND_SPREE_BASE_F (DND_SPREE_BASE * 1.0 / 100)
#define DND_SPECIALAMMO_TEXTID 6001
#define DND_SPREE_BLOODID 6000
#define DND_SPREE_TEXT1ID 5999
#define DND_SPREE_TEXT2ID 5998

#define DND_MAX_MONSTERDELAY 8

#define DND_HEALTHEXPSCALE 5
#define DND_HEALTHCREDITSCALE 10
#define DND_HEALTHCREDITAFTER50SCALE 5
#define DND_HEALTHCREDITAFTER75SCALE 10
#define DND_HEALTHCREDITUPSCALE 1
#define DND_MIN_CREDIT 3
#define DND_REGEN_PERCENT 2

#define DND_ARTIFACT_GAIN 50

#define DND_NECRO_BULKSCALE 8

#define DND_AFTER50_INCREMENT 200
#define DND_AFTER75_INCREMENT 800

#define AGAMOTTO_MOVE_WINDOW 1 << 16

#define DND_MAX_DIGITLEN 7

#define DND_BOSS_SOULGIVE 4

#define RIP_FADE_TIMER 20
#define RIP_MONSTER_FADE_DELAY 3 * TICRATE

#define DND_SOULAMMO_DROPRATE 0.045 // 4.5% chance
#define DND_SOULAMMO_STEALERUPGRADE 0.025 // 2.5% more chance
#define DND_SOULAMMO_SMALLCHANCE 75

#define DND_LEGENDARY_ITEMDROPRATE 0.2 // 20%

#define DND_BOSSCHEST_DROPRATE 0.33 // 33% chance

#define DND_MARINE_SELFRESIST 25 // 25%

// RPG ELEMENTS END

#define DefStepSound "Player/Move"

#define DND_MAX_PULLFORCE 256.0
enum {
	DND_PULLF_CANPULLDEAD = 1,
	DND_PULLF_REVERSEDIR = 2
};

#define MAX_GRAVDIS_COUNT 512
#define GRAVDIS_DELAY_TIME 15 // 3 seconds is total time, 3 x (35 / 7) = 15
#define GRAVDIS_CHECK_PERIOD 7
#define GRAVDIS_MINPULLVEL 2.0
#define GRAVDIS_MAXPULLVEL 32.0
#define GRAVDIS_FLING_FACTOR 128
#define GRAVDIS_HEIGHT_FACTOR 15
#define GRAVDIS_HEIGHTADD_PER 64
#define GRAVDIS_CIRCLE_PARTICLES 48

#define CHARON_PARTICLE_DENSITY 8

#define DARKLANCE_ALT_DURATION 15 // 105 tic duration, 7 tic timer between damage instances => 105 / 7 = 15
#define DARKLANCE_ALT_DURATION_TICS 105
#define DARKLANCE_TICKS 7
#define DARKLANCE_BOUNCE_DIST 256.0

#define DND_LAND_WINDOW 1.0
#define DND_MAX_CORPSELAND_ATTEMPTS 1024

enum {
	DND_SPECIAL_RESEARCH = 1,
	DND_SPECIAL_ORB = 2,
	DND_SPECIAL_TALENTCAPSULE = 4,
	DND_SPECIAL_ELIXIR = 8,
	DND_SPECIAL_CHARM = 16
};

#define DND_BONUS_SCREENTIME 6
enum {
	BONUS_KILL,
	BONUS_ITEM,
	BONUS_SECRET,
	BONUS_BONUS,
	BONUS_EXP_RATE = 5,
	BONUS_CREDIT_RATE = 10,
	BONUS_SECRET_RATE = 3,
};
	
#define MAX_SPREE_TEXT 20
#define MAX_SPREE_MULT 10 // maximum 10 killing sprees will affect your exp gain
#define SPREE_TEXT_PREFIX "DND_SPREETEXT"

str GetSpreeText(int spree_id) {
	return StrParam(s:SPREE_TEXT_PREFIX, d:spree_id);
}

#define AMMODISPLAY_ID 1000

bool PlayerCanLoad[MAXPLAYERS] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

// various states are checked using this, such as bonus states or player joins
// don't add in between, add below!
enum {
	DND_STATECHECK_PLAYERJOIN,
	DND_STATECHECK_KILLBONUS,
	DND_STATECHECK_ITEMBONUS,
	DND_STATECHECK_SECRETBONUS,
	DND_STATECHECK_BONUSBONUS
};

#define MAX_STATES DND_STATECHECK_BONUSBONUS + 1
int DnD_StateChecker[MAX_STATES];
// see if map changed or not

int DnD_BonusMessageY(int bonustype) {
	int res = 34.1;
	for(int i = DND_STATECHECK_KILLBONUS; i <= DND_STATECHECK_BONUSBONUS; ++i)
		if(DnD_StateChecker[i])
			res += 16.0;
	return res;
}

#define DND_MAPDIFF_TIERVAL 250
#define DND_MAXMAPDIFF 9

int GetMonsterKillContribution(int category) {
	static int factor_table[MAX_MONSTER_CATEGORIES] = {
		DND_ZOMBIE_CONTRIB,
		DND_SHOTGUNNER_CONTRIB,
		DND_CHAINGUNNER_CONTRIB,
		DND_DEMON_CONTRIB,
		DND_DEMON_CONTRIB,
		DND_IMP_CONTRIB,
		DND_CACO_CONTRIB,
		DND_PAIN_CONTRIB,
		DND_SOUL_CONTRIB,
		DND_REVENANT_CONTRIB,
		DND_HELLKNIGHT_CONTRIB,
		DND_BARON_CONTRIB,
		DND_FATSO_CONTRIB,
		DND_ARACHNO_CONTRIB,
		DND_VILE_CONTRIB,
		DND_SPIDERMASTERMIND_CONTRIB,
		DND_CYBERDEMON_CONTRIB,
		DND_SHOTGUNNER_CONTRIB
	};
	return factor_table[category];
}

int GetPunisherTierKillBonus(int m_id) {
	return (GetMonsterKillContribution(MonsterProperties[m_id].class) << 16) & 0xFFFF0000;
}

str GetMonsterSpawnerStr(int id) {
	switch(id) {
		case MONSTERCLASS_ZOMBIEMAN:
		return "ZombiemanSpawner";
		case MONSTERCLASS_SHOTGUNGUY:
		return "ShotgunguySpawner";
		case MONSTERCLASS_CHAINGUNGUY:
		return "ChaingunguySpawner";
		case MONSTERCLASS_DEMON:
		return "DemonSpawner";
		case MONSTERCLASS_SPECTRE:
		return "SpectreSpawner";
		case MONSTERCLASS_IMP:
		return "ImpSpawner";
		case MONSTERCLASS_CACODEMON:
		return "CacoSpawner";
		case MONSTERCLASS_PAINELEMENTAL:
		return "PainElementalSpawner";
		case MONSTERCLASS_LOSTSOUL:
		return "LostSoulSpawner";
		case MONSTERCLASS_REVENANT:
		return "RevenantSpawner";
		case MONSTERCLASS_HELLKNIGHT:
		return "HellKnightSpawner";
		case MONSTERCLASS_BARON:
		return "BaronSpawner";
		case MONSTERCLASS_FATSO:
		return "FatsoSpawner";
		case MONSTERCLASS_ARACHNOTRON:
		return "ArachnoSpawner";
		case MONSTERCLASS_ARCHVILE:
		return "ArchVileSpawner";
		case MONSTERCLASS_SPIDERMASTERMIND:
		return "SpiderBossSpawner";
		case MONSTERCLASS_CYBERDEMON:
		return "CyberdemonSpawner";
		case MONSTERCLASS_WOLFENSS:
		return "ZombiemanSpawner";
	}
	return "";
}

#define DND_LOOT_MINMOBTHRESHOLD 100 // up to this threshold the game will give additional loot bonus
#define DND_LOOT_MAXMOBTHRESHOLD 1000 // after this threshold the game will give no loot bonus based on monster count
#define DND_LOOTBONUS_MOBCOUNT 0.5
#define DND_LOOTBONUS_LOWEREDVAL -0.5

// 5 Tiers: 0 -> Very Easy, 1 -> Easy, 2 -> Medium, 3 -> Hard and 4 -> Very Hard.
void CalculateMapDifficulty() {
	int factor = 0;
	
	MapData[DND_MAPDATA_MONSTERTOTAL] = 0;
	MapData[DND_MAPDATA_LOOTBONUS] = 0;

	for(int i = MONSTERCLASS_ZOMBIEMAN; i < MONSTERCLASS_WOLFENSS; ++i) {
		int index = DND_MAPDATA_ZOMBIEMANCOUNT + i;
		MapData[index] = ThingCountName(GetMonsterSpawnerStr(i), 0);
		if(MapData[index] > 0) {
			MapData[DND_MAPDATA_MONSTERTOTAL] += MapData[index];
			factor += MapData[index] * GetMonsterKillContribution(i);
			CheckUniqueMonsterChance(DND_MAPEVENT_UZOMBIEMAN + i);
		}

		// infight prevention check on HK and above tier monsters
		if(!CheckMapEvent(DND_MAPEVENT_NOINFIGHTING) && (HasDungeonAttributeVal(DUN_ATTR_NOINFIGHT) != -1 || (i >= MONSTERCLASS_HELLKNIGHT && MapData[index] >= DND_NOINFIGHT_THRESHOLD)))
			AcceptMapEvent(DND_MAPEVENT_NOINFIGHTING);

		TempArray[TARR_MONID][i] = 0;
	}

	int x = MapData[DND_MAPDATA_MONSTERTOTAL];
	if(x > DND_LOOT_MAXMOBTHRESHOLD)
		x = DND_LOOT_MAXMOBTHRESHOLD;
	MapData[DND_MAPDATA_LOOTBONUS] = 	DND_LOOTBONUS_MOBCOUNT * (x < DND_LOOT_MINMOBTHRESHOLD) +
										(x >= DND_LOOT_MINMOBTHRESHOLD) * LinearMap(x, DND_LOOT_MINMOBTHRESHOLD, DND_LOOT_MAXMOBTHRESHOLD, DND_LOOTBONUS_MOBCOUNT, DND_LOOTBONUS_LOWEREDVAL);

	factor += ThingCountName("BossBrain", 0) * DND_BOSSBRAIN_CONTRIB;

	int factor_mons = factor;
	
	factor += ThingCountName("InvulnerabilitySphere2", 0) * DND_INVUL_CONTRIB;

	if(GameType() != GAME_SINGLE_PLAYER) {
		factor += ThingCountName("Stimpack_MP", 0) * DND_STIM_CONTRIB;
		factor += ThingCountName("Medikit_MP", 0) * DND_MEDIKIT_CONTRIB;
	}
	else {
		factor += ThingCountName("Stimpack_SP", 0) * DND_STIM_CONTRIB;
		factor += ThingCountName("Medikit_SP", 0) * DND_MEDIKIT_CONTRIB;
	}
	
	factor += ThingCountName("NewHealthBonus", 0) * DND_HEALTHBONUS_CONTRIB;
	factor += ThingCountName("NewArmorBonus", 0) * DND_ARMORBONUS_CONTRIB;
	factor += ThingCountName("EvilSphere2", 0) * DND_EVIL_CONTRIB;
	factor += ThingCountName("UberSphere2", 0) * DND_MEGA_CONTRIB;
	factor += ThingCountName("SoulSphere3", 0) * DND_SOULSPHERE_CONTRIB;
	factor += ThingCountName("MegaSphere3", 0) * DND_MEGA_CONTRIB;
	factor += ThingCountName("NewBerserk", 0) * DND_BERSERK_CONTRIB;
	factor += ThingCountName("ArmorDropper", 0) * DND_GREENARMOR_CONTRIB;
	factor += ThingCountName("ArmorDropper_HighTier", 0) * DND_BLUEARMOR_CONTRIB;
	
	// get the value -- <0 in case it overflows... somehow.. lol
	MapData[DND_MAPDATA_DIFFICULTY] = factor / DND_MAPDIFF_TIERVAL;
	if(MapData[DND_MAPDATA_DIFFICULTY] > DND_MAXMAPDIFF || MapData[DND_MAPDATA_DIFFICULTY] < 0)
		MapData[DND_MAPDATA_DIFFICULTY] = DND_MAXMAPDIFF;

	CheckOtherMapEvents();

	// to force initialization of punisher tier count on the map
	// if its a dungeon, do a fixed number based on map difficulty
	if(!InformationInLevel[LEVELINFO_ISDUNGEON])
		GetPunisherTierRequirement(0, factor_mons);
	else
		GetPunisherTierRequirement(0, MapData[DND_MAPDATA_DIFFICULTY] * DND_PUNISHER_TIERSCALE_DUNGEON);
}

void CheckOtherMapEvents() {
	if(HasDungeonAttributeVal(DUN_ATTR_NOPAIN) != -1)
		AcceptMapEvent(DND_MAPEVENT_NOPAIN);

	if(HasDungeonAttributeVal(DUN_ATTR_NORIP) != -1)
		AcceptMapEvent(DND_MAPEVENT_NORIP);

	if(HasDungeonAttributeVal(DUN_ATTR_EXTRAFAST) != -1)
		AcceptMapEvent(DND_MAPEVENT_EXTRAFAST);

	if(HasDungeonAttributeVal(DUN_ATTR_GHOST) != -1)
		AcceptMapEvent(DND_MAPEVENT_GHOST);

	if(HasDungeonAttributeVal(DUN_ATTR_CULLENEMIES) != -1)
		AcceptMapEvent(DND_MAPEVENT_CULLINGMONSTERS);

	if(HasDungeonAttributeVal(DUN_ATTR_EXTRASPEED) != -1)
		AcceptMapEvent(DND_MAPEVENT_EXTRASPEED);

	SetupComplete(SETUP_STATE1, SETUP_MAPEVENTS);
}

int CalculateBonus(int bonustype, int mdifficulty) {
	if(bonustype == BONUS_KILL) {
		// add 5% for each difficulty level
		return BONUS_EXP_RATE * (mdifficulty + 1);
	}
	else if(bonustype == BONUS_ITEM) {
		// add 10% credit for each difficulty level
		return BONUS_CREDIT_RATE * (mdifficulty + 1);
	}
	else if(bonustype == BONUS_SECRET) {
		// add budget for each difficulty level + 1
		return BONUS_SECRET_RATE * (mdifficulty + 2);
	}
	else if(bonustype == BONUS_BONUS) {
		return BONUS_SECRET_RATE * 2 * (mdifficulty + 1);
	}
	return 1;
}

void ShowBonusMessage(int bonustype, int y) {
	int bval = CalculateBonus(bonustype, MapData[DND_MAPDATA_DIFFICULTY]);
	SetHudSize(800, 600, 1);
	SetFont("DBIGFONT");
	switch(bonustype) {
		case BONUS_KILL:
			LocalAmbientSound("RPG/KillBonus", 127);
			HudMessage(l:"DND_TEXT_MAXKILLS", d:bval, l:"DND_TEXT_MAXKILLREWARD"; HUDMSG_FADEINOUT, KILLBONUSID, CR_RED, 400.4, y, 4.0, 1.0, 1.0);
		break;
		case BONUS_ITEM:
			LocalAmbientSound("RPG/ItemBonus", 127);
			HudMessage(l:"DND_TEXT_MAXITEMS", d:bval, l:"DND_TEXT_MAXITEMREWARD"; HUDMSG_FADEINOUT, ITEMBONUSID, CR_GOLD, 400.4, y, 4.0, 1.0, 1.0);
		break;
		case BONUS_SECRET:
			LocalAmbientSound("RPG/SecretBonus", 127);
			HudMessage(l:"DND_TEXT_MAXSECRETS", d:bval, l:"DND_TEXT_MAXSECRETREWARD"; HUDMSG_FADEINOUT, SECRETBONUSID, CR_GOLD, 400.4, y, 4.0, 1.0, 1.0);
		break;
		case BONUS_BONUS:
			LocalAmbientSound("RPG/BonusBonus", 127);
			HudMessage(l:"DND_TEXT_MAPTOAST"; HUDMSG_FADEINOUT, BONUSBONUSID, CR_GOLD, 400.4, y, 4.0, 1.0, 1.0);
		break;
	}
}

void DistributeBonus(int bonustype) {
	int bval = 0, temp = 0, i = 0;
	if(bonustype == BONUS_KILL) {
		bval = CalculateBonus(BONUS_KILL, MapData[DND_MAPDATA_DIFFICULTY]);
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(PlayerInGame(i) && isActorAlive(i + P_TIDSTART)) {
				temp = GetActorLevelExperience(i + P_TIDSTART);
				if(temp > bcs::INT_MAX / bval) {
					temp /= 100;
					temp *= bval;
				}
				else
					temp = temp * bval / 100;

				GiveActorInventory(i + P_TIDSTART, "DnD_KillBonusShower", 1);
				GiveActorExp(i + P_TIDSTART, temp);
			}
		}
	}
	else if(bonustype == BONUS_ITEM) {
		bval = CalculateBonus(BONUS_ITEM, MapData[DND_MAPDATA_DIFFICULTY]);
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(PlayerInGame(i) && isActorAlive(i + P_TIDSTART)) {
				temp = GetActorLevelCredits(i + P_TIDSTART) * bval / 100;
				GiveActorInventory(i + P_TIDSTART, "DnD_ItemBonusShower", 1);
				GiveActorCredit(i + P_TIDSTART, temp);
			}
		}
	}
	else if(bonustype == BONUS_SECRET) {
		bval = CalculateBonus(BONUS_SECRET, MapData[DND_MAPDATA_DIFFICULTY]);
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(PlayerInGame(i) && isActorAlive(i + P_TIDSTART)) {
				GiveActorInventory(i + P_TIDSTART, "DnD_SecretBonusShower", 1);
				GiveActorBudget(i + P_TIDSTART, bval);
			}
		}
	}
	else if(bonustype == BONUS_BONUS) {
		for(i = 0; i < MAXPLAYERS; ++i) {
			temp = i + P_TIDSTART;
			if(PlayerInGame(i) && isActorAlive(temp)) {
				// getspawnhealth needs current activator, this wasn't using it
				SetActivator(temp);
				SpawnDrop("LootChest_ForPlayer", 0, 0, i + 1, 0);
				GiveActorInventory(temp, "DnD_BonusBonusShower", 1);
				SetActivator(-1);
			}
		}
	}
}

void SpawnLootboxRewards(int i, int guaranteed_orb = 0) {
	// real value's +1
	--guaranteed_orb;

	int tmp;
	// for orbs
	int plvl = GetActorLevel(i + P_TIDSTART);

	if(random(0, 1.0) <= DND_LOOTBOX_ORBDROPCHANCE1)
		SpawnOrb(i, true, false, GetOrbDropStack(plvl));

	if(random(0, 1.0) <= DND_LOOTBOX_ORBDROPCHANCE2)
		SpawnOrb(i, true, false, GetOrbDropStack(plvl));

	// for tokens -- same likelihood to drop as orbs
	if(random(0, 1.0) <= DND_LOOTBOX_TOKENDROPCHANCE)
		SpawnToken(i, GetOrbDropStack(plvl));

	if(random(0, 1.0) <= DND_LOOTBOX_ARMORDROPCHANCE) {
		// boot and body armor chance is equal
		tmp = random(1, 100);
		if(tmp <= 25)
			SpawnArmorDrop(i, 0, MAX_REGULAR_ILVL, false);
		else if(tmp <= 60)
			SpawnBoot(i, 0, MAX_REGULAR_ILVL);
		else if(tmp <= 85)
			SpawnHelmDrop(i, 0, MAX_REGULAR_ILVL);
		else {
			// class specific spawn -- check if this is the fitting class of the player later here
			SpawnSpecialtyItem(i, 0, MAX_REGULAR_ILVL, false, GetRandomSpecialtyItem());
		}
	}
	else
		SpawnCharm(i, 0, MAX_REGULAR_ILVL, false, random(0, 100));

	
	if(random(0, 1.0) <= DND_LOOTBOX_CHESTKEYDROPCHANCE)
		SpawnChestKey(i);

	if(random(0, 1.0) <= DND_LOOTBOX_FLASKCHANCE)
		SpawnFlask(i, 0);

	if(guaranteed_orb >= 0)
		SpawnSpecificOrb(i, guaranteed_orb, true, true, random(1, 3));
}

// 0 means they are ready
bool PlayersNotReadyForHardcore() {
	int players_notready = 0;
	for(int i = 0; i < MAXPLAYERS; ++i)
		if (PlayerInGame(i) && PlayerCanLoad[i])
			++players_notready;
	return players_notready;
}

void SpawnTalentCapsule() {
	if(GameType() != GAME_SINGLE_PLAYER) {
		for(int i = 0; i < MAXPLAYERS; ++i)
			if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART))
				SpawnDrop("TalentCapsule", 0, 0, i + 1, 1);
	}
	else
		SpawnDrop("TalentCapsule_SP", 0, 0, 1, 1);
}

void SpawnResearchId(int id) {
	if(GameType() != GAME_SINGLE_PLAYER) {
		for(int i = 0; i < MAXPLAYERS; ++i) {
			// spawn this only if this isn't already found by the player
			if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART) && !CheckResearchStatus(id))
				SpawnPlayerDropTargeted(i, "ResearchModule_MP", 24.0, 16, i + 1, id);
		}
	}
	else if(!CheckResearchStatus(id)) // 1 before id is player (0 + 1)
		SpawnPlayerDropTargeted(0, "ResearchModule_MP", 24.0, 16, 1, id);
}

void SpawnAccessory(int pnum) {
	int id;
	if(isSoftorHardcore()) {
		do {
			id = random(1, MAX_ACCESSORY);
		} while(id == DND_REPLACE_THIS_ACCESSORY);
	}
	else
		id = random(1, MAX_ACCESSORY);
	SpawnPlayerDrop(pnum, StrParam(s:"Accessory_", d:id), 0, 0, 0, 0);
}

void UpdateLevelChestLimit() {
	/* 	2^x + 5x/2 + base_amt (10)
		a healthy range of values rewarding difficulty greatly
		
		Very Easy = 10
		Easy = 13
		Medium = 18
		Hard = 24
		Very Hard = 35
	*/
	// the numbers above are different now that we have a different difficulty scale from 0-9 instead of 0-4
	CurrentLevelData[LEVELDATA_MAXCHESTS] = MAX_BASE_CHESTCOUNT + 5 * MapData[DND_MAPDATA_DIFFICULTY] / 2;
}

void HandleChestSpawn(int chance_penalty) {
	if(CurrentLevelData[LEVELDATA_CHESTSPAWNED] > CurrentLevelData[LEVELDATA_MAXCHESTS])
		return;
		
	if(!chance_penalty)
		chance_penalty = 1;

	int r = random(0, 1.0);
	if(r <= CHEST_DROPWEIGHT / chance_penalty) {
		// chest will now spawn, determine type of it here
		++CurrentLevelData[LEVELDATA_CHESTSPAWNED];
		r = random(0, 1.0) * (100 + HasDungeonUpside(DUN_UPSIDE_RARERCHEST)) / 100;
		if(r < SILVERCHEST_DROPWEIGHT)
			SpawnDrop("DNDSilverChest", 0, 0, 0, 0);
		else if(r < GOLDCHEST_DROPWEIGHT)
			SpawnDrop("DNDGoldChest", 0, 0, 0, 0);
		else
			SpawnDrop("DNDBronzeChest", 0, 0, 0, 0);
	}
	else if(r <= LOOTBOX_DROPWEIGHT / chance_penalty)
		SpawnDrop("LootChest", 0, 0, 0, 0);

	// check for merchant spawn now
	if(CheckMapEvent(DND_MAPEVENT_MERCHANTID)) {
		chance_penalty = CurrentLevelData[LEVELDATA_MAXCHESTS];
		if(!chance_penalty)
			chance_penalty = 1;
		TempArray[TARR_MAPEVENTS][DND_MAPEVENT_MERCHANTID] += 1.0 / chance_penalty;

		if(random(0, 1.0) <= TempArray[TARR_MAPEVENTS][DND_MAPEVENT_MERCHANTID]) {
			SpawnMerchant();
			ClearMapEvent(DND_MAPEVENT_MERCHANTID);
		}
	}
}

void HandleChestDrops(int ctype) {
	// the tid of player who opened the chest
	int tid = GetActorProperty(0, APROP_TARGETTID);
	int pnum = tid - P_TIDSTART;

	if(ctype == DND_CHESTTYPE_BRONZE) {
		SpawnOrbForAll(random(1, 3), 1 + random(1, 5) / 5);
		if(RunDefaultDropChance(pnum, 0.5))
			SpawnItemForAll(DND_ITEM_TOKEN);
	}
	else if(ctype == DND_CHESTTYPE_SILVER) {
		SpawnOrbForAll(random(3, 5), 1 + random(1, 10) / 4);
		if(RunDefaultDropChance(pnum, 0.75))
			SpawnItemForAll(DND_ITEM_TOKEN);
		if(RunDefaultDropChance(pnum, 0.5))
			SpawnItemForAll(GetRandomSpecialtyItem());
	}
	else if(ctype == DND_CHESTTYPE_GOLD) {
		SpawnOrbForAll(random(5, 8), 1 + random(1, 10) / 2);
		SpawnItemForAll(DND_ITEM_TOKEN);
		SpawnItemForAll(GetRandomSpecialtyItem());
		SpawnItemForAll(DND_ITEM_BODYARMOR, MAX_REGULAR_ILVL, random(InformationInLevel[LEVELINFO_MINPLAYERLEVEL], InformationInLevel[LEVELINFO_MAXPLAYERLEVEL]));
	}

	// tid used as value here for credit
	tid = random(500, 750) * (2 * GetCVar("dnd_mode") + 1) * (ctype + 1);
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART)) {
			GiveActorCredit(i, tid);
		}
	}

	// 25% chance to spawn a charm that has boosted synergy chance
	if(RunDefaultDropChance(pnum, 0.25))
		SpawnCharmForAll(0, (ctype + 1) * DND_CHARM_SPECIALTYBOOST_BASE);
	
	// common to all chests, an extra orb can drop with 33% chance and another with 20%
	if(RunDefaultDropChance(pnum, 0.33))
		SpawnOrbForAll(1);
	if(RunDefaultDropChance(pnum, 0.2))
		SpawnOrbForAll(1);
	if(RunDefaultDropChance(pnum, 0.2))
		SpawnItemForAll(DND_ITEM_FLASK);
}

Script "DnD Chest Credit Message" (int amt) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber() || !PlayerInGame(ConsolePlayerNumber()) || !amt)
		Terminate;
	Log(s:"\ccCredit pickup : \c[Y5]", d:amt, s:" credits.");
}

// drop boost increases chance for a drop, rarity is for chance for it to be unique
void HandleItemDropsForLoot(int m_id, int drop_boost, int rarity_boost) {
	int tmp;

	// chest droprate check
	int chest_dropchance = DND_LOOTCHEST_DROPRATE;
	bool incursion = IsIncursionMonster(m_id);

	if(InformationInLevel[LEVELINFO_ISDUNGEON]) {
		chest_dropchance = DND_LOOTCHEST_DROPRATE_DUNGEON;
		if(!incursion)
			incursion = random(1, 100) <= DND_DUNGEON_INCURSION_ODDS;
	}

	drop_boost += 100;
	chest_dropchance = chest_dropchance * drop_boost / 100;

	bool dropped_chest = false;

	for(int i = 0; i < MAXPLAYERS; ++i) {
		// run each player's chance, drop for corresponding player only
		if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART)) {
			// GetDropChance returns fixed value
			int p_chance = GetDropChance(i);
			int quant = GetPlayerDropQuantity(i) * drop_boost / 100;

			// this is the effective weight of nothing dropping -- if we pass this roll that means a loot drop can occur and we can check successive rolls after
			int count = GetAdjustedNothingWeight(p_chance, drop_boost);
#ifdef ISDEBUGBUILD
			if(1) {
#else
			if(random(1, count + LootTables.monster_loot_weight_sum) <= LootTables.monster_loot_weight_sum) {
#endif
				// count how many items to spawn with player's item quant
				count = 0;
				while(quant > 0 && MonsterProperties[m_id].rng_vals[count] <= quant) {
					// for subsequent chance rolls that pass the quant, guaranteed drop them
					tmp = PickFromAliasTable(LootTables.monster_drop_table);
					SpawnLootFromDropTableIndex(i, rarity_boost, tmp, m_id, incursion);

					quant -= 1.0;
					++count;
					if(count > LAST_MON_RNG_INDEX)
						count = LAST_MON_RNG_INDEX;
				}
			}

			if(!dropped_chest && IsLootChestDroppingMonster(m_id) && RunPrecalcDropChance(p_chance, chest_dropchance, m_id, DND_MON_RNG_8)) {
				SpawnDrop("LootChest", 0, 0, 0, 0);
				dropped_chest = true;
			}

			// made this not tied to player's droprate
			tmp = MonsterProperties[m_id].id;
			if(isLegendaryMonster(tmp) && random(0, 1.0) <= DND_LEGENDARY_ITEMDROPRATE)
				HandleLegendaryMonsterDrop(tmp, i);
		}
	}
}

void SpawnLootFromDropTableIndex(int pnum, int rarity_boost, int drop_id, int m_id, bool is_incursion_monster = false) {
	// allow final dungeon bosses to be more rewarding potentially
	int cap = MAX_REGULAR_ILVL;
	if(InformationInLevel[LEVELINFO_ISDUNGEON] && (MonsterProperties[m_id].flags & DND_MONFLAG_ISDUNGEONBOSS))
		cap = MAX_BOSS_ILVL;

	int max_level = MonsterProperties[m_id].level;
	if(max_level > cap)
		max_level = cap;

	switch(drop_id) {
		case DND_MONSTERLOOT_CHARM:
			if(is_incursion_monster && RollIncursionItemChance())
				SpawnCharmWithMods(pnum, PickRandomIncursionMod(), -1, -1, max_level);
			else
				SpawnCharm(pnum, rarity_boost, max_level);
		break;
		case DND_MONSTERLOOT_BODYARMOR:
			if(is_incursion_monster && RollIncursionItemChance())
				SpawnArmorWithMods(pnum, PickRandomIncursionMod(), -1, -1, max_level);
			else
				SpawnArmorDrop(pnum, rarity_boost, max_level, false, m_id);
		break;
		case DND_MONSTERLOOT_HELM:
			if(is_incursion_monster && RollIncursionItemChance())
				SpawnHelmWithMods(pnum, PickRandomIncursionMod(), -1, -1, max_level);
			else
				SpawnHelmDrop(pnum, rarity_boost, max_level);
		break;
		case DND_MONSTERLOOT_BOOT:
			if(is_incursion_monster && RollIncursionItemChance())
				SpawnBootWithMods(pnum, PickRandomIncursionMod(), -1, -1, max_level);
			else
				SpawnBoot(pnum, rarity_boost, max_level);
		break;
		
		// they all spawn a specialty item
		case DND_MONSTERLOOT_SPECIALTY:
			drop_id = GetActorPlayerClass(pnum + P_TIDSTART);
			if(random(1, 100) <= DND_SPECIALTY_BIAS_CHANCE)
				SpawnSpecialtyItem(pnum, rarity_boost, max_level, false, FIRST_SPECIALTY_ITEM_TYPE + drop_id);
			else
				SpawnSpecialtyItem(pnum, rarity_boost, max_level, false, GetRandomSpecialtyItem());
		break;

		case DND_MONSTERLOOT_FLASK:
			SpawnFlask(pnum, rarity_boost);
		break;
		case DND_MONSTERLOOT_ORB:
			SpawnOrb(pnum, true, false, GetOrbDropStack(MonsterProperties[m_id].level));
		break;
		case DND_MONSTERLOOT_CHESTKEY:
			SpawnChestKey(pnum);
		break;

		case DND_MONSTERLOOT_TOKEN:
			SpawnToken(pnum, GetOrbDropStack(MonsterProperties[m_id].level));
		break;

		case DND_MONSTERLOOT_DUNGEONKEY:
			// don't spawn dungeon keys within dungeons as natural loot drop from monsters
			if(!InformationInLevel[LEVELINFO_ISDUNGEON])
				SpawnDungeonKey(pnum);
		break;
	}
}

void HandleLegendaryMonsterDrop(int leg_mon_id, int pnum) {
	int unique_id = -1;
	switch(leg_mon_id) {
		case MONSTER_DREAMINGGOD:
			unique_id = UITEM_DREAMINGGODIRE;
		break;
		case MONSTER_TORRASQUE:
			unique_id = UITEM_STEELBARK;
		break;
		case MONSTER_MORDECQAI:
			unique_id = UITEM_ELEMENTALHARMONY;
		break;
		case MONSTER_GODSLAYER:
			unique_id = UITEM_HEATBREAKER;
		break;
		case MONSTER_GOLGOTH:
			unique_id = UITEM_THORNVEIN;
		break;
	}
	SpawnUniqueCharm(pnum, unique_id);
}

int ApplyCreditFactor(int p_tid, int exp_base) {
	return exp_base / (DND_CREDITGAIN_FACTOR + CheckActorInventory(p_tid, "Level") / DND_CREDITGAIN_PERLEVELREDUCE);
}

void HandleCreditExp_Regular(int this, int target, int m_id) {
	int expshare = GetCVar("dnd_sharexp");
	int creditshare = GetCVar("dnd_sharecredit");
	
    // decide how exp/credit base is calculated
	int exptemp = MonsterProperties[m_id].gain;
	int credtemp = ApplyCreditFactor(target, exptemp);
    int pnum = 0, i = MonsterProperties[m_id].level;
	
	if(credtemp < DND_MIN_CREDIT)
		credtemp = DND_MIN_CREDIT;
	
	int pcount = Clamp_Between(PlayerCount(), 1, DND_MAX_SHARE);
	int expscale = Clamp_Between(GetCVar("dnd_exp_scale"), 1, EXP_SCALE_MAX);
	int creditscale = Clamp_Between(GetCVar("dnd_credit_scale"), 1, CREDIT_SCALE_MAX);
	int addone = 0;

	// if full share is on we won't divide by player count, but if it isn't we will
	if(!GetCVar("dnd_fullshare")) {
		exptemp = (exptemp * expscale) / pcount;
		if(exptemp < MIN_EXP_GAIN)
			exptemp = MIN_EXP_GAIN;
		credtemp = (credtemp * creditscale) / pcount;
		if(credtemp < MIN_CREDIT_GAIN)
			credtemp = MIN_CREDIT_GAIN;
	}
	else {
		exptemp *= expscale;
		credtemp *= creditscale;
	}
	
	// from here on out the scale variables and pcount are useless
	pcount = GetCVar("dnd_gainrange");
	
	// reuse old variable -- we dont need killerplayer w.e info
	HandleMonsterTemporaryWeaponDrop(this, m_id, target - P_TIDSTART, false);

	if(expshare || creditshare) {
		for(i = 0; i < MAXPLAYERS; ++i) {
			expscale = 0;
			creditscale = 0;
			pnum = P_TIDSTART + i;
			if(IsActorAlive(pnum) && pnum != target) { // dont give twice
				// check if range flag is on, if it is check the range
				addone = AproxDistance(GetActorX(target) - GetActorX(pnum), GetActorY(target) - GetActorY(pnum)) >> 16 <= pcount;
				// The "ignore range" clause was Wisdom and Greed mastery before the old perk system was
				// retired; Cunning's Deep Wisdom and Endless Greed are the same effect and now supply it.
				//
				// Read off i, NOT the killer: the perk belongs to the player being paid, so a bystander
				// with Deep Wisdom collects from across the map whoever landed the kill.
				if(expshare && exptemp && (!GetCVar("dnd_gainonlyinrange") || addone || PlayerModData[i].vals[PSTAT_EXPGAIN_ANYRANGE]))
					expscale = RewardActorExp(pnum, exptemp);
					
				if(creditshare && credtemp && (!GetCVar("dnd_gainonlyinrange") || addone || PlayerModData[i].vals[PSTAT_CREDITGAIN_ANYRANGE]))
					creditscale = RewardActorCredit(pnum, credtemp);
				
				// if something could be provided, show it to user
				if(expscale || creditscale) {
					SetActivator(pnum);
					ACS_NamedExecuteWithResult("DND Show Kill Digits", pnum, expscale, creditscale);
				}
			}
		}
	}
	
	// now give target his stuff (if expshare or creditshare, one of them was not on the target would not receive it in the loop)
	expscale = RewardActorExp(target, exptemp);
	creditscale = RewardActorCredit(target, credtemp);
	SetActivator(target);
	ACS_NamedExecuteWithResult("DND Show Kill Digits", target, expscale, creditscale);
}

// A kill the player did not make. It used to share exp and credit with anyone who had mastered the
// Wisdom or Greed perks, and every part of that is gone: the two counters it compared against were
// never written after the retirement, expshare and creditshare were zeroed and never raised, the
// per-player scale values were zeroed at the top of each iteration so the kill digits never drew,
// and the loop's early out was therefore true on its first pass. The exp and credit it computed were
// never spent. What survived all of that is the temp weapon drop, which is the whole function now.
//
// Renamed to match: nothing here checks a mastery or hands out credit or exp.
void HandleNonPlayerKillDrop(int this, int m_id) {
	HandleMonsterTemporaryWeaponDrop(this, m_id, -1, true);
}

// loc_tid is a potential drop location
void HandleLootDrops(int tid, int target, int loc_tid = -1) {
	if(!IsPlayer(target))
		return;

	int m_id = tid - DND_MONSTERTID_BEGIN;
	int pnum = target - P_TIDSTART;
	int temp;
	int p_chance = GetDropChance(pnum);
	
	// Perception / Salvager. This is the drop Munitionist mastery used to make, restored with the
	// perk supplying the chance instead of a constant.
	if((temp = PlayerModData[pnum].vals[PSTAT_AMMOTOKEN_CHANCE]) &&
		RunPrecalcDropChance(p_chance, (temp << 16) / 100, m_id, DND_MON_RNG_1))
		SpawnPlayerDrop(pnum, "DnD_AmmoToken", 24.0, 16, 0, 0);
	
	// research drop
	if(!GetCVar("dnd_allresearchesfound")) {
		// make it less likely to drop
		// addone is the chance here (reusing old variables)
		// note: this doesnt seem to be contributing towards anything...
		//temp = random(0, DND_RESEARCH_DROPMULT * DND_RESEARCH_MAX_CHANCE);
		//printbold(f:CVarValues[DND_CVAR_RESEARCHDROPRATE], s:" ", d:MonsterProperties[m_id].droprate, s:" ", f:CVarValues[DND_CVAR_RESEARCHDROPRATE] * MonsterProperties[m_id].droprate / 100);
/*#ifdef ISDEBUGBUILD
		if(1)
#else*/
		if(RunPrecalcDropChance(p_chance, CVarValues[DND_CVAR_RESEARCHDROPRATE] * MonsterProperties[m_id].droprate / 100, m_id, DND_MON_RNG_2))
//#endif
		SpawnResearch(pnum);
	}
	
	HandleItemDropsForLoot(m_id, MonsterProperties[m_id].droprate, MonsterProperties[m_id].rarity_boost);
	
	// accessory drops (accept only from cyber and spider masterminds)
	if(
		IsBossTID(tid) && 
		random(0, 1.0) <= CVarValues[DND_CVAR_ACCESSORYDROPRATE] &&
		GetAveragePlayerLevel() >= CVarValues[DND_CVAR_ACCESSORYLEVEL]
	)
	{
		// we can drop the spawner
		SpawnAccessory(pnum);
	}
	
	// soul ammo drop -- considers ability - soulstealer as well
	if
	(
		CanDropSoulAmmoTID(tid) && 
		RunPrecalcDropChance(
			p_chance, 
			(1 + CheckActorInventory(tid, "MagicCausedDeath")) * (DND_SOULAMMO_DROPRATE + DND_SOULAMMO_STEALERUPGRADE * CheckActorInventory(target, "Ability_SoulStealer")) * MonsterProperties[m_id].droprate / 100, 
			m_id, 
			DND_MON_RNG_4
		)
	)
	{
		if(random(1, 100) <= DND_SOULAMMO_SMALLCHANCE)
			SpawnPlayerDrop(pnum, "SoulsDrop", 24.0, 16, 0, 0);
		else
			SpawnPlayerDrop(pnum, "LargeSoulsDrop", 24.0, 16, 0, 0);
		TakeActorInventory(tid, "MagicCausedDeath", 1);
	}
	
	// doomguy drop -- we dont multiply with MonsterProperties[m_id].droprate here as it's for loot only
	if(HasActorClassPerk_Fast(target, DND_PLAYER_DOOMGUY, 2) && RunPrecalcDropChance(p_chance, DND_DOOMGUY_DROPCHANCE, m_id, DND_MON_RNG_3)) {
		temp = MonsterProperties[m_id].maxhp;
		SpawnPlayerDrop(pnum, "Doomguy_DemonSoul", 24.0, 16, temp & 0xFFFF, temp >> 16);
	}
		
	if(CheckActorInventory(tid, "BookofDeadCausedDeath")) {
		if(random(1, 100) <= DND_SOULAMMO_SMALLCHANCE)
			SpawnPlayerDrop(pnum, "SoulsDrop", 24.0, 16, 0, 0);
		else
			SpawnPlayerDrop(pnum, "LargeSoulsDrop", 24.0, 16, 0, 0);
		Spawn("SoulEffectSpawner", GetActorX(0), GetActorY(0), GetActorZ(0));
		TakeActorInventory(tid, "BookofDeadCausedDeath", 1);
	}
	
	if(CheckActorInventory(target, "Ability_HeartSeeker") && RunDefaultDropChance(pnum, CHANCE_HEART * MonsterProperties[m_id].droprate / 100))
		SpawnPlayerDrop(pnum, "DemonHeartPickup", 24.0, 16, 0, 0);
}

int GetPVelocity(void) {
	int vel;
	int x = GetActorVelX(0);
	int y = GetActorVelY(0);
	int angle = VectorAngle(x, y);
   
	if(((angle + 0.125) % 0.5) > 0.25)
		vel = FixedDiv(y, sin(angle));
	else
		vel = FixedDiv(x, cos(angle));
	return vel >> 16;
}

int GetWeaponSlotFromFlag(int flags) {
	// the first 14 bits are unnecessary
	flags >>= DND_SHIFTBITS_FOR_SLOTFROMFLAG;
	for(int i = 0; i < 32; ++i)
		if(IsSet(flags, i))
			return i;
	return 0;
}

void CheckMonsterMapEventBuffs(int m_id) {
	int temp;
	if((temp = HasDungeonAttributeVal(DUN_ATTR_FORTIFIED)) != -1 && random(1, 100) <= temp)
		SetEliteFlag(m_id, DND_FORTIFIED, true);

	if(CheckMapEvent(DND_MAPEVENT_GHOST))
		SetEliteFlag(m_id, DND_GHOST, true);

	if(CheckMapEvent(DND_MAPEVENT_NOPAIN))
		SetEliteFlag(m_id, DND_NOPAIN, true);

	if(CheckMapEvent(DND_MAPEVENT_NORIP))
		SetEliteFlag(m_id, DND_HARDENED_SKIN, true);

	if(CheckMapEvent(DND_MAPEVENT_EXTRAFAST))
		SetEliteFlag(m_id, DND_HASTE, true);

	if(CheckMapEvent(DND_MAPEVENT_EXTRASPEED))
		SetEliteFlag(m_id, DND_EXTRASPEED, true);
}

int ScaleMonster(int tid, int m_id, int pcount, int realhp, bool isSummoned, int hp_mult) {
	int base = realhp * hp_mult;
	int add = 0, level = 1, low, high, temp;

	if(hp_mult > 1)
		MonsterProperties[m_id].flags |= DND_MONFLAG_ISDUNGEONBOSS;

	// if we are in a dungeon of specific level, apply it to the monsters
	if(DungeonInformation.level == -1)
		level = InformationInLevel[LEVELINFO_PLAYERLEVELATSTART] / pcount;
	else
		level = DungeonInformation.level;

	// ensure minions use master's level -- do so only if its summoned, boss tier monsters have tids on the spawners that can mess this up during mapload!!!
	if(GetActorProperty(0, APROP_MASTERTID) && isSummoned)
		level = MonsterProperties[GetActorProperty(0, APROP_MASTERTID) - DND_MONSTERTID_BEGIN].level;
	else if(GetCVar("dnd_randomize_levels")) {
		if(!IsUniqueBossMonster(m_id)) {
			low = Clamp_Between(GetCVar("dnd_monsterlevel_low"), 0, 50);
			high = Clamp_Between(GetCVar("dnd_monsterlevel_high"), 0, 50);
			// first 10 levels the monsters shouldn't really get super high levels
			if(level < 10)
				high = Clamp_Between(high, 0, high / 2);
			// give some sort of variety in the levels
			// subtract level from avg half the time
			if(random(0, 1)) {
				temp = low;
				low = -high;
				high = -temp;
			}
			level += random(low, high);
		}
		else // level of unique boss is always level of highest player
			level = InformationInLevel[LEVELINFO_MAXPLAYERLEVEL];
	}
	if(GetCVar("dnd_monsterlevel_behind"))
		level = Clamp_Between(level, 1, InformationInLevel[LEVELINFO_PLAYERLEVELATSTART] / pcount);
	level = Clamp_Between(level, 1, GetCVar("dnd_maxmonsterlevel"));
	if(level > 1) {
		add = GetMonsterHPScaling(m_id, level);
		
		// % increase per player adding
		if(GetCVar("dnd_playercount_scales_monsters"))
			add += DND_MONSTERHP_PLAYERSCALE * (Clamp_Between(pcount - 1, 0, DND_MAX_PLAYERHPSCALE));

		// Folded into the PERCENT, where the numbers are small. Applied after `add` became a health
		// figure it had nothing to spare. Identical arithmetic: base*pct*(100+t)/10000.
		if((temp = HasDungeonAttributeVal(DUN_ATTR_EXTRAHP)) != -1)
			add = add * (100 + temp) / 100;

		// Bounds the percent, then multiplies without forming an intermediate that can wrap. The old
		// `add = base * add / 100` wrapped HERE, and the INT_MAX check that followed it was reading a
		// number that had already gone negative -- see ApplyHPScale.
		add = ApplyHPScale(base, add);
	}

	if(CheckMapEvent(DND_MAPEVENT_NOINFIGHTING)) {
		GiveInventory("MonsterInfightRemover", 1);
		SetActorProperty(0, APROP_SPECIES, "Dummy");
	}

	MonsterProperties[m_id].basehp = base;
	MonsterProperties[m_id].maxhp = base + add;
	MonsterProperties[m_id].level = level;

	//printbold(s:"base ", d:base, s: " add ", d:add, s: " level ", d:level);

	// map event buffs
	CheckMonsterMapEventBuffs(m_id);

	// init to false
	//MonsterProperties[m_id].spawnsIncursionMarker = false;

	return base + add;
}

void ActivateKillingSpree() {
	// if a kill streak wasn't running, run it now
	if(!CheckInventory("DnD_SpreeCountdownRunning")) {
		GiveInventory("DnD_SpreeCountdownRunning", 1);
		ACS_NamedExecuteAlways("DnD Killing Spree Timer", 0);
	}
	// spree count was given, so a possible spree may be in reach
	if(CheckInventory("DnD_SpreeTimer")) {
		GiveInventory("DnD_MultikillCounter", 1);
		
		// punisher perk -- be on cruel or more
		int temp = CheckInventory("DnD_MultikillCounter") + 1;
		if(temp / DND_SPREE_PER >= 1) {
			// punisher perks
			if(HasClassPerk_Fast(DND_PLAYER_PUNISHER, 1)) {
				if(!CheckInventory("Punisher_Perk1_MoveSpeed")) {
					int wepid = GetCurrentWeaponID();
					if(GetSlotOfWeapon(wepid) != 9) {
						if(Weapons_Data[wepid].ammo_name1 != "" && Weapons_Data[wepid].ammo_name1 != "Souls") {
							GiveInventory(Weapons_Data[wepid].ammo_name1, GetAmmoCapacity(Weapons_Data[wepid].ammo_name1) / 10);
							LocalAmbientSound("items/ammo", 90);
						}
						if(Weapons_Data[wepid].ammo_name2 != "" && Weapons_Data[wepid].ammo_name2 != "Souls") {
							GiveInventory(Weapons_Data[wepid].ammo_name2, GetAmmoCapacity(Weapons_Data[wepid].ammo_name2) / 10);
							LocalAmbientSound("items/ammo", 90);
						}
					}
				}
				GiveInventory("Punisher_Perk1_MoveSpeed", 1);
			}
		}
		else if(temp / DND_HALF_SPREE_PER >= 1 && GetArmorID() == BODYARMOR_RAVAGER) {
			// ravager armor activation on killing spree -- no tint on this one, so it
			// goes straight to the buff table rather than through "DnD Give Buff"
			HandlePlayerBuffAssignment(PlayerNumber(), 0, BTI_RAVAGER_POWER);
		}
	}
	// give spree counter
	SetInventory("DnD_SpreeTimer", GetAmmoCapacity("DnD_SpreeTimer"));
}

void ResetPlayerScriptChecks() {
	for(int i = 0; i < MAX_SCRIPT_TRACK; ++i)
		for(int j = 0; j < MAXPLAYERS; ++j)
			PlayerScriptsCheck[i][j] = false;
}

void HandleMonsterTemporaryWeaponDrop(int this, int id, int pnum, bool guaranteed = false) {
	id = MonsterProperties[id].id;
	switch(id) {
		case MONSTER_BLOODFIEND:
		case MONSTER_RAVAGER:
		case MONSTER_LURKER:
			if(guaranteed || RunDefaultDropChance(pnum, TEMPWEP_BLOODFIENDSPINE_DROPCHANCE))
				SpawnPlayerDropAtActor(pnum, this, TemporaryWeaponData[DND_TEMPWEP_BLOODFIENDSPINE][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_VULGAR:
			if(guaranteed || RunDefaultDropChance(pnum, TEMPWEP_VENOM_DROPCHANCE))
				SpawnPlayerDropAtActor(pnum, this, TemporaryWeaponData[DND_TEMPWEP_VENOM][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_CHAINGUNGENERAL:
			if(guaranteed || RunDefaultDropChance(pnum, TEMPWEP_NAILGUN_DROPCHANCE))
				SpawnPlayerDropAtActor(pnum, this, TemporaryWeaponData[DND_TEMPWEP_HEAVYNAILGUN][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_DEATHKNIGHT:
		case MONSTER_HORSHACKER:
			if(guaranteed || RunDefaultDropChance(pnum, TEMPWEP_SOULRENDER_DROPCHANCE))
				SpawnPlayerDropAtActor(pnum, this, TemporaryWeaponData[DND_TEMPWEP_SOULRENDER][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_CORPULENT:
			if(guaranteed || RunDefaultDropChance(pnum, TEMPWEP_HFCANNON_DROPCHANCE))
				SpawnPlayerDropAtActor(pnum, this, TemporaryWeaponData[DND_TEMPWEP_HELLFORGECANNON][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_DARKSERVANT:
			if(guaranteed || RunDefaultDropChance(pnum, TEMPWEP_DARKGLOVES_DROPCHANCE))
				SpawnPlayerDropAtActor(pnum, this, TemporaryWeaponData[DND_TEMPWEP_DARKGLOVES][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
	}
}

void ApplyRandomCurse(int player_tid, int monster_tid) {
	// nasty rushed code here, clean up later with improved curse system
	int curse_id = 0;
	int max_tries = 0;
	do {
		curse_id = random(DND_DEBUFF_BEGIN, DND_DEBUFF_END);
		++max_tries;
	} while(!IsCurse(curse_id) && max_tries < 20);

	int prev_activator = ActivatorTID();
	SetActivator(monster_tid);
	ACS_NamedExecuteWithResult("DnD Give Buff", curse_id);
	SetActivator(prev_activator);
}

void HandleRuination(int this, int target) {
	TakeInventory("RuinationStacks", RUINATION_MAX_STACKS);
	PlaySound(this, "Elite/RuinationTrigger", CHAN_5, 1.0);
	GiveInventory("RuinBlend", 1);
	GiveInventory("RuinationHardDebuff", 1);
}

bool IsEliteException(int m_id, int monster_type) {
	// used to contain HasMonsterTrait(m_id, DND_LEGENDARY) as well
	return 	CheckFlag(0, "FRIENDLY") || isUniqueBossMonster(m_id) || HasMonsterTrait(m_id, DND_SUMMONED) || CheckInventory("DnD_EliteException") ||
			monster_type == MONSTER_PHANTASM || monster_type == MONSTER_WRAITH || monster_type == MONSTER_HADESSPHERE || monster_type == MONSTER_UNDEADPRIESTGHOST;
}

void HandleUniqueDeath(int p_actor, int unique_id, int level) {
	int pnum = p_actor - P_TIDSTART;
	switch(unique_id) {
		case MONSTER_TERON:
			// spawn some budget
			SpawnForced("BudgetDropper_Medium", GetActorX(0), GetActorY(0), GetActorZ(0) + 16.0);
			SpawnForced("BudgetDropper_Medium", GetActorX(0), GetActorY(0), GetActorZ(0) + 16.0);
			SpawnForced("BudgetDropper_Medium", GetActorX(0), GetActorY(0), GetActorZ(0) + 16.0);
		break;
		case MONSTER_GANT:
			if(!random(0, 2))
				SpawnArmor(pnum, 0, -BODYARMOR_SYNTHMETAL);
		break;
		case MONSTER_BRONN:
			if(!random(0, 2))
				SpawnHelm(pnum, 0, HELMS_SYNTHMETAL);
		break;
		case MONSTER_VAAJ:
			// drops vaaj influenced charm: explosives ignore resists mod guaranteed
			if(!random(0, 5))
				SpawnCharmWithMods_ForAll(INV_ESS_VAAJ);
		break;
		case MONSTER_REMUS:
			// drops armor and gives research for it: lightning coil - super lightning protection, 300, 40% damage reduction. When hurt fires lightning bolts.
			if(!random(0, 2))
				SpawnArmor(pnum, 0, -BODYARMOR_LIGHTNINGCOIL);
		break;
		case MONSTER_SSRATH:
			// ssrath influence: soul type weapons penetrate % magic resistance
			if(!random(0, 2))
				SpawnCharmWithMods_ForAll(INV_ESS_SSRATH);
		break;
		case MONSTER_HOLLOWSHELL:
			// hollow orb: adds an extra mod to a non-unique charm even if it's at its limit (at most +1 of its current limit)
			SpawnSpecificOrbForAll(DND_ORB_HOLLOW, 1, GetOrbDropStack(level));
		break;
		case MONSTER_OMNISIGHT:
			// omnisight influence: large accuracy, % increased accuracy rating
			if(!random(0, 2))
				SpawnCharmWithMods_ForAll(INV_ESS_OMNISIGHT, INV_ESS_OMNISIGHT2);
		break;
		case MONSTER_CHEGOVAX:
			// chegovax influence: Ignite damage on enemies increases every tic %
			if(!random(0, 2))
				SpawnCharmWithMods_ForAll(INV_ESS_CHEGOVAX);
		break;
		case MONSTER_ONIMUZ:
			// Phantasmal Orb: grants used weapon ability to hit ghosts but do 25% less damage overall.
			SpawnSpecificOrbForAll(DND_ORB_PHANTASMAL, 1, GetOrbDropStack(level));
		break;
		case MONSTER_HARKIMONDE:
			// harkimonde influence: Attacks have chance to ignore shields.
			if(!random(0, 2))
				SpawnCharmWithMods_ForAll(INV_ESS_HARKIMONDE);
		break;
		case MONSTER_LESHRAC:
			// leshrac influence: poison damage tics twice as fast
			if(!random(0, 2))
				SpawnCharmWithMods_ForAll(INV_ESS_LESHRAC);
		break;
		case MONSTER_KRULL:
			// krull influence: % chance explosives trigger a second time
			if(!random(0, 2))
				SpawnCharmWithMods_ForAll(INV_ESS_KRULL);
		break;
		case MONSTER_THORAX:
			// thorax influence: Homing projectiles can't be reflected.
			if(!random(0, 2))
				SpawnCharmWithMods_ForAll(INV_ESS_THORAX);
		break;
		case MONSTER_ZRAVOG:
			// zravog influence: Occult attacks reduce enemy magic resistance by 2-5% per hit for 3 seconds, up to 5 stacks.
			if(!random(0, 2))
				SpawnCharmWithMods_ForAll(INV_ESS_ZRAVOG);
		break;
		case MONSTER_ERYXIA:
			// eryxia influence: Frozen enemies take % increased cold damage.
			if(!random(0, 2))
				SpawnCharmWithMods_ForAll(INV_ESS_ERYXIA);
		break;
		case MONSTER_ABAXOTH:
			// Assimilation Orb: Assimilates a chosen charm into another, merging them unpredictably, randomly taking modifiers from both charms. Can have up to 1 additional modifier.
			SpawnSpecificOrbForAll(DND_ORB_ASSIMILATION, 1, GetOrbDropStack(level));
		break;
	}
}

int GetAveragePlayerLevel() {
	int temp = InformationInLevel[LEVELINFO_PLAYERCOUNTATSTART];
	if(temp < 1)
		temp = 1;
	return InformationInLevel[LEVELINFO_PLAYERLEVELATSTART] / temp;
}

void ClearLingeringBuffs(int pnum) {
	TakeInventory("MenuFreeze", 1);
	TakeInventory("DnD_PromptLocked", 1);
	SetInventory("AllMapOnlyOnce", 0);

	TakeInventory("DnD_RecoveringStamina", 1);
	TakeInventory("DnD_StaminaLocked", 1);
	TakeInventory("DnD_StaminaDepleted", 1);
	SetInventory("DnD_Parrying", 0);
	TakeInventory("DnD_ParryCooldown", 1);

	SetInventory("DnD_FrenzyChargeCount", 0);
	SetInventory("DnD_EnduranceChargeCount", 0);
	SetInventory("DnD_PowerChargeCount", 0);

	SetInventory("Punisher_Perk5_MoveSpeed", 0);
	SetInventory("Punisher_Perk50_Counter", 0);
	SetInventory("Punisher_Perk50_DamageBonus", 0);
	SetInventory("Punisher_Perk50_Tiers", 0);

	SetInventory("Berserker_DamageTracker", 0);
	SetInventory("Berserker_DamageTimer", 0);
	SetInventory("Berserker_HitTracker", 0);
	SetInventory("Berserker_HitTimer", 0);
	SetInventory("Berserker_NoRoar", 0);
	SetInventory("Berserker_Perk60_HitCounter", 0);
	SetInventory("ReceivedDialogID", 0);
	SetInventory("DarkWanderer_Artifact", 0);

	SetInventory("LifeStealAmount", 0);
	SetInventory("LifestealScriptRunning", 0);

	SetInventory("Hobo_ShotgunFrenzyTimer", 0);

	SetInventory("Trickster_SwapCount", 0);

	SetInventory("Marine_DamageReduction_Timer", 0);
	SetInventory("Marine_Perk50_DamageDealt", 0);
	SetInventory("Marine_Perk50_Cooldown", 0);
	SetInventory("Marine_Perk50_DamageTaken", 0);

	SetInventory("Cyborg_InstabilityStack", 0);
	SetInventory("Cyborg_Instability_Timer", 0);
	SetInventory("Cyborg_NoAnim", 0);
	SetInventory("EShieldChargeNow", 0);

	SetInventory("DaemonicBarrier", 0);
	SetInventory("DaemonicBarrierTimer", 0);

	// rework this when we add persistent pets
	SetInventory("PetCounter", 0);
	
	// some buffs from weapons
	SetInventory("SniperZoomTimer", 0);
	SetInventory("DnD_HandgunMoreCritShots", 0);

	// debuffs
	SetInventory("RuinationStacks", 0);
	SetInventory("DnD_ChillFXRunning", 0);
	SetInventory("DnD_FreezeFXRunning", 0);
	SetInventory("DnD_OverloadFXRunning", 0);
	ResetPlayerBuffs(pnum);

	if(CheckInventory("Wanderer_Ascended"))
		UndoWandererAscension();

	ClearFlaskStates(pnum);
}

void RestorePersistentBuffs(int pnum) {
	// restore these buffs though -- Max charge of types
	if(PlayerModData[pnum].vals[PSTAT_EX_COUNTASHAVINGMAXCHARGEOF]) {
		HandlePlayerBuffAssignment(pnum, 0, BTI_FRENZYCHARGE + ReadPlayerModExtra(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF), 0);
	}
}

void SyncResearchInvestments(int pnum) {
	for(int i = 0; i < MAX_RESEARCHES; ++i) {
		if(ResearchInvestments[pnum][i])
			ACS_NamedExecuteWithResult("DnD Menu Investment Sync", pnum, i, ResearchInvestments[pnum][i]);
	}
}

void PostPlayerLoadRoutine(int pnum) {
	// Load period is finished, now check for level stats
	PlayerDatabaseState[pnum][PLAYER_TRANSFERSTATE] = false;
	TakeInventory("DnD_PDead", 1);
	SpawnedChests = 1;
	SetInventory("CanLoad", 0); //Usually it is 0 at this point, but make sure it is anyways.
	PlayerCanLoad[pnum] = 0;
	
	// we can synchronize the research investments now, load period etc. all finished
	SyncResearchInvestments(pnum);
	
	// force sync client stuff
	GiveInventory("DnD_SyncMe", 1);
	ACS_NamedExecuteAlways("DnD Force Sync Player Bonuses", 0);

	// scripts for berserker in case they have remaining buffs like this, restart the timer scripts
	if(CheckInventory("Berserker_DamageTracker"))
		ACS_NamedExecuteAlways("DnD Berserker Perk20", 0);

	if(CheckInventory("Berserker_HitTracker"))
		ACS_NamedExecuteAlways("DnD Berserker Perk50 Timer", 0, 0);
}

void UnfreezePlayer() {
	SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
	SetPlayerProperty(0, 0, PROP_INVULNERABILITY);
	TakeInventory("P_Frozen", 1);
	GiveInventory("DontAttackRemove", 1);
}

void IncrementPlayerLifetimeKills() {
	// this is the kills in level
	GiveInventory("DnD_Kills", 1);
	
	// we store 3 variables: billions, millions and thousands
	// for every million, we reduce thousands to 1 mil - X, and increment millions by 1, and similar or billions (we subtract 1000 instead from millions)
	int ltk = CheckInventory("DnD_LifeTimeKills");
	int ltk_m = CheckInventory("DnD_LifeTimeKills_Millions");
	SetInventory("DnD_LifeTimeKills_Billions", CheckInventory("DnD_LifeTimeKills_Billions") + (ltk_m + 1) == 1000);
	SetInventory("DnD_LifeTimeKills_Millions", (ltk_m + (ltk + 1 == 1000000)) % 1000);
	SetInventory("DnD_LifeTimeKills", (ltk + 1) % 1000000);
}

void CheckUniqueBossPossibility(int tid, int m_id) {
	if(isUniqueBossMonster(m_id)) {
		DungeonBossData[BOSSDATA_TID] = tid;

		// sync to cs
		ACS_NamedExecuteWithResult("DnD Register Unique Boss", tid, MonsterProperties[m_id].id, MonsterProperties[m_id].maxhp, MonsterProperties[m_id].level);
		ACS_NamedExecuteWithResult("DnD Constant Unique Boss Sync", tid);
	}
}

// checks end of level, if it is and the gamemode is hard/softcore we save players
// called in death or spectate case -- checked with a variable
void CheckEOL(bool isSpectate, int game_mode = -1) {
	if(game_mode == -1)
		game_mode = GetCVar("dnd_mode");
		
	//Log(s:"check hardcore ", d:HardcoreSet, s: " ", d:game_mode, s: " ", d:DND_MODE_HARDCORE, s: " ", d:DND_MODE_SOFTCORE);
		
	if(!isSoftorHardcore() || (game_mode != DND_MODE_HARDCORE && game_mode != DND_MODE_SOFTCORE))
		return;
	
	//Log(s:"spec? ", d:isSpectate, s: " ", d:PlayerCount());
	bool isEOL = true;
	
	// if not spectate, check if everyones dead otherwise check if no players left
	if(!isSpectate) {
		for(int i = 0; i < MAXPLAYERS; ++i) {
			// alive or has more lives left
			if(PlayerInGame(i) && (IsActorAlive(i + P_TIDSTART) || CheckActorInventory(i + P_TIDSTART, "CurrentLives"))) {
				isEOL = false;
				break;
			}
		}
	}
	else
		isEOL = !GetActivePlayerCount();
	
	if(isEOL/* && TransactionMade*/)
	{
		// end of level detected, push database changes
		Log(s:"End of level reached.");
		//FinishDBTransaction();
		
		// level end reached, reset information
		ResetPlayerInformationLevel();
	}
}

void HandlePlayerDataSave(int pnum, bool isDisconnect = false, int game_mode = -1) {
	if(game_mode == -1)
		game_mode = GetCVar("dnd_mode");

	// the last check needs it to see if players are left because if the last player quits then it's not in progress anymore anyway...
	int gstate = GetGameModeState();
	if(!isSoftorHardcore() || (game_mode != DND_MODE_HARDCORE && game_mode != DND_MODE_SOFTCORE) || gstate == GAMESTATE_COUNTDOWN || !(PlayerGameState[pnum] & DND_PSTATE_LOADED))
		return;

	PlayerGameState[pnum] &= ~(DND_PSTATE_LOADED);

	if(!PlayerIsLoggedIn(pnum) || !PlayerDatabaseState[pnum][PLAYER_SAVESTATE])
		return;
		
	if(!isDisconnect) {
		Log(s:"Save player ", d:pnum, s: " activites on death for char id ", d:PlayerActivities[pnum].char_id);

		BeginDBTransaction();

		if(PlayerDatabaseState[pnum][PLAYER_TRANSFERSTATE]) {
			Log(s:"Transferring player data.");
			WipeoutPlayerData(pnum, CheckActorInventory(pnum + P_TIDSTART, "DnD_CharacterID"));
			SetActorInventory(pnum + P_TIDSTART, "DnD_CharacterID", CheckActorInventory(pnum + P_TIDSTART, "DnD_TransfCharacterID"));
			PlayerActivities[pnum].char_id = CheckActorInventory(pnum + P_TIDSTART, "DnD_TransfCharacterID");
			PlayerDatabaseState[pnum][PLAYER_TRANSFERSTATE] = false;
		}
		
		SavePlayerActivities(pnum, PlayerActivities[pnum].char_id);
		EndDBTransaction();
		
		ResetPlayerActivities(pnum, false);
	}
	else {
		Log(s:"Save player ", d:pnum, s: " activites on disconnect for char id ", d:PlayerActivities[pnum].char_id);

		BeginDBTransaction();
		SavePlayerActivities(pnum, PlayerActivities[pnum].char_id);
		EndDBTransaction();

		// resets player activites already
		ResetPlayerInfo(pnum);
		ResetPlayerActivities(pnum, true);
		
		PlayerDatabaseState[pnum][PLAYER_SAVESTATE] = false;
	}
}

void SaveAllPlayerData() {
	Log(s:"Database save mode confirmed.");
	BeginDBTransaction();
	for(int i = 0; i < MAXPLAYERS; ++i) {
		// don't save peoples stuff while they are in load period
		//Log(d:i, s:" ", n:i + 1, s:": ",d:PlayerInGame(i), s:" " ,d:CheckActorInventory(i + P_TIDSTART, "CanLoad"), s:" ", d:PlayerDatabaseState[i][PLAYER_SAVESTATE]);
		int ptid = i + P_TIDSTART;
		if(PlayerInGame(i) && IsActorAlive(ptid) && !CheckActorInventory(ptid, "CanLoad") && PlayerDatabaseState[i][PLAYER_SAVESTATE]) {
			// DnD_CharacterActive is the only "this player actually loaded a character" marker that
			// dies with the connection, so it is what separates a real save from one driven by a
			// stale global. Without it char_id reads back as 0 off a fresh actor and the empty
			// character lands on slot 0.
			if (PlayerIsLoggedIn(i) && CheckActorInventory(ptid, "DnD_CharacterActive")) {
				// if transfer requested, wipeout old one and move to new one
				if(PlayerDatabaseState[i][PLAYER_TRANSFERSTATE]) {
					WipeoutPlayerData(i, CheckActorInventory(ptid, "DnD_CharacterID"));
					SetActorInventory(ptid, "DnD_CharacterID", CheckActorInventory(ptid, "DnD_TransfCharacterID"));
					PlayerDatabaseState[i][PLAYER_TRANSFERSTATE] = false;
				}
				//Log(s:"char id save on ", d:CheckActorInventory(i + P_TIDSTART, "DnD_CharacterID"));
				SavePlayerData(i, CheckActorInventory(ptid, "DnD_CharacterID"));
				ResetPlayerActivities(i, false); // reset this player's activities for the map, no need for them to be stored anymore
				PlayerGameState[i] |= DND_PSTATE_LOADED; //Also make sure the auto-save gets considered as loading a char - which will prevent unecessary loading periods.
				Log(s:"Saving player ", n:i + 1, s:"'s data on character id ", d:CheckActorInventory(ptid, "DnD_CharacterID"), s: ".");
			}
			PlayerDatabaseState[i][PLAYER_SAVESTATE] = false; //This will prevent players that joined and logged in in intermission get the auto-saved character erased.
		}
	}
	EndDBTransaction();
}

void HandleEndOfLevelRewards(int pnum) {
	// for next map things -- give players their rewards and stuff coming from a previous map
	//Log(s:"run open");
	int temp = 0;
	//Log(s:"map changed");

	// Now using PlayerWillBeSaved, because some servers might use multiple lives setting.
	// if hardcore modes are set, check this, otherwise simply give the player the things if they managed to survive the level regularly
	if((!isSetupComplete(SETUP_STATE1, SETUP_HARDCORE) || PlayerDatabaseState[pnum][PLAYER_SAVESTATE])) {
		GiveInventory("LevelToken", 1);
		TakeInventory("DnD_StatListOpen", 1);
		
		// check if the map had at least 1 monster in it... so people don't cheese stupid "skip maps"...
		if(GetLevelInfo(LEVELINFO_TOTAL_MONSTERS)) {
			temp = 2 * (1 + isSetupComplete(SETUP_STATE1, SETUP_HARDCORE)) * ((MapData[DND_MAPDATA_DIFFICULTY] + 1) + Clamp_Between(GetCVar("dnd_budget_reward"), 1, 1000));
			GiveInventory("Budget", temp);
			ACS_NamedExecuteWithResult("DnD Map Beaten Reward Text", temp);
		}
		
		GiveInventory("RoundsSurvived", 1);
	}
}

// registers a unique boss to clientside, can be used to reset it as well
Script "DnD Register Unique Boss" (int tid, int monid, int maxhp, int level) CLIENTSIDE {
	DungeonBossData[BOSSDATA_TID] = tid;
	
	int m_id = tid - DND_MONSTERTID_BEGIN;
	MonsterProperties[m_id].id = monid;
	MonsterProperties[m_id].maxhp = maxhp;
	MonsterProperties[m_id].level = level;
	
	SetResultValue(0);
}

void HandleAsmodeusAttack(int m_id, int isMelee) {
	int tid = m_id + DND_MONSTERTID_BEGIN;
	if(IsMonsterIdDemon(m_id)) {
		if(!isMelee) {
			if(!CheckInventory("AsmodeusFireLobCooldown")) {
				GiveInventory("AsmodeusFireCount", 1);
				if(CheckInventory("AsmodeusFireCount") == DND_ASMODEUS_MAXLOB) {
					SetInventory("AsmodeusFireCount", 0);
					GiveInventory("AsmodeusFireLobCooldown", 1);
				}

				ACS_NamedExecuteWithResult("DnD Asmodeus Mark Lob", 8 * random(-4, 4), random(24, 40));
			}
		}
		else if(!CheckInventory("AsmodeusFireBreathCooldown")) {
			GiveInventory("AsmodeusFireBreathCooldown", 1);
			GiveInventory("AsmodeusFireBreather", 1);
		}
	}
	else if(IsActorRobotic(tid)) {
		if(!isMelee) {
			GiveInventory("AsmodeusPlasmaShooter", 1);
		}
		else {
			GiveInventory("AsmodeusZapSpawner", 1);
		}
	}
	else if(IsActorMagic(tid)) {
		if(!isMelee) {
			GiveInventory("AsmodeusMagicShooter", 1);
		}
		else if(!CheckInventory("AsmodeusMagicCrackCooldown")) {
			GiveInventory("AsmodeusMagicCrackCooldown", 1);
			GiveInventory("AsmodeusMagicCrackShooter", 1);
		}
	}
	else if(!isMelee) { // only option left is undead...
		if(!CheckInventory("AsmodeusGhostCooldown")) {
			GiveInventory("AsmodeusGhostSpawner", 1);
			GiveInventory("AsmodeusGhostCooldown", 1);
		}
	}
	else if(!CheckInventory("AsmodeusMadePullerGhost") && !CheckInventory("AsmodeusPullerGhostCooldown")) {
		GiveInventory("AsmodeusPullerGhost_Spawner", 1);
	}
}

void HandleChargeAcquisitionOnKill(int this, int pnum) {
	int temp = PlayerModData[pnum].vals[PSTAT_IMP_ONKILL_FRENZY];
	if(temp && random(1, 100) <= temp) {
		HandlePlayerBuffAssignment(pnum, 0, BTI_FRENZYCHARGE);
	}

	temp = PlayerModData[pnum].vals[PSTAT_IMP_ONKILL_ENDURANCE];
	if(temp && random(1, 100) <= temp) {
		HandlePlayerBuffAssignment(pnum, 0, BTI_ENDURANCECHARGE);
	}

	temp = PlayerModData[pnum].vals[PSTAT_IMP_ONKILL_POWER];
	if(temp && random(1, 100) <= temp) {
		HandlePlayerBuffAssignment(pnum, 0, BTI_POWERCHARGE);
	}
}

#include "DnD_Damage.h"
#include "DnD_Weapons.h"
#include "DnD_WeaponWheel.h"