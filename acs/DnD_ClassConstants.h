#ifndef DND_CLASSCONST_IN
#define DND_CLASSCONST_IN

enum {
	DND_CLASSPERK_1 = 1,
	DND_CLASSPERK_2,
	DND_CLASSPERK_3,
	DND_CLASSPERK_4,
	DND_CLASSPERK_LAST
};

#define DND_PERK1_THRESHOLD 1
#define DND_PERK_REGULARTHRESHOLD 20 // every 20 levels you get a perk

#define DND_DOOMGUY_DMGREDUCE_PERCENT 90
#define DND_DOOMGUY_PERK5HEAL 15 // 15%
#define DND_DOOMGUY_DROPCHANCE 0.05
#define DND_DOOMGUY_HEALDIV 100
#define DND_DOOMGUY_MAXHEAL 10000
#define DND_DOOMGUY_DMGBONUS 25 // 25%
#define DND_DOOMGUY_RESISTPCT 33

#define DND_MARINE_EXPLOSIVEREDUCTION -0.33
#define DND_MARINE_50REDUCTION -0.5
#define DND_MARINE_DMGREDUCE_TIMER (TICRATE * 15)
#define DND_MARINE_PERK50_COOLDOWN (TICRATE * 180)
#define DND_MARINE_PERK50_HPPER 250
#define MARINE_PROC_WINDOW (TICRATE)

#define DND_HOBO_RESISTPCT 25
#define DND_HOBO_SHOTGUNTAGLIMIT 3
#define DND_HOBO_FRENZYBASETIME (TICRATE * 12)
#define DND_HOBO_ACCURACYBONUS 50
#define DND_HOBO_SHOTGUNFALLOFFDIST 400 // this is the least amount of distance his level 50 perk will gain bonuses from
#define DND_HOBO_SHOTGUNMINBESTDIST (2 * DND_PLAYER_RADIUS)
#define DND_HOBO_SHOTGUNMINBESTDIST_INT ((DND_HOBO_SHOTGUNMINBESTDIST) >> 16)
#define DND_HOBO_SHOTGUNDISTMOREDMG 100 // 100% more dmg
#define DND_HOBO_PERK50_RARITYBONUS 5
#define DND_HOBO_PERK50_CHANCE 30 // 30%

#define DND_PUNISHER_RECOVERY 10 // 10%
#define DND_PUNISHER_LIFESTEALRATE 2.0
#define DND_PUNISHER_MAXLIFESTEALTIMES 10
#define DND_MAX_PUNISHER_PERK50_TIERS 11 // 10 tiers on level 50 perk + the base
#define DND_PUNISHER_PERK50_BONUS_PER_TIER 0.25 // the factor is always 1.25 between tiers, more
#define DND_MAX_PUNISHER_PERK50_CAP 4000.0
#define DND_PUNISHER_PERK50_DAMAGEFACTOR 0.06015 // 10%
#define DND_PUNISHER_PERK50_KSPREEBONUS_MULT 4
#define DND_PUNISHER_PERK50_KSPREEBONUS_DIV 3

#define DND_WANDERER_PERK5_DEBUFFREDUCE 33 // 33% less
#define DND_WANDERER_PERK25_BUFF 25 // 25%
#define DND_WANDERER_EXP_DAMAGE 10 // 1 / 10 = 10%
#define DND_WANDERER_RESREDUCE 10

#define DND_CYBORG_CYBERF 0.33
#define DND_CYBORG_CYBER_MULT 3
#define DND_CYBORG_CYBER_DIV 10
#define DND_MAXCYBORG_INSTABILITY 10
#define DND_DMG_PER_INSTABILITY 2
#define DND_CYBORG_INSTABILITY_CHANCE 0.5 // 50% percent
#define DND_CYBORG_INSTABILITY_TIMER 165

#define DND_BERSERKER_PERK50_MAXSTACKS 30
#define DND_BERSERKER_PERK50_CRITBONUS 50

#define DND_BERSERKER_DAMAGETRACKTIME 122 // 3 is base, x 5 -- +2 for 0.5 second of buffer inclusion
#define DND_BERSERKER_PERK25_MAXSTACKS 15
#define DND_BERSERKER_PERK25_HEALPERCENT 15
#define DND_BERSERKER_PERK25_REDUCTION 2 // 2% per stack
#define DND_BERSERKER_PERK50_TIMER 70 // 14 x 5 = 70 => 2 seconds
#define DND_BERSERKER_PERK50_DMGINCREASE 8 // 8%
#define DND_BERSERKER_PERK50_MAXHITS 5

#define DND_TRICKSTER_SIZEX 0.6
#define DND_TRICKSTER_SIZEY 0.6
#define DND_TRICKSTER_CRIT_GAIN_FROM_MIT 0.002 // 0.2 per 1%
#define DND_TRICKSTER_PHASING_CHANCE 0.2 // 20%
#define DND_TRICKSTER_PERK50_THRESHOLD 40 // 40%
#define DND_TRICKSTER_PERK50_COOLDOWN 450 // 90 x 5
#define DND_TRICKSTER_PERK50_DELAY 7
#define DND_TRICKSTER_RANDOMCLONES_COUNT 3

enum {
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

void DoPreClassAdjustments() {
	if(isPlayerClass(DND_PLAYER_TRICKSTER)) {
		SetActorProperty(0, APROP_SCALEX, DND_TRICKSTER_SIZEX);
		SetActorProperty(0, APROP_SCALEY, DND_TRICKSTER_SIZEY);
	}
}

void CheckDoomguyExecuteReversal(int this) {
	if(CheckActorInventory(this, "Doomguy_ChangedColor")) {
		TakeActorInventory(this, "Doomguy_ChangedColor", 1);
		TakeActorInventory(this, "Doomguy_ValidExecute", 1);
		ACS_NamedExecuteWithResult("DnD Doomguy Execute Translation", this, 1, MonsterProperties[this - DND_MONSTERTID_BEGIN].id);
	}
}

str GetClassPerk(int class, int perk_num) {
	if(class < 0)
		return "";
	str cprefix = StrParam(s:"CLASS", d:class);
	if(perk_num > DND_CLASSPERK_1)
		perk_num = (perk_num - 1) * DND_PERK_REGULARTHRESHOLD;
	return StrParam(l:GetClassLabel(cprefix, DND_CLASS_LABEL_NAME), s:"_Perk", d:perk_num);
}

bool HasClassPerk(int class, int perk_num) {
	return CheckInventory(GetClassPerk(class, perk_num));
}

bool HasActorClassPerk(int tid, int class, int perk_num) {
	return CheckActorInventory(tid, GetClassPerk(class, perk_num));
}

bool HasClassPerk_Fast(str class, int perk_num) {
	if(perk_num > DND_CLASSPERK_1)
		perk_num = (perk_num - 1) * DND_PERK_REGULARTHRESHOLD;
	return CheckInventory(StrParam(s:class, s:"_Perk", d:perk_num));
}

bool HasActorClassPerk_Fast(int tid, str class, int perk_num) {
	if(perk_num > DND_CLASSPERK_1)
		perk_num = (perk_num - 1) * DND_PERK_REGULARTHRESHOLD;
	return CheckActorInventory(tid, StrParam(s:class, s:"_Perk", d:perk_num));
}

#endif