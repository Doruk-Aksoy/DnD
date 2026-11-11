#ifndef DND_MONSTERSCAN_IN
#define DND_MONSTERSCAN_IN

// Blockmap search for monsters near an actor: the probe's A_RadiusGive pings each one, and the ping lists its tid.
// Only monster tids are listed, the set the old walks over UsedMonsterTIDs saw. Server only (A_RadiusGive is).
#define DND_SCAN_MAX 2048
// fdistance runs feet to feet, the engine centre to centre: this margin keeps a 3D feet test inside the sphere.
#define DND_SCAN_FEETMARGIN 128.0

enum {
	DND_SCAN_LIVE,
	DND_SCAN_CORPSES,
	DND_SCAN_SEEN,      // live and in the source's sight, by the engine's own A_RadiusGive check
	DND_SCAN_ANY        // live and dead
};

typedef struct {
	int tid[DND_SCAN_MAX];
	int top, radius, mode;
} monster_scan_T;

monster_scan_T module& GetMonsterScan() {
	static monster_scan_T s;
	return s;
}

// Lists the monsters within radius (fixed, 3D) of the point; returns where they start, GetMonsterScanEnd() the end.
// A stack: release with ReleaseMonsterScan(start) when done, and never Delay while reading it (copy first).
int ScanMonstersAt(int x, int y, int z, int radius, int mode = DND_SCAN_LIVE) {
	int tid = ScratchTID(), start = GetMonsterScan().top;
	if(tid && SpawnForced("DnD_ScanSource", x, y, z, tid)) {
		start = ScanMonstersFrom(tid, radius, mode);
		Thing_Remove(tid);
	}
	return start;
}

// The same from an actor (0: the activator), measured from its centre. A player searches through a stand-in at its
// centre, since a give to a player is sent to every client; any other actor carries the probe itself.
int ScanMonstersNear(int src, int radius, int mode = DND_SCAN_LIVE) {
	if(src ? isPlayer(src) : PlayerNumber() >= 0)
		return ScanMonstersAt(GetActorX(src), GetActorY(src), GetActorZ(src) + GetActorProperty(src, APROP_HEIGHT) / 2, radius, mode);
	return ScanMonstersFrom(src, radius, mode);
}

int ScanMonstersFrom(int src, int radius, int mode) {
	monster_scan_T module& s = GetMonsterScan();
	int start = s.top;
	str probe = mode == DND_SCAN_SEEN ? "DnD_SightScanProbe" : mode == DND_SCAN_LIVE ? "DnD_MonsterScanProbe" : "DnD_CorpseScanProbe";
	s.radius = (radius + 0xFFFF) >> 16;
	s.mode = mode;
	// GiveActorInventory(0, ...) would reach every player, not the activator
	if(src)
		GiveActorInventory(src, probe, 1);
	else
		GiveInventory(probe, 1);
	return start;
}

// For a probe of the mod's own whose A_RadiusGive gives DnD_MonsterScanPing: call before giving it, then read to the end.
int BeginMonsterScan(int mode) {
	GetMonsterScan().mode = mode;
	return GetMonsterScan().top;
}

int GetMonsterScanEnd() {
	return GetMonsterScan().top;
}

int GetMonsterScanTid(int i) {
	return GetMonsterScan().tid[i];
}

void ReleaseMonsterScan(int start) {
	GetMonsterScan().top = start;
}

// The probe's radius, in map units.
Script "DnD Scan Radius" (void) {
	SetResultValue(GetMonsterScan().radius);
}

// One monster in reach: a monster tid goes on the list (DND_SCAN_CORPSES keeps only the dead).
Script "DnD Scan Ping" (void) {
	monster_scan_T module& s = GetMonsterScan();
	int tid = ActivatorTID();
	if(tid >= DND_MONSTERTID_BEGIN && tid < DND_LASTMONSTER_TID && s.top < DND_SCAN_MAX && (s.mode != DND_SCAN_CORPSES || !isAlive()))
		s.tid[s.top++] = tid;
	SetResultValue(0);
}

#endif
