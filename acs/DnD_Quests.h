#ifndef DND_QUESTS_IN
#define DND_QUESTS_IN

#include "DnD_Common.h"
#include "DnD_QuestDefs.h"
#include "DnD_MonsterData.h"

#define DND_QUEST_ARTIBONUS 25 // 25%

quest_T Quest_List[MAX_QUESTS] = {
		{ QTYPE_ATLEASTONE, 											"DnD_QuestReward_RegenCap" 				},
		{ QTYPE_ATLEASTONE, 											"DnD_QuestReward_15PercentDamage"		},
		{ QTYPE_EVERYONE, 												"DnD_QuestReward_100BonusCap"			},
		{ QTYPE_ATLEASTONE | QTYPE_TRACKABLE | QTYPE_IMMEDIATE,			"DnD_QuestReward_KillingSpree15"		},
		{ QTYPE_ATLEASTONE | QTYPE_TRACKABLE | QTYPE_IMMEDIATE,			"DnD_QuestReward_HealingIncrease25"		},
		{ QTYPE_ATLEASTONE | QTYPE_TRACKABLE | QTYPE_IMMEDIATE,			"DnD_QuestReward_ReducedBossDmg"		},
		{ QTYPE_ATLEASTONE | QTYPE_TRACKABLE | QTYPE_IMMEDIATE,			"DnD_QuestReward_EliteDropBonus"		},
		{ QTYPE_ATLEASTONE, 											"DnD_QuestReward_ExtraArtiDmg"			},
		{ QTYPE_ATLEASTONE,												"DnD_QuestReward_EnergyDamage"			},
		{ QTYPE_ATLEASTONE,												"DnD_QuestReward_HealingAndCapIncrease" },
		{ QTYPE_ATLEASTONE,												"DnD_QuestReward_Slot2DmgIncrease"		},
		{ QTYPE_ATLEASTONE,												"DnD_QuestReward_ShotgunDmgIncrease"	},
		{ QTYPE_ATLEASTONE,												"DnD_QuestReward_ArmorCapIncrease"		},
		{ QTYPE_EVERYONE,												"DnD_QuestReward_UndeadDamageIncrease"  },
		{ QTYPE_ATLEASTONE,												"DnD_QuestReward_SuperWeaponDamage"		},
		{ QTYPE_ATLEASTONE,												"DnD_QuestReward_SpecialAmmoDamage"		},
		{ QTYPE_ATLEASTONE | QTYPE_TRACKABLE | QTYPE_IMMEDIATE,			"DnD_QuestReward_MoreCredit"			},
		{ QTYPE_ATLEASTONE | QTYPE_IMMEDIATE,							"DnD_QuestReward_DreamingGodBonus"		},
		{ QTYPE_ATLEASTONE | QTYPE_IMMEDIATE,							"DnD_QuestReward_TorrasqueBonus"		},
		{ QTYPE_ATLEASTONE | QTYPE_IMMEDIATE,							"DnD_QuestReward_MordecqaiBonus"		},
		{ QTYPE_ATLEASTONE | QTYPE_IMMEDIATE,							"DnD_QuestReward_GodSlayerBonus"		},
		{ QTYPE_ATLEASTONE | QTYPE_IMMEDIATE,							"DnD_QuestReward_GolgothBonus"			}
};

enum {
	QUEST_NAME,
	QUEST_REQ,
	QUEST_REW
};

str GetQuestLabel(int id, int which) {
	switch(which) {
		case QUEST_NAME:
		return StrParam(s:"DND_QUESTLABEL", d:id + 1);
		case QUEST_REQ:
		return StrParam(s:"DND_QUESTREQ", d:id + 1);
		case QUEST_REW:
		return StrParam(s:"DND_QUESTREWARD", d:id + 1);
	}
	return "";
}

str Quest_Checkers[MAX_QUESTS] = {
	"DnD_HealthLess50",
	"DnD_WeaponFiredOther",
	"DnD_Boolean",
	"DnD_BerserkQuest_SpreeCounter",
	"DnD_MasterHealerQuest_HealAmount",
	"DnD_BossKillQuest_Counter",
	"DnD_EliteKillQuest_Counter",
	"DnD_UseNoArtifact",
	"DnD_UsingEnergyFailed",
	"DnD_UseNoHealth",
	"DnD_UseOnlyPistol",
	"DnD_UseNoShotgun",
	"DnD_BoughtArmor",
	"DnD_ShotUndead",
	"DnD_UsedSuperWeapon",
	"DnD_UsedNonSpecial",
	"DnD_MoneySpentQuest",
	"DnD_Boolean",
	"DnD_Boolean",
	"DnD_Boolean",
	"DnD_Boolean",
	"DnD_Boolean"
};

void CompleteQuest(int tid, int qid) {
	str tocheck = "DnD_QuestState1";
	if(qid > 30)
		tocheck = "DnD_QuestState2";
	qid %= 31; // for 2nd part
	SetActorInventory(tid, tocheck, CheckActorInventory(tid, tocheck) | (1 << qid));
	GiveActorInventory(tid, Quest_List[qid].qreward, 1);
	SetActorInventory(tid, "QuestCompletionToken", active_quest_id + 1);
	ACS_NamedExecuteAlways("DND Quest Complete", 0);
	SetActorInventory(tid, "ActiveQuestID", 0);
	FinishQuest(tid, qid);
}

bool IsQuestComplete(int tid, int qid) {
	str tocheck = "DnD_QuestState1";
	if(qid > 30)
		tocheck = "DnD_QuestState2";
	qid %= 31; // for 2nd part
	if(!tid)
		return IsSet(CheckInventory(tocheck), qid);
	return IsSet(CheckActorInventory(tid, tocheck), qid);
}

void FailQuest(int tid) {
	// fail marker
	if(CheckActorInventory(tid, "ActiveQuestID")) {
		ACS_NamedExecuteAlways("DND Quest Complete", 0, 1);
		SetActorInventory(tid, "ActiveQuestID", 0);
	}
}

void FinishQuest(int tid, int qid) {
	SetActorInventory(tid, Quest_Checkers[qid], 0);
	// individual quests are who gets it done first, so if someone gets it remove this quest
	if(!(Quest_List[qid].qflag & QTYPE_ATLEASTONE)) {
		active_quest_id = -1;
		// clear from all players
		for(int i = 0; i < MAXPLAYERS; ++i)
			SetActorInventory(i + P_TIDSTART, "ActiveQuestID", 0);
	}
}

bool QuestExitCheckException(int qid) {
	return qid == QUEST_10SPREES || qid == QUEST_SPEND25K || qid == QUEST_KILL10BOSSES || qid == QUEST_KILL20ELITES;
}

int GetQuestProgressRequirement(int qid) {
	int res = 1;
	switch(qid) {
		case QUEST_10SPREES:
			res = DND_QUEST_BERSERK_SPREEREQ;
		break;
		case QUEST_KILL10BOSSES:
			res = DND_QUEST_BOSSKILLER_REQ;
		break;
		case QUEST_KILL20ELITES:
			res = DND_QUEST_ELITEKILLER_REQ;
		break;
		case QUEST_SPEND25K:
			res = DND_QUEST_MONEYMAKERREQ;
		break;
		case QUEST_HEALFOR500:
			res = DND_QUEST_MASTERHEALER_REQ;
		break;
	}
	return res;
}

bool IsValidQuest(int qid) {
	bool res = 1;
	if(qid >= QUEST_KILLDREAMINGGOD)
		res = ThingCountName(LegendaryMonsters[qid - QUEST_KILLDREAMINGGOD], 0) == 1;
	return res;
}

#endif