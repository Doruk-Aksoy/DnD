#ifndef DND_DAMAGECACHE_IN
#define DND_DAMAGECACHE_IN

#include "DnD_Common.h"

enum {
	DND_DMGID_0 = 0,
	DND_DMGID_1,
	DND_DMGID_2,
	DND_DMGID_3,
	DND_DMGID_4,
	DND_DMGID_5,
	DND_DMGID_6,
	DND_DMGID_7,
	DND_DMGID_8
};

typedef struct pdmg {
	int dmg;
	int dmg_low;
	int dmg_high;
} pdmg_T;

#define MAX_CACHE_ELEMENTS 8
typedef struct pdmg_cache {
	int curwep;
	pdmg_T damage_cache[MAX_CACHE_ELEMENTS];
} pdmg_cache_T;

pdmg_cache_T& GetPlayerDamageCache(int pnum) {
	static pdmg_cache_T cache[MAXPLAYERS];
	return cache[pnum];
}

void CachePlayerDamage(int dmg, int id, int dmg_rand) {
	int wepid = CheckInventory("DnD_WeaponID");
	int pnum = PlayerNumber();
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	// if different weapon, clear all
	if(wepid != cache.curwep) {
		for(int i = 0; i < MAX_CACHE_ELEMENTS; ++i) {
			cache.damage_cache[i].dmg = 0;
			cache.damage_cache[i].dmg_low = 0;
			cache.damage_cache[i].dmg_high = 0;
		}
		cache.curwep = wepid;
	}
	// not cached
	if(!cache.damage_cache[id].dmg) {
		cache.damage_cache[id].dmg = dmg;
		cache.damage_cache[id].dmg_low = dmg_rand & 0xFFFF;
		cache.damage_cache[id].dmg_high = dmg_rand >> 16;
	}
}

int GetCachedPlayerDamage(int id) {
	pdmg_cache_T& cache = GetPlayerDamageCache(PlayerNumber());
	if(cache.damage_cache[id].dmg_low && cache.damage_cache[id].dmg_high)
		return cache.damage_cache[id].dmg * random(cache.damage_cache[id].dmg_low, cache.damage_cache[id].dmg_high);
	return cache.damage_cache[id].dmg;
}

#endif