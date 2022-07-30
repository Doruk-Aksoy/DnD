#ifndef DND_DAMAGECACHE_IN
#define DND_DAMAGECACHE_IN

#include "DnD_Common.h"
#include "DnD_WeaponDefs.h"

// the damage_cache holds base damage values and their rng ranges
// end_factor is (the factor we multiply our base value with using all damage increases) * (the constant to keep precision, which is 100k)
typedef struct pdmg_cache {
	bool massRecalculationRequested;
	bool norecalculate[MAXWEPS][MAX_CACHE_ELEMENTS]; 								// false => needs recalculation otherwise no
	pdmg_T damage_cache[MAXWEPS][MAX_CACHE_ELEMENTS];
	int flat_values[MAXWEPS][MAX_CACHE_ELEMENTS];									// holds flat dmg bonuses
	int final_factor[MAXWEPS][MAX_CACHE_ELEMENTS];									// holds final factor we multiply with
} pdmg_cache_T;

pdmg_cache_T& GetPlayerDamageCache(int pnum) {
	static pdmg_cache_T cache[MAXPLAYERS];
	return cache[pnum];
}

bool PlayerDamageNeedsCaching(int pnum, int wepid, int dmgid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	return !cache.norecalculate[wepid][dmgid];
}

void ClearCache(int pnum, int wepid, int dmgid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	cache.flat_values[wepid][dmgid] = 0;
	cache.final_factor[wepid][dmgid] = 100;
}

// this guy gets called last, so we mark recalc stuff here
void MarkCachingComplete(int pnum, int wepid, int dmgid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	cache.norecalculate[wepid][dmgid] = true;
	// clean the previously issued mass recalc request, since player fired now they might want to craft afterwards
	cache.massRecalculationRequested = false;
}

void CachePlayerDamage(int pnum, int dmg, int wepid, int dmgid, int dmg_rand) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	
	cache.damage_cache[wepid][dmgid].dmg = dmg;
	cache.final_factor[wepid][dmgid] = dmg; // final_factor refers to dmg after all dmg calculation
	cache.damage_cache[wepid][dmgid].dmg_low = dmg_rand & 0xFFFF;
	cache.damage_cache[wepid][dmgid].dmg_high = dmg_rand >> 16;
	cache.norecalculate[wepid][dmgid] = false;
}

void CachePlayerFlatDamage(int pnum, int dmg, int wepid, int dmgid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	// not cached
	if(!cache.norecalculate[wepid][dmgid])
		cache.flat_values[wepid][dmgid] = dmg;
}

void InsertCacheFactor(int pnum, int wepid, int dmgid, int factor, bool isAdditive) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	
	// if 0, replace otherwise fixed mul
	if(cache.final_factor[wepid][dmgid]) {
		if(isAdditive)
			cache.final_factor[wepid][dmgid] += factor;
		else {
			// convert to fixed percentages -- these are 1 to 100 originally they need to get mapped to 0.01 to 1.0
			// testing new method: just integers so overflows are a lot less likely to occur later on -- we multiply some value like 125% with 1.33 etc here
			cache.final_factor[wepid][dmgid] = cache.final_factor[wepid][dmgid] * (100 + factor) / 100;
		}
	}
	else
		cache.final_factor[wepid][dmgid] = 100 + factor;
}

// used for multiplicative item mods that are by default fixed point
void InsertCacheFactor_Fixed(int pnum, int wepid, int dmgid, int factor) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	
	// if 0, replace otherwise fixed mul
	if(cache.final_factor[wepid][dmgid]) {
		// since this is already fixed and multiplicative, and is of form (1.0 + more multiplier), we don't add anything here just do fixedmul
		// notice no "isAdditive" check here, it's pointless
		cache.final_factor[wepid][dmgid] = cache.final_factor[wepid][dmgid] * (100 + ((factor * 100) >> 16)) / 100;
	}
	else
		cache.final_factor[wepid][dmgid] = 100 + ((factor * 100) >> 16);
}

int GetCachedPlayerDamage(int pnum, int wepid, int dmgid) {
	return GetPlayerDamageCache(pnum).damage_cache[wepid][dmgid].dmg;
}

int GetCachedPlayerFlatDamage(int pnum, int wepid, int dmgid) {
	return GetPlayerDamageCache(pnum).flat_values[wepid][dmgid];
}

int GetCachedPlayerFactor(int pnum, int wepid, int dmgid) {
	return GetPlayerDamageCache(pnum).final_factor[wepid][dmgid];
}

int GetCachedPlayerRandomRange(int pnum, int wepid, int dmgid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	if(cache.damage_cache[wepid][dmgid].dmg_low && cache.damage_cache[wepid][dmgid].dmg_high)
		return cache.damage_cache[wepid][dmgid].dmg_low | (cache.damage_cache[wepid][dmgid].dmg_high << 16);
	return 1;
}

void ForceClearCache(pdmg_cache_T& cache, int pnum) {
	for(int i = 0; i < MAXWEPS; ++i) {
		for(int j = 0; j < MAX_CACHE_ELEMENTS; ++j) {
			cache.norecalculate[i][j] = false;
			cache.final_factor[i][j] = 1.0;
			cache.flat_values[i][j] = 0;
		}
	}
}

// forces player to recalculate the damage values
void ForcePlayerDamageCaching(int pnum) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	// if a mass recalculation was issued, so we don't keep repeating this over and over if player is simply crafting
	if(!cache.massRecalculationRequested) {
		cache.massRecalculationRequested = true;
		ForceClearCache(cache, pnum);
	}
}

#endif
