#ifndef DND_MONSTERDATA_IN
#define DND_MONSTERDATA_IN

#include "DnD_Common.h"
#include "DnD_Attachments.h"
#include "DnD_MonsterDefs.h"

// this is the base exp value you get for killing a monster of this class, where all bonuses will be applied to
// shifted right by 16 it returns the base droprate multiplier that is applied to base droprates of charms, orbs etc.
int GetMonsterClassBonus(int class) {
	switch(class) {
		case MONSTERCLASS_ZOMBIEMAN:
		return 6 | (9 << 16);
		
		case MONSTERCLASS_SHOTGUNGUY:
		return 9 | (14 << 16);
		
		case MONSTERCLASS_CHAINGUNGUY:
		return 15 | (20 << 16);
		
		case MONSTERCLASS_DEMON:
		return 21 | (25 << 16);
		
		case MONSTERCLASS_SPECTRE:
		return 24 | (27 << 16);
		
		case MONSTERCLASS_IMP:
		return 12 | (18 << 16);
		
		case MONSTERCLASS_CACODEMON:
		return 45 | (50 << 16);
		
		case MONSTERCLASS_PAINELEMENTAL:
		return 54 | (68 << 16);
		
		case MONSTERCLASS_LOSTSOUL:
		return 24 | (18 << 16);
		
		case MONSTERCLASS_REVENANT:
		return 33 | (48 << 16);
		
		case MONSTERCLASS_HELLKNIGHT:
		return 48 | (62 << 16);
		
		case MONSTERCLASS_BARON:
		return 81 | (90 << 16);
		
		case MONSTERCLASS_FATSO:
		return 76 | (85 << 16);
		
		case MONSTERCLASS_ARACHNOTRON:
		return 72 | (85 << 16);
		
		case MONSTERCLASS_ARCHVILE:
		return 90 | (110 << 16);
		
		case MONSTERCLASS_SPIDERMASTERMIND:
		return 600 | (140 << 16);
		
		case MONSTERCLASS_CYBERDEMON:
		return 1080 | (160 << 16);
		
		case MONSTERCLASS_WOLFENSS:
		return 16 | (16 << 16);
	}
	
	return 1;
}

void ScaleMonsterMass(int level) {
	int m = GetActorProperty(0, APROP_MASS);
	int new_m = m * (100 + level * DND_MONSTERMASS_SCALE) / 100;
	
	// overflow check
	if(m < new_m)
		SetActorProperty(0, APROP_MASS, new_m);
	else if(m > new_m)
		SetActorProperty(0, APROP_MASS, INT_MAX);
}

int GetMonsterSizeType(int id) {
	switch(id) {
		case MONSTER_ZOMBIEMAN:
		case MONSTER_SHOTGUNNER:
		case MONSTER_CHAINGUNNER:
		case MONSTER_DEMON:
		case MONSTER_SPECTRE:
		case MONSTER_IMP:
		case MONSTER_NAZI:
		case MONSTER_LOSTSOUL:
		return DND_MONSTERSIZE_SMALL;
		
		case MONSTER_CACODEMON:
		case MONSTER_PAINELEMENTAL:
		case MONSTER_REVENANT:
		case MONSTER_HELLKNIGHT:
		case MONSTER_VILE:
		return DND_MONSTERSIZE_MEDIUM;
		
		case MONSTER_BARON:
		case MONSTER_FATSO:
		case MONSTER_SPIDER:
		case MONSTER_MASTERMIND:
		case MONSTER_CYBERDEMON:
		return DND_MONSTERSIZE_BIG;
		
		default:
		if(id <= DND_CUSTOM_LOSTSOUL_END)
			return DND_MONSTERSIZE_SMALL;
		if(id < DND_CUSTOM_BARON_BEGIN || (id >= DND_CUSTOM_VILE_BEGIN && id <= DND_CUSTOM_VILE_END))
			return DND_MONSTERSIZE_MEDIUM;
		return DND_MONSTERSIZE_BIG;
	}
}

bool IsBlindCastingMonster(int m_id) {
	switch(m_id) {
		case MONSTER_HELLSFURY:
		case MONSTER_OPHIDIAN:
		case MONSTER_IRONLICH:
		return true;
	}
	return false;
}

