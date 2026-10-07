#ifndef DND_SKILLS_IN
#define DND_SKILLS_IN

#include "DnD_CommonStat.h"
#include "DnD_SkillDef.h"
#include "Spells/DnD_SpellTables.h"
#include "Spells/DnD_SpellTree.h"
#include "Spells/DnD_Spells.h"

// ---- minion ability cooldowns -------------------------------------------------------------
//
// Held as an inventory COUNT on the minion and ticked down by a script, not as a powerup. A
// powerup fixes its duration the moment it is given, so a cooldown recovery rate could never
// affect one already running, and there would be nothing to read a rate off either.
//
// The count is in HUNDREDTHS OF A TIC. That is what lets the rate be a percentage: at 0 it steps
// 100 a tic and lands exactly on the base length, and a +50% rate steps 150 without a slow
// cooldown rounding away to nothing the way a per tic integer would.

// MUST match the DND_MINIONCD_* enum in DnD/Actors/Spells/Fire/FireDemon.dec, in order: DECORATE
// passes these numerically, the same contract the pet ids have.
enum {
	DND_MINIONCD_BURNINGGROUND
};

#define DND_MINIONCD_SCALE 100
#define DND_FIREDEMON_GROUNDCD 245    // tics, the demon's burning ground

str GetMinionCooldownItem(int which) {
	switch(which) {
		case DND_MINIONCD_BURNINGGROUND: return "Spell_FireDemon_BurningGround_Cooldown";
	}
	return "";
}

int GetMinionCooldownTics(int which) {
	switch(which) {
		case DND_MINIONCD_BURNINGGROUND: return DND_FIREDEMON_GROUNDCD;
	}
	return 0;
}

// Percent faster a minion recovers its abilities. Nothing grants it yet -- this is the seam that
// stat arrives through, and it is a function rather than a constant so adding one is a single
// edit here instead of a rework of the counter below.
int GetMinionCooldownRate(int pnum) {
	return 0;
}

// Started BY the minion, and runs on it. Gives the cooldown and then spends it.
Script "DnD Minion Cooldown" (int which) {
	str item = GetMinionCooldownItem(which);
	int tics = GetMinionCooldownTics(which);
	if(item == "" || tics <= 0)
		Terminate;

	// Set, not given: a second cast while one is still running restarts it rather than stacking two
	// lengths together. The DECORATE side already refuses to attack while any is left, so this only
	// matters if something else ever starts one.
	SetInventory(item, tics * DND_MINIONCD_SCALE);

	int pnum = GetActorProperty(0, APROP_MASTERTID) - P_TIDSTART;
	int step;

	while(IsAlive() && CheckInventory(item) > 0) {
		Delay(const:1);

		// Read every tic on purpose, so a rate gained or lost midway takes effect immediately --
		// which is the whole reason this is a counter and not a powerup.
		step = DND_MINIONCD_SCALE;
		if(pnum >= 0 && pnum < MAXPLAYERS)
			step += GetMinionCooldownRate(pnum);

		TakeInventory(item, Max(1, step));
	}
}

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

// One rate for every pet. The per kind switch is gone: it keyed off the id shifted out of the
// caller's damage argument, and every pet passes a plain number there, so every pet collected the
// zombie's figure regardless -- a bug that was invisible while the zombie was the only pet, and
// pointless to fix when the answer is the same for all of them anyway.
int GetPetDamageFactor(int master) {
	return GetIntellectEffect(master - P_TIDSTART, DND_PET_DMG_PER_INT);
}

// The summoning spell's "more minion damage", looked up by pet kind. Carried on the pet at spawn
// so the damage scale can apply it without knowing which spell produced the thing it is scaling.
int GetPetSpellDamageBonus(int pnum, int petid) {
	switch(petid) {
		case MONSTER_PET_FIREDEMON:
		return GetSpellValue(pnum, SPL_FIREDEMON, SPELLVAL_DAMAGE2);
	}
	return 0;
}

// this is mainly used to reset all cooldowns of spells when map ends, to prevent bugs
void ResetAllSpellCooldowns() {
	for(int i = 0; i < MAX_SPELLS; ++i)
		TakeInventory(SpellInfo[i][SPELL_COOLDOWNITEM], 1);
}

