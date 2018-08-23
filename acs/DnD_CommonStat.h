#ifndef DND_COMMONSTAT_IN
#define DND_COMMONSTAT_IN

#include "DnD_OrbsDef.h"
#include "DnD_Accessories.h"
#include "DnD_QuestDefs.h"

#define DND_BASE_PLAYERSPEED 1.0
#define SHARPSHOOTING_DAMAGE 5
#define ENDURANCE_RESIST 5
#define DND_BULKINESS_GAIN 0.006
#define DND_BULKINESS_GAIN_AFTER100 0.0025
#define DND_TALENT_INCREASE 5.0
#define DND_DEX_GAIN 0.1
#define DND_INT_GAIN 0.1
#define DND_VIT_INCREASE 4
#define DND_STR_GAIN 0.09
#define DND_STR_GAIN_INT 9
#define DND_STR_CAPINCREASE 1 // 1 out of 200 => 0.5 out of 100
#define DND_STR_CAPFACTOR 200
#define DND_ARMOR_PER_BUL 4
#define DND_CHR_GAIN 0.5
#define DND_LUCK_GAIN 0.15 // 15% multiplicative luck
#define DND_TALENTCAPSULE_DROPRATE 0.01
#define DND_CHESTKEY_DROPRATE 0.015
#define DND_TALENTPOINT_MARK 4
#define DND_MUNITION_GAIN 10

#define PERK_MEDICBONUS 10 // percent
#define PERK_MEDICSTOREBONUS 15
#define PERK_DEADLINESS_BONUS 0.02

#define DND_BUL_KNOCKBACK_GAIN 25
#define DND_STR_KNOCKBACK_GAIN 50
#define DND_BASE_PLAYER_MASS 100

#define DND_STAT_FULLMAX 200
#define DND_BASE_ARMOR_SHOW 100
#define DND_BASE_ARMOR 200
#define DND_BASE_ARMORCAP 300

#define STAT_BASE 0
#define DND_STAT_MAX 100 // for menu only, in game stats can go up to 200
#define DND_PERK_MAX 10

#define MAX_WEAPON_SLOTS 9

#define BIO_HP_ADD_1 5
#define BIO_HP_ADD_2 6
#define BIO_HP_ADD_3 9

#define EXO_AR_ADD_1 5
#define EXO_AR_ADD_2 6
#define EXO_AR_ADD_3 9

enum {
	TALENT_BULLET = 0,
	TALENT_MELEE,
	TALENT_OCCULT,
	TALENT_EXPLOSIVE,
	TALENT_ENERGY,
    TALENT_ELEMENTAL
};
#define MAX_TALENTS TALENT_ELEMENTAL + 1
#define DND_TALENT_BEGIN TALENT_BULLET
#define DND_TALENT_END TALENT_ELEMENTAL + 1

enum {
	STAT_STR,
	STAT_DEX,
	STAT_BUL,
	STAT_CHR,
	STAT_VIT,
	STAT_INT,
	
	STAT_SHRP,
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
#define DND_PERK_BEGIN STAT_SHRP
#define DND_PERK_END STAT_LUCK

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
	RES_PLAYERSPEED = 512
};

typedef struct {	
	int hp_flat_bonus;
	int armor_flat_bonus;
	
	int hp_percent_bonus;
	int armor_percent_bonus;
	
	int greed_percent_bonus;
	int wisdom_percent_bonus;
	
	int speed_bonus;
	int drop_chance;
	int holding;
	
	int luck;
	
	int damage_type_bonus[MAX_TALENTS];
	
	int flat_damage_bonus[MAX_TALENTS];
	int slot_damage_bonus[MAX_WEAPON_SLOTS];

	int magazine_increase;
	int pellet_increase;
	int explosion_radius;
	int explosion_resist;
	int ammo_chance;
	int ammo_gain;
	int regen_cap;
	int crit_chance;
	int crit_percent;
	int crit_damage;
	
	int knockback_resist;
	int damage_percent;
	int accuracy;
} pstat_T;

global pstat_T 16: Player_Bonuses[MAXPLAYERS];

enum {
	SF_FREEZE = 1
};

