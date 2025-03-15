#ifndef DND_MONSTERDATA_IN
#define DND_MONSTERDATA_IN

#include "DnD_Common.h"
#include "DnD_EliteInfo.h"

#define DND_CUSTOMMONSTER_ID 65536
#define DND_MONSTERMASS_SCALE 20 // 20% per level

#define DND_DEFAULT_MONSTERDMG_SCALING_FACTOR 120

#define ETHEREAL_RESIST 33

#define MONSTER_RES_PER_THRESHOLD 25

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

int GetMonsterDMGScaling(int m_id, int level, bool forShow = false, int scaling_factor = 0, int scaling_ramp = 0) {
	// over the old formula of 4x, this provides 500% damage at lvl 100 instead of 400%
	// edit: iterating over the improvement on previous versions, making the game harder -- divisor was 25 instead of 10 -- 10 was too high, trying 20
	int res = level * level / 20 + level;

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
	
	// unique bosses have additional damage multiplier per level -- x^2 * 0.01667 + x
	if(!forShow && IsUniqueBossMonster(m_id))
		res = res * (100 + (level * level) / 60 + level) / 100;
	
	return res;
}

// used to eliminate various random calls when a monster dies, to offload it to initialization phase to reuse the numbers
enum {
	DND_MON_RNG_1,
	DND_MON_RNG_2,
	DND_MON_RNG_3,
	DND_MON_RNG_4
};
#define DND_MAX_MONSTER_PRECALC_RNG 4

typedef struct {
	int basehp;
	int maxhp;
	int level;
	int id;
	int class;										// monster class
	int gain;										// the gain value the monster will provide on killing
	int rarity;										// monster's rarity;
	int droprate;									// droprate multiplier for this monster based on its initialization data
	int rarity_boost;								// item rarity boost from monster
	int killer_tid;									// tid of the killer
	int rng_vals[DND_MAX_MONSTER_PRECALC_RNG];		// precalculated rng outcomes
	bool isElite;
	bool hasTrait;									// used by clients mostly -- do we have traits
	bool isIdle;									// monster is idle
	int resists[MAX_DAMAGE_CATEGORIES];				// resists of the monster
	bool trait_list[MAX_MONSTER_TRAITS_STORED]; 	// 1 if that trait is on, 0 if not
} mo_prop_T;

// allow a max of 8192 monsters' data to be held
global mo_prop_T 25: MonsterProperties[DND_MAX_MONSTERS];
global mo_prop_T 26: PetMonsterProperties[DND_MAX_PETS];

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

// this is the base exp value you get for killing a monster of this class, where all bonuses will be applied to
// shifted right by 16 it returns the droprate multiplier
int GetMonsterClassBonus(int class) {
	switch(class) {
		case MONSTERCLASS_ZOMBIEMAN:
		return 6 | (15 << 16);
		
		case MONSTERCLASS_SHOTGUNGUY:
		return 9 | (20 << 16);
		
		case MONSTERCLASS_CHAINGUNGUY:
		return 15 | (28 << 16);
		
		case MONSTERCLASS_DEMON:
		return 21 | (32 << 16);
		
		case MONSTERCLASS_SPECTRE:
		return 24 | (35 << 16);
		
		case MONSTERCLASS_IMP:
		return 12 | (24 << 16);
		
		case MONSTERCLASS_CACODEMON:
		return 45 | (50 << 16);
		
		case MONSTERCLASS_PAINELEMENTAL:
		return 54 | (60 << 16);
		
		case MONSTERCLASS_LOSTSOUL:
		return 24 | (24 << 16);
		
		case MONSTERCLASS_REVENANT:
		return 33 | (40 << 16);
		
		case MONSTERCLASS_HELLKNIGHT:
		return 48 | (52 << 16);
		
		case MONSTERCLASS_BARON:
		return 81 | (75 << 16);
		
		case MONSTERCLASS_FATSO:
		return 76 | (75 << 16);
		
		case MONSTERCLASS_ARACHNOTRON:
		return 72 | (75 << 16);
		
		case MONSTERCLASS_ARCHVILE:
		return 90 | (96 << 16);
		
		case MONSTERCLASS_SPIDERMASTERMIND:
		return 600 | (125 << 16);
		
		case MONSTERCLASS_CYBERDEMON:
		return 1080 | (135 << 16);
		
		case MONSTERCLASS_WOLFENSS:
		return 16 | (18 << 16);
	}
	
	return 1;
}

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
	MONSTER_YETI,
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
#define DND_CUSTOM_FATSO_END MONSTER_GOLDGOLEM

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

#define DND_BOSS_BEGIN MONSTER_DEMOLISHER
#define LEGENDARY_START MONSTER_DREAMINGGOD
#define LEGENDARY_END MONSTER_GOLGOTH
#define DND_LASTMONSTER_INDEX MONSTER_ABAXOTH
#define DND_MAX_LEGENDARY (LEGENDARY_END - LEGENDARY_START + 1)
#define MONSTER_COUNT (DND_LASTMONSTER_INDEX + 50) // possible compatibility for other wads' monsters

void ScaleMonsterMass(int level) {
	int m = GetActorProperty(0, APROP_MASS);
	int new_m = m * (100 + level * DND_MONSTERMASS_SCALE) / 100;
	
	// overflow check
	if(m < new_m)
		SetActorProperty(0, APROP_MASS, new_m);
	else if(m > new_m)
		SetActorProperty(0, APROP_MASS, INT_MAX);
}

enum {
	DND_MONSTERSIZE_SMALL,
	DND_MONSTERSIZE_MEDIUM,
	DND_MONSTERSIZE_BIG
};

int GetMonsterSizeType(int id) {
	switch(id) {
		case MONSTER_ZOMBIEMAN:
		case MONSTER_SHOTGUNNER:
		case MONSTER_CHAINGUNNER:
		case MONSTER_DEMON:
		case MONSTER_SPECTRE:
		case MONSTER_IMP:
		case MONSTER_NAZI:
		case MONSTER_LOSTSOUL:
		return DND_MONSTERSIZE_SMALL;
		
		case MONSTER_CACODEMON:
		case MONSTER_PAINELEMENTAL:
		case MONSTER_REVENANT:
		case MONSTER_HELLKNIGHT:
		case MONSTER_VILE:
		return DND_MONSTERSIZE_MEDIUM;
		
		case MONSTER_BARON:
		case MONSTER_FATSO:
		case MONSTER_SPIDER:
		case MONSTER_MASTERMIND:
		case MONSTER_CYBERDEMON:
		return DND_MONSTERSIZE_BIG;
		
		default:
		if(id <= DND_CUSTOM_LOSTSOUL_END)
			return DND_MONSTERSIZE_SMALL;
		if(id < DND_CUSTOM_BARON_BEGIN || (id >= DND_CUSTOM_VILE_BEGIN && id <= DND_CUSTOM_VILE_END))
			return DND_MONSTERSIZE_MEDIUM;
		return DND_MONSTERSIZE_BIG;
	}
}

bool IsBlindCastingMonster(int m_id) {
	switch(m_id) {
		case MONSTER_HELLSFURY:
		case MONSTER_OPHIDIAN:
		case MONSTER_IRONLICH:
		return true;
	}
	return false;
}

