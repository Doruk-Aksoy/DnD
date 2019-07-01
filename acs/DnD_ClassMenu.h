#ifndef DND_CLASSMENU
#define DND_CLASSMENU

enum {
	DND_CLASSMENU_SELECTID,
	DND_CLASSMENU_CLASSID,
	DND_CLASSMENU_CLASSLABELID,
	DND_CLASSMENU_LEFTIMG,
	DND_CLASSMENU_MIDIMG,
	DND_CLASSMENU_RIGHTIMG,
	DND_CLASSMENU_CLASSTEXTID,
	DND_CLASSMENU_CLASSPERKID,
	DND_CLASSMENU_CLASSCURSOR_LEFTID,
	DND_CLASSMENU_CLASSCURSOR_RIGHTID,
	DND_CLASSMENU_BACKGROUND = 300
};

#define DND_CLASSMENU_IDCOUNT (DND_CLASSMENU_CLASSCURSOR_RIGHTID + 1)

enum {
	DND_CLASSTEXT_LABEL,
	DND_CLASSTEXT_TITLE,
	DND_CLASSTEXT_EXP,
	DND_CLASSTEXT_FRAMELEFT,
	DND_CLASSTEXT_FRAMEMID,
	DND_CLASSTEXT_FRAMERIGHT,
	DND_CLASSTEXT_PERKS
};
#define DND_MAXPLAYERCLASS_INFO (DND_CLASSTEXT_PERKS + 1)
str DnD_ClassInfo[MAXPLAYERCLASSES][DND_MAXPLAYERCLASS_INFO] = {
	// Doomguy
	{
		"TheDoomguy",
		"The Doomguy",
		"The classic experience. Mean, tough and ready to kick ass! Milennias of experience fighting demons has certainly taken its toll on him however...",
		"DPLYE2E8",
		"DPLYE8E2",
		"DPLYA1",
		
		
		"+ L5: Fire attacks deal 10% less damage.\n
		 + L25: Demons take 15% more damage.\n
		 + L50: Demons have 15% chance to drop healing orbs. Heal for 5% of enemy max health."
	},
	
	// Marine
	{
		"TheMarine",
		"The Marine",
		"Marine has been through a lot, military complexes, other dimensions... All kinds of evil. And the kinds of evil he's seen, has made him a bit... different.",
		"PLAYE2E8",
		"PLAYE8E2",
		"PLAYA1",
		
		"+ L5: Your explosives do 20% less self damage.\n
		 + L25: Enemy explosives deal 20% less damage.\n
		 + L50: Curse immunity."
	},
	
	// Hobo
	{
		"TheHobo",
		"The Hobo",
		"Can't tie him, a loose cannon. He'll do whatever he god damn pleases because that's just how he is. Give him a shotgun and leave the rest to him.",
		"PRENE2E8",
		"PRENE8E2",
		"PRENA1",
		
		"+ L5: Shotgun weapons have 20% discount.\n
		 + L25: Shotguns do 30% more damage.\n
		 + L50: Shotguns do full damage, and have 50% more pellets."
	},
	
	// Punisher
	{
		"ThePunisher",
		"The Punisher",
		"Was a chill guy, until the fateful day his wife died and his dog was murdered right before his very eyes... Ever since, he hasn't been the same, it's as if something ticked in him and he became the greatest murder machine...",
		"BLAZE2E8",
		"BLAZE8E2",
		"BLAZA1",
		
		"+ L5: Being on a killing spree gives 10% movement speed.\n,
		 + L25: At the end of a killing spree, heal and gain armor for 15% of your max caps.\n
		 + L50: Every 50 kills you get boosts your damage and resistances by 2.5% for that map. Resist caps at 75%."
	},

	// Wanderer
	{
		"TheWanderer",
		"The Wanderer",
		"The lone wolf, excels in being the only hero to clean up the menace. When he sets his gaze upon something, that thing hits the ground, lifeless.",
		"MNK1E2E8",
		"MNK1E8E2",
		"MNK1A1",
		
		"+ L5: When no other player is around you, move 7.5% faster.\n
		 + L25: When no other player is around you, deal 12.5% more damage.\n
		 + L50: When no other player is around you, take 17.5% less damage."
	},
	
	// Cyborg
	{
		"TheCyborg",
		"The Cyborg",
		"Finest specimen from the Russian laboratories, the Cyborg is employed when all negotiations have failed. The last thing many see is the cold metallic enforcer's visage, before they perish.",
		"PLY6E2E8",
		"PLY6E8E2",
		"PLY6A1",
		
		"+ L5: Energy attacks deal 15% less damage to you.\n
		 + L25: Cybernetic enhancements are 30% more effective.\n
		 + L50: Energy weapons do full damage."
	},
};

#endif