#ifndef DND_ORBS_IN
#define DND_ORBS_IN

#include "DnD_Common.h"
#include "DnD_Stat.h"
#include "DnD_OrbDefs.h"
#include "DnD_Ammo.h"
#include "DnD_WeaponDefs.h"
#include "DnD_Sync.h"

#define MAX_ITER 200

#define ENHANCEORB_MAX 50 // 50% bonus
#define PROSPERITY_MAX 500 // 500 max
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
#define MAX_STORED_ORB_DATA 64
typedef struct {
	int orb_type;
	int values[MAX_STORED_ORB_DATA];
	// these are used for item data -- they are badly named, i'm aware...
	int p_ammos[MAX_SLOTS][MAX_AMMOTYPES_PER_SLOT];
	int p_tempammo;
	int p_tempwep;
	bool sins_cant_repent;
} orb_info_T;

// holds most recently used orb values
global orb_info_T 3: Player_MostRecent_Orb[MAXPLAYERS];

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
	975,
	985,
	1000,

	// drops only from specific monster
	0xFFFFFF,
	0xFFFFFF,
	0xFFFFFF
};

#define ENHANCEORB_BONUS 1

#define ORB_MAXWEIGHT 1000

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
#define CORRUPTORB_CRITGIVE 0.0875
#define CORRUPTORB_CRITDMGGIVE 25
#define CORRUPTORB_SPEEDGIVE 0.05
#define CORRUPTORB_DROPCHANCEGIVE 0.025
#define CORRUPTORB_STATGIVE 5

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
/*int OrbRichesData[MAX_RICHES][2] = {
	{ 5, 2 },
	{ 5, 9 },
	{ 5, 10 }
};*/

