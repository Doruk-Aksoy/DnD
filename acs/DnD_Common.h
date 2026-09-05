#ifndef DND_COMMON_IN
#define DND_COMMON_IN

#pragma pointer_space global 20

#include "libbcs.bcs"

#define SIZEOF_INT 4

#define ISDEBUGBUILD
//#define WANTCURSORPOS
#define VERBOSE_TID_SETUP
//#define WANT_BUFF_LOG

// The per-base mod pool coverage report: which bases are thin at a given item level, and the summary
// line under it. Useful when tuning mod level gates, noise on every other startup -- the MOD POOL
// DANGER line that actually matters is not gated by any of this and always prints.
//#define VERBOSE_MODPOOL

// What the perk table built: how many perks landed in each archetype's menu list. The list is what
// the tree page draws from, so an empty one is an empty page.
//#define VERBOSE_PERK_SETUP


#ifdef ISDEBUGBUILD
int test_counter = 0;
#endif

#define SKIP_DB_SETTINGS // skips db setting files, only compile when just wanting to test basic things that don't have to do with settings for db modes
//#define ISAPRILFIRST // enables memes... OH NO

struct ValueComponent_T {
	int additive;
	int multiplicative;
};

// string tables should always follow icon + name if they have both
#define STRING_ICON 0
#define STRING_NAME 1

#define CHAOSMARK_DAMAGEBUFF 50
#define ASMODEUSMARK_DAMAGEBUFF 100

#define MAXLEVELS 100
#define DND_EXP_ADJUST_LEVEL 60
#define DND_EXP_ADJUST_LEVELFACTOR 0.975

#define DND_MAX_MONSTERLVL MAXLEVELS

#define FACTOR_FIXED_RESOLUTION 1000

#define MAXPLAYERS 64
#define MAXPLAYERS_BITS 6 // 6 bits needed to store 0-63
#define MAXPLAYERS_MASK 0x3F
#define TICRATE 35
#define TICRATE_F 35.0
#define TICRATE_TIMESFIVE (35 * 5)
#define HALF_TICRATE 17
#define MAXLOOTBOXES 1024

#define DND_BASE_HEALTH 100

#define DND_PLAYER_WEAKEN_LEVELS 40
#define DND_MONSTER_RESIST_LEVELS 40
#define DND_PLAYER_RESIST_REDUCE -25.0

// moved here for better access everywhere, was necessary for monster resists
//
// THE ORDER IS THE DAMAGE CONVERSION LADDER. A category may only convert into one that sits
// LATER in this list, which is what makes conversion acyclic and lets the resolver run as a
// FIRE SITS LAST OF THE ELEMENTS, immediately before occult, so every category except occult can
// convert into it -- Tormentor / Avatar of Fire is the reason, and a forward-only ladder cannot
// express "everything to fire" any other way. The elemental block stayed CONTIGUOUS across that
// move, which is what keeps DND_ELECATEGORY_BEGIN..END a range check rather than a set.
//
// single ascending pass. Occult is the last rung and converts into nothing; Soul sits past the
// end of the ladder entirely and never converts either way, so the ladder bound is exactly the
// existing DND_DAMAGECATEGORY_END.
//
// Four other declarations are kept PARALLEL to this one and must move with it:
//   DamageTypes.dec      -- the DECORATE-side copy of this same enum
//   BUFF_*DAMAGEDEALT    -- indexed by (category - DND_ELECATEGORY_BEGIN)
//   BTI_ELEMENTPOWER_*   -- both DnD_BuffTable.h and BuffIndex.dec, mapped onto the above by subtraction
//   LANGUAGE.gen DND_TALENT1..8 -- GetTalentTag() builds the lump name from the category index
enum {
	DND_DAMAGECATEGORY_MELEE,
	DND_DAMAGECATEGORY_BULLET,
	DND_DAMAGECATEGORY_ENERGY,
	DND_DAMAGECATEGORY_ICE,
	DND_DAMAGECATEGORY_POISON,
	DND_DAMAGECATEGORY_LIGHTNING,
	DND_DAMAGECATEGORY_FIRE,
	DND_DAMAGECATEGORY_OCCULT,
	DND_DAMAGECATEGORY_SOUL
};
#define DND_DAMAGECONVERSION_BEGIN DND_DAMAGECATEGORY_BULLET
#define DND_DAMAGECONVERSION_END DND_DAMAGECATEGORY_OCCULT
#define DND_ELECATEGORY_BEGIN DND_DAMAGECATEGORY_ICE
#define DND_ELECATEGORY_END DND_DAMAGECATEGORY_FIRE
#define MAX_DAMAGE_CATEGORIES (DND_DAMAGECATEGORY_SOUL + 1)

// both of these are multipliers on the damage that gets through, not subtractions from the resist --
// see ApplyResistWeakness. They mean the same thing at every monster level now, which is why the
// specific one came down from 50: as a flat subtraction it was worth far more the higher the level.
#define DND_WEAKNESS_FACTOR 25 // 25% extra dmg
#define DND_SPECIFICELEWEAKNESS_FACTOR 33 // 33% extra dmg taken from specific elemental dmg
#define DND_RESIST_FACTOR 50 // 50% dmg taken
#define DND_IMMUNITY_FACTOR 95 // 5% dmg taken

// Aura setup -- keep refresh rate below the powerup token's duration!!
#define DND_MONSTERAURA_REFRESHRATE (TICRATE * 2)
#define DND_AURAWARD_RESIST 25 // flat, added to the TRUE resist -- see the read in FactorResists

// A_RadiusGive takes a plain unit count and lives in DECORATE, which cannot see this. Aura_Ward_Pulse
// hardcodes the same number; keep the two in step.
#define DND_AURAWARD_DISTANCE 256
#define DND_IMMUNITY_HARDCAP_FACTOR 99 // 1% damage taken

#define DND_DAMAGECATEGORY_BEGIN DND_DAMAGECATEGORY_MELEE
#define DND_DAMAGECATEGORY_END (DND_DAMAGECATEGORY_OCCULT + 1)

// =================================================================================================
// Dense player stat slots -- PlayerModData.vals[]
//
// PlayerModData used to be two MAX_TOTAL_ATTRIBUTES wide arrays per player: 3000 slots each, for the
// 360 attribute ids that exist, of which only ~250 ever hold a player scoped value. vals[] replaces the
// value side with one dense slot per STAT rather than per mod id.
//
// Item data still stores INV_* ids and always will -- an item has to record which mod it rolled, and
// those ids are persisted. MapAttributeToPStat is the single place that turns an id into a slot and
// it runs at equip time only. NOTHING on a hot path may take an INV_* id: read the slot directly.
//
// Two consequences worth stating, because they are the whole point:
//
//  1. A later mod that is just a new SOURCE of an existing stat needs no slot of its own. It gets a
//     case in MapAttributeToPStat pointing at the slot that already exists, and every formula
//     downstream is untouched. That is what used to force a new id, a new accumulator and an extra
//     term added by hand to whichever expression consumed it.
//  2. Families are laid out as RUNS, so the thing that indexes a family IS its storage index. The
//     added damage run is keyed by DND_DAMAGECATEGORY_*, which is why the pair of mapper functions
//     that used to translate between the two enums could be deleted outright -- the mapping is now
//     the address, and it cannot fall out of step with itself.
//
// Adding a family: append its base before PSTAT_COUNT and give it a run of that family's width.
// The damage resistances a player has. An INDEX, deliberately not an attribute id -- see the rename
// note on ApplyPlayerDamageResist for why the two must never be confused.
enum {
	DND_PRESIST_PHYS,
	DND_PRESIST_MAGIC,
	DND_PRESIST_ENERGY,
	DND_PRESIST_ELEM,
	DND_PRESIST_REFL,

	DND_PRESIST_COUNT
};

// Damage that matched no type flag at all. It takes the untyped terms -- all-resist, unity, the
// level weaken -- and nothing that is keyed on a type.
#define DND_PRESIST_NONE -1

// Resist penetration the player carries. NOT keyed by damage category: physical pen answers for both
// MELEE and BULLET, and ELEMENTAL is a bonus on top of the four elements rather than a category of
// its own, so the two enums do not line up and GetResistPenetration keeps its switch to bridge them.
enum {
	DND_PPEN_PHYS,
	DND_PPEN_ENERGY,
	DND_PPEN_OCCULT,
	DND_PPEN_FIRE,
	DND_PPEN_ICE,
	DND_PPEN_LIGHTNING,
	DND_PPEN_POISON,
	DND_PPEN_ELEMENTAL,
	DND_PPEN_ALL,

	DND_PPEN_COUNT
};

// Chance to avoid an ailment outright. Bleed sits in the same run even though it is not elemental --
// the storage is uniform, the two READERS differ (bleed does not take the all-elemental bonus and
// carries the Punisher condition).
enum {
	DND_PAVOID_IGNITE,
	DND_PAVOID_CHILLFREEZE,
	DND_PAVOID_POISON,
	DND_PAVOID_OVERLOAD,
	DND_PAVOID_BLEED,

	DND_PAVOID_COUNT
};

// Martialist melee riders. The slow magnitude and both durations are flat in the notes -- only the
// chance scales with points -- so they are constants rather than slots.
#define DND_EXHAUSTER_SLOWPCT 20
#define DND_EXHAUSTER_SLOWTIME 3        // seconds
#define DND_CRANIUMBASH_STUNTICS 70     // 2 seconds

// Sector light level at or below which Master of Shadows considers the player to be in the dark.
// Doom light levels run 0..255 and a normally lit room sits around 160..192, so this is genuinely
// unlit rather than merely dim.
#define DND_DARKAREA_LIGHTLEVEL 96

// Charge types. The DND_CHARGE_* enum in DnD_CommonStat.h must stay within this count -- it is
// defined here rather than read from there because that file is included later, and bcs does
// not resolve macro forward references.
#define DND_MAX_CHARGETYPES 3

enum {
	// Flat added damage, one slot per DND_DAMAGECATEGORY_*. Categories with no mod that can fill
	// them (MELEE, SOUL) simply stay zero, which is exactly what the old -1 return meant.
	PSTAT_ADDEDFLAT_BASE = 0,

	// Player resistances, one slot per DND_PRESIST_*.
	PSTAT_RESIST_BASE = PSTAT_ADDEDFLAT_BASE + MAX_DAMAGE_CATEGORIES,
	// "reduced damage taken" applies to every type, so it sits outside the run above.
	PSTAT_RESIST_ALL = PSTAT_RESIST_BASE + DND_PRESIST_COUNT,

	// Resist penetration, one slot per DND_PPEN_*.
	PSTAT_PEN_BASE = PSTAT_RESIST_ALL + 1,

	// Ailment avoidance, one slot per DND_PAVOID_*.
	PSTAT_AVOID_BASE = PSTAT_PEN_BASE + DND_PPEN_COUNT,
	// Bonus to every ELEMENTAL avoidance, so it sits outside the run rather than in it.
	PSTAT_AVOID_ELEALL = PSTAT_AVOID_BASE + DND_PAVOID_COUNT,

	// ---- ailments and damage over time ----------------------------------------------------------
	// Named one per stat rather than laid out as a run: nothing indexes these, they are read by name
	// at every site. Flat and percent are separate slots on purpose -- they enter their formulas at
	// different points (base + flat, then * (100 + pct)) and could never share one.
	PSTAT_IGN_CHANCE_PCT = PSTAT_AVOID_ELEALL + 1,
	PSTAT_IGN_CHANCE_FLAT,
	PSTAT_IGN_DMG,
	PSTAT_IGN_DURATION,
	PSTAT_IGN_PROLIF_CHANCE_PCT,
	PSTAT_IGN_PROLIF_CHANCE_FLAT,
	PSTAT_IGN_PROLIF_COUNT,
	PSTAT_IGN_PROLIF_RANGE,

	PSTAT_POIS_TICRATE,
	PSTAT_POIS_DURATION,
	PSTAT_POIS_TICDMG,

	PSTAT_BLEED_CHANCE,
	PSTAT_BLEED_DMG_PCT,
	PSTAT_BLEED_DURATION,

	PSTAT_FREEZE_CHANCE,
	PSTAT_CHILL_THRESHOLD,

	PSTAT_OVERLOAD_ZAPCOUNT,
	PSTAT_OVERLOAD_DMGINCREASE,
	PSTAT_OVERLOAD_DURATION,

