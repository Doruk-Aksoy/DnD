#include "DnD_Common.h"
#include "DnD_Elites.h"
#include "DnD_DatabaseRows.h"
#include "DnD_MenuConstants.h"
#include "DnD_TempWeps.h"
#include "DnD_DamageCache.h"
#include "DnD_Ammo.h"
#include "DnD_Monsters.h"
#include "DnD_SpecialTrails.h"
#include "DnD_ChestKey.h"
#include "DnD_Skills.h"
#include "DnD_Settings.h"
#include "DnD_ClassMenu.h"
#include "DnD_Explosion.h"
#include "DnD_Hud.h"
#include "DnD_Research.h"
#include "DnD_Statistics.h"
#include "DnD_Scoreboard.h"

// for now allocate slots for 10
#define MAX_SCRIPT_TRACK 10

enum {
	DND_SCRIPT_EXPTRACKER,
	DND_SCRIPT_BLEND
};

global bool 17: PlayerScriptsCheck[MAX_SCRIPT_TRACK][MAXPLAYERS];

enum {
	PAINBLEND_RED,
	PAINBLEND_GREEN,
	PAINBLEND_DARKSLATEGRAY,
	PAINBLEND_MIDNIGHTBLUE,
	PAINBLEND_DARKSLATEBLUE,
	PAINBLEND_ORANGE,
	PAINBLEND_LIGHTGREEN,
	PAINBLEND_DARKGOLDENROD,
	PAINBLEND_LIGHTBLUE,
	PAINBLEND_LIGHTSLATEBLUE,
	PAINBLEND_MEDIUMPURPLE,
	PAINBLEND_BLUE,
	PAINBLEND_GRAY
};

#define DND_PAINBLEND_MINTICS 8
#define DND_PAINBLEND_MAXTICS 35
#define DND_PAINBLEND_MAXALPHA 75 // 75%
#define DEATHRAY_MARKER_TID_ADD 1000

#define PERK_GAIN_RATE 5

#define BASEPISTCLIP 12
#define BASEMGCLIP 50
#define BASEHMGCLIP 60

#define DND_EXPBAR_SIZEX 132

// dash script defs, by KeksDose
#define SD_TIMEFRAME		7			// Tics during which you must double press
#define SD_DASHDELAY		27			// Tics before being able to dash again
#define SD_DASHSPEED		7.75	   	// Dash speed whilst in the air
#define SD_DASHJUMP			4.0	   	    // Adds a little z-velocity while in the air
#define SD_DASHSTRONG		12.0		// And dash speed whilst grounded
#define SD_SOUNDVOLUME		1.0			// How loud the dash sound is played

#define CHANCE_HEART 0.05

#define UPGRADETEXTID 6999
#define SURVIVEICO 7000
#define SURVIVETOPTEXTID 7001
#define SURVIVETEXTID 7002
#define SURVIVEID 7003
#define SURVIVETEXTID2 7004
#define SURVIVEBAKID 7005

#define VORTEXTIDSTART 10000
#define P_TEMPTID 200
#define RAILINITTID 2000
#define RAILTIDADD 500
#define SPECIAL_FX_TID 32768

#define REFLECTFXTID 9000

#define INTERVENTION_DURATION TICRATE * 8

#define DND_EMERALD_TRANSLATIONID 7000

#define DND_BUDGET_BASE 3
#define DND_MAX_SHARE 4
#define DND_SPREE_AMOUNT 4 * TICRATE // 4 * 35
#define DND_SPREE_PER 10
#define DND_SPREE_TRIGGER (DND_SPREE_PER - 1) // -1 because current monster killed doesn't count
#define DND_SPREE_BASE 4 // 1 / 4
#define DND_SPECIALAMMO_TEXTID 6001
#define DND_SPREE_BLOODID 6000
#define DND_SPREE_TEXT1ID 5999
#define DND_SPREE_TEXT2ID 5998

#define DND_MAX_MONSTERDELAY 8

#define DND_DEMONBANE_GAIN 50
#define DND_DEMONBANE_REDUCE 4 // 1 / 4 = 25% damage, 75% reduction

#define DND_NETHERMASK_AMP 3 // 3 / 2 => 50%
#define DND_NETHERMASK_DIV 2
#define DND_NETHERGHOST_AMP 5 // 5 / 4 => 25%
#define DND_NETHERGHOST_DIV 4

#define DND_HATESHARD_FACTOR 3 // 50% more
#define DND_HATESHARD_DIV 2
#define DND_HATESHARD_REDUCTION 5 // 1 / 5 => you do 20% of your damage, which is 80% less

#define DND_ARTEMIS_REDUCE 3 // 1 / 3

#define DND_AGAMOTTO_DEFENSE 14 // 14/10 is 1.4 => 40% more damage
#define DND_AGAMOTTO_DEFENSE_FACTOR 10
#define DND_AGAMOTTO_OFFENSE 2 // 100% more damage => x2
#define DND_AGAMOTTO_OFFENSE_FACTOR 1

#define DND_LICHARM_FACTOR 10 // 1 / 10 => 90% reduced damage
#define DND_LICHARM_BUFF 5 // 5 / 4 => 25% more damage
#define DND_LICHARM_BUFF_DIV 4

#define DND_SIGIL_BUFF 2 // x2
#define DND_SIGIL_NERF 0 // x0 => complete negation of damage

#define DND_HUNTERTALISMAN_NERF 3 // 1 / 3 -- this is subtracted

#define DND_CYBERNETICARMOR_AMOUNT 300
#define DND_RAVAGERARMOR_AMOUNT 250

#define DND_HEALTHEXPSCALE 5
#define DND_HEALTHCREDITSCALE 10
#define DND_HEALTHCREDITAFTER50SCALE 5
#define DND_HEALTHCREDITAFTER75SCALE 10
#define DND_HEALTHCREDITUPSCALE 1
#define DND_MIN_CREDIT 3
#define DND_REGEN_PERCENT 2

#define DND_ARTIFACT_GAIN 50

#define DND_NECRO_BULKSCALE 8

#define DND_EXP_BASEFACTOR 4
#define DND_CREDIT_BASEFACTOR 5
#define DND_RESEARCH_MAX_CHANCE 1.0
#define DND_RESEARCH_DROPMULT 3

#define DND_TALISMAN_MARK 42049

#define DND_AFTER50_INCREMENT 200
#define DND_AFTER75_INCREMENT 800

#define AGAMOTTO_MOVE_WINDOW 1 << 16

#define TEMP_TID 999999

#define DND_MAX_DIGITLEN 7

#define DND_BOSS_SOULGIVE 4

#define RIP_FADE_TIMER 20
#define RIP_MONSTER_FADE_DELAY 3 * TICRATE

#define DND_NO_TRANSLATION 70
#define DND_CRIT_TRANSLATION 71

#define DND_SOULAMMO_DROPRATE 0.05 // 5% chance
#define DND_SOULAMMO_SMALLCHANCE 75

