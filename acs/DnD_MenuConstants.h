#ifndef DND_MENUCONSTANTS_IN
#define DND_MENUCONSTANTS_IN

#include "DnD_Ammo.h"
#include "DnD_SpecialAmmo.h"

#define DND_QUICKBUY_POPUPID 0xFFFF
#define DND_LANGUAGE_LOOKUP true
#define DND_NOLOOKUP false

// Box definitions for clickables
// 7 images on menu leftmost bar
#define MAX_MAIN_BOXES 10

// Mainbox Labels
enum {
	MAINBOX_NONE = 0,
	MAINBOX_STATS,
	MAINBOX_PERK,
	MAINBOX_LOAD,
	MAINBOX_SHOP,
	MAINBOX_RESEARCH,
	MAINBOX_ABILITY,
	MAINBOX_HELP,
	MAINBOX_LARR,
	MAINBOX_RET,
	MAINBOX_RARR
};

// Various box enums (I know this part is terrible, dk how to make it look better atm)
// Definition rule: L -> R, T -> B
enum {
	MBOX_1 = 1,
	MBOX_2,
	MBOX_3,
	MBOX_4,
	MBOX_5,
	MBOX_6,
	MBOX_7,
	MBOX_8,
	MBOX_9,
	MBOX_10,
	MBOX_11,
	MBOX_12,
	MBOX_13,
	MBOX_14,
	MBOX_15,
	MBOX_16
};

// Page definitions
enum { 
	MENU_NULL,
	MENU_STAT1,
	MENU_STAT2,
	MENU_STAT3,
	
	MENU_PERK,
	
	MENU_LOAD,
	MENU_LOAD1,
	MENU_LOAD_INVENTORY,
	MENU_LOAD_STASH,
	MENU_LOAD_CHARM,
	MENU_LOAD_CRAFTING,
	MENU_LOAD_CRAFTING_WEAPON,
	MENU_LOAD_CRAFTING_INVENTORY,
	MENU_LOAD_ACC1,
	MENU_LOAD_ACC2,
	MENU_LOAD_ACC3,
	MENU_LOAD_ACC4,
	
	MENU_LOAD_TRADE1,
	MENU_LOAD_TRADE2,
	MENU_LOAD_TRADE3,
	MENU_LOAD_TRADE4,
	MENU_LOAD_TRADE5,
	
	MENU_SHOP,
	MENU_SHOP_WEAPON,
	MENU_SHOP_WEAPON1,
	MENU_SHOP_WEAPON2,
	MENU_SHOP_WEAPON3_1,
	MENU_SHOP_WEAPON3_2,
	MENU_SHOP_WEAPON4_1,
	MENU_SHOP_WEAPON4_2,
	MENU_SHOP_WEAPON5_1,
	MENU_SHOP_WEAPON5_2,
	MENU_SHOP_WEAPON6_1,
	MENU_SHOP_WEAPON6_2,
	MENU_SHOP_WEAPON7,
	MENU_SHOP_WEAPON8,
	
	MENU_SHOP_AMMOSELECT,
	// the following 4 are for ammo categories
	MENU_SHOP_AMMO_1,
	MENU_SHOP_AMMO_1_2,
	MENU_SHOP_AMMO_2,
	MENU_SHOP_AMMO_2_2,
	MENU_SHOP_AMMO_3,
	MENU_SHOP_AMMO_4,
	MENU_SHOP_AMMO_SPECIAL1,
	
	MENU_SHOP_ABILITY_1,
	MENU_SHOP_ABILITY_2,
	
	MENU_SHOP_ARTIFACT,
	
	MENU_SHOP_ARMOR1,
	MENU_SHOP_ARMOR2,
	
	MENU_SHOP_ACCOUNT,
	
	MENU_RESEARCH,
	MENU_RESEARCH_GUNS, // this leads to a subsection, add subsections below this
	MENU_RESEARCH_BODY,
	MENU_RESEARCH_AMMO,
	MENU_RESEARCH_SLOTGUNS,
	MENU_RESEARCH_LUXURYGUNS,
	MENU_RESEARCH_UTILITY,
	MENU_MAIN,
	
	MENU_HELP,
	MENU_HELP_CHARACTER,
	MENU_HELP_CLASSPERKS,
	MENU_HELP_WEAPONPROPS,
	MENU_HELP_RESEARCHES,
	MENU_HELP_DAMAGETYPES,
	MENU_HELP_ORBS,
	MENU_HELP_MMODS,
	MENU_HELP_LEGMONS,
	
