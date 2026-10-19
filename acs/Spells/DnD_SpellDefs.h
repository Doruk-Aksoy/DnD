#ifndef DND_SPELLDEFS_IN
#define DND_SPELLDEFS_IN

#include "../DnD_SkillDef.h"

// Every tree owns a fixed BLOCK of ids -- tree N holds N*40 .. N*40+39 -- rather than the trees
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
#define DND_SPELLS_PER_TREE 40	// the tree page's ceiling: its labels run RANK_ID + 1..40, its nodes NODE_ID + 1..40
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
	SPL_AVALANCHE,

	// ---- Lightning ----
	SPL_CHARGEDBOLT = DND_SKILLTREE_LIGHTNING * DND_SPELLS_PER_TREE,
	SPL_SPARK,
	SPL_LIGHTNINGSPEAR,
	SPL_SHOCK,
	SPL_TELEKINESIS,
	SPL_STATICFIELD,
	SPL_FORKEDLIGHTNING,
	SPL_CRACKLINGLANCE,
	SPL_BLINK,
	SPL_OVERCHARGE,
	SPL_ENERGYBLADE,
	SPL_CONDUCTIVITY,
	SPL_CHAINLIGHTNING,
	SPL_STORMBRAND,
	SPL_WRATH,
	SPL_GALVANICSURGE,
	SPL_MONSOON,
	SPL_THUNDERCLAP,
	SPL_STORMCALL,
	SPL_SURGECAST,

	// ---- Earth ----
	SPL_SHRAPNEL = DND_SKILLTREE_EARTH * DND_SPELLS_PER_TREE,
	SPL_NOXIOUSBLAST,
	SPL_ROCKSLIDE,
	SPL_BLOCKADE,
	SPL_BALEFULINFUSION,
	SPL_TOXICTRAP,
	SPL_VENOMGYRE,
	SPL_ROCKARMOR,
	SPL_UPHEAVAL,
	SPL_THREADEDVOLLEY,
	SPL_VINESOFTORMENT,
	SPL_CORROSIVEBREATH,
	SPL_AGONY,
	SPL_ROCKGOLEM,
	SPL_VULNERABILITY,
	SPL_CRUSHINGROCKS,
	SPL_DESPAIR,
	SPL_POISONNOVA,
	SPL_UNLEASHEARTH,
	SPL_GEYSER
};

// Which numbers a spell carries. The DAMAGE, RADIUS and AMOUNT fields are plain integers; COST,
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
	SPELLVAL_DAMAGEMIN,		// a ranged DAMAGE's low end; it takes DAMAGE's synergies
	SPELLVAL_DAMAGE2MIN,	// and DAMAGE2's
	SPELLVAL_AMOUNT2,
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
	SPLF_SUPPORT		= 1024,		// self or ally only; nothing hostile happens when it is cast

	// Shape tags: what a spell IS, for gems and anything else that must know which modifiers can touch it.
	// PROJECTILE, CURSE and SUMMON above are shape tags too.
	SPLF_AOE			= 2048,		// affects an area: a radius, a zone, a cone or an aura's reach
	SPLF_DURATION		= 4096,		// leaves something timed behind: a zone, a buff or debuff, a burn
	SPLF_DOT			= 8192,		// deals damage over time
	SPLF_TOGGLE			= 16384,	// switched on and off, paying upkeep while it runs
	SPLF_RIDER			= 32768,	// empowers the caster's next attack instead of acting itself
	SPLF_HEAL			= 65536,	// restores life
	SPLF_TRAP			= 131072,	// placed, and goes off when an enemy comes near
	SPLF_MOVE			= 262144,	// moves the caster
	SPLF_CHAIN			= 524288,	// jumps from target to target
	SPLF_NOCOST			= 1048576	// an active spell that costs nothing to cast
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
// which is what keeps Blaze from ever approaching Annihilus.
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
	int even_mask;						// fields that step once per two ranks: +per_rank at rank 2, 4, ...
	int flat_mask;						// COST kept off the cost curve, so a cut per rank stays a cut
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
	int step;		// source ranks per payout: 2 pays once every two ranks
} spell_syn_T;

