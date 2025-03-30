#ifndef DND_POISON_IN
#define DND_POISON_IN

typedef struct dot_dmg {
	int dmg;
	int duration;
	int owner;
	int inflictor;
	str dmg_type;
} dot_T;

#define MAX_DOT_INSTANCES 8
typedef struct dot_cache {
	dot_T dot_list[MAX_DOT_INSTANCES];
	int cursize;
} dot_cache_T;

dot_cache_T player_dot_damages[MAXPLAYERS];

void DealDOTDamage(int target, int dmg, str dmg_type) {
	// this is the player, target is monster
	int this = ActivatorTID();
	SetActivator(target);
	
	int m_id = target - DND_MONSTERTID_BEGIN;
	
	// removed factor stuff from here because the damage is already contributed
	if(dmg_type == "PoisonDOT") {
		dmg = dmg * (100 - CheckActorInventory(this, "IATTR_ReducedPoisonTaken")) / 100;
		dmg = ApplyPlayerResist(this - P_TIDSTART, dmg, INV_DMGREDUCE_POISON);
	}
	else if(dmg_type == "PhysicalDOT")
		dmg = ApplyPlayerResist(this - P_TIDSTART, dmg, INV_DMGREDUCE_PHYS);

	// DaemonicBarrier
	if(dmg > 0) {
		if(!CheckActorInventory(this, "DaemonicBarrier")) {
			IncrementStatistic(DND_STATISTIC_DAMAGETAKEN, dmg, this);
			Thing_Damage2(this, dmg, dmg_type);
		}
		else // take one out for this tic
			TakeActorInventory(this, "DaemonicBarrier", 1);
	}
	SetActivator(this);
}

void ClearDoTInstance(int pnum, int id) {
	player_dot_damages[pnum].dot_list[id].owner = 0;
	player_dot_damages[pnum].dot_list[id].inflictor = 0;
	player_dot_damages[pnum].dot_list[id].dmg = 0;
	player_dot_damages[pnum].dot_list[id].duration = 0;
	--player_dot_damages[pnum].cursize;
}

void RegisterDoTDamage (int damage, int duration, int dmg_type, str inflictor) {
	int this = PlayerNumber();
	int target = GetActorProperty(0, APROP_TARGETTID);
	ACS_NamedExecuteWithResult("DnD Player Receive DoT", damage | (this << 16), duration | (dmg_type << 16), target, inflictor);
}

Script "DnD Player Receive DoT" (int pnum, int duration, int owner, int inflictor) {
	// inflictor is a string
	// unpack damage
	int dmg = pnum & 0xFFFF;
	pnum >>= 16;

	int dtype_int = duration >> 16;
	duration &= 0xFFFF;

	int victim = P_TIDSTART + pnum;
	if(CheckActorInventory(victim, "Wanderer_Perk5"))
		duration = duration * (100 - DND_WANDERER_PERK5_DEBUFFREDUCE) / 100;
	
	// check if player has a poison source of this script running already
	int i;
	for(i = 0; i < MAX_DOT_INSTANCES; ++i) {
		// player already has a poison instance running from the owner that is also the very same actor
		// simply replace its duration and damage, if its higher
		if(player_dot_damages[pnum].dot_list[i].owner == owner && player_dot_damages[pnum].dot_list[i].inflictor == inflictor) {
			player_dot_damages[pnum].dot_list[i].duration = duration;
			if(player_dot_damages[pnum].dot_list[i].dmg < dmg)
				player_dot_damages[pnum].dot_list[i].dmg = dmg;
			Terminate;
		}
	}
	
	// there was no instance of this before, insert it
	// we have no space, find the one with least duration left and replace it
	int min = INT_MAX;
	int pos = 0;
	if(player_dot_damages[pnum].cursize == MAX_DOT_INSTANCES) {
		for(i = 0; i < MAX_DOT_INSTANCES; ++i) {
			if(player_dot_damages[pnum].dot_list[i].duration < min) {
				min = player_dot_damages[pnum].dot_list[i].duration;
				pos = i;
			}
		}
		
		// found one with min duration, replace and terminate
		player_dot_damages[pnum].dot_list[pos].duration = duration;
		player_dot_damages[pnum].dot_list[pos].dmg = dmg;
		player_dot_damages[pnum].dot_list[pos].owner = owner;
		player_dot_damages[pnum].dot_list[pos].inflictor = inflictor;
		Terminate;
	}
	else {
		for(i = 0; i < MAX_DOT_INSTANCES; ++i) {
			if(!player_dot_damages[pnum].dot_list[i].dmg) {
				pos = i;
				break;
			}
		}
	}
	
	// pos is new position to be inserted --- will deal the percentage over the duration
	player_dot_damages[pnum].dot_list[pos].dmg = Max(dmg / duration, 1);
	player_dot_damages[pnum].dot_list[pos].duration = duration;
	player_dot_damages[pnum].dot_list[pos].owner = owner;
	player_dot_damages[pnum].dot_list[pos].inflictor = inflictor;

	if(dtype_int == DND_DAMAGETYPEFLAG_POISON)
		player_dot_damages[pnum].dot_list[pos].dmg_type = "PoisonDOT";
	else if(dtype_int == DND_DAMAGETYPEFLAG_PHYSICAL)
		player_dot_damages[pnum].dot_list[pos].dmg_type = "PhysicalDOT";
	
	// start the damaging process
	while(IsActorAlive(victim)) {
		Delay(const:TICRATE);
		
		if(!player_dot_damages[pnum].dot_list[pos].duration || CheckActorInventory(victim, "RemoveAilments")) {
			ClearDoTInstance(pnum, pos);
			break;
		}
		
		//printbold(s:"damage from ", s:inflictor, s:" owned by ", d:owner);
		DealDOTDamage(player_dot_damages[pnum].dot_list[pos].owner, player_dot_damages[pnum].dot_list[pos].dmg, player_dot_damages[pnum].dot_list[pos].dmg_type);
		--player_dot_damages[pnum].dot_list[pos].duration;
	}
	
	SetResultValue(0);
}

#endif
