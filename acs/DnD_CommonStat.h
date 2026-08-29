#ifndef DND_COMMONSTAT_IN
#define DND_COMMONSTAT_IN

#include "DnD_Accessories.h"
#include "Inventory/DnD_InvAttribs.h"

// After DnD_InvAttribs.h on purpose -- the perk accessors read PlayerModData, and a variable cannot
// be forward referenced the way a function can.
#include "DnD_Perks.h"

#define DND_ACCURACY_CAP 100000

#define DND_SHIFTBITS_FOR_SLOTFROMFLAG 13 // 8192 must return 0 to us

#define DND_SYNTHMASK_EFFECT 4

#define DND_UNITY_DIVISOR 15

#define DND_SPREE_AMOUNT (4 * TICRATE) // 4 * 35
#define DND_SPREE_PER 10

#define DND_BASE_OVERLOADTICK 5
#define DND_BASE_OVERLOADTIME (175 / DND_BASE_OVERLOADTICK) // 5 seconds -- 175 / 5
#define DND_BASE_OVERLOADTIME_NOADJ 175
#define DND_BASE_PLAYEROVERLOADCHANCE 20 // 20%

#define DND_BASE_STAMINA 100
#define DND_BASE_STAMINA_RECOVERYRATE 17
#define DND_BASE_STAMINA_GAIN 5
#define DND_LOWSTAMINA_FACTOR 25
#define DND_DEPLETEDSTAMINA_FACTOR 50

#define DND_CRUSHINGBLOW_PCT 33 // 33% more melee damage taken
#define DND_OPENWOUNDS_BLEEDCHANCE 25 // 25% bonus bleed chance
#define DND_OPENWOUNDS_BLEEDMULTIBONUS 50 // 50% bleed dot multi on enemy
#define DND_DEADSTRIKE_BONUSPCT 100 // 100% dmg bonus

#define DND_BASE_PARRY_AMT 4
#define DND_PARRY_MASTER_BONUS 2
#define DND_PARRYMASTER_STAMINAREDUCE 5
#define DND_MIN_PARRYCOOLDOWN (DND_BASE_PARRY_AMT + DND_PARRY_MASTER_BONUS) // min cooldown should never be shorter than this
#define DND_PARRY_STAMINABLOCKTIME 105
#define DND_PARRY_STAMINA_COST 15
#define DND_PARRY_SPAWNDIST 64
#define DND_PARRY_DAMAGEREDUCTION 30 // 30% damage reduction
#define DND_PARRY_DAMAGEWEAKNESS 100 // double damage taken from melee
#define DND_PARRY_WEAKNESSTIME 105
#define DND_PARRY_WEAKNESS_TICS 7

#define ACCURACY_FACTOR 0.00001875

enum {
	DND_CHARGE_FRENZY,
	DND_CHARGE_ENDURANCE,
	DND_CHARGE_POWER
};
#define DND_BASE_PLAYERCHARGEMAX 3
#define DND_BASE_CHARGEDURATION 10 // 10 seconds

#define DND_ENDURANCECHARGE_BONUS 0.04
#define DND_FRENZYCHARGE_BONUS 0.05
#define DND_FRENZYCHARGE_SPEEDBONUS 3
#define DND_POWERCHARGE_BONUS 0.5

enum {
	DND_WDMG_USETARGET = 1,
	DND_WDMG_ISOCCULT = 2,
	DND_WDMG_ISPISTOL = 4,
	DND_WDMG_ISBOOMSTICK = 8,
	DND_WDMG_ISSUPER = 16,
	DND_WDMG_ISSPECIALAMMO = 32,
	DMG_WDMG_ISARTIFACT = 64,
	DND_WDMG_USEMASTER = 128,
	DND_WDMG_SETMASTER = 256,
	DND_WDMG_USETRACER = 512,
	DND_WDMG_ISRIPPER = 1024,
	DMG_WDMG_ESHIELDSCALE = 2048,
	DND_WDMG_ISMELEE = 4096,
	DND_WDMG_ISSLOT1 = 8192,
	DND_WDMG_ISSLOT2 = 16384,
	DND_WDMG_ISSLOT3 = 32768,
	DND_WDMG_ISSLOT4 = 65536,
	DND_WDMG_ISSLOT5 = 131072,
	DND_WDMG_ISSLOT6 = 262144,
	DND_WDMG_ISSLOT7 = 524288,
	DND_WDMG_ISSLOT8 = 1048576,
	DND_WDMG_ISSLOT9 = 2097152,
	DND_WDMG_ISSPELL = 4194304,
	DND_WDMG_USETARGETSMASTER = 8388608,
	DND_WDMG_ISRADIUSDMG = 16777216,
	/*DND_WDMG_ICEDAMAGE = 33554432,
	DND_WDMG_POISONDAMAGE = 67108864,
	DND_WDMG_LIGHTNINGDAMAGE = 134217728,*/
	DND_WDMG_ISDOT = 268435456
};

enum {
	DND_AILMENT_IGNITE = 1,
	DND_AILMENT_CHILL = 2,
	DND_AILMENT_FREEZE = 4,
	DND_AILMENT_POISON = 8,
	DND_AILMENT_OVERLOAD = 16,
	DND_AILMENT_BLEED = 32
};

enum {
	DND_DAMAGEFLAG_USEMASTER 			= 			0b1,
	DND_DAMAGEFLAG_ISRADIUSDMG 			= 			0b10,
	DND_DAMAGEFLAG_CULL 				= 			0b100,
	DND_DAMAGEFLAG_ISDAMAGEOVERTIME		=			0b1000,
	DND_DAMAGEFLAG_DISTANCEGIVESDAMAGE	=			0b10000,
	DND_DAMAGEFLAG_NOPOISONSTACK		=			0b100000,
	DND_DAMAGEFLAG_HALFDMGSELF			=			0b1000000,
	DND_DAMAGEFLAG_INFLICTPOISON		=			0b10000000,
	DND_DAMAGEFLAG_BLASTSELF			=			0b100000000,
	DND_DAMAGEFLAG_SELFORIGIN			=			0b1000000000,
	DND_DAMAGEFLAG_DOFULLDAMAGE			=			0b10000000000,
	DND_DAMAGEFLAG_EXTRATOUNDEAD		=			0b100000000000,
	DND_DAMAGEFLAG_ISHITSCAN			=			0b1000000000000,
	DND_DAMAGEFLAG_NOIGNITESTACK		=			0b10000000000000,
	DND_DAMAGEFLAG_PERCENTHEALTH		=			0b100000000000000,
	DND_DAMAGEFLAG_RIPSONCE				=			0b1000000000000000,
	DND_DAMAGEFLAG_ISSPELL				=			0b10000000000000000,
	DND_DAMAGEFLAG_ISSPECIALAMMO		=			0b100000000000000000,
	DND_DAMAGEFLAG_ADDEDIGNITE			=			0b1000000000000000000,
	DND_DAMAGEFLAG_COUNTSASMAGIC		=			0b10000000000000000000,
	DND_DAMAGEFLAG_NOPUSH				=			0b100000000000000000000,
	DND_DAMAGEFLAG_NONWEAPON			=			0b1000000000000000000000,
	DND_DAMAGEFLAG_ISEXPLOSIVE			=			0b10000000000000000000000,
	DND_DAMAGEFLAG_ISMELEE				=			0b100000000000000000000000,
	// ignite is sized off the fire in the hit but still has to ROLL the chance -- ADDEDIGNITE above
	// does that AND makes the ignite guaranteed. Keep in step with DamageTypes.dec (16777216).
	DND_DAMAGEFLAG_SCALEIGNITE			=			0b1000000000000000000000000,

