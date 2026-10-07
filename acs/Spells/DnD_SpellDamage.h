#ifndef DND_SPELLDAMAGE_IN
#define DND_SPELLDAMAGE_IN

// Spells run the FULL player damage pipeline, entered on the same no-weapon lane the legacy skills
// use: attribute scaling, % damage, buffs, crit, resists, status effects and damage numbers all
// apply. Only the weapon specific terms are skipped, because there is no weapon to read them from.
//
// The element is the spell's own rather than a blanket magical type -- a fire spell meets fire
// resist and can ignite, a cold spell chills. DND_DAMAGEFLAG_ISSPELL rides alongside it and is what
// spell-only modifiers key off, so the two are independent: being a spell says nothing about what
// element it deals, and dealing fire says nothing about whether an energy shield stops it.
//
// This lives here rather than beside the spell tables because DND_DAMAGETYPE_ is declared in
// DnD_Damage.h and enums do not forward reference.

// 0.4% spell damage per point of intellect, in hundredths of a percent -- the unit "DND Player
// Damage Scale" unpacks back to a fixed fraction.
#define DND_SPELL_INT_ATTUNE 40

// The same figure as the fixed fraction HandleStatBonus takes directly, derived from the one
// above so the two cannot drift. Shift before dividing, or the result truncates low.
#define DND_SPELL_INT_SCALE ((DND_SPELL_INT_ATTUNE << 16) / 10000)

// Every tree deals its own element. A spell that deals something else takes a case of its own above
// the tree switch -- none do yet.
int GetSpellDamageType(int spell) {
	switch(SpellDefs[spell].tree) {
		case DND_SKILLTREE_FIRE:		return DND_DAMAGETYPE_FIRE;
		case DND_SKILLTREE_ICE:			return DND_DAMAGETYPE_ICE;
		case DND_SKILLTREE_LIGHTNING:	return DND_DAMAGETYPE_LIGHTNING;
		case DND_SKILLTREE_BLACK:		return DND_DAMAGETYPE_OCCULT;
		case DND_SKILLTREE_ARCANE:		return DND_DAMAGETYPE_ENERGY;
	}

	// Earth, Chaos and Combat carry no element of their own.
	return DND_DAMAGETYPE_PHYSICAL;
}

int GetSpellDamageCategory(int spell) {
	return GetDamageCategory(GetSpellDamageType(spell), 0);
}

// The word the event handler unpacks: damage, then type, then the spell id in the slot a weapon hit
// puts its weapon id. wep_neg is what keeps the weapon lookups off that slot.
int PackSpellDamage(int dmg, int dtype, int spell) {
	return Min(Max(dmg, 0), SPELLDMG_MASK) | (dtype << SPELL_DMG_SHIFT) | ((spell & SPELLID_MASK) << SPELL_ID_SHIFT);
}

// The caster, from wherever SetupSpellActor put it. -1 when the actor has outlived its owner.
int GetSpellActorOwner() {
	int owner = GetActorProperty(0, APROP_SCORE);
	if(!owner)
		owner = GetActorProperty(0, APROP_TARGETTID);

	int pnum = owner - P_TIDSTART;
	if(pnum < 0 || pnum >= MAXPLAYERS)
		return -1;

	return pnum;
}

// The one damage entry point for a spell actor. Everything is read off the actor, so a spell's
// DECORATE never repeats a number the tables already own:
//
//     Damage(ACS_NamedExecuteWithResult("DnD Spell Damage", SPELLVAL_DAMAGE))
//
// Resolved here at impact rather than stamped at spawn, so a synergy or a +level that landed while
// the projectile was in the air still counts. Pass DND_WDMG_ISRADIUSDMG in flags for an explosion.
//
// div splits the result, for a spell whose text gives a fragment of its own damage to something else
// -- Pyroblast's rank 10 fireballs take an eighth. Applied AFTER the scale, so the fragment is an
// eighth of what the parent would really have dealt rather than an eighth of its table row.
// The intellect attunement, for spells that price themselves and call HandleDamageDeal straight.
//
// The projectile and explosion path picks this up inside "DnD Spell Damage"; without this the
// two halves of the tree disagreed, and whether a spell scaled with INT came down to how it
// happened to be implemented rather than to anything about the spell.
//
// DAMAGE OVER TIME IS INCLUDED, deliberately. HandleNonWeaponDamageScale withholds the attribute
// bonus from anything flagged DOT, but that rule is about WEAPONS: a weapon ignite is derived from
// a hit that already took the attunement, so counting it again would be a second helping of the
// same stat. A spell tick is not -- Immolation and Blaze are priced straight off the spell row and
// Righteous Fire off the health pool, so there is no earlier application to double.
//
// These spells reach HandleDamageDeal directly and never pass through that block at all, so this
// is their only application. Intellect is meant to read as increased spell damage per point, and
// a stat that silently skipped three spells would not.
int ApplySpellIntScaling(int pnum, int dmg) {
	return dmg * (100 + HandleStatBonus(pnum, 0, 0, DND_SPELL_INT_SCALE, false)) / 100;
}

