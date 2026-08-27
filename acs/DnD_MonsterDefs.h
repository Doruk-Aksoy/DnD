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

enum {
	DND_MON_PRESET_TOUGH = 1,
	DND_MON_PRESET_ELITE
};

// used to eliminate various random calls when a monster dies, to offload it to initialization phase to reuse the numbers
enum {
	DND_MON_RNG_1,
	DND_MON_RNG_2,
	DND_MON_RNG_3,
	DND_MON_RNG_4,
	DND_MON_RNG_5,
	DND_MON_RNG_6,
	DND_MON_RNG_7,
	DND_MON_RNG_8,

	DND_MAX_MONSTER_PRECALC_RNG
};
#define LAST_MON_RNG_INDEX DND_MON_RNG_8

enum {
	DND_MONFLAG_ISELITE = 1,
	DND_MONFLAG_ISMAGIC = 2,
	DND_MONFLAG_ISIDLE = 4,
	DND_MONFLAG_HASTRAITS = 8,
	DND_MONFLAG_ISDUNGEONBOSS = 16
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
	int trait_bits[MONSTER_TRAIT_WORDS];			// one bit per trait, see HasMonsterTrait below
} mo_prop_T;

// allow a max of 8192 monsters' data to be held
global mo_prop_T 25: MonsterProperties[DND_MAX_MONSTERS];
global mo_prop_T 26: PetMonsterProperties[DND_MAX_PETS];

typedef struct {
	int health;
	int flags; // monster type flags
	int rarity;
	int trait_bits[MONSTER_TRAIT_WORDS];
} monster_data_T;

global monster_data_T 10: MonsterData[DND_LASTMONSTER_INDEX];

// ============================================================================
//  TRAIT STORAGE
//
//  One bit per trait rather than one int per trait. The old bool array cost MAX_MONSTER_TRAITS_STORED
//  ints per monster -- 99 x 12800 is 1.27 MILLION ints for MonsterProperties alone, and the same
//  field again on the pet and per-monster-type tables. Packed it is four words, so about 51k, and a
//  monster's entire trait set copies or clears in four assignments instead of ninety-nine.
//
//  That is also what makes the client sync cheap: it moves whole words instead of looping the enum,
//  and the enum can grow to 128 traits before the word count moves at all.
//
//  Nothing outside these accessors should touch trait_bits. t >> 5 is the word, t & 31 the bit.
//
//  The `!!` on every read is LOAD BEARING, not decoration. Callers multiply a trait by a factor --
//  the resist block does `HasMonsterTrait(m_id, DND_BULLET_RESIST) * DND_RESIST_FACTOR` and there
//  are a dozen more like it. Returning the masked word instead of a normalized 0/1 would hand them
//  the bit's VALUE: 64 for trait 6, and 2^30 for a trait high in its word, which overflows outright.
//  The old bool array happened to be 0/1 by construction; here it has to be forced.
// ============================================================================
bool HasMonsterTrait(int m_id, int t) {
	return !!(MonsterProperties[m_id].trait_bits[t >> 5] & (1 << (t & 31)));
}

void SetMonsterTrait(int m_id, int t, bool v) {
	if(v)
		MonsterProperties[m_id].trait_bits[t >> 5] |= 1 << (t & 31);
	else
		MonsterProperties[m_id].trait_bits[t >> 5] &= ~(1 << (t & 31));
}

// sets when true, leaves alone when false -- the |= form, for folding in a condition
void AddMonsterTrait(int m_id, int t, bool v) {
	if(v)
		MonsterProperties[m_id].trait_bits[t >> 5] |= 1 << (t & 31);
}

void ClearMonsterTraits(int m_id) {
	for(int i = 0; i < MONSTER_TRAIT_WORDS; ++i)
		MonsterProperties[m_id].trait_bits[i] = 0;
}

bool HasPetMonsterTrait(int m_id, int t) {
	return !!(PetMonsterProperties[m_id].trait_bits[t >> 5] & (1 << (t & 31)));
}

void SetPetMonsterTrait(int m_id, int t, bool v) {
	if(v)
		PetMonsterProperties[m_id].trait_bits[t >> 5] |= 1 << (t & 31);
	else
		PetMonsterProperties[m_id].trait_bits[t >> 5] &= ~(1 << (t & 31));
}

bool HasMonsterDataTrait(int mon, int t) {
	return !!(MonsterData[mon].trait_bits[t >> 5] & (1 << (t & 31)));
}

void SetMonsterDataTrait(int mon, int t, bool v) {
	if(v)
		MonsterData[mon].trait_bits[t >> 5] |= 1 << (t & 31);
	else
		MonsterData[mon].trait_bits[t >> 5] &= ~(1 << (t & 31));
}

