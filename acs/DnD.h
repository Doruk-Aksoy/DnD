#include "DnD_Common.h"
#include "DnD_Elites.h"
#include "DnD_DatabaseRows.h"
#include "DnD_MenuConstants.h"
#include "DnD_TempWeps.h"
#include "DnD_DamageCache.h"
#include "DnD_Ammo.h"
#include "DnD_Monsters.h"
#include "DnD_SpecialTrails.h"
#include "DnD_ChestKey.h"
#include "DnD_Skills.h"
#include "DnD_Settings.h"
#include "DnD_ClassMenu.h"

// for now allocate slots for 10
#define MAX_SCRIPT_TRACK 10

enum {
	DND_SCRIPT_EXPTRACKER
};

global bool 17: PlayerScriptsCheck[MAX_SCRIPT_TRACK][MAXPLAYERS];

#define DEATHRAY_MARKER_TID_ADD 1000

#define PERK_GAIN_RATE 5

#define BASEPISTCLIP 12
#define BASEMGCLIP 50
#define BASEHMGCLIP 60

// dash script defs, by KeksDose
#define SD_TIMEFRAME		7			// Tics during which you must double press
#define SD_DASHDELAY		27			// Tics before being able to dash again
#define SD_DASHSPEED		7.75	   	// Dash speed whilst in the air
#define SD_DASHJUMP			4.0	   	    // Adds a little z-velocity while in the air
#define SD_DASHSTRONG		12.0		// And dash speed whilst grounded
#define SD_SOUNDVOLUME		1.0			// How loud the dash sound is played

#define CHANCE_HEART 0.1

#define UPGRADETEXTID 6999
#define SURVIVEICO 7000
#define SURVIVETOPTEXTID 7001
#define SURVIVETEXTID 7002
#define SURVIVEID 7003

#define VORTEXTIDSTART 10000
#define P_TEMPTID 200
#define RAILINITTID 2000
#define RAILTIDADD 500
#define SPECIAL_FX_TID 32768

#define REFLECTFXTID 9000

#define INTERVENTION_DURATION TICRATE * 8

#define DND_EMERALD_TRANSLATIONID 7000

#define DND_BUDGET_BASE 3
#define DND_MAX_SHARE 4
#define DND_SPREE_AMOUNT 4 * TICRATE // 4 * 35
#define DND_SPREE_PER 15
#define DND_SPREE_TRIGGER DND_SPREE_PER - 1 // -1 because current monster killed doesn't count
#define DND_SPREE_BASE 4 // 1 / 4
#define DND_SPECIALAMMO_TEXTID 6001
#define DND_SPREE_BLOODID 6000
#define DND_SPREE_TEXT1ID 5999
#define DND_SPREE_TEXT2ID 5998

#define DND_MAX_MONSTERDELAY 8

#define DND_DEMONBANE_GAIN 50

#define DND_CYBERNETICARMOR_AMOUNT 300
#define DND_RAVAGERARMOR_AMOUNT 250

#define DND_HEALTHEXPSCALE 5
#define DND_HEALTHCREDITSCALE 10
#define DND_HEALTHCREDITAFTER50SCALE 5
#define DND_HEALTHCREDITAFTER75SCALE 10
#define DND_HEALTHCREDITUPSCALE 1
#define DND_MIN_CREDIT 3
#define DND_REGEN_PERCENT 2

#define TALENT_INCREASE 50
#define TALENT_PER_DEX 1
#define TALENT_PER_INT 1
#define DND_ARTIFACT_GAIN 50

#define DND_NECRO_BULKSCALE 8

#define DND_EXP_BASEFACTOR 4
#define DND_CREDIT_BASEFACTOR 5
#define DND_RESEARCH_MAX_CHANCE 1.0
#define DND_RESEARCH_DROPMULT 3

#define DND_TALISMAN_MARK 42049

#define DND_AFTER50_INCREMENT 200
#define DND_AFTER75_INCREMENT 800

#define AGAMOTTO_MOVE_WINDOW 1 << 16

#define TEMP_TID 999999

#define DND_MAX_DIGITLEN 7

#define DND_BOSS_SOULGIVE 4

#define RIP_FADE_TIMER 20
#define RIP_MONSTER_FADE_DELAY 3 * TICRATE

#define DND_NO_TRANSLATION 70

#define DND_SOULAMMO_DROPRATE 0.1
#define DND_SOULAMMO_SMALLCHANCE 75

#define DND_ITEM_LINGER_TIME 120

#define DND_WANDERER_EXP_DAMAGE 10 // 1 / 10 = 10%
#define DND_WANDERER_EXP_TID 54000

// RPG ELEMENTS END

#define DefStepSound "Player/Move"

// thunderstaff info things
#define DND_THUNDERSTAFF_MAXTARGETS 5
#define DND_THUNDERSTAFF_DAMAGERTID 32769
#define DND_THUNDER_RING_TIDSTART 40000
#define DND_THUNDER_RADIUSPERCOUNT 16
#define DND_THUNDERSTAFF_BASERANGE 128
typedef struct dist_tid_pair {
	int dist;
	int tid;
} dist_tid_pair_T;

#define DND_SHIFTBITS_FOR_SLOTFROMFLAG 13 // 8192 must return 0 to us
enum {
	DND_WDMG_USETARGET = 1,
	DND_WDMG_ISOCCULT = 2,
	DND_WDMG_ISPISTOL = 4,
	DND_WDMG_ISBOOMSTICK = 8,
	DND_WDMG_ISSUPER = 16,
	DND_WDMG_ISSPECIALAMMO = 32,
	DMG_WDMG_ISARTIFACT = 64,
	DND_WDMG_USEMASTER = 128,
	DND_WDMG_SETMASTER = 256,
	DND_WDMG_USETRACER = 512,
	DND_WDMG_ISRIPPER = 1024,
	DMG_WDMG_NECROBULK = 2048,
	DND_WDMG_HASSTRSCALING = 4096,
	DND_WDMG_ISSLOT1 = 8192,
	DND_WDMG_ISSLOT2 = 16384,
	DND_WDMG_ISSLOT3 = 32768,
	DND_WDMG_ISSLOT4 = 65536,
	DND_WDMG_ISSLOT5 = 131072,
	DND_WDMG_ISSLOT6 = 262144,
	DND_WDMG_ISSLOT7 = 524288,
	DND_WDMG_ISSLOT8 = 1048576,
	DND_WDMG_ISSLOT9 = 2097152,
	DND_WDMG_ISSPELL = 4194304
};

enum {
	DND_SPECIAL_RESEARCH = 1,
	DND_SPECIAL_ORB = 2,
	DND_SPECIAL_TALENTCAPSULE = 4,
	DND_SPECIAL_ELIXIR = 8,
	DND_SPECIAL_CHARM = 16
};

enum {
	DND_STACKEDITEM_ORB,
	DND_STACKEDITEM_CHESTKEY,
	DND_STACKEDITEM_ELIXIR
};

#define DND_BONUS_SCREENTIME 6
enum {
	BONUS_KILL,
	BONUS_ITEM,
	BONUS_SECRET,
	BONUS_BONUS,
	BONUS_EXP_RATE = 5,
	BONUS_CREDIT_RATE = 5,
	BONUS_SECRET_RATE = 3,
};
									  
