#ifndef DND_STATCACHE_IN
#define DND_STATCACHE_IN

// Normalized and packed 16.16 multipliers, and the per-player buff damage transform built from them.

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
