#ifndef DND_ACCESSORIES_IN
#define DND_ACCESSORIES_IN

#define DND_GRYPHON_MSPEED 25 // 25%

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