// this is the single source that gives the monsters their proper class protections
// or their weaknesses
void HandleMonsterClassInnates(int mid, int id) {
	if
	(
		id == MONSTER_ZOMBIEMAN || 
		(id >= DND_CUSTOM_ZOMBIEMAN_BEGIN && id <= DND_CUSTOM_ZOMBIEMAN_END) ||
		(id >= DND_UNIQUE_ZOMBIEMAN_BEGIN && id <= DND_UNIQUE_ZOMBIEMAN_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_ZOMBIEMAN;
	}
	else if
	(
		id == MONSTER_SHOTGUNNER || 
		(id >= DND_CUSTOM_SHOTGUNGUY_BEGIN & id <= DND_CUSTOM_SHOTGUNGUY_END) ||
		(id >= DND_UNIQUE_SHOTGUNGUY_BEGIN & id <= DND_UNIQUE_SHOTGUNGUY_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_SHOTGUNGUY;
	}
	else if
	(
		id == MONSTER_CHAINGUNNER || 
		(id >= DND_CUSTOM_CHAINGUNGUY_BEGIN && id <= DND_CUSTOM_CHAINGUNGUY_END) ||
		(id >= DND_UNIQUE_CHAINGUNGUY_BEGIN && id <= DND_UNIQUE_CHAINGUNGUY_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_CHAINGUNGUY;
	}
	else if
	(
		id == MONSTER_IMP || 
		(id >= DND_CUSTOM_IMP_BEGIN && id <= DND_CUSTOM_IMP_END) ||
		(id >= DND_UNIQUE_IMP_BEGIN && id <= DND_UNIQUE_IMP_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_IMP;
	}
	else if
	(
		id == MONSTER_DEMON || 
		(id >= DND_CUSTOM_DEMON_BEGIN && id <= DND_CUSTOM_DEMON_END) ||
		(id >= DND_UNIQUE_DEMON_BEGIN && id <= DND_UNIQUE_DEMON_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_DEMON;
	}
	else if
	(
		id == MONSTER_SPECTRE || 
		(id >= DND_CUSTOM_SPECTRE_BEGIN && id <= DND_CUSTOM_SPECTRE_END) ||
		(id >= DND_UNIQUE_SPECTRE_BEGIN && id <= DND_UNIQUE_SPECTRE_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_SPECTRE;
	}
	else if
	(
		id == MONSTER_LOSTSOUL || 
		(id >= DND_CUSTOM_LOSTSOUL_BEGIN && id <= DND_CUSTOM_LOSTSOUL_END) ||
		(id >= DND_UNIQUE_LOSTSOUL_BEGIN && id <= DND_UNIQUE_LOSTSOUL_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_LOSTSOUL;
	}
	else if
	(
		id == MONSTER_CACODEMON || 
		(id >= DND_CUSTOM_CACODEMON_BEGIN && id <= DND_CUSTOM_CACODEMON_END) ||
		(id >= DND_UNIQUE_CACODEMON_BEGIN && id <= DND_UNIQUE_CACODEMON_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_CACODEMON;
	}
	else if
	(
		id == MONSTER_PAINELEMENTAL ||
		(id >= DND_CUSTOM_PAINE_BEGIN && id <= DND_CUSTOM_PAINE_END) ||
		(id >= DND_UNIQUE_PAINE_BEGIN && id <= DND_UNIQUE_PAINE_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_PAINELEMENTAL;
	}
	else if
	(
		id == MONSTER_REVENANT ||
		(id >= DND_CUSTOM_REVENANT_BEGIN && id <= DND_CUSTOM_REVENANT_END) ||
		(id >= DND_UNIQUE_REVENANT_BEGIN && id <= DND_UNIQUE_REVENANT_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_REVENANT;
	}
	else if
	(
		id == MONSTER_HELLKNIGHT ||
		(id >= DND_CUSTOM_HK_BEGIN && id <= DND_CUSTOM_HK_END) ||
		(id >= DND_UNIQUE_HK_BEGIN && id <= DND_UNIQUE_HK_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_HELLKNIGHT;
	}
	else if
	(
		id == MONSTER_BARON ||
		(id >= DND_CUSTOM_BARON_BEGIN && id <= DND_CUSTOM_BARON_END) ||
		(id >= DND_UNIQUE_BARON_BEGIN && id <= DND_UNIQUE_BARON_END)
	) 
	{
		MonsterProperties[mid].class = MONSTERCLASS_BARON;
	}
	else if
	(
		id == MONSTER_FATSO || 
		(id >= DND_CUSTOM_FATSO_BEGIN && id <= DND_CUSTOM_FATSO_END) ||
		(id >= DND_UNIQUE_FATSO_BEGIN && id <= DND_UNIQUE_FATSO_END)
	) 
	{
		MonsterProperties[mid].class = MONSTERCLASS_FATSO;
	}
	else if
	(
		id == MONSTER_SPIDER ||
		(id >= DND_CUSTOM_ARACHNO_BEGIN && id <= DND_CUSTOM_ARACHNO_END) ||
		(id >= DND_UNIQUE_ARACHNO_BEGIN && id <= DND_UNIQUE_ARACHNO_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_ARACHNOTRON;
	}
	else if
	(
		id == MONSTER_VILE || 
		(id >= DND_CUSTOM_VILE_BEGIN && id <= DND_CUSTOM_VILE_END) ||
		(id >= DND_UNIQUE_VILE_BEGIN && id <= DND_UNIQUE_VILE_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_ARCHVILE;
	}
	else if
	(
		id == MONSTER_MASTERMIND ||
		(id >= DND_CUSTOM_SM_BEGIN && id <= DND_CUSTOM_SM_END) ||
		(id >= DND_UNIQUE_SM_BEGIN && id <= DND_UNIQUE_SM_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_SPIDERMASTERMIND;
	}
	else if
	(
		id == MONSTER_CYBERDEMON ||
		(id >= DND_CUSTOM_CYBER_BEGIN && id <= LEGENDARY_END) ||
		(id >= DND_UNIQUE_CYBER_BEGIN && id <= DND_UNIQUE_CYBER_END)
	)
	{
		MonsterProperties[mid].class = MONSTERCLASS_CYBERDEMON;
	}
	else if(id == MONSTER_NAZI)
		MonsterProperties[mid].class = MONSTERCLASS_WOLFENSS;
	else {
		// incursion
		switch(id) {
			case MONSTER_CHEX_COMMON:
				MonsterProperties[mid].class = MONSTERCLASS_ZOMBIEMAN;
			break;
			case MONSTER_CHEX_BIPEDAL:
				MonsterProperties[mid].class = MONSTERCLASS_SHOTGUNGUY;
			break;
			case MONSTER_CHEX_HEAVYARMORED:
				MonsterProperties[mid].class = MONSTERCLASS_CHAINGUNGUY;
			break;
			case MONSTER_CHEX_ARMORED:
				MonsterProperties[mid].class = MONSTERCLASS_IMP;
			break;
			case MONSTER_CHEX_LARVA:
				MonsterProperties[mid].class = MONSTERCLASS_DEMON;
			break;
			case MONSTER_CHEX_CYCLOPS:
				MonsterProperties[mid].class = MONSTERCLASS_SPECTRE;
			break;
			case MONSTER_CHEX_FLEMMINE:
				MonsterProperties[mid].class = MONSTERCLASS_LOSTSOUL;
			break;
			case MONSTER_CHEX_SUPERCYCLOPS:
				MonsterProperties[mid].class = MONSTERCLASS_CACODEMON;
			break;
			case MONSTER_CHEX_STRIDICUS:
				MonsterProperties[mid].class = MONSTERCLASS_HELLKNIGHT;
			break;
			case MONSTER_CHEX_MAXIMUS:
				MonsterProperties[mid].class = MONSTERCLASS_BARON;
			break;
			case MONSTER_CHEX_FLEMOBIUS:
				MonsterProperties[mid].class = MONSTERCLASS_ARACHNOTRON;
			break;
			case MONSTER_CHEX_BERNABE:
				MonsterProperties[mid].class = MONSTERCLASS_ARCHVILE;
			break;
			case MONSTER_CHEX_QUADWUMPUS:
				MonsterProperties[mid].class = MONSTERCLASS_REVENANT;
			break;
			case MONSTER_CHEX_FLEMBOMINATION:
				MonsterProperties[mid].class = MONSTERCLASS_SPIDERMASTERMIND;
			break;
			case MONSTER_CHEX_SNOTFOLUS:
				MonsterProperties[mid].class = MONSTERCLASS_CYBERDEMON;
			break;
		}
	}
}

#define DND_ELITE_GAINBONUS 25 // 25%
#define DND_MAGIC_GAINBONUS 10

int GetMonsterLevelDroprateBonus(int lvl) {
	// piecewise function so the early 25 levels increase sharper, then slow down but reach to a sweet spot of x2.5
	/*if(lvl <= 25)
		return 4 * lvl;
	else if(lvl <= 50)
		return lvl * lvl / 100 + 3 * lvl + 19;
	return lvl * lvl / 150 + 184;
	if(lvl <= 25)
		return 4 * lvl;
	return 2 * lvl + 50;*/
	// new formula to ensure a sharp curve earlier levels then settle down
	return FixedDiv(540.0, (60.0 / lvl + 3.0)) >> 16;
}

int GetMonsterRarityDroprateBonus(int rarity) {
	switch(rarity) {
		case DND_MWEIGHT_COMMON:
		return 0;
		
		case DND_MWEIGHT_UNCOMMON:
		return 15;
		
		case DND_MWEIGHT_RARE1:
		return 25;
		
		case DND_MWEIGHT_RARE2:
		return 35;
		
		case DND_MWEIGHT_VERYRARE:
		return 60;
		
		case DND_MWEIGHT_EPIC:
		return 100;
	}
	return 1;
}

/* 
	Multiply the bonus with 1000 / rarity of monster to get a percentage
	This is a linear, simple bonus added on top just because of the rarity of the monster. While rarity is important, the level matters more. However early on it should have a tiny impact still.
*/
#define DND_ELITEBONUS_FROM_RARITY 25
int GetMonsterDropBonus(int drop_base, int level, int rarity, int flags) {
	// first a 50% from elites as a multiplicative bonus, then the rest
	bool isMagic = flags & DND_MONFLAG_ISMAGIC;
	bool isElite = flags & DND_MONFLAG_ISELITE;
	drop_base = drop_base * (100 + DND_ELITEBONUS_FROM_RARITY * isMagic + 2 * isElite * DND_ELITEBONUS_FROM_RARITY) / 100;
	drop_base = drop_base * (100 + GetMonsterLevelDroprateBonus(level)) / 100;
	drop_base = drop_base * (100 + GetMonsterRarityDroprateBonus(rarity)) / 100;
	return drop_base;
}

int GetMonsterGainBonus(int flags) {
	if(flags & DND_MONFLAG_ISMAGIC)
		return DND_MAGIC_GAINBONUS;
	return DND_ELITE_GAINBONUS * !!(flags & DND_MONFLAG_ISELITE);
}

// you gain the returned value for exp, and third of that for credits -- rarity is monster rarity not item related rarity!
void CalculateMonsterGainMult(int m_id, int rarity = DND_MWEIGHT_COMMON) {
	int base = 0;
	int drop_base = 0;
	
	// per lvl we get base 20%, and 30% per lvl if monster is elite
	int pct = 	100 + (MonsterProperties[m_id].level - 1) * (DND_MONSTERBONUS_PERLVL + GetMonsterGainBonus(MonsterProperties[m_id].flags));
	//printbold(s:"pct: ", d:pct);
	
	// if rarity is within general rarity range, get monster class respective bonus
	// it may not be the case for unique bosses, or any monster that is special. So we use the value in here as the base exp reward to be given instead
	if(rarity <= DND_MWEIGHT_COMMON) {
		base = GetMonsterClassBonus(MonsterProperties[m_id].class);
		drop_base = base >> 16;
		base &= 0xFFFF;
		
		//printbold(s:"base: ", d:base);
		base = base * pct / 100;
		//printbold(s:"base after pct ", d:base);
		// depending on rarity apply a more multiplier
		MonsterProperties[m_id].gain = base * DND_MWEIGHT_COMMON / rarity;
		//printbold(s:"base final ", d:MonsterProperties[m_id].gain);

		MonsterProperties[m_id].droprate = 0;
		for(int i = FIRST_MONSTER_TRAIT; i < LAST_MONSTER_TRAIT; ++i)
			if(MonsterProperties[m_id].trait_list[i])
				MonsterProperties[m_id].droprate += GetEliteModPower(i);
	
		drop_base = drop_base * (100 + MonsterProperties[m_id].droprate) / 100;

		// droprate for regular monsters, low level trash tier monsters drop nothing or rarely vs. higher tier and higher level monsters
		MonsterProperties[m_id].droprate = GetMonsterDropBonus(drop_base, MonsterProperties[m_id].level, rarity, MonsterProperties[m_id].flags);
		//printbold(s:"droprate % inc for ", s:GetActorClass(0), s: " of level ", d:MonsterProperties[m_id].level, s: " is ", d:MonsterProperties[m_id].droprate);
		
		MonsterProperties[m_id].rarity_boost = MonsterProperties[m_id].droprate / 3; // was 2
	}
	else {
		// since rarity for us doesnt make sense as a multiplier here we just return the amplified percentage
		base = rarity;
		MonsterProperties[m_id].gain = base = base * pct / 100;
		//printbold(s:"base final special: ", d:MonsterProperties[m_id].gain);
		
		// droprate here for custom dungeon bosses is 0, because they should call their own loot dropper script!
		MonsterProperties[m_id].droprate = 0;
		MonsterProperties[m_id].rarity_boost = 33; // 33% rarity boost on dungeon monsters
	}
}

// this has any special trait that requires a script to run BEFORE an elite status can apply to a monster
void HandlePreInitTraits(int mid, int id) {
	if(MonsterProperties[mid].trait_list[DND_REJUVENATING])
		GiveInventory("Rejuvenate_Script_Run", 1);
	if(MonsterProperties[mid].trait_list[DND_REPEL])
		GiveInventory("Repel_Script_Run", 1);
	if(MonsterProperties[mid].trait_list[DND_CRIPPLE])
		GiveInventory("Cripple_Script_Run", 1);
	if(MonsterProperties[mid].trait_list[DND_PHASING])
		GiveInventory("Phasing_Script_Run", 1);
	if(MonsterProperties[mid].trait_list[DND_VIOLENTRETALIATION]) {
		ACS_NamedExecuteAlways("DnD Aura Giver CS", 0, DND_VIOLENTRETALIATION);
	}
	if(MonsterProperties[mid].trait_list[DND_TEMPORALBUBBLE])
		ACS_NamedExecuteAlways("DnD Aura Giver CS", 0, DND_TEMPORALBUBBLE);
	if(MonsterProperties[mid].trait_list[DND_ENSHROUDED])
		ACS_NamedExecuteAlways("DnD Aura Giver CS", 0, DND_ENSHROUDED);

	if(MonsterProperties[mid].trait_list[DND_REFLECTIVE])
		ACS_NamedExecuteAlways("DND Spawn Attachment", 0, mid + DND_MONSTERTID_BEGIN, DND_ELITEFX_REFLECT);
}

// this is put as a seperate function because 
void HandlePostInitTraits(int m_id, int id, int rarity = DND_MWEIGHT_COMMON, bool isRevived = false) {
	if(MonsterProperties[m_id].trait_list[DND_FORTIFIED]) {
		// full fortify exceptions
		if(id != MONSTER_TERON && id != MONSTER_CHEGOVAX)
			SetInventory("MonsterFortifyCount", MonsterProperties[m_id].maxhp * (DND_FORTIFY_AMOUNT + random(0, 100 - DND_FORTIFY_AMOUNT)) / 100);
		else
			SetInventory("MonsterFortifyCount", MonsterProperties[m_id].maxhp);
	}

	// don't do anything past this point if revived, there is no need, the data for these is already stored
	if(isRevived)
		return;
	
	// calculate the gains multiplier -- this is the safest place to do as most of monster data is now known by this point, like level etc.
	// all unique monsters have highest possible rarity
	if(rarity == -1) {
		if(!IsUniqueMonster(id))
			rarity = GetMonsterRarity(m_id);
		else
			rarity = DND_MWEIGHT_EPIC;
	}
		
	//printbold(s:GetActorClass(0), s:" rarity ", d:rarity, s: " ", d:MonsterProperties[m_id].class, s: " ", d:MonsterProperties[m_id].id);
	
	MonsterProperties[m_id].rarity = rarity;
	MonsterProperties[m_id].dmg_bonus = GetMonsterDMGScaling(m_id, MonsterProperties[m_id].level, false);
	
	CalculateMonsterGainMult(m_id, rarity);

	

	// finally handle the monster resists according to its traits
	InitMonsterResists(m_id);
}

void InitMonsterResists(int m_id) {
	int temp = 0;
	int bonus = 0;
	if(MonsterProperties[m_id].level > DND_MONSTER_RESIST_LEVELS)
		bonus += MONSTER_RES_PER_THRESHOLD + ((MonsterProperties[m_id].level - DND_MONSTER_RESIST_LEVELS) / 10) * MONSTER_RES_PER_PLUS40;
	
	int etherealBonus = MonsterProperties[m_id].trait_list[DND_ETHEREAL] * ETHEREAL_RESIST;

	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_BULLET] = 	MonsterProperties[m_id].trait_list[DND_BULLET_RESIST] * DND_RESIST_FACTOR + bonus +
																	etherealBonus +
																	MonsterProperties[m_id].trait_list[DND_BULLET_IMMUNE] * DND_IMMUNITY_FACTOR;

	// can be changed later, bullet and melee is just "phys" atm
	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_MELEE] = MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_BULLET];

	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_ENERGY] =	-MonsterProperties[m_id].trait_list[DND_ENERGY_WEAKNESS] * DND_WEAKNESS_FACTOR + bonus +
																	MonsterProperties[m_id].trait_list[DND_ENERGY_RESIST] * DND_RESIST_FACTOR + 
																	etherealBonus +
																	MonsterProperties[m_id].trait_list[DND_ENERGY_IMMUNE] * DND_IMMUNITY_FACTOR;

	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_OCCULT] =	-MonsterProperties[m_id].trait_list[DND_MAGIC_WEAKNESS] * DND_WEAKNESS_FACTOR + bonus + 
																	MonsterProperties[m_id].trait_list[DND_MAGIC_RESIST] * DND_RESIST_FACTOR + 
																	MonsterProperties[m_id].trait_list[DND_MAGIC_IMMUNE] * DND_IMMUNITY_FACTOR;
	// this is common to elemental stuff
	temp =	-MonsterProperties[m_id].trait_list[DND_ELEMENTAL_WEAKNESS] * DND_WEAKNESS_FACTOR + bonus +
			MonsterProperties[m_id].trait_list[DND_ELEMENTAL_RESIST] * DND_RESIST_FACTOR + 
			etherealBonus +
			MonsterProperties[m_id].trait_list[DND_ELEMENTAL_IMMUNE] * DND_IMMUNITY_FACTOR;

	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_FIRE] = -MonsterProperties[m_id].trait_list[DND_FIRE_WEAKNESS] * DND_SPECIFICELEWEAKNESS_FACTOR + temp;

	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_ICE] = -MonsterProperties[m_id].trait_list[DND_ICE_WEAKNESS] * DND_SPECIFICELEWEAKNESS_FACTOR + temp;

	// these two dont have their own weakness category yet...
	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_POISON] = temp;
	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_LIGHTNING] = temp;

	// soul is same as magic for now
	MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_SOUL] = MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_OCCULT];

	// creature type bonuses -- only one type allowed per creature
	if(MonsterProperties[m_id].trait_list[DND_ICECREATURE]) {
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_ICE] = DND_IMMUNITY_FACTOR + bonus;
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_FIRE] -= DND_SPECIFICELEWEAKNESS_FACTOR;
	}
	else if(MonsterProperties[m_id].trait_list[DND_FIRECREATURE]) {
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_FIRE] = DND_IMMUNITY_FACTOR + bonus;
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_ICE] -= DND_SPECIFICELEWEAKNESS_FACTOR;
	}
	else if(MonsterProperties[m_id].trait_list[DND_STONECREATURE]) {
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_FIRE] += DND_RESIST_FACTOR;
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_ICE] -= DND_SPECIFICELEWEAKNESS_FACTOR;
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_LIGHTNING] = DND_IMMUNITY_FACTOR + bonus;
	}
	else if(MonsterProperties[m_id].trait_list[DND_EARTHCREATURE]) {
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_LIGHTNING] -= DND_SPECIFICELEWEAKNESS_FACTOR;
		MonsterProperties[m_id].resists[DND_DAMAGECATEGORY_POISON] = DND_IMMUNITY_FACTOR + bonus;
	}

	// do a pass on all categories to clamp them
	for(temp = DND_DAMAGECATEGORY_BEGIN; temp <= DND_DAMAGECATEGORY_SOUL ; ++temp)
		if(MonsterProperties[m_id].resists[temp] > DND_IMMUNITY_FACTOR + bonus)
			MonsterProperties[m_id].resists[temp] = DND_IMMUNITY_FACTOR + bonus;
}

