#ifndef DND_COMMONSTAT_IN
#define DND_COMMONSTAT_IN

#include "DnD_Accessories.h"
#include "DnD_QuestDefs.h"
#include "DnD_InvAttribs.h"

enum {
	TALENT_BULLET = 0,
	TALENT_MELEE,
	TALENT_ENERGY,
	TALENT_EXPLOSIVE,
	TALENT_OCCULT,
    TALENT_ELEMENTAL
};
#define MAX_TALENTS TALENT_ELEMENTAL + 1

#include "DnD_OrbsDef.h"
#include "DnD_ElixirDef.h"
#include "DnD_CommonResearch.h"

#define DND_ELITE_BASEDROP 0.035

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
#define DND_PET_CHARISMA_FACTOR 20
#define DND_LUCK_GAIN 0.15 // 15% multiplicative luck
#define DND_TALENTCAPSULE_DROPRATE 0.01
#define DND_CHESTKEY_DROPRATE 0.015
#define DND_TALENTPOINT_MARK 4
#define DND_MUNITION_GAIN 10

#define DND_SAVAGERY_BONUS 10

#define BASE_PET_CAP 3

#define PERK_MEDICBONUS 10 // percent
#define PERK_MEDICSTOREBONUS 15
#define PERK_DEADLINESS_BONUS 0.02

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

#define DND_TALENT_BEGIN TALENT_BULLET
#define DND_TALENT_END TALENT_ELEMENTAL + 1

#define MAX_EXPRESIST_VAL 100 //Also in DnD_Inventory.h

#define DND_CLASSPERK1_LEVEL 5
#define DND_CLASSPERK2_LEVEL 25
#define DND_CLASSPERK3_LEVEL 50

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
#define MAXATTRIBUTES DND_ATTRIB_END - DND_ATTRIB_BEGIN + 1

#define DND_PERK_BEGIN STAT_SHRP
#define DND_PERK_END STAT_LUCK

typedef struct {
	int basehp;
	int maxhp;
	int level;
	int id;
	int traits;
	int traits2;
	int nameskip;
	int properties;
	bool isElite;
} mo_prop_T;

// allow a max of 8k monsters' data to be held
#define DND_MAX_MONSTERS 8000
#define DND_MAX_PETS 1600 // 25 pets per player x 64 players
mo_prop_T MonsterProperties[DND_MAX_MONSTERS];
mo_prop_T PetMonsterProperties[DND_MAX_PETS];

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
	RES_SELFEXPLOSIONRESIST = 4096
};

enum {
	// reset to 0 every 31 elements
	DND_STATBUFF_KNOCKBACKIMMUNE,
	DND_STATBUFF_DOUBLESMALLCHARM,
	DND_STATBUFF_ALWAYSCRITLIGHTNING,
	DND_STATBUFF_PELLETSINCIRCLE,
	DND_STATBUFF_FORBIDARMOR,
	DND_STATBUFF_PAINSHAREDWITHPETS,
	DND_STATBUFF_SOULWEPSFULLDAMAGE,
	DND_STATBUFF_SPELLSDOFULLDAMAGE,
	DND_STATBUFF_SLAINMONSTERSRIP
};

enum {
	SF_FREEZE = 1
};

