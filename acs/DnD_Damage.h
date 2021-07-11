#ifndef DND_DAMAGE_IN
#define DND_DAMAGE_IN

#define DND_DMGPUSH_CAP 96.0
#define DND_PLAYER_HITSCAN_Z 38.0
#define MAX_RIPPERS_ACTIVE 256
#define MAX_RIPPER_HITS_STORED 128
#define DND_CROSSBOW_EXPLOSIONTID 54100

#define DND_WEPID_USERVARIABLE "user_wepid"

#define DND_BERSERKER_DAMAGETRACKTIME 17 // 3 is base, x 5 -- +2 for 0.5 second of buffer inclusion
#define DND_BERSERKER_PERK25_MAXSTACKS 15
#define DND_BERSERKER_PERK25_HEALPERCENT 15
#define DND_BERSERKER_PERK25_REDUCTION 0.02 // 2% per stack
#define DND_BERSERKER_PERK50_TIMER 14 // 14 x 5 = 70 => 2 seconds
#define DND_BERSERKER_PERK50_DMGINCREASE 8 // 8%

#define DND_CRIT_TOKEN 69

enum {
	DND_DAMAGETYPE_MELEE,
	DND_DAMAGETYPE_MELEEOCCULT,
	DND_DAMAGETYPE_PHYSICAL,
	DND_DAMAGETYPE_SILVERBULLET,
	DND_DAMAGETYPE_ENERGY,
	DND_DAMAGETYPE_ENERGYEXPLOSION,
	DND_DAMAGETYPE_EXPLOSIVES,
	DND_DAMAGETYPE_OCCULT,
	DND_DAMAGETYPE_OCCULTFIRE,
	DND_DAMAGETYPE_OCCULTEXPLOSION,

	// elemental types
	DND_DAMAGETYPE_FIRE,
	DND_DAMAGETYPE_ICE,
	DND_DAMAGETYPE_POISON,
	DND_DAMAGETYPE_DESOLATOR,
	DND_DAMAGETYPE_EMERALD,
	DND_DAMAGETYPE_LIGHTNING,
	
	DND_DAMAGETYPE_SOUL
};
#define MAX_DAMAGE_TYPES (DND_DAMAGETYPE_SOUL + 1)
#define DAMAGE_TYPE_SHIFT 5
#define DAMAGE_TYPE_MASK 0x1F

enum {
	DND_DAMAGECATEGORY_MELEE,
	DND_DAMAGECATEGORY_BULLET,
	DND_DAMAGECATEGORY_ENERGY,
	DND_DAMAGECATEGORY_EXPLOSIVES,
	DND_DAMAGECATEGORY_OCCULT,
	DND_DAMAGECATEGORY_ELEMENTAL,
	DND_DAMAGECATEGORY_SOUL
};

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
	
	// elemental
	"Fire",
	"P_Ice",
	"Poison",
	"Desolator",
	"Emerald",
	"Lightning",
	
	"SoulType"
};

enum {
	DND_DAMAGEFLAG_FORCERADIUSDMG 		= 			0b1,
	DND_DAMAGEFLAG_USEMASTER 			= 			0b10,
	DND_DAMAGEFLAG_ISSHOTGUN 			= 			0b100,
	DND_DAMAGEFLAG_CULL 				= 			0b1000,
	DND_DAMAGEFLAG_SELFCULL				=			0b10000,
	DND_DAMAGEFLAG_DISTANCEGIVESDAMAGE	=			0b100000,
	DND_DAMAGEFLAG_NOPOISONSTACK		=			0b1000000,
	DND_DAMAGEFLAG_HALFDMGSELF			=			0b10000000,
	DND_DAMAGEFLAG_INFLICTPOISON		=			0b100000000,
	DND_DAMAGEFLAG_BLASTSELF			=			0b1000000000,
	DND_DAMAGEFLAG_SELFORIGIN			=			0b10000000000,
	DND_DAMAGEFLAG_DOFULLDAMAGE			=			0b100000000000,
	DND_DAMAGEFLAG_EXTRATOUNDEAD		=			0b1000000000000,
	DND_DAMAGEFLAG_ISHITSCAN			=			0b10000000000000,
	DND_DAMAGEFLAG_NOIGNITESTACK		=			0b100000000000000,
	DND_DAMAGEFLAG_PERCENTHEALTH		=			0b1000000000000000,
	
	// below are special things that are cleared after a certain point in HandleImpactDamage function
	DND_DAMAGEFLAG_FOILINVUL			=			0b10000000000000000,
	DND_DAMAGEFLAG_COUNTSASMELEE		=			0b100000000000000000,
};

enum {
	DND_DAMAGETICFLAG_PUSH				=			0b1,
	DND_DAMAGETICFLAG_CRIT				=			0b10,
	DND_DAMAGETICFLAG_ICE				=			0b100,
	DND_DAMAGETICFLAG_FIRE				=			0b1000,
	DND_DAMAGETICFLAG_LIGHTNING			=			0b10000,
	DND_DAMAGETICFLAG_CONSIDERMELEE		=			0b100000,
};

// These are actor inherited flags, like forcepain, foilinvul, painless etc.
// can store at most 16 bits, the last 16 are for wepid reference
enum {
	DND_ACTORFLAG_FOILINVUL				=			0b1,
	DND_ACTORFLAG_FORCEPAIN				=			0b10,
	DND_ACTORFLAG_PAINLESS				=			0b100,
	DND_ACTORFLAG_NOPUSH				=			0b1000,
	DND_ACTORFLAG_CONFIRMEDCRIT			=			0b10000,
	DND_ACTORFLAG_COUNTSASMELEE			=			0b100000,
	DND_ACTORFLAG_THRUGHOST				=			0b1000000,
};

enum {
	DND_SCANNER_BFG,
	DND_SCANNER_BFGUPGRADED,
	DND_SCANNER_HEART,
	DND_SCANNER_BOOK
};

#define MAX_SCANNER_PARTICLES (DND_SCANNER_BOOK + 1)
str ScannerAttackParticles[MAX_SCANNER_PARTICLES] = {
	"BFGExtra2",
	"BFGExtraUpgraded",
	"HeartAttackPuff",
	"BookPuff"
};

typedef struct scan_data {
	int max_dist;
	int fov;
	int spawn_offZ;
} scan_data_T;

scan_data_T ScanAttackData[MAX_SCANNER_PARTICLES] = {
	{ 1024.0, 			0.1875, 			24.0 },
	{ 1024.0,		 	0.1875, 			24.0 },
	{ 4096.0,		 	0.25, 				32.0 },
	{ 2048.0,			0.25,				32.0 }
};

vec3_T PlayerDamageVector[MAXPLAYERS];

int Scan_to_WeaponID(int scan_id) {
	int ret = DND_WEAPON_BFG6000;
	switch(scan_id) {
		case DND_SCANNER_BFGUPGRADED:
			ret = DND_WEAPON_BFG32768;
		break;
		case DND_SCANNER_HEART:
			ret = DND_WEAPON_DEMONHEART;
		break;
		case DND_SCANNER_BOOK:
			ret = -1;
		break;
	}
	return ret;
}

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

#define DND_CULL_BASEPERCENT 10 // 1 / 10
#define DND_DESOLATOR_DMG_GAIN 10 // 10%
#define DND_DISTANCEDAMAGE_VARIABLE "user_tics"

#define DND_BASE_FREEZECHANCE_PERSTACK 2 // 10% base at max slow stacks
#define DND_BASE_FREEZETIMER 21 // 3 seconds base time (21 x 5 = 105)
#define DND_BASE_CHILL_CAP 5 // 50% health dealt in ice = maximum slow
#define DND_BASE_CHILL_DAMAGETHRESHOLD 10 // 10% of the monster's health
#define DND_BASE_CHILL_SLOW 10 // 10% per stack

#define DND_BASE_IGNITETIMER 20 // 4 seconds x 5
#define DND_BASE_IGNITEDMG 10
#define DND_BASE_IGNITECHANCE 15

#define DND_BASE_OVERLOADCHANCE 5
#define DND_BASE_OVERLOADBUFF 30 // 30%
#define DND_BASE_OVERLOADTICK 5
#define DND_BASE_OVERLOADTIME (105 / DND_BASE_OVERLOADTICK) // 3 seconds -- 105 / 5
#define DND_MAX_OVERLOADTARGETS 128 // up to 128 allowed
#define DND_BASE_OVERLOADZAPDELAY 3 // 3 tics