	// Cross-ailment. These are the broadcast mods -- one source, many formulas -- which is exactly
	// why they keep slots of their own instead of being folded into each ailment at equip time.
	PSTAT_DOT_INCREASED,
	PSTAT_DOT_FLAT,
	PSTAT_DOT_DURATION,
	PSTAT_AILMENT_IGNORECHANCE,

	// ---- defense --------------------------------------------------------------------------------
	// Flat and percent are separate slots throughout, same reason as the ailment block: they enter
	// their formulas at different points and could never share one.
	PSTAT_HP_FLAT,
	PSTAT_HP_PCT,
	PSTAT_ARMOR_FLAT,
	PSTAT_ARMOR_PCT,
	PSTAT_SHIELD_FLAT,
	PSTAT_SHIELD_PCT,
	PSTAT_SHIELD_RECHARGEDELAY,
	PSTAT_SHIELD_RECOVERYRATE,

	PSTAT_MIT_CHANCE,
	PSTAT_MIT_EFFECT,

	PSTAT_MAXRESIST_ADDED,
	PSTAT_SELFDMG_RESIST,
	PSTAT_MAGIC_NEGATION,
	PSTAT_ARMOR_DOUBLEDEF,

	// ---- damage bonuses -------------------------------------------------------------------------
	// Two runs keyed BY damage category, which is what lets MapDamageCategoryToFlatBonus and
	// MapDamageCategoryToPercentBonus drop their 30-line switches for three lines of arithmetic.
	// Physical occupies the BULLET slot and answers for MELEE as well, same as the added damage run.
	PSTAT_FLATDMG_BASE,
	PSTAT_PCTDMG_BASE = PSTAT_FLATDMG_BASE + MAX_DAMAGE_CATEGORIES,

	// Broadcasts. One mod feeding all four elements cannot live inside the run -- it is not keyed by
	// category, it applies to a set of them -- so it gets a slot and the readers add it in.
	PSTAT_FLATDMG_ELEM = PSTAT_PCTDMG_BASE + MAX_DAMAGE_CATEGORIES,
	PSTAT_PCTDMG_ELEM,

	// Conditional on the hit being radius damage rather than on its category, so likewise separate.
	PSTAT_FLATDMG_RADIUS,
	PSTAT_PCTDMG_RADIUS,

	// ---- crit, sustain and the misc scalars -----------------------------------------------------
	// Critical strikes.
	PSTAT_CRITCHANCE_INCREASE,
	PSTAT_CRITDAMAGE_INCREASE,
	PSTAT_CRITPERCENT_INCREASE,
	PSTAT_EX_CRITIGNORERESCHANCE,
	PSTAT_EX_MORECRIT_LIGHTNING,
	PSTAT_EX_SWAPFROMMELEECRIT,
	PSTAT_IMP_PRECISIONCRITBONUS,
	PSTAT_INC_CRITFORDOT,
	PSTAT_INC_EXCESSCRIT,
	PSTAT_MELEECRIT_NOTONLOWSTAMINA,

	// Life steal.
	PSTAT_LIFESTEAL,
	PSTAT_LIFESTEAL_CAP,
	PSTAT_LIFESTEAL_DAMAGE,
	PSTAT_LIFESTEAL_RATE,
	PSTAT_LIFESTEAL_RECOVERY,
	PSTAT_INC_INSTANTLIFESTEAL,

	// Accuracy, luck, gain rates, movement and knockback -- one-off scalars with no shared structure,
	// so each keeps the name of the mod that feeds it.
	PSTAT_LUCK_INCREASE,
	PSTAT_ACCURACY_INCREASE,
	PSTAT_INC_ACCURACYFORPRECISION,
	PSTAT_CREDITGAIN_INCREASE,
	PSTAT_EXPGAIN_INCREASE,
	PSTAT_SPEED_INCREASE,
	PSTAT_PROJSPEED,
	PSTAT_KNOCKBACK_RESIST,
	PSTAT_PERCENT_KNOCKBACKRESIST,

	// ---- ammo and projectiles -------------------------------------------------------------------
	PSTAT_AMMOCAP_INCREASE,
	PSTAT_AMMOGAIN_CHANCE,
	PSTAT_AMMOGAIN_INCREASE,
	PSTAT_EX_AMMOCOSTMULTIPLIER,
	PSTAT_EX_CANNOTPICKAMMO,
	PSTAT_EX_MOREAMMOUSE,
	PSTAT_EX_REDUCEDAMMOCAP,
	PSTAT_EX_REFILLAMMOONMELEEKILL,
	PSTAT_EX_SOULPICKUPSINFAMMO,
	PSTAT_EX_SOULPICKUPSONLYAMMO,
	PSTAT_IMP_AMMOGAIN_SHOTGUNS,
	PSTAT_INC_PROJREVERSE,
	PSTAT_PELLET_INCREASE,
	PSTAT_EX_PICKUPS_MORESOUL,


	// ---- the remainder ---------------------------------------------------------------------------
	// Everything that was still living in the id-keyed value[] array. These have no shared structure
	// worth modelling, so each keeps the name of the mod that feeds it. With these in place nothing
	// reads value[] any more and the array is gone -- see the note on what had to be true first.

	// corruption implicits
	PSTAT_CORR_PERCENTSTATS,
	PSTAT_CORR_CYBERNETIC,
	PSTAT_CORR_DMGDOESNTSTOPREGEN,
	PSTAT_CORR_INSTALEECHPCT,
	PSTAT_CORR_MOREAOE,
	PSTAT_CORR_MAXFRENZY,
	PSTAT_CORR_MAXENDURANCE,
	PSTAT_CORR_MAXPOWER,

	// regular implicits
	PSTAT_IMP_RAVAGER,
	PSTAT_IMP_ABSORBLIGHTNING,
	PSTAT_IMP_KNIGHTARMOR,
	PSTAT_IMP_HANDGUNBONUS,
	PSTAT_IMP_NECROARMOR,
	PSTAT_IMP_LESSLIGHTNINGTAKEN,
	PSTAT_IMP_FASTEROVERHEATDISS,
	PSTAT_IMP_LESSPOISONTAKEN,
	PSTAT_IMP_LESSFIRETAKEN,
	PSTAT_IMP_LESSSELFDAMAGETAKEN,
	PSTAT_IMP_BONUSPETCAP,
	PSTAT_IMP_REDUCEDVISIONIMPAIR,
	PSTAT_IMP_MOREDAMAGETOBOSSES,
	PSTAT_IMP_RECOVERESONUNDEADKILL,
	PSTAT_IMP_PERCENTSTR,
	PSTAT_IMP_PERCENTDEX,
	PSTAT_IMP_PERCENTINT,
	PSTAT_IMP_REDUCEDSLOWSHOTGUNS,
	PSTAT_IMP_ONKILL_FRENZY,
	PSTAT_IMP_ONKILL_ENDURANCE,
	PSTAT_IMP_ONKILL_POWER,
	PSTAT_IMP_PHASINGTIME,
	PSTAT_IMP_STAMINAONKILL,

	// essences
	PSTAT_ESS_VAAJ,
	PSTAT_ESS_SSRATH,
	PSTAT_ESS_OMNISIGHT,
	PSTAT_ESS_OMNISIGHT2,
	PSTAT_ESS_CHEGOVAX,
	PSTAT_ESS_HARKIMONDE,
	PSTAT_ESS_LESHRAC,
	PSTAT_ESS_KRULL,
	PSTAT_ESS_THORAX,
	PSTAT_ESS_ZRAVOG,
	PSTAT_ESS_ERYXIA,

	// incursion mods
	PSTAT_INC_STAMINA,
	PSTAT_INC_STAMINARECOVERYRATE,
	PSTAT_INC_STAMINAGAINED,
	PSTAT_INC_MOREHPBONUS,
	PSTAT_INC_MAXPOISONSTACK,
	PSTAT_INC_POISONSPREAD,
	PSTAT_INC_ALLOVERLOAD,
	PSTAT_INC_HPREGENINTERRUPT,
	PSTAT_INC_PASSIVEREGEN,
	PSTAT_INC_ENEMYRIPCHANCE,
	PSTAT_INC_BLOCKPREVENTION,
	PSTAT_INC_RIPPERSEXPLODE,
	PSTAT_INC_INVERTRESISTANCES,

	// unique mods
	PSTAT_EX_CHANCE_CASTELEMSPELLONATK,
	PSTAT_EX_FACTOR_SMALLCHARM,
	PSTAT_EX_CHANCE_HEALMISSINGONPAIN,
	PSTAT_EX_DMGINCREASE_LIGHTNING,
	PSTAT_EX_SECONDEXPBONUS,
	PSTAT_EX_DOUBLE_HEALTHCAP,
	PSTAT_EX_PHYSDAMAGEPER_FLATHEALTH,
	PSTAT_EX_FORBID_ARMOR,
	PSTAT_EX_CHANCE_ONDEATH_RAISEZOMBIE,
	PSTAT_EX_DMGREDUCE_SHAREWITHPETS,
	PSTAT_EX_DMGINCREASE_TAKEN,
	PSTAT_EX_ONKILL_HEALMISSING,
	PSTAT_EX_SOULWEPS_FULLDAMAGE,
	PSTAT_EX_ABILITY_RALLY,
	PSTAT_EX_BEHAVIOR_SPELLSFULLDAMAGE,
	PSTAT_EX_ABILITY_MONSTERSRIP,
	PSTAT_EX_CURSEIMMUNITY,
	PSTAT_EX_LIMITEDSMALLCHARMS,
	PSTAT_EX_FLATPERSHOTGUNOWNED,
	PSTAT_EX_LESSHEALING,
	PSTAT_EX_SOULWEPSPEN,
	PSTAT_EX_DEADEYEBONUS,
	PSTAT_EX_DAMAGPERMISSINGAMMO,
	PSTAT_EX_UNITY,
	PSTAT_EX_UNITY_RES_BONUS,
	PSTAT_EX_UNITY_PEN_BONUS,
	PSTAT_EX_UNITY_NOBONUS,
	PSTAT_EX_INTBONUSTOMELEE,
	PSTAT_EX_STARTESONDEPLETE,
	PSTAT_EX_ESEXPLOSIONHPDMG,
	PSTAT_EX_ESCHARGE_USEHP,
	PSTAT_EX_HPTOESHIELD,
	PSTAT_EX_ESHIELDFULLABSORB,
	PSTAT_EX_HEALTHATONE,
	PSTAT_EX_RESPERESHIELD,
	PSTAT_EX_ESHIELDONLYBLOCKPCT,
	PSTAT_EX_DAMAGELOWESTTAKENASPHYS,
	PSTAT_EX_DEMONBARRIERS,
	PSTAT_EX_STREXTRABONUSTOMELEE,
	PSTAT_EX_CANFIREOVERHEATED,
	PSTAT_EX_CANTFIRENONOVERHEAT,
	PSTAT_EX_MOREDMGPEROVERHEAT,
	PSTAT_EX_LESSDMGTAKENMAXOVERHEAT,
	PSTAT_EX_WEAPONSUSEHEALTH,
	PSTAT_EX_RIPPERSONETIMEONLY,
	PSTAT_EX_RIPPERSRIPALL,
	PSTAT_EX_MIRROROTHERMEDIUM,
	PSTAT_EX_CHANCEGAINXCHARGE,
	PSTAT_EX_CHARGEDURATIONHALVED,
	PSTAT_EX_MOREDAMAGEPERCHARGE,
	PSTAT_EX_COUNTASHAVINGMAXCHARGEOF,
	PSTAT_EX_IGNITE_SPREADRETAIN,   // Choir of Ashes. percent of the burn a spread hop keeps
	PSTAT_EX_FLATDMG_PERACCURACY,   // Sightless Vigil. flat damage per DND_VIGIL_ACCPER accuracy
	PSTAT_EX_SPREAD_PENALTY,        // Sightless Vigil. percent INCREASED spread, so 200 is x3
	PSTAT_EX_OVERLOAD_STOREDMG,     // Faraday Halo. percent of damage an overloaded enemy banks
	PSTAT_EX_OVERLOAD_SELFTIME,     // Faraday Halo. overload ticks you take on discharging
	PSTAT_EX_POISON_NODECAY_KEEP,   // Crown of Suffering. percent a renewing poison stack keeps
	PSTAT_EX_DASH_CHARGES,          // Vaultstride. dashes bankable past the cooldown
	PSTAT_EX_REDUCED_MOVESPEED,     // Vaultstride. percent SUBTRACTED from movement speed
	PSTAT_EX_SHAREDBUFF_POTENCY,    // Archangel Beacon. percent strength a shared buff copy lands at
	PSTAT_EX_BUFF_SHORTER,          // Archangel Beacon. percent off the wearer's OWN buff durations
	PSTAT_EX_ANCHOR_RESTOREHP,      // Undertow. percent of the anchored health the return gives back
	PSTAT_EX_DASH_COOLDOWN_LONG,    // Undertow. percent ADDED to the dash cooldown
	PSTAT_EX_IGNITETRAIL,           // Emberwake. base damage a trail tick deals

