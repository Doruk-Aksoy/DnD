#ifndef DND_DAMAGE_IN
#define DND_DAMAGE_IN

#include "DnD_Poison.h"
#include "DnD_Physics.h"

#define DND_CRIT_TOKEN 69

#define DND_PLAYER_HITSCAN_Z 38.0
#define MAX_RIPPERS_ACTIVE 256
#define MAX_RIPPER_HITS_STORED 128

#define DND_RUINATION_REDUCE_PER_STACK 5

#define DND_MAX_DAMAGELOSEHITS 7 // we let maximum of 70% reduction, so you'll do min 30%
#define DND_DAMAGELOST_PERCENT 10 // 10%

#define DND_HARDCORE_DEBUFF 15 // 15% more damage taken

#define DND_BASE_POISON_FACTOR 2

#define DND_INVULSPHERE_FACTOR 10 // 10% damage taken, divides by 10 => 90% reduce

#define DND_EXPLOSION_FLAGVARIABLE "user_flags"

#define DND_BERSERKER_DAMAGETRACKTIME 17 // 3 is base, x 5 -- +2 for 0.5 second of buffer inclusion
#define DND_BERSERKER_PERK25_MAXSTACKS 15
#define DND_BERSERKER_PERK25_HEALPERCENT 15
#define DND_BERSERKER_PERK25_REDUCTION 2 // 2% per stack
#define DND_BERSERKER_PERK50_TIMER 14 // 14 x 5 = 70 => 2 seconds
#define DND_BERSERKER_PERK50_DMGINCREASE 8 // 8%

#define DND_MONSTER_PERCENTDAMAGEBASE 10 // 10%

#define DND_MONSTER_POISONPERCENT 20 // 20% of damage taken from a hit is dealt as poison damage again over the duration
#define DND_MONSTER_POISONDOT_MINTIME 2
#define DND_MONSTER_POISONDOT_MAXTIME 5

#define OCCULT_WEAKEN_DURATION 2

#define DND_CORRUPTORB_DIV 4 // /4 => 75% reduced dmg

enum {
	DND_DAMAGETYPE_MELEE,
	DND_DAMAGETYPE_MELEEOCCULT,
	DND_DAMAGETYPE_PHYSICAL,
	DND_DAMAGETYPE_SILVERBULLET,
	DND_DAMAGETYPE_ENERGY,
	DND_DAMAGETYPE_ENERGYEXPLOSION,
	DND_DAMAGETYPE_EXPLOSIVES,
	
	// occult
	DND_DAMAGETYPE_OCCULT,
	DND_DAMAGETYPE_OCCULTFIRE,
	DND_DAMAGETYPE_OCCULTEXPLOSION,
	DND_DAMAGETYPE_MAGICSEAL,

	// elemental types
	DND_DAMAGETYPE_FIRE,
	DND_DAMAGETYPE_ICE,
	DND_DAMAGETYPE_POISON,
	DND_DAMAGETYPE_DESOLATOR,
	DND_DAMAGETYPE_EMERALD,
	DND_DAMAGETYPE_LIGHTNING,
	
	DND_DAMAGETYPE_SOUL
};

// monster flag encoding
enum {
	DND_DAMAGETYPEFLAG_PHYSICAL = 1,
	DND_DAMAGETYPEFLAG_HITSCAN = 2,
	DND_DAMAGETYPEFLAG_EXPLOSIVE = 4,
	DND_DAMAGETYPEFLAG_MAGICAL = 8,
	DND_DAMAGETYPEFLAG_ENERGY = 16,
	DND_DAMAGETYPEFLAG_FIRE = 32,
	DND_DAMAGETYPEFLAG_ICE = 64,
	DND_DAMAGETYPEFLAG_POISON = 128,
	DND_DAMAGETYPEFLAG_LIGHTNING = 256,
	DND_DAMAGETYPEFLAG_PERCENTHP = 512,
};

#define MAX_DAMAGE_TYPES (DND_DAMAGETYPE_SOUL + 1)
#define DAMAGE_TYPE_SHIFT 5
#define DAMAGE_TYPE_MASK 0x1F

bool IsMeleeDamage(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_MELEE && damage_type <= DND_DAMAGETYPE_MELEEOCCULT;
}

bool IsBulletDamage(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_PHYSICAL && damage_type <= DND_DAMAGETYPE_SILVERBULLET;
}

bool IsExplosionDamage(int damage_type) {
	return damage_type == DND_DAMAGETYPE_ENERGYEXPLOSION || damage_type == DND_DAMAGETYPE_OCCULTEXPLOSION || damage_type == DND_DAMAGETYPE_EXPLOSIVES;
}

bool IsEnergyDamage(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_ENERGY && damage_type <= DND_DAMAGETYPE_ENERGYEXPLOSION;
}

bool IsOccultDamage(int damage_type) {
	return (damage_type >= DND_DAMAGETYPE_OCCULT && damage_type <= DND_DAMAGETYPE_MAGICSEAL) || damage_type == DND_DAMAGETYPE_MELEEOCCULT || damage_type == DND_DAMAGETYPE_SOUL;
}

bool IsFireDamage(int damage_type) {
	return damage_type == DND_DAMAGETYPE_OCCULTFIRE || damage_type == DND_DAMAGETYPE_FIRE;
}

bool IsIceDamage(int damage_type) {
	return damage_type == DND_DAMAGETYPE_ICE;
}

bool IsPoisonDamage(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_POISON && damage_type <= DND_DAMAGETYPE_EMERALD;
}

bool IsLightningDamage(int damage_type) {
	return damage_type == DND_DAMAGETYPE_LIGHTNING;
}

bool IsElementalDamage(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_FIRE && damage_type <= DND_DAMAGETYPE_LIGHTNING;
}

int GetDamageCategory(int damage_type, int flags) {
	if(IsBulletDamage(damage_type))
		return DND_DAMAGECATEGORY_BULLET;
	else if(IsMeleeDamage(damage_type) && damage_type != DND_DAMAGETYPE_MELEEOCCULT) // little note: while the weapon is melee and can benefit from melee bonuses, if it's occult damage type it'd be occult category
		return DND_DAMAGECATEGORY_MELEE;
	else if(damage_type == DND_DAMAGETYPE_EXPLOSIVES)
		return DND_DAMAGECATEGORY_EXPLOSIVES;
	else if(IsEnergyDamage(damage_type))
		return DND_DAMAGECATEGORY_ENERGY;
	else if(IsElementalDamage(damage_type))
		return DND_DAMAGECATEGORY_ELEMENTAL;
	else if(damage_type == DND_DAMAGETYPE_SOUL || (flags & DND_DAMAGEFLAG_SOULATTACK))
		return DND_DAMAGECATEGORY_SOUL;
	// switched soul up because IsOccultDamage contains soul damage in it, and it'd return occult, absorbing other checks
	return DND_DAMAGECATEGORY_OCCULT;
}

str DamageTypeList[MAX_DAMAGE_TYPES] = {
	"Melee",
	"Melee_Magic",
	"Bullet",
	"BulletMagicX",
	"Energy",
	"EnergyExp",
	"Explosives",
	"Magic",
	"MagicFire",
	"Explosives_Magic",
	"MagicSealing",
	
	// elemental
	"Fire",
	"P_Ice",
	"Poison",
	"Desolator",
	"Emerald",
	"Lightning",
	
	"SoulType"
};

vec3_T PlayerDamageVector[MAXPLAYERS];

enum {
	DND_SPECIALBLOOD_STONE
};

#define MAX_BLOOD_TYPES (DND_SPECIALBLOOD_STONE + 1)
str SpecialBloodFX[MAX_BLOOD_TYPES] = {
	"StoneBitsSpawner"
};

enum {
	DND_HITBEEP_WEAKNESS,
	DND_HITBEEP_RESIST,
	DND_HITBEEP_IMMUNITY,
	DND_HITBEEP_INVULNERABLE
};
#define DND_MAX_HITBEEPS (DND_HITBEEP_INVULNERABLE + 1)
#define HITBEEP_SOUND 0
#define HITBEEP_TIMER 1
str HitBeepSounds[DND_MAX_HITBEEPS][2] = {
	{ "HitBeep/Weakness", 		"WeaknessBeepTimer"		},
	{ "HitBeep/Resist", 		"BeepTimer"				},
	{ "HitBeep/Immune", 		"HardBeepTimer"			},
	{ "HitBeep/Invulnerable",  	"InvulBeepTimer"		}
};

#define DND_CULL_BASEPERCENT 15 // percent
#define DND_DESOLATOR_DMG_GAIN 10 // 10%
#define DND_DISTANCEDAMAGE_VARIABLE "user_tics"

#define DND_BASE_FREEZETIMER 21 // 3 seconds base time (21 x 5 = 105)
#define DND_BASE_CHILL_CAP 5 // 50% health dealt in ice = maximum slow

#define DND_BASE_IGNITETIMER 20 // 4 seconds x 5

#define DND_BASE_OVERLOADCHANCE 5
#define DND_BASE_OVERLOADBUFF 20 // 20%
#define DND_MAX_OVERLOADTARGETS 128 // up to 128 allowed
#define DND_BASE_OVERLOADZAPDELAY 3 // 3 tics

#define DND_BASE_POISON_STACKS 5
#define DND_BASE_POISON_TIMER 3.0

#define DND_EXTRAUNDEADDMG_MULTIPLIER 3

#define DND_WEAKNESS_FACTOR 25 // 25% extra dmg
#define DND_SPECIFICELEWEAKNESS_FACTOR 50 // 50% extra dmg taken from specific elemental dmg
#define DND_RESIST_FACTOR 50 // 50% dmg taken
#define DND_IMMUNITY_FACTOR 95 // 5% dmg taken
#define DND_IMMUNITY_HARDCAP_FACTOR 99 // 1% damage taken

#define DND_MAX_MONSTER_TICDATA 16383 // even this is a bit much but w.e
#define DND_MONSTER_TICDATA_BITMASK 0x3FFF // 14 bits
#define DND_DAMAGE_ACCUM_SHIFT 14 // 2^14 = 16384
global int 27: PlayerDamageTicData[MAXPLAYERS][DND_MAX_MONSTER_TICDATA];

// All resists uniformly follow same factors
int ApplyPlayerResist(int pnum, int dmg, int res_attribute) {
	int temp = GetPlayerAttributeValue(pnum, res_attribute);
	if(!temp)
		return dmg;
	
	// roll damage up
	temp = ApplyResistCap(pnum, temp) + 0.05;
	
	//return ApplyDamageFactor_Safe(dmg, DND_DAMAGERESIST_FACTOR - ((temp * 100) >> 16), DND_DAMAGERESIST_FACTOR);
	return dmg * ((100.0 - temp) >> 16) / 100;
}

bool AdjustDamageRetrievePointers(int flags, bool crit_check = false, int wepid = -1) {
	int temp;
	bool res = false;
	//printbold(s:"prev score? ", d:GetActorProperty(0, APROP_SCORE));
	if(flags & DND_WDMG_USETARGET) { // use target
		// hopefully no projectile uses score
		if(!GetActorProperty(0, APROP_SCORE)) {
			temp = GetActorProperty(0, APROP_TARGETTID);
			SetActorProperty(0, APROP_SCORE, temp);
			
			res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
			SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);
			
			SetActivator(0, AAPTR_TARGET);
		}
	}
	else if(flags & DND_WDMG_USEMASTER) {
		if(flags & DND_WDMG_SETMASTER) {
			temp = GetActorProperty(0, APROP_MASTERTID);
		
			res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
			SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);
		
			// this is a hack
			SetPointer(AAPTR_TARGET, temp);
		}
		if(!GetActorProperty(0, APROP_SCORE)) {
			temp = GetActorProperty(0, APROP_TARGETTID);
		
			SetActorProperty(0, APROP_SCORE, temp);
			
			res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
			SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);
			
			SetActivator(GetActorProperty(0, APROP_MASTERTID));
		}
	}
	else if(flags & DND_WDMG_USETRACER) {
		temp = GetActorProperty(0, APROP_TRACERTID);
		
		res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
		SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);
	
		SetActivator(0, AAPTR_TRACER);
	}
	else if(flags & DND_WDMG_USETARGETSMASTER) {
		temp = ACS_NamedExecuteWithResult("DnD Get Master of Target");
		if(!GetActorProperty(0, APROP_SCORE))
			SetActorProperty(0, APROP_SCORE, temp);
		
		res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
		SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);
		
		SetActivator(temp);
	}

	// printbold(s:"owner ", d:ActivatorTID(), s: " ", d:GetActorProperty(0, APROP_SCORE));
	// no owner can potentially mean this was a reflected projectile, we can manipulate this a bit
	if(!ActivatorTID()) {
		GiveInventory("MarkAsReflected", 1);
		SetActivator(GetActorProperty(0, APROP_SCORE));
	}
	
	return res;
}

void HandleOnHitEffects(int owner) {
	if(HasActorMasteredPerk(owner, STAT_DED)) {
		if(!CheckActorInventory(owner, "DnD_DeadlinessMasteryWindow")) {
			// didn't have before, reset (or had some, we dont care, it needs to be reset)
			// reset to 1, as this is a hit by itself
			SetActorInventory(owner, "DnD_DeadlinessMasteryCounter", 1);
			GiveActorInventory(owner, "DnD_DeadlinessMasteryWindow", 1);
		}
		else {
			GiveActorInventory(owner, "DnD_DeadlinessMasteryCounter", 1);
			if(CheckActorInventory(owner, "DnD_DeadlinessMasteryCounter") == DND_MASTERY_DEADLINESSCOUNTER)
				GiveActorInventory(owner, "DnD_GuaranteeCrit_FromDeadliness", 1);
		}
	}
}

int BigNumberFormula(int dmg, int f) {
	int p = PowersOf10[(digitcount(dmg) + 1) / 2];
	int wepid = dmg % p;
	int temp = 0;
	dmg /= p;
	
	// get rid of 0.9999 crap
	f = f * 100 + 0.001;
	f >>= 16;
	
	dmg *= f;
	temp = (dmg % 100) * p / 100;
	dmg /= 100;
	dmg *= p;
	
	wepid *= f;
	wepid /= 100;
	
	dmg += wepid + temp;

	return dmg;
}

// Factors in generic DOT percentages to a base damage, use for weapons that do DOT on their own!
int FactorDOT(int pnum, int dmg, int percent_increase = 0) {
	// flat portion
	dmg += GetPlayerAttributeValue(pnum, INV_EX_FLATDOT);

	// dot %
	dmg = dmg * (100 + GetPlayerAttributeValue(pnum, INV_INCREASEDDOT) + percent_increase) / 100;
		
	// dot multi
	dmg = dmg * (100 + GetPlayerAttributeValue(pnum, INV_DOTMULTI)) / 100;
	
	return dmg;
}

// set pointers appropriately beforehand!
int RetrieveWeaponDamage(int pnum, int wepid, int dmgid, int damage_category, int flags, int isSpecial, bool proj_crit) {
	// do not lose the weaponid on special ammo -- normally its DMG_ID & (wepid << 16) but special ammo just have the id of the special ammo instead of dmg_id
	// add +1 because flechette is id 0
	if(isSpecial) {
		isSpecial = dmgid + 1;
		dmgid = wepid;
		wepid = CheckInventory("DnD_WeaponID");
	}
	
	//printbold(s:"retrieved acc ", d:GetActorProperty(0, APROP_ACCURACY));

	int res = ScaleCachedDamage(wepid, pnum, dmgid, damage_category, flags, isSpecial);
	
	// special weapons checks -- these are added on top of everything else as the last thing, before crits
	// they are also dynamic and cant be cached...
	if(wepid == DND_WEAPON_LIGHTNINGGUN)
		res = res * (100 + DND_LIGHTNINGGUN_DMGPERSTACK * CheckInventory("LightningStacks")) / 100;
	else if(wepid == DND_WEAPON_DUSKBLADE)
		res = res * (100 + DND_DUSKBLADE_DMGPERSTACK * CheckInventory("SwordHitCharge")) / 100;
		
	return res;
}

int ApplyNonWeaponBaseDamageBonus(int tid, int dmg, int damage_type, int flags) {
	// we need to assign a damage category to this first
	//printbold(d:damage_type, s: " ", d:flags);
	int damage_category = GetDamageCategory(damage_type, flags);
	int damage_category_flags = 0;
	int pnum = tid - P_TIDSTART;
	
	damage_category_flags |= 	(IsFireDamage(damage_type) & DND_WDMG_FIREDAMAGE) 		| 
								(IsFireDamage(damage_type) & DND_WDMG_ICEDAMAGE) 		|
								(IsPoisonDamage(damage_type) & DND_WDMG_POISONDAMAGE) 	|
								(IsLightningDamage(damage_type) & DND_WDMG_LIGHTNINGDAMAGE);
	
	//printbold(s:"add ", d:MapDamageCategoryToFlatBonus(pnum, damage_category, damage_category_flags));
	dmg += MapDamageCategoryToFlatBonus(pnum, damage_category, damage_category_flags);
	
	// overall percentage bonuses -- this is basically ScaleCachedDamage but unwrapped, we need to rewrite these into a common function that just retrieves the overall bonus factor to multiply with!
	int factor = 100 + GetPlayerPercentDamage(pnum, -1, damage_category);
	
	// specialty armor bonuses
	/*int temp = GetArmorID();
	if(
		(temp == BODYARMOR_GUNSLINGER 	&& damage_category == DND_DAMAGECATEGORY_BULLET) 			||
		(temp == BODYARMOR_OCCULT 		&& damage_category == DND_DAMAGECATEGORY_OCCULT)			||
		(temp == BODYARMOR_DEMO 		&& damage_category == DND_DAMAGECATEGORY_EXPLOSIVES)		||
		(temp == BODYARMOR_ENERGY 		&& damage_category == DND_DAMAGECATEGORY_ENERGY)			||
		(temp == BODYARMOR_ELEMENTAL 	&& damage_category == DND_DAMAGECATEGORY_ELEMENTAL)
	)
	{
		factor += DND_SPECIALTYARMOR_BUFF;
	}*/
	
	// apply flat health to damage conversion if player has any
	int temp = GetPlayerAttributeValue(pnum, INV_EX_PHYSDAMAGEPER_FLATHEALTH);
	if((damage_category == DND_DAMAGECATEGORY_MELEE || damage_category == DND_DAMAGECATEGORY_BULLET) && temp)
		factor += GetFlatHealthDamageFactor(temp);
		
	temp = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_LIGHTNING);
	if(temp && (damage_category_flags & DND_WDMG_LIGHTNINGDAMAGE))
		factor += temp;
		
	if(damage_category == DND_DAMAGECATEGORY_ELEMENTAL && IsQuestComplete(tid, QUEST_KILLMORDECQAI))
		factor += DND_MORDECQAI_BOOST;

	// THESE ARE MULTIPLICATIVE STACKING BONUSES BELOW -- HAVE KEYWORD: MORE
	// quest or accessory bonuses	
	// is occult (add demon bane bonus)
	// apply wanderer perk if applicable
	if((damage_category == DND_DAMAGECATEGORY_OCCULT || damage_category == DND_DAMAGECATEGORY_ELEMENTAL) && CheckActorInventory(tid, "Wanderer_Perk25"))
		factor = factor * (100 + DND_WANDERER_PERK25_BUFF) / 100;
	
	if(damage_category == DND_DAMAGECATEGORY_OCCULT)
		factor = factor * (100 + DND_DEMONBANE_GAIN * (!!IsAccessoryEquipped(tid, DND_ACCESSORY_DEMONBANE))) / 100;
	
	// % more damage from charms -- already contains 100 in it as it's a multiplicative mod -- its also fixed!
	if(!(flags & DND_DAMAGEFLAG_ISSPELL)) {
		temp = (GetPlayerAttributeValue(pnum, INV_DAMAGEPERCENT_MORE) * 100) >> 16;
		if(temp)
			factor = factor * (100 + temp) / 100;
	}
		
	// if we had a factor of 0, dont bother here
	if(!factor)
		return dmg;
		
	//printbold(s:"dmg factor mult by ", d:factor, s: " base dmg: ", d:dmg, s: " end result: ", d:dmg * factor / 100);
		
	if(dmg < INT_MAX / factor) {
		dmg *= factor;
		dmg /= 100;
		// no longer fixed
		//dmg >>= 16;
	}
	else {
		// beyond this point wepid doesnt matter so use that instead
		dmg = INT_MAX;//BigNumberFormula(dmg, factor);
	}
	
	return dmg;
}