str WeaponMsg[7] = 	{
	"\ccWeapon Pickup : \c[Y5]Chainsaw - 1\c-",
	"\ccWeapon Pickup : \c[Y5]Shotgun - 3\c-",
	"\ccWeapon Pickup : \c[Y5]Super Shotgun - 3\c-",
	"\ccWeapon Pickup : \c[Y5]Machine Gun - 4\c-",
	"\ccWeapon Pickup : \c[Y5]Rocket Launcher - 5\c-",
	"\ccWeapon Pickup : \c[Y5]Plasma Rifle - 6\c-",
	"\ccWeapon Pickup : \c[Y5]BFG 9000 - 7\c-"
};
							
str WeaponPickupText[MAXWEPS] = {
	 "Time to get your hands dirty. Does 10 - 30 damage normally or 180 - 240 with berserk per hit.",
	 "Open invitiation to the slaughterhouse! Can be \cdreplaced.",
	 "Double the blade, double the fun! Twice as much damage, same firing rate. Forces pain.",
	 "Does 80 - 120 damage per swing, x3 to undead or magical enemies. Alt fire charges and releases 17 baseballs each doing 100 - 150 on impact and 128 damage in a 128 unit radius. Alt fire requires Melee Expertise ability. \cfIgnores shields.\c- Can't hit \cughosts.",
	 "Unsheathing does 140 damage, normal firing does 84. Altfire allows swinging combos to do massive damage. Normal swings block projectiles. Altfire requires Melee Expertise ability.",
	 "60 - 240 damage per swing with 48 - 192 additional damage in a 96 unit radius. Alt fire shoots 5 flames doing 40 - 80 on hit and 192 - 240 damage in a 160 unit radius. Alt fire requires Melee Expertise ability. Can't hit \cughosts.",
	 "Does 100 - 400 damage per swing depending on how long you hold. Alt fire makes you shoot ripping slashes doing 24 damage in a 104 unit radius, run 15% faster but can't change weapons. Alt fire requires Melee Expertise ability.",
	 "Sickle steals life from enemies on hit. Does 40 - 60 damage 3 times. Alt fire swings for irreducable 75 - 90 damage 3 times. Altfire requires Melee Expertise ability.",
	 
	 "Your trustworthy Colt45, been with you since the start. Does 15 damage in a 0.5 spread. Can be \cdreplaced.",
	 "Akimbo longslides. Does 35 damage per shot in a 3.5 by 2.5 spread.",
	 "Magnum is a true classic, shooting bullets each doing 150 - 225 damage. Has a capacity of 6. 50 damage from bullet rip through enemies. Alt fire reloads. \cfIgnores shields.",
	 "Laser Pistol is the fresh invention of UAC. Shoots lasers doing 15 - 30 damage in a 2.0 by 1.25 spread. Alt fire charges to do up to a x5 damage rail. Doesn't use ammo. \cfIgnores shields.",
	 "Assault Rifle does 30 damage per bullet in a 3.6 by 2.4 spread. Magazine capacity of 31. Alt fire zooms, allowing more precise shots.",
	 "Summons mobile viper traps, jumping on enemies doing 120-200 damage in 128 unit radius. They expire after 12 seconds. Alt fire shoots slithering vipers doing 150-250 damage, \cfignoring shields\c-.",
	 "Casts 3 flames doing 10-40 damage each. Alt fire casts a flame circle at most 384 units away doing 25 initial damage and creating 8 flames doing 32-64 damage on hit and 24 radius damage in 24 units. Can't hit \cughosts.",
	 "Scatter Pistol shoots 3 pellets each doing 10 - 20 damage. Pellets scatter to 6 tiny pellets doing 6 - 12 damage. Alt fire shoots one pellet. \cfIgnores shields.\c-",
	 
	 "Typical boomstick, now spreads vertically too. 10 pellets, each doing 10-15 damage. Can be \cdreplaced\c-. Can use \cialternate\c- ammo.",
	 "The purifier shotgun, spread's with 3.6 by 3.6. 15 pellets, each doing 15 damage. Has a shell capacity of 8. Alt fire reloads. Can use \cialternate\c- ammo.",
	 "Killstorm Auto Shotgun, drum fed with 12 shells, can shoot 12 pellets each doing 18 damage in a 7.2 by 5.2 spread. Alt fire reloads.",
	 "Fires 5 blasts doing 20 damage in a 6 by 4.25 spread. Altfire fires a projectile that deals 150 damage, releasing an acid rain, doing 8-32 damage each. These can't hit \cughosts\c-. Victims explode on death doing 100 damage in 96 unit radius.",
	 "Deadlocks fires 16 pellets doing 15 damage in a 7.0 by 5.2 spread. Has a shell capacity of 12. Can use \cialternate\c- ammo.",
	 "Fires shots that do 210 ice damage. Alt fire shoots a blast of nitrogen 384 units ahead, creating 4 series of icy gas streams doing 5 damage.",
	 "An artifact that does 160 damage up to 1024 units, sending a healing bolt. If a \cgdemon\c- was hit, does an explosion in 160 unit radius doing 192 damage. Altfire does 10-20 melee damage. If a \cgdemon\c- is hit, gives 1 ammo.",
	 
	 "Same old buckshot we all love! Can be \cdreplaced.",
	 "Heavy Super Shotgun shoots 28 pellets doing 15 damage in a 9.6 by 5.8 spread. Half of these rip through targets.",
	 "Erasus shotgun shoots highly ballistic shells with 18 pellets each doing 15 damage. Has to reload after shooting twice. Alt fire shoots both shells in the chamber, or reloads.",
	 "Shoots a missile and 3 mini missiles. Missile does 45, mini missiles do 15 and explode for 20 in 32 unit radius, not hitting \cughosts\c-. Main missile can scatter. If it hits an object, explodes for 30 in 64 unit radius. Altfire fires the other side.",
	 "Fires 24 plasma balls in a circular fashion each doing 20 damage. Has a clip size of 5.",
	 "Shoots 18 shells each doing 15 damage and forcing pain. Overheats when used. Altfire releases a portion of it, dealing 108-180 damage in 96 unit radius. \cfIgnores shields.",
	 "Fires 15 shells doing 13 damage in a 11.6 and 9.0 spread, releasing embers on hit doing 2 damage. Altfire shoots a chunk of embers doing 30 damage on hit. Pressing altfire while on flight splits it into 15 embers doing 18 damage.",
	 
	 "The explosive shotgun, the best there is. Fires 10 pellets, each doing 15 on hit. Each pellet does 32-48 damage in a small area. Does self damage. \cfIgnores shields.",
	 "Slayer creates 6 blades each doing 10 damage and rip through. Alt fire detonates blades at will for 100 damage in a 108 unit radius, \cfignoring \cfshields\c-. Blades return to you after travelling a bit. Can't hit \cughosts.",
	 
	 "An accurate and a very reliable weapon. Each bullet does 15-20 damage. Alt fire reloads. Clip size is 50. Can be \cdreplaced\c-. Can use \cialternate\c- ammo.", 
	 "Finest machine guns UAC has to offer. Bullets do 25 damage in a 1.6 by 0.8 spread. Clip size is 60. Can zoom.",
	 "Lead Spitter is a super sonic bullet shooter shooting 2 bullets doing 18 damage in a 6.4 by 4.8 spread. Clip size is 75. \cfIgnores shields.",
	 "Japanese technology brings the finest demon hunting tool. Shoots magic cards that deal 15 damage, reduce monster damage and resistance by 50%. If the monster is below 10% health, culls the monster.",
	 "Templar fires silver bullets doing 20 damage in a 4.4 by 2.8 spread. Bullets deal x3 damage to undead and magical enemies. Clip size of 40. Can use \cigrenades\c-.",
	 "Fires 7 pellets doing 12 damage in a 3.6 by 3.6 spread. Alt fire makes it full auto, but twice as inaccurate. Can use \cialternate\c- ammo. Reload when full to use other ammo.",
	 "Fires acid rounds doing 18 damage on hit and 10-15 damage in a 48 unit radius. Alt fire shoots a bolt that sticks to enemies, detonating after 3 seconds for 96 damage and release toxic cloud doing 10-15 damage in 96 unit radius.",
	 "Fires in bursts of 5 each doing 25 damage in a 6.8 by 4.2 spread. Altfire shoots a fusion grenade doing 150 damage and releasing rippers doing 5 damage around. Attacks do more damage with more distance. \cfIgnores shields.\c-",
	 
	 "Desolator fires highly toxic rounds doing 30 damage. Subsequent hits make enemies take 10% more damage from elemental attacks, maximum 5 stacks. At max stacks, rounds cause a toxic explosion doing 80 damage in 128 unit radius.",
	 "Stronger, faster and better than ever! Poor accuracy, shoots tracers that do 16 - 28 damage each. Alt fire to spin. Can't hit \cughosts.",
	 "The ebony cannon shoots bouncing balls of death. 32 - 48 damage with 48 explosion damage in 64 units. Alt fire shoots scatter bombs. \cfIgnores shields.",
	 
	 "A true classic. Just don't blow yourself up. Can be \cdreplaced. Can't hit \cughosts.",
	 "The Torpedo Launcher shoots fast torpedos each doing 300 - 500 damage on impact and 224 damage in a 144 unit radius. Can't hit \cughosts.",
	 "Mercury Launcher fires accelerating and heat seeking mercury missiles doing 256 - 320 damage on hit and 192 damage in a 160 unit radius over 2 seconds. Can't hit \cughosts.",
	 "Shoots 10 flak shells in 10.4 by 7.8 doing 5 impact damage and 20 explosion damage in 96 unit radius. The shells explode 320 units ahead to scatter into 3 explosive particles each doing 16 damage in 96 unit radius. Alt fire can zoom to improve accuracy by 50%.",
	 "Fires a meteor doing 200 on impact and 192 in a 192 unit radius. The meteor then splits into smaller pieces, and those pieces as well. Main meteor \cfignores shields\c-.",
	 "Fires grenades doing 128 on impact and 128 in a 128 unit radius. The grenade explodes into shrapnels ripping through doing 6-18 damage. Alt fire loads more grenades in the chamber. At most 3 additional grenades. Can't hit \cughosts.",
	 "Launches a ball of ice that does 150 damage on impact. After some time it'll stop and explode doing 150 damage in 176 unit radius, releasing many ice particles around each doing 3-9 damage, ripping through enemies. They also explode and do 36 damage in 64 unit radius. Can \cgoverheat\c-.",
	 "Useful for when you can't reach around corners. Does 80 damage on impact and 128 damage in a 144 unit radius. Can be \cdreplaced\c-. Can't hit \cughosts\c-. Can use \cialternate\c- ammo.",
	 "The UAC Rotary Grenade Launcher does 192 damage on impact and 384 damage on a 192 unit radius. Can't hit \cughosts",
	 "Top of the food chain for rockets. Shoots two homing rockets each doing 192 damage both on impact and explosion. Can't hit \cughosts.",
	 
	 "Best friend of the trigger happy. Can be \cdreplaced.",
	 "Improved with a nuclear reactor, does 36 - 60 on hit and 10 - 30 explosion damage in a 48 unit radius. Can \cgoverheat\c-. Does self damage.",
	 "Turel Cannon fires highly ionized particles doing 125 damage ripping through everything. Continuous fire is less accurate and does 80 damage. Has a range of 768 units. Altfire fires focused shots.",
	 "Launches 5 ice shards doing 15 - 30 damage in a 7.5 by 7.5 spread. Alt fire launches a glacial orb launching ice shards all around with itself doing 6-9 damage and rips through enemies. Can't hit \cughosts.",
	 "Flamethrower does what it says and throws flames doing 1 - 8 damage. When they hit, they leave a trail of flame doing 5 damage every 2 tics. Fuel size of 75.",
	 "UAC offers this shockingly deadly weapon that can shoot lightning doing 9-12 damage. Alt fire shoots forked lightning. Keep firing and da- mage increases by 4% per stack. Stacks additively.",
	 "Fires projectiles doing 25 damage. For every 24 point mark on your overheat, each hit rebounds to 2 other projectiles. Altfire shoots a special projectile doing 600 damage in 256 radius to enemies with \cuHardened Skin\c- or \cfShield\c-.",
	 
	 "Rhino AR fires heavy rounds dealing 75 damage in a 7.2 by 4.8 spread. Alt fire zooms, allowing for higher precision but loss of rapid fire. Can use \cialternate\c- ammo. Reload when full to use other ammo.",
	 "An interesting demonic artifact shooting nails. Each nail does 13 - 21 damage and rips through. Alt fire shoots explosive lava nails that \cfignores shields.\c- Can't hit \cughosts.",
	 "Basilisk is said to be the ancient weapon of hell's most elite warriors. If not loaded, shoots fire balls doing 18 - 48 damage. If loaded, shoots meteors doing 72 - 144 on impact and 96 explosion damage. \cf Ignores shields.",
	 
	 "BFG 6000, an older model but still capable. Can be \cdreplaced.\cf Ignores shields.",
	 "The newest BFG model 32768, devastates with 600 - 900 damage on impact and 384 damage in a 160 unit radius. Also shoots 64 tracers instead of 40.",
	 "Devastator launches five mini rockets each doing 64 to 80 with 32 radius damage in 96 units. Can't hit \cughosts. \c-\cfIgnores shields.",
	 "Fires a destructive orb doing 425 damage on impact and 448 damage in a 768 unit radius. Creates 6 smaller explosions doing 200 damage on impact and 192 damage in a 256 unit radius. Does self damage. Main orb \cfignores shields.",
	 "Fires ionized energy doing 125 impact and 160 area damage in 160 unit radius, then 80 in 80 and 53 in 60. Can \cgoverheat\c-. \cfIngores shields\c-. Can't hit \cughosts\c-.",
	 "Summons a lightning ball that zaps 5 nearest enemies for 115 damage in 420 units. On impact deals 250-500 and 250 radius damage in 96 units. Altfire zaps all enemies in range for 500 on impact and 250 in 256 units, \cfignoring shields.",
	 "Gauss Rifle fires a magnetic pulse dealing 100 direct hit damage and 192 radius damage in a 96 unit radius. Alt fire zooms and amplifies the damage for each zoom. Can't hit \cughosts.\c- \cfIgnores shields.",
	 "This baby can rip through concrete with ease. Each shot does multiples of 92. Alt fire charges up the next shot up to 2 times. \cfIgnores shields.",
	 "Fires energy particles doing 300 damage on impact and 75 area damage in 96 unit radius. Using altfire while particles are midflight causes a laser to be emitted to target area, doing 200 damage on both impact and in a 160 unit radius, \cfignoring shields.\c-",
	 
	 "A magical staff, using demon souls as energy. Fires meteors of magic, bursting on impact. Alt fire fires 3 columns of fire both on floor and ceiling that travel and explode in flames. \cfIgnores shields.",
	 "This once was the ribcage of a powerful demon. Fires magical bone shards that rip through. Alt fire switches the mode to shoot three demon shredders that seek demons.",
	 "This holy relic was lost in ancient battles. Fires sun beams to burn anything. Alt fire channels the very essence of sun causing a massive meltdown. \cfIgnores shields\c-.",
	 "Soul Reaver creates portals that lead to hungry ghosts that devour their enemies. Hold to gain a deflective shield. \cfIgnores shields.",
	 
	 "The sawedoff is a western classic. Fires 16 pellets each doing 15 damage. \ceTemporary Weapon.",
	 "Soul Render is an unholy melee weapon. Each hit makes the weapon become less durable. Alt fire toggles life drain mode. \ceTemporary Weapon.",
	 "A standard UAC submachine gun. Each bullet does 15 damage. \ceTemporary Weapon.",
	 "The arm-cannon of a Corpulent. Shoots metallic cannon balls that split on impact. Alt fire shoots tiny ripping cannon balls. \ceTemporary Weapon.",
	 "The spine of a Bloodfiend. Shoots tiny explosive pukes. Alt fire changes it to a limited range hitscan. Mainfire can't hit \cughosts\c-. \ceTemporary Weapon.",
	 "The laser rifle shoots a laser or two, if using alt fire, each doing 20-45 damage. \cfIgnores \cfshields. \ceTemporary Weapon.",
	 "The torso of a Vulgar. Shoots acidic bones. Alt fire shoots bouncing acid bombs. Can't hit \cughosts\c-. \ceTemporary Weapon.",
	 "A demon heart. Squeeze to damage all enemies  in an area.\cfIgnores shields. \ceTemporary Weapon.",
	 "Gloves of a dark servant. Can shoot pain enforcing lightning bolts doing 60-90 damage. Alt fire shoots a homing explosive lightning ball doing 120-180 damage on hit and 96 area damage over 9 tics. \ceTemporary Weapon.",
	 "A nail-chaingun, quite useful for mass murder of demons. Shoots nails ripping for 16-32 damage. \ceTemporary Weapon.",
	 "True classic, in akimbo fashion too! Shoots bullets doing 10-30 damage each. \ceTemporary Weapon.",
	 "Can blast foes with plasma doing 24-40 damage with 35 explosion damage. Alt fire can shoot a ripping version of this same plasma. \ceTemporary Weapon.",
	 "Ripper fires penetrating rounds in a 14.2 and 7.6 spread each doing 25 damage. \ceTemporary Weapon."
};					
	
