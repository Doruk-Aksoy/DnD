#include "DnD_Common.h"
#include "DnD_Elites.h"
#include "DnD_Database.h"
#include "DnD_MenuConstants.h"
#include "DnD_TempWeps.h"
#include "DnD_Ammo.h"
#include "DnD_Monsters.h"
#include "DnD_SpecialTrails.h"
#include "DnD_ChestKey.h"
#include "DnD_Skills.h"
#include "DnD_Settings.h"
#include "DnD_ClassMenu.h"
#include "DnD_Mugshot.h"
#include "DnD_Research.h"
#include "DnD_Statistics.h"
#include "DnD_Scoreboard.h"

enum {
	ITEMFILTER_STACKABLE = 1,
	ITEMFILTER_CHARM = 2,
	ITEMFILTER_ARMOR = 4
};

#define DND_EXP_BASEFACTOR 4
#define DND_CREDIT_BASEFACTOR 5
#define DND_RESEARCH_MAX_CHANCE 1.0
#define DND_RESEARCH_DROPMULT 3

void SetupCVarTracking() {
	CVarValues[DND_CVAR_RESEARCHDROPRATE] = Clamp_Between(GetCVar("dnd_researchdroprate"), 0.0, DND_RESEARCH_MAX_CHANCE);
	CVarValues[DND_CVAR_ACCESSORYDROPRATE] = (Clamp_Between(GetCVar("dnd_accessory_droprate"), 0, 100) * 1.0) / 100;
	CVarValues[DND_CVAR_ACCESSORYLEVEL] = Clamp_Between(GetCVar("dnd_accessorylevel"), 1, 100);
}

void HandlePlayerPainSound(int pclass) {
	int curhp = GetActorProperty(0, APROP_HEALTH);
	int maxhp = GetSpawnHealth();
	int hpratio = (curhp * 100) / maxhp;
	switch(pclass) {
		case DND_PLAYER_DOOMGUY:
			PlaySound(0, "Doomguy/Pain1", CHAN_BODY, 1.0);
		break;
		case DND_PLAYER_MARINE:
			// marine -- has 2 sounds for each quarter like so: Player/Pain25
			if(hpratio <= 25)
				PlaySound(0, "Player/Pain25", CHAN_BODY, 1.0);
			else if(hpratio <= 50)
				PlaySound(0, "Player/Pain50", CHAN_BODY, 1.0);
			else if(hpratio <= 75)
				PlaySound(0, "Player/Pain75", CHAN_BODY, 1.0);
			else
				PlaySound(0, "Player/Pain100", CHAN_BODY, 1.0);
		break;
		case DND_PLAYER_HOBO:
			// hobo
			if(hpratio <= 50)
				PlaySound(0, "Hobo/HardPain", CHAN_BODY, 1.0);
			else
				PlaySound(0, "Hobo/SoftPain", CHAN_BODY, 1.0);
		break;
		case DND_PLAYER_PUNISHER:
			// punisher
			if(hpratio <= 50)
				PlaySound(0, "Punisher/Pain", CHAN_BODY, 1.0);
			else
				PlaySound(0, "Punisher/PainLow", CHAN_BODY, 1.0);
		break;
		case DND_PLAYER_WANDERER:
			PlaySound(0, "Wanderer/Pain", CHAN_BODY, 1.0);
		break;
		case DND_PLAYER_CYBORG:
			// cyborg
			if(hpratio <= 50)
				PlaySound(0, "CyborgPlayer/PainLow", CHAN_BODY, 1.0);
			else
				PlaySound(0, "CyborgPlayer/PainHigh", CHAN_BODY, 1.0);
		break;
		case DND_PLAYER_BERSERKER:
			PlaySound(0, "BerserkerPlayer/Pain", CHAN_BODY, 1.0);
		break;
	}
}

void HandlePlayerDeathSound(int pclass, bool isXDeath) {
	str snd = "Doomguy/Death";
	switch(pclass) {
		case DND_PLAYER_DOOMGUY:
			if(isXDeath)
				snd = "Doomguy/XDeath";
			else
				snd = "Doomguy/Death";
		break;
		case DND_PLAYER_MARINE:
			snd = "Player/Die";
		break;
		case DND_PLAYER_HOBO:
			snd = "Hobo/Die";
		break;
		case DND_PLAYER_PUNISHER:
			snd = "Punisher/Die";
		break;
		case DND_PLAYER_WANDERER:
			snd = "Wanderer/Die";
		break;
		case DND_PLAYER_CYBORG:
			snd = "CyborgPlayer/Die";
		break;
		case DND_PLAYER_BERSERKER:
			snd = "Berserker/Die";
		break;
	}
	
	PlaySound(0, snd, CHAN_BODY, 1.0);
}

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

#define CHANCE_HEART 0.025

#define UPGRADETEXTID 6999
#define SURVIVEICO 7000
#define SURVIVETOPTEXTID 7001
#define SURVIVETEXTID 7002
#define SURVIVEID 7003
#define SURVIVETEXTID2 7004
#define SURVIVEBAKID 7005

#define VORTEXTIDSTART 10000
#define RAILINITTID 2000
#define RAILTIDADD 500

#define REFLECTFXTID 9000

#define INTERVENTION_DURATION TICRATE * 8

#define DND_EMERALD_TRANSLATIONID 7000

#define DND_BUDGET_BASE 3
#define DND_MAX_SHARE 4
#define DND_SPREE_AMOUNT 4 * TICRATE // 4 * 35
#define DND_SPREE_PER 10
#define DND_HALF_SPREE_PER (DND_SPREE_PER / 2)
#define DND_SPREE_TRIGGER (DND_SPREE_PER - 1) // -1 because current monster killed doesn't count
#define DND_SPREE_BASE 10 // 10%, this is dividing the exp
#define DND_SPECIALAMMO_TEXTID 6001
#define DND_SPREE_BLOODID 6000
#define DND_SPREE_TEXT1ID 5999
#define DND_SPREE_TEXT2ID 5998

