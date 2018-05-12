#ifndef DND_POISON_IN
#define DND_POISON_IN

#include "DnD_Elites.h"

typedef struct poison_dmg {
	int dmg;
	int duration;
	int owner;
	int stime;
	bool state; // 0 inactive, 1 active
} poison_T;

#define MAX_POISON_DAMAGES 5
typedef struct poison_cache {
	poison_T poison_list[MAX_POISON_DAMAGES];
	int cursize;
} poison_cache_T;

poison_cache_T player_poison_damages[MAXPLAYERS];

void DealPoisonDamage(int target, int dmg) {
	int this = ActivatorTID();
	SetActivator(target);
	if(dmg > (1 << 16)) // random component
		dmg *= random((dmg >> 8) & 0xFF, (dmg >> 16) & 0xFF);
	int bonus = (CheckInventory("MonsterTraits2") & DND_EXTRASTRONG) ? DND_ELITE_EXTRASTRONG_BONUS : 0;
	int res = (Clamp_Between(CheckInventory("MonsterLevel"), 1, DND_MAX_MONSTERLVL) * Clamp_Between(GetCVar("dnd_monster_dmgscalepercent"), 0.01, 1.0));
	if(CheckInventory("MonsterIsElite"))
		res = FixedMul(res, 1.0 + GetEliteBonusDamage());
	if(IsExtraStrong())
		res = FixedMul(res, 1.0 + DND_ELITE_EXTRASTRONG_BONUS);
	if(CheckInventory("MonsterLevel") > 50)
		res = FixedMul(res, 1.0 + DND_AFTER50_INCREMENT_DAMAGE);
	//printbold(f:res);
	dmg = ((dmg & 0xFF) * ((res / 2) + 1.0)) >> 16;
	Thing_Damage2(this, dmg, "Poison");
	SetActivator(this);
}

void RegisterPoisonDamage(int pnum, int dmg, int duration, int owner) {
	int pos = -1, i;
	bool sameSource = false;
	// check if we have space first
	if(player_poison_damages[pnum].cursize < MAX_POISON_DAMAGES) {
		// if this was put by the same source, just find the one that's running out and replace that one instead
		for(i = 0; i < MAX_POISON_DAMAGES; ++i) {
			if(player_poison_damages[pnum].poison_list[i].owner == owner) {
				pos = i;
				sameSource = true;
				break;
			}
		}
		// not replaced, check for free spots
		if(pos == -1) {
			// start and check from beginning, see which spot is free
			for(i = 0; i < MAX_POISON_DAMAGES; ++i) {
				if(!player_poison_damages[pnum].poison_list[i].state) {
					pos = i;
					break;
				}
			}
		}
		if(!sameSource)
			++player_poison_damages[pnum].cursize;
		player_poison_damages[pnum].poison_list[pos].dmg = dmg;
		player_poison_damages[pnum].poison_list[pos].duration = duration;
		player_poison_damages[pnum].poison_list[pos].owner = owner;
		player_poison_damages[pnum].poison_list[pos].state = 1;
	}
	else {
		// find one with lowest duration left
		int min = INT_MAX;
		for(i = 0; i < MAX_POISON_DAMAGES; ++i)
			if(player_poison_damages[pnum].poison_list[i].duration < min) {
				min = player_poison_damages[pnum].poison_list[i].duration;
				pos = i;
			}
		// safety check if something stupid like all of them having equal times happens...
		if(pos == -1)
			pos = 0;
		player_poison_damages[pnum].poison_list[pos].dmg = dmg;
		player_poison_damages[pnum].poison_list[pos].duration = duration;
		player_poison_damages[pnum].poison_list[pos].owner = owner;
		player_poison_damages[pnum].poison_list[pos].state = 1;
	}
}

void ClearPoisonDamages() {
	for(int i = 0; i < MAX_POISON_DAMAGES; ++i)
		player_poison_damages[PlayerNumber()].poison_list[i].state = 0;
}

#endif