int GetBonusPlayerSpeed(int pnum) {
	int res = Player_Bonuses[pnum].speed_bonus;
	// add other stuff here
	res += Player_Orb_Data[pnum].orb_stat_bonuses.speed_bonus;
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

int GetHealingBonuses() {
	int bonus = PERK_MEDICBONUS * CheckInventory("Perk_Medic");
	if(CheckInventory("DnD_QuestReward_HealingIncrease25"))
		bonus += DND_QUEST_MASTERHEALER_INCREASE;
	if(CheckInventory("DnD_QuestReward_HealingAndCapIncrease"))
		bonus += DND_QUEST_SKINOTEETH_INCREASE;
	return bonus;
}

int GetResearchHealthBonuses() {
	return BIO_HP_ADD_1 * CheckInventory("Done_Body_Hp_1") + BIO_HP_ADD_2 * CheckInventory("Done_Body_Hp_2") + BIO_HP_ADD_3 * CheckInventory("Done_Body_Hp_3");
}

int GetResearchArmorBonuses() {
	return EXO_AR_ADD_1 * CheckInventory("Done_Body_Ar_1") + EXO_AR_ADD_2 * CheckInventory("Done_Body_Ar_2") + EXO_AR_ADD_3 * CheckInventory("Done_Body_Ar_3");
}

int GetMissingHealth() {
	return GetSpawnHealth() - GetActorProperty(0, APROP_HEALTH);
}

int CalculateHealthCapBonuses() {
	// consider quest bonuses
	int res = 0;
	res += CheckInventory("DnD_QuestReward_100BonusCap") * DND_QUEST_PRECIOUSLIFE_BONUS;
	res += CheckInventory("DnD_QuestReward_HealingAndCapIncrease") * DND_QUEST_SKINOTEETH_BONUS;
	// consider charms
	res += Player_Bonuses[PlayerNumber()].hp_flat_bonus;
	// consider orb effects
	res += Player_Orb_Data[PlayerNumber()].orb_stat_bonuses.hp_flat_bonus;
	
	return res;
}

int GetSpawnHealth() {
	int res = CalculateHealthCapBonuses() + DND_BASE_HEALTH + DND_VIT_INCREASE * CheckInventory("PSTAT_Vitality");
	// consider percent bonuses
	res += (res * (Player_Orb_Data[PlayerNumber()].orb_stat_bonuses.hp_percent_bonus) + DND_TORRASQUE_BOOST * CheckInventory("DnD_QuestReward_TorrasqueBonus")) / 100;
	res += (res * CheckInventory("PSTAT_Strength") * DND_STR_CAPINCREASE) / DND_STR_CAPFACTOR;
	res += (res * CheckInventory("CelestialCheck") * CELESTIAL_BOOST) / 100;
	// research bonuses
	res += (res * GetResearchHealthBonuses()) / 100;
	
	if(IsAccessoryEquipped(ActivatorTID(), DND_ACCESSORY_ANGELICANKH))
		res >>= 1;
	if(res < DND_BASE_HEALTH)
		res = DND_BASE_HEALTH;
	return res;
}

// Generic Player RPG Stat restore function
void RestoreRPGStat (int statflag) {
	if((statflag & RES_PERK_SHARP) && CheckInventory("Perk_Sharpshooting"))
		GiveInventory(StrParam(s:"Damage_Perk_", d:CheckInventory("Perk_Sharpshooting") * SHARPSHOOTING_DAMAGE), 1);
	if((statflag & RES_PERK_ENDURANCE) && CheckInventory("Perk_Endurance"))
		GiveInventory(StrParam(s:"Resist_Perk_", d:CheckInventory("Perk_Endurance") * ENDURANCE_RESIST), 1);
	// can only intervene once per map
	if(IsAccessoryEquipped(ActivatorTID(), DND_ACCESSORY_ANGELICANKH) && !CheckInventory("Intervened")) {
		GiveInventory("CanIntervene", 1);
		SetPlayerProperty(0, 1, PROP_BUDDHA);
	}
	if(statflag & RES_PLAYERSPEED)
		SetActorProperty(0, APROP_SPEED, GetPlayerSpeed(PlayerNumber()));
	if(CheckInventory("HellfireCheck"))
		GiveInventory("Accessory_FireProtection", 1);
	if(CheckInventory("ArtemisCheck"))
		GiveInventory("ArtemisPower", 1);
	if(CheckInventory("DemonBaneCheck"))
		GiveInventory("DemonBaneReduction", 1);
	if(CheckInventory("TaltosCheck"))
		GiveInventory("TaltosPower", 1);
	if(CheckInventory("HateCheck"))
		GiveInventory("PowerReflection", 1);
	if(CheckInventory("Ability_AntiPoison"))
		GiveInventory("PoisonImmunity", 1);
	if(CheckInventory("NetherCheck"))
		GiveInventory("NetherWeaken", 1);
	if(CheckInventory("GryphonCheck")) {
		GiveInventory("GryphonSpeed", 1);
		GiveInventory("CurseImmunity", 1);
	}
	if(CheckInventory("LichCheck"))
		GiveInventory("LichPower", 1);
	if(CheckInventory("CelestialCheck"))
		GiveInventory("CelestialSlow", 1);
	
	// So the player respawns with his actual new max hp
	SetActorProperty(0, APROP_SPAWNHEALTH, GetSpawnHealth());
}

#endif