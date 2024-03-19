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

#define DND_BASE_CRITMODIFIER 200
#define DND_SAVAGERY_MASTERYBONUS 100
#define DND_HARDCORE_DROPRATEBONUS 0.15

#define ESHIELD_RECHARGEDELAY_BASE (10 * TICRATE) // base time is 10 seconds
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

#define DND_SYNTHMASK_EFFECT 4

// we multiply by 100 in calc
#define DND_DAMAGERESIST_FACTOR 10000
#define DND_BASE_DAMAGERESISTCAP 75.0
#define DND_MAX_DAMAGERESISTCAP 90.0

#define DND_TOXICOLOGY_REDUCE 33

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
	DND_ANNOUNCER_TRADEREQUEST
};

// RPG ELEMENTS
int LevelCurve[MAXLEVELS + 1] = { 100, 264, 480, 696, 912, 1128, 1340, 1560, 1780, 1990, 2210, 2420, 2640, 2860, 4200, 5800, 6100, 6600, 6900, 8350, 9900, 10300, 10900, 11400, 11800, 12300, 12800, 13300, 13800, 14300, 19000, 22700, 27000, 31900, 37900, 45000, 53300, 63000, 75000, 89000, 105000, 125000, 149000, 175000, 209000, 247000, 294000, 348000, 412000, 490000, 579000, 680000, 820000, 970000, 1150000, 1360000, 1610000, 1920000, 2270000, 2690000, 3200000, 3780000, 4490000, 5260000, 6300000, 7500000, 8900000, 10600000, 12500000, 14800000, 17500000, 20900000, 24700000, 29300000, 34800000, 41200000, 48900000, 57200000, 69000000, 82000000, 96000000, 115000000, 136000000, 161000000, 192000000, 227000000, 269000000, 319000000, 378000000, 449000000, 528000000, 630000000, 750000000, 890000000, 1050000000, 1250000000, 1480000000, 1750000000, 2090000000, 2147483647, 2147483647 };

// database related
#define PLAYER_SAVESTATE 0
#define PLAYER_TRANSFERSTATE 1
#define MAX_PLAYER_DBSTATES (PLAYER_TRANSFERSTATE + 1)
global bool 9: PlayerDatabaseState[MAXPLAYERS][MAX_PLAYER_DBSTATES];
//global bool 10: TransactionMade;

int GetExpLimit() {
	return LevelCurve[GetStat(STAT_LVL) - 1];
}

int GetExpLimit_Level(int lvl) {
	return LevelCurve[lvl - 1];
}

str GetTalentTag(int id) {
	return StrParam(s:"DND_TALENT", d:id + 1);
}

#define DND_PERKS (DND_PERK_END - DND_PERK_BEGIN + 1)
str StatData[STAT_LVLCRED + 1] = {
	"PSTAT_Strength",
	"PSTAT_Dexterity",
	"PSTAT_Intellect",
	
	"Perk_Sharpshooting",
	"Perk_Brutality",
	"Perk_Endurance",
	"Perk_Wisdom",
	"Perk_Greed",
	"Perk_Medic",
	"Perk_Munitionist",
	"Perk_Deadliness",
	"Perk_Savagery",
	"Perk_Luck",
	
	"Exp",
	"LevelExp",
	"Level",
	"Credit",
	"LevelCredit",
};

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

