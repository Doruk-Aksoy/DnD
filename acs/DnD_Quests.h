#ifndef DND_QUESTS_IN
#define DND_QUESTS_IN

#include "DnD_Common.h"
#include "DnD_QuestDefs.h"
#include "DnD_MonsterData.h"

#define DND_QUEST_ARTIBONUS 25 // 25%

quest_T Quest_List[MAX_QUESTS] = {
		{ QTYPE_ATLEASTONE, 											"DnD_HealthLess50" 						},
		{ QTYPE_ATLEASTONE, 											"DnD_WeaponFiredOther"					},
		{ QTYPE_EVERYONE, 												"DnD_Boolean"							},
		{ QTYPE_ATLEASTONE | QTYPE_TRACKABLE | QTYPE_IMMEDIATE,			"DnD_BerserkQuest_SpreeCounter"			},
		{ QTYPE_ATLEASTONE | QTYPE_TRACKABLE | QTYPE_IMMEDIATE,			"DnD_MasterHealerQuest_HealAmount"		},
		{ QTYPE_ATLEASTONE | QTYPE_TRACKABLE | QTYPE_IMMEDIATE,			"DnD_BossKillQuest_Counter"				},
		{ QTYPE_ATLEASTONE | QTYPE_TRACKABLE | QTYPE_IMMEDIATE,			"DnD_EliteKillQuest_Counter"			},
		{ QTYPE_ATLEASTONE, 											"DnD_UseNoArtifact"						},
		{ QTYPE_ATLEASTONE,												"DnD_UsingEnergyFailed"					},
		{ QTYPE_ATLEASTONE,												"DnD_UseNoHealth" 						},
		{ QTYPE_ATLEASTONE,												"DnD_UseOnlyPistol"						},
		{ QTYPE_ATLEASTONE,												"DnD_UseNoShotgun"						},
		{ QTYPE_ATLEASTONE,												"DnD_BoughtArmor"						},
		{ QTYPE_EVERYONE,												"DnD_ShotUndead"  						},
		{ QTYPE_ATLEASTONE,												"DnD_UsedSuperWeapon"					},
		{ QTYPE_ATLEASTONE,												"DnD_UsedNonSpecial"					},
		{ QTYPE_ATLEASTONE | QTYPE_TRACKABLE | QTYPE_IMMEDIATE,			"DnD_MoneySpentQuest"					},
		{ QTYPE_ATLEASTONE | QTYPE_IMMEDIATE,							"DnD_Boolean"							},
		{ QTYPE_ATLEASTONE | QTYPE_IMMEDIATE,							"DnD_Boolean"							},
		{ QTYPE_ATLEASTONE | QTYPE_IMMEDIATE,							"DnD_Boolean"							},
		{ QTYPE_ATLEASTONE | QTYPE_IMMEDIATE,							"DnD_Boolean"							},
		{ QTYPE_ATLEASTONE | QTYPE_IMMEDIATE,							"DnD_Boolean"							}
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

void CompleteQuest(int tid, int qid) {
	str tocheck = "DnD_QuestState1";
	if(qid > 30)
		tocheck = "DnD_QuestState2";
	qid %= 31; // for 2nd part
	SetActorInventory(tid, tocheck, CheckActorInventory(tid, tocheck) | (1 << qid));
	SetActorInventory(tid, "QuestCompletionToken", active_quest_id + 1);
	ACS_NamedExecuteAlways("DND Quest Complete", 0);
	SetActorInventory(tid, "ActiveQuestID", 0);
	FinishQuest(tid, qid);
}

void FailQuest(int tid) {
	// fail marker
	if(CheckActorInventory(tid, "ActiveQuestID")) {
		ACS_NamedExecuteAlways("DND Quest Complete", 0, 1);
		SetActorInventory(tid, "ActiveQuestID", 0);
	}
}

void FinishQuest(int tid, int qid) {
	SetActorInventory(tid, Quest_List[qid].qchecker, 0);
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

Script "DnD Quest Check" (int q_id) {
	SetResultValue(IsQuestComplete(0, q_id));
}

#endif
