#ifndef DND_ARMOR_IN
#define DND_ARMOR_IN

#define DND_MIT_PER_DEX 0.075
#define DND_MIT_BASE 50.0 // 50%
#define DND_MIT_MAXEFFECT 90.0

#define BASE_ARMOR_FACTOR 6

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
};
#define BODYARMORS_BEGIN BODYARMOR_GREEN
#define BODYARMORS_REGULAREND BODYARMOR_RAVAGER
#define BODYARMORS_END BODYARMOR_LIGHTNINGCOIL

#define BOOTS_BEGIN BOOTS_SILVER
#define BOOTS_END BOOTS_DRAKESKIN

#define HELMS_BEGIN HELMS_LICH
#define HELMS_END HELMS_SYNTHMETAL

enum {
	ARMWEIGHT_GREEN = 20,
	ARMWEIGHT_YELLOW = 35,
	ARMWEIGHT_BLUE = 45,
	ARMWEIGHT_RED = 50,

	ARMWEIGHT_GUNSLINGER = 56,
	ARMWEIGHT_OCCULT = 62,
	ARMWEIGHT_DEMO = 68,
	ARMWEIGHT_ENERGY = 74,
	ARMWEIGHT_ELEMENTAL = 80,

	ARMWEIGHT_MONOLITH = 81,
	ARMWEIGHT_CYBER = 85,
	ARMWEIGHT_DUELIST = 90,
	ARMWEIGHT_NECRO = 93,
	ARMWEIGHT_KNIGHT = 96,
	ARMWEIGHT_RAVAGER = 100
};

int ArmorDropWeights[BODYARMORS_REGULAREND + 1] = {
	ARMWEIGHT_GREEN,
	ARMWEIGHT_YELLOW,
	ARMWEIGHT_BLUE,
	ARMWEIGHT_RED,

	ARMWEIGHT_GUNSLINGER,
	ARMWEIGHT_OCCULT,
	ARMWEIGHT_DEMO,
	ARMWEIGHT_ENERGY,
	ARMWEIGHT_ELEMENTAL,

	ARMWEIGHT_MONOLITH,
	ARMWEIGHT_CYBER,
	ARMWEIGHT_DUELIST,
	ARMWEIGHT_NECRO,
	ARMWEIGHT_KNIGHT,
	ARMWEIGHT_RAVAGER
};

enum {
	BOOTWEIGHT_SILVER = 15,
	BOOTWEIGHT_ENGINEER = 30,
	BOOTWEIGHT_INSULATED = 35,
	BOOTWEIGHT_POWER = 40,
	BOOTWEIGHT_ENERGY = 45,
	BOOTWEIGHT_TACTICAL = 49,
	BOOTWEIGHT_FUSION = 55,
	BOOTWEIGHT_LEATHER = 70,
	BOOTWEIGHT_SNAKESKIN = 85,
	BOOTWEIGHT_DRAKESKIN = 100
};

int BootDropWeights[BOOTS_END + 1] = {
	BOOTWEIGHT_SILVER,
	BOOTWEIGHT_ENGINEER,
	BOOTWEIGHT_INSULATED,
	BOOTWEIGHT_POWER,
	BOOTWEIGHT_ENERGY,
	BOOTWEIGHT_TACTICAL,
	BOOTWEIGHT_FUSION,
	BOOTWEIGHT_LEATHER,
	BOOTWEIGHT_SNAKESKIN,
	BOOTWEIGHT_DRAKESKIN
};

enum {
	HELMWEIGHT_LICH = 25,
	HELMWEIGHT_WARRIOR = 50,
	HELMWEIGHT_SYNTH = 100
};

int HelmDropWeights[HELMS_END + 1] = {
	HELMWEIGHT_LICH,
	HELMWEIGHT_WARRIOR,
	HELMWEIGHT_SYNTH
};

#define DND_BODYARMOR_BASEWIDTH 2
#define DND_BODYARMOR_BASEHEIGHT 2

#define DND_BOOT_BASEWIDTH 2
#define DND_BOOT_BASEHEIGHT 1

#define DND_HELM_BASEWIDTH 1
#define DND_HELM_BASEHEIGHT 1

#define WARRIORHELM_RANGEINC 0.15
#define WARRIORHELM_DMGINC 25

#define MAX_HELM_ATTRIB_DEFAULT 4
#define MAX_BOOT_ATTRIB_DEFAULT 4
#define MAX_ARMOR_ATTRIB_DEFAULT 6

