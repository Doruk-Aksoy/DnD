#ifndef DND_ULTIMATUM_IN
#define DND_ULTIMATUM_IN

namespace ultimatum {
    enum {
        MAX_ULTIMATUM_WAVES = 10,
        MAX_ULTIMATUM_WAVE_FORMATIONS = 16,
        MAX_ULTIMATUM_WAVE_VARIANTS = 4,

        TP_TID_GROUND_BEGIN = 4005,
        TP_TID_GROUND_END = 4010,
        TP_SECTOR_GROUND = 22,

        TP_TID_SIDEROOF_BEGIN = 4011,
        TP_TID_SIDEROOF_END = 4016,
        TP_SECTOR_SIDEROOF = 8,

        TP_TID_FLIERS_BEGIN = 4017,
        TP_TID_FLIERS_END = 4040,
        TP_SECTOR_FLIERS = 6,

        TP_TID_GROUNDBOSS = 4046,
        TP_SECTOR_GROUNDBOSS = 26,

        TP_TID_BACKBOSS_BEGIN = 4000,
        TP_TID_BACKBOSS_END = 4004,
        TP_SECTOR_BACKBOSS = 27,

        TP_TID_WINDOWS_BEGIN = 4041,
        TP_TID_WINDOWS_END = 4044,
        TP_SECTOR_WINDOWS = 28,

        TP_TID_MIDSECTION = 4045,                // used for spawning rewards, the NPC etc.

        ULTIMATUM_RATE_MULT = 0.1,
        ULTIMATUM_WAKECHANCE = 0.6,

        TP_TID_SAWBLADE_BEGIN = 6000,
        TP_TID_SAWBLADE_END = 6008,
        TP_SAWBLADE_COUNT = TP_TID_SAWBLADE_END - TP_TID_SAWBLADE_BEGIN + 1,

        TP_TID_MIASMA = 6009,

        TP_TID_STORMCALL_BEGIN = 6010,
        TP_TID_STORMCALL_END = 6030,
        TP_STORMCALL_COUNT = TP_TID_STORMCALL_END - TP_TID_STORMCALL_BEGIN + 1,

        TP_TID_PUSTULE_BEGIN = 6031,
        TP_TID_PUSTULE_END = 6062,
        TP_PUSTULE_COUNT = TP_TID_PUSTULE_END - TP_TID_PUSTULE_BEGIN + 1,

        TP_TID_RAGINGDEAD_BEGIN = 6063,
        TP_TID_RAGINGDEAD_END = 6078,
        TP_RAGINGDEAD_COUNT = TP_TID_RAGINGDEAD_END - TP_TID_RAGINGDEAD_BEGIN + 1,

    };

    enum {
        WAVE_FORMATION_GROUND = 1,
        WAVE_FORMATION_FLIER = 2,
        WAVE_FORMATION_SIDELINE_WINDOWS = 4,
        WAVE_FORMATION_SIDELINE_ROOFS = 8,
        WAVE_FORMATION_BOSS_BACKLINE = 16,
        WAVE_FORMATION_BOSS_GROUND = 32,
    };

    enum {
        ULTIMATUM_OPTION_EXTRA_ELE_DAMAGE,
        ULTIMATUM_OPTION_SAWBLADES,
        ULTIMATUM_OPTION_MIASMA,
        ULTIMATUM_OPTION_CRACKLINGPAIN,
        ULTIMATUM_OPTION_STORMCALL,
        ULTIMATUM_OPTION_FIRESKULLS,
        ULTIMATUM_OPTION_BLISTERINGCOLD,
        ULTIMATUM_OPTION_HINDERINGFLASKS,
        ULTIMATUM_OPTION_DROUGHT,
        ULTIMATUM_OPTION_ESCALATINGFRAGILITY,
        ULTIMATUM_OPTION_OCCASIONALIMPOTENCE,
        ULTIMATUM_OPTION_OVERWHELMINGMONSTERS,
        ULTIMATUM_OPTION_PRECISEMONSTERS,
        ULTIMATUM_OPTION_FEEBLEREACH,
        ULTIMATUM_OPTION_SIPHONINGMONSTERS,
        ULTIMATUM_OPTION_DIMINISHEDLETHALITY,
        ULTIMATUM_OPTION_UNSTOPPABLETIDE,
        ULTIMATUM_OPTION_FLEETINGLIFE,
        ULTIMATUM_OPTION_GLUTTONOUSTIDE,
        ULTIMATUM_OPTION_RAPIDEXHAUSTION,
        ULTIMATUM_OPTION_TASTETHEPAIN,

        ULTIMATUM_OPTION_COUNT
    };
};

// One reward is rolled per offer. Accepting a challenge banks it; declining drops what is already
// banked and forfeits the pending one. Slots 0..MAX_ULTIMATUM_WAVES-1 are the bank and the slot
// after them holds the pending offer -- so the two are told apart from item data alone and a client
// needs no count synced to it.
#define ULTIMATUM_REWARD_PENDING ultimatum::MAX_ULTIMATUM_WAVES
#define MAX_ULTIMATUM_REWARD_SLOTS (ULTIMATUM_REWARD_PENDING + 1)

typedef struct {
    str actor_name;             // actor to spawn
    int amount;                 // amt to spawn
    int spawn_delay;            // delay between spawns
    int formation_type;         // which teleport group to use
} ultimatum_formation_T;

typedef struct {
    ultimatum_formation_T info[ultimatum::MAX_ULTIMATUM_WAVE_VARIANTS][ultimatum::MAX_ULTIMATUM_WAVE_FORMATIONS];
    int formation_count[ultimatum::MAX_ULTIMATUM_WAVE_VARIANTS];
    int variants;
    int formation_types_used;  // the formation types featured in the wave
    int total_monsters[ultimatum::MAX_ULTIMATUM_WAVE_VARIANTS];
} ultimatum_wave_T;

typedef struct {
    int kills;
    int total_kills;
    int req_kills_wave;
    int curr_wave;
    int is_wave_complete;
} ultimatum_curr_tally_T;

void AddFormationToWave(ultimatum_wave_T module& w, int variant, str actor, int amt, int del, int type) {    
    int count = w.formation_count[variant];
    w.info[variant][count].actor_name = actor;
    w.info[variant][count].amount = amt;
    w.info[variant][count].spawn_delay = (del * ultimatum::ULTIMATUM_RATE_MULT) >> 16;
    w.info[variant][count].formation_type = type;

    w.formation_types_used |= type;

    // only count those coming from the 1st variant, all variants MUST provide the same amount of actor types
    ++w.formation_count[variant];
    w.total_monsters[variant] += amt;
}

// A random player still taking part. alive_only also rules out anyone waiting to respawn: someone
// out of lives is never coming back, and someone who is, is no use to a caller that needs a body
// right now. -1 when the arena has nobody left to pick.
int PickRandomUltimatumPlayer(bool alive_only) {
    int pool[MAXPLAYERS];
    int n = 0;

    for(int i = 0; i < MAXPLAYERS; ++i) {
        if(IsActivePlayer(i) && (!alive_only || IsActorAlive(i + P_TIDSTART)))
            pool[n++] = i;
    }

    if(!n)
        return -1;

    return pool[random(0, n - 1)];
}

// Hands the waker to one of them, which is what pulls a freshly spawned formation onto a player.
// It alerts monsters through the carrier, so it needs a live body -- a corpse wakes nothing, and a
// spectator or a player out of lives has none to give.
void GiveUltimatumPlayerMonsterWaker() {
    int pnum = PickRandomUltimatumPlayer(true);
    if(pnum != -1)
        GiveActorInventory(pnum + P_TIDSTART, "MonsterWaker", 1);
}

int GetUltimatumSpawnSpotTID(int type) {
    using ultimatum;

    switch(type) {
        case WAVE_FORMATION_GROUND:
        return random(TP_TID_GROUND_BEGIN, TP_TID_GROUND_END);

        case WAVE_FORMATION_FLIER:
        return random(TP_TID_FLIERS_BEGIN, TP_TID_FLIERS_END);

        case WAVE_FORMATION_SIDELINE_WINDOWS:
        return random(TP_TID_WINDOWS_BEGIN, TP_TID_WINDOWS_END);

        case WAVE_FORMATION_SIDELINE_ROOFS:
        return random(TP_TID_SIDEROOF_BEGIN, TP_TID_SIDEROOF_END);

        case WAVE_FORMATION_BOSS_BACKLINE:
        return random(TP_TID_BACKBOSS_BEGIN, TP_TID_BACKBOSS_END);

        case WAVE_FORMATION_BOSS_GROUND:
        return TP_TID_GROUNDBOSS;
    }

    return TP_TID_GROUND_BEGIN;
}

