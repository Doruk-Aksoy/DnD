#ifndef DND_CLASSMENU
#define DND_CLASSMENU

#define DND_CLASSMENU_BUTTON_DELAY 4

enum {
	DND_CLASSMENU_LEFTBUTTON_BIT = 0b0001,
	DND_CLASSMENU_RIGHTBUTTON_BIT = 0b0010
};

enum {
	DND_CLASSMENU_SELECTID = 1,
	DND_CLASSMENU_CLASSID,
	DND_CLASSMENU_CLASSEXPID,
	DND_CLASSMENU_LEFTIMG,
	DND_CLASSMENU_RIGHTIMG,
	DND_CLASSMENU_MUGIMG,
	DND_CLASSMENU_PREVCLASS_IMGID,
	DND_CLASSMENU_NEXTCLASS_IMGID,
	DND_CLASSMENU_BOTTOMTEXTID,
	DND_CLASSMENU_CLASSPERK5ID,
	DND_CLASSMENU_CLASSPERK25ID,
	DND_CLASSMENU_CLASSPERK50ID,
	DND_CLASSMENU_CLASSCURSOR_LEFTID,
	DND_CLASSMENU_CLASSCURSOR_RIGHTID,
	DND_CLASSMENU_BACKGROUND = 300
};

#define DND_CLASSMENU_IDCOUNT (DND_CLASSMENU_CLASSCURSOR_RIGHTID + 1)
#define DND_MAX_CLASSPERKS (DND_CLASSMENU_CLASSPERK50ID - DND_CLASSMENU_CLASSPERK5ID + 1)

enum {
	DND_CLASSTEXT_LABEL,
	DND_CLASSTEXT_TITLE,
	DND_CLASSTEXT_EXP,
	DND_CLASSTEXT_FRAMELEFT,
	DND_CLASSTEXT_FRAMERIGHT,
	DND_CLASSTEXT_MUG,
	DND_CLASSTEXT_PERK_L5,
	DND_CLASSTEXT_PERK_L25,
	DND_CLASSTEXT_PERK_L50
};
#define DND_MAXPLAYERCLASS_INFO (DND_CLASSTEXT_PERK_L50 + 1)

#define DND_PUNISHER_DMGINC 0.04
#define DND_PUNISHER_KILLSPER 50

str DnD_ClassInfo[MAXPLAYERCLASSES][DND_MAXPLAYERCLASS_INFO] = {
	// Doomguy
	{
		"TheDoomguy",
		"The Doomguy",
		"The classic experience. Mean, tough and ready to kick ass! Milennias of experience fighting demons has certainly taken its toll on him however...",
		"P1_LIMG",
		"P1_RIMG",
		"STFEVL0",
		
		
		"+ L5: Enemy fire attacks deal 10% less damage.",
		"+ L25: Demons take 20% more damage if Doomguy is present in the map.",
		"+ L50: Demons have 10% chance to drop healing orbs. Heal for 2% of enemy max health."
	},
	
	// Marine
	{
		"TheMarine",
		"The Marine",
		"Marine has been through a lot, military complexes, other dimensions... All kinds of evil. And the kinds of evil he's seen, has made him a bit... different.",
		"P2_LIMG",
		"P2_RIMG",
		"MAREVL0",
		
		"+ L5: Your explosives do 25% less self damage.",
		"+ L25: Enemy explosives deal 33% less damage. Explosives ignore radius damage immunity.",
		"+ L50: Poison and Curse immunity. Projectiles can't be reflected."
	},
	
	// Hobo
	{
		"TheHobo",
		"The Hobo",
		"Can't tie him, a loose cannon. He'll do whatever he god damn pleases because that's just how he is. Give him a shotgun and leave the rest to him.",
		"P3_LIMG",
		"P3_RIMG",
		"RENEVL0",
		
		"+ L5: Shotgun weapons have 20% discount.",
		"+ L25: Shotguns do 30% more damage.",
		"+ L50: Shotguns do irreducible damage, and have 50% more pellets."
	},
	
	// Punisher
	{
		"ThePunisher",
		"The Punisher",
		"Was a chill guy, until the fateful day his wife died and his dog was murdered right before his very eyes... Ever since, he hasn't been the same, it's as if something ticked in him and he became the greatest murder machine...",
		"P4_LIMG",
		"P4_RIMG",
		"BADEVL0",
		
		"+ L5: Being on at least the \ckCruel\c- killing spree gives 10% movement speed.",
		"+ L25: At the end of at least the \ckMerciless\c- killing spree, heal and gain armor for 15% of your max caps.",
		"+ L50: Every 50 kills you get increases your damage by 4% for that map. Caps at 400%."
	},

	// Wanderer
	{
		"TheWanderer",
		"The Wanderer",
		"The lone wolf, excels in being the only hero to clean up the menace. When he sets his gaze upon something, that thing hits the ground, lifeless.",
		"P5_LIMG",
		"P5_RIMG",
		"CRVEVL0",
		
		"+ L5: Poison damage taken reduced by 75%.",
		"+ L25: Elemental and Occult attacks and spells do 25% more damage.",
		"+ L50: Enemies killed by you explode for 10% of their maximum health in a 100 unit radius with a random elemental damage. You can't be hurt by this damage."
	},
	
	// Cyborg
	{
		"TheCyborg",
		"The Cyborg",
		"Finest specimen from the Russian laboratories, the Cyborg is employed when all negotiations have failed. The last thing many see is the cold metallic enforcer's visage, before they perish.",
		"P6_LIMG",
		"P6_RIMG",
		"CYGEVL0",
		
		"+ L5: Energy attacks deal 15% less damage to you.",
		"+ L25: Cybernetic enhancements are 30% more effective.",
		"+ L50: Energy weapons do irreducible damage."
	},
	{
		"TheBerserker",
		"The Berserker",
		"Hailing from the mountains, his courage, bravery... recklessness, all impeccable tools against destroying demons. After having lost his fellow townsfolk to a demon invasion, he volunteered to fight for UAC. It's time for demons to meet Ragnarok.",
		"P7_LIMG",
		"P7_RIMG",
		"BEREVL0",
		
		"+ L5: Can carry unlimited melee weapons. Can't buy any super weapon.",
		"+ L25: Gain 2% damage reduction when hurt up to 30% for 3 seconds. Cooldown of 0.5 seconds. Heal for 15% of max health at the end at max stacks.",
		"+ L50: Melee hits or kills give 8% damage. Max 30 stacks. At max stacks, +30% movement speed and +50% crit damage. Lose a stack every 2 seconds off combat."
	}
};

#endif
