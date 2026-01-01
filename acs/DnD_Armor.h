#ifndef DND_ARMOR_IN
#define DND_ARMOR_IN

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
	BOOTS_WARRIORSABATON,

	HELMS_LICH = 0,
	HELMS_WARRIOR,
	HELMS_SYNTHMETAL,
	HELMS_KNIGHT,
	HELMS_PREDATOR,
	HELMS_CRUSADER,
	HELMS_TACTICAL,
	HELMS_ROBE,
	HELMS_ELDER
};
#define BODYARMORS_BEGIN BODYARMOR_GREEN
#define BODYARMORS_REGULAREND BODYARMOR_TANGLEDRIBCAGE
#define BODYARMORS_END BODYARMOR_LIGHTNINGCOIL

#define BOOTS_BEGIN BOOTS_SILVER
#define BOOTS_END BOOTS_WARRIORSABATON

#define HELMS_BEGIN HELMS_LICH
#define HELMS_END HELMS_ELDER

enum {
	DND_DROPPEDARMOR_BODY,
	DND_DROPPEDARMOR_BOOT,
	DND_DROPPEDARMOR_HELM,

	DND_DROPPEDARMOR_LAST
};

#define MAX_SUB_ARMORTYPES 64 // increase if needed
global int 50: ArmorDropWeights[DND_DROPPEDARMOR_LAST][MAX_SUB_ARMORTYPES];

#define SET_ARMOR_WEIGHT(X, Y, W) 	ArmorDropWeights[X][Y] = weight + W; \
									weight += W

#define MAX_BODYARMOR_WEIGHT ArmorDropWeights[DND_DROPPEDARMOR_BODY][BODYARMORS_REGULAREND]
#define MAX_BOOT_WEIGHT ArmorDropWeights[DND_DROPPEDARMOR_BOOT][BOOTS_END]
#define MAX_HELM_WEIGHT ArmorDropWeights[DND_DROPPEDARMOR_HELM][HELMS_END]

void SetupArmorDropWeights() {
	// Body Armors
	int weight = 0;
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_GREEN, 16);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_YELLOW, 14);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_BLUE, 10);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_RED, 6);

	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_GUNSLINGER, 6);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_OCCULT, 6);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_DEMO, 6);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_ENERGY, 6);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_ELEMENTAL, 6);

	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_MONOLITH, 3);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_CYBER, 3);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_DUELIST, 8);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_NECRO, 3);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_KNIGHT, 3);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_RAVAGER, 4);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BODY, BODYARMOR_TANGLEDRIBCAGE, 2);
	
	// Boots
	weight = 0;
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BOOT, BOOTS_SILVER, 15);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BOOT, BOOTS_ENGINEER, 15);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BOOT, BOOTS_INSULATED, 5);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BOOT, BOOTS_PLATED, 10);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BOOT, BOOTS_POWER, 5);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BOOT, BOOTS_ENERGY, 5);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BOOT, BOOTS_TACTICAL, 3);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BOOT, BOOTS_FUSION, 12);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BOOT, BOOTS_LEATHER, 15);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BOOT, BOOTS_SNAKESKIN, 10);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BOOT, BOOTS_DRAKESKIN, 10);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_BOOT, BOOTS_WARRIORSABATON, 4);

	// Helms
	weight = 0;
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_HELM, HELMS_LICH, 10);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_HELM, HELMS_WARRIOR, 12);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_HELM, HELMS_SYNTHMETAL, 24);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_HELM, HELMS_KNIGHT, 12);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_HELM, HELMS_PREDATOR, 8);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_HELM, HELMS_CRUSADER, 3);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_HELM, HELMS_TACTICAL, 14);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_HELM, HELMS_ROBE, 10);
	SET_ARMOR_WEIGHT(DND_DROPPEDARMOR_HELM, HELMS_ELDER, 1);
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
		res = random(1, MAX_BODYARMOR_WEIGHT);
		for(i = 0; i <= BODYARMORS_REGULAREND; ++i) {
			if(res <= ArmorDropWeights[DND_DROPPEDARMOR_BODY][i]) {
				if(i == BODYARMOR_TANGLEDRIBCAGE && MonsterProperties[extra].level < ARMOR_TANGLED_DROPLVL) {
					// repeat if this happened
					res = random(1, MAX_BODYARMOR_WEIGHT);
					i = -1;
					continue;
				}
				res = i;
				break;
			}
		}
#ifdef ISDEBUGBUILD
		res = random(BODYARMORS_BEGIN, BODYARMORS_END);
		//res = BODYARMOR_SYNTHMETAL;
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
	int res = random(1, MAX_BOOT_WEIGHT);
	for(i = 0; i <= BOOTS_END; ++i)
		if(res <= ArmorDropWeights[DND_DROPPEDARMOR_BOOT][i]) {
			res = i;
			break;
		}

#ifdef ISDEBUGBUILD
	res = BOOTS_WARRIORSABATON;
#endif

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
		res = random(1, MAX_HELM_WEIGHT);
		for(i = 0; i <= HELMS_END; ++i) {
			if(res <= ArmorDropWeights[DND_DROPPEDARMOR_HELM][i]) {
				res = i;
				break;
			}
		}
	}
	else
		res = helm;

	//res = HELMS_ELDER;

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

void RollArmorInfoWithMods(int item_pos, int item_tier, int pnum, int item_type, int armor_type, int max_attr, int m1, int m2, int m3) {
	// only for rolling body armors we access the array for item_tier, as that can be changed in ConstructArmorDataOnField to level this down for lower level players
	int i = 0, roll;
	int count = random(1, max_attr) - 1 - (m2 != -1) - (m3 != -1);
	int special_roll = SetupItemImplicit(item_pos, item_type, armor_type, item_tier);

	AddAttributeToFieldItem(item_pos, m1, pnum);
	if(m2 != -1)
		AddAttributeToFieldItem(item_pos, m2, pnum);
	if(m3 != -1)
		AddAttributeToFieldItem(item_pos, m3, pnum);

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