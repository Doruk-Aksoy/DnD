#ifndef DND_SETTINGS_IN
#define DND_SETTINGS_IN

// Hardcore Info
#define DND_HARDCORE_INVALID 0
#define DND_HARDCORE_VALID 1
#define MAXFLAGS 29
str FlagCheckList[MAXFLAGS] = {
	"sv_maxlives",
	"sv_coop_damagefactor",
	"sv_cheats",
	"sv_fastweapons",
	"sv_nochangemapvote",
	"sv_nodrop",
	"sv_infiniteAmmo",
	"sv_NoMonsters",
	"SV_FastMonsters",
	"SV_RespawnSuper",
	"SV_NoWeaponSpawn",
	"SV_Coop_LoseInventory",
	"SV_Coop_LoseWeapons",
	"SV_Coop_LoseArmor",
	"SV_Coop_LosePowerups",
	"SV_Coop_LoseAmmo",
	"SV_Coop_HalveAmmo",
	"SV_WeaponDrop",
	"SV_DoubleAmmo",
	"SV_Degeneration",
	"SV_BFGFreeAim",
	"SV_ShotgunStart",
	"SV_InfiniteInventory",
	"SV_NoAutomap",
	"SV_NoAutomapAllies",
	"sv_itemrespawn",
	"sv_deadplayerscankeepinventory",
	"Compat_ClientsSendFullButtonInfo",
	"Survival"
};

#define DND_HARDCORE_MAXLIVES 1
int FlagAcceptedValues[MAXFLAGS] = {
	DND_HARDCORE_MAXLIVES,
	1.0,
	0,
	0,
	1,
	1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1,
	0,
	0,
	0,
	0,
	1,
	0,
	1,
	1
};

#define MAXDNDFLAGS 52
str DNDFlagCheckList[MAXDNDFLAGS] = {
	"dnd_monsterbars",
	"dnd_monsterscale",
	"dnd_sharexp",
	"dnd_sharecredit",
	"dnd_healonlevelup",
	"dnd_killspree",
	
	"dnd_firstbackpackold",
	"dnd_playercount_scales_monsters",
	"dnd_fullshare",
	"dnd_monsterlevel_behind",
	"dnd_credit_drops",
	"dnd_gainonlyinrange",
	"dnd_allresearchesfound",
	"dnd_disablelevelbonus",
	"dnd_usesmartmonsterspawner",
	"dnd_enable_quests",
	"dnd_enable_elites",
	"dnd_enable_limiteditems",
	"dnd_no_immunity",
	"dnd_no_reflect",
	
	"dnd_shop_scale",
	"dnd_shop_wep_scale",
	"dnd_shop_ammo_scale",
	"dnd_shop_artifact_scale",
	"dnd_shop_ability_scale",
	"dnd_shop_talent_scale",
	"dnd_shop_armor_scale",
	"dnd_shop_account_scale",
	
	"dnd_monsterlevel_low",
	"dnd_monsterlevel_high",
	"dnd_respawn_count",
	"dnd_accessory_droprate",
	"dnd_credit_droprateadd",
	"dnd_gainrange",
	
	"dnd_orb_dropchanceadd",
	"dnd_quest_chance",
	"dnd_quest_avglevel",
	"dnd_elite_spawnchance",
	"dnd_elite_spawnlevel",
	"dnd_elite_runedrop_chance",
	"dnd_legendary_chance",
	"dnd_limitedrespawn_amount",
	
	"dnd_exp_scale",
	"dnd_credit_scale",
	"dnd_budget_scale",
	"dnd_accessorylevel",
	"dnd_accessorymonsterlevel",
	"dnd_monster_hpscalepercent",
	"dnd_monster_rewardscalepercent",
	"dnd_monster_dmgscalepercent",
	"dnd_researchdroprate",
	
	"dnd_ignore_dropweights"
};

int DNDFlagAcceptedValues[MAXDNDFLAGS] = {
	1,
	1,
	1,
	1,
	0,
	1,
	
	0,
	1,
	1,
	0,
	1,
	1,
	0,
	0,
	1,
	1,
	1,
	1,
	0,
	0,
	
	2,
	1,
	1,
	1,
	2,
	1,
	1,
	1,
	
	1,
	6,
	3,
	3,
	0,
	768,
	
	0,
	20,
	25,
	10,
	20,
	15,
	1,
	2,
	
	1,
	1,
	1,
	20,
	24,
	5,
	5,
	0.04,
	0.00249,
	
	0
};

// Checks pre-defined settings for hardcore mode that has character saving
// returns 1 if succeeded, 0 if failed
int CheckHardcoreSettings() {
	// check all flags compare with default values
	int i;
	int errno = DND_HARDCORE_VALID;
	if (!GetCVar("dnd_test")) { //dnd_test true/1 skips the tests
		for(i = 0; i < MAXFLAGS; ++i) {
			if(GetCVar(FlagCheckList[i]) != FlagAcceptedValues[i]) {
				Log(s:"\"", s:FlagCheckList[i], s:"\" should be set to ", d:FlagAcceptedValues[i], s:" for \cghardcore\c- or \cdsoftcore\c- modes to work! It is set to: ", d:GetCVar(FlagCheckList[i]));
				errno = DND_HARDCORE_INVALID;
			}
		}
		for(i = 0; i < MAXDNDFLAGS; ++i) {
			if(GetCVar(DNDFlagCheckList[i]) != DNDFlagAcceptedValues[i]) {
				Log(s:"\"", s:DNDFlagCheckList[i], s:"\" should be set to ", d:DNDFlagAcceptedValues[i], s:" for \cghardcore\c- or \cdsoftcore\c- modes to work! It is set to: ", d:GetCVar(DNDFlagCheckList[i]));
				errno = DND_HARDCORE_INVALID;
			}
		}
	}
	return errno;
}

#endif