// this is only used in revive of monsters by itself
void HandleSpecialTraits(int tid, int id, bool isRevived = false) {
	int mid = tid - DND_MONSTERTID_BEGIN;
	HandlePreInitTraits(mid, id);
	HandlePostInitTraits(mid, id, -1, isRevived); // this had no params, I don't think sending -1 here will be much of a problem
}

void LoadMonsterTraits(int tid, int monsterid) {
	HandleMonsterClassInnates(tid, monsterid);
	
	// copy preset data to here now
	for(int j = 0; j < MAX_MONSTER_TRAITS_STORED; ++j) {
		//if(MonsterProperties[tid].class == MONSTERCLASS_IMP && MonsterData[monsterid].trait_list[j])
		//	printbold(s:"loading trait ", d:j, s:" for ", s:GetActorclass(ActivatorTID()), s:" <" , d:tid, s: "> == ", d:MonsterData[monsterid].trait_list[j]);
		MonsterProperties[tid].trait_list[j] = MonsterData[monsterid].trait_list[j];
	}
		
	// some of the flags are inherent in actor info, so do make use of that
	MonsterProperties[tid].trait_list[DND_GHOST] 					|= CheckFlag(0, "GHOST");
	MonsterProperties[tid].trait_list[DND_EXPLOSIVE_IMMUNE] 		|= CheckFlag(0, "NORADIUSDMG");
	MonsterProperties[tid].trait_list[DND_ARMORPEN] 				|= CheckFlag(0, "PIERCEARMOR");
	MonsterProperties[tid].trait_list[DND_HARDENED_SKIN] 			|= CheckFlag(0, "DONTRIP");
	MonsterProperties[tid].trait_list[DND_NOPAIN] 					|= CheckFlag(0, "NOPAIN");
	MonsterProperties[tid].trait_list[DND_REFLECTIVE] 				|= CheckFlag(0, "REFLECTIVE");

	GiveInventory("TakeReplacedFlags", 1);

	MonsterProperties[tid].trait_list[DND_LEGENDARY] = isLegendaryMonster(monsterid);
	
	// check for weaknesses and monster not having any kind of resist to this type
	// if magical or undead, give it silver weakness (this is common no exceptions)
	if(MonsterData[monsterid].flags & (DND_MTYPE_UNDEAD_POW | DND_MTYPE_MAGICAL_POW))
		MonsterProperties[tid].trait_list[DND_SILVER_WEAKNESS] = true;
		
	// if robotic, give energy weakness
	if((MonsterData[monsterid].flags & DND_MTYPE_ROBOTIC_POW) && !MonsterProperties[tid].trait_list[DND_ENERGY_RESIST] && !MonsterProperties[tid].trait_list[DND_ENERGY_IMMUNE])
		MonsterProperties[tid].trait_list[DND_ENERGY_WEAKNESS] = true;
	// if magical, give magic weakness
	if((MonsterData[monsterid].flags & DND_MTYPE_MAGICAL_POW) && !MonsterProperties[tid].trait_list[DND_MAGIC_RESIST] && !MonsterProperties[tid].trait_list[DND_MAGIC_IMMUNE])
		MonsterProperties[tid].trait_list[DND_MAGIC_WEAKNESS] = true;

	if(MonsterData[monsterid].flags & DND_MTYPE_FLEMOID_POW) {
		MonsterProperties[tid].trait_list[DND_TOXICBLOOD] = true;
		MonsterProperties[tid].trait_list[DND_FIRE_WEAKNESS] = true;
		MonsterProperties[tid].trait_list[DND_ICE_WEAKNESS] = true;
		MonsterProperties[tid].trait_list[DND_BLOODLESS] = true;
	}
	
	// debug
	//MonsterProperties[tid].trait_list[DND_REPEL] = true;
	
	// this is needed as some monsters have some of the properties in there that need to be applied without being elites
	HandlePreInitTraits(tid, monsterid);
}

void LoadCustomMonsterTraits(int m_id, int mon_type, int traits, int traits2, int traits3) {
	// force reset everything
	for(int j = 0; j < MAX_MONSTER_TRAITS_STORED; ++j) {
		//if(MonsterProperties[m_id].class == MONSTERCLASS_IMP && MonsterData[monsterid].trait_list[j])
		//	printbold(s:"loading trait ", d:j, s:" for ", s:GetActorclass(Activatorm_id()), s:" <" , d:m_id, s: "> == ", d:MonsterData[monsterid].trait_list[j]);
		MonsterProperties[m_id].trait_list[j] = false;
	}

	// some of the flags are inherent in actor info, so do make use of that
	MonsterProperties[m_id].trait_list[DND_GHOST] 					|= CheckFlag(0, "GHOST");
	MonsterProperties[m_id].trait_list[DND_EXPLOSIVE_IMMUNE] 		|= CheckFlag(0, "NORADIUSDMG");
	MonsterProperties[m_id].trait_list[DND_ARMORPEN] 				|= CheckFlag(0, "PIERCEARMOR");
	MonsterProperties[m_id].trait_list[DND_HARDENED_SKIN] 			|= CheckFlag(0, "DONTRIP");
	MonsterProperties[m_id].trait_list[DND_NOPAIN] 					|= CheckFlag(0, "NOPAIN");
	MonsterProperties[m_id].trait_list[DND_REFLECTIVE] 				|= CheckFlag(0, "REFLECTIVE");

	GiveInventory("TakeReplacedFlags", 1);

	if(traits != -1)
		MonsterProperties[m_id].trait_list[traits] = true;
	if(traits2 != -1)
		MonsterProperties[m_id].trait_list[traits2] = true;
	if(traits3 != -1)
		MonsterProperties[m_id].trait_list[traits3] = true;
		
	// check for weaknesses and monster not having any kind of resist to this type
	// if magical or undead, give it silver weakness (this is common no exceptions)
	if(mon_type & (DND_MTYPE_UNDEAD_POW | DND_MTYPE_MAGICAL_POW))
		MonsterProperties[m_id].trait_list[DND_SILVER_WEAKNESS] = true;
		
	// if robotic, give energy weakness
	if((mon_type & DND_MTYPE_ROBOTIC_POW) && !MonsterProperties[m_id].trait_list[DND_ENERGY_RESIST] && !MonsterProperties[m_id].trait_list[DND_ENERGY_IMMUNE])
		MonsterProperties[m_id].trait_list[DND_ENERGY_WEAKNESS] = true;
	// if magical, give magic weakness
	if((mon_type & DND_MTYPE_MAGICAL_POW) && !MonsterProperties[m_id].trait_list[DND_MAGIC_RESIST] && !MonsterProperties[m_id].trait_list[DND_MAGIC_IMMUNE])
		MonsterProperties[m_id].trait_list[DND_MAGIC_WEAKNESS] = true;
		
	HandlePreInitTraits(m_id, -1);
}

// all demon barons, fatsos or arachnos or bosses that are demons can drop a soul ammo
bool CanDropSoulAmmoTID(int tid) {
	int mid = MonsterProperties[tid - DND_MONSTERTID_BEGIN].id;
	return (mid == MONSTER_HELLKNIGHT || (mid >= DND_CUSTOM_HK_BEGIN && mid <= DND_CUSTOM_HK_END) ||
			mid == MONSTER_BARON || (mid >= DND_CUSTOM_BARON_BEGIN && mid <= DND_CUSTOM_BARON_END) || 
			mid == MONSTER_FATSO || (mid >= DND_CUSTOM_FATSO_BEGIN && mid <= DND_CUSTOM_FATSO_END) ||
			mid == MONSTER_SPIDER || (mid >= DND_CUSTOM_ARACHNO_BEGIN && mid <= DND_CUSTOM_ARACHNO_END) ||
			mid == MONSTER_VILE || (mid >= DND_CUSTOM_VILE_BEGIN && mid <= DND_CUSTOM_VILE_END) ||
			isBoss()) && (MonsterData[mid].flags & DND_MTYPE_DEMON_POW);
}

bool CanDropSoulAmmo() {
	return CanDropSoulAmmoTID(ActivatorTID());
}

int GetMonsterRarity(int monster_id) {
	int r = GetActorProperty(monster_id + DND_MONSTERTID_BEGIN, APROP_SCORE);
	if(!r)
		return DND_MWEIGHT_COMMON;
	return r;
}

void HealMonster(int mid, int amount) {
	int hp = GetActorProperty(0, APROP_HEALTH);
	amount = Clamp_Between(amount, 0, MonsterProperties[mid].maxhp - hp);
	SetActorProperty(0, APROP_HEALTH, hp + amount);

	CheckDoomguyExecuteReversal(mid + DND_MONSTERTID_BEGIN);
}