#define DND_MAX_MONSTERDELAY 8

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
#define DND_RESIST_TRANSLATION 72

#define DND_SOULAMMO_DROPRATE 0.03 // 3% chance
#define DND_SOULAMMO_STEALERUPGRADE 0.02 // 2% more chance
#define DND_SOULAMMO_SMALLCHANCE 75

#define DND_BOSSCHEST_DROPRATE 0.33 // 33% chance

#define DND_ITEM_LINGER_TIME 120

#define DND_MARINE_SELFRESIST 25 // 25%

#define DND_WANDERER_EXP_DAMAGE 20 // 1 / 20 = 5%

// RPG ELEMENTS END

#define DefStepSound "Player/Move"

#define DND_MAX_PULLFORCE 256.0
enum {
	DND_PULLF_CANPULLDEAD = 1,
	DND_PULLF_REVERSEDIR = 2
};

#define MAX_GRAVDIS_COUNT 512
#define GRAVDIS_DELAY_TIME 15 // 3 seconds is total time, 3 x (35 / 7) = 15
#define GRAVDIS_CHECK_PERIOD 7
#define GRAVDIS_MINPULLVEL 2.0
#define GRAVDIS_MAXPULLVEL 32.0
#define GRAVDIS_FLING_FACTOR 128
#define GRAVDIS_HEIGHT_FACTOR 15
#define GRAVDIS_HEIGHTADD_PER 64
#define GRAVDIS_CIRCLE_PARTICLES 48

#define CHARON_PARTICLE_DENSITY 8

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
#define MAX_SPREE_MULT 10 // maximum 10 killing sprees will affect your exp gain
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

bool UniqueMonsterAvailability[MAX_MONSTER_CATEGORIES] = { 0 };

void Reset_RPGInfo (int resetflags) {
	int i;
	int pnum = PlayerNumber();
	if(resetflags & RESET_LEVEL) {
		SetPlayerExp(pnum, 0);
		PlayerDataInLevel[PlayerNumber()].levelexp = 0;
		SetInventory("Level", 1);
	}
	
	if(resetflags & RESET_CREDIT)
		SetInventory("Credit", 0);
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

#define DND_MAPDIFF_TIERVAL 250
#define DND_MAXMAPDIFF 9

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
	
	// get the value -- <0 in case it overflows... somehow.. lol
	MapDifficulty = factor / DND_MAPDIFF_TIERVAL;
	if(MapDifficulty > DND_MAXMAPDIFF || MapDifficulty < 0)
		MapDifficulty = DND_MAXMAPDIFF;
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
				temp = GetActorLevelExperience(i + P_TIDSTART) * bval / 100;
				GiveActorInventory(i + P_TIDSTART, "DnD_KillBonusShower", 1);
				GiveActorExp(i + P_TIDSTART, temp);
			}
		}
	}
	else if(bonustype == BONUS_ITEM) {
		bval = CalculateBonus(BONUS_ITEM, MapDifficulty);
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(PlayerInGame(i) && isActorAlive(i + P_TIDSTART)) {
				temp = GetActorLevelCredits(i + P_TIDSTART) * bval / 100;
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

enum {
	MONSTERDATA_VARIATIONCOUNT,
	MONSTERDATA_WEIGHTSUM
};

// MAX_MONSTER_CATEGORIES
global int 21: MonsterCategoryData[][2];

void PickQuest() {
	/*if(GetCVar("dnd_enable_quests") && PlayerCount()) {
		if(GetCVar("dnd_quest_avglevel") <= (PlayerInformationInLevel[PLAYERLEVELINFO_LEVELATSTART] / PlayerCount()) && random(1, 100) <= Clamp_Between(GetCVar("dnd_quest_chance"), 1, 100)) {
			do {
				active_quest_id = random(0, MAX_QUESTS - 1);
			} while(!IsValidQuest(active_quest_id));
		}
	}
	else*/
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
				SpawnPlayerDrop(i, "ResearchModule_MP", 24.0, 16, i + 1, id);
		}
	}
	else if(!CheckResearchStatus(id)) // 1 before id is player (0 + 1)
		SpawnPlayerDrop(0, "ResearchModule_MP", 24.0, 16, 1, id);
}

void SpawnAccessory(int pnum) {
	int id;
	if(isSoftorHardcore()) {
		do {
			id = random(1, MAX_ACCESSORY);
		} while(id == DND_REPLACE_THIS_ACCESSORY);
	}
	else
		id = random(1, MAX_ACCESSORY);
	SpawnPlayerDrop(pnum, StrParam(s:"Accessory_", d:id), 0, 0, 0, 0);
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
	// the numbers above are different now that we have a different difficulty scale from 0-9 instead of 0-4
	CurrentLevelData[LEVELDATA_MAXCHESTS] = MAX_BASE_CHESTCOUNT + 5 * MapDifficulty / 2;
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
	// the tid of player who opened the chest
	int tid = GetActorProperty(0, APROP_TARGETTID);
	int pnum = tid - P_TIDSTART;

	if(ctype == DND_CHESTTYPE_BRONZE) {
		SpawnOrbForAll(random(1, 3));
		if(RunDefaultDropChance(pnum, 0.5))
			SpawnItemForAll(DND_ITEM_TOKEN);
	}
	else if(ctype == DND_CHESTTYPE_SILVER) {
		SpawnOrbForAll(random(3, 5));
		if(RunDefaultDropChance(pnum, 0.75))
			SpawnItemForAll(DND_ITEM_TOKEN);
		if(RunDefaultDropChance(pnum, 0.5))
			SpawnItemForAll(DND_ITEM_POWERCORE);
	}
	else if(ctype == DND_CHESTTYPE_GOLD) {
		SpawnOrbForAll(random(5, 8));
		SpawnItemForAll(DND_ITEM_TOKEN);
		SpawnItemForAll(DND_ITEM_POWERCORE);
		SpawnItemForAll(DND_ITEM_BODYARMOR);
	}
	
	// common to all chests, an extra orb can drop with 33% chance and another with 20%
	if(RunDefaultDropChance(pnum, 0.33))
		SpawnOrbForAll(1);
	if(RunDefaultDropChance(pnum, 0.2))
		SpawnOrbForAll(1);
}

