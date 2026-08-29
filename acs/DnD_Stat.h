#ifndef DND_EXP_IN
#define DND_EXP_IN

#include "DnD_CommonStat.h"
#include "DnD_Common.h"
#include "Inventory/DnD_Charms.h"
#include "DnD_Artifacts.h"
#include "DnD_Abilities.h"
#include "DnD_Activity.h"
#include "DnD_WeaponDefs.h"
#include "DnD_DamageCache.h"

#define DND_WELLROLL_ODDS 0.1

#define DND_MEDKIT_TO_FLASK_FACTOR 4

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
	DND_ANNOUNCER_TRADEREQUEST,
	DND_ANNOUNCER_DUNGEONPOP
};

// RPG ELEMENTS
//int LevelCurve[MAXLEVELS + 1] = { 150, 351, 623, 895, 1165, 1440, 1710, 1980, 2260, 2520, 2800, 3070, 3350, 3610, 3890, 4130, 4400, 4700, 5000, 5200, 5600, 5700, 6100, 6300, 6700, 6800, 7200, 7400, 7700, 8000, 8800, 9800, 11900, 14400, 17500, 21000, 25600, 30800, 37300, 45000, 54300, 64800, 79000, 96000, 114000, 139000, 166000, 201000, 241000, 290000, 348000, 419000, 504000, 596000, 730000, 870000, 1050000, 1260000, 1500000, 1810000, 2170000, 2600000, 3110000, 3730000, 4470000, 5330000, 6400000, 7700000, 9200000, 11000000, 13100000, 15800000, 18900000, 22500000, 27000000, 32200000, 38600000, 46000000, 54400000, 66000000, 79000000, 94000000, 112000000, 134000000, 160000000, 191000000, 229000000, 272000000, 326000000, 388000000, 463000000, 551000000, 660000000, 790000000, 930000000, 1120000000, 1340000000, 1590000000, 1910000000, 2147483647, 2147483647 };
int LevelCurve[MAXLEVELS + 1] = { 150, 351, 623, 895, 1165, 1440, 1710, 1980, 2260, 2520, 2800, 3070, 3350, 3610, 3890, 4130, 4400, 4700, 5000, 5200, 5600, 5700, 6100, 6300, 6700, 6800, 7200, 7400, 7700, 8000, 8450, 9000, 9700, 10600, 11400, 13300, 15500, 18000, 21200, 24600, 28800, 33600, 39200, 45500, 53000, 62000, 73000, 85000, 99000, 115000, 134000, 156000, 183000, 213000, 247000, 289000, 337000, 392000, 454000, 530000, 620000, 720000, 840000, 980000, 1140000, 1330000, 1540000, 1800000, 2090000, 2440000, 2830000, 3300000, 3840000, 4390000, 5200000, 6100000, 7000000, 8200000, 9500000, 11100000, 12900000, 15000000, 17400000, 20200000, 23600000, 27400000, 31800000, 37000000, 43000000, 50000000, 58000000, 67000000, 79000000, 91000000, 106000000, 124000000, 143000000, 166000000, 194000000, 224000000, 224000000 };

// database related
#define PLAYER_SAVESTATE 0
#define PLAYER_TRANSFERSTATE 1
#define MAX_PLAYER_DBSTATES (PLAYER_TRANSFERSTATE + 1)
global bool 9: PlayerDatabaseState[MAXPLAYERS][MAX_PLAYER_DBSTATES];

int GetExpLimit() {
	return LevelCurve[GetLevel() - 1];
}

int GetExpLimit_Level(int lvl) {
	return LevelCurve[lvl - 1];
}

str GetTalentTag(int id) {
	return StrParam(s:"DND_TALENT", d:id + 1);
}

str StatData[STAT_LVL + 1] = {
	"PSTAT_Strength",
	"PSTAT_Dexterity",
	"PSTAT_Intellect",
	
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
}

