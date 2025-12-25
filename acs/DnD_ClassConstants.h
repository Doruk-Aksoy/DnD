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

#define DND_DOOMGUY_DMGREDUCE_PERCENT 50
#define DND_DOOMGUY_DMGREDUCE_PERK3BONUS 40
#define DND_DOOMGUY_PERK5HEAL 15 // 15%
#define DND_DOOMGUY_DROPCHANCE 0.05
#define DND_DOOMGUY_HEALDIV 100
#define DND_DOOMGUY_MAXHEAL 10000
#define DND_DOOMGUY_DMGBONUS 25 // 25%
#define DND_DOOMGUY_CULLBONUS 5 // 5%
#define DND_DOOMGUY_RESISTPCT 33

#define DND_MARINE_EXPLOSIVEREDUCTION -0.33
#define DND_MARINE_50REDUCTION -0.5
#define DND_MARINE_DMGREDUCE_TIMER (TICRATE * 15)
#define DND_MARINE_PERK50_COOLDOWN (TICRATE * 180)
#define DND_MARINE_PERK50_HPPER 250
#define MARINE_PROC_WINDOW (TICRATE)
#define DND_MARINE_EXP_REPEAT_CHANCE 25 // 25%

#define DND_HOBO_RESISTPCT 5
#define DND_HOBO_RESISTPCT_PERLVL 5
#define DND_HOBO_SHOTGUNTAGLIMIT 3
#define DND_HOBO_FRENZYBASETIME (TICRATE * 10)
#define DND_HOBO_ACCURACYBONUS 33
#define DND_HOBO_ACCURACYBONUS_MAXED 17 // added on top for 50%
#define DND_HOBO_SHOTGUNFALLOFFDIST 400 // this is the least amount of distance his level 50 perk will gain bonuses from
#define DND_HOBO_SHOTGUNMINBESTDIST (2 * DND_PLAYER_RADIUS)
#define DND_HOBO_SHOTGUNMINBESTDIST_INT ((DND_HOBO_SHOTGUNMINBESTDIST) >> 16)
#define DND_HOBO_SHOTGUNDISTMOREDMG 50 // 50% more dmg
#define DND_HOBO_PERK50_RARITYBONUS 5
#define DND_HOBO_PERK50_CHANCE 25 // 25%
#define DND_HOBO_PERK50_CHANCE_BONUSMAXED 8 // 8%
#define DND_AMPHETAMINE_MITIGATIONCHANCE 20.0 // 20%
#define DND_AMPHETAMINE_MITIGATIONEFFECT 10.0

#define DND_PUNISHER_RECOVERY 10 // 10%
#define DND_PUNISHER_LIFESTEALRATE 2.0
#define DND_PUNISHER_MAXLIFESTEALTIMES 10
#define DND_MAX_PUNISHER_PERK50_TIERS 11 // 10 tiers on level 50 perk + the base
#define DND_PUNISHER_PERK50_BONUS_PER_TIER 0.25 // the factor is always 1.25 between tiers, more
#define DND_MAX_PUNISHER_PERK50_CAP 4000.0
#define DND_PUNISHER_PERK50_DAMAGEFACTOR 0.06015 // 10%
#define DND_PUNISHER_PERK50_KSPREEBONUS_MULT 4
#define DND_PUNISHER_PERK50_KSPREEBONUS_DIV 3
#define DND_PUNISHER_SPREEBONUS 25
#define DND_PUNISHER_OVERLEECHVAL 50
#define DND_PUNISHER_OVERLEECH_REDUCEFACTOR 33
#define DND_PUNISHER_OVERKILL_LEECHFACTOR 20 // 20%

#define DND_WANDERER_PERK5_DEBUFFREDUCE 33 // 33% less
#define DND_WANDERER_PERK25_BUFF 25 // 25%
#define DND_WANDERER_EXP_DAMAGE 15 // 15%
#define DND_WANDERER_RESREDUCE 10
#define DND_WANDERER_ELEAVOIDCHANCE 20
#define DND_WANDERER_RAY_BASEDIST 1024
#define DND_WANDERER_RAY_DIST_PER 16
#define DND_WANDERER_DASH_DELAY (TICRATE * 3 / 2)
#define DND_WANDERER_DASHVEL 25
#define DND_WANDERER_DASH_TIME 24
#define DND_WANDERER_ASCENSION_TIME (TICRATE * 30)

