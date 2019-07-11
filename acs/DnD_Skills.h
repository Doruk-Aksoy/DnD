#ifndef DND_SKILLS_IN
#define DND_SKILLS_IN

#include "DnD_CommonStat.h"

#define DND_SKILL_MINLEVEL 1
#define DND_SKILL_MAXLEVEL 10
#define MAX_SKILL_LEVELS 10

#define TEMPORARY_PET_TID 1500
#define TEMPORARY_SPELL_TID 1600
#define DND_PET_MOVEDIST 512.0

#define RALLY_DIST_PER_INT 0.75
#define RALLY_DIST_PER_CHR 1.25
#define RALLY_DISTANCE 128.0
#define RALLY_DURATION 8
#define RALLY_FX_COUNT 32
#define RALLY_R 64
#define RALLY_TIC_TIMER 8 * TICRATE
#define RALLY_COOLDOWN 20

#define ICESHIELD_COUNT 8
#define ICESHIELD_BASE_DURATION 5 * TICRATE
#define ICESHIELD_DURATION_PER_INT 5
#define ICESHIELD_HEALTHBASE 75
#define ICESHIELD_HEALTH_PER_LEVEL 10
#define ICESHIELD_HEALTH_PER_INT 5

#define MOLTENBOULDER_BASESPEED 20

#define LIGHTNINGSPEAR_BASE_RIP 5
#define LIGHTNINGSPEAR_INT_FACTOR 20

enum {
	SKILLINFO_ZOMBIEPETTIMER
};

enum {
	DND_SPELL_RALLY,
	DND_SPELL_POISONNOVA,
	DND_SPELL_ICESHIELD,
	DND_SPELL_MOLTENBOULDER,
	DND_SPELL_LIGHTNINGSPEAR,
};
#define MAX_SPELLS DND_SPELL_LIGHTNINGSPEAR + 1

enum {
	DND_SPELLDAMAGE_POISONNOVA,
	DND_SPELLDAMAGE_POISONNOVADOT,
	
	DND_SPELLDAMAGE_MOLTENBOULDER,
	DND_SPELLDAMAGE_MOLTENBOULDERBUMP,
	
	DND_SPELLDAMAGE_LIGHTNINGSPEAR,
	DND_SPELLDAMAGE_LIGHTNINGSPEARBOUNCE,
};
#define MAX_DAMAGING_SPELLS DND_SPELLDAMAGE_LIGHTNINGSPEARBOUNCE + 1

enum {
	DND_SPELLFLAG_ELEMENTAL = 1
};

#define SPELL_COOLDOWN 0
#define SPELL_FLAGS 1
#define SPELL_DAMAGE_INDEX 2
int SpellData[MAX_SPELLS][4] = {
	{ RALLY_COOLDOWN, 		0 },
	{ 0, 					DND_SPELLFLAG_ELEMENTAL },
	{ 0, 					DND_SPELLFLAG_ELEMENTAL },
	{ 0, 					DND_SPELLFLAG_ELEMENTAL },
	{ 0, 					DND_SPELLFLAG_ELEMENTAL }
};


pdmg_T SpellDamageTable[MAX_DAMAGING_SPELLS] = {
	{ 20, 0, 0 },
	{ 2, 5, 8 },
	
	{ 24, 0, 0 },
	{ 256, 0, 0 },
	
	{ 15, 5, 8 },
	{ 45, 0, 0 }
};

#define SPELL_NAME 0
#define SPELL_COOLDOWNITEM 1
#define SPELL_COOLDOWNCOUNTER 2
str SpellInfo[MAX_SPELLS][3] = {
	{ "Rally", 					"RallyCooldown", 					"RallyCooldownCounter" },
	{ "PoisonNova", 			"PoisonNovaCooldown", 				"PoisonNovaCooldownCounter" },
	{ "IceShield",				"IceShieldCooldown",				"IceShieldCooldownCounter" },
	{ "MoltenBoulder",			"MoltenBoulderCooldown",			"MoltenBoulderCooldownCounter" },
	{ "LightningSpear",			"LightningSpearCooldown",			"LightningSpearCooldownCounter" }
};

#define MAX_SUMMON_ZOMBIECOUNT 5
#define SKILL_ZOMBIE_DURATION 8
#define SKILL_ZOMBIE_HP_PER_INT 2
#define ZOMBIE_INT_TIMER_FACTOR 10
#define ZOMBIE_INT_DAMAGE_FACTOR 0.125
#define DND_MAX_PETPAINSHARE 9

