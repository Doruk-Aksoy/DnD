#ifndef DND_MANA_IN
#define DND_MANA_IN

#include "DnD_SpellDefs.h"

// Mana is an Ammo item rather than a module value: the HUD that draws the bar is clientside, and
// inventory is what crosses. Both CAPS cross the same way, as amounts on P_ManaCap and ManaVisual --
// see the note in Sync.dec for why neither is a SetAmmoCapacity.
#define DND_MANA_BASE 10

// INT's share of both pools, deliberately SMALLER than a dedicated affix's. It used to be 2 mana and
// 0.125 regen per point, which made a top tier INT roll worth 82 mana AND 5.12 regen a second -- more
// regen than a top tier regen affix and nearly as much mana as a top tier mana affix, from one mod
// that also carries every other INT benefit. There was no reason to take the dedicated rolls at all.
//
// At 1.5 mana and 0.025 regen per point a top tier INT roll gives 61 mana and 1.02/s, so the
// dedicated affixes are worth about 1.6x and 2x their INT equivalent. INT stays the generalist pick
// and the specific rolls are what a spell build actually wants.
#define DND_MANA_PER_INT 3
#define DND_MANA_INT_DIV 2

// Per level, the same shape as DND_HP_PER_LVL: counted from level 1, so the first level grants
// nothing and the base above is what a new character actually has.
#define DND_MANA_PER_LVL 2

// Regen is stored and spent in HUNDREDTHS so the 0.5/s base and the 0.25 per 2 INT step survive
// integer arithmetic. Everything that contributes a FLAT amount of regen is in those units too --
// DND_MANAREGEN_BASE, PSTAT_MANAREGEN_FLAT and BUFF_MANAREGENFLAT are all hundredths, so 50 means
// half a mana a second wherever it appears. Whole mana is what the POOL is in, not the regen.
#define DND_MANAREGEN_SCALE 100
#define DND_MANAREGEN_BASE 50
#define DND_MANAREGEN_PER_2INT 5

int GetPlayerManaCap(int pnum) {
	// Inside the base, so PSTAT_MANA_PCT scales the levelled part too -- GetRawSpawnHealth puts its
	// per level term in the same place for the same reason.
	int base = DND_MANA_BASE + GetIntellectEffect(pnum, DND_MANA_PER_INT, DND_MANA_INT_DIV) +
		DND_MANA_PER_LVL * (CheckActorInventory(pnum + P_TIDSTART, "Level") - 1) +
		PlayerModData[pnum].vals[PSTAT_MANA_FLAT];

	return Max(1, base * (100 + PlayerModData[pnum].vals[PSTAT_MANA_PCT]) / 100);
}

// Hundredths of mana per second, and so is EVERY flat term feeding it -- the stat, the buff and the
// base alike. PSTAT_MANAREGEN_FLAT used to be whole mana per second and got scaled up on the way in,
// which meant the smallest roll an item could carry was +1.00/s: double the entire base regen, and
// nothing between that and nothing. It also disagreed with the buff lane beside it, which was already
// in hundredths, so the same quantity meant different things depending on where it came from.
int GetPlayerManaRegen(int pnum) {
	int base = DND_MANAREGEN_BASE + GetIntellectEffect(pnum, DND_MANAREGEN_PER_2INT, 2) +
		PlayerModData[pnum].vals[PSTAT_MANAREGEN_FLAT];

	base += pbuffs[pnum].buff_net_values[BUFF_MANAREGENFLAT].additive;

	return Max(0, base * (100 + PlayerModData[pnum].vals[PSTAT_MANAREGEN_PCT] +
		pbuffs[pnum].buff_net_values[BUFF_MANAREGEN].additive) / 100);
}

// R = Base% / (100% + IncEfficiency%), per the design formula.
int GetManaReservationOf(int pnum, int spell) {
	int base = GetSpellValue(pnum, spell, SPELLVAL_COST) >> 16;
	return base * 100 / (100 + PlayerModData[pnum].vals[PSTAT_MANARESERVE_EFF]);
}

// Auras hold a percentage of the cap rather than a flat amount, so this is recomputed rather than
// tracked -- a mana roll on a new charm must not leave a stale reservation behind.
int GetReservedMana(int pnum) {
	int i, pct = 0;
	for(i = 0; i < MAX_SPELL_IDS; ++i) {
		if(!(SpellDefs[i].flags & SPLF_RESERVES) || !IsSpellActive(pnum, i))
			continue;
		pct += GetManaReservationOf(pnum, i);
	}

	if(pct > 100)
		pct = 100;

	return GetPlayerManaCap(pnum) * pct / 100;
}

int GetUnreservedManaCap(int pnum) {
	return Max(0, GetPlayerManaCap(pnum) - GetReservedMana(pnum));
}

int GetPlayerMana(int pnum) {
	return CheckActorInventory(pnum + P_TIDSTART, "Mana");
}

void SetPlayerMana(int pnum, int val) {
	SetActorInventory(pnum + P_TIDSTART, "Mana", Max(0, val));
}

// The figures the clientside bar and the stats page need, pushed together so they can never
// disagree. Regen rides along because every term feeding it -- INT and the level, via a player
// TID -- is server only, so the client cannot recompute it.
void UpdateManaVisualCap(int pnum) {
	int tid = pnum + P_TIDSTART;
	SetActorInventory(tid, "P_ManaCap", GetPlayerManaCap(pnum));
	SetActorInventory(tid, "ManaVisual", GetUnreservedManaCap(pnum));
	SetActorInventory(tid, "P_ManaRegen", GetPlayerManaRegen(pnum));
}

// Clamped on the way in so a shrinking cap -- a swapped charm, a newly enabled aura -- cannot leave
// the player holding more than they can hold.
void GiveMana(int pnum, int amt) {
	if(amt <= 0)
		return;
	SetPlayerMana(pnum, Min(GetPlayerMana(pnum) + amt, GetUnreservedManaCap(pnum)));
}

bool CanAffordSpell(int pnum, int spell) {
	if(SpellDefs[spell].flags & (SPLF_PASSIVE | SPLF_RESERVES))
		return true;
	return GetPlayerMana(pnum) >= (GetSpellValue(pnum, spell, SPELLVAL_COST) >> 16);
}

// Returns false without spending when the player cannot pay, so the cast site can simply bail.
bool SpendSpellMana(int pnum, int spell) {
	if(SpellDefs[spell].flags & (SPLF_PASSIVE | SPLF_RESERVES))
		return true;

	int cost = GetSpellValue(pnum, spell, SPELLVAL_COST) >> 16;
	if(GetPlayerMana(pnum) < cost)
		return false;

	SetPlayerMana(pnum, GetPlayerMana(pnum) - cost);
	return true;
}

// An aura whose reservation no longer fits switches itself off. Called when the cap or the set of
// enabled auras changes, so the two can never disagree.
void ValidateAuraReservations(int pnum) {
	int i, pct = 0, r;
	for(i = 0; i < MAX_SPELL_IDS; ++i) {
		if(!(SpellDefs[i].flags & SPLF_RESERVES) || !IsSpellActive(pnum, i))
			continue;

		r = GetManaReservationOf(pnum, i);
		if(pct + r > 100) {
			SetSpellToggledOff(pnum, i, true);
			SyncSpellToggleWord(pnum, i);
		}
		else
			pct += r;
	}

	UpdateManaVisualCap(pnum);
	if(GetPlayerMana(pnum) > GetUnreservedManaCap(pnum))
		SetPlayerMana(pnum, GetUnreservedManaCap(pnum));
}

#endif