#define DND_BASE_POISON_STACKS 5
#define DND_BASE_POISON_TIMER 3.0
#define DND_BASE_POISON_TIC 0.5

#define DND_EXTRAUNDEADDMG_MULTIPLIER 3

#define DND_WEAKNESS_FACTOR 25 // 25% extra dmg
#define DND_SPECIFICELEWEAKNESS_FACTOR 50 // 50% extra dmg taken from specific elemental dmg
#define DND_RESIST_FACTOR 50 // 50% dmg taken
#define DND_IMMUNITY_FACTOR 95 // 5% dmg taken

#define DND_MAX_MONSTER_TICDATA 16383 // even this is a bit much but w.e
#define DND_MONSTER_TICDATA_BITMASK 0x3FFF // 14 bits
#define DND_DAMAGE_ACCUM_SHIFT 14 // 2^14 = 16384
int PlayerDamageTicData[MAXPLAYERS][DND_MAX_MONSTER_TICDATA];

int ScanActorFlags() {
	int res = 0;
	if(CheckFlag(0, "FOILINVUL"))
		res |= DND_ACTORFLAG_FOILINVUL;
	if(CheckFlag(0, "FORCEPAIN"))
		res |= DND_ACTORFLAG_FORCEPAIN;
	if(CheckFlag(0, "PAINLESS"))
		res |= DND_ACTORFLAG_PAINLESS;
	if(CheckFlag(0, "NODAMAGETHRUST"))
		res |= DND_ACTORFLAG_NOPUSH;
	if(CheckFlag(0, "THRUGHOST"))
		res |= DND_ACTORFLAG_THRUGHOST;
	return res;
}

void AdjustDamageRetrievePointers(int flags) {
	int temp;
	//printbold(s:"prev score? ", d:GetActorProperty(0, APROP_SCORE));
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

	// printbold(s:"owner ", d:ActivatorTID(), s: " ", d:GetActorProperty(0, APROP_SCORE));
	// no owner can potentially mean this was a reflected projectile, we can manipulate this a bit
	if(!ActivatorTID()) {
		GiveInventory("MarkAsReflected", 1);
		SetActivator(GetActorProperty(0, APROP_SCORE));
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

int ScaleCachedDamage(int wepid, int pnum, int dmgid, int talent_type, int flags, bool isSpecial) {
	// we don't cache special ammo damage
	int dmg = 0;
	int temp;
	// get the damage
	if(!isSpecial)
		temp = GetCachedPlayerDamage(pnum, wepid, dmgid);
	else // special ammo damage
		temp = GetSpecialAmmoDamage(dmgid, wepid);
		
	// check if we have a random range cached -- special ammo types dont use this
	int range = GetCachedPlayerRandomRange(pnum, wepid, dmgid);
	if(range > 1 && !isSpecial)
		dmg += temp * random(range & 0xFFFF, range >> 16);
	else // no rng, so just set it to temp
		dmg = temp;

	if(PlayerDamageNeedsCaching(pnum, wepid, dmgid) || isSpecial) {
		// add flat damage bonus mapping talent name to flat bonus type
		temp = MapTalentToFlatBonus(pnum, talent_type, flags);
		dmg += temp;
		
		ClearCache(pnum, wepid, dmgid);
		CachePlayerFlatDamage(pnum, temp, wepid, dmgid); 
		
		// quest or accessory bonuses
		// is occult (add demon bane bonus)
		if(flags & DND_WDMG_ISOCCULT || talent_type == TALENT_OCCULT) {
			temp = DND_DEMONBANE_GAIN * IsAccessoryEquipped(ActivatorTID(), DND_ACCESSORY_DEMONBANE);
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, wepid, dmgid, temp);
		}
		
		// gunslinger affected
		if(flags & DND_WDMG_ISPISTOL) {
			temp = DND_GUNSLINGER_GAIN * CheckInventory(Quest_List[QUEST_ONLYPISTOLWEAPONS].qreward);
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, wepid, dmgid, temp);
		}
		
		// shotgun affected
		if(flags & DND_WDMG_ISBOOMSTICK) {
			temp = DND_BOOMSTICK_GAIN * CheckInventory(Quest_List[QUEST_NOSHOTGUNS].qreward);
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, wepid, dmgid, temp);
		}
			
		// super weapon affected
		if(flags & DND_WDMG_ISSUPER) {
			temp = DND_SUPERWEAPON_GAIN * CheckInventory(Quest_List[QUEST_NOSUPERWEAPONS].qreward);
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, wepid, dmgid, temp);
		}
		
		// include the stat bonus
		if(talent_type == TALENT_MELEE) {
			temp = DND_STR_GAIN * GetStrength();
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, wepid, dmgid, temp);
		}
		
		// occult uses intellect
		if(flags & DND_WDMG_ISOCCULT || talent_type == TALENT_OCCULT) {
			temp = DND_INT_GAIN * GetIntellect();
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, wepid, dmgid, temp);
		}
		else if(talent_type != TALENT_MELEE) {
			temp = DND_DEX_GAIN * GetDexterity();
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, wepid, dmgid, temp);
		}

		// include enhancement orb bonuses
		temp = GetPlayerWeaponEnchant(pnum, wepid);
		if(temp) {
			temp *= ENHANCEORB_BONUS;
			dmg = dmg * (100 + temp) / 100;
		
			InsertCacheFactor(pnum, wepid, dmgid, temp);
		}
			
		// finally apply damage type or percentage bonuses
		temp = 	GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, talent_type) + 
				GetDataFromOrbBonus(pnum, OBI_WEAPON_DMG, wepid) +
				GetPlayerAttributeValue(pnum, INV_DAMAGEPERCENT_INCREASE) + 
				MapTalentToPercentBonus(pnum, talent_type) +
				Player_Elixir_Bonuses[pnum].damage_type_bonus[talent_type];
		if(temp) {
			dmg = dmg * (100 + temp) / 100;
			
			InsertCacheFactor(pnum, wepid, dmgid, temp);
		}
		
		// slot damage bonuses
		temp = GetPlayerAttributeValue(pnum, INV_SLOT1_DAMAGE + GetWeaponSlotFromFlag(flags));
		if(temp) {
			dmg = dmg * (100 + temp) / 100;
			InsertCacheFactor(pnum, wepid, dmgid, temp);
		}
		
		// special damage increase attributes -- usually obtained by means of charms
		temp = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_LIGHTNING);
		if(temp && IsWeaponLightningType(wepid, dmgid, isSpecial)) {
			dmg = dmg * (100 + temp) / 100;
			InsertCacheFactor(pnum, wepid, dmgid, temp);
		}
		
		// shotgun damage bonus -- add hobos perk here too
		temp = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_SHOTGUNS);
		if(CheckInventory("Hobo_Perk25"))
			temp += DND_HOBO_SHOTGUNBONUS;
			
		if((flags & DND_WDMG_ISBOOMSTICK) && temp) {
			dmg = dmg * (100 + temp) / 100;
			InsertCacheFactor(pnum, wepid, dmgid, temp);
		}
		// apply flat health to damage conversion if player has any
		temp = GetPlayerAttributeValue(pnum, INV_EX_DAMAGEPER_FLATHEALTH);
		if(temp) {
			temp = GetFlatHealthDamageFactor(temp);
			dmg = dmg * (100 + temp) / 100;
		
			InsertCacheFactor(pnum, wepid, dmgid, temp);
		}

		MarkCachingComplete(pnum, wepid, dmgid);
		
		//printbold(s:"pre-scale: ", d:temp);
	}
	else {
		// Get the cached flat dmg and factor and apply them both
		dmg += GetCachedPlayerFlatDamage(pnum, wepid, dmgid);
		
		// if there would be an overflow with dmg x temp (temp in [1.0, 65536.0], fixed)
		// scale each part individually then add it up to form the new number -- 1000 seems to be a nice value for doom ranges
		// but in general, the best result comes from an evenly split value
		temp = GetCachedPlayerFactor(pnum, wepid, dmgid);
		if(dmg < INT_MAX / temp) {
			dmg *= temp;
			dmg >>= 16;
		}
		else {
			// beyond this point wepid doesnt matter so use that instead
			dmg = BigNumberFormula(dmg, temp);
		}
		
		// factor was stored as fixed, convert to int
		//printbold(s:"from cache ", d:GetCachedPlayerFlatDamage(pnum, wepid, dmgid), s: " ", f:GetCachedPlayerFactor(pnum, wepid, dmgid));
	}
	
	//printbold(d:dmg);
	
	return dmg;
}