void HandleZombieRaiseOnDeath(int target) {
	int pet_tid = target - P_TIDSTART + TEMPORARY_PET_TID;
	int this = ActivatorTID();
	SpawnForced("ZombiePet", GetActorX(this), GetActorY(this), GetActorFloorZ(this), pet_tid);
	GiveActorInventory(target, "SummonedZombiePets", 1);
	GiveActorInventory(target, "PetCounter", 1);
	// assignments of properties
	SetActorProperty(pet_tid, APROP_MASTERTID, target);
	SetActivator(pet_tid);
	SetPointer(AAPTR_MASTER, target);
	SetActorProperty(0, APROP_FRIENDLY, true);
	Thing_ChangeTID(pet_tid, 0);
	SetActivator(target);
	// do the trigger
	ACS_NamedExecuteAlways("DnD On Pet Summon", 0);
	SetActivator(this);
}

bool HasRoomForPets(int tid) {
	return CheckActorInventory(tid, "PetCounter") < GetPetCap(tid);
}

void CleanPetStuff() {
	SetInventory("SummonedZombiePets", 0);
	SetInventory("PetCounter", 0);
	for(int i = 0; i < DND_MAX_PETPAINSHARE; ++i)
		TakeInventory(StrParam(s:"PetDamageReduction_", d:i + 1), 1);
}

int GetPlayerAllocatedSpell(int spell_id) {
	return 0;
}

void CastRandomElementalSpell() {
	int pick = 0;
	do {
		pick = random(0, MAX_SPELLS - 1);
	} while(!(SpellData[pick][SPELL_FLAGS] & DND_SPELLFLAG_ELEMENTAL));
	ACS_NamedExecuteAlways("DnD Cast Spell", 0, pick, 0);
}

int GetPetDamageFactor(int base, int master) {
	base >>= 16;
	switch(base) {
		case MONSTER_PET_ZOMBIE:
		return ZOMBIE_INT_DAMAGE_FACTOR * GetActorIntellect(master);
	}
	return 0;
}

Script "DnD Cast Spell" (int spell_id, int usesCooldown) NET {
	int spell_level, i, temp, temp2, this = ActivatorTID();
	//int spell = GetPlayerAllocatedSpell(spell_id);
	str sptr1, sptr2, bufftimer = 0;
	switch(spell_id) {
		case DND_SPELL_RALLY:
			ActivatorSound("Spell/RallyCast", 127);
			spell_level = GetPlayerAttributeValue(PlayerNumber(), INV_EX_ABILITY_RALLY);
			temp = RALLY_DISTANCE + GetIntellect() * RALLY_DIST_PER_INT + GetCharisma() * RALLY_DIST_PER_CHR;
			bufftimer = RALLY_DURATION * TICRATE;
			if(CheckInventory("Wanderer_Perk50")) {
				sptr1 = StrParam(s:"Rally_Damage_Wanderer_Lvl", d:spell_level);
				sptr2 = StrParam(s:"Rally_Speed_Wanderer_Lvl", d:spell_level);
				temp += temp / DND_WANDERER_SPELLEFFICIENCY;
				bufftimer += bufftimer / DND_WANDERER_SPELLEFFICIENCY;
			}
			else {
				sptr1 = StrParam(s:"Rally_Damage_Lvl", d:spell_level);
				sptr2 = StrParam(s:"Rally_Speed_Lvl", d:spell_level);
			}

			for(i = P_TIDSTART; i < P_TIDSTART + MAXPLAYERS; ++i) {
				if(fdistance(this, i) <= temp) {
					GiveActorInventory(i, sptr1, 1);
					GiveActorInventory(i, sptr2, 1);
					ACS_NamedExecuteAlways("DnD Spell Effects", 0, DND_SPELL_RALLY, i);
				}
			}
			
			Delay(bufftimer);
		break;
		case DND_SPELL_ICESHIELD:
			ActivatorSound("Spell/IceShieldCast", 127);
			temp = TEMPORARY_SPELL_TID + PlayerNumber();
			temp2 = ICESHIELD_HEALTHBASE + GetActorStat(this, STAT_LVL) * ICESHIELD_HEALTH_PER_LEVEL + GetActorIntellect(this) * ICESHIELD_HEALTH_PER_INT;
			bufftimer = ICESHIELD_BASE_DURATION + GetIntellect() * ICESHIELD_DURATION_PER_INT;
			
			if(CheckInventory("Wanderer_Perk50")) {
				temp2 += temp2 / DND_WANDERER_SPELLEFFICIENCY;
				bufftimer += bufftimer / DND_WANDERER_SPELLEFFICIENCY;
			}
			
			// requires a byte angle
			for(i = 0; i < ICESHIELD_COUNT; ++i) {
				SpawnForced("IceShield_Barrier", GetActorX(0), GetActorY(0), GetActorZ(0), temp, (i * 255) / ICESHIELD_COUNT);
				SetActivator(temp);
				SetPointer(AAPTR_TRACER, this);
				SetActorProperty(temp, APROP_HEALTH, temp2);
				Thing_ChangeTID(0, 0);
				SetActivator(this);
			}
			
			Delay(bufftimer);
			GiveInventory("IceShieldFadeSignal", 1);
		break;
		case DND_SPELL_POISONNOVA:
			ActivatorSound("Spell/PoisonNovaCast", 127);
			GiveInventory("PoisonNovaSpawner", 1);
		break;
		case DND_SPELL_MOLTENBOULDER:
			ActivatorSound("Spell/MoltenBoulderCast", 127);
			temp = TEMPORARY_SPELL_TID + PlayerNumber();
			if(CheckInventory("Wanderer_Perk50"))
				SpawnForced("MoltenBoulderProjectile_Longer", GetActorX(0), GetActorY(0), GetActorCeilingZ(0) - 64.0, temp, GetActorAngle(0) >> 8);
			else
				SpawnForced("MoltenBoulderProjectile", GetActorX(0), GetActorY(0), GetActorCeilingZ(0) - 64.0, temp, GetActorAngle(0) >> 8);
			SetActivator(temp);
			SetPointer(AAPTR_TARGET, this);
			SetActorVelocity(temp, MOLTENBOULDER_BASESPEED * cos(GetActorAngle(0)), MOLTENBOULDER_BASESPEED * sin(GetActorAngle(0)), 0, 0, 0);
			Thing_ChangeTID(temp, 0);
			SetActivator(this);
		break;
		case DND_SPELL_LIGHTNINGSPEAR:
			ActivatorSound("Spell/LightningSpearCast", 127);
			GiveInventory("LightningSpearSpawner", 1);
		break;
	}
	// modify to let more spells here to use cooldown, not just rally
	if(usesCooldown)
		ACS_NamedExecuteAlways("DnD Spell Cooldown", 0, spell_id, RALLY_COOLDOWN);
}

