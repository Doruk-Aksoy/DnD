#ifndef DND_SKILLDEF_IN
#define DND_SKILLDEF_IN

#define DND_SKILL_MINLEVEL 1
#define DND_SKILL_MAXLEVEL 10
#define MAX_SKILL_LEVELS 10

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

#define ICESHIELD_COUNT 8
#define ICESHIELD_BASE_DURATION 5 * TICRATE
#define ICESHIELD_DURATION_PER_INT 5
#define ICESHIELD_HEALTHBASE 75
#define ICESHIELD_HEALTH_PER_LEVEL 10
#define ICESHIELD_HEALTH_PER_INT 5

#define MOLTENBOULDER_BASESPEED 20

#define LIGHTNINGSPEAR_BASE_RIP 3
#define LIGHTNINGSPEAR_INT_FACTOR 20

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

enum {
	DND_SPELL_RALLY,
	DND_SPELL_POISONNOVA,
	DND_SPELL_ICESHIELD,
	DND_SPELL_MOLTENBOULDER,
	DND_SPELL_LIGHTNINGSPEAR,
};
#define MAX_SPELLS (DND_SPELL_LIGHTNINGSPEAR + 1)

enum {
	DND_SPELLDAMAGE_POISONNOVA,
	DND_SPELLDAMAGE_POISONNOVADOT,
	
	DND_SPELLDAMAGE_MOLTENBOULDER,
	DND_SPELLDAMAGE_MOLTENBOULDERBUMP,
	
	DND_SPELLDAMAGE_LIGHTNINGSPEAR,
	DND_SPELLDAMAGE_LIGHTNINGSPEARBOUNCE,
};
#define MAX_DAMAGING_SPELLS DND_SPELLDAMAGE_LIGHTNINGSPEARBOUNCE + 1

enum {
	DND_SPELLFLAG_ELEMENTAL = 1
};

#define DND_MAX_SKILL_REQ 3

typedef struct {
	int cost;
	int cooldown; // fixed_t
	int cast_time; // fixed_t
	int requirement_id[DND_MAX_SKILL_REQ];
	int data_ref; // reference to base values of spell info
	int flags;
} skill_def_T;

#define MAX_SKILLS_IN_TREE 30
typedef struct {
	skill_def_T skills[MAX_SKILLS_IN_TREE];
	int name_index;
} skill_tree_T;

// definition of the skill trees
// define your skills in the brackets
/*skill_tree_T SkillTrees[MAX_SKILL_TREES] = {
};*/

#define SPELL_COOLDOWN 0
#define SPELL_FLAGS 1
#define SPELL_DAMAGE_INDEX 2
// form this into a function later
int SpellData[MAX_SPELLS][4] = {
	{ RALLY_COOLDOWN, 		0 },
	{ 0, 					DND_SPELLFLAG_ELEMENTAL },
	{ 0, 					DND_SPELLFLAG_ELEMENTAL },
	{ 0, 					DND_SPELLFLAG_ELEMENTAL },
	{ 0, 					DND_SPELLFLAG_ELEMENTAL }
};

#define SPELL_NAME 0
#define SPELL_COOLDOWNITEM 1
#define SPELL_COOLDOWNCOUNTER 2
// put these in a global array later
str SpellInfo[MAX_SPELLS][3] = {
	{ "Rally", 					"RallyCooldown", 					"RallyCooldownCounter" },
	{ "PoisonNova", 			"PoisonNovaCooldown", 				"PoisonNovaCooldownCounter" },
	{ "IceShield",				"IceShieldCooldown",				"IceShieldCooldownCounter" },
	{ "MoltenBoulder",			"MoltenBoulderCooldown",			"MoltenBoulderCooldownCounter" },
	{ "LightningSpear",			"LightningSpearCooldown",			"LightningSpearCooldownCounter" }
};

#define MAX_SUMMON_ZOMBIECOUNT 5
#define SKILL_ZOMBIE_DURATION 8
// Replaced by DND_PET_HP_PER_INT below -- kept only so nothing referring to it breaks.
#define SKILL_ZOMBIE_HP_PER_INT 2

// Minion health per point of intellect, in HUNDREDTHS OF A PERCENT of the level scaled total --
// so 40 is 0.4% a point, the same figure DND_SPELL_INT_ATTUNE gives spell damage. One rate reads
// the same across the tree, and a percentage is proportional where the flat 2 it replaces was
// not: that 2 was a sixth of a fresh zombie and a four hundredth of a Fire Demon.
//
// Taken off the SCALED health rather than the base, because the level curve is multiplicative --
// against the base it would decay into nothing by level 50 exactly as the flat value did.
#define DND_PET_HP_PER_INT 40
#define ZOMBIE_INT_TIMER_FACTOR 10
// Replaced by DND_PET_DMG_PER_INT below -- kept only so nothing referring to it breaks.
#define ZOMBIE_INT_DAMAGE_FACTOR 0.125

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

int GetSpellPoisonFactor(int spell_id) {
	if(spell_id == DND_SPELL_POISONNOVA)
		return 20 * random(5, 8);
	return 0;
}

#endif
