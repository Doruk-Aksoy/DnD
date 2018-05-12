#ifndef DND_SYNC_IN
#define DND_SYNC_IN

#define DND_SYNC_NONORB 0
#define DND_SYNC_ORB 1

enum {
	DND_SYNC_WEAPONENHANCE,
	
	DND_SYNC_SPEED,
	
	DND_SYNC_DROPCHANCE,
	
	DND_SYNC_HPFLAT_BONUS,
	DND_SYNC_ARMORFLAT_BONUS,
	DND_SYNC_HPPERCENT_BONUS,
	DND_SYNC_ARMORPERCENT_BONUS,
	
	DND_SYNC_GREEDPERCENT_BONUS,
	DND_SYNC_WISDOMPERCENT_BONUS,
	
	DND_SYNC_HOLDING,
	
	DND_SYNC_DAMAGEBULLET,
	DND_SYNC_DAMAGEMELEE,
	DND_SYNC_DAMAGEOCCULT,
	DND_SYNC_DAMAGEEXPLOSIVE,
	DND_SYNC_DAMAGEENERGY,
	DND_SYNC_DAMAGEELEMENTAL,
	
	DND_SYNC_WEPBONUS_CRIT,
	DND_SYNC_WEPBONUS_CRITDMG,
	DND_SYNC_WEPBONUS_CRITPERCENT,
	DND_SYNC_WEPBONUS_DMG
};
#define MAX_SYNC_VARS DND_SYNC_WEPBONUS_DMG + 1