int ConfirmedCritFactor(int dmg) {
	dmg = dmg * GetCritModifier() / 100;
	HandleHunterTalisman();
	return dmg;
}

// handles crit factor after a crit is confirmed
int HandleWeaponCrit(int dmg, int wepid, int pnum, int dmgid, bool isSpecial) {
	// skip crit calc
	if(PlayerCritState[pnum][DND_CRITSTATE_NOCALC][wepid])
		return dmg;
	// if weapon id is a lightning type, it will always crit with the necessary charm attribute on
	return dmg;
}

void HandleDamagePush(int dmg, int ox, int oy, int oz, int victim) {
	// get push vector
	int dx, dy, dz;
	int m = GetActorProperty(victim, APROP_MASS) / 2;
	if(!m)
		m = 1;
	
	dx =  GetActorX(victim) - ox;
	dy =  GetActorY(victim) - oy;
	dz =  GetActorZ(victim) + GetActorProperty(victim, APROP_HEIGHT) / 2 + 8.0 - oz;
	
	int len = magnitudeThree(dx >> 16, dy >> 16, dz >> 16);
	if(!len)
		len = 1;
		
	dx /= len;
	dy /= len;
	dz /= len;
	
	dx *= dmg;		dx /= m;
	dy *= dmg;		dy /= m;
	dz *= dmg;		dz /= m;
	
	//printbold(s:"impulse: ", d:dmg, s:", force vector: ", f:dx, s:" ", f:dy, s: " ", f:dz);
	dx = Clamp_Between(dx, -DND_DMGPUSH_CAP, DND_DMGPUSH_CAP);
	dy = Clamp_Between(dy, -DND_DMGPUSH_CAP, DND_DMGPUSH_CAP);
	dz = Clamp_Between(dz, -DND_DMGPUSH_CAP, DND_DMGPUSH_CAP);
	
	SetActorVelocity(victim, dx, dy, dz, true, false);
}

// there may be things that add + to cull % later
bool CheckCullRange(int source, int victim, int dmg) {
	return GetActorProperty(victim, APROP_HEALTH) - dmg <= MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp / DND_CULL_BASEPERCENT;
}

