#ifndef DND_ORBS_IN
#define DND_ORBS_IN

#include "DnD_OrbDefs.h"
#include "../DnD_Stat.h"
#include "../DnD_WeaponDefs.h"
#include "../DnD_Sync.h"

#define MAX_ITER 200

#define ENHANCEORB_MAX 50 // 50% bonus
#define AFFLUENCE_MAX 4 // x16
#define AFFLUENCE_MULT 2

#define DND_ORB_SIN_REPENTCHANCE 0.4

int GetOrbDropWeight(int id) {
	static int OrbDropWeights[MAX_ORBS] = {
		450, 		// DND_ORB_ENHANCE
		200, 		// DND_ORB_CORRUPT
		300, 		// DND_ORB_PRISMATIC
		75, 		// DND_ORB_REPENT
		240, 		// DND_ORB_AFFLUENCE

		250,		// DND_ORB_CALAMITY
		300,		// DND_ORB_PROSPERITY
		250,		// DND_ORB_NULLIFICATION
		300,		// DND_ORB_DESTRUCTION
		300,		// DND_ORB_VIOLENCE

		300,		// DND_ORB_FORTITUDE
		100,		// DND_ORB_SIN
		300,		// DND_ORB_TREMORS
		300,		// DND_ORB_TINKERER
		750,		// DND_ORB_REFINEMENT

		400,		// DND_ORB_SCULPTING
		400,		// DND_ORB_ELEVATION
		750,		// DND_ORB_TURMOIL
		300,		// DND_ORB_HEXES
		300,		// DND_ORB_GROWTH

		250,		// DND_ORB_POTENCY
		300,		// DND_ORB_CRACKLING
		300,		// DND_ORB_BRUTE
		300,		// DND_ORB_JAGGED
		250,		// DND_ORB_ALCHEMIST

		250,		// DND_ORB_EVOKER
		300,		// DND_ORB_SAVAGERY

		300,		// DND_ORB_WINTER
		300,		// DND_ORB_VOLTAIC
		300,		// DND_ORB_VILE
		300,		// DND_ORB_EMBERS

		// drops only from specific monster
		0xFFFFFF,
		0xFFFFFF,
		0xFFFFFF
	};

	return OrbDropWeights[id];
}

#define ORB_MAXWEIGHT 1000
#define DND_CORRUPT_FAIL 419
#define DND_CORRUPT_SUCCESS 420

bool CanAddModToItem(int pnum, int itemtype, int item_index, int add_lim) {
	bool res = false;
	if(IsUsableOnInventory(itemtype)) {
		auto item = GetPlayerInventoryItem(pnum, item_index);
		// this one depends on attribute counts of items it is used on
		if(item.item_type > UNIQUE_BEGIN)
			res = false;
		else {
			res = item.attrib_count < GetMaxItemAffixes(item.item_type, item.item_subtype) + add_lim;
		}
	}
	return res;
}

bool IsCorruptableItem(int item_type) {
	switch(item_type) {
		case DND_ITEM_FLASK:
		case DND_ITEM_DUNGEONKEY:
		return false;
	}

	return true;
}

bool IsFracturableItem(int item_type) {
	switch(item_type) {
		case DND_ITEM_FLASK:
		return false;
	}
	return true;
}

bool IsEvokableItem(int item_type) {
	switch(item_type) {
		case DND_ITEM_FLASK:
		case DND_ITEM_DUNGEONKEY:
		return false;
	}

	return true;
}

// extra is inventory position
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
#ifdef ISDEBUGBUILD
			res = true;
#else
			res = IsUsableOnInventory(extratype) && !IsInventoryCorrupted(pnum, extra) && IsCorruptableItem(extratype);
