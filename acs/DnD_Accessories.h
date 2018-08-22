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

#define CELESTIAL_BOOST 25 // percent
#define DND_VEIL_FACTOR 20 // percent
#define DND_VEIL_FACTORUP 3
#define DND_HUNTERTALISMAN_HEALFACTOR 33

int IsAccessoryEquipped(int tid, int acc) {
	if(tid)
		return CheckActorInventory(tid, "Accessory_Index") & acc;
	return CheckInventory("Accessory_Index") & acc;
}

#endif