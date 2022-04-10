#ifndef DND_ORBS_IN
#define DND_ORBS_IN

#include "DnD_Common.h"
#include "DnD_Stat.h"
#include "DnD_Ammo.h"
#include "DnD_TempWeps.h"
#include "DnD_Weapons.h"
#include "DnD_OrbsDef.h"
#include "DnD_Sync.h"

#define MAX_ITER 200

#define ENHANCEORB_MAX 50 // 50% bonus
#define PROSPERITY_MAX 1000 // 500 max
#define FORTITUDE_MAX 200 // 200% max
#define WISDOMORB_MAX 250
#define GREEDORB_MAX 250
#define VIOLENCEORB_MAX 300 // 300% for each category
#define SINORB_MAX_TAKE 8
#define AFFLUENCE_MAX 4 // x16
#define AFFLUENCE_MULT 2

// this is server side only, clients aren't even aware of the values here so we can put as many stuff as needed...
// because zan doesn't sync variables to clients unless told to do (sdee dnd_sync.h for it)

// to be able to store item reroll stuff, 1 for attrib count, 2 per attribute x 9 = 19
#define MAX_STORED_ORB_DATA 19
typedef struct {
	int orb_type;
	int values[MAX_STORED_ORB_DATA];
	// corrupt orb ammo save
	int p_ammos[MAX_SLOTS][MAX_AMMOTYPES_PER_SLOT];
	int p_tempammo;
	int p_tempwep;
	bool sins_cant_repent;
} orb_info_T;

// holds most recently used orb values
global orb_info_T 3: Player_MostRecent_Orb[MAXPLAYERS];

void SetDataToOrbBonus(int pnum, int bonus, int extra, int val, bool overwrite = false, bool removeBit = false) {
	int base = 0;
	switch(bonus) {
		case OBI_HPFLAT:
			base = Player_Orb_Data[pnum].orb_stat_bonuses.hp_flat_bonus;
			Player_Orb_Data[pnum].orb_stat_bonuses.hp_flat_bonus = val;
		break;
		case OBI_ARMORFLAT:
			base = Player_Orb_Data[pnum].orb_stat_bonuses.armor_flat_bonus;
			Player_Orb_Data[pnum].orb_stat_bonuses.armor_flat_bonus = val;
		break;
		case OBI_HPPERCENT:
			base = Player_Orb_Data[pnum].orb_stat_bonuses.hp_percent_bonus;
			Player_Orb_Data[pnum].orb_stat_bonuses.hp_percent_bonus = val;
		break;
		case OBI_ARMORPERCENT:
			base = Player_Orb_Data[pnum].orb_stat_bonuses.armor_percent_bonus;
			Player_Orb_Data[pnum].orb_stat_bonuses.armor_percent_bonus = val;
		break;
		case OBI_WISDOMPERCENT:
			base = Player_Orb_Data[pnum].orb_stat_bonuses.wisdom_percent_bonus;
			Player_Orb_Data[pnum].orb_stat_bonuses.wisdom_percent_bonus = val;
		break;
		case OBI_GREEDPERCENT:
			base = Player_Orb_Data[pnum].orb_stat_bonuses.greed_percent_bonus;
			Player_Orb_Data[pnum].orb_stat_bonuses.greed_percent_bonus = val;
		break;
		case OBI_SPEED:
			base = Player_Orb_Data[pnum].orb_stat_bonuses.speed_bonus;
			Player_Orb_Data[pnum].orb_stat_bonuses.speed_bonus = val;
		break;
		case OBI_DROPCHANCE:
			base = Player_Orb_Data[pnum].orb_stat_bonuses.drop_chance;
			Player_Orb_Data[pnum].orb_stat_bonuses.drop_chance = val;
		break;
		case OBI_HOLDING:
			base = Player_Orb_Data[pnum].orb_stat_bonuses.holding;
			Player_Orb_Data[pnum].orb_stat_bonuses.holding = val;
		break;
		case OBI_DAMAGETYPE:
			base = Player_Orb_Data[pnum].orb_stat_bonuses.damage_type_bonus[extra];
			Player_Orb_Data[pnum].orb_stat_bonuses.damage_type_bonus[extra] = val;
		break;
		
		case OBI_WEAPON_ENCHANT:
			base = Player_Orb_Data[pnum].weapon_stat_bonuses[extra].quality;
			Player_Orb_Data[pnum].weapon_stat_bonuses[extra].quality = val;
		break;
		case OBI_WEAPON_CRIT:
			base = Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRIT].val;
			Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRIT].val = val;
		break;
		case OBI_WEAPON_CRITDMG:
			base = Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRITDMG].val;
			Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRITDMG].val = val;
		break;
		case OBI_WEAPON_CRITPERCENT:
			base = Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRITPERCENT].val;
			Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRITPERCENT].val = val;
		break;
		case OBI_WEAPON_DMG:
			base = Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_DMG].val;
			Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_DMG].val = val;
		break;
		case OBI_WEAPON_POWERSET1:
			if(!overwrite) {
				// this is a bitset so it gets OR'd instead
				if(!removeBit)
					Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_POWERSET1].val |= (1 << val);
				else {
					// negative means remove it, we want the negative state to be carried down to updateactivity so we wont modify it here
					Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_POWERSET1].val &= ~(1 << (-val));
				}
			}
			else {
				// simply update
				Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_POWERSET1].val = val;
			}
		break;
		// weapon mods can come in the future for orbs that give those
	}
	
	// get difference to add
	if(bonus != OBI_WEAPON_POWERSET1)
		val -= base;
	UpdateActivity(pnum, bonus + MAP_ORB_TO_ACTIVITY, val, extra, overwrite, removeBit);
}

// it's too cumbersome to edit all other function calls to exclude the activity update, plus the added if statements...
void SetDataToOrbBonus_NoActivity(int pnum, int bonus, int extra, int val) {
	switch(bonus) {
		case OBI_HPFLAT:
			Player_Orb_Data[pnum].orb_stat_bonuses.hp_flat_bonus = val;
		break;
		case OBI_ARMORFLAT:
			Player_Orb_Data[pnum].orb_stat_bonuses.armor_flat_bonus = val;
		break;
		case OBI_HPPERCENT:
			Player_Orb_Data[pnum].orb_stat_bonuses.hp_percent_bonus = val;
		break;
		case OBI_ARMORPERCENT:
			Player_Orb_Data[pnum].orb_stat_bonuses.armor_percent_bonus = val;
		break;
		case OBI_WISDOMPERCENT:
			Player_Orb_Data[pnum].orb_stat_bonuses.wisdom_percent_bonus = val;
		break;
		case OBI_GREEDPERCENT:
			Player_Orb_Data[pnum].orb_stat_bonuses.greed_percent_bonus = val;
		break;
		case OBI_SPEED:
			Player_Orb_Data[pnum].orb_stat_bonuses.speed_bonus = val;
		break;
		case OBI_DROPCHANCE:
			Player_Orb_Data[pnum].orb_stat_bonuses.drop_chance = val;
		break;
		case OBI_HOLDING:
			Player_Orb_Data[pnum].orb_stat_bonuses.holding = val;
		break;
		case OBI_DAMAGETYPE:
			Player_Orb_Data[pnum].orb_stat_bonuses.damage_type_bonus[extra] = val;
		break;
		
		case OBI_WEAPON_ENCHANT:
			Player_Orb_Data[pnum].weapon_stat_bonuses[extra].quality = val;
		break;
		case OBI_WEAPON_CRIT:
			Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRIT].val = val;
		break;
		case OBI_WEAPON_CRITDMG:
			Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRITDMG].val = val;
		break;
		case OBI_WEAPON_CRITPERCENT:
			Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRITPERCENT].val = val;
		break;
		case OBI_WEAPON_DMG:
			Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_DMG].val = val;
		break;
		case OBI_WEAPON_POWERSET1:
			Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_POWERSET1].val = val;
		break;
		// weapon mods can come in the future for orbs that give those
	}
}

void AddOrbBonusData(int pnum, int bonus, int extra, int val) {
	if(bonus == OBI_WEAPON_ENCHANT)
		printbold(s:"add orb val: ", d: GetDataFromOrbBonus(pnum, bonus, extra) + val);
	SetDataToOrbBonus(pnum, bonus, extra, GetDataFromOrbBonus(pnum, bonus, extra) + val);
}

#define MAX_ORBS_BITS getpow2(MAX_ORBS)
#define MAX_ORBS_BITONES (1 << (MAX_ORBS_BITS)) - 1

// percentages
/*
12
5
10
2.5
1.5
8
12
9
9
5
5
2.5
2.5
2.5
5
5
3.5
0 // drops only from specific monster
0 // drops only from specific monster
0 // drops only from specific monster
*/

int OrbDropWeights[MAX_ORBS] = {
	120,
	170,
	270,
	295,
	310,
	390,
	510,
	600,
	690,
	740,
	790,
	815,
	840,
	865,
	915,
	965,
	1000,
	// drops only from specific monster
	0xFFFFFF,
	0xFFFFFF,
	0xFFFFFF
};

#define ENHANCEORB_BONUS 1

#define ORB_MAXWEIGHT 1000

#define STATORB_MAXWEIGHT 40
#define STATORB_MAXSTATS 6
int SpiritOrb_StatDropWeights[STATORB_MAXSTATS] = { 10, 20, 24, 25, 31, 40 };

enum {
	CORRUPTORB_NOTHING,
	
	CORRUPTORB_TAKEHP,
	CORRUPTORB_TAKEDMG,
	CORRUPTORB_TAKESPEED,
	CORRUPTORB_TAKEAMMO,
	CORRUPTORB_TAKEBACKPACK,
	CORRUPTORB_REDUCEDMGMAP,
	
	CORRUPTORB_ADDDMG,
	CORRUPTORB_ADDCRIT,
	CORRUPTORB_ADDCRITDMG,
	CORRUPTORB_ADDSPEED,
	CORRUPTORB_DROPCHANCE,
	CORRUPTORB_GIVESTAT,
	
	CORRUPTORB_MOD_PERCENTDAMAGE,
	CORRUPTORB_MOD_POISONFORPERCENTDAMAGE,
	CORRUPTORB_MOD_FORCEPAINCHANCE
};

#define CORRUPTORB_WEAPONMOD_BEGIN CORRUPTORB_MOD_PERCENTDAMAGE
#define CORRUPTORB_MAXEFFECTS (CORRUPTORB_MOD_FORCEPAINCHANCE + 1)
#define CORRUPTORB_MAXWEIGHT 1000

/*
chances
50

3
3
3
3
3
3

2
2
4
5
6
6

1
3
3
*/

int CorruptOrb_Weights[CORRUPTORB_MAXEFFECTS] = {
	500,
	
	530,
	560,
	590,
	620,
	650,
	680,
	
	700,
	720,
	760,
	810,
	870,
	930,
	
	940,
	970,
	1000
};

#define CORRUPTORB_MINDMG -100
#define CORRUPTORB_MINSPEED -0.5
#define CORRUPTORB_MAXCRITPERCENT 1.0
#define CORRUPTORB_MAXDMG 100
#define CORRUPTORB_MAXCRITDMG 200
#define CORRUPTORB_MAXSPEED 0.5
#define CORRUPTORB_MAXDROPCHANCE 0.5
#define CORRUPTORB_MINHEALTH 100

#define CORRUPTORB_DMGTAKE 5
#define CORRUPTORB_HPTAKE 10
#define CORRUPTORB_SPEEDTAKE 0.025
#define CORRUPTORB_DMGGIVE 5
#define CORRUPTORB_CRITGIVE 0.05
#define CORRUPTORB_CRITDMGGIVE 25
#define CORRUPTORB_SPEEDGIVE 0.05
#define CORRUPTORB_DROPCHANCEGIVE 0.025
#define CORRUPTORB_STATGIVE 5

