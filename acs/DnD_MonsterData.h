#ifndef DND_MONSTERDATA_IN
#define DND_MONSTERDATA_IN

#include "DnD_Common.h"
#include "DnD_EliteInfo.h"

#define DND_CUSTOMMONSTER_ID 65536
#define DND_MONSTERMASS_SCALE 20 // 20% per level

// note: old formula was multiplicative and multiplied by 3 at level 50 onwards and by 9 from 75 onwards. So according to it, at level 100 a monster would have 3600% increased hp (400% from level, x9 from threshold)
// so our new formula will acommodate for this --- multiplied x^2 factor by 10 it seems to be good
int GetMonsterHPScaling(int m_id, int level) {
	// new formula: x^2 * 0.33 + 5x, when x >= 25, where x is level - 1.
	// else 0.125 x^2 + 0.21 x, when x < 25, which allows for much slower scaling at <25
	int res = 0;
	if(level < 25)
		res = 5 * level * level / 4 + 21 * level / 10;
	else
		res = (33 * level * level) / 100 + 5 * level;
	
	// big bosses have higher scaling than other monsters -- since we reach much higher values than before I decided to go ahead and reduce the big boss scaling here
	if(IsUniqueBossMonster(m_id))
		res *= 1 + (level / 33);

	int cvs = GetCVar("dnd_monster_hpscalepercent");
	if(cvs)
		res = res * (100 + cvs) / 100;
		
	return res;
}

int GetMonsterDMGScaling(int m_id, int level) {
	// over the old formula of 4x, this provides 500% damage at lvl 100 instead of 400%
	int res = level * level / 25 + level;
	
	// unique bosses have additional damage multiplier per level -- x^2 * 0.01667 + x
	if(IsUniqueBossMonster(m_id))
		res = res * (100 + (level * level) / 60 + level) / 100;

	int cvs = GetCVar("dnd_monster_dmgscalepercent");
	if(cvs)
		res = res * (100 + cvs) / 100;
	
	return res;
}

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
	bool isElite;
	bool hasTrait;									// used by clients mostly -- do we have traits
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