void HandleHealthPickup(int amt, int isSpecial, int useTarget, bool noMedkitStore = false, bool notPercentage = false) {
	if(useTarget)
		SetActivatorToTarget(0);
	int curhp = GetActorProperty(0, APROP_HEALTH);
	int healthcap = GetSpawnHealth();
	int pnum = PlayerNumber();

	// dont bother
	if(PlayerModData[pnum].vals[PSTAT_EX_HEALTHATONE])
		return;

	// Endurance / Physician. On the heal itself rather than on the item, so it covers every source
	// of healing rather than only the ones that happen to be pickups.
	int physician = PlayerModData[pnum].vals[PSTAT_HEAL_CLEANSECHANCE];
	if(physician && random(1, 100) <= physician) {
		GiveInventory("RemovePoison", 1);
		GiveInventory("RemoveBleed", 1);
	}

	int bonus = GetHealingBonuses(pnum);
	// holds the old amt
	int toGive = amt, base = amt;
	// the percentage of spawn health is amt to be given
	if(!notPercentage)
		amt = healthcap * amt / 100;

	// consider healing bonuses from quests
	amt = amt * (100 + bonus) / 100;
	
	// health bonus
	if(isspecial == 6) { // +100 above cap
		toGive = healthcap + 100;
	}
	else if(isspecial == 5) {
		// fixed to always go up to x2 health cap
		toGive = healthcap * 2 - curhp;
	}
	else if(isspecial == 3) {
		// map toast heal
		amt = healthcap - curhp;
	    GiveInventory("HealthBonusX", amt);
		GiveInventory("Research_Body_Hp_1_Tracker", amt);
	    return;
	}
	else if(isspecial == 2 || isspecial == 1) {
		// ubersphere / megasphere / soulsphere
		if(amt + curhp > healthcap + toGive)
			toGive = healthcap + toGive - curhp;
		else
			toGive = healthcap + amt - curhp;
	}
	else // for anything else (stims, medkits etc)
	    toGive = healthcap - curhp;
	
	if(toGive > amt)
		toGive = amt;
	
	if(!noMedkitStore && CheckResearchStatus(RES_MEDKITSTORE) == RES_DONE && !isspecial) {
		if(curhp < healthcap) { // if my current curhp is less than max
			GiveInventory("HealthBonusX", toGive);
			GiveInventory("Research_Body_Hp_1_Tracker", toGive);
		}
        if(toGive < amt)
			GiveFlaskChargesPercentage(pnum, DND_MEDKIT_TO_FLASK_FACTOR * Clamp_Between((amt - toGive) * 100 / healthcap, 1, base));
	}
	else {
		GiveInventory("HealthBonusX", toGive);
		GiveInventory("Research_Body_Hp_1_Tracker", toGive);
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
	return CheckActorInventory(tid, "DnD_LevelExp");
}

int GetActorLevelCredits(int tid) {
	return CheckActorInventory(tid, "DnD_LevelCredit");
}

// Was GetPerk, which the attribute paths were calling too because the two shared StatData. Named
// for what it actually reads now that perks no longer live in an inventory item.
int GetStatPoints(int stat_id) {
	return CheckInventory(StatData[stat_id]);
}

str GetStatLabel(int id) {
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
	res = res * 100 / (100 + PlayerModData[pnum].vals[PSTAT_SHIELD_RECHARGEDELAY]);
	if(res < ESHIELD_MIN_TIME)
		res = ESHIELD_MIN_TIME;
	return res;
}

int GetPlayerEnergyShieldRecoveryRate(int pnum, int cap) {
	int bonus = 1;
	if(HasPlayerFlag(pnum, PFLAG_CYBER))
		bonus = 2;
	
	int pct = 100 + PlayerModData[pnum].vals[PSTAT_SHIELD_RECOVERYRATE];
	int res = cap * bonus;

	if(bonus > 1 && HasClassPerk_Fast(DND_PLAYER_CYBORG, 1)) {
		res += res * DND_CYBERNETIC_FACTOR / 100;
	}

	if(pct != 100)
		res = res * pct / 100;
	res /= 333;

	if(HasPlayerFlag(pnum, PFLAG_ESHIELD_NOINTERRUPT))
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
		(HasPlayerFlag(pnum, PFLAG_ESCHARGE_NOINTERRUPT) || HasPlayerFlag(pnum, PFLAG_ESHIELD_NOINTERRUPT) || !CheckInventory("DnD_Hit_CombatTimer")) &&
		!CheckInventory("TaltosUp")
	)
	{
		// finally consider the special condition that this could be from tesseract
		int tmp = GetPlayerEnergyShieldRecoveryRate(pnum, cap);
		int use_hp = PlayerModData[pnum].vals[PSTAT_EX_ESCHARGE_USEHP];
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
	amt = Clamp_Between(CheckInventory(StatData[stat_id]) + amt, 0, DND_STAT_FULLMAX) - CheckInventory(StatData[stat_id]);
	GiveInventory(StatData[stat_id], amt);
	UpdateActivity(PlayerNumber(), DND_ACTIVITY_ATTRIBUTE, amt, stat_id);

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
	UpdateActivity(PlayerNumber(), DND_ACTIVITY_ATTRIBUTE, -amt, stat_id);

	// visual updates
	if(stat_id == STAT_STR) {
		UpdatePlayerKnockbackResist();
		SetActorProperty(0, APROP_SPAWNHEALTH, GetSpawnHealth());
	}
}

void SpawnPlayerDrop(int pnum, str actor, int zoffset, int thrust, int setspecial, int setspecial2, bool noRandomVelXY = false) {
	SpawnDrop(actor, zoffset, thrust, setspecial, setspecial2, noRandomVelXY, pnum + P_TIDSTART);
}

void SpawnPlayerDropTargeted(int pnum, str actor, int zoffset, int thrust, int setspecial, int setspecial2, bool noRandomVelXY = false) {
	SpawnDrop(actor, zoffset, thrust, setspecial, setspecial2, noRandomVelXY);
}

void SpawnPlayerDropAtActor(int pnum, int dest, str actor, int zoffset, int thrust, int setspecial, int setspecial2, bool noRandomVelXY = false) {
	SpawnDropAtActor(dest, actor, zoffset, thrust, setspecial, setspecial2, noRandomVelXY);
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

	GiveInventory("DnD_LevelExp", amt);
	ACS_NamedExecuteAlways("DnD Player Levelup Check", 0, ActivatorTID());
	CalculateExpRatio();
}

void GiveActorExp(int tid, int amt) {
	int pnum = tid - P_TIDSTART;
	AddPlayerExp(pnum, amt);
	GiveActorInventory(tid, "SpreeXP", amt);
	GiveActorInventory(tid, "DnD_LevelExp", amt);
	ACS_NamedExecuteAlways("DnD Player Levelup Check", 0, tid);
	CalculatePlayerExpRatio(tid);
}

int GetPlayerWisdomBonus(int pnum, int tid) {
	// the item modifier is fixed point and is a more multiplier
	// was + BASE_WISDOM_GAIN per Wisdom point
	int base = 100 + HasDungeonUpside(DUN_UPSIDE_EXPANDCREDIT);
	
	// halved
	if(IsAccessoryEquipped(tid, DND_ACCESSORY_TALISMANGREED))
		base >>= 1;
		
	// 50% increase
	if(IsAccessoryEquipped(tid, DND_ACCESSORY_TALISMANWISDOM))
		base = base * 3 / 2;
	
	return ApplyFixedFactorToInt(base, PlayerModData[pnum].vals[PSTAT_EXPGAIN_INCREASE]);
}

int GetPlayerGreedBonus(int pnum, int tid) {
	// the item modifier is fixed point and is a more multiplier
	// was + BASE_GREED_GAIN per Greed point
	int base = 100 + HasDungeonUpside(DUN_UPSIDE_EXPANDCREDIT);
	
	// halved
	if(IsAccessoryEquipped(tid, DND_ACCESSORY_TALISMANWISDOM))
		base >>= 1;
		
	// 50% increase
	if(IsAccessoryEquipped(tid, DND_ACCESSORY_TALISMANGREED))
		base = base * 3 / 2;
	
	return ApplyFixedFactorToInt(base, PlayerModData[pnum].vals[PSTAT_CREDITGAIN_INCREASE]);
}

int RewardActorExp(int tid, int amt) {
	int tmp = GetActorLevel(tid);
	// for now, return 0 if player is max level, maybe later we can do something with exp beyond this
	if(tmp == MAXLEVELS)
		return 0;

	if(tmp >= DND_EXP_ADJUST_LEVEL)
		amt = amt * ((GlobalData.DnD_Constants[DND_CONSTANT_EXPCURVE][tmp - DND_EXP_ADJUST_LEVEL] * 100) >> 16) / 100;

	amt = amt * GetPlayerWisdomBonus(tid - P_TIDSTART, tid) / 100;

	if(PlayerActivities[tid - P_TIDSTART].loot_penalty)
		amt /= MAPLOOTPENALTY_FACTOR;

	GiveActorExp(tid, amt);
	return amt;
}

int RewardActorCredit(int tid, int amt) {
	amt = amt * GetPlayerGreedBonus(tid - P_TIDSTART, tid) / 100;

	if(PlayerActivities[tid - P_TIDSTART].loot_penalty)
		amt /= MAPLOOTPENALTY_FACTOR;

	GiveActorCredit(tid, amt);
	return amt;
}

void GiveCredit(int amt) {
	int pnum = PlayerNumber();
	GiveInventory("Credit", amt);
	GiveInventory("DnD_LevelCredit", amt);
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
	amt = amt * (100 + HasDungeonUpside(DUN_UPSIDE_BUDGET)) / 100;
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
	amt = amt * (100 + HasDungeonUpside(DUN_UPSIDE_BUDGET)) / 100;
	GiveActorInventory(tid, "Budget", amt * Clamp_Between(GetCVar("dnd_budget_scale"), 1, BUDGET_SCALE_MAX));
	ACS_NamedExecuteAlways("DnD Refresh Request", 0, tid - P_TIDSTART, 1);
	UpdateActivity(tid - P_TIDSTART, DND_ACTIVITY_BUDGET, amt, 0);
}

void GiveActorCredit(int tid, int amt) {
	int pnum = tid - P_TIDSTART;
	GiveActorInventory(tid, "Credit", amt);
	GiveActorInventory(tid, "DnD_LevelCredit", amt);
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
	
	if(HasClassPerk_Fast(DND_PLAYER_CYBORG, 1)) {
		res *= DND_CYBORG_CYBER_MULT;
		res /= DND_CYBORG_CYBER_DIV;
	}
	
	return res;
}

int CanPickHealthItem(int type) {
	int res = 0;
    int research_cond = CheckResearchStatus(RES_MEDKITSTORE) == RES_DONE && HasNonfullFlasks(PlayerNumber());
	if(type == 1) // the artifact kit
		res = GetActorProperty(0, APROP_HEALTH) < GetSpawnHealth();
	else // normal health pickup
		res = GetActorProperty(0, APROP_HEALTH) < GetSpawnHealth() || research_cond;
		
	return res;
}

// used for deciding armor pickup values
int GetPlayerArmor(int pnum) {
	int amt = PlayerModData[pnum].vals[PSTAT_ARMOR_FLAT];
	amt += pbuffs[pnum].buff_net_values[BUFF_ARMORFLAT].additive;
	amt += (amt * CheckInventory("CelestialCheck") * CELESTIAL_BOOST) / 100;
	amt += (amt * GetResearchArmorBonuses()) / 100;
	amt += (amt * PlayerModData[pnum].vals[PSTAT_ARMOR_PCT]) / 100;
	return amt;
}

int Calculate_Stats() {
	int res = 0;
	for(int i = DND_ATTRIB_BEGIN; i <= DND_ATTRIB_END; ++i)
		res += CheckInventory(StatData[i]);
	return res;
}

// this is used in drop rates, weapons proc chances etc.
int GetPlayerLuck(int pnum) {
	//return outcome_val * GetActorPerk(pnum + P_TIDSTART, X) + PlayerModData[pnum].vals[PSTAT_LUCK_INCREASE];
	return PlayerModData[pnum].vals[PSTAT_LUCK_INCREASE];
}

bool RunLuckBasedChance(int pnum, int base) {
	int r = random(0, 1.0);
	return r <= FixedMul(base, 1.0 + GetPlayerLuck(pnum));
}

int GetPlayerDropQuantity(int pnum) {
	int base = 1.0;
	base = FixedMul(
		base, 
		1.0 + GetPlayerLuck(pnum) + (HasDungeonUpside(DUN_UPSIDE_QUANT) << 16) / 100 +
		(DungeonInformation.level != -1) * (DungeonInformation.level * DND_DUNGEON_QUANTPERLVL) +
		MapData[DND_MAPDATA_LOOTBONUS]
	);

	return base;
}

// this is the generic drop chance factor
int GetDropChance(int pnum) {
	int base = 1.0; // base val
	// additive bonuses first
	base += PlayerModData[pnum].vals[PSTAT_DROPCHANCE_INCREASE];
		
	// more chance to find loot
	base = FixedMul(base, 1.0 + GetPlayerLuck(pnum));
	if(GetCVar("dnd_mode") == DND_MODE_HARDCORE)
		base = FixedMul(base, 1.0 + DND_HARDCORE_DROPRATEBONUS);

	base += Clamp_Between(DND_STARTER_LUCK - (GetLevel() - 1) * DND_LUCK_DIMINISHPERLVL, 0, 1.0) + MapData[DND_MAPDATA_LOOTBONUS];

	// chances reduced to 25%
	if(PlayerActivities[pnum].loot_penalty)
		base /= MAPLOOTPENALTY_FACTOR;

	return base;
}

int GetPlayerItemRarity(int pnum) {
	int base = 1.0;

	base += PlayerModData[pnum].vals[PSTAT_ITEMRARITY] + (HasDungeonUpside(DUN_UPSIDE_RARITY) << 16) / 100;

	// more chance for rarity -- only at quarter
	base = FixedMul(base, 1.0 + GetPlayerLuck(pnum) / 4);
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
	
	auto a_info = GetAmmoInfo(DND_AMMOSLOT_SOULS, AMMO_SOUL);
	if(IsAccessoryEquipped(this, DND_ACCESSORY_LICHARM)) {
		GiveInventory("LichCheck", 1);
		SetAmmoCapacity("Souls", a_info.initial_capacity * DND_LICH_SOULFACTOR);
	}
	else {
		TakeInventory("LichCheck", 1);
		int tmp = GetAmmoCapacity("Souls");
		if(tmp > a_info.initial_capacity) {
			SetAmmoCapacity("Souls", tmp / 2);
			if(CheckInventory("Souls") > tmp / 2)
				SetInventory("Souls", tmp / 2);
		}
		else
			SetAmmoCapacity("Souls", a_info.initial_capacity);
	}
	
	// sigil order: 1 = fire, 2 = cold, 3 = poison, 4 = lightning (the BTI_ELEMENTPOWER_* order)
	if(IsAccessoryEquipped(this, DND_ACCESSORY_SIGILELEMENTS))
		SetInventory("SigilCheck", 1);
	else {
		SetInventory("SigilCheck", 0);

		// unequipping the sigil drops any element power it granted
		for(int ele = 0; ele < DND_ELEMENTAL_DMGBUFF_COUNT; ++ele)
			RemoveBuffWithTableIndex(this - P_TIDSTART, BTI_ELEMENTPOWER_FIRE + ele);
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
	return PlayerHasNoElementalDamageBuff(PlayerNumber());
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
// Only ever touches OTHER players -- you are never engaged with yourself, so this is purely
// "clean up everyone else's reference to the player who left".
void BreakTradesBetween(int pnum) {
	int tid;
	// check all trades of all players, clean players who have one going with this guy
	for(int j = 0; j < MAXPLAYERS; ++j) {
		if(!PlayerInGame(j))
			continue;

		tid = j + P_TIDSTART;
		if(pnum > 31) {
			if(!IsSet(CheckActorInventory(tid, "DnD_TradeEngaged_2"), pnum - 32))
				continue;
			SetActorInventory(tid, "DnD_TradeEngaged_2", ClearBit(CheckActorInventory(tid, "DnD_TradeEngaged_2"), pnum - 32));
		}
		else {
			if(!IsSet(CheckActorInventory(tid, "DnD_TradeEngaged_1"), pnum))
				continue;
			SetActorInventory(tid, "DnD_TradeEngaged_1", ClearBit(CheckActorInventory(tid, "DnD_TradeEngaged_1"), pnum));
		}

		// Whatever this player had staked is still sitting in their trade view, and nothing saves
		// TRADEVIEW to the database -- put it back before tearing the trade down or it is gone.
		ReturnTradeItems(j);

		// State 2, not 1. 1 only refreshes the pane; 2 also raises DnD_CleanTradeviewRequest, which
		// is what actually deletes the trade graphics. With 1 the trade stayed drawn on top of the
		// normal menu while every click went to the menu underneath.
		// fixes disconnect on trade having players name still there bug
		ACS_NamedExecuteAlways("DnD Refresh Request", 0, j, 2);
		TakeActorInventory(tid, "InTradeView", 1);
		// the rest of what CancelTrade clears, minus the engaged bitfields -- those are cleared
		// per bit above so a pending request from a third player survives
		TakeActorInventory(tid, "DnD_TradeSpaceFit", 1);
		TakeActorInventory(tid, "DnD_TradeAcceptWindow", 1);
		TakeActorInventory(tid, "DnD_Trade_Confirmed", 1);
		SetActorInventory(tid, "DnD_SelectedInventoryBox", 0);
	}
}

void BreakAllTrades() {
	int tid;
	for(int j = 0; j < MAXPLAYERS; ++j) {
		if(PlayerInGame(j)) {
			tid = j + P_TIDSTART;
			// same reasoning as BreakTradesBetween: TRADEVIEW is never saved, so a staked item is
			// lost unless it goes home first. This runs from UNLOADING, ahead of SaveAllPlayerData.
			ReturnTradeItems(j);
			SetActorInventory(tid, "DnD_TradeEngaged_1", 0);
			SetActorInventory(tid, "DnD_TradeEngaged_2", 0);
			TakeActorInventory(tid, "DnD_TradeAcceptWindow", 1);
			// InTradeView is actor inventory and rides along to the next map, so without this a
			// player caught mid trade at map end reopens the menu there still in the trade view
			TakeActorInventory(tid, "InTradeView", 1);
			TakeActorInventory(tid, "DnD_TradeSpaceFit", 1);
			TakeActorInventory(tid, "DnD_Trade_Confirmed", 1);
		}
	}
}

// "Elites and uniques" as Bane of Legends words it. Elite is a spawn flag; unique bosses are their
// own id range and are never flagged elite, so both have to be asked separately.
bool IsEliteOrUniqueTarget(int victim) {
	if(victim < DND_MONSTERTID_BEGIN)
		return false;

	int m_id = victim - DND_MONSTERTID_BEGIN;
	return (MonsterProperties[m_id].flags & DND_MONFLAG_ISELITE) || isUniqueBossMonster(m_id);
}

bool IsBossTarget(int victim) {
	if(victim < DND_MONSTERTID_BEGIN)
		return false;
	return IsMonsterIdBoss(MonsterProperties[victim - DND_MONSTERTID_BEGIN].id);
}

// The General Notes definitions: Full Life is at maximum health, Low Life is <= 50%.
//
// Both read the stored maxhp rather than APROP_SPAWNHEALTH, because monster health is scaled at
// spawn and the spawn property no longer describes the monster you are actually shooting. A zero
// maxhp means the monster was never registered, and answering false there is what keeps a
// mid-registration hit from counting as full life for free.
bool IsTargetAtFullLife(int victim) {
	if(victim < DND_MONSTERTID_BEGIN)
		return false;

	int maxhp = MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp;
	return maxhp > 0 && GetActorProperty(victim, APROP_HEALTH) >= maxhp;
}

bool IsTargetOnLowLife(int victim) {
	if(victim < DND_MONSTERTID_BEGIN)
		return false;

	int maxhp = MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp;
	return maxhp > 0 && GetActorProperty(victim, APROP_HEALTH) * 2 <= maxhp;
}

int GetBaseCritChance(int pnum) {
	// was + PERK_DEADLINESS_BONUS per Deadliness point; Assassination's Deadliness replaces it
	int base = PlayerModData[pnum].vals[PSTAT_CRITCHANCE_INCREASE];
	
	// this one is percentage based, like 1.0 is 1%, but crit is 0.01 = 1%, so adjust
	if(HasClassPerk_Fast(DND_PLAYER_TRICKSTER, 1)) {
		int mit_rounded = (GetMitigationChance(pnum) + 0.5) >> 16;
		base += DND_TRICKSTER_CRIT_GAIN_FROM_MIT * mit_rounded;
	}

	return base;
}

int GetPercentCritChanceIncrease(int pnum, int wepid) {
	int val = 	Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITPERCENT][WMOD_ITEMS].val +
				Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITPERCENT][WMOD_WEP].val +
				PlayerModData[pnum].vals[PSTAT_CRITPERCENT_INCREASE] +
				CheckInventory("DnD_SwappedFromMelee") * PlayerModData[pnum].vals[PSTAT_EX_SWAPFROMMELEECRIT] +
				(IsMeleeWeapon(wepid) && !IsOnLowStamina()) * PlayerModData[pnum].vals[PSTAT_MELEECRIT_NOTONLOWSTAMINA];

	val += pbuffs[pnum].buff_net_values[BUFF_CRITPERCENT].additive + pbuffs[pnum].buff_net_values[BUFF_POWERCHARGE].additive;

	return val;
}

int GetCritChance(int pnum, int victim, int wepid, int isLightning = 0) {
	int chance = GetBaseCritChance(pnum);
	int pct_bonus;
	// add other flat crit bonuses here
	if(wepid != -1) {
		chance += Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRIT][WMOD_ITEMS].val + Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRIT][WMOD_WEP].val;

		// precision bonus from tactical helm if any
		if(IsPrecisionWeapon(wepid) && (pct_bonus = PlayerModData[pnum].vals[PSTAT_IMP_PRECISIONCRITBONUS]))
			chance += pct_bonus;
	}

	// Assassination / Bane of Legends, crit chance half. Fixed point like everything else here.
	if((pct_bonus = PlayerModData[pnum].vals[PSTAT_CRITCHANCE_VS_ELITE]) && IsEliteOrUniqueTarget(victim))
		chance += pct_bonus;

	// Assassination / Backstab. "Back is turned" is the target's own facing, not the attacker's --
	// MaxAngleDiff would answer whether the PLAYER is looking at the monster, which is a different
	// question and true almost always.
	if((pct_bonus = PlayerModData[pnum].vals[PSTAT_CRITCHANCE_FROMBEHIND]) && victim >= DND_MONSTERTID_BEGIN) {
		int to_player = VectorAngle(GetActorX(pnum + P_TIDSTART) - GetActorX(victim),
									GetActorY(pnum + P_TIDSTART) - GetActorY(victim));
		int off = abs(to_player - GetActorAngle(victim));
		if(off > 0.5)
			off = 1.0 - off;

		if(off > (DND_BACKSTAB_ANGLE / 360.0))
			chance += pct_bonus;
	}

	// Perception / Lucky Bullet. Flat, not a multiplier: the notes give it as "+2.5% chance", and
	// every other flat crit source in this function is added the same way.
	if(CheckActorInventory(pnum + P_TIDSTART, "DnD_LuckyBullet"))
		chance += PlayerModData[pnum].vals[PSTAT_LASTROUND_CRIT];

	// Assassination / Preparation. A MULTIPLIER on the whole chance, like the two below -- "50%
	// increased crit chance" on a fixed point probability would be absurd read as +50 points.
	if((pct_bonus = PlayerModData[pnum].vals[PSTAT_CRIT_DROUGHTBONUS]) &&
		!CheckActorInventory(pnum + P_TIDSTART, "DnD_CritDrought"))
		chance = FixedMul(chance, 1.0 + ((pct_bonus << 16) / 100));

	// Assassination / Master of Shadows. Player state rather than victim state, but multiplicative
	// for the same reason and so it belongs in the same block. The light read is deliberately last of
	// the three conditions to be evaluated, since it is the only one that costs an engine call.
	if((pct_bonus = PlayerModData[pnum].vals[PSTAT_CRITCHANCE_INDARK]) &&
		GetActorLightLevel(pnum + P_TIDSTART) <= DND_DARKAREA_LIGHTLEVEL)
		chance = FixedMul(chance, 1.0 + ((pct_bonus << 16) / 100));

	// Assassination / Opening Salvo and Mercy Kill. Multiplicative, and placed after every flat
	// source above so they scale the whole crit chance rather than a partial sum. The two conditions
	// are mutually exclusive unless a monster has 0 or 1 max health, so no ordering question arises.
	if((pct_bonus = PlayerModData[pnum].vals[PSTAT_CRITCHANCE_VS_FULLLIFE]) && IsTargetAtFullLife(victim))
		chance = FixedMul(chance, 1.0 + ((pct_bonus << 16) / 100));

	if((pct_bonus = PlayerModData[pnum].vals[PSTAT_CRITCHANCE_VS_LOWLIFE]) && IsTargetOnLowLife(victim))
		chance = FixedMul(chance, 1.0 + ((pct_bonus << 16) / 100));

	// more player crit chance bonuses, only on sniper rifle currently
	pct_bonus = CheckInventory("SniperZoomTimer");
	if(pct_bonus)
		chance = FixedMul(chance, 1.0 + pct_bonus * SNIPER_CRIT_BOOST_PER);

	pct_bonus = CheckInventory("DnD_HandgunMoreCritShots");
	if(pct_bonus && IsHandgun(wepid))
		chance = FixedMul(chance, 1.0 + PlayerModData[pnum].vals[PSTAT_IMP_HANDGUNBONUS]);

	// monster related bonuses
	//if(victim != -1)
	
	// add percent bonuses here
	pct_bonus = 1.0 + GetPercentCritChanceIncrease(pnum, wepid) + (!!isLightning) * PlayerModData[pnum].vals[PSTAT_EX_MORECRIT_LIGHTNING];
	if(PlayerModData[pnum].vals[PSTAT_EX_DEADEYEBONUS])
		pct_bonus += DND_DEADEYE_BONUSF * (GetActorProperty(0, APROP_ACCURACY) / DND_DEADEYE_PLUSPER);

	if(chance)
		chance = FixedMul(chance, pct_bonus);

	pct_bonus = PlayerModData[pnum].vals[PSTAT_INC_EXCESSCRIT];
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

		if(HasClassPerk_Fast(DND_PLAYER_TRICKSTER, 2) && random(0, 1.0) <= DND_TRICKSTER_PHASING_CHANCE && !HasPlayerBuff(pnum, BTI_PHASING))
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
// applyExcess: the in-game path (GetCritModifier) applies excess-crit itself, using
// the weapon- and victim-aware GetCritChance, so it must suppress this display-chance
// copy. Applying both multiplied the modifier TWICE, and the doubled product wrapped
// for base > 1311 at a 5.0 crit chance -- the "if(base < 100) base = 100" floor then
// caught the negative, so crits landed for exactly base damage.
int GetIndependentCritModifier(int pnum, bool applyExcess = true) {
	// was + DND_SAVAGERY_BONUS per Savagery point
	int base = DND_BASE_CRITMODIFIER + PlayerModData[pnum].vals[PSTAT_CRITDAMAGE_INCREASE];
	if(PlayerModData[pnum].vals[PSTAT_EX_DEADEYEBONUS])
		base -= DND_DEADEYE_BONUS * (GetActorProperty(0, APROP_ACCURACY) / DND_DEADEYE_MINUSPER);

	int temp;
	if(applyExcess && PlayerModData[pnum].vals[PSTAT_INC_EXCESSCRIT] && (temp = GetCritChance_Display(pnum)) > 1.0)
		base = FixedMul(base, temp);

	return base;
}