	// everything else (regular rollables)
	PSTAT_FLASKLIFERECOVERYRATE,
	PSTAT_DROPCHANCE_INCREASE,
	PSTAT_MAGAZINE_INCREASE,
	PSTAT_HANDGUN_PERCENT,
	PSTAT_SHOTGUN_PERCENT,
	PSTAT_AUTOMATIC_PERCENT,
	PSTAT_ARTILLERY_PERCENT,
	PSTAT_PRECISION_PERCENT,
	PSTAT_FLAT_HANDGUN,
	PSTAT_FLAT_SHOTGUN,
	PSTAT_FLAT_AUTOMATIC,
	PSTAT_FLAT_ARTILLERY,
	PSTAT_WEAPONAOE_RADIUS,         // only explosions a weapon caused
	PSTAT_ALLAOE_RADIUS,            // every area of effect the player makes
	PSTAT_SHOPSTOCK_INCREASE,
	PSTAT_REGENCAP_INCREASE,
	PSTAT_DAMAGEPERCENT_MORE,
	PSTAT_STAT_STRENGTH,
	PSTAT_STAT_DEXTERITY,
	PSTAT_STAT_INTELLECT,
	PSTAT_CHANCE_IGNORERADIUSIMMUNITY,
	PSTAT_BLOCKERS_MOREDMG,
	PSTAT_SLOWEFFECT,
	PSTAT_OVERLOADCHANCE,
	PSTAT_MELEERANGE,
	PSTAT_MELEEDAMAGE,
	PSTAT_DOTMULTI,
	PSTAT_DOTMULTI_FIRE,
	PSTAT_DOTMULTI_POISON,
	PSTAT_DOTMULTI_BLEED,
	PSTAT_CHARGEDURATION,
	PSTAT_REGENRATE,
	PSTAT_FLAT_TECH,
	PSTAT_FLAT_PRECISION,
	PSTAT_TECH_PERCENT,
	PSTAT_FLAT_MAGIC,
	PSTAT_MAGIC_PERCENT,
	PSTAT_REDUCED_OVERHEAT,
	PSTAT_ITEMRARITY,
	PSTAT_RIPCOUNT,
	PSTAT_RIPDAMAGE,
	PSTAT_LOCKONAREA,
	PSTAT_LOCKONRANGE,
	PSTAT_INCKILLINGSPREE,
	PSTAT_REDUCEDCURSEEFFECT,
	PSTAT_REDUCEDCURSEDURATION,
	PSTAT_FRENZYCHARGE_ONSHATTER,
	PSTAT_ENDURANCECHARGE_ONMELEE,
	PSTAT_POWERCHARGE_ONOVERLOAD,
	PSTAT_MELEESPLASH_NOTONLOWSTAMINA,
	PSTAT_MELEE_ATKCDR,
	PSTAT_CRUSHINGBLOW,
	PSTAT_DEEPCUTS,
	PSTAT_OPENWOUNDS,
	PSTAT_DEADLYSTRIKE,
	PSTAT_REAPINGCLEAVE,
	PSTAT_INCFLASKCHARGEGAINED,

	// Perk-first slots. No item mod feeds either yet, which is fine -- the slot is the contract, not
	// the source. Both are plain integer percent, matching their read sites.
	PSTAT_HEALING_EFFECT,           // Endurance / Medic. amt * (100 + this) / 100 in HandleHealthPickup
	PSTAT_DOT_DMGTAKEN_REDUCE,      // Endurance / Stone Skin. less-multiplier on DOT hits only
	PSTAT_AVOID_HOMING,             // Endurance / Camouflage. percent chance to shake a seeker's lock

	// Conditional slots. Each one is the same magnitude as an existing unconditional slot but only
	// counts when its condition holds, so it cannot share that slot -- the read site applies the gate.
	PSTAT_BLEED_CHANCE_SLASHING,    // Martialist / Deep Cuts. added only for WPROP_SLASHING weapons
	PSTAT_CRITDAMAGE_VS_BOSS,       // Assassination / Big Game Hunter
	PSTAT_CRITCHANCE_VS_ELITE,      // Assassination / Bane of Legends -- FIXED POINT, 0.01 is 1%
	PSTAT_CRITDAMAGE_VS_ELITE,      // Assassination / Bane of Legends

	// "more susceptible to crits" -- a MULTIPLIER on crit chance, not percentage points added to it.
	// Additive would be enormous: crit chance is a fixed point probability where 0.01 is 1%, so a
	// literal +15 points would be a bigger bonus than every crit mod in the game combined.
	PSTAT_CRITCHANCE_VS_FULLLIFE,   // Assassination / Opening Salvo
	PSTAT_CRITCHANCE_VS_LOWLIFE,    // Assassination / Mercy Kill
	PSTAT_SHELLCAP_FLAT,            // Perception / Pumped. flat shells, folded into the base cap

	// Per charge type duration, indexed by DND_CHARGE_*. PSTAT_CHARGEDURATION stays the all-charges
	// slot item mods write; these three are the Cunning perks that name one type each.
	PSTAT_CRITCHANCE_INDARK,        // Assassination / Master of Shadows. multiplier, like the two above
	PSTAT_BLOCK_PIERCE,             // Perception / Bastion Breaker. percent of a blocked hit that lands
	PSTAT_BLEEDRATE_MELEE,          // Martialist / Flesh Carver. percent FASTER, so it shortens the tic
	PSTAT_SLOWCHANCE_MELEE,         // Martialist / Exhauster. percent chance on a melee hit
	PSTAT_STUNCHANCE_BLUNT,         // Martialist / Cranium Bash. percent chance on a blunt melee hit

	// Tormentor. Each is an ailment number that had no slot because no item mod ever named it: the
	// generic PSTAT_DOT_* and PSTAT_*_CHANCE slots are all-ailment or all-DoT, and these perks name
	// one ailment and one half of it each, so they could not share.
	// Avatar of Fire. The share of non-fire damage that reaches fire; the rest is destroyed.
	// NOT fed through conv_raw -- the ladder only walks forward and fire is rung 3, so ice,
	// poison, lightning and occult can never reach it there. See dnd-damage-conversion.
	PSTAT_AVATAROFFIRE,
	PSTAT_SEPTIC_POISONSHARE,       // Septic Touch. percent of the poison tic the cloud carries
	PSTAT_CREMATOR,                 // Cremator. read by DECORATE through DnD Check Cremator
	PSTAT_REGEN_ONPOISONKILL,       // Flow of Life. percent of the REGEN cap, not of max health
	PSTAT_IGN_TICRATE,              // Blowback. percent FASTER, so it shortens the burn tic
	PSTAT_POISON_RESISTSHRED,       // Corrosion. per poison STACK per second, capped
	PSTAT_PERMAFROST,               // Permafrost. resist lost per second chilled, capped
	PSTAT_COLDIMMUNE_TICS,          // Bringer of Ice. tics of self chill/freeze immunity
	PSTAT_FRENZY_ONMAXPOISON,       // Death's Grip. rolled once, when a stack lands ON the cap
	PSTAT_OVERLOAD_DMGREDUCE,       // Muscle Spasms. read off the ATTACKER, in HandlePlayerResists
	PSTAT_OVERLOAD_STUNCHANCE,      // Jolt. first overload only
	PSTAT_OVERLOAD_STUNTICS,        // Jolt. tics, the unit StunDurationCounter counts in
	PSTAT_CRITCHANCE_VS_BLEEDING,   // Cornered Prey. a MULTIPLIER, like the other vs-state crit slots
	PSTAT_CRITDAMAGE_VS_BLEEDING,   // Cornered Prey
	PSTAT_BLEED_AGGRAVATECHANCE,    // Master of Wounds. rolled once per bleed, not per tic
	PSTAT_POIS_DMG_PCT,             // Student of Decay. poison DOT only, unlike PSTAT_DOT_INCREASED
	PSTAT_IGN_DMG_FLAT,             // Deep Fried. flat on the ignite tic, PSTAT_IGN_DMG is percent
	PSTAT_IGN_DURATION_FLAT,        // Slow Cooker. FIXED POINT SECONDS, converted to ignite loops
	PSTAT_CHILL_CHANCE_FLAT,        // Bitter Frost. chills without meeting the health threshold
	PSTAT_CHILL_DURATION,           // Lingering Cold. percent, lengthens the per stack decay
	PSTAT_FREEZE_CHANCE_FLAT,       // Flash Freeze. added points, PSTAT_FREEZE_CHANCE is a multiplier
	PSTAT_FREEZE_DURATION,          // Crippling Ice. FIXED POINT SECONDS, converted to freeze units
	PSTAT_BLEEDRATE,                // Swift Drain. percent FASTER on any weapon, unlike ..._MELEE

	// Acrobacy dash cluster. Tactical Dash grants the dash at all; the rest read the state it leaves.
	PSTAT_DASH_UNLOCK,              // non-zero means the player has a perk dash
	PSTAT_DASH_COOLDOWNREDUCE,      // percent off DND_PERKDASH_COOLDOWN
	PSTAT_DASH_RANGE,               // percent added to the dash impulse
	PSTAT_DASH_LESSDMGTAKEN,        // percent less damage while the perk dash is recharging
	PSTAT_DASH_MELEEBONUS,          // Swift Reflexes. percent, first melee hit after a dash
	PSTAT_DASH_AVOIDCHANCE,         // Evasive Maneuvers. percent to dodge a direct hit after a dash
	PSTAT_DASH_AVOIDTIME,           // Evasive Maneuvers. extra TICS on that window
	PSTAT_DASH_REFRESHONKILL,       // Unending Rush. non-zero means a kill just after a dash refreshes
	PSTAT_DASH_CRASHSTUN,           // Crash Course. stun TICS on a monster the dash runs into

	// Acrobacy fall-impact cluster. Thumper's power is one percent driving BOTH damage and radius,
	// because the notes scale them together and splitting them would let the two drift apart.
	PSTAT_THUMPER_POWER,            // percent of the base shockwave, 0 means no Thumper
	PSTAT_FALL_SPEEDBONUS,          // Head Start. percent movement speed on landing
	PSTAT_FALL_SPEEDTIME,           // Head Start. tics that lasts
	PSTAT_THUMPER_VULN,             // All-shaking Presence. percent more damage taken
	PSTAT_THUMPER_VULNTIME,         // All-shaking Presence. tics that lasts
	PSTAT_ADRENALINE_HEAL,          // Adrenaline. percent of max health on an affected kill

	// Cunning flask cluster.
	PSTAT_FLASK_CLEANSECHANCE,      // Potent Salve. percent to strip an ailment on use
	PSTAT_FLASK_UTILDURATION,       // Enriched Minerals. percent, utility flasks only
	PSTAT_FLASK_FREEUSECHANCE,      // Desperate Measures. percent to spend no charges on low life
	PSTAT_FLASK_RESISTBONUS,        // Surging Vitality. flat resist while a flask effect runs
	PSTAT_FLASK_ENEMYVULN,          // Spiked Concoction. percent more damage taken, per stack
	PSTAT_FLASK_VULNTIME,           // Spiked Concoction. tics a stack survives

	// Perception.
	PSTAT_SHIELDSTEAL_PCT,          // Shield stealer. percent of ES back on killing a blocker
	PSTAT_PRECISION_FALLOFF,        // Sharpshooter. percent MORE damage at maximum range

	// Cunning. Non-zero means the range gate on shared exp/credit does not apply to this player.
	PSTAT_EXPGAIN_ANYRANGE,         // Deep Wisdom
	PSTAT_CREDITGAIN_ANYRANGE,      // Endless Greed