// sin orb stuff
enum {
	SINORB_STAT,
	SINORB_CRIT,
	SINORB_CRITDMG,
	SINORB_PERK
};
#define SINORB_OPT_BEGIN SINORB_STAT
#define SINORB_OPT_END SINORB_PERK
#define SINORB_CRITMAX 0.2
#define SINORB_CRITDMGMAX 100
#define SINORB_CRED_MIN 50
#define SINORB_CRED_MAX 500
#define SINORB_CRITGIVE 0.01
#define SINORB_CRITDMGGIVE 5
#define SINORB_STATSAVEBITS 8
#define SINORB_STATSAVEMASK 0xFF

#define SINORB_MAXRUNS 12

#define SINORB_STATGIVE 4
#define SINORB_PERKGIVE 1

#define HOLDING_MAX 100
#define HOLDINGORB_BONUS 1

enum {
	RICHES_EXP,
	RICHES_CREDIT,
	RICHES_BUDGET
};
#define MAX_RICHES_WEIGHT 10
#define MAX_RICHES (RICHES_BUDGET + 1)

#define RICHES_AMOUNT 0
#define RICHES_WEIGHTS 1
int OrbRichesData[MAX_RICHES][2] = {
	{ 5, 2 },
	{ 5, 9 },
	{ 5, 10 }
};

bool CanAddModToItem(int itemtype, int item_index, int add_lim) {
	bool res = false;
	int pnum = PlayerNumber();
	if(IsUsableOnInventory(itemtype)) {
		// this one depends on attribute counts of items it is used on
		if(PlayerInventoryList[pnum][item_index].item_type > UNIQUE_BEGIN)
			res = false;
		else if(itemtype == DND_ITEM_CHARM) {
			// printbold(d:PlayerInventoryList[pnum][item_index].attrib_count, s: " < ", d:Charm_MaxAffixes[PlayerInventoryList[pnum][item_index].item_subtype] + add_lim);
			res = PlayerInventoryList[pnum][item_index].attrib_count < Charm_MaxAffixes[PlayerInventoryList[pnum][item_index].item_subtype] + add_lim;
		}
	}
	return res;
}

// extra type holds the base item type, no extra information - this comes from the function that calls this
bool CanUseOrb(int orbtype, int extra, int extratype) {
	bool res = 0;
	int temp = -1, i;
	int pnum = PlayerNumber();
	
	SetInventory("OrbUseType", orbtype + 1);
	switch(orbtype) {
		case DND_ORB_ENHANCE:
			if(extratype == DND_ITEM_WEAPON && GetDataFromOrbBonus(pnum, OBI_WEAPON_ENCHANT, extra) != ENHANCEORB_MAX)
				res = true;
		break;
		case DND_ORB_CORRUPT:
			res = true; // can always use this, the orb will find a way to fuck you up in some way
		break;
		case DND_ORB_SPIRIT:
			for(i = 0; i <= DND_ATTRIB_END && temp == -1; ++i)
				if(GetStat(i) != DND_STAT_FULLMAX)
					temp = i;
			res = temp != -1;
		break;
		case DND_ORB_REPENT:
			temp = Player_MostRecent_Orb[pnum].orb_type;
			if(temp) {
				if(temp - 1 < DND_ORB_REFINEMENT)
					res = Player_MostRecent_Orb[pnum].orb_type - 1 != DND_ORB_REPENT && Player_MostRecent_Orb[pnum].orb_type - 1 != DND_ORB_RICHES;
				else
					res = Player_MostRecent_Orb[pnum].p_tempwep;
			}
		break;
		case DND_ORB_AFFLUENCE:
			res = CheckInventory("AffluenceCounter") < AFFLUENCE_MAX;
		break;
		case DND_ORB_CALAMITY:
			res = HasOrbsBesidesCalamity();
		break;
		case DND_ORB_PROSPERITY:
			res = GetDataFromOrbBonus(pnum, OBI_HPFLAT, -1) != PROSPERITY_MAX;
		break;
		case DND_ORB_FORTITUDE:
			res = GetDataFromOrbBonus(pnum, OBI_HPPERCENT, -1) != FORTITUDE_MAX;
		break;
		case DND_ORB_WISDOM:
			res = GetDataFromOrbBonus(pnum, OBI_WISDOMPERCENT, -1) != GREEDORB_MAX;
		break;
		case DND_ORB_GREED:
			res = GetDataFromOrbBonus(pnum, OBI_GREEDPERCENT, -1) != GREEDORB_MAX;
		break;
		case DND_ORB_VIOLENCE:
			for(i = TALENT_BULLET; i < MAX_TALENTS && !res; ++i)
				if(GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, i) != VIOLENCEORB_MAX)
					res = true;
		break;
		case DND_ORB_SIN:
			res = Calculate_Stats() >= SINORB_MAX_TAKE * GetAffluenceBonus();
		break;
		case DND_ORB_RICHES:
			res = !((CheckInventory("Level") == 100) && (CheckInventory("Credit") == 0x7fffffff) && (CheckInventory("Budget") == 1000));
		break;
		case DND_ORB_HOLDING:
			res = GetDataFromOrbBonus(pnum, OBI_HOLDING, -1) != HOLDING_MAX;
		break;
		case DND_ORB_REFINEMENT:
			if(IsUsableOnInventory(extratype))
				res = true;
		break;
		case DND_ORB_SCULPTING:
			if(IsUsableOnInventory(extratype)) {
				// don't let this be used on a unique
				if(PlayerInventoryList[pnum][extra].item_type > UNIQUE_BEGIN)
					res = false;
				else
					res = PlayerInventoryList[pnum][extra].attrib_count;
			}
		break;
		case DND_ORB_ELEVATION:
			res = CanAddModToItem(extratype, extra, 0);
		break;
		case DND_ORB_HOLLOW:
			// this one is basically an orb of elevation with a +1 additional attribute allowed
			res = CanAddModToItem(extratype, extra, 1);
		break;
		case DND_ORB_PHANTASMAL:
			// if the weapon can't hit ghosts on its own or we didnt give it the ghost hit already
			if(extratype == DND_ITEM_WEAPON && !HasWeaponPower(pnum, extra, WEP_POWER_GHOSTHIT) && (WeaponProperties[extra] & WPROP_CANTHITGHOST))
				res = true;
		break;
		case DND_ORB_ASSIMILATION:
			// extra is itemid1, extratype is itemid2
			// we must have matching item types, so charm x charm, Y x Y etc.
			// and we have at least 1 total attribute to be taking
			res = 	PlayerInventoryList[pnum][extra].item_type == PlayerInventoryList[pnum][extratype].item_type && 
					(PlayerInventoryList[pnum][extra].attrib_count || PlayerInventoryList[pnum][extratype].attrib_count);
		break;
	}
	if(!res)
		SetInventory("OrbResult", 0x7FFFFFFF);
	return res;
}