	// below are special things that are cleared after a certain point in HandleImpactDamage function
	DND_DAMAGEFLAG_ISREFLECTED			=			67108864,
	DND_DAMAGEFLAG_COUNTSASMELEE		=			134217728,
	DND_DAMAGEFLAG_SOULATTACK			=			268435456,
	DND_DAMAGEFLAG_LOSEDAMAGEPERHIT		=			536870912,
	DND_DAMAGEFLAG_FOILINVUL			=			1073741824,
};

// max 18 of these, the data is shifted by 14 bits from elsewhere
enum {
	DND_DAMAGETICFLAG_PUSH				=			0b1,
	DND_DAMAGETICFLAG_CRIT				=			0b10,
	DND_DAMAGETICFLAG_ICE				=			0b100,
	DND_DAMAGETICFLAG_FIRE				=			0b1000,
	DND_DAMAGETICFLAG_LIGHTNING			=			0b10000,
	DND_DAMAGETICFLAG_CONSIDERMELEE		=			0b100000,
	DND_DAMAGETICFLAG_DOT				=			0b1000000,
	DND_DAMAGETICFLAG_ADDEDIGNITE		=			0b10000000,
	DND_DAMAGETICFLAG_EXTRATOUNDEAD		=			0b100000000,
	DND_DAMAGETICFLAG_NOPOISONSTACK		=			0b1000000000,
	DND_DAMAGETICFLAG_NOIGNITESTACK		=			0b10000000000,
	DND_DAMAGETICFLAG_SOULATTACK		=			0b100000000000,
	DND_DAMAGETICFLAG_LESSENED			=			0b1000000000000, // tells damage numbers to paint it gray basically
	DND_DAMAGETICFLAG_PHYSICAL			=			0b10000000000000,
	DND_DAMAGETICFLAG_POISON			=			0b100000000000000,
	DND_DAMAGETICFLAG_INFLICTPOISON		=			0b1000000000000000,
	DND_DAMAGETICFLAG_SPELL				=			0b10000000000000000,
	// 18th and LAST slot -- the comment above is a hard cap, the value is shifted by 14 bits elsewhere.
	// Scales the ignite off the fire in the hit WITHOUT making it guaranteed, which is the half of
	// ADDEDIGNITE that a normal fire weapon wants. ADDEDIGNITE does both and is for weapons meant to
	// ignite every time.
	DND_DAMAGETICFLAG_SCALEIGNITE		=			0b100000000000000000,
};

#include "DnD_CommonResearch.h"

#define DND_ELITE_BASEDROP 0.002 // same as below

#define DND_ELITE_BASEDROP_ORB 0.025
#define DND_MONSTER_ORBSTACK_LEVELTHRESHOLD 10

#define DND_BASEARMOR_DROP 0.01375
#define DND_BASE_CHARMRATE 0.0225
#define DND_BASE_SPECIALTYRATE 0.00875

#define DND_BASE_PLAYERSPEED 0.9
#define DND_LOWEST_PLAYERSPEED 0.05
#define DND_HP_PER_STR 2
#define DND_HP_PER_LVL 5

#define BASE_PET_CAP 3

#define RISK_AVERSION_VALUE 5 // 5%
#define BASE_WISDOM_GAIN 10
#define BASE_GREED_GAIN 10
#define PERK_MEDICBONUS 10 // percent
#define PERK_MEDIC_ESBONUS 2 // percent
#define PERK_MEDICSTOREBONUS 15
#define DND_SAVAGERY_BONUS 20 // percent
#define DND_ACRIMONY_GAIN 8 // 8%
#define DND_ACRIMONY_RECOVERCHANCE 0.2 // 20%
#define DND_ACRIMONY_RECOVERPERCENT 2
#define PERK_DEADLINESS_BONUS 0.01 // 1%
#define DND_LUCK_GAIN 0.03 // 5% multiplicative luck
#define DND_LUCK_OUTCOME_GAIN 0.03

#define DND_STARTER_LUCK 0.75
#define DND_LUCK_DIMINISHPERLVL 0.025

#define DND_SHARPSHOOTER_MASTERY_BONUS 0.01

#define DND_STR_KNOCKBACK_GAIN 50
#define DND_BASE_PLAYER_MASS 250

#define DND_BASE_ARMOR_SHOW 100
#define DND_BASE_ARMOR 200
#define DND_BASE_ARMORCAP 300

#define STAT_BASE 0
#define DND_STAT_MAX 1024 // for menu only, in game stats can go up to 1024
#define DND_STAT_FULLMAX 1024
// Retired with the ten-perk system. DND_PERK_MAXPOINTS in DnD_Common.h is the tree's cap.

#define MAX_WEAPON_SLOTS 9

#define BIO_HP_ADD_1 5
#define BIO_HP_ADD_2 6
#define BIO_HP_ADD_3 9

#define IMP_RES_ADD_1 10
#define IMP_RES_ADD_2 15
#define IMP_RES_ADD_3 15

// The following are also in DnD_Inventory.h, but used here as well because compiling...
#define MAX_EXPRESIST_VAL 100
#define MAX_ELEMRESIST_VAL 100
#define MAX_PHYSRESIST_VAL 100
#define MAX_REFLRESIST_VAL 1000

#define DND_EXP_RES_ABILITY_BONUS 50

#include "DnD_ClassConstants.h"

#define MAX_MAP_DIFFICULTIES 5

enum {
	DND_MAPDATA_DIFFICULTY,
	DND_MAPDATA_INCURSIONSTATE,
	DND_MAPDATA_MAPEVENTS1,

	DND_MAPDATA_MONSTERTOTAL,
	DND_MAPDATA_ZOMBIEMANCOUNT,
	DND_MAPDATA_SHOTGUNGUYCOUNT,
	DND_MAPDATA_CHAINGUNGUYCOUNT,
	DND_MAPDATA_DEMONCOUNTCOUNT,
    DND_MAPDATA_SPECTRECOUNT,
    DND_MAPDATA_IMPCOUNT,
    DND_MAPDATA_CACODEMONCOUNT,
    DND_MAPDATA_PAINELEMENTALCOUNT,
    DND_MAPDATA_LOSTSOULCOUNT,
    DND_MAPDATA_REVENANTCOUNT,
    DND_MAPDATA_HELLKNIGHTCOUNT,
	DND_MAPDATA_BARONCOUNT,
	DND_MAPDATA_FATSOCOUNT,
	DND_MAPDATA_ARACHNOCOUNT,
	DND_MAPDATA_ARCHVILECOUNT,
	DND_MAPDATA_SPIDERMASTERMINDCOUNT,
	DND_MAPDATA_CYBERDEMONCOUNT,

	DND_MAPDATA_LOOTBONUS,

	DND_MAX_MAPDATA
};

global int 4: MapData[DND_MAX_MAPDATA];

#include "DnD_MapEvents.h"

str MapDifficultyLabel(int id) {
	str col = "\cd";
	switch(id) {
		case 3:
		case 4:
			col = "\c[Y5]";
		break;
		case 5:
		case 6:
			col = "\ck";
		break;
		case 7:
		case 8:
			col = "\ci";
		break;
		case 9:
		case 10:
			col = "\cg";
		break;
	}

	return StrParam(l:"DND_MAPDIFF", s:" - ", s:col, d:id + 1);
}