#define DND_ITEM_LINGER_TIME 120

#define DND_MARINE_SELFRESIST 25 // 25%

#define DND_WANDERER_EXP_DAMAGE 10 // 1 / 10 = 10%
#define DND_WANDERER_EXP_TID 54000

// RPG ELEMENTS END

#define DefStepSound "Player/Move"

// thunderstaff info things
#define DND_THUNDERSTAFF_MAXTARGETS 5
#define DND_THUNDERSTAFF_DAMAGERTID 32769
#define DND_THUNDER_RING_TIDSTART 40000
#define DND_THUNDER_RADIUSPERCOUNT 16
#define DND_THUNDERSTAFF_BASERANGE 128
typedef struct dist_tid_pair {
	int dist;
	int tid;
} dist_tid_pair_T;
#define DND_MAXSCANTRACER 256

#define DND_MAX_PULLFORCE 256.0
enum {
	DND_PULLF_CANPULLDEAD = 1,
	DND_PULLF_REVERSEDIR = 2
};

#define MAX_GRAVDIS_COUNT 512
#define GRAVDIS_DELAY_TIME 15 // 3 seconds is total time, 3 x (35 / 7) = 15
#define GRAVDIS_CHECK_PERIOD 7
#define GRAVIDS_MAXPULLVEL 32.0
#define GRAVDIS_FLING_FACTOR 128
#define GRAVDIS_HEIGHT_FACTOR 15
#define GRAVDIS_HEIGHTADD_PER 64
#define GRAVDIS_CIRCLE_PARTICLES 48

#define CHARON_PARTICLE_DENSITY 8

#define KANJI_TRAP_TID 2300
#define KANJI_TRAPEXP_TID 2400

#define DARKLANCE_TID 2100
#define DARKLANCE_SHREDDER 2200
#define DARKLANCE_ALT_DURATION 15 // 105 tic duration, 7 tic timer between damage instances => 105 / 7 = 15
#define DARKLANCE_ALT_DURATION_TICS 105
#define DARKLANCE_TICKS 7
#define DARKLANCE_BOUNCE_DIST 224.0

#define DND_LAND_WINDOW 1.0
#define DND_MAX_CORPSELAND_ATTEMPTS 1024

enum {
	DND_SPECIAL_RESEARCH = 1,
	DND_SPECIAL_ORB = 2,
	DND_SPECIAL_TALENTCAPSULE = 4,
	DND_SPECIAL_ELIXIR = 8,
	DND_SPECIAL_CHARM = 16
};

#define DND_BONUS_SCREENTIME 6
enum {
	BONUS_KILL,
	BONUS_ITEM,
	BONUS_SECRET,
	BONUS_BONUS,
	BONUS_EXP_RATE = 5,
	BONUS_CREDIT_RATE = 5,
	BONUS_SECRET_RATE = 3,
};
	
#define MAX_SPREE_TEXT 20
#define SPREE_TEXT_PREFIX "DND_SPREETEXT"

str GetSpreeText(int spree_id) {
	return StrParam(s:SPREE_TEXT_PREFIX, d:spree_id);
}

#define AMMODISPLAY_ID 1000

bool Quest_Pick_Done = 0;
bool PlayerCanLoad[MAXPLAYERS] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

// various states are checked using this, such as bonus states or player joins
// don't add in between, add below!
enum {
	DND_STATECHECK_PLAYERJOIN,
	DND_STATECHECK_KILLBONUS,
	DND_STATECHECK_ITEMBONUS,
	DND_STATECHECK_SECRETBONUS,
	DND_STATECHECK_BONUSBONUS
};

#define MAX_STATES DND_STATECHECK_BONUSBONUS + 1
int DnD_StateChecker[MAX_STATES];
// see if map changed or not

int PlayerWeaponUsed[MAXPLAYERS] = { -1 };
bool UniqueMonsterAvailability[MAX_MONSTER_CATEGORIES] = { 0 };

void Reset_RPGInfo (int resetflags) {
	int i;
	if(resetflags & RESET_LEVEL) {
		SetInventory("Exp", 0);
		SetInventory("LevelExp", 0);
		SetInventory("Level", 1);
	}
	
	if(resetflags & RESET_CREDIT)
		SetInventory("Credit", 0);
		
	if(resetflags & RESET_STATS) {
		for(i = DND_ATTRIB_BEGIN; i <= DND_ATTRIB_END; ++i)
			SetInventory(StatData[i], 0);
		SetActorProperty(0, APROP_HEALTH, DND_BASE_HEALTH);
	}
	
	if(resetflags & RESET_PERKS) {
		for(i = DND_PERK_BEGIN; i <= DND_PERK_END; ++i)
			SetInventory(StatData[i], 0);
		SetAmmoCapacity("StoredMedkit", DND_BASE_HEALTH);
	}
}

int CheckLevelUp (void) {
	int prevlvl = GetStat(STAT_LVL), exptemp;
	int currlvl;
	// -1 because initial level is 1
	// we need to check for the current up-to-date level, not previous level here!!
	while((currlvl = GetStat(STAT_LVL)) < MAXLEVELS && GetStat(STAT_EXP) >= LevelCurve[currlvl - 1]) {
		exptemp = GetStat(STAT_EXP) - LevelCurve[currlvl - 1];
		if(!((currlvl + 1) % 5)) { // multiples of 5 give perk
			GiveInventory("PerkPoint", 1);
			GiveInventory("PerkedUp", 1);
			ACS_NamedExecuteAlways("DnD Levelup Log", 0, 1);
		}
		// talents are removed from game
		/*if(!((GetStat(STAT_LVL) + 1) % DND_TALENTPOINT_MARK)) {
			// disabled for now
			//GiveInventory("TalentPoint", 1);
			//GiveInventory("TalentedUp", 1);
		}*/
		GiveInventory("Level", 1);
		SetInventory("Exp", exptemp);
		GiveInventory("AttributePoint", ATTRIB_PER_LEVEL);
	}
	return GetStat(STAT_LVL) - prevlvl;
}

void HandleLevelup() {
	int prevlvl = CheckInventory("Level");
	if(CheckLevelUp()) {
		LocalAmbientSound("RPG/LevelUp", 127);
		GiveInventory("LevelUpEffectSpawner", 1);
		GiveInventory("LeveledUp", 1);
		ACS_NamedExecuteAlways("DnD Levelup Log", 0);
		++PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL];
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
		) {
			HandleClassPerks();
			
			// this is done as new perks might increase some damage factors
			ForcePlayerDamageCaching(PlayerNumber());
			
			// make some announcement the player has a new perk
			ACS_NamedExecuteAlways("DnD Announcer", 0, DND_ANNOUNCER_NEWCLASSPERK);
		}
		else
			ACS_NamedExecuteAlways("DnD Announcer", 0, DND_ANNOUNCER_ATTRIBPOINT);
	}
}

