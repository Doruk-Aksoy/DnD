#ifndef DND_DAMAGE_IN
#define DND_DAMAGE_IN

#include "DnD_Poison.h"
#include "DnD_Physics.h"
#include "DnD_AttackInfo.h"

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

#define DND_ADDEDIGNITE_FACTOR 50 // 50%

#define DND_MONSTER_PERCENTDAMAGEBASE 10 // 10%
#define DND_MONSTER_PERCENTDAMAGEBASE_LOW 2 // 2%

#define DND_MONSTER_POISONPERCENT 25 // 25% of damage taken from a hit is dealt as poison damage again over the duration
#define DND_MONSTER_POISONDOT_MINTIME 2
#define DND_MONSTER_POISONDOT_MAXTIME 5

#define OCCULT_WEAKEN_DURATION 2

#define DND_CORRUPTORB_DMGREDUCE 75 // /4 => 75% reduced dmg

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

// monster flag encoding in stamina
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
	DND_DAMAGETYPEFLAG_SPELL = 1024,
	DND_DAMAGETYPEFLAG_PERCENTHP_LOW = 2048,
	DND_DAMAGETYPEFLAG_DOT = 4096,

	DND_DAMAGETYPEFLAG_REFLECTABLE = 1073741824
};

enum {
	DND_IGNITEFLAG_CANPROLIF = 1,
	DND_IGNITEFLAG_ADDEDIGN = 2
};

int MonsterDamageTypeToDamageCategory(int d) {
	if(d & DND_DAMAGETYPEFLAG_PHYSICAL)
		return DND_DAMAGECATEGORY_BULLET;

	if(d & DND_DAMAGETYPEFLAG_FIRE)
		return DND_DAMAGECATEGORY_FIRE;
	if(d & DND_DAMAGETYPEFLAG_ICE)
		return DND_DAMAGECATEGORY_FIRE;
	if(d & DND_DAMAGECATEGORY_ICE)
		return DND_DAMAGECATEGORY_POISON;
	if(d & DND_DAMAGETYPEFLAG_LIGHTNING)
		return DND_DAMAGECATEGORY_LIGHTNING;

	if(d & DND_DAMAGETYPEFLAG_ENERGY)
		return DND_DAMAGECATEGORY_ENERGY;

	if(d & DND_DAMAGETYPEFLAG_MAGICAL)
		return DND_DAMAGECATEGORY_OCCULT;

	if(d & DND_DAMAGECATEGORY_EXPLOSIVES)
		return DND_DAMAGECATEGORY_EXPLOSIVES;

	return DND_DAMAGECATEGORY_MELEE;
}

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
	return damage_type == DND_DAMAGETYPE_EXPLOSIVES;
}

bool IsEnergyDamage(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_ENERGY && damage_type <= DND_DAMAGETYPE_ENERGYEXPLOSION;
}

bool IsOccultDamage(int damage_type) {
	return (damage_type >= DND_DAMAGETYPE_OCCULT && damage_type <= DND_DAMAGETYPE_MAGICSEAL) || damage_type == DND_DAMAGETYPE_MELEEOCCULT || damage_type == DND_DAMAGETYPE_SOUL;
}

