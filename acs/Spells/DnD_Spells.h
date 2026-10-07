#ifndef DND_SPELLS_IN
#define DND_SPELLS_IN

#include "DnD_SpellTree.h"

// What each spell actually DOES, split out from DnD_Skills.h so the legacy skill system and this one
// stop sharing a file. The layers underneath are elsewhere and this file should stay free of them:
//
//   DnD_SpellDefs.h    ids, ranks, the per spell record and its accessors
//   DnD_SpellTables.h  the numbers -- every base and per rank value, requirements, synergies
//   DnD_SpellCast.h    the machinery: cooldowns, mana gates, spawning, aiming, hitscans
//   DnD_SpellDamage.h  the damage pipeline entry points a spell actor calls
//   DnD_Mana.h         the pool and its reservations
//   DnD_SpellHud.h     the hotbar, its mirrors and the in game bars
//   DnD_SpellTree.h    the menu pages
//
// A new spell adds a case to "DnD Spell Cast" and, if it needs one, a script of its own here. Numbers
// belong in the tables, never inline -- GetSpellValue is the only way to read one.

// Binds whatever the spell tree is hovering to this slot. Re-checked here rather than trusted:
// the cvar was written on a client, against a page that may already be stale.
Script "DnD Bind Hotbar" (int slot) NET {
	int pnum = PlayerNumber();
	if(slot < 0 || slot >= GetHotbarSlotCount(pnum))
		Terminate;

	int spell = GetUserCVar(pnum, "dnd_hoveredspell");

	// An empty hover clears the slot, which is the only way to unbind one.
	if(spell < 0 || spell >= MAX_SPELL_IDS) {
		SetHotbarSpell(pnum, slot, -1);
		SyncHotbarSlot(pnum, slot);
		Terminate;
	}

	// Passives and auras never go on the bar -- they work from allocation or a tree toggle.
	if(!GetSpellAllocatedRank(pnum, spell) || (SpellDefs[spell].flags & (SPLF_PASSIVE | SPLF_AURA)))
		Terminate;

	SetHotbarSpell(pnum, slot, spell);
	SyncHotbarSlot(pnum, slot);
	LocalAmbientSound("RPG/MenuChoose", 127);
}

Script "DnD Cast Hotbar" (int slot) NET {
	TryCastHotbarSlot(PlayerNumber(), slot);
}


// ---- shared helpers ----------------------------------------------------------------------------

// Seconds from a spell's 16.16 table value, in tics.
int GetSpellDurationTics(int pnum, int spell, int which = SPELLVAL_DURATION) {
	return (GetSpellValue(pnum, spell, which) * TICRATE) >> 16;
}

// The buff on the caster, and at rank 10 on every living ally inside the spell's radius. Allies are
// other PLAYERS -- the radius values on these spells are written for a party, and a buff node can
// only live on a player anyway.
void GiveSpellBuffAround(int pnum, int spell, int bti, int val, int dur, bool allies) {
	int ptid = pnum + P_TIDSTART;
	HandlePlayerBuffAssignment(pnum, ptid, bti, 0, 0, dur, val);

	if(!allies)
		return;

	// Grown by the caster's area modifiers, so the reach matches the ring the aura FX draws.
	int r = ScalePlayerAoERadius(pnum, GetSpellValue(pnum, spell, SPELLVAL_RADIUS) << 16,
		DND_AOESRC_NONWEAPON);
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(i == pnum || !PlayerInGame(i) || !IsActorAlive(i + P_TIDSTART))
			continue;
		if(fdistance(ptid, i + P_TIDSTART) <= r)
			HandlePlayerBuffAssignment(i, ptid, bti, 0, 0, dur, val);
	}
}

// Who a SPLF_TARGETED support spell lands on: the ally under the crosshair, or the caster when that
// is nobody. PickActor rather than the aim puff, because this answers in the caller's own call and
// the puff cannot report before its first tick -- and because it needs an ACTOR, which is the one
// thing PickActor gives directly.
int GetSpellSupportTarget(int pnum) {
	int caster = pnum + P_TIDSTART;
	int t = PickActor(caster, GetActorAngle(caster), GetActorPitch(caster),
		DND_SPELL_HITSCANRANGE, 0, MF_SHOOTABLE, ML_BLOCKEVERYTHING, PICKAF_RETURNTID);

	// Player tids are one contiguous band, so membership of it is the whole test.
	int i = t - P_TIDSTART;
	if(i >= 0 && i < MAXPLAYERS && i != pnum && PlayerInGame(i) && IsActorAlive(t))
		return i;

	return pnum;
}

// Every aura the player has switched on, refreshed. Auras carry no duration of their own, so they are
// granted for a little longer than this pass's own period and simply lapse when it stops renewing
// them -- which is what makes switching one off take effect without a teardown path per aura.
#define DND_SPELLAURA_REFRESH (2 * TICRATE)

void RefreshSpellAuras(int pnum) {
	bool anger = IsSpellActive(pnum, SPL_ANGER);
	if(anger)
		ApplyAngerAura(pnum, DND_SPELLAURA_REFRESH);

	// Same pass raises and drops the visual. This runs every second whether the aura is on or off,
	// which is exactly what a switchable effect needs -- there is no teardown path to forget.
	SetPlayerAttachment(pnum, DND_PLAYERFX_ANGERAURA, anger);
}

// Heart of Fire. Finishing a fire spell's cast gives every OTHER fire spell on the hotbar that is on
// cooldown a chance to lose a slice of its MAXIMUM cooldown -- of the maximum, not of what is left, so
// the cut is worth the same whether it lands early in the cooldown or late.
//
// Hotbar only, as the text says: it refreshes what the player has bound, not everything they own.
void CheckHeartOfFire(int pnum, int cast_spell) {
	if(!IsSpellActive(pnum, SPL_HEARTOFFIRE) || SpellDefs[cast_spell].tree != DND_SKILLTREE_FIRE)
		return;

	int chance = GetSpellValue(pnum, SPL_HEARTOFFIRE, SPELLVAL_DAMAGE2);

	// "Chance to proc becomes 100%."
	if(SpellThresholdMet(pnum, SPL_HEARTOFFIRE, DND_SPELL_THRESH_HIGH))
		chance = 100;

	// 16.16 percent taken to hundredths, so the half a point per rank is not lost to the shift.
	int cut_pct = (GetSpellValue(pnum, SPL_HEARTOFFIRE, SPELLVAL_DAMAGE) * 100) >> 16;
	bool primes = SpellThresholdMet(pnum, SPL_HEARTOFFIRE, DND_SPELL_THRESH_LOW);
	int window = GetSpellDurationTics(pnum, SPL_HEARTOFFIRE);

	int i, s, cut;
	for(i = 0; i < MAX_HOTBAR_SLOTS; ++i) {
		s = GetHotbarSpell(pnum, i);

		// "other fire spells" -- not the one just cast, which TryCastSpell has already put on cooldown.
		if(s < 0 || s == cast_spell || SpellDefs[s].tree != DND_SKILLTREE_FIRE)
			continue;

		if(!IsSpellOnCooldown(pnum, s) || chance < random(1, 100))
			continue;

		cut = GetSpellCooldownTics(pnum, s) * cut_pct / 10000;
		if(cut <= 0)
			continue;

		ReduceSpellCooldown(pnum, s, cut);

		// "A fire spell that BECOMES READY with this" -- only the ones this cut actually finished off,
		// so a proc that merely shortens a long cooldown grants nothing.
		if(primes && !IsSpellOnCooldown(pnum, s))
			GetSpellPrimes().ends[pnum][s] = Timer() + window;

		// The bar reads its cooldown from a pushed word, so it has to be told the number moved.
		SyncHotbarForSpell(pnum, s);
	}
}

// Searing Bond. Run from the projectile's Death state, so the activator is the bolt: +HITTRACER on
// DnD_ExplosiveBase means its tracer is whatever it struck, which is the one thing that knows the
// victim -- the explosion itself is radius 0 below rank 10 and hits nobody.
Script "DnD Searing Bond Snare" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	// Read off the bolt before the activator moves to what it hit.
	int bx = GetActorX(0), by = GetActorY(0), bz = GetActorZ(0);
	bool binds_area = SpellThresholdMet(pnum, SPL_SEARINGBOND, DND_SPELL_THRESH_HIGH);

	int tics = (GetSpellValue(pnum, SPL_SEARINGBOND, SPELLVAL_DURATION) * TICRATE) >> 16;

	// "+2 seconds snare duration."
	if(SpellThresholdMet(pnum, SPL_SEARINGBOND, DND_SPELL_THRESH_LOW))
		tics += 2 * TICRATE;

	int victim = 0;
	if(SetActivator(0, AAPTR_TRACER))
		victim = ActivatorTID();

	if(victim && IsActorAlive(victim)) {
		SnareMonster(victim, tics);
		ACS_NamedExecuteAlways("DnD Snare FX", 0, victim);
	}

	// "Also binds enemies within 160 units of the target." Measured from the bolt rather than from the
	// victim, because a bolt that struck geometry has no victim to measure from and should still bind.
	if(binds_area) {
		int r = GetSpellValue(pnum, SPL_SEARINGBOND, SPELLVAL_RADIUS) << 16;
		int i, mn, s;

		for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
			i = UsedMonsterTIDs[mn];
			if(i == victim || !IsActorAlive(i) || !CheckFlag(i, "SHOOTABLE"))
				continue;

			if(fdistance_delta(bx - GetActorX(i), by - GetActorY(i), bz - GetActorZ(i)) <= r) {
				SnareMonster(i, tics);
				ACS_NamedExecuteAlways("DnD Snare FX", 0, i);
			}
		}
	}

	SetResultValue(0);
}

// ---- Scorching Ray ------------------------------------------------------------------------------
//
// Five parallel hitscans in an X -- one down the middle, one at each corner of the beam's cross
// section -- so the beam has real WIDTH instead of being a line. They are fired from helper actors
// placed at those offsets, because a trace always leaves its own actor's centre.
//
// The rays deal NO damage. They are detection only: each reports what it struck, the results are
// merged, and the damage is dealt once per monster afterwards. That is what stops a monster standing
// in the middle of the beam taking all five hits.
#define DND_SCORCHRAY_RAYS 5
#define DND_SCORCHRAY_BASELEN 512.0
#define DND_SCORCHRAY_BASEWIDTH 40.0
#define DND_SCORCHRAY_TICKRATE 7            // 0.2s between damage ticks, as the text says
#define DND_SCORCHRAY_EXPOSEAT (TICRATE * 5 / 2)    // 2.5s in, Fire Exposure starts landing
#define DND_SCORCHRAY_EXPOSEPCT 25
#define DND_SCORCHRAY_WIDTHR5 25            // percent wider at rank 5
#define DND_SCORCHRAY_MAXPIERCE 8           // how far the rank 10 beam re-traces past what it hits
#define DND_SCORCHRAY_MAXHITS (DND_SCORCHRAY_MAXPIERCE * DND_SCORCHRAY_RAYS)
#define DND_SCORCHRAY_MAXTRAILS 64          // hard ceiling on one tic's worth of beads
#define DND_SCORCHRAY_TRAILSTEP 20.0        // spacing between beads

// How far below the eye the beam sits. ONE constant for all three of the places that need it -- the
// damage traces, the trace that finds the wall, and the drawn beads -- because they are three
// descriptions of the same line and any gap between them is a lie somewhere: beads that miss where
// the damage is, or a beam drawn to the wrong length.
//
// They were 5, 5 and 8 before, which is how a deliberate 8 unit drop applied to two of them left all
// three disagreeing.
#define DND_SCORCHRAY_ZOFF 16.0

// Length and width both take the AREA stat, and deliberately the NON-WEAPON source: the design says
// area modifiers count here when they are not limited to attacks, and DND_AOESRC_WEAPON is precisely
// the one that is. GetSpellValue has already folded PSTAT_SPELL_AOE into the RADIUS field.
int GetScorchRayLength(int pnum) {
	return ScalePlayerAoERadius(pnum,
		DND_SCORCHRAY_BASELEN + (GetSpellValue(pnum, SPL_SCORCHINGRAY, SPELLVAL_RADIUS) << 16),
		DND_AOESRC_NONWEAPON);
}

int GetScorchRayWidth(int pnum) {
	int w = DND_SCORCHRAY_BASEWIDTH;

	// "+25% beam width."
	if(SpellThresholdMet(pnum, SPL_SCORCHINGRAY, DND_SPELL_THRESH_LOW))
		w = w * (100 + DND_SCORCHRAY_WIDTHR5) / 100;

	return ScalePlayerAoERadius(pnum, w, DND_AOESRC_NONWEAPON);
}

// Just the merge list for one tick's worth of rays.
typedef struct {
	int hit[DND_SCORCHRAY_MAXHITS];
	int count;
} scorchray_T;

scorchray_T module& GetScorchRay() {
	static scorchray_T s;
	return s;
}

// One ray of the five. Traced from a helper at the offset, and at rank 10 re-traced from just past
// whatever it hit so the beam carries on through -- a hitscan stops at the first thing it meets, and
// "passes through all enemies" is the rank 10.
//
// Victims are appended to the shared list rather than damaged here, so the merge can dedupe them.
void TraceScorchRay(int pnum, int offx, int offy, int offz, int len, bool pierces) {
	scorchray_T module& sr = GetScorchRay();

	int caster = pnum + P_TIDSTART;
	int helper = PROJECTILE_HELPER_TID + pnum;
	int puff = TEMPORARY_SPELL_TID + pnum;
	int a = GetActorAngle(caster), p = GetActorPitch(caster);

	// Pushed out past the caster's own hitbox first. P_LineAttack ignores the actor that FIRED the
	// shot, and the shooter here is the helper, not the player -- so a trace starting at the player's
	// feet hits the PLAYER, stops dead, and the ray reports nothing. The range loses the same amount so
	// the beam still reaches as far as it should.
	int nose = GetActorProperty(caster, APROP_RADIUS) + 8.0;
	int fx = FixedMul(cos(a), cos(p)), fy = FixedMul(sin(a), cos(p)), fz = -sin(p);

	int ox = GetActorX(caster) + offx + FixedMul(nose, fx);
	int oy = GetActorY(caster) + offy + FixedMul(nose, fy);
	int oz = GetActorZ(caster) + GetActorViewHeight(caster) - DND_SCORCHRAY_ZOFF + offz +
		FixedMul(nose, fz);

	len -= nose;
	if(len <= 0)
		return;

	int hops = pierces ? DND_SCORCHRAY_MAXPIERCE : 1;
	int i, j, v, step, adv = 0;
	bool dupe;

	for(i = 0; i < hops; ++i) {
		if(!SpawnForced("Spell_TraceHelper", ox, oy, oz, helper, 0))
			return;

		SetActorAngle(helper, a);
		SetActorPitch(helper, p);

		// Zero damage: this is a ruler. The hit is dealt once, later, off the merged list.
		LineAttack(helper, a, p, 0, "Spell_ScorchRayPuff", "None", len - adv,
			FHF_NORANDOMPUFFZ | FHF_NOIMPACTDECAL, puff);

		v = FindHitscanVictim(puff);
		Thing_ChangeTID(puff, 0);

		// Released, NOT removed. The trace fired from this actor and the engine is still holding it
		// as the shot's source for the rest of the tic -- destroying it out from under that is a
		// dangling pointer. CreateProjectile and CreateHitscan both drop the tid the same way and
		// let ProjectileHelper expire on its own five tic state.
		Thing_ChangeTID(helper, 0);

		if(!v)
			return;

		// Already listed, by another ray or by an earlier hop of this one.
		dupe = false;
		for(j = 0; j < sr.count; ++j)
			if(sr.hit[j] == v)
				dupe = true;

		if(!dupe && sr.count < DND_SCORCHRAY_MAXHITS) {
			sr.hit[sr.count] = v;
			++sr.count;
		}

		if(!pierces)
			return;

		// Resume just past the far side of what was hit, so the next hop cannot pick it up again.
		step = GetActorProperty(v, APROP_RADIUS) + 8.0;
		adv += step;
		if(adv >= len)
			return;

		ox += FixedMul(step, fx);
		oy += FixedMul(step, fy);
		oz += FixedMul(step, fz);
	}
}