int GetBaseCritModifier(int pnum, int wepid, bool applyExcess = true) {
	int base = GetIndependentCritModifier(pnum, applyExcess);
	int wep_bonus = 0;
	
	if(wepid != -1)
		wep_bonus = (Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITDMG][WMOD_ITEMS].val + Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITDMG][WMOD_WEP].val);
	return base + wep_bonus;
}

int GetCritModifier(int pnum, int victim, int wepid, bool forcedReturn = false) {
	// forced return would skip this to get the value for dot multiplier bonus calculation
	if(!forcedReturn && PlayerModData[pnum].vals[PSTAT_INC_CRITFORDOT])
		return 100;

	int base = GetBaseCritModifier(pnum, wepid, false); // excess-crit is applied below with the real crit chance
	int temp;
	
	// berserker perk50 check
	base += (CheckInventory("Berserker_HitTracker") == DND_BERSERKER_PERK60_MAXSTACKS) * DND_BERSERKER_PERK60_CRITBONUS;

	// Perception / Lucky Bullet, multiplier half.
	if(CheckActorInventory(pnum + P_TIDSTART, "DnD_LuckyBullet"))
		base += PlayerModData[pnum].vals[PSTAT_LASTROUND_CRITDMG];

	// Assassination / Steady Shot. The stationary counter is kept by "DnD Fall Impact", which already
	// runs per tic per player -- a second loop for the same question would be waste.
	if((temp = PlayerModData[pnum].vals[PSTAT_CRITDAMAGE_PERSTILLSEC]))
		base += temp * Min(CheckActorInventory(pnum + P_TIDSTART, "DnD_StillSeconds"), DND_STEADYSHOT_MAXSECONDS);

	// Assassination / Big Game Hunter and Bane of Legends. Both are integer percent added to the
	// multiplier, and both read the victim the same way the OSMIUM check below does. A boss that is
	// also a unique satisfies both, which is intended -- they are separate perks with separate costs.
	if((temp = PlayerModData[pnum].vals[PSTAT_CRITDAMAGE_VS_BOSS]) && IsBossTarget(victim))
		base += temp;

	if((temp = PlayerModData[pnum].vals[PSTAT_CRITDAMAGE_VS_ELITE]) && IsEliteOrUniqueTarget(victim))
		base += temp;
	
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
	
	if(victim >= DND_MONSTERTID_BEGIN && HasMonsterTrait(victim - DND_MONSTERTID_BEGIN, DND_OSMIUM))
		base -= DND_OSMIUM_REDUCTION;

	forcedReturn = PlayerModData[pnum].vals[PSTAT_INC_EXCESSCRIT];
	if(forcedReturn && (temp = GetCritChance(pnum, victim, wepid, IsWeaponLightningType(wepid))) > 1.0)
		base = FixedMul(base, temp);

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

// Accuracy-derived percent damage.
//
// Deliberately NOT part of GetPlayerPercentDamage, which is called from inside the
// cached block: APROP_ACCURACY is a STORED property that CalculatePlayerAccuracy
// folds a TEMPORARY term into -- the Hobo shotgun-frenzy bonus, granted on a shotgun
// kill and dropped when it expires, both mid-combat. Cached, that froze; read here at
// request time it is always current.
//
// The wepid >= 0 guard is new: callers pass -1 for spells and DoTs, and
// IsPrecisionWeapon(-1) indexes Weapons_Data out of bounds.
int GetPlayerAccuracyDamageBonus(int pnum, int wepid) {
	int res = 0;
	int acc = GetActorProperty(pnum + P_TIDSTART, APROP_ACCURACY);

	if(PlayerModData[pnum].vals[PSTAT_EX_DEADEYEBONUS])
		res += DND_DEADEYE_BONUS * (acc / DND_DEADEYE_PLUSPER);

	if(wepid >= 0 && IsPrecisionWeapon(wepid) && PlayerModData[pnum].vals[PSTAT_INC_ACCURACYFORPRECISION])
		res += acc / DND_INC_ACCURACYFORPRECRATIO;

	return res;
}

int GetPlayerPercentDamage(int pnum, int wepid, int damage_category, int flags) {
	// stuff that dont depend on a wepid
	int res = MapDamageCategoryToPercentBonus(pnum, damage_category, flags);

	// DO NOT add buff-sourced percent damage here.
	//
	// This runs INSIDE the cached block of ScaleCachedDamage, so anything added here is
	// frozen into the per-weapon factor on the first shot and stays until the weapon is
	// re-raised -- present forever or absent forever, regardless of whether the buff is
	// actually up. That cuts both ways: a mid-fight Golgoth Weaken would keep its -75%
	// on that weapon long after expiring.
	//
	// Buff terms belong at the call sites, via GetPlayerBuffIncreasedDamage(pnum),
	// where they are read fresh on every shot.

	return res;
}

void RecalculatePlayerLevelInfo() {
	InformationInLevel[LEVELINFO_PLAYERLEVEL] = 0;
	InformationInLevel[LEVELINFO_PLAYERLEVELATSTART] = 0;
	InformationInLevel[LEVELINFO_MINPLAYERLEVEL] = bcs::INT_MAX;
	InformationInLevel[LEVELINFO_MAXPLAYERLEVEL] = bcs::INT_MIN;
	InformationInLevel[LEVELINFO_PLAYERCOUNTATSTART] = 0;
	int temp = 0;
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART)) {
			temp = CheckActorInventory(i + P_TIDSTART, "Level");
			InformationInLevel[LEVELINFO_PLAYERLEVEL] += temp;
			InformationInLevel[LEVELINFO_PLAYERLEVELATSTART] += temp;
			if(InformationInLevel[LEVELINFO_MINPLAYERLEVEL] > temp)
				InformationInLevel[LEVELINFO_MINPLAYERLEVEL] = temp;
			if(InformationInLevel[LEVELINFO_MAXPLAYERLEVEL] < temp)
				InformationInLevel[LEVELINFO_MAXPLAYERLEVEL] = temp;
			++InformationInLevel[LEVELINFO_PLAYERCOUNTATSTART];
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
		SetInventory(GetAbilityInfo(i), 0);
		
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
	ResetPlayerItemsUsed(pnum);
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

// Replaces a 30 line switch of "case X: base += the mod for X". PSTAT_FLATDMG_BASE is a run keyed BY
// damage category, so the category IS the storage index and the dispatch becomes arithmetic. The
// switch existed only to bridge two enums that no longer need bridging.
int MapDamageCategoryToFlatBonus(int pnum, int talent, int flags) {
	int base = 0;

	if(flags & DND_DAMAGEFLAG_ISRADIUSDMG)
		base += PlayerModData[pnum].vals[PSTAT_FLATDMG_RADIUS];

	// What the old switch did with a category it had no case for: the radius term and nothing else.
	// GetDamageCategory cannot return out of range, but the stat page walks categories itself and
	// HandleNonWeaponDamageScale passes one it resolved elsewhere, so this is checked rather than
	// assumed -- an out of range talent would index vals[] outside the run.
	if(talent < 0 || talent >= MAX_DAMAGE_CATEGORIES)
		return base;

	// Physical lives in the BULLET slot and answers for MELEE too, exactly as the added damage run
	// does. The old switch spelled this out by listing both cases against one mod.
	if(talent == DND_DAMAGECATEGORY_MELEE)
		talent = DND_DAMAGECATEGORY_BULLET;

	base += PlayerModData[pnum].vals[PSTAT_FLATDMG_BASE + talent];

	// One mod feeding all four elements, so it cannot sit in a run keyed by a single category.
	// DND_ELECATEGORY_BEGIN..END is exactly FIRE, ICE, POISON, LIGHTNING -- the four the switch listed.
	if(talent >= DND_ELECATEGORY_BEGIN && talent <= DND_ELECATEGORY_END)
		base += PlayerModData[pnum].vals[PSTAT_FLATDMG_ELEM];

	return base;
}

// Flat damage a source adds to EVERY attack regardless of the weapon's own type -- "+10 cold damage
// to attacks" on a physical shotgun. Distinct from INV_FLAT_ICEDMG and friends above, which only
// apply when the attack is already that type; these create a component that was not there.
//
// These used to need a pair of mapper functions to walk between the attribute enum and the damage
// category enum, written as switches precisely because the two orders could drift apart. They are
// gone: PSTAT_ADDEDFLAT_BASE is a run keyed BY damage category, so the mapping is the address and
// there is nothing left that can fall out of step. The id side of it lives in MapAttributeToPStat
// and is consulted at equip time only.

// Flat added damage this player deals as "category", before any effectiveness or scaling.
int GetPlayerAddedFlatDamage(int pnum, int category) {
	// Physical added damage occupies the BULLET slot and answers for MELEE too. The two old mappers
	// disagreed here on purpose and this preserves it: an added component is never the weapon's own
	// swing so it is DEALT as bullet, but a melee weapon still gets to add it.
	if(category == DND_DAMAGECATEGORY_MELEE)
		category = DND_DAMAGECATEGORY_BULLET;

	return PlayerModData[pnum].vals[PSTAT_ADDEDFLAT_BASE + category];
}

// One bit per damage category the player currently adds damage in. Zero for anyone without a single
// added-damage source, which is the whole point: the mixed-damage paths test this first and cost
// nothing when it is clear.
int GetPlayerAddedDamageMask(int pnum) {
	int mask = 0;
	for(int c = 0; c < MAX_DAMAGE_CATEGORIES; ++c) {
		// MELEE and SOUL have no mod that can fill them, so they stay zero and never set a bit -- the
		// same result the old attribute walk produced, without the walk.
		if(PlayerModData[pnum].vals[PSTAT_ADDEDFLAT_BASE + c])
			mask |= 1 << c;
	}
	return mask;
}

// Same collapse as MapDamageCategoryToFlatBonus above, same reasoning -- see the comments there.
int MapDamageCategoryToPercentBonus(int pnum, int talent, int flags) {
	int base = 0;

	if(flags & DND_DAMAGEFLAG_ISRADIUSDMG)
		base += PlayerModData[pnum].vals[PSTAT_PCTDMG_RADIUS];

	if(talent < 0 || talent >= MAX_DAMAGE_CATEGORIES)
		return base;

	if(talent == DND_DAMAGECATEGORY_MELEE)
		talent = DND_DAMAGECATEGORY_BULLET;

	base += PlayerModData[pnum].vals[PSTAT_PCTDMG_BASE + talent];

	if(talent >= DND_ELECATEGORY_BEGIN && talent <= DND_ELECATEGORY_END)
		base += PlayerModData[pnum].vals[PSTAT_PCTDMG_ELEM];

	return base;
}

int GetFlatHealthDamageFactor(int factor) {
	return GetSpawnHealth() / factor;
}

int GetOverloadTime(int pnum) {
	return (DND_BASE_OVERLOADTIME + ((PlayerModData[pnum].vals[PSTAT_OVERLOAD_DURATION] * TICRATE) >> 16)) / DND_BASE_OVERLOADTICK;
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
		0.01 * PlayerModData[pnum].vals[PSTAT_MELEERANGE]
	);
}