ultimatum_wave_T module& GetUltimatumWaveInfo(int id) {
    using ultimatum;

    static bool init = false;
    static ultimatum_wave_T waves[MAX_ULTIMATUM_WAVES];

    if(!init) {
        init = true;

        // variant 1-N, waves 1-10
        auto curr_wave = waves[0];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "Sabreclaw", 32, TICRATE, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "EarthGolem", 20, TICRATE, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Devil2", 24, TICRATE, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "AxeKnight", 16, TICRATE * 3, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 1, "Nhumcign", 32, TICRATE, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Shadow", 32, TICRATE, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Lurker", 20, TICRATE, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "AxeKnight", 16, TICRATE * 3, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 2, "SoulEater", 32, TICRATE, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Roach", 32, TICRATE, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Devourer", 20, TICRATE, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "AxeKnight", 16, TICRATE * 3, WAVE_FORMATION_SIDELINE_ROOFS);

        curr_wave = waves[1];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "MoonSatyr", 12, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "BloodSatyr", 20, TICRATE * 2, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "HellsFury", 16, TICRATE * 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Shadow", 32, TICRATE * 4, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 1, "IceGolem", 14, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "ShadowBeast", 16, TICRATE * 2, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "ChaosSerpent", 16, TICRATE * 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "AxeKnight", 16, TICRATE * 4, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 2, "SludgeGiant", 20, TICRATE * 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Yeti", 12, TICRATE * 5, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "AxeKnight", 24, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "ChaosSerpent", 16, TICRATE * 5, WAVE_FORMATION_SIDELINE_ROOFS);
    
        curr_wave = waves[2];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "ShadowBeast", 16, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Nhumcign", 30, TICRATE * 2, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "BloodGolem", 6, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Wicked", 20, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "ChaosSerpent", 12, TICRATE * 5, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 1, "IceGolem", 20, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Lurker", 24, TICRATE * 2, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "BloodGolem", 6, TICRATE * 7, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Cacolich", 16, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "AxeKnight", 16, TICRATE * 5, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 2, "SludgeGiant", 20, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "BloodSatyr", 16, TICRATE * 2, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "BloodGolem", 6, TICRATE * 7, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "EarthLich", 16, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "Draugr", 16, TICRATE * 5, WAVE_FORMATION_SIDELINE_ROOFS);

        curr_wave = waves[3];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "BlackKnight", 32, TICRATE * 5, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "DreadKnight", 12, TICRATE * 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "BloodGolem", 12, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Wicked", 20, TICRATE * 4, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "Draugr", 12, TICRATE * 5, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 0, "Corpulent", 6, TICRATE * 4, WAVE_FORMATION_SIDELINE_WINDOWS);

        AddFormationToWave(curr_wave, 1, "ChaosSerpent", 24, TICRATE * 5, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "MagmaSerpent", 18, TICRATE * 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "BloodGolem", 12, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Cacolich", 16, TICRATE * 4, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "AxeKnight", 16, TICRATE * 5, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 1, "Vore", 6, TICRATE * 4, WAVE_FORMATION_SIDELINE_WINDOWS);
    
        AddFormationToWave(curr_wave, 2, "SkullWizard", 36, TICRATE * 5, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "MagmaGolem", 12, TICRATE * 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "BloodGolem", 12, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "EarthLich", 16, TICRATE * 4, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "DarkServant", 16, TICRATE * 5, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 2, "Gamon", 6, TICRATE * 4, WAVE_FORMATION_SIDELINE_WINDOWS);
   
        curr_wave = waves[4];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "DarkServant", 20, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "DarkServant", 12, TICRATE * 6, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "DarkZealot", 12, TICRATE * 9, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Wicked", 24, TICRATE * 4, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "HellArbiter", 10, TICRATE * 9, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "Draugr", 12, TICRATE * 9, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 0, "Corpulent", 9, TICRATE * 3, WAVE_FORMATION_SIDELINE_WINDOWS);

        AddFormationToWave(curr_wave, 1, "HellWarrior", 18, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Kjaroch", 10, TICRATE * 6, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "FleshWizard", 12, TICRATE * 9, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "EarthLich", 20, TICRATE * 4, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "BloodLich", 8, TICRATE * 9, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "Crawler", 16, TICRATE * 9, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 1, "Vore", 9, TICRATE * 4, WAVE_FORMATION_SIDELINE_WINDOWS);

        AddFormationToWave(curr_wave, 2, "IceGolem", 16, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Lavademon", 10, TICRATE * 7, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Hierophant", 12, TICRATE * 9, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "ShadowPriest", 20, TICRATE * 7, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "DESentinel", 8, TICRATE * 8, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "AxeKnight", 16, TICRATE * 9, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 2, "HellforgeSpider", 9, TICRATE * 4, WAVE_FORMATION_SIDELINE_WINDOWS);
    
        curr_wave = waves[5];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "GoldGolem", 16, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "BloodGolem", 12, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "IceGolem", 12, TICRATE * 9, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "ShadowPriest", 20, TICRATE * 5, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "ShadowDisciple", 8, TICRATE * 12, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "AvatarOfChaos", 1, TICRATE * 4, WAVE_FORMATION_BOSS_GROUND);
    
        AddFormationToWave(curr_wave, 1, "Gamon", 16, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "MoonSatyr", 12, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "BloodSatyr", 12, TICRATE * 9, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Inferno", 24, TICRATE * 6, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "Helemental", 10, TICRATE * 12, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "HellsmithX", 1, TICRATE * 4, WAVE_FORMATION_BOSS_GROUND);

        AddFormationToWave(curr_wave, 2, "BlackKnight", 16, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "DreadKnight", 12, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "DeathKnight", 4, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Wicked", 20, TICRATE * 6, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "Defiler", 8, TICRATE * 12, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "PsionicQueen", 1, TICRATE * 4, WAVE_FORMATION_BOSS_GROUND);

        curr_wave = waves[6];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "Nhumcign", 32, TICRATE * 2, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "ArchVile2", 4, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Inferno", 20, TICRATE * 4, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "Draugr", 16, TICRATE * 5, WAVE_FORMATION_SIDELINE_WINDOWS);
        AddFormationToWave(curr_wave, 0, "Guru", 12, TICRATE * 8, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 0, "HellsmithX", 3, TICRATE * 24, WAVE_FORMATION_BOSS_GROUND);

        AddFormationToWave(curr_wave, 1, "Lurker", 32, TICRATE * 2, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "ArchVile2", 4, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "ShadowPriest", 20, TICRATE * 4, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "AxeKnight", 16, TICRATE * 5, WAVE_FORMATION_SIDELINE_WINDOWS);
        AddFormationToWave(curr_wave, 1, "Horshacker", 12, TICRATE * 8, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 1, "AvatarOfChaos", 3, TICRATE * 24, WAVE_FORMATION_BOSS_GROUND);

        AddFormationToWave(curr_wave, 2, "Gravedigger", 32, TICRATE * 2, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "ArchVile2", 4, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "EarthLich", 20, TICRATE * 4, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "Yeti", 16, TICRATE * 5, WAVE_FORMATION_SIDELINE_WINDOWS);
        AddFormationToWave(curr_wave, 2, "Hierophant", 12, TICRATE * 8, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 2, "DarkLich", 3, TICRATE * 24, WAVE_FORMATION_BOSS_GROUND);

        curr_wave = waves[7];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "BloodGolem", 16, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "BloodSeeker", 6, TICRATE * 8, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 0, "Corpulent", 8, TICRATE * 5, WAVE_FORMATION_SIDELINE_WINDOWS);
        AddFormationToWave(curr_wave, 0, "BloodLich", 10, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "GoldLich", 3, TICRATE * 12, WAVE_FORMATION_BOSS_GROUND);
        AddFormationToWave(curr_wave, 0, "DarkLich", 2, TICRATE * 6, WAVE_FORMATION_BOSS_BACKLINE);

        AddFormationToWave(curr_wave, 1, "BloodGolem", 16, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Lavademon", 6, TICRATE * 8, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 1, "Vore", 8, TICRATE * 5, WAVE_FORMATION_SIDELINE_WINDOWS);
        AddFormationToWave(curr_wave, 1, "DESentinel", 10, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "DarkLich", 3, TICRATE * 12, WAVE_FORMATION_BOSS_GROUND);
        AddFormationToWave(curr_wave, 1, "PsionicQueen", 2, TICRATE * 6, WAVE_FORMATION_BOSS_BACKLINE);

        AddFormationToWave(curr_wave, 2, "BloodGolem", 16, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Lavademon", 6, TICRATE * 8, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 2, "HellforgeSpider", 8, TICRATE * 5, WAVE_FORMATION_SIDELINE_WINDOWS);
        AddFormationToWave(curr_wave, 2, "HellArbiter", 10, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "GoldLich", 3, TICRATE * 12, WAVE_FORMATION_BOSS_GROUND);
        AddFormationToWave(curr_wave, 2, "IronLichBoss", 2, TICRATE * 6, WAVE_FORMATION_BOSS_BACKLINE);

        curr_wave = waves[8];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "MagmaSerpent", 32, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "ShadowBeast", 20, TICRATE * 2, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 0, "Draugr", 16, TICRATE * 5, WAVE_FORMATION_SIDELINE_WINDOWS);
        AddFormationToWave(curr_wave, 0, "Wicked", 12, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "ShadowPriest", 12, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "AvatarOfChaos", 1, TICRATE * 12, WAVE_FORMATION_BOSS_GROUND);
        AddFormationToWave(curr_wave, 0, "IronLichBoss", 1, TICRATE * 8, WAVE_FORMATION_BOSS_BACKLINE);
        AddFormationToWave(curr_wave, 0, "GoldLich", 1, TICRATE * 8, WAVE_FORMATION_BOSS_BACKLINE);

        AddFormationToWave(curr_wave, 1, "ChaosSerpent", 32, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Bloodseeker", 20, TICRATE * 2, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 1, "AxeKnight", 16, TICRATE * 5, WAVE_FORMATION_SIDELINE_WINDOWS);
        AddFormationToWave(curr_wave, 1, "Inferno", 12, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "EarthLich", 12, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "HellsmithX", 1, TICRATE * 12, WAVE_FORMATION_BOSS_GROUND);
        AddFormationToWave(curr_wave, 1, "DarkLich", 1, TICRATE * 8, WAVE_FORMATION_BOSS_BACKLINE);
        AddFormationToWave(curr_wave, 1, "IronLichBoss", 1, TICRATE * 8, WAVE_FORMATION_BOSS_BACKLINE);
    
        AddFormationToWave(curr_wave, 2, "BlackKnight", 32, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Kjaroch", 20, TICRATE * 2, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 2, "Yeti", 16, TICRATE * 5, WAVE_FORMATION_SIDELINE_WINDOWS);
        AddFormationToWave(curr_wave, 2, "CacoLich", 12, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "DeathWhisperer", 12, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "Cerberus", 1, TICRATE * 12, WAVE_FORMATION_BOSS_GROUND);
        AddFormationToWave(curr_wave, 2, "DarkLich", 1, TICRATE * 8, WAVE_FORMATION_BOSS_BACKLINE);
        AddFormationToWave(curr_wave, 2, "GoldLich", 1, TICRATE * 8, WAVE_FORMATION_BOSS_BACKLINE);

        // final wave
        curr_wave = waves[9];
        curr_wave.variants = 2;
        AddFormationToWave(curr_wave, 0, "Lavademon", 16, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "BloodGolem", 12, TICRATE * 2, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "DeathVile", 3, TICRATE * 16, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "FleshWizard", 1, TICRATE * 16, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "DarkZealot", 18, TICRATE * 3, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 0, "Ophidian", 16, TICRATE * 3, WAVE_FORMATION_SIDELINE_WINDOWS);
        AddFormationToWave(curr_wave, 0, "DESentinel", 12, TICRATE * 6, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "EarthLich", 12, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "BloodLich", 4, TICRATE * 12, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "AvatarOfChaos", 2, TICRATE * 16, WAVE_FORMATION_BOSS_GROUND);
        AddFormationToWave(curr_wave, 0, "HellsmithX", 1, TICRATE * 16, WAVE_FORMATION_BOSS_GROUND);
        AddFormationToWave(curr_wave, 0, "IronLichBoss", 3, TICRATE * 10, WAVE_FORMATION_BOSS_BACKLINE);

        AddFormationToWave(curr_wave, 1, "Bloodseeker", 16, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "BloodGolem", 12, TICRATE * 2, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "DeathVile", 3, TICRATE * 16, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "FleshWizard", 1, TICRATE * 16, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "DarkZealot", 18, TICRATE * 3, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 1, "Ophidian", 16, TICRATE * 3, WAVE_FORMATION_SIDELINE_WINDOWS);
        AddFormationToWave(curr_wave, 1, "HellArbiter", 12, TICRATE * 6, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "Inferno", 12, TICRATE * 3, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "DESentinel", 4, TICRATE * 12, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "IronLichBoss", 2, TICRATE * 16, WAVE_FORMATION_BOSS_GROUND);
        AddFormationToWave(curr_wave, 1, "DarkLich", 1, TICRATE * 16, WAVE_FORMATION_BOSS_GROUND);
        AddFormationToWave(curr_wave, 1, "AvatarOfChaos", 3, TICRATE * 10, WAVE_FORMATION_BOSS_BACKLINE);
    }

    return waves[id];
}

ultimatum_curr_tally_T module& GetCurrentUltimatumTally() {
    static ultimatum_curr_tally_T tally;
    return tally;
}

void IncrementCurrentUltimatumTally() {
    auto curr_tally = GetCurrentUltimatumTally();
    ++curr_tally.kills;
    ++curr_tally.total_kills;

    // next wave can proceed + present challenge
    if(curr_tally.kills >= curr_tally.req_kills_wave) {
        ACS_NamedExecuteAlways("DnD Ultimatum Wave Completed", 0);
    }
}

// call this script in OPEN after some tic delay, like 35 * 5 for the ResetPlayerLevelInfo to kick in
Script "DnD Init Ultimatum" (void) {
    // just for it to be filled out
    GetUltimatumWaveInfo(0);
    InformationInLevel[LEVELINFO_ISULTIMATUM] = 1;

    AcceptMapEvent(DND_MAPEVENT_NOINFIGHTING);

    // global 20 outlives the map, so last run's bank has to go -- but only once its array exists
    while(!IsSetupComplete(SETUP_STATE1, SETUP_ITEMTABLES))
        Delay(const:10);
    ClearUltimatumRewards();
}

Script "DnD Init Ultimatum - CS" (void) CLIENTSIDE {
    InformationInLevel[LEVELINFO_ISULTIMATUM] = 1;
}

// The map side of an option: the actors it puts in the arena.
//
// Called at the START of every wave for every option taken so far, NOT once when the option
// is accepted. A tier is permanent but the actors are not -- every spawner runs for one wave,
// cleans up after itself and terminates, so a challenge picked in round 2 vanished for round 3
// and never came back.
void StartUltimatumMapHazard(int option, int tier) {
    using ultimatum;

    switch(option) {
        case ULTIMATUM_OPTION_SAWBLADES:
            // tier raises how many blades fly and how fast
            ACS_NamedExecuteAlways("DnD Ultimatum Saw Spawn", 0, tier);
        break;

        case ULTIMATUM_OPTION_MIASMA:
            // The tier is already 1 based, so the actor suffix IS the tier -- a +1 here starts
            // at Tier2 and asks for a Tier5 that does not exist at the cap.
            SpawnSpot(StrParam(s:"DnD_Ultimatum_Miasma_Tier", d:tier), TP_TID_MIASMA);
        break;

        case ULTIMATUM_OPTION_STORMCALL:
            // tier raises how many spots get marked and how fast they strike
            ACS_NamedExecuteAlways("DnD Ultimatum Storm Call Spawn", 0, tier);
        break;

        case ULTIMATUM_OPTION_FIRESKULLS:
            // tier raises how many spirits chase and how much fire each throws
            ACS_NamedExecuteAlways("DnD Ultimatum Raging Dead Spawn", 0, tier);
        break;

        case ULTIMATUM_OPTION_BLISTERINGCOLD:
            // tier raises how many pustules appear, how hard they burst and how fast
            ACS_NamedExecuteAlways("DnD Ultimatum Pustule Spawn", 0, tier);
        break;
    }
}

// Every option taken so far, placed again for the wave that is starting. Options with no actors
// fall straight through the switch -- they are read from their accessors on every hit instead.
void StartUltimatumMapHazards() {
    using ultimatum;

    for(int i = ULTIMATUM_OPTION_EXTRA_ELE_DAMAGE; i < ULTIMATUM_OPTION_COUNT; ++i) {
        int tier = GetUltimatumOptionTier(i);
        if(tier)
            StartUltimatumMapHazard(i, tier);
    }
}

Script "DnD Start Ultimatum Wave" (int wave, int del) {
    if(del)
        Delay(del);

    auto w = GetUltimatumWaveInfo(wave);
    int var = random(0, w.variants - 1);

    Delay(const:1);

    using ultimatum;

    auto curr_tally = GetCurrentUltimatumTally();
    curr_tally.is_wave_complete = 0;
    curr_tally.curr_wave = wave;
    curr_tally.kills = 0;
    curr_tally.req_kills_wave = w.total_monsters[var];

    // After is_wave_complete is cleared, not before: every spawner polls it and would shut
    // itself down on its first tic otherwise.
    StartUltimatumMapHazards();

    if(w.formation_types_used & WAVE_FORMATION_GROUND) {
        if(GetSectorFloorZ(21, 0, 0) != -280.0) {
            Floor_RaiseByValue(21, 8, 8);
            Floor_RaiseByValue(22, 8, 8);
        }

        ChangeFloor(21, "HTX_1549");
        ChangeFloor(22, "HTX_1556");
    }
    else {
        if(GetSectorFloorZ(21, 0, 0) != -288.0) {
            Floor_LowerByValue(21, 8, 8);
            Floor_LowerByValue(22, 8, 8);
        }

        ChangeFloor(21, "HNX_838");
        ChangeFloor(22, "HNX_838");
    }

    if(w.formation_types_used & WAVE_FORMATION_SIDELINE_WINDOWS)
        Ceiling_RaiseToNearest(4, 32);
    else
        Ceiling_LowerToFloor(4, 32);

    if(w.formation_types_used & WAVE_FORMATION_BOSS_BACKLINE)
        Ceiling_RaiseToNearest(7, 32);
    else
        Ceiling_LowerToFloor(7, 32);

    if(w.formation_types_used & WAVE_FORMATION_BOSS_GROUND) {
        if(GetSectorFloorZ(26, 0, 0) != -280.0) {
            Floor_RaiseByValue(29, 8, 8);
            Floor_RaiseByValue(26, 8, 8);
        }

        ChangeFloor(29, "HTX_1549");
        ChangeFloor(26, "HNX_878");
    }
    else {
        if(GetSectorFloorZ(29, 0, 0) != -288.0) {
            Floor_LowerToLowest(29, 32);
            Delay(const:1);
            Floor_LowerToLowest(26, 32);
        }

        ChangeFloor(21, "HNX_838");
        ChangeFloor(26, "HNX_838");
    }

    // start dispatching the variants
    for(int i = 0; i < w.formation_count[var]; ++i) {
        ACS_NamedExecuteAlways("DnD Ultimatum Wave Dispatch", 0, wave, var, i);
    }
}

Script "DnD Ultimatum Wave Dispatch" (int wave, int variant, int formation_id) {
    auto w = GetUltimatumWaveInfo(wave);
    auto info = w.info[variant][formation_id];

    using ultimatum;

    for(int i = 0; i < info.amount; ++i) {
        bool can_spawn = false;
        do {
            int spot_tid = GetUltimatumSpawnSpotTID(info.formation_type);
            can_spawn = SpawnSpotFacing(info.actor_name, spot_tid);
            if(!can_spawn)
                Delay(const:TICRATE / 3);
        } while(!can_spawn);
        SpawnSpot("TeleportFog", spot_tid);
        if(random(0, 1.0) <= ULTIMATUM_WAKECHANCE)
            GiveUltimatumPlayerMonsterWaker();
        Delay(info.spawn_delay);
    }
}

Script "DnD Ultimatum Wave Completed" (void) {
    using ultimatum;

    auto curr_tally = GetCurrentUltimatumTally();
    curr_tally.is_wave_complete = 1;

    // last wave has nothing to bargain over
    if(curr_tally.curr_wave == MAX_ULTIMATUM_WAVES - 1) {
        ACS_NamedExecuteAlways("DnD Handle Ultimatum Finish", 0);
        Terminate;
    }

    // we have so many options it can't max out
    int offer = PickUltimatumChallengeOffer();

    // what this round is worth, shown alongside the offer
    RollUltimatumReward(ULTIMATUM_REWARD_PENDING);

    NPC_States[DND_NPC_DARKWANDERER].aux_data = 0;
    NPC_States[DND_NPC_DARKWANDERER].dialog = random(DW_ULTIMATUM_ASK1, DW_ULTIMATUM_ASK3);
    NPC_States[DND_NPC_DARKWANDERER].offer = offer;
    NPC_States[DND_NPC_DARKWANDERER].chosen_option = -1;

    // The wanderer is the SAME npc every round, so the vote has to go back to untouched. Everywhere
    // else he asks once a map and a concluded state is meant to stick: the prompt reads it as
    // "already answered" and draws the accept recap instead of the offer, and Vote Register refuses
    // to count anything. interaction_count is reset for the same reason -- it only ever returns to
    // zero when a prompt is closed by hand, so across rounds it drifts and never releases the
    // monster timer.
    NPC_States[DND_NPC_DARKWANDERER].n_state = NPC_STATE_IDLE;
    NPC_States[DND_NPC_DARKWANDERER].voting = false;
    NPC_States[DND_NPC_DARKWANDERER].vote_count = 0;
    NPC_States[DND_NPC_DARKWANDERER].interaction_count = 0;

    for(int i = 0; i < MAXPLAYERS; ++i) {
        NPC_States[DND_NPC_DARKWANDERER].voters[i] = 0;
        NPC_States[DND_NPC_DARKWANDERER].voter_choice[i] = -1;
    }
    ACS_NamedExecuteWithResult("DnD NPC Vote Sync", -1, -1, DND_NPC_DARKWANDERER);

    // Pushed whole, here, rather than on each bump: this is the only moment a client can read the
    // table, and doing it here also catches one that joined part way through the run.
    for(i = ULTIMATUM_OPTION_EXTRA_ELE_DAMAGE; i < ULTIMATUM_OPTION_COUNT; ++i)
        ACS_NamedExecuteAlways("DnD Ultimatum Tier Sync", 0, i, GetUltimatumOptionTier(i));

    // Same for the items themselves. Paced the way "DnD Merchant Items" paces its own item syncs --
    // one SyncItemData is around 25 script sends, so the whole pool in one tic is a burst worth
    // spreading.
    for(i = 0; i < MAX_ULTIMATUM_REWARD_SLOTS; ++i) {
        SyncUltimatumRewardSlot(i);
        Delay(const:1);
    }

    ACS_NamedExecuteWithResult("DnD Try Spawn Area", TP_TID_MIDSECTION, "DarkWanderer", DND_NPC_TID, 96 | (4 << 16) | (22 << 24));
}

Script "DnD Is Ultimatum Complete" (void) {
    auto curr_tally = GetCurrentUltimatumTally();
    SetResultValue(curr_tally.is_wave_complete);
}

Script "DnD Handle Ultimatum Finish" (void) {
    // reached on a decline and on the last wave alike, so the bank pays out from one place
    DropUltimatumRewards();

    // decide whether boss can appear or we concluded
}

// max tier per option -- one number each
int GetUltimatumOptionMaxTier(int opt) {
    using ultimatum;

    static int max_tiers[ULTIMATUM_OPTION_COUNT] = {
        1,      // ULTIMATUM_OPTION_EXTRA_ELE_DAMAGE
        4,      // ULTIMATUM_OPTION_SAWBLADES
        4,      // ULTIMATUM_OPTION_MIASMA
        1,      // ULTIMATUM_OPTION_CRACKLINGPAIN
        4,      // ULTIMATUM_OPTION_STORMCALL
        4,      // ULTIMATUM_OPTION_FIRESKULLS
        4,      // ULTIMATUM_OPTION_BLISTERINGCOLD
        1,      // ULTIMATUM_OPTION_HINDERINGFLASKS
        1,      // ULTIMATUM_OPTION_DROUGHT
        1,      // ULTIMATUM_OPTION_ESCALATINGFRAGILITY
        1,      // ULTIMATUM_OPTION_OCCASIONALIMPOTENCE
        1,      // ULTIMATUM_OPTION_OVERWHELMINGMONSTERS
        1,      // ULTIMATUM_OPTION_PRECISEMONSTERS
        1,      // ULTIMATUM_OPTION_FEEBLEREACH
        1,      // ULTIMATUM_OPTION_SIPHONINGMONSTERS
        1,      // ULTIMATUM_OPTION_DIMINISHEDLETHALITY
        1,      // ULTIMATUM_OPTION_UNSTOPPABLETIDE
        1,      // ULTIMATUM_OPTION_FLEETINGLIFE
        1,      // ULTIMATUM_OPTION_GLUTTONOUSTIDE
        1,      // ULTIMATUM_OPTION_RAPIDEXHAUSTION
        1       // ULTIMATUM_OPTION_TASTETHEPAIN
    };

    if(opt < 0 || opt >= ULTIMATUM_OPTION_COUNT)
        return 0;

    return max_tiers[opt];
}

// tier taken per option. map array, so it zeroes on load -- the right empty state
typedef struct {
    int tiers[ultimatum::ULTIMATUM_OPTION_COUNT];
} ultimatum_options_T;

ultimatum_options_T module& GetUltimatumOptionsMade() {
    static ultimatum_options_T opts;
    return opts;
}

int GetUltimatumOptionTier(int opt) {
    using ultimatum;

    if(opt < 0 || opt >= ULTIMATUM_OPTION_COUNT)
        return 0;

    ultimatum_options_T module& o = GetUltimatumOptionsMade();
    return o.tiers[opt];
}

bool IsUltimatumOptionMaxed(int opt) {
    return GetUltimatumOptionTier(opt) >= GetUltimatumOptionMaxTier(opt);
}

// The tier table is a module static, which is a MAP array -- each side of the net keeps its own
// copy. The bump happens on the server and the menu that prints the tier is CLIENTSIDE, so without
// this every option reads as tier 1 forever.
Script "DnD Ultimatum Tier Sync" (int opt, int tier) CLIENTSIDE {
    using ultimatum;

    if(opt >= 0 && opt < ULTIMATUM_OPTION_COUNT) {
        ultimatum_options_T module& o = GetUltimatumOptionsMade();
        o.tiers[opt] = tier;
    }

    SetResultValue(0);
}

// returns the NEW tier, 1 on a first pick
int AddUltimatumOptionTier(int opt) {
    using ultimatum;

    if(opt < 0 || opt >= ULTIMATUM_OPTION_COUNT)
        return 0;

    ultimatum_options_T module& o = GetUltimatumOptionsMade();
    if(o.tiers[opt] < GetUltimatumOptionMaxTier(opt))
        ++o.tiers[opt];

    return o.tiers[opt];
}

// both offers ride in npc_info_T.offer. +1 so a pair never reads as NPC_OFFER_NA, and an
// absent second option is a clean 0 in the high word
#define ULTIMATUM_OFFER_BITS 16
#define ULTIMATUM_OFFER_MASK 0xFFFF

int PackUltimatumOffer(int first, int second) {
    return ((first + 1) & ULTIMATUM_OFFER_MASK) |
            (((second + 1) & ULTIMATUM_OFFER_MASK) << ULTIMATUM_OFFER_BITS);
}

// which: 0 first, 1 second. -1 means empty
int GetUltimatumOfferOption(int packed, int which) {
    if(which)
        return ((packed >> ULTIMATUM_OFFER_BITS) & ULTIMATUM_OFFER_MASK) - 1;

    return (packed & ULTIMATUM_OFFER_MASK) - 1;
}

// two DISTINCT options with a tier left. 0 means all maxed -- caller skips the NPC
// A multi-tier option can be picked again to deepen it, so it stays interesting for the whole
// run; a single-tier one is spent the moment it is taken. Weights rather than a rule -- the flat
// ones are still offered, just less often.
#define DND_ULTIMATUM_OFFERWEIGHT_TIERED 3
#define DND_ULTIMATUM_OFFERWEIGHT_FLAT 1

// Added per TIER already taken, so a challenge the players have committed to keeps coming back
// to be deepened, and one taken twice is likelier again than one taken once. Only multi-tier
// options ever carry it: a single-tier one is maxed the moment it is taken and leaves the pool.
#define DND_ULTIMATUM_OFFERWEIGHT_TAKEN 3

int GetUltimatumOptionWeight(int opt) {
    int w = GetUltimatumOptionMaxTier(opt) > 1 ?
        DND_ULTIMATUM_OFFERWEIGHT_TIERED : DND_ULTIMATUM_OFFERWEIGHT_FLAT;

    return w + GetUltimatumOptionTier(opt) * DND_ULTIMATUM_OFFERWEIGHT_TAKEN;
}

int PickUltimatumChallengeOffer() {
    using ultimatum;

    int pool[ULTIMATUM_OPTION_COUNT];
    int count = 0, total = 0;
    int i;

    for(i = ULTIMATUM_OPTION_EXTRA_ELE_DAMAGE; i < ULTIMATUM_OPTION_COUNT; ++i) {
        if(!IsUltimatumOptionMaxed(i)) {
            pool[count++] = i;
            total += GetUltimatumOptionWeight(i);
        }
    }

    if(!count)
        return 0;

    // Two weighted draws, the first struck out of the pool before the second so they cannot
    // collide. The old "pick out of what is LEFT and shift past it" trick only holds for a flat
    // draw -- with weights the shifted index no longer means the same thing.
    //
    // One option left simply leaves the second at -1, which is what a single-offer round packs.
    int picked[2];
    picked[0] = -1;
    picked[1] = -1;

    for(int n = 0; n < 2 && count; ++n) {
        int roll = random(1, total), acc = 0, at = count - 1;

        // walk the running total; the last entry catches any rounding at the top of the range
        for(i = 0; i < count; ++i) {
            acc += GetUltimatumOptionWeight(pool[i]);
            if(roll <= acc) {
                at = i;
                break;
            }
        }

        picked[n] = pool[at];

        // pull the tail over it -- order in the pool carries no meaning
        total -= GetUltimatumOptionWeight(pool[at]);
        pool[at] = pool[--count];
    }

    return PackUltimatumOffer(picked[0], picked[1]);
}

// ---- reward pool -------------------------------------------------------------------------------
#define DND_ULTIMATUM_REWARD_ITEMCHANCE 0.6
#define DND_ULTIMATUM_REWARD_ORBCHANCE 0.9      // the rest rolls a token

// A reward is meant to beat an ordinary drop on both axes: its mods roll well far more often, and
// it carries a few item levels over what the same drop would have -- which is what opens the higher
// mod tiers, since AddAttributeToFieldItem reads item_level for its tier.
//
// Every one of these is the value at the FIRST offer; the _LATE twin is the value at the last. A
// reward is interpolated between them on how far into the run it was earned, so the run pays better
// the longer it is survived. Each pair is an independent dial.
#define DND_ULTIMATUM_REWARD_WELLROLLCHANCE 0.6
#define DND_ULTIMATUM_REWARD_WELLROLLCHANCE_LATE 0.95

#define DND_ULTIMATUM_REWARD_ILVL_MIN 1
#define DND_ULTIMATUM_REWARD_ILVL_MAX 8

// percent ON TOP of a normal stack, so 200 means the last offer pays triple
#define DND_ULTIMATUM_REWARD_STACKBONUS_LATE 200

// On top of the depth bonus above, a stackable pays for its own rarity: the commoner it is the
// bigger the pile. Percent OF the depth-scaled stack, so the two compose rather than replace.
#define DND_ULTIMATUM_STACK_COMMON 100
#define DND_ULTIMATUM_STACK_UNCOMMON 60
#define DND_ULTIMATUM_STACK_RARE 30

// Orbs already carry a rarity axis in the tier predicates, and it covers the special orbs too.
int GetUltimatumOrbStackPct(int orb) {
    if(IsHighTierOrb(orb))
        return DND_ULTIMATUM_STACK_RARE;
    if(IsMidTierOrb(orb))
        return DND_ULTIMATUM_STACK_UNCOMMON;
    return DND_ULTIMATUM_STACK_COMMON;
}

// A token has no tiers, so its own drop weight stands in for one. ItemDropWeights is stored
// CUMULATIVE -- SET_ITEM_WEIGHT writes running totals -- so a token's own weight is the gap to
// the entry before it. Measured against the commonest token rather than against the total, so
// the answer does not shrink every time another token joins the pool.
int GetUltimatumTokenStackPct(int token) {
    int best = 1, mine = 1, own;

    for(int i = 0; i < MAX_TOKENS; ++i) {
        own = ItemDropWeights[DND_DROPPEDITEM_TOKEN][i];
        if(i)
            own -= ItemDropWeights[DND_DROPPEDITEM_TOKEN][i - 1];

        if(own > best)
            best = own;
        if(i == token)
            mine = own;
    }

    return Clamp_Between(
        DND_ULTIMATUM_STACK_RARE + (DND_ULTIMATUM_STACK_COMMON - DND_ULTIMATUM_STACK_RARE) * mine / best,
        DND_ULTIMATUM_STACK_RARE,
        DND_ULTIMATUM_STACK_COMMON
    );
}

// never rounds a reward away to nothing
int ApplyUltimatumStackRarity(int stack, int pct) {
    return Max(1, stack * pct / 100);
}

// The three orbs that otherwise only drop from specific monsters. The ultimatum is the other
// way in, but only once the run is deep enough to have earned it. Their ids are adjacent in
// DND_ORB_*, which is what lets the draw be a range -- verify_ultstack.py holds that.
#define DND_ULTIMATUM_SPECIALORB_WAVE 6
#define DND_ULTIMATUM_SPECIALORB_CHANCE 0.2

int PickUltimatumRewardOrb(int pnum) {
    auto tally = GetCurrentUltimatumTally();

    if(tally.curr_wave >= DND_ULTIMATUM_SPECIALORB_WAVE && random(0, 1.0) <= DND_ULTIMATUM_SPECIALORB_CHANCE)
        return random(DND_ORB_ORDER, DND_ORB_REVERANCE);

    // the player's own weighted table, which stops short of the monster-only orbs
    return PickPlayerOrb(pnum);
}

// Percent applied to DND_SYNERGYITEM_CHANCE, which is checked after each mod until it locks onto a
// tag -- everything rolled after that chases it. 100 is the base rate, so an early reward synergises
// no more often than any charm always has and the boost is purely what depth buys. Kept short of the
// 800 that would make the lock certain: a synergistic reward should read as a good one, not the norm.
#define DND_ULTIMATUM_REWARD_SYNERGY 100
#define DND_ULTIMATUM_REWARD_SYNERGY_LATE 400

// percent ON TOP of the ordinary unique chance
#define DND_ULTIMATUM_REWARD_UNIQUEBONUS_LATE 300

// Every single-tier option is read through this. Same reason the accessors below are functions and
// not the enum members: DnD_Damage.h and the rest are parsed long before this file, and an enum does
// not forward reference in BCS. False on an ordinary map -- the tier table is a map array and
// nothing outside an ultimatum writes it.
bool UltimatumHasOption(int opt) {
    return InformationInLevel[LEVELINFO_ISULTIMATUM] && GetUltimatumOptionTier(opt) > 0;
}

// ULTIMATUM_OPTION_FIRESKULLS -- see DND_ULTIMATUM6. How many spirits are placed, and the
// share of resistance their fire ignores at tier 3. The percent matches the penetrator elite
// trait deliberately -- it is the same effect wearing a different hat.
#define DND_ULTIMATUM_RAGINGDEAD_BASECOUNT 2
#define DND_ULTIMATUM_RAGINGDEAD_PIERCETIER 3
#define DND_ULTIMATUM_RAGINGDEAD_PIERCE 15

int GetUltimatumFirePiercePercent() {
    using ultimatum;

    if(!InformationInLevel[LEVELINFO_ISULTIMATUM])
        return 0;

    if(GetUltimatumOptionTier(ULTIMATUM_OPTION_FIRESKULLS) < DND_ULTIMATUM_RAGINGDEAD_PIERCETIER)
        return 0;

    return DND_ULTIMATUM_RAGINGDEAD_PIERCE;
}

// ULTIMATUM_OPTION_BLISTERINGCOLD -- see DND_ULTIMATUM7_T3 and _T4. A pustule is a level
// hazard, and HandlePlayerChill is monster only, so the chill and freeze those tiers promise
// have to come off this rather than through the usual ailment path.
#define DND_ULTIMATUM_PUSTULE_BASECOUNT 8
#define DND_ULTIMATUM_PUSTULE_CHILLTIER 3
#define DND_ULTIMATUM_PUSTULE_FREEZETIER 4

int GetUltimatumColdTier() {
    using ultimatum;

    if(!InformationInLevel[LEVELINFO_ISULTIMATUM])
        return 0;

    return GetUltimatumOptionTier(ULTIMATUM_OPTION_BLISTERINGCOLD);
}

// Asked rather than the tier itself: DnD_Damage.h is parsed before this file, and a macro does
// not forward reference in BCS while a function does.
bool UltimatumPustuleAlwaysChills() {
    return GetUltimatumColdTier() >= DND_ULTIMATUM_PUSTULE_CHILLTIER;
}

bool UltimatumPustuleAlwaysFreezes() {
    return GetUltimatumColdTier() >= DND_ULTIMATUM_PUSTULE_FREEZETIER;
}

// ULTIMATUM_OPTION_ESCALATINGFRAGILITY -- see DND_ULTIMATUM10. Increased damage taken, per wave
// cleared, capped. Additive with itself only; it is one term, so there is nothing to compose.
#define DND_ULTIMATUM_FRAGILITY_PCT 8
#define DND_ULTIMATUM_FRAGILITY_MAX 40

int GetUltimatumFragilityPercent() {
    using ultimatum;

    if(!UltimatumHasOption(ULTIMATUM_OPTION_ESCALATINGFRAGILITY))
        return 0;

    // curr_wave is 1 based and is the wave being fought, so the first wave under it adds nothing
    auto tally = GetCurrentUltimatumTally();
    return Min(DND_ULTIMATUM_FRAGILITY_MAX, DND_ULTIMATUM_FRAGILITY_PCT * Max(0, tally.curr_wave - 1));
}

// ULTIMATUM_OPTION_OCCASIONALIMPOTENCE -- see DND_ULTIMATUM11. A dead window on a fixed cycle, read
// off the level timer so every client agrees without anything being synced.
#define DND_ULTIMATUM_IMPOTENCE_PERIOD (8 * TICRATE)
#define DND_ULTIMATUM_IMPOTENCE_WINDOW (2 * TICRATE)

bool IsUltimatumImpotenceActive() {
    using ultimatum;

    if(!UltimatumHasOption(ULTIMATUM_OPTION_OCCASIONALIMPOTENCE))
        return false;

    return (Timer() % DND_ULTIMATUM_IMPOTENCE_PERIOD) < DND_ULTIMATUM_IMPOTENCE_WINDOW;
}

// ULTIMATUM_OPTION_FEEBLEREACH -- see DND_ULTIMATUM14. "less", so it multiplies rather than joining
// the increased pool -- see the increased/more convention.
#define DND_ULTIMATUM_FEEBLEREACH_LESS 60

int GetUltimatumFeebleReachLess() {
    using ultimatum;

    return UltimatumHasOption(ULTIMATUM_OPTION_FEEBLEREACH) ? DND_ULTIMATUM_FEEBLEREACH_LESS : 0;
}

// Folds the reduction into an AoE increase percent. Called at the CONSUMPTION points rather
// than inside GetPlayerAoEIncrease, so the artillery term added after it is reduced too.
// The whole (100 + aoe) factor is scaled, not the bonus -- this is an area multiplier.
int ApplyUltimatumFeebleReach(int aoe) {
    int less = GetUltimatumFeebleReachLess();
    if(!less)
        return aoe;

    // floored so (100 + aoe) can never go negative under the roots downstream
    return Max(-100, (100 + aoe) * (100 - less) / 100 - 100);
}

// ULTIMATUM_OPTION_SIPHONINGMONSTERS -- see DND_ULTIMATUM15. Percent of the hit taken as ammo and
// energy shield.
#define DND_ULTIMATUM_SIPHON_PCT 10

int GetUltimatumSiphonPercent() {
    using ultimatum;

    return UltimatumHasOption(ULTIMATUM_OPTION_SIPHONINGMONSTERS) ? DND_ULTIMATUM_SIPHON_PCT : 0;
}

// ULTIMATUM_OPTION_DIMINISHEDLETHALITY -- see DND_ULTIMATUM16. Percent a crit loses against a
// monster. The "unlucky" half is a second roll taken at the crit check itself.
#define DND_ULTIMATUM_CRITLESS_PCT 50

int GetUltimatumCritLessPercent() {
    using ultimatum;

    return UltimatumHasOption(ULTIMATUM_OPTION_DIMINISHEDLETHALITY) ? DND_ULTIMATUM_CRITLESS_PCT : 0;
}

// same option, the "unlucky" half -- a crit roll has to succeed twice
bool UltimatumHasUnluckyCrits() {
    using ultimatum;

    return UltimatumHasOption(ULTIMATUM_OPTION_DIMINISHEDLETHALITY);
}

// ULTIMATUM_OPTION_FLEETINGLIFE / _RAPIDEXHAUSTION -- see DND_ULTIMATUM18 and 20. Percent taken OFF
// a recovery rate, and percent ADDED to a melee cooldown.
#define DND_ULTIMATUM_FLEETINGLIFE_LESS 50
#define DND_ULTIMATUM_EXHAUSTION_LESS 50
#define DND_ULTIMATUM_EXHAUSTION_CDLONGER 100

int GetUltimatumRecoveryLess() {
    using ultimatum;

    return UltimatumHasOption(ULTIMATUM_OPTION_FLEETINGLIFE) ? DND_ULTIMATUM_FLEETINGLIFE_LESS : 0;
}

int GetUltimatumStaminaRecoveryLess() {
    using ultimatum;

    return UltimatumHasOption(ULTIMATUM_OPTION_RAPIDEXHAUSTION) ? DND_ULTIMATUM_EXHAUSTION_LESS : 0;
}

int GetUltimatumMeleeCooldownLonger() {
    using ultimatum;

    return UltimatumHasOption(ULTIMATUM_OPTION_RAPIDEXHAUSTION) ? DND_ULTIMATUM_EXHAUSTION_CDLONGER : 0;
}

// The plain "was this taken" reads. Named so a call site says what it means rather than repeating
// the enum member inline.
bool UltimatumIgnoresArmor() {
    using ultimatum;
    return UltimatumHasOption(ULTIMATUM_OPTION_OVERWHELMINGMONSTERS);
}

bool UltimatumIgnoresResists() {
    using ultimatum;
    return UltimatumHasOption(ULTIMATUM_OPTION_PRECISEMONSTERS);
}

bool UltimatumBlocksFlaskCharges() {
    using ultimatum;
    return UltimatumHasOption(ULTIMATUM_OPTION_DROUGHT);
}

bool UltimatumHindersFlasks() {
    using ultimatum;
    return UltimatumHasOption(ULTIMATUM_OPTION_HINDERINGFLASKS);
}

bool UltimatumMonstersUnstoppable() {
    using ultimatum;
    return UltimatumHasOption(ULTIMATUM_OPTION_UNSTOPPABLETIDE);
}

bool UltimatumReflectsAilments() {
    using ultimatum;
    return UltimatumHasOption(ULTIMATUM_OPTION_TASTETHEPAIN);
}

bool UltimatumBuffsElites() {
    using ultimatum;
    return UltimatumHasOption(ULTIMATUM_OPTION_GLUTTONOUSTIDE);
}

// ULTIMATUM_OPTION_EXTRA_ELE_DAMAGE. Percent of a monster's hit gained as a random element, per
// tier -- see DND_ULTIMATUM1_T1.
#define DND_ULTIMATUM_EXTRAELE_PCT 30

// What HandlePlayerResists asks, on every monster hit while this is running. A FUNCTION rather than
// the enum member itself: DnD_Damage.h is parsed long before this file and an enum does not forward
// reference in BCS, while a function does. 0 when the option was never taken, which is also what an
// ordinary map answers -- the tier table is a map array and nothing outside an ultimatum writes it.
int GetUltimatumExtraElePercent() {
    using ultimatum;

    if(!InformationInLevel[LEVELINFO_ISULTIMATUM])
        return 0;

    return DND_ULTIMATUM_EXTRAELE_PCT * GetUltimatumOptionTier(ULTIMATUM_OPTION_EXTRA_ELE_DAMAGE);
}

// ULTIMATUM_OPTION_SAWBLADES. Chance a blade leaves the player bleeding, indexed BY TIER -- see
// DND_ULTIMATUM2_T2 and _T4. Tier 3 adds blades rather than bleed, so it holds tier 2's number.
// A function for the same reason GetUltimatumExtraElePercent is one: HandlePlayerResists is parsed
// long before this file and an enum does not forward reference in BCS, while a function does.
int GetUltimatumSawBleedChance() {
    using ultimatum;

    if(!InformationInLevel[LEVELINFO_ISULTIMATUM])
        return 0;

    static int chances[5] = {
        0,      // no tier taken
        0,      // tier 1 -- blades only
        30,     // tier 2
        30,     // tier 3 -- more blades, same chance
        100     // tier 4
    };

    return chances[Clamp_Between(GetUltimatumOptionTier(ULTIMATUM_OPTION_SAWBLADES), 0, 4)];
}

// ULTIMATUM_OPTION_CRACKLINGPAIN. What a shot's ammo cost is multiplied by to get the lightning
// damage the shooter takes for it -- see DND_ULTIMATUM4_T1. Per tier, so raising the option ceiling
// in GetUltimatumOptionMaxTier scales it with no other edit.
#define DND_ULTIMATUM_CRACKLE_MULT 20

int GetUltimatumCracklePainMult() {
    using ultimatum;

    if(!InformationInLevel[LEVELINFO_ISULTIMATUM])
        return 0;

    return DND_ULTIMATUM_CRACKLE_MULT * GetUltimatumOptionTier(ULTIMATUM_OPTION_CRACKLINGPAIN);
}

// The wave from which the reward-only uniques join the helm pool. Anathema is meant to be the shape
// of a run that went deep, so it cannot appear in the early offers at all.
#define DND_ULTIMATUM_REWARDONLY_WAVE 6

// How far into the run this reward was earned, 0 at the first offer and 100 at the last. The final
// wave makes no offer, so the last one that does is MAX_ULTIMATUM_WAVES - 2.
int GetUltimatumRewardProgress() {
    using ultimatum;

    auto tally = GetCurrentUltimatumTally();
    int last = MAX_ULTIMATUM_WAVES - 2;
    if(last <= 0)
        return 100;

    return Clamp_Between(tally.curr_wave * 100 / last, 0, 100);
}

// early + (late - early) * p / 100, for whole numbers and for fixed point alike
int ScaleUltimatumReward(int early, int late, int p) {
    return early + (late - early) * p / 100;
}

// Anathema and anything else reward-only is the shape of a run that went deep, so the tail is out
// of reach until the wave count says it was earned.
bool UltimatumCanRollRewardOnly() {
    auto tally = GetCurrentUltimatumTally();
    return tally.curr_wave >= DND_ULTIMATUM_REWARDONLY_WAVE;
}

// The ultimatum helm pool. Past DND_ULTIMATUM_REWARDONLY_WAVE it is the drop pool PLUS the
// reward-only tail, which is the only way anything in that tail can be rolled -- PickUniqueItem and
// the merchant both stop at UNIQUE_HELM_END. Everything else defers to the ordinary pool.
int PickUltimatumUniqueItem(int itype, bool allow_rewardonly) {
    if(itype != DND_ITEM_HELM || !allow_rewardonly)
        return PickUniqueItem(itype);

    // weights are cumulative, so rolling against the tail's running total is what puts it in reach
    int w = random(1, MAX_UNIQUE_HELM_REWARD_WEIGHT);
    int i;
    for(i = UNIQUE_HELM_BEGIN; i <= UNIQUE_HELM_LAST && w > UniqueItemList[i].weight; ++i);

    return i;
}

// somebody's luck has to roll it, so it is a random participant's
int GetUltimatumRewardRoller() {
    // dead is fine here -- this only needs somebody's luck, not a body
    int pnum = PickRandomUltimatumPlayer(false);
    return pnum == -1 ? 0 : pnum;
}

int GetUltimatumRewardItemType(int slot) {
    auto item = GetUltimatumRewardItem(slot);
    return item.item_type;
}

// how many are banked. the bank fills from the front and only empties wholesale, so it is contiguous
int GetUltimatumRewardCount() {
    using ultimatum;

    int n = 0;
    for(int i = 0; i < MAX_ULTIMATUM_WAVES; ++i) {
        auto item = GetUltimatumRewardItem(i);
        if(item.item_type != DND_ITEM_NULL)
            ++n;
    }
    return n;
}

void ClearUltimatumRewardSlot(int slot) {
    auto item = GetUltimatumRewardItem(slot);
    ClearInventoryItem(item);
    SyncItemData_Null(ULTIMATUM_REWARD_OWNER, slot, DND_SYNC_ITEMSOURCE_TRADEVIEW, 1, 1, true);
}

// global 20 outlives the map, so a fresh run has to start empty
void ClearUltimatumRewards() {
    for(int i = 0; i < MAX_ULTIMATUM_REWARD_SLOTS; ++i)
        ClearUltimatumRewardSlot(i);
}

// One slot's worth of item data, pushed again. Sync is the only way the pool reaches a client --
// the array is global 20 and every side keeps its own -- and a slot the client reads as NULL draws
// an empty frame, which is indistinguishable from a missing graphic. Re-sent at offer time rather
// than trusted to have landed once, so a dropped send or a late joiner both heal.
void SyncUltimatumRewardSlot(int slot) {
    auto item = GetUltimatumRewardItem(slot);
    if(item.item_type == DND_ITEM_NULL)
        SyncItemData_Null(ULTIMATUM_REWARD_OWNER, slot, DND_SYNC_ITEMSOURCE_TRADEVIEW, 1, 1, true);
    else
        SyncItemData(ULTIMATUM_REWARD_OWNER, slot, DND_SYNC_ITEMSOURCE_TRADEVIEW, -1, -1, true);
}

// the armor family, unique chance and all -- same shape the drop path uses
void RollUltimatumRewardArmor(int c, int ilvl, int pnum, int itype, int p) {
    int type, maxattr;
    if(itype == DND_ITEM_BODYARMOR) {
        type = ConstructArmorDataOnField(c, ilvl, 0, -1);
        maxattr = MAX_ARMOR_ATTRIB_DEFAULT;
    }
    else if(itype == DND_ITEM_BOOT) {
        type = ConstructBootDataOnField(c, ilvl);
        maxattr = MAX_BOOT_ATTRIB_DEFAULT;
    }
    else {
        type = ConstructHelmDataOnField(c, ilvl, -1);
        maxattr = MAX_HELM_ATTRIB_DEFAULT;
    }

    int uchance = UNIQUE_ARMOR_DROPCHANCE *
        (100 + ScaleUltimatumReward(0, DND_ULTIMATUM_REWARD_UNIQUEBONUS_LATE, p)) / 100;

    if(RunDefaultDropChance(pnum, uchance)) {
        int id = PickUltimatumUniqueItem(itype, UltimatumCanRollRewardOnly());
        if(UniqueItemList[id].item_level <= ilvl) {
            ConstructUniqueOnField(c, id, pnum);
            return;
        }
    }

    RollArmorInfo(c, ilvl, pnum, itype, type, maxattr,
        ScaleUltimatumReward(DND_ULTIMATUM_REWARD_SYNERGY, DND_ULTIMATUM_REWARD_SYNERGY_LATE, p));
}

void RollUltimatumRewardCharm(int c, int ilvl, int pnum, int p) {
    int uchance = UNIQUE_DROPCHANCE *
        (100 + ScaleUltimatumReward(0, DND_ULTIMATUM_REWARD_UNIQUEBONUS_LATE, p)) / 100;

    if(RunDefaultDropChance(pnum, uchance)) {
        int id = PickUniqueItem(DND_ITEM_CHARM);
        if(UniqueItemList[id].item_level <= ilvl) {
            ConstructUniqueOnField(c, id, pnum);
            return;
        }
    }

    RollCharmInfo(c, ilvl, pnum,
        ScaleUltimatumReward(DND_ULTIMATUM_REWARD_SYNERGY, DND_ULTIMATUM_REWARD_SYNERGY_LATE, p));
}

// Rolled the way a drop is rolled, onto a scratch field slot, then copied into the pool. No actor is
// spawned for that slot, so the ring recycles it like any other -- which is also why the pool cannot
// simply hold field indexes: it wraps.
void RollUltimatumReward(int slot) {
    int c = CreateItemSpot();
    if(c == -1)
        return;

    int pnum = GetUltimatumRewardRoller();
    int p = GetUltimatumRewardProgress();

    // add dungeon ilvl here
    // the level bonus leans low early and opens up to its full range by the last offer
    int ilvl = RollItemLevel(MAX_REGULAR_ILVL) + random(DND_ULTIMATUM_REWARD_ILVL_MIN, Max(DND_ULTIMATUM_REWARD_ILVL_MIN, DND_ULTIMATUM_REWARD_ILVL_MAX * p / 100));

    // MAX_BOSS_ILVL is the ceiling anything above an ordinary drop is allowed to reach, and the
    // bonus is what lets a reward cross MAX_REGULAR_ILVL into the top tiers at all.
    if(ilvl > MAX_BOSS_ILVL)
        ilvl = MAX_BOSS_ILVL;

    // Both raised across the dispatch below and put back straight after. Nothing in there can
    // yield, so no other roll can pick either up. The unique branches read them too, intentionally.
    //
    // The cap override stops the Construct paths pulling a reward back down to the arena's own
    // level, so today a reward can reach MAX_BOSS_ILVL whatever level the run is scaled to.
    //
    // TODO once ultimatum reports as a dungeon: hard set this to the dungeon's own level and keep
    // MAX_BOSS_ILVL as the absolute ceiling over it -- Min(DungeonInformation.level, MAX_BOSS_ILVL).
    ItemLevelCapOverride = MAX_BOSS_ILVL;
    WellRolledChanceOverride = ScaleUltimatumReward(DND_ULTIMATUM_REWARD_WELLROLLCHANCE, DND_ULTIMATUM_REWARD_WELLROLLCHANCE_LATE, p);

    // Stackables pay their depth in size rather than in mods, since they have none to roll.
    // This is the depth half only -- the rarity half needs the thing picked first.
    int stack = GetOrbDropStack(ilvl) * (100 + ScaleUltimatumReward(0, DND_ULTIMATUM_REWARD_STACKBONUS_LATE, p)) / 100;
    if(stack < 1)
        stack = 1;

    int roll = random(0, 1.0), t;
    if(roll <= DND_ULTIMATUM_REWARD_ITEMCHANCE) {
        roll = random(0, 1.0);
        if(roll <= DND_MERCHANT_ARMORCHANCE)
            RollUltimatumRewardArmor(c, ilvl, pnum, DND_ITEM_BODYARMOR, p);
        else if(roll <= DND_MERCHANT_HELMCHANCE)
            RollUltimatumRewardArmor(c, ilvl, pnum, DND_ITEM_HELM, p);
        else if(roll <= DND_MERCHANT_BOOTCHANCE)
            RollUltimatumRewardArmor(c, ilvl, pnum, DND_ITEM_BOOT, p);
        else if(roll <= DND_MERCHANT_SPECIALTYCHANCE)
            RollSpecialtyItemInfo(c, ilvl, pnum, random(FIRST_SPECIALTY_ITEM_TYPE, LAST_SPECIALTY_ITEM_TYPE));
        else
            RollUltimatumRewardCharm(c, ilvl, pnum, p);
    }
    else if(roll <= DND_ULTIMATUM_REWARD_ORBCHANCE) {
        t = PickUltimatumRewardOrb(pnum);
        RollOrbInfo(c, t, ApplyUltimatumStackRarity(stack, GetUltimatumOrbStackPct(t)));
    }
    else {
        t = random(1, MAX_TOKEN_WEIGHT);
        for(roll = 0; roll < MAX_TOKENS && ItemDropWeights[DND_DROPPEDITEM_TOKEN][roll] < t; ++roll);
        RollTokenInfo(c, roll, true, ApplyUltimatumStackRarity(stack, GetUltimatumTokenStackPct(roll)));
    }

    WellRolledChanceOverride = 0;
    ItemLevelCapOverride = 0;

    auto to = GetUltimatumRewardItem(slot);
    auto from = GetFieldItem(c);
    ClearInventoryItem(to);
    SetItemToAnother(to, from);
    to.topleftboxid = slot + 1;
    SyncItemData(ULTIMATUM_REWARD_OWNER, slot, DND_SYNC_ITEMSOURCE_TRADEVIEW, -1, -1, true);
}

// the pending offer into the first free bank slot
bool BankUltimatumPendingReward() {
    using ultimatum;

    auto pending = GetUltimatumRewardItem(ULTIMATUM_REWARD_PENDING);
    if(pending.item_type == DND_ITEM_NULL)
        return false;

    for(int i = 0; i < MAX_ULTIMATUM_WAVES; ++i) {
        auto bank = GetUltimatumRewardItem(i);
        if(bank.item_type != DND_ITEM_NULL)
            continue;

        SetItemToAnother(bank, pending);
        bank.topleftboxid = i + 1;
        SyncItemData(ULTIMATUM_REWARD_OWNER, i, DND_SYNC_ITEMSOURCE_TRADEVIEW, -1, -1, true);
        ClearUltimatumRewardSlot(ULTIMATUM_REWARD_PENDING);
        return true;
    }

    return false;
}

// the bank onto the arena floor, free for anyone, then emptied. the pending offer is not part of it
// -- the round it belonged to was never fought.
void DropUltimatumRewards() {
    using ultimatum;

    int c;
    for(int i = 0; i < MAX_ULTIMATUM_WAVES; ++i) {
        auto item = GetUltimatumRewardItem(i);
        if(item.item_type == DND_ITEM_NULL)
            continue;

        c = CreateItemSpot();
        if(c == -1)
            break;

        auto f = GetFieldItem(c);
        SetItemToAnother(f, item);
        f.topleftboxid = 0;
        SyncItemData_Field(c);

        // 256 is the "anyone" owner a shared inventory drop uses
        SpawnDropAtActor(TP_TID_MIDSECTION, GetItemDropClass(item.item_type, item.item_subtype), 16.0, 16, 256, c);
        ClearUltimatumRewardSlot(i);
    }

    ClearUltimatumRewardSlot(ULTIMATUM_REWARD_PENDING);
}

// map side of an accepted option -- spawn the blades, start the miasma, mark the ground.
// tier is the NEW tier; a repeat pick re-enters higher rather than applying twice.
// How long the next wave holds off after a choice is taken, so the line below is not immediately
// talked over by the wave starting on top of it. Declining says nothing and keeps the short delay.
#define DND_ULTIMATUM_CHOICEVOICE_MINDELAY (3 * TICRATE)
#define DND_ULTIMATUM_CHOICEVOICE_MAXDELAY (4 * TICRATE)

// "Ultimatum/ChoiceX_TY". X is the option's LANGUAGE number -- the same + 1 the prompt lumps
// use, so the sound for an option sits under the same number as its text. Y is the tier just
// reached. A single tier option has no _TY half at all, the same rule the offer pane uses for
// its tier line.
str GetUltimatumChoiceSound(int option, int tier) {
    if(GetUltimatumOptionMaxTier(option) <= 1)
        return StrParam(s:"Ultimatum/Choice", d:option + 1);

    return StrParam(s:"Ultimatum/Choice", d:option + 1, s:"_T", d:tier);
}

// What accepting an option does RIGHT NOW. The tier itself is bumped by the caller, and the
// effect of every option is read from that: the accessors answer on the next hit, and the map
// options are placed by StartUltimatumMapHazards when the wave begins. So all that is left
// here is the announcement.
Script "DnD Ultimatum Apply Option" (int option, int tier) {
    // the whole arena hears the choice land, so AmbientSound rather than the Local variant
    AmbientSound(GetUltimatumChoiceSound(option, tier), 127);

    SetResultValue(0);
}

Script "DnD Ultimatum Saw Spawn" (int tier) {
    using ultimatum;

    auto curr_tally = GetCurrentUltimatumTally();

    int total_saws = 8;
    // the tier is already 1 based -- see the note on the miasma actor name
    str saw_actor = StrParam(s:"DnD_Ultimatum_Saw_Tier", d:tier);
    do {
        if(curr_tally.is_wave_complete)
            break;
        
        int i;
        int picked_spots[TP_SAWBLADE_COUNT];
        for(i = 0; i < TP_SAWBLADE_COUNT; ++i)
            picked_spots[i] = 0;

        for(i = 0; i < total_saws; ++i) {
            int saw_spot_tid = 0;
            int max_tries = 7;
            do {
                saw_spot_tid = random(TP_TID_SAWBLADE_BEGIN, TP_TID_SAWBLADE_END);
                --max_tries;
            } while(picked_spots[saw_spot_tid - TP_TID_SAWBLADE_BEGIN] && max_tries);

            picked_spots[saw_spot_tid - TP_TID_SAWBLADE_BEGIN] = 1;

            // PickRandomUltimatumPlayer answers a player NUMBER and -1 when nobody is eligible,
            // while "DnD Sawblade Track" feeds this straight to Vec3To, which wants a TID. Stored
            // as the TID so the blade chases a player rather than whatever actor owns TID 0..N.
            int chase = PickRandomUltimatumPlayer(true);
            if(chase == -1)
                break;

            Spawn(
                saw_actor,
                GetActorX(saw_spot_tid) + random(-24.0, 24.0),
                GetActorY(saw_spot_tid) + random(-24.0, 24.0),
                GetActorZ(saw_spot_tid) + random(-24.0, 24.0),
                DND_ULTIMATUM_TEMPTID
            );
            SetActorProperty(DND_ULTIMATUM_TEMPTID, APROP_ACCURACY, chase + P_TIDSTART);
            Thing_ChangeTID(DND_ULTIMATUM_TEMPTID, 0);
            Delay(const:5);
        }
        Delay(const:TICRATE * 8);

        curr_tally = GetCurrentUltimatumTally();
    } while(!curr_tally.is_wave_complete);
}

Script "DnD Sawblade Track" (int spd) {
    int target = GetActorProperty(0, APROP_ACCURACY);
    Vec3_T* vec = Vec3To(target);
    ToUnitVec3(vec);
    SetActorVelocity(0, spd * vec.x, spd * vec.y, spd * vec.z, 0, 0);
    bcs::free(vec);
}

Script "DnD Ultimatum Storm Call Spawn" (int tier) {
    using ultimatum;

    auto curr_tally = GetCurrentUltimatumTally();

    int total_saws = 8;
    // the tier is already 1 based -- see the note on the miasma actor name
    str storm_actor = StrParam(s:"DnD_Ultimatum_Stormcall_Tier", d:tier);

    int spawn_count = 5;
    if(tier == 4)
        spawn_count += 3;

    int picked_spots[TP_STORMCALL_COUNT];
    for(int i = 0; i < TP_STORMCALL_COUNT; ++i)
        picked_spots[i] = 0;

    for(i = 0; i < spawn_count; ++i) {
        int max_tries = 10;
        int stormcall_spot_tid = 0;
        do {
            stormcall_spot_tid = random(TP_TID_STORMCALL_BEGIN, TP_TID_STORMCALL_END);
            --max_tries;
        } while(picked_spots[stormcall_spot_tid - TP_TID_STORMCALL_BEGIN] && max_tries);
        Spawn(
            storm_actor, 
            GetActorX(stormcall_spot_tid) + random(-16.0, 16.0),
            GetActorY(stormcall_spot_tid) + random(-16.0, 16.0),
            GetActorZ(stormcall_spot_tid),
            DND_ULTIMATUM_STORMCALL_TID + i
        );
        Delay(const:1);
    }

    // handle vanishing of them too
    do {
        Delay(const:TICRATE);
        curr_tally = GetCurrentUltimatumTally();
    } while(!curr_tally.is_wave_complete);

    for(i = 0; i < spawn_count; ++i)
        SetActorState(DND_ULTIMATUM_STORMCALL_TID + i, "CriticalFinish");
}

Script "DnD Ultimatum Raging Dead Spawn" (int tier) {
    using ultimatum;

    auto curr_tally = GetCurrentUltimatumTally();

    // the tier is already 1 based -- see the note on the miasma actor name
    str spirit_actor = StrParam(s:"DnD_Ultimatum_RagingDead_Tier", d:tier);

    // tier 4 is "an additional spirit" -- see DND_ULTIMATUM6_T4. The extra projectiles that
    // tier also promises are the actor's own Missile state, not this.
    int spawn_count = DND_ULTIMATUM_RAGINGDEAD_BASECOUNT;
    if(tier >= 4)
        ++spawn_count;

    spawn_count = Min(spawn_count, TP_RAGINGDEAD_COUNT);

    int picked[TP_RAGINGDEAD_COUNT];
    int i, spot, max_tries;

    for(i = 0; i < TP_RAGINGDEAD_COUNT; ++i)
        picked[i] = 0;

    // Placed once and never refilled: a spirit is unkillable and stays for the wave, so there
    // is nothing to replace. No two share a starting spot.
    for(i = 0; i < spawn_count; ++i) {
        max_tries = 10;
        do {
            spot = random(0, TP_RAGINGDEAD_COUNT - 1);
            --max_tries;
        } while(picked[spot] && max_tries);

        picked[spot] = 1;
        SpawnSpotFacing(spirit_actor, TP_TID_RAGINGDEAD_BEGIN + spot, DND_ULTIMATUM_RAGINGDEAD_TID + i);
        Delay(const:1);
    }

    do {
        Delay(const:TICRATE);
        curr_tally = GetCurrentUltimatumTally();
    } while(!curr_tally.is_wave_complete);

    // Their own Disappear jump reads this. Handed over rather than Thing_Remove so they fade
    // out through the states the actor already has instead of blinking out.
    for(i = 0; i < spawn_count; ++i)
        GiveActorInventory(DND_ULTIMATUM_RAGINGDEAD_TID + i, "DnD_Boolean", 1);
}

Script "DnD Ultimatum Pustule Spawn" (int tier) {
    using ultimatum;

    auto curr_tally = GetCurrentUltimatumTally();

    // the tier is already 1 based -- see the note on the miasma actor name
    str pustule_actor = StrParam(s:"DnD_Ultimatum_Pustule_Tier", d:tier);

    // tier 2 is "50% more pustules" -- see DND_ULTIMATUM7_T2. This is how many may be ALIVE at
    // once, not a total: the loop refills as they pop. The damage half of that tier is the actor's
    // own bigger A_Explode, and the speed half of tier 3 is its shorter timer.
    int max_alive = DND_ULTIMATUM_PUSTULE_BASECOUNT;
    if(tier >= 2)
        max_alive = max_alive * 3 / 2;

    // one pustule per spot, so the spots the map placed are the real ceiling
    max_alive = Min(max_alive, TP_PUSTULE_COUNT);

    // Spot i carries the actor at DND_ULTIMATUM_PUSTULE_TID + i while it is taken. Keyed by SPOT
    // rather than a free list, so "never two in the same place" is the shape of the data itself
    // and the tid to reap is implied by the index.
    int occupied[TP_PUSTULE_COUNT];
    int free_spots[TP_PUSTULE_COUNT];
    int i, alive = 0, free_count, spot, spot_tid;

    for(i = 0; i < TP_PUSTULE_COUNT; ++i)
        occupied[i] = 0;

    do {
        // Reap first, so a spot freed this tick can be drawn again below. A pustule is gone once
        // its burst states run out -- it stays counted through the burst, which is wanted: the
        // explosion is still happening there.
        for(i = 0; i < TP_PUSTULE_COUNT; ++i) {
            if(occupied[i] && !ThingCount(T_NONE, DND_ULTIMATUM_PUSTULE_TID + i)) {
                occupied[i] = 0;
                --alive;
            }
        }

        // At the cap nothing spawns -- the loop just waits for one to pop.
        if(alive < max_alive) {
            // drawn from the free spots only, rather than retried until one lands, so a nearly
            // full arena still places instantly instead of burning tries
            free_count = 0;
            for(i = 0; i < TP_PUSTULE_COUNT; ++i) {
                if(!occupied[i])
                    free_spots[free_count++] = i;
            }

            if(free_count) {
                spot = free_spots[random(0, free_count - 1)];
                spot_tid = TP_TID_PUSTULE_BEGIN + spot;

                // a spot the map never placed answers 0 here; the reap above clears it right back
                Spawn(
                    pustule_actor,
                    GetActorX(spot_tid) + random(-16.0, 16.0),
                    GetActorY(spot_tid) + random(-16.0, 16.0),
                    GetActorZ(spot_tid),
                    DND_ULTIMATUM_PUSTULE_TID + spot
                );

                occupied[spot] = 1;
                ++alive;
            }
        }

        Delay(random(TICRATE / 2, TICRATE * 5));

        curr_tally = GetCurrentUltimatumTally();
    } while(!curr_tally.is_wave_complete);

    // whatever is still sitting there when the wave ends -- the pustule has no quiet exit state of
    // its own, and leaving them would carry the hazard into the offer between rounds
    for(i = 0; i < TP_PUSTULE_COUNT; ++i) {
        if(occupied[i])
            Thing_Remove(DND_ULTIMATUM_PUSTULE_TID + i);
    }
}

Script "DnD Ultimatum Master Boolean" (void) CLIENTSIDE {
    SetActivator(0, AAPTR_MASTER);
    SetResultValue(CheckInventory("DnD_Boolean"));
}

Script "DnD Ultimatum Storm Call Particle" (int tid, int r, int count, int max_count) CLIENTSIDE {
    // make client aware of this tid
    Thing_ChangeTID(0, tid);
    SpawnForced(
        "DnD_Ultimatum_Storrmcall_Particle",
        GetActorX(0) + r * cos(count * 1.0 / max_count),
        GetActorY(0) + r * sin(count * 1.0 / max_count),
        GetActorZ(0) + 8.0,
        DND_ULTIMATUM_STORMCALL_TEMPTID
    );
    SetActivator(DND_ULTIMATUM_STORMCALL_TEMPTID);
    SetActorProperty(0, APROP_MASTERTID, tid);
    SetPointer(AAPTR_MASTER, tid);
    Thing_ChangeTID(0, 0);
    
    SetResultValue(0);
}

#endif