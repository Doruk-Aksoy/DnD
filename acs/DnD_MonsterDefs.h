#ifndef DND_MONSTERDEFS_H
#define DND_MONSTERDEFS_H

#include "DnD_EliteInfo.h"

#define DND_CUSTOMMONSTER_ID 65536
#define DND_MONSTERMASS_SCALE 20 // 20% per level

#define DND_DEFAULT_MONSTERDMG_SCALING_FACTOR 120

#define DND_ELITE_DMGSCALE 13
#define DND_ELITE_EXTRASTRONG_BONUS 35

#define ETHEREAL_RESIST 33

#define MONSTER_RES_PER_THRESHOLD 30
#define MONSTER_RES_PER_PLUS40 8

#define DND_ASMODEUS_MAXLOB 3

#define DND_NOINFIGHT_THRESHOLD 300 // if there are this many or greater count of a single monster type above HellKnight, active no infight mode to help with lag

#define DND_VAAJ_CHARMTIME 105

// used to eliminate various random calls when a monster dies, to offload it to initialization phase to reuse the numbers
enum {
	DND_MON_RNG_1,
	DND_MON_RNG_2,
	DND_MON_RNG_3,
	DND_MON_RNG_4
};
#define DND_MAX_MONSTER_PRECALC_RNG 4

enum {
	DND_MONFLAG_ISELITE = 1,
	DND_MONFLAG_ISMAGIC = 2,
	DND_MONFLAG_ISIDLE = 4,
	DND_MONFLAG_HASTRAITS = 8
};

typedef struct {
	int basehp;
	int maxhp;
	int level;
	int id;
	int class;										// monster class
	int dmg_bonus;									// dmg bonus %
	int gain;										// the gain value the monster will provide on killing
	int rarity;										// monster's rarity;
	int droprate;									// droprate multiplier for this monster based on its initialization data
	int rarity_boost;								// item rarity boost from monster
	int killer_tid;									// tid of the killer
	int rng_vals[DND_MAX_MONSTER_PRECALC_RNG];		// precalculated rng outcomes
	int flags;										// isElite, isMagic, isIdle etc.
	int resists[MAX_DAMAGE_CATEGORIES];				// resists of the monster
	bool trait_list[MAX_MONSTER_TRAITS_STORED]; 	// 1 if that trait is on, 0 if not
} mo_prop_T;

// allow a max of 8192 monsters' data to be held
global mo_prop_T 25: MonsterProperties[DND_MAX_MONSTERS];
global mo_prop_T 26: PetMonsterProperties[DND_MAX_PETS];

typedef struct {
	int health;
	int flags;
	bool trait_list[MAX_MONSTER_TRAITS_STORED];
} monster_data_T;

global monster_data_T 10: MonsterData[DND_LASTMONSTER_INDEX];

enum {
	DND_BOSS_LOWTIER,
	DND_BOSS_MEDTIER,
	DND_BOSS_HIGHTIER,
	DND_BOSS_ENDGAME
};

enum {
	MONSTERCLASS_ZOMBIEMAN,
	MONSTERCLASS_SHOTGUNGUY,
	MONSTERCLASS_CHAINGUNGUY,
	MONSTERCLASS_DEMON,
	MONSTERCLASS_SPECTRE,
	MONSTERCLASS_IMP,
	MONSTERCLASS_CACODEMON,
	MONSTERCLASS_PAINELEMENTAL,
	MONSTERCLASS_LOSTSOUL,
	MONSTERCLASS_REVENANT,
	MONSTERCLASS_HELLKNIGHT,
	MONSTERCLASS_BARON,
	MONSTERCLASS_FATSO,
	MONSTERCLASS_ARACHNOTRON,
	MONSTERCLASS_ARCHVILE,
	MONSTERCLASS_SPIDERMASTERMIND,
	MONSTERCLASS_CYBERDEMON,
	MONSTERCLASS_WOLFENSS
};

// First element on each list is the "Vanilla" monster, rest follow from their variations with Var1 to VarX
#define MAX_MONSTER_CATEGORIES (MONSTERCLASS_WOLFENSS + 1)
#define MAX_MONSTER_VARIATIONS 17 // this includes vanilla
global int 54: Monster_Weights[MAX_MONSTER_CATEGORIES][MAX_MONSTER_VARIATIONS];

enum {
	// Classics
	MONSTER_ZOMBIEMAN,
	MONSTER_SHOTGUNNER,
	MONSTER_CHAINGUNNER,
	MONSTER_DEMON,
	MONSTER_SPECTRE,
	MONSTER_IMP,
	MONSTER_CACODEMON,
	MONSTER_PAINELEMENTAL,
	MONSTER_LOSTSOUL,
	MONSTER_REVENANT,
	MONSTER_HELLKNIGHT,
	MONSTER_BARON,
	MONSTER_FATSO,
	MONSTER_SPIDER,
	MONSTER_VILE,
	MONSTER_MASTERMIND,
	MONSTER_CYBERDEMON,
	MONSTER_NAZI,
	