#define MAX_SPREE_TEXT 20
str SpreeText[MAX_SPREE_TEXT] = {
	"Cruel",
	"Merciless",
	"Crushing",
	"Sadistic",
	"Vicious",
	"Destructive",
	"Terrifying",
	"Terrorizing",
	"Rampage",
	"Unstoppable",
	"Massacre",
	"Bloodthirsty",
	"Monstrous",
	"Genocide",
	"Holocaust",
	"Diabolical",
	"Epic",
	"Legendary",
	"Godlike",
	"HOLY SHIT"
};

#define DND_GOLDCHESTKEY_DROPRATE 0.1
#define DND_SILVERCHESTKEY_DROPRATE 0.25

#define CHEST_DROPWEIGHT 10
#define SILVERCHEST_DROPWEIGHT 37
#define GOLDCHEST_DROPWEIGHT 50

#define AMMODISPLAY_ID 1000

bool Quest_Pick_Done = 0;
bool PlayerCanLoad[MAXPLAYERS] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

// various states are checked using this, such as bonus states or player joins
// don't add in between, add below!
enum {
	DND_STATECHECK_PLAYERJOIN,
	DND_STATECHECK_KILLBONUS,
	DND_STATECHECK_ITEMBONUS,
	DND_STATECHECK_SECRETBONUS,
	DND_STATECHECK_BONUSBONUS
};