void HandleHealthPickup(int amt, int isSpecial, int useTarget) {
	if(useTarget)
		SetActivatorToTarget(0);
	int curhp = GetActorProperty(0, APROP_HEALTH);
	int healthcap = GetSpawnHealth();
	int bonus = GetHealingBonuses(PlayerNumber());
	// holds the old amt
	int toGive = amt, base = amt;
	// the percentage of spawn health is amt to be given
	amt = healthcap * amt / 100;
	// consider healing bonuses from quests
	amt = amt * (100 + bonus) / 100;
	
	// health bonus
	if(isspecial == 5) {
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
	
	if(CheckResearchStatus(RES_MEDKITSTORE) == RES_DONE && !isspecial) {
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
	
	HandleHealDependencyCheck();
}

#define DND_SPECIALTYARMOR_REDUCE 75
#define DND_SPECIALTYARMOR_BUFF 50
#define DND_KNIGHT_MELEEREDUCE 50
#define DND_KNIGHTARMOR_MELEEWEP_BONUS 0.4

#define DND_RAVAGER_FACTOR 3
#define DND_RAVAGER_REDUCE 4
#define DND_RAVAGER_DMGMUL 3
#define DND_RAVAGER_DMGDIV 2

#define DND_SYNTHMETAL_HITSCANBUFF 50 // 50%
#define DND_SYNTHMETAL_LIGHTNINGNERF 50
#define DND_LIGHTNINGCOIL_SPECIAL 85 // 85%

bool IsWearingSpecialtyArmor() {
	int armor_id = GetArmorID();
	switch(armor_id) {
		case BODYARMOR_GUNSLINGER:
		case BODYARMOR_OCCULT:
		case BODYARMOR_DEMO:
		case BODYARMOR_ENERGY:
		case BODYARMOR_ELEMENTAL:
		return true;
	}
	return false;
}
#define ARMOR_INTEGER_FACTOR 1000

int GetStat(int stat_id) {
	return CheckInventory(StatData[stat_id]);
}

str GetStatLabel(int id) {
	if(id >= DND_PERK_BEGIN && id <= DND_PERK_END)
		return StrParam(s:"DND_PERK", d:id + 1 - DND_PERK_BEGIN);
	return StrParam(s:"DND_STAT", d:id + 1);
}

int GetActorStat(int tid, int stat_id) {
	return CheckActorInventory(tid, StatData[stat_id]);
}

int GetPlayerEnergyShieldCap(int pnum) {
	int base = GetPlayerAttributeValue(pnum, INV_SHIELD_INCREASE);
	base = (base * (100 + GetPlayerAttributeValue(pnum, INV_PERCENTSHIELD_INCREASE) + GetStat(STAT_INT) / 2)) / 100;
	return base;
}

int GetPlayerEnergyShieldRechargeDelay(int pnum) {
	int res = ESHIELD_RECHARGEDELAY_BASE;
	res = res * (100 - GetPlayerAttributeValue(pnum, INV_SHIELD_RECHARGEDELAY)) / 100;
	if(res < TICRATE)
		res = TICRATE;
	return res;
}

int GetPlayerEnergyShieldRecoveryRate(int pnum, int cap) {
	int bonus = 1;
	if(GetPlayerAttributeValue(pnum, INV_EX_PLAYERPOWERSET1) & PPOWER_CYBER)
		bonus = 2;
	
	int pct = 100 + GetPlayerAttributeValue(pnum, INV_SHIELD_RECOVERYRATE);
	int res = cap * bonus;

	if(bonus > 1 && CheckInventory("Cyborg_Perk5")) {
		res *= DND_CYBERNETIC_FACTOR_MUL;
		res /= DND_CYBERNETIC_FACTOR_DIV;
	}

	if(pct != 100)
		res = res * (100 + GetPlayerAttributeValue(pnum, INV_SHIELD_RECOVERYRATE)) / 100;
	res /= 1000;

	if(!res)
		res = 1;
	return res;
}

int GetPlayerEstimatedArmorProtect(int pnum, int cap) {
	int base_dmg = DND_ESTIMATED_AVG_DAMAGE * (100 + GetMonsterDMGScaling(0, GetActorStat(pnum + P_TIDSTART, STAT_LVL))) / 100;
	return DoArmorRatingEffect(base_dmg, cap) * 100 / base_dmg;
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
	return GetStat(stat) == DND_PERK_MAX;
}

bool HasActorMasteredPerk(int tid, int stat) {
	return GetActorStat(tid, stat) == DND_PERK_MAX;
}

bool CheckPlayerLuckDuplicator(int pnum) {
	return HasActorMasteredPerk(pnum + P_TIDSTART, STAT_LUCK) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE;
}

void SpawnPlayerDrop(int pnum, str actor, int zoffset, int thrust, int setspecial, int setspecial2, bool noRandomVelXY = false) {
	SpawnDrop(actor, zoffset, thrust, setspecial, setspecial2, noRandomVelXY);
	if(CheckPlayerLuckDuplicator(pnum))
		SpawnDrop(actor, zoffset, thrust, setspecial, setspecial2);
}

void SpawnPlayerDropAtActor(int pnum, int dest, str actor, int zoffset, int thrust, int setspecial, int setspecial2, bool noRandomVelXY = false) {
	SpawnDropAtActor(dest, actor, zoffset, thrust, setspecial, setspecial2, noRandomVelXY);
	if(CheckPlayerLuckDuplicator(pnum))
		SpawnDropAtActor(dest, actor, zoffset, thrust, setspecial, setspecial2);
}

bool CheckWellRolled(int pnum) {
	return random(0, 1.0) <= DND_LUCK_GAIN * GetActorStat(pnum + P_TIDSTART, STAT_LUCK) / 3;
}

void CalculateExpRatio() {
	int lvl = CheckInventory("Level");
	int cap = GetExpLimit_Level(lvl);
	if(lvl > 50)
		SetInventory("ExpVisual", (1000 * FixedDiv(CheckInventory("Exp"), cap)) >> 16);
	else
		SetInventory("ExpVisual", 1000 * CheckInventory("Exp") / cap);
}

void CalculatePlayerExpRatio(int tid) {
	int lvl = CheckActorInventory(tid, "Level");
	int cap = GetExpLimit_Level(lvl);
	if(lvl > 50)
		SetActorInventory(tid, "ExpVisual", (1000 * FixedDiv(CheckActorInventory(tid, "Exp"), cap)) >> 16);
	else
		SetActorInventory(tid, "ExpVisual", 1000 * CheckActorInventory(tid, "Exp") / cap);
}

int CheckLevelUp (int pnum) {
	int prevlvl = GetStat(STAT_LVL), exptemp;
	int currlvl;
	// -1 because initial level is 1
	// we need to check for the current up-to-date level, not previous level here!!
	while((currlvl = GetStat(STAT_LVL)) < MAXLEVELS && GetStat(STAT_EXP) >= LevelCurve[currlvl - 1]) {
		exptemp = GetStat(STAT_EXP) - LevelCurve[currlvl - 1];
		if(!((currlvl + 1) % 5)) { // multiples of 5 give perk
			GiveInventory("PerkPoint", 1);
			UpdateActivity(pnum, DND_ACTIVITY_PERKPOINT, 1, 0);
			GiveInventory("PerkedUp", 1);
			ACS_NamedExecuteAlways("DnD Levelup Log", 0, 1);
		}

		GiveInventory("Level", 1);
		SetInventory("Exp", exptemp);
		GiveInventory("AttributePoint", ATTRIB_PER_LEVEL);
		UpdateActivity(pnum, DND_ACTIVITY_ATTRIBUTEPOINT, ATTRIB_PER_LEVEL, 0);
		
		++PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL];
		UpdateActivity(pnum, DND_ACTIVITY_LEVEL, 1, 0);
	}
	return GetStat(STAT_LVL) - prevlvl;
}

void HandleLevelup() {
	int pnum = PlayerNumber();
	int prevlvl = CheckInventory("Level");
	if(CheckLevelUp(pnum)) {
		LocalAmbientSound("RPG/LevelUp", 127);
		GiveInventory("LevelUpEffectSpawner", 1);
		GiveInventory("LeveledUp", 1);
		ACS_NamedExecuteAlways("DnD Levelup Log", 0);
		if(GetStat(STAT_LVL) - 1 == PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL])
			PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL] = GetStat(STAT_LVL);
			
		// sync level cap exp
		CalculateExpRatio();
		// heal on level up flag is on
		if(GetCVar("dnd_healonlevelup"))
			ACS_NamedExecuteAlways("DnD Health Pickup", 0, 100, 0);
		
		int curlvl = CheckInventory("Level");
		// player just leveled and got their perks? check if so
		if
		(
			(prevlvl < DND_CLASSPERK1_LEVEL && curlvl >= DND_CLASSPERK1_LEVEL) ||
			(prevlvl < DND_CLASSPERK2_LEVEL && curlvl >= DND_CLASSPERK2_LEVEL) ||
			(prevlvl < DND_CLASSPERK3_LEVEL && curlvl >= DND_CLASSPERK3_LEVEL)
		)
		{
			HandleClassPerks();
			
			// this is done as new perks might increase some damage factors
			ForcePlayerDamageCaching(pnum);
			
			// make some announcement the player has a new perk
			ACS_NamedExecuteAlways("DnD Announcer", 0, DND_ANNOUNCER_NEWCLASSPERK);
		}
		else
			ACS_NamedExecuteAlways("DnD Announcer", 0, DND_ANNOUNCER_ATTRIBPOINT);
	}
	
	UpdateActivity(pnum, DND_ACTIVITY_EXP, GetStat(STAT_EXP), 0);
}

