#ifndef DND_POISON_IN
#define DND_POISON_IN

typedef struct dot_dmg {
	int dmg;
	int duration;
	int owner;
	int inflictor;
	str dmg_type;
} dot_T;

#define MAX_DOT_INSTANCES 16
typedef struct dot_cache {
	dot_T dot_list[MAX_DOT_INSTANCES];
	int highest_ignite_index;
	int cursize;
} dot_cache_T;

dot_cache_T module& GetPlayerDotCache(int pnum) {
	static dot_cache_T dots[MAXPLAYERS];
	return dots[pnum];
}

void DealDOTDamage(int target, int dmg, str dmg_type) {
	// this is the player, target is monster
	int this = ActivatorTID();
	SetActivator(target);
	
	int m_id = target - DND_MONSTERTID_BEGIN;
	
	// removed factor stuff from here because the damage is already contributed
	// these are handled in main damage event already, no need to double dip
	/*if(dmg_type == "PoisonDOT") {
		dmg = dmg * (100 - CheckActorInventory(this, "IATTR_ReducedPoisonTaken")) / 100;
		dmg = ApplyPlayerResist(this - P_TIDSTART, dmg, INV_DMGREDUCE_POISON);
	}
	else if(dmg_type == "PhysicalDOT")
		dmg = ApplyPlayerResist(this - P_TIDSTART, dmg, INV_DMGREDUCE_PHYS);*/

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
	dot_cache_T module& player_dot_damages = GetPlayerDotCache(pnum);
	player_dot_damages.dot_list[id].owner = 0;
	player_dot_damages.dot_list[id].inflictor = 0;
	player_dot_damages.dot_list[id].dmg = 0;
	player_dot_damages.dot_list[id].duration = 0;
	--player_dot_damages.cursize;

	// find new highest ignite if there is
	int cur_max = 0;
	int cur_max_id = 0;
	for(id = 0; id < MAX_DOT_INSTANCES; ++id) {
		if
		(
			player_dot_damages.dot_list[id].dmg_type == "FireDOT" &&
			cur_max < player_dot_damages.dot_list[id].dmg
		)
		{
			cur_max = player_dot_damages.dot_list[id].dmg;
			cur_max_id = id;
		}
	}

	player_dot_damages.highest_ignite_index = cur_max_id;
}

void RegisterDoTDamage (int damage, int duration, int dmg_type, int owner, str inflictor) {
	int this = PlayerNumber();
	ACS_NamedExecuteWithResult("DnD Player Receive DoT", damage | (this << 16), duration | (dmg_type << 16), owner, inflictor);
}