// use only flags with DND_WDMG header here!!!
// NOTE: DO NOT FACTOR ANY DOT MULTIPLIER IN HERE!
// isSpecial is id of the special ammo + 1
// special ammo replaces dmgid 0 of the weapon in cache, so everytime we switch special ammo type we must force damage cache recalc
int ScaleCachedDamage(int wepid, int pnum, int dmgid, int damage_category, int flags, int isSpecial) {
	// we don't cache special ammo damage
	int dmg = 0;
	int temp;
	int tid = pnum + P_TIDSTART;
	// get the damage
	if(!isSpecial)
		temp = GetCachedPlayerDamage(pnum, wepid, dmgid);
	else {
		// special ammo damage
		temp = GetSpecialAmmoDamage(isSpecial - 1, dmgid);
		
		// specialist quest bonus
		if(IsQuestComplete(tid, QUEST_ONLYSPECIALAMMO))
			temp += temp * DND_QUEST_SPECIALGAIN / 100;
	}
		
	// check if we have a random range cached -- special ammo types dont use this
	int range = GetCachedPlayerRandomRange(pnum, wepid, dmgid);
	if(range > 1 && !isSpecial)
		dmg += temp * random(range & 0xFFFF, range >> 16);
	else // no rng, so just set it to temp
		dmg = temp;

	bool is_melee_mastery_exception = (IsMeleeWeapon(wepid) || (flags & DND_WDMG_ISMELEE)) && HasMasteredPerk(STAT_BRUT);

	// only store scaling factors here for later use, no modifying damage in this block
	// damage modifications are done at the end
	if(PlayerDamageNeedsCaching(pnum, wepid, dmgid)) {
		// add potential shotgun flat damage
		temp = (!!(flags & DND_WDMG_ISBOOMSTICK)) * GetPlayerAttributeValue(pnum, INV_EX_FLATPERSHOTGUNOWNED) * CountShotgunWeaponsOwned();
		
		// add flat damage bonus mapping talent name to flat bonus type
		temp += MapDamageCategoryToFlatBonus(pnum, damage_category, flags);
		
		ClearCache(pnum, wepid, dmgid);
		
		if(flags & DND_WDMG_ISDOT)
			temp += GetPlayerAttributeValue(pnum, INV_EX_FLATDOT);
		
		CachePlayerFlatDamage(pnum, temp, wepid, dmgid);
		
		int mult_factor = 0;
		
		// include the stat bonus
		//printbold(d:talent_type == TALENT_MELEE, s: " ", d: IsMeleeWeapon(wepid), s: " ", d:(flags & DND_WDMG_ISMELEE));
		if(damage_category == DND_DAMAGECATEGORY_MELEE || is_melee_mastery_exception) {
			InsertCacheFactor(pnum, wepid, dmgid, GetMeleeDamage(pnum), true);
			//printbold(s:"factor added ", d:temp);
		}
		
		// occult uses intellect
		if((flags & DND_WDMG_ISOCCULT) || damage_category == DND_DAMAGECATEGORY_OCCULT)
			InsertCacheFactor(pnum, wepid, dmgid, GetRangedBonus(pnum, true), true);
		else if(damage_category != DND_DAMAGECATEGORY_MELEE)
			InsertCacheFactor(pnum, wepid, dmgid, GetRangedBonus(pnum), true);
		
		// specialty armor bonuses
		temp = GetArmorID();
		if(
			(temp == BODYARMOR_GUNSLINGER 	&& damage_category == DND_DAMAGECATEGORY_BULLET) 			||
			(temp == BODYARMOR_OCCULT 		&& damage_category == DND_DAMAGECATEGORY_OCCULT)			||
			(temp == BODYARMOR_DEMO 		&& damage_category == DND_DAMAGECATEGORY_EXPLOSIVES)		||
			(temp == BODYARMOR_ENERGY 		&& damage_category == DND_DAMAGECATEGORY_ENERGY)			||
			(temp == BODYARMOR_ELEMENTAL 	&& damage_category == DND_DAMAGECATEGORY_ELEMENTAL)
		)
		{
			InsertCacheFactor(pnum, wepid, dmgid, DND_SPECIALTYARMOR_BUFF, true);
		}

		// include enhancement orb bonuses
		temp = GetPlayerWeaponEnchant(pnum, wepid);
		if(temp) {
			temp *= ENHANCEORB_BONUS;
			InsertCacheFactor(pnum, wepid, dmgid, temp, true);
		}
			
		// finally apply damage type or percentage bonuses
		// last one is for ghost hit power, we reduce its power by a factor
		temp = GetPlayerPercentDamage(pnum, wepid, damage_category);
		if(damage_category != DND_DAMAGECATEGORY_MELEE && is_melee_mastery_exception)
			temp += GetPlayerPercentDamage(pnum, wepid, DND_DAMAGECATEGORY_MELEE);
		if(temp)
			InsertCacheFactor(pnum, wepid, dmgid, temp, true);
		
		// slot damage bonuses
		temp = GetPlayerAttributeValue(pnum, INV_SLOT1_DAMAGE + GetWeaponSlotFromFlag(flags));
		if(temp)
			InsertCacheFactor(pnum, wepid, dmgid, temp, true);
		
		// special damage increase attributes -- usually obtained by means of charms
		temp = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_LIGHTNING);
		if(temp && IsWeaponLightningType(wepid, dmgid, isSpecial - 1))
			InsertCacheFactor(pnum, wepid, dmgid, temp, true);
		
		// shotgun damage bonus
		temp = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_SHOTGUNS);
		if(flags & DND_WDMG_ISBOOMSTICK)
			InsertCacheFactor(pnum, wepid, dmgid, temp, true);
			
		// apply flat health to damage conversion if player has any
		temp = GetPlayerAttributeValue(pnum, INV_EX_PHYSDAMAGEPER_FLATHEALTH);
		if((damage_category == DND_DAMAGECATEGORY_MELEE || damage_category == DND_DAMAGECATEGORY_BULLET) && temp)
			InsertCacheFactor(pnum, wepid, dmgid, GetFlatHealthDamageFactor(temp), true);
			
		// factor dot % increase if this is a dot attack
		if(flags & DND_WDMG_ISDOT)
			InsertCacheFactor(pnum, wepid, dmgid, GetPlayerAttributeValue(pnum, INV_INCREASEDDOT), true);
			
		// check for quest % increases
		if(IsQuestComplete(tid, QUEST_ONLYONEWEAPON))
			InsertCacheFactor(pnum, wepid, dmgid, DND_QUEST_ONEWEAPON_BONUS, true);
			
		if(damage_category == DND_DAMAGECATEGORY_ELEMENTAL && IsQuestComplete(tid, QUEST_KILLMORDECQAI))
			InsertCacheFactor(pnum, wepid, dmgid, DND_MORDECQAI_BOOST, true);

		// THESE ARE MULTIPLICATIVE STACKING BONUSES BELOW -- HAVE KEYWORD: MORE
		// quest or accessory bonuses	
		// is occult (add demon bane bonus)
		// apply wanderer perk if applicable
		if((damage_category == DND_DAMAGECATEGORY_OCCULT || damage_category == DND_DAMAGECATEGORY_ELEMENTAL) && CheckInventory("Wanderer_Perk25"))
			InsertCacheFactor(pnum, wepid, dmgid, DND_WANDERER_PERK25_BUFF, false);
			
		// hobo perk if applicable
		if((flags & DND_WDMG_ISBOOMSTICK) && CheckInventory("Hobo_Perk25"))
			InsertCacheFactor(pnum, wepid, dmgid, DND_HOBO_SHOTGUNBONUS, false);
		
		if(flags & DND_WDMG_ISOCCULT || damage_category == DND_DAMAGECATEGORY_OCCULT)
			InsertCacheFactor(pnum, wepid, dmgid, DND_DEMONBANE_GAIN * (!!IsAccessoryEquipped(tid, DND_ACCESSORY_DEMONBANE)), false);
		
		// these HOPEFULLY dont have anything in common... yet?
		if(flags & DND_WDMG_ISPISTOL) // gunslinger affected
			InsertCacheFactor(pnum, wepid, dmgid, DND_QUEST_SLOT2BONUS * IsQuestComplete(tid, QUEST_ONLYPISTOLWEAPONS), false);
		else if(flags & DND_WDMG_ISBOOMSTICK) // shotgun affected
			InsertCacheFactor(pnum, wepid, dmgid, DND_QUEST_SLOT3BONUS * IsQuestComplete(tid, QUEST_NOSHOTGUNS), false);
		else if(flags & DND_WDMG_ISSUPER) // super weapon affected
			InsertCacheFactor(pnum, wepid, dmgid, DND_QUEST_SUPERGAIN * IsQuestComplete(tid, QUEST_NOSUPERWEAPONS), false);
		
		// add other multiplicative factors below
		
		// % more damage from charms -- already contains 100 in it as it's a multiplicative mod
		temp = GetPlayerAttributeValue(pnum, INV_DAMAGEPERCENT_MORE);
		if(temp)
			InsertCacheFactor_Fixed(pnum, wepid, dmgid, temp);
			
		// % more / less damage from wepmod or orbs
		temp = GetWeaponModValue(pnum, wepid, WEP_MOD_DMG);
		if(temp)
			InsertCacheFactor(pnum, wepid, dmgid, temp, false);
		
		// perk multiplicative factors
		if(mult_factor)
			InsertCacheFactor(pnum, wepid, dmgid, mult_factor, false);

		MarkCachingComplete(pnum, wepid, dmgid);
		
		//printbold(s:"pre-scale: ", d:temp);
	}
	
	// Get the cached flat dmg and factor and apply them both
	temp = GetCachedPlayerFlatDamage(pnum, wepid, dmgid);
	
	// isSpecial isn't used or kept track of below here, so re-use
	// 66% effectiveness of damage scaling on tracer -- dmgid is 1 on tracers
	isSpecial = (flags & DND_WDMG_ISSUPER) && (wepid == DND_WEAPON_BFG32768 || DND_WEAPON_BFG6000) && dmgid == 1;
	if(isSpecial) {
		temp *= 2;
		temp /= 3;
	}
	
	// add flat bonus here
	dmg += temp;
	
	// if there would be an overflow with dmg x temp (temp in [1.0, 65536.0], fixed)
	temp = GetCachedPlayerFactor(pnum, wepid, dmgid);
	if(isSpecial) {
		temp *= 2;
		temp /= 3;
	}
	
	// if we had a factor of 0, dont bother here
	if(!temp)
		return dmg;
	
	if(dmg < INT_MAX / temp) {
		dmg *= temp;
		dmg /= 100;
		// no longer fixed
		//dmg >>= 16;
	}
	else {
		// beyond this point wepid doesnt matter so use that instead
		dmg = INT_MAX;//BigNumberFormula(dmg, temp);
	}
	
	//printbold(d:dmg);
	
	return dmg;
}

// there may be things that add + to cull % later
bool CheckCullRange(int source, int victim, int dmg) {
	return GetActorProperty(victim, APROP_HEALTH) - dmg <= ApplyDamageFactor_Safe(MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp, DND_CULL_BASEPERCENT);
}

void HandleChillEffects(int pnum, int victim) {
	// not ailment immune
	if(CheckAilmentImmunity(pnum, victim - DND_MONSTERTID_BEGIN, DND_FRIGID)) {
		// check health thresholds --- get missing health
		int hpdiff = MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp - GetActorProperty(victim, APROP_HEALTH);
		int stacks = CheckActorInventory(victim, "DnD_ChillStacks");
		int threshold = MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp * GetChillThreshold(pnum, stacks + 1) / 100;

		if(hpdiff >= threshold) {
			// add a new stack of chill and check for freeze
			if(!stacks) {
				GiveActorInventory(victim, "DnD_ChillStacks", 1);
				ACS_NamedExecuteWithResult("DnD Monster Chill", victim, pnum);
			}
			else if(stacks < DND_BASE_CHILL_CAP)
				GiveActorInventory(victim, "DnD_ChillStacks", 1);
			
			// freeze checks --- added freeze chance % increase -- unique boss is immune to freeze
			if(IsUniqueBossMonster(victim - DND_MONSTERTID_BEGIN))
				return;
			
			hpdiff = GetFreezeChance(pnum, CheckActorInventory(victim, "DnD_ChillStacks"));
			if(random(1, 100) <= hpdiff) {
				if(GetActorProperty(victim, APROP_HEALTH) > 0) {
					if(CheckFlag(victim, "BOSS"))
						stacks = DND_BASE_FREEZETIMER / 3;
					else
						stacks = DND_BASE_FREEZETIMER;
					
					// set freeze timer and run script
					if(!CheckActorInventory(victim, "DnD_FreezeTimer")) {
						GiveActorInventory(victim, "DnD_FreezeTimer", stacks);
						ACS_NamedExecuteWithResult("DnD Monster Freeze", victim);
					}
					else
						SetActorInventory(victim, "DnD_FreezeTimer", stacks);
				}
			}
		}
	}
}

void HandleIgniteEffects(int pnum, int victim, int wepid) {
	if
	(
		CheckAilmentImmunity(pnum, victim - DND_MONSTERTID_BEGIN, DND_SCORCHED) &&
		CheckIgniteChance(pnum)
	)
	{
		int amt = DND_BASE_IGNITETIMER * (100 + GetPlayerAttributeValue(pnum, INV_IGNITEDURATION) + GetPlayerAttributeValue(pnum, INV_EX_DOTDURATION)) / 100;
		int current_ign_time = CheckActorInventory(victim, "DnD_IgniteTimer");
		if(!current_ign_time) {
			SetActorInventory(victim, "DnD_IgniteTimer", amt);
			ACS_NamedExecuteWithResult("DnD Monster Ignite", victim, wepid, true);
		}
		else // only replace timer if this is higher
			SetActorInventory(victim, "DnD_IgniteTimer", Max(amt, current_ign_time));
	}
}

void HandleOverloadEffects(int pnum, int victim) {
	if
	(
		CheckAilmentImmunity(pnum, victim - DND_MONSTERTID_BEGIN, DND_INSULATED) &&
		random(1, 100) <= DND_BASE_OVERLOADCHANCE * (100 + GetPlayerAttributeValue(pnum, INV_OVERLOADCHANCE)) / 100 && IsActorAlive(victim)
	)
	{
		if(!CheckActorInventory(victim, "DnD_OverloadTimer")) {
			SetActorInventory(victim, "DnD_OverloadTimer", GetOverloadTime(pnum));
			SetActorInventory(victim, "DnD_OverloadDamage", Max(ConvertFixedFactorToInt(GetPlayerAttributeValue(pnum, INV_OVERLOAD_DMGINCREASE)), CheckActorInventory(victim, "DnD_OverloadDamage")));
			ACS_NamedExecuteWithResult("DnD Monster Overload", victim);
		}
		else
			SetActorInventory(victim, "DnD_OverloadTimer", GetOverloadTime(pnum));
	}
}

int GetResistPenetration(int pnum, int category) {
	switch(category) {
		case DND_DAMAGECATEGORY_BULLET:
		case DND_DAMAGECATEGORY_MELEE:
		return GetPlayerAttributeValue(pnum, INV_PEN_PHYSICAL);
		case DND_DAMAGECATEGORY_ENERGY:
		return GetPlayerAttributeValue(pnum, INV_PEN_ENERGY);
		case DND_DAMAGECATEGORY_EXPLOSIVES:
		return GetPlayerAttributeValue(pnum, INV_PEN_EXPLOSIVE);
		case DND_DAMAGECATEGORY_OCCULT:
		return GetPlayerAttributeValue(pnum, INV_PEN_OCCULT);
		case DND_DAMAGECATEGORY_ELEMENTAL:
		return GetPlayerAttributeValue(pnum, INV_PEN_ELEMENTAL);
		case DND_DAMAGECATEGORY_SOUL:
		return GetPlayerAttributeValue(pnum, INV_PEN_OCCULT) + GetPlayerAttributeValue(pnum, INV_EX_SOULWEPSPEN);
	}
	return 0;
}

Script "DnD Occult Weaken" (int victim) {
	SetActivator(victim);
	
	int time = 0;
	while(time < OCCULT_WEAKEN_DURATION) {
		int prev = CheckInventory("OccultWeaknessStack");
		Delay(const:TICRATE);
		++time;
		if(prev != CheckInventory("OccultWeaknessStack"))
			time = 0;
	}
	
	SetInventory("OccultWeaknessStack", 0);
	
	SetResultValue(0);
}

int ApplyPenetrationToDamage(int pnum, int victim, int dmg, int damage_category, int flags, int resist, int pen) {
	// if occult weakness exists, apply it checking monster's debuff -- to be done as a resist reduction to affect all players later
	if(damage_category == DND_DAMAGECATEGORY_OCCULT || damage_category == DND_DAMAGECATEGORY_SOUL || (flags & DND_DAMAGEFLAG_SOULATTACK)) {
		int occ_weak = GetPlayerAttributeValue(pnum, INV_ESS_ZRAVOG);
		if(occ_weak) {
			if(!CheckActorInventory(victim, "OccultWeaknessStack")) {
				GiveActorInventory(victim, "OccultWeaknessStack", 1);
				ACS_NamedExecuteWithResult("DnD Occult Weaken", victim);
			}
			else
				GiveActorInventory(victim, "OccultWeaknessStack", 1);
			
			// percentage amplification of penetration per stack on victim
			resist = resist * (100 - occ_weak * CheckActorInventory(victim, "OccultWeaknessStack")) / 100;
		}
	}

	// factor is the final resistance the monster will have against the attack
	int factor = (100 - resist + pen);
	if(factor <= 0)
		return 0;
	
	// non-zero, we're good
	return dmg * factor / 100;
}