int GetPlayerDOTMulti(int pnum, int victim = -1, int wepid = -1) {
	int base = PlayerModData[pnum].vals[PSTAT_DOTMULTI];
	int temp = 0;
	if((temp = PlayerModData[pnum].vals[PSTAT_INC_CRITFORDOT]))
		base += GetCritModifier(pnum, victim, wepid, true) * (100 + temp) / 100;
	return base;
}

#define DND_BASE_IGNITEDMG 20
int GetFireDOTDamage(int pnum, int bonus = 0, int victim = -1, int wepid = -1) {
	// flat dmg
	int dmg = 	DND_BASE_IGNITEDMG + 
				bonus +
				PlayerModData[pnum].vals[PSTAT_FLATDMG_BASE + DND_DAMAGECATEGORY_FIRE] + 
				PlayerModData[pnum].vals[PSTAT_DOT_FLAT];
	
	// percent increase
	dmg = dmg * (100 + GetPlayerPercentDamage(pnum, -1, DND_DAMAGECATEGORY_FIRE, 0) + GetPlayerBuffIncreasedDamage(pnum) + GetPlayerAccuracyDamageBonus(pnum, -1) + PlayerModData[pnum].vals[PSTAT_IGN_DMG] + PlayerModData[pnum].vals[PSTAT_DOT_INCREASED]) / 100;
	
	// dot multi;
	dmg = dmg * (100 + GetPlayerDOTMulti(pnum, victim, wepid)) / 100;
	
	// hellfire amulet -- moved here for ignite calculation specifically
	if(IsAccessoryEquipped(pnum + P_TIDSTART, DND_ACCESSORY_AMULETHELLFIRE))
		dmg = MulPercent_Exact(dmg, DND_AMULETHELL_AMP, DND_AMULETHELL_FACTOR);
	
	return dmg;
}

