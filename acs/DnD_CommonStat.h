#ifndef DND_COMMONSTAT_IN
#define DND_COMMONSTAT_IN

#include "DnD_Accessories.h"
#include "DnD_QuestDefs.h"
#include "DnD_InvAttribs.h"

#define DND_ACCURACY_CAP 100000

#define DND_SHIFTBITS_FOR_SLOTFROMFLAG 13 // 8192 must return 0 to us

#define DND_SYNTHMASK_EFFECT 4

#define DND_UNITY_DIVISOR 15

#define DND_SPREE_AMOUNT (4 * TICRATE) // 4 * 35
#define DND_SPREE_PER 10

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
	DND_AILMENT_OVERLOAD = 16
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
};

#include "DnD_CommonResearch.h"

#define DND_ELITE_BASEDROP 0.0025 // same as below

#define DND_ELITE_BASEDROP_ORB 0.0375
#define DND_MONSTER_ORBSTACK_LEVELTHRESHOLD 10

#define DND_BASEARMOR_DROP 0.0125
#define DND_BASE_CHARMRATE 0.01875
#define DND_BASE_POWERCORERATE 0.004

#define DND_BASE_PLAYERSPEED 0.9
#define DND_HP_PER_STR 2
#define DND_HP_PER_LVL 5

#define BASE_PET_CAP 3

#define ENDURANCE_RES_INTEGER 35 // 35 will be divided by 1000 to get 3.5%
#define ENDURANCE_RES_INC 0.035 // 3.5%
#define BASE_WISDOM_GAIN 10
#define BASE_GREED_GAIN 10
#define PERK_MEDICBONUS 10 // percent
#define PERK_MEDIC_ESBONUS 2 // percent
#define PERK_MEDICSTOREBONUS 15
#define DND_SAVAGERY_BONUS 20 // percent
#define PERK_DEADLINESS_BONUS 0.01 // 1%
#define DND_MUNITION_GAIN 10
#define DND_LUCK_GAIN 0.05 // 5% multiplicative luck
#define DND_LUCK_OUTCOME_GAIN 0.05

#define DND_SHARPSHOOTER_MASTERY_BONUS 0.01

#define DND_STR_KNOCKBACK_GAIN 50
#define DND_BASE_PLAYER_MASS 250

#define DND_BASE_ARMOR_SHOW 100
#define DND_BASE_ARMOR 200
#define DND_BASE_ARMORCAP 300

#define STAT_BASE 0
#define DND_STAT_MAX 1024 // for menu only, in game stats can go up to 1024
#define DND_STAT_FULLMAX 1024
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

#define DND_EXP_RES_ABILITY_BONUS 50

#include "DnD_ClassConstants.h"

global int 1: StatListOpened[MAXPLAYERS];

#define MAX_MAP_DIFFICULTIES 5
global int 4: MapDifficulty;

