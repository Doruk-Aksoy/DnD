#ifndef DND_ARMOR_IN
#define DND_ARMOR_IN

#define DND_MIT_PER_DEX 0.2
#define DND_MIT_BASE 50.0 // 50%
#define DND_MIT_MAXEFFECT 90.0

#define BASE_ARMOR_FACTOR 8
#define BASE_ARMOR_MAGIC_EFFECT 80 // 80% REDUCED effect, so really the effect is 20%

#define ARMOR_TANGLED_DROPLVL 40

enum {
    // body armors
    BODYARMOR_GREEN, // 20%
    BODYARMOR_YELLOW, // 15%
    BODYARMOR_BLUE, // 10%
    BODYARMOR_RED, // 5%

	BODYARMOR_GUNSLINGER, // 6%
	BODYARMOR_OCCULT, // 6%
	BODYARMOR_DEMO, // 6%
	BODYARMOR_ENERGY, // 6%
	BODYARMOR_ELEMENTAL, // 6%

	BODYARMOR_MONOLITH, // 1%
	BODYARMOR_CYBER, // 4%
	BODYARMOR_DUELIST, // 5%
	BODYARMOR_NECRO, // 3%
	BODYARMOR_KNIGHT, // 3%
	BODYARMOR_RAVAGER, // 4%
	BODYARMOR_TANGLEDRIBCAGE, // 1%

	// special drops, can't randomly appear
	BODYARMOR_SYNTHMETAL,
	BODYARMOR_LIGHTNINGCOIL,

	BOOTS_SILVER = 0,
	BOOTS_ENGINEER,
	BOOTS_INSULATED,
	BOOTS_PLATED,
	BOOTS_POWER,
	BOOTS_ENERGY,
	BOOTS_TACTICAL,
	BOOTS_FUSION,
	BOOTS_LEATHER,
	BOOTS_SNAKESKIN,
	BOOTS_DRAKESKIN,

	HELMS_LICH = 0,
	HELMS_WARRIOR,
	HELMS_SYNTHMETAL,
	HELMS_KNIGHT,
	HELMS_PREDATOR,
	HELMS_CRUSADER,
	HELMS_TACTICAL,
	HELMS_ROBE
};
#define BODYARMORS_BEGIN BODYARMOR_GREEN
#define BODYARMORS_REGULAREND BODYARMOR_TANGLEDRIBCAGE
#define BODYARMORS_END BODYARMOR_LIGHTNINGCOIL

#define BOOTS_BEGIN BOOTS_SILVER
#define BOOTS_END BOOTS_DRAKESKIN

#define HELMS_BEGIN HELMS_LICH
#define HELMS_END HELMS_ROBE

enum {
	ARMWEIGHT_GREEN = 16,
	ARMWEIGHT_YELLOW = 30,
	ARMWEIGHT_BLUE = 40,
	ARMWEIGHT_RED = 46,

	ARMWEIGHT_GUNSLINGER = 52,
	ARMWEIGHT_OCCULT = 58,
	ARMWEIGHT_DEMO = 64,
	ARMWEIGHT_ENERGY = 70,
	ARMWEIGHT_ELEMENTAL = 76,

	ARMWEIGHT_MONOLITH = 80,
	ARMWEIGHT_CYBER = 83,
	ARMWEIGHT_DUELIST = 88,
	ARMWEIGHT_NECRO = 91,
	ARMWEIGHT_KNIGHT = 94,
	ARMWEIGHT_RAVAGER = 98,
	ARMWEIGHT_TANGLEDRIBCAGE = 100
};

enum {
	BOOTWEIGHT_SILVER = 15,
	BOOTWEIGHT_ENGINEER = 30,
	BOOTWEIGHT_INSULATED = 35,
	BOOTWEIGHT_PLATED = 45,
	BOOTWEIGHT_POWER = 50,
	BOOTWEIGHT_ENERGY = 55,
	BOOTWEIGHT_TACTICAL = 58,
	BOOTWEIGHT_FUSION = 65,
	BOOTWEIGHT_LEATHER = 80,
	BOOTWEIGHT_SNAKESKIN = 90,
	BOOTWEIGHT_DRAKESKIN = 100
};

