#ifndef DND_ELITE_IN
#define DND_ELITE_IN

#define DND_MIN_ELITEMODS 2
#define DND_MAX_ELITEMODS 4
#define DND_ELITE_HPSCALE 15
#define DND_ELITE_DMGSCALE 0.02
#define DND_ELITE_EXTRASTRONG_BONUS 0.35
#define DND_ELITE_VITAL_SCALE 75
#define DND_ELITE_CREDITCHANCE_BONUS 0.1
#define DND_ELITE_EXP_BONUS 25
#define DND_ELITE_CREDIT_BONUS 25
#define DND_ELITE_FX_DENSITY 8
#define MAX_ELITE_FX_WAIT 60
#define DND_ELITE_FX_TID 900

#define DND_ELITE_RESOLUTION 10000
#define DND_ELITE_MIN_INCREMENT 25 // per level add 0.25
#define DND_ELITE_RESOLUTION_SCALE 100

#define DND_ELITE_THIEFRATE 15
#define DND_VIOLENTRETALIATION_CHANCE 50
#define DND_HEXFUSION_CHANCE 33

#define MAX_ELITE_TRIES 50
#define DND_MAX_ELITEIMMUNITIES 2

#define MAX_ROLLABLE_TRAITS 31

#include "DnD_EliteInfo.h"

// these are powers, not actual values
int EliteTraitNumbers[MAX_ROLLABLE_TRAITS] = {
	DND_EXPLOSIVE_RESIST,
	DND_BULLET_RESIST,
	DND_ENERGY_RESIST,
	DND_MAGIC_RESIST,
	DND_ELEMENTAL_RESIST,
	
	DND_EXPLOSIVE_IMMUNE,
	DND_EXPLOSIVE_NONE,
	DND_BULLET_IMMUNE,
	DND_ENERGY_IMMUNE,
	DND_MAGIC_IMMUNE,
	DND_ELEMENTAL_IMMUNE, 
	
	DND_GHOST,
	DND_HARDENED_SKIN,
	DND_REFLECTIVE,
	DND_AGGRESSIVE,
	DND_EXTRAFAST,
	
	DND_FASTREACTION,
	DND_NOPAIN,
	DND_EXTRASTRONG,
	DND_VITAL,
	DND_ARMORPEN,
	
	DND_BLOODLESS,
	DND_VIOLENTRETALIATION,
	DND_THIEF,
	DND_HEXFUSION,
	DND_REBIRTH,
	DND_VENOMANCER,
	DND_FRIGID,
	DND_SCORCHED,
	DND_INSULATED,
	DND_REJUVENATING
};

int GetEliteBonusDamage() {
	return DND_ELITE_DMGSCALE * MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].level;
}

bool HasTrait(int id, int trait_index) {
	return MonsterProperties[id].trait_list[trait_index];
}

int GetRandomEliteTrait() {
	return random(0, MAX_ROLLABLE_TRAITS - 1);
}

bool RollEliteChance() {
	return random(1, DND_ELITE_RESOLUTION) <= Clamp_Between(GetCVar("dnd_elite_spawnchance"), 1, 100) * DND_ELITE_RESOLUTION_SCALE + PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL] * DND_ELITE_MIN_INCREMENT;
}

void SetEliteFlag(int f) {
	int this = ActivatorTID() - DND_MONSTERTID_BEGIN;
	switch (f) {
		case DND_EXPLOSIVE_IMMUNE:
			MonsterProperties[this].trait_list[DND_EXPLOSIVE_RESIST] = false;
			GiveInventory("MakeExplosiveHighResist", 1);
		break;
		case DND_EXPLOSIVE_NONE:
			MonsterProperties[this].trait_list[DND_EXPLOSIVE_RESIST] = false;
			MonsterProperties[this].trait_list[DND_EXPLOSIVE_IMMUNE] = false;
			GiveInventory("MakeExplosiveHighResist", 1);
		break;
		case DND_BULLET_RESIST:
			GiveInventory("MakePhysicalResist", 1);
		break;
		case DND_BULLET_IMMUNE:
			MonsterProperties[this].trait_list[DND_BULLET_RESIST] = false;
			MonsterProperties[this].trait_list[DND_SILVER_WEAKNESS] = false;
			GiveInventory("MakePhysicalResist", 1);
		break;
		case DND_ENERGY_RESIST:
			MonsterProperties[this].trait_list[DND_ENERGY_WEAKNESS] = false;
		break;
		case DND_ENERGY_IMMUNE:
			MonsterProperties[this].trait_list[DND_ENERGY_RESIST] = false;
			MonsterProperties[this].trait_list[DND_ENERGY_WEAKNESS] = false;
		break;
		case DND_MAGIC_RESIST:
			MonsterProperties[this].trait_list[DND_MAGIC_WEAKNESS] = false;
		break;
		case DND_MAGIC_IMMUNE:
			MonsterProperties[this].trait_list[DND_MAGIC_RESIST] = false;
			MonsterProperties[this].trait_list[DND_MAGIC_WEAKNESS] = false;
		break;
		case DND_ELEMENTAL_RESIST:
			MonsterProperties[this].trait_list[DND_FIRE_WEAKNESS] = false;
			MonsterProperties[this].trait_list[DND_ICE_WEAKNESS] = false;
		break;
		case DND_ELEMENTAL_IMMUNE:
			MonsterProperties[this].trait_list[DND_ELEMENTAL_RESIST] = false;
			MonsterProperties[this].trait_list[DND_FIRE_WEAKNESS] = false;
			MonsterProperties[this].trait_list[DND_ICE_WEAKNESS] = false;
		break;
		
		case DND_GHOST:
			GiveInventory("MakeGhost", 1); // setactorflag didnt work...
			SetActorProperty(0, APROP_RENDERSTYLE, STYLE_TRANSLUCENT);
			SetActorProperty(0, APROP_ALPHA, 0.5);
		break;
		case DND_HARDENED_SKIN:
			GiveInventory("MakeHardenedSkin", 1);
		break;
		case DND_REFLECTIVE:
			GiveInventory("MakeReflective", 1);
		break;
		case DND_AGGRESSIVE:
			GiveInventory("MakeAggressive", 1);
		break;
		case DND_EXTRAFAST:
			GiveInventory("MakeFaster", 1);
		break;
		case DND_FASTREACTION:
			GiveInventory("MakeRetaliate", 1);
		break;
		case DND_NOPAIN:
			GiveInventory("MakeNoPain", 1);
		break;
		case DND_ARMORPEN:
			// if monster has pierce, remove it
			MonsterProperties[this].trait_list[DND_PIERCE] = false;
			GiveInventory("MakePierce", 1);
		break;
		case DND_REJUVENATING:
			GiveInventory("Rejuvenate_Script_Run", 1);
		break;
	}
	MonsterProperties[this].trait_list[f] = true;
}