	// DnD Exclusives
	
	// Zombieman
	MONSTER_ZOMBIEMANGRAY,
	MONSTER_ZOMBIERANGER,
	MONSTER_ZOMBIESMG,
	MONSTER_ZOMBIERAPID,
	MONSTER_ZOMBIEMARINE,
	MONSTER_ZOMBIELOS,
	MONSTER_ZOMBIEPISTOL,
	MONSTER_ZOMBIEQUAKE1,
	MONSTER_ZOMBIEHUNTER,
	MONSTER_ZOMBIEPROPHET,
	
	// Shotgunner
	MONSTER_SSGLOS,
	MONSTER_ZSPECSG,
	MONSTER_SGLOS,
	MONSTER_SAWEDOFF1,
	MONSTER_SAWEDOFF2,
	MONSTER_ROGUE,
	MONSTER_ZOMBIEQUAKE2,
	MONSTER_ZOMBIESSG,
	
	// Chaingunner
	MONSTER_ZOMBIEQUAKE3,
	MONSTER_ZOMBIEMG,
	MONSTER_INITIATE,
	MONSTER_DOUBLEGUNNER,
	MONSTER_CGMAJOR,
	MONSTER_MRROBOT,
	MONSTER_ZSEC,
	MONSTER_ZSPECMG,
	MONSTER_PLASMAZOMBIE,
	MONSTER_BERSERKERGUY,
	
	// Demon
	MONSTER_BLOODDEMON,
	MONSTER_BLOODFIEND,
	MONSTER_CYBERFIEND,
	MONSTER_FLAMEDEMON,
	MONSTER_STONEIMP,
	MONSTER_SCAVENGER,
	MONSTER_SOULEATER,
	MONSTER_NHUMCIGN,
	MONSTER_STONEDEMON,
	MONSTER_BRUTY,
	MONSTER_SATYR,
	MONSTER_EARTHGOLEM,
	MONSTER_RAVAGER,
	MONSTER_PUREBREDDEMON,
	MONSTER_SABRECLAW,
	
	// Spectre
	MONSTER_LURKER,
	MONSTER_GRAVEDIGGER,
	MONSTER_DEVOURER,
	MONSTER_NIGHTMAREDEMON,
	
	// Imp
	MONSTER_DARKIMP1,
	MONSTER_VOIDDARKIMP,
	MONSTER_NETHERDARKIMP,
	MONSTER_DARKIMP2,
	MONSTER_IMPABOM,
	MONSTER_STIMP,
	MONSTER_VOIDIMP,
	MONSTER_SOULHARVESTER,
	MONSTER_PYROIMP,
	MONSTER_DEVIL,
	MONSTER_DEVIL2,
	MONSTER_VULGAR,
	MONSTER_UNDEADMAGE,
	MONSTER_SHADOW,
	MONSTER_REAVER,
	MONSTER_ROACH,
	
	// Lost Soul
	MONSTER_BABYCACO,
	MONSTER_ETHEREALSOUL,
	MONSTER_FLESHSPAWN,
	MONSTER_GUARDIANCUBE,
	MONSTER_FORGOTTENONE,
	MONSTER_HADESSPHERE,
	MONSTER_WATCHER,
	MONSTER_DARKLICH_SPIRIT,
	
	// Cacodemon
	MONSTER_WEAKENER,
	MONSTER_SHADOWPRIEST,
	MONSTER_GRELL,
	MONSTER_DEATHWHISPERER,
	MONSTER_CACOLICH,
	MONSTER_INFERNO,
	MONSTER_GUARDIAN,
	MONSTER_ENHANCEDCACO,
	MONSTER_EARTHLICH,
	MONSTER_WICKED,
	
	// Pain Elemental
	MONSTER_BLOODLICH,
	MONSTER_HADESELEMENTAL,
	MONSTER_HELLARBITER,
	MONSTER_DEFILER,
	MONSTER_TORTUREDSOUL,
	MONSTER_SHADOWDISCIPLE,
	MONSTER_SENTINEL,
	MONSTER_PHANTASM,
	MONSTER_WRAITH, // add new ones above wraith
	
	// Revenant
	MONSTER_INCARNATE,
	MONSTER_BEAMREVENANT,
	MONSTER_AXEKNIGHT,
	MONSTER_WIDOWMAKER,
	MONSTER_SLUDGEGIANT,
	MONSTER_SLUDGEGIANT2,
	MONSTER_SLUDGEGIANT3,
	MONSTER_CADAVER,
	MONSTER_DARKSERVANT,
	MONSTER_CRAWLER,
	MONSTER_CYBORGSOLDIER,
	MONSTER_DRAUGR,

