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

// Percent faster a minion recovers its abilities: Frenzied Followers. Read live by the counter below.
int GetMinionCooldownRate(int pnum) {
	return PlayerModData[pnum].vals[PSTAT_MINION_CDR];
}

// Spiritual Aid: a buff a minion gives its master, at the master's increased effect.
int ScaleMinionBuff(int pnum, int v) {
	return v * (100 + PlayerModData[pnum].vals[PSTAT_MINION_BUFFEFFECT]) / 100;
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

// GraveCaller: the dying enemy rises as the spell's zombie where it fell, at its killer's Summon: Zombie rank, 1 if not learned.
void HandleZombieRaiseOnDeath(int target) {
	int pnum = target - P_TIDSTART;
	int pet_tid = TEMPORARY_PET_TID + pnum;
	int this = ActivatorTID();
	if(SpawnForced(GetZombieClass(pnum), GetActorX(this), GetActorY(this), GetActorFloorZ(this), pet_tid)) {
		SetupZombiePet(pnum, pet_tid);
		ApplyPortalBoon(pnum, pet_tid);
	}
	SetActivator(this);
}

void CleanPetStuff() {
	SetInventory("PetCounter", 0);
	for(int i = 0; i < DND_MAX_PETPAINSHARE; ++i)
		TakeInventory(StrParam(s:"PetDamageReduction_", d:i + 1), 1);
}

// Knights, Death Knights and Slayer Knights: every kind Legion Mastery empowers.
bool IsKnightPet(int petid) {
	return petid == MONSTER_PET_KNIGHT || petid == MONSTER_PET_DEATHKNIGHT || petid == MONSTER_PET_SLAYERKNIGHT;
}

// The summoning spell's "more minion damage", looked up by pet kind. Carried on the pet at spawn
// so the damage scale can apply it without knowing which spell produced the thing it is scaling.
// rank 0 reads the spell's own rank; the spell panel passes the rank it previews.
int GetPetSpellDamageBonus(int pnum, int petid, int rank = 0) {
	int bonus = 0, spell = GetPetSummonSpell(petid);
	if(petid == MONSTER_PET_FIREDEMON)
		bonus = GetSpellValue(pnum, SPL_FIREDEMON, SPELLVAL_DAMAGE2, rank);
	// Legion Mastery's more damage, on every knight.
	else if(IsKnightPet(petid))
		bonus = GetSpellValue(pnum, SPL_LEGIONMASTERY, SPELLVAL_DAMAGE);

	// Commander: the summoning spell's gems, MORE on top of the row's own bonus.
	if(spell != -1)
		bonus = ApplySpellGemMore(pnum, spell, GEMSTAT_MORE_MINIONDAMAGE, 100 + bonus) - 100;
	return bonus;
}

int GetPetSummonSpell(int petid) {
	switch(petid) {
		case MONSTER_PET_FIREDEMON:		return SPL_FIREDEMON;
		case MONSTER_PET_ICEGOLEM:		return SPL_ICEGOLEM;
		case MONSTER_PET_ROCKGOLEM:		return SPL_ROCKGOLEM;
		case MONSTER_PET_ZOMBIE:		return SPL_ZOMBIE;
		case MONSTER_PET_KNIGHT:		return SPL_KNIGHT;
		case MONSTER_PET_DEATHKNIGHT:	return SPL_DEATHKNIGHT;
		case MONSTER_PET_GHAST:			return SPL_GHAST;
		case MONSTER_PET_SLAYERKNIGHT:	return SPL_SLAYERKNIGHT;
		case MONSTER_PET_DOOMGUARD:		return SPL_DOOMGUARD;
		case MONSTER_PET_DARKPORTAL:	return SPL_DARKPORTAL;
	}
	return -1;
}

// The pet kind a summon spell makes, or -1.
int GetSummonPetKind(int spell) {
	switch(spell) {
		case SPL_FIREDEMON:		return MONSTER_PET_FIREDEMON;
		case SPL_ICEGOLEM:		return MONSTER_PET_ICEGOLEM;
		case SPL_ROCKGOLEM:		return MONSTER_PET_ROCKGOLEM;
		case SPL_ZOMBIE:		return MONSTER_PET_ZOMBIE;
		case SPL_KNIGHT:		return MONSTER_PET_KNIGHT;
		case SPL_DEATHKNIGHT:	return MONSTER_PET_DEATHKNIGHT;
		case SPL_GHAST:			return MONSTER_PET_GHAST;
		case SPL_SLAYERKNIGHT:	return MONSTER_PET_SLAYERKNIGHT;
		case SPL_DOOMGUARD:		return MONSTER_PET_DOOMGUARD;
		case SPL_DARKPORTAL:	return MONSTER_PET_DARKPORTAL;
	}
	return -1;
}

// Pet cap points a pet takes: the heavier minions more, the Dark Portal none.
int GetPetCapWeight(int petid) {
	switch(petid) {
		case MONSTER_PET_GHAST:
		case MONSTER_PET_DEATHKNIGHT:	return 2;
		case MONSTER_PET_SLAYERKNIGHT:	return 3;
		case MONSTER_PET_DOOMGUARD:		return 5;
		case MONSTER_PET_DARKPORTAL:	return 0;
	}
	return 1;
}

// Pets that follow their master through a regular map exit: the untimed ones. Zombies and knights once a mastery
// takes their duration away; never the Dark Portal.
bool IsPetCarried(int pnum, int petid) {
	switch(petid) {
		case MONSTER_PET_ROCKGOLEM:
		case MONSTER_PET_ICEGOLEM:
		case MONSTER_PET_FIREDEMON:
		case MONSTER_PET_GHAST:
		case MONSTER_PET_SLAYERKNIGHT:
		case MONSTER_PET_DEATHKNIGHT:
		case MONSTER_PET_DOOMGUARD:
		return true;
		case MONSTER_PET_ZOMBIE:
		case MONSTER_PET_KNIGHT:
		return GetMinionLifeTics(pnum, GetPetSummonSpell(petid), 0) == 0;
	}
	return false;
}

// How many of a summon's pets may stand at once. The rows that say so (AMOUNT on the golems and the demon); zombies,
// knights and the heavy minions only the pet cap; one Dark Portal.
int GetSummonKindCap(int pnum, int spell) {
	switch(spell) {
		case SPL_ZOMBIE:
		case SPL_KNIGHT:
		case SPL_GHAST:
		case SPL_SLAYERKNIGHT:
		case SPL_DEATHKNIGHT:
		case SPL_DOOMGUARD:
		return DND_MAX_PETS_PER_PLAYER;
		case SPL_DARKPORTAL:
		return 1;
	}
	return Max(1, GetSpellValue(pnum, spell, SPELLVAL_AMOUNT));
}

// A pet's stored resists, once its traits are in: its spell's rank bonuses first, then the same trait rules as a monster's.
void InitPetResists(int tid, int pnum, int petid) {
	int m_id = tid - DND_PETTID_BEGIN, bonus = 0;
	if(petid == MONSTER_PET_FIREDEMON)
		bonus = DND_FIREDEMON_R5_RES * SpellThresholdMet(pnum, SPL_FIREDEMON, DND_SPELL_THRESH_LOW);
	else if(petid == MONSTER_PET_ROCKGOLEM && SpellThresholdMet(pnum, SPL_ROCKGOLEM, DND_SPELL_THRESH_HIGH)) {
		SetPetMonsterTrait(m_id, DND_ELEMENTAL_IMMUNE, true);
		SetActorInventory(tid, "DnD_ExplosiveResist", DND_ROCKGOLEM_R10_EXPLOSIVE);
	}
	else if(petid == MONSTER_PET_ZOMBIE)
		bonus = DND_ZOMBIE_R10_RES * SpellThresholdMet(pnum, SPL_ZOMBIE, DND_SPELL_THRESH_HIGH);

	// Dark Portal rank 10, for a minion summoned near it.
	if(CheckActorInventory(tid, "DnD_PortalBoon") & DND_PORTALBOON_RESIST)
		bonus += DND_DARKPORTAL_R10_RES;

	// Summon Mastery rank 10: Hardened Skin (rippers stop on it).
	if(SpellThresholdMet(pnum, SPL_SUMMONMASTERY, DND_SPELL_THRESH_HIGH)) {
		SetPetMonsterTrait(m_id, DND_HARDENED_SKIN, true);
		GiveActorInventory(tid, "MakeHardenedSkin", 1);
	}
	FillUnitResists(m_id, bonus, true);
}

// The script a summon spell casts, so a revived or carried minion comes back as its own kind.
str GetSpellSummonScript(int spell) {
	switch(spell) {
		case SPL_FIREDEMON:		return "DnD Summon Fire Demon";
		case SPL_ROCKGOLEM:		return "DnD Summon Rock Golem";
		case SPL_ZOMBIE:		return "DnD Summon Zombie";
		case SPL_KNIGHT:		return "DnD Summon Knight";
		case SPL_DEATHKNIGHT:	return "DnD Summon Death Knight";
		case SPL_GHAST:			return "DnD Summon Ghast";
		case SPL_SLAYERKNIGHT:	return "DnD Summon Slayer Knight";
		case SPL_DOOMGUARD:		return "DnD Summon Doomguard";
		case SPL_DARKPORTAL:	return "DnD Summon Dark Portal";
	}
	return "DnD Summon Ice Golem";
}

// this is mainly used to reset all cooldowns of spells when map ends, to prevent bugs
void ResetAllSpellCooldowns() {
	for(int i = 0; i < MAX_SPELLS; ++i)
		TakeInventory(SpellInfo[i][SPELL_COOLDOWNITEM], 1);
}

// Rally, from the Dead King's Banner; spell_id is a DND_SPELL_* value.
Script "DnD Cast Spell" (int spell_id, int usesCooldown) NET {
	int spell_level, i, temp, temp2, this = ActivatorTID();
	int pnum = PlayerNumber();
	int bufftimer = 0;
	switch(spell_id) {
		case DND_SPELL_RALLY:
			ActivatorSound("Spell/RallyCast", 127);
			spell_level = PlayerModData[pnum].vals[PSTAT_EX_ABILITY_RALLY];
			bufftimer = RALLY_DURATION * TICRATE;

			// Pack both halves of the spell into the one spare parameter:
			// damage percent in the low 16 bits, speed percent in the high 16.
			// Both level curves are resolved here, where their constants live.
			temp2 = Clamp_Between(spell_level, DND_SKILL_MINLEVEL, DND_SKILL_MAXLEVEL);
			temp = RALLY_DISTANCE + (temp2 - 1) * RALLY_DIST_PER_LEVEL;
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
	}
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

#endif