enum {
	HELMWEIGHT_LICH = 12,
	HELMWEIGHT_WARRIOR = 24,
	HELMWEIGHT_SYNTH = 48,
	HELMWEIGHT_KNIGHT = 60,
	HELMWEIGHT_PREDATOR = 68,
	HELMWEIGHT_CRUSADER = 76,
	HELMWEIGHT_TACTICAL = 90,
	HELMWEIGHT_ROBE = 100
};

int HelmDropWeights[HELMS_END + 1] = {
	HELMWEIGHT_LICH,
	HELMWEIGHT_WARRIOR,
	HELMWEIGHT_SYNTH,
	HELMWEIGHT_KNIGHT,
	HELMWEIGHT_PREDATOR,
	HELMWEIGHT_CRUSADER,
	HELMWEIGHT_TACTICAL,
	HELMWEIGHT_ROBE
};

enum {
	DND_DROPPEDARMOR_BODY,
	DND_DROPPEDARMOR_BOOT,
	DND_DROPPEDARMOR_HELM,

	DND_DROPPEDARMOR_LAST
};

#define MAX_SUB_ARMORTYPES 64 // increase if needed
global int 50: ArmorDropWeights[DND_DROPPEDARMOR_LAST][MAX_SUB_ARMORTYPES];

void SetupArmorDropWeights() {
	// Body Armors
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_GREEN] = ARMWEIGHT_GREEN;
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_YELLOW] = ARMWEIGHT_YELLOW;
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_BLUE] = ARMWEIGHT_BLUE;
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_RED] = ARMWEIGHT_RED;
	
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_GUNSLINGER] = ARMWEIGHT_GUNSLINGER;
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_OCCULT] = ARMWEIGHT_OCCULT;
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_DEMO] = ARMWEIGHT_DEMO;
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_ENERGY] = ARMWEIGHT_ENERGY;
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_ELEMENTAL] = ARMWEIGHT_ELEMENTAL;
	
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_MONOLITH] = ARMWEIGHT_MONOLITH;
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_CYBER] = ARMWEIGHT_CYBER;
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_DUELIST] = ARMWEIGHT_DUELIST;
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_NECRO] = ARMWEIGHT_NECRO;
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_KNIGHT] = ARMWEIGHT_KNIGHT;
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_RAVAGER] = ARMWEIGHT_RAVAGER;
	ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMOR_TANGLEDRIBCAGE] = ARMWEIGHT_TANGLEDRIBCAGE;
	
	// Boots
	ArmorDropWeights[DND_DROPPEDARMOR_BOOT][BOOTS_SILVER] = BOOTWEIGHT_SILVER;
	ArmorDropWeights[DND_DROPPEDARMOR_BOOT][BOOTS_ENGINEER] = BOOTWEIGHT_ENGINEER;
	ArmorDropWeights[DND_DROPPEDARMOR_BOOT][BOOTS_INSULATED] = BOOTWEIGHT_INSULATED;
	ArmorDropWeights[DND_DROPPEDARMOR_BOOT][BOOTS_PLATED] = BOOTWEIGHT_PLATED;
	ArmorDropWeights[DND_DROPPEDARMOR_BOOT][BOOTS_POWER] = BOOTWEIGHT_POWER;
	ArmorDropWeights[DND_DROPPEDARMOR_BOOT][BOOTS_ENERGY] = BOOTWEIGHT_ENERGY;
	ArmorDropWeights[DND_DROPPEDARMOR_BOOT][BOOTS_TACTICAL] = BOOTWEIGHT_TACTICAL;
	ArmorDropWeights[DND_DROPPEDARMOR_BOOT][BOOTS_FUSION] = BOOTWEIGHT_FUSION;
	ArmorDropWeights[DND_DROPPEDARMOR_BOOT][BOOTS_LEATHER] = BOOTWEIGHT_LEATHER;
	ArmorDropWeights[DND_DROPPEDARMOR_BOOT][BOOTS_SNAKESKIN] = BOOTWEIGHT_SNAKESKIN;
	ArmorDropWeights[DND_DROPPEDARMOR_BOOT][BOOTS_DRAKESKIN] = BOOTWEIGHT_DRAKESKIN;

	// Helms
	ArmorDropWeights[DND_DROPPEDARMOR_HELM][HELMS_LICH] = HELMWEIGHT_LICH;
	ArmorDropWeights[DND_DROPPEDARMOR_HELM][HELMS_WARRIOR] = HELMWEIGHT_WARRIOR;
	ArmorDropWeights[DND_DROPPEDARMOR_HELM][HELMS_SYNTHMETAL] = HELMWEIGHT_SYNTH;
	ArmorDropWeights[DND_DROPPEDARMOR_HELM][HELMS_KNIGHT] = HELMWEIGHT_KNIGHT;
	ArmorDropWeights[DND_DROPPEDARMOR_HELM][HELMS_PREDATOR] = HELMWEIGHT_PREDATOR;
	ArmorDropWeights[DND_DROPPEDARMOR_HELM][HELMS_CRUSADER] = HELMWEIGHT_CRUSADER;
	ArmorDropWeights[DND_DROPPEDARMOR_HELM][HELMS_TACTICAL] = HELMWEIGHT_TACTICAL;
	ArmorDropWeights[DND_DROPPEDARMOR_HELM][HELMS_ROBE] = HELMWEIGHT_ROBE;
}

