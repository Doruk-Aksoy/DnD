#ifndef DND_EXP_IN
#define DND_EXP_IN

#include "DnD_Quests.h"
#include "DnD_CommonStat.h"
#include "DnD_Common.h"
#include "DnD_Charms.h"
#include "DnD_Elixirs.h"
#include "DnD_Activity.h"

#define DND_CRITSTATE_NOCALC 0
#define DND_CRITSTATE_CONFIRMED 1
bool PlayerCritState[MAXPLAYERS][2][MAXWEPS];

#define DND_BASE_CRITMODIFIER 200
#define DND_HARDCORE_DROPRATEBONUS 0.15

#define EXO_AR_ADD_1 5
#define EXO_AR_ADD_2 6
#define EXO_AR_ADD_3 9

#define TALENT_CAP 100

#define EXP_SCALE_MAX 10
#define CREDIT_SCALE_MAX 10
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

global bool 9: PlayerWillBeSaved[MAXPLAYERS];
global bool 10: PlayerTransferred[MAXPLAYERS];

int GetExpLimit() {
	return LevelCurve[GetStat(STAT_LVL) - 1];
}

int GetExpLimit_Level(int lvl) {
	return LevelCurve[lvl - 1];
}

str TalentNames[MAX_TALENTS] = {
	"Talent_Ballistic",
	"Talent_Melee",
	
	"Talent_Energy",
	"Talent_Explosive",
	"Talent_Occult",
    "Talent_Elemental"
};

str GetTalentTag(int id) {
	return StrParam(s:"DND_TALENT", d:id + 1);
}

