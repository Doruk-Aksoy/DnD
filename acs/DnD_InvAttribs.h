#ifndef DND_INV_ATTRIBS_IN
#define DND_INV_ATTRIBS_IN

// formula for accuracy:
// every 10 points contribute 0.01%
// applied as follows: X = spread of weapon, X * (1.0 - accuracy * factor_per_point), capped at 80 000 accuracy (80%)

#define DND_ACCURACY_CAP 80000

enum {
	INV_HP_INCREASE,
	INV_ARMOR_INCREASE,
	INV_HPPERCENT_INCREASE,
	INV_ARMORPERCENT_INCREASE,
	INV_SPEED_INCREASE,
	INV_MAGAZINE_INCREASE,
	
	INV_FLATPHYS_DAMAGE,
	INV_FLATENERGY_DAMAGE,
	INV_FLATEXP_DAMAGE,
	INV_FLATMAGIC_DAMAGE,
	INV_FLATELEM_DAMAGE,
	
	INV_SLOT1_DAMAGE,
	INV_SLOT2_DAMAGE,
	INV_SLOT3_DAMAGE,
	INV_SLOT4_DAMAGE,
	INV_SLOT5_DAMAGE,
	INV_SLOT6_DAMAGE,
	INV_SLOT7_DAMAGE,
	INV_SLOT8_DAMAGE,
	INV_TEMPWEP_DAMAGE,
	
	INV_PELLET_INCREASE,
	
	INV_EXPLOSION_RADIUS,
	INV_EXPLOSIVE_RESIST,
	
	INV_AMMOGAIN_CHANCE,
	INV_AMMOGAIN_INCREASE,
	
	INV_REGENCAP_INCREASE,
	
	INV_CRITCHANCE_INCREASE,
	INV_CRITPERCENT_INCREASE,
	INV_CRITDAMAGE_INCREASE,
	
	INV_KNOCKBACK_RESIST,
	INV_DAMAGEPERCENT_INCREASE,
	INV_ACCURACY_INCREASE
};

#define LAST_INV_ATTRIBUTE INV_ACCURACY_INCREASE
#define MAX_INV_ATTRIBUTE_TYPES LAST_INV_ATTRIBUTE + 1

typedef struct {
	int attrib_low;
	int attrib_high;
	int attrib_level_modifier;
} inv_attrib_T;

str Inv_Attribute_Names[MAX_INV_ATTRIBUTE_TYPES] = {
	" Health",
	" Armor",
	"% Health",
	"% Armor",
	"% Speed",
	"% magazine size",
	
	" physical damage to such attacks",
	" energy damage to such attacks",
	" explosive damage to such attacks",
	" magic damage to such attacks",
	" elemental damage to such attacks",
	
	"% slot 1 weapon damage",
	"% slot 2 weapon damage",
	"% slot 3 weapon damage",
	"% slot 4 weapon damage",
	"% slot 5 weapon damage",
	"% slot 6 weapon damage",
	"% slot 7 weapon damage",
	"% slot 8 weapon damage",
	"% temporary weapon damage",
	
	"% more pellets on shells",
	
	"% increased explosion radius",
	"% reduced self explosion damage",
	
	"% chance to gain ammo back on firing",
	"% ammo gain from pickups",
	
	" regeneration cap",
	
	"% crit chance",
	"% increased crit chance",
	"% additional crit damage",
	
	" to knockback resist",
	"% increased damage",
	" to accuracy rating"
};

Inv_attrib_T Inv_Attribute_Info[MAX_INV_ATTRIBUTE_TYPES] = {
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
	
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	{ 1, 8, 2 },
	
	{ 3, 8, 2 },
	
	{ 1, 4, 4 },
	{ 1, 5, 2 },
	
	{ 1, 5, 2 },
	{ 1, 3, 2 },
	
	{ 1, 25, 5 },
	
	{ 1, 5, 1 },
	{ 1, 5, 2 },
	{ 1, 10, 2 },
	
	{ 25, 100, 10 },
	{ 1, 5, 2 },
	{ 4, 125, 20 }
};

#define MAX_MAGAZINES 23
str WeaponMagazineList[MAX_MAGAZINES] = {
	"SawedoffCounter",
	"AkimboClipLeft",
	"AkimboClipRight",
	"BulletSize_6",
	"ShellSize_2",
	"ShellSize_8",
	"ShellSize_8N",
	"ShellSize_10",
	"ShellSize_12",
	"ShellSize_16",
	"ShellSize_18",
	"MGClip",
	"MGClip2",
	"MGClip3",
	"MGClip4",
	"MGClip5",
	"MGClip6",
	"MGClip7",
	"LoadedBasilisk",
	"PCanClip",
	"RiotgunClip",
	"AcidClip",
	"HeavyGLCounter"
};

#endif