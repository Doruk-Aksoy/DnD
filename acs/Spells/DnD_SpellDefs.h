#ifndef DND_SPELLDEFS_IN
#define DND_SPELLDEFS_IN

#include "../DnD_SkillDef.h"

// Every tree owns a fixed BLOCK of ids -- tree N holds N*32 .. N*32+31 -- rather than the trees
// sharing one densely packed space. Most of those slots are empty, and that is the point.
//
// An id is a spell's permanent address. Its LANGUAGE lumps are keyed by it (DND_SPLNAME<id>), its
// icon is keyed by its offset inside the block (SP<tree><offset>), and a saved rank lives in the
// nibble it indexes. Packing the trees end to end meant adding one fire spell renumbered every ice
// spell after it -- every lump, every icon, every stored rank. With a block per tree, a new spell
// takes the next free slot in ITS OWN block and nothing else moves, ever.
//
// So: append inside the relevant block. The only thing that still shifts is inserting BEFORE an
// existing spell in the same block, which there is no reason to do -- the tree's on-screen layout
// comes from tx/ty on the def, not from id order.
#define DND_SPELLS_PER_TREE 32
#define MAX_SPELL_IDS (DND_SPELLS_PER_TREE * (DND_SKILLTREE_COMBAT + 1))

// First id of a tree's block. A tree's spells run from here upward.
int GetTreeFirstSpell(int tree) {
	return tree * DND_SPELLS_PER_TREE;
}

enum {
	// ---- Fire ----
	SPL_BLAZE = DND_SKILLTREE_FIRE * DND_SPELLS_PER_TREE,
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
	SPL_HEARTOFFIRE,

	// ---- Cold ----
	SPL_ICEBOLT = DND_SKILLTREE_ICE * DND_SPELLS_PER_TREE,
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
	SPLF_REQ_ANY		= 256,		// the requirement list is OR rather than AND
	SPLF_ALLYTARGET		= 512,		// soft locks onto an ally while the cast runs
	SPLF_SUPPORT		= 1024		// self or ally only; nothing hostile happens when it is cast
};

#define DND_SPELL_RANKCAP 10
#define DND_SPELL_THRESH_LOW 5
#define DND_SPELL_THRESH_HIGH 10

// ---- the rank curve --------------------------------------------------------------------------
// Two problems this solves. Ranks past the first used to cost nothing but a point, so a spell was
// maxed the moment it unlocked; and the linear per_rank step meant rank 10 was ~2.5x rank 1 while
// monster health over the same stretch grows ~8x, so every spell fell flat.
//
// Ranks now ladder up to a level cap, and damage-like fields take a compounding multiplier on top
// of the linear step. The growth rises with req_level, so a late spell climbs harder than an early
// one -- that direction is deliberate: it widens the gap to the capstones instead of closing it,
// which is what keeps Blaze from ever approaching Annihilus. See .claude/notes/dnd-spell-scaling.md.
#define DND_SPELL_RANKSPAN 40		// levels from unlock to rank 10, before the cap bites
#define DND_SPELL_MAXREQLEVEL 83	// no rank may ever ask for more than this
#define DND_SPELL_TOPREQLEVEL 52	// highest req_level in any tree, the growth ramp's far end
#define DND_SPELL_GROWBASE 13		// percent per rank at req_level 0
#define DND_SPELL_GROWSPAN 11		// added percent per rank by DND_SPELL_TOPREQLEVEL

// COST takes the same curve at a fraction of the rate. Without this the damage curve makes
// mana free: Pyroblast's damage grows 9.3x over its ladder against a 2.35x linear cost, so
// damage per mana improved ~4x just by ranking up. At 50 it improves ~2x, which still rewards
// the investment without the pool becoming irrelevant.
#define DND_SPELL_COSTGROWSHARE 50	// percent OF rank_grow that COST climbs at

// Spells that charge HEALTH instead of mana (Boiling Blood's drain, Righteous Fire's health
// percent) climb gentler still. A health cost is paid out of the thing that keeps you alive,
// so it cannot track a mana cost one for one without the spell turning into a suicide button.
#define DND_SPELL_DRAINGROWSHARE 20	// percent OF rank_grow that a self-cost field climbs at

