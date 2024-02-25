#ifndef DND_SKILLDEF_IN
#define DND_SKILLDEF_IN

#define DND_SKILL_MINLEVEL 1
#define DND_SKILL_MAXLEVEL 10
#define MAX_SKILL_LEVELS 10

#define DND_PET_MOVEDIST 512.0

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
int SpellData[MAX_SPELLS][4] = {
	{ RALLY_COOLDOWN, 		0 },
	{ 0, 					DND_SPELLFLAG_ELEMENTAL },
	{ 0, 					DND_SPELLFLAG_ELEMENTAL },
	{ 0, 					DND_SPELLFLAG_ELEMENTAL },
	{ 0, 					DND_SPELLFLAG_ELEMENTAL }
};

// the 2 numbers at the end are scaling factors picked randomly, so range is dmg * random(low, high)
pdmg_T SpellDamageTable[MAX_DAMAGING_SPELLS] = {
	{ 120, 0, 0 },
	{ 20, 5, 8 },
	
	{ 24, 0, 0 },
	{ 256, 0, 0 },
	
	{ 15, 4, 6 },
	{ 45, 0, 0 }
};

#define SPELL_NAME 0
#define SPELL_COOLDOWNITEM 1
#define SPELL_COOLDOWNCOUNTER 2
str SpellInfo[MAX_SPELLS][3] = {
	{ "Rally", 					"RallyCooldown", 					"RallyCooldownCounter" },
	{ "PoisonNova", 			"PoisonNovaCooldown", 				"PoisonNovaCooldownCounter" },
	{ "IceShield",				"IceShieldCooldown",				"IceShieldCooldownCounter" },
	{ "MoltenBoulder",			"MoltenBoulderCooldown",			"MoltenBoulderCooldownCounter" },
	{ "LightningSpear",			"LightningSpearCooldown",			"LightningSpearCooldownCounter" }
};

#define MAX_SUMMON_ZOMBIECOUNT 5
#define SKILL_ZOMBIE_DURATION 8
#define SKILL_ZOMBIE_HP_PER_INT 2
#define ZOMBIE_INT_TIMER_FACTOR 10
#define ZOMBIE_INT_DAMAGE_FACTOR 0.125
#define DND_MAX_PETPAINSHARE 9

int GetSpellPoisonFactor(int spell_id) {
	if(spell_id == DND_SPELL_POISONNOVA)
		return SpellDamageTable[DND_SPELLDAMAGE_POISONNOVADOT].dmg * random(SpellDamageTable[DND_SPELLDAMAGE_POISONNOVADOT].dmg_low, SpellDamageTable[DND_SPELLDAMAGE_POISONNOVADOT].dmg_high);
	return 0;
}

#endif
