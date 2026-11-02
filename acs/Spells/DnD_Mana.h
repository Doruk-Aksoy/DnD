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
	// Blood Magic: no mana at all, so nothing that fills or scales the pool has anything to work with.
	if(HasPlayerFlag(pnum, PFLAG_BLOODMAGIC))
		return 0;

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
	if(HasPlayerFlag(pnum, PFLAG_BLOODMAGIC))
		return 0;

	int base = DND_MANAREGEN_BASE + GetIntellectEffect(pnum, DND_MANAREGEN_PER_2INT, 2) +
		PlayerModData[pnum].vals[PSTAT_MANAREGEN_FLAT];

	base += pbuffs[pnum].buff_net_values[BUFF_MANAREGENFLAT].additive;

	return Max(0, base * (100 + PlayerModData[pnum].vals[PSTAT_MANAREGEN_PCT] +
		pbuffs[pnum].buff_net_values[BUFF_MANAREGEN].additive) / 100);
}

// R = Base% / (100% + IncEfficiency%), per the design formula. A gem can make any spell reserve, and raise a reservation.
int GetManaReservationOf(int pnum, int spell) {
	int base = GetSpellGemMod(pnum, spell, GEMSTAT_RESERVE_ADD);
	if(SpellDefs[spell].flags & SPLF_RESERVES)
		base += GetSpellValue(pnum, spell, SPELLVAL_COST) >> 16;
	base = ApplySpellGemMore(pnum, spell, GEMSTAT_MORE_RESERVE, base);
	return base * 100 / (100 + PlayerModData[pnum].vals[PSTAT_MANARESERVE_EFF] + GetSpellGemMod(pnum, spell, GEMSTAT_RESERVE_EFF));
}

// The spells that CAN reserve, in id order: SPLF_RESERVES, or a gem's GEMSTAT_RESERVE_ADD. Rank, toggle and cost stay live.
typedef struct {
	bool valid;
	int count;
	int ids[MAX_SPELL_IDS];
} mana_reserve_list_T;

mana_reserve_list_T module& GetManaReserveList(int pnum) {
	static mana_reserve_list_T lists[MAXPLAYERS];
	return lists[pnum];
}

// Called by every writer of a gem row's GEMSTAT_RESERVE_ADD, and by InitSpellDefs.
void InvalidateManaReserveList(int pnum) {
	GetManaReserveList(pnum).valid = false;
}

void InvalidateAllManaReserveLists() {
	for(int i = 0; i < MAXPLAYERS; ++i)
		GetManaReserveList(i).valid = false;
}

mana_reserve_list_T module& GetBuiltManaReserveList(int pnum) {
	mana_reserve_list_T module& l = GetManaReserveList(pnum);
	if(!l.valid) {
		l.count = 0;
		for(int i = 0; i < MAX_SPELL_IDS; ++i) {
			if((SpellDefs[i].flags & SPLF_RESERVES) || GetSpellGemMod(pnum, i, GEMSTAT_RESERVE_ADD)) {
				l.ids[l.count] = i;
				++l.count;
			}
		}
		l.valid = true;
	}
	return l;
}

// Auras hold a percentage of the cap rather than a flat amount, so this is recomputed rather than
// tracked -- a mana roll on a new charm must not leave a stale reservation behind.
int GetReservedPercent(int pnum) {
	mana_reserve_list_T module& l = GetBuiltManaReserveList(pnum);
	int i, s, pct = 0;
	for(i = 0; i < l.count; ++i) {
		s = l.ids[i];
		if(IsSpellActive(pnum, s))
			pct += GetManaReservationOf(pnum, s);
	}
	return Min(pct, 100);
}

// Under Blood Magic the auras hold life instead -- DnD_LifeReserved, read by GetSpawnHealth -- and there is no mana to hold.
int GetReservedMana(int pnum) {
	if(HasPlayerFlag(pnum, PFLAG_BLOODMAGIC))
		return 0;
	return GetPlayerManaCap(pnum) * GetReservedPercent(pnum) / 100;
}

int GetUnreservedManaCap(int pnum) {
	return Max(0, GetPlayerManaCap(pnum) - GetReservedMana(pnum));
}

// Blood Magic: a hold of pct percent of this life cap fits only while at least 1 life stays unheld.
bool LifeHoldFits(int cap, int pct) {
	return cap * (100 - pct) >= 100;
}