	MENU_HELP_MMODS_WEAKNESS,
	MENU_HELP_MMODS_RESIST,
	MENU_HELP_MMODS_IMMUNITY,
	MENU_HELP_MMODS_AGGRESSIVE,
	MENU_HELP_MMODS_DEFENSIVE,
	MENU_HELP_MMODS_UTILITY,
	MENU_HELP_MMODS_SPECIAL,

	MENU_ABILITY
};

enum {
	// wep slot 1
		SHOP_WEP_CSAW = 0,
		SHOP_WEP_KATANA,
		SHOP_WEP_EXCALIBAT,
		SHOP_WEP_INFERNOSWORD,
		SHOP_WEP_DUSKBLADE,
		SHOP_WEP_SICKLE,

	// wep slot 2
		SHOP_WEP_AKIMBOPISTOL,
		SHOP_WEP_MAGNUM,
		SHOP_WEP_LASERPISTOL,
		SHOP_WEP_RESPIS1,
		SHOP_WEP_RESPIS2,
		SHOP_WEP_RUBYWAND,
		SHOP_WEP_SCATTERPISTOL,

	// wep slot 3 - 1
		SHOP_WEP_PURIFIER,
		SHOP_WEP_AUTOSG,
		SHOP_WEP_EMERALDWAND,
		SHOP_WEP_RESSG1,
		SHOP_WEP_RESSG2,
		SHOP_WEP_RESSG3,
		SHOP_WEP_RESSG4,
		SHOP_WEP_HSSG,
		
	// wep slot 3 - 2
		SHOP_WEP_ERASUS,
		SHOP_WEP_HELLSMAW,
		SHOP_WEP_RESSSG1,
		SHOP_WEP_RESSSG2,
		SHOP_WEP_RESSSG3,
		SHOP_WEP_RESSSG4,
		SHOP_WEP_SILVER,
		SHOP_WEP_SLAYER,

	// wep slot 4 - 1
		SHOP_WEP_HMG,
		SHOP_WEP_LEAD,
		SHOP_WEP_DSEAL,
		SHOP_WEP_RESMG1,
		SHOP_WEP_RESMG2,
		SHOP_WEP_RESMG3,
		SHOP_WEP_RESMG4,
		SHOP_WEP_RESMG5,
		
	// wep slot 4 - 2
		SHOP_WEP_DESOLATOR,
		SHOP_WEP_MINIGUN,
		SHOP_WEP_EBONY,
		SHOP_WEP_MPPB,

	// wep slot 5 - 1
		SHOP_WEP_TORPEDO,
		SHOP_WEP_MERC,
		SHOP_WEP_VINDICATOR,
		SHOP_WEP_RESRL1,
		SHOP_WEP_RESRL2,
		SHOP_WEP_RESRL3,
		SHOP_WEP_RESRL4,
		SHOP_WEP_RESRL5,
		
	// wep slot 5 - 2
		SHOP_WEP_GRENADE,
		SHOP_WEP_ROTARYGL,
		SHOP_WEP_HEAVYML,
		SHOP_WEP_SEDRIN,

	// wep slot 6 - 1
		SHOP_WEP_NUCLEARPL,
		SHOP_WEP_TUREL,
		SHOP_WEP_FROSTFANG,
		SHOP_WEP_RESPL1,
		SHOP_WEP_RESPL2,
		SHOP_WEP_RESPL3,
		SHOP_WEP_RESPL4,
		
	// wep slot 6 - 2
		SHOP_WEP_RHINO,
		SHOP_WEP_NAIL,
		SHOP_WEP_BASILISK,

	// wep slot 7
		SHOP_WEP_BFG,
		SHOP_WEP_DEVA,
		SHOP_WEP_MFG,
		SHOP_WEP_RESBFG1,
		SHOP_WEP_RESBFG2,
		
		SHOP_WEP_GAUSS,
		SHOP_WEP_RAIL,
		SHOP_WEP_DEATHRAY,

	// wep slot 8
		SHOP_WEP_DEATHSTAFF,
		SHOP_WEP_RAZOR,
		SHOP_WEP_SUN,
		SHOP_WEP_REAVER,

	// ammo list
		SHOP_AMMO_CLIP,
		SHOP_AMMO_SHELL,
		SHOP_AMMO_ROCKET,
		SHOP_AMMO_CELL,
		SHOP_AMMO_EXPSHELL,
		SHOP_AMMO_EBONY,
		SHOP_AMMO_EBONYX,
		SHOP_AMMO_MIS,
		SHOP_AMMO_GL,
		SHOP_AMMO_NAIL,
		