#define DND_BASE_POISON_STACKS 5

// dont include flat ele dmg and percent damage here, as they are applied to the attacks that inflicted the poison already, no double application!
int GetPoisonDOTDamage(int pnum, int base_poison, int victim = -1, int wepid = -1) {
	int dmg = base_poison;
	if(!dmg)
		dmg = 1;
		
	// flat dmg
	dmg += 	PlayerModData[pnum].vals[PSTAT_FLATDMG_BASE + DND_DAMAGECATEGORY_POISON] + 
			PlayerModData[pnum].vals[PSTAT_DOT_FLAT];
	
	// percent increase
	dmg = dmg * (100 + GetPlayerPercentDamage(pnum, -1, DND_DAMAGECATEGORY_POISON, 0) + GetPlayerBuffIncreasedDamage(pnum) + GetPlayerAccuracyDamageBonus(pnum, -1) + PlayerModData[pnum].vals[PSTAT_DOT_INCREASED]) / 100;
	
	// dot multi
	dmg = dmg * (100 + GetPlayerDOTMulti(pnum, victim, wepid)) / 100;
	
	return dmg;
}

// this doesn't consider INV_EX_FLATDOT here because the weapon that calls this does -- if wepid is left at -1, it should then consider it!
int GetGenericDoTDamage(int pnum, int base, int victim = -1, int wepid = -1) {
	if(wepid == -1)
		base += PlayerModData[pnum].vals[PSTAT_DOT_FLAT];
	
	base = base * (100 + PlayerModData[pnum].vals[PSTAT_DOT_INCREASED]) / 100;
	
	// dot multi
	base = base * (100 + GetPlayerDOTMulti(pnum, victim, wepid)) / 100;

	return base;
}