enum {
	DND_LOOTBIT_ORB = 1,
	DND_LOOTBIT_TOKEN = 2,
	DND_LOOTBIT_ARMOR = 4,
	DND_LOOTBIT_CHARM = 8,
	DND_LOOTBIT_POWERCORE = 16,
	DND_LOOTBIT_CHESTKEY = 32
};

enum {
	DND_LOOT_ORB,
	DND_LOOT_TOKEN,
	DND_LOOT_ARMOR,
	DND_LOOT_CHARM,
	DND_LOOT_POWERCORE,
	DND_LOOT_CHESTKEY
};
#define DND_MAX_LOOTBITS 6

// drop boost increases chance for a drop, rarity is for chance for it to be unique
void HandleItemDrops(int tid, int m_id, int drop_boost, int rarity_boost) {
	bool ignoreWeight = GetCVar("dnd_ignore_dropweights");
	bool mon_robot = IsActorRobotic(tid);
	int tmp;

	for(int i = 0; i < MAXPLAYERS; ++i) {
		// run each player's chance, drop for corresponding player only
		if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART)) {
			// for orbs
			int bits = 0;
			int p_chance = GetDropChance(i);

			if(ignoreWeight || RunPrecalcDropChance(p_chance, DND_ELITE_BASEDROP_ORB * drop_boost / 100, m_id, DND_MON_RNG_1)) {
				SpawnOrb(i, true);
				bits |= DND_LOOTBIT_ORB;
			}

			// for tokens -- same likelihood to drop as orbs
			if(ignoreWeight || RunPrecalcDropChance(p_chance, DND_ELITE_BASEDROP * drop_boost / 100, m_id, DND_MON_RNG_2)) {
				SpawnToken(i);
				bits |= DND_LOOTBIT_TOKEN;
			}

			if(ignoreWeight || RunPrecalcDropChance(p_chance, DND_BASEARMOR_DROP * drop_boost / 100, m_id, DND_MON_RNG_3)) {
				// boot and body armor chance is equal
				tmp = random(1, 100);
				if(tmp <= 33)
					SpawnArmor(i, rarity_boost, 0);
				else if(tmp <= 66)
					SpawnBoot(i, rarity_boost);
				else
					SpawnHelm(i, rarity_boost);
				bits |= DND_LOOTBIT_ARMOR;
			}

			if(ignoreWeight || RunPrecalcDropChance(p_chance, DND_BASE_CHARMRATE * drop_boost / 100, m_id, DND_MON_RNG_4)) {
				SpawnCharm(i, rarity_boost);
				bits |= DND_LOOTBIT_CHARM;
			}

			if(ignoreWeight || (mon_robot && RunPrecalcDropChance(p_chance, DND_BASE_POWERCORERATE * drop_boost / 100, m_id, DND_MON_RNG_4))) {
				SpawnPowercore(i, rarity_boost);
				bits |= DND_LOOTBIT_POWERCORE;
			}
			
			if(ignoreWeight || RunPrecalcDropChance(p_chance, DND_CHESTKEY_DROPRATE * drop_boost / 100, m_id, DND_MON_RNG_2)) {
				SpawnChestKey(i);
				bits |= DND_LOOTBIT_CHESTKEY;
			}

			// luck mastery check for inventory items --- they need special handling
			for(int j = 0; j < DND_MAX_LOOTBITS; ++j) {
				if(IsSet(bits, j) && CheckPlayerLuckDuplicator(i)) {
					switch(j) {
						case DND_LOOT_ORB:
							SpawnOrb(i, true);
						break;
						case DND_LOOT_TOKEN:
							SpawnToken(i);
						break;
						case DND_LOOT_ARMOR:
							tmp = random(1, 100);
							if(tmp <= 33)
								SpawnArmor(i, rarity_boost, 0);
							else if(tmp <= 66)
								SpawnBoot(i, rarity_boost);
							else
								SpawnHelm(i, rarity_boost);
						break;
						case DND_LOOT_CHARM:
							SpawnCharm(i, rarity_boost);
						break;
						case DND_LOOT_POWERCORE:
							SpawnPowercore(i, rarity_boost);
						break;
						case DND_LOOT_CHESTKEY:
							SpawnChestKey(i);
						break;
					}
				}
			}
		}
	}
}