	// Martialist.
	PSTAT_MELEESPLASH_CHANCE,       // Echoing Strikes. percent, joins the existing splash roll
	PSTAT_MELEESPLASH_RANGE,        // Fervent Reach. percent on the splash radius
	PSTAT_MELEEHIT_SPEED,           // Swift & Precise. percent movement speed per enemy hit
	PSTAT_PARRY_RESISTREDUCE,       // Expose Weakness. percent off a parried enemy's resists

	// Assassination, crit-reactive group.
	PSTAT_CRIT_SPEEDBONUS,          // Quick Getaway. percent movement speed on landing a crit
	PSTAT_CRIT_DROUGHTBONUS,        // Preparation. percent MORE crit chance while none has landed
	PSTAT_CRITCHANCE_FROMBEHIND,    // Backstab. FIXED POINT, added when the target faces away
	PSTAT_CRITDAMAGE_PERSTILLSEC,   // Steady Shot. percent per second standing still
	PSTAT_CRIT_EXPOSEPCT,           // Pressure Points. percent more of the crit's own damage type

	// Endurance and Acrobacy, remainder.
	PSTAT_HEAL_CLEANSECHANCE,       // Physician. percent to strip poison and bleed on a heal
	PSTAT_UNMITIGATED_LESSDMG,      // Wind Dancer. percent less damage while nothing was mitigated
	PSTAT_MIT_PERRUNSEC,            // Nimbleness. FIXED POINT mitigation per second running

	// Martialist, remainder.
	PSTAT_PARRY_COSTREDUCE,         // Riposte. percent off the parry stamina cost, per stack
	PSTAT_RAMPING_HITS,             // Ramping Assault. hits needed, counts DOWN with points
	PSTAT_RAMPING_BONUS,            // Ramping Assault. percent more on the qualifying hit

	// Perception.
	PSTAT_PELLET_FLAT_SHOTGUN,      // Blastier Shots. flat pellets, shotguns only

	PSTAT_RAGE_ONOVERKILL,          // Martialist / Gratuitous Violence. rage points per overkill
	PSTAT_ELUSIVE_ONCRIT,           // Assassination / Dance with Death. percent chance on a crit

	// Perception, magazine group. All three key off TakeAmmoFromPlayer, the one choke point every
	// shot passes through.
	PSTAT_EMPTYMAG_SPEED,           // Plan B. percent movement speed on emptying a magazine
	PSTAT_FRESHCLIP_SHOTS,          // Fresh Clip. how many shots a finished reload empowers
	PSTAT_FRESHCLIP_DAMAGE,         // Fresh Clip. percent more on each of them
	PSTAT_LASTROUND_CRIT,           // Lucky Bullet. FIXED POINT crit chance on the last round
	PSTAT_LASTROUND_CRITDMG,        // Lucky Bullet. percent crit multiplier on it

	PSTAT_PARRY_SHOCKDMG,           // Martialist / Flash Parry. flat damage of the shockwave
	PSTAT_PARRY_SHOCKRANGE,         // Martialist / Flash Parry. percent on its travel distance
	PSTAT_TAILWIND_SPEED,           // Acrobacy / Tailwind. percent movement speed on a pass
	PSTAT_TAILWIND_CDREDUCE,        // Acrobacy / Tailwind. seconds off its cooldown

	// Perception, overheat group.
	PSTAT_OVERHEAT_GRACE,           // Emergency Protocol. TICS of firing past full overheat
	PSTAT_HEATSINK_CDREDUCE,        // Heatsinks. seconds off the heatsink's recharge
	PSTAT_AUTO_RESISTSHRED,         // Ceaseless Assault. resist a hit strips, and its own cap
	PSTAT_AUTO_RESISTSHREDCAP,
	PSTAT_MAGICKILL_PEN,            // Essence Theft. magic pierce a kill grants
	PSTAT_MAGICKILL_PENCAP,

	PSTAT_CRITKILL_NODEATHFX,       // Assassination / Eradication
	PSTAT_AILMENT_IMMUNETICS,       // Endurance / Indomitable Resolve

	PSTAT_AMMOTOKEN_CHANCE,         // Perception / Salvager. percent for a monster to drop one
	PSTAT_AMMO_CONVERTRATE,         // Perception / Excess Conversion. percent of the overflow kept
	PSTAT_ARTILLERY_RAMP,           // Perception / Earthshaker. percent per continuous attack,
	PSTAT_ARTILLERY_RAMPCAP,        // to BOTH damage and radius. and how many of them count
	PSTAT_ARTILLERY_RADIUS,         // Perception / Quaker. artillery-only explosion radius percent

	PSTAT_RIPPER_SURVIVECHANCE,     // Perception / Unstoppable Force. percent vs hardened skin
	PSTAT_RIPIMMUNE_TICS,           // Endurance / Dense Exoskeleton. tics of DONTRIP after a rip
	PSTAT_BERSERK_ALLMELEE,         // Martialist / Unending Fury. percent melee damage under berserk
	PSTAT_MELEE_TOPROJECTILES,      // Martialist / Blademaster. non-zero routes melee scaling to projectiles
	PSTAT_SLASH_QUICKDEADLY,        // Martialist / Quick & Deadly. non-zero grants the DECORATE token
	PSTAT_FASTRELOAD,               // Perception / Sleight of Hand. non-zero grants the DECORATE token
	PSTAT_FLASK_REFILLCHANCE,       // Overflowing Reserves. percent on an ailment kill

	PSTAT_CHARGEDURATION_BASE,
	PSTAT_CHARGEDURATION_END = PSTAT_CHARGEDURATION_BASE + DND_MAX_CHARGETYPES - 1,

	PSTAT_COUNT
};

// Which area of effect stat answers for a hit. Weapon sourced gets both terms, non weapon gets
// only the universal one. Values match the argument DECORATE passes to the radius script.
enum {
	DND_AOESRC_WEAPON,
	DND_AOESRC_NONWEAPON
};

// Perk tree sizing. Here rather than in DnD_Perks.h because player_item_mod_data_T is sized by these
// and is declared first -- bcs resolves macros in preprocessor order, so a forward reference silently
// expands to nothing. Everything else about perks lives in DnD_Perks.h.
//
// _NEW is a leftover from the migration -- the old DND_MAX_PERKS is gone, so this can lose the
// suffix whenever someone wants to do the rename.
#define DND_MAX_PERKS_NEW       160     // 135 authored, room to grow
#define DND_PERK_MAXPOINTS      3
#define DND_MAX_PERK_ARCHETYPES 8       // 6 authored + Tormentor + spare
#define DND_MAX_PERK_REQS       3       // Blademaster and Gratuitous Violence each name three
#define DND_PERK_BITS           2       // one lane holds 0..3, exactly DND_PERK_MAXPOINTS
#define DND_PERK_WORDS          ((DND_MAX_PERKS_NEW * DND_PERK_BITS + 31) / 32)

// Returned by MapAttributeToPStat for every mod that still lives in the old value[] array.
#define DND_PSTAT_UNMAPPED -1

// =================================================================================================
// Player boolean flags -- PlayerModData.pflags[] / .pflag_rc[]
//
// Mods whose value is only ever tested for truth. As ints they cost a full slot each; as bits they
// cost 1/32 of one.
//
// Each flag carries a REFCOUNT alongside its bit, and that is not decoration. A bare `|= bit` on
// equip and `&= ~bit` on unequip cannot tell "two items grant Cyber" from "one does", so
// unequipping either one would strip the power off a player still wearing the other. The
// int-valued mods get that right for free through +1/-1; bits have to carry the count explicitly.
enum {
	PFLAG_CYBER,
	PFLAG_ESHIELDBLOCKALL,
	PFLAG_MELEEIGNORESHIELD,
	PFLAG_LOWERREFLECT,

	// Not a power -- a rollable unique mod that happens to be tested for truth at all five of its
	// read sites, four of them on the damage path. Kept readable through ReadPlayerModValue by
	// MapAttributeToPFlag so the exotic stat page can still list it.
	PFLAG_ELEPENHARMONY,

	// Defense booleans. All three roll 1..1 and are tested for truth at every read site.
	//
	// PFLAG_ESHIELD_NOINTERRUPT and PFLAG_ESCHARGE_NOINTERRUPT look like the same stat from two
	// sources and are NOT: DnD.bcs:3978 tests only the charge one and DnD_Stat.h:284 tests only the
	// shield one. Different conditions, so they keep separate bits -- the sharing rule in the note.
	PFLAG_MITIGATION_TO_DODGE,
	PFLAG_ESHIELD_NOINTERRUPT,
	PFLAG_ESCHARGE_NOINTERRUPT,

	// All three roll 1..1 and are tested for truth at every read site.
	PFLAG_LUCKYCRIT,
	PFLAG_ACCURACY_REVERSED,
	PFLAG_KNOCKBACK_IMMUNITY,
	PFLAG_PELLETS_FIRE_CIRCLE,

	// Unique boot immunities, both 1..1 and both tested for truth. Absolute, not a chance, which
	// is why they are flags rather than another PSTAT_AVOID_* slot.
	PFLAG_CANNOTBEFROZEN,
	PFLAG_CANNOTBEIGNITED,

	// Choir of Ashes. The pair is deliberate: chaining without the one-light rule would let a burn
	// walk back into a monster whose script has ended and loop across the map forever.
	PFLAG_IGNITE_CHAINS,
	PFLAG_IGNITE_NOREFRESH,

	// Sightless Vigil. Accuracy keeps every other thing it does -- it just stops being spent on
	// tightening the cone, which is what frees it to be read as damage instead.
	PFLAG_ACCURACY_NOSPREAD,

	// Crown of Suffering. The pack shares a stack pool and a kill empties it. The third piece of
	// the mechanic, how much a renewing stack keeps, carries a rolled value and lives in a stat slot.
	PFLAG_POISON_SHAREDSTACKS,
	PFLAG_POISON_CLEARONKILL,

	// Faraday Halo.
	PFLAG_OVERLOAD_CHAINSTOSELF,
	PFLAG_OVERLOAD_ANYELEMENT,

	// Vaultstride.
	PFLAG_DASH_KILLREFUND,

	// Undertow.
	PFLAG_DASH_ANCHOR,

	// Emberwake.
	PFLAG_TRAIL_SPEEDSCALES,
	PFLAG_TRAIL_BURNSSTILL,

	PFLAG_COUNT
};

// The handful of mods that carry a second number alongside their value -- a weapon id, a charge
// index, a conversion pair. Only 13 of the 360 attributes have one, which is why the extra side
// was the most wasteful half of the old table: a full MAX_TOTAL_ATTRIBUTES array serving 13 mods.
enum {
	PEXTRA_ESS_VAAJ,
	PEXTRA_IMP_ABSORBLIGHTNING,
	PEXTRA_IMP_KNIGHTARMOR,
	PEXTRA_IMP_RAVAGER,
	PEXTRA_IMP_STAMINAONKILL,
	PEXTRA_IMP_UNSTABLECORE,
	PEXTRA_INC_INSTANTLIFESTEAL,
	PEXTRA_INC_MOREHPBONUS,
	PEXTRA_INC_PASSIVEREGEN,
	PEXTRA_EX_CHANCEGAINXCHARGE,
	PEXTRA_EX_CHANCE_HEALMISSINGONPAIN,
	PEXTRA_EX_COUNTASHAVINGMAXCHARGEOF,
	PEXTRA_EX_SECONDEXPBONUS,

	DND_PEXTRA_COUNT
};

// Returned by MapAttributeToPExtra for every mod that carries no extra.
#define DND_PEXTRA_UNMAPPED -1

// Returned by MapAttributeToPFlag for every mod that is not stored as a flag.
#define DND_PFLAG_UNMAPPED -1

#define DND_PFLAG_WORDS ((PFLAG_COUNT + 31) / 32)
// 8 bit counters packed 4 to an int. MAX_ITEMS_EQUIPPABLE is 13, so 255 sources is unreachable.
#define DND_PFLAG_RCWORDS ((PFLAG_COUNT + 3) / 4)

// this dumb number wasted weeks of our time, fuck you dumb number!
//#define DND_DROP_TID 343 // some dumb number

#define EPSILON 0.000001

#define DND_VIEWCHECK_DENSITY 8

#define DND_MONSTERBONUS_PERLVL 20 // 20% increase per level on gains granted by monsters