bool IsFireDamage(int damage_type) {
	return damage_type == DND_DAMAGETYPE_FIRE;
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

bool IsElementalDamageType(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_FIRE && damage_type <= DND_DAMAGETYPE_LIGHTNING;
}

bool IsElementalDamageCategory(int damage_category) {
	return damage_category >= DND_ELECATEGORY_BEGIN && damage_category <= DND_ELECATEGORY_END;
}

bool IsDamageCapableOfIgnite(int damage_type) {
	return damage_type == DND_DAMAGETYPE_FIRE || damage_type == DND_DAMAGETYPE_OCCULTFIRE;
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
	else if(IsFireDamage(damage_type))
		return DND_DAMAGECATEGORY_FIRE;
	else if(IsIceDamage(damage_type))
		return DND_DAMAGECATEGORY_ICE;
	else if(IsPoisonDamage(damage_type))
		return DND_DAMAGECATEGORY_POISON;
	else if(IsLightningDamage(damage_type))
		return DND_DAMAGECATEGORY_LIGHTNING;
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

#define DND_EXTRAUNDEADDMG_MULTIPLIER 300

#define DND_MAX_MONSTER_TICDATA 16383 // even this is a bit much but w.e
#define DND_MONSTER_TICDATA_BITMASK 0x3FFF // 14 bits
#define DND_DAMAGE_ACCUM_SHIFT 14 // 2^14 = 16384
global int 27: PlayerDamageTicData[MAXPLAYERS][DND_MAX_MONSTER_TICDATA];

// we use this as a bitfield -- 64 players => 2 ints
// stores player weapon crit state
int PlayerDamageCritState[MAXWEPS][2];
int PlayerDamageCritLock[MAXWEPS][2];
#define CRIT_CLEAR_WAIT_TIME 2 // +1 tics added on top

bool GetPlayerWeaponCritState(int pnum, int wepid) {
	if(pnum > 31)
		return PlayerDamageCritState[wepid][1] & (1 << (pnum - 32));
	return PlayerDamageCritState[wepid][0] & (1 << pnum);
}

void SetPlayerWeaponCritState(int pnum, int wepid) {
	if(pnum > 31)
		PlayerDamageCritState[wepid][1] |= (1 << (pnum - 32));
	PlayerDamageCritState[wepid][0] |= (1 << pnum);
}

void UnsetPlayerWeaponCritState(int pnum, int wepid) {
	if(pnum > 31)
		PlayerDamageCritState[wepid][1] &= ~(1 << (pnum - 32));
	PlayerDamageCritState[wepid][0] &= ~(1 << pnum);
}

bool GetPlayerWeaponCritLock(int pnum, int wepid) {
	if(pnum > 31)
		return PlayerDamageCritLock[wepid][1] & (1 << (pnum - 32));
	return PlayerDamageCritLock[wepid][0] & (1 << pnum);
}

void LockPlayerCritState(int pnum, int wepid) {
	if(pnum > 31)
		PlayerDamageCritLock[wepid][1] |= (1 << (pnum - 32));
	PlayerDamageCritLock[wepid][0] |= (1 << pnum);
}

void UnlockPlayerCritState(int pnum, int wepid) {
	if(pnum > 31)
		PlayerDamageCritLock[wepid][1] &= ~(1 << (pnum - 32));
	PlayerDamageCritLock[wepid][0] &= ~(1 << pnum);
}

// All resists uniformly follow same factors
int ApplyPlayerResist(int pnum, int dmg, int res_attribute, int bonus = 0) {
	int unity = 1.0 * GetPlayerAttributeValue(pnum, INV_EX_UNITY_RES_BONUS) * GetUnity() / DND_UNITY_DIVISOR;
	int temp = 	GetPlayerAttributeValue(pnum, res_attribute) + 
				bonus + 
				GetPlayerAttributeValue(pnum, INV_DMGREDUCE_ALL) +
				unity +
				DND_PLAYER_RESIST_REDUCE * (GetLevel() / DND_PLAYER_WEAKEN_LEVELS);

	unity = GetPlayerAttributeValue(pnum, INV_EX_RESPERESHIELD);
	if(unity)
		temp += DND_RES_PER_PRISMGUARD * (CheckInventory("EShieldAmount") / unity);

	if(!temp)
		return dmg;
	
	// roll damage up
	temp = ApplyResistCap(pnum, temp) + 0.05;
	
	return dmg * ((100.0 - temp) >> 16) / 100;
}

int GetLowestResist(int pnum) {
	static int res_ids[9][2] = { 
		{ INV_DMGREDUCE_PHYS, DND_DAMAGETYPEFLAG_PHYSICAL },
		{ INV_DMGREDUCE_HITSCAN, DND_DAMAGETYPEFLAG_HITSCAN },
		{ INV_DMGREDUCE_EXPLOSION, DND_DAMAGETYPEFLAG_EXPLOSIVE },
		{ INV_DMGREDUCE_MAGIC, DND_DAMAGETYPEFLAG_MAGICAL },
		{ INV_DMGREDUCE_ENERGY, DND_DAMAGETYPEFLAG_ENERGY },
		{ INV_DMGREDUCE_FIRE, DND_DAMAGETYPEFLAG_FIRE },
		{ INV_DMGREDUCE_ICE, DND_DAMAGETYPEFLAG_ICE },
		{ INV_DMGREDUCE_LIGHTNING, DND_DAMAGETYPEFLAG_LIGHTNING },
		{ INV_DMGREDUCE_POISON, DND_DAMAGETYPEFLAG_POISON }
	};

	int val = INT_MAX;
	int min_type = 0;
	for(int i = 0; i < 9; ++i) {
		int temp = GetPlayerAttributeValue(pnum, res_ids[i][0]);
		if(val >= temp) {
			// not yet established, reset
			if(val != temp)
				min_type = 0;
			val = temp;
			min_type |= res_ids[i][1];
		}
	}

	return min_type;
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
			
			/*res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
			SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);*/
			
			SetActivator(0, AAPTR_TARGET);
		}
	}
	else if(flags & DND_WDMG_USEMASTER) {
		if(flags & DND_WDMG_SETMASTER) {
			temp = GetActorProperty(0, APROP_MASTERTID);
		
			/*res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
			SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);*/
		
			// this is a hack
			SetPointer(AAPTR_TARGET, temp);
		}
		if(!GetActorProperty(0, APROP_SCORE)) {
			temp = GetActorProperty(0, APROP_TARGETTID);
		
			SetActorProperty(0, APROP_SCORE, temp);
			
			/*res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
			SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);*/
			
			SetActivator(GetActorProperty(0, APROP_MASTERTID));
		}
	}
	else if(flags & DND_WDMG_USETRACER) {
		temp = GetActorProperty(0, APROP_TRACERTID);
		
		/*res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
		SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);*/
	
		SetActivator(0, AAPTR_TRACER);
	}
	else if(flags & DND_WDMG_USETARGETSMASTER) {
		temp = ACS_NamedExecuteWithResult("DnD Get Master of Target");
		if(!GetActorProperty(0, APROP_SCORE))
			SetActorProperty(0, APROP_SCORE, temp);
		
		/*res = crit_check && (CheckActorInventory(temp, "DnD_CritToken") || (wepid != -1 && PlayerCritState[temp - P_TIDSTART][DND_CRITSTATE_CONFIRMED][wepid]));
		SetActorProperty(0, APROP_ACCURACY, res * DND_CRIT_TOKEN);*/
		
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

// for now does nothing per weapon, later add new corruptions perhaps
int GetPlayerIgniteAddedDmg(int pnum, int wepid, int added_dmg) {
	return added_dmg * DND_ADDEDIGNITE_FACTOR / 100;
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
// uses DND_DAMAGEFLAG for flags
int RetrieveWeaponDamage(int pnum, int wepid, int dmgid, int damage_category, int flags, int isSpecial) {
	// do not lose the weaponid on special ammo -- normally its DMG_ID & (wepid << 16) but special ammo just have the id of the special ammo instead of dmg_id
	// add +1 because flechette is id 0
	if(isSpecial) {
		isSpecial = wepid + 1;
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
	
	//printbold(s:"add ", d:MapDamageCategoryToFlatBonus(pnum, damage_category, damage_category_flags));
	dmg += MapDamageCategoryToFlatBonus(pnum, damage_category);
	
	// overall percentage bonuses -- this is basically ScaleCachedDamage but unwrapped, we need to rewrite these into a common function that just retrieves the overall bonus factor to multiply with!
	int factor = 100 + GetPlayerPercentDamage(pnum, -1, damage_category);
	
	// apply flat health to damage conversion if player has any
	int temp = GetPlayerAttributeValue(pnum, INV_EX_PHYSDAMAGEPER_FLATHEALTH);
	if((damage_category == DND_DAMAGECATEGORY_MELEE || damage_category == DND_DAMAGECATEGORY_BULLET) && temp)
		factor += GetFlatHealthDamageFactor(temp);
		
	temp = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_LIGHTNING);
	if(temp && IsLightningDamage(damage_type))
		factor += temp;
	
	temp = IsElementalDamageCategory(damage_category);
	if(temp && IsQuestComplete(tid, QUEST_KILLMORDECQAI))
		factor += DND_MORDECQAI_BOOST;

	// THESE ARE MULTIPLICATIVE STACKING BONUSES BELOW -- HAVE KEYWORD: MORE
	// quest or accessory bonuses	
	// is occult (add demon bane bonus)
	
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
	int temp,  pct_tmp = 0;
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

	bool is_melee_mastery_exception = (IsMeleeWeapon(wepid) || (flags & DND_DAMAGEFLAG_COUNTSASMELEE)) && HasMasteredPerk(STAT_BRUT);

	// only store scaling factors here for later use, no modifying damage in this block
	// damage modifications are done at the end
	if(PlayerDamageNeedsCaching(pnum, wepid, dmgid)) {
		// add potential shotgun flat damage
		temp = (!!IsBoomstick(wepid)) * GetPlayerAttributeValue(pnum, INV_EX_FLATPERSHOTGUNOWNED) * CountShotgunWeaponsOwned();
		
		// add flat damage bonus mapping talent name to flat bonus type
		temp += MapDamageCategoryToFlatBonus(pnum, damage_category);
		
		ClearCache(pnum, wepid, dmgid);
		
		if(flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME)
			temp += GetPlayerAttributeValue(pnum, INV_EX_FLATDOT);

		// special weapon type checks
		if(IsTechWeapon(wepid)) {
			temp += GetPlayerAttributeValue(pnum, INV_FLAT_TECH);
			pct_tmp += GetPlayerAttributeValue(pnum, INV_TECH_PERCENT);
		}

		if(IsHandgun(wepid)) {
			temp += GetPlayerAttributeValue(pnum, INV_FLAT_HANDGUN);
			pct_tmp += GetPlayerAttributeValue(pnum, INV_HANDGUN_PERCENT);
		}

		if(IsBoomstick(wepid)) {
			temp += GetPlayerAttributeValue(pnum, INV_FLAT_SHOTGUN);
			pct_tmp += GetPlayerAttributeValue(pnum, INV_SHOTGUN_PERCENT);
		}

		if(IsAutomaticWeapon(wepid)) {
			temp += GetPlayerAttributeValue(pnum, INV_FLAT_AUTOMATIC);
			pct_tmp += GetPlayerAttributeValue(pnum, INV_AUTOMATIC_PERCENT);
		}

		if(IsPrecisionWeapon(wepid)) {
			temp += GetPlayerAttributeValue(pnum, INV_FLAT_PRECISION);
			pct_tmp += GetPlayerAttributeValue(pnum, INV_PRECISION_PERCENT);
		}

		if(IsArtilleryWeapon(wepid)) {
			temp += GetPlayerAttributeValue(pnum, INV_FLAT_ARTILLERY);
			pct_tmp += GetPlayerAttributeValue(pnum, INV_ARTILLERY_PERCENT);
		}

		if(IsMagicalWeapon(wepid)) {
			temp += GetPlayerAttributeValue(pnum, INV_FLAT_MAGIC);
			pct_tmp += GetPlayerAttributeValue(pnum, INV_MAGIC_PERCENT);
		}
		
		CachePlayerFlatDamage(pnum, temp, wepid, dmgid);
		
		int mult_factor = 0;
		
		// include the stat attunement bonus
		InsertCacheFactor(pnum, wepid, dmgid, GetStatAttunementBonus(pnum, wepid, damage_category == DND_DAMAGECATEGORY_MELEE || is_melee_mastery_exception), true);

		// include enhancement orb bonuses
		temp = GetPlayerWeaponEnchant(pnum, wepid);
		if(temp) {
			temp *= ENHANCEORB_BONUS;
			InsertCacheFactor(pnum, wepid, dmgid, temp, true);
		}
			
		// finally apply damage type or percentage bonuses
		// last one is for ghost hit power, we reduce its power by a factor -- add the top pct values from above to here too
		temp = GetPlayerPercentDamage(pnum, wepid, damage_category) + pct_tmp;
		if(damage_category != DND_DAMAGECATEGORY_MELEE && is_melee_mastery_exception)
			temp += GetPlayerPercentDamage(pnum, wepid, DND_DAMAGECATEGORY_MELEE);
		if(temp)
			InsertCacheFactor(pnum, wepid, dmgid, temp, true);
		
		// special damage increase attributes -- usually obtained by means of charms
		temp = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_LIGHTNING);
		if(temp && IsWeaponLightningType(wepid))
			InsertCacheFactor(pnum, wepid, dmgid, temp, true);
			
		// apply flat health to damage conversion if player has any
		temp = GetPlayerAttributeValue(pnum, INV_EX_PHYSDAMAGEPER_FLATHEALTH);
		if((damage_category == DND_DAMAGECATEGORY_MELEE || damage_category == DND_DAMAGECATEGORY_BULLET) && temp)
			InsertCacheFactor(pnum, wepid, dmgid, GetFlatHealthDamageFactor(temp), true);
			
		// factor dot % increase if this is a dot attack
		if(flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME)
			InsertCacheFactor(pnum, wepid, dmgid, GetPlayerAttributeValue(pnum, INV_INCREASEDDOT), true);
			
		// check for quest % increases
		if(IsQuestComplete(tid, QUEST_ONLYONEWEAPON))
			InsertCacheFactor(pnum, wepid, dmgid, DND_QUEST_ONEWEAPON_BONUS, true);
		
		temp = IsElementalDamageCategory(damage_category);
		if(temp && IsQuestComplete(tid, QUEST_KILLMORDECQAI))
			InsertCacheFactor(pnum, wepid, dmgid, DND_MORDECQAI_BOOST, true);

		// THESE ARE MULTIPLICATIVE STACKING BONUSES BELOW -- HAVE KEYWORD: MORE
		// quest or accessory bonuses	
		// is occult (add demon bane bonus)
		if(flags & DND_DAMAGEFLAG_COUNTSASMAGIC || damage_category == DND_DAMAGECATEGORY_OCCULT)
			InsertCacheFactor(pnum, wepid, dmgid, DND_DEMONBANE_GAIN * (!!IsAccessoryEquipped(tid, DND_ACCESSORY_DEMONBANE)), false);
		
		// these HOPEFULLY dont have anything in common... yet?
		if(IsHandgun(wepid)) // gunslinger affected
			InsertCacheFactor(pnum, wepid, dmgid, DND_QUEST_SLOT2BONUS * IsQuestComplete(tid, QUEST_ONLYPISTOLWEAPONS), false);
		else if(IsBoomstick(wepid)) // shotgun affected
			InsertCacheFactor(pnum, wepid, dmgid, DND_QUEST_SLOT3BONUS * IsQuestComplete(tid, QUEST_NOSHOTGUNS), false);
		else if(IsSuperWeapon(wepid)) // super weapon affected
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
	range = CheckActorInventory(tid, "Cyborg_InstabilityStack");
	if(range == DND_MAXCYBORG_INSTABILITY && IsTechWeapon(wepid))
		temp += DND_DMG_PER_INSTABILITY * range;
	
	// isSpecial isn't used or kept track of below here, so re-use
	// 66% effectiveness of damage scaling on tracer -- dmgid is 1 on tracers
	isSpecial = (IsSuperWeapon(wepid)) && (wepid == DND_WEAPON_BFG32768 || wepid == DND_WEAPON_BFG6000) && dmgid == 1;
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

void HandleIgniteEffects(int pnum, int victim, int wepid, int flags, int dmg_within_tic) {
	bool addedIgn = flags & DND_DAMAGETICFLAG_ADDEDIGNITE;
	if
	(
		CheckAilmentImmunity(pnum, victim - DND_MONSTERTID_BEGIN, DND_SCORCHED) &&
		(addedIgn || CheckIgniteChance(pnum))
	)
	{
		int amt = DND_BASE_IGNITETIMER * (100 + GetPlayerAttributeValue(pnum, INV_IGNITEDURATION) + GetPlayerAttributeValue(pnum, INV_EX_DOTDURATION)) / 100;
		int current_ign_time = CheckActorInventory(victim, "DnD_IgniteTimer");
		int ign_flags = DND_IGNITEFLAG_CANPROLIF;
		if(addedIgn)
			ign_flags |= DND_IGNITEFLAG_ADDEDIGN;

		if(!current_ign_time) {
			SetActorInventory(victim, "DnD_IgniteTimer", amt);
			ACS_NamedExecuteWithResult("DnD Monster Ignite", victim, wepid, ign_flags, dmg_within_tic);
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

// tid, mon_id, weaken %
Script "DnD Occult Weaken" (int victim, int mon_id) {
	SetActivator(victim);
	int time = 0;
	int base_res = MonsterProperties[mon_id].resists[DND_DAMAGECATEGORY_OCCULT];
	while(time < OCCULT_WEAKEN_DURATION) {
		int prev = CheckInventory("OccultWeaknessStack");
		Delay(const:TICRATE);
		++time;
		if(prev != CheckInventory("OccultWeaknessStack") || CheckInventory("OccultWeaknessTimeReset"))
			time = 0;
	}
	SetInventory("OccultWeaknessStack", 0);
	SetResultValue(0);
}

int ApplyPenetrationToDamage(int pnum, int victim, int dmg, int damage_category, int flags, int resist, int pen) {
	// factor is the final resistance the monster will have against the attack
	int factor = (100 - resist + pen);
	if(factor <= 0)
		return 0;
	
	// non-zero, we're good
	return dmg * factor / 100;
}

int FactorResists(int source, int victim, int wepid, int dmg, int damage_type, int actor_flags, int flags, bool forced_full, bool wep_neg = false) {
	// check penetration stuff on source -- set it accordingly to damage type being checked down below
	int mon_id = victim - DND_MONSTERTID_BEGIN;
	int damage_category = GetDamageCategory(damage_type, flags);
	int pnum = PlayerNumber();
	int pen = GetResistPenetration(pnum, damage_category);
	
	// if doomguy perk 50 is there and this is a monster, ignore res
	// added crit ignore res modifier here from below
	forced_full |= (!wep_neg && (actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT) && GetPlayerAttributeValue(pnum, INV_EX_CRITIGNORERESCHANCE) >= random(1, 100));
	
	int resist = MonsterProperties[mon_id].resists[damage_category];
	int temp;
	int pct_val = 0;

	// apply percentage reductions to resist HERE, ABOVE checking the penetration
	// if occult weakness exists, apply it checking monster's debuff -- to be done as a resist reduction to affect all players later
	// we will handle all percentage reductions here deliberately so that we don't mess up the base resist value of the monster!
	if(damage_category == DND_DAMAGECATEGORY_OCCULT || damage_category == DND_DAMAGECATEGORY_SOUL || (flags & DND_DAMAGEFLAG_SOULATTACK)) {
		temp = GetPlayerAttributeValue(pnum, INV_ESS_ZRAVOG);
		if(temp) {
			if(!CheckActorInventory(victim, "OccultWeaknessStack")) {
				GiveActorInventory(victim, "OccultWeaknessStack", 1);
				ACS_NamedExecuteWithResult("DnD Occult Weaken", victim, mon_id);
			}
			else {
				// latter forces the reset on debuff timer
				GiveActorInventory(victim, "OccultWeaknessStack", 1);
				GiveActorInventory(victim, "OccultWeaknessTimeReset", 1);
			}
			pct_val += temp * CheckActorInventory(victim, "OccultWeaknessStack");
		}
	}
	else if(damage_category == DND_DAMAGECATEGORY_LIGHTNING) {
		pct_val += DND_THUNDERAXE_WEAKENPCT * (!!CheckActorInventory(victim, "ThunderAxeWeakenTimer"));
	}

	if(IsBoomstick(wepid) && CheckInventory("Hobo_Perk25"))
		pct_val += DND_HOBO_RESISTPCT;
	
	if(CheckActorInventory(victim, "Doomguy_ResistReduced"))
		pct_val += DND_DOOMGUY_RESISTPCT;

	// apply pct reduction
	if(pct_val) {
		if(pct_val > 100)
			pct_val = 100;
		resist = resist * (100 - pct_val) / 100;
	}

	// debuffs to reduce flat
	resist -= CountMonsterAilments(victim) * DND_WANDERER_RESREDUCE;

	//printbold(s:"new res ", d:resist);

	// if we do full dmg, either do dmg as is or check for pen overpowering the resist, so we can go ahead and do extra damage
	if(forced_full) {
		if(pen - resist > 0)
			return ApplyPenetrationToDamage(pnum, victim, dmg, damage_category, flags, resist, pen);
		return dmg;
	}
	
	// no full dmg, so we just factor resist in as usual
	return ApplyPenetrationToDamage(pnum, victim, dmg, damage_category, flags, resist, pen);
}

// for player hitting others damage
int HandlePlayerBuffs(int p_tid, int enemy_tid, int damage_type, int wepid, int flags) {
	int more_bonus = 100;
	if(!IsOccultDamage(damage_type) && IsAccessoryEquipped(p_tid, DND_ACCESSORY_DEMONBANE))
		more_bonus = more_bonus * (100 - DND_DEMONBANE_REDUCE) / 100;
		
	// ghost enemies take more damage if nether mask is equipped
	if(CheckFlag(enemy_tid, "GHOST") && IsAccessoryEquipped(p_tid, DND_ACCESSORY_NETHERMASK))
		more_bonus = more_bonus * (100 + DND_NETHERGHOST_BONUS) / 100;
		
	// amps fire damage, reduces ice damage
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_AMULETHELLFIRE)) {
		// we handle ignite damage buff in the dot calculation
		if(IsFireDamage(damage_type) && !(flags & DND_DAMAGETICFLAG_NOIGNITESTACK))
			more_bonus = more_bonus * (100 + DND_AMULETHELL_DAMAGE) / 100;
		else if(IsIceDamage(damage_type))
			more_bonus = more_bonus * (100 - DND_AMULETHELL_DAMAGE) / 100;
	}
	
	if(!(IsMeleeDamage(damage_type) || (flags & DND_DAMAGETICFLAG_CONSIDERMELEE)) && IsAccessoryEquipped(p_tid, DND_ACCESSORY_HATESHARD))
		more_bonus = more_bonus * (100 - DND_HATESHARD_REDUCTION) / 100;
	
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_HANDARTEMIS) && wepid >= 0) {
		if(IsSuperWeapon(wepid))
			more_bonus = more_bonus * (100 - DND_ARTEMIS_REDUCE_SUPER) / 100;
		else
			more_bonus = more_bonus * (100 - DND_ARTEMIS_REDUCE) / 100;
	}
		
	// if there's poison stack that means it's a regular poison attack, but if there's not that means its a poison dot, do not include it again for that
	if(CheckInventory("AgamottoOffense") && !(flags & DND_DAMAGEFLAG_NOPOISONSTACK))
		more_bonus = more_bonus * (100 + DND_AGAMOTTO_OFFENSE) / 100;
	
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_LICHARM)) {
		if(damage_type == DND_DAMAGETYPE_SOUL || (flags & DND_DAMAGETICFLAG_SOULATTACK))
			more_bonus = more_bonus * (100 + DND_LICHARM_BUFF) / 100;
		else
			more_bonus = more_bonus * (100 - DND_LICHARM_NERF) / 100;
	}

	if
	(
		(IsFireDamage(damage_type) && CheckInventory("ElementPower_Fire")) ||
		(IsIceDamage(damage_type) && CheckInventory("ElementPower_Ice")) ||
		(IsLightningDamage(damage_type) && CheckInventory("ElementPower_Lightning")) ||
		(IsPoisonDamage(damage_type) && CheckInventory("ElementPower_Earth"))
	)
	{
		more_bonus = more_bonus * (100 + DND_SIGIL_BUFF) / 100;
	}
	else if(IsElementalDamageType(damage_type) && IsAccessoryEquipped(p_tid, DND_ACCESSORY_SIGILELEMENTS))
		more_bonus = more_bonus * (100 - DND_SIGIL_NERF) / 100;
	
	// 50% more damage taken
	if(CheckInventory("HateWeakness"))
		more_bonus = more_bonus * (100 + DND_HATESHARD_BUFF) / 100;
	
	return more_bonus;
}

// for others hitting player damage
int HandlePlayerOnHitBuffs(int p_tid, int enemy_tid, int dmg, int dmg_data, str arg2) {
	// take extra damage only if they aren't ghost
	if(IsAccessoryEquipped(p_tid, DND_ACCESSORY_NETHERMASK) && !CheckFlag(enemy_tid, "GHOST"))
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
		
	// 10%
	if(CheckActorInventory(p_tid, "Invulnerable_Better"))
		dmg /= DND_INVULSPHERE_FACTOR;
	
	return dmg;
}

// This function is responsible for handling all damage effects player has that affect their damage some way
// ex: curses etc.
int HandleGenericPlayerMoreDamageEffects(int pnum, int wepid) {
	int more_bonus = 100;

	// little orbs he drops
	if(CheckInventory("Doomguy_Perk25_Damage"))
		more_bonus = more_bonus * (100 + DND_DOOMGUY_DMGBONUS) / 100;

	// 25% reduction, so 3 / 4
	if(CheckInventory("FleshWizardWeaken"))
		more_bonus = more_bonus * 3 / 4;
	
	// 70% reduction, so 3 / 10
	if(CheckInventory("PowerLessDamage"))
		more_bonus = more_bonus * 3 / 10;
		
	int temp;
	if(CheckInventory("PlayerIsLeeching") && (temp = GetPlayerAttributeValue(pnum, INV_LIFESTEAL_DAMAGE)))
		more_bonus = more_bonus * (100 + ConvertFixedFactorToInt(temp)) / 100;
		
	if(CheckInventory("CorruptOrb_DamageReduction"))
		more_bonus = more_bonus * (100 - DND_CORRUPTORB_DMGREDUCE) / 100;
	
	temp = CheckInventory("Punisher_Perk50_Counter") / DND_PUNISHER_PERK3_KILLCOUNT;
	if(temp)
		more_bonus = more_bonus * (100 + temp * DND_PUNISHER_DMGINC) / 100;
		
	temp = CheckInventory("Rally_DamageBuff");
	if(temp)
		more_bonus = more_bonus * (100 + RALLY_BASEDAMAGE + (temp - 1) * RALLY_DAMAGEPERLVL) / 100;
		
	// dmg is multiplied by 3/2 = 1.5, 50% more dmg
	if(GetArmorID() == BODYARMOR_RAVAGER && CheckInventory("RavagerPower"))
		more_bonus = more_bonus * (100 + DND_RAVAGER_DMGBONUS) / 100;
		
	// artifact things
	if(CheckInventory("DnD_ArtiDmgPower"))
		more_bonus = more_bonus * (100 + DND_QUEST_NOARTIDMG) / 100;
		
	if(CheckInventory("TripleDamagePower"))
		more_bonus *= 3;
	else if(CheckInventory("TripleDamagePower2"))
		more_bonus = more_bonus * 9 / 2;

	// 30% more effectiveness
	if(wepid >= 0 && CheckInventory("Cyborg_Perk5") && (Weapons_Data[wepid].properties & WPROP_TECH))
		more_bonus = more_bonus * 13 / 10;
	
	return more_bonus;
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
int HandleDamageDeal(int source, int victim, int dmg, int damage_type, int wepid, int flags, int ox, int oy, int oz, int actor_flags, bool wep_neg = false, bool oneTimeRipperHack = false) {
	str s_damagetype = DamageTypeList[damage_type];
	bool forced_full = false;
	bool no_ignite_stack = flags & DND_DAMAGEFLAG_NOIGNITESTACK;
	int temp;
	
	int pnum = source - P_TIDSTART;

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
			return 0;
		}
	}

	if(MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_TEMPORALBUBBLE] && !CheckActorInventory(victim, "TemporalBubbleCooldown")) {
		GiveActorInventory(victim, "TemporalBubbleCooldown", 1);
		PlaySound(victim, "TemporalBubble/Pop", CHAN_7, 1.0);
		ACS_NamedExecuteAlways("DnD Temporal Bubble Ticker", 0, victim, victim - DND_MONSTERTID_BEGIN);
		return 0;
	}

	// check if the damage is to be dealt without any reductions from resistances or immunities
	forced_full |= 	(flags & DND_DAMAGEFLAG_DOFULLDAMAGE)																		||
					((flags & DND_DAMAGEFLAG_ISSPELL) && CheckUniquePropertyOnPlayer(pnum, PUP_SPELLSDOFULL))					||
					(IsOccultDamage(damage_type) && IsQuestComplete(0, QUEST_KILLDREAMINGGOD))									||
					(IsExplosionDamage(damage_type) && CheckUniquePropertyOnPlayer(pnum, PUP_EXPLOSIVEIGNORERESIST))			||
					(damage_type == DND_DAMAGETYPE_SOUL && CheckUniquePropertyOnPlayer(pnum, PUP_SOULWEPSDOFULL));
	
	int extra = 0;
	int poison_factor = 0;
	
	// pain checks
	if(actor_flags & DND_ACTORFLAG_PAINLESS)
		s_damagetype = StrParam(s:s_damagetype, s:"_NoPain");
	else if(actor_flags & DND_ACTORFLAG_FORCEPAIN)
		s_damagetype = StrParam(s:s_damagetype, s:"_ForcePain");
	
	// extra property checks moved here
	// WE CHECK FOR CRITS HERE, EITHER WEAPON OR SPELL! THE FINAL STEP BEFORE RESISTS
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

		// not DOT and we can roll crit
		if
		(
			!(actor_flags & DND_ACTORFLAG_ISDAMAGEOVERTIME) && 
			!(flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME) && 
			((actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT) || GetPlayerWeaponCritState(pnum, wepid) || (!GetPlayerWeaponCritLock(pnum, wepid) && CheckCritChance(pnum, victim, wepid, IsLightningDamage(damage_type))))
		)
		{
			SetPlayerWeaponCritState(pnum, wepid);
			actor_flags |= DND_ACTORFLAG_CONFIRMEDCRIT;
		}

		LockPlayerCritState(pnum, wepid);
	}
	else if(flags & DND_DAMAGEFLAG_ISSPELL) {
		// check if it has any poison factor on the spell
		poison_factor = GetSpellPoisonFactor(wepid);

		if
		(
			!(actor_flags & DND_ACTORFLAG_ISDAMAGEOVERTIME) && 
			!(flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME) && 
			((actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT) || CheckCritChance(pnum, victim, -1, 0, IsLightningDamage(damage_type)))
		)
		{
			actor_flags |= DND_ACTORFLAG_CONFIRMEDCRIT;
		}
	}

	// FINALIZED DAMAGE WILL BE BELOW, AFTER RESISTS!
	//printbold(s:"res calc");
	temp = dmg;
	if(!wep_neg)
		dmg = FactorResists(source, victim, wepid, dmg, damage_type, actor_flags, flags, forced_full, wep_neg);
	
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

	// hit beeps and stuff
	// if more that means we hit a weakness, otherwise below conditions check immune and resist respectively
	extra = 0;
	if(dmg > temp)
		ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_WEAKNESS);
	else if(dmg < temp / 4) {
		extra |= DND_DAMAGETICFLAG_LESSENED;
		ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_IMMUNITY);
	}
	else if(dmg < temp) {
		extra |= DND_DAMAGETICFLAG_LESSENED;
		ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_RESIST);
	}

	// damage number handling - NO MORE DAMAGE FIDDLING FROM BELOW HERE
	// all damage calculations should be done by this point, besides cull --- cull should not reflect on here
	// printbold(s:"apply ", d:dmg, s: " of type ", s:s_damagetype, s: " pnum: ", d:pnum);
	// this part handles damage pushing
	temp = victim - DND_MONSTERTID_BEGIN;
	
	// extra represents the flag list of damageticflag
	extra |= 	(!(flags & DND_DAMAGEFLAG_NOPUSH) * DND_DAMAGETICFLAG_PUSH) 					|
				(!!(actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT) * DND_DAMAGETICFLAG_CRIT)			|
				(!!(actor_flags & DND_ACTORFLAG_COUNTSASMELEE) * DND_DAMAGETICFLAG_CONSIDERMELEE)	|
				(!!(flags & DND_DAMAGEFLAG_ADDEDIGNITE) * DND_DAMAGETICFLAG_ADDEDIGNITE)			|
				(!!(flags & DND_DAMAGEFLAG_EXTRATOUNDEAD) * DND_DAMAGETICFLAG_EXTRATOUNDEAD)		|
				(!!(flags & DND_DAMAGEFLAG_NOPOISONSTACK) * DND_DAMAGETICFLAG_NOPOISONSTACK)		|
				(no_ignite_stack * DND_DAMAGETICFLAG_NOIGNITESTACK)									|
				(!!(flags & DND_DAMAGEFLAG_SOULATTACK) * DND_DAMAGETICFLAG_SOULATTACK)				|
				(!!((actor_flags & DND_ACTORFLAG_ISDAMAGEOVERTIME) || (flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME)) * DND_DAMAGETICFLAG_DOT);
	
	// we send particular damage types in that can cause certain status effects like chill, freeze etc.
	if(damage_type == DND_DAMAGETYPE_ICE)
		extra |= DND_DAMAGETICFLAG_ICE;
	else if(IsDamageCapableOfIgnite(damage_type) && !no_ignite_stack)
		extra |= DND_DAMAGETICFLAG_FIRE;
	else if(damage_type == DND_DAMAGETYPE_LIGHTNING)
		extra |= DND_DAMAGETICFLAG_LIGHTNING;

	if(!PlayerDamageTicData[pnum][temp]) {
		PlayerDamageVector[pnum].x = ox;
		PlayerDamageVector[pnum].y = oy;
		PlayerDamageVector[pnum].z = oz;
		ACS_NamedExecuteWithResult("DnD Damage Accumulate", temp | (extra << DND_DAMAGE_ACCUM_SHIFT), wepid, wep_neg | (oneTimeRipperHack << 1), damage_type);
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
		dmg = GetActorProperty(victim, APROP_HEALTH) * 2;
	}
	
	if(dmg > 0) {
		// give this token early to prevent order of events getting mixed up
		if(GetActorProperty(victim, APROP_HEALTH) <= dmg)
			GiveActorInventory(victim, "MonsterKilledByPlayer", 1);

		if(isActorAlive(victim) && CheckInventory("Doomguy_Perk50") && IsMonsterIdDemon(victim - DND_MONSTERTID_BEGIN))
			GiveActorInventory(victim, "Doomguy_ResistReduced", 1);
	}
	
	// monster or w.e we shot at died
	if(!isActorAlive(victim)) {
		// give this for non-magic seal weapons (seals their souls...)
		if(damage_type != DND_DAMAGETYPE_MAGICSEAL && (IsOccultDamage(damage_type) || (!wep_neg && IsSoulDroppingWeapon(wepid))))
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

	return dmg;
}