void HandleAddRandomMod(int pnum, int item_index, int add_lim, bool isWellRolled) {
	int i, temp, aff = GetAffluenceBonus();
	bool finish = false;
	// save
	Player_MostRecent_Orb[pnum].p_tempwep = item_index + 1;
	Player_MostRecent_Orb[pnum].values[0] = PlayerInventoryList[pnum][item_index].attrib_count;
	for(i = 0; i < Player_MostRecent_Orb[pnum].values[0]; ++i) {
		Player_MostRecent_Orb[pnum].values[2 * i + 1] = PlayerInventoryList[pnum][item_index].attributes[i].attrib_id;
		Player_MostRecent_Orb[pnum].values[2 * i + 2] = PlayerInventoryList[pnum][item_index].attributes[i].attrib_val;
	}
	
	for(int s = 0; s < aff && !finish; ++s) {
		i = PlayerInventoryList[pnum][item_index].attrib_count;
		// find an attribute that this item doesn't have
		do {
			temp = random(0, LAST_INV_ATTRIBUTE);
		} while(CheckItemAttribute(pnum, item_index, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, i) != -1);
		
		// if not well rolled by default, run the chance (orbs may force it, but sometimes they may not)
		AddAttributeToItem(pnum, item_index, temp, !isWellRolled ? CheckWellRolled(pnum) : isWellRolled);
		finish = PlayerInventoryList[pnum][item_index].item_type == DND_ITEM_CHARM && PlayerInventoryList[pnum][item_index].attrib_count >= Charm_MaxAffixes[PlayerInventoryList[pnum][item_index].item_subtype] + add_lim;
	}
	SyncItemAttributes(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	SetInventory("OrbResult", item_index);
}

// picks an item id from two weigted by their level x 10
int PickWeightedFromTwoItems(int pnum, int item1, int item2) {
	int lvl1 = PlayerInventoryList[pnum][item1].item_level * 10;
	int lvl2 = PlayerInventoryList[pnum][item2].item_level * 10;
	int res = random(1, lvl1 + lvl2);
	
	// pick from the weight which item index we are taking
	if(res > lvl1)
		res = item2;
	else if(res < lvl1)
		res = item1;
	else {
		// if they are both the same we randomly pick the one
		res = random(0, 1) ? item1 : item2;
	}
	return res;
}

void HandleOrbUse (int pnum, int orbtype, int extra, int extra2 = -1) {
	int res = -1;
	int temp;
	int i, s;
	SetInventory("OrbUseType", orbtype + 1);
	// for any other orb, reset most recently used orb
	if(orbtype != DND_ORB_REPENT)
		ResetMostRecentOrb(pnum);
	switch(orbtype) {
		case DND_ORB_ENHANCE:
			res = extra;
			SetDataToOrbBonus(pnum, OBI_WEAPON_ENCHANT, res, Clamp_Between(GetDataFromOrbBonus(pnum, OBI_WEAPON_ENCHANT, res) + GetAffluenceBonus() * ENHANCEORB_BONUS, 0, ENHANCEORB_MAX));
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
			Player_MostRecent_Orb[pnum].values[1] = GetAffluenceBonus() * ENHANCEORB_BONUS;
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEAPONENHANCE, res);
		break;
		case DND_ORB_CORRUPT:
			// roll what to do
			do {
				temp = random(0, CORRUPTORB_MAXWEIGHT);
				for(i = 0; i < CORRUPTORB_MAXEFFECTS && temp > CorruptOrb_Weights[i]; ++i);
			} while(!CorruptionMaxChecks(i) && res++ < MAX_ITER);
			// i = CORRUPTORB_MOD_POISONFORPERCENTDAMAGE;
			// i has the effect to do now
			SetInventory("OrbResult", i);
			Player_MostRecent_Orb[pnum].values[0] = i;
			Player_MostRecent_Orb[pnum].values[0] |= GetAffluenceBonus() << 8;
			HandleCorruptOrbUse(i);
		break;
		case DND_ORB_SPIRIT:
			// find a random stat to give points to, checking they aren't max already
			do {
				temp = random(0, STATORB_MAXWEIGHT);
				for(i = 0; i < STATORB_MAXSTATS; ++i)
					if(temp <= SpiritOrb_StatDropWeights[i])
						break;
			} while(GetStat(i) == DND_STAT_FULLMAX);
			GiveStat(i, GetAffluenceBonus());
			res = i;
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
			Player_MostRecent_Orb[pnum].values[1] = GetAffluenceBonus();
		break;
		case DND_ORB_REPENT:
			RevertLastOrbEffect();
		break;
		case DND_ORB_AFFLUENCE:
			GiveInventory("AffluenceCounter", 1);
			res = CheckInventory("AffluenceCounter");
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
		break;
		case DND_ORB_CALAMITY:
			do {
				temp = PickRandomOrb();
			} while(temp == DND_ORB_CALAMITY || !HasOrbOfType(temp));
			// take the orb, then roll another new orb
			i = TakeOrbFromPlayer(temp, GetAffluenceBonus());
			res = temp;
			do {
				temp = PickRandomOrb();
			} while(temp == DND_ORB_CALAMITY);
			ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, temp, i);
			res += temp * 100;
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
			Player_MostRecent_Orb[pnum].values[1] = i;
		break;
		case DND_ORB_PROSPERITY:
			res = GetDataFromOrbBonus(pnum, OBI_HPFLAT, -1);
			SetDataToOrbBonus(pnum, OBI_HPFLAT, -1, Clamp_Between(res + GetAffluenceBonus(), 0, PROSPERITY_MAX));
			res = GetDataFromOrbBonus(pnum, OBI_ARMORFLAT, -1);
			SetDataToOrbBonus(pnum, OBI_ARMORFLAT, -1, Clamp_Between(res + GetAffluenceBonus(), 0, PROSPERITY_MAX));
			UpdateArmorVisual();
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
			SyncClientsideVariable_Orb(pnum, DND_SYNC_HPFLAT_BONUS, 0);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_ARMORFLAT_BONUS, 0);
		break;
		case DND_ORB_FORTITUDE:
			res = GetDataFromOrbBonus(pnum, OBI_HPPERCENT, -1);
			SetDataToOrbBonus(pnum, OBI_HPPERCENT, -1, Clamp_Between(res + GetAffluenceBonus(), 0, FORTITUDE_MAX));
			res = GetDataFromOrbBonus(pnum, OBI_ARMORPERCENT, -1);
			SetDataToOrbBonus(pnum, OBI_ARMORPERCENT, -1, Clamp_Between(res + GetAffluenceBonus(), 0, FORTITUDE_MAX));
			UpdateArmorVisual();
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
			SyncClientsideVariable_Orb(pnum, DND_SYNC_HPPERCENT_BONUS, 0);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_ARMORPERCENT_BONUS, 0);
		break;
		case DND_ORB_WISDOM:
			res = GetDataFromOrbBonus(pnum, OBI_WISDOMPERCENT, -1);
			SetDataToOrbBonus(pnum, OBI_WISDOMPERCENT, -1, Clamp_Between(res + GetAffluenceBonus(), 0, WISDOMORB_MAX));
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WISDOMPERCENT_BONUS, 0);
		break;
		case DND_ORB_GREED:
			res = GetDataFromOrbBonus(pnum, OBI_GREEDPERCENT, -1);
			SetDataToOrbBonus(pnum, OBI_GREEDPERCENT, -1, Clamp_Between(res + GetAffluenceBonus(), 0, GREEDORB_MAX));
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
			SyncClientsideVariable_Orb(pnum, DND_SYNC_GREEDPERCENT_BONUS, 0);
		break;
		case DND_ORB_VIOLENCE:
			do {
				temp = PickRandomTalent();
			} while(GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, temp) == VIOLENCEORB_MAX);
			res = GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, temp);
			Player_MostRecent_Orb[pnum].values[0] = temp;
			Player_MostRecent_Orb[pnum].values[0] += 100 * res;
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, temp, Clamp_Between(res + GetAffluenceBonus(), 0, VIOLENCEORB_MAX));
			SetInventory("OrbResult", temp);
			// start from ballistic talent end with occult
			SyncClientsideVariable_Orb(pnum, DND_SYNC_DAMAGEBULLET + temp, 0);
		break;
		case DND_ORB_SIN:
			// take away random stats
			s = GetAffluenceBonus();
			temp = random(1, SINORB_MAX_TAKE);
			res = 0;
			// save each stat's previous values in 7 bits (it can take max 64), 6 stats = 42 bits needed => 2 ints. 3rd int can hold type of sin orb effect + affluence count
			RecentOrb_SaveOldStats(pnum);
			for(i = 0; i < temp * s; ++i)
				TakeStat(GetRandomSinStat(), 1);
			RecentOrb_FindDifference(pnum);
			// use inv style encoding for which states are gone
			//res |= s;
			// got from 0 to 5 in res
			res |= temp;
			temp = random(SINORB_OPT_BEGIN, SINORB_OPT_END);
			Player_MostRecent_Orb[pnum].values[2] = s;
			Player_MostRecent_Orb[pnum].values[2] |= temp << 5;
			res |= temp << 8;
			res |= HandleSinOrbBonus(temp) << 11;
			// save the bonus type
			SetInventory("OrbResult", res);
		break;
		case DND_ORB_RICHES:
			do {
				temp = random(1, MAX_RICHES_WEIGHT);
				for(i = 0; i < MAX_RICHES && temp > OrbRichesData[i][RICHES_WEIGHTS]; ++i);
			} while(
				(i == 0 && CheckInventory("Level") == 100) ||
				(i == 1 && CheckInventory("Credit") == 0x7fffffff) ||
				(i == 2 && CheckInventory("Budget") == 1000));

			res = GetAffluenceBonus() * OrbRichesData[i][RICHES_AMOUNT];
			Player_MostRecent_Orb[pnum].values[0] = i;
			Player_MostRecent_Orb[pnum].values[1] = res;
			if(!i)
				GiveExp((LevelCurve[CheckInventory("Level") - 1] / 100) * res); // don't want overflows! -- technically it still can, but hopefully wont
			else if(i == 1) {
				res = Max(2000, (CheckInventory("Credit") / 100) * res);
				GiveCredit(res);
				UpdateActivity(pnum, DND_ACTIVITY_CREDIT, res, -1);
			}
			else if(i == 2) {
				GiveBudget(res);
				UpdateActivity(pnum, DND_ACTIVITY_BUDGET, res, -1);
			}
			res |= i << 30;
			SetInventory("OrbResult", res);
		break;
		case DND_ORB_HOLDING:
			res = GetAffluenceBonus() * HOLDINGORB_BONUS;
			temp = GetDataFromOrbBonus(pnum, OBI_HOLDING, -1);
			SetDataToOrbBonus(pnum, OBI_HOLDING, -1, Clamp_Between(temp + res, 0, HOLDING_MAX));
			Player_MostRecent_Orb[pnum].values[0] = GetDataFromOrbBonus(pnum, OBI_HOLDING, -1) - temp;
			SetAllAmmoCapacities();
			SetInventory("OrbResult", res);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_HOLDING, 0);
		break;
		case DND_ORB_REFINEMENT:
			// save
			Player_MostRecent_Orb[pnum].p_tempwep = extra + 1;
			for(i = 0; i < PlayerInventoryList[pnum][extra].attrib_count; ++i)
				Player_MostRecent_Orb[pnum].values[i] = PlayerInventoryList[pnum][extra].attributes[i].attrib_val;
				
			s = GetAffluenceBonus();
			if(PlayerInventoryList[pnum][extra].item_type > UNIQUE_BEGIN) {
				// handle unique random roll case
				temp = (PlayerInventoryList[pnum][extra].item_type >> UNIQUE_BITS) - 1;
				for(res = 0; res < s; ++res) {
					for(i = 0; i < PlayerInventoryList[pnum][extra].attrib_count; ++i)
						PlayerInventoryList[pnum][extra].attributes[i].attrib_val = RollUniqueAttributeValue(temp, i, CheckWellRolled(pnum));
				}
			}
			else {
				for(res = 0; res < s; ++res) {
					for(i = 0; i < PlayerInventoryList[pnum][extra].attrib_count; ++i) {
						temp = PlayerInventoryList[pnum][extra].attributes[i].attrib_id;
						PlayerInventoryList[pnum][extra].attributes[i].attrib_val = RollAttributeValue(temp, PlayerInventoryList[pnum][extra].attributes[i].attrib_tier, CheckWellRolled(pnum));
					}
				}
			}
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_SCULPTING:
			// save
			Player_MostRecent_Orb[pnum].p_tempwep = extra + 1;
			Player_MostRecent_Orb[pnum].values[0] = PlayerInventoryList[pnum][extra].attrib_count;
			for(i = 0; i < Player_MostRecent_Orb[pnum].values[0]; ++i) {
				Player_MostRecent_Orb[pnum].values[2 * i + 1] = PlayerInventoryList[pnum][extra].attributes[i].attrib_id;
				Player_MostRecent_Orb[pnum].values[2 * i + 2] = PlayerInventoryList[pnum][extra].attributes[i].attrib_val;
			}
			for(s = 0; s < GetAffluenceBonus() && PlayerInventoryList[pnum][extra].attrib_count; ++s) {
				res = random(0, PlayerInventoryList[pnum][extra].attrib_count - 1);
				// all attributes must be shifted left from the position of the deleted attribute now
				for(i = res; i < PlayerInventoryList[pnum][extra].attrib_count - 1; ++i) {
					PlayerInventoryList[pnum][extra].attributes[i].attrib_id = PlayerInventoryList[pnum][extra].attributes[i + 1].attrib_id;
					PlayerInventoryList[pnum][extra].attributes[i].attrib_val = PlayerInventoryList[pnum][extra].attributes[i + 1].attrib_val;
				}
				--PlayerInventoryList[pnum][extra].attrib_count;
			}
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_ELEVATION:
			HandleAddRandomMod(pnum, extra, 0, false);
		break;
		case DND_ORB_HOLLOW:
			HandleAddRandomMod(pnum, extra, 1, true);
		break;
		case DND_ORB_PHANTASMAL:
			res = extra;
			SetDataToOrbBonus(pnum, OBI_WEAPON_POWERSET1, res, WEP_POWER_GHOSTHIT);
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
			Player_MostRecent_Orb[pnum].values[1] = WEP_POWER_GHOSTHIT;
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_POWERSET1, res);
		break;
		case DND_ORB_ASSIMILATION:
			// extra and extra2 are the item indexes in use, with extra being the one assimilated into extra2
			// we will store the attributes in a temporary array, then copy them to extra2
			int attrib_ids[MAX_ITEM_ATTRIBUTES];
			int picked_mod;
			bool fail_pick;
			
			// init the temp array to -1s
			for(i = 0; i < MAX_ITEM_ATTRIBUTES; ++i)
				attrib_ids[i] = -1;
			
			// we determine when to stop, we either stop until max affixes or we have minimum of both attrib counts total
			s = Min(Charm_MaxAffixes[PlayerInventoryList[pnum][extra2].item_subtype] + 1, PlayerInventoryList[pnum][extra].attrib_count + PlayerInventoryList[pnum][extra2].attrib_count);
			temp = 0;
			
			do {
				do {
					fail_pick = false;
					// choose which item has their attribute to be taken
					// pick a random attribute from it
					// confirm this is a real attribute and the charm isn't devoid of any attributes
					res = PickWeightedFromTwoItems(pnum, extra, extra2);
					picked_mod = random(0, PlayerInventoryList[pnum][res].attrib_count - 1);
					// log(s:"picked ", d: res, s:" from: ", d:extra, s: " - ", d:extra2, s: "\nmod: ", d:picked_mod);
					
					// if somehow this item has no attributes, pick from the other one
					if(picked_mod == -1) {
						res = extra2 + extra - res;
						picked_mod = random(0, PlayerInventoryList[pnum][res].attrib_count - 1);
					}
					
					// set this to be the actual attribute now instead of just an index in the item itself
					picked_mod = PlayerInventoryList[pnum][res].attributes[picked_mod].attrib_id;
					
					// if its not in our array add it to it
					for(i = 0; i < MAX_ITEM_ATTRIBUTES && attrib_ids[i] != -1; ++i) {
						if(picked_mod == attrib_ids[i]) {
							fail_pick = true;
							break;
						}
					}
				} while(fail_pick);
				// confirmed addable attribute
				++temp;
				attrib_ids[i] = picked_mod;
			} while(temp < s);

			// we are going to assimilate the first one into second, so the first one is destroyed
			// and make sure the extra2 item has no attributes left anymore
			FreeItem(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
			ResetPlayerItemAttributes(pnum, extra2);
			
			// copy the attributes into extra2
			for(i = 0; i < MAX_ITEM_ATTRIBUTES && attrib_ids[i] != -1; ++i)
				AddAttributeToItem(pnum, extra2, attrib_ids[i], true);
			
			// finally, sync it
			SyncItemData(pnum, extra2, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[pnum][extra2].width, PlayerInventoryList[pnum][extra2].height);
			GiveInventory("DnD_CleanCraftingRequest", 1);
			GiveInventory("DnD_RefreshPane", 1);
			SetInventory("OrbResult", 1);
		break;
	}
	Player_MostRecent_Orb[pnum].orb_type = orbtype + 1; // +1 because 0 is used as no orb
}

