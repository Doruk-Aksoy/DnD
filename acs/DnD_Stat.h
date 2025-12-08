#ifndef DND_EXP_IN
#define DND_EXP_IN

#include "DnD_Quests.h"
#include "DnD_CommonStat.h"
#include "DnD_Common.h"
#include "DnD_Charms.h"
#include "DnD_Artifacts.h"
#include "DnD_Abilities.h"
#include "DnD_Activity.h"
#include "DnD_WeaponDefs.h"
#include "DnD_DamageCache.h"

#define DND_WELLROLL_ODDS 0.1

#define DND_BASE_CRITMODIFIER 200
#define DND_SAVAGERY_MASTERYBONUS 100
#define DND_HARDCORE_DROPRATEBONUS 0.15

#define ESHIELD_RECHARGEDELAY_BASE (10 * TICRATE) // base time is 10 seconds
#define ESHIELD_MIN_TIME 7
#define ESHIELD_RECOVERYRATE_TICS 1
#define DND_ESHIELD_NONE_BASE 50

#define EXO_AR_ADD_1 5
#define EXO_AR_ADD_2 6
#define EXO_AR_ADD_3 9

#define DND_ESTIMATED_AVG_DAMAGE 40 // This is used in estimated damage reduction for armor rating, assumes avg damage a player would receive throughout anything really

#define TALENT_CAP 100

#ifdef ISDEBUGBUILD
	#define EXP_SCALE_MAX 100
	#define CREDIT_SCALE_MAX 100
#else
	#define EXP_SCALE_MAX 10
	#define CREDIT_SCALE_MAX 10
#endif

#define BUDGET_SCALE_MAX 10

#define BLOODRUNE_LIFESTEAL_AMT 30.0
#define BLOODRUNE_LIFESTEAL_AMT2 45.0

// we multiply by 100 in calc
#define DND_DAMAGERESIST_FACTOR 10000
#define DND_BASE_DAMAGERESISTCAP 75.0
#define DND_MAX_DAMAGERESISTCAP 90.0

#define DND_TOXICOLOGY_REDUCE 0.33

#define DND_MAX_PET_DAMAGESHARE 9

#define LEVELDATA_CHESTSPAWNED 0
#define LEVELDATA_MAXCHESTS 1
#define LEVELDATA_WISDOMMASTERED 2 // counts of players who have mastered wisdom and greed perks
#define LEVELDATA_GREEDMASTERED 3
#define MAX_LEVEL_DATA_ITEMS 4
int CurrentLevelData[MAX_LEVEL_DATA_ITEMS];

enum {
	DND_ANNOUNCER_QUEST,
	DND_ANNOUNCER_ATTRIBPOINT,
	DND_ANNOUNCER_NEWCLASSPERK,
	DND_ANNOUNCER_LEGENDARYMONSTER,
	DND_ANNOUNCER_RESEARCHDISCOVER,
	DND_ANNOUNCER_INCURSION,
	DND_ANNOUNCER_TRADEREQUEST
};

// RPG ELEMENTS
//int LevelCurve[MAXLEVELS + 1] = { 150, 351, 623, 895, 1165, 1440, 1710, 1980, 2260, 2520, 2800, 3070, 3350, 3610, 3890, 4130, 4400, 4700, 5000, 5200, 5600, 5700, 6100, 6300, 6700, 6800, 7200, 7400, 7700, 8000, 8800, 9800, 11900, 14400, 17500, 21000, 25600, 30800, 37300, 45000, 54300, 64800, 79000, 96000, 114000, 139000, 166000, 201000, 241000, 290000, 348000, 419000, 504000, 596000, 730000, 870000, 1050000, 1260000, 1500000, 1810000, 2170000, 2600000, 3110000, 3730000, 4470000, 5330000, 6400000, 7700000, 9200000, 11000000, 13100000, 15800000, 18900000, 22500000, 27000000, 32200000, 38600000, 46000000, 54400000, 66000000, 79000000, 94000000, 112000000, 134000000, 160000000, 191000000, 229000000, 272000000, 326000000, 388000000, 463000000, 551000000, 660000000, 790000000, 930000000, 1120000000, 1340000000, 1590000000, 1910000000, 2147483647, 2147483647 };
int LevelCurve[MAXLEVELS + 1] = { 150, 351, 623, 895, 1165, 1440, 1710, 1980, 2260, 2520, 2800, 3070, 3350, 3610, 3890, 4130, 4400, 4700, 5000, 5200, 5600, 5700, 6100, 6300, 6700, 6800, 7200, 7400, 7700, 8000, 8500, 9200, 10700, 12700, 14900, 17600, 20700, 24400, 28700, 33800, 39800, 46800, 55000, 64000, 77000, 89000, 106000, 124000, 145000, 172000, 201000, 237000, 278000, 327000, 385000, 451000, 529000, 620000, 730000, 860000, 1010000, 1190000, 1390000, 1630000, 1920000, 2250000, 2650000, 3100000, 3640000, 4270000, 4990000, 5900000, 6900000, 8100000, 9500000, 11100000, 13100000, 15300000, 18000000, 21100000, 24700000, 29000000, 34000000, 39900000, 46600000, 54000000, 65000000, 75000000, 88000000, 104000000, 121000000, 142000000, 166000000, 195000000, 229000000, 267000000, 314000000, 368000000, 430000000, 495000000, 495000000 };

// database related
#define PLAYER_SAVESTATE 0
#define PLAYER_TRANSFERSTATE 1
#define MAX_PLAYER_DBSTATES (PLAYER_TRANSFERSTATE + 1)
global bool 9: PlayerDatabaseState[MAXPLAYERS][MAX_PLAYER_DBSTATES];
//global bool 10: TransactionMade;

int GetExpLimit() {
	return LevelCurve[GetLevel() - 1];
}

int GetExpLimit_Level(int lvl) {
	return LevelCurve[lvl - 1];
}

str GetTalentTag(int id) {
	return StrParam(s:"DND_TALENT", d:id + 1);
}

#define DND_PERKS (DND_PERK_END - DND_PERK_BEGIN + 1)
str StatData[STAT_LVL + 1] = {
	"PSTAT_Strength",
	"PSTAT_Dexterity",
	"PSTAT_Intellect",
	
	"Perk_Sharpshooting",
	"Perk_Brutality",
	"Perk_RiskAversion",
	"Perk_Wisdom",
	"Perk_Greed",
	"Perk_Medic",
	"Perk_Munitionist",
	"Perk_Deadliness",
	"Perk_Savagery",
	"Perk_Acrimony",
	
	"Level"
};

bool IsPlayerMoving(int pnum, int ptid) {
	int input = GetPlayerInput(pnum, INPUT_BUTTONS);
	return (input & (BT_FORWARD | BT_BACK | BT_MOVELEFT | BT_MOVERIGHT)) || abs(GetActorVelX(ptid)) > 1.0 || abs(GetActorVelY(ptid)) > 1.0 || abs(GetActorVelZ(ptid)) > 1.0;
}

void HandleHealDependencyCheck() {
	// Research dependencies
	if(CheckInventory("Research_Body_Hp_1_Tracker") == GetAmmoCapacity("Research_Body_Hp_1_Tracker") && CheckResearchStatus(RES_BIO1) == RES_NA)
		GiveResearch(RES_BIO1, true);
	
	// Quest records
	if(active_quest_id == QUEST_HEALFOR500 && !IsQuestComplete(ActivatorTID(), active_quest_id) && CheckInventory("DnD_MasterHealerQuest_HealAmount") >= DND_QUEST_MASTERHEALER_REQ)
		CompleteQuest(ActivatorTID(), active_quest_id);
	
	// Skin O' My Teeth check
	if(active_quest_id == QUEST_NOHEALINGPICKUP)
		FailQuest(ActivatorTID());
}

void HandleHealthPickup(int amt, int isSpecial, int useTarget, bool noMedkitStore = false, bool notPercentage = false) {
	if(useTarget)
		SetActivatorToTarget(0);
	int curhp = GetActorProperty(0, APROP_HEALTH);
	int healthcap = GetSpawnHealth();
	int pnum = PlayerNumber();

	// dont bother
	if(GetPlayerAttributeValue(pnum, INV_EX_HEALTHATONE))
		return;

	int bonus = GetHealingBonuses(pnum);
	// holds the old amt
	int toGive = amt, base = amt;
	// the percentage of spawn health is amt to be given
	if(!notPercentage)
		amt = healthcap * amt / 100;
	// consider healing bonuses from quests
	amt = amt * (100 + bonus) / 100;
	
	// health bonus
	if(isspecial == 6) { // x1.5 overheal
		toGive = healthcap * 3 / 2 - curhp;
	}
	else if(isspecial == 5) {
		// fixed to always go up to x2 health cap
		toGive = healthcap * 2 - curhp;
	}
	else if(isspecial == 3) {
		// map toast heal
		amt = healthcap - curhp;
	    GiveInventory("HealthBonusX", amt);
		GiveInventory("DnD_MasterHealerQuest_HealAmount", amt);
		GiveInventory("Research_Body_Hp_1_Tracker", amt);
	    return;
	}
	else if(isspecial == 2) { // ubersphere / megasphere
		toGive /= 100; // get the multiplier for mega / uber
		toGive = healthcap * toGive - curhp;
	}
	else if(isspecial == 1)
		toGive = amt + healthcap - curhp;
	else // for anything else (stims, medkits etc)
	    toGive = healthcap - curhp;
	
	if(toGive > amt)
		toGive = amt;
	
	if(!noMedkitStore && CheckResearchStatus(RES_MEDKITSTORE) == RES_DONE && !isspecial) {
		if(curhp < healthcap) { // if my current curhp is less than max
			GiveInventory("HealthBonusX", toGive);
			GiveInventory("DnD_MasterHealerQuest_HealAmount", toGive);
			GiveInventory("Research_Body_Hp_1_Tracker", toGive);
		}
        if(toGive < amt)
            GiveInventory("StoredMedkit", Clamp_Between(amt - toGive, 1, base));
	}
	else {
		GiveInventory("HealthBonusX", toGive);
		GiveInventory("DnD_MasterHealerQuest_HealAmount", toGive);
		GiveInventory("Research_Body_Hp_1_Tracker", toGive);
	}

	if(HasMasteredPerk(STAT_MED)) {
		bonus = GetPlayerEnergyShieldCap(PlayerNumber());
		amt = toGive * PERK_MEDIC_ESBONUS / 100;
		if(amt && CheckInventory("EShieldAmount") < bonus)
			AddEnergyShield(amt);
		else
			SetEnergyShield(bonus);
	}
	
	HandleHealDependencyCheck();
}

