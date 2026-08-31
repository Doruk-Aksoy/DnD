#ifndef DND_PERKS_IN
#define DND_PERKS_IN

#define DND_PERK_MULTIPLES 3

// The perk tree. Definitions are static shared data built in both OPEN scripts the way
// ItemModTable is; the points a player has spent live in PlayerModData, which already has exactly
// this lifetime and is already mirrored clientside for the menu to read.
//
// Sizing constants are in DnD_Common.h, NOT here -- player_item_mod_data_T is declared before this
// file is reached and macro forward references do not work in bcs.

enum {
    PERK_ARCH_ACRO,
    PERK_ARCH_ASSN,
    PERK_ARCH_CUN,
    PERK_ARCH_END,
    PERK_ARCH_MART,
    PERK_ARCH_PERC,
    PERK_ARCH_TORM,

    PERK_ARCH_COUNT
};

// A perk BELONGS to a tag; another perk REQUIRES a point in anything carrying it. One tag today.
#define PERKTAG_CHARGE 0b1

// PERK_REQ_ALL needs every listed perk, PERK_REQ_ANY needs one of them, PERK_REQ_TAG needs a
// point in any perk carrying req_tag. The threshold is checked separately and always applies.
enum { PERK_REQ_NONE, PERK_REQ_ALL, PERK_REQ_ANY, PERK_REQ_TAG };

// Set by phase 3 as each perk is wired to a stat.
#define PERKF_AFFECTSDAMAGE 0b1     // spending re-runs the damage cache
#define PERKF_BESPOKE       0b10    // no pstat, the behaviour reads HasPerk at its own call site

// TWO stat slots, not one. Several perks give two numbers at once -- Unwavering Stance is
// "200 knockback resist AND 5% health" per point -- and splitting those into two perk entries would
// lie about the tree's shape to both the menu and the point economy.
#define DND_PERK_STATSLOTS 2

typedef struct {
    int archetype, category;
    int max_points;
    int threshold;                      // min points in the ARCHETYPE, 0 = ungated
    int req_kind;
    int req_perk[DND_MAX_PERK_REQS];
    int req_points[DND_MAX_PERK_REQS];
    int tags;                           // PERKTAG_* this perk carries
    int req_tag;                        // PERKTAG_* this perk needs a point in
    int pstat[DND_PERK_STATSLOTS];      // PSTAT_* this slot feeds, -1 for an unused slot

    // The notes are NOT linear in points and assuming they were is wrong for a large part of the
    // tree. Enhanced Iron is "10% armor, +10% per point" -- 10/20/30, where first == extra. Augmented
    // Footpads is "2% movement speed, +1% per point" -- 2/3/4, where they differ. Value at N points
    // is first_point + (N-1) * per_extra, so both shapes are the same two numbers.
    int first_point[DND_PERK_STATSLOTS];
    int per_extra[DND_PERK_STATSLOTS];
    int pflag;                          // PFLAG_* held while the perk has any point, -1 for none
    int flags;
} perk_def_T;

global perk_def_T 44: PerkTable[DND_MAX_PERKS_NEW];

enum {
    // ---- Acrobacy: 14 perks, 36 points ----
    PERK_ACRO_TACTICALDASH,
    PERK_ACRO_KINETICPADS,
    PERK_ACRO_SWIFTREFLEXES,
    PERK_ACRO_ENHANCEDJOINTS,
    PERK_ACRO_EVASIVEMANEUVERS,
    PERK_ACRO_CRASHCOURSE,
    PERK_ACRO_UNENDINGRUSH,

    PERK_ACRO_THUMPER,
    PERK_ACRO_HEADSTART,
    PERK_ACRO_ALLSHAKINGPRESENCE,
    PERK_ACRO_ADRENALINE,

    PERK_ACRO_AUGMENTEDFOOTPADS,
    PERK_ACRO_TAILWIND,
    PERK_ACRO_NIMBLENESS,

    // ---- Assassination: 14 perks, 34 points ----
    PERK_ASSN_BACKSTAB,
    PERK_ASSN_DEADLINESS,
    PERK_ASSN_PREPARATION,
    PERK_ASSN_BIGGAMEHUNTER,
    PERK_ASSN_BANEOFLEGENDS,
    PERK_ASSN_OPENINGSALVO,
    PERK_ASSN_ERADICATION,

    PERK_ASSN_QUICKGETAWAY,
    PERK_ASSN_MASTEROFSHADOWS,
    PERK_ASSN_PLANNEDEXECUTION,
    PERK_ASSN_STEADYSHOT,
    PERK_ASSN_PRESSUREPOINTS,
    PERK_ASSN_MERCYKILL,
    PERK_ASSN_DANCEWITHDEATH,

    // ---- Cunning: 18 perks, 33 points ----
    PERK_CUN_POTENTSALVE,
    PERK_CUN_EFFICIENTRECYCLING,
    PERK_CUN_ENRICHEDMINERALS,
    PERK_CUN_SURGINGVITALITY,
    PERK_CUN_DESPERATEMEASURES,
    PERK_CUN_OVERFLOWINGRESERVES,
    PERK_CUN_SPIKEDCONCOCTION,

    PERK_CUN_DEEPWISDOM,
    PERK_CUN_ENDLESSGREED,

    PERK_CUN_ENDURINGCOMPOSURE,
    PERK_CUN_LASTINGFRENZY,
    PERK_CUN_ASTOUNDINGPOWER,
    PERK_CUN_DISCIPLEOFTHEUNYIELDING,
    PERK_CUN_DISCIPLEOFTHESLAUGHTER,
    PERK_CUN_DISCIPLEOFTHEFORBIDDEN,
    PERK_CUN_MASTEROFRESILIENCE,
    PERK_CUN_MASTEROFFEROCITY,
    PERK_CUN_MASTEROFTHEARCANE,

    // ---- Endurance: 16 perks, 38 points ----
    PERK_END_PHYSICIAN,
    PERK_END_UNWAVERINGSTANCE,
    PERK_END_MEDIC,
    PERK_END_VIGOUR,
    PERK_END_RESTLESSVITALITY,
    PERK_END_INDOMITABLERESOLVE,

    PERK_END_CAMOUFLAGE,
    PERK_END_DENSEEXOSKELETON,
    PERK_END_STONESKIN,
    PERK_END_CRYSTALSKIN,

    PERK_END_ENHANCEDIRON,
    PERK_END_FINESSE,
    PERK_END_SHIELDING,
    PERK_END_PERFECTSHELL,
    PERK_END_WINDDANCER,
    PERK_END_HIDDENRESERVE,

    // ---- Martialist: 16 perks, 32 points ----
    PERK_MART_RAMPINGASSAULT,
    PERK_MART_CRANIUMBASH,
    PERK_MART_ECHOINGSTRIKES,
    PERK_MART_UNENDINGFURY,
    PERK_MART_FERVENTREACH,
    PERK_MART_BLADEMASTER,

    PERK_MART_EXHAUSTER,
    PERK_MART_DEEPCUTS,
    PERK_MART_QUICKDEADLY,
    PERK_MART_SWIFTPRECISE,
    PERK_MART_FLESHCARVER,

    PERK_MART_FLASHPARRY,
    PERK_MART_RIPOSTE,
    PERK_MART_EXPOSEWEAKNESS,
    PERK_MART_MARTIALFINESSE,
    PERK_MART_GRATUITOUSVIOLENCE,

    // ---- Perception: 21 perks, 45 points ----
    PERK_PERC_MUNITIONIST,
    PERK_PERC_PLANB,
    PERK_PERC_EXCESSCONVERSION,
    PERK_PERC_FRESHCLIP,
    PERK_PERC_LUCKYBULLET,
    PERK_PERC_SALVAGER,
    PERK_PERC_SLEIGHTOFHAND,

    PERK_PERC_PRECISETECHNIQUE,
    PERK_PERC_FIREDUP,
    PERK_PERC_PUMPED,
    PERK_PERC_QUAKER,
    PERK_PERC_DARKARTS,
    PERK_PERC_SHARPSHOOTER,
    PERK_PERC_CEASELESSASSAULT,
    PERK_PERC_BLASTIERSHOTS,
    PERK_PERC_EARTHSHAKER,
    PERK_PERC_ESSENCETHEFT,

    PERK_PERC_BASTIONBREAKER,
    PERK_PERC_SHIELDSTEALER,
    PERK_PERC_UNSTOPPABLEFORCE,
    PERK_PERC_HEATSINKS,
    PERK_PERC_EMERGENCYPROTOCOL,

    // ---- Tormentor: 35 perks, 84 points ----
    // Five ailment lines of seven. Order here is the order the page lists them in.
    // poison
    PERK_TORM_STUDENTOFDECAY,
    PERK_TORM_PROLONGEDTORTURE,
    PERK_TORM_DEATHSGRIP,
    PERK_TORM_FLOWOFLIFE,
    PERK_TORM_CORROSION,
    PERK_TORM_ACRIMONY,
    PERK_TORM_SEPTICTOUCH,

    // cold
    PERK_TORM_BITTERFROST,
    PERK_TORM_LINGERINGCOLD,
    PERK_TORM_FLASHFREEZE,
    PERK_TORM_CRIPPLINGICE,
    PERK_TORM_BRINGEROFICE,
    PERK_TORM_BREATHOFRIME,
    PERK_TORM_PERMAFROST,

    // fire
    PERK_TORM_SLOWCOOKER,
    PERK_TORM_DEEPFRIED,
    PERK_TORM_BLOWBACK,
    PERK_TORM_COOKEDALIVE,
    PERK_TORM_WILDFIRE,
    PERK_TORM_CREMATOR,
    PERK_TORM_AVATAROFFIRE,

    // overload
    PERK_TORM_SHOCKER,
    PERK_TORM_COURSINGCURRENT,
    PERK_TORM_MAXIMUMOVERDRIVE,
    PERK_TORM_MUSCLESPASMS,
    PERK_TORM_POWERUP,
    PERK_TORM_JOLT,
    PERK_TORM_SUPERCONDUCTOR,

    // bleed
    PERK_TORM_WOUNDER,
    PERK_TORM_BLOODLETTING,
    PERK_TORM_SWIFTDRAIN,
    PERK_TORM_SAVAGEWOUNDS,
    PERK_TORM_CORNEREDPREY,
    PERK_TORM_MASTEROFWOUNDS,

    PERK_COUNT
};

#define PERK_ARCH_FIRST PERK_ARCH_ACRO

// ---------------------------------------------------------------- authoring
// Guarded the way SetTierStep is. A bad id here writes past PerkTable and there is no plausible way
// to notice at runtime, so it says so at load instead.

void DefinePerk(int id, int arch, int cat, int max_points, int threshold = 0, int tags = 0) {
    if(id < 0 || id >= DND_MAX_PERKS_NEW) {
        Log(s:"DefinePerk: perk id ", d:id, s:" is outside 0..", d:DND_MAX_PERKS_NEW - 1, s:"!");
        return;
    }

    if(arch < 0 || arch >= DND_MAX_PERK_ARCHETYPES) {
        Log(s:"DefinePerk: perk ", d:id, s:" has archetype ", d:arch, s:", outside 0..",
            d:DND_MAX_PERK_ARCHETYPES - 1, s:"!");
        return;
    }

    if(max_points < 1 || max_points > DND_PERK_MAXPOINTS) {
        Log(s:"DefinePerk: perk ", d:id, s:" wants ", d:max_points, s:" points, cap is ",
            d:DND_PERK_MAXPOINTS, s:"!");
        return;
    }

    PerkTable[id].archetype = arch;
    PerkTable[id].category = cat;
    PerkTable[id].max_points = max_points;
    PerkTable[id].threshold = threshold;
    PerkTable[id].tags = tags;
    PerkTable[id].req_kind = PERK_REQ_NONE;
    PerkTable[id].req_tag = 0;
    PerkTable[id].pflag = -1;
    PerkTable[id].flags = 0;

    int i;
    for(i = 0; i < DND_PERK_STATSLOTS; ++i) {
        PerkTable[id].pstat[i] = -1;
        PerkTable[id].first_point[i] = 0;
        PerkTable[id].per_extra[i] = 0;
    }

    for(i = 0; i < DND_MAX_PERK_REQS; ++i)
        PerkTable[id].req_perk[i] = -1;
}

// Up to DND_MAX_PERK_REQS named prerequisites. -1 in a slot ends the list.
void SetPerkReq(int id, int kind, int p1, int n1 = 1, int p2 = -1, int n2 = 1, int p3 = -1, int n3 = 1) {
    if(id < 0 || id >= DND_MAX_PERKS_NEW) {
        Log(s:"SetPerkReq: perk id ", d:id, s:" is outside 0..", d:DND_MAX_PERKS_NEW - 1, s:"!");
        return;
    }

    // max_points is 0 only when DefinePerk has not run. Catching that here is what stops a
    // requirement being attached to a perk that does not exist yet and then vanishing silently.
    if(!PerkTable[id].max_points) {
        Log(s:"SetPerkReq: perk ", d:id, s:" has no DefinePerk yet, requirement dropped!");
        return;
    }

    PerkTable[id].req_kind = kind;
    PerkTable[id].req_perk[0] = p1; PerkTable[id].req_points[0] = n1;
    PerkTable[id].req_perk[1] = p2; PerkTable[id].req_points[1] = n2;
    PerkTable[id].req_perk[2] = p3; PerkTable[id].req_points[2] = n3;
}