void HandleChillEffects(int victim) {
	if(!MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_FRIGID]) {
		// check health thresholds --- get missing health
		int hpdiff = MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp - GetActorProperty(victim, APROP_HEALTH);
		int stacks = CheckActorInventory(victim, "DnD_ChillStacks");
		if(hpdiff >= (MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp / (DND_BASE_CHILL_DAMAGETHRESHOLD * (100 + CheckInventory("IATTR_ChillThreshold")) / 100)) * (stacks + 1)) {
			// add a new stack of chill and check for freeze
			if(!stacks) {
				GiveActorInventory(victim, "DnD_ChillStacks", 1);
				ACS_NamedExecuteWithResult("DnD Monster Chill", victim);
			}
			else if(stacks < DND_BASE_CHILL_CAP)
				GiveActorInventory(victim, "DnD_ChillStacks", 1);
			
			// freeze checks --- added freeze chance % increase
			hpdiff = DND_BASE_FREEZECHANCE_PERSTACK * CheckActorInventory(victim, "DnD_ChillStacks") * (100 + CheckInventory("IATTR_FreezeChance")) / 100;
			if(random(1, 100) <= hpdiff) {
				if(GetActorProperty(victim, APROP_HEALTH) > 0) {
					// is boss? half duration
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

void HandleIgniteEffects(int victim) {
	if(!MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_SCORCHED] && random(1, 100) <= DND_BASE_IGNITECHANCE * (100 + CheckInventory("IATTR_IgniteChance")) / 100) {
		int amt = DND_BASE_IGNITETIMER * (100 + CheckInventory("IATTR_IgniteDuration") + CheckInventory("IATTR_DotDuration")) / 100;
		if(!CheckActorInventory(victim, "DnD_IgniteTimer")) {
			SetActorInventory(victim, "DnD_IgniteTimer", amt);
			ACS_NamedExecuteWithResult("DnD Monster Ignite", victim);
		}
		else
			SetActorInventory(victim, "DnD_IgniteTimer", amt);
	}
}

void HandleOverloadEffects(int victim) {
	if(!MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_INSULATED] && random(1, 100) <= DND_BASE_OVERLOADCHANCE * (100 + CheckInventory("IATTR_OverloadChance")) / 100 && IsActorAlive(victim)) {
		if(!CheckActorInventory(victim, "DnD_OverloadTimer")) {
			SetActorInventory(victim, "DnD_OverloadTimer", DND_BASE_OVERLOADTIME);
			ACS_NamedExecuteWithResult("DnD Monster Overload", victim);
		}
		else
			SetActorInventory(victim, "DnD_OverloadTimer", DND_BASE_OVERLOADTIME);
	}
}

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
	return damage_type >= DND_DAMAGETYPE_OCCULT && damage_type <= DND_DAMAGETYPE_OCCULTEXPLOSION;
}

bool IsElementalDamage(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_FIRE && damage_type <= DND_DAMAGETYPE_LIGHTNING;
}

bool IsPoisonDamage(int damage_type) {
	return damage_type >= DND_DAMAGETYPE_POISON && damage_type <= DND_DAMAGETYPE_EMERALD;
}

int GetDamageCategory(int damage_type) {
	if(IsBulletDamage(damage_type))
		return DND_DAMAGECATEGORY_BULLET;
	else if(IsMeleeDamage(damage_type))
		return DND_DAMAGECATEGORY_MELEE;
	else if(damage_type == DND_DAMAGETYPE_EXPLOSIVES)
		return DND_DAMAGECATEGORY_EXPLOSIVES;
	else if(IsEnergyDamage(damage_type))
		return DND_DAMAGECATEGORY_ENERGY;
	else if(IsOccultDamage(damage_type))
		return DND_DAMAGECATEGORY_OCCULT;
	else if(IsElementalDamage(damage_type))
		return DND_DAMAGECATEGORY_ELEMENTAL;
	return DND_DAMAGECATEGORY_SOUL;
}

int GetResistPenetration(int category) {
	switch(category) {
		case DND_DAMAGECATEGORY_BULLET:
		case DND_DAMAGECATEGORY_MELEE:
		return CheckInventory("IATTR_PhysPen");
		case DND_DAMAGECATEGORY_ENERGY:
		return CheckInventory("IATTR_EnergyPen");
		case DND_DAMAGECATEGORY_EXPLOSIVES:
		return CheckInventory("IATTR_ExplosivePen");
		case DND_DAMAGECATEGORY_OCCULT:
		return CheckInventory("IATTR_OccultPen");
		case DND_DAMAGECATEGORY_ELEMENTAL:
		return CheckInventory("IATTR_ElementalPen");
		case DND_DAMAGECATEGORY_SOUL:
		return CheckInventory("IATTR_OccultPen") + CheckInventory("IATTR_SoulPenetration");
	}
	return 0;
}

int FactorResists(int source, int victim, int dmg, int damage_type, bool forced_full, int wepid) {
	// check penetration stuff on source -- set it accordingly to damage type being checked down below
	int mon_id = victim - DND_MONSTERTID_BEGIN;
	int temp = GetDamageCategory(damage_type);
	int pen = GetResistPenetration(temp);
	
	// weaknesses
	if(MonsterProperties[mon_id].trait_list[DND_ENERGY_WEAKNESS] && temp == DND_DAMAGECATEGORY_ENERGY)
		dmg = dmg * (100 + DND_WEAKNESS_FACTOR + pen) / 100;
	else if(MonsterProperties[mon_id].trait_list[DND_FIRE_WEAKNESS] && damage_type == DND_DAMAGETYPE_FIRE)
		dmg = dmg * (100 + DND_SPECIFICELEWEAKNESS_FACTOR + pen) / 100;
	else if(MonsterProperties[mon_id].trait_list[DND_ICE_WEAKNESS] && damage_type == DND_DAMAGETYPE_ICE)
		dmg = dmg * (100 + DND_SPECIFICELEWEAKNESS_FACTOR + pen) / 100;
	else if(MonsterProperties[mon_id].trait_list[DND_MAGIC_WEAKNESS] && (temp == DND_DAMAGECATEGORY_OCCULT || damage_type == DND_DAMAGETYPE_MELEEOCCULT))
		// if melee's sub type is occult then let it benefit from the pen
		dmg = dmg * (100 + DND_WEAKNESS_FACTOR + pen) / 100;
	else if(MonsterProperties[mon_id].trait_list[DND_ELEMENTAL_WEAKNESS] && temp == DND_DAMAGECATEGORY_ELEMENTAL)
		dmg = dmg * (100 + DND_WEAKNESS_FACTOR + pen) / 100;
	
	// special bonuses for certain creature types
	if(MonsterProperties[mon_id].trait_list[DND_ICECREATURE]) {
		// make sure to check ice and fire pen seperate
		if(damage_type == DND_DAMAGETYPE_ICE)
			dmg = dmg * (100 - DND_IMMUNITY_FACTOR + pen) / 100;
		else if(damage_type == DND_DAMAGETYPE_FIRE)
			dmg = dmg * (100 + DND_SPECIFICELEWEAKNESS_FACTOR + pen) / 100;
	}
	else if(MonsterProperties[mon_id].trait_list[DND_FIRECREATURE]) {
		// make sure to check ice and fire pen seperate
		if(damage_type == DND_DAMAGETYPE_FIRE)
			dmg = dmg * (100 - DND_IMMUNITY_FACTOR + pen) / 100;
		else if(damage_type == DND_DAMAGETYPE_ICE)
			dmg = dmg * (100 + DND_SPECIFICELEWEAKNESS_FACTOR + pen) / 100;
	}
	else if(MonsterProperties[mon_id].trait_list[DND_STONECREATURE]) {
		// make sure to check these seperate
		if(damage_type == DND_DAMAGETYPE_FIRE)
			dmg = dmg * (100 - DND_RESIST_FACTOR + pen) / 100;
		else if(damage_type == DND_DAMAGETYPE_ICE)
			dmg = dmg * (100 + DND_SPECIFICELEWEAKNESS_FACTOR + pen) / 100;
		else if(damage_type == DND_DAMAGETYPE_LIGHTNING || damage_type == DND_DAMAGETYPE_POISON)
			dmg = (dmg * pen) / 100;
	}
	else if(MonsterProperties[mon_id].trait_list[DND_EARTHCREATURE]) {
		// make sure to check these seperate
		if(damage_type == DND_DAMAGETYPE_LIGHTNING)
			dmg = dmg * (100 + DND_SPECIFICELEWEAKNESS_FACTOR + pen) / 100;
		else if(damage_type == DND_DAMAGETYPE_POISON)
			dmg = (dmg * pen) / 100;
	}
	
	// addition of pen here means if we ignored resists and immunities, we still give penetration a chance to weaken the defences further
	// return early as we ignored resists and immunities
	if(PlayerCritState[PlayerNumber()][DND_CRITSTATE_CONFIRMED][wepid] && CheckInventory("IATTR_CritIgnoreRes") >= random(1, 100))
		return dmg * (100 + pen) / 100;
		
	// if we do forced full damage skip resists and immunities
	if(forced_full) {
		if(pen)
			return dmg * (100 + pen) / 100;
		return dmg;
	}
		
	// resists from here on -- could be nicely tidied up with some array lining up but I dont really want to bother with that right now -- some more careful organization could be better later down the line
	if(MonsterProperties[mon_id].trait_list[DND_EXPLOSIVE_RESIST] && damage_type == DND_DAMAGETYPE_EXPLOSIVES)
		dmg = dmg * (100 - DND_RESIST_FACTOR + pen) / 100;
	else if(MonsterProperties[mon_id].trait_list[DND_BULLET_RESIST] && (temp == DND_DAMAGECATEGORY_BULLET || temp == DND_DAMAGECATEGORY_MELEE))
		dmg = dmg * (100 - DND_RESIST_FACTOR + pen) / 100;
	else if(MonsterProperties[mon_id].trait_list[DND_ENERGY_RESIST] && temp == DND_DAMAGECATEGORY_ENERGY)
		dmg = dmg * (100 - DND_RESIST_FACTOR + pen) / 100;
	else if(MonsterProperties[mon_id].trait_list[DND_MAGIC_RESIST] && (temp == DND_DAMAGECATEGORY_OCCULT ||damage_type == DND_DAMAGETYPE_SOUL))
		dmg = dmg * (100 - DND_RESIST_FACTOR + pen) / 100;
	else if(MonsterProperties[mon_id].trait_list[DND_ELEMENTAL_RESIST] && temp == DND_DAMAGECATEGORY_ELEMENTAL)
		dmg = dmg * (100 - DND_RESIST_FACTOR + pen) / 100;
	// immunities
	else if(MonsterProperties[mon_id].trait_list[DND_EXPLOSIVE_NONE] && damage_type == DND_DAMAGETYPE_EXPLOSIVES)
		dmg = dmg * (100 - DND_IMMUNITY_FACTOR + pen) / 100;
	else if(MonsterProperties[mon_id].trait_list[DND_BULLET_IMMUNE] && (temp == DND_DAMAGECATEGORY_BULLET || temp == DND_DAMAGECATEGORY_MELEE))
		dmg = dmg * (100 - DND_IMMUNITY_FACTOR + pen) / 100;
	else if(MonsterProperties[mon_id].trait_list[DND_ENERGY_IMMUNE] && temp == DND_DAMAGECATEGORY_ENERGY)
		dmg = dmg * (100 - DND_IMMUNITY_FACTOR + pen) / 100;
	else if(MonsterProperties[mon_id].trait_list[DND_MAGIC_IMMUNE] && (temp == DND_DAMAGECATEGORY_OCCULT ||damage_type == DND_DAMAGETYPE_SOUL))
		dmg = dmg * (100 - DND_IMMUNITY_FACTOR + pen) / 100;
	else if(MonsterProperties[mon_id].trait_list[DND_ELEMENTAL_IMMUNE] && temp == DND_DAMAGECATEGORY_ELEMENTAL)
			dmg = dmg * (100 - DND_IMMUNITY_FACTOR + pen) / 100;
	
	return dmg;
}

// returns the filtered, reduced etc. damage when factoring in all resists or weaknesses ie. this is the final damage the actor will take
// This is strictly for player doing damage to other monsters!
void HandleDamageDeal(int source, int victim, int dmg, int damage_type, int flags, int ox, int oy, int oz, int actor_flags, int extra = 0, int poison_factor = 0) {
	str s_damagetype = DamageTypeList[damage_type];
	bool forced_full = false;
	bool no_ignite_stack = flags & DND_DAMAGEFLAG_NOIGNITESTACK;
	int temp;
	int pnum = -1;
	
	// extract imported wepid
	int wepid = actor_flags >> 16;
	actor_flags &= 0xFFFF;
	
	if(isPlayer(source))
		pnum = source - P_TIDSTART;
	
	// for now, just add full suffix at the end, later we'll instead make them bypass resists properly with numbers
	if(
		CheckActorInventory(source, "NetherCheck") 																	|| 
		(flags & DND_DAMAGEFLAG_DOFULLDAMAGE)																		||
		(IsOccultDamage(damage_type) && CheckInventory("DnD_QuestReward_DreamingGodBonus"))							||
		(IsEnergyDamage(damage_type) && CheckInventory("Cyborg_Perk50")) 											||
		(IsExplosionDamage(damage_type) && CheckInventory("StatbuffCounter_ExplosiveResistIgnore"))					||
		(damage_type == DND_DAMAGETYPE_SOUL && CheckInventory("StatbuffCounter_SoulWepsDoFullDamage"))				||
		((flags & DND_DAMAGEFLAG_ISSHOTGUN) && CheckInventory("Hobo_Perk50"))
	) 
	{
		// no need for this suffix anymore
		//s_damagetype = StrParam(s:s_damagetype, s:"Full");
		forced_full = true;
	}

	// check blocking status of monster -- if they are and we dont have foilinvul on this, no penetration
	if(MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_ISBLOCKING] && !(actor_flags & DND_ACTORFLAG_FOILINVUL)) {
		temp = CheckInventory("IATTR_ChanceIgnoreShield");
		// we have 0 chance  or we have chance but it didn't roll in our favor
		if(!temp || temp < random(1, 100)) {
			if(pnum != -1)
				ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_INVULNERABLE);
			return;
		}
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
			dmg = dmg * (100 + temp * DND_DESOLATOR_DMG_GAIN) / 100;
	}
	
	// handle resists and all that here
	//printbold(s:"res calc");
	temp = dmg;
	dmg = FactorResists(source, victim, dmg, damage_type, forced_full, wepid);
	// if more that means we hit a weakness, otherwise below conditions check immune and resist respectively
	if(pnum != -1) {
		if(dmg > temp)
			ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_WEAKNESS);
		else if(dmg < temp / 4)
			ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_IMMUNITY);
		else if(dmg < temp)
			ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_RESIST);
	}
	
	if((flags & DND_DAMAGEFLAG_CULL) && CheckCullRange(source, victim, dmg)) {
		GiveActorInventory(victim, "DnD_CullSuccess", 1);
		
		if(flags & DND_DAMAGEFLAG_SELFCULL)
			Thing_Destroy(victim, false, 0);
	}
	// handle poison checks
	// printbold(d:damage_type, s: " ", d:IsPoisonDamage(damage_type), s: " ", d:!(flags & DND_DAMAGEFLAG_NOPOISONSTACK), s: " ", d:flags);
	if((IsPoisonDamage(damage_type) || (flags & DND_DAMAGEFLAG_INFLICTPOISON)) && !(flags & DND_DAMAGEFLAG_NOPOISONSTACK) && !MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_VENOMANCER]) {
		// poison damage deals 10% of its damage per stack over 3 seconds
		if(CheckActorInventory(victim, "DnD_PoisonStacks") < DND_BASE_POISON_STACKS && dmg > 0) {
			GiveActorInventory(victim, "DnD_PoisonStacks", 1);
			// 2% of damage
			if(poison_factor)
				ACS_NamedExecuteWithResult("DnD Do Poison Damage", victim, (dmg * poison_factor) / 100);
			else
				ACS_NamedExecuteWithResult("DnD Do Poison Damage", victim, dmg / 50);
			//printbold(s:"poison received by ", d:victim);
		}
	}
	
	if((flags & DND_DAMAGEFLAG_EXTRATOUNDEAD) && (MonsterData[MonsterProperties[victim - DND_MONSTERTID_BEGIN].id].flags & (DND_MTYPE_UNDEAD_POW | DND_MTYPE_MAGICAL_POW)))
		dmg *= DND_EXTRAUNDEADDMG_MULTIPLIER;
	
	// final check on explosions if we are hurting ourselves, factor in our resists and stuff like that
	if(source == victim && pnum != -1) {
		if(CheckActorInventory(source, "Ability_ExplosionMastery"))
			dmg /= 2;
		// check our self damage reductions
		temp = Clamp_Between(GetPlayerAttributeValue(pnum, INV_EXPLOSIVE_RESIST), 0, MAX_EXPRESIST_VAL) + CheckActorInventory(source, "Marine_Perk5") * DND_MARINE_SELFRESIST;
		if(temp)
			dmg = dmg * (MAX_EXPRESIST_VAL - temp) / MAX_EXPRESIST_VAL;
	}
	
	// check blockers take more dmg modifier
	if(MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_ISBLOCKING]) {
		temp = CheckInventory("IATTR_BlockDmg");
		if(temp)
			dmg = dmg * (100 + temp) / 100;
	}
	
	if(CheckActorInventory(victim, "DnD_OverloadTimer")) {
		if(damage_type != DND_DAMAGETYPE_LIGHTNING)
			dmg = dmg * (100 + DND_BASE_OVERLOADBUFF) / 100;
		GiveActorInventory(victim, "DnD_OverloadDamage", dmg);
	}
	
	if(flags & DND_DAMAGEFLAG_PERCENTHEALTH) {
		dmg += (MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp * extra) / 100;
		// printbold(s:"% bonus: ", d:extra, s:" ", d:(MonsterProperties[victim - DND_MONSTERTID_BEGIN].maxhp * extra) / 100);
	}
	
	// damage number handling
	// printbold(s:"apply ", d:dmg, s: " of type ", s:s_damagetype, s: " pnum: ", d:pnum);
	if(pnum != -1) {
		// this part handles damage pushing
		temp = victim - DND_MONSTERTID_BEGIN;
		
		// flags represent the flag list of damageticflag
		flags = 0;
		if(!(actor_flags & DND_ACTORFLAG_NOPUSH))
			flags |= DND_DAMAGETICFLAG_PUSH;
		
		if(actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT)
			flags |= DND_DAMAGETICFLAG_CRIT;
			
		if(actor_flags & DND_ACTORFLAG_COUNTSASMELEE)
			flags |= DND_DAMAGETICFLAG_CONSIDERMELEE;
		
		// we send particular damage types in that can cause certain status effects like chill, freeze etc.
		if(damage_type == DND_DAMAGETYPE_ICE)
			flags |= DND_DAMAGETICFLAG_ICE;
		else if(damage_type == DND_DAMAGETYPE_FIRE && !no_ignite_stack)
			flags |= DND_DAMAGETICFLAG_FIRE;
		else if(damage_type == DND_DAMAGETYPE_LIGHTNING)
			flags |= DND_DAMAGETICFLAG_LIGHTNING;

		if(!PlayerDamageTicData[pnum][temp]) {
			PlayerDamageVector[pnum].x = ox;
			PlayerDamageVector[pnum].y = oy;
			PlayerDamageVector[pnum].z = oz;
			
			ACS_NamedExecuteWithResult("DnD Damage Accumulate", temp | (flags << DND_DAMAGE_ACCUM_SHIFT), wepid);
		}
		PlayerDamageTicData[pnum][temp] += dmg;
	}
	
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
	
	if(dmg > 0)
		Thing_Damage2(victim, dmg, s_damagetype);
	
	if(!isActorAlive(victim) && CheckActorInventory(source, "Berserker_Perk50")) {
		SetActorInventory(source, "Berserker_HitTimer", DND_BERSERKER_PERK50_TIMER);
		if((temp = CheckActorInventory(source, "Berserker_HitTracker")) < DND_BERSERKER_PERK50_MAXSTACKS) {
			GiveActorInventory(source, "Berserker_HitTracker", 1);
			if(!temp)
				ACS_NamedExecuteAlways("DnD Berserker Perk50 Timer", 0, source);
		}
		if(temp + 1 >= DND_BERSERKER_PERK50_MAXSTACKS) {
			if(!CheckActorInventory(source, "Berserker_RoarCD") && !CheckActorInventory(source, "Berserker_NoRoar"))
				HandleBerserkerRoar(source);
			GiveActorInventory(source, "Berserker_Perk50_Speed", 1);
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
			res = (res * FixedDiv(radius - dist, radius - fullradius)) >> 16;
		}
	}
	return res;
}