// these are filled by hand, finding the rarest monster of each class is needlessly complicated (find top classes, equal ones must be considered as sharing top spot etc.)
// this requires frequent updates the more monsters are added, but is super fast
void HandleSubordinateSpawn(int tid, int mid) {
	int class = MonsterProperties[mid].class;
	int rng;
	str toSpawn = "";
	switch(class) {
		case MONSTERCLASS_ZOMBIEMAN:
			rng = random(0, 3);
			switch(rng) {
				case 0:
					toSpawn = "FormerRanger";
				break;
				case 1:
					toSpawn = "RapidFireTrooper";
				break;
				case 2:
					toSpawn = "ZombieMarine";
				break;
				case 3:
					toSpawn = "LOSZombieMan";
				break;
			}
		break;
		case MONSTERCLASS_WOLFENSS:
			toSpawn = "WolfensteinSS2";
		break;
		case MONSTERCLASS_SHOTGUNGUY:
			toSpawn = "SSGZombie";
		break;
		case MONSTERCLASS_CHAINGUNGUY:
			toSpawn = "DoubleChaingunner";
		break;
		case MONSTERCLASS_DEMON:
			toSpawn = "PurebredDemon";
		break;
		case MONSTERCLASS_SPECTRE:
			toSpawn = "RavagerGhost";
		break;
		case MONSTERCLASS_IMP:
			toSpawn = "ReaverRobot";
		break;
		case MONSTERCLASS_CACODEMON:
			rng = random(0, 1);
			switch(rng) {
				case 0:
					toSpawn = "CacoLich";
				break;
				case 1:
					toSpawn = "EarthLich";
				break;
			}
		break;
		case MONSTERCLASS_PAINELEMENTAL:
			rng = random(0, 1);
			switch(rng) {
				case 0:
					toSpawn = "BloodLich";
				break;
				case 1:
					toSpawn = "DESentinel";
				break;
			}
		break;
		case MONSTERCLASS_LOSTSOUL:
			rng = random(0, 1);
			switch(rng) {
				case 0:
					toSpawn = "ESoul";
				break;
				case 1:
					toSpawn = "Watcher";
				break;
			}
		break;
		case MONSTERCLASS_REVENANT:
			rng = random(0, 2);
			switch(rng) {
				case 0:
					toSpawn = "WidowMaker";
				break;
				case 1:
					toSpawn = "Cadaver";
				break;
				case 2:
					toSpawn = "CyborgSoldier";
				break;
			}
		break;
		case MONSTERCLASS_HELLKNIGHT:
			rng = random(0, 2);
			switch(rng) {
				case 0:
					toSpawn = "IceGolem";
				break;
				case 1:
					toSpawn = "Putrefier";
				break;
				case 2:
					toSpawn = "Gladiator";
				break;
			}
		break;
		case MONSTERCLASS_BARON:
			toSpawn = "Warmaster";
		break;
		case MONSTERCLASS_FATSO:
			rng = random(0, 2);
			switch(rng) {
				case 0:
					toSpawn = "Gamon";
				break;
				case 1:
					toSpawn = "Mephisto";
				break;
				case 2:
					toSpawn = "GoldGolem";
				break;
			}
		break;
		case MONSTERCLASS_ARACHNOTRON:
			toSpawn = "Manticore";
		break;
		case MONSTERCLASS_ARCHVILE:
			rng = random(0, 2);
			switch(rng) {
				case 0:
					toSpawn = "UndeadPriest";
				break;
				case 1:
					toSpawn = "Horshacker";
				break;
				case 2:
					toSpawn = "FleshWizard";
				break;
			}
		break;
		case MONSTERCLASS_SPIDERMASTERMIND:
			toSpawn = "GoldLich";
		break;
		case MONSTERCLASS_CYBERDEMON:
			rng = random(0, 1);
			switch(rng) {
				case 0:
					toSpawn = "Cerberus";
				break;
				case 1:
					toSpawn = "Thanatos";
				break;
			}
		break;
	}
	
	// create some special fx
	if(Spawn(toSpawn, GetActorX(tid), GetActorY(tid), GetActorZ(tid), DND_SUBORDINATE_TEMPTID, GetActorAngle(tid))) {
		rng = GetActorProperty(tid, APROP_TARGETTID);
		SetActorProperty(DND_SUBORDINATE_TEMPTID, APROP_TARGETTID, rng);
		Thing_Hate(DND_SUBORDINATE_TEMPTID, rng, 4);
		SetActorAngle(DND_SUBORDINATE_TEMPTID, AngleToFace(DND_SUBORDINATE_TEMPTID, rng));
		GiveActorInventory(DND_SUBORDINATE_TEMPTID, "DnD_EliteException", 1);
		Thing_ChangeTID(DND_SUBORDINATE_TEMPTID, 0);
		ACS_NamedExecuteAlways("DnD Subordinate Spawn FX", 0, tid);
	}
}

Script "DnD Subordinate Spawn FX" (int tid) CLIENTSIDE {
	GiveActorInventory(tid, "SubordinateFXSpawner", 1);
}

str LegendaryMonsters[DND_MAX_LEGENDARY] = {
	"DreamingGod",
	"Torrasque",
	"Mordecqai",
	"GodSlayer",
	"Golgoth"
};