// can add cooldown reduction calculations for future as well
Script "DnD Spell Cooldown" (int spell_id, int cooldown) {
	int i;
	
	if(CheckInventory("Wanderer_Perk50"))
		cooldown -= cooldown / DND_WANDERER_COOLDOWNBONUS;
	
	SetInventory(SpellInfo[spell_id][SPELL_COOLDOWNCOUNTER], cooldown);
	for(i = 0; i < cooldown; ++i) {
		Delay(TICRATE);
		TakeInventory(SpellInfo[spell_id][SPELL_COOLDOWNCOUNTER], 1);
	}
	TakeInventory(SpellInfo[spell_id][SPELL_COOLDOWNITEM], 1);
}

Script "DnD Spell Effects" (int spell_id, int activator) CLIENTSIDE {
	SetActivator(activator);
	int cx, cy, cz, i, ang_off, nx, ny, nz, t_ang;
	int stimer = 0;
	switch(spell_id) {
		case DND_SPELL_RALLY:
			while(isAlive() && stimer <= RALLY_TIC_TIMER) {
				for(i = 0; i < RALLY_FX_COUNT && IsAlive() && stimer <= RALLY_TIC_TIMER; ++i) {
					cx = GetActorX(0);
					cy = GetActorY(0);
					cz = GetActorZ(0);
					ang_off = GetActorAngle(0);
					t_ang = i * 1.0 / RALLY_FX_COUNT;
					nx = cx + RALLY_R * FixedMul(1.0 - sin(t_ang / 4), cos(t_ang + ang_off));
					ny = cy + RALLY_R * FixedMul(1.0 - sin(t_ang / 4), sin(t_ang + ang_off));
					nz = cz + RALLY_R * sin(t_ang / 4); // 90 degree to guarantee max val
					Spawn("OrangeMagicSparkFX", nx, ny, nz);
					nx = cx + RALLY_R * FixedMul(1.0 - sin(t_ang / 4), cos(t_ang + ang_off + 0.5));
					ny = cy + RALLY_R * FixedMul(1.0 - sin(t_ang / 4), sin(t_ang + ang_off + 0.5));
					Spawn("OrangeMagicSparkFX", nx, ny, nz);
					stimer += 2;
					Delay(2);
				}
				stimer += 5;
				Delay(5);
			}
		break;
	}
}

Script "DnD Display Spell Cooldown" (int spell_id) CLIENTSIDE {
	Log(s:"\cc", s:SpellInfo[spell_id][SPELL_NAME], s: " Cooldown Left: ", d:CheckInventory(SpellInfo[spell_id][SPELL_COOLDOWNCOUNTER]));
}

Script "DnD Boulder Hit Check" (void) {
	int mx = GetActorX(0), my = GetActorY(0), mz = GetActorZ(0);
	Delay(1);
	if(!(mx - GetActorX(0)) && !(my - GetActorY(0)) && !(mz - GetActorZ(0)))
		GiveInventory("MoltenBoulderStop", 1);
}

Script "DnD LightningSpear Rip Retrieve" (void) {
	int res = LIGHTNINGSPEAR_BASE_RIP + GetActorIntellect(GetActorProperty(0, APROP_TARGETTID)) / LIGHTNINGSPEAR_INT_FACTOR;
	SetResultValue(res);
}

#endif