#define MAX_STATES DND_STATECHECK_BONUSBONUS + 1
int DnD_StateChecker[MAX_STATES];
// see if map changed or not

int PlayerWeaponUsed[MAXPLAYERS] = { -1 };

int CurrentLevelReward[MAXPLAYERS];
int CurrentStatReward[MAXPLAYERS];

#define MAXCREDITDROPS 3
#define CREDITDROP_NORMAL 0
#define CREDITDROP_ELITE 1
str CreditLabels[MAXCREDITDROPS][2] = {
	{ "LargeCreditDropper", "LargeCreditDropper_Elite" },
	{ "MediumCreditDropper", "MediumCreditDropper_Elite" },
	{ "SmallCreditDropper", "SmallCreditDropper_Elite" }
};

#define CREDITDROP_THRESHOLD 0
#define CREDITDROP_CHANCE 1
int CreditDropValues[MAXCREDITDROPS][2] = {
	{ 10000, 0.05 },
	{ 5000, 0.15 },
	{ 750, 0.3 }
};

void Reset_RPGInfo (int resetflags) {
	int i;
	if(resetflags & RESET_LEVEL) {
		SetInventory("Exp", 0);
		SetInventory("LevelExp", 0);
		SetInventory("Level", 1);
	}
	
	if(resetflags & RESET_CREDIT)
		SetInventory("Credit", 0);
		
	if(resetflags & RESET_STATS) {
		for(i = DND_ATTRIB_BEGIN; i <= DND_ATTRIB_END; ++i)
			SetInventory(StatNames[i], 0);
		SetActorProperty(0, APROP_HEALTH, DND_BASE_HEALTH);
	}
	
	if(resetflags & RESET_PERKS) {
		if(CheckInventory("Perk_Sharpshooting"))
			TakeInventory(StrParam(s:"Damage_Perk_", d:CheckInventory("Perk_Sharpshooting") * 5), 1);
		if(CheckInventory("Perk_Endurance"))
			TakeInventory(StrParam(s:"Resist_Perk_", d:CheckInventory("Perk_Endurance") * 5), 1);
		for(i = DND_PERK_BEGIN; i <= DND_PERK_END; ++i)
			SetInventory(StatNames[i], 0);
		SetAmmoCapacity("StoredMedkit", DND_BASE_HEALTH);
	}
}

int CheckLevelUp (void) {
	int curlevel = GetStat(STAT_LVL), exptemp;
	// -1 because initial level is 1
	while(curlevel < MAXLEVELS && GetStat(STAT_EXP) >= LevelCurve[GetStat(STAT_LVL) - 1]) {
		exptemp = GetStat(STAT_EXP) - LevelCurve[GetStat(STAT_LVL) - 1];
		if(!((GetStat(STAT_LVL) + 1) % 5)) { // multiples of 5 give perk
			GiveInventory("PerkPoint", 1);
			GiveInventory("PerkedUp", 1);
		}
		if(!((GetStat(STAT_LVL) + 1) % DND_TALENTPOINT_MARK)) {
			// disabled for now
			//GiveInventory("TalentPoint", 1);
			//GiveInventory("TalentedUp", 1);
		}
		GiveInventory("Level", 1);
		SetAmmoCapacity("ExpVisual", LevelCurve[GetStat(STAT_LVL) - 1]);
		SetInventory("Exp", exptemp);
		SetInventory("ExpVisual", exptemp);
		GiveInventory("AttributePoint", ATTRIB_PER_LEVEL);
	}
	return GetStat(STAT_LVL) - curlevel;
}