	// Hell Knight
	MONSTER_BLOODSATYR,
	MONSTER_HELLWARRIOR,
	MONSTER_HELLSFURY,
	MONSTER_BLACKKNIGHT,
	MONSTER_ARCHON,
	MONSTER_WARLORD,
	MONSTER_SKULLWIZARD,
	MONSTER_CYBORGWARRIOR,
	MONSTER_SHADOWBEAST,
	MONSTER_CHAOSSERPENT,
	MONSTER_MOONSATYR,
	MONSTER_ICEGOLEM,
	MONSTER_PUTREFIER,
	MONSTER_GLADIATOR,

	// Baron
	MONSTER_LAVADEMON,
	MONSTER_LORDOFHERESY,
	MONSTER_BORMERETH,
	MONSTER_BARBATOS,
	MONSTER_BLOODSEEKER,
	MONSTER_SHADOWWIZARD,
	MONSTER_KJAROCH,
	MONSTER_CYBRUISER,
	MONSTER_BRUISERDEMON,
	MONSTER_MAGMASERPENT,
	MONSTER_DREADKNIGHT,
	MONSTER_MAGMAGOLEM,
	MONSTER_JUDICATOR,
	MONSTER_WARMASTER,
	
	// Fatso
	MONSTER_CORPULENT,
	MONSTER_DAEDABUS,
	MONSTER_PALADIN,
	MONSTER_GAMON,
	MONSTER_MEPHISTO,
	MONSTER_MAFIBUS,
	MONSTER_ICEFATSO,
	MONSTER_ABOMINATION,
	MONSTER_REDEEMER,
	MONSTER_GOLDGOLEM,
	MONSTER_YETI,
	
	// Arachnotron
	MONSTER_FUSIONSPIDER,
	MONSTER_RAILARACHNOTRON,
	MONSTER_HELLFORGESPIDER,
	MONSTER_VORE,
	MONSTER_BABYDEMOLISHER,
	MONSTER_CHAINGUNGENERAL,
	MONSTER_CHAINGUNCOMMANDO,
	MONSTER_LEGIONNAIRE,
	MONSTER_MANTICORE,
	MONSTER_OPHIDIAN,
	
	// ArchVile
	MONSTER_DIABLOIST,
	MONSTER_UNDEADPRIEST,
	MONSTER_UNDEADPRIESTGHOST,
	MONSTER_DEATHVILE,
	MONSTER_HIEROPHANT,
	MONSTER_GURU,
	MONSTER_DEATHKNIGHT,
	MONSTER_HORSHACKER,
	MONSTER_DARKZEALOT,
	MONSTER_FLESHWIZARD,
	
	// Spider Mastermind
	MONSTER_DEMOLISHER,
	MONSTER_ARACHNOPHYTE,
	MONSTER_PSIONICQUEEN,
	MONSTER_ANGELOFDEATH,
	MONSTER_GOLDLICH,
	MONSTER_GOLDLICHFAKE,
	MONSTER_IRONLICH,
	MONSTER_SPIDEROVERLORD,
	MONSTER_DARKLICH,
	
	// Cyberdemon
	MONSTER_CARDINAL,
	MONSTER_TERMINATOR,
	MONSTER_THAMUZ,
	MONSTER_AZAZEL,
	MONSTER_HELLSMITH,
	MONSTER_THANATOS,
	MONSTER_AVATAR,
	MONSTER_CERBERUS,
	MONSTER_DEATHWYVERN,
	
	MONSTER_DREAMINGGOD,
	MONSTER_TORRASQUE,
	MONSTER_MORDECQAI,
	MONSTER_GODSLAYER,
	MONSTER_GOLGOTH,
	
	// zombieman uniques
	MONSTER_TERON,
	// shotgunner uniques
	MONSTER_GANT,
	// chaingunnner uniques
	MONSTER_BRONN,
	// imp uniques
	MONSTER_VAAJ,
	// demon uniques
	MONSTER_REMUS,
	// spectre uniques
	MONSTER_SSRATH,
	// lost soul uniques
	MONSTER_HOLLOWSHELL,
	// cacodemon uniques
	MONSTER_OMNISIGHT,
	// pain e. uniques
	MONSTER_CHEGOVAX,
	// rev uniques
	MONSTER_ONIMUZ,
	// hk uniques
	MONSTER_HARKIMONDE,
	// baron uniques
	MONSTER_LESHRAC,
	MONSTER_LESHRACPOD,
	// manc uniques
	MONSTER_KRULL,
	// arachno uniques
	MONSTER_THORAX,
	// vile uniques
	MONSTER_ZRAVOG,
	// sm uniques
	MONSTER_ERYXIA,
	// cyber uniques
	MONSTER_ABAXOTH,

