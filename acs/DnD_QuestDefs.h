#ifndef DND_QUESTDEFS_IN
#define DND_QUESTDEFS_IN

enum {
	QUEST_HPOVER50,
	QUEST_ONLYONEWEAPON,
	QUEST_NODYING,
	QUEST_10SPREES,
	QUEST_HEALFOR500,
	QUEST_KILL10BOSSES,
	QUEST_KILL20ELITES,
	QUEST_USENOARTIFACT,
	QUEST_ONLYENERGY,
	QUEST_NOHEALINGPICKUP,
	QUEST_ONLYPISTOLWEAPONS,
	QUEST_NOSHOTGUNS,
	QUEST_NOARMORS,
	QUEST_SPAREZOMBIES,
	QUEST_NOSUPERWEAPONS,
	QUEST_ONLYSPECIALAMMO,
	QUEST_SPEND25K,
	QUEST_KILLDREAMINGGOD,
	QUEST_KILLTORRASQUE,
	QUEST_KILLMORDECQAI,
	QUEST_KILLGODSLAYER,
	QUEST_KILLGOLGOTH
};
#define MAX_QUESTS QUEST_KILLGOLGOTH + 1

#define DND_QUEST_CAUTIOUS_REGENBONUS 100
#define DND_QUEST_PRECIOUSLIFE_BONUS 100
#define DND_QUEST_BERSERK_SPREEREQ 10
#define DND_QUEST_BERSERK_SPREEBONUS 15
#define DND_QUEST_MASTERHEALER_INCREASE 25
#define DND_QUEST_MASTERHEALER_REQ 500
#define DND_QUEST_BOSSKILLER_REQ 20
#define DND_QUEST_ELITEKILLER_REQ 50
#define DND_QUEST_SKINOTEETH_BONUS 100
#define DND_QUEST_SKINOTEETH_INCREASE 25
#define DND_QUEST_MONEYMAKERREQ 25000
#define DND_QUEST_CREDITBONUS 15
#define DND_QUEST_ARMORBONUS 150

#define DND_LEGQUEST_START QUEST_KILLDREAMINGGOD
#define DND_TORRASQUE_BOOST 15

#define DND_GUNSLINGER_GAIN 50
#define DND_BOOMSTICK_GAIN 50
#define DND_SUPERWEAPON_GAIN 25
#define DND_ELITEDROP_GAIN 0.25

typedef struct quest {
	int qflag;
	str qreward;
} quest_T;

enum {
	QTYPE_INDIVIDUAL = 1, // individual satisfaction of requirements, rewarded to those who accomplish first (only 1)
	QTYPE_EVERYONE = 2, // everyone must satisfy requirements, rewarded to all (everyone or no one)
	QTYPE_ATLEASTONE = 4, // at least one person must satisfy requirement, rewarded to all those who accomplish
	QTYPE_TRACKABLE = 8, // progress tracking
	QTYPE_IMMEDIATE = 16, // if conditions are met, doesn't wait till map ends to give
	QTYPE_NOSAVE = 32 // for some reason this quests reward should not be saved
};

#endif