int GetPlayerSyncValue(int pos, int extra, bool isOrb) {
	int pnum = PlayerNumber();
	if(!isOrb) {
		switch(pos) {
			case DND_SYNC_WEAPONENHANCE:
			return Player_Weapon_Infos[pnum][extra].enchants;
			case DND_SYNC_SPEED:
			return Player_Bonuses[pnum].speed_bonus;
			case DND_SYNC_DROPCHANCE:
			return Player_Bonuses[pnum].drop_chance;
			case DND_SYNC_HPFLAT_BONUS:
			return Player_Bonuses[pnum].hp_flat_bonus;
			case DND_SYNC_ARMORFLAT_BONUS:
			return Player_Bonuses[pnum].armor_flat_bonus;
			case DND_SYNC_HPPERCENT_BONUS:
			return Player_Bonuses[pnum].hp_percent_bonus;
			case DND_SYNC_ARMORPERCENT_BONUS:
			return Player_Bonuses[pnum].armor_percent_bonus;
			case DND_SYNC_GREEDPERCENT_BONUS:
			return Player_Bonuses[pnum].greed_percent_bonus;
			case DND_SYNC_WISDOMPERCENT_BONUS:
			return Player_Bonuses[pnum].wisdom_percent_bonus;
			case DND_SYNC_HOLDING:
			return Player_Bonuses[pnum].holding;
			case DND_SYNC_DAMAGEBULLET:
			return Player_Bonuses[pnum].damage_type_bonus[TALENT_BULLET];
			case DND_SYNC_DAMAGEENERGY:
			return Player_Bonuses[pnum].damage_type_bonus[TALENT_ENERGY];
			case DND_SYNC_DAMAGEEXPLOSIVE:
			return Player_Bonuses[pnum].damage_type_bonus[TALENT_EXPLOSIVE];
			case DND_SYNC_DAMAGEMELEE:
			return Player_Bonuses[pnum].damage_type_bonus[TALENT_MELEE];
			case DND_SYNC_DAMAGEELEMENTAL:
			return Player_Bonuses[pnum].damage_type_bonus[TALENT_ELEMENTAL];
			case DND_SYNC_DAMAGEOCCULT:
			return Player_Bonuses[pnum].damage_type_bonus[TALENT_OCCULT];
			case DND_SYNC_WEPBONUS_CRIT:
			return Player_Weapon_Infos[pnum][extra].wep_bonuses[WEP_BONUS_CRIT].amt;
			case DND_SYNC_WEPBONUS_CRITDMG:
			return Player_Weapon_Infos[pnum][extra].wep_bonuses[WEP_BONUS_CRITDMG].amt;
			case DND_SYNC_WEPBONUS_CRITPERCENT:
			return Player_Weapon_Infos[pnum][extra].wep_bonuses[WEP_BONUS_CRITPERCENT].amt;
			case DND_SYNC_WEPBONUS_DMG:
			return Player_Weapon_Infos[pnum][extra].wep_bonuses[WEP_BONUS_DMG].amt;
		}
	}
	else {
		switch(pos) {
			case DND_SYNC_WEAPONENHANCE:
			return GetDataFromOrbBonus(pnum, OBI_WEAPON_ENCHANT, extra);
			case DND_SYNC_SPEED:
			return GetDataFromOrbBonus(pnum, OBI_SPEED, -1);
			case DND_SYNC_DROPCHANCE:
			return GetDataFromOrbBonus(pnum, OBI_DROPCHANCE, -1);
			case DND_SYNC_HPFLAT_BONUS:
			return GetDataFromOrbBonus(pnum, OBI_HPFLAT, -1);
			case DND_SYNC_ARMORFLAT_BONUS:
			return GetDataFromOrbBonus(pnum, OBI_ARMORFLAT, -1);
			case DND_SYNC_HPPERCENT_BONUS:
			return GetDataFromOrbBonus(pnum, OBI_HPPERCENT, -1);
			case DND_SYNC_ARMORPERCENT_BONUS:
			return GetDataFromOrbBonus(pnum, OBI_ARMORPERCENT, -1);
			case DND_SYNC_GREEDPERCENT_BONUS:
			return GetDataFromOrbBonus(pnum, OBI_GREEDPERCENT, -1);
			case DND_SYNC_WISDOMPERCENT_BONUS:
			return GetDataFromOrbBonus(pnum, OBI_WISDOMPERCENT, -1);
			case DND_SYNC_HOLDING:
			return GetDataFromOrbBonus(pnum, OBI_HOLDING, -1);
			case DND_SYNC_DAMAGEBULLET:
			return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_BULLET);
			case DND_SYNC_DAMAGEENERGY:
			return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_ENERGY);
			case DND_SYNC_DAMAGEEXPLOSIVE:
			return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_EXPLOSIVE);
			case DND_SYNC_DAMAGEMELEE:
			return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_MELEE);
			case DND_SYNC_DAMAGEELEMENTAL:
			return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_ELEMENTAL);
			case DND_SYNC_DAMAGEOCCULT:
			return GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_OCCULT);
			case DND_SYNC_WEPBONUS_CRIT:
			return GetDataFromOrbBonus(pnum, OBI_WEAPON_CRIT, extra);
			case DND_SYNC_WEPBONUS_CRITDMG:
			return GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, extra);
			case DND_SYNC_WEPBONUS_CRITPERCENT:
			return GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITPERCENT, extra);
			case DND_SYNC_WEPBONUS_DMG:
			return GetDataFromOrbBonus(pnum, OBI_WEAPON_DMG, extra);
		}
	}
	return 0;
}

