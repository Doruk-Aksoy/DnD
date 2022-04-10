#ifndef DND_ARTI_IN
#define DND_ARTI_IN

enum {
	ARTI_FIELDKIT,
	ARTI_SALVATIONSPHERE,
	ARTI_PORTABLESHIELD,
	ARTI_BLOODRUNE,
	ARTI_TRIPLEDMG,
	ARTI_BLADEVORTEX,
	ARTI_BOOK,
	ARTI_ALLMAP,
	ARTI_NEWBACKPACK,
	ARTI_STATRESET
};
#define MAXARTIFACTS (ARTI_STATRESET + 1)

#define ARTI_ICON 0
#define ARTI_NAME 1
str ArtifactInfo[MAXARTIFACTS][2] = { 
	{ "FKITD0",				"FieldKit"				},	
	{ "SALVRICO",			"SalvationSphere"		},
	{ "SHIELICO",			"PortableShield"		},
	{ "BRUNICO",			"BloodRune"				},
	{ "DMGICO",			    "TripleDamage"			},
	{ "SHRUA0",			    "BladeVortex"			},
	{ "BOTDICO",		    "BookOfTheDead"		    },
	{ "PMAPA0",			    "AllMapOnlyOnce"		},
	{ "BPAKA0",			    "NewBackPack"			},
	{ "RESETICO",           "StatReset"             }
};

#endif
