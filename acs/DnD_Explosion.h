#ifndef DND_EXPLOSION_IN
#define DND_EXPLOSION_IN

#define MAX_MONSTERS_PER_EXPLOSION 1024
#define MAX_EXPLOSION_INSTANCES 16

typedef struct dnd_exp_monster {
	int amt;
	int monsters[MAX_MONSTERS_PER_EXPLOSION];
} explosion_monsterlist_T;

typedef struct dnd_exp {
	int curr_instance;
	int instance_amt;
	bool InstancesInUse[MAX_EXPLOSION_INSTANCES];
	explosion_monsterlist_T list[MAX_EXPLOSION_INSTANCES];
} explosion_T;

// this is the list that holds all explosions
explosion_T PlayerExplosionList[MAXPLAYERS];

// return -1 to denote full capacity
int Create_ExplosionInstance(int pnum) {
	if(PlayerExplosionList[pnum].instance_amt != MAX_EXPLOSION_INSTANCES) {
		PlayerExplosionList[pnum].instance_amt++;
		// look for a suitable instance id
		for(int i = 0; i < MAX_EXPLOSION_INSTANCES; ++i)
			if(!PlayerExplosionList[pnum].InstancesInUse[i]) {
				PlayerExplosionList[pnum].InstancesInUse[i] = true;
				return i;
			}
		return -1;
	}
	return -1;
}

void Free_Explosion_Instance(int pnum, int instance) {
	// mark as free spot and decrement total instance counter
	PlayerExplosionList[pnum].InstancesInUse[instance] = false;
	--PlayerExplosionList[pnum].instance_amt;
	
	// clear monster list
	int lim = PlayerExplosionList[pnum].list[instance].amt;
	for(int i = 0; i < lim; ++i)
		PlayerExplosionList[pnum].list[instance].monsters[i] = 0;
	PlayerExplosionList[pnum].list[instance].amt = 0;
}

int Add_Monster_To_Explosion(int pnum, int instance) {
	if(PlayerExplosionList[pnum].list[instance].amt != MAX_MONSTERS_PER_EXPLOSION)
		return PlayerExplosionList[pnum].list[instance].amt++;
	return -1;
}

Script "DnD Register Explosion Instance" (void) {
	int owner = GetActorProperty(0, APROP_TARGETTID);
	if(!isPlayer(owner))
		owner = GetActorProperty(0, APROP_SCORE);
	int pnum = owner - P_TIDSTART;
	
	//printbold(s:"Register Explosion to ptid ", d:owner);
	
	PlayerExplosionList[pnum].curr_instance = Create_ExplosionInstance(pnum);
	SetInventory("DnD_ProjPnum", pnum);
	
	SetResultValue(0);
}

// target => pet, master => summoner
Script "DnD Register Explosion Instance (Pets)" (void) {
	int owner = ACS_NamedExecuteWithResult("DnD Get Master of Target");
	
	int pnum = owner - P_TIDSTART;
	
	//printbold(s:"Register Explosion to ptid ", d:owner);
	
	PlayerExplosionList[pnum].curr_instance = Create_ExplosionInstance(pnum);
	SetInventory("DnD_ProjPnum", pnum);
	
	SetResultValue(0);
}

Script "DnD Register Monster to Instance" (int pnum) {
	int list_id = PlayerExplosionList[pnum].curr_instance;
	int mon_index;
	// check the tid of recipient first
	if(ActivatorTID() && (mon_index = Add_Monster_To_Explosion(pnum, list_id)) != -1) {
		//printbold(s:"Add monster ", d:ActivatorTID(), s: " to player ", d:pnum, s: " of instance ", d:list_id);
		// we register this monster to list_id instance of player pnum for processing
		PlayerExplosionList[pnum].list[list_id].monsters[mon_index] = ActivatorTID();
	}
	//printbold(s:"Monster insertion checked on ", d:ActivatorTID(), s: " ", d: mon_index);
	SetResultValue(0);
}

#endif