// this is the single source that gives the monsters their proper class protections
// or their weaknesses
void HandleMonsterClassInnates(int mid, int id) {
	if
	(
		id == MONSTER_ZOMBIEMAN || 
		(id >= DND_CUSTOM_ZOMBIEMAN_BEGIN && id <= DND_CUSTOM_ZOMBIEMAN_END) ||
		(id >= DND_UNIQUE_ZOMBIEMAN_BEGIN && id <= DND_UNIQUE_ZOMBIEMAN_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_ZOMBIEMAN;
	}
	else if
	(
		id == MONSTER_SHOTGUNNER || 
		(id >= DND_CUSTOM_SHOTGUNGUY_BEGIN & id <= DND_CUSTOM_SHOTGUNGUY_END) ||
		(id >= DND_UNIQUE_SHOTGUNGUY_BEGIN & id <= DND_UNIQUE_SHOTGUNGUY_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_SHOTGUNGUY;
	}
	else if
	(
		id == MONSTER_CHAINGUNNER || 
		(id >= DND_CUSTOM_CHAINGUNGUY_BEGIN && id <= DND_CUSTOM_CHAINGUNGUY_END) ||
		(id >= DND_UNIQUE_CHAINGUNGUY_BEGIN && id <= DND_UNIQUE_CHAINGUNGUY_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_CHAINGUNGUY;
	}
	else if
	(
		id == MONSTER_IMP || 
		(id >= DND_CUSTOM_IMP_BEGIN && id <= DND_CUSTOM_IMP_END) ||
		(id >= DND_UNIQUE_IMP_BEGIN && id <= DND_UNIQUE_IMP_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_IMP;
	}
	else if
	(
		id == MONSTER_DEMON || 
		(id >= DND_CUSTOM_DEMON_BEGIN && id <= DND_CUSTOM_DEMON_END) ||
		(id >= DND_UNIQUE_DEMON_BEGIN && id <= DND_UNIQUE_DEMON_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_DEMON;
	}
	else if
	(
		id == MONSTER_SPECTRE || 
		(id >= DND_CUSTOM_SPECTRE_BEGIN && id <= DND_CUSTOM_SPECTRE_END) ||
		(id >= DND_UNIQUE_SPECTRE_BEGIN && id <= DND_UNIQUE_SPECTRE_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_SPECTRE;
	}
	else if
	(
		id == MONSTER_LOSTSOUL || 
		(id >= DND_CUSTOM_LOSTSOUL_BEGIN && id <= DND_CUSTOM_LOSTSOUL_END) ||
		(id >= DND_UNIQUE_LOSTSOUL_BEGIN && id <= DND_UNIQUE_LOSTSOUL_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_LOSTSOUL;
	}
	else if
	(
		id == MONSTER_CACODEMON || 
		(id >= DND_CUSTOM_CACODEMON_BEGIN && id <= DND_CUSTOM_CACODEMON_END) ||
		(id >= DND_UNIQUE_CACODEMON_BEGIN && id <= DND_UNIQUE_CACODEMON_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_CACODEMON;
	}
	else if
	(
		id == MONSTER_PAINELEMENTAL ||
		(id >= DND_CUSTOM_PAINE_BEGIN && id <= DND_CUSTOM_PAINE_END) ||
		(id >= DND_UNIQUE_PAINE_BEGIN && id <= DND_UNIQUE_PAINE_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_PAINELEMENTAL;
	}
	else if
	(
		id == MONSTER_REVENANT ||
		(id >= DND_CUSTOM_REVENANT_BEGIN && id <= DND_CUSTOM_REVENANT_END) ||
		(id >= DND_UNIQUE_REVENANT_BEGIN && id <= DND_UNIQUE_REVENANT_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_REVENANT;
	}
	else if
	(
		id == MONSTER_HELLKNIGHT ||
		(id >= DND_CUSTOM_HK_BEGIN && id <= DND_CUSTOM_HK_END) ||
		(id >= DND_UNIQUE_HK_BEGIN && id <= DND_UNIQUE_HK_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_HELLKNIGHT;
	}
	else if
	(
		id == MONSTER_BARON ||
		(id >= DND_CUSTOM_BARON_BEGIN && id <= DND_CUSTOM_BARON_END) ||
		(id >= DND_UNIQUE_BARON_BEGIN && id <= DND_UNIQUE_BARON_END)
	) 
	{
		MonsterProperties[mid].class = MONSTERCLASS_BARON;
	}
	else if
	(
		id == MONSTER_FATSO || 
		(id >= DND_CUSTOM_FATSO_BEGIN && id <= DND_CUSTOM_FATSO_END) ||
		(id >= DND_UNIQUE_FATSO_BEGIN && id <= DND_UNIQUE_FATSO_END)
	) 
	{
		MonsterProperties[mid].class = MONSTERCLASS_FATSO;
	}
	else if
	(
		id == MONSTER_SPIDER ||
		(id >= DND_CUSTOM_ARACHNO_BEGIN && id <= DND_CUSTOM_ARACHNO_END) ||
		(id >= DND_UNIQUE_ARACHNO_BEGIN && id <= DND_UNIQUE_ARACHNO_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_ARACHNOTRON;
	}
	else if
	(
		id == MONSTER_VILE || 
		(id >= DND_CUSTOM_VILE_BEGIN && id <= DND_CUSTOM_VILE_END) ||
		(id >= DND_UNIQUE_VILE_BEGIN && id <= DND_UNIQUE_VILE_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_ARCHVILE;
	}
	else if
	(
		id == MONSTER_MASTERMIND ||
		(id >= DND_CUSTOM_SM_BEGIN && id <= DND_CUSTOM_SM_END) ||
		(id >= DND_UNIQUE_SM_BEGIN && id <= DND_UNIQUE_SM_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_SPIDERMASTERMIND;
	}
	else if
	(
		id == MONSTER_CYBERDEMON ||
		(id >= DND_CUSTOM_CYBER_BEGIN && id <= LEGENDARY_END) ||
		(id >= DND_UNIQUE_CYBER_BEGIN)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_CYBERDEMON;
	}
	else if(id == MONSTER_NAZI)
		MonsterProperties[mid].class = MONSTERCLASS_WOLFENSS;
}

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

#define DND_ELITE_GAINBONUS 30 // 30%

int GetMonsterLevelDroprateBonus(int lvl) {
	// piecewise function so the early 25 levels increase sharper, then slow down but reach to a sweet spot of x2.5
	/*if(lvl <= 25)
		return 4 * lvl;
	else if(lvl <= 50)
		return lvl * lvl / 100 + 3 * lvl + 19;
	return lvl * lvl / 150 + 184;
	if(lvl <= 25)
		return 4 * lvl;
	return 2 * lvl + 50;*/
	// new formula to ensure a sharp curve earlier levels then settle down
	return FixedDiv(600.0, (80.0 / lvl + 3.0)) >> 16;
}

int GetMonsterRarityDroprateBonus(int rarity) {
	switch(rarity) {
		case DND_MWEIGHT_COMMON:
		return 0;
		
		case DND_MWEIGHT_UNCOMMON:
		return 1;
		
		case DND_MWEIGHT_RARE1:
		return 2;
		
		case DND_MWEIGHT_RARE2:
		return 3;
		
		case DND_MWEIGHT_VERYRARE:
		return 4;
		
		case DND_MWEIGHT_EPIC:
		return 5;
	}
	return 1;
}

/* 
	Multiply the bonus with 1000 / rarity of monster to get a percentage
	This is a linear, simple bonus added on top just because of the rarity of the monster. While rarity is important, the level matters more. However early on it should have a tiny impact still.
*/
#define DND_DROPBONUS_FROM_RARITY 20
#define DND_ELITEBONUS_FROM_RARITY 50
int GetMonsterDropBonus(int drop_base, int level, int rarity, bool isElite) {
	// first a 50% from elites as a multiplicative bonus, then the rest
	return 	(drop_base * (100 + DND_ELITEBONUS_FROM_RARITY * isElite) / 100) * 
			(100 + GetMonsterLevelDroprateBonus(level) + DND_DROPBONUS_FROM_RARITY * GetMonsterRarityDroprateBonus(rarity)) / 100;
}

// you gain the returned value for exp, and third of that for credits -- rarity is monster rarity not item related rarity!
void CalculateMonsterGainMult(int m_id, int rarity = DND_MWEIGHT_COMMON) {
	int base = 0;
	int drop_base = 0;
	
	// per lvl we get base 20%, and 30% per lvl if monster is elite
	int pct = 	100 + (MonsterProperties[m_id].level - 1) * (DND_MONSTERBONUS_PERLVL + DND_ELITE_GAINBONUS * MonsterProperties[m_id].isElite);
	//printbold(s:"pct: ", d:pct);
	
	// if rarity is within general rarity range, get monster class respective bonus
	// it may not be the case for unique bosses, or any monster that is special. So we use the value in here as the base exp reward to be given instead
	if(rarity <= DND_MWEIGHT_COMMON) {
		base = GetMonsterClassBonus(MonsterProperties[m_id].class);
		drop_base = base >> 16;
		base &= 0xFFFF;
		
		//printbold(s:"base: ", d:base);
		base = base * pct / 100;
		//printbold(s:"base after pct ", d:base);
		// depending on rarity apply a more multiplier
		MonsterProperties[m_id].gain = base * DND_MWEIGHT_COMMON / rarity;
		//printbold(s:"base final ", d:MonsterProperties[m_id].gain);
	
		// droprate for regular monsters, low level trash tier monsters drop nothing or rarely vs. higher tier and higher level monsters
		MonsterProperties[m_id].droprate = GetMonsterDropBonus(drop_base, MonsterProperties[m_id].level, rarity, MonsterProperties[m_id].isElite);
		//printbold(s:"droprate % inc for ", s:GetActorClass(0), s: " of level ", d:MonsterProperties[m_id].level, s: " is ", d:MonsterProperties[m_id].droprate);
		
		MonsterProperties[m_id].rarity_boost = MonsterProperties[m_id].droprate / 3; // was 2
	}
	else {
		// since rarity for us doesnt make sense as a multiplier here we just return the amplified percentage
		base = rarity;
		MonsterProperties[m_id].gain = base = base * pct / 100;
		//printbold(s:"base final special: ", d:MonsterProperties[m_id].gain);
		
		// droprate here for custom dungeon bosses is 0, because they should call their own loot dropper script!
		MonsterProperties[m_id].droprate = 0;
		MonsterProperties[m_id].rarity_boost = 33; // 33% rarity boost on dungeon monsters
	}
}

// this has any special trait that requires a script to run BEFORE an elite status can apply to a monster
void HandlePreInitTraits(int mid, int id) {
	if(MonsterProperties[mid].trait_list[DND_REJUVENATING])
		GiveInventory("Rejuvenate_Script_Run", 1);
	if(MonsterProperties[mid].trait_list[DND_REPEL])
		GiveInventory("Repel_Script_Run", 1);
	if(MonsterProperties[mid].trait_list[DND_CRIPPLE])
		GiveInventory("Cripple_Script_Run", 1);
	if(MonsterProperties[mid].trait_list[DND_PHASING])
		GiveInventory("Phasing_Script_Run", 1);
	if(MonsterProperties[mid].trait_list[DND_VIOLENTRETALIATION]) {
		ACS_NamedExecuteAlways("DnD Aura Giver CS", 0, DND_VIOLENTRETALIATION);
	}
	if(MonsterProperties[mid].trait_list[DND_TEMPORALBUBBLE])
		ACS_NamedExecuteAlways("DnD Aura Giver CS", 0, DND_TEMPORALBUBBLE);
	if(MonsterProperties[mid].trait_list[DND_ENSHROUDED])
		ACS_NamedExecuteAlways("DnD Aura Giver CS", 0, DND_ENSHROUDED);

	if(MonsterProperties[mid].trait_list[DND_REFLECTIVE])
		ACS_NamedExecuteAlways("DND Spawn Attachment", 0, mid + DND_MONSTERTID_BEGIN, DND_ELITEFX_REFLECT);
}

// this is put as a seperate function because 
void HandlePostInitTraits(int m_id, int id, int rarity = DND_MWEIGHT_COMMON, bool isRevived = false) {
	if(MonsterProperties[m_id].trait_list[DND_FORTIFIED]) {
		// full fortify exceptions
		if(id != MONSTER_TERON && id != MONSTER_CHEGOVAX)
			SetInventory("MonsterFortifyCount", MonsterProperties[m_id].maxhp * DND_FORTIFY_AMOUNT / 100);
		else
			SetInventory("MonsterFortifyCount", MonsterProperties[m_id].maxhp);
	}

	// don't do anything past this point if revived, there is no need, the data for these is already stored
	if(isRevived)
		return;
	
	// calculate the gains multiplier -- this is the safest place to do as most of monster data is now known by this point, like level etc.
	// all unique monsters have highest possible rarity
	if(rarity == -1) {
		if(!IsUniqueMonster(id))
			rarity = GetMonsterRarity(m_id);
		else
			rarity = DND_MWEIGHT_EPIC;
	}
		
	//printbold(s:GetActorClass(0), s:" rarity ", d:rarity, s: " ", d:MonsterProperties[m_id].class, s: " ", d:MonsterProperties[m_id].id);
	
	MonsterProperties[m_id].rarity = rarity;
	
	CalculateMonsterGainMult(m_id, rarity);

	// finally handle the monster resists according to its traits
	InitMonsterResists(m_id);
}

void InitMonsterResists(int m_id) {
	int temp = 0;
	int bonus = MONSTER_RES_PER_THRESHOLD * (MonsterProperties[m_id].level / DND_MONSTER_RESIST_LEVELS);
	int etherealBonus = MonsterProperties[m_id].trait_list[DND_ETHEREAL] * ETHEREAL_RESIST;

	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_BULLET] = 	MonsterProperties[m_id].trait_list[DND_BULLET_RESIST] * DND_RESIST_FACTOR + bonus +
																	etherealBonus +
																	MonsterProperties[m_id].trait_list[DND_BULLET_IMMUNE] * DND_IMMUNITY_FACTOR;

	// can be changed later, bullet and melee is just "phys" atm
	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_MELEE] = MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_BULLET];

	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_ENERGY] =	-MonsterProperties[m_id].trait_list[DND_ENERGY_WEAKNESS] * DND_WEAKNESS_FACTOR + bonus +
																	MonsterProperties[m_id].trait_list[DND_ENERGY_RESIST] * DND_RESIST_FACTOR + 
																	etherealBonus +
																	MonsterProperties[m_id].trait_list[DND_ENERGY_IMMUNE] * DND_IMMUNITY_FACTOR;

	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_OCCULT] =	-MonsterProperties[m_id].trait_list[DND_MAGIC_WEAKNESS] * DND_WEAKNESS_FACTOR + bonus + 
																	MonsterProperties[m_id].trait_list[DND_MAGIC_RESIST] * DND_RESIST_FACTOR + 
																	MonsterProperties[m_id].trait_list[DND_MAGIC_IMMUNE] * DND_IMMUNITY_FACTOR;
	// this is common to elemental stuff
	temp =	-MonsterProperties[m_id].trait_list[DND_ELEMENTAL_WEAKNESS] * DND_WEAKNESS_FACTOR + bonus +
			MonsterProperties[m_id].trait_list[DND_ELEMENTAL_RESIST] * DND_RESIST_FACTOR + 
			etherealBonus +
			MonsterProperties[m_id].trait_list[DND_ELEMENTAL_IMMUNE] * DND_IMMUNITY_FACTOR;

	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_FIRE] = -MonsterProperties[m_id].trait_list[DND_FIRE_WEAKNESS] * DND_SPECIFICELEWEAKNESS_FACTOR + temp;

	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_ICE] = -MonsterProperties[m_id].trait_list[DND_ICE_WEAKNESS] * DND_SPECIFICELEWEAKNESS_FACTOR + temp;

	// these two dont have their own weakness category yet...
	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_POISON] = temp;
	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_LIGHTNING] = temp;

	// immune for expl is the radius dmg component, none is not even direct hits etc.
	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_EXPLOSIVES] = 	MonsterProperties[m_id].trait_list[DND_EXPLOSIVE_RESIST] * DND_RESIST_FACTOR + bonus +
																		etherealBonus +
																		MonsterProperties[m_id].trait_list[DND_EXPLOSIVE_NONE] * DND_IMMUNITY_FACTOR;

	// soul is same as magic for now
	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_SOUL] = MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_OCCULT];

	// creature type bonuses -- only one type allowed per creature
	if(MonsterProperties[m_id].trait_list[DND_ICECREATURE]) {
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_ICE] = DND_IMMUNITY_FACTOR + bonus;
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_FIRE] -= DND_SPECIFICELEWEAKNESS_FACTOR;
	}
	else if(MonsterProperties[m_id].trait_list[DND_FIRECREATURE]) {
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_FIRE] = DND_IMMUNITY_FACTOR + bonus;
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_ICE] -= DND_SPECIFICELEWEAKNESS_FACTOR;
	}
	else if(MonsterProperties[m_id].trait_list[DND_STONECREATURE]) {
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_FIRE] += DND_RESIST_FACTOR;
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_ICE] -= DND_SPECIFICELEWEAKNESS_FACTOR;
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_LIGHTNING] = DND_IMMUNITY_FACTOR + bonus;
	}
	else if(MonsterProperties[m_id].trait_list[DND_EARTHCREATURE]) {
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_LIGHTNING] -= DND_SPECIFICELEWEAKNESS_FACTOR;
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_POISON] = DND_IMMUNITY_FACTOR + bonus;
	}

	// do a pass on all categories to clamp them
	for(temp = DND_DAMAGECATEGORY_BEGIN; temp <= DND_DAMAGECATEGORY_SOUL ; ++temp)
		if(MonsterProperties[m_id].resists[temp] > DND_IMMUNITY_FACTOR + bonus)
			MonsterProperties[m_id].resists[temp] = DND_IMMUNITY_FACTOR + bonus;
}

