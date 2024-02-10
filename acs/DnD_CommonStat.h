#ifndef DND_COMMONSTAT_IN
#define DND_COMMONSTAT_IN

#include "DnD_Accessories.h"
#include "DnD_QuestDefs.h"
#include "DnD_InvAttribs.h"

#define DND_ACCURACY_CAP 100000

#define DND_EXP_ADJUST_LEVEL 80
#define DND_EXP_ADJUST_LEVELFACTOR 0.9

enum {
	DND_DAMAGECATEGORY_MELEE,
	DND_DAMAGECATEGORY_BULLET,
	DND_DAMAGECATEGORY_ENERGY,
	DND_DAMAGECATEGORY_EXPLOSIVES,
	DND_DAMAGECATEGORY_OCCULT,
	DND_DAMAGECATEGORY_ELEMENTAL,
	DND_DAMAGECATEGORY_SOUL
};
#define MAX_DAMAGE_CATEGORIES (DND_DAMAGECATEGORY_SOUL + 1)

#define DND_DAMAGECATEGORY_BEGIN DND_DAMAGECATEGORY_MELEE
#define DND_DAMAGECATEGORY_END (DND_DAMAGECATEGORY_ELEMENTAL + 1)

/*enum {
	TALENT_BULLET = 0,
	TALENT_MELEE,
	TALENT_ENERGY,
	TALENT_EXPLOSIVE,
	TALENT_OCCULT,
    TALENT_ELEMENTAL
};
#define MAX_TALENTS TALENT_ELEMENTAL + 1*/

#define DND_SHIFTBITS_FOR_SLOTFROMFLAG 13 // 8192 must return 0 to us
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
	DMG_WDMG_NECROBULK = 2048,
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
	DND_WDMG_FIREDAMAGE = 16777216,
	DND_WDMG_ICEDAMAGE = 33554432,
	DND_WDMG_POISONDAMAGE = 67108864,
	DND_WDMG_LIGHTNINGDAMAGE = 134217728,
	DND_WDMG_ISDOT = 268435456
};

enum {
	DND_DAMAGEFLAG_USEMASTER 			= 			0b1,
	DND_DAMAGEFLAG_ISSHOTGUN 			= 			0b10,
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
	DND_DAMAGEFLAG_SIMULATERIPPER		=			0b1000000000000000,
	DND_DAMAGEFLAG_ISSPELL				=			0b10000000000000000,
	DND_DAMAGEFLAG_ISSPECIALAMMO		=			0b100000000000000000,
	
	// below are special things that are cleared after a certain point in HandleImpactDamage function
	DND_DAMAGEFLAG_COUNTSASMELEE		=			0b1000000000000000000,
	DND_DAMAGEFLAG_SOULATTACK			=			0b10000000000000000000,
	DND_DAMAGEFLAG_LOSEDAMAGEPERHIT		=			0b100000000000000000000,
	DND_DAMAGEFLAG_FOILINVUL			=			0b1000000000000000000000,
};

enum {
	DND_DAMAGETICFLAG_PUSH				=			0b1,
	DND_DAMAGETICFLAG_CRIT				=			0b10,
	DND_DAMAGETICFLAG_ICE				=			0b100,
	DND_DAMAGETICFLAG_FIRE				=			0b1000,
	DND_DAMAGETICFLAG_LIGHTNING			=			0b10000,
	DND_DAMAGETICFLAG_CONSIDERMELEE		=			0b100000,
	DND_DAMAGETICFLAG_DOT				=			0b1000000,
};

#include "DnD_CommonResearch.h"

#define DND_ELITE_BASEDROP 0.0025 // same as below
// this was 0.05 before, but now 0.005 because we have 10% chance for elite in the game as base, so it'll scale up for all monsters from 0.005 now
// trying 0.01, 1% chance
#define DND_ELITE_BASEDROP_ORB 0.015

#define DND_BASE_PLAYERSPEED 1.0
#define DND_BULKINESS_GAIN 0.006
#define DND_BULKINESS_GAIN_AFTER100 0.0025
#define DND_DEX_GAIN 3
#define DND_INT_GAIN 3
#define DND_VIT_INCREASE 4
#define DND_STR_GAIN 9
#define DND_STR_CAPINCREASE 1 // 1 out of 200 => 0.5 out of 100
#define DND_STR_CAPFACTOR 200
#define DND_ARMOR_PER_BUL 4
#define DND_CHR_GAIN 0.5
#define DND_PET_CHARISMA_FACTOR 20
#define DND_TALENTPOINT_MARK 4