int DnD_BonusMessageY(int bonustype) {
	int res = 34.1;
	for(int i = DND_STATECHECK_KILLBONUS; i <= DND_STATECHECK_BONUSBONUS; ++i)
		if(DnD_StateChecker[i])
			res += 16.0;
	return res;
}

// Map Evaluation

enum {
	// Monsters
	DND_ZOMBIE_CONTRIB = 3,
	DND_SHOTGUNNER_CONTRIB = 4,
	DND_CHAINGUNNER_CONTRIB = 5,
	DND_IMP_CONTRIB = 4,
	DND_DEMON_CONTRIB = 6,
	DND_HELLKNIGHT_CONTRIB = 10,
	DND_BARON_CONTRIB = 18,
	DND_CACO_CONTRIB = 8,
	DND_PAIN_CONTRIB = 12,
	DND_SOUL_CONTRIB = 4,
	DND_REVENANT_CONTRIB = 9,
	DND_ARACHNO_CONTRIB = 16,
	DND_VILE_CONTRIB = 20,
	DND_FATSO_CONTRIB = 16,
	DND_SPIDERMASTERMIND_CONTRIB = 45,
	DND_CYBERDEMON_CONTRIB = 50,
	DND_BOSSBRAIN_CONTRIB = 9000,
	
	// Items
	DND_HEALTHBONUS_CONTRIB = -1,
	DND_ARMORBONUS_CONTRIB = -1,
	DND_STIM_CONTRIB = -6,
	DND_MEDIKIT_CONTRIB = -12,
	DND_BERSERK_CONTRIB = -30,
	DND_INVUL_CONTRIB = -80,
	DND_SOULSPHERE_CONTRIB = -30,
	DND_MEGA_CONTRIB = -60,
	DND_EVIL_CONTRIB = -20,
	DND_GREENARMOR_CONTRIB = -12,
	DND_YELLOWARMOR_CONTRIB = -18,
	DND_BLUEARMOR_CONTRIB = -24,
	DND_REDARMOR_CONTRIB = -36
};

enum {
	DND_VERYEASY,
	DND_EASY,
	DND_MEDIUM,
	DND_HARD,
	DND_VERYHARD
};

enum {
	DND_VERYEASY_LIMIT = 300,
	DND_EASY_LIMIT = 500,
	DND_MEDIUM_LIMIT = 800,
	DND_HARD_LIMIT = 1250,
	DND_VERYHARD_LIMIT = 1800
};

// 5 Tiers: 0 -> Very Easy, 1 -> Easy, 2 -> Medium, 3 -> Hard and 4 -> Very Hard.
void CalculateMapDifficulty() {
	int factor = 0;
	// yes this is ugly but it won't ever change, no new spawners will come etc so why not :)
	factor += ThingCountName("ZombiemanSpawner", 0) * DND_ZOMBIE_CONTRIB;
	factor += ThingCountName("ShotgunguySpawner", 0) * DND_SHOTGUNNER_CONTRIB;
	factor += ThingCountName("ChaingunguySpawner", 0) * DND_CHAINGUNNER_CONTRIB;
	factor += ThingCountName("DemonSpawner", 0) * DND_DEMON_CONTRIB;
	factor += ThingCountName("SpectreSpawner", 0) * DND_DEMON_CONTRIB;
	factor += ThingCountName("ImpSpawner", 0) * DND_IMP_CONTRIB;
	factor += ThingCountName("LostSoulSpawner", 0) * DND_SOUL_CONTRIB;
	factor += ThingCountName("CacodemonSpawner", 0) * DND_CACO_CONTRIB;
	factor += ThingCountName("PainElementalSpawner", 0) * DND_PAIN_CONTRIB;
	factor += ThingCountName("RevenantSpawner", 0) * DND_REVENANT_CONTRIB;
	factor += ThingCountName("HellKnightSpawner", 0) * DND_HELLKNIGHT_CONTRIB;
	factor += ThingCountName("BaronSpawner", 0) * DND_BARON_CONTRIB;
	factor += ThingCountName("FatsoSpawner", 0) * DND_FATSO_CONTRIB;
	factor += ThingCountName("SpiderSpawner", 0) * DND_ARACHNO_CONTRIB;
	factor += ThingCountName("ArchVileSpawner", 0) * DND_VILE_CONTRIB;
	factor += ThingCountName("MastermindSpawner", 0) * DND_SPIDERMASTERMIND_CONTRIB;
	factor += ThingCountName("CyberSpawner", 0) * DND_CYBERDEMON_CONTRIB;
	factor += ThingCountName("BossBrain", 0) * DND_BOSSBRAIN_CONTRIB;
	
	factor += ThingCountName("InvulnerabilitySphere2", 0) * DND_INVUL_CONTRIB;
	factor += ThingCountName("StimpackSpawner", 0) * DND_STIM_CONTRIB;
	factor += ThingCountName("MedikitSpawner", 0) * DND_MEDIKIT_CONTRIB;
	factor += ThingCountName("BonusReplacer", 0) * DND_HEALTHBONUS_CONTRIB;
	factor += ThingCountName("BonusReplacer2", 0) * DND_ARMORBONUS_CONTRIB;
	factor += ThingCountName("EvilSphere2", 0) * DND_EVIL_CONTRIB;
	factor += ThingCountName("UberSphere2", 0) * DND_MEGA_CONTRIB;
	factor += ThingCountName("SoulSphere3", 0) * DND_SOULSPHERE_CONTRIB;
	factor += ThingCountName("MegaSphere3", 0) * DND_MEGA_CONTRIB;
	factor += ThingCountName("Berserk2", 0) * DND_BERSERK_CONTRIB;
	factor += ThingCountName("NewGreenArmor2", 0) * DND_GREENARMOR_CONTRIB;
	factor += ThingCountName("YellowArmor2", 0) * DND_YELLOWARMOR_CONTRIB;
	factor += ThingCountName("NewBlueArmor2", 0) * DND_BLUEARMOR_CONTRIB;
	factor += ThingCountName("TheRedArmor2", 0) * DND_REDARMOR_CONTRIB;
	if(factor >= DND_VERYHARD_LIMIT)
		MapDifficulty = DND_VERYHARD;
	else if(factor >= DND_HARD_LIMIT)
		MapDifficulty = DND_HARD;
	else if(factor >= DND_MEDIUM_LIMIT)
		MapDifficulty = DND_MEDIUM;
	else if(factor >= DND_EASY_LIMIT)
		MapDifficulty = DND_EASY;
	else
		MapDifficulty = DND_VERYEASY;
	
	SetInventory("MapDifficultyClientside", MapDifficulty);
}

