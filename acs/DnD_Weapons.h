#ifndef DND_WEAPONS_IN
#define DND_WEAPONS_IN

#include "DnD_WeaponDefs.h"
#include "DnD_Attack.h"

#define MAX_WEAPON_STRINGS (WEAPON_ICON + 1)

typedef struct wep_data {
	int ammo_use1;
	int ammo_use2;
	int properties;
	str name;
	str ammo_name1;
	str ammo_name2;
	str icon;
} wep_data_T;

global wep_data_T 31: Weapons_Data[MAXWEPS];

void SetupWeaponData() {
	// SLOT 1
	Weapons_Data[DND_WEAPON_FIST].name = " Fist ";
	Weapons_Data[DND_WEAPON_FIST].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_FIST].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_FIST].icon = "WEPICO1";
	Weapons_Data[DND_WEAPON_FIST].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_FIST].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_FIST].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_CHAINSAW].name = " Chainsaw ";
	Weapons_Data[DND_WEAPON_CHAINSAW].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_CHAINSAW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_CHAINSAW].icon = "WEPICO2";
	Weapons_Data[DND_WEAPON_CHAINSAW].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_CHAINSAW].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_CHAINSAW].properties = WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].name = "Upgraded Chainsaw";
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].icon = "WEPICO3";
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DOUBLECHAINSAW].properties = WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_KATANA].name = "Katana";
	Weapons_Data[DND_WEAPON_KATANA].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_KATANA].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_KATANA].icon = "WEPICO4";
	Weapons_Data[DND_WEAPON_KATANA].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_KATANA].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_KATANA].properties = WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_EXCALIBAT].name = "Excalibat";
	Weapons_Data[DND_WEAPON_EXCALIBAT].ammo_name1 = "BatCharge";
	Weapons_Data[DND_WEAPON_EXCALIBAT].ammo_name2 = "Souls";
	Weapons_Data[DND_WEAPON_EXCALIBAT].icon = "WEPICO5";
	Weapons_Data[DND_WEAPON_EXCALIBAT].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_EXCALIBAT].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_EXCALIBAT].properties = WPROP_CANTHITGHOST | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_INFERNOSWORD].name = "ResMelee1";
	Weapons_Data[DND_WEAPON_INFERNOSWORD].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_INFERNOSWORD].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_INFERNOSWORD].icon = "WEPICO6";
	Weapons_Data[DND_WEAPON_INFERNOSWORD].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_INFERNOSWORD].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_INFERNOSWORD].properties = WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_DUSKBLADE].name = "Dusk Blade";
	Weapons_Data[DND_WEAPON_DUSKBLADE].ammo_name1 = "SwordHitCharge";
	Weapons_Data[DND_WEAPON_DUSKBLADE].ammo_name2 = "Souls";
	Weapons_Data[DND_WEAPON_DUSKBLADE].icon = "WEPICO7";
	Weapons_Data[DND_WEAPON_DUSKBLADE].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_DUSKBLADE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DUSKBLADE].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_SICKLE].name = "Sickle";
	Weapons_Data[DND_WEAPON_SICKLE].ammo_name1 = "";
	Weapons_Data[DND_WEAPON_SICKLE].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SICKLE].icon = "WEPICO8";
	Weapons_Data[DND_WEAPON_SICKLE].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_SICKLE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SICKLE].properties = WPROP_IRREDUCIBLE;
	
	// SLOT 2
	Weapons_Data[DND_WEAPON_PISTOL].name = " Pistol ";
	Weapons_Data[DND_WEAPON_PISTOL].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_PISTOL].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_PISTOL].icon = "WEPICO9";
	Weapons_Data[DND_WEAPON_PISTOL].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PISTOL].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_PISTOL].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].name = " Akimbo Pistols ";
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].icon = "WEPICO10";
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_AKIMBOPISTOL].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_MAGNUM].name = "Magnum";
	Weapons_Data[DND_WEAPON_MAGNUM].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_MAGNUM].ammo_name2 = "BulletSize_6";
	Weapons_Data[DND_WEAPON_MAGNUM].icon = "WEPICO11";
	Weapons_Data[DND_WEAPON_MAGNUM].ammo_use1 = 0;
	Weapons_Data[DND_WEAPON_MAGNUM].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_MAGNUM].properties = WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_LASERPISTOL].name = "Laser Pistol";
	Weapons_Data[DND_WEAPON_LASERPISTOL].ammo_name1 = "LPistolCharge";
	Weapons_Data[DND_WEAPON_LASERPISTOL].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_LASERPISTOL].icon = "WEPICO12";
	Weapons_Data[DND_WEAPON_LASERPISTOL].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_LASERPISTOL].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_LASERPISTOL].properties = WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].name = "ResPistol1";
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].ammo_name2 = "MGClip5";
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].icon = "WEPICO13";
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_ASSAULTRIFLE].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_VIPERSTAFF].name = "ResPistol2";
	Weapons_Data[DND_WEAPON_VIPERSTAFF].ammo_name1 = "ViperAmmo";
	Weapons_Data[DND_WEAPON_VIPERSTAFF].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_VIPERSTAFF].icon = "WEPICO14";
	Weapons_Data[DND_WEAPON_VIPERSTAFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_VIPERSTAFF].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_VIPERSTAFF].properties = WPROP_IGNORESHIELD | WPROP_POISON;
	
	Weapons_Data[DND_WEAPON_RUBYWAND].name = "RubyWand";
	Weapons_Data[DND_WEAPON_RUBYWAND].ammo_name1 = "RubyAmmo";
	Weapons_Data[DND_WEAPON_RUBYWAND].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_RUBYWAND].icon = "WEPICO15";
	Weapons_Data[DND_WEAPON_RUBYWAND].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RUBYWAND].ammo_use2 = 7;
	Weapons_Data[DND_WEAPON_RUBYWAND].properties = WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_SCATTERGUN].name = "ScatterGun";
	Weapons_Data[DND_WEAPON_SCATTERGUN].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_SCATTERGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SCATTERGUN].icon = "WEPICO16";
	Weapons_Data[DND_WEAPON_SCATTERGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SCATTERGUN].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_SCATTERGUN].properties = WPROP_IGNORESHIELD | WPROP_IRREDUCIBLE;
	
	// SLOT 3 - SHOTGUNS
	Weapons_Data[DND_WEAPON_SHOTGUN].name = " Shotgun ";
	Weapons_Data[DND_WEAPON_SHOTGUN].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_SHOTGUN].ammo_name2 = "ShellSize_8N";
	Weapons_Data[DND_WEAPON_SHOTGUN].icon = "WEPICO17";
	Weapons_Data[DND_WEAPON_SHOTGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SHOTGUN].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_SHOTGUN].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_PURIFIER].name = "Upgraded Shotgun";
	Weapons_Data[DND_WEAPON_PURIFIER].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_PURIFIER].ammo_name2 = "ShellSize_8N";
	Weapons_Data[DND_WEAPON_PURIFIER].icon = "WEPICO18";
	Weapons_Data[DND_WEAPON_PURIFIER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PURIFIER].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_PURIFIER].properties = WPROP_ALTAMMO;
	
	Weapons_Data[DND_WEAPON_KILLSTORM].name = "Upgraded Shotgun2";
	Weapons_Data[DND_WEAPON_KILLSTORM].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_KILLSTORM].ammo_name2 = "ShellSize_10";
	Weapons_Data[DND_WEAPON_KILLSTORM].icon = "WEPICO19";
	Weapons_Data[DND_WEAPON_KILLSTORM].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_KILLSTORM].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_KILLSTORM].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_EMERALDWAND].name = "Upgraded Shotgun3";
	Weapons_Data[DND_WEAPON_EMERALDWAND].ammo_name1 = "EmeraldMana";
	Weapons_Data[DND_WEAPON_EMERALDWAND].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_EMERALDWAND].icon = "WEPICO20";
	Weapons_Data[DND_WEAPON_EMERALDWAND].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_EMERALDWAND].ammo_use2 = 8;
	Weapons_Data[DND_WEAPON_EMERALDWAND].properties = WPROP_CANTHITGHOST | WPROP_POISON;
	
	Weapons_Data[DND_WEAPON_DEADLOCK].name = "ResShotgun1";
	Weapons_Data[DND_WEAPON_DEADLOCK].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_DEADLOCK].ammo_name2 = "ShellSize_12";
	Weapons_Data[DND_WEAPON_DEADLOCK].icon = "WEPICO21";
	Weapons_Data[DND_WEAPON_DEADLOCK].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEADLOCK].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_DEADLOCK].properties = WPROP_ALTAMMO;
	
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].name = "ResShotgun2";
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].ammo_name1 = "NitrogenCanister";
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].icon = "WEPICO22";
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_NITROGENCROSSBOW].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].name = "ResShotgun3";
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].ammo_name1 = "DemonBlood";
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].icon = "WEPICO23";
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_WHEELOFTORMENT].properties = WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_CHARONBLASTER].name = "ResShotgun4";
	Weapons_Data[DND_WEAPON_CHARONBLASTER].ammo_name1 = "CharonAmmo";
	Weapons_Data[DND_WEAPON_CHARONBLASTER].ammo_name2 = "CharonStacks";
	Weapons_Data[DND_WEAPON_CHARONBLASTER].icon = "WEPICO93";
	Weapons_Data[DND_WEAPON_CHARONBLASTER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_CHARONBLASTER].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_CHARONBLASTER].properties = WPROP_NONE;
	
	// SLOT 3 - SUPER SHOTGUNS
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].name = " Super Shotgun ";
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].icon = "WEPICO24";
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SUPERSHOTGUN].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].name = "Upgraded Super Shotgun";
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].icon = "WEPICO25";
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].properties = WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_ERASUS].name = "Upgraded Super Shotgun2";
	Weapons_Data[DND_WEAPON_ERASUS].ammo_name1 = "Shell";
	Weapons_Data[DND_WEAPON_ERASUS].ammo_name2 = "ShellSize_2";
	Weapons_Data[DND_WEAPON_ERASUS].icon = "WEPICO26";
	Weapons_Data[DND_WEAPON_ERASUS].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ERASUS].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_ERASUS].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_HELLSMAW].name = "Upgraded Super Shotgun3";
	Weapons_Data[DND_WEAPON_HELLSMAW].ammo_name1 = "HellsMawAmmo";
	Weapons_Data[DND_WEAPON_HELLSMAW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_HELLSMAW].icon = "WEPICO27";
	Weapons_Data[DND_WEAPON_HELLSMAW].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HELLSMAW].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_HELLSMAW].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_PLASMACANNON].name = "ResSSG1";
	Weapons_Data[DND_WEAPON_PLASMACANNON].ammo_name1 = "PCanAmmo";
	Weapons_Data[DND_WEAPON_PLASMACANNON].ammo_name2 = "PCanClip";
	Weapons_Data[DND_WEAPON_PLASMACANNON].icon = "WEPICO28";
	Weapons_Data[DND_WEAPON_PLASMACANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PLASMACANNON].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_PLASMACANNON].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_SHOCKER].name = "ResSSG2";
	Weapons_Data[DND_WEAPON_SHOCKER].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_SHOCKER].ammo_name2 = "IonOverheat";
	Weapons_Data[DND_WEAPON_SHOCKER].icon = "WEPICO29";
	Weapons_Data[DND_WEAPON_SHOCKER].ammo_use1 = 5;
	Weapons_Data[DND_WEAPON_SHOCKER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SHOCKER].properties = WPROP_IGNORESHIELD | WPROP_OVERHEAT;
	
	Weapons_Data[DND_WEAPON_HADES].name = "ResSSG3";
	Weapons_Data[DND_WEAPON_HADES].ammo_name1 = "HadesAmmo";
	Weapons_Data[DND_WEAPON_HADES].ammo_name2 = "ShellSize_16";
	Weapons_Data[DND_WEAPON_HADES].icon = "WEPICO30";
	Weapons_Data[DND_WEAPON_HADES].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_HADES].ammo_use2 = 2;
	Weapons_Data[DND_WEAPON_HADES].properties = WPROP_RIPPER | WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_CROSSBOW].name = "ResSSG4";
	Weapons_Data[DND_WEAPON_CROSSBOW].ammo_name1 = "FlayerAmmo";
	Weapons_Data[DND_WEAPON_CROSSBOW].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_CROSSBOW].icon = "WEPICO90";
	Weapons_Data[DND_WEAPON_CROSSBOW].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_CROSSBOW].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_CROSSBOW].properties = WPROP_RIPPER | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_SILVERGUN].name = "Silver Gun";
	Weapons_Data[DND_WEAPON_SILVERGUN].ammo_name1 = "ExplodingShell";
	Weapons_Data[DND_WEAPON_SILVERGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SILVERGUN].icon = "WEPICO31";
	Weapons_Data[DND_WEAPON_SILVERGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SILVERGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SILVERGUN].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_SLAYER].name = "Slayer";
	Weapons_Data[DND_WEAPON_SLAYER].ammo_name1 = "SlayerAmmo";
	Weapons_Data[DND_WEAPON_SLAYER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SLAYER].icon = "WEPICO32";
	Weapons_Data[DND_WEAPON_SLAYER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SLAYER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SLAYER].properties = WPROP_SELFDMG | WPROP_CANTHITGHOST | WPROP_RIPPER;
	
	// SLOT 4
	Weapons_Data[DND_WEAPON_MACHINEGUN].name = " Machine gun ";
	Weapons_Data[DND_WEAPON_MACHINEGUN].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_MACHINEGUN].ammo_name2 = "MGClip";
	Weapons_Data[DND_WEAPON_MACHINEGUN].icon = "WEPICO33";
	Weapons_Data[DND_WEAPON_MACHINEGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_MACHINEGUN].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_MACHINEGUN].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].name = "Upgraded Machine Gun";
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].ammo_name2 = "MGClip2";
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].icon = "WEPICO34";
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_HEAVYMACHINEGUN].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_LEADSPITTER].name = "Upgraded Machine Gun2";
	Weapons_Data[DND_WEAPON_LEADSPITTER].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_LEADSPITTER].ammo_name2 = "MGClip3";
	Weapons_Data[DND_WEAPON_LEADSPITTER].icon = "WEPICO35";
	Weapons_Data[DND_WEAPON_LEADSPITTER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_LEADSPITTER].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_LEADSPITTER].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_DEMONSEALER].name = "Upgraded Machine Gun3";
	Weapons_Data[DND_WEAPON_DEMONSEALER].ammo_name1 = "DsealAmmo";
	Weapons_Data[DND_WEAPON_DEMONSEALER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DEMONSEALER].icon = "WEPICO36";
	Weapons_Data[DND_WEAPON_DEMONSEALER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEMONSEALER].ammo_use2 = 10;
	Weapons_Data[DND_WEAPON_DEMONSEALER].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_TEMPLARMG].name = "ResMG1";
	Weapons_Data[DND_WEAPON_TEMPLARMG].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_TEMPLARMG].ammo_name2 = "MGClip4";
	Weapons_Data[DND_WEAPON_TEMPLARMG].icon = "WEPICO37";
	Weapons_Data[DND_WEAPON_TEMPLARMG].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_TEMPLARMG].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_TEMPLARMG].properties = WPROP_ALTAMMO;
	
	Weapons_Data[DND_WEAPON_RIOTCANNON].name = "ResMG2";
	Weapons_Data[DND_WEAPON_RIOTCANNON].ammo_name1 = "RiotgunShell";
	Weapons_Data[DND_WEAPON_RIOTCANNON].ammo_name2 = "RiotgunClip";
	Weapons_Data[DND_WEAPON_RIOTCANNON].icon = "WEPICO38";
	Weapons_Data[DND_WEAPON_RIOTCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RIOTCANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_RIOTCANNON].properties = WPROP_ALTAMMO;
	
	Weapons_Data[DND_WEAPON_ACIDRIFLE].name = "ResMG3";
	Weapons_Data[DND_WEAPON_ACIDRIFLE].ammo_name1 = "AcidAmmo";
	Weapons_Data[DND_WEAPON_ACIDRIFLE].ammo_name2 = "AcidClip";
	Weapons_Data[DND_WEAPON_ACIDRIFLE].icon = "WEPICO39";
	Weapons_Data[DND_WEAPON_ACIDRIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ACIDRIFLE].ammo_use2 = 8;
	Weapons_Data[DND_WEAPON_ACIDRIFLE].properties = WPROP_POISON;
	
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].name = "ResMG4";
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].ammo_name1 = "FusionCell";
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].ammo_name2 = "MGClip6";
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].icon = "WEPICO40";
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].ammo_use2 = 20;
	Weapons_Data[DND_WEAPON_FUSIONBLASTER].properties = WPROP_IGNORESHIELD | WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_INCINERATOR].name = "ResMG5";
	Weapons_Data[DND_WEAPON_INCINERATOR].ammo_name1 = "IncineratorAmmo";
	Weapons_Data[DND_WEAPON_INCINERATOR].ammo_name2 = "IncineratorStacks";
	Weapons_Data[DND_WEAPON_INCINERATOR].icon = "WEPICO94";
	Weapons_Data[DND_WEAPON_INCINERATOR].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_INCINERATOR].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_INCINERATOR].properties = WPROP_IGNORESHIELD | WPROP_RIPPER | WPROP_SELFDMG | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_DESOLATOR].name = "Desolator";
	Weapons_Data[DND_WEAPON_DESOLATOR].ammo_name1 = "DesolatorAmmo";
	Weapons_Data[DND_WEAPON_DESOLATOR].ammo_name2 = "DesolatorOverheat";
	Weapons_Data[DND_WEAPON_DESOLATOR].icon = "WEPICO41";
	Weapons_Data[DND_WEAPON_DESOLATOR].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DESOLATOR].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DESOLATOR].properties = WPROP_OVERHEAT | WPROP_POISON;
	
	Weapons_Data[DND_WEAPON_MINIGUN].name = " Minigun ";
	Weapons_Data[DND_WEAPON_MINIGUN].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_MINIGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_MINIGUN].icon = "WEPICO42";
	Weapons_Data[DND_WEAPON_MINIGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_MINIGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_MINIGUN].properties = WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_EBONYCANNON].name = "Ebony Cannon";
	Weapons_Data[DND_WEAPON_EBONYCANNON].ammo_name1 = "EbonyAmmo";
	Weapons_Data[DND_WEAPON_EBONYCANNON].ammo_name2 = "EbonyAmmoX";
	Weapons_Data[DND_WEAPON_EBONYCANNON].icon = "WEPICO43";
	Weapons_Data[DND_WEAPON_EBONYCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_EBONYCANNON].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_EBONYCANNON].properties = WPROP_IGNORESHIELD | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_MPPB].name = "MPPB";
	Weapons_Data[DND_WEAPON_MPPB].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_MPPB].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_MPPB].icon = "WEPICO95";
	Weapons_Data[DND_WEAPON_MPPB].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_MPPB].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_MPPB].properties = WPROP_RIPPER | WPROP_NOREFLECT | WPROP_CANTHITGHOST;
	
	// SLOT 5
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].name = "Rocket Launcher";
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].ammo_name1 = "RocketAmmo";
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].icon = "WEPICO44";
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].name = "Upgraded Rocket Launcher";
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].ammo_name1 = "RocketAmmo";
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].icon = "WEPICO45";
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_TORPEDOLAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].name = "Grenade Launcher";
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_name1 = "Grenades";
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].icon = "WEPICO51";
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_GRENADELAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_ALTAMMO;
	
	Weapons_Data[DND_WEAPON_VINDICATOR].name = "Upgraded Rocket Launcher3";
	Weapons_Data[DND_WEAPON_VINDICATOR].ammo_name1 = "FlakShell";
	Weapons_Data[DND_WEAPON_VINDICATOR].ammo_name2 = "ShellSize_18";
	Weapons_Data[DND_WEAPON_VINDICATOR].icon = "WEPICO47";
	Weapons_Data[DND_WEAPON_VINDICATOR].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_VINDICATOR].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_VINDICATOR].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].name = "ResRL1";
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].ammo_name1 = "MeteorAmmo";
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].icon = "WEPICO48";
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_METEORLAUNCHER].properties = WPROP_IGNORESHIELD | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_HEAVYGL].name = "ResRL2";
	Weapons_Data[DND_WEAPON_HEAVYGL].ammo_name1 = "HeavyGrenades";
	Weapons_Data[DND_WEAPON_HEAVYGL].ammo_name2 = "HeavyGLCounter";
	Weapons_Data[DND_WEAPON_HEAVYGL].icon = "WEPICO49";
	Weapons_Data[DND_WEAPON_HEAVYGL].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HEAVYGL].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HEAVYGL].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG | WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_FREEZER].name = "ResRL3";
	Weapons_Data[DND_WEAPON_FREEZER].ammo_name1 = "EverIce";
	Weapons_Data[DND_WEAPON_FREEZER].ammo_name2 = "FreezerOverheat";
	Weapons_Data[DND_WEAPON_FREEZER].icon = "WEPICO50";
	Weapons_Data[DND_WEAPON_FREEZER].ammo_use1 = 5;
	Weapons_Data[DND_WEAPON_FREEZER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_FREEZER].properties = WPROP_RIPPER | WPROP_OVERHEAT;
	
	Weapons_Data[DND_WEAPON_GRAVDIS].name = "ResRL4";
	Weapons_Data[DND_WEAPON_GRAVDIS].ammo_name1 = "GravdisAmmo";
	Weapons_Data[DND_WEAPON_GRAVDIS].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_GRAVDIS].icon = "WEPICO91";
	Weapons_Data[DND_WEAPON_GRAVDIS].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_GRAVDIS].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_GRAVDIS].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_VOIDCANNON].name = "ResRL5";
	Weapons_Data[DND_WEAPON_VOIDCANNON].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_VOIDCANNON].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_VOIDCANNON].icon = "WEPICO96";
	Weapons_Data[DND_WEAPON_VOIDCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_VOIDCANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_VOIDCANNON].properties = WPROP_RIPPER | WPROP_IGNORESHIELD | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].name = "Upgraded Rocket Launcher2";
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].ammo_name1 = "RocketAmmo";
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].icon = "WEPICO46";
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_MERCURYLAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].name = "Upgraded Grenade Launcher";
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].ammo_name1 = "Grenades";
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].icon = "WEPICO52";
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_ROTARYGRENADELAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].name = "Heavy Missile Launcher";
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].ammo_name1 = "MISAmmo";
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].icon = "WEPICO53";
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HEAVYMISSILELAUNCHER].properties = WPROP_CANTHITGHOST | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].name = "Sedrin Staff";
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].ammo_name1 = "SedrinAmmo";
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].icon = "WEPICO92";
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SEDRINSTAFF].properties = WPROP_SELFDMG | WPROP_POISON;
	
	// SLOT 6
	Weapons_Data[DND_WEAPON_PLASMARIFLE].name = "Plasma Rifle";
	Weapons_Data[DND_WEAPON_PLASMARIFLE].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_PLASMARIFLE].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_PLASMARIFLE].icon = "WEPICO54";
	Weapons_Data[DND_WEAPON_PLASMARIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PLASMARIFLE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_PLASMARIFLE].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].name = "Upgraded Plasma Rifle";
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].ammo_name2 = "PlasmaOverheat";
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].icon = "WEPICO55";
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_NUCLEARPLASMARIFLE].properties = WPROP_IGNORESHIELD | WPROP_OVERHEAT | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_TURELCANNON].name = "Upgraded Plasma Rifle2";
	Weapons_Data[DND_WEAPON_TURELCANNON].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_TURELCANNON].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_TURELCANNON].icon = "WEPICO56";
	Weapons_Data[DND_WEAPON_TURELCANNON].ammo_use1 = 2;
	Weapons_Data[DND_WEAPON_TURELCANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_TURELCANNON].properties = WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_FROSTFANG].name = "Upgraded Plasma Rifle3";
	Weapons_Data[DND_WEAPON_FROSTFANG].ammo_name1 = "EverIce";
	Weapons_Data[DND_WEAPON_FROSTFANG].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_FROSTFANG].icon = "WEPICO57";
	Weapons_Data[DND_WEAPON_FROSTFANG].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_FROSTFANG].ammo_use2 = 30;
	Weapons_Data[DND_WEAPON_FROSTFANG].properties = WPROP_RIPPER | WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_FLAMETHROWER].name = "ResPlasma1";
	Weapons_Data[DND_WEAPON_FLAMETHROWER].ammo_name1 = "Fuel";
	Weapons_Data[DND_WEAPON_FLAMETHROWER].ammo_name2 = "FuelClip";
	Weapons_Data[DND_WEAPON_FLAMETHROWER].icon = "WEPICO58";
	Weapons_Data[DND_WEAPON_FLAMETHROWER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_FLAMETHROWER].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_FLAMETHROWER].properties = WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].name = "ResPlasma2";
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].ammo_name1 = "LightningCell";
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].ammo_name2 = "LightningStacks";
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].icon = "WEPICO59";
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_LIGHTNINGGUN].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_REBOUNDER].name = "ResPlasma3";
	Weapons_Data[DND_WEAPON_REBOUNDER].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_REBOUNDER].ammo_name2 = "RebounderOverheat";
	Weapons_Data[DND_WEAPON_REBOUNDER].icon = "WEPICO60";
	Weapons_Data[DND_WEAPON_REBOUNDER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_REBOUNDER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_REBOUNDER].properties = WPROP_OVERHEAT | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_DARKLANCE].name = "ResPlasma4";
	Weapons_Data[DND_WEAPON_DARKLANCE].ammo_name1 = "DemonBlood";
	Weapons_Data[DND_WEAPON_DARKLANCE].ammo_name2 = "LanceStacks";
	Weapons_Data[DND_WEAPON_DARKLANCE].icon = "WEPICO97";
	Weapons_Data[DND_WEAPON_DARKLANCE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DARKLANCE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DARKLANCE].properties = WPROP_NOREFLECT | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_RHINORIFLE].name = "Nailgun";
	Weapons_Data[DND_WEAPON_RHINORIFLE].ammo_name1 = "Clip";
	Weapons_Data[DND_WEAPON_RHINORIFLE].ammo_name2 = "MGClip7";
	Weapons_Data[DND_WEAPON_RHINORIFLE].icon = "WEPICO61";
	Weapons_Data[DND_WEAPON_RHINORIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RHINORIFLE].ammo_use2 = 1;
	Weapons_Data[DND_WEAPON_RHINORIFLE].properties = WPROP_ALTAMMO;
	
	Weapons_Data[DND_WEAPON_NAILGUN].name = "Nailgun";
	Weapons_Data[DND_WEAPON_NAILGUN].ammo_name1 = "NailgunAmmo";
	Weapons_Data[DND_WEAPON_NAILGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_NAILGUN].icon = "WEPICO62";
	Weapons_Data[DND_WEAPON_NAILGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_NAILGUN].ammo_use2 = 2;
	Weapons_Data[DND_WEAPON_NAILGUN].properties = WPROP_RIPPER | WPROP_CANTHITGHOST | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_BASILISK].name = "Basilisk";
	Weapons_Data[DND_WEAPON_BASILISK].ammo_name1 = "BasiliskAmmo";
	Weapons_Data[DND_WEAPON_BASILISK].ammo_name2 = "LoadedBasilisk";
	Weapons_Data[DND_WEAPON_BASILISK].icon = "WEPICO63";
	Weapons_Data[DND_WEAPON_BASILISK].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_BASILISK].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_BASILISK].properties = WPROP_IGNORESHIELD;
	
	// SLOT 7
	Weapons_Data[DND_WEAPON_BFG6000].name = "BFG 9000";
	Weapons_Data[DND_WEAPON_BFG6000].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_BFG6000].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_BFG6000].icon = "WEPICO64";
	Weapons_Data[DND_WEAPON_BFG6000].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_BFG6000].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_BFG6000].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_BFG32768].name = "Upgraded BFG 9000";
	Weapons_Data[DND_WEAPON_BFG32768].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_BFG32768].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_BFG32768].icon = "WEPICO65";
	Weapons_Data[DND_WEAPON_BFG32768].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_BFG32768].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_BFG32768].properties = WPROP_IGNORESHIELD | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_DEVASTATOR].name = "Devastator";
	Weapons_Data[DND_WEAPON_DEVASTATOR].ammo_name1 = "DevastatorAmmo";
	Weapons_Data[DND_WEAPON_DEVASTATOR].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DEVASTATOR].icon = "WEPICO66";
	Weapons_Data[DND_WEAPON_DEVASTATOR].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEVASTATOR].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DEVASTATOR].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_CANTHITGHOST | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_MFG].name = "MFG";
	Weapons_Data[DND_WEAPON_MFG].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_MFG].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_MFG].icon = "WEPICO67";
	Weapons_Data[DND_WEAPON_MFG].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_MFG].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_MFG].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_IONCANNON].name = "ResBFG1";
	Weapons_Data[DND_WEAPON_IONCANNON].ammo_name1 = "IonAmmo";
	Weapons_Data[DND_WEAPON_IONCANNON].ammo_name2 = "IonOverHeat";
	Weapons_Data[DND_WEAPON_IONCANNON].icon = "WEPICO68";
	Weapons_Data[DND_WEAPON_IONCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_IONCANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_IONCANNON].properties = WPROP_OVERHEAT | WPROP_IGNORESHIELD | WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].name = "ResBFG2";
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].ammo_name1 = "ThunderAmmo";
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].ammo_name2 = "ThunderRangeCounter";
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].icon = "WEPICO69";
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_THUNDERSTAFF].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].name = "Gauss Rifle";
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].ammo_name1 = "GaussRound";
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].icon = "WEPICO70";
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_GAUSSRIFLE].properties = WPROP_SELFDMG | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_RAILGUN].name = "Rail gun";
	Weapons_Data[DND_WEAPON_RAILGUN].ammo_name1 = "Cell";
	Weapons_Data[DND_WEAPON_RAILGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_RAILGUN].icon = "WEPICO71";
	Weapons_Data[DND_WEAPON_RAILGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RAILGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_RAILGUN].properties = WPROP_IGNORESHIELD | WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_DEATHRAY].name = "Death Ray";
	Weapons_Data[DND_WEAPON_DEATHRAY].ammo_name1 = "IonAmmo";
	Weapons_Data[DND_WEAPON_DEATHRAY].ammo_name2 = "DeathrayOverheat";
	Weapons_Data[DND_WEAPON_DEATHRAY].icon = "WEPICO88";
	Weapons_Data[DND_WEAPON_DEATHRAY].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEATHRAY].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DEATHRAY].properties = WPROP_IGNORESHIELD | WPROP_OVERHEAT;
	
	// SLOT 8
	Weapons_Data[DND_WEAPON_DEATHSTAFF].name = "Death Staff";
	Weapons_Data[DND_WEAPON_DEATHSTAFF].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_DEATHSTAFF].ammo_name2 = "Souls";
	Weapons_Data[DND_WEAPON_DEATHSTAFF].icon = "WEPICO72";
	Weapons_Data[DND_WEAPON_DEATHSTAFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEATHSTAFF].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DEATHSTAFF].properties = WPROP_IGNORESHIELD | WPROP_SELFDMG;
	
	Weapons_Data[DND_WEAPON_RAZORFANG].name = "Razorfang";
	Weapons_Data[DND_WEAPON_RAZORFANG].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_RAZORFANG].ammo_name2 = "Souls";
	Weapons_Data[DND_WEAPON_RAZORFANG].icon = "WEPICO73";
	Weapons_Data[DND_WEAPON_RAZORFANG].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RAZORFANG].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_RAZORFANG].properties = WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_SUNSTAFF].name = "Sun Staff";
	Weapons_Data[DND_WEAPON_SUNSTAFF].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_SUNSTAFF].ammo_name2 = "Souls";
	Weapons_Data[DND_WEAPON_SUNSTAFF].icon = "WEPICO74";
	Weapons_Data[DND_WEAPON_SUNSTAFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SUNSTAFF].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SUNSTAFF].properties = WPROP_IGNORESHIELD | WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_SOULREAVER].name = "Soul Reaver";
	Weapons_Data[DND_WEAPON_SOULREAVER].ammo_name1 = "Souls";
	Weapons_Data[DND_WEAPON_SOULREAVER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SOULREAVER].icon = "WEPICO75";
	Weapons_Data[DND_WEAPON_SOULREAVER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SOULREAVER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SOULREAVER].properties = WPROP_IGNORESHIELD | WPROP_RIPPER;
	
	// TEMP WEPS
	Weapons_Data[DND_WEAPON_SAWEDOFF].name = "Sawedoff";
	Weapons_Data[DND_WEAPON_SAWEDOFF].ammo_name1 = "SawedoffShell";
	Weapons_Data[DND_WEAPON_SAWEDOFF].ammo_name2 = "SawedoffCounter";
	Weapons_Data[DND_WEAPON_SAWEDOFF].icon = "WEPICO76";
	Weapons_Data[DND_WEAPON_SAWEDOFF].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SAWEDOFF].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SAWEDOFF].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_SOULRENDER].name = "Soul Render";
	Weapons_Data[DND_WEAPON_SOULRENDER].ammo_name1 = "BladeHits";
	Weapons_Data[DND_WEAPON_SOULRENDER].ammo_name2 = "BladeCharge";
	Weapons_Data[DND_WEAPON_SOULRENDER].icon = "WEPICO77";
	Weapons_Data[DND_WEAPON_SOULRENDER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SOULRENDER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SOULRENDER].properties = WPROP_RIPPER | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_SMG].name = "SMG";
	Weapons_Data[DND_WEAPON_SMG].ammo_name1 = "SMGAmmo";
	Weapons_Data[DND_WEAPON_SMG].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_SMG].icon = "WEPICO78";
	Weapons_Data[DND_WEAPON_SMG].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_SMG].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_SMG].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].name = "Hellforge Cannon";
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].ammo_name1 = "IronBalls";
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].ammo_name2 = "IronBalls";
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].icon = "WEPICO79";
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HELLFORGECANNON].properties = WPROP_RIPPER | WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].name = "Bloodfiend Spine";
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].ammo_name1 = "BloodAmmo";
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].ammo_name2 = "BloodAmmo";
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].icon = "WEPICO80";
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_BLOODFIENDSPINE].properties = WPROP_CANTHITGHOST;

	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].name = "Enforcer Rifle";
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].ammo_name1 = "LaserAmmo";
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].ammo_name2 = "LaserAmmo";
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].icon = "WEPICO81";
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_ENFORCERRIFLE].properties = WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_VENOM].name = "Venom";
	Weapons_Data[DND_WEAPON_VENOM].ammo_name1 = "VenomAmmo";
	Weapons_Data[DND_WEAPON_VENOM].ammo_name2 = "VenomAmmo";
	Weapons_Data[DND_WEAPON_VENOM].icon = "WEPICO82";
	Weapons_Data[DND_WEAPON_VENOM].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_VENOM].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_VENOM].properties = WPROP_RIPPER | WPROP_CANTHITGHOST;
	
	Weapons_Data[DND_WEAPON_DEMONHEART].name = "Demon Heart";
	Weapons_Data[DND_WEAPON_DEMONHEART].ammo_name1 = "HeartAmmo";
	Weapons_Data[DND_WEAPON_DEMONHEART].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DEMONHEART].icon = "WEPICO83";
	Weapons_Data[DND_WEAPON_DEMONHEART].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DEMONHEART].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DEMONHEART].properties = WPROP_IGNORESHIELD;
	
	Weapons_Data[DND_WEAPON_DARKGLOVES].name = "DarkServantGloves";
	Weapons_Data[DND_WEAPON_DARKGLOVES].ammo_name1 = "DarkServantEnergy";
	Weapons_Data[DND_WEAPON_DARKGLOVES].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_DARKGLOVES].icon = "WEPICO84";
	Weapons_Data[DND_WEAPON_DARKGLOVES].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_DARKGLOVES].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_DARKGLOVES].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].name = "Nailgun2";
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].ammo_name1 = "BigNail";
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].icon = "WEPICO85";
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_HEAVYNAILGUN].properties = WPROP_RIPPER;
	
	Weapons_Data[DND_WEAPON_BERETTAS].name = "Berettas";
	Weapons_Data[DND_WEAPON_BERETTAS].ammo_name1 = "BerettaAmmo";
	Weapons_Data[DND_WEAPON_BERETTAS].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_BERETTAS].icon = "WEPICO86";
	Weapons_Data[DND_WEAPON_BERETTAS].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_BERETTAS].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_BERETTAS].properties = WPROP_NONE;
	
	Weapons_Data[DND_WEAPON_PLASMABOLTER].name = "Plasma Bolter";
	Weapons_Data[DND_WEAPON_PLASMABOLTER].ammo_name1 = "PlasmaBolterAmmo";
	Weapons_Data[DND_WEAPON_PLASMABOLTER].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_PLASMABOLTER].icon = "WEPICO87";
	Weapons_Data[DND_WEAPON_PLASMABOLTER].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_PLASMABOLTER].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_PLASMABOLTER].properties = WPROP_RIPPER | WPROP_NOREFLECT;
	
	Weapons_Data[DND_WEAPON_RIPPERCANNON].name = "Ripper Cannon";
	Weapons_Data[DND_WEAPON_RIPPERCANNON].ammo_name1 = "RipperCannonAmmo";
	Weapons_Data[DND_WEAPON_RIPPERCANNON].ammo_name2 = "";
	Weapons_Data[DND_WEAPON_RIPPERCANNON].icon = "WEPICO89";
	Weapons_Data[DND_WEAPON_RIPPERCANNON].ammo_use1 = 1;
	Weapons_Data[DND_WEAPON_RIPPERCANNON].ammo_use2 = 0;
	Weapons_Data[DND_WEAPON_RIPPERCANNON].properties = WPROP_RIPPER;
}
	