// Switches off, in id order, every active aura whose reservation no longer fits.
void FitAuraReservations(int pnum) {
	mana_reserve_list_T module& l = GetBuiltManaReserveList(pnum);
	bool blood = HasPlayerFlag(pnum, PFLAG_BLOODMAGIC);
	int cap = blood ? GetSpawnHealthBeforeReserve(false, pnum) : 0;
	int i, k, pct = 0, r;
	for(k = 0; k < l.count; ++k) {
		i = l.ids[k];
		if(!IsSpellActive(pnum, i))
			continue;

		r = GetManaReservationOf(pnum, i);
		if(pct + r > 100 || (blood && !LifeHoldFits(cap, pct + r))) {
			SetSpellToggledOff(pnum, i, true);
			SyncSpellToggleWord(pnum, i);
		}
		else
			pct += r;
	}
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
	int cap = GetUnreservedManaCap(pnum);

	SetActorInventory(tid, "P_ManaCap", GetPlayerManaCap(pnum));
	SetActorInventory(tid, "ManaVisual", cap);
	SetActorInventory(tid, "P_ManaRegen", GetPlayerManaRegen(pnum));

	// A SHRINKING cap has to take the surplus with it. GiveMana clamps on the way in, so nothing
	// could ever push a player over their cap -- but unequipping a +mana charm lowers the cap
	// under mana already held, and that surplus used to sit there indefinitely. CanAffordSpell
	// reads the held amount, so a spell the player could no longer pay for stayed castable.
	//
	// Here rather than at the equip sites: this is what every cap change already calls, and the
	// once-a-second regeneration tick calls it too, so gear applied by any route is caught.
	if(CheckActorInventory(tid, "Mana") > cap)
		SetActorInventory(tid, "Mana", cap);

	// Blood Magic: the auras' hold on life, kept on a marker so GetSpawnHealth does not sum every aura per call. A
	// changed hold moves the health cap, so it is pushed at once and the player's health clamped under it.
	int life = 0;
	if(HasPlayerFlag(pnum, PFLAG_BLOODMAGIC)) {
		life = GetReservedPercent(pnum);
		// a life cap that fell under the hold (1 max life holds nothing) drops the auras that no longer fit
		if(life && !LifeHoldFits(GetSpawnHealthBeforeReserve(false, pnum), life)) {
			FitAuraReservations(pnum);
			life = GetReservedPercent(pnum);
		}
	}
	if(CheckActorInventory(tid, "DnD_LifeReserved") != life) {
		SetActorInventory(tid, "DnD_LifeReserved", life);
		cap = GetSpawnHealth(false, pnum);
		SetActorProperty(tid, APROP_SPAWNHEALTH, cap);
		if(GetActorProperty(tid, APROP_HEALTH) > cap)
			SetActorProperty(tid, APROP_HEALTH, cap);
	}
}

// the mana side of IsLowLife: half the unreserved pool or less
bool IsLowMana(int pnum) {
	return GetPlayerMana(pnum) <= GetUnreservedManaCap(pnum) / 2;
}

// Clamped on the way in so a shrinking cap -- a swapped charm, a newly enabled aura -- cannot leave
// the player holding more than they can hold.
void GiveMana(int pnum, int amt) {
	if(amt <= 0)
		return;
	SetPlayerMana(pnum, Min(GetPlayerMana(pnum) + amt, GetUnreservedManaCap(pnum)));
}

// Mana leech pays out as life leech does: a pool held to a share of the unreserved mana, paid a share of it a pulse.
#define DND_BASE_MANALEECHCAP 33		// percent of the unreserved mana the pool holds (life leech: 33 of the heal cap)
#define DND_BASE_MANALEECHRATE 25		// tics between pulses (life leech's base rate)
#define DND_BASE_MANALEECHRECOVERY 1	// percent of the unreserved mana a pulse pays (life leech: 1 of the heal cap)

int GetManaLeechCap(int pnum) {
	return GetUnreservedManaCap(pnum) * DND_BASE_MANALEECHCAP / 100;
}

int GetManaLeechRecovery(int pnum) {
	return Max(1, GetUnreservedManaCap(pnum) * DND_BASE_MANALEECHRECOVERY / 100);
}

// Tics between pulses; the instant leech mod's reduced leech recovery rate slows it as it slows life leech.
int GetManaLeechRate(int pnum) {
	return Max(1, DND_BASE_MANALEECHRATE * (100 + ReadPlayerModExtra(pnum, INV_INC_INSTANTLIFESTEAL)) / 100);
}

