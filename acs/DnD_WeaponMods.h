#ifndef DND_WEAPONMOD_IN
#define DND_WEAPONMOD_IN

// weapon mod data, mod_id contains the modifier, low and high are the rolled values
typedef struct {
	int tier;
	int val;
} wep_mod_T;

enum {
	WEP_MOD_TIER_LOW,
	WEP_MOD_TIER_MEDIUM,
	WEP_MOD_TIER_HIGH
};
#define MAX_WEP_MODTIER (WEP_MOD_TIER_HIGH + 1)

enum {
	WEP_MOD_PERCENTDAMAGE,
	WEP_MOD_POISONFORPERCENTDAMAGE,
	WEP_MOD_FORCEPAINCHANCE
};
#define MAX_WEP_MODS (WEP_MOD_FORCEPAINCHANCE + 1) // max allowed mods

str WeaponModTexts[MAX_WEP_MODS] = {
	"% monster health on hits",
	"% of damage dealt as additional poison on hits",
	"% chance to force pain on hits"
};

typedef struct {
	int low;
	int high;
	int tier_add;
} wep_mod_vals_T;

wep_mod_vals_T WeaponModValues[MAX_WEP_MODS] = {
	{ 1, 2, 1 },
	{ 4, 10, 5 },
	{ 5, 10, 10 },
};

int RollWeaponMod(int mod, int tier) {
	int inc = WeaponModValues[mod].tier_add * tier;
	return random(WeaponModValues[mod].low + inc, WeaponModValues[mod].high + inc);
}

#endif
