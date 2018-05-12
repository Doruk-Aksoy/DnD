#ifndef DND_SPECIALTRAILS_IN
#define DND_SPECIALTRAILS_IN

enum {
	DND_TRAILACTOR_RUBYFLAME,
	DND_TRAILACTOR_THUNDERZAP
};
#define MAXTRAILACTORS DND_TRAILACTOR_THUNDERZAP + 1

typedef struct traildef {
	str actor;
	int waittime;
	int maxdiff;
} traildef_T;

traildef_T DnD_TrailDef[MAXTRAILACTORS] = {
	{ "RubyFlameTrail", 2, 0 },
	{ "SmallZapPuff", 0, 16 }
};

str RailGunTrails[3] = { "RedRayTrail", "YellowRayTrail", "BlueRayTrail" };
str RailGunDamagers[3] = { "RailDamager_Red", "RailDamager_Yellow", "RailDamager_Blue" };
str RailGunDamagers_Full[3] = { "RailDamager_Red_FullDMG", "RailDamager_Yellow_FullDMG", "RailDamager_Blue_FullDMG" };

#endif