int GetOrbRichesData(int data, int id) {
	if(!id)
		return 5;
	
	switch(data) {
		case RICHES_EXP:
		return 2;
		case RICHES_CREDIT:
		return 9;
		case RICHES_BUDGET:
		return 10;
	}
	
	return 0;
}

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
			if(extratype == DND_ITEM_WEAPON && GetPlayerWeaponQuality(pnum, extra) != ENHANCEORB_MAX)
				res = true;
		break;
		case DND_ORB_CORRUPT:
			res = true; // can always use this, the orb will find a way to fuck you up in some way
		break;
		case DND_ORB_REPENT:
			temp = Player_MostRecent_Orb[pnum].orb_type;
			if(temp) {
				--temp;
				if(temp < DND_ORB_REFINEMENT || temp == DND_ORB_PHANTASMAL)
					res = Player_MostRecent_Orb[pnum].orb_type - 1 != DND_ORB_REPENT;
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
		case DND_ORB_SIN:
			if(IsUsableOnInventory(extratype)) {
				// if there's a fractured mod or it's a unique, don't let
				temp = PlayerInventoryList[pnum][extra].item_type;
				if(temp == DND_ITEM_CHARM)
					res = PlayerInventoryList[pnum][extra].item_type < UNIQUE_BEGIN && PlayerInventoryList[pnum][extra].attrib_count >= Max(2, Charm_MaxAffixes[PlayerInventoryList[pnum][extra].item_subtype] / 2);
				
				for(i = 0; i < PlayerInventoryList[pnum][extra].attrib_count; ++i) {
					if(PlayerInventoryList[pnum][extra].attributes[i].fractured) {
						res = false;
						break;
					}
				}
			}
		break;
		// reforge ones only need to be checked for "can use on inventory type" condition
		case DND_ORB_PRISMATIC:
		case DND_ORB_DESTRUCTION:
		case DND_ORB_VIOLENCE:
		case DND_ORB_FORTITUDE:
		case DND_ORB_PROSPERITY:
		case DND_ORB_TINKERER:
		case DND_ORB_TURMOIL:
		case DND_ORB_TREMORS:
		case DND_ORB_HEXES:
		case DND_ORB_GROWTH:
		case DND_ORB_CRACKLING:
		case DND_ORB_BRUTE:
		case DND_ORB_JAGGED:
			if(IsUsableOnInventory(extratype)) {
				// don't let this be used on a unique
				res = PlayerInventoryList[pnum][extra].item_type < UNIQUE_BEGIN;
			}
		break;
		case DND_ORB_REFINEMENT:
			if(IsUsableOnInventory(extratype))
				res = true;
		break;
		case DND_ORB_SCULPTING:
		case DND_ORB_NULLIFICATION:
			if(IsUsableOnInventory(extratype)) {
				// don't let this be used on a unique
				if(PlayerInventoryList[pnum][extra].item_type > UNIQUE_BEGIN)
					res = false;
				else
					res = PlayerInventoryList[pnum][extra].attrib_count - GetItemFracturedModCount(pnum, extra) > 0;
			}
		break;
		case DND_ORB_ELEVATION:
			res = CanAddModToItem(extratype, extra, 0);
		break;
		case DND_ORB_POTENCY:
			temp = PlayerInventoryList[pnum][extra].attrib_count;
			res = false;
			for(i = 0; i < temp; ++i) {
				if(PlayerInventoryList[pnum][extra].attributes[i].fractured)
					continue;

				if(PlayerInventoryList[pnum][extra].attributes[i].attrib_tier < MAX_CHARM_AFFIXTIERS)
					res = true;
			}
		break;

		case DND_ORB_HOLLOW:
			// this one is basically an orb of elevation with a +1 additional attribute allowed
			res = CanAddModToItem(extratype, extra, 1);
		break;
		case DND_ORB_PHANTASMAL:
			// if the weapon can't hit ghosts on its own or we didnt give it the ghost hit already
			if(extratype == DND_ITEM_WEAPON && !HasWeaponPower(pnum, extra, WEP_POWER_GHOSTHIT) && (Weapons_Data[extra].properties & WPROP_CANTHITGHOST))
				res = true;
		break;
		case DND_ORB_ASSIMILATION:
			// extra is itemid1, extratype is itemid2
			// we must have matching item types, so charm x charm, Y x Y etc.
			// and we have at least an attribute on both to be taking
			res = 	PlayerInventoryList[pnum][extra].item_type == PlayerInventoryList[pnum][extratype].item_type && 
					(PlayerInventoryList[pnum][extra].attrib_count && PlayerInventoryList[pnum][extratype].attrib_count);
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
	SaveUsedItemAttribs(pnum, item_index);
	
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

void SaveUsedItemAttribs(int pnum, int item_id) {
	Player_MostRecent_Orb[pnum].p_tempwep = item_id + 1;
	Player_MostRecent_Orb[pnum].values[0] = PlayerInventoryList[pnum][item_id].attrib_count;
	for(int i = 0; i < Player_MostRecent_Orb[pnum].values[0]; ++i) {
		Player_MostRecent_Orb[pnum].values[4 * i + 1] = PlayerInventoryList[pnum][item_id].attributes[i].attrib_id;
		Player_MostRecent_Orb[pnum].values[4 * i + 2] = PlayerInventoryList[pnum][item_id].attributes[i].attrib_val;
		Player_MostRecent_Orb[pnum].values[4 * i + 3] = PlayerInventoryList[pnum][item_id].attributes[i].attrib_tier;
		Player_MostRecent_Orb[pnum].values[4 * i + 4] = PlayerInventoryList[pnum][item_id].attributes[i].fractured;
	}
}

void RestoreItemAttribsFromUsedOrb(int pnum) {
	int temp = Player_MostRecent_Orb[pnum].p_tempwep - 1;
	PlayerInventoryList[pnum][temp].attrib_count = Player_MostRecent_Orb[pnum].values[0];
	
	for(int i = 0; i < Player_MostRecent_Orb[pnum].values[0]; ++i) {
		PlayerInventoryList[pnum][temp].attributes[i].attrib_id = Player_MostRecent_Orb[pnum].values[4 * i + 1];
		PlayerInventoryList[pnum][temp].attributes[i].attrib_val = Player_MostRecent_Orb[pnum].values[4 * i + 2];
		PlayerInventoryList[pnum][temp].attributes[i].attrib_tier = Player_MostRecent_Orb[pnum].values[4 * i + 3];
		PlayerInventoryList[pnum][temp].attributes[i].fractured = Player_MostRecent_Orb[pnum].values[4 * i + 4];
	}
	SyncItemAttributes(pnum,temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

void HandleOrbUse (int pnum, int orbtype, int extra, int extra2 = -1) {
	int res = -1;
	int temp;
	int prev;
	int i, s;
	int affluence = GetAffluenceBonus();
	SetInventory("OrbUseType", orbtype + 1);
	// for any other orb, reset most recently used orb
	if(orbtype != DND_ORB_REPENT)
		ResetMostRecentOrb(pnum);
	switch(orbtype) {
		case DND_ORB_ENHANCE:
			res = extra;
			prev = GetPlayerWeaponQuality(pnum, res);
			temp = Clamp_Between(prev + affluence * ENHANCEORB_BONUS, 0, ENHANCEORB_MAX);
			SetPlayerWeaponQuality(pnum, res, temp);
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
			Player_MostRecent_Orb[pnum].values[1] = temp;
			SyncClientsideVariable_WeaponProperties(pnum, res);
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
			Player_MostRecent_Orb[pnum].values[0] |= affluence << 8;
			HandleCorruptOrbUse(i);
		break;
		case DND_ORB_PRISMATIC:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeWithOneTagGuaranteed(pnum, extra, INV_ATTR_TAG_ELEMENTAL_ID);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
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
			i = TakeOrbFromPlayer(temp, affluence);
			res = temp;
			do {
				temp = PickRandomOrb();
			} while(temp == DND_ORB_CALAMITY);
			ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, temp, i);
			// store type of orb x100 away
			res += temp * 100;
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
			Player_MostRecent_Orb[pnum].values[1] = i;
		break;
		case DND_ORB_PROSPERITY:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeWithOneTagGuaranteed(pnum, extra, INV_ATTR_TAG_LIFE_ID);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_FORTITUDE:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeWithOneTagGuaranteed(pnum, extra, INV_ATTR_TAG_DEFENSE_ID);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_NULLIFICATION:
			// save
			SaveUsedItemAttribs(pnum, extra);
			
			for(s = 0; s < affluence && PlayerInventoryList[pnum][extra].attrib_count; ++s) {
				// find the attribute with the lowest tier, in case of multiple, return a random one
				res = 0;
				temp = MAX_CHARM_AFFIXTIERS;
				for(i = 0; i < PlayerInventoryList[pnum][extra].attrib_count; ++i) {
					// ignore the fractured mods
					if(PlayerInventoryList[pnum][extra].attributes[i].fractured)
						continue;

					if(PlayerInventoryList[pnum][extra].attributes[i].attrib_tier < temp) {
						// we use res to hold the count of elements in this temporary array
						// reset current count if we found a new minimum, then add it to our array
						res = 0;
						TempArray[res++] = i;
						temp = PlayerInventoryList[pnum][extra].attributes[i].attrib_tier;
					}
					else if(PlayerInventoryList[pnum][extra].attributes[i].attrib_tier == temp) // if equal to current min, store it
						TempArray[res++] = i;
				}

				// res holds count now, so we must have non-zero or do nothing
				if(res) {
					res = random(0, res - 1);

					// remove the randomized minimum found
					RemoveAttributeFromItem(pnum, extra, TempArray[res]);
				}
			}

			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_DESTRUCTION:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeWithOneTagGuaranteed(pnum, extra, INV_ATTR_TAG_CRIT_ID);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_VIOLENCE:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeWithOneTagGuaranteed(pnum, extra, INV_ATTR_TAG_ATTACK_ID);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_SIN:
			// save
			SaveUsedItemAttribs(pnum, extra);

			// pick random attribute to fracture
			s = random(0, PlayerInventoryList[pnum][extra].attrib_count - 1);
			PlayerInventoryList[pnum][extra].attributes[s].fractured = true;
		
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_TREMORS:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeWithOneTagGuaranteed(pnum, extra, INV_ATTR_TAG_EXPLOSIVE_ID);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_TINKERER:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeWithOneTagGuaranteed(pnum, extra, INV_ATTR_TAG_UTILITY_ID);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_REFINEMENT:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
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
						// ignore fractured mods
						if(PlayerInventoryList[pnum][extra].attributes[i].fractured)
							continue;

						temp = PlayerInventoryList[pnum][extra].attributes[i].attrib_id;
						PlayerInventoryList[pnum][extra].attributes[i].attrib_val = RollAttributeValue(
							temp, 
							PlayerInventoryList[pnum][extra].attributes[i].attrib_tier, 
							CheckWellRolled(pnum),
							PlayerInventoryList[pnum][extra].item_type,
							PlayerInventoryList[pnum][extra].item_subtype
						);
					}
				}
			}
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_SCULPTING:
			// save
			SaveUsedItemAttribs(pnum, extra);
			
			temp = 0;
			for(s = 0; s < affluence && PlayerInventoryList[pnum][extra].attrib_count; ++s) {
				// reroll if fractured
				do {
					res = random(0, PlayerInventoryList[pnum][extra].attrib_count - 1);
				} while(temp < DND_MAX_ORB_REROLL_ATTEMPTS && PlayerInventoryList[pnum][extra].attributes[res].fractured);
				RemoveAttributeFromItem(pnum, extra, res);
			}

			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_ELEVATION:
			HandleAddRandomMod(pnum, extra, 0, false);
		break;
		case DND_ORB_TURMOIL:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeItem(pnum, extra);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_HEXES:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeWithOneTagGuaranteed(pnum, extra, INV_ATTR_TAG_OCCULT_ID);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_GROWTH:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeWithOneTagGuaranteed(pnum, extra, INV_ATTR_TAG_STAT_ID);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_POTENCY:
			// save
			SaveUsedItemAttribs(pnum, extra);

			// failsafe, if it tried it 100 times there's a really good chance the item now has perfect tiers... don't bother
			s = 0;
			for(i = 0; i < affluence; ++i) {
				do {
					temp = random(0, PlayerInventoryList[pnum][extra].attrib_count - 1);
					++s;
				} while(s < DND_MAX_ORB_REROLL_ATTEMPTS && (PlayerInventoryList[pnum][extra].attributes[temp].attrib_tier == MAX_CHARM_AFFIXTIERS || PlayerInventoryList[pnum][extra].attributes[temp].fractured));
				
				// increment the tier and reroll that attribute!
				if(!PlayerInventoryList[pnum][extra].attributes[temp].fractured && PlayerInventoryList[pnum][extra].attributes[temp].attrib_tier < MAX_CHARM_AFFIXTIERS) {
					++PlayerInventoryList[pnum][extra].attributes[temp].attrib_tier;
					PlayerInventoryList[pnum][extra].attributes[temp].attrib_val = RollAttributeValue(
							PlayerInventoryList[pnum][extra].attributes[temp].attrib_id, 
							PlayerInventoryList[pnum][extra].attributes[temp].attrib_tier,
							false, 
							PlayerInventoryList[pnum][extra].item_type, 
							PlayerInventoryList[pnum][extra].item_subtype
						);
				}
			}

			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_CRACKLING:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeWithOneTagGuaranteed(pnum, extra, INV_ATTR_TAG_ENERGY_ID);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_BRUTE:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeWithOneTagGuaranteed(pnum, extra, INV_ATTR_TAG_MELEE_ID);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_JAGGED:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeWithOneTagGuaranteed(pnum, extra, INV_ATTR_TAG_PHYSICAL_ID);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;

		case DND_ORB_HOLLOW:
			HandleAddRandomMod(pnum, extra, 1, true);
		break;
		case DND_ORB_PHANTASMAL:
			res = extra;

			SetWeaponModPowerset(pnum, extra, WEP_POWER_GHOSTHIT, true);

			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
			Player_MostRecent_Orb[pnum].values[1] = WEP_POWER_GHOSTHIT;
			SyncClientsideVariable_WeaponMods(pnum, extra);
		break;
		case DND_ORB_ASSIMILATION:
			// extra and extra2 are the item indexes in use, with extra being the one assimilated into extra2
			// we will store the attributes in a temporary array, then copy them to extra2
			int picked_mod;
			bool fail_pick;
			
			// init the temp array to -1s
			for(i = 0; i < MAX_ITEM_ATTRIBUTES * ATTRIB_DATA_COUNT; ++i)
				TempArray[i] = -1;
			
			// we determine when to stop, we either stop until max affixes or we have minimum of both attrib counts total
			// make the latter count unique attributes on both combined, not just total attribute count of both
			// for that we assume base count on item 1, then we check item 2 for non-occuring attributes
			temp = PlayerInventoryList[pnum][extra].attrib_count;
			for(i = 0; i < PlayerInventoryList[pnum][extra2].attrib_count; ++i) {
				fail_pick = false;
				for(s = 0; s < PlayerInventoryList[pnum][extra].attrib_count; ++s) {
					if(PlayerInventoryList[pnum][extra2].attributes[i].attrib_id == PlayerInventoryList[pnum][extra].attributes[s].attrib_id) {
						fail_pick = true;
						break;
					}
				}
				temp += !fail_pick;
			}
			
			// will pick anywhere from half of max affix count of a charm to max affix count + 1
			// if we don't have at least half of affix count of item on total sum of mods, we'll pick between 1 and the sum instead
			if(temp < Charm_MaxAffixes[PlayerInventoryList[pnum][extra2].item_subtype] / 2)
				s = random(1, temp);
			else
				s = random(Charm_MaxAffixes[PlayerInventoryList[pnum][extra2].item_subtype] / 2, Min(Charm_MaxAffixes[PlayerInventoryList[pnum][extra2].item_subtype] + 1, temp));
			
			/*printbold(
				s:"start picking ", d:s, s: " attribs with ", d:temp, s: " unique attributes (random from ",
				d:Charm_MaxAffixes[PlayerInventoryList[pnum][extra2].item_subtype] / 2, s: " ", 
				d:Min(Charm_MaxAffixes[PlayerInventoryList[pnum][extra2].item_subtype] + 1, temp)
			);*/
			
			temp = 0;
			
			do {
				do {
					fail_pick = false;
					// choose which item has their attribute to be taken
					// pick a random attribute from it
					// confirm this is a real attribute and the charm isn't devoid of any attributes
					res = PickWeightedFromTwoItems(pnum, extra, extra2);
					prev = random(0, PlayerInventoryList[pnum][res].attrib_count - 1);
					//log(s:"picked ", d: res, s:" from: ", d:extra, s: " - ", d:extra2, s: "\nmod: ", d:picked_mod);
					
					// set this to be the actual attribute now instead of just an index in the item itself
					picked_mod = PlayerInventoryList[pnum][res].attributes[prev].attrib_id;
					
					// if its not in our array add it to it
					for(i = 0; i < MAX_ITEM_ATTRIBUTES && TempArray[i * ATTRIB_DATA_COUNT] != -1; ++i) {
						if(picked_mod == TempArray[i * ATTRIB_DATA_COUNT]) {
							fail_pick = true;
							break;
						}
					}
				} while(fail_pick);

				// confirmed addable attribute
				TempArray[temp * ATTRIB_DATA_COUNT] = PlayerInventoryList[pnum][res].attributes[prev].attrib_id;
				TempArray[temp * ATTRIB_DATA_COUNT + 1] = PlayerInventoryList[pnum][res].attributes[prev].attrib_val;
				TempArray[temp * ATTRIB_DATA_COUNT + 2] = PlayerInventoryList[pnum][res].attributes[prev].attrib_tier;
				TempArray[temp * ATTRIB_DATA_COUNT + 3] = PlayerInventoryList[pnum][res].attributes[prev].fractured;

				++temp;
			} while(temp < s);

			// we are going to assimilate the first one into second, so the first one is destroyed
			// and make sure the extra2 item has no attributes left anymore
			FreeItem(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
			ScourItem(pnum, extra2);
			
			// copy the attributes into extra2
			for(i = 0; i < temp && TempArray[i * ATTRIB_DATA_COUNT] != -1; ++i) {
				InsertAttributeToItem(
					pnum, 
					extra2, 
					TempArray[i * ATTRIB_DATA_COUNT], 
					TempArray[i * ATTRIB_DATA_COUNT + 1],
					TempArray[i * ATTRIB_DATA_COUNT + 2],
					TempArray[i * ATTRIB_DATA_COUNT + 3]
				);
			}
			
			// finally, sync it
			SyncItemData(pnum, extra2, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[pnum][extra2].width, PlayerInventoryList[pnum][extra2].height);
			GiveInventory("DnD_CleanCraftingRequest", 1);
			GiveInventory("DnD_RefreshPane", 1);
			SetInventory("OrbResult", 1);
		break;
	}
	Player_MostRecent_Orb[pnum].orb_type = orbtype + 1; // +1 because 0 is used as no orb
	
	// clientside msg
	ACS_NamedExecuteAlways("DND Orb Use Message", 0, CheckInventory("OrbUseType") - 1, CheckInventory("OrbResult"), affluence);
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

void RevertLastOrbEffect() {
	int pnum = PlayerNumber(), i, temp;
	switch(Player_MostRecent_Orb[pnum].orb_type - 1) {
		case DND_ORB_ENHANCE:
			// val 1 is wepid, val2 is overall gain
			temp = Player_MostRecent_Orb[pnum].values[0];
			SetPlayerWeaponQuality(pnum, temp, GetPlayerWeaponQuality(pnum, temp) - Player_MostRecent_Orb[pnum].values[1]);
			SyncClientsideVariable_WeaponProperties(pnum, temp);
		break;
		case DND_ORB_PRISMATIC:
		case DND_ORB_PROSPERITY:
		case DND_ORB_FORTITUDE:
		case DND_ORB_TINKERER:
		case DND_ORB_VIOLENCE:
		case DND_ORB_SIN:
		case DND_ORB_DESTRUCTION:
		case DND_ORB_REFINEMENT:
		case DND_ORB_SCULPTING:
		case DND_ORB_TURMOIL:
		case DND_ORB_NULLIFICATION:
		case DND_ORB_TREMORS:
		case DND_ORB_HEXES:
		case DND_ORB_POTENCY:
		case DND_ORB_CRACKLING:
		case DND_ORB_BRUTE:
		case DND_ORB_JAGGED:
			RestoreItemAttribsFromUsedOrb(pnum);
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
			if(i)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, Player_MostRecent_Orb[pnum].values[0] % 100, i);
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
			SetWeaponModPowerset(pnum, temp, WEP_POWER_GHOSTHIT, false);
			SyncClientsideVariable_WeaponMods(pnum, temp);
		break;
		
	}
	SetInventory("OrbResult", Player_MostRecent_Orb[pnum].orb_type - 1);
	ResetMostRecentOrb(pnum);
}