// check if player has any orbs besides calamity
bool HasOrbsBesidesCalamity() {
	int pn = PlayerNumber();
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(PlayerInventoryList[pn][i].item_type == DND_ITEM_ORB) {
			if(PlayerInventoryList[pn][i].item_subtype == DND_ORB_CALAMITY || IsOrbDropException(PlayerInventoryList[pn][i].item_subtype))
				continue;
			return true;
		}
	}
	return false;
}

bool HasOrbOfType(int type) {
	int pn = PlayerNumber();
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(PlayerInventoryList[pn][i].item_type == DND_ITEM_ORB && PlayerInventoryList[pn][i].item_subtype == type)
			return true;
	}
	return false;
}

int TakeOrbFromPlayer(int otype, int amt) {
	int i, j, pnum = PlayerNumber();
	int res = 0;
	// search player inventory to take from first
	for(i = 0; i < MAX_INVENTORY_BOXES && res < amt; ++i) {
		if(PlayerInventoryList[pnum][i].item_type == DND_ITEM_ORB && PlayerInventoryList[pnum][i].item_subtype == otype) {
			if(PlayerInventoryList[pnum][i].item_stack > amt - res) {
				UsePlayerItem_Count(pnum, i, amt - res);
				res += amt;
			}
			else { 
				// use however many we are left to reach amt
				res += PlayerInventoryList[pnum][i].item_stack;
				UsePlayerItem_Count(pnum, i, PlayerInventoryList[pnum][i].item_stack);
			}
		}
	}
	
	if(res == amt)
		return res;
	
	// now check player stash if we couldn't find enough
	for(i = 0; i < CheckInventory("DnD_PlayerInventoryPages") && res < amt; ++i) {
		for(j = 0; j < MAX_INVENTORY_BOXES && res < amt; ++j) {
			if(PlayerStashList[pnum][i][j].item_type == DND_ITEM_ORB && PlayerStashList[pnum][i][j].item_subtype == otype) {
				if(PlayerStashList[pnum][i][j].item_stack > amt - res) {
					UsePlayerStashItem_Count(pnum, i, j, amt - res);
					res += amt;
				}
				else { 
					// use however many we are left to reach amt
					res += PlayerStashList[pnum][i][j].item_stack;
					UsePlayerStashItem_Count(pnum, i, j, PlayerStashList[pnum][i][j].item_stack);
				}
			}
		}
	}
	
	// return whatever we acquired
	return res;
}

// very badly coded, improve later
void GiveOrbToPlayer(int pnum, int otype, int amt) {
	int res = 0;
	int i, j;
	// try to find space in player inventory first
	i = GetFreeSpotForSingleSpotItem(pnum, DND_ITEM_ORB, otype);
	if(i != -1) {
		j = GetStackValue(DND_ITEM_ORB);
		if(PlayerInventoryList[pnum][i].item_type != DND_ITEM_NULL) {
			// we can add to this item's stack
			if(amt - res < j - PlayerInventoryList[pnum][i].item_stack) {
				PlayerInventoryList[pnum][i].item_stack += amt - res;
				res = amt;
			}
			else {
				// difference is added
				res += j - PlayerInventoryList[pnum][i].item_stack;
				PlayerInventoryList[pnum][i].item_stack = j;
			}
			SyncItemStack(pnum, i, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		}
		else {
			// implant this item here, and add as many stacks as we can to this
			PlayerInventoryList[pnum][i].item_level = 1;
			PlayerInventoryList[pnum][i].item_type = DND_ITEM_ORB;
			PlayerInventoryList[pnum][i].item_subtype = otype;
			PlayerInventoryList[pnum][i].width = 1;
			PlayerInventoryList[pnum][i].height = 1;
			PlayerInventoryList[pnum][i].attrib_count = 0;
			PlayerInventoryList[pnum][i].item_image = ITEM_IMAGE_ORB_BEGIN + otype;
			PlayerInventoryList[pnum][i].topleftboxid = i + 1;
			if(amt - res < j - PlayerInventoryList[pnum][i].item_stack) {
				PlayerInventoryList[pnum][i].item_stack += amt - res;
				res = amt;
			}
			else {
				// difference is added
				res += j - PlayerInventoryList[pnum][i].item_stack;
				PlayerInventoryList[pnum][i].item_stack = j;
			}
			SyncItemData(pnum, i, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, 1, 1);
		}
	}
	
	if(res < amt) {
		// no space or need more? try stash
		for(i = 0; i < CheckInventory("DnD_PlayerInventoryPages") && res < amt; ++i) {
			for(j = 0; j < MAX_INVENTORY_BOXES && res < amt; ++j) {
				if(PlayerStashList[pnum][i][j].item_type == DND_ITEM_NULL) {
					// implant item here with remaining stacks
					PlayerStashList[pnum][i][j].item_level = 1;
					PlayerStashList[pnum][i][j].item_type = DND_ITEM_ORB;
					PlayerStashList[pnum][i][j].item_subtype = otype;
					PlayerStashList[pnum][i][j].width = 1;
					PlayerStashList[pnum][i][j].height = 1;
					PlayerStashList[pnum][i][j].attrib_count = 0;
					PlayerStashList[pnum][i][j].item_image = ITEM_IMAGE_ORB_BEGIN + otype;
					PlayerStashList[pnum][i][j].topleftboxid = j + 1;
					
					if(amt - res < GetStackValue(DND_ITEM_ORB) - PlayerStashList[pnum][i][j].item_stack) {
						PlayerStashList[pnum][i][j].item_stack += amt - res;
						res = amt;
					}
					else {
						res += GetStackValue(DND_ITEM_ORB) - PlayerStashList[pnum][i][j].item_stack;
						PlayerStashList[pnum][i][j].item_stack = GetStackValue(DND_ITEM_ORB);
					}
					SyncItemData(pnum, j, DND_SYNC_ITEMSOURCE_STASH | (i << 16), 1, 1);
				}
				else if(PlayerStashList[pnum][i][j].item_type == DND_ITEM_ORB && PlayerStashList[pnum][i][j].item_subtype == otype) {
					if(amt - res < GetStackValue(DND_ITEM_ORB) - PlayerStashList[pnum][i][j].item_stack) {
						PlayerStashList[pnum][i][j].item_stack += amt - res;
						res = amt;
					}
					else {
						res += GetStackValue(DND_ITEM_ORB) - PlayerStashList[pnum][i][j].item_stack;
						PlayerStashList[pnum][i][j].item_stack = GetStackValue(DND_ITEM_ORB);
					}
					SyncItemStack(pnum, j, DND_SYNC_ITEMSOURCE_STASH | (i << 16));
				}
			}
		}
	}
}

//WORKAROUND: Function arrays are not supported. See: tracker/view.php?id=2472&nbn=5)
int perk_indexes[DND_PERKS];
int stat_indexes[6];

int HandleSinOrbBonus(int type) {
	int temp, loop = 0, i;
	int pnum = PlayerNumber();
	switch(type) {
		case SINORB_STAT:
			// give random stat points
			int stat_indexes_left = 6;
			stat_indexes[0] = STAT_STR;
			stat_indexes[1] = STAT_DEX;
			stat_indexes[2] = STAT_BUL;
			stat_indexes[3] = STAT_CHR;
			stat_indexes[4] = STAT_VIT;
			stat_indexes[5] = STAT_INT;
			 //Make sure the full 32-bit range is used
			//printbold(s:"starting vals - stats: ", d:Player_MostRecent_Orb[pnum].values[3], s:", ", d:Player_MostRecent_Orb[pnum].values[4]);
			for(i = 0; i < GetAffluenceBonus() * SINORB_STATGIVE; ++i) {
				while(stat_indexes_left > 0) {
					temp = random(0, stat_indexes_left-1);
					if (GetStat(stat_indexes[temp]) < DND_STAT_FULLMAX)
						break;
					stat_indexes[temp] = stat_indexes[--stat_indexes_left];
				}
				if (stat_indexes_left > 0) {
					GiveStat(stat_indexes[temp], 1);
					// 6 stats, can give max of 64 on 1 stat. 8 x 6 = 48 bits needed. 2 ints
					if(stat_indexes[temp] < STAT_INT)
						Player_MostRecent_Orb[pnum].values[3] += pow(65, stat_indexes[temp]);
					else
						Player_MostRecent_Orb[pnum].values[4] += 1;
				}
				else {
					GiveInventory("StatPoint", 1);
					Player_MostRecent_Orb[pnum].values[4] += 65;
				}
			}
			//printbold(s:"ending vals - stats: ", d:Player_MostRecent_Orb[pnum].values[3], s:", ", d:Player_MostRecent_Orb[pnum].values[4]);
		return SINORB_STATGIVE;
		case SINORB_PERK:
			int perk_indexes_left = DND_PERKS;
			perk_indexes[0] = STAT_SHRP;
			perk_indexes[1] = STAT_END;
			perk_indexes[2] = STAT_WIS;
			perk_indexes[3] = STAT_GRE;
			perk_indexes[4] = STAT_MED;
			perk_indexes[5] = STAT_MUN;
			perk_indexes[6] = STAT_DED;
			perk_indexes[7] = STAT_SAV;
			perk_indexes[8] = STAT_LUCK;
			for(i = 0; i < GetAffluenceBonus() * SINORB_PERKGIVE; ++i) {
				while(perk_indexes_left > 0) {
					temp = random(0, perk_indexes_left - 1);
					//printbold(s:"temp: ",d:temp,s:"perk: ",d:perk_indexes[temp],s:"left: ",d:perk_indexes_left);
					if (GetStat(perk_indexes[temp]) < DND_PERK_MAX)
						break;
					perk_indexes[temp] = perk_indexes[--perk_indexes_left];
				}
				if (perk_indexes_left > 0) {
					GiveStat(perk_indexes[temp], 1);
					// int_max is 10 digits, max give is 16
					if (perk_indexes[temp] < STAT_LUCK)
						Player_MostRecent_Orb[pnum].values[3] += pow(11, perk_indexes[temp] - DND_PERK_BEGIN);
					else
						Player_MostRecent_Orb[pnum].values[4] += 1;
				} else {
					GiveInventory("PerkPoint", 1);
					UpdateActivity(pnum, DND_ACTIVITY_PERKPOINT, 1, -1);
					Player_MostRecent_Orb[pnum].values[4] += 11;
				}
			}
			//printbold(s:"ending vals - perks: ", d:Player_MostRecent_Orb[pnum].val4, s:", ", d:Player_MostRecent_Orb[pnum].val5);
		return SINORB_PERKGIVE;
		case SINORB_CRIT:
			do {
				temp = PickRandomOwnedWeapon();
			} while(GetDataFromOrbBonus(pnum, OBI_WEAPON_CRIT, temp) >= SINORB_CRITMAX && loop++ != MAX_ITER);
			i = GetDataFromOrbBonus(pnum, OBI_WEAPON_CRIT, temp);
			Player_MostRecent_Orb[pnum].values[4] = temp;
			Player_MostRecent_Orb[pnum].values[3] = i;
			SetDataToOrbBonus(pnum, OBI_WEAPON_CRIT, temp, Clamp_Between(i + SINORB_CRITGIVE * pow(2, CheckInventory("AffluenceCounter")), 0, SINORB_CRITMAX));
			// get the difference, should be negative
			Player_MostRecent_Orb[pnum].values[3] -= GetDataFromOrbBonus(pnum, OBI_WEAPON_CRIT, temp);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_CRIT, temp);
		return temp;
		case SINORB_CRITDMG:
			do {
				temp = PickRandomOwnedWeapon();
			} while(GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, temp) >= SINORB_CRITDMGMAX && loop++ != MAX_ITER);
			i = GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, temp);
			Player_MostRecent_Orb[pnum].values[4] = temp;
			Player_MostRecent_Orb[pnum].values[3] = i;
			SetDataToOrbBonus(pnum, OBI_WEAPON_CRITDMG, temp, Clamp_Between(i + SINORB_CRITDMGGIVE * pow(2, CheckInventory("AffluenceCounter")), 0, SINORB_CRITDMGMAX));
			Player_MostRecent_Orb[pnum].values[3] -= GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, temp);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_CRITDMG, temp);
		return temp;
	}
	return -1;
}