#define DND_SPECIALTYARMOR_REDUCE 75
#define DND_SPECIALTYARMOR_BUFF 50
#define DND_KNIGHT_MELEEREDUCE 50
#define DND_KNIGHTARMOR_MELEEWEP_BONUS 0.4

#define DND_RAVAGER_FACTOR 17
#define DND_RAVAGER_REDUCE 20
#define DND_RAVAGER_DMGBONUS 25

#define DND_SYNTHMETAL_HITSCANBUFF 50 // 50%
#define DND_SYNTHMETAL_LIGHTNINGNERF 50
#define DND_LIGHTNINGCOIL_SPECIAL 85 // 85%

/*int GetStat(int stat_id) {
	return CheckInventory(StatData[stat_id]);
}*/

/*int GetActorStat(int tid, int stat_id) {
	return CheckActorInventory(tid, StatData[stat_id]);
}*/

int GetLevel() {
	return CheckInventory(StatData[STAT_LVL]);
}

int GetActorLevel(int tid) {
	return CheckActorInventory(tid, StatData[STAT_LVL]);
}

int GetActorLevelExperience(int tid) {
	return PlayerDataInLevel[tid - P_TIDSTART].levelexp;
}

int GetActorLevelCredits(int tid) {
	return PlayerDataInLevel[tid - P_TIDSTART].levelcredit;
}

int GetPerk(int attr) {
	return CheckInventory(StatData[attr]);
}

void SetPerk(int attr, int val) {
	SetInventory(StatData[attr], val);
}

int GetActorPerk(int tid, int attr) {
	return CheckActorInventory(tid, StatData[attr]);
}

str GetStatLabel(int id) {
	if(id >= DND_PERK_BEGIN && id <= DND_PERK_END)
		return StrParam(s:"DND_PERK", d:id + 1 - DND_PERK_BEGIN);
	return StrParam(s:"DND_STAT", d:id + 1);
}

int GetPlayerEnergyShieldPercent(int pnum) {
	int cap = GetPlayerEnergyShieldCap(pnum);
	if(!cap)
		return 0;
	return 100 * CheckInventory("EShieldAmount") / cap;
}

int GetPlayerEnergyShieldRechargeDelay(int pnum) {
	int res = ESHIELD_RECHARGEDELAY_BASE;
	res = res * 100 / (100 + GetPlayerAttributeValue(pnum, INV_SHIELD_RECHARGEDELAY));
	if(res < ESHIELD_MIN_TIME)
		res = ESHIELD_MIN_TIME;
	return res;
}

int GetPlayerEnergyShieldRecoveryRate(int pnum, int cap) {
	int bonus = 1;
	if(GetPlayerAttributeValue(pnum, INV_EX_PLAYERPOWERSET1) & PPOWER_CYBER)
		bonus = 2;
	
	int pct = 100 + GetPlayerAttributeValue(pnum, INV_SHIELD_RECOVERYRATE);
	int res = cap * bonus;

	if(bonus > 1 && HasClassPerk_Fast("Cyborg", 1)) {
		res += res * DND_CYBERNETIC_FACTOR / 100;
	}

	if(pct != 100)
		res = res * pct / 100;
	res /= 333;

	if(GetPlayerAttributeValue(pnum, INV_INC_ESHIELDNOINTERRUPT))
		res /= 2;

	if(!res)
		res = 1;
	return res;
}

// Returns 0 if player can't regen yet, otherwise returns player's eshield cap
int CanRegenEShield(int pnum) {
	int cap = GetPlayerEnergyShieldCap(pnum);
	if
	(
		cap &&
		CheckInventory("EShieldAmount") < cap &&
		!CheckInventory("EShieldCharging") &&
		(GetPlayerAttributeValue(pnum, INV_EX_ESCHARGE_DMGNOINTERRUPT) || GetPlayerAttributeValue(pnum, INV_INC_ESHIELDNOINTERRUPT) || !CheckInventory("DnD_Hit_CombatTimer")) &&
		!CheckInventory("TaltosUp")
	)
	{
		// finally consider the special condition that this could be from tesseract
		int tmp = GetPlayerEnergyShieldRecoveryRate(pnum, cap);
		int use_hp = GetPlayerAttributeValue(pnum, INV_EX_ESCHARGE_USEHP);
		if(use_hp) {
			// take life from player now
			use_hp = tmp * use_hp / 100;
			if(use_hp <= 0)
				use_hp = 1;

			// player can't afford to regen, ignore
			if(GetActorProperty(0, APROP_HEALTH) - use_hp <= 0)
				return 0;
		}
		return cap;
	}
	return 0;
}

int GetPlayerEstimatedArmorProtect(int pnum, int cap) {
	int base_dmg = DND_ESTIMATED_AVG_DAMAGE * (100 + GetMonsterDMGScaling(0, GetActorLevel(pnum + P_TIDSTART), true, DND_DEFAULT_MONSTERDMG_SCALING_FACTOR, 40)) / 100;
	return 100 - DoArmorRatingEffect(base_dmg, cap) * 100 / base_dmg;
}

void GiveStat(int stat_id, int amt) {
	// get cap
	int lim = stat_id <= DND_ATTRIB_END ? DND_STAT_FULLMAX : DND_PERK_MAX;
	amt = Clamp_Between(CheckInventory(StatData[stat_id]) + amt, 0, lim) - CheckInventory(StatData[stat_id]);
	GiveInventory(StatData[stat_id], amt);
	
	if(lim == DND_STAT_FULLMAX)
		UpdateActivity(PlayerNumber(), DND_ACTIVITY_ATTRIBUTE, amt, stat_id);
	else if(lim == DND_PERK_MAX) {
		UpdateActivity(PlayerNumber(), DND_ACTIVITY_PERK, amt, stat_id - DND_PERK_BEGIN);
		UpdatePerkStuff(stat_id);
	}

	// visual updates
	if(stat_id == STAT_STR) {
		UpdatePlayerKnockbackResist();
		SetActorProperty(0, APROP_SPAWNHEALTH, GetSpawnHealth());
	}
}

// Takes a stat from the player, also removing effects of it
void TakeStat(int stat_id, int amt) {
	// printbold(s:"take stat ", d:stat_id, s: " amt: ", d:amt);
	TakeInventory(StatData[stat_id], amt);
	
	if(stat_id <= DND_ATTRIB_END)
		UpdateActivity(PlayerNumber(), DND_ACTIVITY_ATTRIBUTE, -amt, stat_id);
	else if(stat_id <= DND_PERK_END) {
		UpdateActivity(PlayerNumber(), DND_ACTIVITY_PERK, -amt, stat_id - DND_PERK_BEGIN);
		UpdatePerkStuff(stat_id);
	}
		
	// visual updates
	if(stat_id == STAT_STR) {
		UpdatePlayerKnockbackResist();
		SetActorProperty(0, APROP_SPAWNHEALTH, GetSpawnHealth());
	}
}

bool HasMasteredPerk(int stat) {
	return GetPerk(stat) == DND_PERK_MAX;
}

bool HasActorMasteredPerk(int tid, int stat) {
	return GetActorPerk(tid, stat) == DND_PERK_MAX;
}

bool CheckPlayerLuckDuplicator(int pnum) {
	return false;
	//return HasActorMasteredPerk(pnum + P_TIDSTART, X) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE;
}

void SpawnPlayerDrop(int pnum, str actor, int zoffset, int thrust, int setspecial, int setspecial2, bool noRandomVelXY = false) {
	SpawnDrop(actor, zoffset, thrust, setspecial, setspecial2, noRandomVelXY);
	//if(CheckPlayerLuckDuplicator(pnum))
	//	SpawnDrop(actor, zoffset, thrust, setspecial, setspecial2);
}

void SpawnPlayerDropAtActor(int pnum, int dest, str actor, int zoffset, int thrust, int setspecial, int setspecial2, bool noRandomVelXY = false) {
	SpawnDropAtActor(dest, actor, zoffset, thrust, setspecial, setspecial2, noRandomVelXY);
	//if(CheckPlayerLuckDuplicator(pnum))
	//	SpawnDropAtActor(dest, actor, zoffset, thrust, setspecial, setspecial2);
}

bool CheckWellRolled(int pnum) {
	return CheckActorInventory(pnum + P_TIDSTART, "ReveranceUsed") || random(0, 1.0) <= DND_WELLROLL_ODDS;
}

void CalculateExpRatio() {
	int lvl = CheckInventory("Level");
	int cap = GetExpLimit_Level(lvl);
	if(lvl > 50)
		SetInventory("ExpVisual", (1000 * FixedDiv(GetPlayerExp(PlayerNumber()), cap)) >> 16);
	else
		SetInventory("ExpVisual", 1000 * GetPlayerExp(PlayerNumber()) / cap);
}

void CalculatePlayerExpRatio(int tid) {
	int lvl = CheckActorInventory(tid, "Level");
	int cap = GetExpLimit_Level(lvl);
	if(lvl > 50)
		SetActorInventory(tid, "ExpVisual", (1000 * FixedDiv(GetPlayerExp(tid - P_TIDSTART), cap)) >> 16);
	else
		SetActorInventory(tid, "ExpVisual", 1000 * GetPlayerExp(tid - P_TIDSTART) / cap);
}

void GiveExp(int amt, bool resetSpree = false) {
	int pnum = PlayerNumber();
	AddPlayerExp(pnum, amt);

	if(!resetSpree)
		GiveInventory("SpreeXP", amt);
	else
		SetInventory("SpreeXP", 0);

	PlayerDataInLevel[pnum].levelexp += amt;
	ACS_NamedExecuteAlways("DnD Player Levelup Check", 0, ActivatorTID());
	CalculateExpRatio();
}

