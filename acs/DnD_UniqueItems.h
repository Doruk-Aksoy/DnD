#ifndef DND_UNIQUE_IN
#define DND_UNIQUE_IN

// s, m and g indicate charm type
enum {
	// M - 50% increased armor cap, 10-25% reduced elemental damage taken, 2-5% chance of casting a random elemental spell while firing
	UITEM_ELEMENTALBULWARK,
	// G - 5-20 flat physical damage, 10-25 bulkiness, 10-25% reduced physical damage taken, 75-200 regen cap, knockback immunity
	UITEM_IRONBARK,
	// G - Doubles effects of all small charms
	UITEM_WELLOFPOWER,
	// S - 5-10 to all stats, 3-8% chance to be healed for 10-15% of your maximum health on being hit
	UITEM_ANCIENTGEMSTONE,
	// S - 5-25% increased lightning damage, Lightning type attacks always crit
	UITEM_DEATHSPARK,
	// S - 15-25% increased pellets, Shotgun type weapons deal 10-40% more damage
	UITEM_SHELLSHOCK,
	// M - Your health cap is doubled, Can't use armor, Gain 1% damage increase every 50 health
	UITEM_OAKHEART,
	// M - 50% more pellets on shotguns, 200% slot 3 damage, Pellets fire in a circle around you regardless of accuracy
	UITEM_PELLETSTORM,
	// G - 15-25 Intellect, Slain enemies have 10-15% chance to raise a zombie (Max 5, Lasts 15 seconds), Damage taken is shared between all summoned creatures, Take 20-10% more damage
	UITEM_GRAVECALLER,
	// M - 6-10% speed, 2-5 flat damage to all attacks, Killing enemies heals for 1-5% missing health 
	UITEM_LIFELEECH,
	// S - 
};

#define LAST_UNIQUE_ITEM UITEM_LIFELEECH
#define MAX_UNIQUE_ITEMS LAST_UNIQUE_ITEM + 1

str UniqueItemNames[MAX_UNIQUE_ITEMS] = {
	"Elemental Bulwark",
	"Ironbark",
	"Well of Power",
	"Ancient Gemstone",
	"Deathspark",
	"Shellshock",
	"Oakheart",
	"Pellet Storm",
	"Grave Caller",
	"Life Leech",
};

int UniqueItemDropWeight[MAX_UNIQUE_ITEMS] = {
	1,
	1,
	1,
	1,
	1,
	1
};

global inventory_T 63: UniqueItemList[MAX_UNIQUE_ITEMS];

// initializes all uniques
Script "DnD Load Uniques" OPEN {

}

#endif