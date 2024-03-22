#ifndef DND_ACCESSORIES_IN
#define DND_ACCESSORIES_IN

#define DND_GRYPHON_MSPEED 25 // 25%
#define DND_CELESTIAL_MSPEED -20 // -20%

enum {
	DND_ACCESSORY_TALISMANWISDOM = 1,
	DND_ACCESSORY_TALISMANGREED = 2,
	DND_ACCESSORY_DEMONBANE = 4,
	DND_ACCESSORY_AMULETHELLFIRE = 8,
	DND_ACCESSORY_ANGELICANKH = 16,
	DND_ACCESSORY_PENDANTREFLECTION = 32,
	DND_ACCESSORY_NETHERMASK = 64,
	DND_ACCESSORY_TALTOSRING = 128,
	DND_ACCESSORY_HATESHARD = 256,
	DND_ACCESSORY_HANDARTEMIS = 512,
	DND_ACCESSORY_AGAMOTTO = 1024,
	DND_ACCESSORY_GRYPHONBOOTS = 2048,
	DND_ACCESSORY_LICHARM = 4096,
	DND_ACCESSORY_SIGILELEMENTS = 8192,
	DND_ACCESSORY_CELESTIAL = 16384,
	DND_ACCESSORY_VEIL = 32768,
	DND_ACCESSORY_HUNTERTALISMAN = 65536,
};
#define DND_REPLACE_THIS_ACCESSORY 5 // angelic ankh
#define MAX_ACCESSORY 17
#define DND_ACCESSORY_BASELIMIT 2

#define DND_LICH_SOULFACTOR 2
#define CELESTIAL_BOOST 25 // percent
#define DND_VEIL_FACTOR 20 // percent
#define DND_VEIL_FACTORUP 3
#define DND_HUNTERTALISMAN_HEALFACTOR 33
#define DND_TALTOS_LIFESTEAL 25.0 // 25%

#define DND_AMULETHELL_FACTOR 4 // 25% damage
#define DND_AMULETHELL_AMP 7 // 7 / 4 = 75%
#define DND_AMULETHELL_DAMAGE 50 // 50%

#define DND_DEMONBANE_GAIN 50
#define DND_DEMONBANE_REDUCE 75 // 1 / 4 = 25% damage, 75% reduction

#define DND_NETHERMASK_AMP 3 // 3 / 2 => 50%
#define DND_NETHERMASK_DIV 2
#define DND_NETHERGHOST_BONUS 25 // 25%

#define DND_HATESHARD_BUFF 50 // 50% more
#define DND_HATESHARD_REDUCTION 80 // 80%

#define DND_ARTEMIS_REDUCE 66
#define DND_ARTEMIS_REDUCE_SUPER 90 // 90%

#define DND_AGAMOTTO_DEFENSE 14 // 14/10 is 1.4 => 40% more damage
#define DND_AGAMOTTO_DEFENSE_FACTOR 10
#define DND_AGAMOTTO_OFFENSE 2 // 100% more damage => x2
#define DND_AGAMOTTO_OFFENSE_FACTOR 1

#define DND_LICHARM_NERF 90 // 90% reduced damage
#define DND_LICHARM_BUFF 25 // 25% more damage

#define DND_SIGIL_BUFF 100 // x2
#define DND_SIGIL_NERF 99999 // x0 => complete negation of damage

#define DND_HUNTERTALISMAN_NERF 3 // 1 / 3 -- this is subtracted

#define ACCESSORY_NAME 0
#define ACCESSORY_ICON 1
str AccessoryInfo[MAX_ACCESSORY][3] = { 
	{ "Accessory_1", 		"ACC1B0"	},
	{ "Accessory_2", 		"ACC2B0"	},
	{ "Accessory_3", 		"ACC3B0"	},
	{ "Accessory_4", 		"ACC4B0"	},
	{ "Accessory_5", 		"ACC5B0"	},
	{ "Accessory_6", 		"ACC6B0"	},
	{ "Accessory_7", 		"ACC7B0"	},
	{ "Accessory_8", 		"ACC8B0"	},
	{ "Accessory_9", 		"ACC9B0"	},
	{ "Accessory_10", 	"AC10B0"	},
	{ "Accessory_11", 	"AC11B0"	},
	{ "Accessory_12", 	"AC12B0"	},
	{ "Accessory_13", 	"AC13B0"	},
	{ "Accessory_14", 	"AC14B0"	},
	{ "Accessory_15", 	"AC15B0"	},
	{ "Accessory_16", 	"AC16B0"	},
	{ "Accessory_17", 	"AC17B0"	}
};

#define ACCESSORY_TAG 0
#define ACCESSORY_PRO 1
#define ACCESSORY_CON 2
str GetAccessoryText(int id, int which) {
	if(which == ACCESSORY_TAG)
		return StrParam(s:"ACCTX_T", d:id);
	else if(which == ACCESSORY_PRO)
		return StrParam(s:"ACCTX_P", d:id);
	return StrParam(s:"ACCTX_C", d:id);
}

int IsAccessoryEquipped(int tid, int acc) {
	if(tid)
		return CheckActorInventory(tid, "Accessory_Index") & acc;
	return CheckInventory("Accessory_Index") & acc;
}

#endif
