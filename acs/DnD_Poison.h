#ifndef DND_POISON_IN
#define DND_POISON_IN

typedef struct poison_dmg {
	int dmg;
	int duration;
	int owner;
	int inflictor;
} poison_T;

#define MAX_POISON_DAMAGES 5
typedef struct poison_cache {
	poison_T poison_list[MAX_POISON_DAMAGES];
	int cursize;
} poison_cache_T;

poison_cache_T player_poison_damages[MAXPLAYERS];

void DealPoisonDamage(int target, int dmg) {
	// this is the player, target is monster
	int this = ActivatorTID();
	SetActivator(target);
	
	int m_id = target - DND_MONSTERTID_BEGIN;
	
	int factor = 	(MonsterProperties[m_id].level > 1) * GetMonsterDMGScaling(m_id, MonsterProperties[m_id].level) + 
					MonsterProperties[m_id].trait_list[DND_EXTRASTRONG] * DND_ELITE_EXTRASTRONG_BONUS;
			
	dmg = dmg * (100 + factor - CheckActorInventory(this, "IATTR_ReducedPoisonTaken")) / 100;
			
	// elite damage bonus is multiplicative
	if(MonsterProperties[m_id].isElite/* && dmg < INT_MAX / factor*/)
		dmg = dmg * (100 + GetEliteBonusDamage(m_id)) / 100;
		
	if(dmg > 0) {
		IncrementStatistic(DND_STATISTIC_DAMAGETAKEN, dmg, this);
		Thing_Damage2(this, dmg, "PoisonDOT");
	}
	SetActivator(this);
}

void ClearPoisonInstance(int pnum, int id) {
	player_poison_damages[pnum].poison_list[id].owner = 0;
	player_poison_damages[pnum].poison_list[id].inflictor = 0;
	player_poison_damages[pnum].poison_list[id].dmg = 0;
	player_poison_damages[pnum].poison_list[id].duration = 0;
	--player_poison_damages[pnum].cursize;
}

void RegisterPoisonDamage (int damage, int duration, str inflictor) {
	int this = PlayerNumber();
	int target = GetActorProperty(0, APROP_TARGETTID);
	ACS_NamedExecuteWithResult("DnD Player Poisoned", damage | (this << 16), duration, target, inflictor);
}

Script "DnD Player Poisoned" (int pnum, int duration, int owner, int inflictor) {
	// inflictor is a string
	// unpack damage
	int dmg = pnum & 0xFFFF;
	pnum >>= 16;
	
	// check if player has a poison source of this script running already
	int i;
	for(i = 0; i < MAX_POISON_DAMAGES; ++i) {
		// player already has a poison instance running from the owner that is also the very same actor
		// simply replace its duration and damage, if its higher
		if(player_poison_damages[pnum].poison_list[i].owner == owner && player_poison_damages[pnum].poison_list[i].inflictor == inflictor) {
			player_poison_damages[pnum].poison_list[i].duration = duration;
			if(player_poison_damages[pnum].poison_list[i].dmg < dmg)
				player_poison_damages[pnum].poison_list[i].dmg = dmg;
			Terminate;
		}
	}
	
	// there was no instance of this before, insert it
	// we have no space, find the one with least duration left and replace it
	int min = INT_MAX;
	int pos = 0;
	if(player_poison_damages[pnum].cursize == MAX_POISON_DAMAGES) {
		for(i = 0; i < MAX_POISON_DAMAGES; ++i) {
			if(player_poison_damages[pnum].poison_list[i].duration < min) {
				min = player_poison_damages[pnum].poison_list[i].duration;
				pos = i;
			}
		}
		
		// found one with min duration, replace and terminate
		player_poison_damages[pnum].poison_list[pos].duration = duration;
		player_poison_damages[pnum].poison_list[pos].dmg = dmg;
		player_poison_damages[pnum].poison_list[pos].owner = owner;
		player_poison_damages[pnum].poison_list[pos].inflictor = inflictor;
		Terminate;
	}
	else {
		for(i = 0; i < MAX_POISON_DAMAGES; ++i) {
			if(!player_poison_damages[pnum].poison_list[i].dmg) {
				pos = i;
				break;
			}
		}
	}
	
	// pos is new position to be inserted --- will deal the percentage over the duration
	player_poison_damages[pnum].poison_list[pos].dmg = Max(dmg / duration, 1);
	player_poison_damages[pnum].poison_list[pos].duration = duration;
	player_poison_damages[pnum].poison_list[pos].owner = owner;
	player_poison_damages[pnum].poison_list[pos].inflictor = inflictor;
	
	// start the damaging process
	int victim = P_TIDSTART + pnum;
	while(IsActorAlive(victim)) {
		Delay(const:TICRATE);
		
		if(!player_poison_damages[pnum].poison_list[pos].duration || CheckActorInventory(victim, "RemoveAilments")) {
			ClearPoisonInstance(pnum, pos);
			break;
		}
		
		//printbold(s:"damage from ", s:inflictor, s:" owned by ", d:owner);
		DealPoisonDamage(player_poison_damages[pnum].poison_list[pos].owner, player_poison_damages[pnum].poison_list[pos].dmg);
		--player_poison_damages[pnum].poison_list[pos].duration;
	}
	
	SetResultValue(0);
}

#endif
