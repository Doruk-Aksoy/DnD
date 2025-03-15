#ifndef DND_ELITE_IN
#define DND_ELITE_IN

#define DND_MIN_ELITEMODS 2
#define DND_MAX_ELITEMODS 4
#define DND_ELITE_DMGSCALE 13
#define DND_ELITE_EXTRASTRONG_BONUS 35
#define DND_ELITE_VITAL_SCALE 75
#define DND_ELITE_CREDITCHANCE_BONUS 0.1
#define DND_ELITE_FX_DENSITY 8
#define MAX_ELITE_FX_WAIT 60
#define DND_ELITE_FX_TID 900
#define DND_NUCLEAR_EXPLOSION_FACTOR 10
#define DND_NUCLEAR_MINDMG 100

// linear scale, returns the old 15% at level 40 and returns 25% at level 80. Returns 30% at level 100.
int GetEliteHealthScale(int level) {
	return level / 4 + 5;
}

#define DND_ELITE_RESOLUTION 10000
#define DND_ELITE_MIN_INCREMENT 33 // per level add 0.33
#define DND_ELITE_RESOLUTION_SCALE 100

#define DND_ELITE_THIEFRATE 25
#define DND_VIOLENTRETALIATION_CHANCE 50
#define DND_HEXFUSION_CHANCE 33

#define MAX_ELITE_TRIES 50
#define DND_MAX_ELITEIMMUNITIES 2

#define MAX_ROLLABLE_TRAITS 49

#include "DnD_EliteInfo.h"

// these are powers, not actual values
#define ELITETRAIT_ID 0
#define ELITETRAIT_LVLREQ 1
#define ELITETRAIT_WEIGHT 2
int EliteTraitNumbers[MAX_ROLLABLE_TRAITS][3] = {
	{ DND_EXPLOSIVE_RESIST, 0, 250 },
	{ DND_BULLET_RESIST, 0, 250 },
	{ DND_ENERGY_RESIST, 0, 250 },
	{ DND_MAGIC_RESIST, 0, 250 },
	{ DND_ELEMENTAL_RESIST, 0, 250 },
	
	{ DND_EXPLOSIVE_IMMUNE, 36, 125 },
	{ DND_EXPLOSIVE_NONE, 36, 125 },
	{ DND_BULLET_IMMUNE, 36, 125 },
	{ DND_ENERGY_IMMUNE, 36, 125 },
	{ DND_MAGIC_IMMUNE, 36, 125 },
	{ DND_ELEMENTAL_IMMUNE, 36, 125 },
	
	{ DND_GHOST, 0, 200 },
	{ DND_HARDENED_SKIN, 0, 250 },
	{ DND_REFLECTIVE, 40, 75 },
	{ DND_AGGRESSIVE, 0, 375 },
	{ DND_EXTRAFAST, 30, 150 },
	
	{ DND_FASTREACTION, 0, 500 },
	{ DND_NOPAIN, 0, 250 },
	{ DND_EXTRASTRONG, 0, 250 },
	{ DND_VITAL, 0, 300 },
	{ DND_ARMORPEN, 30, 200 },
	
	{ DND_BLOODLESS, 0, 325 },
	{ DND_VIOLENTRETALIATION, 40, 180 },
	{ DND_THIEF, 0, 200 },
	{ DND_HEXFUSION, 36, 150 },
	{ DND_REBIRTH, 0, 100 },
	{ DND_VENOMANCER, 0, 400 },
	{ DND_FRIGID, 0, 400 },
	{ DND_SCORCHED, 0, 400 },
	{ DND_INSULATED, 0, 400 },
	{ DND_REJUVENATING, 30, 175 },
	
	{ DND_HATRED, 0, 350 },
	{ DND_SHOCKER, 40, 200 },
	{ DND_VAMPIRISM, 0, 300 },
	{ DND_FORTIFIED, 30, 250 },
	{ DND_SUBORDINATE, 30, 150 },
	{ DND_REPEL, 36, 125 },
	{ DND_PHANTASM, 36, 100 },
	{ DND_CRIPPLE, 40, 185 },
	{ DND_RUINATION, 40, 185 },
	{ DND_NUCLEAR, 40, 50 },
	{ DND_SILENT, 40, 200 },
	{ DND_OSMIUM, 50, 250 },
	{ DND_PHASING, 33, 200 },
	{ DND_OTHERWORLDGRIP, 50, 100 },
	{ DND_THUNDERSTRUCK, 45, 100 },
	{ DND_TEMPORALBUBBLE, 50, 175 },
	{ DND_BLACKOUT, 40, 175 },
	{ DND_ENSHROUDED, 30, 150 }
};

