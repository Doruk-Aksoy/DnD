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

#define MAX_ELITE_TRIES 50
#define DND_MAX_ELITEIMMUNITIES 2

#define MAX_ROLLABLE_TRAITS 21
#define ELITE_FLAG2_BEGIN 11

#include "DnD_ElitePow.h"

// these are powers, not actual values
int EliteTraitNumbers[MAX_ROLLABLE_TRAITS] = {
	DND_EXPLOSIVE_RESIST_POW,
	DND_EXPLOSIVE_IMMUNE_POW,
	DND_EXPLOSIVE_NONE_POW,
	DND_BULLET_RESIST_POW,
	DND_GHOST_POW,
	DND_HARDENED_SKIN_POW,
	DND_REFLECTIVE_POW,
	DND_AGGRESSIVE_POW,
	DND_EXTRAFAST_POW,
	DND_FASTREACTION_POW,
	DND_NOPAIN_POW,
	
	// from here and onwards consider traits flag2
	DND_BULLET_IMMUNE_POW,
	DND_ENERGY_RESIST_POW,
	DND_ENERGY_IMMUNE_POW,
	DND_MAGIC_RESIST_POW,
	DND_MAGIC_IMMUNE_POW,
	DND_ELEMENTAL_RESIST_POW,
	DND_ELEMENTAL_IMMUNE_POW, 
	DND_EXTRASTRONG_POW,
	DND_VITAL_POW,
	DND_ARMORPEN_POW
};

int GetEliteBonusDamage() {
	return DND_ELITE_DMGSCALE * MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].level;
}

bool IsExtraStrong() {
	return IsSet(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits2, DND_EXTRASTRONG_POW);
}

int GetRandomEliteTrait() {
	return random(0, MAX_ROLLABLE_TRAITS - 1);
}

bool RollEliteChance() {
	return random(1, DND_ELITE_RESOLUTION) <= Clamp_Between(GetCVar("dnd_elite_spawnchance"), 1, 100) * DND_ELITE_RESOLUTION_SCALE + PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL] * DND_ELITE_MIN_INCREMENT;
}

void SetEliteFlag(int f, int flagside) {
	if(!flagside) {
		switch (f) {
			case DND_EXPLOSIVE_RESIST_POW:
				GiveInventory("MakeExplosiveResist", 1);
			break;
			case DND_EXPLOSIVE_IMMUNE_POW:
				GiveInventory("MakeExplosiveHighResist", 1);
			break;
			case DND_EXPLOSIVE_NONE_POW:
				GiveInventory("MakeExplosiveFullImmune", 1);
			break;
			case DND_BULLET_RESIST_POW:
				GiveInventory("MakePhysicalResist", 1);
			break;
			case DND_GHOST_POW:
				GiveInventory("MakeGhost", 1); // setactorflag didnt work...
				SetActorProperty(0, APROP_RENDERSTYLE, STYLE_TRANSLUCENT);
				SetActorProperty(0, APROP_ALPHA, 0.5);
			break;
			case DND_HARDENED_SKIN_POW:
				GiveInventory("MakeHardenedSkin", 1);
			break;
			case DND_REFLECTIVE_POW:
				GiveInventory("MakeReflective", 1);
			break;
			case DND_AGGRESSIVE_POW:
				GiveInventory("MakeAggressive", 1);
			break;
			case DND_EXTRAFAST_POW:
				GiveInventory("MakeFaster", 1);
			break;
			case DND_FASTREACTION_POW:
				GiveInventory("MakeRetaliate", 1);
			break;
			case DND_NOPAIN_POW:
				GiveInventory("MakeNoPain", 1);
			break;
		}
		MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits |= (1 << f);
	}
	else {
		switch (f) {
			case DND_BULLET_IMMUNE_POW:
				if(IsSet(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, DND_SILVER_WEAKNESS_POW))
					MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits = ClearBit(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, DND_SILVER_WEAKNESS_POW);
				GiveInventory("MakePhysicalImmune", 1);
			break;
			case DND_ENERGY_RESIST_POW:
				MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits = ClearBit(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, DND_ENERGY_WEAKNESS_POW);
				GiveInventory("MakeEnergyResist", 1);
			break;
			case DND_ENERGY_IMMUNE_POW:
				MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits = ClearBit(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, DND_ENERGY_WEAKNESS_POW);
				GiveInventory("MakeEnergyImmune", 1);
			break;
			case DND_MAGIC_RESIST_POW:
				MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits = ClearBit(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, DND_MAGIC_WEAKNESS_POW);
				GiveInventory("MakeMagicResist", 1);
			break;
			case DND_MAGIC_IMMUNE_POW:
				MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits = ClearBit(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, DND_MAGIC_WEAKNESS_POW);
				GiveInventory("MakeMagicImmune", 1);
			break;
			case DND_ELEMENTAL_RESIST_POW:
				MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits = ClearBit(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, DND_FIRE_WEAKNESS_POW);
				MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits = ClearBit(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, DND_ICE_WEAKNESS_POW);
				GiveInventory("MakeElementalResist", 1);
			break;
			case DND_ELEMENTAL_IMMUNE_POW:
				MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits = ClearBit(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, DND_FIRE_WEAKNESS_POW);
				MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits = ClearBit(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, DND_ICE_WEAKNESS_POW);
				GiveInventory("MakeElementalImmune", 1);
			break;
			case DND_ARMORPEN_POW:
				// if monster has pierce, remove it
				if(IsSet(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, DND_PIERCE_POW))
					MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits = ClearBit(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, DND_PIERCE_POW);
				GiveInventory("MakePierce", 1);
			break;
			case DND_REVIVED_POW:
				//printbold(s:"Mob got revived!");
				GiveInventory("Mo_Revived", 1);
			break;
		}
		MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits2 |= (1 << f);
	}
}

