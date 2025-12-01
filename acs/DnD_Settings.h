#ifndef DND_SETTINGS_IN
#define DND_SETTINGS_IN

// Hardcore Info
#define DND_HARDCORE_INVALID 0
#define DND_HARDCORE_VALID 1
#define MAXFLAGS 31
str FlagCheckList[MAXFLAGS] = {
	"sv_maxlives",
	"sv_maxclientsperip",
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
	"Survival",
	"sv_smartaim"
};

#define DND_HARDCORE_MAXLIVES 1
#define DND_SOFTCORE_MAXLIVES 2
int FlagAcceptedValues[MAXFLAGS] = {
	DND_HARDCORE_MAXLIVES,
	
	#ifdef ISDEBUGBUILD
	2,
	#elif defined SKIP_DB_SETTINGS
	2,
	#else
	1,
	#endif

	1.0,
	
	// sv_cheats
	#ifdef ISDEBUGBUILD
		1,
	#else
		0,
	#endif
	
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
	1,
	2
};

#define MAXDNDFLAGS 50
str DNDFlagCheckList[MAXDNDFLAGS] = {
	"dnd_monsterbars",
	"dnd_monsterscale",
	"dnd_sharexp",
	"dnd_sharecredit",
	"dnd_healonlevelup",
	"dnd_killspree",
	
	"dnd_budget_reward",
	
	"dnd_firstbackpackold",
	"dnd_playercount_scales_monsters",
	"dnd_fullshare",
	"dnd_monsterlevel_behind",
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
	"dnd_shop_account_scale",
	
	"dnd_monsterlevel_low",
	"dnd_monsterlevel_high",
	"dnd_respawn_count",
	"dnd_accessory_droprate",
	"dnd_gainrange",
	
	"dnd_quest_chance",
	"dnd_quest_avglevel",
	"dnd_elite_spawnchance",
	"dnd_elite_spawnlevel",
	"dnd_legendary_chance",
	"dnd_limitedrespawn_amount",
	
	"dnd_incursion_level",
	"dnd_npc_appear_level",
	
	"dnd_exp_scale",
	"dnd_credit_scale",
	"dnd_budget_scale",
	"dnd_accessorylevel",
	"dnd_accessorymonsterlevel",
	"dnd_monster_rewardscalepercent",
	"dnd_uniquemonster_spawnchance",
	"dnd_researchdroprate",
	
	"dnd_ignore_dropweights",
	"dnd_ignorelootpenalty"
};

int DNDFlagAcceptedValues[MAXDNDFLAGS] = {
	1,
	1,
	1,
	1,
	0,
	1,
	
	3,
	
	0,
	1,
	1,
	0,
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
	6,
	3,
	3,
	768,
	
	20,
	25,
	10,
	20,
	1,
	2,
	
	25,
	25,
	
	1,
	1,
	1,
	20,
	24,
	5,
	0.01,
	0.005,
	
	0,
	0
};

// Checks pre-defined settings for hardcore mode that has character saving
// returns 1 if succeeded, 0 if failed
int CheckHardcoreSettings() {
	// check all flags compare with default values
	int i;
	int errno = DND_HARDCORE_VALID;

#ifdef SKIP_DB_SETTINGS
	return errno;
#endif
	
	// only check this in debug builds...
#ifdef ISDEBUGBUILD
	if (!GetCVar("dnd_test")) { //dnd_test true/1 skips the tests
#else
	if(1) {
#endif
		for(i = 0; i < MAXFLAGS; ++i) {
			if(GetCVar(FlagCheckList[i]) != FlagAcceptedValues[i]) {
				// sv_maxlives is allowed to be 2 for softcore
				if(!i && GetCVar("dnd_mode") == DND_MODE_SOFTCORE && GetCVar(FlagCheckList[i]) == DND_SOFTCORE_MAXLIVES)
					continue;
				Log(s:"\"", s:FlagCheckList[i], s:"\" should be set to ", d:FlagAcceptedValues[i], s:" for \cghardcore\c- or \cdsoftcore\c- modes to work! It is set to: ", d:GetCVar(FlagCheckList[i]));
				errno = DND_HARDCORE_INVALID;
			}
		}
		for(i = 0; i < MAXDNDFLAGS; ++i) {
			if(GetCVar(DNDFlagCheckList[i]) != DNDFlagAcceptedValues[i]) {
				// exception for small values
				if(GetCVar(DNDFlagCheckList[i]) < 1.0 && abs(DNDFlagAcceptedValues[i] - GetCVar(DNDFlagCheckList[i])) < 0.0005)
					continue;
				
				Log(s:"\"", s:DNDFlagCheckList[i], s:"\" should be set to ", d:DNDFlagAcceptedValues[i], s:" for \cghardcore\c- or \cdsoftcore\c- modes to work! It is set to: ", d:GetCVar(DNDFlagCheckList[i]));
				errno = DND_HARDCORE_INVALID;
			}
		}
	}
	return errno;
}

#endif