// this is only used in revive of monsters by itself
void HandleSpecialTraits(int mid, int id, bool isRevived = false) {
	HandlePreInitTraits(mid, id);
	HandlePostInitTraits(mid, id, -1, isRevived); // this had no params, I don't think sending -1 here will be much of a problem
}

void LoadMonsterTraits(int tid, int monsterid) {
	HandleMonsterClassInnates(tid, monsterid);
	
	// copy preset data to here now
	for(int j = 0; j < MAX_MONSTER_TRAITS_STORED; ++j) {
		//if(MonsterProperties[tid].class == MONSTERCLASS_IMP && MonsterData[monsterid].trait_list[j])
		//	printbold(s:"loading trait ", d:j, s:" for ", s:GetActorclass(ActivatorTID()), s:" <" , d:tid, s: "> == ", d:MonsterData[monsterid].trait_list[j]);
		MonsterProperties[tid].trait_list[j] = MonsterData[monsterid].trait_list[j];
	}
		
	// some of the flags are inherent in actor info, so do make use of that
	MonsterProperties[tid].trait_list[DND_GHOST] 					|= CheckFlag(0, "GHOST");
	MonsterProperties[tid].trait_list[DND_EXPLOSIVE_IMMUNE] 		|= CheckFlag(0, "NORADIUSDMG");
	MonsterProperties[tid].trait_list[DND_ARMORPEN] 				|= CheckFlag(0, "PIERCEARMOR");
	MonsterProperties[tid].trait_list[DND_HARDENED_SKIN] 			|= CheckFlag(0, "DONTRIP");
	MonsterProperties[tid].trait_list[DND_NOPAIN] 					|= CheckFlag(0, "NOPAIN");
	MonsterProperties[tid].trait_list[DND_REFLECTIVE] 				|= CheckFlag(0, "REFLECTIVE");

	GiveInventory("TakeReplacedFlags", 1);

	MonsterProperties[tid].trait_list[DND_LEGENDARY] = isLegendaryMonster(monsterid);
	
	// check for weaknesses and monster not having any kind of resist to this type
	// if magical or undead, give it silver weakness (this is common no exceptions)
	if(MonsterData[monsterid].flags & (DND_MTYPE_UNDEAD_POW | DND_MTYPE_MAGICAL_POW))
		MonsterProperties[tid].trait_list[DND_SILVER_WEAKNESS] = true;
		
	// if robotic, give energy weakness
	if((MonsterData[monsterid].flags & DND_MTYPE_ROBOTIC_POW) && !MonsterProperties[tid].trait_list[DND_ENERGY_RESIST] && !MonsterProperties[tid].trait_list[DND_ENERGY_IMMUNE])
		MonsterProperties[tid].trait_list[DND_ENERGY_WEAKNESS] = true;
	// if magical, give magic weakness
	if((MonsterData[monsterid].flags & DND_MTYPE_MAGICAL_POW) && !MonsterProperties[tid].trait_list[DND_MAGIC_RESIST] && !MonsterProperties[tid].trait_list[DND_MAGIC_IMMUNE])
		MonsterProperties[tid].trait_list[DND_MAGIC_WEAKNESS] = true;
	
	// debug
	//MonsterProperties[tid].trait_list[DND_REPEL] = true;
	
	// this is needed as some monsters have some of the properties in there that need to be applied without being elites
	HandlePreInitTraits(tid, monsterid);
}

void LoadCustomMonsterTraits(int m_id, int mon_type, int traits, int traits2, int traits3) {
	// force reset everything
	for(int j = 0; j < MAX_MONSTER_TRAITS_STORED; ++j) {
		//if(MonsterProperties[m_id].class == MONSTERCLASS_IMP && MonsterData[monsterid].trait_list[j])
		//	printbold(s:"loading trait ", d:j, s:" for ", s:GetActorclass(Activatorm_id()), s:" <" , d:m_id, s: "> == ", d:MonsterData[monsterid].trait_list[j]);
		MonsterProperties[m_id].trait_list[j] = false;
	}

	// some of the flags are inherent in actor info, so do make use of that
	MonsterProperties[m_id].trait_list[DND_GHOST] 					|= CheckFlag(0, "GHOST");
	MonsterProperties[m_id].trait_list[DND_EXPLOSIVE_IMMUNE] 		|= CheckFlag(0, "NORADIUSDMG");
	MonsterProperties[m_id].trait_list[DND_ARMORPEN] 				|= CheckFlag(0, "PIERCEARMOR");
	MonsterProperties[m_id].trait_list[DND_HARDENED_SKIN] 			|= CheckFlag(0, "DONTRIP");
	MonsterProperties[m_id].trait_list[DND_NOPAIN] 					|= CheckFlag(0, "NOPAIN");
	MonsterProperties[m_id].trait_list[DND_REFLECTIVE] 				|= CheckFlag(0, "REFLECTIVE");

	GiveInventory("TakeReplacedFlags", 1);

	if(traits != -1)
		MonsterProperties[m_id].trait_list[traits] = true;
	if(traits2 != -1)
		MonsterProperties[m_id].trait_list[traits2] = true;
	if(traits3 != -1)
		MonsterProperties[m_id].trait_list[traits3] = true;
		
	// check for weaknesses and monster not having any kind of resist to this type
	// if magical or undead, give it silver weakness (this is common no exceptions)
	if(mon_type & (DND_MTYPE_UNDEAD_POW | DND_MTYPE_MAGICAL_POW))
		MonsterProperties[m_id].trait_list[DND_SILVER_WEAKNESS] = true;
		
	// if robotic, give energy weakness
	if((mon_type & DND_MTYPE_ROBOTIC_POW) && !MonsterProperties[m_id].trait_list[DND_ENERGY_RESIST] && !MonsterProperties[m_id].trait_list[DND_ENERGY_IMMUNE])
		MonsterProperties[m_id].trait_list[DND_ENERGY_WEAKNESS] = true;
	// if magical, give magic weakness
	if((mon_type & DND_MTYPE_MAGICAL_POW) && !MonsterProperties[m_id].trait_list[DND_MAGIC_RESIST] && !MonsterProperties[m_id].trait_list[DND_MAGIC_IMMUNE])
		MonsterProperties[m_id].trait_list[DND_MAGIC_WEAKNESS] = true;
		
	HandlePreInitTraits(m_id, -1);
}

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
};