int FactorResists(int source, int victim, int dmg, int damage_type, int actor_flags, int flags, bool forced_full, int wepid, bool wep_neg = false) {
	// check penetration stuff on source -- set it accordingly to damage type being checked down below
	int mon_id = victim - DND_MONSTERTID_BEGIN;
	int damage_category = GetDamageCategory(damage_type, flags);
	int pnum = PlayerNumber();
	int pen = GetResistPenetration(pnum, damage_category);
	int resist = 0;
	
	// if doomguy perk 50 is there and this is a monster, ignore res
	// added crit ignore res modifier here from below
	forced_full |= 	(CheckInventory("Doomguy_Perk50") && !(flags & DND_DAMAGEFLAG_ISSPELL) && IsMonsterIdDemon(mon_id)) || 
					(!wep_neg && (actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT) && GetPlayerAttributeValue(pnum, INV_EX_CRITIGNORERESCHANCE) >= random(1, 100));
	
	// weaknesses
	if
	(
		(MonsterProperties[mon_id].trait_list[DND_ENERGY_WEAKNESS] && damage_category == DND_DAMAGECATEGORY_ENERGY) ||
		(MonsterProperties[mon_id].trait_list[DND_ELEMENTAL_WEAKNESS] && damage_category == DND_DAMAGECATEGORY_ELEMENTAL) ||
		(MonsterProperties[mon_id].trait_list[DND_MAGIC_WEAKNESS] && (damage_category == DND_DAMAGECATEGORY_OCCULT || damage_category == DND_DAMAGECATEGORY_SOUL))
	)
	{
		resist = -DND_WEAKNESS_FACTOR;
	}
	else if
	(
		(MonsterProperties[mon_id].trait_list[DND_FIRE_WEAKNESS] && damage_type == DND_DAMAGETYPE_FIRE) ||
		(MonsterProperties[mon_id].trait_list[DND_ICE_WEAKNESS] && damage_type == DND_DAMAGETYPE_ICE)
	)
	{
		resist = -DND_SPECIFICELEWEAKNESS_FACTOR;
	}
	
	// special bonuses for certain creature types
	if(MonsterProperties[mon_id].trait_list[DND_ICECREATURE]) {
		// make sure to check ice and fire pen seperate
		if(damage_type == DND_DAMAGETYPE_ICE)
			resist += DND_IMMUNITY_FACTOR * (!forced_full);
		else if(damage_type == DND_DAMAGETYPE_FIRE)
			resist += -DND_SPECIFICELEWEAKNESS_FACTOR;
	}
	else if(MonsterProperties[mon_id].trait_list[DND_FIRECREATURE]) {
		// make sure to check ice and fire pen seperate
		if(damage_type == DND_DAMAGETYPE_FIRE)
			resist += DND_IMMUNITY_FACTOR * (!forced_full);
		else if(damage_type == DND_DAMAGETYPE_ICE)
			resist += -DND_SPECIFICELEWEAKNESS_FACTOR;
	}
	else if(MonsterProperties[mon_id].trait_list[DND_STONECREATURE]) {
		// make sure to check these seperate
		if(damage_type == DND_DAMAGETYPE_FIRE)
			resist += DND_RESIST_FACTOR * (!forced_full);
		else if(damage_type == DND_DAMAGETYPE_ICE)
			resist += -DND_SPECIFICELEWEAKNESS_FACTOR;
		else if(damage_type == DND_DAMAGETYPE_LIGHTNING || damage_type == DND_DAMAGETYPE_POISON)
			resist += DND_IMMUNITY_FACTOR * (!forced_full);
	}
	else if(MonsterProperties[mon_id].trait_list[DND_EARTHCREATURE]) {
		// make sure to check these seperate
		if(damage_type == DND_DAMAGETYPE_LIGHTNING)
			resist += -DND_SPECIFICELEWEAKNESS_FACTOR;
		else if(damage_type == DND_DAMAGETYPE_POISON)
			resist += -DND_IMMUNITY_FACTOR * (!forced_full);
	}
	
	// addition of pen here means if we ignored resists and immunities, we still give penetration a chance to weaken the defences further
	// return early as we ignored resists and immunities
	// if we do forced full damage skip resists and immunities
	if(forced_full) {
		if(pen)
			return ApplyPenetrationToDamage(pnum, victim, dmg, damage_category, flags, resist, pen);
		return dmg;
	}
		
	// resists from here on -- could be nicely tidied up with some array lining up but I dont really want to bother with that right now -- some more careful organization could be better later down the line
	if
	(
		(MonsterProperties[mon_id].trait_list[DND_EXPLOSIVE_RESIST] && damage_type == DND_DAMAGETYPE_EXPLOSIVES) ||
		(MonsterProperties[mon_id].trait_list[DND_BULLET_RESIST] && (damage_category == DND_DAMAGECATEGORY_BULLET || damage_category == DND_DAMAGECATEGORY_MELEE)) ||
		(MonsterProperties[mon_id].trait_list[DND_ENERGY_RESIST] && damage_category == DND_DAMAGECATEGORY_ENERGY) ||
		(MonsterProperties[mon_id].trait_list[DND_MAGIC_RESIST] && (damage_category == DND_DAMAGECATEGORY_OCCULT || damage_category == DND_DAMAGECATEGORY_SOUL)) ||
		(MonsterProperties[mon_id].trait_list[DND_ELEMENTAL_RESIST] && damage_category == DND_DAMAGECATEGORY_ELEMENTAL)
	)
		resist += DND_RESIST_FACTOR;
	// immunities
	else if
	(
		(MonsterProperties[mon_id].trait_list[DND_EXPLOSIVE_NONE] && damage_type == DND_DAMAGETYPE_EXPLOSIVES) ||
		(MonsterProperties[mon_id].trait_list[DND_BULLET_IMMUNE] && (damage_category == DND_DAMAGECATEGORY_BULLET || damage_category == DND_DAMAGECATEGORY_MELEE)) ||
		(MonsterProperties[mon_id].trait_list[DND_ENERGY_IMMUNE] && damage_category == DND_DAMAGECATEGORY_ENERGY) ||
		(MonsterProperties[mon_id].trait_list[DND_MAGIC_IMMUNE] && (damage_category == DND_DAMAGECATEGORY_OCCULT || damage_category == DND_DAMAGECATEGORY_SOUL)) ||
		(MonsterProperties[mon_id].trait_list[DND_ELEMENTAL_IMMUNE] && damage_category == DND_DAMAGECATEGORY_ELEMENTAL)
		
	)
		resist += DND_IMMUNITY_FACTOR;
	else if(MonsterProperties[mon_id].trait_list[DND_ETHEREAL] && damage_category != DND_DAMAGECATEGORY_OCCULT && damage_category != DND_DAMAGECATEGORY_SOUL)
		resist += DND_IMMUNITY_HARDCAP_FACTOR;
	
	// no special factors, process as is
	return ApplyPenetrationToDamage(pnum, victim, dmg, damage_category, flags, resist, pen);
}

// for player hitting others damage
int HandlePlayerBuffs(int p_tid, int enemy_tid, int dmg, int damage_type, int wepid, int flags, bool isIgnite) {
	if(!IsOccultDamage(damage_type) && IsAccessoryEquipped(p_tid, DND_ACCESSORY_DEMONBANE))
		dmg /= DND_DEMONBANE_REDUCE;
		
	// ghost enemies take more damage if nether mask is equipped
	if(CheckFlag(enemy_tid, "GHOST") && IsAccessoryEquipped(p_tid, DND_ACCESSORY_NETHERMASK))
		dmg = ApplyDamageFactor_Safe(dmg, DND_NETHERGHOST_AMP, DND_NETHERGHOST_DIV);
		
	// amps fire damage, reduces ice damage
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_AMULETHELLFIRE)) {
		// we handle ignite damage buff in the dot calculation
		if(IsFireDamage(damage_type) && !isIgnite)
			dmg = ApplyDamageFactor_Safe(dmg, DND_AMULETHELL_DAMAGE_MUL, DND_AMULETHELL_DAMAGE_DIV);
		else if(IsIceDamage(damage_type))
			dmg /= DND_AMULETHELL_DAMAGE_DIV;
	}
	
	if(!(IsMeleeDamage(damage_type) || (flags & DND_DAMAGEFLAG_COUNTSASMELEE)) && IsAccessoryEquipped(p_tid, DND_ACCESSORY_HATESHARD))
		dmg /= DND_HATESHARD_REDUCTION;
	
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_HANDARTEMIS)) {
		if(IsSuperWeapon(wepid))
			dmg /= DND_ARTEMIS_REDUCE_SUPER;
		else
			dmg /= DND_ARTEMIS_REDUCE;
	}
		
	// if there's poison stack that means it's a regular poison attack, but if there's not that means its a poison dot, do not include it again for that
	if(CheckInventory("AgamottoOffense") && !(flags & DND_DAMAGEFLAG_NOPOISONSTACK))
		dmg = ApplyDamageFactor_Safe(dmg, DND_AGAMOTTO_OFFENSE, DND_AGAMOTTO_OFFENSE_FACTOR);
	
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_LICHARM)) {
		if(damage_type == DND_DAMAGETYPE_SOUL || (flags & DND_DAMAGEFLAG_SOULATTACK))
			dmg = ApplyDamageFactor_Safe(dmg, DND_LICHARM_BUFF, DND_LICHARM_BUFF_DIV);
		else
			dmg /= DND_LICHARM_FACTOR;
	}
	
	if(CheckInventory("ElementPower_Fire")) {
		if(IsFireDamage(damage_type))
			dmg *= DND_SIGIL_BUFF;
		else if(IsElementalDamage(damage_type))
			dmg *= DND_SIGIL_NERF;
	}
	else if(CheckInventory("ElementPower_Ice")) {
		if(IsIceDamage(damage_type))
			dmg *= DND_SIGIL_BUFF;
		else if(IsElementalDamage(damage_type))
			dmg /= DND_SIGIL_NERF;
	}
	else if(CheckInventory("ElementPower_Lightning")) {
		if(IsLightningDamage(damage_type))
			dmg *= DND_SIGIL_BUFF;
		else if(IsElementalDamage(damage_type))
			dmg /= DND_SIGIL_NERF;
	}
	else if(CheckInventory("ElementPower_Earth")) {
		if(IsPoisonDamage(damage_type))
			dmg *= DND_SIGIL_BUFF;
		else if(IsElementalDamage(damage_type))
			dmg /= DND_SIGIL_NERF;
	}
	
	// 50% more damage taken
	if(CheckInventory("HateWeakness"))
		dmg = ApplyDamageFactor_Safe(dmg, DND_HATESHARD_FACTOR, DND_HATESHARD_DIV);
	
	return dmg;
}

// for others hitting player damage
int HandlePlayerOnHitBuffs(int p_tid, int enemy_tid, int dmg, int dmg_data, str arg2) {
	// take extra damage only if they aren't ghost
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_NETHERMASK) && !CheckFlag("enemy_tid", "GHOST"))
		dmg = ApplyDamageFactor_Safe(dmg, DND_NETHERMASK_AMP, DND_NETHERMASK_DIV);
		
	// amps ice damage taken, reduces fire damage
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_AMULETHELLFIRE)) {
		if((dmg_data & DND_DAMAGETYPEFLAG_FIRE) || arg2 == "Slime")
			dmg /= DND_AMULETHELL_FACTOR;
		else if(dmg_data & DND_DAMAGETYPEFLAG_ICE)
			dmg = ApplyDamageFactor_Safe(dmg, DND_AMULETHELL_AMP, DND_AMULETHELL_FACTOR);
	}
	
	// agamotto defense
	if(CheckActorInventory(p_tid, "AgamottoDefense"))
		dmg = ApplyDamageFactor_Safe(dmg, DND_AGAMOTTO_DEFENSE, DND_AGAMOTTO_DEFENSE_FACTOR);
		
	if(CheckActorInventory(enemy_tid, "HunterTalismanDebuff"))
		dmg -= dmg / DND_HUNTERTALISMAN_NERF;
		
	// 25%
	if(CheckActorInventory(p_tid, "Invulnerable_Better"))
		dmg /= DND_INVULSPHERE_FACTOR;
	
	return dmg;
}

// This function is responsible for handling all damage effects player has that affect their damage some way
// ex: curses etc.
int HandleGenericPlayerDamageEffects(int pnum, int dmg) {
	// little orbs he drops
	if(CheckInventory("Doomguy_Perk25_Damage"))
		dmg = ApplyDamageFactor_Safe(dmg, DND_DOOMGUY_DMGMULT, DND_DOOMGUY_DMGDIV);

	// 25% reduction, so 3 / 4
	if(CheckInventory("FleshWizardWeaken"))
		dmg = ApplyDamageFactor_Safe(dmg, 3, 4);
	
	// 70% reduction, so 3 / 10
	if(CheckInventory("PowerLessDamage"))
		dmg = ApplyDamageFactor_Safe(dmg, 3, 10);
		
	int temp;
	if(CheckInventory("PlayerIsLeeching") && (temp = GetPlayerAttributeValue(pnum, INV_LIFESTEAL_DAMAGE)))
		dmg = ApplyFixedFactorToInt(dmg, temp);
		
	if(CheckInventory("CorruptOrb_DamageReduction"))
		dmg /= DND_CORRUPTORB_DIV;
	
	temp = CheckInventory("Punisher_Perk50_Counter") / DND_PUNISHER_PERK3_KILLCOUNT;
	if(temp)
		dmg = ApplyDamageFactor_Safe(dmg, 100 + temp * DND_PUNISHER_DMGINC);
		
	temp = CheckInventory("Rally_DamageBuff");
	if(temp)
		dmg = ApplyDamageFactor_Safe(dmg, 100 + RALLY_BASEDAMAGE + (temp - 1) * RALLY_DAMAGEPERLVL);
		
	// dmg is multiplied by 3/2 = 1.5, 50% more dmg
	if(GetArmorID() == BODYARMOR_RAVAGER && CheckInventory("RavagerPower"))
		dmg = ApplyDamageFactor_Safe(dmg, DND_RAVAGER_DMGMUL, DND_RAVAGER_DMGDIV);
		
	// artifact things
	if(CheckInventory("DnD_ArtiDmgPower"))
		dmg = ApplyDamageFactor_Safe(dmg, 100 + DND_QUEST_NOARTIDMG);
		
	if(CheckInventory("TripleDamagePower"))
		dmg *= 3;
	else if(CheckInventory("TripleDamagePower2"))
		dmg = dmg * 9 / 2;
	
	return dmg;
}

void HandleTargetPicking(int montid) {
	// off cd and we dont have a target, since we damaged it we can pick it for now
	int curr_tid = CheckInventory("TargetTID");
	if(!curr_tid) {
		SetInventory("TargetTID", montid);
		
		if(!CheckInventory("TargetPickCooldown"))
			GiveInventory("TargetPickCooldown", 1);
	}
	else if(montid == curr_tid) // attacking same monster, refresh cd
		GiveInventory("TargetPickCooldown", 1);
}

