#ifndef DND_ULTIMATUM_IN
#define DND_ULTIMATUM_IN

namespace ultimatum {
    enum {
        MAX_WAVES = 10,
        MAX_WAVE_FORMATIONS = 16,
        MAX_VARIANTS = 4,

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

        TP_TID_MIDSECTION = 4045                // used for spawning rewards, the NPC etc.
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

        ULTIMATUM_OPTION_COUNT
    };
};

typedef struct {
    str actor_name;             // actor to spawn
    int amount;                 // amt to spawn
    int spawn_delay;            // delay between spawns
    int formation_type;         // which teleport group to use
} ultimatum_formation_T;

typedef struct {
    ultimatum_formation_T info[ultimatum::MAX_VARIANTS][ultimatum::MAX_WAVE_FORMATIONS];
    int formation_count[ultimatum::MAX_VARIANTS];
    int variants;
    int formation_types_used;  // the formation types featured in the wave
    int total_monsters[ultimatum::MAX_VARIANTS];
} ultimatum_wave_T;

typedef struct {
    int kills;
    int total_kills;
    int req_kills_wave;
    int curr_wave;
} ultimatum_curr_tally_T;

void AddFormationToWave(ultimatum_wave_T module& w, int variant, str actor, int amt, int del, int type) {
    int count = w.formation_count[variant];
    w.info[variant][count].actor_name = actor;
    w.info[variant][count].amount = amt;
    w.info[variant][count].spawn_delay = del;
    w.info[variant][count].formation_type = type;

    w.formation_types_used |= type;

    // only count those coming from the 1st variant, all variants MUST provide the same amount of actor types
    ++w.formation_count[variant];
    w.total_monsters[variant] += amt;
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
    static ultimatum_wave_T waves[MAX_WAVES];

    if(!init) {
        init = true;

        // variant 1-N, waves 1-10
        auto curr_wave = waves[0];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "Sabreclaw", 1, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "EarthGolem", 1, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Devil2", 1, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "AxeKnight", 1, TICRATE * 3 / 2, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 1, "Nhumcign", 1, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Shadow", 1, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Lurker", 1, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "AxeKnight", 1, TICRATE * 3 / 2, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 2, "SoulEater", 1, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Roach", 1, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Devourer", 1, TICRATE / 2, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "AxeKnight", 1, TICRATE * 3 / 2, WAVE_FORMATION_SIDELINE_ROOFS);
        /*AddFormationToWave(curr_wave, 0, "Sabreclaw", 32, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "EarthGolem", 20, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Devil2", 24, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "AxeKnight", 16, TICRATE * 3 / 2, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 1, "Nhumcign", 32, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Shadow", 32, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Lurker", 20, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "AxeKnight", 16, TICRATE * 3 / 2, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 2, "SoulEater", 32, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Roach", 32, TICRATE / 3, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Devourer", 20, TICRATE / 2, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "AxeKnight", 16, TICRATE * 3 / 2, WAVE_FORMATION_SIDELINE_ROOFS);
*/
        curr_wave = waves[1];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "MoonSatyr", 8, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "BloodSatyr", 16, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "HellsFury", 12, TICRATE * 6, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Shadow", 24, TICRATE * 8, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 1, "IceGolem", 10, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "ShadowBeast", 12, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "ChaosSerpent", 12, TICRATE * 6, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "AxeKnight", 12, TICRATE * 8, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 2, "SludgeGiant", 16, TICRATE * 6, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Yeti", 8, TICRATE * 10, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "AxeKnight", 18, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "ChaosSerpent", 12, TICRATE * 10, WAVE_FORMATION_SIDELINE_ROOFS);
    
        curr_wave = waves[2];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "ShadowBeast", 16, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Nhumcign", 30, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "BloodGolem", 6, TICRATE * 15, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Wicked", 20, TICRATE * 8, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "ChaosSerpent", 12, TICRATE * 10, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 1, "IceGolem", 20, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Lurker", 24, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "BloodGolem", 6, TICRATE * 15, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Cacolich", 16, TICRATE * 8, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "AxeKnight", 16, TICRATE * 10, WAVE_FORMATION_SIDELINE_ROOFS);

        AddFormationToWave(curr_wave, 2, "SludgeGiant", 20, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "BloodSatyr", 16, TICRATE * 4, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "BloodGolem", 6, TICRATE * 15, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "EarthLich", 16, TICRATE * 8, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "Draugr", 16, TICRATE * 10, WAVE_FORMATION_SIDELINE_ROOFS);

        curr_wave = waves[3];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "BlackKnight", 32, TICRATE * 7, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "DreadKnight", 12, TICRATE * 5, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "BloodGolem", 12, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Wicked", 20, TICRATE * 8, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "Draugr", 12, TICRATE * 10, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 0, "Corpulent", 6, TICRATE * 4, WAVE_FORMATION_SIDELINE_WINDOWS);

        AddFormationToWave(curr_wave, 1, "ChaosSerpent", 24, TICRATE * 7, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "MagmaSerpent", 18, TICRATE * 5, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "BloodGolem", 12, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Cacolich", 16, TICRATE * 8, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "AxeKnight", 16, TICRATE * 10, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 1, "Vore", 6, TICRATE * 4, WAVE_FORMATION_SIDELINE_WINDOWS);
    
        AddFormationToWave(curr_wave, 2, "SkullWizard", 36, TICRATE * 7, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "MagmaGolem", 12, TICRATE * 5, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "BloodGolem", 12, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "EarthLich", 16, TICRATE * 8, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "DarkServant", 16, TICRATE * 10, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 2, "Gamon", 6, TICRATE * 4, WAVE_FORMATION_SIDELINE_WINDOWS);
   
        curr_wave = waves[4];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "DarkServant", 20, TICRATE * 16, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "DarkServant", 12, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "DarkZealot", 12, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "Wicked", 24, TICRATE * 8, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "HellArbiter", 10, TICRATE * 12, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "Draugr", 12, TICRATE * 10, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 0, "Corpulent", 9, TICRATE * 4, WAVE_FORMATION_SIDELINE_WINDOWS);

        AddFormationToWave(curr_wave, 1, "HellWarrior", 18, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Kjaroch", 10, TICRATE * 7, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "FleshWizard", 12, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "EarthLich", 20, TICRATE * 8, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "BloodLich", 8, TICRATE * 12, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "Crawler", 16, TICRATE * 10, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 1, "Vore", 9, TICRATE * 4, WAVE_FORMATION_SIDELINE_WINDOWS);

        AddFormationToWave(curr_wave, 2, "IceGolem", 16, TICRATE * 13, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Lavademon", 10, TICRATE * 7, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Hierophant", 12, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "ShadowPriest", 20, TICRATE * 8, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "DESentinel", 8, TICRATE * 12, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "AxeKnight", 16, TICRATE * 10, WAVE_FORMATION_SIDELINE_ROOFS);
        AddFormationToWave(curr_wave, 2, "HellforgeSpider", 9, TICRATE * 4, WAVE_FORMATION_SIDELINE_WINDOWS);
    
        curr_wave = waves[5];
        curr_wave.variants = 3;
        AddFormationToWave(curr_wave, 0, "GoldGolem", 16, TICRATE * 16, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "BloodGolem", 12, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "IceGolem", 12, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 0, "ShadowPriest", 20, TICRATE * 6, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "ShadowDisciple", 8, TICRATE * 16, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 0, "AvatarOfChaos", 1, TICRATE * 4, WAVE_FORMATION_BOSS_GROUND);
    
        AddFormationToWave(curr_wave, 1, "Gamon", 16, TICRATE * 16, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "MoonSatyr", 12, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "BloodSatyr", 12, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 1, "Inferno", 24, TICRATE * 6, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "Helemental", 10, TICRATE * 16, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 1, "HellsmithX", 1, TICRATE * 4, WAVE_FORMATION_BOSS_GROUND);

        AddFormationToWave(curr_wave, 2, "BlackKnight", 16, TICRATE * 8, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "DreadKnight", 12, TICRATE * 16, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "DeathKnight", 4, TICRATE * 12, WAVE_FORMATION_GROUND);
        AddFormationToWave(curr_wave, 2, "Wicked", 20, TICRATE * 6, WAVE_FORMATION_FLIER);
        AddFormationToWave(curr_wave, 2, "Defiler", 8, TICRATE * 16, WAVE_FORMATION_FLIER);
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
}