int CalculateBonus(int bonustype, int mdifficulty) {
	if(bonustype == BONUS_KILL) {
		// add 5% for each difficulty level
		return BONUS_EXP_RATE * (mdifficulty + 1);
	}
	else if(bonustype == BONUS_ITEM) {
		// add 250 credit for each difficulty level
		return BONUS_CREDIT_RATE * (mdifficulty + 1);
	}
	else if(bonustype == BONUS_SECRET) {
		// add budget for each difficulty level + 1
		return BONUS_SECRET_RATE * (mdifficulty + 2);
	}
	else if(bonustype == BONUS_BONUS) {
		return BONUS_SECRET_RATE * 2 * (mdifficulty + 1);
	}
	return 1;
}

void ShowBonusMessage(int bonustype, int y) {
	int bval = CalculateBonus(bonustype, CheckInventory("MapDifficultyClientside"));
	SetHudSize(800, 600, 1);
	SetFont("BIGFONT");
	switch(bonustype) {
		case BONUS_KILL:
			LocalAmbientSound("RPG/KillBonus", 127);
			HudMessage(l:"DND_TEXT_MAXKILLS", d:bval, l:"DND_TEXT_MAXKILLREWARD"; HUDMSG_FADEINOUT, KILLBONUSID, CR_RED, 400.4, y, 4.0, 1.0, 1.0);
		break;
		case BONUS_ITEM:
			LocalAmbientSound("RPG/ItemBonus", 127);
			HudMessage(l:"DND_TEXT_MAXITEMS", d:bval, l:"DND_TEXT_MAXITEMREWARD"; HUDMSG_FADEINOUT, ITEMBONUSID, CR_GOLD, 400.4, y, 4.0, 1.0, 1.0);
		break;
		case BONUS_SECRET:
			LocalAmbientSound("RPG/SecretBonus", 127);
			HudMessage(l:"DND_TEXT_MAXSECRETS", d:bval, l:"DND_TEXT_MAXSECRETREWARD"; HUDMSG_FADEINOUT, SECRETBONUSID, CR_GOLD, 400.4, y, 4.0, 1.0, 1.0);
		break;
		case BONUS_BONUS:
			LocalAmbientSound("RPG/BonusBonus", 127);
			HudMessage(l:"DND_TEXT_MAPTOAST"; HUDMSG_FADEINOUT, BONUSBONUSID, CR_GOLD, 400.4, y, 4.0, 1.0, 1.0);
		break;
	}
}

void DistributeBonus(int bonustype) {
	int bval = 0, temp = 0, i = 0;
	if(bonustype == BONUS_KILL) {
		bval = CalculateBonus(BONUS_KILL, MapDifficulty);
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(PlayerInGame(i) && isActorAlive(i + P_TIDSTART)) {
				temp = GetActorStat(i + P_TIDSTART, STAT_LVLEXP) * bval / 100;
				GiveActorInventory(i + P_TIDSTART, "DnD_KillBonusShower", 1);
				GiveActorExp(i + P_TIDSTART, temp);
			}
		}
	}
	else if(bonustype == BONUS_ITEM) {
		bval = CalculateBonus(BONUS_ITEM, MapDifficulty);
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(PlayerInGame(i) && isActorAlive(i + P_TIDSTART)) {
				temp = GetActorStat(i + P_TIDSTART, STAT_LVLCRED) * bval / 100;
				GiveActorInventory(i + P_TIDSTART, "DnD_ItemBonusShower", 1);
				GiveActorCredit(i + P_TIDSTART, bval);
			}
		}
	}
	else if(bonustype == BONUS_SECRET) {
		bval = CalculateBonus(BONUS_SECRET, MapDifficulty);
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(PlayerInGame(i) && isActorAlive(i + P_TIDSTART)) {
				GiveActorInventory(i + P_TIDSTART, "DnD_SecretBonusShower", 1);
				GiveActorBudget(i + P_TIDSTART, bval);
			}
		}
	}
	else if(bonustype == BONUS_BONUS) {
		for(i = 0; i < MAXPLAYERS; ++i) {
			temp = i + P_TIDSTART;
			if(PlayerInGame(i) && isActorAlive(temp)) {
				// getspawnhealth needs current activator, this wasn't using it
				SetActivator(temp);
				bval = GetSpawnHealth();
				if(GetActorProperty(temp, APROP_HEALTH) < bval)
					GiveInventory("HealthBonusX", bval - GetActorProperty(temp, APROP_HEALTH));
				GiveActorInventory(temp, "DnD_BonusBonusShower", 1);
				SetActivator(-1);
			}
		}
	}
}

int IsUsingMeleeWeapon() {
	return CheckWeapon(" Fists ") || 
		   CheckWeapon(" Chainsaw ") || 
		   CheckWeapon("Upgraded Chainsaw") || 
		   CheckWeapon("Sickle") || 
		   CheckWeapon("Excalibat") || 
		   CheckWeapon("Katana") ||
		   CheckWeapon("Dusk Blade") ||
		   CheckWeapon("ResMelee1") ||
		   CheckWeapon("Soul Render");
}

enum {
	MONSTERDATA_VARIATIONCOUNT,
	MONSTERDATA_WEIGHTSUM
};

// MAX_MONSTER_CATEGORIES
global int 21: MonsterCategoryData[][2];

void PickQuest() {
	if(GetCVar("dnd_enable_quests") && PlayerCount()) {
		if(GetCVar("dnd_quest_avglevel") <= (PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] / PlayerCount()) && random(1, 100) <= Clamp_Between(GetCVar("dnd_quest_chance"), 1, 100)) {
			do {
				active_quest_id = random(0, MAX_QUESTS - 1);
			} while(!IsValidQuest(active_quest_id));
		}
	}
	else
		active_quest_id = -1;
	Quest_Pick_Done = 1;
}

void ClearQuestCheckers() {
	for(int i = 0; i < MAX_QUESTS; ++i)
		SetInventory(Quest_List[i].qchecker, 0);
}

void CheckMapExitQuest(int pnum, int qid) {
	int tid = pnum + P_TIDSTART;
	bool cond = 0;
	// handle exceptions
	// immediate conclusion quests are concluded as soon as condition is met
	if(Quest_List[qid].qflag & QTYPE_IMMEDIATE)
		return;
	if(qid == QUEST_NODYING)
		cond = true; // it would fail if people died, if not it succeeded
	else if(!QuestExitCheckException(qid)) // typical map exit quests
		cond = !CheckActorInventory(tid, Quest_List[qid].qchecker);
		
	if(!IsQuestComplete(tid, qid)) {
		if(cond)
			CompleteQuest(tid, qid);
		else
			FailQuest(tid);
	}
}

str GetCreditDropperName(int id, bool isElite) {
	str res = "";
	switch(id) {
		case 0:
			res = "LargeCreditDropper";
		break;
		case 1:
			res = "MediumCreditDropper";
		break;
		case 2:
			res = "SmallCreditDropper";
		break;
	}
	
	if(isElite)
		return StrParam(s:res, s:"_Elite");
	return res;
}