	// incursion monsters
	// chex
	MONSTER_CHEX_COMMON,
	MONSTER_CHEX_BIPEDAL,
	MONSTER_CHEX_ARMORED,
	MONSTER_CHEX_HEAVYARMORED,
	MONSTER_CHEX_CYCLOPS,
	MONSTER_CHEX_FLEMMINE,
	MONSTER_CHEX_LARVA,
	MONSTER_CHEX_STRIDICUS,
	MONSTER_CHEX_QUADWUMPUS,
	MONSTER_CHEX_SUPERCYCLOPS,
	MONSTER_CHEX_MAXIMUS,
	MONSTER_CHEX_FLEMOBIUS,
	MONSTER_CHEX_BERNABE,
    MONSTER_CHEX_METEOR,
	MONSTER_CHEX_FLEMCARRIER,
	MONSTER_CHEX_FLEMBOMINATION,
	MONSTER_CHEX_SNOTFOLUS,

	MONSTER_ABYSS_ZOMBIEMAN,
	MONSTER_ABYSS_SHOTGUNNER,
	MONSTER_ABYSS_CHAINGUNNER,
	MONSTER_ABYSS_IMP,
	MONSTER_ABYSS_NIGHTMAREIMP,
	MONSTER_ABYSS_DEMON,
	MONSTER_ABYSS_SPECTRE,
	MONSTER_ABYSS_HOUND,
	MONSTER_ABYSS_LOSTSOUL,
	MONSTER_ABYSS_CACODEMON,
	MONSTER_ABYSS_NIGHTMARECACO,
	MONSTER_ABYSS_PAINELEMENTAL,
	MONSTER_ABYSS_HELLKNIGHT,
	MONSTER_ABYSS_BARON,
	MONSTER_ABYSS_REVENANT,
	MONSTER_ABYSS_MANCUBUS,
	MONSTER_ABYSS_ARACHNOTRON,
	MONSTER_ABYSS_ARCHVILE,
	MONSTER_ABYSS_SPIDERDEMON,
	MONSTER_ABYSS_CYBERDEMON,
	MONSTER_ABYSS_MOTHERDEMON,

	DND_LASTMONSTER_INDEX,
	
	MONSTER_CUSTOM,
	MONSTER_CUSTOM_BOSS,
	MONSTER_CUSTOM_UNIQUEBOSS
};

#define DND_CUSTOM_ZOMBIEMAN_BEGIN MONSTER_ZOMBIEMANGRAY
#define DND_CUSTOM_ZOMBIEMAN_END MONSTER_ZOMBIEPROPHET

#define DND_CUSTOM_SHOTGUNGUY_BEGIN MONSTER_SSGLOS
#define DND_CUSTOM_SHOTGUNGUY_END MONSTER_ZOMBIESSG

#define DND_CUSTOM_CHAINGUNGUY_BEGIN MONSTER_ZOMBIEQUAKE3
#define DND_CUSTOM_CHAINGUNGUY_END MONSTER_BERSERKERGUY

#define DND_CUSTOM_IMP_BEGIN MONSTER_DARKIMP1
#define DND_CUSTOM_IMP_END MONSTER_ROACH

#define DND_CUSTOM_DEMON_BEGIN MONSTER_BLOODDEMON
#define DND_CUSTOM_DEMON_END MONSTER_SABRECLAW

#define DND_CUSTOM_SPECTRE_BEGIN MONSTER_LURKER
#define DND_CUSTOM_SPECTRE_END MONSTER_NIGHTMAREDEMON

#define DND_CUSTOM_LOSTSOUL_BEGIN MONSTER_BABYCACO
#define DND_CUSTOM_LOSTSOUL_END MONSTER_DARKLICH_SPIRIT

#define DND_CUSTOM_CACODEMON_BEGIN MONSTER_WEAKENER
#define DND_CUSTOM_CACODEMON_END MONSTER_WICKED

#define DND_CUSTOM_PAINE_BEGIN MONSTER_BLOODLICH
#define DND_CUSTOM_PAINE_END MONSTER_WRAITH

#define DND_CUSTOM_REVENANT_BEGIN MONSTER_INCARNATE
#define DND_CUSTOM_REVENANT_END MONSTER_DRAUGR

#define DND_CUSTOM_HK_BEGIN MONSTER_BLOODSATYR
#define DND_CUSTOM_HK_END MONSTER_GLADIATOR

#define DND_CUSTOM_BARON_BEGIN MONSTER_LAVADEMON
#define DND_CUSTOM_BARON_END MONSTER_WARMASTER

#define DND_CUSTOM_FATSO_BEGIN MONSTER_CORPULENT
#define DND_CUSTOM_FATSO_END MONSTER_YETI

#define DND_CUSTOM_ARACHNO_BEGIN MONSTER_FUSIONSPIDER
#define DND_CUSTOM_ARACHNO_END MONSTER_OPHIDIAN

#define DND_CUSTOM_VILE_BEGIN MONSTER_DIABLOIST
#define DND_CUSTOM_VILE_END MONSTER_FLESHWIZARD

#define DND_CUSTOM_SM_BEGIN MONSTER_DEMOLISHER
#define DND_CUSTOM_SM_END MONSTER_DARKLICH

