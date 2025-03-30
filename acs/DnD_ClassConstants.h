#ifndef DND_CLASSCONST_IN
#define DND_CLASSCONST_IN

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

void CheckDoomguyExecuteReversal(int this) {
	if(CheckActorInventory(this, "Doomguy_ChangedColor")) {
		TakeActorInventory(this, "Doomguy_ChangedColor", 1);
		TakeActorInventory(this, "Doomguy_ValidExecute", 1);
		ACS_NamedExecuteWithResult("DnD Doomguy Execute Translation", this, 1, MonsterProperties[this - DND_MONSTERTID_BEGIN].id);
	}
}

#endif