enum {
	DND_PLAYER_DOOMGUY,
	DND_PLAYER_MARINE,
	DND_PLAYER_HOBO,
	DND_PLAYER_PUNISHER,
	DND_PLAYER_WANDERER,
	DND_PLAYER_CYBORG,
	DND_PLAYER_BERSERKER,
	DND_PLAYER_TRICKSTER
};
#define MAXPLAYERCLASSES (DND_PLAYER_TRICKSTER + 1)

#define DND_PLAYER_RADIUS 16.0
#define DND_PLAYER_RADIUS_INT (DND_PLAYER_RADIUS >> 16)

#define DND_MARINE_SELFEXPLOSIVEREDUCE 25

#define DND_HOBO_SHOTGUNBONUS 30
#define DND_HOBO_SHOTGUNPELLETBONUS 0.1
#define DND_HOBO_SHOTGUNPELLETBONUS_PERLVL 0.15

#define DND_PUNISHER_SPREERECOVER 32 // 32%
#define DND_PUNISHER_SPREEPERBONUS 8

#define PUNISHERPERKID1 7010
#define PUNISHERPERKID2 7011

#define DND_WANDERER_SPELLEFFICIENCY 4
#define DND_WANDERER_COOLDOWNBONUS 4

#define DND_EXP_HUDID_FILL 266
#define DND_EXP_HUDID_BACK 267

#define DND_MAX_MONSTERS 12800
#define DND_MAX_MONSTER_ATTACHMENTS 32

#define DND_MAX_TEMP_PROJ 200
#define DND_MAX_TEMP_PROJ_SMALL 32
#define DND_EMERALD_TIDADD 100
#define DND_TEMPORARY_TIDADD 200
#define DND_AVATAR_CUBESKIP 6000

#define MAX_SHARED_ITEMS 2048
#define SHARED_ITEM_TYPE_TID_MAX 19048

#define LIMITED_RESPAWN_TID_END 29049
#define LIMITED_RESPAWN_MAX 10000

#define THUNDERSTAFF_RING_SKIP 2048 // this is calculated in spawner code, but basically 32 x 64 = 2048

#define MAX_SHOOTABLE_TIDS 4000
#define MAX_NPCS 512
#define MAX_PETS 1600 // max of 1600 pets stored here because => 25 max pets per player x 64 = 1600
#define MAX_PICKUPS 3000

#define DND_BASE_POISON_FACTOR 5

enum {
	// dont change order of these, add below the last!
	STAT_STR,
	STAT_DEX,
	STAT_INT,
	
	// The ten point-spend perks used to sit here, between the attributes and the level, because
	// they shared StatData and its inventory-item storage with them. Retired 2026-08-28 -- the
	// perk tree in DnD_Perks.h keeps its points in PlayerModData instead. Nothing persisted or
	// transmitted a raw STAT_ index, so STAT_LVL sliding from 12 to 3 is safe.

	STAT_LVL
};

#define DND_ATTRIB_BEGIN STAT_STR
#define DND_ATTRIB_END STAT_INT
#define DND_MAX_ATTRIBUTES (DND_ATTRIB_END - DND_ATTRIB_BEGIN + 1)

#define DND_STAT_ATTUNEMENT_GAIN 0.05

#define DND_MAX_INCURSION_MARKERS 1024
#define DND_MAX_INCURSION_PORTALS 256

enum {
	// from 1 to DND_MAX_MONSTERS + 1 is the amount of monsters we support -- we'll do tid - 1 to access array loc
	AUX_INCURSION_PUFF_TID = 1,
	DND_MONSTERTID_BEGIN,
	
	// 12801 - 12864 player tid range
	P_TIDSTART = DND_MONSTERTID_BEGIN + DND_MAX_MONSTERS,
	// emerald death actors add +100 to here, temp weapons use +200
	
	// temporary proj tid for use with scripts firing custom proj
	// also skips emerald death tid add
	DND_TEMP_PROJTID = P_TIDSTART + MAXPLAYERS + DND_TEMPORARY_TIDADD,

	// 16 * (tid + 1) % 100
	VORTEXTIDSTART = DND_TEMP_PROJTID + 4096,
	// reflection accessory, 3 spawns with player tid so 3 * maxplayers
	REFLECTFXTID = VORTEXTIDSTART + 192,
	
	// draugr teleport fx
	DRAUGR_TEMP_FX = REFLECTFXTID + DND_MAX_TEMP_PROJ,

	ZRAVOG_SOUND_TID,
	
	// 64 player temp tid range
	TEMPORARY_PET_TID,
	
	// 64 player temp tid range
	TEMPORARY_SPELL_TID = TEMPORARY_PET_TID + MAXPLAYERS,
	
	// 64 players temp tid range
	TEMPORARY_DATADUMMY_TID = TEMPORARY_SPELL_TID + MAXPLAYERS,

	PROJECTILE_HELPER_TID = TEMPORARY_DATADUMMY_TID + MAXPLAYERS,
	
	// circle attacks from burst attack modifiers
	TEMPORARY_ATTACK_TID = PROJECTILE_HELPER_TID + MAXPLAYERS,
	AUX_PROJ_TID = TEMPORARY_ATTACK_TID + MAXPLAYERS,

	// 64 player temp tid range
	AUX_SIGNAL_TID = AUX_PROJ_TID + MAXPLAYERS,
	
	// 64 player temp tid range
	DND_MENUFLOATYICON_TID = AUX_SIGNAL_TID + MAXPLAYERS,
	
	// 64 player temp tid range
	DEATHRAY_MARKER_TID = DND_MENUFLOATYICON_TID + MAXPLAYERS,
	
	// 64 player temp tid range
	DARKLANCE_TID = DEATHRAY_MARKER_TID + MAXPLAYERS,
	DARKLANCE_SHREDDER = DARKLANCE_TID + MAXPLAYERS,
	
	// 64 player temp tid range
	KANJI_TRAP_TID = DARKLANCE_SHREDDER + MAXPLAYERS,
	KANJI_TRAPEXP_TID = KANJI_TRAP_TID + MAXPLAYERS,
	
	UNIQUE_MON_AUX_TID,
	DND_FROZENFX_TID,
	DND_CHAOSMARKFX_TID,
	DND_OTHERWORDLYGRIP_TID,
	DND_THUNDERSTRUCK_TID,
	DND_AVATAR_CUBEPROJ_TID,
	DND_AVATAR_CUBEFLARE_TID,
	ZEALOT_SHIELD_TID,

	AUX_FX_TID,
	AUX_CHARGEFX_TID,
	AUX_SPAWNER_TID,

	AUX_PARRY_TID,

	WANDERER_AURA_TID = AUX_PARRY_TID + MAXPLAYERS,
	
	AVATAR_SOUL_TID,
	
	// has 6000 skip here, supports max 2000 avatars x 3 = 6000
	AVATAR_CUBE_TID,
	
	// max 2048
	SHARED_ITEM_TID_BEGIN = AVATAR_CUBE_TID + DND_AVATAR_CUBESKIP,
	
	// limited respawn skips MAX_SHARED_ITEMS from above, and has 10k skip for next
	LIMITED_RESPAWN_TID_BEGIN = SHARED_ITEM_TID_BEGIN + MAX_SHARED_ITEMS,
	
	DND_DAMAGENUMBER_TID = LIMITED_RESPAWN_TID_BEGIN + LIMITED_RESPAWN_MAX,
	
	DND_THUNDERSTAFF_DAMAGERTID,
	DND_ICECHUNK_TID,
	DND_BLINDFX_TID,
	DND_NUCLEAREXP_TID,
	DND_UNSTABLEEXP_TID,
	DND_TEMP_PLAYERPROJTID,
	
	// we allocate each player proj tid to their own pnum
	DND_THUNDER_RING_TIDSTART = DND_TEMP_PLAYERPROJTID + 32 * MAXPLAYERS,
	
	DND_SHOOTABLETID_BEGIN = DND_THUNDER_RING_TIDSTART + THUNDERSTAFF_RING_SKIP,
	
	// 64 player temp tid range
	DND_WANDERER_EXP_TID = DND_SHOOTABLETID_BEGIN + MAX_SHOOTABLE_TIDS,
	
	// 64 player temp tid range
	DND_CROSSBOW_EXPLOSIONTID = DND_WANDERER_EXP_TID + MAXPLAYERS,

	// 64 player temp tid range -- Tormentor / Septic Touch
	DND_SEPTIC_CLOUD_TID = DND_CROSSBOW_EXPLOSIONTID + MAXPLAYERS,

	DND_TRICKSTERCLONE_TID = DND_SEPTIC_CLOUD_TID + MAXPLAYERS,
	
	DND_MERCHANT_TID = DND_TRICKSTERCLONE_TID + MAXPLAYERS,
	DND_NPC_TID,
	
	DND_PETTID_BEGIN = DND_NPC_TID + MAX_NPCS,
	
	DND_PICKUPTID_BEGIN = DND_PETTID_BEGIN + MAX_PETS,
	
	DND_SUBORDINATE_TEMPTID = DND_PICKUPTID_BEGIN + MAX_PICKUPS,

	DND_LOOTBOX_TID,

	DND_TRICKSTER_POINTERTID = DND_LOOTBOX_TID + MAXLOOTBOXES,

	DND_INCURSIONMARKER_AUX = DND_TRICKSTER_POINTERTID + MAXPLAYERS,
	DND_INCURSION_SPAWNER_AUX,
	DND_INCURSIONMARKER_TID,
	DND_INCURSIONPORTAL_TID = DND_INCURSIONMARKER_TID + DND_MAX_INCURSION_MARKERS,

	// used for shit like elite sparkles or reflect shields that are attached to a monster
	// DND_MAX_MONSTERS * DND_MAX_MONSTER_ATTACHMENTS is the skip for next
	DND_MONSTER_ATTACHMENT_TID_BEGIN = DND_INCURSIONPORTAL_TID + DND_MAX_INCURSION_PORTALS,
	
	DND_TORCH_TEMP_TID = bcs::INT_MAX - 2,
	DND_DROP_TID,
	SPECIAL_FX_TID
};

#define DND_LASTMONSTER_TID (DND_MAX_MONSTERS + DND_MONSTERTID_BEGIN)
#define DND_PETTID_END (DND_PETTID_BEGIN + MAX_PETS)

enum {
	SETUP_STATE1
};

enum {
	// 32 per
	SETUP_HARDCORE,
	SETUP_SOFTCORE,
	SETUP_MONSTERS,
	SETUP_ITEMTABLES,
	SETUP_MAPCHANGED,
	SETUP_PLAYERDATAFINISHED,
	SETUP_WEAPONDATA,
	SETUP_PLAYERINFO_MINMAXLEVELS,
	SETUP_CLEANINGMONSTERTIDS,
	SETUP_GLOBALCONSTANTSETUPS,
	SETUP_DUNGEONDATAFIRSTTIMECLEAR,
	SETUP_MAPEVENTS,
};
global int 55: SetupStates[2];

void SetupComplete(int state, int flag) {
	SetupStates[state] |= 1 << flag;
}

void SetupUndo(int state, int flag) {
	SetupStates[state] &= ~(1 << flag);
}

bool isSetupComplete(int state, int flag) {
	return SetupStates[state] & (1 << flag);
}

bool isSoftorHardcore() {
	return isSetupComplete(SETUP_STATE1, SETUP_SOFTCORE) || isSetupComplete(SETUP_STATE1, SETUP_HARDCORE);
}

bool isHardcore() {
	return isSetupComplete(SETUP_STATE1, SETUP_HARDCORE);
}

enum {
	DND_PSTATE_LOADED = 1,
	DND_PSTATE_DEAD = 2,
};

global int 6: PlayerGameState[MAXPLAYERS];

#define MAPLOOTPENALTY_FACTOR 4

// can voteskip only twice before loot affects badly -- reason this is 2 + 1 is because each time we enter a map we add 1, if we beat the map legitimately its reduced by 1
// but if we voteskip from the map we got, then this'll be 2 and then we are in trouble until we beat maps legitimately again -- once we legit beat maps if our skip was > 1 we reduce it by 2
#define DND_VOTESKIP_LIMIT 2
#define MAX_MAPS_RECORDED 4

// We hold record of maps visited so far, in case they keep getting voted over and over, they linger here and affect loot drops negatively to incentivize people playing other maps
// When a map is entered, we check our list if we have the map then we add it to our list, so if its the 2nd time visiting we limit loot, once a map is properly 
bool IsLobbyMap(str mapname) {
	return !StrCmp(mapname, "VR") || !StrCmp(mapname, "HUBMAP");
}