void SetSyncValue(int pos, int val, int extra, bool isOrb) {
	int pnum = PlayerNumber();
	if(!isOrb) {
		switch(pos) {
			case DND_SYNC_WEAPONENHANCE:
				Player_Weapon_Infos[pnum][extra].enchants = val;
			break;
			case DND_SYNC_SPEED:
				Player_Bonuses[pnum].speed_bonus = val;
			break;
			case DND_SYNC_DROPCHANCE:
				Player_Bonuses[pnum].drop_chance = val;
			break;
			case DND_SYNC_HPFLAT_BONUS:
				Player_Bonuses[pnum].hp_flat_bonus = val;
			break;
			case DND_SYNC_ARMORFLAT_BONUS:
				Player_Bonuses[pnum].armor_flat_bonus = val;
			break;
			case DND_SYNC_HPPERCENT_BONUS:
				Player_Bonuses[pnum].hp_percent_bonus = val;
			break;
			case DND_SYNC_ARMORPERCENT_BONUS:
				Player_Bonuses[pnum].armor_percent_bonus = val;
			break;
			case DND_SYNC_GREEDPERCENT_BONUS:
				Player_Bonuses[pnum].greed_percent_bonus = val;
			break;
			case DND_SYNC_WISDOMPERCENT_BONUS:
				Player_Bonuses[pnum].wisdom_percent_bonus = val;
			break;
			case DND_SYNC_HOLDING:
				Player_Bonuses[pnum].holding = val;
			break;
			case DND_SYNC_DAMAGEBULLET:
				Player_Bonuses[pnum].damage_type_bonus[TALENT_BULLET] = val;
			break;
			case DND_SYNC_DAMAGEENERGY:
				Player_Bonuses[pnum].damage_type_bonus[TALENT_ENERGY] = val;
			break;
			case DND_SYNC_DAMAGEEXPLOSIVE:
				Player_Bonuses[pnum].damage_type_bonus[TALENT_EXPLOSIVE] = val;
			break;
			case DND_SYNC_DAMAGEMELEE:
				Player_Bonuses[pnum].damage_type_bonus[TALENT_MELEE] = val;
			break;
			case DND_SYNC_DAMAGEELEMENTAL:
				Player_Bonuses[pnum].damage_type_bonus[TALENT_ELEMENTAL] = val;
			break;
			case DND_SYNC_DAMAGEOCCULT:
				Player_Bonuses[pnum].damage_type_bonus[TALENT_OCCULT] = val;
			break;
			case DND_SYNC_WEPBONUS_CRIT:
				Player_Weapon_Infos[pnum][extra].wep_bonuses[WEP_BONUS_CRIT].amt = val;
			break;
			case DND_SYNC_WEPBONUS_CRITDMG:
				Player_Weapon_Infos[pnum][extra].wep_bonuses[WEP_BONUS_CRITDMG].amt = val;
			break;
			case DND_SYNC_WEPBONUS_CRITPERCENT:
				Player_Weapon_Infos[pnum][extra].wep_bonuses[WEP_BONUS_CRITPERCENT].amt = val;
			break;
			case DND_SYNC_WEPBONUS_DMG:
				Player_Weapon_Infos[pnum][extra].wep_bonuses[WEP_BONUS_DMG].amt = val;
			break;
		}
	}
	else {
		switch(pos) {
			case DND_SYNC_WEAPONENHANCE:
				SetDataToOrbBonus(pnum, OBI_WEAPON_ENCHANT, extra, val);
			break;
			case DND_SYNC_SPEED:
				SetDataToOrbBonus(pnum, OBI_SPEED, -1, val);
			break;
			case DND_SYNC_DROPCHANCE:
				SetDataToOrbBonus(pnum, OBI_DROPCHANCE, -1, val);
			break;
			case DND_SYNC_HPFLAT_BONUS:
				SetDataToOrbBonus(pnum, OBI_HPFLAT, -1, val);
			break;
			case DND_SYNC_ARMORFLAT_BONUS:
				SetDataToOrbBonus(pnum, OBI_ARMORFLAT, -1, val);
			break;
			case DND_SYNC_HPPERCENT_BONUS:
				SetDataToOrbBonus(pnum, OBI_HPPERCENT, -1, val);
			break;
			case DND_SYNC_ARMORPERCENT_BONUS:
				SetDataToOrbBonus(pnum, OBI_ARMORPERCENT, -1, val);
			break;
			case DND_SYNC_GREEDPERCENT_BONUS:
				SetDataToOrbBonus(pnum, OBI_GREEDPERCENT, -1, val);
			break;
			case DND_SYNC_WISDOMPERCENT_BONUS:
				SetDataToOrbBonus(pnum, OBI_WISDOMPERCENT, -1, val);
			break;
			case DND_SYNC_HOLDING:
				SetDataToOrbBonus(pnum, OBI_HOLDING, -1, val);
			break;
			case DND_SYNC_DAMAGEBULLET:
				SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_BULLET, val);
			break;
			case DND_SYNC_DAMAGEENERGY:
				SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_ENERGY, val);
			break;
			case DND_SYNC_DAMAGEEXPLOSIVE:
				SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_EXPLOSIVE, val);
			break;
			case DND_SYNC_DAMAGEMELEE:
				SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_MELEE, val);
			break;
			case DND_SYNC_DAMAGEELEMENTAL:
				SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_ELEMENTAL, val);
			break;
			case DND_SYNC_DAMAGEOCCULT:
				SetDataToOrbBonus(pnum, OBI_DAMAGETYPE, TALENT_OCCULT, val);
			break;
			case DND_SYNC_WEPBONUS_CRIT:
				SetDataToOrbBonus(pnum, OBI_WEAPON_CRIT, extra, val);
			break;
			case DND_SYNC_WEPBONUS_CRITDMG:
				SetDataToOrbBonus(pnum, OBI_WEAPON_CRITDMG, extra, val);
			break;
			case DND_SYNC_WEPBONUS_CRITPERCENT:
				SetDataToOrbBonus(pnum, OBI_WEAPON_CRITPERCENT, extra, val);
			break;
			case DND_SYNC_WEPBONUS_DMG:
				SetDataToOrbBonus(pnum, OBI_WEAPON_DMG, extra, val);
			break;
		}

	}
}

