#ifndef DND_MAPEVENTS_IN
#define DND_MAPEVENTS_IN

#define DND_MERCHANT_SPAWNCHANCE 0.2 // 20% chance to spawn per map

enum {
    DND_MAPEVENT_MERCHANTID,
    DND_MAPEVENT_ACIDRAINID,
    DND_MAPEVENT_NOINFIGHTING,

    DND_MAXMAPEVENTS
};

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
    if(random(0, 1.0) <= DND_MERCHANT_SPAWNCHANCE) {
        MapData[DND_MAPDATA_MAPEVENTS1] |= (1 << DND_MAPEVENT_MERCHANTID);
    }
}

#endif