void DoExplosionDamage(int owner, int dmg, int radius, int fullradius, int damage_type, int wepid) {
	int pnum = owner - P_TIDSTART;
	// extract flags off damage type
	int flags = damage_type >> 16;
	damage_type &= 0xFFFF;
	
	int instance = PlayerExplosionList[pnum].curr_instance;
	int lim = PlayerExplosionList[pnum].list[instance].amt;
		
	int actor_flags = ScanActorFlags();
	
	int px = GetActorX(0), py = GetActorY(0), pz = GetActorZ(0);
	// printbold(s:"Explosion owner: ", d:owner);
	
	int proj_r = GetActorProperty(0, APROP_RADIUS);
	int final_dmg;

	// set activator to player for dmg registry
	SetActivator(P_TIDSTART + pnum);
	
	// turn them to fixed
	radius <<= 16;
	fullradius <<= 16;
	for(int i = 0; i < lim; ++i) {
		int mon_id = PlayerExplosionList[pnum].list[instance].monsters[i];

		// first check if this monster is immune to splash damage (its an easy flag check and eases calculation later)
		// if we have nethermask simply ignore all these checks anyways
		if(!CheckFlag(mon_id, "SHOOTABLE"))
			continue;
			
		if(!CheckActorInventory(owner, "NetherCheck")) {
			if
			(
				(CheckFlag(mon_id, "NORADIUSDMG") && (!CheckActorInventory(owner, "Marine_Perk25") || !(flags & DND_DAMAGEFLAG_FORCERADIUSDMG))) ||
				(wepid == DND_WEAPON_SEDRINSTAFF && IsActorFullRobotic(mon_id))
			)
				continue;
		}
		
		final_dmg = ScaleExplosionToDistance(mon_id, dmg, radius, fullradius, px, py, pz, proj_r);
		
		if(final_dmg == -1)
			continue;
		
		// dont deal 0 dmg
		if(!final_dmg)
			final_dmg = 1;
		
		HandleDamageDeal(owner, mon_id, final_dmg, damage_type, flags, px, py, pz, actor_flags);
		
		//printbold(s:"Dealing ", d: final_dmg, s: " damage to ", d:mon_id, s: " of type ", s:DamageTypeList[damage_type]);
	}
	
	// finally check player, if we are close to our own explosion with self dmg flag, hurt us too
	if(flags & DND_DAMAGEFLAG_BLASTSELF) {
		// we are the owner here at this point, we can use 0 for ourselves
		
		// sedrin staff armor check
		// if not sedrin staff, immediately check
		// if sedrin staff and if we have armor, both are false so no damage to us
		if(wepid != DND_WEAPON_SEDRINSTAFF || !CheckActorInventory(owner, "Armor")) {
			final_dmg = ScaleExplosionToDistance(owner, dmg, radius, fullradius, px, py, pz, proj_r);
			if(final_dmg != -1) {
				// final dmg cant be negative -- apply penetration and self explosion resist here
				final_dmg = final_dmg * (100 - CheckInventory("IATTR_ExplosionResist") + CheckInventory("IATTR_ExplosivePen")) / 100;
				if(final_dmg < 0)
					final_dmg = 0;
				HandleDamageDeal(owner, owner, final_dmg, damage_type, flags, px, py, pz, actor_flags);
			}
		}
	}
	
	// damage is dealt we are done with this instance, free it up
	Free_Explosion_Instance(pnum, instance);
}