void SetupMonsterData() {
	MonsterData[MONSTER_ZOMBIEMAN].health = 20;
	MonsterData[MONSTER_ZOMBIEMAN].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_SHOTGUNNER].health = 30;
	MonsterData[MONSTER_SHOTGUNNER].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_CHAINGUNNER].health = 70;
	MonsterData[MONSTER_CHAINGUNNER].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_DEMON].health = 150;
	MonsterData[MONSTER_DEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SPECTRE].health = 150;
	MonsterData[MONSTER_SPECTRE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_IMP].health = 60;
	MonsterData[MONSTER_IMP].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_CACODEMON].health = 400;
	MonsterData[MONSTER_CACODEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_PAINELEMENTAL].health = 400;
	MonsterData[MONSTER_PAINELEMENTAL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_LOSTSOUL].health = 50;
	MonsterData[MONSTER_LOSTSOUL].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_REVENANT].health = 300;
	MonsterData[MONSTER_REVENANT].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_HELLKNIGHT].health = 500;
	MonsterData[MONSTER_HELLKNIGHT].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_BARON].health = 1000;
	MonsterData[MONSTER_BARON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_FATSO].health = 600;
	MonsterData[MONSTER_FATSO].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_SPIDER].health = 500;
	MonsterData[MONSTER_SPIDER].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_VILE].health = 700;
	MonsterData[MONSTER_VILE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_MASTERMIND].health = 3000;
	MonsterData[MONSTER_MASTERMIND].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_CYBERDEMON].health = 4000;
	MonsterData[MONSTER_CYBERDEMON].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_NAZI].health = 50;
	MonsterData[MONSTER_NAZI].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;

	// zombieman
	MonsterData[MONSTER_ZOMBIEMANGRAY].health = 40;
	MonsterData[MONSTER_ZOMBIEMANGRAY].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIERANGER].health = 45;
	MonsterData[MONSTER_ZOMBIERANGER].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIESMG].health = 35;
	MonsterData[MONSTER_ZOMBIESMG].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIERAPID].health = 30;
	MonsterData[MONSTER_ZOMBIERAPID].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEMARINE].health = 45;
	MonsterData[MONSTER_ZOMBIEMARINE].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIELOS].health = 45;
	MonsterData[MONSTER_ZOMBIELOS].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEPISTOL].health = 25;
	MonsterData[MONSTER_ZOMBIEPISTOL].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEQUAKE1].health = 55;
	MonsterData[MONSTER_ZOMBIEQUAKE1].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEHUNTER].health = 65;
	MonsterData[MONSTER_ZOMBIEHUNTER].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEPROPHET].health = 50;
	MonsterData[MONSTER_ZOMBIEPROPHET].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	
	// shotgunner
	MonsterData[MONSTER_SSGLOS].health = 50;
	MonsterData[MONSTER_SSGLOS].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZSPECSG].health = 50;
	MonsterData[MONSTER_ZSPECSG].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_SGLOS].health = 50;
	MonsterData[MONSTER_SGLOS].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_SAWEDOFF1].health = 50;
	MonsterData[MONSTER_SAWEDOFF1].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_SAWEDOFF2].health = 50;
	MonsterData[MONSTER_SAWEDOFF2].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ROGUE].health = 65;
	MonsterData[MONSTER_ROGUE].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEQUAKE2].health = 70;
	MonsterData[MONSTER_ZOMBIEQUAKE2].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIESSG].health = 75;
	MonsterData[MONSTER_ZOMBIESSG].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;

	// chaingunguy
	MonsterData[MONSTER_ZOMBIEQUAKE3].health = 85;
	MonsterData[MONSTER_ZOMBIEQUAKE3].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZOMBIEMG].health = 85;
	MonsterData[MONSTER_ZOMBIEMG].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_INITIATE].health = 65;
	MonsterData[MONSTER_INITIATE].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_DOUBLEGUNNER].health = 100;
	MonsterData[MONSTER_DOUBLEGUNNER].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_CGMAJOR].health = 110;
	MonsterData[MONSTER_CGMAJOR].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_MRROBOT].health = 100;
	MonsterData[MONSTER_MRROBOT].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_ZSEC].health = 70;
	MonsterData[MONSTER_ZSEC].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_ZSPECMG].health = 60;
	MonsterData[MONSTER_ZSPECMG].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_PLASMAZOMBIE].health = 85;
	MonsterData[MONSTER_PLASMAZOMBIE].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_BERSERKERGUY].health = 150;
	MonsterData[MONSTER_BERSERKERGUY].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;

	// demon
	MonsterData[MONSTER_BLOODDEMON].health = 250;
	MonsterData[MONSTER_BLOODDEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_BLOODFIEND].health = 160;
	MonsterData[MONSTER_BLOODFIEND].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_CYBERFIEND].health = 300;
	MonsterData[MONSTER_CYBERFIEND].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_FLAMEDEMON].health = 200;
	MonsterData[MONSTER_FLAMEDEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_STONEIMP].health = 180;
	MonsterData[MONSTER_STONEIMP].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_SCAVENGER].health = 195;
	MonsterData[MONSTER_SCAVENGER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SOULEATER].health = 200;
	MonsterData[MONSTER_SOULEATER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_NHUMCIGN].health = 275;
	MonsterData[MONSTER_NHUMCIGN].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_STONEDEMON].health = 250;
	MonsterData[MONSTER_STONEDEMON].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_BRUTY].health = 240;
	MonsterData[MONSTER_BRUTY].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SATYR].health = 150;
	MonsterData[MONSTER_SATYR].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_EARTHGOLEM].health = 325;
	MonsterData[MONSTER_EARTHGOLEM].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_RAVAGER].health = 250;
	MonsterData[MONSTER_RAVAGER].flags = DND_MTYPE_DEMON_POW;	
	MonsterData[MONSTER_PUREBREDDEMON].health = 275;
	MonsterData[MONSTER_PUREBREDDEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SABRECLAW].health = 200;
	MonsterData[MONSTER_SABRECLAW].flags = DND_MTYPE_MAGICAL_POW;
	
	// spectre
	MonsterData[MONSTER_LURKER].health = 160;
	MonsterData[MONSTER_LURKER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_GRAVEDIGGER].health = 160;
	MonsterData[MONSTER_GRAVEDIGGER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DEVOURER].health = 175;
	MonsterData[MONSTER_DEVOURER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_NIGHTMAREDEMON].health = 150;
	MonsterData[MONSTER_NIGHTMAREDEMON].flags = DND_MTYPE_DEMON_POW;

	// imp
	MonsterData[MONSTER_DARKIMP1].health = 80;
	MonsterData[MONSTER_DARKIMP1].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_VOIDDARKIMP].health = 100;
	MonsterData[MONSTER_VOIDDARKIMP].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_NETHERDARKIMP].health = 100;
	MonsterData[MONSTER_NETHERDARKIMP].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DARKIMP2].health = 90;
	MonsterData[MONSTER_DARKIMP2].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_IMPABOM].health = 80;
	MonsterData[MONSTER_IMPABOM].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_STIMP].health = 55;
	MonsterData[MONSTER_STIMP].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_VOIDIMP].health = 65;
	MonsterData[MONSTER_VOIDIMP].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SOULHARVESTER].health = 100;
	MonsterData[MONSTER_SOULHARVESTER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_PYROIMP].health = 60;
	MonsterData[MONSTER_PYROIMP].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DEVIL].health = 85;
	MonsterData[MONSTER_DEVIL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DEVIL2].health = 125;
	MonsterData[MONSTER_DEVIL2].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_VULGAR].health = 70;
	MonsterData[MONSTER_VULGAR].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_UNDEADMAGE].health = 80;
	MonsterData[MONSTER_UNDEADMAGE].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_SHADOW].health = 85;
	MonsterData[MONSTER_SHADOW].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_REAVER].health = 90;
	MonsterData[MONSTER_REAVER].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_ROACH].health = 80;
	MonsterData[MONSTER_ROACH].flags = DND_MTYPE_DEMON_POW;

	// lost soul
	MonsterData[MONSTER_BABYCACO].health = 60;
	MonsterData[MONSTER_BABYCACO].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_ETHEREALSOUL].health = 60;
	MonsterData[MONSTER_ETHEREALSOUL].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_FLESHSPAWN].health = 50;
	MonsterData[MONSTER_FLESHSPAWN].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_GUARDIANCUBE].health = 75;
	MonsterData[MONSTER_GUARDIANCUBE].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_FORGOTTENONE].health = 40;
	MonsterData[MONSTER_FORGOTTENONE].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_HADESSPHERE].health = 35;
	MonsterData[MONSTER_HADESSPHERE].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_WATCHER].health = 100;
	MonsterData[MONSTER_WATCHER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DARKLICH_SPIRIT].health = 75;
	MonsterData[MONSTER_DARKLICH_SPIRIT].flags = DND_MTYPE_UNDEAD_POW;

	// cacodemon
	MonsterData[MONSTER_WEAKENER].health = 300;
	MonsterData[MONSTER_WEAKENER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SHADOWPRIEST].health = 250;
	MonsterData[MONSTER_SHADOWPRIEST].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_GRELL].health = 325;
	MonsterData[MONSTER_GRELL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DEATHWHISPERER].health = 500;
	MonsterData[MONSTER_DEATHWHISPERER].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_CACOLICH].health = 500;
	MonsterData[MONSTER_CACOLICH].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_INFERNO].health = 400;
	MonsterData[MONSTER_INFERNO].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_GUARDIAN].health = 275;
	MonsterData[MONSTER_GUARDIAN].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_ENHANCEDCACO].health = 300;
	MonsterData[MONSTER_ENHANCEDCACO].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_EARTHLICH].health = 425;
	MonsterData[MONSTER_EARTHLICH].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_WICKED].health = 300;
	MonsterData[MONSTER_WICKED].flags = DND_MTYPE_DEMON_POW;

	// pain ele
	MonsterData[MONSTER_BLOODLICH].health = 675;
	MonsterData[MONSTER_BLOODLICH].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_HADESELEMENTAL].health = 800;
	MonsterData[MONSTER_HADESELEMENTAL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_HELLARBITER].health = 700;
	MonsterData[MONSTER_HELLARBITER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DEFILER].health = 500;
	MonsterData[MONSTER_DEFILER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_TORTUREDSOUL].health = 550;
	MonsterData[MONSTER_TORTUREDSOUL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SHADOWDISCIPLE].health = 500;
	MonsterData[MONSTER_SHADOWDISCIPLE].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_SENTINEL].health = 750;
	MonsterData[MONSTER_SENTINEL].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_PHANTASM].health = 300;
	MonsterData[MONSTER_PHANTASM].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_WRAITH].health = 75;
	MonsterData[MONSTER_WRAITH].flags = DND_MTYPE_UNDEAD_POW;
	
	// revenant
	MonsterData[MONSTER_INCARNATE].health = 250;
	MonsterData[MONSTER_INCARNATE].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_BEAMREVENANT].health = 250;
	MonsterData[MONSTER_BEAMREVENANT].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_AXEKNIGHT].health = 250;
	MonsterData[MONSTER_AXEKNIGHT].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_WIDOWMAKER].health = 300;
	MonsterData[MONSTER_WIDOWMAKER].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_SLUDGEGIANT].health = 280;
	MonsterData[MONSTER_SLUDGEGIANT].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_SLUDGEGIANT2].health = 140;
	MonsterData[MONSTER_SLUDGEGIANT2].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_SLUDGEGIANT3].health = 70;
	MonsterData[MONSTER_SLUDGEGIANT3].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_CADAVER].health = 350;
	MonsterData[MONSTER_CADAVER].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_DARKSERVANT].health = 275;
	MonsterData[MONSTER_DARKSERVANT].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_CRAWLER].health = 325;
	MonsterData[MONSTER_CRAWLER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_CYBORGSOLDIER].health = 200;
	MonsterData[MONSTER_CYBORGSOLDIER].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_DRAUGR].health = 350;
	MonsterData[MONSTER_DRAUGR].flags = DND_MTYPE_UNDEAD_POW;

	// hell knight
	MonsterData[MONSTER_BLOODSATYR].health = 550;
	MonsterData[MONSTER_BLOODSATYR].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_HELLWARRIOR].health = 400;
	MonsterData[MONSTER_HELLWARRIOR].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_HELLSFURY].health = 600;
	MonsterData[MONSTER_HELLSFURY].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_BLACKKNIGHT].health = 550;
	MonsterData[MONSTER_BLACKKNIGHT].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_ARCHON].health = 700;
	MonsterData[MONSTER_ARCHON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_WARLORD].health = 650;
	MonsterData[MONSTER_WARLORD].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SKULLWIZARD].health = 500;
	MonsterData[MONSTER_SKULLWIZARD].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_CYBORGWARRIOR].health = 650;
	MonsterData[MONSTER_CYBORGWARRIOR].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_SHADOWBEAST].health = 400;
	MonsterData[MONSTER_SHADOWBEAST].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_CHAOSSERPENT].health = 450;
	MonsterData[MONSTER_CHAOSSERPENT].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_MOONSATYR].health = 750;
	MonsterData[MONSTER_MOONSATYR].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_ICEGOLEM].health = 400;
	MonsterData[MONSTER_ICEGOLEM].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_PUTREFIER].health = 500;
	MonsterData[MONSTER_PUTREFIER].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_GLADIATOR].health = 450;
	MonsterData[MONSTER_GLADIATOR].flags = DND_MTYPE_DEMON_POW;

	// baron
	MonsterData[MONSTER_LAVADEMON].health = 1000;
	MonsterData[MONSTER_LAVADEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_LORDOFHERESY].health = 1100;
	MonsterData[MONSTER_LORDOFHERESY].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_BORMERETH].health = 1200;
	MonsterData[MONSTER_BORMERETH].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_BARBATOS].health = 1100;
	MonsterData[MONSTER_BARBATOS].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_BLOODSEEKER].health = 1000;
	MonsterData[MONSTER_BLOODSEEKER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_SHADOWWIZARD].health = 900;
	MonsterData[MONSTER_SHADOWWIZARD].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_KJAROCH].health = 1650;
	MonsterData[MONSTER_KJAROCH].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_CYBRUISER].health = 1500;
	MonsterData[MONSTER_CYBRUISER].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_BRUISERDEMON].health = 1500;
	MonsterData[MONSTER_BRUISERDEMON].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_MAGMASERPENT].health = 900;
	MonsterData[MONSTER_MAGMASERPENT].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DREADKNIGHT].health = 900;
	MonsterData[MONSTER_DREADKNIGHT].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_MAGMAGOLEM].health = 900;
	MonsterData[MONSTER_MAGMAGOLEM].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_JUDICATOR].health = 800;
	MonsterData[MONSTER_JUDICATOR].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_WARMASTER].health = 1250;
	MonsterData[MONSTER_WARMASTER].flags = DND_MTYPE_DEMON_POW;

	// fatso
	MonsterData[MONSTER_CORPULENT].health = 850;
	MonsterData[MONSTER_CORPULENT].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DAEDABUS].health = 850;
	MonsterData[MONSTER_DAEDABUS].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_PALADIN].health = 775;
	MonsterData[MONSTER_PALADIN].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_GAMON].health = 875;
	MonsterData[MONSTER_GAMON].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_MEPHISTO].health = 900;
	MonsterData[MONSTER_MEPHISTO].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_MAFIBUS].health = 1100;
	MonsterData[MONSTER_MAFIBUS].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_ICEFATSO].health = 650;
	MonsterData[MONSTER_ICEFATSO].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_ABOMINATION].health = 775;
	MonsterData[MONSTER_ABOMINATION].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_REDEEMER].health = 800;
	MonsterData[MONSTER_REDEEMER].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_GOLDGOLEM].health = 1000;
	MonsterData[MONSTER_GOLDGOLEM].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_YETI].health = 800;
	MonsterData[MONSTER_YETI].flags = DND_MTYPE_MAGICAL_POW;

	// arachnotron
	MonsterData[MONSTER_FUSIONSPIDER].health = 500;
	MonsterData[MONSTER_FUSIONSPIDER].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_RAILARACHNOTRON].health = 450;
	MonsterData[MONSTER_RAILARACHNOTRON].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_HELLFORGESPIDER].health = 700;
	MonsterData[MONSTER_HELLFORGESPIDER].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_VORE].health = 700;
	MonsterData[MONSTER_VORE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_BABYDEMOLISHER].health = 700;
	MonsterData[MONSTER_BABYDEMOLISHER].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_CHAINGUNGENERAL].health = 550;
	MonsterData[MONSTER_CHAINGUNGENERAL].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_CHAINGUNCOMMANDO].health = 700;
	MonsterData[MONSTER_CHAINGUNCOMMANDO].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_LEGIONNAIRE].health = 600;
	MonsterData[MONSTER_LEGIONNAIRE].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_MANTICORE].health = 650;
	MonsterData[MONSTER_MANTICORE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_OPHIDIAN].health = 700;
	MonsterData[MONSTER_OPHIDIAN].flags = DND_MTYPE_MAGICAL_POW;

	// vile
	MonsterData[MONSTER_DIABLOIST].health = 850;
	MonsterData[MONSTER_DIABLOIST].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_UNDEADPRIEST].health = 800;
	MonsterData[MONSTER_UNDEADPRIEST].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_UNDEADPRIESTGHOST].health = 100;
	MonsterData[MONSTER_UNDEADPRIESTGHOST].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_DEATHVILE].health = 800;
	MonsterData[MONSTER_DEATHVILE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_HIEROPHANT].health = 1250;
	MonsterData[MONSTER_HIEROPHANT].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_GURU].health = 800;
	MonsterData[MONSTER_GURU].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_DEATHKNIGHT].health = 900;
	MonsterData[MONSTER_DEATHKNIGHT].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_HORSHACKER].health = 900;
	MonsterData[MONSTER_HORSHACKER].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_DARKZEALOT].health = 750;
	MonsterData[MONSTER_DARKZEALOT].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_FLESHWIZARD].health = 625;
	MonsterData[MONSTER_FLESHWIZARD].flags = DND_MTYPE_DEMON_POW;

	// sm
	MonsterData[MONSTER_DEMOLISHER].health = 4000;
	MonsterData[MONSTER_DEMOLISHER].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_ARACHNOPHYTE].health = 3500;
	MonsterData[MONSTER_ARACHNOPHYTE].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_PSIONICQUEEN].health = 4000;
	MonsterData[MONSTER_PSIONICQUEEN].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_ANGELOFDEATH].health = 4000;
	MonsterData[MONSTER_ANGELOFDEATH].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_GOLDLICH].health = 4750;
	MonsterData[MONSTER_GOLDLICH].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_GOLDLICHFAKE].health = 300;
	MonsterData[MONSTER_GOLDLICHFAKE].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_IRONLICH].health = 4000;
	MonsterData[MONSTER_IRONLICH].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_SPIDEROVERLORD].health = 4000;
	MonsterData[MONSTER_SPIDEROVERLORD].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_DARKLICH].health = 4000;
	MonsterData[MONSTER_DARKLICH].flags = DND_MTYPE_MAGICAL_POW;

	// cyber
	MonsterData[MONSTER_CARDINAL].health = 4500;
	MonsterData[MONSTER_CARDINAL].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_TERMINATOR].health = 5750;
	MonsterData[MONSTER_TERMINATOR].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_THAMUZ].health = 4750;
	MonsterData[MONSTER_THAMUZ].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_AZAZEL].health = 5250;
	MonsterData[MONSTER_AZAZEL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_HELLSMITH].health = 6000;
	MonsterData[MONSTER_HELLSMITH].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_THANATOS].health = 6500;
	MonsterData[MONSTER_THANATOS].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_AVATAR].health = 5500;
	MonsterData[MONSTER_AVATAR].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_CERBERUS].health = 6000;
	MonsterData[MONSTER_CERBERUS].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_DEATHWYVERN].health = 3500;
	MonsterData[MONSTER_DEATHWYVERN].flags = DND_MTYPE_UNDEAD_POW;

	// legendaries
	MonsterData[MONSTER_DREAMINGGOD].health = 20000;
	MonsterData[MONSTER_DREAMINGGOD].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_TORRASQUE].health = 16500;
	MonsterData[MONSTER_TORRASQUE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_MORDECQAI].health = 18500;
	MonsterData[MONSTER_MORDECQAI].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_GODSLAYER].health = 13500;
	MonsterData[MONSTER_GODSLAYER].flags = DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_GOLGOTH].health = 17500;
	MonsterData[MONSTER_GOLGOTH].flags = DND_MTYPE_DEMON_POW;

	// uniques
	MonsterData[MONSTER_TERON].health = 2000;
	MonsterData[MONSTER_TERON].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_GANT].health = 2800;
	MonsterData[MONSTER_GANT].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_BRONN].health = 4000;
	MonsterData[MONSTER_BRONN].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_VAAJ].health = 3600;
	MonsterData[MONSTER_VAAJ].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_REMUS].health = 6000;
	MonsterData[MONSTER_REMUS].flags = DND_MTYPE_UNDEAD_POW | DND_MTYPE_ZOMBIE_POW;
	MonsterData[MONSTER_SSRATH].health = 4500;
	MonsterData[MONSTER_SSRATH].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_HOLLOWSHELL].health = 2000;
	MonsterData[MONSTER_HOLLOWSHELL].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_OMNISIGHT].health = 6400;
	MonsterData[MONSTER_OMNISIGHT].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_CHEGOVAX].health = 4800;
	MonsterData[MONSTER_CHEGOVAX].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_ONIMUZ].health = 3750;
	MonsterData[MONSTER_ONIMUZ].flags = DND_MTYPE_UNDEAD_POW;
	MonsterData[MONSTER_HARKIMONDE].health = 6000;
	MonsterData[MONSTER_HARKIMONDE].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_LESHRAC].health = 7000;
	MonsterData[MONSTER_LESHRAC].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_LESHRACPOD].health = 200;
	MonsterData[MONSTER_LESHRACPOD].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_KRULL].health = 7000;
	MonsterData[MONSTER_KRULL].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_THORAX].health = 7000;
	MonsterData[MONSTER_THORAX].flags = DND_MTYPE_DEMON_POW | DND_MTYPE_ROBOTIC_POW;
	MonsterData[MONSTER_ZRAVOG].health = 6500;
	MonsterData[MONSTER_ZRAVOG].flags = DND_MTYPE_MAGICAL_POW;
	MonsterData[MONSTER_ERYXIA].health = 12000;
	MonsterData[MONSTER_ERYXIA].flags = DND_MTYPE_DEMON_POW;
	MonsterData[MONSTER_ABAXOTH].health = 15000;
	MonsterData[MONSTER_ABAXOTH].flags = DND_MTYPE_DEMON_POW;

	// classics
	MonsterData[MONSTER_LOSTSOUL].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_PAINELEMENTAL].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_PAINELEMENTAL].trait_list[DND_SPLIT] = true;
	MonsterData[MONSTER_VILE].trait_list[DND_RESURRECT] = true;
	
	// zombieman
	MonsterData[MONSTER_ZOMBIEHUNTER].trait_list[DND_MOBILITY] = true;
	
	// shotgunner
	MonsterData[MONSTER_SGLOS].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_SSGLOS].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_ROGUE].trait_list[DND_MOBILITY] = true;
	
	// chaingunner
	MonsterData[MONSTER_INITIATE].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_MRROBOT].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_MRROBOT].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_BERSERKERGUY].trait_list[DND_RAGE] = true;
	
	// demon
	MonsterData[MONSTER_STONEIMP].trait_list[DND_STONECREATURE] = true;
	MonsterData[MONSTER_STONEDEMON].trait_list[DND_STONECREATURE] = true;
	MonsterData[MONSTER_STONEDEMON].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_BRUTY].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_SCAVENGER].trait_list[DND_RESURRECT] = true;
	MonsterData[MONSTER_NHUMCIGN].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_NHUMCIGN].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_EARTHGOLEM].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_EARTHGOLEM].trait_list[DND_EARTHCREATURE] = true;
	MonsterData[MONSTER_PUREBREDDEMON].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_SABRECLAW].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_SABRECLAW].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_SABRECLAW].trait_list[DND_BULLET_RESIST] = true;
	
	// spectre
	MonsterData[MONSTER_NIGHTMAREDEMON].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_GRAVEDIGGER].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_DEVOURER].trait_list[DND_MOBILITY] = true;
	
	// imp
	MonsterData[MONSTER_DARKIMP2].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_SOULHARVESTER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_PYROIMP].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_VULGAR].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_UNDEADMAGE].trait_list[DND_RESURRECT] = true;
	MonsterData[MONSTER_REAVER].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_REAVER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_ROACH].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_ROACH].trait_list[DND_HEAL] = true;
	
	// lost soul
	MonsterData[MONSTER_FLESHSPAWN].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_ETHEREALSOUL].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_ETHEREALSOUL].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_GUARDIANCUBE].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_FORGOTTENONE].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_FORGOTTENONE].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_WATCHER].trait_list[DND_PIERCE] = true;
	
	// caco
	MonsterData[MONSTER_CACOLICH].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_DEATHWHISPERER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_DEATHWHISPERER].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_DEATHWHISPERER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_EARTHLICH].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_EARTHLICH].trait_list[DND_EARTHCREATURE] = true;
	MonsterData[MONSTER_INFERNO].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_INFERNO].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_GRELL].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_SHADOWPRIEST].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_SHADOWPRIEST].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_GUARDIAN].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_WICKED].trait_list[DND_MAGIC_RESIST] = true;
	MonsterData[MONSTER_WICKED].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_WICKED].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_WICKED].trait_list[DND_HOMING] = true;
	
	// pain
	MonsterData[MONSTER_TORTUREDSOUL].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_TORTUREDSOUL].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_DEFILER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_DEFILER].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_HADESELEMENTAL].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_HADESELEMENTAL].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_HADESELEMENTAL].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_HADESSPHERE].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_SENTINEL].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_SENTINEL].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_BLOODLICH].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_BLOODLICH].trait_list[DND_CURSE] = true;
	MonsterData[MONSTER_BLOODLICH].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_WRAITH].trait_list[DND_RESURRECT] = true;
	MonsterData[MONSTER_SHADOWDISCIPLE].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_SHADOWDISCIPLE].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_SHADOWDISCIPLE].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_HELLARBITER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_HELLARBITER].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_HELLARBITER].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_PHANTASM].trait_list[DND_SUMMONED] = true;
	MonsterData[MONSTER_PHANTASM].trait_list[DND_ETHEREAL] = true;
	
	// rev
	MonsterData[MONSTER_INCARNATE].trait_list[DND_RAISE] = true;
	MonsterData[MONSTER_CADAVER].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_CADAVER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_WIDOWMAKER].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_WIDOWMAKER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_AXEKNIGHT].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_YETI].trait_list[DND_ICECREATURE] = true;
	MonsterData[MONSTER_YETI].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_SLUDGEGIANT].trait_list[DND_SPLIT] = true;
	MonsterData[MONSTER_SLUDGEGIANT].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_SLUDGEGIANT2].trait_list[DND_SPLIT] = true;
	MonsterData[MONSTER_SLUDGEGIANT2].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_SLUDGEGIANT2].trait_list[DND_SUMMONED] = true;
	MonsterData[MONSTER_SLUDGEGIANT3].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_SLUDGEGIANT3].trait_list[DND_SUMMONED] = true;
	MonsterData[MONSTER_DARKSERVANT].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_DARKSERVANT].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_DARKSERVANT].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_CRAWLER].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_CRAWLER].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_CYBORGSOLDIER].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_CYBORGSOLDIER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_DRAUGR].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_DRAUGR].trait_list[DND_MOBILITY] = true;
	
	// hk
	MonsterData[MONSTER_BLOODSATYR].trait_list[DND_VAMPIRISM] = true;
	MonsterData[MONSTER_HELLWARRIOR].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_HELLSFURY].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_WARLORD].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_SHADOWBEAST].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_BLACKKNIGHT].trait_list[DND_ICE_WEAKNESS] = true;
	MonsterData[MONSTER_BLACKKNIGHT].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_BLACKKNIGHT].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_SKULLWIZARD].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_SKULLWIZARD].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_CYBORGWARRIOR].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_CYBORGWARRIOR].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_MOONSATYR].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_MOONSATYR].trait_list[DND_ENERGY_RESIST] = true;
	MonsterData[MONSTER_ICEGOLEM].trait_list[DND_HEAL] = true;
	MonsterData[MONSTER_ICEGOLEM].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_ICEGOLEM].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	MonsterData[MONSTER_PUTREFIER].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	MonsterData[MONSTER_PUTREFIER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_PUTREFIER].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_PUTREFIER].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_GLADIATOR].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_GLADIATOR].trait_list[DND_ICE_WEAKNESS] = true;
	
	// baron
	MonsterData[MONSTER_LAVADEMON].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_LAVADEMON].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_CYBRUISER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_BRUISERDEMON].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_BORMERETH].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_BORMERETH].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_BORMERETH].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_BLOODSEEKER].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_BARBATOS].trait_list[DND_ELEMENTAL_WEAKNESS] = true;
	MonsterData[MONSTER_SHADOWWIZARD].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_SHADOWWIZARD].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_KJAROCH].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_KJAROCH].trait_list[DND_EARTHCREATURE] = true;
	MonsterData[MONSTER_MAGMASERPENT].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_DREADKNIGHT].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_DREADKNIGHT].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_DREADKNIGHT].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_DREADKNIGHT].trait_list[DND_MAGIC_RESIST] = true;
	MonsterData[MONSTER_MAGMAGOLEM].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_MAGMAGOLEM].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_JUDICATOR].trait_list[DND_BULLET_IMMUNE] = true;
	MonsterData[MONSTER_WARMASTER].trait_list[DND_HEAL] = true;
	MonsterData[MONSTER_WARMASTER].trait_list[DND_RAGE] = true;
	
	// fatty
	MonsterData[MONSTER_CORPULENT].trait_list[DND_ELEMENTAL_WEAKNESS] = true;
	MonsterData[MONSTER_DAEDABUS].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_ICEFATSO].trait_list[DND_ICECREATURE] = true;
	MonsterData[MONSTER_MAFIBUS].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_PALADIN].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_PALADIN].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_PALADIN].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_PALADIN].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_GAMON].trait_list[DND_ENERGY_RESIST] = true;
	MonsterData[MONSTER_GAMON].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_GAMON].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_ABOMINATION].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_REDEEMER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_REDEEMER].trait_list[DND_DEATH] = true;
	MonsterData[MONSTER_GOLDGOLEM].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_GOLDGOLEM].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	MonsterData[MONSTER_GOLDGOLEM].trait_list[DND_MAGIC_RESIST] = true;
	
	// arachno
	MonsterData[MONSTER_BABYDEMOLISHER].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_VORE].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_VORE].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_CHAINGUNCOMMANDO].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_LEGIONNAIRE].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_OPHIDIAN].trait_list[DND_FIRECREATURE] = true;
	
	// vile
	MonsterData[MONSTER_DIABLOIST].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_DEATHVILE].trait_list[DND_RESURRECT] = true;
	MonsterData[MONSTER_DEATHVILE].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_HIEROPHANT].trait_list[DND_CURSE] = true;
	MonsterData[MONSTER_HIEROPHANT].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_HIEROPHANT].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_HIEROPHANT].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_DEATHKNIGHT].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_DEATHKNIGHT].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_HORSHACKER].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_HORSHACKER].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_UNDEADPRIEST].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_UNDEADPRIEST].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_UNDEADPRIEST].trait_list[DND_RESURRECT] = true;
	MonsterData[MONSTER_GURU].trait_list[DND_CURSE] = true;
	MonsterData[MONSTER_GURU].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_DARKZEALOT].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_DARKZEALOT].trait_list[DND_HEAL] = true;
	MonsterData[MONSTER_DARKZEALOT].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_FLESHWIZARD].trait_list[DND_MAGIC_IMMUNE] = true;
	MonsterData[MONSTER_FLESHWIZARD].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_FLESHWIZARD].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_FLESHWIZARD].trait_list[DND_DEATH] = true;
	
	// sm
	MonsterData[MONSTER_PSIONICQUEEN].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_PSIONICQUEEN].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_PSIONICQUEEN].trait_list[DND_ENERGY_IMMUNE] = true;
	MonsterData[MONSTER_ANGELOFDEATH].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	MonsterData[MONSTER_ANGELOFDEATH].trait_list[DND_MAGIC_RESIST] = true;
	MonsterData[MONSTER_GOLDLICH].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_GOLDLICH].trait_list[DND_SPLIT] = true;
	MonsterData[MONSTER_GOLDLICH].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_IRONLICH].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_IRONLICH].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_IRONLICH].trait_list[DND_CURSE] = true;
	MonsterData[MONSTER_IRONLICH].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_IRONLICH].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_SPIDEROVERLORD].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_SPIDEROVERLORD].trait_list[DND_ENERGY_IMMUNE] = true;
	MonsterData[MONSTER_SPIDEROVERLORD].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_DARKLICH].trait_list[DND_MAGIC_IMMUNE] = true;
	MonsterData[MONSTER_DARKLICH].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_DARKLICH].trait_list[DND_HOMING] = true;
	
	// cyber
	MonsterData[MONSTER_CARDINAL].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_TERMINATOR].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_TERMINATOR].trait_list[DND_ENERGY_RESIST] = true;
	MonsterData[MONSTER_THAMUZ].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_THAMUZ].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_THAMUZ].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_AZAZEL].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_AZAZEL].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_AZAZEL].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_AZAZEL].trait_list[DND_MAGIC_IMMUNE] = true;
	MonsterData[MONSTER_HELLSMITH].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_HELLSMITH].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_AVATAR].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	MonsterData[MONSTER_AVATAR].trait_list[DND_MAGIC_RESIST] = true;
	MonsterData[MONSTER_AVATAR].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_AVATAR].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_THANATOS].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_CERBERUS].trait_list[DND_MAGIC_IMMUNE] = true;
	MonsterData[MONSTER_CERBERUS].trait_list[DND_RAGE] = true;
	MonsterData[MONSTER_DEATHWYVERN].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_DEATHWYVERN].trait_list[DND_MOBILITY] = true;
	
	// legendary monsters
	MonsterData[MONSTER_DREAMINGGOD].trait_list[DND_MAGIC_IMMUNE] = true;
	MonsterData[MONSTER_DREAMINGGOD].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	MonsterData[MONSTER_DREAMINGGOD].trait_list[DND_LEGENDARY] = true;
	
	MonsterData[MONSTER_TORRASQUE].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_TORRASQUE].trait_list[DND_ENERGY_RESIST] = true;
	MonsterData[MONSTER_TORRASQUE].trait_list[DND_LEGENDARY] = true;
	
	MonsterData[MONSTER_MORDECQAI].trait_list[DND_FIRECREATURE] = true;
	MonsterData[MONSTER_MORDECQAI].trait_list[DND_LEGENDARY] = true;
	
	MonsterData[MONSTER_GODSLAYER].trait_list[DND_BULLET_IMMUNE] = true;
	MonsterData[MONSTER_GODSLAYER].trait_list[DND_ENERGY_RESIST] = true;
	MonsterData[MONSTER_GODSLAYER].trait_list[DND_LEGENDARY] = true;
	
	MonsterData[MONSTER_GOLGOTH].trait_list[DND_MAGIC_RESIST] = true;
	MonsterData[MONSTER_GOLGOTH].trait_list[DND_LEGENDARY] = true;
	
	// unique monsters
	MonsterData[MONSTER_TERON].trait_list[DND_FORTIFIED] = true;
	
	MonsterData[MONSTER_GANT].trait_list[DND_FORTIFIED] = true;
	
	MonsterData[MONSTER_BRONN].trait_list[DND_FORTIFIED] = true;
	
	MonsterData[MONSTER_VAAJ].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_VAAJ].trait_list[DND_ARMORPEN] = true;
	MonsterData[MONSTER_VAAJ].trait_list[DND_MAGIC_IMMUNE] = true;
	
	MonsterData[MONSTER_REMUS].trait_list[DND_TELEPORT] = true;
	
	MonsterData[MONSTER_SSRATH].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_SSRATH].trait_list[DND_CURSE] = true;
	
	MonsterData[MONSTER_OMNISIGHT].trait_list[DND_TELEPORT] = true;
	MonsterData[MONSTER_OMNISIGHT].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_OMNISIGHT].trait_list[DND_MAGIC_RESIST] = true;
	MonsterData[MONSTER_OMNISIGHT].trait_list[DND_ELEMENTAL_RESIST] = true;
	
	MonsterData[MONSTER_CHEGOVAX].trait_list[DND_FORTIFIED] = true;
	MonsterData[MONSTER_CHEGOVAX].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_CHEGOVAX].trait_list[DND_BULLET_IMMUNE] = true;
	MonsterData[MONSTER_CHEGOVAX].trait_list[DND_MAGIC_IMMUNE] = true;
	MonsterData[MONSTER_CHEGOVAX].trait_list[DND_MOLTENBLOOD] = true;
	
	MonsterData[MONSTER_ONIMUZ].trait_list[DND_BULLET_RESIST] = true;
	MonsterData[MONSTER_ONIMUZ].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_ONIMUZ].trait_list[DND_MAGIC_IMMUNE] = true;
	
	MonsterData[MONSTER_HARKIMONDE].trait_list[DND_BLOCK] = true;
	MonsterData[MONSTER_HARKIMONDE].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_HARKIMONDE].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_HARKIMONDE].trait_list[DND_MAGIC_RESIST] = true;
	
	MonsterData[MONSTER_LESHRAC].trait_list[DND_POISON] = true;
	MonsterData[MONSTER_LESHRAC].trait_list[DND_MINIONS] = true;
	MonsterData[MONSTER_LESHRAC].trait_list[DND_VENOMANCER] = true;
	
	MonsterData[MONSTER_KRULL].trait_list[DND_FORTIFIED] = true;
	MonsterData[MONSTER_KRULL].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_KRULL].trait_list[DND_MOLTENBLOOD] = true;
	
	MonsterData[MONSTER_THORAX].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_THORAX].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_THORAX].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	
	MonsterData[MONSTER_ZRAVOG].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_ZRAVOG].trait_list[DND_CURSE] = true;
	MonsterData[MONSTER_ZRAVOG].trait_list[DND_MAGIC_IMMUNE] = true;
	
	MonsterData[MONSTER_ERYXIA].trait_list[DND_CURSE] = true;
	MonsterData[MONSTER_ERYXIA].trait_list[DND_FORTIFIED] = true;
	MonsterData[MONSTER_ERYXIA].trait_list[DND_MAGIC_RESIST] = true;
	MonsterData[MONSTER_ERYXIA].trait_list[DND_ELEMENTAL_IMMUNE] = true;
	MonsterData[MONSTER_ERYXIA].trait_list[DND_FROSTBLOOD] = true;
	
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_HOMING] = true;
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_FORTIFIED] = true;
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_PIERCE] = true;
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_MOBILITY] = true;
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_AGGRESSIVE] = true;
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_ELEMENTAL_RESIST] = true;
	MonsterData[MONSTER_ABAXOTH].trait_list[DND_MAGIC_IMMUNE] = true;
}