#define DND_BODYARMOR_BASEWIDTH 2
#define DND_BODYARMOR_BASEHEIGHT 2

#define DND_BOOT_BASEWIDTH 2
#define DND_BOOT_BASEHEIGHT 1

#define DND_HELM_BASEWIDTH 1
#define DND_HELM_BASEHEIGHT 1

#define LIGHTNINGCOIL_ABSORBFACTOR 80

#define CRUSADER_ESPCT 1

#define MAX_HELM_ATTRIB_DEFAULT 4
#define MAX_BOOT_ATTRIB_DEFAULT 4
#define MAX_ARMOR_ATTRIB_DEFAULT 6

// returns type of armor as result, extra holds m_id
int ConstructArmorDataOnField(int item_pos, int item_tier, int tiers = 0, int extra = -1) {
    // decide what type of armor to spawn here -- droppers have tiers not equal to zero, so they can determine some easy armors to drop
	int res, i;
	if(!tiers) {
		// pick with some weight here -- <= here for index, size is that +1
		res = random(1, 100);
		for(i = 0; i <= BODYARMORS_REGULAREND; ++i) {
			if(res <= ArmorDropWeights[DND_DROPPEDARMOR_BODY][i]) {
				if(i == BODYARMOR_TANGLEDRIBCAGE && MonsterProperties[extra].level < ARMOR_TANGLED_DROPLVL) {
					// repeat if this happened
					res = random(1, 100);
					i = -1;
					continue;
				}
				res = i;
				break;
			}
		}
#ifdef ISDEBUGBUILD
		res = random(BODYARMORS_BEGIN, BODYARMORS_END);
		//res = BODYARMOR_TANGLEDRIBCAGE;
#endif
	}
	else if(tiers == 1) {
		res = random(0, 2);
		// 1/3rd chance to be yellow spawn
		if(res)
			res = BODYARMOR_GREEN;
		else
			res = BODYARMOR_YELLOW;
		// make sure everyone can equip this armor (we need this to ensure map spawned armors are readily equippable)
		item_tier = PlayerInformationInLevel[PLAYERLEVELINFO_MINLEVEL];
		if(!item_tier || item_tier == INT_MAX)
			item_tier = 1;
	}
	else if(tiers == 2) {
		// 1/5 chance to be red
		res = random(0, 4);
		if(res)
			res = BODYARMOR_BLUE;
		else
			res = BODYARMOR_RED;
		item_tier = PlayerInformationInLevel[PLAYERLEVELINFO_MINLEVEL];
		if(!item_tier || item_tier == INT_MAX)
			item_tier = 1 + random(PlayerInformationInLevel[PLAYERLEVELINFO_MINLEVEL] / 2, PlayerInformationInLevel[PLAYERLEVELINFO_MINLEVEL] - 1);
	}
	else if(tiers < 0)
		res = -tiers;

	if(item_tier > GetCVar("dnd_maxmonsterlevel"))
		item_tier = GetCVar("dnd_maxmonsterlevel");

#ifdef ISDEBUGBUILD
	res = BODYARMOR_GUNSLINGER;
#endif

	Inventories_On_Field[item_pos].item_level = item_tier;
	Inventories_On_Field[item_pos].item_stack = 0;
	Inventories_On_Field[item_pos].item_type = DND_ITEM_BODYARMOR;
	Inventories_On_Field[item_pos].item_subtype = res;
	Inventories_On_Field[item_pos].width = DND_BODYARMOR_BASEWIDTH;
	Inventories_On_Field[item_pos].height = DND_BODYARMOR_BASEHEIGHT;

	Inventories_On_Field[item_pos].corrupted = false;
	Inventories_On_Field[item_pos].quality = 0;

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		Inventories_On_Field[item_pos].implicit[i].attrib_id = -1;
		Inventories_On_Field[item_pos].implicit[i].attrib_val = 0;
		Inventories_On_Field[item_pos].implicit[i].attrib_tier = 0;
		Inventories_On_Field[item_pos].implicit[i].attrib_extra = 0;
	}
	
	Inventories_On_Field[item_pos].attrib_count = 0;
	for(i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		Inventories_On_Field[item_pos].attributes[i].attrib_id = -1;

	return res;
}