void GiveActorExp(int tid, int amt) {
	int pnum = tid - P_TIDSTART;
	AddPlayerExp(pnum, amt);
	GiveActorInventory(tid, "SpreeXP", amt);
	PlayerDataInLevel[pnum].levelexp += amt;
	ACS_NamedExecuteAlways("DnD Player Levelup Check", 0, tid);
	CalculatePlayerExpRatio(tid);
}

int GetPlayerWisdomBonus(int pnum, int tid) {
	// the item modifier is fixed point and is a more multiplier
	int base = 100 + BASE_WISDOM_GAIN * CheckActorInventory(tid, "Perk_Wisdom");
	
	// halved
	if(IsAccessoryEquipped(tid, DND_ACCESSORY_TALISMANGREED))
		base >>= 1;
		
	// 50% increase
	if(IsAccessoryEquipped(tid, DND_ACCESSORY_TALISMANWISDOM))
		base = base * 3 / 2;
	
	return ApplyFixedFactorToInt(base, GetPlayerAttributeValue(pnum, INV_EXPGAIN_INCREASE));
}

int GetPlayerGreedBonus(int pnum, int tid) {
	// the item modifier is fixed point and is a more multiplier
	int base = 100 + BASE_GREED_GAIN * CheckActorInventory(tid, "Perk_Greed");
	
	// halved
	if(IsAccessoryEquipped(tid, DND_ACCESSORY_TALISMANWISDOM))
		base >>= 1;
		
	// 50% increase
	if(IsAccessoryEquipped(tid, DND_ACCESSORY_TALISMANGREED))
		base = base * 3 / 2;
	
	return ApplyFixedFactorToInt(base, GetPlayerAttributeValue(pnum, INV_CREDITGAIN_INCREASE));
}

int RewardActorExp(int tid, int amt) {
	int tmp = GetActorLevel(tid);
	// for now, return 0 if player is max level, maybe later we can do something with exp beyond this
	if(tmp == MAXLEVELS)
		return 0;

	if(tmp >= DND_EXP_ADJUST_LEVEL)
		amt = amt * ((DnD_Constants[DND_CONSTANT_EXPCURVE][tmp - DND_EXP_ADJUST_LEVEL] * 100) >> 16) / 100;

	amt = amt * GetPlayerWisdomBonus(tid - P_TIDSTART, tid) / 100;

	if(PlayerActivities[tid - P_TIDSTART].loot_penalty)
		amt /= MAPLOOTPENALITY_FACTOR;

	GiveActorExp(tid, amt);
	return amt;
}

int RewardActorCredit(int tid, int amt) {
	amt = amt * GetPlayerGreedBonus(tid - P_TIDSTART, tid) / 100;

	if(PlayerActivities[tid - P_TIDSTART].loot_penalty)
		amt /= MAPLOOTPENALITY_FACTOR;

	GiveActorCredit(tid, amt);
	return amt;
}

void GiveCredit(int amt) {
	int pnum = PlayerNumber();
	GiveInventory("Credit", amt);
	PlayerDataInLevel[pnum].levelcredit += amt;
	ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
	GiveInventory("DnD_MoneySpentQuest", amt);
	UpdateActivity(pnum, DND_ACTIVITY_CREDIT, amt, 0);
}

void TakeCredit(int amt) {
	int pnum = PlayerNumber();
	TakeInventory("Credit", amt);
	ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
	UpdateActivity(pnum, DND_ACTIVITY_CREDIT, -amt, 0);
}

void GiveBudget(int amt) {
	GiveInventory("Budget", amt * Clamp_Between(GetCVar("dnd_budget_scale"), 1, BUDGET_SCALE_MAX));
	ACS_NamedExecuteAlways("DnD Refresh Request", 0, PlayerNumber(), 1);
	UpdateActivity(PlayerNumber(), DND_ACTIVITY_BUDGET, amt, 0);
}

void TakeBudget(int amt) {
	TakeInventory("Budget", amt);
	ACS_NamedExecuteAlways("DnD Refresh Request", 0, PlayerNumber(), 1);
	UpdateActivity(PlayerNumber(), DND_ACTIVITY_BUDGET, -amt, 0);
}

void GiveActorBudget(int tid, int amt) {
	GiveActorInventory(tid, "Budget", amt * Clamp_Between(GetCVar("dnd_budget_scale"), 1, BUDGET_SCALE_MAX));
	ACS_NamedExecuteAlways("DnD Refresh Request", 0, tid - P_TIDSTART, 1);
	UpdateActivity(tid - P_TIDSTART, DND_ACTIVITY_BUDGET, amt, 0);
}

void GiveActorCredit(int tid, int amt) {
	int pnum = tid - P_TIDSTART;
	GiveActorInventory(tid, "Credit", amt);
	PlayerDataInLevel[pnum].levelcredit += amt;
	ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
	UpdateActivity(pnum, DND_ACTIVITY_CREDIT, amt, 0);
}

int GetPlayerCredit(int pnum) {
	return CheckActorInventory(pnum + P_TIDSTART, "Credit");
}

void ConsumeAttributePointOn(int pnum, int stat, int amt) {
	GiveStat(stat, amt);
	UpdateActivity(pnum, DND_ACTIVITY_ATTRIBUTEPOINT, -amt, 0);

	if(stat == STAT_INT)
		UpdateEnergyShieldVisual(GetPlayerEnergyShieldCap(pnum));

	TakeInventory("AttributePoint", amt);

	CalculateUnity(pnum);
}

void ConsumePerkPointOn(int perk, int amt) {
	GiveStat(perk, amt);
	UpdateActivity(PlayerNumber(), DND_ACTIVITY_PERKPOINT, -amt, 0);
	TakeInventory("PerkPoint", amt);
}

bool ReachedAccessoryLimit() {
	// consider accessory limit enhancement here
	int baselimit = DND_ACCESSORY_BASELIMIT;
	int cur = CheckInventory("Accessory_Index");
	int count = 0;
	// counts bits of 1
	while(cur && count < baselimit) {
		if(cur & 1)
			++count;
		cur >>= 1;
	}
	return count >= baselimit;
}

void EquipAccessory(int acc) {
	SetInventory("Accessory_Index", CheckInventory("Accessory_Index") | acc);
	DecideAccessories();
}

void UnequipAccessory(int acc) {
	SetInventory("Accessory_Index", CheckInventory("Accessory_Index") & ~acc);
	DecideAccessories();
}

int GetResearchArmorBonuses() {
	int res = EXO_AR_ADD_1 * (CheckResearchStatus(RES_EXO1) == RES_DONE);
	res += EXO_AR_ADD_2 * (CheckResearchStatus(RES_EXO2) == RES_DONE);
	res += EXO_AR_ADD_3 * (CheckResearchStatus(RES_EXO3) == RES_DONE);
	
	if(HasClassPerk_Fast("Cyborg", 1)) {
		res *= DND_CYBORG_CYBER_MULT;
		res /= DND_CYBORG_CYBER_DIV;
	}
	
	return res;
}

int CanPickHealthItem(int type) {
	int res = 0;
    int research_cond = CheckResearchStatus(RES_MEDKITSTORE) == RES_DONE && CheckInventory("StoredMedkit") < GetAmmoCapacity("StoredMedkit");
	if(type == 2) // portable medikit
        res = GetActorProperty(0, APROP_HEALTH) < GetSpawnHealth() && CheckInventory("StoredMedkit");
    else if(type == 1) // the artifact kit
		res = GetActorProperty(0, APROP_HEALTH) < GetSpawnHealth();
	else // normal health pickup
		res = GetActorProperty(0, APROP_HEALTH) < GetSpawnHealth() || research_cond;
		
	return res;
}

// used for deciding armor pickup values
int GetPlayerArmor(int pnum) {
	int amt = CheckInventory("DnD_QuestReward_ArmorCapIncrease") * DND_QUEST_ARMORBONUS + GetPlayerAttributeValue(pnum, INV_ARMOR_INCREASE);
	int inc = 0;
	
	// other bonuses
	inc = 	DND_QUEST_ARMORBONUS * IsQuestComplete(0, QUEST_NOARMORS);
			DND_TORRASQUE_BOOST * IsQuestComplete(0, QUEST_KILLTORRASQUE);
	
	amt += amt * inc / 100;
	amt += (amt * CheckInventory("CelestialCheck") * CELESTIAL_BOOST) / 100;
	amt += (amt * GetResearchArmorBonuses()) / 100;
	amt += (amt * GetPlayerAttributeValue(pnum, INV_ARMORPERCENT_INCREASE)) / 100;
	return amt;
}

int Calculate_Stats() {
	int res = 0;
	for(int i = DND_ATTRIB_BEGIN; i <= DND_ATTRIB_END; ++i)
		res += CheckInventory(StatData[i]);
	return res;
}

int Calculate_Perks() {
	int res = 0;
	for(int i = DND_PERK_BEGIN; i <= DND_PERK_END; ++i)
		res += CheckInventory(StatData[i]);
	return res;
}

// this is used in drop rates, weapons proc chances etc.
int GetPlayerLuck(int pnum, int outcome_val = DND_LUCK_GAIN) {
	//return outcome_val * GetActorPerk(pnum + P_TIDSTART, X) + GetPlayerAttributeValue(pnum, INV_LUCK_INCREASE);
	return GetPlayerAttributeValue(pnum, INV_LUCK_INCREASE);
}

bool RunLuckBasedChance(int pnum, int base, int outcome_val = DND_LUCK_GAIN) {
	int r = random(0, 1.0);
	return r <= FixedMul(base, 1.0 + GetPlayerLuck(pnum, outcome_val));
}

// this is the generic drop chance factor
int GetDropChance(int pnum) {
	int base = 1.0; // base val
	// additive bonuses first
	base += GetPlayerAttributeValue(pnum, INV_DROPCHANCE_INCREASE);
			
	//if(isElite && IsQuestComplete(pnum + P_TIDSTART, QUEST_KILL20ELITES))
	//	base += DND_ELITEDROP_GAIN;
		
	// more chance to find loot
	base = FixedMul(base, 1.0 + GetPlayerLuck(pnum));
	if(GetCVar("dnd_mode") == DND_MODE_HARDCORE)
		base = FixedMul(base, 1.0 + DND_HARDCORE_DROPRATEBONUS);

	// chances reduced to 25%
	if(PlayerActivities[pnum].loot_penalty)
		base /= MAPLOOTPENALITY_FACTOR;

	return base;
}

