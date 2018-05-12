#ifndef DND_RESEARCH_IN
#define DND_RESEARCH_IN

enum {
	RES_RAREARMOR,
	RES_SUPERARMOR,
	RES_MEDKITSTORE,
	RES_ACCESSORY,
	
	RES_FLECHETTE,
	RES_PIERCING,
	RES_ELECTRIC,
    RES_NITRO,
	
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
	
	RES_SLOT3SSGUPG1,
	RES_SLOT3SSGUPG2,
	
	RES_SLOT4UPG1,
    RES_SLOT4UPG2,
	RES_SLOT4UPG3,
	
	RES_SLOT5UPG1,
	RES_SLOT5GL,
	RES_SLOT5UPG2,
	RES_SLOT5UPG3,
	
	RES_SLOT6UPG1,
    RES_SLOT6UPG2,
	
	RES_SLOT7UPG1,
	RES_SLOT7UPG2,
	
	RES_OCCULTABILITY,
	RES_SLOT8REVEAL,
	RES_NANOTECH,
	RES_OCCULTARTIFACT,
};


#define DND_RESEARCH_BEGIN RES_RAREARMOR
#define DND_RESEARCH_END RES_OCCULTARTIFACT
#define MAX_RESEARCHES RES_OCCULTARTIFACT + 1
str ResearchPrefix = "\ccResearch Item : \c[Y5]Discovered ";
str Research_List[MAX_RESEARCHES] = {
	"RareArmors",
	"SuperArmor",
	"MedkitStore",
	"AccessoryTap",

	"FlechetteShell",
	"PiercingShell",
	"ElectricShell",
    "NitroShell",

	"SonicGrenade",
	"HEGrenade",
	
	"DoubleSpecialCap",

	"Slot1Occult",
	"Slot2Luxury",
	"Slot3Luxury",
	"Slot4Luxury",
	"Slot5Luxury",
	"Slot6Luxury",
	"Slot7Luxury",

	"Slot1Upgrade1",
	"Slot1Upgrade2",
	
	"Slot2Upgrade1",
	"Slot2Upgrade2",
	
	"Slot3Upgrade1",
    "Slot3Upgrade2",
	
	"Slot3SSGUpgrade1",
	"Slot3SSGUpgrade2",
	
	"Slot4Upgrade1",
    "Slot4Upgrade2",
	"Slot4Upgrade3",
	
	"Slot5Upgrade1",
	"Slot5GL",
	"Slot5Upgrade2",
	"Slot5Upgrade3",
	
	"Slot6Upgrade1",
    "Slot6Upgrade2",
	
	"Slot7Upgrade1",
	"Slot7Upgrade2",

	"OccultAbility",
	"Slot8Reveal",
	"NanoTech",
	"OccultArtifact"
};

str Research_Label[MAX_RESEARCHES] = {
    "Rare Armors",
    "Super Armor",
    "Medikit Storing",
    "Accessory Tap",
   
    "Flechette Shells",
    "Magnum Shells",
    "Electric Shells",
    "Nitrogen Shells",
    
    "High Explosive Grenades",
    "Sonic Grenades",
    
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
	
    "Plasma Cannon (3)",
	"Shocker (3)",
	
    "Templar Machine Gun (4)",
    "Riot Cannon (4)",
	"Acid Rifle (4)",
	
    "Meteor Launcher (5)",
    "Grenade Launcher (5)",
	"Heavy Grenade Launcher (5)",
	"Freezer Cannon (5)",
	
    "Flamethrower (6)",
    "Lightning Gun (6)",
	
    "Ion Cannon (7)",
	"Thunder Staff (7)",
    
    "Occult Abilities",
    "Slot 8 Weapons",
    "Nano Technology",
    "Occult Artifacts"
};

enum {
	RES_NA = 0,
	RES_KNOWN,
	RES_DONE
};

// 0 for NA, 1 for found, 2 for researched
int CheckResearchStatus(int res_id) {
	if(CheckInventory(StrParam(s:"Done_", s:Research_List[res_id])))
		return RES_DONE;
	if(CheckInventory(StrParam(s:"Research_", s:Research_List[res_id])))
		return RES_KNOWN;
	return RES_NA;
}

int CheckActorResearchStatus(int tid, int res_id) {
	if(CheckActorInventory(tid, StrParam(s:"Done_", s:Research_List[res_id])))
		return RES_DONE;
	if(CheckActorInventory(tid, StrParam(s:"Research_", s:Research_List[res_id])))
		return RES_KNOWN;
	return RES_NA;
}

void GiveResearch(int res_id) {
	GiveInventory(StrParam(s:"Research_", s:Research_List[res_id]), 1);
}

void CompleteResearch(int res_id) {
	GiveInventory(StrParam(s:"Done_", s:Research_List[res_id]), 1);
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
		GiveResearch(i);
		CompleteResearch(i);
	}
	GiveInventory("MedkitItem", 1);
	DoubleSpecialAmmoCapacity();
}

#endif