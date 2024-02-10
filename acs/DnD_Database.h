#ifndef DND_DATABASE_IN
#define DND_DATABASE_IN

#include "DnD_DatabaseRows.h"
#include "DnD_Common.h"
#include "DnD_MenuConstants.h"
#include "DnD_WeaponDefs.h"
#include "DnD_Research.h"
#include "DnD_Stat.h"
#include "DnD_Orbs.h"
#include "DnD_ClassMenu.h"

#define DND_MAX_CHARS 5

enum {
	DND_LOGIN_NOCHAR,
	DND_LOGIN_CREATECHAROK,
	DND_LOGIN_CHARINUSE,
	DND_LOGIN_NOCHARINUSE,
	DND_LOGIN_NOTINGAME,
	DND_LOGIN_NOTHARDCORE,
	DND_LOGIN_INCOUNTDOWN,
	DND_LOGIN_NOTLOGGED,
	DND_LOGIN_NOTIME,
	DND_LOGIN_CHARNOTINRANGE,
	DND_LOGIN_TRANSFSAMESLOT,
	DND_CHARLOADED,
	DND_LOGIN_CHARTRANSFERRED,
};

enum {
	DND_PINVFLAGS_CHARMUSED = 1,
	DND_PINVFLAGS_INVENTORY = 2,
	DND_PINVFLAGS_STASH = 4,
	DND_PINVFLAGS_ALL = 7
};

// this is a wrapper for BeginDBTransaction, we won't call it again if there's a big on-going one already -- fixes console errors
void StartDBTransaction() {
	if(!TransactionMade) {
		TransactionMade = true;
		BeginDBTransaction();
	}
}

void FinishDBTransaction() {
	EndDBTransaction();
	TransactionMade = false;
}

void RestoreResearchItems() {
	if(CheckResearchStatus(RES_DOUBLESPECIALCAP) == RES_DONE)
		DoubleSpecialAmmoCapacity();
}

str GetCharField(str fname, int char_id) {
	return StrParam(s:"C", d:char_id, s:"_", s:fname);
}

void SavePlayerInventoryStuff(int pnum, int char_id, str pacc, int flags) {
	int i, j, temp, idx;
	// Inventory related stuff
	// save inventory state of player
	// prep the loaded id checker
	if(flags & DND_PINVFLAGS_INVENTORY) {
		for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
			// we got the main item pointer here
			idx = PlayerInventoryList[pnum][i].topleftboxid - 1;
			// log(s:"i: ", d:i, s:" idx: ", d:idx, s: " null? ", d:PlayerInventoryList[pnum][idx].item_type, s: " =? ", d:DND_ITEM_NULL);
			if(idx == i) {
				// log(s:"save on ", d:i);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), char_id), pacc, PlayerInventoryList[pnum][i].width);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), char_id), pacc, PlayerInventoryList[pnum][i].height);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), char_id), pacc, PlayerInventoryList[pnum][i].item_image);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), char_id), pacc, PlayerInventoryList[pnum][i].item_type);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), char_id), pacc, PlayerInventoryList[pnum][i].item_subtype);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), char_id), pacc, PlayerInventoryList[pnum][i].item_level);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_STACK), char_id), pacc, PlayerInventoryList[pnum][i].item_stack);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), char_id), pacc, PlayerInventoryList[pnum][i].attrib_count);

				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), char_id), pacc, PlayerInventoryList[pnum][i].corrupted);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), char_id), pacc, PlayerInventoryList[pnum][i].quality);
				
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), char_id), pacc, PlayerInventoryList[pnum][i].implicit.attrib_id);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), char_id), pacc, PlayerInventoryList[pnum][i].implicit.attrib_val);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), char_id), pacc, PlayerInventoryList[pnum][i].implicit.attrib_tier);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), char_id), pacc, PlayerInventoryList[pnum][i].implicit.attrib_extra);

				for(j = 0; j < PlayerInventoryList[pnum][i].attrib_count; ++j) {
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:j), char_id), pacc, PlayerInventoryList[pnum][i].attributes[j].attrib_val);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:j), char_id), pacc, PlayerInventoryList[pnum][i].attributes[j].attrib_id);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:j), char_id), pacc, PlayerInventoryList[pnum][i].attributes[j].attrib_tier);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:j), char_id), pacc, PlayerInventoryList[pnum][i].attributes[j].attrib_extra);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:j), char_id), pacc, PlayerInventoryList[pnum][i].attributes[j].fractured);
				}
			}
			else {
				// log(s:"delete on i: ", d:i);
				// clean whatever state existed here previously if we are null
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), char_id), pacc, 0);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), char_id), pacc, 0);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), char_id), pacc, 0);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), char_id), pacc, DND_ITEM_NULL);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), char_id), pacc, 0);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), char_id), pacc, 0);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_STACK), char_id), pacc, 0);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), char_id), pacc, 0);

				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), char_id), pacc, 0);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), char_id), pacc, 0);

				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), char_id), pacc, 0);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), char_id), pacc, 0);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), char_id), pacc, 0);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), char_id), pacc, 0);

				for(j = 0; j < PlayerInventoryList[pnum][i].attrib_count; ++j) {
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:j), char_id), pacc, 0);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:j), char_id), pacc, 0);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:j), char_id), pacc, 0);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:j), char_id), pacc, 0);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:j), char_id), pacc, 0);
				}
			}
		}
	}
	
	// used charms
	if(flags & DND_PINVFLAGS_CHARMUSED){
		// save charms used
		for(i = 0; i < MAX_CHARMS_EQUIPPABLE; ++i) {
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), char_id), pacc, Charms_Used[pnum][i].width);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), char_id), pacc, Charms_Used[pnum][i].height);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), char_id), pacc, Charms_Used[pnum][i].item_image);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), char_id), pacc, Charms_Used[pnum][i].item_type);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), char_id), pacc, Charms_Used[pnum][i].item_subtype);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), char_id), pacc, Charms_Used[pnum][i].item_level);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_STACK), char_id), pacc, Charms_Used[pnum][i].item_stack);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), char_id), pacc, Charms_Used[pnum][i].attrib_count);
			
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), char_id), pacc, Charms_Used[pnum][i].corrupted);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), char_id), pacc, Charms_Used[pnum][i].quality);
			
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), char_id), pacc, Charms_Used[pnum][i].implicit.attrib_id);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), char_id), pacc, Charms_Used[pnum][i].implicit.attrib_val);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), char_id), pacc, Charms_Used[pnum][i].implicit.attrib_tier);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), char_id), pacc, Charms_Used[pnum][i].implicit.attrib_extra);

			
			for(j = 0; j < Charms_Used[pnum][i].attrib_count; ++j) {
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:j), char_id), pacc, Charms_Used[pnum][i].attributes[j].attrib_val);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:j), char_id), pacc, Charms_Used[pnum][i].attributes[j].attrib_id);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:j), char_id), pacc, Charms_Used[pnum][i].attributes[j].attrib_tier);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:j), char_id), pacc, Charms_Used[pnum][i].attributes[j].attrib_extra);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:j), char_id), pacc, Charms_Used[pnum][i].attributes[j].fractured);
			}
		}
	}
	
	if(flags & DND_PINVFLAGS_STASH) {
		// save state of player's stash (this is shared between all accounts so no charfield)
		SetDBEntry(DND_DB_STASH_PAGES, pacc, PlayerActivities[pnum].stash_pages);
		for(i = 0; i < PlayerActivities[pnum].stash_pages; ++i) {
			for(j = 0; j < MAX_INVENTORY_BOXES; ++j) {
				idx = PlayerStashList[pnum][i][j].topleftboxid - 1;
				if(idx == j) {
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), pacc, PlayerStashList[pnum][i][j].width);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), pacc, PlayerStashList[pnum][i][j].height);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), pacc, PlayerStashList[pnum][i][j].item_image);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), pacc, PlayerStashList[pnum][i][j].item_type);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), pacc, PlayerStashList[pnum][i][j].item_subtype);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), pacc, PlayerStashList[pnum][i][j].item_level);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_STACK), pacc, PlayerStashList[pnum][i][j].item_stack);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), pacc, PlayerStashList[pnum][i][j].attrib_count);
					
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), pacc, PlayerStashList[pnum][i][j].corrupted);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), pacc, PlayerStashList[pnum][i][j].quality);

					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), pacc, PlayerStashList[pnum][i][j].implicit.attrib_val);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), pacc, PlayerStashList[pnum][i][j].implicit.attrib_id);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), pacc, PlayerStashList[pnum][i][j].implicit.attrib_tier);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), pacc, PlayerStashList[pnum][i][j].implicit.attrib_extra);

					for(temp = 0; temp < PlayerStashList[pnum][i][j].attrib_count; ++temp) {
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:temp), pacc, PlayerStashList[pnum][i][j].attributes[temp].attrib_val);
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:temp), pacc, PlayerStashList[pnum][i][j].attributes[temp].attrib_id);
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:temp), pacc, PlayerStashList[pnum][i][j].attributes[temp].attrib_tier);
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:temp), pacc, PlayerStashList[pnum][i][j].attributes[temp].attrib_extra);
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:temp), pacc, PlayerStashList[pnum][i][j].attributes[temp].fractured);
					}
				}
				else {
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), pacc, 0);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), pacc, 0);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), pacc, 0);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), pacc, DND_ITEM_NULL);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), pacc, 0);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), pacc, 0);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_STACK), pacc, 0);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), pacc, 0);

					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), pacc, 0);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), pacc, 0);

					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), pacc, 0);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), pacc, 0);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), pacc, 0);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), pacc, 0);


					for(temp = 0; temp < PlayerStashList[pnum][i][j].attrib_count; ++temp) {
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:temp), pacc, 0);
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:temp), pacc, 0);
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:temp), pacc, 0);
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:temp), pacc, 0);
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:temp), pacc, 0);
					}
				}
			}
		}
	}
}