// MORE spell damage, from any source granting BUFF_SPELLDAMAGE -- Righteous Fire today, an item or a
// perk tomorrow. Multiplicative, so it is applied here rather than joining the increased pile.
//
// A spell OPTS IN by calling this. Spells priced through a DECORATE Damage expression get it from
// "DnD Spell Damage" below; the ones that price themselves and call HandleDamageDeal straight have to
// ask, which is why Scorching Ray and Immolation do and Infernal Strike deliberately does not.
//
// Righteous Fire is refused its OWN grant here rather than at the call sites, so no future caller can
// reintroduce the feedback: the buff is what the burn BUYS, and its damage is already a percent of
// the health it costs, so compounding it would hit the same pool twice.
int ApplySpellMoreDamage(int pnum, int spell, int dmg) {
	if(spell == SPL_RIGHTEOUSFIRE)
		return dmg;

	int more = pbuffs[pnum].buff_net_values[BUFF_SPELLDAMAGE].multiplicative;
	if(!more || more == 1.0)
		return dmg;

	return FixedMul(dmg, more);
}

// user_rankat lets an actor price itself at a rank OTHER than the caster's live allocation.
// Declared on the one actor that needs it, and GetUserVariable returns 0 for a class that does not
// declare it at all, so every other spell reads 0 and keeps resolving against the live rank.
Script "DnD Spell Damage" (int which, int flags, int div) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	// All read before the scale script runs: AdjustDamageRetrievePointers moves the activator off
	// the spell actor and onto the caster.
	int spell = GetUserVariable(0, "user_spellid");
	int dmg = GetSpellValue(pnum, spell, which, GetUserVariable(0, "user_rankat"));
	int dtype = GetSpellDamageType(spell);
	int cat = GetSpellDamageCategory(spell);

	if(dmg <= 0) {
		SetResultValue(0);
		Terminate;
	}

	dmg = ACS_NamedExecuteWithResult("DND Player Damage Scale", dmg, cat,
		DND_WDMG_USETARGET | DND_WDMG_ISSPELL | flags, DND_SPELL_INT_ATTUNE << INT_ATTUNE_BITS);

	// Heart of Fire rank 5: a spell that its proc made ready hits harder for a few seconds. MORE, so
	// it goes on after the scale rather than into it.
	int prime = GetSpellPrimes().ends[pnum][spell];
	if(prime && Timer() <= prime)
		dmg = dmg * (100 + GetSpellValue(pnum, SPL_HEARTOFFIRE, SPELLVAL_AMOUNT)) / 100;

	// Beside the prime for the same reason: both are MORE, so both multiply after the scale.
	dmg = ApplySpellMoreDamage(pnum, spell, dmg);

	// Never below 1: a fragment of a small hit should still be a hit, not nothing.
	if(div > 1)
		dmg = Max(1, dmg / div);

	SetResultValue(PackSpellDamage(dmg, dtype, spell));
}

// Whether the owner of this spell actor has reached a rank threshold. DECORATE cannot read the rank
// tables, so anything gated on a threshold asks through here.
Script "DnD Spell Threshold" (int thresh) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	SetResultValue(SpellThresholdMet(pnum, GetUserVariable(0, "user_spellid"), thresh));
}

// How many flight frames the jet gets, from its own DURATION row. Driven off the table rather than
// an unrolled run of identical states, so a duration modifier or a synergy lengthens the jet -- and
// its reach with it -- instead of needing more copy pasted frames.
//
// FRAMETICS must match the duration on the flight frame in DECORATE, or the jet lasts a different
// time than its row claims.
#define DND_FIREJET_FRAMETICS 2

Script "DnD Fire Jet Lifetime" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(1);
		Terminate;
	}

	int tics = GetSpellDurationTics(pnum, GetUserVariable(0, "user_spellid"));

	// At least one, so a zeroed row still produces a jet rather than nothing at all.
	SetResultValue(Max(1, tics / DND_FIREJET_FRAMETICS));
}

// Fire Jet's sideways flames, thrown as it flies. Spawned from ACS rather than with A_SpawnItemEx
// because user variables do NOT transfer on a DECORATE spawn: a side spawned that way carries no
// spell id, prices itself as spell 0 and resolves as Blaze.
Script "DnD Fire Jet Sides" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	int spell = GetUserVariable(0, "user_spellid");
	int x = GetActorX(0), y = GetActorY(0), z = GetActorZ(0);
	int a = GetActorAngle(0);
	int tid = TEMPORARY_SPELL_TID + pnum;
	int i;

	// One to each side. Angle is set after the spawn rather than through SpawnForced's own argument,
	// which takes byte angles and would wrap a 16.16 one into nonsense.
	for(i = 0; i < 2; ++i) {
		if(!SpawnForced("Spell_FireJetSide", x, y, z, tid, 0))
			continue;

		SetActorAngle(tid, a + (i ? -0.25 : 0.25));
		SetupSpellActor(tid, pnum, spell);
		Thing_ChangeTID(tid, 0);
	}

	SetResultValue(0);
}