// this is the base exp value you get for killing a monster of this class, where all bonuses will be applied to
// shifted right by 16 it returns the droprate multiplier
int GetMonsterClassBonus(int class) {
	switch(class) {
		case MONSTERCLASS_ZOMBIEMAN:
		return 10 | (15 << 16);
		
		case MONSTERCLASS_SHOTGUNGUY:
		return 15 | (20 << 16);
		
		case MONSTERCLASS_CHAINGUNGUY:
		return 24 | (28 << 16);
		
		case MONSTERCLASS_DEMON:
		return 32 | (30 << 16);
		
		case MONSTERCLASS_SPECTRE:
		return 35 | (30 << 16);
		
		case MONSTERCLASS_IMP:
		return 20 | (25 << 16);
		
		case MONSTERCLASS_CACODEMON:
		return 75 | (48 << 16);
		
		case MONSTERCLASS_PAINELEMENTAL:
		return 90 | (56 << 16);
		
		case MONSTERCLASS_LOSTSOUL:
		return 40 | (20 << 16);
		
		case MONSTERCLASS_REVENANT:
		return 55 | (36 << 16);
		
		case MONSTERCLASS_HELLKNIGHT:
		return 80 | (48 << 16);
		
		case MONSTERCLASS_BARON:
		return 135 | (72 << 16);
		
		case MONSTERCLASS_FATSO:
		return 130 | (72 << 16);
		
		case MONSTERCLASS_ARACHNOTRON:
		return 120 | (72 << 16);
		
		case MONSTERCLASS_ARCHVILE:
		return 150 | (90 << 16);
		
		case MONSTERCLASS_SPIDERMASTERMIND:
		return 1000 | (115 << 16);
		
		case MONSTERCLASS_CYBERDEMON:
		return 1750 | (125 << 16);
		
		case MONSTERCLASS_WOLFENSS:
		return 15 | (15 << 16);
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
#define DND_CUSTOM_DEMON_END MONSTER_PUREBREDDEMON

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
#define DND_CUSTOM_ARACHNO_END MONSTER_MANTICORE

#define DND_CUSTOM_VILE_BEGIN MONSTER_DIABLOIST
#define DND_CUSTOM_VILE_END MONSTER_FLESHWIZARD

#define DND_CUSTOM_SM_BEGIN MONSTER_DEMOLISHER
#define DND_CUSTOM_SM_END MONSTER_DARKLICH

#define DND_CUSTOM_CYBER_BEGIN MONSTER_CARDINAL
#define DND_CUSTOM_CYBER_END MONSTER_CERBERUS

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

#define DND_ELITE_GAINBONUS 25 // 25%
#define DND_CREDITGAIN_FACTOR 3 // divides the regular gain by 3

int GetMonsterLevelDroprateBonus(int lvl) {
	// piecewise function so the early 25 levels increase sharper, then mid 25 are slower and the later 25 are a bit sharper again
	if(lvl <= 25)
		return 4 * lvl;
	else if(lvl <= 50)
		return lvl * lvl / 100 + 3 * lvl + 19;
	return lvl * lvl / 150 + 184;
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
	
	// per lvl we get base 15%, then 25% per lvl if monster is elite
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
		
		MonsterProperties[m_id].rarity_boost = MonsterProperties[m_id].droprate / 2;
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
}

// this is put as a seperate function because 
void HandlePostInitTraits(int m_id, int id, int rarity = DND_MWEIGHT_COMMON) {
	if(MonsterProperties[m_id].trait_list[DND_FORTIFIED]) {
		// full fortify exceptions
		if(id != MONSTER_TERON && id != MONSTER_CHEGOVAX)
			SetInventory("MonsterFortifyCount", MonsterProperties[m_id].maxhp * DND_FORTIFY_AMOUNT / 10);
		else
			SetInventory("MonsterFortifyCount", MonsterProperties[m_id].maxhp);
	}
	
	// calculate the gains multiplier -- this is the safest place to do as most of monster data is now known by this point, like level etc.
	// all unique monsters have highest possible rarity
	if(rarity == -1) {
		if(!IsUniqueMonster(id))
			rarity = GetMonsterRarity(m_id);
		else
			rarity = DND_MWEIGHT_EPIC;
	}
		
	//printbold(s:"rarity ", d:rarity, s: " ", d:MonsterProperties[m_id].class, s: " ", d:MonsterProperties[m_id].id);
	
	MonsterProperties[m_id].rarity = rarity;
	
	CalculateMonsterGainMult(m_id, rarity);
}

// this is only used in revive of monsters by itself
void HandleSpecialTraits(int mid, int id) {
	HandlePreInitTraits(mid, id);
	HandlePostInitTraits(mid, id);
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
		
	if(monsterid >= LEGENDARY_START)
		MonsterProperties[tid].trait_list[DND_LEGENDARY] |= true;
	
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

monster_data_T MonsterData[DND_LASTMONSTER_INDEX + 1] = {
	{ 20, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW	 			},//DND_ZOMBIEMANID,
	{ 30, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_SHOTGUNNERID,
	{ 70, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW	 			},//DND_CHAINGUNGUYID,
	{ 150, 			DND_MTYPE_DEMON_POW										},//DND_DEMONID,
	{ 150, 			DND_MTYPE_DEMON_POW										},//DND_SPECTREID,
	{ 60, 			DND_MTYPE_DEMON_POW										},//DND_IMPID,
	{ 400, 			DND_MTYPE_DEMON_POW										},//DND_CACODEMONID,
	{ 400, 			DND_MTYPE_DEMON_POW										},//DND_PAINELEMENTALID,
	{ 50, 			DND_MTYPE_UNDEAD_POW									},//DND_LOSTSOULID,
	{ 300, 			DND_MTYPE_UNDEAD_POW									},//DND_REVENANTID,
	{ 500, 			DND_MTYPE_DEMON_POW										},//DND_HELLKNIGHTID,
	{ 1000, 		DND_MTYPE_DEMON_POW										},//DND_BARONID,
	{ 600, 			DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_FATSOID,
	{ 500, 			DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_ARACHNOTRONID,
	{ 700, 			DND_MTYPE_DEMON_POW										},//DND_ARCHVILEID,
	{ 3000, 		DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_SPIDERMASTERMINDID,
	{ 4000, 		DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_CYBERDEMONID,
	{ 50, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_NAZIID,	
	
	/// Zombieman
	{ 40, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZOMBIEGRAY,
	{ 45, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZOMBIERANGER,
	{ 35, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZOMBIESMG,
	{ 30, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZOMBIERAPID,
	{ 45, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZOMBIEMARINE,
    { 45, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZOMBIELOS,
    { 25, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZOMBIEPISTOL,
    { 55, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZOMBIEQUAKE1,
    { 65, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZOMBIEHUNTER,
    { 50, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_PROPHET,

	// Shotgunner
    { 50, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW		 		},//DND_SSGLOS,
    { 50, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZSPECSG,
    { 50, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_SGLOS,
    { 50, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_SAWEDOFF1,
    { 50, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_SAWEDOFF2,
    { 65, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ROGUE,
    { 70, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZOMBIEQUAKE2,
    { 75, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZOMBIESSG,

	// Chaingunner
    { 85, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZOMBIEQUAKE3,
    { 85, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZOMBIEMG,
    { 65, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_INITIATE,
    { 100, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_DOUBLEGUNNER,
    { 110, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_CGMAJOR,
    { 100, 			DND_MTYPE_ROBOTIC_POW									},//DND_MRROBOT,
    { 70, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZSEC,
    { 60,			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_ZSPECMG,
	{ 85, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_PLASMAZOMBIE,
	{ 150, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_BERSERKERGUY,

	// Demon
    { 250, 			DND_MTYPE_DEMON_POW										},//DND_BLOODDEMON,
    { 160, 			DND_MTYPE_DEMON_POW										},//DND_BLOODFIEND,
    { 300, 			DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_CYBERFIEND,
    { 200, 			DND_MTYPE_DEMON_POW										},//DND_FLAMEDEMON,
    { 180, 			DND_MTYPE_ROBOTIC_POW									},//DND_STONEIMP,
    { 195, 			DND_MTYPE_DEMON_POW										},//DND_SCAVENGER,
    { 200, 			DND_MTYPE_DEMON_POW										},//DND_SOULEATER,
    { 275, 			DND_MTYPE_DEMON_POW										},//DND_NHUMCIGN,
    { 250, 			DND_MTYPE_ROBOTIC_POW									},//DND_STONEDEMON,
    { 240, 			DND_MTYPE_DEMON_POW										},//DND_BRUTY,
	{ 150, 			DND_MTYPE_DEMON_POW										},//DND_SATYR,
	{ 325, 			DND_MTYPE_MAGICAL_POW									},//DND_EARTHGOLEM,
	{ 250, 			DND_MTYPE_DEMON_POW										},//DND_RAVAGER,
	{ 275, 			DND_MTYPE_DEMON_POW										},//DND_PUREBREDDEMON,

	// Spectre
    { 160, 			DND_MTYPE_DEMON_POW										},//DND_LURKER,
    { 160, 			DND_MTYPE_DEMON_POW										},//DND_GRAVEDIGGER,
    { 175, 			DND_MTYPE_DEMON_POW										},//DND_DEVOURER,
    { 150, 			DND_MTYPE_DEMON_POW										},//DND_NIGHTMAREDEMON,
	
	// Imp
    { 80, 			DND_MTYPE_DEMON_POW										},//DND_DARKIMP1,
    { 100, 			DND_MTYPE_DEMON_POW										},//DND_VOIDDARKIMP,
    { 100, 			DND_MTYPE_DEMON_POW										},//DND_NETHERDARKIMP,
    { 90, 			DND_MTYPE_DEMON_POW										},//DND_DARKIMP2,
    { 80, 			DND_MTYPE_DEMON_POW										},//DND_IMPABOM,
    { 55, 			DND_MTYPE_DEMON_POW										},//DND_STIMP,
    { 65, 			DND_MTYPE_DEMON_POW										},//DND_VOIDIMP,
    { 100, 			DND_MTYPE_DEMON_POW										},//DND_SOULHARVESTER,
    { 60, 			DND_MTYPE_DEMON_POW										},//DND_PYROIMP,
    { 85, 			DND_MTYPE_DEMON_POW										},//DND_DEVIL,
	{ 125, 			DND_MTYPE_DEMON_POW										},//DND_DEVIL2,
    { 70, 			DND_MTYPE_DEMON_POW										},//DND_VULGAR,
    { 80, 			DND_MTYPE_UNDEAD_POW									},//DND_UNDEADMAGE,
	{ 85, 			DND_MTYPE_DEMON_POW										},//DND_SHADOW,
	{ 90,			DND_MTYPE_ROBOTIC_POW									},//DND_REAVER
	{ 80, 			DND_MTYPE_DEMON_POW										},//DND_ROACH,

	// Lost Soul
    { 60, 			DND_MTYPE_DEMON_POW										},//DND_BABYCACO,
    { 60, 			DND_MTYPE_UNDEAD_POW									},//DND_ETHEREALSOUL,
    { 50, 			DND_MTYPE_DEMON_POW										},//DND_FLESHSPAWN,
    { 75, 			DND_MTYPE_MAGICAL_POW									},//DND_GUARDIANCUBE,
    { 40, 			DND_MTYPE_UNDEAD_POW									},//DND_FORGOTTENONE,
	{ 35, 			DND_MTYPE_MAGICAL_POW									},//DND_HADESSPHERE, S
	{ 100, 			DND_MTYPE_DEMON_POW										},//DND_WATCHER,
	{ 75, 			DND_MTYPE_UNDEAD_POW									},//DND_DARKLICH_SPIRIT, S

	// Cacodemon
    { 300, 			DND_MTYPE_DEMON_POW										},//DND_WEAKENER,
    { 250, 			DND_MTYPE_MAGICAL_POW									},//DND_SHADOWPRIEST,
    { 325, 			DND_MTYPE_DEMON_POW										},//DND_GRELL,
    { 500, 			DND_MTYPE_UNDEAD_POW									},//DND_DEATHWHISPERER,
    { 500, 			DND_MTYPE_UNDEAD_POW									},//DND_CACOLICH,
    { 400, 			DND_MTYPE_DEMON_POW										},//DND_INFERNO,
    { 275, 			DND_MTYPE_MAGICAL_POW									},//DND_GUARDIAN,
    { 300, 			DND_MTYPE_DEMON_POW										},//DND_ENHANCEDCACO,
    { 425, 			DND_MTYPE_MAGICAL_POW									},//DND_EARTHLICH,
	{ 300, 			DND_MTYPE_DEMON_POW										},//DND_WICKED,

	// Pain E.
	{ 675, 			DND_MTYPE_MAGICAL_POW									},//DND_BLOODLICH,
	{ 800, 			DND_MTYPE_DEMON_POW										},//DND_HADESELEMENTAL,
	{ 700, 			DND_MTYPE_DEMON_POW										},//DND_HELLARBITER,
	{ 500, 			DND_MTYPE_DEMON_POW										},//DND_DEFILER,
	{ 550, 			DND_MTYPE_DEMON_POW										},//DND_TORTUREDSOUL,
	{ 500, 			DND_MTYPE_MAGICAL_POW									},//DND_SHADOWDISCIPLE,
	{ 750, 			DND_MTYPE_MAGICAL_POW									},//DND_SENTINEL,
	{ 300,			DND_MTYPE_MAGICAL_POW									},//DND_PHANTASM S
	{ 75, 			DND_MTYPE_UNDEAD_POW									},//DND_WRAITH, S

	// Revenant
	{ 250, 			DND_MTYPE_UNDEAD_POW									},//DND_INCARNATE,
	{ 250, 			DND_MTYPE_UNDEAD_POW									},//DND_BEAMREVENANT,
	{ 250, 			DND_MTYPE_UNDEAD_POW									},//DND_AXEKNIGHT,
	{ 300, 			DND_MTYPE_UNDEAD_POW									},//DND_WIDOWMAKER,
	{ 350, 			DND_MTYPE_MAGICAL_POW									},//DND_YETI,
	{ 280, 			DND_MTYPE_MAGICAL_POW									},//DND_SLUDGEGIANT,
	{ 140, 			DND_MTYPE_MAGICAL_POW									},//DND_SLUDGEGIANT2, S
	{ 70, 			DND_MTYPE_MAGICAL_POW									},//DND_SLUDGEGIANT3, S
	{ 350, 			DND_MTYPE_UNDEAD_POW									},//DND_CADAVER,
	{ 275, 			DND_MTYPE_MAGICAL_POW									},//DND_DARKSERVANT,
	{ 325, 			DND_MTYPE_DEMON_POW										},//DND_CRAWLER,
	{ 200, 			DND_MTYPE_ROBOTIC_POW									},//DND_CYBORGSOLDIER,
	{ 350, 			DND_MTYPE_UNDEAD_POW									},//DND_DRAUGR,

	// Hell Knight
	{ 550, 			DND_MTYPE_DEMON_POW										},//DND_BLOODSATYR,
	{ 400, 			DND_MTYPE_DEMON_POW										},//DND_HELLWARRIOR,
	{ 600, 			DND_MTYPE_DEMON_POW										},//DND_HELLSFURY,
	{ 550, 			DND_MTYPE_UNDEAD_POW									},//DND_BLACKKNIGHT,
	{ 700, 			DND_MTYPE_DEMON_POW										},//DND_ARCHON,
	{ 650, 			DND_MTYPE_DEMON_POW										},//DND_WARLORD,
	{ 500, 			DND_MTYPE_MAGICAL_POW									},//DND_SKULLWIZARD,
	{ 650, 			DND_MTYPE_ROBOTIC_POW									},//DND_CYBORGWARRIOR,
	{ 400, 			DND_MTYPE_DEMON_POW										},//DND_SHADOWBEAST,
	{ 450, 			DND_MTYPE_DEMON_POW										},//DND_CHAOSSERPENT,
	{ 750, 			DND_MTYPE_DEMON_POW										},//DND_MOONSATYR,
	{ 400, 			DND_MTYPE_MAGICAL_POW									},//DND_ICEGOLEM,
	{ 500,			DND_MTYPE_ROBOTIC_POW									},//DND_PUTREFIER,
	{ 450, 			DND_MTYPE_DEMON_POW										},//DND_GLADIATOR,

	// Baron
	{ 1000, 		DND_MTYPE_DEMON_POW										},//DND_LAVADEMON,
	{ 1100, 		DND_MTYPE_DEMON_POW										},//DND_LORDOFHERESY,
	{ 1200, 		DND_MTYPE_DEMON_POW										},//DND_BORMERETH,
	{ 1100, 		DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_BARBATOS,
	{ 1000, 		DND_MTYPE_DEMON_POW										},//DND_BLOODSEEKER,
	{ 900, 			DND_MTYPE_MAGICAL_POW									},//DND_SHADOWWIZARD,
	{ 1650, 		DND_MTYPE_DEMON_POW										},//DND_KJAROCH,
	{ 1500, 		DND_MTYPE_ROBOTIC_POW									},//DND_CYBRUISER,
	{ 1500, 		DND_MTYPE_DEMON_POW										},//DND_BRUISERDEMON,
	{ 900, 			DND_MTYPE_DEMON_POW										},//DND_MAGMASERPENT,
	{ 900, 			DND_MTYPE_UNDEAD_POW									},//DND_DREADKNIGHT,
	{ 900, 			DND_MTYPE_MAGICAL_POW									},//DND_MAGMAGOLEM,
	{ 800,			DND_MTYPE_ROBOTIC_POW									},//DND_JUDICATOR,
	{ 1250, 		DND_MTYPE_DEMON_POW										},//DND_WARMASTER,
	
	// Fatso
	{ 850, 			DND_MTYPE_DEMON_POW										},//DND_CORPULENT,
	{ 850, 			DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_DAEDABUS,
	{ 775, 			DND_MTYPE_ROBOTIC_POW									},//DND_PALADIN,
	{ 875, 			DND_MTYPE_MAGICAL_POW									},//DND_GAMON,
	{ 900, 			DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_MEPHISTO,
	{ 1100, 		DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_MAFIBUS,
	{ 650, 			DND_MTYPE_DEMON_POW										},//DND_ICEFATSO,
	{ 775, 			DND_MTYPE_DEMON_POW										},//DND_ABOMINATION,
	{ 800,			DND_MTYPE_ROBOTIC_POW									},//DND_REDEEMER
	{ 1000, 		DND_MTYPE_MAGICAL_POW									},//DND_GOLDGOLEM,

	// Arachnotron
	{ 500, 			DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_FUSIONSPIDER,
	{ 450, 			DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_RAILARACHNOTRON,
	{ 700, 			DND_MTYPE_DEMON_POW										},//DND_HELLFORGESPIDER,
	{ 700, 			DND_MTYPE_DEMON_POW										},//DND_VORE,
	{ 700, 			DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_BABYDEMOLISHER,
	{ 550, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_CHAINGUNGENERAL,
	{ 550, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_CHAINGUNCOMMANDO,
	{ 600, 			DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW				},//DND_LEGIONNAIRE,
	{ 650, 			DND_MTYPE_DEMON_POW										},//DND_MANTICORE,

	// ArchVile
	{ 850, 			DND_MTYPE_DEMON_POW										},//DND_DIABLOIST,
	{ 800, 			DND_MTYPE_UNDEAD_POW									},//DND_UNDEADPRIEST,
	{ 100, 			DND_MTYPE_UNDEAD_POW									},//DND_UNDEADPRIESTGHOST,
	{ 800, 			DND_MTYPE_DEMON_POW										},//DND_DEATHVILE,
	{ 1250, 		DND_MTYPE_DEMON_POW										},//DND_HIEROPHANT,
	{ 800, 			DND_MTYPE_MAGICAL_POW									},//DND_GURU,
	{ 900, 			DND_MTYPE_UNDEAD_POW									},//DND_DEATHKNIGHT,
	{ 900, 			DND_MTYPE_UNDEAD_POW									},//DND_HORSHACKER,
	{ 750, 			DND_MTYPE_MAGICAL_POW									},//DND_DARKZEALOT,
	{ 625, 			DND_MTYPE_DEMON_POW										},//DND_FLESHWIZARD,
	
	// Spider Mastermind
	{ 4000, 		DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_DEMOLISHER,
	{ 3500, 		DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_ARACHNOPHYTE,
	{ 4000, 		DND_MTYPE_DEMON_POW										},//DND_PSIONICQUEEN,
	{ 4000, 		DND_MTYPE_DEMON_POW										},//DND_ANGELOFDEATH,
	{ 4750, 		DND_MTYPE_MAGICAL_POW									},//DND_GOLDLICH,
	{ 300, 			DND_MTYPE_MAGICAL_POW									},//DND_GOLDLICHFAKE,
	{ 4000, 		DND_MTYPE_MAGICAL_POW									},//DND_IRONLICH,
	{ 4000, 		DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_SPIDEROVERLORD,
	{ 4000, 		DND_MTYPE_MAGICAL_POW									},//DND_DARKLICH,
	
	// Cyberdemon
	{ 4500, 		DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_CARDINAL,
	{ 5750, 		DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_TERMINATOR,
	{ 4750, 		DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_THAMUZ,
	{ 5250, 		DND_MTYPE_DEMON_POW										},//DND_AZAZEL,
	{ 6000, 		DND_MTYPE_DEMON_POW										},//DND_HELLSMITH,
	{ 6500, 		DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_THANATOS,
	{ 5500, 		DND_MTYPE_MAGICAL_POW									},//DND_AVATAR,
	{ 6000, 		DND_MTYPE_DEMON_POW										},//DND_CERBERUS,
	
	// Legendaries
	{ 20000, 		DND_MTYPE_DEMON_POW										},//DND_DREAMER,
	{ 16500, 		DND_MTYPE_DEMON_POW										},//DND_TORRASQUE,
	{ 18500, 		DND_MTYPE_DEMON_POW										},//DND_MORDECQAI,
	{ 13500, 		DND_MTYPE_ROBOTIC_POW									},//DND_GODSLAYER,
	{ 17500, 		DND_MTYPE_DEMON_POW										},//DND_GOLGOTH,
	
	// uniques
	{ 500,			DND_MTYPE_ZOMBIE_POW | DND_MTYPE_UNDEAD_POW				},//DND_TERON
	{ 800,			DND_MTYPE_ZOMBIE_POW | DND_MTYPE_UNDEAD_POW				},//DND_GANT
	{ 1200,			DND_MTYPE_ZOMBIE_POW | DND_MTYPE_UNDEAD_POW				},//DND_BRONN
	{ 900,			DND_MTYPE_DEMON_POW										},//DND_VAAJ
	{ 1500,			DND_MTYPE_ZOMBIE_POW | DND_MTYPE_UNDEAD_POW				},//DND_REMUS
	{ 1250,			DND_MTYPE_DEMON_POW										},//DND_SSRATH
	{ 600,			DND_MTYPE_UNDEAD_POW									},//DND_HOLLOWSHELL
	{ 1500,			DND_MTYPE_DEMON_POW										},//DND_OMNISIGHT
	{ 1000,			DND_MTYPE_MAGICAL_POW									},//DND_CHEGOVAX
	{ 900,			DND_MTYPE_UNDEAD_POW									},//DND_ONIMUZ
	{ 1750,			DND_MTYPE_DEMON_POW										},//DND_HARKIMONDE
	{ 2250,			DND_MTYPE_DEMON_POW										},//DND_LESHRAC
	{ 200,			DND_MTYPE_MAGICAL_POW									},//DND_LESHRACPOD
	{ 2000,			DND_MTYPE_DEMON_POW										},//DND_KRULL manc
	{ 1750,			DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW				},//DND_THORAX arach
	{ 1800,			DND_MTYPE_MAGICAL_POW									},//DND_ZRAVOG vile
	{ 7500,			DND_MTYPE_DEMON_POW										},//DND_ERYXIA
	{ 8500,			DND_MTYPE_DEMON_POW										} //DND_ABAXOTH
};

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
	return tid >= DND_PETTID_BEGIN && tid < DND_PETTID_END;
}

// all demon barons, fatsos or arachnos or bosses that are demons can drop a soul ammo
bool CanDropSoulAmmoTID(int tid) {
	int mid = MonsterProperties[tid - DND_MONSTERTID_BEGIN].id;
	return (mid == MONSTER_BARON || (mid >= DND_CUSTOM_BARON_BEGIN && mid <= DND_CUSTOM_BARON_END) || 
			mid == MONSTER_FATSO || (mid >= DND_CUSTOM_FATSO_BEGIN && mid <= DND_CUSTOM_FATSO_END) ||
			mid == MONSTER_SPIDER || (mid >= DND_CUSTOM_ARACHNO_BEGIN && mid <= DND_CUSTOM_ARACHNO_END) ||
			isBoss()) && (MonsterData[mid].flags & DND_MTYPE_DEMON_POW);
}

bool CanDropSoulAmmo() {
	return CanDropSoulAmmoTID(ActivatorTID());
}

// First element on each list is the "Vanilla" monster, rest follow from their variations with Var1 to VarX
#define MAX_MONSTER_CATEGORIES (MONSTERCLASS_WOLFENSS + 1)
#define MAX_MONSTER_VARIATIONS 17 // this includes vanilla

int Monster_Weights[MAX_MONSTER_CATEGORIES][MAX_MONSTER_VARIATIONS] = {
    // Zombieman
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_ENDMARKER
	},
    // Shotgunguy
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_VERYRARE, 
		DND_MWEIGHT_ENDMARKER 
	},
    // Chaingunguy
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_VERYRARE, 
		DND_MWEIGHT_EPIC, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_VERYRARE, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_VERYRARE, 
		DND_MWEIGHT_ENDMARKER
	},
    // Demon
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_VERYRARE, 
		DND_MWEIGHT_VERYRARE, 
		DND_MWEIGHT_EPIC, 
		DND_MWEIGHT_ENDMARKER
	},
    // Spectre
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_ENDMARKER
	},
    // Imp
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_RARE1 
	},
    // Caco
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_ENDMARKER
	},
    // Pain Elemental
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_VERYRARE, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_ENDMARKER 
	},
    // Lost Soul
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_ENDMARKER 
	},
    // Revenant
    { 
		DND_MWEIGHT_COMMON,
		DND_MWEIGHT_COMMON,
		DND_MWEIGHT_UNCOMMON,
		DND_MWEIGHT_UNCOMMON,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_ENDMARKER
	},
    // HellKnight
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_UNCOMMON,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_ENDMARKER
	},
    // Baron
    { 
		DND_MWEIGHT_COMMON,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_UNCOMMON,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_UNCOMMON,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_EPIC,
		DND_MWEIGHT_ENDMARKER 
	},
    // Fatso
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_RARE1, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_VERYRARE, 
		DND_MWEIGHT_VERYRARE, 
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_COMMON,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_ENDMARKER
	},
    // Arachno
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_COMMON,
		DND_MWEIGHT_COMMON,
		DND_MWEIGHT_UNCOMMON,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_ENDMARKER
	},
    // ArchVile
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_EPIC,
		DND_MWEIGHT_UNCOMMON,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_EPIC,
		DND_MWEIGHT_EPIC,
		DND_MWEIGHT_ENDMARKER
	},
    // Spider Mastermind
    { 
		DND_MWEIGHT_COMMON,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_UNCOMMON,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_RARE1,
		DND_MWEIGHT_EPIC,
		DND_MWEIGHT_RARE2,
		DND_MWEIGHT_VERYRARE,
		DND_MWEIGHT_ENDMARKER 
	},
    // Cyberdemon
    { 
		DND_MWEIGHT_COMMON, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_UNCOMMON, 
		DND_MWEIGHT_VERYRARE, 
		DND_MWEIGHT_VERYRARE, 
		DND_MWEIGHT_EPIC, 
		DND_MWEIGHT_RARE2, 
		DND_MWEIGHT_EPIC,
		DND_MWEIGHT_ENDMARKER 
	},
	// nazi -- only nazi
	{
		DND_MWEIGHT_COMMON,
		DND_MWEIGHT_ENDMARKER
	}
};

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
	MonsterData[MONSTER_WARMASTER].trait_list[DND_REFLECTIVE] = true;
	
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
}

Script "DnD Setup Monster Data" OPEN {
	SetupMonsterData();
}

#endif