// used for aux stuff -- 16 unique sections
enum {
	TARR_ORB1,
	TARR_ORB2,
	TARR_MAPEVENTS,
	TARR_MONID
};
global int 58: TempArray[16][256];

enum {
	DND_MODE_NORMAL,
	DND_MODE_SOFTCORE,
	DND_MODE_HARDCORE
};

// These are actor inherited flags, like forcepain, foilinvul, painless etc.
// can store at most 16 bits, the last 16 are for wepid reference
enum {
	DND_ACTORFLAG_FOILINVUL				=			0b1,
	DND_ACTORFLAG_FORCEPAIN				=			0b10,
	DND_ACTORFLAG_PAINLESS				=			0b100,
	DND_ACTORFLAG_DROPSOUL				=			0b1000,
	DND_ACTORFLAG_CONFIRMEDCRIT			=			0b10000,
	DND_ACTORFLAG_COUNTSASMELEE			=			0b100000,
	DND_ACTORFLAG_THRUGHOST				=			0b1000000,
	DND_ACTORFLAG_FORCERADIUSDMG		=			0b10000000,
	DND_ACTORFLAG_ISDAMAGEOVERTIME		=			0b100000000,
};

int ScanActorFlags() {
	return 	CheckFlag(0, "FOILINVUL") * DND_ACTORFLAG_FOILINVUL				|
			CheckFlag(0, "FORCEPAIN") * DND_ACTORFLAG_FORCEPAIN				|
			CheckFlag(0, "PAINLESS") * DND_ACTORFLAG_PAINLESS				|
			CheckFlag(0, "THRUGHOST") * DND_ACTORFLAG_THRUGHOST				|
			CheckFlag(0, "FORCERADIUSDMG") * DND_ACTORFLAG_FORCERADIUSDMG;
}

enum {
	MVMT_BT_FORWARD,
	MVMT_BT_LEFT,
	MVMT_BT_BACK,
	MVMT_BT_RIGHT
};

int GetMovementButton(int b) {
	switch(b) {
		case MVMT_BT_FORWARD:
		return BT_FORWARD;
		case MVMT_BT_LEFT:
		return BT_MOVELEFT | BT_LEFT;
		case MVMT_BT_BACK:
		return BT_BACK;
		case MVMT_BT_RIGHT:
		return BT_MOVERIGHT | BT_RIGHT;
	}
	return BT_FORWARD;
}

// order follows above
int GetMovementAngle(int b) {
	return b * 0.25;
}

int IsButtonPressed (int input, int oldInput, int mask) {
	return (input & ~oldInput) & mask;
}

int IsButtonHeld (int input, int mask) {
	return input & mask;
}

// tracking of CVars and caching them here for values that have limits (to avoid calling constant Clamp)
enum {
	DND_CVAR_RESEARCHDROPRATE,
	DND_CVAR_ACCESSORYDROPRATE,
	DND_CVAR_ACCESSORYLEVEL,
};
#define MAX_CVARS_TRACKED (DND_CVAR_ACCESSORYLEVEL + 1)

global int 29: CVarValues[MAX_CVARS_TRACKED];

// for now allocate slots for 10
#define MAX_SCRIPT_TRACK 10

enum {
	DND_SCRIPT_EXPTRACKER,
	DND_SCRIPT_BLEND,
	DND_SCRIPT_DAMAGETAKENTIC
};

global bool 17: PlayerScriptsCheck[MAX_SCRIPT_TRACK][MAXPLAYERS];

#define DND_MAX_PETS_PER_PLAYER 25
#define DND_MAX_PETS (DND_MAX_PETS_PER_PLAYER * MAXPLAYERS) // 25 pets per player x 64 players

// holds the monster tids that are in use -- arbitrary order
global int 33: UsedMonsterTIDs[DND_MAX_MONSTERS];

// NOTE: state keyed on monster ids has to be dropped alongside these counters. FlushDamageTicResidue
// is called beside EVERY caller of this function, not just the map change ones -- any caller
// restarts the ids, and a latch that outlives the restart can never be cleared again.
// BuildActivePlayerList goes beside the three that then release monsters.
//
// (BCS forward references DO work across headers -- GiveMonsterTID above calls
// DropMonsterTicResidue from DnD_Damage.h, included later. Only WITHIN one file must a call
// follow its definition.)
void ResetUsedTIDs() {
	if(IsSetupComplete(SETUP_STATE1, SETUP_CLEANINGMONSTERTIDS))
		return;

#ifdef VERBOSE_TID_SETUP
	Log(s:"Clearing used monster TIDs.");
#endif

	ResetTIDCounts();

	SetupComplete(SETUP_STATE1, SETUP_CLEANINGMONSTERTIDS);
}

enum {
	LEVELINFO_PLAYERLEVEL, // sum of all players' levels
	LEVELINFO_MINPLAYERLEVEL,
	LEVELINFO_MAXPLAYERLEVEL,
	LEVELINFO_PLAYERCOUNTATSTART,
	LEVELINFO_SKIPPEDMONSTERTID, // how many monster tids were skipped, this can happen if mappers allocated tids of their own
	LEVELINFO_PLAYERLEVELATSTART, // level total of players at the start of the game

	LEVELINFO_ISDUNGEON,
	LEVELINFO_DUNGEONMUSTRESET,

	// keeps at what tid we are left off
	LEVELINFO_TID_MONSTER,
	LEVELINFO_TID_SHOOTABLE,
	LEVELINFO_TID_PICKUPS,
	LEVELINFO_TID_SHAREDITEMS,
	LEVELINFO_TID_LOOTBOXES,
	LEVELINFO_TID_INCURSIONMARKERS,

	// how many entries of the active player block below are valid
	LEVELINFO_ACTIVEPLAYERCOUNT,

	// Everything past here is the active player block, NOT a level info value. "DnD Request Level
	// Info" clamps to this rather than to the array end, so that mapper facing API cannot hand back
	// a player slot number as though it were level info.
	MAX_LEVELINFO_SCALARS,

	// The player slots that can deal damage this map, packed. Deliberately a LIST and not a bitmask:
	// DropMonsterTicResidue walks it once per monster, and the whole point is to iterate the players
	// who exist rather than all MAXPLAYERS of them.
	LEVELINFO_ACTIVEPLAYERS = MAX_LEVELINFO_SCALARS,

	MAX_LEVELINFO_DATA = LEVELINFO_ACTIVEPLAYERS + MAXPLAYERS
};

bool pinfo_pending_reset = true;
global int 28: InformationInLevel[MAX_LEVELINFO_DATA];

void ResetPlayerInformationLevel() {
	InformationInLevel[LEVELINFO_PLAYERLEVEL] = 0;
	InformationInLevel[LEVELINFO_MINPLAYERLEVEL] = bcs::INT_MAX;
	InformationInLevel[LEVELINFO_MAXPLAYERLEVEL] = bcs::INT_MIN;
	InformationInLevel[LEVELINFO_PLAYERCOUNTATSTART] = 0;
	InformationInLevel[LEVELINFO_PLAYERLEVELATSTART] = 0;

	pinfo_pending_reset = false;

	SetupUndo(SETUP_STATE1, SETUP_PLAYERINFO_MINMAXLEVELS);
}

void ResetTIDCounts() {
	InformationInLevel[LEVELINFO_SKIPPEDMONSTERTID] = 0;
	InformationInLevel[LEVELINFO_TID_MONSTER] = 0;
	InformationInLevel[LEVELINFO_TID_SHOOTABLE] = 0;
	InformationInLevel[LEVELINFO_TID_PICKUPS] = 0;
	InformationInLevel[LEVELINFO_TID_SHAREDITEMS] = 0;
	InformationInLevel[LEVELINFO_TID_LOOTBOXES] = 0;
	InformationInLevel[LEVELINFO_TID_INCURSIONMARKERS] = 0;
}

void UpdateLevelInformation() {
	// get this map's lump name, dungeon names follow "DNDXX"
	str map_lump = StrParam(n:PRINTNAME_LEVEL);

	//Log(s:"lump name: ", s:map_lump);

	InformationInLevel[LEVELINFO_ISDUNGEON] = StrLen(map_lump) > 3 && GetChar(map_lump, 0) == 'D' && GetChar(map_lump, 1) == 'N' && GetChar(map_lump, 2) == 'D';

	//Log(s:"Map is dungeon: ", d:InformationInLevel[LEVELINFO_ISDUNGEON]);
}

void GiveMonsterTID(int base_tid) {
	int temp;

	// Everything downstream indexes MonsterProperties[tid - DND_MONSTERTID_BEGIN], which is exactly
	// DND_MAX_MONSTERS long, so a tid from outside the pool indexes outside the array. Mapper
	// assigned tids do exactly that. Such a monster reads someone else's entry: maxhp comes back 0
	// so no bar is drawn, and trait_list reads junk, which players see as phantom immunities. Drop
	// the tid and let the pool assign a real one. Pets go through GivePetTID instead.
	if(base_tid && (base_tid < DND_MONSTERTID_BEGIN || base_tid >= DND_LASTMONSTER_TID)) {
	#ifdef VERBOSE_MONSTER_SETUP
		Log(s:"Reassigning out of range tid ", d:base_tid, s:" on ", s:GetActorClass(0));
	#endif
		base_tid = 0;
	}

	if(!base_tid) {
		temp = DND_MONSTERTID_BEGIN + InformationInLevel[LEVELINFO_TID_MONSTER] + InformationInLevel[LEVELINFO_SKIPPEDMONSTERTID];
		
		// we have to constantly check if we have run into a specific tid monster...
		while(ThingCount(0, temp)) {
			++temp;
			++InformationInLevel[LEVELINFO_SKIPPEDMONSTERTID];
		}
		base_tid = temp;
		Thing_ChangeTID(0, base_tid);
	}
	// This id has just changed hands, so drop whatever the last owner left latched on it. No Delay
	// may be introduced between the line above and this one -- see DropMonsterTicResidue.
	DropMonsterTicResidue(base_tid - DND_MONSTERTID_BEGIN);

	temp = InformationInLevel[LEVELINFO_TID_MONSTER];
	UsedMonsterTIDs[InformationInLevel[LEVELINFO_TID_MONSTER]++] = base_tid;

#ifdef VERBOSE_MONSTER_SETUP
	Log(s:"monster count: ", d:InformationInLevel[LEVELINFO_TID_MONSTER], s: " given tid ", d:base_tid);
#endif

	ACS_NamedExecuteAlways("DnD Update Monster TID CS", 0, temp, base_tid, InformationInLevel[LEVELINFO_TID_MONSTER]);
}

Script "DnD Update Monster TID CS" (int id, int val, int newcount) CLIENTSIDE {
	if(GameType() == GAME_SINGLE_PLAYER)
		Terminate;

	UsedMonsterTIDs[id] = val;
	InformationInLevel[LEVELINFO_TID_MONSTER] = newcount;

	// this is necessary to fix a bug
	Thing_ChangeTID(0, val);
}

void GivePetTID(int master_tid) {
	int master_pnum = master_tid - P_TIDSTART;
	int i;
	int pet_tid_beg = DND_PETTID_BEGIN + master_pnum * DND_MAX_PETS_PER_PLAYER;
	
	// check linearly while the pet tids are occupied, find one that isn't
	for(i = 0; i < DND_MAX_PETS_PER_PLAYER && ThingCount(T_NONE, pet_tid_beg + i); ++i);
	
	// we haven't skipped over lim which is beginning of pet tid for that player + max pet cap + 1
	if(i != pet_tid_beg + DND_MAX_PETS_PER_PLAYER + 1)
		Thing_ChangeTID(0, pet_tid_beg + i);
}

void GiveShootableTID() {
	Thing_ChangeTID(0, DND_SHOOTABLETID_BEGIN + InformationInLevel[LEVELINFO_TID_SHOOTABLE]);
	++InformationInLevel[LEVELINFO_TID_SHOOTABLE];
}