// returns type of charm as result
int ConstructBootDataOnField(int item_pos, int item_tier) {
    // decide what type of armor to spawn here -- droppers have tiers not equal to zero, so they can determine some easy armors to drop
	int i;
	int res = random(1, 100);
	for(i = 0; i <= BOOTS_END; ++i)
		if(res <= ArmorDropWeights[DND_DROPPEDARMOR_BOOT][i]) {
			res = i;
			break;
		}

	if(item_tier > GetCVar("dnd_maxmonsterlevel"))
		item_tier = GetCVar("dnd_maxmonsterlevel");

	Inventories_On_Field[item_pos].item_level = item_tier;
	Inventories_On_Field[item_pos].item_stack = 0;
	Inventories_On_Field[item_pos].item_type = DND_ITEM_BOOT;
	Inventories_On_Field[item_pos].item_subtype = res;
	Inventories_On_Field[item_pos].width = DND_BOOT_BASEWIDTH;
	Inventories_On_Field[item_pos].height = DND_BOOT_BASEHEIGHT;

	Inventories_On_Field[item_pos].corrupted = false;
	Inventories_On_Field[item_pos].quality = 0;

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		Inventories_On_Field[item_pos].implicit[i].attrib_id = -1;
		Inventories_On_Field[item_pos].implicit[i].attrib_val = 0;
		Inventories_On_Field[item_pos].implicit[i].attrib_tier = 0;
		Inventories_On_Field[item_pos].implicit[i].attrib_extra = 0;
	}
	
	Inventories_On_Field[item_pos].attrib_count = 0;
	for(i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		Inventories_On_Field[item_pos].attributes[i].attrib_id = -1;

	return res;
}

