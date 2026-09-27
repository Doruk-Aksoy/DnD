#ifndef DND_SPELLDEFS_IN
#define DND_SPELLDEFS_IN

#include "../DnD_SkillDef.h"

// Spell ids are a flat, densely packed space with the tree stored on the def rather than derived
// from the id. Ranks are saved by id, so this enum is APPEND ONLY -- inserting anywhere shifts every
// spell after it and silently rewrites saved characters.
enum {
	// ---- Fire ----
	SPL_BLAZE,
	SPL_FIREBALL,
	SPL_INFERNALSTRIKE,
	SPL_WARMTH,
	SPL_HEATSHIELD,
	SPL_INCINERATE,
	SPL_BOILINGBLOOD,
	SPL_ANGER,
	SPL_PYROBLAST,
	SPL_SEARINGBOND,
	SPL_SCORCHINGRAY,
	SPL_FLAMMABILITY,
	SPL_IMMOLATION,
	SPL_RAINOFFIRE,
	SPL_FLAMEPILLAR,
	SPL_FIREJET,
	SPL_FIREDEMON,
	SPL_MOLTENBOULDER_S,
	SPL_RIGHTEOUSFIRE,
	SPL_ANNIHILUS,
	SPL_VOLCANO,

	// ---- Cold ----
	SPL_ICEBOLT,
	SPL_FREEZINGPULSE,
	SPL_GLACIALSPIKE,
	SPL_CHILLINGBREATH,
	SPL_CREEPINGFROST,
	SPL_ICESHIELD_S,
	SPL_FROSTSHARDS,
	SPL_GUSTOFFROST,
	SPL_FROSTBOMB,
	SPL_ICENOVA,
	SPL_ICEGOLEM,
	SPL_HATRED,
	SPL_FROSTBITE,
	SPL_BLIZZARD,
	SPL_WINTERORB,
	SPL_ICESPEAR,
	SPL_GLACIALCASCADE,
	SPL_SHIVERINGARMOR,
	SPL_AVALANCHE
};
#define MAX_SPELL_IDS (SPL_AVALANCHE + 1)

// Which numbers a spell carries. DAMAGE, DAMAGE2, RADIUS and AMOUNT are plain integers; COST,
// COOLDOWN, CASTTIME, DURATION and CDR are 16.16 -- seconds for the times, mana for the cost,
// percent for the rate. Times convert with (v * TICRATE) >> 16 at the point of use.
enum {
	SPELLVAL_DAMAGE,
	SPELLVAL_DAMAGE2,
	SPELLVAL_COST,
	SPELLVAL_COOLDOWN,
	SPELLVAL_CASTTIME,
	SPELLVAL_DURATION,
	SPELLVAL_RADIUS,
	SPELLVAL_AMOUNT,
	SPELLVAL_CDR,
	SPELLVAL_MAX
};

enum {
	SPLF_PASSIVE		= 1,		// works from allocation alone, never goes on the hotbar
	SPLF_AURA			= 2,		// toggled in the tree, holds a mana reservation
	SPLF_CURSE			= 4,
	SPLF_CHANNELED		= 8,		// weapon must be idle
	SPLF_SUMMON			= 16,
	SPLF_PROJECTILE		= 32,
	SPLF_TARGETED		= 64,		// wants a target point rather than a facing
	SPLF_RESERVES		= 128,		// COST is a reservation percent, not a spend
	SPLF_REQ_ANY		= 256		// the requirement list is OR rather than AND
};

#define DND_SPELL_RANKCAP 10
#define DND_SPELL_THRESH_LOW 5
#define DND_SPELL_THRESH_HIGH 10

typedef struct {
	int base[SPELLVAL_MAX];
	int per_rank[SPELLVAL_MAX];
	int req_spell[DND_MAX_SKILL_REQ];	// stored +1 so an unset slot reads 0
	int req_rank[DND_MAX_SKILL_REQ];
	int req_level;
	int req_tree_ranks;
	int tree;
	int flags;
	int tx;								// authored position in tree space
	int ty;
} spell_def_T;

global spell_def_T 46: SpellDefs[MAX_SPELL_IDS];

// ---- synergies -------------------------------------------------------------------------------
// "5% more damage per Blaze rank" is one row, not a line inside Blaze's cast code. per_rank is
// 16.16 percent for the scaling kinds and a plain integer for SYNF_FLAT.
enum {
	SYNF_MORE		= 1,	// multiplicative, per the increased/more rule
	SYNF_FLAT		= 2		// adds per_rank directly to the field
};

typedef struct {
	int target;
	int source;
	int field;
	int per_rank;
	int flags;
} spell_syn_T;