		SHOP_AMMO_BASILISK,
		SHOP_AMMO_GAUSS,
		SHOP_AMMO_SLAYER,
		SHOP_AMMO_RUBY,
		SHOP_AMMO_PCAN,
		SHOP_AMMO_RIOTSHELL,
		SHOP_AMMO_METEOR,
		SHOP_AMMO_FUEL,
		SHOP_AMMO_LG,
		SHOP_AMMO_NITROGEN,
		
		SHOP_AMMO_ION,
		SHOP_AMMO_ACID,
		SHOP_AMMO_EVERICE,
		SHOP_AMMO_VIPER,
		SHOP_AMMO_THUNDER,
		SHOP_AMMO_DSEAL,
		SHOP_AMMO_FUSION,
		SHOP_AMMO_FLAK,
		SHOP_AMMO_DESOLATOR,
		SHOP_AMMO_HADES,
		
	// ammo 4
		SHOP_AMMO_DEMONBLOOD,
		SHOP_AMMO_EMERALDMANA,
		SHOP_AMMO_HELLSMAW,
		SHOP_AMMO_DEVASTATOR,
		SHOP_AMMO_HEAVYGRENADE,
		SHOP_AMMO_FLAYER,
		SHOP_AMMO_GRAVDIS,
		SHOP_AMMO_SEDRIN,
		SHOP_AMMO_CHARON,
		SHOP_AMMO_INCINERATOR,

	// ammo special
		SHOP_AMMO_FLECHETTE,
		SHOP_AMMO_PIERCING,
		SHOP_AMMO_ELECTRIC,
		SHOP_AMMO_NITROSHELL,
		SHOP_AMMO_SLUGSHELL,
		SHOP_AMMO_SONICGRENADE,
		SHOP_AMMO_HEGRENADE,

	// abilities
		SHOP_ABILITY_KICK,
		SHOP_ABILITY_RELOAD,
		SHOP_ABILITY_DASH,
		SHOP_ABILITY_ARCANE,
		SHOP_ABILITY_POISON,
		SHOP_ABILITY_EXPLOSION,
		SHOP_ABILITY_HEART,
		SHOP_ABILITY_REGEN,
		SHOP_ABILITY_TEMPORAL,
		SHOP_ABILITY_SOUL,

	// armor 1
		SHOP_ARMOR_GREEN,
		SHOP_ARMOR_YELLOW,
		SHOP_ARMOR_BLUE,
		SHOP_ARMOR_RED,
		SHOP_ARMOR_GUNSLINGER,
		SHOP_ARMOR_OCCULT,
		SHOP_ARMOR_DEMO,
		SHOP_ARMOR_ENERGY,
		SHOP_ARMOR_ELEMENTAL,

	// armor 2
		SHOP_ARMOR_MONOLITH,
		SHOP_ARMOR_CYBERNETIC,
		SHOP_ARMOR_DUELIST,
		SHOP_ARMOR_NECRO,
		SHOP_ARMOR_KNIGHT,
		SHOP_ARMOR_RAVAGER,
		SHOP_ARMOR_SYNTHMETAL,
		
	// account
		SHOP_ACCOUNT_STASHTAB,

	// arti
		SHOP_ARTI_KIT,
		SHOP_ARTI_SALVATE,
		SHOP_ARTI_SHIELD,
		SHOP_ARTI_BLOOD,
		SHOP_ARTI_TRIPLE,
		SHOP_ARTI_VORTEX,
		SHOP_ARTI_BOOK,
		SHOP_ARTI_MAP,
		SHOP_ARTI_BACKPACK,
		SHOP_ARTI_RESET
};

#define MAXSHOPITEMS (SHOP_ARTI_RESET + 1)

#define SHOP_FIRSTAMMO_PAGE MENU_SHOP_AMMO_1
#define SHOP_FIRSTAMMO_INDEX SHOP_AMMO_CLIP
#define SHOP_FIRSTARMOR_INDEX SHOP_ARMOR_GREEN
#define SHOP_FIRSTARMOR2_INDEX SHOP_ARMOR_MONOLITH
#define SHOP_ARMORPAGE1_BEGIN SHOP_FIRSTARMOR_INDEX
#define SHOP_ARMORPAGE2_BEGIN SHOP_FIRSTARMOR2_INDEX
#define SHOP_FIRSTAMMOSPECIAL_INDEX SHOP_AMMO_FLECHETTE
#define SHOP_FIRSTAMMOSPECIAL_GRENADE_INDEX SHOP_AMMO_SONICGRENADE
#define SHOP_FIRSTARTI_INDEX SHOP_ARTI_KIT
#define SHOP_ABILITY1_BEGIN SHOP_ABILITY_KICK
#define SHOP_ABILITY2_BEGIN SHOP_ABILITY_TEMPORAL
#define SHOP_ACCOUNT_BEGIN SHOP_ACCOUNT_STASHTAB

