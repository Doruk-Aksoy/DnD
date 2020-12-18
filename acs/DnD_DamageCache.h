#ifndef DND_DAMAGECACHE_IN
#define DND_DAMAGECACHE_IN

#include "DnD_Common.h"
#include "DnD_WeaponDefs.h"

// the damage_cache holds base damage values and their rng ranges
// end_factor is (the factor we multiply our base value with using all damage increases) * (the constant to keep precision, which is 100k)
typedef struct pdmg_cache {
	bool massRecalculationRequested;
	int cached_weapon_id;													// id of the weapon we cached for
	bool norecalculate[MAX_CACHE_ELEMENTS]; 								// false => needs recalculation otherwise no
	pdmg_T damage_cache[MAX_CACHE_ELEMENTS];
	int flat_values[MAX_CACHE_ELEMENTS];									// holds flat dmg bonuses
	int final_factor[MAX_CACHE_ELEMENTS];									// holds final factor we multiply with
} pdmg_cache_T;

pdmg_cache_T& GetPlayerDamageCache(int pnum) {
	static pdmg_cache_T cache[MAXPLAYERS];
	return cache[pnum];
}

bool PlayerDamageNeedsCaching(int pnum, int wepid, int dmgid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	return !cache.norecalculate[dmgid];
}

void ClearCache(int pnum, int dmgid, int wepid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	cache.flat_values[dmgid] = 0;
	cache.final_factor[dmgid] = 0;
	cache.cached_weapon_id = wepid;
}

// this guy gets called last, so we mark recalc stuff here
void MarkCachingComplete(int pnum, int dmgid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	cache.norecalculate[dmgid] = true;
	// clean the previously issued mass recalc request, since player fired now they might want to craft afterwards
	cache.massRecalculationRequested = false;
}

void CachePlayerDamage(int pnum, int dmg, int dmgid, int dmg_rand, int wepid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	// not cached
	if(cache.cached_weapon_id != wepid)
		ForceClearCache(cache, pnum);
	
	cache.damage_cache[dmgid].dmg = dmg;
	cache.final_factor[dmgid] = dmg; // final_factor refers to dmg after all dmg calculation
	cache.damage_cache[dmgid].dmg_low = dmg_rand & 0xFFFF;
	cache.damage_cache[dmgid].dmg_high = dmg_rand >> 16;
	cache.norecalculate[dmgid] = false;
	cache.cached_weapon_id = wepid;
}

void CachePlayerFlatDamage(int pnum, int dmg, int dmgid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	// not cached
	if(!cache.norecalculate[dmgid])
		cache.final_factor[dmgid] = dmg;
}

void InsertCacheFactor(int pnum, int dmgid, int factor) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	
	// convert to fixed percentages -- these are 1 to 100 originally they need to get mapped to 0.01 to 1.0
	factor = 1.0 + factor * 0.01;
	
	// if 0, replace otherwise fixed mul
	if(cache.final_factor[dmgid])
		cache.final_factor[dmgid] = FixedMul(cache.final_factor[dmgid], factor);
	else
		cache.final_factor[dmgid] = factor;
}

int GetCachedPlayerDamage(int pnum, int dmgid) {
	return GetPlayerDamageCache(pnum).damage_cache[dmgid].dmg;
}

int GetCachedPlayerFlatDamage(int pnum, int dmgid) {
	return GetPlayerDamageCache(pnum).flat_values[dmgid];
}

int GetCachedPlayerFactor(int pnum, int dmgid) {
	return GetPlayerDamageCache(pnum).final_factor[dmgid];
}

int GetCachedPlayerRandomRange(int pnum, int dmgid) {
	pdmg_cache_T& cache = GetPlayerDamageCache(pnum);
	if(cache.damage_cache[dmgid].dmg_low && cache.damage_cache[dmgid].dmg_high)
		return cache.damage_cache[dmgid].dmg_low | (cache.damage_cache[dmgid].dmg_high << 16);
	return 1;
}

void ForceClearCache(pdmg_cache_T& cache, int pnum) {
	for(int j = 0; j < MAX_CACHE_ELEMENTS; ++j) {
		/*cache.damage_cache[j].dmg = 0;
		cache.damage_cache[j].dmg_low = 0;
		cache.damage_cache[j].dmg_high = 0;*/
		cache.norecalculate[j] = false;
		cache.final_factor[j] = 0;
		cache.flat_values[j] = 0;
	}
	cache.cached_weapon_id = -1;
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
