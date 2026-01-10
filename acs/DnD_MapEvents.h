#ifndef DND_MAPEVENTS_IN
#define DND_MAPEVENTS_IN

#define DND_MERCHANT_SPAWNCHANCE 0.25 // 25% chance to spawn per map

enum {
    DND_MAPEVENT_MERCHANTID,
    DND_MAPEVENT_ACIDRAINID,
    DND_MAPEVENT_NOINFIGHTING,

    // unique monster chances
    DND_MAPEVENT_UZOMBIEMAN,
    DND_MAPEVENT_USHOTGUNGUY,
    DND_MAPEVENT_UCHAINGUNGUY,
    DND_MAPEVENT_UDEMON,
    DND_MAPEVENT_USPECTRE,
    DND_MAPEVENT_UIMP,
    DND_MAPEVENT_UCACODEMON,
    DND_MAPEVENT_UPAINELEMENTAL,
    DND_MAPEVENT_ULOSTSOUL,
    DND_MAPEVENT_UREVENANT,
    DND_MAPEVENT_UHELLKNIGHT,
    DND_MAPEVENT_UBARON,
    DND_MAPEVENT_UFATSO,
    DND_MAPEVENT_UARACHNOTRON,
    DND_MAPEVENT_UARCHVILE,
    DND_MAPEVENT_USPIDERMASTERMIND,
    DND_MAPEVENT_UCYBERDEMON,

    DND_MAXMAPEVENTS
};
#define DND_MAPEVENT_FIRSTUNIQUEMONSTER DND_MAPEVENT_UZOMBIEMAN

void CheckUniqueMonsterChance(int event_id) {
    if(random(0, 1.0) <= GetCVar("dnd_uniquemonster_spawnchance"))
        AcceptMapEvent(event_id);
}

void AcceptMapEvent(int e) {
    MapData[DND_MAPDATA_MAPEVENTS1] |= (1 << e);
}

bool CheckMapEvent(int e) {
	return MapData[DND_MAPDATA_MAPEVENTS1] & (1 << e);
}

void ClearMapEvent(int e) {
    MapData[DND_MAPDATA_MAPEVENTS1] &= ~(1 << e);
}

void ResetMapEvents() {
    MapData[DND_MAPDATA_MAPEVENTS1] = 0;

    for(int i = 0; i < DND_MAXMAPEVENTS; ++i)
        TempArray[TARR_MAPEVENTS][i] = 0;
}

void RollMapEvents() {
#ifdef ISDEBUGBUILD
    if(1) {
#else
    if(random(0, 1.0) <= DND_MERCHANT_SPAWNCHANCE) {
#endif
        MapData[DND_MAPDATA_MAPEVENTS1] |= (1 << DND_MAPEVENT_MERCHANTID);
    }
}

#endif