// returns the filtered, reduced etc. damage when factoring in all resists or weaknesses ie. this is the final damage the actor will take
// This is strictly for player doing damage to other monsters or shootables!
// All damage factors here are applied in the "more" method, ie. multiplicative
void HandleDamageDeal(int source, int victim, int dmg, int damage_type, int wepid, int flags, int ox, int oy, int oz, int actor_flags, bool wep_neg = false, bool oneTimeRipperHack = false) {
	str s_damagetype = DamageTypeList[damage_type];
	bool forced_full = false;
	bool no_ignite_stack = flags & DND_DAMAGEFLAG_NOIGNITESTACK;
	int temp;
	
	// if not player sourced, return
	if(!isPlayer(source))
		return;
	
	int pnum = source - P_TIDSTART;
	
	// check if the damage is to be dealt without any reductions from resistances or immunities
	forced_full |= 	(flags & DND_DAMAGEFLAG_DOFULLDAMAGE)																		||
					((flags & DND_DAMAGEFLAG_ISSPELL) && CheckUniquePropertyOnPlayer(pnum, PUP_SPELLSDOFULL))					||
					(IsOccultDamage(damage_type) && IsQuestComplete(0, QUEST_KILLDREAMINGGOD))									||
					(IsEnergyDamage(damage_type) && CheckInventory("Cyborg_Perk50")) 											||
					(IsExplosionDamage(damage_type) && CheckUniquePropertyOnPlayer(pnum, PUP_EXPLOSIVEIGNORERESIST))			||
					(damage_type == DND_DAMAGETYPE_SOUL && CheckUniquePropertyOnPlayer(pnum, PUP_SOULWEPSDOFULL))				||
					((flags & DND_DAMAGEFLAG_ISSHOTGUN) && CheckInventory("Hobo_Perk50"));

	// check blocking/invulnerable status of monster -- if they are and we dont have foilinvul on this, no penetration
	if
	(
		(MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_ISBLOCKING] || CheckFlag(victim, "INVULNERABLE")) && !(actor_flags & DND_ACTORFLAG_FOILINVUL)
	)
	{
		temp = GetPlayerAttributeValue(pnum, INV_ESS_HARKIMONDE);
		// we have 0 chance or we have chance but it didn't roll in our favor
		if(!temp || temp < random(1, 100)) {
			ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_INVULNERABLE);
			return;
		}
	}
	
	int extra = 0;
	int poison_factor = 0;
	
	// extra property checks moved here
	if(!wep_neg) {
		// chance to force pain
		extra = GetPlayerWeaponModVal(pnum, wepid, WEP_MOD_FORCEPAINCHANCE);
		if(extra && extra > random(1, 100))
			actor_flags |= DND_ACTORFLAG_FORCEPAIN;
		
		// poison on hit with % dmg
		extra = GetPlayerWeaponModVal(pnum, wepid, WEP_MOD_POISONFORPERCENTDAMAGE);
		poison_factor = extra + (!!(flags & DND_DAMAGEFLAG_INFLICTPOISON)) * DND_BASE_POISON_FACTOR;
		//flags |= (!!poison_factor) * DND_DAMAGEFLAG_INFLICTPOISON;
		
		// percent damage of monster if it exists
		extra = GetPlayerWeaponModVal(pnum, wepid, WEP_MOD_PERCENTDAMAGE);
		dmg += (MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp * extra) / 100;
		flags |= (!!extra) * DND_DAMAGEFLAG_PERCENTHEALTH;
	}
	else if(flags & DND_DAMAGEFLAG_ISSPELL) {
		// check if it has any poison factor on the spell
		poison_factor = GetSpellPoisonFactor(wepid);
	}
	
	// pain checks
	if(actor_flags & DND_ACTORFLAG_PAINLESS)
		s_damagetype = StrParam(s:s_damagetype, s:"_NoPain");
	else if(actor_flags & DND_ACTORFLAG_FORCEPAIN)
		s_damagetype = StrParam(s:s_damagetype, s:"_ForcePain");
	
	// desolator damage increase
	if(damage_type == DND_DAMAGETYPE_DESOLATOR) {
		if(!CheckActorInventory(victim, "DesolatorStackCounter")) {
			GiveActorInventory(victim, "DesolatorStackTimer", 52);
			ACS_NamedExecuteAlways("DND Desolator Debuff FX", 0, victim);
		}
		else
			GiveActorInventory(victim, "DesolatorStackTimer", 17);
		GiveActorInventory(victim, "DesolatorStackCounter", 1);
	}
	
	// increase damage they take from elemental attacks for each stack
	// poison damage gets sent already scaled, dont scale twice
	if(IsElementalDamage(damage_type) && !(flags & DND_DAMAGEFLAG_NOPOISONSTACK)) {
		temp = CheckActorInventory(victim, "DesolatorStackCounter");
		// 10% increase from desolator
		if(temp)
			dmg = ApplyDamageFactor_Safe(dmg, 100 + temp * DND_DESOLATOR_DMG_GAIN);
	}
	
	// handle resists and all that here
	//printbold(s:"res calc");
	temp = dmg;
	dmg = FactorResists(source, victim, dmg, damage_type, actor_flags, flags, forced_full, wepid, wep_neg);
	// if more that means we hit a weakness, otherwise below conditions check immune and resist respectively
	if(dmg > temp)
		ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_WEAKNESS);
	else if(dmg < temp / 4)
		ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_IMMUNITY);
	else if(dmg < temp)
		ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_RESIST);
	
	// handle poison checks
	// printbold(d:damage_type, s: " ", d:IsPoisonDamage(damage_type), s: " ", d:!(flags & DND_DAMAGEFLAG_NOPOISONSTACK), s: " ", d:flags);
	if((IsPoisonDamage(damage_type) || (flags & DND_DAMAGEFLAG_INFLICTPOISON)) && !(flags & DND_DAMAGEFLAG_NOPOISONSTACK) && CheckAilmentImmunity(pnum, victim - DND_MONSTERTID_BEGIN, DND_VENOMANCER)) {
		// poison damage deals 10% of its damage per stack over 3 seconds
		if(CheckActorInventory(victim, "DnD_PoisonStacks") < DND_BASE_POISON_STACKS && dmg > 0) {
			GiveActorInventory(victim, "DnD_PoisonStacks", 1);
			// 2% of damage or by the factor -- if factor is with a weapon that already has inflictpoison, it empowers poison of the weapon by +2%
			poison_factor = Max(DND_BASE_POISON_FACTOR, poison_factor);
			extra = Max((dmg * poison_factor) / 100, 1);
			ACS_NamedExecuteWithResult("DnD Do Poison Damage", victim, extra, wepid);
			//printbold(s:"poison received by ", d:victim);
		}
	}
	
	if((flags & DND_DAMAGEFLAG_EXTRATOUNDEAD) && (MonsterData[MonsterProperties[victim - DND_MONSTERTID_BEGIN].id].flags & (DND_MTYPE_UNDEAD_POW | DND_MTYPE_MAGICAL_POW))) {
		dmg *= DND_EXTRAUNDEADDMG_MULTIPLIER;
		
		if(IsQuestComplete(0, QUEST_SPAREZOMBIES))
			dmg += dmg * DND_QUEST_UNDEADGAIN / 100;
	}
	
	// check blockers take more dmg modifier
	if(MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_ISBLOCKING]) {
		temp = GetPlayerAttributeValue(pnum, INV_BLOCKERS_MOREDMG);
		dmg = ApplyFixedFactorToInt(dmg, temp);
	}
	
	// buff effectiveness is the maximum of what the monster might have had previously from another player vs. most up-to-date, which is overwritten into its DnD_OverloadDamage item
	if(CheckActorInventory(victim, "DnD_OverloadTimer")) {
		if(damage_type != DND_DAMAGETYPE_LIGHTNING)
			dmg = dmg * (100 + DND_BASE_OVERLOADBUFF + CheckActorInventory(victim, "DnD_OverloadDamage")) / 100;
		//GiveActorInventory(victim, "DnD_OverloadDamage", dmg);
	}
	
	// additional damage vs frozen enemies modifier
	temp = GetPlayerAttributeValue(pnum, INV_ESS_ERYXIA);
	if(CheckActorInventory(victim, "DnD_FreezeTimer") && temp)
		dmg = ApplyFixedFactorToInt(dmg, temp);
		
	// 50% more damage taken, so dmg * 3 / 2
	if(CheckActorInventory(victim, "DemonSealResistDebuff"))
		dmg = ApplyDamageFactor_Safe(dmg, 3, 2);
		
	// CURSE EFFECTS
	dmg = HandleGenericPlayerDamageEffects(pnum, dmg);
	
	// ACCESSORY EFFECTS
	dmg = HandlePlayerBuffs(source, victim, dmg, damage_type, wepid, flags, no_ignite_stack);
	
	// finally the crit check if applicable
	if(actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT) {
		//printbold(s:"confirmed crit with factor application: ", d:dmg, s: " => ", d: ConfirmedCritFactor(dmg, wepid));
		// DOT doesn't crit
		if((actor_flags & DND_ACTORFLAG_ISDAMAGEOVERTIME) || (flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME))
			actor_flags ^= DND_ACTORFLAG_CONFIRMEDCRIT;
		else
			dmg = ConfirmedCritFactor(dmg, wepid);
	}
	
	// damage number handling - NO MORE DAMAGE FIDDLING FROM BELOW HERE
	// all damage calculations should be done by this point, besides cull --- cull should not reflect on here
	// printbold(s:"apply ", d:dmg, s: " of type ", s:s_damagetype, s: " pnum: ", d:pnum);
	// this part handles damage pushing
	temp = victim - DND_MONSTERTID_BEGIN;
	
	// extra represents the flag list of damageticflag
	extra = (!!(actor_flags & DND_ACTORFLAG_NOPUSH) * DND_DAMAGETICFLAG_PUSH) 					|
			(!!(actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT) * DND_DAMAGETICFLAG_CRIT)			|
			(!!(actor_flags & DND_ACTORFLAG_COUNTSASMELEE) * DND_DAMAGETICFLAG_CONSIDERMELEE)	|
			(!!((actor_flags & DND_ACTORFLAG_ISDAMAGEOVERTIME) || (flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME)) * DND_DAMAGETICFLAG_DOT);
	
	// we send particular damage types in that can cause certain status effects like chill, freeze etc.
	if(damage_type == DND_DAMAGETYPE_ICE)
		extra |= DND_DAMAGETICFLAG_ICE;
	else if(IsFireDamage(damage_type) && !no_ignite_stack)
		extra |= DND_DAMAGETICFLAG_FIRE;
	else if(damage_type == DND_DAMAGETYPE_LIGHTNING)
		extra |= DND_DAMAGETICFLAG_LIGHTNING;

	if(!PlayerDamageTicData[pnum][temp]) {
		PlayerDamageVector[pnum].x = ox;
		PlayerDamageVector[pnum].y = oy;
		PlayerDamageVector[pnum].z = oz;
		ACS_NamedExecuteWithResult("DnD Damage Accumulate", temp | (extra << DND_DAMAGE_ACCUM_SHIFT), wepid, wep_neg | (oneTimeRipperHack << 1));
	}
	PlayerDamageTicData[pnum][temp] += dmg;
	
	if((temp = CheckActorInventory(victim, "MonsterFortifyCount")) && !(actor_flags & DND_ACTORFLAG_FOILINVUL)) {
		if(temp <= dmg) {
			temp = dmg;
			PlaySound(victim, "Elite/FortifyCrack", CHAN_VOICE, 1.0);
			// remove fortify modifier from monster
			ACS_NamedExecuteWithResult("DnD Monster Trait Take Single", victim, DND_FORTIFIED);
		}
		SetActorInventory(victim, "MonsterFortifyCount", temp - dmg);
		dmg -= temp;
	}
	
	HandleTargetPicking(victim);
	
	// cull checks
	if((flags & DND_DAMAGEFLAG_CULL) && CheckCullRange(source, victim, dmg)) {
		// if self cull is in effect simply destroy it otherwise return from here
		GiveActorInventory(victim, "MonsterKilledByPlayer", 1);
		Thing_Damage2(victim, GetActorProperty(victim, APROP_HEALTH) * 2, s_damagetype);
		return;
	}
	
	if(dmg > 0) {
		// give this token early to prevent order of events getting mixed up
		if(GetActorProperty(victim, APROP_HEALTH) <= dmg)
			GiveActorInventory(victim, "MonsterKilledByPlayer", 1);
		Thing_Damage2(victim, dmg, s_damagetype);
		IncrementStatistic(DND_STATISTIC_DAMAGEDEALT, dmg, source);
	}
	
	// monster or w.e we shot at died
	if(!isActorAlive(victim)) {
		// give this for non-magic seal weapons (seals their souls...)
		if(damage_type != DND_DAMAGETYPE_MAGICSEAL && IsOccultDamage(damage_type))
			GiveActorInventory(victim, "MagicCausedDeath", 1);
	
		if(CheckActorInventory(source, "Berserker_Perk50") && (IsMeleeDamage(damage_type) || flags & DND_DAMAGETICFLAG_CONSIDERMELEE)) {
			SetActorInventory(source, "Berserker_HitTimer", DND_BERSERKER_PERK50_TIMER);
			if((temp = CheckActorInventory(source, "Berserker_HitTracker")) < DND_BERSERKER_PERK50_MAXSTACKS) {
				GiveActorInventory(source, "Berserker_HitTracker", 1);
				if(!temp)
					ACS_NamedExecuteAlways("DnD Berserker Perk50 Timer", 0, source);
			}
			if(temp + 1 >= DND_BERSERKER_PERK50_MAXSTACKS) {
				if(!CheckActorInventory(source, "Berserker_NoRoar"))
					HandleBerserkerRoar(source);
				GiveActorInventory(source, "Berserker_Perk50_Speed", 1);
			}
		}
	}
}

int ScaleExplosionToDistance(int mon_id, int dmg, int radius, int fullradius, int ox, int oy, int oz, int proj_r) {
	// calculate damage falloff based on distance -- subtract projectile's radius from distance to get a better estimate
	// idea here: monster hitboxes are actual rectangles and not circles, so unless hit occured perpendicular to the hitbox, you won't deal max damage
	// to fix that, subtract (r + r * sqrt2) / 2, which is 1.207. Reason: radius scales from r to r * sqrt2 over the square's center to diagonal.
	int res = dmg;
	int dist = fdistance_delta(ox - GetActorX(mon_id), oy - GetActorY(mon_id), oz - GetActorZ(mon_id));
	dist -= FixedMul(GetActorProperty(mon_id, APROP_RADIUS) + proj_r, 1.207);
	
	// printbold(s:"check dist: ", f:dist, s: " ", f:radius, s: " ", f:fullradius);
	
	// not within range, skip
	if(dist >= radius)
		return -1;
		
	// if full radius is more or equal, then we dont need to consider distance as a factor for scaling damage further
	if(fullradius < radius) {
		if(dist < 0)
			dist = 0;
		// we will reduce damage if we are past fullradius and within radius
		if(dist > fullradius && dist <= radius) {
			// printbold(s:"dist ", f:dist, s:" factor ", f:FixedDiv(radius - dist, radius - fullradius));
			res = res * ((radius - dist) >> 16) / ((radius - fullradius) >> 16);
		}
	}
	return res;
}

void DoExplosionDamage(int owner, int dmg, int radius, int fullradius, int damage_type, int wepid, int flags) {
	int pnum = owner - P_TIDSTART;
	int instance = PlayerExplosionList[pnum].curr_instance;
	int lim = PlayerExplosionList[pnum].list[instance].amt;
		
	int actor_flags = ScanActorFlags();
	bool isArmorPiercing = CheckFlag(0, "PIERCEARMOR");
	
	int px = GetActorX(0), py = GetActorY(0), pz = GetActorZ(0);
	// printbold(s:"Explosion owner: ", d:owner);
	
	int proj_r = GetActorProperty(0, APROP_RADIUS);
	int final_dmg;
	
	bool wep_neg = wepid < 0 || (flags & (DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISSPECIALAMMO));
	
	// confirm the crit damage -- only should work for now if this isn't a spell!
	actor_flags |= (!(flags & DND_DAMAGEFLAG_ISSPELL) && GetActorProperty(0, APROP_ACCURACY) == DND_CRIT_TOKEN) * DND_ACTORFLAG_CONFIRMEDCRIT;
	
	// turn them to fixed
	radius <<= 16;
	fullradius <<= 16;
	
	// moved player self damage check here because we need this current projectile for sight checks
	if(flags & DND_DAMAGEFLAG_BLASTSELF) {
		// we are the owner here at this point, we can use 0 for ourselves
		// sedrin staff armor check
		// if not sedrin staff, immediately check
		// if sedrin staff and if we have armor, both are false so no damage to us
		if(wepid != DND_WEAPON_SEDRINSTAFF || ActorHasNoArmor(owner)) {
			// if this flag is in place, do half damage within half radius
			if(flags & DND_DAMAGEFLAG_HALFDMGSELF)
				final_dmg = ScaleExplosionToDistance(owner, dmg / 2, radius / 2, fullradius / 2, px, py, pz, proj_r);
			else
				final_dmg = ScaleExplosionToDistance(owner, dmg, radius, fullradius, px, py, pz, proj_r);
				
			// crit check done explicitly here for player, player doesn't use HandleDamageDeal
			final_dmg = ConfirmedCritFactor(final_dmg, wepid);
				
			// added sight check to fix explosives hurting behind walls bug
			if(final_dmg > 0 && CheckSight(0, owner, CSF_NOBLOCKALL)) {
				// set activator to us for damage credit -- we no longer need projectile itself here
				SetActivator(owner);
			
				// push with some greater force
				HandleDamagePush(final_dmg * 4, px, py, pz, 0);
				
				// handle player's self explosion resists here
				final_dmg = HandlePlayerSelfDamage(pnum, final_dmg, damage_type, wepid, flags, isArmorPiercing);
				Thing_Damage2(0, final_dmg, DamageTypeList[damage_type]);
			}
		}
	}

	// set activator to player for dmg registry
	SetActivator(owner);
	
	for(int i = 0; i < lim; ++i) {
		int mon_id = PlayerExplosionList[pnum].list[instance].monsters[i];

		// first check if this monster is immune to splash damage (its an easy flag check and eases calculation later)
		if(!CheckFlag(mon_id, "SHOOTABLE"))
			continue;

		// if enemy has NORADIUSDMG and we don't have resist ignore on explosives and we don't have forceradiusdmg on attack itself
		if
		(
			(CheckFlag(mon_id, "NORADIUSDMG") && !CheckUniquePropertyOnPlayer(pnum, PUP_EXPLOSIVEIGNORERESIST) && !(actor_flags & DND_ACTORFLAG_FORCERADIUSDMG)) ||
			(CheckFlag(mon_id, "GHOST") && (actor_flags & DND_ACTORFLAG_THRUGHOST))	||
			(wepid == DND_WEAPON_SEDRINSTAFF && IsActorFullRobotic(mon_id))
		)
			continue;

		final_dmg = ScaleExplosionToDistance(mon_id, dmg, radius, fullradius, px, py, pz, proj_r);
		
		if(final_dmg == -1)
			continue;
		
		// dont deal 0 dmg
		if(!final_dmg)
			final_dmg = 1;
		
		HandleDamageDeal(owner, mon_id, final_dmg, damage_type, wepid, flags, px, py, pz, actor_flags, wep_neg);
		HandleOnHitEffects(owner);
		
		//printbold(s:"Dealing ", d: final_dmg, s: " damage to ", d:mon_id, s: " of type ", s:DamageTypeList[damage_type]);
	}
	
	// damage is dealt we are done with this instance, free it up
	Free_Explosion_Instance(pnum, instance);
}

Script "DnD Do Explosion Damage" (int dmg, int radius, int fullradius, int damage_type) {
	int flags = GetUserVariable(0, DND_EXPLOSION_FLAGVARIABLE);

	// player information
	int owner = GetActorProperty(0, APROP_TARGETTID);
	if(!isPlayer(owner))
		owner = GetActorProperty(0, APROP_SCORE);
	
	// we embed weapon id into damage_type << 16
	DoExplosionDamage(owner, dmg, radius, fullradius, damage_type & 0xFFFF, damage_type >> 16, flags);
	
	SetResultValue(0);
}

Script "DnD Do Explosion Damage (Pets)" (int dmg, int radius, int fullradius, int damage_type) {
	int flags = GetUserVariable(0, DND_EXPLOSION_FLAGVARIABLE);

	// player information
	int owner = CheckInventory("DnD_ProjPnum") + P_TIDSTART;
	
	DoExplosionDamage(owner, dmg, radius, fullradius, damage_type & 0xFFFF, damage_type >> 16, flags);
	
	SetResultValue(0);
}

Script "DnD Crossbow Explosion" (int this, int target) {
	int exptid = DND_CROSSBOW_EXPLOSIONTID + target - P_TIDSTART;
	SpawnForced("Crossbow_Explosion", GetActorX(this), GetActorY(this), GetActorZ(this) + GetActorProperty(this, APROP_HEIGHT) / 2 + 24.0, exptid);
	SetActivator(exptid);
	SetPointer(AAPTR_TARGET, target);
	Thing_ChangeTID(exptid, 0);
	
	SetResultValue(0);
}