Script "DnD Do Explosion Damage" (int dmg, int radius, int fullradius, int damage_type) {
	// player information
	int owner = GetActorProperty(0, APROP_TARGETTID);
	if(!isPlayer(owner))
		owner = GetActorProperty(0, APROP_SCORE);
	
	int wepid = GetUserVariable(0, DND_WEPID_USERVARIABLE);
	
	DoExplosionDamage(owner, dmg, radius, fullradius, damage_type, wepid);
	
	SetResultValue(0);
}

Script "DnD Do Explosion Damage (Pets)" (int dmg, int radius, int fullradius, int damage_type) {
	// player information
	int owner = CheckInventory("DnD_ProjPnum") + P_TIDSTART;
	int wepid = GetUserVariable(0, DND_WEPID_USERVARIABLE);
	
	DoExplosionDamage(owner, dmg, radius, fullradius, damage_type, wepid);
	
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

void HandleImpactDamage(int owner, int victim, int dmg, int damage_type, int flags, int wepid) {
	int px, py, pz;

	if(flags & DND_DAMAGEFLAG_DISTANCEGIVESDAMAGE)
		dmg += GetUserVariable(0, DND_DISTANCEDAMAGE_VARIABLE);
		
	// spawn special blood particles attached to monsters
	if((flags & DND_DAMAGEFLAG_ISHITSCAN) && CheckFlag(victim, "NOBLOOD")) {
		if(MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_STONECREATURE])
			ACS_NamedExecuteWithResult("DnD Spawn Bloodtype", DND_SPECIALBLOOD_STONE);
	}
	
	if(wepid == -1)
		wepid = CheckActorInventory(owner, "DnD_WeaponID");
		
	int actor_flags = ScanActorFlags();
		
	if(flags & DND_DAMAGEFLAG_FOILINVUL) {
		actor_flags |= DND_ACTORFLAG_FOILINVUL;
		flags ^= DND_DAMAGEFLAG_FOILINVUL;
	}
	
	if(flags & DND_DAMAGEFLAG_COUNTSASMELEE) {
		actor_flags |= DND_ACTORFLAG_COUNTSASMELEE;
		flags ^= DND_DAMAGEFLAG_COUNTSASMELEE;
	}
		
	// berserker perk50 dmg increase portion and other melee increases
	if((IsMeleeWeapon(wepid) || actor_flags & DND_ACTORFLAG_COUNTSASMELEE)) {
		if(CheckActorInventory(owner, "Berserker_Perk50")) {
			SetActorInventory(owner, "Berserker_HitTimer", DND_BERSERKER_PERK50_TIMER);
			if((px = CheckActorInventory(owner, "Berserker_HitTracker")) < DND_BERSERKER_PERK50_MAXSTACKS) {
				GiveActorInventory(owner, "Berserker_HitTracker", 1);
				if(!px)
					ACS_NamedExecuteAlways("DnD Berserker Perk50 Timer", 0, owner);
			}
			if(px + 1 >= DND_BERSERKER_PERK50_MAXSTACKS) {
				if(!CheckActorInventory(owner, "Berserker_RoarCD") && !CheckActorInventory(owner, "Berserker_NoRoar"))
					HandleBerserkerRoar(owner);
				GiveActorInventory(owner, "Berserker_Perk50_Speed", 1);
			}
			dmg += dmg * (100 + (px + 1) * DND_BERSERKER_PERK50_DMGINCREASE) / 100;
		}
		
		dmg += dmg * (100 + GetActorAttributeValue(owner, INV_MELEEDAMAGE)) / 100;
	}
	
	if(flags & DND_DAMAGEFLAG_ISHITSCAN) {
		if(!isActorAlive(victim)) {
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
			LineAttack(0, ang, pitch, 0, puff, DamageTypeList[damage_type], 2048.0, FHF_NORANDOMPUFFZ, 0);
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
		px = GetActorX(0);
		py = GetActorY(0);
		pz = GetActorZ(0);
	}
	
	// printbold(d:owner, s: " ", d:victim);
	if(GetActorProperty(0, APROP_ACCURACY) == DND_CRIT_TOKEN) {
		actor_flags |= DND_ACTORFLAG_CONFIRMEDCRIT;
		actor_flags |= (wepid << 16);
	}
	
	// Flayer magic or undead check
	if(wepid == DND_WEAPON_CROSSBOW && IsActorMagicOrUndead(victim))
		ACS_NamedExecuteWithResult("DnD Crossbow Explosion", victim, owner);
	
	SetActivator(owner);
	int pnum = PlayerNumber();
	int extra = Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_PERCENTDAMAGE].val;

	// percent damage of monster if it exists
	if(extra)
		flags |= DND_DAMAGEFLAG_PERCENTHEALTH;
		
	// chance to force pain
	if(Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_FORCEPAINCHANCE].val && Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_FORCEPAINCHANCE].val > random(1, 100)) {
		actor_flags |= DND_ACTORFLAG_FORCEPAIN;
	}
	
	if(Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_POISONFORPERCENTDAMAGE].val)
		flags |= DND_DAMAGEFLAG_INFLICTPOISON;
		
	if(PlayerCritState[pnum][DND_CRITSTATE_CONFIRMED][wepid] && !(actor_flags & DND_ACTORFLAG_CONFIRMEDCRIT)) {
		// the wepid is stored as reference in the last 16 bits
		actor_flags |= DND_ACTORFLAG_CONFIRMEDCRIT;
		actor_flags |= (wepid << 16);
	}
	
	// printbold(d:dmg);
	if(owner && victim)
		HandleDamageDeal(owner, victim, dmg, damage_type, flags, px, py, pz, actor_flags, extra, Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_POISONFORPERCENTDAMAGE].val);
}

Script "DnD Do Impact Damage" (int dmg, int damage_type, int flags, int wepid) {
	int owner = GetActorProperty(0, APROP_TARGETTID);
	int victim = GetActorProperty(0, APROP_TRACERTID);
	
	// sedrin check
	if(!CheckActorInventory(owner, "NetherCheck") && wepid == DND_WEAPON_SEDRINSTAFF && IsActorFullRobotic(victim)) {
		SetActivator(owner);
		ACS_NamedExecuteAlways("DnD Handle Hitbeep", 0, DND_HITBEEP_IMMUNITY);
	}
	else
		HandleImpactDamage(owner, victim, dmg, damage_type, flags, wepid);
	
	SetResultValue(0);
}

// has embedded data
Script "DnD Do Impact Damage Ripper" (int dmg, int damage_type, int flags, int wepid) {
	//printbold(s:"FUCKING HURT ", d:damage_type >> DAMAGE_TYPE_SHIFT);
	HandleImpactDamage(GetActorProperty(0, APROP_TARGETTID), damage_type >> DAMAGE_TYPE_SHIFT, dmg, damage_type & DAMAGE_TYPE_MASK, flags, wepid);
	
	SetResultValue(0);
}

void HandleRipperHitSound(int tid, int owner, int wepid) {
	switch(wepid) {
		case DND_WEAPON_CROSSBOW:
			PlaySound(tid, "Crossbow/Hit", 5, 1.0);
		break;
		case DND_WEAPON_DARKLANCE:
			// stack building on kill
			if(!isActorAlive(tid))
				GiveActorInventory(owner, "LanceStacks", 1);
		break;
	}
}

