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

	int r = GetSpellValue(pnum, spell, SPELLVAL_RADIUS) << 16;
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(i == pnum || !PlayerInGame(i) || !IsActorAlive(i + P_TIDSTART))
			continue;
		if(fdistance(ptid, i + P_TIDSTART) <= r)
			HandlePlayerBuffAssignment(i, ptid, bti, 0, 0, dur, val);
	}
}

// Who a SPLF_TARGETED support spell lands on: the ally under the crosshair, or the caster when that
// is nobody. Traced with the aim puff, then matched against the player tids -- LineAttack hands back
// no victim, so this is the same working-backwards the hitscans do.
int GetSpellSupportTarget(int pnum) {
	if(!TraceSpellAim(pnum, DND_SPELL_HITSCANRANGE))
		return pnum;

	int ax = GetSpellAim().x[pnum], ay = GetSpellAim().y[pnum], az = GetSpellAim().z[pnum];
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(i == pnum || !PlayerInGame(i) || !IsActorAlive(i + P_TIDSTART))
			continue;

		int t = i + P_TIDSTART;
		if(az < GetActorZ(t) - DND_SPELL_HITSCANSLACK ||
			az > GetActorZ(t) + GetActorProperty(t, APROP_HEIGHT) + DND_SPELL_HITSCANSLACK)
			continue;

		if(fdistance_delta(ax - GetActorX(t), ay - GetActorY(t), 0) <=
			GetActorProperty(t, APROP_RADIUS) + DND_SPELL_HITSCANSLACK)
			return i;
	}

	return pnum;
}

// Every aura the player has switched on, refreshed. Auras carry no duration of their own, so they are
// granted for a little longer than this pass's own period and simply lapse when it stops renewing
// them -- which is what makes switching one off take effect without a teardown path per aura.
#define DND_SPELLAURA_REFRESH (2 * TICRATE)

void RefreshSpellAuras(int pnum) {
	if(IsSpellActive(pnum, SPL_ANGER))
		ApplyAngerAura(pnum, DND_SPELLAURA_REFRESH);
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

		GetSpellCooldowns().at[pnum][s] -= cut;

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

	if(victim && IsActorAlive(victim))
		SnareMonster(victim, tics);

	// "Also binds enemies within 160 units of the target." Measured from the bolt rather than from the
	// victim, because a bolt that struck geometry has no victim to measure from and should still bind.
	if(binds_area) {
		int r = GetSpellValue(pnum, SPL_SEARINGBOND, SPELLVAL_RADIUS) << 16;
		int i, mn, s;

		for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
			i = UsedMonsterTIDs[mn];
			if(i == victim || !IsActorAlive(i) || !CheckFlag(i, "SHOOTABLE"))
				continue;

			if(fdistance_delta(bx - GetActorX(i), by - GetActorY(i), bz - GetActorZ(i)) <= r)
				SnareMonster(i, tics);
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
		ACS_NamedExecuteAlways("DnD Scorching Ray FX", 0, GetScorchRayCutoff(pnum, len),
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
			for(i = 0; i < sr.count; ++i) {
				v = sr.hit[i];
				if(!IsActorAlive(v))
				continue;

				dealt = HandleDamageDeal(caster, v, GetSpellValue(pnum, SPL_SCORCHINGRAY, SPELLVAL_DAMAGE),
				DND_DAMAGETYPE_FIRE, SPL_SCORCHINGRAY, DND_DAMAGEFLAG_ISSPELL, 0, 0, 0, 0, true);
				if(dealt > 0) {
					// SkipHandle, not "Fire": HandleDamageDeal has already applied resists, and the handler's
					// skip list is what stops this second application re-running the whole pipeline on it.
					Thing_Damage2(v, dealt, "SkipHandle");
				}

				// "After 2.5 seconds" is how long the BEAM has been going, which the loop counter already
				// is -- so from that point on everything it touches is exposed. Tracking it per target
				// instead needs a contact time per monster, and there is nothing in the spell that asks for
				// the difference.
				if(t < DND_SCORCHRAY_EXPOSEAT)
				continue;

				if(CheckActorInventory(v, "DnD_FireExposed") < DND_SCORCHRAY_EXPOSEPCT)
				SetActorInventory(v, "DnD_FireExposed", DND_SCORCHRAY_EXPOSEPCT);

				SetActorInventory(v, "DnD_FireExposedTimer",
				GetSpellDurationTics(pnum, SPL_SCORCHINGRAY));
				ACS_NamedExecuteAlways("DnD Fire Exposure Timer", 0, v);
			}
		}

		Delay(const:1);
		++t;
	}

	EndSpellBusy(pnum);
}

