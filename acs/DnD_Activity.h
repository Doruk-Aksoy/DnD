#ifndef DND_ACTIVITY_IN
#define DND_ACTIVITY_IN

/* 
	Hold any kind of player **ORB** activity here.
	
	List of activities to be recorded:
	
		* Exp gain
		* Credit gain
		* Budget gain from orb
		* Level gain
		* Free talent points
		* Perk X change
		* Attribute X change
		* Unused attribute points or perk points
		* Bonuses from orbs obtained
		* Player's inventory is saved after each change forcibly. No way around it. I don't want to keep two copies of the whole inventory...

	In the future, maybe record everything? Might be too resource demanding.
*/

typedef struct {
	int exp;
	int credit;
	int budget;
	int level;
	int perk_change[DND_MAX_PERKS];
	int attribute_change[DND_MAX_ATTRIBUTES];
	int free_perks;
	int free_attributes;
	
	int char_id;
	int stash_pages;
	
	orb_max_info_T orb_change;
} player_activity_T;

global player_activity_T 18: PlayerActivities[MAXPLAYERS];

enum {
	DND_ACTIVITY_EXP,
	DND_ACTIVITY_CREDIT,
	DND_ACTIVITY_BUDGET,
	DND_ACTIVITY_LEVEL,
	DND_ACTIVITY_TALENT,
	DND_ACTIVITY_PERK,
	DND_ACTIVITY_ATTRIBUTE,
	DND_ACTIVITY_PERKPOINT,
	DND_ACTIVITY_ATTRIBUTEPOINT,
	
	// same order as in dnd_orbs.h
	DND_ACTIVITY_ORB_HPFLAT,
	DND_ACTIVITY_ORB_ARMORFLAT,
	
	DND_ACTIVITY_ORB_HPPERCENT,
	DND_ACTIVITY_ORB_ARMORPERCENT,
	
	DND_ACTIVITY_ORB_GREED,
	DND_ACTIVITY_ORB_WISDOM,
	
	DND_ACTIVITY_ORB_SPEED,
	DND_ACTIVITY_ORB_DROPCHANCE,
	DND_ACTIVITY_ORB_HOLDING,
	DND_ACTIVITY_ORB_DAMAGETYPE,
	
	DND_ACTIVITY_ORB_WEAPONENCHANT,
	DND_ACTIVITY_ORB_WEAPONBONUS_CRIT,
	DND_ACTIVITY_ORB_WEAPONBONUS_CRITDAMAGE,
	DND_ACTIVITY_ORB_WEAPONBONUS_CRITPERCENT,
	DND_ACTIVITY_ORB_WEAPONBONUS_DAMAGE,
	DND_ACTIVITY_ORB_WEAPONBONUS_POWERSET1
};
#define MAP_ORB_TO_ACTIVITY (DND_ACTIVITY_ORB_HPFLAT - DND_ACTIVITY_EXP)

