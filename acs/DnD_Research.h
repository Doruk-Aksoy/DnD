#ifndef DND_RESEARCH_IN
#define DND_RESEARCH_IN

#include "DnD_SpecialAmmo.h"

enum {
	RES_RAREARMOR,
	RES_SUPERARMOR,
	RES_MEDKITSTORE,
	RES_ACCESSORY,
	
	RES_FLECHETTE,
	RES_PIERCING,
	RES_ELECTRIC,
    RES_NITRO,
	RES_SLUGSHELL,
	
	RES_SONICGRENADE,
	RES_HEGRENADE,
	
	RES_DOUBLESPECIALCAP,
	
	RES_SLOT1OCCULT,
	RES_SLOT2LUXURY,
	RES_SLOT3LUXURY,
	RES_SLOT4LUXURY,
	RES_SLOT5LUXURY,
	RES_SLOT6LUXURY,
	RES_SLOT7LUXURY,
	
	RES_SLOT1UPG1,
	RES_SLOT1UPG2,
	
	RES_SLOT2UPG1,
	RES_SLOT2UPG2,
	
	RES_SLOT3UPG1,
    RES_SLOT3UPG2,
	RES_SLOT3UPG3,
	
	RES_SLOT3SSGUPG1,
	RES_SLOT3SSGUPG2,
	RES_SLOT3SSGUPG3,
	
	RES_SLOT4UPG1,
    RES_SLOT4UPG2,
	RES_SLOT4UPG3,
	RES_SLOT4UPG4,
	
	RES_SLOT5UPG1,
	RES_SLOT5GL,
	RES_SLOT5UPG2,
	RES_SLOT5UPG3,
	
	RES_SLOT6UPG1,
    RES_SLOT6UPG2,
	RES_SLOT6UPG3,
	
	RES_SLOT7UPG1,
	RES_SLOT7UPG2,
	
	RES_OCCULTABILITY,
	RES_SLOT8REVEAL,
	RES_NANOTECH,
	RES_OCCULTARTIFACT,
	
	RES_BIO1,
	RES_BIO2,
	RES_BIO3,
	RES_EXO1,
	RES_EXO2,
	RES_EXO3,
	RES_IMP1,
	RES_IMP2,
	RES_IMP3,
	
	RES_STASHTAB
};

#define DND_RESEARCH_BEGIN RES_RAREARMOR
#define DND_RESEARCH_END RES_STASHTAB
#define MAX_RESEARCHES (DND_RESEARCH_END + 1)
#define RESEARCH_BITSETS ((MAX_RESEARCHES / 31) + 1) //Remember: use 31 bits (zand uses signed numbers only, and min inv amount min is 0)
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
	RESF_HASREQUIREMENT = 2,
	RESF_UNLOCKSOTHER = 4
};

enum {
	RES_REQID_SUPERARMOR,
	RES_REQID_WHEEL,
	RES_REQID_BIO1,
	RES_REQID_BIO2,
	RES_REQID_BIO3,
	RES_REQID_EXO1,
	RES_REQID_EXO2,
	RES_REQID_EXO3,
	RES_REQID_IMP1,
	RES_REQID_IMP2,
	RES_REQID_IMP3,
};
#define LAST_RES_REQID RES_REQID_IMP3

typedef struct {
	int res_flags;
	int res_req_id;
} res_req_info_T;

res_req_info_T ResearchFlags[MAX_RESEARCHES] = {
	{ RESF_UNLOCKSOTHER, RES_REQID_SUPERARMOR },
	{ RESF_NODROP, RES_REQID_SUPERARMOR },
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
	{ RESF_NODROP | RESF_HASREQUIREMENT, RES_REQID_WHEEL },
	
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
	
	{ RESF_NODROP | RESF_HASREQUIREMENT, RES_REQID_BIO1 },
	{ RESF_NODROP | RESF_HASREQUIREMENT, RES_REQID_BIO2 },
	{ RESF_NODROP | RESF_HASREQUIREMENT, RES_REQID_BIO3 },
	{ RESF_NODROP | RESF_HASREQUIREMENT, RES_REQID_EXO1 },
	{ RESF_NODROP | RESF_HASREQUIREMENT, RES_REQID_EXO2 },
	{ RESF_NODROP | RESF_HASREQUIREMENT, RES_REQID_EXO3 },
	{ RESF_NODROP | RESF_HASREQUIREMENT, RES_REQID_IMP1 },
	{ RESF_NODROP | RESF_HASREQUIREMENT, RES_REQID_IMP2 },
	{ RESF_NODROP | RESF_HASREQUIREMENT, RES_REQID_IMP3 },
	
	{ 0, -1 },
};