// The channel. Runs for the spell's duration rather than while a button is held -- the cast gate is
// IsWeaponIdleForCast and there is no held-input path through the hotbar.
// The channel. Loops EVERY TIC so the beam is redrawn where the player is currently looking -- it is
// a beam, not a volley, and at one draw per damage tick it visibly lagged the view and left stale
// segments hanging in the air behind a turn. Damage keeps its own slower cadence inside.
Script "DnD Scorching Ray" (int pnum) {
	scorchray_T module& sr = GetScorchRay();

	int caster = pnum + P_TIDSTART;
	bool pierces = SpellThresholdMet(pnum, SPL_SCORCHINGRAY, DND_SPELL_THRESH_HIGH);

	BeginSpellBusy(pnum);

	// Runs until something STOPS it, with no duration of its own: the button is let go, the mana runs
	// out, or the caster dies or leaves. SPELLVAL_DURATION is Fire Exposure's five seconds and has
	// nothing to do with how long the beam may be held -- reading it as a channel length put a third,
	// invented limit on the spell.
	//
	// Unbounded is safe because the mana exit always arrives: the drain is far larger than any regen,
	// so a finite pool is always spent. Every other exit is a break below.
	bool held = false;
	int i, t = 0, v, dealt, len, half, a, rx, ry;

	PlaySound(caster, "ScorchingRay/Start", CHAN_5);
	PlaySound(caster, "ScorchingRay/Loop", CHAN_7, 1.0, true);

	while(true) {
		if(!PlayerInGame(pnum) || !IsActorAlive(caster))
			break;

		// Held ends it; not yet held is forgiven until the grace window runs out.
		if(IsChannelHeld())
			held = true;
		else if(held || t >= DND_CHANNEL_GRACE) {
			break;
		}

		len = GetScorchRayLength(pnum);

		// Every tic, so the beam turns with the player. The second argument is how much wider than
		// base it is right now, which the beads scale themselves by.
		ACS_NamedExecuteAlways("DnD Scorching Ray FX", 0, GetScorchRayCutoff(pnum, len, t % DND_SCORCHRAY_TICKRATE),
			FixedDiv(GetScorchRayWidth(pnum), DND_SCORCHRAY_BASEWIDTH));

		// Damage on its own cadence, not the draw's. Written as a positive test rather than an early
		// `continue`, because t is advanced at the bottom of the loop now and skipping it would hang.
		if(!(t % DND_SCORCHRAY_TICKRATE)) {
			// Billed with the damage, one cost per instance -- the cost and the damage are quoted on
			// the same 0.2s period, so they are charged on the same one. Running dry ends the channel
			// exactly as letting go of the button does.
			if(!PayChannelTick(pnum, SPL_SCORCHINGRAY, !t))
				break;

			half = GetScorchRayWidth(pnum) / 2;
			sr.count = 0;

			// The X: centre, then the four corners of the cross section. rx/ry is the beam's own right,
			// a quarter turn off its facing, so the offsets stay square to it however the player looks.
			a = GetActorAngle(caster);
			rx = FixedMul(cos(a - 0.25), half);
			ry = FixedMul(sin(a - 0.25), half);

			TraceScorchRay(pnum, 0, 0, 0, len, pierces);
			TraceScorchRay(pnum, rx, ry, half, len, pierces);
			TraceScorchRay(pnum, rx, ry, -half, len, pierces);
			TraceScorchRay(pnum, -rx, -ry, half, len, pierces);
			TraceScorchRay(pnum, -rx, -ry, -half, len, pierces);

			// One hit per monster, however many rays found it.
			// Hoisted: it does not vary per target, and the buff lookup is a global read.
			int ray_dmg = ApplySpellIntScaling(pnum,
				ApplySpellMoreDamage(pnum, SPL_SCORCHINGRAY,
					GetSpellValue(pnum, SPL_SCORCHINGRAY, SPELLVAL_DAMAGE)));

			for(i = 0; i < sr.count; ++i) {
				v = sr.hit[i];
				if(!IsActorAlive(v))
					continue;

				dealt = HandleDamageDeal(caster, v, ray_dmg,
				DND_DAMAGETYPE_FIRE, SPL_SCORCHINGRAY, DND_DAMAGEFLAG_ISSPELL, 0, 0, 0, 0, true);
				if(dealt > 0) {
					// SkipHandle, not "Fire": HandleDamageDeal has already applied resists, and the handler's
					// skip list is what stops this second application re-running the whole pipeline on it.
					Thing_Damage2(v, dealt, "SkipHandle");
				}

				// the 2.5 expose timer
				if(t < DND_SCORCHRAY_EXPOSEAT)
					continue;
				else if(t == DND_SCORCHRAY_EXPOSEAT)
					PlaySound(caster, "ScorchingRay/Exposure", CHAN_5);

				ApplyFireExposure(v, DND_SCORCHRAY_EXPOSEPCT, GetSpellDurationTics(pnum, SPL_SCORCHINGRAY));
			}
		}

		Delay(const:1);
		++t;
	}

	StopSound(caster, CHAN_7);

	EndSpellBusy(pnum);
}

// How far the beam is allowed to be DRAWN: wherever a trace that ignores actors first meets
// geometry. Server side, like everything else here -- the Wanderer ray guesses at this by watching a
// Spawn fail, and asking the engine is exact.
int GetScorchRayCutoff(int pnum, int len, bool withSmoke) {
	int caster = pnum + P_TIDSTART;
	int puff = TEMPORARY_DATADUMMY_TID + pnum;
	int helper = PROJECTILE_HELPER_TID + pnum;

	int a = GetActorAngle(caster), p = GetActorPitch(caster);
	int fx = FixedMul(cos(a), cos(p)), fy = FixedMul(sin(a), cos(p)), fz = -sin(p);
	int nose = GetActorProperty(caster, APROP_RADIUS) + 8.0;

	// Measured from where the BEAM starts, so the drawn length neither stops short of the wall nor
	// runs past it.
	int ox = GetActorX(caster), oy = GetActorY(caster);
	int oz = GetActorZ(caster) + GetActorViewHeight(caster) - DND_SCORCHRAY_ZOFF;

	// Fired from a helper sitting ON the beam line, not by the caster. LineAttack leaves the SHOOTER's
	// own attack height and takes no origin, so a trace fired by the player left its marker well above
	// the beam it is marking. The damaging rays already go through a helper for the same reason, and
	// this one is placed and nosed forward exactly as they are.
	if(!SpawnForced("Spell_TraceHelper", ox + FixedMul(nose, fx), oy + FixedMul(nose, fy),
		oz + FixedMul(nose, fz), helper, 0))
		return len;

	SetActorAngle(helper, a);
	SetActorPitch(helper, p);

	if(!withSmoke)
		LineAttack(helper, a, p, 0, "Spell_ScorchRayMarker", "None", len - nose, FHF_NORANDOMPUFFZ | FHF_NOIMPACTDECAL, puff);
	else
		LineAttack(helper, a, p, 0, "Spell_ScorchRayMarker_WithFX", "None", len - nose, FHF_NORANDOMPUFFZ | FHF_NOIMPACTDECAL, puff);

	Thing_ChangeTID(helper, 0);

	int stop = fdistance_delta(GetActorX(puff) - ox, GetActorY(puff) - oy, GetActorZ(puff) - oz);
	Thing_ChangeTID(puff, 0);

	if(stop <= 0 || stop > len)
		stop = len;

	return stop;
}

// The beam, drawn clientside -- and ONLY drawn. It spawns CLIENTSIDEONLY trails and touches nothing
// else: the trace that found the wall, the tid it needed and the puff it left behind all belong to
// the server, and doing any of that from here creates server-owned actors on one machine only.
//
// Everything here comes off the ACTIVATOR, never off pnum + P_TIDSTART. Player tids are assigned
// server side only, so that expression is not a player on a client -- it reads as nothing, and every
// bead of the beam spawned at the world origin instead of in front of the caster. The server
// dispatches this with the caster as activator, and an activator IS resolvable on both sides.
// girth is how much wider than base the beam currently is, in 16.16 -- 1.0 when nothing is modifying
// it. The beads are scaled by it so the drawn beam thickens with the volume that is actually being
// hit: area modifiers are supposed to grow this in every direction, and scaling only the ray offsets
// widened what it struck while it went on looking exactly as thin as before.
Script "DnD Scorching Ray FX" (int stop, int girth) CLIENTSIDE {
	if(!isAlive())
		Terminate;

	// Built the same way "DnD Ray of Disintegration Trails" is, which is the one trail spawner in the
	// mod known to work: the direction from GetDirectionVector, the beads from plain Spawn.
	Vec3_T* v = GetDirectionVector(0);

	int ox = GetActorX(0), oy = GetActorY(0);
	int oz = GetActorZ(0) + GetActorViewHeight(0) - DND_SCORCHRAY_ZOFF;

	int d, n = 0;

	// One tid, reused and released for each bead in turn.
	int bead = TEMPORARY_DATADUMMY_TID + ConsolePlayerNumber();

	// Counted as well as measured: a bad stop would otherwise be an unbounded spawn loop, and this
	// runs every single tic.
	for(d = DND_SCORCHRAY_TRAILSTEP; d < stop && n < DND_SCORCHRAY_MAXTRAILS;
		d += DND_SCORCHRAY_TRAILSTEP, ++n) {

		// Forced: a bead is scenery and must not be dropped because something happens to be standing
		// where it goes -- the beam has to read as continuous through a crowd.
		//
		// Spawned onto a tid so its scale can be set, then released again before the next one takes
		// it. An actor's scale is not something Spawn can be told, and the beads are identical
		// otherwise, so this is the only per bead work in the loop.
		if(!SpawnForced("Spell_ScorchRayTrail", ox + FixedMul(v.x, d), oy + FixedMul(v.y, d),
			oz + FixedMul(v.z, d), bead, 0))
			continue;

		if(girth != 1.0) {
			SetActorProperty(bead, APROP_SCALEX, FixedMul(GetActorProperty(bead, APROP_SCALEX), girth));
			SetActorProperty(bead, APROP_SCALEY, FixedMul(GetActorProperty(bead, APROP_SCALEY), girth));
		}

		Thing_ChangeTID(bead, 0);
	}

	bcs::free(v);
	SetResultValue(0);
}

// ---- the spells --------------------------------------------------------------------------------

// Warmth. DAMAGE is the percent and DAMAGE2 the flat half, which is 16.16 mana per second and so
// converts into the hundredths GetPlayerManaRegen works in. Both halves ride one buff -- the table
// case issues the flat node itself, the way Rally issues its speed half.
// "Duration becomes 12 seconds" at rank 5. An absolute value, so it is written as one.
#define DND_WARMTH_R5_DURATION (12 * TICRATE)

void CastWarmth(int pnum) {
	int pct = GetSpellValue(pnum, SPL_WARMTH, SPELLVAL_DAMAGE);
	int flat = (GetSpellValue(pnum, SPL_WARMTH, SPELLVAL_DAMAGE2) * DND_MANAREGEN_SCALE) >> 16;
	int dur = GetSpellDurationTics(pnum, SPL_WARMTH);

	// "Duration becomes 12 seconds" -- an absolute replacement, NOT a multiple of the base. It was
	// written as a doubling when the base was 5; the base is 8 now and that silently read as 16.
	if(SpellThresholdMet(pnum, SPL_WARMTH, DND_SPELL_THRESH_LOW))
		dur = DND_WARMTH_R5_DURATION;

	GiveSpellBuffAround(pnum, SPL_WARMTH, BTI_SPELL_WARMTH, (pct & 0xFFFF) | (flat << 16), dur,
		SpellThresholdMet(pnum, SPL_WARMTH, DND_SPELL_THRESH_HIGH));
}

// Heat Shield. DAMAGE is the armor rating. The ignite-on-being-hit half is carried by a marker item
// rather than by the buff, because the retaliation fires from the damage path and that path can read
// inventory far more cheaply than it can walk a buff list.
void CastHeatShield(int pnum) {
	// Whoever the soft lock settled on during the cast. Falls back to the caster if they died or
	// left while the bar was running.
	int target = GetSpellLockTarget(pnum);
	if(target < 0 || !PlayerInGame(target) || !IsActorAlive(target + P_TIDSTART))
		target = pnum;

	int dur = GetSpellDurationTics(pnum, SPL_HEATSHIELD);

	PlaySound(pnum + P_TIDSTART, "HeatShield/Cast", CHAN_6);
	PlaySound(target + P_TIDSTART, "HeatShield/Cast", CHAN_6);

	HandlePlayerBuffAssignment(target, pnum + P_TIDSTART, BTI_SPELL_HEATSHIELD, 0, 0, dur,
		GetSpellValue(pnum, SPL_HEATSHIELD, SPELLVAL_DAMAGE));

	// Tested BEFORE the stamp below: a recast on someone who already has the shield refreshes it, and
	// issuing the token again would leave a second pair of shields on them.
	bool wasUp = !!CheckActorInventory(target + P_TIDSTART, "DnD_HeatShieldRank");

	// Rank is stamped on the marker so the retaliation can read the caster's thresholds off the
	// WEARER, who may not be the caster.
	SetActorInventory(target + P_TIDSTART, "DnD_HeatShieldRank", GetSpellRank(pnum, SPL_HEATSHIELD, true));

	if(!wasUp)
		RaisePlayerAttachment(target, DND_PLAYERFX_HEATSHIELD);
	ACS_NamedExecuteAlways("DnD Heat Shield Timer", 0, target, Timer() + dur);
}

// Boiling Blood. Three effects on one duration: DAMAGE is the movement percent, CDR the recovery
// rate, and DAMAGE2 the health it costs every second -- which is a drain, not a buff, so it gets a
// ticker of its own.
void CastBoilingBlood(int pnum) {
	int dur = GetSpellDurationTics(pnum, SPL_BOILINGBLOOD);

	// "+2 seconds duration."
	if(SpellThresholdMet(pnum, SPL_BOILINGBLOOD, DND_SPELL_THRESH_LOW))
		dur += 2 * TICRATE;

	int cdr = GetSpellValue(pnum, SPL_BOILINGBLOOD, SPELLVAL_CDR) >> 16;

	// "+15% cooldown recovery rate" -- the same figure again, so it is read from the table rather
	// than written out a second time.
	if(SpellThresholdMet(pnum, SPL_BOILINGBLOOD, DND_SPELL_THRESH_HIGH))
		cdr *= 2;

	int ptid = pnum + P_TIDSTART;

	PlaySound(ptid, "BloodBoil/Cast", CHAN_5);

	HandlePlayerBuffAssignment(pnum, ptid, BTI_SPELL_BOILINGBLOOD, 0, 0, dur,
		GetSpellValue(pnum, SPL_BOILINGBLOOD, SPELLVAL_DAMAGE));
	HandlePlayerBuffAssignment(pnum, ptid, BTI_SPELL_BOILINGBLOOD_CDR, 0, 0, dur, cdr);

	ACS_NamedExecuteAlways("DnD Boiling Blood Drain", 0, pnum,
		GetSpellValue(pnum, SPL_BOILINGBLOOD, SPELLVAL_DAMAGE2), dur);
}

// Immolation. A real time toggle: the cast switches it on, a second cast switches it off, and it
// also stops on running dry or dying. NOT IsSpellToggleable -- that is the menu opt out, which is a
// build decision taken out of combat, and this spell deliberately does not have one.
//
// The running flag is raised HERE rather than in the ticker so a second press in the same breath
// cannot slip past it and start a second loop.
#define DND_IMMOLATION_R5_COSTCUT 4

void CastImmolation(int pnum) {
	// Activation only. A press that switches the spell OFF is answered by TryCastSpell and never
	// reaches the cast path at all, so there is nothing here to guard against.
	PlaySound(pnum + P_TIDSTART, "Immolation/Cast", CHAN_6);

	SetSpellRunning(pnum, SPL_IMMOLATION, true);
	ACS_NamedExecuteAlways("DnD Immolation Tick", 0, pnum);
}

