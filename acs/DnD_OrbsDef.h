#ifndef DND_ORBSDEF_IN
#define DND_ORBSDEF_IN

#include "DnD_WeaponDefs.h"

// kept seperate to reduce dependancy

typedef struct {
	int hp_flat_bonus;
	int armor_flat_bonus;
	
	int hp_percent_bonus;
	int armor_percent_bonus;
	
	int greed_percent_bonus;
	int wisdom_percent_bonus;
	
	int speed_bonus;
	int drop_chance;
	int holding;
	
	int damage_type_bonus[MAX_DAMAGE_CATEGORIES];
} orb_stat_T;

typedef struct {
	orb_stat_T orb_stat_bonuses;
	wep_info_T weapon_stat_bonuses[MAXWEPS];
} orb_max_info_T;

global orb_max_info_T 8: Player_Orb_Data[MAXPLAYERS];

// orb bonus info
enum {
	OBI_HPFLAT,
	OBI_ARMORFLAT,
	
	OBI_HPPERCENT,
	OBI_ARMORPERCENT,
	
	OBI_GREEDPERCENT,
	OBI_WISDOMPERCENT,

	OBI_SPEED,
	OBI_DROPCHANCE,
	OBI_HOLDING,
	OBI_DAMAGETYPE,
	
	OBI_WEAPON_ENCHANT,
	OBI_WEAPON_CRIT,
	OBI_WEAPON_CRITDMG,
	OBI_WEAPON_CRITPERCENT,
	OBI_WEAPON_DMG,
	OBI_WEAPON_POWERSET1 // this particular one is used like a bitset of 32 bits
};

int GetDataFromOrbBonus(int pnum, int bonus, int extra = 0) {
	int res = 0;
	switch(bonus) {
		case OBI_HPFLAT:
			res = Player_Orb_Data[pnum].orb_stat_bonuses.hp_flat_bonus;
		break;
		case OBI_ARMORFLAT:
			res = Player_Orb_Data[pnum].orb_stat_bonuses.armor_flat_bonus;
		break;
		case OBI_HPPERCENT:
			res = Player_Orb_Data[pnum].orb_stat_bonuses.hp_percent_bonus;
		break;
		case OBI_ARMORPERCENT:
			res = Player_Orb_Data[pnum].orb_stat_bonuses.armor_percent_bonus;
		break;
		case OBI_WISDOMPERCENT:
			res = Player_Orb_Data[pnum].orb_stat_bonuses.wisdom_percent_bonus;
		break;
		case OBI_GREEDPERCENT:
			res = Player_Orb_Data[pnum].orb_stat_bonuses.greed_percent_bonus;
		break;
		case OBI_SPEED:
			res = Player_Orb_Data[pnum].orb_stat_bonuses.speed_bonus;
		break;
		case OBI_DROPCHANCE:
			res = Player_Orb_Data[pnum].orb_stat_bonuses.drop_chance;
		break;
		case OBI_HOLDING:
			res = Player_Orb_Data[pnum].orb_stat_bonuses.holding;
		break;
		case OBI_DAMAGETYPE:
			res = Player_Orb_Data[pnum].orb_stat_bonuses.damage_type_bonus[extra];
		break;
		
		case OBI_WEAPON_ENCHANT:
			res = Player_Orb_Data[pnum].weapon_stat_bonuses[extra].quality;
		break;
		case OBI_WEAPON_CRIT:
			res = Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRIT].val;
		break;
		case OBI_WEAPON_CRITDMG:
			res = Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRITDMG].val;
		break;
		case OBI_WEAPON_CRITPERCENT:
			res = Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRITPERCENT].val;
		break;
		case OBI_WEAPON_DMG:
			res = Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_DMG].val;
		break;
		case OBI_WEAPON_POWERSET1:
			res = Player_Orb_Data[pnum].weapon_stat_bonuses[extra].wep_mods[WEP_MOD_POWERSET1].val;
		break;
		// weapon mods can come in the future for orbs that give those
	}
	return res;
}

#endif