void HandleCreditExp_Regular(int this, int target, int m_id) {
	int expshare = GetCVar("dnd_sharexp");
	int creditshare = GetCVar("dnd_sharecredit");
	
    // decide how exp/credit base is calculated
	int exptemp = MonsterProperties[m_id].gain;
	int credtemp = exptemp / DND_CREDITGAIN_FACTOR;
    int pnum = 0, i = MonsterProperties[m_id].level;
	
	if(credtemp < DND_MIN_CREDIT)
		credtemp = DND_MIN_CREDIT;
	
	int pcount = Clamp_Between(PlayerCount(), 1, DND_MAX_SHARE);
	int expscale = Clamp_Between(GetCVar("dnd_exp_scale"), 1, EXP_SCALE_MAX);
	int creditscale = Clamp_Between(GetCVar("dnd_credit_scale"), 1, CREDIT_SCALE_MAX);
	int addone = 0;

	// if full share is on we won't divide by player count, but if it isn't we will
	if(!GetCVar("dnd_fullshare")) {
		exptemp = (exptemp * expscale) / pcount;
		if(exptemp < MIN_EXP_GAIN)
			exptemp = MIN_EXP_GAIN;
		credtemp = (credtemp * creditscale) / pcount;
		if(credtemp < MIN_CREDIT_GAIN)
			credtemp = MIN_CREDIT_GAIN;
	}
	else {
		exptemp *= expscale;
		credtemp *= creditscale;
	}
	
	// from here on out the scale variables and pcount are useless
	pcount = GetCVar("dnd_gainrange");
	
	// reuse old variable -- we dont need killerplayer w.e info
	HandleMonsterTemporaryWeaponDrop(this, m_id, target - P_TIDSTART, false);

	if(expshare || creditshare) {
		for(i = 0; i < MAXPLAYERS; ++i) {
			expscale = 0;
			creditscale = 0;
			pnum = P_TIDSTART + i;
			if(IsActorAlive(pnum) && pnum != target) { // dont give twice
				// check if range flag is on, if it is check the range
				addone = AproxDistance(GetActorX(target) - GetActorX(pnum), GetActorY(target) - GetActorY(pnum)) >> 16 <= pcount;
				if(expshare && exptemp && (!GetCVar("dnd_gainonlyinrange") || addone || HasActorMasteredPerk(pnum, STAT_WIS)))
					expscale = RewardActorExp(pnum, exptemp);
					
				if(creditshare && credtemp && (!GetCVar("dnd_gainonlyinrange") || addone || HasActorMasteredPerk(pnum, STAT_GRE)))
					creditscale = RewardActorCredit(pnum, credtemp);
				
				// if something could be provided, show it to user
				if(expscale || creditscale) {
					SetActivator(pnum);
					ACS_NamedExecuteWithResult("DND Show Kill Digits", pnum, expscale, creditscale);
				}
			}
		}
	}
	
	// now give target his stuff (if expshare or creditshare, one of them was not on the target would not receive it in the loop)
	expscale = RewardActorExp(target, exptemp);
	creditscale = RewardActorCredit(target, credtemp);
	SetActivator(target);
	ACS_NamedExecuteWithResult("DND Show Kill Digits", target, expscale, creditscale);
}

void HandleCreditExp_MasteryCheck(int this, int target, int m_id) {
	int expshare = GetCVar("dnd_sharexp");
	int creditshare = GetCVar("dnd_sharecredit");
	
    // decide how exp/credit base is calculated
	int exptemp = MonsterProperties[m_id].gain;
	int credtemp = exptemp / DND_CREDITGAIN_FACTOR;
    int pnum = 0, i = MonsterProperties[m_id].level;
	
	if(credtemp < DND_MIN_CREDIT)
		credtemp = DND_MIN_CREDIT;
	
	int pcount = Clamp_Between(PlayerCount(), 1, DND_MAX_SHARE);
	int expscale = Clamp_Between(GetCVar("dnd_exp_scale"), 1, EXP_SCALE_MAX);
	int creditscale = Clamp_Between(GetCVar("dnd_credit_scale"), 1, CREDIT_SCALE_MAX);
	
	// if full share is on we won't divide by player count, but if it isn't we will
	if(!GetCVar("dnd_fullshare")) {
		exptemp = (exptemp * expscale) / pcount;
		if(exptemp < MIN_EXP_GAIN)
			exptemp = MIN_EXP_GAIN;
		credtemp = (credtemp * creditscale) / pcount;
		if(credtemp < MIN_CREDIT_GAIN)
			credtemp = MIN_CREDIT_GAIN;
	}
	else {
		exptemp *= expscale;
		credtemp *= creditscale;
	}

	// from here on out the scale variables and pcount are useless
	pcount = GetCVar("dnd_gainrange");
	expshare = 0;
	creditshare = 0;
	
	for(i = 0; i < MAXPLAYERS; ++i) {
		expscale = 0;
		creditscale = 0;
		target = P_TIDSTART + i;
		if(IsActorAlive(target)) {
			if(HasActorMasteredPerk(target, STAT_WIS)) {
				expscale = RewardActorExp(target, exptemp);
				++expshare;
			}
			
			if(HasActorMasteredPerk(target, STAT_GRE)) {
				creditscale = RewardActorCredit(target, credtemp);
				++creditshare;
			}
			
			if(expscale || creditscale) {
				SetActivator(target);
				ACS_NamedExecuteWithResult("DND Show Kill Digits", target, expscale, creditscale);
			}
			
			// quit early if we concluded all mastery players
			if(expshare == CurrentLevelData[LEVELDATA_WISDOMMASTERED] && creditshare == CurrentLevelData[LEVELDATA_GREEDMASTERED])
				break;
		}
	}
	
	// guaranteed temp weapon drop
	HandleMonsterTemporaryWeaponDrop(this, m_id, -1, true);
}