Script "DnD Immolation Tick" (int pnum) {
	int ptid = pnum + P_TIDSTART;
	SetPlayerAttachment(pnum, DND_PLAYERFX_IMMOLATION, true);

	int i, mn, v, dealt, r, dmg, cost;
	bool first = true;

	while(true) {
		// Three ways out, and the flag covers two of them: a second cast clears it, and so does a
		// teardown elsewhere. Death and leaving are tested directly.
		if(!IsSpellRunning(pnum, SPL_IMMOLATION) || !PlayerInGame(pnum) || !IsActorAlive(ptid))
			break;

		cost = GetSpellValue(pnum, SPL_IMMOLATION, SPELLVAL_COST) >> 16;

		// "Mana cost reduced by 4."
		if(SpellThresholdMet(pnum, SPL_IMMOLATION, DND_SPELL_THRESH_LOW))
			cost = Max(0, cost - DND_IMMOLATION_R5_COSTCUT);

		// The press already bought the first second, the same way a channel's first tick is free.
		if(!first) {
			if(GetPlayerMana(pnum) < cost)
				break;
			SetPlayerMana(pnum, GetPlayerMana(pnum) - cost);
		}
		first = false;

		// One per damage pass, on its own channel so it never cuts the activation. Per monster would
		// stack a copy for every enemy in the ring.
		PlaySound(ptid, "Immolation/Proc", CHAN_7);

		r = ScalePlayerAoERadius(pnum, GetSpellValue(pnum, SPL_IMMOLATION, SPELLVAL_RADIUS) << 16,
			DND_AOESRC_NONWEAPON);
		dmg = ApplySpellIntScaling(pnum, ApplySpellMoreDamage(pnum, SPL_IMMOLATION,
			GetSpellValue(pnum, SPL_IMMOLATION, SPELLVAL_DAMAGE)));

		for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
			v = UsedMonsterTIDs[mn];
			if(!IsActorAlive(v) || !CheckFlag(v, "SHOOTABLE"))
				continue;
			if(fdistance(ptid, v) > r)
				continue;

			// Walls stop it. A burning aura is not a gas: distance alone had it cooking things through
			// the floor and in the next room, the same bug Annihilus had.
			if(!CheckSight(ptid, v, CSF_NOBLOCKALL))
				continue;

			// ISDAMAGEOVERTIME is what stops the tick rolling a crit, the same way Blaze's burn does.
			// A periodic aura attached to the player is damage over time by nature -- it would roll a
			// fresh crit several times a second otherwise, which no single hit ever gets to do.
			dealt = HandleDamageDeal(ptid, v, dmg, DND_DAMAGETYPE_FIRE, SPL_IMMOLATION,
				DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISRADIUSDMG | DND_DAMAGEFLAG_ISDAMAGEOVERTIME,
				0, 0, 0, 0, true);
			if(dealt > 0)
				Thing_Damage2(v, dealt, "SkipHandle");

			// "The damage also applies Blaze." Blaze's own burn, priced off Blaze.
			if(SpellThresholdMet(pnum, SPL_IMMOLATION, DND_SPELL_THRESH_HIGH))
				ACS_NamedExecuteAlways("DnD Blaze Burn", 0, v, pnum,
					GetSpellValue(pnum, SPL_BLAZE, SPELLVAL_DAMAGE));
		}

		Delay(const:TICRATE);
	}

	SetSpellRunning(pnum, SPL_IMMOLATION, false);
	SetPlayerAttachment(pnum, DND_PLAYERFX_IMMOLATION, false);
}

// The health price. Separate from the buff because the buff system grants values, it does not bill
// for them, and because this has to stop the moment the player dies.
Script "DnD Boiling Blood Drain" (int pnum, int per_second, int dur) {
	int ptid = pnum + P_TIDSTART;
	int endtic = Timer() + dur;

	// A recast takes OWNERSHIP rather than adding a second drain. The cooldown is shorter than the
	// duration, so recasting mid effect is the normal case and not an edge one -- two loops billed the
	// health twice a second, and the first to finish tore the visual down under the second. Same end
	// tic stamp "DnD Heat Shield Timer" uses, for the same reason.
	SetActorInventory(ptid, "DnD_BoilingBloodEnd", endtic);

	// The visual rides this loop rather than the buff, so it lasts exactly as long as the spell is
	// being paid for -- and stops when the player dies partway through.
	SetPlayerAttachment(pnum, DND_PLAYERFX_BOILINGBLOOD, true);

	while(Timer() < endtic) {
		Delay(const:TICRATE);

		if(!PlayerInGame(pnum) || !IsActorAlive(ptid))
			break;

		// Superseded. The newer cast owns the drain and the visual now, so this one leaves without
		// billing and without tearing anything down.
		if(CheckActorInventory(ptid, "DnD_BoilingBloodEnd") != endtic)
			Terminate;

		// DegenPlayer, not Thing_Damage2. This is upkeep, not a hit: it should not roll pain, and it
		// should not be eaten by armour on the way in -- the drain was going in as damage and not
		// landing on health at all. Righteous Fire pays its own cost through the same primitive, and
		// that one visibly works.
		//
		// Never lethal either: DegenPlayer floors at one health, which is the rule this already had.
		DegenPlayer(pnum, per_second);
	}

	// Only the owner clears up, so a run superseded a tick before its own end cannot pull the visual
	// out from under the one still going.
	if(CheckActorInventory(ptid, "DnD_BoilingBloodEnd") == endtic) {
		SetActorInventory(ptid, "DnD_BoilingBloodEnd", 0);
		SetPlayerAttachment(pnum, DND_PLAYERFX_BOILINGBLOOD, false);
	}
}

// Owns the Heat Shield marker only -- the armor is the buff's and expires on its own ticker. The end
// tic is stamped on the wearer so a recast takes ownership and the older timer bows out, rather than
// clearing the marker out from under the newer shield.
Script "DnD Heat Shield Timer" (int pnum, int endtic) {
	int ptid = pnum + P_TIDSTART;
	SetActorInventory(ptid, "DnD_HeatShieldEnd", endtic);

	while(Timer() < endtic && IsActorAlive(ptid) && CheckActorInventory(ptid, "DnD_HeatShieldEnd") == endtic)
		Delay(const:TICRATE / 2);

	if(CheckActorInventory(ptid, "DnD_HeatShieldEnd") == endtic) {
		SetActorInventory(ptid, "DnD_HeatShieldRank", 0);
		SetActorInventory(ptid, "DnD_HeatShieldEnd", 0);

		PlaySound(pnum + P_TIDSTART, "HeatShield/End", CHAN_6);
	}
}

// Anger. An aura, so it has no duration of its own -- "DnD Spell Aura Upkeep" refreshes it while the
// aura is switched on and simply stops when it is switched off. Both halves are separate buff nodes
// for the same reason Rally's are: one node carries one value.
void ApplyAngerAura(int pnum, int dur) {
	bool allies = true;
	// DAMAGE is 16.16 here ("15.0, more fire damage"), unlike DAMAGE2 next to it which is a plain
	// percent. The buff case wants whole percent, as Rally's does.
	GiveSpellBuffAround(pnum, SPL_ANGER, BTI_SPELL_ANGER,
		GetSpellValue(pnum, SPL_ANGER, SPELLVAL_DAMAGE) >> 16, dur, allies);

	int ign = GetSpellValue(pnum, SPL_ANGER, SPELLVAL_DAMAGE2);

	// "+5% ignite chance."
	if(SpellThresholdMet(pnum, SPL_ANGER, DND_SPELL_THRESH_LOW))
		ign += 5;

	GiveSpellBuffAround(pnum, SPL_ANGER, BTI_SPELL_ANGER_IGNITE, ign, dur, allies);
}

// Has to match Spell_Fireball's own Speed: CreateProjectile writes this onto the actor it spawns, so
// the two disagreeing would leave DECORATE's figure silently overridden.
#define DND_FIREBALL_SPEED 30

// How far off aim the rank 5 and rank 10 extras go, as a fraction of a full turn -- 0.02 is about 7
// degrees, wide enough to read as a second shot without spraying.
#define DND_FIREBALL_SPREAD 0.02

// A quarter second, as Blaze's text promises. TICRATE/4 is 8.75, so this is the nearest whole tic.
#define DND_BLAZE_TICRATE 9

// Blaze's burn. Deliberately NOT the ignite subsystem: ignite prices its tick off the player's ignite
// stats and the weapon that caused it, which would throw away the "8 fire damage every 0.25 seconds"
// the spell actually promises. This ticks the spell's own number on the spell's own clock.
//
// base is passed in already scaled, because the caster has just run it through "DnD Spell Damage" for
// the hit and there is no reason to price it twice.
Script "DnD Blaze Burn" (int victim, int pnum, int base) {
	int ticks = (GetSpellValue(pnum, SPL_BLAZE, SPELLVAL_DURATION) * TICRATE) >> 16;

	// "Burns for 1 second longer."
	if(SpellThresholdMet(pnum, SPL_BLAZE, DND_SPELL_THRESH_LOW))
		ticks += TICRATE;

	ticks /= DND_BLAZE_TICRATE;
	if(ticks <= 0)
		Terminate;

	// Refresh rather than stack: a recast extends the burn and the stronger tick wins.
	if(base > CheckActorInventory(victim, "DnD_BlazeDamage"))
		SetActorInventory(victim, "DnD_BlazeDamage", base);

	SetActorInventory(victim, "DnD_BlazeTimer",
		Max(ticks, CheckActorInventory(victim, "DnD_BlazeTimer")));

	// Ownership is the SCRIPT REFCOUNT, never the timer: the timer is inventory and outlives the
	// script, so a stale one would trap the monster in the refresh branch for good -- lit and taking
	// nothing. Same rule, and the same reason, as DnD_IgniteScripts.
	if(CheckActorInventory(victim, "DnD_BlazeScripts"))
		Terminate;

	GiveActorInventory(victim, "DnD_BlazeScripts", 1);

	ACS_NamedExecuteAlways("DnD Spell Clientside FX", 0, SPL_BLAZE, victim);

	int dmg, source = pnum + P_TIDSTART;
	while(CheckActorInventory(victim, "DnD_BlazeTimer") && IsActorAlive(victim)) {
		// SHOOTABLE excludes a monster mid teleport, exactly as the ignite tic does.
		if(CheckFlag(victim, "SHOOTABLE")) {
			ACS_NamedExecuteAlways("DnD Spell Clientside FX", 0, SPL_BLAZE, victim, 1);
			// Priced as a DoT before it is dealt as one. GetGenericDoTDamage is what applies
			// PSTAT_DOT_FLAT, PSTAT_DOT_INCREASED, the DoT multi and ailment "more" -- none of which
			// "DnD Spell Damage" touches, so this adds the damage-over-time stats rather than repeating
			// anything. Recomputed per tick so a gear change mid burn is live, as the ignite tic is.
			//
			// It also carries INC_CRITFORDOT, and that one matters: that mod trades crit damage on hits
			// (GetCritModifier returns a flat 100) for crit folded into the DoT multi. Without this call
			// a player who took it would lose crit on Blaze's hit and gain nothing on its burn -- a
			// strict downgrade. The tick still never ROLLS a crit; the flag below is what stops that.
			//
			// wepid -1, not the spell id: this one indexes Player_Weapon_Infos, and -1 is its "no weapon"
			// path. That is a different argument from HandleDamageDeal's wepid, which takes the spell.
			dmg = ApplySpellIntScaling(pnum,
				GetGenericDoTDamage(pnum, CheckActorInventory(victim, "DnD_BlazeDamage"), victim, -1, true));

			// ISDAMAGEOVERTIME is what keeps the tick from rolling a crit -- HandleDamageDeal gates the
			// spell crit roll on its absence, the same as the ignite, poison and bleed tics do. wep_neg
			// is the trailing true: there is no weapon behind this, so a spell id in the weapon slot is
			// not to be read as one.
			dmg = HandleDamageDeal(source, victim, dmg,
				DND_DAMAGETYPE_FIRE, SPL_BLAZE, DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_NOPUSH,
				0, 0, 0, DND_ACTORFLAG_ISDAMAGEOVERTIME | DND_ACTORFLAG_PAINLESS, true);

			if(dmg > 0)
				Thing_Damage2(victim, dmg, "IgniteNoPain");
		}

		Delay(const:DND_BLAZE_TICRATE);
		TakeActorInventory(victim, "DnD_BlazeTimer", 1);
	}

	TakeActorInventory(victim, "DnD_BlazeScripts", 1);
	SetActorInventory(victim, "DnD_BlazeTimer", 0);
	SetActorInventory(victim, "DnD_BlazeDamage", 0);
}

// Hit visuals, dispatched from the server so every client draws its own. The spawners these hand out
// produce +CLIENTSIDEONLY actors, so giving the item server side spawned nothing anyone could see --
// which is why the burn looked damaging but invisible.
Script "DnD Spell Clientside FX" (int spell, int victim_tid, int extra) CLIENTSIDE {
	switch(spell) {
		case SPL_BLAZE:
			if(!extra)
				GiveActorInventory(victim_tid, "Spell_Blaze_HitFXSpawner", 1);
			else
				GiveActorInventory(victim_tid, "Spell_Blaze_HitFXSpawner_Tic", 1);
		break;
	}
	SetResultValue(0);
}

// Incinerate. A passive: a FIRE kill has a DAMAGE2 percent chance to burst the corpse for DAMAGE
// percent of that monster's own maximum health, to everything inside RADIUS.
void CheckIncinerateOnKill(int pnum, int victim, int dealt) {
	if(!IsSpellActive(pnum, SPL_INCINERATE))
		return;

	// Only an actual kill. This runs before the engine applies the blow, so the corpse is still alive
	// here and the test is against the health it is about to lose.
	if(GetActorProperty(victim, APROP_HEALTH) > dealt)
		return;

	if(GetSpellValue(pnum, SPL_INCINERATE, SPELLVAL_DAMAGE2) < random(1, 100))
		return;

	ACS_NamedExecuteAlways("DnD Incinerate Burst", 0, pnum, victim);
}

Script "DnD Incinerate Burst" (int pnum, int victim) {
	int m_id = victim - DND_MONSTERTID_BEGIN;
	if(m_id < 0 || m_id >= DND_MAX_MONSTERS)
		Terminate;

	// DAMAGE is 16.16 percent, so it is taken to hundredths of a percent first -- maxhp times the raw
	// 16.16 value would overflow on a big monster long before the shift undid it.
	int pct = (GetSpellValue(pnum, SPL_INCINERATE, SPELLVAL_DAMAGE) * 100) >> 16;
	int dmg = MonsterProperties[m_id].maxhp * pct / 10000;
	if(dmg <= 0)
		Terminate;

	int r = GetSpellValue(pnum, SPL_INCINERATE, SPELLVAL_RADIUS);

	// "Radius increases to 192 units."
	if(SpellThresholdMet(pnum, SPL_INCINERATE, DND_SPELL_THRESH_LOW))
		r = 192;
	r <<= 16;

	// The burst FX, on the corpse the proc fired on. Given rather than spawned here because the actor
	// is CLIENTSIDEONLY and this script is the server's -- the token is what crosses.
	GiveActorInventory(victim, "Spell_Incinerate_BurstFXSpawner", 1);

	int i, mn, out, source = pnum + P_TIDSTART;
	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		if(i == victim || !IsActorAlive(i) || !CheckFlag(i, "SHOOTABLE"))
			continue;
		if(fdistance(victim, i) > r)
			continue;

		out = HandleDamageDeal(source, i, dmg, DND_DAMAGETYPE_FIRE, SPL_INCINERATE,
			DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISRADIUSDMG, 0, 0, 0, 0, true);
		if(out > 0)
			Thing_Damage2(i, out, "SkipHandle");
	}

	// "The explosion no longer harms you" is the rank 10, so below it the caster is inside their own
	// blast. Half, as the mod's other self-damaging explosions do.
	if(!SpellThresholdMet(pnum, SPL_INCINERATE, DND_SPELL_THRESH_HIGH) &&
		IsActorAlive(source) && fdistance(victim, source) <= r)
		// Live type on purpose, unlike the hits above: this one never went through HandleDamageDeal,
		// so the caster's own fire resistance has still to be applied to it.
		Thing_Damage2(source, dmg / 2, "Fire");
}

// Pyroblast's rank 10: "The explosion erupts into smaller fireballs dealing an eighth of its damage."
// Eight of them, matching the fraction, thrown level and evenly around the blast.
// Has to match Spell_SearingBond's own Speed, as Fireball's does.
#define DND_SEARINGBOND_SPEED 40

// Likewise Spell_Pyroblast's.
#define DND_PYROBLAST_SPEED 24

// Likewise Spell_FireJet's.
#define DND_FIREJET_SPEED 36