Script "DnD Crossbow Explosion" (int this, int target) {
	int exptid = DND_CROSSBOW_EXPLOSIONTID + target - P_TIDSTART;
	SpawnForced("Crossbow_Explosion", GetActorX(this), GetActorY(this), GetActorZ(this) + GetActorProperty(this, APROP_HEIGHT) / 2 + 24.0, exptid);
	SetActivator(exptid);
	SetPointer(AAPTR_TARGET, target);
	Thing_ChangeTID(exptid, 0);
	
	SetResultValue(0);
}

// we check with if statement here now just in case we add more flags in the future, the check below is to ensure we bypass script execution
Script "DnD Adjust Impact Damage" (int flags, int dmg, int owner) {
	//if(flags & DND_DAMAGEFLAG_LOSEDAMAGEPERHIT) {
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
	//}
	SetResultValue(dmg);
}

void HandleRipperHitSound(int tid, int owner, int wepid) {
	switch(wepid) {
		case DND_WEAPON_AXE:
			PlaySound(tid, "Axe/HitBleeding", 5, 1.0);
			SpawnForced("BladeHitFX", GetActorX(tid), GetActorY(tid), GetActorZ(tid));
		break;
		case DND_WEAPON_CROSSBOW:
			PlaySound(tid, "Crossbow/Hit", 5, 1.0);
		break;
		case DND_WEAPON_DARKLANCE:
			// stack building on kill
			GiveActorInventory(owner, "LanceStacks", 1);
		break;
	}
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
	if(GetActorProperty(0, APROP_HEALTH) >= GetSpawnHealth() || !dmg)
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

int HandleNonWeaponDamageScale(int dmg, int damage_category, int flags) {
	int temp, temp2;
	int pnum = PlayerNumber();
	int pct_bonus = 0;
	bool isSpell = flags & DND_WDMG_ISSPELL;

	// spell damage is now stored as raw value in the dmg
	/*if(isSpell) {
		// we take id of spell from the table as input, then read its damage
		temp2 = dmg;
		dmg = SpellDamageTable[dmg].dmg;
	}*/
	
	dmg += (!isSpell) * MapDamageCategoryToFlatBonus(pnum, damage_category);

	if(flags & DMG_WDMG_ESHIELDSCALE)
		dmg += CheckInventory("EShieldAmount") / 20; // 5%
	
	// attribute bonus
	if(!(flags & DND_WDMG_ISDOT)) {
		bool isMelee = damage_category == DND_DAMAGECATEGORY_MELEE || (flags & DND_WDMG_ISMELEE);
		if(isMelee)
			pct_bonus += HandleStatBonus(pnum, DND_STAT_ATTUNEMENT_GAIN, 0, 0, isMelee);
		else if((flags & DND_WDMG_ISOCCULT) || damage_category == DND_DAMAGECATEGORY_OCCULT || isSpell)
			pct_bonus += HandleStatBonus(pnum, 0, 0, DND_STAT_ATTUNEMENT_GAIN, isMelee);
		else
			pct_bonus += HandleStatBonus(pnum, 0, DND_STAT_ATTUNEMENT_GAIN, 0, isMelee);
	}
		
	if((flags & DND_WDMG_ISOCCULT) || damage_category == DND_DAMAGECATEGORY_OCCULT) // is occult (add demon bane bonus)
		dmg = dmg * (100 + DND_DEMONBANE_GAIN * IsAccessoryEquipped(ActivatorTID(), DND_ACCESSORY_DEMONBANE)) / 100;
		
	if(flags & DMG_WDMG_ISARTIFACT)
		dmg = dmg * (100 + DND_ARTIFACT_GAIN * CheckInventory("Ability_Arcanery")) / 100;
	
	temp = GetPlayerAttributeValue(pnum, INV_EX_PHYSDAMAGEPER_FLATHEALTH);
	if((damage_category == DND_DAMAGECATEGORY_MELEE || damage_category == DND_DAMAGECATEGORY_BULLET) && temp)
		pct_bonus += GetFlatHealthDamageFactor(temp);
	
	temp = GetPlayerPercentDamage(pnum, -1, damage_category);
	if(temp/* && !isSpell*/)
		pct_bonus += temp;

	// apply the % bonus now
	dmg = dmg * (100 + pct_bonus) / 100;
		
	// finally crit chance
	// spells will have their own crit source compared to attacks
	/*if(!isSpell && CheckCritChance(-1, false, -1)) {
		dmg = dmg * GetCritModifier(-1) / 100;
		HandleHunterTalisman();
	}*/
	
	// final additions
	if((flags & DMG_WDMG_ISARTIFACT) && IsQuestcomplete(0, QUEST_USENOARTIFACT))
		dmg = dmg * (100 + DND_QUEST_ARTIBONUS) / 100;
	
	/*if(isSpell) {
		if(SpellDamageTable[temp2].dmg_low)
			dmg *= random(SpellDamageTable[temp2].dmg_low, SpellDamageTable[temp2].dmg_high);
	}*/
	
	//printbold(s:"scaled player damage ", d:dmg);
	return dmg;
}

// ASSUMPTION: PLAYER RUNS THIS! -- care if adapting this later for other things
Script "DnD Damage Accumulate" (int victim_data, int wepid, int wep_neg, int damage_type) {
	int pnum = PlayerNumber();
	int flags = victim_data >> DND_DAMAGE_ACCUM_SHIFT;
	victim_data &= DND_MONSTER_TICDATA_BITMASK;

	int ox = PlayerDamageVector[pnum].x;
	int oy = PlayerDamageVector[pnum].y;
	int oz = PlayerDamageVector[pnum].z;

	int victim_tid = victim_data + DND_MONSTERTID_BEGIN;
	int temp;

	Delay(const:1);

	/*
		THINGS THAT ALTER DAMAGE IN ANY WAY AFTER ACCUMULATION END UP HERE!!!!
	*/
	int prev_dmg = PlayerDamageTicData[pnum][victim_data];
	int more_dmg = 100; // baseline damage, 100% is regular value

	// desolator damage increase
	if(damage_type == DND_DAMAGETYPE_DESOLATOR) {
		if(!CheckActorInventory(victim_tid, "DesolatorStackCounter")) {
			GiveActorInventory(victim_tid, "DesolatorStackTimer", 52);
			ACS_NamedExecuteAlways("DND Desolator Debuff FX", 0, victim_tid);
		}
		else
			GiveActorInventory(victim_tid, "DesolatorStackTimer", 17);
		GiveActorInventory(victim_tid, "DesolatorStackCounter", 1);
	}

	// increase damage they take from elemental attacks for each stack
	// poison damage gets sent already scaled, dont scale twice
	if(IsElementalDamageType(damage_type) && !(flags & DND_DAMAGETICFLAG_NOPOISONSTACK)) {
		temp = CheckActorInventory(victim_tid, "DesolatorStackCounter");
		// 10% increase from desolator
		if(temp)
			more_dmg = more_dmg * (100 + temp * DND_DESOLATOR_DMG_GAIN) / 100;
	}

	if((flags & DND_DAMAGETICFLAG_EXTRATOUNDEAD) && MonsterProperties[victim_data].trait_list[DND_SILVER_WEAKNESS])
		more_dmg = more_dmg * (100 + DND_EXTRAUNDEADDMG_MULTIPLIER + IsQuestComplete(0, QUEST_SPAREZOMBIES) * DND_QUEST_UNDEADGAIN) / 100;

	// check blockers take more dmg modifier
	if(MonsterProperties[victim_data].trait_list[DND_ISBLOCKING] && (temp = GetPlayerAttributeValue(pnum, INV_BLOCKERS_MOREDMG)))
		more_dmg = more_dmg * (100 + ConvertFixedFactorToInt(temp)) / 100;
	
	// buff effectiveness is the maximum of what the monster might have had previously from another player vs. most up-to-date, which is overwritten into its DnD_OverloadDamage item
	if(damage_type != DND_DAMAGETYPE_LIGHTNING && CheckActorInventory(victim_tid, "DnD_OverloadTimer"))
		more_dmg = more_dmg * (100 + DND_BASE_OVERLOADBUFF + CheckActorInventory(victim_tid, "DnD_OverloadDamage")) / 100;
	
	if(IsMonsterIdBoss(MonsterProperties[victim_data].id) && HasPlayerPowerSet(pnum, PPOWER_BOSSTAKEMOREDMG))
		more_dmg = more_dmg * (100 + PREDATOR_DMG_BONUS) / 100;

	// additional damage vs frozen enemies modifier
	if(CheckActorInventory(victim_tid, "DnD_FreezeTimer") && (temp = GetPlayerAttributeValue(pnum, INV_ESS_ERYXIA)))
		more_dmg = more_dmg * (100 + ConvertFixedFactorToInt(temp)) / 100;
		
	// 50% more damage taken, so dmg * 3 / 2
	if(CheckActorInventory(victim_tid, "DemonSealResistDebuff"))
		more_dmg = more_dmg * (100 + DEMONSEAL_DMGTAKEN_DEBUFF) / 100;

	// General buff effects, includes curses and stuff too
	more_dmg = more_dmg * HandleGenericPlayerMoreDamageEffects(pnum, wepid) / 100;
	
	// ACCESSORY EFFECTS
	more_dmg = more_dmg * HandlePlayerBuffs(pnum + P_TIDSTART, victim_tid, damage_type, wepid, flags) / 100;

	// check hobo's level 50 perk here, after 1 tic, and deal the extra damage with "_NoPain" attached
	// this is the most efficient way to handle this bonus as then we won't be calculating the distance check PER PELLET!!!
	// plus we get to adjust the push factor and other things before they affect the monster proper here
	bool isHoboPowerApplicable = wepid >= 0 && IsBoomstick(wepid) && CheckInventory("Hobo_Perk50");
	if(isHoboPowerApplicable && CheckInventory("Hobo_ShotgunFrenzyTimer")) {
		temp = fdistance_delta(ox - GetActorX(victim_tid), oy - GetActorY(victim_tid), oz - GetActorZ(victim_tid));
		temp -= FixedMul(GetActorProperty(victim_tid, APROP_RADIUS) + DND_PLAYER_RADIUS, 1.207);

		// give leeway for max scale window
		if(temp < DND_HOBO_SHOTGUNMINBESTDIST)
			temp = DND_HOBO_SHOTGUNMINBESTDIST;
		temp >>= 16;

		if(temp <= DND_HOBO_SHOTGUNFALLOFFDIST) {
			temp = LinearMap(temp, DND_HOBO_SHOTGUNMINBESTDIST_INT, DND_HOBO_SHOTGUNFALLOFFDIST, 0, 100);
			// 100 + (100 - temp) would mean 200 - temp, and we scale inversely with distance so if we are farthest, we will be getting 100 to be dealing the same amount of damage anyway
			more_dmg = more_dmg * (100 + DND_HOBO_SHOTGUNDISTMOREDMG - temp) / 100;
		}
	}

	// moved crit at the end here -- copied code to save from 1 extra if check to see if more_dmg or crit is non-zero
	if(flags & DND_DAMAGETICFLAG_CRIT) {
		if(more_dmg != 100)
			PlayerDamageTicData[pnum][victim_data] = ApplyDamageFactor_Safe(PlayerDamageTicData[pnum][victim_data], more_dmg);

		// amplify the overall damage as a crit here -- wepid negativity check happens inside np
		more_dmg = GetCritModifier(pnum, victim_tid, wepid);

		PlayerDamageTicData[pnum][victim_data] = ApplyDamageFactor_Safe(PlayerDamageTicData[pnum][victim_data], more_dmg);

		HandleHunterTalisman();
	}
	else if(more_dmg != 100)
		PlayerDamageTicData[pnum][victim_data] = ApplyDamageFactor_Safe(PlayerDamageTicData[pnum][victim_data], more_dmg);

	//printbold(s:"before ", d:prev_dmg, s: " new dmg: ", d:PlayerDamageTicData[pnum][victim_data], s: " ", d:more_dmg);

	// deal the damage difference between the crit and original on top, like hobo thing -- note use of Special_NoPain
	if(PlayerDamageTicData[pnum][victim_data] > prev_dmg)
		Thing_Damage2(victim_tid, PlayerDamageTicData[pnum][victim_data] - prev_dmg, "Special_NoPain");
	else if(IsActorAlive(victim_tid) && PlayerDamageTicData[pnum][victim_data] != prev_dmg) // we have reduced the overall damage instead, heal for the difference instead -- hope we dont need HealThing here...
		SetActorProperty(victim_tid, APROP_HEALTH, GetactorProperty(victim_tid, APROP_HEALTH) + prev_dmg - PlayerDamageTicData[pnum][victim_data]);

	/*
		DMG ALTERING ENDS BY HERE! NO MORE! FINALIZED!
	*/

	// moved here as it's simpler and more efficient to run this function after 1 tic rather than immediately with multiple instances
	IncrementStatistic(DND_STATISTIC_DAMAGEDEALT, PlayerDamageTicData[pnum][victim_data], pnum + P_TIDSTART);

	// do the real pushing after 1 tic of dmg data has been accumulated and we have non-zero damage in effect
	// wep_neg here contains 2 bits: was it negative at 1st bit and was it a one time ripper in 2nd bit
	if((flags & DND_DAMAGETICFLAG_PUSH) && PlayerDamageTicData[pnum][victim_data] > 0)
		HandleDamagePush(2 * PlayerDamageTicData[pnum][victim_data], ox, oy, oz, victim_tid, wep_neg & 2);
	
	// has wepid non neg
	if(!(wep_neg & 1)) {
		// check if player has lifesteal, if they do reward some hp back
		if(!MonsterProperties[victim_data].trait_list[DND_BLOODLESS] && !(flags & DND_DAMAGETICFLAG_DOT))
			HandleLifesteal(pnum, wepid, flags, PlayerDamageTicData[pnum][victim_data]);
	}
	
	// if ice damage, add stacks of slow and check for potential freeze chance
	// do these if only the actor was alive after the tic they received dmg
	if(IsActorAlive(victim_tid)) {
		if(flags & DND_DAMAGETICFLAG_ICE)
			HandleChillEffects(pnum, victim_tid);
		else if((flags & DND_DAMAGETICFLAG_FIRE) || (flags & DND_DAMAGETICFLAG_ADDEDIGNITE)) // should be able to ign if it has addedignite flag even if damagetype isnt fire!
			HandleIgniteEffects(pnum, victim_tid, wepid, flags, GetPlayerIgniteAddedDmg(pnum, wepid, PlayerDamageTicData[pnum][victim_data]));
		else if(flags & DND_DAMAGETICFLAG_LIGHTNING)
			HandleOverloadEffects(pnum, victim_tid);
		
		// frozen monsters cant retaliate	
		if(!CheckActorInventory(ox, "DnD_FreezeTimer")) {
			if(MonsterProperties[victim_data].trait_list[DND_VIOLENTRETALIATION] && random(1, 100) <= DND_VIOLENTRETALIATION_CHANCE)
				GiveActorInventory(victim_tid, "DnD_ViolentRetaliationItem", 1);
			if(MonsterProperties[victim_data].trait_list[DND_THUNDERSTRUCK] && !CheckInventory("ThunderstruckCooldown")) {
				ACS_NamedExecuteAlways("DnD Thunderstruck", 0, victim_tid);
				GiveInventory("ThunderstruckCooldown", 1);
			}
		}
		GiveActorInventory(victim_tid, "DnD_HurtToken", 1);

		// actor is alive, we can tag with shotgun for hobo perk 50
		if(isHoboPowerApplicable && !CheckInventory("Hobo_ShotgunFrenzyTimer")) {
			// if the window passed, ignore remaining tags
			if(!CheckActorInventory(victim_tid, "Hobo_ShotgunTag_Window"))
				SetActorInventory(victim_tid, "Hobo_ShotgunTag", 0);
			GiveActorInventory(victim_tid, "Hobo_ShotgunTag_Window", 1);
			GiveActorInventory(victim_tid, "Hobo_ShotgunTag", 1);

			if(CheckActorInventory(victim_tid, "Hobo_ShotgunTag") >= DND_HOBO_SHOTGUNTAGLIMIT) {
				SetActorInventory(victim_tid, "Hobo_ShotgunTag", 0);
				GiveInventory("Hobo_ShotgunFrenzyTimer", DND_HOBO_FRENZYBASETIME);
				CalculatePlayerAccuracy(pnum);
				ACS_NamedExecuteAlways("DnD Hobo Frenzy Timer", 0);
			}
		}
	}

	ACS_NamedExecuteWithResult("DnD Damage Numbers", victim_tid, PlayerDamageTicData[pnum][victim_data], flags);

	if(CheckInventory("Marine_DamageReduction_Timer"))
		GiveInventory("Marine_Perk50_DamageDealt", PlayerDamageTicData[pnum][victim_data]);
	
	// reset dmg counter on this mob
	PlayerDamageTicData[pnum][victim_data] = 0;

	Delay(const:CRIT_CLEAR_WAIT_TIME);
	UnsetPlayerWeaponCritState(pnum, wepid);
	UnlockPlayerCritState(pnum, wepid);
	
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
	else if(flags & DND_DAMAGETICFLAG_LESSENED) {
		// apply the gray translation
		Thing_SetTranslation(DND_DAMAGENUMBER_TID, DND_RESIST_TRANSLATION);
	}
	
	Thing_ChangeTID(DND_DAMAGENUMBER_TID, 0);
	
	SetResultValue(0);
}

Script "DnD Do Poison Damage" (int victim, int dmg, int wepid) {
	int pnum = PlayerNumber();
	int source = pnum + P_TIDSTART;
	int time_limit = DND_BASE_POISON_TIMER * (100 + GetPlayerAttributeValue(pnum, INV_POISON_DURATION) + GetPlayerAttributeValue(pnum, INV_EX_DOTDURATION)) / 100;
	int trigger_tic = GetPoisonTicrate(pnum);
	
	int tic_temp = trigger_tic;
	int counter = 0;
	int temp;
	
	// divide trigger tic count by half to make it twice as fast -- if poison ticrate is 100% reduction we'll do poison damage at every 2 tics, which is the most one would need
	if(CheckUniquePropertyOnPlayer(pnum, PUP_POISONTICSTWICE))
		trigger_tic /= 2;

	dmg = GetPoisonDOTDamage(pnum, dmg);

	if(CheckActorInventory(source, "Wanderer_Perk25"))
		AddMonsterAilment(victim, DND_AILMENT_POISON);
		
	while(counter < time_limit && IsActorAlive(victim)) {
		if(counter >= trigger_tic) {
			temp = HandleDamageDeal(source, victim, dmg, DND_DAMAGETYPE_POISON, wepid, DND_DAMAGEFLAG_NOPOISONSTACK | DND_DAMAGEFLAG_NOPUSH, 0, 0, 0, DND_ACTORFLAG_PAINLESS | DND_ACTORFLAG_FOILINVUL | DND_ACTORFLAG_ISDAMAGEOVERTIME, wepid < 0);
			if(temp > 0)
				Thing_Damage2(victim, temp, "Special_NoPain");
			ACS_NamedExecuteAlways("DnD Spawn Poison FX", 0, victim, CheckActorInventory(victim, "DnD_PoisonStacks"));
			
			// go up to the next threshold for next tic etc.
			trigger_tic += tic_temp;
		}
		counter += DND_POISON_CHECKRATE;
		Delay(const:DND_POISON_TICCHECK);
	}

	if(CheckActorInventory(source, "Wanderer_Perk25"))
		RemoveMonsterAilment(victim, DND_AILMENT_POISON);

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

	if(CheckInventory("Wanderer_Perk25"))
		AddMonsterAilment(victim, DND_AILMENT_CHILL);
	
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

	if(CheckInventory("Wanderer_Perk25"))
		RemoveMonsterAilment(victim, DND_AILMENT_CHILL);
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

	if(CheckInventory("Wanderer_Perk25"))
		AddMonsterAilment(victim, DND_AILMENT_FREEZE);
	
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

	if(CheckInventory("Wanderer_Perk25"))
		RemoveMonsterAilment(victim, DND_AILMENT_FREEZE);
	
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

Script "DnD Monster Ignite" (int victim, int wepid, int ign_flags, int added_dmg) {
	int pnum = PlayerNumber();
	int source = pnum + P_TIDSTART;

	// if no added dmg, reset it
	if(!(ign_flags & DND_IGNITEFLAG_ADDEDIGN))
		added_dmg = 0;

	int dmg = GetFireDOTDamage(pnum, added_dmg);
	int dmg_tic_buff = GetPlayerAttributeValue(pnum, INV_ESS_CHEGOVAX);
	
	dmg = HandleNonWeaponDamageScale(dmg, DND_DAMAGECATEGORY_FIRE, DND_WDMG_ISDOT);
	
	int next_dmg = dmg;
	int inc_by = dmg * dmg_tic_buff / 100;
	int i;
	
	// this is the value we will use to set the ignite timers on proliferated targets, if any
	int ign_time = CheckActorInventory(victim, "DnD_IgniteTimer");

	if(CheckActorInventory(source, "Wanderer_Perk25"))
		AddMonsterAilment(victim, DND_AILMENT_IGNITE);
	
	do {
		ACS_NamedExecuteAlways("DnD Monster Ignite FX", 0, victim);
		TakeActorInventory(victim, "DnD_IgniteTimer", 1);
		i = HandleDamageDeal(source, victim, next_dmg, DND_DAMAGETYPE_FIRE, wepid, DND_DAMAGEFLAG_NOIGNITESTACK | DND_DAMAGEFLAG_NOPUSH, 0, 0, 0, DND_ACTORFLAG_ISDAMAGEOVERTIME);
		if(i > 0)
			Thing_Damage2(victim, i, "SkipHandle");
		
		// add base damage's value, not previous
		next_dmg += inc_by;
		
		// x 5
		Delay(const:7);
	} while(CheckActorInventory(victim, "DnD_IgniteTimer") && IsActorAlive(victim));
	
	// find N closest targets to victim for igniting
	//printbold(d:canProlif, s: " ", d:!IsActorAlive(victim), s: " ", d:CheckIgniteProlifChance(pnum));
	if((ign_flags & DND_IGNITEFLAG_CANPROLIF) && !IsActorAlive(victim) && CheckIgniteProlifChance(pnum)) {
		// Moved here, makes more sense to only check if applicable...
		// check ignite prolif
		int owner = P_TIDSTART + pnum;
		int prolif_dist = GetIgniteProlifRange(pnum);
		int prolif_count = GetIgniteProlifCount(pnum);
		
		// clear ignite prolif from subsequent ignites from this monster jumping, we don't want that, too laggy
		ign_flags ^= DND_IGNITEFLAG_CANPROLIF;
		next_dmg = 0; // used as temp variable
		inc_by = 0; // same as above
		dmg_tic_buff = 0; // same as above...
		
		static dist_tid_pair_T tlist[MAXPLAYERS][DND_MAX_IGNITEPROLIFS];
		
		// init list
		for(i = 0; i < DND_MAX_IGNITEPROLIFS; ++i) {
			tlist[pnum][i].tid = 0;
			tlist[pnum][i].dist = prolif_dist;
		}

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
						if(inc_by == prolif_count)
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
			for(i = 0, j = 0; i < prolif_count; ++i) {
				if(tlist[pnum][i].tid) {
					//printbold(s:"prolif to ", d:tlist[pnum][i].tid);
					// check if target was ignited already, if not ignite if so replace timer
					next_dmg = CheckActorInventory(tlist[pnum][i].tid, "DnD_IgniteTimer");
					if(!next_dmg) {
						SetActorInventory(tlist[pnum][i].tid, "DnD_IgniteTimer", ign_time);
						
						// we don't proliferate from the proliferated targets... that'd be busted
						// note: WAIT AND SEE IF ITS OP!
						ACS_NamedExecuteWithResult("DnD Monster Ignite", tlist[pnum][i].tid, wepid, ign_flags, added_dmg);
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

	if(CheckActorInventory(source, "Wanderer_Perk25"))
		RemoveMonsterAilment(victim, DND_AILMENT_IGNITE);
	
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
	int source = ActivatorTID();
	// we dont have any player involvement here so
	SetActivator(victim);
	
	PlaySound(0, "Overload/Loop", CHAN_ITEM, 1.0, true);

	if(CheckActorInventory(source, "Wanderer_Perk25"))
		AddMonsterAilment(victim, DND_AILMENT_OVERLOAD);
	
	while(CheckInventory("DnD_OverloadTimer")) {
		if(!ActivatorTID())
			Terminate;
	
		ACS_NamedExecuteWithResult("DnD Monster Overload Particles");
		TakeInventory("DnD_OverloadTimer", 1);
		Delay(const:DND_BASE_OVERLOADTICK);
		GiveInventory("Overload_SoundStopper", 1);
	}

	if(CheckActorInventory(source, "Wanderer_Perk25"))
		RemoveMonsterAilment(victim, DND_AILMENT_OVERLOAD);

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

		// check rekindled sparks
		res = GetPlayerAttributeValue(pnum, INV_EX_SECONDEXPBONUS);
		if(res) {
			res += 100;

			// we embed a damage factor into this, so we can scale that and reput it
			int temp = GetUserVariable(0, "user_expdmg");
			int factor = temp >> DPCT_SHIFT;
			temp &= BITMASK_NOFACTOR;

			factor = factor * res / 100;

			SetUserVariable(0, "user_expdmg", temp | (factor << DPCT_SHIFT));
			SetUserVariable(0, "user_expradius", GetUserVariable(0, "user_expradius") * res / 100);
			SetUserVariable(0, "user_fullexpradius", GetUserVariable(0, "user_fullexpradius") * res / 100);

			SetActorProperty(0, APROP_SCALEX, GetActorProperty(0, APROP_SCALEX) * res / 100);
			SetActorProperty(0, APROP_SCALEY, GetActorProperty(0, APROP_SCALEY) * res / 100);
		}

		res = 1;
	}
	
	SetResultValue(res);
}

int HandlePlayerSelfDamage(int pnum, int dmg, int dmg_type, int wepid, int flags, bool isArmorPiercing) {
	dmg = dmg * ((GetSelfExplosiveResist(pnum) * 100) >> 16) / 100;

	if(HasPlayerPowerset(pnum, PPOWER_REDUCEDSELFDMG))
		dmg = dmg * (100 - REDUCED_SELF_DMG_FACTOR) / 100;
	
	// apply accessory and other sources of damage -- convert to dmg tic flag due to the recent rewrite
	int tflag = (!!(flags & DND_DAMAGEFLAG_ADDEDIGNITE) * DND_DAMAGETICFLAG_ADDEDIGNITE)			|
				(!!(flags & DND_DAMAGEFLAG_EXTRATOUNDEAD) * DND_DAMAGETICFLAG_EXTRATOUNDEAD)		|
				(!!(flags & DND_DAMAGEFLAG_NOPOISONSTACK) * DND_DAMAGETICFLAG_NOPOISONSTACK)		|
				(!!(flags & DND_DAMAGEFLAG_NOIGNITESTACK) * DND_DAMAGETICFLAG_NOIGNITESTACK)		|
				(!!(flags & DND_DAMAGEFLAG_SOULATTACK) * DND_DAMAGETICFLAG_SOULATTACK)				|
				(!!(flags & DND_DAMAGEFLAG_ISDAMAGEOVERTIME) * DND_DAMAGETICFLAG_DOT);
	int amp = HandlePlayerBuffs(pnum + P_TIDSTART, pnum + P_TIDSTART, dmg_type, wepid, tflag);
	if(amp != 100)
		dmg = ApplyDamageFactor_Safe(dmg, 100 + amp);

	// factor in players armor here!!! -- NO DON'T DO THAT! We have a generic resist and armor handle in main dmg script
	//dmg = HandlePlayerArmor(pnum, dmg, "null", DND_DAMAGETYPEFLAG_EXPLOSIVE, isArmorPiercing);
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
int HandlePlayerResists(int pnum, int dmg, str dmg_string, int dmg_data, bool isReflected, str inflictor_class) {
	int temp = 0;
	int dot_temp;

	bool isDot = IsDamageStringDOT(dmg_string) || (dmg_data & DND_DAMAGETYPEFLAG_DOT);
	
	//if(isHardcore())
	//	dmg = ApplyDamageFactor_Safe(dmg, 100 + DND_HARDCORE_DEBUFF);
	
	dmg = HandleCursePlayerResistEffects(dmg);
	
	// reflection becomes its own thing not affected by other damage type functions, so we can immediately return here
	if(isReflected)
		return ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_REFL);
	
	if(dmg_data & DND_DAMAGETYPEFLAG_PHYSICAL)
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_PHYS);

	if(dmg_data & DND_DAMAGETYPEFLAG_HITSCAN)
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_HITSCAN);
	
	if(dmg_data & DND_DAMAGETYPEFLAG_MAGICAL) {
		temp = HasPlayerPowerset(pnum, PPOWER_INCMAGICRES) * RESIST_BOOST_FROM_BOOTS;
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_MAGIC, temp);
	}
	
	// ELEMENTAL DAMAGE BLOCK BEGINS
	// we can only have 1 element attributed to one damage type at a time
	if(dmg_data & DND_DAMAGETYPEFLAG_FIRE) {
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_FIRE, GetPlayerAttributeValue(pnum, INV_DMGREDUCE_ELEM));

		if(HasPlayerPowerset(pnum, PPOWER_REDUCEDFIRETAKEN))
			dmg = dmg * (100 - DMGREDUCE_BOOST_FROM_BOOTS) / 100;
	}
	else if(dmg_data & DND_DAMAGETYPEFLAG_ICE)
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_ICE, GetPlayerAttributeValue(pnum, INV_DMGREDUCE_ELEM));
	else if(dmg_data & DND_DAMAGETYPEFLAG_LIGHTNING) {
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_LIGHTNING, GetPlayerAttributeValue(pnum, INV_DMGREDUCE_ELEM));

		if(HasPlayerPowerset(pnum, PPOWER_REDUCEDLIGHTNINGTAKEN))
			dmg = dmg * (100 - DMGREDUCE_BOOST_FROM_BOOTS) / 100;
	}
	else if((dmg_data & DND_DAMAGETYPEFLAG_POISON) || dmg_string == "PoisonDOT") {
		// PoisonDOT directly deals damage through the monster, so it can't have its "stamina" / dmg_data set
		// wanderer perk
		if(CheckInventory("Wanderer_Perk5"))
			dmg = ApplyDamageFactor_Safe(dmg, 100 - DND_WANDERER_POISONPERCENT);

		if(HasPlayerPowerset(pnum, PPOWER_REDUCEDPOISONTAKEN))
			dmg = dmg * (100 - DMGREDUCE_BOOST_FROM_BOOTS) / 100;
		
		// reduced poison damage taken
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_POISON, GetPlayerAttributeValue(pnum, INV_DMGREDUCE_ELEM));

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
			RegisterDoTDamage(
				random(dot_temp, (dot_temp * 6) / 5), 
				random(DND_MONSTER_POISONDOT_MINTIME, DND_MONSTER_POISONDOT_MAXTIME),
				DND_DAMAGETYPEFLAG_POISON, 
				inflictor_class
			);
		}
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
		temp = HasPlayerPowerset(pnum, PPOWER_INCENERGYRES) * RESIST_BOOST_FROM_BOOTS;
		dmg = ApplyPlayerResist(pnum, dmg, INV_DMGREDUCE_ENERGY, temp);
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

	// marine perk 50 -- 50% reduction
	if(CheckInventory("Marine_DamageReduction_Timer"))
		dmg /= 2;

	// overheat unique charm
	temp = GetPlayerAttributeValue(pnum, INV_EX_LESSDMGTAKENMAXOVERHEAT);
	if(temp && HasRunningOverheatCooldown(pnum + P_TIDSTART)) {
		dmg = dmg * (100 - temp) / 100;
	}
	
	// ALL DAMAGE AMPLIFYING EFFECTS COME LAST!
	temp = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_TAKEN);
	if(temp)
		dmg = ApplyDamageFactor_Safe(dmg, 100 + temp);

	// find player's lowest resist
	dot_temp = GetPlayerAttributeValue(pnum, INV_EX_DAMAGELOWESTTAKENASPHYS);
	if(dot_temp && !isDot && (GetLowestResist(pnum) & dmg_data)) {
		// create new dot instance of phys damage to player making sure to only get a portion of it as DoT
		dot_temp = dmg * dot_temp / 100;
		if(!dot_temp)
			dot_temp = 1;

		// this is the "instead" part of the "DoT", the rest
		dmg -= dot_temp;
		if(dmg < 1)
			dmg = 1;

		RegisterDoTDamage(dot_temp, 5, DND_DAMAGETYPEFLAG_PHYSICAL, inflictor_class);
	}
	
	return dmg;
}