// Could this monster have any touch trait? One word read and one masked test, in place of three
// HasMonsterTrait calls that for almost every monster all return false -- and this runs on every hit
// a player takes, so the wasted calls were the whole cost of the feature for players fighting
// ordinary monsters.
//
// The word comparison folds at compile time. If the enum ever splits the three traits across words
// this returns true unconditionally and the caller's loop does the real work: it degrades to the old
// behaviour instead of silently skipping the monsters that do have one.
bool MonsterHasAnyTouchTrait(int m_id) {
	if((DND_FIRST_TOUCHTRAIT >> 5) != (DND_LAST_TOUCHTRAIT >> 5))
		return true;
	return !!(MonsterProperties[m_id].trait_bits[DND_FIRST_TOUCHTRAIT >> 5] & DND_TOUCHTRAIT_MASK);
}

// the per-monster-type preset, copied onto a spawned monster whole
void CopyMonsterDataTraits(int m_id, int mon) {
	for(int i = 0; i < MONSTER_TRAIT_WORDS; ++i)
		MonsterProperties[m_id].trait_bits[i] = MonsterData[mon].trait_bits[i];
}

enum {
	DND_BOSS_LOWTIER,
	DND_BOSS_MEDTIER,
	DND_BOSS_HIGHTIER,
	DND_BOSS_ENDGAME
};

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

	MONSTER_DUNGEON_BLOODGOLEM,
	MONSTER_DUNGEON_DEMENTOR,
	MONSTER_DUNGEON_VASSAGO,
	MONSTER_DUNGEON_SHAMBLER,

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

