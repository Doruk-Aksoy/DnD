#ifndef DND_SKILLDEF_IN
#define DND_SKILLDEF_IN

#define DND_SKILL_MINLEVEL 1
#define DND_SKILL_MAXLEVEL 10

#define DND_PET_MOVEDIST 512.0

// Reactivity. A pet carries +QUICKTORETALIATE so it answers whatever is hitting it, but that flag
// alone makes it change its mind on every hit and never land a swing. So the flag comes OFF for
// COMMIT tics each time it picks something new, and goes back on after.
#define DND_PET_COMMIT_TICS 70   // how long it stays loyal to a fresh target
#define DND_PET_REACT_TICS 5     // how often it notices it switched

#define RALLY_DIST_PER_INT 0.75
#define RALLY_DISTANCE 128.0
#define RALLY_DURATION 8
#define RALLY_FX_COUNT 32
#define RALLY_R 64
#define RALLY_TIC_TIMER 8 * TICRATE
#define RALLY_COOLDOWN 20
#define RALLY_BASEDAMAGE 35
#define RALLY_DAMAGEPERLVL 4

enum {
	SKILLINFO_ZOMBIEPETTIMER
};

#define MAX_SKILL_TREES 8
enum {
	DND_SKILLTREE_FIRE,
	DND_SKILLTREE_ICE,
	DND_SKILLTREE_LIGHTNING,
	DND_SKILLTREE_EARTH,
	DND_SKILLTREE_BLACK,
	DND_SKILLTREE_ARCANE,
	DND_SKILLTREE_CHAOS,
	DND_SKILLTREE_COMBAT
};

// Rally, cast by the Dead King's Banner through "DnD Cast Spell"; must match the enum in Actors/Spells/Common.dec.
enum {
	DND_SPELL_RALLY
};
#define MAX_SPELLS (DND_SPELL_RALLY + 1)

// A spell tree node's prerequisites.
#define DND_MAX_SKILL_REQ 3

#define SPELL_NAME 0
#define SPELL_COOLDOWNITEM 1
#define SPELL_COOLDOWNCOUNTER 2
str SpellInfo[MAX_SPELLS][3] = {
	{ "Rally", "RallyCooldown", "RallyCooldownCounter" }
};

#define MAX_SUMMON_ZOMBIECOUNT 5
#define SKILL_ZOMBIE_DURATION 8

// Minion health per point of intellect, in HUNDREDTHS OF A PERCENT of the level scaled total --
// so 40 is 0.4% a point, the same figure DND_SPELL_INT_ATTUNE gives spell damage. One rate reads
// the same across the tree, and a percentage is proportional where the flat 2 it replaces was
// not: that 2 was a sixth of a fresh zombie and a four hundredth of a Fire Demon.
//
// Taken off the SCALED health rather than the base, because the level curve is multiplicative --
// against the base it would decay into nothing by level 50 exactly as the flat value did.
#define DND_PET_HP_PER_INT 40

// Hundredths of a percent per point, same rate as minion health: a barrier is a durability pool on
// a conjured object, so it scales the way one does. The legacy skill used a FLAT +5, which on the
// spell's 200 base would have buried the row.
#define DND_SPELL_BARRIERHP_PER_INT 40
#define ZOMBIE_INT_TIMER_FACTOR 10

// Minion damage per point of intellect, shared by every pet. This is ADDED to a multiplier that
// starts at 1.0, so 0.04 is 4% of base damage a point -- x5 at 100 intellect, x9 at 200.
//
// NOT the 0.004 the spell and pet health rates use, and deliberately so: those two multiply a
// figure that is already level scaled, while this one competes with a level term worth under a
// percent. At 0.004 intellect moved pet damage by a handful of points across the whole stat
// range. The zombie 0.125 it replaces was the opposite error -- 12.5% a point left intellect as
// the ONLY meaningful input, with level contributing almost nothing beside it.
#define DND_PET_DMG_PER_INT 0.04
#define DND_MAX_PETPAINSHARE 9

#endif