void HandleImpactDamage(int owner, int victim, int dmg, int damage_type, int flags, int wepid, bool oneTimeRipperHack = false, bool wasCrit = false) {
	int px, py, pz;
	bool victim_IsMonster = IsMonster(victim);

	if(flags & DND_DAMAGEFLAG_DISTANCEGIVESDAMAGE)
		dmg += GetUserVariable(0, DND_DISTANCEDAMAGE_VARIABLE);
		
	// spawn special blood particles attached to monsters
	if((flags & DND_DAMAGEFLAG_ISHITSCAN) && CheckFlag(victim, "NOBLOOD") && victim_IsMonster && MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_STONECREATURE])
		ACS_NamedExecuteWithResult("DnD Spawn Bloodtype", DND_SPECIALBLOOD_STONE);
		
	int actor_flags = ScanActorFlags();
		
	if(flags & DND_DAMAGEFLAG_FOILINVUL) {
		actor_flags |= DND_ACTORFLAG_FOILINVUL;
		flags ^= DND_DAMAGEFLAG_FOILINVUL;
	}
	
	if(flags & DND_DAMAGEFLAG_COUNTSASMELEE) {
		actor_flags |= DND_ACTORFLAG_COUNTSASMELEE;
		flags ^= DND_DAMAGEFLAG_COUNTSASMELEE;
	}

	if(flags & DND_DAMAGEFLAG_ISHITSCAN) {
		// prevents melee hits from acting like railguns!
		if(!isActorAlive(victim) && !IsMeleeDamage(damage_type)) {
			// if actor died before the rest of the pellets can take effect, fire corresponding bullet attacks from behind this monster
			// calculate a pitch and angle to fire it from this guy
			// get vector from player to puff
			int ang, pitch;
			
			px = GetActorX(0) - GetActorX(owner);
			py = GetActorY(0) - GetActorY(owner);
			pz = GetActorZ(0) - GetActorZ(owner) - DND_PLAYER_HITSCAN_Z;
			ang = VectorAngle(px, py);
			flags = AproxDistance(px, py);
			pitch = -VectorAngle(flags >> 8, pz >> 8);
			flags = DND_SHOTGUNPUFF_REMOVETID + owner - P_TIDSTART; // tid of shell puffs to remove

			// by now we have ang and pitch ready, store player's previous positions, angle and pitch and move player there to fire
			str puff = GetActorClass(0);
			Thing_ChangeTID(0, flags);
			
			// player is now in charge of firing the puff
			SetActivator(owner);
			
			px = GetActorX(0);
			py = GetActorY(0);
			pz = GetActorZ(0);
			
			// move past this monster along this angle
			SetActorPosition(0, px + cos(ang) * (GetActorProperty(victim, APROP_RADIUS) >> 16), py + sin(ang) * (GetActorProperty(victim, APROP_RADIUS) >> 16), pz + pitch * (GetActorProperty(victim, APROP_RADIUS) >> 16), 0);
			
			LineAttack(0, ang, pitch, 0, puff, DamageTypeList[damage_type], 2048.0, FHF_NORANDOMPUFFZ);
			
			SetActorPosition(0, px, py, pz, 0);
			
			// return to puff to early cancel, no need to do damage calculation for this particular pellet anymore
			SetActivator(flags);
			Thing_Remove(flags);
			SetResultValue(0);
			return;
		}
		else {
			px = GetActorX(owner);
			py = GetActorY(owner);
			pz = GetActorZ(owner) + GetActorProperty(owner, APROP_HEIGHT) / 2 + 8.0;
		}
	}
	else {
		if(!oneTimeRipperHack) {
			px = GetActorX(0);
			py = GetActorY(0);
			pz = GetActorZ(0);
		}
		else {
			px = GetActorVelX(0);
			py = GetActorVelY(0);
			pz = GetActorVelZ(0);
		}
	}
	
	// true if wepid is negative (misc damage sources like armors etc.) or if we used a spell
	bool wep_neg = wepid < 0 || (flags & (DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISSPECIALAMMO));
	
	if(!wep_neg) {
		// printbold(d:owner, s: " ", d:victim);
		if(wasCrit || GetActorProperty(0, APROP_ACCURACY) == DND_CRIT_TOKEN) {
			actor_flags |= DND_ACTORFLAG_CONFIRMEDCRIT;
			// printbold(s:"actor got crit confirm");
		}

		// berserker perk50 dmg increase portion and other melee increases
		if((IsMeleeWeapon(wepid) || (actor_flags & DND_ACTORFLAG_COUNTSASMELEE))) {
			if(CheckActorInventory(owner, "Berserker_Perk50")) {
				SetActorInventory(owner, "Berserker_HitTimer", DND_BERSERKER_PERK50_TIMER);
				if((px = CheckActorInventory(owner, "Berserker_HitTracker")) < DND_BERSERKER_PERK50_MAXSTACKS) {
					GiveActorInventory(owner, "Berserker_HitTracker", 1);
					if(!px)
						ACS_NamedExecuteAlways("DnD Berserker Perk50 Timer", 0, owner);
				}
				if(px + 1 >= DND_BERSERKER_PERK50_MAXSTACKS) {
					if(!CheckActorInventory(owner, "Berserker_NoRoar"))
						HandleBerserkerRoar(owner);
					GiveActorInventory(owner, "Berserker_Perk50_Speed", 1);
				}
				dmg = dmg * (100 + (px + 1) * DND_BERSERKER_PERK50_DMGINCREASE) / 100;
			}
			
			dmg = dmg * (100 + GetActorAttributeValue(owner, INV_MELEEDAMAGE)) / 100;
		}
		
		// Flayer magic or undead check
		if(wepid == DND_WEAPON_CROSSBOW && victim_IsMonster && IsActorMagicOrUndead(victim))
			ACS_NamedExecuteWithResult("DnD Crossbow Explosion", victim, owner);
	}
	
	SetActivator(owner);
	
	//printbold(d:dmg, s: " to ", d:victim);
	if(owner && victim) {
		if(victim_IsMonster)
			HandleDamageDeal(owner, victim, dmg, damage_type, wepid, flags, px, py, pz, actor_flags, wep_neg, oneTimeRipperHack);
		else // just deal damage to the shootable
			Thing_Damage2(victim, dmg, "Normal");
	}
}

// we check with if statement here now just in case we add more flags in the future, the check below is to ensure we bypass script execution
Script "DnD Adjust Impact Damage" (int flags, int dmg, int owner) {
	if(flags & DND_DAMAGEFLAG_LOSEDAMAGEPERHIT) {
		int dummy_tid = TEMPORARY_DATADUMMY_TID + owner - P_TIDSTART;
		if(!CheckActorInventory(owner, "DnD_DummySpawned")) {
			//printbold(s:"make dummy");
			GiveActorInventory(owner, "DnD_DummySpawned", 1);
			SpawnForced("DnD_DataDummy", 0, 0, 0, dummy_tid);
			SetActivator(dummy_tid);
			SetPointer(AAPTR_TARGET, owner);
		}
		
		int hitcount = GetUserVariable(dummy_tid, "user_hitcount");
		if(hitcount != DND_MAX_DAMAGELOSEHITS) {
			dmg = dmg * (100 - DND_DAMAGELOST_PERCENT * hitcount) / 100;
			if(dmg < 1)
				dmg = 1;
			
			++hitcount;
			SetUserVariable(dummy_tid, "user_hitcount", hitcount);
			//printbold(s:"hitc ", d:hitcount);
		}
	}
	SetResultValue(dmg);
}

Script "DnD Do Impact Damage" (int dmg, int damage_type, int flags, int wepid) {
	int owner = GetActorProperty(0, APROP_TARGETTID);
	int victim = GetActorProperty(0, APROP_TRACERTID);
	
	// add 1 flip sign, damage functions require wepid to be non-negative, if they are we will know they need to use spell index
	if(flags & DND_DAMAGEFLAG_ISSPELL)
		wepid = -(wepid + 1);
	
	// sedrin check
	if(wepid == DND_WEAPON_SEDRINSTAFF && IsActorFullRobotic(victim)) {
		SetActivator(owner);
		ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_IMMUNITY);
	}
	else {
		if(flags & DND_DAMAGEFLAG_LOSEDAMAGEPERHIT)
			dmg = ACS_NamedExecuteWithResult("DnD Adjust Impact Damage", flags, dmg, owner);
	
		// do the previous damage here because this'll be the initial hit, subsequent hits will follow the new damage
		HandleImpactDamage(owner, victim, dmg, damage_type, flags, wepid);
	}
		
	HandleOnHitEffects(owner);
	
	SetResultValue(0);
}

// has embedded data -- this needs to be a script because the script this gets called in can't have the activator pointer manipulated
Script "DnD Do Impact Damage Ripper" (int dmg, int damage_type, int flags, int wepid) {
	//printbold(s:"FUCKING HURT ", d:damage_type >> DAMAGE_TYPE_SHIFT);
	int owner = GetActorProperty(0, APROP_TARGETTID);
	HandleImpactDamage(owner, damage_type >> DAMAGE_TYPE_SHIFT, dmg, damage_type & DAMAGE_TYPE_MASK, flags, wepid, true);
	HandleOnHitEffects(owner);
	
	SetResultValue(0);
}

void HandleRipperHitSound(int tid, int owner, int wepid) {
	switch(wepid) {
		case DND_WEAPON_CROSSBOW:
			PlaySound(tid, "Crossbow/Hit", 5, 1.0);
		break;
		case DND_WEAPON_DARKLANCE:
			// stack building on kill
			GiveActorInventory(owner, "LanceStacks", 1);
		break;
	}
}

// to be used sparingly, it scans all monsters at all times since we dont have linetraces
Script "DnD One Time Ripper" (int dmg, int damage_type, int flags, int wepid) {
	int owner = GetActorProperty(0, APROP_TARGETTID);
	
	GiveInventory("DnD_RippingBegan", 1);
	
	int r;
	int h;
	if(flags & DND_DAMAGEFLAG_SIMULATERIPPER) {
		r = GetUserVariable(0, "user_r");
		h = GetUserVariable(0, "user_h") << 16;
	}
	else {
		r = GetActorProperty(0, APROP_RADIUS) >> 16;
		h = GetActorProperty(0, APROP_HEIGHT);
	}
	
	int i = 0, m = 0, s = 0, mn;
	int actor_flags = ScanActorFlags();
	
	// increment id by 1 for each call, doesnt matter if it overflows
	int ripper_id = -1;
	static int ripper_count = -1;
	static int ripper_hits[MAX_RIPPERS_ACTIVE][MAX_RIPPER_HITS_STORED];
	ripper_count = (ripper_count + 1) % MAX_RIPPERS_ACTIVE;
	ripper_id = ripper_count;
	
	// reset ripper hit array
	for(i = 0; i < MAX_RIPPER_HITS_STORED; ++i)
		ripper_hits[ripper_id][i] = -1;
	
	// top left, bot right -- no need to be precise with rotation of bounding box here, the engine itself uses AABB anyway
	int top_x, top_y, bot_x, bot_y;
	
	// projectiles spawn speed units ahead of player, this is especially noticable in faster projectiles
	// we must check backwards initially for point blank case
	// find monsters in a rectangle from actor xyz, +-r * cos / sin and +-h on z
	// simple rectanglular box check from rectangle sides
	bool found = false;
	bool first_tic = true;
	int a_x, a_y, a_r;
	
	top_x = GetActorX(0) - (r << 16), top_y = GetActorY(0) + (r << 16), bot_x = GetActorX(0) + (r << 16), bot_y = GetActorY(0) - (r << 16);
	// start target picking
	while(GetActorVelX(0) || GetActorVelY(0) || GetActorVelZ(0)) {
		// get projectile dir, and imagine as if the projectile is stepping forwards from its location
		// this is so faster projectiles are predicted -- we get dir, calculate just how many steps it'd take by speed / radius, then iterate the box over
		int dir_x = GetActorVelX(0);
		int dir_y = GetActorVelY(0);
		int dir_z = GetActorVelZ(0);
		int len = VectorLength3d(dir_x, dir_y, dir_z);
		
		// get most up-to-date speed (it could be slowing down via decorate)
		int speed = len;
		int steps = 0;
		
		// we need len as int here
		len >>= 16;
		if(!len)
			len = 1;
		
		dir_x /= len;		dir_x *= r;
		dir_y /= len;		dir_y *= r;
		dir_z /= len;		dir_z *= r;
		
		for(mn = 0; mn < DnD_TID_Counter[DND_TID_MONSTER]; ++mn) {
			i = UsedMonsterTIDs[mn];
		
			// dead, skip
			if(!isActorAlive(i) || !IsMonster(i))
				continue;
			
			found = false;
			a_x = GetActorX(i), a_y = GetActorY(i), a_r = GetActorProperty(i, APROP_RADIUS);
			steps = 3 * Max(speed, a_r + (r << 16)) / 2;
			
			//if(GetActorClass(i) == "BaronOfHell2")
			//	printbold(f:AproxDistance(GetActorX(0) - a_x, GetActorY(0) - a_y), s: " > ", f:steps, s: " spd: ", f:speed);
			
			if(AproxDistance(GetActorX(0) - a_x, GetActorY(0) - a_y) > steps || ((actor_flags & DND_ACTORFLAG_THRUGHOST) && CheckFlag(i, "GHOST")))
				continue;
			
			// i noticed in some cases where the projectile did not travel all the way to it's speed units distance, it fails to detect enemies
			// so starting from -steps / 3 "kind of" solves it... there will probably still be some weird cases where the hit won't register, but this will never be perfect anyway
			steps = (speed / r) >> 16;
			if(steps < 3)
				steps = 3;
			for(s = -steps / 3; s < steps; ++s) {
				if(CheckProjectileCollision(h, top_x, top_y, bot_x, bot_y, s * dir_x, s * dir_y, s * dir_z, a_x, a_y, a_r, i))
					continue;
				
				//printbold(s:"IN BOX ", d:ripper_id, s: " actor: ", d:i);
				// insert into ripper hit list, and call impact damage script on this guy IF not in list
				for(m = 0; m < MAX_RIPPER_HITS_STORED && ripper_hits[ripper_id][m] != -1; ++m) {
					if(ripper_hits[ripper_id][m] == i) {
						found = true;
						break;
					}
				}
				//printbold(s:"found or max? ", d:found, s: " ", d:m < MAX_RIPPER_HITS_STORED);
				// not in this list yet, insert it and do damage deal routine
				if(!found && m < MAX_RIPPER_HITS_STORED) {
					//printbold(s:"deal damage to ", d:i, s: " by ripper id ", d:ripper_id);
					ripper_hits[ripper_id][m] = i;
					ACS_NamedExecuteWithResult("DnD Do Impact Damage Ripper", dmg, damage_type | (i << DAMAGE_TYPE_SHIFT), flags, wepid);
					HandleRipperHitSound(i, owner, wepid);
				}
				break;
			}
		}
		
		// check for shootables now -- same loop as above but for shootables
		for(mn = 0; mn < DnD_TID_Counter[DND_TID_SHOOTABLE]; ++mn) {
			i = mn + DND_SHOOTABLETID_BEGIN;
			
			// dead, skip -- no monster check here
			if(!isActorAlive(i))
				continue;
			
			found = false;
			a_x = GetActorX(i), a_y = GetActorY(i), a_r = GetActorProperty(i, APROP_RADIUS);
			steps = 3 * Max(speed, a_r + (r << 16)) / 2;
			
			//if(GetActorClass(i) == "BaronOfHell2")
			//	printbold(f:AproxDistance(GetActorX(0) - a_x, GetActorY(0) - a_y), s: " > ", f:steps, s: " spd: ", f:speed);
			
			// shootable decorative actors are most likely not ghosts...
			if(AproxDistance(GetActorX(0) - a_x, GetActorY(0) - a_y) > steps)
				continue;
			
			// i noticed in some cases where the projectile did not travel all the way to it's speed units distance, it fails to detect enemies
			// so starting from -steps / 3 "kind of" solves it... there will probably still be some weird cases where the hit won't register, but this will never be perfect anyway
			steps = (speed / r) >> 16;
			if(steps < 3)
				steps = 3;
			for(s = -steps / 4; s < steps; ++s) {
				if(CheckProjectileCollision(h, top_x, top_y, bot_x, bot_y, s * dir_x, s * dir_y, s * dir_z, a_x, a_y, a_r, i))
					continue;
				
				//printbold(s:"IN BOX ", d:ripper_id, s: " actor: ", d:i);
				// insert into ripper hit list, and call impact damage script on this guy IF not in list
				for(m = 0; m < MAX_RIPPER_HITS_STORED && ripper_hits[ripper_id][m] != -1; ++m) {
					if(ripper_hits[ripper_id][m] == i) {
						found = true;
						break;
					}
				}
				//printbold(s:"found or max? ", d:found, s: " ", d:m < MAX_RIPPER_HITS_STORED);
				// not in this list yet, insert it and do damage deal routine
				if(!found && m < MAX_RIPPER_HITS_STORED) {
					//printbold(s:"deal damage to ", d:i, s: " by ripper id ", d:ripper_id);
					ripper_hits[ripper_id][m] = i;
					ACS_NamedExecuteWithResult("DnD Do Impact Damage Ripper", dmg, damage_type | (i << DAMAGE_TYPE_SHIFT), flags, wepid);
					HandleRipperHitSound(i, owner, wepid);
				}
				break;
			}
		}
		
		
		//printbold(s:"running id ", d:ripper_id);
		Delay(const:1);
		first_tic = false;
		
		// update now, we updated at 0 tic case
		top_x = GetActorX(0) - (r << 16), top_y = GetActorY(0) + (r << 16), bot_x = GetActorX(0) + (r << 16), bot_y = GetActorY(0) - (r << 16);
	}
	
	SetResultValue(0);
}