void HandleLevelup() {
	int prevlvl = CheckInventory("Level");
	if(CheckLevelUp()) {
		LocalAmbientSound("RPG/LevelUp", 127);
		GiveInventory("LevelUpEffectSpawner", 1);
		GiveInventory("LeveledUp", 1);
		++PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL];
		if(GetStat(STAT_LVL) - 1 == PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL])
			PlayerInformationInLevel[PLAYERLEVELINFO_MAXLEVEL] = GetStat(STAT_LVL);
		// heal on level up flag is on
		if(GetCVar("dnd_healonlevelup"))
			ACS_NamedExecuteAlways("DnD Health Pickup", 0, 100, 0);
		
		int curlvl = CheckInventory("Level");
		// player just leveled and got their perks? check if so
		if
		(
			(prevlvl < DND_CLASSPERK1_LEVEL && curlvl >= DND_CLASSPERK1_LEVEL) ||
			(prevlvl < DND_CLASSPERK2_LEVEL && curlvl >= DND_CLASSPERK2_LEVEL) ||
			(prevlvl < DND_CLASSPERK3_LEVEL && curlvl >= DND_CLASSPERK3_LEVEL)
		) {
			HandleClassPerks();
			// make some announcement the player has a new perk
			ACS_NamedExecuteAlways("DnD Announcer", 0, DND_ANNOUNCER_NEWCLASSPERK);
		}
		else
			ACS_NamedExecuteAlways("DnD Announcer", 0, DND_ANNOUNCER_ATTRIBPOINT);
	}
}

int DnD_BonusMessageY(int bonustype) {
	int res = 34.1;
	for(int i = DND_STATECHECK_KILLBONUS; i <= DND_STATECHECK_BONUSBONUS; ++i)
		if(DnD_StateChecker[i])
			res += 16.0;
	return res;
}

// Map Evaluation

enum {
	// Monsters
	DND_ZOMBIE_CONTRIB = 3,
	DND_SHOTGUNNER_CONTRIB = 4,
	DND_CHAINGUNNER_CONTRIB = 5,
	DND_IMP_CONTRIB = 4,
	DND_DEMON_CONTRIB = 6,
	DND_HELLKNIGHT_CONTRIB = 10,
	DND_BARON_CONTRIB = 18,
	DND_CACO_CONTRIB = 8,
	DND_PAIN_CONTRIB = 12,
	DND_SOUL_CONTRIB = 4,
	DND_REVENANT_CONTRIB = 9,
	DND_ARACHNO_CONTRIB = 16,
	DND_VILE_CONTRIB = 20,
	DND_FATSO_CONTRIB = 16,
	DND_SPIDERMASTERMIND_CONTRIB = 45,
	DND_CYBERDEMON_CONTRIB = 50,
	DND_BOSSBRAIN_CONTRIB = 9000,
	
	// Items
	DND_HEALTHBONUS_CONTRIB = -1,
	DND_ARMORBONUS_CONTRIB = -1,
	DND_STIM_CONTRIB = -6,
	DND_MEDIKIT_CONTRIB = -12,
	DND_BERSERK_CONTRIB = -30,
	DND_INVUL_CONTRIB = -80,
	DND_SOULSPHERE_CONTRIB = -30,
	DND_MEGA_CONTRIB = -60,
	DND_EVIL_CONTRIB = -20,
	DND_GREENARMOR_CONTRIB = -12,
	DND_YELLOWARMOR_CONTRIB = -18,
	DND_BLUEARMOR_CONTRIB = -24,
	DND_REDARMOR_CONTRIB = -36
};

enum {
	DND_VERYEASY,
	DND_EASY,
	DND_MEDIUM,
	DND_HARD,
	DND_VERYHARD
};

enum {
	DND_VERYEASY_LIMIT = 300,
	DND_EASY_LIMIT = 500,
	DND_MEDIUM_LIMIT = 800,
	DND_HARD_LIMIT = 1250,
	DND_VERYHARD_LIMIT = 1800
};

// 5 Tiers: 0 -> Very Easy, 1 -> Easy, 2 -> Medium, 3 -> Hard and 4 -> Very Hard.
void CalculateMapDifficulty() {
	int factor = 0;
	// yes this is ugly but it won't ever change, no new spawners will come etc so why not :)
	factor += ThingCountName("ZombiemanSpawner", 0) * DND_ZOMBIE_CONTRIB;
	factor += ThingCountName("ShotgunguySpawner", 0) * DND_SHOTGUNNER_CONTRIB;
	factor += ThingCountName("ChaingunguySpawner", 0) * DND_CHAINGUNNER_CONTRIB;
	factor += ThingCountName("DemonSpawner", 0) * DND_DEMON_CONTRIB;
	factor += ThingCountName("SpectreSpawner", 0) * DND_DEMON_CONTRIB;
	factor += ThingCountName("ImpSpawner", 0) * DND_IMP_CONTRIB;
	factor += ThingCountName("LostSoulSpawner", 0) * DND_SOUL_CONTRIB;
	factor += ThingCountName("CacodemonSpawner", 0) * DND_CACO_CONTRIB;
	factor += ThingCountName("PainElementalSpawner", 0) * DND_PAIN_CONTRIB;
	factor += ThingCountName("RevenantSpawner", 0) * DND_REVENANT_CONTRIB;
	factor += ThingCountName("HellKnightSpawner", 0) * DND_HELLKNIGHT_CONTRIB;
	factor += ThingCountName("BaronSpawner", 0) * DND_BARON_CONTRIB;
	factor += ThingCountName("FatsoSpawner", 0) * DND_FATSO_CONTRIB;
	factor += ThingCountName("SpiderSpawner", 0) * DND_ARACHNO_CONTRIB;
	factor += ThingCountName("ArchVileSpawner", 0) * DND_VILE_CONTRIB;
	factor += ThingCountName("MastermindSpawner", 0) * DND_SPIDERMASTERMIND_CONTRIB;
	factor += ThingCountName("CyberSpawner", 0) * DND_CYBERDEMON_CONTRIB;
	factor += ThingCountName("BossBrain", 0) * DND_BOSSBRAIN_CONTRIB;
	
	factor += ThingCountName("InvulnerabilitySphere2", 0) * DND_INVUL_CONTRIB;
	factor += ThingCountName("StimpackSpawner", 0) * DND_STIM_CONTRIB;
	factor += ThingCountName("MedikitSpawner", 0) * DND_MEDIKIT_CONTRIB;
	factor += ThingCountName("BonusReplacer", 0) * DND_HEALTHBONUS_CONTRIB;
	factor += ThingCountName("BonusReplacer2", 0) * DND_ARMORBONUS_CONTRIB;
	factor += ThingCountName("EvilSphere2", 0) * DND_EVIL_CONTRIB;
	factor += ThingCountName("UberSphere2", 0) * DND_MEGA_CONTRIB;
	factor += ThingCountName("SoulSphere3", 0) * DND_SOULSPHERE_CONTRIB;
	factor += ThingCountName("MegaSphere3", 0) * DND_MEGA_CONTRIB;
	factor += ThingCountName("Berserk2", 0) * DND_BERSERK_CONTRIB;
	factor += ThingCountName("NewGreenArmor2", 0) * DND_GREENARMOR_CONTRIB;
	factor += ThingCountName("YellowArmor2", 0) * DND_YELLOWARMOR_CONTRIB;
	factor += ThingCountName("NewBlueArmor2", 0) * DND_BLUEARMOR_CONTRIB;
	factor += ThingCountName("TheRedArmor2", 0) * DND_REDARMOR_CONTRIB;
	if(factor >= DND_VERYHARD_LIMIT)
		MapDifficulty = DND_VERYHARD;
	else if(factor >= DND_HARD_LIMIT)
		MapDifficulty = DND_HARD;
	else if(factor >= DND_MEDIUM_LIMIT)
		MapDifficulty = DND_MEDIUM;
	else if(factor >= DND_EASY_LIMIT)
		MapDifficulty = DND_EASY;
	else
		MapDifficulty = DND_VERYEASY;
	
	SetInventory("MapDifficultyClientside", MapDifficulty);
}