int GetArmorRatingEffect(int dmg, int armor_id, int dmg_data, bool isArmorPiercing) {
	int pnum = PlayerNumber();
	int rating = GetPlayerArmor(pnum);

	if(CheckInventory("RuinationHardDebuff"))
		rating /= 4;
	else {
		pnum = CheckInventory("RuinationStacks");
		if(pnum)
			rating -= rating * pnum * DND_RUINATION_REDUCE_PER_STACK / 100;
	}

	if(dmg_data & DND_DAMAGETYPEFLAG_MAGICAL)
		rating /= 5;

	// rating is treated as 40% instead of 100% if monster is armor piercing
	if(isArmorPiercing)
		rating = rating * 2 / 5;

	return DoArmorRatingEffect(dmg, rating);
}

bool IsDamageStringDOT(str s) {
	return s == "PoisonDOT" || s == "PhysicalDOT";
}

int HandlePlayerArmor(int pnum, int dmg, str dmg_string, int dmg_data, bool isArmorPiercing) {
	int armor_id = GetArmorID();
	bool is_dot = IsDamageStringDOT(dmg_string);
	int factor = 0.0;

	// DoT is not negated by armor
	if(armor_id != -1 && !is_dot) {
		// retrieve and convert factor to an integer, we convert ex: 0.417 to 417, we will apply damage factor safe method
		// dmg here is the one to be dealt to the player's health pool
		factor = 0.0;

		// apply armor effect on this damage
		dmg = GetArmorRatingEffect(dmg, armor_id, dmg_data, isArmorPiercing);
		
		// special armor cases: Knight gives more reduction if using melee weapon, Duelist negates all hitscan 100% at cost of armor
		if(armor_id == BODYARMOR_KNIGHT && IsUsingMeleeWeapon())
			factor += DND_KNIGHTARMOR_MELEEWEP_BONUS;
		else if(armor_id == BODYARMOR_DUELIST && (dmg_data & DND_DAMAGETYPEFLAG_HITSCAN))
			factor = 0.75;
		
		factor *= ARMOR_INTEGER_FACTOR;
		factor >>= 16;
		
		// armor reduced factor amount of damage, this is what the player will take as damage
		dmg = ApplyDamageFactor_Safe(dmg, ARMOR_INTEGER_FACTOR - factor, ARMOR_INTEGER_FACTOR);
		
		// if we have ravager armor and on killing spree, reduce damage to 17/20 (15% reduced)
		if(armor_id == BODYARMOR_RAVAGER && CheckInventory("RavagerPower"))
			dmg = ApplyDamageFactor_Safe(dmg, DND_RAVAGER_FACTOR, DND_RAVAGER_REDUCE);
		else if(armor_id == BODYARMOR_KNIGHT && dmg_string == "Melee") // apply special reductions offered by certain armors
			dmg = ApplyDamageFactor_Safe(dmg, 100 - DND_KNIGHT_MELEEREDUCE);
	}

	// mitigation -- poison goes through as well
	int temp;
	if(!is_dot && CouldMitigateDamage(pnum)) {
		temp = GetMitigationEffect(pnum);
		dmg = dmg * ((100.0 - temp) >> 16) / 100;
		LocalAmbientSound("Mitigation/Success", 96);
	}
	
	return dmg;
}