void RevertLastOrbEffect() {
	int pnum = PlayerNumber(), i, temp;
	switch(Player_MostRecent_Orb[pnum].orb_type - 1) {
		case DND_ORB_ENHANCE:
			// val 1 is wepid, val2 is overall gain
			AddOrbBonusData(pnum, OBI_WEAPON_ENCHANT, Player_MostRecent_Orb[pnum].values[0], -Player_MostRecent_Orb[pnum].values[1]);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEAPONENHANCE, Player_MostRecent_Orb[pnum].values[0]);
		break;
		case DND_ORB_SPIRIT:
			TakeStat(Player_MostRecent_Orb[pnum].values[0], Player_MostRecent_Orb[pnum].values[1]);
		break;
		case DND_ORB_AFFLUENCE:
			TakeInventory("AffluenceCounter", 1);
		break;
		case DND_ORB_CORRUPT:
			UndoCorruptOrbEffect();
		break;
		case DND_ORB_CALAMITY:
			// find out how many this player does really have left, and give back that many (will give none if you used up all!)
			i = TakeOrbFromPlayer(Player_MostRecent_Orb[pnum].values[0] / 100, Player_MostRecent_Orb[pnum].values[1]);
			ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, Player_MostRecent_Orb[pnum].values[0] % 100, i);
		break;
		case DND_ORB_PROSPERITY:
			SetDataToOrbBonus(pnum, OBI_HPFLAT, -1, Player_MostRecent_Orb[pnum].values[0]);
			SetDataToOrbBonus(pnum, OBI_ARMORFLAT, -1, Player_MostRecent_Orb[pnum].values[0]);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_HPFLAT_BONUS, 0);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_ARMORFLAT_BONUS, 0);
		break;
		case DND_ORB_FORTITUDE:
			SetDataToOrbBonus(pnum, OBI_HPPERCENT, -1, Player_MostRecent_Orb[pnum].values[0]);
			SetDataToOrbBonus(pnum, OBI_ARMORPERCENT, -1, Player_MostRecent_Orb[pnum].values[0]);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_HPPERCENT_BONUS, 0);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_ARMORPERCENT_BONUS, 0);
		break;
		case DND_ORB_WISDOM:
			SetDataToOrbBonus(pnum, OBI_WISDOMPERCENT, -1, Player_MostRecent_Orb[pnum].values[0]);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WISDOMPERCENT_BONUS, 0);
		break;
		case DND_ORB_GREED:
			SetDataToOrbBonus(pnum, OBI_GREEDPERCENT, -1, Player_MostRecent_Orb[pnum].values[0]);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_GREEDPERCENT_BONUS, 0);
		break;
		case DND_ORB_VIOLENCE:
			SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, Player_MostRecent_Orb[pnum].values[0] % 100, Player_MostRecent_Orb[pnum].values[0] / 100);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_DAMAGEBULLET + (Player_MostRecent_Orb[pnum].values[0] % 100), 0);
		break;
		case DND_ORB_SIN:
			// give back stats
			for(i = STAT_STR; i <= STAT_INT; ++i) {
				if(i < 4) {
					GiveStat(i, Player_MostRecent_Orb[pnum].values[0] & SINORB_STATSAVEMASK);
					Player_MostRecent_Orb[pnum].values[0] >>= SINORB_STATSAVEBITS;
				}
				else {
					GiveStat(i, Player_MostRecent_Orb[pnum].values[1] & SINORB_STATSAVEMASK);
					Player_MostRecent_Orb[pnum].values[1] >>= SINORB_STATSAVEBITS;
				}
			}
			// take away the property given by sin orb
			UndoSinOrbEffect();
		break;
		case DND_ORB_HOLDING:
			AddOrbBonusData(pnum, OBI_HOLDING, -1, -Player_MostRecent_Orb[pnum].values[0]);
			SetAllAmmoCapacities();
			SyncClientsideVariable_Orb(pnum, DND_SYNC_HOLDING, 0);
		break;
		case DND_ORB_REFINEMENT:
			// item index
			temp = Player_MostRecent_Orb[pnum].p_tempwep - 1;
			for(i = 0; i < PlayerInventoryList[pnum][temp].attrib_count; ++i)
				PlayerInventoryList[pnum][temp].attributes[i].attrib_val = Player_MostRecent_Orb[pnum].values[i];
			SyncItemAttributes(pnum, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		break;
		case DND_ORB_SCULPTING:
			temp = Player_MostRecent_Orb[pnum].p_tempwep - 1;
			PlayerInventoryList[pnum][temp].attrib_count = Player_MostRecent_Orb[pnum].values[0];
			for(i = 0; i < Player_MostRecent_Orb[pnum].values[0]; ++i) {
				PlayerInventoryList[pnum][temp].attributes[i].attrib_id = Player_MostRecent_Orb[pnum].values[2 * i + 1];
				PlayerInventoryList[pnum][temp].attributes[i].attrib_val = Player_MostRecent_Orb[pnum].values[2 * i + 2];
			}
			SyncItemAttributes(pnum,temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		break;
		case DND_ORB_ELEVATION:
		case DND_ORB_HOLLOW:
			temp = Player_MostRecent_Orb[pnum].p_tempwep - 1;
			PlayerInventoryList[pnum][temp].attrib_count = Player_MostRecent_Orb[pnum].values[0];
			for(i = 0; i < Player_MostRecent_Orb[pnum].values[0]; ++i) {
				PlayerInventoryList[pnum][temp].attributes[i].attrib_id = Player_MostRecent_Orb[pnum].values[2 * i + 1];
				PlayerInventoryList[pnum][temp].attributes[i].attrib_val = Player_MostRecent_Orb[pnum].values[2 * i + 2];
			}
			SyncItemAttributes(pnum, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		break;
		case DND_ORB_PHANTASMAL:
			temp = Player_MostRecent_Orb[pnum].values[0];
			SetDataToOrbBonus(pnum, OBI_WEAPON_POWERSET1, temp, -WEP_POWER_GHOSTHIT);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_POWERSET1, temp);
		break;
		
	}
	SetInventory("OrbResult", Player_MostRecent_Orb[pnum].orb_type - 1);
	ResetMostRecentOrb(pnum);
}

void UndoSinOrbEffect() {
	// val3 holds affluence on first 5 bits
	int pnum = PlayerNumber(), i, temp;
	switch ((Player_MostRecent_Orb[pnum].values[2] & 0b111100000) >> 5) {
		case SINORB_STAT:
			// val4 and val5 hold stat values that we currently have gained, so we take them away
			for(i = STAT_STR; i < STAT_INT; ++i) {
				TakeStat(i, Player_MostRecent_Orb[pnum].values[3] % 65);
				Player_MostRecent_Orb[pnum].values[3] /= 65;
			}
			TakeStat(STAT_INT, Player_MostRecent_Orb[pnum].values[4] % 65);
			Player_MostRecent_Orb[pnum].values[4] /= 65;
			//Remove allocated stats randomly until enough perk points are available
			//Unlike the orb of sin use, there's no way to have less stats then the ones being taken - thus loop can be simpler
			
			while (CheckInventory("StatPoint") < (Player_MostRecent_Orb[pnum].values[4] % 65)) {
				temp = random(STAT_STR, STAT_INT);
				if (GetStat(temp) > 0) {
					TakeStat(temp, 1);
					GiveInventory("StatPoint", 1); }}
			TakeInventory("StatPoint", Player_MostRecent_Orb[pnum].values[4]);
		break;
		case SINORB_PERK:
			for(i = DND_PERK_BEGIN; i < STAT_LUCK-1; ++i) {
				TakeStat(i, Player_MostRecent_Orb[pnum].values[3] % 11);
				Player_MostRecent_Orb[pnum].values[3] /= 11;
			}
			TakeStat(STAT_LUCK, Player_MostRecent_Orb[pnum].values[4] % 11);
			Player_MostRecent_Orb[pnum].values[4] /= 11;
			
			//Remove allocated perks randomly until enough perk points are available
			//Unlike the orb of sin use, there's no way to have less perks then the ones being taken - thus loop can be simpler
			while (CheckInventory("PerkPoint") < Player_MostRecent_Orb[pnum].values[4]) {
				temp = random(STAT_SHRP, STAT_LUCK);
				if (GetStat(temp) > 0) {
					TakeStat(temp, 1);
					GiveInventory("PerkPoint", 1);
					UpdateActivity(pnum, DND_ACTIVITY_PERKPOINT, 1, -1);
				}
			}
			TakeInventory("PerkPoint", Player_MostRecent_Orb[pnum].values[4]);
			UpdateActivity(pnum, DND_ACTIVITY_PERKPOINT, -Player_MostRecent_Orb[pnum].values[4], -1);
			// after operation perk checks
			UpdatePerkStuff();
		break;
		case SINORB_CRIT:
			//printbold(d: Player_MostRecent_Orb[pnum].val5, s: " ", f:Player_Weapon_Infos[pnum][Player_MostRecent_Orb[pnum].val5].WEP_MODes[WEP_MOD_CRIT].amt, s: " ", f:Player_MostRecent_Orb[pnum].val4);
			AddOrbBonusData(pnum, OBI_WEAPON_CRIT, Player_MostRecent_Orb[pnum].values[4], Player_MostRecent_Orb[pnum].values[3]);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_CRIT, Player_MostRecent_Orb[pnum].values[4]);
		break;
		case SINORB_CRITDMG:
			AddOrbBonusData(pnum, OBI_WEAPON_CRITDMG, Player_MostRecent_Orb[pnum].values[4], Player_MostRecent_Orb[pnum].values[3]);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_CRITDMG, Player_MostRecent_Orb[pnum].values[4]);
		break;
	}
}

// picks a random orb
int PickRandomOrb() {
	int i;
	do {
		i = random(DND_ORB_ENHANCE, MAX_ORBS - 1);
	} while(IsOrbDropException(i));
	return i;
}

int PickRandomTalent() {
	return random(TALENT_BULLET, MAX_TALENTS - 1);
}

int PickRandomStat() {
	return random(STAT_STR, STAT_INT);
}

int GetAffluenceBonus() {
	return 1 << CheckInventory("AffluenceCounter");
}

int GetRandomSinStat() {
	int s = STAT_STR;
	int runs = 0;
	do {
		s = PickRandomStat();
		// unlucky rng counter measures
		if(++runs > SINORB_MAXRUNS) {
			for(runs = STAT_STR; runs <= STAT_INT; ++runs) {
				if(GetStat(runs))
					return runs;
			}
			break;
		}
	} while(!GetStat(s));
	return s;
}

void RecentOrb_SaveOldStats(int pnum) {
	// 0 to 5, need 8 bits
	for(int i = STAT_STR; i <= STAT_INT; ++i) {
		if(i < 4)
			Player_MostRecent_Orb[pnum].values[0] |= GetStat(i) << (i * SINORB_STATSAVEBITS);
		else
			Player_MostRecent_Orb[pnum].values[1] |= GetStat(i) << ((i - 4) * SINORB_STATSAVEBITS);
	}
}