void SetupIncursionMonsters() {
	// Chex
	MonsterData[MONSTER_CHEX_COMMON].health = 100;
	MonsterData[MONSTER_CHEX_COMMON].flags = DND_MTYPE_FLEMOID_POW;

	MonsterData[MONSTER_CHEX_BIPEDAL].health = 200;
	MonsterData[MONSTER_CHEX_BIPEDAL].flags = DND_MTYPE_FLEMOID_POW;

	MonsterData[MONSTER_CHEX_ARMORED].health = 300;
	MonsterData[MONSTER_CHEX_ARMORED].flags = DND_MTYPE_FLEMOID_POW;

	MonsterData[MONSTER_CHEX_HEAVYARMORED].health = 450;
	MonsterData[MONSTER_CHEX_HEAVYARMORED].flags = DND_MTYPE_FLEMOID_POW;
	MonsterData[MONSTER_CHEX_HEAVYARMORED].trait_list[DND_BULLET_RESIST] = true;

	MonsterData[MONSTER_CHEX_CYCLOPS].health = 330;
	MonsterData[MONSTER_CHEX_CYCLOPS].flags = DND_MTYPE_FLEMOID_POW;

	MonsterData[MONSTER_CHEX_FLEMMINE].health = 180;
	MonsterData[MONSTER_CHEX_FLEMMINE].flags = DND_MTYPE_FLEMOID_POW;

	MonsterData[MONSTER_CHEX_LARVA].health = 400;
	MonsterData[MONSTER_CHEX_LARVA].flags = DND_MTYPE_FLEMOID_POW;

	MonsterData[MONSTER_CHEX_QUADWUMPUS].health = 600;
	MonsterData[MONSTER_CHEX_QUADWUMPUS].flags = DND_MTYPE_FLEMOID_POW;

	MonsterData[MONSTER_CHEX_STRIDICUS].health = 750;
	MonsterData[MONSTER_CHEX_STRIDICUS].flags = DND_MTYPE_FLEMOID_POW;
	
	MonsterData[MONSTER_CHEX_SUPERCYCLOPS].health = 900;
	MonsterData[MONSTER_CHEX_SUPERCYCLOPS].flags = DND_MTYPE_FLEMOID_POW;
	MonsterData[MONSTER_CHEX_SUPERCYCLOPS].trait_list[DND_MOBILITY] = true;

	MonsterData[MONSTER_CHEX_MAXIMUS].health = 2500;
	MonsterData[MONSTER_CHEX_MAXIMUS].flags = DND_MTYPE_FLEMOID_POW;

	MonsterData[MONSTER_CHEX_FLEMOBIUS].health = 2000;
	MonsterData[MONSTER_CHEX_FLEMOBIUS].flags = DND_MTYPE_FLEMOID_POW;

	MonsterData[MONSTER_CHEX_BERNABE].health = 1750;
	MonsterData[MONSTER_CHEX_BERNABE].flags = DND_MTYPE_FLEMOID_POW;
	MonsterData[MONSTER_CHEX_BERNABE].trait_list[DND_FORTIFIED] = true;

	MonsterData[MONSTER_CHEX_METEOR].health = 600;
	MonsterData[MONSTER_CHEX_METEOR].flags = DND_MTYPE_FLEMOID_POW;
	MonsterData[MONSTER_CHEX_METEOR].trait_list[DND_SUMMONED] = true;

	MonsterData[MONSTER_CHEX_FLEMCARRIER].health = 500;
	MonsterData[MONSTER_CHEX_FLEMCARRIER].flags = DND_MTYPE_FLEMOID_POW;
	MonsterData[MONSTER_CHEX_FLEMCARRIER].trait_list[DND_SUMMONED] = true;
	
	MonsterData[MONSTER_CHEX_FLEMBOMINATION].health = 4500;
	MonsterData[MONSTER_CHEX_FLEMBOMINATION].flags = DND_MTYPE_FLEMOID_POW;

	MonsterData[MONSTER_CHEX_SNOTFOLUS].health = 6000;
	MonsterData[MONSTER_CHEX_SNOTFOLUS].flags = DND_MTYPE_FLEMOID_POW;
}