// returns type of charm as result
int ConstructArmorDataOnField(int item_pos, int item_tier, int tiers = 0) {
    // decide what type of armor to spawn here -- droppers have tiers not equal to zero, so they can determine some easy armors to drop
	int res, i;
	if(!tiers) {
		// pick with some weight here -- <= here for index, size is that +1
		res = random(1, 100);
		for(i = 0; i <= BODYARMORS_REGULAREND; ++i)
			if(res <= ArmorDropWeights[i]) {
				res = i;
				break;
			}
		//res = BODYARMOR_CYBER;
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
		if(!item_tier)
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
		if(!item_tier)
			item_tier = 1;
	}
	else if(tiers < 0)
		res = -tiers;

	Inventories_On_Field[item_pos].item_level = item_tier;
	Inventories_On_Field[item_pos].item_stack = 0;
	Inventories_On_Field[item_pos].item_type = DND_ITEM_BODYARMOR;
	Inventories_On_Field[item_pos].item_subtype = res;
	Inventories_On_Field[item_pos].width = DND_BODYARMOR_BASEWIDTH;
	Inventories_On_Field[item_pos].height = DND_BODYARMOR_BASEHEIGHT;

	Inventories_On_Field[item_pos].corrupted = false;
	Inventories_On_Field[item_pos].quality = 0;
	Inventories_On_Field[item_pos].implicit.attrib_id = -1;
	Inventories_On_Field[item_pos].implicit.attrib_val = 0;
	Inventories_On_Field[item_pos].implicit.attrib_tier = 0;
	Inventories_On_Field[item_pos].implicit.attrib_extra = 0;
	
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
		if(res <= BootDropWeights[i]) {
			res = i;
			break;
		}

	Inventories_On_Field[item_pos].item_level = item_tier;
	Inventories_On_Field[item_pos].item_stack = 0;
	Inventories_On_Field[item_pos].item_type = DND_ITEM_BOOT;
	Inventories_On_Field[item_pos].item_subtype = res;
	Inventories_On_Field[item_pos].width = DND_BOOT_BASEWIDTH;
	Inventories_On_Field[item_pos].height = DND_BOOT_BASEHEIGHT;

	Inventories_On_Field[item_pos].corrupted = false;
	Inventories_On_Field[item_pos].quality = 0;
	Inventories_On_Field[item_pos].implicit.attrib_id = -1;
	Inventories_On_Field[item_pos].implicit.attrib_val = 0;
	Inventories_On_Field[item_pos].implicit.attrib_tier = 0;
	Inventories_On_Field[item_pos].implicit.attrib_extra = 0;
	
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
			if(res <= HelmDropWeights[i]) {
				res = i;
				break;
			}
		}
	}
	else
		res = helm;

	Inventories_On_Field[item_pos].item_level = item_tier;
	Inventories_On_Field[item_pos].item_stack = 0;
	Inventories_On_Field[item_pos].item_type = DND_ITEM_HELM;
	Inventories_On_Field[item_pos].item_subtype = res;
	Inventories_On_Field[item_pos].width = DND_HELM_BASEWIDTH;
	Inventories_On_Field[item_pos].height = DND_HELM_BASEHEIGHT;

	Inventories_On_Field[item_pos].corrupted = false;
	Inventories_On_Field[item_pos].quality = 0;
	Inventories_On_Field[item_pos].implicit.attrib_id = -1;
	Inventories_On_Field[item_pos].implicit.attrib_val = 0;
	Inventories_On_Field[item_pos].implicit.attrib_tier = 0;
	Inventories_On_Field[item_pos].implicit.attrib_extra = 0;
	
	Inventories_On_Field[item_pos].attrib_count = 0;
	for(i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
		Inventories_On_Field[item_pos].attributes[i].attrib_id = -1;
	return res;
}