void GiveExp(int amt, bool resetSpree = false) {
	GiveInventory("Exp", amt);

	if(!resetSpree)
		GiveInventory("SpreeXP", amt);
	else
		SetInventory("SpreeXP", 0);

	GiveInventory("LevelExp", amt);
	HandleLevelup();
	CalculateExpRatio();
}

void GiveActorExp(int tid, int amt) {
	GiveActorInventory(tid, "Exp", amt);
	GiveActorInventory(tid, "SpreeXP", amt);
	GiveActorInventory(tid, "LevelExp", amt);
	GiveActorInventory(tid, "LevelUpChecker", 1);
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
	// if player is >= lvl 80, they only get 25% of the experience -- if too slow precalc and store in an array later
	int tmp = GetActorStat(tid, STAT_LVL);
	if(tmp >= DND_EXP_ADJUST_LEVEL)
		amt = amt * fpow(DND_EXP_ADJUST_LEVELFACTOR, tmp - DND_EXP_ADJUST_LEVEL + 1);

	amt = amt * GetPlayerWisdomBonus(tid - P_TIDSTART, tid) / 100;
	GiveActorExp(tid, amt);
	return amt;
}

int RewardActorCredit(int tid, int amt) {
	amt = amt * GetPlayerGreedBonus(tid - P_TIDSTART, tid) / 100;
	GiveActorCredit(tid, amt);
	return amt;
}

