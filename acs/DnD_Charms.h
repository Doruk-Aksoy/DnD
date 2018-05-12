#ifndef DND_CHARMS_IN
#define DND_CHARMS_IN

#include "DnD_CharmAttribs.h"
#include "DnD_Common.h"

#define MAX_CHARM_ATTRIBUTE_FLAGSETS 2
#define MAX_CHARM_LEVEL 100
#define MAX_CHARM_AFFIXTIERS 4
#define CHARM_TIER_SEPERATOR 100 / MAX_CHARM_AFFIXTIERS

#define MAX_SMALL_CHARMS_USED 4
#define MAX_MEDIUM_CHARMS_USED 2
#define MAX_LARGE_CHARMS_USED 1

#define MAX_CHARMS_ON_FIELD 128
#define MAX_CHARMS_ON_PLAYER 20

#define SMALL_CHARM_ATTRIB_MAX 2

#define NULL_CHARM -1

#define DND_CHARMTID_BEGIN 29050
#define DND_CHARMTID_END 29150

#define CHARM_TIER_VARIANCE_LOWER 15
#define CHARM_TIER_VARIANCE_HIGHER 5

enum {
	DND_CHARM_SMALL,
	DND_CHARM_MEDIUM,
	DND_CHARM_LARGE
};

#define MAX_CHARM_TYPES DND_CHARM_LARGE + 1
str Charm_TypeName[MAX_CHARM_TYPES] = {
	"Small",
	"Medium",
	"Grand"
};

int Charm_MaxAffixes[MAX_CHARM_TYPES] = {
	2,
	3,
	5
};

int Charm_MaxUsable[MAX_CHARM_TYPES] = {
	4,
	2,
	1
};

enum {
	DND_CHARM_IMAGE0,
	DND_CHARM_IMAGE1,
	DND_CHARM_IMAGE2
};

#define MAX_CHARM_IMAGES (DND_CHARM_LARGE + 1) * (DND_CHARM_IMAGE2 + 1)
str Charm_Images[MAX_CHARM_IMAGES] = {
	"SCHRM1",
	"SCHRM2",
	"SCHRM3",
	
	"MCHRM1",
	"MCHRM2",
	"MCHRM3",
	
	"LCHRM1",
	"LCHRM2",
	"LCHRM3"
};

str Charm_Tiers[MAX_CHARM_AFFIXTIERS] = {
	"Whispering",
	"Fine",
	"Supreme",
	"Majestic"
};

typedef struct {
	int 				charm_type; // small medium large etc.
	int 				charm_image; // the image index (0, MAX_CHARM_IMAGES - 1)
	int 				charm_level; // (0, MAX_CHARM_LEVEL)
	int					charm_attrib_count;
	// stores indexes to charm_attrib_t struct array that contains all info, as bits
	int 				charm_attributes[MAX_CHARM_ATTRIBUTE_FLAGSETS];
} charm_info_T;

charm_info_T Charms_On_Field[MAX_CHARMS_ON_FIELD];
charm_info_T Charms_On_Players[MAXPLAYERS][MAX_CHARMS_ON_PLAYER];

void CopyCharmInfo_FieldToPlayer(int field_index, int player_index) {
	int pos = -1, i;
	for(i = 0; i < MAX_CHARMS_ON_PLAYER; ++i)
		if(Charms_On_Players[player_index][i].charm_type == NULL_CHARM) {
			pos = i;
			break;
		}
	if(pos == -1)
		return;
	// copy now
	Charms_On_Players[player_index][pos].charm_type = Charms_On_Field[field_index].charm_type;
	Charms_On_Players[player_index][pos].charm_image = Charms_On_Field[field_index].charm_image;
	Charms_On_Players[player_index][pos].charm_level = Charms_On_Field[field_index].charm_level;
	for(i = 0; i < MAX_CHARM_ATTRIBUTE_FLAGSETS; ++i)
		Charms_On_Players[player_index][pos].charm_attributes[i] = Charms_On_Field[field_index].charm_attributes[i];
	
	// the leftover spot is a null charm
	Charms_On_Field[field_index].charm_type = NULL_CHARM;
}

void CopyCharmInfo_PlayerToField(int pcharm_index, int player_index) {
	int pos = -1, i;
	for(i = 0; i < MAX_CHARMS_ON_FIELD; ++i)
		if(Charms_On_Field[i].charm_type == NULL_CHARM) {
			pos = i;
			break;
		}
	if(pos == -1)
		return;
	// copy now
	Charms_On_Field[pos].charm_type = Charms_On_Players[player_index][pcharm_index].charm_type;
	Charms_On_Field[pos].charm_image = Charms_On_Players[player_index][pcharm_index].charm_image;
	Charms_On_Field[pos].charm_level = Charms_On_Players[player_index][pcharm_index].charm_level;
	for(i = 0; i < MAX_CHARM_ATTRIBUTE_FLAGSETS; ++i)
		Charms_On_Field[pos].charm_attributes[i] = Charms_On_Players[player_index][pcharm_index].charm_attributes[i];
	
	// the leftover spot is a null charm
	Charms_On_Players[player_index][pcharm_index].charm_type = NULL_CHARM;
}

// based on average player level
int GetWeightedCharmTier() {
	// return average player level, +- some value
	return ((total_level / PlayerCount()) + random(-CHARM_TIER_VARIANCE_LOWER, CHARM_TIER_VARIANCE_HIGHER)) / CHARM_TIER_SEPERATOR;
}

void DropCharm(bool fromMonster) {
	if(fromMonster) {
		int c = CreateCharm();
		if(c != -1) {
			RollCharmInfo(c, random(0, MAX_CHARM_TYPES - 1), GetWeightedCharmTier(), 1);
		}
	}
	else {
		// player dropped so copy from player to field
	}
}

bool CheckCharmAttribute(int charm_pos, int attrib_index, bool onField, int pnum) {
	int i = attrib_index / 32;
	if(onField)
		return IsSet(Charms_On_Field[charm_pos].charm_attributes[i], attrib_index);
	else
		return IsSet(Charms_On_Players[pnum][charm_pos].charm_attributes[i], attrib_index);
	return false;
}

void RollCharmInfo(int charm_pos, int charm_type, int charm_tier, bool onField) {
	// roll random attributes for the charm
	int count = random(2, 2 * (charm_type + 1)), i, roll;
	while(i < count) {
		do {
			roll = random(0, LAST_CHARM_ATTRIBUTE);
		} while(CheckCharmAttribute(charm_pos, roll, onField, -1));
		AddAttributeToCharm(charm_pos, roll);
		++i;
	}
}

// can only add attributes to charms that are about to be created ie. on field dropped from monster
void AddAttributeToCharm(int charm_pos, int attrib) {
	if(Charms_On_Field[charm_pos].charm_attrib_count < Charm_MaxAffixes[Charms_On_Field[charm_pos].charm_type]) {
		++Charms_On_Field[charm_pos].charm_attrib_count;
		Charms_On_Field[charm_pos].charm_attributes[attrib / 32] |= 1 << attrib;
	}
}

// Creates a charm on the game field
int CreateCharm() {
	int pos = -1, i;
	for(i = 0; i < MAX_CHARMS_ON_FIELD; ++i)
		if(Charms_On_Field[i].charm_type == NULL_CHARM) {
			pos = i;
			break;
		}
	return pos;
}

#endif