// How far the beam is allowed to be DRAWN: wherever a trace that ignores actors first meets
// geometry. Server side, like everything else here -- the Wanderer ray guesses at this by watching a
// Spawn fail, and asking the engine is exact.
int GetScorchRayCutoff(int pnum, int len) {
	int caster = pnum + P_TIDSTART;
	int puff = TEMPORARY_DATADUMMY_TID + pnum;

	// Measured from where the BEAM starts, not from where the engine fires this trace. LineAttack
	// leaves the player's own attack height whatever we do -- it takes no origin -- so what this can
	// control is the point the distance is measured from, and that has to be the beam's origin or the
	// drawn length stops short of the wall or runs past it.
	int ox = GetActorX(caster), oy = GetActorY(caster);
	int oz = GetActorZ(caster) + GetActorViewHeight(caster) - DND_SCORCHRAY_ZOFF;

	LineAttack(caster, GetActorAngle(caster), GetActorPitch(caster), 0, "Spell_ScorchRayMarker",
		"None", len, FHF_NORANDOMPUFFZ | FHF_NOIMPACTDECAL, puff);

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
void CastWarmth(int pnum) {
	int pct = GetSpellValue(pnum, SPL_WARMTH, SPELLVAL_DAMAGE);
	int flat = (GetSpellValue(pnum, SPL_WARMTH, SPELLVAL_DAMAGE2) * DND_MANAREGEN_SCALE) >> 16;
	int dur = GetSpellDurationTics(pnum, SPL_WARMTH);

	// "Duration becomes 10 seconds" -- the table's 5 doubled, rather than a second figure to keep in
	// step with it.
	if(SpellThresholdMet(pnum, SPL_WARMTH, DND_SPELL_THRESH_LOW))
		dur *= 2;

	GiveSpellBuffAround(pnum, SPL_WARMTH, BTI_SPELL_WARMTH, (pct & 0xFFFF) | (flat << 16), dur,
		SpellThresholdMet(pnum, SPL_WARMTH, DND_SPELL_THRESH_HIGH));
}

// Heat Shield. DAMAGE is the armor rating. The ignite-on-being-hit half is carried by a marker item
// rather than by the buff, because the retaliation fires from the damage path and that path can read
// inventory far more cheaply than it can walk a buff list.
void CastHeatShield(int pnum) {
	int target = GetSpellSupportTarget(pnum);
	int dur = GetSpellDurationTics(pnum, SPL_HEATSHIELD);

	HandlePlayerBuffAssignment(target, pnum + P_TIDSTART, BTI_SPELL_HEATSHIELD, 0, 0, dur,
		GetSpellValue(pnum, SPL_HEATSHIELD, SPELLVAL_DAMAGE));

	// Rank is stamped on the marker so the retaliation can read the caster's thresholds off the
	// WEARER, who may not be the caster.
	SetActorInventory(target + P_TIDSTART, "DnD_HeatShieldRank", GetSpellRank(pnum, SPL_HEATSHIELD, true));
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
	HandlePlayerBuffAssignment(pnum, ptid, BTI_SPELL_BOILINGBLOOD, 0, 0, dur,
		GetSpellValue(pnum, SPL_BOILINGBLOOD, SPELLVAL_DAMAGE));
	HandlePlayerBuffAssignment(pnum, ptid, BTI_SPELL_BOILINGBLOOD_CDR, 0, 0, dur, cdr);

	ACS_NamedExecuteAlways("DnD Boiling Blood Drain", 0, pnum,
		GetSpellValue(pnum, SPL_BOILINGBLOOD, SPELLVAL_DAMAGE2), dur);
}

// The health price. Separate from the buff because the buff system grants values, it does not bill
// for them, and because this has to stop the moment the player dies.
Script "DnD Boiling Blood Drain" (int pnum, int per_second, int dur) {
	int ptid = pnum + P_TIDSTART;

	for(int t = 0; t < dur; t += TICRATE) {
		Delay(const:TICRATE);

		if(!PlayerInGame(pnum) || !IsActorAlive(ptid))
			Terminate;

		// Never lethal: the spell is a cost, not a suicide. One health is the floor.
		int hp = GetActorProperty(ptid, APROP_HEALTH);
		if(hp > per_second)
			Thing_Damage2(ptid, per_second, "SkipHandle");
	}
}

// Owns the Heat Shield marker only -- the armor is the buff's and expires on its own ticker. The end
// tic is stamped on the wearer so a recast takes ownership and the older timer bows out, rather than
// clearing the marker out from under the newer shield.
Script "DnD Heat Shield Timer" (int pnum, int endtic) {
	int ptid = pnum + P_TIDSTART;
	SetActorInventory(ptid, "DnD_HeatShieldEnd", endtic);

	while(Timer() < endtic && IsActorAlive(ptid) &&
		CheckActorInventory(ptid, "DnD_HeatShieldEnd") == endtic)
		Delay(const:TICRATE / 2);

	if(CheckActorInventory(ptid, "DnD_HeatShieldEnd") == endtic) {
		SetActorInventory(ptid, "DnD_HeatShieldRank", 0);
		SetActorInventory(ptid, "DnD_HeatShieldEnd", 0);
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
			dmg = GetGenericDoTDamage(pnum, CheckActorInventory(victim, "DnD_BlazeDamage"), victim, -1);

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

	int dmg = GetSpellValue(pnum, SPL_INFERNALSTRIKE, SPELLVAL_DAMAGE);
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
Script "DnD Spell Cast" (int spell, int pnum) {
	// The cast time is spent HERE rather than in TryCastSpell, which has already taken the mana and
	// started the cooldown -- so a cast that is interrupted still costs, and the cooldown runs from the
	// press rather than from the finish. The effect is what waits.
	// Deliberately does NOT lock the weapon. Only a channel does that, because only a channel is
	// driven by the attack button and so cannot share it. A plain cast time leaves the player free to
	// keep shooting through it.
	int ct = GetSpellCastTics(pnum, spell);
	if(ct > 0) {
		int casting = StartSpellCastBar(pnum, spell, ct);
		Delay(ct);
		EndSpellCastBar(pnum, casting);
	}

	int i, count, victim, temp;

	// "When you finish casting a fire spell" -- after the cast time, not at the press.
	CheckHeartOfFire(pnum, spell);

	switch(spell) {
		case SPL_BLAZE:
			victim = SpellHitscan(pnum, spell, "Spell_BlazePuff");
			if(!victim)
				break;

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

		case SPL_SCORCHINGRAY:
			ACS_NamedExecuteAlways("DnD Scorching Ray", 0, pnum);
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

#endif