// Flame Pillar's standing flame, dropped as the advancing projectile travels. Spawned from ACS
// for the same reason Fire Jet's sides are -- user variables do NOT transfer on a DECORATE
// spawn, so an A_CustomMissile pillar carries no spell id and resolves as spell 0, Blaze.
//
// Placed on the FLOOR under the projectile rather than at its height: the projectile flies at
// eye level and climbs terrain with STEPMISSILE, and a pillar of flame starts at the ground.
Script "DnD Flame Pillar Drop" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	int spell = GetUserVariable(0, "user_spellid");
	int tid = TEMPORARY_SPELL_TID + pnum;

	if(SpawnForced("Spell_FlamePillarDamage", GetActorX(0), GetActorY(0), GetActorFloorZ(0), tid, 0)) {
		SetupSpellActor(tid, pnum, spell);
		Thing_ChangeTID(tid, 0);
	}

	SetResultValue(0);
}

// Every number an exploding spell needs, in ONE pass. These were three separate calls from the
// Death state, each re-resolving the owner and the spell id for the same burst.
//
// Pinned to a tid first and written by tid, never by activator: resolving the damage moves the
// activator off the projectile and onto the caster, so anything written afterwards by activator
// lands on the wrong actor. Radius is resolved BEFORE damage for the same reason -- it reads the
// spell id off the activator.
// div divides the result, for a blast that is a FRACTION of the field it reads -- Molten
// Boulder's rank 10 shards split its impact six ways. Passed to "DnD Spell Damage" as its own
// div argument, never applied to what that returns: the return is a PACKED word carrying the
// damage, the element and the spell id together, so dividing it corrupts all three.
//
// which selects the damage field, so a spell with two blasts can price each separately --
// Molten Boulder rolls for SPELLVAL_DAMAGE and lands for SPELLVAL_DAMAGE2. radius_pct scales
// SPELLVAL_RADIUS for the same reason. Both default to 0 because DECORATE passes 0 for the
// arguments it omits, and 0 reads as "SPELLVAL_DAMAGE at the full radius" -- which is exactly
// what every existing caller wants.
Script "DnD Spell Explosion Setup" (int which, int radius_pct, int div) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	int had = ActivatorTID();
	if(!had)
		Thing_ChangeTID(0, DND_SPELLEXP_TID);

	int me = had ? had : DND_SPELLEXP_TID;
	int spell = GetUserVariable(me, "user_spellid");

	// Self damage is OPT IN. A_Explode does include the source -- XF_HURTSOURCE is set -- but the
	// handler drops radius damage on the shooter outright unless DND_DAMAGEFLAG_BLASTSELF is on it,
	// and nothing used to set it. That is why Pyroblast never hurt anyone at any rank.
	int flags = 0;

	// "No longer damages you" is Pyroblast's rank 5. Below it, the blast is the spell's own cost.
	if(spell == SPL_PYROBLAST && !SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_LOW))
		flags = DND_DAMAGEFLAG_BLASTSELF;

	// Fire Jet only bursts at all from rank 5. The caller branches on what this returns.
	if(spell == SPL_FIREJET && !SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_LOW)) {
		if(!had)
			Thing_ChangeTID(me, 0);
		SetResultValue(0);
		Terminate;
	}

	// "A direct hit explodes for HALF its damage." Only when the caller named no divisor of its
	// own, so an explicit one always wins.
	if(!div && spell == SPL_FIREJET)
		div = 2;

	int radius = ACS_NamedExecuteWithResult("DnD Spell Radius", SPELLVAL_RADIUS);
	if(radius_pct > 0)
		radius = radius * radius_pct / 100;

	// The core that takes the hit at full strength, with no distance falloff. A PERCENT of the blast,
	// not a distance -- so it is taken off the already scaled radius and tracks area modifiers for
	// free, and a spell keeps the same shape however big its explosion grows.
	//
	// Zero on every spell that does not want one, and A_Explode reads a zero core as "falloff all the
	// way in", so this costs the others nothing.
	int full = GetSpellValue(pnum, spell, SPELLVAL_AMOUNT, GetUserVariable(0, "user_rankat"));
	if(full > 0)
		full = radius * Min(full, 100) / 100;

	int dmg = ACS_NamedExecuteWithResult("DnD Spell Damage", which, DND_WDMG_ISRADIUSDMG, div);

	SetUserVariable(me, "user_expdmg", dmg);
	SetUserVariable(me, "user_expradius", radius);
	SetUserVariable(me, "user_expflags", flags);
	SetUserVariable(me, "user_fullexpradius", full);

	// Only ours to give back if we took it.
	if(!had)
		Thing_ChangeTID(me, 0);

	// Non zero: the caller may be gating its burst on this.
	SetResultValue(1);
}

// Companion to the above for explosive spells. PSTAT_SPELL_AOE is already in the value GetSpellValue
// hands back; this adds the universal area stat on top, the same as any other non weapon explosion.
Script "DnD Spell Radius" (int which) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	int r = GetSpellValue(pnum, GetUserVariable(0, "user_spellid"), which,
		GetUserVariable(0, "user_rankat"));
	SetResultValue(ACS_NamedExecuteWithResult("DnD Explosion Radius Retrieve", r, 1, DND_AOESRC_NONWEAPON));
}

#endif