#endif
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
			if(IsUsableOnInventory(extratype) && !IsInventoryCorrupted(pnum, extra) && IsFracturableItem(extratype)) {
				// if there's a fractured mod or it's a unique, don't let
				auto item = GetPlayerInventoryItem(pnum, extra);
				temp = item.item_type;

				// unique case
				res = temp < UNIQUE_BEGIN;

				// mod count > half case
				res = res && item.attrib_count >= Max(2, 1 + GetMaxItemAffixes(temp, item.item_subtype) / 2);

				// fracture case
				for(i = 0; res && i < item.attrib_count; ++i) {
					if(item.attributes[i].fractured) {
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
		case DND_ORB_TREMORS:
		case DND_ORB_HEXES:
		case DND_ORB_GROWTH:
		case DND_ORB_CRACKLING:
		case DND_ORB_BRUTE:
		case DND_ORB_JAGGED:
		case DND_ORB_SAVAGERY:
		case DND_ORB_WINTER:
		case DND_ORB_VOLTAIC:
		case DND_ORB_VILE:
		case DND_ORB_EMBERS:
			if(IsUsableOnInventory(extratype) && !IsInventoryCorrupted(pnum, extra) && extratype != DND_ITEM_FLASK) {
				// don't let this be used on a unique
				res = GlobalItemStorage.PlayerInventoryList[pnum][extra].item_type < UNIQUE_BEGIN;
			}
		break;
		case DND_ORB_TURMOIL:
			if(IsUsableOnInventory(extratype) && !IsInventoryCorrupted(pnum, extra)) {
				// don't let this be used on a unique
				res = GlobalItemStorage.PlayerInventoryList[pnum][extra].item_type < UNIQUE_BEGIN;
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
				item = GetPlayerInventoryItem(pnum, extra);
				if(item.item_type > UNIQUE_BEGIN)
					res = false;
				else
					res = item.attrib_count - GetItemFracturedModCount(pnum, extra) > 0;
			}
		break;
		case DND_ORB_ELEVATION:
			res =  !IsInventoryCorrupted(pnum, extra) && CanAddModToItem(pnum, extratype, extra, 0);
		break;
		case DND_ORB_POTENCY:
			item = GetPlayerInventoryItem(pnum, extra);
			if(!IsInventoryCorrupted(pnum, extra) && item.item_type < UNIQUE_BEGIN) {
				temp = item.attrib_count;
				res = false;
				for(i = 0; i < temp; ++i) {
					if(item.attributes[i].fractured)
						continue;

					if(item.attributes[i].attrib_tier < MAX_CHARM_AFFIXTIERS)
						res = true;
				}
			}
		break;
		case DND_ORB_ALCHEMIST:
			item = GetPlayerInventoryItem(pnum, extra);
			res = (extratype & 0xFFFF) == DND_ITEM_CHARM && item.quality < GetItemMaxQuality(pnum, extra);

			// check if item has cybernetic -- it shouldn't have it!
			res &= !ItemIsCybernetic(pnum, extra, item.attrib_count, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			res &= !IsInventoryCorrupted(pnum, extra);
		break;
		case DND_ORB_EVOKER:
			// won't work on uniques
			item = GetPlayerInventoryItem(pnum, extra);
			res = item.attrib_count > 0 && item.item_type < UNIQUE_BEGIN && IsEvokableItem(extratype);
		break;

		case DND_ORB_HOLLOW:
			// this one is basically an orb of elevation with a +1 additional attribute allowed
			res = !IsInventoryCorrupted(pnum, extra) && extratype != DND_ITEM_FLASK && CanAddModToItem(pnum, extratype, extra, 1);
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
			item = GetPlayerInventoryItem(pnum, extra);
			res = 	item.item_type == GlobalItemStorage.PlayerInventoryList[pnum][extratype].item_type && 
					item.item_type < UNIQUE_BEGIN && GlobalItemStorage.PlayerInventoryList[pnum][extratype].item_type < UNIQUE_BEGIN &&
					(item.item_type != DND_ITEM_CHARM || (item.item_subtype == GlobalItemStorage.PlayerInventoryList[pnum][extratype].item_subtype)) &&
					(item.attrib_count && GlobalItemStorage.PlayerInventoryList[pnum][extratype].attrib_count);
		break;
		case DND_ORB_REVERANCE:
		case DND_ORB_DESTINY:
		case DND_ORB_ORDER:
			res = true;
		break;
	}
	if(!res)
		SetInventory("OrbResult", 0x7FFFFFFF);
	return res;
}

void HandleAddRandomMod(int pnum, int item_index, int add_lim, bool isWellRolled) {
	int i, temp, aff = GetAffluenceBonus();
	bool finish = false;
	auto item = GetPlayerInventoryItem(pnum, item_index);
	int max_possible = GetMaxItemAffixes(item.item_type, item.item_subtype);

	// save
	SaveUsedItemAttribs(pnum, item_index);

	int special_roll = GetSpecialRollAttribute(pnum, item_index);

	for(int s = 0; s < aff && !finish; ++s) {
		i = item.attrib_count;
		// find an attribute that this item doesn't have
		do {
			temp = PickRandomAttribute(
				item.item_type,
				item.item_subtype, 
				special_roll, 
				item.implicit[0].attrib_id,
				CheckInventory("OrderStored")
			);
		} while(CheckItemAttribute(pnum, item_index, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, i) != -1);
		
		// if not well rolled by default, run the chance (orbs may force it, but sometimes they may not)
		AddAttributeToItem(pnum, item_index, temp, !isWellRolled ? CheckWellRolled(pnum) : isWellRolled);
		finish = item.attrib_count >= max_possible + add_lim;
	}

	if(CheckInventory("OrderStored"))
		SetInventory("OrderStored", 0);

	SyncItemAttributes(pnum, item_index, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	SetInventory("OrbResult", item_index);
}

// picks an item id from two weigted by their level x 10
int PickWeightedFromTwoItems(int pnum, int item1, int item2) {
	int lvl1 = GlobalItemStorage.PlayerInventoryList[pnum][item1].item_level * 10;
	int lvl2 = GlobalItemStorage.PlayerInventoryList[pnum][item2].item_level * 10;

	if(lvl1 == lvl2)
		return random(0, 1) ? item1 : item2;

	int res = random(1, lvl1 + lvl2);
	
	// pick from the weight which item index we are taking
	if(res > lvl1)
		return item2;
	return item1;
}

void SaveUsedItemAttribs(int pnum, int item_id) {
	auto item = GetPlayerInventoryItem(pnum, item_id);
	Player_MostRecent_Orb[pnum].p_tempwep = item_id + 1;
	Player_MostRecent_Orb[pnum].values[0] = item.attrib_count;
	Player_MostRecent_Orb[pnum].values[1] = item.item_level;

	for(int i = 0; i < Player_MostRecent_Orb[pnum].values[0]; ++i) {
		Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 2] = item.attributes[i].attrib_id;
		Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 3] = item.attributes[i].attrib_val;
		Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 4] = item.attributes[i].attrib_tier;
		Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 5] = item.attributes[i].attrib_extra;
		Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 6] = item.attributes[i].fractured;
	}
}

void SaveUsedItemQuality(int pnum, int item_id) {
	Player_MostRecent_Orb[pnum].p_tempwep = item_id + 1;
	Player_MostRecent_Orb[pnum].values[0] = GlobalItemStorage.PlayerInventoryList[pnum][item_id].quality;
}

void SaveUsedItemImplicit(int pnum, int item_id) {
	auto item = GetPlayerInventoryItem(pnum, item_id);
	Player_MostRecent_Orb[pnum].p_tempwep = item_id + 1;

	for(int i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		Player_MostRecent_Orb[pnum].values[IMPLICIT_DATA_COUNT * i] = item.implicit[i].attrib_id;
		Player_MostRecent_Orb[pnum].values[IMPLICIT_DATA_COUNT * i + 1] = item.implicit[i].attrib_val;
		Player_MostRecent_Orb[pnum].values[IMPLICIT_DATA_COUNT * i + 2] = item.implicit[i].attrib_tier;
		Player_MostRecent_Orb[pnum].values[IMPLICIT_DATA_COUNT * i + 3] = item.implicit[i].attrib_extra;
	}
}