void GiveCredit(int amt) {
	GiveInventory("Credit", amt);
	GiveInventory("LevelCredit", amt);
	GiveInventory("DnD_RefreshRequest", 1);
	GiveInventory("DnD_MoneySpentQuest", amt);
	UpdateActivity(PlayerNumber(), DND_ACTIVITY_CREDIT, amt, 0);
}

void TakeCredit(int amt) {
	TakeInventory("Credit", amt);
	GiveInventory("DnD_RefreshRequest", 1);
	UpdateActivity(PlayerNumber(), DND_ACTIVITY_CREDIT, -amt, 0);
}

void GiveBudget(int amt) {
	GiveInventory("Budget", amt * Clamp_Between(GetCVar("dnd_budget_scale"), 1, BUDGET_SCALE_MAX));
	GiveInventory("DnD_RefreshRequest", 1);
	UpdateActivity(PlayerNumber(), DND_ACTIVITY_BUDGET, amt, 0);
}

void TakeBudget(int amt) {
	TakeInventory("Budget", amt);
	GiveInventory("DnD_RefreshRequest", 1);
	UpdateActivity(PlayerNumber(), DND_ACTIVITY_BUDGET, -amt, 0);
}

void GiveActorBudget(int tid, int amt) {
	GiveActorInventory(tid, "Budget", amt * Clamp_Between(GetCVar("dnd_budget_scale"), 1, BUDGET_SCALE_MAX));
	GiveActorInventory(tid, "DnD_RefreshRequest", 1);
	UpdateActivity(tid - P_TIDSTART, DND_ACTIVITY_BUDGET, amt, 0);
}

void GiveActorCredit(int tid, int amt) {
	GiveActorInventory(tid, "Credit", amt);
	GiveActorInventory(tid, "LevelCredit", amt);
	GiveActorInventory(tid, "DnD_RefreshRequest", 1);
	UpdateActivity(tid - P_TIDSTART, DND_ACTIVITY_CREDIT, amt, 0);
}

void ConsumeAttributePointOn(int stat, int amt) {
	GiveStat(stat, amt);
	UpdateActivity(PlayerNumber(), DND_ACTIVITY_ATTRIBUTEPOINT, -amt, 0);
	TakeInventory("AttributePoint", amt);
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
	
	if(CheckInventory("Cyborg_Perk50")) {
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
	//amt += (amt * GetStrength() * DND_STR_CAPINCREASE) / DND_STR_CAPFACTOR;
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
	return outcome_val * CheckActorInventory(pnum + P_TIDSTART, "Perk_Luck") + GetPlayerAttributeValue(pnum, INV_LUCK_INCREASE);
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

/*void HandleArmorDependencyCheck() {
	// Research Dependency
	if(CheckInventory("Research_Body_Ar_1_Tracker") == GetAmmoCapacity("Research_Body_Ar_1_Tracker") && CheckResearchStatus(RES_EXO1) == RES_NA)
		GiveResearch(RES_EXO1, true);

	// check for thick skin quest
	if(active_quest_id == QUEST_NOARMORS && !CheckInventory(Quest_List[active_quest_id].qchecker)) {
		GiveInventory(Quest_List[active_quest_id].qchecker, 1);
		FailQuest(ActivatorTID());
	}
}*/

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
					GiveActorInventory(tid, "DnD_RefreshRequest", 1);
					TakeActorInventory(tid, "InTradeView", 1);
				}
			}
			else if(IsSet(CheckActorInventory(tid, "DnD_TradeEngaged_1"), pnum)) {
				SetActorInventory(tid, "DnD_TradeEngaged_1", ClearBit(CheckActorInventory(tid, "DnD_TradeEngaged_1"), pnum));
				// fixes disconnect on trade having players name still there bug
				GiveActorInventory(tid, "DnD_RefreshRequest", 1);
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
	return CheckInventory("Perk_Deadliness") * PERK_DEADLINESS_BONUS + GetPlayerAttributeValue(pnum, INV_CRITCHANCE_INCREASE);
}