#define DND_PERKS (DND_PERK_END - DND_PERK_BEGIN + 1)
str StatData[STAT_LVLCRED + 1] = {
	"PSTAT_Strength",
	"PSTAT_Dexterity",
	"PSTAT_Bulkiness",
	"PSTAT_Charisma",
	"PSTAT_Vitality",
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

enum {
	DND_ARMOR_BONUS,
	DND_ARMOR_GREEN,
	DND_ARMOR_YELLOW,
	DND_ARMOR_BLUE,
	DND_ARMOR_RED,
	
	DND_ARMOR_GUNSLINGER,
	DND_ARMOR_OCCULT,
	DND_ARMOR_DEMO,
	DND_ARMOR_ENERGY,
	DND_ARMOR_ELEMENTAL,
	
	DND_ARMOR_SUPER,
	DND_ARMOR_CYBERNETIC,
	DND_ARMOR_DUELIST,
	DND_ARMOR_NECRO,
	DND_ARMOR_KNIGHT,
	DND_ARMOR_RAVAGER,
	DND_ARMOR_SYNTHMETAL,
	DND_ARMOR_LIGHTNINGCOIL
};
#define MAXARMORS (DND_ARMOR_LIGHTNINGCOIL + 1)

#define DND_SPECIALTYARMOR_REDUCE 75
#define DND_SPECIALTYARMOR_BUFF 50
#define DND_KNIGHT_MELEEREDUCE 50
#define DND_KNIGHTARMOR_MELEEWEP_BONUS 0.4
#define DND_SYNTHMETAL_HITSCANBUFF 50 // 50%
#define DND_SYNTHMETAL_LIGHTNINGNERF 50
#define DND_LIGHTNINGCOIL_SPECIAL 85 // 85%

bool IsWearingSpecialtyArmor() {
	int armor_id = GetArmorID();
	switch(armor_id) {
		case DND_ARMOR_GUNSLINGER:
		case DND_ARMOR_OCCULT:
		case DND_ARMOR_DEMO:
		case DND_ARMOR_ENERGY:
		case DND_ARMOR_ELEMENTAL:
		return true;
	}
	return false;
}

str ArmorIcons[MAXARMORS] = {
	"BON2C0",
	"ARM1A0",
	"ARM3A0",
	"ARM2A0",
	"QRARA0",
	
	"ARM9A0",
	"AR10A0",
	"AR11A0",
	"AR12A1",
    "AR14A0",
    
    "AR13A0",
	"ARM4A1",
	"ARM5A0",
	"ARM6A0",
	"ARM7A0",
	"ARM8A0",
	"AR15B0",
	"AR16A0"
};

#define ARMOR_INTEGER_FACTOR 1000
#define ARMORDATA_BASEAMOUNT 0
#define ARMORDATA_PROTECTIONFACTOR 1
int ArmorData[MAXARMORS][2] = {
	{	100,	0.33	},
	{	100,	0.33	},
	{	150,	0.417	},
	{	200,	0.5		},
	{	300,	0.75	},
	
	{	200,	0.33	},
	{	200,	0.33	},
	{	200,	0.33	},
	{	200,	0.33	},
	{	200,	0.33	},
	
	{	400,	1.0		},
	{	300,	0.45	},
	{	250,	0.33	},
	{	400,	0.55	},
	{	400,	0.35	},
	{	250,	0.25	},
	{	400,	0.65	},
	{	300,	0.4		}
};

int GetArmorAmount() {
	return CheckInventory("ArmorAmount");
}

int GetActorArmorAmount(int tid) {
	return CheckActorInventory(tid, "ArmorAmount");
}

int GetArmorID() {
	return CheckInventory("DnD_ArmorType") - 1;
}

int GetActorArmorID(int tid) {
	return CheckActorInventory(tid, "DnD_ArmorType") - 1;
}

void SetArmorAmount(int amt) {
	SetInventory("ArmorAmount", amt);
	SetInventory("ArmorAmountVisual", amt);
}

void AddArmorAmount(int amt) {
	GiveInventory("ArmorAmount", amt);
	GiveInventory("ArmorAmountVisual", amt);
}

void TakeArmorAmount(int amt) {
	TakeInventory("ArmorAmount", amt);
	TakeInventory("ArmorAmountVisual", amt);
	
	// no armor left, remove it
	if(!CheckInventory("ArmorAmount"))
		SetInventory("DnD_ArmorType", 0);
}

// we always add +1, as 0 is no armor
void SetArmorType(int id) {
	SetInventory("DnD_ArmorType", id + 1);
	SetAmmoCapacity("ArmorAmountVisual", GetArmorCapFromID(id));
}

void UpdateArmorVisual() {
	SetAmmoCapacity("ArmorAmountVisual", GetArmorCapFromID(GetArmorID()));
}

void RemoveAllArmor() {
	SetInventory("DnD_ArmorType", 0);
	SetArmorAmount(0);
}

#define DND_MIN_ARMOR_EFFICIENCY 0.1
int GetArmorEfficiency() {
	int temp = Clamp_Between(GetBulkiness(), 0, DND_STAT_FULLMAX);
	if(!temp)
		return 0;
	
	if(temp > 100) {
		temp -= 100;
		return Clamp_Between(DND_BULKINESS_GAIN * 100 + DND_BULKINESS_GAIN_AFTER100 * temp, DND_MIN_ARMOR_EFFICIENCY, 1.0);
	}
	return Clamp_Between(DND_BULKINESS_GAIN * temp, DND_MIN_ARMOR_EFFICIENCY, 1.0);
}

enum {
	DND_CKEY_BRONZE,
	DND_CKEY_SILVER,
	DND_CKEY_GOLD
};

#define MAX_CHEST_KEYS DND_CKEY_GOLD + 1
str ChestKeyTypes[MAX_CHEST_KEYS] = {
	"BronzeChestKey",
	"SilverChestKey",
	"GoldChestKey"
};

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

void GiveStat(int stat_id, int amt) {
	// get cap
	int lim = stat_id <= DND_ATTRIB_END ? DND_STAT_FULLMAX : DND_PERK_MAX;
	amt = Clamp_Between(CheckInventory(StatData[stat_id]) + amt, 0, lim) - CheckInventory(StatData[stat_id]);
	GiveInventory(StatData[stat_id], amt);
	
	if(lim == DND_STAT_FULLMAX)
		UpdateActivity(PlayerNumber(), DND_ACTIVITY_ATTRIBUTE, amt, stat_id);
	else if(lim == DND_PERK_MAX)
		UpdateActivity(PlayerNumber(), DND_ACTIVITY_PERK, amt, stat_id);
		
	UpdatePlayerKnockbackResist();
	UpdateArmorVisual();
}

/*void GiveActorStat(int tid, int stat_id, int amt) {
	// get cap
	int lim = stat_id <= DND_ATTRIB_END ? DND_STAT_FULLMAX : DND_PERK_MAX;
	amt = Clamp_Between(CheckActorInventory(tid, StatData[stat_id]) + amt, 0, lim) - CheckActorInventory(tid, StatData[stat_id]);
	GiveActorInventory(tid, StatData[stat_id], amt);
	
	if(lim == DND_STAT_FULLMAX)
		UpdateActivity(tid - P_TIDSTART, DND_ACTIVITY_ATTRIBUTE, amt, stat_id);
	else if(lim == DND_PERK_MAX)
		UpdateActivity(tid - P_TIDSTART, DND_ACTIVITY_PERK, amt, stat_id);
}*/

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

void GiveExp(int amt) {
	GiveInventory("Exp", amt);
	GiveInventory("SpreeXP", amt);
	GiveInventory("LevelExp", amt);
	GiveInventory("LevelUpChecker", 1);
	CalculateExpRatio();
}

void GiveActorExp(int tid, int amt) {
	GiveActorInventory(tid, "Exp", amt);
	GiveActorInventory(tid, "SpreeXP", amt);
	GiveActorInventory(tid, "LevelExp", amt);
	GiveActorInventory(tid, "LevelUpChecker", 1);
	CalculatePlayerExpRatio(tid);
}

void GiveCredit(int amt) {
	GiveInventory("Credit", amt);
	GiveInventory("LevelCredit", amt);
}

void GiveBudget(int amt) {
	GiveInventory("Budget", amt * Clamp_Between(GetCVar("dnd_budget_scale"), 1, BUDGET_SCALE_MAX));
}

void GiveActorBudget(int tid, int amt) {
	GiveActorInventory(tid, "Budget", amt * Clamp_Between(GetCVar("dnd_budget_scale"), 1, BUDGET_SCALE_MAX));
}

void GiveActorCredit(int tid, int amt) {
	GiveActorInventory(tid, "Credit", amt);
	GiveActorInventory(tid, "LevelCredit", amt);
}

void ConsumeAttributePointOn(int stat, int amt) {
	GiveInventory(StatData[stat], amt);
	TakeInventory("AttributePoint", amt);
}

void ConsumePerkPointOn(int perk, int amt) {
	GiveInventory(StatData[perk], amt);
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

int CalculateArmorCapBonuses(int pnum) {
	int res = CheckInventory("DnD_QuestReward_ArmorCapIncrease") * DND_QUEST_ARMORBONUS;
	
	// consider orb effects
	res = 	GetDataFromOrbBonus(pnum, OBI_ARMORFLAT, -1) + 
			Player_Elixir_Bonuses[pnum].armor_flat_bonus + 
			GetPlayerAttributeValue(pnum, INV_ARMOR_INCREASE);
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

// used for displaying to hud
int GetArmorCap(bool useMenuShow) {
	// see if this DND_BASE_ARMOR_SHOW breaks anything -- breaks armor shards
	int pnum = PlayerNumber();
	int res = CalculateArmorCapBonuses(pnum) + DND_ARMOR_PER_BUL * GetBulkiness();
	if(useMenuShow)
		res += DND_BASE_ARMOR_SHOW;
	else
		res += DND_BASE_ARMOR;
	//res += (res * GetStrength() * DND_STR_CAPINCREASE) / DND_STR_CAPFACTOR;
	
	int percent = 	GetDataFromOrbBonus(pnum, OBI_ARMORPERCENT, -1) + DND_TORRASQUE_BOOST * CheckInventory("DnD_QuestReward_TorrasqueBonus") +
					CheckInventory("CelestialCheck") * CELESTIAL_BOOST +
					GetResearchArmorBonuses() +
					Player_Elixir_Bonuses[pnum].armor_percent_bonus + GetPlayerAttributeValue(pnum, INV_ARMORPERCENT_INCREASE);
	res += (res * percent) / 100;
	return res;
}

// used for deciding armor pickup values
int GetArmorCapFromID(int armor_id) {
	int amt = ArmorData[armor_id][ARMORDATA_BASEAMOUNT];
	int pnum = PlayerNumber();
	
	// any other armor besides the armor bonuses
	amt += CalculateArmorCapBonuses(pnum) + DND_ARMOR_PER_BUL * GetBulkiness();
	amt += amt * (GetDataFromOrbBonus(pnum, OBI_ARMORPERCENT, -1) + DND_TORRASQUE_BOOST * CheckInventory("DnD_QuestReward_TorrasqueBonus")) / 100;
	//amt += (amt * GetStrength() * DND_STR_CAPINCREASE) / DND_STR_CAPFACTOR;
	amt += (amt * CheckInventory("CelestialCheck") * CELESTIAL_BOOST) / 100;
	amt += (amt * GetResearchArmorBonuses()) / 100;
	amt += (amt * (Player_Elixir_Bonuses[pnum].armor_percent_bonus + GetPlayerAttributeValue(pnum, INV_ARMORPERCENT_INCREASE))) / 100;
	return amt;
}

// compare armor types t1 and t2, check if t1 is of higher tier than t2
// if base armor of item greater and it is listed greater
bool IsArmorTierHigher(int t1, int t2) {
	if(t1 < 0 || t2 < 0)
		return true;
	return ArmorData[t1][ARMORDATA_BASEAMOUNT] > ArmorData[t2][ARMORDATA_BASEAMOUNT] && t1 > t2;
}

void HandleArmorPickup(int armor_type, int amount, bool replace) {
	int armor = GetArmorAmount(), cap = 0, curr_armor_id = GetArmorID();
	GiveInventory("DnD_BoughtArmor", 1);
	//printbold(s:"init ", d:armor, s: " ", d:armor_type, s: " ", d:amount, s: " ", d:curr_armor_id);
	// this will prevent -1 array index operations
	// make sure if there's no armor, despite lingering DnD_ArmorType, force replace
	if(!curr_armor_id || !armor)
		replace = true;

	bool highertier = IsArmorTierHigher(armor_type, curr_armor_id);
	// Give new armor type only if it's a higher tier, or is a replacement
	if(replace || highertier) {
		// Set new type
		SetArmorType(armor_type);
	}
	
	// adapt armor count to whatever stats makes it be
	// update
	curr_armor_id = GetArmorID();
	cap = GetArmorCapFromID(curr_armor_id);
	
	// only do update if my armor is less than cap
	int check_cap = cap;
	int base_amt = ArmorData[curr_armor_id][ARMORDATA_BASEAMOUNT];
	
	// shards can make armor go up to 3x of whatever current armor cap is (with just armor shard, can go up to 300).
	if(armor_type == DND_ARMOR_BONUS) {
		check_cap *= 3;
		base_amt = ArmorData[DND_ARMOR_GREEN][ARMORDATA_BASEAMOUNT];
	}
	
	if(armor < check_cap) {
		amount = (amount * cap) / base_amt;
		
		// printbold(s:"want to add ", d:Min(armor + amount, cap), s: " with armor, amt, cap = ", d:armor, s: " ", d:amount, s: " ", d:cap);
		SetArmorAmount(Min(armor + amount, check_cap));
		GiveInventory("Research_Body_Ar_1_Tracker", Min(armor + amount, check_cap) - armor); //Trackers should only be additive, so this is more complicated.
		
		HandleArmorDependencyCheck();
	}
	else // hack to sync
		SetInventory("ArmorAmountVisual", armor);
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

int GetDropChance(int pnum, bool isElite) {
	int base = 1.0; // base val
	int temp = 0;
	base += GetPlayerAttributeValue(pnum, INV_DROPCHANCE_INCREASE) + GetDataFromOrbBonus(pnum, OBI_DROPCHANCE, -1); // additive bonuses first
	if(isElite && CheckActorInventory(pnum + P_TIDSTART, "DnD_QuestReward_EliteDropBonus"))
		base += DND_ELITEDROP_GAIN;
	// luck benefits are multiplicative
	temp = GetPlayerAttributeValue(pnum, INV_LUCK_INCREASE) + Player_Elixir_Bonuses[pnum].luck;
	base = FixedMul(base, 1.0 + DND_LUCK_GAIN * CheckActorInventory(pnum + P_TIDSTART, "Perk_Luck") + temp);
	if(GetCVar("dnd_mode") == DND_MODE_HARDCORE)
		base = FixedMul(base, 1.0 + DND_HARDCORE_DROPRATEBONUS);
	return base;
}

int GetPlayerWisdomBonus(int pnum) {
	return GetPlayerAttributeValue(pnum, INV_EXPGAIN_INCREASE) + GetDataFromOrbBonus(pnum, OBI_WISDOMPERCENT, -1);
}

int GetPlayerGreedBonus(int pnum) {
	return GetPlayerAttributeValue(pnum, INV_CREDITGAIN_INCREASE) + GetDataFromOrbBonus(pnum, OBI_GREEDPERCENT, -1);
}

bool RunDefaultDropChance(int pnum, bool isElite, int basechance) {
	return RunDropChance(pnum, isElite, basechance, 0, 1.0);
}

bool RunDropChance(int pnum, bool isElite, int basechance, int low, int high) {
	return FixedMul(GetDropChance(pnum, isElite), basechance) >= random(low, high);
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
		GiveInventory("TaltosUnsetEffect", 1);
		TakeInventory("TaltosUp", 1);
	}
	
	if(IsAccessoryEquipped(this, DND_ACCESSORY_HATESHARD)) {
		GiveInventory("HateCheck", 1);
		GiveInventory("PowerReflection", 1);
	}
	else {
		TakeInventory("PowerReflection", 1);
		TakeInventory("HateCheck", 1);
	}
	
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
		GiveInventory("GryphonSpeed", 1);
		UpdatePlayerKnockbackResist();
	}
	else {
		HandleCurseImmunityRemoval();
		TakeInventory("GryphonCheck", 1);
		TakeInventory("GryphonSpeed", 1);
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
	if(IsAccessoryEquipped(this, DND_ACCESSORY_CELESTIAL)) {
		SetInventory("CelestialCheck", 1);
		GiveInventory("CelestialSlow", 1);
	}
	else {
		SetInventory("CelestialCheck", 0);
		TakeInventory("CelestialSlow", 1);
	}
	
	SetInventory("HunterTalismanCheck", IsAccessoryEquipped(this, DND_ACCESSORY_HUNTERTALISMAN));
	SetInventory("VeilCheck", IsAccessoryEquipped(this, DND_ACCESSORY_VEIL));
}

bool HasNoSigilPower() {
	return !CheckInventory("ElementPower_Fire") && !CheckInventory("ElementPower_Ice") && !CheckInventory("ElementPower_Lightning") && !CheckInventory("ElementPower_Earth");
}

// Takes a stat from the player, also removing effects of it
void TakeStat(int stat_id, int amt) {
	TakeInventory(StatData[stat_id], amt);
	if(stat_id <= DND_ATTRIB_END)
		UpdateActivity(PlayerNumber(), DND_ACTIVITY_ATTRIBUTE, amt, stat_id);
	else if(stat_id <= DND_PERK_END)
		UpdateActivity(PlayerNumber(), DND_ACTIVITY_PERK, amt, stat_id);
		
	UpdateArmorVisual();
	UpdatePlayerKnockbackResist();
}

void UpdatePerkStuff() {
	SetAmmoCapacity("StoredMedkit", GetAmmoCapacity("StoredMedkit") + 15 * CheckInventory("Perk_Medic"));
}

bool HasKilledLegendary(int id) {
	return IsSet(CheckInventory("LegendaryKills"), id);
}

void UpdateLegendaryKill(int pnum, int mon_id) {
	SetActorInventory(pnum + P_TIDSTART, "LegendaryKills", SetBit(CheckActorInventory(pnum + P_TIDSTART, "LegendaryKills"), mon_id));
}

int GetPlayerWeaponEnchant(int pnum, int wepid) {
	return GetDataFromOrbBonus(pnum, OBI_WEAPON_ENCHANT, wepid) + Player_Weapon_Infos[pnum][wepid].quality;
}

void HandleArmorDependencyCheck() {
	// Research Dependency
	if(CheckInventory("Research_Body_Ar_1_Tracker") == GetAmmoCapacity("Research_Body_Ar_1_Tracker") && CheckResearchStatus(RES_EXO1) == RES_NA)
		GiveResearch(RES_EXO1, true);

	// check for thick skin quest
	if(active_quest_id == QUEST_NOARMORS && !CheckInventory(Quest_Checkers[active_quest_id])) {
		GiveInventory(Quest_Checkers[active_quest_id], 1);
		FailQuest(ActivatorTID());
	}
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
	return Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITPERCENT].val + GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITPERCENT, wepid);
}

int GetCritChance(int pnum, int wepid) {
	int chance = GetBaseCritChance(pnum);
	int temp = 0;
	// add current weapon crit bonuses
	if(wepid != -1) {
		chance += Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRIT].val;
		chance += GetDataFromOrbBonus(pnum, OBI_WEAPON_CRIT, wepid);
		temp = GetPercentCritChanceIncrease(pnum, wepid);
	}
	// add percent bonus
	if(chance)
		chance = FixedMul(chance, 1.0 + temp + GetPlayerAttributeValue(pnum, INV_CRITPERCENT_INCREASE));
	return chance;
}