void RecentOrb_FindDifference(int pnum) {
	// 0 to 5
	int tmp1 = Player_MostRecent_Orb[pnum].values[0], tmp2 = Player_MostRecent_Orb[pnum].values[1];
	Player_MostRecent_Orb[pnum].values[0] = Player_MostRecent_Orb[pnum].values[1] = 0;
	for(int i = STAT_STR; i <= STAT_INT; ++i) {
		if(i < 4) {
			Player_MostRecent_Orb[pnum].values[0] |= ((tmp1 & SINORB_STATSAVEMASK) - GetStat(i)) << (i * SINORB_STATSAVEBITS);
			tmp1 >>= SINORB_STATSAVEBITS;
		}
		else {
			Player_MostRecent_Orb[pnum].values[1] |= ((tmp2 & SINORB_STATSAVEMASK) - GetStat(i)) << ((i - 4) * SINORB_STATSAVEBITS);
			tmp2 >>= SINORB_STATSAVEBITS;
		}
	}
}

void DoSinOrbMessage(int val, int affluence) {
	int pts = (val & 0x7F) * affluence;
	str temp = "";
	if(pts > 1)
		temp = StrParam(s:"\cj", l:"DND_ORBUSETEXT12A", s:" \cg", d:pts, s:"\cj ", l:"DND_ORBUSETEXT12B", s:" ");
	else
		temp = StrParam(s:"\cj", l:"DND_ORBUSETEXT12A", s:" \cg", d:pts, s:"\cj ", l:"DND_ORBUSETEXT12C", s:" ");
	//Log(s:"type ", d: (val >> 8) & 0x7);
	switch((val >> 8) & 0x7) {
		case SINORB_STAT:
			Log(s:temp, l:"DND_ORB_ANDGRANTS", s:" \cd", d:(val >> 11) * affluence, s:" ", l:"DND_ORB_STATPOINTS", s:"!");
		break;
		case SINORB_PERK:
			pts = (val >> 11) * affluence;
			if(pts > 1)
				Log(s:temp, l:"DND_ORB_ANDGRANTS", s:" \cd", d:pts, s:" ", l:"DND_ORB_RANDOMPERKS", s:"!");
			else
				Log(s:temp, l:"DND_ORB_ANDGRANTS", s:" \cd", d:pts, s:" ", l:"DND_ORB_RANDOMPERK", s:"!");
		break;
		case SINORB_CRIT:
			Log(s:temp, l:"DND_ORB_ANDGRANTS", s:" \cd", f:ftrunc(SINORB_CRITGIVE * affluence * 100), s:"% ", l:"DND_ORBUSETEXT2I", s:" \cd", l:GetWeaponTag(val >> 11), s:"\c-!");
		break;
		case SINORB_CRITDMG:
			Log(s:temp, l:"DND_ORB_ANDGRANTS", s:" \cd", d:SINORB_CRITDMGGIVE * affluence, s:"% ", l:"DND_ORBUSETEXT2J", s:" \cd", l:GetWeaponTag(val >> 11), s:"\c-!");
		break;
	}
}

bool HasWeaponWithoutCritDmgMax() {
	int pnum = PlayerNumber();
	for(int i = 0; i < MAXWEPS; ++i) {
		if(GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, i) < CORRUPTORB_MAXCRITDMG)
			return 1;
	}
	return 0;
}

bool HasUnmaxedStats() {
	for(int i = STAT_STR; i <= STAT_INT; ++i) {
		if(GetStat(i) < DND_STAT_FULLMAX)
			return 1;
	}
	return 0;
}

// do bad stuff as much as you can (except map duration dmg reduction, do it only once in the map)
bool CorruptionMaxChecks(int effect) {
	switch(effect) {
		case CORRUPTORB_NOTHING:
		case CORRUPTORB_TAKEAMMO:
		return 1; 
		
		case CORRUPTORB_TAKEBACKPACK:
		return CheckInventory("BackpackCounter"); // take only if we have at least 1
		
		case CORRUPTORB_TAKEDMG:
		return GetDataFromOrbBonus(PlayerNumber(), OBI_WEAPON_DMG, GetWeaponPosFromTable()) > CORRUPTORB_MINDMG;
		
		case CORRUPTORB_TAKEHP:
		return GetSpawnHealth() > CORRUPTORB_MINHEALTH;
		
		case CORRUPTORB_TAKESPEED:
		return GetDataFromOrbBonus(PlayerNumber(), OBI_SPEED, -1) > CORRUPTORB_MINSPEED;
		
		case CORRUPTORB_REDUCEDMGMAP:
		return !CheckInventory("CorruptOrb_DamageReduction");
		
		// good ones
		
		case CORRUPTORB_ADDCRIT:
		return GetDataFromOrbBonus(PlayerNumber(), OBI_WEAPON_CRITPERCENT, GetWeaponPosFromTable()) < CORRUPTORB_MAXCRITPERCENT;
	
		case CORRUPTORB_ADDDMG:
		return GetDataFromOrbBonus(PlayerNumber(), OBI_WEAPON_DMG, GetWeaponPosFromTable()) < CORRUPTORB_MAXDMG;
	
		case CORRUPTORB_ADDCRITDMG:
		return HasWeaponWithoutCritDmgMax();
		
		case CORRUPTORB_ADDSPEED:
		return GetDataFromOrbBonus(PlayerNumber(), OBI_SPEED, -1) < CORRUPTORB_MAXSPEED;
		
		case CORRUPTORB_DROPCHANCE:
		return GetDataFromOrbBonus(PlayerNumber(), OBI_SPEED, -1) < CORRUPTORB_MAXDROPCHANCE;
		
		case CORRUPTORB_GIVESTAT:
		return HasUnmaxedStats();
		
		case CORRUPTORB_MOD_PERCENTDAMAGE:
		case CORRUPTORB_MOD_POISONFORPERCENTDAMAGE:
		case CORRUPTORB_MOD_FORCEPAINCHANCE:
		return 1;
	}
	return 1;
}

void HandleCorruptOrbUse(int type) {
	int i;
	int pnum = PlayerNumber(), temp = 0;
	switch(type) {
		case CORRUPTORB_TAKEAMMO:
			for(i = 0; i < MAX_SLOTS; ++i)
				for(int j = 0; j < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[i][j].initial_capacity != -1; ++j) {
					Player_MostRecent_Orb[pnum].p_ammos[i][j] = CheckInventory(AmmoInfo_Str[i][j][AMMOINFO_NAME]);
					SetInventory(AmmoInfo_Str[i][j][AMMOINFO_NAME], 0);
				}
			for(i = 0; i < MAXTEMPWEPS; ++i) {
				if(CheckInventory(TemporaryWeaponData[i][TEMPWEP_AMMO])) {
					Player_MostRecent_Orb[pnum].p_tempammo = CheckInventory(TemporaryWeaponData[i][TEMPWEP_AMMO]);
					Player_MostRecent_Orb[pnum].p_tempwep = i;
					SetInventory(TemporaryWeaponData[i][TEMPWEP_AMMO], 0);
					break;
				}
			}
		break;
		case CORRUPTORB_TAKEBACKPACK:
			Player_MostRecent_Orb[pnum].values[2] = CheckInventory("BackpackCounter");
			TakeInventory("BackpackCounter", GetAffluenceBonus());
			Player_MostRecent_Orb[pnum].values[2] -= CheckInventory("BackpackCounter");
		break;
		case CORRUPTORB_TAKEDMG:
			// we hold the difference while we can
			temp = CORRUPTORB_DMGTAKE * GetAffluenceBonus();
			i = PickRandomOwnedWeaponID();
			Player_MostRecent_Orb[pnum].values[3] = i;
			if(GetDataFromOrbBonus(pnum, OBI_WEAPON_DMG, i) - temp > CORRUPTORB_MINDMG) {
				Player_MostRecent_Orb[pnum].values[2] = temp;
				AddOrbBonusData(pnum, OBI_WEAPON_DMG, i, -temp);
			}
			else {
				AddOrbBonusData(pnum, OBI_WEAPON_DMG, i, -CORRUPTORB_MINDMG);
				Player_MostRecent_Orb[pnum].values[2] = GetDataFromOrbBonus(pnum, OBI_WEAPON_DMG, i);
				SetDataToOrbBonus(pnum, OBI_WEAPON_DMG, i, CORRUPTORB_MINDMG);
			}
			SetInventory("OrbResult", CheckInventory("OrbResult") | i << 8);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_DMG, Player_MostRecent_Orb[pnum].values[3]);
		break;
		case CORRUPTORB_TAKEHP:
			temp = CORRUPTORB_HPTAKE * GetAffluenceBonus();
			if(GetSpawnHealth() - temp > CORRUPTORB_MINHEALTH) {
				Player_MostRecent_Orb[pnum].values[2] = temp;
				AddOrbBonusData(pnum, OBI_HPFLAT, -1, -temp);
			}
			else {
				Player_MostRecent_Orb[pnum].values[2] = GetSpawnHealth() - CORRUPTORB_MINHEALTH;
				SetDataToOrbBonus(pnum, OBI_HPFLAT, -1, CORRUPTORB_MINHEALTH);
			}
			SyncClientsideVariable_Orb(pnum, DND_SYNC_HPFLAT_BONUS, 0);
		break;
		case CORRUPTORB_TAKESPEED:
			temp = CORRUPTORB_SPEEDTAKE * GetAffluenceBonus();
			if(GetDataFromOrbBonus(pnum, OBI_SPEED, -1) - temp > CORRUPTORB_MINSPEED) {
				Player_MostRecent_Orb[pnum].values[2] = temp;
				AddOrbBonusData(pnum, OBI_SPEED, -1, -temp);
			}
			else {
				Player_MostRecent_Orb[pnum].values[2] = GetDataFromOrbBonus(pnum, OBI_SPEED, -1) - CORRUPTORB_MINSPEED;
				SetDataToOrbBonus(pnum, OBI_SPEED, -1, CORRUPTORB_MINSPEED);
			}
			RestoreRPGStat(RES_PLAYERSPEED);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_SPEED, 0);
		break;
		case CORRUPTORB_REDUCEDMGMAP:
			GiveInventory("CorruptOrb_DamageReduction", 1);
		break;
		
		case CORRUPTORB_ADDDMG:
			temp = CORRUPTORB_DMGGIVE * GetAffluenceBonus();
			i = PickRandomOwnedWeaponID();
			Player_MostRecent_Orb[pnum].values[3] = i;
			Player_MostRecent_Orb[pnum].values[2] = GetDataFromOrbBonus(pnum, OBI_WEAPON_DMG, i);
			SetDataToOrbBonus(pnum, OBI_WEAPON_DMG, i, Clamp_Between(GetDataFromOrbBonus(pnum, OBI_WEAPON_DMG, i) + temp, 0, CORRUPTORB_MAXDMG));
			Player_MostRecent_Orb[pnum].values[2] -= GetDataFromOrbBonus(pnum, OBI_WEAPON_DMG, i);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_DMG, Player_MostRecent_Orb[pnum].values[3]);
			SetInventory("OrbResult", CheckInventory("OrbResult") | i << 8);
		break;
		case CORRUPTORB_ADDCRIT:
			temp = CORRUPTORB_CRITGIVE * GetAffluenceBonus();
			i = PickRandomOwnedWeaponID();
			Player_MostRecent_Orb[pnum].values[3] = i;
			Player_MostRecent_Orb[pnum].values[2] = GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITPERCENT, i);
			SetDataToOrbBonus(pnum, OBI_WEAPON_CRITPERCENT, i, Clamp_Between(GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITPERCENT, i) + temp, 0, CORRUPTORB_MAXCRITPERCENT));
			Player_MostRecent_Orb[pnum].values[2] -= GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITPERCENT, i);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_CRITPERCENT, Player_MostRecent_Orb[pnum].values[3]);
			SetInventory("OrbResult", CheckInventory("OrbResult") | i << 8);
		break;
		case CORRUPTORB_ADDCRITDMG:
			temp = CORRUPTORB_CRITDMGGIVE * GetAffluenceBonus();
			i = PickRandomOwnedWeaponID();
			Player_MostRecent_Orb[pnum].values[3] = i;
			Player_MostRecent_Orb[pnum].values[2] = GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, i);
			SetDataToOrbBonus(pnum, OBI_WEAPON_CRITDMG, i, Clamp_Between(GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, i) + temp, 0, CORRUPTORB_MAXCRITDMG));
			Player_MostRecent_Orb[pnum].values[2] -= GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, i);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_CRITDMG, Player_MostRecent_Orb[pnum].values[3]);
			SetInventory("OrbResult", CheckInventory("OrbResult") | i << 8);
		break;
		case CORRUPTORB_ADDSPEED:
			temp = CORRUPTORB_SPEEDGIVE * GetAffluenceBonus();
			Player_MostRecent_Orb[pnum].values[2] = GetDataFromOrbBonus(pnum, OBI_SPEED, -1);
			SetDataToOrbBonus(pnum, OBI_SPEED, -1, Clamp_Between(GetDataFromOrbBonus(pnum, OBI_SPEED, -1) + temp, 0, CORRUPTORB_MAXSPEED));
			Player_MostRecent_Orb[pnum].values[2] -= GetDataFromOrbBonus(pnum, OBI_SPEED, -1);
			RestoreRPGStat(RES_PLAYERSPEED);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_SPEED, 0);
		break;
		case CORRUPTORB_DROPCHANCE:
			temp = CORRUPTORB_DROPCHANCEGIVE * GetAffluenceBonus();
			Player_MostRecent_Orb[pnum].values[2] = GetDataFromOrbBonus(pnum, OBI_DROPCHANCE, -1);
			SetDataToOrbBonus(pnum, OBI_DROPCHANCE, -1, Clamp_Between(GetDataFromOrbBonus(pnum, OBI_DROPCHANCE, -1) + temp, 0, CORRUPTORB_MAXDROPCHANCE));
			Player_MostRecent_Orb[pnum].values[2] -= GetDataFromOrbBonus(pnum, OBI_DROPCHANCE, -1);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_DROPCHANCE, 0);
		break;
		case CORRUPTORB_GIVESTAT:
			do {
				temp = random(0, STATORB_MAXWEIGHT);
				for(i = 0; i < STATORB_MAXSTATS; ++i)
					if(temp <= SpiritOrb_StatDropWeights[i])
						break;
			} while(GetStat(i) == DND_STAT_FULLMAX);
			GiveStat(i, CORRUPTORB_STATGIVE * GetAffluenceBonus());
			Player_MostRecent_Orb[pnum].values[2] = i;
			SetInventory("OrbResult", CheckInventory("OrbResult") | i << 8);
		break;
		case CORRUPTORB_MOD_PERCENTDAMAGE:
		case CORRUPTORB_MOD_POISONFORPERCENTDAMAGE:
		case CORRUPTORB_MOD_FORCEPAINCHANCE:
			int tier = CheckInventory("AffluenceCounter");
			if(tier > MAX_WEP_MODTIER)
				tier = MAX_WEP_MODTIER;
			
			// mod order is the same as defined in dnd_weaponmods.h
			// this is the value, we know the tier now roll a weapon to apply this to
			i = PickRandomOwnedWeaponID();
			temp = RollWeaponMod(type - CORRUPTORB_WEAPONMOD_BEGIN, tier);
			Player_MostRecent_Orb[pnum].values[2] = i;
			Player_MostRecent_Orb[pnum].values[3] = temp;
			Player_MostRecent_Orb[pnum].values[4] = tier;
			
			// give the mod -- index to 0 base
			type = type + WEP_MOD_PERCENTDAMAGE - CORRUPTORB_MOD_PERCENTDAMAGE;
			Player_Weapon_Infos[pnum][i].wep_mods[type].tier = tier;
			Player_Weapon_Infos[pnum][i].wep_mods[type].val = temp;
			
			SyncClientsideVariable_WeaponMods(pnum, i);
			SetInventory("OrbResult", CheckInventory("OrbResult") | i << 8);
		break;
	}
}