int GetPlayerItemRarity(int pnum) {
	int base = 1.0;

	base += GetPlayerAttributeValue(pnum, INV_ITEMRARITY);

	// more chance for rarity -- only at half
	base = FixedMul(base, 1.0 + GetPlayerLuck(pnum) / 2);
	if(GetCVar("dnd_mode") == DND_MODE_HARDCORE)
		base = FixedMul(base, 1.0 + DND_HARDCORE_DROPRATEBONUS);

	return base;
}

bool RunDefaultDropChance(int pnum, int basechance) {
	return RunDropChance(pnum, basechance, 0, 1.0);
}

bool RunPrecalcDropChance(int p_chance, int basechance, int m_id, int rng_id) {
	return FixedMul(p_chance, basechance) >= MonsterProperties[m_id].rng_vals[rng_id];
}

bool RunDropChance(int pnum, int basechance, int low, int high) {
	//printbold(s:"dc ", f:GetDropChance(pnum), s: " x ", f:basechance, s: " = ", f:FixedMul(GetDropChance(pnum), basechance));
	return FixedMul(GetDropChance(pnum), basechance) >= random(low, high);
}

bool RunDefaultRarityChance(int pnum, int basechance) {
	return FixedMul(GetPlayerItemRarity(pnum), basechance) >= random(0.0, 1.0);
}

void DecideAccessories() {
	int this = ActivatorTID();
	if(IsAccessoryEquipped(this, DND_ACCESSORY_AMULETHELLFIRE))
		GiveInventory("HellfireCheck", 1);
	else
		TakeInventory("HellfireCheck", 1);
		
	if(IsAccessoryEquipped(this, DND_ACCESSORY_ANGELICANKH)) {
		if(!CheckInventory("Intervened")) {
			SetPlayerProperty(0, 1, PROP_BUDDHA);
			GiveInventory("CanIntervene", 1);
		}
	}
	else {
		SetPlayerProperty(0, 0, PROP_BUDDHA);
		TakeInventory("CanIntervene", 1);
	}
	
	if(IsAccessoryEquipped(this, DND_ACCESSORY_DEMONBANE))
		GiveInventory("DemonBaneCheck", 1);
	else
		TakeInventory("DemonBaneCheck", 1);
	
	if(IsAccessoryEquipped(this, DND_ACCESSORY_PENDANTREFLECTION))
		GiveInventory("CanDeflect", 1);
	else
		TakeInventory("CanDeflect", 1);
		
	if(IsAccessoryEquipped(this, DND_ACCESSORY_NETHERMASK))
		GiveInventory("NetherCheck", 1);
	else
		TakeInventory("NetherCheck", 1);

	if(IsAccessoryEquipped(this, DND_ACCESSORY_TALTOSRING)) {
		GiveInventory("TaltosEffect", 1);
		GiveInventory("TaltosUp", 1);

		if(CheckInventory("EShieldAmount"))
			SetInventory("EShieldAmount", 0);
	}
	else {
		// take the ghost portion away as well if unequipped so people can't cheese and are stuck with no healing
		GiveInventory("TaltosUnsetEffect", 1);
		if(IsSetupComplete(SETUP_STATE1, SETUP_MAPCHANGED)) {
			// only let player take these away if map has changed, so they have to commit
			TakeInventory("TaltosUp", 1);
		}
	}
	
	if(IsAccessoryEquipped(this, DND_ACCESSORY_HATESHARD))
		GiveInventory("HateCheck", 1);
	else
		TakeInventory("HateCheck", 1);
	
	if(IsAccessoryEquipped(this, DND_ACCESSORY_HANDARTEMIS)) {
		GiveInventory("ArtemisPower", 1);
		GiveInventory("ArtemisCheck", 1);
	}
	else {
		TakeInventory("ArtemisPower", 1);
		TakeInventory("ArtemisCheck", 1);
	}
	
	if(IsAccessoryEquipped(this, DND_ACCESSORY_AGAMOTTO))
		GiveInventory("AgamottoCheck", 1);
	else
		TakeInventory("AgamottoCheck", 1);
	
	if(IsAccessoryEquipped(this, DND_ACCESSORY_GRYPHONBOOTS)) {
		GiveInventory("CurseImmunity", 1);
		GiveInventory("GryphonCheck", 1);
		UpdatePlayerKnockbackResist();
		SetActorProperty(0, APROP_SPEED, GetPlayerSpeed(PlayerNumber()));
	}
	else {
		HandleCurseImmunityRemoval();
		TakeInventory("GryphonCheck", 1);
		SetActorProperty(0, APROP_SPEED, GetPlayerSpeed(PlayerNumber()));
	}
	
	if(IsAccessoryEquipped(this, DND_ACCESSORY_LICHARM)) {
		GiveInventory("LichCheck", 1);
		SetAmmoCapacity("Souls", AmmoInfo[DND_AMMOSLOT_SOULS][AMMO_SOUL].initial_capacity * DND_LICH_SOULFACTOR);
	}
	else {
		TakeInventory("LichCheck", 1);
		int tmp = GetAmmoCapacity("Souls");
		if(tmp > AmmoInfo[DND_AMMOSLOT_SOULS][AMMO_SOUL].initial_capacity) {
			SetAmmoCapacity("Souls", tmp / 2);
			if(CheckInventory("Souls") > tmp / 2)
				SetInventory("Souls", tmp / 2);
		}
		else
			SetAmmoCapacity("Souls", AmmoInfo[DND_AMMOSLOT_SOULS][AMMO_SOUL].initial_capacity);
	}
	
	// sigil order: 1 = fire, 2 = ice, 3 = lightning, 4 = poison
	if(IsAccessoryEquipped(this, DND_ACCESSORY_SIGILELEMENTS))
		SetInventory("SigilCheck", 1);
	else {
		SetInventory("SigilCheck", 0);
		TakeInventory("ElementPower_Fire", 1);
		TakeInventory("ElementPower_Ice", 1);
		TakeInventory("ElementPower_Lightning", 1);
		TakeInventory("ElementPower_Earth", 1);
	}
	if(IsAccessoryEquipped(this, DND_ACCESSORY_CELESTIAL))
		SetInventory("CelestialCheck", 1);
	else
		SetInventory("CelestialCheck", 0);
	
	SetInventory("HunterTalismanCheck", IsAccessoryEquipped(this, DND_ACCESSORY_HUNTERTALISMAN));
	SetInventory("VeilCheck", IsAccessoryEquipped(this, DND_ACCESSORY_VEIL));
	
	// when deciding which accessory should be used, do a cache recalc request in any case
	ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, PlayerNumber());
}

bool HasNoSigilPower() {
	return !CheckInventory("ElementPower_Fire") && !CheckInventory("ElementPower_Ice") && !CheckInventory("ElementPower_Lightning") && !CheckInventory("ElementPower_Earth");
}

void UpdatePerkStuff(int perk) {
	if(perk == STAT_MED)
		SetAmmoCapacity("StoredMedkit", GetAmmoCapacity("StoredMedkit") + 15 * CheckInventory("Perk_Medic"));
	else if(perk == STAT_WIS && CheckInventory(StatData[perk]) == DND_PERK_MAX)
		++CurrentLevelData[LEVELDATA_WISDOMMASTERED];
	else if(perk == STAT_GRE && CheckInventory(StatData[perk]) == DND_PERK_MAX)
		++CurrentLevelData[LEVELDATA_GREEDMASTERED];
}

bool HasKilledLegendary(int id) {
	return IsSet(CheckInventory("LegendaryKills"), id);
}

void UpdateLegendaryKill(int pnum, int mon_id) {
	SetActorInventory(pnum + P_TIDSTART, "LegendaryKills", SetBit(CheckActorInventory(pnum + P_TIDSTART, "LegendaryKills"), mon_id));
}

int GetPlayerWeaponEnchant(int pnum, int wepid) {
	return Player_Weapon_Infos[pnum][wepid].quality;
}

// break all trades between this player and others
void BreakTradesBetween(int pnum) {
	int i;
	int tid;
	// check all trades of all players, clean players who have one going with this guy
	for(int j = 0; j < MAXPLAYERS; ++j) {
		if(PlayerInGame(j)) {
			tid = j + P_TIDSTART;
			if(pnum > 31) {
				if(IsSet(CheckActorInventory(tid, "DnD_TradeEngaged_2"), pnum - 32)) {
					SetActorInventory(tid, "DnD_TradeEngaged_2", ClearBit(CheckActorInventory(tid, "DnD_TradeEngaged_2"), pnum - 32));
					// fixes disconnect on trade having players name still there bug
					ACS_NamedExecuteAlways("DnD Refresh Request", 0, tid - P_TIDSTART, 1);
					TakeActorInventory(tid, "InTradeView", 1);
				}
			}
			else if(IsSet(CheckActorInventory(tid, "DnD_TradeEngaged_1"), pnum)) {
				SetActorInventory(tid, "DnD_TradeEngaged_1", ClearBit(CheckActorInventory(tid, "DnD_TradeEngaged_1"), pnum));
				// fixes disconnect on trade having players name still there bug
				ACS_NamedExecuteAlways("DnD Refresh Request", 0, tid - P_TIDSTART, 1);
				TakeActorInventory(tid, "InTradeView", 1);
			}
		}
	}
}

void BreakAllTrades() {
	int tid;
	for(int j = 0; j < MAXPLAYERS; ++j) {
		if(PlayerInGame(j)) {
			tid = j + P_TIDSTART;
			SetActorInventory(tid, "DnD_TradeEngaged_1", 0);
			SetActorInventory(tid, "DnD_TradeEngaged_2", 0);
			TakeActorInventory(tid, "DnD_TradeAcceptWindow", 1);
		}
	}
}