void SavePlayerItem(int pnum, int char_id, int itemid, int source) {
	if(HardcoreSet && (GetCVar("dnd_mode") >= DND_MODE_SOFTCORE) && PlayerIsLoggedIn(pnum)) {
		StartDBTransaction();
		int tid = pnum + P_TIDSTART, i, j, k, temp;
		char_id = Clamp_Between(char_id, 0, DND_MAX_CHARS - 1); // DnD_CharacterID defaults to 1 if no cmds are used.
		str pacc = GetPlayerAccountName(pnum);
		
		int w = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, itemid, -1, source);
		int h = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, itemid, -1, source);
		int page = source >> 16;
		source &= 0xFFFF;
		if(source == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) {
			for(i = 0; i < w; ++i) {
				for(j = 0; j < h; ++j) {
					temp = itemid + i + j * MAXINVENTORYBLOCKS_VERT;
					// save item stuff
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), char_id), pacc, PlayerInventoryList[pnum][itemid].width);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), char_id), pacc, PlayerInventoryList[pnum][itemid].height);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), char_id), pacc, PlayerInventoryList[pnum][itemid].item_image);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), char_id), pacc, PlayerInventoryList[pnum][itemid].item_type);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), char_id), pacc, PlayerInventoryList[pnum][itemid].item_subtype);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), char_id), pacc, PlayerInventoryList[pnum][itemid].item_level);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_STACK), char_id), pacc, PlayerInventoryList[pnum][itemid].item_stack);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), char_id), pacc, PlayerInventoryList[pnum][itemid].attrib_count);
					
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), char_id), pacc, PlayerInventoryList[pnum][itemid].corrupted);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), char_id), pacc, PlayerInventoryList[pnum][itemid].quality);
					
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), char_id), pacc, PlayerInventoryList[pnum][itemid].implicit.attrib_id);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), char_id), pacc, PlayerInventoryList[pnum][itemid].implicit.attrib_val);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), char_id), pacc, PlayerInventoryList[pnum][itemid].implicit.attrib_tier);
					SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), char_id), pacc, PlayerInventoryList[pnum][itemid].implicit.attrib_extra);

					for(k = 0; k < PlayerInventoryList[pnum][itemid].attrib_count; ++k) {
						SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:k), char_id), pacc, PlayerInventoryList[pnum][itemid].attributes[k].attrib_val);
						SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:k), char_id), pacc, PlayerInventoryList[pnum][itemid].attributes[k].attrib_id);
						SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:k), char_id), pacc, PlayerInventoryList[pnum][itemid].attributes[k].attrib_tier);
						SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:k), char_id), pacc, PlayerInventoryList[pnum][itemid].attributes[k].attrib_extra);
						SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:temp, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:k), char_id), pacc, PlayerInventoryList[pnum][itemid].attributes[k].fractured);
					}
				}
			}
		}
		else if(source == DND_SYNC_ITEMSOURCE_STASH) {
			for(i = 0; i < w; ++i) {
				for(j = 0; j < h; ++j) {
					temp = itemid + i + j * MAXINVENTORYBLOCKS_VERT;
					// save item stuff
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), pacc, PlayerStashList[pnum][page][temp].width);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), pacc, PlayerStashList[pnum][page][temp].height);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), pacc, PlayerStashList[pnum][page][temp].item_image);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), pacc, PlayerStashList[pnum][page][temp].item_type);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), pacc, PlayerStashList[pnum][page][temp].item_subtype);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), pacc, PlayerStashList[pnum][page][temp].item_level);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_STACK), pacc, PlayerStashList[pnum][page][temp].item_stack);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), pacc, PlayerStashList[pnum][page][temp].attrib_count);
					
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), pacc, PlayerStashList[pnum][page][temp].corrupted);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), pacc, PlayerStashList[pnum][page][temp].quality);
					
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), pacc, PlayerStashList[pnum][page][temp].implicit.attrib_id);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), pacc, PlayerStashList[pnum][page][temp].implicit.attrib_val);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), pacc, PlayerStashList[pnum][page][temp].implicit.attrib_tier);
					SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), pacc, PlayerStashList[pnum][page][temp].implicit.attrib_extra);


					for(k = 0; k < PlayerStashList[pnum][page][temp].attrib_count; ++k) {
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:k), pacc, PlayerStashList[pnum][page][temp].attributes[k].attrib_val);
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:k), pacc, PlayerStashList[pnum][page][temp].attributes[k].attrib_id);
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:k), pacc, PlayerStashList[pnum][page][temp].attributes[k].attrib_tier);
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:k), pacc, PlayerStashList[pnum][page][temp].attributes[k].attrib_extra);
						SetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:page, s:"_", d:temp, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:k), pacc, PlayerStashList[pnum][page][temp].attributes[k].fractured);
					}
				}
			}
		}
		else {
			// charms used
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), char_id), pacc, Charms_Used[pnum][itemid].width);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), char_id), pacc, Charms_Used[pnum][itemid].height);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), char_id), pacc, Charms_Used[pnum][itemid].item_image);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), char_id), pacc, Charms_Used[pnum][itemid].item_type);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), char_id), pacc, Charms_Used[pnum][itemid].item_subtype);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), char_id), pacc, Charms_Used[pnum][itemid].item_level);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_STACK), char_id), pacc, Charms_Used[pnum][itemid].item_stack);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), char_id), pacc, Charms_Used[pnum][itemid].attrib_count);
			
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), char_id), pacc, Charms_Used[pnum][itemid].corrupted);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), char_id), pacc, Charms_Used[pnum][itemid].quality);

			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), char_id), pacc, Charms_Used[pnum][itemid].implicit.attrib_id);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), char_id), pacc, Charms_Used[pnum][itemid].implicit.attrib_val);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), char_id), pacc, Charms_Used[pnum][itemid].implicit.attrib_tier);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), char_id), pacc, Charms_Used[pnum][itemid].implicit.attrib_extra);

			for(j = 0; j < Charms_Used[pnum][itemid].attrib_count; ++j) {
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:j), char_id), pacc, Charms_Used[pnum][itemid].attributes[j].attrib_val);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:j), char_id), pacc, Charms_Used[pnum][itemid].attributes[j].attrib_id);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:j), char_id), pacc, Charms_Used[pnum][itemid].attributes[j].attrib_tier);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:j), char_id), pacc, Charms_Used[pnum][itemid].attributes[j].attrib_extra);
				SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:itemid, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:j), char_id), pacc, Charms_Used[pnum][itemid].attributes[j].fractured);
			}
		}
	}
}