// target is stored +1, so the first zero row ends the list and no separate count is needed.
#define MAX_SPELL_SYNERGIES 192
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

// Gem sockets: the first opens at allocated rank 4, the second with it once RES_GEMSOCKET2 is done.
#define DND_SPELL_MAXSOCKETS 2
#define DND_SPELL_SOCKETRANK 4
#define SPELL_SOCKET_INTS (MAX_SPELL_IDS * DND_SPELL_MAXSOCKETS)

typedef struct {
	int ranks[SPELL_RANK_INTS];
	int hotbar[MAX_HOTBAR_SLOTS];
	int aura_on[SPELL_AURA_INTS];
	int unspent;
	int sockets[SPELL_SOCKET_INTS];		// spell * DND_SPELL_MAXSOCKETS + slot; see MakeSocketValue
} spell_player_T;

global spell_player_T 45: SpellPlayerData[MAXPLAYERS];

// ---- per player, per spell: gem modifiers ----------------------------------------------------
// A cache rebuilt from a spell's sockets whenever they change; the sockets are what gets saved.
enum {
	// MORE terms: the product minus one, in 16.16, so an empty row means no change. A "less" is negative.
	GEMSTAT_MORE_DAMAGE,
	GEMSTAT_MORE_HITDAMAGE,
	GEMSTAT_MORE_DOTDAMAGE,
	GEMSTAT_MORE_COST,
	GEMSTAT_MORE_HASTE,
	GEMSTAT_MORE_DURATION,
	GEMSTAT_MORE_AREA,
	GEMSTAT_MORE_RESERVE,		// mana reservation
	GEMSTAT_MORE_MINIONLIFE,
	GEMSTAT_MORE_MINIONDAMAGE,
	GEMSTAT_MORE_HEALING,
	GEMSTAT_MORE_CURSE,			// curse effect
	GEMSTAT_MORE_LOWLIFE,		// damage to targets on low life
	// Additive terms, each in the unit its hook already uses.
	GEMSTAT_HASTE,				// percent, beside PSTAT_SPELL_HASTE
	GEMSTAT_CDR,				// percent cooldown recovery rate
	GEMSTAT_CRITCHANCE,			// 16.16, 0.01 is 1%
	GEMSTAT_CRITMULT,			// percent
	GEMSTAT_RESERVE_EFF,		// percent reservation efficiency
	GEMSTAT_EXTRAPROJ,
	GEMSTAT_IGNITECHANCE,		// the four ailment chances: percent, rolled on any element
	GEMSTAT_CHILLCHANCE,
	GEMSTAT_POISONCHANCE,
	GEMSTAT_OVERLOADCHANCE,
	GEMSTAT_BLEEDCHANCE,
	GEMSTAT_LEECH,				// tenths of a percent of damage dealt, as life
	GEMSTAT_PENETRATION,		// percent resistance penetration
	GEMSTAT_CANCULL,			// non zero: the spell's hits can cull
	GEMSTAT_CULLRANGE,			// percent added to the cull range
	GEMSTAT_MAGUS,				// percent chance to get the unreduced cost back on casting
	GEMSTAT_ARCHMAGE,			// percent of unreserved mana added as lightning damage
	GEMSTAT_ELUSIVE,			// percent chance on a crit to gain Elusive
	GEMSTAT_REPEAT,				// extra casts
	GEMSTAT_FREEPARRY,			// tics after casting in which a parry is free
	GEMSTAT_STAMINAREGEN,		// percent faster stamina recovery after casting
	GEMSTAT_ENDURANCECHANCE,	// percent chance to gain a charge: on stagger, on chill or freeze, on crit
	GEMSTAT_FRENZYCHANCE,
	GEMSTAT_POWERCHANCE,
	GEMSTAT_ENDURANCEDAMAGE,	// percent more damage per charge of that kind
	GEMSTAT_FRENZYDAMAGE,
	GEMSTAT_POWERDAMAGE,
	GEMSTAT_PASSIVE_EFFECT,		// percent increased passive effect
	GEMSTAT_RESERVE_ADD,		// percent of mana the spell reserves while active
	GEMSTAT_LIFECOST,			// percent of the final mana cost paid in life instead; 0 pays mana
	GEMSTAT_NECRO,				// percent chance a slain minion returns
	MAX_GEMSTATS
};
#define LAST_GEMSTAT_MORE GEMSTAT_MORE_LOWLIFE
#define GEMSTAT_ROWMASK MAX_GEMSTATS			// two words: a bit per stat the row holds, so syncs and resets skip the rest
#define GEMSTAT_ROWPREV (MAX_GEMSTATS + 2)		// the mask before a rebuild, so the sync also covers what it cleared
#define GEMSTAT_ROWSIZE (MAX_GEMSTATS + 4)