void SetPerkReqTag(int id, int tag) {
    if(id < 0 || id >= DND_MAX_PERKS_NEW || !PerkTable[id].max_points) {
        Log(s:"SetPerkReqTag: perk id ", d:id, s:" is invalid or undefined!");
        return;
    }

    PerkTable[id].req_kind = PERK_REQ_TAG;
    PerkTable[id].req_tag = tag;
}

// A perk's magnitude: what the FIRST point gives, then what EACH FURTHER point adds. Call twice
// for a perk that gives two numbers at once; the second call fills slot 1.
//
// per_extra defaults to first_point, which is the common "10%, +10%" shape. Pass it explicitly for
// the perks whose first point is worth more or less than the rest.
void SetPerkStat(int id, int pstat, int first_point, int per_extra = -0x7FFFFFFF, int flags = 0) {
    if(id < 0 || id >= DND_MAX_PERKS_NEW || !PerkTable[id].max_points) {
        Log(s:"SetPerkStat: perk id ", d:id, s:" is invalid or undefined!");
        return;
    }

    if(per_extra == -0x7FFFFFFF)
        per_extra = first_point;

    if(PerkTable[id].max_points == 1 && per_extra != first_point)
        Log(s:"SetPerkStat: perk ", d:id, s:" holds one point, so per_extra is unreachable.");

    for(int i = 0; i < DND_PERK_STATSLOTS; ++i) {
        if(PerkTable[id].pstat[i] == -1) {
            PerkTable[id].pstat[i] = pstat;
            PerkTable[id].first_point[i] = first_point;
            PerkTable[id].per_extra[i] = per_extra;
            PerkTable[id].flags |= flags;
            return;
        }
    }

    Log(s:"SetPerkStat: perk ", d:id, s:" already has ", d:DND_PERK_STATSLOTS, s:" stats!");
}

// A boolean capability, held while the perk has ANY point. Deliberately the same PFLAG_* space item
// mods use, through the same refcounted setter -- so a capability can come from an item or a perk or
// both, and every reader keeps asking one question. The refcount is what makes two sources correct.
void SetPerkFlag(int id, int pflag, int flags = 0) {
    if(id < 0 || id >= DND_MAX_PERKS_NEW || !PerkTable[id].max_points) {
        Log(s:"SetPerkFlag: perk id ", d:id, s:" is invalid or undefined!");
        return;
    }

    PerkTable[id].pflag = pflag;
    PerkTable[id].flags |= flags;
}

// ---------------------------------------------------------------- player state
// Two bits per perk, so DND_PERK_MAXPOINTS 3 is exactly what one lane holds. Raising the cap past 3
// means widening the lane, which is why the mask is written against the constant rather than 0b11.

// DND_PERK_BITS is in DnD_Common.h next to DND_PERK_WORDS, which it sizes.
#define DND_PERK_LANEMASK 0b11

int GetPerkPoints(int pnum, int perk) {
    if(perk < 0 || perk >= DND_MAX_PERKS_NEW)
        return 0;

    int bit = perk * DND_PERK_BITS;
    return (PlayerModData[pnum].perks_packed[bit >> 5] >> (bit & 31)) & DND_PERK_LANEMASK;
}

bool HasPerk(int pnum, int perk) {
    return GetPerkPoints(pnum, perk) > 0;
}

// The ONE writer. spent_in is a running total rather than a scan, so it has to move with the lane or
// every threshold in that archetype drifts -- and nothing reads the lanes back to cross check it.
void SetPerkPoints(int pnum, int perk, int pts) {
    if(perk < 0 || perk >= DND_MAX_PERKS_NEW)
        return;

    pts = Clamp_Between(pts, 0, PerkTable[perk].max_points);

    int prev = GetPerkPoints(pnum, perk);
    if(prev == pts)
        return;

    int bit = perk * DND_PERK_BITS;
    int word = bit >> 5;
    int shift = bit & 31;

    PlayerModData[pnum].perks_packed[word] =
        (PlayerModData[pnum].perks_packed[word] & ~(DND_PERK_LANEMASK << shift)) | (pts << shift);

    PlayerModData[pnum].spent_in[PerkTable[perk].archetype] += pts - prev;
}

int GetPerkSpentIn(int pnum, int arch) {
    if(arch < 0 || arch >= DND_MAX_PERK_ARCHETYPES)
        return 0;
    return PlayerModData[pnum].spent_in[arch];
}

// Rebuild the totals from the lanes. Only the database load needs this -- SetPerkPoints keeps them
// honest during play -- but a load writes lanes in bulk and a running total cannot survive that.
void RecountPerkPoints(int pnum) {
    int i;
    for(i = 0; i < DND_MAX_PERK_ARCHETYPES; ++i)
        PlayerModData[pnum].spent_in[i] = 0;

    for(i = 0; i < PERK_COUNT; ++i) {
        int pts = GetPerkPoints(pnum, i);
        if(pts && PerkTable[i].max_points)
            PlayerModData[pnum].spent_in[PerkTable[i].archetype] += pts;
    }
}

// ---------------------------------------------------------------- the unspent pool
// The pool stays on the PerkPoint item: level ups, the database row and the HUD already read it from
// the actor, and moving it here would leave three readers to chase. PlayerModData.unspent mirrors it
// for the clientside tree only, and every writer below refreshes the mirror from the item rather than
// tracking it separately -- one direction, so the two cannot drift.

// Reconciles the mirror against the item rather than being told what the item became. The
// difference matters: the item has writers this code does not own. `give PerkPoint 5` from the
// console is one of them, and it left the mirror at zero while the player plainly had five points,
// so the tree refused every spend. Anything that hands out points without going through
// GivePerkPoints has the same effect, and there is no way to make the console call a function.
//
// Cheap enough to call on a timer: one inventory read, and the sync only goes out when the number
// actually moved. force is for the full resync, where the client's copy has to be re-stated even
// when the server's has not changed -- the client may have been zeroed independently.
void RefreshUnspentPerkPoints(int pnum, bool force = false) {
    int real = CheckActorInventory(pnum + P_TIDSTART, "PerkPoint");
    if(!force && real == PlayerModData[pnum].unspent)
        return;

    PlayerModData[pnum].unspent = real;
    ACS_NamedExecuteWithResult("DnD Request Perk Point Sync", pnum, real);
}

// The activity delta is how the grant reaches the database -- SavePlayerActivities increments the
// stored row by it -- so a give that skips this is a point the character loses on its next save.
void GivePerkPoints(int pnum, int amt) {
    if(amt <= 0)
        return;

    GiveActorInventory(pnum + P_TIDSTART, "PerkPoint", amt);
    UpdateActivity(pnum, DND_ACTIVITY_PERKPOINT, amt, 0);
    RefreshUnspentPerkPoints(pnum);
}

void TakePerkPoints(int pnum, int amt) {
    if(amt <= 0)
        return;

    TakeActorInventory(pnum + P_TIDSTART, "PerkPoint", amt);
    UpdateActivity(pnum, DND_ACTIVITY_PERKPOINT, -amt, 0);
    RefreshUnspentPerkPoints(pnum);
}

// Push the one word a lane lives in. Deliberately NOT inside SetPerkPoints: the refund legality probe
// moves a point and puts it straight back, and syncing that would send two words for every frame the
// menu asks whether a refund is legal.
void SyncPerkWord(int pnum, int perk) {
    int word = (perk * DND_PERK_BITS) >> 5;
    ACS_NamedExecuteWithResult("DnD Request Perk Sync", pnum, word, PlayerModData[pnum].perks_packed[word]);
}

void ResetPlayerPerks(int pnum) {
    int i;
    for(i = 0; i < DND_PERK_WORDS; ++i)
        PlayerModData[pnum].perks_packed[i] = 0;
    for(i = 0; i < DND_MAX_PERK_ARCHETYPES; ++i)
        PlayerModData[pnum].spent_in[i] = 0;
    PlayerModData[pnum].unspent = 0;
}

// ---------------------------------------------------------------- gating
// Both halves always apply: the archetype threshold AND the Requires clause. A perk with neither is
// spendable from the first point.

bool IsPerkThresholdMet(int pnum, int perk) {
    return GetPerkSpentIn(pnum, PerkTable[perk].archetype) >= PerkTable[perk].threshold;
}

// Any point in any perk carrying the tag. Six Cunning perks gate this way on PERKTAG_CHARGE.
bool HasPointInPerkTag(int pnum, int tag) {
    for(int i = 0; i < PERK_COUNT; ++i)
        if((PerkTable[i].tags & tag) && GetPerkPoints(pnum, i))
            return true;
    return false;
}

bool IsPerkRequirementMet(int pnum, int perk) {
    int kind = PerkTable[perk].req_kind;
    if(kind == PERK_REQ_NONE)
        return true;

    if(kind == PERK_REQ_TAG)
        return HasPointInPerkTag(pnum, PerkTable[perk].req_tag);

    // ANY is settled by the first hit, ALL by the first miss. An empty slot is -1 and ends the list
    // either way, so a one-entry ALL and a one-entry ANY agree -- which is what makes the notes'
    // single-prerequisite form safe to record as ALL.
    for(int i = 0; i < DND_MAX_PERK_REQS; ++i) {
        int r = PerkTable[perk].req_perk[i];
        if(r == -1)
            break;

        bool has = GetPerkPoints(pnum, r) >= PerkTable[perk].req_points[i];
        if(kind == PERK_REQ_ANY) {
            if(has)
                return true;
        }
        else if(!has)
            return false;
    }

    return kind == PERK_REQ_ALL;
}

bool IsPerkUnlocked(int pnum, int perk) {
    if(perk < 0 || perk >= DND_MAX_PERKS_NEW || !PerkTable[perk].max_points)
        return false;
    return IsPerkThresholdMet(pnum, perk) && IsPerkRequirementMet(pnum, perk);
}

bool CanSpendOnPerk(int pnum, int perk) {
    if(perk < 0 || perk >= DND_MAX_PERKS_NEW || !PerkTable[perk].max_points)
        return false;
    if(GetPerkPoints(pnum, perk) >= PerkTable[perk].max_points)
        return false;
    if(!PlayerModData[pnum].unspent)
        return false;
    return IsPerkUnlocked(pnum, perk);
}

// Which perk a refund would break, or -1 when the refund is legal.
//
// Two ways to break it, and the second is the one that is easy to miss: dropping a point can push
// spent_in below a threshold, invalidating perks that never named this one. So the test is on the
// RESULTING state of every perk still holding points, not on this perk's named dependents.
//
// It moves the point, looks, and puts it back. That is why SetPerkPoints has to be the only writer
// -- anything caching off the lanes would see the probe.
int GetPerkRefundBlocker(int pnum, int perk) {
    if(perk < 0 || perk >= DND_MAX_PERKS_NEW || !GetPerkPoints(pnum, perk))
        return perk;

    int prev = GetPerkPoints(pnum, perk);
    SetPerkPoints(pnum, perk, prev - 1);

    int blocker = -1;
    for(int i = 0; i < PERK_COUNT && blocker == -1; ++i)
        if(i != perk && GetPerkPoints(pnum, i) && !IsPerkUnlocked(pnum, i))
            blocker = i;

    SetPerkPoints(pnum, perk, prev);
    return blocker;
}

bool CanRefundPerkPoint(int pnum, int perk) {
    return GetPerkRefundBlocker(pnum, perk) == -1;
}

// ---------------------------------------------------------------- applying to stats
// Perks fold into PlayerModData the way item mods do, so nothing on the damage path has to know a
// perk exists. The only difference is that a perk writes its PSTAT slot directly rather than going
// through the attribute-id mapper -- it has no attribute id to map.

void AddPerkStatValue(int pnum, int pstat, int delta) {
    if(pstat < 0 || pstat >= PSTAT_COUNT || !delta)
        return;

    PlayerModData[pnum].vals[pstat] += delta;
    ACS_NamedExecuteWithResult("DnD Request Stat Sync", pnum, pstat, PlayerModData[pnum].vals[pstat]);
}

int GetPerkStatValueAt(int perk, int slot, int points) {
    if(points <= 0)
        return 0;
    return PerkTable[perk].first_point[slot] + (points - 1) * PerkTable[perk].per_extra[slot];
}

// Move a perk from one point count to another and fold the DIFFERENCE into the player's stats.
// Taking before/after rather than a delta is what lets one function serve the spend (n -> n+1), the
// refund (n -> n-1) and the database load (0 -> n) without any of them special casing the curve.
void ApplyPerkPoints(int pnum, int perk, int before, int after) {
    if(perk < 0 || perk >= PERK_COUNT || before == after)
        return;

    int i;
    for(i = 0; i < DND_PERK_STATSLOTS; ++i) {
        if(PerkTable[perk].pstat[i] == -1)
            break;
        AddPerkStatValue(pnum, PerkTable[perk].pstat[i],
            GetPerkStatValueAt(perk, i, after) - GetPerkStatValueAt(perk, i, before));
    }

    // Refcounted by SetPlayerFlag, so only the crossings of zero are ours to report.
    if(PerkTable[perk].pflag != -1) {
        if(!before && after > 0)
            SetPlayerFlag(pnum, PerkTable[perk].pflag, false);
        else if(before > 0 && !after)
            SetPlayerFlag(pnum, PerkTable[perk].pflag, true);
    }
}