enum {
	DND_MHR_PERIODIC,
	DND_MHR_DURATION
};

enum {
	PUP_HOMINGNOREFLECT,
	PUP_IGNORERADIUSIMMUNITY,
	PUP_SLAINENEMIESRIP,
	PUP_FORBIDARMOR,
	PUP_LUCKYCRITS,
	PUP_PELLETSFIRECIRCLE,
	PUP_KNOCKBACKIMMUNE,
	PUP_CURSEIMMUNITY,
	PUP_PAINSHAREDWITHPETS,
	PUP_SOULWEPSDOFULL,
	PUP_SPELLSDOFULL,
	PUP_POISONTICSTWICE
};

// extra1 and extra2 hold additional information, for example for homing: SEEKINGMISSLE or SCREENSEEKER conditions
bool CheckUniquePropertyOnPlayer(int pnum, int prop, int extra1 = 0, int extra2 = 0) {
	switch(prop) {
		// homing not reflect can come from thorax or marine's perk50 (more generic, all proj cant be reflected)
		case PUP_HOMINGNOREFLECT:
		return (PlayerModData[pnum].vals[PSTAT_ESS_THORAX] && (extra1 | extra2));
		
		// +FORCERADIUSDMG can come from vaaj or marine
		case PUP_IGNORERADIUSIMMUNITY:
		return HasActorClassPerk_Fast(pnum + P_TIDSTART, DND_PLAYER_MARINE, 3);
		
		case PUP_SLAINENEMIESRIP:
		return PlayerModData[pnum].vals[PSTAT_EX_ABILITY_MONSTERSRIP] || pbuffs[pnum].buff_net_values[BUFF_SULPHUR].additive;
		
		case PUP_FORBIDARMOR:
		return PlayerModData[pnum].vals[PSTAT_EX_FORBID_ARMOR];
		
		case PUP_LUCKYCRITS:
		return HasPlayerFlag(pnum, PFLAG_LUCKYCRIT);
		
		case PUP_PELLETSFIRECIRCLE:
		return HasPlayerFlag(pnum, PFLAG_PELLETS_FIRE_CIRCLE);
		
		case PUP_KNOCKBACKIMMUNE:
		return HasPlayerFlag(pnum, PFLAG_KNOCKBACK_IMMUNITY) || CheckActorInventory(pnum + P_TIDSTART, "GryphonCheck");
		
		case PUP_CURSEIMMUNITY:
		return PlayerModData[pnum].vals[PSTAT_EX_CURSEIMMUNITY];
		
		case PUP_PAINSHAREDWITHPETS:
		return PlayerModData[pnum].vals[PSTAT_EX_DMGREDUCE_SHAREWITHPETS];
		
		case PUP_SOULWEPSDOFULL:
		return PlayerModData[pnum].vals[PSTAT_EX_SOULWEPS_FULLDAMAGE];
		
		case PUP_SPELLSDOFULL:
		return PlayerModData[pnum].vals[PSTAT_EX_BEHAVIOR_SPELLSFULLDAMAGE];
		
		case PUP_POISONTICSTWICE:
		return PlayerModData[pnum].vals[PSTAT_ESS_LESHRAC];
	}
	return false;
}

Script "DnD Check Unique Player Property" (int prop) {
	SetResultValue(CheckUniquePropertyOnPlayer(PlayerNumber(), prop));
}

#define MGNUMID 709
#define BONUSBONUSID 708
#define SECRETBONUSID 707
#define ITEMBONUSID 706
#define KILLBONUSID 705
#define PERKID 704
#define CASHSPLITID 703
#define MENUUSEID 702

#define MIN_CREDIT_GAIN 7
#define MIN_EXP_GAIN 10
#define ATTRIB_PER_LEVEL 2
#define NANOCAP 350

#define DND_MONSTERHP_PLAYERSCALE 25
#define DND_MAX_PLAYERHPSCALE 16

#define DND_HARDCORE_LOADTIME 5 // minimum time for hardcore load
#define DND_HARDCORE_SCREENID 6

str GetAttributeLabel(int id) {
	return StrParam(s:"DND_MENU_ATTR", d:id + 1);
}

str GetAttributeLabel_Short(int id) {
	return StrParam(s:"DND_MENU_ATTR_SHORT", d:id + 1);
}

#define DND_PERK_SHARPSHOOTER_INC 5 // 5%
#define DND_PERK_BRUTALITY_DAMAGEINC 5 // 5%
#define DND_PERK_BRUTALITY_RANGEINC 8 // 8%
#define DND_MUNITION_MASTERY_CHANCE 0.03 // 3%
#define DND_MASTERY_DEADLINESSCOUNTER 6 // 7th is the crit, we give crit token at 6th so the 7th will be one
#define DND_MASTERY_LUCKCHANCE 0.25 // 25%

enum {	
	RES_STAT_DEX = 1,
	RES_PERK_SHARP = 2,
	RES_PERK_RISKAVERSION = 4,
	RES_PERK_CHARISMA = 8,
	RES_PERK_MEDIC = 16,
	RES_PERK_MUNITIONIST = 32,
	RES_PERK_DEADLINESS = 64,
	RES_PERK_SAVAGERY = 128,
	RES_PERK_LUCK = 256,
	RES_PLAYERSPEED = 512,
	RES_ACCURACY = 1024,
	RES_EXPLOSIONRADIUS = 2048,
	RES_NOCLASSPERK = 4096
};

enum {
	SF_FREEZE = 1
};

// Rage. A stacking, decaying damage bonus described in the General Notes and stored on DnD_Rage so
// the status bar can read it. Gaining any refreshes the grace period, so a fight keeps it alive and
// a lull drains it -- "goes down to 0 in 3 seconds if no rage is gained".
void GiveRage(int pnum, int amt) {
	int ptid = pnum + P_TIDSTART;
	GiveActorInventory(ptid, "DnD_Rage", amt);
	SetActorInventory(ptid, "DnD_RageGrace", DND_RAGE_GRACE);
	ACS_NamedExecuteAlways("DnD Rage Decay", 0, ptid);
}

// Acrobacy / Tailwind. Grant and cooldown together, since the cooldown is the whole limiter on it.
void GiveTailwind(int pnum, int amt) {
	int ptid = pnum + P_TIDSTART;
	SetActorInventory(ptid, "DnD_TailwindSpeed", amt);
	SetActorInventory(ptid, "DnD_TailwindTimer", DND_TAILWIND_TICS);

	int cd = DND_TAILWIND_BASECOOLDOWN - PlayerModData[pnum].vals[PSTAT_TAILWIND_CDREDUCE] * TICRATE;
	SetActorInventory(ptid, "DnD_TailwindCooldown", Max(TICRATE, cd));

	ACS_NamedExecuteAlways("DnD Tailwind Timer", 0, ptid);
	ACS_NamedExecuteAlways("DnD Tailwind Cooldown", 0, ptid);
}