#define CREDITDROP_THRESHOLD 0
#define CREDITDROP_CHANCE 1
#define MAXCREDITDROPS 3
int GetCreditDropData(int id, int which) {
	int res = 0;
	
	switch(id) {
		case 0:
			if(!which)
				res = 7500;
			else
				res = 0.05;
		break;
		case 1:
			if(!which)
				res = 2500;
			else
				res = 0.15;
		break;
		case 2:
			if(!which)
				res = 1000;
			else
				res = 0.3;
		break;
	}
	
	return res;
}

void HandleCashDrops(int m_id, int pnum, bool isElite) {
	int addedchance = (Clamp_Between(GetCVar("dnd_credit_droprateadd"), 0, 100) << 16) / 100;
	for(int i = 0; i < MAXCREDITDROPS; ++i) {
		// apply simple monster hp scaling code here to scale threshold with player levels
		int threshold = GetCreditDropData(i, CREDITDROP_THRESHOLD) * (100 + PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL] * Clamp_Between(GetCVar("dnd_monster_hpscalepercent"), 1, 100)) / 100;
		if(MonsterProperties[m_id].maxhp >= threshold) {
			if(!RunDefaultDropChance(pnum, isElite, GetCreditDropData(i, CREDITDROP_CHANCE) + addedchance))
				continue;
			GiveInventory(GetCreditDropperName(i, isElite), 1);
			break;
		}
	}
}

// 0 means they are ready
bool PlayersNotReadyForHardcore() {
	int players_notready = 0;
	for(int i = 0; i < MAXPLAYERS; ++i)
		if (PlayerInGame(i) && PlayerCanLoad[i])
			players_notready++;
	return players_notready;
}

void SpawnTalentCapsule() {
	if(GameType() != GAME_SINGLE_PLAYER) {
		for(int i = 0; i < MAXPLAYERS; ++i)
			if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART))
				SpawnDrop("TalentCapsule", 0, 0, i + 1, 1);
	}
	else
		SpawnDrop("TalentCapsule_SP", 0, 0, 1, 1);
}

void SpawnResearchId(int id) {
	if(GameType() != GAME_SINGLE_PLAYER) {
		for(int i = 0; i < MAXPLAYERS; ++i) {
			// spawn this only if this isn't already found by the player
			if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART) && !CheckResearchStatus(id))
				SpawnDrop("ResearchModule_MP", 24.0, 16, i + 1, id);
		}
	}
	else if(!CheckResearchStatus(id)) // 1 before id is player (0 + 1)
		SpawnDrop("ResearchModule_MP", 24.0, 16, 1, id);
}

void SpawnAccessory() {
	int id;
	if(HardcoreSet) {
		do {
			id = random(1, MAX_ACCESSORY);
		} while(id == DND_REPLACE_THIS_ACCESSORY);
	}
	else
		id = random(1, MAX_ACCESSORY);
	SpawnDrop(StrParam(s:"Accessory_", d:id), 0, 0, 0, 0);
}

void UpdateLevelChestLimit() {
	/* 	2^x + 5x/2 + base_amt (10)
		a healthy range of values rewarding difficulty greatly
		
		Very Easy = 10
		Easy = 13
		Medium = 18
		Hard = 24
		Very Hard = 35
	*/
	CurrentLevelData[LEVELDATA_MAXCHESTS] = MAX_BASE_CHESTCOUNT + (1 << MapDifficulty) + 5 * MapDifficulty / 2;
}

void HandleChestSpawn(int chance_penalty) {
	if(CurrentLevelData[LEVELDATA_CHESTSPAWNED] > CurrentLevelData[LEVELDATA_MAXCHESTS])
		return;
		
	if(!chance_penalty)
		chance_penalty = 1;

	int r = random(0, 1.0);
	if(r <= CHEST_DROPWEIGHT / chance_penalty) {
		// chest will now spawn, determine type of it here
		++CurrentLevelData[LEVELDATA_CHESTSPAWNED];
		r = random(0, 1.0);
		if(r < SILVERCHEST_DROPWEIGHT)
			SpawnDrop("DNDSilverChest", 0, 0, 0, 0);
		else if(r < GOLDCHEST_DROPWEIGHT)
			SpawnDrop("DNDGoldChest", 0, 0, 0, 0);
		else
			SpawnDrop("DNDBronzeChest", 0, 0, 0, 0);
	}
}

void HandleChestDrops(int ctype) {
	HandleElixirDrop(true);
	if(ctype == DND_CHESTTYPE_BRONZE) {
		SpawnOrbForAll(1);
		if(random(0, 1))
			SpawnTokenForAll(1);
	}
	else if(ctype == DND_CHESTTYPE_SILVER) {
		SpawnOrbForAll(2);
		if(random(0, 3))
			SpawnTokenForAll(1);
	}
	else if(ctype == DND_CHESTTYPE_GOLD) {
		SpawnOrbForAll(3);
		SpawnTokenForAll(1);
	}
}

void HandleEliteDrops() {
	int addchance = (Clamp_Between(GetCVar("dnd_orb_dropchanceadd"), 0, 100) << 16) / 100;
	for(int i = 0; i < MAXPLAYERS; ++i) {
		// run each player's chance, drop for corresponding player only
		if(PlayerInGame(i) && (GetCVar("dnd_ignore_dropweights") || (IsActorAlive(i + P_TIDSTART)))) {
			// for orbs
			if(RunDefaultDropChance(i, true, DND_ELITE_BASEDROP_ORB + addchance))
				SpawnOrb(i, true);
			// for tokens -- same likelihood to drop as orbs
			if(RunDefaultDropChance(i, true, DND_ELITE_BASEDROP + addchance))
				SpawnToken(i, true);
		}
	}
}

void HandleCharmLootDrop(bool isElite) {
	int addchance = 0;
	if(isElite)
		addchance = DND_ELITE_BASEDROP / 2;
	for(int i = 0; i < MAXPLAYERS; ++i) {
		// run each player's chance, drop for corresponding player only
		if
		(
			PlayerInGame(i) &&
			(GetCVar("dnd_ignore_dropweights") || (IsActorAlive(i + P_TIDSTART) && RunDefaultDropChance(i, isElite, DND_BASE_CHARMRATE + addchance)))
		)
			SpawnCharm(i, isElite);
	}
	//SpawnCharm(0, isElite);
}

void HandleSoulDrop() {
	if(IsDemon()) {
		str drop = "SoulsDrop";
		if(IsBoss())
			drop = "LargeSoulsDrop";
		SpawnDrop(drop, GetActorProperty(0, APROP_HEIGHT) / 2, 12, 0, 0);
	}
}