void SetupMonsterWeights() {
	// zombieman
	int id = 0;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ZOMBIEMAN][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_SHOTGUNGUY][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CHAINGUNGUY][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_DEMON][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_SPECTRE][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_IMP][id++] = DND_MWEIGHT_RARE1;

	id = 0;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_CACODEMON][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_PAINELEMENTAL][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_LOSTSOUL][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_REVENANT][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_HELLKNIGHT][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_BARON][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_FATSO][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ARACHNOTRON][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_ARCHVILE][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_RARE1;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_SPIDERMASTERMIND][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_UNCOMMON;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_RARE2;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_EPIC;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_VERYRARE;
	Monster_Weights[MONSTERCLASS_CYBERDEMON][id++] = DND_MWEIGHT_ENDMARKER;

	id = 0;
	Monster_Weights[MONSTERCLASS_WOLFENSS][id++] = DND_MWEIGHT_COMMON;
	Monster_Weights[MONSTERCLASS_WOLFENSS][id++] = DND_MWEIGHT_ENDMARKER;
}

Script "DnD Setup Monster Data" OPEN {
	if(!isSetupComplete(SETUP_STATE1, SETUP_MONSTERS)) {
		SetupMonsterData();
		Delay(const:15);
		SetupMonsterWeights();
		SetupComplete(SETUP_STATE1, SETUP_MONSTERS);
		Delay(const:10);
		SetupIncursionMonsters();
	}
}