// check if the given flag conforms to the cvars we declared
bool CheckEliteCvar(int t) {
	bool res = 0;
	if(GetCVar("dnd_no_immunity")) {
		res = (t == DND_EXPLOSIVE_NONE) || (t == DND_BULLET_IMMUNE) || (t == DND_ENERGY_IMMUNE) || (t == DND_MAGIC_IMMUNE) || (t == DND_ELEMENTAL_IMMUNE);
	}
	if(GetCVar("dnd_no_reflect"))
		res = (t == DND_REFLECTIVE);
	return res;
}

// Exception occurs when: Either we rolled a resist when we have the corresponding immunity OR we rolled an immunity when we said this can't roll due to cvar
bool HasTraitExceptions(int t) {
	int i = ActivatorTID() - DND_MONSTERTID_BEGIN;
	return 	CheckEliteCvar(t) 																||
			(t == DND_EXPLOSIVE_RESIST && HasTrait(i, DND_EXPLOSIVE_IMMUNE)) 				|| 
			(t == DND_EXPLOSIVE_IMMUNE && HasTrait(i, DND_EXPLOSIVE_NONE)) 					||
			(t == DND_BULLET_RESIST && HasTrait(i, DND_BULLET_IMMUNE)) 						||
			(t == DND_ENERGY_RESIST && HasTrait(i, DND_ENERGY_IMMUNE)) 						|| 
			(t == DND_MAGIC_RESIST && HasTrait(i, DND_MAGIC_IMMUNE)) 						|| 
			(t == DND_ELEMENTAL_RESIST && HasTrait(i, DND_ELEMENTAL_IMMUNE))				||
			(t == DND_REBIRTH && (HasTrait(i, DND_SUMMONED) || HasTrait(i, DND_REVIVED)));
}

bool HasMaxImmunes() {
	int i = ActivatorTID() - DND_MONSTERTID_BEGIN;
	return 	HasTrait(i, DND_EXPLOSIVE_NONE) + HasTrait(i, DND_BULLET_IMMUNE) + HasTrait(i, DND_ENERGY_IMMUNE) + 
			HasTrait(i, DND_MAGIC_IMMUNE) 	+ HasTrait(i, DND_ELEMENTAL_IMMUNE) >= DND_MAX_ELITEIMMUNITIES;
}

bool IsImmunityFlag(int flag) {
	return flag == DND_EXPLOSIVE_NONE || flag == DND_BULLET_IMMUNE || flag == DND_ENERGY_IMMUNE || flag == DND_MAGIC_IMMUNE || flag == DND_ELEMENTAL_IMMUNE;
}

bool CheckImmunityFlagStatus(int try_trait) {
	return !(IsImmunityFlag(try_trait) && HasMaxImmunes());
}

void DecideEliteTraits(int count) {
	int tries = 0;
	int this = ActivatorTID() - DND_MONSTERTID_BEGIN;
	// Run the elite special fx script on this monster
	GiveInventory("RunEliteFXScript", 1);
	while(tries < MAX_ELITE_TRIES && count) {
		int try_trait = GetRandomEliteTrait();
		if(!HasTrait(this, EliteTraitNumbers[try_trait])) {
			// dont give explosive immunity with resist etc
			if(!HasTraitExceptions(EliteTraitNumbers[try_trait]) && CheckImmunityFlagStatus(EliteTraitNumbers[try_trait])) {
				SetEliteFlag(EliteTraitNumbers[try_trait]);
				--count;
			}
		}
		++tries;
	}
}

#endif