void SetupEliteModWeights() {
	// cascade weights
	for(int i = 0; i < MAX_ROLLABLE_TRAITS - 1; ++i) {
		EliteTraitNumbers[i + 1][ELITETRAIT_WEIGHT] += EliteTraitNumbers[i][ELITETRAIT_WEIGHT];
	}
}

int GetEliteBonusDamage(int m_id) {
	// at level 100 this yields 125%, at level 0 13% and at level 50 44%
	int lvl = MonsterProperties[m_id].level;
	return DND_ELITE_DMGSCALE + lvl * lvl / 100 + (lvl * 3) / 25;
}

bool HasTrait(int id, int trait_index) {
	return MonsterProperties[id].trait_list[trait_index];
}

int GetRandomEliteTrait() {
	int l = 0, h = MAX_ROLLABLE_TRAITS - 1;
	int w = random(0, EliteTraitNumbers[h][ELITETRAIT_WEIGHT]);
	// binary search to find a bit faster than linear... it'll matter with how many monsters can be elites...
	//Log(s:"rolled ", d:w);
	while(l <= h) {
		int m = l + (h - l) / 2;
		//Log(s:"try ", d:EliteTraitNumbers[m][ELITETRAIT_WEIGHT], s: " < ", d:w, s: " ", d: EliteTraitNumbers[m + 1][ELITETRAIT_WEIGHT]);
		if(EliteTraitNumbers[m][ELITETRAIT_WEIGHT] < w && w <= EliteTraitNumbers[m + 1][ELITETRAIT_WEIGHT]) {
			//Log(s:"picked ", d:m, s:" elite mod: ", d:EliteTraitNumbers[m][ELITETRAIT_ID]);
			return m + 1;
		}
		else if(EliteTraitNumbers[m][ELITETRAIT_WEIGHT] < w) {
			l = m + 1;
			//Log(s:"left up to ", d:l);
		}
		else {
			h = m - 1;
			//Log(s:"right down to ", d:h);
		}
	}
	// shouldn't reach here ever unless it picked 0th element
	return 0;
}

bool RollEliteChance() {
	return random(1, DND_ELITE_RESOLUTION) <= Clamp_Between(GetCVar("dnd_elite_spawnchance"), 1, 100) * DND_ELITE_RESOLUTION_SCALE + PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL] * DND_ELITE_MIN_INCREMENT;
}