#define BASE_PET_CAP 3

#define ENDURANCE_RES_INTEGER 35 // 35 will be divided by 1000 to get 3.5%
#define ENDURANCE_RES_INC 0.035 // 3.5%
#define BASE_WISDOM_GAIN 10
#define BASE_GREED_GAIN 10
#define PERK_MEDICBONUS 10 // percent
#define PERK_MEDICSTOREBONUS 15
#define DND_SAVAGERY_BONUS 20 // percent
#define PERK_DEADLINESS_BONUS 0.01 // 1%
#define DND_MUNITION_GAIN 10
#define DND_LUCK_GAIN 0.15 // 15% multiplicative lucke

#define DND_SHARPSHOOTER_MASTERY_BONUS 1

#define DND_BUL_KNOCKBACK_GAIN 25
#define DND_STR_KNOCKBACK_GAIN 50
#define DND_BASE_PLAYER_MASS 100

#define DND_BASE_ARMOR_SHOW 100
#define DND_BASE_ARMOR 200
#define DND_BASE_ARMORCAP 300

#define STAT_BASE 0
#define DND_STAT_MAX 100 // for menu only, in game stats can go up to 1024
#define DND_STAT_FULLMAX 200
#define DND_PERK_MAX 10

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

#define DND_CLASSPERK1_LEVEL 5
#define DND_CLASSPERK2_LEVEL 25
#define DND_CLASSPERK3_LEVEL 50

#define DND_DOOMGUY_DMGREDUCE_PERCENT 10
#define DND_DOOMGUY_DROPCHANCE 0.05
#define DND_DOOMGUY_HEALDIV 100
#define DND_DOOMGUY_MAXHEAL 10000
#define DND_DOOMGUY_DMGMULT 5
#define DND_DOOMGUY_DMGDIV 4

#define DND_WANDERER_PERK25_BUFF 25 // 25%

#define DND_BERSERKER_PERK50_MAXSTACKS 30
#define DND_BERSERKER_PERK50_CRITBONUS 50

#define DND_EXP_RES_ABILITY_BONUS 50

global int 1: StatListOpened[];

#define MAX_MAP_DIFFICULTIES 5
global int 4: MapDifficulty;

str GetMapDifficultyLabel(int id) {
	return StrParam(s:"DND_MAPDIFF", d:id + 1);
}

enum {
	DND_MHR_PERIODIC,
	DND_MHR_DURATION
};

int IsHardcore() {
	return HardcoreSet;
}

enum {
	PUP_HOMINGNOREFLECT,
	PUP_EXPLOSIVEIGNORERESIST,
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
		return (GetPlayerAttributeValue(pnum, INV_ESS_THORAX) && (extra1 | extra2)) || CheckActorInventory(pnum + P_TIDSTART, "Marine_Perk50");
		
		// +FORCERADIUSDMG can come from vaaj or marine
		case PUP_EXPLOSIVEIGNORERESIST:
		return GetPlayerAttributeValue(pnum, INV_ESS_VAAJ) || CheckActorInventory(pnum + P_TIDSTART, "Marine_Perk25");
		
		case PUP_SLAINENEMIESRIP:
		return GetPlayerAttributeValue(pnum, INV_EX_ABILITY_MONSTERSRIP);
		
		case PUP_FORBIDARMOR:
		return GetPlayerAttributeValue(pnum, INV_EX_FORBID_ARMOR);
		
		case PUP_LUCKYCRITS:
		return GetPlayerAttributeValue(pnum, INV_EX_ABILITY_LUCKYCRIT);
		
		case PUP_PELLETSFIRECIRCLE:
		return GetPlayerAttributeValue(pnum, INV_EX_BEHAVIOR_PELLETSFIRECIRCLE);
		
		case PUP_KNOCKBACKIMMUNE:
		return GetPlayerAttributeValue(pnum, INV_EX_KNOCKBACK_IMMUNITY) || CheckActorInventory(pnum + P_TIDSTART, "GryphonCheck");
		