#define SHOP_WEAPON_BEGIN SHOP_WEP_CSAW
#define SHOP_WEAPON1_BEGIN SHOP_WEP_CSAW
#define SHOP_WEAPON2_BEGIN SHOP_WEP_AKIMBOPISTOL
#define SHOP_WEAPON31_BEGIN SHOP_WEP_PURIFIER
#define SHOP_WEAPON32_BEGIN SHOP_WEP_ERASUS
#define SHOP_WEAPON41_BEGIN SHOP_WEP_HMG
#define SHOP_WEAPON42_BEGIN SHOP_WEP_DESOLATOR
#define SHOP_WEAPON51_BEGIN SHOP_WEP_TORPEDO
#define SHOP_WEAPON52_BEGIN SHOP_WEP_GRENADE
#define SHOP_WEAPON61_BEGIN SHOP_WEP_NUCLEARPL
#define SHOP_WEAPON62_BEGIN SHOP_WEP_RHINO
#define SHOP_WEAPON7_BEGIN SHOP_WEP_BFG
#define SHOP_WEAPON8_BEGIN SHOP_WEP_DEATHSTAFF

#define SHOP_WEAPON_SLOT1END SHOP_WEP_SICKLE
#define SHOP_WEAPON_SLOT2END SHOP_WEP_SCATTERPISTOL
#define SHOP_WEAPON_SLOT31END SHOP_WEP_HSSG
#define SHOP_WEAPON_SLOT32END SHOP_WEP_SLAYER
#define SHOP_WEAPON_SLOT3END SHOP_WEP_SLAYER
#define SHOP_WEAPON_SLOT41END SHOP_WEP_RESMG5
#define SHOP_WEAPON_SLOT42END SHOP_WEP_MPPB
#define SHOP_WEAPON_SLOT51END SHOP_WEP_RESRL5
#define SHOP_WEAPON_SLOT52END SHOP_WEP_SEDRIN
#define SHOP_WEAPON_SLOT61END SHOP_WEP_RESPL4
#define SHOP_WEAPON_SLOT62END SHOP_WEP_BASILISK
#define SHOP_WEAPON_SLOT7END SHOP_WEP_DEATHRAY
#define SHOP_WEAPON_SLOT8END SHOP_WEP_REAVER

// Weapon slot stuff

#define SHOP_WEAPON1CSAW_BEGIN SHOP_WEP_CSAW
#define SHOP_WEAPON1CSAW_END SHOP_WEP_INFERNOSWORD
#define SHOP_WEAPON2PISTOL_BEGIN SHOP_WEP_AKIMBOPISTOL
#define SHOP_WEAPON2PISTOL_END SHOP_WEP_RESPIS2
#define SHOP_WEAPON3SG_BEGIN SHOP_WEP_PURIFIER
#define SHOP_WEAPON3SG_END SHOP_WEP_RESSG3
#define SHOP_WEAPON3SSG_BEGIN SHOP_WEP_HSSG
#define SHOP_WEAPON3SSG_END SHOP_WEP_RESSSG4
#define SHOP_WEAPON4CG_BEGIN SHOP_WEP_HMG
#define SHOP_WEAPON4CG_END SHOP_WEP_RESMG5
#define SHOP_WEAPON5RL_BEGIN SHOP_WEP_TORPEDO
#define SHOP_WEAPON5RL_END SHOP_WEP_RESRL3
#define SHOP_WEAPON6PL_BEGIN SHOP_WEP_NUCLEARPL
#define SHOP_WEAPON6PL_END SHOP_WEP_RESPL4
#define SHOP_WEAPON7BFG_BEGIN SHOP_WEP_BFG
#define SHOP_WEAPON7BFG_END SHOP_WEP_RESBFG2

#define SHOP_LASTAMMO_PAGE MENU_SHOP_AMMO_SPECIAL1
#define SHOP_LASTAMMO_SPECIALINDEX SHOP_AMMO_HEGRENADE
#define SHOP_LASTAMMO_NORMALINDEX SHOP_AMMO_INCINERATOR
#define SHOP_LASTWEP_INDEX SHOP_WEP_REAVER
#define SHOP_ABILITY1_END SHOP_ABILITY_REGEN
#define SHOP_LASTABILITY_INDEX SHOP_ABILITY_SOUL
#define SHOP_LASTARMOR_INDEX SHOP_ARMOR_SYNTHMETAL
#define SHOP_LASTARTI_INDEX SHOP_ARTI_RESET
#define SHOP_LASTDRAWNARTI_INDEX SHOP_ARTI_BOOK
#define SHOP_ARMORPAGE1_END SHOP_ARMOR_ELEMENTAL