#define DND_CUSTOM_CYBER_BEGIN MONSTER_CARDINAL
#define DND_CUSTOM_CYBER_END MONSTER_DEATHWYVERN

#define DND_UNIQUE_ZOMBIEMAN_BEGIN MONSTER_TERON
#define DND_UNIQUE_ZOMBIEMAN_END MONSTER_TERON

#define DND_UNIQUE_SHOTGUNGUY_BEGIN MONSTER_GANT
#define DND_UNIQUE_SHOTGUNGUY_END MONSTER_GANT

#define DND_UNIQUE_CHAINGUNGUY_BEGIN MONSTER_BRONN
#define DND_UNIQUE_CHAINGUNGUY_END MONSTER_BRONN

#define DND_UNIQUE_IMP_BEGIN MONSTER_VAAJ
#define DND_UNIQUE_IMP_END MONSTER_VAAJ

#define DND_UNIQUE_DEMON_BEGIN MONSTER_REMUS
#define DND_UNIQUE_DEMON_END MONSTER_REMUS

#define DND_UNIQUE_SPECTRE_BEGIN MONSTER_SSRATH
#define DND_UNIQUE_SPECTRE_END MONSTER_SSRATH

#define DND_UNIQUE_LOSTSOUL_BEGIN MONSTER_HOLLOWSHELL
#define DND_UNIQUE_LOSTSOUL_END MONSTER_HOLLOWSHELL

#define DND_UNIQUE_CACODEMON_BEGIN MONSTER_OMNISIGHT
#define DND_UNIQUE_CACODEMON_END MONSTER_OMNISIGHT

#define DND_UNIQUE_PAINE_BEGIN MONSTER_CHEGOVAX
#define DND_UNIQUE_PAINE_END MONSTER_CHEGOVAX

#define DND_UNIQUE_REVENANT_BEGIN MONSTER_ONIMUZ
#define DND_UNIQUE_REVENANT_END MONSTER_ONIMUZ

#define DND_UNIQUE_HK_BEGIN MONSTER_HARKIMONDE
#define DND_UNIQUE_HK_END MONSTER_HARKIMONDE

#define DND_UNIQUE_BARON_BEGIN MONSTER_LESHRAC
#define DND_UNIQUE_BARON_END MONSTER_LESHRAC

#define DND_UNIQUE_FATSO_BEGIN MONSTER_KRULL
#define DND_UNIQUE_FATSO_END MONSTER_KRULL

#define DND_UNIQUE_ARACHNO_BEGIN MONSTER_THORAX
#define DND_UNIQUE_ARACHNO_END MONSTER_THORAX

#define DND_UNIQUE_VILE_BEGIN MONSTER_ZRAVOG
#define DND_UNIQUE_VILE_END MONSTER_ZRAVOG

#define DND_UNIQUE_SM_BEGIN MONSTER_ERYXIA
#define DND_UNIQUE_SM_END MONSTER_ERYXIA

#define DND_UNIQUE_CYBER_BEGIN MONSTER_ABAXOTH
#define DND_UNIQUE_CYBER_END MONSTER_ABAXOTH

#define DND_UNIQUEMONSTER_BEGIN MONSTER_TERON
#define DND_UNIQUEMONSTER_END MONSTER_ABAXOTH
#define DND_UNIQUEBOSS_BEGIN MONSTER_ERYXIA

#define DND_FIRST_INCURSIONMONSTER MONSTER_CHEX_COMMON
#define DND_LAST_INCURSIONMONSTER MONSTER_ABYSS_MOTHERDEMON

bool IsIncursionMonster(int m_id) {
	return MonsterProperties[m_id].id >= DND_FIRST_INCURSIONMONSTER && MonsterProperties[m_id].id <= DND_LAST_INCURSIONMONSTER;
}

#define DND_BOSS_BEGIN MONSTER_DEMOLISHER
#define LEGENDARY_START MONSTER_DREAMINGGOD
#define LEGENDARY_END MONSTER_GOLGOTH
#define DND_MAX_LEGENDARY (LEGENDARY_END - LEGENDARY_START + 1)
#define MONSTER_COUNT (DND_LASTMONSTER_INDEX + 50) // possible compatibility for other wads' monsters

enum {
	DND_MONSTERSIZE_SMALL,
	DND_MONSTERSIZE_MEDIUM,
	DND_MONSTERSIZE_BIG
};

/*
	Monster rarities formulated:
		- Common: These are vanilla tier monsters. Either vanilla monsters, or monsters with similar behavior reside here.
		- Uncommon: These are a monsters that are just a tad bit stronger than vanilla.
		- Rare - 1: These guys are strong. You don't want to encounter them early on.
		- Rare - 2: A variant of rare, a little more rare.
		- Very Rare: These guys... You'd really not want to be against these but they are still better than seeing a Legendary monster.
		- Epic: Truly dangerous. High threat monsters.
*/