// Everything a character's perks contribute, in one pass. For the database load, which writes the
// packed lanes in bulk and so cannot fold as it goes.
void ApplyAllPerkStats(int pnum) {
    for(int i = 0; i < PERK_COUNT; ++i) {
        int pts = GetPerkPoints(pnum, i);
        if(pts)
            ApplyPerkPoints(pnum, i, 0, pts);
    }
}

bool PerkAffectsDamage(int perk) {
    return !!(PerkTable[perk].flags & PERKF_AFFECTSDAMAGE);
}

// ---------------------------------------------------------------- spend and refund
// Both go through the gate rather than trusting the caller, because the menu is clientside and a
// spend that arrives from there has to be re-checked on the server anyway.

bool SpendPerkPoint(int pnum, int perk) {
    if(!CanSpendOnPerk(pnum, perk))
        return false;

    int before = GetPerkPoints(pnum, perk);
    SetPerkPoints(pnum, perk, before + 1);
    TakePerkPoints(pnum, 1);
    SyncPerkWord(pnum, perk);
    ApplyPerkPoints(pnum, perk, before, before + 1);

    // The stat is already folded in above; this is only the cached damage that was computed from the
    // old value. Perks that feed nothing on the damage path do not pay for it.
    if(PerkAffectsDamage(perk))
        ForcePlayerDamageCaching(pnum);

    return true;
}

// The credit cost is the caller's business, not this function's -- respec is a menu action and this
// is also what the post-load legality sweep uses, which must not be charged for.
bool RefundPerkPoint(int pnum, int perk) {
    if(!CanRefundPerkPoint(pnum, perk))
        return false;

    int before = GetPerkPoints(pnum, perk);
    SetPerkPoints(pnum, perk, before - 1);
    GivePerkPoints(pnum, 1);
    SyncPerkWord(pnum, perk);
    ApplyPerkPoints(pnum, perk, before, before - 1);

    if(PerkAffectsDamage(perk))
        ForcePlayerDamageCaching(pnum);

    return true;
}

// Deeper perks cost more, scaled by level.
//
// LEVELSCALE 9 makes the level multiplier run 1x at level 0 to 10x at MAXLEVELS, linearly. It is the
// only one of the three that isolates the expensive corner: BASECOST and POINTWEIGHT raise the cheap
// end along with the dear one, and the cheap end is not what needed raising. The deepest perk at
// level 100 went from 33k to 110k, which is the 3x the author asked for, while a shallow perk at
// level 1 moved from 2,040 to 2,180.
//
//   depth = threshold + POINTWEIGHT * points_held        (0..21 across the authored tree)
//   cost  = BASECOST * (1 + depth) * (100 + LEVELSCALE * level) / 100
//
//               level 1     level 50    level 100
//   shallow      2,180       11,000       20,000
//   deep        11,990       60,500      110,000
//
// Against the shop -- 150 priced items, median 5,000, p90 17,500 -- a deep respec at cap is about
// six p90 items, which is meant to be a commitment rather than a convenience.
#define DND_RESPEC_BASECOST     500
#define DND_RESPEC_POINTWEIGHT  3
#define DND_RESPEC_LEVELSCALE   9

// The floor the deepest perk at max level has to clear, in credits. Stated so the three constants
// above cannot be retuned back under it without the verifier saying so.
#define DND_RESPEC_DEEPFLOOR    99000

// Why a refund cannot happen right now, or 0 when it can. Same shape as GetPerkSpendBlocker and for
// the same reason: "another perk depends on this" and "you cannot afford it" are different problems
// and send the player somewhere different.
enum {
    PERKREFUND_NONE,
    PERKREFUND_NOPOINTS,
    PERKREFUND_LOCKS,       // dropping this point would strand a perk that is still held
    PERKREFUND_POOR
};

int GetPerkRespecCost(int pnum, int perk) {
    if(perk < 0 || perk >= PERK_COUNT)
        return 0;

    // threshold is the honest depth proxy: it is what the tree already charges to reach the perk and
    // the one number that rises monotonically going down a column.
    int depth = PerkTable[perk].threshold + DND_RESPEC_POINTWEIGHT * GetPerkPoints(pnum, perk);
    return DND_RESPEC_BASECOST * (1 + depth) * (100 + DND_RESPEC_LEVELSCALE * GetActorLevel(pnum + P_TIDSTART)) / 100;
}

// ---------------------------------------------------------------- the menu's view of the tree
// Everything below exists for the tree menu and nothing else reads it. It is here rather than in the
// menu files because it is a property of the tree, and because both the drawing side and the click
// side have to agree on it exactly -- a click arrives as a BOX NUMBER, and the only thing that turns
// that back into a perk is the ordered list built here.

// The LANGUAGE key IS the enum name. Generated from the same pass that writes LANGUAGE.perk, so a
// renamed perk cannot leave the menu looking up a string that no longer exists -- regenerate both
// rather than editing either by hand.
str GetPerkLangKey(int id) {
    static str keys[PERK_COUNT] = {
        // Acrobacy
        "PERK_ACRO_TACTICALDASH",
        "PERK_ACRO_KINETICPADS",
        "PERK_ACRO_SWIFTREFLEXES",
        "PERK_ACRO_ENHANCEDJOINTS",
        "PERK_ACRO_EVASIVEMANEUVERS",
        "PERK_ACRO_CRASHCOURSE",
        "PERK_ACRO_UNENDINGRUSH",
        "PERK_ACRO_THUMPER",
        "PERK_ACRO_HEADSTART",
        "PERK_ACRO_ALLSHAKINGPRESENCE",
        "PERK_ACRO_ADRENALINE",
        "PERK_ACRO_AUGMENTEDFOOTPADS",
        "PERK_ACRO_TAILWIND",
        "PERK_ACRO_NIMBLENESS",
        // Assassination
        "PERK_ASSN_BACKSTAB",
        "PERK_ASSN_DEADLINESS",
        "PERK_ASSN_PREPARATION",
        "PERK_ASSN_BIGGAMEHUNTER",
        "PERK_ASSN_BANEOFLEGENDS",
        "PERK_ASSN_OPENINGSALVO",
        "PERK_ASSN_ERADICATION",
        "PERK_ASSN_QUICKGETAWAY",
        "PERK_ASSN_MASTEROFSHADOWS",
        "PERK_ASSN_PLANNEDEXECUTION",
        "PERK_ASSN_STEADYSHOT",
        "PERK_ASSN_PRESSUREPOINTS",
        "PERK_ASSN_MERCYKILL",
        "PERK_ASSN_DANCEWITHDEATH",
        // Cunning
        "PERK_CUN_POTENTSALVE",
        "PERK_CUN_EFFICIENTRECYCLING",
        "PERK_CUN_ENRICHEDMINERALS",
        "PERK_CUN_SURGINGVITALITY",
        "PERK_CUN_DESPERATEMEASURES",
        "PERK_CUN_OVERFLOWINGRESERVES",
        "PERK_CUN_SPIKEDCONCOCTION",
        "PERK_CUN_DEEPWISDOM",
        "PERK_CUN_ENDLESSGREED",
        "PERK_CUN_ENDURINGCOMPOSURE",
        "PERK_CUN_LASTINGFRENZY",
        "PERK_CUN_ASTOUNDINGPOWER",
        "PERK_CUN_DISCIPLEOFTHEUNYIELDING",
        "PERK_CUN_DISCIPLEOFTHESLAUGHTER",
        "PERK_CUN_DISCIPLEOFTHEFORBIDDEN",
        "PERK_CUN_MASTEROFRESILIENCE",
        "PERK_CUN_MASTEROFFEROCITY",
        "PERK_CUN_MASTEROFTHEARCANE",
        // Endurance
        "PERK_END_PHYSICIAN",
        "PERK_END_UNWAVERINGSTANCE",
        "PERK_END_MEDIC",
        "PERK_END_VIGOUR",
        "PERK_END_RESTLESSVITALITY",
        "PERK_END_INDOMITABLERESOLVE",
        "PERK_END_CAMOUFLAGE",
        "PERK_END_DENSEEXOSKELETON",
        "PERK_END_STONESKIN",
        "PERK_END_CRYSTALSKIN",
        "PERK_END_ENHANCEDIRON",
        "PERK_END_FINESSE",
        "PERK_END_SHIELDING",
        "PERK_END_PERFECTSHELL",
        "PERK_END_WINDDANCER",
        "PERK_END_HIDDENRESERVE",
        // Martialist
        "PERK_MART_RAMPINGASSAULT",
        "PERK_MART_CRANIUMBASH",
        "PERK_MART_ECHOINGSTRIKES",
        "PERK_MART_UNENDINGFURY",
        "PERK_MART_FERVENTREACH",
        "PERK_MART_BLADEMASTER",
        "PERK_MART_EXHAUSTER",
        "PERK_MART_DEEPCUTS",
        "PERK_MART_QUICKDEADLY",
        "PERK_MART_SWIFTPRECISE",
        "PERK_MART_FLESHCARVER",
        "PERK_MART_FLASHPARRY",
        "PERK_MART_RIPOSTE",
        "PERK_MART_EXPOSEWEAKNESS",
        "PERK_MART_MARTIALFINESSE",
        "PERK_MART_GRATUITOUSVIOLENCE",
        // Perception
        "PERK_PERC_MUNITIONIST",
        "PERK_PERC_PLANB",
        "PERK_PERC_EXCESSCONVERSION",
        "PERK_PERC_FRESHCLIP",
        "PERK_PERC_LUCKYBULLET",
        "PERK_PERC_SALVAGER",
        "PERK_PERC_SLEIGHTOFHAND",
        "PERK_PERC_PRECISETECHNIQUE",
        "PERK_PERC_FIREDUP",
        "PERK_PERC_PUMPED",
        "PERK_PERC_QUAKER",
        "PERK_PERC_DARKARTS",
        "PERK_PERC_SHARPSHOOTER",
        "PERK_PERC_CEASELESSASSAULT",
        "PERK_PERC_BLASTIERSHOTS",
        "PERK_PERC_EARTHSHAKER",
        "PERK_PERC_ESSENCETHEFT",
        "PERK_PERC_BASTIONBREAKER",
        "PERK_PERC_SHIELDSTEALER",
        "PERK_PERC_UNSTOPPABLEFORCE",
        "PERK_PERC_HEATSINKS",
        "PERK_PERC_EMERGENCYPROTOCOL",

        "PERK_TORM_STUDENTOFDECAY",
        "PERK_TORM_PROLONGEDTORTURE",
        "PERK_TORM_DEATHSGRIP",
        "PERK_TORM_FLOWOFLIFE",
        "PERK_TORM_CORROSION",
        "PERK_TORM_ACRIMONY",
        "PERK_TORM_SEPTICTOUCH",
        "PERK_TORM_BITTERFROST",
        "PERK_TORM_LINGERINGCOLD",
        "PERK_TORM_FLASHFREEZE",
        "PERK_TORM_CRIPPLINGICE",
        "PERK_TORM_BRINGEROFICE",
        "PERK_TORM_BREATHOFRIME",
        "PERK_TORM_PERMAFROST",
        "PERK_TORM_SLOWCOOKER",
        "PERK_TORM_DEEPFRIED",
        "PERK_TORM_BLOWBACK",
        "PERK_TORM_COOKEDALIVE",
        "PERK_TORM_WILDFIRE",
        "PERK_TORM_CREMATOR",
        "PERK_TORM_AVATAROFFIRE",
        "PERK_TORM_SHOCKER",
        "PERK_TORM_COURSINGCURRENT",
        "PERK_TORM_MAXIMUMOVERDRIVE",
        "PERK_TORM_MUSCLESPASMS",
        "PERK_TORM_POWERUP",
        "PERK_TORM_JOLT",
        "PERK_TORM_SUPERCONDUCTOR",
        "PERK_TORM_WOUNDER",
        "PERK_TORM_BLOODLETTING",
        "PERK_TORM_SWIFTDRAIN",
        "PERK_TORM_SAVAGEWOUNDS",
        "PERK_TORM_CORNEREDPREY",
        "PERK_TORM_MASTEROFWOUNDS",
    };

    if(id < 0 || id >= PERK_COUNT)
        return "";
    return keys[id];
}

str GetPerkArchLangKey(int arch) {
    static str akeys[PERK_ARCH_COUNT] = {
        "PERK_ARCH_ACRO",
        "PERK_ARCH_ASSN",
        "PERK_ARCH_CUN",
        "PERK_ARCH_END",
        "PERK_ARCH_MART",
        "PERK_ARCH_PERC",
        "PERK_ARCH_TORM"
    };

    if(arch < 0 || arch >= PERK_ARCH_COUNT)
        return "";
    return akeys[arch];
}

// One archetype's perks in display order, plus where its categories break. Built once with the table
// because the menu walks it every frame and the click handler indexes straight into it.
//
// The largest authored archetype is Tormentor at 35, five categories of seven. The cap is the
// pane's, not the tree's: a page cannot offer more clickable rows than MAX_PANE_BOXES holds, so an
// archetype that outgrew this would be silently unclickable past the cap rather than merely cramped.
//
// 40 rather than 35 for headroom, and it is also what bounds the menu's row id band -- see the note
// on DND_PERKROW_IDBASE. Raising this without checking that band silently pushes rows past the
// range the menu clears each redraw.
#define DND_MAX_PERKS_PERARCH 40

