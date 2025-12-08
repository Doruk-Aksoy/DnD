#ifndef DND_RESEARCH_IN
#define DND_RESEARCH_IN

#include "DnD_SpecialAmmo.h"
#include "DnD_CommonResearch.h"

str GetResearchLabel(int res_id) {
	return StrParam(s:"RES_LABEL", d:res_id);
}

str GetResearchDescription(int res_id) {
	return StrParam(s:"RES_DESC", d:res_id);
}

enum {
	RESF_NODROP = 1,
	RESF_HASREQUIREMENT = 2, //This is only here as a visual guide atm
	RESF_UNLOCKSOTHER = 4
};

enum {
	RES_REQID_BIO1,
	RES_REQID_BIO2,
	RES_REQID_EXO1,
	RES_REQID_EXO2,
	//RES_REQID_IMP1,
	//RES_REQID_IMP2
};
#define LAST_RES_REQID RES_REQID_EXO2

typedef struct {
	int res_flags;
	int res_req_id;
} res_req_info_T;

res_req_info_T ResearchFlags[MAX_RESEARCHES] = { 
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
    { 0, -1 },
   
    { 0, -1 },
    { 0, -1 },
    { 0, -1 },
    { 0, -1 },
	{ 0, -1 },
    
    { 0, -1 },
    { 0, -1 },
    
	{ 0, -1 },
    
    { 0, -1 },
    { 0, -1 },
    { 0, -1 },
    { 0, -1 },
    { 0, -1 },
    { 0, -1 },
    { 0, -1 },
    
	// slot 1
	{ 0, -1 },
	{ 0, -1 },
	
	// slot 2
    { 0, -1 },
	{ 0, -1 },
	
	// slot 3
    { 0, -1 },
    { 0, -1 },
	{ RESF_NODROP | RESF_HASREQUIREMENT, -1},
	{ 0, -1 },
	
	// slot 3 ssg
    { 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	
	// slot 4
    { 0, -1 },
    { 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	
	// slot 5
    { 0, -1 },
    { 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	
	// slot 6
    { 0, -1 },
    { 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	
	// slot 7
    { 0, -1 },
	{ 0, -1 },
    
    { 0, -1 },
    { 0, -1 },
    { 0, -1 },
    { 0, -1 },

	// destr gen
	{ 0, -1 },

	{ RESF_NODROP | RESF_HASREQUIREMENT | RESF_UNLOCKSOTHER, RES_REQID_BIO1 },
	{ RESF_NODROP | RESF_UNLOCKSOTHER, RES_REQID_BIO2 },
	{ RESF_NODROP, -1 },
	{ RESF_NODROP | RESF_HASREQUIREMENT | RESF_UNLOCKSOTHER, RES_REQID_EXO1 },
	{ RESF_NODROP | RESF_UNLOCKSOTHER, RES_REQID_EXO2 },
	{ RESF_NODROP, -1 },
	/*{ RESF_NODROP | RESF_HASREQUIREMENT | RESF_UNLOCKSOTHER, RES_REQID_IMP1 },
	{ RESF_NODROP | RESF_UNLOCKSOTHER, RES_REQID_IMP2 },
	{ RESF_NODROP, -1},*/
	
	{ 0, -1 },
};

#define MAX_RES_REQID (LAST_RES_REQID + 1)
#define MAX_RES_DEPENDENCIES 1 //all researches have at most 1 depencency. -- can have more in the future
typedef struct {
	int unlocks;
	int dependencies[MAX_RES_DEPENDENCIES];
} res_dependency_T;

res_dependency_T ResearchDependencies[MAX_RES_REQID] = {
	{RES_BIO2,			{RES_BIO1}},
	{RES_BIO3,			{RES_BIO2}},
	{RES_EXO2,			{RES_EXO1}},
	{RES_EXO3,			{RES_EXO2}},
	//{RES_IMP2,			{RES_IMP1}},
	//{RES_IMP3,			{RES_IMP2}},
};

enum {
	RES_TRACKER_WHEEL,
	RES_TRACKER_BIO1,
	RES_TRACKER_EXO1,
	//RES_TRACKER_IMP1
};

#define MAX_RESEARCH_TRACKERS (RES_TRACKER_EXO1 + 1)
str ResearchTrackers[MAX_RESEARCH_TRACKERS] = {
	"Research_Slot3Upgrade3_Tracker",
	"Research_Body_Hp_1_Tracker",
	"Research_Body_Ar_1_Tracker"
	//"Research_Body_Im_1_Tracker"
};

void SpawnResearch(int pnum, bool noRepeat = false) {
	// spawn copies of this research
	int temp = -1;
	int i;
	
	// first determine if we have some weight attached to any research
	// idea: generate a random roll from 1 to 100. Assume the following is true for 4 researches: 10% - 10% - 40% - 30%.
	// Case 1: Roll 15. Check 10% => Roll is 25. Check 20% (2nd res) => Roll is 35. Check 3rd, 35 < 40, we pick this one.
	// Case 2: Roll 45. Check 10% => Roll is 55. Check 20%, roll is 65. Check 60, Roll is 105. Check 90. Failed all, because we aren't in any of their percentage ranges.
	// This works with the percentages acting as their individual weights. It's not 100% fair, because if we roll 9, it'll pick leftmost id research.
	// Most fair method would be putting equal chance researches into their bins. However putting multiple investments into different researches is a bad strategy.
	// Therefore it's not important to worry about that kind of solution here.
	int weight_acc = random(1, 1000);
	int invest_acc = 0;
	int drop_rate = 0;
	for(i = 0; i < MAX_RESEARCHES; ++i) {
		if(ResearchInvestments[pnum][i]) {
			drop_rate = GetResearchDropRate(pnum, i) * 10;
			invest_acc += drop_rate;
			if(weight_acc <= invest_acc) {
				temp = i;
				// important! we must reset this now so as to not make this affect results again
				ResearchInvestments[pnum][i] = 0;
				ACS_NamedExecuteWithResult("DnD Menu Investment Sync", pnum, i, 0);
				break;
			}
			else
				weight_acc += drop_rate;
		}
	}
	
	// roll another if this has nodrop flag on it
	// only attempt if we had no weights attached (temp would be -1)
	if(temp == -1) {
		do {
			temp = random(0, MAX_RESEARCHES - 1);
		} while(ResearchFlags[temp].res_flags & RESF_NODROP);
	}
	
	if(GameType() != GAME_SINGLE_PLAYER) {
		for(i = 0; i < MAXPLAYERS; ++i) {
			drop_rate = i + P_TIDSTART;
			// spawn this only if this isn't already found by the player
			if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART) && !CheckActorResearchStatus(drop_rate, temp))
				SpawnDrop("ResearchModule_MP", 24.0, 16, i + 1, temp);
		}
	}
	else if(!CheckResearchStatus(temp)) // 1 before temp is player (0 + 1)
		SpawnDrop("ResearchModule_MP", 24.0, 16, 1, temp);
	
	// input "noRepeat" as true so we don't call this again
	//if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, X) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
	//	SpawnResearch(pnum, true);
}

// This place handles researches that depend on others being researched
void HandleDependentResearch(int res_id) {
	// bio research 2-3 needs previous ones
	if(ResearchFlags[res_id].res_flags & RESF_UNLOCKSOTHER) {
		bool req = true;
		int reqid = ResearchFlags[res_id].res_req_id;
		for(int i = 0; i < MAX_RES_DEPENDENCIES && ResearchDependencies[reqid].unlocks != -1; ++i)
			// Check if all the dependencies are done
			if(CheckResearchStatus(ResearchDependencies[reqid].dependencies[i]) != -1 && //-1 = no dependency - will be needed if max deps is > 1 and 1 research doesn't need that many deps
			   CheckResearchStatus(ResearchDependencies[reqid].dependencies[i]) != RES_DONE) {
				req = false;
				break;
			}
		// if all conditions are satisfied
		if(req)
			GiveResearch(ResearchDependencies[reqid].unlocks, true);
	}
}

void CompleteResearch(int res_id) {
	// Remember: Actor inventory max value is 32 bits (signed, and min is 0).
	if(res_id > DND_RESEARCH_MAXBITS2)
		SetInventory("Research_Done_3", SetBit(CheckInventory("Research_Done_3"), res_id % 31));
	else if(res_id > DND_RESEARCH_MAXBITS1)
		SetInventory("Research_Done_2", SetBit(CheckInventory("Research_Done_2"), res_id % 31));
	else
		SetInventory("Research_Done_1", SetBit(CheckInventory("Research_Done_1"), res_id));
	
	HandleDependentResearch(res_id);
	
	if(res_id == RES_DOUBLESPECIALCAP)
		DoubleSpecialAmmoCapacity();
	if(res_id == RES_MEDKITSTORE)
		GiveInventory("MedkitItem", 1);
}

bool HasIncompleteResearches() {
	for(int i = 0; i < MAX_RESEARCHES; ++i)
		if(CheckResearchStatus(i) != RES_DONE)
			return 1;
	return 0;
}

void GiveAndDoAllResearch() {
	for(int i = 0; i < MAX_RESEARCHES; ++i) {
		GiveResearch(i, false);
		CompleteResearch(i);
	}
	GiveInventory("MedkitItem", 1);
	DoubleSpecialAmmoCapacity();
}

void DoubleSpecialAmmoCapacity() {
	for(int i = 0; i < MAX_SPECIAL_AMMOS; ++i)
		SetAmmoCapacity(SpecialAmmoInfo_Str[i][AMMOINFO_NAME], SpecialAmmoInfo[i].initial_capacity * 2);
}

#endif