enum {
	DND_MWEIGHT_COMMON = 1000,
	DND_MWEIGHT_UNCOMMON = 750,
	DND_MWEIGHT_RARE1 = 600,
	DND_MWEIGHT_RARE2 = 500,
	DND_MWEIGHT_VERYRARE = 325,
	DND_MWEIGHT_EPIC = 275,
	DND_MWEIGHT_ENDMARKER = -1
};

enum {
	MONSTER_PET_ZOMBIE
};
#define MAX_PET_TYPES MONSTER_PET_ZOMBIE + 1

enum {
	DND_ZOMBIEMANID,
	DND_SHOTGUNNERID,
	DND_CHAINGUNGUYID,
	DND_DEMONID,
	DND_SPECTREID,
	DND_IMPID,
	DND_CACODEMONID,
	DND_PAINELEMENTALID,
	DND_LOSTSOULID,
	DND_REVENANTID,
	DND_HELLKNIGHTID,
	DND_BARONID,
	DND_FATSOID,
	DND_ARACHNOTRONID,
	DND_ARCHVILEID,
	DND_SPIDERMASTERMINDID,
	DND_CYBERDEMONID,
	DND_NAZIID
};

enum {
	DND_MTYPE_DEMON_POW 				= 			0b1,
	DND_MTYPE_UNDEAD_POW 				= 			0b10,
	DND_MTYPE_MAGICAL_POW 				= 			0b100,
	DND_MTYPE_ROBOTIC_POW 				= 			0b1000,
	DND_MTYPE_ZOMBIE_POW 				= 			0b10000,
	DND_MTYPE_HUMAN_POW					=			0b100000,
	DND_MTYPE_FLEMOID_POW				=			0b1000000,
};

enum {
	DND_MTYPE_DEMON,
	DND_MTYPE_UNDEAD,
	DND_MTYPE_MAGICAL,
	DND_MTYPE_ROBOTIC,
	DND_MTYPE_HUMAN,
	DND_MTYPE_FLEMOID
};

// first 5 above dictate the basic monster types
#define MAX_MONSTER_TYPES 6
str GetMonsterTypeIcon(int type) {
	switch(type) {
		case DND_MTYPE_DEMON:
		return "DNDEDEM";
		case DND_MTYPE_UNDEAD:
		return "DNDEUND";
		case DND_MTYPE_MAGICAL:
		return "DNDEMAG";
		case DND_MTYPE_ROBOTIC:
		return "DNDEROB";
		case DND_MTYPE_HUMAN:
		return "DNDEHUM";
		case DND_MTYPE_FLEMOID:
		return "DNDESLM";
	}
	return "DNDEDEM";
}

int InferMonsterPower(int mid) {
	if(mid & DND_MTYPE_ROBOTIC_POW) {
		// give priority to demon on half demon half robots
		if(mid & DND_MTYPE_DEMON_POW)
			return DND_MTYPE_DEMON;
		return DND_MTYPE_ROBOTIC;
	}
	if(mid & DND_MTYPE_MAGICAL_POW)
		return DND_MTYPE_MAGICAL;
	if(mid & DND_MTYPE_UNDEAD_POW)
		return DND_MTYPE_UNDEAD;
	if(mid & DND_MTYPE_HUMAN_POW)
		return DND_MTYPE_HUMAN;
	if(mid & DND_MTYPE_FLEMOID_POW)
		return DND_MTYPE_FLEMOID;
	return DND_MTYPE_DEMON;
}

int GetMonsterType(int monsterID, int monster_tid) {
	if(monsterID < MONSTER_CUSTOM)
		return InferMonsterPower(MonsterData[monsterID].flags);
	return InferMonsterPower(CheckActorInventory(monster_tid, "MonsterTypeToken"));
}

int MonsterPetTypeList[MAX_PET_TYPES] = {
	DND_MTYPE_UNDEAD_POW
};

bool IsMonster(int tid) {
	return tid >= DND_MONSTERTID_BEGIN && tid < P_TIDSTART;
}

bool IsBoss() {
	int id = MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].id;
	return IsMonsterIdBoss(id);
}

bool IsBossTID(int tid) {
	int id = MonsterProperties[tid - DND_MONSTERTID_BEGIN].id;
	return IsMonsterIdBoss(id);
}

bool IsMonsterIdBoss(int id) {
	return id == MONSTER_MASTERMIND || id == MONSTER_CYBERDEMON || id == MONSTER_CUSTOM_BOSS || isUniqueBossMonster_Id(id) || (id >= DND_BOSS_BEGIN && id < DND_UNIQUEMONSTER_BEGIN) || (id >= DND_UNIQUEBOSS_BEGIN && id < DND_UNIQUEMONSTER_END);
}

bool IsUniqueMonster(int id) {
	return id >= DND_UNIQUEMONSTER_BEGIN && id <= DND_UNIQUEMONSTER_END;
}