int GetPlayerPoisonStacks(int pnum) {
	return DND_BASE_POISON_STACKS + PlayerModData[pnum].vals[PSTAT_INC_MAXPOISONSTACK];
}

#define DND_BASEREGENCAP 33
int GetRegenCap(int pnum) {
	int base = (DND_BASEREGENCAP + PlayerModData[pnum].vals[PSTAT_REGENCAP_INCREASE]) * GetSpawnHealth() / 100;
	return base;
}

int GetLifesteal(int pnum) {
	int base = PlayerModData[pnum].vals[PSTAT_LIFESTEAL];

	if(HasClassPerk_Fast(DND_PLAYER_PUNISHER, 2)) {
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
	if((temp = ReadPlayerModExtra(pnum, INV_INC_PASSIVEREGEN)))
		bonus -= temp;

	return Clamp_Between(
		(hp_cap * (DND_BASE_LIFESTEALCAP + PlayerModData[pnum].vals[PSTAT_LIFESTEAL_CAP] + bonus)) / 100, 
		1, 
		hp_cap
	);
}

#define DND_BASE_LIFESTEALRATE 25
int GetLifestealRate(int pnum) {
	// don't return any faster than 1 tic
	int reductions = ReadPlayerModExtra(pnum, INV_INC_INSTANTLIFESTEAL);
	return max(1, DND_BASE_LIFESTEALRATE * (100 - PlayerModData[pnum].vals[PSTAT_LIFESTEAL_RATE] + reductions) / 100);
}

#define DND_BASE_LIFERECOVERY 1 // 1% of healthcap
int GetLifestealLifeRecovery(int pnum, int cap) {
	// avoid recalculating over and over if possible
	//cap = cap * DND_BASE_LIFERECOVERY / 100;
	int bonus = 0;
	cap = cap * DND_BASE_LIFERECOVERY / 100;

	if(HasClassPerk_Fast(DND_PLAYER_PUNISHER, 2)) {
		// spree * 2 => merciless
		bonus = Clamp_Between(CheckInventory("DnD_MultiKillCounter") / DND_SPREE_PER - 1, 0, DND_PUNISHER_MAXLIFESTEALTIMES);
		bonus += bonus * DND_PUNISHER_RECOVERY;
	}

	cap = cap * (100 + bonus + PlayerModData[pnum].vals[PSTAT_LIFESTEAL_RECOVERY]) / 100;
	if(cap <= 0)
		cap = 1;
	
	return cap;
}

// returns true if monster isn't ailment immune, or we can bypass it
bool CheckAilmentImmunity(int pnum, int m_id, int ailment_mod) {
	// is not immune or if it is, we rolled ailment ignore chance
	return !HasMonsterTrait(m_id, ailment_mod) || random(1, 100) < PlayerModData[pnum].vals[PSTAT_AILMENT_IGNORECHANCE];
}

#define DND_BASE_BLEEDCHANCE_MELEE 20
#define DND_BASE_BLEEDCHANCE_PROJ 5
#define DND_BASE_BLEED_MULT 3
#define DND_BASE_BLEED_MULT_PROJ 1
#define DND_BASE_BLEED_DIV 10
#define DND_BASE_BLEED_TIME_PLAYER 5
#define DND_BLEED_TICRATE 11

int CheckBleedChance(int pnum, int wepid, int victim) {
	int base = 0;
	if(IsMeleeWeapon(wepid))
		base = DND_BASE_BLEEDCHANCE_MELEE;
	else
		base = DND_BASE_BLEEDCHANCE_PROJ;

	base += PlayerModData[pnum].vals[PSTAT_BLEED_CHANCE];

	// Martialist / Deep Cuts. The gate belongs here rather than in the perk because this is the only
	// place that knows which weapon swung -- and the function already had wepid for the melee split.
	if(IsSlashingWeapon(wepid))
		base += PlayerModData[pnum].vals[PSTAT_BLEED_CHANCE_SLASHING];

	base += CheckActorInventory(victim, "DnD_OpenWounds") * DND_OPENWOUNDS_BLEEDCHANCE;
	return random(1, 100) <= base;
}

// Deliberately does NOT fold in PSTAT_BLEED_CHANCE_SLASHING. This is a melee/projectile pair with
// no weapon in hand to test, so a slashing-only bonus has no honest place in it -- showing it would
// promise the number on a pistol. It wants its own line when the perk UI exists.
str GetBleedChanceDisplay(int pnum) {
	int mval = DND_BASE_BLEEDCHANCE_MELEE + PlayerModData[pnum].vals[PSTAT_BLEED_CHANCE];
	int pval = DND_BASE_BLEEDCHANCE_PROJ + PlayerModData[pnum].vals[PSTAT_BLEED_CHANCE];

	return StrParam(s:"\c[Q9]", d:mval, s:"% \c-", l:"DND_AND", s:"\c[Q9] ", d:pval, s:"%\c- ", l:"DND_CHANCEBLEED");
}

int GetBleedDamage(int pnum, int wepid, int dmg, int victim = -1) {
	int mult = DND_BASE_BLEED_MULT_PROJ;
	if(IsMeleeWeapon(wepid))
		mult = DND_BASE_BLEED_MULT;

	dmg += PlayerModData[pnum].vals[PSTAT_DOT_FLAT];

	dmg = dmg * mult / DND_BASE_BLEED_DIV;
	if(!dmg)
		dmg = 1;
	
	// percent increase
	dmg = (dmg * (100 + PlayerModData[pnum].vals[PSTAT_BLEED_DMG_PCT] + PlayerModData[pnum].vals[PSTAT_DOT_INCREASED]) / 100);
	
	// dot multi;
	dmg = dmg * (100 + GetPlayerDOTMulti(pnum, victim, wepid) + CheckActorInventory(victim, "DnD_OpenWounds") * DND_OPENWOUNDS_BLEEDMULTIBONUS) / 100;

	return dmg;
}

int GetPlayerBleedTime(int pnum) {
	return DND_BASE_BLEED_TIME_PLAYER * (100 + PlayerModData[pnum].vals[PSTAT_BLEED_DURATION] + PlayerModData[pnum].vals[PSTAT_DOT_DURATION]) / 100;
}

str GetPlayerBleedTimeDisplay(int pnum) {
	return StrParam(f:ftrunc(GetPlayerBleedTime(pnum) * (DND_BLEED_TICRATE << 16) / TICRATE));
}


#define DND_BASE_IGNITECHANCE 15 // 15%
// Base DoT durations live here rather than in DnD_Damage.h with the rest of the ailment constants:
// the Get*Duration accessors below are read by the stat menu as well as by the damage code, and this
// header is parsed FIRST, so a define left behind in DnD_Damage.h is not visible to them.
#define DND_BASE_IGNITETIMER 20 // 20 loops x DND_IGNITE_TICKRATE = 4 seconds
// One loop of "DnD Monster Ignite" -- the Delay in that script MUST use this, not a bare 7, or the
// duration shown on the stat page silently stops matching the burn it describes.
#define DND_IGNITE_TICKRATE 7
#define DND_ADDEDIGNITE_CHANCE 25 // flat chance a DND_DAMAGEFLAG_ADDEDIGNITE weapon adds on top of the base
#define DND_BASE_IGNITEPROLIFCHANCE 20 // 20% chance to prolif on monster death
#define DND_BASE_IGNITEPROLIFRANGE 128.0
#define DND_BASE_IGNITEPROLIFCOUNT 5 // max 5 enemies can be proliferated to
#define DND_MAX_IGNITEPROLIFS 32 // max 32 enemies can be proliferated to from one target

// flat_bonus is a weapon side contribution, currently only DND_ADDEDIGNITE_CHANCE. It goes in the same
// bucket as INV_CHANCE_FLATIGNITE so the player's % ignite chance scales it too -- a weapon that helps
// you ignite should get better as you invest in igniting, not sit outside your build.
// The menu readout calls this with no bonus on purpose: it is a player stat, not a per weapon one.
int GetIgniteChance(int pnum, int flat_bonus = 0) {
	return Clamp_Between((DND_BASE_IGNITECHANCE + flat_bonus + PlayerModData[pnum].vals[PSTAT_IGN_CHANCE_FLAT]) * (100 + PlayerModData[pnum].vals[PSTAT_IGN_CHANCE_PCT]) / 100, 0, 100);
}

int CheckIgniteChance(int pnum, int flat_bonus = 0) {
	return random(1, 100) <= GetIgniteChance(pnum, flat_bonus);
}

int GetIgniteProlifChance(int pnum) {
	return Clamp_Between((DND_BASE_IGNITEPROLIFCHANCE + PlayerModData[pnum].vals[PSTAT_IGN_PROLIF_CHANCE_FLAT]) * (100 + PlayerModData[pnum].vals[PSTAT_IGN_PROLIF_CHANCE_PCT]) / 100, 0, 100);
}

bool CheckIgniteProlifChance(int pnum) {
	return random(1, 100) < GetIgniteProlifChance(pnum);
}

int GetIgniteProlifRange(int pnum) {
	//return DND_BASE_IGNITEPROLIFRANGE * (100 + PlayerModData[pnum].vals[PSTAT_IGN_PROLIF_RANGE]) / 100;
	return FixedMul(DND_BASE_IGNITEPROLIFRANGE, 1.0 + PlayerModData[pnum].vals[PSTAT_IGN_PROLIF_RANGE]);
}

int GetIgniteProlifCount(int pnum) {
	// clamp between max prolifs
	return Clamp_Between(DND_BASE_IGNITEPROLIFCOUNT + PlayerModData[pnum].vals[PSTAT_IGN_PROLIF_COUNT], 0, DND_MAX_IGNITEPROLIFS);
}

int GetIgniteDuration(int pnum) {
	return DND_BASE_IGNITETIMER * (100 + PlayerModData[pnum].vals[PSTAT_IGN_DURATION] + PlayerModData[pnum].vals[PSTAT_DOT_DURATION]) / 100;
}

#define DND_POISON_CHECKRATE 0.1
#define DND_BASE_POISON_TIMER 3.0
#define DND_BASE_POISON_TIC 0.5
#define DND_POISON_TICCHECK 3 // increments ticker every 3 tics
int GetPoisonTicrate(int pnum) {
	int ticrate = (DND_BASE_POISON_TIC * 100) / (100 + PlayerModData[pnum].vals[PSTAT_POIS_TICRATE] + (100 * !!PlayerModData[pnum].vals[PSTAT_ESS_LESHRAC]));
	
	// keep min checkrate, there's no point for it to be lower it'll not go below minimum of 3 tics to trigger
	if(ticrate < DND_POISON_CHECKRATE)
		ticrate = DND_POISON_CHECKRATE;
		
	return ticrate;
}

int GetPoisonDuration(int pnum) {
	return DND_BASE_POISON_TIMER * (100 + PlayerModData[pnum].vals[PSTAT_POIS_DURATION] + PlayerModData[pnum].vals[PSTAT_DOT_DURATION]) / 100;
}

// Ignite counts DOWN in loops of DND_IGNITE_TICKRATE tics, so its duration is a loop count and has to
// be converted the same way the bleed one is.
str GetPlayerIgniteTimeDisplay(int pnum) {
	return StrParam(f:ftrunc(GetIgniteDuration(pnum) * (DND_IGNITE_TICKRATE << 16) / TICRATE));
}

// Poison is the odd one out: DND_BASE_POISON_TIMER is already 3.0 in FIXED POINT SECONDS and the
// burn loop counts up to it in DND_POISON_CHECKRATE steps, so there is no conversion to do.
str GetPlayerPoisonTimeDisplay(int pnum) {
	return StrParam(f:ftrunc(GetPoisonDuration(pnum)));
}

int GetGenericDoTDuration(int pnum, int base) {
	return base * (100 + PlayerModData[pnum].vals[PSTAT_DOT_DURATION]) / 100;
}

#define DND_BASE_CHILL_SLOW 0.1 // 10% per stack
#define DND_BASE_CHILL_DAMAGETHRESHOLD 20 // 20% of the monster's health
#define DND_BASE_CHILL_DAMAGETHRESHOLD_PLAYER (DND_BASE_CHILL_DAMAGETHRESHOLD / 2) // 10% of the player's health
#define DND_CHILL_HARDTHRESHOLD 2
#define DND_BASE_FREEZECHANCE_PERSTACK 2 // 10% base at max slow stacks
int GetChillEffect(int pnum, int stacks) {
	// call with 1 stack to get "per stack" value
	int chill = stacks * DND_BASE_CHILL_SLOW * (100 + PlayerModData[pnum].vals[PSTAT_SLOWEFFECT]) / 100;
	
	if(chill > 0.99)
		chill = 0.99;
	
	return chill;
}

int GetChillThreshold(int pnum, int stacks) {
	// chill threshold reducing is a good thing for player applying it to monsters
	return Clamp_Between((DND_BASE_CHILL_DAMAGETHRESHOLD * (100 - PlayerModData[pnum].vals[PSTAT_CHILL_THRESHOLD]) / 100) * stacks, DND_CHILL_HARDTHRESHOLD, 100);
}

int GetFreezeChance(int pnum, int stacks) {
	return DND_BASE_FREEZECHANCE_PERSTACK * stacks * (100 + PlayerModData[pnum].vals[PSTAT_FREEZE_CHANCE]) / 100;
}

int GetMonsterChillThreshold(int m_id) {
	// for this, since monster applies to player, player's resistance to this should increase the threshold instead
	// returns a random percentage between the minimum of 2 and the player's value of (currently) 5% -- if player has higher chill threshold this will go up
	return random(DND_CHILL_HARDTHRESHOLD, DND_BASE_CHILL_DAMAGETHRESHOLD_PLAYER);
}

int GetMonsterFreezeChance(int m_id, int stacks) {
	return DND_BASE_FREEZECHANCE_PERSTACK * 5 * stacks / 2;
}

int GetCritChance_Display(int pnum) {
	int base = GetCritChance(pnum, -1, -1);
	
	// how it works: let crit chance be "p", you either get a crit, which is probability "p", or you don't and then you get it, which is p * (1 - p)
	// add them both, we get: 2p - p^2, which is our theoretical crit chance if we are lucky
	if(HasPlayerFlag(pnum, PFLAG_LUCKYCRIT) && base < 1.0)
		base = 2 * base - FixedMul(base, base);

	return base;
}

int GetPelletIncrease(int pnum) {
	int base = 1.0 + PlayerModData[pnum].vals[PSTAT_PELLET_INCREASE];
	if(HasClassPerk_Fast(DND_PLAYER_HOBO, 2))
		return CombineFactors(base, DND_HOBO_SHOTGUNPELLETBONUS + (GetLevel() / DND_PERK_REGULARTHRESHOLD) * DND_HOBO_SHOTGUNPELLETBONUS_PERLVL);
	return base;
}

// wepid defaults to -1 for the callers that genuinely have no weapon in hand. Perception's Blastier
// Shots is shotgun-only and there is no other way to know: GetPelletIncrease is a global factor and
// the base count says nothing about what fired it.
int GetPelletCount(int pnum, int base, int wepid = -1) {
	// factor base is 1.0
	int count = ApplyFixedFactorToInt(base, GetPelletIncrease(pnum) - 1.0);

	// Added AFTER the factor, unlike Pumped's shell: a pellet bonus multiplied by a pellet factor
	// compounds into whole extra volleys, and "an additional pellet" means one.
	if(wepid != -1 && IsBoomstick(wepid))
		count += PlayerModData[pnum].vals[PSTAT_PELLET_FLAT_SHOTGUN];

	return count;
}

int HandleStatBonus(int pnum, int strength, int dexterity, int intellect, bool isMelee) {
	// 1.0 is 100%, we get stuff like 0.03 here for 3% etc.
	int statOf = 0;
	int hasStrToIntConversion = PlayerModData[pnum].vals[PSTAT_EX_INTBONUSTOMELEE];
	if(isMelee) {
		// steelbark bonus potentially checked here
		strength += PlayerModData[pnum].vals[PSTAT_EX_STREXTRABONUSTOMELEE];

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
		statOf = statOf + (PlayerModData[pnum].vals[PSTAT_MELEEDAMAGE] << 16) / 100;

	statOf = (statOf * 100) >> 16;

	return statOf;
}

int GetStatAttunementBonus(int pnum, int wepid, bool isMelee) {
	// sharpshooting is a more multiplier
	return HandleStatBonus(pnum, Weapons_Data[wepid].attunement[STAT_STR], Weapons_Data[wepid].attunement[STAT_DEX], Weapons_Data[wepid].attunement[STAT_INT], isMelee);
}

int GetMaxResistCap(int pnum) {
	// cap the cap...
	int cap = PlayerModData[pnum].vals[PSTAT_MAXRESIST_ADDED] + DND_BASE_DAMAGERESISTCAP;
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
	int isMarine = HasActorClassPerk_Fast(tid, DND_PLAYER_MARINE, 5);
	int bonus = isMarine * DND_MARINE_EXP_REPEAT_CHANCE;
	if(isMarine) {
		isMarine = CheckActorInventory(tid, "PlayerHealthCap");
		bonus += Clamp_Between((((isMarine - GetActorProperty(tid, APROP_HEALTH)) * 150) / isMarine), 0, 75); // * 150 so 50% missing health can reach cap
	}

	return bonus + PlayerModData[pnum].vals[PSTAT_ESS_KRULL];
}

int GetSelfExplosiveResist(int pnum) {
	int base = 1.0; // 100%
	
	if(HasClassPerk_Fast(DND_PLAYER_MARINE, 1))
		base = FixedMul(base, (100 - DND_MARINE_SELFEXPLOSIVEREDUCE) * 1.0 / 100);
	
	// get player selfdmg res
	int temp = (PlayerModData[pnum].vals[PSTAT_SELFDMG_RESIST] << 16);// + ReadPlayerModValue(pnum, INV_DMGREDUCE_EXPLOSION);

	// roll damage up
	if(temp) {
		// temp ranges between 0-90.0 potentially, map it back into 1.0 range
		temp = ApplyResistCap(pnum, temp);
		base = FixedMul(base, 1.0 - temp / 100);
	}
	
	// properly include this ability's benefit here, including cyborg check
	if(CheckInventory("Ability_ExplosionMastery")) {
		if(!HasClassPerk_Fast(DND_PLAYER_CYBORG, 1))
			base = FixedMul(base, (100 - DND_EXP_RES_ABILITY_BONUS) * 1.0 / 100);
		else
			base = FixedMul(base, (100 - (DND_EXP_RES_ABILITY_BONUS + DND_EXP_RES_ABILITY_BONUS * DND_CYBORG_CYBER_MULT / DND_CYBORG_CYBER_DIV)) * 1.0 / 100);
	}
		
	// this is 75.0 or maximum 90.0, map it to 0-1.0 and reverse it
	temp = PlayerModData[pnum].vals[PSTAT_MAXRESIST_ADDED] + DND_BASE_DAMAGERESISTCAP;
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
	int temp = PlayerModData[pnum].vals[PSTAT_IMP_LESSSELFDAMAGETAKEN];
	if(temp)
		base = base * (100 - temp) / 100;

	if(base < 0.1)
		base = 0.1;

	return base;
}

// Takes a DND_PAVOID_* INDEX, not an attribute id -- renamed from GetPlayerElementalAvoidance for the
// same reason ApplyPlayerDamageResist was, since the signature is otherwise unchanged.
int GetPlayerElementalAvoidChance(int pnum, int avoid_id) {
	int ptid = pnum + P_TIDSTART;
	if(CheckActorInventory(ptid, "Perk_AversionActivated"))
		return 100;

	if((HasActorClassPerk_Fast(ptid, DND_PLAYER_WANDERER, 3) && CheckActorInventory(ptid, "EShieldAmount")))
		return 100;

	// was + RISK_AVERSION_VALUE per RiskAversion point
	return PlayerModData[pnum].vals[PSTAT_AVOID_BASE + avoid_id] + PlayerModData[pnum].vals[PSTAT_AVOID_ELEALL];
}

// Split out of GetPlayerNonElementalAvoidance, which was two functions wearing one name: bleed was
// the only caller passing an avoid mod, and its Punisher clause was gated on the parameter being
// INV_AVOID_BLEED. Everything else calling it passed a LESSxTAKEN implicit and took the branch that
// is false by construction. Splitting is what lets the avoid family be indexed while the stat page
// keeps its id-keyed reuse below.
int GetPlayerBleedAvoidChance(int pnum) {
	int ptid = pnum + P_TIDSTART;
	if(CheckActorInventory(ptid, "Perk_AversionActivated"))
		return 100;

	int base = PlayerModData[pnum].vals[PSTAT_AVOID_BASE + DND_PAVOID_BLEED];

	// special conditions like punisher and wanderer
	if
	(
		(HasActorClassPerk_Fast(ptid, DND_PLAYER_PUNISHER, 5) && (CheckActorInventory(ptid, "DnD_MultikillCounter") + 1) / DND_SPREE_PER >= 1) ||
		(HasActorClassPerk_Fast(ptid, DND_PLAYER_WANDERER, 3) && CheckActorInventory(ptid, "EShieldAmount"))
	)
		base = 100;

	return base;
}

// Still ID-KEYED, and deliberately so. Its only callers are the stat page lines for
// INV_IMP_LESSFIRETAKEN and friends, which are damage reductions rather than avoidance rolls but are
// worded and clamped the same way on the page. Kept exactly as it behaved for them; the bleed branch
// it used to carry is gone with the caller that needed it.
int GetPlayerNonElementalAvoidance(int pnum, int attr) {
	int ptid = pnum + P_TIDSTART;
	if(CheckActorInventory(ptid, "Perk_AversionActivated"))
		return 100;
	int base = ReadPlayerModValue(pnum, attr);

	if(HasActorClassPerk_Fast(ptid, DND_PLAYER_WANDERER, 3) && CheckActorInventory(ptid, "EShieldAmount"))
		base = 100;

	return base;
}

// The Perk_AversionActivated powerup and its three readers in the defence path are deliberately
// still here -- they are a working mechanic that simply has no source now that RiskAversion is gone.
// Re-granting it is a job for the new tree, not a rebuild.
void HandleRiskAversion() {
}

int GetPlayerBonusProjectiles(int pnum, int wepid) {
	return Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_EXTRAPROJ][WMOD_WEP].val + Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_EXTRAPROJ][WMOD_ITEMS].val;
}