// Supporting 8 such properties. See: RPGMENUBACKGROUNDID for increasing this
enum {
	WPROP_NONE,
	WPROP_CANTHITGHOST = 1,
	WPROP_SELFDMG = 2,
	WPROP_OVERHEAT = 4,
	WPROP_IGNORESHIELD = 8,
	WPROP_ALTAMMO = 16,
	WPROP_RIPPER = 32,
	WPROP_IRREDUCIBLE = 64,
	WPROP_NOREFLECT = 128,
	WPROP_POISON = 256
};
#define MAX_WEAPON_PROPERTIES 9

str WeaponPropertyImages[MAX_WEAPON_PROPERTIES] = {
	"WNOGHST",
	"WSLFDMG",
	"WOHEAT",
	"WOSHLD",
	"WOALT",
	"WORIP",
	"WOFULL",
	"WNOREFL",
	"WOPOIS"
};

// put them in this place if they are luxury weapons
bool IsLuxuryWeapon(int id) {
	switch(id) {
		case DND_WEAPON_DUSKBLADE:
		case DND_WEAPON_SICKLE:
		
		case DND_WEAPON_RUBYWAND:
		case DND_WEAPON_SCATTERGUN:
		
		case DND_WEAPON_SILVERGUN:
		case DND_WEAPON_SLAYER:
		
		case DND_WEAPON_DESOLATOR:
		case DND_WEAPON_MINIGUN:
		case DND_WEAPON_EBONYCANNON:
		case DND_WEAPON_MPPB:
		
		case DND_WEAPON_ROTARYGRENADELAUNCHER:
		case DND_WEAPON_HEAVYMISSILELAUNCHER:
		case DND_WEAPON_SEDRINSTAFF:
			
		case DND_WEAPON_RHINORIFLE:
		case DND_WEAPON_NAILGUN:
		case DND_WEAPON_BASILISK:
			
		case DND_WEAPON_GAUSSRIFLE:
		case DND_WEAPON_RAILGUN:
		case DND_WEAPON_DEATHRAY:
			
		// these shouldnt be luxury weapons, there are no regular weapons for this slot anyway!
		/*case DND_WEAPON_DEATHSTAFF:
		case DND_WEAPON_RAZORFANG:
		case DND_WEAPON_SUNSTAFF:
		case DND_WEAPON_SOULREAVER:*/
		return true;
	}
	return false;
}