typedef struct {
	int base[SPELLVAL_MAX];
	int per_rank[SPELLVAL_MAX];
	int req_spell[DND_MAX_SKILL_REQ];	// stored +1 so an unset slot reads 0
	int req_rank[DND_MAX_SKILL_REQ];
	int req_level;
	int tree;
	int flags;
	int scale_mask;						// which SPELLVAL_* fields take the rank curve, as 1 << field
	int drain_mask;						// fields that are a SELF cost in health, on the gentler curve
	int tx;								// authored position in tree space
	int ty;
} spell_def_T;

global spell_def_T 46: SpellDefs[MAX_SPELL_IDS];

// The level at which this spell can reach DND_SPELL_RANKCAP. Capped, so a late spell gets a
// shorter ladder rather than one running past where the player can go.
int GetSpellMaxLevel(int spell) {
	return Min(SpellDefs[spell].req_level + DND_SPELL_RANKSPAN, DND_SPELL_MAXREQLEVEL);
}

// Front loaded, half linear half quadratic: cheap early ranks, dear late ones. Endpoints unchanged.
// See .claude/notes/dnd-spell-scaling.md.
int GetSpellRankLevelReq(int spell, int rank) {
	if(rank <= 1)
		return SpellDefs[spell].req_level;

	int k = rank - 1;
	int n = DND_SPELL_RANKCAP - 1;
	int span = GetSpellMaxLevel(spell) - SpellDefs[spell].req_level;

	return SpellDefs[spell].req_level + span * (n * k + k * k) / (2 * n * n);
}

// Percent compounded per rank, keyed off how deep in the tree the spell sits.
int GetSpellRankGrow(int spell) {
	return DND_SPELL_GROWBASE +
		DND_SPELL_GROWSPAN * SpellDefs[spell].req_level / DND_SPELL_TOPREQLEVEL;
}

// A reservation is a percent of the pool, so it must never compound -- an aura would march itself
// to 100% reserved. Everything that actually spends mana scales.
int GetSpellCostGrow(int spell) {
	if(SpellDefs[spell].flags & SPLF_RESERVES)
		return 0;
	return GetSpellRankGrow(spell) * DND_SPELL_COSTGROWSHARE / 100;
}

int GetSpellDrainGrow(int spell) {
	return GetSpellRankGrow(spell) * DND_SPELL_DRAINGROWSHARE / 100;
}

// Compounded, as a percent. Integer truncation each step is intentional -- it keeps this identical
// on client and server, which a fixed point pow would not.
//
// The loop is bounded because the rank reaching here comes from GetSpellRank(.., true), which adds
// PSTAT_SPELLLEVEL_* UNCAPPED. Nothing writes those today, but a garbage value must not spin here,
// and a +levels mod must not multiply damage without someone deciding the cap first.
int CompoundRankMult(int grow, int rank) {
	int res = 100, top = Min(rank, DND_SPELL_RANKCAP);
	for(int i = 1; i < top; ++i)
		res = res * (100 + grow) / 100;
	return res;
}

int GetSpellRankMult(int spell, int rank) {
	return CompoundRankMult(GetSpellRankGrow(spell), rank);
}

int GetSpellCostMult(int spell, int rank) {
	return CompoundRankMult(GetSpellCostGrow(spell), rank);
}