bool CheckEliteCvar(int t, int flagtype) {
	bool res = 0;
	if(GetCVar("dnd_no_immunity")) {
		res |= (!flagtype && ((t == DND_EXPLOSIVE_NONE_POW))) ||
			   (flagtype && ((t == DND_BULLET_IMMUNE_POW) | (t == DND_ENERGY_IMMUNE_POW) | (t == DND_MAGIC_IMMUNE_POW) | (t == DND_ELEMENTAL_IMMUNE_POW)));
	}
	if(GetCVar("dnd_no_reflect"))
		res |= !flagtype && t == DND_REFLECTIVE_POW;
	return res;
}

bool HasTraitExceptions(int t, int flagtype) {
	int traits = MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, traits2 = MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits2;
	if(!flagtype)
		return ((t == DND_EXPLOSIVE_RESIST_POW && IsSet(traits, DND_EXPLOSIVE_IMMUNE_POW)) || (t == DND_EXPLOSIVE_IMMUNE_POW && IsSet(traits, DND_EXPLOSIVE_RESIST_POW))) ||
			   ((t == DND_EXPLOSIVE_IMMUNE_POW && IsSet(traits, DND_EXPLOSIVE_NONE_POW)) || (t == DND_EXPLOSIVE_NONE_POW && IsSet(traits, DND_EXPLOSIVE_IMMUNE_POW))) ||
			   ((t == DND_EXPLOSIVE_RESIST_POW && IsSet(traits, DND_EXPLOSIVE_NONE_POW)) || (t == DND_EXPLOSIVE_NONE_POW && IsSet(traits, DND_EXPLOSIVE_RESIST_POW))) ||
			   ((t == DND_BULLET_RESIST_POW && IsSet(traits2, DND_BULLET_IMMUNE_POW))) || CheckEliteCvar(t, flagtype);
	else
		return ((t == DND_ENERGY_RESIST_POW && IsSet(traits2, DND_ENERGY_IMMUNE_POW)) || (t == DND_ENERGY_IMMUNE_POW && IsSet(traits2, DND_ENERGY_RESIST_POW))) ||
			   ((t == DND_MAGIC_RESIST_POW && IsSet(traits2, DND_MAGIC_IMMUNE_POW)) || (t == DND_MAGIC_IMMUNE_POW && IsSet(traits2, DND_MAGIC_RESIST_POW))) ||
			   ((t == DND_ELEMENTAL_RESIST_POW && IsSet(traits2, DND_ELEMENTAL_IMMUNE_POW))) || ((t == DND_ELEMENTAL_IMMUNE_POW && IsSet(traits2, DND_ELEMENTAL_RESIST_POW))) ||
			   ((t == DND_BULLET_IMMUNE_POW && IsSet(traits, DND_BULLET_RESIST_POW))) || CheckEliteCvar(t, flagtype);
}

bool HasMaxImmunes() {
	int traits2 = MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits2;
	return !!IsSet(MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits, DND_EXPLOSIVE_NONE_POW) + 
		   !!IsSet(traits2, DND_BULLET_IMMUNE_POW) + 
		   !!IsSet(traits2, DND_ENERGY_IMMUNE_POW) +
		   !!IsSet(traits2, DND_MAGIC_IMMUNE_POW) +
		   !!IsSet(traits2, DND_ELEMENTAL_IMMUNE_POW) >= DND_MAX_ELITEIMMUNITIES;
}

bool IsImmunityFlag(int flag, int flagside) {
	return (!flagside && flag == DND_EXPLOSIVE_NONE_POW) || (flagside && (flag == DND_BULLET_IMMUNE_POW || flag == DND_ENERGY_IMMUNE_POW || flag == DND_MAGIC_IMMUNE_POW || flag == DND_ELEMENTAL_IMMUNE_POW));
}

bool CheckImmunityFlagStatus(int try_trait, int flagside) {
	return !(IsImmunityFlag(try_trait, flagside) && HasMaxImmunes());
}

void DecideEliteTraits(int count) {
	int tries = 0;
	// Run the elite special fx script on this monster
	GiveInventory("RunEliteFXScript", 1);
	while(tries < MAX_ELITE_TRIES && count) {
		int tocheck = MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits;
		int flagside = 0;
		int try_trait = GetRandomEliteTrait();
		
		if(try_trait >= ELITE_FLAG2_BEGIN) {
			tocheck = MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].traits2;
			flagside = 1;
		}

		if(!IsSet(tocheck, EliteTraitNumbers[try_trait])) {
			// dont give explosive immunity with resist etc
			if(!HasTraitExceptions(EliteTraitNumbers[try_trait], flagside) && CheckImmunityFlagStatus(EliteTraitNumbers[try_trait], flagside)) {
				SetEliteFlag(EliteTraitNumbers[try_trait], flagside);
				--count;
			}
		}
		++tries;
	}
}

#endif