void RestoreItemAttribsFromUsedOrb(int pnum) {
	int temp = Player_MostRecent_Orb[pnum].p_tempwep - 1;
	auto item = GetPlayerInventoryItem(pnum, temp);
	item.attrib_count = Player_MostRecent_Orb[pnum].values[0];
	item.item_level = Player_MostRecent_Orb[pnum].values[1];
	
	for(int i = 0; i < Player_MostRecent_Orb[pnum].values[0]; ++i) {
		item.attributes[i].attrib_id = Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 2];
		item.attributes[i].attrib_val = Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 3];
		item.attributes[i].attrib_tier = Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 4];
		item.attributes[i].attrib_extra = Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 5];
		item.attributes[i].fractured = Player_MostRecent_Orb[pnum].values[ATTRIB_DATA_COUNT * i + 6];
	}
	SyncItemAttributes(pnum, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

void RestoreItemImplicitsFromUsedOrb(int pnum) {
	int temp = Player_MostRecent_Orb[pnum].p_tempwep - 1;
	auto item = GetPlayerInventoryItem(pnum, temp);

	for(int i = 0; i < MAX_ITEM_IMPLICITS; ++i) {
		item.implicit[i].attrib_id = Player_MostRecent_Orb[pnum].values[IMPLICIT_DATA_COUNT * i];
		item.implicit[i].attrib_val = Player_MostRecent_Orb[pnum].values[IMPLICIT_DATA_COUNT * i + 1];
		item.implicit[i].attrib_tier = Player_MostRecent_Orb[pnum].values[IMPLICIT_DATA_COUNT * i + 2];
		item.implicit[i].attrib_extra = Player_MostRecent_Orb[pnum].values[IMPLICIT_DATA_COUNT * i + 3];
	}

	SyncItemImplicits(pnum, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

void RestoreItemQualityFromUsedOrb(int pnum) {
	int temp = Player_MostRecent_Orb[pnum].p_tempwep - 1;
	GlobalItemStorage.PlayerInventoryList[pnum][temp].quality = Player_MostRecent_Orb[pnum].values[0];
	SyncItemQuality(pnum, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

void HandleTaggedModGive(int pnum, int extra, int tag, int affluence, bool well_rolled = false) {
	// little exception handling, as these aren't very dense in tags yet
	if(GlobalItemStorage.PlayerInventoryList[pnum][extra].item_type == DND_ITEM_DUNGEONKEY) {
		SaveUsedItemAttribs(pnum, extra);
		ReforgeItem(pnum, extra);
		SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		SetInventory("OrbResult", extra);
		return;
	}

	if(!CheckInventory("OrderUsed")) {
		SaveUsedItemAttribs(pnum, extra);

		ReforgeWithOneTagGuaranteed(pnum, extra, tag, affluence, well_rolled);
		
		SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		SetInventory("OrbResult", extra);
	}
	else {
		// saves +1 of the actual tag if we havent used one, otherwise similar functionality
		if(!CheckInventory("OrderStored"))
			SetInventory("OrderStored", tag + 1);
		else {
			SetInventory("OrderStored", 0);
			SetInventory("OrderUsed", 0);

			// same stub as above
			SaveUsedItemAttribs(pnum, extra);
					
			ReforgeWithOneTagGuaranteed(pnum, extra, tag, affluence, well_rolled);
			
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		}
	}
}

// Returns -1 if theres nothing stored
// Returns 0 if OrderStored but tags dont match
// Returns 1 if OrderStored and tags match
int CheckOrderOrb(int attribute_id) {
	int stored_tag = CheckInventory("OrderStored");
	if(!stored_tag)
		return -1;

	// we store tag value +1
	--stored_tag;
	//printbold(s:"stored tag: ", d:stored_tag, s: " return: ", d:ItemModTable[attribute_id].tags & (1 << stored_tag));
	return ItemModTable[attribute_id].tags & (1 << stored_tag);
}

// extra holds item position
// extra2 only matters for orb of assimilation (or 2 item use crafts)
void HandleOrbUse (int pnum, int orbtype, int extra, int extra2 = -1) {
	int res = -1;
	int temp;
	int prev;
	int i, s, x = -1;
	int affluence = GetAffluenceBonus();
	int overrideValue = 0;

	SetInventory("OrbUseType", orbtype + 1);
	// for any other orb, reset most recently used orb
	if(orbtype != DND_ORB_REPENT)
		ResetMostRecentOrb(pnum);
	switch(orbtype) {
		case DND_ORB_ENHANCE:
			res = extra;
			prev = GetPlayerWeaponQuality(pnum, res);
			affluence *= random(QUALITY_ITEM_ADD_MIN, QUALITY_ITEM_ADD_MAX);
			temp = Clamp_Between(prev + affluence, 0, ENHANCEORB_MAX);
			if(CheckInventory("ReveranceUsed"))
				temp = ENHANCEORB_MAX;

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
			auto item = GetPlayerInventoryItem(pnum, extra);
			item.corrupted = true;

#ifdef ISDEBUGBUILD
			temp = 1000;
#else
			temp = random(1, 1000);
#endif

			if(temp <= 500 && !CheckInventory("DestinyUsed")) {
				// does nothing but corrupt the item
				SetInventory("OrbResult", DND_CORRUPT_FAIL);
				SyncItemImplicits(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			}
			else if(temp <= 750 && !CheckInventory("DestinyUsed")) {
				// if it was a unique item, destroy it
				if(item.item_type > UNIQUE_BEGIN) {
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
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_ELEMENTAL_ID, affluence, true);
		break;
		case DND_ORB_REPENT:
			RevertLastOrbEffect();
		break;
		case DND_ORB_AFFLUENCE:
			GiveInventory("AffluenceCounter", 1);
			if(CheckInventory("ReveranceUsed"))
				GiveInventory("AffluenceCounter", 3);
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
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_LIFE_ID, affluence);
		break;
		case DND_ORB_FORTITUDE:
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_DEFENSE_ID, affluence);
		break;
		case DND_ORB_NULLIFICATION:
			// save
			SaveUsedItemAttribs(pnum, extra);
			item = GetPlayerInventoryItem(pnum, extra);
			for(s = 0; s < affluence && item.attrib_count; ++s) {
				// find the attribute with the lowest tier, in case of multiple, return a random one
				res = 0;
				temp = MAX_CHARM_AFFIXTIERS;
				for(i = 0; i < item.attrib_count; ++i) {
					// ignore the fractured mods
					if(item.attributes[i].fractured || !CheckOrderOrb(item.attributes[i].attrib_id))
						continue;

					if(item.attributes[i].attrib_tier < temp) {
						// we use res to hold the count of elements in this temporary array
						// reset current count if we found a new minimum, then add it to our array

						res = 0;
						TempArray[TARR_ORB1][res++] = i;
						temp = item.attributes[i].attrib_tier;
					}
					else if(item.attributes[i].attrib_tier == temp) // if equal to current min, store it
						TempArray[TARR_ORB1][res++] = i;
				}

				// consumed now
				SetInventory("OrderStored", 0);

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
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_CRIT_ID, affluence);
		break;
		case DND_ORB_VIOLENCE:
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_ATTACK_ID, affluence);
		break;
		case DND_ORB_SIN:
			// save
			SaveUsedItemAttribs(pnum, extra);
			item = GetPlayerInventoryItem(pnum, extra);

			// pick random attribute to fracture
			// if order orb is used, attempt to pick attribute that matches tag -- maximum of 30 tries
			temp = 0;
			do {
				s = random(0, item.attrib_count - 1);
			} while(!CheckOrderOrb(item.attributes[s].attrib_id) || temp++ < DND_MAX_ORB_REROLL_ATTEMPTS);

			SetInventory("OrderStored", 0);

			item.attributes[s].fractured = true;
		
			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_TREMORS:
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_EXPLOSIVE_ID, affluence);
		break;
		case DND_ORB_TINKERER:
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_UTILITY_ID, affluence);
		break;
		case DND_ORB_REFINEMENT:
			// save
			SaveUsedItemAttribs(pnum, extra);
			item = GetPlayerInventoryItem(pnum, extra);
				
			s = affluence;
			if(item.item_type > UNIQUE_BEGIN) {
				// handle unique random roll case
				temp = (item.item_type >> UNIQUE_BITS) - 1;
				for(res = 0; res < s; ++res) {
					for(i = 0; i < item.attrib_count; ++i) {
						// if its not a regular attribute that can have tags we shouldn't consider
						if(IsUniqueModRerollException(item.attributes[i].attrib_id) || (item.attributes[i].attrib_id <= LAST_INV_ATTRIBUTE && !CheckOrderOrb(item.attributes[i].attrib_id)))
							continue;

						item.attributes[i].attrib_val = RollUniqueAttributeValue(temp, i, CheckWellRolled(pnum));
						if(item.attributes[i].attrib_extra)
							item.attributes[i].attrib_extra = RollUniqueAttributeExtra(temp, i, CheckWellRolled(pnum));
					}
				}
			}
			else {
				for(res = 0; res < s; ++res) {
					for(i = 0; i < item.attrib_count; ++i) {
						// ignore fractured mods
						if(item.attributes[i].fractured || !CheckOrderOrb(item.attributes[i].attrib_id))
							continue;

						temp = item.attributes[i].attrib_id;
						x = CheckWellRolled(pnum);
						item.attributes[i].attrib_val = RollAttributeValue(
							temp, 
							item.attributes[i].attrib_tier, 
							x,
							item.item_type,
							item.item_subtype
						);

						if(CanRerollAttributeExtra(temp)) {
							item.attributes[i].attrib_extra = GetExtraForMod(
								pnum, temp, item.attributes[i].attrib_tier,
								item.item_type,
								item.item_subtype,
								x,
								item.attributes[i].attrib_val
							);
						}
						else if(item.item_type == DND_ITEM_DUNGEONKEY) {
							item.attributes[i].attrib_extra = (item.attributes[i].attrib_extra & 0xFFFF) |
								RollDungeonAttributeExtra(
									(item.attributes[i].attrib_extra & 0xFFFF), 
									temp, 
									item.attributes[i].attrib_tier, 
									x
								) << 16;
						}
					}
				}
			}

			SetInventory("OrderStored", 0);

			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_SCULPTING:
			// save
			SaveUsedItemAttribs(pnum, extra);
			item = GetPlayerInventoryItem(pnum, extra);
			
			temp = 0;
			for(s = 0; s < affluence && item.attrib_count; ++s) {
				// reroll if fractured
				do {
					res = random(0, item.attrib_count - 1);
				} while(!CheckOrderOrb(item.attributes[res].attrib_id) || (temp++ < DND_MAX_ORB_REROLL_ATTEMPTS && item.attributes[res].fractured));
				
				// just to be safe
				if(!item.attributes[res].fractured)
					RemoveAttributeFromItem(pnum, extra, res);
			}

			SetInventory("OrderStored", 0);

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
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_OCCULT_ID, affluence);
		break;
		case DND_ORB_GROWTH:
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_STAT_ID, affluence);
		break;
		case DND_ORB_POTENCY:
			res = 0;

			// get the highest tier mod's tier and save it here, check if we go above it and hold the old value
			prev = GetHighestModTierOnItem(pnum, extra);
			x = prev;
			// save
			SaveUsedItemAttribs(pnum, extra);
			item = GetPlayerInventoryItem(pnum, extra);
			for(i = 0; i < affluence; ++i) {
				if(RunLuckBasedChance(pnum, DND_POTENCY_CHANCE) || CheckInventory("DestinyUsed")) {
					// failsafe, if it tried it 100 times there's a really good chance the item now has perfect tiers... don't bother
					s = 0;
				
					do {
						temp = random(0, item.attrib_count - 1);
					} while(
						!CheckOrderOrb(item.attributes[temp].attrib_id) ||
						(
							s++ < DND_MAX_ORB_REROLL_ATTEMPTS && 
							(item.attributes[temp].attrib_tier == MAX_CHARM_AFFIXTIERS || item.attributes[temp].fractured)
						)
					);
					
					// increment the tier and reroll that attribute!
					if(!item.attributes[temp].fractured && item.attributes[temp].attrib_tier < MAX_CHARM_AFFIXTIERS) {
						++item.attributes[temp].attrib_tier;
						item.attributes[temp].attrib_val = RollAttributeValue(
							item.attributes[temp].attrib_id, 
							item.attributes[temp].attrib_tier,
							false, 
							item.item_type, 
							item.item_subtype
						);
						++res;

						if(item.attributes[temp].attrib_tier > prev)
							prev = item.attributes[temp].attrib_tier;
					}
				}
			}

			SetInventory("OrderStored", 0);

			// check how many ilvls this should jump now -- only if the item tier itself is higher than the allowed item level range, so low tiers increasing on level 50 don't cause this bump
			// this rounds it up, so getting tier 5 on ilvl 48 for example would check vs 50 > 48 + 5 / 10 = 5 * 10 = 50, and it's okay that way
			temp = prev - x;
			if(temp > 0 && prev * MAX_CHARM_AFFIXTIERS > ((item.item_level + 5) / MAX_CHARM_AFFIXTIERS) * MAX_CHARM_AFFIXTIERS) {
				// clear difference here, so adjust ilvl accordingly by +6-10 ilvls
				for(s = 0; s < temp; ++s)
					item.item_level += random(3 * MAX_CHARM_AFFIXTIERS / 5, MAX_CHARM_AFFIXTIERS);
				if(item.item_level > MAX_ITEM_LEVEL)
					item.item_level = MAX_ITEM_LEVEL;
			}

			SyncItemAttributes(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", res);
		break;
		case DND_ORB_CRACKLING:
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_ENERGY_ID, affluence);
		break;
		case DND_ORB_BRUTE:
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_MELEE_ID, affluence);
		break;
		case DND_ORB_JAGGED:
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_PHYSICAL_ID, affluence);
		break;
		case DND_ORB_ALCHEMIST:
			// save
			SaveUsedItemAttribs(pnum, extra);
			item = GetPlayerInventoryItem(pnum, extra);

			affluence *= random(QUALITY_ITEM_ADD_MIN, QUALITY_ITEM_ADD_MAX);
			item.quality += affluence;
			prev = GetItemMaxQuality(pnum, extra);
			if(item.quality > prev || CheckInventory("ReveranceUsed")) {
				item.quality = prev;
				overrideValue = CheckInventory("ReveranceUsed") * prev;
			}
			
			SyncItemQuality(pnum, extra, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("OrbResult", extra);
		break;
		case DND_ORB_EVOKER:
			// check every attribute, find the one with highest tiers, and select one randomly and get an appropriate orb
			// if it returns a tag we don't have currently, return a random orb
			prev = -1;
			s = 0;
			item = GetPlayerInventoryItem(pnum, extra);
			temp = item.attrib_count;
			for(i = 0; i < temp; ++i) {
				if(item.attributes[i].attrib_tier > prev) {
					prev = item.attributes[i].attrib_tier;
					s = 0;
					TempArray[TARR_ORB1][s++] = item.attributes[i].attrib_id;
				}
				else if(item.attributes[i].attrib_tier == prev)
					TempArray[TARR_ORB1][s++] = item.attributes[i].attrib_id;
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
			if(s & INV_ATTR_TAG_ELEMENTAL) {
				if(!random(0, 4))
					ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_PRISMATIC, affluence);
				else {
					if(s & INV_ATTR_TAG_ICE)
						ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_WINTER, affluence);
					else if(s & INV_ATTR_TAG_LIGHTNING)
						ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_VOLTAIC, affluence);
					else if(s & INV_ATTR_TAG_POISON)
						ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_VILE, affluence);
					else
						ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_EMBERS, affluence);
				}
			}
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
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_DAMAGE_ID, affluence);
		break;
		case DND_ORB_WINTER:
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_ICE_ID, affluence);
		break;
		case DND_ORB_VOLTAIC:
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_LIGHTNING_ID, affluence);
		break;
		case DND_ORB_VILE:
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_POISON_ID, affluence);
		break;
		case DND_ORB_EMBERS:
			HandleTaggedModGive(pnum, extra, INV_ATTR_TAG_FIRE_ID, affluence);
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
			int order_result = -1;
			bool fail_pick;
			
			// init the temp array to -1s
			for(i = 0; i < MAX_ITEM_ATTRIBUTES * ATTRIB_DATA_COUNT; ++i)
				TempArray[TARR_ORB2][i] = -1;
			
			// we determine when to stop, we either stop until max affixes or we have minimum of both attrib counts total
			// make the latter count unique attributes on both combined, not just total attribute count of both
			// for that we assume base count on item 1, then we check item 2 for non-occuring attributes
			item = GetPlayerInventoryItem(pnum, extra);
			temp = item.attrib_count;
			for(i = 0; i < GlobalItemStorage.PlayerInventoryList[pnum][extra2].attrib_count; ++i) {
				fail_pick = false;
				for(s = 0; s < item.attrib_count; ++s) {
					if(GlobalItemStorage.PlayerInventoryList[pnum][extra2].attributes[i].attrib_id == item.attributes[s].attrib_id) {
						fail_pick = true;
						break;
					}
				}
				temp += !fail_pick;
			}
			
			// will pick anywhere from half of max affix count of a charm to max affix count + 1
			// if we don't have at least half of affix count of item on total sum of mods, we'll pick between 1 and the sum instead
			x = GetMaxItemAffixes(GlobalItemStorage.PlayerInventoryList[pnum][extra2].item_type, GlobalItemStorage.PlayerInventoryList[pnum][extra2].item_subtype);
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
					item = GetPlayerInventoryItem(pnum, res);

					if(!item.attrib_count)
						continue;

					prev = random(0, item.attrib_count - 1);
					//log(s:"picked ", d: res, s:" from: ", d:extra, s: " - ", d:extra2, s: "\nmod: ", d:picked_mod);
					
					// set this to be the actual attribute now instead of just an index in the item itself
					picked_mod = item.attributes[prev].attrib_id;
					
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
				TempArray[TARR_ORB2][temp * ATTRIB_DATA_COUNT] = item.attributes[prev].attrib_id;
				TempArray[TARR_ORB2][temp * ATTRIB_DATA_COUNT + 1] = item.attributes[prev].attrib_val;
				TempArray[TARR_ORB2][temp * ATTRIB_DATA_COUNT + 2] = item.attributes[prev].attrib_tier;
				TempArray[TARR_ORB2][temp * ATTRIB_DATA_COUNT + 3] = item.attributes[prev].attrib_extra;
				TempArray[TARR_ORB2][temp * ATTRIB_DATA_COUNT + 4] = item.attributes[prev].fractured;

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
				if(IsAttribInItem(pnum, extra2, TempArray[TARR_ORB2][i * ATTRIB_DATA_COUNT]) || !(order_result = CheckOrderOrb(TempArray[TARR_ORB2][i * ATTRIB_DATA_COUNT])))
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

			SetInventory("OrderStored", 0);
			
			SyncItemData(pnum, extra2, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, GlobalItemStorage.PlayerInventoryList[pnum][extra2].width, GlobalItemStorage.PlayerInventoryList[pnum][extra2].height);
			GiveInventory("DnD_CleanCraftingRequest", 1);
			GiveInventory("DnD_RefreshPane", 1);
			SetInventory("OrbResult", 1);
		break;

		case DND_ORB_DESTINY:
			GiveInventory("DestinyUsed", 1);
			res = CheckInventory("DestinyUsed");
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
		break;
		case DND_ORB_REVERANCE:
			GiveInventory("ReveranceUsed", 1);
			res = CheckInventory("ReveranceUsed");
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
		break;
		case DND_ORB_ORDER:
			GiveInventory("OrderUsed", 1);
			res = CheckInventory("OrderUsed");
			SetInventory("OrbResult", res);
			Player_MostRecent_Orb[pnum].values[0] = res;
		break;
	}

	if(orbtype != DND_ORB_DESTINY)
		TakeInventory("DestinyUsed", 1);
	if(orbtype != DND_ORB_REVERANCE)
		TakeInventory("ReveranceUsed", 1);
	if(orbtype != DND_ORB_ORDER)
		TakeInventory("OrderUsed", 1);

	Player_MostRecent_Orb[pnum].orb_type = orbtype + 1; // +1 because 0 is used as no orb
	
	// clientside msg
	ACS_NamedExecuteWithResult("DND Orb Use Message", CheckInventory("OrbUseType") - 1, CheckInventory("OrbResult"), affluence, overrideValue);
}