int GetRageDamageBonus(int pnum) {
	int res = CheckActorInventory(pnum + P_TIDSTART, "DnD_Rage") * DND_RAGE_DAMAGEPER;

	// Perception / Fresh Clip rides the same live term rather than the cached one, for the same
	// reason rage does: it is counted in SHOTS, so a cached factor would freeze it on the first.
	if(CheckActorInventory(pnum + P_TIDSTART, "DnD_FreshClipShots"))
		res += PlayerModData[pnum].vals[PSTAT_FRESHCLIP_DAMAGE];

	// Martialist / Unending Fury. Berserk otherwise only changes which hitscan the FIST uses, which
	// no other melee weapon can borrow -- so the perk is a damage percentage while berserk is up
	// rather than a share of the fist's variant, which is not a number that exists to take a share of.
	if(CheckActorInventory(pnum + P_TIDSTART, "PowerStrength"))
		res += PlayerModData[pnum].vals[PSTAT_BERSERK_ALLMELEE];

	// Perception / Earthshaker. Live for the same reason -- the ramp changes shot to shot.
	res += PlayerModData[pnum].vals[PSTAT_ARTILLERY_RAMP] *
			CheckActorInventory(pnum + P_TIDSTART, "DnD_ArtilleryRamp");

	return res;
}

// How much of Elusive is left, as a percent. Everything Elusive does is scaled by this rather than
// switched on and off, which is what "rapidly decaying from 100% effect to 0%" asks for.
int GetElusiveEffect(int pnum) {
	int left = CheckActorInventory(pnum + P_TIDSTART, "DnD_Elusive");
	return left <= 0 ? 0 : (left * 100) / DND_ELUSIVE_TICS;
}

int GetBonusPlayerSpeed(int pnum) {
	int ptid = pnum + P_TIDSTART;
	int res = PlayerModData[pnum].vals[PSTAT_SPEED_INCREASE] + GetPlayerFrenzyCharges(ptid, pnum) * DND_FRENZYCHARGE_SPEEDBONUS;

	// Acrobacy / Head Start. A token rather than a PSTAT because it is a temporary window, and a
	// PSTAT would have to be added and removed symmetrically -- a missed removal is permanent.
	res += CheckActorInventory(ptid, "DnD_HeadStartSpeed");

	// Martialist / Swift & Precise. Stacks times the per-hit percent, so the cap on the stack IS
	// the cap on the bonus -- there is no second clamp to keep in sync with it.
	res += CheckActorInventory(ptid, "DnD_SwiftPreciseStacks") * PlayerModData[pnum].vals[PSTAT_MELEEHIT_SPEED];
	res += CheckActorInventory(ptid, "DnD_CritSpeed");

	// Elusive, scaled by what is left of it.
	res += DND_ELUSIVE_SPEED * GetElusiveEffect(pnum) / 100;
	res += CheckActorInventory(ptid, "DnD_PlanBSpeed");
	res += CheckActorInventory(ptid, "DnD_TailwindSpeed");
	// add other stuff here
	res += CheckActorInventory(ptid, "GryphonCheck") * DND_GRYPHON_MSPEED + CheckActorInventory(ptid, "CelestialCheck") * DND_CELESTIAL_MSPEED;
	return res;
}

int GetPlayerSpeed(int pnum) {
	return DND_BASE_PLAYERSPEED * (100 + GetBonusPlayerSpeed(pnum)) / 100;
}

// returns true if there are things that'd nullify effect of dexterity
bool HasPlayerDexterityDisablers(int pnum) {
	return PlayerModData[pnum].vals[PSTAT_EX_UNITY];
}

// These getters must be used when doing calculations based on benefit of these stats
int GetDexterity(int pnum) {
	return (
		CheckActorInventory(pnum + P_TIDSTART, "PSTAT_Dexterity") + 
		PlayerModData[pnum].vals[PSTAT_STAT_DEXTERITY]
	) * (100 + PlayerModData[pnum].vals[PSTAT_IMP_PERCENTDEX] + PlayerModData[pnum].vals[PSTAT_CORR_PERCENTSTATS]) / 100;
}

int GetDexterityEffect(int pnum, int factor, int divisor = 1) {
	if(!HasPlayerDexterityDisablers(pnum))
		return GetDexterity(pnum) * factor / divisor;
	return 0;
}

// returns true if there are things that'd nullify effect of int
bool HasPlayerIntellectDisablers(int pnum) {
	return PlayerModData[pnum].vals[PSTAT_EX_UNITY];
}

int GetIntellect(int pnum) {
	return (
		CheckActorInventory(pnum + P_TIDSTART, "PSTAT_Intellect") + 
		PlayerModData[pnum].vals[PSTAT_STAT_INTELLECT]
	) * (100 + PlayerModData[pnum].vals[PSTAT_IMP_PERCENTINT] + PlayerModData[pnum].vals[PSTAT_CORR_PERCENTSTATS]) / 100;
}

int GetIntellectEffect(int pnum, int factor, int divisor = 1) {
	if(!HasPlayerIntellectDisablers(pnum))
		return GetIntellect(pnum) * factor / divisor;
	return 0;
}

// returns true if there are things that'd nullify effect of strength
bool HasPlayerStrengthDisablers(int pnum) {
	return PlayerModData[pnum].vals[PSTAT_EX_UNITY] || PlayerModData[pnum].vals[PSTAT_EX_INTBONUSTOMELEE];
}

int GetStrength(int pnum) {
	return (
		CheckActorInventory(pnum + P_TIDSTART, "PSTAT_Strength") + 
		PlayerModData[pnum].vals[PSTAT_STAT_STRENGTH]
	) * (100 + PlayerModData[pnum].vals[PSTAT_IMP_PERCENTSTR] + PlayerModData[pnum].vals[PSTAT_CORR_PERCENTSTATS]) / 100;
}

int GetStrengthEffect(int pnum, int factor, int divisor = 1) {
	if(!HasPlayerStrengthDisablers(pnum))
		return GetStrength(pnum) * factor / divisor;
	return 0;
}

// this sets player's unity item to cache it so we don't request it over and over in intense calculations
// dont use the "geteffect" functions here as unity is concerned only with raw value of the stats themselves
void CalculateUnity(int pnum) {
	int val = (
			GetStrength(pnum) +
			GetDexterity(pnum) +
			GetIntellect(pnum)
		) * (100 + PlayerModData[pnum].vals[PSTAT_CORR_PERCENTSTATS]) / 100;
	SetInventory("PSTAT_Unity", val);
}

void ConditionalCalculateUnity(int pnum) {
	if(PlayerModData[pnum].vals[PSTAT_EX_UNITY])
		CalculateUnity(pnum);
}

int GetUnity() {
	return CheckInventory("PSTAT_Unity");
}

int GetPetCap() {
	return BASE_PET_CAP + PlayerModData[PlayerNumber()].vals[PSTAT_IMP_BONUSPETCAP];
}

int GetActorPetCap(int tid) {
	return BASE_PET_CAP + PlayerModData[tid - P_TIDSTART].vals[PSTAT_IMP_BONUSPETCAP];
}

bool CanActorHaveMorePets(int tid) {
	return CheckActorInventory(tid, "PetCounter") < GetActorPetCap(tid);
}

int GetHealingBonuses(int pnum) {
	// Endurance / Medic feeds this. Integer percent -- HandleHealthPickup does amt * (100 + b) / 100.
	int bonus = PlayerModData[pnum].vals[PSTAT_HEALING_EFFECT];
	// doesn't make sense for it to go below 0
	int less_mod = Clamp_Between(100 - PlayerModData[pnum].vals[PSTAT_EX_LESSHEALING], 0, 100);
	bonus = bonus * less_mod / 100;
	return bonus;
}