// Matches the actor's own Speed and the description's "advances 30 units per tic".
#define DND_FLAMEPILLAR_SPEED 30

// One pass of the pillar's SpawnState: a 4 tic frame and a 0 tic A_Countdown. Its life is counted
// in those passes, so the duration row has to be divided by this to become a ReactionTime.
#define DND_FLAMEPILLAR_LOOPTICS 4

// Matches the old MOLTENBOULDER_BASESPEED. The boulder drops from the ceiling with this as its
// horizontal push, then takes its velocity over from its own states.
#define DND_MOLTENBOULDER_SPEED 20

// How far under the ceiling it appears, as the old cast placed it.
#define DND_MOLTENBOULDER_DROP 64.0

// "Lasts 3 seconds longer." Added at the cast like every other threshold duration bonus --
// GetSpellDurationTics is the raw table value and knows nothing about ranks.
#define DND_MOLTENBOULDER_R5_BONUS (3 * TICRATE)

// Rank 10: "Sends six Pyroblasts outward when it shatters."
// "You gain 25% additional fire resistance against this spell." Against THIS spell, so it is
// applied to the self burn here rather than added to the player's fire resistance generally.
#define DND_RIGHTEOUSFIRE_R5_RESIST 25

// The burn hum. Held for as long as the aura is. CHAN_6 keeps it off CHAN_7, where Scorching Ray
// holds its own loop and calls StopSound when it ends -- that would have killed this one
// outright rather than merely interrupting it.
#define DND_RIGHTEOUSFIRE_CHAN CHAN_6

// The spell damage buff is renewed every second, so it only has to outlast the gap between two
// renewals. Two seconds, so a tick arriving late never lets it lapse mid burn.
#define DND_RIGHTEOUSFIRE_BUFFTICS (2 * TICRATE)

#define DND_MOLTENBOULDER_SHARDS 6
#define DND_MOLTENBOULDER_SHARDSPEED 24

// Rank 10's second pillar. 32 units to each side, so the pair runs 64 apart.
#define DND_FLAMEPILLAR_SIDEOFF 32.0

#define DND_PYROBLAST_FRAGMENTS 8
#define DND_PYROBLAST_FRAGSPEED 24

// Run FROM the Pyroblast's own state, so the activator is the projectile and already carries both the
// owner and user_spellid. The children are stamped with that same pair, which is what lets their
// Damage expression resolve as Pyroblast -- without it they would read user_spellid 0 and price
// themselves as Blaze.
Script "DnD Pyroblast Burst" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	int spell = GetUserVariable(0, "user_spellid");
	int x = GetActorX(0), y = GetActorY(0), z = GetActorZ(0);
	int tid = TEMPORARY_SPELL_TID + pnum;
	int i, a;

	for(i = 0; i < DND_PYROBLAST_FRAGMENTS; ++i) {
		if(!SpawnForced("Spell_Pyroblast_TinyFireballs", x, y, z, tid, 0))
			continue;

		SetupSpellActor(tid, pnum, spell);

		a = (1.0 * i) / DND_PYROBLAST_FRAGMENTS;
		SetActorAngle(tid, a);
		SetActorVelocity(tid, DND_PYROBLAST_FRAGSPEED * cos(a), DND_PYROBLAST_FRAGSPEED * sin(a), 0,
			false, false);

		// Released before the next one takes the same tid.
		Thing_ChangeTID(tid, 0);
	}

	SetResultValue(0);
}

// Infernal Strike. Arms the NEXT melee swing rather than doing anything on cast, so the charge is an
// item the melee path can test for a few instructions. Rank rides with it because the thresholds have
// to resolve when the swing lands, not when it was armed.
// "Enemies in a 256 unit area take -30% fire resistance for 8 seconds and gain 25% base chance to be
// ignited." SPLF_TARGETED, so the area is centred on what the player is aiming at rather than on the
// caster -- the aim trace always lands a puff, so an empty sky still gives a point at max range.
#define DND_FLAMMABILITY_R5_AOE 25

int GetFlammabilityRadius(int pnum) {
	// "25% increased area of effect" -- increased, so it joins the additive pool.
	return ScalePlayerAoERadius(pnum, GetSpellValue(pnum, SPL_FLAMMABILITY, SPELLVAL_RADIUS) << 16,
		DND_AOESRC_NONWEAPON,
		DND_FLAMMABILITY_R5_AOE * SpellThresholdMet(pnum, SPL_FLAMMABILITY, DND_SPELL_THRESH_LOW));
}

// The ring drawn where the curse lands. CLIENTSIDE because the markers are CLIENTSIDEONLY -- spawning
// one from the server produces nothing on anybody's screen.
//
// Centre and radius arrive as arguments rather than being recomputed here: the radius comes off the
// CASTER's area mods and rank, which a client cannot ask for on someone else's behalf.
//
// Dispatched with NamedExecuteWithResult, not NamedExecuteAlways: the four figures fit (Always
// carries three), and this is the call the attachment path already proves reaches a CLIENTSIDE
// script from a server side one.
#define DND_FLAMMABILITY_RINGSTEP 48.0
#define DND_FLAMMABILITY_RINGMIN 12
#define DND_FLAMMABILITY_RINGMAX 40
// Lifted off the centre's own z. A monster's origin is its FEET, so a ring drawn flat at that
// height sinks into the floor it is standing on.
#define DND_FLAMMABILITY_RINGLIFT 18.0

Script "DnD Flammability Area FX" (int cx, int cy, int cz, int r) CLIENTSIDE {
	if(r <= 0) {
		SetResultValue(0);
		Terminate;
	}

	// Scaled off the circumference so a widened area reads as a bigger ring rather than the same ring
	// with wider gaps between its markers.
	int n = Clamp_Between((FixedDiv(r, DND_FLAMMABILITY_RINGSTEP) >> 16) * 6,
		DND_FLAMMABILITY_RINGMIN, DND_FLAMMABILITY_RINGMAX);

	int i, a;
	for(i = 0; i < n; ++i) {
		a = (i * 1.0) / n;
		SpawnForced("FlammabilityAreaMarkerFX",
			cx + FixedMul(cos(a), r),
			cy + FixedMul(sin(a), r),
			cz + DND_FLAMMABILITY_RINGLIFT, 0, 0);
	}

	SetResultValue(0);
}

// Rain of Fire. Marks a patch of ground and drops comets into it for the duration.
//
// The comets are aimed rather than dropped straight down: a landing point is picked inside the
// scatter disc first, the comet is started high and to one side of it, and its velocity points at
// the landing point. That is what makes them fall at varied angles and still all land in the area.
#define DND_RAINOFFIRE_SPEED 40.0
#define DND_RAINOFFIRE_R5_SPEEDUP 20        // percent, "comets fall 20% faster"
#define DND_RAINOFFIRE_R5_MORECOMETS 15     // percent, "rains 15% more comets"
#define DND_RAINOFFIRE_RATE 6               // tics between volleys -- mid-high over a 4 second rain
#define DND_RAINOFFIRE_PERVOLLEY 2          // comets per volley. Density without a faster cadence.
#define DND_RAINOFFIRE_MARKRATE (TICRATE / 2)
#define DND_RAINOFFIRE_RISE 384.0           // how high they start when the ceiling is far away
#define DND_RAINOFFIRE_HEADROOM 24.0        // kept clear of the ceiling so they do not spawn inside it
// A share of every volley is aimed at an inner circle instead of the whole patch, so the fall has
// a dense heart rather than reading as even drizzle across the ground.
#define DND_RAINOFFIRE_COREPCT 40    // percent of comets aimed at it
#define DND_RAINOFFIRE_COREFRAC 0.5 // its radius, as a share of the full scatter

#define DND_RAINOFFIRE_SLANT 128.0          // how far to the side a comet may start from its landing point

// The ring, redrawn on a timer so it lasts the rain without the markers needing a duration.
Script "DnD Rain Of Fire Mark" (int cx, int cy, int packed) CLIENTSIDE {
	int cz = packed << 16;
	int r = (packed >> 16) << 16;
	if(r <= 0) {
		SetResultValue(0);
		Terminate;
	}

	int n = Clamp_Between((FixedDiv(r, DND_FLAMMABILITY_RINGSTEP) >> 16) * 6,
		DND_FLAMMABILITY_RINGMIN, DND_FLAMMABILITY_RINGMAX);

	int i, a;
	for(i = 0; i < n; ++i) {
		a = (i * 1.0) / n;
		SpawnForced("Spell_RainOfFireMarkerFX",
			cx + FixedMul(cos(a), r),
			cy + FixedMul(sin(a), r),
			cz + DND_FLAMMABILITY_RINGLIFT, 0, 0);
	}

	SetResultValue(0);
}

Script "DnD Rain Of Fire" (int pnum) {
	int caster = pnum + P_TIDSTART;
	int cx, cy, cz;

	// Same two ways in as Flammability: what the player is looking at, else where the aim puff struck.
	int target = PickActor(caster, GetActorAngle(caster), GetActorPitch(caster),
		DND_SPELL_HITSCANRANGE, 0, MF_SHOOTABLE, ML_BLOCKEVERYTHING, PICKAF_RETURNTID);

	if(target && IsActorAlive(target)) {
		cx = GetActorX(target);
		cy = GetActorY(target);
		cz = GetActorZ(target);
	}
	else {
		if(!TraceSpellAim(pnum, DND_SPELL_HITSCANRANGE))
			Terminate;

		int wait = 0;
		while(!ReadSpellAim(pnum) && wait < DND_SPELLAIM_WAIT) {
			Delay(const:1);
			++wait;
		}

		if(!SpellAimReady(pnum) || !PlayerInGame(pnum) || !IsActorAlive(caster))
			Terminate;

		auto aim = GetSpellAim();
		cx = aim.x[pnum];
		cy = aim.y[pnum];
		cz = aim.z[pnum];
	}

	// AMOUNT is the scatter radius -- the patch itself -- and grows with area modifiers. RADIUS is
	// each comet's own blast and is left to the explosion, which reads it off user_spellid.
	int scatter = ScalePlayerAoERadius(pnum,
		GetSpellValue(pnum, SPL_RAINOFFIRE, SPELLVAL_AMOUNT) << 16, DND_AOESRC_NONWEAPON);
	int dur = GetSpellDurationTics(pnum, SPL_RAINOFFIRE);

	int speed = DND_RAINOFFIRE_SPEED;

	// Both halves of rank 5 hang off this, so it is asked once.
	bool r5 = SpellThresholdMet(pnum, SPL_RAINOFFIRE, DND_SPELL_THRESH_LOW);

	// "Comets fall 20% faster."
	if(r5)
		speed = speed * (100 + DND_RAINOFFIRE_R5_SPEEDUP) / 100;

	// Floor and ceiling read ONCE from the middle rather than per comet: the patch is one room in
	// almost every case, and a dummy spawn for each of two dozen comets is not worth the difference.
	int probe = TEMPORARY_DATADUMMY_TID + pnum;
	int ground = cz, roof = cz + DND_RAINOFFIRE_RISE;

	if(SpawnForced("DnD_SpellAnchor", cx, cy, cz, probe, 0)) {
		ground = GetActorFloorZ(probe);
		roof = Min(GetActorCeilingZ(probe) - DND_RAINOFFIRE_HEADROOM, ground + DND_RAINOFFIRE_RISE);
		Thing_Remove(probe);
	}

	// A low ceiling would otherwise put the start point under the landing point.
	if(roof <= ground + DND_RAINOFFIRE_HEADROOM)
		roof = ground + DND_RAINOFFIRE_HEADROOM;

	int tid = TEMPORARY_SPELL_TID + pnum;
	int t, c, a, d, lx, ly, sx, sy, dx, dy, dz, len, reach, n;

	// Hundredths of a comet carried between volleys. "15% more" of a 2 comet volley is 0.3, which
	// no integer volley size can express -- rolling for it would make the rank a coin flip that
	// might never land, and raising PERVOLLEY or dropping RATE both overshoot badly at these sizes.
	// Accumulated, the extra arrives on schedule and the rain is exactly 15% heavier.
	int comet_acc = 0;

	for(t = 0; t < dur; t += DND_RAINOFFIRE_RATE) {
		if(!PlayerInGame(pnum))
			break;

		// Redrawn on its own slower cadence so the ring persists without a marker per comet.
		if(!(t % DND_RAINOFFIRE_MARKRATE))
			ACS_NamedExecuteWithResult("DnD Rain Of Fire Mark", cx, cy,
				((scatter >> 16) << 16) | ((ground >> 16) & 0xFFFF));

		// A VOLLEY, not a comet. Each one rolls its own landing point and its own slant, so they come
		// down together but never as a pair on the same line.
		n = DND_RAINOFFIRE_PERVOLLEY;
		if(r5) {
			comet_acc += DND_RAINOFFIRE_PERVOLLEY * DND_RAINOFFIRE_R5_MORECOMETS;
			n += comet_acc / 100;
			comet_acc %= 100;
		}

		for(c = 0; c < n; ++c) {
			// Rolled per comet, not per volley: a volley that went entirely to the middle or entirely
			// to the edge would read as two different spells taking turns.
			reach = scatter;
			if(random(1, 100) <= DND_RAINOFFIRE_COREPCT)
				reach = FixedMul(scatter, DND_RAINOFFIRE_COREFRAC);

			// Uniform over the disc rather than over the radius -- without the square root they bunch
			// up in the middle and the edge of the patch stays empty. Applied to whichever reach was
			// drawn, so the inner circle is evenly covered too and does not grow its own hot spot.
			a = random(0, 1.0);
			d = FixedMul(reach, fsqrt(random(0, 1.0)));
			lx = cx + FixedMul(cos(a), d);
			ly = cy + FixedMul(sin(a), d);

			// Started high and to one side, so it comes down at an angle and still lands on the spot.
			a = random(0, 1.0);
			d = random(0, DND_RAINOFFIRE_SLANT);
			sx = lx + FixedMul(cos(a), d);
			sy = ly + FixedMul(sin(a), d);

			if(!SpawnForced("Spell_RainOfFire", sx, sy, roof, tid, 0))
				continue;

			// Stamps the spell identity, which is what the Damage expression and the blast radius both
			// resolve off. Without it the comet prices itself as spell 0.
			SetupSpellActor(tid, pnum, SPL_RAINOFFIRE);

			dx = lx - sx;
			dy = ly - sy;
			dz = ground - roof;
			len = fdistance_delta(dx, dy, dz);

			if(len > 0)
				SetActorVelocity(tid,
					FixedMul(FixedDiv(dx, len), speed),
					FixedMul(FixedDiv(dy, len), speed),
					FixedMul(FixedDiv(dz, len), speed), false, false);

			// Released before the next comet of the volley takes the same scratch tid.
			Thing_ChangeTID(tid, 0);
		}

		Delay(const:DND_RAINOFFIRE_RATE);
	}
}

// Two ways to find the centre, because the curse has to land on a patch of floor as readily as on a
// monster. PickActor answers in the same call and exactly, so it is the fast path; failing that the
// aim puff tags itself where it struck geometry, which costs the few tics it takes the puff to tick.
Script "DnD Flammability Cast" (int pnum) {
	int caster = pnum + P_TIDSTART;
	int cx, cy, cz;

	PlaySound(caster, "Flammability/Cast", 6);

	int target = PickActor(caster, GetActorAngle(caster), GetActorPitch(caster),
		DND_SPELL_HITSCANRANGE, 0, MF_SHOOTABLE, ML_BLOCKEVERYTHING, PICKAF_RETURNTID);

	if(target && IsActorAlive(target)) {
		cx = GetActorX(target);
		cy = GetActorY(target);
		cz = GetActorZ(target);
	}
	else {
		if(!TraceSpellAim(pnum, DND_SPELL_HITSCANRANGE))
			Terminate;

		int wait = 0;
		while(!ReadSpellAim(pnum) && wait < DND_SPELLAIM_WAIT) {
			Delay(const:1);
			++wait;
		}

		if(!SpellAimReady(pnum) || !PlayerInGame(pnum) || !IsActorAlive(caster))
			Terminate;

		auto aim = GetSpellAim();
		cx = aim.x[pnum];
		cy = aim.y[pnum];
		cz = aim.z[pnum];
	}

	int r = GetFlammabilityRadius(pnum);
	int tics = GetSpellDurationTics(pnum, SPL_FLAMMABILITY);

	ACS_NamedExecuteWithResult("DnD Flammability Area FX", cx, cy, cz, r);
	int pct = GetSpellValue(pnum, SPL_FLAMMABILITY, SPELLVAL_DAMAGE);
	int chance = GetSpellValue(pnum, SPL_FLAMMABILITY, SPELLVAL_DAMAGE2);

	int i, mn;
	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		if(!IsActorAlive(i) || !CheckFlag(i, "SHOOTABLE"))
			continue;
		if(fdistance_delta(cx - GetActorX(i), cy - GetActorY(i), cz - GetActorZ(i)) > r)
			continue;

		ApplyFireExposure(i, pct, tics);

		// Strongest and longest win independently, same rule as the exposure lane beside it.
		if(CheckActorInventory(i, "DnD_Flammable") < chance)
			SetActorInventory(i, "DnD_Flammable", chance);

		// One ticker per monster -- it owns the marker, so a second would leave an orphan attachment.
		bool ticking = !!CheckActorInventory(i, "DnD_FlammableTimer");
		if(CheckActorInventory(i, "DnD_FlammableTimer") < tics)
			SetActorInventory(i, "DnD_FlammableTimer", tics);

		if(!ticking)
			ACS_NamedExecuteAlways("DnD Flammability Timer", 0, i);
	}

}