// target is stored +1, so the first zero row ends the list and no separate count is needed.
#define MAX_SPELL_SYNERGIES 96
global spell_syn_T 48: SpellSynergies[MAX_SPELL_SYNERGIES];

// ---- per player, persistent ------------------------------------------------------------------
// Ranks pack 8 per int at 4 bits each, which covers the 0-10 range with headroom for 128 spells
// before this needs resizing. Hotbar stores id + 1 so an untouched slot reads as empty.
#define SPELL_RANK_INTS 16
#define MAX_HOTBAR_SLOTS 6
#define DND_HOTBAR_BASESLOTS 3
#define HOTBAR_EMPTY 0
#define SPELL_AURA_INTS 4

typedef struct {
	int ranks[SPELL_RANK_INTS];
	int hotbar[MAX_HOTBAR_SLOTS];
	int aura_on[SPELL_AURA_INTS];
	int unspent;
} spell_player_T;

global spell_player_T 45: SpellPlayerData[MAXPLAYERS];

// ---- accessors -------------------------------------------------------------------------------

int GetSpellAllocatedRank(int pnum, int spell) {
	return (SpellPlayerData[pnum].ranks[spell >> 3] >> ((spell & 7) << 2)) & 0xF;
}

// Push the one word a rank lives in, the way SyncPerkWord does. SpellPlayerData is a GLOBAL, so
// each side holds its own copy: the server allocates and the clientside menu draw would keep reading
// zeros. Single player shares the globals, hence the early out.
void SyncSpellRankWord(int pnum, int spell) {
	ACS_NamedExecuteWithResult("DnD Request Spell Sync", pnum, spell >> 3,
		SpellPlayerData[pnum].ranks[spell >> 3]);
}

void SetSpellAllocatedRank(int pnum, int spell, int rank) {
	int w = spell >> 3, sh = (spell & 7) << 2;
	SpellPlayerData[pnum].ranks[w] = (SpellPlayerData[pnum].ranks[w] & ~(0xF << sh)) | ((rank & 0xF) << sh);
}

// Gear adds to a spell you already bought and never to one you did not. A future "Grants level X"
// mod would feed the ALLOCATED side instead, so it grants and scales in one step.
int GetSpellRank(int pnum, int spell, bool effective) {
	int alloc = GetSpellAllocatedRank(pnum, spell);
	if(!effective || !alloc)
		return alloc;

	return alloc + PlayerModData[pnum].vals[PSTAT_SPELLLEVEL_ALL] +
		PlayerModData[pnum].vals[PSTAT_SPELLLEVEL_FIRE + SpellDefs[spell].tree];
}

// Unlocked, and at the threshold once gear is counted.
bool SpellThresholdMet(int pnum, int spell, int at) {
	return GetSpellAllocatedRank(pnum, spell) && GetSpellRank(pnum, spell, true) >= at;
}

// Requirements read the ALLOCATED rank on both sides, so no amount of +levels opens a branch.
bool IsSpellUnlockable(int pnum, int spell) {
	if(GetActorLevel(pnum + P_TIDSTART) < SpellDefs[spell].req_level)
		return false;

	int i, req, listed = 0, met = 0;
	for(i = 0; i < DND_MAX_SKILL_REQ; ++i) {
		req = SpellDefs[spell].req_spell[i];
		if(!req)
			continue;
		++listed;
		if(GetSpellAllocatedRank(pnum, req - 1) >= SpellDefs[spell].req_rank[i])
			++met;
	}

	if(SpellDefs[spell].flags & SPLF_REQ_ANY) {
		if(listed && !met)
			return false;
	}
	else if(met != listed)
		return false;

	if(SpellDefs[spell].req_tree_ranks) {
		int spent = 0;
		for(i = 0; i < MAX_SPELL_IDS; ++i)
			if(SpellDefs[i].tree == SpellDefs[spell].tree)
				spent += GetSpellAllocatedRank(pnum, i);
		if(spent < SpellDefs[spell].req_tree_ranks)
			return false;
	}

	return true;
}