bool IsSoulWeapon(int wepid) {
	switch(wepid) {
		case DND_WEAPON_DEATHSTAFF:
		case DND_WEAPON_RAZORFANG:
		case DND_WEAPON_SUNSTAFF:
		case DND_WEAPON_SOULREAVER:
		case DND_WEAPON_DUSKBLADE:
		case DND_WEAPON_INFERNOSWORD:
		return true;
	}
	return false;
}

bool IsSuperWeapon(int wepid) {
	return wepid >= FIRST_SLOT7_WEAPON && wepid <= LAST_SLOT8_WEAPON;
}

#define WEPCHECK_SLOT1 "P_Slot1Replaced"
#define WEPCHECK_SLOT1L "P_Slot1Luxury"
#define WEPCHECK_SLOT2 "P_Slot2Replaced"
#define WEPCHECK_SLOT2L "P_Slot2Luxury"
#define WEPCHECK_SLOT3 "P_Slot3Replaced"
#define WEPCHECK_SLOT3X "P_Slot3XReplaced"
#define WEPCHECK_SLOT3L "P_Slot3Luxury"
#define WEPCHECK_SLOT4 "P_Slot4Replaced"
#define WEPCHECK_SLOT4L "P_Slot4Luxury"
#define WEPCHECK_SLOT5 "P_Slot5Replaced"
#define WEPCHECK_SLOT5L "P_Slot5Luxury"
#define WEPCHECK_SLOT6 "P_Slot6Replaced"
#define WEPCHECK_SLOT6L "P_Slot6Luxury"
#define WEPCHECK_SLOT7 "P_Slot7Replaced"
#define WEPCHECK_SLOT7L "P_Slot7Luxury"
#define WEPCHECK_SLOT8 "P_Slot8Luxury"	// this says luxury here but theres really no point, they all share the same slot regardless and simplifies checking algorithm