// expects spell_id 1 more than what it really is --- decorate uses that style
Script "DnD Cast Spell" (int spell_id, int usesCooldown) NET {
	int spell_level, i, temp, temp2, this = ActivatorTID();
	int pnum = PlayerNumber();
	//int spell = GetPlayerAllocatedSpell(spell_id);
	str sptr1, sptr2, bufftimer = 0;
	switch(spell_id) {
		case DND_SPELL_RALLY:
			ActivatorSound("Spell/RallyCast", 127);
			spell_level = PlayerModData[pnum].vals[PSTAT_EX_ABILITY_RALLY];
			temp = RALLY_DISTANCE + GetIntellectEffect(pnum, RALLY_DIST_PER_INT);
			bufftimer = RALLY_DURATION * TICRATE;

			// Pack both halves of the spell into the one spare parameter:
			// damage percent in the low 16 bits, speed percent in the high 16.
			// Both level curves are resolved here, where their constants live.
			temp2 = Clamp_Between(spell_level, DND_SKILL_MINLEVEL, DND_SKILL_MAXLEVEL);
			temp2 = (RALLY_BASEDAMAGE + (temp2 - 1) * RALLY_DAMAGEPERLVL) | ((8 + temp2) << 16);

			for(i = P_TIDSTART; i < P_TIDSTART + MAXPLAYERS; ++i) {
				if(fdistance(this, i) <= temp && IsPlayerBuffStateOK(i - P_TIDSTART)) {
					// The buff goes into THAT player's own buff array, not the caster's,
					// so players can rally each other. BTI_RALLY is NODUPLICATE_STRICT,
					// so a second caster does not stack: the stronger cast wins and a
					// weaker one is ignored.
					//
					// One call carries damage AND speed -- BTI_RALLY issues the speed
					// node itself, so there is no external PowerSpeed dependency.
					HandlePlayerBuffAssignment(i - P_TIDSTART, this, BTI_RALLY, 0, 0, 0, temp2);

					ACS_NamedExecuteAlways("DnD Spell Effects", 0, DND_SPELL_RALLY, i);
				}
			}
			
			Delay(bufftimer);
		break;
		case DND_SPELL_ICESHIELD:
			ActivatorSound("Spell/IceShieldCast", 127);
			temp = TEMPORARY_SPELL_TID + PlayerNumber();
			temp2 = ICESHIELD_HEALTHBASE + GetActorLevel(this) * ICESHIELD_HEALTH_PER_LEVEL + GetIntellectEffect(pnum, ICESHIELD_HEALTH_PER_INT);
			bufftimer = ICESHIELD_BASE_DURATION + GetIntellectEffect(pnum, ICESHIELD_DURATION_PER_INT);
			
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
			SpawnForced("MoltenBoulderProjectile", GetActorX(0), GetActorY(0), GetActorCeilingZ(0) - 64.0, temp, GetActorAngle(0) >> 8);
			SetActivator(temp);
			SetPointer(AAPTR_TARGET, this);
			SetActorProperty(0, APROP_TARGETTID, this);
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
	
	SetInventory(SpellInfo[spell_id][SPELL_COOLDOWNCOUNTER], cooldown);
	for(i = 0; i < cooldown; ++i) {
		Delay(const:TICRATE);
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
	int pnum = GetActorProperty(0, APROP_TARGETTID) - P_TIDSTART;
	int res = LIGHTNINGSPEAR_BASE_RIP + GetIntellect(pnum) / LIGHTNINGSPEAR_INT_FACTOR;
	SetResultValue(res);
}

Script "DnD Spell Buff Ticking" (int spell_id, int time, int period) {
	time /= period;
	while(time && isAlive()) {
		Delay(period);
		--time;
	}
	
	// times out, take away buffs
	// (nothing here needs a timer at the moment -- buff-backed spells expire on the
	//  buff ticker instead, which is what BTI_RALLY / BTI_RALLY_SPEED rely on)
	switch(spell_id) {
	}
}

#endif