// energy shield reduction
int ApplyPlayerEnergyShield(int pnum, int dmg, str dmg_string, int dmg_data) {
	int temp = CheckInventory("EShieldAmount");
	int factor = GetEShieldMagicAbsorbValue(pnum);
	bool is_dot = IsDamageStringDOT(dmg_string);
	int armor_id = GetArmorID();
	int to_take = 0;
	if(temp) {
		// this isn't DOT or magical attack and we have energy shield, so we can deduct damage from it
		if(is_dot || (dmg_data & DND_DAMAGETYPEFLAG_MAGICAL)) {
			// no ways to prevent this type of damage, return raw dmg
			if(!factor)
				return dmg;
			/*if(!HasPlayerPowerset(pnum, PPOWER_ESHIELDBLOCKALL) && !factor)
				return dmg;
			factor += !!(HasPlayerPowerset(pnum, PPOWER_ESHIELDBLOCKALL)) * 100;*/
		}
		else
			factor = 100;

		// lightning coil absorbs 80% by itself, so 20% of the damage will go through
		if((dmg_data & DND_DAMAGETYPEFLAG_LIGHTNING) && armor_id == BODYARMOR_LIGHTNINGCOIL)
			factor += LIGHTNINGCOIL_ABSORBFACTOR;

		// force clamp
		if(factor > 100)
			factor = 100;

		// only block this much if this is on
		to_take = GetPlayerAttributeValue(pnum, INV_EX_ESHIELDONLYBLOCKPCT);
		if(to_take)
			factor = to_take;

		// only this much is prevented
		to_take = dmg * factor / 100;
		if(to_take < 1)
			to_take = 1;
		dmg -= to_take;

		// completely absorbed by our shield, so just reduce our shield amount
		if(dmg < 0) {
			TakeEnergyShield(to_take + dmg);
			dmg = 0;
			LocalAmbientSound("EShield/Hit", 127);
		}
		else if(to_take < temp) {
			TakeEnergyShield(to_take);
			LocalAmbientSound("EShield/Hit", 127);
		}
		else {
			SetEnergyShield(0);
			LocalAmbientSound("EShield/Break", 127);

			temp = GetPlayerAttributeValue(pnum, INV_EX_STARTESONDEPLETE);
			if(temp && random(1, 100) <= temp && (to_take = CanRegenEShield(pnum))) {
				GiveInventory("EShieldChargeNow", 1);
				ACS_NamedExecuteAlways("DnD Energy Shield Regen", 0, to_take, pnum);
			}
		}
	}
	return dmg;
}

