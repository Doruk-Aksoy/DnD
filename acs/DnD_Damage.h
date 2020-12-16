#ifndef DND_DAMAGE_IN
#define DND_DAMAGE_IN

enum {
	DND_DAMAGE_EXPLOSIVES
	
};
#define MAX_DAMAGE_TYPES (DND_DAMAGE_EXPLOSIVES + 1)

str DamageTypeList[MAX_DAMAGE_TYPES] = {
	"Explosives"

};

// returns the filtered, reduced etc. damage when factoring in all resists or weaknesses ie. this is the final damage the actor will take
// dmg value is FIXED POINT NOT INT!
int HandleDamageDeal(int source, int victim, int dmg, int damage_type) {

	// we are done scaling, make this a normal int now
	return dmg >> 16;
}

Script "DnD Do Explosion Damage" (int dmg, int radius, int fullradius, int damage_type) {
	// player information
	int owner = GetActorProperty(0, APROP_TARGETTID);
	int pnum = owner - P_TIDSTART;
	
	int instance = PlayerExplosionList[pnum].curr_instance;
	int lim = PlayerExplosionList[pnum].list[instance].amt;
	
	int px = GetActorX(0), py = GetActorY(0), pz = GetActorZ(0);

	// set activator to player for dmg registry
	SetActivator(P_TIDSTART + pnum);
	
	// turn them to fixed
	radius <<= 16;
	fullradius <<= 16;
	dmg <<= 16;
	for(int i = 0; i < lim; ++i) {
		int mon_id = PlayerExplosionList[pnum].list[instance].monsters[i];
		
		// calculate damage falloff based on distance
		int final_dmg = dmg;
		int dist = fdistance_delta(px - GetActorX(mon_id), py - GetActorY(mon_id), pz - GetActorZ(mon_id));
		
		// if full radius is less than radius, if  fullradius == radius or more, no need to scale it
		if(fullradius < radius) {
			// idea here: monster hitboxes are actual rectangles and not circles, so unless hit occured perpendicular to the hitbox, you won't deal max damage
			// to fix that, subtract (r + r * sqrt2) / 2, which is 1.207. Reason: radius scales from r to r * sqrt2 over the square's center to diagonal.
			dist -= FixedMul(GetActorProperty(mon_id, APROP_RADIUS), 1.4142);
			if(dist < 0)
				dist = 0;
			// we will reduce damage if we are past fullradius and within radius
			if(dist > fullradius && dist <= radius) {
				//printbold(s:"dist ", f:dist, s:" factor ", f:FixedDiv(radius - dist, radius - fullradius));
				final_dmg = FixedMul(final_dmg, FixedDiv(radius - dist, radius - fullradius));
			}
		}
		
		// dont deal 0 dmg
		if(!final_dmg)
			final_dmg = 1.0;
		
		final_dmg = HandleDamageDeal(owner, mon_id, final_dmg, damage_type);
		Thing_Damage2(mon_id, final_dmg, DamageTypeList[damage_type]);
		
		//printbold(s:"Dealing ", d: final_dmg, s: " damage to ", d:mon_id, s: " of type ", s:DamageTypeList[damage_type]);
	}
	
	// damage is dealt we are done with this instance, free it up
	Free_Explosion_Instance(pnum, instance);
	
	SetResultValue(0);
}

Script "DnD Do Impact Damage" (int dmg, int damage_type) {
	int owner = GetActorProperty(0, APROP_TARGETTID);
	int victim = GetActorProperty(0, APROP_TRACERTID);
	
	//printbold(d:owner, s: " ", d:victim);
	SetActivator(owner);
	
	if(owner && victim) {
		int final_dmg = HandleDamageDeal(owner, victim, dmg << 16, damage_type);
		Thing_Damage2(victim, final_dmg, DamageTypeList[damage_type]);
	}
	
	SetResultValue(0);
}

#endif