#define MAX_WEPCONDITION_CHECKERS 16
str WeaponConditionCheckers[MAX_WEPCONDITION_CHECKERS] = {
	WEPCHECK_SLOT1,
	WEPCHECK_SLOT1L,
	WEPCHECK_SLOT2,
	WEPCHECK_SLOT2L,
	WEPCHECK_SLOT3,
	WEPCHECK_SLOT3X,
	WEPCHECK_SLOT3L,
	WEPCHECK_SLOT4,
	WEPCHECK_SLOT4L,
	WEPCHECK_SLOT5,
	WEPCHECK_SLOT5L,
	WEPCHECK_SLOT6,
	WEPCHECK_SLOT6L,
	WEPCHECK_SLOT7,
	WEPCHECK_SLOT7L,
	WEPCHECK_SLOT8
};

enum {
	DND_OVERHEAT_SHOCKER,
	DND_OVERHEAT_FREEZER,
	DND_OVERHEAT_NUCLEARPR,
	DND_OVERHEAT_ION,
	DND_OVERHEAT_REBOUNDER,
	DND_OVERHEAT_DESOLATOR,
	DND_OVERHEAT_DEATHRAY
};

#define MAXOVERHEATWEPS DND_OVERHEAT_DEATHRAY + 1
str WeaponOverheatItems[MAXOVERHEATWEPS] = {
	"ShockerOverheat",
	"FreezerOverheat",
	"PlasmaOverheat",
	"IonOverheat",
	"RebounderOverheat",
	"DesolatorOverheat",
	"DeathRayOverheat"
};