// These counters define a tid range other systems treat as DENSE -- the artifact placer picks a
// random index inside it and reads coordinates off the tid. So the counter must only advance when
// an actor actually took the tid. It could not before: "Dnd Pickup Setup" waits on
// SETUP_CLEANINGMONSTERTIDS, so the item can be gone by the time this runs, and Thing_ChangeTID
// then quietly does nothing while the counter advanced anyway.
void GivePickupTID() {
	// The range is MAX_PICKUPS wide and DND_SUBORDINATE_TEMPTID begins immediately after it, so
	// running past the end hands pickup tids to other systems' actors. Shared items guard this at
	// their call site in DnD_Shared.h; pickups had no guard anywhere.
	if(InformationInLevel[LEVELINFO_TID_PICKUPS] >= MAX_PICKUPS)
		return;

	int tid = DND_PICKUPTID_BEGIN + InformationInLevel[LEVELINFO_TID_PICKUPS];
	Thing_ChangeTID(0, tid);
	if(ThingCount(T_NONE, tid))
		++InformationInLevel[LEVELINFO_TID_PICKUPS];
}

void GiveSharedItemTID() {
	int tid = SHARED_ITEM_TID_BEGIN + InformationInLevel[LEVELINFO_TID_SHAREDITEMS];
	Thing_ChangeTID(0, tid);
	if(ThingCount(T_NONE, tid))
		++InformationInLevel[LEVELINFO_TID_SHAREDITEMS];
}

int GiveIncursionMarkerTID() {
	int toGive = DND_INCURSIONMARKER_TID + (InformationInLevel[LEVELINFO_TID_INCURSIONMARKERS]++);
	Thing_ChangeTID(DND_INCURSIONMARKER_AUX, toGive);
	return toGive;
}

// naive search -- assumes lowercase key
bool StringContains(str s, str key) {
	int len = StrLen(s);
	int key_len = StrLen(key);
	
	int match_id = 0;
	for(int i = 0; i < len; ++i) {
		// force lowercase
		int c = GetChar(s, i);
		if(c < 'a')
			c += 32;
		if(c == GetChar(key, match_id)) {
			++match_id;
			if(match_id == key_len)
				return true;
		}
		else
			match_id = 0;
	}
	return false;
}

bool isPlayer(int tid) {
	return tid >= P_TIDSTART && tid < P_TIDSTART + MAXPLAYERS;
}

int AngleToFace(int this, int to) {
	int x = GetActorX(to) - GetActorX(this);
	int y = GetActorY(to) - GetActorY(this);
	return VectorAngle(x, y);
}

int PitchToFace(int m1, int m2) {
	int xdiff = GetActorX(m2) - GetActorX(m1);
	int ydiff = GetActorY(m2) - GetActorY(m1);
	int dist = AproxDistance(xdiff, ydiff);
	//dist >>= 8;
	int zdiff = GetActorZ(m2) - GetActorZ(m1);
	//zdiff >>= 8;
	return -VectorAngle(dist, zdiff);
}

int PitchToFace_Height(int m1, int m2) {
	int xdiff = GetActorX(m2) - GetActorX(m1);
	int ydiff = GetActorY(m2) - GetActorY(m1);
	int dist = AproxDistance(xdiff, ydiff);
	//dist >>= 8;
	int zdiff = (GetActorZ(m2) + GetActorProperty(m2, APROP_HEIGHT) / 2) - (GetActorZ(m1) + GetActorProperty(m1, APROP_HEIGHT) / 2);
	//zdiff >>= 8;
	return -VectorAngle(dist, zdiff);
}

int AngleToFaceActor(int this, int to) {
	int x = GetActorX(to) - GetActorX(this);
	int y = GetActorY(to) - GetActorY(this);
	return VectorAngle(x, y);
}

void FaceActor(int this, int to) {
	int x = GetActorX(to) - GetActorX(this);
	int y = GetActorY(to) - GetActorY(this);
	SetActorAngle(this, VectorAngle(x, y));
}

// Exact  v * pct / div,  with `div` times more headroom than the naive form.
//
// The split is an identity, not an approximation:
//     floor(v*p/d)  ==  (v/d)*p + floor((v%d)*p/d)
// so this returns bit-identical results wherever the naive form doesn't
// overflow, and correct results where it does. Saturation happens only when the
// TRUE product exceeds INT_MAX -- the naive `if(v < INT_MAX/p)` guard bails at
// 1/div of that, clamping to INT_MAX while the real answer still fits.
//
// For div = 100 that is the difference between saturating at v = 21,474,836 and
// saturating at v = 2,147,483,647.
int MulPercent_Exact(int v, int pct, int div = 100) {
	if(v <= 0 || pct <= 0 || div <= 0)
		return 0;

	// fast path: the naive product fits
	if(v <= bcs::INT_MAX / pct)
		return v * pct / div;

	// v * pct overflows int32, so split BOTH operands by div. With
	//     v = q1*div + r1  and  pct = q2*div + r2
	// this identity is exact:
	//     floor(v*pct/div) == q1*pct + r1*q2 + floor(r1*r2/div)
	// Only the first term can overflow: r1*q2 < pct, and r1*r2 < div*div.
	// (Splitting v alone is NOT enough -- (v % div) * pct overflows on its own,
	//  and when v < div that term is the entire answer.)
	int q1 = v / div;
	if(q1 > bcs::INT_MAX / pct)
		return bcs::INT_MAX;

	int res = q1 * pct;
	int r1 = v % div;
	int acc = r1 * (pct / div) + (r1 * (pct % div)) / div;

	if(res > bcs::INT_MAX - acc)
		return bcs::INT_MAX;

	return res + acc;
}

// Same split, but ROUNDS instead of truncating. Only worth reaching for where the result is a small
// number and losing half a unit on every one of them adds up -- a 20 pellet shotgun's added or
// converted component is the case this exists for, where truncating cost 19% of the component.
// The primary damage path deliberately still truncates: rounding it would move every weapon's
// numbers for no reason.
//
// IMPORTANT: div must be <= 46340. The split's last term is (v % div) * (pct % div), which fits an
// int only while div * div does. MulPercent_Exact carries the same requirement -- it is just never
// stated there, because until the component path came along every caller passed 100.
int MulPercent_Round(int v, int pct, int div = 100) {
	if(v <= 0 || pct <= 0 || div <= 0)
		return 0;

	// fast path: the biased product fits
	if(v <= (bcs::INT_MAX - div / 2) / pct)
		return (v * pct + div / 2) / div;

	int q1 = v / div;
	if(q1 > bcs::INT_MAX / pct)
		return bcs::INT_MAX;

	// q1 * pct and r1 * (pct / div) are both exact, so the whole fraction lives in the last term --
	// biasing it there rounds the result as a whole
	int res = q1 * pct;
	int r1 = v % div;
	int acc = r1 * (pct / div) + (r1 * (pct % div) + div / 2) / div;

	if(res > bcs::INT_MAX - acc)
		return bcs::INT_MAX;

	return res + acc;
}

// Scales only the BONUS portion of an integer-percent factor, leaving the 100
// baseline untouched.
//
// "66% effectiveness of damage scaling" means a +300% bonus becomes +200%. It
// does NOT mean the base damage is multiplied by 0.66 -- but that is exactly
// what `factor = factor * 2 / 3` does, because the baseline is part of `factor`.
// An ungeared weapon has factor == 100 and comes out at 66.
int ScaleFactorBonus(int factor, int num, int den) {
	if(factor <= 100 || den <= 0)
		return factor;
	return 100 + (factor - 100) * num / den;
}

int SetInventory (str item, int count) {
	int n = count - CheckInventory (item);
	if (n > 0)
		GiveInventory (item, n);
	else if (n < 0)
		TakeInventory (item, -n);
	return n;
}

int SetActorInventory (int tid, str item, int count) {
	int n = count - CheckActorInventory (tid, item);
	if (n > 0)
		GiveActorInventory (tid, item, n);
	else if (n < 0)
		TakeActorInventory (tid, item, -n);
	return n;
}

void GiveOrTake(str item, int count, bool mode) {
	if(!mode)
		GiveInventory(item, count);
	else
		TakeInventory(item, count);
}

int Clamp_Between(int x, int low, int high) {
	if(x < low)
		return low;
	if(x > high)
		return high;
	return x;
}

int IsAlive() {
	return GetActorProperty(0, APROP_HEALTH) > 0;
}

int IsActorAlive(int tid) {
	return GetActorProperty(tid, APROP_HEALTH) > 0;
}

int IsDigit(int c) {
	return c >= '0' && c <= '9';
}

bool IsActivePlayer(int pnum) {
	return PlayerInGame(pnum) && !PlayerIsSpectator(pnum);
}

int GetActivePlayerCount() {
	int res = 0;
	
	for(int i = 0; i < MAXPLAYERS; ++i)
		res += PlayerInGame(i) && !PlayerIsSpectator(i);
	
	return res;
}

// Captures the players that can deal damage this map, so per monster work walks P entries instead
// of all MAXPLAYERS. Two engine calls per slot ONCE, against two per slot per monster.
//
// Call BEFORE ResetUsedTIDs: that sets SETUP_CLEANINGMONSTERTIDS and every monster setup script
// parks on it, so no monster can read a half filled list.
//
// Capturing once is sound because those call sites sit behind the GAMESTATE_INPROGRESS wait and
// survival does not let anyone join after the countdown. A later joiner cannot damage anything
// until the next map, which rebuilds this and clears their slot on the way.
void BuildActivePlayerList() {
	int n = 0;

	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(IsActivePlayer(i))
			InformationInLevel[LEVELINFO_ACTIVEPLAYERS + n++] = i;
	}

	// written last, so the count is never larger than the part of the list that is filled in
	InformationInLevel[LEVELINFO_ACTIVEPLAYERCOUNT] = n;
}

int GetPlayerCountAny() {
	int res = 0;
	
	for(int i = 0; i < MAXPLAYERS; ++i)
		res += PlayerInGame(i) || PlayerIsSpectator(i);
	
	return res;
}

bool PlayerIsInvulnerable() {
	return CheckInventory("P_Invulnerable") || CheckInventory("Invulnerable_Better") || GetActorProperty(0, APROP_INVULNERABLE);
}

// primarily to be used for cases where the actor in question has health that can overflow when multiplied by 100
int GetHealthPercentage(int currhp, int maxhp) {
	// get the 1% of the max hp
	int one_pct = maxhp / 100;
	if(one_pct < 1)
		one_pct = 1;
		
	// find how many of this one pct we contain by dividing the curr hp again
	// forced exception to make sure the player knows they have only done 1% after they really have, and not just 99% displayed on the slightest dmg dealt
	if(currhp > maxhp - one_pct)
		return 100;

	// get the pct of the hp now
	return currhp / one_pct;
}

// How much of a bar_px wide bar is filled at val out of max. This exists for the same reason as
// GetHealthPercentage above, one step further along: a caller that takes a percentage and then
// scales it to the bar's width rounds twice, and a caller that skips the percentage and writes
// val * bar_px / max directly overflows sooner than the percent form does.
//
// The overflow is not theoretical. val * 100 stops fitting in an int above 21474836 and monster
// health passes that -- the product wraps negative, the width comes out negative, and the bar is
// drawn at no width at all, so it stays empty until the health falls back under the limit partway
// into the fight. At bar_px 450 the limit is 4771076, lower still.
int GetBarFill(int val, int max, int bar_px) {
	if(val <= 0 || max <= 0)
		return 0;

	// full bar, which also absorbs the overheal several monsters have rather than running past
	// the end of the bar
	if(val >= max)
		return bar_px;

	if(val <= bcs::INT_MAX / bar_px)
		return bar_px * val / max;

	// Only reachable with val above INT_MAX / bar_px and max above val, so max / bar_px is at
	// least INT_MAX / (bar_px * bar_px) -- five figures for any bar drawn here, never zero.
	// Shrinking the divisor rather than the dividend keeps the error under a thousandth of the
	// bar's width.
	return val / (max / bar_px);
}

// user must guarantee setspecial and setspecial2 are less than 65536
void SpawnDrop(str actor, int zoffset, int thrust, int setspecial, int setspecial2, bool noRandomVelXY = false, int target_tid = -1) {
	SpawnForced(actor, GetActorX(0), GetActorY(0), GetActorZ(0) + zoffset, DND_DROP_TID);
	if(!noRandomVelXY)
		ThrustThing(random(0, 255), random(3, 6), 0, DND_DROP_TID);
	else
		GiveActorInventory(DND_DROP_TID, "DnD_NoLingerCheck", 1);
	ThrustThingZ(DND_DROP_TID, thrust, 0, 1);
	SetActorProperty(DND_DROP_TID, APROP_MASS, setspecial | (setspecial2 << 16));
	if(target_tid != -1) {
		SetActorProperty(DND_DROP_TID, APROP_TARGETTID, target_tid);
		ACS_NamedExecuteWithResult("DnD Update Target", DND_DROP_TID, target_tid);
	}
	Thing_ChangeTID(DND_DROP_TID, 0);
}