bool isLegendaryMonster(int id) {
	return id >= LEGENDARY_START && id <= LEGENDARY_END;
}

// to be specifically used as sole boss encounters, not randomly spawning boss monster to replace another enemy -- these have their own hp bar code
// right now only the custom unique boss type exists, in the future we can allocate other monster ids here
bool isUniqueBossMonster(int m_id) {
	return MonsterProperties[m_id].id == MONSTER_CUSTOM_UNIQUEBOSS;
}

// used for when we have access to raw id value from clientside -- clients normally dont know the values of these
bool isUniqueBossMonster_Id(int id) {
	return id == MONSTER_CUSTOM_UNIQUEBOSS;
}

bool IsDemon() {
	return GetMonsterType(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].id, ActivatorTID()) == DND_MTYPE_DEMON;
}

bool IsMonsterIdDemon(int m_id) {
	return GetMonsterType(MonsterProperties[m_id].id, m_id + DND_MONSTERTID_BEGIN) == DND_MTYPE_DEMON;
}

bool IsZombie() {
	int monsterID = MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].id;
	if(monsterID < MONSTER_CUSTOM)
		return MonsterData[monsterID].flags & DND_MTYPE_ZOMBIE_POW;
	return CheckActorInventory(ActivatorTID(), "MonsterTypeToken") & DND_MTYPE_ZOMBIE_POW;
}

bool IsMonsterIdZombie(int m_id) {
	int monsterID = MonsterProperties[m_id].id;
	if(monsterID < MONSTER_CUSTOM)
		return MonsterData[monsterID].flags & DND_MTYPE_ZOMBIE_POW;
	return CheckActorInventory(m_id + DND_MONSTERTID_BEGIN, "MonsterTypeToken") & DND_MTYPE_ZOMBIE_POW;
}

bool IsUndead() {
	int monsterID = MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].id;
	if(monsterID < MONSTER_CUSTOM)
		return MonsterData[monsterID].flags & DND_MTYPE_UNDEAD_POW;
	return CheckActorInventory(ActivatorTID(), "MonsterTypeToken") & DND_MTYPE_UNDEAD_POW;
}

bool IsMagic() {
	int monsterID = MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].id;
	if(monsterID < MONSTER_CUSTOM)
		return MonsterData[monsterID].flags & DND_MTYPE_MAGICAL_POW;
	return CheckActorInventory(ActivatorTID(), "MonsterTypeToken") & DND_MTYPE_MAGICAL_POW;
}

bool IsActorMagic(int tid) {
	int monsterID = MonsterProperties[tid - DND_MONSTERTID_BEGIN].id;
	if(monsterID < MONSTER_CUSTOM)
		return MonsterData[monsterID].flags & DND_MTYPE_MAGICAL_POW;
	return CheckActorInventory(tid, "MonsterTypeToken") & DND_MTYPE_MAGICAL_POW;
}

bool IsMagicOrUndead() {
	int monsterID = MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].id;
	if(monsterID < MONSTER_CUSTOM)
		return MonsterData[monsterID].flags & (DND_MTYPE_MAGICAL_POW | DND_MTYPE_UNDEAD_POW);
	return CheckActorInventory(ActivatorTID(), "MonsterTypeToken") & (DND_MTYPE_MAGICAL_POW | DND_MTYPE_UNDEAD_POW);
}

bool IsActorMagicOrUndead(int i) {
	int monsterID = MonsterProperties[i - DND_MONSTERTID_BEGIN].id;
	if(monsterID < MONSTER_CUSTOM)
		return MonsterData[monsterID].flags & (DND_MTYPE_MAGICAL_POW | DND_MTYPE_UNDEAD_POW);
	return CheckActorInventory(i, "MonsterTypeToken") & (DND_MTYPE_MAGICAL_POW | DND_MTYPE_UNDEAD_POW);
}

bool IsRobotic() {
	int monsterID = MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].id;
	if(monsterID < MONSTER_CUSTOM)
		return MonsterData[monsterID].flags & DND_MTYPE_ROBOTIC_POW;
	return CheckActorInventory(ActivatorTID(), "MonsterTypeToken") & DND_MTYPE_ROBOTIC_POW;
}

bool IsActorRobotic(int tid) {
	int monsterID = MonsterProperties[tid - DND_MONSTERTID_BEGIN].id;
	if(monsterID < MONSTER_CUSTOM)
		return MonsterData[monsterID].flags & DND_MTYPE_ROBOTIC_POW;
	return CheckActorInventory(tid, "MonsterTypeToken") & DND_MTYPE_ROBOTIC_POW;
}

bool IsActorFullRobotic(int tid) {
	int monsterID = MonsterProperties[tid - DND_MONSTERTID_BEGIN].id;
	if(monsterID < MONSTER_CUSTOM)
		return MonsterData[monsterID].flags == DND_MTYPE_ROBOTIC_POW;
	return CheckActorInventory(tid, "MonsterTypeToken") == DND_MTYPE_ROBOTIC_POW;
}