// 32 overheat weapons supported
int PlayerRunsOverheat[MAXPLAYERS] = { 0 };

void ResetWeaponMods(int pnum) {
	int i, j;
	for(j = 0; j < MAXWEPS; ++j) {
		Player_Weapon_Infos[pnum][j].quality = 0;
		for(i = 0; i < MAX_WEP_MODS; ++i) {
			Player_Weapon_Infos[pnum][j].wep_mods[i].tier = 0;
			Player_Weapon_Infos[pnum][j].wep_mods[i].val = 0;
		}
	}
}

// This basically checks eligibility of weapon pickups if we have that slot's weapon or not
bool CheckSlotWeapon(int slot) {
	int beg = 0, end = 0;
	
	switch(slot) {
		case 0:
			beg = DND_WEAPON_CHAINSAW;
			end = LAST_SLOT0_WEAPON;
		break;
		case 1:
			beg = FIRST_SLOT1_WEAPON;
			end = LAST_SLOT1_WEAPON;
		break;
		case 2:
			beg = FIRST_SLOT2_WEAPON;
			end = LAST_SLOT2_WEAPON;
		break;
		case 3:
			beg = FIRST_SLOT3_WEAPON;
			end = LAST_SLOT3_WEAPON;
		break;
		case 4:
			beg = FIRST_SLOT4_WEAPON;
			end = LAST_SLOT4_WEAPON;
		break;
		case 5:
			beg = FIRST_SLOT5_WEAPON;
			end = LAST_SLOT5_WEAPON;
		break;
		case 6:
			beg = FIRST_SLOT6_WEAPON;
			end = LAST_SLOT6_WEAPON;
		break;
		case 7:
			beg = FIRST_SLOT7_WEAPON;
			end = LAST_SLOT7_WEAPON;
		break;
	}
	
	for(int i = beg; i <= end; ++i) {
		if(IsLuxuryWeapon(i))
			return false;
		if(CheckInventory(Weapons_Data[i].name))
			return true;
	}
	return false;
}

int GetWeaponPosFromTable() {
	return CheckInventory("DnD_WeaponID");
}

int GetActorWeaponID(int actor) {
	return CheckActorInventory(actor, "DnD_WeaponID");
}

int PickRandomOwnedWeaponID(void) {
	int count = 0, i, pnum = PlayerNumber();
	static int owned_weapons[MAXPLAYERS][MAXWEPS];
	// setup the list
	for(i = 0; i < MAXWEPS; ++i) {
		if(CheckInventory(Weapons_Data[i].name)) {
			owned_weapons[pnum][count] = i;
			++count;
		}
	}
	// pick from random weapons owned
	return owned_weapons[pnum][random(0, count - 1)];
}

str GetWeaponAmmoType(int wepid, int which) {
	if(!which)
		return Weapons_Data[wepid].ammo_name1;
	return Weapons_Data[wepid].ammo_name2;
}

bool IsBoomstick(int id) {
	switch(id) {
		case DND_WEAPON_SHOTGUN:
		case DND_WEAPON_PURIFIER:
		case DND_WEAPON_KILLSTORM:
		case DND_WEAPON_DEADLOCK:
		case DND_WEAPON_SUPERSHOTGUN:
		case DND_WEAPON_HEAVYSUPERSHOTGUN:
		case DND_WEAPON_ERASUS:
		case DND_WEAPON_SHOCKER:
		case DND_WEAPON_HADES:
		case DND_WEAPON_SILVERGUN:
		case DND_WEAPON_INCINERATOR:
		case DND_WEAPON_VINDICATOR:
		case DND_WEAPON_SAWEDOFF:
		return true;
	}
	return false;
}

int CountShotgunWeaponsOwned() {
	// optimized to not loop over all weapons, but newer shotguns must be added here
	return 	CheckInventory(Weapons_Data[DND_WEAPON_SHOTGUN].name) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_PURIFIER].name) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_KILLSTORM].name) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_DEADLOCK].name) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_SUPERSHOTGUN].name) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_HEAVYSUPERSHOTGUN].name) 		+
			CheckInventory(Weapons_Data[DND_WEAPON_ERASUS].name) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_SHOCKER].name) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_HADES].name) 					+
			CheckInventory(Weapons_Data[DND_WEAPON_SILVERGUN].name) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_INCINERATOR].name) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_VINDICATOR].name) 				+
			CheckInventory(Weapons_Data[DND_WEAPON_SAWEDOFF].name);
}

bool IsTemporaryWeapon(int id) {
	return id >= DND_WEAPON_SAWEDOFF;
}

int GetWeaponCount() {
	int res = 0;
	for(int i = 0; i < MAXWEPS; ++i)
		if(CheckInventory(Weapons_Data[i].name))
			++res;
	return res;
}

int MapClassicWeaponToTableID(int id) {
	int ret = DND_WEAPON_FIST;
	switch (id) {
		case 0:
			ret = DND_WEAPON_CHAINSAW;
		break;
		case 1:
			ret = DND_WEAPON_SHOTGUN;
		break;
		case 2:
			ret = DND_WEAPON_SUPERSHOTGUN;
		break;
		case 3:
			ret = DND_WEAPON_MACHINEGUN;
		break;
		case 4:
			ret = DND_WEAPON_ROCKETLAUNCHER;
		break;
		case 5:
			ret = DND_WEAPON_PLASMARIFLE;
		break;
		case 6:
			ret = DND_WEAPON_BFG6000;
		break;
	}
	return ret;
}

bool IsWeaponLightningType(int wepid, int extra, bool isSpecial) {
	switch(wepid) {
		case DND_WEAPON_SHOCKER:
		case DND_WEAPON_LIGHTNINGGUN:
		case DND_WEAPON_THUNDERSTAFF:
		return true;
	}
	return isSpecial && extra == SSAM_SHOCK;
}

bool IsMeleeWeapon(int wepid) {
	switch(wepid) {
		case DND_WEAPON_FIST:
		case DND_WEAPON_CHAINSAW:
		case DND_WEAPON_DOUBLECHAINSAW:
		case DND_WEAPON_KATANA:
		case DND_WEAPON_EXCALIBAT:
		case DND_WEAPON_INFERNOSWORD:
		case DND_WEAPON_DUSKBLADE:
		case DND_WEAPON_SICKLE:
		case DND_WEAPON_SOULRENDER:
		return true;
	}
	return false;
}

void DoWeaponTip(int curweap) {
	if(!CheckInventory("TipBoxOpen")) {
		ACS_ExecuteAlways(977, 0, 0, curweap);
		GiveInventory("TipBoxOpen", 1);
	}
}

// This needs a better name for real, but it returns indexing of slot for tabular format, not in-game slot of weapons
// this will never change so I dont want to waste an array spot for this
int GetSlotOfWeapon(int wepid) {
	if(wepid <= LAST_SLOT0_WEAPON)
		return 0;
	if(wepid <= LAST_SLOT1_WEAPON)
		return 1;
	if(wepid <= LAST_SLOT2_WEAPON)
		return 2;
	if(wepid <= LAST_SLOT3_WEAPON)
		return 3;
	if(wepid <= LAST_SLOT4_WEAPON)
		return 4;
	if(wepid <= LAST_SLOT5_WEAPON)
		return 5;
	if(wepid <= LAST_SLOT6_WEAPON)
		return 6;
	if(wepid <= LAST_SLOT7_WEAPON)
		return 7;
	if(wepid <= LAST_SLOT8_WEAPON)
		return 8;
	return 9;
}

// This gets the in-game slot of the weapon
int GetGameSlotOfWeapon(int wepid) {
	int slot = GetSlotOfWeapon(wepid);
	// 0 is melee, 1 is pistols, 2 is shotguns, 3 is super shotguns and 3 and above is fine, we need to add +1 to things below 2
	if(slot <= 2)
		return slot + 1;
	return slot;
}

// Retrieves a weapon's order within the slot cluster
int GetWeaponOrderSlot(int wepid, int slot) {
	switch(slot) {
		case 0:
		return wepid - FIRST_SLOT0_WEAPON;
		case 1:
		return wepid - FIRST_SLOT1_WEAPON;
		case 2:
		return wepid - FIRST_SLOT2_WEAPON;
		case 3:
		return wepid - FIRST_SLOT3_WEAPON;
		case 4:
		return wepid - FIRST_SLOT4_WEAPON;
		case 5:
		return wepid - FIRST_SLOT5_WEAPON;
		case 6:
		return wepid - FIRST_SLOT6_WEAPON;
		case 7:
		return wepid - FIRST_SLOT7_WEAPON;
		case 8:
		return wepid - FIRST_SLOT8_WEAPON;
		case 9:
		return wepid - FIRST_SLOT9_WEAPON;
	}
	return 0;
}

str GetWeaponTipText(int wepid) {
	int slot = GetSlotOfWeapon(wepid);
	int subid = GetWeaponOrderSlot(wepid, slot);
	return StrParam(s:"WEPTIP_", d:slot, s:"_", d:subid);
}