Script "DnD Setup Monster Data CS" OPEN CLIENTSIDE {
	if(!isSetupComplete(SETUP_STATE1, SETUP_MONSTERS)) {
		Delay(const:20);
		SetupMonsterData();
		SetupComplete(SETUP_STATE1, SETUP_MONSTERS);
		Delay(const:10);
		SetupIncursionMonsters();
	}
}

enum {
	DND_NO_TRANSLATION = 70,
	DND_CRIT_TRANSLATION,
	DND_RESIST_TRANSLATION,

	DND_EXECUTE_TRANSLATION = 75,

	DND_QUAKEZOMBIESG_TRANSLATION,

	DND_SMGGUY_TRANSLATION,
	DND_QUAKEZOMBIECG_TRANSLATION,

	DND_NETHERDARKIMP_TRANSLATION,

	DND_FLAMEDEMON_TRANSLATION,
	DND_SCAVENGER_TRANSLATION,
	DND_NHUMCIGN_TRANSLATION,
	DND_NIGHTMARESPECTRE_TRANSLATION,
	DND_GRAVEDIGGER_TRANSLATION,
	DND_DEVOURER_TRANSLATION,

	DND_ENHANCEDCACO_TRANSLATION,

	DND_DEFILER_TRANSLATION,

	DND_CADAVER_TRANSLATION,

	DND_ICEFATSO_TRANSLATION,

	DND_DREADKNIGHT_TRANSLATION,
	DND_BLOODSATYR_TRANSLATION,
	DND_MOONSATYR_TRANSLATION,
	DND_WARLORDHELL_TRANSLATION,
	DND_LORDHERESY_TRANSLATION,
	DND_BARBATOS_TRANSLATION,
	DND_KJAROCH_TRANSLATION,

	DND_HORSHACKER_TRANSLATION,
	DND_BERNABE_TRANSLATION
};

// to do: when acs and zandronum support TRNSLATE lump, move this crap there
void CreateMonsterTranslationTables() {
	// ravagerghost translation is Ice
	CreateTranslation(DND_QUAKEZOMBIESG_TRANSLATION, 128:143=25:40, 236:239=41:47);
	
	CreateTranslation(DND_SMGGUY_TRANSLATION, 112:127=[146,67,51]:[7,0,0]);
	CreateTranslation(DND_QUAKEZOMBIECG_TRANSLATION, 236:239=108:111, 128:143=92:107);
	
	CreateTranslation(DND_NETHERDARKIMP_TRANSLATION, 16:49=112:122, 48:51=112:113, 160:167=112:119, 168:191=114:120, 197:206=160:166, 208:223=116:117, 232:235=116:119, 248:254=115:121);
	
	CreateTranslation(DND_FLAMEDEMON_TRANSLATION, 16:39=101:111, 40:47=5:8, 79:79=8:8, 190:191=7:8, 210:223=87:100, 224:224=161:161);
	CreateTranslation(DND_SCAVENGER_TRANSLATION, 1:2=[2,2,1]:[1,1,1], 7:8=0:0, 9:12=[18,14,12]:[3,2,2], 15:15=[5,4,3]:[5,4,3], 78:78=[12,9,8]:[12,9,8], 127:131=[1,1,1]:[165,133,111], 152:159=[124,94,73]:[24,20,17]);
	CreateTranslation(DND_NHUMCIGN_TRANSLATION, 0:255=%[0.00,0.00,0.00]:[1.17,0.10,1.29], 0:47=0:47, 80:127=80:127, 152:235=152:235, 240:255=240:255);
	CreateTranslation(DND_NIGHTMARESPECTRE_TRANSLATION, 16:31=199:207, 32:47=240:247, 79:79=247:247, 80:111=96:111, 190:191=246:247, 210:223=194:199);
	CreateTranslation(DND_GRAVEDIGGER_TRANSLATION, 0:255=%[0.00,0.00,0.00]:[0.64,0.55,1.02], 0:15=0:15, 48:78=48:78, 80:111=80:111, 160:189=160:189, 224:231=224:231);
	CreateTranslation(DND_DEVOURER_TRANSLATION, 0:255=%[0.00,0.00,0.00]:[0.52,0.45,0.34], 48:78=48:78, 80:111=80:111, 160:189=160:189, 224:235=224:235);
	
	CreateTranslation(DND_WARLORDHELL_TRANSLATION, 1:2=8:0, 13:15=5:7, 48:64=90:110, 64:70=109:112, 70:83=5:10, 110:123=196:207, 80:111=80:111, 124:127=242:245, 128:141=102:111, 143:149=104:114, 142:143=5:5, 148:151=5:7);
	CreateTranslation(DND_BLOODSATYR_TRANSLATION, 0:255=%[0.00,0.00,0.00]:[1.10,0.13,0.13], 3:8=3:8, 16:47=16:47, 80:111=80:111, 168:191=168:191);
	
	CreateTranslation(DND_ENHANCEDCACO_TRANSLATION, 9:12=190:190, 16:31=88:103, 32:39=104:109, 40:47=110:111, 112:116=173:175, 117:127=176:191, 166:166=14:14, 167:167=79:79, 168:175=80:103, 176:187=106:111, 188:189=5:5, 190:191=6:6, 192:207=168:191, 223:223=14:14, 235:235=107:107, 240:246=191:191);

	CreateTranslation(DND_DEFILER_TRANSLATION, 1:2=7:8, 9:12=7:7, 13:14=2:2, 15:15=7:7, 63:77=0:3, 48:63=72:79, 64:67=238:239, 77:79=7:8, 128:134=110:112, 134:136=5:5, 137:140=1:1, 141:142=6:2, 143:143=2:2, 144:151=79:79, 149:151=1:2, 152:159=7:8, 236:237=1:2, 238:239=7:8);

	CreateTranslation(DND_CADAVER_TRANSLATION, 1:2=0:0, 4:4=89:89, 13:15=5:7, 16:47=112:127, 16:20=106:110, 48:76=103:111, 48:48=90:90, 77:79=5:5, 80:108=90:111, 109:111=5:5, 131:167=5:8, 128:141=101:111, 144:149=107:111, 152:159=101:111, 160:167=118:123, 169:191=112:127, 168:170=89:97, 192:207=88:111, 209:223=117:122, 208:210=89:93, 224:224=89:89, 225:235=113:127, 236:239=5:5, 248:249=117:117);

	CreateTranslation(DND_ICEFATSO_TRANSLATION, 1:2=109:110, 13:15=107:109, 17:18=87:88, 48:53=4:4, 53:79=80:106, 112:125=192:202, 128:143=90:111, 144:151=100:107, 209:213=81:89, 236:239=107:109);
	
	CreateTranslation(DND_MOONSATYR_TRANSLATION, 0:255=%[0.00,0.00,0.00]:[1.12,1.20,1.57], 3:8=3:8, 16:47=16:47, 80:111=80:111, 168:191=168:191);
	CreateTranslation(DND_BARBATOS_TRANSLATION, 0:255=%[0.00,0.00,0.00]:[0.76,0.67,0.50], 0:47=0:47, 80:111=80:111, 160:191=160:191, 208:235=208:235);
	CreateTranslation(DND_DREADKNIGHT_TRANSLATION, 3:3=40:40, 80:111=%[0.00,0.00,0.00]:[1.43,0.31,0.31]);
	CreateTranslation(DND_LORDHERESY_TRANSLATION, 16:43=52:77, 33:33=220:220, 34:37=[155,91,19]:[155,91,19], 38:40=[135,67,7]:[135,67,7], 41:43=[115,43,0]:[115,43,0], 44:48=237:239, 190:191=238:238);
	CreateTranslation(DND_KJAROCH_TRANSLATION, 0:47=[202,102,121]:[0,0,0], 0:12=0:12, 13:15=1:2, 48:72=102:111, 73:79=6:8, 79:79=8:8, 137:141=110:112, 141:143=5:5, 144:150=109:109, 151:164=6:110, 190:191=[4,2,2]:[0,0,0]);

	CreateTranslation(DND_HORSHACKER_TRANSLATION, 0:255=%[0.00,0.00,0.00]:[0.28,0.88,1.98], 80:111=80:111, 0:15=0:15, 128:159=128:159, 48:79=48:79, 160:167=160:167, 208:239=208:239, 249:249=249:249);
	CreateTranslation(DND_BERNABE_TRANSLATION, 112:127=%[0.00,0.00,0.00]:[1.23,0.90,0.39]);
}

#endif