bool CheckCritChance(int wepid, bool isSpecial, int extra) {
	int pnum = PlayerNumber();
	// veil disables crits for the cooldown period
	if(CheckInventory("VeilCheck") && CheckInventory("VeilCooldown"))
		return false;
	bool res = false;
	int chance = GetCritChance(pnum, wepid);
	
	if(IsWeaponLightningType(wepid, extra, isSpecial))
		chance = FixedMul(chance, 1.0 + GetPlayerAttributeValue(pnum, INV_EX_MORECRIT_LIGHTNING));
		
	//printbold(s:"running crit chance: ", f:chance);
	
	res = chance > random(0, 1.0);
	
	// reroll if bad luck and lucky crit is on
	if(!res && CheckInventory("StatbuffCounter_LuckyCrit"))
		res = chance > random(0, 1.0);
	
	if(res) {
		if(wepid != -1)
			PlayerCritState[pnum][DND_CRITSTATE_CONFIRMED][wepid] = true;
		GiveInventory("DnD_CritToken", 1);
		
		// veil check
		if(CheckInventory("VeilCheck") && !CheckInventory("VeilCooldown") && !CheckInventory("VeilMarkTimer")) {
			GiveInventory("VeilMarkTimer", 1);
			ActivatorSound("VeilOfAssassin/Active", 97);
		}
	}
	
	return res;
}