// Molten Boulder. The same spawn as the pre-tree version -- dropped from just under the ceiling at
// the caster's angle, pushed horizontally, then left to roll -- plus the two things the old cast
// could not do: SetupSpellActor stamps the spell identity its damagers now read back, and the
// lifetime comes off SPELLVAL_DURATION instead of the actor's hardcoded ReactionTime.
// Righteous Fire. A toggle, like Immolation: the press that switches it OFF is answered by
// TryCastSpell and never reaches here, so there is nothing to guard against.
void CastRighteousFire(int pnum) {
	SetSpellRunning(pnum, SPL_RIGHTEOUSFIRE, true);
	ACS_NamedExecuteAlways("DnD Righteous Fire Tick", 0, pnum);
}

void CastMoltenBoulder(int pnum) {
	int caster = pnum + P_TIDSTART;
	int tid = TEMPORARY_SPELL_TID + pnum;
	int a = GetActorAngle(caster);

	// Byte angle for SpawnForced, as everywhere else that spawns by hand.
	if(!SpawnForced("Spell_MoltenBoulder", GetActorX(caster), GetActorY(caster),
		GetActorCeilingZ(caster) - DND_MOLTENBOULDER_DROP, tid, a >> 8))
		return;

	SetupSpellActor(tid, pnum, SPL_MOLTENBOULDER_S);

	// A_CountDown runs about once a tic across the boulder's loops, so the tic count IS the countdown.
	int tics = GetSpellDurationTics(pnum, SPL_MOLTENBOULDER_S);
	if(SpellThresholdMet(pnum, SPL_MOLTENBOULDER_S, DND_SPELL_THRESH_LOW))
		tics += DND_MOLTENBOULDER_R5_BONUS;

	// At least one, so a zeroed row still produces a boulder rather than one that dies on spawn.
	SetActorProperty(tid, APROP_REACTIONTIME, Max(1, tics));

	SetActorVelocity(tid, DND_MOLTENBOULDER_SPEED * cos(a), DND_MOLTENBOULDER_SPEED * sin(a), 0,
		false, false);
	Thing_ChangeTID(tid, 0);
}

void CastInfernalStrike(int pnum) {
	SetActorInventory(pnum + P_TIDSTART, "DnD_InfernalStrikeRank",
		GetSpellRank(pnum, SPL_INFERNALSTRIKE, true));
}

// Spent by the melee hit in "DnD Event Handler". Returns nothing -- the damage it adds is dealt here
// rather than folded into the swing, so the fire half meets fire resistance on its own.
Script "DnD Infernal Strike Hit" (int pnum, int victim) {
	int ptid = pnum + P_TIDSTART;
	if(!CheckActorInventory(ptid, "DnD_InfernalStrikeRank"))
		Terminate;

	SetActorInventory(ptid, "DnD_InfernalStrikeRank", 0);

	int dmg = ApplySpellIntScaling(pnum,
		GetSpellValue(pnum, SPL_INFERNALSTRIKE, SPELLVAL_DAMAGE));
	int flags = DND_DAMAGEFLAG_ISSPELL;
	int actor_flags = 0;

	// "The strike is always a critical hit."
	if(SpellThresholdMet(pnum, SPL_INFERNALSTRIKE, DND_SPELL_THRESH_HIGH))
		actor_flags |= DND_ACTORFLAG_CONFIRMEDCRIT;

	int out = HandleDamageDeal(ptid, victim, dmg, DND_DAMAGETYPE_FIRE, SPL_INFERNALSTRIKE,
		flags, 0, 0, 0, actor_flags, true);
	if(out > 0)
		Thing_Damage2(victim, out, "SkipHandle");

	// "with a 25% chance to strike a 128 unit area", +25% at rank 5.
	int chance = 25 + 25 * SpellThresholdMet(pnum, SPL_INFERNALSTRIKE, DND_SPELL_THRESH_LOW);
	if(chance < random(1, 100))
		Terminate;

	int r = GetSpellValue(pnum, SPL_INFERNALSTRIKE, SPELLVAL_RADIUS) << 16;
	int i, mn;
	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		if(i == victim || !IsActorAlive(i) || !CheckFlag(i, "SHOOTABLE"))
			continue;
		if(fdistance(victim, i) > r)
			continue;

		out = HandleDamageDeal(ptid, i, dmg, DND_DAMAGETYPE_FIRE, SPL_INFERNALSTRIKE,
			flags | DND_DAMAGEFLAG_ISRADIUSDMG, 0, 0, 0, actor_flags, true);
		if(out > 0)
			Thing_Damage2(i, out, "SkipHandle");
	}
}

// Heat Shield's retaliation, fired from HandlePlayerResists when a monster lands a hit on a wearer.
// The wearer may not be the caster, so the rank comes off the marker they carry.
//
// A real ignite, not a burn of its own: both of this spell's thresholds are written in the ignite
// system's own terms -- one lengthens the ignite, the other forces it to spread -- and neither can be
// expressed by a private DoT. The ignite is guaranteed rather than rolled because the spell states it
// outright, which is what ApplyForcedIgnite exists for.
Script "DnD Heat Shield Retaliate" (int pnum, int attacker) {
	int rank = CheckActorInventory(pnum + P_TIDSTART, "DnD_HeatShieldRank");
	if(!rank || !IsActorAlive(attacker))
		Terminate;

	// "Ignites from this last 100% longer" -- of the player's own ignite duration, so it compounds
	// with their ignite duration gear rather than replacing it with a flat figure.
	int dur_pct = 100;
	if(rank >= DND_SPELL_THRESH_LOW)
		dur_pct = 200;

	// "Ignites from this are guaranteed to spread."
	ApplyForcedIgnite(pnum, attacker, dur_pct, rank >= DND_SPELL_THRESH_HIGH);
}

// Effects for the new spell system. TryCastSpell has already checked the gates, spent the mana and
// started the cooldown by the time this runs -- this script only produces the effect.
// ============================ Ice Bolt ============================
// A plain projectile. Nothing here walks the monster list: the slow is applied by the cold on-hit
// seam in "DnD Damage Accumulate" when the bolt damages something, and the rank 10 splash is an
// A_Explode through the shared spell explosion machinery.

#define DND_ICEBOLT_SPEED 25

// The doc gives no duration. Two seconds sits in chill's own rhythm -- its stacks decay one a
// second -- and outlasts the 1 second cast without becoming permanent on everything you touch.
#define DND_ICEBOLT_SLOWTICS (2 * TICRATE)

#define DND_ICEBOLT_R5_SLOW 75

// ============================ Freezing Pulse ============================
// A fan of ripping sub-projectiles whose boxes overlap into the crescent the spell is meant to be.
// One actor cannot be a crescent: a Doom hitbox is an axis aligned BOX of Radius x Radius x Height,
// so a shape is composed out of several of them or not at all.
//
// Laterally they sit a step apart; forward they lag by the sagitta of the arc they sit on, which is
// quadratic in that step -- the outer pair falls back four times as far as the inner pair. The lag
// is applied as a SPEED difference rather than a spawn offset so the bow deepens as the wave
// travels, which is what the reference art does.
//
// Every sub-projectile carries the same reserved DnD_RipperId, so RIPSONCE dedupes the whole fan as
// ONE attack. Without that an enemy caught by three of them would be hit three times.

#define DND_FREEZINGPULSE_COUNT 5     // odd, so there is a centre
#define DND_FREEZINGPULSE_SPEED 26

// Off the floor, not the eyeline -- a wave that flies level from chest height just floats.
#define DND_FREEZINGPULSE_Z 2.0
#define DND_FREEZINGPULSE_SPACING 26.0

// Speed lost per step out from the centre, squared -- this IS the arc. 1 per step means the outer
// pair trails the centre by about four units a tic.
#define DND_FREEZINGPULSE_LAG 1

// A little outward so the fan opens as it goes. Fixed point angle, 0.006 is a bit over two degrees.
#define DND_FREEZINGPULSE_SPREAD 0.006

#define DND_FREEZINGPULSE_R5_RANGE 25   // percent further
#define DND_FREEZINGPULSE_R10_WIDTH 25  // percent wider

// GUESS: the doc states a freeze chance but no freeze length.
#define DND_FREEZINGPULSE_FREEZETICS TICRATE

// When each caster last loosed a pulse, and the id its fan shares. The tic is what the falloff is
// measured against -- the whole fan leaves at once and travels at one speed, so time since the cast
// is the distance it has come, and no projectile has to be asked.
// A module& must return a reference to a STRUCT, so the cell is one.
typedef struct {
	int val;
} pulse_cell_T;

pulse_cell_T module& GetPulseCastTic(int pnum) {
	static pulse_cell_T tics[MAXPLAYERS];
	return tics[pnum];
}

pulse_cell_T module& GetPulseRipperId(int pnum) {
	static pulse_cell_T ids[MAXPLAYERS];
	return ids[pnum];
}

// How many tics a pulse is in the air, which is also the span its freeze chance decays over.
// How much wider than base this cast is. ONE number, because the arcs and the model it is drawn
// with have to agree -- the visual is a claim about where the damage is.
//
// Rank 10's +25% joins the ADDITIVE area pool rather than multiplying the finished width, which is
// what ScalePlayerAoERadius exists to do: a rank that reads "increased" stacks with the player's
// own increases instead of compounding on top of them. Asking it to scale exactly 1.0 hands back
// the factor itself.
int GetPulseWidthFactor(int pnum) {
	return ScalePlayerAoERadius(pnum, 1.0, DND_AOESRC_NONWEAPON,
		DND_FREEZINGPULSE_R10_WIDTH * SpellThresholdMet(pnum, SPL_FREEZINGPULSE, DND_SPELL_THRESH_HIGH));
}

// Set by the carrier on its first tic. The actor's scaleX drives the model's x AND y, and scaleY
// drives its z, so one factor on both grows the wave evenly with the fan it is standing in for.
Script "DnD Freezing Pulse Scale" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum != -1) {
		int f = GetPulseWidthFactor(pnum);

		// MULTIPLIED into whatever the actor already carries, not assigned. The DECORATE Scale is the
		// base size of the wave and stays the place to change it; this only applies the cast's own
		// widening on top. Assigning would silently throw that base away.
		SetActorProperty(0, APROP_SCALEX, FixedMul(GetActorProperty(0, APROP_SCALEX), f));
		SetActorProperty(0, APROP_SCALEY, FixedMul(GetActorProperty(0, APROP_SCALEY), f));
	}

	SetResultValue(0);
}

int GetPulseFlightTics(int pnum) {
	int range = GetSpellValue(pnum, SPL_FREEZINGPULSE, SPELLVAL_RADIUS);
	if(SpellThresholdMet(pnum, SPL_FREEZINGPULSE, DND_SPELL_THRESH_LOW))
		range = range * (100 + DND_FREEZINGPULSE_R5_RANGE) / 100;

	return Max(1, range / DND_FREEZINGPULSE_SPEED);
}

// Claimed by each sub-projectile on its first tic. The owner is already on APROP_SCORE by then --
// "DnD Projectile Checks" put it there -- so the fan finds its shared id without being handed one.
Script "DnD Freezing Pulse Claim" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum != -1)
		SetInventory("DnD_RipperId", GetPulseRipperId(pnum).val + 1);

	SetResultValue(0);
}

void CastFreezingPulse(int pnum) {
	auto tic = GetPulseCastTic(pnum);
	tic.val = Timer();

	// One id for the whole fan, reserved BEFORE any of it exists so every piece claims the same one.
	auto rid = GetPulseRipperId(pnum);
	rid.val = ReserveRipperId();

	int life = GetPulseFlightTics(pnum);

	// Same factor the carrier scales its model by, so the two never drift apart.
	int step = FixedMul(DND_FREEZINGPULSE_SPACING, GetPulseWidthFactor(pnum));

	// The arcs. Spell_FreezingPulse_Part, NOT Spell_FreezingPulse: the damage lives on the invisible
	// fan, and the named actor is the single visible carrier spawned below. Spawning the carrier
	// here instead would stack five copies of the model and deal nothing, since it has Damage 0.
	int k, off;
	int mid = DND_FREEZINGPULSE_COUNT / 2;
	for(k = 0; k < DND_FREEZINGPULSE_COUNT; ++k) {
		off = k - mid;

		// off * off is the sagitta term: the arc depth grows with the SQUARE of how far out this
		// piece sits, which is what separates a bowed wave from a flat rank of projectiles.
		// flat: a wave travels along the floor, so pitch is ignored and only facing is used.
		SpawnSpellProjectile(pnum, SPL_FREEZINGPULSE, "Spell_FreezingPulse_Part",
			DND_FREEZINGPULSE_SPEED - DND_FREEZINGPULSE_LAG * off * off,
			0, DND_FREEZINGPULSE_SPREAD * off, step * off, life, 0, true, DND_FREEZINGPULSE_Z);
	}

	// One carrier down the middle at the centre speed, which is what wears the model. The model is
	// itself the whole cascade, so it is spawned ONCE -- one per arc would be five cascades inside
	// each other. It deals nothing and claims no ripper id; it only has to be in the right place.
	SpawnSpellProjectile(pnum, SPL_FREEZINGPULSE, "Spell_FreezingPulse",
		DND_FREEZINGPULSE_SPEED, 0, 0, 0, life, 0, true, DND_FREEZINGPULSE_Z);
}


// ============================ cold, on hit ============================
// Reached from "DnD Damage Accumulate" the moment a cold SPELL damages a monster, so a spell never
// has to go looking for who it hit. One damage instance, one call -- and RIPSONCE already makes that
// one instance per enemy for the ripping spells.
Script "DnD Spell Cold On Hit" (int pnum, int victim, int spell) {
	int pct, chance, gone, span;

	switch(spell) {
		case SPL_ICEBOLT:
			pct = SpellThresholdMet(pnum, SPL_ICEBOLT, DND_SPELL_THRESH_LOW) ?
				DND_ICEBOLT_R5_SLOW : GetSpellValue(pnum, SPL_ICEBOLT, SPELLVAL_DAMAGE2);
			SlowMonster(victim, pct, DND_ICEBOLT_SLOWTICS);
		break;

		case SPL_FREEZINGPULSE:
			// Full at the muzzle, nothing at the edge. Measured in TIME since the cast: the fan leaves
			// together and holds its speed, so this is the distance it has come without asking any
			// projectile for its position.
			span = GetPulseFlightTics(pnum);
			gone = Clamp_Between(Timer() - GetPulseCastTic(pnum).val, 0, span);

			chance = GetSpellValue(pnum, SPL_FREEZINGPULSE, SPELLVAL_DAMAGE2) * (span - gone) / span;
			if(chance > 0 && random(1, 100) <= chance)
				FreezeMonster(pnum, victim, DND_FREEZINGPULSE_FREEZETICS);
		break;
	}

	SetResultValue(0);
}