void SyncClientsideVariable(int var, int extra, bool isOrb) {
	if(!isOrb) {
		if(var == DND_SYNC_WEAPONENHANCE || var >= DND_SYNC_WEPBONUS_CRIT)
			ACS_NamedExecuteAlways("DND Clientside Syncer", 0, var, GetPlayerSyncValue(var, extra, DND_SYNC_NONORB), extra);
		else
			ACS_NamedExecuteAlways("DND Clientside Syncer", 0, var, GetPlayerSyncValue(var, 0, DND_SYNC_NONORB), 0);
	}
	else {
		if(var == DND_SYNC_WEAPONENHANCE || var >= DND_SYNC_WEPBONUS_CRIT)
			ACS_NamedExecuteAlways("DND Clientside Orb Syncer", 0, var, GetPlayerSyncValue(var, extra, DND_SYNC_ORB), extra);
		else
			ACS_NamedExecuteAlways("DND Clientside Orb Syncer", 0, var, GetPlayerSyncValue(var, 0, DND_SYNC_ORB), 0);
	}
}

void SyncAllClientsideVariables() {
	int i, j;
	// sync others
	for(i = 0; i < MAX_SYNC_VARS; ++i) {
		if(i == DND_SYNC_WEAPONENHANCE || i >= DND_SYNC_WEPBONUS_CRIT) {
			for(j = 0; j < MAXWEPS; ++j)
				ACS_NamedExecuteAlways("DND Clientside Syncer", 0, i, GetPlayerSyncValue(i, j, DND_SYNC_NONORB), j);
		}
		else
			ACS_NamedExecuteAlways("DND Clientside Syncer", 0, i, GetPlayerSyncValue(i, 0, DND_SYNC_NONORB), 0);
	}
	// sync orbs
	for(i = 0; i < MAX_SYNC_VARS; ++i) {
		if(i == DND_SYNC_WEAPONENHANCE || i >= DND_SYNC_WEPBONUS_CRIT) {
			for(j = 0; j < MAXWEPS; ++j)
				ACS_NamedExecuteAlways("DND Clientside Orb Syncer", 0, i, GetPlayerSyncValue(i, j, DND_SYNC_ORB), j);
		}
		else
			ACS_NamedExecuteAlways("DND Clientside Orb Syncer", 0, i, GetPlayerSyncValue(i, 0, DND_SYNC_ORB), 0);
	}
}

#endif