void DoCorruptOrbMessage(int val, int affluence) {
	// val just has the option, except for stat case which has which stat it is << 8
	switch(val & 0xF) {
		case CORRUPTORB_NOTHING:
			Log(s:"\cj", l:"DND_ORBUSETEXT2A");
		break;
		case CORRUPTORB_TAKEAMMO:
			Log(s:"\cj", l:"DND_ORBUSETEXT2B");
		break;
		case CORRUPTORB_TAKEBACKPACK:
			Log(s:"\cj", l:"DND_ORBUSETEXT2C", s:" ", d:affluence, s:" ", l:"DND_ORBUSETEXT2D");
		break;
		case CORRUPTORB_TAKEDMG:
			Log(s:"\cj", l:"DND_ORBUSETEXT2C", s:" ", d:CORRUPTORB_DMGTAKE * affluence, s:"% ", l:"DND_DAMAGE", s:"\cj ", l:"DND_FROM", s:" \cv", l:GetWeaponTag(val >> 8), s:"\cj!");
		break;
		case CORRUPTORB_REDUCEDMGMAP:
			Log(s:"\cj", l:"DND_ORBUSETEXT2E");
		break;
		case CORRUPTORB_TAKEHP:
			Log(s:"\cj", l:"DND_ORBUSETEXT2F", s:" ", d:CORRUPTORB_HPTAKE * affluence, s:"\cj!");
		break;
		case CORRUPTORB_TAKESPEED:
			Log(s:"\cj", l:"DND_ORBUSETEXT2G", s:" ", f:ftrunc(CORRUPTORB_SPEEDTAKE * affluence * 100), s:"%\cj!");
		break;
		
		case CORRUPTORB_ADDCRIT:
			Log(s:"\cj", l:"DND_ORBUSETEXT2H", s:" \cv", f:ftrunc(CORRUPTORB_CRITGIVE * affluence * 100), s:"%\cj ", l:"DND_ORBUSETEXT2I", s:" \cv", l:GetWeaponTag(val >> 8), s:"\cj!");
		break;
		case CORRUPTORB_ADDCRITDMG:
			Log(s:"\cj", l:"DND_ORBUSETEXT2H", s:" \cv", d:CORRUPTORB_CRITDMGGIVE * affluence, s:"%\cj ", l:"DND_ORBUSETEXT2J", s:" ", l:GetWeaponTag(val >> 8), s:"\cj!");
		break;
		case CORRUPTORB_ADDDMG:
			Log(s:"\cj", l:"DND_ORBUSETEXT2H", s:" \cv", d:CORRUPTORB_DMGGIVE * affluence, s:"%\cj ", l:"DND_ORBUSETEXT2K", s:" ", l:GetWeaponTag(val >> 8), s:"\cj!");
		break;
		case CORRUPTORB_ADDSPEED:
			Log(s:"\cj", l:"DND_ORBUSETEXT2H", s:" \cv", f:ftrunc(CORRUPTORB_SPEEDGIVE * affluence * 100), s:"%\cj ", l:"DND_ORBUSETEXT2L", s:"!");
		break;
		case CORRUPTORB_DROPCHANCE:
			Log(s:"\cj", l:"DND_ORBUSETEXT2M", s:" \cv", f:ftrunc(CORRUPTORB_DROPCHANCEGIVE * affluence * 100), s:"%\cj!");
		break;
		case CORRUPTORB_GIVESTAT:
			Log(s:"\cj", l:"DND_ORBUSETEXT2N", s:" \cv", d:CORRUPTORB_STATGIVE * affluence, s:" ", l:GetStatLabel(val >> 8), s:"\cj ", l:"DND_POINTS", s:"!");
		break;
		case CORRUPTORB_MOD_PERCENTDAMAGE:
			Log(s:"\cj", l:"DND_ORBUSETEXT2O", s:" ", l:GetWeaponTag(val >> 8), s:"\cj!");
		break;
		case CORRUPTORB_MOD_POISONFORPERCENTDAMAGE:
			Log(s:"\cj", l:"DND_ORBUSETEXT2P", s:" ", l:GetWeaponTag(val >> 8), s:"\cj!");
		break;
		case CORRUPTORB_MOD_FORCEPAINCHANCE:
			Log(s:"\cj", l:"DND_ORBUSETEXT2Q", s:" ", l:GetWeaponTag(val >> 8), s:"\cj!");
		break;
	}
}

void UndoCorruptOrbEffect() {
	// if type is take, we give, if type is give, we take!
	int i;
	int pnum = PlayerNumber();
	int type = Player_MostRecent_Orb[pnum].values[0] & 0xF;
	switch(type) {
		case CORRUPTORB_TAKEAMMO:
			for(i = 0; i < MAX_SLOTS; ++i)
				for(int j = 0; j < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[i][j].initial_capacity != -1; ++j)
					GiveInventory(AmmoInfo_Str[i][j][AMMOINFO_NAME], Player_MostRecent_Orb[pnum].p_ammos[i][j]);
			// try to give this temp weapon if only player doesn't have a temp wep
			if(HasNoTempWeapon()) {
				GiveInventory(TemporaryWeaponData[Player_MostRecent_Orb[pnum].p_tempwep][TEMPWEP_AMMO], Player_MostRecent_Orb[pnum].p_tempammo);
				GiveInventory(TemporaryWeaponData[Player_MostRecent_Orb[pnum].p_tempwep][TEMPWEP_NAME], 1);
			}
		break;
		case CORRUPTORB_TAKEHP:
			AddOrbBonusData(pnum, OBI_HPFLAT, -1, Player_MostRecent_Orb[pnum].values[2]);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_HPFLAT_BONUS, 0);
		break;
		case CORRUPTORB_TAKEBACKPACK:
			GiveInventory("BackpackCounter", Player_MostRecent_Orb[pnum].values[2]);
		break;
		case CORRUPTORB_TAKEDMG:
			AddOrbBonusData(pnum, OBI_WEAPON_DMG, Player_MostRecent_Orb[pnum].values[3], Player_MostRecent_Orb[pnum].values[2]);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_DMG, Player_MostRecent_Orb[pnum].values[3]);
		break;
		case CORRUPTORB_TAKESPEED:
			AddOrbBonusData(pnum, OBI_SPEED, -1, Player_MostRecent_Orb[pnum].values[2]);
			RestoreRPGStat(RES_PLAYERSPEED);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_SPEED, 0);
		break;
		case CORRUPTORB_REDUCEDMGMAP:
			TakeInventory("CorruptOrb_DamageReduction", 1);
		break;
		
		case CORRUPTORB_ADDCRIT:
			// since diff is meant to be negative, we now add it, not subtract it
			AddOrbBonusData(pnum, OBI_WEAPON_CRITPERCENT, Player_MostRecent_Orb[pnum].values[3], Player_MostRecent_Orb[pnum].values[2]);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_CRITPERCENT, Player_MostRecent_Orb[pnum].values[3]);
		break;
		case CORRUPTORB_ADDCRITDMG:
			AddOrbBonusData(pnum, OBI_WEAPON_CRITDMG, Player_MostRecent_Orb[pnum].values[3], Player_MostRecent_Orb[pnum].values[2]);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_CRITDMG, Player_MostRecent_Orb[pnum].values[3]);
		break;
		case CORRUPTORB_ADDDMG:
			AddOrbBonusData(pnum, OBI_WEAPON_DMG, Player_MostRecent_Orb[pnum].values[3], Player_MostRecent_Orb[pnum].values[2]);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_WEPMOD_DMG, Player_MostRecent_Orb[pnum].values[3]);
		break;
		case CORRUPTORB_ADDSPEED:
			AddOrbBonusData(pnum, OBI_SPEED, -1, Player_MostRecent_Orb[pnum].values[2]);
			RestoreRPGStat(RES_PLAYERSPEED);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_SPEED, 0);
		break;
		case CORRUPTORB_GIVESTAT:
			TakeStat(Player_MostRecent_Orb[pnum].values[2], CORRUPTORB_STATGIVE * (Player_MostRecent_Orb[pnum].values[0] >> 8));
		break;
		case CORRUPTORB_DROPCHANCE:
			AddOrbBonusData(pnum, OBI_DROPCHANCE, -1, Player_MostRecent_Orb[pnum].values[2]);
			SyncClientsideVariable_Orb(pnum, DND_SYNC_DROPCHANCE, 0);
		break;
		case CORRUPTORB_MOD_PERCENTDAMAGE:
		case CORRUPTORB_MOD_POISONFORPERCENTDAMAGE:
		case CORRUPTORB_MOD_FORCEPAINCHANCE:
			type = type + WEP_MOD_PERCENTDAMAGE - CORRUPTORB_MOD_PERCENTDAMAGE;
			i = Player_MostRecent_Orb[pnum].values[2];
			// 2 is wep id, 3 is mod's value, 4 is tier
			Player_Weapon_Infos[pnum][i].wep_mods[type].tier -= Player_MostRecent_Orb[pnum].values[4];
			Player_Weapon_Infos[pnum][i].wep_mods[type].val -= Player_MostRecent_Orb[pnum].values[3];
			SyncClientsideVariable_WeaponMods(pnum, i);
		break;
	}
}

