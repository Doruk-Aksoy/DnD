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

#define DND_SPELL_INT_ATTUNE 50		// 0.5 per point, matching the legacy skills

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
Script "DnD Spell Damage" (int which, int flags) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	// All read before the scale script runs: AdjustDamageRetrievePointers moves the activator off
	// the spell actor and onto the caster.
	int spell = GetUserVariable(0, "user_spellid");
	int dmg = GetSpellValue(pnum, spell, which);
	int dtype = GetSpellDamageType(spell);
	int cat = GetSpellDamageCategory(spell);

	if(dmg <= 0) {
		SetResultValue(0);
		Terminate;
	}

	dmg = ACS_NamedExecuteWithResult("DND Player Damage Scale", dmg, cat,
		DND_WDMG_USETARGET | DND_WDMG_ISSPELL | flags, DND_SPELL_INT_ATTUNE << INT_ATTUNE_BITS);

	SetResultValue(PackSpellDamage(dmg, dtype, spell));
}

// Companion to the above for explosive spells. PSTAT_SPELL_AOE is already in the value GetSpellValue
// hands back; this adds the universal area stat on top, the same as any other non weapon explosion.
Script "DnD Spell Radius" (int which) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	int r = GetSpellValue(pnum, GetUserVariable(0, "user_spellid"), which);
	SetResultValue(ACS_NamedExecuteWithResult("DnD Explosion Radius Retrieve", r, 1, DND_AOESRC_NONWEAPON));
}

#endif