bool IsLootChestDroppingMonster(int m_id) {
	bool category_ok = false;
	switch(MonsterProperties[m_id].class) {
		case MONSTERCLASS_HELLKNIGHT:
		case MONSTERCLASS_BARON:
		case MONSTERCLASS_FATSO:
		case MONSTERCLASS_ARACHNOTRON:
		case MONSTERCLASS_ARCHVILE:
		case MONSTERCLASS_SPIDERMASTERMIND:
		case MONSTERCLASS_CYBERDEMON:
			category_ok = true;
		break;
	}

	// rarer than VERYRARE and fitting category
	return category_ok && MonsterProperties[m_id].rarity <= DND_MWEIGHT_VERYRARE;
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

// ============================================================================
//  MONSTER HEALTH SCALING -- returns the PERCENT added to base health, so 400 means 5x total.
//
//  SPINE:  level < 25 -> 0.42x^2 + 2x,  level >= 25 -> 0.38x^2 + 3x
//  The branches are equal at EXACTLY x=25, so the linear term must be added to BOTH or the
//  crossover breaks and a level 24 monster outranks a level 25 one. That extra x is the low tier
//  bump: +5..7% below level 40, washed out by 100.
//
//  TWO TAILS, because one curve cannot do both jobs:
//   - QUADRATIC from LIFTKNEE. A cubic is nearly flat just past its own knee, so one placed near
//     60 contributes nothing AT 60. This lifts the 60-80 stretch.
//   - CUBIC from SPIKEKNEE. Zero at 80 by construction, small at 85, then it dominates. This is
//     the "real threat starts at 80" term.
//
//    lvl      60     70     80     90    100     60->80   80->100
//    old    15.9x  21.0x  26.9x  33.6x  41.0x     1.81x     1.52x
//    new    17.6x  24.8x  33.8x  48.6x  89.1x     1.92x     2.64x
//
//  DND_HPSCALE_SPIKEDIV is THE balance knob, and it moves nothing below 80: div 3 gives 76x at
//  level 100, div 2 gives 90x. The old formula was multiplicative (x3 from level 50, x9 from 75).
// ============================================================================
#define DND_HPSCALE_LIFTKNEE 45
#define DND_HPSCALE_LIFTDIV 2
#define DND_HPSCALE_SPIKEKNEE 80
// The spike carries a numerator because the endpoint is fixed: sharpening the lift above raises
// level 100 as well, so the cubic has to come back down by the same amount to keep 100 where it was.
// 2/5 is what holds it at ~89x; no whole divisor lands there.
#define DND_HPSCALE_SPIKEMUL 2
#define DND_HPSCALE_SPIKEDIV 5

// Cubing (level - SPIKEKNEE) forces a ceiling on the INPUT: dnd_maxmonsterlevel is a server cvar
// and nothing stops an admin setting 5000, where the cube alone wraps. Past this the curve
// plateaus, which costs nothing -- DND_MAX_HPSCALE_PCT capped the result long before (~level 129).
#define DND_HPSCALE_LEVELCAP 250

// How much of the curve is precomputed. Monster level caps at 100 (dnd_maxmonsterlevel), so that is
// the whole useful range. Levels past it fall through to the arithmetic rather than being clamped, so
// a server that raises the cvar stays correct instead of silently flattening.
#define DND_HPSCALE_TABLEMAX 100

int GetMonsterHPScalingRaw(int level) {
	if(level <= 0)
		return 0;

	if(level > DND_HPSCALE_LEVELCAP)
		level = DND_HPSCALE_LEVELCAP;

	int res;
	if(level < 25)
		res = 21 * level * level / 50 + 2 * level;
	else
		res = (38 * level * level) / 100 + 3 * level;

	int t;

	// the 60-80 lift
	if(level > DND_HPSCALE_LIFTKNEE) {
		t = level - DND_HPSCALE_LIFTKNEE;
		res += t * t / DND_HPSCALE_LIFTDIV;
	}

	// the endgame spike
	if(level > DND_HPSCALE_SPIKEKNEE) {
		t = level - DND_HPSCALE_SPIKEKNEE;
		res += t * t * t * DND_HPSCALE_SPIKEMUL / DND_HPSCALE_SPIKEDIV;
	}

	return res;
}

// Memoised level -> percent. The curve depends on nothing but the level, so it is built once on
// first use and read as an array from then on -- this runs for every monster on the map, and at map
// load they all clear their setup wait on the same tic.
int GetMonsterHPScaleForLevel(int level) {
	static int cache[DND_HPSCALE_TABLEMAX + 1];
	static bool cache_built = false;

	if(!cache_built) {
		for(int i = 0; i <= DND_HPSCALE_TABLEMAX; ++i)
			cache[i] = GetMonsterHPScalingRaw(i);
		cache_built = true;
	}

	if(level >= 0 && level <= DND_HPSCALE_TABLEMAX)
		return cache[level];

	return GetMonsterHPScalingRaw(level);
}

int GetMonsterHPScaling(int m_id, int level) {
	int res = GetMonsterHPScaleForLevel(level);

	// big bosses have higher scaling than other monsters
	if(IsUniqueBossMonster(m_id)) {
		// clamped for the same reason the curve is -- this multiplies, so an unbounded level here
		// would undo the ceiling above
		if(level > DND_HPSCALE_LEVELCAP)
			level = DND_HPSCALE_LEVELCAP;
		res *= 1 + (level / 33);
	}

	return res;
}

// ============================================================================
//  HEALTH CEILINGS
//
//  Final health must stay far enough below INT_MAX for everything DOWNSTREAM too: borrowed time
//  and energy leech stock MonsterFortifyCount from maxhp, and the cull path does health * 2.
//  Half a billion leaves each a clean factor of four.
#define DND_MAX_MONSTER_HP 500000000

//  The largest percentage any monster may receive, boss multiplier and dungeon bonuses included.
//  BOUNDING THE PERCENT is what makes base * pct safe -- checking the product afterwards cannot
//  help, it has already wrapped. 2000x is a backstop for absurd cvars, not a balance knob: a
//  level 100 unique boss lands on 354x.
#define DND_MAX_HPSCALE_PCT 200000

// base * pct / 100, without ever forming an intermediate that can wrap.
int ApplyHPScale(int base, int pct) {
	if(base <= 0 || pct <= 0)
		return 0;

	if(pct > DND_MAX_HPSCALE_PCT)
		pct = DND_MAX_HPSCALE_PCT;

	int add;
	if(base >= 100) {
		// Divide FIRST. (base / 100) * pct keeps the product two orders of magnitude smaller, which
		// is the whole difference between fitting and wrapping; the dropped remainder is under
		// 99 * pct / 100, noise beside numbers this size. Only reachable with an absurd hp_mult, but
		// saturate rather than hand back a negative health if even that would not fit.
		if(base / 100 > bcs::INT_MAX / pct)
			add = DND_MAX_MONSTER_HP;
		else
			add = (base / 100) * pct;
	}
	else
		add = base * pct / 100;

	// clamp the TOTAL, not just the addition
	if(add > DND_MAX_MONSTER_HP - base)
		add = DND_MAX_MONSTER_HP - base;

	return add < 0 ? 0 : add;
}

int GetEliteBonusDamage(int m_id) {
	// at level 100 this yields 125%, at level 0 13% and at level 50 44%
	int lvl = MonsterProperties[m_id].level;
	return DND_ELITE_DMGSCALE + lvl * lvl / 100 + (lvl * 3) / 25;
}

int GetBasicMonsterDMGScaling(int level, bool bonusScaling) {
	int base = level * level * 27 / 400 - level / 5 + 5;

	if(bonusScaling)
		base += level * 5 / 2;

	return base;
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

	if(HasMonsterTrait(m_id, DND_EXTRASTRONG))
		res = res * (100 + DND_ELITE_EXTRASTRONG_BONUS) / 100;

	// elite damage bonus is multiplicative
	if(MonsterProperties[m_id].flags & DND_MONFLAG_ISELITE)
		res = res * (100 + GetEliteBonusDamage(m_id)) / 100;
	else if(MonsterProperties[m_id].flags & DND_MONFLAG_ISMAGIC) // half of elite
		res = res * (100 + GetEliteBonusDamage(m_id)) / 200;

	// chaos mark is multiplicative
	if(HasMonsterTrait(m_id, DND_MARKOFCHAOS))
		res = res * (100 + CHAOSMARK_DAMAGEBUFF) / 100;
	else if(HasMonsterTrait(m_id, DND_MARKOFASMODEUS))
		res = res * (100 + ASMODEUSMARK_DAMAGEBUFF) / 100;
	
	// unique bosses have additional damage multiplier per level -- x^2 * 0.01667 + x
	if(IsUniqueBossMonster(m_id))
		res = res * (100 + (level * level) / 60 + level) / 100;

	if((level = HasDungeonAttributeVal(DUN_ATTR_MOREDMG)) != -1)
		res = res * (100 + level) / 100;
	
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