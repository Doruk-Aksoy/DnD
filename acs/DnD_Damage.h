#ifndef DND_DAMAGE_IN
#define DND_DAMAGE_IN

enum {
	DND_DAMAGETYPE_PHYSICAL,
	DND_DAMAGETYPE_ENERGY,
	DND_DAMAGETYPE_EXPLOSIVES,
	DND_DAMAGETYPE_OCCULT,
	DND_DAMAGETYPE_OCCULTEXPLOSION,

	// elemental types
	DND_DAMAGETYPE_FIRE,
	DND_DAMAGETYPE_ICE,
	DND_DAMAGETYPE_POISON,
	DND_DAMAGETYPE_EMERALD,
	DND_DAMAGETYPE_LIGHTNING
};
#define MAX_DAMAGE_TYPES (DND_DAMAGETYPE_LIGHTNING + 1)

str DamageTypeList[MAX_DAMAGE_TYPES] = {
	"Bullet",
	"Energy",
	"Explosives",
	"Magic",
	"Explosives_Magic",
	
	// elemental
	"Fire",
	"Ice",
	"Poison",
	"Emerald",
	"Lightning"
};

enum {
	DND_DAMAGEFLAG_FOILINVUL 			= 			0b1,
	DND_DAMAGEFLAG_FORCERADIUSDMG 		= 			0b10,
	DND_DAMAGEFLAG_USEMASTER 			= 			0b100,
	DND_DAMAGEFLAG_ISSHOTGUN 			= 			0b1000,
	//DND_DAMAGEFLAG_EXTRATODEMON 		= 			0b10000,
};

void AdjustDamageRetrievePointers(int flags) {
	int temp;
	if(flags & DND_WDMG_USETARGET) { // use target
		// hopefully no projectile uses score
		if(!GetActorProperty(0, APROP_SCORE)) {
			SetActorProperty(0, APROP_SCORE, GetActorProperty(0, APROP_TARGETTID));
			SetActivator(0, AAPTR_TARGET);
		}
	}
	else if(flags & DND_WDMG_USEMASTER) {
		if(flags & DND_WDMG_SETMASTER) // this is a hack
			SetPointer(AAPTR_TARGET, GetActorProperty(0, APROP_MASTERTID));
		if(!GetActorProperty(0, APROP_SCORE)) {
			SetActorProperty(0, APROP_SCORE, GetActorProperty(0, APROP_TARGETTID));
			SetActivator(GetActorProperty(0, APROP_MASTERTID));
		}
	}
	else if(flags & DND_WDMG_USETRACER)
		SetActivator(0, AAPTR_TRACER);
	else if(flags & DND_WDMG_USETARGETSMASTER) {
		temp = ACS_NamedExecuteWithResult("DnD Get Master of Target");
		if(!GetActorProperty(0, APROP_SCORE)) {
			SetActorProperty(0, APROP_SCORE, temp);
			SetActivator(temp);
		}
		SetActivator(temp);
	}

	// no owner can potentially mean this was a reflected projectile, we can manipulate this a bit
	if(!ActivatorTID()) {
		GiveInventory("MarkAsReflected", 1);
		SetActivator(GetActorProperty(0, APROP_SCORE));
	}
}