// The share of any leech, life or mana, paid at once by the instant leech mods; at most all of it.
int GetInstantLeechPercent(int pnum) {
	return Min(100, PlayerModData[pnum].vals[PSTAT_CORR_INSTALEECHPCT] + PlayerModData[pnum].vals[PSTAT_INC_INSTANTLIFESTEAL]);
}

// "More damage while leeching": held while either leech pays out, so the one ending first leaves it to the other.
void GrantLeechingDamage(int pnum) {
	int v = PlayerModData[pnum].vals[PSTAT_LIFESTEAL_DAMAGE];
	if(v && !HasPlayerBuff(pnum, BTI_LEECHINGDAMAGE))
		HandlePlayerBuffAssignment(pnum, 0, BTI_LEECHINGDAMAGE, 0, 0, 0, v);
}

void EndLeechingDamage(int pnum) {
	int ptid = pnum + P_TIDSTART;
	if(!CheckActorInventory(ptid, "LifestealScriptRunning") && !CheckActorInventory(ptid, "ManaLeechScriptRunning"))
		RemoveBuffWithTableIndex(pnum, BTI_LEECHINGDAMAGE);
}

// Leeched mana into the pool, up to its cap, less the instant share paid now; starts the payout if it is not running.
void ResolveManaLeech(int pnum, int amt) {
	// "Players cannot leech" covers mana too, blocked before the pool as life's is, so a dungeon cannot bank it either.
	if(DungeonBlocksLeech())
		return;

	int ptid = pnum + P_TIDSTART;
	amt = Min(amt, GetManaLeechCap(pnum) - CheckActorInventory(ptid, "ManaLeechAmount"));
	if(amt <= 0)
		return;

	int inst = amt * GetInstantLeechPercent(pnum) / 100;
	if(inst) {
		GiveMana(pnum, inst);
		amt -= inst;
		if(amt <= 0)
			return;
	}

	GiveActorInventory(ptid, "ManaLeechAmount", amt);
	if(!CheckActorInventory(ptid, "ManaLeechScriptRunning")) {
		GiveActorInventory(ptid, "ManaLeechScriptRunning", 1);
		ACS_NamedExecuteAlways("DnD Mana Leech", 0, ptid);
	}
}

// Pays the pool out a pulse at a time, the first at once; full mana or death ends the leech and empties it, as with life.
Script "DnD Mana Leech" (int tid) {
	SetActivator(tid);
	int pnum = PlayerNumber(), amt;
	GrantLeechingDamage(pnum);
	while(IsAlive() && (amt = CheckInventory("ManaLeechAmount"))) {
		amt = Min(amt, GetManaLeechRecovery(pnum));
		TakeInventory("ManaLeechAmount", amt);
		GiveMana(pnum, amt);
		if(GetPlayerMana(pnum) >= GetUnreservedManaCap(pnum))
			break;
		Delay(GetManaLeechRate(pnum));
	}
	SetInventory("ManaLeechAmount", 0);
	SetInventory("ManaLeechScriptRunning", 0);
	EndLeechingDamage(pnum);
}

#ifdef ISDEBUGBUILD
// Players whose spells cost nothing, set by "DnD Debug Free Spells". Map scope, so a new map clears it.
int[] module& GetDebugFreeSpells() {
	static int on[MAXPLAYERS];
	return on;
}
#endif

// Debug builds only: this player's spells cost nothing to cast, mana or life. Always false in a release build.
bool HasDebugFreeSpells(int pnum) {
#ifdef ISDEBUGBUILD
	int[] module& on = GetDebugFreeSpells();
	return pnum >= 0 && pnum < MAXPLAYERS && on[pnum];
#else
	return false;
#endif
}

// Temporal Surge returns this share of a paid cost as life, in even pulses over this long.
#define DND_TEMPORALSURGE_PCT 40
#define DND_TEMPORALSURGE_TICS (4 * TICRATE)
#define DND_TEMPORALSURGE_PULSES 8

// Temporal Surge: a paid cost may come back, part of it, as life over a few seconds.
void RollCostRecovery(int pnum, int paid) {
	int chance = PlayerModData[pnum].vals[PSTAT_COSTRECOVER_CHANCE];
	if(chance && paid > 0 && random(1, 100) <= chance)
		ACS_NamedExecuteAlways("DnD Temporal Surge", 0, pnum, paid * DND_TEMPORALSURGE_PCT / 100);
}

// Percent of a spell's cost paid in life, 0 for mana: all of it under Blood Magic, which leaves Haemocrafter nothing to convert.
int GetSpellLifeCostRate(int pnum, int spell) {
	if(HasPlayerFlag(pnum, PFLAG_BLOODMAGIC))
		return 100;
	return GetSpellGemMod(pnum, spell, GEMSTAT_LIFECOST);
}