int RollArmorInfo(int item_pos, int item_tier, int pnum, int tiers = 0) {
	// roll random attributes for the charm
	int i = 0, roll;
	int armor_type = ConstructArmorDataOnField(item_pos, item_tier, tiers);

	//armor_type = BODYARMOR_CYBER;

	int count = random(1, MAX_ARMOR_ATTRIB_DEFAULT);
	int special_roll = 0;

	Inventories_On_Field[item_pos].item_image = IIMG_ARM_1 + armor_type;

	// implicits that come along with the item always
	switch(armor_type) {
		case BODYARMOR_GREEN:
			// 100 armor, 33% damage reduction
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 100, 0, item_tier, 50);
		break;
		case BODYARMOR_YELLOW:
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 150, 0, item_tier, 75);
		break;
		case BODYARMOR_BLUE:
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 200, 0, item_tier, 100);
		break;
        case BODYARMOR_RED:
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 300, 0, item_tier, 100);
		break;

		case BODYARMOR_GUNSLINGER:
			special_roll = PPOWER_CANROLLPHYS;
			GiveImplicitToField(item_pos, INV_IMP_INCMIT, 20.0, PPOWER_CANROLLPHYS, item_tier, 1.25);
		break;
		case BODYARMOR_OCCULT:
			special_roll = PPOWER_CANROLLOCCULT;
			GiveImplicitToField(item_pos, INV_IMP_INCMITSHIELD, 80, PPOWER_CANROLLOCCULT, item_tier, 30);
		break;
		case BODYARMOR_DEMO:
			special_roll = PPOWER_CANROLLEXP;
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 175, PPOWER_CANROLLEXP, item_tier, 100);
		break;
		case BODYARMOR_ENERGY:
			special_roll = PPOWER_CANROLLENERGY;
			GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 80, PPOWER_CANROLLENERGY, item_tier, 30);
		break;
		case BODYARMOR_ELEMENTAL:
			special_roll = PPOWER_CANROLLELEMENTAL;
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 125, PPOWER_CANROLLELEMENTAL, item_tier, 60);
		break;

		case BODYARMOR_MONOLITH:
			GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 115, 0, item_tier, 40);
		break;
		case BODYARMOR_CYBER:
			GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 120, PPOWER_CYBER, item_tier, 40);
		break;
		case BODYARMOR_DUELIST:
			GiveImplicitToField(item_pos, INV_IMP_INCMIT, 20.0, PPOWER_HITSCANPROTECT, item_tier, 1.75);
		break;
		case BODYARMOR_NECRO:
			GiveImplicitToField(item_pos, INV_IMP_INCMITARMOR, 150, PPOWER_SPIKES, item_tier, 60);
		break;
		case BODYARMOR_KNIGHT:
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 250, PPOWER_KNIGHTMELEEBONUS, item_tier, 100);
		break;
		case BODYARMOR_RAVAGER:
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 200, PPOWER_RAVAGER, item_tier, 80);
		break;

		case BODYARMOR_SYNTHMETAL:
			GiveImplicitToField(item_pos, INV_IMP_INCMITARMOR, 180, PPOWER_LOWERREFLECT, item_tier, 80);
		break;
		case BODYARMOR_LIGHTNINGCOIL:
			GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 80, PPOWER_LIGHTNINGABSORB, item_tier, 25);
		break;
	}

	while(i < count) {
		do {
			roll = PickRandomAttribute(DND_ITEM_BODYARMOR, special_roll);
		} while(CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(item_pos, roll, pnum, count);
		++i;
	}

	return armor_type;
}

int RollBootInfo(int item_pos, int item_tier, int pnum) {
	// roll random attributes for the charm
	int i = 0, roll;
	int armor_type = ConstructBootDataOnField(item_pos, item_tier);
	int count = random(1, MAX_BOOT_ATTRIB_DEFAULT);
	int special_roll = 0;

	Inventories_On_Field[item_pos].item_image = IIMG_BOO_1 + armor_type;
	// implicits that come along with the item always
	switch(armor_type) {
		case BOOTS_SILVER:
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 50, PPOWER_INCMAGICRES, item_tier, 20);
		break;
		case BOOTS_ENGINEER:
			GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 16, PPOWER_INCENERGYRES, item_tier, 5);
		break;
		case BOOTS_INSULATED:
			GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 10, PPOWER_REDUCEDLIGHTNINGTAKEN, item_tier, 5);
		break;
		case BOOTS_PLATED:
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 75, 0, item_tier, 25);
		break;
		case BOOTS_POWER:
			GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 25, PPOWER_OVERHEATGOFAST, item_tier, 8);
		break;
		case BOOTS_ENERGY:
			GiveImplicitToField(item_pos, INV_IMP_INCSHIELD, 12, PPOWER_REDUCEDSELFDMG, item_tier, 6);
		break;
		case BOOTS_TACTICAL:
			GiveImplicitToField(item_pos, INV_IMP_INCMITARMOR, 45, 0, item_tier, 25);
		break;
		case BOOTS_FUSION:
			GiveImplicitToField(item_pos, INV_IMP_INCMITSHIELD, 18, PPOWER_LESSOVERHEAT, item_tier, 8);
		break;
		case BOOTS_LEATHER:
			GiveImplicitToField(item_pos, INV_IMP_INCMIT, 7.5, 0, item_tier, 0.5);
		break;
		case BOOTS_SNAKESKIN:
			GiveImplicitToField(item_pos, INV_IMP_INCMIT, 4.0, PPOWER_REDUCEDPOISONTAKEN, item_tier, 0.5);
		break;
		case BOOTS_DRAKESKIN:
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 35, PPOWER_REDUCEDFIRETAKEN, item_tier, 10);
		break;
	}

	while(i < count) {
		do {
			roll = PickRandomAttribute(DND_ITEM_BOOT, special_roll);
		} while(CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(item_pos, roll, pnum, count);
		++i;
	}

	return armor_type;
}