// pellets is used for shotgun / burst type attacks which share the same damage uniformly to avoid recalculation of multiple of these
int ScaleCachedDamage(int wepid, int pnum, int dmgid, int talent_type, int flags, bool isSpecial, int pellets) {
	// we don't cache special ammo damage
	int dmg = 0;
	int temp;
	// get the damage
	if(!isSpecial)
		temp = GetCachedPlayerDamage(pnum, dmgid);
	else // special ammo damage
		temp = GetSpecialAmmoDamage(dmgid, wepid);
		
	// if there are shotgun things that have random values in them, we will include them as such into overall damage
	int range = GetCachedPlayerRandomRange(pnum, dmgid);
	if(range > 1) {
		for(int i = 0; i < pellets; ++i)
			dmg += temp * random(range & 0xFFFF, range >> 16);
	}
	else // no rng, so just set it to pellets * temp
		dmg = temp * pellets;

	if(PlayerDamageNeedsCaching(pnum, wepid, dmgid) || isSpecial) {
		// add flat damage bonus mapping talent name to flat bonus type
		temp = MapTalentToFlatBonus(pnum, talent_type);
		dmg += temp;
		
		ClearCache(pnum, dmgid, wepid);
		CachePlayerFlatDamage(pnum, temp, dmgid); 
		
		// quest or accessory bonuses
		// is occult (add demon bane bonus)
		if(flags & DND_WDMG_ISOCCULT || talent_type == TALENT_OCCULT) {
			temp = DND_DEMONBANE_GAIN * IsAccessoryEquipped(ActivatorTID(), DND_ACCESSORY_DEMONBANE);
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, dmgid, temp);
		}
		
		// gunslinger affected
		if(flags & DND_WDMG_ISPISTOL) {
			temp = DND_GUNSLINGER_GAIN * CheckInventory(Quest_List[QUEST_ONLYPISTOLWEAPONS].qreward);
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, dmgid, temp);
		}
		
		// shotgun affected
		if(flags & DND_WDMG_ISBOOMSTICK) {
			temp = DND_BOOMSTICK_GAIN * CheckInventory(Quest_List[QUEST_NOSHOTGUNS].qreward);
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, dmgid, temp);
		}
			
		// super weapon affected
		if(flags & DND_WDMG_ISSUPER) {
			temp = DND_SUPERWEAPON_GAIN * CheckInventory(Quest_List[QUEST_NOSUPERWEAPONS].qreward);
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, dmgid, temp);
		}
		
		// include the stat bonus
		if(talent_type == TALENT_MELEE) {
			temp = DND_STR_GAIN * GetStrength();
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, dmgid, temp);
		}
		
		// occult uses intellect
		if(flags & DND_WDMG_ISOCCULT || talent_type == TALENT_OCCULT) {
			temp = DND_INT_GAIN * GetIntellect();
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, dmgid, temp);
		}
		else if(talent_type != TALENT_MELEE) {
			temp = DND_DEX_GAIN * GetDexterity();
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, dmgid, temp);
		}
		
		
		if(active_quest_id == QUEST_ONLYNOTALENTWEAPONS && (CheckInventory("DnD_NonLowestTalents") & (1 << talent_type)))
			GiveInventory(Quest_Checkers[QUEST_ONLYNOTALENTWEAPONS], 1);

		// include enhancement orb bonuses
		temp = GetPlayerWeaponEnchant(pnum, wepid);
		if(temp) {
			temp *= ENHANCEORB_BONUS;
			dmg = dmg * (100 + temp) / 100;
		
			InsertCacheFactor(pnum, dmgid, temp);
		}
			
		// finally apply damage type bonuses
		temp = 	GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, talent_type) + 
				GetPlayerAttributeValue(pnum, INV_DAMAGEPERCENT_INCREASE) + 
				MapTalentToPercentBonus(pnum, talent_type) +
				Player_Elixir_Bonuses[pnum].damage_type_bonus[talent_type];
		if(temp) {
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, dmgid, temp);
		}
		
		// slot damage bonuses
		temp = GetPlayerAttributeValue(pnum, INV_SLOT1_DAMAGE + GetWeaponSlotFromFlag(flags));
		if(temp) {
			dmg = dmg * (100 + temp) / 100;
			InsertCacheFactor(pnum, dmgid, temp);
		}
		
		// special damage increase attributes -- usually obtained by means of charms
		temp = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_LIGHTNING);
		if(temp && IsWeaponLightningType(wepid, dmgid, isSpecial)) {
			dmg = dmg * (100 + temp) / 100;
			InsertCacheFactor(pnum, dmgid, temp);
		}
		
		// shotgun damage bonus -- add hobos perk here too
		temp = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_SHOTGUNS);
		if(CheckInventory("Hobo_Perk25"))
			temp += DND_HOBO_SHOTGUNBONUS;
			
		if((flags & DND_WDMG_ISBOOMSTICK) && temp) {
			dmg = dmg * (100 + temp) / 100;
			InsertCacheFactor(pnum, dmgid, temp);
		}
		// apply flat health to damage conversion if player has any
		temp = GetPlayerAttributeValue(pnum, INV_EX_DAMAGEPER_FLATHEALTH);
		if(temp) {
			temp = GetFlatHealthDamageFactor(temp);
			dmg = dmg * (100 + temp) / 100;
		
			InsertCacheFactor(pnum, dmgid, temp);
		}

		MarkCachingComplete(pnum, dmgid);
		printbold(s:"from regular");
	}
	else {
		// Get the cached flat dmg and factor and apply them both
		dmg += GetCachedPlayerFlatDamage(pnum, dmgid);
		dmg *= GetCachedPlayerFactor(pnum, dmgid);
		
		// factor was stored as fixed, convert to int
		dmg >>= 16;
		printbold(s:"from cache ", d:GetCachedPlayerFlatDamage(pnum, dmgid), s: " ", f:GetCachedPlayerFactor(pnum, dmgid));
	}
	
	printbold(d:dmg);
	
	return dmg;
}

int HandleWeaponCrit(int dmg, int wepid, int pnum, int dmgid, bool isSpecial) {
	// crit chance
	// if weapon id is a lightning type, it will always crit with the necessary charm attribute on
	if((IsWeaponLightningType(wepid, dmgid, isSpecial) && IsSet(GetPlayerAttributeValue(pnum, INV_EX_ALWAYSCRIT_LIGHTNING), DND_STATBUFF_ALWAYSCRITLIGHTNING)) || CheckCritChance(wepid)) {
		dmg = dmg * GetCritModifier() / 100;
		HandleHunterTalisman();
	}
	return dmg;
}

