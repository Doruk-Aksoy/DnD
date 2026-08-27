#ifndef DND_MODTIERS_IN
#define DND_MODTIERS_IN

// Per-mod tier ladders: how many tiers a mod has, what item level each one needs, how often it
// sticks. Profiles are interned, most mods share a handful of shapes.

#define DND_MAX_MOD_TIERS 12                    // item level tops out at MAX_BOSS_ILVL
#define DND_MAX_TIER_PROFILES 32
#define DND_TIER_WEIGHT_MAX 1000                // same scale as ItemModTable[].weight

enum {
    DND_TIERSET_COMMON,                         // must stay 0 -- the fallback for unassigned mods
    DND_TIERSET_FLAT,                           // this never scales, it's for pure word labels or unscaling factor mods
    DND_TIERSET_OFFENSE_NORMAL,                 // normal offsense, 8 tiers and not gated by boss
    DND_TIERSET_OFFENSE_STRONG,                 // 6 tiers last tier is boss only and rare
    DND_TIERSET_DEFENSE_NORMAL,                 // 8 tiers
    DND_TIERSET_DEFENSE_STRONG,                 // 7 tiers, last one is boss drop only
    DND_TIERSET_VERYSTRONG,                     // 6 tiers, sharp scaling, last one is boss only
    DND_TIERSET_COMMON_LASTFROMBOSS,            // currently what exists but last tier comes from boss only              
    DND_TIER_PROFILE_COUNT
};

typedef struct {
    int count;
    int req[DND_MAX_MOD_TIERS];                 // min item level for this tier
    int weight[DND_MAX_MOD_TIERS];              // 0..DND_TIER_WEIGHT_MAX, chance the tier sticks
} tier_profile_T;

typedef struct {
    int profile_of[UNIQUE_ATTRIB_ID_BEGIN];     // mod id -> profile, 0 unless assigned
    tier_profile_T profiles[DND_MAX_TIER_PROFILES];
} mod_tier_data_T;

global mod_tier_data_T 43: ModTierData;

// A tier past the ladder's own count is an authoring error. It used to land in dead storage and
// leave the step it was MEANT for at its old weight, silently -- which is how three inverted ladders
// shipped. Logged like CreateAliasTable's size guard, because silent corruption was the failure mode.
void SetTierStep(int profile, int tier, int req = -1, int weight = DND_TIER_WEIGHT_MAX) {
    if(profile < 0 || profile >= DND_MAX_TIER_PROFILES) {
        Log(s:"SetTierStep: profile ", d:profile, s:" is outside 0..", d:DND_MAX_TIER_PROFILES - 1, s:"!");
        return;
    }

    if(tier < 0 || tier >= ModTierData.profiles[profile].count) {
        Log(s:"SetTierStep: profile ", d:profile, s:" has ", d:ModTierData.profiles[profile].count,
            s:" tiers, cannot set tier ", d:tier, s:"! Define the profile first, and index 0..count-1.");
        return;
    }

    // req is optional -- -1 keeps whatever DefineTierProfileEven laid down
    if(req != -1)
        ModTierData.profiles[profile].req[tier] = req;
    ModTierData.profiles[profile].weight[tier] = weight;
}

// Even spread of requirements at a flat weight. SetTierStep overrides individual steps after.
void DefineTierProfileEven(int profile, int count, int first_req, int last_req, int weight) {
    ModTierData.profiles[profile].count = count;
    for(int i = 0; i < count; ++i)
        SetTierStep(profile, i, count > 1 ? first_req + (last_req - first_req) * i / (count - 1) : first_req, weight);
}

// profile_of sits immediately before profiles[] in the struct, so an out of range mod id writes
// straight onto profiles[0].count and silently corrupts the COMMON ladder. Unique mod ids are the
// ones that reach past the end -- they have no tiers and must never be assigned one.
void SetModTierProfile(int attr, int profile) {
    if(attr < 0 || attr >= UNIQUE_ATTRIB_ID_BEGIN) {
        Log(s:"SetModTierProfile: mod id ", d:attr, s:" is outside the tierable range 0..",
            d:UNIQUE_ATTRIB_ID_BEGIN - 1, s:"!");
        return;
    }

    if(profile < 0 || profile >= DND_MAX_TIER_PROFILES) {
        Log(s:"SetModTierProfile: profile ", d:profile, s:" is outside 0..", d:DND_MAX_TIER_PROFILES - 1, s:"!");
        return;
    }

    ModTierData.profile_of[attr] = profile;
}

