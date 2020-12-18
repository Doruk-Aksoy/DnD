#ifndef DND_DAMAGECACHEDEFS_IN
#define DND_DAMAGECACHEDEFS_IN

enum {
	DND_DMGID_0,
	DND_DMGID_1,
	DND_DMGID_2,
	DND_DMGID_3,
	DND_DMGID_4,
	DND_DMGID_5
};

typedef struct pdmg {
	int dmg;
	int dmg_low;
	int dmg_high;
} pdmg_T;

#define MAX_CACHE_ELEMENTS (DND_DMGID_5 + 1)

#endif