int CalculateBonus(int bonustype, int mdifficulty) {
	if(bonustype == BONUS_KILL) {
		// add 5% for each difficulty level
		return BONUS_EXP_RATE * (mdifficulty + 1);
	}
	else if(bonustype == BONUS_ITEM) {
		// add 250 credit for each difficulty level
		return BONUS_CREDIT_RATE * (mdifficulty + 1);
	}
	else if(bonustype == BONUS_SECRET) {
		// add budget for each difficulty level + 1
		return BONUS_SECRET_RATE * (mdifficulty + 2);
	}
	else if(bonustype == BONUS_BONUS) {
		return BONUS_SECRET_RATE * 2 * (mdifficulty + 1);
	}
	return 1;
}

void ShowBonusMessage(int bonustype, int y) {
	int bval = CalculateBonus(bonustype,CheckInventory("MapDifficultyClientside"));
	SetHudSize(800, 600, 1);
	SetFont("BIGFONT");
	switch(bonustype) {
		case BONUS_KILL:
			LocalAmbientSound("RPG/KillBonus", 127);
			HudMessage(s:"All monsters have been killed! ", d:bval, s:"% Exp bonus!"; HUDMSG_FADEINOUT, KILLBONUSID, CR_RED, 400.4, y, 4.0, 1.0, 1.0);
		break;
		case BONUS_ITEM:
			LocalAmbientSound("RPG/ItemBonus", 127);
			HudMessage(s:"All items have been taken! ", d:bval, s:"% Credit Bonus!"; HUDMSG_FADEINOUT, ITEMBONUSID, CR_GOLD, 400.4, y, 4.0, 1.0, 1.0);
		break;
		case BONUS_SECRET:
			LocalAmbientSound("RPG/SecretBonus", 127);
			HudMessage(s:"All secrets have been found! ", d:bval, s:"k Budget!"; HUDMSG_FADEINOUT, SECRETBONUSID, CR_GOLD, 400.4, y, 4.0, 1.0, 1.0);
		break;
		case BONUS_BONUS:
			LocalAmbientSound("RPG/BonusBonus", 127);
			HudMessage(s:"The entire map is toast! You're fully healed!"; HUDMSG_FADEINOUT, BONUSBONUSID, CR_GOLD, 400.4, y, 4.0, 1.0, 1.0);
		break;
	}
}

void DistributeBonus(int bonustype) {
	int bval = 0, temp = 0, i = 0;
	if(bonustype == BONUS_KILL) {
		bval = CalculateBonus(BONUS_KILL, MapDifficulty);
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(PlayerInGame(i) && isActorAlive(i + P_TIDSTART)) {
				temp = GetActorStat(i + P_TIDSTART, STAT_LVLEXP) * bval / 100;
				GiveActorInventory(i + P_TIDSTART, "DnD_KillBonusShower", 1);
				GiveActorExp(i + P_TIDSTART, temp);
			}
		}
	}
	else if(bonustype == BONUS_ITEM) {
		bval = CalculateBonus(BONUS_ITEM, MapDifficulty);
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(PlayerInGame(i) && isActorAlive(i + P_TIDSTART)) {
				temp = GetActorStat(i + P_TIDSTART, STAT_LVLCRED) * bval / 100;
				GiveActorInventory(i + P_TIDSTART, "DnD_ItemBonusShower", 1);
				GiveActorCredit(i + P_TIDSTART, bval);
			}
		}
	}
	else if(bonustype == BONUS_SECRET) {
		bval = CalculateBonus(BONUS_SECRET, MapDifficulty);
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(PlayerInGame(i) && isActorAlive(i + P_TIDSTART)) {
				GiveActorInventory(i + P_TIDSTART, "DnD_SecretBonusShower", 1);
				GiveActorBudget(i + P_TIDSTART, bval);
			}
		}
	}
	else if(bonustype == BONUS_BONUS) {
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(PlayerInGame(i) && isActorAlive(i + P_TIDSTART)) {
				SetActivator(i + P_TIDSTART);
				GiveInventory("DnD_BonusBonusShower", 1);
				ACS_NamedExecuteAlways("DnD Health Pickup", 0, 100, 4); // full heal
			}
		}
		SetActivator(-1);
	}
}

int IsUsingMeleeWeapon() {
	return CheckWeapon(" Fists ") || 
		   CheckWeapon(" Chainsaw ") || 
		   CheckWeapon("Upgraded Chainsaw") || 
		   CheckWeapon("Sickle") || 
		   CheckWeapon("Excalibat") || 
		   CheckWeapon("Katana") ||
		   CheckWeapon("Dusk Blade") ||
		   CheckWeapon("ResMelee1") ||
		   CheckWeapon("Soul Render");
}

// make these actually global sometime
int Monster_VariationCounts[MAX_MONSTER_CATEGORIES] = { 0 };
int Monster_CategoryWeightSum[MAX_MONSTER_CATEGORIES] = { 0 };

void HandleHunterTalisman() {
	if(CheckInventory("HunterTalismanCheck") && !CheckInventory("HunterTalismanCooldown")) {
		ActivatorSound("HunterTalisman/Activate", 1.0);
		GiveInventory("HunterTalismanCooldown", 1);
		GiveInventory("HunterTalismanEffector", 1);
	}
}

void PickQuest() {
	if(GetCVar("dnd_enable_quests") && PlayerCount()) {
		if(GetCVar("dnd_quest_avglevel") <= (PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] / PlayerCount()) && random(1, 100) <= Clamp_Between(GetCVar("dnd_quest_chance"), 1, 100)) {
			do {
				active_quest_id = random(0, MAX_QUESTS - 1);
			} while(!IsValidQuest(active_quest_id));
		}
	}
	else
		active_quest_id = -1;
	Quest_Pick_Done = 1;
}

void ClearQuestCheckers() {
	for(int i = 0; i < MAX_QUESTS; ++i)
		SetInventory(Quest_Checkers[i], 0);
}

int GetTalent(int talent_type) {
	return CheckInventory(TalentNames[talent_type][TALENT_NAME]);
}

void CheckMapExitQuest(int pnum, int qid) {
	int tid = pnum + P_TIDSTART;
	bool cond = 0;
	// handle exceptions
	// immediate conclusion quests are concluded as soon as condition is met
	if(Quest_List[qid].qflag & QTYPE_IMMEDIATE)
		return;
	if(qid == QUEST_NODYING)
		cond = true; // it would fail if people died, if not it succeeded
	else if(!QuestExitCheckException(qid)) // typical map exit quests
		cond = !CheckActorInventory(tid, Quest_Checkers[qid]);
		
	if(!IsQuestComplete(tid, qid)) {
		if(cond)
			CompleteQuest(tid, qid);
		else
			FailQuest(tid);
	}
}

