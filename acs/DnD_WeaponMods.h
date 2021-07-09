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
	WEP_MOD_CRIT,
	WEP_MOD_CRITDMG,
	WEP_MOD_CRITPERCENT,
	WEP_MOD_DMG,
	WEP_MOD_PERCENTDAMAGE,
	WEP_MOD_POISONFORPERCENTDAMAGE,
	WEP_MOD_FORCEPAINCHANCE,
	WEP_MOD_POWERSET1 // holds boolean style modifiers
};
#define MAX_WEP_MODS (WEP_MOD_POWERSET1 + 1) // max allowed mods

// powersets
enum {
	WEP_POWER_GHOSTHIT
};

str GetWeaponModText(int val, int id, int extra_bit = 0) {
	switch(id) {
		case WEP_MOD_CRIT:
		return StrParam(d:val, s:"% ", l:"IATTR_T37");
		case WEP_MOD_CRITDMG:
		return StrParam(d:val, s:"% ", l:"IATTR_T39");
		case WEP_MOD_CRITPERCENT:
		return StrParam(d:val, s:"% ", l:"IATTR_T38");
		case WEP_MOD_DMG:
		return StrParam(d:val, s:"% ", l:"IATTR_T41");
		case WEP_MOD_PERCENTDAMAGE:
		return StrParam(d:val, s:"% ", l:"DND_WEPMOD_PERCENTDMG");
		case WEP_MOD_POISONFORPERCENTDAMAGE:
		return StrParam(d:val, s:"% ", l:"DND_WEPMOD_POISONHITS");
		case WEP_MOD_FORCEPAINCHANCE:
		return StrParam(d:val, s:"% ", l:"DND_WEPMOD_CHANCEFORCEPAIN");
		case WEP_MOD_POWERSET1:
			// use extra_bit to check which sub mods we are after
			switch(extra_bit) {
				case WEP_POWER_GHOSTHIT:
				return StrParam(l:"DND_WEPMOD_POW_GHOSTHIT");
			}
		break;
	}
	return "";
}

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