Script "DnD Spell Cast" (int spell, int pnum) {
	// Monsters wake to a hostile cast the way they wake to a gunshot. At the START of the cast, not
	// when the effect lands, so a long cast bar cannot be used to open on a sleeping room for free
	// -- Molten Boulder would otherwise get 2.5 seconds of silence before anything noticed.
	//
	// MonsterWaker is a CustomInventory whose Pickup runs A_AlertMonsters through whoever holds it,
	// so it needs a live carrier; the caster is one by definition at this point.
	if(IsHostileSpell(spell))
		GiveActorInventory(pnum + P_TIDSTART, "MonsterWaker", 1);

	// The cast time is spent HERE rather than in TryCastSpell, which has already taken the mana and
	// started the cooldown -- so a cast that is interrupted still costs, and the cooldown runs from the
	// press rather than from the finish. The effect is what waits.
	// Deliberately does NOT lock the weapon. Only a channel does that, because only a channel is
	// driven by the attack button and so cannot share it. A plain cast time leaves the player free to
	// keep shooting through it.
	int ct = GetSpellCastTics(pnum, spell);
	bool ally = !!(SpellDefs[spell].flags & SPLF_ALLYTARGET);

	if(ct > 0) {
		BeginSpellCasting(pnum);
		int casting = StartSpellCastBar(pnum, spell, ct);

		if(ally) {
			// Re-aimed every tic for the first stretch of the bar, then COMMITTED. The last quarter is
			// locked in so the spell lands where the marker said it would, rather than on whoever the
			// crosshair happened to cross on the final tic.
			int track = ct * DND_SPELLLOCK_TRACKPCT / 100;
			for(int e = 0; e < ct; ++e) {
				if(e < track)
					SetSpellLockTarget(pnum, GetSpellAllyTarget(pnum));
				Delay(const:1);
			}
		}
		else
			Delay(ct);

		EndSpellCastBar(pnum, casting);
		EndSpellCasting(pnum);

		// The box goes now; WHO was locked is still needed by the cast below.
		if(ally)
			DropSpellLockMarker(pnum);
	}
	else if(ally)
		// No bar to lock during -- resolve it on the spot, and no marker is ever shown.
		SetSpellLockTarget(pnum, GetSpellAllyTarget(pnum));

	int i, count, victim, temp, pillar_life;

	// "When you finish casting a fire spell" -- after the cast time, not at the press.
	CheckHeartOfFire(pnum, spell);

	switch(spell) {
		case SPL_BLAZE:
			victim = SpellHitscan(pnum, spell, "Spell_BlazePuff");
			if(!victim)
				break;

			PlaySound(pnum + P_TIDSTART, "Blaze/Cast", CHAN_6);

			// The trace already dealt one tick's worth; the burn carries the rest.
			count = GetSpellValue(pnum, spell, SPELLVAL_DAMAGE);
			ACS_NamedExecuteAlways("DnD Blaze Burn", 0, victim, pnum, count);

			// "Also ignites enemies within 96 units of the target."
			if(SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_HIGH)) {
				temp = GetSpellValue(pnum, spell, SPELLVAL_RADIUS);
				for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
					int other = UsedMonsterTIDs[i];
					if(other == victim || !IsActorAlive(other) || !CheckFlag(other, "SHOOTABLE"))
						continue;
					if(fdistance(victim, other) <= (temp << 16))
						ACS_NamedExecuteAlways("DnD Blaze Burn", 0, other, pnum, count);
				}
			}
		break;


		case SPL_WARMTH:
			CastWarmth(pnum);
		break;

		case SPL_HEATSHIELD:
			CastHeatShield(pnum);
		break;

		case SPL_BOILINGBLOOD:
			CastBoilingBlood(pnum);
		break;

		case SPL_INFERNALSTRIKE:
			CastInfernalStrike(pnum);
		break;

		case SPL_SEARINGBOND:
			// A seeker, so it only needs throwing in the right general direction.
			SpawnSpellProjectile(pnum, spell, "Spell_SearingBond", DND_SEARINGBOND_SPEED);
		break;

		case SPL_FIREJET:
			// The jet throws its own sideways flames as it flies, so this only has to launch it.
			SpawnSpellProjectile(pnum, spell, "Spell_FireJet", DND_FIREJET_SPEED);
		break;

		case SPL_RIGHTEOUSFIRE:
			CastRighteousFire(pnum);
		break;

		case SPL_MOLTENBOULDER_S:
			CastMoltenBoulder(pnum);
		break;

		case SPL_FLAMEPILLAR:
			// The pillar lays its own flame down as it advances, so this only has to launch it.
			// Rank 10 runs a parallel pair instead of one, offset sideways rather than fanned --
			// they are meant to sweep a corridor abreast, not diverge.
			if(SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_HIGH)) {
				// ReactionTime off the row. DECORATE had 18 frozen into it, which is 72 tics against the
				// 70 the duration asks for -- near enough by luck, and completely deaf to the row.
				pillar_life = Max(1, GetSpellDurationTics(pnum, spell) / DND_FLAMEPILLAR_LOOPTICS);

				SpawnSpellProjectile(pnum, spell, "Spell_FlamePillar", DND_FLAMEPILLAR_SPEED, 0, 0,
					DND_FLAMEPILLAR_SIDEOFF, pillar_life);
				SpawnSpellProjectile(pnum, spell, "Spell_FlamePillar", DND_FLAMEPILLAR_SPEED, 0, 0,
					-DND_FLAMEPILLAR_SIDEOFF, pillar_life);
			}
			else
				SpawnSpellProjectile(pnum, spell, "Spell_FlamePillar", DND_FLAMEPILLAR_SPEED, 0, 0, 0,
					Max(1, GetSpellDurationTics(pnum, spell) / DND_FLAMEPILLAR_LOOPTICS));
		break;

		case SPL_RAINOFFIRE:
			ACS_NamedExecuteAlways("DnD Rain Of Fire", 0, pnum);
		break;

		case SPL_IMMOLATION:
			CastImmolation(pnum);
		break;

		case SPL_FLAMMABILITY:
			ACS_NamedExecuteAlways("DnD Flammability Cast", 0, pnum);
		break;

		case SPL_SCORCHINGRAY:
			ACS_NamedExecuteAlways("DnD Scorching Ray", 0, pnum);
		break;

		case SPL_ANNIHILUS:
			ACS_NamedExecuteAlways("DnD Annihilus", 0, pnum);
		break;

		case SPL_VOLCANO:
			ACS_NamedExecuteAlways("DnD Volcano", 0, pnum);
		break;

		case SPL_FIREDEMON:
			ACS_NamedExecuteAlways("DnD Summon Fire Demon", 0, pnum);
		break;

		case SPL_ICEBOLT:
			// Everything cold about it waits for the impact, which is where the rank is read.
			SpawnSpellProjectile(pnum, spell, "Spell_IceBolt", DND_ICEBOLT_SPEED);
		break;

		case SPL_FREEZINGPULSE:
			CastFreezingPulse(pnum);
		break;

		case SPL_PYROBLAST:
			// One bolt. The rank 10 fragments are not thrown here -- the explosion spawns them from
			// its own state, so they come off the blast rather than off the caster.
			SpawnSpellProjectile(pnum, spell, "Spell_Pyroblast", DND_PYROBLAST_SPEED);
		break;

		case SPL_FIREBALL:
			// AMOUNT is the base count, so a synergy that raises it still counts; the rank 5 and rank 10
			// thresholds each add the one extra "to the side" their text promises. Damage is NOT worked
			// out here -- each ball resolves its own at impact off user_spellid, which SpawnSpellProjectile
			// stamps on it.
			count = GetSpellValue(pnum, spell, SPELLVAL_AMOUNT) +
				SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_LOW) +
				SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_HIGH);

			for(i = 0; i < count; ++i)
				SpawnSpellProjectile(pnum, spell, "Spell_Fireball", DND_FIREBALL_SPEED, 0,
					FanOffset(i, DND_FIREBALL_SPREAD));
		break;
	}
}

// Molten Boulder's two blasts. Spawned from ACS for the same reason Fire Jet's sides and Flame
// Pillar's flame are: user variables do NOT transfer on a DECORATE spawn, so an A_SpawnItemEx
// damager carries no spell id, prices itself as spell 0 and resolves as Blaze.
//
// isBump picks which damager to drop; each asks the explosion setup for its own field and radius.
// Position is the boulder's own, which is what A_SpawnItemEx with no offset gave.
Script "DnD Molten Boulder Damage" (int isBump) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	int spell = GetUserVariable(0, "user_spellid");
	int tid = TEMPORARY_SPELL_TID + pnum;

	if(SpawnForced(isBump ? "Spell_MoltenBoulder_Bump" : "Spell_MoltenBoulder_Roll",
		GetActorX(0), GetActorY(0), GetActorZ(0), tid, 0)) {
		SetupSpellActor(tid, pnum, spell);
		Thing_ChangeTID(tid, 0);
	}

	SetResultValue(0);
}

// "It shatters on a wall": one tic with no movement at all means it is wedged. The legacy
// "DnD Boulder Hit Check" does the same job for the old boulder and gives the old token.
Script "DnD Spell Boulder Stall" (void) {
	int x = GetActorX(0), y = GetActorY(0), z = GetActorZ(0);
	Delay(1);
	if(x == GetActorX(0) && y == GetActorY(0) && z == GetActorZ(0))
		GiveInventory("Spell_MoltenBoulderStall", 1);
}

// Rank 10's six shards, thrown by the shatter. Called unconditionally from the Death state and
// gated HERE rather than with a state jump, so the shatter's own FX sequence keeps the exact shape
// it had in the old file.
//
// Fanned evenly on the horizontal, which is what "outward" asks for; they take their own arcs from
// there like any other Pyroblast.
Script "DnD Molten Boulder Shards" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	int spell = GetUserVariable(0, "user_spellid");
	if(!SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_HIGH)) {
		SetResultValue(0);
		Terminate;
	}

	int x = GetActorX(0), y = GetActorY(0), z = GetActorZ(0);
	int tid = TEMPORARY_SPELL_TID + pnum;
	int rank = Max(1, GetSpellRank(pnum, SPL_PYROBLAST, true));
	int i, a;

	for(i = 0; i < DND_MOLTENBOULDER_SHARDS; ++i) {
		if(!SpawnForced("Spell_MoltenBoulder_Shard", x, y, z, tid, 0))
			continue;

		// Stamped as PYROBLAST, not as the boulder: these are Pyroblasts and are priced like them.
		SetupSpellActor(tid, pnum, SPL_PYROBLAST);

		// At the caster's own Pyroblast rank, or rank 1 if they never took it. The tree does not
		// require Pyroblast -- this spell's only prerequisite is Flame Pillar, which has none -- and
		// GetSpellValue returns 0 for an unallocated spell, so without the floor the rank 10 would
		// silently do nothing for such a build.
		SetUserVariable(tid, "user_rankat", rank);
		SetUserVariable(tid, "user_spellrank", rank);

		a = (1.0 * i) / DND_MOLTENBOULDER_SHARDS;
		SetActorAngle(tid, a);
		SetActorVelocity(tid, DND_MOLTENBOULDER_SHARDSPEED * cos(a),
			DND_MOLTENBOULDER_SHARDSPEED * sin(a), 0, false, false);

		// Released before the next one takes the same tid.
		Thing_ChangeTID(tid, 0);
	}

	SetResultValue(0);
}

// Righteous Fire's burn. Modelled on "DnD Immolation Tick" -- same toggle flag, same teardown, same
// once-a-second cadence.
//
// The damage is a PERCENT OF THE CASTER rather than a flat row: "50% of your health and energy
// shield". Recomputed every second so a shield that regenerates mid burn counts, which is the whole
// reason the spell scales with a defensive build at all.
Script "DnD Righteous Fire Tick" (int pnum) {
	int ptid = pnum + P_TIDSTART;
	SetPlayerAttachment(pnum, DND_PLAYERFX_RIGHTEOUSFIRE, true);

	// Looping, and paired with the StopSound in the teardown below. Started HERE rather than in
	// CastRighteousFire so the two sit together: every way out of this script -- a toggle off, life
	// bottoming out, death, leaving the game -- falls through to that teardown, so there is no exit
	// that can leave the hum running.
	PlaySound(ptid, "RighteousFire/Loop", DND_RIGHTEOUSFIRE_CHAN, 1.0, true);

	int mn, v, dealt, r, dmg, enemy_dmg, self_rate, t;

	// Carries the sub-tic remainder between seconds, so spreading the degen never rounds any of
	// it away. Outside the loop deliberately.
	int acc = 0;

	// Rank 5's resistance is against THIS spell only, so it is a straight cut on the self burn rather
	// than anything added to the player's fire resistance. Read once -- a rank cannot change mid burn.
	int res = SpellThresholdMet(pnum, SPL_RIGHTEOUSFIRE, DND_SPELL_THRESH_LOW) ?
		DND_RIGHTEOUSFIRE_R5_RESIST : 0;

	while(true) {
		// Three ways out, and the flag covers two: a second cast clears it, so does a teardown
		// elsewhere. Death and leaving are tested directly.
		if(!IsSpellRunning(pnum, SPL_RIGHTEOUSFIRE) || !PlayerInGame(pnum) || !IsActorAlive(ptid))
			break;

		r = ScalePlayerAoERadius(pnum, GetSpellValue(pnum, SPL_RIGHTEOUSFIRE, SPELLVAL_RADIUS) << 16,
			DND_AOESRC_NONWEAPON);

		// A percent of the MAXIMUM health and shield, not the current. Off current health the figure
		// fell as the burn ate into it -- taking half of what was left every second, which is
		// repeated halving rather than a steady degeneration, and it made the damage dealt decay
		// along with it. Off the caps it is a flat rate for as long as the spell is up.
		dmg = (GetSpawnHealth(false, pnum) + GetPlayerEnergyShieldCap(pnum)) *
			GetSpellValue(pnum, SPL_RIGHTEOUSFIRE, SPELLVAL_DAMAGE) / 100;

		// Intellect scales what it BURNS, never what it costs. dmg prices both halves here, so the
		// enemy figure is taken apart from it -- otherwise investing in the stat would quietly raise
		// your own upkeep and the spell would get harder to hold the better you got at it.
		enemy_dmg = ApplySpellIntScaling(pnum, dmg);

		// "20% more spell damage while it burns." Renewed each second rather than granted once, so it
		// lasts exactly as long as the burn is actually being paid for.
		HandlePlayerBuffAssignment(pnum, ptid, BTI_SPELL_RIGHTEOUSFIRE, 0, 0,
			DND_RIGHTEOUSFIRE_BUFFTICS, GetSpellValue(pnum, SPL_RIGHTEOUSFIRE, SPELLVAL_DAMAGE2));

		// Rank 10: cannot be chilled, frozen or ignited while burning. Re-stamped every second
		// rather than read once like res, so gear that crosses the threshold mid burn counts.
		SetActorInventory(ptid, "DnD_RighteousFireWard",
			SpellThresholdMet(pnum, SPL_RIGHTEOUSFIRE, DND_SPELL_THRESH_HIGH));

		for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
			v = UsedMonsterTIDs[mn];
			if(!IsActorAlive(v) || !CheckFlag(v, "SHOOTABLE"))
				continue;
			if(fdistance(ptid, v) > r)
				continue;

			// ISDAMAGEOVERTIME keeps the tick from rolling a crit, as Immolation's does. A periodic aura
			// attached to the player is damage over time by nature.
			dealt = HandleDamageDeal(ptid, v, enemy_dmg, DND_DAMAGETYPE_FIRE, SPL_RIGHTEOUSFIRE,
				DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISRADIUSDMG | DND_DAMAGEFLAG_ISDAMAGEOVERTIME,
				0, 0, 0, 0, true);
			if(dealt > 0)
				Thing_Damage2(v, dealt, "SkipHandle");
		}

		// And it burns the caster -- as DEGENERATION, not as damage. Righteous Fire is not the player
		// hurting themselves; it fights their regeneration to offset it, so nothing that triggers on
		// being hurt may fire. DegenPlayer is what guarantees that, and why this is not a Thing_Damage2.
		//
		// Shield recharge is still held down for the duration, but that happens in
		// ChargePoolInterrupted off the aura marker rather than through a hit timer here.
		self_rate = dmg * (100 - res) / 100;

		// Spread across the second rather than taken as one lump. Regeneration also lands once a
		// second, so a lump made survival depend on which of the two happened to run first; a tic by
		// tic drain genuinely contends with it, which is what a degeneration is meant to do.
		for(t = 0; t < TICRATE; ++t) {
			acc += self_rate;
			if(acc >= TICRATE) {
				DegenPlayer(pnum, acc / TICRATE);
				acc %= TICRATE;
			}

			if(!PlayerInGame(pnum) || !IsActorAlive(ptid))
				break;

			// Switches itself off at one life. DegenPlayer floors there, so this is the moment the
			// burn has taken everything it can -- holding it on past that would be a dead aura
			// reserving a spell slot and still interrupting shield recharge for nothing.
			if(GetActorProperty(ptid, APROP_HEALTH) <= 1) {
				SetSpellRunning(pnum, SPL_RIGHTEOUSFIRE, false);
				break;
			}

			Delay(const:1);
		}
	}

	SetSpellRunning(pnum, SPL_RIGHTEOUSFIRE, false);
	SetPlayerAttachment(pnum, DND_PLAYERFX_RIGHTEOUSFIRE, false);
	SetActorInventory(ptid, "DnD_RighteousFireWard", 0);
	StopSound(ptid, DND_RIGHTEOUSFIRE_CHAN);
}