// check if player has any orbs besides calamity
bool HasOrbsBesidesCalamity() {
	int pn = PlayerNumber();
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto item = GetPlayerInventoryItem(pn, i);
		if(item.item_type == DND_ITEM_ORB) {
			if(item.item_subtype == DND_ORB_CALAMITY || IsOrbDropException(item.item_subtype))
				continue;
			return true;
		}
	}
	return false;
}

bool HasOrbOfType(int type) {
	int pn = PlayerNumber();
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto item = GetPlayerInventoryItem(pn, i);
		if(item.item_type == DND_ITEM_ORB && item.item_subtype == type)
			return true;
	}
	return false;
}

int TakeOrbFromPlayer(int otype, int amt) {
	int i, j, pnum = PlayerNumber();
	int res = 0;
	// search player inventory to take from first
	for(i = 0; i < MAX_INVENTORY_BOXES && res < amt; ++i) {
		auto item = GetPlayerInventoryItem(pnum, i);
		if(item.item_type == DND_ITEM_ORB && item.item_subtype == otype) {
			if(item.item_stack > amt - res) {
				UsePlayerItem_Count(pnum, i, amt - res);
				res += amt;
			}
			else { 
				// use however many we are left to reach amt
				res += item.item_stack;
				UsePlayerItem_Count(pnum, i, item.item_stack);
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
			item = GetPlayerStashItem(pnum, i, j);
			if(item.item_type == DND_ITEM_ORB && item.item_subtype == otype) {
				if(item.item_stack > amt - res) {
					UsePlayerStashItem_Count(pnum, i, j, amt - res);
					res += amt;
				}
				else { 
					// use however many we are left to reach amt
					res += item.item_stack;
					UsePlayerStashItem_Count(pnum, i, j, item.item_stack);
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
		auto item = GetPlayerInventoryItem(pnum, i);
		if(item.item_type == DND_ITEM_ORB && item.item_subtype == otype)
			amt += item.item_stack;
	}
	return amt;
}

// used for taking only from inventory of player, and returns error codes if it can't
int TakeOrbFromPlayer_NoStash(int pnum, int otype, int amt) {
	int i;
	int res = 0;
	// search player inventory to take from first
	for(i = 0; i < MAX_INVENTORY_BOXES && res < amt; ++i) {
		auto item = GetPlayerInventoryItem(pnum, i);
		if(item.item_type == DND_ITEM_ORB && item.item_subtype == otype) {
			if(item.item_stack > amt - res) {
				UsePlayerItem_Count(pnum, i, amt - res);
				res += amt;
			}
			else { 
				// use however many we are left to reach amt
				res += item.item_stack;
				UsePlayerItem_Count(pnum, i, item.item_stack);
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
		auto item = GetPlayerInventoryItem(pnum, i);
		if(item.item_type != DND_ITEM_NULL) {
			// we can add to this item's stack
			if(amt - res < j - item.item_stack) {
				item.item_stack += amt - res;
				res = amt;
			}
			else {
				// difference is added
				res += j - item.item_stack;
				item.item_stack = j;
			}
			SyncItemStack(pnum, i, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
		}
		else {
			// implant this item here, and add as many stacks as we can to this
			item.item_level = 1;
			item.item_type = DND_ITEM_ORB;
			item.item_subtype = otype;
			item.width = 1;
			item.height = 1;
			item.attrib_count = 0;
			item.item_image = GetOrbItemImage(otype);
			item.topleftboxid = i + 1;

			if(amt - res < j - item.item_stack) {
				item.item_stack += amt - res;
				res = amt;
			}
			else {
				// difference is added
				res += j - item.item_stack;
				item.item_stack = j;
			}
			SyncItemData(pnum, i, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, 1, 1);
		}
	}
	
	if(res < amt) {
		// no space or need more? try stash
		for(i = 0; i < CheckInventory("DnD_PlayerInventoryPages") && res < amt; ++i) {
			for(j = 0; j < MAX_INVENTORY_BOXES && res < amt; ++j) {
				item = GetPlayerStashItem(pnum, i, j);
				if(item.item_type == DND_ITEM_NULL) {
					// implant item here with remaining stacks
					item.item_level = 1;
					item.item_type = DND_ITEM_ORB;
					item.item_subtype = otype;
					item.width = 1;
					item.height = 1;
					item.attrib_count = 0;
					item.item_image = GetOrbItemImage(otype);
					item.topleftboxid = j + 1;
					
					if(amt - res < GetStackValue(DND_ITEM_ORB) - item.item_stack) {
						item.item_stack += amt - res;
						res = amt;
					}
					else {
						res += GetStackValue(DND_ITEM_ORB) - item.item_stack;
						item.item_stack = GetStackValue(DND_ITEM_ORB);
					}
					SyncItemData(pnum, j, DND_SYNC_ITEMSOURCE_STASH | (i << 16), 1, 1);
				}
				else if(item.item_type == DND_ITEM_ORB && item.item_subtype == otype) {
					if(amt - res < GetStackValue(DND_ITEM_ORB) - item.item_stack) {
						item.item_stack += amt - res;
						res = amt;
					}
					else {
						res += GetStackValue(DND_ITEM_ORB) - item.item_stack;
						item.item_stack = GetStackValue(DND_ITEM_ORB);
					}
					SyncItemStack(pnum, j, DND_SYNC_ITEMSOURCE_STASH | (i << 16));
				}
			}
		}
	}

	GiveInventory("DnD_RefreshPane", 1);
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
		case DND_ORB_WINTER:
		case DND_ORB_VOLTAIC:
		case DND_ORB_VILE:
		case DND_ORB_EMBERS:
		case DND_ORB_ELEVATION:
		case DND_ORB_HOLLOW:
			RestoreItemAttribsFromUsedOrb(pnum);
			SetInventory("OrbResult", Player_MostRecent_Orb[pnum].orb_type - 1);
		break;
		case DND_ORB_SIN:
			if(RunLuckBasedChance(pnum, DND_ORB_SIN_REPENTCHANCE) || CheckInventory("DestinyUsed")) {
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
		case DND_ORB_ORDER:
			SetInventory("OrderStored", 0);
		break;
	}
	ResetMostRecentOrb(pnum);
}

// picks a random orb
int PickRandomOrb(bool doExceptionCheck = false) {
	int i = DND_ORB_ENHANCE;
	do {
		i = random(DND_ORB_ENHANCE, DND_MON_DROP_ORB_BEGIN - 1);
	} while(doExceptionCheck && IsOrbDropException(i));
	return i;
}

int GetAffluenceBonus() {
	return 1 << CheckInventory("AffluenceCounter");
}

void HandleOrbUseMessage(int orbtype, int val, int affluence, int overrideValue) {
	if(ConsolePlayerNumber() != PlayerNumber()) 
		return;
	switch(orbtype) {
		case DND_ORB_ENHANCE:
			if(val != 0x7FFFFFFF)
				Log(s:"\cj", l:"DND_ORBUSETEXT1A", s:" \cd", l:GetWeaponTag(val), s:"\cv ", l:"DND_ORBUSETEXT1B", s:" \cd", d:affluence, s:"%\c-!");
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
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT3", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT3_ORD", s:"\cj!");
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
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT7", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT7_ORD", s:"\cj!");
		break;
		case DND_ORB_FORTITUDE:
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT8", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT8_ORD", s:"\cj!");
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
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT10", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT10_ORD", s:"\cj!");
		break;
		case DND_ORB_VIOLENCE:
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT11", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT11_ORD", s:"\cj!");
		break;
		case DND_ORB_SIN:
			if(val != 0x7FFFFFFF)
				Log(s:"\cg", l:"DND_ORBUSETEXT12", s:"\c-!");
			else
				Log(s:"\cg", l:"DND_ORBUSEFAIL12", s:"\c-!");
		break;
		case DND_ORB_TREMORS:
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT13", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT13_ORD", s:"\cj!");
		break;
		case DND_ORB_TINKERER:
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT14", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT14_ORD", s:"\cj!");
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
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT22", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT22_ORD", s:"\cj!");
		break;
		case DND_ORB_GROWTH:
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT23", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT23_ORD", s:"\cj!");
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
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT25", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT25_ORD", s:"\cj!");
		break;
		case DND_ORB_BRUTE:
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT26", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT26_ORD", s:"\cj!");
		break;
		case DND_ORB_JAGGED:
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT27", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT27_ORD", s:"\cj!");
		break;
		case DND_ORB_ALCHEMIST:
			if(overrideValue)
				affluence = overrideValue;
			Log(s:"\cj", l:"DND_ORBUSETEXT28", s:"\cd ", d:affluence, s:"%", s:"\cj!");
		break;
		case DND_ORB_EVOKER:
			if(affluence > 1)
				Log(s:"\cj", l:"DND_ORBUSETEXT29", s:"\cd ", d:affluence, s:" ", l:"DND_ORBUSETEXT29BS", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT29", s:"\cd ", d:affluence, s:" ", l:"DND_ORBUSETEXT29B", s:"\cj!");
		break;
		case DND_ORB_SAVAGERY:
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT30", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT30_ORD", s:"\cj!");
		break;
		case DND_ORB_WINTER:
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT31", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT31_ORD", s:"\cj!");
		break;
		case DND_ORB_VOLTAIC:
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT32", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT32_ORD", s:"\cj!");
		break;
		case DND_ORB_VILE:
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT33", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT33_ORD", s:"\cj!");
		break;
		case DND_ORB_EMBERS:
			if(!CheckInventory("OrderStored"))
				Log(s:"\cj", l:"DND_ORBUSETEXT34", s:"\cj!");
			else
				Log(s:"\cj", l:"DND_ORBUSETEXT34_ORD", s:"\cj!");
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
		case DND_ORB_ORDER:
			Log(s:"\cj", l:"DND_ORBUSETEXT35");
		break;
		case DND_ORB_DESTINY:
			Log(s:"\cj", l:"DND_ORBUSETEXT36");
		break;
		case DND_ORB_REVERANCE:
			Log(s:"\cj", l:"DND_ORBUSETEXT37");
		break;
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