// this particular version of the script runs ONCE, in corner cases where the above version did not get to run at all because of the following:
// fast projectiles dont execute their scripts if they had expired in a distance <= their speed. So we abuse this script to get it to work regardless
// when this executes they have no speed so we have no speed related checks
// TO BE USED FOR EXTREME EDGE CASE PROJECTILES, SPEED > 100
Script "DnD One Time Ripper Fix" (int dmg, int damage_type, int flags, int wepid) {
	if(CheckInventory("DnD_RippingBegan"))
		Terminate;

	int owner = GetActorProperty(0, APROP_TARGETTID);
	
	int r;
	int h;
	if(flags & DND_DAMAGEFLAG_SIMULATERIPPER) {
		r = GetUserVariable(0, "user_r");
		h = GetUserVariable(0, "user_h") << 16;
		
		// assume defaults if actor died before being set
		if(!r) {
			r = GetActorProperty(0, APROP_RADIUS) >> 16;
			h = GetActorProperty(0, APROP_HEIGHT);
		}
	}
	else {
		r = GetActorProperty(0, APROP_RADIUS) >> 16;
		h = GetActorProperty(0, APROP_HEIGHT);
	}
	
	int i = 0, m = 0, s, mn;
	int actor_flags = ScanActorFlags();
	
	// increment id by 1 for each call, doesnt matter if it overflows
	int ripper_id = -1;
	static int ripper_count = -1;
	static int ripper_hits[MAX_RIPPERS_ACTIVE][MAX_RIPPER_HITS_STORED];
	ripper_count = (ripper_count + 1) % MAX_RIPPERS_ACTIVE;
	ripper_id = ripper_count;
	
	// reset ripper hit array
	for(i = 0; i < MAX_RIPPER_HITS_STORED; ++i)
		ripper_hits[ripper_id][i] = -1;
	
	// top left, bot right -- no need to be precise with rotation of bounding box here, the engine itself uses AABB anyway
	int top_x, top_y, bot_x, bot_y;
	
	int dir_x = GetActorX(owner) - GetActorX(0);
	int dir_y = GetActorY(owner) - GetActorY(0);
	
	// steps will be from owner to proj, in a vector created that points from projectile to player
	// this is ok since if this script ever runs it means the initial ripper script did not, and we triggered this on very close dist
	// steps will be calculated by dividing it by radius of proj
	int steps = AproxDistance(dir_x, dir_y);
	if(!steps)
		steps = 1.0;
	
	dir_x = r * FixedDiv(dir_x, steps);
	dir_y = r * FixedDiv(dir_y, steps);
	
	steps /= r;
	steps >>= 16;
	if(steps < 2)
		steps = 2;
	
	// projectiles spawn speed units ahead of player, this is especially noticable in faster projectiles
	// we must check backwards initially for point blank case
	// find monsters in a rectangle from actor xyz, +-r * cos / sin and +-h on z
	// simple rectanglular box check from rectangle sides
	bool found = false;
	int a_x, a_y, a_r;
	top_x = GetActorX(0) - (r << 16), top_y = GetActorY(0) + (r << 16), bot_x = GetActorX(0) + (r << 16), bot_y = GetActorY(0) - (r << 16);
	
	// start target picking
	for(mn = 0; mn < DnD_TID_Counter[DND_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		
		// dead, skip
		if(!isActorAlive(i) || !IsMonster(i))
			continue;

		found = false;
		a_x = GetActorX(i), a_y = GetActorY(i), a_r = GetActorProperty(i, APROP_RADIUS);
		/*if(GetActorClass(i) == "DoomImp2")
			printbold(s:"dist check ", f:AproxDistance(GetActorX(0) - a_x, GetActorY(0) - a_y), s: " > ", f:5 * (a_r + (r << 16)));*/
			
		// give a more generous window here in case proj skipped way too much
		if(AproxDistance(GetActorX(0) - a_x, GetActorY(0) - a_y) > 5 * (a_r + (r << 16)) || ((actor_flags & DND_ACTORFLAG_THRUGHOST) && CheckFlag(i, "GHOST")))
			continue;
		
		for(s = 0; s < steps; ++s) {
			// eliminate cases where it'd fail to touch
			// check front and back on tic 0, because projectile spawns speed units farther
			// top and bot are projectile's coords, a_x etc. are the actor in question
			/*
			y
			|	top(x,y)
			|		*-------
			|		-------*
			|				bot(x,y)
			*-----------------------x
			*/
			
			if(CheckProjectileCollision(h, top_x, top_y, bot_x, bot_y, -s * dir_x, -s * dir_y, 0, a_x, a_y, a_r, i))
				continue;
			
			//printbold(s:"IN BOX ", d:ripper_id, s: " actor: ", d:i);
			// insert into ripper hit list, and call impact damage script on this guy IF not in list
			for(m = 0; m < MAX_RIPPER_HITS_STORED && ripper_hits[ripper_id][m] != -1; ++m) {
				if(ripper_hits[ripper_id][m] == i) {
					found = true;
					break;
				}
			}
			// not in this list yet, insert it and do damage deal routine
			if(!found && m < MAX_RIPPER_HITS_STORED) {
				//printbold(s:"deal damage to ", d:i, s: " by ripper id ", d:ripper_id);
				ripper_hits[ripper_id][m] = i;
				ACS_NamedExecuteWithResult("DnD Do Impact Damage Ripper", dmg, damage_type | (i << DAMAGE_TYPE_SHIFT), flags, wepid);
				HandleRipperHitSound(i, owner, wepid);
			}
		}
	}
	
	// start target picking
	for(mn = 0; mn < DnD_TID_Counter[DND_TID_SHOOTABLE]; ++mn) {
		i = mn + DND_SHOOTABLETID_BEGIN;
		
		// dead, skip
		if(!isActorAlive(i))
			continue;

		found = false;
		a_x = GetActorX(i), a_y = GetActorY(i), a_r = GetActorProperty(i, APROP_RADIUS);
		/*if(GetActorClass(i) == "DoomImp2")
			printbold(s:"dist check ", f:AproxDistance(GetActorX(0) - a_x, GetActorY(0) - a_y), s: " > ", f:5 * (a_r + (r << 16)));*/
			
		// give a more generous window here in case proj skipped way too much
		if(AproxDistance(GetActorX(0) - a_x, GetActorY(0) - a_y) > 5 * (a_r + (r << 16)))
			continue;
		
		for(s = 0; s < steps; ++s) {
			// eliminate cases where it'd fail to touch
			// check front and back on tic 0, because projectile spawns speed units farther
			// top and bot are projectile's coords, a_x etc. are the actor in question
			/*
			y
			|	top(x,y)
			|		*-------
			|		-------*
			|				bot(x,y)
			*-----------------------x
			*/
			
			if(CheckProjectileCollision(h, top_x, top_y, bot_x, bot_y, -s * dir_x, -s * dir_y, 0, a_x, a_y, a_r, i))
				continue;
			
			//printbold(s:"IN BOX ", d:ripper_id, s: " actor: ", d:i);
			// insert into ripper hit list, and call impact damage script on this guy IF not in list
			for(m = 0; m < MAX_RIPPER_HITS_STORED && ripper_hits[ripper_id][m] != -1; ++m) {
				if(ripper_hits[ripper_id][m] == i) {
					found = true;
					break;
				}
			}
			// not in this list yet, insert it and do damage deal routine
			if(!found && m < MAX_RIPPER_HITS_STORED) {
				//printbold(s:"deal damage to ", d:i, s: " by ripper id ", d:ripper_id);
				ripper_hits[ripper_id][m] = i;
				ACS_NamedExecuteWithResult("DnD Do Impact Damage Ripper", dmg, damage_type | (i << DAMAGE_TYPE_SHIFT), flags, wepid);
				HandleRipperHitSound(i, owner, wepid);
			}
		}
	}
	
	SetResultValue(0);
}

Script "DnD Handle Hitbeep" (int beep_type) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	if(GetCVar("dnd_hitbeeps") && !CheckInventory(HitBeepSounds[beep_type][HITBEEP_TIMER])) {
		LocalAmbientSound(HitBeepSounds[beep_type][HITBEEP_SOUND], 127);
		GiveInventory(HitBeepSounds[beep_type][HITBEEP_TIMER], 1);
	}
}

void HandleLifesteal(int pnum, int wepid, int flags, int dmg) {
	// in order for this to work we must have less health than our cap
	if(GetActorProperty(0, APROP_HEALTH) >= GetSpawnHealth())
		return;
		
	int taltos = (IsMeleeWeapon(wepid) || (flags & DND_DAMAGETICFLAG_CONSIDERMELEE)) && CheckInventory("TaltosUp");
	int brune_1 = CheckInventory("FakeBloodPower");
	int brune_2 = CheckInventory("FakeBloodPowerBetter");
	int cap = GetPlayerAttributeValue(pnum, INV_LIFESTEAL);
	if(cap || taltos || brune_1 || brune_2) {
		taltos = cap + taltos * DND_TALTOS_LIFESTEAL + brune_1 * BLOODRUNE_LIFESTEAL_AMT + brune_2 * BLOODRUNE_LIFESTEAL_AMT2;
		
		//printbold(s:"lifesteal factor ", f:taltos);
		
		// divide by 100 as its a percentage -- and >> 16 to make it int -- added little overflow check here too
		taltos /= 100;
		if(taltos > INT_MAX / dmg) {
			taltos >>= 16;
			taltos *= dmg;
		}
		else {
			taltos *= dmg;
			taltos >>= 16;
		}
				
		//printbold(s:"to be given ", d:taltos);
		
		// no longer accept 1 point of lifesteal
		if(taltos <= 0)
			return;
		
		// give up to the lifesteal limit
		brune_1 = CheckInventory("LifeStealAmount");
		cap = GetLifestealCap(pnum);
		// if over the cap, make it so that it would only be gaining up to reach the cap
		if(taltos + brune_1 > cap)
			taltos = cap - brune_1;
			
		//printbold(s:"ls amt: ", d:taltos, s: " prev counter: ", d:brune_1);
		
		if(!brune_1) {
			GiveInventory("LifeStealAmount", taltos);
			ACS_NamedExecuteAlways("DnD Lifesteal Script", 0);
		}
		else
			GiveInventory("LifeStealAmount", taltos);
	}
}

// ASSUMPTION: PLAYER RUNS THIS! -- care if adapting this later for other things
Script "DnD Damage Accumulate" (int victim_data, int wepid, int wep_neg) {
	int pnum = PlayerNumber();
	int flags = victim_data >> DND_DAMAGE_ACCUM_SHIFT;
	victim_data &= DND_MONSTER_TICDATA_BITMASK;

	int ox = PlayerDamageVector[pnum].x;
	int oy = PlayerDamageVector[pnum].y;
	int oz = PlayerDamageVector[pnum].z;
	
	Delay(const:1);

	// do the real pushing after 1 tic of dmg data has been accumulated and we have non-zero damage in effect
	// wep_neg here contains 2 bits: was it negative at 1st bit and was it a one time ripper in 2nd bit
	if((flags & DND_DAMAGETICFLAG_PUSH) && PlayerDamageTicData[pnum][victim_data] > 0)
		HandleDamagePush(2 * PlayerDamageTicData[pnum][victim_data], ox, oy, oz, victim_data + DND_MONSTERTID_BEGIN, wep_neg & 2);
	
	if(!(wep_neg & 1)) {
		// check if player has lifesteal, if they do reward some hp back
		if(!MonsterProperties[victim_data].trait_list[DND_BLOODLESS] && !(flags & DND_DAMAGETICFLAG_DOT))
			HandleLifesteal(pnum, wepid, flags, PlayerDamageTicData[pnum][victim_data]);
	}
	
	// if ice damage, add stacks of slow and check for potential freeze chance
	// do these if only the actor was alive after the tic they received dmg
	if(IsActorAlive(victim_data + DND_MONSTERTID_BEGIN)) {
		if(flags & DND_DAMAGETICFLAG_ICE)
			HandleChillEffects(pnum, victim_data + DND_MONSTERTID_BEGIN);
		else if(flags & DND_DAMAGETICFLAG_FIRE)
			HandleIgniteEffects(pnum, victim_data + DND_MONSTERTID_BEGIN, wepid);
		else if(flags & DND_DAMAGETICFLAG_LIGHTNING)
			HandleOverloadEffects(pnum, victim_data + DND_MONSTERTID_BEGIN);
		
		// frozen monsters cant retaliate		
		if(MonsterProperties[victim_data].trait_list[DND_VIOLENTRETALIATION] && random(1, 100) <= DND_VIOLENTRETALIATION_CHANCE && !CheckActorInventory(victim_data + DND_MONSTERTID_BEGIN, "DnD_FreezeTimer"))
			GiveActorInventory(victim_data + DND_MONSTERTID_BEGIN, "DnD_ViolentRetaliationItem", 1);
		GiveActorInventory(victim_data + DND_MONSTERTID_BEGIN, "DnD_HurtToken", 1);
	}

	ACS_NamedExecuteWithResult("DnD Damage Numbers", victim_data + DND_MONSTERTID_BEGIN, PlayerDamageTicData[pnum][victim_data], flags);
	
	// reset dmg counter on this mob
	PlayerDamageTicData[pnum][victim_data] = 0;
	
	SetResultValue(0);
}

Script "DnD Damage Numbers" (int tid, int dmg, int flags) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber() || !GetCVar("dnd_dmgnum"))
		Terminate;
	
	// if dmg is more than 9999 show using K instead
	int dmg_temp = dmg;
	
	bool show_k = dmg > 9999;
	if(show_k)
		dmg /= 1000;
	
	int digit_pos = 1;
	int r = GetActorProperty(tid, APROP_RADIUS) / 2;
	int x = GetActorX(tid) + random(-r, r) / 2;
	int y = GetActorY(tid) + random(-r, r) / 2;
	int z = GetActorZ(tid) + random(0.0, 16.0);
	
	// dead monsters have their height divided by 4
	if(IsActorAlive(tid))
		z += GetActorProperty(tid, APROP_HEIGHT);
	else
		z += GetActorProperty(tid, APROP_HEIGHT) * 4;
		
	// Log(f:x, s: " ", f:y, s: " ", f:z);
	
	while(dmg > 0) {
		SpawnForced(StrParam(s:"Digit", d:digit_pos, s:"Num", d:dmg % 10), x, y, z, DND_DAMAGENUMBER_TID);
		dmg /= 10;
		++digit_pos;
	}
	
	if(show_k)
		SpawnForced("ThousandSymbol", x, y, z, DND_DAMAGENUMBER_TID);
	
	SetActorVelocity(DND_DAMAGENUMBER_TID, random(-0.5, 0.5), random(-0.5, 0.5), random(0.0, 0.5), false, false);
	
	// set translation depending on crit dealt
	if(flags & DND_DAMAGETICFLAG_CRIT) {
		SetActorProperty(DND_DAMAGENUMBER_TID, APROP_SCALEX, 1.25);
		SetActorProperty(DND_DAMAGENUMBER_TID, APROP_SCALEY, 1.25);
		Thing_SetTranslation(DND_DAMAGENUMBER_TID, DND_CRIT_TRANSLATION);
	}
	
	Thing_ChangeTID(DND_DAMAGENUMBER_TID, 0);
	
	SetResultValue(0);
}

Script "DnD Do Poison Damage" (int victim, int dmg, int wepid) {
	int pnum = PlayerNumber();
	int time_limit = DND_BASE_POISON_TIMER * (100 + GetPlayerAttributeValue(pnum, INV_POISON_DURATION) + GetPlayerAttributeValue(pnum, INV_EX_DOTDURATION)) / 100;
	int trigger_tic = GetPoisonTicrate(pnum);
	
	int tic_temp = trigger_tic;
	int counter = 0;
	
	// divide trigger tic count by half to make it twice as fast -- if poison ticrate is 100% reduction we'll do poison damage at every 2 tics, which is the most one would need
	if(CheckUniquePropertyOnPlayer(pnum, PUP_POISONTICSTWICE))
		trigger_tic /= 2;

	dmg = GetPoisonDOTDamage(pnum, dmg);
		
	while(counter < time_limit && IsActorAlive(victim)) {
		if(counter >= trigger_tic) {
			HandleDamageDeal(ActivatorTID(), victim, dmg, DND_DAMAGETYPE_POISON, wepid, DND_DAMAGEFLAG_NOPOISONSTACK, 0, 0, 0, DND_ACTORFLAG_PAINLESS | DND_ACTORFLAG_NOPUSH | DND_ACTORFLAG_FOILINVUL | DND_ACTORFLAG_ISDAMAGEOVERTIME);
			ACS_NamedExecuteAlways("DnD Spawn Poison FX", 0, victim, CheckActorInventory(victim, "DnD_PoisonStacks"));
			
			// go up to the next threshold for next tic etc.
			trigger_tic += tic_temp;
		}
		counter += DND_POISON_CHECKRATE;
		Delay(const:DND_POISON_TICCHECK);
	}
	TakeActorInventory(victim, "DnD_PoisonStacks", 1);
	SetResultValue(0);
}

Script "DnD Spawn Poison FX" (int orig, int amt) CLIENTSIDE {
	int r = GetActorProperty(orig, APROP_RADIUS);
	int h = GetActorProperty(orig, APROP_HEIGHT);
	for(int i = 0; i <= amt; ++i) {
		SpawnForced("DnD_PoisonFX", GetActorX(orig) + random(-r, r), GetActorY(orig) + random(-r, r), GetActorZ(orig) + random(0, h), 0);
		Delay(random(1, 3));
	}
}

Script "DnD Monster Chill" (int victim, int pnum) {
	int cur_stacks;
	int base_speed = GetActorProperty(victim, APROP_SPEED);
	
	// revoke monster's extra fast flag if it has it
	if(MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_EXTRAFAST])
		GiveActorInventory(victim, "UnMakeFaster", 1);
	
	while((cur_stacks = CheckActorInventory(victim, "DnD_ChillStacks"))) {
		// slow down
		SetActorProperty(victim, APROP_SPEED, FixedMul(base_speed, 1.0 - GetChillEffect(pnum, cur_stacks)));
		ACS_NamedExecuteAlways("DnD Monster Chill FX", 0, victim);
		Delay(const:TICRATE);
		TakeActorInventory(victim, "DnD_ChillStacks", 1);
	}
	
	SetActorProperty(victim, APROP_SPEED, base_speed);
	
	// retain super fast property after chill ends
	if(MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_EXTRAFAST])
		GiveActorInventory(victim, "MakeFaster", 1);
}

Script "DnD Monster Chill FX" (int tid) CLIENTSIDE {
	SetActivator(tid);
	
	for(int i = 0; i < 5; ++i) {
		Delay(const:7);
		
		// don't continue if actor no longer exists -- returns 0 if thats the case
		if(!ActivatorTID())
			Terminate;
		
		if(random(0, 1))
			GiveInventory("DnD_ChillWindSpawner", 1);
	}
}

Script "DnD Monster Freeze" (int victim) {
	SetActorState(victim, "Frozen", 0);
	
	// make sure to check BEFORE actually giving NOPAIN property... this would give the trait too, oopsie here!
	bool hasNoPain = MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_NOPAIN];
	
	GiveActorInventory(victim, "MakeNoPain", 1);
	
	// actor flags dont get changed properly this way for some reason
	//printbold(s:"actor flag: ", d:CheckFlag(victim, "NOPAIN"));
	int tics = 0;
	while(CheckActorInventory(victim, "DnD_FreezeTimer")) {
		// create freeze fx and adjust it every 5 tics
		ACS_NamedExecuteWithResult("DnD Monster Freeze Adjust", victim, tics, tics >= 2, CheckActorInventory(victim, "DnD_FreezeTimer") == 1);
		Delay(const:5);
		TakeActorInventory(victim, "DnD_FreezeTimer", 1);
		tics = (tics + 1) % 4;
	}
	
	// remove frozen nopain thing if monster didnt have it before
	if(!hasNoPain)
		GiveActorInventory(victim, "TakeNoPain", 1);
	SetResultValue(0);
}

Script "DnD Monster Freeze Adjust" (int victim, int tics, int reverse, int is_last) CLIENTSIDE {
	SpawnForced("FrozenFX", GetActorX(victim), GetActorY(victim), GetActorZ(victim) + 16.0, DND_FROZENFX_TID);
	SetActorProperty(DND_FROZENFX_TID, APROP_MASTERTID, victim);
	// 127 and 121 are sprite dimensions
	SetActorProperty(DND_FROZENFX_TID, APROP_SCALEX, (GetActorProperty(victim, APROP_RADIUS) + 16.0) / 127);
	SetActorProperty(DND_FROZENFX_TID, APROP_SCALEY, (GetActorProperty(victim, APROP_HEIGHT) + 16.0) / 121);
	
	SetActivator(DND_FROZENFX_TID);
	Thing_ChangeTID(0, 0);
	
	// always face viewer
	for(int i = 0; i < 6; ++i) {
		int ang = AngleToFace(victim, ConsolePlayerNumber() + P_TIDSTART);
		SetActorPosition(0, GetActorX(victim) + 8 * cos(ang), GetActorY(victim) + 8 * sin(ang), GetActorZ(victim) + 16.0, 0);
		if(!reverse)
			SetActorProperty(0, APROP_ALPHA, 1.0 - (i + 5 * tics) * 0.025);
		else
			SetActorProperty(0, APROP_ALPHA, 0.75 + (i + 5 * (tics - 2)) * 0.025);
		Delay(const:1);
		
		// thawing
		if(is_last) {
			SetActorProperty(0, APROP_SCALEX, 8 * GetActorProperty(0, APROP_SCALEX) / 10);
			SetActorProperty(0, APROP_SCALEY, 8 * GetActorProperty(0, APROP_SCALEY) / 10);
		}
	}
	SetResultValue(0);
}