// Immolation scatters its flames over this many units at the spell's BASE radius. Kept in step
// with the fallback const of the same name in Spell_ImmolationFX.
#define DND_IMMOLATION_SPREAD 40

// How wide the emitter should throw them NOW. The flames keep the same proportion of the radius
// that the old fixed 40 had of the base 96, so a grown radius widens the field the player stands
// in rather than only reaching further to deal damage.
//
// A number rather than a scale factor, because Spell_ImmolationFX is an invisible TNT1 emitter --
// there is no art on it for the aura spawner to grow, and DECORATE places the flames itself.
Script "DnD Immolation Spread" (void) CLIENTSIDE {
	// The emitter is the activator here, and the caster is its target.
	if(!SetActivatorToTarget(0)) {
		SetResultValue(DND_IMMOLATION_SPREAD);
		Terminate;
	}

	int pnum = PlayerNumber();
	int base = SpellDefs[SPL_IMMOLATION].base[SPELLVAL_RADIUS];
	if(pnum < 0 || base <= 0) {
		SetResultValue(DND_IMMOLATION_SPREAD);
		Terminate;
	}

	// Built exactly like the damage radius in "DnD Immolation Tick", so the two cannot drift.
	int r = ScalePlayerAoERadius(pnum, GetSpellValue(pnum, SPL_IMMOLATION, SPELLVAL_RADIUS) << 16,
		DND_AOESRC_NONWEAPON) >> 16;

	SetResultValue(Max(1, r * DND_IMMOLATION_SPREAD / base));
}

// ======================= Summon: Fire Demon =======================
// One demon per caster, and it obeys the pet cap by EVICTING rather than refusing: a full roster
// would otherwise make the spell silently do nothing, which reads as a broken button.

#define DND_FIREDEMON_DIST 96.0    // how far in front of the caster it appears

// Rank 10: the demon throws its OWNER's Fire Jet, at the rank THEY have it -- so the reward is
// worth more the more they invested in the jet, and worth nothing if they never took it.
//
// Rolls and casts in one call, returning whether it fired, so the demon only needs a single jump.
#define DND_FIREDEMON_JETCHANCE 25

Script "DnD Fire Demon Jet" (void) {
	int demon = ActivatorTID();
	int pnum = GetActorProperty(0, APROP_MASTERTID) - P_TIDSTART;

	// Both halves have to be there: the demon's own rank 10, and an actual Fire Jet to borrow.
	if(pnum < 0 || pnum >= MAXPLAYERS ||
		!SpellThresholdMet(pnum, SPL_FIREDEMON, DND_SPELL_THRESH_HIGH) ||
		!GetSpellRank(pnum, SPL_FIREJET, true) ||
		random(1, 100) > DND_FIREDEMON_JETCHANCE) {
		SetResultValue(0);
		Terminate;
	}

	// Launched FROM the demon, owned BY the caster: SpawnSpellProjectile stamps the caster on it, so
	// the jet prices itself off their SPL_FIREJET row and their stats exactly as if they had cast it.
	SpawnSpellProjectile(pnum, SPL_FIREJET, "Spell_FireJet", DND_FIREJET_SPEED, 0, 0, 0, 0, demon);
	SetResultValue(1);
}

Script "DnD Summon Fire Demon" (int pnum) {
	int caster = pnum + P_TIDSTART;

	// How many demons may stand at once, from the row. At its current 1 this reads as "recasting
	// replaces the one you have"; raise the row and it becomes a real roster limit with no code
	// change. The point is that the PET cap alone would happily be filled with nothing but these.
	int maxkind = Max(1, GetSpellValue(pnum, SPL_FIREDEMON, SPELLVAL_AMOUNT));

	// The oldest DEMON makes way, not an unrelated pet, and only once the kind is actually full.
	// Done BEFORE the global cap is consulted, so a demon never costs something else its place.
	if(CountPlayerPetsOfKind(pnum, MONSTER_PET_FIREDEMON) >= maxkind)
		UnsummonPet(FindOldestPlayerPetOfKind(pnum, MONSTER_PET_FIREDEMON));

	// Still full, so the longest standing pet makes way. The kill runs its death states, which is
	// what returns the PetCounter point this summon is about to take.
	if(!CanActorHaveMorePets(caster))
		UnsummonPet(FindOldestPlayerPet(pnum));

	// In front where there is room, and around the caster where there is not.
	int tid = TEMPORARY_PET_TID + pnum;
	if(!SpawnSummonNear("Spell_FireDemon", caster, tid, DND_FIREDEMON_DIST))
		Terminate;

	// Dropped to the floor it actually landed over, which is not the caster's if it fanned out.
	SetActorPosition(tid, GetActorX(tid), GetActorY(tid), GetActorFloorZ(tid), false);

	// Health off the row, and it has to be written BEFORE the demon takes its first tic: "DnD Pet
	// Monster Scale" reads APROP_HEALTH as its base there and levels up from it. Writing it here is
	// what makes the spell's own number the one that gets scaled, rather than the 500 in DECORATE.
	SetActorProperty(tid, APROP_HEALTH,
		Max(1, GetSpellValue(pnum, SPL_FIREDEMON, SPELLVAL_DAMAGE)));

	SetActorProperty(tid, APROP_MASTERTID, caster);
	SetActivator(tid);
	SetPointer(AAPTR_MASTER, caster);
	SetActorProperty(0, APROP_FRIENDLY, true);

	// No "DnD Timed Monster" call: the demon stands until something kills it. The zombie is the
	// timed pet, and SPL_FIREDEMON carries no DURATION row to read any more either.
	Thing_ChangeTID(tid, 0);

	GiveActorInventory(caster, "PetCounter", 1);

	SetActivator(caster);
	ACS_NamedExecuteAlways("DnD On Pet Summon", 0);
}

// ============================ Volcano ============================
// A cone planted at the aim point that erupts on a loop until its countdown runs out.

#define DND_VOLCANO_LOOPTICS 43       // one pass of SpawnStuff in DECORATE, in tics

// Crater height. The grow-in applies A_SetScale 19 times at 1.2125, so the cone settles at 1.95
// and the sprite stands about 68 units. 58 is its summit.
#define DND_VOLCANO_MOUTH 58.0

// Scattered across the mouth, which is narrow: the sprite is 31 units wide at the BASE, and the
// cone has tapered well past that by the time it reaches MOUTH.
#define DND_VOLCANO_SPREAD 9.0
// Horizontal throw. NOT the actor's own Speed, which nothing reads here -- the velocity is set
// outright. Flight is about 48 tics at the default rise and gravity, so every unit here is ~48
// units of reach: 15 threw rocks over 700 units from a cone 31 wide.
#define DND_VOLCANO_SPEED 11.0
#define DND_VOLCANO_SPEEDVAR 25       // percent either way
#define DND_VOLCANO_RISE 16.0         // upward kick, which is what makes them arc rather than spray
#define DND_VOLCANO_RISEVAR 40        // percent either way, the loosest of the three on purpose
#define DND_VOLCANO_GRAVITY 0.666     // matches the actor's own Gravity
#define DND_VOLCANO_GRAVVAR 30        // percent either way, so they do not all land together

// v, give or take pct percent. Fixed point in and out.
int VaryFixedByPercent(int v, int pct) {
	return v + FixedMul(v, (random(-pct, pct) << 16) / 100);
}

Script "DnD Volcano" (int pnum) {
	int caster = pnum + P_TIDSTART;
	int cx, cy, cz;

	// Same two ways in as Rain of Fire: what the player is looking at, else where the aim puff hit.
	int target = PickActor(caster, GetActorAngle(caster), GetActorPitch(caster),
		DND_SPELL_HITSCANRANGE, 0, MF_SHOOTABLE, ML_BLOCKEVERYTHING, PICKAF_RETURNTID);

	if(target && IsActorAlive(target)) {
		cx = GetActorX(target);
		cy = GetActorY(target);
		cz = GetActorZ(target);
	}
	else {
		if(!TraceSpellAim(pnum, DND_SPELL_HITSCANRANGE))
			Terminate;

		int wait = 0;
		while(!ReadSpellAim(pnum) && wait < DND_SPELLAIM_WAIT) {
			Delay(const:1);
			++wait;
		}

		if(!SpellAimReady(pnum) || !PlayerInGame(pnum) || !IsActorAlive(caster))
			Terminate;

		auto aim = GetSpellAim();
		cx = aim.x[pnum];
		cy = aim.y[pnum];
		cz = aim.z[pnum];
	}

	int tid = TEMPORARY_SPELL_TID + pnum;
	if(!SpawnForced("Spell_Volcano", cx, cy, cz, tid, 0))
		Terminate;

	// Sat on the floor it was aimed at: the cone grows upward out of its own base.
	SetActorPosition(tid, cx, cy, GetActorFloorZ(tid), false);
	SetupSpellActor(tid, pnum, SPL_VOLCANO);

	// A_CountDown fires once per SpawnStuff pass, so the countdown is in LOOPS rather than tics.
	// Read off the row so the +0.5s a rank in DURATION actually lengthens the eruption.
	SetActorProperty(tid, APROP_REACTIONTIME,
		Max(1, GetSpellDurationTics(pnum, SPL_VOLCANO) / DND_VOLCANO_LOOPTICS));

	Thing_ChangeTID(tid, 0);
}

// One eruption. The volcano itself is the activator, called inline from its own frame.
//
// Everything it needs off the activator is read UP FRONT: SetupSpellActor below reassigns the
// activator to each rock and only restores it when the previous one had a tid, which this one does
// not. Reading first makes that irrelevant instead of load bearing.
Script "DnD Volcano Erupt" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1)
		Terminate;

	int n = GetSpellValue(pnum, SPL_VOLCANO, SPELLVAL_DAMAGE2);
	if(n <= 0)
		Terminate;

	int x = GetActorX(0);
	int y = GetActorY(0);
	int z = GetActorZ(0) + DND_VOLCANO_MOUTH;

	int btid = TEMPORARY_DATADUMMY_TID + pnum;
	int i, a, hs, vs;

	for(i = 0; i < n; ++i) {
		// Scattered across the mouth rather than all from one point, so the column has width.
		a = random(0, 1.0);
		if(!SpawnForced("Spell_VolcanoBit",
			x + FixedMul(cos(a), random(0, DND_VOLCANO_SPREAD)),
			y + FixedMul(sin(a), random(0, DND_VOLCANO_SPREAD)), z, btid, 0))
			continue;

		SetupSpellActor(btid, pnum, SPL_VOLCANO);

		// Its own direction, its own throw, its own weight. Sharing any of the three made an eruption
		// a single fan of identical rocks landing in one ring at the same moment.
		a = random(0, 1.0);
		hs = VaryFixedByPercent(DND_VOLCANO_SPEED, DND_VOLCANO_SPEEDVAR);
		vs = VaryFixedByPercent(DND_VOLCANO_RISE, DND_VOLCANO_RISEVAR);

		SetActorVelocity(btid, FixedMul(cos(a), hs), FixedMul(sin(a), hs), vs, false, false);

		// Gravity last, and per rock: SpawnProjectile only takes gravity as an on/off flag, so varying
		// the WEIGHT has to be a property write. It is what spreads the landings out in time.
		SetActorProperty(btid, APROP_GRAVITY,
			VaryFixedByPercent(DND_VOLCANO_GRAVITY, DND_VOLCANO_GRAVVAR));

		Thing_ChangeTID(btid, 0);
	}
}

// ============================ Annihilus ============================
// A charge planted at a point and grown by channelling, released as one blast. The anchor is
// server side and held by tid for the whole channel, which is safe because BeginSpellBusy bars a
// second spell -- nothing else of this player can claim the same scratch tid meanwhile.

#define DND_ANNIHILUS_TICKRATE (TICRATE / 2)   // the 0.5s the damage is quoted on
#define DND_ANNIHILUS_SLOW 50                  // percent movement lost while channelling
#define DND_ANNIHILUS_BUFFTICS (TICRATE / 2)   // slow refresh window, one damage period
#define DND_ANNIHILUS_R5_RANGE 768             // rank 5 raises the reach to this
#define DND_ANNIHILUS_R10_PROCCHANCE 20        // rank 10, percent per charge to detonate early
// Radius units per scale unit, MEASURED rather than taken from the aura comment that claims 64.
//
// flat.md3 carries four frames at escalating sizes and MODELDEF selects frame 3, whose quad is
// +-15.62 units -- not the +-1.0 of frame 0. The lava fills 88.8% of ANNIHILS.png, so one scale
// unit is 15.62 * 0.888 = 13.9 units of visible radius.
//
// The old 56.8 came from that comment and drew the charge about a quarter of its true blast.
#define DND_ANNIHILUS_SCALEDIV 13.9
#define DND_ANNIHILUS_FULLRADIUS 512           // units the blast reaches at FULL channel
#define DND_ANNIHILUS_ESCALATE 30              // percent each later instance is worth over the first
#define DND_ANNIHILUS_FXCOUNT 14               // scattered bursts at the BASE radius
#define DND_ANNIHILUS_FXMAX 30                 // and the most a widened one may spawn
#define DND_ANNIHILUS_HOLD (TICRATE / 2)       // how long the charge rides out its own blast
#define DND_ANNIHILUS_FXLIFT 48.0              // how far up a burst may sit off the floor
#define DND_ANNIHILUS_FXINNER 0.30             // bursts stay out of this share of the middle
#define DND_ANNIHILUS_MAINEXTRA 3              // big explosions around the centre, besides it
#define DND_ANNIHILUS_MAINRING 0.55            // how far out those sit, as a share of the radius
#define DND_ANNIHILUS_FXBASE 96.0              // the radius the FX scales are drawn for

// Doubled at rank 10, which is the whole of that threshold.
// Rank 5 raises the reach to 768. Max rather than assignment, so a row or a modifier that already
// reaches further is never pulled back down to it.
int GetAnnihilusRange(int pnum) {
	int r = GetSpellValue(pnum, SPL_ANNIHILUS, SPELLVAL_AMOUNT);
	if(SpellThresholdMet(pnum, SPL_ANNIHILUS, DND_SPELL_THRESH_LOW))
		r = Max(r, DND_ANNIHILUS_R5_RANGE);
	return r;
}