void SavePlayerData(int pnum, int char_id) {
	int temp, tid = pnum + P_TIDSTART, i, j;
	char_id = Clamp_Between(char_id, 0, DND_MAX_CHARS - 1); // DnD_CharacterID defaults to 1 if no cmds are used.
	str pacc = GetPlayerAccountName(pnum);
	
	// save class id -- the character class they use!
	SetDBEntry(GetCharField(DND_DB_CLASSID, char_id), pacc, CheckActorInventory(tid, "DnD_Character"));
	
	// save weapons
	for(i = 0; i < MAXWEPS; ++i) {
		j = i / 32;
		if(CheckActorInventory(tid, Weapons_Data[i].name)) {
			temp = SetBit(temp, i - 32 * j);
		}
		if(j != (i + 1) / 32 || i == MAXWEPS - 1) { // ie. our j changed or we hit maxweps
			// send temp over
			SetDBEntry(GetCharField(StrParam(s:DND_DB_WEAPONINT, d:j + 1), char_id), pacc, temp);
			temp = 0;
		}
	}
	
	for(i = 0; i < MAXTEMPWEPS; ++i)
		SetDBEntry(GetCharField(StrParam(s:DND_DB_TEMPAMMO, d:i), char_id), pacc, CheckActorInventory(tid, TemporaryWeaponData[i][TEMPWEP_AMMO]));
	SetDBEntry(GetCharField(DND_DB_TEMPAMMO_BLADECHARGE, char_id), pacc, CheckActorInventory(tid, "BladeCharge"));
	
	// save stats of player (can fit 4 stats in one int, using 7 bits)
	temp = CheckActorInventory(tid, "PSTAT_Strength");
	temp |= (0xFF & CheckActorInventory(tid, "PSTAT_Dexterity")) << 8;
	temp |= (0xFF & CheckActorInventory(tid, "PSTAT_Bulkiness")) << 16;
	temp |= (0xFF & CheckActorInventory(tid, "PSTAT_Charisma")) << 24;
	// send temp over
	SetDBEntry(GetCharField(DND_DB_STATS_1, char_id), pacc, temp);
	
	// save vitality, backpack count and active accessory
	temp = CheckActorInventory(tid, "PSTAT_Vitality");
	// backpack covers 6 bits max (count = 55 < 63)
	temp |= CheckActorInventory(tid, "BackpackCounter") << 8; // for backpack
	// send temp over
	SetDBEntry(GetCharField(DND_DB_STATS_2, char_id), pacc, temp);
	
	// save intellect
	temp = CheckActorInventory(tid, "PSTAT_Intellect");

	SetDBEntry(GetCharField(DND_DB_STATS_3, char_id), pacc, temp);
	
	// save perks (use 4 bits per perk, max is 10 for a perk)
	temp = CheckActorInventory(tid, "Perk_Sharpshooting");
	temp |= (0xF & CheckActorInventory(tid, "Perk_Brutality")) << 4;
	temp |= (0xF & CheckActorInventory(tid, "Perk_Endurance")) << 8;
	temp |= (0xF & CheckActorInventory(tid, "Perk_Wisdom")) << 12;
	temp |= (0xF & CheckActorInventory(tid, "Perk_Greed")) << 16;
	temp |= (0xF & CheckActorInventory(tid, "Perk_Medic")) << 20;
	temp |= (0xF & CheckActorInventory(tid, "Perk_Munitionist")) << 24;
	temp |= (0xF & CheckActorInventory(tid, "Perk_Deadliness")) << 28;
	// send temp over
	SetDBEntry(GetCharField(DND_DB_PERKS, char_id), pacc, temp);
	
	// save perks - 2
	temp = CheckActorInventory(tid, "Perk_Savagery");
	temp |= (0xF & CheckActorInventory(tid, "Perk_Luck")) << 4;
	SetDBEntry(GetCharField(DND_DB_PERKS2, char_id), pacc, temp);
	
	// save health and armor
	temp = GetActorProperty(tid, APROP_HEALTH);
	temp <<= 16;
	temp |= GetActorArmorAmount(tid);
	SetDBEntry(GetCharField(DND_DB_HEALTHARMOR, char_id), pacc, temp);
	
	// save armor type
	temp = 0;
	for(i = 0; i < MAXARMORS; ++i) {
		if(GetActorArmorID(tid) == i) {
			temp = SetBit(temp, i);
			break;
		}
	}
	// send temp over
	SetDBEntry(GetCharField(DND_DB_ARMORTYPE, char_id), pacc, temp);
	
	// save ammo counts
	for(i = 0; i < MAX_SLOTS; ++i)
		for(j = 0; j < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[i][j].initial_capacity != -1; ++j) {
			temp = CheckActorInventory(tid, AmmoInfo_Str[i][j][AMMOINFO_NAME]);
			// send temp over
			SetDBEntry(StrParam(s:GetCharField(DND_DB_AMMO, char_id), s:AmmoInfo_Str[i][j][AMMOINFO_NAME]), pacc, temp);
		}
		
	for(i = 0; i < MAX_SPECIAL_AMMOS; ++i) {
		temp = CheckActorInventory(tid, SpecialAmmoInfo_Str[i][AMMOINFO_NAME]);
		// send temp over
		SetDBEntry(StrParam(s:GetCharField(DND_DB_SPECIALAMMO, char_id), s:SpecialAmmoInfo_Str[i][AMMOINFO_NAME]), pacc, temp);
	}
	
	// save weapon clips/magazines that use them
	for(i = 0; i < MAXCLIPAMMOTYPES; ++i) {
		temp = CheckActorInventory(tid, ClipAmmoTypes[i]);
		// send temp over
		SetDBEntry(StrParam(s:GetCharField(DND_DB_AMMO, char_id), s:ClipAmmoTypes[i]), pacc, temp);
	}
	
	// save exp, credit and level (when loading give expvisual, amt = exp % levelcurve[level])
	temp = CheckActorInventory(tid, "Exp");
	// send temp over
	SetDBEntry(GetCharField(DND_DB_EXP, char_id), pacc, temp);
	
	temp = CheckActorInventory(tid, "Credit");
	//send temp over
	SetDBEntry(GetCharField(DND_DB_CREDIT, char_id), pacc, temp);
	
	temp = CheckActorInventory(tid, "Level");
	if(!temp)
		temp = 1; // fix level 0 saves
	// send temp over
	SetDBEntry(GetCharField(DND_DB_LEVEL, char_id), pacc, temp);
	
	// save abilities
	temp = 0;
	for(i = 0; i < MAXABILITIES; ++i) {
		if(CheckActorInventory(tid, AbilityInfo[i]))
			temp = SetBit(temp, i);
	}
	// send temp over
	SetDBEntry(GetCharField(DND_DB_ABILITY, char_id), pacc, temp);
	
	// save unspent attribute/perk points
	temp = CheckActorInventory(tid, "AttributePoint");
	temp |= (0x7FF & CheckActorInventory(tid, "PerkPoint")) << 11;
	// send temp over
	SetDBEntry(GetCharField(DND_DB_ATTRIBUTEPERK, char_id), pacc, temp);
	
	// save accessories and artifacts
	temp = 0;
	for(i = 0; i < MAX_ACCESSORY; ++i) {
		if(CheckActorInventory(tid, StrParam(s:"Accessory_", d:i + 1)))
			temp = SetBit(temp, i);
	}
	// send temp over
	SetDBEntry(GetCharField(DND_DB_ACCESSORIES, char_id), pacc, temp);
	
	// save active accessories
	SetDBEntry(GetCharField(DND_DB_ACTIVEACCESSORIES, char_id), pacc, CheckActorInventory(tid, "Accessory_Index"));
	
	temp = 0;
	// fieldkits can have 3, assume 3 bits. (Maybe in future) Portable Shields hold 3 as well. Rest are all 1. Do not save automap.
	// Take fieldkit and portable shields last
	// this guy breaks the loop sequence so take him first
	temp |= CheckActorInventory(tid, "SalvationSphere");
	for(i = 3; i < MAXARTIFACTS - 3; ++i) {
		if(CheckActorInventory(tid, ArtifactInfo[i][ARTI_NAME]))
			temp = SetBit(temp, i - 2); // because 0th bit is for salvation sphere
	}
	// last 3 won't be saved and these two arent saved yet, so anything besides these are saved until here
	temp |= CheckActorInventory(tid, "FieldKit") << (MAXARTIFACTS - 5); 
	temp |= CheckActorInventory(tid, "PortableShield") << (MAXARTIFACTS - 3);
	temp |= CheckActorInventory(tid, "StatReset") << (MAXARTIFACTS - 1);
	// send temp over
	SetDBEntry(GetCharField(DND_DB_ARTIFACTS, char_id), pacc, temp);
	
	temp = 0;
	// save researches
	for(i = 0; i < RESEARCH_BITSETS; ++i) {
		SetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCH_DISCOVERED, char_id), d:i + 1), pacc, CheckActorInventory(tid, StrParam(s:"Research_Discovered_", d:i + 1)));
		SetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCH_DONE, char_id), d:i + 1), pacc, CheckActorInventory(tid, StrParam(s:"Research_Done_", d:i + 1)));
	}
	
	// save research investment
	for(i = 0; i < MAX_RESEARCHES; ++i)
		SetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCH_INVESTED, char_id), d:i + 1), pacc, ResearchInvestments[pnum][i]);
	
	// save research trackers
	for(i = 0; i < MAX_RESEARCH_TRACKERS; ++i)
		SetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCHTRACKER, char_id), d:i + 1), pacc, CheckActorInventory(tid, ResearchTrackers[i]));
	
	// save legendary monster kills
	SetDBEntry(StrParam(s:GetCharField(DND_DB_LEGENDARYTRACKER, char_id), d:i + 1), pacc, CheckActorInventory(tid, "LegendaryKills"));
	
	// save budget
	temp = CheckActorInventory(tid, "Budget");
	SetDBEntry(GetCharField(DND_DB_BUDGET, char_id), pacc, temp);
	
	// save quests
	temp = CheckActorInventory(tid, "DnD_QuestState1");
	SetDBEntry(GetCharField(DND_DB_QUESTSTATE1, char_id), pacc, temp);
	temp = CheckActorInventory(tid, "DnD_QuestState2");
	SetDBEntry(GetCharField(DND_DB_QUESTSTATE2, char_id), pacc, temp);
	
	// save player's lifetime
	temp = CheckActorInventory(tid, "RoundsSurvived");
	SetDBEntry(GetCharField(DND_DB_SURVIVECOUNT, char_id), pacc, temp);
	
	// save weapon checkers
	temp = 0;
	for(i = 0; i < MAX_WEPCONDITION_CHECKERS; ++i)
		if(CheckActorInventory(tid, WeaponConditionCheckers[i]))
			temp |= 1 << i;
	SetDBEntry(GetCharField(DND_DB_PLAYERWEPCHECKERS, char_id), pacc, temp);
	
	// save player's weapon mods
	temp = 0;
	for(i = 0; i < MAXWEPS; ++i) {
		// check mods
		// enhancement orbs used
		temp = Player_Weapon_Infos[pnum][i].quality;
		SetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONQUALITY, char_id), d:i), pacc, temp & 0xFF);

		// rest of the mods for the weapon
		for(j = 0; j < MAX_WEP_MODS; ++j) {
			temp = Player_Weapon_Infos[pnum][i].wep_mods[j].tier;
			SetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONMOD_TIER, char_id), d:j, s:"_Weapon", d:i), pacc, temp);
			temp = Player_Weapon_Infos[pnum][i].wep_mods[j].val;
			SetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONMOD_VAL, char_id), d:j, s:"_Weapon", d:i), pacc, temp);
		}
	}
	
	// save stored medkit
	SetDBEntry(GetCharField(DND_DB_STOREDKIT, char_id), pacc, CheckActorInventory(tid, "StoredMedkit"));
	
	SetDBEntry(GetCharField(DND_DB_NPCTRACKER, char_id), pacc, CheckActorInventory(tid, "DnD_NPC_Meet"));
	
	SavePlayerInventoryStuff(pnum, char_id, pacc, DND_PINVFLAGS_ALL);
	
	SetDBEntry(GetCharField(DND_DB_KILLTRACKER, char_id), pacc, CheckActorInventory(pnum + P_TIDSTART, "DnD_LifeTimeKills"));
	SetDBEntry(GetCharField(DND_DB_KILLTRACKER_MILLION, char_id), pacc, CheckActorInventory(pnum + P_TIDSTART, "DnD_LifeTimeKills_Millions"));
	SetDBEntry(GetCharField(DND_DB_KILLTRACKER_BILLION, char_id), pacc, CheckActorInventory(pnum + P_TIDSTART, "DnD_LifeTimeKills_Billions"));
	
	#ifdef ISAPRILFIRST
		// save nft
		SetDBEntry(GetCharField(DND_DB_NFT, char_id), pacc, CheckActorInventory(tid, "NFT_Token"));
	#endif
}