void HandleCashDrops(int pnum, bool isElite) {
	int basechance = 1.0;
	int addedchance = (Clamp_Between(GetCVar("dnd_credit_droprateadd"), 0, 100) << 16) / 100;
	int luck = GetDropChance(pnum, isElite);
	if(isElite) {
		basechance -= DND_ELITE_CREDITCHANCE_BONUS;
		basechance -= GetDropChance(pnum, isElite);
	}
	
	basechance = random(0, basechance); // reduced max number implies increased chances of getting
	for(int i = 0; i < MAXCREDITDROPS; ++i) {
		if(CheckInventory("MonsterBaseHealth") >= CreditDropValues[i][CREDITDROP_THRESHOLD]) {
			if(basechance - addedchance - luck > CreditDropValues[i][CREDITDROP_CHANCE])
				continue;
			if(isElite)
				GiveInventory(CreditLabels[i][CREDITDROP_ELITE], 1);
			else
				GiveInventory(CreditLabels[i][CREDITDROP_NORMAL], 1);
			break;
		}
	}
}

// 0 means they are ready
bool PlayersNotReadyForHardcore() {
	int players_notready = 0;
	for(int i = 0; i < MAXPLAYERS; ++i)
		if (PlayerInGame(i) && PlayerCanLoad[i])
			players_notready++;
	return players_notready;
}

void SpawnTalentCapsule() {
	if(GameType() != GAME_SINGLE_PLAYER) {
		for(int i = 0; i < MAXPLAYERS; ++i)
			if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART))
				SpawnDrop("TalentCapsule", 0, 0, i + 1, 1);
	}
	else
		SpawnDrop("TalentCapsule_SP", 0, 0, 1, 1);
}

void SpawnResearch() {
	// spawn copies of this research
	int temp = 0;
	// roll another if this has nodrop flag on it
	do {
		temp = random(0, MAX_RESEARCHES - 1);
	} while(ResearchFlags[temp].res_flags & RESF_NODROP);
	if(GameType() != GAME_SINGLE_PLAYER) {
		for(int i = 0; i < MAXPLAYERS; ++i) {
			// spawn this only if this isn't already found by the player
			if(PlayerInGame(i) && IsActorAlive(i + P_TIDSTART) && !CheckResearchStatus(temp))
				SpawnDrop("ResearchModule_MP", 24.0, 16, i + 1, temp);
		}
	}
	else if(!CheckResearchStatus(temp)) // 1 before temp is player (0 + 1)
		SpawnDrop("ResearchModule_MP", 24.0, 16, 1, temp);
}

void SpawnAccessory() {
	int id;
	if(HardcoreSet) {
		do {
			id = random(1, MAX_ACCESSORY);
		} while(id == DND_REPLACE_THIS_ACCESSORY);
	}
	else
		id = random(1, MAX_ACCESSORY);
	SpawnDrop(StrParam(s:"Accessory_", d:id), 0, 0, 0, 0);
}

void HandleChestSpawn() {
	int r = random(1, 100);
	if(r >= CHEST_DROPWEIGHT) {
		if(!SpawnedChests) {
			r = random(0, 100);
			if(r < SILVERCHEST_DROPWEIGHT)
				SpawnDrop("DNDSilverChest", 0, 0, 0, 0);
			else if(r < GOLDCHEST_DROPWEIGHT)
				SpawnDrop("DNDGoldChest", 0, 0, 0, 0);
			else
				SpawnDrop("DNDBronzeChest", 0, 0, 0, 0);
		}
	}
}

void HandleChestDrops(int ctype) {
	HandleElixirDrop(true);
	if(ctype == DND_CHESTTYPE_BRONZE)
		SpawnOrbForAll(1);
	else if(ctype == DND_CHESTTYPE_SILVER)
		SpawnOrbForAll(2);
	else if(ctype == DND_CHESTTYPE_GOLD)
		SpawnOrbForAll(3);
}

void HandleOrbDrop() {
	int addchance = (Clamp_Between(GetCVar("dnd_orb_dropchanceadd"), 0, 100) << 16) / 100;
	for(int i = 0; i < MAXPLAYERS; ++i) {
		// run each player's chance, drop for corresponding player only
		if(PlayerInGame(i) && (GetCVar("dnd_ignore_dropweights") || (IsActorAlive(i + P_TIDSTART) && RunDefaultDropChance(i, 1, DND_ELITE_BASEDROP + addchance))))
			SpawnOrb(i,true);
	}
}

void HandleCharmLootDrop(bool isElite) {
	int addchance = 0;
	if(isElite)
		addchance = DND_ELITE_BASEDROP / 2;
	for(int i = 0; i < MAXPLAYERS; ++i) {
		// run each player's chance, drop for corresponding player only
		if(PlayerInGame(i) && (GetCVar("dnd_ignore_dropweights") || (IsActorAlive(i + P_TIDSTART) && RunDefaultDropChance(i, isElite, DND_BASE_CHARMRATE + addchance))))
			SpawnCharm(i, isElite);
	}
}

void HandleSoulDrop() {
	if(IsDemon()) {
		str drop = "Souls";
		if(IsBoss())
			drop = "LargeSouls";
		SpawnDrop(drop, GetActorProperty(0, APROP_HEIGHT) / 2, 12, 0, 0);
	}
}

void HandleOtherDrops(int target) {
	if(IsZombie())
		GiveActorInventory(target, "DnD_ShotUndead", 1);
	if(CheckInventory("MagicCausedDeath")) {
		// Soul Stealer case
		if(CheckActorInventory(target, "Ability_SoulStealer"))
			HandleSoulDrop();
		TakeInventory("MagicCausedDeath", 1);
	}
	else if(CheckInventory("BookofDeadCausedDeath")) {
		GiveActorInventory(target, "Souls", 1 + DND_BOSS_SOULGIVE * IsBoss());
		Spawn("SoulEffectSpawner", GetActorX(0), GetActorY(0), GetActorZ(0));
		TakeInventory("BookofDeadCausedDeath", 1);
	}
	
	if(CheckActorInventory(target, "Ability_HeartSeeker") && RunDefaultDropChance(target - P_TIDSTART, CheckInventory("MonsterIsElite"), CHANCE_HEART))
		SpawnDrop("DemonHeartPickup", 24.0, 16, 0, 0);
}

int GetPVelocity(void) {
	int vel;
	int x = GetActorVelX(0);
	int y = GetActorVelY(0);
	int angle = VectorAngle(x, y);
   
	if(((angle + 0.125) % 0.5) > 0.25)
		vel = FixedDiv(y, sin(angle));
	else
		vel = FixedDiv(x, cos(angle));
	return vel >> 16;
}

void HandleHealDependencyCheck() {
	// Research dependencies
	if(CheckInventory("Research_Body_Hp_1_Tracker") == GetAmmoCapacity("Research_Body_Hp_1_Tracker") && CheckResearchStatus(RES_BIO1) == RES_NA)
		GiveResearch(RES_BIO1, true);
	
	// Quest records
	if(active_quest_id == QUEST_HEALFOR500 && !IsQuestComplete(ActivatorTID(), active_quest_id) && CheckInventory("DnD_MasterHealerQuest_HealAmount") >= DND_QUEST_MASTERHEALER_REQ)
		CompleteQuest(ActivatorTID(), active_quest_id);
	
	// Skin O' My Teeth check
	if(active_quest_id == QUEST_NOHEALINGPICKUP)
		FailQuest(ActivatorTID());
}

int GetWeaponSlotFromFlag(int flags) {
	// the first 14 bits are unnecessary
	flags >>= DND_SHIFTBITS_FOR_SLOTFROMFLAG;
	for(int i = 0; i < 32; ++i)
		if(IsSet(flags, i))
			return i;
	return 0;
}