enum {
	DND_MTYPE_DEMON,
	DND_MTYPE_UNDEAD,
	DND_MTYPE_MAGICAL,
	DND_MTYPE_ROBOTIC,
	DND_MTYPE_HUMAN
};

// first 5 above dictate the basic monster types
#define MAX_MONSTER_TYPES 5
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
	return DND_MTYPE_DEMON;
}

int GetMonsterType(int monsterID, int monster_tid) {
	if(monsterID < MONSTER_CUSTOM)
		return InferMonsterPower(MonsterData[monsterID].flags);
	return InferMonsterPower(CheckActorInventory(monster_tid, "MonsterTypeToken"));
}

typedef struct {
	int health;
	int flags;
	bool trait_list[MAX_MONSTER_TRAITS_STORED];
} monster_data_T;

global monster_data_T 10: MonsterData[DND_LASTMONSTER_INDEX + 1];

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
	return id == MONSTER_MASTERMIND || id == MONSTER_CYBERDEMON || id == MONSTER_CUSTOM_BOSS || isUniqueBossMonster_Id(id) || (id >= DND_BOSS_BEGIN && id < DND_UNIQUEMONSTER_BEGIN) || (id >= DND_UNIQUEBOSS_BEGIN && id < MONSTER_CUSTOM);
}