int ConstructHelmDataOnField(int item_pos, int item_tier, int helm = -1) {
    // decide what type of armor to spawn here -- droppers have tiers not equal to zero, so they can determine some easy armors to drop
	int i;
	int res = 0;
	if(helm == -1) {
		res = random(1, 100);
		for(i = 0; i <= HELMS_END; ++i) {
			if(res <= ArmorDropWeights[DND_DROPPEDARMOR_HELM][i]) {
				res = i;
				break;
			}
		}
	}
	else
		res = helm;

	if(item_tier > GetCVar("dnd_maxmonsterlevel"))
		item_tier = GetCVar("dnd_maxmonsterlevel");

	Inventories_On_Field[item_pos].item_level = item_tier;
	Inventories_On_Field[item_pos].item_stack = 0;
	Inventories_On_Field[item_pos].item_type = DND_ITEM_HELM;
	Inventories_On_Field[item_pos].item_subtype = res;
	Inventories_On_Field[item_pos].width = DND_HELM_BASEWIDTH;
	Inventories_On_Field[item_pos].height = DND_HELM_BASEHEIGHT;

	Inventories_On_Field[item_pos].corrupted = false;
	Inventories_On_Field[item_pos].quality = 0;

	for(i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		Inventories_On_Field[item_pos].implicit[i].attrib_id = -1;
		Inventories_On_Field[item_pos].implicit[i].attrib_val = 0;
		Inventories_On_Field[item_pos].implicit[i].attrib_tier = 0;
		Inventories_On_Field[item_pos].implicit[i].attrib_extra = 0;
	}
	
	Inventories_On_Field[item_pos].attrib_count = 0;
	for(i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		Inventories_On_Field[item_pos].attributes[i].attrib_id = -1;
	return res;
}

// extra holds m_id
int InitializeArmor(int item_pos, int item_tier, int pnum, int tiers = 0, int extra = -1) {
	int armor_type = ConstructArmorDataOnField(item_pos, item_tier, tiers, extra);
	Inventories_On_Field[item_pos].item_image = IIMG_ARM_1 + armor_type;
	return armor_type;
}

int InitializeBoot(int item_pos, int item_tier, int pnum) {
	int armor_type = ConstructBootDataOnField(item_pos, item_tier);
	Inventories_On_Field[item_pos].item_image = IIMG_BOO_1 + armor_type;
	return armor_type;
}

int InitializeHelm(int item_pos, int item_tier, int pnum, int type = -1) {
	int armor_type = ConstructHelmDataOnField(item_pos, item_tier, type);
	Inventories_On_Field[item_pos].item_image = IIMG_HLM_1 + armor_type;
	return armor_type;
}

void RollArmorInfo(int item_pos, int item_tier, int pnum, int item_type, int armor_type, int max_attr) {
	// only for rolling body armors we access the array for item_tier, as that can be changed in ConstructArmorDataOnField to level this down for lower level players
	int i = 0, roll;
	int count = random(1, max_attr);
	int special_roll = SetupItemImplicit(item_pos, item_type, armor_type, item_tier);

	while(i < count) {
		do {
			roll = PickRandomAttribute(item_type, armor_type, special_roll, Inventories_On_Field[item_pos].implicit[0].attrib_id);
		} while(CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(item_pos, roll, pnum, count);
		++i;
	}
}

str GetArmorDropClass(int type) {
	return StrParam(s:"ArmorDrop_", d:type);
}

str GetBootDropClass(int type) {
	return StrParam(s:"BootDrop_", d:type);
}

str GetHelmDropClass(int type) {
	return StrParam(s:"HelmDrop_", d:type);
}

int GetArmorID(int pnum = -1) {
	if(pnum == -1)
		pnum = PlayerNumber();

	if(Items_Used[pnum][BODY_ARMOR_INDEX].item_type == DND_ITEM_NULL)
		return -1;

	return Items_Used[pnum][BODY_ARMOR_INDEX].item_subtype;
}

int GetActorArmorID(int tid) {
	return GetArmorID(tid - P_TIDSTART);
}

int GetHelmID(int pnum = -1) {
	if(pnum == -1)
		pnum = PlayerNumber();

	if(Items_Used[pnum][HELM_INDEX].item_type == DND_ITEM_NULL)
		return -1;

	return Items_Used[pnum][HELM_INDEX].item_subtype;
}

bool ActorHasNoArmor(int tid) {
	int pnum = tid - P_TIDSTART;
	return Items_Used[pnum][BODY_ARMOR_INDEX].item_type == DND_ITEM_NULL && Items_Used[pnum][BOOT_INDEX].item_type == DND_ITEM_NULL && Items_Used[pnum][HELM_INDEX].item_type == DND_ITEM_NULL;
}

int DoArmorRatingEffect(int dmg, int rating) {
	// you will need 8 times the damage to gain half reduction
	int dmg_f = BASE_ARMOR_FACTOR;
	dmg_f = dmg - (dmg * rating) / (rating + dmg_f * dmg);
	return dmg_f;
}

void SetEnergyShield(int val) {
	SetInventory("EShieldAmount", val);
	SetInventory("EShieldAmountVisual", val);
}

void SetActorEnergyShield(int tid, int val) {
	SetActorInventory(tid, "EShieldAmount", val);
	SetActorInventory(tid, "EShieldAmountVisual", val);
}

void AddEnergyShield(int val) {
	GiveInventory("EShieldAmount", val);
	GiveInventory("EShieldAmountVisual", val);
}

void AddActorEnergyShield(int tid, int val) {
	GiveActorInventory(tid, "EShieldAmount", val);
	GiveActorInventory(tid, "EShieldAmountVisual", val);
}

void TakeEnergyShield(int val) {
	TakeInventory("EShieldAmount", val);
	TakeInventory("EShieldAmountVisual", val);
}

void UpdateEnergyShieldVisual(int val) {
	SetAmmoCapacity("EShieldAmountVisual", val);
}

// Absorb value for magic or poison attacks
int GetEShieldMagicAbsorbValue(int pnum) {
	if(GetPlayerAttributeValue(pnum, INV_EX_ESHIELDFULLABSORB) || HasPlayerPowerset(pnum, PPOWER_ESHIELDBLOCKALL))
		return 100;
	return GetPlayerAttributeValue(pnum, INV_MAGIC_NEGATION);
}

int GetMitigationChance(int pnum) {
	int base = GetDexterityEffect(pnum, DND_MIT_PER_DEX) + GetPlayerAttributeValue(pnum, INV_MIT_INCREASE);
	base += CheckActorInventory(pnum + P_TIDSTART, "DnD_HasAmphetamine") * DND_AMPHETAMINE_MITIGATIONCHANCE;
	return base;
}

bool CouldMitigateDamage(int pnum) {
	//Log(f:random(1.0, 100.0), s: " vs ", f:GetMitigationChance(pnum));
	return random(0.0, 100.0) <= GetMitigationChance(pnum);
}

int GetMitigationEffect(int pnum) {
	int mit_eff = GetPlayerAttributeValue(pnum, INV_MITEFFECT_INCREASE) + DND_MIT_BASE;
	mit_eff += CheckActorInventory(pnum + P_TIDSTART, "DnD_HasAmphetamine") * DND_AMPHETAMINE_MITIGATIONEFFECT;
	if(mit_eff > DND_MIT_MAXEFFECT)
		mit_eff = DND_MIT_MAXEFFECT;
	return mit_eff;
}

Script "DnD Armor Item Pickup" (int sp) {
    if((sp & 0xFFFF) == 255)
		SetActivatorToTarget(0);
	else
		SetActivator((sp & 0xFFFF) + P_TIDSTART);

	ACS_NamedExecuteAlways("DnD Armor Message", 0, Inventories_On_Field[sp >> 16].item_subtype, Inventories_On_Field[sp >> 16].item_type);
    GiveInventory("ArmorSoundPlayer", 1);
	
    HandleInventoryPickup(sp >> 16);
}

Script "DnD Armor Message" (int id, int type) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	if(type > UNIQUE_BEGIN) {
		Log(s:StrParam(s:"\cc", l:"DND_PICKUP_ARMOR", s:": \c[Y5]", l:GetUniqueItemName(type & 0xFFFF, (type >> UNIQUE_BITS) - 1), s:"!\c-"));
	}
	else
		Log(s:StrParam(s:"\cc", l:"DND_PICKUP_ARMOR", s:": \c[Y5]", l:GetItemTagName(type, id), s:"!\c-"));
}

Script "DnD Drop Random Basic Armor" (int higher_tier, int dont_remove) {
	// wait for the setup phase to finish
	while(!isSetupComplete(SETUP_STATE1, SETUP_ITEMTABLES) || !IsSetupComplete(SETUP_STATE1, SETUP_PLAYERINFO_MINMAXLEVELS) || IsSetupComplete(SETUP_STATE1 , SETUP_PLAYERDATAFINISHED))
		Delay(const:TICRATE);

	Delay(const:HALF_TICRATE);

	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && !PlayerIsSpectator(i) && IsActorAlive(i + P_TIDSTART))
			SpawnArmor(i, 0, higher_tier, true);
	}

	Delay(const:TICRATE);

	if(!dont_remove)
		Thing_Remove(0);
}

#endif