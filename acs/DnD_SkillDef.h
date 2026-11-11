#ifndef DND_SKILLDEF_IN
#define DND_SKILLDEF_IN

#define DND_SKILL_MINLEVEL 1
#define DND_SKILL_MAXLEVEL 10

#define DND_PET_MOVEDIST 768.0

// Reactivity. A pet carries +QUICKTORETALIATE so it answers whatever is hitting it, but that flag
// alone makes it change its mind on every hit and never land a swing. So the flag comes OFF for
// COMMIT tics each time it picks something new, and goes back on after.
#define DND_PET_COMMIT_TICS 70   // how long it stays loyal to a fresh target
#define DND_PET_REACT_TICS 5     // how often it notices it switched

#define RALLY_DIST_PER_LEVEL 24.0
#define RALLY_DISTANCE 128.0
#define RALLY_DURATION 8
#define RALLY_FX_COUNT 32
#define RALLY_R 64
#define RALLY_TIC_TIMER 8 * TICRATE
#define RALLY_COOLDOWN 20
#define RALLY_BASEDAMAGE 35
#define RALLY_DAMAGEPERLVL 4

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

// Barrier health per point of intellect, hundredths of a percent: 0.4%, the rate spells take damage at.
#define DND_SPELL_BARRIERHP_PER_INT 40

#define DND_MAX_PETPAINSHARE 9

#endif