typedef struct {
    int count;
    int rows;                           // count plus one blank per category break, which is what scrolls
    int maxpoints;                      // every point this archetype could ever hold
    int perk[DND_MAX_PERKS_PERARCH];
    int depth[DND_MAX_PERKS_PERARCH];   // rows to indent, so the requirement chain reads as a tree
    bool newcat[DND_MAX_PERKS_PERARCH]; // first perk of a category, which the page spaces away
} perk_arch_list_T;

perk_arch_list_T module& GetPerkArchList(int arch) {
    static perk_arch_list_T lists[DND_MAX_PERK_ARCHETYPES];
    return lists[arch];
}

// How deep a perk sits in its own requirement chain. Recursive, and bounded by fuel rather than by
// trusting the data: a cycle cannot occur in authored requirements, but if one ever did it would take
// the whole table build down with it rather than producing a visibly wrong indent.
int GetPerkChainDepth(int perk, int fuel) {
    if(perk < 0 || perk >= PERK_COUNT || fuel <= 0 || PerkTable[perk].req_kind == PERK_REQ_NONE)
        return 0;

    // A tag requirement has no single parent to hang under, so it reads as a root.
    if(PerkTable[perk].req_kind == PERK_REQ_TAG)
        return 0;

    int deepest = 0;
    for(int i = 0; i < DND_MAX_PERK_REQS; ++i) {
        int r = PerkTable[perk].req_perk[i];
        if(r == -1)
            break;

        int d = GetPerkChainDepth(r, fuel - 1);
        if(d > deepest)
            deepest = d;
    }

    return deepest + 1;
}

// Called by the menu before it reads any list, on both sides. The build and the READ are not
// guaranteed to happen on the same one: SetupPerkTable runs inside a block gated on
// SETUP_ITEMTABLES, and a client already carrying that flag skips the whole block -- so the server
// builds the lists, the clientside menu draws from them, and on that client nothing ever filled them.
// Seven empty archetypes and not one clickable row.
//
// Keyed on Acrobacy holding nothing while the table holds something, which can only mean the build
// has not run on this side: Acrobacy has perks, so a built list is never empty. Kept out of
// GetPerkArchList itself because BuildPerkArchLists reads through that accessor and would re-enter.
void EnsurePerkArchLists() {
    if(!GetPerkArchList(PERK_ARCH_ACRO).count && IsPerkTableReady())
        BuildPerkArchLists();
}

void BuildPerkArchLists() {
    int a, i;

    for(a = 0; a < DND_MAX_PERK_ARCHETYPES; ++a) {
        perk_arch_list_T module& wipe = GetPerkArchList(a);
        wipe.count = 0;
        wipe.rows = 0;
        wipe.maxpoints = 0;
    }

    for(i = 0; i < PERK_COUNT; ++i) {
        if(!PerkTable[i].max_points)
            continue;

        perk_arch_list_T module& list = GetPerkArchList(PerkTable[i].archetype);
        if(list.count >= DND_MAX_PERKS_PERARCH) {
            Log(s:"BuildPerkArchLists: archetype ", d:PerkTable[i].archetype, s:" has more than ",
                d:DND_MAX_PERKS_PERARCH, s:" perks, the rest will not be reachable in the menu!");
            continue;
        }

        // Written as a statement rather than folded into one expression with `list.count &&`:
        // the subscript is list.perk[count - 1], which is [-1] on the first perk of an archetype.
        // Reading a global array at -1 kills the setup script outright, taking every list with it.
        bool breaks_cat = false;
        if(list.count)
            breaks_cat = PerkTable[list.perk[list.count - 1]].category != PerkTable[i].category;

        list.perk[list.count] = i;
        list.depth[list.count] = GetPerkChainDepth(i, DND_MAX_PERK_REQS + 1);
        list.newcat[list.count] = breaks_cat;

        list.rows += 1 + breaks_cat;
        list.maxpoints += PerkTable[i].max_points;
        ++list.count;
    }

#ifdef VERBOSE_PERK_SETUP
    for(a = 0; a < PERK_ARCH_COUNT; ++a) {
        perk_arch_list_T module& done = GetPerkArchList(a);
        Log(s:"  perk archetype ", d:a, s:": ", d:done.count, s:" perks, ", d:done.rows,
            s:" rows, ", d:done.maxpoints, s:" points to spend.");
    }
#endif
}

// The two scroll POSITIONS are the scroll bars' own -- bar 0 for the list, bar 1 for the panel --
// so nothing here tracks them. What is left is what the bars cannot hold: shown_perk, which is the
// perk the panel is describing, kept across the cursor LEAVING the list. Moving down to read the
// panel is the same gesture as un-hovering the row it belongs to, so a panel that followed hover
// would empty exactly as you reached for it.
typedef struct {
    int panel_lines;    // estimated, for the panel bar's thumb only -- see DND_PERKPANEL_CPL
    int shown_perk;
    int shown_arch;     // which page shown_perk belongs to, so a page change drops it
    bool pinned;        // jump + click held the panel on shown_perk; hover stops moving it
} perk_scroll_T;

perk_scroll_T module& GetPerkScroll() {
    static perk_scroll_T s;
    return s;
}

// Why a perk cannot be spent on right now, or 0 when it can. The menu needs the REASON rather than
// the yes/no CanSpendOnPerk gives, because "you are 3 points short in this archetype" and "you have
// no points left" send the player somewhere completely different.
enum {
    PERKBLOCK_NONE,
    PERKBLOCK_MAXED,
    PERKBLOCK_THRESHOLD,
    PERKBLOCK_REQUIRES,
    PERKBLOCK_NOPOINTS
};

int GetPerkSpendBlocker(int pnum, int perk) {
    if(perk < 0 || perk >= PERK_COUNT || !PerkTable[perk].max_points)
        return PERKBLOCK_MAXED;

    if(GetPerkPoints(pnum, perk) >= PerkTable[perk].max_points)
        return PERKBLOCK_MAXED;

    // Order matters: the gates are reported before the wallet, because a locked perk stays locked
    // however many points the player is holding and that is the more useful thing to be told.
    if(!IsPerkThresholdMet(pnum, perk))
        return PERKBLOCK_THRESHOLD;

    if(!IsPerkRequirementMet(pnum, perk))
        return PERKBLOCK_REQUIRES;

    if(!PlayerModData[pnum].unspent)
        return PERKBLOCK_NOPOINTS;

    return PERKBLOCK_NONE;
}

// ---------------------------------------------------------------- the database side
// The save holds nothing but the packed lanes and the pool. Everything else -- the archetype totals,
// which perks are legal, what they contribute -- is derived from the tree, which is authored data
// that ships with the mod. So a load writes the lanes and then rebuilds the rest from scratch, and a
// tree that changed between sessions is reconciled rather than trusted.

// Asks the TABLE, not the setup flag. Those are not the same question on a client: the setup block
// is gated on SETUP_ITEMTABLES and a client that already carries the flag skips the block entirely,
// so the flag can read complete while this side's table is still empty. A load that judged a saved
// perk against an empty table would read every one of them as retired and strip the character.
bool IsPerkTableReady() {
    return !!PerkTable[PERK_ACRO_TACTICALDASH].max_points;
}

// How long the clientside fallback below waits for the normal setup before building the table itself.
#define DND_PERKTABLE_FALLBACK_WAIT (5 * TICRATE)

// Reconcile a freshly loaded character against the tree as it exists now, returning the points that
// had to come back. A perk can have been retired, had its cap lowered, had its threshold raised, or
// lost a prerequisite it named -- none of which the save knows about.
//
// The threshold sweep iterates because dropping one perk lowers spent_in, which can push a second
// below ITS threshold; PERK_COUNT rounds bounds it, since every round but the last drops a perk.
int ValidatePlayerPerks(int pnum) {
    int i, refunded = 0;

    // Caps first, and over the whole array rather than PERK_COUNT: a lane above the authored range
    // can only be corruption, and SetPerkPoints clamps it to the zeroed table's 0. No gating is
    // involved here, which is why it is settled before the totals are counted.
    for(i = 0; i < DND_MAX_PERKS_NEW; ++i) {
        int pts = GetPerkPoints(pnum, i);
        int cap = 0;
        if(i < PERK_COUNT)
            cap = PerkTable[i].max_points;

        if(pts > cap) {
            SetPerkPoints(pnum, i, cap);
            refunded += pts - cap;
        }
    }

    // Only now, with every lane inside the authored range, are the totals meaningful -- counting
    // before the clamp would fold points from retired perks into an archetype that no longer has them.
    RecountPerkPoints(pnum);

    bool changed = true;
    for(int round = 0; round < PERK_COUNT && changed; ++round) {
        changed = false;

        for(i = 0; i < PERK_COUNT; ++i) {
            int held = GetPerkPoints(pnum, i);
            if(held && !IsPerkUnlocked(pnum, i)) {
                SetPerkPoints(pnum, i, 0);
                refunded += held;
                changed = true;
            }
        }
    }

    return refunded;
}

// The database writes the lanes in bulk, so nothing folds into the player's stats as it goes and the
// running totals cannot survive the write. This is where both are put right.
//
// It is a script rather than a function because it has to be able to WAIT: a character load can land
// before the OPEN script has finished building the perk table, and a function cannot delay. Reading
// the lanes is safe at any time, which is why the database does that part itself and leaves
// everything that consults the table to here.
Script "DnD Apply Loaded Perks" (int pnum) {
    while(!IsPerkTableReady())
        Delay(const:1);

    // That wait can outlive the load that started this, so the activator is re-established rather
    // than inherited -- the derived-property calls at the bottom are activator based.
    if(!PlayerInGame(pnum) || !SetActivator(pnum + P_TIDSTART))
        Terminate;

    int refunded = ValidatePlayerPerks(pnum);
    if(refunded) {
        GivePerkPoints(pnum, refunded);
        Log(s:"Refunded ", d:refunded, s:" perk point(s) sitting on nodes the tree no longer allows.");
    }

    ApplyAllPerkStats(pnum);
    RefreshUnspentPerkPoints(pnum);

    // Whole words, unconditionally -- a word going back to 0 is exactly the update a client must not
    // miss, and a load is the one moment its copy is guaranteed stale.
    for(int i = 0; i < DND_PERK_WORDS; ++i)
        ACS_NamedExecuteWithResult("DnD Request Perk Sync", pnum, i, PlayerModData[pnum].perks_packed[i]);

    // Speed, mass and spawn health are DERIVED from the stats rather than read out of them, so none
    // of them move on their own when the tree folds in. RestoreRPGStat is deliberately not used for
    // this -- most of what it does hands out one-per-map items and is not safe to repeat.
    SetActorProperty(0, APROP_SPEED, GetPlayerSpeed(pnum));
    SetActorProperty(0, APROP_SPAWNHEALTH, GetSpawnHealth(false, pnum));
    UpdatePlayerKnockbackResist();

    // One recalculation for the whole tree rather than one per perk. Everything is already folded
    // into PlayerModData by this point, so the cache is the only thing left holding old numbers.
    ForcePlayerDamageCaching(pnum);
    SetResultValue(0);
}

int GetPerkRefundStatus(int pnum, int perk) {
    if(perk < 0 || perk >= PERK_COUNT || !GetPerkPoints(pnum, perk))
        return PERKREFUND_NOPOINTS;

    if(!CanRefundPerkPoint(pnum, perk))
        return PERKREFUND_LOCKS;

    if(GetPlayerCredit(pnum) < GetPerkRespecCost(pnum, perk))
        return PERKREFUND_POOR;

    return PERKREFUND_NONE;
}

// The paid refund: one point, one charge. Refund FIRST and charge only once it has happened -- the
// other order can take the credits for a refund that then declines, and RefundPerkPoint re-runs the
// whole legality check rather than trusting the one above it.
bool RespecPerkPoint(int pnum, int perk) {
    if(GetPerkRefundStatus(pnum, perk) != PERKREFUND_NONE)
        return false;

    // Read before the refund. The cost is a function of the points still on the perk, so asking
    // afterwards prices the point that is left rather than the one being taken back.
    int cost = GetPerkRespecCost(pnum, perk);

    if(!RefundPerkPoint(pnum, perk))
        return false;

    TakeCredit(cost);
    return true;
}