int GetResearchHealthBonuses() {
	int res = BIO_HP_ADD_1 * (CheckResearchStatus(RES_BIO1) == RES_DONE);
	res += BIO_HP_ADD_2 * (CheckResearchStatus(RES_BIO2) == RES_DONE);
	res += BIO_HP_ADD_3 * (CheckResearchStatus(RES_BIO3) == RES_DONE);
	
	// cyborg's bonus
	if(HasClassPerk_Fast(DND_PLAYER_CYBORG, 1)) {
		res *= DND_CYBORG_CYBER_MULT;
		res /= DND_CYBORG_CYBER_DIV;
	}

	return res;
}

int GetActorResearchHealthBonuses(int tid) {
	int res = BIO_HP_ADD_1 * (CheckActorResearchStatus(tid, RES_BIO1) == RES_DONE);
	res += BIO_HP_ADD_2 * (CheckActorResearchStatus(tid, RES_BIO2) == RES_DONE);
	res += BIO_HP_ADD_3 * (CheckActorResearchStatus(tid, RES_BIO3) == RES_DONE);
	
	// cyborg's bonus
	if(HasClassPerk_Fast(DND_PLAYER_CYBORG, 1)) {
		res *= DND_CYBORG_CYBER_MULT;
		res /= DND_CYBORG_CYBER_DIV;
	}

	return res;
}

int GetMissingHealth() {
	return GetSpawnHealth() - GetActorProperty(0, APROP_HEALTH);
}

int CalculateHealthCapBonuses(int pnum) {
	int base = PlayerModData[pnum].vals[PSTAT_HP_FLAT];
	int temp = 0;
	if((temp = PlayerModData[pnum].vals[PSTAT_INC_MOREHPBONUS])) {
		// add negative component then double it -- this used to be the other way around but now it's more challenging to use
		base += ReadPlayerModExtra(pnum, INV_INC_MOREHPBONUS);
		base = base * (100 + temp) / 100;
	}

	return base;
}

// returns player max health
int GetSpawnHealth(bool bypassEShieldCheck = false, int pnum = -1) {
	if(pnum == -1)
		pnum = PlayerNumber();

	int tid = pnum + P_TIDSTART;

	if(!bypassEShieldCheck && PlayerModData[pnum].vals[PSTAT_EX_HEALTHATONE]) {
		SetActorInventory(tid, "PlayerHealthCap", 1);
		return 1;
	}

	int str_bonus = GetStrengthEffect(pnum, DND_HP_PER_STR);
	int res = CalculateHealthCapBonuses(pnum) + DND_BASE_HEALTH + DND_HP_PER_LVL * (CheckActorInventory(tid, "Level") - 1) + str_bonus;
	// consider percent bonuses from here on
	int percent  = PlayerModData[pnum].vals[PSTAT_HP_PCT];
	int temp = 0;
	if((temp = PlayerModData[pnum].vals[PSTAT_INC_MOREHPBONUS]))
		percent = percent * (100 + temp) / 100;
	
	percent += 	CheckActorInventory(tid, "CelestialCheck") * CELESTIAL_BOOST +
				GetActorResearchHealthBonuses(tid);
				   
	// player bonus + % research bonus
	res += (res * percent) / 100;
	if(IsAccessoryEquipped(tid, DND_ACCESSORY_ANGELICANKH))
		res >>= 1;

	if(PlayerModData[pnum].vals[PSTAT_EX_DOUBLE_HEALTHCAP])
		res <<= 1;

	if(res < DND_BASE_HEALTH)
		res = DND_BASE_HEALTH;

	if(HasActorClassPerk_Fast(tid, DND_PLAYER_CYBORG, 5))
		res /= 2;

	// last bit here is necessary to fix a mugshot related bug that may still call this function properly and end up seeing our health is 1
	SetActorInventory(tid, "PlayerHealthCap", !PlayerModData[pnum].vals[PSTAT_EX_HEALTHATONE] ? res : 1);
	return res;
}

int GetHealthPercent() {
	int hp_pct = CheckInventory("PlayerHealthCap");
	if(!hp_pct)
		hp_pct = 100;
	else {
		hp_pct = (GetActorProperty(0, APROP_HEALTH) * 100) / hp_pct;
		if(hp_pct > 100)
			hp_pct = 100;
	}
	
	return hp_pct;
}

// for players
int IsLowLife() {
	return GetHealthPercent() <= 50;
}

int GetActorSpawnHealth(int t) {
	int caller = ActivatorTID();
	SetActivator(t);
	int cap = GetSpawnHealth();
	SetActivator(caller);
    return cap;
}

void HandleCurseImmunityRemoval() {
	// we shouldnt immediately take it as there might be other ways the player has obtained curse immunity
	if(!PlayerModData[PlayerNumber()].vals[PSTAT_EX_CURSEIMMUNITY])
		TakeInventory("CurseImmunity", 1);
}

void UpdatePlayerKnockbackResist() {
	int pnum = PlayerNumber();
	if(CheckUniquePropertyOnPlayer(pnum, PUP_KNOCKBACKIMMUNE))
		SetActorProperty(0, APROP_MASS, bcs::INT_MAX);
	else {
		int val = GetStrengthEffect(pnum, DND_STR_KNOCKBACK_GAIN);
		val += DND_BASE_PLAYER_MASS + PlayerModData[pnum].vals[PSTAT_KNOCKBACK_RESIST];

		int pct = (100 + PlayerModData[pnum].vals[PSTAT_PERCENT_KNOCKBACKRESIST]);
		if(val < bcs::INT_MAX / pct)
			val = val * pct / 100;

		SetActorProperty(0, APROP_MASS, val);
	}
}

// Explosion radius that only artillery weapons get. Separate from GetPlayerAoEIncrease because that
// one is every explosion the player causes -- item mods, class perks, artifacts -- and folding an
// artillery-only bonus into it would have widened all of them.
//
// Quaker is the flat part. Earthshaker rides the SAME continuous-attack counter its damage half
// reads, so one stack is worth the same percentage to both, which is what its note describes.
//
// Judged on the weapon the owner is holding NOW, because the explosion knows its owner but not the
// weapon that fired it -- the radius script is called from around a hundred DECORATE states with no
// weapon argument. A projectile that lands after a swap is therefore judged by the new weapon. For
// Earthshaker that is self correcting, since the counter is zeroed the moment a weapon returns to
// Ready; for Quaker it is a real if small inaccuracy on slow projectiles.
int GetPlayerArtilleryAoEIncrease(int pnum, int tid) {
	if(!IsArtilleryWeapon(GetActorWeaponID(tid)))
		return 0;

	return PlayerModData[pnum].vals[PSTAT_ARTILLERY_RADIUS] +
		PlayerModData[pnum].vals[PSTAT_ARTILLERY_RAMP] * CheckActorInventory(tid, "DnD_ArtilleryRamp");
}

int GetPlayerAoEIncrease(int pnum) {
	int base = PlayerModData[pnum].vals[PSTAT_EXPLOSION_RADIUS];
	int temp = PlayerModData[pnum].vals[PSTAT_CORR_MOREAOE];
	if(temp)
		base = base * (100 + (((temp + 0.005) * 100) >> 16)) / 100;

	return base;
}