// Profile 0 is GetItemTier's ladder exactly, built off the same constant so the two cannot drift.
// Globals start zeroed and DND_TIERSET_COMMON is 0, so every unassigned mod already points here.
void SetupModTierProfiles() {
    // count first -- SetTierStep now bounds-checks against it
    ModTierData.profiles[DND_TIERSET_COMMON].count = MAX_CHARM_AFFIXTIERS + 1;
    for(int i = 0; i <= MAX_CHARM_AFFIXTIERS; ++i)
        SetTierStep(DND_TIERSET_COMMON, i, i * CHARM_ATTRIBLEVEL_SEPERATOR, DND_TIER_WEIGHT_MAX);

    DefineTierProfileEven(DND_TIERSET_FLAT, 1, 1, 1, DND_TIER_WEIGHT_MAX);

    DefineTierProfileEven(DND_TIERSET_OFFENSE_NORMAL, 8, 1, 83, DND_TIER_WEIGHT_MAX);
    SetTierStep(DND_TIERSET_OFFENSE_NORMAL, 5, -1, 300);
    SetTierStep(DND_TIERSET_OFFENSE_NORMAL, 6, -1, 200);
    SetTierStep(DND_TIERSET_OFFENSE_NORMAL, 7, -1, 100);

    DefineTierProfileEven(DND_TIERSET_OFFENSE_STRONG, 6, 20, 85, DND_TIER_WEIGHT_MAX);
    SetTierStep(DND_TIERSET_OFFENSE_STRONG, 3, -1, 400);
    SetTierStep(DND_TIERSET_OFFENSE_STRONG, 4, -1, 250);
    SetTierStep(DND_TIERSET_OFFENSE_STRONG, 5, -1, 75);

    DefineTierProfileEven(DND_TIERSET_DEFENSE_NORMAL, 8, 1, 83, DND_TIER_WEIGHT_MAX);
    SetTierStep(DND_TIERSET_DEFENSE_NORMAL, 5, -1, 400);
    SetTierStep(DND_TIERSET_DEFENSE_NORMAL, 6, -1, 300);
    SetTierStep(DND_TIERSET_DEFENSE_NORMAL, 7, -1, 200);

    DefineTierProfileEven(DND_TIERSET_DEFENSE_STRONG, 7, 10, 85, DND_TIER_WEIGHT_MAX);
    SetTierStep(DND_TIERSET_DEFENSE_STRONG, 4, -1, 400);
    SetTierStep(DND_TIERSET_DEFENSE_STRONG, 5, -1, 250);
    SetTierStep(DND_TIERSET_DEFENSE_STRONG, 6, -1, 75);

    DefineTierProfileEven(DND_TIERSET_VERYSTRONG, 6, 25, 88, DND_TIER_WEIGHT_MAX);
    SetTierStep(DND_TIERSET_VERYSTRONG, 5, -1, 75);

    DefineTierProfileEven(DND_TIERSET_COMMON_LASTFROMBOSS, 10, 1, 86, DND_TIER_WEIGHT_MAX);
    SetTierStep(DND_TIERSET_COMMON_LASTFROMBOSS, 7, -1, 300);
    SetTierStep(DND_TIERSET_COMMON_LASTFROMBOSS, 8, -1, 200);
    SetTierStep(DND_TIERSET_COMMON_LASTFROMBOSS, 9, -1, 100);
}

// Dungeon key mod ids collide with INV_ ids, so they may never index profile_of and stay on the
// common ladder until keys get ladders of their own.
int GetModTierProfile(int attr, int item_type) {
    if(attr < 0 || attr >= UNIQUE_ATTRIB_ID_BEGIN || HasSeparateModIdSpace(item_type))
        return DND_TIERSET_COMMON;
    return ModTierData.profile_of[attr];
}

int GetModTierCount(int attr, int item_type) {
    return ModTierData.profiles[GetModTierProfile(attr, item_type)].count;
}