// Units are NOT uniform across PSTAT slots and getting one wrong is a 65536x error, so each of these
// was checked against an item mod that feeds the same slot rather than assumed. Fixed point slots are
// marked; everything else is a plain integer.
//
// Only the perks whose effect IS one of these stats are here. The rest are conditional or
// behavioural and want a hook where the behaviour lives -- they read HasPerk at their own call site
// and are listed at the bottom of .claude/notes/dnd-perk-rework.md.
void SetupPerkStats() {
    // ---------------- Acrobacy ----------------
    // Tactical Dash gives the dash on its first point and 10% off the cooldown on every point,
    // including the first -- the notes read "unlocks ... 6 second cooldown" then "+10% reduced",
    // so point one is the plain 6 seconds and the reduction is what the later points buy.
    SetPerkStat(PERK_ACRO_TACTICALDASH, PSTAT_DASH_UNLOCK, 1, 0);
    SetPerkStat(PERK_ACRO_TACTICALDASH, PSTAT_DASH_COOLDOWNREDUCE, 0, 10);

    SetPerkStat(PERK_ACRO_ENHANCEDJOINTS, PSTAT_DASH_RANGE, 15);
    SetPerkStat(PERK_ACRO_KINETICPADS, PSTAT_DASH_LESSDMGTAKEN, 5, 3);
    SetPerkStat(PERK_ACRO_SWIFTREFLEXES, PSTAT_DASH_MELEEBONUS, 10, 5, PERKF_AFFECTSDAMAGE);

    // 5% then +2.5% rounds to 3; the duration bonus is half a second (17 tics) per further point
    SetPerkStat(PERK_ACRO_EVASIVEMANEUVERS, PSTAT_DASH_AVOIDCHANCE, 5, 3);
    SetPerkStat(PERK_ACRO_EVASIVEMANEUVERS, PSTAT_DASH_AVOIDTIME, 0, 17);

    SetPerkStat(PERK_ACRO_UNENDINGRUSH, PSTAT_DASH_REFRESHONKILL, 1);

    // 1.5s then +0.25s per further point, in tics
    SetPerkStat(PERK_ACRO_CRASHCOURSE, PSTAT_DASH_CRASHSTUN, 52, 9);

    // fall-impact cluster. Thumper's power is a percent OF the base, so point one is 100%.
    SetPerkStat(PERK_ACRO_THUMPER, PSTAT_THUMPER_POWER, 100, 25, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_ACRO_HEADSTART, PSTAT_FALL_SPEEDBONUS, 10, 3);
    SetPerkStat(PERK_ACRO_HEADSTART, PSTAT_FALL_SPEEDTIME, 105, 17);

    // the + line on this one is duration only, so the vulnerability itself does not scale
    SetPerkStat(PERK_ACRO_ALLSHAKINGPRESENCE, PSTAT_THUMPER_VULN, 10, 0);
    SetPerkStat(PERK_ACRO_ALLSHAKINGPRESENCE, PSTAT_THUMPER_VULNTIME, 175, 35);

    SetPerkStat(PERK_ACRO_ADRENALINE, PSTAT_ADRENALINE_HEAL, 10);

    SetPerkStat(PERK_ACRO_AUGMENTEDFOOTPADS, PSTAT_SPEED_INCREASE, 2, 1);

    // fixed point: PSTAT_MIT_CHANCE is a 16.16 slot, so 1% is 1.0
    SetPerkStat(PERK_ACRO_NIMBLENESS, PSTAT_MIT_PERRUNSEC, 1.0, 0.5);

    // the + line is cooldown only, so the speed itself does not scale
    SetPerkStat(PERK_ACRO_TAILWIND, PSTAT_TAILWIND_SPEED, 10, 0);
    SetPerkStat(PERK_ACRO_TAILWIND, PSTAT_TAILWIND_CDREDUCE, 0, 1);

    // ---------------- Assassination ----------------
    // fixed point: INV_CRITCHANCE_INCREASE authors 0.005..0.009, so 1% is 0.01
    SetPerkStat(PERK_ASSN_DEADLINESS, PSTAT_CRITCHANCE_INCREASE, 0.01, 0.015, PERKF_AFFECTSDAMAGE);

    // already conditional in GetCritChance, which gates it on IsPrecisionWeapon -- fixed point too
    SetPerkStat(PERK_ASSN_PLANNEDEXECUTION, PSTAT_IMP_PRECISIONCRITBONUS, 0.05, -0x7FFFFFFF, PERKF_AFFECTSDAMAGE);

    SetPerkStat(PERK_ASSN_BIGGAMEHUNTER, PSTAT_CRITDAMAGE_VS_BOSS, 10, -0x7FFFFFFF, PERKF_AFFECTSDAMAGE);

    // both halves of one perk, which is what the second stat slot is for
    SetPerkStat(PERK_ASSN_BANEOFLEGENDS, PSTAT_CRITCHANCE_VS_ELITE, 0.05, -0x7FFFFFFF, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_ASSN_BANEOFLEGENDS, PSTAT_CRITDAMAGE_VS_ELITE, 30);

    // "N% more susceptible to crits" -- integer percent, applied as a multiplier at the read site
    SetPerkStat(PERK_ASSN_OPENINGSALVO, PSTAT_CRITCHANCE_VS_FULLLIFE, 15, 5, PERKF_AFFECTSDAMAGE);

    // notes say 10% then +2.5%; the slot is integer, so the increment rounds to 3 the way Deep Cuts
    // rounded rather than widening a slot for one perk
    SetPerkStat(PERK_ASSN_MERCYKILL, PSTAT_CRITCHANCE_VS_LOWLIFE, 10, 3, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_ASSN_MASTEROFSHADOWS, PSTAT_CRITCHANCE_INDARK, 25, 25, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_ASSN_QUICKGETAWAY, PSTAT_CRIT_SPEEDBONUS, 5, 1);
    SetPerkStat(PERK_ASSN_PREPARATION, PSTAT_CRIT_DROUGHTBONUS, 50, -0x7FFFFFFF, PERKF_AFFECTSDAMAGE);

    // fixed point, same slot family as every other crit chance
    SetPerkStat(PERK_ASSN_BACKSTAB, PSTAT_CRITCHANCE_FROMBEHIND, 0.02, 0.01, PERKF_AFFECTSDAMAGE);

    SetPerkStat(PERK_ASSN_STEADYSHOT, PSTAT_CRITDAMAGE_PERSTILLSEC, 10, 5, PERKF_AFFECTSDAMAGE);

    // 10% then +2.5% rounds to 3, as elsewhere
    SetPerkStat(PERK_ASSN_PRESSUREPOINTS, PSTAT_CRIT_EXPOSEPCT, 10, 3, PERKF_AFFECTSDAMAGE);

    // 10% then +2.5%, rounded as elsewhere
    SetPerkStat(PERK_ASSN_DANCEWITHDEATH, PSTAT_ELUSIVE_ONCRIT, 10, 3);
    SetPerkStat(PERK_ASSN_ERADICATION, PSTAT_CRITKILL_NODEATHFX, 1);

    // ---------------- Cunning ----------------
    SetPerkStat(PERK_CUN_EFFICIENTRECYCLING, PSTAT_INCFLASKCHARGEGAINED, 15);
    SetPerkStat(PERK_CUN_POTENTSALVE, PSTAT_FLASK_CLEANSECHANCE, 33);
    SetPerkStat(PERK_CUN_DEEPWISDOM, PSTAT_EXPGAIN_ANYRANGE, 1);
    SetPerkStat(PERK_CUN_ENDLESSGREED, PSTAT_CREDITGAIN_ANYRANGE, 1);
    SetPerkStat(PERK_CUN_ENRICHEDMINERALS, PSTAT_FLASK_UTILDURATION, 15);
    SetPerkStat(PERK_CUN_SURGINGVITALITY, PSTAT_FLASK_RESISTBONUS, 5);
    SetPerkStat(PERK_CUN_DESPERATEMEASURES, PSTAT_FLASK_FREEUSECHANCE, 25);
    SetPerkStat(PERK_CUN_OVERFLOWINGRESERVES, PSTAT_FLASK_REFILLCHANCE, 10);

    // the + line is duration only, so the per-stack percent does not scale
    SetPerkStat(PERK_CUN_SPIKEDCONCOCTION, PSTAT_FLASK_ENEMYVULN, 5, 0, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_CUN_SPIKEDCONCOCTION, PSTAT_FLASK_VULNTIME, 105, 35);
    // "N% longer" per charge type -- the slot run is indexed by DND_CHARGE_*, so each perk names one
    SetPerkStat(PERK_CUN_ENDURINGCOMPOSURE, PSTAT_CHARGEDURATION_BASE + DND_CHARGE_ENDURANCE, 20, 15);
    SetPerkStat(PERK_CUN_LASTINGFRENZY, PSTAT_CHARGEDURATION_BASE + DND_CHARGE_FRENZY, 20, 15);
    SetPerkStat(PERK_CUN_ASTOUNDINGPOWER, PSTAT_CHARGEDURATION_BASE + DND_CHARGE_POWER, 20, 15);

    SetPerkStat(PERK_CUN_DISCIPLEOFTHEUNYIELDING, PSTAT_IMP_ONKILL_ENDURANCE, 8);
    SetPerkStat(PERK_CUN_DISCIPLEOFTHESLAUGHTER, PSTAT_IMP_ONKILL_FRENZY, 8);
    SetPerkStat(PERK_CUN_DISCIPLEOFTHEFORBIDDEN, PSTAT_IMP_ONKILL_POWER, 8);
    SetPerkStat(PERK_CUN_MASTEROFRESILIENCE, PSTAT_CORR_MAXENDURANCE, 1);
    SetPerkStat(PERK_CUN_MASTEROFFEROCITY, PSTAT_CORR_MAXFRENZY, 1);
    SetPerkStat(PERK_CUN_MASTEROFTHEARCANE, PSTAT_CORR_MAXPOWER, 1);

    // ---------------- Endurance ----------------
    // the two-slot case: "200 knockback resist AND 5% health" is one perk giving two numbers
    SetPerkStat(PERK_END_UNWAVERINGSTANCE, PSTAT_KNOCKBACK_RESIST, 200);
    SetPerkStat(PERK_END_UNWAVERINGSTANCE, PSTAT_HP_PCT, 5);

    SetPerkStat(PERK_END_CAMOUFLAGE, PSTAT_AVOID_HOMING, 10);
    SetPerkStat(PERK_END_PHYSICIAN, PSTAT_HEAL_CLEANSECHANCE, 50, 25);
    SetPerkStat(PERK_END_WINDDANCER, PSTAT_UNMITIGATED_LESSDMG, 10);

    // five seconds of immunity after the first ailment lands, in tics
    SetPerkStat(PERK_END_INDOMITABLERESOLVE, PSTAT_AILMENT_IMMUNETICS, 175);
    SetPerkStat(PERK_END_DENSEEXOSKELETON, PSTAT_RIPIMMUNE_TICS, 10);

    // Hidden Reserve's downside SHRINKS with points -- 40% reduced recharge, then 25% -- so the
    // slot runs -40 to -25 rather than compounding downward like every other per_extra.
    SetPerkFlag(PERK_END_HIDDENRESERVE, PFLAG_ESCHARGE_NOINTERRUPT);
    SetPerkStat(PERK_END_HIDDENRESERVE, PSTAT_SHIELD_RECOVERYRATE, -40, 15);
    SetPerkStat(PERK_END_MEDIC, PSTAT_HEALING_EFFECT, 20);
    SetPerkStat(PERK_END_STONESKIN, PSTAT_DOT_DMGTAKEN_REDUCE, 15, 10);
    SetPerkStat(PERK_END_VIGOUR, PSTAT_REGENRATE, 2);
    SetPerkStat(PERK_END_RESTLESSVITALITY, PSTAT_CORR_DMGDOESNTSTOPREGEN, 1);
    SetPerkStat(PERK_END_CRYSTALSKIN, PSTAT_MAXRESIST_ADDED, 1.5);          // fixed point
    SetPerkStat(PERK_END_ENHANCEDIRON, PSTAT_ARMOR_PCT, 10);
    SetPerkStat(PERK_END_FINESSE, PSTAT_MIT_CHANCE, 5.0);                   // fixed point
    SetPerkStat(PERK_END_SHIELDING, PSTAT_SHIELD_PCT, 5);
    SetPerkStat(PERK_END_PERFECTSHELL, PSTAT_ARMOR_DOUBLEDEF, 8);

    // ---------------- Martialist ----------------
    // 15% then +7.5% in the notes; the slot is integer percent and shared with item mods that
    // author integers, so the increment is rounded to 8 rather than widening the slot under them.
    SetPerkStat(PERK_MART_DEEPCUTS, PSTAT_BLEED_CHANCE_SLASHING, 15, 8);
    SetPerkStat(PERK_MART_EXHAUSTER, PSTAT_SLOWCHANCE_MELEE, 15, 5);

    // hits NEEDED counts down with points -- 6 then 5 then 4 -- so per_extra is negative
    SetPerkStat(PERK_MART_RAMPINGASSAULT, PSTAT_RAMPING_HITS, 6, -1);
    SetPerkStat(PERK_MART_RAMPINGASSAULT, PSTAT_RAMPING_BONUS, 20, 0, PERKF_AFFECTSDAMAGE);

    SetPerkStat(PERK_MART_RIPOSTE, PSTAT_PARRY_COSTREDUCE, 10);
    SetPerkStat(PERK_MART_UNENDINGFURY, PSTAT_BERSERK_ALLMELEE, 25, 25, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_MART_BLADEMASTER, PSTAT_MELEE_TOPROJECTILES, 1, -0x7FFFFFFF, PERKF_AFFECTSDAMAGE);

    // the effect itself is DECORATE-side; this only grants the token it looks for
    SetPerkStat(PERK_MART_QUICKDEADLY, PSTAT_SLASH_QUICKDEADLY, 1);

    // Perception / Sleight of Hand. This is the retired Fast Reload ability: Ability_Reloader is a
    // token roughly forty weapon reload states already branch on, so the perk grants that same token
    // rather than reimplementing the timing. Nothing in DECORATE changes.
    SetPerkStat(PERK_PERC_SLEIGHTOFHAND, PSTAT_FASTRELOAD, 1);
    SetPerkStat(PERK_MART_GRATUITOUSVIOLENCE, PSTAT_RAGE_ONOVERKILL, 5);
    SetPerkStat(PERK_MART_FLASHPARRY, PSTAT_PARRY_SHOCKDMG, 100, 50, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_MART_FLASHPARRY, PSTAT_PARRY_SHOCKRANGE, 0, 10);

    // 12.5% then +12.5%; the slot is integer percent so both round to 13
    SetPerkStat(PERK_MART_ECHOINGSTRIKES, PSTAT_MELEESPLASH_CHANCE, 13, 13, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_MART_FERVENTREACH, PSTAT_MELEESPLASH_RANGE, 20);
    SetPerkStat(PERK_MART_SWIFTPRECISE, PSTAT_MELEEHIT_SPEED, 10);
    SetPerkStat(PERK_MART_EXPOSEWEAKNESS, PSTAT_PARRY_RESISTREDUCE, 25, 15);
    SetPerkStat(PERK_MART_CRANIUMBASH, PSTAT_STUNCHANCE_BLUNT, 4, 3);
    SetPerkStat(PERK_MART_FLESHCARVER, PSTAT_BLEEDRATE_MELEE, 10, 10, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_MART_MARTIALFINESSE, PSTAT_MELEE_ATKCDR, 8);

    // ---------------- Perception ----------------
    SetPerkStat(PERK_PERC_MUNITIONIST, PSTAT_AMMOGAIN_INCREASE, 20, 15);
    SetPerkStat(PERK_PERC_PRECISETECHNIQUE, PSTAT_PRECISION_PERCENT, 15, 15, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_PERC_FIREDUP, PSTAT_FLAT_AUTOMATIC, 1, 2, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_PERC_BASTIONBREAKER, PSTAT_BLOCK_PIERCE, 15, 15, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_PERC_SHIELDSTEALER, PSTAT_SHIELDSTEAL_PCT, 5);

    // both of these carry their own cap as a second slot, because "up to N" is part of the perk
    SetPerkStat(PERK_PERC_CEASELESSASSAULT, PSTAT_AUTO_RESISTSHRED, 2, -0x7FFFFFFF, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_PERC_CEASELESSASSAULT, PSTAT_AUTO_RESISTSHREDCAP, 20);

    SetPerkStat(PERK_PERC_ESSENCETHEFT, PSTAT_MAGICKILL_PEN, 2, -0x7FFFFFFF, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_PERC_ESSENCETHEFT, PSTAT_MAGICKILL_PENCAP, 10);
    SetPerkStat(PERK_PERC_SALVAGER, PSTAT_AMMOTOKEN_CHANCE, 2);
    SetPerkStat(PERK_PERC_UNSTOPPABLEFORCE, PSTAT_RIPPER_SURVIVECHANCE, 5, 8);
    SetPerkStat(PERK_PERC_EXCESSCONVERSION, PSTAT_AMMO_CONVERTRATE, 15, 10);

    // 10% a stack, up to three continuous attacks
    SetPerkStat(PERK_PERC_EARTHSHAKER, PSTAT_ARTILLERY_RAMP, 10, -0x7FFFFFFF, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_PERC_EARTHSHAKER, PSTAT_ARTILLERY_RAMPCAP, 3);
    SetPerkStat(PERK_PERC_SHARPSHOOTER, PSTAT_PRECISION_FALLOFF, 30, -0x7FFFFFFF, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_PERC_PUMPED, PSTAT_SHELLCAP_FLAT, 1);
    SetPerkStat(PERK_PERC_BLASTIERSHOTS, PSTAT_PELLET_FLAT_SHOTGUN, 1);
    SetPerkStat(PERK_PERC_PLANB, PSTAT_EMPTYMAG_SPEED, 10);

    // 3 seconds of firing past full overheat, in tics
    SetPerkStat(PERK_PERC_EMERGENCYPROTOCOL, PSTAT_OVERHEAT_GRACE, 105);

    // 45 second recharge, 10 seconds off per further point
    SetPerkStat(PERK_PERC_HEATSINKS, PSTAT_HEATSINK_CDREDUCE, 3, 5);

    // "empowers the next 5 shots", "+1 round" per further point -- the DAMAGE does not scale
    SetPerkStat(PERK_PERC_FRESHCLIP, PSTAT_FRESHCLIP_SHOTS, 5, 1);
    SetPerkStat(PERK_PERC_FRESHCLIP, PSTAT_FRESHCLIP_DAMAGE, 15, 0, PERKF_AFFECTSDAMAGE);

    // fixed point crit chance, integer crit multiplier -- the two halves use different slots
    SetPerkStat(PERK_PERC_LUCKYBULLET, PSTAT_LASTROUND_CRIT, 0.025, -0x7FFFFFFF, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_PERC_LUCKYBULLET, PSTAT_LASTROUND_CRITDMG, 25);
    // The artillery-only slot, not PSTAT_EXPLOSION_RADIUS: the note says artillery weapons, and
    // that slot is every explosion the player causes.
    SetPerkStat(PERK_PERC_QUAKER, PSTAT_ARTILLERY_RADIUS, 10, 10);
    SetPerkStat(PERK_PERC_DARKARTS, PSTAT_MAGIC_PERCENT, 10, 15, PERKF_AFFECTSDAMAGE);

    // ================= Tormentor =================
    // Poison
    SetPerkStat(PERK_TORM_PROLONGEDTORTURE, PSTAT_POIS_DURATION, 15, 5);
    SetPerkStat(PERK_TORM_ACRIMONY, PSTAT_DOTMULTI_POISON, 5, 5, PERKF_AFFECTSDAMAGE);

    // Cold. "Effect of cold ailments" is the chill slow, which is what PSTAT_SLOWEFFECT scales.
    SetPerkStat(PERK_TORM_BREATHOFRIME, PSTAT_SLOWEFFECT, 20, 10);
    SetPerkStat(PERK_TORM_BREATHOFRIME, PSTAT_PCTDMG_BASE + DND_DAMAGECATEGORY_ICE, 25, 15, PERKF_AFFECTSDAMAGE);

    // Fire. Prolif range is a FIXED POINT FRACTION of DND_BASE_IGNITEPROLIFRANGE, which is 128 --
    // so the note's "a further 96 units" is 0.75, not 96.
    SetPerkStat(PERK_TORM_COOKEDALIVE, PSTAT_IGN_CHANCE_FLAT, 5, 5);
    SetPerkStat(PERK_TORM_WILDFIRE, PSTAT_IGN_PROLIF_CHANCE_FLAT, 10, 0);
    SetPerkStat(PERK_TORM_WILDFIRE, PSTAT_IGN_PROLIF_RANGE, 0, 0.75);

    // Overload. Duration is fixed point SECONDS, damage increase is a fixed point percent.
    SetPerkStat(PERK_TORM_SHOCKER, PSTAT_OVERLOAD_DURATION, 1.0, 1.0);
    SetPerkStat(PERK_TORM_COURSINGCURRENT, PSTAT_OVERLOAD_ZAPCOUNT, 1, 1);
    SetPerkStat(PERK_TORM_MAXIMUMOVERDRIVE, PSTAT_OVERLOAD_DMGINCREASE, 0.1, 0.1, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_TORM_POWERUP, PSTAT_POWERCHARGE_ONOVERLOAD, 5, 3);

    // PSTAT_INC_ALLOVERLOAD is what lets ice/fire/poison tics overload at all, which is the whole
    // perk. The slot doubles as a PENALTY on overload damage at the same magnitude, so this is 1
    // and not a rounder number -- the perk is not supposed to weaken the overload it unlocks.
    SetPerkStat(PERK_TORM_SUPERCONDUCTOR, PSTAT_INC_ALLOVERLOAD, 1);

    // Bleed
    SetPerkStat(PERK_TORM_WOUNDER, PSTAT_BLEED_CHANCE, 5, 5);
    SetPerkStat(PERK_TORM_BLOODLETTING, PSTAT_BLEED_DMG_PCT, 10, 10, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_TORM_SAVAGEWOUNDS, PSTAT_DOTMULTI_BLEED, 5, 5, PERKF_AFFECTSDAMAGE);

    SetPerkStat(PERK_TORM_STUDENTOFDECAY, PSTAT_POIS_DMG_PCT, 10, 10, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_TORM_DEEPFRIED, PSTAT_IGN_DMG_FLAT, 10, 5, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_TORM_SLOWCOOKER, PSTAT_IGN_DURATION_FLAT, 0.5, 0.25);
    SetPerkStat(PERK_TORM_BITTERFROST, PSTAT_CHILL_CHANCE_FLAT, 5, 5);
    SetPerkStat(PERK_TORM_LINGERINGCOLD, PSTAT_CHILL_DURATION, 10, 10);
    SetPerkStat(PERK_TORM_FLASHFREEZE, PSTAT_FREEZE_CHANCE_FLAT, 4, 3);
    SetPerkStat(PERK_TORM_CRIPPLINGICE, PSTAT_FREEZE_DURATION, 0.5, 0.25);
    SetPerkStat(PERK_TORM_SWIFTDRAIN, PSTAT_BLEEDRATE, 4, 3);

    SetPerkStat(PERK_TORM_DEATHSGRIP, PSTAT_FRENZY_ONMAXPOISON, 5, 5);
    SetPerkStat(PERK_TORM_BLOWBACK, PSTAT_IGN_TICRATE, 5, 5, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_TORM_FLOWOFLIFE, PSTAT_REGEN_ONPOISONKILL, 8, 4);
    SetPerkStat(PERK_TORM_CORROSION, PSTAT_POISON_RESISTSHRED, 1, 1, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_TORM_PERMAFROST, PSTAT_PERMAFROST, 1, -0x7FFFFFFF, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_TORM_CREMATOR, PSTAT_CREMATOR, 1);
    SetPerkStat(PERK_TORM_SEPTICTOUCH, PSTAT_SEPTIC_POISONSHARE, 10, 5);
    SetPerkStat(PERK_TORM_AVATAROFFIRE, PSTAT_AVATAROFFIRE, 50, -0x7FFFFFFF, PERKF_AFFECTSDAMAGE);

    // 4 seconds, then 2 more a point, in tics
    SetPerkStat(PERK_TORM_BRINGEROFICE, PSTAT_COLDIMMUNE_TICS, 140, 70);
    SetPerkStat(PERK_TORM_MUSCLESPASMS, PSTAT_OVERLOAD_DMGREDUCE, 4, 3);

    // 2 seconds then half a second a point, in tics -- StunDurationCounter counts down one a tic.
    SetPerkStat(PERK_TORM_JOLT, PSTAT_OVERLOAD_STUNCHANCE, 15, 5);
    SetPerkStat(PERK_TORM_JOLT, PSTAT_OVERLOAD_STUNTICS, 70, 17);

    // "Increased" crit chance is a multiplier here, so it is a plain integer percent; the crit
    // multi half is added points, and only the first point gives it.
    SetPerkStat(PERK_TORM_CORNEREDPREY, PSTAT_CRITCHANCE_VS_BLEEDING, 0.4, 0.4, PERKF_AFFECTSDAMAGE);
    SetPerkStat(PERK_TORM_CORNEREDPREY, PSTAT_CRITDAMAGE_VS_BLEEDING, 20, 0);

    SetPerkStat(PERK_TORM_MASTEROFWOUNDS, PSTAT_BLEED_AGGRAVATECHANCE, 10, -0x7FFFFFFF, PERKF_AFFECTSDAMAGE);

}