bool IsLostSoul() {
	int id = MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].id;
	return (id >= MONSTER_BABYCACO && id <= MONSTER_DARKLICH_SPIRIT) || id == MONSTER_LOSTSOUL;
}

bool isPet(int tid) {
	return (tid >= DND_PETTID_BEGIN && tid < DND_PETTID_END);
}

bool IsMonsterCategoryResurrectable(int id) {
	return MonsterProperties[id].class != MONSTERCLASS_ARCHVILE && !IsMonsterIdBoss(id);
}

// note: old formula was multiplicative and multiplied by 3 at level 50 onwards and by 9 from 75 onwards. So according to it, at level 100 a monster would have 3600% increased hp (400% from level, x9 from threshold)
// so our new formula will acommodate for this --- multiplied x^2 factor by 10 it seems to be good
int GetMonsterHPScaling(int m_id, int level) {
	// new formula: 0.42x^2 + x, where x is level - 1.
	// else 0.38x^2 + 2x, which allows for much slower scaling at <25 and sharper from 25 onwards
	int res = 0;
	if(level < 25)
		res = 21 * level * level / 50 + level;
	else
		res = (38 * level * level) / 100 + 2 * level;
	
	// big bosses have higher scaling than other monsters -- since we reach much higher values than before I decided to go ahead and reduce the big boss scaling here
	if(IsUniqueBossMonster(m_id))
		res *= 1 + (level / 33);
		
	return res;
}

int GetEliteBonusDamage(int m_id) {
	// at level 100 this yields 125%, at level 0 13% and at level 50 44%
	int lvl = MonsterProperties[m_id].level;
	return DND_ELITE_DMGSCALE + lvl * lvl / 100 + (lvl * 3) / 25;
}

int GetMonsterDMGScaling(int m_id, int level, bool forShow = false, int scaling_factor = 0, int scaling_ramp = 0) {
	// over the old formula of 4x, this provides 500% damage at lvl 100 instead of 400%
	// edit: iterating over the improvement on previous versions, making the game harder -- divisor was 25 instead of 10 -- 10 was too high, trying 20
	//int res = level * level / 20 + level;

	// new formula for lower values till 67 then higher -- 0.0675x^{2}-0.2x+5
	int res = level * level * 27 / 400 - level / 5 + 5;

	// maybe add higher scaling factor to certain enemies in the future here
	if(!scaling_ramp)
		scaling_ramp = GetActorProperty(m_id + DND_MONSTERTID_BEGIN, APROP_ACCURACY);

	// scaling factor contribution
	if(scaling_ramp) {
		// assign default scaling factor for melee things... monster's accuracy can exist, but not the melee hit's
		// in those cases the factor and ramp can be the same value too, prevent that
		// it will also assign a default of 1.2 for projectiles that dont have one when the monster has a ramp value attached
		if(!scaling_factor || scaling_factor == scaling_ramp)
			scaling_factor = DND_DEFAULT_MONSTERDMG_SCALING_FACTOR;
		if(level < scaling_ramp)
			scaling_factor = scaling_factor * level / scaling_ramp;
		res = res * (100 + scaling_factor) / 100;
	}

	// just for display, we don't need to use m_id for it
	if(forShow)
		return res;

	if(MonsterProperties[m_id].trait_list[DND_EXTRASTRONG])
		res = res * (100 + DND_ELITE_EXTRASTRONG_BONUS) / 100;

	// elite damage bonus is multiplicative
	if(MonsterProperties[m_id].flags & DND_MONFLAG_ISELITE)
		res = res * (100 + GetEliteBonusDamage(m_id)) / 100;
	else if(MonsterProperties[m_id].flags & DND_MONFLAG_ISMAGIC) // half of elite
		res = res * (100 + GetEliteBonusDamage(m_id)) / 200;

	// chaos mark is multiplicative
	if(MonsterProperties[m_id].trait_list[DND_MARKOFCHAOS])
		res = res * (100 + CHAOSMARK_DAMAGEBUFF) / 100;
	else if(MonsterProperties[m_id].trait_list[DND_MARKOFASMODEUS])
		res = res * (100 + ASMODEUSMARK_DAMAGEBUFF) / 100;
	
	// unique bosses have additional damage multiplier per level -- x^2 * 0.01667 + x
	if(IsUniqueBossMonster(m_id))
		res = res * (100 + (level * level) / 60 + level) / 100;
	
	return res;
}

void CheckDoomguyExecuteReversal(int this) {
	if(CheckActorInventory(this, "Doomguy_ChangedColor")) {
		TakeActorInventory(this, "Doomguy_ChangedColor", 1);
		TakeActorInventory(this, "Doomguy_ValidExecute", 1);
		ACS_NamedExecuteWithResult("DnD Doomguy Execute Translation", this, 1, MonsterProperties[this - DND_MONSTERTID_BEGIN].id);
	}
}

#endif