str GetWeaponTag(int wepid) {
	return StrParam(s:"WEP_", d:wepid, s:"_TAG");
}

#include "DnD_Projectiles.h"

void HandleAmmoGainChance(int slot, int ammo, int amount, int owner = 0) {
	// if hand of artemis is equipped don't run this
	if(random(1, 100) <= GetPlayerAttributeValue(PlayerNumber(), INV_AMMOGAIN_CHANCE)) {
		if(slot == DND_AMMOSLOT_MAGAZINE)
			GiveInventory(ClipAmmoTypes[ammo], amount);
		else if(slot == DND_AMMOSLOT_SPECIAL) {
			// subtract one if we are above slug, we have grenade for an exception
			if(ammo > SSAM_SLUG)
				--ammo;
			GiveInventory(SpecialAmmoInfo_Str[ammo][AMMOINFO_NAME], amount);
		}
		else if(slot == DND_AMMOSLOT_TEMPORARY) {
			GiveInventory(TemporaryWeaponData[ammo][TEMPWEP_AMMO], amount);
			if(ammo == DND_TEMPWEP_SOULRENDER)
				GiveInventory("BladeCharge", 100);
		}
		else
			GiveInventory(AmmoInfo_Str[slot][ammo][AMMOINFO_NAME], amount);
	}
}

