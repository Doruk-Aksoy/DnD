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
#define AFFLUENCE_MAX 4 // x16
#define AFFLUENCE_MULT 2

#define DND_ORB_SIN_REPENTCHANCE 0.33

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

int OrbDropWeights[MAX_ORBS] = {
	60, // 6%
	84, // 2.4%
	129, // 4.5%
	147, // 1.8%
	172, // 2.5%

	202, // 3%
	247, // 4.5%
	272, // 2.5%
	302, // 3%
	347, // 4.5%

	392, // 4.5%
	410, // 1.8%
	455, // 4.5%
	490, // 3.5%
	540, // 5%

	580, // 4%
	615, // 3.5%
	675, // 6%
	720, // 4.5%
	756, // 3.6%

	781, // 2.5%
	826, // 4.5%
	871, // 4.5%
	916, // 4.5%
	940, // 2.4%
	
	965, // 2.5%
	1000, // 3.5%

	// drops only from specific monster
	0xFFFFFF,
	0xFFFFFF,
	0xFFFFFF
};

#define ENHANCEORB_BONUS 1

#define ORB_MAXWEIGHT 1000
#define DND_CORRUPT_FAIL 419
#define DND_CORRUPT_SUCCESS 420

bool CanAddModToItem(int pnum, int itemtype, int item_index, int add_lim) {
	bool res = false;
	if(IsUsableOnInventory(itemtype)) {
		// this one depends on attribute counts of items it is used on
		if(PlayerInventoryList[pnum][item_index].item_type > UNIQUE_BEGIN)
			res = false;
		else {
			res = PlayerInventoryList[pnum][item_index].attrib_count < GetMaxItemAffixes(PlayerInventoryList[pnum][item_index].item_type, PlayerInventoryList[pnum][item_index].item_subtype) + add_lim;
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
			res = extratype == DND_ITEM_WEAPON && GetPlayerWeaponQuality(pnum, extra) != ENHANCEORB_MAX;
		break;
		case DND_ORB_CORRUPT:
			res = IsUsableOnInventory(extratype) && !IsInventoryCorrupted(pnum, extra);
		break;
		case DND_ORB_REPENT:
			temp = Player_MostRecent_Orb[pnum].orb_type;
			if(temp) {
				--temp;
				if(temp == DND_ORB_CORRUPT || temp == DND_ORB_REPENT)
					res = false;
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
			if(IsUsableOnInventory(extratype) && !IsInventoryCorrupted(pnum, extra)) {
				// if there's a fractured mod or it's a unique, don't let
				temp = PlayerInventoryList[pnum][extra].item_type;

				// unique case
				res = temp < UNIQUE_BEGIN;

				// mod count > half case
				res = res && PlayerInventoryList[pnum][extra].attrib_count >= Max(2, 1 + GetMaxItemAffixes(temp, PlayerInventoryList[pnum][extra].item_subtype) / 2);

				// fracture case
				for(i = 0; res && i < PlayerInventoryList[pnum][extra].attrib_count; ++i) {
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
		case DND_ORB_SAVAGERY:
			if(IsUsableOnInventory(extratype) && !IsInventoryCorrupted(pnum, extra)) {
				// don't let this be used on a unique
				res = PlayerInventoryList[pnum][extra].item_type < UNIQUE_BEGIN;
			}
		break;
		case DND_ORB_REFINEMENT:
			if(IsUsableOnInventory(extratype) && !IsInventoryCorrupted(pnum, extra))
				res = true;
		break;
		case DND_ORB_SCULPTING:
		case DND_ORB_NULLIFICATION:
			if(IsUsableOnInventory(extratype) && !IsInventoryCorrupted(pnum, extra)) {
				// don't let this be used on a unique
				if(PlayerInventoryList[pnum][extra].item_type > UNIQUE_BEGIN)
					res = false;
				else
					res = PlayerInventoryList[pnum][extra].attrib_count - GetItemFracturedModCount(pnum, extra) > 0;
			}
		break;
		case DND_ORB_ELEVATION:
			res =  !IsInventoryCorrupted(pnum, extra) && CanAddModToItem(pnum, extratype, extra, 0);
		break;
		case DND_ORB_POTENCY:
			if(!IsInventoryCorrupted(pnum, extra) && PlayerInventoryList[pnum][extra].item_type < UNIQUE_BEGIN) {
				temp = PlayerInventoryList[pnum][extra].attrib_count;
				res = false;
				for(i = 0; i < temp; ++i) {
					if(PlayerInventoryList[pnum][extra].attributes[i].fractured)
						continue;

					if(PlayerInventoryList[pnum][extra].attributes[i].attrib_tier < MAX_CHARM_AFFIXTIERS)
						res = true;
				}
			}
		break;
		case DND_ORB_ALCHEMIST:
			res = (extratype & 0xFFFF) == DND_ITEM_CHARM && PlayerInventoryList[pnum][extra].quality < GetItemMaxQuality(pnum, extra);

			// check if item has cybernetic -- it shouldn't have it!
			res &= !ItemIsCybernetic(pnum, extra, PlayerInventoryList[pnum][extra].attrib_count, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			res &= !IsInventoryCorrupted(pnum, extra);
		break;
		case DND_ORB_EVOKER:
			// won't work on uniques
			res = PlayerInventoryList[pnum][extra].attrib_count > 0 && PlayerInventoryList[pnum][extra].item_type < UNIQUE_BEGIN;
		break;

		case DND_ORB_HOLLOW:
			// this one is basically an orb of elevation with a +1 additional attribute allowed
			res =  !IsInventoryCorrupted(pnum, extra) && CanAddModToItem(pnum, extratype, extra, 1);
		break;
		case DND_ORB_PHANTASMAL:
			// if the weapon can't hit ghosts on its own or we didnt give it the ghost hit already
#ifndef ISDEBUGBUILD
			if(extratype == DND_ITEM_WEAPON && !HasWeaponPower(pnum, extra, WEP_POWER_GHOSTHIT) && (Weapons_Data[extra].properties & WPROP_CANTHITGHOST))
				res = true;
#else
				res = true;
#endif
		break;
		case DND_ORB_ASSIMILATION:
			// extra is itemid1, extratype is itemid2
			// we must have matching item types, so charm x charm, Y x Y etc.
			// and we have at least an attribute on both to be taking
			res = 	PlayerInventoryList[pnum][extra].item_type == PlayerInventoryList[pnum][extratype].item_type && 
					PlayerInventoryList[pnum][extra].item_type < UNIQUE_BEGIN && PlayerInventoryList[pnum][extratype].item_type < UNIQUE_BEGIN &&
					(PlayerInventoryList[pnum][extra].item_type != DND_ITEM_CHARM || (PlayerInventoryList[pnum][extra].item_subtype == PlayerInventoryList[pnum][extratype].item_subtype)) &&
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
	int max_possible = GetMaxItemAffixes(PlayerInventoryList[pnum][item_index].item_type, PlayerInventoryList[pnum][item_index].item_subtype);

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
		finish = PlayerInventoryList[pnum][item_index].attrib_count >= max_possible + add_lim;
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
	Player_MostRecent_Orb[pnum].values[1] = PlayerInventoryList[pnum][item_id].item_level;

	for(int i = 0; i < Player_MostRecent_Orb[pnum].values[0]; ++i) {
		Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 2] = PlayerInventoryList[pnum][item_id].attributes[i].attrib_id;
		Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 3] = PlayerInventoryList[pnum][item_id].attributes[i].attrib_val;
		Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 4] = PlayerInventoryList[pnum][item_id].attributes[i].attrib_tier;
		Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 5] = PlayerInventoryList[pnum][item_id].attributes[i].attrib_extra;
		Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 6] = PlayerInventoryList[pnum][item_id].attributes[i].fractured;
	}
}

void SaveUsedItemQuality(int pnum, int item_id) {
	Player_MostRecent_Orb[pnum].p_tempwep = item_id + 1;
	Player_MostRecent_Orb[pnum].values[0] = PlayerInventoryList[pnum][item_id].quality;
}

void SaveUsedItemImplicit(int pnum, int item_id) {
	Player_MostRecent_Orb[pnum].p_tempwep = item_id + 1;
	Player_MostRecent_Orb[pnum].values[0] = PlayerInventoryList[pnum][item_id].implicit.attrib_id;
	Player_MostRecent_Orb[pnum].values[1] = PlayerInventoryList[pnum][item_id].implicit.attrib_val;
	Player_MostRecent_Orb[pnum].values[2] = PlayerInventoryList[pnum][item_id].implicit.attrib_tier;
	Player_MostRecent_Orb[pnum].values[3] = PlayerInventoryList[pnum][item_id].implicit.attrib_extra;
}

void RestoreItemAttribsFromUsedOrb(int pnum) {
	int temp = Player_MostRecent_Orb[pnum].p_tempwep - 1;
	PlayerInventoryList[pnum][temp].attrib_count = Player_MostRecent_Orb[pnum].values[0];
	PlayerInventoryList[pnum][temp].item_level = Player_MostRecent_Orb[pnum].values[1];
	
	for(int i = 0; i < Player_MostRecent_Orb[pnum].values[0]; ++i) {
		PlayerInventoryList[pnum][temp].attributes[i].attrib_id = Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 2];
		PlayerInventoryList[pnum][temp].attributes[i].attrib_val = Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 3];
		PlayerInventoryList[pnum][temp].attributes[i].attrib_tier = Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 4];
		PlayerInventoryList[pnum][temp].attributes[i].attrib_extra = Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 5];
		PlayerInventoryList[pnum][temp].attributes[i].fractured = Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 6];
	}
	SyncItemAttributes(pnum, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

void RestoreItemImplicitsFromUsedOrb(int pnum) {
	int temp = Player_MostRecent_Orb[pnum].p_tempwep - 1;

	PlayerInventoryList[pnum][temp].implicit.attrib_id = Player_MostRecent_Orb[pnum].values[0];
	PlayerInventoryList[pnum][temp].implicit.attrib_val = Player_MostRecent_Orb[pnum].values[1];
	PlayerInventoryList[pnum][temp].implicit.attrib_tier = Player_MostRecent_Orb[pnum].values[2];
	PlayerInventoryList[pnum][temp].implicit.attrib_extra = Player_MostRecent_Orb[pnum].values[3];

	SyncItemImplicits(pnum, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

void RestoreItemQualityFromUsedOrb(int pnum) {
	int temp = Player_MostRecent_Orb[pnum].p_tempwep - 1;
	PlayerInventoryList[pnum][temp].quality = Player_MostRecent_Orb[pnum].values[0];
	SyncItemQuality(pnum, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

void HandleOrbUse (int pnum, int orbtype, int extra, int extra2 = -1) {
	int res = -1;
	int temp;
	int prev;
	int i, s, x;
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
			/* 
				corrupt orb can:
				1. Do Nothing (50% chance)
				2. Reforge item completely (25% chance)
				3. Add or replace existing implicit with a corruption implicit (25% chance)
			*/
			PlayerInventoryList[pnum][extra].corrupted = true;

#ifdef ISDEBUGBUILD
			temp = 1000;
#else
			temp = random(1, 1000);
#endif
			if(temp <= 500) {
				// does nothing but corrupt the item
				SetInventory("OrbResult", DND_CORRUPT_FAIL);
				SyncItemImplicits(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			}
			else if(temp <= 750) {
				// if it was a unique item, destroy it
				if(PlayerInventoryList[pnum][extra].item_type > UNIQUE_BEGIN) {
					FreeItem(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
					GiveInventory("DnD_CleanCraftingRequest", 1);
					GiveInventory("DnD_RefreshPane", 1);
					GiveInventory("DnD_CursorDataClearRequest", 1);
				}
				else
					ReforgeItem(pnum, extra);

				SetInventory("OrbResult", extra);
				SyncItemImplicits(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
				SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			}
			else {
				GiveCorruptionEffect(pnum, extra);
				SetInventory("OrbResult", DND_CORRUPT_SUCCESS);
				SyncItemImplicits(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			}
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
						TempArray[TARR_ORB1][res++] = i;
						temp = PlayerInventoryList[pnum][extra].attributes[i].attrib_tier;
					}
					else if(PlayerInventoryList[pnum][extra].attributes[i].attrib_tier == temp) // if equal to current min, store it
						TempArray[TARR_ORB1][res++] = i;
				}

				// res holds count now, so we must have non-zero or do nothing
				if(res) {
					res = random(0, res - 1);

					// remove the randomized minimum found
					RemoveAttributeFromItem(pnum, extra, TempArray[TARR_ORB1][res]);
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
			res = 0;

			// get the highest tier mod's tier and save it here, check if we go above it and hold the old value
			prev = GetHighestModTierOnItem(pnum, extra);
			x = prev;
			// save
			SaveUsedItemAttribs(pnum, extra);
			for(i = 0; i < affluence; ++i) {
				if(RunLuckBasedChance(pnum, DND_POTENCY_CHANCE, DND_LUCK_OUTCOME_GAIN)) {
					// failsafe, if it tried it 100 times there's a really good chance the item now has perfect tiers... don't bother
					s = 0;
				
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
						++res;

						if(PlayerInventoryList[pnum][extra].attributes[temp].attrib_tier > prev)
							prev = PlayerInventoryList[pnum][extra].attributes[temp].attrib_tier;
					}
				}
			}

			// check how many ilvls this should jump now
			temp = prev - x;
			if(temp > 0) {
				// clear difference here, so adjust ilvl accordingly by +8-10 ilvls
				for(s = 0; s < temp; ++s)
					PlayerInventoryList[pnum][extra].item_level += random(4 * MAX_CHARM_AFFIXTIERS / 5, MAX_CHARM_AFFIXTIERS);
				if(PlayerInventoryList[pnum][extra].item_level > MAX_ITEM_LEVEL)
					PlayerInventoryList[pnum][extra].item_level = MAX_ITEM_LEVEL;
			}

			// sync at end all at once
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", res);
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
		case DND_ORB_ALCHEMIST:
			// save
			SaveUsedItemAttribs(pnum, extra);

			PlayerInventoryList[pnum][extra].quality += affluence;
			prev = GetItemMaxQuality(pnum, extra);
			if(PlayerInventoryList[pnum][extra].quality > prev)
				PlayerInventoryList[pnum][extra].quality = prev;
			
			SyncItemQuality(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_EVOKER:
			// check every attribute, find the one with highest tiers, and select one randomly and get an appropriate orb
			// if it returns a tag we don't have currently, return a random orb
			prev = -1;
			s = 0;
			temp = PlayerInventoryList[pnum][extra].attrib_count;
			for(i = 0; i < temp; ++i) {
				if(PlayerInventoryList[pnum][extra].attributes[i].attrib_tier > prev) {
					prev = PlayerInventoryList[pnum][extra].attributes[i].attrib_tier;
					s = 0;
					TempArray[TARR_ORB1][s++] = PlayerInventoryList[pnum][extra].attributes[i].attrib_id;
				}
				else if(PlayerInventoryList[pnum][extra].attributes[i].attrib_tier == prev)
					TempArray[TARR_ORB1][s++] = PlayerInventoryList[pnum][extra].attributes[i].attrib_id;
			}

			// now we have a list of the highest attributes, pick one random
			s = ItemModTable[TempArray[TARR_ORB1][random(0, s - 1)]].tags;
			if(s & INV_ATTR_TAG_ATTACK)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_VIOLENCE, affluence);
			if(s & INV_ATTR_TAG_CRIT)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_DESTRUCTION, affluence);
			if(s & INV_ATTR_TAG_LIFE)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_PROSPERITY, affluence);
			if(s & INV_ATTR_TAG_DEFENSE)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_FORTITUDE, affluence);
			if(s & INV_ATTR_TAG_UTILITY)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_TINKERER, affluence);
			if(s & INV_ATTR_TAG_ELEMENTAL)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_PRISMATIC, affluence);
			if(s & INV_ATTR_TAG_EXPLOSIVE)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_TREMORS, affluence);
			if(s & INV_ATTR_TAG_OCCULT)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_HEXES, affluence);
			if(s & INV_ATTR_TAG_STAT)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_GROWTH, affluence);
			if(s & INV_ATTR_TAG_PHYSICAL)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_JAGGED, affluence);
			if(s & INV_ATTR_TAG_ENERGY)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_CRACKLING, affluence);
			if(s & INV_ATTR_TAG_MELEE)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_BRUTE, affluence);
			if(s & INV_ATTR_TAG_DAMAGE)
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_SAVAGERY, affluence);

			FreeItem(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
			GiveInventory("DnD_CleanCraftingRequest", 1);
			GiveInventory("DnD_RefreshPane", 1);
			GiveInventory("DnD_CursorDataClearRequest", 1);
			SetInventory("OrbResult", 1);
		break;
		case DND_ORB_SAVAGERY:
			// save
			SaveUsedItemAttribs(pnum, extra);
				
			s = affluence;
			for(res = 0; res < s; ++res)
				ReforgeWithOneTagGuaranteed(pnum, extra, INV_ATTR_TAG_DAMAGE_ID);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;

		case DND_ORB_HOLLOW:
			HandleAddRandomMod(pnum, extra, 1, true);
		break;
		case DND_ORB_PHANTASMAL:
			res = extra;

			SetWeaponModPowerset(pnum, extra, WEP_POWER_GHOSTHIT, true, WMOD_WEP);

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
				TempArray[TARR_ORB2][i] = -1;
			
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
			x = GetMaxItemAffixes(PlayerInventoryList[pnum][extra2].item_type, PlayerInventoryList[pnum][extra2].item_subtype);
			if(temp < x / 2)
				s = random(1, temp);
			else
				s = random(x / 2, Min(x + 1, temp));
			
			/*printbold(
				s:"start picking ", d:s, s: " attribs with ", d:temp, s: " unique attributes (random from ",
				d:Charm_MaxAffixes[PlayerInventoryList[pnum][extra2].item_subtype] / 2, s: " ", 
				d:Min(Charm_MaxAffixes[PlayerInventoryList[pnum][extra2].item_subtype] + 1, temp)
			);*/
			
			temp = 0;
			do {
				affluence = 0; // assimilation has no meaningful way of making use of this orb, so we can reuse it
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
					for(i = 0; i < MAX_ITEM_ATTRIBUTES && TempArray[TARR_ORB2][i * ATTRIB_DATA_COUNT] != -1; ++i) {
						if(picked_mod == TempArray[TARR_ORB2][i * ATTRIB_DATA_COUNT]) {
							fail_pick = true;
							break;
						}
					}

					++affluence;
				} while(fail_pick && affluence != MAX_ITER);

				// confirmed addable attribute
				TempArray[TARR_ORB2][temp * ATTRIB_DATA_COUNT] = PlayerInventoryList[pnum][res].attributes[prev].attrib_id;
				TempArray[TARR_ORB2][temp * ATTRIB_DATA_COUNT + 1] = PlayerInventoryList[pnum][res].attributes[prev].attrib_val;
				TempArray[TARR_ORB2][temp * ATTRIB_DATA_COUNT + 2] = PlayerInventoryList[pnum][res].attributes[prev].attrib_tier;
				TempArray[TARR_ORB2][temp * ATTRIB_DATA_COUNT + 3] = PlayerInventoryList[pnum][res].attributes[prev].attrib_extra;
				TempArray[TARR_ORB2][temp * ATTRIB_DATA_COUNT + 4] = PlayerInventoryList[pnum][res].attributes[prev].fractured;

				++temp;
			} while(temp < s);

			// we are going to assimilate the first one into second, so the first one is destroyed
			// and make sure the extra2 item has no attributes left anymore
			FreeItem(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
			s = ScourItem(pnum, extra2);

			// if whatever fractures this had plus however many we wanna add is bigger than its max affix + 1, set it to that
			if(temp + s > x + 1)
				temp = x + 1 - s;
			
			// copy the attributes into extra2
			for(i = 0; i < temp && TempArray[TARR_ORB2][i * ATTRIB_DATA_COUNT] != -1; ++i) {
				// check if the host already has it! we shouldn't add it again!
				if(IsAttribInItem(pnum, extra2, TempArray[TARR_ORB2][i * ATTRIB_DATA_COUNT]))
					continue;

				InsertAttributeToItem(
					pnum, 
					extra2, 
					TempArray[TARR_ORB2][i * ATTRIB_DATA_COUNT], 
					TempArray[TARR_ORB2][i * ATTRIB_DATA_COUNT + 1],
					TempArray[TARR_ORB2][i * ATTRIB_DATA_COUNT + 2],
					TempArray[TARR_ORB2][i * ATTRIB_DATA_COUNT + 3],
					TempArray[TARR_ORB2][i * ATTRIB_DATA_COUNT + 4]
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

	// reduce remaining
	amt -= res;
	
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

int CountOrbsOfTypeInventory(int pnum, int otype) {
	int amt = 0, i;
	// search player inventory to take from first
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		if(PlayerInventoryList[pnum][i].item_type == DND_ITEM_ORB && PlayerInventoryList[pnum][i].item_subtype == otype)
			amt += PlayerInventoryList[pnum][i].item_stack;
	}
	return amt;
}

// used for taking only from inventory of player, and returns error codes if it can't
int TakeOrbFromPlayer_NoStash(int pnum, int otype, int amt) {
	int i;
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
	return -1;
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
			SetInventory("OrbResult", Player_MostRecent_Orb[pnum].orb_type - 1);
		break;
		case DND_ORB_PRISMATIC:
		case DND_ORB_PROSPERITY:
		case DND_ORB_FORTITUDE:
		case DND_ORB_TINKERER:
		case DND_ORB_VIOLENCE:
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
		case DND_ORB_SAVAGERY:
		case DND_ORB_ELEVATION:
		case DND_ORB_HOLLOW:
			RestoreItemAttribsFromUsedOrb(pnum);
			SetInventory("OrbResult", Player_MostRecent_Orb[pnum].orb_type - 1);
		break;
		case DND_ORB_SIN:
			if(RunLuckBasedChance(pnum, DND_ORB_SIN_REPENTCHANCE, DND_LUCK_OUTCOME_GAIN)) {
				RestoreItemAttribsFromUsedOrb(pnum);
				SetInventory("OrbResult", Player_MostRecent_Orb[pnum].orb_type - 1);
			}
			else
				SetInventory("OrbResult", 0x7FFFFFFF);
		break;
		case DND_ORB_ALCHEMIST:
			RestoreItemQualityFromUsedOrb(pnum);
			SetInventory("OrbResult", Player_MostRecent_Orb[pnum].orb_type - 1);
		break;
		case DND_ORB_AFFLUENCE:
			TakeInventory("AffluenceCounter", 1);
			SetInventory("OrbResult", Player_MostRecent_Orb[pnum].orb_type - 1);
		break;
		case DND_ORB_CALAMITY:
			// find out how many this player does really have left, and give back that many (will give none if you used up all!)
			i = TakeOrbFromPlayer(Player_MostRecent_Orb[pnum].values[0] / 100, Player_MostRecent_Orb[pnum].values[1]);
			if(i) {
				ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, Player_MostRecent_Orb[pnum].values[0] % 100, i);
				SetInventory("OrbResult", Player_MostRecent_Orb[pnum].orb_type - 1);
			}
			else
				SetInventory("OrbResult", 0x7FFFFFFF);
		break;
		case DND_ORB_PHANTASMAL:
			temp = Player_MostRecent_Orb[pnum].values[0];
			SetWeaponModPowerset(pnum, temp, WEP_POWER_GHOSTHIT, false, WMOD_WEP);
			SyncClientsideVariable_WeaponMods(pnum, temp);
			SetInventory("OrbResult", Player_MostRecent_Orb[pnum].orb_type - 1);
		break;
	}
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
			if(val == DND_CORRUPT_SUCCESS)
				Log(s:"\cj", l:"DND_ORBUSETEXT2C");
			else if(val == DND_CORRUPT_FAIL)
				Log(s:"\cg", l:"DND_ORBUSETEXT2A");
			else
				Log(s:"\cg", l:"DND_ORBUSETEXT2B");
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
			Log(s:"\cj", l:"DND_ORBUSETEXT11", s:"\cj!");
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
			if(val > 0) {
				if(val > 1)
					Log(s:"\cj", l:"DND_ORBUSETEXT24A", s:" \cd ", d:val, s:"\c- ", l:"DND_ORBUSETEXT24B", s:"!");
				else
					Log(s:"\cj", l:"DND_ORBUSETEXT24A", s:" \cd ", d:val, s:"\c- ", l:"DND_ORBUSETEXT24C", s:"!");
			}
			else if(val == -1)
				Log(s:"\cj", l:"DND_ORBUSEFAIL24");
			else
				Log(s:"\cj", l:"DND_ORBUSEFAIL24B");
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
		case DND_ORB_ALCHEMIST:
			Log(s:"\cj", l:"DND_ORBUSETEXT28", s:"\cd ", d:affluence, s:"%", s:"\cj!");
		break;
		case DND_ORB_EVOKER:
			if(affluence > 1)
				Log(s:"\cj", l:"DND_ORBUSETEXT29", s:"\cd ", d:affluence, s:" ", l:"DND_ORBUSETEXT29BS", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT29", s:"\cd ", d:affluence, s:" ", l:"DND_ORBUSETEXT29B", s:"\cj!");
		break;
		case DND_ORB_SAVAGERY:
			Log(s:"\cj", l:"DND_ORBUSETEXT30", s:"\cj!");
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
		//i = DND_ORB_POTENCY;
		RollOrbInfo(c, i, true, stack);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(GetInventoryName(i + ORBS_BEGIN), 24.0, 16, pnum + 1, c);
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
#ifndef ISDEBUGBUILD
				if(PlayerActivities[j].loot_penalty)
					continue;
#endif
				SpawnOrb(j, false, false, stack);
		}
	}
}

void SpawnSpecificOrb(int pnum, int id, bool sound, bool noRepeat = false) {
	int c = CreateItemSpot();
	if(c != -1) {
		RollOrbInfo(c, id, true);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(GetInventoryName(id + ORBS_BEGIN), 24.0, 16, pnum + 1, c);
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

Script "DND Orb Use Message" (int type, int result, int affluence) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;
	HandleOrbUseMessage(type, result, affluence);
}

Script "DND Orb Use Message (Server)" (void) {
	ACS_NamedExecuteAlways("DND Orb Use Message", 0, GetAffluenceBonus());
}

Script "DND Orb Use" (int orbtype, int extra, int extra2) {
	int pnum = PlayerNumber();
	HandleOrbUse(pnum, orbtype, extra, extra2);
	LocalAmbientSound("Items/OrbUse", 127);
	ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, pnum);
	Delay(const:2);
	if(orbtype != DND_ORB_AFFLUENCE)
		SetInventory("AffluenceCounter", 0);
}

#endif