void HandleHunterTalisman() {
	if(CheckInventory("HunterTalismanCheck") && !CheckInventory("HunterTalismanCooldown")) {
		ActivatorSound("HunterTalisman/Activate", 1.0);
		GiveInventory("HunterTalismanCooldown", 1);
		GiveInventory("HunterTalismanEffector", 1);
	}
}

int ConfirmedCritFactor(int dmg) {
	dmg = dmg * GetCritModifier() / 100;
	HandleHunterTalisman();
	return dmg;
}

// this one doesnt depend on a weapon, its used as it is in the menu etc.
int GetIndependentCritModifier(int pnum) {
	return DND_BASE_CRITMODIFIER + DND_SAVAGERY_BONUS * CheckInventory("Perk_Savagery") + 
			GetPlayerAttributeValue(pnum, INV_CRITDAMAGE_INCREASE);
}

int GetBaseCritModifier(int pnum, int wepid) {
	return 	GetIndependentCritModifier(pnum) +
			Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_CRITDMG].val +
			GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, wepid);
}

int GetCritModifier() {
	int pnum = PlayerNumber(), wepid = GetWeaponPosFromTable();
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
	return base;
}

// common place of weapon and orb data
int GetWeaponModValue(int pnum, int wep, int mod) {
	return Player_Weapon_Infos[pnum][wep].wep_mods[mod].val + Player_Orb_Data[pnum].weapon_stat_bonuses[wep].wep_mods[mod].val;
}