bool IsUniqueMonster(int id) {
	return id >= DND_UNIQUEMONSTER_BEGIN && id < MONSTER_CUSTOM;
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

// all demon barons, fatsos or arachnos or bosses that are demons can drop a soul ammo
bool CanDropSoulAmmoTID(int tid) {
	int mid = MonsterProperties[tid - DND_MONSTERTID_BEGIN].id;
	return (mid == MONSTER_HELLKNIGHT || (mid >= DND_CUSTOM_HK_BEGIN && mid <= DND_CUSTOM_HK_END) ||
			mid == MONSTER_BARON || (mid >= DND_CUSTOM_BARON_BEGIN && mid <= DND_CUSTOM_BARON_END) || 
			mid == MONSTER_FATSO || (mid >= DND_CUSTOM_FATSO_BEGIN && mid <= DND_CUSTOM_FATSO_END) ||
			mid == MONSTER_SPIDER || (mid >= DND_CUSTOM_ARACHNO_BEGIN && mid <= DND_CUSTOM_ARACHNO_END) ||
			mid == MONSTER_VILE || (mid >= DND_CUSTOM_VILE_BEGIN && mid <= DND_CUSTOM_VILE_END) ||
			isBoss()) && (MonsterData[mid].flags & DND_MTYPE_DEMON_POW);
}

bool CanDropSoulAmmo() {
	return CanDropSoulAmmoTID(ActivatorTID());
}

int GetMonsterRarity(int monster_id) {
	int r = GetActorProperty(monster_id + DND_MONSTERTID_BEGIN, APROP_SCORE);
	if(!r)
		return DND_MWEIGHT_COMMON;
	return r;
}

// these are filled by hand, finding the rarest monster of each class is needlessly complicated (find top classes, equal ones must be considered as sharing top spot etc.)
// this requires frequent updates the more monsters are added, but is super fast
void HandleSubordinateSpawn(int tid, int mid) {
	int class = MonsterProperties[mid].class;
	int rng;
	str toSpawn = "";
	switch(class) {
		case MONSTERCLASS_ZOMBIEMAN:
			rng = random(0, 3);
			switch(rng) {
				case 0:
					toSpawn = "FormerRanger";
				break;
				case 1:
					toSpawn = "RapidFireTrooper";
				break;
				case 2:
					toSpawn = "ZombieMarine";
				break;
				case 3:
					toSpawn = "LOSZombieMan";
				break;
			}
		break;
		case MONSTERCLASS_WOLFENSS:
			toSpawn = "WolfensteinSS2";
		break;
		case MONSTERCLASS_SHOTGUNGUY:
			toSpawn = "SSGZombie";
		break;
		case MONSTERCLASS_CHAINGUNGUY:
			toSpawn = "DoubleChaingunner";
		break;
		case MONSTERCLASS_DEMON:
			toSpawn = "PurebredDemon";
		break;
		case MONSTERCLASS_SPECTRE:
			toSpawn = "RavagerGhost";
		break;
		case MONSTERCLASS_IMP:
			toSpawn = "ReaverRobot";
		break;
		case MONSTERCLASS_CACODEMON:
			rng = random(0, 1);
			switch(rng) {
				case 0:
					toSpawn = "CacoLich";
				break;
				case 1:
					toSpawn = "EarthLich";
				break;
			}
		break;
		case MONSTERCLASS_PAINELEMENTAL:
			rng = random(0, 1);
			switch(rng) {
				case 0:
					toSpawn = "BloodLich";
				break;
				case 1:
					toSpawn = "DESentinel";
				break;
			}
		break;
		case MONSTERCLASS_LOSTSOUL:
			rng = random(0, 1);
			switch(rng) {
				case 0:
					toSpawn = "ESoul";
				break;
				case 1:
					toSpawn = "Watcher";
				break;
			}
		break;
		case MONSTERCLASS_REVENANT:
			rng = random(0, 2);
			switch(rng) {
				case 0:
					toSpawn = "WidowMaker";
				break;
				case 1:
					toSpawn = "Cadaver";
				break;
				case 2:
					toSpawn = "CyborgSoldier";
				break;
			}
		break;
		case MONSTERCLASS_HELLKNIGHT:
			rng = random(0, 2);
			switch(rng) {
				case 0:
					toSpawn = "IceGolem";
				break;
				case 1:
					toSpawn = "Putrefier";
				break;
				case 2:
					toSpawn = "Gladiator";
				break;
			}
		break;
		case MONSTERCLASS_BARON:
			toSpawn = "Warmaster";
		break;
		case MONSTERCLASS_FATSO:
			rng = random(0, 2);
			switch(rng) {
				case 0:
					toSpawn = "Gamon";
				break;
				case 1:
					toSpawn = "Mephisto";
				break;
				case 2:
					toSpawn = "GoldGolem";
				break;
			}
		break;
		case MONSTERCLASS_ARACHNOTRON:
			toSpawn = "Manticore";
		break;
		case MONSTERCLASS_ARCHVILE:
			rng = random(0, 2);
			switch(rng) {
				case 0:
					toSpawn = "UndeadPriest";
				break;
				case 1:
					toSpawn = "Horshacker";
				break;
				case 2:
					toSpawn = "FleshWizard";
				break;
			}
		break;
		case MONSTERCLASS_SPIDERMASTERMIND:
			toSpawn = "GoldLich";
		break;
		case MONSTERCLASS_CYBERDEMON:
			rng = random(0, 1);
			switch(rng) {
				case 0:
					toSpawn = "Cerberus";
				break;
				case 1:
					toSpawn = "Thanatos";
				break;
			}
		break;
	}
	
	// create some special fx
	if(Spawn(toSpawn, GetActorX(tid), GetActorY(tid), GetActorZ(tid), DND_SUBORDINATE_TEMPTID, GetActorAngle(tid))) {
		rng = GetActorProperty(tid, APROP_TARGETTID);
		SetActorProperty(DND_SUBORDINATE_TEMPTID, APROP_TARGETTID, rng);
		Thing_Hate(DND_SUBORDINATE_TEMPTID, rng, 4);
		SetActorAngle(DND_SUBORDINATE_TEMPTID, AngleToFace(DND_SUBORDINATE_TEMPTID, rng));
		GiveActorInventory(DND_SUBORDINATE_TEMPTID, "DnD_EliteException", 1);
		Thing_ChangeTID(DND_SUBORDINATE_TEMPTID, 0);
		ACS_NamedExecuteAlways("DnD Subordinate Spawn FX", 0, tid);
	}
}

Script "DnD Subordinate Spawn FX" (int tid) CLIENTSIDE {
	GiveActorInventory(tid, "SubordinateFXSpawner", 1);
}

str LegendaryMonsters[DND_MAX_LEGENDARY] = {
	"DreamingGod",
	"Torrasque",
	"Mordecqai",
	"GodSlayer",
	"Golgoth"
};

void SetupMonsterData() {
	MonsterData[MONSTER_ZOMBIEMAN].health = 20;
	MonsterData[MONSTER_ZOMBIEMAN].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_SHOTGUNNER].health = 30;
	MonsterData[MONSTER_SHOTGUNNER].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_CHAINGUNNER].health = 70;
	MonsterData[MONSTER_CHAINGUNNER].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_DEMON].health = 150;
	MonsterData[MONSTER_DEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SPECTRE].health = 150;
	MonsterData[MONSTER_SPECTRE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_IMP].health = 60;
	MonsterData[MONSTER_IMP].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_CACODEMON].health = 400;
	MonsterData[MONSTER_CACODEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_PAINELEMENTAL].health = 400;
	MonsterData[MONSTER_PAINELEMENTAL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_LOSTSOUL].health = 50;
	MonsterData[MONSTER_LOSTSOUL].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_REVENANT].health = 300;
	MonsterData[MONSTER_REVENANT].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_HELLKNIGHT].health = 500;
	MonsterData[MONSTER_HELLKNIGHT].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_BARON].health = 1000;
	MonsterData[MONSTER_BARON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_FATSO].health = 600;
	MonsterData[MONSTER_FATSO].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_SPIDER].health = 500;
	MonsterData[MONSTER_SPIDER].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_VILE].health = 700;
	MonsterData[MONSTER_VILE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_MASTERMIND].health = 3000;
	MonsterData[MONSTER_MASTERMIND].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_CYBERDEMON].health = 4000;
	MonsterData[MONSTER_CYBERDEMON].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_NAZI].health = 50;
	MonsterData[MONSTER_NAZI].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;

	// zombieman
	MonsterData[MONSTER_ZOMBIEMANGRAY].health = 40;
	MonsterData[MONSTER_ZOMBIEMANGRAY].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIERANGER].health = 45;
	MonsterData[MONSTER_ZOMBIERANGER].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIESMG].health = 35;
	MonsterData[MONSTER_ZOMBIESMG].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIERAPID].health = 30;
	MonsterData[MONSTER_ZOMBIERAPID].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEMARINE].health = 45;
	MonsterData[MONSTER_ZOMBIEMARINE].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIELOS].health = 45;
	MonsterData[MONSTER_ZOMBIELOS].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEPISTOL].health = 25;
	MonsterData[MONSTER_ZOMBIEPISTOL].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEQUAKE1].health = 55;
	MonsterData[MONSTER_ZOMBIEQUAKE1].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEHUNTER].health = 65;
	MonsterData[MONSTER_ZOMBIEHUNTER].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEPROPHET].health = 50;
	MonsterData[MONSTER_ZOMBIEPROPHET].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	
	// shotgunner
	MonsterData[MONSTER_SSGLOS].health = 50;
	MonsterData[MONSTER_SSGLOS].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZSPECSG].health = 50;
	MonsterData[MONSTER_ZSPECSG].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_SGLOS].health = 50;
	MonsterData[MONSTER_SGLOS].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_SAWEDOFF1].health = 50;
	MonsterData[MONSTER_SAWEDOFF1].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_SAWEDOFF2].health = 50;
	MonsterData[MONSTER_SAWEDOFF2].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ROGUE].health = 65;
	MonsterData[MONSTER_ROGUE].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEQUAKE2].health = 70;
	MonsterData[MONSTER_ZOMBIEQUAKE2].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIESSG].health = 75;
	MonsterData[MONSTER_ZOMBIESSG].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;

	// chaingunguy
	MonsterData[MONSTER_ZOMBIEQUAKE3].health = 85;
	MonsterData[MONSTER_ZOMBIEQUAKE3].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEMG].health = 85;
	MonsterData[MONSTER_ZOMBIEMG].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_INITIATE].health = 65;
	MonsterData[MONSTER_INITIATE].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_DOUBLEGUNNER].health = 100;
	MonsterData[MONSTER_DOUBLEGUNNER].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_CGMAJOR].health = 110;
	MonsterData[MONSTER_CGMAJOR].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_MRROBOT].health = 100;
	MonsterData[MONSTER_MRROBOT].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_ZSEC].health = 70;
	MonsterData[MONSTER_ZSEC].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZSPECMG].health = 60;
	MonsterData[MONSTER_ZSPECMG].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_PLASMAZOMBIE].health = 85;
	MonsterData[MONSTER_PLASMAZOMBIE].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_BERSERKERGUY].health = 150;
	MonsterData[MONSTER_BERSERKERGUY].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;

	// demon
	MonsterData[MONSTER_BLOODDEMON].health = 250;
	MonsterData[MONSTER_BLOODDEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_BLOODFIEND].health = 160;
	MonsterData[MONSTER_BLOODFIEND].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_CYBERFIEND].health = 300;
	MonsterData[MONSTER_CYBERFIEND].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_FLAMEDEMON].health = 200;
	MonsterData[MONSTER_FLAMEDEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_STONEIMP].health = 180;
	MonsterData[MONSTER_STONEIMP].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_SCAVENGER].health = 195;
	MonsterData[MONSTER_SCAVENGER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SOULEATER].health = 200;
	MonsterData[MONSTER_SOULEATER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_NHUMCIGN].health = 275;
	MonsterData[MONSTER_NHUMCIGN].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_STONEDEMON].health = 250;
	MonsterData[MONSTER_STONEDEMON].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_BRUTY].health = 240;
	MonsterData[MONSTER_BRUTY].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SATYR].health = 150;
	MonsterData[MONSTER_SATYR].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_EARTHGOLEM].health = 325;
	MonsterData[MONSTER_EARTHGOLEM].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_RAVAGER].health = 250;
	MonsterData[MONSTER_RAVAGER].flags = DND_MTYPE_DEMON_POW;	
	MonsterData[MONSTER_PUREBREDDEMON].health = 275;
	MonsterData[MONSTER_PUREBREDDEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SABRECLAW].health = 200;
	MonsterData[MONSTER_SABRECLAW].flags = DND_MTYPE_MAGICAL_POW;
	
	// spectre
	MonsterData[MONSTER_LURKER].health = 160;
	MonsterData[MONSTER_LURKER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_GRAVEDIGGER].health = 160;
	MonsterData[MONSTER_GRAVEDIGGER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DEVOURER].health = 175;
	MonsterData[MONSTER_DEVOURER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_NIGHTMAREDEMON].health = 150;
	MonsterData[MONSTER_NIGHTMAREDEMON].flags = DND_MTYPE_DEMON_POW;

	// imp
	MonsterData[MONSTER_DARKIMP1].health = 80;
	MonsterData[MONSTER_DARKIMP1].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_VOIDDARKIMP].health = 100;
	MonsterData[MONSTER_VOIDDARKIMP].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_NETHERDARKIMP].health = 100;
	MonsterData[MONSTER_NETHERDARKIMP].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DARKIMP2].health = 90;
	MonsterData[MONSTER_DARKIMP2].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_IMPABOM].health = 80;
	MonsterData[MONSTER_IMPABOM].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_STIMP].health = 55;
	MonsterData[MONSTER_STIMP].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_VOIDIMP].health = 65;
	MonsterData[MONSTER_VOIDIMP].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SOULHARVESTER].health = 100;
	MonsterData[MONSTER_SOULHARVESTER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_PYROIMP].health = 60;
	MonsterData[MONSTER_PYROIMP].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DEVIL].health = 85;
	MonsterData[MONSTER_DEVIL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DEVIL2].health = 125;
	MonsterData[MONSTER_DEVIL2].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_VULGAR].health = 70;
	MonsterData[MONSTER_VULGAR].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_UNDEADMAGE].health = 80;
	MonsterData[MONSTER_UNDEADMAGE].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_SHADOW].health = 85;
	MonsterData[MONSTER_SHADOW].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_REAVER].health = 90;
	MonsterData[MONSTER_REAVER].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_ROACH].health = 80;
	MonsterData[MONSTER_ROACH].flags = DND_MTYPE_DEMON_POW;

	// lost soul
	MonsterData[MONSTER_BABYCACO].health = 60;
	MonsterData[MONSTER_BABYCACO].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_ETHEREALSOUL].health = 60;
	MonsterData[MONSTER_ETHEREALSOUL].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_FLESHSPAWN].health = 50;
	MonsterData[MONSTER_FLESHSPAWN].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_GUARDIANCUBE].health = 75;
	MonsterData[MONSTER_GUARDIANCUBE].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_FORGOTTENONE].health = 40;
	MonsterData[MONSTER_FORGOTTENONE].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_HADESSPHERE].health = 35;
	MonsterData[MONSTER_HADESSPHERE].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_WATCHER].health = 100;
	MonsterData[MONSTER_WATCHER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DARKLICH_SPIRIT].health = 75;
	MonsterData[MONSTER_DARKLICH_SPIRIT].flags = DND_MTYPE_UNDEAD_POW;

	// cacodemon
	MonsterData[MONSTER_WEAKENER].health = 300;
	MonsterData[MONSTER_WEAKENER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SHADOWPRIEST].health = 250;
	MonsterData[MONSTER_SHADOWPRIEST].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_GRELL].health = 325;
	MonsterData[MONSTER_GRELL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DEATHWHISPERER].health = 500;
	MonsterData[MONSTER_DEATHWHISPERER].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_CACOLICH].health = 500;
	MonsterData[MONSTER_CACOLICH].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_INFERNO].health = 400;
	MonsterData[MONSTER_INFERNO].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_GUARDIAN].health = 275;
	MonsterData[MONSTER_GUARDIAN].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_ENHANCEDCACO].health = 300;
	MonsterData[MONSTER_ENHANCEDCACO].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_EARTHLICH].health = 425;
	MonsterData[MONSTER_EARTHLICH].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_WICKED].health = 300;
	MonsterData[MONSTER_WICKED].flags = DND_MTYPE_DEMON_POW;

	// pain ele
	MonsterData[MONSTER_BLOODLICH].health = 675;
	MonsterData[MONSTER_BLOODLICH].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_HADESELEMENTAL].health = 800;
	MonsterData[MONSTER_HADESELEMENTAL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_HELLARBITER].health = 700;
	MonsterData[MONSTER_HELLARBITER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DEFILER].health = 500;
	MonsterData[MONSTER_DEFILER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_TORTUREDSOUL].health = 550;
	MonsterData[MONSTER_TORTUREDSOUL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SHADOWDISCIPLE].health = 500;
	MonsterData[MONSTER_SHADOWDISCIPLE].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_SENTINEL].health = 750;
	MonsterData[MONSTER_SENTINEL].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_PHANTASM].health = 300;
	MonsterData[MONSTER_PHANTASM].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_WRAITH].health = 75;
	MonsterData[MONSTER_WRAITH].flags = DND_MTYPE_UNDEAD_POW;
	
	// revenant
	MonsterData[MONSTER_INCARNATE].health = 250;
	MonsterData[MONSTER_INCARNATE].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_BEAMREVENANT].health = 250;
	MonsterData[MONSTER_BEAMREVENANT].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_AXEKNIGHT].health = 250;
	MonsterData[MONSTER_AXEKNIGHT].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_WIDOWMAKER].health = 300;
	MonsterData[MONSTER_WIDOWMAKER].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_YETI].health = 300;
	MonsterData[MONSTER_YETI].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_SLUDGEGIANT].health = 280;
	MonsterData[MONSTER_SLUDGEGIANT].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_SLUDGEGIANT2].health = 140;
	MonsterData[MONSTER_SLUDGEGIANT2].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_SLUDGEGIANT3].health = 70;
	MonsterData[MONSTER_SLUDGEGIANT3].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_CADAVER].health = 350;
	MonsterData[MONSTER_CADAVER].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_DARKSERVANT].health = 275;
	MonsterData[MONSTER_DARKSERVANT].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_CRAWLER].health = 325;
	MonsterData[MONSTER_CRAWLER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_CYBORGSOLDIER].health = 200;
	MonsterData[MONSTER_CYBORGSOLDIER].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_DRAUGR].health = 350;
	MonsterData[MONSTER_DRAUGR].flags = DND_MTYPE_UNDEAD_POW;

	// hell knight
	MonsterData[MONSTER_BLOODSATYR].health = 550;
	MonsterData[MONSTER_BLOODSATYR].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_HELLWARRIOR].health = 400;
	MonsterData[MONSTER_HELLWARRIOR].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_HELLSFURY].health = 600;
	MonsterData[MONSTER_HELLSFURY].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_BLACKKNIGHT].health = 550;
	MonsterData[MONSTER_BLACKKNIGHT].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_ARCHON].health = 700;
	MonsterData[MONSTER_ARCHON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_WARLORD].health = 650;
	MonsterData[MONSTER_WARLORD].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SKULLWIZARD].health = 500;
	MonsterData[MONSTER_SKULLWIZARD].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_CYBORGWARRIOR].health = 650;
	MonsterData[MONSTER_CYBORGWARRIOR].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_SHADOWBEAST].health = 400;
	MonsterData[MONSTER_SHADOWBEAST].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_CHAOSSERPENT].health = 450;
	MonsterData[MONSTER_CHAOSSERPENT].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_MOONSATYR].health = 750;
	MonsterData[MONSTER_MOONSATYR].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_ICEGOLEM].health = 400;
	MonsterData[MONSTER_ICEGOLEM].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_PUTREFIER].health = 500;
	MonsterData[MONSTER_PUTREFIER].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_GLADIATOR].health = 450;
	MonsterData[MONSTER_GLADIATOR].flags = DND_MTYPE_DEMON_POW;

	// baron
	MonsterData[MONSTER_LAVADEMON].health = 1000;
	MonsterData[MONSTER_LAVADEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_LORDOFHERESY].health = 1100;
	MonsterData[MONSTER_LORDOFHERESY].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_BORMERETH].health = 1200;
	MonsterData[MONSTER_BORMERETH].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_BARBATOS].health = 1100;
	MonsterData[MONSTER_BARBATOS].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_BLOODSEEKER].health = 1000;
	MonsterData[MONSTER_BLOODSEEKER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SHADOWWIZARD].health = 900;
	MonsterData[MONSTER_SHADOWWIZARD].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_KJAROCH].health = 1650;
	MonsterData[MONSTER_KJAROCH].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_CYBRUISER].health = 1500;
	MonsterData[MONSTER_CYBRUISER].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_BRUISERDEMON].health = 1500;
	MonsterData[MONSTER_BRUISERDEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_MAGMASERPENT].health = 900;
	MonsterData[MONSTER_MAGMASERPENT].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DREADKNIGHT].health = 900;
	MonsterData[MONSTER_DREADKNIGHT].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_MAGMAGOLEM].health = 900;
	MonsterData[MONSTER_MAGMAGOLEM].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_JUDICATOR].health = 800;
	MonsterData[MONSTER_JUDICATOR].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_WARMASTER].health = 1250;
	MonsterData[MONSTER_WARMASTER].flags = DND_MTYPE_DEMON_POW;

	// fatso
	MonsterData[MONSTER_CORPULENT].health = 850;
	MonsterData[MONSTER_CORPULENT].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DAEDABUS].health = 850;
	MonsterData[MONSTER_DAEDABUS].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_PALADIN].health = 775;
	MonsterData[MONSTER_PALADIN].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_GAMON].health = 875;
	MonsterData[MONSTER_GAMON].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_MEPHISTO].health = 900;
	MonsterData[MONSTER_MEPHISTO].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_MAFIBUS].health = 1100;
	MonsterData[MONSTER_MAFIBUS].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_ICEFATSO].health = 650;
	MonsterData[MONSTER_ICEFATSO].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_ABOMINATION].health = 775;
	MonsterData[MONSTER_ABOMINATION].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_REDEEMER].health = 800;
	MonsterData[MONSTER_REDEEMER].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_GOLDGOLEM].health = 1000;
	MonsterData[MONSTER_GOLDGOLEM].flags = DND_MTYPE_MAGICAL_POW;

	// arachnotron
	MonsterData[MONSTER_FUSIONSPIDER].health = 500;
	MonsterData[MONSTER_FUSIONSPIDER].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_RAILARACHNOTRON].health = 450;
	MonsterData[MONSTER_RAILARACHNOTRON].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_HELLFORGESPIDER].health = 700;
	MonsterData[MONSTER_HELLFORGESPIDER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_VORE].health = 700;
	MonsterData[MONSTER_VORE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_BABYDEMOLISHER].health = 700;
	MonsterData[MONSTER_BABYDEMOLISHER].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_CHAINGUNGENERAL].health = 550;
	MonsterData[MONSTER_CHAINGUNGENERAL].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_CHAINGUNCOMMANDO].health = 700;
	MonsterData[MONSTER_CHAINGUNCOMMANDO].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_LEGIONNAIRE].health = 600;
	MonsterData[MONSTER_LEGIONNAIRE].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_MANTICORE].health = 650;
	MonsterData[MONSTER_MANTICORE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_OPHIDIAN].health = 700;
	MonsterData[MONSTER_OPHIDIAN].flags = DND_MTYPE_MAGICAL_POW;

	// vile
	MonsterData[MONSTER_DIABLOIST].health = 850;
	MonsterData[MONSTER_DIABLOIST].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_UNDEADPRIEST].health = 800;
	MonsterData[MONSTER_UNDEADPRIEST].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_UNDEADPRIESTGHOST].health = 100;
	MonsterData[MONSTER_UNDEADPRIESTGHOST].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_DEATHVILE].health = 800;
	MonsterData[MONSTER_DEATHVILE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_HIEROPHANT].health = 1250;
	MonsterData[MONSTER_HIEROPHANT].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_GURU].health = 800;
	MonsterData[MONSTER_GURU].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_DEATHKNIGHT].health = 900;
	MonsterData[MONSTER_DEATHKNIGHT].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_HORSHACKER].health = 900;
	MonsterData[MONSTER_HORSHACKER].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_DARKZEALOT].health = 750;
	MonsterData[MONSTER_DARKZEALOT].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_FLESHWIZARD].health = 625;
	MonsterData[MONSTER_FLESHWIZARD].flags = DND_MTYPE_DEMON_POW;

	// sm
	MonsterData[MONSTER_DEMOLISHER].health = 4000;
	MonsterData[MONSTER_DEMOLISHER].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_ARACHNOPHYTE].health = 3500;
	MonsterData[MONSTER_ARACHNOPHYTE].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_PSIONICQUEEN].health = 4000;
	MonsterData[MONSTER_PSIONICQUEEN].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_ANGELOFDEATH].health = 4000;
	MonsterData[MONSTER_ANGELOFDEATH].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_GOLDLICH].health = 4750;
	MonsterData[MONSTER_GOLDLICH].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_GOLDLICHFAKE].health = 300;
	MonsterData[MONSTER_GOLDLICHFAKE].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_IRONLICH].health = 4000;
	MonsterData[MONSTER_IRONLICH].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_SPIDEROVERLORD].health = 4000;
	MonsterData[MONSTER_SPIDEROVERLORD].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_DARKLICH].health = 4000;
	MonsterData[MONSTER_DARKLICH].flags = DND_MTYPE_MAGICAL_POW;

	// cyber
	MonsterData[MONSTER_CARDINAL].health = 4500;
	MonsterData[MONSTER_CARDINAL].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_TERMINATOR].health = 5750;
	MonsterData[MONSTER_TERMINATOR].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_THAMUZ].health = 4750;
	MonsterData[MONSTER_THAMUZ].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_AZAZEL].health = 5250;
	MonsterData[MONSTER_AZAZEL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_HELLSMITH].health = 6000;
	MonsterData[MONSTER_HELLSMITH].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_THANATOS].health = 6500;
	MonsterData[MONSTER_THANATOS].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_AVATAR].health = 5500;
	MonsterData[MONSTER_AVATAR].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_CERBERUS].health = 6000;
	MonsterData[MONSTER_CERBERUS].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DEATHWYVERN].health = 3500;
	MonsterData[MONSTER_DEATHWYVERN].flags = DND_MTYPE_UNDEAD_POW;

	// legendaries
	MonsterData[MONSTER_DREAMINGGOD].health = 20000;
	MonsterData[MONSTER_DREAMINGGOD].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_TORRASQUE].health = 16500;
	MonsterData[MONSTER_TORRASQUE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_MORDECQAI].health = 18500;
	MonsterData[MONSTER_MORDECQAI].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_GODSLAYER].health = 13500;
	MonsterData[MONSTER_GODSLAYER].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_GOLGOTH].health = 17500;
	MonsterData[MONSTER_GOLGOTH].flags = DND_MTYPE_DEMON_POW;

	// uniques
	MonsterData[MONSTER_TERON].health = 500;
	MonsterData[MONSTER_TERON].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_GANT].health = 800;
	MonsterData[MONSTER_GANT].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_BRONN].health = 1200;
	MonsterData[MONSTER_BRONN].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_VAAJ].health = 900;
	MonsterData[MONSTER_VAAJ].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_REMUS].health = 1500;
	MonsterData[MONSTER_REMUS].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_SSRATH].health = 1250;
	MonsterData[MONSTER_SSRATH].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_HOLLOWSHELL].health = 600;
	MonsterData[MONSTER_HOLLOWSHELL].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_OMNISIGHT].health = 1600;
	MonsterData[MONSTER_OMNISIGHT].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_CHEGOVAX].health = 1200;
	MonsterData[MONSTER_CHEGOVAX].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_ONIMUZ].health = 900;
	MonsterData[MONSTER_ONIMUZ].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_HARKIMONDE].health = 1750;
	MonsterData[MONSTER_HARKIMONDE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_LESHRAC].health = 2250;
	MonsterData[MONSTER_LESHRAC].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_LESHRACPOD].health = 200;
	MonsterData[MONSTER_LESHRACPOD].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_KRULL].health = 2000;
	MonsterData[MONSTER_KRULL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_THORAX].health = 1750;
	MonsterData[MONSTER_THORAX].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_ZRAVOG].health = 1800;
	MonsterData[MONSTER_ZRAVOG].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_ERYXIA].health = 7500;
	MonsterData[MONSTER_ERYXIA].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_ABAXOTH].health = 8500;
	MonsterData[MONSTER_ABAXOTH].flags = DND_MTYPE_DEMON_POW;

	// classics
	MonsterData[MONSTER_LOSTSOUL].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_PAINELEMENTAL].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_PAINELEMENTAL].trait_list[DND_SPLIT] = true;
	MonsterData[MONSTER_VILE].trait_list[DND_RESURRECT] = true;
	
	// zombieman
	MonsterData[MONSTER_ZOMBIEHUNTER].trait_list[DND_MOBILITY] = true;
	
	// shotgunner
	MonsterData[MONSTER_SGLOS].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_SSGLOS].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_ROGUE].trait_list[DND_MOBILITY] = true;
	
	// chaingunner
	MonsterData[MONSTER_INITIATE].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_MRROBOT].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_MRROBOT].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_BERSERKERGUY].trait_list[DND_RAGE] = true;
	
	// demon
	MonsterData[MONSTER_STONEIMP].trait_list[DND_STONECREATURE] = true;
	MonsterData[MONSTER_STONEDEMON].trait_list[DND_STONECREATURE] = true;
	MonsterData[MONSTER_STONEDEMON].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_BRUTY].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_SCAVENGER].trait_list[DND_RESURRECT] = true;
	MonsterData[MONSTER_NHUMCIGN].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_NHUMCIGN].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_EARTHGOLEM].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_EARTHGOLEM].trait_list[DND_EARTHCREATURE] = true;
	MonsterData[MONSTER_PUREBREDDEMON].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_SABRECLAW].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_SABRECLAW].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_SABRECLAW].trait_list[DND_BULLET_RESIST] = true;
	
	// spectre
	MonsterData[MONSTER_NIGHTMAREDEMON].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_GRAVEDIGGER].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_DEVOURER].trait_list[DND_MOBILITY] = true;
	
	// imp
	MonsterData[MONSTER_DARKIMP2].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_SOULHARVESTER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_PYROIMP].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_VULGAR].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_UNDEADMAGE].trait_list[DND_RESURRECT] = true;
	MonsterData[MONSTER_REAVER].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_REAVER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_ROACH].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_ROACH].trait_list[DND_HEAL] = true;
	
	// lost soul
	MonsterData[MONSTER_FLESHSPAWN].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_ETHEREALSOUL].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_ETHEREALSOUL].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_GUARDIANCUBE].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_FORGOTTENONE].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_FORGOTTENONE].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_WATCHER].trait_list[DND_PIERCE] = true;
	
	// caco
	MonsterData[MONSTER_CACOLICH].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_DEATHWHISPERER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_DEATHWHISPERER].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_DEATHWHISPERER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_EARTHLICH].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_EARTHLICH].trait_list[DND_EARTHCREATURE] = true;
	MonsterData[MONSTER_INFERNO].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_INFERNO].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_GRELL].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_SHADOWPRIEST].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_SHADOWPRIEST].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_GUARDIAN].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_WICKED].trait_list[DND_MAGIC_RESIST] = true;
	MonsterData[MONSTER_WICKED].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_WICKED].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_WICKED].trait_list[DND_HOMING] = true;
	
	// pain
	MonsterData[MONSTER_TORTUREDSOUL].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_TORTUREDSOUL].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_DEFILER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_DEFILER].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_HADESELEMENTAL].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_HADESELEMENTAL].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_HADESELEMENTAL].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_HADESSPHERE].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_SENTINEL].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_SENTINEL].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_BLOODLICH].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_BLOODLICH].trait_list[DND_CURSE] = true;
	MonsterData[MONSTER_BLOODLICH].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_WRAITH].trait_list[DND_RESURRECT] = true;
	MonsterData[MONSTER_SHADOWDISCIPLE].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_SHADOWDISCIPLE].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_SHADOWDISCIPLE].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_HELLARBITER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_HELLARBITER].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_HELLARBITER].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_PHANTASM].trait_list[DND_SUMMONED] = true;
	MonsterData[MONSTER_PHANTASM].trait_list[DND_ETHEREAL] = true;
	
	// rev
	MonsterData[MONSTER_INCARNATE].trait_list[DND_RAISE] = true;
	MonsterData[MONSTER_CADAVER].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_CADAVER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_WIDOWMAKER].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_WIDOWMAKER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_AXEKNIGHT].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_YETI].trait_list[DND_ICECREATURE] = true;
	MonsterData[MONSTER_YETI].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_SLUDGEGIANT].trait_list[DND_SPLIT] = true;
	MonsterData[MONSTER_SLUDGEGIANT].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_SLUDGEGIANT2].trait_list[DND_SPLIT] = true;
	MonsterData[MONSTER_SLUDGEGIANT2].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_SLUDGEGIANT2].trait_list[DND_SUMMONED] = true;
	MonsterData[MONSTER_SLUDGEGIANT3].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_SLUDGEGIANT3].trait_list[DND_SUMMONED] = true;
	MonsterData[MONSTER_DARKSERVANT].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_DARKSERVANT].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_DARKSERVANT].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_CRAWLER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_CRAWLER].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_CYBORGSOLDIER].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_CYBORGSOLDIER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_DRAUGR].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_DRAUGR].trait_list[DND_MOBILITY] = true;
	
	// hk
	MonsterData[MONSTER_BLOODSATYR].trait_list[DND_VAMPIRISM] = true;
	MonsterData[MONSTER_HELLWARRIOR].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_HELLSFURY].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_WARLORD].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_SHADOWBEAST].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_BLACKKNIGHT].trait_list[DND_ICE_WEAKNESS] = true;
	MonsterData[MONSTER_BLACKKNIGHT].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_BLACKKNIGHT].trait_list[DND_EXPLOSIVE_RESIST] = true;
	MonsterData[MONSTER_BLACKKNIGHT].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_SKULLWIZARD].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_SKULLWIZARD].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_CYBORGWARRIOR].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_CYBORGWARRIOR].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_CYBORGWARRIOR].trait_list[DND_EXPLOSIVE_RESIST] = true;
	MonsterData[MONSTER_MOONSATYR].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_MOONSATYR].trait_list[DND_ENERGY_RESIST] = true;
	MonsterData[MONSTER_ICEGOLEM].trait_list[DND_HEAL] = true;
	MonsterData[MONSTER_ICEGOLEM].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_ICEGOLEM].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	MonsterData[MONSTER_PUTREFIER].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	MonsterData[MONSTER_PUTREFIER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_PUTREFIER].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_PUTREFIER].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_GLADIATOR].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_GLADIATOR].trait_list[DND_ICE_WEAKNESS] = true;
	
	// baron
	MonsterData[MONSTER_LAVADEMON].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_LAVADEMON].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_CYBRUISER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_BRUISERDEMON].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_BORMERETH].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_BORMERETH].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_BORMERETH].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_BLOODSEEKER].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_BARBATOS].trait_list[DND_ELEMENTAL_WEAKNESS] = true;
	MonsterData[MONSTER_BARBATOS].trait_list[DND_EXPLOSIVE_RESIST] = true;
	MonsterData[MONSTER_SHADOWWIZARD].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_SHADOWWIZARD].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_KJAROCH].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_KJAROCH].trait_list[DND_EARTHCREATURE] = true;
	MonsterData[MONSTER_MAGMASERPENT].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_DREADKNIGHT].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_DREADKNIGHT].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_DREADKNIGHT].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_DREADKNIGHT].trait_list[DND_MAGIC_RESIST] = true;
	MonsterData[MONSTER_MAGMAGOLEM].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_MAGMAGOLEM].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_MAGMAGOLEM].trait_list[DND_EXPLOSIVE_RESIST] = true;
	MonsterData[MONSTER_JUDICATOR].trait_list[DND_BULLET_IMMUNE] = true;
	MonsterData[MONSTER_WARMASTER].trait_list[DND_HEAL] = true;
	MonsterData[MONSTER_WARMASTER].trait_list[DND_RAGE] = true;
	
	// fatty
	MonsterData[MONSTER_CORPULENT].trait_list[DND_ELEMENTAL_WEAKNESS] = true;
	MonsterData[MONSTER_DAEDABUS].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_ICEFATSO].trait_list[DND_ICECREATURE] = true;
	MonsterData[MONSTER_MAFIBUS].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_PALADIN].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_PALADIN].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_PALADIN].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_PALADIN].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_GAMON].trait_list[DND_ENERGY_RESIST] = true;
	MonsterData[MONSTER_GAMON].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_GAMON].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_MEPHISTO].trait_list[DND_EXPLOSIVE_RESIST] = true;
	MonsterData[MONSTER_ABOMINATION].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_REDEEMER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_REDEEMER].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_GOLDGOLEM].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_GOLDGOLEM].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	MonsterData[MONSTER_GOLDGOLEM].trait_list[DND_MAGIC_RESIST] = true;
	
	// arachno
	MonsterData[MONSTER_BABYDEMOLISHER].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_VORE].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_VORE].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_CHAINGUNCOMMANDO].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_LEGIONNAIRE].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_OPHIDIAN].trait_list[DND_FIRECREATURE] = true;
	
	// vile
	MonsterData[MONSTER_DIABLOIST].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_DEATHVILE].trait_list[DND_RESURRECT] = true;
	MonsterData[MONSTER_DEATHVILE].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_HIEROPHANT].trait_list[DND_CURSE] = true;
	MonsterData[MONSTER_HIEROPHANT].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_HIEROPHANT].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_HIEROPHANT].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_DEATHKNIGHT].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_DEATHKNIGHT].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_HORSHACKER].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_HORSHACKER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_UNDEADPRIEST].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_UNDEADPRIEST].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_UNDEADPRIEST].trait_list[DND_RESURRECT] = true;
	MonsterData[MONSTER_GURU].trait_list[DND_CURSE] = true;
	MonsterData[MONSTER_GURU].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_DARKZEALOT].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_DARKZEALOT].trait_list[DND_HEAL] = true;
	MonsterData[MONSTER_DARKZEALOT].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_FLESHWIZARD].trait_list[DND_MAGIC_IMMUNE] = true;
	MonsterData[MONSTER_FLESHWIZARD].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_FLESHWIZARD].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_FLESHWIZARD].trait_list[DND_DEATH] = true;
	
	// sm
	MonsterData[MONSTER_ARACHNOPHYTE].trait_list[DND_EXPLOSIVE_RESIST] = true;
	MonsterData[MONSTER_PSIONICQUEEN].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_PSIONICQUEEN].trait_list[DND_EXPLOSIVE_RESIST] = true;
	MonsterData[MONSTER_PSIONICQUEEN].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_PSIONICQUEEN].trait_list[DND_ENERGY_IMMUNE] = true;
	MonsterData[MONSTER_ANGELOFDEATH].trait_list[DND_EXPLOSIVE_NONE] = true;
	MonsterData[MONSTER_ANGELOFDEATH].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	MonsterData[MONSTER_ANGELOFDEATH].trait_list[DND_MAGIC_RESIST] = true;
	MonsterData[MONSTER_GOLDLICH].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_GOLDLICH].trait_list[DND_EXPLOSIVE_NONE] = true;
	MonsterData[MONSTER_GOLDLICH].trait_list[DND_SPLIT] = true;
	MonsterData[MONSTER_GOLDLICH].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_IRONLICH].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_IRONLICH].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_IRONLICH].trait_list[DND_CURSE] = true;
	MonsterData[MONSTER_IRONLICH].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_IRONLICH].trait_list[DND_EXPLOSIVE_NONE] = true;
	MonsterData[MONSTER_IRONLICH].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_SPIDEROVERLORD].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_SPIDEROVERLORD].trait_list[DND_ENERGY_IMMUNE] = true;
	MonsterData[MONSTER_SPIDEROVERLORD].trait_list[DND_EXPLOSIVE_RESIST] = true;
	MonsterData[MONSTER_SPIDEROVERLORD].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_DARKLICH].trait_list[DND_MAGIC_IMMUNE] = true;
	MonsterData[MONSTER_DARKLICH].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_DARKLICH].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_DARKLICH].trait_list[DND_EXPLOSIVE_NONE] = true;
	
	// cyber
	MonsterData[MONSTER_CARDINAL].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_TERMINATOR].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_TERMINATOR].trait_list[DND_ENERGY_RESIST] = true;
	MonsterData[MONSTER_THAMUZ].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_THAMUZ].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_THAMUZ].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_AZAZEL].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_AZAZEL].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_AZAZEL].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_AZAZEL].trait_list[DND_MAGIC_IMMUNE] = true;
	MonsterData[MONSTER_HELLSMITH].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_HELLSMITH].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_AVATAR].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	MonsterData[MONSTER_AVATAR].trait_list[DND_MAGIC_RESIST] = true;
	MonsterData[MONSTER_AVATAR].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_AVATAR].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_THANATOS].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_THANATOS].trait_list[DND_EXPLOSIVE_NONE] = true;
	MonsterData[MONSTER_CERBERUS].trait_list[DND_MAGIC_IMMUNE] = true;
	MonsterData[MONSTER_CERBERUS].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_DEATHWYVERN].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_DEATHWYVERN].trait_list[DND_MOBILITY] = true;
	
	// legendary monsters
	MonsterData[MONSTER_DREAMINGGOD].trait_list[DND_MAGIC_IMMUNE] = true;
	MonsterData[MONSTER_DREAMINGGOD].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	MonsterData[MONSTER_DREAMINGGOD].trait_list[DND_LEGENDARY] = true;
	
	MonsterData[MONSTER_TORRASQUE].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_TORRASQUE].trait_list[DND_EXPLOSIVE_RESIST] = true;
	MonsterData[MONSTER_TORRASQUE].trait_list[DND_ENERGY_RESIST] = true;
	MonsterData[MONSTER_TORRASQUE].trait_list[DND_LEGENDARY] = true;
	
	MonsterData[MONSTER_MORDECQAI].trait_list[DND_EXPLOSIVE_NONE] = true;
	MonsterData[MONSTER_MORDECQAI].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_MORDECQAI].trait_list[DND_LEGENDARY] = true;
	
	MonsterData[MONSTER_GODSLAYER].trait_list[DND_BULLET_IMMUNE] = true;
	MonsterData[MONSTER_GODSLAYER].trait_list[DND_ENERGY_RESIST] = true;
	MonsterData[MONSTER_GODSLAYER].trait_list[DND_LEGENDARY] = true;
	
	MonsterData[MONSTER_GOLGOTH].trait_list[DND_EXPLOSIVE_NONE] = true;
	MonsterData[MONSTER_GOLGOTH].trait_list[DND_MAGIC_RESIST] = true;
	MonsterData[MONSTER_GOLGOTH].trait_list[DND_LEGENDARY] = true;
	
	// unique monsters
	MonsterData[MONSTER_TERON].trait_list[DND_FORTIFIED] = true;
	
	MonsterData[MONSTER_GANT].trait_list[DND_FORTIFIED] = true;
	
	MonsterData[MONSTER_BRONN].trait_list[DND_FORTIFIED] = true;
	
	MonsterData[MONSTER_VAAJ].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_VAAJ].trait_list[DND_ARMORPEN] = true;
	
	MonsterData[MONSTER_REMUS].trait_list[DND_TELEPORT] = true;
	
	MonsterData[MONSTER_SSRATH].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_SSRATH].trait_list[DND_CURSE] = true;
	
	MonsterData[MONSTER_OMNISIGHT].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_OMNISIGHT].trait_list[DND_MINIONS] = true;
	
	MonsterData[MONSTER_CHEGOVAX].trait_list[DND_FORTIFIED] = true;
	MonsterData[MONSTER_CHEGOVAX].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_CHEGOVAX].trait_list[DND_BULLET_IMMUNE] = true;
	
	MonsterData[MONSTER_ONIMUZ].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_ONIMUZ].trait_list[DND_HOMING] = true;
	
	MonsterData[MONSTER_HARKIMONDE].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_HARKIMONDE].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_HARKIMONDE].trait_list[DND_HOMING] = true;
	
	MonsterData[MONSTER_LESHRAC].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_LESHRAC].trait_list[DND_MINIONS] = true;
	
	MonsterData[MONSTER_KRULL].trait_list[DND_FORTIFIED] = true;
	MonsterData[MONSTER_KRULL].trait_list[DND_EXPLOSIVE_RESIST] = true;
	
	MonsterData[MONSTER_THORAX].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_THORAX].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_THORAX].trait_list[DND_EXPLOSIVE_RESIST] = true;
	
	MonsterData[MONSTER_ZRAVOG].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_ZRAVOG].trait_list[DND_CURSE] = true;
	MonsterData[MONSTER_ZRAVOG].trait_list[DND_MAGIC_IMMUNE] = true;
	
	MonsterData[MONSTER_ERYXIA].trait_list[DND_CURSE] = true;
	MonsterData[MONSTER_ERYXIA].trait_list[DND_FORTIFIED] = true;
	MonsterData[MONSTER_ERYXIA].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_FORTIFIED] = true;
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_AGGRESSIVE] = true;
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_MAGIC_RESIST] = true;
}