void SpawnDropAtActor(int dest_tid, str actor, int zoffset, int thrust, int setspecial, int setspecial2, bool noRandomVelXY = false) {
	SpawnForced(actor, GetActorX(dest_tid), GetActorY(dest_tid), GetActorZ(dest_tid) + zoffset, DND_DROP_TID);
	if(!noRandomVelXY)
		ThrustThing(random(0, 255), random(3, 6), 0, DND_DROP_TID);
	ThrustThingZ(DND_DROP_TID, thrust, 0, 1);
	SetActorProperty(DND_DROP_TID, APROP_MASS, setspecial | (setspecial2 << 16));
	Thing_ChangeTID(DND_DROP_TID, 0);
}

void SpawnDropFacing(str actor, int zoffset, int thrust, int setspecial, int setspecial2) {
	SpawnForced(actor, GetActorX(0), GetActorY(0), GetActorZ(0) + zoffset, DND_DROP_TID);
	ThrustThing((GetActorAngle(0) >> 8) + random(-64, 64), 6, 0, DND_DROP_TID);
	ThrustThingZ(DND_DROP_TID, thrust, 0, 1);
	SetActorProperty(DND_DROP_TID, APROP_MASS, setspecial | (setspecial2 << 16));
	Thing_ChangeTID(DND_DROP_TID, 0);
}

// r first 16 bits contain dist, next 8 bits ang increment, next 8 bits begin_angle
// all this needed because this engine is shit so the whole thing is an awful hack, dont give anything above 255 to this thing...
Script "DnD Try Spawn Area" (int stid, int actortype, int newtid, int r) {
	// tries to spawn an object in a circle around stid
	// tries halving radius if a full circular attempt failed until radius becomes 1
	int degree_inc = (r >> 16) & 0xFF;
	int ang_begin = (((r >> 24) & 0xFF) << 16) / 360;
	r &= 0xFFFF;
	r <<= 16;

	int tries = 0, circle_comp = 360 / degree_inc;
	int sang = GetActorAngle(stid) + ang_begin;
	bool finish = false;
	// convert to byte angle for this part
	degree_inc <<= 16;
	degree_inc /= 360; 

	int cx, cy, cz;
	while(r > 1.0 && !finish) {
		// try to spawn at this one point
		LineAttack(stid, sang, 0, 0, "DnD_IncursionPuff", "", r);

		if(ThingCountName("DnD_IncursionPuff", AUX_INCURSION_PUFF_TID)) {
			cx = GetActorX(AUX_INCURSION_PUFF_TID);
			cy = GetActorY(AUX_INCURSION_PUFF_TID);
			cz = GetSectorFloorZ(0, cx >> 16, cy >> 16);

			//Log(s:"try ", f:cx, s: " ", f:cy, s: " ", f:cz);

			if(Spawn(actortype, cx, cy, cz, newtid, 0))
				finish = true;
			else {
				Thing_ChangeTID(AUX_INCURSION_PUFF_TID, 0);
				++tries;
				if(tries == circle_comp) {
					tries = 0;
					r /= 2;
				}
			}
		}
		else {
			++tries;
			if(tries == circle_comp) {
				tries = 0;
				r /= 2;
			}
		}

		sang += tries * degree_inc;
	}

	// dark wanderer special thingy (yes its awful i know)
	if(newtid == DND_NPC_TID) {
		SetThingSpecial(newtid, ACS_ExecuteAlways, 895, 0, 0, DND_NPC_TID);
		FaceActor(newtid, stid);
	}
	else if(newtid == DND_MERCHANT_TID) {
		SetThingSpecial(newtid, ACS_ExecuteAlways, 895, 0, 1, DND_MERCHANT_TID);
		FaceActor(newtid, stid);
		//Log(s:"Bernabe spawned at ", f:GetActorX(newtid), s:", ", f:GetActorY(newtid));
	}
	
	SetResultValue(0);
}

int SpawnAreaRandomTID(int stid, int radius, str actortype, int newtid, int max_tries = 8) {
	int count = 0;
	bool finished = false;
	while(!finished && count++ < max_tries) {
		// try to spawn at this one point
		int r = FixedMul(radius, random(0.1875, 1.0));
		LineAttack(stid, random(0, 1.0), 0, 0, actortype, "", r);
		finished = true;
		break;
		/*if(ThingCountName("DnD_IncursionPuff", AUX_INCURSION_PUFF_TID)) {
			int cx = GetActorX(AUX_INCURSION_PUFF_TID);
			int cy = GetActorY(AUX_INCURSION_PUFF_TID);
			int cz = GetSectorFloorZ(0, cx >> 16, cy >> 16);

			if(Spawn(actortype, cx, cy, cz, newtid, 0))
				finished = true;
			else
				Thing_ChangeTID(AUX_INCURSION_PUFF_TID, 0);
		}*/
	}

	return finished;
}

void DeleteText(int textid) {
	HudMessage(s:""; HUDMSG_PLAIN, textid, -1, 160.0, 100.0, 0.1);
}

// assumes r2 > r1
void DeleteTextRange(int r1, int r2) {
	for(int i = 0; i < r2 - r1 + 1; i++)
		HudMessage(s:""; HUDMSG_PLAIN, r1 + i, -1, 160.0, 100.0, 0.1);
}

int VectorLength3d(int x, int y, int z) {
	int len = VectorLength(x, y);
	len = VectorLength(z, len);
	return len;
}

bool isPlayerClass(int ctype) {
	return CheckInventory("DnD_Character") - 1 == ctype;
}

bool isActorPlayerClass(int tid, int ctype) {
	return CheckActorInventory(tid, "DnD_Character") - 1 == ctype;
}

int GetPlayerClass() {
	return CheckInventory("DnD_Character") - 1;
}

int GetActorPlayerClass(int tid) {
	return CheckActorInventory(tid, "DnD_Character") - 1;
}

#include "DnD_Math.h"

bool IsActorFacing(int this, int target, int threshold) {
	Vec3_T* v_this = GetDirectionVector(this);
	Vec3_T* v_pos_this = Vec3To_Pos(target, GetActorX(this), GetActorY(this), GetActorZ(this));
	ToUnitVec3(v_pos_this);
	int dot = DotProductVec3(v_this, v_pos_this);

	bcs::free(v_this);
	bcs::free(v_pos_this);

	return dot >= threshold;
}

// moved these here due to co-dependency
#define DND_MAX_CHARS 5

str GetCharField(str fname, int char_id) {
	return StrParam(s:"C", d:char_id, s:"_", s:fname);
}

// Map Evaluation

enum {
	// Monsters
	DND_ZOMBIE_CONTRIB = 3,
	DND_SHOTGUNNER_CONTRIB = 4,
	DND_CHAINGUNNER_CONTRIB = 5,
	DND_IMP_CONTRIB = 4,
	DND_DEMON_CONTRIB = 6,
	DND_HELLKNIGHT_CONTRIB = 10,
	DND_BARON_CONTRIB = 18,
	DND_CACO_CONTRIB = 8,
	DND_PAIN_CONTRIB = 12,
	DND_SOUL_CONTRIB = 4,
	DND_REVENANT_CONTRIB = 9,
	DND_ARACHNO_CONTRIB = 16,
	DND_VILE_CONTRIB = 20,
	DND_FATSO_CONTRIB = 16,
	DND_SPIDERMASTERMIND_CONTRIB = 45,
	DND_CYBERDEMON_CONTRIB = 50,
	DND_BOSSBRAIN_CONTRIB = 9000,
	
	// Items
	DND_HEALTHBONUS_CONTRIB = -1,
	DND_ARMORBONUS_CONTRIB = -1,
	DND_STIM_CONTRIB = -6,
	DND_MEDIKIT_CONTRIB = -12,
	DND_BERSERK_CONTRIB = -30,
	DND_INVUL_CONTRIB = -80,
	DND_SOULSPHERE_CONTRIB = -30,
	DND_MEGA_CONTRIB = -60,
	DND_EVIL_CONTRIB = -20,
	DND_GREENARMOR_CONTRIB = -15,
	DND_BLUEARMOR_CONTRIB = -30,
};

bool HasMapLump(str lump) {
	// first try if the MAP01, MAP02 etc. is in there, if not try full path on the lump if that doesn't exist we know for sure its not there
	int id = LumpOpen(lump, 0);
	if(id == -1) {
		id = LumpOpen(StrParam(s:"Maps/", s:lump, s:".wad"), 0, LUMP_OPEN_FULLPATH);
		
		if(id != -1)
			LumpClose(id);

		return id != -1;
	}

	LumpClose(id);

	return true;
}

int GetMonsterLevelDroprateBonus(int lvl) {
	// new formula to ensure a sharp curve earlier levels then settle down
	return FixedDiv(400.0, (100.0 / lvl + 3.0)) >> 16;
}

// Ranks for everything that dims the player's screen. They all share one blend and one IsBlinded
// flag, so a weaker effect must not repaint over a stronger one -- and, just as important, must not
// be the thing that clears it. DND_VISIONRANK_LIGHT is the base the DND_BLIND_XXX types are offset
// from, so that enum can stay exactly as it is (it is mirrored in DECORATE).
enum {
	DND_VISIONRANK_NONE,
	DND_VISIONRANK_BUFF,		// the player's own tints -- phasing, amphetamine, the doomguy orb
	DND_VISIONRANK_CURSE,		// monster curses and debuffs
	DND_VISIONRANK_LIGHT,		// DND_VISIONRANK_LIGHT + DND_BLIND_LIGHT
	DND_VISIONRANK_HEAVY,		// ...           + DND_BLIND_HEAVY
	DND_VISIONRANK_PETRIFY		// ...           + DND_BLIND_PETRIFY
};

// Below this the effect is only a tint. At or above it the player is BLINDED, which is a gameplay
// state and not just a colour: IsBlinded gates the menu (script 900, so a petrify cannot be cheesed
// by opening the inventory) as well as the pain blend. A buff or curse must never set it.
#define DND_VISIONRANK_BLINDFLOOR DND_VISIONRANK_LIGHT

// Activator must be the player being impaired. False means outranked -- do not touch the screen.
//
// A weaker effect is refused outright rather than merely skipping the paint: letting it extend
// BlindTimer would let a stream of curse ticks hold a petrify's freeze open indefinitely. The
// counterpart is that a light blind landing during a petrify simply does not register -- the screen
// clears when the petrify does, which is the safe direction to be wrong in.
//
// There is deliberately no "somebody else already owns the countdown" answer here any more.
// Inferring a live owner from a non-zero BlindTimer was wrong: running scripts are destroyed at
// level exit but these counters are inventory and are not, so a map change during any vision effect
// left a timer with nothing counting it down. Every later claim was then told an owner existed, no
// countdown ever started again, and the state was stuck for the rest of the session -- petrifies
// that never released, fades that never restored, hit blends suppressed forever, and every lower
// ranked effect denied from then on. Callers now always start "DnD Vision Impair Timer" and the
// newest instance wins, so the next effect to land repairs a stale counter by itself.
bool ClaimVisionImpair(int rank, int duration) {
	int active = CheckInventory("BlindSeverity");

	if(duration <= 0 || rank < active)
		return false;

	SetInventory("BlindSeverity", rank);

	// A strictly stronger effect takes the countdown over outright rather than inheriting a weaker
	// one's longer timer. Keeping the longer value would leave the screen owned by something that has
	// already visually finished, and there would be no moment at which the weaker effect could be
	// handed back -- see the buff tint restore in the timer script. Equal rank is a refresh, so there
	// the longer of the two wins.
	if(rank > active || duration > CheckInventory("BlindTimer"))
		SetInventory("BlindTimer", duration);

	// Only a real blind sets IsBlinded -- see DND_VISIONRANK_BLINDFLOOR. It is a gameplay state (it
	// gates the menu, so a petrify cannot be cheesed by opening the inventory), which means it has to
	// be set whenever a blind lands -- not only when the blind happens to be the one that started the
	// countdown, which is what it used to do.
	if(rank >= DND_VISIONRANK_BLINDFLOOR)
		GiveInventory("IsBlinded", 1);

	return true;
}

#include "DnD_Bitset.h"
#include "DnD_Alias.h"
#include "DnD_Globals.h"

#endif