int GetPercentCritChanceIncrease(int pnum, int wepid) {
	return 	Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITPERCENT][WMOD_ITEMS].val +
			Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITPERCENT][WMOD_WEP].val +
			GetPlayerAttributeValue(pnum, INV_CRITPERCENT_INCREASE);
}

int GetCritChance(int pnum, int victim, int wepid) {
	int chance = GetBaseCritChance(pnum);
	// add other flat crit bonuses here
	if(wepid != -1)
		chance += Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRIT][WMOD_ITEMS].val + Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRIT][WMOD_WEP].val;

	// monster related bonuses
	//if(victim != -1)
	

	// add percent bonuses here
	if(chance)
		chance = FixedMul(chance, 1.0 + GetPercentCritChanceIncrease(pnum, wepid));

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
	int chance = GetCritChance(pnum, victim, wepid);
	
	if(isLightning)
		chance = FixedMul(chance, 1.0 + GetPlayerAttributeValue(pnum, INV_EX_MORECRIT_LIGHTNING));
		
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

int ConfirmedCritFactor(int pnum, int victim, int dmg, int wepid = -1) {
	dmg = dmg * GetCritModifier(pnum, victim, wepid) / 100;
	return dmg;
}

// this one doesnt depend on a weapon, its used as it is in the menu etc.
int GetIndependentCritModifier(int pnum) {
	return DND_BASE_CRITMODIFIER + DND_SAVAGERY_BONUS * CheckInventory("Perk_Savagery") + 
			GetPlayerAttributeValue(pnum, INV_CRITDAMAGE_INCREASE);
}

int GetBaseCritModifier(int pnum, int wepid) {
	int base = GetIndependentCritModifier(pnum);
	int wep_bonus = 0;
	
	if(wepid != -1)
		wep_bonus = (Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITDMG][WMOD_ITEMS].val + Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITDMG][WMOD_WEP].val);
	return base + wep_bonus;
}

int GetCritModifier(int pnum, int victim, int wepid) {
	int base = GetBaseCritModifier(pnum, wepid); // calculates the regular "base" bonuses
	
	// berserker perk50 check
	base += (CheckInventory("Berserker_HitTracker") == DND_BERSERKER_PERK50_MAXSTACKS) * DND_BERSERKER_PERK50_CRITBONUS;
	
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
	
	return base;
}

int GetWeaponModValue(int pnum, int wep, int mod) {
	return Player_Weapon_Infos[pnum][wep].wep_mods[mod][WMOD_ITEMS].val + Player_Weapon_Infos[pnum][wep].wep_mods[mod][WMOD_WEP].val;
}

bool HasWeaponPower(int pnum, int wep, int power) {
	return IsSet(Player_Weapon_Infos[pnum][wep].wep_mods[WEP_MOD_POWERSET1][WMOD_ITEMS].val, power) || IsSet(Player_Weapon_Infos[pnum][wep].wep_mods[WEP_MOD_POWERSET1][WMOD_WEP].val, power);
}

int GetPlayerPercentDamage(int pnum, int wepid, int damage_category) {
	// stuff that dont depend on a wepid
	int res = MapDamageCategoryToPercentBonus(pnum, damage_category);
				
	if(damage_category == DND_DAMAGECATEGORY_ENERGY && IsQuestComplete(0, QUEST_ONLYENERGY))
		res += DND_QUEST_ENERGYBONUS;
				
	// stuff that do ---- removed orb bonus from here
	if(wepid != -1)
		res -= (HasWeaponPower(pnum, wepid, WEP_POWER_GHOSTHIT) * WEP_POWER_GHOSTHIT_REDUCE);
	return res;
}

void RecalculatePlayerLevelInfo() {
	PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] = 0;
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
	int i;
	for(i = 0; i < MAXWEPS; ++i)
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
void ResetPlayerInfo(int pnum) {
	// reset player items
	ResetPlayerInventory(pnum);
	ResetPlayerCharmsUsed(pnum);
	ResetTradeViewList(pnum);
	ResetPlayerStash(pnum);
	// reset weapon mod variable
	ResetAllWeaponMods(pnum);
	ResetMostRecentOrb(pnum);
	//ResetOrbData(pnum);
	ResetPlayerModList(pnum);
	ResetInvestments(pnum);
	SyncAllClientsideVariables(pnum);
}