global int 40: SpellGemMods[MAXPLAYERS][MAX_SPELL_IDS][GEMSTAT_ROWSIZE];

int GetSpellGemMod(int pnum, int spell, int stat) {
	return SpellGemMods[pnum][spell][stat];
}

// v under one MORE term, rounded: 0.15 is 9830 in 16.16, and a truncated 100 tics x 1.15 came out 114.
int ApplySpellGemMore(int pnum, int spell, int stat, int v) {
	int m = SpellGemMods[pnum][spell][stat];
	return m ? FixedMulRound(v, 1.0 + m) : v;
}

// MORE area scales AREA, so a radius takes the root of it.
int ApplySpellGemArea(int pnum, int spell, int r) {
	int m = SpellGemMods[pnum][spell][GEMSTAT_MORE_AREA];
	return m ? FixedMulRound(r, fsqrt(1.0 + m)) : r;
}

bool GemRowHasBit(int pnum, int spell, int base, int stat) {
	return !!(SpellGemMods[pnum][spell][base + (stat >> 5)] & (1 << (stat & 31)));
}

// One gem term into a spell's row: additive terms add, MORE terms multiply, so two Rubies are 1.15 x 1.15.
void AddSpellGemMod(int pnum, int spell, int stat, int val) {
	int cur = SpellGemMods[pnum][spell][stat];
	if(stat <= LAST_GEMSTAT_MORE)
		cur += val + FixedMul(cur, val);
	else
		cur += val;

	SpellGemMods[pnum][spell][stat] = cur;
	int w = GEMSTAT_ROWMASK + (stat >> 5);
	if(cur)
		SpellGemMods[pnum][spell][w] |= 1 << (stat & 31);
	else
		SpellGemMods[pnum][spell][w] &= ~(1 << (stat & 31));
}

// A term that does not stack, set outright: Haemocrafter's life cost keeps the cheapest multiplier socketed.
void SetSpellGemMod(int pnum, int spell, int stat, int val) {
	SpellGemMods[pnum][spell][stat] = val;
	SpellGemMods[pnum][spell][GEMSTAT_ROWMASK + (stat >> 5)] |= 1 << (stat & 31);
}

// For files ahead of this one in the include order, where the macro is not yet defined.
bool IsGemStatMore(int stat) {
	return stat <= LAST_GEMSTAT_MORE;
}

// A whole percent in the stat's stored unit: 16.16 for the MORE terms and crit chance, as is for the rest.
int GemStatFromPercent(int stat, int pct) {
	if(stat <= LAST_GEMSTAT_MORE || stat == GEMSTAT_CRITCHANCE)
		return (pct << 16) / 100;
	return pct;
}