// returns the filtered, reduced etc. damage when factoring in all resists or weaknesses ie. this is the final damage the actor will take
// dmg value is FIXED POINT NOT INT!
// This is strictly for player doing damage to other monsters!
void HandleDamageDeal(int source, int victim, int dmg, int damage_type, int flags) {
	str s_damagetype = DamageTypeList[damage_type];
	// for now, just add full suffix at the end, later we'll instead make them bypass resists properly with numbers
	if(
		CheckActorInventory(source, "NetherCheck") 																				|| 
		(damage_type == DND_DAMAGETYPE_OCCULT) && CheckActorInventory(source, "DnD_QuestReward_DreamingGodBonus")				||
		(damage_type == DND_DAMAGETYPE_ENERGY && CheckActorInventory(source, "Cyborg_Perk50")) 									||
		((flags & DND_DAMAGEFLAG_ISSHOTGUN) && CheckActorInventory(source, "Hobo_Perk50"))
	)
		s_damagetype = StrParam(s:s_damagetype, s:"Full");
	
	dmg >>= 16;
	Thing_Damage2(victim, dmg, DamageTypeList[damage_type]);
}

void DoExplosionDamage(int owner, int dmg, int radius, int fullradius, int damage_type) {
	int pnum = owner - P_TIDSTART;
	// extract flags off damage type
	int flags = damage_type >> 16;
	damage_type = 0xFFFF;
	
	int instance = PlayerExplosionList[pnum].curr_instance;
	int lim = PlayerExplosionList[pnum].list[instance].amt;
	
	int px = GetActorX(0), py = GetActorY(0), pz = GetActorZ(0);
	int proj_r = GetActorProperty(0, APROP_RADIUS);

	// set activator to player for dmg registry
	SetActivator(P_TIDSTART + pnum);
	
	// turn them to fixed
	radius <<= 16;
	fullradius <<= 16;
	dmg <<= 16;
	for(int i = 0; i < lim; ++i) {
		int mon_id = PlayerExplosionList[pnum].list[instance].monsters[i];

		// first check if this monster is immune to splash damage (its an easy flag check and eases calculation later)
		if(CheckFlag(mon_id, "NORADIUSDMG") && !CheckActorInventory(owner, "NetherCheck") && !(flags & DND_DAMAGEFLAG_FORCERADIUSDMG))
			continue;
		
		// calculate damage falloff based on distance -- subtract projectile's radius from distance to get a better estimate
		// idea here: monster hitboxes are actual rectangles and not circles, so unless hit occured perpendicular to the hitbox, you won't deal max damage
		// to fix that, subtract (r + r * sqrt2) / 2, which is 1.207. Reason: radius scales from r to r * sqrt2 over the square's center to diagonal.
		int final_dmg = dmg;
		int dist = fdistance_delta(px - GetActorX(mon_id), py - GetActorY(mon_id), pz - GetActorZ(mon_id));
		dist -= FixedMul(GetActorProperty(mon_id, APROP_RADIUS) + proj_r, 1.207);
		
		// printbold(s:"check dist: ", f:dist, s: " ", f:radius, s: " ", f:fullradius);
		
		// not within range, skip
		if(dist >= radius)
			continue;
			
		// if full radius is more or equal, then we dont need to consider distance as a factor for scaling damage further
		if(fullradius < radius) {
			if(dist < 0)
				dist = 0;
			// we will reduce damage if we are past fullradius and within radius
			if(dist > fullradius && dist <= radius) {
				// printbold(s:"dist ", f:dist, s:" factor ", f:FixedDiv(radius - dist, radius - fullradius));
				final_dmg = FixedMul(final_dmg, FixedDiv(radius - dist, radius - fullradius));
			}
		}
		
		// dont deal 0 dmg
		if(!final_dmg)
			final_dmg = 1.0;
		
		HandleDamageDeal(owner, mon_id, final_dmg, damage_type, flags);
		
		//printbold(s:"Dealing ", d: final_dmg, s: " damage to ", d:mon_id, s: " of type ", s:DamageTypeList[damage_type]);
	}
	
	// damage is dealt we are done with this instance, free it up
	Free_Explosion_Instance(pnum, instance);
}

Script "DnD Do Explosion Damage" (int dmg, int radius, int fullradius, int damage_type) {
	// player information
	int owner = GetActorProperty(0, APROP_TARGETTID);
	DoExplosionDamage(owner, dmg, radius, fullradius, damage_type);
	
	SetResultValue(0);
}

Script "DnD Do Explosion Damage (Pets)" (int dmg, int radius, int fullradius, int damage_type) {
	// player information
	int owner = CheckInventory("DnD_ProjPnum") + P_TIDSTART;
	DoExplosionDamage(owner, dmg, radius, fullradius, damage_type);
	
	SetResultValue(0);
}

Script "DnD Do Impact Damage" (int dmg, int damage_type, int flags) {
	int owner = GetActorProperty(0, APROP_TARGETTID);
	int victim = GetActorProperty(0, APROP_TRACERTID);
	
	// printbold(d:owner, s: " ", d:victim);
	SetActivator(owner);
	
	if(owner && victim)
		HandleDamageDeal(owner, victim, dmg << 16, damage_type, flags);
	
	SetResultValue(0);
}

#endif