int GetBaseCritChance(int pnum) {
	int base = CheckInventory("Perk_Deadliness") * PERK_DEADLINESS_BONUS + GetPlayerAttributeValue(pnum, INV_CRITCHANCE_INCREASE);
	
	// this one is percentage based, like 1.0 is 1%, but crit is 0.01 = 1%, so adjust
	if(HasClassPerk_Fast("Trickster", 1)) {
		int mit_rounded = (GetMitigationChance(pnum) + 0.5) >> 16;
		base += DND_TRICKSTER_CRIT_GAIN_FROM_MIT * mit_rounded;
	}

	return base;
}

int GetPercentCritChanceIncrease(int pnum, int wepid) {
	int val = 	Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITPERCENT][WMOD_ITEMS].val +
				Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITPERCENT][WMOD_WEP].val +
				GetPlayerAttributeValue(pnum, INV_CRITPERCENT_INCREASE) +
				CheckInventory("DnD_SwappedFromMelee") * GetPlayerAttributeValue(pnum, INV_EX_SWAPFROMMELEECRIT);

	return val;
}

int GetCritChance(int pnum, int victim, int wepid, int isLightning = 0) {
	int chance = GetBaseCritChance(pnum);
	int pct_bonus;
	// add other flat crit bonuses here
	if(wepid != -1) {
		chance += Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRIT][WMOD_ITEMS].val + Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRIT][WMOD_WEP].val;

		// precision bonus from tactical helm if any
		if(IsPrecisionWeapon(wepid) && (pct_bonus = GetPlayerAttributeValue(pnum, INV_IMP_PRECISIONCRITBONUS)))
			chance += pct_bonus;
	}

	// more player crit chance bonuses, only on sniper rifle currently
	pct_bonus = CheckInventory("SniperZoomTimer");
	if(pct_bonus)
		chance = FixedMul(chance, 1.0 + pct_bonus * SNIPER_CRIT_BOOST_PER);

	pct_bonus = CheckInventory("DnD_HandgunMoreCritShots");
	if(pct_bonus && IsHandgun(wepid))
		chance = FixedMul(chance, 1.0 + GetPlayerAttributeValue(pnum, INV_IMP_HANDGUNBONUS));

	// monster related bonuses
	//if(victim != -1)
	
	// add percent bonuses here
	pct_bonus = 1.0 + GetPercentCritChanceIncrease(pnum, wepid) + (!!isLightning) * GetPlayerAttributeValue(pnum, INV_EX_MORECRIT_LIGHTNING);
	if(GetPlayerAttributeValue(pnum, INV_EX_DEADEYEBONUS))
		pct_bonus += DND_DEADEYE_BONUSF * (GetActorProperty(0, APROP_ACCURACY) / DND_DEADEYE_PLUSPER);

	if(chance)
		chance = FixedMul(chance, pct_bonus);

	pct_bonus = GetPlayerAttributeValue(pnum, INV_INC_EXCESSCRIT);
	if(pct_bonus && 1.0 - pct_bonus > 0)
		chance = FixedMul(chance, 1.0 - pct_bonus);

	return chance;
}

// for now this isn't very special but in the future it can be
// if we have guaranteed crit from deadliness and we are within the window
bool CheckGuaranteedCritCases() {
	return CheckInventory("DnD_GuaranteeCrit_FromDeadliness") && CheckInventory("DnD_DeadlinessMasteryWindow");
}

bool CheckCritChance(int pnum, int victim, int wepid, bool isLightning, bool noToken = false) {
	// veil disables crits for the cooldown period
	if(CheckInventory("VeilCheck") && CheckInventory("VeilCooldown"))
		return false;

	bool res = false;
	int chance = GetCritChance(pnum, victim, wepid, isLightning);
		
	//printbold(s:"running crit chance: ", f:chance);
	
	res = chance > random(0, 1.0);
	
	// reroll if bad luck and lucky crit is on
	if(!res && CheckUniquePropertyOnPlayer(pnum, PUP_LUCKYCRITS))
		res = chance > random(0, 1.0);
	
	// rolled crit or has source of a guaranteed crit
	if(res || CheckGuaranteedCritCases()) {
		// return true if we got in due to guaranteed case!
		res = true;
		if(!noToken)
			GiveInventory("DnD_CritToken", 1);
		
		TakeInventory("DnD_GuaranteeCrit_FromDeadliness", 1);
		
		// veil check
		if(CheckInventory("VeilCheck") && !CheckInventory("VeilCooldown") && !CheckInventory("VeilMarkTimer")) {
			GiveInventory("VeilMarkTimer", 1);
			ActivatorSound("VeilOfAssassin/Active", 97);
		}

		if(HasClassPerk_Fast("Trickster", 2) && random(0, 1.0) <= DND_TRICKSTER_PHASING_CHANCE && !HasPlayerBuff(pnum, BTI_PHASING))
			ACS_NamedExecuteWithResult("DnD Give Buff", DND_BUFF_PHASING, DEBUFF_F_PLAYERISACTIVATOR);
	}
	
	//printbold(s:"roll crit ", d:res);
	
	return res;
}

void HandleHunterTalisman() {
	if(CheckInventory("HunterTalismanCheck") && !CheckInventory("HunterTalismanCooldown")) {
		ActivatorSound("HunterTalisman/Activate", 1.0);
		GiveInventory("HunterTalismanCooldown", 1);
		GiveInventory("HunterTalismanEffector", 1);
	}
}

// this one doesnt depend on a weapon, its used as it is in the menu etc.
int GetIndependentCritModifier(int pnum) {
	int base = DND_BASE_CRITMODIFIER + DND_SAVAGERY_BONUS * CheckInventory("Perk_Savagery") + GetPlayerAttributeValue(pnum, INV_CRITDAMAGE_INCREASE);
	if(GetPlayerAttributeValue(pnum, INV_EX_DEADEYEBONUS))
		base -= DND_DEADEYE_BONUS * (GetActorProperty(0, APROP_ACCURACY) / DND_DEADEYE_MINUSPER);
	return base;
}

int GetBaseCritModifier(int pnum, int wepid) {
	int base = GetIndependentCritModifier(pnum);
	int wep_bonus = 0;
	
	if(wepid != -1)
		wep_bonus = (Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITDMG][WMOD_ITEMS].val + Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITDMG][WMOD_WEP].val);
	return base + wep_bonus;
}

int GetCritModifier(int pnum, int victim, int wepid, bool forcedReturn = false) {
	// forced return would skip this to get the value for dot multiplier bonus calculation
	if(!forcedReturn && GetPlayerAttributeValue(pnum, INV_INC_CRITFORDOT))
		return 100;

	int base = GetBaseCritModifier(pnum, wepid); // calculates the regular "base" bonuses
	int temp;
	
	// berserker perk50 check
	base += (CheckInventory("Berserker_HitTracker") == DND_BERSERKER_PERK60_MAXSTACKS) * DND_BERSERKER_PERK60_CRITBONUS;
	
	if(CheckInventory("HunterTalismanCheck"))
		base >>= 1;
	if(CheckInventory("VeilMarkTimer")) {
		if(!CheckInventory("VeilNextAttack"))
			GiveInventory("VeilNextAttack", 1);
		else {
			base <<= 1;
			TakeInventory("VeilMarkTimer", 1);
			TakeInventory("VeilNextAttack", 1);
			GiveInventory("VeilCooldown", 1);
			GiveInventory("VeilHealFXSpawner", 1);
			ACS_NamedExecuteAlways("DnD Health Pickup", 0, (DND_VEIL_FACTORUP * GetMissingHealth()) / DND_VEIL_FACTOR);
		}
	}
	
	if(HasMasteredPerk(STAT_SAV) &&!CheckInventory("DnD_SavageryMasteryTimer")) {
		base += DND_SAVAGERY_BONUS;
		GiveInventory("DnD_SavageryMasteryTimer", 1);
	}

	if(victim >= DND_MONSTERTID_BEGIN && MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_OSMIUM])
		base -= DND_OSMIUM_REDUCTION;

	forcedReturn = GetPlayerAttributeValue(pnum, INV_INC_EXCESSCRIT);
	if(forcedReturn && (temp = GetCritChance(pnum, victim, wepid, IsWeaponLightningType(wepid))) > 1.0)
		base = (base * temp) >> 16;

	// damage is returned as it is if its 100, makes no sense for it to be less than 100 (it'd actually lower damage for critting...)
	if(base < 100)
		base = 100;
	
	return base;
}

int GetWeaponModValue(int pnum, int wep, int mod) {
	return Player_Weapon_Infos[pnum][wep].wep_mods[mod][WMOD_ITEMS].val + Player_Weapon_Infos[pnum][wep].wep_mods[mod][WMOD_WEP].val;
}

bool HasWeaponPower(int pnum, int wep, int power) {
	return IsSet(Player_Weapon_Infos[pnum][wep].wep_mods[WEP_MOD_POWERSET1][WMOD_ITEMS].val, power) || IsSet(Player_Weapon_Infos[pnum][wep].wep_mods[WEP_MOD_POWERSET1][WMOD_WEP].val, power);
}

int GetPlayerPercentDamage(int pnum, int wepid, int damage_category, int flags) {
	// stuff that dont depend on a wepid
	int res = MapDamageCategoryToPercentBonus(pnum, damage_category, flags);
	buffData_T module& pbuffs = GetPlayerBuffData(pnum);

	if(GetPlayerAttributeValue(pnum, INV_EX_DEADEYEBONUS)) {
		// add accuracy as % bonus dmg
		res += DND_DEADEYE_BONUS * (GetActorProperty(0, APROP_ACCURACY) / DND_DEADEYE_PLUSPER);
	}

	if(IsPrecisionWeapon(wepid) && GetPlayerAttributeValue(pnum, INV_INC_ACCURACYFORPRECISION))
		res += GetActorProperty(0, APROP_ACCURACY) / DND_INC_ACCURACYFORPRECRATIO;
	
	// buff sourced percent damage

	res += pbuffs.buff_net_values[BUFF_DAMAGEDEALT].additive;

	return res;
}