// This uses a method completely off grid of inventory data --- eventually move on to this completely!!!
void SavePlayerActivities(int pnum, int char_id) {
	int i, j, vt;
	char_id = Clamp_Between(char_id, 0, DND_MAX_CHARS - 1);
	int temp; //DnD_CharacterID defaults to 1 if no cmds are used.
	str pacc = RecoverPlayerAccountName(pnum);
	//Log(s:"player ", d:pnum, s: " account name ", s:pacc);
	
	// save stats of player (can fit 4 stats in one int, using 7 bits)
	temp = 0;
	temp = PlayerActivities[pnum].attribute_change[STAT_STR];
	
	vt = PlayerActivities[pnum].attribute_change[STAT_DEX];
	if(vt < 0)
		temp -= (-vt) << 8;
	else
		temp += vt << 8;
	
	vt = PlayerActivities[pnum].attribute_change[STAT_BUL];
	if(vt < 0)
		temp -= (-vt) << 16;
	else
		temp += vt << 16;
		
	vt = PlayerActivities[pnum].attribute_change[STAT_CHR];
	if(vt < 0)
		temp -= (-vt) << 24;
	else
		temp += vt << 24;
	// send temp over
	IncrementDBEntry(GetCharField(DND_DB_STATS_1, char_id), pacc, temp);
	
	// save vitality
	temp = PlayerActivities[pnum].attribute_change[STAT_VIT];
	// send temp over -- we increment this so the backpack count should not be affected
	IncrementDBEntry(GetCharField(DND_DB_STATS_2, char_id), pacc, temp);
	
	// save intellect
	temp = PlayerActivities[pnum].attribute_change[STAT_INT];
	IncrementDBEntry(GetCharField(DND_DB_STATS_3, char_id), pacc, temp);
	
	// save perks (use 4 bits per perk, max is 10 for a perk so it saves the first 8 perks packed into 4 bits)
	i = STAT_SHRP - DND_PERK_BEGIN;
	temp = PlayerActivities[pnum].perk_change[i];
	//printbold(s:"perk ", d:i, s: " change: ", d:temp);
	for(j = 0; j < 7; ++j) {
		vt = PlayerActivities[pnum].perk_change[++i];
		//printbold(s:"perk ", d:i, s: " change: ", d:vt);
		if(vt < 0)
			temp -= (-vt) << ((j + 1) * 4);
		else
			temp += vt << ((j + 1) * 4);
	}
	// send temp over
	IncrementDBEntry(GetCharField(DND_DB_PERKS, char_id), pacc, temp);
	
	// save perks - 2 -- send the leftovers, as long as we have <= 16 perks we're fine here
	// -1 is because we always set the initial value to the previous one we left at, so the first batch saved 8 perks and is waiting at 9th
	// 9th and 10th need to be saved, our initial is 9 we need 10th saved so max perks - 9 = 1, loop will run once for 10 perks
	temp = PlayerActivities[pnum].perk_change[++i];
	//printbold(s:"2nd loop perk ", d:i, s: " change: ", d:temp);
	for(j = 0; j < DND_MAX_PERKS - 9; ++j) {
		vt = PlayerActivities[pnum].perk_change[++i];
		//printbold(s:"perk ", d:i, s: " change: ", d:vt);
		if(vt < 0)
			temp -= (-vt) << ((j + 1) * 4);
		else
			temp += vt << ((j + 1) * 4);
	}
	IncrementDBEntry(GetCharField(DND_DB_PERKS2, char_id), pacc, temp);
	
	for(i = 0; i < MAXWEPS; ++i) {
		// check mods
		// enhancement orbs used
		// orb bonus
		//IncrementDBEntry(StrParam(s:GetCharField(DND_DB_ORBWEAPONQUALITY, char_id), d:i), pacc, PlayerActivities[pnum].orb_change.weapon_stat_bonuses[i].quality);

		// rest of the mods for the weapon
		for(j = 0; j < MAX_WEP_MODS; ++j) {
			temp = Player_Weapon_Infos[pnum][i].wep_mods[j].tier;
			SetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONMOD_TIER, char_id), d:j, s:"_Weapon", d:i), pacc, temp);
			temp = Player_Weapon_Infos[pnum][i].wep_mods[j].val;
			SetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONMOD_VAL, char_id), d:j, s:"_Weapon", d:i), pacc, temp);
		}
	}
	
	temp = PlayerActivities[pnum].credit;
	Log(s:"player ", d:pnum, s: " change on credit: ", d:PlayerActivities[pnum].credit);
	//send temp over
	IncrementDBEntry(GetCharField(DND_DB_CREDIT, char_id), pacc, temp);
	
	temp = PlayerActivities[pnum].level;
	// send temp over
	IncrementDBEntry(GetCharField(DND_DB_LEVEL, char_id), pacc, temp);
	
	// save budget
	temp = PlayerActivities[pnum].budget;
	IncrementDBEntry(GetCharField(DND_DB_BUDGET, char_id), pacc, temp);
	
	// save exp
	temp = PlayerActivities[pnum].exp;
	SetDBEntry(GetCharField(DND_DB_EXP, char_id), pacc, temp);
	
	// save unspent attribute/perk points
	temp = PlayerActivities[pnum].free_attributes;
	vt = PlayerActivities[pnum].free_perks;
	//printbold(s:"free attrib perk ", d:temp, s: " ", d:vt);
	if(vt < 0)
		temp -= (-vt) << 11;
	else
		temp += vt << 11;
	// send temp over
	IncrementDBEntry(GetCharField(DND_DB_ATTRIBUTEPERK, char_id), pacc, temp);
	
	// save player inventory state (might have been affected by an orb)
	// save singular items that have their ids noted by activities instead in here
	SavePlayerInventoryStuff(pnum, char_id, pacc, DND_PINVFLAGS_ALL);
	
	SetDBEntry(GetCharField(DND_DB_NPCTRACKER, char_id), pacc, CheckActorInventory(pnum + P_TIDSTART, "DnD_NPC_Meet"));
	
	SetDBEntry(GetCharField(DND_DB_KILLTRACKER, char_id), pacc, CheckActorInventory(pnum + P_TIDSTART, "DnD_LifeTimeKills"));
	SetDBEntry(GetCharField(DND_DB_KILLTRACKER_MILLION, char_id), pacc, CheckActorInventory(pnum + P_TIDSTART, "DnD_LifeTimeKills_Millions"));
	SetDBEntry(GetCharField(DND_DB_KILLTRACKER_BILLION, char_id), pacc, CheckActorInventory(pnum + P_TIDSTART, "DnD_LifeTimeKills_Billions"));
	
	Log(s:"Saving player ", d:pnum, s:"'s activities.");
}