Script "DnD Init Ultimatum - CS" (void) CLIENTSIDE {
    InformationInLevel[LEVELINFO_ISULTIMATUM] = 1;
}

Script "DnD Start Ultimatum Wave" (int wave) {
    auto w = GetUltimatumWaveInfo(wave);
    int var = random(0, w.variants - 1);

    Delay(const:1);

    using ultimatum;

    auto curr_tally = GetCurrentUltimatumTally();
    curr_tally.curr_wave = wave;
    curr_tally.kills = 0;
    curr_tally.req_kills_wave = w.total_monsters[var];

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
                Delay(const:TICRATE / 2);
        } while(!can_spawn);
        SpawnSpot("TeleportFog", spot_tid);
        Delay(info.spawn_delay);
    }
}

Script "DnD Ultimatum Wave Completed" (void) {
    using ultimatum;

    auto curr_tally = GetCurrentUltimatumTally();

    NPC_States[DND_NPC_DARKWANDERER].aux_data = 0;
    NPC_States[DND_NPC_DARKWANDERER].dialog = random(DW_ULTIMATUM_ASK1, DW_ULTIMATUM_ASK3);
    NPC_States[DND_NPC_DARKWANDERER].offer = PickUltimatumChallengeOffer();

    for(int i = 0; i < MAXPLAYERS; ++i)
        NPC_States[DND_NPC_DARKWANDERER].voters[i] = 0;
    ACS_NamedExecuteWithResult("DnD NPC Vote Sync", -1, -1, DND_NPC_DARKWANDERER);

    ACS_NamedExecuteWithResult("DnD Try Spawn Area", TP_TID_MIDSECTION, "DarkWanderer", DND_NPC_TID, 96 | (4 << 16) | (22 << 24));
}

Script "DnD Handle Ultimatum Finish" (void) {
    // decide whether boss can appear or we concluded
}

typedef struct {
    int id;
    int tiers;
} ultimatum_option_T;

ultimatum_option_T module& GetUltimatumOptionsMade() {
    static ultimatum_option_T opts;
    return opts;
}

// pick a challenge based on its tier and whatever is left
int PickUltimatumChallengeOffer() {
    return random(ULTIMATUM_OPTION_EXTRA_ELE_DAMAGE, ULTIMATUM_OPTION_COUNT - 1);
}

#endif