void ThunderstaffLightningWork(int target, int this, int dmg, str dmgtype, str toSpawn) {
	ACS_NamedExecuteAlways("DND ThunderStaff FX Spawn", 0, target);
	SpawnForced(toSpawn, GetActorX(target), GetActorY(target), GetActorFloorZ(target) + 16.0, DND_THUNDERSTAFF_DAMAGERTID);
	Thing_Damage2(target, dmg, dmgtype);
	SetActivator(DND_THUNDERSTAFF_DAMAGERTID);
	SetPointer(AAPTR_TARGET, this);
	SetActorProperty(DND_THUNDERSTAFF_DAMAGERTID, APROP_TARGETTID, this);
	Thing_ChangeTID(DND_THUNDERSTAFF_DAMAGERTID, 0);
	SetActivator(this);
}

int ScaleMonster(int pcount, int realhp) {
	int base = realhp;
	int add = 0, level = 1, low, high, temp;
	level = PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] / pcount;
	// ensure minions use master's level
	if(GetActorProperty(0, APROP_MASTERTID)) {
		level = MonsterProperties[GetActorProperty(0, APROP_MASTERTID) - DND_MONSTERTID_BEGIN].level;
	}
	if(GetCVar("dnd_randomize_levels")) {
		low = Clamp_Between(GetCVar("dnd_monsterlevel_low"), 0, 50);
		high = Clamp_Between(GetCVar("dnd_monsterlevel_high"), 0, 50);
		// give some sort of variety in the levels
		// subtract level from avg half the time
		if(random(0, 1)) {
			temp = low;
			low = -high;
			high = -temp;
		}
		level += random(low, high);
	}
	if(GetCVar("dnd_monsterlevel_behind"))
		level = Clamp_Between(level, 1, PlayerInformationInLevel[PLAYERLEVELINFO_LEVEL] / pcount);
	else
		level = Clamp_Between(level, 1, DND_MAX_MONSTERLVL);
	level = Clamp_Between(level, 1, GetCVar("dnd_maxmonsterlevel"));
	if(level > 1) {
		add = (base * Clamp_Between(GetCVar("dnd_monster_hpscalepercent"), 1, 100)) / 100;
		if(level > 75)
			temp = DND_AFTER75_INCREMENT;
		else if(level > 50)
			temp = DND_AFTER50_INCREMENT;
		else
			temp = 1;
		// we are in boost range for hp
		if(temp != 1) {
			if(add > 100) {
				add /= 100;
				add *= 100 + temp;
			}
			else {
				add *= 100 + temp;
				add /= 100;
			}
		}
		// add level factor to it
		// first overflow check
		if(add < (INT_MAX - base) / Max((level - 1),1)) {
			add *= level - 1;
			if(GetCVar("dnd_playercount_scales_monsters")) {
				if(add > 100) {
					add /= 100;
					add *= 100 + DND_MONSTERHP_PLAYERSCALE * (Clamp_Between(pcount - 1, 0, DND_MAX_PLAYERHPSCALE));
				}
				else {
					add *= 100 + DND_MONSTERHP_PLAYERSCALE * (Clamp_Between(pcount - 1, 0, DND_MAX_PLAYERHPSCALE));
					add /= 100;
				}
			}
		}
		else
			add = INT_MAX - base;
	}
	MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].basehp = base;
	MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].maxhp = base + add;
	MonsterProperties[ActivatorTID() - DND_MONSTERTID_BEGIN].level = level;
	return base + add;
}

void ActivateKillingSpree() {
	// if a kill streak wasn't running, run it now
	if(!CheckInventory("DnD_SpreeCountdownRunning")) {
		GiveInventory("DnD_SpreeCountdownRunning", 1);
		ACS_NamedExecuteAlways("DnD Killing Spree Timer", 0);
	}
	// spree count was given, so a possible spree may be in reach
	if(CheckInventory("DnD_SpreeTimer")) {
		GiveInventory("DnD_MultikillCounter", 1);
		if(GetArmorType("RavagerArmor", PlayerNumber())) {
			GiveInventory("RavagerPower", 1);
			GiveInventory("RavagerPowerDefense", 1);
		}
		
		// punisher perk -- be on cruel or more
		if(CheckInventory("Punisher_Perk5") && (CheckInventory("DnD_MultikillCounter") + 1) / DND_SPREE_PER >= 1)
			GiveInventory("Punisher_Perk5_MoveSpeed", 1);
	}
	// give spree counter
	GiveInventory("DnD_SpreeTimer", DND_SPREE_AMOUNT);
}

void ResetPlayerScriptChecks() {
	for(int i = 0; i < MAX_SCRIPT_TRACK; ++i)
		for(int j = 0; j < MAXPLAYERS; ++j)
			PlayerScriptsCheck[i][j] = false;
}

// check if we reached a multiple of 50 yet
void CheckPunisherKillTally() {
	int count = CheckInventory("Punisher_Perk50_Counter");
	if(count > DND_PUNISHER_PERK3_KILLCOUNT) {
		// take all previous
		for(int i = 0; i < DND_PUNISHER_PERK3_MAX; ++i)
			TakeInventory(StrParam(s:"Punisher_Perk50_Damage_", d:(i + 1)), 1);
		// give new based on total kill
		GiveInventory(StrParam(s:"Punisher_Perk50_Damage_", d:CheckInventory("DnD_Kills") / 50), 1);
		// clean current count
		SetInventory("Punisher_Perk50_Counter", count % DND_PUNISHER_PERK3_KILLCOUNT);
	}
}

void HandleMonsterTemporaryWeaponDrop(int id, int pnum, bool isElite) {
	id = MonsterProperties[id].id;
	switch(id) {
		case MONSTER_BLOODFIEND:
		case MONSTER_RAVAGER:
		case MONSTER_LURKER:
			if(RunDefaultDropChance(pnum, isElite, TEMPWEP_BLOODFIENDSPINE_DROPCHANCE))
				SpawnDrop(TemporaryWeapons[DND_TEMPWEP_BLOODFIENDSPINE][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_VULGAR:
			if(RunDefaultDropChance(pnum, isElite, TEMPWEP_VENOM_DROPCHANCE))
				SpawnDrop(TemporaryWeapons[DND_TEMPWEP_VENOM][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_CHAINGUNGENERAL:
			if(RunDefaultDropChance(pnum, isElite, TEMPWEP_NAILGUN_DROPCHANCE))
				SpawnDrop(TemporaryWeapons[DND_TEMPWEP_HEAVYNAILGUN][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_DEATHKNIGHT:
		case MONSTER_HORSHACKER:
			if(RunDefaultDropChance(pnum, isElite, TEMPWEP_SOULRENDER_DROPCHANCE))
				SpawnDrop(TemporaryWeapons[DND_TEMPWEP_SOULRENDER][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_CORPULENT:
			if(RunDefaultDropChance(pnum, isElite, TEMPWEP_HFCANNON_DROPCHANCE))
				SpawnDrop(TemporaryWeapons[DND_TEMPWEP_HELLFORGECANNON][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		case MONSTER_DARKSERVANT:
			if(RunDefaultDropChance(pnum, isElite, TEMPWEP_DARKGLOVES_DROPCHANCE))
				SpawnDrop(TemporaryWeapons[DND_TEMPWEP_DARKGLOVES][TEMPWEP_DROP], 24.0, 16, 0, 0);
		break;
		default:
		break;
	}
}