// picks a random orb
int PickRandomOrb() {
	int i;
	do {
		i = random(DND_ORB_ENHANCE, MAX_ORBS - 1);
	} while(IsOrbDropException(i));
	return i;
}

int GetAffluenceBonus() {
	return 1 << CheckInventory("AffluenceCounter");
}

bool HasWeaponWithoutCritDmgMax() {
	int pnum = PlayerNumber();
	/*for(int i = 0; i < MAXWEPS; ++i) {
		if(GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, i) < CORRUPTORB_MAXCRITDMG)
			return 1;
	}*/
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
/*	switch(effect) {
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
	}*/
	return 1;
}

void HandleCorruptOrbUse(int type) {
/*	int i;
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
	}*/
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
			Log(s:"\cj", l:"DND_ORBUSETEXT2H", s:" \cv", s:GetFixedRepresentation(CORRUPTORB_CRITGIVE * affluence, true), s:"%\cj ", l:"DND_ORBUSETEXT2I", s:" \cv", l:GetWeaponTag(val >> 8), s:"\cj!");
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
/*	int i;
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
	}*/
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
		case DND_ORB_PRISMATIC:
			Log(s:"\cj", l:"DND_ORBUSETEXT3", s:"\cj!");
		break;
		case DND_ORB_REPENT:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT4", s:" \cv", l:GetInventoryTag(val + ORBS_BEGIN), s:"\cj!");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL4");
		break;
		case DND_ORB_AFFLUENCE:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT5", s:" \cdx", d:affluence << 1, s: "\c-!");
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
			Log(s:"\cj", l:"DND_ORBUSETEXT7", s:"\cj!");
		break;
		case DND_ORB_FORTITUDE:
			Log(s:"\cj", l:"DND_ORBUSETEXT8", s:"\cj!");
		break;
		case DND_ORB_NULLIFICATION:
			if(val != 0x7FFFFFFF) {
				if(affluence > 1)
					Log(s:"\cj", l:"DND_ORBUSETEXT9A", s:" \cd", d:affluence, s:"\c- ", l:"DND_ORB_ATTRIBUTES", s:"!");
				else
					Log(s:"\cj", l:"DND_ORBUSETEXT16B");
			}
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL9");
		break;
		case DND_ORB_DESTRUCTION:
			Log(s:"\cj", l:"DND_ORBUSETEXT10", s:"\cj!");
		break;
		case DND_ORB_VIOLENCE:
			Log(s:"\cj", l:"DND_ORBUSETEXT11A", s:"\cj!");
		break;
		case DND_ORB_SIN:
			if(val != 0x7FFFFFFF)
				Log(s:"\cg", l:"DND_ORBUSETEXT12", s:"\c-!");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL12", s:"\c-!");
		break;
		case DND_ORB_TREMORS:
			Log(s:"\cj", l:"DND_ORBUSETEXT13", s:"\cj!");
		break;
		case DND_ORB_TINKERER:
			Log(s:"\cj", l:"DND_ORBUSETEXT14", s:"\cj!");
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
		case DND_ORB_TURMOIL:
			Log(s:"\cj", l:"DND_ORBUSETEXT21", s:"\cj!");
		break;
		case DND_ORB_HEXES:
			Log(s:"\cj", l:"DND_ORBUSETEXT22", s:"\cj!");
		break;
		case DND_ORB_GROWTH:
			Log(s:"\cj", l:"DND_ORBUSETEXT23", s:"\cj!");
		break;
		case DND_ORB_POTENCY:
			if(val != 0x7FFFFFFF) {
				if(affluence > 1)
					Log(s:"\cj", l:"DND_ORBUSETEXT24A", s:" \cd ", d:affluence, s:"\c- ", l:"DND_ORBUSETEXT24B", s:"!");
				else
				Log(s:"\cj", l:"DND_ORBUSETEXT24A", s:" \cd ", d:affluence, s:"\c- ", l:"DND_ORBUSETEXT24C", s:"!");
			}
			else
				Log(s:"\cj", l:"DND_ORBUSEFAIL24");
		break;
		case DND_ORB_CRACKLING:
			Log(s:"\cj", l:"DND_ORBUSETEXT25", s:"\cj!");
		break;
		case DND_ORB_BRUTE:
			Log(s:"\cj", l:"DND_ORBUSETEXT26", s:"\cj!");
		break;
		case DND_ORB_JAGGED:
			Log(s:"\cj", l:"DND_ORBUSETEXT27", s:"\cj!");
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

