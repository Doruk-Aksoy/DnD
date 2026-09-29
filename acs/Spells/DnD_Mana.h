#ifndef DND_MANA_IN
#define DND_MANA_IN

#include "DnD_SpellDefs.h"

// Mana is an Ammo item rather than a module value: the HUD that draws the bar is clientside, and
// inventory is what crosses. Both CAPS cross the same way, as amounts on P_ManaCap and ManaVisual --
// see the note in Sync.dec for why neither is a SetAmmoCapacity.
#define DND_MANA_BASE 10
#define DND_MANA_PER_INT 2

// Per level, the same shape as DND_HP_PER_LVL: counted from level 1, so the first level grants
// nothing and the base above is what a new character actually has.
#define DND_MANA_PER_LVL 2

// Regen is stored and spent in HUNDREDTHS so the 0.5/s base and the 0.25 per 2 INT step survive
// integer arithmetic. Only the accumulator below is in these units; everything else is whole mana.
#define DND_MANAREGEN_SCALE 100
#define DND_MANAREGEN_BASE 50
#define DND_MANAREGEN_PER_2INT 25

int GetPlayerManaCap(int pnum) {
	// Inside the base, so PSTAT_MANA_PCT scales the levelled part too -- GetRawSpawnHealth puts its
	// per level term in the same place for the same reason.
	int base = DND_MANA_BASE + GetIntellectEffect(pnum, DND_MANA_PER_INT) +
		DND_MANA_PER_LVL * (CheckActorInventory(pnum + P_TIDSTART, "Level") - 1) +
		PlayerModData[pnum].vals[PSTAT_MANA_FLAT];

	return Max(1, base * (100 + PlayerModData[pnum].vals[PSTAT_MANA_PCT]) / 100);
}

// Hundredths of mana per second.
int GetPlayerManaRegen(int pnum) {
	int base = DND_MANAREGEN_BASE + GetIntellectEffect(pnum, DND_MANAREGEN_PER_2INT, 2) +
		PlayerModData[pnum].vals[PSTAT_MANAREGEN_FLAT] * DND_MANAREGEN_SCALE;

	// Warmth's flat half is in whole mana per second, so it scales into the hundredths this works in.
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

// Both figures the clientside bar needs, pushed together so they can never disagree.
void UpdateManaVisualCap(int pnum) {
	int tid = pnum + P_TIDSTART;
	SetActorInventory(tid, "P_ManaCap", GetPlayerManaCap(pnum));
	SetActorInventory(tid, "ManaVisual", GetUnreservedManaCap(pnum));
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