int GetOrbDropStack(int monster_level) {
	// later make stacks larger on more common orbs instead using their weights
	monster_level = random(0, monster_level / DND_MONSTER_ORBSTACK_LEVELTHRESHOLD);
	return 1 + monster_level / random(1, 3);
}

void SpawnOrb(int pnum, bool sound, bool noRepeat = false, int stack = 1) {
	int c = CreateItemSpot();
	if(c != -1) {
		int i;
		
		i = PickPlayerOrb(pnum);
		
		// c is the index on the field now
		//i = DND_ORB_POTENCY;
		RollOrbInfo(c, i, stack);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(GetInventoryName(i + ORBS_BEGIN), 24.0, 16, pnum + 1, c);
		if (sound)
			ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_ORB, i);
	}
}

void SpawnOrbForAll(int repeats, int stack = 1) {
	for(int j = 0; j < MAXPLAYERS; ++j) {
		if(PlayerInGame(j) && !PlayerIsSpectator(j)) {
#ifndef ISDEBUGBUILD
			if(PlayerActivities[j].loot_penalty)
				continue;
#endif
			for(int k = 0; k < repeats; ++k)
				SpawnOrb(j, false, false, stack);
		}
	}
}

void SpawnSpecificOrb(int pnum, int id, bool sound, bool noRepeat = false, int stack = 1) {
	int c = CreateItemSpot();
	if(c != -1) {
		RollOrbInfo(c, id, stack);
		SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
		SpawnDrop(GetInventoryName(id + ORBS_BEGIN), 24.0, 16, pnum + 1, c);
		if(sound)
			ACS_NamedExecuteAlways("DnD Play Local Item Drop Sound", 0, pnum, DND_ITEM_ORB, id);
			
		//if(!noRepeat && HasActorMasteredPerk(pnum + P_TIDSTART, X) && random(0, 1.0) <= DND_MASTERY_LUCKCHANCE)
		//	SpawnSpecificOrb(pnum, id, sound, true, stack);
	}
}