Script "DnD Player Receive DoT" (int pnum, int duration, int owner, int inflictor) {
	// inflictor is a string
	// unpack damage
	int dmg = pnum & 0xFFFF;
	pnum >>= 16;

	int dtype_int = duration >> 16;
	duration &= 0xFFFF;

	int victim = P_TIDSTART + pnum;
	if(HasActorClassPerk_Fast(victim, "Wanderer", 1))
		duration = duration * (100 - DND_WANDERER_PERK5_DEBUFFREDUCE) / 100;
	
	// check if player has a dot source of this script running already
	int i;
	dot_cache_T module& player_dot_damages = GetPlayerDotCache(pnum);
	for(i = 0; i < MAX_DOT_INSTANCES; ++i) {
		// player already has a dot instance running from the owner that is also the very same actor
		// simply replace its duration and damage, if its higher
		if(player_dot_damages.dot_list[i].owner == owner && player_dot_damages.dot_list[i].inflictor == inflictor) {
			player_dot_damages.dot_list[i].duration = duration;
			if(player_dot_damages.dot_list[i].dmg < dmg) {
				player_dot_damages.dot_list[i].dmg = dmg;

				// update highest ignite index in case this is greater
				if(player_dot_damages.dot_list[i].dmg_type == "FireDOT" && player_dot_damages.dot_list[player_dot_damages.highest_ignite_index].dmg < dmg)
					player_dot_damages.highest_ignite_index = i;
			}
			Terminate;
		}
	}
	
	// there was no instance of this before, insert it
	// we have no space, find the one with least duration left and replace it
	int min = INT_MAX;
	int pos = 0;
	if(player_dot_damages.cursize == MAX_DOT_INSTANCES) {
		for(i = 0; i < MAX_DOT_INSTANCES; ++i) {
			if(player_dot_damages.dot_list[i].duration < min) {
				min = player_dot_damages.dot_list[i].duration;
				pos = i;
			}
		}
		
		// found one with min duration, replace and terminate
		player_dot_damages.dot_list[pos].duration = duration;
		player_dot_damages.dot_list[pos].dmg = dmg;
		player_dot_damages.dot_list[pos].owner = owner;
		player_dot_damages.dot_list[pos].inflictor = inflictor;
		if(player_dot_damages.dot_list[pos].dmg_type == "FireDOT" && player_dot_damages.dot_list[player_dot_damages.highest_ignite_index].dmg < dmg)
			player_dot_damages.highest_ignite_index = pos;
		Terminate;
	}
	else {
		for(i = 0; i < MAX_DOT_INSTANCES; ++i) {
			if(!player_dot_damages.dot_list[i].dmg) {
				pos = i;
				break;
			}
		}
	}
	
	// pos is new position to be inserted --- will deal the percentage over the duration
	player_dot_damages.dot_list[pos].duration = duration;
	player_dot_damages.dot_list[pos].owner = owner;
	player_dot_damages.dot_list[pos].inflictor = inflictor;

	if(dtype_int & DND_DAMAGETYPEFLAG_POISON) {
		player_dot_damages.dot_list[pos].dmg_type = "PoisonDOT";
		player_dot_damages.dot_list[pos].dmg = Max(dmg / duration, 1);
	}
	else if(dtype_int & DND_DAMAGETYPEFLAG_PHYSICAL) {
		if(dtype_int & DND_DAMAGETYPEFLAG_ISBLEED)
			player_dot_damages.dot_list[pos].dmg_type = "BleedDOT";
		else
			player_dot_damages.dot_list[pos].dmg_type = "PhysicalDOT";
		player_dot_damages.dot_list[pos].dmg = dmg;
	}
	else if(dtype_int & DND_DAMAGETYPEFLAG_FIRE) {
		// fire dot deals the percentage of the damage throughout each tic, not over a duration
		player_dot_damages.dot_list[pos].dmg_type = "FireDOT";
		player_dot_damages.dot_list[pos].dmg = dmg;

		// update highest
		//printbold(s:"insert ", d:dmg, s:" into fire dot");
		if(player_dot_damages.dot_list[player_dot_damages.highest_ignite_index].dmg < dmg) {
			player_dot_damages.highest_ignite_index = pos;
			//printbold(s:"highest index ", d:pos);
		}
	}
	
	// start the damaging process
	if(player_dot_damages.dot_list[pos].dmg_type == "PoisonDOT") {
		while(IsActorAlive(victim)) {
			Delay(const:TICRATE);
			
			if(!player_dot_damages.dot_list[pos].duration || CheckActorInventory(victim, "RemoveAilments")) {
				ClearDoTInstance(pnum, pos);
				break;
			}

			ACS_NamedExecuteAlways("DnD Spawn Poison FX", 0, victim, random(1, 3));
			
			//printbold(s:"damage from ", s:inflictor, s:" owned by ", d:owner);
			DealDOTDamage(player_dot_damages.dot_list[pos].owner, player_dot_damages.dot_list[pos].dmg, player_dot_damages.dot_list[pos].dmg_type);
			--player_dot_damages.dot_list[pos].duration;
		}
	}
	else if(player_dot_damages.dot_list[pos].dmg_type == "FireDOT") {
		while(IsActorAlive(victim)) {
			Delay(const:26);
			
			if(!player_dot_damages.dot_list[pos].duration || CheckActorInventory(victim, "RemoveAilments") || CheckActorInventory(victim, "DnD_DousePlayer")) {
				ClearDoTInstance(pnum, pos);
				break;
			}

			// only receive the damage from the highest ignite index
			if(pos == player_dot_damages.highest_ignite_index) {
				//printbold(s:" do damage ", d:player_dot_damages.dot_list[pos].dmg);
				ACS_NamedExecuteAlways("DnD Monster Ignite FX", 0, victim, random(3, 5));
				DealDOTDamage(player_dot_damages.dot_list[pos].owner, player_dot_damages.dot_list[pos].dmg, player_dot_damages.dot_list[pos].dmg_type);
			}

			--player_dot_damages.dot_list[pos].duration;
		}
	}
	else if(player_dot_damages.dot_list[pos].dmg_type == "BleedDOT") {
			PlaySound(victim, "Bleed/Loop", CHAN_7, 0.875, true);

		while(IsActorAlive(victim)) {
			Delay(const:HALF_TICRATE);
			
			if
			(
				!player_dot_damages.dot_list[pos].duration || 
				CheckActorInventory(victim, "RemoveAilments") ||
				(HasActorClassPerk_Fast(victim, "Punisher", 5) && (CheckActorInventory(victim, "DnD_MultikillCounter") + 1) / DND_SPREE_PER >= 1) // bleed remove if punisher on spree with last perk
			)
			{
				ClearDoTInstance(pnum, pos);
				break;
			}

			// spawn blood and stuff
			ACS_NamedExecuteAlways("DnD Bleed FX", 0, victim);

			min = player_dot_damages.dot_list[pos].dmg;
			if(IsPlayerMoving(pnum, victim))
				min *= DND_BLEED_MOVEMENT_MULTIPLIER;
			
			//printbold(s:"damage from ", s:inflictor, s:" owned by ", d:owner, s:" amount: ", d:min);
			DealDOTDamage(player_dot_damages.dot_list[pos].owner, min, player_dot_damages.dot_list[pos].dmg_type);
			--player_dot_damages.dot_list[pos].duration;
		}

		StopSound(victim, CHAN_7);
		PlaySound(victim, "Bleed/End", CHAN_7);
	}
	else {
		while(IsActorAlive(victim)) {
			Delay(const:TICRATE);
			
			if(!player_dot_damages.dot_list[pos].duration || CheckActorInventory(victim, "RemoveAilments")) {
				ClearDoTInstance(pnum, pos);
				break;
			}

			// spawn blood and stuff
			//ACS_NamedExecuteAlways("DnD Spawn Poison FX", 0, victim, random(1, 3));
			
			//printbold(s:"damage from ", s:inflictor, s:" owned by ", d:owner);
			DealDOTDamage(player_dot_damages.dot_list[pos].owner, player_dot_damages.dot_list[pos].dmg, player_dot_damages.dot_list[pos].dmg_type);
			--player_dot_damages.dot_list[pos].duration;
		}

		if(player_dot_damages.dot_list[pos].dmg_type == "BleedDOT") {
			StopSound(victim, CHAN_7);
			PlaySound(victim, "Bleed/End", CHAN_7);
		}
	}
	
	SetResultValue(0);
}

#endif
