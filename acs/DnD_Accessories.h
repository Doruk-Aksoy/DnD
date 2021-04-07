#ifndef DND_ACCESSORIES_IN
#define DND_ACCESSORIES_IN

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

#define ACCESSORY_TAG 0
#define ACCESSORY_NAME 1
#define ACCESSORY_ICON 2
#define ACCESSORY_PRO 3
#define ACCESSORY_CON 4
str AccessoryInfo[MAX_ACCESSORY][5] = { 
	{ "Wisdom Talisman", 		"Accessory_1", 		"ACC1B0", "Increases exp gain by 50%.", "Reduces credit gain by 50%." },
	{ "Greed Talisman", 		"Accessory_2", 		"ACC2B0", "Increases credit gain by 50%.", "Reduces experience gain by 50%." },
	{ "Demon Bane", 			"Accessory_3", 		"ACC3B0", "Increases damage of occult weapons by 100%.", "Reduces other damages by 75%." },
	{ "Amulet of Hellfire", 	"Accessory_4", 		"ACC4B0", "Protects from all fire damage and hazards by 75%. Increases your fire damage by 50%.", "Increases ice damage taken by 75% and reduces your ice damage by 50%." },
	{ "Angelic Ankh", 			"Accessory_5", 		"ACC5B0", "When about to die, the ankh will intervene, making you invulnerable for 8 seconds. Works once very map.", "Your health cap is halved. (Never below 100)" },
	{ "Reflection Pendant", 	"Accessory_6", 		"ACC6B0", "When hit, 20% chance to go invulnerable and reflective for 5 seconds. Can happen only 5 times every map.", "While active, move 25% slower." },
	{ "Nether Mask", 			"Accessory_7", 		"ACC7B0", "All your weapons can hit ghosts and do irreducable damage.", "Take 50% more damage." },
	{ "Taltos Ring", 			"Accessory_8", 		"ACC8B0", "+25% lifesteal for melee attacks and become a ghost.", "Health items are unpickable. Only Megasphere or Berserk can be picked that give health." },
	{ "Hate Shard", 			"Accessory_9", 		"ACC9B0", "Reflect damage back to enemies. Attackers take x2 more damage for 5 seconds.", "Non-melee damage reduced by 50%." },
	{ "Hand of Artemis", 		"Accessory_10", 	"AC10B0", "Your weapons do not require any ammo. Does not affect \cstemporary\c- weapons.", "Do 66% less damage." },
	{ "Scroll of Agamotto", 	"Accessory_11", 	"AC11B0", "Do 150% more damage while standing.", "Take 30% more damage while moving." },
	{ "Gryphon Boots", 			"Accessory_12", 	"AC12B0", "Gives 25% movement speed, immunity to curses and fall damage, and knockback immunity.", "Can't regenerate." },
	{ "Lich Arm", 				"Accessory_13", 	"AC13B0", "Soul capacity becomes 150 and ultimate weapons do x2.5 damage.", "Any other weapon does 90% reduced damage." },
	{ "Sigil of Elements", 		"Accessory_14", 	"AC14B0", "Gives x3 elemental damage to a single element for 20 seconds. This works in a rotation.", "Other elements do 50% less damage." },
	{ "Celestial Relic", 		"Accessory_15", 	"AC15B0", "Gives 25% increased health and armor capacity.", "Slows you down by 20%." },
	{ "Veil of the Assassin", 	"Accessory_16", 	"AC16B0", "After a crit, your next crit's damage is doubled. Expend this crit to heal for 15% of missing health. Lasts for 5 seconds or until you expend the crit.", "On expending your doubled crit, never crit for 10 seconds." },
	{ "Hunter's Talisman", 		"Accessory_17", 	"AC17B0", "Your critical hits mark nearby monsters once every 20 seconds, making them deal 33% less damage and they replenish 3% of your max health on kill.", "Your overall crit damage is halved." }
};

int IsAccessoryEquipped(int tid, int acc) {
	if(tid)
		return CheckActorInventory(tid, "Accessory_Index") & acc;
	return CheckInventory("Accessory_Index") & acc;
}

#endif