void SpawnSpecificOrbForAll(int id, int repeats, int stack = 1) {
	for(int k = 0; k < repeats; ++k) {
		for(int j = 0; j < MAXPLAYERS; ++j) {
			if(PlayerInGame(j) && !PlayerIsSpectator(j))
				SpawnSpecificOrb(j, id, false, false, stack);
		}
	}
}

int GetOrbItemImage(int orbtype) {
	// monster drop only ones have different image range
	if(orbtype < DND_ORB_HOLLOW)
		return ITEM_IMAGE_ORB_BEGIN + orbtype;
	return ITEM_IMAGE_MONSTERORB_BEGIN + orbtype - DND_MON_DROP_ORB_BEGIN;
}

void RollOrbInfo(int item_pos, int orbtype, int stack = 1) {
	// roll random attributes for the charm
	auto item = GetFieldItem(item_pos);
	item.item_level = 1;
	item.item_stack = stack; // orbs have default stack of 1
	item.item_type = DND_ITEM_ORB;
	item.item_subtype = orbtype;
	item.width = 1;
	item.height = 1;
	item.attrib_count = 0;

	item.item_image = GetOrbItemImage(orbtype);
}

Script "DnD Give Orb Delayed" (int type, int amt) {
	Delay(1);
	GiveOrbToPlayer(PlayerNumber(), type, amt);
	GiveInventory("DnD_RefreshPane", 1);
}