		case PUP_CURSEIMMUNITY:
		return GetPlayerAttributeValue(pnum, INV_EX_CURSEIMMUNITY) || CheckActorInventory(pnum + P_TIDSTART, "Marine_Perk50");
		
		case PUP_PAINSHAREDWITHPETS:
		return GetPlayerAttributeValue(pnum, INV_EX_DMGREDUCE_SHAREWITHPETS);
		
		case PUP_SOULWEPSDOFULL:
		return GetPlayerAttributeValue(pnum, INV_EX_SOULWEPS_FULLDAMAGE);
		
		case PUP_SPELLSDOFULL:
		return GetPlayerAttributeValue(pnum, INV_EX_BEHAVIOR_SPELLSFULLDAMAGE);
		
		case PUP_POISONTICSTWICE:
		return GetPlayerAttributeValue(pnum, INV_ESS_LESHRAC);
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
#define ATTRIB_PER_LEVEL 3
#define NANOCAP 350

#define DND_MONSTERHP_PLAYERSCALE 25
#define DND_MAX_PLAYERHPSCALE 16

#define DND_HARDCORE_LOADTIME 5 // minimum time for hardcore load
#define DND_HARDCORE_SCREENID 6

enum {
	RESET_LEVEL = 1,
	RESET_CREDIT = 2,
	RESET_STATS = 4,
	RESET_PERKS = 8
};

enum {
	// dont change order of these, add below the last!
	STAT_STR,
	STAT_DEX,
	STAT_BUL,
	STAT_CHR,
	STAT_VIT,
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

#define DND_PERK_BEGIN STAT_SHRP
#define DND_PERK_END STAT_LUCK
#define DND_MAX_PERKS (DND_PERK_END - DND_PERK_BEGIN + 1)

#define DND_PERK_SHARPSHOOTER_INC 5 // 5%
#define DND_PERK_BRUTALITY_DAMAGEINC 5 // 5%
#define DND_PERK_BRUTALITY_RANGEINC 8 // 8%
#define DND_ENDURANCE_MASTERY_CHANCE 10 // 10%
#define DND_MUNITION_MASTERY_CHANCE 0.03 // 3%
#define DND_MASTERY_DEADLINESSCOUNTER 6 // 7th is the crit, we give crit token at 6th so the 7th will be one
#define DND_MASTERY_LUCKCHANCE 0.25 // 25%

enum {	
	RES_STAT_DEX = 1,
	RES_PERK_SHARP = 2,
	RES_PERK_ENDURANCE = 4,
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

int GetBonusPlayerSpeed(int pnum) {
	int res = GetPlayerAttributeValue(pnum, INV_SPEED_INCREASE);
	// add other stuff here
	res = res * (100 + CheckInventory("GryphonCheck") * DND_GRYPHON_MSPEED + CheckInventory("CelestialCheck") * DND_CELESTIAL_MSPEED) / 100;
	return res;
}

int GetPlayerSpeed(int pnum) {
	return DND_BASE_PLAYERSPEED + GetBonusPlayerSpeed(pnum);
}

void SlowPlayer(int amt, int mode, int pnum) {
	if(!pnum)
		pnum = PlayerNumber();
	// don't slow if player has gryphon boots
	if(CheckActorInventory(P_TIDSTART + pnum, "GryphonCheck") && !(mode & SF_FREEZE))
		return;
	if(mode & SF_FREEZE) {
		SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
		SetActorProperty(0, APROP_SPEED, 0.0);
		GiveInventory("P_Frozen", 1);
	}
	else
		SetActorProperty(P_TIDSTART + pnum, APROP_SPEED, GetPlayerSpeed(pnum) - amt);
}

// These getters must be used when doing calculations based on benefit of these stats
int GetDexterity() {
	int pnum = PlayerNumber();
	return (CheckInventory("PSTAT_Dexterity") + GetPlayerAttributeValue(pnum, INV_STAT_DEXTERITY)) * (100 + GetPlayerAttributeValue(pnum, INV_CORR_PERCENTSTAT)) / 100;
}

int GetIntellect() {
	int pnum = PlayerNumber();
	return (CheckInventory("PSTAT_Intellect") + GetPlayerAttributeValue(pnum, INV_STAT_INTELLECT)) * (100 + GetPlayerAttributeValue(pnum, INV_CORR_PERCENTSTAT)) / 100;
}

int GetActorIntellect(int tid) {
	int pnum = tid - P_TIDSTART;
	return (CheckActorInventory(tid, "PSTAT_Intellect") + GetPlayerAttributeValue(pnum, INV_STAT_INTELLECT)) * (100 + GetPlayerAttributeValue(pnum, INV_CORR_PERCENTSTAT)) / 100;
}

int GetStrength() {
	int pnum = PlayerNumber();
	return (CheckInventory("PSTAT_Strength") + GetPlayerAttributeValue(pnum, INV_STAT_STRENGTH)) * (100 + GetPlayerAttributeValue(pnum, INV_CORR_PERCENTSTAT)) / 100;
}

int GetBulkiness() {
	int pnum = PlayerNumber();
	return (CheckInventory("PSTAT_Bulkiness") + GetPlayerAttributeValue(pnum, INV_STAT_BULKINESS)) * (100 + GetPlayerAttributeValue(pnum, INV_CORR_PERCENTSTAT)) / 100;
}

int GetVitality() {
	int pnum = PlayerNumber();
	return (CheckInventory("PSTAT_Vitality") + GetPlayerAttributeValue(pnum, INV_STAT_VITALITY)) * (100 + GetPlayerAttributeValue(PlayerNumber(), INV_CORR_PERCENTSTAT)) / 100;
}

int GetCharisma() {
	int pnum = PlayerNumber();
	return (CheckInventory("PSTAT_Charisma") + GetPlayerAttributeValue(pnum, INV_STAT_CHARISMA)) * (100 + GetPlayerAttributeValue(PlayerNumber(), INV_CORR_PERCENTSTAT)) / 100;
}

int GetActorCharisma(int tid) {
	int pnum = PlayerNumber();
	return (CheckActorInventory(tid, "PSTAT_Charisma") + GetPlayerAttributeValue(pnum, INV_STAT_CHARISMA)) * (100 + GetPlayerAttributeValue(PlayerNumber(), INV_CORR_PERCENTSTAT)) / 100;
}

int GetPetCap(int tid) {
	return BASE_PET_CAP + GetActorCharisma(tid) / DND_PET_CHARISMA_FACTOR;
}

int GetHealingBonuses(int pnum) {
	int bonus = PERK_MEDICBONUS * CheckInventory("Perk_Medic");
	if(IsQuestComplete(0, QUEST_HEALFOR500))
		bonus += DND_QUEST_MASTERHEALER_INCREASE;
	if(IsQuestComplete(pnum + P_TIDSTART, QUEST_NOHEALINGPICKUP))
		bonus += DND_QUEST_SKINOTEETH_INCREASE;
	// doesn't make sense for it to go below 0
	int less_mod = Clamp_Between(100 - GetPlayerAttributeValue(pnum, INV_EX_LESSHEALING), 0, 100);
	bonus = bonus * less_mod / 100;
	return bonus;
}

int GetResearchHealthBonuses() {
	int res = BIO_HP_ADD_1 * (CheckResearchStatus(RES_BIO1) == RES_DONE);
	res += BIO_HP_ADD_2 * (CheckResearchStatus(RES_BIO2) == RES_DONE);
	res += BIO_HP_ADD_3 * (CheckResearchStatus(RES_BIO3) == RES_DONE);
	
	// cyborg's bonus
	if(CheckInventory("Cyborg_Perk50")) {
		res *= DND_CYBORG_CYBER_MULT;
		res /= DND_CYBORG_CYBER_DIV;
	}

	return res;
}

int GetMissingHealth() {
	return GetSpawnHealth() - GetActorProperty(0, APROP_HEALTH);
}

int CalculateHealthCapBonuses(int pnum) {
	// consider quest bonuses, charms, orb effects
	return IsQuestComplete(pnum + P_TIDSTART, QUEST_NODYING) * DND_QUEST_PRECIOUSLIFE_BONUS 		+
		   IsQuestComplete(pnum + P_TIDSTART, QUEST_NOHEALINGPICKUP) * DND_QUEST_SKINOTEETH_BONUS 	+
		   GetPlayerAttributeValue(pnum, INV_HP_INCREASE);
}

int GetSpawnHealth() {
	int pnum = PlayerNumber();
	int res = CalculateHealthCapBonuses(pnum) + DND_BASE_HEALTH + DND_VIT_INCREASE * GetVitality();
	// consider percent bonuses from here on
	int percent  = DND_TORRASQUE_BOOST * IsQuestComplete(0, QUEST_KILLTORRASQUE) 			+
				   // GetStrength() * DND_STR_CAPINCREASE 										+
				   CheckInventory("CelestialCheck") * CELESTIAL_BOOST 						+
				   GetResearchHealthBonuses() 												+
				   GetPlayerAttributeValue(pnum, INV_HPPERCENT_INCREASE);
	// player bonus + % research bonus
	res += (res * percent) / 100;
	if(IsAccessoryEquipped(ActivatorTID(), DND_ACCESSORY_ANGELICANKH))
		res >>= 1;
	if(res < DND_BASE_HEALTH)
		res = DND_BASE_HEALTH;
	SetInventory("PlayerHealthCap", res);
	return res;
}

int GetHealthPercent() {
	int hp_pct = CheckInventory("PlayerHealthCap");
	if(!hp_pct)
		hp_pct = 100;
	else {
		hp_pct = (GetActorProperty(0, APROP_HEALTH) * 100) / CheckInventory("PlayerHealthCap");
		if(hp_pct > 100)
			hp_pct = 100;
	}
	
	return hp_pct;
}

int GetActorSpawnHealth(int t) {
	int caller = ActivatorTID();
	SetActivator(t);
	int cap = GetSpawnHealth();
	SetActivator(caller);
    return cap;
}

void HandleCurseImmunityRemoval() {
	// if not marine and level less than 50, take it
	// we shouldnt immediately take it as there might be other ways the player has obtained curse immunity
	bool isMarine = isPlayerClass(DND_PLAYER_MARINE);
	if((!isMarine || CheckInventory("Level") < 50) && !GetPlayerAttributeValue(PlayerNumber(), INV_EX_CURSEIMMUNITY))
		TakeInventory("CurseImmunity", 1);
}

void UpdatePlayerKnockbackResist() {
	if(CheckUniquePropertyOnPlayer(PlayerNumber(), PUP_KNOCKBACKIMMUNE))
		SetActorProperty(0, APROP_MASS, INT_MAX);
	else {
		int bul = GetBulkiness();
		int strgth = GetStrength();
		SetActorProperty(0, APROP_MASS, DND_BASE_PLAYER_MASS + bul * DND_BUL_KNOCKBACK_GAIN + strgth * DND_STR_KNOCKBACK_GAIN + GetPlayerAttributeValue(PlayerNumber(), INV_KNOCKBACK_RESIST));
	}
}

// Generic Player RPG Stat restore function
void RestoreRPGStat (int statflag) {
	int pnum = PlayerNumber();
	if(statflag & RES_ACCURACY)
		CalculatePlayerAccuracy(pnum);
	if((statflag & RES_EXPLOSIONRADIUS) && GetPlayerAttributeValue(pnum, INV_EXPLOSION_RADIUS))
		SetActorProperty(0, APROP_SCORE, GetPlayerAttributeValue(pnum, INV_EXPLOSION_RADIUS));
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
		GiveInventory("HateReduction", 1);
	}