void ResetHardcoreStuff(int pnum) {
	ResetPlayerInfo(pnum);
	RecalculatePlayerLevelInfo();
	BreakTradesBetween(pnum);
	// may join later, sync everything
	if(PlayerIsSpectator(pnum)) {
		SyncAllClientsideVariables(pnum);
		SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_ITEMSUSED);
		SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_STASH);
	}
}

int MapDamageCategoryToFlatBonus(int pnum, int talent) {
	switch(talent) {
		case DND_DAMAGECATEGORY_BULLET:
		case DND_DAMAGECATEGORY_MELEE:
		return GetPlayerAttributeValue(pnum, INV_FLATPHYS_DAMAGE);
		case DND_DAMAGECATEGORY_OCCULT:
		return GetPlayerAttributeValue(pnum, INV_FLATMAGIC_DAMAGE);
		case DND_DAMAGECATEGORY_EXPLOSIVES:
		return GetPlayerAttributeValue(pnum, INV_FLATEXP_DAMAGE);
		case DND_DAMAGECATEGORY_ENERGY:
		return GetPlayerAttributeValue(pnum, INV_FLATENERGY_DAMAGE);

		case DND_DAMAGECATEGORY_FIRE:
		return GetPlayerAttributeValue(pnum, INV_FLATELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_FLAT_FIREDMG);
		case DND_DAMAGECATEGORY_ICE:
		return GetPlayerAttributeValue(pnum, INV_FLATELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_FLAT_ICEDMG);
		case DND_DAMAGECATEGORY_LIGHTNING:
		return GetPlayerAttributeValue(pnum, INV_FLATELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_FLAT_LIGHTNINGDMG);
		case DND_DAMAGECATEGORY_POISON:
		return GetPlayerAttributeValue(pnum, INV_FLATELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_FLAT_POISONDMG);
	}
	return 0;
}

int MapDamageCategoryToPercentBonus(int pnum, int talent) {
	switch(talent) {
		case DND_DAMAGECATEGORY_BULLET:
		case DND_DAMAGECATEGORY_MELEE:
		return GetPlayerAttributeValue(pnum, INV_PERCENTPHYS_DAMAGE);
		case DND_DAMAGECATEGORY_OCCULT:
		return GetPlayerAttributeValue(pnum, INV_PERCENTMAGIC_DAMAGE);
		case DND_DAMAGECATEGORY_EXPLOSIVES:
		return GetPlayerAttributeValue(pnum, INV_PERCENTEXP_DAMAGE);
		case DND_DAMAGECATEGORY_ENERGY:
		return GetPlayerAttributeValue(pnum, INV_PERCENTENERGY_DAMAGE);

		case DND_DAMAGECATEGORY_FIRE:
		return GetPlayerAttributeValue(pnum, INV_PERCENTELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_PERCENTFIRE_DAMAGE);
		case DND_DAMAGECATEGORY_ICE:
		return GetPlayerAttributeValue(pnum, INV_PERCENTELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_PERCENTICE_DAMAGE);
		case DND_DAMAGECATEGORY_LIGHTNING:
		return GetPlayerAttributeValue(pnum, INV_PERCENTELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_PERCENTLIGHTNING_DAMAGE);
		case DND_DAMAGECATEGORY_POISON:
		return GetPlayerAttributeValue(pnum, INV_PERCENTELEM_DAMAGE) + GetPlayerAttributeValue(pnum, INV_PERCENTPOISON_DAMAGE);
	}
	return 0;
}

int GetFlatHealthDamageFactor(int factor) {
	return GetSpawnHealth() / factor;
}

#define DND_BASE_OVERLOADTICK 5
#define DND_BASE_OVERLOADTIME (105 / DND_BASE_OVERLOADTICK) // 3 seconds -- 105 / 5
int GetOverloadTime(int pnum) {
	return (DND_BASE_OVERLOADTIME + ((GetPlayerAttributeValue(pnum, INV_OVERLOAD_DURATION) * TICRATE) >> 16)) / DND_BASE_OVERLOADTICK;
}

// returns fixed point range
int GetPlayerMeleeRange(int pnum, int range) {
	return FixedMul(range, 1.0 + 0.1 * (GetPlayerAttributeValue(pnum, INV_MELEERANGE) + GetStat(STAT_BRUT) * DND_PERK_BRUTALITY_RANGEINC));
}