// loc_tid is a potential drop location
void HandleLootDrops(int tid, int target, bool isElite = false, int loc_tid = -1) {
	if(!IsPlayer(target))
		return;

	int m_id = tid - DND_MONSTERTID_BEGIN;
	int pnum = target - P_TIDSTART;
	int temp;
	int p_chance = GetDropChance(pnum);
	
	// ammo specialty drop
	if(HasActorMasteredPerk(target, STAT_MUN) && RunPrecalcDropChance(p_chance, DND_MUNITION_MASTERY_CHANCE, m_id, DND_MON_RNG_1))
		SpawnPlayerDrop(pnum, "DnD_AmmoToken", 24.0, 16, 0, 0);
	
	// research drop
	if(!GetCVar("dnd_allresearchesfound")) {
		// make it less likely to drop
		// addone is the chance here (reusing old variables)
		temp = random(0, DND_RESEARCH_DROPMULT * DND_RESEARCH_MAX_CHANCE);

		
		if(GetCVar("dnd_ignore_dropweights") || RunPrecalcDropChance(p_chance, CVarValues[DND_CVAR_RESEARCHDROPRATE] * MonsterProperties[m_id].droprate / 100, m_id, DND_MON_RNG_2))
			SpawnResearch(pnum);
	}
	
	// if elite, roll orb and equipment drops
	// new: we let every monster drop orbs, not just elites but with an overall lower chance
	//if(GetCVar("dnd_ignore_dropweights") || isElite) {
		// handle orb drops
	HandleItemDrops(tid, m_id, MonsterProperties[m_id].droprate, MonsterProperties[m_id].rarity_boost);
	//}
	
	// accessory drops (accept only from cyber and spider masterminds)
	//#ifdef ISDEBUGBUILD
	//	SpawnAccessory();
	//#else
	if(
		IsBossTID(tid) && 
		random(0, 1.0) <= CVarValues[DND_CVAR_ACCESSORYDROPRATE] &&
		GetAveragePlayerLevel() >= CVarValues[DND_CVAR_ACCESSORYLEVEL]
	)
	{
		// we can drop the spawner
		SpawnAccessory(pnum);
	}
	//#endif
	
	#ifdef ISDEBUGBUILD
		//SpawnElixir(0);
		SpawnCharm(0, true);
		SpawnOrb(0, true);
		//SpawnToken(0, true);
	#endif
	
	// soul ammo drop -- considers ability - soulstealer as well
	if
	(
		CanDropSoulAmmoTID(tid) && 
		RunPrecalcDropChance(
			p_chance, 
			(1 + CheckActorInventory(tid, "MagicCausedDeath")) * (DND_SOULAMMO_DROPRATE + DND_SOULAMMO_STEALERUPGRADE * CheckActorInventory(target, "Ability_SoulStealer")) * MonsterProperties[m_id].droprate / 100, 
			m_id, 
			DND_MON_RNG_4
		)
	)
	{
		if(random(1, 100) <= DND_SOULAMMO_SMALLCHANCE)
			SpawnPlayerDrop(pnum, "SoulsDrop", 24.0, 16, 0, 0);
		else
			SpawnPlayerDrop(pnum, "LargeSoulsDrop", 24.0, 16, 0, 0);
		TakeActorInventory(tid, "MagicCausedDeath", 1);
	}
	
	// doomguy drop -- we dont multiply with MonsterProperties[m_id].droprate here as it's for loot only
	if(CheckActorInventory(target, "Doomguy_Perk25") && RunPrecalcDropChance(p_chance, DND_DOOMGUY_DROPCHANCE, m_id, DND_MON_RNG_3)) {
		temp = MonsterProperties[m_id].maxhp;
		SpawnPlayerDrop(pnum, "Doomguy_DemonSoul", 24.0, 16, temp & 0xFFFF, temp >> 16);
	}

	if(IsMonsterIdZombie(m_id))
		GiveActorInventory(target, "DnD_ShotUndead", 1);
		
	if(CheckActorInventory(tid, "BookofDeadCausedDeath")) {
		GiveActorInventory(target, "Souls", (1 + DND_BOSS_SOULGIVE * IsBossTID(tid)) * (100 + GetPlayerAttributeValue(pnum, INV_EX_PICKUPS_MORESOUL)) / 100);
		Spawn("SoulEffectSpawner", GetActorX(0), GetActorY(0), GetActorZ(0));
		TakeActorInventory(tid, "BookofDeadCausedDeath", 1);
	}
	
	if(CheckActorInventory(target, "Ability_HeartSeeker") && RunDefaultDropChance(pnum, CHANCE_HEART * MonsterProperties[m_id].droprate / 100))
		SpawnPlayerDrop(pnum, "DemonHeartPickup", 24.0, 16, 0, 0);
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

int GetWeaponSlotFromFlag(int flags) {
	// the first 14 bits are unnecessary
	flags >>= DND_SHIFTBITS_FOR_SLOTFROMFLAG;
	for(int i = 0; i < 32; ++i)
		if(IsSet(flags, i))
			return i;
	return 0;
}

int ScaleMonster(int tid, int m_id, int pcount, int realhp, bool isSummoned) {
	int base = realhp;
	int add = 0, level = 1, low, high, temp;
	level = PlayerInformationInLevel[PLAYERLEVELINFO_LEVELATSTART] / pcount;
	// ensure minions use master's level -- do so only if its summoned, boss tier monsters have tids on the spawners that can mess this up during mapload!!!
	if(GetActorProperty(0, APROP_MASTERTID) && isSummoned)
		level = MonsterProperties[GetActorProperty(0, APROP_MASTERTID) - DND_MONSTERTID_BEGIN].level;
	else if(GetCVar("dnd_randomize_levels")) {
		if(!IsUniqueBossMonster(m_id)) {
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
		else // level of unique boss is always level of highest player
			level = PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL];
	}
	if(GetCVar("dnd_monsterlevel_behind"))
		level = Clamp_Between(level, 1, PlayerInformationInLevel[PLAYERLEVELINFO_LEVELATSTART] / pcount);
	level = Clamp_Between(level, 1, GetCVar("dnd_maxmonsterlevel"));
	if(level > 1) {
		add = GetMonsterHPScaling(m_id, level);
		
		// % increase per player adding
		if(GetCVar("dnd_playercount_scales_monsters"))
			add += DND_MONSTERHP_PLAYERSCALE * (Clamp_Between(pcount - 1, 0, DND_MAX_PLAYERHPSCALE));
		
		add = base * add / 100;

		// add level factor to it
		// first overflow check
		if(add > INT_MAX - base)
			add = INT_MAX - base;
	}
	MonsterProperties[m_id].basehp = base;
	MonsterProperties[m_id].maxhp = base + add;
	MonsterProperties[m_id].level = level;
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
		
		// punisher perk -- be on cruel or more
		int temp = CheckInventory("DnD_MultikillCounter") + 1;
		if(temp / DND_SPREE_PER >= 1) {
			// punisher perks
			if(CheckInventory("Punisher_Perk5")) {
				if(!CheckInventory("Punisher_Perk5_MoveSpeed")) {
					int wepid = CheckInventory("DnD_WeaponID");
					if(GetSlotOfWeapon(wepid) != 9) {
						if(Weapons_Data[wepid].ammo_name1 != "" && Weapons_Data[wepid].ammo_name1 != "Souls") {
							GiveInventory(Weapons_Data[wepid].ammo_name1, GetAmmoCapacity(Weapons_Data[wepid].ammo_name1) / 10);
							LocalAmbientSound("items/ammo", 90);
						}
						if(Weapons_Data[wepid].ammo_name2 != "" && Weapons_Data[wepid].ammo_name2 != "Souls") {
							GiveInventory(Weapons_Data[wepid].ammo_name2, GetAmmoCapacity(Weapons_Data[wepid].ammo_name2) / 10);
							LocalAmbientSound("items/ammo", 90);
						}
					}
				}
				GiveInventory("Punisher_Perk5_MoveSpeed", 1);
			}
		}
		else if(temp / DND_HALF_SPREE_PER >= 1 && GetArmorID() == BODYARMOR_RAVAGER) {
			// ravager armor activation on killing spree
			GiveInventory("RavagerPower", 1);
		}
	}
	// give spree counter
	GiveInventory("DnD_SpreeTimer", DND_SPREE_AMOUNT);
}