// The one place the rank curve lives: base plus a linear step per rank past the first, then the
// synergy rows that name this field. Nothing clamps, because the only shrinking values left in the
// trees are cooldown RECOVERY RATE, which is asymptotic through FinalCD = CD / (1 + rate/100).
// rank_at previews a rank the player does not have, which is the only way the tree can show a
// locked spell's numbers. Zero means "whatever they actually have".
int GetSpellValue(int pnum, int spell, int which, int rank_at = 0) {
	int rank = rank_at ? rank_at : GetSpellRank(pnum, spell, true);
	if(!rank)
		return 0;

	int res = SpellDefs[spell].base[which] + (rank - 1) * SpellDefs[spell].per_rank[which];

	int i, src, more = 0;
	for(i = 0; i < MAX_SPELL_SYNERGIES && SpellSynergies[i].target; ++i) {
		if(SpellSynergies[i].target - 1 != spell || SpellSynergies[i].field != which)
			continue;

		src = GetSpellRank(pnum, SpellSynergies[i].source, true);
		if(!src)
			continue;

		if(SpellSynergies[i].flags & SYNF_FLAT)
			res += SpellSynergies[i].per_rank * src;
		else
			more += (SpellSynergies[i].per_rank >> 16) * src;
	}

	if(more)
		res = res * (100 + more) / 100;

	// Gear applies last and is additive, so a synergy's "more" does not multiply it.
	if(which == SPELLVAL_RADIUS)
		res = res * Max(0, 100 + PlayerModData[pnum].vals[PSTAT_SPELL_AOE]) / 100;
	else if(which == SPELLVAL_DURATION)
		res = res * Max(0, 100 + PlayerModData[pnum].vals[PSTAT_SPELL_DURATION]) / 100;

	return res;
}

// Icons are positional: SPL<id> in colour, SPL<id>G greyscale for a locked tree node. The prefix
// is three characters because a graphic lump name may not exceed eight -- SPICO<id>G would have
// capped the whole system at id 99.
bool IsAuraEnabled(int pnum, int spell) {
	return !!(SpellPlayerData[pnum].aura_on[spell >> 5] & (1 << (spell & 31)));
}

void SetAuraEnabled(int pnum, int spell, bool on) {
	if(on)
		SpellPlayerData[pnum].aura_on[spell >> 5] |= 1 << (spell & 31);
	else
		SpellPlayerData[pnum].aura_on[spell >> 5] &= ~(1 << (spell & 31));
}

// -1 for an empty slot: the array holds id + 1 so an untouched global reads as empty.
int GetHotbarSpell(int pnum, int slot) {
	return SpellPlayerData[pnum].hotbar[slot] - 1;
}

void SetHotbarSpell(int pnum, int slot, int spell) {
	SpellPlayerData[pnum].hotbar[slot] = spell + 1;
}

// Three to start, Wanderer one more, items up to two -- six in total.
int GetHotbarSlotCount(int pnum) {
	int n = DND_HOTBAR_BASESLOTS + PlayerModData[pnum].vals[PSTAT_EX_SPELLSLOTS];
	if(isActorPlayerClass(pnum + P_TIDSTART, DND_PLAYER_WANDERER))
		++n;
	return Min(n, MAX_HOTBAR_SLOTS);
}

// Where a spell sits among its OWN tree, counted in enum order. Stable for everything already in
// the tree, because the enum is append only: a spell added later takes the next index in its tree
// and moves nothing.
int GetSpellTreeIndex(int spell) {
	int i, n = 0;
	for(i = 0; i < spell; ++i)
		if(SpellDefs[i].tree == SpellDefs[spell].tree)
			++n;
	return n;
}

// SP<tree><index>, zero padded to two -- SP000..SP020 for Fire, SP100.. for Cold, plus G for the
// greyscale a locked spell draws.
//
// Named by TREE rather than by spell id on purpose. Ids are append only because saved ranks are
// keyed by them, so a Fire spell added later would take id 40 and its icon would sort in the middle
// of Cold's files and stay there. Tree relative names keep a folder coherent whatever gets appended,
// and at 6 characters they are well inside the 8 character lump limit.
str GetSpellIcon(int spell, bool locked) {
	int idx = GetSpellTreeIndex(spell);
	return StrParam(s:"SP", d:SpellDefs[spell].tree, s:idx < 10 ? "0" : "", d:idx, s:locked ? "G" : "");
}

str GetSpellNameLump(int spell) {
	return StrParam(s:"DND_SPLNAME", d:spell);
}

// Hover text. The rank bonuses are underscored because DND_SPLR5_10 and DND_SPLR51_0 would
// otherwise be the same lump.
str GetSpellDescLump(int spell) {
	return StrParam(s:"DND_SPLDESC", d:spell);
}

str GetSpellPerRankLump(int spell) {
	return StrParam(s:"DND_SPLPER", d:spell);
}

str GetSpellThresholdLump(int spell, int at) {
	return StrParam(s:"DND_SPLR", d:at, s:"_", d:spell);
}

str GetSpellFieldLump(int which) {
	return StrParam(s:"DND_SPLFIELD", d:which);
}

#endif