int GetSpellDrainMult(int spell, int rank) {
	return CompoundRankMult(GetSpellDrainGrow(spell), rank);
}

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
// Ranks pack 8 per int at 4 bits each, which covers the 0-10 range. Sized off the id space rather
// than off the spell count, because the id space is what indexes them and most of it is empty.
// Hotbar stores id + 1 so an untouched slot reads as empty.
#define SPELL_RANK_INTS (MAX_SPELL_IDS / 8)
#define MAX_HOTBAR_SLOTS 6
#define DND_HOTBAR_BASESLOTS 3
#define HOTBAR_EMPTY 0
#define SPELL_AURA_INTS (MAX_SPELL_IDS / 32)

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
	SendOwnerSync("DnD Request Spell Sync", pnum, pnum, spell >> 3,
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
// at_rank is the rank being bought, so the level gate ladders instead of only guarding rank 1.
// It defaults to 1, which is the "can this node ever open" question the tree draw asks.
bool IsSpellUnlockable(int pnum, int spell, int at_rank = 1) {
	if(GetActorLevel(pnum + P_TIDSTART) < GetSpellRankLevelReq(spell, at_rank))
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

	return true;
}

// The one place the rank curve lives: base plus a linear step per rank past the first, then the
// synergy rows that name this field. Nothing clamps, because the only shrinking values left in the
// trees are cooldown RECOVERY RATE, which is asymptotic through FinalCD = CD / (1 + rate/100).
// rank_at previews a rank the player does not have, which is the only way the tree can show a
// locked spell's numbers. Zero means "whatever they actually have".
// v * pct / 100 without overflowing int32, for the curve below.
//
// SPELLVAL_COST and the other time fields are 16.16, so Volcano's 235 mana is already 15,400,960
// before the curve touches it. Times a 272% rank multiplier that is 4.19 BILLION, which wraps to
// -105,906,176 -- a cost of -17. SpendSpellMana then passes its affordability check against a
// negative number and SUBTRACTS it, handing the caster mana for casting.
//
// Splitting the divide across quotient and remainder keeps the full precision of the old form and
// never builds the product that wrapped.
int ScaleSpellValue(int v, int pct) {
	return (v / 100) * pct + ((v % 100) * pct) / 100;
}

// The rank of `source` that `spell` FORCES the player to own before it can be unlocked at all.
//
// Zero under SPLF_REQ_ANY: there the list is an OR, so the source may never have been bought and
// any ranks in it genuinely were a choice.
int GetForcedSourceRank(int spell, int source) {
	if(SpellDefs[spell].flags & SPLF_REQ_ANY)
		return 0;

	int i;
	for(i = 0; i < DND_MAX_SKILL_REQ; ++i) {
		if(!SpellDefs[spell].req_spell[i])
			continue;
		if(SpellDefs[spell].req_spell[i] - 1 == source)
			return SpellDefs[spell].req_rank[i];
	}

	return 0;
}

int GetSpellValue(int pnum, int spell, int which, int rank_at = 0) {
	int rank = rank_at ? rank_at : GetSpellRank(pnum, spell, true);
	if(!rank)
		return 0;

	int res = SpellDefs[spell].base[which] + (rank - 1) * SpellDefs[spell].per_rank[which];

	// The compounding part of the curve. Only fields the def opted in carry it -- DAMAGE doubles
	// as a percent or a health figure on plenty of spells, and those must stay linear.
	if(SpellDefs[spell].scale_mask & (1 << which))
		res = ScaleSpellValue(res, GetSpellRankMult(spell, rank));
	// Never more than one -- COST and the drain fields are kept out of scale_mask so neither can
	// take the full damage rate.
	else if(SpellDefs[spell].drain_mask & (1 << which))
		res = ScaleSpellValue(res, GetSpellDrainMult(spell, rank));
	else if(which == SPELLVAL_COST)
		res = ScaleSpellValue(res, GetSpellCostMult(spell, rank));

	int i, src, more = 0;
	for(i = 0; i < MAX_SPELL_SYNERGIES && SpellSynergies[i].target; ++i) {
		if(SpellSynergies[i].target - 1 != spell || SpellSynergies[i].field != which)
			continue;

		src = GetSpellRank(pnum, SpellSynergies[i].source, true);

		// Only ranks the player CHOSE pay out. Where this spell already forces the source to a rank,
		// those are the price of entry rather than a synergy -- counting them handed the spell its
		// own synergy bonus for free the instant it unlocked, which made it part of the base value
		// wearing a synergy label.
		//
		// EFFECTIVE rank against an ALLOCATED floor on purpose: +spell level from gear was never
		// forced, so it still counts.
		src -= GetForcedSourceRank(spell, SpellSynergies[i].source);
		if(src <= 0)
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

// Whether casting this is a HOSTILE ACT -- which is a wider question than whether it deals
// damage. A curse, a slow and a summon all announce you to a room without a point of damage
// between them.
//
// Hostile by DEFAULT, and a support spell opts out with SPLF_SUPPORT. That direction is the
// deliberate one: forgetting the flag on a new buff costs a pointless alert, where the inverse
// would let a new attack spell silently sneak up on a sleeping room. An earlier version tested
// scale_mask instead and missed exactly the cases this was meant to catch.
// COST, COOLDOWN, CASTTIME and DURATION are stored as fixed point; DAMAGE, RADIUS and AMOUNT are
// plain integers. Anything printing a raw row value has to know which, or a +0.25s duration bonus
// reads as "+16384".
bool IsSpellFieldFixedPoint(int field) {
	return field == SPELLVAL_COST || field == SPELLVAL_COOLDOWN ||
		field == SPELLVAL_CASTTIME || field == SPELLVAL_DURATION;
}

bool IsHostileSpell(int spell) {
	return !(SpellDefs[spell].flags & SPLF_SUPPORT);
}

// Icons are positional: SPL<id> in colour, SPL<id>G greyscale for a locked tree node. The prefix
// is three characters because a graphic lump name may not exceed eight -- SPICO<id>G would have
// capped the whole system at id 99.
// Anything that works from allocation alone rather than from a cast, and can therefore be switched
// off: auras, which hold a mana reservation, and passives, whose effect a build may not want.
bool IsSpellToggleable(int spell) {
	return !!(SpellDefs[spell].flags & (SPLF_AURA | SPLF_PASSIVE));
}

// Whether a real time toggle is currently RUNNING. A different thing from the menu opt out below:
// that one is a build decision the player leaves set, this one is switched on and off mid fight by
// casting. Deliberately outside SpellPlayerData, which is saved -- a spell left running at logout
// must come back off, and a map change clearing this is the behaviour we want.
typedef struct {
	int words[MAXPLAYERS][SPELL_AURA_INTS];
} spell_running_T;

spell_running_T module& GetSpellRunningData() {
	static spell_running_T s;
	return s;
}

bool IsSpellRunning(int pnum, int spell) {
	return !!(GetSpellRunningData().words[pnum][spell >> 5] & (1 << (spell & 31)));
}

void SetSpellRunning(int pnum, int spell, bool on) {
	auto run = GetSpellRunningData();
	if(on)
		run.words[pnum][spell >> 5] |= 1 << (spell & 31);
	else
		run.words[pnum][spell >> 5] &= ~(1 << (spell & 31));
}

// The bit means TOGGLED OFF, not "on". A zeroed global is the common case -- a spell the player has
// just learned and never touched -- and that has to read as working, so the flag records the opt OUT.
bool IsSpellToggledOff(int pnum, int spell) {
	return !!(SpellPlayerData[pnum].aura_on[spell >> 5] & (1 << (spell & 31)));
}

void SetSpellToggledOff(int pnum, int spell, bool off) {
	if(off)
		SpellPlayerData[pnum].aura_on[spell >> 5] |= 1 << (spell & 31);
	else
		SpellPlayerData[pnum].aura_on[spell >> 5] &= ~(1 << (spell & 31));
}

// Push the toggle word, the same way and for the same reason as SyncSpellRankWord: SpellPlayerData is
// a global, so a server side toggle would never reach the clientside pocket that has to display it.
void SyncSpellToggleWord(int pnum, int spell) {
	SendOwnerSync("DnD Request Spell Toggle Sync", pnum, pnum, spell >> 5,
		SpellPlayerData[pnum].aura_on[spell >> 5]);
}

// Learned AND not switched off. Every reader of a passive or an aura wants this, not the rank alone.
bool IsSpellActive(int pnum, int spell) {
	return GetSpellAllocatedRank(pnum, spell) > 0 && !IsSpellToggledOff(pnum, spell);
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
// A spell's offset inside its tree's block, which is what its icon is named after. Now just the id's
// position in the block, so the icon never moves -- it is tied to the SLOT rather than to how many
// spells happen to sit below it.
int GetSpellTreeIndex(int spell) {
	return spell % DND_SPELLS_PER_TREE;
}

// Whether a slot actually holds a spell. Most of the id space does not, and a zeroed def reports tree
// 0 -- so without this every empty slot would read as a Fire spell sitting at position (0,0).
// req_level is the marker: SPELL_DEF always writes one and no real spell has a level of zero.
bool IsSpellDefined(int spell) {
	return spell >= 0 && spell < MAX_SPELL_IDS && SpellDefs[spell].req_level > 0;
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

// A field one spell uses for something its generic name does not describe: Blizzard's DAMAGE2 is
// its volley rate in tenths of a percent, Rain of Fire's its extra comets per volley.
bool IsSpellFieldTenthsPercent(int spell, int which) {
	return spell == SPL_BLIZZARD && which == SPELLVAL_DAMAGE2;
}

str GetSpellFieldLumpFor(int spell, int which) {
	if(IsSpellFieldTenthsPercent(spell, which))
		return "DND_SPLFIELD_VOLLEYRATE";
	if(spell == SPL_RAINOFFIRE && which == SPELLVAL_DAMAGE2)
		return "DND_SPLFIELD_COMETS";
	return GetSpellFieldLump(which);
}

#endif