void SetupMonsterWeights() {
	// zombieman
	int id = 0;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE1;

	id = 0;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_WOLFENSS][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_WOLFENSS][id++] = DND_MWEIGHT_ENDMARKER;
}

Script "DnD Setup Monster Data" OPEN {
	if(!isSetupComplete(SETUP_STATE1, SETUP_MONSTERS)) {
		SetupMonsterData();
		Delay(const:15);
		SetupMonsterWeights();
		SetupComplete(SETUP_STATE1, SETUP_MONSTERS);
	}
}

Script "DnD Setup Monster Data CS" OPEN CLIENTSIDE {
	if(!isSetupComplete(SETUP_STATE1, SETUP_MONSTERS)) {
		Delay(const:20);
		SetupMonsterData();
		SetupComplete(SETUP_STATE1, SETUP_MONSTERS);
	}
}

// do not send tid here, send monster id (tid - DND_MONSTERTID_BEGIN)
void CreateMonsterAttachment(int tid, str actor_name, int xoff = 0, int yoff = 0, int zoff = 0, int angle = 0) {
	// base tid skip
	int sfx_id = 0;
	int m_id = tid - DND_MONSTERTID_BEGIN;
	int temp = MonsterAttachmentUsed[m_id];
	while(temp & 1) {
		temp >>= 1;
		++sfx_id;
	}
	MonsterAttachmentUsed[m_id] |= 1 << sfx_id;

	// offset to tid
	sfx_id += DND_MONSTER_ATTACHMENT_TID_BEGIN + m_id * DND_MAX_MONSTER_ATTACHMENTS;
	//Log(s:"sfx tid: ", d:sfx_id, s: " for monster tid: ", d:tid);
	SpawnForced(actor_name, GetActorX(tid) + xoff, GetActorY(tid) + yoff, GetActorZ(tid) + zoff, sfx_id, angle);

	// setup the attachment
	SetActivator(sfx_id);
	SetPointer(AAPTR_TARGET, tid);
	SetActorProperty(sfx_id, APROP_TARGETTID, tid);

	// radius and other things
	SetActorProperty(sfx_id, APROP_MASS, zoff >> 16);
	SetActorProperty(sfx_id, APROP_SCORE, xoff >> 16);

	SetActivator(tid);
}

// When a monster is killed this is called to do cleanup
void DisposeAttachments(int m_id) {
	// if theres any attachment
	if(MonsterAttachmentUsed[m_id]) {
		int count = 0;
		int base = DND_MONSTER_ATTACHMENT_TID_BEGIN + m_id * DND_MAX_MONSTER_ATTACHMENTS;
		while(MonsterAttachmentUsed[m_id]) {
			if(MonsterAttachmentUsed[m_id] & 1)
				SetActorState(count + base, "Disappear");

			MonsterAttachmentUsed[m_id] >>= 1;
			++count;
		}
	}
}

#endif