void HandleOtherDrops(int target) {
	int pnum = target - P_TIDSTART;
	if(IsZombie())
		GiveActorInventory(target, "DnD_ShotUndead", 1);
	if(CheckInventory("BookofDeadCausedDeath")) {
		GiveActorInventory(target, "Souls", (1 + DND_BOSS_SOULGIVE * IsBoss()) * (100 + GetPlayerAttributeValue(pnum, INV_EX_PICKUPS_MORESOUL)) / 100);
		Spawn("SoulEffectSpawner", GetActorX(0), GetActorY(0), GetActorZ(0));
		TakeInventory("BookofDeadCausedDeath", 1);
	}
	
	if(CheckActorInventory(target, "Ability_HeartSeeker") && RunDefaultDropChance(pnum, CheckInventory("MonsterIsElite"), CHANCE_HEART))
		SpawnDrop("DemonHeartPickup", 24.0, 16, 0, 0);
}

int GetPVelocity(void) {
	int vel;
	int x = GetActorVelX(0);
	int y = GetActorVelY(0);
	int angle = VectorAngle(x, y);
   
	if(((angle + 0.125) % 0.5) > 0.25)
		vel = FixedDiv(y, sin(angle));
	else
		vel = FixedDiv(x, cos(angle));
	return vel >> 16;
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

int GetWeaponSlotFromFlag(int flags) {
	// the first 14 bits are unnecessary
	flags >>= DND_SHIFTBITS_FOR_SLOTFROMFLAG;
	for(int i = 0; i < 32; ++i)
		if(IsSet(flags, i))
			return i;
	return 0;
}

int ScaleMonster(int tid, int pcount, int realhp) {
	int base = realhp;
	int add = 0, level = 1, low, high, temp;
	level = PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] / pcount;
	// ensure minions use master's level
	if(GetActorProperty(0, APROP_MASTERTID)) {
		level = MonsterProperties[GetActorProperty(0, APROP_MASTERTID) - DND_MONSTERTID_BEGIN].level;
	}
	if(GetCVar("dnd_randomize_levels")) {
		low = Clamp_Between(GetCVar("dnd_monsterlevel_low"), 0, 50);
		high = Clamp_Between(GetCVar("dnd_monsterlevel_high"), 0, 50);
		// first 10 levels the monsters shouldn't really get super high levels
		if(level < 10)
			high = Clamp_Between(high, 0, high / 2);
		// give some sort of variety in the levels
		// subtract level from avg half the time
		if(random(0, 1)) {
			temp = low;
			low = -high;
			high = -temp;
		}
		level += random(low, high);
	}
	if(GetCVar("dnd_monsterlevel_behind"))
		level = Clamp_Between(level, 1, PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] / pcount);
	else
		level = Clamp_Between(level, 1, DND_MAX_MONSTERLVL);
	level = Clamp_Between(level, 1, GetCVar("dnd_maxmonsterlevel"));
	if(level > 1) {
		add = (base * Clamp_Between(GetCVar("dnd_monster_hpscalepercent"), 1, 100)) / 100;
		if(level > 75)
			temp = DND_AFTER75_INCREMENT;
		else if(level > 50)
			temp = DND_AFTER50_INCREMENT;
		else
			temp = 1;
		// we are in boost range for hp
		if(temp != 1) {
			if(add > 100) {
				add /= 100;
				add *= 100 + temp;
			}
			else {
				add *= 100 + temp;
				add /= 100;
			}
		}
		// add level factor to it
		// first overflow check
		if(add < (INT_MAX - base) / Max((level - 1), 1)) {
			add *= level - 1;
			if(GetCVar("dnd_playercount_scales_monsters")) {
				if(add > 100) {
					add /= 100;
					add *= 100 + DND_MONSTERHP_PLAYERSCALE * (Clamp_Between(pcount - 1, 0, DND_MAX_PLAYERHPSCALE));
				}
				else {
					add *= 100 + DND_MONSTERHP_PLAYERSCALE * (Clamp_Between(pcount - 1, 0, DND_MAX_PLAYERHPSCALE));
					add /= 100;
				}
			}
		}
		else
			add = INT_MAX - base;
	}
	MonsterProperties[tid - DND_MONSTERTID_BEGIN].basehp = base;
	MonsterProperties[tid - DND_MONSTERTID_BEGIN].maxhp = base + add;
	MonsterProperties[tid - DND_MONSTERTID_BEGIN].level = level;
	return base + add;
}

void ActivateKillingSpree() {
	// if a kill streak wasn't running, run it now
	if(!CheckInventory("DnD_SpreeCountdownRunning")) {
		GiveInventory("DnD_SpreeCountdownRunning", 1);
		ACS_NamedExecuteAlways("DnD Killing Spree Timer", 0);
	}
	// spree count was given, so a possible spree may be in reach
	if(CheckInventory("DnD_SpreeTimer")) {
		GiveInventory("DnD_MultikillCounter", 1);
		
		// screen tint
		if(GetArmorID() == DND_ARMOR_RAVAGER)
			GiveInventory("RavagerPower", 1);
		
		// punisher perk -- be on cruel or more
		if(CheckInventory("Punisher_Perk5") && (CheckInventory("DnD_MultikillCounter") + 1) / DND_SPREE_PER >= 1)
			GiveInventory("Punisher_Perk5_MoveSpeed", 1);
	}
	// give spree counter
	GiveInventory("DnD_SpreeTimer", DND_SPREE_AMOUNT);
}

void ResetPlayerScriptChecks() {
	for(int i = 0; i < MAX_SCRIPT_TRACK; ++i)
		for(int j = 0; j < MAXPLAYERS; ++j)
			PlayerScriptsCheck[i][j] = false;
}

