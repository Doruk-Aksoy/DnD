#ifndef DND_DAMAGECACHE_IN
#define DND_DAMAGECACHE_IN

#include "DnD_Common.h"
#include "DnD_WeaponDefs.h"

typedef struct pdmg_cache {
	bool massRecalculationRequested;
	bool norecalculate[MAXWEPS][MAX_CACHE_ELEMENTS]; // false => needs recalculation otherwise no
	pdmg_T damage_cache[MAXWEPS][MAX_CACHE_ELEMENTS];
	int end_damage_cache[MAXWEPS][MAX_CACHE_ELEMENTS];
} pdmg_cache_T;

pdmg_cache_T& GetPlayerDamageCache(int pnum) {
	static pdmg_cache_T cache[MAXPLAYERS];
	return cache[pnum];
}

void CachePlayerDamage(int dmg, int dmgid, int dmg_rand) {
	int wepid = CheckInventory("DnD_WeaponID");
	pdmg_cache_T& cache = GetPlayerDamageCache(PlayerNumber());
	// not cached
	if(!cache.damage_cache[wepid][dmgid].dmg && !cache.norecalculate[wepid][dmgid]) {
		cache.damage_cache[wepid][dmgid].dmg = dmg;
		cache.end_damage_cache[wepid][dmgid] = dmg; //end_damage_cache refers to dmg after all dmg calculation
		cache.damage_cache[wepid][dmgid].dmg_low = dmg_rand & 0xFFFF;
		cache.damage_cache[wepid][dmgid].dmg_high = dmg_rand >> 16;
	}
}

void CachePlayerEndDamageWithID(int wepid, int dmg, int dmgid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(PlayerNumber());
	// not cached
	if(!cache.norecalculate[wepid][dmgid]) {
		cache.end_damage_cache[wepid][dmgid] = dmg;
		cache.norecalculate[wepid][dmgid] = true;
		// clean the previously issued mass recalc request, since player fired now they might want to craft afterwards
		cache.massRecalculationRequested = false;
	}
}

bool PlayerDamageNeedsCaching(int wepid, int dmgid) {
	return !GetPlayerDamageCache(PlayerNumber()).norecalculate[wepid][dmgid];
}

int GetCachedPlayerDamage(int wepid, int dmgid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(PlayerNumber());
	return cache.damage_cache[wepid][dmgid].dmg;
}

int GetCachedPlayerEndDamage(int wepid, int dmgid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(PlayerNumber());
	return cache.end_damage_cache[wepid][dmgid];
}

int GetCachedPlayerRandomRange(int wepid, int dmgid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(PlayerNumber());
	if(cache.damage_cache[wepid][dmgid].dmg_low && cache.damage_cache[wepid][dmgid].dmg_high)
		return cache.damage_cache[wepid][dmgid].dmg_low | (cache.damage_cache[wepid][dmgid].dmg_high << 16);
	return 1;
}

// forces player to recalculate the damage values
void ForcePlayerDamageCaching(int pnum) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	// if a mass recalculation was issued, so we don't keep repeating this over and over if player is simply crafting
	if(!cache.massRecalculationRequested) {
		cache.massRecalculationRequested = true;
		for(int i = 0; i < MAXWEPS; ++i)
			for(int j = 0; j < MAX_CACHE_ELEMENTS; ++j)
				cache.norecalculate[i][j] = 0;
	}
}

#endif