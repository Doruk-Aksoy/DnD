#ifndef DND_RESEARCH_IN
#define DND_RESEARCH_IN

#include "DnD_SpecialAmmo.h"
#include "DnD_CommonResearch.h"

str ResearchPrefix = "\ccResearch Item : \c[Y5]Discovered ";

str Research_Label[MAX_RESEARCHES] = {
    "Rare Armors",
    "Super Armor",
    "Medikit Storing",
    "Accessory Tap",
   
    "Flechette Shells",
    "Magnum Shells",
    "Electric Shells",
    "Nitrogen Shells",
	"Slug Shells",
    
    "Sonic Grenades",
    "High Explosive Grenades",
    
    "Special Ammo Holding",
    
    "Slot 1 Occult Weapons",
    "Slot 2 Luxury Weapons",
    "Slot 3 Luxury Weapons",
    "Slot 4 Luxury Weapons",
    "Slot 5 Luxury Weapons",
    "Slot 6 Luxury Weapons",
    "Slot 7 Luxury Weapons",
    
	"Dusk Blade (1)",
	"Inferno Sword (1)",
	
    "Assault Rifle (2)",
	"Viper Staff (2)",
	
    "Deadlock Shotgun (3)",
    "Nitrogen Crossbow (3)",
	"Wheel of Torment (3)",
	
    "Plasma Cannon (3)",
	"Shocker (3)",
	"Hades Auto Shotgun (3)",
	
    "Templar Machine Gun (4)",
    "Riot Cannon (4)",
	"Acid Rifle (4)",
	"Fusion Blaster (4)",
	
    "Meteor Launcher (5)",
    "Grenade Launcher (5)",
	"Heavy Grenade Launcher (5)",
	"Freezer Cannon (5)",
	
    "Flamethrower (6)",
    "Lightning Gun (6)",
	"Rebounder (6)",
	
    "Ion Cannon (7)",
	"Thunder Staff (7)",
    
    "Occult Abilities",
    "Slot 8 Weapons",
    "Nano Technology",
    "Occult Artifacts",
	
	"Bio-Enhancement - I",
	"Bio-Enhancement - II",
	"Bio-Enhancement - III",
	"Exoskeleton - I",
	"Exoskeleton - II",
	"Exoskeleton - III",
	"Impact Protection - I",
	"Impact Protection - II",
	"Impact Protection - III",
	
	"Increased Stash Tabs"
};

enum {
	RESF_NODROP = 1,
	RESF_HASREQUIREMENT = 2, //This is only here as a visual guide atm
	RESF_UNLOCKSOTHER = 4
};

enum {
	RES_REQID_SUPERARMOR,
	RES_REQID_BIO1,
	RES_REQID_BIO2,
	RES_REQID_EXO1,
	RES_REQID_EXO2,
	RES_REQID_IMP1,
	RES_REQID_IMP2
};
#define LAST_RES_REQID RES_REQID_IMP2

typedef struct {
	int res_flags;
	int res_req_id;
} res_req_info_T;

res_req_info_T ResearchFlags[MAX_RESEARCHES] = {
	{ RESF_UNLOCKSOTHER, RES_REQID_SUPERARMOR },
	{ RESF_NODROP, -1 },
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
	
    { 0, -1 },
	{ 0, -1 },
	
    { 0, -1 },
    { 0, -1 },
	{ RESF_NODROP | RESF_HASREQUIREMENT, -1},
	
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
    { 0, -1 },
	
	{ RESF_NODROP | RESF_HASREQUIREMENT | RESF_UNLOCKSOTHER, RES_REQID_BIO1 },
	{ RESF_NODROP | RESF_UNLOCKSOTHER, RES_REQID_BIO2 },
	{ RESF_NODROP, -1 },
	{ RESF_NODROP | RESF_HASREQUIREMENT | RESF_UNLOCKSOTHER, RES_REQID_EXO1 },
	{ RESF_NODROP | RESF_UNLOCKSOTHER, RES_REQID_EXO2 },
	{ RESF_NODROP, -1 },
	{ RESF_NODROP | RESF_HASREQUIREMENT | RESF_UNLOCKSOTHER, RES_REQID_IMP1 },
	{ RESF_NODROP | RESF_UNLOCKSOTHER, RES_REQID_IMP2 },
	{ RESF_NODROP, -1},
	
	{ 0, -1 },
};

#define MAX_RES_REQID (LAST_RES_REQID + 1)
#define MAX_RES_DEPENDENCIES 1 //all researches have at most 1 depencency. -- can have more in the future
typedef struct {
	int unlocks;
	int dependencies[MAX_RES_DEPENDENCIES];
} res_dependency_T;

res_dependency_T ResearchDependencies[MAX_RES_REQID] = {
	{RES_SUPERARMOR, 	{RES_RAREARMOR}},
	{RES_BIO2,			{RES_BIO1}},
	{RES_BIO3,			{RES_BIO2}},
	{RES_EXO2,			{RES_EXO1}},
	{RES_EXO3,			{RES_EXO2}},
	{RES_IMP2,			{RES_IMP1}},
	{RES_IMP3,			{RES_IMP2}},
};

enum {
	RES_TRACKER_WHEEL,
	RES_TRACKER_BIO1,
	RES_TRACKER_EXO1,
	RES_TRACKER_IMP1
};

#define MAX_RESEARCH_TRACKERS RES_TRACKER_IMP1 + 1
str ResearchTrackers[MAX_RESEARCH_TRACKERS] = {
	"Research_Slot3Upgrade3_Tracker",
	"Research_Body_Hp_1_Tracker",
	"Research_Body_Ar_1_Tracker",
	"Research_Body_Im_1_Tracker"
};

void GiveResearch(int res_id, bool fancy) {
	if(fancy)
		ACS_NamedExecuteAlways("DnD Research Fancy Message", 0, res_id);
	if(res_id > 30) {
		res_id %= 31;
		SetInventory("Research_Discovered_2", SetBit(CheckInventory("Research_Discovered_2"), res_id));
	}
	else
		SetInventory("Research_Discovered_1", SetBit(CheckInventory("Research_Discovered_1"), res_id));
}

// This place handles researches that depend on others being researched
void HandleDependentResearch(int res_id) {
	// bio research 2-3 needs previous ones
	if(ResearchFlags[res_id].res_flags & RESF_UNLOCKSOTHER) {
		bool req = true;
		int reqid = ResearchFlags[res_id].res_req_id;
		for(int i = 0; i < MAX_RES_DEPENDENCIES && ResearchDependencies[reqid].unlocks != -1; ++i)
			// Check if all the dependencies are done
			if(CheckResearchStatus(ResearchDependencies[reqid].dependencies[i]) != RES_DONE) {
				req = false;
				break;
			}
		// if all conditions are satisfied
		if(req)
			GiveResearch(ResearchDependencies[reqid].unlocks, true);
	}
}

void CompleteResearch(int res_id) {
	// Remember: Actor inventory max value is 31 bits (signed, and min is 0).
	if(res_id > 30)
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