bool HasWeaponPower(int pnum, int wep, int power) {
	return 	IsSet(Player_Weapon_Infos[pnum][wep].wep_mods[WEP_MOD_POWERSET1].val, power) || 
			IsSet(GetDataFromOrbBonus(pnum, OBI_WEAPON_POWERSET1, wep), power);
}

int GetPlayerPercentDamage(int pnum, int wepid, int talent_type) {
	// stuff that dont depend on a wepid
	int res = 	GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, talent_type) +
				GetPlayerAttributeValue(pnum, INV_DAMAGEPERCENT_INCREASE) +
				MapTalentToPercentBonus(pnum, talent_type) +
				Player_Elixir_Bonuses[pnum].damage_type_bonus[talent_type];
				
	// stuff that do
	if(wepid != -1)
		res += 	GetDataFromOrbBonus(pnum, OBI_WEAPON_DMG, wepid) -
				(HasWeaponPower(pnum, wepid, WEP_POWER_GHOSTHIT) * WEP_POWER_GHOSTHIT_REDUCE);
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
			if(PlayerInformationInLevel[PLAYERLEVELINFO_MINLEVEL] > temp)
				PlayerInformationInLevel[PLAYERLEVELINFO_MINLEVEL] = temp;
			if(PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL] < temp)
				PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL] = temp;
			++PlayerInformationInLevel[PLAYERLEVELINFO_COUNTATSTART];
		}
	}
}