#define SHOP_RESPAGE_BEGIN MENU_RESEARCH_BODY
#define SHOP_RESPAGE_END MENU_RESEARCH_UTILITY

#define MENU_FIRST_ACCESSORY_PAGE MENU_LOAD_ACC1
#define MENU_LAST_ACCESSORY_PAGE MENU_LOAD_ACC4

#define PAGE1_ARMOR_COUNT (SHOP_FIRSTARMOR2_INDEX - SHOP_FIRSTARMOR_INDEX)
#define PAGE2_ARMOR_COUNT (SHOP_LASTARMOR_INDEX - SHOP_FIRSTARMOR2_INDEX + 1)
#define DND_ARMORFILL_FACTOR 100

#define SHOP_ACCOUNT_END SHOP_ACCOUNT_STASHTAB

#define MENU_MAXRES_PAGES (SHOP_RESPAGE_END - SHOP_RESPAGE_BEGIN + 1)

#define MAXSHOPWEAPONS (SHOP_LASTWEP_INDEX - SHOP_WEAPON_BEGIN + 1)
#define MAXSHOPNORMALAMMOS (SHOP_LASTAMMO_NORMALINDEX - SHOP_FIRSTAMMO_INDEX + 1)
#define MAXSHOPAMMOS (SHOP_LASTAMMO_SPECIALINDEX - SHOP_FIRSTAMMO_INDEX + 1)
#define MAXABILITIES (SHOP_LASTABILITY_INDEX - SHOP_ABILITY1_BEGIN + 1)
#define MAXARTIFACTS (SHOP_LASTARTI_INDEX - SHOP_FIRSTARTI_INDEX + 1)

#define MENU_HELP_BEGIN MENU_HELP_CHARACTER
#define MENU_HELP_END MENU_HELP_LEGMONS

#define MENU_MMOD_HELP_BEGIN MENU_HELP_MMODS_WEAKNESS
#define MENU_MMOD_HELP_END MENU_HELP_MMODS_SPECIAL

#define AMMO_ICON 0
#define AMMO_NAME 1	
#define AMMO_PAGE2_BEGIN (SHOP_FIRSTAMMO2_INDEX - SHOP_FIRSTAMMO_INDEX)
#define AMMO_PAGE3_BEGIN (SHOP_FIRSTAMMO3_INDEX - SHOP_FIRSTAMMO_INDEX)
#define AMMO_PAGESPECIAL_BEGIN (SHOP_FIRSTAMMOSPECIAL_INDEX - SHOP_FIRSTAMMO_INDEX)

#define MENU_LAST_TRADE MENU_LOAD_TRADE5
#define MENU_FIRST_TRADE MENU_LOAD_TRADE1
#define MENU_TRADE_PAGECOUNT (MENU_LAST_TRADE - MENU_FIRST_TRADE + 1)
#define MENU_TRADE_PLAYERCOUNT 16

#define MENU_LOAD_CRAFTING_FIRST MENU_LOAD_CRAFTING_WEAPON
#define MENU_LOAD_CRAFTING_LAST MENU_LOAD_CRAFTING_INVENTORY

#define MAXACCOUNTITEMS (SHOP_ACCOUNT_END - SHOP_ACCOUNT_BEGIN + 1)

str AbilityInfo[MAXABILITIES] = {
	"Ability_Kick",
	"Ability_Reloader",
	"Ability_Dash",
	"Ability_Arcanery",
	"Ability_AntiPoison",
	"Ability_ExplosionMastery",
	"Ability_HeartSeeker",
	"Ability_Regeneration",
	"Ability_Temporal",
	"Ability_SoulStealer"
};

#define ARTI_ICON 0
#define ARTI_NAME 1
str ArtifactInfo[MAXARTIFACTS][2] = { 
	{ "FKITD0",				"FieldKit"				},	
	{ "SALVRICO",			"SalvationSphere"		},
	{ "SHIELICO",			"PortableShield"		},
	{ "BRUNICO",			"BloodRune"				},
	{ "DMGICO",			    "TripleDamage"			},
	{ "SHRUA0",			    "BladeVortex"			},
	{ "BOTDICO",		    "BookOfTheDead"		    },
	{ "PMAPA0",			    "AllMapOnlyOnce"		},
	{ "BPAKA0",			    "NewBackPack"			},
	{ "RESETICO",           "StatReset"             }
};

#define MAX_ACCESSORIES (DND_ACCESSORY_LICHARM + 1)

#endif