// One detonation at the anchor: everything inside r takes dmg, and the blast is drawn. Shared by
// the final explosion and by the rank 10 procs, so the two can never end up describing different
// explosions. Self damage is NOT here -- only the final blast can hurt the caster, and only below
// rank 5, which rank 10 implies anyway.
void AnnihilusDetonate(int pnum, int tid, int dmg, int r) {
	int caster = pnum + P_TIDSTART;

	// Here rather than at the two call sites, so the final blast and the rank 10 procs are scaled
	// once each and cannot drift apart.
	dmg = ApplySpellIntScaling(pnum, dmg);
	int mn, v, dealt;

	// Drawn clientside. Every FX actor the blast uses is +CLIENTSIDEONLY through DnD_SpecialFX, so
	// spawning them from here would put nothing on anyone screen -- the same reason the aura
	// spawner is CLIENTSIDE. Packed like Rain of Fire packs its ring: radius high, floor Z low.
	ACS_NamedExecuteAlways("DnD Annihilus Blast FX", 0, GetActorX(tid), GetActorY(tid),
		(((r >> 16) & 0xFFFF) << 16) | ((GetActorZ(tid) >> 16) & 0xFFFF));

	// Measured from the ANCHOR, which is why it is kept standing until the blast is resolved --
	// fdistance wants two actors, and the charge is one of them.
	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		v = UsedMonsterTIDs[mn];
		if(!IsActorAlive(v) || !CheckFlag(v, "SHOOTABLE"))
			continue;
		if(fdistance(tid, v) > r)
			continue;

		// Walls stop it. This loop is hand rolled rather than an A_Explode, and P_RadiusAttack does
		// its own P_CheckSight from the bomb spot -- so every other explosion in the mod already
		// behaves this way and this one was the exception, killing through walls.
		//
		// NOBLOCKALL so ML_BLOCKEVERYTHING lines do not stop it, matching the ignite proliferation
		// check; those are usually invisible blockers that an explosion has no business respecting.
		if(!CheckSight(tid, v, CSF_NOBLOCKALL))
			continue;

		// One hit, not a tick, so no ISDAMAGEOVERTIME -- these are allowed to crit.
		dealt = HandleDamageDeal(caster, v, dmg, DND_DAMAGETYPE_FIRE, SPL_ANNIHILUS,
			DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISRADIUSDMG, 0, 0, 0, 0, true);
		if(dealt > 0)
			Thing_Damage2(v, dealt, "SkipHandle");
	}
}

// The blast, drawn. CLIENTSIDE because every actor it spawns is +CLIENTSIDEONLY: a server side
// SpawnForced of one of those produces nothing on any client, which is the bug this replaced.
//
// Scales come off the blast radius, so a widened Annihilus reads as a bigger detonation instead of
// the same puffs spread over more ground.
Script "DnD Annihilus Blast FX" (int cx, int cy, int packed) CLIENTSIDE {
	int cz = (packed & 0xFFFF) << 16;
	int r = ((packed >> 16) & 0xFFFF) << 16;
	if(r <= 0) {
		SetResultValue(0);
		Terminate;
	}

	// Square rooted on purpose. A 512 blast is over five times the base radius, and scaling each
	// burst by that made single puffs wider than the explosion they belong to. Damped size, more
	// of them, which is what a bigger detonation actually looks like.
	int f = fsqrt(FixedDiv(r, DND_ANNIHILUS_FXBASE));
	int n = Clamp_Between(FixedMul(DND_ANNIHILUS_FXCOUNT << 16, f) >> 16,
		DND_ANNIHILUS_FXCOUNT, DND_ANNIHILUS_FXMAX);

	// Paced so the whole scatter lands inside the half second the anchor lingers for, however many
	// bursts a widened blast asked for -- a fixed rate emptied early on a small one and overran a
	// big one.
	int rate = Max(1, (n + DND_ANNIHILUS_HOLD - 1) / DND_ANNIHILUS_HOLD);

	int tid = DND_PLAYERAURA_TID;
	int i, a, d, bx, by, base_a;
	str fx;

	// The big ones, before the scatter so it lands on top of them rather than under. One on the
	// centre and MAINEXTRA more pushed out around it: a single sprite in the middle of a 512 unit
	// blast left the whole outer half with nothing large happening in it.
	//
	// The ring is rotated by a random amount each time, so repeat casts do not stamp the same
	// three points on the ground.
	base_a = random(0, 1.0);
	for(i = 0; i <= DND_ANNIHILUS_MAINEXTRA; ++i) {
		bx = cx;
		by = cy;
		if(i) {
			a = base_a + FixedDiv((i - 1) << 16, DND_ANNIHILUS_MAINEXTRA << 16);
			bx += FixedMul(cos(a), FixedMul(r, DND_ANNIHILUS_MAINRING));
			by += FixedMul(sin(a), FixedMul(r, DND_ANNIHILUS_MAINRING));
		}

		if(!SpawnForced("Spell_AnnihilusMainExplosionFX", bx, by, cz, tid, 0))
			continue;

		SetActorProperty(tid, APROP_SCALEX,
			FixedMul(GetActorProperty(tid, APROP_SCALEX), f));
		SetActorProperty(tid, APROP_SCALEY,
			FixedMul(GetActorProperty(tid, APROP_SCALEY), f));

		// Only the centre one speaks. Four overlapping copies of the same sample is not four times
		// as impressive, just louder. Played on the FX rather than written into the actor, so that
		// one stays a pure visual.
		if(!i)
			PlaySound(tid, "Annihilus/MainExp", CHAN_AUTO);

		Thing_ChangeTID(tid, 0);
	}

	for(i = 0; i < n; ++i) {
		// Angle as a full turn in fixed point. The distance IS area corrected now -- a flat random
		// packs most of the bursts into the middle, which reads as one clump rather than a field --
		// and it is held out of the centre, where the main explosion already is.
		a = random(0, 1.0);
		d = FixedMul(r, DND_ANNIHILUS_FXINNER +
			FixedMul(1.0 - DND_ANNIHILUS_FXINNER, fsqrt(random(0, 1.0))));

		// Size only -- the four way $random on Annihilus/MiniExp supplies the audible variety.
		switch(random(0, 2)) {
			case 0: fx = "Spell_AnnihilusMiniExplosionFX"; break;
			case 1: fx = "Spell_AnnihilusMiniExplosionFX2"; break;
			default: fx = "Spell_AnnihilusMiniExplosionFX3"; break;
		}

		if(SpawnForced(fx, cx + FixedMul(cos(a), d), cy + FixedMul(sin(a), d),
			cz + random(0, DND_ANNIHILUS_FXLIFT), tid, 0)) {
			SetActorProperty(tid, APROP_SCALEX,
				FixedMul(GetActorProperty(tid, APROP_SCALEX), f));
			SetActorProperty(tid, APROP_SCALEY,
				FixedMul(GetActorProperty(tid, APROP_SCALEY), f));

			// Released before the next spawn takes the same scratch tid, with no Delay in between,
			// so two blasts on one tic cannot collide over it.
			Thing_ChangeTID(tid, 0);
		}

		if(!((i + 1) % rate))
			Delay(const:1);
	}

	SetResultValue(0);
}

Script "DnD Annihilus" (int pnum) {
	int caster = pnum + P_TIDSTART;
	int range = GetAnnihilusRange(pnum) << 16;
	int cx, cy, cz;

	// Same two ways in as Rain of Fire: what the player is looking at, else where the aim puff hit.
	// Traced at the SPELL's own range rather than the generic hitscan one, so the cap IS the cap.
	int target = PickActor(caster, GetActorAngle(caster), GetActorPitch(caster),
		range, 0, MF_SHOOTABLE, ML_BLOCKEVERYTHING, PICKAF_RETURNTID);

	if(target && IsActorAlive(target)) {
		cx = GetActorX(target);
		cy = GetActorY(target);
		cz = GetActorZ(target);
	}
	else {
		if(!TraceSpellAim(pnum, range))
			Terminate;

		int wait = 0;
		while(!ReadSpellAim(pnum) && wait < DND_SPELLAIM_WAIT) {
			Delay(const:1);
			++wait;
		}

		if(!SpellAimReady(pnum) || !PlayerInGame(pnum) || !IsActorAlive(caster))
			Terminate;

		auto aim = GetSpellAim();
		cx = aim.x[pnum];
		cy = aim.y[pnum];
		cz = aim.z[pnum];
	}

	int tid = TEMPORARY_SPELL_TID + pnum;
	if(!SpawnForced("Spell_AnnihilusAnchor", cx, cy, cz, tid, 0))
		Terminate;

	// Dropped onto the floor it was aimed at, so a charge thrown at a wall still reads as planted.
	SetActorPosition(tid, cx, cy, GetActorFloorZ(tid), false);

	// The underside. Same tid on purpose -- the scaling, the Vanish and the tid release below all
	// address the pair as one, so a charge aimed upward is visible from beneath without this
	// script tracking a second actor. A failure here costs the back face and nothing else.
	SpawnForced("Spell_AnnihilusAnchorFlip", GetActorX(tid), GetActorY(tid), GetActorZ(tid), tid, 0);

	BeginSpellBusy(pnum);

	// The charge hum is DECORATE's: Spawn starts it on CHAN_BODY and Vanish stops it, so it lives
	// and dies with the actor rather than needing a matching StopSound on every exit here.

	// The row radius is where the blast STARTS; FULLRADIUS is where a full channel takes it. Stated
	// as a TARGET rather than a multiplier so retuning the table row moves the floor without
	// quietly breaking the reach the description promises.
	//
	// BOTH ends go through ScalePlayerAoERadius, so area of effect gear widens the whole ramp
	// rather than only its start -- a 512 blast is 512 before area modifiers and more after.
	int rbase = ScalePlayerAoERadius(pnum, GetSpellValue(pnum, SPL_ANNIHILUS, SPELLVAL_RADIUS) << 16,
		DND_AOESRC_NONWEAPON);
	int rfull = ScalePlayerAoERadius(pnum, DND_ANNIHILUS_FULLRADIUS << 16, DND_AOESRC_NONWEAPON);

	// A row that already reaches further than the target keeps its own reach; this only ever grows.
	rfull = Max(rfull, rbase);

	int r = rbase;

	// Instances rather than tics, so the 2.5s cap lands on exactly the number of 0.5s periods the
	// text promises instead of on whatever the tic arithmetic rounds to.
	int maxinst = Max(1, GetSpellDurationTics(pnum, SPL_ANNIHILUS) / DND_ANNIHILUS_TICKRATE);

	// The charge grows against TICS, not against instances: five discrete jumps a second apart
	// read as a charge that is not growing at all.
	//
	// maxinst - 1, not maxinst. The loop stops the moment the LAST instance lands, which is one
	// period before the full window elapses -- counting the whole window left the ramp at 80%,
	// so a full channel reached 429 units instead of the 512 it promises.
	int captics = Max(1, (maxinst - 1) * DND_ANNIHILUS_TICKRATE);
	int per = GetSpellValue(pnum, SPL_ANNIHILUS, SPELLVAL_DAMAGE);

	bool held = false;
	int t = 0, inst = 0, charge = 0, prog, weight, added, self_dmg;

	while(inst < maxinst) {
		if(!PlayerInGame(pnum) || !IsActorAlive(caster))
			break;

		// Held ends it; not yet held is forgiven until the grace window runs out. Letting go early is
		// the "cast again to release it for less damage" the description promises.
		if(IsChannelHeld())
			held = true;
		else if(held || t >= DND_CHANNEL_GRACE)
			break;

		// Billed on the same 0.5s the damage is quoted on, so cost and payoff stay in step.
		//
		// EVERY instance pays, including the first -- `first` is passed false rather than !t. The other
		// channels waive it because TryCastSpell already charged at the press, but here the press buys
		// the right to start charging and nothing more: no part of this blast is free.
		//
		// Later instances are worth progressively MORE in DAMAGE, so a full channel is worth holding
		// for rather than being five identical taps.
		//
		// The COST stays flat at the row value -- no pct, so PayChannelTick bills exactly what the
		// row says. That figure already grows with rank through GetSpellValue, so a rank that prices
		// the spell at 60 pays 60 every half second rather than 50.
		if(!(t % DND_ANNIHILUS_TICKRATE)) {
			weight = 100 + inst * DND_ANNIHILUS_ESCALATE;
			if(!PayChannelTick(pnum, SPL_ANNIHILUS, false))
				break;

			added = per * weight / 100;
			charge += added;
			++inst;

			// Rank 10: each charge may go off on its own. It spends nothing -- the charge it just
			// added stays in the pool for the final blast -- so this is free damage for holding, at
			// whatever radius the charge has reached by now.
			if(SpellThresholdMet(pnum, SPL_ANNIHILUS, DND_SPELL_THRESH_HIGH) &&
				random(1, 100) <= DND_ANNIHILUS_R10_PROCCHANCE) {
				AnnihilusDetonate(pnum, tid, ApplySpellMoreDamage(pnum, SPL_ANNIHILUS, added), r);
			}
		}

		HandlePlayerBuffAssignment(pnum, caster, BTI_SPELL_ANNIHILUS_SLOW, 0, 0,
			DND_ANNIHILUS_BUFFTICS, DND_ANNIHILUS_SLOW);

		// The radius itself grows, and the anchor is scaled to it. Both are live every tic, so the
		// charge visibly swells instead of sitting at one size until it goes off.
		prog = Min(1.0, FixedDiv(t << 16, captics << 16));
		r = rbase + FixedMul(rfull - rbase, prog);

		SetActorProperty(tid, APROP_SCALEX, FixedDiv(r, DND_ANNIHILUS_SCALEDIV));
		SetActorProperty(tid, APROP_SCALEY, FixedDiv(r, DND_ANNIHILUS_SCALEDIV));

		Delay(const:1);
		++t;
	}

	EndSpellBusy(pnum);

	// Nothing was ever charged -- the channel died inside the first period. Clean up and go.
	if(charge <= 0) {
		Thing_Remove(tid);
		Terminate;
	}

	charge = ApplySpellMoreDamage(pnum, SPL_ANNIHILUS, charge);
	AnnihilusDetonate(pnum, tid, charge, r);

	// "No longer harms you" at rank 5. Below it the caster is in their own blast like anything else,
	// and at full charge that is lethal -- which is the point of standing clear.
	//
	// SkipHandle, with the resist applied by hand first, which is how every other ACS self damage
	// in the mod does it. A real damage type here went through the event handler instead, and that
	// prices player damage off a MONSTER source -- there is none, so the hit came to nothing and
	// the spell appeared not to hurt the caster at all.
	if(!SpellThresholdMet(pnum, SPL_ANNIHILUS, DND_SPELL_THRESH_LOW) &&
		IsActorAlive(caster) && fdistance(tid, caster) <= r &&
		CheckSight(tid, caster, CSF_NOBLOCKALL)) {
		// Fire lives under the elemental resist, the same slot the Crackle self damage uses.
		self_dmg = ApplyPlayerDamageResist(pnum, charge, DND_PRESIST_ELEM);
		if(self_dmg > 0)
			Thing_Damage2(caster, self_dmg, "SkipHandle");
	}

	// Handed to its own Vanish state rather than deleted: it fades through the explosions it just
	// set off, and stops its own hum on the way. The tid goes back straight away, so a spell cast
	// during that half second cannot land on the same scratch tid.
	SetActorState(tid, "Vanish");
	Thing_ChangeTID(tid, 0);
}

// The spell an aura belongs to, or -1. Only rows whose art DRAWS THE EDGE of the effect are listed,
// because that is the art the radius is supposed to describe -- anything absent keeps the behaviour
// it had, which is area modifiers only.
//
// Immolation is deliberately absent. Its damage radius scales like the others, but Spell_ImmolationFX
// is an invisible TNT1 emitter that throws flames at a fixed 40 unit spread, so scaling it would move
// nothing: there is no art on it to grow, and the flames are placed by DECORATE.
int GetPlayerAttachmentSpell(int which) {
	switch(which) {
		case DND_PLAYERFX_ANGERAURA: return SPL_ANGER;
		case DND_PLAYERFX_RIGHTEOUSFIRE: return SPL_RIGHTEOUSFIRE;
	}
	return -1;
}

// How far the spell row has grown past its base radius, as a fixed point factor for the aura art.
// GetSpellValue already folds in rank, synergies and spell area gear; the global area modifiers
// are applied separately by the caller, which is exactly how the damage radius is built too.
//
// A named script rather than a function because the caller lives in DnD_Attachments.h, which
// expands before the spell headers -- a script name resolves at runtime, so the order stops
// mattering. CLIENTSIDE to match that caller.
Script "DnD Aura Radius Factor" (int which) CLIENTSIDE {
	int pnum = PlayerNumber();
	int spell = GetPlayerAttachmentSpell(which);
	if(pnum < 0 || spell == -1) {
		SetResultValue(1.0);
		Terminate;
	}

	int base = SpellDefs[spell].base[SPELLVAL_RADIUS];
	int cur = GetSpellValue(pnum, spell, SPELLVAL_RADIUS);

	// An unranked or radiusless row leaves the art exactly as DECORATE declared it.
	if(base <= 0 || cur <= 0) {
		SetResultValue(1.0);
		Terminate;
	}

	SetResultValue((cur << 16) / base);
}

#endif
