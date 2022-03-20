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
	DND_CLASS_LABEL_ACTOR,
	DND_CLASS_LABEL_NAME,
	DND_CLASS_LABEL_LEFTIMG,
	DND_CLASS_LABEL_RIGHTIMG,
	DND_CLASS_LABEL_MUGSHOT,
	DND_CLASS_LABEL_TEXT,
	DND_CLASS_LABEL_PERK1,
	DND_CLASS_LABEL_PERK2,
	DND_CLASS_LABEL_PERK3
};

str GetClassLabel(str class_prefix, int label) {
	switch(label) {
		case DND_CLASS_LABEL_ACTOR:
		return StrParam(s:class_prefix, s:"_ACTOR");
		
		case DND_CLASS_LABEL_NAME:
		return StrParam(s:class_prefix, s:"_NAME");
		
		case DND_CLASS_LABEL_LEFTIMG:
		return StrParam(s:class_prefix, s:"_LEFTIMG");
		
		case DND_CLASS_LABEL_RIGHTIMG:
		return StrParam(s:class_prefix, s:"_RIGHTIMG");
		
		case DND_CLASS_LABEL_MUGSHOT:
		return StrParam(s:class_prefix, s:"_MUGSHOT");
		
		case DND_CLASS_LABEL_TEXT:
		return StrParam(s:class_prefix, s:"_TEXT");
		
		case DND_CLASS_LABEL_PERK1:
		return StrParam(s:class_prefix, s:"_PERK1");
		
		case DND_CLASS_LABEL_PERK2:
		return StrParam(s:class_prefix, s:"_PERK2");
		
		case DND_CLASS_LABEL_PERK3:
		return StrParam(s:class_prefix, s:"_PERK3");
	}
	return "";
}

str GetClassName(int id) {
	return StrParam(s:"CLASS", d:id, s:"_NAME");
}

void HandlePlayerPainSound(int pclass) {
	int curhp = GetActorProperty(0, APROP_HEALTH);
	int maxhp = GetSpawnHealth();
	int hpratio = (curhp * 100) / maxhp;
	switch(pclass) {
		case 2:
			// marine -- has 2 sounds for each quarter like so: Player/Pain25
			if(hpratio <= 25)
				PlaySound(0, "Player/Pain25", CHAN_BODY, 1.0);
			else if(hpratio <= 50)
				PlaySound(0, "Player/Pain50", CHAN_BODY, 1.0);
			else if(hpratio <= 75)
				PlaySound(0, "Player/Pain75", CHAN_BODY, 1.0);
			else
				PlaySound(0, "Player/Pain100", CHAN_BODY, 1.0);
		break;
		case 3:
			// hobo
			if(hpratio <= 50)
				PlaySound(0, "Hobo/HardPain", CHAN_BODY, 1.0);
			else
				PlaySound(0, "Hobo/SoftPain", CHAN_BODY, 1.0);
		break;
		case 4:
			// punisher
			if(hpratio <= 50)
				PlaySound(0, "Punisher/Pain", CHAN_BODY, 1.0);
			else
				PlaySound(0, "Punisher/PainLow", CHAN_BODY, 1.0);
		break;
		case 6:
			// cyborg
			if(hpratio <= 50)
				PlaySound(0, "CyborgPlayer/PainLow", CHAN_BODY, 1.0);
			else
				PlaySound(0, "CyborgPlayer/PainHigh", CHAN_BODY, 1.0);
		break;
	}
}

#endif