void RecalculatePlayerLevelInfo() {
	PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] = 0;
	PlayerInformationInLevel[PLAYERLEVELINFO_LEVELATSTART] = 0;
	PlayerInformationInLevel[PLAYERLEVELINFO_MINLEVEL] = INT_MAX;
	PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL] = INT_MIN;
	PlayerInformationInLevel[PLAYERLEVELINFO_COUNTATSTART] = 0;
	int temp = 0;
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART)) {
			temp = CheckActorInventory(i + P_TIDSTART, "Level");
			PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] += temp;
			PlayerInformationInLevel[PLAYERLEVELINFO_LEVELATSTART] += temp;
			if(PlayerInformationInLevel[PLAYERLEVELINFO_MINLEVEL] > temp)
				PlayerInformationInLevel[PLAYERLEVELINFO_MINLEVEL] = temp;
			if(PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL] < temp)
				PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL] = temp;
			++PlayerInformationInLevel[PLAYERLEVELINFO_COUNTATSTART];
		}
	}
}

// this is used to reset things player gathered that are actual items, not variables
void ResetPlayerItems(int pnum) {
	// reset weapons
	// take all weapons
	int i; // start from 1, skip fists
	for(i = 1; i < MAXWEPS; ++i)
		TakeInventory(Weapons_Data[i].name, 1);
	for(i = 1; i <= 9; ++i)
		TakeInventory(StrParam(s:"H_WeaponSlot", d:i), 1);
		
	SetAllAmmoCapacitiesToDefault();

	// take all abilities
	for(i = 0; i < MAXABILITIES; ++i)
		SetInventory(AbilityInfo[i], 0);
		
	// take all researches, artifacts etc.
	for(i = 0; i < RESEARCH_BITSETS; ++i) {
		SetInventory(StrParam(s:"Research_Discovered_", d:i + 1), 0);
		SetInventory(StrParam(s:"Research_Done_", d:i + 1), 0);
	}
	
	SetInventory("BackpackCounter", 0);
	
	for(i = 0; i < MAXARTIFACTS; ++i)
		SetInventory(ArtifactInfo[i][ARTI_NAME], 0);
}				

// resets all info (bearings, loadouts, etc.)
void ResetPlayerInfo(int pnum, bool resetStash = true) {
	// reset player items
	ResetPlayerInventory(pnum);
	ResetPlayerCharmsUsed(pnum);
	ResetTradeViewList(pnum);

	if(resetStash)
		ResetPlayerStash(pnum);
	
	// reset weapon mod variable
	ResetAllWeaponMods(pnum);
	ResetMostRecentOrb(pnum);
	//ResetOrbData(pnum);
	ResetPlayerModList(pnum);
	ResetInvestments(pnum);

	SyncAllClientsideVariables(pnum);
	/*SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_ITEMSUSED);
	SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_STASH);*/
}

void ResetHardcoreStuff(int pnum, bool resetStash = false) {
	ResetPlayerInfo(pnum, resetStash);
	RecalculatePlayerLevelInfo();
	BreakTradesBetween(pnum);
	// may join later, sync everything
	if(PlayerIsSpectator(pnum)) {
		//SyncAllClientsideVariables(pnum);
		SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_ITEMSUSED);
		SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_STASH);
	}
}

int MapDamageCategoryToFlatBonus(int pnum, int talent, int flags) {
	int base = 0;

	if(flags & DND_DAMAGEFLAG_ISRADIUSDMG)
		base += GetPlayerAttributeValue(pnum, INV_FLATRADIUS_DAMAGE);

	switch(talent) {
		case DND_DAMAGECATEGORY_BULLET:
		case DND_DAMAGECATEGORY_MELEE:
			base += GetPlayerAttributeValue(pnum, INV_FLATPHYS_DAMAGE);
		break;
		case DND_DAMAGECATEGORY_OCCULT:
			base += GetPlayerAttributeValue(pnum, INV_FLATMAGIC_DAMAGE);
		break;
		case DND_DAMAGECATEGORY_ENERGY:
			base += GetPlayerAttributeValue(pnum, INV_FLATENERGY_DAMAGE);
		break;

		case DND_DAMAGECATEGORY_FIRE:
			base += GetPlayerAttributeValue(pnum, INV_FLATELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_FLAT_FIREDMG);
		break;
		case DND_DAMAGECATEGORY_ICE:
			base += GetPlayerAttributeValue(pnum, INV_FLATELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_FLAT_ICEDMG);
		break;
		case DND_DAMAGECATEGORY_LIGHTNING:
			base += GetPlayerAttributeValue(pnum, INV_FLATELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_FLAT_LIGHTNINGDMG);
		break;
		case DND_DAMAGECATEGORY_POISON:
			base += GetPlayerAttributeValue(pnum, INV_FLATELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_FLAT_POISONDMG);
		break;
	}
	return base;
}

int MapDamageCategoryToPercentBonus(int pnum, int talent, int flags) {
	int base = 0;

	if(flags & DND_DAMAGEFLAG_ISRADIUSDMG)
		base += GetPlayerAttributeValue(pnum, INV_PERCENTRADIUS_DAMAGE);

	switch(talent) {
		case DND_DAMAGECATEGORY_MELEE:
			base += GetPlayerAttributeValue(pnum, INV_PERCENTPHYS_DAMAGE);
		break;
		case DND_DAMAGECATEGORY_BULLET:
			base += GetPlayerAttributeValue(pnum, INV_PERCENTPHYS_DAMAGE);
		break;
		case DND_DAMAGECATEGORY_OCCULT:
			base += GetPlayerAttributeValue(pnum, INV_PERCENTMAGIC_DAMAGE);
		break;
		case DND_DAMAGECATEGORY_ENERGY:
			base += GetPlayerAttributeValue(pnum, INV_PERCENTENERGY_DAMAGE);
		break;

		case DND_DAMAGECATEGORY_FIRE:
			base += GetPlayerAttributeValue(pnum, INV_PERCENTELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_PERCENTFIRE_DAMAGE);
		break;
		case DND_DAMAGECATEGORY_ICE:
			base += GetPlayerAttributeValue(pnum, INV_PERCENTELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_PERCENTICE_DAMAGE);
		break;
		case DND_DAMAGECATEGORY_LIGHTNING:
			base += GetPlayerAttributeValue(pnum, INV_PERCENTELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_PERCENTLIGHTNING_DAMAGE);
		break;
		case DND_DAMAGECATEGORY_POISON:
			base += GetPlayerAttributeValue(pnum, INV_PERCENTELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_PERCENTPOISON_DAMAGE);
		break;
	}
	return base;
}

int GetFlatHealthDamageFactor(int factor) {
	return GetSpawnHealth() / factor;
}

int GetOverloadTime(int pnum) {
	return (DND_BASE_OVERLOADTIME + ((GetPlayerAttributeValue(pnum, INV_OVERLOAD_DURATION) * TICRATE) >> 16)) / DND_BASE_OVERLOADTICK;
}

int GetMonsterOverloadChance(int m_id, int pnum) {
	return DND_BASE_PLAYEROVERLOADCHANCE;
}

int GetMonsterOverloadTime(int m_id, int pnum) {
	return DND_BASE_OVERLOADTIME_NOADJ;
}

#define DND_BLEED_CHANCE 25 // 25%
#define DND_BLEED_MOVEMENT_MULTIPLIER 3 // x3 damage

int GetMonsterBleedChance(int m_id, int pnum, bool isMelee, bool isHitscan) {
	return DND_BLEED_CHANCE * (1 + isMelee) / (1 + 2 * isHitscan);
}

int GetMonsterBleedDamage(int dmg, int m_id, int pnum) {
	// 10% of damage by default
	return dmg / 10;
}

#define DND_BASE_BLEED_TIME 4
int GetMonsterBleedDuration(int m_id, int pnum) {
	return random(0, 3) + DND_BASE_BLEED_TIME;
}

// returns fixed point range
int GetPlayerMeleeRange(int pnum, int range) {
	return FixedMul(
		range, 
		1.0 +  
		0.01 * (GetPlayerAttributeValue(pnum, INV_MELEERANGE) + GetPerk(STAT_BRUT) * DND_PERK_BRUTALITY_RANGEINC)
	);
}

int GetPlayerDOTMulti(int pnum, int victim = -1, int wepid = -1) {
	int base = GetPlayerAttributeValue(pnum, INV_DOTMULTI) + DND_ACRIMONY_GAIN * GetActorPerk(pnum + P_TIDSTART, STAT_ACRM);

	if(GetPlayerAttributeValue(pnum, INV_INC_CRITFORDOT))
		base += GetCritModifier(pnum, victim, wepid, true) / 2;
	return base;
}

#define DND_BASE_IGNITEDMG 20
int GetFireDOTDamage(int pnum, int bonus = 0, int victim = -1, int wepid = -1) {
	// flat dmg
	int dmg = 	DND_BASE_IGNITEDMG + 
				bonus +
				GetPlayerAttributeValue(pnum, INV_FLAT_FIREDMG) + 
				GetPlayerAttributeValue(pnum, INV_EX_FLATDOT);
	
	// percent increase
	dmg = dmg * (100 + GetPlayerPercentDamage(pnum, -1, DND_DAMAGECATEGORY_FIRE, 0) + GetPlayerAttributeValue(pnum, INV_IGNITEDMG) + GetPlayerAttributeValue(pnum, INV_INCREASEDDOT)) / 100;
	
	// dot multi;
	dmg = dmg * (100 + GetPlayerDOTMulti(pnum, victim, wepid)) / 100;
	
	// hellfire amulet -- moved here for ignite calculation specifically
	if(IsAccessoryEquipped(pnum + P_TIDSTART, DND_ACCESSORY_AMULETHELLFIRE))
		dmg = ApplyDamageFactor_Safe(dmg, DND_AMULETHELL_AMP, DND_AMULETHELL_FACTOR);
	
	return dmg;
}

#define DND_BASE_POISON_STACKS 5

// dont include flat ele dmg and percent damage here, as they are applied to the attacks that inflicted the poison already, no double application!
int GetPoisonDOTDamage(int pnum, int base_poison, int victim = -1, int wepid = -1) {
	int dmg = base_poison;
	if(!dmg)
		dmg = 1;
		
	// flat dmg
	dmg += 	GetPlayerAttributeValue(pnum, INV_FLAT_POISONDMG) + 
			GetPlayerAttributeValue(pnum, INV_EX_FLATDOT);
	
	// percent increase
	dmg = dmg * (100 + GetPlayerPercentDamage(pnum, -1, DND_DAMAGECATEGORY_POISON, 0) + GetPlayerAttributeValue(pnum, INV_POISON_TICDMG) + GetPlayerAttributeValue(pnum, INV_INCREASEDDOT)) / 100;
	
	// dot multi
	dmg = dmg * (100 + GetPlayerDOTMulti(pnum, victim, wepid)) / 100;
	
	return dmg;
}