#define DND_BASE_IGNITEDMG 20
int GetFireDOTDamage(int pnum, int bonus = 0) {
	// flat dmg
	int dmg = 	DND_BASE_IGNITEDMG + 
				bonus +
				GetPlayerAttributeValue(pnum, INV_FLAT_FIREDMG) + 
				GetPlayerAttributeValue(pnum, INV_EX_FLATDOT);
	
	// percent increase
	dmg = dmg * (100 + GetPlayerAttributeValue(pnum, INV_IGNITEDMG) + GetPlayerAttributeValue(pnum, INV_INCREASEDDOT)) / 100;
	
	// dot multi;
	dmg = dmg * (100 + GetPlayerAttributeValue(pnum, INV_DOTMULTI)) / 100;
	
	// hellfire amulet -- moved here for ignite calculation specifically
	if(IsAccessoryEquipped(pnum + P_TIDSTART, DND_ACCESSORY_AMULETHELLFIRE))
		dmg = ApplyDamageFactor_Safe(dmg, DND_AMULETHELL_AMP, DND_AMULETHELL_FACTOR);
	
	return dmg;
}

// dont include flat ele dmg and percent damage here, as they are applied to the attacks that inflicted the poison already, no double application!
int GetPoisonDOTDamage(int pnum, int base_poison) {
	int dmg = base_poison;
	if(!dmg)
		dmg = 1;
		
	// flat dmg
	dmg += 	GetPlayerAttributeValue(pnum, INV_FLAT_POISONDMG) + 
			GetPlayerAttributeValue(pnum, INV_EX_FLATDOT);
	
	// percent increase
	dmg = dmg * (100 + GetPlayerAttributeValue(pnum, INV_POISON_TICDMG) + GetPlayerAttributeValue(pnum, INV_INCREASEDDOT)) / 100;
	
	// dot multi
	dmg = dmg * (100 + GetPlayerAttributeValue(pnum, INV_DOTMULTI)) / 100;
	
	return dmg;
}

#define DND_BASE_LIFESTEALCAP 20
int GetLifestealCap(int pnum) {
	// avoid recalculating over and over if possible
	int hp_cap = Max(CheckInventory("PlayerHealthCap"), GetSpawnHealth());
	return Clamp_Between((hp_cap * (DND_BASE_LIFESTEALCAP + GetPlayerAttributeValue(pnum, INV_LIFESTEAL_CAP))) / 100, 1, hp_cap);
}

#define DND_BASE_LIFESTEALRATE 20
int GetLifestealRate(int pnum) {
	return DND_BASE_LIFESTEALRATE * (100 - GetPlayerAttributeValue(pnum, INV_LIFESTEAL_RATE)) / 100;
}

#define DND_BASE_LIFERECOVERY 1 // 1% of healthcap
int GetLifestealLifeRecovery(int pnum, int cap) {
	// avoid recalculating over and over if possible
	cap = cap * DND_BASE_LIFERECOVERY / 100;
	cap = cap * (100 + GetPlayerAttributeValue(pnum, INV_LIFESTEAL_RECOVERY)) / 100;
	if(!cap)
		cap = 1;
	
	return cap;
}

int GetMeleeRangeIncrease(int tid) {
	return 	GetPlayerAttributeValue(tid - P_TIDSTART, INV_MELEERANGE) +
			GetActorStat(tid, STAT_BRUT) * DND_PERK_BRUTALITY_RANGEINC;
}

// returns true if monster isn't ailment immune, or we can bypass it
bool CheckAilmentImmunity(int pnum, int m_id, int ailment_mod) {
	// is not immune or if it is, we rolled ailment ignore chance
	return !MonsterProperties[m_id].trait_list[ailment_mod] || random(1, 100) < GetPlayerAttributeValue(pnum, INV_CHANCE_AILMENTIGNORE);
}

#define DND_BASE_IGNITECHANCE 15 // 15%
#define DND_BASE_IGNITEPROLIFCHANCE 20 // 20% chance to prolif on monster death
#define DND_BASE_IGNITEPROLIFRANGE 128.0
#define DND_BASE_IGNITEPROLIFCOUNT 5 // max 5 enemies can be proliferated to
#define DND_MAX_IGNITEPROLIFS 128 // max 128 enemies can be proliferated to from one target

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
	return DND_BASE_IGNITEPROLIFRANGE * (100 + GetPlayerAttributeValue(pnum, INV_IGNITE_PROLIFRANGE)) / 100;
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
#define DND_BASE_CHILL_DAMAGETHRESHOLD 10 // 10% of the monster's health
#define DND_BASE_FREEZECHANCE_PERSTACK 2 // 10% base at max slow stacks
int GetChillEffect(int pnum, int stacks) {
	// call with 1 stack to get "per stack" value
	int chill = stacks * DND_BASE_CHILL_SLOW * (100 + GetPlayerAttributeValue(pnum, INV_SLOWEFFECT)) / 100;
	
	if(chill > 0.99)
		chill = 0.99;
	
	return chill;
}