str GetMapDifficultyLabel(int id) {
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
		return (GetPlayerAttributeValue(pnum, INV_ESS_THORAX) && (extra1 | extra2));
		
		// +FORCERADIUSDMG can come from vaaj or marine
		case PUP_IGNORERADIUSIMMUNITY:
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
		return GetPlayerAttributeValue(pnum, INV_EX_CURSEIMMUNITY)/* || CheckActorInventory(pnum + P_TIDSTART, "Marine_Perk50")*/;
		
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

str GetAttributeLabel(int id) {
	return StrParam(s:"DND_MENU_ATTR", d:id + 1);
}

str GetAttributeLabel_Short(int id) {
	return StrParam(s:"DND_MENU_ATTR_SHORT", d:id + 1);
}

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

// returns true if there are things that'd nullify effect of dexterity
bool HasPlayerDexterityDisablers(int pnum) {
	return GetPlayerAttributeValue(pnum, INV_EX_UNITY);
}

// These getters must be used when doing calculations based on benefit of these stats
int GetDexterity(int pnum) {
	return (CheckActorInventory(pnum + P_TIDSTART, "PSTAT_Dexterity") + GetPlayerAttributeValue(pnum, INV_STAT_DEXTERITY)) * (100 + GetPlayerAttributeValue(pnum, INV_CORR_PERCENTSTATS)) / 100;
}

int GetDexterityEffect(int pnum, int factor, int divisor = 1) {
	if(!HasPlayerDexterityDisablers(pnum))
		return GetDexterity(pnum) * factor / divisor;
	return 0;
}

// returns true if there are things that'd nullify effect of int
bool HasPlayerIntellectDisablers(int pnum) {
	return GetPlayerAttributeValue(pnum, INV_EX_UNITY);
}

int GetIntellect(int pnum) {
	return (CheckActorInventory(pnum + P_TIDSTART, "PSTAT_Intellect") + GetPlayerAttributeValue(pnum, INV_STAT_INTELLECT)) * (100 + GetPlayerAttributeValue(pnum, INV_CORR_PERCENTSTATS)) / 100;
}

int GetIntellectEffect(int pnum, int factor, int divisor = 1) {
	if(!HasPlayerIntellectDisablers(pnum))
		return GetIntellect(pnum) * factor / divisor;
	return 0;
}

// returns true if there are things that'd nullify effect of strength
bool HasPlayerStrengthDisablers(int pnum) {
	return GetPlayerAttributeValue(pnum, INV_EX_UNITY) || GetPlayerAttributeValue(pnum, INV_EX_INTBONUSTOMELEE);
}

int GetStrength(int pnum) {
	return (CheckActorInventory(pnum + P_TIDSTART, "PSTAT_Strength") + GetPlayerAttributeValue(pnum, INV_STAT_STRENGTH)) * (100 + GetPlayerAttributeValue(pnum, INV_CORR_PERCENTSTATS)) / 100;
}

int GetStrengthEffect(int pnum, int factor, int divisor = 1) {
	if(!HasPlayerStrengthDisablers(pnum))
		return GetStrength(pnum) * factor / divisor;
	return 0;
}

// this sets player's unity item to cache it so we don't request it over and over in intense calculations
void CalculateUnity(int pnum) {
	int val = (
			CheckInventory("PSTAT_Strength") + 
			GetPlayerAttributeValue(pnum, INV_STAT_STRENGTH) +
			CheckInventory("PSTAT_Dexterity") + 
			GetPlayerAttributeValue(pnum, INV_STAT_DEXTERITY) +
			CheckInventory("PSTAT_Intellect") +
			GetPlayerAttributeValue(pnum, INV_STAT_INTELLECT)
		) * (100 + GetPlayerAttributeValue(pnum, INV_CORR_PERCENTSTATS)) / 100;
	SetInventory("PSTAT_Unity", val);
}

void ConditionalCalculateUnity(int pnum) {
	if(GetPlayerAttributeValue(pnum, INV_EX_UNITY))
		CalculateUnity(pnum);
}

int GetUnity() {
	return CheckInventory("PSTAT_Unity");
}

int GetPetCap() {
	return BASE_PET_CAP + HasPlayerPowerset(PlayerNumber(), PPOWER_PETCAP);
}

int GetActorPetCap(int tid) {
	return BASE_PET_CAP + HasPlayerPowerset(tid - P_TIDSTART, PPOWER_PETCAP);
}

bool CanActorHaveMorePets(int tid) {
	return CheckActorInventory(tid, "PetCounter") < GetActorPetCap(tid);
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

int GetActorResearchHealthBonuses(int tid) {
	int res = BIO_HP_ADD_1 * (CheckActorResearchStatus(tid, RES_BIO1) == RES_DONE);
	res += BIO_HP_ADD_2 * (CheckActorResearchStatus(tid, RES_BIO2) == RES_DONE);
	res += BIO_HP_ADD_3 * (CheckActorResearchStatus(tid, RES_BIO3) == RES_DONE);
	
	// cyborg's bonus
	if(CheckActorInventory(tid, "Cyborg_Perk50")) {
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

// returns player max health
int GetSpawnHealth(bool bypassEShieldCheck = false) {
	int pnum = PlayerNumber();

	if(!bypassEShieldCheck && GetPlayerAttributeValue(pnum, INV_EX_HEALTHATONE)) {
		SetInventory("PlayerHealthCap", 1);
		return 1;
	}

	int str_bonus = GetSTrengthEffect(pnum, DND_HP_PER_STR);
	int res = CalculateHealthCapBonuses(pnum) + DND_BASE_HEALTH + DND_HP_PER_LVL * (CheckInventory("Level") - 1) + str_bonus;
	// consider percent bonuses from here on
	int percent  = DND_TORRASQUE_BOOST * IsQuestComplete(0, QUEST_KILLTORRASQUE) 			+
				   CheckInventory("CelestialCheck") * CELESTIAL_BOOST 						+
				   GetResearchHealthBonuses() 												+
				   GetPlayerAttributeValue(pnum, INV_HPPERCENT_INCREASE);
	// player bonus + % research bonus
	res += (res * percent) / 100;
	if(IsAccessoryEquipped(ActivatorTID(), DND_ACCESSORY_ANGELICANKH))
		res >>= 1;

	if(GetPlayerAttributeValue(pnum, INV_EX_DOUBLE_HEALTHCAP))
		res <<= 1;

	if(res < DND_BASE_HEALTH)
		res = DND_BASE_HEALTH;
	// last bit here is necessary to fix a mugshot related bug that may still call this function properly and end up seeing our health is 1
	SetInventory("PlayerHealthCap", !GetPlayerAttributeValue(pnum, INV_EX_HEALTHATONE) ? res : 1);
	return res;
}

int GetPlayerSpawnHealth(int pnum, bool bypassEShieldCheck = false) {
	int tid = pnum + P_TIDSTART;
	if(!bypassEShieldCheck && GetPlayerAttributeValue(pnum, INV_EX_HEALTHATONE)) {
		SetActorInventory(pnum + P_TIDSTART, "PlayerHealthCap", 1);
		return 1;
	}

	int str_bonus = 0;
	if(!GetPlayerAttributeValue(pnum, INV_EX_UNITY))
		str_bonus = GetStrengthEffect(pnum, DND_HP_PER_STR);

	int res = CalculateHealthCapBonuses(pnum) + DND_BASE_HEALTH + DND_HP_PER_LVL * (CheckActorInventory(tid, "Level") - 1) + str_bonus;
	// consider percent bonuses from here on
	int percent  = DND_TORRASQUE_BOOST * IsQuestComplete(0, QUEST_KILLTORRASQUE) 			+
				   CheckActorInventory(tid, "CelestialCheck") * CELESTIAL_BOOST 			+
				   GetActorResearchHealthBonuses(tid) 										+
				   GetPlayerAttributeValue(pnum, INV_HPPERCENT_INCREASE);
	// player bonus + % research bonus
	res += (res * percent) / 100;
	if(IsAccessoryEquipped(tid, DND_ACCESSORY_ANGELICANKH))
		res >>= 1;
	if(res < DND_BASE_HEALTH)
		res = DND_BASE_HEALTH;
	// last bit here is necessary to fix a mugshot related bug that may still call this function properly and end up seeing our health is 1
	SetActorInventory(tid, "PlayerHealthCap", !GetPlayerAttributeValue(pnum, INV_EX_HEALTHATONE) ? res : 1);
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

int GetActorSpawnHealth(int t) {
	int caller = ActivatorTID();
	SetActivator(t);
	int cap = GetSpawnHealth();
	SetActivator(caller);
    return cap;
}

void HandleCurseImmunityRemoval() {
	// we shouldnt immediately take it as there might be other ways the player has obtained curse immunity
	if(!GetPlayerAttributeValue(PlayerNumber(), INV_EX_CURSEIMMUNITY))
		TakeInventory("CurseImmunity", 1);
}

void UpdatePlayerKnockbackResist() {
	int pnum = PlayerNumber();
	if(CheckUniquePropertyOnPlayer(pnum, PUP_KNOCKBACKIMMUNE))
		SetActorProperty(0, APROP_MASS, INT_MAX);
	else {
		int val = GetStrengthEffect(pnum, DND_STR_KNOCKBACK_GAIN);
		val += DND_BASE_PLAYER_MASS + GetPlayerAttributeValue(pnum, INV_KNOCKBACK_RESIST);

		int pct = (100 + GetPlayerAttributeValue(pnum, INV_PERCENT_KNOCKBACKRESIST));
		if(val < INT_MAX / pct)
			val = val * pct / 100;

		SetActorProperty(0, APROP_MASS, val);
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
		//GiveInventory("HateReduction", 1);
	}

	if(CheckInventory("GryphonCheck"))
		GiveInventory("CurseImmunity", 1);
		
	if(CheckUniquePropertyOnPlayer(pnum, PUP_CURSEIMMUNITY))
		GiveInventory("CurseImmunity", 1);
	
	// So the player respawns with his actual new max hp
	SetActorProperty(0, APROP_SPAWNHEALTH, GetSpawnHealth());
	HandleResearchBonuses();
	
	if(!(statflag & RES_NOCLASSPERK))
		HandleClassPerks(ActivatorTID());

	TakeInventory("IsBlinded", 1);
}

bool HasPlayerPowerset(int pnum, int power) {
	// powers are already bitfields here!
	return !!(GetPlayerAttributeValue(pnum, INV_EX_PLAYERPOWERSET1) & power);
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

void CalculatePlayerAccuracy(int pnum, int wepid = -1) {
	int acc = GetPlayerAttributeValue(pnum, INV_ACCURACY_INCREASE) + GetPlayerAttributeValue(pnum, INV_ESS_OMNISIGHT);
	int acc_pct = GetPlayerAttributeValue(pnum, INV_ESS_OMNISIGHT2);

	if(wepid == -1)
		wepid = CheckInventory("DnD_WeaponID");

	if(CheckInventory("Hobo_ShotgunFrenzyTimer") && IsBoomstick(wepid))
		acc_pct += DND_HOBO_ACCURACYBONUS;

	// omnisight essence gives % increased accuracy
	acc += (acc * acc_pct) / 100;
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
void HandleClassPerks(int tid) {
	int lvl = CheckActorInventory(tid, "Level");
	int class = CheckActorInventory(tid, "DnD_Character") - 1; // we use 0 based
	if(lvl >= 5) {
		// 1st perk
		switch(class) {
			case DND_PLAYER_DOOMGUY:
				GiveActorInventory(tid, "Doomguy_Perk5", 1);
				TakeActorInventory(tid, "Marine_Perk5", 1);
				TakeActorInventory(tid, "Hobo_Perk5", 1);
				TakeActorInventory(tid, "Punisher_Perk5", 1);
				TakeActorInventory(tid, "Wanderer_Perk5", 1);
				TakeActorInventory(tid, "Cyborg_Perk5", 1);
				TakeActorInventory(tid, "Berserker_Perk5", 1);
			break;
			case DND_PLAYER_MARINE:
				GiveActorInventory(tid, "Marine_Perk5", 1);
				TakeActorInventory(tid, "Doomguy_Perk5", 1);
				TakeActorInventory(tid, "Hobo_Perk5", 1);
				TakeActorInventory(tid, "Punisher_Perk5", 1);
				TakeActorInventory(tid, "Wanderer_Perk5", 1);
				TakeActorInventory(tid, "Cyborg_Perk5", 1);
				TakeActorInventory(tid, "Berserker_Perk5", 1);
			break;			
			case DND_PLAYER_HOBO:
				GiveActorInventory(tid, "Hobo_Perk5", 1);
				TakeActorInventory(tid, "Doomguy_Perk5", 1);
				TakeActorInventory(tid, "Marine_Perk5", 1);
				TakeActorInventory(tid, "Punisher_Perk5", 1);
				TakeActorInventory(tid, "Wanderer_Perk5", 1);
				TakeActorInventory(tid, "Cyborg_Perk5", 1);
				TakeActorInventory(tid, "Berserker_Perk5", 1);
			break;
			case DND_PLAYER_PUNISHER:
				GiveActorInventory(tid, "Punisher_Perk5", 1);
				TakeActorInventory(tid, "Doomguy_Perk5", 1);
				TakeActorInventory(tid, "Marine_Perk5", 1);
				TakeActorInventory(tid, "Hobo_Perk5", 1);
				TakeActorInventory(tid, "Wanderer_Perk5", 1);
				TakeActorInventory(tid, "Cyborg_Perk5", 1);
				TakeActorInventory(tid, "Berserker_Perk5", 1);
			break;
			case DND_PLAYER_WANDERER:
				GiveActorInventory(tid, "Wanderer_Perk5", 1);
				TakeActorInventory(tid, "Doomguy_Perk5", 1);
				TakeActorInventory(tid, "Marine_Perk5", 1);
				TakeActorInventory(tid, "Hobo_Perk5", 1);
				TakeActorInventory(tid, "Punisher_Perk5", 1);
				TakeActorInventory(tid, "Cyborg_Perk5", 1);
				TakeActorInventory(tid, "Berserker_Perk5", 1);
			break;
			case DND_PLAYER_CYBORG:
				GiveActorInventory(tid, "Cyborg_Perk5", 1);
				TakeActorInventory(tid, "Doomguy_Perk5", 1);
				TakeActorInventory(tid, "Marine_Perk5", 1);
				TakeActorInventory(tid, "Hobo_Perk5", 1);
				TakeActorInventory(tid, "Punisher_Perk5", 1);
				TakeActorInventory(tid, "Wanderer_Perk5", 1);
				TakeActorInventory(tid, "Berserker_Perk5", 1);
			break;
			case DND_PLAYER_BERSERKER:
				GiveActorInventory(tid, "Berserker_Perk5", 1);
				TakeActorInventory(tid, "Doomguy_Perk5", 1);
				TakeActorInventory(tid, "Marine_Perk5", 1);
				TakeActorInventory(tid, "Hobo_Perk5", 1);
				TakeActorInventory(tid, "Punisher_Perk5", 1);
				TakeActorInventory(tid, "Wanderer_Perk5", 1);
				TakeActorInventory(tid, "Cyborg_Perk5", 1);
			break;
		}
	}
	
	if(lvl >= 25) {
		// 2nd perk
		switch(class) {
			case DND_PLAYER_DOOMGUY:
				GiveActorInventory(tid, "Doomguy_Perk25", 1);
				TakeActorInventory(tid, "Marine_Perk25", 1);
				TakeActorInventory(tid, "Hobo_Perk25", 1);
				TakeActorInventory(tid, "Punisher_Perk25", 1);
				TakeActorInventory(tid, "Wanderer_Perk25", 1);
				TakeActorInventory(tid, "Cyborg_Perk25", 1);
				TakeActorInventory(tid, "Berserker_Perk25", 1);
			break;
			case DND_PLAYER_MARINE:
				GiveActorInventory(tid, "Marine_Perk25", 1);
				TakeActorInventory(tid, "Doomguy_Perk25", 1);
				TakeActorInventory(tid, "Hobo_Perk25", 1);
				TakeActorInventory(tid, "Punisher_Perk25", 1);
				TakeActorInventory(tid, "Wanderer_Perk25", 1);
				TakeActorInventory(tid, "Cyborg_Perk25", 1);
				TakeActorInventory(tid, "Berserker_Perk25", 1);
			break;			
			case DND_PLAYER_HOBO:
				GiveActorInventory(tid, "Hobo_Perk25", 1);
				TakeActorInventory(tid, "Doomguy_Perk25", 1);
				TakeActorInventory(tid, "Marine_Perk25", 1);
				TakeActorInventory(tid, "Punisher_Perk25", 1);
				TakeActorInventory(tid, "Wanderer_Perk25", 1);
				TakeActorInventory(tid, "Cyborg_Perk25", 1);
				TakeActorInventory(tid, "Berserker_Perk25", 1);
			break;
			case DND_PLAYER_PUNISHER:
				GiveActorInventory(tid, "Punisher_Perk25", 1);
				TakeActorInventory(tid, "Doomguy_Perk25", 1);
				TakeActorInventory(tid, "Marine_Perk25", 1);
				TakeActorInventory(tid, "Hobo_Perk25", 1);
				TakeActorInventory(tid, "Wanderer_Perk25", 1);
				TakeActorInventory(tid, "Cyborg_Perk25", 1);
				TakeActorInventory(tid, "Berserker_Perk25", 1);
			break;
			case DND_PLAYER_WANDERER:
				GiveActorInventory(tid, "Wanderer_Perk25", 1);
				TakeActorInventory(tid, "Doomguy_Perk25", 1);
				TakeActorInventory(tid, "Marine_Perk25", 1);
				TakeActorInventory(tid, "Hobo_Perk25", 1);
				TakeActorInventory(tid, "Punisher_Perk25", 1);
				TakeActorInventory(tid, "Cyborg_Perk25", 1);
				TakeActorInventory(tid, "Berserker_Perk25", 1);
			break;
			case DND_PLAYER_CYBORG:
				GiveActorInventory(tid, "Cyborg_Perk25", 1);
				TakeActorInventory(tid, "Doomguy_Perk25", 1);
				TakeActorInventory(tid, "Marine_Perk25", 1);
				TakeActorInventory(tid, "Hobo_Perk25", 1);
				TakeActorInventory(tid, "Punisher_Perk25", 1);
				TakeActorInventory(tid, "Wanderer_Perk25", 1);
				TakeActorInventory(tid, "Berserker_Perk25", 1);
			break;
			case DND_PLAYER_BERSERKER:
				GiveActorInventory(tid, "Berserker_Perk25", 1);
				TakeActorInventory(tid, "Doomguy_Perk25", 1);
				TakeActorInventory(tid, "Marine_Perk25", 1);
				TakeActorInventory(tid, "Hobo_Perk25", 1);
				TakeActorInventory(tid, "Punisher_Perk25", 1);
				TakeActorInventory(tid, "Wanderer_Perk25", 1);
				TakeActorInventory(tid, "Cyborg_Perk25", 1);
			break;
		}
	}
	
	if(lvl >= 50) {
		// 3rd perk
		switch(class) {
			case DND_PLAYER_DOOMGUY:
				GiveActorInventory(tid, "Doomguy_Perk50", 1);
				TakeActorInventory(tid, "Marine_Perk50", 1);
				TakeActorInventory(tid, "Hobo_Perk50", 1);
				TakeActorInventory(tid, "Punisher_Perk50", 1);
				TakeActorInventory(tid, "Wanderer_Perk50", 1);
				TakeActorInventory(tid, "Cyborg_Perk50", 1);
				TakeActorInventory(tid, "Berserker_Perk50", 1);
			break;
			case DND_PLAYER_MARINE:
				GiveActorInventory(tid, "Marine_Perk50", 1);
				TakeActorInventory(tid, "Doomguy_Perk50", 1);
				TakeActorInventory(tid, "Hobo_Perk50", 1);
				TakeActorInventory(tid, "Punisher_Perk50", 1);
				TakeActorInventory(tid, "Wanderer_Perk50", 1);
				TakeActorInventory(tid, "Cyborg_Perk50", 1);
				TakeActorInventory(tid, "Berserker_Perk50", 1);
			break;			
			case DND_PLAYER_HOBO:
				GiveActorInventory(tid, "Hobo_Perk50", 1);
				TakeActorInventory(tid, "Doomguy_Perk50", 1);
				TakeActorInventory(tid, "Marine_Perk50", 1);
				TakeActorInventory(tid, "Punisher_Perk50", 1);
				TakeActorInventory(tid, "Wanderer_Perk50", 1);
				TakeActorInventory(tid, "Cyborg_Perk50", 1);
				TakeActorInventory(tid, "Berserker_Perk50", 1);
			break;
			case DND_PLAYER_PUNISHER:
				GiveActorInventory(tid, "Punisher_Perk50", 1);
				TakeActorInventory(tid, "Doomguy_Perk50", 1);
				TakeActorInventory(tid, "Marine_Perk50", 1);
				TakeActorInventory(tid, "Hobo_Perk50", 1);
				TakeActorInventory(tid, "Wanderer_Perk50", 1);
				TakeActorInventory(tid, "Cyborg_Perk50", 1);
				TakeActorInventory(tid, "Berserker_Perk50", 1);
			break;
			case DND_PLAYER_WANDERER:
				GiveActorInventory(tid, "Wanderer_Perk50", 1);
				TakeActorInventory(tid, "Doomguy_Perk50", 1);
				TakeActorInventory(tid, "Marine_Perk50", 1);
				TakeActorInventory(tid, "Hobo_Perk50", 1);
				TakeActorInventory(tid, "Punisher_Perk50", 1);
				TakeActorInventory(tid, "Cyborg_Perk50", 1);
				TakeActorInventory(tid, "Berserker_Perk50", 1);
			break;
			case DND_PLAYER_CYBORG:
				GiveActorInventory(tid, "Cyborg_Perk50", 1);
				TakeActorInventory(tid, "Doomguy_Perk50", 1);
				TakeActorInventory(tid, "Marine_Perk50", 1);
				TakeActorInventory(tid, "Hobo_Perk50", 1);
				TakeActorInventory(tid, "Punisher_Perk50", 1);
				TakeActorInventory(tid, "Wanderer_Perk50", 1);
				TakeActorInventory(tid, "Berserker_Perk50", 1);
			break;
			case DND_PLAYER_BERSERKER:
				GiveActorInventory(tid, "Berserker_Perk50", 1);
				TakeActorInventory(tid, "Doomguy_Perk50", 1);
				TakeActorInventory(tid, "Marine_Perk50", 1);
				TakeActorInventory(tid, "Hobo_Perk50", 1);
				TakeActorInventory(tid, "Punisher_Perk50", 1);
				TakeActorInventory(tid, "Wanderer_Perk50", 1);
				TakeActorInventory(tid, "Cyborg_Perk50", 1);
			break;
		}
	}
}

int GetResistPenetration(int pnum, int category) {
	int val = 0;
	switch(category) {
		case DND_DAMAGECATEGORY_BULLET:
		case DND_DAMAGECATEGORY_MELEE:
			val = GetPlayerAttributeValue(pnum, INV_PEN_PHYSICAL);
		break;
		case DND_DAMAGECATEGORY_ENERGY:
			val = GetPlayerAttributeValue(pnum, INV_PEN_ENERGY);
		break;
		case DND_DAMAGECATEGORY_OCCULT:
			val = GetPlayerAttributeValue(pnum, INV_PEN_OCCULT);
		break;

		case DND_DAMAGECATEGORY_FIRE:
			val = GetPlayerAttributeValue(pnum, INV_PEN_FIRE);

			if(GetPlayerAttributeValue(pnum, INV_EX_ELEPENHARMONY))
				val = max(val, 
						max(
							GetPlayerAttributeValue(pnum, INV_PEN_ICE),
							max(
								GetPlayerAttributeValue(pnum, INV_PEN_LIGHTNING),
								GetPlayerAttributeValue(pnum, INV_PEN_POISON)
							)
						)
				);
			val += GetPlayerAttributeValue(pnum, INV_PEN_ELEMENTAL);
		break;
		case DND_DAMAGECATEGORY_ICE:
			val = GetPlayerAttributeValue(pnum, INV_PEN_ICE);

			if(GetPlayerAttributeValue(pnum, INV_EX_ELEPENHARMONY))
				val = max(val, 
						max(
							GetPlayerAttributeValue(pnum, INV_PEN_FIRE),
							max(
								GetPlayerAttributeValue(pnum, INV_PEN_LIGHTNING),
								GetPlayerAttributeValue(pnum, INV_PEN_POISON)
							)
						)
				);
			val += GetPlayerAttributeValue(pnum, INV_PEN_ELEMENTAL);
		break;
		case DND_DAMAGECATEGORY_LIGHTNING:
			val = GetPlayerAttributeValue(pnum, INV_PEN_LIGHTNING);

			if(GetPlayerAttributeValue(pnum, INV_EX_ELEPENHARMONY))
				val = max(val, 
						max(
							GetPlayerAttributeValue(pnum, INV_PEN_ICE),
							max(
								GetPlayerAttributeValue(pnum, INV_PEN_FIRE),
								GetPlayerAttributeValue(pnum, INV_PEN_POISON)
							)
						)
				);
			val += GetPlayerAttributeValue(pnum, INV_PEN_ELEMENTAL);
		break;
		case DND_DAMAGECATEGORY_POISON:
			val = GetPlayerAttributeValue(pnum, INV_PEN_POISON);

			if(GetPlayerAttributeValue(pnum, INV_EX_ELEPENHARMONY))
				val = max(val, 
						max(
							GetPlayerAttributeValue(pnum, INV_PEN_ICE),
							max(
								GetPlayerAttributeValue(pnum, INV_PEN_LIGHTNING),
								GetPlayerAttributeValue(pnum, INV_PEN_FIRE)
							)
						)
				);
			val += GetPlayerAttributeValue(pnum, INV_PEN_ELEMENTAL);
		break;

		case DND_DAMAGECATEGORY_SOUL:
			val = GetPlayerAttributeValue(pnum, INV_PEN_OCCULT) + GetPlayerAttributeValue(pnum, INV_EX_SOULWEPSPEN);
		break;
	}

	val += GetPlayerAttributeValue(pnum, INV_EX_UNITY_PEN_BONUS) * GetUnity() / DND_UNITY_DIVISOR;

	return val;
}

// purely used for cosmetic reasons
int GetHighestElePen(int pnum) {
	// this is very awful code but it doesn't matter -- we also dont care about unity or all ele pen... as they all would have it anyways
	int fpen = GetPlayerAttributeValue(pnum, INV_PEN_FIRE);
	int lpen = GetPlayerAttributeValue(pnum, INV_PEN_LIGHTNING);
	int ipen = GetPlayerAttributeValue(pnum, INV_PEN_ICE);
	int ppen = GetPlayerAttributeValue(pnum, INV_PEN_POISON);

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

void AddMonsterAilment(int tid, int ailment) {
	int prev = CheckActorInventory(tid, "DnD_AilmentToken");
	SetActorInventory(tid, "DnD_AilmentToken", prev | ailment);
}

int CountMonsterAilments(int tid) {
	int count = 0;
	int val = CheckActorInventory(tid, "DnD_AilmentToken");
	while(val) {
		++count;
		val >>= 1;
	}
	return count;
}

void HealMonster(int mid, int amount) {
	int hp = GetActorProperty(0, APROP_HEALTH);
	amount = Clamp_Between(amount, 0, MonsterProperties[mid].maxhp - hp);
	SetActorProperty(0, APROP_HEALTH, hp + amount);

	CheckDoomguyExecuteReversal(mid + DND_MONSTERTID_BEGIN);
}

#endif