int RollHelmInfo(int item_pos, int item_tier, int pnum, int type = -1) {
	// roll random attributes for the charm
	int i = 0, roll;
	int armor_type = ConstructHelmDataOnField(item_pos, item_tier, type);
	int count = random(1, MAX_HELM_ATTRIB_DEFAULT);
	int special_roll = 0;

	Inventories_On_Field[item_pos].item_image = IIMG_HLM_1 + armor_type;
	// implicits that come along with the item always
	switch(armor_type) {
		case HELMS_LICH:
			GiveImplicitToField(item_pos, INV_IMP_INCARMORSHIELD, 50, PPOWER_PETCAP, item_tier, 20);
		break;
		case HELMS_WARRIOR:
			GiveImplicitToField(item_pos, INV_IMP_INCARMOR, 80, PPOWER_MELEEDAMAGE, item_tier, 40);
		break;
		case HELMS_SYNTHMETAL:
			GiveImplicitToField(item_pos, INV_IMP_INCMIT, 10.0, PPOWER_SYNTHMETALMASK, item_tier, 1.25);
		break;
	}

	while(i < count) {
		do {
			roll = PickRandomAttribute(DND_ITEM_HELM, special_roll);
		} while(CheckItemAttribute(pnum, item_pos, roll, DND_SYNC_ITEMSOURCE_FIELD, count) != -1);
		AddAttributeToFieldItem(item_pos, roll, pnum, count);
		++i;
	}

	return armor_type;
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
	// you will need 5 times the damage to gain half reduction
	int dmg_f = BASE_ARMOR_FACTOR - HasMasteredPerk(STAT_END);
	return dmg - (dmg * rating) / (rating + dmg_f * dmg);
}

void SetEnergyShield(int val) {
	SetInventory("EShieldAmount", val);
	SetInventory("EShieldAmountVisual", val);
}

void AddEnergyShield(int val) {
	GiveInventory("EShieldAmount", val);
	GiveInventory("EShieldAmountVisual", val);
}

void TakeEnergyShield(int val) {
	TakeInventory("EShieldAmount", val);
	TakeInventory("EShieldAmountVisual", val);
}

void UpdateEnergyShieldVisual(int val) {
	SetAmmoCapacity("EShieldAmountVisual", val);
}

int GetMitigationChance(int pnum) {
	return GetDexterity() * DND_MIT_PER_DEX + GetPlayerAttributeValue(pnum, INV_MIT_INCREASE);
}

bool CouldMitigateDamage(int pnum) {
	//Log(f:random(1.0, 100.0), s: " vs ", f:GetMitigationChance(pnum));
	return random(1.0, 100.0) <= GetMitigationChance(pnum);
}

int GetMitigationEffect(int pnum) {
	int mit_eff = GetPlayerAttributeValue(pnum, INV_MITEFFECT_INCREASE) + DND_MIT_BASE;
	if(mit_eff > DND_MIT_MAXEFFECT)
		mit_eff = DND_MIT_MAXEFFECT;
	return mit_eff;
}

str GetArmorInventoryTag(int subt) {
	return StrParam(l:StrParam(s:"DND_ARMOR", d:subt + 1));
}

str GetBootInventoryTag(int subt) {
	return StrParam(l:StrParam(s:"DND_BOOT", d:subt + 1));
}

str GetHelmInventoryTag(int subt) {
	return StrParam(l:StrParam(s:"DND_HELM", d:subt + 1));
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
		type = (type >> UNIQUE_BITS) - 1;
		Log(s:StrParam(s:"\cc", l:"DND_PICKUP_ARMOR", s:": \c[Y5]", l:GetUniqueItemName(type), s:"!\c-"));
	}
	else if(type == DND_ITEM_BODYARMOR)
		Log(s:StrParam(s:"\cc", l:"DND_PICKUP_ARMOR", s:": \c[Y5]", l:GetArmorInventoryTag(id), s:"!\c-"));
	else if(type == DND_ITEM_BOOT)
		Log(s:StrParam(s:"\cc", l:"DND_PICKUP_ARMOR", s:": \c[Y5]", l:GetBootInventoryTag(id), s:"!\c-"));
	else if(type == DND_ITEM_HELM)
		Log(s:StrParam(s:"\cc", l:"DND_PICKUP_ARMOR", s:": \c[Y5]", l:GetHelmInventoryTag(id), s:"!\c-"));
}

Script "DnD Drop Random Basic Armor" (int higher_tier, int dont_remove) {
	// wait for the setup phase to finish
	while(!isSetupComplete(SETUP_STATE1, SETUP_ITEMTABLES) || !IsSetupComplete(SETUP_STATE1, SETUP_PLAYERINFO_MINMAXLEVELS))
		Delay(const:TICRATE);

	Delay(const:HALF_TICRATE);

	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(PlayerInGame(i) && !PlayerIsSpectator(i) && IsActorAlive(i + P_TIDSTART))
			SpawnArmor(i, true, higher_tier, true);
	}

	Delay(const:TICRATE);

	if(!dont_remove)
		Thing_Remove(0);
}

#endif