// Generic Player RPG Stat restore function
void RestoreRPGStat (int statflag) {
	int pnum = PlayerNumber();
	if(statflag & RES_ACCURACY)
		CalculatePlayerAccuracy(pnum);
	if((statflag & RES_EXPLOSIONRADIUS) && GetPlayerAoEIncrease(pnum))
		SetActorProperty(0, APROP_SCORE, GetPlayerAoEIncrease(pnum));
	if(statflag & RES_PLAYERSPEED)
		SetActorProperty(0, APROP_SPEED, GetPlayerSpeed(pnum));
		
	UpdatePlayerKnockbackResist();
	
	// accessories
	// can only intervene once per map
	if(IsAccessoryEquipped(ActivatorTID(), DND_ACCESSORY_ANGELICANKH) && !CheckInventory("Intervened")) {
		GiveInventory("CanIntervene", 1);
		SetPlayerProperty(0, 1, PROP_BUDDHA);
	}

	if(CheckInventory("ArtemisCheck"))
		GiveInventory("ArtemisPower", 1);
		
	if(CheckInventory("HateCheck")) {
		GiveInventory("PowerReflection", 1);
		//GiveInventory("HateReduction", 1);
	}

	if(CheckInventory("GryphonCheck"))
		GiveInventory("CurseImmunity", 1);
		
	if(CheckUniquePropertyOnPlayer(pnum, PUP_CURSEIMMUNITY))
		GiveInventory("CurseImmunity", 1);
	
	// So the player respawns with his actual new max hp
	SetActorProperty(0, APROP_SPAWNHEALTH, GetSpawnHealth());
	
	if(!(statflag & RES_NOCLASSPERK))
		HandleClassPerks(ActivatorTID());

	// A player who died mid blind leaves the loop in "DnD Monster Blind Cast" without reaching its
	// restore, so the freeze and the ownership state have to be dropped here too -- a petrify that
	// killed you would otherwise respawn you frozen.
	TakeInventory("IsBlinded", 1);
	SetInventory("BlindTimer", 0);
	SetInventory("BlindSeverity", 0);
	SetInventory("BuffTintTimer", 0);
	SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
}

// Renamed from HasPlayerPowerset deliberately. It used to take a PPOWER_* BITMASK and test it
// against a packed int; it now takes a PFLAG_* INDEX. The two are not interchangeable and a stale
// PPOWER_ argument would silently test the wrong bit, so the old name had to stop compiling.
bool HasPlayerFlag(int pnum, int flag) {
	return !!(PlayerModData[pnum].pflags[flag >> 5] & (1 << (flag & 31)));
}

// GetPlayerAttributeValue / GetPlayerAttributeExtra / GetActorAttributeValue used to live here. The
// first two became one-line forwards to ReadPlayerModValue / ReadPlayerModExtra once the id-keyed
// arrays went, so they were two names for one function and are gone; the third had no callers at all.
//
// Call ReadPlayerModValue / ReadPlayerModExtra directly. Both are id-keyed and pay a mapper switch,
// so they are for the paths that genuinely only have an attribute id in hand -- item application, the
// sync scripts, the stat pages. Gameplay code on a hot path reads its slot: PlayerModData[p].vals[PSTAT_X].

void CalculatePlayerAccuracy(int pnum, int wepid = -1) {
	int acc = PlayerModData[pnum].vals[PSTAT_ACCURACY_INCREASE] + PlayerModData[pnum].vals[PSTAT_ESS_OMNISIGHT];
	int acc_pct = PlayerModData[pnum].vals[PSTAT_ESS_OMNISIGHT2];

	if(wepid == -1)
		wepid = CheckInventory("DnD_WeaponID");

	if(CheckInventory("Hobo_ShotgunFrenzyTimer") && IsBoomstick(wepid)) {
		acc_pct += DND_HOBO_ACCURACYBONUS + HasClassPerk_Fast(DND_PLAYER_HOBO, 5) * DND_HOBO_ACCURACYBONUS;
	}

	// omnisight essence gives % increased accuracy
	acc += (acc * acc_pct) / 100;
	if(acc > DND_ACCURACY_CAP)
		acc = DND_ACCURACY_CAP;
	//printbold(s:"calc acc ", d:acc);
	SetActorProperty(0, APROP_ACCURACY, acc);
	
	// sync to clients
	ACS_NamedExecuteAlways("DnD Sync Actor Property", 0, ActivatorTID(), APROP_ACCURACY, acc);
}

void UpdatePlayerSpreeTimer(int pnum) {
	int base = DND_SPREE_AMOUNT * (100 + PlayerModData[pnum].vals[PSTAT_INCKILLINGSPREE]) / 100;
	base = base * (100 + DND_PUNISHER_SPREEBONUS * (HasClassPerk_Fast(DND_PLAYER_PUNISHER, 4))) / 100;
	SetAmmoCapacity("DnD_SpreeTimer", base);
}

// Give powerups and stuff of the classes if they satisfy their perk things
void HandleClassPerks(int tid) {
	int lvl = CheckActorInventory(tid, "Level");
	int class = CheckActorInventory(tid, "DnD_Character") - 1; // we use 0 based
	str perkToGive = "";
	if(lvl >= DND_PERK1_THRESHOLD) {
		perkToGive = GetClassPerk(class, DND_CLASSPERK_1);
		if(perkToGive != "")
			GiveActorInventory(tid, perkToGive, 1);
	}

	for(int i = DND_CLASSPERK_2; i <= DND_CLASSPERK_LAST; ++i) {
		if(lvl >= (i - 1) * DND_PERK_REGULARTHRESHOLD) {
			perkToGive = GetClassPerk(class, i);
			if(perkToGive != "")
				GiveActorInventory(tid, perkToGive, 1);
		}
	}

	// Perk items are granted ONLY above, so this is the one place the perk cache can go
	// stale -- rebuild it here, before anything below reads it back.
	CachePlayerPerkData(tid - P_TIDSTART);

	// he has a perk that updates spree timers so best to do it here
	if(isActorPlayerClass(tid, DND_PLAYER_PUNISHER))
		UpdatePlayerSpreeTimer(tid - P_TIDSTART);
	
	if(isActorPlayerClass(tid, DND_PLAYER_CYBORG)) {
		if(HasActorClassPerk_Fast(tid, DND_PLAYER_CYBORG, 4)) {
			lvl = ActivatorTID();

			// change activator and update ammo cap
			SetActivator(tid);
			SetAmmoCapacity("Cyborg_Instability_Timer", DND_CYBORG_INSTABILITY_TIMER + DND_CYBORG_INSTABILITY_BONUS);
			SetActivator(lvl);
		}

		if(HasActorClassPerk_Fast(tid, DND_PLAYER_CYBORG, 5)) {
			GetSpawnHealth(false, tid - P_TIDSTART);
			HandleEShieldChange(tid - P_TIDSTART, true);
		}
	}
}

void SetEnergyShield(int val) {
	SetInventory("EShieldAmount", val);
	SetInventory("EShieldAmountVisual", val);
}

void SetActorEnergyShield(int tid, int val) {
	SetActorInventory(tid, "EShieldAmount", val);
	SetActorInventory(tid, "EShieldAmountVisual", val);
}

void AddEnergyShield(int val) {
	GiveInventory("EShieldAmount", val);
	GiveInventory("EShieldAmountVisual", val);
}

void AddActorEnergyShield(int tid, int val) {
	GiveActorInventory(tid, "EShieldAmount", val);
	GiveActorInventory(tid, "EShieldAmountVisual", val);
}

void TakeEnergyShield(int val) {
	TakeInventory("EShieldAmount", val);
	TakeInventory("EShieldAmountVisual", val);
}

void TakeActorEnergyShield(int tid, int val) {
	TakeActorInventory(tid, "EShieldAmount", val);
	TakeActorInventory(tid, "EShieldAmountVisual", val);
}

void UpdateEnergyShieldVisual(int val) {
	SetAmmoCapacity("EShieldAmountVisual", val);
}