void HandleMonsterDamageModChecks(int m_id, int monster_tid, int victim, int dmg) {
	// vampirism check
	if(MonsterProperties[m_id].trait_list[DND_VAMPIRISM] && isActorAlive(monster_tid)) {
		// if this monster is trying to leech off of a bloodless monster, do not allow (we cant have all rules be against players... right?)
		if(IsMonster(victim) && MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_BLOODLESS])
			return;
	
		// 10% or 10 flat healing per hit, minimum
		int hp = Max(dmg / 10, 10);
		
		// ignite effects prevent vampirism healing
		if(!CheckActorInventory(monster_tid, "DnD_IgniteTimer")) {
			if(GetActorProperty(monster_tid, APROP_HEALTH) < MonsterProperties[m_id].maxhp - hp)
				SetActorProperty(monster_tid, APROP_HEALTH, GetActorProperty(monster_tid, APROP_HEALTH) + hp);
			else
				SetActorProperty(monster_tid, APROP_HEALTH, MonsterProperties[m_id].maxhp);
			ACS_NamedExecuteAlways("DnD Vampirism FX CS", 0, monster_tid);
		}
	}

	if(MonsterProperties[m_id].trait_list[DND_BLACKOUT] && isPlayer(victim))
		ACS_NamedExecuteAlways("DnD Blackout", 0, victim);
}

int HandlePetMonsterDamageScale(int this, int master, int victim, int dmg, int dmg_data, int flags) {
	// set pointer to owner
	SetActivator(master);

	int pnum = master - P_TIDSTART, temp;
	bool wantDmgNums = flags == -1;
	int dmgnum_flags = 0;

	// extract damage category from dmg_data stamina
	int dmg_category = MonsterDamageTypeToDamageCategory(dmg_data);
	
	// revived monsters have half stat gain
	dmg += MapDamageCategoryToFlatBonus(pnum, dmg_category);
	dmg = dmg * (100 + HandleStatBonus(pnum, 0, 0, DND_STAT_ATTUNEMENT_GAIN / 2, true)) / 100;

	if((dmg_category == DND_DAMAGECATEGORY_BULLET || dmg_category == DND_DAMAGECATEGORY_MELEE) && (temp = GetPlayerAttributeValue(pnum, INV_EX_PHYSDAMAGEPER_FLATHEALTH))) {
		temp = GetFlatHealthDamageFactor(temp);
		dmg = dmg * (100 + temp) / 100;
	}

	temp = MapDamageCategoryToPercentBonus(pnum, dmg_category);
	if(temp)
		dmg = dmg * (100 + temp) / 100;
	
	// finally crit chance -- move to damagedeal for pets in damage event later
	if(CheckCritChance(pnum, -1, -1, false, -1)) {
		dmg = dmg * GetCritModifier(pnum, -1, -1) / 100;
		HandleHunterTalisman();
		dmgnum_flags |= DND_DAMAGETICFLAG_CRIT;
	}

	if(flags == -1)
		ACS_NamedExecuteWithResult("DnD Damage Numbers", victim, dmg, dmgnum_flags);

	SetActivator(this);

	return dmg;
}

int HandlePercentDamageFromEnemy(int victim, int dmg, int dmg_data) {
	// check inflictor momentarily
	if((!(dmg_data & DND_DAMAGETYPEFLAG_PERCENTHP) && !(dmg_data & DND_DAMAGETYPEFLAG_PERCENTHP_LOW)) || CheckActorInventory(victim, "DnD_PercentDamageHalt"))
		return 0;
	
	int pct = DND_MONSTER_PERCENTDAMAGEBASE_LOW;
	// big pct damage has a halt check here
	if(dmg_data & DND_DAMAGETYPEFLAG_PERCENTHP) {
		GiveActorInventory(victim, "DnD_PercentDamageHalt", 1);
		pct = DND_MONSTER_PERCENTDAMAGEBASE;
	}

	int res = (GetActorProperty(victim, APROP_HEALTH) * pct) / 100;
	if(res < 0)
		res = 1;

	return res;
}

int MonsterSpecificDamageChecks(int m_id, int victim, int dmg) {
	if(IsMonsterIdBoss(MonsterProperties[m_id].id) && IsQuestComplete(victim, QUEST_KILL10BOSSES))
		dmg = dmg * (100 - DND_QUEST_BOSSREDUCE) / 100;
	return dmg;
}