Script "DnD Monster Ignite" (int victim, int wepid, int canProlif) {
	int pnum = PlayerNumber();
	int dmg = GetFireDOTDamage(pnum);
	int dmg_tic_buff = GetPlayerAttributeValue(pnum, INV_ESS_CHEGOVAX);
	
	dmg = ACS_NamedExecuteWithResult("DND Player Damage Scale", dmg, DND_DAMAGECATEGORY_ELEMENTAL);
	
	int next_dmg = dmg;
	int inc_by = dmg * dmg_tic_buff / 100;
	
	// this is the value we will use to set the ignite timers on proliferated targets, if any
	int ign_time = CheckActorInventory(victim, "DnD_IgniteTimer");
	
	do {	
		ACS_NamedExecuteAlways("DnD Monster Ignite FX", 0, victim);
		TakeActorInventory(victim, "DnD_IgniteTimer", 1);
		HandleDamageDeal(ActivatorTID(), victim, next_dmg, DND_DAMAGETYPE_FIRE, wepid, DND_DAMAGEFLAG_NOIGNITESTACK, 0, 0, 0, DND_ACTORFLAG_NOPUSH | DND_ACTORFLAG_ISDAMAGEOVERTIME);
		//if(random(0, 1))
		//	GiveActorInventory(victim, "DnD_IgniteFXSpawner", 1);
		
		// add base damage's value, not previous
		next_dmg += inc_by;
		
		// x 5
		Delay(const:7);		
	} while(CheckActorInventory(victim, "DnD_IgniteTimer") && IsActorAlive(victim));
	
	// check ignite prolif
	int owner = P_TIDSTART + pnum;
	int prolif_dist = GetIgniteProlifRange(pnum);
	int prolif_count = GetIgniteProlifCount(pnum);
	
	next_dmg = 0; // used as temp variable
	inc_by = 0; // same as above
	dmg_tic_buff = 0; // same as above...
	
	static dist_tid_pair_T tlist[MAXPLAYERS][DND_MAX_IGNITEPROLIFS];
	
	// init list
	int i;
	for(i = 0; i < DND_MAX_IGNITEPROLIFS; ++i) {
		tlist[pnum][i].tid = 0;
		tlist[pnum][i].dist = prolif_dist;
	}
	
	// find N closest targets to victim for igniting
	//printbold(d:canProlif, s: " ", d:!IsActorAlive(victim), s: " ", d:CheckIgniteProlifChance(pnum));
	if(canProlif && !IsActorAlive(victim) && CheckIgniteProlifChance(pnum)) {
		int j, k;
		for(int mn = 0; mn < DnD_TID_Counter[DND_TID_MONSTER]; ++mn) {
			i = UsedMonsterTIDs[mn];
			if(IsActorAlive(i) && CheckFlag(i, "ISMONSTER")) {
				next_dmg = fdistance(victim, i);
				if(next_dmg < prolif_dist && CheckSight(victim, i, CSF_NOBLOCKALL)) {
					// insert sorted
					inc_by = dmg_tic_buff;
					// while our calc dist > alloc dist, keep going -- we add things to the end
					// if we come by a point where we are smaller, shift things
					for(j = 0; j < inc_by && next_dmg > tlist[pnum][j].dist; ++j);

					// we know where to add, check if we must shift (if we should)
					if(j < inc_by) {
						// less, so that means we are in-between things
						// push everything for insertion
						// this is needed to move in 0 index shifts
						if(inc_by == DND_MAX_IGNITEPROLIFS)
							--inc_by;
						
						for(k = inc_by; k > j; --k) {
							// slide data
							tlist[pnum][k].dist = tlist[pnum][k - 1].dist;
							tlist[pnum][k].tid = tlist[pnum][k - 1].tid;
						}
					}
					
					tlist[pnum][j].dist = next_dmg;
					tlist[pnum][j].tid = i;
					
					if(dmg_tic_buff < prolif_count)
						++dmg_tic_buff;
				}
			}
		}
		
		//printbold(s:"check prolif ", d:dmg_tic_buff);
		// we have things to prolif to
		if(dmg_tic_buff) {
			//printbold(s:"begin prolif");
			for(i = 0, j = 0; i < DND_MAX_IGNITEPROLIFS; ++i) {
				if(tlist[pnum][i].tid) {
					//printbold(s:"prolif to ", d:tlist[pnum][i].tid);
					// check if target was ignited already, if not ignite if so replace timer
					next_dmg = CheckActorInventory(tlist[pnum][i].tid, "DnD_IgniteTimer");
					if(!next_dmg) {
						SetActorInventory(tlist[pnum][i].tid, "DnD_IgniteTimer", ign_time);
						
						// we don't proliferate from the proliferated targets... that'd be busted
						// note: WAIT AND SEE IF ITS OP!
						ACS_NamedExecuteWithResult("DnD Monster Ignite", tlist[pnum][i].tid, wepid, true);
					}
					else
						SetActorInventory(tlist[pnum][i].tid, "DnD_IgniteTimer", Max(ign_time, next_dmg));

					// abort if we reached our count
					++j;
					if(j == dmg_tic_buff)
						break;
				}
			}
		}
	}
	
	SetActorInventory(victim, "DnD_IgniteTimer", 0);

	SetResultValue(0);
}

Script "DnD Monster Ignite FX" (int tid) CLIENTSIDE {
	SetActivator(tid);
	
	for(int i = 0; i < 2; ++i) {
		Delay(const:7);
		
		// if thing no longer exists, stop
		if(!ActivatorTID())
			Terminate;
		
		if(random(0, 1))
			GiveInventory("DnD_IgniteFXSpawner", 1);
	}
}

Script "DnD Monster Overload" (int victim) {
	// we dont have any player involvement here so
	SetActivator(victim);
	
	PlaySound(0, "Overload/Loop", CHAN_ITEM, 1.0, true);
	
	while(CheckInventory("DnD_OverloadTimer")) {
		if(!ActivatorTID())
			Terminate;
	
		ACS_NamedExecuteWithResult("DnD Monster Overload Particles");
		TakeInventory("DnD_OverloadTimer", 1);
		Delay(const:DND_BASE_OVERLOADTICK);
		GiveInventory("Overload_SoundStopper", 1);
	}

	// remove accumulated damage
	SetInventory("DnD_OverloadDamage", 0);
	StopSound(0, CHAN_ITEM);
	
	SetResultValue(0);
}

Script "DnD Monster Overload Particles" (void) CLIENTSIDE {
	int r = GetActorProperty(0, APROP_RADIUS);
	int h = GetActorProperty(0, APROP_HEIGHT);
	for(int i = 0; i < 3; ++i) {
		SpawnForced("OverloadZap_Particles", GetActorX(0) + random(-r, r) / 2, GetActorY(0) + random(-r, r) / 2, GetActorZ(0) + (random(16.0, h + 32.0)) / 2, 0);
		Delay(random(1, 3));
	}
}

// this simply distributes the overload debuff, no more zapping special fx!!
Script "DnD Monster Overload Zap" (int this, int killer) {
	if(!isPlayer(killer))
		Terminate;
	
	ActivatorSound("Overload/ZapBegin", 127);
	//SpawnForced("OverloadZap_Source", GetActorX(this), GetActorY(this), GetActorZ(this) + GetActorProperty(this, APROP_HEIGHT) + 16.0, 0);
	
	// first look up potential targets and then store them, we'll zap later with some delay
	int pnum = killer - P_TIDSTART;
	
	int i;
	int zap_count = GetPlayerAttributeValue(pnum, INV_OVERLOAD_ZAPCOUNT) + 1;
	int cur_count = 0;
	static int zap_tids[MAXPLAYERS][DND_MAX_OVERLOADTARGETS];
	for(i = 0; i < zap_count; ++i)
		zap_tids[pnum][i] = 0;
	
	// we dont deal damage now just apply debuff!
	//int dmg = ACS_NamedExecuteWithResult("DND Player Damage Scale", CheckInventory("DnD_OverloadDamage") * (100 + CheckActorInventory(killer, "IATTR_OverloadZapDmg")) / 100, TALENT_ELEMENTAL, DND_WDMG_LIGHTNINGDAMAGE);
	for(int mn = 0; mn < DnD_TID_Counter[DND_TID_MONSTER] && zap_count; ++mn) {
		// if currently alive and received the checker item
		i = UsedMonsterTIDs[mn];
		if(CheckActorInventory(i, "DnD_OverloadZapCandidate") && isActorAlive(i) && CheckFlag(i, "ISMONSTER") && i != this)
			zap_tids[pnum][cur_count++] = i;
	}
	
	for(i = 0; i < cur_count; ++i) {
		// no more zap fx
		// ACS_NamedExecuteAlways("DnD Overload Zap FX", 0, this, zap_tids[pnum][i]);
		
		// do damage, give credit to killer
		SetActivator(killer);
		
		// no more damage
		// HandleDamageDeal(killer, zap_tids[pnum][i], dmg, DND_DAMAGETYPE_LIGHTNING, 0, GetActorX(this), GetActorY(this), GetActorZ(this), DND_ACTORFLAG_FOILINVUL | DND_ACTORFLAG_FORCEPAIN);
		ActivatorSound("Overload/Zap", 127);
		
		// overload this monster if its still alive
		if(isActorAlive(zap_tids[pnum][i])) {
			if(!CheckActorInventory(zap_tids[pnum][i], "DnD_OverloadTimer")) {
				SetActorInventory(zap_tids[pnum][i], "DnD_OverloadTimer", GetOverloadTime(pnum));
				// overload damage amp is set to maximum of whatever the monster might have had (from another player) or this new instance of overload
				SetActorInventory(zap_tids[pnum][i], "DnD_OverloadDamage", Max(ConvertFixedFactorToInt(GetPlayerAttributeValue(pnum, INV_OVERLOAD_DMGINCREASE)), CheckActorInventory(zap_tids[pnum][i], "DnD_OverloadDamage")));
				ACS_NamedExecuteWithResult("DnD Monster Overload", zap_tids[pnum][i]);
			}
			else
				SetActorInventory(zap_tids[pnum][i], "DnD_OverloadTimer", GetOverloadTime(pnum));
			GiveActorInventory(zap_tids[pnum][i], "DnD_OverloadLockTime", 1);
		}
		SetActivator(this);
		
		Delay(const:DND_BASE_OVERLOADZAPDELAY);
	}
}

Script "DnD Spawn Bloodtype" (int type) CLIENTSIDE {
	SpawnForced(SpecialBloodFX[type], GetActorX(0), GetActorY(0), GetActorZ(0), SPECIAL_FX_TID);
	SetResultValue(0);
}

Script "DnD Check Explosion Repeat" (void) {
	// player information
	int owner = GetActorProperty(0, APROP_TARGETTID);
	if(!isPlayer(owner))
		owner = GetActorProperty(0, APROP_SCORE);

	int res = 0;
	int pnum = owner - P_TIDSTART;
	
	// if explosion did not repeat and we have chance for it to repeat, go for it
	if(!CheckInventory("DnD_ExplosionRepeated") && random(1, 100) <= GetExplosiveRepeatChance(pnum)) {
		GiveInventory("DnD_ExplosionRepeated", 1);
		res = 1;
	}
	
	SetResultValue(res);
}

int HandlePlayerSelfDamage(int pnum, int dmg, int dmg_type, int wepid, int flags, bool isArmorPiercing) {
	switch(dmg_type) {
		case DND_DAMAGETYPE_ENERGYEXPLOSION:
		case DND_DAMAGETYPE_EXPLOSIVES:
		case DND_DAMAGETYPE_OCCULTEXPLOSION:
			dmg = dmg * ((GetSelfExplosiveResist(pnum) * 100) >> 16) / 100;
			
			// apply accessory and other sources of damage
			dmg = HandlePlayerBuffs(pnum + P_TIDSTART, pnum + P_TIDSTART, dmg, dmg_type, wepid, flags, false);
			
			// factor in players armor here!!! -- NO DON'T DO THAT! We have a generic resist and armor handle in main dmg script
			//dmg = HandlePlayerArmor(pnum, dmg, "null", DND_DAMAGETYPEFLAG_EXPLOSIVE, isArmorPiercing);
		break;
	}
	return dmg;
}

// This function is responsible for handling all curse effects player has that affect their resistance in some way
int HandleCursePlayerResistEffects(int dmg) {
	// 50% amp, so 3 / 2
	if(CheckInventory("PowerHalfProtection"))
		dmg = ApplyDamageFactor_Safe(dmg, 3, 2);
	
	// 75% amp, so 7 / 4
	if(CheckInventory("PowerWeaken75"))
		dmg = ApplyDamageFactor_Safe(dmg, 7, 4);
	
	return dmg;
}

// dmg data encapsulates the information about what damage types this attack involved
// uses DND_DAMAGETYPEFLAG enums
int HandlePlayerResists(int pnum, int dmg, int dmg_string, int dmg_data, bool isReflected, str inflictor_class) {
	int temp;
	int dot_temp;
	
	if(isSetupComplete(SETUP_STATE1, SETUP_HARDCORE))
		dmg = ApplyDamageFactor_Safe(dmg, 100 + DND_HARDCORE_DEBUFF);
	
	dmg = HandleCursePlayerResistEffects(dmg);
	
	// reflection becomes its own thing not affected by other damage type functions, so we can immediately return here
	if(isReflected)
		return ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_REFL);
	
	if(dmg_data & DND_DAMAGETYPEFLAG_PHYSICAL)
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_PHYS);

	if(dmg_data & DND_DAMAGETYPEFLAG_HITSCAN)
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_HITSCAN);
	
	if(dmg_data & DND_DAMAGETYPEFLAG_MAGICAL)
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_MAGIC);
	
	// ELEMENTAL DAMAGE BLOCK BEGINS
	// we can only have 1 element attributed to one damage type at a time
	if(dmg_data & DND_DAMAGETYPEFLAG_FIRE) {
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_FIRE);
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_ELEM);
	}
	else if(dmg_data & DND_DAMAGETYPEFLAG_ICE) {
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_ICE);
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_ELEM);
	}
	else if(dmg_data & DND_DAMAGETYPEFLAG_LIGHTNING) {
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_LIGHTNING);
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_ELEM);
	}
	else if((dmg_data & DND_DAMAGETYPEFLAG_POISON) || dmg_string == "PoisonDOT") {
		// PoisonDOT directly deals damage through the monster, so it can't have its "stamina" / dmg_data set
		// marine 50 perk
		if(!CheckInventory("Marine_Perk50")) {
			// wanderer perk
			if(CheckInventory("Wanderer_Perk5"))
				dmg = ApplyDamageFactor_Safe(dmg, 100 - DND_WANDERER_POISONPERCENT);
			
			// reduced poison damage taken
			dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_POISON);
			dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_ELEM);
			
			// toxicology ability
			if(CheckInventory("Ability_AntiPoison")) {
				if(!CheckInventory("Cyborg_Perk25"))
					dmg = ApplyDamageFactor_Safe(dmg, 100 - DND_TOXICOLOGY_REDUCE);
				else
					dmg = ApplyDamageFactor_Safe(dmg, 100 - (DND_TOXICOLOGY_REDUCE + DND_TOXICOLOGY_REDUCE * DND_CYBORG_CYBER_MULT / DND_CYBORG_CYBER_DIV));
			}
			
			// check if we should apply poison here
			// do not register more instances on poison dots
			if(dmg && (dmg_data & DND_DAMAGETYPEFLAG_POISON)) {
				dot_temp = ApplyDamageFactor_Safe(dmg, DND_MONSTER_POISONPERCENT);
				if(!dot_temp)
					dot_temp = 1;
				// apply poison damage for 2 to 5 seconds worth 10% of the damage received from this hit
				// random damage of 10% to 12% of it is applied below
				RegisterPoisonDamage(random(dot_temp, (dot_temp * 6) / 5), random(DND_MONSTER_POISONDOT_MINTIME, DND_MONSTER_POISONDOT_MAXTIME), inflictor_class);
			}
		}
		else // marine perk50 gives immunity to poison, so reduce it to 1%
			dmg /= 100;
	}
	// ELEMENTAL DAMAGE BLOCK ENDS
	
	// explosion sources
	if(dmg_data & DND_DAMAGETYPEFLAG_EXPLOSIVE) {
		if(CheckInventory("Marine_Perk25"))
			dmg = ApplyDamageFactor_Safe(dmg, 100 - DND_MARINE_EXPLOSIVEREDUCTION);
			
		// apply impact protection research
		dmg = ApplyDamageFactor_Safe(dmg, 100 - GetResearchResistBonuses());	
		
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_EXPLOSION);
	}
	
	// energy sources
	if(dmg_data & DND_DAMAGETYPEFLAG_ENERGY) {
		if(CheckInventory("Cyborg_Perk5"))
			dmg = ApplyDamageFactor_Safe(dmg, 100 - DND_CYBORG_ENERGYREDUCE);

		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_ENERGY);
	}
	
	// gravecaller unique mod
	if(CheckUniquePropertyOnPlayer(pnum, PUP_PAINSHAREDWITHPETS)) {
		// damage is shared between you and pets, therefore if you have 1 pet you take half
		// you have 2 you get 1/3rd, which is what this'll do
		temp = CheckInventory("PetCounter") + 1;
		if(temp > DND_MAX_PET_DAMAGESHARE)
			temp = DND_MAX_PET_DAMAGESHARE;
		dmg /= temp;
		
		// distribute this damage to other pets
	}
	
	// ALL DAMAGE AMPLIFYING EFFECTS COME LAST!
	temp = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_TAKEN);
	if(temp)
		dmg = ApplyDamageFactor_Safe(dmg, 100 + temp);
	
	return dmg;
}

int GetArmorRatingEffect(int dmg, int armor_id, bool isArmorPiercing) {
	int pnum = PlayerNumber();
	int rating = GetPlayerArmor(pnum);

	if(CheckInventory("RuinationHardDebuff"))
		rating /= 4;
	else {
		pnum = CheckInventory("RuinationStacks");
		if(pnum)
			rating -= rating * pnum * DND_RUINATION_REDUCE_PER_STACK / 100;
	}

	// rating is treated as 50% instead of 100% if monster is armor piercing
	if(isArmorPiercing)
		rating >>= 1;

	return DoArmorRatingEffect(dmg, rating);
}

