#ifndef DND_DATABASEROWS_IN
#define DND_DATABASEROWS_IN

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

#define DND_DB_CLASSID "DnD_ClassID"
#define DND_DB_WEAPONINT "DND_PlayerWeapons_"
#define DND_DB_STATS_STR "DND_PlayerStats_1"
#define DND_DB_STATS_DEX "DND_PlayerStats_2"
#define DND_DB_STATS_INT "DND_PlayerStats_3"
#define DND_DB_BACKPACKS "DND_Backpacks"
#define DND_DB_PERK "DND_PlayerPerk_"
#define DND_DB_HEALTH "DND_PlayerHealth"
#define DND_DB_ESHIELD "DnD_PlayerEShield"
#define DND_DB_AMMO "DND_PlayerAmmo_"
#define DND_DB_SPECIALAMMO "DND_PlayerSpecialAmmo_"
#define DND_DB_TEMPAMMO "DND_PlayerTemporaryAmmo"
#define DND_DB_TEMPAMMO_BLADECHARGE "DND_PlayerTemporaryAmmo_BladeCharge"
#define DND_DB_EXP "DND_PlayerEXP"
#define DND_DB_CREDIT "DND_PlayerCredit"
#define DND_DB_LEVEL "DND_PlayerLevel"
#define DND_DB_ABILITY "DND_PlayerAbilities"
#define DND_DB_UNSPENTATTRIB "DND_PlayerUnspentAttribs"
#define DND_DB_UNSPENTPERK "DND_PlayerUnspentPerks"
#define DND_DB_ACTIVEACCESSORIES "DND_SelectedAccessories"
#define DND_DB_ACCESSORIES "DND_PlayerAccessories"
#define DND_DB_ARTIFACTS "DND_PlayerArtifacts"
#define DND_DB_RESEARCH_DONE "DND_PlayerResearch_Done_"
#define DND_DB_RESEARCH_DISCOVERED "DND_PlayerResearch_Discovered_"
#define DND_DB_RESEARCH_INVESTED "DND_PlayerResearch_Invested_"
#define DND_DB_BUDGET "DND_PlayerBudget"
#define DND_DB_QUESTSTATE1 "DnD_PlayerQuest_1"
#define DND_DB_QUESTSTATE2 "DnD_PlayerQuest_2"
#define DND_DB_WEAPONQUALITY "DnD_WeaponQuality_"
#define DND_DB_WEAPONMOD_VAL "DnD_WeaponModVal_"
#define DND_DB_WEAPONMOD_TIER "DnD_WeaponModTier_"
#define DND_DB_STOREDKIT "DnD_StoredKit"
#define DND_DB_SURVIVECOUNT "DnD_PlayerLifeTime"
#define DND_DB_PLAYERWEPCHECKERS "DnD_PlayerWepCheckers"
#define DND_DB_MISC "DnD_PlayerMiscallenous"
#define DND_DB_KEYS "DnD_PlayerChestKeys"
#define DND_DB_RESEARCHTRACKER "DnD_ResearchTracker"
#define DND_DB_NPCTRACKER "DnD_NPCTracker"
#define DND_DB_LEGENDARYTRACKER "DnD_LegendaryKills"
#define DND_DB_ORBRECIPETRACKER_1 "DnD_OrbRecipe_1"
#define DND_DB_ORBRECIPETRACKER_2 "DnD_OrbRecipe_2"

#define DND_DB_WEAPONDISCARDS "DnD_WepDiscards"

#define DND_DB_VOTESKIPS "DnD_Voteskips"
#define DND_DB_TRACKEDMAP "DnD_TrackedMap_"

#define DND_DB_KILLTRACKER "DnD_LifetimeKills"
#define DND_DB_KILLTRACKER_MILLION "DnD_LifetimeKills_Mil"
#define DND_DB_KILLTRACKER_BILLION "DnD_LifetimeKills_Bil"

#define DND_DB_PLAYERINVENTORY "DnD_PlayerInventory_"
#define DND_DB_PLAYERINVENTORYFIELD_WIDTH "Width"
#define DND_DB_PLAYERINVENTORYFIELD_HEIGHT "Height"
#define DND_DB_PLAYERINVENTORYFIELD_IMAGE "Image"
#define DND_DB_PLAYERINVENTORYFIELD_TYPE "Type"
#define DND_DB_PLAYERINVENTORYFIELD_SUBTYPE "SubType"
#define DND_DB_PLAYERINVENTORYFIELD_LEVEL "Level"
#define DND_DB_PLAYERINVENTORYFIELD_STACK "Stack"
#define DND_DB_PLAYERINVENTORYFIELD_ATTRIBCOUNT "AttributeCount"
#define DND_DB_PLAYERINVENTORYFIELD_ATTRIBVAL "AttributeVal"
#define DND_DB_PLAYERINVENTORYFIELD_ATTRIBID "AttributeID"
#define DND_DB_PLAYERINVENTORYFIELD_ATTRIBTIER "AttributeTier"
#define DND_DB_PLAYERINVENTORYFIELD_ATTRIBEXTRA "AttributeExtra"
#define DND_DB_PLAYERINVENTORYFIELD_FRACTURE "IsFractured"
#define DND_DB_PLAYERINVENTORYFIELD_QUALITY "Quality"
#define DND_DB_PLAYERINVENTORYFIELD_CORRUPTED "AttributeCorruption"
#define DND_DB_PLAYERINVENTORYFIELD_IMPLICITVAL "ImplicitVal"
#define DND_DB_PLAYERINVENTORYFIELD_IMPLICITID "ImplicitID"
#define DND_DB_PLAYERINVENTORYFIELD_IMPLICITTIER "ImplicitTier"
#define DND_DB_PLAYERINVENTORYFIELD_IMPLICITEXTRA "ImplicitExtra"

#define DND_DB_CHARMUSED "CharmsUsed_"

#define DND_DB_STASHPAGE "StashPage_"
#define DND_DB_STASH_PAGES "DnD_PlayerStashPages"

#ifdef ISAPRILFIRST
	#define DND_DB_NFT "NFT"
#endif

#endif