#define MAX_RES_REQID LAST_RES_REQID + 1
#define MAX_RES_DEPENDENCIES 2
typedef struct {
	int unlocks;
	int dependencies[MAX_RES_DEPENDENCIES];
} res_dependency_T;

res_dependency_T ResearchDependencies[MAX_RES_REQID] = {
	{
			RES_SUPERARMOR,
		{ 	RES_RAREARMOR, 			-1 	}
	},
	{
			-1,
		{ 	-1, 					-1 	}
	},
	{
			-1,
		{ 	-1 							},
	},
	{
			RES_BIO2,
		{ 	RES_REQID_BIO1, 		-1 	},
	},
	{	
			RES_BIO3,
		{	RES_REQID_BIO2, 		-1 	},
	},
	{
			-1,
		{	-1							},
	},
	{
			RES_EXO2,
		{	RES_REQID_EXO1,			-1	},
	},
	{
			RES_EXO3,
		{	RES_REQID_EXO2,			-1	}
	},
	{
			-1,
		{	-1,						-1	}
	},
	{
			RES_IMP2,
		{	RES_REQID_IMP1,			-1	}
	},
	{
			RES_IMP3,
		{	RES_REQID_IMP2,			-1	}
	}
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

enum {
	RES_NA = 0,
	RES_KNOWN,
	RES_DONE
};

// 0 for NA, 1 for found, 2 for researched
int CheckResearchStatus(int res_id) {
	if(res_id > 30) {
		res_id %= 31;
		if(IsSet(CheckInventory("Research_Done_2"), res_id))
			return RES_DONE;
		if(IsSet(CheckInventory("Research_Discovered_2"), res_id))
			return RES_KNOWN;
	}
	else {
		if(IsSet(CheckInventory("Research_Done_1"), res_id))
			return RES_DONE;
		if(IsSet(CheckInventory("Research_Discovered_1"), res_id))
			return RES_KNOWN;
	}
	return RES_NA;
}

int CheckActorResearchStatus(int tid, int res_id) {
	// if we go over 62, we need another check, refrain from using Strparam here as these are potentially called often
	// Remember: Actor inventory max value is 31 bits (signed, and min is 0).
	if(res_id > 30) {
		res_id %= 31;
		if(IsSet(CheckActorInventory(tid, "Research_Done_2"), res_id))
			return RES_DONE;
		if(IsSet(CheckActorInventory(tid, "Research_Discovered_2"), res_id))
			return RES_KNOWN;
	}
	else {
		if(IsSet(CheckActorInventory(tid, "Research_Done_1"), res_id))
			return RES_DONE;
		if(IsSet(CheckActorInventory(tid, "Research_Discovered_1"), res_id))
			return RES_KNOWN;
	}
	return RES_NA;
}

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
		int reqid = ResearchFlags[res_id].res_req_id;
		bool canDo = true;
		for(int i = 0; i < MAX_RES_DEPENDENCIES && ResearchDependencies[reqid].unlocks != -1; ++i)
			if(!CheckResearchStatus(ResearchDependencies[reqid].dependencies[i]))
				canDo = false;
		// check if all requirements are met
		if(canDo)
			GiveResearch(ResearchDependencies[reqid].unlocks, true);
	}
}

void CompleteResearch(int res_id) {
	HandleDependentResearch(res_id);
	
	// Remember: Actor inventory max value is 31 bits (signed, and min is 0).
	if(res_id > 30)
		SetInventory("Research_Done_2", SetBit(CheckInventory("Research_Done_2"), res_id % 31));
	else
		SetInventory("Research_Done_1", SetBit(CheckInventory("Research_Done_1"), res_id));
	
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

void HandleResearchBonuses() {
	if(CheckResearchStatus(RES_IMP3))
		GiveInventory("ImpactProtection_3", 1);
	else if(CheckResearchStatus(RES_IMP2))
		GiveInventory("ImpactProtection_2", 1);
	else if(CheckResearchStatus(RES_IMP1))
		GiveInventory("ImpactProtection_1", 1);
}

#endif