void SetEliteFlag(int f, bool updateCS) {
	int this = ActivatorTID() - DND_MONSTERTID_BEGIN;
	int i;
	switch (f) {
		case DND_EXPLOSIVE_RESIST:
			if(MonsterProperties[this].resists[DND_DAMAGECATEGORY_EXPLOSIVES] < DND_RESIST_FACTOR)
				MonsterProperties[this].resists[DND_DAMAGECATEGORY_EXPLOSIVES] = DND_RESIST_FACTOR;

			if(MonsterProperties[this].resists[DND_DAMAGECATEGORY_EXPLOSIVES] < DND_RESIST_FACTOR)
				MonsterProperties[this].resists[DND_DAMAGECATEGORY_EXPLOSIVES] = DND_RESIST_FACTOR;
		break;
		case DND_EXPLOSIVE_IMMUNE:
			MonsterProperties[this].trait_list[DND_EXPLOSIVE_RESIST] = false;
			GiveInventory("MakeExplosiveHighResist", 1);
		break;
		case DND_EXPLOSIVE_NONE:
			MonsterProperties[this].trait_list[DND_EXPLOSIVE_RESIST] = false;
			MonsterProperties[this].trait_list[DND_EXPLOSIVE_IMMUNE] = false;
			GiveInventory("MakeExplosiveHighResist", 1);

			if(MonsterProperties[this].resists[DND_DAMAGECATEGORY_EXPLOSIVES] < DND_IMMUNITY_FACTOR)
				MonsterProperties[this].resists[DND_DAMAGECATEGORY_EXPLOSIVES] = DND_IMMUNITY_FACTOR;
		break;
		case DND_BULLET_RESIST:
			GiveInventory("MakePhysicalResist", 1);

			if(MonsterProperties[this].resists[DND_DAMAGECATEGORY_BULLET] < DND_RESIST_FACTOR)
				MonsterProperties[this].resists[DND_DAMAGECATEGORY_BULLET] = DND_RESIST_FACTOR;
		break;
		case DND_BULLET_IMMUNE:
			MonsterProperties[this].trait_list[DND_BULLET_RESIST] = false;
			MonsterProperties[this].trait_list[DND_SILVER_WEAKNESS] = false;
			GiveInventory("MakePhysicalResist", 1);

			if(MonsterProperties[this].resists[DND_DAMAGECATEGORY_BULLET] < DND_IMMUNITY_FACTOR)
				MonsterProperties[this].resists[DND_DAMAGECATEGORY_BULLET] = DND_IMMUNITY_FACTOR;
		break;
		case DND_ENERGY_RESIST:
			MonsterProperties[this].trait_list[DND_ENERGY_WEAKNESS] = false;

			if(MonsterProperties[this].resists[DND_DAMAGECATEGORY_ENERGY] < DND_RESIST_FACTOR)
				MonsterProperties[this].resists[DND_DAMAGECATEGORY_ENERGY] = DND_RESIST_FACTOR;
		break;
		case DND_ENERGY_IMMUNE:
			MonsterProperties[this].trait_list[DND_ENERGY_RESIST] = false;
			MonsterProperties[this].trait_list[DND_ENERGY_WEAKNESS] = false;

			if(MonsterProperties[this].resists[DND_DAMAGECATEGORY_ENERGY] < DND_IMMUNITY_FACTOR)
				MonsterProperties[this].resists[DND_DAMAGECATEGORY_ENERGY] = DND_IMMUNITY_FACTOR;
		break;
		case DND_MAGIC_RESIST:
			MonsterProperties[this].trait_list[DND_MAGIC_WEAKNESS] = false;

			if(MonsterProperties[this].resists[DND_DAMAGECATEGORY_OCCULT] < DND_RESIST_FACTOR)
				MonsterProperties[this].resists[DND_DAMAGECATEGORY_OCCULT] = DND_RESIST_FACTOR;
		break;
		case DND_MAGIC_IMMUNE:
			MonsterProperties[this].trait_list[DND_MAGIC_RESIST] = false;
			MonsterProperties[this].trait_list[DND_MAGIC_WEAKNESS] = false;

			if(MonsterProperties[this].resists[DND_DAMAGECATEGORY_OCCULT] < DND_IMMUNITY_FACTOR)
				MonsterProperties[this].resists[DND_DAMAGECATEGORY_OCCULT] = DND_IMMUNITY_FACTOR;
		break;
		case DND_ELEMENTAL_RESIST:
			MonsterProperties[this].trait_list[DND_FIRE_WEAKNESS] = false;
			MonsterProperties[this].trait_list[DND_ICE_WEAKNESS] = false;

			for(i = DND_ELECATEGORY_BEGIN; i <= DND_ELECATEGORY_END; ++i)
				if(MonsterProperties[this].resists[i] < DND_RESIST_FACTOR)
					MonsterProperties[this].resists[i] = DND_RESIST_FACTOR;
		break;
		case DND_ELEMENTAL_IMMUNE:
			MonsterProperties[this].trait_list[DND_ELEMENTAL_RESIST] = false;
			MonsterProperties[this].trait_list[DND_FIRE_WEAKNESS] = false;
			MonsterProperties[this].trait_list[DND_ICE_WEAKNESS] = false;

			for(i = DND_ELECATEGORY_BEGIN; i <= DND_ELECATEGORY_END; ++i)
				if(MonsterProperties[this].resists[i] < DND_IMMUNITY_FACTOR)
					MonsterProperties[this].resists[i] = DND_IMMUNITY_FACTOR;
		break;
		
		case DND_GHOST:
			GiveInventory("MakeGhost", 1); // setactorflag didnt work...
			SetActorProperty(0, APROP_RENDERSTYLE, STYLE_TRANSLUCENT);
			SetActorProperty(0, APROP_ALPHA, 0.5);
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
		case DND_HATRED:
			GiveInventory("MakeHatred", 1);
		break;
		case DND_FORTIFIED:
			GiveInventory("MonsterFortifyCount", MonsterProperties[this].maxhp / DND_FORTIFY_AMOUNT);
		break;
		case DND_REPEL:
			GiveInventory("Repel_Script_Run", 1);
		break;
		case DND_CRIPPLE:
			GiveInventory("Cripple_Script_Run", 1);
		break;
		case DND_VIOLENTRETALIATION:
			ACS_NamedExecuteAlways("DnD Aura Giver CS", 0, DND_VIOLENTRETALIATION);
		break;
		case DND_SILENT:
			SetActorProperty(0, APROP_ACTIVESOUND, "NullSound");
			SetActorProperty(0, APROP_SEESOUND, "NullSound");
		break;
		case DND_PHASING:
			GiveInventory("Phasing_Script_Run", 1);
		break;
		case DND_TEMPORALBUBBLE:
			ACS_NamedExecuteAlways("DnD Aura Giver CS", 0, DND_TEMPORALBUBBLE);
		break;
		case DND_ENSHROUDED:
			ACS_NamedExecuteAlways("DnD Aura Giver CS", 0, DND_ENSHROUDED);
		break;
		case DND_REFLECTIVE:
			ACS_NamedExecuteAlways("DND Spawn Attachment", 0, ActivatorTID(), DND_ELITEFX_REFLECT);
		break;
	}

/*#ifdef ISDEBUGBUILD
	if(f == DND_NUCLEAR)
		++test_counter;
#endif*/

	MonsterProperties[this].trait_list[f] = true;
	
	// sync to client too
	if(updateCS)
		ACS_NamedExecuteWithResult("DnD Monster Trait Give CS", f, -1, -1, -1);
}