	if(CheckInventory("GryphonCheck"))
		GiveInventory("CurseImmunity", 1);
	
	if(CheckResearchStatus(RES_SYNTHMASK) == RES_DONE)
		GiveInventory("SynthMaskToken", 1);
		
	if(CheckUniquePropertyOnPlayer(pnum, PUP_CURSEIMMUNITY))
		GiveInventory("CurseImmunity", 1);
	
	// So the player respawns with his actual new max hp
	SetActorProperty(0, APROP_SPAWNHEALTH, GetSpawnHealth());
	HandleResearchBonuses();
	
	if(!(statflag & RES_NOCLASSPERK))
		HandleClassPerks();
}

int GetPlayerAttributeValue(int pnum, int attrib) {
	return PlayerModValues[pnum][attrib];
}

int GetPlayerAttributeExtra(int pnum, int attrib) {
	return PlayerModExtras[pnum][attrib];
}

int GetActorAttributeValue(int tid, int attrib) {
	return PlayerModValues[tid - P_TIDSTART][attrib];
}

void CalculatePlayerAccuracy(int pnum) {
	int acc = GetPlayerAttributeValue(pnum, INV_ACCURACY_INCREASE) + GetPlayerAttributeValue(pnum, INV_ESS_OMNISIGHT);
	// omnisight essence gives % increased accuracy
	acc += (acc * GetPlayerAttributeValue(pnum, INV_ESS_OMNISIGHT2)) / 100;
	if(acc > DND_ACCURACY_CAP)
		acc = DND_ACCURACY_CAP;
	//printbold(s:"calc acc ", d:acc);
	SetActorProperty(0, APROP_ACCURACY, acc);
	
	// sync to clients
	ACS_NamedExecuteAlways("DnD Sync Actor Property", 0, ActivatorTID(), APROP_ACCURACY, acc);
}

void HandleResearchBonuses() {
	if(CheckResearchStatus(RES_IMP3) == RES_DONE) {
		if(CheckInventory("Cyborg_Perk25")) {
			GiveInventory("ImpactProtection_3_Cyborg", 1);
			TakeInventory("ImpactProtection_2_Cyborg", 1);
			TakeInventory("ImpactProtection_1_Cyborg", 1);
		}
		else {
			GiveInventory("ImpactProtection_3", 1);
			TakeInventory("ImpactProtection_2", 1);
			TakeInventory("ImpactProtection_1", 1);
		}
	}
	else if(CheckResearchStatus(RES_IMP2) == RES_DONE) {
		if(CheckInventory("Cyborg_Perk25")) {
			GiveInventory("ImpactProtection_2_Cyborg", 1);
			TakeInventory("ImpactProtection_2_Cyborg", 1);
		}
		else {
			GiveInventory("ImpactProtection_2", 1);
			TakeInventory("ImpactProtection_1", 1);
		}
	}
	else if(CheckResearchStatus(RES_IMP1) == RES_DONE) {
		if(CheckInventory("Cyborg_Perk25"))
			GiveInventory("ImpactProtection_1_Cyborg", 1);
		else
			GiveInventory("ImpactProtection_1", 1);
	}
}

// Give powerups and stuff of the classes if they satisfy their perk things
void HandleClassPerks() {
	int lvl = CheckInventory("Level");
	int class = CheckInventory("DnD_Character") - 1; // we use 0 based
	if(lvl >= 5) {
		// 1st perk
		switch(class) {
			case DND_PLAYER_DOOMGUY:
				GiveInventory("Doomguy_Perk5", 1);
				TakeInventory("Marine_Perk5", 1);
				TakeInventory("Hobo_Perk5", 1);
				TakeInventory("Punisher_Perk5", 1);
				TakeInventory("Wanderer_Perk5", 1);
				TakeInventory("Cyborg_Perk5", 1);
				TakeInventory("Berserker_Perk5", 1);
			break;
			case DND_PLAYER_MARINE:
				GiveInventory("Marine_Perk5", 1);
				TakeInventory("Doomguy_Perk5", 1);
				TakeInventory("Hobo_Perk5", 1);
				TakeInventory("Punisher_Perk5", 1);
				TakeInventory("Wanderer_Perk5", 1);
				TakeInventory("Cyborg_Perk5", 1);
				TakeInventory("Berserker_Perk5", 1);
			break;			
			case DND_PLAYER_HOBO:
				GiveInventory("Hobo_Perk5", 1);
				TakeInventory("Doomguy_Perk5", 1);
				TakeInventory("Marine_Perk5", 1);
				TakeInventory("Punisher_Perk5", 1);
				TakeInventory("Wanderer_Perk5", 1);
				TakeInventory("Cyborg_Perk5", 1);
				TakeInventory("Berserker_Perk5", 1);
			break;
			case DND_PLAYER_PUNISHER:
				GiveInventory("Punisher_Perk5", 1);
				TakeInventory("Doomguy_Perk5", 1);
				TakeInventory("Marine_Perk5", 1);
				TakeInventory("Hobo_Perk5", 1);
				TakeInventory("Wanderer_Perk5", 1);
				TakeInventory("Cyborg_Perk5", 1);
				TakeInventory("Berserker_Perk5", 1);
			break;
			case DND_PLAYER_WANDERER:
				GiveInventory("Wanderer_Perk5", 1);
				TakeInventory("Doomguy_Perk5", 1);
				TakeInventory("Marine_Perk5", 1);
				TakeInventory("Hobo_Perk5", 1);
				TakeInventory("Punisher_Perk5", 1);
				TakeInventory("Cyborg_Perk5", 1);
				TakeInventory("Berserker_Perk5", 1);
			break;
			case DND_PLAYER_CYBORG:
				GiveInventory("Cyborg_Perk5", 1);
				TakeInventory("Doomguy_Perk5", 1);
				TakeInventory("Marine_Perk5", 1);
				TakeInventory("Hobo_Perk5", 1);
				TakeInventory("Punisher_Perk5", 1);
				TakeInventory("Wanderer_Perk5", 1);
				TakeInventory("Berserker_Perk5", 1);
			break;
			case DND_PLAYER_BERSERKER:
				GiveInventory("Berserker_Perk5", 1);
				TakeInventory("Doomguy_Perk5", 1);
				TakeInventory("Marine_Perk5", 1);
				TakeInventory("Hobo_Perk5", 1);
				TakeInventory("Punisher_Perk5", 1);
				TakeInventory("Wanderer_Perk5", 1);
				TakeInventory("Cyborg_Perk5", 1);
			break;
		}
	}
	
	if(lvl >= 25) {
		// 2nd perk
		switch(class) {
			case DND_PLAYER_DOOMGUY:
				GiveInventory("Doomguy_Perk25", 1);
				TakeInventory("Marine_Perk25", 1);
				TakeInventory("Hobo_Perk25", 1);
				TakeInventory("Punisher_Perk25", 1);
				TakeInventory("Wanderer_Perk25", 1);
				TakeInventory("Cyborg_Perk25", 1);
				TakeInventory("Berserker_Perk25", 1);
			break;
			case DND_PLAYER_MARINE:
				GiveInventory("Marine_Perk25", 1);
				TakeInventory("Doomguy_Perk25", 1);
				TakeInventory("Hobo_Perk25", 1);
				TakeInventory("Punisher_Perk25", 1);
				TakeInventory("Wanderer_Perk25", 1);
				TakeInventory("Cyborg_Perk25", 1);
				TakeInventory("Berserker_Perk25", 1);
			break;			
			case DND_PLAYER_HOBO:
				GiveInventory("Hobo_Perk25", 1);
				TakeInventory("Doomguy_Perk25", 1);
				TakeInventory("Marine_Perk25", 1);
				TakeInventory("Punisher_Perk25", 1);
				TakeInventory("Wanderer_Perk25", 1);
				TakeInventory("Cyborg_Perk25", 1);
				TakeInventory("Berserker_Perk25", 1);
			break;
			case DND_PLAYER_PUNISHER:
				GiveInventory("Punisher_Perk25", 1);
				TakeInventory("Doomguy_Perk25", 1);
				TakeInventory("Marine_Perk25", 1);
				TakeInventory("Hobo_Perk25", 1);
				TakeInventory("Wanderer_Perk25", 1);
				TakeInventory("Cyborg_Perk25", 1);
				TakeInventory("Berserker_Perk25", 1);
			break;
			case DND_PLAYER_WANDERER:
				GiveInventory("Wanderer_Perk25", 1);
				TakeInventory("Doomguy_Perk25", 1);
				TakeInventory("Marine_Perk25", 1);
				TakeInventory("Hobo_Perk25", 1);
				TakeInventory("Punisher_Perk25", 1);
				TakeInventory("Cyborg_Perk25", 1);
				TakeInventory("Berserker_Perk25", 1);
			break;
			case DND_PLAYER_CYBORG:
				GiveInventory("Cyborg_Perk25", 1);
				TakeInventory("Doomguy_Perk25", 1);
				TakeInventory("Marine_Perk25", 1);
				TakeInventory("Hobo_Perk25", 1);
				TakeInventory("Punisher_Perk25", 1);
				TakeInventory("Wanderer_Perk25", 1);
				TakeInventory("Berserker_Perk25", 1);
			break;
			case DND_PLAYER_BERSERKER:
				GiveInventory("Berserker_Perk25", 1);
				TakeInventory("Doomguy_Perk25", 1);
				TakeInventory("Marine_Perk25", 1);
				TakeInventory("Hobo_Perk25", 1);
				TakeInventory("Punisher_Perk25", 1);
				TakeInventory("Wanderer_Perk25", 1);
				TakeInventory("Cyborg_Perk25", 1);
			break;
		}
	}
	
	if(lvl >= 50) {
		// 3rd perk
		switch(class) {
			case DND_PLAYER_DOOMGUY:
				GiveInventory("Doomguy_Perk50", 1);
				TakeInventory("Marine_Perk50", 1);
				TakeInventory("Hobo_Perk50", 1);
				TakeInventory("Punisher_Perk50", 1);
				TakeInventory("Wanderer_Perk50", 1);
				TakeInventory("Cyborg_Perk50", 1);
				TakeInventory("Berserker_Perk50", 1);
			break;
			case DND_PLAYER_MARINE:
				GiveInventory("Marine_Perk50", 1);
				TakeInventory("Doomguy_Perk50", 1);
				TakeInventory("Hobo_Perk50", 1);
				TakeInventory("Punisher_Perk50", 1);
				TakeInventory("Wanderer_Perk50", 1);
				TakeInventory("Cyborg_Perk50", 1);
				TakeInventory("Berserker_Perk50", 1);
			break;			
			case DND_PLAYER_HOBO:
				GiveInventory("Hobo_Perk50", 1);
				TakeInventory("Doomguy_Perk50", 1);
				TakeInventory("Marine_Perk50", 1);
				TakeInventory("Punisher_Perk50", 1);
				TakeInventory("Wanderer_Perk50", 1);
				TakeInventory("Cyborg_Perk50", 1);
				TakeInventory("Berserker_Perk50", 1);
			break;
			case DND_PLAYER_PUNISHER:
				GiveInventory("Punisher_Perk50", 1);
				TakeInventory("Doomguy_Perk50", 1);
				TakeInventory("Marine_Perk50", 1);
				TakeInventory("Hobo_Perk50", 1);
				TakeInventory("Wanderer_Perk50", 1);
				TakeInventory("Cyborg_Perk50", 1);
				TakeInventory("Berserker_Perk50", 1);
			break;
			case DND_PLAYER_WANDERER:
				GiveInventory("Wanderer_Perk50", 1);
				TakeInventory("Doomguy_Perk50", 1);
				TakeInventory("Marine_Perk50", 1);
				TakeInventory("Hobo_Perk50", 1);
				TakeInventory("Punisher_Perk50", 1);
				TakeInventory("Cyborg_Perk50", 1);
				TakeInventory("Berserker_Perk50", 1);
			break;
			case DND_PLAYER_CYBORG:
				GiveInventory("Cyborg_Perk50", 1);
				TakeInventory("Doomguy_Perk50", 1);
				TakeInventory("Marine_Perk50", 1);
				TakeInventory("Hobo_Perk50", 1);
				TakeInventory("Punisher_Perk50", 1);
				TakeInventory("Wanderer_Perk50", 1);
				TakeInventory("Berserker_Perk50", 1);
			break;
			case DND_PLAYER_BERSERKER:
				GiveInventory("Berserker_Perk50", 1);
				TakeInventory("Doomguy_Perk50", 1);
				TakeInventory("Marine_Perk50", 1);
				TakeInventory("Hobo_Perk50", 1);
				TakeInventory("Punisher_Perk50", 1);
				TakeInventory("Wanderer_Perk50", 1);
				TakeInventory("Cyborg_Perk50", 1);
			break;
		}
	}
}

void HandleBerserkerRoar(int tid) {
	GiveActorInventory(tid, "Berserker_NoRoar", 1);
	ACS_NamedExecuteAlways("DnD Berserker Roar", 0, tid);
}

#endif