void HandleEShieldChange(int pnum, bool remove) {
	int i = GetPlayerEnergyShieldCap(pnum);
	UpdateEnergyShieldVisual(i);

	if(remove && CheckInventory("EShieldAmount") > i)
		SetEnergyShield(i);
}

// Absorb value for magic or poison attacks
int GetEShieldMagicAbsorbValue(int pnum) {
	if(PlayerModData[pnum].vals[PSTAT_EX_ESHIELDFULLABSORB] || HasPlayerFlag(pnum, PFLAG_ESHIELDBLOCKALL))
		return 100;
	return PlayerModData[pnum].vals[PSTAT_MAGIC_NEGATION];
}

int GetPlayerEnergyShieldCap(int pnum) {
	int base = PlayerModData[pnum].vals[PSTAT_SHIELD_FLAT];
	
	int int_bonus = GetIntellectEffect(pnum, 1, 2);
	int spawn_health = GetSpawnHealth(true, pnum);
	
	// cyborg eshield conversion from hp is half
	base += spawn_health * (PlayerModData[pnum].vals[PSTAT_EX_HPTOESHIELD] + HasActorClassPerk_Fast(pnum + P_TIDSTART, DND_PLAYER_CYBORG, 5) * 50) / 100;

	base = (base * (100 + PlayerModData[pnum].vals[PSTAT_SHIELD_PCT] + int_bonus)) / 100;
	return base;
}

#define DND_MIT_PER_DEX 0.2
#define DND_MIT_BASE 50.0 // 50%
#define DND_MIT_MAXEFFECT 90.0
#define DND_MIT_MAXCHANCE 100.0

int GetMitigationChance(int pnum, bool forcedReturn = false) {
	if(!forcedReturn && HasPlayerFlag(pnum, PFLAG_MITIGATION_TO_DODGE))
		return 0;

	int base = GetDexterityEffect(pnum, DND_MIT_PER_DEX) + PlayerModData[pnum].vals[PSTAT_MIT_CHANCE];

	// Acrobacy / Nimbleness. Its OWN cap, not the shared one: the notes cap this perk at 25%,
	// which says nothing about how much mitigation the player may have from everything else.
	int nimble = PlayerModData[pnum].vals[PSTAT_MIT_PERRUNSEC];
	if(nimble) {
		nimble *= CheckActorInventory(pnum + P_TIDSTART, "DnD_RunSeconds") / DND_NIMBLENESS_SECSPERSTACK;
		if(nimble > DND_NIMBLENESS_CAP)
			nimble = DND_NIMBLENESS_CAP;
		base += nimble;
	}
	base += CheckActorInventory(pnum + P_TIDSTART, "DnD_HasAmphetamine") * DND_AMPHETAMINE_MITIGATIONCHANCE;
	base += pbuffs[pnum].buff_net_values[BUFF_MITIGATION].additive;

	if(base >= DND_MIT_MAXCHANCE)
		base = DND_MIT_MAXCHANCE;

	return base;
}

bool CouldMitigateDamage(int pnum) {
	//Log(f:random(1.0, 100.0), s: " vs ", f:GetMitigationChance(pnum));
	return random(0.01, 100.0) <= GetMitigationChance(pnum);
}

int GetMitigationEffect(int pnum) {
	int mit_eff = PlayerModData[pnum].vals[PSTAT_MIT_EFFECT] + DND_MIT_BASE;
	mit_eff += CheckActorInventory(pnum + P_TIDSTART, "DnD_HasAmphetamine") * DND_AMPHETAMINE_MITIGATIONEFFECT;
	if(mit_eff > DND_MIT_MAXEFFECT)
		mit_eff = DND_MIT_MAXEFFECT;
	return mit_eff;
}

#define DND_DODGECHANCE_CAP 60.0

int GetDodgeChance(int pnum) {
	int base = 0;
	
	if(HasPlayerFlag(pnum, PFLAG_MITIGATION_TO_DODGE))
		base += GetMitigationChance(pnum, true) / 2;

	// Acrobacy / Evasive Maneuvers. "Avoid taking damage from direct hits" IS the dodge, so it joins
	// the existing roll rather than adding a second one -- two independent rolls would compound and
	// quietly beat DND_DODGECHANCE_CAP, which is enforced below on this sum alone.
	if(CheckActorInventory(pnum + P_TIDSTART, "DnD_DashEvadeWindow"))
		base += PlayerModData[pnum].vals[PSTAT_DASH_AVOIDCHANCE] << 16;

	// Elusive. "Avoid damage from hits" is the DODGE, not mitigation -- mitigation reduces a hit and
	// dodge skips it. Scaled by what is left of the effect, which is the whole character of Elusive.
	base += (DND_ELUSIVE_AVOID * GetElusiveEffect(pnum) / 100) << 16;

	if(base > DND_DODGECHANCE_CAP)
		base = DND_DODGECHANCE_CAP;
	return base;
}

int GetResistPenetration(int pnum, int category) {
	int val = 0;
	switch(category) {
		case DND_DAMAGECATEGORY_BULLET:
		case DND_DAMAGECATEGORY_MELEE:
			val = PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_PHYS];
		break;
		case DND_DAMAGECATEGORY_ENERGY:
			val = PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_ENERGY];
		break;
		case DND_DAMAGECATEGORY_OCCULT:
			val = PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_OCCULT];
		break;

		case DND_DAMAGECATEGORY_FIRE:
			val = PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_FIRE];

			if(HasPlayerFlag(pnum, PFLAG_ELEPENHARMONY))
				val = max(val, 
						max(
							PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_ICE],
							max(
								PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_LIGHTNING],
								PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_POISON]
							)
						)
				);
			val += PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_ELEMENTAL];
		break;
		case DND_DAMAGECATEGORY_ICE:
			val = PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_ICE];

			if(HasPlayerFlag(pnum, PFLAG_ELEPENHARMONY))
				val = max(val, 
						max(
							PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_FIRE],
							max(
								PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_LIGHTNING],
								PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_POISON]
							)
						)
				);
			val += PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_ELEMENTAL];
		break;
		case DND_DAMAGECATEGORY_LIGHTNING:
			val = PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_LIGHTNING];

			if(HasPlayerFlag(pnum, PFLAG_ELEPENHARMONY))
				val = max(val, 
						max(
							PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_ICE],
							max(
								PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_FIRE],
								PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_POISON]
							)
						)
				);
			val += PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_ELEMENTAL];
		break;
		case DND_DAMAGECATEGORY_POISON:
			val = PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_POISON];

			if(HasPlayerFlag(pnum, PFLAG_ELEPENHARMONY))
				val = max(val, 
						max(
							PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_ICE],
							max(
								PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_LIGHTNING],
								PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_FIRE]
							)
						)
				);
			val += PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_ELEMENTAL];
		break;

		case DND_DAMAGECATEGORY_SOUL:
			val = PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_OCCULT] + PlayerModData[pnum].vals[PSTAT_EX_SOULWEPSPEN];
		break;
	}

	val += PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_ALL] + PlayerModData[pnum].vals[PSTAT_EX_UNITY_PEN_BONUS] * GetUnity() / DND_UNITY_DIVISOR;

	return val;
}