void ResetHardcoreStuff(int pnum) {
	// reset player items
	ResetPlayerInventory(pnum);
	ResetPlayerCharmsUsed(pnum);
	ResetTradeViewList(pnum);
	ResetPlayerStash(pnum);
	ResetPlayerElixirBonuses(pnum);
	// reset weapon mod variable
	ResetWeaponMods(pnum);
	ResetMostRecentOrb(pnum);
	ResetOrbData(pnum);
	RecalculatePlayerLevelInfo();
	BreakTradesBetween(pnum);
	// may join later, sync everything
	if(PlayerIsSpectator(pnum)) {
		SyncAllClientsideVariables();
		SyncAllItemData(DND_SYNC_ITEMSOURCE_CHARMUSED);
		SyncAllItemData(DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		SyncAllItemData(DND_SYNC_ITEMSOURCE_STASH);
	}
}

int MapTalentToFlatBonus(int pnum, int talent, int flags) {
	switch(talent) {
		case TALENT_BULLET:
		case TALENT_MELEE:
		return GetPlayerAttributeValue(pnum, INV_FLATPHYS_DAMAGE);
		case TALENT_OCCULT:
		return GetPlayerAttributeValue(pnum, INV_FLATMAGIC_DAMAGE);
		case TALENT_EXPLOSIVE:
		return GetPlayerAttributeValue(pnum, INV_FLATEXP_DAMAGE);
		case TALENT_ENERGY:
		return GetPlayerAttributeValue(pnum, INV_FLATENERGY_DAMAGE);
		case TALENT_ELEMENTAL:
			int bonus = 0;
			if(flags & DND_WDMG_FIREDAMAGE)
				bonus += CheckActorInventory(pnum + P_TIDSTART, "IATTR_FlatFireDmg");
			else if(flags & DND_WDMG_ICEDAMAGE)
				bonus += CheckActorInventory(pnum + P_TIDSTART, "IATTR_FlatIceDmg");
			else if(flags & DND_WDMG_POISONDAMAGE)
				bonus += CheckActorInventory(pnum + P_TIDSTART, "IATTR_FlatPoisonDmg");
			else if(flags & DND_WDMG_LIGHTNINGDAMAGE)
				bonus += CheckActorInventory(pnum + P_TIDSTART, "IATTR_FlatLightningDmg");
		return bonus + GetPlayerAttributeValue(pnum, INV_FLATELEM_DAMAGE);
	}
	return 0;
}

int MapTalentToPercentBonus(int pnum, int talent) {
	switch(talent) {
		case TALENT_BULLET:
		case TALENT_MELEE:
		return GetPlayerAttributeValue(pnum, INV_PERCENTPHYS_DAMAGE);
		case TALENT_OCCULT:
		return GetPlayerAttributeValue(pnum, INV_PERCENTMAGIC_DAMAGE);
		case TALENT_EXPLOSIVE:
		return GetPlayerAttributeValue(pnum, INV_PERCENTEXP_DAMAGE);
		case TALENT_ENERGY:
		return GetPlayerAttributeValue(pnum, INV_PERCENTENERGY_DAMAGE);
		case TALENT_ELEMENTAL:
		return GetPlayerAttributeValue(pnum, INV_PERCENTELEM_DAMAGE);
	}
	return 0;
}

int GetFlatHealthDamageFactor(int factor) {
	return GetSpawnHealth() / (100 * factor);
}

int GetNonLowestTalents() {
	// set here the lowest possible talents (for quest)
	int lowest_talent_amount = TALENT_CAP;
	int lowest_talents = 0;
	for(int i = 0; i < MAX_TALENTS; ++i) {
		int talent_amount = CheckInventory(TalentNames[i]);
		if(talent_amount < lowest_talent_amount) {
			lowest_talent_amount = talent_amount;
			lowest_talents = (1 << i);
		} else if(talent_amount == lowest_talent_amount) { // There can be more than 1 lowest talent (specially if char is new or OP).
			lowest_talents |= (1 << i);
		}
	}
	return 0xFF ^ lowest_talents; // To get non-lowest talents, do a ~ (for some reason ~ bugs here, so I just used 0xFF ^ and it works).
}

#define DND_BASE_OVERLOADTICK 5
#define DND_BASE_OVERLOADTIME (105 / DND_BASE_OVERLOADTICK) // 3 seconds -- 105 / 5
int GetOverloadTime(int pnum) {
	return (DND_BASE_OVERLOADTIME + ((GetPlayerAttributeValue(pnum, INV_OVERLOAD_DURATION) * TICRATE) >> 16)) / DND_BASE_OVERLOADTICK;
}

#define DND_BASE_LIFESTEALCAP 20
int GetLifestealCap(int pnum) {
	// avoid recalculating over and over if possible
	int hp_cap = Max(CheckInventory("PlayerHealthCap"), GetSpawnHealth());
	return Clamp_Between((hp_cap * (DND_BASE_LIFESTEALCAP + GetPlayerAttributeValue(pnum, INV_LIFESTEAL_CAP))) / 100, 1, hp_cap);
}

#define DND_BASE_LIFESTEALRATE 20
int GetLifestealRate(int pnum) {
	return DND_BASE_LIFESTEALRATE * (100 - GetPlayerAttributeValue(pnum, INV_LIFESTEAL_CAP)) / 100;
}

#define DND_BASE_LIFERECOVERY 1 // 1% of healthcap
int GetLifestealLifeRecovery(int pnum) {
	// avoid recalculating over and over if possible
	int hp_cap = Max(CheckInventory("PlayerHealthCap"), GetSpawnHealth());
	hp_cap = (hp_cap * DND_BASE_LIFERECOVERY / 100);
	hp_cap = (hp_cap * (100 + GetPlayerAttributeValue(pnum, INV_LIFESTEAL_RECOVERY)) / 100);
	if(!hp_cap)
		hp_cap = 1;
	
	return hp_cap;
}

#endif