// we get weapon id, primary or alt and flags only
// ammo_slot >> 16 contains subslot, first 16 contains main slot
Script "DnD Fire Weapon" (int wepid, int isAltfire, int ammo_slot, int flags) {
	int owner = ActivatorTID();
	int pnum = PlayerNumber();
	
	bool hasArtemis = IsAccessoryEquipped(owner, DND_ACCESSORY_HANDARTEMIS);
	
	flags |= hasArtemis * DND_ATF_NOAMMOTAKE;
	
	int ammo_sub_slot = ammo_slot >> 16;
	ammo_slot &= 0xFFFF;
	
	// Handles ammo gain checks, the chances etc.
	int ammo_take_amt;
	str ammo_type = "";
	if((isAltfire & DND_ATK_SECONDARY) && Weapons_Data[wepid].ammo_name2 != "") {
		ammo_type = Weapons_Data[wepid].ammo_name2;
		ammo_take_amt = Weapons_Data[wepid].ammo_use2;
	}
	else {
		ammo_type = Weapons_Data[wepid].ammo_name1;
		ammo_take_amt = Weapons_Data[wepid].ammo_use1;
	}
	
	if(ammo_slot != DND_AMMOSLOT_SPECIAL)
		GiveInventory("DnD_UsedNonSpecial", 1);
	
	int proj_id = -1;				// id of proj to fire
	int count = 1;					// proj count
	int sp_x = 0, sp_y = 0;			// spread values
	int angle_vec = GetVec2();		// negative is left, positive is right
	int offset_vec = GetVec3();
	bool use_default = false;		// default behavior
	
	// we will scale count whenever the weapon would require it!
	// unused variables use their default values from above
	switch(wepid) {
		// SLOT 2
		case DND_WEAPON_PISTOL:
			proj_id = DND_PROJ_PISTOL;
			use_default = true;
			sp_x = 0.5;
		break;
		case DND_WEAPON_AKIMBOPISTOL:
			proj_id = DND_PROJ_AKIMBO;
			use_default = true;
			sp_x = 3.5;
			sp_y = 2.5;
		break;
		case DND_WEAPON_MAGNUM:
			proj_id = DND_PROJ_MAGNUMREVOLVER;
			use_default = true;
			sp_x = 0.1;
			sp_y = 0.1;
		break;
		case DND_WEAPON_LASERPISTOL:
			proj_id = DND_PROJ_LASERPISTOL;
			use_default = true;
			sp_x = 2.0;
			sp_y = 1.25;
		break;
		case DND_WEAPON_SCATTERGUN:
			proj_id = DND_PROJ_SCATTERGUN;
			use_default = true;
			// primary fire has spread
			if(!(isAltfire & DND_ATK_SECONDARY))
				sp_x = 3.0;
		break;
		case DND_WEAPON_RUBYWAND:
			// secondary attack fires one projectile
			if(isAltfire & DND_ATK_SECONDARY) {
				proj_id = DND_PROJ_RUBYWANDFLAME;
				use_default = true;
			}
			else if(isAltFire & DND_ATK_OTHER_DIR) {
				// other dir fires a single projectile
				proj_id = DND_PROJ_RUBYWAND;
				use_default = true;
			}
			else {
				// regular primary fire fires 3 projectiles with fixed angle, so we don't use default
				proj_id = DND_PROJ_RUBYWAND;
				use_default = false;
				
				// -3, 0, 3 deg angles
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, sp_x, sp_y, flags);
				vec2[angle_vec].x = -3.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, sp_x, sp_y, flags);
				vec2[angle_vec].x = 3.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, sp_x, sp_y, flags);
			}
		break;
		case DND_WEAPON_ASSAULTRIFLE:
			if(isAltFire & DND_ATK_OTHER_DIR) {
				sp_x = 0.1;
				sp_y = 0.1;
			}
			else {
				sp_x = 3.6;
				sp_y = 2.4;
			}
			use_default = true;
			proj_id = DND_PROJ_ASSAULTRIFLE;
		break;
		case DND_WEAPON_VIPERSTAFF:
			if(isAltFire & DND_ATK_SECONDARY) {
				use_default = true;
				proj_id = DND_PROJ_VIPERMISSILE;
			}
			else {
				use_default = false;
				
				// this spawns a minion
				SetVec3XYZ(offset_vec, 24.0, 0.0, -12.0);
				Do_Minion_Summon(owner, ProjectileInfo[DND_PROJ_VIPERCRAWLER].name, offset_vec, ProjectileInfo[DND_PROJ_VIPERCRAWLER].spd_range);
			}
		break;
		
		// SLOT 3
		case DND_WEAPON_SHOTGUN:
			proj_id = DND_PROJ_SHOTGUN;
			use_default = true;
			count = 10;
			sp_x = 5.6;
			sp_y = 5.6;
			
			if(!(flags & DND_ATF_NOAMMOTAKE)) {
				HandleAmmoGainChance(DND_AMMOSLOT_SHELL, AMMO_SHELL, Weapons_Data[wepid].ammo_use1, owner);
				TakeInventory(Weapons_Data[wepid].ammo_name1, Weapons_Data[wepid].ammo_use1);
			}
		break;
		case DND_WEAPON_PURIFIER:
			proj_id = DND_PROJ_PURIFIER;
			use_default = true;
			count = 15;
			sp_x = 3.6;
			sp_y = 3.6;
			
			if(!(flags & DND_ATF_NOAMMOTAKE)) {
				HandleAmmoGainChance(DND_AMMOSLOT_SHELL, AMMO_SHELL, Weapons_Data[wepid].ammo_use1, owner);
				TakeInventory(Weapons_Data[wepid].ammo_name1, Weapons_Data[wepid].ammo_use1);
			}
		break;
		case DND_WEAPON_KILLSTORM:
			proj_id = DND_PROJ_KILLSTORM;
			use_default = true;
			count = 12;
			sp_x = 9.6;
			sp_y = 7.2;
		break;
		case DND_WEAPON_EMERALDWAND:
			use_default = true;
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				proj_id = DND_PROJ_EMERALDMAIN;
				count = 5;
				sp_x = 6.0;
				sp_y = 4.5;
			}
			else
				proj_id = DND_PROJ_EMERALDRAINER;
		break;
		case DND_WEAPON_SUPERSHOTGUN:
			proj_id = DND_PROJ_SSG;
			use_default = true;
			count = 20;
			sp_x = 11.2;
			sp_y = 7.1;
		break;
		case DND_WEAPON_HEAVYSUPERSHOTGUN:
			use_default = false;
			count = 14;
			sp_x = 9.6;
			sp_y = 5.8;
			
			// check if circle attack, fire in circle otherwise
			if(!CheckUniquePropertyOnPlayer(pnum, INV_EX_BEHAVIOR_PELLETSFIRECIRCLE)) {
				// this weapon fires half pellets as railgun, half as hitscan -- so we tuck it at the end
				Do_Hitscan_Attack(owner, pnum, DND_PROJ_HEAVYSSG, wepid, count, ProjectileInfo[DND_PROJ_HEAVYSSG].spd_range, sp_x, sp_y, flags);
				Do_Railgun_Attack("HeavySSG_RailHelper", count);
			}
			else {
				GiveInventory("HeavySSG_RailHelper_Circle", 1);
				Do_Attack_Circle(owner, pnum, DND_PROJ_HEAVYSSG, wepid, GetPelletCount(pnum, count), ProjectileInfo[DND_PROJ_HEAVYSSG].spd_range, flags);
			}
		break;
		case DND_WEAPON_ERASUS:
			proj_id = DND_PROJ_ERASUS;
			use_default = true;
			
			// use other dir to make it different from primary vs alt, and alt to use the magazine not main ammo pool
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				count = 18;
				sp_x = 9.0;
				sp_y = 6.4;
			}
			else {
				count = 36;
				sp_x = 14.8;
				sp_y = 10.2;
				
				// take twice ammo for altfire
				ammo_take_amt <<= 1;
			}
		break;
		case DND_WEAPON_HELLSMAW:
			use_default = false;
			
			// left or right option to check for here
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				// left
				vec2[angle_vec].x = -1.0;
				vec3[offset_vec].y = -12.0;
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMAIN, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				
				SetVec3XYZ(offset_vec, 0.0, 4.0, 0.0);
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMINI, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				SetVec3XYZ(offset_vec, 0.0, -12.0, 16.0);
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMINI, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				SetVec3XYZ(offset_vec, 0.0, -28.0, 0.0);
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMINI, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
			}
			else {
				// right
				vec2[angle_vec].x = 1.0;
				vec3[offset_vec].y = 12.0;
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMAIN, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				
				SetVec3XYZ(offset_vec, 0.0, -4.0, 0.0);
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMINI, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				SetVec3XYZ(offset_vec, 0.0, 12.0, 16.0);
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMINI, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				SetVec3XYZ(offset_vec, 0.0, 28.0, 0.0);
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HELLSMAWMINI, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
			}
		break;
		case DND_WEAPON_SILVERGUN:
			proj_id = DND_PROJ_WHITEDEATH;
			use_default = true;
			count = 10;
			sp_x = 6.4;
			sp_y = 4.8;
		break;
		case DND_WEAPON_SLAYER:
			use_default = false;
			proj_id = DND_PROJ_SLAYER;
		
			if(!CheckUniquePropertyOnPlayer(pnum, INV_EX_BEHAVIOR_PELLETSFIRECIRCLE)) {
				vec2[angle_vec].x = -3.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				vec2[angle_vec].x = 3.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				vec2[angle_vec].x = -6.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				vec2[angle_vec].x = 6.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				vec2[angle_vec].x = -1.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				vec2[angle_vec].x = 1.0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
			}
			else
				Do_Attack_Circle(owner, pnum, proj_id, wepid, GetPelletCount(pnum, 6), ProjectileInfo[proj_id].spd_range, flags);
		break;
		case DND_WEAPON_DEADLOCK:
			proj_id = DND_PROJ_DEADLOCK;
			use_default = true;
			count = 16;
			sp_x = 7.0;
			sp_y = 5.2;
			
			if(!(flags & DND_ATF_NOAMMOTAKE)) {
				HandleAmmoGainChance(DND_AMMOSLOT_SHELL, AMMO_SHELL, Weapons_Data[wepid].ammo_use1, owner);
				TakeInventory(Weapons_Data[wepid].ammo_name1, Weapons_Data[wepid].ammo_use1);
			}
		break;
		case DND_WEAPON_NITROGENCROSSBOW:
			use_default = false;
			
			if(!(isAltFire & DND_ATK_SECONDARY))
				Do_Railgun_Attack("CryobowRailHelper_1", 1);
			else
				Do_Railgun_Attack("CryobowRailHelper_2", 1);
		break;
		case DND_WEAPON_WHEELOFTORMENT:
			use_default = true;
			
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				// primary
				proj_id = DND_PROJ_WHEELOFTORMENT_1;
			}
			else if(isAltFire & DND_ATK_OTHER_DIR) {
				// special 3rd proj that gives ammo
				proj_id = DND_PROJ_WHEELOFTORMENT_3;
			}
			else {
				// 2nd proj
				proj_id = DND_PROJ_WHEELOFTORMENT_2;
			}
		break;
		case DND_WEAPON_CHARONBLASTER:
			use_default = false;
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				proj_id = DND_PROJ_CHARON;
				if(!CheckUniquePropertyOnPlayer(pnum, INV_EX_BEHAVIOR_PELLETSFIRECIRCLE)) {
					vec2[angle_vec].x = -12.5;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = 0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = 12.5;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				}
				else
					Do_Attack_Circle(owner, pnum, proj_id, wepid, GetPelletCount(pnum, 3), ProjectileInfo[proj_id].spd_range, flags);
			}
			else {
				sp_x = CheckInventory("CharonBeamCounter");
				
				switch(sp_x) {
					case 0:
						Do_Hitscan_Attack_Named(owner, pnum, "CharonDamagingPuff_1", wepid, 1, 768.0, 0, 0, 0);
					break;
					case 1:
						Do_Hitscan_Attack_Named(owner, pnum, "CharonDamagingPuff_2", wepid, 1, 768.0, 0, 0, 0);
					break;
					case 2:
						Do_Hitscan_Attack_Named(owner, pnum, "CharonDamagingPuff_3", wepid, 1, 768.0, 0, 0, 0);
					break;
				}
				
				GiveInventory("CharonBeamCounter", 1);
				// we reset at every 3
				if(sp_x == 2)
					SetInventory("CharonBeamCounter", 0);
			}
		break;
		case DND_WEAPON_PLASMACANNON:
			use_default = false;
			proj_id = DND_PROJ_PLASMACANNON;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				// circle pattern -- check circle fire too
				if(!CheckUniquePropertyOnPlayer(pnum, INV_EX_BEHAVIOR_PELLETSFIRECIRCLE)) {
					for(sp_x = 0; sp_x < 12; ++sp_x) {
						SetVec2XY(angle_vec, 4 * cos(30.0 * sp_x), 4 * sin(30.0 * sp_x));
						Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					}
				}
				else
					Do_Attack_Circle(owner, pnum, proj_id, wepid, GetPelletCount(pnum, 12), ProjectileInfo[proj_id].spd_range, flags);
			}
			else {
				// spread pattern -- check circle fire too				
				if(!CheckUniquePropertyOnPlayer(pnum, INV_EX_BEHAVIOR_PELLETSFIRECIRCLE)) {
					vec2[angle_vec].x = 1.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = 4.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = 6.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = -1.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = -4.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
					vec2[angle_vec].x = -6.0;
					Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, flags);
				}
				else
					Do_Attack_Circle(owner, pnum, proj_id, wepid, GetPelletCount(pnum, 6), ProjectileInfo[proj_id].spd_range, flags);
			}
		break;
		case DND_WEAPON_SHOCKER:
			// altfire fires an AoE but we dont need direction on that to be precise really, so it just fires pellets
			count = 18;
			proj_id = DND_PROJ_SHOCKER;
			use_default = true;
			sp_x = 8.4;
			sp_y = 6.2;
		break;
		case DND_WEAPON_HADES:
			use_default = true;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				proj_id = DND_PROJ_HADES;
				count = 15;
				sp_x = 11.6;
				sp_y = 9.0;
			}
			else {
				proj_id = DND_PROJ_HADES_2;
				count = 1;
			}
		break;
		case DND_WEAPON_CROSSBOW:
			use_default = false;
			sp_x = CheckInventory("Crossbow_PullLevel");
			
			switch(sp_x) {
				case 0:
					Do_Projectile_Attack_Named(owner, pnum, "CrossbowBolt_1", wepid, 1, 64, angle_vec, offset_vec, 0, 0, 0);
				break;
				case 1:
					Do_Projectile_Attack_Named(owner, pnum, "CrossbowBolt_2", wepid, 1, 80, angle_vec, offset_vec, 0, 0, 0);
				break;
				case 2:
					Do_Projectile_Attack_Named(owner, pnum, "CrossbowBolt_3", wepid, 1, 120, angle_vec, offset_vec, 0, 0, 0);
				break;
				case 3:
					Do_Projectile_Attack_Named(owner, pnum, "CrossbowBolt_4", wepid, 1, 160, angle_vec, offset_vec, 0, 0, 0);
				break;
				case 4:
					Do_Projectile_Attack_Named(owner, pnum, "CrossbowBolt_5", wepid, 1, 200, angle_vec, offset_vec, 0, 0, 0);
				break;
			}
		break;
		
		// SLOT 4
		case DND_WEAPON_MACHINEGUN:
			use_default = true;
			proj_id = DND_PROJ_MACHINEGUN;
			sp_x = 2.4;
			sp_y = 1.6;
		break;
		case DND_WEAPON_HEAVYMACHINEGUN:
			use_default = true;
			proj_id = DND_PROJ_HEAVYMACHINEGUN;
			
			if(!CheckInventory("WeaponZoom")) {
				sp_x = 4.0;
				sp_y = 2.0;
			}
			else {
				sp_x = 1.6;
				sp_y = 0.8;
			}
		break;
		case DND_WEAPON_LEADSPITTER:
			use_default = true;
			proj_id = DND_PROJ_LEADSPITTER;
			count = 2;
			sp_x = 7.2;
			sp_y = 5.2;
		break;
		case DND_WEAPON_DEMONSEALER:
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				// fire as normal projectile
				use_default = true;
				proj_id = DND_PROJ_DEMONSEALER;
			}
			else {
				use_default = false;
				
				// make as a minion that sets TARGET pointer instead of MASTER
				vec3[offset_vec].z = -36.0; // set it to feet level of player not above!
				Do_Minion_Summon(owner, "DemonSealTrap", offset_vec, 64, KANJI_TRAP_TID + pnum, DND_MF_MAKETARGET);
			}
		break;
		case DND_WEAPON_DESOLATOR:
			use_default = true;
			proj_id = DND_PROJ_DESOLATOR;
			sp_x = 8.4;
			sp_y = 6.2;
		break;
		case DND_WEAPON_MINIGUN:
			use_default = true;
			proj_id = DND_PROJ_MINIGUN;
			count = 2; // actually fires 2 bullets for high bpm
			sp_x = 10.5;
			sp_y = 5.25;
		break;
		case DND_WEAPON_EBONYCANNON:
			use_default = false;
			
			if(!(isAltFire & DND_ATK_SECONDARY))
				proj_id = DND_PROJ_EBONY_1;
			else {
				proj_id = DND_PROJ_EBONY_2;
			}
			
			// 4 pattern of ebony balls or circle
			if(!CheckUniquePropertyOnPlayer(pnum, INV_EX_BEHAVIOR_PELLETSFIRECIRCLE)) {
				SetVec3XYZ(offset_vec, 0, 10.0, 8.0);
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
				SetVec3XYZ(offset_vec, 0, -10.0, 8.0);
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
				SetVec3XYZ(offset_vec, 0, 0, 18.0);
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
				SetVec3XYZ(offset_vec, 0, 0, -2.0);
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
			}
			else
				Do_Attack_Circle(owner, pnum, proj_id, wepid, GetPelletCount(pnum, 4), ProjectileInfo[proj_id].spd_range, flags);
		break;
		case DND_WEAPON_MPPB:
			use_default = true;
			sp_x = 8.0;
			sp_y = 5.0;
			vec3[offset_vec].z = 2.0; // 6.0 z offset, we have default 36.0 from 32.0 so
			
			count = CheckInventory("MPPB_Phase");
			switch(count) {
				case 0:
					proj_id = DND_PROJ_MPPB_1;
				break;
				case 1:
					proj_id = DND_PROJ_MPPB_2;
				break;
				case 2:
					proj_id = DND_PROJ_MPPB_3;
				break;
			}
			
			if(count == 2)
				SetInventory("MPPB_Phase", 0);
			else
				GiveInventory("MPPB_Phase", 1);
			count = 1;
		break;
		case DND_WEAPON_TEMPLARMG:
			use_default = true;
			proj_id = DND_PROJ_TEMPLARMG;
			sp_x = 4.4;
			sp_y = 2.8;
		break;
		case DND_WEAPON_RIOTCANNON:
			use_default = true;
			proj_id = DND_PROJ_RIOTGUN;
			count = 7;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				sp_x = 3.6;
				sp_y = 3.6;
			}
			else {
				sp_x = 7.2;
				sp_y = 7.2;
			}
			
			if(!(flags & DND_ATF_NOAMMOTAKE)) {
				HandleAmmoGainChance(DND_AMMOSLOT_CLIP, AMMO_RIOT, Weapons_Data[wepid].ammo_use1, owner);
				TakeInventory(Weapons_Data[wepid].ammo_name1, Weapons_Data[wepid].ammo_use1);
			}
		break;
		case DND_WEAPON_ACIDRIFLE:
			use_default = true;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				proj_id = DND_PROJ_ACIDBULLET;
				sp_x = 5.6;
				sp_y = 3.2;
			}
			else {
				SetVec3XYZ(offset_vec, 0, 4.0, -12.0);
				proj_id = DND_PROJ_ACIDSTAKE;
			}
		break;
		case DND_WEAPON_FUSIONBLASTER:
			use_default = true;
			vec3[offset_vec].y = 8.0;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				proj_id = DND_PROJ_FUSIONBLASTER;
				sp_x = 2.4;
				sp_y = 1.6;
			}
			else {
				proj_id = DND_PROJ_FUSIONBLASTERGRENADE;
				vec2[angle_vec].x = 1.0;
			}
		break;
		case DND_WEAPON_INCINERATOR:
			use_default = true;
			proj_id = DND_PROJ_INCINERATOR;
			count = 11;
			sp_x = 9.0;
			sp_y = 4.8;
			
			GiveInventory("IncineratorStacks", 1);
			if(CheckInventory("IncineratorStacks") == 4) {
				SetInventory("IncineratorStacks", 0);
				
				// fire the two extra lava balls
				vec3[offset_vec].y = -18.0;
				Do_Projectile_Attack_Named(owner, pnum, "IncineratorProjectile2", wepid, 1, 32, angle_vec, offset_vec, 0, 0, 0);
				vec3[offset_vec].y = 18.0;
				Do_Projectile_Attack_Named(owner, pnum, "IncineratorProjectile2", wepid, 1, 32, angle_vec, offset_vec, 0, 0, 0);
			}
			
			vec3[offset_vec].y = 0.0;
		break;
		
		// SLOT 5
		case DND_WEAPON_ROCKETLAUNCHER:
			use_default = true;
			proj_id = DND_PROJ_ROCKETLAUNCHER;
		break;
		case DND_WEAPON_TORPEDOLAUNCHER:
			use_default = true;
			proj_id = DND_PROJ_TORPEDOLAUNCHER;
		break;
		case DND_WEAPON_VINDICATOR:
			use_default = true;
			proj_id = DND_PROJ_VINDICATOR;
			count = 10;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				sp_x = 10.4;
				sp_y = 7.8;
			}
			else {
				sp_x = 5.2;
				sp_y = 3.9;
			}
		break;
		case DND_WEAPON_HEAVYMISSILELAUNCHER:
			use_default = false;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				vec2[angle_vec].x = -4.0;
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HEAVYMISSILELAUNCHER_1, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
				vec2[angle_vec].x = 4.0;
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HEAVYMISSILELAUNCHER_1, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
			}
			else {
				vec3[offset_vec].y = -8.0;
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HEAVYMISSILELAUNCHER_2, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
				vec3[offset_vec].y = 8.0;
				Do_Projectile_Attack(owner, pnum, DND_PROJ_HEAVYMISSILELAUNCHER_2, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
			}
		break;
		case DND_WEAPON_SEDRINSTAFF:
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				use_default = true;
				proj_id = DND_PROJ_SEDRIN_1;
			}
			else {
				use_default = false;
				proj_id = DND_PROJ_SEDRIN_2;
				
				vec2[angle_vec].x = -9.25;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
				vec2[angle_vec].x = -0;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
				vec2[angle_vec].x = 9.25;
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, 1, angle_vec, offset_vec, 0, 0, 0);
			}
		break;
		case DND_WEAPON_MERCURYLAUNCHER:
			use_default = true;
			proj_id = DND_PROJ_MERCURY;
			vec2[angle_vec].x = 1.0;
		break;
		case DND_WEAPON_METEORLAUNCHER:
			use_default = true;
			proj_id = DND_PROJ_METEORLAUNCHER;
		break;
		case DND_WEAPON_HEAVYGL:
			use_default = true;
			proj_id = DND_PROJ_HEAVYGRENADE;
			count = CheckInventory("HeavyGLCounter") + 1;
			
			if(count > 1) {
				sp_x = 4.8;
				sp_y = 2.0;
				SetInventory("HeavyGLCounter", 0);
				
				// this is new
				flags |= DND_ATF_CANFIRECIRCLE;
			}
		break;
		case DND_WEAPON_FREEZER:
			use_default = true;
			proj_id = DND_PROJ_FREEZER;
			if(!(isAltFire & DND_ATK_OTHER_DIR))
				GiveInventory("FreezerOverheat", 5);
			else {
				GiveInventory("FreezerOverheat", 10);
				sp_x = 2.0;
				sp_y = 2.0;
			}
		break;
		case DND_WEAPON_GRAVDIS:
			use_default = true;
			proj_id = DND_PROJ_GRAVDIS;
		break;
		case DND_WEAPON_VOIDCANNON:
			use_default = true;
			proj_id = DND_PROJ_VOIDCANNON;
		break;
		
		// SLOT 6
		case DND_WEAPON_PLASMARIFLE:
			use_default = true;
			proj_id = DND_PROJ_PLASMA;
		break;
		case DND_WEAPON_NUCLEARPLASMARIFLE:
			use_default = true;
			proj_id = DND_PROJ_NUCLEARPLASMA;
			GiveInventory("PlasmaOverheat", 2);
			if(CheckInventory("PlasmaOverheat") == 100)
				GiveInventory("PlasmaOverheatCooldown", 1);
		break;
		case DND_WEAPON_TURELCANNON:
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				if(isAltFire & DND_ATK_OTHER_DIR)
					Do_Railgun_Attack("TurelShooter2", 1);
				else
					Do_Railgun_Attack("TurelShooter1", 1);
			}
			else {
				Do_Railgun_Attack("TurelShooter3", 1);
				ammo_take_amt <<= 1; // double ammo on this one
			}
		break;
		case DND_WEAPON_NAILGUN:
			use_default = true;
			
			// proj type
			if(isAltFire & DND_ATK_SECONDARY)
				proj_id = DND_PROJ_NAILGUN_2;
			else
				proj_id = DND_PROJ_NAILGUN_1;
			
			// left right offset
			if(CheckInventory("NailCounter")) {
				SetInventory("NailCounter", 0);
				vec3[offset_vec].y = 6.0;
			}
			else {
				GiveInventory("NailCounter", 1);
				vec3[offset_vec].y = -6.0;
			}			
			
			GiveInventory("NailCounter", 1);
		break;
		case DND_WEAPON_BASILISK:
			use_default = true;
			
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				proj_id = DND_PROJ_BASILISK_1;
				sp_x = 2.5;
				sp_y = 2.0;
			}
			else {
				sp_x = 3.25;
				sp_y = 2.75;
			}
		break;
		case DND_WEAPON_FROSTFANG:
			use_default = true;
			if(!(isAltFire & DND_ATK_SECONDARY)) {
				proj_id = DND_PROJ_FROSTFANG_1;
				SetVec3XYZ(offset_vec, 0, random(-7.5, 7.5), random(-7.5, 7.5));
			}
			else
				proj_id = DND_PROJ_FROSTFANG_2;
		break;
		case DND_WEAPON_RHINORIFLE:
			use_default = true;
			proj_id = DND_PROJ_RHINO;
			if(!(isAltFire & DND_ATK_OTHER_DIR)) {
				sp_x = 7.2;
				sp_y = 4.8;
			}
			else {
				sp_x = 0.75;
				sp_y = 0.75;
			}
		break;
		case DND_WEAPON_FLAMETHROWER:
			use_default = true;
			proj_id = DND_PROJ_FLAMETHROWER;
			sp_x = 4.0;
		break;
		
		
		// SLOT 7
		case DND_WEAPON_DEVASTATOR:
			proj_id = DND_PROJ_DEVASTATOR;
			use_default = true;
			count = 5;
			sp_x = 4.8;
			sp_y = 2.8;
			
			if(isAltfire & DND_ATK_OTHER_DIR)
				SetVec3XYZ(offset_vec, 0.0, 12.0, 0.0);
			else
				SetVec3XYZ(offset_vec, 0.0, -12.0, 0.0);
		break;
	}
	
	// take away ammo if each shot isn't using ammo and we don't have the "no ammo take" flag
	if(!(flags & DND_ATF_NOAMMOTAKE))
		TakeInventory(ammo_type, ammo_take_amt);
	
	// moved doesn't have artemis check to here for clarity
	if(!(flags & DND_ATF_NOAMMOGAINCHECK) && !hasArtemis)
		HandleAmmoGainChance(ammo_slot, ammo_sub_slot, ammo_take_amt, owner);
		
	// NOTE: This rolls a crit chance to be used by the subsequent things below! Disabling it with the flag may remove crits!
	if(!(flags & DND_ATF_NOATTACKTRIGGER))
		HandleAttackEvent(ammo_slot == DND_AMMOSLOT_SPECIAL, ammo_sub_slot > SSAM_SLUG ? ammo_sub_slot - 1 : ammo_sub_slot);
	
	// for generic weapons that fire single shots these few lines will suffice, special spread weapons will have specifically crafted code for their attack
	if(proj_id != -1 && use_default) {
		// scale count if its a shotgun and/or we got pellets fire circle with non-zero count
		// every weapon that can fire in a circle is either a burst or a shotgun! -- this is the underlying assumption so far!
		if(count > 1 && (flags & DND_ATF_CANFIRECIRCLE))
			count = GetPelletCount(pnum, count);
	
		if(!CheckUniquePropertyOnPlayer(pnum, INV_EX_BEHAVIOR_PELLETSFIRECIRCLE) || !(flags & DND_ATF_CANFIRECIRCLE)) {
			// projectile, hitscan or melee: Check and do their corresponding attack function
			if(ProjectileInfo[proj_id].flags & DND_PROJ_HITSCAN)
				Do_Hitscan_Attack(owner, pnum, proj_id, wepid, count, ProjectileInfo[proj_id].spd_range, sp_x, sp_y, flags);
			else
				Do_Projectile_Attack(owner, pnum, proj_id, wepid, count, angle_vec, offset_vec, sp_x, sp_y, flags);
		}
		else // let circle attack decide what kind of attack we need to do on its own
			Do_Attack_Circle(owner, pnum, proj_id, wepid, count, ProjectileInfo[proj_id].spd_range, flags);
	}
	
	FreeVec2(angle_vec);
	FreeVec3(offset_vec);
	
	SetResultValue(0);
}

Script "DnD Load Weapon Information" OPEN {
	SetupProjectileData();
	SetupWeaponData();
}

Script "DnD Load Weapon Information CS" OPEN CLIENTSIDE {
	// clients dont need proj data so far!
	//SetupProjectileData();
	SetupWeaponData();
}

#endif