// to be used sparingly, it scans all monsters at all times since we dont have linetraces
Script "DnD One Time Ripper" (int dmg, int damage_type, int flags, int wepid) {
	int owner = GetActorProperty(0, APROP_TARGETTID);
	int r = GetActorProperty(0, APROP_RADIUS) >> 16;
	int h = GetActorProperty(0, APROP_HEIGHT);
	int i = 0, m = 0, s = 0;
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
	
	// get projectile dir, and imagine as if the projectile is stepping forwards from its location
	// this is so faster projectiles are predicted -- we get dir, calculate just how many steps it'd take by speed / radius, then iterate the box over
	int dir_x = GetActorVelX(0);
	int dir_y = GetActorVelY(0);
	int dir_z = GetActorVelZ(0);
	int len = VectorLength3d(dir_x, dir_y, dir_z) >> 16;
	if(!len)
		len = 1;
	
	dir_x /= len;
	dir_y /= len;
	dir_z /= len;
	
	// projectiles spawn speed units ahead of player, this is especially noticable in faster projectiles
	// we must check backwards initially for point blank case
	// find monsters in a rectangle from actor xyz, +-r * cos / sin and +-h on z
	// simple rectanglular box check from rectangle sides
	bool found = false;
	bool first_tic = true;
	int a_x, a_y, a_r;
	top_x = GetActorX(0) - (r << 16), top_y = GetActorY(0) - (r << 16), bot_x = GetActorX(0) + (r << 16), bot_y = GetActorY(0) + (r << 16);

	// start target picking
	while(GetActorVelX(0) || GetActorVelY(0) || GetActorVelZ(0)) {
		for(i = DND_MONSTERTID_BEGIN; i < DnD_TID_List[DND_TID_MONSTER]; ++i) {
			// dead, skip
			if(!isActorAlive(i))
				continue;
		
			found = false;
			a_x = GetActorX(i), a_y = GetActorY(i), a_r = GetActorProperty(i, APROP_RADIUS);
			
			int speed = GetActorProperty(0, APROP_SPEED);
			int steps = Max(speed, a_r + (r << 16));
			
			//printbold(s:"spd: ", f:speed);
		
			if(AproxDistance(GetActorX(0) - a_x, GetActorY(0) - a_y) > steps || ((actor_flags & DND_ACTORFLAG_THRUGHOST) && CheckFlag(i, "GHOST")))
				continue;
				
			steps = (speed / r) >> 16;
			if(steps < 2)
				steps = 2;
			
			for(s = 0; s < steps; ++s) {
				// eliminate cases where it'd fail to touch
				if(!first_tic) {
					if(bot_x + s * dir_x * r < a_x - a_r || a_x + a_r < top_x + s * dir_x * r || bot_y + s * dir_y * r < a_y - a_r || a_y + a_r < top_y + s * dir_y * r)
						continue;
				}
				else {
					// check front and back on tic 0, because projectile spawns speed units farther
					if(bot_x - s * dir_x * r < a_x - a_r || a_x + a_r < top_x - s * dir_x * r || bot_y - s * dir_y * r < a_y - a_r || a_y + a_r < top_y - s * dir_y * r) {
						// if s == 0, we dont need to check this so leave asap
						if(!s || bot_x + s * dir_x * r < a_x - a_r || a_x + a_r < top_x + s * dir_x * r || bot_y + s * dir_y * r < a_y - a_r || a_y + a_r < top_y + s * dir_y * r) {
							continue;
						}
					}
				}
				
				//printbold(s:"x-y valid on: ", f:a_x, s: " ", f:a_y, s:" ", f:top_x + s * dir_x * r, s: " ", f:top_y + s * dir_y * r, s: " ", f:bot_x + s * dir_x * r, s: " ", f:bot_y + s * dir_y * r);
				//printbold(s:"try z: ", f:GetActorZ(0) - h, s: " ", f:GetActorZ(0) + h, s: " ", f:GetActorZ(i), s: " ", f:GetActorZ(i) + GetActorProperty(i, APROP_HEIGHT));
				if(GetActorZ(0) - h / 2 > GetActorZ(i) + GetActorProperty(i, APROP_HEIGHT) || GetActorZ(0) + h / 2 < GetActorZ(i) || !CheckSight(0, i, 0))
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
		top_x = GetActorX(0) - (r << 16), top_y = GetActorY(0) - (r << 16), bot_x = GetActorX(0) + (r << 16), bot_y = GetActorY(0) + (r << 16);
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

void HandleLifesteal(int wepid, int flags, int dmg) {
	int taltos = (IsMeleeWeapon(wepid) || (flags & DND_DAMAGETICFLAG_CONSIDERMELEE)) && CheckInventory("TaltosUp");
	int brune_1 = CheckInventory("FakeBloodPower");
	int brune_2 = CheckInventory("FakeBloodPowerBetter");
	if(CheckInventory("IATTR_Lifesteal") || taltos || brune_1 || brune_2) {
		taltos = CheckInventory("IATTR_Lifesteal") + taltos * DND_TALTOS_LIFESTEAL + brune_1 * BLOODRUNE_LIFESTEAL_AMT + brune_2 * BLOODRUNE_LIFESTEAL_AMT2;
		
		// divide by 100 as its a percentage -- and >> 16 to make it int
		taltos /= 100;
		taltos *= dmg;
		taltos >>= 16;
		if(taltos)
			ACS_NamedExecuteAlways("DnD Health Pickup", 0, taltos);
	}
}

// ASSUMPTION: PLAYER RUNS THIS! -- care if adapting this later for other things
Script "DnD Damage Accumulate" (int victim_data, int wepid) {
	int pnum = PlayerNumber();
	int flags = victim_data >> DND_DAMAGE_ACCUM_SHIFT;
	victim_data &= DND_MONSTER_TICDATA_BITMASK;

	int ox = PlayerDamageVector[pnum].x;
	int oy = PlayerDamageVector[pnum].y;
	int oz = PlayerDamageVector[pnum].z;
	
	Delay(const:1);

	// do the real pushing after 1 tic of dmg data has been accumulated and we have non-zero damage in effect
	if((flags & DND_DAMAGETICFLAG_PUSH) && PlayerDamageTicData[pnum][victim_data] > 0)
		HandleDamagePush(PlayerDamageTicData[pnum][victim_data], ox, oy, oz, victim_data + DND_MONSTERTID_BEGIN);
		
	if(PlayerCritState[pnum][DND_CRITSTATE_CONFIRMED][wepid] || flags & DND_DAMAGETICFLAG_CRIT)
		PlayerCritState[pnum][DND_CRITSTATE_CONFIRMED][wepid] = false;
	
	PlayerCritState[pnum][DND_CRITSTATE_NOCALC][wepid] = false;
	
	// check if player has lifesteal, if they do reward some hp back
	if(!MonsterProperties[victim_data].trait_list[DND_BLOODLESS])
		HandleLifesteal(wepid, flags, PlayerDamageTicData[pnum][victim_data]);
	
	// if ice damage, add stacks of slow and check for potential freeze chance
	// do these if only the actor was alive after the tic they received dmg
	if(IsActorAlive(victim_data + DND_MONSTERTID_BEGIN)) {
		if(flags & DND_DAMAGETICFLAG_ICE)
			HandleChillEffects(victim_data + DND_MONSTERTID_BEGIN);
		else if(flags & DND_DAMAGETICFLAG_FIRE)
			HandleIgniteEffects(victim_data + DND_MONSTERTID_BEGIN);
		else if(flags & DND_DAMAGETICFLAG_LIGHTNING)
			HandleOverloadEffects(victim_data + DND_MONSTERTID_BEGIN);
		
		// frozen monsters cant retaliate		
		if(MonsterProperties[victim_data].trait_list[DND_VIOLENTRETALIATION] && random(1, 100) <= DND_VIOLENTRETALIATION_CHANCE && !CheckActorInventory(victim_data + DND_MONSTERTID_BEGIN, "DnD_FreezeTimer"))
			GiveActorInventory(victim_data + DND_MONSTERTID_BEGIN, "DnD_ViolentRetaliationItem", 1);
		GiveActorInventory(victim_data + DND_MONSTERTID_BEGIN, "DnD_HurtToken", 1);
	}
	
	// printbold(s:"do dmg numbers: ", d:pnum);
	ACS_NamedExecuteWithResult("DnD Damage Numbers", victim_data + DND_MONSTERTID_BEGIN, PlayerDamageTicData[pnum][victim_data], flags);
	
	// reset dmg counter on this mob
	PlayerDamageTicData[pnum][victim_data] = 0;
	
	SetResultValue(0);
}

Script "DnD Damage Numbers" (int tid, int dmg, int flags) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber() || !GetCVar("dnd_dmgnum"))
		Terminate;
	
	// if dmg is more than 9999 show using K instead
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

Script "DnD Do Poison Damage" (int victim, int dmg) {
	int time_limit = DND_BASE_POISON_TIMER * (100 + CheckInventory("IATTR_PoisonDuration") + CheckInventory("IATTR_DotDuration")) / 100;
	int trigger_tic = DND_BASE_POISON_TIC * (100 - CheckInventory("IATTR_PoisonTicrate")) / 100;
	int tic_temp = trigger_tic;
	int counter = 0;
	
	// divide trigger tic count by half to make it twice as fast -- if poison ticrate is 100% reduction we'll do poison damage at every 2 tics, which is the most one would need
	if(CheckInventory("StatbuffCounter_PoisonTicTwiceFast"))
		trigger_tic /= 2;

	// scale received percentage of damage with flat bonuses
	if(!dmg)
		dmg = 1;
	dmg += CheckInventory("IATTR_FlatPoisonDmg") + CheckInventory("IATTR_FlatDotDamage");
	dmg = dmg * (100 + CheckInventory("IATTR_PoisonTicDmg")) / 100;
		
	while(counter < time_limit && IsActorAlive(victim)) {
		if(counter >= trigger_tic) {
			HandleDamageDeal(ActivatorTID(), victim, dmg, DND_DAMAGETYPE_POISON, DND_DAMAGEFLAG_NOPOISONSTACK, 0, 0, 0, DND_ACTORFLAG_NOPUSH | DND_ACTORFLAG_FOILINVUL);
			ACS_NamedExecuteAlways("DnD Spawn Poison FX", 0, victim, CheckActorInventory(victim, "DnD_PoisonStacks"));
			
			// go up to the next threshold for next tic etc.
			trigger_tic += tic_temp;
		}
		counter += 0.1;
		Delay(const:2);
	}
	TakeActorInventory(victim, "DnD_PoisonStacks", 1);
	SetResultValue(0);
}

Script "DnD Spawn Poison FX" (int orig, int amt) CLIENTSIDE {
	int r = GetActorProperty(orig, APROP_RADIUS);
	int h = GetActorProperty(orig, APROP_HEIGHT);
	for(int i = 0; i <= amt; ++i) {
		SpawnForced("DnD_PoisonFX", GetActorX(orig) + random(-r, r), GetActorY(orig) + random(-r, r), GetActorZ(orig) + random(-h, h), 0);
		Delay(random(1, 3));
	}
}

Script "DnD Monster Chill" (int victim) {
	int cur_stacks;
	int base_speed = GetActorProperty(victim, APROP_SPEED);
	
	if(MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_EXTRAFAST])
		SetActorFlag(victim, "ALWAYSFAST", 0);
	
	while((cur_stacks = CheckActorInventory(victim, "DnD_ChillStacks"))) {
		// slow down
		SetActorProperty(victim, APROP_SPEED, (base_speed * (100 - cur_stacks * DND_BASE_CHILL_SLOW) / 100) * (100 + CheckInventory("IATTR_SlowEffect")) / 100);
		ACS_NamedExecuteAlways("DnD Monster Chill FX", 0, victim);
		Delay(const:TICRATE);
		TakeActorInventory(victim, "DnD_ChillStacks", 1);
	}
	
	SetActorProperty(victim, APROP_SPEED, base_speed);
	
	// retain super fast property after chill ends
	if(MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_EXTRAFAST])
		SetActorFlag(victim, "ALWAYSFAST", 1);
}

Script "DnD Monster Chill FX" (int tid) CLIENTSIDE {
	SetActivator(tid);
	for(int i = 0; i < 5; ++i) {
		Delay(const:7);
		if(random(0, 1))
			GiveInventory("DnD_ChillWindSpawner", 1);
	}
}

Script "DnD Monster Freeze" (int victim) {
	SetActorState(victim, "Frozen", 0);
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
	if(!MonsterProperties[victim - DND_MONSTERTID_BEGIN].trait_list[DND_NOPAIN])
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

Script "DnD Monster Ignite" (int victim) {
	int dmg = (DND_BASE_IGNITEDMG + CheckInventory("IATTR_FlatFireDmg") + CheckInventory("IATTR_FlatDotDamage")) * (100 + CheckInventory("IATTR_IgniteDmg")) / 100;
	int dmg_tic_buff = CheckInventory("IATTR_IgniteDamageEachTic");
	dmg = ACS_NamedExecuteWithResult("DND Player Damage Scale", dmg, TALENT_ELEMENTAL);
	
	while(CheckActorInventory(victim, "DnD_IgniteTimer") && IsActorAlive(victim)) {
		// x 5
		Delay(const:7);
		ACS_NamedExecuteAlways("DnD Monster Ignite FX", 0, victim);
		TakeActorInventory(victim, "DnD_IgniteTimer", 1);
		HandleDamageDeal(ActivatorTID(), victim, dmg, DND_DAMAGETYPE_FIRE, DND_DAMAGEFLAG_NOIGNITESTACK, 0, 0, 0, DND_ACTORFLAG_NOPUSH);
		if(random(0, 1))
			GiveActorInventory(victim, "DnD_IgniteFXSpawner", 1);
			
		dmg += (dmg * dmg_tic_buff) / 100;
	}
	
	SetActorInventory(victim, "DnD_IgniteTimer", 0);

	SetResultValue(0);
}

Script "DnD Monster Ignite FX" (int tid) CLIENTSIDE {
	SetActivator(tid);
	for(int i = 0; i < 2; ++i) {
		Delay(const:7);
		if(random(0, 1))
			GiveInventory("DnD_IgniteFXSpawner", 1);
	}
}

Script "DnD Monster Overload" (int victim) {
	// we dont have any player involvement here so
	SetActivator(victim);
	PlaySound(0, "Overload/Loop", CHAN_ITEM, 1.0, true);
	
	while(CheckInventory("DnD_OverloadTimer")) {
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

Script "DnD Monster Overload Zap" (int this, int killer) {
	if(!isPlayer(killer))
		Terminate;
	
	ActivatorSound("Overload/ZapBegin", 127);
	SpawnForced("OverloadZap_Source", GetActorX(this), GetActorY(this), GetActorZ(this) + GetActorProperty(this, APROP_HEIGHT) + 16.0, 0);
	
	// first look up potential targets and then store them, we'll zap later with some delay
	int pnum = killer - P_TIDSTART;
	
	int i;
	int zap_count = CheckActorInventory(killer, "IATTR_OverloadZapCount") + 1;
	int cur_count = 0;
	static int zap_tids[MAXPLAYERS][DND_MAX_OVERLOADTARGETS];
	for(i = 0; i < zap_count; ++i)
		zap_tids[pnum][i] = 0;
	
	int dmg = ACS_NamedExecuteWithResult("DND Player Damage Scale", CheckInventory("DnD_OverloadDamage") * (100 + CheckActorInventory(killer, "IATTR_OverloadZapDmg")) / 100, TALENT_ELEMENTAL, DND_WDMG_LIGHTNINGDAMAGE);
	for(i = DND_MONSTERTID_BEGIN; i < DnD_TID_List[DND_TID_MONSTER] && zap_count; ++i) {
		// if currently alive and received the checker item
		if(CheckActorInventory(i, "DnD_OverloadZapCandidate") && isActorAlive(i) && i != this)
			zap_tids[pnum][cur_count++] = i;
	}
	
	for(i = 0; i < cur_count; ++i) {
		ACS_NamedExecuteAlways("DnD Overload Zap FX", 0, this, zap_tids[pnum][i]);
		
		// do damage, give credit to killer
		SetActivator(killer);
		
		HandleDamageDeal(killer, zap_tids[pnum][i], dmg, DND_DAMAGETYPE_LIGHTNING, 0, GetActorX(this), GetActorY(this), GetActorZ(this), DND_ACTORFLAG_FOILINVUL | DND_ACTORFLAG_FORCEPAIN);
		ActivatorSound("Overload/Zap", 127);
		
		// overload this monster if its still alive
		if(isActorAlive(zap_tids[pnum][i])) {
			if(!CheckActorInventory(zap_tids[pnum][i], "DnD_OverloadTimer")) {
				SetActorInventory(zap_tids[pnum][i], "DnD_OverloadTimer", DND_BASE_OVERLOADTIME);
				ACS_NamedExecuteWithResult("DnD Monster Overload", zap_tids[pnum][i]);
			}
			else
				SetActorInventory(zap_tids[pnum][i], "DnD_OverloadTimer", DND_BASE_OVERLOADTIME);
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

#endif
