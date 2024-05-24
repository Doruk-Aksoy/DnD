#ifndef DND_WEAPONMOD_IN
#define DND_WEAPONMOD_IN

enum {
	WEP_MOD_TIER_LOW,
	WEP_MOD_TIER_MEDIUM,
	WEP_MOD_TIER_HIGH
};
#define MAX_WEP_MODTIER (WEP_MOD_TIER_HIGH + 1)

// powersets
enum {
	WEP_POWER_GHOSTHIT
};
#define MAX_WEP_MOD_POWERSET1 (WEP_POWER_GHOSTHIT + 1)

int GetPlayerWeaponQuality(int pnum, int wep) {
	return Player_Weapon_Infos[pnum][wep].quality;
}

void SetPlayerWeaponQuality(int pnum, int wep, int val) {
	Player_Weapon_Infos[pnum][wep].quality = val;
	Player_Weapon_Infos[pnum][wep].needs_save = true;
}

// gives a particular powerset to the weapon
void SetWeaponModPowerset(int pnum, int wepid, int powerset, bool val, int source) {
	if(val)
		Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_POWERSET1][source].val |= 1 << powerset;
	else
		Player_Weapon_Infos[pnum][wepid].wep_mods[WEP_MOD_POWERSET1][source].val &= ~(1 << powerset);
	Player_Weapon_Infos[pnum][wepid].needs_save = true;
}

// This will always return the sum of all sources, as the "net" value for use in calculations
int GetPlayerWeaponModVal(int pnum, int wepid, int wepmod) {
	return Player_Weapon_Infos[pnum][wepid].wep_mods[wepmod][WMOD_ITEMS].val + Player_Weapon_Infos[pnum][wepid].wep_mods[wepmod][WMOD_WEP].val;
}

#define WEP_POWER_GHOSTHIT_REDUCE 33 // %

str GetWeaponModText(int val, int id, int extra_bit = 0) {
	switch(id) {
		case WEP_MOD_CRIT:
			return StrParam(s:"\c[Y5]* \c[Q9]", s:GetFixedRepresentation(val, true), s:"% ", l:"IATTR_T37");
		case WEP_MOD_CRITDMG:
			return StrParam(s:"\c[Y5]* \c[Q9]", d:val, s:"% ", l:"IATTR_T39");
		case WEP_MOD_CRITPERCENT:
			return StrParam(s:"\c[Y5]* \c[Q9]", s:GetFixedRepresentation(val, true), s:"% ", l:"IATTR_T38");
		case WEP_MOD_DMG:
			if(val > 0)
				return StrParam(s:"\c[Y5]* \c[Q9]", d:val, s:"% ", l:"IATTR_T41");
			// put - here because it already says "less", -x% less is weird
			return StrParam(s:"\c[Y5]* \c[Q2]", d:-val, s:"% ", l:"IATTR_T41NEG");
		case WEP_MOD_PERCENTDAMAGE:
			return StrParam(s:"\c[Y5]* \c[Q9]", d:val, s:"% ", l:"DND_WEPMOD_PERCENTDMG");
		case WEP_MOD_POISONFORPERCENTDAMAGE:
			return StrParam(s:"\c[Y5]* \c[Q9]", d:val, s:"% ", l:"DND_WEPMOD_POISONHITS");
		case WEP_MOD_FORCEPAINCHANCE:
			return StrParam(s:"\c[Y5]* \c[Q9]", d:val, s:"% ", l:"DND_WEPMOD_CHANCEFORCEPAIN");
		case WEP_MOD_POWERSET1:
			// use extra_bit to check which sub mods we are after
			switch(extra_bit) {
				case WEP_POWER_GHOSTHIT:
					return StrParam(s:"\c[Y5]* \c[Q9]", l:"DND_WEPMOD_POW_GHOSTHIT");
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