// One spell cost, paid by every rule at once: Blood Magic and Haemocrafter in life, Tap into the Arcane's share in
// energy shield, the rest in mana. dry only asks, so the afford check and the spend can never disagree.
bool PaySpellCost(int pnum, int spell, int cost, bool dry = false) {
	int ptid = pnum + P_TIDSTART;

	// Life is paid through DegenPlayer, the never lethal upkeep path Boiling Blood uses, and never with the last point.
	int life = GetSpellLifeCostRate(pnum, spell);
	if(life) {
		life = cost * life / 100;
		if(GetActorProperty(ptid, APROP_HEALTH) <= life)
			return false;
		if(!dry) {
			DegenPlayer(pnum, life);
			RollCostRecovery(pnum, life);
		}
		return true;
	}

	// Tap into the Arcane: its share from energy shield, as much as the shield holds. Under Sanguine Covenant the shield
	// is the pool that kills, so its last point stays.
	int shield = 0, pct = PlayerModData[pnum].vals[PSTAT_MANACOST_TOSHIELD];
	if(pct) {
		shield = CheckActorInventory(ptid, "EShieldAmount");
		if(HasPlayerFlag(pnum, PFLAG_SWAP_HP_SHIELD))
			--shield;
		shield = Clamp_Between(cost * pct / 100, 0, Max(0, shield));
	}

	if(GetPlayerMana(pnum) < cost - shield)
		return false;
	if(!dry) {
		if(shield)
			TakeActorEnergyShield(ptid, shield);
		SetPlayerMana(pnum, GetPlayerMana(pnum) - (cost - shield));
		RollCostRecovery(pnum, cost);
	}
	return true;
}

// A cost handed back in what paid it (Magus): life at the rate it was taken, up to the cap, or mana.
void RefundSpellCost(int pnum, int spell, int amt) {
	int life = GetSpellLifeCostRate(pnum, spell);
	if(!life) {
		GiveMana(pnum, amt);
		return;
	}
	amt = Min(amt * life / 100, GetHealPoolCap(pnum) - GetHealPoolCur(pnum));
	if(amt > 0)
		GiveHealPool(pnum, amt);
}

// pct scales the cost as SpendSpellMana's does, so a press that will pay less is judged on what it pays.
bool CanAffordSpell(int pnum, int spell, int pct = 100) {
	if((SpellDefs[spell].flags & (SPLF_PASSIVE | SPLF_RESERVES)) || HasDebugFreeSpells(pnum))
		return true;
	return PaySpellCost(pnum, spell, (GetSpellValue(pnum, spell, SPELLVAL_COST) >> 16) * pct / 100, true);
}

// Returns false without spending when the player cannot pay, so the cast site can simply bail.
// pct scales the row cost for callers whose instances are not all worth the same -- Annihilus bills
// a rising charge on a rising price. It defaults to the plain row cost, so nothing else changes.
bool SpendSpellMana(int pnum, int spell, int pct = 100) {
	if((SpellDefs[spell].flags & (SPLF_PASSIVE | SPLF_RESERVES)) || HasDebugFreeSpells(pnum))
		return true;
	return PaySpellCost(pnum, spell, (GetSpellValue(pnum, spell, SPELLVAL_COST) >> 16) * pct / 100);
}

// Temporal Surge's heal: the total in even pulses over a few seconds, summing to it exactly.
Script "DnD Temporal Surge" (int pnum, int total) {
	int amt;
	for(int k = 1; k <= DND_TEMPORALSURGE_PULSES; ++k) {
		Delay(const:DND_TEMPORALSURGE_TICS / DND_TEMPORALSURGE_PULSES);
		if(!PlayerInGame(pnum) || !IsActorAlive(pnum + P_TIDSTART))
			Terminate;
		amt = total * k / DND_TEMPORALSURGE_PULSES - total * (k - 1) / DND_TEMPORALSURGE_PULSES;
		if(amt > 0)
			HealSpellAlly(pnum, amt);
	}
}

// An aura whose reservation no longer fits switches itself off. Called when the cap or the set of
// enabled auras changes, so the two can never disagree.
void ValidateAuraReservations(int pnum) {
	FitAuraReservations(pnum);

	// The clamp lives inside this now, so switching an aura on still cannot leave the player
	// holding more than the reservation left them.
	UpdateManaVisualCap(pnum);
}

#endif
