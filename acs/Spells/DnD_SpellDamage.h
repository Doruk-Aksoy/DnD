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

// Spell attunement in hundredths of a percent per point; every profile sums to 0.4%.
#define DND_SPELL_ATTUNE_TOTAL 40
#define DND_SPELL_ATTUNE_MAJOR 25
#define DND_SPELL_ATTUNE_MINOR 15
#define DND_SPELL_ATTUNE_EVEN 20
#define DND_SPELL_ATTUNE_SLIGHT 10
#define DND_SPELL_ATTUNE_DOMINANT 30

// STR, DEX and INT a byte each, the layout "DND Player Damage Scale" unpacks.
#define PACK_SPELL_ATTUNE(S, D, I) ((S) | ((D) << DEX_ATTUNE_BITS) | ((I) << INT_ATTUNE_BITS))

// Every tree deals its own element. A spell that deals something else takes a case of its own above
// the tree switch -- none do yet.
int GetSpellDamageType(int spell) {
	// Gust's only damage is its slam, which is physical, so cold increases must not price it.
	if(spell == SPL_GUSTOFFROST)
		return DND_DAMAGETYPE_PHYSICAL;
	// Implosion's corpse goes up in fire
	if(spell == SPL_IMPLOSION)
		return DND_DAMAGETYPE_FIRE;

	// Earth's poison half; its rock spells stay physical below.
	switch(spell) {
		case SPL_NOXIOUSBLAST:
		case SPL_TOXICTRAP:
		case SPL_VENOMGYRE:
		case SPL_VINESOFTORMENT:
		case SPL_CORROSIVEBREATH:
		case SPL_POISONNOVA:
		case SPL_GEYSER:
		return DND_DAMAGETYPE_POISON;
	}

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

// A spell's own attunement where it differs from its tree's, or 0.
int GetSpellAttunementOverride(int spell) {
	switch(spell) {
		// cast around the caster, or riding a melee hit
		case SPL_IMMOLATION:
		case SPL_RIGHTEOUSFIRE:
		case SPL_HEATSHIELD:
		case SPL_SEARINGBOND:
		case SPL_VOLCANO:
		case SPL_THUNDERCLAP:
		case SPL_ICENOVA:
		// the undead that fight up close
		case SPL_ZOMBIE:
		case SPL_KNIGHT:
		case SPL_SLAYERKNIGHT:
		case SPL_DEATHKNIGHT:
		case SPL_DOOMGUARD:
		return PACK_SPELL_ATTUNE(DND_SPELL_ATTUNE_MINOR, 0, DND_SPELL_ATTUNE_MAJOR);

		case SPL_INFERNALSTRIKE:
		case SPL_GUSTOFFROST:
		case SPL_GLACIALCASCADE:
		case SPL_AVALANCHE:
		case SPL_ICEGOLEM:
		return PACK_SPELL_ATTUNE(DND_SPELL_ATTUNE_MAJOR, 0, DND_SPELL_ATTUNE_MINOR);

		// fast or swarming
		case SPL_ICEBOLT:
		case SPL_GLACIALSPIKE:
		case SPL_FROSTSHARDS:
		case SPL_ICESPEAR:
		case SPL_FREEZINGPULSE:
		case SPL_CHILLINGBREATH:
		case SPL_CREEPINGFROST:
		case SPL_WINTERORB:
		case SPL_GHAST:
		case SPL_CARRIONCROWS:
		return PACK_SPELL_ATTUNE(0, DND_SPELL_ATTUNE_MINOR, DND_SPELL_ATTUNE_MAJOR);

		// Black's damage spells: mostly Intellect, a touch of speed or of raw force
		case SPL_PLAGUE:
		case SPL_DARKWIND:
		case SPL_ESSENCETHEFT:
		case SPL_FINGEROFDEATH:
		case SPL_CONTAGION:
		case SPL_IMPLOSION:
		case SPL_NETHERWALK:
		case SPL_GRAVEGRASP:
		return PACK_SPELL_ATTUNE(0, DND_SPELL_ATTUNE_SLIGHT, DND_SPELL_ATTUNE_DOMINANT);
		case SPL_SOULSTORM:
		case SPL_DEATHANDDECAY:
		return PACK_SPELL_ATTUNE(DND_SPELL_ATTUNE_SLIGHT, 0, DND_SPELL_ATTUNE_DOMINANT);
	}
	return 0;
}

int GetSpellAttunementPacked(int spell) {
	int res = GetSpellAttunementOverride(spell);
	if(res)
		return res;

	switch(SpellDefs[spell].tree) {
		case DND_SKILLTREE_LIGHTNING:
		return PACK_SPELL_ATTUNE(0, DND_SPELL_ATTUNE_MINOR, DND_SPELL_ATTUNE_MAJOR);
		case DND_SKILLTREE_EARTH:
			if(GetSpellDamageType(spell) == DND_DAMAGETYPE_POISON)
				return PACK_SPELL_ATTUNE(0, DND_SPELL_ATTUNE_EVEN, DND_SPELL_ATTUNE_EVEN);
		return PACK_SPELL_ATTUNE(DND_SPELL_ATTUNE_MAJOR, 0, DND_SPELL_ATTUNE_MINOR);
	}
	return PACK_SPELL_ATTUNE(0, 0, DND_SPELL_ATTUNE_TOTAL);
}

// One stat's share in hundredths of a percent.
int GetSpellAttunementRaw(int spell, int stat) {
	int shift = stat == STAT_STR ? 0 : stat == STAT_DEX ? DEX_ATTUNE_BITS : INT_ATTUNE_BITS;
	return (GetSpellAttunementPacked(spell) >> shift) & ATTUNEMENT_MASK;
}

// The fixed fraction HandleStatBonus takes. Shift before dividing, or it truncates low.
int GetSpellAttunement(int spell, int stat) {
	return (GetSpellAttunementRaw(spell, stat) << 16) / 10000;
}

int GetSpellStatBonus(int pnum, int spell) {
	return HandleStatBonus(pnum, GetSpellAttunement(spell, STAT_STR), GetSpellAttunement(spell, STAT_DEX), GetSpellAttunement(spell, STAT_INT), false);
}

// A pet's hit through the level curve monsters take; the pet damage script and the spell panel share it.
int GetPetLevelDamage(int level, int base) {
	return base * (100 + (level > 1) * GetMonsterDMGScaleForLevel(level)) / 100;
}

// What the master adds: the summoning spell's attunement, increased minion damage (Summon Mastery's in its 2.5% steps)
// and the flat health conversion. The increases add up in tenths of a percent.
int ApplyPetMasterDamage(int pnum, int spell, int dmg) {
	int temp = PlayerModData[pnum].vals[PSTAT_MINION_DAMAGE];
	if(spell != -1)
		temp += GetSpellStatBonus(pnum, spell);
	temp = temp * 10 + (GetSpellValue(pnum, SPL_SUMMONMASTERY, SPELLVAL_DAMAGE) * 10 >> 16);
	dmg = dmg / 1000 * (1000 + temp) + dmg % 1000 * (1000 + temp) / 1000;

	if((temp = PlayerModData[pnum].vals[PSTAT_EX_PHYSDAMAGEPER_FLATHEALTH]))
		dmg = dmg * (100 + GetFlatHealthDamageFactor(temp)) / 100;
	return dmg;
}

// A pet's health from its base: the monster level curve, then increased minion health (Summon Mastery's too) in
// hundredths of a percent.
int GetPetScaledHealth(int pnum, int base, int level) {
	int add = level > 1 ? ApplyHPScale(base, GetMonsterHPScaleForLevel(level)) : 0;
	int pct = PlayerModData[pnum].vals[PSTAT_MINION_HEALTH] * 100 + (GetSpellValue(pnum, SPL_SUMMONMASTERY, SPELLVAL_DAMAGE2) * 100 >> 16);

	// Split, as dividing first gave nothing under 10000 health and multiplying first overflows.
	add += (base + add) / 10000 * pct + (base + add) % 10000 * pct / 10000;
	return base + Clamp_Between(add, 0, DND_MAX_MONSTER_HP - base);
}

// The word the event handler unpacks: damage, then type, then the spell id in the slot a weapon hit
// puts its weapon id. wep_neg is what keeps the weapon lookups off that slot.
int PackSpellDamage(int dmg, int dtype, int spell) {
	return Min(Max(dmg, 0), SPELLDMG_MASK) | (GetSpellElementCode(dtype) << SPELL_DMG_SHIFT) | ((spell & SPELLID_MASK) << SPELL_ID_SHIFT);
}

// The word's 3 bit element. A spell element not listed here packs as physical, so a new one needs a code.
int GetSpellElementCode(int dtype) {
	switch(dtype) {
		case DND_DAMAGETYPE_FIRE:		return 1;
		case DND_DAMAGETYPE_ICE:		return 2;
		case DND_DAMAGETYPE_LIGHTNING:	return 3;
		case DND_DAMAGETYPE_POISON:		return 4;
		case DND_DAMAGETYPE_OCCULT:		return 5;
		case DND_DAMAGETYPE_ENERGY:		return 6;
	}
	return 0;
}

int GetSpellElementType(int code) {
	switch(code) {
		case 1:	return DND_DAMAGETYPE_FIRE;
		case 2:	return DND_DAMAGETYPE_ICE;
		case 3:	return DND_DAMAGETYPE_LIGHTNING;
		case 4:	return DND_DAMAGETYPE_POISON;
		case 5:	return DND_DAMAGETYPE_OCCULT;
		case 6:	return DND_DAMAGETYPE_ENERGY;
	}
	return DND_DAMAGETYPE_PHYSICAL;
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

// The spell's attunement on its own, for a tick: the DoT path withholds the attribute bonus with the hit piles,
// and a spell tick, priced off its row, has no earlier hit that already took it. Called by PriceSpellDamage.
int ApplySpellAttunement(int pnum, int spell, int dmg) {
	return dmg * (100 + GetSpellStatBonus(pnum, spell)) / 100;
}

// MORE spell damage from BUFF_SPELLDAMAGE, applied by PriceSpellDamage. Righteous Fire is refused its own grant
// here, so no caller can reintroduce the feedback: the buff is what its burn buys.
int ApplySpellMoreDamage(int pnum, int spell, int dmg) {
	if(spell == SPL_RIGHTEOUSFIRE)
		return dmg;

	int more = pbuffs[pnum].buff_net_values[BUFF_SPELLDAMAGE].multiplicative;
	if(!more || more == 1.0)
		return dmg;

	return FixedMul(dmg, more);
}

// Which damage FIELD of a spell is a tick, not a hit.
bool IsSpellDoTField(int spell, int which) {
	return spell == SPL_CREEPINGFROST && which == SPELLVAL_DAMAGE2;
}

// A spell's own rank bonus worded "increased damage". Additive, so it joins the gear pile in the scale.
int GetSpellRankDamageIncrease(int pnum, int spell) {
	if(spell == SPL_VOLCANO)
		return DND_VOLCANO_R5_INC * SpellThresholdMet(pnum, SPL_VOLCANO, DND_SPELL_THRESH_LOW);
	return 0;
}

// "Increased minion damage" a minion's own spell or ailment joins its pile with: gear and Summon Mastery, in percent.
int GetMinionDamageIncrease(int pnum) {
	return PlayerModData[pnum].vals[PSTAT_MINION_DAMAGE] + (GetSpellValue(pnum, SPL_SUMMONMASTERY, SPELLVAL_DAMAGE) >> 16);
}

// A summon's own "more minion damage" (its row, Legion Mastery, Commander), as 100 + percent.
int GetMinionDamageMore(int pnum, int summon) {
	return 100 + GetPetSpellDamageBonus(pnum, GetSummonPetKind(summon));
}

// One spell hit or tick, priced the same whichever way it lands: "DnD Spell Damage" and the spells that call
// HandleDamageDeal themselves both come here. isDot takes the DoT pile instead; noMore is Infernal Strike's.
// minion is the summon spell + 1 of a minion casting it: minion damage joins, increased with the rest, more after.
int PriceSpellDamage(int pnum, int spell, int dmg, int scale_flags = 0, bool isDot = false, int victim = -1, bool noMore = false, int minion = 0) {
	// A hit takes its flat damage (spell worded and generic) before every increase; a tick none, as a weapon's DoT takes no attack flat.
	if(!isDot)
		dmg += GetSpellFlatDamage(pnum, GetSpellDamageCategory(spell), (scale_flags & DND_WDMG_ISRADIUSDMG) ? DND_DAMAGEFLAG_ISRADIUSDMG : 0);

	int own = GetSpellRankDamageIncrease(pnum, spell) + (minion ? GetMinionDamageIncrease(pnum) : 0);

	if(isDot) {
		// No hit fed a tick, so it takes the hit side increases (element, buffs, accuracy, spell damage) beside the DoT ones.
		dmg = GetGenericDoTDamage(pnum, dmg, victim, -1, true, GetHitSideDoTIncrease(pnum, GetSpellDamageCategory(spell), true) + own);
		scale_flags |= DND_WDMG_ISDOT;
	}

	dmg = HandleNonWeaponDamageScale(dmg, GetSpellDamageCategory(spell), DND_WDMG_ISSPELL | scale_flags,
		GetSpellAttunement(spell, STAT_STR), GetSpellAttunement(spell, STAT_DEX), GetSpellAttunement(spell, STAT_INT), pnum, own);

	// ISDOT dropped the attunement with the hit piles, so it goes back on at the rate a hit gets.
	if(isDot)
		dmg = ApplySpellAttunement(pnum, spell, dmg);

	dmg = FinishSpellDamage(pnum, spell, dmg, isDot, noMore);
	if(minion)
		dmg = dmg * GetMinionDamageMore(pnum, minion - 1) / 100;
	return dmg;
}

// A share-of-life spell's damage after resists: whole up to the share, then keep_pct of what gear adds past it, up to cap_pct of it.
int ScaleCappedShare(int dmg, int share, int keep_pct, int cap_pct) {
	if(dmg <= share)
		return dmg;
	int over = dmg - share;
	return Min(share + over / 100 * keep_pct + over % 100 * keep_pct / 100, share / 100 * cap_pct + share % 100 * cap_pct / 100);
}

// A flat a spell hit adds in another type (Sightless Vigil's physical): priced as the spell's hit, in that category.
int PriceSpellAddedDamage(int pnum, int spell, int dmg, int category, int scale_flags) {
	dmg = HandleNonWeaponDamageScale(dmg, category, DND_WDMG_ISSPELL | scale_flags,
		GetSpellAttunement(spell, STAT_STR), GetSpellAttunement(spell, STAT_DEX), GetSpellAttunement(spell, STAT_INT), pnum, GetSpellRankDamageIncrease(pnum, spell));
	return FinishSpellDamage(pnum, spell, dmg, false, false);
}

// A spell's poison tick: its attunement and own multipliers. "DnD Do Poison Damage" adds the poison, DoT and spell damage gear.
int PriceSpellPoison(int pnum, int spell, int dmg) {
	return FinishSpellDamage(pnum, spell, ApplySpellAttunement(pnum, spell, dmg), true, false);
}

// What every spell hit or tick takes after its scale.
// The spell's gems: MORE on every hit and tick, then on hits or on ticks alone, then Greater Resolute, Ravager and
// Powerful's more per charge held. noMore in FinishSpellDamage is the buff's, not theirs.
int ApplySpellGemDamage(int pnum, int spell, int dmg, bool isDot) {
	dmg = ApplySpellGemMore(pnum, spell, GEMSTAT_MORE_DAMAGE, dmg);
	dmg = ApplySpellGemMore(pnum, spell, isDot ? GEMSTAT_MORE_DOTDAMAGE : GEMSTAT_MORE_HITDAMAGE, dmg);
	return ApplySpellGemChargeDamage(pnum, spell, dmg);
}

int FinishSpellDamage(int pnum, int spell, int dmg, bool isDot, bool noMore) {
	// Heart of Fire rank 5: a spell its proc made ready hits harder for a few seconds. MORE, so after the scale.
	int prime = GetSpellPrimes().ends[pnum][spell];
	if(prime && Timer() <= prime)
		dmg = dmg * (100 + GetSpellValue(pnum, SPL_HEARTOFFIRE, SPELLVAL_AMOUNT)) / 100;

	dmg = ApplySpellGemDamage(pnum, spell, dmg, isDot);
	dmg = ApplyBlackEmpowerDamage(pnum, spell, dmg);

	if(!noMore)
		dmg = ApplySpellMoreDamage(pnum, spell, dmg);

	// Pain Attunement: MORE on low life, read off the pool that kills (the activator here is the spell actor).
	int v = PlayerModData[pnum].vals[PSTAT_SPELL_MORE_LOWLIFE];
	if(v && IsLethalPoolBelow(pnum, 50))
		dmg = dmg * (100 + v) / 100;
	return dmg;
}

// Prodigal Perfection: increased spell damage per 100 maximum mana, up to its cap. Read live, so mana gear counts at once.
int GetSpellDamageFromMana(int pnum) {
	int per = PlayerModData[pnum].vals[PSTAT_SPELLDMG_PERMANA];
	if(!per)
		return 0;
	return Min(PlayerModData[pnum].vals[PSTAT_SPELLDMG_PERMANACAP], per * (GetPlayerManaCap(pnum) / 100));
}

// An ailment seeded from a spell HIT is damage over time: the hit's own gem multiplier traded for the DoT one.
int GetSpellAilmentSeed(int pnum, int spell, int dmg) {
	int h = GetSpellGemMod(pnum, spell, GEMSTAT_MORE_HITDAMAGE);
	if(h && h > -1.0)
		dmg = FixedDiv(dmg, 1.0 + h);
	return ApplySpellGemMore(pnum, spell, GEMSTAT_MORE_DOTDAMAGE, dmg);
}

// All increased spell damage: gear, Wizardry and Prodigal Perfection. A spell's hits and its damage over time both take it.
int GetSpellDamageIncrease(int pnum) {
	return PlayerModData[pnum].vals[PSTAT_SPELL_DAMAGE] + GetSpellDamageFromMana(pnum) +
		GetHybridStatDamage(pnum, PSTAT_INC_SPELLDMG_PERSTR, STAT_STR) + GetHybridStatDamage(pnum, PSTAT_INC_SPELLDMG_PERDEX, STAT_DEX);
}

int ApplySpellGemChargeDamage(int pnum, int spell, int dmg) {
	int tid = pnum + P_TIDSTART, v;
	if((v = GetSpellGemMod(pnum, spell, GEMSTAT_ENDURANCEDAMAGE)))
		dmg = ScaleSpellValue(dmg, 100 + v * GetPlayerEnduranceCharges(tid, pnum));
	if((v = GetSpellGemMod(pnum, spell, GEMSTAT_FRENZYDAMAGE)))
		dmg = ScaleSpellValue(dmg, 100 + v * GetPlayerFrenzyCharges(tid, pnum));
	if((v = GetSpellGemMod(pnum, spell, GEMSTAT_POWERDAMAGE)))
		dmg = ScaleSpellValue(dmg, 100 + v * GetPlayerPowerCharges(tid, pnum));
	return dmg;
}

// A field's base value, rolled from its low end to itself when it is a range.
int RollSpellValue(int pnum, int spell, int which, int rank_at = 0) {
	int v = GetSpellValue(pnum, spell, which, rank_at);
	int lo = GetSpellMinField(spell, which);
	return lo == -1 ? v : random(GetSpellValue(pnum, spell, lo, rank_at), v);
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

	// Read off the spell actor, which stays the activator: the pricing below takes the caster as pnum.
	int spell = GetUserVariable(0, "user_spellid");
	int parent = GetUserVariable(0, "user_parent");
	// Winter Orb's throws price at the thrown spell's own rank, or at rank 1 when the caster never learned it.
	int rank_at = GetUserVariable(0, "user_rankat");
	if(!rank_at && parent == SPL_WINTERORB + 1 && !GetSpellRank(pnum, spell, true))
		rank_at = 1;

	// Rolled before any pricing, so gear scales the roll.
	int dmg = RollSpellValue(pnum, spell, which, rank_at);
	int dtype = GetSpellDamageType(spell);

	if(dmg <= 0) {
		SetResultValue(0);
		Terminate;
	}

	// A tick field prices through the DoT pile, as Blaze does; see IsSpellDoTField. A minion's cast takes minion damage.
	dmg = PriceSpellDamage(pnum, spell, dmg, flags, IsSpellDoTField(spell, which), -1, false, GetUserVariable(0, "user_minion"));

	// Winter Orb's ranks strengthen what it throws, read live like the rest. MORE, beside the others.
	if(parent == SPL_WINTERORB + 1)
		dmg = dmg * (100 + GetSpellValue(pnum, SPL_WINTERORB, SPELLVAL_AMOUNT)) / 100;

	// A copy that hits for a multiple of the spell, as Venom Gyre's dash catch; 0 on every class without the var.
	int mult = GetUserVariable(0, "user_dmgmult");
	if(mult > 1)
		dmg *= mult;

	// A percent of the spell instead, as Threaded Volley's energized rock over overlapping areas; 0 when absent too.
	int pct = GetUserVariable(0, "user_dmgpct");
	if(pct > 0)
		dmg = dmg * pct / 100;

	// A gem chain's last hop: the parked projectile's own blast loses what the chain lost.
	pct = GetUserVariable(0, "user_chainpct");
	if(pct > 0)
		dmg = dmg * pct / 100;

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
	bool rip = SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_HIGH);

	// One to each side. Angle is set after the spawn rather than through SpawnForced's own argument,
	// which takes byte angles and would wrap a 16.16 one into nonsense.
	for(i = 0; i < 2; ++i) {
		if(!SpawnForced("Spell_FireJetSide", x, y, z, tid, 0))
			continue;

		SetActorAngle(tid, a + (i ? -0.25 : 0.25));
		SetupSpellActor(tid, pnum, spell);
		SetUserVariable(tid, "user_minion", GetUserVariable(0, "user_minion"));
		// Rank 10's ripping flames: through everything, each enemy once per flame.
		if(rip) {
			SetSpellRipLimit(pnum, tid, MAX_RIPCOUNT);
			SetActorProperty(tid, APROP_STAMINA, GetActorProperty(tid, APROP_STAMINA) | DND_DAMAGEFLAG_RIPSONCE);
		}
		Thing_ChangeTID(tid, 0);
	}

	SetResultValue(0);
}

// A side flame's touch: only the rank 10 ripper deals damage by touch, below that its landing burst does.
Script "DnD Fire Jet Side Hit" (void) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1 || !SpellThresholdMet(pnum, SPL_FIREJET, DND_SPELL_THRESH_HIGH)) {
		SetResultValue(0);
		Terminate;
	}
	SetResultValue(ACS_NamedExecuteWithResult("DnD Spell Damage", SPELLVAL_DAMAGE2));
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
// A blast's full damage core, as a percent of its radius. Only Pyroblast's and Rockslide's AMOUNT rows are one.
int GetSpellBlastCore(int pnum, int spell, int rank_at) {
	if(spell == SPL_PYROBLAST || spell == SPL_ROCKSLIDE)
		return GetSpellValue(pnum, spell, SPELLVAL_AMOUNT, rank_at);
	return 0;
}

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

	// Fire Jet's bolt only bursts at all from rank 5; its side flames (DAMAGE2) always do. The caller branches on this.
	if(spell == SPL_FIREJET && which == SPELLVAL_DAMAGE && !SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_LOW)) {
		if(!had)
			Thing_ChangeTID(me, 0);
		SetResultValue(0);
		Terminate;
	}

	int radius = ACS_NamedExecuteWithResult("DnD Spell Radius", SPELLVAL_RADIUS);
	if(radius_pct > 0)
		radius = radius * radius_pct / 100;

	// "Radius increases to 256" is Frost Bomb's rank 10: it REPLACES the row and still takes area gear. Whole units,
	// as A_Explode reads them; a 16.16 radius here was 11.8 million units across.
	if(spell == SPL_FROSTBOMB && SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_HIGH))
		radius = ScaleSpellArea(pnum, spell, DND_FROSTBOMB_R10_RADIUS << 16) >> 16;

	// Blizzard's rank 5 grows each icicle's blast in step with the storm, so the bigger storm does not thin out.
	if(spell == SPL_BLIZZARD && SpellThresholdMet(pnum, spell, DND_SPELL_THRESH_LOW)) {
		int storm = Max(1, GetSpellValue(pnum, spell, SPELLVAL_AMOUNT));
		radius = radius / storm * (storm + DND_BLIZZARD_R5_RADIUS);
	}

	// Full strength inside the core, taken off the scaled radius so it tracks area gear; 0 is falloff all the way in.
	int full = GetSpellBlastCore(pnum, spell, GetUserVariable(0, "user_rankat"));
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