bool IsOrbDropException(int orb_id) {
	switch(orb_id) {
		case DND_ORB_HOLLOW:
		case DND_ORB_PHANTASMAL:
		case DND_ORB_ASSIMILATION:
		return true;
	}
	return false;
}

void SpawnOrb(int pnum, bool sound, bool noRepeat = false, int stack = 1) {
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
		RollOrbInfo(c, i, true, stack);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(InventoryInfo[i + ORBS_BEGIN], 24.0, 16, pnum + 1, c);
		if (sound)
			ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_ORB);
			
		// force noRepeat
		if(!noRepeat && CheckPlayerLuckDuplicator(pnum))
			SpawnOrb(pnum, sound, true);
	}
}

void SpawnOrbForAll(int repeats, int stack = 1) {
	for(int k = 0; k < repeats; ++k) {
		for(int j = 0; j < MAXPLAYERS; ++j) {
			if(PlayerInGame(j) && !PlayerIsSpectator(j))
				SpawnOrb(j, false, false, stack);
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

void RollOrbInfo(int item_pos, int orbtype, bool onField, int stack = 1) {
	// roll random attributes for the charm
	Inventories_On_Field[item_pos].item_level = 1;
	Inventories_On_Field[item_pos].item_stack = stack; // orbs have default stack of 1
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
