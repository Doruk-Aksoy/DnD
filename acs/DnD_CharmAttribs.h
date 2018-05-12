#ifndef DND_CHARM_ATTRIBS_IN
#define DND_CHARM_ATTRIBS_IN

enum {
	CHARM_HP_INCREASE,
	CHARM_ARMOR_INCREASE,
	CHARM_HPPERCENT_INCREASE,
	CHARM_ARMORPERCENT_INCREASE,
	CHARM_SPEED_INCREASE,
	CHARM_MAGAZINE_INCREASE,
	
	CHARM_FLATPHYS_DAMAGE,
	CHARM_FLATENERGY_DAMAGE,
	CHARM_FLATEXP_DAMAGE,
	CHARM_FLATMAGIC_DAMAGE,
	CHARM_FLATELEM_DAMAGE,
	
	CHARM_SLOT1_DAMAGE,
	CHARM_SLOT2_DAMAGE,
	CHARM_SLOT3_DAMAGE,
	CHARM_SLOT4_DAMAGE,
	CHARM_SLOT5_DAMAGE,
	CHARM_SLOT6_DAMAGE,
	CHARM_SLOT7_DAMAGE,
	CHARM_SLOT8_DAMAGE,
	CHARM_TEMPWEP_DAMAGE,
	
	CHARM_PELLET_INCREASE,
	
	CHARM_EXPLOSION_RADIUS,
	CHARM_EXPLOSIVE_RESIST,
	
	CHARM_AMMOGAIN_CHANCE,
	CHARM_AMMOGAIN_INCREASE,
	
	CHARM_REGENCAP_INCREASE,
	
	CHARM_CRITCHANCE_INCREASE,
	CHARM_CRITPERCENT_INCREASE,
	CHARM_CRITDAMAGE_INCREASE,
	
	CHARM_KNOCKBACK_RESIST,
	CHARM_DAMAGEPERCENT_INCREASE,
	
	// 2nd flag set
	CHARM_ACCURACY_INCREASE
};

enum {
	CH_ATTR_LOW,
	CH_ATTR_HIGH,
	CH_ATTR_LEVELMOD
};
#define MAX_CHARM_ATTRIBUTE_SPECIFIERS CH_ATTR_LEVELMOD + 1

#define LAST_CHARM_ATTRIBUTE CHARM_ACCURACY_INCREASE
#define MAX_CHARM_ATTRIBUTE_TYPES LAST_CHARM_ATTRIBUTE + 1

typedef struct {
	int attrib_low;
	int attrib_high;
	int attrib_level_modifier;
} charm_attrib_T;

str Charm_Attribute_Names[MAX_CHARM_ATTRIBUTE_TYPES] = {
	"to Health",
	"to Armor",
	"% increased Health",
	"% increased Armor",
	"% increased Speed",
	"% increased magazine size",
	
	"added physical damage to attacks",
	"added energy damage to attacks",
	"added explosive damage to attacks",
	"added magic damage to attacks",
	"added elemental damage to attacks",
	
	"% increased slot 1 weapon damage",
	"% increased slot 2 weapon damage",
	"% increased slot 3 weapon damage",
	"% increased slot 4 weapon damage",
	"% increased slot 5 weapon damage",
	"% increased slot 6 weapon damage",
	"% increased slot 7 weapon damage",
	"% increased slot 8 weapon damage",
	"% increased temporary weapon damage",
	
	"% increased pellets on shells",
	
	"% increased explosion radius",
	"% increased explosion resist",
	
	"% chance to gain ammo back on firing",
	"% increased ammo gain from pickups",
	
	"to regeneration cap"
	
	"% additional crit chance",
	"% increased crit chance",
	"% additional crit damage",
	
	"to knockback resist",
	"% increased damage",
	"% increased accuracy"
};

charm_attrib_T Charm_Attribute_Info[MAX_CHARM_ATTRIBUTE_TYPES] = {
	{ 5, 20, 5 },
	{ 5, 20, 5 },
	{ 2, 6, 2 },
	{ 2, 6, 2 },
	{ 1, 3, 1 },
	{ 2, 4, 2 },

	{ 1, 4, 2 },
	{ 1, 4, 2 },
	{ 1, 4, 2 },
	{ 1, 4, 2 },
	{ 1, 4, 2 },
	
	{ 5, 10, 2 },
	{ 5, 10, 2 },
	{ 5, 10, 2 },
	{ 5, 10, 2 },
	{ 5, 10, 2 },
	{ 5, 10, 2 },
	{ 5, 10, 2 },
	{ 5, 10, 2 },
	{ 5, 10, 2 },
	
	{ 5, 10, 2 },
	
	{ 4, 8, 3 },
	{ 5, 10, 2 },
	
	{ 1, 5, 2 },
	{ 1, 10, 2 },
	
	{ 1, 25, 5 },
	
	{ 1, 5, 1 },
	{ 1, 5, 2 },
	{ 1, 10, 2 },
	
	{ 25, 100, 10 },
	{ 1, 10, 2 },
	{ 5, 20, 2 }
};

int GetCharmAttributeInfo(int type, int info) {
	switch(info) {
		case CH_ATTR_LOW:
		return Charm_Attribute_Info[type].attrib_low;
		case CH_ATTR_HIGH:
		return Charm_Attribute_Info[type].attrib_high;
		case CH_ATTR_LEVELMOD:
		return Charm_Attribute_Info[type].attrib_level_modifier;
	}
	return Charm_Attribute_Info[type].attrib_low;
}

#endif