int GetPlayerAccuracyFactor(int pnum) {
	int base = ACCURACY_FACTOR;
	if(HasPlayerFlag(pnum, PFLAG_ACCURACY_REVERSED))
		base *= -1;
	return base;
}

int GetPlayerStaminaRecoveryRate(int pnum) {
	int base = DND_BASE_STAMINA_RECOVERYRATE * (100 - PlayerModData[pnum].vals[PSTAT_INC_STAMINARECOVERYRATE]) / 100;
	if(base <= 0)
		base = 3; // minimum value is 3 for tic delay here
	return base;
}

void TakeStamina(int amt) {
	TakeInventory("DnD_Stamina", amt);
	GiveInventory("DnD_StaminaLocked", 1);

	if(!CheckInventory("DnD_Stamina"))
		GiveInventory("DnD_StaminaDepleted", 1);
}

void GiveStamina(int amt) {
	int cap = GetAmmoCapacity("DnD_Stamina");
	if(CheckInventory("DnD_Stamina") + amt >= cap)
		SetInventory("DnD_Stamina", cap);
	else
		GiveInventory("DnD_Stamina", amt);
}

int GetPlayerStaminaGain(int pnum) {
	return DND_BASE_STAMINA_GAIN * (100 + PlayerModData[pnum].vals[PSTAT_INC_STAMINAGAINED]) / 100;
}

bool IsOnLowStamina() {
	return CheckInventory("DnD_Stamina") < GetAmmoCapacity("DnD_Stamina") / 2;
}

bool CanGainStaminaOnKill(int pnum) {
	int temp = PlayerModData[pnum].vals[PSTAT_IMP_STAMINAONKILL];
	temp += HasClassPerk_Fast(DND_PLAYER_BERSERKER, 2) * DND_BERSERKER_PERK40_STAMINACHANCE;
	return temp >= random(1, 100);
}

int GetStaminaGainOnKill(int pnum) {
	int temp = ReadPlayerModExtra(pnum, INV_IMP_STAMINAONKILL);
	temp += HasClassPerk_Fast(DND_PLAYER_BERSERKER, 2) * DND_BERSERKER_PERK40_STAMINAGAIN;
	return temp;
}

#include "DnD_Player.h"

#endif