// Remembers the mask before an edit, so EndSpellGemRebuild can sync whatever the edit cleared.
void SnapshotSpellGemMask(int pnum, int spell) {
	SpellGemMods[pnum][spell][GEMSTAT_ROWPREV] = SpellGemMods[pnum][spell][GEMSTAT_ROWMASK];
	SpellGemMods[pnum][spell][GEMSTAT_ROWPREV + 1] = SpellGemMods[pnum][spell][GEMSTAT_ROWMASK + 1];
}

// Empties a spell's row before its gems are re-added.
void BeginSpellGemRebuild(int pnum, int spell) {
	SnapshotSpellGemMask(pnum, spell);
	for(int i = 0; i < MAX_GEMSTATS; ++i)
		if(GemRowHasBit(pnum, spell, GEMSTAT_ROWPREV, i))
			SpellGemMods[pnum][spell][i] = 0;
	SpellGemMods[pnum][spell][GEMSTAT_ROWMASK] = 0;
	SpellGemMods[pnum][spell][GEMSTAT_ROWMASK + 1] = 0;
}

// Sends the owner every stat the row held before or holds now: a global, so the client keeps its own copy.
void EndSpellGemRebuild(int pnum, int spell) {
	for(int i = 0; i < MAX_GEMSTATS; ++i)
		if(GemRowHasBit(pnum, spell, GEMSTAT_ROWPREV, i) || GemRowHasBit(pnum, spell, GEMSTAT_ROWMASK, i))
			SendOwnerSync("DnD Request Spell Gem Sync", pnum, pnum, spell, i, SpellGemMods[pnum][spell][i]);
}

// Every row a player holds, for a reset; the mask skips spells without gems.
void ResetSpellGemMods(int pnum) {
	for(int s = 0; s < MAX_SPELL_IDS; ++s) {
		if(SpellGemMods[pnum][s][GEMSTAT_ROWMASK] || SpellGemMods[pnum][s][GEMSTAT_ROWMASK + 1]) {
			BeginSpellGemRebuild(pnum, s);
			EndSpellGemRebuild(pnum, s);
		}
	}
}

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

// Rank 5 and 10 bonuses that move a row's number, added before gear so the cast and the tooltip agree.
int GetSpellThresholdBonus(int spell, int which, int rank) {
	bool r5 = rank >= DND_SPELL_THRESH_LOW, r10 = rank >= DND_SPELL_THRESH_HIGH;
	switch(spell) {
		case SPL_STORMBRAND:
			return (which == SPELLVAL_AMOUNT && r5) || (which == SPELLVAL_AMOUNT2 && r10) ? 1 : 0;
		case SPL_MONSOON:
			return which == SPELLVAL_DURATION && r5 ? 5.0 : 0;
		case SPL_THUNDERCLAP:
			return which == SPELLVAL_AMOUNT && r10 ? 1 : 0;
		case SPL_STORMCALL:
			return which == SPELLVAL_RADIUS && r5 ? 128 : which == SPELLVAL_DAMAGE2 && r10 ? 25 : 0;
		case SPL_NOXIOUSBLAST:
			return which == SPELLVAL_AMOUNT2 && r5 ? 10 : which == SPELLVAL_RADIUS && r10 ? 64 : 0;
		case SPL_ROCKSLIDE:
			return which == SPELLVAL_RADIUS && r10 ? 84 : 0;
		case SPL_TOXICTRAP:
			return which == SPELLVAL_AMOUNT && r5 ? 2 : which == SPELLVAL_DURATION && r10 ? 1.0 : 0;
	}
	return 0;
}