Script "DnD Aura Giver CS" (int trait, int extra) CLIENTSIDE {
	int tid = ActivatorTID();
	switch(trait) {
		case DND_CRIPPLE:
			ACS_NamedExecuteAlways("DND Spawn Attachment", 0, tid, DND_ELITEFX_CRIPPLEAURA);
		break;
		case DND_VIOLENTRETALIATION:
			ACS_NamedExecuteAlways("DND Spawn Attachment", 0, tid, DND_ELITEFX_VIOLENTAURA);
		break;
		case DND_TEMPORALBUBBLE:
			if(!extra)
				ACS_NamedExecuteAlways("DND Spawn Attachment", 0, tid, DND_ELITEFX_TEMPORALBUBBLE);
			else
				GiveInventory("TemporalBubbleCooldown", 1);
		break;
		case DND_ENSHROUDED:
			if(!extra) {
				ACS_NamedExecuteAlways("DND Spawn Attachment", 0, tid, DND_ELITEFX_ENSHROUD);
				SetActorProperty(0, APROP_RENDERSTYLE, STYLE_TranslucentStencil);
				SetActorProperty(0, APROP_STENCILCOLOR, 0x555555);

				if(GetActorProperty(0, APROP_ALPHA) == 1.0)
					SetActorProperty(0, APROP_ALPHA, 0.5);
			}
			else {
				SetActorProperty(0, APROP_ALPHA, 1.0);
				SetActorProperty(0, APROP_RENDERSTYLE, STYLE_NORMAL);
			}
		break;
	}
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
bool HasTraitExceptions(int m_id, int trait) {
	int t = EliteTraitNumbers[trait][ELITETRAIT_ID];
	int i = ActivatorTID() - DND_MONSTERTID_BEGIN;
	return 	CheckEliteCvar(t) 																||
			MonsterProperties[m_id].level < EliteTraitNumbers[trait][ELITETRAIT_LVLREQ]		||
			(t == DND_EXPLOSIVE_RESIST && HasTrait(i, DND_EXPLOSIVE_IMMUNE)) 				|| 
			(t == DND_EXPLOSIVE_IMMUNE && HasTrait(i, DND_EXPLOSIVE_NONE)) 					||
			(t == DND_BULLET_RESIST && HasTrait(i, DND_BULLET_IMMUNE)) 						||
			(t == DND_ENERGY_RESIST && HasTrait(i, DND_ENERGY_IMMUNE)) 						|| 
			(t == DND_MAGIC_RESIST && HasTrait(i, DND_MAGIC_IMMUNE)) 						|| 
			(t == DND_ELEMENTAL_RESIST && HasTrait(i, DND_ELEMENTAL_IMMUNE))				||
			(t == DND_PHASING && HasTrait(i, DND_GHOST))									||
			(t == DND_GHOST && HasTrait(i, DND_PHASING))									||
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

// from which segment to return, 0-31, 32-63, 64-95 etc.
int GetMonsterTraits(int monster_id, int segment) {
	int ret = 0;
	int seg5 = segment << 5;
	int lim = seg5 + 32;
	for(int i = seg5; i < lim && i < MAX_MONSTER_TRAITS_STORED; ++i) {
		//if(MonsterProperties[monster_id].class == MONSTERCLASS_IMP && MonsterProperties[monster_id].trait_list[i])
		//	Log(s:"pack trait ", d:i, s: " for monster ", s:GetActorClass(ActivatorTID()), s: " <", d:monster_id, s: ">: ", d:MonsterProperties[monster_id].trait_list[i]);
		ret |= MonsterProperties[monster_id].trait_list[i] * (1 << (i - seg5));
	}
	return ret;
}

int GetPetMonsterTraits(int monster_id, int segment) {
	int ret = 0;
	int seg5 = segment << 5;
	int lim = seg5 + 32;
	for(int i = seg5; i < lim && i < MAX_MONSTER_TRAITS_STORED; ++i)
		ret |= PetMonsterProperties[monster_id].trait_list[i] * (1 << (i - seg5));
	return ret;
}

void DecideEliteTraits(int tid, int m_id, int count) {
	int tries = 0;
	while(tries < MAX_ELITE_TRIES && count) {
		int try_trait = GetRandomEliteTrait();
		if(!HasTrait(m_id, EliteTraitNumbers[try_trait][ELITETRAIT_ID]) && !HasTraitExceptions(m_id, try_trait) && CheckImmunityFlagStatus(EliteTraitNumbers[try_trait][ELITETRAIT_ID])) {
			// dont give explosive immunity with resist etc
			SetEliteFlag(EliteTraitNumbers[try_trait][ELITETRAIT_ID], false);
			--count;
		}
		++tries;
	}

	// Run the elite special fx script on this monster
	ACS_NamedExecuteAlways("DND Spawn Attachment", 0, tid);
}

Script "DnD Monster Nuclear Explosion" (int this) {
	int hpdamage = MonsterProperties[this - DND_MONSTERTID_BEGIN].maxhp / DND_NUCLEAR_EXPLOSION_FACTOR;
	if(hpdamage < DND_NUCLEAR_MINDMG)
		hpdamage = DND_NUCLEAR_MINDMG;
	
	SpawnForced("MonsterNuclearExplosion", GetActorX(this), GetActorY(this), GetActorZ(this) + GetActorProperty(this, APROP_HEIGHT) / 2, DND_NUCLEAREXP_TID);
	SetActorProperty(DND_NUCLEAREXP_TID, APROP_MASS, hpdamage);
	SetActivator(DND_NUCLEAREXP_TID);
	SetPointer(AAPTR_TARGET, this);
	Thing_ChangeTID(DND_NUCLEAREXP_TID, 0);
}

#endif