#define DND_CYBORG_CYBERF 0.33
#define DND_CYBORG_CYBER_MULT 3
#define DND_CYBORG_CYBER_DIV 10
#define DND_MAXCYBORG_INSTABILITY 10
#define DND_DMG_PER_INSTABILITY 2
#define DND_CYBORG_INSTABILITY_CHANCE 0.5 // 50% percent
#define DND_CYBORG_INSTABILITY_TIMER 165
#define DND_CYBORG_INSTABILITY_BONUS 165
#define DND_CYBORG_REGENCONTCHANCE 0.25 // 25%

#define DND_BERSERKER_PERK60_MAXSTACKS 30
#define DND_BERSERKER_PERK60_CRITBONUS 50

#define DND_BERSERKER_MELEEWEPRESIST 20
#define DND_BERSERKER_DAMAGETRACKTIME 122 // 3 is base, x 5 -- +2 for 0.5 second of buffer inclusion
#define DND_BERSERKER_PERK20_MAXSTACKS 15
#define DND_BERSERKER_PERK20_HEALPERCENT 25
#define DND_BERSERKER_PERK20_REDUCTION 2 // 2% per stack
#define DND_BERSERKER_PERK40_SPLASHPCT 33
#define DND_BERSERKER_PERK40_SPLASHRADIUS 128.0
#define DND_BERSERKER_PERK40_SPLASHANGLE (ANG_TO_DOOM(120.0))
#define DND_BERSERKER_PERK60_TIMER 70 // 14 x 5 = 70 => 2 seconds
#define DND_BERSERKER_PERK60_DMGINCREASE 10 // 8%
#define DND_BERSERKER_PERK60_MAXHITS 5
#define DND_BERSERKER_PERK80_EXTENSION 30 // 30 seconds

#define DND_TRICKSTER_SIZEX 0.6
#define DND_TRICKSTER_SIZEY 0.6
#define DND_TRICKSTER_CRIT_GAIN_FROM_MIT 0.001 // 0.1 per 1%
#define DND_TRICKSTER_PHASING_CHANCE 0.2 // 20%
#define DND_TRICKSTER_PERK40_THRESHOLD 40 // 40%
#define DND_TRICKSTER_PERK40_COOLDOWN 450 // 90 x 5
#define DND_TRICKSTER_PERK40_DELAY 7
#define DND_TRICKSTER_RANDOMCLONES_COUNT 3
#define DND_TRICKSTER_ACROBAT_PHASINGBONUS 25 // 25%

enum {
	DND_CLASS_LABEL_NAME,
	DND_CLASS_LABEL_LEFTIMG,
	DND_CLASS_LABEL_RIGHTIMG,
	DND_CLASS_LABEL_MUGSHOT,
	DND_CLASS_LABEL_TEXT,

	DND_CLASS_LABEL_PERK1,
	DND_CLASS_LABEL_PERK2,
	DND_CLASS_LABEL_PERK3,
	DND_CLASS_LABEL_PERK4,
	DND_CLASS_LABEL_PERK5,

	DND_CLASS_LABEL_PERK1_DESC,
	DND_CLASS_LABEL_PERK2_DESC,
	DND_CLASS_LABEL_PERK3_DESC,
	DND_CLASS_LABEL_PERK4_DESC,
	DND_CLASS_LABEL_PERK5_DESC
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

		case DND_CLASS_LABEL_PERK4:
		return StrParam(s:class_prefix, s:"_PERK4");

		case DND_CLASS_LABEL_PERK5:
		return StrParam(s:class_prefix, s:"_PERK5");

		case DND_CLASS_LABEL_PERK1_DESC:
		return StrParam(s:class_prefix, s:"_PERK1_DESC");
		
		case DND_CLASS_LABEL_PERK2_DESC:
		return StrParam(s:class_prefix, s:"_PERK2_DESC");
		
		case DND_CLASS_LABEL_PERK3_DESC:
		return StrParam(s:class_prefix, s:"_PERK3_DESC");

		case DND_CLASS_LABEL_PERK4_DESC:
		return StrParam(s:class_prefix, s:"_PERK4_DESC");

		case DND_CLASS_LABEL_PERK5_DESC:
		return StrParam(s:class_prefix, s:"_PERK5_DESC");

	}
	return "";
}

void DoPreClassAdjustments() {
	if(isPlayerClass(DND_PLAYER_TRICKSTER)) {
		SetActorProperty(0, APROP_SCALEX, DND_TRICKSTER_SIZEX);
		SetActorProperty(0, APROP_SCALEY, DND_TRICKSTER_SIZEY);
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