void HandleOrbUseMessage(int orbtype, int val, int affluence) {
	if(ConsolePlayerNumber() != PlayerNumber()) 
		return;
	switch(orbtype) {
		case DND_ORB_ENHANCE:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT1A", s:" \cd", l:GetWeaponTag(val), s:"\cv ", l:"DND_ORBUSETEXT1B", s:" \cd", d:affluence * ENHANCEORB_BONUS, s:"%\c-!");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL1");
		break;
		case DND_ORB_CORRUPT:
			DoCorruptOrbMessage(val, affluence);
		break;
		case DND_ORB_SPIRIT:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT3", s:" \cv", d:affluence, s:" \cd", l:GetStatLabel(val), s:"\cv!");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL3");
		break;
		case DND_ORB_REPENT:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT4", s:" \cv", l:GetInventoryTag(val + ORBS_BEGIN), s:"\cj!");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL4");
		break;
		case DND_ORB_AFFLUENCE:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT5", s:" \cdx", d:affluence, s: "\c-!");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL5", s:". (\ck", d:1 << AFFLUENCE_MAX, s:"\cg)");
		break;
		case DND_ORB_CALAMITY:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT6", s:" \ck", l:GetInventoryTag((val % 100) + ORBS_BEGIN), s:"\c- ", s:"<====>", s:" \cd", l:GetInventoryTag((val / 100) + ORBS_BEGIN), s:"!");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL6");
		break;
		case DND_ORB_PROSPERITY:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT7A", s:" \cd", d:affluence, s:" ", l:"DND_ORBUSETEXT7B");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL7", s:"! (\ck+", d:PROSPERITY_MAX, s:"\c-)");
		break;
		case DND_ORB_FORTITUDE:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT8A", s:" \cd", d:affluence, s:"% ", l:"DND_ORBUSETEXT8B");
			else
				Log(s:"\cg", l:"", s:"! (\ck", d:FORTITUDE_MAX, s:"%\c-)");
		break;
		case DND_ORB_WISDOM:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT9A", s:" \cd", d:affluence, s:"% ", l:"DND_ORBUSETEXT9B");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL9", s:"! (\ck", d:WISDOMORB_MAX, s:"%\c-)");
		break;
		case DND_ORB_GREED:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT10A", s:" \cd", d:affluence, s:"% ", l:"DND_ORBUSETEXT10B");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL10", s:"! (\ck", d:GREEDORB_MAX, s:"%\c-)");
		break;
		case DND_ORB_VIOLENCE:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT11A", s:" \cd", d:affluence, s:"% ", l:GetTalentTag(val), s:" ", l:"DND_ORBUSETEXT11B");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL11", s:"! (\ck", d:VIOLENCEORB_MAX, s:"%\c-)");
		break;
		case DND_ORB_SIN:
			if(val != 0x7FFFFFFF)
				DoSinOrbMessage(val, affluence);
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL12", s:" \ck", d:SINORB_MAX_TAKE * affluence, s:"\c-!");
		break;
		case DND_ORB_RICHES:
			if(!(val >> 30)) // exp
				Log(s:"\cj", l:"DND_ORBUSETEXT13", s:" \cd", d:val & 0xFFFF, s:"\c- \ck% ", l:"DND_STAT16", s:"!");
			else if((val >> 30) == 1)
				Log(s:"\cj", l:"DND_ORBUSETEXT13", s:" \cd", d:val & 0xFFFF, s:"\c- \cf", l:"DND_STAT19", s:"\c-!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT13", s:" \cd", d:val & 0xFFFF, s:"k\c- \cu", l:"DND_MENU_BUDGET", s:"\c-!");
		break;
		case DND_ORB_HOLDING:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT14A", s:" \cd", d:val, s:"% ", l:"DND_ORBUSETEXT14B", s:"!");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL14", s:"! (\ck", d:HOLDING_MAX, s:"\c-)!");
		break;
		case DND_ORB_REFINEMENT:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT15A", s:" ", d:affluence, s:" ", l:"DND_ORB_TIMES", s:"!");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL15", s:"\c-!");
		break;
		case DND_ORB_SCULPTING:
			if(val != 0x7FFFFFFF) {
				if(affluence > 1)
					Log(s:"\cj", l:"DND_ORBUSETEXT16A", s:" \cd", d:affluence, s:"\c- ", l:"DND_ORB_ATTRIBUTES", s:"!");
				else
					Log(s:"\cj", l:"DND_ORBUSETEXT16B");
			}
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL16");
		break;
		case DND_ORB_ELEVATION:
			if(val != 0x7FFFFFFF) {
				if(affluence > 1)
					Log(s:"\cj", l:"DND_ORBUSETEXT17A", s:" \cd", d:affluence, s:"\c- ", l:"DND_ORB_ATTRIBUTES", s:"!");
				else
					Log(s:"\cj", l:"DND_ORBUSETEXT17B");
			}
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL17");
		break;
		case DND_ORB_HOLLOW:
			if(val != 0x7FFFFFFF) {
				if(affluence > 1)
					Log(s:"\cj", l:"DND_ORBUSETEXT18A", s:" \cd", d:affluence, s:"\c- ", l:"DND_ORB_ATTRIBUTES", s:"!");
				else
					Log(s:"\cj", l:"DND_ORBUSETEXT18B");
			}
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL18");
		break;
		case DND_ORB_PHANTASMAL:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT19A", s:" \cd", l:GetWeaponTag(val), s:"\cv ", l:"DND_ORBUSETEXT19B");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL19");
		break;
		case DND_ORB_ASSIMILATION:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT20");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL20");
		break;
	}
}

void ResetMostRecentOrb(int pnum) {
	int i;
	Player_MostRecent_Orb[pnum].orb_type = 0;
	for(i = 0; i < MAX_STORED_ORB_DATA; ++i)
		Player_MostRecent_Orb[pnum].values[i] = 0;
	for(i = 0; i < MAX_SLOTS; ++i)
		for(int j = 0; j < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[i][j].initial_capacity != -1; ++j)
			Player_MostRecent_Orb[pnum].p_ammos[i][j] = 0;
	Player_MostRecent_Orb[pnum].p_tempammo = 0;
	Player_MostRecent_Orb[pnum].p_tempwep = 0;
	UpdateArmorVisual();
}

void ResetOrbData(int pnum) {
	int i, j;
	// reset orb stats affecting player only
	Player_Orb_Data[pnum].orb_stat_bonuses.hp_flat_bonus = 0;
	Player_Orb_Data[pnum].orb_stat_bonuses.armor_flat_bonus = 0;
	Player_Orb_Data[pnum].orb_stat_bonuses.hp_percent_bonus = 0;
	Player_Orb_Data[pnum].orb_stat_bonuses.armor_percent_bonus = 0;
	Player_Orb_Data[pnum].orb_stat_bonuses.greed_percent_bonus = 0;
	Player_Orb_Data[pnum].orb_stat_bonuses.wisdom_percent_bonus = 0;
	Player_Orb_Data[pnum].orb_stat_bonuses.speed_bonus = 0;
	Player_Orb_Data[pnum].orb_stat_bonuses.drop_chance = 0;
	Player_Orb_Data[pnum].orb_stat_bonuses.holding = 0;
	
	for(i = 0; i < MAX_TALENTS; ++i)
		Player_Orb_Data[pnum].orb_stat_bonuses.damage_type_bonus[i] = 0;
	
	// reset orb stats affecting players' weapons
	for(i = 0; i < MAXWEPS; ++i) {
		Player_Orb_Data[pnum].weapon_stat_bonuses[i].quality = 0;
		// horrible naming convention here, clean up later?
		for(j = 0; j < MAX_WEP_MODS; ++j) {
			Player_Orb_Data[pnum].weapon_stat_bonuses[i].wep_mods[j].val = 0;
			Player_Orb_Data[pnum].weapon_stat_bonuses[i].wep_mods[j].tier = 0;
		}
	}
}

bool IsOrbDropException(int orb_id) {
	switch(orb_id) {
		case DND_ORB_HOLLOW:
		case DND_ORB_PHANTASMAL:
		case DND_ORB_ASSIMILATION:
		return true;
	}
	return false;
}

void SpawnOrb(int pnum, bool sound, bool noRepeat = false) {
	int c = CreateItemSpot();
	if(c != -1) {
		int i;
		
#ifdef ISDEBUGBUILD
		i = random(0, MAX_ORBS - 1);
#else
	do {
		int w = random(1, ORB_MAXWEIGHT);
		if(GetCVar("dnd_ignore_dropweights"))
			i = random(0, MAX_ORBS - 1);
		else
			for(i = 0; i < MAX_ORBS && OrbDropWeights[i] < w; ++i);
	} while(IsOrbDropException(i));
#endif
		// c is the index on the field now
		// i = DND_ORB_CORRUPT;
		RollOrbInfo(c, i, true);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(InventoryInfo[i + ORBS_BEGIN], 24.0, 16, pnum + 1, c);
		if (sound)
			ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_ORB);
			
		// force noRepeat
		if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, STAT_LUCK) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
			SpawnOrb(pnum, sound, true);
	}
}

void SpawnOrbForAll(int repeats) {
	for(int k = 0; k < repeats; ++k) {
		for(int j = 0; j < MAXPLAYERS; ++j) {
			if(PlayerInGame(j) && !PlayerIsSpectator(j))
				SpawnOrb(j, false);
		}
	}
}

void SpawnSpecificOrb(int pnum, int id, bool sound, bool noRepeat = false) {
	int c = CreateItemSpot();
	if(c != -1) {
		RollOrbInfo(c, id, true);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(InventoryInfo[id + ORBS_BEGIN], 24.0, 16, pnum + 1, c);
		if(sound)
			ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_ORB);
			
		if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, STAT_LUCK) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
			SpawnSpecificOrb(pnum, id, sound, true);
	}
}

void SpawnSpecificOrbForAll(int id, int repeats) {
	for(int k = 0; k < repeats; ++k) {
		for(int j = 0; j < MAXPLAYERS; ++j) {
			if(PlayerInGame(j) && !PlayerIsSpectator(j))
				SpawnSpecificOrb(j, id, false);
		}
	}
}

void RollOrbInfo(int item_pos, int orbtype, bool onField) {
	// roll random attributes for the charm
	Inventories_On_Field[item_pos].item_level = 1;
	Inventories_On_Field[item_pos].item_stack = 1; // orbs have default stack of 1
	Inventories_On_Field[item_pos].item_type = DND_ITEM_ORB;
	Inventories_On_Field[item_pos].item_subtype = orbtype;
	Inventories_On_Field[item_pos].width = 1;
	Inventories_On_Field[item_pos].height = 1;
	Inventories_On_Field[item_pos].attrib_count = 0;
	Inventories_On_Field[item_pos].item_image = ITEM_IMAGE_ORB_BEGIN + orbtype;
}

Script "DnD Give Orb Delayed" (int type, int amt) {
	Delay(1);
	GiveOrbToPlayer(PlayerNumber(), type, amt);
	GiveInventory("DnD_RefreshPane", 1);
}

#endif