void LoadPlayerStash(int pnum, str pacc) {
	int temp, w, h, i, j;
	ResetPlayerStash(pnum);
	// load stash
	temp = GetDBEntry(DND_DB_STASH_PAGES, pacc);
	SetInventory("DnD_PlayerInventoryPages", temp);
	PlayerActivities[pnum].stash_pages = temp;
	// keep base amount at 1
	if(!temp) {
		GiveInventory("DnD_PlayerInventoryPages", 1);
		PlayerActivities[pnum].stash_pages = 1;
	}
	for(i = 0; i < CheckInventory("DnD_PlayerInventoryPages"); ++i) {
		for(j = 0; j < MAX_INVENTORY_BOXES; ++j) {
			temp = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), pacc);
			if(temp != DND_ITEM_NULL) {
				PlayerStashList[pnum][i][j].width = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), pacc);
				PlayerStashList[pnum][i][j].height = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), pacc);
				PlayerStashList[pnum][i][j].item_image = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), pacc);
				PlayerStashList[pnum][i][j].item_type = temp;
				PlayerStashList[pnum][i][j].item_subtype = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), pacc);
				PlayerStashList[pnum][i][j].item_level = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), pacc);
				PlayerStashList[pnum][i][j].item_stack = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_STACK), pacc);
				PlayerStashList[pnum][i][j].attrib_count = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), pacc);
				
				PlayerStashList[pnum][i][j].corrupted = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), pacc);
				PlayerStashList[pnum][i][j].quality = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), pacc);
				
				PlayerStashList[pnum][i][j].implicit.attrib_id = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), pacc);
				PlayerStashList[pnum][i][j].implicit.attrib_val = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), pacc);
				PlayerStashList[pnum][i][j].implicit.attrib_tier = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), pacc);
				PlayerStashList[pnum][i][j].implicit.attrib_extra = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), pacc);

				for(temp = 0; temp < PlayerStashList[pnum][i][j].attrib_count; ++temp) {
					PlayerStashList[pnum][i][j].attributes[temp].attrib_val = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:temp), pacc);
					PlayerStashList[pnum][i][j].attributes[temp].attrib_id = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:temp), pacc);
					PlayerStashList[pnum][i][j].attributes[temp].attrib_tier = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:temp), pacc);
					PlayerStashList[pnum][i][j].attributes[temp].attrib_extra = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:temp), pacc);
					PlayerStashList[pnum][i][j].attributes[temp].fractured = GetDBEntry(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_STASHPAGE, d:i, s:"_", d:j, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:temp), pacc);
				}
				
				// init pointers
				for(w = 0; w < PlayerStashList[pnum][i][j].width; ++w)
					for(h = 0; h < PlayerStashList[pnum][i][j].height; ++h) {
						temp = j + w + h * MAXINVENTORYBLOCKS_VERT;
						PlayerStashList[pnum][i][temp].item_type = PlayerStashList[pnum][i][j].item_type;
						PlayerStashList[pnum][i][temp].topleftboxid = j + 1;
					}
			}
		}
	}
	// sync all of stash
	SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_STASH);
	
	// we must do this here after loading
	SyncAllClientsideVariables(pnum);
}