// Clamped: assimilation can hand over a donor tier the host's ladder does not have.
int GetModTierRequirement(int attr, int item_type, int tier) {
    int p = GetModTierProfile(attr, item_type);
    return ModTierData.profiles[p].req[Clamp_Between(tier, 0, ModTierData.profiles[p].count - 1)];
}

int GetModTierWeight(int attr, int item_type, int tier) {
    int p = GetModTierProfile(attr, item_type);
    return ModTierData.profiles[p].weight[Clamp_Between(tier, 0, ModTierData.profiles[p].count - 1)];
}

// Highest tier this item level affords, -1 when the mod cannot exist on it at all.
int GetModTierForLevel(int attr, int item_type, int ilvl) {
    int p = GetModTierProfile(attr, item_type);
    for(int i = ModTierData.profiles[p].count - 1; i >= 0; --i)
        if(ModTierData.profiles[p].req[i] <= ilvl)
            return i;
    return -1;
}

// The mod's first tier is the only one that decides whether it may appear on an item at all.
bool CanModRollAtLevel(int attr, int item_type, int ilvl) {
    return GetModTierRequirement(attr, item_type, 0) <= ilvl;
}

// A saved tier can outlive the ladder it was rolled on: shortening a profile leaves old items
// carrying an index their mod no longer has, and GetModTierText would render it as a NEGATIVE tier.
// Clamp on the way in from the database. The VALUE is never recomputed, so a legacy over-roll simply
// reads as a very well rolled T1.
int SanitizeModTier(int attr, int item_type, int tier) {
    return Clamp_Between(tier, 0, GetModTierCount(attr, item_type) - 1);
}

// A mod's tier on the common 0..MAX_CHARM_AFFIXTIERS ladder. The ONLY way to compare or average
// tiers of mods whose ladders differ -- T3 of 5 is near perfect, T3 of 11 is junk.
int GetModTierNormalized(int attr, int item_type, int tier) {
    int c = GetModTierCount(attr, item_type);
    if(c < 2)
        return MAX_CHARM_AFFIXTIERS;
    return Clamp_Between(tier, 0, c - 1) * MAX_CHARM_AFFIXTIERS / (c - 1);
}

// Charm_Strings is a 10 entry colour ramp, not a tier count. A one tier mod never scaled, so it
// takes the dullest rather than the ramp's top.
int GetModTierColourIndex(int attr, int item_type, int tier) {
    if(GetModTierCount(attr, item_type) < 2)
        return 0;
    return Clamp_Between(GetModTierNormalized(attr, item_type, tier), 0, MAX_CHARM_AFFIXTIERS - 1);
}

// Potency may push one step past what the item level affords -- the last step is earned.
int GetModPotencyCeiling(int attr, int item_type, int ilvl) {
    int top = GetModTierCount(attr, item_type) - 1;
    int nat = GetModTierForLevel(attr, item_type, ilvl) + 1;
    return nat < top ? nat : top;
}

// The +1/-N shuffle around what the item level grants. Same numbers as the old GetItemTierRoll; the
// only change is that the ceiling is the mod's own ladder instead of a constant.
int RollModTier(int attr, int item_type, int lvl, bool isWellRolled) {
    bool revered = CheckInventory("ReveranceUsed");

    // 10% chance to roll a tier up -- if well rolled 20%
    if(!random(0, 9 - 5 * isWellRolled) || revered)
        ++lvl;
    else // 0-1 do nothing, 2-3 is -1, 4-5 is -2 => well rolled has only 33% chance to downgrade
        lvl -= random(0, 9 - 3 * isWellRolled) / 2;

    lvl = Clamp_Between(lvl, 0, GetModTierCount(attr, item_type) - 1);

    // A rare tier has to earn its place and drops one step when it does not, so the walk compounds:
    // weights 100/400/1000 on the top three give 10% / 36% / 54%. Tier 0 always sticks. Reverance
    // is a guarantee item, so it skips the walk rather than having its push undone.
    while(lvl > 0 && !revered && random(1, DND_TIER_WEIGHT_MAX) > GetModTierWeight(attr, item_type, lvl))
        --lvl;

    return lvl;
}

#endif
