#ifndef DND_STATCACHE_IN
#define DND_STATCACHE_IN

// ============================================================================
//  CACHED PLAYER STAT TRANSFORMS
//
//  A cached stat is a TRANSFORM, never a value. The per-shot base (the weapon's
//  randomized damage roll) is supplied at request time and is never stored here.
//
//      out = ApplyCachedPlayerStatToBase(pnum, id, rolled_base)
//          = (rolled_base + flat * flat_eff/100) * (100 + increased)/100 * PROD(mult[i])
//
//  Layer summary -- the unit of every field is fixed by this table:
//
//    flat        integer damage points     exactly reversible (+= / -=)
//    flat_eff    integer percent, 100=1x   per-weapon flat effectiveness
//    increased   integer percent DELTA     exactly reversible; total = 100 + increased
//    mult[]      16.16 ABSOLUTE multiplier NOT reversible -> kept as an array and
//                (1.0 == no change)        recomputed from scratch on removal
//
//  The product of mult[] is stored NORMALIZED as (net_mant, net_exp):
//
//      net = net_mant * 2^net_exp ,  net_mant in [1.0, 2.0) as 16.16
//
//  Why not a plain 16.16 product (the previous design):
//    - it overflows after ~40 stacked "+30% more" factors and WRAPS NEGATIVE
//      (FixedMul has a 64-bit intermediate but casts the result back to int32),
//    - "less" factors drive it to exactly 0 after 17 halvings, an absorbing state,
//    - and routing it through (100 * net) >> 16 to reach an integer percent
//      overflows at only 327.68x -- reached by 23 stacked "+30% more" factors.
//  The normalized form has no practical ceiling, is LOSSLESS in the shrink
//  direction (normalizing a mantissa upward is an exact shift), and holds a
//  constant ~1e-4 worst-case relative error out to 64 stacked factors.
// ============================================================================

#define MAX_MULTIPLICATIVE_FACTORS 64 // realistically won't even happen, but you never know

enum {
    DND_CACHEDSTAT_GENERICDAMAGE,

    DND_CACHEDSTAT_FLATPHYS,
    DND_CACHEDSTAT_PHYSICALDAMAGE,

    MAX_CACHED_STATS
};

enum {
    DND_STATCACHEFLAG_DIRTY          = 0b1,
    DND_STATCACHEFLAG_ZEROED         = 0b10,     // a x0 factor is present, result is a hard 0
    DND_STATCACHEFLAG_COMPONENTSETUP = 0b100,    // components have been allocated for this player
};