void HandleMonsterTemporaryWeaponDrop(int id, int pnum, bool isElite, bool guaranteed = false) {
	id = MonsterProperties[id].id;
	switch(id) {
		case MONSTER_BLOODFIEND:
		case MONSTER_RAVAGER:
		case MONSTER_LURKER:
			if(guaranteed || RunDefaultDropChance(pnum, isElite, TEMPWEP_BLOODFIENDSPINE_DROPCHANCE))
				SpawnDrop(TemporaryWeaponData[DND_TEMPWEP_BLOODFIENDSPINE][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_VULGAR:
			if(guaranteed || RunDefaultDropChance(pnum, isElite, TEMPWEP_VENOM_DROPCHANCE))
				SpawnDrop(TemporaryWeaponData[DND_TEMPWEP_VENOM][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_CHAINGUNGENERAL:
			if(guaranteed || RunDefaultDropChance(pnum, isElite, TEMPWEP_NAILGUN_DROPCHANCE))
				SpawnDrop(TemporaryWeaponData[DND_TEMPWEP_HEAVYNAILGUN][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_DEATHKNIGHT:
		case MONSTER_HORSHACKER:
			if(guaranteed || RunDefaultDropChance(pnum, isElite, TEMPWEP_SOULRENDER_DROPCHANCE))
				SpawnDrop(TemporaryWeaponData[DND_TEMPWEP_SOULRENDER][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_CORPULENT:
			if(guaranteed || RunDefaultDropChance(pnum, isElite, TEMPWEP_HFCANNON_DROPCHANCE))
				SpawnDrop(TemporaryWeaponData[DND_TEMPWEP_HELLFORGECANNON][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_DARKSERVANT:
			if(guaranteed || RunDefaultDropChance(pnum, isElite, TEMPWEP_DARKGLOVES_DROPCHANCE))
				SpawnDrop(TemporaryWeaponData[DND_TEMPWEP_DARKGLOVES][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
	}
	
	if(pnum != -1 && HasActorMasteredPerk(pnum + P_TIDSTART, STAT_LUCK) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
		HandleMonsterTemporaryWeaponDrop(id, -1, isElite, guaranteed);
}

void ApplyRandomCurse() {
	// nasty rushed code here, clean up later with improved curse system
	int curse_id = random(0, 7);
	switch(curse_id) {
		case 0:
			GiveInventory("DnD_Curse_RedLich", 1);
		break;
		case 1:
			GiveInventory("DnD_Curse_Hierophant", 1);
		break;
		case 2:
			GiveInventory("DnD_Curse_Guru", 1);
		break;
		case 3:
			GiveInventory("DnD_Curse_LichVisionDim", 1);
		break;
		case 4:
			GiveInventory("DnD_Curse_LichSlow", 1);
		break;
		case 5:
			GiveInventory("DnD_Curse_LichPoison", 1);
		break;
		case 6:
			GiveInventory("DnD_Curse_Ensnare", 1);
		break;
		case 7:
			GiveInventory("DnD_Curse_Paladin", 1);
		break;
	}
}

void SpawnDarkLanceProjectile(int this, int a, int proj_tid, int x, int y, int z, int vx, int vy, int vz, int dist, int spd) {
	SpawnProjectile(this, "DarkLanceProjectile", a, 0, 0, 0, proj_tid);
	SetActivator(proj_tid);
	SetActorVelocity(0, vx * spd, vy * spd, vz * spd, 0, 0);
	SetActorPosition(0, x + vx * dist, y + vy * dist, z + 32.0 + vz * dist, 0);
	SetPointer(AAPTR_TARGET, this);
	SetActorProperty(0, APROP_TARGETTID, this);
	SetActorProperty(0, APROP_SPEED, spd << 16);
	Thing_ChangeTID(proj_tid, 0);
	SetActivator(this);
}

void SpawnDarkLanceProjectile_Side(int this, int a, int proj_tid, int x, int y, int z, int vx, int vy, int vz, int xd, int yd, int zd, int spd) {
	SpawnProjectile(this, "DarkLanceProjectile", a, 0, 0, 0, proj_tid);
	SetActivator(proj_tid);
	SetActorVelocity(0, vx * spd, vy * spd, vz * spd, 0, 0);
	SetActorPosition(0, x + vx * xd + vy * yd, y + vy * xd - vx * yd, z + 32.0 + vz * zd, 0);
	SetPointer(AAPTR_TARGET, this);
	SetActorProperty(0, APROP_TARGETTID, this);
	SetActorProperty(0, APROP_SPEED, spd << 16);
	Thing_ChangeTID(proj_tid, 0);
	SetActivator(this);
}

void DoSlotWeaponQuestChecks(int wepid) {
	int pnum = PlayerNumber();
	// Simple Minded quest check
	if(PlayerWeaponUsed[pnum] == -1)
		PlayerWeaponUsed[pnum] = wepid;
	else if(!CheckInventory("DnD_WeaponFiredOther") && PlayerWeaponUsed[pnum] != wepid)
		GiveInventory(Quest_List[QUEST_ONLYONEWEAPON].qchecker, 1);

	// only slot 2 quest check
	if(
		active_quest_id == QUEST_ONLYPISTOLWEAPONS && !CheckInventory(Quest_List[QUEST_ONLYPISTOLWEAPONS].qchecker) &&
		!IsQuestComplete(ActivatorTID(), QUEST_ONLYPISTOLWEAPONS) && !CheckInventory("H_WeaponSlot2")
	  )
		GiveInventory(Quest_List[QUEST_ONLYPISTOLWEAPONS].qchecker, 1);
	
	// only boomstick quest check
	if(
		active_quest_id == QUEST_NOSHOTGUNS && !CheckInventory(Quest_List[QUEST_NOSHOTGUNS].qchecker) && 
		!IsQuestComplete(ActivatorTID(), QUEST_NOSHOTGUNS) && IsBoomstick(wepid)
	  )
		GiveInventory(Quest_List[QUEST_NOSHOTGUNS].qchecker, 1);
	
	// no superweapon quest check
	if(
		active_quest_id == QUEST_NOSUPERWEAPONS && !CheckInventory(Quest_List[QUEST_NOSUPERWEAPONS].qchecker) &&
		!IsQuestComplete(ActivatorTID(), QUEST_NOSUPERWEAPONS) &&
		(CheckInventory("H_WeaponSlot7") || CheckInventory("H_WeaponSlot8"))
	  )
		GiveInventory(Quest_List[QUEST_NOSUPERWEAPONS].qchecker, 1);
		
	// only energy quest check
	if(
		active_quest_id == QUEST_ONLYENERGY && !CheckInventory("DnD_UsingEnergy") && !CheckInventory(Quest_List[QUEST_ONLYENERGY].qchecker)
	  )
		GiveInventory(Quest_List[QUEST_ONLYENERGY].qchecker, 1);
}

void HandleRuination(int this, int target) {
	if(GetArmorAmount()) {
		// remove a third of armor
		TakeArmorAmount(GetArmorCapFromID(GetArmorID()) / 3);
	}
	else
		ACS_NamedExecuteAlways("DnD Hurt Actor", 0, target, this, GetSpawnHealth() / 3);
	TakeInventory("RuinationStacks", RUINATION_MAX_STACKS);
	PlaySound(this, "Elite/RuinationTrigger", CHAN_5, 1.0);
	GiveInventory("RuinBlend", 1);
}

bool IsEliteException(int m_id, int monster_type) {
	return 	MonsterProperties[m_id].trait_list[DND_SUMMONED] || MonsterProperties[m_id].trait_list[DND_LEGENDARY] || CheckInventory("DnD_EliteException") ||
			monster_type == MONSTER_PHANTASM || monster_type == MONSTER_WRAITH || monster_type == MONSTER_HADESSPHERE || monster_type == MONSTER_UNDEADPRIESTGHOST;
}

void HandleUniqueDeath(int unique_id) {
	switch(unique_id) {
		case MONSTER_TERON:
			// spawn some budget
			SpawnForced("BudgetDropper_Medium", GetActorX(0), GetActorY(0), GetActorZ(0) + 16.0);
			SpawnForced("BudgetDropper_Medium", GetActorX(0), GetActorY(0), GetActorZ(0) + 16.0);
			SpawnForced("BudgetDropper_Medium", GetActorX(0), GetActorY(0), GetActorZ(0) + 16.0);
		break;
		case MONSTER_GANT:
			if(GameType() != GAME_SINGLE_PLAYER)
				SpawnDrop("SynthMetalPickup_MP", 24.0, 16, 0, 0);
			else
				SpawnDrop("SynthMetal_SP", 24.0, 16, 0, 0);
		break;
		case MONSTER_BRONN:
			SpawnResearchId(RES_SYNTHMASK);
		break;
		case MONSTER_VAAJ:
			// drops vaaj influenced charm: explosives ignore resists mod guaranteed
			SpawnCharmWithMods_ForAll(INV_ESS_VAAJ);
		break;
		case MONSTER_REMUS:
			// drops armor and gives research for it: lightning coil - super lightning protection, 300, 40% damage reduction. When hurt fires lightning bolts.
			if(GameType() != GAME_SINGLE_PLAYER)
				SpawnDrop("LightningCoilPickup_MP", 24.0, 16, 0, 0);
			else
				SpawnDrop("LightningCoil_SP", 24.0, 16, 0, 0);
		break;
		case MONSTER_SSRATH:
			// ssrath influence: soul type weapons penetrate % magic resistance
			SpawnCharmWithMods_ForAll(INV_ESS_SSRATH);
		break;
		case MONSTER_HOLLOWSHELL:
			// hollow orb: adds an extra mod to a non-unique charm even if it's at its limit (at most +1 of its current limit)
			SpawnSpecificOrbForAll(DND_ORB_HOLLOW, 1);
		break;
		case MONSTER_OMNISIGHT:
			// omnisight influence: large accuracy, % increased accuracy rating
			SpawnCharmWithMods_ForAll(INV_ESS_OMNISIGHT, INV_ESS_OMNISIGHT2);
		break;
		case MONSTER_CHEGOVAX:
			// chegovax influence: Ignite damage on enemies increases every tic %
			SpawnCharmWithMods_ForAll(INV_ESS_CHEGOVAX);
		break;
		case MONSTER_ONIMUZ:
			// Phantasmal Orb: grants used weapon ability to hit ghosts but do 25% less damage overall.
			SpawnSpecificOrbForAll(DND_ORB_PHANTASMAL, 1);
		break;
		case MONSTER_HARKIMONDE:
			// harkimonde influence: Attacks have chance to ignore shields.
			SpawnCharmWithMods_ForAll(INV_ESS_HARKIMONDE);
		break;
		case MONSTER_LESHRAC:
			// leshrac influence: poison damage tics twice as fast
			SpawnCharmWithMods_ForAll(INV_ESS_LESHRAC);
		break;
		case MONSTER_KRULL:
			// krull influence: % chance explosives trigger a second time
			SpawnCharmWithMods_ForAll(INV_ESS_KRULL);
		break;
		case MONSTER_THORAX:
			// thorax influence: Homing projectiles can't be reflected.
			SpawnCharmWithMods_ForAll(INV_ESS_THORAX);
		break;
		case MONSTER_ZRAVOG:
			// zravog influence: Occult attacks reduce enemy magic resistance by 2-5% per hit for 3 seconds, up to 5 stacks.
			SpawnCharmWithMods_ForAll(INV_ESS_ZRAVOG);
		break;
		case MONSTER_ERYXIA:
			// eryxia influence: Frozen enemies take % increased ice damage.
			SpawnCharmWithMods_ForAll(INV_ESS_ERYXIA);
		break;
		case MONSTER_ABAXOTH:
			// Assimilation Orb: Assimilates a chosen charm into another, merging them unpredictably, randomly taking modifiers from both charms. Can have up to 1 additional modifier.
			SpawnSpecificOrbForAll(DND_ORB_ASSIMILATION, 1);
		break;
	}
}

int GetAveragePlayerLevel() {
	int temp = PlayerInformationInLevel[PLAYERLEVELINFO_COUNTATSTART];
	if(temp < 1)
		temp = 1;
	return PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] / temp;
}

void ClearLingeringBuffs() {
	SetInventory("AllMapOnlyOnce", 0);
	SetInventory("Punisher_Perk50_Counter", 0);
	SetInventory("Berserker_DamageTracker", 0);
	SetInventory("Berserker_DamageTimer", 0);
	SetInventory("Berserker_HitTracker", 0);
	SetInventory("Berserker_HitTimer", 0);
	SetInventory("Berserker_NoRoar", 0);
	SetInventory("ReceivedDialogID", 0);
	SetInventory("DarkWanderer_Artifact", 0);
	SetInventory("PlayerIsLeeching", 0);
	SetInventory("LifeStealAmount", 0);
	
	// some buffs from spells, that arent powerups
	SetInventory("Rally_DamageBuff", 0);
	
	// debuffs
	SetInventory("RuinationStacks", 0);
}

void SyncResearchInvestments(int pnum) {
	for(int i = 0; i < MAX_RESEARCHES; ++i) {
		if(ResearchInvestments[pnum][i])
			ACS_NamedExecuteWithResult("DnD Menu Investment Sync", pnum, i, ResearchInvestments[pnum][i]);
	}
}

void PostPlayerLoadRoutine(int pnum) {
	// Load period is finished, now check for level stats
	PlayerDied[pnum] = 0;
	PlayerTransferred[pnum] = false;
	TakeInventory("DnD_PDead", 1);
	SpawnedChests = 1;
	SetInventory("CanLoad", 0); //Usually it is 0 at this point, but make sure it is anyways.
	PlayerCanLoad[pnum] = 0;
	
	// we can synchronize the research investments now, load period etc. all finished
	SyncResearchInvestments(pnum);
	
	// force sync client stuff
	GiveInventory("DnD_SyncMe", 1);
	ACS_NamedExecuteAlways("DnD Force Sync Player Bonuses", 0);

	if(HasMasteredPerk(STAT_WIS))
		++CurrentLevelData[LEVELDATA_WISDOMMASTERED];
	if(HasMasteredPerk(STAT_GRE))
		++CurrentLevelData[LEVELDATA_GREEDMASTERED];
}

void UnfreezePlayer() {
	SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
	SetPlayerProperty(0, 0, PROP_INVULNERABILITY);
	TakeInventory("P_Frozen", 1);
	GiveInventory("DontAttackRemove", 1);
}

void IncrementPlayerLifetimeKills() {
	// this is the kills in level
	GiveInventory("DnD_Kills", 1);
	
	// we store 3 variables: billions, millions and thousands
	// for every million, we reduce thousands to 1 mil - X, and increment millions by 1, and similar or billions (we subtract 1000 instead from millions)
	int ltk = CheckInventory("DnD_LifeTimeKills");
	int ltk_m = CheckInventory("DnD_LifeTimeKills_Millions");
	SetInventory("DnD_LifeTimeKills_Billions", CheckInventory("DnD_LifeTimeKills_Billions") + (ltk_m + 1) == 1000);
	SetInventory("DnD_LifeTimeKills_Millions", (ltk_m + (ltk + 1 == 1000000)) % 1000);
	SetInventory("DnD_LifeTimeKills", (ltk + 1) % 1000000);
}

#include "DnD_Damage.h"