// Loads from database and gives necessary items
void LoadPlayerData(int pnum, int char_id) {
	// assumes all checks have been performed before reaching this function
	int i = 0, j = 0, h, w, temp;
	str pacc = GetPlayerAccountName(pnum);
	
	// set player class to what they initially chose -- CAREFUL ITS NOT CHARACTERID ITS CHARACTER!!! FORMER IS DATABASE ID
	temp = GetDBEntry(GetCharField(DND_DB_CLASSID, char_id), pacc);
	SetInventory("DnD_Character", temp);
	
	// force an update on the player sprite immediately!
	ACS_NamedExecuteWithResult("DnD Player Mugshot Class Update", temp - 1);
	SetActorState(0, "Spawn");
	
	/*int res = SetPlayerClass(
		pnum, 
		StrParam(l:GetClassLabel(StrParam(s:"CLASS", d:temp - 1), DND_CLASS_LABEL_ACTOR)),
		true
	);
	*/
	
	// read level -- moved here due to dependency on class perks
	temp = GetDBEntry(GetCharField(DND_DB_LEVEL, char_id), pacc);
	if(!temp)
		temp = 1;
	SetInventory("Level", temp);
	SetInventory("LevelExpCap", GetExpLimit_Level(temp));
	
	// read exp
	temp = GetDBEntry(GetCharField(DND_DB_EXP, char_id), pacc);
	SetInventory("Exp", temp);
	CalculateExpRatio();
	
	// immediately update exp, because this is 0 initially and people could lose exp potentially
	UpdateActivity(pnum, DND_ACTIVITY_EXP, temp, 0);
	
	// read credits
	SetInventory("Credit", GetDBEntry(GetCharField(DND_DB_CREDIT, char_id), pacc));
	
	// load class perks
	HandleClassPerks();
	
	// make sure starting char code doesn't run
	SetInventory("ReflectCount", 0);
	SetInventory("Intervened", 0);
	SetInventory("CanIntervene", 1);
	TakeInventory("SetMyLives", 1);
	SetInventory("ShowingMenu", 0);
	
	// load player inventory
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		temp = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), char_id), pacc);
		// we got the main item pointer here
		if(temp != DND_ITEM_NULL) {
			PlayerInventoryList[pnum][i].width = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), char_id), pacc);
			PlayerInventoryList[pnum][i].height = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), char_id), pacc);
			PlayerInventoryList[pnum][i].item_image = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), char_id), pacc);
			PlayerInventoryList[pnum][i].item_type = temp;
			PlayerInventoryList[pnum][i].item_subtype = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), char_id), pacc);
			PlayerInventoryList[pnum][i].item_level = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), char_id), pacc);
			PlayerInventoryList[pnum][i].item_stack = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_STACK), char_id), pacc);
			PlayerInventoryList[pnum][i].attrib_count = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), char_id), pacc);
			
			PlayerInventoryList[pnum][i].corrupted = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), char_id), pacc);
			PlayerInventoryList[pnum][i].quality = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), char_id), pacc);
			
			PlayerInventoryList[pnum][i].implicit.attrib_id = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), char_id), pacc);
			PlayerInventoryList[pnum][i].implicit.attrib_val = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), char_id), pacc);
			PlayerInventoryList[pnum][i].implicit.attrib_tier = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), char_id), pacc);
			PlayerInventoryList[pnum][i].implicit.attrib_extra = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), char_id), pacc);


			for(j = 0; j < PlayerInventoryList[pnum][i].attrib_count; ++j) {
				PlayerInventoryList[pnum][i].attributes[j].attrib_val = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:j), char_id), pacc);
				PlayerInventoryList[pnum][i].attributes[j].attrib_id = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:j), char_id), pacc);
				PlayerInventoryList[pnum][i].attributes[j].attrib_tier = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:j), char_id), pacc);
				PlayerInventoryList[pnum][i].attributes[j].attrib_extra = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:j), char_id), pacc);
				PlayerInventoryList[pnum][i].attributes[j].fractured = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:j), char_id), pacc);
			}
			
			// init pointers
			for(w = 0; w < PlayerInventoryList[pnum][i].width; ++w)
				for(h = 0; h < PlayerInventoryList[pnum][i].height; ++h) {
					temp = i + w + h * MAXINVENTORYBLOCKS_VERT;
					PlayerInventoryList[pnum][temp].item_type = PlayerInventoryList[pnum][i].item_type;
					PlayerInventoryList[pnum][temp].topleftboxid = i + 1;
				}
		}
	}
	// sync all of inventory
	SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	
	// load charms used
	for(i = 0; i < MAX_CHARMS_EQUIPPABLE; ++i) {
		Charms_Used[pnum][i].topleftboxid = i + 1;
		Charms_Used[pnum][i].width = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), char_id), pacc);
		Charms_Used[pnum][i].height = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), char_id), pacc);
		Charms_Used[pnum][i].item_image = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), char_id), pacc);
		Charms_Used[pnum][i].item_type = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), char_id), pacc);
		Charms_Used[pnum][i].item_subtype = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), char_id), pacc);
		Charms_Used[pnum][i].item_level = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), char_id), pacc);
		Charms_Used[pnum][i].item_stack = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_STACK), char_id), pacc);
		Charms_Used[pnum][i].attrib_count = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), char_id), pacc);
		
		Charms_Used[pnum][i].corrupted = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), char_id), pacc);
		Charms_Used[pnum][i].quality = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), char_id), pacc);
		
		Charms_Used[pnum][i].implicit.attrib_id = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), char_id), pacc);
		Charms_Used[pnum][i].implicit.attrib_val = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), char_id), pacc);
		Charms_Used[pnum][i].implicit.attrib_tier = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), char_id), pacc);
		Charms_Used[pnum][i].implicit.attrib_extra = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), char_id), pacc);

		for(j = 0; j < Charms_Used[pnum][i].attrib_count; ++j) {
			Charms_Used[pnum][i].attributes[j].attrib_val = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:j), char_id), pacc);
			Charms_Used[pnum][i].attributes[j].attrib_id = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:j), char_id), pacc);
			Charms_Used[pnum][i].attributes[j].attrib_tier = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:j), char_id), pacc);
			Charms_Used[pnum][i].attributes[j].attrib_extra = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:j), char_id), pacc);
			Charms_Used[pnum][i].attributes[j].fractured = GetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:j), char_id), pacc);
		}
		
		// apply the loaded charm's features
		if(Charms_Used[pnum][i].item_type != DND_ITEM_NULL)
			ApplyItemFeatures(pnum, i, DND_SYNC_ITEMSOURCE_CHARMUSED);
	}
	// sync all of charms used
	SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_CHARMUSED);
	
	LoadPlayerStash(pnum, pacc);
	
	// read temp ammo -- first so weapon doesnt get removed
	for(i = 0; i < MAXTEMPWEPS; ++i)
		SetInventory(TemporaryWeaponData[i][TEMPWEP_AMMO], GetDBEntry(GetCharField(StrParam(s:DND_DB_TEMPAMMO, d:i), char_id), pacc));
		
	// read weapons
	temp = GetDBEntry(GetCharField(StrParam(s:DND_DB_WEAPONINT, d:1), char_id), pacc);
	for(i = 0; i < MAXWEPS; ++i) {
		j = i / 32;
		if(IsSet(temp, i - 32 * j)) {
			GiveInventory(Weapons_Data[i].name, 1);
		}
		if(j != (i + 1) / 32 || i == MAXWEPS - 1) { // ie. our j changed
			// update bitset
			temp = GetDBEntry(GetCharField(StrParam(s:DND_DB_WEAPONINT, d:j + 2), char_id), pacc);
		}
	}
	
	// read stats
	temp = GetDBEntry(GetCharField(DND_DB_STATS_1, char_id), pacc);
	SetInventory("PSTAT_Strength", temp & 0xFF);
	temp >>= 8;
	SetInventory("PSTAT_Dexterity", temp & 0xFF);
	temp >>= 8;
	SetInventory("PSTAT_Bulkiness", temp & 0xFF);
	temp >>= 8;
	SetInventory("PSTAT_Charisma", temp & 0xFF);
	
	temp = GetDBEntry(GetCharField(DND_DB_STATS_2, char_id), pacc);
	SetInventory("PSTAT_Vitality", temp & 0xFF);
	temp >>= 8;
	SetInventory("BackpackCounter", temp & 0xFF);
	
	// intellect
	temp = GetDBEntry(GetCharField(DND_DB_STATS_3, char_id), pacc);
	SetInventory("PSTAT_Intellect", temp & 0xFF);
	
	// read perks - 1
	temp = GetDBEntry(GetCharField(DND_DB_PERKS, char_id), pacc);
	SetInventory("Perk_Sharpshooting", temp & 0xF);
	temp >>= 4;
	SetInventory("Perk_Brutality", temp & 0xF);
	temp >>= 4;
	SetInventory("Perk_Endurance", temp & 0xF);
	temp >>= 4;
	SetInventory("Perk_Wisdom", temp & 0xF);
	temp >>= 4;
	SetInventory("Perk_Greed", temp & 0xF);
	temp >>= 4;
	SetInventory("Perk_Medic", temp & 0xF);
	temp >>= 4;
	SetInventory("Perk_Munitionist", temp & 0xF);
	temp >>= 4;
	SetInventory("Perk_Deadliness", temp & 0xF);
	
	// read perks - 2
	temp = GetDBEntry(GetCharField(DND_DB_PERKS2, char_id), pacc);
	SetInventory("Perk_Savagery", temp & 0xF);
	temp >>= 4;
	SetInventory("Perk_Luck", temp & 0xF);
	
	// read health and armor
	temp = GetDBEntry(GetCharField(DND_DB_HEALTHARMOR, char_id), pacc);
	// make sure charms and shit are added here too
	int armor = temp & 0xFFFF;
	temp >>= 16;
	SetActorProperty(0, APROP_SPAWNHEALTH, GetSpawnHealth());
	SetActorProperty(0, APROP_HEALTH, (temp & 0xFFFF) ? temp & 0xFFFF : DND_BASE_HEALTH);
	
	// read armor type
	// reset whatever armor might have been on the player at the time
	RemoveAllArmor();
	temp = GetDBEntry(GetCharField(DND_DB_ARMORTYPE, char_id), pacc);
	for(i = 0; i < MAXARMORS; ++i) {
		if(IsSet(temp, i)) {
			SetArmorType(i);
			SetArmorAmount(armor);
			break;
		}
	}
	
	// read accessories
	temp = GetDBEntry(GetCharField(DND_DB_ACCESSORIES, char_id), pacc);
	for(i = 0; i < MAX_ACCESSORY; ++i)
		SetInventory(StrParam(s:"Accessory_", d:i + 1), !!(temp & (1 << i)));
	// save active accessories
	SetInventory("Accessory_Index", GetDBEntry(GetCharField(DND_DB_ACTIVEACCESSORIES, char_id), pacc));
	DecideAccessories();
	
	// set all ammo capacities first
	SetAllAmmoCapacities();
	
	// read each ammo count
	for(i = 0; i < MAX_SLOTS; ++i)
		for(j = 0; j < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[i][j].initial_capacity != -1; ++j) {
			temp = GetDBEntry(StrParam(s:GetCharField(DND_DB_AMMO, char_id), s:AmmoInfo_Str[i][j][AMMOINFO_NAME]), pacc);
			SetInventory(AmmoInfo_Str[i][j][AMMOINFO_NAME], temp);
		}
	
	// read special ammos
	for(i = 0; i < MAX_SPECIAL_AMMOS; ++i) {
		temp = GetDBEntry(StrParam(s:GetCharField(DND_DB_SPECIALAMMO, char_id), s:SpecialAmmoInfo_Str[i][AMMOINFO_NAME]), pacc);
		SetInventory(SpecialAmmoInfo_Str[i][AMMOINFO_NAME], temp);
	}
	
	// read clip ammos
	for(i = 0; i < MAXCLIPAMMOTYPES; ++i) {
		temp = GetDBEntry(StrParam(s:GetCharField(DND_DB_AMMO, char_id), s:ClipAmmoTypes[i]), pacc);
		SetInventory(ClipAmmoTypes[i], temp);
	}
	
	// read abilities
	temp = GetDBEntry(GetCharField(DND_DB_ABILITY, char_id), pacc);
	for(i = 0; i < MAXABILITIES; ++i) {
		if(IsSet(temp, i))
			GiveInventory(AbilityInfo[i], 1);
	}
	
	// read unspent attribute/perk points
	// added: Update player activity to prevent it going from negative
	temp = GetDBEntry(GetCharField(DND_DB_ATTRIBUTEPERK, char_id), pacc);
	SetInventory("AttributePoint", temp & 0x7FF);
	temp >>= 11;
	SetInventory("PerkPoint", temp & 0x7FF);

	temp = GetDBEntry(GetCharField(DND_DB_ARTIFACTS, char_id), pacc);
	// read artifacts
	SetInventory("SalvationSphere", temp & 1);
	for(i = 3; i < MAXARTIFACTS - 3; ++i)
		// because 0th bit is for salvation sphere
		SetInventory(ArtifactInfo[i][ARTI_NAME], !!(temp & (1 << (i - 2)))); // these have only 1 carry limit
	// at this point MAXARTIFACTS - 3 bits are occupied
	temp >>= (MAXARTIFACTS - 5);
	SetInventory("FieldKit", temp & 0x3);
	temp >>= 2;
	SetInventory("PortableShield", temp & 0x3);
	temp >>= 2;
	SetInventory("StatReset", temp & 1);

	// read researches
	for(i = 0; i < RESEARCH_BITSETS; ++i) {
		int k;
		int res_on_cur_bitset = 31; //Each bitset fits 31-bits (Actor inventory can only use 31-bits).
		if (i == RESEARCH_BITSETS - 1) {
			res_on_cur_bitset = MAX_RESEARCHES % 31;
			if (res_on_cur_bitset == 0)
				res_on_cur_bitset = 31; //Make sure having exactly 31/62/93... researches don't break this.
		}
		
		temp = GetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCH_DONE, char_id), d: i + 1), pacc);
		k = 0;
		for(j = 0; j < res_on_cur_bitset; ++j)
			if(IsSet(temp, j))
				k = SetBit(k, j);
		SetInventory(StrParam(s:"Research_Done_", d:i + 1), k);
		
		temp = GetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCH_DISCOVERED, char_id), d: i + 1), pacc);
		k = 0;
		for(j = 0; j < res_on_cur_bitset; ++j)
			if(IsSet(temp, j))
				k = SetBit(k, j);
		SetInventory(StrParam(s:"Research_Discovered_", d:i + 1), k);
	}
	
	// read research investment
	for(i = 0; i < MAX_RESEARCHES; ++i) {
		ResearchInvestments[pnum][i] = GetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCH_INVESTED, char_id), d:i + 1), pacc);
		
		// if non-zero, run this. we don't want to run a bunch of these when most of these are most likely to be 0
		if(ResearchInvestments[pnum][i])
			ACS_NamedExecuteWithResult("DnD Menu Investment Sync", pnum, i, ResearchInvestments[pnum][i]);
	}
	
	HandleResearchBonuses();
	
	// read research trackers
	for(i = 0; i < MAX_RESEARCH_TRACKERS; ++i)
		SetInventory(ResearchTrackers[i], GetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCHTRACKER, char_id), d:i + 1), pacc));
		
	// read legendary monster kills
	SetInventory("LegendaryKills", GetDBEntry(StrParam(s:GetCharField(DND_DB_LEGENDARYTRACKER, char_id), d:i + 1), pacc));
	
	// read budget
	temp = GetDBEntry(GetCharField(DND_DB_BUDGET, char_id), pacc);
	SetInventory("Budget", temp);
	
	// load quest states and give rewards accordingly
	temp = GetDBEntry(GetCharField(DND_DB_QUESTSTATE1, char_id), pacc);
	SetInventory("DnD_QuestState1", temp);
	
	// quest count is < 32 so only 1 variable is enough atm
	/*temp = GetDBEntry(GetCharField(DND_DB_QUESTSTATE2, char_id), pacc);
	SetInventory("DnD_QuestState2", temp);*/
		
	if(!CheckInventory("HardcoreDebuff"))
		GiveInventory("HardcoreDebuff", 1);
	
	// load weapon checkers
	temp = GetDBEntry(GetCharField(DND_DB_PLAYERWEPCHECKERS, char_id), pacc);
	for(i = 0; i < MAX_WEPCONDITION_CHECKERS; ++i)
		if(IsSet(temp, i))
			GiveInventory(WeaponConditionCheckers[i], 1);
	
	
	// load player's weapon mods
	for(i = 0; i < MAXWEPS; ++i) {
		// check mods
		// enhancement orbs used
		temp = GetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONQUALITY, char_id), d:i), pacc);
		Player_Weapon_Infos[pnum][i].quality = temp & 0xFF;
		
		// rest of the mods for the weapon
		for(j = 0; j < MAX_WEP_MODS; ++j) {
			Player_Weapon_Infos[pnum][i].wep_mods[j].tier = GetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONMOD_TIER, char_id), d:j, s:"_Weapon", d:i), pacc);
			Player_Weapon_Infos[pnum][i].wep_mods[j].val = GetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONMOD_VAL, char_id), d:j, s:"_Weapon", d:i), pacc);
		}
	}
	
	// load stored medkit
	temp = GetDBEntry(GetCharField(DND_DB_STOREDKIT, char_id), pacc);
	SetAmmoCapacity("StoredMedkit", GetAmmoCapacity("StoredMedkit") + 15 * CheckInventory("Perk_Medic"));
	SetInventory("StoredMedkit", temp);
	
	// load npc meet status
	temp = GetDBEntry(GetCharField(DND_DB_NPCTRACKER, char_id), pacc);
	SetInventory("DnD_NPC_Meet", temp);
	
	// Restoration steps
	RestoreRPGStat(RES_PLAYERSPEED | RES_NOCLASSPERK);
	UpdatePlayerKnockbackResist();
	
	// Research stuff
	RestoreResearchItems();
	// fixes some clientside things not being synced right on load
	ACS_NamedExecuteAlways("DnD Menu Reset Forced", 0);
	// this seems unnecessary, as it syncs the 3 item source datas (we already do that as soon as we finished loading them)
	// and also the other clientside variables, which we do at the end of enter script...
	//ACS_NamedExecuteAlways("DnD Force Sync Player Global Data", 0);
	
	SetInventory("DnD_LifeTimeKills", GetDBEntry(GetCharField(DND_DB_KILLTRACKER, char_id), pacc));
	SetInventory("DnD_LifeTimeKills_Millions", GetDBEntry(GetCharField(DND_DB_KILLTRACKER_MILLION, char_id), pacc));
	SetInventory("DnD_LifeTimeKills_Billions", GetDBEntry(GetCharField(DND_DB_KILLTRACKER_BILLION, char_id), pacc));
	
	#ifdef ISAPRILFIRST
		// load nft
		SetInventory("NFT_Token", GetDBEntry(GetCharField(DND_DB_NFT, char_id), pacc));
	#endif
	
	//printbold(s:"finish load");
}