bool IsComponentUsingCachedStat(int id) {
    switch(id) {
        case DND_CACHEDSTAT_FLATPHYS:
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
//  NUMERIC PRIMITIVES
// ---------------------------------------------------------------------------

// Round-to-nearest fixed multiply, built only from operations BCS has.
//
// FixedMul(a,b) yields floor(a*b / 2^16) -- it truncates, and because the
// truncation is always in the same direction it accumulates into a SYSTEMATIC
// downward bias over a chain of multiplies. The correction bit is free: ACS's
// integer multiply wraps, but the LOW 16 BITS of the wrapped 32-bit product are
// still exact, and those are precisely the bits FixedMul discarded.
//
// Verified exact against true round-half-up over 400k random mantissa pairs.
int FixedMulRound(int a, int b) {
    return FixedMul(a, b) + (((a * b) & 0xFFFF) >= 0x8000);
}

// Exact  fixed_value * 100  as a plain integer percent.
//
// The idiom used everywhere else is (x * 100) >> 16, which wraps once x passes
// 327.68 because the multiply happens in 32 bits -- and wraps from +32767 straight
// to -32768, i.e. a huge bonus becomes a huge penalty. Splitting the integer and
// fractional halves cannot overflow for ANY 16.16 input: the integer part tops out
// at 32767 (x100 = 3,276,700) and the fractional term at 65535 * 100.
int FixedToPercent(int x) {
    bool neg = x < 0;
    if(neg) {
        if(x < -bcs::INT_MAX)   // -INT_MIN is not representable; one raw unit is nothing
            x = -bcs::INT_MAX;
        x = -x;
    }

    int res = (x >> 16) * 100 + (((x & 0xFFFF) * 100) >> 16);

    if(neg)
        return -res;
    return res;
}

// Applies a normalized multiplier (mant, exp) to a PLAIN INTEGER value.
//
// Two things make this cheap and accurate:
//   - FixedMul(int_value, fixed_factor) already gives "int_value scaled by the
//     factor" as a plain integer, using the native's 64-bit intermediate. No
//     <<16 staging, and no 32-bit intermediate to overflow.
//   - For a positive exponent we shift UP FIRST. Multiplying then shifting left
//     amplifies the multiply's truncation error by 2^exp -- that ordering
//     measures 4.5e-3 worst-case relative error at damage >= 1000, versus
//     9.8e-4 for this ordering.
int ApplyNormalizedMultiplier(int v, int mant, int exp) {
    if(v <= 0 || mant <= 0)
        return 0;

    if(exp > 0) {
        // guard leaves one bit of room because mant can be just under 2.0
        if(exp >= 30 || v > (bcs::INT_MAX >> (exp + 1)))
            return bcs::INT_MAX;
        return FixedMul(v << exp, mant);
    }

    if(!exp) {
        // mant < 2.0, so the product still needs one bit of headroom. Without
        // this, v > 2^30 wraps the int32 cast NEGATIVE inside FixedMul.
        if(v > (bcs::INT_MAX >> 1))
            return bcs::INT_MAX;
        return FixedMul(v, mant);
    }

    // exp < 0: shrinking
    int k = -exp;
    if(k >= 31)
        return 0;

    if(v > (bcs::INT_MAX >> 1)) {
        // too large to multiply first without wrapping, so shift first. At this
        // magnitude 30+ bits survive, so the precision cost is immaterial.
        v = (v + (1 << (k - 1))) >> k;
        return FixedMul(v, mant);
    }

    v = FixedMul(v, mant);
    return (v + (1 << (k - 1))) >> k; // rounding shift, not a bare >>
}

// Applies TWO normalized transforms in one pass.
//
// This is the whole point of the normalized form: two transforms compose by
// multiplying mantissas and ADDING exponents, so a slow-moving per-weapon
// transform and a fast-moving per-player buff transform can live in SEPARATE
// caches with SEPARATE dirty bits and still cost one multiply at request time.
//
// That separation is not an optimization, it is a correctness requirement. A
// buff percentage folded into a per-weapon cached factor is frozen there until
// something forces a rebuild, so the bonus is either permanently present or
// permanently absent regardless of whether the buff is actually up.
int ApplyTwoNormalizedMultipliers(int v, int m1, int e1, int m2, int e2) {
    if(v <= 0 || m1 <= 0 || m2 <= 0)
        return 0;

    // both mantissas are in [1.0, 2.0), so the product is in [1.0, 4.0)
    // and cannot overflow; one conditional shift renormalizes it.
    int mant = FixedMulRound(m1, m2);
    int exp = e1 + e2;

    if(mant >= 2.0) {
        mant = (mant + 1) >> 1;
        ++exp;
    }

    return ApplyNormalizedMultiplier(v, mant, exp);
}

// ---------------------------------------------------------------------------
//  PACKED NORMALIZED MULTIPLIERS
//
//  A (mantissa, exponent) pair squeezed into one int, so a per-weapon cache can
//  hold one per slot instead of paying for two parallel arrays.
//
//      bits  0..15 : mantissa - 1.0   (mantissa is in [1.0, 2.0), so it fits)
//      bits 16..26 : exponent + bias
//
//  A packed value of 0 is RESERVED to mean "a x0 factor is present". That is why
//  the exponent is biased: the identity must not encode as 0.
// ---------------------------------------------------------------------------

#define DND_PACKED_EXP_BIAS         512
#define DND_PACKED_EXP_LIMIT        511
#define DND_PACKED_MANT_MASK        0xFFFF
#define DND_PACKED_MULT_ZERO        0
#define DND_PACKED_MULT_IDENTITY    (DND_PACKED_EXP_BIAS << 16)

int PackNormalizedMultiplier(int mant, int exp) {
    if(mant <= 0)
        return DND_PACKED_MULT_ZERO;

    // ApplyNormalizedMultiplier already saturates past +-31, so clamping here
    // costs nothing and keeps the reserved 0 pattern unreachable
    if(exp > DND_PACKED_EXP_LIMIT)
        exp = DND_PACKED_EXP_LIMIT;
    else if(exp < -DND_PACKED_EXP_LIMIT)
        exp = -DND_PACKED_EXP_LIMIT;

    return ((exp + DND_PACKED_EXP_BIAS) << 16) | ((mant - 1.0) & DND_PACKED_MANT_MASK);
}

int GetPackedMultiplierMantissa(int packed) {
    return 1.0 + (packed & DND_PACKED_MANT_MASK);
}

int GetPackedMultiplierExponent(int packed) {
    return (packed >> 16) - DND_PACKED_EXP_BIAS;
}

// scales a plain integer by a packed multiplier
int ApplyPackedMultiplier(int v, int packed) {
    if(!packed)
        return 0;
    return ApplyNormalizedMultiplier(v, GetPackedMultiplierMantissa(packed), GetPackedMultiplierExponent(packed));
}

// folds one ABSOLUTE 16.16 factor (1.0 == no change) into a packed multiplier
int CombinePackedMultiplier(int packed, int factor) {
    if(!packed || factor <= 0)
        return DND_PACKED_MULT_ZERO;

    int mant = GetPackedMultiplierMantissa(packed);
    int exp = GetPackedMultiplierExponent(packed);

    // normalize the incoming factor into [1.0, 2.0). Shifting UP is exact, which
    // is what stops a stack of "less" factors from collapsing to nothing.
    int fe = 0;
    while(factor >= 2.0) {
        factor = (factor + 1) >> 1;
        ++fe;
    }
    while(factor < 1.0) {
        factor <<= 1;
        --fe;
    }

    // both operands in [1.0, 2.0) => product in [1.0, 4.0), cannot overflow
    mant = FixedMulRound(mant, factor);
    exp += fe;

    if(mant >= 2.0) {
        mant = (mant + 1) >> 1;
        ++exp;
    }

    return PackNormalizedMultiplier(mant, exp);
}

// Merges two packed multipliers into one. Needed wherever a value is scaled by more than one packed
// product: applying them one after another truncates twice, and the note on ordering in
// ScaleCachedDamage is the whole reason that matters -- the second truncation amplifies the error of
// the first. Merge here, apply once.
//
// Both mantissas are already normalized into [1.0, 2.0), so this is just the tail of
// CombinePackedMultiplier with no renormalization pass needed on either operand.
int CombinePackedMultipliers(int packed_a, int packed_b) {
    if(!packed_a || !packed_b)
        return DND_PACKED_MULT_ZERO;

    // product of two [1.0, 2.0) values lands in [1.0, 4.0), so it cannot overflow
    int mant = FixedMulRound(GetPackedMultiplierMantissa(packed_a), GetPackedMultiplierMantissa(packed_b));
    int exp = GetPackedMultiplierExponent(packed_a) + GetPackedMultiplierExponent(packed_b);

    if(mant >= 2.0) {
        mant = (mant + 1) >> 1;
        ++exp;
    }

    return PackNormalizedMultiplier(mant, exp);
}

typedef struct {
    int source;                                     // owning token, so removal is by identity not by value
    int factor;                                     // 16.16 ABSOLUTE multiplier: 1.0 == no change, 1.25 == +25% more
} mult_entry_T;

typedef struct {
    int increased;                                  // integer percent DELTA on a 0 baseline; total = 100 + increased

    int mult_count;                                 // live entries in mult[]
    mult_entry_T mult[MAX_MULTIPLICATIVE_FACTORS];  // insertion order is load-bearing, see RemoveMultiplicative...

    int net_mant;                                   // 16.16 in [1.0, 2.0), or 0 when ZEROED
    int net_exp;                                    // net = net_mant * 2^net_exp
} stat_component_T;

#define STAT_COMPONENT_SIZE (SIZEOF_INT * (4 + 2 * MAX_MULTIPLICATIVE_FACTORS))

typedef struct {
    stat_component_T? components;                   // null for flat-only stats, to save memory
    int flat;                                       // integer damage points, added to the per-shot base
    int flat_eff;                                   // integer percent, 100 == full effectiveness
    int flags;
} cached_stat_T;

global cached_stat_T 49: PlayerStatCache[MAXPLAYERS][MAX_CACHED_STATS];

void ResetCachedPlayerStat(int pnum, int id) {
    auto stat = PlayerStatCache[pnum][id];

    if(IsComponentUsingCachedStat(id)) {
        if(!stat.components)
            stat.components = bcs::malloc(STAT_COMPONENT_SIZE);

        auto comp = stat.components;

        comp.increased = 0;

        // the identity for a list of ABSOLUTE multipliers is the EMPTY list -- seeding
        // a 1.0 entry here would read as "+100% more" to anything treating them as deltas
        comp.mult_count = 0;

        comp.net_mant = 1.0;
        comp.net_exp = 0;
    }

    stat.flat = 0;
    stat.flat_eff = 100;
    stat.flags = DND_STATCACHEFLAG_DIRTY;
}

// done when player joins and leaves
void PrepareCachedPlayerStats(int pnum, bool forced = false) {
    if((PlayerStatCache[pnum][0].flags & DND_STATCACHEFLAG_COMPONENTSETUP) && !forced)
        return;

    for(int i = 0; i < MAX_CACHED_STATS; ++i)
        ResetCachedPlayerStat(pnum, i);

    // don't repeat on each player join
    PlayerStatCache[pnum][0].flags |= DND_STATCACHEFLAG_COMPONENTSETUP;
}

// ---------------------------------------------------------------------------
//  RECOMPUTE -- only runs when DIRTY, never on the per-shot path
// ---------------------------------------------------------------------------

// Folds "increased" and every "more" factor into one normalized multiplier.
// O(mult_count); called on gear/buff change, not per shot.
void RecomputeCachedPlayerStatNet(int pnum, int id) {
    auto stat = PlayerStatCache[pnum][id];
    auto comp = stat.components;

    int total = 100 + comp.increased;

    // an increased total at or below 0 is a hard zero, not a sign flip
    if(total <= 0) {
        comp.net_mant = 0;
        comp.net_exp = 0;
        stat.flags |= DND_STATCACHEFLAG_ZEROED;
        stat.flags &= ~DND_STATCACHEFLAG_DIRTY;
        return;
    }

    // seed the mantissa from the additive layer at FULL precision by dividing at
    // the right scale, rather than building total/100 and shifting it down after
    // (shifting down would throw away low bits that FixedDiv keeps).
    int num = total;
    int den = 100;
    int e = 0;

    while(num >= 2 * den) {
        den <<= 1;
        ++e;
    }
    while(num < den) {
        num <<= 1;      // num < den <= 100 here, so this cannot overflow
        --e;
    }

    int mant = FixedDiv(num, den);  // num/den is in [1.0, 2.0), far from FixedDiv's saturation band

    int i, f, fe;
    for(i = 0; i < comp.mult_count; ++i) {
        f = comp.mult[i].factor;

        // a x0 (or malformed negative) factor zeroes the whole product
        if(f <= 0) {
            comp.net_mant = 0;
            comp.net_exp = 0;
            stat.flags |= DND_STATCACHEFLAG_ZEROED;
            stat.flags &= ~DND_STATCACHEFLAG_DIRTY;
            return;
        }

        // normalize this factor into [1.0, 2.0).
        // shifting UP is exact, so shrinking factors lose nothing at all here --
        // this is what stops a stack of "less" multipliers from collapsing to 0.
        fe = 0;
        while(f >= 2.0) {
            f = (f + 1) >> 1;
            ++fe;
        }
        while(f < 1.0) {
            f <<= 1;
            --fe;
        }

        // both operands in [1.0, 2.0) => product in [1.0, 4.0): cannot overflow
        mant = FixedMulRound(mant, f);
        e += fe;

        if(mant >= 2.0) {
            mant = (mant + 1) >> 1;
            ++e;
        }
    }

    comp.net_mant = mant;
    comp.net_exp = e;

    stat.flags &= ~(DND_STATCACHEFLAG_DIRTY | DND_STATCACHEFLAG_ZEROED);
}

// ---------------------------------------------------------------------------
//  THE PER-SHOT PATH
// ---------------------------------------------------------------------------

// Applies the cached transform to THIS shot's rolled base damage.
// Hot path cost when clean: 1 branch + 1 add (+1 mul/div if flat_eff != 100),
// then 1 FixedMul + 1 shift.
int ApplyCachedPlayerStatToBase(int pnum, int id, int base) {
    auto stat = PlayerStatCache[pnum][id];

    // 1. flat bonuses, scaled by this weapon's flat effectiveness
    if(stat.flat) {
        if(stat.flat_eff != 100)
            base += stat.flat * stat.flat_eff / 100;
        else
            base += stat.flat;
    }

    if(base <= 0)
        return 0;

    auto comp = stat.components;
    if(!comp)
        return base;                // flat-only stat, no transform to apply

    if(stat.flags & DND_STATCACHEFLAG_DIRTY)
        RecomputeCachedPlayerStatNet(pnum, id);

    // 2. the single normalized multiply carrying both "increased" and every "more"
    return ApplyNormalizedMultiplier(base, comp.net_mant, comp.net_exp);
}

// ---------------------------------------------------------------------------
//  MUTATION -- flat layer (exactly reversible, does not dirty the multiplier)
// ---------------------------------------------------------------------------

// can be used to remove damage with -val, as flat damage doesn't have factors
void InsertFlatValueToCachedPlayerStat(int pnum, int id, int val) {
    PlayerStatCache[pnum][id].flat += val;
}

void SetCachedPlayerStatFlatEfficiency(int pnum, int id, int pct) {
    PlayerStatCache[pnum][id].flat_eff = pct;
}

// ---------------------------------------------------------------------------
//  MUTATION -- additive "increased" layer (exactly reversible)
// ---------------------------------------------------------------------------

// Every mutator below must go through this. `components` is deliberately null
// for flat-only stats, so dereferencing it unguarded is a crash on exactly the
// stat the null optimization exists for. Lazily allocates so a stat that starts
// flat-only but later receives a factor still works.
stat_component_T? EnsureCachedStatComponents(int pnum, int id) {
    auto stat = PlayerStatCache[pnum][id];

    if(!stat.components) {
        if(!IsComponentUsingCachedStat(id)) {
            //Log(s:"DnD: stat ", d:id, s:" is flat-only, factor insert ignored");
            return null;
        }

        stat.components = bcs::malloc(STAT_COMPONENT_SIZE);

        auto comp = stat.components;
        comp.increased = 0;
        comp.mult_count = 0;
        comp.net_mant = 1.0;
        comp.net_exp = 0;
    }

    return stat.components;
}

void InsertAdditiveFactorToCachedPlayerStat(int pnum, int id, int val) {
    auto comp = EnsureCachedStatComponents(pnum, id);
    if(!comp)
        return;

    comp.increased += val;
    PlayerStatCache[pnum][id].flags |= DND_STATCACHEFLAG_DIRTY;
}

void RemoveAdditiveFactorFromCachedPlayerStat(int pnum, int id, int val) {
    auto comp = EnsureCachedStatComponents(pnum, id);
    if(!comp)
        return;

    comp.increased -= val;
    PlayerStatCache[pnum][id].flags |= DND_STATCACHEFLAG_DIRTY;
}

// ---------------------------------------------------------------------------
//  MUTATION -- multiplicative "more" layer (NOT reversible, recomputed)
// ---------------------------------------------------------------------------

// val is a 16.16 ABSOLUTE multiplier: 1.0 == no change, 1.25 == +25% more.
// source is an identity token (tid, item id, buff table index, ...) so that two
// sources contributing the SAME numeric factor stay distinguishable on removal.
// Returns false if the factor could NOT be stored, so the caller never issues a
// matching Remove() for a factor that was silently dropped -- that would delete
// a different live source's entry and cause permanent, invisible damage loss.
bool InsertMultiplicativeFactorToCachedPlayerStat(int pnum, int id, int source, int val) {
    auto comp = EnsureCachedStatComponents(pnum, id);
    if(!comp)
        return false;

    if(comp.mult_count >= MAX_MULTIPLICATIVE_FACTORS) {
        // never drop silently -- a dropped factor is an invisible damage loss
        Log(s:"DnD: stat ", d:id, s:" multiplier list full, dropped factor ", f:val);
        return false;
    }

    comp.mult[comp.mult_count].source = source;
    comp.mult[comp.mult_count].factor = val;
    ++comp.mult_count;

    PlayerStatCache[pnum][id].flags |= DND_STATCACHEFLAG_DIRTY;
    return true;
}

// Removes the first entry owned by source and shifts the tail left.
//
// Order is preserved deliberately: the normalized product is very slightly
// order-dependent (measured worst spread 5.5e-5 over permutations of 20
// factors), so keeping insertion order makes recompute deterministic and makes
// a remove-then-reinsert cycle reproduce the previous value bit-for-bit.
void RemoveMultiplicativeFactorFromCachedPlayerStat(int pnum, int id, int source) {
    auto comp = PlayerStatCache[pnum][id].components;
    if(!comp)
        return;

    for(int i = 0; i < comp.mult_count; ++i) {
        if(comp.mult[i].source == source) {
            for(int j = i + 1; j < comp.mult_count; ++j) {
                comp.mult[j - 1].source = comp.mult[j].source;
                comp.mult[j - 1].factor = comp.mult[j].factor;
            }

            --comp.mult_count;
            comp.mult[comp.mult_count].source = 0;
            comp.mult[comp.mult_count].factor = 0;

            PlayerStatCache[pnum][id].flags |= DND_STATCACHEFLAG_DIRTY;
            return;
        }
    }
}

// ---------------------------------------------------------------------------
//  READBACK FOR UI
// ---------------------------------------------------------------------------

// Total multiplier as hundredths of a percent (10000 == 1.00x == "100.00%").
// Saturates rather than wrapping; use GetCachedPlayerStatExponent when the true
// magnitude is past what an int can show.
int GetCachedPlayerStatPercent(int pnum, int id) {
    auto stat = PlayerStatCache[pnum][id];
    auto comp = stat.components;

    if(!comp)
        return 10000;

    if(stat.flags & DND_STATCACHEFLAG_DIRTY)
        RecomputeCachedPlayerStatNet(pnum, id);

    return ApplyNormalizedMultiplier(10000, comp.net_mant, comp.net_exp);
}

// The raw normalized pair, for displaying magnitudes that overflow an int.
int GetCachedPlayerStatMantissa(int pnum, int id) {
    auto comp = PlayerStatCache[pnum][id].components;
    if(!comp)
        return 1.0;
    return comp.net_mant;
}

int GetCachedPlayerStatExponent(int pnum, int id) {
    auto comp = PlayerStatCache[pnum][id].components;
    if(!comp)
        return 0;
    return comp.net_exp;
}

// ---------------------------------------------------------------------------
//  ONE-TIME ENGINE PROBE
//
//  This whole design rests on FixedMul using a 64-bit intermediate. That is a
//  property of the ZDoom/Zandronum VM, not of BCS, so verify it once on the
//  target build rather than trusting it:
//
//      MUST log 15258789.  Anything else (a 32-bit intermediate wraps this to
//      -11099) means ApplyNormalizedMultiplier needs a hi/lo decomposition --
//      nothing else in this file changes.
// ---------------------------------------------------------------------------
void ProbeFixedMulWidth() {
    Log(s:"DnD FixedMul probe (expect 15258789): ", d:FixedMul(1000000, 1000000));
}

// ---------------------------------------------------------------------------
//  THE BUFF DAMAGE TRANSFORM
//
//  A per-player transform derived PURELY from pbuffs[pnum].buff_net_values.
//  That purity is the whole point:
//    - the client already mirrors buff_net_values, so it derives the identical
//      transform with no new sync traffic and no way to drift;
//    - nothing here reads gear or inventory, so it cannot go stale against an
//      equipment change the way a mixed derivation would.
//
//  It has its OWN dirty bit and never touches norecalculate[][]. Buff churn --
//  the ticker runs every 5 tics -- must not invalidate the per-weapon cache, or
//  a ~816-slot sweep would run several times a second mid-fight.
// ---------------------------------------------------------------------------

typedef struct {
    int inc_pct;        // integer percent DELTA; joins the weapon layer's inc_sum
    int mul_packed;     // packed normalized multiplier, applies to ALL damage

    // Per-element multipliers (sigil element powers). Kept OUT of mul_packed because
    // they only apply when the attack matches the element -- and deliberately not
    // cached per weapon, because damage_type is not pinned by (wepid, dmgid): special
    // ammo swaps a slot's element, so a cached copy would freeze whichever element
    // fired first.
    int ele_packed[DND_ELEMENTAL_DMGBUFF_COUNT];
} buff_dmg_xform_T;

buff_dmg_xform_T module& GetPlayerBuffDamageXform(int pnum) {
    static buff_dmg_xform_T xform[MAXPLAYERS];
    return xform[pnum];
}

// 1.0 is the neutral sentinel for buff_net_values.multiplicative everywhere else in the buff system
// -- ResetPlayerBuffs writes it, and the "is this buff doing anything" tests compare against it. So a
// non-positive value here is an UNWRITTEN slot, not a x0. Folding one as-is yields
// DND_PACKED_MULT_ZERO, ApplyPackedMultiplier turns that into a hard zero on the WHOLE tic, and the
// heal back in "DnD Damage Accumulate" then refunds every point of damage the player dealt.
int SafeBuffFactor(int factor) {
    return factor > 0 ? factor : 1.0;
}

void RecomputePlayerBuffDamageXform(int pnum) {
    buff_dmg_xform_T module& x = GetPlayerBuffDamageXform(pnum);

    // Sulphur is summed here rather than being copied into BUFF_DAMAGEDEALT's slot
    // by the buff system. Deriving it means it cannot leak; BUFF_SULPHUR also stays
    // readable on its own for the monster-rip check in DnD_CommonStat.h.
    x.inc_pct = FixedToPercent(pbuffs[pnum].buff_net_values[BUFF_DAMAGEDEALT].additive +
                               pbuffs[pnum].buff_net_values[BUFF_SULPHUR].additive);

    int p = DND_PACKED_MULT_IDENTITY;
    p = CombinePackedMultiplier(p, SafeBuffFactor(pbuffs[pnum].buff_net_values[BUFF_DAMAGEDEALT].multiplicative));
    p = CombinePackedMultiplier(p, SafeBuffFactor(pbuffs[pnum].buff_net_values[BUFF_FRENZYCHARGE].multiplicative));
    p = CombinePackedMultiplier(p, SafeBuffFactor(pbuffs[pnum].buff_net_values[BUFF_RALLY].multiplicative));
    x.mul_packed = p;

    for(int i = 0; i < DND_ELEMENTAL_DMGBUFF_COUNT; ++i)
        x.ele_packed[i] = CombinePackedMultiplier(DND_PACKED_MULT_IDENTITY, SafeBuffFactor(pbuffs[pnum].buff_net_values[DND_FIRST_ELEMENTAL_DMGBUFF + i].multiplicative));

    pbuffs[pnum].dmg_xform_dirty = false;
}

// The packed-zero tests are self-initialisation backstops, not decoration: the static array starts
// zeroed and a packed 0 means "hard zero", so without them a player deals no damage until their first
// buff event. ele_packed needs the same test as mul_packed -- a zeroed elemental entry is a permanent
// x0 on that element and nothing else would ever clear it.
bool PlayerBuffDamageXformNeedsRecompute(int pnum) {
    if(pbuffs[pnum].dmg_xform_dirty || !GetPlayerBuffDamageXform(pnum).mul_packed)
        return true;

    for(int i = 0; i < DND_ELEMENTAL_DMGBUFF_COUNT; ++i) {
        if(!GetPlayerBuffDamageXform(pnum).ele_packed[i])
            return true;
    }

    return false;
}

int GetPlayerBuffIncreasedDamage(int pnum) {
    if(PlayerBuffDamageXformNeedsRecompute(pnum))
        RecomputePlayerBuffDamageXform(pnum);
    return GetPlayerBuffDamageXform(pnum).inc_pct;
}

int GetPlayerBuffMoreDamagePacked(int pnum) {
    if(PlayerBuffDamageXformNeedsRecompute(pnum))
        RecomputePlayerBuffDamageXform(pnum);
    return GetPlayerBuffDamageXform(pnum).mul_packed;
}

// ele_idx is (damage_category - DND_ELECATEGORY_BEGIN); returns the identity for a
// non-elemental attack so the caller can multiply unconditionally.
int GetPlayerBuffElementalMorePacked(int pnum, int ele_idx) {
    if(ele_idx < 0 || ele_idx >= DND_ELEMENTAL_DMGBUFF_COUNT)
        return DND_PACKED_MULT_IDENTITY;

    if(PlayerBuffDamageXformNeedsRecompute(pnum))
        RecomputePlayerBuffDamageXform(pnum);

    return GetPlayerBuffDamageXform(pnum).ele_packed[ele_idx];
}

// true when no sigil element power is active -- replaces HasNoSigilPower()'s four
// CheckInventory calls with four array reads
bool PlayerHasNoElementalDamageBuff(int pnum) {
    for(int i = 0; i < DND_ELEMENTAL_DMGBUFF_COUNT; ++i) {
        if(pbuffs[pnum].buff_net_values[DND_FIRST_ELEMENTAL_DMGBUFF + i].multiplicative != 1.0)
            return false;
    }
    return true;
}

#endif