void ResetPlayerScriptChecks() {
	for(int i = 0; i < MAX_SCRIPT_TRACK; ++i)
		for(int j = 0; j < MAXPLAYERS; ++j)
			PlayerScriptsCheck[i][j] = false;
}

void HandleMonsterTemporaryWeaponDrop(int this, int id, int pnum, bool guaranteed = false) {
	id = MonsterProperties[id].id;
	switch(id) {
		case MONSTER_BLOODFIEND:
		case MONSTER_RAVAGER:
		case MONSTER_LURKER:
			if(guaranteed || RunDefaultDropChance(pnum, TEMPWEP_BLOODFIENDSPINE_DROPCHANCE))
				SpawnPlayerDropAtActor(pnum, this, TemporaryWeaponData[DND_TEMPWEP_BLOODFIENDSPINE][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_VULGAR:
			if(guaranteed || RunDefaultDropChance(pnum, TEMPWEP_VENOM_DROPCHANCE))
				SpawnPlayerDropAtActor(pnum, this, TemporaryWeaponData[DND_TEMPWEP_VENOM][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_CHAINGUNGENERAL:
			if(guaranteed || RunDefaultDropChance(pnum, TEMPWEP_NAILGUN_DROPCHANCE))
				SpawnPlayerDropAtActor(pnum, this, TemporaryWeaponData[DND_TEMPWEP_HEAVYNAILGUN][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_DEATHKNIGHT:
		case MONSTER_HORSHACKER:
			if(guaranteed || RunDefaultDropChance(pnum, TEMPWEP_SOULRENDER_DROPCHANCE))
				SpawnPlayerDropAtActor(pnum, this, TemporaryWeaponData[DND_TEMPWEP_SOULRENDER][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_CORPULENT:
			if(guaranteed || RunDefaultDropChance(pnum, TEMPWEP_HFCANNON_DROPCHANCE))
				SpawnPlayerDropAtActor(pnum, this, TemporaryWeaponData[DND_TEMPWEP_HELLFORGECANNON][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_DARKSERVANT:
			if(guaranteed || RunDefaultDropChance(pnum, TEMPWEP_DARKGLOVES_DROPCHANCE))
				SpawnPlayerDropAtActor(pnum, this, TemporaryWeaponData[DND_TEMPWEP_DARKGLOVES][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
	}
}

void ApplyRandomCurse(int tid) {
	// nasty rushed code here, clean up later with improved curse system
	int curse_id = random(0, 7);
	switch(curse_id) {
		case 0:
			GiveActorInventory(tid, "DnD_Curse_RedLich", 1);
		break;
		case 1:
			GiveActorInventory(tid, "DnD_Curse_Hierophant", 1);
		break;
		case 2:
			GiveActorInventory(tid, "DnD_Curse_Guru", 1);
		break;
		case 3:
			GiveActorInventory(tid, "DnD_Curse_LichVisionDim", 1);
		break;
		case 4:
			GiveActorInventory(tid, "DnD_Curse_LichSlow", 1);
		break;
		case 5:
			GiveActorInventory(tid, "DnD_Curse_LichPoison", 1);
		break;
		case 6:
			GiveActorInventory(tid, "DnD_Curse_Ensnare", 1);
		break;
		case 7:
			GiveActorInventory(tid, "DnD_Curse_Paladin", 1);
		break;
	}
}

void HandleRuination(int this, int target) {
	TakeInventory("RuinationStacks", RUINATION_MAX_STACKS);
	PlaySound(this, "Elite/RuinationTrigger", CHAN_5, 1.0);
	GiveInventory("RuinBlend", 1);
	GiveInventory("RuinationHardDebuff", 1);
}

bool IsEliteException(int m_id, int monster_type) {
	return 	CheckFlag(0, "FRIENDLY") || isUniqueBossMonster(m_id) || MonsterProperties[m_id].trait_list[DND_SUMMONED] || MonsterProperties[m_id].trait_list[DND_LEGENDARY] || CheckInventory("DnD_EliteException") ||
			monster_type == MONSTER_PHANTASM || monster_type == MONSTER_WRAITH || monster_type == MONSTER_HADESSPHERE || monster_type == MONSTER_UNDEADPRIESTGHOST;
}

void HandleUniqueDeath(int p_actor, int unique_id) {
	int pnum = p_actor - P_TIDSTART;
	switch(unique_id) {
		case MONSTER_TERON:
			// spawn some budget
			SpawnForced("BudgetDropper_Medium", GetActorX(0), GetActorY(0), GetActorZ(0) + 16.0);
			SpawnForced("BudgetDropper_Medium", GetActorX(0), GetActorY(0), GetActorZ(0) + 16.0);
			SpawnForced("BudgetDropper_Medium", GetActorX(0), GetActorY(0), GetActorZ(0) + 16.0);
		break;
		case MONSTER_GANT:
			SpawnArmor(pnum, 0, -BODYARMOR_SYNTHMETAL);
		break;
		case MONSTER_BRONN:
			SpawnHelm(pnum, 0, HELMS_SYNTHMETAL);
		break;
		case MONSTER_VAAJ:
			// drops vaaj influenced charm: explosives ignore resists mod guaranteed
			SpawnCharmWithMods_ForAll(INV_ESS_VAAJ);
		break;
		case MONSTER_REMUS:
			// drops armor and gives research for it: lightning coil - super lightning protection, 300, 40% damage reduction. When hurt fires lightning bolts.
			SpawnArmor(pnum, 0, -BODYARMOR_LIGHTNINGCOIL);
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
	return PlayerInformationInLevel[PLAYERLEVELINFO_LEVELATSTART] / temp;
}

void ClearLingeringBuffs() {
	SetInventory("AllMapOnlyOnce", 0);
	SetInventory("Punisher_Perk50_Counter", 0);
	SetInventory("Berserker_DamageTracker", 0);
	SetInventory("Berserker_DamageTimer", 0);
	SetInventory("Berserker_HitTracker", 0);
	SetInventory("Berserker_HitTimer", 0);
	SetInventory("Berserker_NoRoar", 0);
	SetInventory("Berserker_Perk50_HitCounter", 0);
	SetInventory("ReceivedDialogID", 0);
	SetInventory("DarkWanderer_Artifact", 0);
	SetInventory("PlayerIsLeeching", 0);
	SetInventory("LifeStealAmount", 0);

	SetInventory("Hobo_ShotgunFrenzyTimer", 0);

	SetInventory("Marine_DamageReduction_Timer", 0);
	SetInventory("Marine_Perk50_DamageDealt", 0);
	SetInventory("Marine_Perk50_Cooldown", 0);
	SetInventory("Marine_Perk50_DamageTaken", 0);

	SetInventory("Cyborg_InstabilityStack", 0);
	SetInventory("Cyborg_Instability_Timer", 0);
	SetInventory("Cyborg_NoAnim", 0);
	SetInventory("EShieldChargeNow", 0);

	// rework this when we add persistent pets
	SetInventory("PetCounter", 0);
	
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
	PlayerDatabaseState[pnum][PLAYER_TRANSFERSTATE] = false;
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

void CheckUniqueBossPossibility(int tid, int m_id) {
	if(isUniqueBossMonster(m_id)) {
		DungeonBossData[BOSSDATA_TID] = tid;

		// sync to cs
		ACS_NamedExecuteWithResult("DnD Register Unique Boss", tid, MonsterProperties[m_id].id, MonsterProperties[m_id].maxhp, MonsterProperties[m_id].level);
		ACS_NamedExecuteWithResult("DnD Constant Unique Boss Sync", tid);
	}
}

// checks end of level, if it is and the gamemode is hard/softcore we save players
// called in death or spectate case -- checked with a variable
void CheckEOL(bool isSpectate, int game_mode = -1) {
	if(game_mode == -1)
		game_mode = GetCVar("dnd_mode");
		
	//Log(s:"check hardcore ", d:HardcoreSet, s: " ", d:game_mode, s: " ", d:DND_MODE_HARDCORE, s: " ", d:DND_MODE_SOFTCORE);
		
	if(!isSoftorHardcore() || (game_mode != DND_MODE_HARDCORE && game_mode != DND_MODE_SOFTCORE))
		return;
	
	//Log(s:"spec? ", d:isSpectate, s: " ", d:PlayerCount());
	bool isEOL = true;
	
	// if not spectate, check if everyones dead otherwise check if no players left
	if(!isSpectate) {
		for(int i = 0; i < MAXPLAYERS; ++i) {
			// alive or has more lives left
			if(PlayerInGame(i) && (IsActorAlive(i + P_TIDSTART) || CheckActorInventory(i + P_TIDSTART, "CurrentLives"))) {
				isEOL = false;
				break;
			}
		}
	}
	else
		isEOL = !PlayerCount();
	
	if(isEOL/* && TransactionMade*/)
	{
		// end of level detected, push database changes
		Log(s:"End of level reached.");
		//FinishDBTransaction();
		
		// level end reached, reset information
		ResetPlayerInformationLevel();
	}
}

void HandlePlayerDataSave(int pnum, bool isDisconnect = false, int game_mode = -1) {
	if(game_mode == -1)
		game_mode = GetCVar("dnd_mode");

	// the last check needs it to see if players are left because if the last player quits then it's not in progress anymore anyway...
	int gstate = GetGameModeState();
	/*Log(
		d:!isSoftorHardcore(),
		s:" ",
		d:PlayerLoaded[pnum]
	);*/
	if(!isSoftorHardcore() || (game_mode != DND_MODE_HARDCORE && game_mode != DND_MODE_SOFTCORE) || gstate == GAMESTATE_COUNTDOWN || !PlayerLoaded[pnum])
		return;

	PlayerLoaded[pnum] = 0;

	if(!PlayerIsLoggedIn(pnum) || !PlayerDatabaseState[pnum][PLAYER_SAVESTATE])
		return;
		
	if(!isDisconnect) {
		Log(s:"Save player ", d:pnum, s: " activites on death for char id ", d:PlayerActivities[pnum].char_id);

		BeginDBTransaction();

		if(PlayerDatabaseState[pnum][PLAYER_TRANSFERSTATE]) {
			Log(s:"Transferring player data.");
			WipeoutPlayerData(pnum, CheckActorInventory(pnum + P_TIDSTART, "DnD_CharacterID"));
			SetActorInventory(pnum + P_TIDSTART, "DnD_CharacterID", CheckActorInventory(pnum + P_TIDSTART, "DnD_TransfCharacterID"));
			PlayerActivities[pnum].char_id = CheckActorInventory(pnum + P_TIDSTART, "DnD_TransfCharacterID");
			PlayerDatabaseState[pnum][PLAYER_TRANSFERSTATE] = false;
		}
		
		SavePlayerActivities(pnum, PlayerActivities[pnum].char_id);
		ResetPlayerActivities(pnum, false);
		EndDBTransaction();
	}
	else {
		Log(s:"Save player ", d:pnum, s: " activites on disconnect for char id ", d:PlayerActivities[pnum].char_id);

		BeginDBTransaction();
		SavePlayerActivities(pnum, PlayerActivities[pnum].char_id);
		EndDBTransaction();

		// resets player activites already
		ResetPlayerInfo(pnum);
		ResetPlayerActivities(pnum, true);
		
		PlayerDatabaseState[pnum][PLAYER_SAVESTATE] = false;
	}
}

void SaveAllPlayerData() {
	Log(s:"Database save mode confirmed.");
	BeginDBTransaction();
	for(int i = 0; i < MAXPLAYERS; ++i) {
		// don't save peoples stuff while they are in load period
		//Log(d:i,s:": ",d:PlayerInGame(i), s:" " ,d:CheckActorInventory(i + P_TIDSTART, "CanLoad"), s:" ", d:PlayerDatabaseState[i][PLAYER_SAVESTATE]);
		if(PlayerInGame(i) && !CheckActorInventory(i + P_TIDSTART, "CanLoad") && PlayerDatabaseState[i][PLAYER_SAVESTATE]) {
			if (PlayerIsLoggedIn(i)) {
				// if transfer requested, wipeout old one and move to new one
				if(PlayerDatabaseState[i][PLAYER_TRANSFERSTATE]) {
					WipeoutPlayerData(i, CheckActorInventory(i + P_TIDSTART, "DnD_CharacterID"));
					SetActorInventory(i + P_TIDSTART, "DnD_CharacterID", CheckActorInventory(i + P_TIDSTART, "DnD_TransfCharacterID"));
					PlayerDatabaseState[i][PLAYER_TRANSFERSTATE] = false;
				}
				SavePlayerData(i, CheckActorInventory(i + P_TIDSTART, "DnD_CharacterID"));
				ResetPlayerActivities(i, false); // reset this player's activities for the map, no need for them to be stored anymore
				PlayerLoaded[i] = 1; //Also make sure the auto-save gets considered as loading a char - which will prevent unecessary loading periods.
				Log(s:"Saving player ", n:i + 1, s:"'s data.");
			}
			PlayerDatabaseState[i][PLAYER_SAVESTATE] = false; //This will prevent players that joined and logged in in intermission get the auto-saved character erased.
		}
	}
	EndDBTransaction();
}

void HandleEndOfLevelRewards(int pnum) {
	// for next map things -- give players their rewards and stuff coming from a previous map
	//Log(s:"run open");
	int temp = 0;
	//Log(s:"map changed");

	// Now using PlayerWillBeSaved, because some servers might use multiple lives setting.
	// if hardcore modes are set, check this, otherwise simply give the player the things if they managed to survive the level regularly
	if((!isSetupComplete(SETUP_STATE1, SETUP_HARDCORE) || PlayerDatabaseState[pnum][PLAYER_SAVESTATE])) {
		GiveInventory("LevelToken", 1);
		StatListOpened[pnum] = 0;
		
		// check if the map had at least 1 monster in it... so people don't cheese stupid "skip maps"...
		if(GetLevelInfo(LEVELINFO_TOTAL_MONSTERS)) {
			temp = 2 * (1 + isSetupComplete(SETUP_STATE1, SETUP_HARDCORE)) * ((MapDifficulty + 1) + Clamp_Between(GetCVar("dnd_budget_reward"), 1, 1000));
			GiveInventory("Budget", temp);
			ACS_NamedExecuteWithResult("DnD Map Beaten Reward Text", temp);
		}
		
		GiveInventory("RoundsSurvived", 1);
	}
	// Check quests
	if(active_quest_id != -1)
		CheckMapExitQuest(pnum, active_quest_id);
}

// registers a unique boss to clientside, can be used to reset it as well
Script "DnD Register Unique Boss" (int tid, int monid, int maxhp, int level) CLIENTSIDE {
	DungeonBossData[BOSSDATA_TID] = tid;
	
	int m_id = tid - DND_MONSTERTID_BEGIN;
	MonsterProperties[m_id].id = monid;
	MonsterProperties[m_id].maxhp = maxhp;
	MonsterProperties[m_id].level = level;
	
	SetResultValue(0);
}

#include "DnD_Damage.h"
#include "DnD_Weapons.h"