// A ripping spell's rip budget with the ripper gear a weapon's takes: increased ripcount, "rippers can penetrate all
// targets", "rippers only rip a target once". MAX_RIPCOUNT stays unlimited; tid 0 is the activator.
void SetSpellRipLimit(int pnum, int tid, int rips) {
	if(PlayerModData[pnum].vals[PSTAT_EX_RIPPERSRIPALL])
		rips = MAX_RIPCOUNT;
	else if(rips < MAX_RIPCOUNT)
		rips = Max(1, rips * (100 + PlayerModData[pnum].vals[PSTAT_RIPCOUNT]) / 100);

	if(PlayerModData[pnum].vals[PSTAT_EX_RIPPERSONETIMEONLY])
		SetActorProperty(tid, APROP_STAMINA, GetActorProperty(tid, APROP_STAMINA) | DND_DAMAGEFLAG_RIPSONCE);
	// The actor-inventory calls give tid 0 to the players, not the activator.
	if(tid)
		SetActorInventory(tid, "DnD_RipLimit", rips);
	else
		SetInventory("DnD_RipLimit", rips);
}

// A spell's area, as a 16.16 radius: spell area gear joins the other area increases in one additive pool, as an
// "increased" must, and scales AREA, not radius. extra_inc is a rank's own increase; spell is for the gem term.
int ScaleSpellArea(int pnum, int spell, int r, int extra_inc = 0) {
	r = ScalePlayerAoERadius(pnum, r, DND_AOESRC_NONWEAPON, PlayerModData[pnum].vals[PSTAT_SPELL_AOE] + extra_inc);
	return ApplyBlackEmpowerArea(pnum, spell, ApplySpellGemArea(pnum, spell, r));
}

// Companion to the above for explosive spells: the explosion retriever with spell area gear in its pool.
Script "DnD Spell Radius" (int which) {
	int pnum = GetSpellActorOwner();
	if(pnum == -1) {
		SetResultValue(0);
		Terminate;
	}

	int spell = GetUserVariable(0, "user_spellid");
	int r = GetSpellValue(pnum, spell, which, GetUserVariable(0, "user_rankat"));
	r = ACS_NamedExecuteWithResult("DnD Explosion Radius Retrieve", r, 1, DND_AOESRC_NONWEAPON,
		PlayerModData[pnum].vals[PSTAT_SPELL_AOE]);
	SetResultValue(ApplySpellGemArea(pnum, spell, r));
}

#endif