int GetGenericDoTDamage(int pnum, int base, int victim = -1, int wepid = -1) {
	base = base * (100 + GetPlayerAttributeValue(pnum, INV_INCREASEDDOT)) / 100;
	
	// dot multi
	base = base * (100 + GetPlayerDOTMulti(pnum, victim, wepid)) / 100;

	return base;
}

int GetPlayerPoisonStacks(int pnum) {
	return DND_BASE_POISON_STACKS + GetPlayerAttributeValue(pnum, INV_INC_MAXPOISONSTACK);
}

#define DND_BASEREGENCAP 33
int GetRegenCap(int pnum) {
	int base = (DND_BASEREGENCAP + GetPlayerAttributeValue(pnum, INV_REGENCAP_INCREASE)) * GetSpawnHealth() / 100;
	return base;
}

int GetLifesteal(int pnum) {
	int base = GetPlayerAttributeValue(pnum, INV_LIFESTEAL);

	if(HasClassPerk_Fast("Punisher", 2)) {
		// spree * 2 => merciless
		int bonus = Clamp_Between(CheckInventory("DnD_MultiKillCounter") / DND_SPREE_PER - 1, 0, DND_PUNISHER_MAXLIFESTEALTIMES);
		base += bonus * DND_PUNISHER_LIFESTEALRATE;
	}

	return base;
}

#define DND_BASE_LIFESTEALCAP 33
int GetLifestealCap(int pnum) {
	// avoid recalculating over and over if possible -- changed from the above because if this gets to this point the GetSpawnHealth function has ran once
	//int hp_cap = Max(CheckInventory("PlayerHealthCap"), GetSpawnHealth());
	int hp_cap = CheckActorInventory(pnum + P_TIDSTART, "PlayerHealthCap");
	int bonus = 0;
	int temp;
	if((temp = GetPlayerAttributeExtra(pnum, INV_INC_PASSIVEREGEN)))
		bonus -= temp;

	return Clamp_Between(
		(hp_cap * (DND_BASE_LIFESTEALCAP + GetPlayerAttributeValue(pnum, INV_LIFESTEAL_CAP) + bonus)) / 100, 
		1, 
		hp_cap
	);
}

#define DND_BASE_LIFESTEALRATE 25
int GetLifestealRate(int pnum) {
	// don't return any faster than 1 tic
	int reductions = GetPlayerAttributeExtra(pnum, INV_INC_INSTANTLIFESTEAL);
	return max(1, DND_BASE_LIFESTEALRATE * (100 - GetPlayerAttributeValue(pnum, INV_LIFESTEAL_RATE) + reductions) / 100);
}

#define DND_BASE_LIFERECOVERY 1 // 1% of healthcap
int GetLifestealLifeRecovery(int pnum, int cap) {
	// avoid recalculating over and over if possible
	//cap = cap * DND_BASE_LIFERECOVERY / 100;
	int bonus = 0;
	cap = cap * DND_BASE_LIFERECOVERY / 100;

	if(HasClassPerk_Fast("Punisher", 2)) {
		// spree * 2 => merciless
		bonus = Clamp_Between(CheckInventory("DnD_MultiKillCounter") / DND_SPREE_PER - 1, 0, DND_PUNISHER_MAXLIFESTEALTIMES);
		bonus += bonus * DND_PUNISHER_RECOVERY;
	}

	cap = cap * (100 + bonus + GetPlayerAttributeValue(pnum, INV_LIFESTEAL_RECOVERY)) / 100;
	if(cap <= 0)
		cap = 1;
	
	return cap;
}

// returns true if monster isn't ailment immune, or we can bypass it
bool CheckAilmentImmunity(int pnum, int m_id, int ailment_mod) {
	// is not immune or if it is, we rolled ailment ignore chance
	return !MonsterProperties[m_id].trait_list[ailment_mod] || random(1, 100) < GetPlayerAttributeValue(pnum, INV_CHANCE_AILMENTIGNORE);
}

#define DND_BASE_BLEEDCHANCE_MELEE 20
#define DND_BASE_BLEEDCHANCE_PROJ 5
#define DND_BASE_BLEED_MULT 3
#define DND_BASE_BLEED_MULT_PROJ 1
#define DND_BASE_BLEED_DIV 10
#define DND_BASE_BLEED_TIME_PLAYER 5
#define DND_BLEED_TICRATE 11

int CheckBleedChance(int pnum, int wepid) {
	int base = 0;
	if(IsMeleeWeapon(wepid))
		base = DND_BASE_BLEEDCHANCE_MELEE;
	else
		base = DND_BASE_BLEEDCHANCE_PROJ;

	return random(1, 100) < base + GetPlayerAttributeValue(pnum, INV_CHANCE_BLEED);
}

str GetBleedChanceDisplay(int pnum) {
	int mval = DND_BASE_BLEEDCHANCE_MELEE + GetPlayerAttributeValue(pnum, INV_CHANCE_BLEED);
	int pval = DND_BASE_BLEEDCHANCE_PROJ + GetPlayerAttributeValue(pnum, INV_CHANCE_BLEED);

	return StrParam(s:"\c[Q9]", d:mval, s:"% \c-", l:"DND_AND", s:"\c[Q9] ", d:pval, s:"%\c- ", l:"DND_CHANCEBLEED");
}

int GetBleedDamage(int pnum, int wepid, int dmg, int victim = -1) {
	int mult = DND_BASE_BLEED_MULT_PROJ;
	if(IsMeleeWeapon(wepid))
		mult = DND_BASE_BLEED_MULT;

	dmg += GetPlayerAttributeValue(pnum, INV_EX_FLATDOT);

	dmg = dmg * mult / DND_BASE_BLEED_DIV;
	if(!dmg)
		dmg = 1;
	
	// percent increase
	dmg = (dmg * (100 + GetPlayerAttributeValue(pnum, INV_PERCENTDMG_BLEED) + GetPlayerAttributeValue(pnum, INV_INCREASEDDOT)) / 100);
	
	// dot multi;
	dmg = dmg * (100 + GetPlayerDOTMulti(pnum, victim, wepid)) / 100;
	
	return dmg;
}

int GetPlayerBleedTime(int pnum) {
	return DND_BASE_BLEED_TIME_PLAYER * (100 + GetPlayerAttributeValue(pnum, INV_BLEED_DURATION) + GetPlayerAttributeValue(pnum, INV_EX_DOTDURATION)) / 100;
}

str GetPlayerBleedTimeDisplay(int pnum) {
	return StrParam(f:ftrunc(GetPlayerBleedTime(pnum) * (DND_BLEED_TICRATE << 16) / TICRATE));
}

#define DND_BASE_IGNITECHANCE 15 // 15%
#define DND_BASE_IGNITEPROLIFCHANCE 20 // 20% chance to prolif on monster death
#define DND_BASE_IGNITEPROLIFRANGE 128.0
#define DND_BASE_IGNITEPROLIFCOUNT 5 // max 5 enemies can be proliferated to
#define DND_MAX_IGNITEPROLIFS 32 // max 32 enemies can be proliferated to from one target

int GetIgniteChance(int pnum) {
	return Clamp_Between((DND_BASE_IGNITECHANCE + GetPlayerAttributeValue(pnum, INV_CHANCE_FLATIGNITE)) * (100 + GetPlayerAttributeValue(pnum, INV_IGNITECHANCE)) / 100, 0, 100);
}

int CheckIgniteChance(int pnum) {
	return random(1, 100) <= GetIgniteChance(pnum);
}

int GetIgniteProlifChance(int pnum) {
	return Clamp_Between((DND_BASE_IGNITEPROLIFCHANCE + GetPlayerAttributeValue(pnum, INV_CHANCE_FLATPROLIF)) * (100 + GetPlayerAttributeValue(pnum, INV_IGNITE_PROLIFCHANCE)) / 100, 0, 100);
}

bool CheckIgniteProlifChance(int pnum) {
	return random(1, 100) < GetIgniteProlifChance(pnum);
}

int GetIgniteProlifRange(int pnum) {
	//return DND_BASE_IGNITEPROLIFRANGE * (100 + GetPlayerAttributeValue(pnum, INV_IGNITE_PROLIFRANGE)) / 100;
	return FixedMul(DND_BASE_IGNITEPROLIFRANGE, 1.0 + GetPlayerAttributeValue(pnum, INV_IGNITE_PROLIFRANGE));
}

int GetIgniteProlifCount(int pnum) {
	// clamp between max prolifs
	return Clamp_Between(DND_BASE_IGNITEPROLIFCOUNT + GetPlayerAttributeValue(pnum, INV_IGNITE_PROLIFCOUNT), 0, DND_MAX_IGNITEPROLIFS);
}

#define DND_POISON_CHECKRATE 0.1
#define DND_BASE_POISON_TIC 0.5
#define DND_POISON_TICCHECK 3 // increments ticker every 3 tics
int GetPoisonTicrate(int pnum) {
	int ticrate = (DND_BASE_POISON_TIC * 100) / (100 + GetPlayerAttributeValue(pnum, INV_POISON_TICRATE) + (100 * !!GetPlayerAttributeValue(pnum, INV_ESS_LESHRAC)));
	
	// keep min checkrate, there's no point for it to be lower it'll not go below minimum of 3 tics to trigger
	if(ticrate < DND_POISON_CHECKRATE)
		ticrate = DND_POISON_CHECKRATE;
		
	return ticrate;
}

#define DND_BASE_CHILL_SLOW 0.1 // 10% per stack
#define DND_BASE_CHILL_DAMAGETHRESHOLD 20 // 20% of the monster's health
#define DND_CHILL_HARDTHRESHOLD 2
#define DND_BASE_FREEZECHANCE_PERSTACK 2 // 10% base at max slow stacks
int GetChillEffect(int pnum, int stacks) {
	// call with 1 stack to get "per stack" value
	int chill = stacks * DND_BASE_CHILL_SLOW * (100 + GetPlayerAttributeValue(pnum, INV_SLOWEFFECT)) / 100;
	
	if(chill > 0.99)
		chill = 0.99;
	
	return chill;
}