void UpdateActivity(int pnum, int activity, int val, int extra, bool overwrite = false, bool removeBit = false) {
	switch(activity) {
		case DND_ACTIVITY_EXP:
			PlayerActivities[pnum].exp = val;
		break;
		case DND_ACTIVITY_CREDIT:
			PlayerActivities[pnum].credit += val;
		break;
		case DND_ACTIVITY_BUDGET:
			PlayerActivities[pnum].budget += val;
		break;
		case DND_ACTIVITY_LEVEL:
			PlayerActivities[pnum].level += val;
		break;
		case DND_ACTIVITY_PERK:
			PlayerActivities[pnum].perk_change[extra] += val;
			//printbold(s:"activity for perk ", d:extra, s: " became ", d:PlayerActivities[pnum].perk_change[extra]);
		break;
		case DND_ACTIVITY_ATTRIBUTE:
			PlayerActivities[pnum].attribute_change[extra] += val;
			//printbold(s:"activity for attribute ", d:extra, s: " became ", d:PlayerActivities[pnum].attribute_change[extra]);
		break;
		case DND_ACTIVITY_PERKPOINT:
			PlayerActivities[pnum].free_perks += val;
		break;
		case DND_ACTIVITY_ATTRIBUTEPOINT:
			PlayerActivities[pnum].free_attributes += val;
		break;
		
		case DND_ACTIVITY_ORB_HPFLAT:
			PlayerActivities[pnum].orb_change.orb_stat_bonuses.hp_flat_bonus += val;
		break;
		case DND_ACTIVITY_ORB_HPPERCENT:
			PlayerActivities[pnum].orb_change.orb_stat_bonuses.hp_percent_bonus += val;
		break;
		case DND_ACTIVITY_ORB_ARMORFLAT:
			PlayerActivities[pnum].orb_change.orb_stat_bonuses.armor_flat_bonus += val;
		break;
		case DND_ACTIVITY_ORB_ARMORPERCENT:
			PlayerActivities[pnum].orb_change.orb_stat_bonuses.armor_percent_bonus += val;
		break;
		case DND_ACTIVITY_ORB_GREED:
			PlayerActivities[pnum].orb_change.orb_stat_bonuses.greed_percent_bonus += val;
		break;
		case DND_ACTIVITY_ORB_WISDOM:
			PlayerActivities[pnum].orb_change.orb_stat_bonuses.wisdom_percent_bonus += val;
		break;
		case DND_ACTIVITY_ORB_SPEED:
			PlayerActivities[pnum].orb_change.orb_stat_bonuses.speed_bonus += val;
		break;
		case DND_ACTIVITY_ORB_DROPCHANCE:
			PlayerActivities[pnum].orb_change.orb_stat_bonuses.drop_chance += val;
		break;
		case DND_ACTIVITY_ORB_HOLDING:
			PlayerActivities[pnum].orb_change.orb_stat_bonuses.holding += val;
		break;
		case DND_ACTIVITY_ORB_DAMAGETYPE:
			PlayerActivities[pnum].orb_change.orb_stat_bonuses.damage_type_bonus[extra] += val;
		break;
		
		case DND_ACTIVITY_ORB_WEAPONENCHANT:
			PlayerActivities[pnum].orb_change.weapon_stat_bonuses[extra].quality += val;
			//if(extra == DND_WEAPON_PLASMARIFLE)
			//	printbold(s:"update to activity: ", d:PlayerActivities[pnum].orb_change.weapon_stat_bonuses[extra].quality, s: " +", d:val, s:" to wep: ", d:extra);
		break;
		case DND_ACTIVITY_ORB_WEAPONBONUS_DAMAGE:
			PlayerActivities[pnum].orb_change.weapon_stat_bonuses[extra].wep_mods[WEP_MOD_DMG].val += val;
		break;
		case DND_ACTIVITY_ORB_WEAPONBONUS_CRIT:
			PlayerActivities[pnum].orb_change.weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRIT].val += val;
		break;
		case DND_ACTIVITY_ORB_WEAPONBONUS_CRITDAMAGE:
			PlayerActivities[pnum].orb_change.weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRITDMG].val += val;
		break;
		case DND_ACTIVITY_ORB_WEAPONBONUS_CRITPERCENT:
			PlayerActivities[pnum].orb_change.weapon_stat_bonuses[extra].wep_mods[WEP_MOD_CRITPERCENT].val += val;
		break;
		case DND_ACTIVITY_ORB_WEAPONBONUS_POWERSET1:
			if(!overwrite) {
				if(!removeBit)
					PlayerActivities[pnum].orb_change.weapon_stat_bonuses[extra].wep_mods[WEP_MOD_POWERSET1].val |= val;
				else
					PlayerActivities[pnum].orb_change.weapon_stat_bonuses[extra].wep_mods[WEP_MOD_POWERSET1].val &= ~(1 << (-val));
			}
			else
				PlayerActivities[pnum].orb_change.weapon_stat_bonuses[extra].wep_mods[WEP_MOD_POWERSET1].val = val;
		break;
	}
}

void ResetPlayerActivities(int pnum, bool hardReset) {
	int i, j;
	PlayerActivities[pnum].exp = 0;
	PlayerActivities[pnum].credit = 0;
	PlayerActivities[pnum].budget = 0;
	PlayerActivities[pnum].level = 0;
	PlayerActivities[pnum].free_perks = 0;
	PlayerActivities[pnum].free_attributes = 0;
	
	if(hardReset) {
		// only if someone quits or spectates should the hard reset be resetting these
		PlayerActivities[pnum].char_id = 0;
		PlayerActivities[pnum].stash_pages = 0;
	}
	
	for(i = 0; i < DND_MAX_PERKS; ++i)
		PlayerActivities[pnum].perk_change[i] = 0;
	for(i = 0; i < DND_MAX_ATTRIBUTES; ++i)
		PlayerActivities[pnum].attribute_change[i] = 0;
		
	PlayerActivities[pnum].orb_change.orb_stat_bonuses.hp_flat_bonus = 0;
	PlayerActivities[pnum].orb_change.orb_stat_bonuses.armor_flat_bonus = 0;
	PlayerActivities[pnum].orb_change.orb_stat_bonuses.hp_percent_bonus = 0;
	PlayerActivities[pnum].orb_change.orb_stat_bonuses.armor_percent_bonus = 0;
	PlayerActivities[pnum].orb_change.orb_stat_bonuses.greed_percent_bonus = 0;
	PlayerActivities[pnum].orb_change.orb_stat_bonuses.wisdom_percent_bonus = 0;
	PlayerActivities[pnum].orb_change.orb_stat_bonuses.speed_bonus = 0;
	PlayerActivities[pnum].orb_change.orb_stat_bonuses.drop_chance = 0;
	PlayerActivities[pnum].orb_change.orb_stat_bonuses.holding = 0;

	for(i = 0; i < MAX_DAMAGE_CATEGORIES; ++i)
		PlayerActivities[pnum].orb_change.orb_stat_bonuses.damage_type_bonus[i] = 0;

	for(i = 0; i < MAXWEPS; ++i) {
		PlayerActivities[pnum].orb_change.weapon_stat_bonuses[i].quality = 0;
		for(j = 0; j < MAX_WEP_MODS; ++j) {
			PlayerActivities[pnum].orb_change.weapon_stat_bonuses[i].wep_mods[j].val = 0;
			PlayerActivities[pnum].orb_change.weapon_stat_bonuses[i].wep_mods[j].tier = 0;
		}
	}
}

#endif
