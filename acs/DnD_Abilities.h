#ifndef DND_ABILITY_IN
#define DND_ABILITY_IN

enum {
	ABILITY_KICK,
	ABILITY_RELOADER,
	ABILITY_DASH,
	ABILITY_ARCANERY,
	ABILITY_ANTIPOISON,
	ABILITY_EXPLOSIONMASTERY,
	ABILITY_HEARTSEEKER,
	ABILITY_REGEN,
	ABILITY_TEMPORAL,
	ABILITY_SOULSTEALER,
};

#define MAXABILITIES (ABILITY_SOULSTEALER + 1)

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

#endif