int GetChillThreshold(int pnum, int stacks) {
	// chill threshold reducing is a good thing for player applying it to monsters
	return Clamp_Between((DND_BASE_CHILL_DAMAGETHRESHOLD * (100 - GetPlayerAttributeValue(pnum, INV_CHILLTHRESHOLD)) / 100) * stacks, DND_CHILL_HARDTHRESHOLD, 100);
}

int GetFreezeChance(int pnum, int stacks) {
	return DND_BASE_FREEZECHANCE_PERSTACK * stacks * (100 + GetPlayerAttributeValue(pnum, INV_FREEZECHANCE)) / 100;
}

int GetMonsterChillThreshold(int m_id, int stacks) {
	// for this, since monster applies to player, player's resistance to this should increase the threshold instead
	// by default this is halved for players as well
	return Clamp_Between(DND_BASE_CHILL_DAMAGETHRESHOLD * stacks, DND_CHILL_HARDTHRESHOLD, 100);
}

int GetMonsterFreezeChance(int m_id, int stacks) {
	return DND_BASE_FREEZECHANCE_PERSTACK * 5 * stacks / 2;
}

int GetCritChance_Display(int pnum) {
	int base = GetCritChance(pnum, -1, -1);
	
	// how it works: let crit chance be "p", you either get a crit, which is probability "p", or you don't and then you get it, which is p * (1 - p)
	// add them both, we get: 2p - p^2, which is our theoretical crit chance if we are lucky
	if(GetPlayerAttributeValue(pnum, INV_EX_ABILITY_LUCKYCRIT))
		base = 2 * base - FixedMul(base, base);
	
	return base;
}

int GetPelletIncrease(int pnum) {
	int base = 1.0 + GetPlayerAttributeValue(pnum, INV_PELLET_INCREASE);
	if(HasClassPerk_Fast("Hobo", 2))
		return CombineFactors(base, DND_HOBO_SHOTGUNPELLETBONUS + (GetLevel() / DND_PERK_REGULARTHRESHOLD) * DND_HOBO_SHOTGUNPELLETBONUS_PERLVL);
	return base;
}

int GetPelletCount(int pnum, int base) {
	// factor base is 1.0
	return ApplyFixedFactorToInt(base, GetPelletIncrease(pnum) - 1.0);
}

int HandleStatBonus(int pnum, int strength, int dexterity, int intellect, bool isMelee) {
	if(HasMasteredPerk(STAT_SHRP))
		dexterity += DND_SHARPSHOOTER_MASTERY_BONUS;

	// 1.0 is 100%, we get stuff like 0.03 here for 3% etc.
	int statOf = 0;
	int hasStrToIntConversion = GetPlayerAttributeValue(pnum, INV_EX_INTBONUSTOMELEE);
	if(isMelee) {
		// steelbark bonus potentially checked here
		strength += GetPlayerAttributeValue(pnum, INV_EX_STREXTRABONUSTOMELEE);

		statOf = hasStrToIntConversion;
		if(statOf) {
			intellect += FixedMul(statOf, strength);
			strength = 0;
		}
	}

	if(hasStrToIntConversion)
		strength = 0;

	statOf = GetStrengthEffect(pnum, strength) + GetDexterityEffect(pnum, dexterity) + GetIntellectEffect(pnum, intellect);

	// brutality is a more multiplier, if there are other "more" things related to melee, keep multiplying here
	if(isMelee)
		statOf = (statOf + (GetPlayerAttributeValue(pnum, INV_MELEEDAMAGE) << 16) / 100) * (100 + GetPerk(STAT_BRUT) * DND_PERK_BRUTALITY_DAMAGEINC) / 100;
	else
		statOf = (statOf * (100 + GetPerk(STAT_SHRP) * DND_PERK_SHARPSHOOTER_INC)) / 100;

	statOf = (statOf * 100) >> 16;

	return statOf;
}

int GetStatAttunementBonus(int pnum, int wepid, bool isMelee) {
	// sharpshooting is a more multiplier
	return HandleStatBonus(pnum, Weapons_Data[wepid].attunement[STAT_STR], Weapons_Data[wepid].attunement[STAT_DEX], Weapons_Data[wepid].attunement[STAT_INT], isMelee);
}

int GetMaxResistCap(int pnum) {
	// cap the cap...
	int cap = GetPlayerAttributeValue(pnum, INV_ADDEDMAXRESIST) + DND_BASE_DAMAGERESISTCAP;
	if(cap > DND_MAX_DAMAGERESISTCAP)
		return DND_MAX_DAMAGERESISTCAP;
	return cap;
}

int ApplyResistCap(int pnum, int res, int cap = 0) {
	// these are in fixed point, so we gotta convert them later
	if(cap)
		return Clamp_Between(res, -500.0, cap);
	return Clamp_Between(res, -500.0, GetMaxResistCap(pnum));
}

int GetExplosiveRepeatChance(int pnum) {
	int tid = pnum + P_TIDSTART;
	int isMarine = HasActorClassPerk_Fast(tid, "Marine", 5);
	int bonus = isMarine * DND_MARINE_EXP_REPEAT_CHANCE;
	if(isMarine) {
		isMarine = CheckActorInventory(tid, "PlayerHealthCap");
		bonus += LinearMap(
			Clamp_Between((((isMarine - GetActorProperty(tid, APROP_HEALTH)) * 100) / isMarine), 50, 100),
			100,
			50,
			0,
			75
		);
	}

	return bonus + GetPlayerAttributeValue(pnum, INV_ESS_KRULL);
}

int GetSelfExplosiveResist(int pnum) {
	int base = 1.0; // 100%
	
	if(HasClassPerk_Fast("Marine", 1))
		base = FixedMul(base, (100 - DND_MARINE_SELFEXPLOSIVEREDUCE) * 1.0 / 100);
	
	// get player selfdmg res
	int temp = (GetPlayerAttributeValue(pnum, INV_SELFDMG_RESIST) << 16);// + GetPlayerAttributeValue(pnum, INV_DMGREDUCE_EXPLOSION);

	// roll damage up
	if(temp) {
		// temp ranges between 0-90.0 potentially, map it back into 1.0 range
		temp = ApplyResistCap(pnum, temp);
		base = FixedMul(base, 1.0 - temp / 100);
	}
	
	// properly include this ability's benefit here, including cyborg check
	if(CheckInventory("Ability_ExplosionMastery")) {
		if(!HasClassPerk_Fast("Cyborg", 1))
			base = FixedMul(base, (100 - DND_EXP_RES_ABILITY_BONUS) * 1.0 / 100);
		else
			base = FixedMul(base, (100 - (DND_EXP_RES_ABILITY_BONUS + DND_EXP_RES_ABILITY_BONUS * DND_CYBORG_CYBER_MULT / DND_CYBORG_CYBER_DIV)) * 1.0 / 100);
	}
		
	// this is 75.0 or maximum 90.0, map it to 0-1.0 and reverse it
	temp = GetPlayerAttributeValue(pnum, INV_ADDEDMAXRESIST) + DND_BASE_DAMAGERESISTCAP;
	if(temp > DND_MAX_DAMAGERESISTCAP)
		temp = DND_MAX_DAMAGERESISTCAP;
	temp = 1.0 - temp / 100;
	
	// absolute minimum of 0.1 multiplier, taking 10% damage
	if(base < temp)
		base = temp;
	
	return base;
}

int GetPlayerSelfDamageReduction_Display(int pnum) {
	int base = GetSelfExplosiveResist(pnum);
	int temp = GetPlayerAttributeValue(pnum, INV_IMP_LESSSELFDAMAGETAKEN);
	if(temp)
		base = base * (100 - temp) / 100;

	if(base < 0.1)
		base = 0.1;

	return base;
}

int GetPlayerElementalAvoidance(int pnum, int ele_mod) {
	int ptid = pnum + P_TIDSTART;
	if(CheckActorInventory(ptid, "Perk_AversionActivated"))
		return 100;

	if((HasActorClassPerk_Fast(ptid, "Wanderer", 3) && CheckActorInventory(ptid, "EShieldAmount")))
		return 100;

	return GetPlayerAttributeValue(pnum, ele_mod) + GetPlayerAttributeValue(pnum, INV_AVOID_ELEAILMENTS) + RISK_AVERSION_VALUE * CheckActorInventory(ptid, "Perk_RiskAversion");
}

int GetPlayerNonElementalAvoidance(int pnum, int ele_mod) {
	int ptid = pnum + P_TIDSTART;
	if(CheckActorInventory(ptid, "Perk_AversionActivated"))
		return 100;
	int base = GetPlayerAttributeValue(pnum, ele_mod) + RISK_AVERSION_VALUE * CheckActorInventory(pnum + P_TIDSTART, "Perk_RiskAversion");
	
	// special conditions like punisher and wanderer
	if
	(
		(ele_mod == INV_AVOID_BLEED && HasActorClassPerk_Fast(ptid, "Punisher", 5) && (CheckActorInventory(ptid, "DnD_MultikillCounter") + 1) / DND_SPREE_PER >= 1) ||
		(HasActorClassPerk_Fast(ptid, "Wanderer", 3) && CheckActorInventory(ptid, "EShieldAmount"))
	)
		base = 100;
	
	return base;
}

void HandleRiskAversion() {
	if(HasMasteredPerk(STAT_RISK) && !CheckInventory("Perk_AversionActivated"))
		GiveInventory("Perk_AversionActivated", 1);
}

int GetPlayerBonusProjectiles(int pnum, int wepid) {
	return Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_EXTRAPROJ][WMOD_WEP].val + Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_EXTRAPROJ][WMOD_ITEMS].val;
}

int GetPlayerAccuracyFactor(int pnum) {
	int base = ACCURACY_FACTOR;
	if(GetPlayerAttributeValue(pnum, INV_INC_ACCURACYREVERSED))
		base *= -1;
	return base;
}

#endif
