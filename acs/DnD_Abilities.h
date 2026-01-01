#ifndef DND_ABILITY_IN
#define DND_ABILITY_IN

enum {
	ABILITY_PARRYMASTER,
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

str GetAbilityInfo(int id) {
	switch(id) {
		case ABILITY_PARRYMASTER:
		return "Ability_ParryMaster";
		case ABILITY_RELOADER:
		return "Ability_Reloader";
		case ABILITY_DASH:
		return "Ability_Dash";
		case ABILITY_ARCANERY:
		return "Ability_Arcanery";
		case ABILITY_ANTIPOISON:
		return "Ability_AntiPoison";
		case ABILITY_EXPLOSIONMASTERY:
		return "Ability_ExplosionMastery";
		case ABILITY_HEARTSEEKER:
		return "Ability_HeartSeeker";
		case ABILITY_REGEN:
		return"Ability_Regeneration";
		case ABILITY_TEMPORAL:
		return "Ability_Temporal";
		case ABILITY_SOULSTEALER:
		return "Ability_SoulStealer";
	}
	return "";
}

#endif