void OnPlayerHit(int this, int pnum, int target, bool isMonster) {
	int m_id, val;

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

	// check unstable power core
	if(HasPlayerPowerset(pnum, PPOWER_ESHIELDEXPLODE) && (m_id = CheckInventory("EShieldAmount")) && RunLuckBasedChance(pnum, CheckInventory("EShieldBlowChance"), DND_LUCK_OUTCOME_GAIN / 2)) {
		// explode for this amount now
		SpawnForced("UnstableExplosion", GetActorX(0), GetActorY(0), GetActorZ(0) + GetActorViewHeight(this) / 2, DND_UNSTABLEEXP_TID);
		SetActivator(DND_UNSTABLEEXP_TID);
		SetActorProperty(0, APROP_MASS, 256);
		//  damage scales from 25% to 50% depending on player's eshield %
		// if has % hp contribution mod, include it to base dmg too!
		val = GetPlayerAttributeValue(pnum, INV_EX_ESEXPLOSIONHPDMG);
		if(val) {
			m_id += GetActorProperty(this, APROP_HEALTH) * val / 100;
			SetActorProperty(0, APROP_HEALTH, 2);
		}

		m_id = m_id * (UNSTABLE_DMG_PCT + UNSTABLE_DMG_PCT * GetPlayerEnergyShieldPercent(pnum) / 100) / 100;
		m_id &= 0xFFFF; // limit to 65536
		// encode damage type
		m_id |= DND_DAMAGETYPE_ENERGY << 16;

		SetActorProperty(0, APROP_SCORE, m_id);
		SetActorProperty(0, APROP_TARGETTID, this);
		SetPointer(AAPTR_TARGET, this);
		SetActivator(this);

		// take away all ES
		SetEnergyShield(0);

		// depletion check
		temp = GetPlayerAttributeValue(pnum, INV_EX_STARTESONDEPLETE);
		if(temp && random(1, 100) <= temp && (m_id = CanRegenEShield(pnum))) {
			GiveInventory("EShieldChargeNow", 1);
			ACS_NamedExecuteAlways("DnD Energy Shield Regen", 0, m_id, pnum);
		}
	}
	
	// player heal on hit check -- target is 0 if we are the target, but the extra check in there is for safety
	// extra is chance to proc, value is the heal %
	temp = GetPlayerAttributeExtra(pnum, INV_EX_CHANCE_HEALMISSINGONPAIN);
	if(temp && target && target != this && random(1, 100) <= temp && !CheckActorInventory(temp, "DnD_HealOnMissingCD")) {
		// heal for missing health
		GiveActorInventory(this, "VeilHealFXSpawner", 1); // use same fx as veil for now
		GiveActorInventory(this, "DnD_HealOnMissingCD", 1);
		SetActivator(this);
		HandleHealthPickup((GetPlayerAttributeValue(pnum, INV_EX_CHANCE_HEALMISSINGONPAIN) * GetMissingHealth()) / 100, 0, 0);
		// restore ptr
		SetActivator(0, AAPTR_DAMAGE_TARGET);
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
	m_id = target - DND_MONSTERTID_BEGIN;
	if(MonsterProperties[m_id].trait_list[DND_THIEF]) {
		// get current weapon's ammo and steal it if possible
		temp = random(0, 1);
		str cur_ammo = GetWeaponAmmoType(GetActorWeaponID(this), temp);
		
		// if we picked no ammo, flip to check the other one using negation
		if(cur_ammo == "")
			cur_ammo = GetWeaponAmmoType(GetActorWeaponID(this), !temp);
		
		// if it's something we can take ammo from		
		if(cur_ammo != "")
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

bool HandleRipperHit(int shooter, int victim) {
	// increment id by 1 for each call, doesnt matter if it overflows
	static int ripper_count = 0;
	static int ripper_hits[MAX_RIPPERS_ACTIVE][MAX_RIPPER_HITS_STORED];

	int i;

	// reset ripper hit array
	int ripper_id = GetUserVariable(0, "user_ripperid");
	if(!ripper_id) {
		ripper_count = (ripper_count + 1) % MAX_RIPPERS_ACTIVE;
		ripper_id = ripper_count;

		for(i = 0; i < MAX_RIPPER_HITS_STORED; ++i)
			ripper_hits[ripper_id][i] = -1;

		SetUserVariable(0, "user_ripperid", ripper_id);
	}

	bool found = false;

	for(i = 0; i < MAX_RIPPER_HITS_STORED && ripper_hits[ripper_id][i] != -1; ++i) {
		if(ripper_hits[ripper_id][i] == victim) {
			found = true;
			break;
		}
	}

	// record it as added into the array and return true
	if(!found && i < MAX_RIPPER_HITS_STORED) {
		ripper_hits[ripper_id][i] = victim;
		return false;
	}

	return true;
}

// should this projectile reflect back? -- takes dmg_data as flags
// 0 is yes, 1 is no
bool CheckReflect(int owner, int pnum, int flags) {
	//printbold(d:(flags & DND_DAMAGEFLAG_ISEXPLOSIVE), s: " ", d:CheckActorInventory(owner, "Marine_Perk5"));
	return 	CheckFlag(0, "DONTREFLECT") || CheckUniquePropertyOnPlayer(pnum, PUP_HOMINGNOREFLECT, CheckFlag(0, "SEEKERMISSILE"), CheckFlag(0, "SCREENSEEKER")) ||
			((flags & DND_DAMAGEFLAG_ISEXPLOSIVE) && CheckActorInventory(owner, "Marine_Perk5"));
}

// shooter is who fired initially and victim is the tid of the actor that got hit that'll now own the projectile
void HandleReflect(int shooter, int victim, str proj_name, int encoded_data, int dmg_data, int spd, bool useGravity, int hit_x, int hit_y, int hit_z) {
	int pnum = shooter - P_TIDSTART;

	// the hit location
	int v_Pos = GetVec3(hit_x, hit_y, hit_z);

	// velocity vector will be randomized ie. towards the shooter but with some randomness
	int v_Vel = Vec3To_Pos(shooter, hit_x, hit_y, hit_z);
	ToUnitVec3(v_Vel);
	ScaleVec3(v_Vel, spd);
	SetVec3Z(v_Vel, FixedMul(GetVec3Z(v_Vel), random(0.75, 1.25)));
	RotateVector3(v_Vel, ANG_TO_DOOM(random(-45.0, 45.0)));

	int wid, dmg, dtype;
	if(IsPlayer(shooter)) {
		// wepid
		wid = encoded_data & ATK_WID_MASK;
		encoded_data >>= ATK_CACHE_SHIFT;

		// dmg cache id
		dmg = encoded_data & ATK_CACHE_MASK;
		encoded_data >>= ATK_DTYPE_SHIFT;

		// dmg type
		dtype = encoded_data & ATK_DTYPE_MASK;
		encoded_data >>= ATK_DPCT_SHIFT;

		SetActivator(shooter);
		dmg = RetrieveWeaponDamage(pnum, wid, dmg, GetDamageCategory(dtype, dmg_data), dmg_data, dmg_data & DND_DAMAGEFLAG_ISSPECIALAMMO);
	}
	else {
		dmg = encoded_data;
	}
	SetActivator(victim);

	dtype = DND_ATF_DAMAGEINEXTRA | (useGravity ? DND_ATF_USEGRAVITY : 0);

	CreateProjectile(
		victim,
		PROJECTILE_HELPER_TID + pnum,
		proj_name,
		AngleOfVector3(v_Vel),
		0, // pitch doesn't matter for this
		spd >> 16,
		v_Vel,
		v_Pos,
		dtype,
		dmg
	);
	FreeVec3(v_Pos);
	FreeVec3(v_Vel);
}

Script "DnD Event Handler" (int type, int arg1, int arg2) EVENT {
	// arg1 contains damage, arg2 contains damage type as a string
	// this causes A_KillChildren etc. to actually work...
	if(arg2 == "Perish" || arg2 == "Special_NoPain" || arg2 == "SkipHandle") {
		SetResultValue(arg1);
		Terminate;
	}

	// in monster shooting player case, temp holds accuracy stored in the projectile!
	int temp, dmg, m_id;
	int pnum;
	if(type == GAMEEVENT_ACTOR_DAMAGED) {
		bool isRipper = false;
		int shooter = -1;

		SetActivator(0, AAPTR_DAMAGE_TARGET);
		int victim = ActivatorTID();

		// damage inflictor (projectile etc.) -- reflected projectiles seem to have "None" as their class
		// poisonDOT or any DOT has this characteristic as well so we must check for those as exceptions here
		SetActivator(0, AAPTR_DAMAGE_INFLICTOR);
		if(arg2 == "Melee")
			shooter = ActivatorTID(); // apparently the damagesource is 0 under melee case for some reason...
		//printbold(s:GetactorClass(0), s:" inflicts damage ", d:arg1, s:" type ", s:arg2);
		int dmg_data = GetActorProperty(0, APROP_STAMINA);
		//printbold(s:"dmg flag: ", d:dmg_data);
		int inflictor_class = GetActorClass(0);

		int ox = GetActorX(0);
		int oy = GetActorY(0);
		int oz = GetActorZ(0);
		int factor = 0;
		int dist_damage_bonus = 0;

		int actor_flags = ScanActorFlags();
		temp = GetActorProperty(0, APROP_ACCURACY);
		if(temp == DND_CRIT_TOKEN) {
			actor_flags |= DND_ACTORFLAG_CONFIRMEDCRIT;
			// printbold(s:"actor got crit confirm");
		}

		if(dmg_data & DND_DAMAGEFLAG_DISTANCEGIVESDAMAGE)
			dist_damage_bonus = GetUserVariable(0, DND_DISTANCEDAMAGE_VARIABLE);

		int isArmorPiercing = CheckFlag(0, "PIERCEARMOR");
		isRipper = CheckFlag(0, "RIPPER");

		// set activator to owner of this projectile for certain crediting
		SetActivator(0, AAPTR_DAMAGE_SOURCE);
		if(shooter == -1) {
			//printbold(s:"this ? ", d:ActivatorTID(), s: " ", s:GetActorClass(ActivatorTID()));
			shooter = ActivatorTID();
		}

		// this flag shares the same value as a damagetype for monsters, so we need to seperate it
		SetActivator(0, AAPTR_DAMAGE_INFLICTOR);
		if((shooter == -1 || shooter == 0) && !IsMonster(shooter) && dmg_data & DND_DAMAGEFLAG_USEMASTER) {
			//printbold(s:"take shooter as ", d:GetActorProperty(0, APROP_SCORE));
			shooter = GetActorProperty(0, APROP_SCORE);
		}

		// if whatever fired this is a monster but original owner was player, it's reflected
		int isReflected = GetActorProperty(0, APROP_SCORE);
		if
		(
			(
				IsPlayer(shooter) && 
				IsMonster(victim) && 
				!(dmg_data & DND_DAMAGEFLAG_ISHITSCAN) && 
				!(dmg_data & DND_DAMAGEFLAG_ISRADIUSDMG) && 
				HasTrait(victim - DND_MONSTERTID_BEGIN, DND_REFLECTIVE) &&
				!CheckReflect(shooter, shooter - P_TIDSTART, dmg_data)
			) ||
			(
				IsMonster(shooter) &&
				IsPlayer(victim) &&
				(dmg_data & DND_DAMAGETYPEFLAG_REFLECTABLE) &&
				!CheckFlag(0, "DONTREFLECT") &&
				CheckActorInventory(victim, "ReflectiveState")
			)
		)
		{
			// fire back at player the same thing that hurt the player
			Thing_ChangeTID(0, AUX_PROJ_TID + shooter - P_TIDSTART);

			isArmorPiercing = GetActorProperty(0, APROP_SPEED);
			isRipper = !CheckFlag(0, "NOGRAVITY");
			factor = GetActorClass(0);

			SetActivator(0, AAPTR_DAMAGE_TARGET);

			// handle reflection firing projectile code
			HandleReflect(shooter, ActivatorTID(), factor, arg1, dmg_data, isArmorPiercing, isRipper, ox, oy, oz);

			Thing_Remove(AUX_PROJ_TID + shooter - P_TIDSTART);
			SetResultValue(0);
			Terminate;
		}

		SetActivator(0, AAPTR_DAMAGE_SOURCE);
		if(dmg_data & DND_DAMAGEFLAG_ISHITSCAN) {
			ox = GetActorX(shooter);
			oy = GetActorY(shooter);
			oz = GetActorZ(shooter);
		}

		// if the inflictor had no damage data for some reason, try to look it up from the monster
		if(!dmg_data)
			dmg_data = GetActorProperty(0, APROP_STAMINA);
		isArmorPiercing |= CheckFlag(shooter, "PIERCEARMOR");

		// set the activator to us now
		SetActivator(0, AAPTR_DAMAGE_TARGET);
		//printbold(s:"Victim is ", s:GetActorClass(0));
		
		dmg = arg1;

		// FROM HERE ON WHOEVER TOOK DAMAGE IS THE ACTIVATOR, PLAYER OR MONSTER!
		if(IsMonster(shooter)) {
			m_id = shooter - DND_MONSTERTID_BEGIN;
			isArmorPiercing |= MonsterProperties[m_id].trait_list[DND_PIERCE];
		
			// if victim was a monster, check for infight situation
			// BOTH VICTIM AND SHOOTER ARE MONSTERS HERE
			//printbold(s:GetActorProperty(victim, APROP_SPECIES), s: " ", s:GetActorProperty(shooter, APROP_SPECIES));
			if
			(
				// added condition for friendlies
				(IsPlayer(victim) && GetActorProperty(shooter, APROP_SPECIES) == "Player") ||
				(
					IsMonster(victim) && 
					victim != shooter && 
					GetActorProperty(victim, APROP_SPECIES) == GetActorProperty(shooter, APROP_SPECIES)
				)
			)
			{
				// no damage dealt from same species, makes damage things much easier to keep track of	
				SetResultValue(0);
				Terminate;
			}

			// DoT shouldn't double dip
			if(!IsDamageStringDOT(arg2)) {
				// dont scale reflected damage by this -- reflected damage will have score property as damage of proj, if it's not a player tid its reflected
				//printbold(s:"here?? ", d:isReflected);
				if(!isReflected || IsPlayer(isReflected)) {
					// special bonuses
					factor += 	(MonsterProperties[m_id].level > 1) * GetMonsterDMGScaling(m_id, MonsterProperties[m_id].level, false, temp, GetActorProperty(shooter, APROP_ACCURACY)) + 
								MonsterProperties[m_id].trait_list[DND_EXTRASTRONG] * DND_ELITE_EXTRASTRONG_BONUS;

					dmg = dmg * (100 + factor) / 100;

					// elite damage bonus is multiplicative
					if(MonsterProperties[m_id].isElite/* && dmg < INT_MAX / factor*/)
						dmg = dmg * (100 + GetEliteBonusDamage(m_id)) / 100;
						
					// chaos mark is multiplicative
					factor = 100 + CHAOSMARK_DAMAGEBUFF;
					if(MonsterProperties[m_id].trait_list[DND_MARKOFCHAOS]/* && dmg < INT_MAX / factor*/)
						dmg = dmg * (100 + CHAOSMARK_DAMAGEBUFF) / 100;
						
					// % damage effects -- this is same for all monsters which is 10% of player's maximum health added as damage
					dmg += HandlePercentDamageFromEnemy(victim, dmg, dmg_data);
				}
				else {
					// unpack regular weapon dmg from isReflected variable
					//printbold(s:"reflected damage ", d:isReflected);
					arg1 = isReflected / 2;
					dmg = arg1;
					arg2 = "Reflection";
					isReflected = true;
				}
			}

			if(isRipper)
				dmg >>= 1;
				
			// halved by demon sealer effect if any
			if(CheckActorInventory(shooter, "DemonSealDamageDebuff"))
				dmg >>= 1;

			// if this was a player, factor their resists in
			// resists of player now will factor in after we've calculated the damage accurately
			if(IsPlayer(victim)) {
				if(CheckActorInventory(victim, "DnD_CountdownProtection")) {
					SetResultValue(0);
					Terminate;
				}

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
					if(GetActorProperty(shooter, APROP_HEALTH) <= dmg)
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
				dmg = HandlePlayerOnHitBuffs(victim, shooter, dmg, dmg_data, arg2);
				// finally apply player armor
				dmg = HandlePlayerArmor(pnum, dmg, arg2, dmg_data, isArmorPiercing);
					
				// doomguy demon reduction
				if(IsMonsterIdDemon(m_id) && CheckInventory("Doomguy_Perk5"))
					dmg = ApplyDamageFactor_Safe(dmg, 100 - DND_DOOMGUY_DMGREDUCE_PERCENT);

				// berserker damage reduction
				temp = CheckInventory("Berserker_DamageTracker");
				if(temp)
					dmg = ApplyDamageFactor_Safe(dmg, 100 - temp * DND_BERSERKER_PERK25_REDUCTION);
				
				// final check, if damage is less than 10% of it, cap it at 10%
				temp = arg1 / 10;
				if(temp <= 0)
					temp = 1;

				if(dmg < temp)
					dmg = temp;

				// the real final check vs eshield
				dmg = ApplyPlayerEnergyShield(pnum, dmg, arg2, dmg_data);

				// damage amplifications
				temp = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_TAKEN) + 100;
				if(temp > 100)
					dmg = ApplyDamageFactor_Safe(dmg, temp);
					
				dmg = MonsterSpecificDamageChecks(m_id, victim, dmg);
					
				if(dmg) {
					// add to player stat
					IncrementStatistic(DND_STATISTIC_DAMAGETAKEN, dmg, victim);
					//GiveInventory("DnD_DamageReceived", dmg);
					PlayerScriptsCheck[DND_SCRIPT_DAMAGETAKENTIC][pnum] = dmg;
					PlayerScriptsCheck[DND_SCRIPT_BLEND][pnum] = false;

					if(CheckInventory("Marine_Perk50") && !CheckInventory("Marine_Perk50_Cooldown"))
						GiveInventory("Marine_Perk50_DamageTaken", dmg);
					
					// mugshot hook
					ACS_NamedExecuteWithResult("DnD Player MugshotData Ouchies", shooter, dmg);
				}

				// these are on monsters only, dont have much to do with us beyond this point
				HandleMonsterDamageModChecks(m_id, shooter, victim, dmg);
			}
			else {
				temp = GetActorProperty(shooter, APROP_MASTERTID);
				if(IsPlayer(temp)) {
					dmg = HandlePetMonsterDamageScale(shooter, temp, victim, dmg, dmg_data, -1);

					// these are on monsters only, dont have much to do with us beyond this point
					HandleMonsterDamageModChecks(m_id, shooter, victim, dmg);
					
					if(GetActorProperty(victim, APROP_HEALTH) <= dmg)
						GiveActorInventory(victim, "MonsterKilledByPlayer", 1);
				}
			}
			
			//printbold(s:"old dmg ", d:arg1, s: " new dmg: ", d:dmg);
			SetResultValue(dmg);
		}
		else if(IsPlayer(shooter) && !IsPlayer(victim)) {
			// PLAYER HURTING MONSTERS CODE HERE
			// extract the encoded damage data, and proceed
			// stamina contains any special flags we might need
			// variable swap here to fix a bug with radius damage projectiles that also rip once
			if(!isReflected || IsPlayer(isReflected)) {
				factor = arg1;
				SetActivator(0, AAPTR_DAMAGE_INFLICTOR);
				if(dmg_data & DND_DAMAGEFLAG_RIPSONCE) {
					// insert victim to a temporary array and check if it exists in there before continuing
					// this marks that we had a ripper case that we need to handle later that we hit, we havent got access to weapon id at this point so we cant know until later
					if(!HandleRipperHit(shooter, victim)) {
						isArmorPiercing = true;

						// we really care about this if the ripper hits like this, not explosion portion at all
						if((temp = GetUserVariable(0, "user_expdmg")))
							arg1 = temp;
					}
					else {
						// ignore this event
						SetResultValue(0);
						Terminate;
					}
				}

				if(dmg_data & DND_DAMAGEFLAG_ISRADIUSDMG) {
					// explosions do not hit monsters under these conditions
					if(!CheckFlag(victim, "SHOOTABLE") || (CheckFlag(victim, "GHOST") && (actor_flags & DND_ACTORFLAG_THRUGHOST))) {
						SetResultValue(0);
						Terminate;
					}

					// save the percentage of damage from radius falloff into somewhere (arg1 base is 100, so we can use it as percentage)
					//printbold(s:inflictor_class, s: " aoe dmg ", d:GetUserVariable(0, "user_expdmg"), s: " factor ", d:factor);
					inflictor_class = factor;
					arg1 = GetUserVariable(0, "user_expdmg");
					dmg_data |= GetUserVariable(0, "user_expflags");
				}
				
				SetActivator(shooter);

				pnum = shooter - P_TIDSTART;

				// spells get raw damage in here, they don't encode weapon data or anything
				if(dmg_data & DND_DAMAGEFLAG_NONWEAPON) {
					// non-weapon sources that aren't spells
					dmg = arg1 & 0xFFFF; // dmg
					temp = arg1 >> 16; // dmg_type
					m_id = -1;
				}
				else if(dmg_data & DND_DAMAGEFLAG_ISSPELL) {
					// first 16 bits is spell damage, next is damage type and last is spell id
					dmg = arg1 & SPELLDMG_MASK;
					dmg += dist_damage_bonus; // depending on distance increasing damage modifier this can be non-zero

					arg1 >>= SPELL_DMG_SHIFT;
					temp = arg1 & SPELLDTYPE_MASK;

					arg1 >>= SPELL_DTYPE_SHIFT;
					m_id = arg1 & SPELLID_MASK;
				}
				else {
					// regular weapon dmg
					// wepid
					m_id = arg1 & ATK_WID_MASK;
					arg1 >>= ATK_CACHE_SHIFT;

					// dmg cache id
					dmg = arg1 & ATK_CACHE_MASK;
					arg1 >>= ATK_DTYPE_SHIFT;

					// dmg type
					temp = arg1 & ATK_DTYPE_MASK;
					arg1 >>= ATK_DPCT_SHIFT;

					// weapon check for sedrin staff
					if(m_id == DND_WEAPON_SEDRINSTAFF && IsActorFullRobotic(victim)) {
						ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_IMMUNITY);
						SetResultValue(0);
						Terminate;
					}

					//printbold(d:dmg, s: " ", d:m_id, s: " ", d:temp, s: " ", d:arg1);

					dmg = RetrieveWeaponDamage(pnum, m_id, dmg, GetDamageCategory(temp, dmg_data), dmg_data, dmg_data & DND_DAMAGEFLAG_ISSPECIALAMMO);

					//printbold(s:"retrieved dmg ", d:dmg);

					dmg += dist_damage_bonus; // depending on distance increasing damage modifier this can be non-zero
					// % adjustment factor -- extract after the flat addition to reuse variables
					factor = arg1 & ATK_DPCT_MASK;

					if(dmg_data & DND_DAMAGEFLAG_LOSEDAMAGEPERHIT)
						dmg = ACS_NamedExecuteWithResult("DnD Adjust Impact Damage", dmg_data, dmg, shooter);

					// setup the flags and factor
					if(factor != 100)
						dmg = dmg * factor / 100;
				}

				if(dmg_data & DND_DAMAGEFLAG_ISRADIUSDMG) {
					//printbold(d:dmg, s: " ", d:inflictor_class, s:" ", d:dmg * inflictor_class / 100);
					dmg = dmg * inflictor_class / 100;
					if(!dmg) {
						SetResultValue(0);
						Terminate;
					}
				}

				if((dmg_data & DND_DAMAGEFLAG_FOILINVUL) || (HasPlayerPowerSet(pnum, PPOWER_MELEEIGNORESHIELD) && (IsMeleeDamage(temp) || (dmg_data & DND_DAMAGEFLAG_COUNTSASMELEE)))) {
					actor_flags |= DND_ACTORFLAG_FOILINVUL;
					dmg_data ^= DND_DAMAGEFLAG_FOILINVUL;
				}
				
				if(dmg_data & DND_DAMAGEFLAG_COUNTSASMELEE) {
					actor_flags |= DND_ACTORFLAG_COUNTSASMELEE;
					dmg_data ^= DND_DAMAGEFLAG_COUNTSASMELEE;
				}

				if(isArmorPiercing)
					HandleRipperHitSound(victim, shooter, m_id);

				// damage boost on overheating things
				// factor variable isnt used below for anything saved prior so we can use it too
				isArmorPiercing = GetPlayerAttributeValue(pnum, INV_EX_MOREDMGPEROVERHEAT);
				factor = GetCurrentWeaponID();
				if(isArmorPiercing && CanWeaponOverheat(factor)) {
					// add the extra damage as "more" on top --- ammo2 is always the overheat on overheating weapons
					factor = CheckInventory(Weapons_Data[factor].ammo_name2);
					dmg = dmg * (((100.0 + isArmorPiercing * factor) >> 16)) / 100;
				}

				// Class effects here -- isArmorPiercing holds if wepid is negative or not
				isArmorPiercing = (m_id < 0 || (dmg_data & (DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISSPECIALAMMO)));
				if(!isArmorPiercing) {
					// berserker perk50 dmg increase portion and other melee increases
					if((IsMeleeWeapon(m_id) || (actor_flags & DND_ACTORFLAG_COUNTSASMELEE))) {
						if(CheckInventory("Berserker_Perk50")) {
							SetInventory("Berserker_HitTimer", DND_BERSERKER_PERK50_TIMER);
							if
							(
								!CheckInventory("Berserker_Perk50_HitCooldown") &&
								(factor = CheckInventory("Berserker_HitTracker")) < DND_BERSERKER_PERK50_MAXSTACKS && 
								(inflictor_class = CheckInventory("Berserker_Perk50_HitCounter")) < DND_BERSERKER_PERK50_MAXHITS
							)
							{
								GiveInventory("Berserker_HitTracker", 1);

								GiveInventory("Berserker_Perk50_HitCounter", 1);
								if(inflictor_class + 1 >= DND_BERSERKER_PERK50_MAXHITS) {
									// now that we hit cooldown time, reset the counter
									GiveInventory("Berserker_Perk50_HitCooldown", 1);
									SetInventory("Berserker_Perk50_HitCounter", 0);
								}

								if(!factor)
									ACS_NamedExecuteAlways("DnD Berserker Perk50 Timer", 0, shooter);
							}

							if(factor + 1 >= DND_BERSERKER_PERK50_MAXSTACKS) {
								if(!CheckInventory("Berserker_NoRoar"))
									HandleBerserkerRoar(shooter);
								GiveInventory("Berserker_Perk50_Speed", 1);
							}
							dmg = dmg * (100 + (factor + 1) * DND_BERSERKER_PERK50_DMGINCREASE) / 100;
						}
						
						dmg = dmg * (100 + GetPlayerAttributeValue(pnum, INV_MELEEDAMAGE)) / 100;
					}
					
					// Flayer magic or undead check
					if(m_id == DND_WEAPON_CROSSBOW && IsActorMagicOrUndead(victim))
						ACS_NamedExecuteWithResult("DnD Crossbow Explosion", victim, shooter);
				}

				// cyborg perk50
				if(!isArmorPiercing && CheckInventory("Cyborg_Perk50") && IsTechWeapon(m_id)) {
					factor = CheckInventory("Cyborg_InstabilityStack");
					
					SetInventory("Cyborg_Instability_Timer", DND_CYBORG_INSTABILITY_TIMER);
					if(!factor)
						ACS_NamedExecuteAlways("DnD Cyborg Instability Timer", 0);
					else if(factor == DND_MAXCYBORG_INSTABILITY - 1 && !CheckInventory("Cyborg_NoAnim")) {
						// we check -1 above because we'll give 1 already
						PlaySound(shooter, "Cyborg/Unstable", CHAN_BODY, 1.0);
						GiveInventory("Cyborg_NoAnim", 1);
						ACS_NamedExecuteAlways("DnD Cyborg Visor Anim", 0);
					}
					GiveInventory("Cyborg_InstabilityStack", 1);
				}
			}
			else {
				SetActivator(shooter);
				m_id = -1;
				dmg = isReflected;
			}

			// 25% less damage taken
			if(CheckActorInventory(victim, "WarmasterProtect"))
				dmg = 3 * dmg / 4;
 
			// finally dealing the damage
			if(victim) {
				dmg = HandleDamageDeal(shooter, victim, dmg, temp, m_id, dmg_data, ox, oy, oz, actor_flags, (m_id < 0) || dmg_data & (DND_DAMAGEFLAG_ISSPELL | DND_DAMAGEFLAG_ISSPECIALAMMO), 0);
				if(dmg < 0)
					dmg = 0;

				SetResultValue(dmg);
				HandleOnHitEffects(shooter);
			}
			else {
				// simply deal the extracted damage, nothing to be done -- support for non-mod monsters and objects
				SetResultValue(dmg);
				HandleOnHitEffects(shooter);
			}
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
			if(CheckActorInventory(victim, "DnD_CountdownProtection")) {
				SetResultValue(0);
				Terminate;
			}

			// exception for map related hazards
			if(arg2 == "Slime" || arg2 == "Crush" || arg2 == "Drowning" || arg2 == "Telefrag" || arg2 == "Suicide" || arg2 == "InstantDeath" || arg2 == "Exit") {
				// apply eshield to these only
				if(arg2 == "Slime" || arg2 == "Crush" || arg2 == "Drowning")
					dmg = ApplyPlayerEnergyShield(pnum, dmg, arg2, 0);
				GiveActorInventory(victim, "DnD_Hit_CombatTimer", 1);
				SetResultValue(dmg);
				PlayerScriptsCheck[DND_SCRIPT_DAMAGETAKENTIC][pnum] = arg1;
				Terminate;
			}

			// the above check was necessary
			// hurt self -- handleplayerselfdamage is ran in explosion side of things, we run additional stuff that isnt handled by that here, like resists and armor
			pnum = PlayerNumber();

			if(dmg_data & DND_DAMAGEFLAG_ISRADIUSDMG) {
				SetActivator(0, AAPTR_DAMAGE_INFLICTOR);

				// save the percentage of damage from radius falloff into somewhere (arg1 base is 100, so we can use it as percentage)
				inflictor_class = arg1;
				arg1 = GetUserVariable(0, "user_expdmg");
				dmg_data |= GetUserVariable(0, "user_expflags");

				// not meant to be hurting self
				if(!(dmg_data & DND_DAMAGEFLAG_BLASTSELF)) {
					SetResultValue(0);
					Terminate;
				}
			}

			SetActivator(shooter);

			// wepid
			m_id = arg1 & ATK_WID_MASK;
			arg1 >>= ATK_CACHE_SHIFT;

			// dmg cache id
			dmg = arg1 & ATK_CACHE_MASK;
			arg1 >>= ATK_DTYPE_SHIFT;

			// dmg type
			temp = arg1 & ATK_DTYPE_MASK;
			arg1 >>= ATK_DPCT_SHIFT;

			// % adjustment factor
			factor = arg1 & ATK_DPCT_MASK;

			dmg = RetrieveWeaponDamage(pnum, m_id, dmg, GetDamageCategory(temp, dmg_data), dmg_data, dmg_data & DND_DAMAGEFLAG_ISSPECIALAMMO);

			// setup the flags and factor
			if(factor != 100)
				dmg = dmg * factor / 100;

			if(dmg_data & DND_DAMAGEFLAG_ISRADIUSDMG) {
				dmg = dmg * inflictor_class / 100;
				if(!dmg) {
					SetResultValue(0);
					Terminate;
				}
			}

			// a third to self
			if(dmg_data & DND_DAMAGEFLAG_HALFDMGSELF)
				dmg /= 3;

			GiveActorInventory(victim, "DnD_Hit_CombatTimer", 1);

			if(!CheckActorInventory(victim, "DnD_Hit_Cooldown")) {
				OnPlayerHit(victim, pnum, shooter, false);
				GiveActorInventory(victim, "DnD_Hit_Cooldown", 1);
			}

			dmg = HandlePlayerOnHitBuffs(victim, shooter, dmg, dmg_data, arg2);
			dmg = HandlePlayerResists(pnum, dmg, arg2, dmg_data, isReflected, inflictor_class);
			dmg = HandlePlayerArmor(pnum, dmg, arg2, dmg_data, false);
			dmg = ApplyPlayerEnergyShield(pnum, dmg, arg2, dmg_data);
			//GiveInventory("DnD_DamageReceived", dmg);
			PlayerScriptsCheck[DND_SCRIPT_DAMAGETAKENTIC][pnum] = dmg;
			IncrementStatistic(DND_STATISTIC_DAMAGETAKEN, dmg, victim);

			if(CheckInventory("Marine_Perk50") && !CheckInventory("Marine_Perk50_Cooldown"))
				GiveInventory("Marine_Perk50_DamageTaken", dmg);
			
			// mugshot hook
			ACS_NamedExecuteWithResult("DnD Player MugshotData Ouchies", shooter, dmg);

			if(!(dmg_data & DND_DAMAGEFLAG_NOPUSH))
				HandleDamagePush(dmg * 4, ox, oy, oz, shooter);
			SetResultValue(dmg);
		}
	}
}

#endif