int HandlePlayerArmor(int pnum, int dmg, str dmg_string, int dmg_data, bool isArmorPiercing) {
	int armor_id = GetArmorID();
	if(armor_id != -1) {
		// if we are affected by poison and we dont have a specialty armor providing res to ele dmg, skip this, poison DOT shouldn't be negated by armor
		if(dmg_string == "PoisonDOT" && armor_id != BODYARMOR_ELEMENTAL)
			return dmg;
		
		// retrieve and convert factor to an integer, we convert ex: 0.417 to 417, we will apply damage factor safe method
		// dmg here is the one to be dealt to the player's health pool
		int factor = 0.0;

		// apply armor effect on this damage
		dmg = GetArmorRatingEffect(dmg, armor_id, isArmorPiercing);
		
		// special armor cases: Knight gives more reduction if using melee weapon, Duelist negates all hitscan 100% at cost of armor
		if(armor_id == BODYARMOR_KNIGHT && IsUsingMeleeWeapon())
			factor += DND_KNIGHTARMOR_MELEEWEP_BONUS;
		else if(armor_id == BODYARMOR_DUELIST && (dmg_data & DND_DAMAGETYPEFLAG_HITSCAN))
			factor = 0.75;
		
		factor *= ARMOR_INTEGER_FACTOR;
		factor >>= 16;
		
		// armor reduced factor amount of damage, this is what the player will take as damage
		dmg = ApplyDamageFactor_Safe(dmg, ARMOR_INTEGER_FACTOR - factor, ARMOR_INTEGER_FACTOR);
		
		// if we have ravager armor and on killing spree, reduce damage to 3/4 (25% reduced)
		if(armor_id == BODYARMOR_RAVAGER && CheckInventory("RavagerPower"))
			dmg = ApplyDamageFactor_Safe(dmg, DND_RAVAGER_FACTOR, DND_RAVAGER_REDUCE);
		
		// apply special reductions offered by certain armors
		if
		(
			(armor_id == BODYARMOR_GUNSLINGER 	&& (dmg_data & DND_DAMAGETYPEFLAG_HITSCAN)) 																						||
			(armor_id == BODYARMOR_OCCULT 		&& (dmg_data & DND_DAMAGETYPEFLAG_MAGICAL))																							||
			(armor_id == BODYARMOR_DEMO 		&& (dmg_data & DND_DAMAGETYPEFLAG_EXPLOSIVE))																						||
			(armor_id == BODYARMOR_ENERGY 		&& (dmg_data & DND_DAMAGETYPEFLAG_ENERGY))																							||
			(armor_id == BODYARMOR_ELEMENTAL 	&& (dmg_data & (DND_DAMAGETYPEFLAG_FIRE | DND_DAMAGETYPEFLAG_ICE | DND_DAMAGETYPEFLAG_POISON | DND_DAMAGETYPEFLAG_LIGHTNING)))
		)
		{
			dmg = ApplyDamageFactor_Safe(dmg, 100 - DND_SPECIALTYARMOR_REDUCE);
		}
		else if(armor_id == BODYARMOR_KNIGHT && dmg_string == "Melee")
			dmg = ApplyDamageFactor_Safe(dmg, 100 - DND_KNIGHT_MELEEREDUCE);
		else if(armor_id == BODYARMOR_SYNTHMETAL) {
			if(dmg_data & DND_DAMAGETYPEFLAG_HITSCAN)
				dmg = ApplyDamageFactor_Safe(dmg, 100 - DND_SYNTHMETAL_HITSCANBUFF);
			else if(dmg_data & DND_DAMAGETYPEFLAG_LIGHTNING)
				dmg = ApplyDamageFactor_Safe(dmg, 100 + DND_SYNTHMETAL_LIGHTNINGNERF);
		}
		else if((dmg_data & DND_DAMAGETYPEFLAG_LIGHTNING) && armor_id == BODYARMOR_LIGHTNINGCOIL)
			dmg = ApplyDamageFactor_Safe(dmg, 100 - DND_LIGHTNINGCOIL_SPECIAL);
	}

	// mitigation
	int temp;
	if(CouldMitigateDamage(pnum)) {
		temp = GetMitigationEffect(pnum);
		dmg = dmg * ((100.0 - temp) >> 16) / 100;
		LocalAmbientSound("Mitigation/Success", 96);
	}

	// energy shield reduction
	temp = CheckInventory("EShieldAmount");
	if(dmg_string != "PoisonDOT" && !(dmg_data & DND_DAMAGETYPEFLAG_MAGICAL) && temp) {
		// this isn't DOT or magical attack and we have energy shield, so we can deduct damage from it
		if(armor_id != BODYARMOR_LIGHTNINGCOIL)
			dmg -= temp;
		else
			dmg -= temp / 5; // only 20% reduced from ES

		if(dmg < 0) {
			// completely absorbed by our shield, so set our shield to -dmg
			SetEnergyShield(-dmg);
			dmg = 0;
			LocalAmbientSound("EShield/Hit", 127);
		}
		else {
			// fully depleted if dmg >= shield, we couldnt reduce it
			SetEnergyShield(0);
			LocalAmbientSound("EShield/Break", 127);
		}
	}
	
	return dmg;
}

void HandleMonsterDamageModChecks(int m_id, int monster_tid, int victim) {
	// vampirism check
	if(MonsterProperties[m_id].trait_list[DND_VAMPIRISM] && isActorAlive(monster_tid)) {
		// if this monster is trying to leech off of a bloodless monster, do not allow (we cant have all rules be against players... right?)
		if(IsMonster(victim) && MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_BLOODLESS])
			return;
	
		// 10% or 10 flat healing per hit, minimum
		int hp = Max(MonsterProperties[m_id].maxhp / 10, 10);
		
		// ignite effects prevent vampirism healing
		if(!CheckActorInventory(monster_tid, "DnD_IgniteTimer")) {
			if(GetActorProperty(monster_tid, APROP_HEALTH) < MonsterProperties[m_id].maxhp - hp)
				SetActorProperty(monster_tid, APROP_HEALTH, GetActorProperty(monster_tid, APROP_HEALTH) + hp);
			else
				SetActorProperty(monster_tid, APROP_HEALTH, MonsterProperties[m_id].maxhp);
			ACS_NamedExecuteAlways("DnD Vampirism FX CS", 0, monster_tid);
		}
	}	
}

int HandlePercentDamageFromEnemy(int dmg, int dmg_data) {
	// check inflictor momentarily
	SetActivator(0, AAPTR_DAMAGE_INFLICTOR);
	if(CheckInventory("DnD_PercentDamageHalt")) {
		SetActivator(0, AAPTR_DAMAGE_TARGET);
		return 0;
	}
	
	GiveInventory("DnD_PercentDamageHalt", 1);
	SetActivator(0, AAPTR_DAMAGE_TARGET);
	
	return !!(dmg_data & DND_DAMAGETYPEFLAG_PERCENTHP) * (GetActorProperty(0, APROP_HEALTH) * DND_MONSTER_PERCENTDAMAGEBASE) / 100;
}

int MonsterSpecificDamageChecks(int m_id, int victim, int dmg) {
	if(IsMonsterIdBoss(MonsterProperties[m_id].id) && IsQuestComplete(victim, QUEST_KILL10BOSSES))
		dmg = dmg * (100 - DND_QUEST_BOSSREDUCE) / 100;
	return dmg;
}

void OnPlayerHit(int this, int pnum, int target, bool isMonster) {
	if(CheckActorInventory(this, "HateCheck") && target != this && isMonster)
		GiveActorInventory(target, "HateWeakness", 1);
	
	// necro and lightning coil chance
	int temp = GetActorArmorID(this);
	if(temp == BODYARMOR_NECRO && !CheckActorInventory(this, "NecroSpikeCooldown") && !random(0, 2)) {
		GiveActorInventory(this, "NecroSpikeShooter", 1);
		GiveActorInventory(this, "NecroSpikeCooldown", 1);
	}
	else if(temp == BODYARMOR_LIGHTNINGCOIL && !CheckActorInventory(this, "LightningCoilCooldown") && !random(0, 3)) {
		// 25% chance
		GiveActorInventory(this, "LightningCoilShooter", 1);
		GiveActorInventory(this, "LightningCoilCooldown", 1);
	}
	
	// player heal on hit check -- target is 0 if we are the target, but the extra check in there is for safety
	temp = GetPlayerAttributeValue(pnum, INV_EX_CHANCE_HEALMISSINGONPAIN);
	if(temp && target && target != this) {
		// roll chance
		if(random(1, 100) <= GetPlayerAttributeExtra(pnum, INV_EX_CHANCE_HEALMISSINGONPAIN)) {
			// heal for missing health
			GiveActorInventory(this, "VeilHealFXSpawner", 1); // use same fx as veil for now
			
			SetActivator(this);
			HandleHealthPickup((temp * GetMissingHealth()) / 100, 0, 0);
			// restore ptr
			SetActivator(0, AAPTR_DAMAGE_TARGET);
		}
	}
	
	// check perk25 for berserker with cooldown
	if(CheckActorInventory(this, "Berserker_Perk25") && !CheckActorInventory(this, "Berserker_Perk25_CD")) {
		// basically make sure only one instance of this runs
		if(!CheckActorInventory(this, "Berserker_DamageTimer"))
			ACS_NamedExecuteAlways("DnD Berserker Perk25", 0);
			
		SetActorInventory(this, "Berserker_DamageTimer", DND_BERSERKER_DAMAGETRACKTIME);
		GiveActorInventory(this, "Berserker_Perk25_CD", 1);
		if(CheckActorInventory(this, "Berserker_DamageTracker") < DND_BERSERKER_PERK25_MAXSTACKS)
			GiveActorInventory(this, "Berserker_DamageTracker", 1);
	}
	
	// rest of the code doesn't work if we weren't hit by a monster
	if(!isMonster)
		return;
	
	// monster might be thief, check it
	int m_id = target - DND_MONSTERTID_BEGIN;
	if(MonsterProperties[m_id].trait_list[DND_THIEF]) {
		// get current weapon's ammo and steal it if possible
		temp = random(0, 1);
		str cur_ammo = GetWeaponAmmoType(GetActorWeaponID(this), temp);
		
		// if we picked no ammo, flip to check the other one using negation
		if(cur_ammo == " ")
			cur_ammo = GetWeaponAmmoType(GetActorWeaponID(this), !temp);
		
		// if it's something we can take ammo from		
		if(cur_ammo != " ")
			TakeActorInventory(this, cur_ammo, CheckActorInventory(this, cur_ammo) * DND_ELITE_THIEFRATE / 100);
	}
	
	// shocker check
	if(MonsterProperties[m_id].trait_list[DND_SHOCKER])
		GiveActorInventory(this, "PlayerStopper", 1);
	
	// ruination check
	if(MonsterProperties[m_id].trait_list[DND_RUINATION]) {
		if(!CheckActorInventory(this, "RuinationStacks")) {
			SetActivator(this);
			
			ACS_NamedExecuteAlways("DnD Ruination Ticker", 0);
			
			// restore ptr
			SetActivator(0, AAPTR_DAMAGE_TARGET);
		}
		GiveActorInventory(this, "RuinationStacks", 1);

		if(CheckActorInventory(this, "RuinationStacks") == RUINATION_MAX_STACKS)
			HandleRuination(this, target);
	}

	// the curse is applied if the player is not immune, the checks are delegated to curse items
	if(MonsterProperties[m_id].trait_list[DND_HEXFUSION] && random(1, 100) <= DND_HEXFUSION_CHANCE)
		ApplyRandomCurse(this);
}

Script "DnD Event Handler" (int type, int arg1, int arg2) EVENT {
	// arg1 contains damage, arg2 contains damage type as a string
	// this causes A_KillChildren etc. to actually work...
	if(arg2 == "Perish") {
		SetResultValue(arg1);
		Terminate;
	}

	int temp, dmg, m_id;
	int pnum;
	if(type == GAMEEVENT_ACTOR_DAMAGED) {
		bool isRipper = false;
		
		// damage inflictor (projectile etc.) -- reflected projectiles seem to have "None" as their class
		// poisonDOT or any DOT has this characteristic as well so we must check for those as exceptions here
		SetActivator(0, AAPTR_DAMAGE_INFLICTOR);
		//printbold(s:GetactorClass(0), s:" inflicts damage ", d:GetActorProperty(0, APROP_DAMAGE), s: " ", d:arg1, s:" type ", s:arg2);
		int dmg_data = GetActorProperty(0, APROP_STAMINA);
		// printbold(s:"dmg flag: ", d:dmg_data);
		int inflictor_class = GetActorClass(0);
		bool isReflected = inflictor_class == "None" && arg2 != "PoisonDOT";
		bool isArmorPiercing = CheckFlag(0, "PIERCEARMOR");
		if(CheckFlag(0, "RIPPER"))
			isRipper = true;
		
		// set activator to owner of this projectile for certain crediting
		SetActivator(0, AAPTR_DAMAGE_SOURCE);
		int shooter = ActivatorTID();
		
		// if the inflictor had no damage data for some reason, try to look it up from the monster
		if(!dmg_data)
			dmg_data = GetActorProperty(0, APROP_STAMINA);
		isArmorPiercing |= CheckFlag(shooter, "PIERCEARMOR");

		// set the activator to us now
		SetActivator(0, AAPTR_DAMAGE_TARGET);
		int victim = ActivatorTID();
		
		dmg = arg1;
		
		// printbold(s:"do dmg from inflictor class ", s:inflictor_class, s: " isReflected? ", d:isReflected, s: " ", d:dmg_data);
		
		// FROM HERE ON WHOEVER TOOK DAMAGE IS THE ACTIVATOR, PLAYER OR MONSTER!
		if(IsMonster(shooter)) {
			m_id = shooter - DND_MONSTERTID_BEGIN;
		
			// if victim was a monster, check for infight situation
			// BOTH VICTIM AND SHOOTER ARE MONSTERS HERE
			// printbold(s:GetActorProperty(victim, APROP_SPECIES), s: " ", s:GetActorProperty(shooter, APROP_SPECIES));
			if(IsMonster(victim) && victim != shooter && GetActorProperty(victim, APROP_SPECIES) == GetActorProperty(shooter, APROP_SPECIES)) {
				// no damage dealt from same species, makes damage things much easier to keep track of	
				SetResultValue(0);
				Terminate;
			}

			int factor = 0;
			
			// dont scale reflected damage by this
			// special bonuses
			factor += 	!isReflected * (MonsterProperties[m_id].level > 1) * GetMonsterDMGScaling(m_id, MonsterProperties[m_id].level) +
						MonsterProperties[m_id].trait_list[DND_EXTRASTRONG] * DND_ELITE_EXTRASTRONG_BONUS;
				
			dmg = dmg * (100 + factor) / 100;
			
			// elite damage bonus is multiplicative
			if(MonsterProperties[m_id].isElite/* && dmg < INT_MAX / factor*/)
				dmg = dmg * (100 + GetEliteBonusDamage(m_id)) / 100;
				
			// chaos mark is multiplicative
			factor = 100 + CHAOSMARK_DAMAGEBUFF;
			if(MonsterProperties[m_id].trait_list[DND_MARKOFCHAOS]/* && dmg < INT_MAX / factor*/)
				dmg = dmg * (100 + CHAOSMARK_DAMAGEBUFF) / 100;
				
			if(isRipper)
				dmg >>= 1;
				
			// halved by demon sealer effect if any
			if(CheckActorInventory(shooter, "DemonSealDamageDebuff"))
				dmg >>= 1;
				
			// % damage effects -- this is same for all monsters which is 10% of player's maximum health added as damage
			dmg += HandlePercentDamageFromEnemy(dmg, dmg_data);

			// if this was a player, factor their resists in
			// resists of player now will factor in after we've calculated the damage accurately
			if(IsPlayer(victim)) {
				pnum = victim - P_TIDSTART;

				// out of combat hit timer, 3 seconds
				GiveActorInventory(victim, "DnD_Hit_CombatTimer", 1);
				
				if(!CheckActorInventory(victim, "DnD_Hit_Cooldown")) {
					OnPlayerHit(victim, pnum, shooter, true);
					GiveActorInventory(victim, "DnD_Hit_Cooldown", 1);
				}
				
				// hate shard reflection
				if(CheckActorInventory(victim, "HateCheck")) {
					// this is needed for kill credit
					if(GetActorProperty(shooter, APROP_HEALTH) < dmg)
						GiveActorInventory(shooter, "MonsterKilledByPlayer", 1);
					Thing_Damage2(shooter, dmg, "Reflection");
					ACS_NamedExecuteWithResult("DnD Damage Numbers", shooter, dmg, 0);
				}
				
				temp = CheckInventory("Perk_Endurance");
				if(temp) {
					// 1000 because integer factor is 35 => to make 3.5% we scale by 10
					//if(dmg < INT_MAX / 1000)
						dmg = dmg * (1000 - temp * ENDURANCE_RES_INTEGER) / 1000;
					/*else
						dmg = (dmg / 1000) * (1000 - temp * ENDURANCE_RES_INTEGER);*/
				}
				
				// check for special reduced damage factors
				// store damage before reductions to apply to armor later
				dmg = HandlePlayerResists(pnum, dmg, arg2, dmg_data, isReflected, inflictor_class);

				// 90% less => 1/10th
				if(isReflected)
					dmg /= 10;
				
				// finally apply player armor
				dmg = HandlePlayerArmor(pnum, dmg, arg2, dmg_data, isArmorPiercing);
					
				// doomguy demon reduction
				if(IsMonsterIdDemon(m_id) && CheckInventory("Doomguy_Perk5"))
					dmg = ApplyDamageFactor_Safe(dmg, 100 - DND_DOOMGUY_DMGREDUCE_PERCENT);

				// berserker damage reduction
				temp = CheckInventory("Berserker_DamageTracker");
				if(temp)
					dmg = ApplyDamageFactor_Safe(dmg, 100 - temp * DND_BERSERKER_PERK25_REDUCTION);
					
				dmg = HandlePlayerOnHitBuffs(victim, shooter, dmg, dmg_data, arg2);
				
				// damage amplifications
				temp = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_TAKEN);
				if(temp)
					dmg = ApplyDamageFactor_Safe(dmg, 100 + temp);
					
				dmg = MonsterSpecificDamageChecks(m_id, victim, dmg);
					
				if(dmg) {
					// add to player stat
					IncrementStatistic(DND_STATISTIC_DAMAGETAKEN, dmg, victim);
					//GiveInventory("DnD_DamageReceived", dmg);
					PlayerScriptsCheck[DND_SCRIPT_DAMAGETAKENTIC][pnum] = dmg;
					PlayerScriptsCheck[DND_SCRIPT_BLEND][pnum] = false;
					
					// mugshot hook
					ACS_NamedExecuteWithResult("DnD Player MugshotData Ouchies", shooter, dmg);
				}
			}
			
			// these are on monsters only, dont have much to do with us beyond this point
			HandleMonsterDamageModChecks(m_id, shooter, victim);
			
			//printbold(s:"old dmg ", d:arg1, s: " new dmg: ", d:dmg);
			SetResultValue(dmg);
		}
		else if(IsPlayer(shooter) && IsPlayer(victim) && shooter != victim) {
			// if victim is a player, and shooter is also a player and its not us, make sure they take no damage! YOU NEVER KNOW!!
			SetResultValue(0);
		}
		else if(IsPet(victim) && IsPlayer(shooter)) {
			// players shouldnt hurt pets
			SetResultValue(0);
		}
		else if(IsPet(shooter) && shooter != victim) {
			// shooter is pet, it most likely attacked a monster, factor in things related to pets and put damage numbers!
			// make sure activator is the player themselves now
			SetActivator(GetActorProperty(shooter, APROP_MASTERTID));
			ACS_NamedExecuteWithResult("DnD Damage Numbers", victim, dmg, 0);
			SetResultValue(dmg);
		}
		else if(IsMonster(victim) && dmg_data) {
			// last option, player hurt monster in here --- we normally don't handle this here but for reflection we can
			// if we have dmg_data, currently it can only come from monster projectile
			SetActivator(GetActorProperty(shooter, APROP_MASTERTID));
			ACS_NamedExecuteWithResult("DnD Damage Numbers", victim, dmg, 0);
			SetResultValue(dmg);
			Terminate;
		}
		else if(!IsMonster(victim)) {
			// the above check was necessary
			// hurt self -- handleplayerselfdamage is ran in explosion side of things, we run additional stuff that isnt handled by that here, like resists and armor
			pnum = PlayerNumber();
			
			if(!CheckActorInventory(victim, "DnD_Hit_Cooldown")) {
				OnPlayerHit(victim, pnum, shooter, false);
				GiveActorInventory(victim, "DnD_Hit_Cooldown", 1);
			}

			dmg = HandlePlayerResists(PlayerNumber(), dmg, arg2, dmg_data, isReflected, inflictor_class);
			dmg = HandlePlayerArmor(pnum, dmg, arg2, dmg_data, false);
			//GiveInventory("DnD_DamageReceived", dmg);
			PlayerScriptsCheck[DND_SCRIPT_DAMAGETAKENTIC][pnum] = dmg;
			IncrementStatistic(DND_STATISTIC_DAMAGETAKEN, dmg, victim);
			
			// mugshot hook
			ACS_NamedExecuteWithResult("DnD Player MugshotData Ouchies", shooter, dmg);
			
			SetResultValue(dmg);
		}
	}
}

#endif