// Generated from Perk_Notes/. Structural fields only -- archetype, category, point cap, threshold
// and requirements all come straight from the notes. pstat/per_point/flags are phase 3 and are set
// by SetPerkStat below, which nothing calls yet.
void SetupPerkTable() {
    // ================= Acrobacy =================

    // -- category 0 --
    DefinePerk(PERK_ACRO_TACTICALDASH, PERK_ARCH_ACRO, 0, 3);
    DefinePerk(PERK_ACRO_KINETICPADS, PERK_ARCH_ACRO, 0, 3);
    SetPerkReq(PERK_ACRO_KINETICPADS, PERK_REQ_ALL, PERK_ACRO_TACTICALDASH, 1);
    DefinePerk(PERK_ACRO_SWIFTREFLEXES, PERK_ARCH_ACRO, 0, 3);
    SetPerkReq(PERK_ACRO_SWIFTREFLEXES, PERK_REQ_ALL, PERK_ACRO_TACTICALDASH, 1);
    DefinePerk(PERK_ACRO_ENHANCEDJOINTS, PERK_ARCH_ACRO, 0, 1, 5);
    SetPerkReq(PERK_ACRO_ENHANCEDJOINTS, PERK_REQ_ALL, PERK_ACRO_TACTICALDASH, 1);
    DefinePerk(PERK_ACRO_EVASIVEMANEUVERS, PERK_ARCH_ACRO, 0, 3, 9);
    SetPerkReq(PERK_ACRO_EVASIVEMANEUVERS, PERK_REQ_ALL, PERK_ACRO_TACTICALDASH, 1, PERK_ACRO_ENHANCEDJOINTS, 1);
    DefinePerk(PERK_ACRO_CRASHCOURSE, PERK_ARCH_ACRO, 0, 3, 9);
    SetPerkReq(PERK_ACRO_CRASHCOURSE, PERK_REQ_ALL, PERK_ACRO_TACTICALDASH, 1, PERK_ACRO_KINETICPADS, 1);
    DefinePerk(PERK_ACRO_UNENDINGRUSH, PERK_ARCH_ACRO, 0, 1, 12);
    SetPerkReq(PERK_ACRO_UNENDINGRUSH, PERK_REQ_ALL, PERK_ACRO_CRASHCOURSE, 1);

    // -- category 1 --
    DefinePerk(PERK_ACRO_THUMPER, PERK_ARCH_ACRO, 1, 3);
    DefinePerk(PERK_ACRO_HEADSTART, PERK_ARCH_ACRO, 1, 3);
    SetPerkReq(PERK_ACRO_HEADSTART, PERK_REQ_ALL, PERK_ACRO_THUMPER, 1);
    DefinePerk(PERK_ACRO_ALLSHAKINGPRESENCE, PERK_ARCH_ACRO, 1, 3, 5);
    SetPerkReq(PERK_ACRO_ALLSHAKINGPRESENCE, PERK_REQ_ALL, PERK_ACRO_THUMPER, 1);
    DefinePerk(PERK_ACRO_ADRENALINE, PERK_ARCH_ACRO, 1, 1, 9);
    SetPerkReq(PERK_ACRO_ADRENALINE, PERK_REQ_ALL, PERK_ACRO_ALLSHAKINGPRESENCE, 1);

    // -- category 2 --
    DefinePerk(PERK_ACRO_AUGMENTEDFOOTPADS, PERK_ARCH_ACRO, 2, 3);
    DefinePerk(PERK_ACRO_TAILWIND, PERK_ARCH_ACRO, 2, 3);
    SetPerkReq(PERK_ACRO_TAILWIND, PERK_REQ_ALL, PERK_ACRO_AUGMENTEDFOOTPADS, 1);
    DefinePerk(PERK_ACRO_NIMBLENESS, PERK_ARCH_ACRO, 2, 3, 5);
    SetPerkReq(PERK_ACRO_NIMBLENESS, PERK_REQ_ALL, PERK_ACRO_AUGMENTEDFOOTPADS, 1);

    // ================= Assassination =================

    // -- category 0 --
    DefinePerk(PERK_ASSN_BACKSTAB, PERK_ARCH_ASSN, 0, 3);
    DefinePerk(PERK_ASSN_DEADLINESS, PERK_ARCH_ASSN, 0, 3);
    DefinePerk(PERK_ASSN_PREPARATION, PERK_ARCH_ASSN, 0, 1, 3);
    SetPerkReq(PERK_ASSN_PREPARATION, PERK_REQ_ANY, PERK_ASSN_BACKSTAB, 1, PERK_ASSN_DEADLINESS, 1);
    DefinePerk(PERK_ASSN_BIGGAMEHUNTER, PERK_ARCH_ASSN, 0, 3, 3);
    SetPerkReq(PERK_ASSN_BIGGAMEHUNTER, PERK_REQ_ANY, PERK_ASSN_BACKSTAB, 1, PERK_ASSN_DEADLINESS, 1);
    DefinePerk(PERK_ASSN_BANEOFLEGENDS, PERK_ARCH_ASSN, 0, 1, 9);
    DefinePerk(PERK_ASSN_OPENINGSALVO, PERK_ARCH_ASSN, 0, 3, 12);
    DefinePerk(PERK_ASSN_ERADICATION, PERK_ARCH_ASSN, 0, 1, 12);

    // -- category 1 --
    DefinePerk(PERK_ASSN_QUICKGETAWAY, PERK_ARCH_ASSN, 1, 3);
    DefinePerk(PERK_ASSN_MASTEROFSHADOWS, PERK_ARCH_ASSN, 1, 3);
    DefinePerk(PERK_ASSN_PLANNEDEXECUTION, PERK_ARCH_ASSN, 1, 1, 5);
    SetPerkReq(PERK_ASSN_PLANNEDEXECUTION, PERK_REQ_ANY, PERK_ASSN_QUICKGETAWAY, 1, PERK_ASSN_MASTEROFSHADOWS, 1);
    DefinePerk(PERK_ASSN_STEADYSHOT, PERK_ARCH_ASSN, 1, 3, 5);
    SetPerkReq(PERK_ASSN_STEADYSHOT, PERK_REQ_ANY, PERK_ASSN_QUICKGETAWAY, 1, PERK_ASSN_MASTEROFSHADOWS, 1);
    DefinePerk(PERK_ASSN_PRESSUREPOINTS, PERK_ARCH_ASSN, 1, 3, 8);
    SetPerkReq(PERK_ASSN_PRESSUREPOINTS, PERK_REQ_ANY, PERK_ASSN_QUICKGETAWAY, 1, PERK_ASSN_MASTEROFSHADOWS, 1);
    DefinePerk(PERK_ASSN_MERCYKILL, PERK_ARCH_ASSN, 1, 3, 12);
    DefinePerk(PERK_ASSN_DANCEWITHDEATH, PERK_ARCH_ASSN, 1, 3, 12);

    // ================= Cunning =================

    // -- category 0 --
    DefinePerk(PERK_CUN_POTENTSALVE, PERK_ARCH_CUN, 0, 3);
    DefinePerk(PERK_CUN_EFFICIENTRECYCLING, PERK_ARCH_CUN, 0, 3);
    DefinePerk(PERK_CUN_ENRICHEDMINERALS, PERK_ARCH_CUN, 0, 2, 6);
    SetPerkReq(PERK_CUN_ENRICHEDMINERALS, PERK_REQ_ANY, PERK_CUN_POTENTSALVE, 1, PERK_CUN_EFFICIENTRECYCLING, 1);
    DefinePerk(PERK_CUN_SURGINGVITALITY, PERK_ARCH_CUN, 0, 2, 6);
    SetPerkReq(PERK_CUN_SURGINGVITALITY, PERK_REQ_ANY, PERK_CUN_POTENTSALVE, 1, PERK_CUN_EFFICIENTRECYCLING, 1);
    DefinePerk(PERK_CUN_DESPERATEMEASURES, PERK_ARCH_CUN, 0, 1, 8);
    DefinePerk(PERK_CUN_OVERFLOWINGRESERVES, PERK_ARCH_CUN, 0, 2, 8);
    SetPerkReq(PERK_CUN_OVERFLOWINGRESERVES, PERK_REQ_ANY, PERK_CUN_POTENTSALVE, 1, PERK_CUN_EFFICIENTRECYCLING, 1);
    DefinePerk(PERK_CUN_SPIKEDCONCOCTION, PERK_ARCH_CUN, 0, 3, 12);

    // -- category 1 --
    DefinePerk(PERK_CUN_DEEPWISDOM, PERK_ARCH_CUN, 1, 1, 9);
    DefinePerk(PERK_CUN_ENDLESSGREED, PERK_ARCH_CUN, 1, 1, 9);

    // -- category 2 --
    DefinePerk(PERK_CUN_ENDURINGCOMPOSURE, PERK_ARCH_CUN, 2, 3, 0, PERKTAG_CHARGE);
    DefinePerk(PERK_CUN_LASTINGFRENZY, PERK_ARCH_CUN, 2, 3, 0, PERKTAG_CHARGE);
    DefinePerk(PERK_CUN_ASTOUNDINGPOWER, PERK_ARCH_CUN, 2, 3, 0, PERKTAG_CHARGE);
    DefinePerk(PERK_CUN_DISCIPLEOFTHEUNYIELDING, PERK_ARCH_CUN, 2, 1, 9);
    SetPerkReqTag(PERK_CUN_DISCIPLEOFTHEUNYIELDING, PERKTAG_CHARGE);
    DefinePerk(PERK_CUN_DISCIPLEOFTHESLAUGHTER, PERK_ARCH_CUN, 2, 1, 9);
    SetPerkReqTag(PERK_CUN_DISCIPLEOFTHESLAUGHTER, PERKTAG_CHARGE);
    DefinePerk(PERK_CUN_DISCIPLEOFTHEFORBIDDEN, PERK_ARCH_CUN, 2, 1, 9);
    SetPerkReqTag(PERK_CUN_DISCIPLEOFTHEFORBIDDEN, PERKTAG_CHARGE);
    DefinePerk(PERK_CUN_MASTEROFRESILIENCE, PERK_ARCH_CUN, 2, 1, 12);
    SetPerkReqTag(PERK_CUN_MASTEROFRESILIENCE, PERKTAG_CHARGE);
    DefinePerk(PERK_CUN_MASTEROFFEROCITY, PERK_ARCH_CUN, 2, 1, 12);
    SetPerkReqTag(PERK_CUN_MASTEROFFEROCITY, PERKTAG_CHARGE);
    DefinePerk(PERK_CUN_MASTEROFTHEARCANE, PERK_ARCH_CUN, 2, 1, 12);
    SetPerkReqTag(PERK_CUN_MASTEROFTHEARCANE, PERKTAG_CHARGE);

    // ================= Endurance =================

    // -- category 0 --
    DefinePerk(PERK_END_PHYSICIAN, PERK_ARCH_END, 0, 3);
    DefinePerk(PERK_END_UNWAVERINGSTANCE, PERK_ARCH_END, 0, 3);
    DefinePerk(PERK_END_MEDIC, PERK_ARCH_END, 0, 2, 3);
    DefinePerk(PERK_END_VIGOUR, PERK_ARCH_END, 0, 3, 6);
    DefinePerk(PERK_END_RESTLESSVITALITY, PERK_ARCH_END, 0, 1, 9);
    SetPerkReq(PERK_END_RESTLESSVITALITY, PERK_REQ_ANY, PERK_END_PHYSICIAN, 1, PERK_END_VIGOUR, 1);
    DefinePerk(PERK_END_INDOMITABLERESOLVE, PERK_ARCH_END, 0, 1, 12);
    SetPerkReq(PERK_END_INDOMITABLERESOLVE, PERK_REQ_ANY, PERK_END_PHYSICIAN, 1, PERK_END_VIGOUR, 1);

    // -- category 1 --
    DefinePerk(PERK_END_CAMOUFLAGE, PERK_ARCH_END, 1, 3, 3);
    DefinePerk(PERK_END_DENSEEXOSKELETON, PERK_ARCH_END, 1, 3, 6);
    DefinePerk(PERK_END_STONESKIN, PERK_ARCH_END, 1, 2, 8);
    DefinePerk(PERK_END_CRYSTALSKIN, PERK_ARCH_END, 1, 2, 12);

    // -- category 2 --
    DefinePerk(PERK_END_ENHANCEDIRON, PERK_ARCH_END, 2, 3);
    DefinePerk(PERK_END_FINESSE, PERK_ARCH_END, 2, 3);
    DefinePerk(PERK_END_SHIELDING, PERK_ARCH_END, 2, 3);
    DefinePerk(PERK_END_PERFECTSHELL, PERK_ARCH_END, 2, 2, 9);
    SetPerkReq(PERK_END_PERFECTSHELL, PERK_REQ_ALL, PERK_END_ENHANCEDIRON, 1);
    DefinePerk(PERK_END_WINDDANCER, PERK_ARCH_END, 2, 2, 9);
    SetPerkReq(PERK_END_WINDDANCER, PERK_REQ_ALL, PERK_END_FINESSE, 1);
    DefinePerk(PERK_END_HIDDENRESERVE, PERK_ARCH_END, 2, 2, 9);
    SetPerkReq(PERK_END_HIDDENRESERVE, PERK_REQ_ALL, PERK_END_SHIELDING, 1);

    // ================= Martialist =================

    // -- category 0 --
    DefinePerk(PERK_MART_RAMPINGASSAULT, PERK_ARCH_MART, 0, 3);
    DefinePerk(PERK_MART_CRANIUMBASH, PERK_ARCH_MART, 0, 3);
    DefinePerk(PERK_MART_ECHOINGSTRIKES, PERK_ARCH_MART, 0, 2, 5);
    SetPerkReq(PERK_MART_ECHOINGSTRIKES, PERK_REQ_ANY, PERK_MART_RAMPINGASSAULT, 1, PERK_MART_CRANIUMBASH, 1);
    DefinePerk(PERK_MART_UNENDINGFURY, PERK_ARCH_MART, 0, 2, 10);
    SetPerkReq(PERK_MART_UNENDINGFURY, PERK_REQ_ALL, PERK_MART_RAMPINGASSAULT, 1);
    DefinePerk(PERK_MART_FERVENTREACH, PERK_ARCH_MART, 0, 2, 12);
    SetPerkReq(PERK_MART_FERVENTREACH, PERK_REQ_ALL, PERK_MART_ECHOINGSTRIKES, 1);
    DefinePerk(PERK_MART_BLADEMASTER, PERK_ARCH_MART, 0, 1, 12);
    SetPerkReq(PERK_MART_BLADEMASTER, PERK_REQ_ANY, PERK_MART_DEEPCUTS, 1, PERK_MART_RAMPINGASSAULT, 1, PERK_MART_CRANIUMBASH, 1);

    // -- category 1 --
    DefinePerk(PERK_MART_EXHAUSTER, PERK_ARCH_MART, 1, 3);
    DefinePerk(PERK_MART_DEEPCUTS, PERK_ARCH_MART, 1, 3);
    DefinePerk(PERK_MART_QUICKDEADLY, PERK_ARCH_MART, 1, 1, 8);
    SetPerkReq(PERK_MART_QUICKDEADLY, PERK_REQ_ALL, PERK_MART_DEEPCUTS, 1);
    DefinePerk(PERK_MART_SWIFTPRECISE, PERK_ARCH_MART, 1, 1, 8);
    DefinePerk(PERK_MART_FLESHCARVER, PERK_ARCH_MART, 1, 2, 12);
    SetPerkReq(PERK_MART_FLESHCARVER, PERK_REQ_ALL, PERK_MART_DEEPCUTS, 1);

    // -- category 2 --
    DefinePerk(PERK_MART_FLASHPARRY, PERK_ARCH_MART, 2, 3);
    DefinePerk(PERK_MART_RIPOSTE, PERK_ARCH_MART, 2, 1, 3);
    DefinePerk(PERK_MART_EXPOSEWEAKNESS, PERK_ARCH_MART, 2, 2, 8);
    DefinePerk(PERK_MART_MARTIALFINESSE, PERK_ARCH_MART, 2, 2, 12);
    SetPerkReq(PERK_MART_MARTIALFINESSE, PERK_REQ_ALL, PERK_MART_EXPOSEWEAKNESS, 1);
    DefinePerk(PERK_MART_GRATUITOUSVIOLENCE, PERK_ARCH_MART, 2, 1, 12);
    SetPerkReq(PERK_MART_GRATUITOUSVIOLENCE, PERK_REQ_ANY, PERK_MART_EXPOSEWEAKNESS, 1, PERK_MART_FERVENTREACH, 1, PERK_MART_SWIFTPRECISE, 1);

    // ================= Perception =================

    // -- category 0 --
    DefinePerk(PERK_PERC_MUNITIONIST, PERK_ARCH_PERC, 0, 3);
    DefinePerk(PERK_PERC_PLANB, PERK_ARCH_PERC, 0, 3);
    DefinePerk(PERK_PERC_EXCESSCONVERSION, PERK_ARCH_PERC, 0, 2, 6);
    DefinePerk(PERK_PERC_FRESHCLIP, PERK_ARCH_PERC, 0, 3, 6);
    SetPerkReq(PERK_PERC_FRESHCLIP, PERK_REQ_ANY, PERK_PERC_MUNITIONIST, 1, PERK_PERC_PLANB, 1);
    DefinePerk(PERK_PERC_LUCKYBULLET, PERK_ARCH_PERC, 0, 2, 9);
    SetPerkReq(PERK_PERC_LUCKYBULLET, PERK_REQ_ANY, PERK_PERC_MUNITIONIST, 1, PERK_PERC_PLANB, 1);
    DefinePerk(PERK_PERC_SALVAGER, PERK_ARCH_PERC, 0, 1, 12);
    DefinePerk(PERK_PERC_SLEIGHTOFHAND, PERK_ARCH_PERC, 0, 1, 12);
    SetPerkReq(PERK_PERC_SALVAGER, PERK_REQ_ANY, PERK_PERC_MUNITIONIST, 1, PERK_PERC_PLANB, 1);

    // -- category 1 --
    DefinePerk(PERK_PERC_PRECISETECHNIQUE, PERK_ARCH_PERC, 1, 3);
    DefinePerk(PERK_PERC_FIREDUP, PERK_ARCH_PERC, 1, 3);
    DefinePerk(PERK_PERC_PUMPED, PERK_ARCH_PERC, 1, 3);
    DefinePerk(PERK_PERC_QUAKER, PERK_ARCH_PERC, 1, 3);
    DefinePerk(PERK_PERC_DARKARTS, PERK_ARCH_PERC, 1, 3);
    DefinePerk(PERK_PERC_SHARPSHOOTER, PERK_ARCH_PERC, 1, 1, 6);
    SetPerkReq(PERK_PERC_SHARPSHOOTER, PERK_REQ_ALL, PERK_PERC_PRECISETECHNIQUE, 1);
    DefinePerk(PERK_PERC_CEASELESSASSAULT, PERK_ARCH_PERC, 1, 1, 6);
    SetPerkReq(PERK_PERC_CEASELESSASSAULT, PERK_REQ_ALL, PERK_PERC_FIREDUP, 1);
    DefinePerk(PERK_PERC_BLASTIERSHOTS, PERK_ARCH_PERC, 1, 1, 6);
    SetPerkReq(PERK_PERC_BLASTIERSHOTS, PERK_REQ_ALL, PERK_PERC_PUMPED, 1);
    DefinePerk(PERK_PERC_EARTHSHAKER, PERK_ARCH_PERC, 1, 1, 6);
    SetPerkReq(PERK_PERC_EARTHSHAKER, PERK_REQ_ALL, PERK_PERC_QUAKER, 1);
    DefinePerk(PERK_PERC_ESSENCETHEFT, PERK_ARCH_PERC, 1, 1, 6);
    SetPerkReq(PERK_PERC_ESSENCETHEFT, PERK_REQ_ALL, PERK_PERC_DARKARTS, 1);

    // -- category 2 --
    DefinePerk(PERK_PERC_BASTIONBREAKER, PERK_ARCH_PERC, 2, 3);
    DefinePerk(PERK_PERC_SHIELDSTEALER, PERK_ARCH_PERC, 2, 1, 6);
    SetPerkReq(PERK_PERC_SHIELDSTEALER, PERK_REQ_ALL, PERK_PERC_BASTIONBREAKER, 1);
    DefinePerk(PERK_PERC_UNSTOPPABLEFORCE, PERK_ARCH_PERC, 2, 3, 9);
    DefinePerk(PERK_PERC_HEATSINKS, PERK_ARCH_PERC, 2, 3, 3);
    DefinePerk(PERK_PERC_EMERGENCYPROTOCOL, PERK_ARCH_PERC, 2, 1, 9);

    // ================= Tormentor =================

    // -- category 0: poison --
    DefinePerk(PERK_TORM_STUDENTOFDECAY, PERK_ARCH_TORM, 0, 3);
    DefinePerk(PERK_TORM_PROLONGEDTORTURE, PERK_ARCH_TORM, 0, 3);
    DefinePerk(PERK_TORM_DEATHSGRIP, PERK_ARCH_TORM, 0, 3, 6);
    SetPerkReq(PERK_TORM_DEATHSGRIP, PERK_REQ_ANY, PERK_TORM_STUDENTOFDECAY, 1, PERK_TORM_PROLONGEDTORTURE, 1);
    DefinePerk(PERK_TORM_FLOWOFLIFE, PERK_ARCH_TORM, 0, 3, 6);
    SetPerkReq(PERK_TORM_FLOWOFLIFE, PERK_REQ_ANY, PERK_TORM_STUDENTOFDECAY, 1, PERK_TORM_PROLONGEDTORTURE, 1);
    DefinePerk(PERK_TORM_CORROSION, PERK_ARCH_TORM, 0, 2, 9);
    SetPerkReq(PERK_TORM_CORROSION, PERK_REQ_ANY, PERK_TORM_DEATHSGRIP, 1, PERK_TORM_FLOWOFLIFE, 1);
    DefinePerk(PERK_TORM_ACRIMONY, PERK_ARCH_TORM, 0, 2, 9);
    SetPerkReq(PERK_TORM_ACRIMONY, PERK_REQ_ANY, PERK_TORM_DEATHSGRIP, 1, PERK_TORM_FLOWOFLIFE, 1);
    DefinePerk(PERK_TORM_SEPTICTOUCH, PERK_ARCH_TORM, 0, 2, 12);

    // -- category 1: cold --
    DefinePerk(PERK_TORM_BITTERFROST, PERK_ARCH_TORM, 1, 3);
    DefinePerk(PERK_TORM_LINGERINGCOLD, PERK_ARCH_TORM, 1, 3);
    DefinePerk(PERK_TORM_FLASHFREEZE, PERK_ARCH_TORM, 1, 3, 6);
    SetPerkReq(PERK_TORM_FLASHFREEZE, PERK_REQ_ANY, PERK_TORM_BITTERFROST, 1, PERK_TORM_LINGERINGCOLD, 1);
    DefinePerk(PERK_TORM_CRIPPLINGICE, PERK_ARCH_TORM, 1, 3, 6);
    SetPerkReq(PERK_TORM_CRIPPLINGICE, PERK_REQ_ANY, PERK_TORM_BITTERFROST, 1, PERK_TORM_LINGERINGCOLD, 1);
    DefinePerk(PERK_TORM_BRINGEROFICE, PERK_ARCH_TORM, 1, 2, 9);
    SetPerkReq(PERK_TORM_BRINGEROFICE, PERK_REQ_ANY, PERK_TORM_CRIPPLINGICE, 1, PERK_TORM_FLASHFREEZE, 1);
    DefinePerk(PERK_TORM_BREATHOFRIME, PERK_ARCH_TORM, 1, 2, 9);
    SetPerkReq(PERK_TORM_BREATHOFRIME, PERK_REQ_ANY, PERK_TORM_CRIPPLINGICE, 1, PERK_TORM_FLASHFREEZE, 1);
    DefinePerk(PERK_TORM_PERMAFROST, PERK_ARCH_TORM, 1, 1, 12);

    // -- category 2: fire --
    DefinePerk(PERK_TORM_SLOWCOOKER, PERK_ARCH_TORM, 2, 3);
    DefinePerk(PERK_TORM_DEEPFRIED, PERK_ARCH_TORM, 2, 3);
    DefinePerk(PERK_TORM_BLOWBACK, PERK_ARCH_TORM, 2, 3, 6);
    SetPerkReq(PERK_TORM_BLOWBACK, PERK_REQ_ANY, PERK_TORM_DEEPFRIED, 1, PERK_TORM_SLOWCOOKER, 1);
    DefinePerk(PERK_TORM_COOKEDALIVE, PERK_ARCH_TORM, 2, 3, 6);
    SetPerkReq(PERK_TORM_COOKEDALIVE, PERK_REQ_ANY, PERK_TORM_DEEPFRIED, 1, PERK_TORM_SLOWCOOKER, 1);
    DefinePerk(PERK_TORM_WILDFIRE, PERK_ARCH_TORM, 2, 2, 9);
    SetPerkReq(PERK_TORM_WILDFIRE, PERK_REQ_ANY, PERK_TORM_BLOWBACK, 1, PERK_TORM_COOKEDALIVE, 1);
    DefinePerk(PERK_TORM_CREMATOR, PERK_ARCH_TORM, 2, 1, 9);
    SetPerkReq(PERK_TORM_CREMATOR, PERK_REQ_ANY, PERK_TORM_BLOWBACK, 1, PERK_TORM_COOKEDALIVE, 1);
    DefinePerk(PERK_TORM_AVATAROFFIRE, PERK_ARCH_TORM, 2, 1, 12);

    // -- category 3: overload --
    DefinePerk(PERK_TORM_SHOCKER, PERK_ARCH_TORM, 3, 3);
    DefinePerk(PERK_TORM_COURSINGCURRENT, PERK_ARCH_TORM, 3, 3);
    DefinePerk(PERK_TORM_MAXIMUMOVERDRIVE, PERK_ARCH_TORM, 3, 3, 6);
    SetPerkReq(PERK_TORM_MAXIMUMOVERDRIVE, PERK_REQ_ANY, PERK_TORM_SHOCKER, 1, PERK_TORM_COURSINGCURRENT, 1);
    DefinePerk(PERK_TORM_MUSCLESPASMS, PERK_ARCH_TORM, 3, 3, 6);
    SetPerkReq(PERK_TORM_MUSCLESPASMS, PERK_REQ_ANY, PERK_TORM_SHOCKER, 1, PERK_TORM_COURSINGCURRENT, 1);
    DefinePerk(PERK_TORM_POWERUP, PERK_ARCH_TORM, 3, 2, 9);
    SetPerkReq(PERK_TORM_POWERUP, PERK_REQ_ANY, PERK_TORM_MAXIMUMOVERDRIVE, 1, PERK_TORM_MUSCLESPASMS, 1);
    DefinePerk(PERK_TORM_JOLT, PERK_ARCH_TORM, 3, 2, 9);
    SetPerkReq(PERK_TORM_JOLT, PERK_REQ_ANY, PERK_TORM_MAXIMUMOVERDRIVE, 1, PERK_TORM_MUSCLESPASMS, 1);
    DefinePerk(PERK_TORM_SUPERCONDUCTOR, PERK_ARCH_TORM, 3, 1, 12);

    // -- category 4: bleed --
    DefinePerk(PERK_TORM_WOUNDER, PERK_ARCH_TORM, 4, 3);
    DefinePerk(PERK_TORM_BLOODLETTING, PERK_ARCH_TORM, 4, 3);
    DefinePerk(PERK_TORM_SWIFTDRAIN, PERK_ARCH_TORM, 4, 3, 6);
    SetPerkReq(PERK_TORM_SWIFTDRAIN, PERK_REQ_ANY, PERK_TORM_WOUNDER, 1, PERK_TORM_BLOODLETTING, 1);
    DefinePerk(PERK_TORM_SAVAGEWOUNDS, PERK_ARCH_TORM, 4, 3, 6);
    SetPerkReq(PERK_TORM_SAVAGEWOUNDS, PERK_REQ_ANY, PERK_TORM_WOUNDER, 1, PERK_TORM_BLOODLETTING, 1);
    DefinePerk(PERK_TORM_CORNEREDPREY, PERK_ARCH_TORM, 4, 2, 9);
    SetPerkReq(PERK_TORM_CORNEREDPREY, PERK_REQ_ANY, PERK_TORM_SWIFTDRAIN, 1, PERK_TORM_SAVAGEWOUNDS, 1);
    DefinePerk(PERK_TORM_MASTEROFWOUNDS, PERK_ARCH_TORM, 4, 1, 12);

    // Both last, and in this order: SetPerkStat refuses to touch a perk DefinePerk has not reached
    // yet, and the menu lists are a read of the finished table.
    SetupPerkStats();
    BuildPerkArchLists();
}
#endif