Script "DND Orb Use Message" (int type, int result, int affluence, int overrideValue) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;
	HandleOrbUseMessage(type, result, affluence, overrideValue);
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

typedef struct orb_craft_result {
	int count;
	int effect_type;
	int id_list[MAX_ITEM_ATTRIBUTES];
} orb_craft_result_T;

orb_craft_result_T hovered_orb_craft_result;

// positive values indicate tags
void GetOrbAffectedIds(int orb_type, int pnum, int item_pos, int source) {
	hovered_orb_craft_result.count = 0;

	int affluence = GetAffluenceBonus();

	int i, j, temp;
	switch(orb_type) {
		case DND_ORB_PRISMATIC:
		case DND_ORB_DESTRUCTION:
		case DND_ORB_VIOLENCE:
		case DND_ORB_FORTITUDE:
		case DND_ORB_PROSPERITY:
		case DND_ORB_TINKERER:
		case DND_ORB_TREMORS:
		case DND_ORB_HEXES:
		case DND_ORB_GROWTH:
		case DND_ORB_CRACKLING:
		case DND_ORB_BRUTE:
		case DND_ORB_JAGGED:
		case DND_ORB_SAVAGERY:
		case DND_ORB_WINTER:
		case DND_ORB_VOLTAIC:
		case DND_ORB_VILE:
		case DND_ORB_EMBERS:
			// if order isn't used, then we mark everything as its a direct reforge
			if(!CheckInventory("OrderUsed")) {
				hovered_orb_craft_result.count = GlobalItemStorage.PlayerInventoryList[pnum][item_pos].attrib_count;
				for(i = 0; i < hovered_orb_craft_result.count; ++i)
					hovered_orb_craft_result.id_list[i] = i;
				hovered_orb_craft_result.effect_type = DND_ORBEFFECT_WHOLE;
			}
		break;

		// other orbs that do a certain effect based on conditions including order orb
		case DND_ORB_TURMOIL:
			auto item = GetPlayerInventoryItem(pnum, item_pos);
			for(i = 0; i < item.attrib_count; ++i) {
				if(item.attributes[i].fractured || !CheckOrderOrb(item.attributes[i].attrib_id))
					continue;
				hovered_orb_craft_result.id_list[hovered_orb_craft_result.count++] = i;
			}
			hovered_orb_craft_result.effect_type = DND_ORBEFFECT_WHOLE;
		break;

		case DND_ORB_SCULPTING:
			item = GetPlayerInventoryItem(pnum, item_pos);
			for(i = 0; i < item.attrib_count; ++i) {
				if(item.attributes[i].fractured || !CheckOrderOrb(item.attributes[i].attrib_id))
					continue;
				hovered_orb_craft_result.id_list[hovered_orb_craft_result.count++] = i;
			}
			hovered_orb_craft_result.effect_type = DND_ORBEFFECT_WHOLE;
		break;

		case DND_ORB_ELEVATION:
		case DND_ORB_HOLLOW:
			hovered_orb_craft_result.effect_type = DND_ORBEFFECT_NEWATTRIBUTE;
		break;

		case DND_ORB_REFINEMENT:
		case DND_ORB_SIN:
		case DND_ORB_POTENCY:
			item = GetPlayerInventoryItem(pnum, item_pos);
			for(i = 0; i < item.attrib_count; ++i) {
				if(item.attributes[i].fractured || !CheckOrderOrb(item.attributes[i].attrib_id))
					continue;
				hovered_orb_craft_result.id_list[hovered_orb_craft_result.count++] = i;
			}
			hovered_orb_craft_result.effect_type = DND_ORBEFFECT_NUMBER;
		break;

		case DND_ORB_NULLIFICATION:
			// find the attribute with the lowest tier, in case of multiple, return a random one
			hovered_orb_craft_result.count = 0;
			temp = MAX_CHARM_AFFIXTIERS;
			item = GetPlayerInventoryItem(pnum, item_pos);
			for(i = 0; i < item.attrib_count; ++i) {
				// ignore the fractured mods
				if(item.attributes[i].fractured || !CheckOrderOrb(item.attributes[i].attrib_id))
					continue;

				if(item.attributes[i].attrib_tier < temp) {
					// we use count to hold the count of elements in this temporary array
					// reset current count if we found a new minimum, then add it to our array

					hovered_orb_craft_result.count = 0;
					hovered_orb_craft_result.id_list[hovered_orb_craft_result.count++] = i;
					temp = item.attributes[i].attrib_tier;
				}
				else if(item.attributes[i].attrib_tier == temp) // if equal to current min, store it
					hovered_orb_craft_result.id_list[hovered_orb_craft_result.count++] = i;
			}
			hovered_orb_craft_result.effect_type = DND_ORBEFFECT_WHOLE;
		break;

		case DND_ORB_CORRUPT:
		case DND_ORB_EVOKER:
		case DND_ORB_ASSIMILATION:
			hovered_orb_craft_result.effect_type = DND_ORBEFFECT_ENTIREITEM;
		break;
	}
}

#endif