void load_char(int pnum, int char_id) {
	SetInventory("DnD_CharacterID", char_id);
	PlayerActivities[pnum].char_id = char_id;
	
	// store unique id for future lookup to work even if a player disconnects
	StrCpy(PlayerActivities[pnum].player_account, GetPlayerAccountName(pnum));
	
	PlayerLoaded[pnum] = 1;
	SetInventory("CanLoad", 0);
	LoadPlayerData(pnum, char_id);
}

int check_load_char(int pnum, int char_id) {
	// if not hardcore don't bother trying to load
	if(!(GetCVar("dnd_mode") >= DND_MODE_SOFTCORE) || !HardcoreSet) return DND_LOGIN_NOTHARDCORE;
	if(!PlayerIsLoggedIn(pnum)) return DND_LOGIN_NOTLOGGED;
	if(!PlayerInGame(pnum)) return DND_LOGIN_NOTINGAME;
	// if gamestate is countdown don't let them because it'll be reset anyways
	if(GetGameModeState() == GAMESTATE_COUNTDOWN) return DND_LOGIN_INCOUNTDOWN;
	if(char_id < 0 || char_id > DND_MAX_CHARS - 1) return DND_LOGIN_CHARNOTINRANGE;
	// handle no data case -- only let people to load their stuff if there really is some data
	if(!GetDBEntry(GetCharField(DND_DB_HEALTHARMOR, char_id), GetPlayerAccountName(pnum))) return DND_LOGIN_NOCHAR;
	// prevent people from loading their stuff to escape death, prevent loading if a save on this player happened already!
	if(PlayerLoaded[pnum]) return DND_LOGIN_CHARINUSE;
	if(!CheckInventory("CanLoad")) return DND_LOGIN_NOTIME;
	return DND_CHARLOADED;
}

// on Hardcore death, wipeout all data related to this player
void WipeoutPlayerData(int pnum, int cid) {
	int i, j, char_id = Clamp_Between(cid, 0, DND_MAX_CHARS - 1);
	str pacc = GetPlayerAccountName(pnum);
	for(i = 1; i <= (MAXWEPS / 32) + 1; ++i)
		SetDBEntry(GetCharField(StrParam(s:DND_DB_WEAPONINT, d:i), char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_TEMPAMMO, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_STATS_1, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_STATS_2, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_STATS_3, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_PERKS, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_HEALTHARMOR, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_ARMORTYPE, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_CLASSID, char_id), pacc, 0);
	
	// reset ammo counts
	for(i = 0; i < MAX_SLOTS; ++i)
		for(j = 0; j < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[i][j].initial_capacity != -1; ++j) {
			SetDBEntry(StrParam(s:GetCharField(DND_DB_AMMO, char_id), s:AmmoInfo_Str[i][j][AMMOINFO_NAME]), pacc, 0);
		}
	
	for(i = 0; i < MAX_SPECIAL_AMMOS; ++i)
		SetDBEntry(StrParam(s:GetCharField(DND_DB_SPECIALAMMO, char_id), s:SpecialAmmoInfo_Str[i][AMMOINFO_NAME]), pacc, 0);
	
	// reset weapon clips/magazines that use them -- first 6 are firing mode information
	for(i = 0; i < 6; ++i)
		SetDBEntry(StrParam(s:GetCharField(DND_DB_AMMO, char_id), s:ClipAmmoTypes[i]), pacc, 0);
	
	for(i = 6; i < MAXCLIPAMMOTYPES; ++i)
		SetDBEntry(StrParam(s:GetCharField(DND_DB_AMMO, char_id), s:ClipAmmoTypes[i]), pacc, GetAmmoCapacity(ClipAmmoTypes[i]));
	
	SetDBEntry(GetCharField(DND_DB_EXP, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_CREDIT, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_LEVEL, char_id), pacc, 1);
	SetDBEntry(GetCharField(DND_DB_ABILITY, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_ATTRIBUTEPERK, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_ACCESSORIES, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_ARTIFACTS, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_BUDGET, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_QUESTSTATE1, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_QUESTSTATE2, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_SURVIVECOUNT, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_PLAYERWEPCHECKERS, char_id), pacc, 0);
	
	for(i = 0; i < RESEARCH_BITSETS; ++i) {
		SetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCH_DONE, char_id), d:i + 1), pacc, 0);
		SetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCH_DISCOVERED, char_id), d:i + 1), pacc, 0);
	}
	
	// reset researches
	for(i = 0; i < MAX_RESEARCH_TRACKERS; ++i)
		SetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCHTRACKER, char_id), d:i + 1), pacc, 0);
		
	// reset investments
	for(i = 0; i < MAX_RESEARCHES; ++i)
		SetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCH_INVESTED, char_id), d:i + 1), pacc, 0);
	
	// reset legendary kill track
	SetDBEntry(StrParam(s:GetCharField(DND_DB_LEGENDARYTRACKER, char_id), d:i + 1), pacc, 0);

	for(i = 0; i < MAXWEPS; ++i) {
		// enhancement orbs used
		SetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONQUALITY, char_id), d:i), pacc, 0);

		// rest of the mods
		for(j = 0; j < MAX_WEP_MODS; ++j) {
			SetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONMOD_TIER, char_id), d:j, s:"_Weapon", d:i), pacc, 0);
			SetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONMOD_VAL, char_id), d:j, s:"_Weapon", d:i), pacc, 0);
		}
	}
	
	// clean up inventory stuff
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_STACK), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), char_id), pacc, 0);

		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), char_id), pacc, 0);

		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), char_id), pacc, 0);

		for(j = 0; j < PlayerInventoryList[pnum][i].attrib_count; ++j) {
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:j), char_id), pacc, 0);
		}
	}
	SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	// reset charms used
	for(i = 0; i < MAX_CHARMS_EQUIPPABLE; ++i) {
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_STACK), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), char_id), pacc, 0);

		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), char_id), pacc, 0);

		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), char_id), pacc, 0);

		for(j = 0; j < Charms_Used[pnum][i].attrib_count; ++j) {
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:j), char_id), pacc, 0);
		}
	}
	SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_CHARMUSED);
	// we don't reset the stash, we're merciful xD
	
	SetDBEntry(GetCharField(DND_DB_KILLTRACKER, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_KILLTRACKER_MILLION, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_KILLTRACKER_BILLION, char_id), pacc, 0);
	
	#ifdef ISAPRILFIRST
		// load nft
		SetDBEntry(GetCharField(DND_DB_NFT, char_id), pacc, 0);
	#endif
}