int GetSpellValue(int pnum, int spell, int which, int rank_at = 0) {
	int rank = rank_at ? rank_at : GetSpellRank(pnum, spell, true);
	if(!rank)
		return 0;

	// Blink: no cast time from rank 5, free from rank 10. Here so the cast, the bill and the tooltip agree.
	if(spell == SPL_BLINK && ((which == SPELLVAL_CASTTIME && rank >= DND_SPELL_THRESH_LOW) || (which == SPELLVAL_COST && rank >= DND_SPELL_THRESH_HIGH)))
		return 0;

	int steps = (SpellDefs[spell].even_mask & (1 << which)) ? rank / 2 : rank - 1;
	int res = SpellDefs[spell].base[which] + steps * SpellDefs[spell].per_rank[which];

	// The compounding part of the curve. Only fields the def opted in carry it -- DAMAGE doubles
	// as a percent or a health figure on plenty of spells, and those must stay linear.
	if(SpellDefs[spell].scale_mask & (1 << which))
		res = ScaleSpellValue(res, GetSpellRankMult(spell, rank));
	// Never more than one -- COST and the drain fields are kept out of scale_mask so neither can
	// take the full damage rate.
	else if(SpellDefs[spell].drain_mask & (1 << which))
		res = ScaleSpellValue(res, GetSpellDrainMult(spell, rank));
	else if(which == SPELLVAL_COST && !(SpellDefs[spell].flat_mask & (1 << which)))
		res = ScaleSpellValue(res, GetSpellCostMult(spell, rank));

	res += GetSpellThresholdBonus(spell, which, rank);

	// A range's low end takes the rows written for its high end, so "5% more damage" moves both.
	int syn_field = which == SPELLVAL_DAMAGEMIN ? SPELLVAL_DAMAGE : which == SPELLVAL_DAMAGE2MIN ? SPELLVAL_DAMAGE2 : which;
	int i, src, more = 0;
	for(i = 0; i < MAX_SPELL_SYNERGIES && SpellSynergies[i].target; ++i) {
		if(SpellSynergies[i].target - 1 != spell || SpellSynergies[i].field != syn_field)
			continue;

		src = GetSpellRank(pnum, SpellSynergies[i].source, true);

		// Only ranks the player CHOSE pay out. Where this spell already forces the source to a rank,
		// those are the price of entry rather than a synergy -- counting them handed the spell its
		// own synergy bonus for free the instant it unlocked, which made it part of the base value
		// wearing a synergy label.
		//
		// EFFECTIVE rank against an ALLOCATED floor on purpose: +spell level from gear was never
		// forced, so it still counts.
		src = (src - GetForcedSourceRank(spell, SpellSynergies[i].source)) / SpellSynergies[i].step;
		if(src <= 0)
			continue;

		if(SpellSynergies[i].flags & SYNF_FLAT)
			res += SpellSynergies[i].per_rank * src;
		else
			more += SpellSynergies[i].per_rank * src;
	}

	// more is a 16.16 percent, so 2.5 a rank pays 2.5 and not 2. Applied in hundredths, split as ScaleSpellValue
	// is, since COST, DURATION and the other 16.16 fields overflow a plain product.
	if(more) {
		more = 10000 + (more * 25 >> 14);
		res = (res / 10000) * more + ((res % 10000) * more) / 10000;
	}

	// Gear applies last and is additive, so a synergy's "more" does not multiply it. Spell area gear is not here:
	// it scales AREA, in ScaleSpellArea, where it joins the other area increases rather than multiplying them.
	if(which == SPELLVAL_DURATION) {
		res = ScaleSpellValue(res, Max(0, 100 + PlayerModData[pnum].vals[PSTAT_SPELL_DURATION]));
		res = ApplySpellGemMore(pnum, spell, GEMSTAT_MORE_DURATION, res);
	}
	// A reservation is not a cost: gems' less cost stays off auras, which have reservation efficiency instead.
	else if(which == SPELLVAL_COST && !(SpellDefs[spell].flags & SPLF_RESERVES))
		res = ApplySpellGemMore(pnum, spell, GEMSTAT_MORE_COST, res);
	// Influencer: a passive's effect is its DAMAGE row on all four (burst share, more fire or cold, cooldown cut).
	else if(which == SPELLVAL_DAMAGE && (SpellDefs[spell].flags & SPLF_PASSIVE))
		res = ScaleSpellValue(res, 100 + GetSpellGemMod(pnum, spell, GEMSTAT_PASSIVE_EFFECT));

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

// A slot's whole spell state, for a reset. The globals outlive the connection, so a new character in a
// reused slot inherited the last one's ranks, toggles and binds.
void ResetSpellPlayerData(int pnum) {
	int i;
	for(i = 0; i < SPELL_RANK_INTS; ++i) {
		if(SpellPlayerData[pnum].ranks[i]) {
			SpellPlayerData[pnum].ranks[i] = 0;
			SendOwnerSync("DnD Request Spell Sync", pnum, pnum, i, 0);
		}
	}

	for(i = 0; i < SPELL_AURA_INTS; ++i) {
		if(SpellPlayerData[pnum].aura_on[i]) {
			SpellPlayerData[pnum].aura_on[i] = 0;
			SendOwnerSync("DnD Request Spell Toggle Sync", pnum, pnum, i, 0);
		}
		GetSpellRunningData().words[pnum][i] = 0;
	}

	for(i = 0; i < MAX_HOTBAR_SLOTS; ++i)
		SetHotbarSpell(pnum, i, -1);
	SyncHotbarAll(pnum);

	for(i = 0; i < SPELL_SOCKET_INTS; ++i) {
		if(SpellPlayerData[pnum].sockets[i]) {
			SpellPlayerData[pnum].sockets[i] = 0;
			SendOwnerSync("DnD Request Spell Socket Sync", pnum, pnum, i, 0);
		}
	}

	SpellPlayerData[pnum].unspent = 0;
	ResetAllSpellCooldownsNew(pnum);
}

// ---- gem sockets ------------------------------------------------------------------------------
// A socket holds its gem as (subtype + 1) | (role + 1) << 9, so 0 is empty and the whole gem fits 15 bits.
int MakeSocketValue(int subt, int role) {
	return (subt + 1) | ((role + 1) << 9);
}

int GetSocketGemSubtype(int v) {
	return (v & 0x1FF) - 1;
}

int GetSocketGemRole(int v) {
	return (v >> 9) - 1;
}

int GetSpellSocket(int pnum, int spell, int slot) {
	return SpellPlayerData[pnum].sockets[spell * DND_SPELL_MAXSOCKETS + slot];
}

void SetSpellSocket(int pnum, int spell, int slot, int v) {
	int i = spell * DND_SPELL_MAXSOCKETS + slot;
	SpellPlayerData[pnum].sockets[i] = v;
	SendOwnerSync("DnD Request Spell Socket Sync", pnum, pnum, i, v);
}

// Sockets open on the ALLOCATED rank, so gear levels never open one; the second needs its research.
int GetSpellSocketCount(int pnum, int spell) {
	if(GetSpellAllocatedRank(pnum, spell) < DND_SPELL_SOCKETRANK)
		return 0;
	return 1 + (CheckActorResearchStatus(pnum + P_TIDSTART, RES_GEMSOCKET2) == RES_DONE);
}

// One spell's gem row from its sockets: the targeted rebuild every socket change and load ends in.
void RebuildSpellGems(int pnum, int spell) {
	BeginSpellGemRebuild(pnum, spell);
	int v;
	for(int i = 0; i < GetSpellSocketCount(pnum, spell); ++i)
		if((v = GetSpellSocket(pnum, spell, i)))
			AddGemToSpellRow(pnum, spell, GetSocketGemSubtype(v), GetSocketGemRole(v));
	EndSpellGemRebuild(pnum, spell);
}

// After a load: the client gets every rank, toggle and socket, and each socketed spell rebuilds its gems.
void SyncAllSpellData(int pnum) {
	int i;
	for(i = 0; i < SPELL_RANK_INTS; ++i)
		SendOwnerSync("DnD Request Spell Sync", pnum, pnum, i, SpellPlayerData[pnum].ranks[i]);
	for(i = 0; i < SPELL_AURA_INTS; ++i)
		SendOwnerSync("DnD Request Spell Toggle Sync", pnum, pnum, i, SpellPlayerData[pnum].aura_on[i]);

	for(i = 0; i < MAX_SPELL_IDS; ++i) {
		if(!GetSpellSocket(pnum, i, 0) && !GetSpellSocket(pnum, i, 1))
			continue;
		SendOwnerSync("DnD Request Spell Socket Sync", pnum, pnum, i * DND_SPELL_MAXSOCKETS, GetSpellSocket(pnum, i, 0));
		SendOwnerSync("DnD Request Spell Socket Sync", pnum, pnum, i * DND_SPELL_MAXSOCKETS + 1, GetSpellSocket(pnum, i, 1));
		RebuildSpellGems(pnum, i);
	}
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

// The low end of a ranged damage field, or -1 when the field is a single number.
int GetSpellMinField(int spell, int which) {
	int lo = which == SPELLVAL_DAMAGE ? SPELLVAL_DAMAGEMIN : which == SPELLVAL_DAMAGE2 ? SPELLVAL_DAMAGE2MIN : -1;
	if(lo == -1 || !(SpellDefs[spell].scale_mask & (1 << lo)))
		return -1;
	return lo;
}

// " per rank", or " per 2 ranks" for a row that pays once every few.
str GetSynergyStepText(int syn) {
	if(SpellSynergies[syn].step > 1)
		return StrParam(s:" per ", d:SpellSynergies[syn].step, s:" ranks");
	return " per rank";
}

str GetSpellFieldLump(int which) {
	return StrParam(s:"DND_SPLFIELD", d:which);
}

// A field one spell uses for something its generic name does not describe: Blizzard's DAMAGE2 is
// its volley rate in tenths of a percent, Rain of Fire's its extra comets per volley.
bool IsSpellFieldTenthsPercent(int spell, int which) {
	return spell == SPL_BLIZZARD && which == SPELLVAL_DAMAGE2;
}

// A poison duration in tenths of a second: Earth's poisoning spells keep it in AMOUNT2.
bool IsSpellFieldPoisonTenths(int spell, int which) {
	if(which != SPELLVAL_AMOUNT2)
		return false;
	switch(spell) {
		case SPL_NOXIOUSBLAST:
		case SPL_BALEFULINFUSION:
		case SPL_TOXICTRAP:
		case SPL_VENOMGYRE:
		case SPL_CORROSIVEBREATH:
		case SPL_POISONNOVA:
		return true;
	}
	return false;
}

str GetSpellFieldLumpFor(int spell, int which) {
	if(IsSpellFieldTenthsPercent(spell, which))
		return "DND_SPLFIELD_VOLLEYRATE";
	if(IsSpellFieldPoisonTenths(spell, which))
		return "DND_SPLFIELD_POISONTIME";
	if(spell == SPL_RAINOFFIRE && which == SPELLVAL_DAMAGE2)
		return "DND_SPLFIELD_COMETS";
	if(spell == SPL_NOXIOUSBLAST && which == SPELLVAL_DAMAGE)
		return "DND_SPLFIELD_POISONPS";
	if((spell == SPL_CORROSIVEBREATH && which == SPELLVAL_DAMAGE) || ((spell == SPL_VENOMGYRE || spell == SPL_POISONNOVA) && which == SPELLVAL_DAMAGE2))
		return "DND_SPLFIELD_POISON";
	if(spell == SPL_UPHEAVAL && which == SPELLVAL_AMOUNT2)
		return "DND_SPLFIELD_THROW";
	if(spell == SPL_THREADEDVOLLEY && which == SPELLVAL_AMOUNT2)
		return "DND_SPLFIELD_ENERGIZED";
	return GetSpellFieldLump(which);
}

#endif
