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

#endif