void create_char(int pnum, int char_id) {
	SetInventory("DnD_CharacterID", char_id);
	PlayerActivities[pnum].char_id = char_id;
	PlayerLoaded[pnum] = 1;
	SetInventory("CanLoad", 0); //Let create char end loading timer
}

int check_create_char(int pnum, int char_id) {
	if(!(GetCVar("dnd_mode") >= DND_MODE_SOFTCORE) || !HardcoreSet) return DND_LOGIN_NOTHARDCORE;
	if (!PlayerIsLoggedIn(pnum)) return DND_LOGIN_NOTLOGGED;
	if(!PlayerInGame(pnum)) return DND_LOGIN_NOTINGAME;
	if(GetGameModeState() == GAMESTATE_COUNTDOWN) return DND_LOGIN_INCOUNTDOWN;
	if(char_id < 0 || char_id > DND_MAX_CHARS - 1) return DND_LOGIN_CHARNOTINRANGE;
	if(PlayerLoaded[pnum]) return DND_LOGIN_CHARINUSE;
	if(!CheckInventory("CanLoad")) return DND_LOGIN_NOTIME; //For now, allow player to setchar until end of map manually. -- Uncommented to see if it prevents stashes being destroyed.
	return DND_LOGIN_CREATECHAROK;
}

void transfer_char(int pnum, int char_id) {
	if(char_id == CheckInventory("DnD_CharacterID"))
		PlayerDatabaseState[pnum][PLAYER_TRANSFERSTATE] = false;
	else {
		PlayerDatabaseState[pnum][PLAYER_TRANSFERSTATE] = true;
		SetInventory("DnD_TransfCharacterID", char_id);
	}
}

int check_transfer_char(int pnum, int char_id) {
	if(!(GetCVar("dnd_mode") >= DND_MODE_SOFTCORE) || !HardcoreSet) return DND_LOGIN_NOTHARDCORE;
	if (!PlayerIsLoggedIn(pnum)) return DND_LOGIN_NOTLOGGED;
	if(!PlayerInGame(pnum)) return DND_LOGIN_NOTINGAME;
	if(GetGameModeState() == GAMESTATE_COUNTDOWN) return DND_LOGIN_INCOUNTDOWN;
	if(!PlayerLoaded[pnum]) return DND_LOGIN_NOCHARINUSE;
	if(char_id < 0 || char_id > DND_MAX_CHARS-1) return DND_LOGIN_CHARNOTINRANGE;
	
	if(char_id == CheckInventory("DnD_CharacterID")) return DND_LOGIN_TRANSFSAMESLOT;
	return DND_LOGIN_CHARTRANSFERRED;
}

void BulkResetPlayerData(int pnum) {
	ResetPlayerInfo(pnum);
	ResetPlayerActivities(pnum, true);
}

void SaveDefaultPlayer(int pnum, int char_id) {
	int i, j;
	str pacc = GetPlayerAccountName(pnum);
	char_id = Clamp_Between(char_id, 0, DND_MAX_CHARS - 1);
	
	int temp = (1 << DND_WEAPON_FIST) | (1 << DND_WEAPON_PISTOL);
	SetDBEntry(GetCharField(StrParam(s:DND_DB_WEAPONINT, d:1), char_id), pacc, temp);
	
	for(i = 2; i <= (MAXWEPS / 32) + 1; ++i) {
		SetDBEntry(GetCharField(StrParam(s:DND_DB_WEAPONINT, d:i), char_id), pacc, 0);
	}
	
	SetDBEntry(GetCharField(DND_DB_TEMPAMMO, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_STATS_1, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_STATS_2, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_STATS_3, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_PERKS, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_HEALTHARMOR, char_id), pacc, 100); // base health
	SetDBEntry(GetCharField(DND_DB_ARMORTYPE, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_CLASSID, char_id), pacc, CheckActorInventory(pnum + P_TIDSTART, "DnD_Character"));
	
	// reset ammo counts
	for(i = 0; i < MAX_SLOTS; ++i)
		for(j = 0; j < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[i][j].initial_capacity != -1; ++j) {
			if(i != DND_AMMOSLOT_CLIP || j != AMMO_CLIP)
				SetDBEntry(StrParam(s:GetCharField(DND_DB_AMMO, char_id), s:AmmoInfo_Str[i][j][AMMOINFO_NAME]), pacc, 0);
			else
				SetDBEntry(StrParam(s:GetCharField(DND_DB_AMMO, char_id), s:AmmoInfo_Str[i][j][AMMOINFO_NAME]), pacc, 50);
		}
	
	for(i = 0; i < MAX_SPECIAL_AMMOS; ++i)
		SetDBEntry(StrParam(s:GetCharField(DND_DB_SPECIALAMMO, char_id), s:SpecialAmmoInfo_Str[i][AMMOINFO_NAME]), pacc, 0);
	
	// reset weapon clips/magazines that use them
	// first 6 store firing modes, we need to set them to 0
	str amm;
	for(i = 0; i < 6; ++i) {
		amm = StrParam(s:GetCharField(DND_DB_AMMO, char_id), s:ClipAmmoTypes[i]);
		SetDBEntry(amm, pacc, 0);
	}
	
	for(i = 6; i < MAXCLIPAMMOTYPES; ++i) {
		amm = StrParam(s:GetCharField(DND_DB_AMMO, char_id), s:ClipAmmoTypes[i]);
		SetDBEntry(amm, pacc, GetAmmoCapacity(ClipAmmoTypes[i]));
	}
	
	SetDBEntry(GetCharField(DND_DB_EXP, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_CREDIT, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_LEVEL, char_id), pacc, 1);
	SetDBEntry(GetCharField(DND_DB_ABILITY, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_ATTRIBUTEPERK, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_ACCESSORIES, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_ARTIFACTS, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_BUDGET, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_QUESTSTATE1, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_QUESTSTATE2, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_SURVIVECOUNT, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_PLAYERWEPCHECKERS, char_id), pacc, 0);
	
	for(i = 0; i < RESEARCH_BITSETS; ++i) {
		SetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCH_DONE, char_id), d:i + 1), pacc, 0);
		SetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCH_DISCOVERED, char_id), d:i + 1), pacc, 0);
	}
	
	// reset researches
	for(i = 0; i < MAX_RESEARCH_TRACKERS; ++i)
		SetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCHTRACKER, char_id), d:i + 1), pacc, 0);
		
	// reset investments
	for(i = 0; i < MAX_RESEARCHES; ++i)
		SetDBEntry(StrParam(s:GetCharField(DND_DB_RESEARCH_INVESTED, char_id), d:i + 1), pacc, 0);
	
	// reset legendary kill track
	SetDBEntry(StrParam(s:GetCharField(DND_DB_LEGENDARYTRACKER, char_id), d:i + 1), pacc, 0);

	for(i = 0; i < MAXWEPS; ++i) {
		// enhancement orbs used
		SetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONQUALITY, char_id), d:i), pacc, 0);

		// rest of the mods
		for(j = 0; j < MAX_WEP_MODS; ++j) {
			SetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONMOD_TIER, char_id), d:j, s:"_Weapon", d:i), pacc, 0);
			SetDBEntry(StrParam(s:GetCharField(DND_DB_WEAPONMOD_VAL, char_id), d:j, s:"_Weapon", d:i), pacc, 0);
		}
	}
	
	// clean up inventory stuff
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_STACK), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), char_id), pacc, 0);

		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), char_id), pacc, 0);

		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), char_id), pacc, 0);

		for(j = 0; j < PlayerInventoryList[pnum][i].attrib_count; ++j) {
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, d:i, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:j), char_id), pacc, 0);
		}
	}
	SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	// reset charms used
	for(i = 0; i < MAX_CHARMS_EQUIPPABLE; ++i) {
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_WIDTH), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_HEIGHT), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMAGE), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_TYPE), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_SUBTYPE), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_LEVEL), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_STACK), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT), char_id), pacc, 0);

		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_CORRUPTED), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_QUALITY), char_id), pacc, 0);

		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITID), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER), char_id), pacc, 0);
		SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA), char_id), pacc, 0);

		for(j = 0; j < Charms_Used[pnum][i].attrib_count; ++j) {
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBID, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA, s:"_", d:j), char_id), pacc, 0);
			SetDBEntry(GetCharField(StrParam(s:DND_DB_PLAYERINVENTORY, s:DND_DB_CHARMUSED, d:i, s:DND_DB_PLAYERINVENTORYFIELD_FRACTURE, s:"_", d:j), char_id), pacc, 0);
		}
	}
	SyncAllItemData(pnum, DND_SYNC_ITEMSOURCE_CHARMUSED);
	// we don't reset the stash, we're merciful xD
	
	SetDBEntry(GetCharField(DND_DB_KILLTRACKER, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_KILLTRACKER_MILLION, char_id), pacc, 0);
	SetDBEntry(GetCharField(DND_DB_KILLTRACKER_BILLION, char_id), pacc, 0);
	
	#ifdef ISAPRILFIRST
		// load nft
		SetDBEntry(GetCharField(DND_DB_NFT, char_id), pacc, 0);
	#endif
}

#endif