// purely used for cosmetic reasons
int GetHighestElePen(int pnum) {
	// this is very awful code but it doesn't matter -- we also dont care about unity or all ele pen... as they all would have it anyways
	int fpen = PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_FIRE];
	int lpen = PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_LIGHTNING];
	int ipen = PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_ICE];
	int ppen = PlayerModData[pnum].vals[PSTAT_PEN_BASE + DND_PPEN_POISON];

	if(fpen > lpen && fpen > ipen && fpen > ppen)
		return DND_DAMAGECATEGORY_FIRE;
	if(ipen > lpen && ipen > fpen && ipen > ppen)
		return DND_DAMAGECATEGORY_ICE;
	if(lpen > ipen && lpen > fpen && lpen > ppen)
		return DND_DAMAGECATEGORY_LIGHTNING;
	if(ppen > lpen && ppen > fpen && ppen > ipen)
		return DND_DAMAGECATEGORY_POISON;
	return 0;
}

void RemoveMonsterAilment(int tid, int ailment) {
	int prev = CheckActorInventory(tid, "DnD_AilmentToken");
	SetActorInventory(tid, "DnD_AilmentToken", prev & ~ailment);
}

void AddMonsterAilment(int source, int tid, int ailment) {
	int prev = CheckActorInventory(tid, "DnD_AilmentToken");
	SetActorInventory(tid, "DnD_AilmentToken", prev | ailment);

	if(HasActorClassPerk_Fast(source, DND_PLAYER_WANDERER, 3) && CheckActorInventory(source, "EShieldAmount") && random(1, 100) <= DND_WANDERER_ELEAVOIDCHANCE)
		GiveActorInventory(source, "RemoveAilments", 1);
}

// How many ailments are on this monster, which is a POPCOUNT of the token -- not its bit length.
// Shifting to zero and counting the steps returns the position of the highest set bit instead, so a
// monster carrying nothing but bleed (bit 5) counted as six ailments and a monster carrying ignite
// alone counted as one. Feeds the wanderer's flat resist reduction in FactorResists, so the error
// went straight into damage.
//
// Clears the lowest set bit each pass rather than walking every bit position: it costs one iteration
// per ailment actually present, and it cannot spin on a negative value the way a >>= 1 loop would.
int CountMonsterAilments(int tid) {
	int count = 0;
	int val = CheckActorInventory(tid, "DnD_AilmentToken");
	while(val) {
		val &= val - 1;
		++count;
	}
	return count;
}

// charge_type is DND_CHARGE_*, or -1 for a caller that genuinely means "any charge". The per type
// bonus is summed with the all-charges one rather than multiplied, so a player carrying both an item
// mod and the matching Cunning perk gets what both plainly say.
int GetPlayerChargeDuration(int pnum, int charge_type = -1) {
	int base = DND_BASE_CHARGEDURATION;
	if(PlayerModData[pnum].vals[PSTAT_EX_CHARGEDURATIONHALVED])
		base >>= 1;

	int inc = PlayerModData[pnum].vals[PSTAT_CHARGEDURATION];
	if(charge_type >= 0 && charge_type < DND_MAX_CHARGETYPES)
		inc += PlayerModData[pnum].vals[PSTAT_CHARGEDURATION_BASE + charge_type];

	return base * (100 + inc) / 100;
}

int GetPlayerMaxFrenzyCharges(int pnum) {
	return DND_BASE_PLAYERCHARGEMAX + PlayerModData[pnum].vals[PSTAT_CORR_MAXFRENZY];
}

int GetPlayerMaxEnduranceCharges(int pnum) {
	return DND_BASE_PLAYERCHARGEMAX + PlayerModData[pnum].vals[PSTAT_CORR_MAXENDURANCE];
}

int GetPlayerMaxPowerCharges(int pnum) {
	return DND_BASE_PLAYERCHARGEMAX + PlayerModData[pnum].vals[PSTAT_CORR_MAXPOWER];
}

int GetPlayerFrenzyCharges(int tid, int pnum) {
	if(!PlayerModData[pnum].vals[PSTAT_EX_COUNTASHAVINGMAXCHARGEOF] || ReadPlayerModExtra(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF) != DND_CHARGE_FRENZY)
		return CheckActorInventory(tid, "DnD_FrenzyChargeCount");
	return GetPlayerMaxFrenzyCharges(pnum);
}

int GetPlayerEnduranceCharges(int tid, int pnum) {
	if(!PlayerModData[pnum].vals[PSTAT_EX_COUNTASHAVINGMAXCHARGEOF] || ReadPlayerModExtra(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF) != DND_CHARGE_ENDURANCE)
		return CheckActorInventory(tid, "DnD_EnduranceChargeCount");
	return GetPlayerMaxEnduranceCharges(pnum);
}

int GetPlayerPowerCharges(int tid, int pnum) {
	if(!PlayerModData[pnum].vals[PSTAT_EX_COUNTASHAVINGMAXCHARGEOF] || ReadPlayerModExtra(pnum, INV_EX_COUNTASHAVINGMAXCHARGEOF) != DND_CHARGE_POWER)
		return CheckActorInventory(tid, "DnD_PowerChargeCount");
	return GetPlayerMaxPowerCharges(pnum);
}

int CanActorHaveFrenzyCharges(int tid, int pnum) {
	return CheckActorInventory(tid, "DnD_FrenzyChargeCount") < GetPlayerMaxFrenzyCharges(pnum);
}

int CanActorHaveEnduranceCharges(int tid, int pnum) {
	return CheckActorInventory(tid, "DnD_EnduranceChargeCount") < GetPlayerMaxEnduranceCharges(pnum);
}

int CanActorHavePowerCharges(int tid, int pnum) {
	return CheckActorInventory(tid, "DnD_PowerChargeCount") < GetPlayerMaxPowerCharges(pnum);
}

int GetChargeCount(int pnum) {
	int ptid = pnum + P_TIDSTART;
	return GetPlayerFrenzyCharges(ptid, pnum) + GetPlayerEnduranceCharges(ptid, pnum) + GetPlayerPowerCharges(ptid, pnum);
}

void GiveActorFrenzyCharge(int tid, int amt, bool noSpawn = false) {
	GiveActorInventory(tid, "DnD_FrenzyChargeCount", amt);

	SetActorProperty(tid, APROP_SPEED, GetPlayerSpeed(tid - P_TIDSTART));

	// spawn charge actors
	if(!noSpawn)
		ACS_NamedExecuteWithResult("DnD Spawn Charge", tid, DND_CHARGE_FRENZY, amt);
}

void GiveActorEnduranceCharge(int tid, int amt, bool noSpawn = false) {
	GiveActorInventory(tid, "DnD_EnduranceChargeCount", amt);

	if(!noSpawn)
		ACS_NamedExecuteWithResult("DnD Spawn Charge", tid, DND_CHARGE_ENDURANCE, amt);
}

void GiveActorPowerCharge(int tid, int amt, bool noSpawn = false) {
	GiveActorInventory(tid, "DnD_PowerChargeCount", amt);

	if(!noSpawn)
		ACS_NamedExecuteWithResult("DnD Spawn Charge", tid, DND_CHARGE_POWER, amt);
}

int GetPlayerParryWeakenTimer(int pnum, int mon_tid) {
	int base = DND_PARRY_WEAKNESSTIME / DND_PARRY_WEAKNESS_TICS;

	if(IsBossTID(mon_tid))
		base /= 3;

	return base;
}

int CheckPlayerCleave(int pnum) {
	int res = PlayerModData[pnum].vals[PSTAT_REAPINGCLEAVE];
	return res && random(1, 100) <= res;
}

#include "DnD_Buffs.h"
#include "DnD_BuffInterface.h"
#include "DnD_StatCache.h"

#endif