int GetBonusPlayerSpeed(int pnum) {
	int res = GetPlayerAttributeValue(pnum, INV_SPEED_INCREASE);
	// add other stuff here
	res += Player_Orb_Data[pnum].orb_stat_bonuses.speed_bonus;
	res += Player_Elixir_Bonuses[pnum].speed_bonus;
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

int GetDexterity() {
	int res = CheckInventory("PSTAT_Dexterity") + GetPlayerAttributeValue(PlayerNumber(), INV_STAT_DEXTERITY);
	if(CheckInventory("DnD_QuestReward_TalentIncrease"))
		res = res * (100 + DND_QUEST_TALENTBONUS) / 100;
	return res;
}

int GetIntellect() {
	int res = CheckInventory("PSTAT_Intellect") + GetPlayerAttributeValue(PlayerNumber(), INV_STAT_INTELLECT);
	if(CheckInventory("DnD_QuestReward_TalentIncrease"))
		res = res * (100 + DND_QUEST_TALENTBONUS) / 100;
	return res;
}

int GetActorIntellect(int tid) {
	int res = CheckActorInventory(tid, "PSTAT_Intellect") + GetPlayerAttributeValue(tid - P_TIDSTART, INV_STAT_INTELLECT);
	if(CheckActorInventory(tid, "DnD_QuestReward_TalentIncrease"))
		res = res * (100 + DND_QUEST_TALENTBONUS) / 100;
	return res;
}

int GetStrength() {
	return CheckInventory("PSTAT_Strength") + GetPlayerAttributeValue(PlayerNumber(), INV_STAT_STRENGTH);
}

int GetBulkiness() {
	return CheckInventory("PSTAT_Bulkiness") + GetPlayerAttributeValue(PlayerNumber(), INV_STAT_BULKINESS);
}

int GetVitality() {
	return CheckInventory("PSTAT_Vitality") + GetPlayerAttributeValue(PlayerNumber(), INV_STAT_VITALITY);
}

int GetCharisma() {
	return CheckInventory("PSTAT_Charisma") + GetPlayerAttributeValue(PlayerNumber(), INV_STAT_CHARISMA);
}

int GetActorCharisma(int tid) {
	return CheckActorInventory(tid, "PSTAT_Charisma") + GetPlayerAttributeValue(tid - P_TIDSTART, INV_STAT_CHARISMA);
}

int GetPetCap(int tid) {
	return BASE_PET_CAP + GetActorCharisma(tid) / DND_PET_CHARISMA_FACTOR;
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
	int res = BIO_HP_ADD_1 * CheckInventory("Done_Body_Hp_1") + BIO_HP_ADD_2 * CheckInventory("Done_Body_Hp_2") + BIO_HP_ADD_3 * CheckInventory("Done_Body_Hp_3");
	
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

int CalculateHealthCapBonuses() {
	// consider quest bonuses
	int res = 0;
	res += CheckInventory("DnD_QuestReward_100BonusCap") * DND_QUEST_PRECIOUSLIFE_BONUS;
	res += CheckInventory("DnD_QuestReward_HealingAndCapIncrease") * DND_QUEST_SKINOTEETH_BONUS;
	// consider charms
	res += GetPlayerAttributeValue(PlayerNumber(), INV_HP_INCREASE);
	// consider orb effects
	res += Player_Orb_Data[PlayerNumber()].orb_stat_bonuses.hp_flat_bonus;
	res += Player_Elixir_Bonuses[PlayerNumber()].hp_flat_bonus;
	return res;
}

int GetSpawnHealth() {
	int res = CalculateHealthCapBonuses() + DND_BASE_HEALTH + DND_VIT_INCREASE * GetVitality();
	// consider percent bonuses from here on
	res += (res * (Player_Orb_Data[PlayerNumber()].orb_stat_bonuses.hp_percent_bonus + DND_TORRASQUE_BOOST * CheckInventory("DnD_QuestReward_TorrasqueBonus"))) / 100;
	res += (res * GetStrength() * DND_STR_CAPINCREASE) / DND_STR_CAPFACTOR;
	res += (res * CheckInventory("CelestialCheck") * CELESTIAL_BOOST) / 100;
	// research bonuses
	res += (res * GetResearchHealthBonuses()) / 100;
	// player bonus + elixir
	res += (res * (Player_Elixir_Bonuses[PlayerNumber()].hp_percent_bonus + GetPlayerAttributeValue(PlayerNumber(), INV_HPPERCENT_INCREASE))) / 100;
	if(IsAccessoryEquipped(ActivatorTID(), DND_ACCESSORY_ANGELICANKH))
		res >>= 1;
	if(res < DND_BASE_HEALTH)
		res = DND_BASE_HEALTH;
	return res;
}

// Generic Player RPG Stat restore function
void RestoreRPGStat (int statflag) {
	int pnum = PlayerNumber();

	if((statflag & RES_PERK_SHARP) && CheckInventory("Perk_Sharpshooting"))
		GiveInventory(StrParam(s:"Damage_Perk_", d:CheckInventory("Perk_Sharpshooting") * SHARPSHOOTING_DAMAGE), 1);
	if((statflag & RES_PERK_ENDURANCE) && CheckInventory("Perk_Endurance"))
		GiveInventory(StrParam(s:"Resist_Perk_", d:CheckInventory("Perk_Endurance") * ENDURANCE_RESIST), 1);
	if((statflag & RES_ACCURACY) && GetPlayerAttributeValue(pnum, INV_ACCURACY_INCREASE))
		SetActorProperty(0, APROP_ACCURACY, GetPlayerAttributeValue(pnum, INV_ACCURACY_INCREASE));
	if((statflag & RES_EXPLOSIONRADIUS) && GetPlayerAttributeValue(pnum, INV_EXPLOSION_RADIUS))
		SetActorProperty(0, APROP_SCORE, GetPlayerAttributeValue(pnum, INV_EXPLOSION_RADIUS));
	if((statflag & RES_SELFEXPLOSIONRESIST) && GetPlayerAttributeValue(pnum, INV_EXPLOSIVE_RESIST))
		GiveInventory(StrParam(s:"ExplosionResist_", d:Clamp_Between(GetPlayerAttributeValue(pnum, INV_EXPLOSIVE_RESIST), 1, MAX_EXPRESIST_VAL)), 1);
	// can only intervene once per map
	if(IsAccessoryEquipped(ActivatorTID(), DND_ACCESSORY_ANGELICANKH) && !CheckInventory("Intervened")) {
		GiveInventory("CanIntervene", 1);
		SetPlayerProperty(0, 1, PROP_BUDDHA);
	}
	if(statflag & RES_PLAYERSPEED)
		SetActorProperty(0, APROP_SPEED, GetPlayerSpeed(pnum));
	if(CheckInventory("HellfireCheck")) {
		GiveInventory("Accessory_FireProtection", 1);
		GiveInventory("Accessory_FireBuff", 1);
	}
	if(CheckInventory("ArtemisCheck"))
		GiveInventory("ArtemisPower", 1);
	if(CheckInventory("DemonBaneCheck"))
		GiveInventory("DemonBaneReduction", 1);
	if(CheckInventory("TaltosCheck"))
		GiveInventory("TaltosPower", 1);
	if(CheckInventory("HateCheck"))
		GiveInventory("PowerReflection", 1);
	if(CheckInventory("Ability_AntiPoison"))
		GiveInventory("PoisonResist", 1);
	if(CheckInventory("Ability_ExplosionMastery"))
		GiveInventory("ExplosionResistAbility", 1);
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
	HandleResearchBonuses();
	HandleClassPerks();
}

// Retrieves attributes from items that the player has on them
str GetPlayerAttributeString(int attrib) {
	return Inv_Attribute_Names[attrib][INVATTR_CHECKER];
}

int GetPlayerAttributeValue(int pnum, int attrib) {
	return CheckActorInventory(pnum + P_TIDSTART, GetPlayerAttributeString(attrib));
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
			break;
			case DND_PLAYER_MARINE:
				GiveInventory("Marine_Perk5", 1);
				TakeInventory("Doomguy_Perk5", 1);
				TakeInventory("Hobo_Perk5", 1);
				TakeInventory("Punisher_Perk5", 1);
				TakeInventory("Wanderer_Perk5", 1);
				TakeInventory("Cyborg_Perk5", 1);
			break;			
			case DND_PLAYER_HOBO:
				GiveInventory("Hobo_Perk5", 1);
				TakeInventory("Doomguy_Perk5", 1);
				TakeInventory("Marine_Perk5", 1);
				TakeInventory("Punisher_Perk5", 1);
				TakeInventory("Wanderer_Perk5", 1);
				TakeInventory("Cyborg_Perk5", 1);
			break;
			case DND_PLAYER_PUNISHER:
				GiveInventory("Punisher_Perk5", 1);
				TakeInventory("Doomguy_Perk5", 1);
				TakeInventory("Marine_Perk5", 1);
				TakeInventory("Hobo_Perk5", 1);
				TakeInventory("Wanderer_Perk5", 1);
				TakeInventory("Cyborg_Perk5", 1);
			break;
			case DND_PLAYER_WANDERER:
				GiveInventory("Wanderer_Perk5", 1);
				TakeInventory("Doomguy_Perk5", 1);
				TakeInventory("Marine_Perk5", 1);
				TakeInventory("Hobo_Perk5", 1);
				TakeInventory("Punisher_Perk5", 1);
				TakeInventory("Cyborg_Perk5", 1);
			break;
			case DND_PLAYER_CYBORG:
				GiveInventory("Cyborg_Perk5", 1);
				TakeInventory("Doomguy_Perk5", 1);
				TakeInventory("Marine_Perk5", 1);
				TakeInventory("Hobo_Perk5", 1);
				TakeInventory("Punisher_Perk5", 1);
				TakeInventory("Wanderer_Perk5", 1);
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
			break;
			case DND_PLAYER_MARINE:
				GiveInventory("Marine_Perk25", 1);
				TakeInventory("Doomguy_Perk25", 1);
				TakeInventory("Hobo_Perk25", 1);
				TakeInventory("Punisher_Perk25", 1);
				TakeInventory("Wanderer_Perk25", 1);
				TakeInventory("Cyborg_Perk25", 1);
			break;			
			case DND_PLAYER_HOBO:
				GiveInventory("Hobo_Perk25", 1);
				TakeInventory("Doomguy_Perk25", 1);
				TakeInventory("Marine_Perk25", 1);
				TakeInventory("Punisher_Perk25", 1);
				TakeInventory("Wanderer_Perk25", 1);
				TakeInventory("Cyborg_Perk25", 1);
			break;
			case DND_PLAYER_PUNISHER:
				GiveInventory("Punisher_Perk25", 1);
				TakeInventory("Doomguy_Perk25", 1);
				TakeInventory("Marine_Perk25", 1);
				TakeInventory("Hobo_Perk25", 1);
				TakeInventory("Wanderer_Perk25", 1);
				TakeInventory("Cyborg_Perk25", 1);
			break;
			case DND_PLAYER_WANDERER:
				GiveInventory("Wanderer_Perk25", 1);
				TakeInventory("Doomguy_Perk25", 1);
				TakeInventory("Marine_Perk25", 1);
				TakeInventory("Hobo_Perk25", 1);
				TakeInventory("Punisher_Perk25", 1);
				TakeInventory("Cyborg_Perk25", 1);
			break;
			case DND_PLAYER_CYBORG:
				GiveInventory("Cyborg_Perk25", 1);
				TakeInventory("Doomguy_Perk25", 1);
				TakeInventory("Marine_Perk25", 1);
				TakeInventory("Hobo_Perk25", 1);
				TakeInventory("Punisher_Perk25", 1);
				TakeInventory("Wanderer_Perk25", 1);
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
			break;
			case DND_PLAYER_MARINE:
				GiveInventory("CurseImmunity", 1);
				TakeInventory("Doomguy_Perk50", 1);
				TakeInventory("Hobo_Perk50", 1);
				TakeInventory("Punisher_Perk50", 1);
				TakeInventory("Wanderer_Perk50", 1);
				TakeInventory("Cyborg_Perk50", 1);
			break;			
			case DND_PLAYER_HOBO:
				GiveInventory("Hobo_Perk50", 1);
				TakeInventory("Doomguy_Perk50", 1);
				TakeInventory("Marine_Perk50", 1);
				TakeInventory("Punisher_Perk50", 1);
				TakeInventory("Wanderer_Perk50", 1);
				TakeInventory("Cyborg_Perk50", 1);
			break;
			case DND_PLAYER_PUNISHER:
				GiveInventory("Punisher_Perk50", 1);
				TakeInventory("Doomguy_Perk50", 1);
				TakeInventory("Marine_Perk50", 1);
				TakeInventory("Hobo_Perk50", 1);
				TakeInventory("Wanderer_Perk50", 1);
				TakeInventory("Cyborg_Perk50", 1);
			break;
			case DND_PLAYER_WANDERER:
				GiveInventory("Wanderer_Perk50", 1);
				TakeInventory("Doomguy_Perk50", 1);
				TakeInventory("Marine_Perk50", 1);
				TakeInventory("Hobo_Perk50", 1);
				TakeInventory("Punisher_Perk50", 1);
				TakeInventory("Cyborg_Perk50", 1);
			break;
			case DND_PLAYER_CYBORG:
				GiveInventory("Cyborg_Perk50", 1);
				TakeInventory("Doomguy_Perk50", 1);
				TakeInventory("Marine_Perk50", 1);
				TakeInventory("Hobo_Perk50", 1);
				TakeInventory("Punisher_Perk50", 1);
				TakeInventory("Wanderer_Perk50", 1);
			break;
		}
	}
}

#endif