int GetChillThreshold(int pnum, int stacks) {
	return (DND_BASE_CHILL_DAMAGETHRESHOLD * (100 + GetPlayerAttributeValue(pnum, INV_CHILLTHRESHOLD)) / 100) * stacks;
}

int GetFreezeChance(int pnum, int stacks) {
	return DND_BASE_FREEZECHANCE_PERSTACK * stacks * (100 + GetPlayerAttributeValue(pnum, INV_FREEZECHANCE)) / 100;
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
	return CombineMultiplicativeFactors(
		GetPlayerAttributeValue(pnum, INV_PELLET_INCREASE),
		CheckInventory("Hobo_Perk25") * DND_HOBO_SHOTGUNPELLETBONUS
	);
}

int GetPelletCount(int pnum, int base) {
	// factor base is 1.0
	return ApplyFixedFactorToInt(base, GetPelletIncrease(pnum) - 1.0);
}

int HandleStatBonus(int pnum, int strength, int dexterity, int intellect, bool isMelee) {
	if(HasMasteredPerk(STAT_SHRP))
		dexterity += DND_SHARPSHOOTER_MASTERY_BONUS;

	// 1.0 is 100%, we get stuff like 0.03 here for 3% etc.
	int statOf = GetStat(STAT_STR) * strength + GetStat(STAT_DEX) * dexterity + GetStat(STAT_INT) * intellect;

	// brutality is a more multiplier, if there are other "more" things related to melee, keep multiplying here
	if(isMelee)
		statOf = (statOf + (GetPlayerAttributeValue(pnum, INV_MELEEDAMAGE) << 16) / 100) * (100 + GetStat(STAT_BRUT) * DND_PERK_BRUTALITY_DAMAGEINC) / 100;
	else
		statOf = (statOf * (100 + GetStat(STAT_SHRP) * DND_PERK_SHARPSHOOTER_INC)) / 100;

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
		return Clamp_Between(res, 0, cap);
	return Clamp_Between(res, 0, GetMaxResistCap(pnum));
}

int GetExplosiveRepeatChance(int pnum) {
	return GetPlayerAttributeValue(pnum, INV_ESS_KRULL);
}

int GetResearchResistBonuses() {
	int res = IMP_RES_ADD_1 * (CheckResearchStatus(RES_IMP1) == RES_DONE);
	res += IMP_RES_ADD_2 * (CheckResearchStatus(RES_IMP2) == RES_DONE);
	res += IMP_RES_ADD_3 * (CheckResearchStatus(RES_IMP3) == RES_DONE);
	
	// cyborg's bonus
	if(CheckInventory("Cyborg_Perk25"))
		res += res * DND_CYBORG_CYBER_MULT / DND_CYBORG_CYBER_DIV;

	return res;
}

int GetSelfExplosiveResist(int pnum) {
	int base = 1.0; // 100%
	
	if(CheckInventory("Marine_Perk5"))
		base = FixedMul(base, (100 - DND_MARINE_SELFEXPLOSIVEREDUCE) * 1.0 / 100);
	
	// get player selfdmg res
	int temp = (GetPlayerAttributeValue(pnum, INV_SELFDMG_RESIST) << 16) + GetPlayerAttributeValue(pnum, INV_DMGREDUCE_EXPLOSION);

	// roll damage up
	if(temp) {
		// temp ranges between 0-90.0 potentially, map it back into 1.0 range
		temp = ApplyResistCap(pnum, temp);
		base = FixedMul(base, 1.0 - temp / 100);
	}
	
	// properly include this ability's benefit here, including cyborg check
	if(CheckInventory("Ability_ExplosionMastery")) {
		if(!CheckInventory("Cyborg_Perk25"))
			base = FixedMul(base, (100 - DND_EXP_RES_ABILITY_BONUS) * 1.0 / 100);
		else
			base = FixedMul(base, (100 - (DND_EXP_RES_ABILITY_BONUS + DND_EXP_RES_ABILITY_BONUS * DND_CYBORG_CYBER_MULT / DND_CYBORG_CYBER_DIV)) * 1.0 / 100);
	}
	
	// apply impact protection research
	base = FixedMul(base, (100 - GetResearchResistBonuses()) * 1.0 / 100);
	
	// golgoth quest
	if(IsQuestComplete(0, QUEST_KILLGOLGOTH))
		base = FixedMul(base, (100 - DND_GOLGOTH_GAIN) * 1.0 / 100);
		
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

#endif
