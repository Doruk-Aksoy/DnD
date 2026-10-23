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

// A fixed duration from a spell's text outside its DURATION row, scaled by the duration gear that row takes.
// Freezes are ailments, not spell effects, and stay out. The spell's gems add their MORE duration after.
int ScaleSpellEffectTics(int pnum, int spell, int tics) {
	return ApplySpellGemMore(pnum, spell, GEMSTAT_MORE_DURATION, tics * Max(0, 100 + PlayerModData[pnum].vals[PSTAT_SPELL_DURATION]) / 100);
}

enum {
	DND_LOOPSOUND_BLIZZARD,
	DND_LOOPSOUND_AVALANCHE
};

str GetSpellLoopSound(int which) {
	if(which == DND_LOOPSOUND_AVALANCHE)
		return "Avalanche/Loop";
	return "Blizzard/Loop";
}

#define DND_LOOPSOUND_NEAR 256.0        // a spell loop is at full volume within this of its spot
#define DND_LOOPSOUND_FAR 2048.0        // and silent past this

// Per machine: the newest loop of each kind owns its channel, so an older one ending cannot cut it.
int[] module& GetSpellLoopTickets() {
	static int tickets[2];
	return tickets;
}

// Starts a spell loop at a spot on every machine for tics, or until the caster leaves (or dies, with alive).
void StartSpellLoopSound(int pnum, int which, int x, int y, int z, int tics, bool alive) {
	ACS_NamedExecuteAlways("DnD Spell Loop Sound", 0, ((x >> 16) & 0xFFFF) | (y & 0xFFFF0000),
		((z >> 16) & 0xFFFF) | (tics << 16), which | (pnum << 8) | (alive << 16));
}

// How loud a spell loop at x, y, z is to this listener: squared, so it eases off as a normal sound would.
int SpellLoopVolume(int me, int x, int y, int z) {
	int d = VectorLength(VectorLength(GetActorX(me) - x, GetActorY(me) - y), GetActorZ(me) - z);
	if(d <= DND_LOOPSOUND_NEAR)
		return 1.0;
	if(d >= DND_LOOPSOUND_FAR)
		return 0;
	int v = FixedDiv(DND_LOOPSOUND_FAR - d, DND_LOOPSOUND_FAR - DND_LOOPSOUND_NEAR);
	return FixedMul(v, v);
}

// Played on the listener's own player, the one place the engine ranks a sound high (80) so nothing crowds it
// out, and turned by their distance to the spot every tic. A start at volume 0 never plays, hence the floor.
Script "DnD Spell Loop Sound" (int xy, int zt, int packed) CLIENTSIDE {
	int x = xy << 16, y = xy & 0xFFFF0000, z = zt << 16, tics = zt >> 16;
	int which = packed & 0xFF, pnum = (packed >> 8) & 0xFF;
	bool alive = packed >> 16;
	int me = ConsolePlayerNumber() + P_TIDSTART;
	int chan = which == DND_LOOPSOUND_AVALANCHE ? CHAN_6 : CHAN_7;
	int[] module& tickets = GetSpellLoopTickets();
	int ticket = ++tickets[which];

	PlaySound(me, GetSpellLoopSound(which), chan, Max(SpellLoopVolume(me, x, y, z), 0.01), true);
	for(int t = 0; t < tics && ticket == tickets[which] && PlayerInGame(pnum) &&
		(!alive || IsActorAlive(pnum + P_TIDSTART)); ++t) {
		Delay(const:1);
		SoundVolume(me, chan, SpellLoopVolume(me, x, y, z));
	}
	if(ticket == tickets[which])
		StopSound(me, chan);
}

// An aura's reach before area modifiers. Hatred's rank 5 doubles it; the ring art reads this too.
#define DND_HATRED_R5_RADIUSMULT 2

int GetSpellAuraRadius(int pnum, int spell) {
	int r = GetSpellValue(pnum, spell, SPELLVAL_RADIUS);
	if(spell == SPL_HATRED && SpellThresholdMet(pnum, SPL_HATRED, DND_SPELL_THRESH_LOW))
		r *= DND_HATRED_R5_RADIUSMULT;
	return r;
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
	int r = ScaleSpellArea(pnum, spell, GetSpellAuraRadius(pnum, spell) << 16);
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

	bool hatred = IsSpellActive(pnum, SPL_HATRED);
	if(hatred)
		ApplyHatredAura(pnum, DND_SPELLAURA_REFRESH);
	SetPlayerAttachment(pnum, DND_PLAYERFX_HATREDAURA, hatred);

	bool wrath = IsSpellActive(pnum, SPL_WRATH);
	if(wrath)
		ApplyWrathAura(pnum, DND_SPELLAURA_REFRESH);
	SetPlayerAttachment(pnum, DND_PLAYERFX_WRATHAURA, wrath);

	bool agony = IsSpellActive(pnum, SPL_AGONY);
	if(agony)
		ApplyAgonyAura(pnum, DND_SPELLAURA_REFRESH);
	SetPlayerAttachment(pnum, DND_PLAYERFX_AGONYAURA, agony);
	SetAgonyZone(pnum, agony && SpellThresholdMet(pnum, SPL_AGONY, DND_SPELL_THRESH_LOW) ?
		ScaleSpellArea(pnum, SPL_AGONY, GetSpellAuraRadius(pnum, SPL_AGONY) << 16) : 0);

	// Energy Blade: the marker follows the toggle, and the shield is brought under its new cap whenever it changes.
	int less = IsSpellRunning(pnum, SPL_ENERGYBLADE) ? GetEnergyBladeLess(pnum) : 0;
	if(CheckActorInventory(pnum + P_TIDSTART, "DnD_EnergyBladeLess") != less) {
		SetActorInventory(pnum + P_TIDSTART, "DnD_EnergyBladeLess", less);
		HandleEShieldChange(pnum, true);
	}
	// A regeneration begun before the cut fills toward the old cap, so the shield is held under the new one.
	else if(less && CheckInventory("EShieldAmount") > GetPlayerEnergyShieldCap(pnum))
		SetEnergyShield(GetPlayerEnergyShieldCap(pnum));

	// Static Field zaps on its own clock, so it gets a loop of its own; this pass restarts it after a death.
	if(IsSpellActive(pnum, SPL_STATICFIELD) && !IsSpellRunning(pnum, SPL_STATICFIELD)) {
		SetSpellRunning(pnum, SPL_STATICFIELD, true);
		ACS_NamedExecuteAlways("DnD Static Field", 0, pnum);
	}
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
		int r = ScaleSpellArea(pnum, SPL_SEARINGBOND, GetSpellValue(pnum, SPL_SEARINGBOND, SPELLVAL_RADIUS) << 16);
		int i, mn, s;

		for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
			i = UsedMonsterTIDs[mn];
			if(i == victim || !IsActorAlive(i) || !IsSpellEnemy(i))
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

// Length and width both take the area stats through ScaleSpellArea, the NON-WEAPON source: the design counts
// area modifiers here when they are not limited to attacks.
int GetScorchRayLength(int pnum) {
	return ScaleSpellArea(pnum, SPL_SCORCHINGRAY,
		DND_SCORCHRAY_BASELEN + (GetSpellValue(pnum, SPL_SCORCHINGRAY, SPELLVAL_RADIUS) << 16));
}

int GetScorchRayWidth(int pnum) {
	int w = DND_SCORCHRAY_BASEWIDTH;

	// "+25% beam width."
	if(SpellThresholdMet(pnum, SPL_SCORCHINGRAY, DND_SPELL_THRESH_LOW))
		w = w * (100 + DND_SCORCHRAY_WIDTHR5) / 100;

	return ScaleSpellArea(pnum, SPL_SCORCHINGRAY, w);
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
			int ray_dmg = PriceSpellDamage(pnum, SPL_SCORCHINGRAY, GetSpellValue(pnum, SPL_SCORCHINGRAY, SPELLVAL_DAMAGE));

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
		dur = ScaleSpellEffectTics(pnum, SPL_WARMTH, DND_WARMTH_R5_DURATION);

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
		if(!first && !HasDebugFreeSpells(pnum)) {
			if(GetPlayerMana(pnum) < cost)
				break;
			SetPlayerMana(pnum, GetPlayerMana(pnum) - cost);
		}
		first = false;

		// One per damage pass, on its own channel so it never cuts the activation. Per monster would
		// stack a copy for every enemy in the ring.
		PlaySound(ptid, "Immolation/Proc", CHAN_7);

		r = ScaleSpellArea(pnum, SPL_IMMOLATION, GetSpellValue(pnum, SPL_IMMOLATION, SPELLVAL_RADIUS) << 16);
		// A tick (it lands as damage over time), so it prices through the DoT pile.
		dmg = PriceSpellDamage(pnum, SPL_IMMOLATION, GetSpellValue(pnum, SPL_IMMOLATION, SPELLVAL_DAMAGE),
			DND_WDMG_ISRADIUSDMG, true);

		for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
			v = UsedMonsterTIDs[mn];
			if(!IsActorAlive(v) || !IsSpellEnemy(v))
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

#define DND_HATRED_R10_CRIT 0.05        // "5% additional crit chance to cold spells and attacks"

// Hatred, Anger's cold twin. DAMAGE is 16.16 with a 1.5 step, so it is handed over in hundredths of
// a percent rather than whole ones, or every odd rank would lose its half.
void ApplyHatredAura(int pnum, int dur) {
	GiveSpellBuffAround(pnum, SPL_HATRED, BTI_SPELL_HATRED,
		(GetSpellValue(pnum, SPL_HATRED, SPELLVAL_DAMAGE) * 100) >> 16, dur, true);

	if(SpellThresholdMet(pnum, SPL_HATRED, DND_SPELL_THRESH_HIGH))
		GiveSpellBuffAround(pnum, SPL_HATRED, BTI_SPELL_HATRED_CRIT, DND_HATRED_R10_CRIT, dur, true);
}

#define DND_WRATH_R5_OVERLOAD 5
#define DND_WRATH_R10_LESS 50

// Wrath, Anger's lightning twin: more lightning damage and base overload chance, and at rank 10 shorter overloads on whoever it covers.
void ApplyWrathAura(int pnum, int dur) {
	GiveSpellBuffAround(pnum, SPL_WRATH, BTI_SPELL_WRATH, GetSpellValue(pnum, SPL_WRATH, SPELLVAL_DAMAGE) >> 16, dur, true);
	GiveSpellBuffAround(pnum, SPL_WRATH, BTI_SPELL_WRATH_OVERLOAD, GetSpellValue(pnum, SPL_WRATH, SPELLVAL_DAMAGE2) +
		DND_WRATH_R5_OVERLOAD * SpellThresholdMet(pnum, SPL_WRATH, DND_SPELL_THRESH_LOW), dur, true);

	if(SpellThresholdMet(pnum, SPL_WRATH, DND_SPELL_THRESH_HIGH))
		GiveSpellBuffAround(pnum, SPL_WRATH, BTI_SPELL_WRATH_GROUNDED, DND_WRATH_R10_LESS, dur, true);
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
			// The DoT pile (flat, increased, DoT multi, ailment more, INC_CRITFORDOT), recomputed per tick so gear changed
			// mid burn counts. The tick never ROLLS a crit; ISDAMAGEOVERTIME below is what stops that.
			dmg = PriceSpellDamage(pnum, SPL_BLAZE, CheckActorInventory(victim, "DnD_BlazeDamage"), 0, true, victim);

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
		// extra is the caster + 1 when the cast itself lands, for the beam or arc from them.
		case SPL_SPARK:
			SpawnBodyCrackle(victim_tid, "Spell_Spark_Hit");
		break;

		case SPL_SHOCK:
			SpawnShockBeam(extra - 1 + P_TIDSTART, victim_tid);
		break;

		case SPL_TELEKINESIS:
			SpawnCrackleLine(extra - 1 + P_TIDSTART, victim_tid, "Spell_Telekinesis_Arc", "Spell_Telekinesis_Burst");
		break;


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
	r = ScaleSpellArea(pnum, SPL_INCINERATE, r << 16);

	// The burst FX, on the corpse the proc fired on. Given rather than spawned here because the actor
	// is CLIENTSIDEONLY and this script is the server's -- the token is what crosses.
	GiveActorInventory(victim, "Spell_Incinerate_BurstFXSpawner", 1);

	// Spell damage scales what the burst deals to enemies; the caster's own half below stays the raw share.
	int hit = PriceSpellDamage(pnum, SPL_INCINERATE, dmg, DND_WDMG_ISRADIUSDMG);
	int i, mn, out, source = pnum + P_TIDSTART;
	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		if(i == victim || !IsActorAlive(i) || !IsSpellEnemy(i))
			continue;
		if(fdistance(victim, i) > r)
			continue;

		out = HandleDamageDeal(source, i, hit, DND_DAMAGETYPE_FIRE, SPL_INCINERATE,
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

// The boulder drops from the ceiling with this as its horizontal push, then takes its velocity over from its own states.
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
	return ScaleSpellArea(pnum, SPL_FLAMMABILITY, GetSpellValue(pnum, SPL_FLAMMABILITY, SPELLVAL_RADIUS) << 16,
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

// Frostbite, Flammability's cold twin: the same targeting, with a cold exposure and a chill lane.
#define DND_FROSTBITE_R5_AOE 25
#define DND_FROSTBITE_R10_TICS (4 * TICRATE)

int GetFrostbiteRadius(int pnum) {
	// "25% increased area of effect" -- increased, so it joins the additive pool.
	return ScaleSpellArea(pnum, SPL_FROSTBITE, GetSpellValue(pnum, SPL_FROSTBITE, SPELLVAL_RADIUS) << 16,
		DND_FROSTBITE_R5_AOE * SpellThresholdMet(pnum, SPL_FROSTBITE, DND_SPELL_THRESH_LOW));
}

// Flammability's ring, drawn with frost bursts instead. CLIENTSIDE for the same reason.
Script "DnD Frostbite Area FX" (int cx, int cy, int cz, int r) CLIENTSIDE {
	if(r <= 0) {
		SetResultValue(0);
		Terminate;
	}

	int n = Clamp_Between((FixedDiv(r, DND_FLAMMABILITY_RINGSTEP) >> 16) * 6,
		DND_FLAMMABILITY_RINGMIN, DND_FLAMMABILITY_RINGMAX);

	int i, a;
	for(i = 0; i < n; ++i) {
		a = (i * 1.0) / n;
		SpawnForced("FrostbiteAreaMarkerFX",
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
// ice draws Blizzard's ring instead of the fire one.
Script "DnD Rain Of Fire Mark" (int cx, int cy, int packed, int ice) CLIENTSIDE {
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
		SpawnForced(ice ? "Spell_BlizzardMarkerFX" : "Spell_RainOfFireMarkerFX",
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
	int scatter = ScaleSpellArea(pnum, SPL_RAINOFFIRE,
		GetSpellValue(pnum, SPL_RAINOFFIRE, SPELLVAL_AMOUNT) << 16);
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
	int volcano = Max(0, GetSpellValue(pnum, SPL_RAINOFFIRE, SPELLVAL_DAMAGE2));

	for(t = 0; t < dur; t += DND_RAINOFFIRE_RATE) {
		if(!PlayerInGame(pnum))
			break;

		// Redrawn on its own slower cadence so the ring persists without a marker per comet.
		if(!(t % DND_RAINOFFIRE_MARKRATE))
			ACS_NamedExecuteWithResult("DnD Rain Of Fire Mark", cx, cy,
				((scatter >> 16) << 16) | ((ground >> 16) & 0xFFFF));

		// A VOLLEY, not a comet. Each one rolls its own landing point and its own slant, so they come
		// down together but never as a pair on the same line.
		// Volcano's synergy adds whole comets; rank 5's 15% is then of the whole volley.
		n = DND_RAINOFFIRE_PERVOLLEY + volcano;
		if(r5) {
			comet_acc += n * DND_RAINOFFIRE_R5_MORECOMETS;
			n += comet_acc / 100;
			comet_acc %= 100;
		}
		n += GetSpellExtraProjectiles(pnum, SPL_RAINOFFIRE);

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

			// The roof was read at the middle: start under the ceiling where this one actually is, or not at all.
			dz = GetActorCeilingZ(tid) - DND_RAINOFFIRE_HEADROOM;
			if(dz < GetActorFloorZ(tid) + DND_RAINOFFIRE_HEADROOM || !SetActorPosition(tid, sx, sy, Min(dz, roof), 0)) {
				Thing_Remove(tid);
				continue;
			}

			// Stamps the spell identity, which is what the Damage expression and the blast radius both
			// resolve off. Without it the comet prices itself as spell 0.
			SetupSpellActor(tid, pnum, SPL_RAINOFFIRE);

			dx = lx - sx;
			dy = ly - sy;
			dz = ground - GetActorZ(tid);
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
	// Witch: more curse effect on both of what the curse does.
	int pct = ApplySpellGemMore(pnum, SPL_FLAMMABILITY, GEMSTAT_MORE_CURSE, GetSpellValue(pnum, SPL_FLAMMABILITY, SPELLVAL_DAMAGE));
	int chance = ApplySpellGemMore(pnum, SPL_FLAMMABILITY, GEMSTAT_MORE_CURSE, GetSpellValue(pnum, SPL_FLAMMABILITY, SPELLVAL_DAMAGE2));

	int i, mn;
	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		if(!IsActorAlive(i) || !IsSpellEnemy(i))
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

// Same two ways in as Flammability, and the same one ticker per monster owning its marker.
Script "DnD Frostbite Cast" (int pnum) {
	int caster = pnum + P_TIDSTART;
	int cx, cy, cz;

	PlaySound(caster, "Frostbite/Cast", 6);

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

	int r = GetFrostbiteRadius(pnum);
	int tics = GetSpellDurationTics(pnum, SPL_FROSTBITE) +
		ScaleSpellEffectTics(pnum, SPL_FROSTBITE, DND_FROSTBITE_R10_TICS) * SpellThresholdMet(pnum, SPL_FROSTBITE, DND_SPELL_THRESH_HIGH);

	ACS_NamedExecuteWithResult("DnD Frostbite Area FX", cx, cy, cz, r);
	// Witch, as for Flammability.
	int pct = ApplySpellGemMore(pnum, SPL_FROSTBITE, GEMSTAT_MORE_CURSE, GetSpellValue(pnum, SPL_FROSTBITE, SPELLVAL_DAMAGE));
	int chance = ApplySpellGemMore(pnum, SPL_FROSTBITE, GEMSTAT_MORE_CURSE, GetSpellValue(pnum, SPL_FROSTBITE, SPELLVAL_DAMAGE2));

	int i, mn;
	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		if(!IsActorAlive(i) || !IsSpellEnemy(i))
			continue;
		if(fdistance_delta(cx - GetActorX(i), cy - GetActorY(i), cz - GetActorZ(i)) > r)
			continue;

		ApplyColdExposure(i, pct, tics);

		if(CheckActorInventory(i, "DnD_Frostbitten") < chance)
			SetActorInventory(i, "DnD_Frostbitten", chance);

		bool ticking = !!CheckActorInventory(i, "DnD_FrostbittenTimer");
		if(CheckActorInventory(i, "DnD_FrostbittenTimer") < tics)
			SetActorInventory(i, "DnD_FrostbittenTimer", tics);

		if(!ticking)
			ACS_NamedExecuteAlways("DnD Frostbite Timer", 0, i);
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

void CastMoltenBoulder(int pnum, int angle_off = 0) {
	int caster = pnum + P_TIDSTART;
	int tid = TEMPORARY_SPELL_TID + pnum;
	int a = GetActorAngle(caster) + angle_off;

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

	// Kept out of MORE spell damage (noMore), as it always has been.
	int dmg = PriceSpellDamage(pnum, SPL_INFERNALSTRIKE, GetSpellValue(pnum, SPL_INFERNALSTRIKE, SPELLVAL_DAMAGE),
		0, false, -1, true);
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

	int r = ScaleSpellArea(pnum, SPL_INFERNALSTRIKE, GetSpellValue(pnum, SPL_INFERNALSTRIKE, SPELLVAL_RADIUS) << 16);
	int i, mn;
	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		if(i == victim || !IsActorAlive(i) || !IsSpellEnemy(i))
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
#define DND_ICENOVA_Z 8.0
#define DND_GLACIALSPIKE_SPEED 34

// Two seconds sits in chill's own rhythm -- its stacks decay one a second -- and outlasts the
// 1 second cast without becoming permanent on everything you touch.
#define DND_ICEBOLT_SLOWTICS (2 * TICRATE)

#define DND_ICEBOLT_R5_SLOW 75

// Up here rather than with the rest of Chilling Breath: the cold on-hit seam reads them and sits
// above that block in the file.
#define DND_CHILLINGBREATH_R5_SLOW 50
#define DND_CHILLINGBREATH_SLOWTICS (2 * TICRATE)

// Same reason, for Frost Bomb.
#define DND_FROSTBOMB_NOREGEN (5 * TICRATE)
#define DND_FROSTBOMB_R5_EXPOSE 15      // rank 5: cold resistance points taken off
#define DND_FROSTBOMB_EXPOSETICS (4 * TICRATE)

// ============================ Freezing Pulse ============================
// A fan of ripping sub-projectiles whose boxes overlap into the crescent the spell is meant to be.
// One actor cannot be a crescent: a Doom hitbox is an axis aligned BOX of Radius x Radius x Height,
// so a shape is composed out of several of them or not at all.
//
// Laterally they sit a step apart; forward they lag by the sagitta of the arc they sit on, which is
// quadratic in that step -- the outer pair falls back four times as far as the inner pair. The lag
// is applied as a SPEED difference rather than a spawn offset so the bow deepens as the wave
// travels, which is what the reference art does.

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

#define DND_FREEZINGPULSE_FREEZETICS TICRATE	// freeze length

// Per caster: when the pulse left. A module& must return a reference to a STRUCT, so the cell is one.
typedef struct {
	int val;
} pulse_cell_T;

pulse_cell_T module& GetPulseCastTic(int pnum) {
	static pulse_cell_T tics[MAXPLAYERS];
	return tics[pnum];
}

// How much wider than base this cast is: one factor, so the arcs and the model drawn over them never drift apart.
int GetPulseWidthFactor(int pnum) {
	int f = ScaleSpellArea(pnum, SPL_FREEZINGPULSE, 1.0);

	// Rank 10 is width itself, as Scorching Ray's and Chilling Breath's are; through the area pool it was only 12%.
	if(SpellThresholdMet(pnum, SPL_FREEZINGPULSE, DND_SPELL_THRESH_HIGH))
		f = f * (100 + DND_FREEZINGPULSE_R10_WIDTH) / 100;
	return f;
}

// Set by the carrier on its first tic. scaleX drives the model's x and y, scaleY its z, so one factor grows it evenly.
Script "DnD Freezing Pulse Scale" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum != -1) {
		int f = GetPulseWidthFactor(pnum);

		// Multiplied in, not assigned: the DECORATE Scale stays the base size.
		SetActorProperty(0, APROP_SCALEX, FixedMul(GetActorProperty(0, APROP_SCALEX), f));
		SetActorProperty(0, APROP_SCALEY, FixedMul(GetActorProperty(0, APROP_SCALEY), f));
	}

	SetResultValue(0);
}

// How many tics a pulse is in the air, which is also the span its freeze chance decays over.
int GetPulseFlightTics(int pnum) {
	int range = GetSpellValue(pnum, SPL_FREEZINGPULSE, SPELLVAL_RADIUS);
	if(SpellThresholdMet(pnum, SPL_FREEZINGPULSE, DND_SPELL_THRESH_LOW))
		range = range * (100 + DND_FREEZINGPULSE_R5_RANGE) / 100;

	return Max(1, range / DND_FREEZINGPULSE_SPEED);
}


void CastFreezingPulse(int pnum) {
	auto tic = GetPulseCastTic(pnum);
	tic.val = Timer();

	int life = GetPulseFlightTics(pnum);

	// Same factor the carrier scales its model by, so the two never drift apart.
	int step = FixedMul(DND_FREEZINGPULSE_SPACING, GetPulseWidthFactor(pnum));

	// The arcs carry the damage; Spell_FreezingPulse below is the visible carrier. "+X projectiles" adds whole fanned
	// waves, each with its own ripper id, so a wave hits a monster once and two overlapping waves hit it twice.
	int k, off, w, wa, rid;
	int mid = DND_FREEZINGPULSE_COUNT / 2;
	for(w = 0; w < 1 + GetSpellExtraProjectiles(pnum, SPL_FREEZINGPULSE); ++w) {
		wa = FanOffset(w, DND_SPELL_FANSPREAD * 2);
		rid = ReserveRipperId() + 1;
		for(k = 0; k < DND_FREEZINGPULSE_COUNT; ++k) {
			off = k - mid;

			// off * off is the sagitta: arc depth grows with the square of the lateral step, so the wave bows.
			SpawnSpellProjectile(pnum, SPL_FREEZINGPULSE, "Spell_FreezingPulse_Part",
				DND_FREEZINGPULSE_SPEED - DND_FREEZINGPULSE_LAG * off * off,
				0, wa + DND_FREEZINGPULSE_SPREAD * off, step * off, life, 0, true, DND_FREEZINGPULSE_Z, 0, -1, rid);
		}

		// One carrier per wave, centred: the model IS the whole cascade, so one per arc would nest five.
		SpawnSpellProjectile(pnum, SPL_FREEZINGPULSE, "Spell_FreezingPulse",
			DND_FREEZINGPULSE_SPEED, 0, wa, 0, life, 0, true, DND_FREEZINGPULSE_Z);
	}
}


// ============================ cold, on hit ============================
// Reached from "DnD Damage Accumulate" the moment a cold SPELL damages a monster, so a spell never
#define DND_AVALANCHE_R5_FREEZE 15      // percent chance a piece freezes, rank 5
#define DND_AVALANCHE_FREEZETICS TICRATE

Script "DnD Spell Cold On Hit" (int pnum, int victim, int spell) {
	int pct, chance, gone, span;

	switch(spell) {
		case SPL_ICEBOLT:
			pct = SpellThresholdMet(pnum, SPL_ICEBOLT, DND_SPELL_THRESH_LOW) ?
				DND_ICEBOLT_R5_SLOW : GetSpellValue(pnum, SPL_ICEBOLT, SPELLVAL_DAMAGE2);
			SlowMonster(victim, pct, ScaleSpellEffectTics(pnum, SPL_ICEBOLT, DND_ICEBOLT_SLOWTICS));
		break;

		case SPL_CHILLINGBREATH:
			// Rank 5 only; below it the breath just damages.
			if(SpellThresholdMet(pnum, SPL_CHILLINGBREATH, DND_SPELL_THRESH_LOW))
				SlowMonster(victim, DND_CHILLINGBREATH_R5_SLOW, ScaleSpellEffectTics(pnum, SPL_CHILLINGBREATH, DND_CHILLINGBREATH_SLOWTICS));
		break;

		case SPL_FROSTBOMB:
			// Every pulse refreshes it, so standing in the crystal is a rolling 5 seconds of no regen.
			PauseMonsterRegen(victim, DND_FROSTBOMB_NOREGEN);

			if(SpellThresholdMet(pnum, SPL_FROSTBOMB, DND_SPELL_THRESH_LOW))
				ApplyColdExposure(victim, DND_FROSTBOMB_R5_EXPOSE, ScaleSpellEffectTics(pnum, SPL_FROSTBOMB, DND_FROSTBOMB_EXPOSETICS));
		break;

		case SPL_FREEZINGPULSE:
			// Full at the muzzle, nothing at the edge: time since the cast stands in for distance, as the fan holds its speed.
			span = GetPulseFlightTics(pnum);
			gone = Clamp_Between(Timer() - GetPulseCastTic(pnum).val, 0, span);

			chance = GetSpellValue(pnum, SPL_FREEZINGPULSE, SPELLVAL_DAMAGE2) * (span - gone) / span;
			if(chance > 0 && random(1, 100) <= chance && FreezeMonster(pnum, victim, DND_FREEZINGPULSE_FREEZETICS))
				RollSpellGemCharge(pnum, spell, GEMSTAT_FRENZYCHANCE);
		break;

		case SPL_WINTERORB:
			// One DURATION carries the slow and, from rank 5, the freeze -- it is what Gust's synergy lengthens.
			span = GetSpellDurationTics(pnum, SPL_WINTERORB);
			if(SpellThresholdMet(pnum, SPL_WINTERORB, DND_SPELL_THRESH_LOW)) {
				if(FreezeMonster(pnum, victim, span))
					RollSpellGemCharge(pnum, spell, GEMSTAT_FRENZYCHANCE);
			}
			else
				SlowMonster(victim, GetSpellValue(pnum, SPL_WINTERORB, SPELLVAL_DAMAGE2), span);
		break;

		case SPL_AVALANCHE:
			if(SpellThresholdMet(pnum, SPL_AVALANCHE, DND_SPELL_THRESH_LOW) && random(1, 100) <= DND_AVALANCHE_R5_FREEZE &&
				FreezeMonster(pnum, victim, DND_AVALANCHE_FREEZETICS))
				RollSpellGemCharge(pnum, spell, GEMSTAT_FRENZYCHANCE);
		break;
	}

	SetResultValue(0);
}

// ============================ Ice Shield ============================
// The spell-system port of the legacy skill. Numbers off the row, and its own fade token.

#define DND_ICESHIELD_R5_BONUS 2.5    // extra seconds at rank 5
#define DND_ICESHIELD_FADETICS 20     // how long the barriers take to fade once told to

Script "DnD Ice Shield" (int pnum) {
	int caster = pnum + P_TIDSTART;
	int tid = TEMPORARY_SPELL_TID + pnum;

	int count = Max(1, GetSpellValue(pnum, SPL_ICESHIELD_S, SPELLVAL_AMOUNT));
	int hp = Max(1, GetSpellValue(pnum, SPL_ICESHIELD_S, SPELLVAL_DAMAGE));

	// Intellect, as a percent of the ranked value. INT is the spell stat, so a defensive pool
	// answers to it the same way minion health does.
	hp += hp * GetIntellectEffect(pnum, DND_SPELL_BARRIERHP_PER_INT) / 10000;
	bool hard = SpellThresholdMet(pnum, SPL_ICESHIELD_S, DND_SPELL_THRESH_HIGH);

	int tics = GetSpellDurationTics(pnum, SPL_ICESHIELD_S);
	if(SpellThresholdMet(pnum, SPL_ICESHIELD_S, DND_SPELL_THRESH_LOW))
		tics += (DND_ICESHIELD_R5_BONUS * TICRATE) >> 16;

	// A stale token from a previous cast would fade this one on its first tic.
	TakeActorInventory(caster, "Spell_IceShieldFade", 1);

	ActivatorSound("Spell/IceShieldCast", 127);

	int i;
	for(i = 0; i < count; ++i) {
		// Byte angle, evenly around the caster.
		if(!SpawnForced("Spell_IceShieldBarrier", GetActorX(caster), GetActorY(caster),
			GetActorZ(caster), tid, (i * 255) / count))
			continue;

		SetActivator(tid);
		SetPointer(AAPTR_TRACER, caster);
		SetActorProperty(0, APROP_HEALTH, hp);
		if(hard)
			GiveInventory("Spell_IceShieldHarden", 1);
		Thing_ChangeTID(0, 0);
		SetActivator(caster);
	}

	Delay(tics);

	GiveActorInventory(caster, "Spell_IceShieldFade", 1);
	Delay(const:DND_ICESHIELD_FADETICS);
	TakeActorInventory(caster, "Spell_IceShieldFade", 1);
}

// ============================ Glacial Spike ============================
// Rips each enemy once through RIPSONCE, and stops after AMOUNT of them. The cap is counted on the

#define DND_GLACIALSPIKE_LIFE 105      // tics in the air before it shatters on its own
#define DND_GLACIALSPIKE_R5_FREEZE 2.0
#define DND_GLACIALSPIKE_SHATTER 96.0 // how far the shatter freezes

Script "DnD Glacial Spike Setup" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum != -1) {
		// DnD_RipLimit is the mod's own rip bookkeeping: the handler tallies DnD_RipCount per NEW
		// victim and hands out TakeRipperAway at the limit.
		SetSpellRipLimit(pnum, 0, Max(1, GetSpellValue(pnum, SPL_GLACIALSPIKE, SPELLVAL_AMOUNT)));

		// Rank 10: rips through Hardened Skin. Still only AMOUNT of them.
		if(SpellThresholdMet(pnum, SPL_GLACIALSPIKE, DND_SPELL_THRESH_HIGH))
			GiveInventory("DnD_RipThruHardened", 1);
	}

	SetActorProperty(0, APROP_REACTIONTIME, DND_GLACIALSPIKE_LIFE);
	SetResultValue(0);
}

// One shot on impact, not a per-tic sweep: the shatter happens once and then the spike is gone.
Script "DnD Glacial Spike Shatter" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1)
		Terminate;

	int tics = GetSpellDurationTics(pnum, SPL_GLACIALSPIKE);
	if(SpellThresholdMet(pnum, SPL_GLACIALSPIKE, DND_SPELL_THRESH_LOW))
		tics = (DND_GLACIALSPIKE_R5_FREEZE * TICRATE) >> 16;

	int r = ScaleSpellArea(pnum, SPL_GLACIALSPIKE, DND_GLACIALSPIKE_SHATTER);
	int cut = r >> 16;
	int ox = GetActorX(0) >> 16, oy = GetActorY(0) >> 16, oz = GetActorZ(0) >> 16;

	int i, mn, dx, dy, dz;
	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];

		// Box reject first: UsedMonsterTIDs counts every monster the map ever spawned.
		dx = (GetActorX(i) >> 16) - ox;
		if(dx > cut || dx < -cut)
			continue;
		dy = (GetActorY(i) >> 16) - oy;
		if(dy > cut || dy < -cut)
			continue;
		dz = (GetActorZ(i) >> 16) - oz;
		if(dz > cut || dz < -cut)
			continue;

		if(dx * dx + dy * dy + dz * dz > cut * cut)
			continue;
		if(!IsActorAlive(i) || !IsSpellEnemy(i))
			continue;

		if(FreezeMonster(pnum, i, tics))
			RollSpellGemCharge(pnum, SPL_GLACIALSPIKE, GEMSTAT_FRENZYCHANCE);
	}
}

// ============================ Chilling Breath ============================
// Channelled. A constant stream of short ripping puffs, each its own attack.

#define DND_CHILLINGBREATH_TICK 17     // tics between MANA payments, ie. half a second
#define DND_CHILLINGBREATH_GAPMIN 2    // and between puffs, which is a separate clock now
#define DND_CHILLINGBREATH_GAPMAX 3
#define DND_CHILLINGBREATH_EXTRA 33    // percent chance of a second puff in the same breath
#define DND_CHILLINGBREATH_ARC 0.030   // half-arc, fixed point angle. ~22 degrees total
#define DND_CHILLINGBREATH_VARC 0.022  // and vertically, so the cone has height
#define DND_CHILLINGBREATH_JITTER 5    // speed either side of base, so they do not fly as a wall
#define DND_CHILLINGBREATH_R10_WIDE 30 // percent wider at rank 10
#define DND_CHILLINGBREATH_SPEED 24
#define DND_CHILLINGBREATH_SMOKE 2     // smoke puffs a breath
#define DND_CHILLINGBREATH_SMOKEZ 15.0 // below the eye, where the breath leaves
#define DND_CHILLINGBREATH_SMOKEFWD 24.0
#define DND_CHILLINGBREATH_SMOKESPD 11

// The smoke. PlasmaSmoke2 is +CLIENTSIDEONLY through DnD_SpecialFX, so a server side spawn of it
// draws on nobody's screen -- the client has to make its own, as Scorching Ray's beads do.
Script "DnD Chilling Breath FX" (int arc) CLIENTSIDE {
	if(!isAlive())
		Terminate;

	int a = GetActorAngle(0), p = GetActorPitch(0);
	int ox = GetActorX(0), oy = GetActorY(0);
	int oz = GetActorZ(0) + GetActorViewHeight(0) - DND_CHILLINGBREATH_SMOKEZ;

	// One tid, reused and released for each puff of smoke in turn.
	int tid = TEMPORARY_DATADUMMY_TID + ConsolePlayerNumber();

	int k, sa, sp, cp;
	for(k = 0; k < DND_CHILLINGBREATH_SMOKE; ++k) {
		// Its own scatter, so the smoke fills the cone rather than tracing the exact aim.
		sa = (a + random(-arc, arc) + 1.0) % 1.0;
		sp = Clamp_Between(p + random(-DND_CHILLINGBREATH_VARC, DND_CHILLINGBREATH_VARC),
			-0.248, 0.248);
		cp = cos(sp);

		// Forced: smoke is scenery and must not be dropped because something happens to be stood
		// in front of the caster.
		if(!SpawnForced("PlasmaSmoke2",
			ox + FixedMul(DND_CHILLINGBREATH_SMOKEFWD, FixedMul(cos(sa), cp)),
			oy + FixedMul(DND_CHILLINGBREATH_SMOKEFWD, FixedMul(sin(sa), cp)),
			oz - FixedMul(DND_CHILLINGBREATH_SMOKEFWD, sin(sp)), tid, 0))
			continue;

		SetActorVelocity(tid,
			DND_CHILLINGBREATH_SMOKESPD * FixedMul(cos(sa), cp),
			DND_CHILLINGBREATH_SMOKESPD * FixedMul(sin(sa), cp),
			-sin(sp) * DND_CHILLINGBREATH_SMOKESPD, false, false);

		// Released before the next one takes it. The smoke retags itself on its own first tic.
		Thing_ChangeTID(tid, 0);
	}

	SetResultValue(0);
}

// One puff. Pulled out because the breath throws a second one a third of the time.
void BreatheChillingPuff(int pnum, int arc) {
	// Before the projectile: SpawnSpellProjectile moves the activator about, and the FX needs the
	// caster to still be it.
	ACS_NamedExecuteAlways("DnD Chilling Breath FX", 0, arc);

	SpawnSpellProjectile(pnum, SPL_CHILLINGBREATH, "Spell_ChillingBreath",
		DND_CHILLINGBREATH_SPEED + random(-DND_CHILLINGBREATH_JITTER, DND_CHILLINGBREATH_JITTER),
		0, random(-arc, arc), 0, 0, 0, false, DND_SPELLPROJ_FROMVIEW,
		random(-DND_CHILLINGBREATH_VARC, DND_CHILLINGBREATH_VARC));
}

// No ripper id is claimed here on purpose. Every puff is its OWN attack, and HandleRipperHit
// reserves an id for a projectile that has none -- so each lands once on each enemy it passes.
Script "DnD Chilling Breath Claim" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	// MAX_RIPCOUNT is the one limit that skips the rip budget. UNSET reads as 0, which strips the
	// ripper on contact number one -- which is why the breath was not ripping at all.
	SetSpellRipLimit(pnum, 0, MAX_RIPCOUNT);

	// Reach, as tics of flight. The row is in units and the puff covers SPEED of them a tic.
	int reach = GetSpellValue(pnum, SPL_CHILLINGBREATH, SPELLVAL_RADIUS);
	SetActorProperty(0, APROP_REACTIONTIME, Max(2, reach / DND_CHILLINGBREATH_SPEED));
	SetResultValue(0);
}

Script "DnD Chilling Breath" (int pnum) {
	int caster = pnum + P_TIDSTART;

	BeginSpellBusy(pnum);
	PlaySound(caster, "ChillingBreath/Start", CHAN_6, 1.0, false);

	int arc = DND_CHILLINGBREATH_ARC;
	if(SpellThresholdMet(pnum, SPL_CHILLINGBREATH, DND_SPELL_THRESH_HIGH))
		arc = arc * (100 + DND_CHILLINGBREATH_R10_WIDE) / 100;

	bool held = false, first = true;
	int t = 0, due = 0;

	while(true) {
		if(!PlayerInGame(pnum) || !IsActorAlive(caster))
			break;

		if(IsChannelHeld())
			held = true;
		else if(held || t >= DND_CHANNEL_GRACE)
			break;

		// Mana keeps its own half second clock; the breath no longer pauses between payments.
		if(!(t % DND_CHILLINGBREATH_TICK)) {
			if(!PayChannelTick(pnum, SPL_CHILLINGBREATH, first))
				break;

			if(first)
				PlaySound(caster, "ChillingBreath/Loop", CHAN_7, 1.0, true);
			first = false;
		}

		// Rapid and uneven rather than metronomic: the gap is rerolled every time and the breath
		// sometimes doubles up, so it reads as breathing instead of as a rank of projectiles.
		if(t >= due) {
			BreatheChillingPuff(pnum, arc);

			if(random(1, 100) <= DND_CHILLINGBREATH_EXTRA)
				BreatheChillingPuff(pnum, arc);

			due = t + random(DND_CHILLINGBREATH_GAPMIN, DND_CHILLINGBREATH_GAPMAX);
		}

		Delay(const:1);
		++t;
	}

	StopSound(caster, CHAN_7);
	EndSpellBusy(pnum);
	StartSpellCooldown(pnum, SPL_CHILLINGBREATH);
}

// ============================ Creeping Frost ============================
// Bursts, then leaves crawlers that creep toward whatever is nearest and pulse as they go.

#define DND_CREEPINGFROST_SPEED 28
// One. Three seeked independently and read as three separate things rather than one spreading
// patch; raising this brings that back unless they are made to share a tracer.
#define DND_CREEPINGFROST_CRAWLERS 1
#define DND_CREEPINGFROST_SPREAD 0.11   // how far apart the crawlers set off
#define DND_CREEPINGFROST_PULSE 7       // tics per crawler pulse; divides TICRATE exactly
#define DND_CREEPINGFROST_PPS 5         // and so this many a second, which DAMAGE2 is split by

// Half-extent the quad actually SHOWS at scale 1: flat.md3 frame 3 is 15.62 and the shader's mask
// trims it to about 0.81 of that. The crawler scales itself off this so the frost matches its blast.
#define DND_CREEPINGFROST_QUADR 12.65
#define DND_CREEPINGFROST_R5_BONUS 4.0  // extra seconds at rank 5
#define DND_CREEPINGFROST_R10_RISE 64.0 // rank 10: how much higher it may climb
#define DND_CREEPINGFROST_SEEK 384      // how far a crawler looks for something to crawl at

Script "DnD Creeping Frost Burst" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1)
		Terminate;

	int tics = GetSpellDurationTics(pnum, SPL_CREEPINGFROST);
	if(SpellThresholdMet(pnum, SPL_CREEPINGFROST, DND_SPELL_THRESH_LOW))
		tics += (DND_CREEPINGFROST_R5_BONUS * TICRATE) >> 16;

	// Counted down one per pulse loop, so the lifetime is in pulses rather than tics.
	int life = Max(1, tics / DND_CREEPINGFROST_PULSE);
	int caster = pnum + P_TIDSTART;
	bool climbs = SpellThresholdMet(pnum, SPL_CREEPINGFROST, DND_SPELL_THRESH_HIGH);

	// The FLOOR under the impact, not the impact itself. A direct hit on an enemy dies at chest
	// height, and crawlers born there spend their first tics dropping -- which read as sinking.
	int x = GetActorX(0), y = GetActorY(0), z = GetActorFloorZ(0);
	int a = GetActorAngle(0);
	int tid = TEMPORARY_SPELL_TID + pnum;

	int k, off;
	int mid = DND_CREEPINGFROST_CRAWLERS / 2;
	for(k = 0; k < DND_CREEPINGFROST_CRAWLERS; ++k) {
		off = k - mid;
		if(!SpawnForced("Spell_CreepingFrost_Crawl", x, y, z, tid,
			((a + DND_CREEPINGFROST_SPREAD * off + 1.0) % 1.0) >> 8))
			continue;

		SetupSpellActor(tid, pnum, SPL_CREEPINGFROST);
		SetActorProperty(tid, APROP_REACTIONTIME, life);
		SetActorProperty(tid, APROP_SPEED, GetSpellValue(pnum, SPL_CREEPINGFROST, SPELLVAL_AMOUNT) * 1.0 / 10);
		if(climbs)
			SetActorProperty(tid, APROP_MAXSTEPHEIGHT, GetActorProperty(tid, APROP_MAXSTEPHEIGHT) + DND_CREEPINGFROST_R10_RISE);

		// Hand spawned, so it has no target -- and "DnD Projectile Checks" reads the owner off one.
		// Without this the crawler never learns who cast it and every pulse prices as nothing.
		SetActivator(tid);
		SetPointer(AAPTR_TARGET, caster);
		SetActivator(caster);

		// Settled onto the floor it actually landed over, which is not the impact's if it fanned out.
		SetActorPosition(tid, GetActorX(tid), GetActorY(tid), GetActorFloorZ(tid), false);

		Thing_ChangeTID(tid, 0);
	}
}

// Sizes the patch to the blast it does, so the two agree under area gear as well as at base.
Script "DnD Creeping Frost Size" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum != -1) {
		int r = ScaleSpellArea(pnum, SPL_CREEPINGFROST,
			GetSpellValue(pnum, SPL_CREEPINGFROST, SPELLVAL_RADIUS) << 16);

		SetActorProperty(0, APROP_SCALEX, FixedDiv(r, DND_CREEPINGFROST_QUADR));
	}

	SetResultValue(0);
}

// ============================ Frost Shards ============================

#define DND_FROSTSHARDS_SPEED 30
#define DND_FROSTSHARDS_VOLLEY 17      // tics a volley cycle, ie. half a second
#define DND_FROSTSHARDS_FORM 10         // of which this many are spent materialising
#define DND_FROSTSHARDS_R5_EXTRA 2     // more shards a volley at rank 5
#define DND_FROSTSHARDS_SPREAD 36.0    // half-width of the arc they form along
#define DND_FROSTSHARDS_RISE 20.0      // and how far the middle of it caps over the ends
#define DND_FROSTSHARDS_CONVERGE 640.0 // where the arc meets when nothing is under the crosshair
#define DND_FROSTSHARDS_NEAR 96.0      // and the closest it will ever be told to meet
#define DND_FROSTSHARDS_ARC 0.0012     // leftover jitter, so a volley is not one solid line
#define DND_FROSTSHARDS_VARC 0.0012
#define DND_FROSTSHARDS_RIPS 3         // rank 10: how many each may pass through

// Distance to whatever the crosshair is actually on, so the volley meets THERE.
int FrostShardsAimDist(int caster) {
	int t = PickActor(caster, GetActorAngle(caster), GetActorPitch(caster),
		DND_SPELL_HITSCANRANGE, 0, MF_SHOOTABLE, ML_BLOCKEVERYTHING, PICKAF_RETURNTID);

	if(!t)
		return DND_FROSTSHARDS_CONVERGE;

	int d = magnitudeTwo((GetActorX(t) - GetActorX(caster)) >> 16,
		(GetActorY(t) - GetActorY(caster)) >> 16) << 16;

	return Max(DND_FROSTSHARDS_NEAR, d);
}

// Where shard k sits in the arc: runs -1 to 1 across it, left to right.
int FrostShardPos(int k, int count) {
	if(count < 2)
		return 0;

	return ((k << 17) / (count - 1)) - 1.0;
}

int FrostShardOffset(int k, int count) {
	return -FixedMul(DND_FROSTSHARDS_SPREAD, FrostShardPos(k, count));
}

// A parabola over the same t, so the middle of the arc caps up over the ends.
int FrostShardRise(int k, int count) {
	int t = FrostShardPos(k, count);
	return FixedMul(DND_FROSTSHARDS_RISE, 1.0 - FixedMul(t, t));
}

// Rank 10 only: the shards rip, once per enemy and only so many.
Script "DnD Frost Shard Setup" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum != -1 && SpellThresholdMet(pnum, SPL_FROSTSHARDS, DND_SPELL_THRESH_HIGH)) {
		// ACS cannot set an actor flag in Zandronum, so the pickup does it.
		GiveInventory("DnD_MakeRipper", 1);
		SetActorProperty(0, APROP_STAMINA, GetActorProperty(0, APROP_STAMINA) | DND_DAMAGEFLAG_RIPSONCE);
		SetSpellRipLimit(pnum, 0, DND_FROSTSHARDS_RIPS);
	}

	SetResultValue(0);
}

// The shards the player watches form. CLIENTSIDE because a server side SpawnForced of a
// CLIENTSIDEONLY actor draws on nobody's screen and reports no error.
Script "DnD Frost Shards Form" (int count) CLIENTSIDE {
	if(!isAlive())
		Terminate;

	int caster = PlayerNumber() + P_TIDSTART;
	int tid = TEMPORARY_DATADUMMY_TID + ConsolePlayerNumber();

	// Read once: the loop swaps the activator about, so it must not depend on one.
	int x = GetActorX(0), y = GetActorY(0), z = GetActorZ(0);

	int k;
	for(k = 0; k < count; ++k) {
		// Dropped on the caster; the actor's own A_Warp puts it out front and keeps it there.
		if(!SpawnForced("Spell_FrostShard_Form", x, y, z, tid, 0))
			continue;

		SetActivator(tid);
		SetPointer(AAPTR_TARGET, caster);
		SetActorProperty(tid, APROP_TARGETTID, caster);
		// Plain units: DECORATE reads an int handed to a fixed parameter as whole units.
		SetUserVariable(tid, "user_side", FrostShardOffset(k, count) >> 16);
		SetUserVariable(tid, "user_rise", FrostShardRise(k, count) >> 16);
		SetActivator(caster);

		Thing_ChangeTID(tid, 0);
	}

	SetResultValue(0);
}

Script "DnD Frost Shards" (int pnum) {
	int caster = pnum + P_TIDSTART;

	int count = Max(1, GetSpellValue(pnum, SPL_FROSTSHARDS, SPELLVAL_AMOUNT));
	if(SpellThresholdMet(pnum, SPL_FROSTSHARDS, DND_SPELL_THRESH_LOW))
		count += DND_FROSTSHARDS_R5_EXTRA;
	count += GetSpellExtraProjectiles(pnum, SPL_FROSTSHARDS);

	int volleys = Max(1, GetSpellDurationTics(pnum, SPL_FROSTSHARDS) / DND_FROSTSHARDS_VOLLEY);

	int v, k, d, rise, eye, aim;
	for(v = 0; v < volleys; ++v) {
		if(!PlayerInGame(pnum) || !IsActorAlive(caster))
			break;

		ACS_NamedExecuteAlways("DnD Frost Shards Form", 0, count);
		Delay(const:DND_FROSTSHARDS_FORM);

		if(!PlayerInGame(pnum) || !IsActorAlive(caster))
			break;

		eye = GetActorViewHeight(caster) - 15.0;
		aim = FrostShardsAimDist(caster);

		for(k = 0; k < count; ++k) {
			d = FrostShardOffset(k, count);
			rise = FrostShardRise(k, count);

			// Turned in by exactly what it takes to reach the aim point, so the arc closes on the
			// crosshair. Left is positive, hence -d; pitch down is positive, so rise is not negated.
			SpawnSpellProjectile(pnum, SPL_FROSTSHARDS, "Spell_FrostShard", DND_FROSTSHARDS_SPEED, 0,
				VectorAngle(aim, -d) + random(-DND_FROSTSHARDS_ARC, DND_FROSTSHARDS_ARC),
				d, 0, 0, false, eye + rise,
				VectorAngle(aim, rise) + random(-DND_FROSTSHARDS_VARC, DND_FROSTSHARDS_VARC));
		}

		Delay(const:DND_FROSTSHARDS_VOLLEY - DND_FROSTSHARDS_FORM);
	}
}

// ============================ Ice Nova ============================
// An expanding set of blasts. Each step hurts only the ANNULUS it just reached, so "once per cast"
// falls out of the geometry and needs no bookkeeping at all.

#define DND_ICENOVA_STEPS 7            // expansions in one cast
#define DND_ICENOVA_STEPTICS 2         // tics between them, and so the whole span
#define DND_ICENOVA_R5_FASTER 20       // percent quicker at rank 5
#define DND_ICENOVA_R10_WEAKEN 15      // percent less damage dealt, rank 10
#define DND_ICENOVA_R10_TICS (3 * TICRATE)

// Half-extent the quad shows at scale 1: flat2.md3 is 15.62 and IceNova.fp fills it to the spike
// tips, so the ring radius IS this times the scale. That is what keeps it on the blast.
#define DND_ICENOVA_QUADR 15.62
#define DND_ICENOVA_START 24           // the ring is born this wide, so it does not pop from nothing

// Ease, 16.16 in and out: 3u^2 - 2u^3. Rest, accelerate, then settle at the far edge -- a ring
// crawling out at a constant rate is what read as mechanical.
int NovaEase(int u) {
	return FixedMul(FixedMul(u, u), 3.0 - 2 * u);
}

Script "DnD Ice Nova" (int pnum) {
	int caster = pnum + P_TIDSTART;

	// Reach off the row, through the area stat. The last step lands exactly here.
	int full = ScaleSpellArea(pnum, SPL_ICENOVA,
		GetSpellValue(pnum, SPL_ICENOVA, SPELLVAL_RADIUS) << 16) >> 16;
	if(full < 1)
		Terminate;

	PlaySound(caster, "IceNova/Cast", CHAN_7);

	// Rank 5 is the same steps over a shorter span; a whole tic off each step was 2 to 1, ie. twice as fast.
	int span = DND_ICENOVA_STEPS * DND_ICENOVA_STEPTICS;
	if(SpellThresholdMet(pnum, SPL_ICENOVA, DND_SPELL_THRESH_LOW))
		span = Max(DND_ICENOVA_STEPS, (span * 100 + (100 + DND_ICENOVA_R5_FASTER) / 2) / (100 + DND_ICENOVA_R5_FASTER));

	bool weaken = SpellThresholdMet(pnum, SPL_ICENOVA, DND_SPELL_THRESH_HIGH);

	int ox = GetActorX(caster), oy = GetActorY(caster);
	int bx = ox >> 16, by = oy >> 16;

	// The drawn ring, on a tid of its own for the whole cast because its size is rewritten every
	// tic. It does not have to land on the damage steps -- it has to look smooth.
	int fx = DND_ICENOVA_FX_TID + pnum;
	bool drawn = !!SpawnForced("Spell_IceNova", ox, oy, GetActorFloorZ(caster) + DND_ICENOVA_Z, fx, 0);
	if(drawn) {
		SetupSpellActor(fx, pnum, SPL_ICENOVA);
		SetActorProperty(fx, APROP_REACTIONTIME, span);
	}

	int dmg = PriceSpellDamage(pnum, SPL_ICENOVA, GetSpellValue(pnum, SPL_ICENOVA, SPELLVAL_DAMAGE), DND_WDMG_ISRADIUSDMG);

	// Never wider than the blast it is drawing, however small that is.
	int start = Min(DND_ICENOVA_START, full);

	int t, step = 0, prev = 0, r, ring, i, mn, dx, dy, d2, out;
	for(t = 1; t <= span; ++t) {
		// SCALEX only. For a model the HORIZONTAL pair both come off scaleX and scaleY is the
		// VERTICAL one -- writing it is what made the ring climb as it grew.
		if(drawn && IsActorAlive(fx)) {
			// Parenthesised: >> binds looser than +, so without these the start radius gets shifted.
			ring = start +
				(FixedMul(NovaEase((t << 16) / span), (full - start) << 16) >> 16);
			SetActorProperty(fx, APROP_SCALEX, FixedDiv(ring << 16, DND_ICENOVA_QUADR));
		}

		// The damage keeps its own steps, due evenly across the span; only the drawing is eased.
		if(step >= t * DND_ICENOVA_STEPS / span) {
			Delay(const:1);
			continue;
		}

		++step;
		r = full * step / DND_ICENOVA_STEPS;

		for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
			i = UsedMonsterTIDs[mn];

			// Box reject first: UsedMonsterTIDs counts every monster the map ever spawned.
			dx = (GetActorX(i) >> 16) - bx;
			if(dx > r || dx < -r)
				continue;
			dy = (GetActorY(i) >> 16) - by;
			if(dy > r || dy < -r)
				continue;

			// The ANNULUS, not the disc. Anything nearer was caught by an earlier step.
			d2 = dx * dx + dy * dy;
			if(d2 > r * r || d2 <= prev * prev)
				continue;
			if(!IsActorAlive(i) || !IsSpellEnemy(i))
				continue;

			out = HandleDamageDeal(caster, i, dmg, GetSpellDamageType(SPL_ICENOVA), SPL_ICENOVA,
				DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISRADIUSDMG, 0, 0, 0, 0, true);
			if(out > 0)
				Thing_Damage2(i, out, "SkipHandle");

			// "Enemies hit near the furthest section." Only the last step reaches out there, and a
			// monster is in exactly one annulus, so this needs no distance test of its own.
			if(weaken && step == DND_ICENOVA_STEPS)
				WeakenMonster(i, DND_ICENOVA_R10_WEAKEN, ScaleSpellEffectTics(pnum, SPL_ICENOVA, DND_ICENOVA_R10_TICS));
		}

		prev = r;
		Delay(const:1);
	}

	if(drawn && IsActorAlive(fx))
		Thing_ChangeTID(fx, 0);
}

// ============================ Gust of Frost ============================
// The hit front steps out with the drawn wave, so each enemy is struck as the wind reaches it.

#define DND_GUSTOFFROST_HALFARC 0.14    // half the cone, ie. about 50 degrees a side
#define DND_GUSTOFFROST_PUSH 26         // shove at the caster, tapering to nothing at the edge
#define DND_GUSTOFFROST_MINPUSH 6
#define DND_GUSTOFFROST_SLOWTICS (3 * TICRATE)
#define DND_GUSTOFFROST_R5_SLOW 60
#define DND_PUSH_BOSS_PCT 25            // the share of a spell's shove a boss or legendary takes
#define DND_PUSH_HEAVY_PCT 50           // and a Baron-class or heavier monster, or a unique

enum {
	DND_PUSHTIER_NORMAL,
	DND_PUSHTIER_HEAVY,             // the Baron class up to the Cyberdemon's, and uniques
	DND_PUSHTIER_BOSS,              // bosses and legendaries
	DND_PUSHTIER_DUNGEONBOSS        // whatever its kind, spawned as a dungeon's boss
};

int GetMonsterPushTier(int victim) {
	int m_id = victim - DND_MONSTERTID_BEGIN;
	if(m_id < 0 || m_id >= DND_MAX_MONSTERS)
		return DND_PUSHTIER_NORMAL;
	if(MonsterProperties[m_id].flags & DND_MONFLAG_ISDUNGEONBOSS)
		return DND_PUSHTIER_DUNGEONBOSS;

	int id = MonsterProperties[m_id].id;
	if(IsMonsterIdBoss(id) || isLegendaryMonster(id))
		return DND_PUSHTIER_BOSS;

	int c = MonsterProperties[m_id].class;
	if((c >= MONSTERCLASS_BARON && c <= MONSTERCLASS_CYBERDEMON) || IsUniqueMonster(id))
		return DND_PUSHTIER_HEAVY;
	return DND_PUSHTIER_NORMAL;
}

// How much of a spell's shove a monster takes: bosses and the Baron tier up resist being thrown.
int GetMonsterPushPercent(int victim) {
	switch(GetMonsterPushTier(victim)) {
		case DND_PUSHTIER_HEAVY:
		return DND_PUSH_HEAVY_PCT;
		case DND_PUSHTIER_BOSS:
		case DND_PUSHTIER_DUNGEONBOSS:
		return DND_PUSH_BOSS_PCT;
	}
	return 100;
}

#define DND_GUSTOFFROST_MAXHIT 128      // enemies one gust remembers, for once-only and the slam
#define DND_GUSTOFFROST_FRAMES 28       // tics the front takes to cross the reach; the pieces keep its pace
#define DND_GUSTOFFROST_LOOKBACK 32     // band re-checked behind the front, for enemies charging in
#define DND_GUSTOFFROST_WINDLEN 72      // GUSTWIND.md3 at scale 1, so scale = reach / this
#define DND_GUSTOFFROST_WINDZ 26.0
#define DND_GUSTOFFROST_SLAMHPPCT 10    // rank 10: percent of max health added, never more than the mass part
#define DND_GUSTOFFROST_MASSKNEE 900    // mass that takes half the ceiling
#define DND_GUSTOFFROST_MASSCLAMP 100000  // clamped first, or ceiling * mass overflows a signed int
#define DND_GUSTOFFROST_SLAMSPEED 8     // units a tic a push must still carry for a sudden stop to count
#define DND_GUSTOFFROST_SLAMTRACK 35    // tics pushed enemies are still watched after the wave ends

// Horizontal speed squared, in whole units a tic.
int GustSpeed2(int tid) {
	int vx = GetActorVelX(tid) >> 16, vy = GetActorVelY(tid) >> 16;
	return vx * vx + vy * vy;
}

// Retries the move a stopped monster was making. One that stopped ITSELF -- an attack, a freeze --
// can still make it and is put straight back; only a real wall or body refuses it.
bool GustBlocked(int tid, int vx, int vy) {
	int x = GetActorX(tid), y = GetActorY(tid), z = GetActorZ(tid);
	if(!SetActorPosition(tid, x + vx, y + vy, z, false))
		return true;
	SetActorPosition(tid, x, y, z, false);
	return false;
}

// The slam: the ceiling times a saturating mass share, so heavy enemies take nearly all of it.
// Rank 10 adds a share of max health, divided first so a boss's health cannot overflow it.
void GustSlam(int pnum, int caster, int victim, int ceiling, bool health) {
	int m = Clamp_Between(GetActorProperty(victim, APROP_MASS), 0, DND_GUSTOFFROST_MASSCLAMP);
	int dmg = ceiling * m / (m + DND_GUSTOFFROST_MASSKNEE);

	int m_id = victim - DND_MONSTERTID_BEGIN;
	if(health && m_id >= 0 && m_id < DND_MAX_MONSTERS)
		dmg += Min(MonsterProperties[m_id].maxhp / 100 * DND_GUSTOFFROST_SLAMHPPCT, dmg);

	// All of it is base damage, so the spell pricing scales the health share too.
	dmg = PriceSpellDamage(pnum, SPL_GUSTOFFROST, dmg);
	if(dmg <= 0)
		return;

	int out = HandleDamageDeal(caster, victim, dmg, GetSpellDamageType(SPL_GUSTOFFROST), SPL_GUSTOFFROST,
		DND_DAMAGEFLAG_ISSPELL, 0, 0, 0, 0, true);
	if(out > 0)
		Thing_Damage2(victim, out, "SkipHandle");
}

Script "DnD Gust Of Frost" (int pnum) {
	int caster = pnum + P_TIDSTART;
	int face = GetActorAngle(caster);

	int cut = ScaleSpellArea(pnum, SPL_GUSTOFFROST,
		GetSpellValue(pnum, SPL_GUSTOFFROST, SPELLVAL_RADIUS) << 16) >> 16;
	if(cut < 1)
		Terminate;

	int pct = SpellThresholdMet(pnum, SPL_GUSTOFFROST, DND_SPELL_THRESH_LOW) ?
		DND_GUSTOFFROST_R5_SLOW : GetSpellValue(pnum, SPL_GUSTOFFROST, SPELLVAL_DAMAGE2);

	// The planted origin, on a tid held for the whole wave so walls can be checked from it. Scaled
	// here, because a spawn frame's action never runs; the pieces it scatters copy the scale.
	int fx = DND_GUSTOFFROST_FX_TID + pnum;
	bool drawn = !!SpawnForced("Spell_GustOfFrost_Wind", GetActorX(caster), GetActorY(caster),
		GetActorZ(caster) + DND_GUSTOFFROST_WINDZ, fx, GetActorAngle(caster) >> 8);
	if(drawn) {
		SetupSpellActor(fx, pnum, SPL_GUSTOFFROST);
		SetActorProperty(fx, APROP_SCALEX, FixedDiv(cut << 16, DND_GUSTOFFROST_WINDLEN << 16));
		SetActorProperty(fx, APROP_SCALEY, FixedDiv(cut << 16, DND_GUSTOFFROST_WINDLEN << 16));
	}
	int eye = drawn ? fx : caster;

	// Percent, 10 more a rank.
	int push = GetSpellValue(pnum, SPL_GUSTOFFROST, SPELLVAL_AMOUNT);

	PlaySound(caster, "GustOfFrost/Cast", CHAN_7);

	int ox = GetActorX(caster) >> 16, oy = GetActorY(caster) >> 16;

	// The slam's ceiling, before mass. Rank 10 adds the health share on top.
	int slam = GetSpellValue(pnum, SPL_GUSTOFFROST, SPELLVAL_DAMAGE);
	bool health = SpellThresholdMet(pnum, SPL_GUSTOFFROST, DND_SPELL_THRESH_HIGH);

	// Last speed squared (0 once no longer watched) and velocity, per blown enemy.
	int blown[DND_GUSTOFFROST_MAXHIT], v2[DND_GUSTOFFROST_MAXHIT];
	int vx[DND_GUSTOFFROST_MAXHIT], vy[DND_GUSTOFFROST_MAXHIT];
	int n = 0, moving = 0;

	int t, r, lo, prev = 0, i, k, mn, dx, dy, d2, to, off, s2;
	for(t = 1; t <= DND_GUSTOFFROST_FRAMES ||
		(moving && t <= DND_GUSTOFFROST_FRAMES + DND_GUSTOFFROST_SLAMTRACK); ++t) {
		// A blocked monster's velocity is zeroed outright, while friction takes a tenth a tic. A drop of
		// over half is then confirmed by GustBlocked, which rules out a monster that stopped itself.
		for(k = 0; k < n && moving; ++k) {
			if(!v2[k])
				continue;

			s2 = GustSpeed2(blown[k]);
			if(s2 * 4 < v2[k]) {
				if(IsActorAlive(blown[k]) && GustBlocked(blown[k], vx[k], vy[k]))
					GustSlam(pnum, caster, blown[k], slam, health);
				v2[k] = 0;
				--moving;
			}
			else if(s2 < DND_GUSTOFFROST_SLAMSPEED * DND_GUSTOFFROST_SLAMSPEED) {
				v2[k] = 0;
				--moving;
			}
			else {
				v2[k] = s2;
				vx[k] = GetActorVelX(blown[k]);
				vy[k] = GetActorVelY(blown[k]);
			}
		}

		if(t > DND_GUSTOFFROST_FRAMES) {
			Delay(const:1);
			continue;
		}

		// Matches the pieces, which fly reach / FRAMES a tic.
		r = cut * t / DND_GUSTOFFROST_FRAMES;
		lo = prev - DND_GUSTOFFROST_LOOKBACK;

		for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
			i = UsedMonsterTIDs[mn];

			// Box reject first: UsedMonsterTIDs counts every monster the map ever spawned.
			dx = (GetActorX(i) >> 16) - ox;
			if(dx > r || dx < -r)
				continue;
			dy = (GetActorY(i) >> 16) - oy;
			if(dy > r || dy < -r)
				continue;

			// Only the band the front just crossed, plus a little behind it.
			d2 = dx * dx + dy * dy;
			if(d2 > r * r || (lo > 0 && d2 <= lo * lo))
				continue;
			if(!IsActorAlive(i) || !IsSpellEnemy(i))
				continue;

			// A cone off the facing at cast, not a ring -- this blows forward.
			to = VectorAngle(dx << 16, dy << 16);
			off = abs(to - face);
			if(off > 0.5)
				off = 1.0 - off;
			if(off > DND_GUSTOFFROST_HALFARC)
				continue;

			// The lookback band overlaps the last step, so remember who was already blown.
			for(k = 0; k < n; ++k)
				if(blown[k] == i)
					break;
			if(k < n)
				continue;

			// Last, as the dearest test: the wind does not blow through walls.
			if(!CheckSight(eye, i, 0))
				continue;

			SlowMonster(i, pct, ScaleSpellEffectTics(pnum, SPL_GUSTOFFROST, DND_GUSTOFFROST_SLOWTICS));

			// Along the line from the CASTER, so something off to one side is blown outward.
			ThrustThing(to >> 8, push *
				Max(DND_GUSTOFFROST_MINPUSH, DND_GUSTOFFROST_PUSH * (cut - sqrt(d2)) / cut) / 100 * GetMonsterPushPercent(i) / 100, 1, i);

			if(n < DND_GUSTOFFROST_MAXHIT) {
				// Read straight after the shove, so a monster already against a wall still registers.
				s2 = GustSpeed2(i);
				if(s2 < DND_GUSTOFFROST_SLAMSPEED * DND_GUSTOFFROST_SLAMSPEED)
					s2 = 0;
				else
					++moving;

				blown[n] = i;
				v2[n] = s2;
				vx[n] = GetActorVelX(i);
				vy[n++] = GetActorVelY(i);
			}
		}

		prev = r;
		Delay(const:1);
	}

	if(drawn && IsActorAlive(fx))
		Thing_ChangeTID(fx, 0);
}

// ============================ Frost Bomb ============================
// Planted where you aimed. DECORATE's frame run paces the pulses; PULSE must match it.

#define DND_FROSTBOMB_PULSE 17          // tics a pulse, ie. half a second
#define DND_FROSTBOMB_RISE 24.0         // off the floor, so the blast is not half buried
#define DND_FROSTBOMB_R10_RADIUS 256    // rank 10, and absolute -- it replaces the row

Script "DnD Frost Bomb Setup" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum != -1)
		SetActorProperty(0, APROP_REACTIONTIME,
			Max(1, GetSpellDurationTics(pnum, SPL_FROSTBOMB) / DND_FROSTBOMB_PULSE));

	SetResultValue(0);
}

Script "DnD Frost Bomb" (int pnum) {
	int caster = pnum + P_TIDSTART;
	int cx, cy, cz;

	// Same two ways in as Rain of Fire and Volcano: what the player is looking at, else where the
	// aim puff landed.
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
	if(!SpawnForced("Spell_FrostBomb", cx, cy, cz, tid, 0))
		Terminate;

	// Lifted off the floor it was aimed at, so the blast is centred on the crystal rather than
	// half buried in the ground.
	SetActorPosition(tid, cx, cy, GetActorFloorZ(tid) + DND_FROSTBOMB_RISE, false);
	SetupSpellActor(tid, pnum, SPL_FROSTBOMB);
	Thing_ChangeTID(tid, 0);

	PlaySound(caster, "FrostBomb/Cast", CHAN_BODY);
}

// ============================ Summon: Ice Golem ============================
// Built on the Fire Demon, which is the mod's one worked example of a spell summon.

#define DND_ICEGOLEM_DIST 72
#define DND_ICEGOLEM_DASHMULT 3         // 50 melee and 150 dash are exactly this
#define DND_ICEGOLEM_BOLTCHANCE 30
#define DND_ICEGOLEM_R5_MANAREGEN 50    // percent, rank 5
#define DND_ICEGOLEM_AURATICS 35        // how often the rank 5 buff is pushed out again
#define DND_ICEGOLEM_AURAPOLL 5         // and how often the golem is looked for, so a death ends it promptly

// Melee off the row, with the dash a straight multiple of it -- so the dash has no field to drift
// from and both move together when the row does.
Script "DnD Ice Golem Melee" (int mult) {
	int pnum = GetActorProperty(0, APROP_MASTERTID) - P_TIDSTART;
	if(pnum < 0 || pnum >= MAXPLAYERS) {
		SetResultValue(0);
		Terminate;
	}

	SetResultValue(ACS_NamedExecuteWithResult("DND Pet Monster Damage Scale",
		GetSpellValue(pnum, SPL_ICEGOLEM, SPELLVAL_DAMAGE2) * Max(1, mult), 0, 0));
}

// Rank 10: the golem throws its OWNER's Ice Bolt at the rank THEY have it, so the reward is worth
// what they put into the bolt and nothing if they never took it. Rolls and casts in one call.
Script "DnD Ice Golem Bolt" (void) {
	int golem = ActivatorTID();
	int pnum = GetActorProperty(0, APROP_MASTERTID) - P_TIDSTART;

	if(pnum < 0 || pnum >= MAXPLAYERS ||
		!SpellThresholdMet(pnum, SPL_ICEGOLEM, DND_SPELL_THRESH_HIGH) ||
		!GetSpellRank(pnum, SPL_ICEBOLT, true) ||
		random(1, 100) > DND_ICEGOLEM_BOLTCHANCE) {
		SetResultValue(0);
		Terminate;
	}

	// Thrown FROM the golem, owned BY the caster, exactly as the demon's jet is.
	SpawnSpellProjectile(pnum, SPL_ICEBOLT, "Spell_IceBolt", DND_ICEBOLT_SPEED, 0, 0, 0, 0, golem);
	SetResultValue(1);
}

Script "DnD Summon Ice Golem" (int pnum) {
	int caster = pnum + P_TIDSTART;

	int maxkind = Max(1, GetSpellValue(pnum, SPL_ICEGOLEM, SPELLVAL_AMOUNT));

	// The oldest GOLEM makes way first, and only once its own kind is full, so a golem never costs
	// an unrelated pet its place.
	if(CountPlayerPetsOfKind(pnum, MONSTER_PET_ICEGOLEM) >= maxkind)
		UnsummonPet(FindOldestPlayerPetOfKind(pnum, MONSTER_PET_ICEGOLEM));

	if(!CanActorHaveMorePets(caster))
		UnsummonPet(FindOldestPlayerPet(pnum));

	int tid = TEMPORARY_PET_TID + pnum;
	if(!SpawnSummonNear("Spell_IceGolem", caster, tid, DND_ICEGOLEM_DIST))
		Terminate;

	SetActorPosition(tid, GetActorX(tid), GetActorY(tid), GetActorFloorZ(tid), false);

	// Before its first tic: "DnD Pet Monster Scale" reads APROP_HEALTH as the base it levels up
	// from, so this is what makes the row's number the one that gets scaled.
	SetActorProperty(tid, APROP_HEALTH, Max(1, ApplySpellGemMore(pnum, SPL_ICEGOLEM, GEMSTAT_MORE_MINIONLIFE, GetSpellValue(pnum, SPL_ICEGOLEM, SPELLVAL_DAMAGE))));

	SetActorProperty(tid, APROP_MASTERTID, caster);
	SetActivator(tid);
	SetPointer(AAPTR_MASTER, caster);
	SetActorProperty(0, APROP_FRIENDLY, true);
	Thing_ChangeTID(tid, 0);

	GiveActorInventory(caster, "PetCounter", 1);

	SetActivator(caster);
	ACS_NamedExecuteAlways("DnD On Pet Summon", 0);

	if(SpellThresholdMet(pnum, SPL_ICEGOLEM, DND_SPELL_THRESH_HIGH) ||
		SpellThresholdMet(pnum, SPL_ICEGOLEM, DND_SPELL_THRESH_LOW))
		ACS_NamedExecuteAlways("DnD Ice Golem Aura", 0, pnum);
}

// Whether a player's aura ticker runs. Map scope, so it resets with the scripts on a new map.
int[] module& GetIceGolemAuras() {
	static int on[MAXPLAYERS];
	return on;
}

// Rank 5's mana regeneration, pushed out again while a golem stands and removed as soon as none does.
Script "DnD Ice Golem Aura" (int pnum) {
	if(!SpellThresholdMet(pnum, SPL_ICEGOLEM, DND_SPELL_THRESH_LOW))
		Terminate;

	int[] module& on = GetIceGolemAuras();
	if(on[pnum])
		Terminate;
	on[pnum] = true;

	int caster = pnum + P_TIDSTART;
	for(int t = 0; PlayerInGame(pnum) && CountPlayerPetsOfKind(pnum, MONSTER_PET_ICEGOLEM) > 0; t += DND_ICEGOLEM_AURAPOLL) {
		// Granted for twice the refresh so a late tick leaves no gap; a dead summoner gets it back on respawn.
		if(!(t % DND_ICEGOLEM_AURATICS) && IsActorAlive(caster))
			HandlePlayerBuffAssignment(pnum, caster, BTI_SPELL_ICEGOLEM_MANA, 0, 0, DND_ICEGOLEM_AURATICS * 2, DND_ICEGOLEM_R5_MANAREGEN);
		Delay(const:DND_ICEGOLEM_AURAPOLL);
	}

	if(PlayerInGame(pnum))
		RemoveBuffWithTableIndex(pnum, BTI_SPELL_ICEGOLEM_MANA);
	on[pnum] = false;
}

// ============================ Ice Spear ============================
// Two phases in ONE actor, so the owner and rip bookkeeping survive. Its added crit is still owed.

#define DND_ICESPEAR_PHASE1 14          // tics in the slow form; DECORATE's 2 hidden + 6 x 2
#define DND_ICESPEAR_PHASE2 70          // and tics it lives after
#define DND_ICESPEAR_FASTMULT 4         // how much quicker the evolved form travels
#define DND_ICESPEAR_FASTMAX 64.0       // but never faster than this, in units a tic
#define DND_ICESPEAR_RIPS 10            // flat, hence a define and not a field
#define DND_ICESPEAR_CRIT 0.10          // added crit chance once primed; rank 5 adds R5_CRIT
#define DND_ICESPEAR_R5_CRIT 0.02
#define DND_ICESPEAR_CRITMULT 50        // added crit multiplier, integer percent; rank 10 adds R10
#define DND_ICESPEAR_R10_CRITMULT 50
#define DND_ICESPEAR_FORMTICS 14        // ISPF A-N in DECORATE; the spear launches as they end
#define DND_ICESPEAR_HANDOFF 2          // the formed spear is held while the real one clears the hand
#define DND_ICESPEAR_SIDE 16.0          // to the caster's right; Spell_IceSpear_Form's A_Warp must match

// The spear the player watches form. CLIENTSIDE for the same reason as the Frost Shards one, and it
// keeps running to hand the model the caster's pitch each tic, which A_Warp cannot copy.
Script "DnD Ice Spear Form" (void) CLIENTSIDE {
	if(!isAlive())
		Terminate;

	int pnum = PlayerNumber();
	int caster = pnum + P_TIDSTART;
	int tid = DND_ICESPEAR_FORM_TID + pnum;

	// Dropped on the caster; the actor's own A_Warp puts it at their right and keeps it there.
	if(!SpawnForced("Spell_IceSpear_Form", GetActorX(0), GetActorY(0), GetActorZ(0), tid, 0))
		Terminate;

	SetActivator(tid);
	SetPointer(AAPTR_TARGET, caster);
	SetActivator(caster);

	// Negated: an inherited model pitch tips the point UP for a positive value, a view tips DOWN.
	for(int t = 0; t < DND_ICESPEAR_FORMTICS + DND_ICESPEAR_HANDOFF; ++t) {
		SetActorPitch(tid, -GetActorPitch(caster));
		Delay(const:1);
	}

	Thing_ChangeTID(tid, 0);
}

// Forms at the caster's right, then flies from there to whatever the crosshair is on.
Script "DnD Ice Spear" (int pnum) {
	int caster = pnum + P_TIDSTART;

	ACS_NamedExecuteAlways("DnD Ice Spear Form", 0);
	Delay(const:DND_ICESPEAR_FORMTICS);

	if(!PlayerInGame(pnum) || !IsActorAlive(caster))
		Terminate;

	// Launched AT the field's speed, so the synergy moves it; the evolved speed is a multiple of it.
	// Turned in by what it takes to meet the crosshair from the right; a positive angle turns left.
	int conv = VectorAngle(FrostShardsAimDist(caster), DND_ICESPEAR_SIDE);
	for(int i = 0; i < 1 + GetSpellExtraProjectiles(pnum, SPL_ICESPEAR); ++i)
		SpawnSpellProjectile(pnum, SPL_ICESPEAR, "Spell_IceSpear",
			Max(1, GetSpellValue(pnum, SPL_ICESPEAR, SPELLVAL_AMOUNT)), 0,
			conv + FanOffset(i, DND_SPELL_FANSPREAD), -DND_ICESPEAR_SIDE);
}

// The evolution. Velocity is rewritten rather than only APROP_SPEED: speed is what a projectile is
// LAUNCHED at, and this one is already in the air.
Script "DnD Ice Spear Evolve" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	// FASTMULT times quicker, capped at FASTMAX: the whole vector is rescaled, so the aim holds.
	int vx = GetActorVelX(0), vy = GetActorVelY(0), vz = GetActorVelZ(0);
	int speed = VectorLength(VectorLength(vx, vy), vz);
	if(speed > 0) {
		int scale = FixedDiv(Min(speed * DND_ICESPEAR_FASTMULT, DND_ICESPEAR_FASTMAX), speed);
		SetActorVelocity(0, FixedMul(vx, scale), FixedMul(vy, scale), FixedMul(vz, scale), false, false);
	}

	// Each enemy once, and only so many of them, which is the same bookkeeping Glacial Spike uses.
	GiveInventory("DnD_MakeRipper", 1);
	SetActorProperty(0, APROP_STAMINA, GetActorProperty(0, APROP_STAMINA) | DND_DAMAGEFLAG_RIPSONCE);
	SetSpellRipLimit(pnum, 0, DND_ICESPEAR_RIPS);

	// Its own crit, which the damage event reads off the actor. Phase 1 has none.
	SetUserVariable(0, "user_critchance", DND_ICESPEAR_CRIT +
		SpellThresholdMet(pnum, SPL_ICESPEAR, DND_SPELL_THRESH_LOW) * DND_ICESPEAR_R5_CRIT);
	SetUserVariable(0, "user_critmult", DND_ICESPEAR_CRITMULT +
		SpellThresholdMet(pnum, SPL_ICESPEAR, DND_SPELL_THRESH_HIGH) * DND_ICESPEAR_R10_CRITMULT);

	SetActorProperty(0, APROP_REACTIONTIME, DND_ICESPEAR_PHASE2);
	SetResultValue(0);
}

// ============================ Blizzard ============================
// Rain of Fire's loop with icicles: the same aim and slant, AMOUNT the storm and RADIUS one icicle's blast.

#define DND_BLIZZARD_SPEED 28.0         // sets the volley's fall time; each icicle's own speed fits it
#define DND_BLIZZARD_INTERVAL 23        // tics between volleys at the base rate (0.66 s); duration sets the count
#define DND_BLIZZARD_PERVOLLEY 16       // icicles in one volley
#define DND_BLIZZARD_R5_RADIUS 128      // added to the storm at rank 5
#define DND_BLIZZARD_R10_MORE 30        // rank 10: percent increased volley rate, and percent more ice in each
#define DND_BLIZZARD_SLANT 72.0         // windblown, but steeper than a comet
#define DND_BLIZZARD_MARKRATE (TICRATE / 2)
#define DND_BLIZZARD_CLEAR 16.0         // an icicle's height and a little: how far under a ceiling it starts
#define DND_BLIZZARD_SOUNDLIFT 32.0     // how far over the storm's floor its loop sits

Script "DnD Blizzard" (int pnum) {
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

	int storm = GetSpellValue(pnum, SPL_BLIZZARD, SPELLVAL_AMOUNT);
	if(SpellThresholdMet(pnum, SPL_BLIZZARD, DND_SPELL_THRESH_LOW))
		storm += DND_BLIZZARD_R5_RADIUS;
	int scatter = ScaleSpellArea(pnum, SPL_BLIZZARD, storm << 16);
	int dur = GetSpellDurationTics(pnum, SPL_BLIZZARD);
	bool r10 = SpellThresholdMet(pnum, SPL_BLIZZARD, DND_SPELL_THRESH_HIGH);

	// Floor and ceiling read once from the middle, as Rain of Fire does.
	int probe = TEMPORARY_DATADUMMY_TID + pnum;
	int ground = cz, roof = cz + DND_RAINOFFIRE_RISE;
	if(SpawnForced("DnD_SpellAnchor", cx, cy, cz, probe, 0)) {
		ground = GetActorFloorZ(probe);
		roof = Min(GetActorCeilingZ(probe) - DND_RAINOFFIRE_HEADROOM, ground + DND_RAINOFFIRE_RISE);
		Thing_Remove(probe);
	}
	if(roof <= ground + DND_RAINOFFIRE_HEADROOM)
		roof = ground + DND_RAINOFFIRE_HEADROOM;

	// DAMAGE2 is the volley RATE in tenths of a percent (1000 at base). Rank 10 and Ice Bolt's synergy
	// are both "increased", so they add to it; the duration then decides how many volleys fall.
	int rate = GetSpellValue(pnum, SPL_BLIZZARD, SPELLVAL_DAMAGE2), per = DND_BLIZZARD_PERVOLLEY;
	if(r10) {
		rate += DND_BLIZZARD_R10_MORE * 10;
		per = (per * (100 + DND_BLIZZARD_R10_MORE) + 50) / 100;
	}
	per += GetSpellExtraProjectiles(pnum, SPL_BLIZZARD);
	rate = Max(1, rate);
	int need = DND_BLIZZARD_INTERVAL * 1000;
	int volleys = Max(1, dur * rate / need);

	// Paced by an accumulator rather than a whole number of tics, so every tenth of a percent counts.
	// Primed full, so the first volley falls at once.
	int acc = need;

	// One fall time for the whole volley: each icicle covers its own path in it, so they all land together.
	int fall = Max(1, (roof - ground) / DND_BLIZZARD_SPEED);

	// Its sound at the storm, low over its floor, for as long as it rains and the last volley falls.
	StartSpellLoopSound(pnum, DND_LOOPSOUND_BLIZZARD, cx, cy, ground + DND_BLIZZARD_SOUNDLIFT, dur + fall, false);

	int tid = TEMPORARY_SPELL_TID + pnum;
	int t, v = 0, c, a, d, lx, ly, sx, sy, sz, dx, dy;

	for(t = 0; v < volleys; ++t) {
		if(!PlayerInGame(pnum))
			break;

		if(!(t % DND_BLIZZARD_MARKRATE))
			ACS_NamedExecuteWithResult("DnD Rain Of Fire Mark", cx, cy,
				((scatter >> 16) << 16) | ((ground >> 16) & 0xFFFF), true);

		if(acc >= need) {
			acc -= need;
			++v;

			for(c = 0; c < per; ++c) {
				// A little toward the middle: u^0.75 sits between uniform over the disc (u^0.5) and u.
				a = random(0, 1.0);
				d = random(0, 1.0);
				d = FixedMul(scatter, FixedMul(fsqrt(d), fsqrt(fsqrt(d))));
				lx = cx + FixedMul(cos(a), d);
				ly = cy + FixedMul(sin(a), d);

				a = random(0, 1.0);
				d = random(0, DND_BLIZZARD_SLANT);
				sx = lx + FixedMul(cos(a), d);
				sy = ly + FixedMul(sin(a), d);

				dx = lx - sx;
				dy = ly - sy;

				// Faced along its fall, so the icicle model points the way it travels.
				if(!SpawnForced("Spell_BlizzardIce", sx, sy, roof, tid, VectorAngle(dx, dy) >> 8))
					continue;

				// The roof was read at the middle: start under the ceiling where this one actually is, or not at all.
				sz = GetActorCeilingZ(tid) - DND_BLIZZARD_CLEAR;
				if(sz < GetActorFloorZ(tid) + DND_BLIZZARD_CLEAR || !SetActorPosition(tid, sx, sy, Min(sz, roof), 0)) {
					Thing_Remove(tid);
					continue;
				}
				SetupSpellActor(tid, pnum, SPL_BLIZZARD);

				// The volley's one fall time from wherever it starts, so it still lands with the rest.
				SetActorVelocity(tid, dx / fall, dy / fall, (ground - GetActorZ(tid)) / fall, false, false);

				Thing_ChangeTID(tid, 0);
			}
		}

		acc += rate;
		Delay(const:1);
	}
}

// ============================ Avalanche ============================
// After a brief delay, ice is hurled from behind and above the caster down onto the ground ahead.

#define DND_AVALANCHE_DELAY 18          // the brief delay before the throw, in tics
#define DND_AVALANCHE_SOUNDTAIL 20      // tics the last ice is still forming and falling after the throw
#define DND_AVALANCHE_SPEED 30.0
#define DND_AVALANCHE_BEHIND 96.0       // how far behind the caster the ice starts, give or take half
#define DND_AVALANCHE_SPREAD 160.0      // half-width across the caster's back it starts from
#define DND_AVALANCHE_RISE 112.0        // and how high above their feet
#define DND_AVALANCHE_RISELOW 48.0      // the lowest start, about eye height, so some fly in near level
#define DND_AVALANCHE_NEAR 96.0         // the band ahead it lands in
#define DND_AVALANCHE_FAR 640.0
#define DND_AVALANCHE_FANARC 0.07       // landing spread off the facing, a share of a turn (~25 degrees)
#define DND_AVALANCHE_R10_RIP 25        // percent of the ice that rips, unbounded, at rank 10
#define DND_AVALANCHE_PITCHMAX 0.125    // the committed aim is held within 45 degrees of level
#define DND_AVALANCHE_SLOWEVERY 8       // tics between sweeps of the landing ground for its slow
#define DND_AVALANCHE_SLOWTICS 12       // each sweep's slow outlasts the gap to the next
#define DND_AVALANCHE_SLOWTAIL 3        // sweeps after the last throw, while the last ice comes down
#define DND_AVALANCHE_SLOWMARGIN 32.0   // a monster's radius, give or take, past the fan's edges
#define DND_AVALANCHE_SLOWZBAND 96.0    // how far above or below the aim's line an enemy is still on it

// Where each caster stood and aimed as their cast bar ended: the barrage commits to all of it.
typedef struct {
	int x[MAXPLAYERS], y[MAXPLAYERS], z[MAXPLAYERS], angle[MAXPLAYERS], pitch[MAXPLAYERS];
} avalanche_aim_T;

avalanche_aim_T module& GetAvalancheAim() {
	static avalanche_aim_T s;
	return s;
}

// How hard one tic throws, as a multiple of the pace still needed: often nothing, now and then a burst.
// The weights average out to the pace, so the throw still ends on time.
int AvalancheSurge() {
	int roll = random(1, 100);
	if(roll <= 30)
		return 0;
	if(roll <= 60)
		return 0.6;
	if(roll <= 80)
		return 1.4;
	if(roll <= 93)
		return 2.2;
	return 3.5;
}

// Slows every enemy on the landing ground: the throw's own fan, NEAR to FAR along the committed aim and
// within a band of the height it gives there. A box test throws out most of the list before any maths.
void SlowAvalancheGround(int pnum, int ox, int oy, int oz, int face, int cp, int sp) {
	int pct = GetSpellValue(pnum, SPL_AVALANCHE, SPELLVAL_DAMAGE2);
	if(pct <= 0)
		return;

	int c = cos(face), s = sin(face);
	int near = FixedMul(DND_AVALANCHE_NEAR, cp) - DND_AVALANCHE_SLOWMARGIN;
	int far = FixedMul(DND_AVALANCHE_FAR, cp) + DND_AVALANCHE_SLOWMARGIN;
	int spread = FixedDiv(sin(DND_AVALANCHE_FANARC), cos(DND_AVALANCHE_FANARC));
	int mn, v, dx, dy, along, lat;
	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		v = UsedMonsterTIDs[mn];
		dx = GetActorX(v) - ox;
		if(dx > far || dx < -far)
			continue;
		dy = GetActorY(v) - oy;
		if(dy > far || dy < -far)
			continue;

		along = FixedMul(dx, c) + FixedMul(dy, s);
		if(along < near || along > far)
			continue;
		lat = FixedMul(dy, c) - FixedMul(dx, s);
		if(abs(lat) > FixedMul(along, spread) + DND_AVALANCHE_SLOWMARGIN)
			continue;
		if(abs(GetActorZ(v) - (oz - FixedDiv(FixedMul(along, sp), cp))) > DND_AVALANCHE_SLOWZBAND)
			continue;
		if(!IsActorAlive(v) || !IsSpellEnemy(v))
			continue;

		SlowMonster(v, pct, DND_AVALANCHE_SLOWTICS);
	}
}

// Called by "DnD Spell Cast" once the bar is done, before the script that throws gets to run.
void RecordAvalancheAim(int pnum) {
	int caster = pnum + P_TIDSTART;
	GetAvalancheAim().x[pnum] = GetActorX(caster);
	GetAvalancheAim().y[pnum] = GetActorY(caster);
	GetAvalancheAim().z[pnum] = GetActorZ(caster);
	GetAvalancheAim().angle[pnum] = GetActorAngle(caster);
	GetAvalancheAim().pitch[pnum] = GetActorPitch(caster);
}

Script "DnD Avalanche" (int pnum) {
	int caster = pnum + P_TIDSTART;

	// Copied out at once, so a quick recast cannot move a barrage already under way.
	int ox = GetAvalancheAim().x[pnum], oy = GetAvalancheAim().y[pnum], oz = GetAvalancheAim().z[pnum];
	int face = GetAvalancheAim().angle[pnum];
	int pitch = Clamp_Between(GetAvalancheAim().pitch[pnum], -DND_AVALANCHE_PITCHMAX, DND_AVALANCHE_PITCHMAX);
	int cp = cos(pitch), sp = sin(pitch);

	// The throw lasts the spell's duration, so gear stretches it; the ice keeps its pace, so more is thrown.
	int span = Max(1, GetSpellDurationTics(pnum, SPL_AVALANCHE));

	// Its loop where the ice gathers, from the delay until the last of it is down.
	StartSpellLoopSound(pnum, DND_LOOPSOUND_AVALANCHE, ox - FixedMul(cos(face), DND_AVALANCHE_BEHIND),
		oy - FixedMul(sin(face), DND_AVALANCHE_BEHIND), oz + DND_AVALANCHE_RISE * 3 / 4,
		DND_AVALANCHE_DELAY + span + DND_AVALANCHE_SOUNDTAIL, true);
	// The storm round it is client side, so the aim goes along packed: whole-unit x | y, z | span, angle | pitch.
	ACS_NamedExecuteAlways("DnD Avalanche FX", 0, ((ox >> 16) & 0xFFFF) | (oy & 0xFFFF0000),
		((oz >> 16) & 0xFFFF) | (span << 16), (face & 0xFFFF) | (pitch << 16));
	Delay(const:DND_AVALANCHE_DELAY);

	int total = Max(1, GetSpellValue(pnum, SPL_AVALANCHE, SPELLVAL_AMOUNT) * span /
		Max(1, (SpellDefs[SPL_AVALANCHE].base[SPELLVAL_DURATION] * TICRATE) >> 16));
	// The ice streams rather than falling in volleys, so for "+X projectiles" each sweep's worth of tics is one.
	total += GetSpellExtraProjectiles(pnum, SPL_AVALANCHE) * span / DND_AVALANCHE_SLOWEVERY;
	bool rips = SpellThresholdMet(pnum, SPL_AVALANCHE, DND_SPELL_THRESH_HIGH);
	int tid = TEMPORARY_SPELL_TID + pnum;
	int thrown = 0, t, rest, due, back, side, sx, sy, sz, la, ld, lx, ly, dx, dy, dz, len;

	// Over SPAN tics, each one surging or stalling around the pace the rest still needs; the last
	// throws whatever is left. The random fraction rounds a slow pace up now and then.
	for(t = 1; t <= span && thrown < total; ++t) {
		if(!PlayerInGame(pnum) || !IsActorAlive(caster))
			break;

		if(t % DND_AVALANCHE_SLOWEVERY == 1)
			SlowAvalancheGround(pnum, ox, oy, oz, face, cp, sp);

		rest = total - thrown;
		if(t == span)
			due = total;
		else
			due = thrown + Min(rest, (FixedMul(rest * 1.0 / (span - t + 1), AvalancheSurge()) +
				random(0, 0.99)) >> 16);
		for(; thrown < due; ++thrown) {
			back = random(DND_AVALANCHE_BEHIND / 2, DND_AVALANCHE_BEHIND * 3 / 2);
			side = random(-DND_AVALANCHE_SPREAD, DND_AVALANCHE_SPREAD);
			sx = ox - FixedMul(cos(face), back) + FixedMul(cos(face + 0.25), side);
			sy = oy - FixedMul(sin(face), back) + FixedMul(sin(face + 0.25), side);
			sz = oz + random(DND_AVALANCHE_RISELOW, DND_AVALANCHE_RISE * 5 / 4);

			la = face + random(-DND_AVALANCHE_FANARC, DND_AVALANCHE_FANARC);
			// Along the committed aim from the caster's feet: level lands on their floor, looking down nearer.
			ld = random(DND_AVALANCHE_NEAR, DND_AVALANCHE_FAR);
			lx = ox + FixedMul(cos(la), FixedMul(ld, cp));
			ly = oy + FixedMul(sin(la), FixedMul(ld, cp));
			dx = lx - sx;
			dy = ly - sy;
			dz = oz - FixedMul(ld, sp) - sz;

			// Spawn, not SpawnForced: a start point inside a wall behind the caster is skipped.
			if(!Spawn("Spell_AvalancheIce", sx, sy, sz, tid, VectorAngle(dx, dy) >> 8))
				continue;
			SetupSpellActor(tid, pnum, SPL_AVALANCHE);

			// Rank 10, the same rip bookkeeping the evolved Ice Spear uses, without a limit.
			if(rips && random(1, 100) <= DND_AVALANCHE_R10_RIP) {
				GiveActorInventory(tid, "DnD_MakeRipper", 1);
				SetActorProperty(tid, APROP_STAMINA, GetActorProperty(tid, APROP_STAMINA) | DND_DAMAGEFLAG_RIPSONCE);
				SetSpellRipLimit(pnum, tid, MAX_RIPCOUNT);
			}

			// It forms in the air first and launches itself, so the velocity waits on it. It hangs already
			// pointing along the throw; INHERITACTORPITCH tips the point up for a positive pitch.
			len = fdistance_delta(dx, dy, dz);
			if(len > 0) {
				SetUserVariable(tid, "user_vx", FixedMul(FixedDiv(dx, len), DND_AVALANCHE_SPEED));
				SetUserVariable(tid, "user_vy", FixedMul(FixedDiv(dy, len), DND_AVALANCHE_SPEED));
				SetUserVariable(tid, "user_vz", FixedMul(FixedDiv(dz, len), DND_AVALANCHE_SPEED));
				int dip = VectorAngle(VectorLength(dx, dy), dz);
				SetActorPitch(tid, dip > 0.5 ? dip - 1.0 : dip);
			}

			Thing_ChangeTID(tid, 0);
		}

		Delay(const:1);
	}

	// The last ice is still forming and falling, so the ground stays slow a little longer.
	for(t = 0; t < DND_AVALANCHE_SLOWTAIL && PlayerInGame(pnum) && IsActorAlive(caster); ++t) {
		Delay(const:DND_AVALANCHE_SLOWEVERY);
		SlowAvalancheGround(pnum, ox, oy, oz, face, cp, sp);
	}
}

// The storm round an avalanche, purely for show: Gust of Frost's winds blowing past the ice, its smoke
// riders, and haze over where it lands. It builds through the delay and dies down after the throw.
#define DND_AVALANCHE_FXAFTER 10        // tics the storm runs on after the throw
#define DND_AVALANCHE_FXFADE 16         // tics it takes to die down at the end
#define DND_AVALANCHE_WINDS 2.08        // winds a tic at the height of it
#define DND_AVALANCHE_RIDERS 0.3        // smoke riders a tic
#define DND_AVALANCHE_HAZE 0.9          // haze puffs a tic
#define DND_AVALANCHE_WINDLEN 2.5714    // Gust's pacing: a piece crosses 72 x its scale over its life
#define DND_AVALANCHE_SNOW 0.4          // snow clouds a tic, streaming down onto the landing ground
#define DND_AVALANCHE_SNOWOVER 44.0     // how far above the landing ground each one is aimed
#define DND_AVALANCHE_BILLOWS 0.35      // billows a tic, thrown up off the landing ground
#define DND_AVALANCHE_BILLOWAT 14       // tics after the delay before the first ice is down
#define DND_AVALANCHE_FLAKES 1.5        // flakes a tic in the spray

// Which wind: mostly wisps, some clouds, a few swooshes, curling to the side they start on.
str AvalancheWind(int side) {
	int r = random(1, 100);
	if(r <= 60)
		return side > 0 ? "Spell_GustOfFrost_WispL" : "Spell_GustOfFrost_WispR";
	if(r <= 85)
		return side > 0 ? "Spell_GustOfFrost_CloudL" : "Spell_GustOfFrost_CloudR";
	return side > 0 ? "Spell_GustOfFrost_SwooshL" : "Spell_GustOfFrost_SwooshR";
}

// One piece of the storm: along and side place it off the aim, sp sends it forward, s sizes it if set,
// and turn faces the model that far off the way it flies.
void SpawnAvalancheFX(str type, int ox, int oy, int oz, int face, int along, int side, int h, int sp, int s,
	int turn = 0) {
	int a = face + random(-0.025, 0.025);
	int x = ox + FixedMul(cos(face), along) + FixedMul(cos(face + 0.25), side);
	int y = oy + FixedMul(sin(face), along) + FixedMul(sin(face + 0.25), side);
	int tid = ScratchTID();
	if(!tid || !Spawn(type, x, y, oz + h, tid, ((a + turn) >> 8) & 0xFF))
		return;
	if(s) {
		SetActorProperty(tid, APROP_SCALEX, s);
		SetActorProperty(tid, APROP_SCALEY, s);
	}
	SetActorVelocity(tid, FixedMul(cos(a), sp), FixedMul(sin(a), sp), random(-0.8, 0.2), false, false);
	Thing_ChangeTID(tid, 0);
}

// One piece of the snow, sent along vx, vy, vz a tic and sized s; a lift of 0 or more sets it on the floor.
void SpawnAvalancheSnow(str type, int x, int y, int z, int vx, int vy, int vz, int s, int lift = -1) {
	int tid = ScratchTID();
	if(!tid || !Spawn(type, x, y, z, tid, 0))
		return;
	if(lift >= 0)
		SetActorPosition(tid, x, y, GetActorFloorZ(tid) + lift, 0);
	if(s) {
		SetActorProperty(tid, APROP_SCALEX, s);
		SetActorProperty(tid, APROP_SCALEY, s);
	}
	SetActorVelocity(tid, vx, vy, vz, false, false);
	Thing_ChangeTID(tid, 0);
}

// Snow and flakes start where the ice gathers and are aimed as it is, onto the landing ground: a cloud
// to SNOWOVER above it, arriving as it fades, and a flake under its own 0.4 gravity.
void AvalancheSnowFlung(int ox, int oy, int oz, int face, int cp, int sp, bool flake) {
	int back = random(0, DND_AVALANCHE_BEHIND * 3 / 2), side = random(-DND_AVALANCHE_SPREAD, DND_AVALANCHE_SPREAD);
	int sx = ox - FixedMul(cos(face), back) + FixedMul(cos(face + 0.25), side);
	int sy = oy - FixedMul(sin(face), back) + FixedMul(sin(face + 0.25), side);
	int sz = oz + random(DND_AVALANCHE_RISE * 9 / 10, DND_AVALANCHE_RISE * 7 / 5);
	int la = face + random(-DND_AVALANCHE_FANARC, DND_AVALANCHE_FANARC);
	int ld = random(DND_AVALANCHE_NEAR * 3 / 2, DND_AVALANCHE_FAR * 9 / 10);
	int dx = ox + FixedMul(cos(la), FixedMul(ld, cp)) - sx;
	int dy = oy + FixedMul(sin(la), FixedMul(ld, cp)) - sy;
	int dz = oz - FixedMul(ld, sp) - sz;
	int trip;
	if(flake) {
		trip = random(16, 26);
		SpawnAvalancheSnow(random(0, 2) ? (random(0, 1) ? "WhiteBallParticle2" : "WhiteBallParticle3") : "WhiteBallParticle4",
			sx, sy, sz, dx / trip, dy / trip, dz / trip + trip * 0.2, 0);
	}
	else {
		trip = random(40, 50);
		SpawnAvalancheSnow("Spell_AvalancheSnow", sx, sy, sz, dx / trip, dy / trip, (dz + DND_AVALANCHE_SNOWOVER) / trip,
			random(0.7, 1.0));
	}
}

// Snow thrown up off the landing ground, drifting on and rising a little.
void AvalancheSnowBillow(int ox, int oy, int oz, int face, int cp, int sp) {
	int la = face + random(-DND_AVALANCHE_FANARC, DND_AVALANCHE_FANARC);
	int ld = random(DND_AVALANCHE_NEAR * 6 / 5, DND_AVALANCHE_FAR);
	int v = random(2.0, 4.0);
	SpawnAvalancheSnow("Spell_AvalancheBillow", ox + FixedMul(cos(la), FixedMul(ld, cp)),
		oy + FixedMul(sin(la), FixedMul(ld, cp)), oz - FixedMul(ld, sp), FixedMul(cos(la), v), FixedMul(sin(la), v),
		random(0.2, 0.7), random(0.6, 0.9), random(0.0, 24.0));
}

Script "DnD Avalanche FX" (int xy, int zs, int aim) CLIENTSIDE {
	int ox = xy << 16, oy = xy & 0xFFFF0000, oz = zs << 16;
	int fxtics = DND_AVALANCHE_DELAY + (zs >> 16) + DND_AVALANCHE_FXAFTER;
	int face = aim & 0xFFFF, pitch = aim >> 16;
	int cp = cos(pitch), sp = sin(pitch);
	// dnd_nofx leaves the snow out: it is the heaviest part of the storm to draw.
	bool snow = !GetCVar("dnd_nofx");
	int t, i, n, peak, side, s, roll;
	for(t = 0; t < fxtics; ++t) {
		if(t < DND_AVALANCHE_DELAY)
			peak = (t + 1) * 1.0 / DND_AVALANCHE_DELAY;
		else if(t >= fxtics - DND_AVALANCHE_FXFADE)
			peak = (fxtics - t) * 1.0 / DND_AVALANCHE_FXFADE;
		else
			peak = 1.0;

		// Winds from where the ice gathers, blowing out over where it lands. Half curl out to their side
		// as Gust's do; half are turned a quarter onto the slope, with the variant that rolls them forward.
		n = (FixedMul(DND_AVALANCHE_WINDS, peak) + random(0, 0.99)) >> 16;
		for(i = 0; i < n; ++i) {
			side = random(-DND_AVALANCHE_SPREAD, DND_AVALANCHE_SPREAD);
			s = random(6.0, 9.0);
			roll = random(0, 1);
			SpawnAvalancheFX(AvalancheWind(roll ? -side : side), ox, oy, oz, face,
				random(-DND_AVALANCHE_BEHIND * 3 / 2, 64.0), side, random(8.0, DND_AVALANCHE_RISE),
				FixedMul(s, DND_AVALANCHE_WINDLEN), s, roll ? (side > 0 ? 0.25 : -0.25) : 0);
		}

		// Smoke riders low down, trailing Frost Bomb's smoke the length of the slope.
		n = (FixedMul(DND_AVALANCHE_RIDERS, peak) + random(0, 0.99)) >> 16;
		for(i = 0; i < n; ++i)
			SpawnAvalancheFX("Spell_GustOfFrost_SmokeRider", ox, oy, oz, face,
				random(-DND_AVALANCHE_BEHIND, 32.0), random(-DND_AVALANCHE_SPREAD, DND_AVALANCHE_SPREAD),
				random(0.0, 48.0), random(12.0, 20.0), 0);

		// Haze drifting over the whole landing ground.
		n = (FixedMul(DND_AVALANCHE_HAZE, peak) + random(0, 0.99)) >> 16;
		for(i = 0; i < n; ++i)
			SpawnAvalancheFX(random(0, 1) ? "VeryGraySmoke2" : "VeryGraySmoke4", ox, oy, oz, face,
				random(-DND_AVALANCHE_BEHIND, DND_AVALANCHE_FAR), random(-DND_AVALANCHE_SPREAD, DND_AVALANCHE_SPREAD),
				random(0.0, 40.0), random(2.0, 5.0), 0);

		// Snow: clouds and flakes flung down the slope, and billows off the ground once the ice is down.
		if(snow) {
			n = (FixedMul(DND_AVALANCHE_SNOW, peak) + random(0, 0.99)) >> 16;
			for(i = 0; i < n; ++i)
				AvalancheSnowFlung(ox, oy, oz, face, cp, sp, false);
			n = (FixedMul(DND_AVALANCHE_FLAKES, peak) + random(0, 0.99)) >> 16;
			for(i = 0; i < n; ++i)
				AvalancheSnowFlung(ox, oy, oz, face, cp, sp, true);
			if(t >= DND_AVALANCHE_DELAY + DND_AVALANCHE_BILLOWAT) {
				n = (FixedMul(DND_AVALANCHE_BILLOWS, peak) + random(0, 0.99)) >> 16;
				for(i = 0; i < n; ++i)
					AvalancheSnowBillow(ox, oy, oz, face, cp, sp);
			}
		}

		Delay(const:1);
	}
}

// ============================ Winter Orb ============================
// A slow orb ripping each enemy once, throwing real Ice Bolts (the caster's own rank) in a spiral.

#define DND_WINTERORB_SPEED 6
#define DND_WINTERORB_SPIRALSTEP 0.11   // turn between throws, a share of a full turn (~40 degrees)
#define DND_WINTERORB_ORBITERS 2        // companions circling the orb, evenly spaced

Script "DnD Winter Orb Setup" (void) {
	// Each enemy once, and no cap on how many.
	int pnum = GetSpellActorOwner();
	if(pnum != -1)
		SetSpellRipLimit(pnum, 0, MAX_RIPCOUNT);
	else
		SetInventory("DnD_RipLimit", MAX_RIPCOUNT);
	SetResultValue(0);
}

// SXF_SETMASTER only links monsters here, so this points the companions at the orb, as the torch
// flares do: the orb lends itself a local tid just long enough for SetPointer to find it.
Script "DnD Winter Orb Orbiters" (void) CLIENTSIDE {
	int old = ActivatorTID();
	int orb = ScratchTID();
	if(!orb) {
		SetResultValue(0);
		Terminate;
	}
	Thing_ChangeTID(0, orb);

	for(int i = 0; i < DND_WINTERORB_ORBITERS; ++i) {
		int tid = ScratchTID();
		if(!tid || !SpawnForced("Spell_WinterOrb_Orbiter", GetActorX(orb), GetActorY(orb), GetActorZ(orb), tid, 0))
			continue;

		// The orbiter takes this as its starting place round the orb.
		SetActorAngle(tid, GetActorAngle(orb) + i * 1.0 / DND_WINTERORB_ORBITERS);
		SetActivator(tid);
		SetPointer(AAPTR_MASTER, orb);
		Thing_ChangeTID(tid, 0);
	}

	Thing_ChangeTID(orb, old);
	SetResultValue(0);
}

// Called by the orb on each throw; k counts them, which is what turns the spiral.
Script "DnD Winter Orb Throw" (int k) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	// The orb borrows a scratch tid only for the spawn call, which takes its position and facing.
	int orb = TEMPORARY_DATADUMMY_TID + pnum;
	Thing_ChangeTID(0, orb);

	bool shard = (k & 1) && SpellThresholdMet(pnum, SPL_WINTERORB, DND_SPELL_THRESH_HIGH);
	int off = (k * DND_WINTERORB_SPIRALSTEP) % 1.0;

	// Winter Orb's "+X projectiles" adds to each throw, fanned around its spiral angle; the orb stays one.
	int n = 1 + GetSpellExtraProjectiles(pnum, SPL_WINTERORB);
	for(int i = 0; i < n; ++i) {
		if(shard)
			SpawnSpellProjectile(pnum, SPL_FROSTSHARDS, "Spell_FrostShard", DND_FROSTSHARDS_SPEED, 0,
				off + FanOffset(i, DND_SPELL_FANSPREAD), 0, 0, orb, true, 0, 0, SPL_WINTERORB);
		else
			SpawnSpellProjectile(pnum, SPL_ICEBOLT, "Spell_IceBolt", DND_ICEBOLT_SPEED, 0,
				off + FanOffset(i, DND_SPELL_FANSPREAD), 0, 0, orb, true, 0, 0, SPL_WINTERORB);
	}

	Thing_ChangeTID(orb, 0);
	SetResultValue(0);
}

// ============================ Glacial Cascade ============================
// An invisible floor-hugging front carries the wave, so a wall ends it; it calls a burst every STEP tics.

#define DND_GLACIALCASCADE_STEP 8         // tics between bursts
#define DND_GLACIALCASCADE_WIDTH 80       // one burst's radius, before area modifiers
#define DND_GLACIALCASCADE_ZBAND 64       // how far above or below the burst an enemy is still caught
#define DND_GLACIALCASCADE_R5_RANGE 25    // percent increased range
#define DND_GLACIALCASCADE_RAMP 120       // each burst as a percent of the last
#define DND_GLACIALCASCADE_FINAL 200      // and the final one, of the one before it
#define DND_GLACIALCASCADE_WIDEN 110      // each burst's width as a percent of the last
#define DND_GLACIALCASCADE_R10_WIDEN 125  // and at rank 10, instead
#define DND_GLACIALCASCADE_KNOCK 18       // the one shove
#define DND_GLACIALCASCADE_VOLMIN 0.7     // the first burst's volume; the last is at full

// AMOUNT, one more from rank 5 and one per "+X projectiles"; each also adds range, to keep them as far apart.
int GetGlacialCascadeBursts(int pnum) {
	return Max(1, GetSpellValue(pnum, SPL_GLACIALCASCADE, SPELLVAL_AMOUNT)) +
		SpellThresholdMet(pnum, SPL_GLACIALCASCADE, DND_SPELL_THRESH_LOW) + GetSpellExtraProjectiles(pnum, SPL_GLACIALCASCADE);
}

void CastGlacialCascade(int pnum) {
	int range = GetSpellValue(pnum, SPL_GLACIALCASCADE, SPELLVAL_RADIUS);
	if(SpellThresholdMet(pnum, SPL_GLACIALCASCADE, DND_SPELL_THRESH_LOW))
		range = range * (100 + DND_GLACIALCASCADE_R5_RANGE) / 100;

	// Paced so burst k lands at (k + 1) / bursts of the range. Extra steps go on past the last at the same spacing.
	int bursts = GetGlacialCascadeBursts(pnum);
	range = range * bursts / Max(1, bursts - GetSpellExtraProjectiles(pnum, SPL_GLACIALCASCADE));
	int speed = Max(1, range / (bursts * DND_GLACIALCASCADE_STEP));

	PlaySound(pnum + P_TIDSTART, "GlacialCascade/Cast", CHAN_7);
	SpawnSpellProjectile(pnum, SPL_GLACIALCASCADE, "Spell_GlacialCascade", speed, 0, 0, 0, 0, 0, true);
}

// The front calls this with its own burst index. The cast tic it stamps on burst 0 is the cascade's
// identity, so "knocks back once" holds across all its bursts without a list.
Script "DnD Glacial Cascade Burst" (int k) {
	int pnum = GetSpellActorOwner();
	int bursts = pnum == -1 ? 0 : GetGlacialCascadeBursts(pnum);
	if(k >= bursts) {
		SetActorState(0, "Done");
		SetResultValue(0);
		Terminate;
	}

	if(!k)
		SetUserVariable(0, "user_extra", Timer());
	int id = GetUserVariable(0, "user_extra");

	int caster = pnum + P_TIDSTART;
	int dmg = PriceSpellDamage(pnum, SPL_GLACIALCASCADE, GetSpellValue(pnum, SPL_GLACIALCASCADE, SPELLVAL_DAMAGE),
		DND_WDMG_ISRADIUSDMG);
	int w = ScaleSpellArea(pnum, SPL_GLACIALCASCADE, DND_GLACIALCASCADE_WIDTH << 16) >> 16;
	int widen = SpellThresholdMet(pnum, SPL_GLACIALCASCADE, DND_SPELL_THRESH_HIGH) ?
		DND_GLACIALCASCADE_R10_WIDEN : DND_GLACIALCASCADE_WIDEN;

	int i;
	for(i = 1; i <= k; ++i) {
		dmg = dmg * (i == bursts - 1 ? DND_GLACIALCASCADE_FINAL : DND_GLACIALCASCADE_RAMP) / 100;
		w = w * widen / 100;
	}

	int bx = GetActorX(0), by = GetActorY(0), bz = GetActorZ(0);
	int push = GetActorAngle(0) >> 8;

	// Width, burst index, burst count and heading in one argument; the FX script has only four.
	int info = Min(w, 0xFFF) | (k << 12) | (Min(bursts, 0xF) << 16) | ((push & 0xFF) << 20);
	ACS_NamedExecuteWithResult("DnD Glacial Cascade FX", bx, by, bz, info);

	// Both layers together, louder the further into the cascade: 0.6 on the first burst, 1.0 on the last.
	int vol = DND_GLACIALCASCADE_VOLMIN + (1.0 - DND_GLACIALCASCADE_VOLMIN) * k / Max(1, bursts - 1);
	PlaySound(0, "GlacialCascade/Explode", CHAN_5, vol);
	PlaySound(0, "GlacialCascade/ExplodeAdd", CHAN_6, vol);

	// Everything the front can tell is read; now the caster becomes the activator, as the damage path
	// reads THEIR inventory and owns the hit beep.
	if(k + 1 >= bursts)
		SetActorState(0, "Done");
	if(!SetActivator(caster)) {
		SetResultValue(0);
		Terminate;
	}

	int mn, v, dx, dy, out;
	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		v = UsedMonsterTIDs[mn];

		dx = (GetActorX(v) - bx) >> 16;
		if(dx > w || dx < -w)
			continue;
		dy = (GetActorY(v) - by) >> 16;
		if(dy > w || dy < -w || dx * dx + dy * dy > w * w)
			continue;
		if(abs(GetActorZ(v) - bz) > (DND_GLACIALCASCADE_ZBAND << 16))
			continue;
		if(!IsActorAlive(v) || !IsSpellEnemy(v))
			continue;

		out = HandleDamageDeal(caster, v, dmg, GetSpellDamageType(SPL_GLACIALCASCADE), SPL_GLACIALCASCADE,
			DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISRADIUSDMG, 0, 0, 0, 0, true);
		// Channel 5, as the ripper hit sounds use on their victims.
		if(out > 0) {
			PlaySound(v, "GlacialCascade/Hit", CHAN_5, 1.0);
			Thing_Damage2(v, out, "SkipHandle");
		}

		// Once per cascade: the first burst to reach an enemy shoves it along the wave.
		if(IsActorAlive(v) && CheckActorInventory(v, "DnD_CascadeKnocked") != id) {
			SetActorInventory(v, "DnD_CascadeKnocked", id);
			ThrustThing(push, DND_GLACIALCASCADE_KNOCK * GetMonsterPushPercent(v) / 100, 0, v);
		}
	}

	SetResultValue(0);
}

// The eruption, CLIENTSIDE as the crystals are. Count and size run from the first burst's to the
// last's by how far along the cascade it is, so the final one looks the same with four or five bursts.
#define DND_GLACIALCASCADE_CRYSTALS 8      // the first burst's crystals, at the base width
#define DND_GLACIALCASCADE_CRYSTALSEND 23  // the last burst's, before its extra
#define DND_GLACIALCASCADE_CRYSTALSLAST 4  // the final burst's extra on top
#define DND_GLACIALCASCADE_CRYSTALSMAX 40
#define DND_GLACIALCASCADE_SIZE 110        // the first burst's crystals, in hundredths of the model
#define DND_GLACIALCASCADE_SIZEEND 180     // the last burst's, before its kick
#define DND_GLACIALCASCADE_GROWLAST 130    // and the final burst's kick on top
#define DND_GLACIALCASCADE_FORWARD 1.6     // how hard the crystals turn to point along the wave
#define DND_GLACIALCASCADE_FLOOR 115       // the floor patch's radius, as a percent of the burst's
#define DND_GLACIALCASCADE_SMOKE 5         // smoke rounds off the first burst's floor, at the base width
#define DND_GLACIALCASCADE_SMOKEEND 10     // and off the last's
#define DND_GLACIALCASCADE_SMOKEMAX 16
#define DND_GLACIALCASCADE_ALONG 135       // MODELDEF's stretch of the floor along the heading, in percent
// Half-extent the floor quad SHOWS at scale 1: flat2 is 15.62, and CascadeFloor.fp's ragged rim trims it.
#define DND_GLACIALCASCADE_QUADR 12.5

// One puff over a floor patch of radius r, landing within share percent of it and rising a little.
void SpawnCascadeSmoke(str type, int bx, int by, int bz, int r, int fwd, int share) {
	int lim = r * share / 100;
	int along = random(-lim, lim) * DND_GLACIALCASCADE_ALONG / 100, across = random(-lim, lim);
	int x = bx + FixedMul(cos(fwd), along << 16) - FixedMul(sin(fwd), across << 16);
	int y = by + FixedMul(sin(fwd), along << 16) + FixedMul(cos(fwd), across << 16);
	int tid = ScratchTID();
	if(tid && SpawnForced(type, x, y, bz + (random(0, r * 13 / 100) << 16), tid, 0)) {
		SetActorVelocity(tid, random(-3.5, 3.5), random(-3.5, 3.5), random(-1.5, 1.5), false, false);
		Thing_ChangeTID(tid, 0);
	}
}

Script "DnD Glacial Cascade FX" (int bx, int by, int bz, int info) CLIENTSIDE {
	int w = info & 0xFFF;
	int k = (info >> 12) & 0xF;
	int bursts = Max(1, (info >> 16) & 0xF);
	int fwd = ((info >> 20) & 0xFF) << 8;
	bool last = k + 1 >= bursts;
	// How far along the cascade this burst is, 0 to 1.
	int prog = bursts > 1 ? k * 1.0 / (bursts - 1) : 1.0;

	int size = DND_GLACIALCASCADE_SIZE + FixedMul(DND_GLACIALCASCADE_SIZEEND - DND_GLACIALCASCADE_SIZE, prog);
	if(last)
		size = size * DND_GLACIALCASCADE_GROWLAST / 100;

	// A wider burst (the widening, area gear) packs proportionally more.
	int n = (DND_GLACIALCASCADE_CRYSTALS + FixedMul(DND_GLACIALCASCADE_CRYSTALSEND - DND_GLACIALCASCADE_CRYSTALS, prog)) *
		Max(w, DND_GLACIALCASCADE_WIDTH) / DND_GLACIALCASCADE_WIDTH;
	if(last)
		n += DND_GLACIALCASCADE_CRYSTALSLAST;
	n = Min(n, DND_GLACIALCASCADE_CRYSTALSMAX);

	// Additive crystals pile up to white, so the denser bursts are each a little fainter.
	int alpha = 0.9 - FixedMul(0.45, prog);

	// The frosted floor under it, stretched along the wave. Raised a hair per burst so overlaps never fight.
	int pad = ScratchTID();
	int r = w * DND_GLACIALCASCADE_FLOOR / 100;
	if(pad && SpawnForced("Spell_GlacialCascadeFloor", bx, by, bz + k * 0.5, pad, 0)) {
		SetActorAngle(pad, fwd);
		SetUserVariable(pad, "user_s", FixedDiv(r << 16, DND_GLACIALCASCADE_QUADR) * 100 >> 16);
		Thing_ChangeTID(pad, 0);
	}

	// Smoke off the floor: more rounds further along and on wider bursts. A round is all three
	// smokes, the 2nd and 3rd skipping a fifth and two fifths of the time, the bigger ones landing wider.
	int puffs = Min(DND_GLACIALCASCADE_SMOKEMAX,
		(DND_GLACIALCASCADE_SMOKE + FixedMul(DND_GLACIALCASCADE_SMOKEEND - DND_GLACIALCASCADE_SMOKE, prog)) *
		Max(w, DND_GLACIALCASCADE_WIDTH) / DND_GLACIALCASCADE_WIDTH);
	for(int p = 0; p < puffs; ++p) {
		SpawnCascadeSmoke("VeryGraySmoke", bx, by, bz, r, fwd, 35);
		if(random(1, 5) <= 4)
			SpawnCascadeSmoke("VeryGraySmoke2", bx, by, bz, r, fwd, 26);
		if(random(1, 5) <= 3)
			SpawnCascadeSmoke("VeryGraySmoke3", bx, by, bz, r, fwd, 17);
	}

	// The first crystal stands in the middle, and the rest must see it, so none erupts past a wall.
	int i, anchor = 0, tid, a, d, lean, s, x, y;
	for(i = 0; i < n; ++i) {
		a = random(0, 1.0);
		if(i < 2) {
			d = random(0, w / 5);
			lean = random(0.03, 0.06);
			s = size * random(105, 125) / 100;
		}
		else {
			// Fanned out round the middle, leaning further the further out they stand.
			d = FixedMul(w * 3 / 4, fsqrt(random(0, 1.0)));
			lean = 0.09 + 0.07 * d / w + random(-0.02, 0.02);
			s = size * random(70, 105) / 100 * (115 - 30 * d / w) / 100;
		}

		tid = ScratchTID();
		x = bx + FixedMul(cos(a), d << 16);
		y = by + FixedMul(sin(a), d << 16);
		if(!tid || !SpawnForced("Spell_GlacialCascadeSpike", x, y, bz, tid, 0))
			continue;
		SetActorPosition(tid, x, y, GetActorFloorZ(tid), 0);
		if(anchor && !CheckSight(anchor, tid, 0)) {
			Thing_Remove(tid);
			continue;
		}

		// Leaning outward, pulled hard forward along the wave. A positive pitch tips the point up.
		SetActorAngle(tid, VectorAngle(cos(a) + FixedMul(DND_GLACIALCASCADE_FORWARD, cos(fwd)),
			sin(a) + FixedMul(DND_GLACIALCASCADE_FORWARD, sin(fwd))) + random(-0.04, 0.04));
		SetActorPitch(tid, 0.25 - lean);
		SetActorProperty(tid, APROP_ALPHA, alpha);
		SetUserVariable(tid, "user_sx", s);
		SetUserVariable(tid, "user_sy", s * random(85, 130) / 100);

		if(!anchor)
			anchor = tid;
		else
			Thing_ChangeTID(tid, 0);
	}

	if(anchor)
		Thing_ChangeTID(anchor, 0);
	SetResultValue(0);
}

// ============================ Shivering Armor ============================
// Heat Shield's shape: a damage-taken buff, plus a freeze chance on the WEARER that hitting it rolls.

// The ally nearest the caster, or -1.
int GetNearestAlly(int pnum) {
	int best = -1, bestd = 0x7FFFFFFF, d;
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(i == pnum || !PlayerInGame(i) || !IsActorAlive(i + P_TIDSTART))
			continue;
		d = fdistance(pnum + P_TIDSTART, i + P_TIDSTART);
		if(d < bestd) {
			bestd = d;
			best = i;
		}
	}
	return best;
}

void GrantShiveringArmor(int pnum, int target, int dur) {
	int ptid = target + P_TIDSTART;

	// Hundredths of a percent, as Hatred's: the rank step is 1.5.
	HandlePlayerBuffAssignment(target, pnum + P_TIDSTART, BTI_SPELL_SHIVERINGARMOR, 0, 0, dur,
		(GetSpellValue(pnum, SPL_SHIVERINGARMOR, SPELLVAL_DAMAGE) * 100) >> 16);

	int armor = GetSpellValue(pnum, SPL_SHIVERINGARMOR, SPELLVAL_AMOUNT);
	if(armor > 0)
		HandlePlayerBuffAssignment(target, pnum + P_TIDSTART, BTI_SPELL_SHIVERINGARMOR_ARMOR, 0, 0, dur, armor);

	// Tested BEFORE the stamp, so a recast refreshes rather than raising a second pair of crystals.
	bool wasUp = !!CheckActorInventory(ptid, "DnD_ShiveringArmorChance");
	SetActorInventory(ptid, "DnD_ShiveringArmorChance", GetSpellValue(pnum, SPL_SHIVERINGARMOR, SPELLVAL_DAMAGE2));

	// Rank 10: rippers stop on the wearer. Dense Exoskeleton shares the flag, so each side checks the other.
	if(SpellThresholdMet(pnum, SPL_SHIVERINGARMOR, DND_SPELL_THRESH_HIGH) &&
		!CheckActorInventory(ptid, "DnD_ShiveringRipGuard")) {
		SetActorInventory(ptid, "DnD_ShiveringRipGuard", 1);
		GiveActorInventory(ptid, "Perk_RipImmunityOn", 1);
	}

	PlaySound(ptid, "ShiveringArmor/Cast", CHAN_6);

	if(!wasUp)
		RaisePlayerAttachment(target, DND_PLAYERFX_SHIVERINGARMOR);
	ACS_NamedExecuteAlways("DnD Shivering Armor Timer", 0, target, Timer() + dur);
}

void CastShiveringArmor(int pnum) {
	int target = GetSpellLockTarget(pnum);
	if(target < 0 || !PlayerInGame(target) || !IsActorAlive(target + P_TIDSTART))
		target = pnum;

	int dur = GetSpellDurationTics(pnum, SPL_SHIVERINGARMOR);
	GrantShiveringArmor(pnum, target, dur);

	// Rank 5: an ally target covers the caster too, and no target covers the nearest ally too.
	if(SpellThresholdMet(pnum, SPL_SHIVERINGARMOR, DND_SPELL_THRESH_LOW)) {
		if(target != pnum)
			GrantShiveringArmor(pnum, pnum, dur);
		else if((target = GetNearestAlly(pnum)) != -1)
			GrantShiveringArmor(pnum, target, dur);
	}
}

// Owns the wearer's markers; the reduction and armor are buffs and expire on their own ticker. The end
// tic is stamped so a recast takes over and the older timer bows out, as Heat Shield's does.
Script "DnD Shivering Armor Timer" (int pnum, int endtic) {
	int ptid = pnum + P_TIDSTART;
	SetActorInventory(ptid, "DnD_ShiveringArmorEnd", endtic);

	while(Timer() < endtic && IsActorAlive(ptid) && CheckActorInventory(ptid, "DnD_ShiveringArmorEnd") == endtic)
		Delay(const:TICRATE / 2);

	if(CheckActorInventory(ptid, "DnD_ShiveringArmorEnd") != endtic)
		Terminate;

	SetActorInventory(ptid, "DnD_ShiveringArmorChance", 0);
	SetActorInventory(ptid, "DnD_ShiveringArmorEnd", 0);

	if(CheckActorInventory(ptid, "DnD_ShiveringRipGuard")) {
		SetActorInventory(ptid, "DnD_ShiveringRipGuard", 0);
		// Dense Exoskeleton may still hold the flag; it clears it itself when it ends.
		if(!CheckActorInventory(ptid, "DnD_RipImmuneTimer"))
			GiveActorInventory(ptid, "Perk_RipImmunityOff", 1);
	}
}

// ============================ Charged Bolt ============================

#define DND_CHARGEDBOLT_SPEED 10
#define DND_CHARGEDBOLT_STEP 3          // tics per turn of DECORATE's Fly loop, one A_CountDown each
#define DND_CHARGEDBOLT_SPREAD 0.05     // turn fraction either side a volley leaves within, about 18 degrees
#define DND_CHARGEDBOLT_R5_LIFE 5.0

void CastChargedBolt(int pnum) {
	int n = GetSpellValue(pnum, SPL_CHARGEDBOLT, SPELLVAL_AMOUNT) + GetSpellExtraProjectiles(pnum, SPL_CHARGEDBOLT);
	int tics = GetSpellDurationTics(pnum, SPL_CHARGEDBOLT);

	// Rank 5 stretches the life by its own share, on top of whatever duration gear already did to the row.
	if(SpellThresholdMet(pnum, SPL_CHARGEDBOLT, DND_SPELL_THRESH_LOW))
		tics = FixedMul(tics, FixedDiv(DND_CHARGEDBOLT_R5_LIFE, SpellDefs[SPL_CHARGEDBOLT].base[SPELLVAL_DURATION]));

	int steps = Max(1, tics / DND_CHARGEDBOLT_STEP);
	for(int i = 0; i < n; ++i)
		SpawnSpellProjectile(pnum, SPL_CHARGEDBOLT, "Spell_ChargedBolt", DND_CHARGEDBOLT_SPEED, 0,
			i ? random(-DND_CHARGEDBOLT_SPREAD, DND_CHARGEDBOLT_SPREAD) : 0, 0, steps);
	PlaySound(pnum + P_TIDSTART, "ChargedBolt/Cast", CHAN_7);
}

// ============================ Lightning Spear ============================

#define DND_LIGHTNINGSPEAR_SPEED 56
#define DND_LIGHTNINGSPEAR_STRIKESPEED 28.0
#define DND_LIGHTNINGSPEAR_R5_PASSES 2

// Bounces a fresh spear has, read once by its first state.
Script "DnD Lightning Spear Bounces" (void) {
	int pnum = GetSpellActorOwner();
	SetResultValue(pnum == -1 ? 0 : GetSpellValue(pnum, SPL_LIGHTNINGSPEAR, SPELLVAL_AMOUNT));
}

// Pass attempts once the spear meets an enemy.
Script "DnD Lightning Spear Passes" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	int n = GetSpellValue(pnum, SPL_LIGHTNINGSPEAR, SPELLVAL_AMOUNT2);
	if(SpellThresholdMet(pnum, SPL_LIGHTNINGSPEAR, DND_SPELL_THRESH_LOW))
		n += DND_LIGHTNINGSPEAR_R5_PASSES;
	SetResultValue(n);
}

// The bounce blast where the activator stands, stamped as the spear so it prices as DAMAGE2 in RADIUS.
Script "DnD Lightning Spear Blast" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum != -1)
		SpawnSpellAnchor(pnum, SPL_LIGHTNINGSPEAR, "Spell_LightningSpear_Blast", GetActorX(0), GetActorY(0), GetActorZ(0));
	SetResultValue(0);
}

// One pass attempt: a strike from where the spear stopped into the middle of what it hit.
Script "DnD Lightning Spear Strike" (void) {
	int pnum = GetSpellActorOwner();
	int victim = GetActorProperty(0, APROP_TRACERTID);
	if(pnum == -1 || !victim || !IsActorAlive(victim)) {
		SetResultValue(0);
		Terminate;
	}

	int x = GetActorX(0), y = GetActorY(0), z = GetActorZ(0);
	int tid = TEMPORARY_SPELL_TID + pnum;
	if(SpawnForced("Spell_LightningSpear_Strike", x, y, z, tid, 0)) {
		SetupSpellActor(tid, pnum, SPL_LIGHTNINGSPEAR);
		int dx = GetActorX(victim) - x, dy = GetActorY(victim) - y;
		int dz = GetActorZ(victim) + GetActorProperty(victim, APROP_HEIGHT) / 2 - z;
		int len = Max(VectorLength(VectorLength(dx, dy), dz), 1.0);
		SetActorAngle(tid, VectorAngle(dx, dy));
		SetActorVelocity(tid, FixedMul(FixedDiv(dx, len), DND_LIGHTNINGSPEAR_STRIKESPEED),
			FixedMul(FixedDiv(dy, len), DND_LIGHTNINGSPEAR_STRIKESPEED),
			FixedMul(FixedDiv(dz, len), DND_LIGHTNINGSPEAR_STRIKESPEED), false, false);
		Thing_ChangeTID(tid, 0);
	}
	SetResultValue(0);
}

// ============================ Lightning targeting and FX ============================

// The enemy under the crosshair, else the one nearest it inside the lock cone and in sight; 0 when none.
int GetSpellEnemyTarget(int pnum, int range) {
	int caster = pnum + P_TIDSTART;
	int t = PickActor(caster, GetActorAngle(caster), GetActorPitch(caster), range, 0, MF_SHOOTABLE, ML_BLOCKEVERYTHING, PICKAF_RETURNTID);
	if(IsMonster(t) && IsActorAlive(t) && IsSpellEnemy(t))
		return t;

	int a = GetActorAngle(caster), cx = cos(a), cy = sin(a);
	int best = 0, bestdot = cos(DND_SPELLLOCK_CONE);
	int i, m, dx, dy, dz, len, dot;
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(!IsActorAlive(m) || !IsSpellEnemy(m) || fdistance(caster, m) > range)
			continue;

		dx = GetActorX(m) - GetActorX(caster);
		dy = GetActorY(m) - GetActorY(caster);
		len = VectorLength(dx, dy);
		if(len <= 0)
			continue;

		dot = FixedMul(FixedDiv(dx, len), cx) + FixedMul(FixedDiv(dy, len), cy);
		if(dot <= bestdot)
			continue;

		dz = GetActorZ(m) + (GetActorProperty(m, APROP_HEIGHT) >> 1) - (GetActorZ(caster) + GetActorViewHeight(caster));
		if(cos(-GetActorPitch(caster) - VectorAngle(len, dz)) < cos(DND_SPELLLOCK_PITCHCONE))
			continue;
		if(!CheckSight(caster, m, CSF_NOBLOCKALL))
			continue;

		bestdot = dot;
		best = m;
	}
	return best;
}

#define DND_CRACKLE_LINE 7              // crackles strung along an arc from the caster to a target
#define DND_BEAM_HANDS 24.0             // a caster's beams leave from this far in front of them
#define DND_BEAM_DROP 16.0              // and this far below the eye
#define DND_BEAM_MIN 24.0               // a beam shorter than this is not drawn

// Client side: a beam model len long along +x, stretched between two points. It pitches from its velocity, so a crawl along the line aims it.
void SpawnStretchedBeam(str actor, int len, int x0, int y0, int z0, int x1, int y1, int z1) {
	int dx = x1 - x0, dy = y1 - y0, dz = z1 - z0;
	int dist = VectorLength(VectorLength(dx, dy), dz);
	// PITCHFROMMOMENTUM takes the absolute pitch when nothing moves sideways, so a straight drop would point up; lean it.
	if(dx < dist / 128 && dx > -dist / 128 && dy < dist / 128 && dy > -dist / 128)
		dx = dist / 128;
	int tid = ScratchTID();
	if(dist < DND_BEAM_MIN || !tid || !Spawn(actor, x0, y0, z0, tid))
		return;

	SetActorAngle(tid, VectorAngle(dx, dy));
	SetActorVelocity(tid, FixedDiv(dx, dist) / 100, FixedDiv(dy, dist) / 100, FixedDiv(dz, dist) / 100, false, false);
	SetActorProperty(tid, APROP_SCALEX, FixedDiv(dist, len));
	SetActorProperty(tid, APROP_SCALEY, FixedDiv(dist, len));
	Thing_ChangeTID(tid, 0);
}

// Two map coordinates in one script argument, whole units, for the clientside beam scripts.
int PackMapPair(int a, int b) {
	return ((a >> 16) & 0xFFFF) | (b & 0xFFFF0000);
}

// Sends a beam from a point to an enemy's middle to every client, on a script taking (from xy, to xy, z pair).
void SendBeamToEnemy(str fx, int x0, int y0, int z0, int victim) {
	ACS_NamedExecuteAlways(fx, 0, PackMapPair(x0, y0), PackMapPair(GetActorX(victim), GetActorY(victim)),
		PackMapPair(z0, GetActorZ(victim) + GetActorProperty(victim, APROP_HEIGHT) / 2));
}

// One hit of a spell from a base amount, priced and dealt, as area damage when area is set. Returns what landed.
int DealSpellAmount(int pnum, int spell, int victim, int base, bool area = false, int flags = 0) {
	int dmg = PriceSpellDamage(pnum, spell, base, area ? DND_WDMG_ISRADIUSDMG : 0);
	int dealt = HandleDamageDeal(pnum + P_TIDSTART, victim, dmg, GetSpellDamageType(spell), spell,
		DND_DAMAGEFLAG_ISSPELL | (area ? DND_DAMAGEFLAG_ISRADIUSDMG : 0) | flags, 0, 0, 0, 0, true);
	if(dealt > 0)
		Thing_Damage2(victim, dealt, "SkipHandle");
	return dealt;
}

// One rolled hit of a beam spell at pct of its damage, priced and dealt, as area damage when area is set. Returns what landed.
int DealSpellBeamHit(int pnum, int spell, int victim, int pct, bool area = false, int flags = 0) {
	return DealSpellAmount(pnum, spell, victim, RollSpellValue(pnum, spell, SPELLVAL_DAMAGE) * pct / 100, area, flags);
}

// Client side: a crackle somewhere on an actor's body.
void SpawnBodyCrackle(int tid, str actor) {
	int r = GetActorProperty(tid, APROP_RADIUS), h = GetActorProperty(tid, APROP_HEIGHT);
	Spawn(actor, GetActorX(tid) + random(-r, r), GetActorY(tid) + random(-r, r), GetActorZ(tid) + random(h / 4, h * 3 / 4));
}

// Client side: arc pieces strung from the caster's chest to the middle of a target, jittered, and a burst there.
void SpawnCrackleLine(int from, int to, str arc, str burst) {
	int x0 = GetActorX(from), y0 = GetActorY(from), z0 = GetActorZ(from) + GetActorViewHeight(from) - 12.0;
	int x1 = GetActorX(to), y1 = GetActorY(to), z1 = GetActorZ(to) + GetActorProperty(to, APROP_HEIGHT) / 2;
	for(int i = 1; i <= DND_CRACKLE_LINE; ++i) {
		int f = (1.0 * i) / (DND_CRACKLE_LINE + 1);
		Spawn(arc, x0 + FixedMul(x1 - x0, f) + random(-6.0, 6.0), y0 + FixedMul(y1 - y0, f) + random(-6.0, 6.0),
			z0 + FixedMul(z1 - z0, f) + random(-6.0, 6.0));
	}
	Spawn(burst, x1, y1, z1);
}

// ============================ Spark ============================

#define DND_SPARK_REACH 128.0           // how far ahead the area may sit, along the aim
#define DND_SPARK_PULLBACK 8.0          // off whatever the aim struck, so sight checks start in the open
#define DND_SPARK_PERIOD 5              // tics between zaps, about 0.15 seconds
#define DND_SPARK_R5_RADIUS 160
#define DND_SPARK_R10_DURATION 3.0
#define DND_SPARK_FX 3                  // crackles thrown across the area each zap
#define DND_SPARK_HITFX 4               // enemies a zap draws its hit burst on; the rest are hit all the same

// One zap: every enemy in the radius and in sight of the centre takes its own roll.
void SparkZap(int pnum, int cx, int cy, int cz, int r) {
	int caster = pnum + P_TIDSTART;
	ACS_NamedExecuteAlways("DnD Spark FX", 0, cx >> 16, cy >> 16, (r << 16) | ((cz >> 16) & 0xFFFF));

	int tid = ScratchTID();
	if(!tid || !SpawnForced("DnD_SpellAnchor", cx, cy, cz, tid, 0))
		return;
	PlaySound(tid, "Spark/Zap", CHAN_BODY, 0.5);

	int i, m, dx, dy, mz, dz, reach, dmg, dealt, fx = 0;
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(!IsActorAlive(m) || !IsSpellEnemy(m))
			continue;

		dx = (GetActorX(m) - cx) >> 16;
		dy = (GetActorY(m) - cy) >> 16;
		reach = r + (GetActorProperty(m, APROP_RADIUS) >> 16);
		if(dx * dx + dy * dy > reach * reach)
			continue;

		mz = GetActorZ(m);
		dz = cz < mz ? mz - cz : cz > mz + GetActorProperty(m, APROP_HEIGHT) ? cz - mz - GetActorProperty(m, APROP_HEIGHT) : 0;
		if(dz > r << 16 || !CheckSight(tid, m, CSF_NOBLOCKALL))
			continue;

		dmg = PriceSpellDamage(pnum, SPL_SPARK, RollSpellValue(pnum, SPL_SPARK, SPELLVAL_DAMAGE), DND_WDMG_ISRADIUSDMG);
		dealt = HandleDamageDeal(caster, m, dmg, GetSpellDamageType(SPL_SPARK), SPL_SPARK,
			DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISRADIUSDMG, 0, 0, 0, 0, true);
		if(dealt > 0)
			Thing_Damage2(m, dealt, "SkipHandle");
		if(fx++ < DND_SPARK_HITFX)
			ACS_NamedExecuteAlways("DnD Spell Clientside FX", 0, SPL_SPARK, m, 0);
	}
	Thing_Remove(tid);
}

// The area is fixed where the cast lands; its centre is kept as numbers, not a tid, across the zaps.
Script "DnD Spark" (int pnum) {
	int caster = pnum + P_TIDSTART;
	if(!TraceSpellAim(pnum, DND_SPARK_REACH))
		Terminate;

	int wait = 0;
	while(!ReadSpellAim(pnum) && wait < DND_SPELLAIM_WAIT) {
		Delay(const:1);
		++wait;
	}
	if(!SpellAimReady(pnum) || !PlayerInGame(pnum) || !IsActorAlive(caster))
		Terminate;

	auto aim = GetSpellAim();
	int cx = aim.x[pnum], cy = aim.y[pnum], cz = aim.z[pnum];
	int back = VectorAngle(GetActorX(caster) - cx, GetActorY(caster) - cy);
	cx += FixedMul(DND_SPARK_PULLBACK, cos(back));
	cy += FixedMul(DND_SPARK_PULLBACK, sin(back));

	int r = SpellThresholdMet(pnum, SPL_SPARK, DND_SPELL_THRESH_LOW) ? DND_SPARK_R5_RADIUS : GetSpellValue(pnum, SPL_SPARK, SPELLVAL_RADIUS);
	r = ScaleSpellArea(pnum, SPL_SPARK, r << 16) >> 16;

	int tics = GetSpellDurationTics(pnum, SPL_SPARK);
	if(SpellThresholdMet(pnum, SPL_SPARK, DND_SPELL_THRESH_HIGH))
		tics = FixedMul(tics, FixedDiv(DND_SPARK_R10_DURATION, SpellDefs[SPL_SPARK].base[SPELLVAL_DURATION]));

	PlaySound(caster, "Spark/Cast", CHAN_7);
	for(int z = Max(1, tics / DND_SPARK_PERIOD); z > 0; --z) {
		if(!PlayerInGame(pnum))
			break;
		SparkZap(pnum, cx, cy, cz, r);
		Delay(DND_SPARK_PERIOD);
	}
}

// Client side: a lit crackle at the centre and a few more scattered through the area. Packed is radius << 16 | z.
Script "DnD Spark FX" (int x, int y, int packed) CLIENTSIDE {
	int r = packed >> 16, z = (packed << 16) >> 16;
	Spawn("Spell_Spark_Flash", x << 16, y << 16, z << 16);
	for(int i = 0; i < DND_SPARK_FX; ++i) {
		int a = random(0, 1.0), d = random(0, r);
		Spawn("Spell_Spark_Crackle", (x << 16) + d * cos(a), (y << 16) + d * sin(a), (z + random(-r / 3, r / 3)) << 16);
	}
}

// ============================ Shock ============================

#define DND_SHOCK_RANGE 1024.0
#define DND_SHOCK_R5_DURATION 8.0
#define DND_SHOCK_FXEVERY 9             // tics between crackles on a shocked enemy
#define DND_SHOCK_BEAMLEN 64.0          // the beam model's length; the actor is stretched to the distance

// Client side: one puff on the target's middle and a thin beam to it from the caster's hands.
void SpawnShockBeam(int from, int to) {
	int x1 = GetActorX(to), y1 = GetActorY(to), z1 = GetActorZ(to) + GetActorProperty(to, APROP_HEIGHT) / 2;
	Spawn("Spell_Shock_Puff", x1, y1, z1);

	int a = GetActorAngle(from);
	SpawnStretchedBeam("Spell_Shock_Beam", DND_SHOCK_BEAMLEN, GetActorX(from) + FixedMul(DND_BEAM_HANDS, cos(a)),
		GetActorY(from) + FixedMul(DND_BEAM_HANDS, sin(a)), GetActorZ(from) + GetActorViewHeight(from) - DND_BEAM_DROP, x1, y1, z1);
}

// Strongest and longest win, as the other monster debuffs; the all-elements flag only ever turns on.
void ShockMonster(int victim, int tenths, int tics, bool all) {
	if(CheckActorInventory(victim, "DnD_Shocked") < tenths)
		SetActorInventory(victim, "DnD_Shocked", tenths);
	if(all)
		SetActorInventory(victim, "DnD_ShockedAll", 1);

	bool ticking = !!CheckActorInventory(victim, "DnD_ShockedTimer");
	if(CheckActorInventory(victim, "DnD_ShockedTimer") < tics) {
		SetActorInventory(victim, "DnD_ShockedTimer", tics);
		ACS_NamedExecuteAlways("DnD Shock FX", 0, victim, tics);
	}

	if(!ticking)
		ACS_NamedExecuteAlways("DnD Shock Timer", 0, victim);
}

// Shock from any spell: the caster's Shock rank sets it, rank 1 if they never learned it.
void ApplySpellShock(int pnum, int victim) {
	int rank = Max(1, GetSpellRank(pnum, SPL_SHOCK, true));
	int tenths = (GetSpellValue(pnum, SPL_SHOCK, SPELLVAL_DAMAGE, rank) * 10) >> 16;
	int tics = (GetSpellValue(pnum, SPL_SHOCK, SPELLVAL_DURATION, rank) * TICRATE) >> 16;
	if(rank >= DND_SPELL_THRESH_LOW)
		tics = FixedMul(tics, FixedDiv(DND_SHOCK_R5_DURATION, SpellDefs[SPL_SHOCK].base[SPELLVAL_DURATION]));
	ShockMonster(victim, tenths, tics, rank >= DND_SPELL_THRESH_HIGH);
}

void CastShock(int pnum) {
	int victim = GetSpellEnemyTarget(pnum, DND_SHOCK_RANGE);
	if(!victim)
		return;

	ApplySpellShock(pnum, victim);
	PlaySound(victim, "Shock/Hit", CHAN_7);
	ACS_NamedExecuteAlways("DnD Spell Clientside FX", 0, SPL_SHOCK, victim, pnum + 1);
}

Script "DnD Shock Timer" (int m_tid) {
	while(CheckActorInventory(m_tid, "DnD_ShockedTimer") && IsActorAlive(m_tid)) {
		TakeActorInventory(m_tid, "DnD_ShockedTimer", 1);
		Delay(const:1);
	}

	SetActorInventory(m_tid, "DnD_Shocked", 0);
	SetActorInventory(m_tid, "DnD_ShockedTimer", 0);
	SetActorInventory(m_tid, "DnD_ShockedAll", 0);
}

// Client side: crackles on a shocked enemy until its shock runs out; a reapplication only resets the clock.
Script "DnD Shock FX" (int m_tid, int tics) CLIENTSIDE {
	bool running = !!CheckActorInventory(m_tid, "DnD_ShockFXTimer");
	SetActorInventory(m_tid, "DnD_ShockFXTimer", tics);
	if(running)
		Terminate;

	for(int t = 0; CheckActorInventory(m_tid, "DnD_ShockFXTimer") && IsActorAlive(m_tid); ++t) {
		if(!(t % DND_SHOCK_FXEVERY))
			SpawnBodyCrackle(m_tid, "Spell_Shock_Crackle");
		TakeActorInventory(m_tid, "DnD_ShockFXTimer", 1);
		Delay(const:1);
	}
	SetActorInventory(m_tid, "DnD_ShockFXTimer", 0);
}

// ============================ Telekinesis ============================

#define DND_TELEKINESIS_PUSH 24         // shove at full strength, units a tic; AMOUNT is its percent
#define DND_TELEKINESIS_R5_STUN TICRATE
#define DND_TELEKINESIS_R10_HPPCT 10

void CastTelekinesis(int pnum) {
	int caster = pnum + P_TIDSTART;

	PlaySound(caster, "Telekinesis/Cast", CHAN_VOICE);

	int victim = GetSpellEnemyTarget(pnum, GetSpellValue(pnum, SPL_TELEKINESIS, SPELLVAL_RADIUS) << 16);
	if(!victim)
		return;

	int away = VectorAngle(GetActorX(victim) - GetActorX(caster), GetActorY(victim) - GetActorY(caster));
	ThrustThing(away >> 8, DND_TELEKINESIS_PUSH * GetSpellValue(pnum, SPL_TELEKINESIS, SPELLVAL_AMOUNT) / 100 * GetMonsterPushPercent(victim) / 100, 1, victim);
	PlaySound(victim, "Telekinesis/Push", CHAN_7);
	ACS_NamedExecuteAlways("DnD Spell Clientside FX", 0, SPL_TELEKINESIS, victim, pnum + 1);

	if(SpellThresholdMet(pnum, SPL_TELEKINESIS, DND_SPELL_THRESH_LOW))
		StunMonster(victim, ScaleSpellEffectTics(pnum, SPL_TELEKINESIS, DND_TELEKINESIS_R5_STUN));

	// Rank 10: a share of max health, never more than the DAMAGE row, then priced like any spell hit.
	int m_id = victim - DND_MONSTERTID_BEGIN;
	if(!SpellThresholdMet(pnum, SPL_TELEKINESIS, DND_SPELL_THRESH_HIGH) || m_id < 0 || m_id >= DND_MAX_MONSTERS)
		return;

	int dmg = Min(MonsterProperties[m_id].maxhp / 100 * DND_TELEKINESIS_R10_HPPCT, GetSpellValue(pnum, SPL_TELEKINESIS, SPELLVAL_DAMAGE));
	dmg = PriceSpellDamage(pnum, SPL_TELEKINESIS, dmg);
	if(dmg <= 0)
		return;

	int dealt = HandleDamageDeal(caster, victim, dmg, GetSpellDamageType(SPL_TELEKINESIS), SPL_TELEKINESIS,
		DND_DAMAGEFLAG_ISSPELL, 0, 0, 0, 0, true);
	if(dealt > 0)
		Thing_Damage2(victim, dealt, "SkipHandle");
}

// ============================ Static Field ============================

#define DND_STATICFIELD_PERIOD (5 * TICRATE)
#define DND_STATICFIELD_R5_PERIOD (5 * TICRATE / 2)
#define DND_STATICFIELD_POLL 5          // tics between looks for a target while a zap waits for one
#define DND_STATICFIELD_FXNEAR 6.0      // the bolt stands this far from the enemy's middle, toward its viewer

// A random enemy in reach and in sight takes the zap. False when there is none, so the zap is held.
bool StaticFieldZap(int pnum) {
	int ptid = pnum + P_TIDSTART;
	int r = ScaleSpellArea(pnum, SPL_STATICFIELD, GetSpellValue(pnum, SPL_STATICFIELD, SPELLVAL_RADIUS) << 16);
	int i, m, n = 0, victim = 0;
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(!IsActorAlive(m) || !IsSpellEnemy(m) || fdistance(ptid, m) > r || !CheckSight(ptid, m, CSF_NOBLOCKALL))
			continue;
		if(random(1, ++n) == 1)
			victim = m;
	}
	if(!victim)
		return false;

	ACS_NamedExecuteAlways("DnD Static Field FX", 0, GetActorX(victim) >> 16, GetActorY(victim) >> 16, GetActorZ(victim) >> 16);

	// Each zap lands at once, so it is a hit that can crit, unlike a tick delivered over time.
	int dmg = PriceSpellDamage(pnum, SPL_STATICFIELD, GetSpellValue(pnum, SPL_STATICFIELD, SPELLVAL_DAMAGE), DND_WDMG_ISRADIUSDMG);
	int dealt = HandleDamageDeal(ptid, victim, dmg, GetSpellDamageType(SPL_STATICFIELD), SPL_STATICFIELD,
		DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISRADIUSDMG, 0, 0, 0, 0, true);
	if(dealt > 0)
		Thing_Damage2(victim, dealt, "SkipHandle");

	if(SpellThresholdMet(pnum, SPL_STATICFIELD, DND_SPELL_THRESH_HIGH) && IsActorAlive(victim))
		ApplySpellShock(pnum, victim);
	return true;
}

// Lives while the aura is on and the player alive. The first zap is ready at once.
Script "DnD Static Field" (int pnum) {
	int ptid = pnum + P_TIDSTART;
	int ready = Timer();
	while(IsSpellActive(pnum, SPL_STATICFIELD) && PlayerInGame(pnum) && IsActorAlive(ptid)) {
		if(Timer() >= ready) {
			if(StaticFieldZap(pnum))
				ready = Timer() + (SpellThresholdMet(pnum, SPL_STATICFIELD, DND_SPELL_THRESH_LOW) ? DND_STATICFIELD_R5_PERIOD : DND_STATICFIELD_PERIOD);
		}
		Delay(Max(1, Min(DND_STATICFIELD_POLL, ready - Timer())));
	}
	SetSpellRunning(pnum, SPL_STATICFIELD, false);
}

// Client side: the bolt is flat, so it faces this viewer, a step in front of the enemy so its sprite cannot hide it.
Script "DnD Static Field FX" (int x, int y, int z) CLIENTSIDE {
	x <<= 16;
	y <<= 16;
	int a = random(0, 1.0);
	int me = ConsolePlayerNumber() + P_TIDSTART;
	if(me >= P_TIDSTART) {
		a = VectorAngle(GetActorX(me) - x, GetActorY(me) - y);
		x += FixedMul(DND_STATICFIELD_FXNEAR, cos(a));
		y += FixedMul(DND_STATICFIELD_FXNEAR, sin(a));
	}
	Spawn("Spell_StaticField_Strike", x, y, z << 16, 0, a >> 8);
}

// ============================ Forked Lightning ============================

#define DND_FORKBEAM_LEN 64.0           // the forking beam model's length; Chain Lightning draws it too
#define DND_FORKED_RANGE 1024.0         // reach to the enemy you face
#define DND_FORKED_REACH 320.0          // how far behind that enemy a fork may land
#define DND_FORKED_CONE 0.5             // cosine of the widest angle off the line a fork may take
#define DND_FORKED_R10_FORKS 5
#define DND_FORKED_MAXCAND 48

typedef struct {
	int tid[DND_FORKED_MAXCAND];
	int dist[DND_FORKED_MAXCAND];
} fork_cand_T;

fork_cand_T module& GetForkCandidates() {
	static fork_cand_T s;
	return s;
}

// Strikes the enemy you face, then forks from it to the nearest enemies behind it, within a cone along the line.
void CastForkedLightning(int pnum) {
	int ptid = pnum + P_TIDSTART;
	int target = GetSpellEnemyTarget(pnum, DND_FORKED_RANGE);
	if(!target)
		return;
	PlaySound(ptid, "ForkedLightning/Cast", CHAN_7);

	int forks = GetSpellValue(pnum, SPL_FORKEDLIGHTNING, SPELLVAL_AMOUNT) + GetSpellExtraProjectiles(pnum, SPL_FORKEDLIGHTNING);
	if(SpellThresholdMet(pnum, SPL_FORKEDLIGHTNING, DND_SPELL_THRESH_HIGH))
		forks += DND_FORKED_R10_FORKS;

	int tx = GetActorX(target), ty = GetActorY(target), tz = GetActorZ(target) + GetActorProperty(target, APROP_HEIGHT) / 2;
	int ang = VectorAngle(tx - GetActorX(ptid), ty - GetActorY(ptid));
	int lx = cos(ang), ly = sin(ang);

	auto cand = GetForkCandidates();
	int i, m, dx, dy, d, n = 0;
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER] && n < DND_FORKED_MAXCAND; ++i) {
		m = UsedMonsterTIDs[i];
		if(m == target || !IsActorAlive(m) || !IsSpellEnemy(m))
			continue;
		dx = GetActorX(m) - tx;
		dy = GetActorY(m) - ty;
		d = VectorLength(dx, dy);
		if(d > DND_FORKED_REACH || FixedMul(dx, lx) + FixedMul(dy, ly) < FixedMul(d, DND_FORKED_CONE) || !CheckSight(target, m, CSF_NOBLOCKALL))
			continue;
		cand.tid[n] = m;
		cand.dist[n] = d;
		++n;
	}

	int a = GetActorAngle(ptid);
	SendBeamToEnemy("DnD Forked Beam FX", GetActorX(ptid) + FixedMul(DND_BEAM_HANDS, cos(a)), GetActorY(ptid) + FixedMul(DND_BEAM_HANDS, sin(a)),
		GetActorZ(ptid) + GetActorViewHeight(ptid) - DND_BEAM_DROP, target);

	// Nearest first: each pass takes the closest candidate left.
	int f, best;
	for(f = 0; f < forks; ++f) {
		best = -1;
		for(i = 0; i < n; ++i)
			if(cand.tid[i] && (best == -1 || cand.dist[i] < cand.dist[best]))
				best = i;
		if(best == -1)
			break;
		SendBeamToEnemy("DnD Forked Beam FX", tx, ty, tz, cand.tid[best]);
		DealSpellBeamHit(pnum, SPL_FORKEDLIGHTNING, cand.tid[best], 100);
		PlaySound(cand.tid[best], "ForkedLightning/Hit", CHAN_7);
		cand.tid[best] = 0;
	}

	DealSpellBeamHit(pnum, SPL_FORKEDLIGHTNING, target, 100);
	PlaySound(target, "ForkedLightning/Hit", CHAN_7);
}

// Client side: a forking beam between two points and its hit at the far end. Each argument is a packed pair.
Script "DnD Forked Beam FX" (int from_xy, int to_xy, int zz) CLIENTSIDE {
	int x1 = to_xy << 16, y1 = to_xy & 0xFFFF0000, z1 = zz & 0xFFFF0000;
	Spawn("Spell_ForkedLightning_Hit", x1, y1, z1);
	SpawnStretchedBeam("Spell_ForkedLightning_Beam", DND_FORKBEAM_LEN, from_xy << 16, from_xy & 0xFFFF0000, zz << 16, x1, y1, z1);
}

// ============================ Crackling Lance ============================

#define DND_CLANCE_LEN 64.0             // the lance model's length; it is stretched to the beam's
#define DND_CLANCE_STILL 1.0            // speed, units a tic, that counts as moving
#define DND_CLANCE_TELEPORT 48          // a one tic jump this many units past the caster's speed is a teleport
#define DND_CLANCE_FANCUT 33            // branching lost per intensity stack, percent
#define DND_CLANCE_LENGTH 10            // more length per stack, percent
#define DND_CLANCE_MORE 35              // more damage per stack, percent
#define DND_CLANCE_R5_WIDTH 120         // beam width at rank 5, percent
#define DND_CLANCE_STAGES 4             // looks: none, thin laser, tangled, full laser, and the rank 10 stack's bigger one
#define DND_CLANCE_STAGESTEP 0.05       // the shader reads the stage and the spread from the upright scale; keep in step with it
#define DND_CLANCE_SPREADSTEP 0.04
#define DND_CLANCE_MAXSPREAD 0.6        // the most spread over length the model's planes can show
#define DND_CLANCE_GLOWGAP 8.0          // world units between the laser's glows at the first stage, +6 a stage as they grow
#define DND_CLANCE_GLOWSTART 20.0       // the glows begin just past the hands
#define DND_CLANCE_NEAR 72.0            // and stay thin this far, so the laser does not fill the caster's view
#define DND_CLANCE_GLOWNEAR 64.0        // a glow's alpha is the root of this over its distance from the hands
#define DND_CLANCE_GLOWFAR 0.3          // and never fainter than this
#define DND_CLANCE_HITPACK 6            // hit enemies told to clients per message, two to an argument
#define DND_CLANCE_GUSH 8               // glows sprayed out of the hands per release
#define DND_CLANCE_LIFT 8.0             // the lance leaves this far above a caster's other beams, so its laser clears the floor
#define DND_CLANCE_SELFALPHA 0.5        // how strongly the caster sees their own lance; everyone else sees it in full

typedef struct {
	int stacks[MAXPLAYERS];
	bool watching[MAXPLAYERS];
} lance_T;

lance_T module& GetLanceData() {
	static lance_T s;
	return s;
}

// Client side: up to six hit enemies, each a monster id + 1 in half an argument: a crackle and the hit sound on each.
Script "DnD Crackling Lance Hits" (int ab, int cd, int ef) CLIENTSIDE {
	int ids[3] = { ab, cd, ef };
	for(int i = 0; i < 6; ++i) {
		int id = (ids[i / 2] >> ((i & 1) * 16)) & 0xFFFF;
		if(id) {
			SpawnBodyCrackle(DND_MONSTERTID_BEGIN + id - 1, "Spell_CracklingLance_Hit");
			PlaySound(DND_MONSTERTID_BEGIN + id - 1, "CracklingLance/Hit", CHAN_7);
		}
	}
}

// The beam leaves the hands along the aim, its full width there. With no intensity it widens to twice that at the far
// end; each stack cuts that widening by a third, down to a plain rectangle. Area gear scales its length and width.
void CastCracklingLance(int pnum) {
	int ptid = pnum + P_TIDSTART;
	auto ld = GetLanceData();
	int stacks = ld.stacks[pnum];
	int fan = Max(0, 100 - stacks * DND_CLANCE_FANCUT);
	int area = ScaleSpellArea(pnum, SPL_CRACKLINGLANCE, 1.0);
	int len = FixedMul((GetSpellValue(pnum, SPL_CRACKLINGLANCE, SPELLVAL_RADIUS) * (100 + stacks * DND_CLANCE_LENGTH) / 100) << 16, area);
	int width = GetSpellValue(pnum, SPL_CRACKLINGLANCE, SPELLVAL_AMOUNT2);
	if(SpellThresholdMet(pnum, SPL_CRACKLINGLANCE, DND_SPELL_THRESH_LOW))
		width = width * DND_CLANCE_R5_WIDTH / 100;
	int half = FixedMul(width << 16, area) / 2;

	int a = GetActorAngle(ptid), pitch = GetActorPitch(ptid);
	int x0 = GetActorX(ptid) + FixedMul(DND_BEAM_HANDS, cos(a)), y0 = GetActorY(ptid) + FixedMul(DND_BEAM_HANDS, sin(a));
	int z0 = GetActorZ(ptid) + GetActorViewHeight(ptid) - DND_BEAM_DROP + DND_CLANCE_LIFT;
	int dx = FixedMul(cos(pitch), cos(a)), dy = FixedMul(cos(pitch), sin(a)), dz = -sin(pitch);
	int most = GetSpellValue(pnum, SPL_CRACKLINGLANCE, SPELLVAL_AMOUNT);
	if(SpellThresholdMet(pnum, SPL_CRACKLINGLANCE, DND_SPELL_THRESH_HIGH))
		++most;

	// The crackle fades into the laser as intensity builds: all crackle with none, all laser at the most it can hold.
	int mix = FixedDiv(Min(stacks, most) << 16, Max(1, most) << 16);
	if(mix < 1.0)
		PlaySound(ptid, "CracklingLance/Cast", CHAN_7, 1.0 - mix);
	if(mix > 0)
		PlaySound(ptid, "CracklingLance/Laser", CHAN_5, mix);
	ACS_NamedExecuteAlways("DnD Crackling Lance FX", 0, pnum | (Min(stacks, DND_CLANCE_STAGES) << 8) | ((len >> 16) << 16),
		(a & 0xFFFF) | (pitch << 16), half >> 15);

	int i, m, rad, rx, ry, rz, along, hw, n = 0;
	int hits[3] = { 0, 0, 0 };
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(!IsActorAlive(m) || !IsSpellEnemy(m))
			continue;
		rad = GetActorProperty(m, APROP_RADIUS);
		rx = GetActorX(m) - x0;
		ry = GetActorY(m) - y0;
		rz = GetActorZ(m) + GetActorProperty(m, APROP_HEIGHT) / 2 - z0;
		along = FixedMul(rx, dx) + FixedMul(ry, dy) + FixedMul(rz, dz);
		if(along < -rad || along > len + rad)
			continue;
		hw = half + FixedMul(half, FixedMul(fan * 1.0 / 100, FixedDiv(Clamp_Between(along, 0, len), len)));
		if(VectorLength(VectorLength(rx - FixedMul(along, dx), ry - FixedMul(along, dy)), rz - FixedMul(along, dz)) > hw + rad)
			continue;
		if(!CheckSight(ptid, m, CSF_NOBLOCKALL))
			continue;
		DealSpellBeamHit(pnum, SPL_CRACKLINGLANCE, m, 100 + stacks * DND_CLANCE_MORE, true);
		// Every hit enemy gets its crackle drawn by the clients, six to a message rather than one each.
		hits[n / 2] |= (m - DND_MONSTERTID_BEGIN + 1) << ((n & 1) * 16);
		if(++n == DND_CLANCE_HITPACK) {
			ACS_NamedExecuteAlways("DnD Crackling Lance Hits", 0, hits[0], hits[1], hits[2]);
			hits[0] = hits[1] = hits[2] = n = 0;
		}
	}
	if(n)
		ACS_NamedExecuteAlways("DnD Crackling Lance Hits", 0, hits[0], hits[1], hits[2]);

	ld.stacks[pnum] = Min(stacks + 1, most);
	SetActorInventory(ptid, "P_LanceStacks", ld.stacks[pnum]);
	if(!ld.watching[pnum]) {
		ld.watching[pnum] = true;
		ACS_NamedExecuteAlways("DnD Crackling Lance Watch", 0, pnum);
	}
}

// Intensity holds while the caster keeps still, cast or not: moving loses one per DURATION of time spent moving, and a
// teleport loses one at once.
Script "DnD Crackling Lance Watch" (int pnum) {
	int ptid = pnum + P_TIDSTART;
	auto ld = GetLanceData();
	int x = GetActorX(ptid) >> 16, y = GetActorY(ptid) >> 16, moved = 0, prev = 0, speed, jx, jy, reach;
	while(ld.stacks[pnum] && PlayerInGame(pnum) && IsActorAlive(ptid)) {
		speed = VectorLength(GetActorVelX(ptid), GetActorVelY(ptid));
		// Whole units, the box first: a teleport across the map would overflow a fixed point length.
		jx = (GetActorX(ptid) >> 16) - x;
		jy = (GetActorY(ptid) >> 16) - y;
		x += jx;
		y += jy;
		reach = (Max(speed, prev) >> 16) + DND_CLANCE_TELEPORT;
		if(jx > reach || jx < -reach || jy > reach || jy < -reach || jx * jx + jy * jy > reach * reach)
			SetActorInventory(ptid, "P_LanceStacks", --ld.stacks[pnum]);
		else if(speed > DND_CLANCE_STILL && ++moved >= GetSpellDurationTics(pnum, SPL_CRACKLINGLANCE)) {
			moved = 0;
			SetActorInventory(ptid, "P_LanceStacks", --ld.stacks[pnum]);
		}
		prev = speed;
		Delay(const:1);
	}
	ld.stacks[pnum] = 0;
	ld.watching[pnum] = false;
	SetActorInventory(ptid, "P_LanceStacks", 0);
}

// Client side: the lance along the aim. Packed is pnum | stage << 8 | length << 16; aim is angle | pitch << 16; spread is
// the half width the no-intensity branches reach at the far end.
Script "DnD Crackling Lance FX" (int packed, int aim, int spread) CLIENTSIDE {
	int ptid = (packed & 0xFF) + P_TIDSTART, stage = (packed >> 8) & 0xFF, len = (packed >> 16) << 16;
	int a = aim & 0xFFFF, pitch = aim >> 16;
	int k = FixedDiv(len, DND_CLANCE_LEN);
	int seen = (packed & 0xFF) == ConsolePlayerNumber() ? DND_CLANCE_SELFALPHA : 1.0;
	int tid = ScratchTID();
	if(!tid || !Spawn("Spell_CracklingLance_Beam", GetActorX(ptid) + FixedMul(DND_BEAM_HANDS, cos(a)), GetActorY(ptid) + FixedMul(DND_BEAM_HANDS, sin(a)),
		GetActorZ(ptid) + GetActorViewHeight(ptid) - DND_BEAM_DROP + DND_CLANCE_LIFT, tid))
		Terminate;
	SetActorProperty(tid, APROP_ALPHA, seen);

	// The model pitches from its velocity; its upright axis stretched past the length's scale carries the stage and spread to the shader.
	int x0 = GetActorX(tid), y0 = GetActorY(tid), z0 = GetActorZ(tid);
	int dx = FixedMul(cos(pitch), cos(a)), dy = FixedMul(cos(pitch), sin(a)), dz = -sin(pitch);
	SetActorAngle(tid, a);
	SetActorVelocity(tid, dx / 100, dy / 100, dz / 100, false, false);
	SetActorProperty(tid, APROP_SCALEX, k);
	SetActorProperty(tid, APROP_SCALEY, FixedMul(k, 1.0 + stage * DND_CLANCE_STAGESTEP +
		FixedMul(Min(FixedDiv(spread << 16, len), DND_CLANCE_MAXSPREAD), DND_CLANCE_SPREADSTEP)));
	Thing_ChangeTID(tid, 0);

	// The laser forms from the first stack: soft glows strung along the aim, thicker each stage, fading gently with distance.
	int d, g;
	if(stage) {
		str glow = StrParam(s:"Spell_CracklingLance_Glow", d:stage);
		int gap = DND_CLANCE_GLOWGAP + (stage - 1) * 6.0;
		for(d = DND_CLANCE_GLOWSTART; d < len; d += d < DND_CLANCE_NEAR ? DND_CLANCE_GLOWGAP : gap) {
			g = ScratchTID();
			if(!g || !Spawn(d < DND_CLANCE_NEAR ? "Spell_CracklingLance_GlowNear" : glow, x0 + FixedMul(dx, d), y0 + FixedMul(dy, d), z0 + FixedMul(dz, d), g))
				break;
			SetActorProperty(g, APROP_ALPHA, FixedMul(seen, Clamp_Between(FixedSqrt(FixedDiv(DND_CLANCE_GLOWNEAR, Max(d, 1.0))), DND_CLANCE_GLOWFAR, 0.6)));
			Thing_ChangeTID(g, 0);
		}
	}

	// The burst: energy sprayed out of the hands in a cone along the aim.
	int i, sp;
	for(i = 0; i < DND_CLANCE_GUSH; ++i) {
		g = ScratchTID();
		if(!g || !Spawn("Spell_CracklingLance_Gush", x0, y0, z0, g))
			break;
		SetActorProperty(g, APROP_ALPHA, FixedMul(seen, GetActorProperty(g, APROP_ALPHA)));
		sp = random(4.0, 9.0);
		SetActorVelocity(g, FixedMul(dx, sp) + random(-1.6, 1.6), FixedMul(dy, sp) + random(-1.6, 1.6), FixedMul(dz, sp) + random(-1.6, 1.6), false, false);
		Thing_ChangeTID(g, 0);
	}
}

// ============================ Energy Blade ============================

#define DND_ENERGYBLADE_R5_MIN 5        // the minimum share at rank 5, percent of energy shield
#define DND_ENERGYBLADE_R10_LESS 40     // percent less energy shield at rank 10

int GetEnergyBladeLess(int pnum) {
	return SpellThresholdMet(pnum, SPL_ENERGYBLADE, DND_SPELL_THRESH_HIGH) ? DND_ENERGYBLADE_R10_LESS : GetSpellValue(pnum, SPL_ENERGYBLADE, SPELLVAL_AMOUNT);
}

// A real time toggle: the cast draws it, a second cast puts it away. The once a second aura pass does the upkeep.
void CastEnergyBlade(int pnum) {
	SetSpellRunning(pnum, SPL_ENERGYBLADE, true);
	PlaySound(pnum + P_TIDSTART, "EnergyBlade/Cast", CHAN_7);
}

// A melee weapon's added lightning while the blade is out, rolled between the spell's two shares of the current maximum energy shield.
int GetEnergyBladeDamage(int pnum, int wepid) {
	if(!IsSpellRunning(pnum, SPL_ENERGYBLADE) || wepid < 0 || !IsMeleeWeapon(wepid))
		return 0;
	int cap = GetPlayerEnergyShieldCap(pnum);
	int lo = SpellThresholdMet(pnum, SPL_ENERGYBLADE, DND_SPELL_THRESH_LOW) ? DND_ENERGYBLADE_R5_MIN : GetSpellValue(pnum, SPL_ENERGYBLADE, SPELLVAL_DAMAGE2);
	return random(cap * lo, cap * GetSpellValue(pnum, SPL_ENERGYBLADE, SPELLVAL_DAMAGE)) / 100;
}

// ============================ Blink ============================

#define DND_BLINK_CHEST 32.0            // the streak runs at this height over the caster's feet
#define DND_BLINK_SURGE_TICS 2          // how long Conduit's distance waits for a running Overcharge to take it
#define DND_BLINK_SCANSTEP 8.0          // how finely the landing spot is searched, back from the full distance
#define DND_BLINK_ONFLOOR (-2147483647 - 1)	// the engine's ONFLOORZ: a spawn at this z stands on the floor

void CastBlink(int pnum) {
	ACS_NamedExecuteAlways("DnD Blink Dash", 0, pnum);
}

// The farthest distance up to d along a where the caster fits among actors, the probe standing on that spot's floor,
// so the dash never ends inside a monster or a decoration. Walls and ledges are left to the dash itself.
int BlinkClearDistance(int ptid, int a, int d) {
	int probe = ScratchTID(), x0 = GetActorX(ptid), y0 = GetActorY(ptid);
	if(!probe)
		return d;
	for(; d > 0; d -= DND_BLINK_SCANSTEP) {
		if(Spawn("DnD_BlinkProbe", x0 + FixedMul(d, cos(a)), y0 + FixedMul(d, sin(a)), DND_BLINK_ONFLOOR, probe, 0)) {
			Thing_Remove(probe);
			return d;
		}
	}
	return 0;
}

// A player-sized solid probe cannot spawn where the caster stands while anything solid still overlaps them.
bool BlinkOverlapsActor(int ptid) {
	int probe = ScratchTID();
	if(!probe)
		return false;
	if(!Spawn("DnD_BlinkProbe", GetActorX(ptid), GetActorY(ptid), GetActorZ(ptid), probe, 0))
		return true;
	Thing_Remove(probe);
	return false;
}

// Static Wake: Shock every enemy the caster's body swept through, from (x0, y0) to where they stand now.
void ShockBlinkPath(int pnum, int x0, int y0, int z0) {
	int ptid = pnum + P_TIDSTART;
	int dx = GetActorX(ptid) - x0, dy = GetActorY(ptid) - y0, len = VectorLength(dx, dy);
	if(len < 1.0)
		return;
	int ux = FixedDiv(dx, len), uy = FixedDiv(dy, len), body = GetActorProperty(ptid, APROP_RADIUS);
	int zlo = Min(z0, GetActorZ(ptid)), zhi = Max(z0, GetActorZ(ptid)) + GetActorProperty(ptid, APROP_HEIGHT);
	int i, m, rx, ry, along, reach;
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(!IsActorAlive(m) || !IsSpellEnemy(m))
			continue;
		reach = body + GetActorProperty(m, APROP_RADIUS);
		rx = GetActorX(m) - x0;
		ry = GetActorY(m) - y0;
		along = FixedMul(rx, ux) + FixedMul(ry, uy);
		if(along < 0 || along > len + reach || abs(FixedMul(rx, uy) - FixedMul(ry, ux)) > reach)
			continue;
		if(GetActorZ(m) > zhi || GetActorZ(m) + GetActorProperty(m, APROP_HEIGHT) < zlo)
			continue;
		ApplySpellShock(pnum, m);
	}
}

// The whole distance as one tic of velocity: the engine moves the caster in collision-checked steps, so walls
// and ledges taller than a step stop the dash as they stop walking. Actors do not: the caster is unsolid for it.
Script "DnD Blink Dash" (int pnum) {
	int ptid = pnum + P_TIDSTART;
	int a = GetActorAngle(ptid), d = BlinkClearDistance(ptid, a, GetSpellValue(pnum, SPL_BLINK, SPELLVAL_RADIUS) << 16);
	int x0 = GetActorX(ptid), y0 = GetActorY(ptid), z0 = GetActorZ(ptid);
	int vx = GetActorVelX(ptid), vy = GetActorVelY(ptid);

	GiveActorInventory(ptid, "AllowThruObjects", 1);
	SetActorVelocity(ptid, FixedMul(d, cos(a)), FixedMul(d, sin(a)), GetActorVelZ(ptid), false, false);
	PlaySound(ptid, "Blink/Cast", CHAN_7);
	Delay(const:1);

	SetActorVelocity(ptid, vx, vy, GetActorVelZ(ptid), false, false);
	ACS_NamedExecuteAlways("DnD Blink FX", 0, PackMapPair(x0, y0), PackMapPair(GetActorX(ptid), GetActorY(ptid)),
		PackMapPair(z0 + DND_BLINK_CHEST, GetActorZ(ptid) + DND_BLINK_CHEST));

	if(SpellThresholdMet(pnum, SPL_BLINK, DND_SPELL_THRESH_LOW))
		ShockBlinkPath(pnum, x0, y0, z0);

	// Conduit: the distance covered, for a running Overcharge; cleared after so none waits for a later cast.
	if(SpellThresholdMet(pnum, SPL_BLINK, DND_SPELL_THRESH_HIGH)) {
		SetActorInventory(ptid, "DnD_OverchargeSurge", VectorLength(GetActorX(ptid) - x0, GetActorY(ptid) - y0) >> 16);
		Delay(const:DND_BLINK_SURGE_TICS);
		SetActorInventory(ptid, "DnD_OverchargeSurge", 0);
	}

	// In case a monster stepped onto the landing spot: a player turned solid inside a monster cannot move until it does.
	while(IsActorAlive(ptid) && BlinkOverlapsActor(ptid))
		Delay(const:1);
	if(IsActorAlive(ptid))
		GiveActorInventory(ptid, "TakeThruObjects", 1);
}

// Client side: a streak of lightning along the dash and a burst at each end. Each argument is a packed pair.
Script "DnD Blink FX" (int from_xy, int to_xy, int zz) CLIENTSIDE {
	int x0 = from_xy << 16, y0 = from_xy & 0xFFFF0000, z0 = zz << 16;
	int x1 = to_xy << 16, y1 = to_xy & 0xFFFF0000, z1 = zz & 0xFFFF0000;
	Spawn("Spell_Blink_Burst", x0, y0, z0);
	Spawn("Spell_Blink_Burst", x1, y1, z1);
	SpawnStretchedBeam("Spell_Blink_Beam", DND_FORKBEAM_LEN, x0, y0, z0, x1, y1, z1);
}

// ============================ Overcharge ============================

#define DND_OVERCHARGE_SPEEDCAP 24.0    	// speed, units a tic, counted toward charge at most
#define DND_OVERCHARGE_PERCHARGE 16.0   	// units moved for one charge
#define DND_OVERCHARGE_R5_RATE 125      	// percent charge generation at rank 5
#define DND_OVERCHARGE_R10_TICS (2 * TICRATE)
#define DND_OVERCHARGE_FXEVERY 3        	// tics between crackles on the caster while charging
#define DND_OVERCHARGE_HELDFX 35        	// and while the charge is held
#define DND_OVERCHARGE_STILL 1.0        	// speed, units a tic, at or under which the caster is standing still
#define DND_OVERCHARGE_GRACE TICRATE / 2	// tics standing still before the charge starts to bleed
#define DND_OVERCHARGE_DECAY 5          	// then one charge lost every this many tics

// The caster's count of tics stood still, from last tic's: back to 0 once they move.
int GetOverchargeStill(int ptid, int still) {
	return VectorLength(GetActorVelX(ptid), GetActorVelY(ptid)) > DND_OVERCHARGE_STILL ? 0 : still + 1;
}

// Whether this tic of standing still costs a charge: past the grace, every DECAY tics.
bool OverchargeBleeds(int still) {
	return still > DND_OVERCHARGE_GRACE && !((still - DND_OVERCHARGE_GRACE) % DND_OVERCHARGE_DECAY);
}

// Blink's Conduit: whole units its dash just covered, at the charge rate, taken so they count once.
int TakeOverchargeSurge(int ptid, int rate) {
	int units = CheckActorInventory(ptid, "DnD_OverchargeSurge");
	if(units)
		SetActorInventory(ptid, "DnD_OverchargeSurge", 0);
	return units * rate / 100;
}

void CastOvercharge(int pnum) {
	PlaySound(pnum + P_TIDSTART, "Overcharge/Cast", CHAN_7);
	ACS_NamedExecuteAlways("DnD Overcharge", 0, pnum);
}

// Charges for the window, faster the faster the caster moves, then holds them for the next weapon hit. Standing still
// bleeds the charge, while charging or holding. A recast keeps what is held and charges on top of it.
Script "DnD Overcharge" (int pnum) {
	int ptid = pnum + P_TIDSTART;
	int cap = GetSpellValue(pnum, SPL_OVERCHARGE, SPELLVAL_AMOUNT);
	int rate = SpellThresholdMet(pnum, SPL_OVERCHARGE, DND_SPELL_THRESH_LOW) ? DND_OVERCHARGE_R5_RATE : 100;
	int tics = GetSpellDurationTics(pnum, SPL_OVERCHARGE) +
		ScaleSpellEffectTics(pnum, SPL_OVERCHARGE, DND_OVERCHARGE_R10_TICS) * SpellThresholdMet(pnum, SPL_OVERCHARGE, DND_SPELL_THRESH_HIGH);
	int acc = FixedMul(CheckActorInventory(ptid, "DnD_OverchargeReady") << 16, DND_OVERCHARGE_PERCHARGE), charges, held, t, still = 0;
	SetActorInventory(ptid, "DnD_OverchargeReady", 0);
	ACS_NamedExecuteAlways("DnD Overcharge FX", 0, pnum, tics);

	for(t = 0; t < tics && PlayerInGame(pnum) && IsActorAlive(ptid); ++t) {
		acc += Min(VectorLength(GetActorVelX(ptid), GetActorVelY(ptid)), DND_OVERCHARGE_SPEEDCAP) * rate / 100 + (TakeOverchargeSurge(ptid, rate) << 16);
		// Kept at the cap, so a charge lost standing still shows at once rather than off a surplus.
		acc = Min(acc, cap * DND_OVERCHARGE_PERCHARGE);
		still = GetOverchargeStill(ptid, still);
		if(OverchargeBleeds(still))
			acc = Max(0, acc - DND_OVERCHARGE_PERCHARGE);
		charges = Min(cap, FixedDiv(acc, DND_OVERCHARGE_PERCHARGE) >> 16);
		if(charges != CheckActorInventory(ptid, "P_OverchargeCharges"))
			SetActorInventory(ptid, "P_OverchargeCharges", charges);
		Delay(const:1);
	}

	SetActorInventory(ptid, "DnD_OverchargeReady", CheckActorInventory(ptid, "P_OverchargeCharges"));

	// Held until a weapon hit spends it, standing still bleeds it away, and death drops it.
	for(t = 0; (held = CheckActorInventory(ptid, "DnD_OverchargeReady")) && PlayerInGame(pnum) && IsActorAlive(ptid); ++t) {
		if(t % DND_OVERCHARGE_HELDFX == 0)
			ACS_NamedExecuteAlways("DnD Overcharge FX", 0, pnum, 0);
		charges = Min(cap, held + TakeOverchargeSurge(ptid, rate) / (DND_OVERCHARGE_PERCHARGE >> 16));
		still = GetOverchargeStill(ptid, still);
		if(OverchargeBleeds(still))
			--charges;
		if(charges != held) {
			SetActorInventory(ptid, "DnD_OverchargeReady", charges);
			SetActorInventory(ptid, "P_OverchargeCharges", charges);
		}
		Delay(const:1);
	}
	if(!IsActorAlive(ptid) || !PlayerInGame(pnum)) {
		SetActorInventory(ptid, "DnD_OverchargeReady", 0);
		SetActorInventory(ptid, "P_OverchargeCharges", 0);
	}
}

// The unleash, from the weapon hit that spent the charges: their damage as one lightning hit.
Script "DnD Overcharge Release" (int pnum, int victim, int charges) {
	int ptid = pnum + P_TIDSTART;
	SetActorInventory(ptid, "P_OverchargeCharges", 0);
	if(!IsActorAlive(victim))
		Terminate;

	int dmg = PriceSpellDamage(pnum, SPL_OVERCHARGE, GetSpellValue(pnum, SPL_OVERCHARGE, SPELLVAL_DAMAGE) * charges);
	int dealt = HandleDamageDeal(ptid, victim, dmg, GetSpellDamageType(SPL_OVERCHARGE), SPL_OVERCHARGE, DND_DAMAGEFLAG_ISSPELL, 0, 0, 0, 0, true);
	if(dealt > 0)
		Thing_Damage2(victim, dealt, "SkipHandle");
	PlaySound(victim, "Overcharge/Release", CHAN_7);
	ACS_NamedExecuteAlways("DnD Overcharge Release FX", 0, GetActorX(victim) >> 16, GetActorY(victim) >> 16,
		((GetActorProperty(victim, APROP_HEIGHT) >> 17) << 16) | ((GetActorZ(victim) >> 16) & 0xFFFF));
}

// Client side: crackles round the caster, every few tics for tics, or a pair when tics is 0.
Script "DnD Overcharge FX" (int pnum, int tics) CLIENTSIDE {
	int ptid = pnum + P_TIDSTART, t = 0, i;
	do {
		for(i = 0; i < 2; ++i)
			Spawn("Spell_Overcharge_Crackle", GetActorX(ptid) + random(-20.0, 20.0), GetActorY(ptid) + random(-20.0, 20.0), GetActorZ(ptid) + random(8.0, 48.0));
		Delay(DND_OVERCHARGE_FXEVERY);
		t += DND_OVERCHARGE_FXEVERY;
	} while(t < tics && IsActorAlive(ptid));
}

// Client side: a bolt down onto the struck enemy, facing this viewer, and a burst at its middle. Packed is half height << 16 | z.
Script "DnD Overcharge Release FX" (int x, int y, int packed) CLIENTSIDE {
	int z = (packed << 16) >> 16, mid = packed >> 16;
	x <<= 16;
	y <<= 16;
	Spawn("Spell_Overcharge_Burst", x, y, (z + mid) << 16);
	int a = random(0, 1.0), me = ConsolePlayerNumber() + P_TIDSTART;
	if(me >= P_TIDSTART) {
		a = VectorAngle(GetActorX(me) - x, GetActorY(me) - y);
		x += FixedMul(DND_STATICFIELD_FXNEAR, cos(a));
		y += FixedMul(DND_STATICFIELD_FXNEAR, sin(a));
	}
	Spawn("Spell_Overcharge_Strike", x, y, z << 16, 0, a >> 8);
}

// ============================ Conductivity ============================

#define DND_CONDUCTIVITY_R5_AOE 25
#define DND_CONDUCTIVITY_R10_TICS (4 * TICRATE)

int GetConductivityRadius(int pnum) {
	return ScaleSpellArea(pnum, SPL_CONDUCTIVITY, GetSpellValue(pnum, SPL_CONDUCTIVITY, SPELLVAL_RADIUS) << 16,
		DND_CONDUCTIVITY_R5_AOE * SpellThresholdMet(pnum, SPL_CONDUCTIVITY, DND_SPELL_THRESH_LOW));
}

// Flammability's ring, in crackles. CLIENTSIDE for the same reason: the markers are client side only.
Script "DnD Conductivity Area FX" (int cx, int cy, int cz, int r) CLIENTSIDE {
	if(r <= 0) {
		SetResultValue(0);
		Terminate;
	}

	int n = Clamp_Between((FixedDiv(r, DND_FLAMMABILITY_RINGSTEP) >> 16) * 6, DND_FLAMMABILITY_RINGMIN, DND_FLAMMABILITY_RINGMAX);
	for(int i = 0; i < n; ++i) {
		int a = (i * 1.0) / n;
		SpawnForced("ConductivityAreaMarkerFX", cx + FixedMul(cos(a), r), cy + FixedMul(sin(a), r), cz + DND_FLAMMABILITY_RINGLIFT, 0, 0);
	}
	SetResultValue(0);
}

// Frostbite's lightning twin: the same two ways to find the centre, a lightning exposure and an overload lane.
Script "DnD Conductivity Cast" (int pnum) {
	int caster = pnum + P_TIDSTART;
	int cx, cy, cz;

	PlaySound(caster, "Conductivity/Cast", 6);

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

	int r = GetConductivityRadius(pnum);
	int tics = GetSpellDurationTics(pnum, SPL_CONDUCTIVITY) +
		ScaleSpellEffectTics(pnum, SPL_CONDUCTIVITY, DND_CONDUCTIVITY_R10_TICS) * SpellThresholdMet(pnum, SPL_CONDUCTIVITY, DND_SPELL_THRESH_HIGH);

	ACS_NamedExecuteWithResult("DnD Conductivity Area FX", cx, cy, cz, r);
	// Witch: more curse effect on both halves.
	int pct = ApplySpellGemMore(pnum, SPL_CONDUCTIVITY, GEMSTAT_MORE_CURSE, GetSpellValue(pnum, SPL_CONDUCTIVITY, SPELLVAL_DAMAGE));
	int chance = ApplySpellGemMore(pnum, SPL_CONDUCTIVITY, GEMSTAT_MORE_CURSE, GetSpellValue(pnum, SPL_CONDUCTIVITY, SPELLVAL_DAMAGE2));

	int i, mn;
	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		if(!IsActorAlive(i) || !IsSpellEnemy(i))
			continue;
		if(fdistance_delta(cx - GetActorX(i), cy - GetActorY(i), cz - GetActorZ(i)) > r)
			continue;

		ApplyLightningExposure(i, pct, tics);

		if(CheckActorInventory(i, "DnD_Conductive") < chance)
			SetActorInventory(i, "DnD_Conductive", chance);

		bool ticking = !!CheckActorInventory(i, "DnD_ConductiveTimer");
		if(CheckActorInventory(i, "DnD_ConductiveTimer") < tics)
			SetActorInventory(i, "DnD_ConductiveTimer", tics);

		if(!ticking)
			ACS_NamedExecuteAlways("DnD Conductivity Timer", 0, i);
	}
}

// ============================ Chain Lightning ============================

#define DND_CHAIN_RANGE 1024.0          // reach to the enemy you face
#define DND_CHAIN_HOPTICS 3             // tics between hops, so the chain can be followed
#define DND_CHAIN_MAXLOSS 75            // damage lost to chaining never passes this percent
#define DND_CHAIN_R5_LOSS 4
#define DND_CHAIN_R10_SHOCK 30

// The nearest enemy in reach of a point and in sight of it, other than the last two the chain hit, or also.
int FindChainTarget(int x, int y, int z, int reach, int last, int before, int also = 0) {
	int tid = ScratchTID();
	if(!tid || !SpawnForced("DnD_SpellAnchor", x, y, z, tid, 0))
		return 0;

	int i, m, d, best = 0, bestd = reach + 1;
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(m == last || m == before || m == also || !IsActorAlive(m) || !IsSpellEnemy(m))
			continue;
		d = VectorLength(VectorLength(GetActorX(m) - x, GetActorY(m) - y), GetActorZ(m) + GetActorProperty(m, APROP_HEIGHT) / 2 - z);
		if(d < bestd && CheckSight(tid, m, CSF_NOBLOCKALL)) {
			best = m;
			bestd = d;
		}
	}
	Thing_Remove(tid);
	return best;
}

void CastChainLightning(int pnum) {
	int target = GetSpellEnemyTarget(pnum, DND_CHAIN_RANGE);
	if(target)
		ACS_NamedExecuteAlways("DnD Chain Lightning", 0, pnum, target);
}

// One chain hit at pct of the damage: its sounds, the damage and rank 10's Shock roll.
void ChainLightningHit(int pnum, int victim, int pct, bool shocks) {
	if(random(0, 1))
		PlaySound(victim, "ChainLightning/Cast", CHAN_6);
	else
		PlaySound(victim, "ChainLightning/Beam", CHAN_6);
	PlaySound(victim, "ChainLightning/Hit", CHAN_7);
	DealSpellBeamHit(pnum, SPL_CHAINLIGHTNING, victim, pct);
	if(shocks && IsActorAlive(victim) && random(1, 100) <= DND_CHAIN_R10_SHOCK)
		ApplySpellShock(pnum, victim);
}

// The first hit comes from the caster's hands; each hop then leaves from the enemy just hit, losing damage as it goes.
// Every hop also forks a bolt that cannot chain to one more enemy, at the hop's damage.
Script "DnD Chain Lightning" (int pnum, int target) {
	int ptid = pnum + P_TIDSTART;
	int chains = GetSpellValue(pnum, SPL_CHAINLIGHTNING, SPELLVAL_AMOUNT) + GetSpellExtraProjectiles(pnum, SPL_CHAINLIGHTNING);
	int loss = SpellThresholdMet(pnum, SPL_CHAINLIGHTNING, DND_SPELL_THRESH_LOW) ? DND_CHAIN_R5_LOSS : GetSpellValue(pnum, SPL_CHAINLIGHTNING, SPELLVAL_DAMAGE2);
	int reach = GetSpellValue(pnum, SPL_CHAINLIGHTNING, SPELLVAL_RADIUS) << 16;
	bool shocks = SpellThresholdMet(pnum, SPL_CHAINLIGHTNING, DND_SPELL_THRESH_HIGH);

	int a = GetActorAngle(ptid);
	int x = GetActorX(ptid) + FixedMul(DND_BEAM_HANDS, cos(a)), y = GetActorY(ptid) + FixedMul(DND_BEAM_HANDS, sin(a));
	int z = GetActorZ(ptid) + GetActorViewHeight(ptid) - DND_BEAM_DROP;
	int victim = target, last = 0, before = 0, side = 0, pct;

	PlaySound(ptid, "ChainLightning/Cast", CHAN_7);
	PlaySound(ptid, "ChainLightning/CastLayer", CHAN_5);

	for(int c = 0; c <= chains && PlayerInGame(pnum); ++c) {
		if(c) {
			victim = FindChainTarget(x, y, z, reach, last, before);
			if(!victim)
				break;
			side = FindChainTarget(x, y, z, reach, last, before, victim);
		}

		SendBeamToEnemy("DnD Chain Beam FX", x, y, z, victim);
		if(side)
			SendBeamToEnemy("DnD Chain Beam FX", x, y, z, side);
		x = GetActorX(victim);
		y = GetActorY(victim);
		z = GetActorZ(victim) + GetActorProperty(victim, APROP_HEIGHT) / 2;

		pct = 100 - Min(DND_CHAIN_MAXLOSS, c * loss);
		ChainLightningHit(pnum, victim, pct, shocks);
		if(side)
			ChainLightningHit(pnum, side, pct, shocks);

		before = last;
		last = victim;
		Delay(DND_CHAIN_HOPTICS);
	}
}

// Client side: one hop of the chain and its hit at the far end. Each argument is a packed pair.
Script "DnD Chain Beam FX" (int from_xy, int to_xy, int zz) CLIENTSIDE {
	int x1 = to_xy << 16, y1 = to_xy & 0xFFFF0000, z1 = zz & 0xFFFF0000;
	Spawn("Spell_ChainLightning_Hit", x1, y1, z1);
	SpawnStretchedBeam("Spell_ChainLightning_Beam", DND_FORKBEAM_LEN, from_xy << 16, from_xy & 0xFFFF0000, zz << 16, x1, y1, z1);
}

// ============================ Galvanic Surge ============================

#define DND_GALVANICSURGE_R10_HASTE 25
#define DND_GALVANICSURGE_R10_TICS (5 * TICRATE)
#define DND_GALVANICSURGE_FXCOUNT 8

// Every ailment on the player gone: DoTs through RemoveAilments, chill and freeze through the buff ticker, overload here.
void CurePlayerAilments(int pnum) {
	int ptid = pnum + P_TIDSTART;
	GiveActorInventory(ptid, "RemoveAilments", 1);
	GiveActorInventory(ptid, "RemoveChillFreeze", 1);
	if(HasPlayerBuff(pnum, BTI_OVERLOAD))
		RemoveBuffWithTableIndex(pnum, BTI_OVERLOAD);
	SetActorInventory(ptid, "DnD_OverloadTimer", 0);
}

// A charge back on the last spell put on cooldown; rank 5 cleanses, rank 10 hastes.
void CastGalvanicSurge(int pnum) {
	int ptid = pnum + P_TIDSTART;
	int s = CheckActorInventory(ptid, "DnD_LastCooldownSpell") - 1;
	if(s >= 0 && s < MAX_SPELL_IDS)
		RecoverSpellCharge(pnum, s);

	if(SpellThresholdMet(pnum, SPL_GALVANICSURGE, DND_SPELL_THRESH_LOW))
		CurePlayerAilments(pnum);

	if(SpellThresholdMet(pnum, SPL_GALVANICSURGE, DND_SPELL_THRESH_HIGH))
		HandlePlayerBuffAssignment(pnum, ptid, BTI_SPELL_GALVANICSURGE_HASTE, 0, 0,
			ScaleSpellEffectTics(pnum, SPL_GALVANICSURGE, DND_GALVANICSURGE_R10_TICS), DND_GALVANICSURGE_R10_HASTE);

	PlaySound(ptid, "GalvanicSurge/Cast", CHAN_6);
	ACS_NamedExecuteAlways("DnD Galvanic Surge FX", 0, pnum);
}

// Client side: a flash at the caster's chest and two rings of crackles climbing round them.
Script "DnD Galvanic Surge FX" (int pnum) CLIENTSIDE {
	int ptid = pnum + P_TIDSTART, i, a, w;
	Spawn("Spell_GalvanicSurge_Flash", GetActorX(ptid), GetActorY(ptid), GetActorZ(ptid) + 32.0);
	for(w = 0; w < 2; ++w) {
		for(i = 0; i < DND_GALVANICSURGE_FXCOUNT; ++i) {
			a = (i * 1.0 + w * 0.5) / DND_GALVANICSURGE_FXCOUNT;
			Spawn("Spell_Overcharge_Crackle", GetActorX(ptid) + 22 * cos(a), GetActorY(ptid) + 22 * sin(a), GetActorZ(ptid) + 12.0 + w * 24.0 + random(0, 12.0));
		}
		Delay(const:3);
	}
}

// ============================ Surgecast ============================

#define DND_SURGECAST_TICS (5 * TICRATE)
#define DND_SURGECAST_R5_REFUND 20
#define DND_SURGECAST_FXEVERY 4

int[] module& GetSurgecastFXTickets() {
	static int tickets[MAXPLAYERS];
	return tickets;
}

// Arms Surgecast at once, so a cast right behind it finds it; a recast restarts the window rather than banking a second one.
void CastSurgecast(int pnum) {
	int ptid = pnum + P_TIDSTART;
	int tics = ScaleSpellEffectTics(pnum, SPL_SURGECAST, DND_SURGECAST_TICS);
	GiveActorInventory(ptid, "DnD_SurgecastTicket", 1);
	SetActorInventory(ptid, "DnD_SurgecastReady", 1);
	SetActorInventory(ptid, "P_SurgecastTime", (tics + TICRATE - 1) / TICRATE);
	PlaySound(ptid, "Surgecast/Cast", CHAN_6);
	ACS_NamedExecuteAlways("DnD Surgecast FX", 0, pnum, tics);
	ACS_NamedExecuteAlways("DnD Surgecast", 0, pnum, CheckActorInventory(ptid, "DnD_SurgecastTicket"), tics);
}

// Counts the window down; a spend or a recast takes it over.
Script "DnD Surgecast" (int pnum, int ticket, int tics) {
	int ptid = pnum + P_TIDSTART;
	for(int t = tics; t > 0 && ticket == CheckActorInventory(ptid, "DnD_SurgecastTicket") && CheckActorInventory(ptid, "DnD_SurgecastReady") &&
		PlayerInGame(pnum) && IsActorAlive(ptid); --t) {
		int s = (t + TICRATE - 1) / TICRATE;
		if(s != CheckActorInventory(ptid, "P_SurgecastTime"))
			SetActorInventory(ptid, "P_SurgecastTime", s);
		Delay(const:1);
	}

	// Lapsed or died; a spend or a recast has already taken it over otherwise.
	if(ticket == CheckActorInventory(ptid, "DnD_SurgecastTicket") && CheckActorInventory(ptid, "DnD_SurgecastReady")) {
		SetActorInventory(ptid, "DnD_SurgecastReady", 0);
		SetActorInventory(ptid, "P_SurgecastTime", 0);
	}
}

// Spends the armed Surgecast on this cast and rolls its refund; returns the cast time left, none.
int SpendSurgecast(int pnum, int spell) {
	int ptid = pnum + P_TIDSTART;
	SetActorInventory(ptid, "DnD_SurgecastReady", 0);
	SetActorInventory(ptid, "P_SurgecastTime", 0);

	// Paid in life, there is no mana to hand back.
	int chance = GetSpellValue(pnum, SPL_SURGECAST, SPELLVAL_DAMAGE) + DND_SURGECAST_R5_REFUND * SpellThresholdMet(pnum, SPL_SURGECAST, DND_SPELL_THRESH_LOW);
	if(!GetSpellGemMod(pnum, spell, GEMSTAT_LIFECOST) && random(1, 100) <= chance)
		GiveMana(pnum, GetSpellValue(pnum, spell, SPELLVAL_COST) >> 16);

	ACS_NamedExecuteAlways("DnD Surgecast Spend FX", 0, pnum);
	return 0;
}

// Client side: a burst on the cast, then crackles at the caster's hands until the window ends or a spend or recast takes over.
Script "DnD Surgecast FX" (int pnum, int tics) CLIENTSIDE {
	int[] module& tickets = GetSurgecastFXTickets();
	int ticket = ++tickets[pnum], ptid = pnum + P_TIDSTART, a, side;
	Spawn("Spell_Surgecast_Burst", GetActorX(ptid), GetActorY(ptid), GetActorZ(ptid) + 32.0);
	for(int t = 0; t < tics && ticket == tickets[pnum] && IsActorAlive(ptid); t += DND_SURGECAST_FXEVERY) {
		a = GetActorAngle(ptid);
		side = (t / DND_SURGECAST_FXEVERY) & 1 ? 10.0 : -10.0;
		Spawn("Spell_Surgecast_Crackle", GetActorX(ptid) + 14 * cos(a) + FixedMul(side, cos(a + 0.25)) + random(-3.0, 3.0),
			GetActorY(ptid) + 14 * sin(a) + FixedMul(side, sin(a + 0.25)) + random(-3.0, 3.0), GetActorZ(ptid) + random(26.0, 34.0));
		Delay(const:DND_SURGECAST_FXEVERY);
	}
}

// Client side: the spend stops the crackles and flashes ahead of the caster's hands, far enough out to clear their weapon.
Script "DnD Surgecast Spend FX" (int pnum) CLIENTSIDE {
	int[] module& tickets = GetSurgecastFXTickets();
	++tickets[pnum];
	int ptid = pnum + P_TIDSTART, a = GetActorAngle(ptid);
	Spawn("Spell_Surgecast_Release", GetActorX(ptid) + 32 * cos(a), GetActorY(ptid) + 32 * sin(a), GetActorZ(ptid) + 30.0);
}

// ============================ Targeted spots and area sweeps ============================

// The sector floor under a point; a probe's own floor can be the top of a monster standing there.
int GetFloorZAt(int x, int y) {
	return GetSectorFloorZ(0, x >> 16, y >> 16);
}

int GetCeilingZAt(int x, int y) {
	return GetSectorCeilingZ(0, x >> 16, y >> 16);
}

// A probe for sight checks at a point, on a scratch tid the caller removes; 0 if it would not spawn.
int SpawnSpellProbe(int x, int y, int z) {
	int tid = ScratchTID();
	if(!tid || !SpawnForced("DnD_SpellAnchor", x, y, z, tid))
		return 0;
	return tid;
}

// A targeted spell's spot: the enemy it aims at (returned, its feet in the aim slot), else 0 with the ruler fired; -1 if
// that failed. The caller waits a few tics on ReadSpellAim for the ruler, as functions cannot.
int BeginSpellSpot(int pnum, int range) {
	auto aim = GetSpellAim();
	int target = GetSpellEnemyTarget(pnum, range);
	if(target) {
		aim.x[pnum] = GetActorX(target);
		aim.y[pnum] = GetActorY(target);
		aim.z[pnum] = GetActorZ(target);
		return target;
	}
	return TraceSpellAim(pnum, range) ? 0 : -1;
}

#define DND_SPELLAREA_BODYPAD 128      // the box test's allowance for a body, before the real radius is read

// Whether a living enemy's body is within r of a point across the floor, and the probe there can see it.
// Whole units, and the box test before any call: the sweep runs over every monster the map ever spawned.
bool InSpellArea(int probe, int m, int x, int y, int r) {
	int reach = (r >> 16) + DND_SPELLAREA_BODYPAD;
	int dx = (GetActorX(m) >> 16) - (x >> 16), dy = (GetActorY(m) >> 16) - (y >> 16);
	if(dx > reach || dx < -reach || dy > reach || dy < -reach || !probe || !IsActorAlive(m) || !IsSpellEnemy(m))
		return false;
	reach = (r + GetActorProperty(m, APROP_RADIUS)) >> 16;
	return dx * dx + dy * dy <= reach * reach && CheckSight(probe, m, CSF_NOBLOCKALL);
}

// Whether a body's height comes within r of z, for an area that is a ball rather than a column.
bool InSpellHeight(int m, int z, int r) {
	int mz = GetActorZ(m);
	return mz <= z + r && mz + GetActorProperty(m, APROP_HEIGHT) >= z - r;
}

// ============================ Storm Brand ============================

#define DND_STORMBRAND_RANGE 1024.0     // how far off the aim it can be placed
#define DND_STORMBRAND_REACH 96.0       // how close an enemy must be to the brand for it to attach
#define DND_STORMBRAND_PERIOD 17.5      // tics between activations without haste: half a second
#define DND_STORMBRAND_SEEK 5           // tics between looks for a host while it waits
#define DND_STORMBRAND_LIFT 20.0        // the globe floats this far over its host's head
#define DND_STORMBRAND_HOVER 24.0       // and this far over the floor while it waits, under eye height so its circle shows
#define DND_STORMBRAND_SLOTS 8          // brands a caster can have out at once, as far as their clientside FX go
#define DND_STORMBRAND_MAXCAND 64
#define DND_STORMBRAND_SPREAD 28.0      // brands sharing a host sit at least this far from its middle; a ring is 22 across
#define DND_STORMBRAND_GLIDE 4          // a brand closes 1/this of the way to a new seat each tic

int[] module& GetStormBrandSerials() {
	static int s[MAXPLAYERS];
	return s;
}

int[] module& GetStormBrandFXTickets() {
	static int t[MAXPLAYERS * DND_STORMBRAND_SLOTS];
	return t;
}

// Server side: the host each brand slot rides, 0 while it waits or is gone.
int[] module& GetStormBrandHosts() {
	static int h[MAXPLAYERS * DND_STORMBRAND_SLOTS];
	return h;
}

// Each slot's offset from its host's middle, packed whole units; the server sets them and tells every client.
int[] module& GetStormBrandSeats() {
	static int s[MAXPLAYERS * DND_STORMBRAND_SLOTS];
	return s;
}

// Spreads the brands riding a host evenly round it, side by side as this caster sees them; a lone one sits in the middle.
void SeatStormBrands(int host, int pnum) {
	int[] module& hosts = GetStormBrandHosts();
	int[] module& seats = GetStormBrandSeats();
	int i, a, d = 0, k = 0, n = 0, ptid = pnum + P_TIDSTART;
	for(i = 0; i < MAXPLAYERS * DND_STORMBRAND_SLOTS; ++i) {
		if(hosts[i] == host)
			++n;
	}
	if(n > 1)
		d = Max(GetActorProperty(host, APROP_RADIUS), DND_STORMBRAND_SPREAD);
	int base = VectorAngle(GetActorX(host) - GetActorX(ptid), GetActorY(host) - GetActorY(ptid)) + 0.25;
	for(i = 0; i < MAXPLAYERS * DND_STORMBRAND_SLOTS && k < n; ++i) {
		if(hosts[i] != host)
			continue;
		a = base + k++ * 1.0 / n;
		seats[i] = PackMapPair(FixedMul(d, cos(a)), FixedMul(d, sin(a)));
		ACS_NamedExecuteAlways("DnD Storm Brand Seat", 0, i, seats[i]);
	}
}

// Client side: a brand's new seat on its host, which its FX glides to.
Script "DnD Storm Brand Seat" (int slot, int seat) CLIENTSIDE {
	int[] module& seats = GetStormBrandSeats();
	seats[slot] = seat;
}

// Half a second between activations, shortened by spell haste and stretched by its reductions; fixed point tics.
int GetStormBrandPeriod(int pnum) {
	int haste = PlayerModData[pnum].vals[PSTAT_SPELL_HASTE] + GetSpellGemMod(pnum, SPL_STORMBRAND, GEMSTAT_HASTE) +
		pbuffs[pnum].buff_net_values[BUFF_SPELLHASTE].additive;
	int p = FixedDiv(DND_STORMBRAND_PERIOD, Max(10, 100 + haste) << 16) * 100;
	int more = GetSpellGemMod(pnum, SPL_STORMBRAND, GEMSTAT_MORE_HASTE);
	if(more)
		p = FixedDiv(p, 1.0 + more);
	return Max(1.0, p);
}

// The nearest living enemy within reach of a point that has room for another brand, or 0.
int FindStormBrandHost(int x, int y, int z, int reach, int cap) {
	int i, m, d, dx, dy, dz, best = 0, bestd = reach + 1.0, box = (reach >> 16) + DND_SPELLAREA_BODYPAD;
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		dx = (GetActorX(m) >> 16) - (x >> 16);
		dy = (GetActorY(m) >> 16) - (y >> 16);
		dz = (GetActorZ(m) >> 16) - (z >> 16);
		if(dx > box || dx < -box || dy > box || dy < -box || dz > box || dz < -box)
			continue;
		if(!IsActorAlive(m) || !IsSpellEnemy(m) || CheckActorInventory(m, "DnD_StormBrands") >= cap)
			continue;
		d = VectorLength(VectorLength(dx << 16, dy << 16), dz << 16) - GetActorProperty(m, APROP_RADIUS);
		if(d < bestd) {
			best = m;
			bestd = d;
		}
	}
	return best;
}

// One activation: a beam from the globe into the host, harder, and one into each of the nearest enemies round the host.
void FireStormBrand(int pnum, int host, int slot) {
	static int cand[DND_STORMBRAND_MAXCAND];
	static int dist[DND_STORMBRAND_MAXCAND];
	int[] module& seats = GetStormBrandSeats();
	int gx = GetActorX(host) + (seats[slot] << 16), gy = GetActorY(host) + (seats[slot] & 0xFFFF0000);
	int gz = GetActorZ(host) + GetActorProperty(host, APROP_HEIGHT) + DND_STORMBRAND_LIFT;
	int extra = GetSpellValue(pnum, SPL_STORMBRAND, SPELLVAL_AMOUNT) + GetSpellExtraProjectiles(pnum, SPL_STORMBRAND);
	int reach = ScaleSpellArea(pnum, SPL_STORMBRAND, GetSpellValue(pnum, SPL_STORMBRAND, SPELLVAL_RADIUS) << 16);
	int i, j, m, n = 0;

	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER] && n < DND_STORMBRAND_MAXCAND; ++i) {
		m = UsedMonsterTIDs[i];
		if(m == host || !InSpellArea(host, m, gx, gy, reach))
			continue;
		cand[n] = m;
		dist[n] = VectorLength(GetActorX(m) - gx, GetActorY(m) - gy);
		++n;
	}

	// Nearest first, and only as far as the beams go.
	for(i = 0; i < extra && i < n; ++i) {
		for(j = i + 1; j < n; ++j) {
			if(dist[j] < dist[i]) {
				m = cand[i]; cand[i] = cand[j]; cand[j] = m;
				m = dist[i]; dist[i] = dist[j]; dist[j] = m;
			}
		}
	}
	n = Min(n, extra);

	SendBeamToEnemy("DnD Forked Beam FX", gx, gy, gz, host);
	for(i = 0; i < n; ++i)
		SendBeamToEnemy("DnD Forked Beam FX", gx, gy, gz, cand[i]);
	PlaySound(host, "StormBrand/Zap", CHAN_7);

	DealSpellBeamHit(pnum, SPL_STORMBRAND, host, 100 + GetSpellValue(pnum, SPL_STORMBRAND, SPELLVAL_DAMAGE2));
	for(i = 0; i < n; ++i) {
		if(IsActorAlive(cand[i]))
			DealSpellBeamHit(pnum, SPL_STORMBRAND, cand[i], 100);
	}
}

// Tells every client where a brand's globe is: on its host at its seat, or hovering over x, y, dropped there when
// detached; 0 tics ends it.
void SendStormBrandFX(int slot, int host, int x, int y, int tics, bool detached = false) {
	int[] module& seats = GetStormBrandSeats();
	ACS_NamedExecuteAlways("DnD Storm Brand FX", 0, slot | (detached << 9) | (tics << 10), host, host ? seats[slot] : PackMapPair(x, y));
}

// A brand takes a host: it counts toward the host's cap, and the brands on it make room.
void AttachStormBrand(int pnum, int slot, int host) {
	int[] module& hosts = GetStormBrandHosts();
	GiveActorInventory(host, "DnD_StormBrands", 1);
	hosts[slot] = host;
	SeatStormBrands(host, pnum);
}

// One brand: it lands at the aim, rides the first enemy within reach, and drops where its host falls to wait again.
Script "DnD Storm Brand" (int pnum) {
	int host = BeginSpellSpot(pnum, DND_STORMBRAND_RANGE);
	for(int w = 0; !host && !ReadSpellAim(pnum) && w < DND_SPELLAIM_WAIT; ++w)
		Delay(const:1);
	if(host < 0 || (!host && !SpellAimReady(pnum)) || !PlayerInGame(pnum))
		Terminate;

	auto aim = GetSpellAim();
	int x = aim.x[pnum], y = aim.y[pnum], z = GetFloorZAt(aim.x[pnum], aim.y[pnum]);
	int[] module& serials = GetStormBrandSerials();
	int slot = pnum * DND_STORMBRAND_SLOTS + serials[pnum] % DND_STORMBRAND_SLOTS;
	++serials[pnum];
	int cap = GetSpellValue(pnum, SPL_STORMBRAND, SPELLVAL_AMOUNT2);
	int reach = ScaleSpellArea(pnum, SPL_STORMBRAND, DND_STORMBRAND_REACH);
	int tics = GetSpellDurationTics(pnum, SPL_STORMBRAND);
	int period = GetStormBrandPeriod(pnum), acc = 0, t;
	int[] module& hosts = GetStormBrandHosts();

	// The enemy it was aimed at when that one has room, else whatever is nearest the spot.
	if(!host || CheckActorInventory(host, "DnD_StormBrands") >= cap)
		host = FindStormBrandHost(x, y, z, reach, cap);
	if(host) {
		AttachStormBrand(pnum, slot, host);
		acc = period;
	}
	PlaySound(pnum + P_TIDSTART, "StormBrand/Cast", CHAN_6);
	SendStormBrandFX(slot, host, x, y, tics);

	for(t = 0; t < tics && PlayerInGame(pnum); ++t) {
		// Every brand on a dying host drops on its own this tic, so there is no one left to make room for.
		if(host && !IsActorAlive(host)) {
			x = GetActorX(host);
			y = GetActorY(host);
			z = GetActorFloorZ(host);
			TakeActorInventory(host, "DnD_StormBrands", 1);
			hosts[slot] = 0;
			host = 0;
			SendStormBrandFX(slot, 0, x, y, tics - t, true);
		}

		if(!host) {
			if(!(t % DND_STORMBRAND_SEEK) && (host = FindStormBrandHost(x, y, z, reach, cap))) {
				AttachStormBrand(pnum, slot, host);
				SendStormBrandFX(slot, host, x, y, tics - t);
				acc = period;
			}
		}
		else if((acc += 1.0) >= period) {
			acc -= period;
			FireStormBrand(pnum, host, slot);
			period = GetStormBrandPeriod(pnum);
		}
		Delay(const:1);
	}

	// Ending on a living host: the brands left on it close up.
	if(host) {
		TakeActorInventory(host, "DnD_StormBrands", 1);
		if(hosts[slot] == host) {
			hosts[slot] = 0;
			SeatStormBrands(host, pnum);
		}
	}
	SendStormBrandFX(slot, 0, x, y, 0);
}

// Client side: one part of a brand, riding the host at lift units up, or hovering over the floor where it waits.
// It joins group, or with none keeps the tid it returns.
int SpawnStormBrandPart(str actor, int group, int x, int y, int z, int host, int lift) {
	int tid = ScratchTID();
	if(!tid || !SpawnForced(actor, x, y, z, tid))
		return 0;
	if(host) {
		SetActivator(tid);
		SetPointer(AAPTR_TARGET, host);
		SetActorProperty(tid, APROP_MASS, lift >> 16);
	}
	else
		SetActorPosition(tid, x, y, GetActorFloorZ(tid) + DND_STORMBRAND_HOVER, false);
	if(group)
		Thing_ChangeTID(tid, group);
	return tid;
}

// Client side: a brand's globe, rune circle and core, with crackles round it; a newer message for its slot ends this one.
Script "DnD Storm Brand FX" (int packed, int host, int xy) CLIENTSIDE {
	int slot = packed & 0x1FF, tics = packed >> 10;
	bool detached = packed & 0x200;
	int[] module& tickets = GetStormBrandFXTickets();
	int ticket = ++tickets[slot];
	if(tics <= 0)
		Terminate;

	// With a host, xy is the brand's seat off its middle rather than a spot.
	int x = xy << 16, y = xy & 0xFFFF0000, z = 0, lift = 0, ox = x, oy = y;
	int[] module& seats = GetStormBrandSeats();
	if(host) {
		if(!IsActorAlive(host))
			Terminate;
		seats[slot] = xy;
		x += GetActorX(host);
		y += GetActorY(host);
		lift = GetActorProperty(host, APROP_HEIGHT) + DND_STORMBRAND_LIFT;
		z = GetActorZ(host) + lift;
	}

	// The globe keeps a tid of its own and carries the brand's sounds; its hum ends when it is removed.
	int globe = SpawnStormBrandPart("Spell_StormBrand_Globe", 0, x, y, z, host, lift);
	if(!globe)
		Terminate;
	int group = ScratchTID();
	if(!group) {
		Thing_Remove(globe);
		Terminate;
	}
	SpawnStormBrandPart("Spell_StormBrand_Ring", group, x, y, z, host, lift);
	SpawnStormBrandPart("Spell_StormBrand_RingFlip", group, x, y, z, host, lift);
	SpawnStormBrandPart("Spell_StormBrand_Core", group, x, y, z, host, lift);
	if(host) {
		PlaySound(globe, "StormBrand/Attach", CHAN_5);
		PlaySound(globe, "StormBrand/Loop", CHAN_6, 1.0, true);
	}
	else if(detached)
		PlaySound(globe, "StormBrand/Detach", CHAN_5);

	for(int t = 0; t < tics && ticket == tickets[slot] && (!host || IsActorAlive(host)); ++t) {
		if(host) {
			ox += ((seats[slot] << 16) - ox) / DND_STORMBRAND_GLIDE;
			oy += ((seats[slot] & 0xFFFF0000) - oy) / DND_STORMBRAND_GLIDE;
			SetUserVariable(globe, "user_ox", ox >> 16);
			SetUserVariable(globe, "user_oy", oy >> 16);
			SetUserVariable(group, "user_ox", ox >> 16);
			SetUserVariable(group, "user_oy", oy >> 16);
		}
		if(!(t % 5))
			Spawn("Spell_StaticField_Spark", GetActorX(globe) + random(-12.0, 12.0), GetActorY(globe) + random(-12.0, 12.0),
				GetActorZ(globe) + random(-8.0, 8.0));
		Delay(const:1);
	}
	Thing_Remove(group);
	Thing_Remove(globe);
}

// ============================ Monsoon ============================

#define DND_MONSOON_RANGE 2048.0
#define DND_MONSOON_GATHER TICRATE      // the clouds gather for a second before the first strike
#define DND_MONSOON_SPLASH 80.0
#define DND_MONSOON_LOCK TICRATE        // an enemy takes a strike at most once a second
#define DND_MONSOON_MINGAP 9            // 0.25 to 0.75 seconds between strikes
#define DND_MONSOON_MAXGAP 26
#define DND_MONSOON_R10_SHOCK 10
#define DND_MONSOON_LOCKS 64
#define DND_MONSOON_GROUNDTRIES 4
#define DND_MONSOON_CLOUDS 200.0        // the clouds' middle sits this far over the floor, ceiling allowing
#define DND_MONSOON_CLOUDMIN 72.0       // and never lower than this over the floor
#define DND_MONSOON_CLOUDHALF 110.0     // the tallest cloud's half height at size 1: MNCL is 192 high, sized up to 1.15
#define DND_MONSOON_CLOUDBOB 26.0       // how far above its place a cloud may ride
#define DND_MONSOON_ROOMFULL 384.0      // a room this tall or taller gets full size clouds
#define DND_MONSOON_CLOUDCOUNT 22
#define DND_MONSOON_CLOUDCORE 60        // percent of them massed over the target; the rest ring the storm
#define DND_MONSOON_CLOUDSPIN 0.03      // turns a second the mass wheels round
#define DND_MONSOON_CLOUDFROM 2.4       // how far out they gather from, as a multiple of where they settle
#define DND_SKYBOLT_TALL 1024.0         // the MNBT frames' height in pixels
#define DND_SKYBOLT_BIG 512.0           // how high Storm Call's bolt comes down from, ceiling allowing

int[] module& GetMonsoonLockTids() {
	static int t[MAXPLAYERS * DND_MONSOON_LOCKS];
	return t;
}

int[] module& GetMonsoonLockEnds() {
	static int t[MAXPLAYERS * DND_MONSOON_LOCKS];
	return t;
}

// Whether the caster's storms struck this enemy in the last second.
bool IsMonsoonLocked(int pnum, int m) {
	int[] module& tids = GetMonsoonLockTids();
	int[] module& ends = GetMonsoonLockEnds();
	int base = pnum * DND_MONSOON_LOCKS, now = Timer();
	for(int i = 0; i < DND_MONSOON_LOCKS; ++i) {
		if(tids[base + i] == m && ends[base + i] > now)
			return true;
	}
	return false;
}

void LockMonsoon(int pnum, int m) {
	int[] module& tids = GetMonsoonLockTids();
	int[] module& ends = GetMonsoonLockEnds();
	int base = pnum * DND_MONSOON_LOCKS, now = Timer();
	for(int i = 0; i < DND_MONSOON_LOCKS; ++i) {
		if(ends[base + i] <= now) {
			tids[base + i] = m;
			ends[base + i] = now + DND_MONSOON_LOCK;
			return;
		}
	}
}

// One strike at a spot: every enemy round it the storm has not struck in the last second takes the hit, and is locked.
void MonsoonStrike(int pnum, int x, int y, int z, int splash) {
	int probe = SpawnSpellProbe(x, y, z + 32.0), i, m;
	bool shock = SpellThresholdMet(pnum, SPL_MONSOON, DND_SPELL_THRESH_HIGH);
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(!InSpellArea(probe, m, x, y, splash) || IsMonsoonLocked(pnum, m))
			continue;
		LockMonsoon(pnum, m);
		DealSpellBeamHit(pnum, SPL_MONSOON, m, 100, true);
		if(shock && IsActorAlive(m) && random(1, 100) <= DND_MONSOON_R10_SHOCK)
			ApplySpellShock(pnum, m);
	}
	if(probe)
		Thing_Remove(probe);
	ACS_NamedExecuteAlways("DnD Monsoon Strike FX", 0, PackMapPair(x, y), z >> 16);
}

// A random enemy in the storm the probe at its centre can see and that was not struck in the last second, or 0.
int PickMonsoonVictim(int pnum, int probe, int x, int y, int radius) {
	static int cand[DND_MONSOON_LOCKS];
	int i, m, n = 0;
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER] && n < DND_MONSOON_LOCKS; ++i) {
		m = UsedMonsterTIDs[i];
		if(InSpellArea(probe, m, x, y, radius) && !IsMonsoonLocked(pnum, m))
			cand[n++] = m;
	}
	return n ? cand[random(0, n - 1)] : 0;
}

// The storm: a second of gathering, then a strike every 0.25-0.75s on an enemy in reach not struck in the last second,
// or on open ground in sight of the centre when none is free. The centre follows the target, and stays where it fell.
Script "DnD Monsoon" (int pnum) {
	int target = BeginSpellSpot(pnum, DND_MONSOON_RANGE);
	for(int w = 0; !target && !ReadSpellAim(pnum) && w < DND_SPELLAIM_WAIT; ++w)
		Delay(const:1);
	if(target < 0 || (!target && !SpellAimReady(pnum)) || !PlayerInGame(pnum))
		Terminate;

	auto aim = GetSpellAim();
	int cx = aim.x[pnum], cy = aim.y[pnum], cz = GetFloorZAt(aim.x[pnum], aim.y[pnum]);
	int probe = SpawnSpellProbe(cx, cy, cz + 32.0);
	if(!probe)
		Terminate;

	int tics = GetSpellDurationTics(pnum, SPL_MONSOON);
	int radius = ScaleSpellArea(pnum, SPL_MONSOON, GetSpellValue(pnum, SPL_MONSOON, SPELLVAL_RADIUS) << 16);
	int splash = ScaleSpellArea(pnum, SPL_MONSOON, DND_MONSOON_SPLASH);
	int t, gap, victim, k, a, d, sx, sy, sz, spot;

	ACS_NamedExecuteAlways("DnD Monsoon Clouds FX", 0, target, PackMapPair(cx, cy),
		((tics + DND_MONSOON_GATHER) & 0xFFFF) | (radius & 0xFFFF0000));
	PlaySound(probe, "Monsoon/Gather", CHAN_6);
	Delay(const:DND_MONSOON_GATHER);

	for(t = 0; t < tics && PlayerInGame(pnum); t += gap) {
		if(target && IsActorAlive(target)) {
			cx = GetActorX(target);
			cy = GetActorY(target);
			cz = GetActorFloorZ(target);
			SetActorPosition(probe, cx, cy, cz + 32.0, false);
		}

		victim = PickMonsoonVictim(pnum, probe, cx, cy, radius);
		if(victim) {
			sx = GetActorX(victim);
			sy = GetActorY(victim);
			sz = GetActorZ(victim);
		}
		else {
			// Open ground the centre can see; failing that, the centre itself.
			sx = cx;
			sy = cy;
			sz = cz;
			for(k = 0; k < DND_MONSOON_GROUNDTRIES; ++k) {
				a = random(0, 1.0);
				d = FixedMul(radius, random(0.15, 1.0));
				spot = SpawnSpellProbe(cx + FixedMul(d, cos(a)), cy + FixedMul(d, sin(a)), cz + 32.0);
				if(spot && CheckSight(probe, spot, CSF_NOBLOCKALL)) {
					sx = GetActorX(spot);
					sy = GetActorY(spot);
					sz = GetFloorZAt(sx, sy);
					Thing_Remove(spot);
					break;
				}
				if(spot)
					Thing_Remove(spot);
			}
		}

		MonsoonStrike(pnum, sx, sy, sz, splash);
		gap = random(DND_MONSOON_MINGAP, DND_MONSOON_MAXGAP);
		Delay(gap);
	}
	Thing_Remove(probe);
}

// The storm clouds' size for a room: full under a high ceiling, smaller in a low one so they fit over the heads below.
int GetMonsoonCloudSize(int floor, int ceil) {
	return Clamp_Between(FixedDiv(ceil - floor, DND_MONSOON_ROOMFULL), 0.45, 1.25);
}

// The height of the storm clouds' middle over a floor, their tops kept under the ceiling; Monsoon's bolts come out of it.
int GetMonsoonCloudZ(int floor, int ceil) {
	int top = ceil - FixedMul(DND_MONSOON_CLOUDHALF, GetMonsoonCloudSize(floor, ceil)) - DND_MONSOON_CLOUDBOB;
	return Max(floor + DND_MONSOON_CLOUDMIN, Min(floor + DND_MONSOON_CLOUDS, top));
}

// Client side: a bolt from the cloud height down onto a spot, a little toward this viewer, and a burst on the ground.
void SpawnSkyBolt(int x, int y, int z, bool big) {
	int me = ConsolePlayerNumber() + P_TIDSTART, a;
	if(me >= P_TIDSTART && PlayerInGame(me - P_TIDSTART)) {
		a = VectorAngle(GetActorX(me) - x, GetActorY(me) - y);
		x += FixedMul(DND_STATICFIELD_FXNEAR * 2, cos(a));
		y += FixedMul(DND_STATICFIELD_FXNEAR * 2, sin(a));
	}

	int tid = ScratchTID();
	if(tid && SpawnForced(big ? "Spell_StormCall_Bolt" : "Spell_Monsoon_Bolt", x, y, z, tid)) {
		int top = big ? Min(GetActorFloorZ(tid) + DND_SKYBOLT_BIG, GetActorCeilingZ(tid)) : GetMonsoonCloudZ(GetActorFloorZ(tid), GetActorCeilingZ(tid));
		SetActorProperty(tid, APROP_SCALEY, FixedDiv(Max(64.0, top - z), DND_SKYBOLT_TALL));
		Spawn("Spell_Monsoon_CloudFlash", x, y, top);
		if(big) {
			PlaySound(tid, "StormCall/Explode", CHAN_BODY);
			PlaySound(tid, "StormCall/Pulse", CHAN_ITEM);
		}
		else
			PlaySound(tid, "Monsoon/Strike", CHAN_BODY);
		PlaySound(tid, "StormCall/Add", CHAN_VOICE);
		SetActorProperty(tid, APROP_SCALEX, big ? 0.9 : 0.45);
		Thing_ChangeTID(tid, 0);
	}
	Spawn(big ? "Spell_StormCall_Burst" : "Spell_Overcharge_Burst", x, y, z + 8.0);
}

Script "DnD Monsoon Strike FX" (int xy, int z) CLIENTSIDE {
	SpawnSkyBolt(xy << 16, xy & 0xFFFF0000, z << 16, false);
}

// Client side: big storm clouds drifting in from all round to mass over the target, wheeling slowly as they ride it.
// They hold their places round an anchor that follows the target while it lives and stays where it fell.
Script "DnD Monsoon Clouds FX" (int target, int xy, int packed) CLIENTSIDE {
	int tics = packed & 0xFFFF, radius = packed & 0xFFFF0000;
	int anchor = ScratchTID();
	if(!anchor || !SpawnForced("Spell_Monsoon_CloudAnchor", xy << 16, xy & 0xFFFF0000, 0, anchor))
		Terminate;
	if(target && IsActorAlive(target))
		SetActorPosition(anchor, GetActorX(target), GetActorY(target), GetActorZ(target), false);
	else
		SetActorPosition(anchor, GetActorX(anchor), GetActorY(anchor), GetActorFloorZ(anchor), false);

	int floor = GetActorFloorZ(anchor), ceil = GetActorCeilingZ(anchor);
	int lift = GetMonsoonCloudZ(floor, ceil) - floor;
	int size = GetMonsoonCloudSize(floor, ceil);
	int tid[DND_MONSOON_CLOUDCOUNT], ang[DND_MONSOON_CLOUDCOUNT], dist[DND_MONSOON_CLOUDCOUNT], bob[DND_MONSOON_CLOUDCOUNT];
	int i, t, g, d, a, part, n = 0;

	for(i = 0; i < DND_MONSOON_CLOUDCOUNT; ++i) {
		part = ScratchTID();
		if(!part || !SpawnForced("Spell_Monsoon_Cloud", GetActorX(anchor), GetActorY(anchor), floor + lift, part))
			continue;
		SetActorProperty(part, APROP_SCALEX, FixedMul(size, random(0.8, 1.15)));
		SetActorProperty(part, APROP_SCALEY, GetActorProperty(part, APROP_SCALEX));
		SetActivator(part);
		SetPointer(AAPTR_TARGET, anchor);
		tid[n] = part;
		ang[n] = random(0, 1.0);
		dist[n] = FixedMul(radius, i * 100 < DND_MONSOON_CLOUDCORE * DND_MONSOON_CLOUDCOUNT ? random(0.0, 0.4) : random(0.5, 0.85));
		bob[n] = random(-14.0, DND_MONSOON_CLOUDBOB - 6.0);
		++n;
	}

	// The first cloud, one of the core over the target, is the storm's voice: its loop while it lasts, its end as it clears.
	if(n)
		PlaySound(tid[0], "Monsoon/Loop", CHAN_BODY, 1.0, true);

	for(t = 0; t < tics; ++t) {
		if(target && IsActorAlive(target))
			SetActorPosition(anchor, GetActorX(target), GetActorY(target), GetActorZ(target), false);

		// Eased in over the gathering second, from far out to where they settle.
		g = t < DND_MONSOON_GATHER ? FixedDiv(t << 16, DND_MONSOON_GATHER << 16) : 1.0;
		g = 1.0 - FixedMul(1.0 - g, 1.0 - g);
		for(i = 0; i < n; ++i) {
			d = FixedMul(dist[i], DND_MONSOON_CLOUDFROM - FixedMul(DND_MONSOON_CLOUDFROM - 1.0, g));
			a = ang[i] + DND_MONSOON_CLOUDSPIN * t / TICRATE;
			SetUserVariable(tid[i], "user_ox", FixedMul(d, cos(a)) >> 16);
			SetUserVariable(tid[i], "user_oy", FixedMul(d, sin(a)) >> 16);
			SetUserVariable(tid[i], "user_oz", (lift + bob[i] + 6 * sin(ang[i] + t * 0.004)) >> 16);
		}
		Delay(const:1);
	}

	// On the loop's channel, so it takes over from it; it plays on where the cloud was once it has faded out.
	if(n)
		PlaySound(tid[0], "Monsoon/End", CHAN_BODY);
	for(i = 0; i < n; ++i) {
		SetActorState(tid[i], "Clear");
		Thing_ChangeTID(tid[i], 0);
	}
	Thing_Remove(anchor);
}

// ============================ Thunder Clap ============================

#define DND_THUNDERCLAP_GROWTH 1.3      // each ring is this much of the one before, in reach and in damage
#define DND_THUNDERCLAP_GAP 10          // tics between rings
#define DND_THUNDERCLAP_GROW 8          // tics a ring takes to spread out to its reach
#define DND_THUNDERCLAP_SPARKS 4        // sparks thrown round a spreading ring each tic
#define DND_THUNDERCLAP_R5_STUN (TICRATE * 3 / 2)
#define DND_FLATMD3_QUAD 15.625         // flat.md3's frame 3 half width, which the ring and field art fill
#define DND_THUNDERCLAP_WAVEH 56.0      // a ring's shockwave height; the first stands a quarter taller
#define DND_THUNDERCLAP_WAVEPOP 3       // tics the shockwave takes to rise
#define DND_THUNDERCLAP_WAVEFADE 10     // tics it takes to sink and fade once its ring is out
#define DND_SLAMWAVE_BASE 16.0          // SLAMWAVE.md3's base radius and height

// The slam's damage: the spell's own, priced, plus its share of the held melee weapon's swing, priced as that weapon's.
int GetThunderClapDamage(int pnum) {
	int dmg = PriceSpellDamage(pnum, SPL_THUNDERCLAP, GetSpellValue(pnum, SPL_THUNDERCLAP, SPELLVAL_DAMAGE), DND_WDMG_ISRADIUSDMG);
	int wep = GetActorWeaponID(pnum + P_TIDSTART);
	if(IsMeleeWeapon(wep))
		dmg += RetrieveWeaponDamage(pnum, wep, 0, DND_DAMAGECATEGORY_MELEE, 0, 0) * GetSpellValue(pnum, SPL_THUNDERCLAP, SPELLVAL_DAMAGE2) / 100;
	return dmg;
}

// The slam: rings spreading from where the caster stood, each further and harder than the last, each hitting all inside it.
Script "DnD Thunder Clap" (int pnum) {
	int caster = pnum + P_TIDSTART;
	SetActivator(caster);
	int x = GetActorX(caster), y = GetActorY(caster), z = GetActorZ(caster);
	int probe = SpawnSpellProbe(x, y, z + 32.0);
	int dmg = GetThunderClapDamage(pnum), dealt;
	int rings = 1 + GetSpellValue(pnum, SPL_THUNDERCLAP, SPELLVAL_AMOUNT);
	int r = ScaleSpellArea(pnum, SPL_THUNDERCLAP, GetSpellValue(pnum, SPL_THUNDERCLAP, SPELLVAL_RADIUS) << 16), prev = 0;
	int stun = SpellThresholdMet(pnum, SPL_THUNDERCLAP, DND_SPELL_THRESH_LOW) ? ScaleSpellEffectTics(pnum, SPL_THUNDERCLAP, DND_THUNDERCLAP_R5_STUN) : 0;
	int i, j, m;

	PlaySound(caster, "ThunderClap/Add", CHAN_5);
	PlaySound(caster, "ThunderClap/Slam", CHAN_6);
	for(i = 0; i < rings; ++i) {
		ACS_NamedExecuteAlways("DnD Thunder Clap FX", 0, PackMapPair(x, y), z >> 16, ((prev >> 16) & 0xFFFF) | (r & 0xFFFF0000));
		for(j = 0; j < InformationInLevel[LEVELINFO_TID_MONSTER]; ++j) {
			m = UsedMonsterTIDs[j];
			if(!InSpellArea(probe, m, x, y, r))
				continue;
			dealt = HandleDamageDeal(caster, m, dmg, GetSpellDamageType(SPL_THUNDERCLAP), SPL_THUNDERCLAP,
				DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISRADIUSDMG, 0, 0, 0, 0, true);
			if(dealt > 0)
				Thing_Damage2(m, dealt, "SkipHandle");
			if(stun && IsActorAlive(m))
				StunMonster(m, stun);
		}
		prev = r;
		r = FixedMul(r, DND_THUNDERCLAP_GROWTH);
		dmg = dmg * 13 / 10;
		Delay(const:DND_THUNDERCLAP_GAP);
	}
	if(probe)
		Thing_Remove(probe);
}

// Client side: one ring spreading from the last one's reach to its own over a few tics, throwing sparks off its edge,
// with its shockwave popping up and riding just ahead of it, then sinking and fading.
Script "DnD Thunder Clap FX" (int xy, int z, int radii) CLIENTSIDE {
	int x = xy << 16, y = xy & 0xFFFF0000, from = radii << 16, to = radii & 0xFFFF0000;
	z <<= 16;
	int tid = ScratchTID(), t, k, a, rr, e, h, f;
	if(!tid || !SpawnForced("Spell_ThunderClap_Ring", x, y, z + 2.0, tid))
		Terminate;
	if(!from)
		Spawn("Spell_ThunderClap_Burst", x, y, z + 8.0);

	int wave = ScratchTID();
	if(wave && !SpawnForced("Spell_ThunderClap_Shockwave", x, y, z + 1.0, wave))
		wave = 0;
	int hmax = from ? DND_THUNDERCLAP_WAVEH : DND_THUNDERCLAP_WAVEH * 5 / 4, start = from ? from : to / 6;

	for(t = 1; t <= DND_THUNDERCLAP_GROW + DND_THUNDERCLAP_WAVEFADE; ++t) {
		if(t <= DND_THUNDERCLAP_GROW) {
			rr = from + (to - from) / DND_THUNDERCLAP_GROW * t;
			SetActorProperty(tid, APROP_SCALEX, FixedDiv(rr, DND_FLATMD3_QUAD));
			for(k = 0; k < DND_THUNDERCLAP_SPARKS; ++k) {
				a = random(0, 1.0);
				Spawn("Spell_ThunderClap_Spark", x + FixedMul(rr, cos(a)), y + FixedMul(rr, sin(a)), z + random(4.0, 40.0));
			}
		}
		if(wave) {
			// Eased out, so the wave's front runs a little ahead of the ring and meets it at the ring's reach.
			e = FixedDiv(Min(t, DND_THUNDERCLAP_GROW) << 16, DND_THUNDERCLAP_GROW << 16);
			rr = start + FixedMul(to - start, 1.0 - FixedMul(1.0 - e, 1.0 - e));
			e = FixedDiv(Min(t, DND_THUNDERCLAP_WAVEPOP) << 16, DND_THUNDERCLAP_WAVEPOP << 16);
			h = FixedMul(hmax, 1.0 - FixedMul(1.0 - e, 1.0 - e));
			if(t > DND_THUNDERCLAP_GROW) {
				f = FixedDiv((t - DND_THUNDERCLAP_GROW) << 16, DND_THUNDERCLAP_WAVEFADE << 16);
				rr += FixedMul(to / 12, f);
				h = FixedMul(h, 1.0 - FixedMul(0.6, f));
				SetActorProperty(wave, APROP_ALPHA, FixedMul(0.95, 1.0 - f));
			}
			SetActorProperty(wave, APROP_SCALEX, FixedDiv(rr, DND_SLAMWAVE_BASE));
			SetActorProperty(wave, APROP_SCALEY, FixedDiv(h, DND_SLAMWAVE_BASE));
		}
		Delay(const:1);
	}
	Thing_ChangeTID(tid, 0);
	if(wave)
		Thing_Remove(wave);
}

// ============================ Storm Call ============================

#define DND_STORMCALL_RANGE 2048.0
#define DND_STORMCALL_WAIT 56           // 1.6 seconds from the mark to the strike
#define DND_STORMCALL_CHAINGAP 7        // tics between the pending calls a strike sets off
#define DND_STORMCALL_SLOTS 8

typedef struct {
	int x[MAXPLAYERS * DND_STORMCALL_SLOTS];
	int y[MAXPLAYERS * DND_STORMCALL_SLOTS];
	int z[MAXPLAYERS * DND_STORMCALL_SLOTS];
	int r[MAXPLAYERS * DND_STORMCALL_SLOTS];
	int order[MAXPLAYERS * DND_STORMCALL_SLOTS];	// 0 for a free slot, else when it was marked; chains go oldest first
	int marked[MAXPLAYERS];
	int chaining[MAXPLAYERS];		// the newest call the running chain sets off, 0 while none runs
} storm_call_T;

storm_call_T module& GetStormCalls() {
	static storm_call_T s;
	return s;
}

// Client side: per slot, the serial of the mark whose field shows, negated once its strike lands. Serials rise per
// caster, so a field and its strike starting on the same tic work out in either order.
int[] module& GetStormCallFXSerials() {
	static int t[MAXPLAYERS * DND_STORMCALL_SLOTS];
	return t;
}

// The caster's oldest pending call marked no later than upto, or -1.
int GetOldestStormCall(int pnum, int upto = 0x7FFFFFFF) {
	auto sc = GetStormCalls();
	int i, k, best = -1;
	for(i = 0; i < DND_STORMCALL_SLOTS; ++i) {
		k = pnum * DND_STORMCALL_SLOTS + i;
		if(sc.order[k] && sc.order[k] <= upto && (best == -1 || sc.order[k] < sc.order[best]))
			best = k;
	}
	return best;
}

// A call's strike: everything in its area takes a rolled hit, and its mark gives way to a bolt from the sky.
void StormCallStrike(int pnum, int k) {
	auto sc = GetStormCalls();
	int x = sc.x[k], y = sc.y[k], z = sc.z[k], r = sc.r[k], i, m, serial = sc.order[k];
	sc.order[k] = 0;
	int probe = SpawnSpellProbe(x, y, z + 32.0);
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(InSpellArea(probe, m, x, y, r))
			DealSpellBeamHit(pnum, SPL_STORMCALL, m, 100, true);
	}
	if(probe)
		Thing_Remove(probe);
	ACS_NamedExecuteAlways("DnD Storm Call Strike FX", 0, k | (serial << 9), PackMapPair(x, y), z >> 16);
}

// Sets off the caster's calls marked up to upto, oldest first, a short beat apart; a strike while one runs extends
// it. Calls marked after the strike wait out their own delay.
Script "DnD Storm Call Chain" (int pnum, int upto) {
	auto sc = GetStormCalls();
	bool running = sc.chaining[pnum] != 0;
	sc.chaining[pnum] = Max(sc.chaining[pnum], upto);
	if(running)
		Terminate;
	int k;
	while((k = GetOldestStormCall(pnum, sc.chaining[pnum])) != -1 && PlayerInGame(pnum)) {
		Delay(const:DND_STORMCALL_CHAINGAP);
		// Its own delay may have set it off meanwhile, and a newer mark taken the slot.
		if(sc.order[k] && sc.order[k] <= sc.chaining[pnum])
			StormCallStrike(pnum, k);
	}
	sc.chaining[pnum] = 0;
}

// Marks the spot and waits out the delay unless it strikes at once; a strike sets off the caster's other pending calls.
Script "DnD Storm Call" (int pnum) {
	int target = BeginSpellSpot(pnum, DND_STORMCALL_RANGE);
	for(int w = 0; !target && !ReadSpellAim(pnum) && w < DND_SPELLAIM_WAIT; ++w)
		Delay(const:1);
	if(target < 0 || (!target && !SpellAimReady(pnum)) || !PlayerInGame(pnum))
		Terminate;

	auto aim = GetSpellAim();
	auto sc = GetStormCalls();
	int i, k = -1, base = pnum * DND_STORMCALL_SLOTS;
	for(i = 0; i < DND_STORMCALL_SLOTS && k == -1; ++i) {
		if(!sc.order[base + i])
			k = base + i;
	}

	// Every slot pending: the oldest goes off now to make room.
	if(k == -1) {
		k = GetOldestStormCall(pnum);
		StormCallStrike(pnum, k);
	}

	sc.x[k] = aim.x[pnum];
	sc.y[k] = aim.y[pnum];
	sc.z[k] = GetFloorZAt(aim.x[pnum], aim.y[pnum]);
	sc.r[k] = ScaleSpellArea(pnum, SPL_STORMCALL, GetSpellValue(pnum, SPL_STORMCALL, SPELLVAL_RADIUS) << 16);
	int order = ++sc.marked[pnum];
	sc.order[k] = order;

	PlaySound(pnum + P_TIDSTART, "StormCall/Cast", CHAN_6);
	ACS_NamedExecuteAlways("DnD Storm Call Field FX", 0, k | (order << 9), PackMapPair(sc.x[k], sc.y[k]),
		((sc.z[k] >> 16) & 0xFFFF) | (sc.r[k] & 0xFFFF0000));

	if(random(1, 100) > GetSpellValue(pnum, SPL_STORMCALL, SPELLVAL_DAMAGE2)) {
		Delay(const:DND_STORMCALL_WAIT);
		// The chain got to it first, or the caster is gone.
		if(sc.order[k] != order || !PlayerInGame(pnum))
			Terminate;
	}
	StormCallStrike(pnum, k);
	ACS_NamedExecuteAlways("DnD Storm Call Chain", 0, pnum, sc.marked[pnum]);
}

// Client side: the marked ground, charging through the wait, until its own strike or a newer mark in its slot.
Script "DnD Storm Call Field FX" (int ks, int xy, int zr) CLIENTSIDE {
	int k = ks & 0x1FF, serial = ks >> 9;
	int[] module& shown = GetStormCallFXSerials();
	// Its strike got here first, or a newer mark already holds the slot.
	if(shown[k] == -serial || (shown[k] < 0 ? -shown[k] : shown[k]) > serial)
		Terminate;
	shown[k] = serial;

	int x = xy << 16, y = xy & 0xFFFF0000, z = ((zr << 16) >> 16) << 16, r = zr & 0xFFFF0000;
	int tid = ScratchTID(), t, a, d;
	if(!tid || !SpawnForced("Spell_StormCall_Field", x, y, z + 1.0, tid))
		Terminate;
	SetActorProperty(tid, APROP_SCALEX, FixedDiv(r, DND_FLATMD3_QUAD));

	for(t = 0; shown[k] == serial && t < DND_STORMCALL_WAIT * 3; ++t) {
		SetActorProperty(tid, APROP_ALPHA, Min(1.0, 0.25 + 0.75 * t / DND_STORMCALL_WAIT));
		if(!(t % 3)) {
			a = random(0, 1.0);
			d = FixedMul(r, random(0.1, 0.95));
			Spawn("Spell_ThunderClap_Spark", x + FixedMul(d, cos(a)), y + FixedMul(d, sin(a)), z + random(4.0, 20.0));
		}
		Delay(const:1);
	}
	Thing_Remove(tid);
}

// Client side: the strike ends its own mark's field, never a newer one in the slot, and drops the big bolt on the centre.
Script "DnD Storm Call Strike FX" (int ks, int xy, int z) CLIENTSIDE {
	int k = ks & 0x1FF, serial = ks >> 9;
	int[] module& shown = GetStormCallFXSerials();
	if((shown[k] < 0 ? -shown[k] : shown[k]) <= serial)
		shown[k] = -serial;
	SpawnSkyBolt(xy << 16, xy & 0xFFFF0000, z << 16, true);
}

// ============================ Earth ============================

// A spell's own poison on one enemy: a stack of per_sec a second for tenths of a second, before duration gear.
void ApplySpellPoison(int pnum, int spell, int victim, int per_sec, int tenths) {
	// Priced per tick at the base poison rate, so faster poison ticks deal more, as a weapon's do.
	if(per_sec > 0)
		PoisonWithSpell(pnum, spell, victim, Max(1, PriceSpellPoison(pnum, spell, (per_sec * DND_BASE_POISON_TIC) >> 16)), tenths);
}

// A spell hit's poison as a weapon hit's would be: a tick of DND_BASE_POISON_FACTOR percent of what landed.
void PoisonFromSpellHit(int pnum, int spell, int victim, int dealt, int tenths) {
	if(dealt > 0)
		PoisonWithSpell(pnum, spell, victim, Max(1, dealt * DND_BASE_POISON_FACTOR / 100), tenths);
}

// One poison stack of tick a tick for tenths of a second, with the spell's duration gear; tick is the poison's base.
void PoisonWithSpell(int pnum, int spell, int victim, int tick, int tenths) {
	if(tenths <= 0 || !IsActorAlive(victim) || !CheckAilmentImmunity(pnum, victim - DND_MONSTERTID_BEGIN, DND_TOXICBLOOD))
		return;

	int prev = ActivatorTID();
	if(!SetActivator(pnum + P_TIDSTART))
		return;

	// Poison, DoT and spell duration are one increased pool; the spell's duration gems multiply after.
	int dur = GetPoisonDuration(pnum, (tenths << 16) / 10, PlayerModData[pnum].vals[PSTAT_SPELL_DURATION]);
	dur = ApplySpellGemMore(pnum, spell, GEMSTAT_MORE_DURATION, dur) << 1;
	bool first = !CheckActorInventory(victim, "DnD_PoisonStacks");
	int extra = GainPoisonStacks(pnum, victim);

	ACS_NamedExecuteWithResult("DnD Do Poison Damage", victim, tick, DND_SPELL_POISON_SOURCE, dur | first);
	while(extra-- > 0)
		ACS_NamedExecuteWithResult("DnD Do Poison Damage", victim, tick, DND_SPELL_POISON_SOURCE, dur);

	if(prev)
		SetActivator(prev);
}

// ---- Shrapnel ----

#define DND_SHRAPNEL_SPEED 40
#define DND_SHRAPNEL_LIFE 70           // tics in flight
#define DND_SHRAPNEL_R10_SHARDS 3
#define DND_SHRAPNEL_UNEARTH 40.0      // how far ahead of the caster the shard comes out of the ground

void CastShrapnel(int pnum) {
	int n = (SpellThresholdMet(pnum, SPL_SHRAPNEL, DND_SPELL_THRESH_HIGH) ? DND_SHRAPNEL_R10_SHARDS : 1) + GetSpellExtraProjectiles(pnum, SPL_SHRAPNEL);
	PlaySound(pnum + P_TIDSTART, "Shrapnel/Cast", CHAN_6);
	for(int i = 0; i < n; ++i)
		SpawnSpellProjectile(pnum, SPL_SHRAPNEL, "Spell_Shrapnel", DND_SHRAPNEL_SPEED, 0, FanOffset(i, DND_SPELL_FANSPREAD));
	ACS_NamedExecuteAlways("DnD Earth Unearth FX", 0, pnum, DND_SHRAPNEL_UNEARTH >> 16, 0);
}

Script "DnD Shrapnel Setup" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum != -1) {
		SetSpellRipLimit(pnum, 0, Max(1, GetSpellValue(pnum, SPL_SHRAPNEL, SPELLVAL_AMOUNT)));
		if(SpellThresholdMet(pnum, SPL_SHRAPNEL, DND_SPELL_THRESH_LOW))
			GiveInventory("DnD_RipThruHardened", 1);
	}

	SetActorProperty(0, APROP_REACTIONTIME, DND_SHRAPNEL_LIFE);
	SetResultValue(0);
}

// Client side: the ground breaking dist units ahead of the caster where a rock was torn out; big adds a dust burst.
Script "DnD Earth Unearth FX" (int pnum, int dist, int big) CLIENTSIDE {
	int ptid = pnum + P_TIDSTART, a = GetActorAngle(ptid);
	int x = GetActorX(ptid) + dist * cos(a), y = GetActorY(ptid) + dist * sin(a), z = GetFloorZAt(x, y);
	Spawn("Spell_Shrapnel_Unearth", x, y, z);
	for(int i = 0; i < big; ++i)
		Spawn("Spell_Rockslide_Dust", x + random(-12.0, 12.0), y + random(-12.0, 12.0), z + 4.0);
}

// ---- Rockslide ----

#define DND_ROCKSLIDE_SPEED 20.0       // across the ground, a tic
#define DND_ROCKSLIDE_R5_SPEED 30.0    // 50% faster from rank 5
#define DND_ROCKSLIDE_GRAVITY 0.35     // matches the actor's own Gravity
#define DND_ROCKSLIDE_RANGE 1024.0
#define DND_ROCKSLIDE_MINRANGE 64.0
#define DND_ROCKSLIDE_STUN 52          // tics: 1.5 seconds, 2.5 at rank 10
#define DND_ROCKSLIDE_R10_STUN 87
#define DND_ROCKSLIDE_UNEARTH 80

// Tossed in an arc that comes down on the enemy under the crosshair, or on the spot aimed at.
Script "DnD Rockslide" (int pnum) {
	int target = BeginSpellSpot(pnum, DND_ROCKSLIDE_RANGE);
	for(int w = 0; !target && !ReadSpellAim(pnum) && w < DND_SPELLAIM_WAIT; ++w)
		Delay(const:1);
	if(target < 0 || (!target && !SpellAimReady(pnum)) || !PlayerInGame(pnum))
		Terminate;

	auto aim = GetSpellAim();
	int ptid = pnum + P_TIDSTART;
	int dx = aim.x[pnum] - GetActorX(ptid), dy = aim.y[pnum] - GetActorY(ptid);
	int d = Max(VectorLength(dx, dy), DND_ROCKSLIDE_MINRANGE);
	int rise = (target ? GetActorZ(target) + GetActorProperty(target, APROP_HEIGHT) / 2 : aim.z[pnum]) -
		(GetActorZ(ptid) + GetActorViewHeight(ptid) - 15.0);

	// Across at a set speed, so the flight takes d / speed tics; the climb that lands it there under gravity follows.
	int across = SpellThresholdMet(pnum, SPL_ROCKSLIDE, DND_SPELL_THRESH_LOW) ? DND_ROCKSLIDE_R5_SPEED : DND_ROCKSLIDE_SPEED;
	int t = FixedDiv(d, across);
	int up = FixedDiv(rise, t) + FixedMul(DND_ROCKSLIDE_GRAVITY >> 1, t);
	int speed = VectorLength(across, up) >> 16;
	int yaw = VectorAngle(dx, dy) - GetActorAngle(ptid), pitch = -VectorAngle(across, up);

	PlaySound(ptid, "Rockslide/Cast", CHAN_6);
	int n = 1 + GetSpellExtraProjectiles(pnum, SPL_ROCKSLIDE);
	for(int i = 0; i < n; ++i)
		SpawnSpellProjectile(pnum, SPL_ROCKSLIDE, "Spell_Rockslide", speed, DND_ATF_USEGRAVITY, yaw + FanOffset(i, DND_SPELL_FANSPREAD), 0, 0, 0, true,
			DND_SPELLPROJ_FROMVIEW, pitch);
	ACS_NamedExecuteAlways("DnD Earth Unearth FX", 0, pnum, DND_ROCKSLIDE_UNEARTH, 2);
}

// Run from the boulder's burst: stuns every enemy its blast reached, in sight of where it landed.
Script "DnD Rockslide Stun" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	int r = GetUserVariable(0, "user_expradius") << 16;
	int tics = ScaleSpellEffectTics(pnum, SPL_ROCKSLIDE,
		SpellThresholdMet(pnum, SPL_ROCKSLIDE, DND_SPELL_THRESH_HIGH) ? DND_ROCKSLIDE_R10_STUN : DND_ROCKSLIDE_STUN);
	int x = GetActorX(0), y = GetActorY(0), z = GetActorZ(0), i, m, reach;
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(!IsActorAlive(m) || !IsSpellEnemy(m))
			continue;
		reach = r + GetActorProperty(m, APROP_RADIUS);
		if(abs(GetActorX(m) - x) > reach || abs(GetActorY(m) - y) > reach || VectorLength(GetActorX(m) - x, GetActorY(m) - y) > reach)
			continue;
		if(z < GetActorZ(m) - r || z > GetActorZ(m) + GetActorProperty(m, APROP_HEIGHT) + r || !CheckSight(0, m, 0))
			continue;
		StunMonster(m, tics);
	}
	SetResultValue(0);
}

// ---- Blockade ----

#define DND_BLOCKADE_RANGE 384.0        // the farthest ahead the wall rises
#define DND_BLOCKADE_MINDIST 48.0       // and the nearest, clear of the caster
#define DND_BLOCKADE_BACKOFF 40.0       // pulled back this far when the aim ended on a wall or a body
#define DND_BLOCKADE_STEP 16.0          // between its pieces; each is 20 across, so nothing fits between
#define DND_BLOCKADE_ROCKSTEP 24.0      // between the debris bursts of a broken wall
#define DND_BLOCKADE_SLABW 48.0         // a drawn slab's width at scale 1 (BLKWALL*.md3)
#define DND_BLOCKADE_SLABS 4            // slab models, in the order their plates continue
#define DND_BLOCKADE_MAXSLABS 48
#define DND_BLOCKADE_SLABPAD 16.0       // the drawn wall runs this much past the pieces' centres, over their radius
#define DND_BLOCKADE_RISE 8             // tics the slabs take to come up
#define DND_BLOCKADE_SINK 12            // and to go back down when it lapses
#define DND_BLOCKADE_DEPTH 100.0        // how far under the floor they start, the slabs' full height
#define DND_BLOCKADE_LEAN 0.02          // the slabs lean back toward the caster by this turn fraction
#define DND_BLOCKADE_SEEALPHA 0.5       // from rank 5 the rocks are see-through, as shots are
#define DND_BLOCKADE_SHARDS 8           // rank 10: Shrapnels across the wall when it breaks
#define DND_BLOCKADE_SHARDZ 32.0

typedef struct {
	int hp[MAXPLAYERS];         // the wall's health, shared by every piece
	int len[MAXPLAYERS];
	int serial[MAXPLAYERS];     // bumped by every cast, so the older wall's watcher ends
} blockade_T;

blockade_T module& GetBlockades() {
	static blockade_T s;
	return s;
}

// A monster's hit on any piece of the wall, taken off the shared pool.
void HitBlockade(int pnum, int dmg) {
	GetBlockades().hp[pnum] -= dmg;
}

// Takes the wall down. Broken, it bursts; rank 10 also sends Shrapnels out across its length.
void EndBlockade(int pnum, bool broken) {
	int anchor = DND_BLOCKADE_ANCHOR_TID + pnum, len = GetBlockades().len[pnum];
	Thing_Remove(DND_BLOCKADE_TID + pnum);
	if(broken) {
		ACS_NamedExecuteAlways("DnD Blockade Break FX", 0, PackMapPair(GetActorX(anchor), GetActorY(anchor)),
			((GetActorZ(anchor) >> 16) & 0xFFFF) | (len & 0xFFFF0000), (GetActorAngle(anchor) & 0xFFFF) | (pnum << 16));
		if(SpellThresholdMet(pnum, SPL_BLOCKADE, DND_SPELL_THRESH_HIGH)) {
			int n = DND_BLOCKADE_SHARDS + GetSpellExtraProjectiles(pnum, SPL_BLOCKADE);
			for(int i = 0; i < n; ++i)
				SpawnSpellProjectile(pnum, SPL_SHRAPNEL, "Spell_Shrapnel", DND_SHRAPNEL_SPEED, 0, 0,
					len / (2 * n) * (2 * i + 1) - len / 2, 0, anchor, true, DND_BLOCKADE_SHARDZ);
		}
	}
	Thing_Remove(anchor);
}

// Raised across the caster's facing where the aim ends; one wall a caster, a new one replacing the old.
Script "DnD Blockade" (int pnum) {
	TraceSpellAim(pnum, DND_BLOCKADE_RANGE);
	for(int w = 0; !ReadSpellAim(pnum) && w < DND_SPELLAIM_WAIT; ++w)
		Delay(const:1);
	if(!SpellAimReady(pnum) || !PlayerInGame(pnum))
		Terminate;

	auto aim = GetSpellAim();
	auto bl = GetBlockades();
	int ptid = pnum + P_TIDSTART, a = GetActorAngle(ptid), ca = cos(a), sa = sin(a);
	int d = FixedMul(aim.x[pnum] - GetActorX(ptid), ca) + FixedMul(aim.y[pnum] - GetActorY(ptid), sa);
	if(aim.z[pnum] - GetFloorZAt(aim.x[pnum], aim.y[pnum]) > 2.0)
		d -= DND_BLOCKADE_BACKOFF;
	d = Clamp_Between(d, DND_BLOCKADE_MINDIST, DND_BLOCKADE_RANGE);
	int cx = GetActorX(ptid) + FixedMul(d, ca), cy = GetActorY(ptid) + FixedMul(d, sa);

	if(bl.serial[pnum])
		EndBlockade(pnum, false);
	int serial = ++bl.serial[pnum];
	int tid = DND_BLOCKADE_TID + pnum, anchor = DND_BLOCKADE_ANCHOR_TID + pnum;
	if(!SpawnForced("Spell_Blockade_Anchor", cx, cy, GetFloorZAt(cx, cy), anchor))
		Terminate;
	SetActorAngle(anchor, a);
	if(!CheckSight(ptid, anchor, CSF_NOBLOCKALL)) {
		Thing_Remove(anchor);
		Terminate;
	}

	// Pieces pass each other by species, so only a body or a wall stops one; out of the anchor's sight is past a corner.
	int len = ScaleSpellArea(pnum, SPL_BLOCKADE, GetSpellValue(pnum, SPL_BLOCKADE, SPELLVAL_AMOUNT) << 16);
	int n = len / DND_BLOCKADE_STEP + 1, i, off, x, y, s;
	for(i = 0; i < n; ++i) {
		off = i * DND_BLOCKADE_STEP - len / 2;
		x = cx - FixedMul(sa, off);
		y = cy + FixedMul(ca, off);
		s = ScratchTID();
		if(!s || !Spawn("Spell_Blockade_Segment", x, y, GetFloorZAt(x, y), s, a >> 8))
			continue;
		if(CheckSight(anchor, s, CSF_NOBLOCKALL))
			Thing_ChangeTID(s, tid);
		else
			Thing_Remove(s);
	}
	// Rank 5: friendly lets player shots through (sv_shootthroughallies), the pets' species lets theirs through.
	bool through = SpellThresholdMet(pnum, SPL_BLOCKADE, DND_SPELL_THRESH_LOW);
	if(through) {
		SetActorProperty(tid, APROP_FRIENDLY, true);
		SetActorProperty(tid, APROP_SPECIES, "Player");
	}

	bl.hp[pnum] = GetSpellValue(pnum, SPL_BLOCKADE, SPELLVAL_DAMAGE);
	bl.len[pnum] = len;
	PlaySound(anchor, "Blockade/Rise", CHAN_BODY);
	ACS_NamedExecuteAlways("DnD Blockade FX", 0, pnum, len >> 16, through);

	int life = GetSpellDurationTics(pnum, SPL_BLOCKADE);
	for(int t = 0; t < life && bl.hp[pnum] > 0 && PlayerInGame(pnum); ++t) {
		Delay(const:1);
		if(bl.serial[pnum] != serial)
			Terminate;
	}
	EndBlockade(pnum, bl.hp[pnum] <= 0 && PlayerInGame(pnum));
}

typedef struct {
	int seq;						// bumped by each FX script, so an anchor's token names the one drawing its rocks
	int broke[MAXPLAYERS];			// the Timer() a player's wall broke at, so its rocks vanish rather than sink
} blockade_fx_T;

blockade_fx_T module& GetBlockadeFX() {
	static blockade_fx_T s;
	return s;
}

// Client side: the rocks rise along the wall, stand while its anchor does, and sink when it lapses or vanish when it breaks.
Script "DnD Blockade FX" (int pnum, int len, int through) CLIENTSIDE {
	int anchor = DND_BLOCKADE_ANCHOR_TID + pnum;
	int rocks = ScratchTID();
	if(!rocks || !ThingCount(T_NONE, anchor))
		Terminate;
	int me = ++GetBlockadeFX().seq;
	SetActorInventory(anchor, "DnD_BlockadeFX", me);

	int a = GetActorAngle(anchor), cx = GetActorX(anchor), cy = GetActorY(anchor);
	// Slabs laid along +y in their cyclic order, so the plates run on across every joint, stretched to fill the length.
	len = (len << 16) + DND_BLOCKADE_SLABPAD;
	int n = Clamp_Between((len + DND_BLOCKADE_SLABW / 2) / DND_BLOCKADE_SLABW, 2, DND_BLOCKADE_MAXSLABS);
	int step = len / n, phase = random(0, DND_BLOCKADE_SLABS - 1), i, off, x, y, r;
	for(i = 0; i < n; ++i) {
		off = step * i + step / 2 - len / 2;
		x = cx - FixedMul(sin(a), off);
		y = cy + FixedMul(cos(a), off);
		if(!(r = ScratchTID()) || !SpawnForced(StrParam(s:"Spell_Blockade_Slab", d:(phase + i) % DND_BLOCKADE_SLABS + 1), x, y, GetFloorZAt(x, y) - DND_BLOCKADE_DEPTH, r))
			continue;
		SetActorAngle(r, a);
		SetActorPitch(r, DND_BLOCKADE_LEAN);
		if(!i)
			SetActorState(r, "First");
		else if(i == n - 1)
			SetActorState(r, "Last");
		SetActorProperty(r, APROP_SCALEX, FixedDiv(step, DND_BLOCKADE_SLABW));
		if(through) {
			SetActorProperty(r, APROP_RENDERSTYLE, STYLE_TRANSLUCENT);
			SetActorProperty(r, APROP_ALPHA, DND_BLOCKADE_SEEALPHA);
		}
		Thing_ChangeTID(r, rocks);
	}

	SetActorVelocity(rocks, 0, 0, DND_BLOCKADE_DEPTH / DND_BLOCKADE_RISE, false, false);
	Delay(const:DND_BLOCKADE_RISE);
	SetActorVelocity(rocks, 0, 0, 0, false, false);
	while(ThingCount(T_NONE, anchor) && CheckActorInventory(anchor, "DnD_BlockadeFX") == me)
		Delay(const:4);

	// The break script runs a tic behind the anchor's removal.
	Delay(const:1);
	if(GetBlockadeFX().broke[pnum] >= Timer() - 4) {
		Thing_Remove(rocks);
		Terminate;
	}
	SetActorVelocity(rocks, 0, 0, -DND_BLOCKADE_DEPTH / DND_BLOCKADE_SINK, false, false);
	Delay(const:DND_BLOCKADE_SINK);
	Thing_Remove(rocks);
}

// Client side: chunks and dust burst along a broken wall.
Script "DnD Blockade Break FX" (int xy, int zlen, int ap) CLIENTSIDE {
	int cx = xy << 16, cy = xy & 0xFFFF0000, z = zlen << 16, len = zlen & 0xFFFF0000, a = ap & 0xFFFF, off, x, y, i;
	GetBlockadeFX().broke[ap >> 16] = Timer();
	for(off = -len / 2; off <= len / 2; off += DND_BLOCKADE_ROCKSTEP) {
		x = cx - FixedMul(sin(a), off);
		y = cy + FixedMul(cos(a), off);
		for(i = 0; i < 3; ++i)
			Spawn("Spell_RockArmor_Chunk", x + random(-8.0, 8.0), y + random(-8.0, 8.0), z + random(16.0, 64.0));
		Spawn("Spell_Rockslide_Dust", x, y, z + random(8.0, 40.0));
	}
	i = ScratchTID();
	if(i && Spawn("Spell_Rockslide_Dust", cx, cy, z + 32.0, i)) {
		PlaySound(i, "Blockade/Break", CHAN_BODY);
		Thing_ChangeTID(i, 0);
	}
}

// ---- Toxic Trap ----

#define DND_TOXICTRAP_SLOTS 4          // the most a caster has out: AMOUNT is 2, 4 from rank 5
#define DND_TOXICTRAP_AHEAD 40.0       // laid this far in front of the caster
#define DND_TOXICTRAP_REACH 24.0       // an enemy's body this close sets it off
#define DND_TOXICTRAP_LIFE 30          // seconds a trap waits; 15 more at rank 10
#define DND_TOXICTRAP_R10_LIFE 15
#define DND_TOXICTRAP_CHECK 4          // tics between looks for an enemy
#define DND_TOXICTRAP_TICK 9           // tics between gas ticks, about 0.25 seconds
#define DND_TOXICTRAP_CLOUDS 5         // one in the middle, four round it
#define DND_TOXICTRAP_SPREAD 48.0      // how far the outer clouds sit from the middle
#define DND_TOXICTRAP_GASHEIGHT 64.0   // how high the gas reaches off the floor
#define DND_TOXICTRAP_MAXPOISONED 64  // enemies one burst can poison

typedef struct {
	int order[MAXPLAYERS * DND_TOXICTRAP_SLOTS];	// 0 for a free slot, else when it was laid
	int laid[MAXPLAYERS];
} toxic_trap_T;

toxic_trap_T module& GetToxicTraps() {
	static toxic_trap_T s;
	return s;
}

// Frees a trap slot, its watcher sees the change and ends; the trap springs open if it fired, else fades.
void ClearToxicTrap(int k, bool fired) {
	int tid = DND_TOXICTRAP_TID + k;
	GetToxicTraps().order[k] = 0;
	SetActorState(tid, fired ? "Open" : "Fade");
	Thing_ChangeTID(tid, 0);
}

// Lays a trap ahead, or at the caster's feet when the spot is out of sight; the oldest gives way when all are out.
void CastToxicTrap(int pnum) {
	auto tt = GetToxicTraps();
	int most = Clamp_Between(GetSpellValue(pnum, SPL_TOXICTRAP, SPELLVAL_AMOUNT), 1, DND_TOXICTRAP_SLOTS);
	int i, k, n, oldest, free, base = pnum * DND_TOXICTRAP_SLOTS;
	do {
		n = 0;
		oldest = -1;
		free = -1;
		for(i = 0; i < DND_TOXICTRAP_SLOTS; ++i) {
			k = base + i;
			if(!tt.order[k]) {
				if(free == -1)
					free = k;
			}
			else {
				++n;
				if(oldest == -1 || tt.order[k] < tt.order[oldest])
					oldest = k;
			}
		}
		if(n >= most || free == -1)
			ClearToxicTrap(oldest, false);
	} while(n >= most || free == -1);

	int ptid = pnum + P_TIDSTART, a = GetActorAngle(ptid), tid = DND_TOXICTRAP_TID + free;
	int x = GetActorX(ptid) + FixedMul(DND_TOXICTRAP_AHEAD, cos(a)), y = GetActorY(ptid) + FixedMul(DND_TOXICTRAP_AHEAD, sin(a));
	if(!SpawnForced("Spell_ToxicTrap", x, y, GetFloorZAt(x, y), tid, a >> 8))
		return;
	if(!CheckSight(ptid, tid, CSF_NOBLOCKALL))
		SetActorPosition(tid, GetActorX(ptid), GetActorY(ptid), GetActorFloorZ(ptid), false);

	int order = ++tt.laid[pnum];
	tt.order[free] = order;
	PlaySound(ptid, "ToxicTrap/Lay", CHAN_6);
	ACS_NamedExecuteAlways("DnD Toxic Trap", 0, pnum, free, order);
}

// Watches one trap until an enemy touches it, it runs out, a newer trap takes its slot or the caster leaves.
Script "DnD Toxic Trap" (int pnum, int k, int order) {
	auto tt = GetToxicTraps();
	int tid = DND_TOXICTRAP_TID + k;
	int life = DND_TOXICTRAP_LIFE + (SpellThresholdMet(pnum, SPL_TOXICTRAP, DND_SPELL_THRESH_HIGH) ? DND_TOXICTRAP_R10_LIFE : 0);
	life = ScaleSpellEffectTics(pnum, SPL_TOXICTRAP, life * TICRATE);
	int x = GetActorX(tid), y = GetActorY(tid), z = GetActorZ(tid), t, i, m, reach, dx, dy;

	for(t = 0; t < life; t += DND_TOXICTRAP_CHECK) {
		Delay(const:DND_TOXICTRAP_CHECK);
		if(tt.order[k] != order)
			Terminate;
		if(!PlayerInGame(pnum))
			break;

		for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
			m = UsedMonsterTIDs[i];
			if(!IsActorAlive(m) || !IsSpellEnemy(m) || GetActorZ(m) > z + 32.0 || GetActorZ(m) + GetActorProperty(m, APROP_HEIGHT) < z)
				continue;
			reach = (DND_TOXICTRAP_REACH + GetActorProperty(m, APROP_RADIUS)) >> 16;
			dx = (GetActorX(m) - x) >> 16;
			dy = (GetActorY(m) - y) >> 16;
			if(dx * dx + dy * dy <= reach * reach) {
				ClearToxicTrap(k, true);
				ACS_NamedExecuteAlways("DnD Toxic Trap Gas", 0, pnum, PackMapPair(x, y), z >> 16);
				Terminate;
			}
		}
	}
	ClearToxicTrap(k, false);
}

// The burst: a cluster of clouds that ticks every enemy inside any of them once, every DND_TOXICTRAP_TICK.
Script "DnD Toxic Trap Gas" (int pnum, int xy, int zz) {
	int ptid = pnum + P_TIDSTART;
	SetActivator(ptid);
	int x = xy << 16, y = xy & 0xFFFF0000, z = zz << 16;
	int dur = GetSpellDurationTics(pnum, SPL_TOXICTRAP);
	int r = ScaleSpellArea(pnum, SPL_TOXICTRAP, GetSpellValue(pnum, SPL_TOXICTRAP, SPELLVAL_RADIUS) << 16);
	int a0 = random(0, 1.0), a, k, t, i, m, reach, dx, dy, dmg, dealt;
	int cx[DND_TOXICTRAP_CLOUDS], cy[DND_TOXICTRAP_CLOUDS];
	cx[0] = x;
	cy[0] = y;
	for(k = 1; k < DND_TOXICTRAP_CLOUDS; ++k) {
		a = a0 + (k - 1) * 1.0 / (DND_TOXICTRAP_CLOUDS - 1);
		cx[k] = x + FixedMul(DND_TOXICTRAP_SPREAD, cos(a));
		cy[k] = y + FixedMul(DND_TOXICTRAP_SPREAD, sin(a));
	}

	ACS_NamedExecuteAlways("DnD Toxic Trap Gas FX", 0, xy, (zz & 0xFFFF) | (r & 0xFFFF0000), Min(dur, 0xFFFF) | ((a0 >> 8) << 16));
	int probe = SpawnSpellProbe(x, y, z + 32.0);
	int poisoned[DND_TOXICTRAP_MAXPOISONED], np = 0, j;

	for(t = 0; t < dur && PlayerInGame(pnum); t += DND_TOXICTRAP_TICK) {
		// A tick, so the DoT pile prices it; read live, as Blaze's burn is.
		dmg = PriceSpellDamage(pnum, SPL_TOXICTRAP, GetSpellValue(pnum, SPL_TOXICTRAP, SPELLVAL_DAMAGE), DND_WDMG_ISRADIUSDMG, true);
		for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
			m = UsedMonsterTIDs[i];
			if(!IsActorAlive(m) || !IsSpellEnemy(m) || GetActorZ(m) > z + DND_TOXICTRAP_GASHEIGHT || GetActorZ(m) + GetActorProperty(m, APROP_HEIGHT) < z)
				continue;
			reach = (r + GetActorProperty(m, APROP_RADIUS)) >> 16;
			for(k = 0; k < DND_TOXICTRAP_CLOUDS; ++k) {
				dx = (GetActorX(m) - cx[k]) >> 16;
				dy = (GetActorY(m) - cy[k]) >> 16;
				if(dx * dx + dy * dy <= reach * reach)
					break;
			}
			if(k == DND_TOXICTRAP_CLOUDS || (probe && !CheckSight(probe, m, CSF_NOBLOCKALL)))
				continue;

			dealt = HandleDamageDeal(ptid, m, dmg, DND_DAMAGETYPE_POISON, SPL_TOXICTRAP,
				DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISRADIUSDMG | DND_DAMAGEFLAG_NOPOISONSTACK | DND_DAMAGEFLAG_NOPUSH,
				0, 0, 0, DND_ACTORFLAG_ISDAMAGEOVERTIME | DND_ACTORFLAG_PAINLESS, true);
			if(dealt > 0)
				Thing_Damage2(m, dealt, "Special_NoPain");

			// One poison per enemy per trap, from the first tick that reaches it.
			j = 0;
			while(j < np && poisoned[j] != m)
				++j;
			if(j == np && np < DND_TOXICTRAP_MAXPOISONED) {
				poisoned[np++] = m;
				PoisonFromSpellHit(pnum, SPL_TOXICTRAP, m, dealt, GetSpellValue(pnum, SPL_TOXICTRAP, SPELLVAL_AMOUNT2));
			}
		}
		Delay(const:DND_TOXICTRAP_TICK);
	}
	if(probe)
		Thing_Remove(probe);
}

// Client side: the burst, then gas puffs welling up across the clouds for as long as the gas lasts.
Script "DnD Toxic Trap Gas FX" (int xy, int zr, int da) CLIENTSIDE {
	int x = xy << 16, y = xy & 0xFFFF0000, z = ((zr << 16) >> 16) << 16, r = zr & 0xFFFF0000;
	int dur = da & 0xFFFF, a0 = (da >> 16) << 8, t, k, cx, cy, a, d;
	Spawn("Spell_ToxicTrap_Burst", x, y, z + 8.0);
	for(t = 0; t < dur; t += 3) {
		k = (t / 3) % DND_TOXICTRAP_CLOUDS;
		cx = x;
		cy = y;
		if(k) {
			a = a0 + (k - 1) * 1.0 / (DND_TOXICTRAP_CLOUDS - 1);
			cx += FixedMul(DND_TOXICTRAP_SPREAD, cos(a));
			cy += FixedMul(DND_TOXICTRAP_SPREAD, sin(a));
		}
		a = random(0, 1.0);
		d = FixedMul(r, random(0, 0.6));
		Spawn("Spell_ToxicTrap_Gas", cx + FixedMul(d, cos(a)), cy + FixedMul(d, sin(a)), z + random(4.0, 28.0));
		Delay(const:3);
	}
}

// ---- Baleful Infusion ----

#define DND_BALEFUL_R5_PEN 20
#define DND_BALEFUL_KILLWINDOW 70      // tics after the hit that a kill still spreads its poison (rank 10)
#define DND_BALEFUL_SPREAD 192.0
#define DND_BALEFUL_FXEVERY 3
#define DND_BALEFUL_CLOUDS 7            // spread FX clouds at a 96 unit radius, more as it grows

int[] module& GetBalefulFXTickets() {
	static int tickets[MAXPLAYERS];
	return tickets;
}

// Arms the next melee hit; a recast restarts the window.
void CastBalefulInfusion(int pnum) {
	int ptid = pnum + P_TIDSTART;
	int tics = GetSpellDurationTics(pnum, SPL_BALEFULINFUSION);
	GiveActorInventory(ptid, "DnD_BalefulTicket", 1);
	SetActorInventory(ptid, "DnD_BalefulReady", 1);
	SetActorInventory(ptid, "DnD_BalefulSpending", 0);
	SetActorInventory(ptid, "P_BalefulTime", (tics + TICRATE - 1) / TICRATE);
	PlaySound(ptid, "BalefulInfusion/Cast", CHAN_6);
	ACS_NamedExecuteAlways("DnD Baleful FX", 0, pnum, tics);
	ACS_NamedExecuteAlways("DnD Baleful Infusion", 0, pnum, CheckActorInventory(ptid, "DnD_BalefulTicket"), tics);
}

// Counts the window down; a spend or a recast takes it over.
Script "DnD Baleful Infusion" (int pnum, int ticket, int tics) {
	int ptid = pnum + P_TIDSTART;
	for(int t = tics; t > 0 && ticket == CheckActorInventory(ptid, "DnD_BalefulTicket") && CheckActorInventory(ptid, "DnD_BalefulReady") &&
		PlayerInGame(pnum) && IsActorAlive(ptid); --t) {
		int s = (t + TICRATE - 1) / TICRATE;
		if(s != CheckActorInventory(ptid, "P_BalefulTime"))
			SetActorInventory(ptid, "P_BalefulTime", s);
		Delay(const:1);
	}

	if(ticket == CheckActorInventory(ptid, "DnD_BalefulTicket") && !CheckActorInventory(ptid, "DnD_BalefulSpending")) {
		SetActorInventory(ptid, "DnD_BalefulReady", 0);
		SetActorInventory(ptid, "P_BalefulTime", 0);
		ACS_NamedExecuteAlways("DnD Baleful FX", 0, pnum, 0);
	}
}

// The share of a melee hit Baleful Infusion turns to poison while armed: never past what the weapon's own conversion left.
int GetBalefulShare(int pnum, int wepid, int category) {
	if(category == DND_DAMAGECATEGORY_POISON || !IsMeleeWeapon(wepid) || !CheckActorInventory(pnum + P_TIDSTART, "DnD_BalefulReady"))
		return 0;
	return Clamp_Between(GetSpellValue(pnum, SPL_BALEFULINFUSION, SPELLVAL_DAMAGE), 0, 100 - GetPlayerConversionRowTotal(pnum, category));
}

// That share as poison, priced as a conversion is: it keeps the increases and more of both its type and poison.
int GetBalefulHit(int pnum, int slot, int dmgid, int wepid, int category, int base) {
	int pct = GetBalefulShare(pnum, wepid, category);
	if(!pct || base <= 0)
		return 0;
	int inc = 100 + GetCachedPlayerIncreased(pnum, slot, dmgid) + GetPlayerBuffIncreasedDamage(pnum) + GetLiveWeaponPerkBonus(pnum) +
		GetPlayerAccuracyDamageBonus(pnum, wepid) + GetCachedPlayerIncreasedTyped(pnum, slot, dmgid, category) +
		GetCachedPlayerIncreasedTyped(pnum, slot, dmgid, DND_DAMAGECATEGORY_POISON);
	int more = CombinePackedMultipliers(CombinePackedMultipliers(GetCachedPlayerMorePacked(pnum, slot, dmgid), CombinePackedMultipliers(
		GetCachedPlayerMorePackedTyped(pnum, slot, dmgid, category), GetCachedPlayerMorePackedTyped(pnum, slot, dmgid, DND_DAMAGECATEGORY_POISON))),
		GetLiveWeaponPerkMorePacked(pnum, wepid));
	int factor = MulPercent_Round(ApplyPackedMultiplier(inc, more) * DND_COMPFACTOR_SCALE, pct);
	return factor > 0 ? MulPercent_Round(base, factor, 100 * DND_COMPFACTOR_SCALE) : 0;
}

// The first converted hit spends it; every hit landing on that same tic still converts.
void SpendBaleful(int pnum) {
	int ptid = pnum + P_TIDSTART;
	if(CheckActorInventory(ptid, "DnD_BalefulSpending"))
		return;
	SetActorInventory(ptid, "DnD_BalefulSpending", 1);
	ACS_NamedExecuteAlways("DnD Baleful Spend", 0, pnum);
}

Script "DnD Baleful Spend" (int pnum) {
	int ptid = pnum + P_TIDSTART;
	PlaySound(ptid, "BalefulInfusion/Spend", CHAN_7);
	Delay(const:1);
	SetActorInventory(ptid, "DnD_BalefulReady", 0);
	SetActorInventory(ptid, "DnD_BalefulSpending", 0);
	SetActorInventory(ptid, "P_BalefulTime", 0);
	ACS_NamedExecuteAlways("DnD Baleful FX", 0, pnum, 0);
}

// Rank 5: the enchanted hit's poison pierces poison resistance.
int GetBalefulPenetration(int pnum, int wepid) {
	if(!IsMeleeWeapon(wepid) || !CheckActorInventory(pnum + P_TIDSTART, "DnD_BalefulSpending") || !SpellThresholdMet(pnum, SPL_BALEFULINFUSION, DND_SPELL_THRESH_LOW))
		return 0;
	return DND_BALEFUL_R5_PEN;
}

// An enemy the enchanted hit's poison reached: its poison runs longer, and a kill soon after spreads it (rank 10).
// tick stands in for the poison's own tick until the poison route sets the real one.
void MarkBalefulVictim(int pnum, int victim, int tick) {
	SetActorInventory(victim, "DnD_BalefulOwner", pnum + 1);
	SetActorInventory(victim, "DnD_BalefulUntil", Timer() + DND_BALEFUL_KILLWINDOW);
	SetActorInventory(victim, "DnD_BalefulFresh", 1);
	SetActorInventory(victim, "DnD_BalefulPoison", tick);
}

// The player's poison duration with the spell's bonus, as "DnD Do Poison Damage" takes it: fixed seconds, shifted up one.
int GetBalefulPoisonDuration(int pnum) {
	int base = DND_BASE_POISON_TIMER + (GetSpellValue(pnum, SPL_BALEFULINFUSION, SPELLVAL_AMOUNT2) << 16) / 10;
	int dur = GetPoisonDuration(pnum, base, PlayerModData[pnum].vals[PSTAT_SPELL_DURATION]);
	return ApplySpellGemMore(pnum, SPL_BALEFULINFUSION, GEMSTAT_MORE_DURATION, dur) << 1;
}

// Called where a hit's poison is applied: the enchanted hit's own poison takes the spell's duration, and its tick is kept for a spread.
int TakeBalefulPoison(int pnum, int victim, int tick) {
	if(!CheckActorInventory(victim, "DnD_BalefulFresh") || CheckActorInventory(victim, "DnD_BalefulOwner") != pnum + 1)
		return 0;
	SetActorInventory(victim, "DnD_BalefulFresh", 0);
	SetActorInventory(victim, "DnD_BalefulPoison", tick);
	return GetBalefulPoisonDuration(pnum);
}

// Rank 10: an enemy killed soon after the enchanted hit passes that hit's poison to every enemy around it.
Script "DnD Baleful Spread" (int pnum, int dead) {
	if(!PlayerInGame(pnum) || !SpellThresholdMet(pnum, SPL_BALEFULINFUSION, DND_SPELL_THRESH_HIGH))
		Terminate;
	SetActivator(pnum + P_TIDSTART);

	int tick = CheckActorInventory(dead, "DnD_BalefulPoison"), dur = GetBalefulPoisonDuration(pnum);
	int x = GetActorX(dead), y = GetActorY(dead), z = GetActorZ(dead);
	int r = ScaleSpellArea(pnum, SPL_BALEFULINFUSION, DND_BALEFUL_SPREAD);
	int probe = SpawnSpellProbe(x, y, z + 32.0), i, m, first, extra;
	ACS_NamedExecuteAlways("DnD Baleful Spread FX", 0, PackMapPair(x, y), z >> 16, r >> 16);
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(!InSpellArea(probe, m, x, y, r) || !CheckAilmentImmunity(pnum, m - DND_MONSTERTID_BEGIN, DND_TOXICBLOOD))
			continue;
		first = !CheckActorInventory(m, "DnD_PoisonStacks");
		extra = GainPoisonStacks(pnum, m);
		ACS_NamedExecuteWithResult("DnD Do Poison Damage", m, tick, -1, first | dur);
		while(extra-- > 0)
			ACS_NamedExecuteWithResult("DnD Do Poison Damage", m, tick, -1, dur);
	}
	if(probe)
		Thing_Remove(probe);
}

// Client side: poison welling round the caster's hands while armed; tics 0 stops it.
Script "DnD Baleful FX" (int pnum, int tics) CLIENTSIDE {
	int[] module& tickets = GetBalefulFXTickets();
	int ticket = ++tickets[pnum], ptid = pnum + P_TIDSTART, t, a, h, side;
	for(t = 0; t < tics && ticket == tickets[pnum] && IsActorAlive(ptid); t += DND_BALEFUL_FXEVERY) {
		a = GetActorAngle(ptid);
		side = random(-14.0, 14.0);
		h = GetActorViewHeight(ptid) - random(18.0, 26.0);
		Spawn("Spell_Baleful_Mote", GetActorX(ptid) + 22 * cos(a) - FixedMul(side, sin(a)), GetActorY(ptid) + 22 * sin(a) + FixedMul(side, cos(a)),
			GetActorZ(ptid) + h);
		Delay(const:DND_BALEFUL_FXEVERY);
	}
}

// Client side: the poison bursting off the dead enemy, as wide as the spread.
Script "DnD Baleful Spread FX" (int xy, int z, int r) CLIENTSIDE {
	int x = xy << 16, y = xy & 0xFFFF0000, n = DND_BALEFUL_CLOUDS * r / 96, a, d;
	z <<= 16;
	r <<= 16;
	Spawn("Spell_NoxiousBlast_Burst", x, y, z);
	for(int i = 0; i < n; ++i) {
		a = random(0, 1.0);
		d = FixedMul(r, random(0.25, 0.85));
		Spawn("Spell_NoxiousBlast_Cloud", x + FixedMul(d, cos(a)), y + FixedMul(d, sin(a)), z + random(4.0, 16.0));
	}
}

// ---- Noxious Blast ----

#define DND_NOXIOUSBLAST_RANGE 512.0
#define DND_NOXIOUSBLAST_DELAY 14      // tics from the cast to the burst
#define DND_NOXIOUSBLAST_CLOUDS 7      // burst clouds at the base radius, more as it grows
#define DND_NOXIOUSBLAST_FLOORSNAP 32.0 // a spot this close over its floor bursts on the floor
#define DND_NOXIOUSBLAST_HEADROOM 24.0  // one in the air stays at least this far under the ceiling
#define DND_NOXIOUSBLAST_DISC 15.156    // map units per unit of the ring's scale: flat2's 15.625 by the skin's rim
#define DND_NOXIOUSBLAST_SMALLEST 0.2   // the ring's size at the burst, a share of the radius it starts at

// Marks the spot, then bursts there and poisons every enemy caught.
Script "DnD Noxious Blast" (int pnum) {
	int target = BeginSpellSpot(pnum, DND_NOXIOUSBLAST_RANGE);
	for(int w = 0; !target && !ReadSpellAim(pnum) && w < DND_SPELLAIM_WAIT; ++w)
		Delay(const:1);
	if(target < 0 || (!target && !SpellAimReady(pnum)) || !PlayerInGame(pnum))
		Terminate;

	auto aim = GetSpellAim();
	int x = aim.x[pnum], y = aim.y[pnum], z = aim.z[pnum], floor = GetFloorZAt(x, y);
	// On the floor when the spot is near it, else in the air where aimed: an airborne target at its middle.
	bool grounded = z - floor <= DND_NOXIOUSBLAST_FLOORSNAP;
	if(grounded)
		z = floor;
	else {
		if(target)
			z += GetActorProperty(target, APROP_HEIGHT) / 2;
		z = Max(floor, Min(z, GetCeilingZAt(x, y) - DND_NOXIOUSBLAST_HEADROOM));
	}
	int r = ScaleSpellArea(pnum, SPL_NOXIOUSBLAST, GetSpellValue(pnum, SPL_NOXIOUSBLAST, SPELLVAL_RADIUS) << 16);

	PlaySound(pnum + P_TIDSTART, "NoxiousBlast/Cast", CHAN_6);
	ACS_NamedExecuteAlways("DnD Noxious Blast FX", 0, PackMapPair(x, y), z >> 16, r >> 16);
	Delay(const:DND_NOXIOUSBLAST_DELAY);
	if(!PlayerInGame(pnum))
		Terminate;

	int per_sec = GetSpellValue(pnum, SPL_NOXIOUSBLAST, SPELLVAL_DAMAGE);
	int tenths = GetSpellValue(pnum, SPL_NOXIOUSBLAST, SPELLVAL_AMOUNT2);
	int probe = SpawnSpellProbe(x, y, grounded ? z + 32.0 : z), i, m;
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(InSpellArea(probe, m, x, y, r) && InSpellHeight(m, z, r))
			ApplySpellPoison(pnum, SPL_NOXIOUSBLAST, m, per_sec, tenths);
	}
	if(probe)
		Thing_Remove(probe);
}

// Client side: a poison ring over the blast's whole radius closing on the spot through the delay, poison drawn in off
// its edge; at its smallest, the burst filling the radius.
Script "DnD Noxious Blast FX" (int xy, int z, int r) CLIENTSIDE {
	int x = xy << 16, y = xy & 0xFFFF0000, t, a, d, px, py, tid, p, size;
	z <<= 16;
	r <<= 16;

	int ring = ScratchTID();
	if(ring && !SpawnForced("Spell_NoxiousBlast_Ring", x, y, z, ring))
		ring = 0;
	for(t = 0; t < DND_NOXIOUSBLAST_DELAY; ++t) {
		// Closing quicker and quicker; the shader reads the progress off ScaleY.
		p = t * 1.0 / (DND_NOXIOUSBLAST_DELAY - 1);
		size = FixedMul(r, 1.0 - FixedMul(1.0 - DND_NOXIOUSBLAST_SMALLEST, FixedMul(p, p)));
		if(ring) {
			SetActorProperty(ring, APROP_SCALEX, FixedDiv(size, DND_NOXIOUSBLAST_DISC));
			SetActorProperty(ring, APROP_SCALEY, 1.0 + p);
		}
		if(!(t % 2) && (tid = ScratchTID())) {
			a = random(0, 1.0);
			d = FixedMul(size, random(0.8, 1.0));
			px = x + FixedMul(d, cos(a));
			py = y + FixedMul(d, sin(a));
			if(SpawnForced("Spell_NoxiousBlast_Mote", px, py, z + random(2.0, 10.0), tid)) {
				SetActorVelocity(tid, (x - px) / 16, (y - py) / 16, random(0.4, 1.2), false, false);
				Thing_ChangeTID(tid, 0);
			}
		}
		Delay(const:1);
	}

	if(ring)
		Thing_Remove(ring);
	Spawn("Spell_NoxiousBlast_Burst", x, y, z);
	int n = DND_NOXIOUSBLAST_CLOUDS * r / (96 << 16);
	for(t = 0; t < n; ++t) {
		a = random(0, 1.0);
		d = FixedMul(r, random(0.25, 0.85));
		Spawn("Spell_NoxiousBlast_Cloud", x + FixedMul(d, cos(a)), y + FixedMul(d, sin(a)), z + random(4.0, 16.0));
	}
}

// ---- Vines of Torment ----

#define DND_VINES_REACH 512             // how far the lash runs; 100% farther at rank 10
#define DND_VINES_HALFWIDTH 48.0        // half its width before area gear; 25% more at rank 5
#define DND_VINES_SPEED 24              // units a tic at AMOUNT 100
#define DND_VINES_LIFE 88               // tics a stretch stands after it sprouts: 2.5 seconds
#define DND_VINES_START 32              // how far ahead of the caster it breaks the ground
#define DND_VINES_STEP 16               // the probes' stride while tracing the path
#define DND_VINES_CLIMB 256.0           // the tallest single rise the vines climb
#define DND_VINES_DROP 256.0            // and the deepest drop they follow down
#define DND_VINES_REACHUP 64.0          // how far over its floor a body can be and still be caught
#define DND_VINES_CHECK 3               // tics between looks for who touches it
#define DND_VINES_MAXTOUCH 96           // enemies one lash remembers

// How far the lash runs from (x, y) along a on the floor, stopped by a wall, a rise or drop past its limits or lost sight.
// The leapfrogging probes stand at the higher floor for the sight test, so a ledge's edge never hides one from the other.
int TraceVinesPath(int x, int y, int a, int reach) {
	int fz = GetFloorZAt(x, y), p0 = ScratchTID();
	if(!p0 || !SpawnForced("Spell_Vines_Probe", x, y, fz, p0))
		return 0;
	int p1 = ScratchTID();
	if(!p1 || !SpawnForced("Spell_Vines_Probe", x, y, fz, p1)) {
		Thing_Remove(p0);
		return 0;
	}

	int d, px = x, py = y, nx, ny, nz, hz;
	for(d = DND_VINES_STEP; d <= reach; d += DND_VINES_STEP) {
		nx = x + d * cos(a);
		ny = y + d * sin(a);
		nz = GetFloorZAt(nx, ny);
		if(nz - fz > DND_VINES_CLIMB || fz - nz > DND_VINES_DROP)
			break;
		hz = Max(fz, nz);
		if(!SetActorPosition(p0, px, py, hz, false) || !SetActorPosition(p1, nx, ny, hz, false) || !CheckSight(p0, p1, CSF_NOBLOCKALL))
			break;
		px = nx;
		py = ny;
		fz = nz;
	}
	Thing_Remove(p0);
	Thing_Remove(p1);
	return d - DND_VINES_STEP;
}

// Whether a body overlaps the live stretch [lo, hi] of a lash from (x, y) along (ca, sa), half wide, low enough and in sight.
bool VinesTouches(int m, int x, int y, int ca, int sa, int lo, int hi, int half, int probe) {
	int dx = (GetActorX(m) - x) >> 16, dy = (GetActorY(m) - y) >> 16, pad = hi + DND_SPELLAREA_BODYPAD;
	if(!probe || dx > pad || dx < -pad || dy > pad || dy < -pad)
		return false;

	int u = (dx * ca + dy * sa) >> 16, v = (dy * ca - dx * sa) >> 16, r = GetActorProperty(m, APROP_RADIUS) >> 16;
	if(u < lo - r || u > hi + r || v > half + r || v < -half - r)
		return false;
	if(GetActorZ(m) - GetFloorZAt(GetActorX(m), GetActorY(m)) > DND_VINES_REACHUP)
		return false;

	u = Clamp_Between(u, lo, hi);
	int px = x + u * ca, py = y + u * sa;
	SetActorPosition(probe, px, py, GetFloorZAt(px, py) + 16.0, false);
	return CheckSight(probe, m, CSF_NOBLOCKALL);
}

// A flat heal on another player, as a health pickup of that amount with their healing gear.
void HealSpellAlly(int who, int amt) {
	int prev = ActivatorTID();
	if(!SetActivator(who + P_TIDSTART))
		return;
	HandleHealthPickup(amt, 0, 0, true, true);
	if(prev)
		SetActivator(prev);
}

// The lash grows from the caster's feet at its speed; each stretch stands DND_VINES_LIFE tics, with duration gear. Enemies it touches are
// hit and snared once, allies healed once.
Script "DnD Vines Of Torment" (int pnum) {
	int ptid = pnum + P_TIDSTART, a = GetActorAngle(ptid), ca = cos(a), sa = sin(a);
	int x = GetActorX(ptid), y = GetActorY(ptid);
	bool far = SpellThresholdMet(pnum, SPL_VINESOFTORMENT, DND_SPELL_THRESH_HIGH);
	int len = TraceVinesPath(x, y, a, far ? DND_VINES_REACH * 2 : DND_VINES_REACH);
	PlaySound(ptid, "Vines/Cast", CHAN_6);
	if(len <= DND_VINES_START)
		Terminate;

	int half = DND_VINES_HALFWIDTH;
	if(SpellThresholdMet(pnum, SPL_VINESOFTORMENT, DND_SPELL_THRESH_LOW))
		half = half * 5 / 4;
	half = ScaleSpellArea(pnum, SPL_VINESOFTORMENT, half) >> 16;
	int speed = Max(1, DND_VINES_SPEED * GetSpellValue(pnum, SPL_VINESOFTORMENT, SPELLVAL_AMOUNT) / 100);
	int snare = GetSpellDurationTics(pnum, SPL_VINESOFTORMENT);
	// The ally heal takes more-healing gems, as Rejuvenation does.
	int heal = ApplySpellGemMore(pnum, SPL_VINESOFTORMENT, GEMSTAT_MORE_HEALING, GetSpellValue(pnum, SPL_VINESOFTORMENT, SPELLVAL_DAMAGE2));
	int life = ScaleSpellEffectTics(pnum, SPL_VINESOFTORMENT, DND_VINES_LIFE);
	ACS_NamedExecuteAlways("DnD Vines FX", 0, PackMapPair(x, y), (a & 0xFFFF) | (Min(speed, 255) << 16) | (Min(life / 4, 255) << 24),
		len | (half << 16));

	int touched[DND_VINES_MAXTOUCH], n = 0;
	bool healed[MAXPLAYERS];
	int probe = SpawnSpellProbe(x, y, GetFloorZAt(x, y) + 16.0), t, i, k, m, lo, hi;
	bool seen;
	for(t = 0; PlayerInGame(pnum); ++t) {
		hi = Min(len, DND_VINES_START + (t + 1) * speed);
		lo = Max(DND_VINES_START, DND_VINES_START + (t - life) * speed);
		if(lo >= hi)
			break;

		if(!(t % DND_VINES_CHECK)) {
			for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER] && n < DND_VINES_MAXTOUCH; ++i) {
				m = UsedMonsterTIDs[i];
				if(!VinesTouches(m, x, y, ca, sa, lo, hi, half, probe) || !IsActorAlive(m) || !IsSpellEnemy(m))
					continue;
				for(k = 0, seen = false; k < n && !seen; ++k)
					seen = touched[k] == m;
				if(seen)
					continue;
				touched[n++] = m;
				DealSpellBeamHit(pnum, SPL_VINESOFTORMENT, m, 100);
				if(IsActorAlive(m) && !UltimatumMonstersUnstoppable()) {
					SnareMonster(m, snare);
					ACS_NamedExecuteAlways("DnD Vines Snare FX", 0, m, snare);
				}
			}
			for(i = 0; i < MAXPLAYERS; ++i) {
				if(i != pnum && !healed[i] && PlayerInGame(i) && IsActorAlive(i + P_TIDSTART) &&
					VinesTouches(i + P_TIDSTART, x, y, ca, sa, lo, hi, half, probe)) {
					healed[i] = true;
					HealSpellAlly(i, heal);
				}
			}
		}
		Delay(const:1);
	}
	if(probe)
		Thing_Remove(probe);
}

#define DND_VINES_COILGAP 44            // units between rows of coils along the lash
#define DND_VINES_COILREACH 30          // a coil's half length, and half its width below: no end may hang over a drop
#define DND_VINES_COILSIDE 14
#define DND_VINES_COILLEVEL 24.0        // how far a spot beside the line may sit off the line's floor
#define DND_VINES_COILSPAN 48.0         // the most floor height a coil's footprint may span, two stair steps
#define DND_VINES_COILSINK 20.0         // how deep a coil starts, rising to the floor in 8 tics
#define DND_VINES_NOFLOOR 0x7FFFFFFF

// The floor a coil at (px, py) along (ca, sa) stands on: the lowest under its middle, ends and flanks, so none of it
// hangs in the air and what lies over higher floor is buried in it. DND_VINES_NOFLOOR if the footprint spans too much.
int VinesCoilFloor(int px, int py, int ca, int sa) {
	int i, s, f, lo = GetFloorZAt(px, py), hi = lo;
	for(i = 0; i < 4; ++i) {
		s = (i & 1) ? 1 : -1;
		f = GetFloorZAt(px + ((i < 2) ? s * DND_VINES_COILREACH * ca : -s * DND_VINES_COILSIDE * sa),
			py + ((i < 2) ? s * DND_VINES_COILREACH * sa : s * DND_VINES_COILSIDE * ca));
		lo = Min(lo, f);
		hi = Max(hi, f);
	}
	return hi - lo > DND_VINES_COILSPAN ? DND_VINES_NOFLOOR : lo;
}

// One of the three coil shapes at random, its axis under the floor, standing life tics (counted in 2 tic steps).
void SpawnVinesCoil(int px, int py, int z, int ang, int life) {
	int tid = ScratchTID(), k = random(1, 3);
	if(!tid)
		return;
	z -= DND_VINES_COILSINK;
	if(k == 1)
		k = SpawnForced("Spell_Vines_Coil1", px, py, z, tid);
	else if(k == 2)
		k = SpawnForced("Spell_Vines_Coil2", px, py, z, tid);
	else
		k = SpawnForced("Spell_Vines_Coil3", px, py, z, tid);
	if(k) {
		SetActorAngle(tid, ang);
		SetActorProperty(tid, APROP_REACTIONTIME, life / 2);
		Thing_ChangeTID(tid, 0);
	}
}

// Client side: rows of thorny coils turning up through the floor as the lash runs, each standing its stretch's life
// (carried in 4 tic steps). A spot beside the line off its floor, out of its sight or too close to a wall moves onto it.
Script "DnD Vines FX" (int xy, int as, int lw) CLIENTSIDE {
	int x = xy << 16, y = xy & 0xFFFF0000, a = as & 0xFFFF, speed = (as >> 16) & 0xFF, life = ((as >> 24) & 0xFF) * 4;
	int len = lw & 0xFFFF, half = lw >> 16, ca = cos(a), sa = sin(a), n = Max(1, half / 24), next = DND_VINES_START;
	int d, k, off, cx, cy, cz, px, py, pz, c0 = ScratchTID(), c1 = 0;
	if(c0 && Spawn("Spell_Vines_FXProbe", x, y, GetFloorZAt(x, y), c0) && (c1 = ScratchTID()))
		Spawn("Spell_Vines_FXProbe", x, y, GetFloorZAt(x, y), c1);
	for(d = DND_VINES_START; d < len; d += speed) {
		for(; next <= d && next < len; next += DND_VINES_COILGAP) {
			cx = x + next * ca;
			cy = y + next * sa;
			cz = GetFloorZAt(cx, cy);
			for(k = 0; k < n; ++k) {
				off = (2 * k + 1 - n) * half / n + random(-8, 8);
				px = cx - off * sa;
				py = cy + off * ca;
				pz = GetFloorZAt(px, py);
				if(!c1 || abs(pz - cz) > DND_VINES_COILLEVEL || !SetActorPosition(c0, cx, cy, cz + 8.0, false) ||
					!SetActorPosition(c1, px, py, pz + 8.0, false) || !CheckSight(c0, c1, CSF_NOBLOCKALL) ||
					(pz = VinesCoilFloor(px, py, ca, sa)) == DND_VINES_NOFLOOR) {
					px = cx;
					py = cy;
					if((pz = VinesCoilFloor(px, py, ca, sa)) == DND_VINES_NOFLOOR)
						continue;
				}
				SpawnVinesCoil(px, py, pz, a + random(-0.07, 0.07), life);
			}
		}
		px = x + d * ca;
		py = y + d * sa;
		Spawn("Spell_Shrapnel_Dust", px, py, GetFloorZAt(px, py) + 4.0);
		Delay(const:1);
	}
	if(c0)
		Thing_Remove(c0);
	if(c1)
		Thing_Remove(c1);
}

#define DND_VINES_WRAPR 16.0           // the wrap model's body radius and climb at scale 1
#define DND_VINES_WRAPH 40.0
#define DND_VINES_WRAPGIRTH 95          // percent of the enemy's radius the vines hug
#define DND_VINES_WRAPCLIMB 70          // percent of its height they climb

// Client side: vines grown up round a snared enemy until the snare ends; a snare on one already wrapped extends it.
Script "DnD Vines Snare FX" (int m, int tics) CLIENTSIDE {
	int left = CheckActorInventory(m, "DnD_VinesWrapFX"), w;
	if(left) {
		if(left < tics)
			SetActorInventory(m, "DnD_VinesWrapFX", tics);
		Terminate;
	}
	if(!(w = ScratchTID()) || !SpawnForced("Spell_Vines_Wrap", GetActorX(m), GetActorY(m), GetActorZ(m), w, random(0, 255)))
		Terminate;
	SetActorProperty(w, APROP_SCALEX, FixedDiv(GetActorProperty(m, APROP_RADIUS) * DND_VINES_WRAPGIRTH / 100, DND_VINES_WRAPR));
	SetActorProperty(w, APROP_SCALEY, FixedDiv(GetActorProperty(m, APROP_HEIGHT) * DND_VINES_WRAPCLIMB / 100, DND_VINES_WRAPH));
	SetActorInventory(m, "DnD_VinesWrapFX", tics);
	while((left = CheckActorInventory(m, "DnD_VinesWrapFX")) > 0 && IsActorAlive(m)) {
		SetActorInventory(m, "DnD_VinesWrapFX", left - 1);
		SetActorPosition(w, GetActorX(m), GetActorY(m), GetActorZ(m), false);
		Delay(const:1);
	}
	SetActorInventory(m, "DnD_VinesWrapFX", 0);
	SetActorState(w, "Wither");
	Thing_ChangeTID(w, 0);
}

// ---- Venom Gyre ----

#define DND_VENOMGYRE_SPEED 20
#define DND_VENOMGYRE_OUT 175           // tics out before it turns back: 5 seconds, before duration gear
#define DND_VENOMGYRE_BACK 140          // tics it chases its caster before it is lost
#define DND_VENOMGYRE_CATCH 40.0        // how close it must come back to be caught
#define DND_VENOMGYRE_POISONCHANCE 50
#define DND_VENOMGYRE_MORE 10           // percent more poison damage for a catch
#define DND_VENOMGYRE_MORETICS 175      // and for how long, before duration gear
#define DND_VENOMGYRE_FRESHDELAY 10     // tics before a dash catch throws a fresh one: 0.3 seconds
#define DND_VENOMGYRE_R10_EXTRA 2

void CastVenomGyre(int pnum) {
	int n = 1 + DND_VENOMGYRE_R10_EXTRA * SpellThresholdMet(pnum, SPL_VENOMGYRE, DND_SPELL_THRESH_HIGH) + GetSpellExtraProjectiles(pnum, SPL_VENOMGYRE);
	PlaySound(pnum + P_TIDSTART, "VenomGyre/Throw", CHAN_6);
	for(int i = 0; i < n; ++i)
		SpawnSpellProjectile(pnum, SPL_VENOMGYRE, "Spell_VenomGyre", DND_VENOMGYRE_SPEED, 0, FanOffset(i, DND_SPELL_FANSPREAD));
}

// Each gyre gets the rip count and a scratch tid its watcher follows; the watcher turns it back.
Script "DnD Venom Gyre Setup" (void) {
	int pnum = GetSpellActorOwner(), tid = ScratchTID();
	SetResultValue(0);
	if(pnum == -1 || !tid)
		Terminate;
	SetSpellRipLimit(pnum, 0, GetSpellValue(pnum, SPL_VENOMGYRE, SPELLVAL_AMOUNT));
	Thing_ChangeTID(0, tid);
	ACS_NamedExecuteAlways("DnD Venom Gyre Out", 0, pnum, tid);
}

// Out of rips or out of time, it turns for home.
Script "DnD Venom Gyre Out" (int pnum, int tid) {
	int out = ScaleSpellEffectTics(pnum, SPL_VENOMGYRE, DND_VENOMGYRE_OUT);
	for(int t = 0; ThingCount(T_NONE, tid); ++t) {
		if(t >= out || CheckActorInventory(tid, "DnD_RipCount") >= CheckActorInventory(tid, "DnD_RipLimit")) {
			TurnVenomGyre(pnum, tid);
			Terminate;
		}
		Delay(const:1);
	}
}

// Its Death state: something stopped it before the watcher did.
Script "DnD Venom Gyre Turn" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum != -1)
		TurnVenomGyre(pnum, ActivatorTID());
	SetResultValue(0);
}

// The outgoing gyre gives way to a homing one: a fresh ripper, so each enemy can be hit once more, with no rip limit.
void TurnVenomGyre(int pnum, int tid) {
	if(!tid || CheckActorInventory(tid, "DnD_GyreTurned"))
		return;
	GiveActorInventory(tid, "DnD_GyreTurned", 1);

	// Kept inside the room: one that turned at a sky or ceiling would touch it again on its first move home.
	int rt = ScratchTID(), x = GetActorX(tid), y = GetActorY(tid);
	int z = Clamp_Between(GetActorZ(tid), GetActorFloorZ(tid), GetActorCeilingZ(tid) - GetActorProperty(tid, APROP_HEIGHT) - 1.0);
	Thing_Remove(tid);
	if(!rt || !PlayerInGame(pnum) || !SpawnForced("Spell_VenomGyre_Return", x, y, z, rt))
		return;
	SetupSpellActor(rt, pnum, SPL_VENOMGYRE);
	SetSpellRipLimit(pnum, rt, MAX_RIPCOUNT);
	ACS_NamedExecuteAlways("DnD Venom Gyre Back", 0, pnum, rt);
}

// Steers it at the caster's chest each tic; close enough is a catch, too long is lost.
Script "DnD Venom Gyre Back" (int pnum, int rt) {
	int ptid = pnum + P_TIDSTART, t, dx, dy, dz, d;
	for(t = 0; t < DND_VENOMGYRE_BACK && ThingCount(T_NONE, rt) && PlayerInGame(pnum) && IsActorAlive(ptid); ++t) {
		dx = GetActorX(ptid) - GetActorX(rt);
		dy = GetActorY(ptid) - GetActorY(rt);
		dz = GetActorZ(ptid) + 32.0 - GetActorZ(rt);
		d = VectorLength(VectorLength(dx, dy), dz);
		if(d <= DND_VENOMGYRE_CATCH) {
			CatchVenomGyre(pnum, rt);
			Terminate;
		}
		SetActorVelocity(rt, DND_VENOMGYRE_SPEED * FixedDiv(dx, d), DND_VENOMGYRE_SPEED * FixedDiv(dy, d),
			DND_VENOMGYRE_SPEED * FixedDiv(dz, d), false, false);
		// Facing its flight, which the model's tilt reads to tell climbing from falling.
		SetActorAngle(rt, VectorAngle(dx, dy));
		Delay(const:1);
	}
	if(ThingCount(T_NONE, rt))
		Thing_Remove(rt);
}

// A plain catch is more poison damage for a while; a catch during a dash throws a fresh gyre at double damage.
void CatchVenomGyre(int pnum, int rt) {
	int ptid = pnum + P_TIDSTART;
	Thing_Remove(rt);
	PlaySound(ptid, "VenomGyre/Catch", CHAN_7);
	if(CheckActorInventory(ptid, "DashEffect"))
		ACS_NamedExecuteAlways("DnD Venom Gyre Fresh", 0, pnum);
	else
		HandlePlayerBuffAssignment(pnum, ptid, BTI_SPELL_VENOMGYRE, 0, 0,
			ScaleSpellEffectTics(pnum, SPL_VENOMGYRE, DND_VENOMGYRE_MORETICS), DND_VENOMGYRE_MORE);
}

Script "DnD Venom Gyre Fresh" (int pnum) {
	Delay(const:DND_VENOMGYRE_FRESHDELAY);
	if(!PlayerInGame(pnum) || !IsActorAlive(pnum + P_TIDSTART))
		Terminate;
	PlaySound(pnum + P_TIDSTART, "VenomGyre/Throw", CHAN_6);
	SpawnSpellProjectile(pnum, SPL_VENOMGYRE, "Spell_VenomGyre_Fresh", DND_VENOMGYRE_SPEED);
}

// The fresh gyre: the rip count of a normal throw, at double damage.
Script "DnD Venom Gyre Fresh Setup" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum != -1)
		SetSpellRipLimit(pnum, 0, GetSpellValue(pnum, SPL_VENOMGYRE, SPELLVAL_AMOUNT));
	SetUserVariable(0, "user_dmgmult", 2);
	SetResultValue(0);
}

// Venom Gyre's own poison, rolled on each enemy it hits in place of a poison hit's usual one.
void RollVenomGyrePoison(int pnum, int victim) {
	if(random(1, 100) > DND_VENOMGYRE_POISONCHANCE)
		return;
	int tenths = Max(1, GetSpellValue(pnum, SPL_VENOMGYRE, SPELLVAL_AMOUNT2));
	ApplySpellPoison(pnum, SPL_VENOMGYRE, victim, GetSpellValue(pnum, SPL_VENOMGYRE, SPELLVAL_DAMAGE2) * 10 / tenths, tenths);
}

// ---- Rock Armor ----

#define DND_ROCKARMOR_SHARDS 8          // Shrapnels thrown around the caster when rank 10's armor shatters
#define DND_ROCKARMOR_SHARDZ 32.0       // and how high off the feet they leave
#define DND_ROCKARMOR_OWNALPHA 0.25     // how strong the wearer sees its own plates
#define DND_ROCKARMOR_FADE 8            // tics the plates take to come up, and to go when the armor lapses
#define DND_ROCKARMOR_RING 30.0         // the plates' distance from the wearer, RKPLATE.md3's

typedef struct {
	int seq;		// bumped by each FX script, so the wearer's token names the one drawing its plates
} rock_armor_fx_T;

rock_armor_fx_T module& GetRockArmorFX() {
	static rock_armor_fx_T s;
	return s;
}

void CastRockArmor(int pnum) {
	int ptid = pnum + P_TIDSTART, tics = GetSpellDurationTics(pnum, SPL_ROCKARMOR);
	int pts = Max(1, GetSpellValue(pnum, SPL_ROCKARMOR, SPELLVAL_DAMAGE));
	GiveActorInventory(ptid, "DnD_RockArmorTicket", 1);
	SetActorInventory(ptid, "DnD_RockArmor", pts);
	SetActorInventory(ptid, "P_RockArmor", pts);
	PlaySound(ptid, "RockArmor/Cast", CHAN_6);
	ACS_NamedExecuteAlways("DnD Rock Armor FX", 0, pnum, tics);
	ACS_NamedExecuteAlways("DnD Rock Armor", 0, pnum, CheckActorInventory(ptid, "DnD_RockArmorTicket"), tics);
}

// Lapses at the end of its time unless a recast took over or a hit already broke it.
Script "DnD Rock Armor" (int pnum, int ticket, int tics) {
	int ptid = pnum + P_TIDSTART;
	for(int t = tics; t > 0 && ticket == CheckActorInventory(ptid, "DnD_RockArmorTicket") && CheckActorInventory(ptid, "DnD_RockArmor") &&
		PlayerInGame(pnum) && IsActorAlive(ptid); --t)
		Delay(const:1);

	if(ticket == CheckActorInventory(ptid, "DnD_RockArmorTicket")) {
		SetActorInventory(ptid, "DnD_RockArmor", 0);
		SetActorInventory(ptid, "P_RockArmor", 0);
	}
}

// The layer outside the energy shield, on the player taking a hit: soaks it whole, a magic one only half. Returns the rest.
int AbsorbRockArmor(int pnum, int dmg, bool magical) {
	int pts = CheckInventory("DnD_RockArmor");
	if(!pts || dmg <= 0)
		return dmg;

	int take = Min(magical ? dmg / 2 : dmg, pts);
	pts -= take;
	SetInventory("DnD_RockArmor", pts);
	SetInventory("P_RockArmor", pts);
	if(!pts)
		ACS_NamedExecuteAlways("DnD Rock Armor Shatter", 0, pnum);
	else if(take > 0)
		LocalAmbientSound("RockArmor/Hit", 90);
	return dmg - take;
}

// Broken: the burst, and at rank 10 a ring of Shrapnels at the caster's own Shrapnel rank.
Script "DnD Rock Armor Shatter" (int pnum) {
	int ptid = pnum + P_TIDSTART;
	PlaySound(ptid, "RockArmor/Shatter", CHAN_7);
	ACS_NamedExecuteAlways("DnD Rock Armor Shatter FX", 0, pnum);
	if(SpellThresholdMet(pnum, SPL_ROCKARMOR, DND_SPELL_THRESH_HIGH) && IsActorAlive(ptid))
		for(int i = 0; i < DND_ROCKARMOR_SHARDS; ++i)
			SpawnSpellProjectile(pnum, SPL_SHRAPNEL, "Spell_Shrapnel", DND_SHRAPNEL_SPEED, 0, i * 1.0 / DND_ROCKARMOR_SHARDS,
				0, 0, 0, true, DND_ROCKARMOR_SHARDZ);
}

// Client side: two stone plates circling the wearer while the armor holds, grit shed off them; faint to the wearer
// itself. A recast's script takes over, a break shatters them, a lapse fades them.
Script "DnD Rock Armor FX" (int pnum, int tics) CLIENTSIDE {
	int ptid = pnum + P_TIDSTART, me = ++GetRockArmorFX().seq, tid = UniqueTID(), t, a;
	int peak = ConsolePlayerNumber() == pnum ? DND_ROCKARMOR_OWNALPHA : 1.0;
	SetActorInventory(ptid, "DnD_RockArmorFX", me);
	Spawn("Spell_Shrapnel_Unearth", GetActorX(ptid), GetActorY(ptid), GetActorZ(ptid));
	if(!tid || !SpawnForced("Spell_RockArmor_Plates", GetActorX(ptid), GetActorY(ptid), GetActorZ(ptid), tid, 0))
		Terminate;
	for(t = 0; t < tics && IsActorAlive(ptid) && CheckActorInventory(ptid, "DnD_RockArmorFX") == me; ++t) {
		if(ConsolePlayerNumber() == pnum && !CheckActorInventory(ptid, "P_RockArmor"))
			break;
		// Steered onto where the wearer will be next tic, so the plates keep up smoothly.
		SetActorVelocity(tid, GetActorX(ptid) + GetActorVelX(ptid) - GetActorX(tid), GetActorY(ptid) + GetActorVelY(ptid) - GetActorY(tid),
			GetActorZ(ptid) + GetActorVelZ(ptid) - GetActorZ(tid), false, false);
		SetActorProperty(tid, APROP_ALPHA, FixedMul(peak, Min(1.0, t * 1.0 / DND_ROCKARMOR_FADE)));
		if(!(t & 7)) {
			a = random(0, 1.0);
			Spawn("Spell_RockArmor_Pebble", GetActorX(ptid) + FixedMul(DND_ROCKARMOR_RING, cos(a)), GetActorY(ptid) + FixedMul(DND_ROCKARMOR_RING, sin(a)),
				GetActorZ(ptid) + random(20.0, 44.0));
		}
		Delay(const:1);
	}

	// Shattered: gone at once, in pieces. Lapsed or replaced: they fade where they are. A break's signal comes a few tics
	// after the points run out, so it is waited for.
	for(t = 0; t < 4 && CheckActorInventory(ptid, "DnD_RockArmorFX"); ++t)
		Delay(const:1);
	if(!CheckActorInventory(ptid, "DnD_RockArmorFX")) {
		for(t = 0; t < 8; ++t) {
			a = random(0, 1.0);
			Spawn("Spell_RockArmor_Chunk", GetActorX(tid) + FixedMul(DND_ROCKARMOR_RING, cos(a)), GetActorY(tid) + FixedMul(DND_ROCKARMOR_RING, sin(a)),
				GetActorZ(tid) + random(20.0, 44.0));
		}
		Thing_Remove(tid);
		Terminate;
	}
	SetActorVelocity(tid, 0, 0, 0, false, false);
	a = GetActorProperty(tid, APROP_ALPHA);
	for(t = DND_ROCKARMOR_FADE; t > 0; --t) {
		SetActorProperty(tid, APROP_ALPHA, a * t / DND_ROCKARMOR_FADE);
		Delay(const:1);
	}
	Thing_Remove(tid);
}

Script "DnD Rock Armor Shatter FX" (int pnum) CLIENTSIDE {
	int ptid = pnum + P_TIDSTART;
	SetActorInventory(ptid, "DnD_RockArmorFX", 0);
	for(int i = 0; i < 10; ++i)
		Spawn("Spell_RockArmor_Chunk", GetActorX(ptid) + random(-16.0, 16.0), GetActorY(ptid) + random(-16.0, 16.0), GetActorZ(ptid) + random(16.0, 48.0));
}

// ---- Rejuvenation ----

#define DND_REJUV_PULSE 17              // tics between heals; few and large, so the target's healing bonus survives rounding
#define DND_REJUV_FADEIN 8              // tics the sphere takes to come up
#define DND_REJUV_FADEOUT 12            // and to go once the heal ends
#define DND_REJUV_OWNALPHA 0.2          // how strong the target sees its own sphere, from inside it
#define DND_REJUV_RADIUS 34.0           // the sphere's radius in REJUVSPH.md3; bigger pets get a bigger one

// A pet's health back up to the ceiling it was summoned with; pets carry no healing bonuses of their own.
void HealSpellPet(int tid, int amt) {
	int hp = GetActorProperty(tid, APROP_HEALTH), cap = PetMonsterProperties[tid - DND_PETTID_BEGIN].maxhp;
	if(amt > 0 && hp < cap)
		SetActorProperty(tid, APROP_HEALTH, Min(cap, hp + amt));
}

// Every ailment lane a pet, being a monster, can carry.
void CurePetAilments(int tid) {
	SetActorInventory(tid, "DnD_AilmentToken", 0);
	SetActorInventory(tid, "DnD_PoisonStacks", 0);
	SetActorInventory(tid, "DnD_ChillStacks", 0);
	SetActorInventory(tid, "DnD_FreezeTimer", 0);
	SetActorInventory(tid, "DnD_SlowPercent", 0);
	SetActorInventory(tid, "DnD_SlowTimer", 0);
	SetActorInventory(tid, "StunDurationCounter", 0);
	SetActorInventory(tid, "DnD_Shocked", 0);
}

// The locked ally, a player or a pet, or the caster recovers DAMAGE over DURATION. A ticket on the target makes a new
// cast replace the old.
void CastRejuvenation(int pnum) {
	int target = GetSpellLockTarget(pnum), ttid = target >= 0 ? GetSpellLockTid(target) : 0;
	if(!ttid || !IsActorAlive(ttid) || (target < DND_SPELLLOCK_PET && !PlayerInGame(target)))
		ttid = pnum + P_TIDSTART;

	int total = ApplySpellGemMore(pnum, SPL_REJUVENATION, GEMSTAT_MORE_HEALING, GetSpellValue(pnum, SPL_REJUVENATION, SPELLVAL_DAMAGE));
	int pulses = Max(1, GetSpellDurationTics(pnum, SPL_REJUVENATION) / DND_REJUV_PULSE);
	if(SpellThresholdMet(pnum, SPL_REJUVENATION, DND_SPELL_THRESH_HIGH)) {
		if(isPlayer(ttid))
			CurePlayerAilments(ttid - P_TIDSTART);
		else
			CurePetAilments(ttid);
	}

	GiveActorInventory(ttid, "DnD_RejuvTicket", 1);
	PlaySound(ttid, "Rejuvenation/Cast", CHAN_6);
	ACS_NamedExecuteAlways("DnD Rejuvenation Spell Heal", 0, ttid, total,
		pulses | (SpellThresholdMet(pnum, SPL_REJUVENATION, DND_SPELL_THRESH_LOW) << 16));
	ACS_NamedExecuteAlways("DnD Rejuvenation Spell FX", 0, ttid, pulses * DND_REJUV_PULSE);
}

// Heals in equal pulses that sum to the total exactly. Any drop in the target's health, or a player's heal pool, ends it
// until rank 5; a recast takes it over instead.
Script "DnD Rejuvenation Spell Heal" (int ttid, int total, int pf) {
	int pulses = pf & 0xFFFF, ticket = CheckActorInventory(ttid, "DnD_RejuvTicket"), pnum = ttid - P_TIDSTART;
	bool steady = !!(pf >> 16), player = isPlayer(ttid);
	int hp = GetActorProperty(ttid, APROP_HEALTH), pool = player ? GetHealPoolCur(pnum) : 0, k, t, amt;

	for(k = 1; k <= pulses; ++k) {
		for(t = 0; t < DND_REJUV_PULSE; ++t) {
			Delay(const:1);
			if(CheckActorInventory(ttid, "DnD_RejuvTicket") != ticket)
				Terminate;
			if(!IsActorAlive(ttid) || (player && !PlayerInGame(pnum)) ||
				(!steady && (GetActorProperty(ttid, APROP_HEALTH) < hp || (player && GetHealPoolCur(pnum) < pool)))) {
				ACS_NamedExecuteAlways("DnD Rejuvenation Spell FX", 0, ttid, 0);
				Terminate;
			}
			hp = GetActorProperty(ttid, APROP_HEALTH);
			pool = player ? GetHealPoolCur(pnum) : 0;
		}
		amt = total * k / pulses - total * (k - 1) / pulses;
		if(player)
			HealSpellAlly(pnum, amt);
		else
			HealSpellPet(ttid, amt);
		hp = GetActorProperty(ttid, APROP_HEALTH);
		pool = player ? GetHealPoolCur(pnum) : 0;
	}
}

// ---- Poison Nova ----

#define DND_POISONNOVA_SPEED 12         // slow enough that the ring's 4 seconds carry it about 1700 units
#define DND_POISONNOVA_Z 32.0           // how high off the feet it flies
#define DND_POISONNOVA_PITCH 0.0625     // the most the caster's aim tilts the ring up or down (22.5 degrees)

// A ring round the caster on one ripper id, so each enemy in its way is hit and poisoned once a cast; it tilts with the
// caster's aim, capped so it never flies straight up or down.
void CastPoisonNova(int pnum) {
	int n = Max(1, GetSpellValue(pnum, SPL_POISONNOVA, SPELLVAL_AMOUNT)) + GetSpellExtraProjectiles(pnum, SPL_POISONNOVA);
	int life = GetSpellDurationTics(pnum, SPL_POISONNOVA), rid = ReserveRipperId() + 1;
	int tilt = Clamp_Between(GetActorPitch(pnum + P_TIDSTART), -DND_POISONNOVA_PITCH, DND_POISONNOVA_PITCH);
	PlaySound(pnum + P_TIDSTART, "Spell/PoisonNovaCast", CHAN_6);
	for(int i = 0; i < n; ++i)
		SpawnSpellProjectile(pnum, SPL_POISONNOVA, "Spell_PoisonNova", DND_POISONNOVA_SPEED, 0, i * 1.0 / n, 0, life, 0, true,
			DND_POISONNOVA_Z, tilt, -1, rid);
}

// Its own poison on every enemy the ring meets, in place of the usual roll its NOPOISONSTACK hit skips.
void PoisonNovaPoison(int pnum, int victim) {
	int tenths = Max(1, GetSpellValue(pnum, SPL_POISONNOVA, SPELLVAL_AMOUNT2));
	ApplySpellPoison(pnum, SPL_POISONNOVA, victim, GetSpellValue(pnum, SPL_POISONNOVA, SPELLVAL_DAMAGE2) * 10 / tenths, tenths);
}

// ---- Corrosive Breath ----

#define DND_CORROSIVE_RELEASE (3 * TICRATE / 2) // tics the fumes pour out once the cast bar ends
#define DND_CORROSIVE_SPEED 16                  // units a tic the fumes' front moves out
#define DND_CORROSIVE_CHECK 3                   // tics between looks for who the fumes reach
#define DND_CORROSIVE_SLOW 25
#define DND_CORROSIVE_TAKEN 10                  // percent more poison damage taken; 25 from rank 5
#define DND_CORROSIVE_R5_TAKEN 25
#define DND_CORROSIVE_DECAY 50                  // the most of an enemy's fortification the fumes eat
#define DND_CORROSIVE_DECAYGAP 5                // tics between bites of it
#define DND_CORROSIVE_REACHUP 96.0              // how far above or below the caster a body can be and still be reached
#define DND_CORROSIVE_NEAR 32.0                 // past its radius, closer than this a body is in the cone whatever the angle
#define DND_CORROSIVE_MAXTOUCH 96

// One enemy reached: the poison, the slow, more poison damage taken, and a fortified one's fortification eaten.
void AfflictCorrosion(int pnum, int m, int per_sec, int tenths, int tics, int taken) {
	ApplySpellPoison(pnum, SPL_CORROSIVEBREATH, m, per_sec, tenths);
	if(!IsActorAlive(m))
		return;
	SlowMonster(m, DND_CORROSIVE_SLOW, tics);
	CurseMonster(m, "DnD_Corroded", "DnD_CorrodedTimer", "DnD Corroded Timer", taken, tics);
	if(CheckActorInventory(m, "MonsterFortifyCount") && !CheckActorInventory(m, "DnD_CorrosionDecay"))
		ACS_NamedExecuteAlways("DnD Corrosive Decay", 0, m, tics);
}

// The cone follows the caster's aim as the fumes pour out, its front moving out at their speed; each enemy reached is afflicted once.
Script "DnD Corrosive Breath" (int pnum) {
	int caster = pnum + P_TIDSTART;
	int reach = ScaleSpellArea(pnum, SPL_CORROSIVEBREATH, GetSpellValue(pnum, SPL_CORROSIVEBREATH, SPELLVAL_RADIUS) << 16);
	int half = GetSpellValue(pnum, SPL_CORROSIVEBREATH, SPELLVAL_AMOUNT) * 1.0 / 720, halfcos = cos(half);
	int tics = GetSpellDurationTics(pnum, SPL_CORROSIVEBREATH);
	int tenths = Max(1, GetSpellValue(pnum, SPL_CORROSIVEBREATH, SPELLVAL_AMOUNT2));
	int per_sec = GetSpellValue(pnum, SPL_CORROSIVEBREATH, SPELLVAL_DAMAGE) * 10 / tenths;
	int taken = SpellThresholdMet(pnum, SPL_CORROSIVEBREATH, DND_SPELL_THRESH_LOW) ? DND_CORROSIVE_R5_TAKEN : DND_CORROSIVE_TAKEN;

	PlaySound(caster, "CorrosiveBreath/Release", CHAN_6);
	ACS_NamedExecuteAlways("DnD Corrosive Breath FX", 0, pnum, reach >> 16,
		half | (SpellThresholdMet(pnum, SPL_CORROSIVEBREATH, DND_SPELL_THRESH_HIGH) << 16));

	int touched[DND_CORROSIVE_MAXTOUCH], n = 0, t, mn, m, k, front, dx, dy, d, a, ca, sa;
	bool seen;
	for(t = 0; t < DND_CORROSIVE_RELEASE && PlayerInGame(pnum) && IsActorAlive(caster); t += DND_CORROSIVE_CHECK) {
		front = Min(reach, (DND_CORROSIVE_SPEED * (t + DND_CORROSIVE_CHECK)) << 16);
		a = GetActorAngle(caster);
		ca = cos(a);
		sa = sin(a);
		for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER] && n < DND_CORROSIVE_MAXTOUCH; ++mn) {
			m = UsedMonsterTIDs[mn];
			if(!IsActorAlive(m) || !IsSpellEnemy(m) || abs(GetActorZ(m) - GetActorZ(caster)) > DND_CORROSIVE_REACHUP)
				continue;
			dx = GetActorX(m) - GetActorX(caster);
			dy = GetActorY(m) - GetActorY(caster);
			d = VectorLength(dx, dy);
			if(d > front + GetActorProperty(m, APROP_RADIUS))
				continue;
			if(d > GetActorProperty(m, APROP_RADIUS) + DND_CORROSIVE_NEAR && FixedDiv(FixedMul(dx, ca) + FixedMul(dy, sa), d) < halfcos)
				continue;
			for(k = 0, seen = false; k < n && !seen; ++k)
				seen = touched[k] == m;
			if(seen || !CheckSight(caster, m, CSF_NOBLOCKALL))
				continue;
			touched[n++] = m;
			AfflictCorrosion(pnum, m, per_sec, tenths, tics, taken);
		}
		Delay(const:DND_CORROSIVE_CHECK);
	}
}

// Eats up to half of a fortified enemy's fortification over the affliction, never its last point: a hit breaks that.
Script "DnD Corrosive Decay" (int m, int tics) {
	int eat = CheckActorInventory(m, "MonsterFortifyCount") * DND_CORROSIVE_DECAY / 100, steps = Max(1, tics / DND_CORROSIVE_DECAYGAP);
	if(eat <= 0)
		Terminate;
	SetActorInventory(m, "DnD_CorrosionDecay", 1);
	int k, bite, cur;
	for(k = 1; k <= steps && IsActorAlive(m); ++k) {
		Delay(const:DND_CORROSIVE_DECAYGAP);
		bite = eat * k / steps - eat * (k - 1) / steps;
		cur = CheckActorInventory(m, "MonsterFortifyCount");
		if(cur <= bite)
			break;
		SetActorInventory(m, "MonsterFortifyCount", cur - bite);
	}
	if(IsActorAlive(m))
		SetActorInventory(m, "DnD_CorrosionDecay", 0);
}

// Owns the more-poison-damage-taken lane.
Script "DnD Corroded Timer" (int m) {
	while(CheckActorInventory(m, "DnD_CorrodedTimer") && IsActorAlive(m)) {
		TakeActorInventory(m, "DnD_CorrodedTimer", 1);
		Delay(const:1);
	}
	SetActorInventory(m, "DnD_Corroded", 0);
	SetActorInventory(m, "DnD_CorrodedTimer", 0);
}

// Client side: puffs of fume thrown along the cone from below the eye while it pours; a third more from rank 10.
Script "DnD Corrosive Breath FX" (int pnum, int reach, int hd) CLIENTSIDE {
	int caster = pnum + P_TIDSTART, half = hd & 0xFFFF, gap = (hd >> 16) ? 3 : 4;
	int t, k, a, p, cp, tid;
	for(t = 0; t < DND_CORROSIVE_RELEASE && IsActorAlive(caster); t += gap) {
		for(k = 0; k < 4; ++k) {
			a = GetActorAngle(caster) + random(-half, half);
			p = Clamp_Between(GetActorPitch(caster) + random(-0.02, 0.02), -0.2, 0.2);
			cp = cos(p);
			if(!(tid = ScratchTID()) || !SpawnForced("Spell_CorrosiveBreath_Fume", GetActorX(caster) + 20 * cos(a),
				GetActorY(caster) + 20 * sin(a), GetActorZ(caster) + GetActorViewHeight(caster) - 20.0, tid))
				continue;
			SetActorVelocity(tid, DND_CORROSIVE_SPEED * FixedMul(cos(a), cp), DND_CORROSIVE_SPEED * FixedMul(sin(a), cp),
				-DND_CORROSIVE_SPEED * sin(p), false, false);
			Thing_ChangeTID(tid, 0);
		}
		Delay(gap);
	}
}

// ---- Crushing Rocks ----

#define DND_CRUSHING_RISE 10            // tics the rocks take to rise beside the target
#define DND_CRUSHING_SLAM 6             // and to meet once risen
#define DND_CRUSHING_GAP 28             // tics from one crush to the next
#define DND_CRUSHING_HOLD 10            // tics the rocks grind on what they caught, before drawing apart for the next
#define DND_CRUSHING_WIND 14.0          // how much further out they draw back before each slam
#define DND_CRUSHING_BITE 8.0           // how far they bite into a caught body's edge
#define DND_CRUSHING_SQUASH 0.78        // a caught body's width at the impact, a share of its own, and its height
#define DND_CRUSHING_STRETCH 1.07
#define DND_CRUSHING_SHEAR 4.0          // how far the rocks rub past each other along their length while grinding
#define DND_CRUSHING_SINK 8             // tics they take to sink after the last
#define DND_CRUSHING_DROP 64.0          // how far under the crush point the rocks rise from
#define DND_CRUSHING_LIFT 20.0          // and how far over it they stand, so a rock rests on the floor rather than in it
#define DND_CRUSHING_SIDE 48.0          // how far past the target's radius each rock rises
#define DND_CRUSHING_MEET 31.0          // each rock's half width across the slam: where the two meet
#define DND_CRUSHING_CATCH 24.0         // past its radius, how far from the crush point the target may stray and still be caught

// The target under the crosshair, or the aimed spot; each crush re-forms round where the target stands, and misses one that left.
Script "DnD Crushing Rocks" (int pnum) {
	int caster = pnum + P_TIDSTART, cx, cy, cz;
	int target = PickActor(caster, GetActorAngle(caster), GetActorPitch(caster),
		DND_SPELL_HITSCANRANGE, 0, MF_SHOOTABLE, ML_BLOCKEVERYTHING, PICKAF_RETURNTID);
	if(!target || !IsActorAlive(target) || !IsSpellEnemy(target)) {
		target = 0;
		if(!TraceSpellAim(pnum, DND_SPELL_HITSCANRANGE))
			Terminate;
		for(int w = 0; !ReadSpellAim(pnum) && w < DND_SPELLAIM_WAIT; ++w)
			Delay(const:1);
		if(!SpellAimReady(pnum) || !PlayerInGame(pnum) || !IsActorAlive(caster))
			Terminate;
		auto aim = GetSpellAim();
		cx = aim.x[pnum];
		cy = aim.y[pnum];
		cz = GetFloorZAt(cx, cy);
	}

	int crushes = Max(1, GetSpellValue(pnum, SPL_CRUSHINGROCKS, SPELLVAL_AMOUNT));
	int r = SpellThresholdMet(pnum, SPL_CRUSHINGROCKS, DND_SPELL_THRESH_LOW) ?
		ScaleSpellArea(pnum, SPL_CRUSHINGROCKS, GetSpellValue(pnum, SPL_CRUSHINGROCKS, SPELLVAL_RADIUS) << 16) : 0;
	int k, a, side, mn, m;
	if(target) {
		cx = GetActorX(target);
		cy = GetActorY(target);
		cz = GetActorZ(target);
	}
	a = VectorAngle(cx - GetActorX(caster), cy - GetActorY(caster));
	side = (target ? GetActorProperty(target, APROP_RADIUS) : 16.0) + DND_CRUSHING_SIDE;
	// One pair of rocks for every crush: the FX follows the target itself, or keeps to the aimed spot.
	ACS_NamedExecuteAlways("DnD Crushing Rocks FX", 0, target ? target : PackMapPair(cx, cy), ((cz >> 16) & 0xFFFF) | (crushes << 16),
		((side >> 16) & 0x7FFF) | ((a & 0xFFFF) << 15) | (target ? 1 << 31 : 0));
	Delay(const:DND_CRUSHING_RISE);
	for(k = 0; k < crushes && PlayerInGame(pnum); ++k) {
		// Where the rocks start this slam: on the target, as the FX has drawn them.
		if(target && IsActorAlive(target)) {
			cx = GetActorX(target);
			cy = GetActorY(target);
			cz = GetActorZ(target);
		}
		Delay(const:DND_CRUSHING_SLAM);

		// Rank 5: everything near the crush point, the target included.
		if(r) {
			for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
				m = UsedMonsterTIDs[mn];
				if(IsActorAlive(m) && IsSpellEnemy(m) && fdistance_delta(cx - GetActorX(m), cy - GetActorY(m), cz - GetActorZ(m)) <= r)
					DealSpellBeamHit(pnum, SPL_CRUSHINGROCKS, m, 100, true);
			}
		}
		else if(target && IsActorAlive(target) &&
			VectorLength(GetActorX(target) - cx, GetActorY(target) - cy) <= GetActorProperty(target, APROP_RADIUS) + DND_CRUSHING_CATCH)
			DealSpellBeamHit(pnum, SPL_CRUSHINGROCKS, target, 100);
		// An aimed spot crushes whatever stands between the rocks.
		else if(!target) {
			for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
				m = UsedMonsterTIDs[mn];
				if(IsActorAlive(m) && IsSpellEnemy(m) && abs(GetActorZ(m) - cz) <= DND_CRUSHING_LIFT + DND_CRUSHING_DROP &&
					VectorLength(GetActorX(m) - cx, GetActorY(m) - cy) <= GetActorProperty(m, APROP_RADIUS) + DND_CRUSHING_CATCH)
					DealSpellBeamHit(pnum, SPL_CRUSHINGROCKS, m, 100);
			}
		}

		Delay(const:DND_CRUSHING_GAP - DND_CRUSHING_SLAM);
	}
}

// Steers an FX rock onto a spot by the next tic: by velocity, as SetActorPosition refuses a spot under the floor.
void SteerCrushingRock(int tid, int x, int y, int z) {
	SetActorVelocity(tid, x - GetActorX(tid), y - GetActorY(tid), z - GetActorZ(tid), false, false);
}

void SteerCrushingPair(int t1, int t2, int cx, int cy, int z, int px, int py, int off) {
	SteerCrushingRock(t1, cx + FixedMul(px, off), cy + FixedMul(py, off), z);
	SteerCrushingRock(t2, cx - FixedMul(px, off), cy - FixedMul(py, off), z);
}

// The grind after an impact, tic t: how far out from the bite the rocks stand, a recoil and then pressing in.
int GetCrushingGrind(int t) {
	switch(t) {
		case 0:
		return 10.0;
		case 1:
		return 5.0;
		case 2:
		return -3.0;
		case 3:
		return 1.0;
		case 4:
		return -4.0;
		case 6:
		return -3.0;
		case 7:
		return 1.0;
		case 8:
		return -2.0;
	}
	return 0;
}

// Client side: the spell's two rocks. They rise either side of the point once, across the caster's line, and for every
// crush draw back, slam into the target's body, grind on it and pull apart over to where it now stands; then sink.
Script "DnD Crushing Rocks FX" (int what, int zn, int sa) CLIENTSIDE {
	int target = sa < 0 ? what : 0, n = (zn >> 16) & 0xFF, side = (sa & 0x7FFF) << 16, a = (sa >> 15) & 0xFFFF;
	int cx = what << 16, cy = what & 0xFFFF0000, cz = ((zn << 16) >> 16) << 16;
	if(target) {
		if(!IsActorAlive(target))
			Terminate;
		cx = GetActorX(target);
		cy = GetActorY(target);
		cz = GetActorZ(target);
	}
	int px = -sin(a), py = cos(a), ux = cos(a), uy = sin(a), t1 = ScratchTID(), t2 = 0, k, t, off, e, sh, sx = 0, sy = 0;
	int open = side + DND_CRUSHING_WIND, bite = target ? GetActorProperty(target, APROP_RADIUS) + DND_CRUSHING_MEET - DND_CRUSHING_BITE : DND_CRUSHING_MEET;
	if(!t1 || !SpawnForced("Spell_CrushingRocks_Rock", cx + FixedMul(px, side), cy + FixedMul(py, side), cz - DND_CRUSHING_DROP, t1, a >> 8))
		Terminate;
	if(!(t2 = ScratchTID()) || !SpawnForced("Spell_CrushingRocks_Rock", cx - FixedMul(px, side), cy - FixedMul(py, side), cz - DND_CRUSHING_DROP, t2, a >> 8)) {
		Thing_Remove(t1);
		Terminate;
	}
	PlaySound(t1, "CrushingRocks/Rise", CHAN_BODY);
	for(t = 1; t <= DND_CRUSHING_RISE; ++t) {
		SteerCrushingPair(t1, t2, cx, cy, cz - DND_CRUSHING_DROP + (DND_CRUSHING_DROP + DND_CRUSHING_LIFT) * t / DND_CRUSHING_RISE, px, py, open);
		Delay(const:1);
	}

	for(k = 0; k < n; ++k) {
		// The slam: slow off the mark, then faster and faster into the body.
		for(t = 1; t <= DND_CRUSHING_SLAM; ++t) {
			off = open - FixedMul(open - bite, (t * t * t * 1.0) / (DND_CRUSHING_SLAM * DND_CRUSHING_SLAM * DND_CRUSHING_SLAM));
			SteerCrushingPair(t1, t2, cx, cy, cz + DND_CRUSHING_LIFT, px, py, off);
			Delay(const:1);
		}
		PlaySound(t1, "CrushingRocks/Crush", CHAN_BODY);
		Radius_Quake(2, 6, 0, 3, t1);
		e = bite - DND_CRUSHING_MEET;
		Spawn("Spell_CrushingRocks_Impact", cx + FixedMul(px, e), cy + FixedMul(py, e), cz + random(12.0, 36.0));
		if(e > 0)
			Spawn("Spell_CrushingRocks_Impact", cx - FixedMul(px, e), cy - FixedMul(py, e), cz + random(12.0, 36.0));
		// Squashed in the rocks' grip, unless another pair already has it.
		if(target && IsActorAlive(target) && !CheckActorInventory(target, "DnD_CrushSquashFX")) {
			GiveActorInventory(target, "DnD_CrushSquashFX", 1);
			sx = GetActorProperty(target, APROP_SCALEX);
			sy = GetActorProperty(target, APROP_SCALEY);
		}

		// The grind: recoil, then pressing in again and again, the rocks rubbing past each other and rocking.
		for(t = 0; t < DND_CRUSHING_HOLD; ++t) {
			off = bite + GetCrushingGrind(t);
			sh = (t & 2 ? DND_CRUSHING_SHEAR : -DND_CRUSHING_SHEAR) * (DND_CRUSHING_HOLD - t) / DND_CRUSHING_HOLD;
			SteerCrushingRock(t1, cx + FixedMul(px, off) + FixedMul(ux, sh), cy + FixedMul(py, off) + FixedMul(uy, sh), cz + DND_CRUSHING_LIFT + random(-2.0, 2.0));
			SteerCrushingRock(t2, cx - FixedMul(px, off) - FixedMul(ux, sh), cy - FixedMul(py, off) - FixedMul(uy, sh), cz + DND_CRUSHING_LIFT + random(-2.0, 2.0));
			SetActorAngle(t1, a + random(-0.012, 0.012));
			SetActorAngle(t2, a + random(-0.012, 0.012));
			if(sx) {
				e = t * 1.0 / DND_CRUSHING_HOLD;
				SetActorProperty(target, APROP_SCALEX, FixedMul(sx, DND_CRUSHING_SQUASH + FixedMul(1.0 - DND_CRUSHING_SQUASH, FixedMul(e, e))));
				SetActorProperty(target, APROP_SCALEY, FixedMul(sy, DND_CRUSHING_STRETCH - FixedMul(DND_CRUSHING_STRETCH - 1.0, FixedMul(e, e))));
			}
			Delay(const:1);
		}
		SetActorAngle(t1, a);
		SetActorAngle(t2, a);
		if(sx) {
			SetActorProperty(target, APROP_SCALEX, sx);
			SetActorProperty(target, APROP_SCALEY, sy);
			TakeActorInventory(target, "DnD_CrushSquashFX", 1);
			sx = 0;
		}
		if(k == n - 1)
			break;

		// Apart again, easing out, the pair's middle closing on the target a share of the rest each tic.
		for(t = 1; t <= DND_CRUSHING_GAP - DND_CRUSHING_SLAM - DND_CRUSHING_HOLD; ++t) {
			e = t * 1.0 / (DND_CRUSHING_GAP - DND_CRUSHING_SLAM - DND_CRUSHING_HOLD);
			e = 1.0 - FixedMul(1.0 - e, 1.0 - e);
			if(target && IsActorAlive(target)) {
				cx += (GetActorX(target) - cx) / (DND_CRUSHING_GAP - DND_CRUSHING_SLAM - DND_CRUSHING_HOLD - t + 1);
				cy += (GetActorY(target) - cy) / (DND_CRUSHING_GAP - DND_CRUSHING_SLAM - DND_CRUSHING_HOLD - t + 1);
				cz += (GetActorZ(target) - cz) / (DND_CRUSHING_GAP - DND_CRUSHING_SLAM - DND_CRUSHING_HOLD - t + 1);
			}
			SteerCrushingPair(t1, t2, cx, cy, cz + DND_CRUSHING_LIFT, px, py, bite + FixedMul(open - bite, e));
			Delay(const:1);
		}
	}

	for(t = 1; t <= DND_CRUSHING_SINK; ++t) {
		SteerCrushingPair(t1, t2, cx, cy, cz + DND_CRUSHING_LIFT - (DND_CRUSHING_DROP + DND_CRUSHING_LIFT) * t / DND_CRUSHING_SINK,
			px, py, bite);
		Delay(const:1);
	}
	Thing_Remove(t1);
	Thing_Remove(t2);
}

// ---- Summon: Rock Golem ----

#define DND_ROCKGOLEM_DIST 72
#define DND_ROCKGOLEM_SLIDECD (8 * TICRATE)  // rank 5: tics between its Rockslides
#define DND_ROCKGOLEM_SLIDEMIN 160.0         // and the nearest a target may be for one
#define DND_ROCKGOLEM_HAND 40.0              // how high off its feet the boulder leaves
#define DND_ROCKGOLEM_PHYSLESS 10            // percent less physical damage its summoner takes while it stands
#define DND_ROCKGOLEM_GUARDTICS 35           // how often that buff is pushed out again
#define DND_ROCKGOLEM_GUARDPOLL 5            // and how often the golem is looked for, so a death ends it promptly

// Whether a player's guard ticker runs. Map scope, so it resets with the scripts on a new map.
int[] module& GetRockGolemGuards() {
	static int on[MAXPLAYERS];
	return on;
}

// Rank 10 summons the hardened kind: immune to elemental damage, resisting explosions.
Script "DnD Summon Rock Golem" (int pnum) {
	int caster = pnum + P_TIDSTART;
	int maxkind = Max(1, GetSpellValue(pnum, SPL_ROCKGOLEM, SPELLVAL_AMOUNT));
	if(CountPlayerPetsOfKind(pnum, MONSTER_PET_ROCKGOLEM) >= maxkind)
		UnsummonPet(FindOldestPlayerPetOfKind(pnum, MONSTER_PET_ROCKGOLEM));
	if(!CanActorHaveMorePets(caster))
		UnsummonPet(FindOldestPlayerPet(pnum));

	int tid = TEMPORARY_PET_TID + pnum;
	if(!SpawnSummonNear(SpellThresholdMet(pnum, SPL_ROCKGOLEM, DND_SPELL_THRESH_HIGH) ? "Spell_RockGolem_Hardened" : "Spell_RockGolem",
		caster, tid, DND_ROCKGOLEM_DIST))
		Terminate;

	SetActorPosition(tid, GetActorX(tid), GetActorY(tid), GetActorFloorZ(tid), false);
	// Before its first tic: "DnD Pet Monster Scale" levels up from whatever APROP_HEALTH holds.
	SetActorProperty(tid, APROP_HEALTH, Max(1, ApplySpellGemMore(pnum, SPL_ROCKGOLEM, GEMSTAT_MORE_MINIONLIFE, GetSpellValue(pnum, SPL_ROCKGOLEM, SPELLVAL_DAMAGE))));
	SetActorProperty(tid, APROP_MASTERTID, caster);
	SetActivator(tid);
	SetPointer(AAPTR_MASTER, caster);
	SetActorProperty(0, APROP_FRIENDLY, true);
	Thing_ChangeTID(tid, 0);

	GiveActorInventory(caster, "PetCounter", 1);
	SetActivator(caster);
	PlaySound(caster, "RockGolem/Cast", CHAN_6);
	ACS_NamedExecuteAlways("DnD On Pet Summon", 0);
	ACS_NamedExecuteAlways("DnD Rock Golem Guard", 0, pnum);
}

// Its summoner takes less physical damage while a Rock Golem stands; the buff is removed as soon as none does.
Script "DnD Rock Golem Guard" (int pnum) {
	int[] module& on = GetRockGolemGuards();
	if(on[pnum])
		Terminate;
	on[pnum] = true;

	int caster = pnum + P_TIDSTART;
	for(int t = 0; PlayerInGame(pnum) && CountPlayerPetsOfKind(pnum, MONSTER_PET_ROCKGOLEM) > 0; t += DND_ROCKGOLEM_GUARDPOLL) {
		// Granted for twice the refresh so a late tick leaves no gap; a dead summoner gets it back on respawn.
		if(!(t % DND_ROCKGOLEM_GUARDTICS) && IsActorAlive(caster))
			HandlePlayerBuffAssignment(pnum, caster, BTI_SPELL_ROCKGOLEM_PHYSLESS, 0, 0, DND_ROCKGOLEM_GUARDTICS * 2, DND_ROCKGOLEM_PHYSLESS);
		Delay(const:DND_ROCKGOLEM_GUARDPOLL);
	}

	if(PlayerInGame(pnum))
		RemoveBuffWithTableIndex(pnum, BTI_SPELL_ROCKGOLEM_PHYSLESS);
	on[pnum] = false;
}

Script "DnD Rock Golem Melee" (void) {
	int pnum = GetActorProperty(0, APROP_MASTERTID) - P_TIDSTART;
	if(pnum < 0 || pnum >= MAXPLAYERS) {
		SetResultValue(0);
		Terminate;
	}
	SetResultValue(ACS_NamedExecuteWithResult("DND Pet Monster Damage Scale", GetSpellValue(pnum, SPL_ROCKGOLEM, SPELLVAL_DAMAGE2), 0, 0));
}

// Rank 5: its OWNER's Rockslide thrown in an arc at its target, owned by the owner, as the Ice Golem throws Ice Bolt.
Script "DnD Rock Golem Rockslide" (void) {
	int golem = ActivatorTID(), pnum = GetActorProperty(0, APROP_MASTERTID) - P_TIDSTART;
	if(!golem || pnum < 0 || pnum >= MAXPLAYERS || !PlayerInGame(pnum) || Timer() < GetUserVariable(0, "user_slide") ||
		!SpellThresholdMet(pnum, SPL_ROCKGOLEM, DND_SPELL_THRESH_LOW) || !SetActivator(0, AAPTR_TARGET)) {
		SetResultValue(0);
		Terminate;
	}
	int target = ActivatorTID();
	SetActivator(golem);
	int dx = GetActorX(target) - GetActorX(golem), dy = GetActorY(target) - GetActorY(golem), d = VectorLength(dx, dy);
	if(!target || !IsActorAlive(target) || d < DND_ROCKGOLEM_SLIDEMIN) {
		SetResultValue(0);
		Terminate;
	}

	// The arc Rockslide's own cast solves, from the golem's hand to the target's middle.
	int rise = GetActorZ(target) + GetActorProperty(target, APROP_HEIGHT) / 2 - (GetActorZ(golem) + DND_ROCKGOLEM_HAND);
	int across = SpellThresholdMet(pnum, SPL_ROCKSLIDE, DND_SPELL_THRESH_LOW) ? DND_ROCKSLIDE_R5_SPEED : DND_ROCKSLIDE_SPEED;
	int t = FixedDiv(d, across), up = FixedDiv(rise, t) + FixedMul(DND_ROCKSLIDE_GRAVITY >> 1, t);
	SpawnSpellProjectile(pnum, SPL_ROCKSLIDE, "Spell_RockGolem_Boulder", VectorLength(across, up) >> 16, DND_ATF_USEGRAVITY,
		VectorAngle(dx, dy) - GetActorAngle(golem), 0, 0, golem, true, DND_ROCKGOLEM_HAND, -VectorAngle(across, up));
	SetUserVariable(golem, "user_slide", Timer() + DND_ROCKGOLEM_SLIDECD);
	PlaySound(golem, "Rockslide/Cast", CHAN_WEAPON);
	SetResultValue(1);
}

// The boulder prices at rank 1 when its owner never learned Rockslide.
Script "DnD Rock Golem Boulder Setup" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum != -1 && !GetSpellRank(pnum, SPL_ROCKSLIDE, true))
		SetUserVariable(0, "user_rankat", 1);
	SetResultValue(0);
}

// Client side: the sphere round the target while the heal runs; a recast pushes its end out, an early stop (tics 0) pulls
// it in. The target's own screen sees it far fainter, from inside it.
Script "DnD Rejuvenation Spell FX" (int ttid, int tics) CLIENTSIDE {
	if(CheckActorInventory(ttid, "DnD_RejuvFX")) {
		SetActorInventory(ttid, "DnD_RejuvFX", Timer() + tics);
		Terminate;
	}
	int tid = ScratchTID();
	if(!tics || !tid || !SpawnForced("Spell_Rejuvenation_Sphere", GetActorX(ttid), GetActorY(ttid), GetActorZ(ttid), tid))
		Terminate;
	SetActorInventory(ttid, "DnD_RejuvFX", Timer() + tics);

	int peak = ConsolePlayerNumber() == ttid - P_TIDSTART ? DND_REJUV_OWNALPHA : 1.0;
	int h = GetActorProperty(ttid, APROP_HEIGHT), sc = Max(1.0, FixedDiv(h * 3 / 5, DND_REJUV_RADIUS)), t, left, a;
	SetActorProperty(tid, APROP_SCALEX, sc);
	SetActorProperty(tid, APROP_SCALEY, sc);
	for(t = 1; IsActorAlive(ttid); ++t) {
		left = CheckActorInventory(ttid, "DnD_RejuvFX") - Timer();
		if(left <= -DND_REJUV_FADEOUT)
			break;
		a = FixedMul(peak, Min(1.0, t * 1.0 / DND_REJUV_FADEIN));
		if(left < 0)
			a = a * (DND_REJUV_FADEOUT + left) / DND_REJUV_FADEOUT;
		SetActorPosition(tid, GetActorX(ttid), GetActorY(ttid), GetActorZ(ttid) + h / 2, false);
		SetActorProperty(tid, APROP_ALPHA, a);
		Delay(const:1);
	}
	Thing_Remove(tid);
	SetActorInventory(ttid, "DnD_RejuvFX", 0);
}

// ---- Agony ----

#define DND_AGONY_R5_CHANCE 10          // base percent to poison and to bleed, for any hit inside the aura
#define DND_AGONY_R10_LESS 25           // percent off a monster's poison on whoever the aura covers

// Each player's Agony reach while its rank 5 is on, 0 when not; the last slot counts them, the hit path's quick out.
int[] module& GetAgonyZones() {
	static int z[MAXPLAYERS + 1];
	return z;
}

void ApplyAgonyAura(int pnum, int dur) {
	GiveSpellBuffAround(pnum, SPL_AGONY, BTI_SPELL_AGONY, GetSpellValue(pnum, SPL_AGONY, SPELLVAL_DAMAGE) >> 16, dur, true);
	if(SpellThresholdMet(pnum, SPL_AGONY, DND_SPELL_THRESH_HIGH))
		GiveSpellBuffAround(pnum, SPL_AGONY, BTI_SPELL_AGONY_POISONLESS, DND_AGONY_R10_LESS, dur, true);
}

// Keeps a player's rank 5 zone in step with the aura; r is the scaled reach, 0 drops it.
void SetAgonyZone(int pnum, int r) {
	int[] module& z = GetAgonyZones();
	if(!z[pnum] != !r)
		z[MAXPLAYERS] += r ? 1 : -1;
	z[pnum] = r;
}

// Agony rank 5's base chance for an enemy: whether it stands inside any such aura.
int GetAgonyAilmentChance(int victim) {
	int[] module& z = GetAgonyZones();
	if(z[MAXPLAYERS] <= 0)
		return 0;
	for(int i = 0; i < MAXPLAYERS; ++i)
		if(z[i] && PlayerInGame(i) && fdistance(victim, i + P_TIDSTART) <= z[i])
			return DND_AGONY_R5_CHANCE;
	return 0;
}

// ---- Vulnerability and Despair ----

#define DND_EARTHCURSE_R5_AOE 25        // rank 5's increased area, both curses

// A curse's chance on one enemy: the strongest chance and the longest clock win, one ticker per monster.
void CurseMonster(int m, str lane, str clock, str ticker, int chance, int tics) {
	if(CheckActorInventory(m, lane) < chance)
		SetActorInventory(m, lane, chance);

	bool ticking = !!CheckActorInventory(m, clock);
	if(CheckActorInventory(m, clock) < tics)
		SetActorInventory(m, clock, tics);

	if(!ticking)
		ACS_NamedExecuteAlways(ticker, 0, m);
}

// Client side: a curse's ring of markers round where it landed.
void SpawnCurseRing(str fx, int cx, int cy, int cz, int r) {
	int n = Clamp_Between((FixedDiv(r, DND_FLAMMABILITY_RINGSTEP) >> 16) * 6, DND_FLAMMABILITY_RINGMIN, DND_FLAMMABILITY_RINGMAX);
	for(int i = 0; i < n; ++i) {
		int a = (i * 1.0) / n;
		SpawnForced(fx, cx + FixedMul(cos(a), r), cy + FixedMul(sin(a), r), cz + DND_FLAMMABILITY_RINGLIFT, 0, 0);
	}
}

Script "DnD Vulnerability Area FX" (int cx, int cy, int cz, int r) CLIENTSIDE {
	if(r > 0)
		SpawnCurseRing("VulnerabilityAreaMarkerFX", cx, cy, cz, r);
	SetResultValue(0);
}

Script "DnD Despair Area FX" (int cx, int cy, int cz, int r) CLIENTSIDE {
	if(r > 0)
		SpawnCurseRing("DespairAreaMarkerFX", cx, cy, cz, r);
	SetResultValue(0);
}

// Both Earth curses, Conductivity's way in: the enemy under the crosshair, else where the aim trace struck.
Script "DnD Earth Curse" (int pnum, int spell) {
	int caster = pnum + P_TIDSTART, cx, cy, cz;
	bool vuln = spell == SPL_VULNERABILITY;
	PlaySound(caster, vuln ? "Vulnerability/Cast" : "Despair/Cast", 6);

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
		for(int w = 0; !ReadSpellAim(pnum) && w < DND_SPELLAIM_WAIT; ++w)
			Delay(const:1);
		if(!SpellAimReady(pnum) || !PlayerInGame(pnum) || !IsActorAlive(caster))
			Terminate;
		auto aim = GetSpellAim();
		cx = aim.x[pnum];
		cy = aim.y[pnum];
		cz = aim.z[pnum];
	}

	int r = ScaleSpellArea(pnum, spell, GetSpellValue(pnum, spell, SPELLVAL_RADIUS) << 16,
		DND_EARTHCURSE_R5_AOE * SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_LOW));
	int tics = GetSpellDurationTics(pnum, spell);
	if(vuln)
		ACS_NamedExecuteWithResult("DnD Vulnerability Area FX", cx, cy, cz, r);
	else
		ACS_NamedExecuteWithResult("DnD Despair Area FX", cx, cy, cz, r);

	// Witch: more curse effect on both halves.
	int pct = ApplySpellGemMore(pnum, spell, GEMSTAT_MORE_CURSE, GetSpellValue(pnum, spell, SPELLVAL_DAMAGE));
	int chance = ApplySpellGemMore(pnum, spell, GEMSTAT_MORE_CURSE, GetSpellValue(pnum, spell, SPELLVAL_DAMAGE2));
	bool noregen = !vuln && SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_HIGH);

	int i, mn;
	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		if(!IsActorAlive(i) || !IsSpellEnemy(i) || fdistance_delta(cx - GetActorX(i), cy - GetActorY(i), cz - GetActorZ(i)) > r)
			continue;

		if(vuln) {
			ApplyPhysicalExposure(i, pct, tics);
			CurseMonster(i, "DnD_Vulnerable", "DnD_VulnerableTimer", "DnD Vulnerability Timer", chance, tics);
		}
		else {
			ApplyPoisonExposure(i, pct, tics);
			CurseMonster(i, "DnD_Despaired", "DnD_DespairedTimer", "DnD Despair Timer", chance, tics);
			if(noregen)
				PauseMonsterRegen(i, tics);
		}
	}
}

// ---- Upheaval ----

#define DND_UPHEAVAL_WAIT (TICRATE * 3 / 2)  // tics from the cast to the first blast
#define DND_UPHEAVAL_UNSTABLE (2 * TICRATE)  // and from it to the unstable ground's
#define DND_UPHEAVAL_R10_REPEATS 2           // more unstable blasts at rank 10, a second apart
#define DND_UPHEAVAL_STUN (2 * TICRATE)
#define DND_UPHEAVAL_TOSS 16.0               // upward speed of a full strength toss, 136 high; the big go less high
#define DND_UPHEAVAL_HEAVY_PCT 85            // of the toss, the Baron tier up and uniques: 99 high
#define DND_UPHEAVAL_BOSS_PCT 75             // bosses and legendaries, 78; a dungeon boss keeps the usual 25
#define DND_UPHEAVAL_MASSFULL 1000           // the mass at which the throw's extra is whole, a Baron's
#define DND_UPHEAVAL_MASSDMG 500             // that extra
#define DND_UPHEAVAL_REACH 128.0             // how far over its floor an enemy can be and still be caught
#define DND_UPHEAVAL_LANDWAIT (3 * TICRATE)  // tics a tossed enemy has to come down before the throw lands anyway
#define DND_UPHEAVAL_BOX 1024.0              // the rifts never reach this far, so a cheap test can skip the rest
#define DND_UPHEAVAL_TIPS 32                 // cracks one cast can have growing: the 2 main ones, their branches and twigs
#define DND_UPHEAVAL_MAXCRACKS 200           // segments one cast lays, all of which the blasts heat
#define DND_UPHEAVAL_CRACKSEG 48.0           // a main crack's segment, UPCRACK.md3's at scale 1; branches' are shorter and thinner
#define DND_UPHEAVAL_BRANCHSEG 34.0
#define DND_UPHEAVAL_TWIGSEG 24.0
#define DND_UPHEAVAL_BRANCHFROM 48.0         // no branch leaves a main crack nearer the caster than this
#define DND_UPHEAVAL_BRANCH 70               // percent chance a crack sends a branch off at a joint, never two joints running
#define DND_UPHEAVAL_TWIG 40                 // and a branch a twig
#define DND_UPHEAVAL_OUTER 70                // percent of a main crack's branches that go its own way out, the rest inward
#define DND_UPHEAVAL_BRANCHTURN 0.1          // how sharply a branch leaves, in turns, plus up to 0.06 more
#define DND_UPHEAVAL_BEND 0.05               // the heading a branch bends round to as it goes: aside, but mostly forward
#define DND_UPHEAVAL_WANDER 0.02             // how far a heading wanders at each joint
#define DND_UPHEAVAL_LANE 0.5                // a main crack's lane: this share of the cone's half width out on its side
#define DND_UPHEAVAL_SPREAD0 0.6             // the cone's half width at the caster, a share of the far end's
#define DND_UPHEAVAL_MIDS 6                  // UPCRACK's looks: middles, ends (turned round, starts), and pointed both ways
#define DND_UPHEAVAL_ENDS 4
#define DND_UPHEAVAL_BOTHS 2
#define DND_UPHEAVAL_START 0.25              // and where it starts, a share of the cone's half width at the caster
#define DND_UPHEAVAL_LANE3 0.62              // the side cracks' lanes when rank 5's third runs down the middle
#define DND_UPHEAVAL_CRACKHIT 40.0           // an enemy this near a crack, past its radius, is caught by the explosions
#define DND_UPHEAVAL_STEPS ((DND_UPHEAVAL_WAIT + 2) / 3)   // the tear's steps of 3 tics; the first explosion follows the last
#define DND_UPHEAVAL_SLOTS 8                 // layouts each side keeps at once, the server's and a client's apart: one machine runs both
#define DND_UPHEAVAL_CRACKSTEP 8.0           // the most floor height a segment may span
#define DND_UPHEAVAL_CRACKFADE 40            // tics the cracks stay after the last blast, cooling to black, then fade
#define DND_UPHEAVAL_HEATBASE 0.1            // UpheavalCrack.fp reads ScaleY / ScaleX - 1: 0 pulses, from this up a blast's heat
#define DND_UPHEAVAL_HEATHOLD 3              // tics a blast keeps the cracks white hot
#define DND_UPHEAVAL_HEATCOOL 24             // and they take to cool to black

// Whether a living enemy stands in the rifts: len ahead of (x0, y0) along (ux, uy), half to either side, near its floor.
// The rifts spread as they go, as Crackling Lance's cone does: half the width at the caster, all of it at the far end.
int GetUpheavalSpread(int along, int len, int half) {
	return FixedMul(half, DND_UPHEAVAL_SPREAD0) + FixedMul(FixedMul(half, 1.0 - DND_UPHEAVAL_SPREAD0), FixedDiv(Clamp_Between(along, 0, len), len));
}

bool InUpheavalRifts(int probe, int m, int x0, int y0, int ux, int uy, int len, int half) {
	int rx = GetActorX(m) - x0, ry = GetActorY(m) - y0;
	if(abs(rx) > DND_UPHEAVAL_BOX || abs(ry) > DND_UPHEAVAL_BOX || !IsActorAlive(m) || !IsSpellEnemy(m))
		return false;
	int rad = GetActorProperty(m, APROP_RADIUS), along = FixedMul(rx, ux) + FixedMul(ry, uy);
	if(along < -rad || along > len + rad || abs(FixedMul(rx, uy) - FixedMul(ry, ux)) > GetUpheavalSpread(along, len, half) + rad)
		return false;
	return GetActorZ(m) <= GetActorFloorZ(m) + DND_UPHEAVAL_REACH && CheckSight(probe, m, CSF_NOBLOCKALL);
}

// The rifts' layouts, grown alike on the server and every client from one seed. Slots 0-7 the server's, 8-15 a client's.
// Joints are packed: along in quarter units over 16 bits, side in quarter units + 0x8000 under them.
typedef struct {
	int joint0[2 * DND_UPHEAVAL_SLOTS * DND_UPHEAVAL_MAXCRACKS];
	int joint1[2 * DND_UPHEAVAL_SLOTS * DND_UPHEAVAL_MAXCRACKS];
	int segs[2 * DND_UPHEAVAL_SLOTS];
	int tips[2 * DND_UPHEAVAL_SLOTS];
	int dice[2 * DND_UPHEAVAL_SLOTS];
	int next[2];
	int at[2 * DND_UPHEAVAL_SLOTS * DND_UPHEAVAL_TIPS];		// each growing crack: how far along, how far aside, its heading
	int side[2 * DND_UPHEAVAL_SLOTS * DND_UPHEAVAL_TIPS];
	int head[2 * DND_UPHEAVAL_SLOTS * DND_UPHEAVAL_TIPS];
	int bend[2 * DND_UPHEAVAL_SLOTS * DND_UPHEAVAL_TIPS];	// the heading it eases round to
	int gen[2 * DND_UPHEAVAL_SLOTS * DND_UPHEAVAL_TIPS];		// 0 main, 1 branch, 2 twig
	int way[2 * DND_UPHEAVAL_SLOTS * DND_UPHEAVAL_TIPS];		// its side, -1 or 1; 0 for rank 5's middle crack
	int wait[2 * DND_UPHEAVAL_SLOTS * DND_UPHEAVAL_TIPS];	// joints before it may branch again
	int dead[2 * DND_UPHEAVAL_SLOTS * DND_UPHEAVAL_TIPS];
	int begun[2 * DND_UPHEAVAL_SLOTS * DND_UPHEAVAL_TIPS];	// it has laid a segment
	int segcrack[2 * DND_UPHEAVAL_SLOTS * DND_UPHEAVAL_MAXCRACKS];	// the crack a segment belongs to; 0x100 on its first
} upheaval_rifts_T;

upheaval_rifts_T module& GetUpheavalRifts() {
	static upheaval_rifts_T s;
	return s;
}

int NextUpheavalSlot(bool client) {
	auto rf = GetUpheavalRifts();
	int c = client ? 1 : 0;
	rf.next[c] = (rf.next[c] + 1) % DND_UPHEAVAL_SLOTS;
	return rf.next[c] + c * DND_UPHEAVAL_SLOTS;
}

// The seed both sides start from: the server's 10 bit roll mixed with where and which way the cast went.
int GetUpheavalDice(int roll, int xy, int a) {
	return (roll + 1) * -1640531535 ^ xy ^ ((a & 0xFFFF) << 9);
}

// The layout's own dice, the same everywhere: a fixed value from lo to hi, or a percent chance.
int RollUpheaval(int k, int lo, int hi) {
	auto rf = GetUpheavalRifts();
	rf.dice[k] = rf.dice[k] * 1103515245 + 12345;
	return lo + FixedMul(hi - lo, (rf.dice[k] >> 15) & 0xFFFF);
}

bool UpheavalChance(int k, int pct) {
	auto rf = GetUpheavalRifts();
	rf.dice[k] = rf.dice[k] * 1103515245 + 12345;
	return ((rf.dice[k] >> 16) & 0x7FFF) % 100 < pct;
}

int PackUpheavalJoint(int along, int side) {
	return ((along >> 14) << 16) | (((side >> 14) + 0x8000) & 0xFFFF);
}

int GetUpheavalJointA(int p) {
	return (p >> 16) & 0xFFFF;
}

int GetUpheavalJointS(int p) {
	return (p & 0xFFFF) - 0x8000;
}

// Where the segment from (a0, s0) to (a1, s1) first crosses the one from (b0, t0) to (b1, t1), as a fixed share of
// the first, or -1. All in quarter units of the rifts: products of map fixed point would overflow.
int CrossUpheavalCracks(int a0, int s0, int a1, int s1, int b0, int t0, int b1, int t1) {
	int ra = a1 - a0, rs = s1 - s0, qa = b1 - b0, qs = t1 - t0;
	int den = ra * qs - rs * qa, un = (b0 - a0) * qs - (t0 - s0) * qa, vn = (b0 - a0) * rs - (t0 - s0) * ra;
	if(!den)
		return -1;
	if(den < 0) {
		den = -den;
		un = -un;
		vn = -vn;
	}
	if(un <= 0 || un > den || vn < 0 || vn > den)
		return -1;
	return FixedDiv(un, den);
}

// The main cracks, from the caster: 2, or 3 at rank 5 with the third down the middle.
void BeginUpheavalRifts(int k, int dice, int len, int half, int mains) {
	auto rf = GetUpheavalRifts();
	int base = k * DND_UPHEAVAL_TIPS, i, w;
	rf.dice[k] = dice;
	rf.segs[k] = 0;
	rf.tips[k] = mains;
	for(i = 0; i < mains; ++i) {
		w = mains == 3 ? i - 1 : i * 2 - 1;
		rf.at[base + i] = RollUpheaval(k, 24.0, 40.0);
		rf.side[base + i] = w * FixedMul(FixedMul(DND_UPHEAVAL_START, GetUpheavalSpread(0, len, half)), RollUpheaval(k, 0.8, 1.2));
		rf.head[base + i] = w ? w * RollUpheaval(k, 0, 0.02) : RollUpheaval(k, -0.01, 0.01);
		rf.bend[base + i] = 0;
		rf.gen[base + i] = 0;
		rf.way[base + i] = w;
		rf.wait[base + i] = 0;
		rf.dead[base + i] = false;
		rf.begun[base + i] = false;
	}
}

// The tear's front at step i of DND_UPHEAVAL_STEPS.
int GetUpheavalFront(int i, int len) {
	return FixedMul(len, (i * 3 * 1.0) / DND_UPHEAVAL_WAIT) + 48.0;
}

// Grows slot k's cracks up to the front d: main cracks steer for their lanes, branches and twigs leave them mostly outward
// and bend forward; a crack stops where it meets another, a branch at the cone's edge. New segments join the slot's joints.
void GrowUpheavalRifts(int k, int d, int len, int half, int mains) {
	auto rf = GetUpheavalRifts();
	int base = k * DND_UPHEAVAL_TIPS, sb = k * DND_UPHEAVAL_MAXCRACKS, i, j, c, n, seg, turn, lane, step, s, a1, cone, best, u, qa, qs;
	for(i = 0; i < rf.tips[k]; ++i) {
		c = base + i;
		seg = rf.gen[c] ? (rf.gen[c] == 1 ? DND_UPHEAVAL_BRANCHSEG : DND_UPHEAVAL_TWIGSEG) : DND_UPHEAVAL_CRACKSEG;
		while(!rf.dead[c] && rf.at[c] < len && rf.at[c] + FixedMul(seg, cos(rf.head[c])) <= d) {
			--rf.wait[c];
			n = rf.tips[k];
			if(n < DND_UPHEAVAL_TIPS && rf.gen[c] < 2 && rf.at[c] >= DND_UPHEAVAL_BRANCHFROM && rf.wait[c] < 0 &&
				UpheavalChance(k, rf.gen[c] ? DND_UPHEAVAL_TWIG : DND_UPHEAVAL_BRANCH)) {
				rf.wait[c] = 1;
				// The middle crack's branches take either side; the others' mostly their own side, out.
				if(!rf.way[c])
					turn = UpheavalChance(k, 50) ? 1 : -1;
				else
					turn = rf.gen[c] || UpheavalChance(k, DND_UPHEAVAL_OUTER) ? rf.way[c] : -rf.way[c];
				rf.at[base + n] = rf.at[c];
				rf.side[base + n] = rf.side[c];
				rf.head[base + n] = rf.head[c] + turn * (DND_UPHEAVAL_BRANCHTURN + RollUpheaval(k, 0, 0.06));
				rf.bend[base + n] = turn * DND_UPHEAVAL_BEND;
				rf.gen[base + n] = rf.gen[c] + 1;
				rf.way[base + n] = turn;
				rf.wait[base + n] = 0;
				rf.dead[base + n] = false;
				rf.begun[base + n] = false;
				rf.tips[k] = n + 1;
			}
			// A main crack eases toward its lane.
			if(!rf.gen[c]) {
				lane = rf.way[c] * FixedMul(mains == 3 ? DND_UPHEAVAL_LANE3 : DND_UPHEAVAL_LANE, GetUpheavalSpread(rf.at[c] + 2 * seg, len, half));
				rf.bend[c] = VectorAngle(2 * seg, lane - rf.side[c]);
				if(rf.bend[c] > 0.5)
					rf.bend[c] -= 1.0;
			}
			step = FixedMul(seg, cos(rf.head[c]));
			s = rf.side[c] + FixedMul(seg, sin(rf.head[c]));
			a1 = rf.at[c] + step;
			cone = GetUpheavalSpread(a1, len, half);
			if(abs(s) > cone) {
				if(rf.gen[c]) {
					rf.dead[c] = true;
					break;
				}
				s = s > 0 ? cone : -cone;
			}
			// Meeting another crack: cut at the crossing and end. Segments touching this one's start do not count.
			qa = rf.at[c] >> 14;
			qs = rf.side[c] >> 14;
			best = -1;
			for(j = 0; j < rf.segs[k]; ++j) {
				n = rf.joint0[sb + j];
				u = rf.joint1[sb + j];
				if((abs(GetUpheavalJointA(n) - qa) <= 4 && abs(GetUpheavalJointS(n) - qs) <= 4) ||
					(abs(GetUpheavalJointA(u) - qa) <= 4 && abs(GetUpheavalJointS(u) - qs) <= 4))
					continue;
				u = CrossUpheavalCracks(qa, qs, a1 >> 14, s >> 14, GetUpheavalJointA(n), GetUpheavalJointS(n), GetUpheavalJointA(u), GetUpheavalJointS(u));
				if(u >= 0 && (best < 0 || u < best))
					best = u;
			}
			if(best >= 0) {
				a1 = rf.at[c] + FixedMul(a1 - rf.at[c], best);
				s = rf.side[c] + FixedMul(s - rf.side[c], best);
				rf.dead[c] = true;
			}
			if(rf.segs[k] < DND_UPHEAVAL_MAXCRACKS) {
				rf.joint0[sb + rf.segs[k]] = PackUpheavalJoint(rf.at[c], rf.side[c]);
				rf.joint1[sb + rf.segs[k]] = PackUpheavalJoint(a1, s);
				rf.segcrack[sb + rf.segs[k]] = i | (rf.begun[c] ? 0 : 0x100);
				rf.begun[c] = true;
				++rf.segs[k];
			}
			rf.at[c] = a1;
			rf.side[c] = s;
			// The heading eases round to its bend, and wanders a little.
			rf.head[c] += (rf.bend[c] - rf.head[c]) / 3 + RollUpheaval(k, -DND_UPHEAVAL_WANDER, DND_UPHEAVAL_WANDER);
		}
	}
}

// Whether an enemy is caught by the explosions: in the rifts' cone, and within CRACKHIT of one of slot k's cracks.
bool OnUpheavalCracks(int k, int probe, int m, int x0, int y0, int ux, int uy, int len, int half) {
	auto rf = GetUpheavalRifts();
	if(!InUpheavalRifts(probe, m, x0, y0, ux, uy, len, half))
		return false;
	int rx = GetActorX(m) - x0, ry = GetActorY(m) - y0, sb = k * DND_UPHEAVAL_MAXCRACKS;
	int pa = (FixedMul(rx, ux) + FixedMul(ry, uy)) >> 14, ps = (FixedMul(ry, ux) - FixedMul(rx, uy)) >> 14;
	int reach = (DND_UPHEAVAL_CRACKHIT + GetActorProperty(m, APROP_RADIUS)) >> 14, j, a0, s0, da, ds, dot, l2, dist;
	for(j = 0; j < rf.segs[k]; ++j) {
		a0 = GetUpheavalJointA(rf.joint0[sb + j]);
		s0 = GetUpheavalJointS(rf.joint0[sb + j]);
		da = GetUpheavalJointA(rf.joint1[sb + j]) - a0;
		ds = GetUpheavalJointS(rf.joint1[sb + j]) - s0;
		dot = (pa - a0) * da + (ps - s0) * ds;
		l2 = da * da + ds * ds;
		if(dot <= 0 || !l2)
			dist = sqrt((pa - a0) * (pa - a0) + (ps - s0) * (ps - s0));
		else if(dot >= l2)
			dist = sqrt((pa - a0 - da) * (pa - a0 - da) + (ps - s0 - ds) * (ps - s0 - ds));
		else
			dist = abs((pa - a0) * ds - (ps - s0) * da) / sqrt(l2);
		if(dist <= reach)
			return true;
	}
	return false;
}

int GetUpheavalTossPercent(int m) {
	switch(GetMonsterPushTier(m)) {
		case DND_PUSHTIER_HEAVY:
		return DND_UPHEAVAL_HEAVY_PCT;
		case DND_PUSHTIER_BOSS:
		return DND_UPHEAVAL_BOSS_PCT;
		case DND_PUSHTIER_DUNGEONBOSS:
		return DND_PUSH_BOSS_PCT;
	}
	return 100;
}

// Rumbles, then throws and stuns what stands in the rifts; the unstable ground then blows, three times at rank 10.
Script "DnD Upheaval" (int pnum) {
	int ptid = pnum + P_TIDSTART;
	int a = GetActorAngle(ptid), ux = cos(a), uy = sin(a), x0 = GetActorX(ptid), y0 = GetActorY(ptid);
	int len = ScaleSpellArea(pnum, SPL_UPHEAVAL, GetSpellValue(pnum, SPL_UPHEAVAL, SPELLVAL_RADIUS) << 16);
	int half = ScaleSpellArea(pnum, SPL_UPHEAVAL, GetSpellValue(pnum, SPL_UPHEAVAL, SPELLVAL_AMOUNT) << 16) / 2;
	int probe = SpawnSpellProbe(x0, y0, GetActorZ(ptid) + 32.0);
	if(!probe)
		Terminate;

	// The cracks: the clients draw them, the server grows the same ones from the same seed and hits along them.
	int blasts = 1 + DND_UPHEAVAL_R10_REPEATS * SpellThresholdMet(pnum, SPL_UPHEAVAL, DND_SPELL_THRESH_HIGH);
	int cracks = DND_UPHEAVAL_STEPS * 3 + DND_UPHEAVAL_UNSTABLE + (blasts - 1) * TICRATE + DND_UPHEAVAL_CRACKFADE;
	int mains = 2 + SpellThresholdMet(pnum, SPL_UPHEAVAL, DND_SPELL_THRESH_LOW), roll = random(0, 0x3FF), rk = NextUpheavalSlot(false);
	int i, m, k;
	BeginUpheavalRifts(rk, GetUpheavalDice(roll, PackMapPair(x0, y0), a), len, half, mains);
	PlaySound(probe, "Upheaval/Rumble", CHAN_6);
	Radius_Quake(2, DND_UPHEAVAL_STEPS * 3, 0, (len >> 16) / 64 + 4, probe);
	ACS_NamedExecuteAlways("DnD Upheaval Rift FX", 0, PackMapPair(x0, y0), (a & 0xFFFF) | (cracks << 16) | (mains << 26),
		((len >> 16) & 0x7FF) | (((half >> 16) & 0x7FF) << 11) | (roll << 22));
	for(i = 0; i < DND_UPHEAVAL_STEPS; ++i) {
		GrowUpheavalRifts(rk, GetUpheavalFront(i, len), len, half, mains);
		Delay(const:3);
	}

	int stun = ScaleSpellEffectTics(pnum, SPL_UPHEAVAL, DND_UPHEAVAL_STUN);
	PlaySound(probe, "Upheaval/Blast", CHAN_7);
	Radius_Quake(4, 12, 0, (len >> 16) / 64 + 4, probe);
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(!OnUpheavalCracks(rk, probe, m, x0, y0, ux, uy, len, half))
			continue;
		DealSpellAmount(pnum, SPL_UPHEAVAL, m, RollSpellValue(pnum, SPL_UPHEAVAL, SPELLVAL_DAMAGE), true, DND_DAMAGEFLAG_NOPUSH);
		if(!IsActorAlive(m))
			continue;
		StunMonster(m, stun);
		// A floater thrown up would never come down: it is stunned where it is instead.
		if(!CheckFlag(m, "NOGRAVITY"))
			SetActorVelocity(m, GetActorVelX(m), GetActorVelY(m), DND_UPHEAVAL_TOSS * GetUpheavalTossPercent(m) / 100, false, false);
		ACS_NamedExecuteAlways("DnD Upheaval Landing", 0, pnum, m);
	}

	for(k = 0; k < blasts; ++k) {
		Delay(k ? TICRATE : DND_UPHEAVAL_UNSTABLE);
		if(!PlayerInGame(pnum))
			break;
		PlaySound(probe, "Upheaval/Blast", CHAN_7);
		Radius_Quake(3, 10, 0, (len >> 16) / 64 + 4, probe);
		for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
			m = UsedMonsterTIDs[i];
			if(OnUpheavalCracks(rk, probe, m, x0, y0, ux, uy, len, half))
				DealSpellAmount(pnum, SPL_UPHEAVAL, m, RollSpellValue(pnum, SPL_UPHEAVAL, SPELLVAL_DAMAGE2), true);
		}
	}
	Thing_Remove(probe);
}

// A thrown enemy coming down: the throw lands then, its base plus up to MASSDMG more the heavier it is.
Script "DnD Upheaval Landing" (int pnum, int m) {
	Delay(const:2);
	for(int t = 2; t < DND_UPHEAVAL_LANDWAIT && IsActorAlive(m); ++t) {
		if(GetActorZ(m) <= GetActorFloorZ(m) && GetActorVelZ(m) <= 0)
			break;
		Delay(const:1);
	}
	if(!IsActorAlive(m) || !PlayerInGame(pnum))
		Terminate;

	int mass = Clamp_Between(GetActorProperty(m, APROP_MASS), 0, DND_UPHEAVAL_MASSFULL);
	DealSpellAmount(pnum, SPL_UPHEAVAL, m, RollSpellValue(pnum, SPL_UPHEAVAL, SPELLVAL_AMOUNT2) + DND_UPHEAVAL_MASSDMG * mass / DND_UPHEAVAL_MASSFULL);
	PlaySound(m, "Upheaval/Slam", CHAN_7);
	ACS_NamedExecuteAlways("DnD Upheaval Slam FX", 0, m);
}

// A point of the rifts, client side: along of the length ahead, side of the half width across (both 0..1 fixed).
int UpheavalPointX(int x0, int a, int along, int side) {
	return x0 + FixedMul(along, cos(a)) - FixedMul(side, sin(a));
}

int UpheavalPointY(int y0, int a, int along, int side) {
	return y0 + FixedMul(along, sin(a)) + FixedMul(side, cos(a));
}

enum {
	DND_UPCRACK_MID,
	DND_UPCRACK_END,                     // converging on the segment's far joint
	DND_UPCRACK_START,                   // converging on its near joint: an end turned round
	DND_UPCRACK_BOTH
};

// A crack segment's look; dir runs from its near joint to its far one. Middles and both-ways ones lie either way round.
void SetUpheavalCrackLook(int tid, int kind, int dir) {
	str st;
	switch(kind) {
		case DND_UPCRACK_MID:
			st = StrParam(s:"M", d:random(1, DND_UPHEAVAL_MIDS));
			dir += random(0, 1) * 0.5;
		break;
		case DND_UPCRACK_END:
			st = StrParam(s:"T", d:random(1, DND_UPHEAVAL_ENDS));
		break;
		case DND_UPCRACK_START:
			st = StrParam(s:"T", d:random(1, DND_UPHEAVAL_ENDS));
			dir += 0.5;
		break;
		default:
			st = StrParam(s:"B", d:random(1, DND_UPHEAVAL_BOTHS));
			dir += random(0, 1) * 0.5;
	}
	SetActorAngle(tid, dir);
	SetActorState(tid, st);
}

// One crack segment from joint (d0, s0) to (d1, s1) of the rifts, left to stand for life tics; skipped across a step.
// Its scale is its length over the model's, so a short branch is a thin one too. Returns its tid, or 0.
int LayUpheavalCrack(int x0, int y0, int a, int d0, int s0, int d1, int s1, int life, int kind) {
	int ax = UpheavalPointX(x0, a, d0, s0), ay = UpheavalPointY(y0, a, d0, s0);
	int bx = UpheavalPointX(x0, a, d1, s1), by = UpheavalPointY(y0, a, d1, s1);
	int mx = (ax + bx) / 2, my = (ay + by) / 2, z = GetFloorZAt(mx, my), tid = UniqueTID();
	if(!tid || abs(GetFloorZAt(ax, ay) - GetFloorZAt(bx, by)) > DND_UPHEAVAL_CRACKSTEP || abs(GetFloorZAt(ax, ay) - z) > DND_UPHEAVAL_CRACKSTEP)
		return 0;
	int dir = VectorAngle(bx - ax, by - ay), sc = FixedDiv(VectorLength(bx - ax, by - ay), DND_UPHEAVAL_CRACKSEG);
	if(!SpawnForced("Spell_Upheaval_Crack", mx, my, z, tid, dir >> 8))
		return 0;
	SetUpheavalCrackLook(tid, kind, dir);
	SetActorProperty(tid, APROP_SCALEX, sc);
	SetActorProperty(tid, APROP_SCALEY, sc);
	SetUserVariable(tid, "user_life", life);
	return tid;
}

// A blast's heat on the cracks, 0 to 1: white hot for HOLD tics, then cooling to black. t counts from the blast.
int GetUpheavalHeat(int t) {
	if(t < 0)
		return -1;
	if(t < DND_UPHEAVAL_HEATHOLD)
		return 1.0;
	int c = Max(0, 1.0 - (t - DND_UPHEAVAL_HEATHOLD) * 1.0 / DND_UPHEAVAL_HEATCOOL);
	return FixedMul(c, c);
}

// Client side: the rifts tear forward from the server's seed, a crack laid per new segment; each explosion then bursts
// along them and leaves them white hot, cooling to black, and black they fade.
Script "DnD Upheaval Rift FX" (int xy, int al, int shape) CLIENTSIDE {
	auto rf = GetUpheavalRifts();
	int x0 = xy << 16, y0 = xy & 0xFFFF0000, a = al & 0xFFFF, life = (al >> 16) & 0x3FF, mains = (al >> 26) & 3;
	int len = (shape & 0x7FF) << 16, half = ((shape >> 11) & 0x7FF) << 16, k = NextUpheavalSlot(true);
	int blasts = 1 + (life - DND_UPHEAVAL_STEPS * 3 - DND_UPHEAVAL_UNSTABLE - DND_UPHEAVAL_CRACKFADE) / TICRATE + 1;
	int crack[DND_UPHEAVAL_MAXCRACKS], scale[DND_UPHEAVAL_MAXCRACKS], nc = 0;
	int lastTid[DND_UPHEAVAL_TIPS], lastDir[DND_UPHEAVAL_TIPS], c, first, tid;
	bool lastFirst[DND_UPHEAVAL_TIPS];
	int i, j, t, s, x, y, p0, p1, next, heat, sb = k * DND_UPHEAVAL_MAXCRACKS;
	BeginUpheavalRifts(k, GetUpheavalDice((shape >> 22) & 0x3FF, xy, a), len, half, mains);
	for(i = 0; i < DND_UPHEAVAL_STEPS; ++i) {
		j = rf.segs[k];
		GrowUpheavalRifts(k, GetUpheavalFront(i, len), len, half, mains);
		// Each new segment is laid pointed, so a growing crack's front closes off; the one it follows, pointed till now, turns
		// into a middle (or into a start, if it was the crack's first).
		for(; j < rf.segs[k]; ++j) {
			p0 = rf.joint0[sb + j];
			p1 = rf.joint1[sb + j];
			c = rf.segcrack[sb + j] & 0xFF;
			first = rf.segcrack[sb + j] & 0x100;
			if(!first && lastTid[c])
				SetUpheavalCrackLook(lastTid[c], lastFirst[c] ? DND_UPCRACK_START : DND_UPCRACK_MID, lastDir[c]);
			tid = 0;
			if(nc < DND_UPHEAVAL_MAXCRACKS && (tid = LayUpheavalCrack(x0, y0, a, GetUpheavalJointA(p0) << 14, GetUpheavalJointS(p0) << 14,
				GetUpheavalJointA(p1) << 14, GetUpheavalJointS(p1) << 14, life - i * 3, first ? DND_UPCRACK_BOTH : DND_UPCRACK_END))) {
				crack[nc] = tid;
				scale[nc++] = FixedDiv(VectorLength((GetUpheavalJointA(p1) - GetUpheavalJointA(p0)) << 14, (GetUpheavalJointS(p1) - GetUpheavalJointS(p0)) << 14),
					DND_UPHEAVAL_CRACKSEG);
			}
			lastTid[c] = tid;
			lastFirst[c] = !!first;
			lastDir[c] = VectorAngle(UpheavalPointX(x0, a, GetUpheavalJointA(p1) << 14, GetUpheavalJointS(p1) << 14) - UpheavalPointX(x0, a, GetUpheavalJointA(p0) << 14, GetUpheavalJointS(p0) << 14),
				UpheavalPointY(y0, a, GetUpheavalJointA(p1) << 14, GetUpheavalJointS(p1) << 14) - UpheavalPointY(y0, a, GetUpheavalJointA(p0) << 14, GetUpheavalJointS(p0) << 14));
		}
		for(j = k * DND_UPHEAVAL_TIPS; j < k * DND_UPHEAVAL_TIPS + rf.tips[k]; ++j) {
			if(rf.dead[j] || rf.at[j] >= len || random(0, 1))
				continue;
			x = UpheavalPointX(x0, a, rf.at[j], rf.side[j]);
			y = UpheavalPointY(y0, a, rf.at[j], rf.side[j]);
			Spawn("Spell_Rockslide_Dust", x, y, GetFloorZAt(x, y) + 4.0);
			if(!random(0, 2))
				Spawn("Spell_Shrapnel_Piece", x, y, GetFloorZAt(x, y) + 4.0);
		}
		Delay(const:3);
	}

	// The explosions, on the server's beat: the first as the tear ends, the unstable ground's UNSTABLE later, a second apart after.
	for(i = 0; i < blasts; ++i) {
		for(j = Clamp_Between(nc / 4, 6, 30); j > 0 && nc; --j) {
			s = crack[random(0, nc - 1)];
			Spawn("Spell_Upheaval_Burst", GetActorX(s) + random(-12.0, 12.0), GetActorY(s) + random(-12.0, 12.0), GetActorZ(s));
		}
		next = i + 1 < blasts ? (i ? TICRATE : DND_UPHEAVAL_UNSTABLE) : DND_UPHEAVAL_CRACKFADE;
		for(t = 0; t < next; ++t) {
			heat = GetUpheavalHeat(t);
			if(t <= DND_UPHEAVAL_HEATHOLD + DND_UPHEAVAL_HEATCOOL)
				for(j = 0; j < nc; ++j)
					SetActorProperty(crack[j], APROP_SCALEY, FixedMul(scale[j], 1.0 + DND_UPHEAVAL_HEATBASE + heat));
			Delay(const:1);
		}
	}
}

// Client side: dust where a thrown enemy lands.
Script "DnD Upheaval Slam FX" (int m) CLIENTSIDE {
	for(int i = 0; i < 6; ++i)
		Spawn("Spell_Rockslide_Dust", GetActorX(m) + random(-24.0, 24.0), GetActorY(m) + random(-24.0, 24.0), GetActorZ(m) + 4.0);
}

// ---- Threaded Volley ----

#define DND_TVOLLEY_SPEED 36            // the small rocks
#define DND_TVOLLEY_BIGSPEED 26         // the energized rock
#define DND_TVOLLEY_GAP1 16            // tics before the 2nd rock and the 3rd: the throws quicken, then every GAPMAX
#define DND_TVOLLEY_GAP2 11
#define DND_TVOLLEY_GAPMAX 7
#define DND_TVOLLEY_AREA 256            // energized area radius, 384 at rank 10
#define DND_TVOLLEY_R10_AREA 384
#define DND_TVOLLEY_R5_MORE 20          // more damage per overlapping area past the first, from rank 5
#define DND_TVOLLEY_BIGBLAST 200        // the energized rock's blast, percent of the RADIUS row: 80 -> 160
#define DND_TVOLLEY_CHUNKS 8            // chunks the energized rock bursts into
#define DND_TVOLLEY_CHUNKDMG 16
#define DND_TVOLLEY_CHUNKRIPS 4
#define DND_TVOLLEY_CHUNKSPEED 24
#define DND_TVOLLEY_CHUNKLIFE 16        // tics a chunk flies
#define DND_TVOLLEY_DISC 15.156         // map units per unit of the ground disc's scale: flat2's 15.625 by the skin's rim
#define DND_TVOLLEY_DISCFADE 42         // tics the ground spends fading in (8) and out (34)

// Outside any of its own energized areas: lays one where it is cast, then throws the rocks one by one along the
// caster's facing. Inside: the one big rock instead, more for each overlapping area from rank 5.
Script "DnD Threaded Volley" (int pnum) {
	auto v = GetVolleyData();
	int ptid = pnum + P_TIDSTART, areas = v.press[pnum], i, k;
	int extra = GetSpellExtraProjectiles(pnum, SPL_THREADEDVOLLEY);

	if(areas) {
		v.pct[pnum] = 100 + DND_TVOLLEY_R5_MORE * (areas - 1) * SpellThresholdMet(pnum, SPL_THREADEDVOLLEY, DND_SPELL_THRESH_LOW);
		PlaySound(ptid, "ThreadedVolley/Energized", CHAN_6);
		for(i = 0; i <= extra; ++i)
			SpawnSpellProjectile(pnum, SPL_THREADEDVOLLEY, "Spell_ThreadedVolley_Boulder", DND_TVOLLEY_BIGSPEED, 0, FanOffset(i, DND_SPELL_FANSPREAD));
		ACS_NamedExecuteAlways("DnD Earth Unearth FX", 0, pnum, 48, 2);
		Terminate;
	}

	int r = ScaleSpellArea(pnum, SPL_THREADEDVOLLEY,
		(SpellThresholdMet(pnum, SPL_THREADEDVOLLEY, DND_SPELL_THRESH_HIGH) ? DND_TVOLLEY_R10_AREA : DND_TVOLLEY_AREA) << 16);
	// Laid on the floor under the caster, even from the air of a jump.
	int tics = GetSpellDurationTics(pnum, SPL_THREADEDVOLLEY), fz = GetActorFloorZ(ptid);
	AddVolleyArea(pnum, GetActorX(ptid), GetActorY(ptid), fz, r, tics);
	ACS_NamedExecuteAlways("DnD Threaded Volley Area FX", 0, PackMapPair(GetActorX(ptid), GetActorY(ptid)), fz >> 16,
		(r >> 16) | (tics << 16));

	int n = Max(1, GetSpellValue(pnum, SPL_THREADEDVOLLEY, SPELLVAL_AMOUNT));
	for(k = 0; k < n && PlayerInGame(pnum) && IsActorAlive(ptid); ++k) {
		if(k)
			Delay(k == 1 ? DND_TVOLLEY_GAP1 : k == 2 ? DND_TVOLLEY_GAP2 : DND_TVOLLEY_GAPMAX);
		PlaySound(ptid, "ThreadedVolley/Throw", CHAN_6);
		for(i = 0; i <= extra; ++i)
			SpawnSpellProjectile(pnum, SPL_THREADEDVOLLEY, "Spell_ThreadedVolley_Rock", DND_TVOLLEY_SPEED, 0, FanOffset(i, DND_SPELL_FANSPREAD));
		ACS_NamedExecuteAlways("DnD Earth Unearth FX", 0, pnum, 40, 0);
	}
}

// The energized rock and its chunks deal the percent the cast set.
Script "DnD Volley Setup" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum != -1)
		SetUserVariable(0, "user_dmgpct", GetVolleyData().pct[pnum]);
	SetResultValue(0);
}

// A chunk's damage: a set amount, priced as the spell's and at the cast's percent. That is read off the caster, as a
// chunk born inside an enemy hits it before any setup of its own could run.
Script "DnD Volley Chunk Damage" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}
	int dmg = PriceSpellDamage(pnum, SPL_THREADEDVOLLEY, DND_TVOLLEY_CHUNKDMG);
	int pct = GetVolleyData().pct[pnum];
	if(pct > 0)
		dmg = dmg * pct / 100;
	SetResultValue(PackSpellDamage(Max(1, dmg), GetSpellDamageType(SPL_THREADEDVOLLEY), SPL_THREADEDVOLLEY));
}

// From the energized rock's burst: chunks flung out level in a ring, owned by the caster. Each is its own ripper, its
// rips and life stamped at spawn: one born inside an enemy has already hit it by its first frame.
Script "DnD Volley Chunks" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}
	int me = ActivatorTID(), had = me;
	if(!me) {
		me = ScratchTID();
		Thing_ChangeTID(0, me);
	}
	for(int i = 0; i < DND_TVOLLEY_CHUNKS; ++i)
		SpawnSpellProjectile(pnum, SPL_THREADEDVOLLEY, "Spell_ThreadedVolley_Chunk", DND_TVOLLEY_CHUNKSPEED, 0, i * 1.0 / DND_TVOLLEY_CHUNKS,
			0, DND_TVOLLEY_CHUNKLIFE, me, true, 8.0, 0, -1, ReserveRipperId() + 1, DND_TVOLLEY_CHUNKRIPS);
	if(!had)
		Thing_ChangeTID(me, 0);
	SetResultValue(0);
}

// Client side: an energized area's ground, a soil disc and its glowing rim, fading out as the area lapses.
// Packed is radius | tics << 16. The tid lives only this tic; a small random lift keeps overlapping discs off each other.
Script "DnD Threaded Volley Area FX" (int xy, int z, int packed) CLIENTSIDE {
	int x = xy << 16, y = xy & 0xFFFF0000, r = (packed & 0xFFFF) << 16, tics = packed >> 16, tid = UniqueTID();
	if(!tid)
		Terminate;
	z = (z << 16) + random(0.0, 0.75);
	Spawn("Spell_ThreadedVolley_Ground", x, y, z, tid);
	Spawn("Spell_ThreadedVolley_GroundGlow", x, y, z, tid);
	SetActorProperty(tid, APROP_ScaleX, FixedDiv(r, DND_TVOLLEY_DISC));
	SetActorAngle(tid, random(0, 1.0));
	SetUserVariable(tid, "user_life", Max(0, tics - DND_TVOLLEY_DISCFADE));
	Thing_ChangeTID(tid, 0);
}

// ---- Geyser ----

#define DND_GEYSER_AHEAD 224.0         // the middle of the field, ahead of the caster
#define DND_GEYSER_SPREAD 160.0        // vents open within this of it
#define DND_GEYSER_TRIES 6             // spots tried for a vent before it is skipped
#define DND_GEYSER_MAX 32
#define DND_GEYSER_RATEMIN 8           // drops a second from one vent, rolled per vent
#define DND_GEYSER_RATEMAX 12
#define DND_GEYSER_REACHMIN 24         // how far a drop lands from its vent
#define DND_GEYSER_REACHMAX 144
#define DND_GEYSER_FLIGHT 14           // a drop's tics in the air, plus one for every FLIGHTDIV units it travels
#define DND_GEYSER_FLIGHTDIV 12
#define DND_GEYSER_GRAVITY 0.6         // Spell_Geyser_Drop's Gravity
#define DND_GEYSER_LIFT 18.0           // drops leave the vent this far up, the middle of its pool
#define DND_GEYSER_PENDING 32          // drops one vent can have in the air
#define DND_GEYSER_RISE 10             // tics the rock pile takes to come up, before the first spit
#define DND_GEYSER_CRUMBLE 30          // and to crumble into the ground once it is spent (Crumble's frames, and a little)
#define DND_GEYSER_DEPTH 32.0          // how far under the floor it starts, all of it hidden

// The vents' shared dice: the server and every client step it from the same seed, so their drops agree.
int GeyserStep(int s) {
	return s * 1103515245 + 12345;
}

// A step read as 0..1.0.
int GeyserFrac(int s) {
	return (s >> 15) & 0xFFFF;
}

// A vent's drops a second, the first thing its dice decide.
int GeyserRate(int s, int extra) {
	return DND_GEYSER_RATEMIN + ((s >> 16) & 0x7FFF) % (DND_GEYSER_RATEMAX - DND_GEYSER_RATEMIN + 1) + extra;
}

// How far a drop goes, off its second step.
int GeyserReach(int s) {
	return (DND_GEYSER_REACHMIN << 16) + FixedMul((DND_GEYSER_REACHMAX - DND_GEYSER_REACHMIN) << 16, GeyserFrac(s));
}

// The ground ahead splits into AMOUNT vents (50% more at rank 10), each spitting acid for DURATION.
Script "DnD Geyser" (int pnum) {
	int ptid = pnum + P_TIDSTART, a = GetActorAngle(ptid);
	int cx = GetActorX(ptid) + FixedMul(DND_GEYSER_AHEAD, cos(a)), cy = GetActorY(ptid) + FixedMul(DND_GEYSER_AHEAD, sin(a));
	int n = GetSpellValue(pnum, SPL_GEYSER, SPELLVAL_AMOUNT);
	if(SpellThresholdMet(pnum, SPL_GEYSER, DND_SPELL_THRESH_HIGH))
		n += n / 2;
	n = Min(n, DND_GEYSER_MAX);
	int life = Min(GetSpellDurationTics(pnum, SPL_GEYSER), 0xFFFF), extra = Min(GetSpellExtraProjectiles(pnum, SPL_GEYSER), 0x7FFF);

	// One probe walked over the candidates: each vent must be in the caster's sight.
	int probe = SpawnSpellProbe(cx, cy, GetFloorZAt(cx, cy) + 32.0);
	if(!probe)
		Terminate;
	PlaySound(ptid, "Geyser/Quake", CHAN_6);
	Radius_Quake(4, TICRATE, 0, 8, ptid);

	int i, k, gx, gy, gz, d, ga, zseed;
	for(i = 0; i < n; ++i) {
		for(k = 0; k < DND_GEYSER_TRIES; ++k) {
			ga = random(0, 1.0);
			d = FixedMul(DND_GEYSER_SPREAD, fsqrt(random(0, 1.0)));
			gx = cx + FixedMul(d, cos(ga));
			gy = cy + FixedMul(d, sin(ga));
			gz = GetFloorZAt(gx, gy);
			SetActorPosition(probe, gx, gy, gz + 32.0, false);
			if(CheckSight(ptid, probe, CSF_NOBLOCKALL))
				break;
		}
		if(k == DND_GEYSER_TRIES)
			continue;
		zseed = ((gz >> 16) & 0xFFFF) | (random(1, 0x7FFF) << 16);
		ACS_NamedExecuteAlways("DnD Geyser Vent", 0, pnum, PackMapPair(gx, gy), zseed);
		ACS_NamedExecuteAlways("DnD Geyser FX", 0, PackMapPair(gx, gy), zseed, life | (extra << 16));
	}
	Thing_Remove(probe);
}

// Where a drop lands: every enemy within the splash takes the hit, which always poisons.
void GeyserSplash(int pnum, int probe, int x, int y, int r) {
	int z = GetFloorZAt(x, y), i, m;
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(InSpellArea(probe, m, x, y, r) && InSpellHeight(m, z, r))
			DealSpellBeamHit(pnum, SPL_GEYSER, m, 100, true);
	}
}

// One vent, server side: it throws on its seed's schedule and each drop lands its damage when its flight ends.
Script "DnD Geyser Vent" (int pnum, int xy, int zseed) {
	int x = xy << 16, y = xy & 0xFFFF0000, z = zseed << 16, s = GeyserStep((zseed >> 16) & 0x7FFF);
	int life = GetSpellDurationTics(pnum, SPL_GEYSER), rate = GeyserRate(s, GetSpellExtraProjectiles(pnum, SPL_GEYSER));
	int r = GetSpellValue(pnum, SPL_GEYSER, SPELLVAL_RADIUS) << 16;
	if(SpellThresholdMet(pnum, SPL_GEYSER, DND_SPELL_THRESH_LOW))
		r *= 2;
	r = ScaleSpellArea(pnum, SPL_GEYSER, r);
	int probe = SpawnSpellProbe(x, y, z + 32.0);
	if(!probe)
		Terminate;
	// The clients' pile comes up first; their drops start after it, and so do these.
	Delay(const:DND_GEYSER_RISE);

	int land_t[DND_GEYSER_PENDING], land_x[DND_GEYSER_PENDING], land_y[DND_GEYSER_PENDING], i, k, t, a, d, acc = 0;
	for(k = 0; k < DND_GEYSER_PENDING; ++k)
		land_t[k] = -1;
	int last = life + DND_GEYSER_FLIGHT + DND_GEYSER_REACHMAX / DND_GEYSER_FLIGHTDIV + 1;
	for(t = 0; t <= last && PlayerInGame(pnum); ++t) {
		if(t < life) {
			for(acc += rate; acc >= TICRATE; acc -= TICRATE) {
				s = GeyserStep(s);
				a = GeyserFrac(s);
				s = GeyserStep(s);
				d = GeyserReach(s);
				for(k = 0; k < DND_GEYSER_PENDING && land_t[k] != -1; ++k);
				if(k == DND_GEYSER_PENDING)
					continue;
				land_t[k] = t + DND_GEYSER_FLIGHT + (d >> 16) / DND_GEYSER_FLIGHTDIV;
				land_x[k] = x + FixedMul(d, cos(a));
				land_y[k] = y + FixedMul(d, sin(a));
			}
		}
		for(k = 0; k < DND_GEYSER_PENDING; ++k) {
			if(land_t[k] == t) {
				GeyserSplash(pnum, probe, land_x[k], land_y[k], r);
				land_t[k] = -1;
			}
		}
		Delay(const:1);
	}
	Thing_Remove(probe);
}

// Client side: the rock pile comes up, then its drops fly on the same dice the server lands them by, then it crumbles.
Script "DnD Geyser FX" (int xy, int zseed, int lifeextra) CLIENTSIDE {
	int x = xy << 16, y = xy & 0xFFFF0000, z = zseed << 16, s = GeyserStep((zseed >> 16) & 0x7FFF);
	int life = lifeextra & 0xFFFF, rate = GeyserRate(s, lifeextra >> 16);
	int vent = UniqueTID(), t, a, d, ft, tid, q, acc = 0;
	if(vent && !SpawnForced("Spell_Geyser_Vent", x, y, z - DND_GEYSER_DEPTH, vent, random(0, 255)))
		vent = 0;
	for(t = 0; t < 2; ++t)
		Spawn("Spell_Rockslide_Dust", x + random(-20.0, 20.0), y + random(-20.0, 20.0), z + 2.0);
	for(t = 0; t < 3; ++t)
		Spawn("Spell_RockArmor_Chunk", x + random(-16.0, 16.0), y + random(-16.0, 16.0), z + random(4.0, 20.0));
	// Steered by velocity, each tic onto its mark, quick out of the ground and easing in: SetActorPosition refuses under the floor.
	for(t = 1; t <= DND_GEYSER_RISE; ++t) {
		q = 1.0 - t * 1.0 / DND_GEYSER_RISE;
		if(vent)
			SetActorVelocity(vent, 0, 0, z - FixedMul(DND_GEYSER_DEPTH, FixedMul(FixedMul(q, q), q)) - GetActorZ(vent), false, false);
		Delay(const:1);
	}
	if(vent)
		SetActorVelocity(vent, 0, 0, 0, false, false);
	PlaySound(vent, "Geyser/Spit", CHAN_BODY);
	for(t = 0; t < life; ++t) {
		for(acc += rate; acc >= TICRATE; acc -= TICRATE) {
			s = GeyserStep(s);
			a = GeyserFrac(s);
			s = GeyserStep(s);
			d = GeyserReach(s);
			ft = DND_GEYSER_FLIGHT + (d >> 16) / DND_GEYSER_FLIGHTDIV;
			if(!(tid = UniqueTID()) || !Spawn("Spell_Geyser_Drop", x, y, z + DND_GEYSER_LIFT, tid))
				continue;
			// Up enough to come down LIFT lower than it left, on the floor, when the server lands it; pointed the way it flies.
			SetActorVelocity(tid, FixedMul(cos(a), d / ft), FixedMul(sin(a), d / ft),
				FixedMul(DND_GEYSER_GRAVITY, ft << 16) / 2 - DND_GEYSER_LIFT / ft, false, false);
			SetActorAngle(tid, a);
			Thing_ChangeTID(tid, 0);
		}
		Delay(const:1);
	}
	StopSound(vent, CHAN_BODY);
	if(!vent)
		Terminate;
	SetActorState(vent, "Crumble");
	for(t = 0; t < 4; ++t)
		Spawn("Spell_RockArmor_Chunk", x + random(-14.0, 14.0), y + random(-14.0, 14.0), z + random(10.0, 22.0));
	Spawn("Spell_Rockslide_Dust", x + random(-12.0, 12.0), y + random(-12.0, 12.0), z + 2.0);
	Delay(const:DND_GEYSER_CRUMBLE);
	Thing_Remove(vent);
}

// ---- Unleash Earth ----

#define DND_UNLEASH_RANGE 640.0
#define DND_UNLEASH_GAP 12             // tics between pillars: 0.33 seconds, whatever the haste
#define DND_UNLEASH_IDLE (5 * TICRATE)  // the channel ends after this long without a pillar
#define DND_UNLEASH_WARN 7             // tics the ground cracks before a pillar erupts: 0.2 seconds
#define DND_UNLEASH_MINH 128           // a pillar's height, kept short of the ceiling
#define DND_UNLEASH_MAXH 240
#define DND_UNLEASH_BOSSLOW 108.0      // how high a boss or legendary is thrown, from the shortest pillar to the tallest; never past it
#define DND_UNLEASH_BOSSHIGH 128.0
#define DND_UNLEASH_MAXPILLARS 5       // a channel's pillars, the first included
#define DND_UNLEASH_HEADROOM 16.0
#define DND_UNLEASH_REACHUP 48.0       // how far off the pillar's floor an enemy's feet may be and still be caught
#define DND_UNLEASH_LEAN 0.04          // most a pillar leans, as a turn fraction; its toss goes the way it leans
#define DND_UNLEASH_MORE 25            // percent more an enemy takes for each earlier time the same channel flung it
#define DND_UNLEASH_MASSFULL 1000      // the mass at which the landing is whole; the lightest take half
#define DND_UNLEASH_LANDWAIT (5 * TICRATE)
#define DND_UNLEASH_BUMP 2.0           // rank 10: a flying enemy that stops from this speed hit a wall
#define DND_UNLEASH_BUMPCHECK 3        // tics between looks for a collision
#define DND_UNLEASH_PILLARSTEP 48.0    // the pillar model's frames stand 48, 96 ... 288 tall; the nearest is scaled to fit
#define DND_UNLEASH_PILLARFRAMES 6
#define DND_UNLEASH_PILLARRISE 4       // tics it takes to burst up
#define DND_UNLEASH_PILLARHOLD 30      // to stand
#define DND_UNLEASH_PILLARSINK 24      // and to sink back

typedef struct {
	int channels;		// bumped by each channel, so an enemy's DnD_UnleashChannel tells this one from an older
} unleash_T;

unleash_T module& GetUnleash() {
	static unleash_T s;
	return s;
}

// How much of its height a pillar stands out of the ground, tic t of its rise: out sharply, a little past, settling.
int GetUnleashRise(int t) {
	switch(t) {
		case 0:
		return 0.5;
		case 1:
		return 0.85;
		case 2:
		return 1.03;
	}
	return 1.0;
}

// The cast raises the first pillar; then each attack press raises one more, at most one every GAP tics, a press in the
// gap kept for when it ends. Casting it again ends the channel, as do IDLE tics without a pillar and running out of mana.
Script "DnD Unleash Earth" (int pnum) {
	// One channel at a time: Quicktongue's repeat lands while the first is running.
	if(IsSpellRunning(pnum, SPL_UNLEASHEARTH))
		Terminate;
	int caster = pnum + P_TIDSTART, wait = DND_UNLEASH_GAP, idle = 0, n = 1, channel = ++GetUnleash().channels;
	bool was = IsChannelHeld(), held, want = false;
	BeginSpellBusy(pnum);
	SetSpellRunning(pnum, SPL_UNLEASHEARTH, true);
	ACS_NamedExecuteAlways("DnD Unleash Pillar", 0, pnum, channel);
	// The cast bar counts the window down, full again with every pillar.
	int bar = StartSpellCastBar(pnum, SPL_UNLEASHEARTH, DND_UNLEASH_IDLE, true);
	while(n < DND_UNLEASH_MAXPILLARS && PlayerInGame(pnum) && IsActorAlive(caster) && IsSpellRunning(pnum, SPL_UNLEASHEARTH) &&
		idle < DND_UNLEASH_IDLE && CanAffordSpell(pnum, SPL_UNLEASHEARTH)) {
		// Back up once a Quicktongue repeat's own bar has come and gone.
		if(!CheckActorInventory(caster, "P_CastProgress")) {
			SetActorInventory(caster, "P_CastSpell", (SPL_UNLEASHEARTH + 1) | DND_CASTBAR_DRAIN);
			SetActorInventory(caster, "P_CastProgress", bar);
		}
		held = IsChannelHeld();
		want |= held && !was;
		was = held;
		if(wait)
			--wait;
		else if(want) {
			if(!PayChannelTick(pnum, SPL_UNLEASHEARTH, false))
				break;
			ACS_NamedExecuteAlways("DnD Unleash Pillar", 0, pnum, channel);
			bar = StartSpellCastBar(pnum, SPL_UNLEASHEARTH, DND_UNLEASH_IDLE, true);
			++n;
			want = false;
			wait = DND_UNLEASH_GAP;
			idle = -1;
		}
		++idle;
		Delay(const:1);
	}
	SetSpellRunning(pnum, SPL_UNLEASHEARTH, false);
	EndSpellCastBar(pnum, bar);
	EndSpellBusy(pnum);
	StartSpellCooldown(pnum, SPL_UNLEASHEARTH);
}

// One pillar: under the enemy in the crosshair or where the aim ends, it cracks, then erupts, hitting and flinging what stands there.
Script "DnD Unleash Pillar" (int pnum, int channel) {
	int caster = pnum + P_TIDSTART, x, y;
	int target = PickActor(caster, GetActorAngle(caster), GetActorPitch(caster), DND_UNLEASH_RANGE, 0, MF_SHOOTABLE, ML_BLOCKEVERYTHING, PICKAF_RETURNTID);
	if(target && IsActorAlive(target) && IsSpellEnemy(target)) {
		x = GetActorX(target);
		y = GetActorY(target);
	}
	else {
		if(!TraceSpellAim(pnum, DND_UNLEASH_RANGE))
			Terminate;
		for(int w = 0; !ReadSpellAim(pnum) && w < DND_SPELLAIM_WAIT; ++w)
			Delay(const:1);
		if(!SpellAimReady(pnum) || !PlayerInGame(pnum))
			Terminate;
		x = GetSpellAim().x[pnum];
		y = GetSpellAim().y[pnum];
	}

	int z = GetFloorZAt(x, y);
	int h = Max(32.0, Min(random(DND_UNLEASH_MINH, DND_UNLEASH_MAXH) << 16, GetCeilingZAt(x, y) - z - DND_UNLEASH_HEADROOM));
	int la = random(0, 1.0), lean = random(0, DND_UNLEASH_LEAN);
	int r = ScaleSpellArea(pnum, SPL_UNLEASHEARTH, GetSpellValue(pnum, SPL_UNLEASHEARTH, SPELLVAL_RADIUS) << 16);
	ACS_NamedExecuteAlways("DnD Unleash Pillar FX", 0, PackMapPair(x, y), ((z >> 16) & 0xFFFF) | ((h >> 16) << 16), (la & 0xFFFF) | (lean << 16));
	Delay(const:DND_UNLEASH_WARN);
	int probe = SpawnSpellProbe(x, y, z + 32.0);
	if(!probe || !PlayerInGame(pnum))
		Terminate;

	// Thrown about as high as the pillar stands, and aside the way it leans.
	int up = fsqrt(2 * h), side = FixedMul(up, sin(lean)), i, m, v, vz, peak, pct, count;
	for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
		m = UsedMonsterTIDs[i];
		if(!InSpellArea(probe, m, x, y, r) || abs(GetActorZ(m) - z) > DND_UNLEASH_REACHUP)
			continue;
		count = CheckActorInventory(m, "DnD_UnleashChannel") == channel ? CheckActorInventory(m, "DnD_UnleashCaught") + 1 : 1;
		SetActorInventory(m, "DnD_UnleashChannel", channel);
		SetActorInventory(m, "DnD_UnleashCaught", count);
		pct = 100 + DND_UNLEASH_MORE * (count - 1);
		// No push of its own: the fling is the push, and a hit's would throw it past its height.
		DealSpellBeamHit(pnum, SPL_UNLEASHEARTH, m, pct, true, DND_DAMAGEFLAG_NOPUSH);
		if(!IsActorAlive(m) || CheckFlag(m, "NOGRAVITY"))
			continue;
		v = GetMonsterPushPercent(m);
		vz = up * v / 100;
		// A boss is thrown to a set height instead. Gravity takes 1 a tic, so a toss of vz peaks at vz (vz + 1) / 2.
		if(GetMonsterPushTier(m) == DND_PUSHTIER_BOSS) {
			peak = Min(h, DND_UNLEASH_BOSSLOW + (DND_UNLEASH_BOSSHIGH - DND_UNLEASH_BOSSLOW) *
				Clamp_Between((h >> 16) - DND_UNLEASH_MINH, 0, DND_UNLEASH_MAXH - DND_UNLEASH_MINH) / (DND_UNLEASH_MAXH - DND_UNLEASH_MINH));
			vz = (fsqrt(8 * peak + 1.0) - 1.0) / 2;
			v = (FixedDiv(vz, up) * 100) >> 16;
		}
		SetActorVelocity(m, FixedMul(side, cos(la)) * v / 100, FixedMul(side, sin(la)) * v / 100, vz, false, false);
		GiveActorInventory(m, "DnD_UnleashFlight", 1);
		ACS_NamedExecuteAlways("DnD Unleash Landing", 0, pnum, m, pct | (CheckActorInventory(m, "DnD_UnleashFlight") << 16));
	}

	// Rank 5: a pillar under the caster launches them, unharmed.
	if(SpellThresholdMet(pnum, SPL_UNLEASHEARTH, DND_SPELL_THRESH_LOW) && IsActorAlive(caster) && abs(GetActorZ(caster) - z) <= DND_UNLEASH_REACHUP &&
		VectorLength(GetActorX(caster) - x, GetActorY(caster) - y) <= r + GetActorProperty(caster, APROP_RADIUS))
		SetActorVelocity(caster, FixedMul(side, cos(la)), FixedMul(side, sin(la)), up, false, false);
	Thing_Remove(probe);
}

// A flung enemy coming down: the crash deals up to DAMAGE2, by how high it went and how heavy it is. Rank 10: a wall or another body hit on the way costs half that.
Script "DnD Unleash Landing" (int pnum, int m, int packed) {
	int pct = packed & 0xFFFF, flight = packed >> 16, z0 = GetActorZ(m), top = z0, t, sp, prev = 0, o, i, reach;
	bool r10 = SpellThresholdMet(pnum, SPL_UNLEASHEARTH, DND_SPELL_THRESH_HIGH), bumped = false;
	Delay(const:2);
	for(t = 2; t < DND_UNLEASH_LANDWAIT && IsActorAlive(m); ++t) {
		if(CheckActorInventory(m, "DnD_UnleashFlight") != flight)
			Terminate;
		top = Max(top, GetActorZ(m));
		if(GetActorZ(m) <= GetActorFloorZ(m) && GetActorVelZ(m) <= 0)
			break;
		// Stopped short mid flight: it hit a wall or a body, and a body it hit takes the same.
		if(r10 && !bumped && !(t % DND_UNLEASH_BUMPCHECK)) {
			sp = VectorLength(GetActorVelX(m), GetActorVelY(m));
			if(prev >= DND_UNLEASH_BUMP && sp < prev / 4) {
				bumped = true;
				DealSpellAmount(pnum, SPL_UNLEASHEARTH, m, RollSpellValue(pnum, SPL_UNLEASHEARTH, SPELLVAL_DAMAGE2) / 2);
				for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
					o = UsedMonsterTIDs[i];
					if(o == m || !IsActorAlive(o) || !IsSpellEnemy(o))
						continue;
					reach = GetActorProperty(m, APROP_RADIUS) + GetActorProperty(o, APROP_RADIUS) + 2.0;
					if(abs(GetActorX(o) - GetActorX(m)) <= reach && abs(GetActorY(o) - GetActorY(m)) <= reach &&
						GetActorZ(o) <= GetActorZ(m) + GetActorProperty(m, APROP_HEIGHT) && GetActorZ(m) <= GetActorZ(o) + GetActorProperty(o, APROP_HEIGHT)) {
						DealSpellAmount(pnum, SPL_UNLEASHEARTH, o, RollSpellValue(pnum, SPL_UNLEASHEARTH, SPELLVAL_DAMAGE2) / 2);
						break;
					}
				}
			}
			prev = sp;
		}
		Delay(const:1);
	}
	if(!IsActorAlive(m) || !PlayerInGame(pnum))
		Terminate;

	int mass = Clamp_Between(GetActorProperty(m, APROP_MASS), 0, DND_UNLEASH_MASSFULL);
	int dmg = RollSpellValue(pnum, SPL_UNLEASHEARTH, SPELLVAL_DAMAGE2) * (50 + 50 * mass / DND_UNLEASH_MASSFULL) / 100 * pct / 100;
	dmg = FixedMul(dmg, Min(1.0, FixedDiv(top - z0, DND_UNLEASH_MAXH << 16)));
	if(dmg > 0)
		DealSpellAmount(pnum, SPL_UNLEASHEARTH, m, dmg);
	PlaySound(m, "UnleashEarth/Land", CHAN_7);
	ACS_NamedExecuteAlways("DnD Upheaval Slam FX", 0, m);
}

// Client side: the ground cracks for WARN tics, then a pillar bursts up leaning its way, stands, and sinks back.
Script "DnD Unleash Pillar FX" (int xy, int zh, int lean) CLIENTSIDE {
	int x = xy << 16, y = xy & 0xFFFF0000, z = zh << 16, h = (zh >> 16) << 16, la = lean & 0xFFFF, i, t;
	for(t = 0; t < DND_UNLEASH_WARN; ++t) {
		Spawn("Spell_Rockslide_Dust", x + random(-48.0, 48.0), y + random(-48.0, 48.0), z + 4.0);
		Delay(const:1);
	}
	int p = UniqueTID(), k = Clamp_Between((h + DND_UNLEASH_PILLARSTEP / 2) / DND_UNLEASH_PILLARSTEP, 1, DND_UNLEASH_PILLARFRAMES), q;
	if(!p || !SpawnForced("Spell_UnleashEarth_Pillar", x, y, z - h, p, la >> 8))
		Terminate;
	SetActorState(p, StrParam(s:"H", d:k));
	SetActorPitch(p, -(lean >> 16));
	SetActorProperty(p, APROP_SCALEY, FixedDiv(h, k * DND_UNLEASH_PILLARSTEP));
	PlaySound(p, "UnleashEarth/Erupt", CHAN_BODY);
	for(i = 0; i < 8; ++i)
		Spawn("Spell_RockArmor_Chunk", x + random(-32.0, 32.0), y + random(-32.0, 32.0), z + random(8.0, 48.0));
	// Steered by velocity, each tic exactly onto its mark: SetActorPosition refuses a spot under the floor.
	for(t = 0; t < DND_UNLEASH_PILLARRISE; ++t) {
		SetActorVelocity(p, 0, 0, z - FixedMul(h, 1.0 - GetUnleashRise(t)) - GetActorZ(p), false, false);
		Delay(const:1);
	}
	SetActorVelocity(p, 0, 0, 0, false, false);
	Delay(const:DND_UNLEASH_PILLARHOLD);
	// Back under, quick at first and easing out: 1 - (1 - t)^3 of its height.
	for(t = 1; t <= DND_UNLEASH_PILLARSINK; ++t) {
		q = 1.0 - t * 1.0 / DND_UNLEASH_PILLARSINK;
		SetActorVelocity(p, 0, 0, z - FixedMul(h, 1.0 - FixedMul(FixedMul(q, q), q)) - GetActorZ(p), false, false);
		Delay(const:1);
	}
	Thing_Remove(p);
}

// depth counts Quicktongue's repeats; a cast from TryCastSpell is depth 0. A proc (DND_SPELLCAST_PROC) has no bar and leaves the lock alone.
Script "DnD Spell Cast" (int spell, int pnum, int depth) {
	bool proc = !!(depth & DND_SPELLCAST_PROC);
	depth &= ~DND_SPELLCAST_PROC;

	// Monsters wake to a hostile cast the way they wake to a gunshot. At the START of the cast, not
	// when the effect lands, so a long cast bar cannot be used to open on a sleeping room for free
	if(IsHostileSpell(spell))
		GiveActorInventory(pnum + P_TIDSTART, "MonsterWaker", 1);

	// The cast time is spent HERE rather than in TryCastSpell, which has already taken the mana and
	// started the cooldown -- so a cast that is interrupted still costs, and the cooldown runs from the
	int ct = GetSpellCastTics(pnum, spell);
	bool ally = !!(SpellDefs[spell].flags & SPLF_ALLYTARGET);

	// Surgecast skips the first cast time in its window; ct stays, so Quicktongue still repeats.
	int wait = proc ? 0 : ct;
	if(wait > 0 && !depth && spell != SPL_SURGECAST && CheckActorInventory(pnum + P_TIDSTART, "DnD_SurgecastReady"))
		wait = SpendSurgecast(pnum, spell);

	// The press already holds the casting lock; Quicktongue's repeats take their own, and an instant cast lets it go now.
	if(wait > 0) {
		if(depth)
			BeginSpellCasting(pnum);
		int casting = StartSpellCastBar(pnum, spell, wait);

		if(ally) {
			// Re-aimed every tic for the first stretch of the bar, then COMMITTED. The last quarter is
			// locked in so the spell lands where the marker said it would, rather than on whoever the
			int track = wait * DND_SPELLLOCK_TRACKPCT / 100;
			for(int e = 0; e < wait; ++e) {
				if(e < track)
					SetSpellLockTarget(pnum, GetSpellAllyTarget(pnum, spell == SPL_REJUVENATION));
				Delay(const:1);
			}
		}
		else
			Delay(wait);

		EndSpellCastBar(pnum, casting);
		EndSpellCasting(pnum);

		// The box goes now; WHO was locked is still needed by the cast below.
		if(ally)
			DropSpellLockMarker(pnum);
	}
	else if(!depth && !proc)
		EndSpellCasting(pnum);

	if(wait <= 0 && ally)
		// No bar to lock during -- resolve it on the spot, and no marker is ever shown.
		SetSpellLockTarget(pnum, GetSpellAllyTarget(pnum, spell == SPL_REJUVENATION));

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
				temp = ScaleSpellArea(pnum, spell, GetSpellValue(pnum, spell, SPELLVAL_RADIUS) << 16);
				for(i = 0; i < InformationInLevel[LEVELINFO_TID_MONSTER]; ++i) {
					int other = UsedMonsterTIDs[i];
					if(other == victim || !IsActorAlive(other) || !IsSpellEnemy(other))
						continue;
					if(fdistance(victim, other) <= temp)
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
			SpawnSpellFan(pnum, spell, "Spell_SearingBond", DND_SEARINGBOND_SPEED);
		break;

		case SPL_FIREJET:
			// The jet throws its own sideways flames as it flies, so this only has to launch it.
			SpawnSpellFan(pnum, spell, "Spell_FireJet", DND_FIREJET_SPEED);
		break;

		case SPL_RIGHTEOUSFIRE:
			CastRighteousFire(pnum);
		break;

		case SPL_MOLTENBOULDER_S:
			for(i = 0; i < 1 + GetSpellExtraProjectiles(pnum, spell); ++i)
				CastMoltenBoulder(pnum, FanOffset(i, DND_SPELL_FANSPREAD));
		break;

		case SPL_FLAMEPILLAR:
			// The pillar lays its own flame down as it advances, so this only has to launch it.
			// Rank 10 runs a parallel pair instead of one, offset sideways rather than fanned --
			// they are meant to sweep a corridor abreast, not diverge.
			pillar_life = Max(1, GetSpellDurationTics(pnum, spell) / DND_FLAMEPILLAR_LOOPTICS);
			// Spawners, so "+X projectiles" adds pillars: all abreast, two SIDEOFFs apart, centred on the aim.
			count = 1 + SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_HIGH) + GetSpellExtraProjectiles(pnum, spell);
			for(i = 0; i < count; ++i)
				SpawnSpellProjectile(pnum, spell, "Spell_FlamePillar", DND_FLAMEPILLAR_SPEED, 0, 0,
					(2 * i - (count - 1)) * DND_FLAMEPILLAR_SIDEOFF, pillar_life);
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

		case SPL_FROSTBITE:
			ACS_NamedExecuteAlways("DnD Frostbite Cast", 0, pnum);
		break;

		case SPL_BLIZZARD:
			ACS_NamedExecuteAlways("DnD Blizzard", 0, pnum);
		break;

		case SPL_WINTERORB:
			// One orb always: Winter Orb's "+X projectiles" goes into its throws instead, in "DnD Winter Orb Throw".
			SpawnSpellProjectile(pnum, spell, "Spell_WinterOrb", DND_WINTERORB_SPEED);
		break;

		case SPL_GLACIALCASCADE:
			CastGlacialCascade(pnum);
		break;

		case SPL_SHIVERINGARMOR:
			CastShiveringArmor(pnum);
		break;

		// The aim and footing are read here, as the cast ends, rather than whenever the script gets to run.
		case SPL_AVALANCHE:
			RecordAvalancheAim(pnum);
			ACS_NamedExecuteAlways("DnD Avalanche", 0, pnum);
		break;

		case SPL_CHARGEDBOLT:
			CastChargedBolt(pnum);
		break;

		case SPL_SPARK:
			ACS_NamedExecuteAlways("DnD Spark", 0, pnum);
		break;

		case SPL_SHOCK:
			CastShock(pnum);
		break;

		case SPL_TELEKINESIS:
			CastTelekinesis(pnum);
		break;

		case SPL_FORKEDLIGHTNING:
			CastForkedLightning(pnum);
		break;

		case SPL_CHAINLIGHTNING:
			CastChainLightning(pnum);
		break;

		case SPL_CRACKLINGLANCE:
			CastCracklingLance(pnum);
		break;

		case SPL_CONDUCTIVITY:
			ACS_NamedExecuteAlways("DnD Conductivity Cast", 0, pnum);
		break;

		case SPL_BLINK:
			CastBlink(pnum);
		break;

		case SPL_OVERCHARGE:
			CastOvercharge(pnum);
		break;

		case SPL_GALVANICSURGE:
			CastGalvanicSurge(pnum);
		break;

		case SPL_SURGECAST:
			CastSurgecast(pnum);
		break;

		case SPL_STORMBRAND:
			ACS_NamedExecuteAlways("DnD Storm Brand", 0, pnum);
		break;

		case SPL_MONSOON:
			ACS_NamedExecuteAlways("DnD Monsoon", 0, pnum);
		break;

		case SPL_THUNDERCLAP:
			ACS_NamedExecuteAlways("DnD Thunder Clap", 0, pnum);
		break;

		case SPL_STORMCALL:
			ACS_NamedExecuteAlways("DnD Storm Call", 0, pnum);
		break;

		case SPL_ENERGYBLADE:
			CastEnergyBlade(pnum);
		break;

		case SPL_SHRAPNEL:
			CastShrapnel(pnum);
		break;

		case SPL_NOXIOUSBLAST:
			ACS_NamedExecuteAlways("DnD Noxious Blast", 0, pnum);
		break;

		case SPL_ROCKSLIDE:
			ACS_NamedExecuteAlways("DnD Rockslide", 0, pnum);
		break;

		case SPL_BLOCKADE:
			ACS_NamedExecuteAlways("DnD Blockade", 0, pnum);
		break;

		case SPL_TOXICTRAP:
			CastToxicTrap(pnum);
		break;

		case SPL_BALEFULINFUSION:
			CastBalefulInfusion(pnum);
		break;

		case SPL_VINESOFTORMENT:
			ACS_NamedExecuteAlways("DnD Vines Of Torment", 0, pnum);
		break;

		case SPL_VENOMGYRE:
			CastVenomGyre(pnum);
		break;

		case SPL_ROCKARMOR:
			CastRockArmor(pnum);
		break;

		case SPL_REJUVENATION:
			CastRejuvenation(pnum);
		break;

		case SPL_POISONNOVA:
			CastPoisonNova(pnum);
		break;

		case SPL_CORROSIVEBREATH:
			ACS_NamedExecuteAlways("DnD Corrosive Breath", 0, pnum);
		break;

		case SPL_CRUSHINGROCKS:
			ACS_NamedExecuteAlways("DnD Crushing Rocks", 0, pnum);
		break;

		case SPL_ROCKGOLEM:
			ACS_NamedExecuteAlways("DnD Summon Rock Golem", 0, pnum);
		break;

		case SPL_GEYSER:
			ACS_NamedExecuteAlways("DnD Geyser", 0, pnum);
		break;

		case SPL_UNLEASHEARTH:
			ACS_NamedExecuteAlways("DnD Unleash Earth", 0, pnum);
		break;

		case SPL_VULNERABILITY:
		case SPL_DESPAIR:
			ACS_NamedExecuteAlways("DnD Earth Curse", 0, pnum, spell);
		break;

		case SPL_UPHEAVAL:
			ACS_NamedExecuteAlways("DnD Upheaval", 0, pnum);
		break;

		case SPL_THREADEDVOLLEY:
			ACS_NamedExecuteAlways("DnD Threaded Volley", 0, pnum);
		break;

		case SPL_LIGHTNINGSPEAR:
			PlaySound(pnum + P_TIDSTART, "Spell/LightningSpearCast", CHAN_7);
			SpawnSpellFan(pnum, spell, "Spell_LightningSpear", DND_LIGHTNINGSPEAR_SPEED);
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
			SpawnSpellFan(pnum, spell, "Spell_IceBolt", DND_ICEBOLT_SPEED);
		break;

		case SPL_FREEZINGPULSE:
			CastFreezingPulse(pnum);
		break;

		case SPL_GLACIALSPIKE:
			SpawnSpellFan(pnum, spell, "Spell_GlacialSpike", DND_GLACIALSPIKE_SPEED);
		break;

		case SPL_ICESHIELD_S:
			ACS_NamedExecuteAlways("DnD Ice Shield", 0, pnum);
		break;

		case SPL_CHILLINGBREATH:
			ACS_NamedExecuteAlways("DnD Chilling Breath", 0, pnum);
		break;

		case SPL_CREEPINGFROST:
			SpawnSpellFan(pnum, spell, "Spell_CreepingFrost", DND_CREEPINGFROST_SPEED);
		break;

		case SPL_FROSTSHARDS:
			ACS_NamedExecuteAlways("DnD Frost Shards", 0, pnum);
		break;

		case SPL_ICENOVA:
			ACS_NamedExecuteAlways("DnD Ice Nova", 0, pnum);
		break;

		case SPL_GUSTOFFROST:
			ACS_NamedExecuteAlways("DnD Gust Of Frost", 0, pnum);
		break;

		case SPL_FROSTBOMB:
			ACS_NamedExecuteAlways("DnD Frost Bomb", 0, pnum);
		break;

		case SPL_ICEGOLEM:
			ACS_NamedExecuteAlways("DnD Summon Ice Golem", 0, pnum);
		break;

		case SPL_ICESPEAR:
			ACS_NamedExecuteAlways("DnD Ice Spear", 0, pnum);
		break;

		case SPL_PYROBLAST:
			// One bolt. The rank 10 fragments are not thrown here -- the explosion spawns them from
			// its own state, so they come off the blast rather than off the caster.
			SpawnSpellFan(pnum, spell, "Spell_Pyroblast", DND_PYROBLAST_SPEED);
		break;

		case SPL_FIREBALL:
			// AMOUNT is the base count, so a synergy that raises it still counts; the rank 5 and rank 10
			// thresholds each add the one extra "to the side" their text promises. Damage is NOT worked
			// out here -- each ball resolves its own at impact off user_spellid, which SpawnSpellProjectile
			// stamps on it.
			count = GetSpellValue(pnum, spell, SPELLVAL_AMOUNT) +
				SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_LOW) +
				SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_HIGH) + GetSpellExtraProjectiles(pnum, spell);

			for(i = 0; i < count; ++i)
				SpawnSpellProjectile(pnum, spell, "Spell_Fireball", DND_FIREBALL_SPEED, 0,
					FanOffset(i, DND_FIREBALL_SPREAD));
		break;
	}

	// Bladedancer and Fighter open their windows when the cast lands.
	OpenGemCastWindows(pnum, spell);

	// Quicktongue: once more, cast bar and its rules included, free -- TryCastSpell billed the first one.
	if(!proc && ct > 0 && depth < GetSpellGemMod(pnum, spell, GEMSTAT_REPEAT) && !(SpellDefs[spell].flags & SPLF_TOGGLE))
		ACS_NamedExecuteAlways("DnD Spell Cast", 0, spell, pnum, depth + 1);
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

// "It shatters on a wall": one tic with no movement at all means it is wedged.
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

		r = ScaleSpellArea(pnum, SPL_RIGHTEOUSFIRE, GetSpellValue(pnum, SPL_RIGHTEOUSFIRE, SPELLVAL_RADIUS) << 16);

		// A percent of the MAXIMUM health and shield, not the current. Off current health the figure
		// fell as the burn ate into it -- taking half of what was left every second, which is
		// repeated halving rather than a steady degeneration, and it made the damage dealt decay
		// along with it. Off the caps it is a flat rate for as long as the spell is up.
		dmg = (GetSpawnHealth(false, pnum) + GetPlayerEnergyShieldCap(pnum)) *
			GetSpellValue(pnum, SPL_RIGHTEOUSFIRE, SPELLVAL_DAMAGE) / 100;

		// Pricing scales what it BURNS, never what it costs: dmg is also the upkeep, so the enemy figure is
		// taken apart from it. A tick, so it prices through the DoT pile.
		enemy_dmg = PriceSpellDamage(pnum, SPL_RIGHTEOUSFIRE, dmg, DND_WDMG_ISRADIUSDMG, true);

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
			if(!IsActorAlive(v) || !IsSpellEnemy(v))
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
	int r = ScaleSpellArea(pnum, SPL_IMMOLATION, GetSpellValue(pnum, SPL_IMMOLATION, SPELLVAL_RADIUS) << 16) >> 16;

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
		Max(1, ApplySpellGemMore(pnum, SPL_FIREDEMON, GEMSTAT_MORE_MINIONLIFE, GetSpellValue(pnum, SPL_FIREDEMON, SPELLVAL_DAMAGE))));

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

	int n = GetSpellValue(pnum, SPL_VOLCANO, SPELLVAL_DAMAGE2) + GetSpellExtraProjectiles(pnum, SPL_VOLCANO);
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
	dmg = PriceSpellDamage(pnum, SPL_ANNIHILUS, dmg, DND_WDMG_ISRADIUSDMG);
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
		if(!IsActorAlive(v) || !IsSpellEnemy(v))
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
	// BOTH ends go through ScaleSpellArea, so area of effect gear widens the whole ramp
	// rather than only its start -- a 512 blast is 512 before area modifiers and more after.
	int rbase = ScaleSpellArea(pnum, SPL_ANNIHILUS, GetSpellValue(pnum, SPL_ANNIHILUS, SPELLVAL_RADIUS) << 16);
	int rfull = ScaleSpellArea(pnum, SPL_ANNIHILUS, DND_ANNIHILUS_FULLRADIUS << 16);

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
				AnnihilusDetonate(pnum, tid, added, r);
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
	int cur = GetSpellAuraRadius(pnum, spell);

	// An unranked or radiusless row leaves the art exactly as DECORATE declared it.
	if(base <= 0 || cur <= 0) {
		SetResultValue(1.0);
		Terminate;
	}

	// The caller already applied the general area gear; this adds spell area gear to that pool, as the reach does.
	int general = ScalePlayerAoERadius(pnum, 1.0, DND_AOESRC_NONWEAPON);
	int pool = general > 0 ? FixedDiv(ScaleSpellArea(pnum, spell, 1.0), general) : 1.0;
	SetResultValue(FixedMul((cur << 16) / base, pool));
}

#endif
