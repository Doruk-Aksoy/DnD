#ifndef DND_SPELLCAST_IN
#define DND_SPELLCAST_IN

#include "DnD_Mana.h"

// Cooldowns are MAP scoped. The legacy path reset them on map end for the same reason: a Timer()
// stamp that outlived its map reads as a cooldown that never ends. Zero means never cast, which is
// also ready, so the zero fill every map load leaves behind is already the correct rest state.
//
// Behind an accessor with a function static rather than declared at module scope, which is the
// house rule for anything this size -- see GetPerkScroll. This one is MAXPLAYERS x MAX_SPELL_IDS.
typedef struct {
	int at[MAXPLAYERS][MAX_SPELL_IDS];
} spell_cooldown_T;

// Heart of Fire's rank 5 window: the tic a spell's empowerment runs out on, or 0. Sits next to the
// cooldowns because it is the same kind of state and is written in the same breath as one. Server
// side only -- the damage that reads it is resolved there too -- so unlike the ranks it needs no sync.
typedef struct {
	int ends[MAXPLAYERS][MAX_SPELL_IDS];
} spell_prime_T;

spell_prime_T module& GetSpellPrimes() {
	static spell_prime_T s;
	return s;
}

spell_cooldown_T module& GetSpellCooldowns() {
	static spell_cooldown_T s;
	return s;
}

#define DND_SPELL_MAXCDR 75			// net cooldown reduction ceiling, per the design

// One point every two levels. MAXLEVELS is 100, so levelling alone reaches the cap exactly -- the
// cap is there to hold if the level ceiling ever moves. Tokens cap separately, for 100 in total.
#define DND_SPELLPOINT_PERLEVELS 2
#define DND_SPELLPOINT_MAXFROMLEVEL 50
#define DND_SPELLPOINT_MAXFROMTOKEN 50

int GetSpellCooldownRate(int pnum, int spell) {
	return (GetSpellValue(pnum, spell, SPELLVAL_CDR) >> 16) + PlayerModData[pnum].vals[PSTAT_SPELL_CDR] +
		pbuffs[pnum].buff_net_values[BUFF_SPELLCDR].additive;
}

// FinalCD = Cooldown / (1 + Rate/100), floored by the 75% ceiling. Rate is the only shrinking term
// in the whole system and it is asymptotic, so the floor is a design cap rather than a safety one.
int GetSpellCooldownTics(int pnum, int spell) {
	int cd = (GetSpellValue(pnum, spell, SPELLVAL_COOLDOWN) * TICRATE) >> 16;
	if(cd <= 0)
		return 0;

	int rate = Max(0, GetSpellCooldownRate(pnum, spell));
	return Max(cd * 100 / (100 + rate), cd * (100 - DND_SPELL_MAXCDR) / 100);
}

// Haste is given as casts per second in the design; solved for the interval it is the same shape as
// the cooldown formula.
int GetSpellCastTics(int pnum, int spell) {
	int ct = (GetSpellValue(pnum, spell, SPELLVAL_CASTTIME) * TICRATE) >> 16;
	if(ct <= 0)
		return 0;

	int haste = PlayerModData[pnum].vals[PSTAT_SPELL_HASTE];
	if(haste <= 0)
		return ct;

	return Max(1, ct * 100 / (100 + haste));
}

bool IsSpellOnCooldown(int pnum, int spell) {
	return GetSpellCooldowns().at[pnum][spell] > Timer();
}

int GetSpellCooldownLeft(int pnum, int spell) {
	return Max(0, GetSpellCooldowns().at[pnum][spell] - Timer());
}

// 0-100 for the hotbar overlay. The total is recomputed rather than stored: a second array per
// player per spell buys only the case where a cooldown stat changes mid-cooldown, which moves the
// bar slightly and nothing else.
int GetSpellCooldownPercent(int pnum, int spell) {
	int left = GetSpellCooldownLeft(pnum, spell);
	if(!left)
		return 0;

	int total = GetSpellCooldownTics(pnum, spell);
	if(total <= 0)
		return 0;

	return Min(100, left * 100 / total);
}

void StartSpellCooldown(int pnum, int spell) {
	int cd = GetSpellCooldownTics(pnum, spell);
	if(cd > 0)
		GetSpellCooldowns().at[pnum][spell] = Timer() + cd;

	// Every bind holding this spell, since one spell may sit on several.
	SyncHotbarForSpell(pnum, spell);
}

void ResetAllSpellCooldownsNew(int pnum) {
	for(int i = 0; i < MAX_SPELL_IDS; ++i)
		GetSpellCooldowns().at[pnum][i] = 0;
}

// ---- spell points ----------------------------------------------------------------------------
// Inventory backed, as perk points are: it saves, it syncs, and the console can read it.

int GetSpellPoints(int pnum) {
	return CheckActorInventory(pnum + P_TIDSTART, "SpellPoint");
}

// Mirrors GivePerkPoints: the pool is an inventory item, and the activity lane is the DELTA the
// level-end save writes. Both have to move together or a save between full writes loses the change.
void GiveSpellPoints(int pnum, int amt) {
	if(amt > 0)
		GiveActorInventory(pnum + P_TIDSTART, "SpellPoint", amt);
		UpdateActivity(pnum, DND_ACTIVITY_SPELLPOINT, amt, 0);
}

// Both sources run through here. The counter is what the cap is read off, not the unspent pool, so
// spending points never earns a player more of them. Returns what was actually granted.
int GiveSpellPointsFrom(int pnum, int amt, str counter, int cap) {
	if(amt <= 0)
		return 0;

	int given = CheckActorInventory(pnum + P_TIDSTART, counter);
	amt = Min(amt, cap - given);
	if(amt <= 0)
		return 0;

	GiveActorInventory(pnum + P_TIDSTART, counter, amt);
	GiveSpellPoints(pnum, amt);
	return amt;
}

int GiveSpellPointsFromLevel(int pnum, int amt) {
	return GiveSpellPointsFrom(pnum, amt, "SpellPointsFromLevel", DND_SPELLPOINT_MAXFROMLEVEL);
}

// For the monster and chest drops that are not in yet.
int GiveSpellPointsFromToken(int pnum, int amt) {
	return GiveSpellPointsFrom(pnum, amt, "SpellPointsFromToken", DND_SPELLPOINT_MAXFROMTOKEN);
}

bool CanAllocateSpellPoint(int pnum, int spell) {
	return GetSpellPoints(pnum) > 0 &&
		GetSpellAllocatedRank(pnum, spell) < DND_SPELL_RANKCAP &&
		IsSpellUnlockable(pnum, spell);
}

bool AllocateSpellPoint(int pnum, int spell) {
	if(!CanAllocateSpellPoint(pnum, spell))
		return false;

	TakeActorInventory(pnum + P_TIDSTART, "SpellPoint", 1);
	UpdateActivity(pnum, DND_ACTIVITY_SPELLPOINT, -1, 0);
	SetSpellAllocatedRank(pnum, spell, GetSpellAllocatedRank(pnum, spell) + 1);
	SyncSpellRankWord(pnum, spell);

	// A rank can move an aura's reservation, so the two are reconciled on the same tick.
	ValidateAuraReservations(pnum);
	return true;
}

// ---- spawning --------------------------------------------------------------------------------

// The variables every template carries. Split out because the three spawn shapes below have nothing
// else in common -- a projectile is thrown by CreateProjectile, an anchor is placed, and neither
// wants the other's plumbing.
void SetupSpellActor(int tid, int pnum, int spell) {
	int caster = pnum + P_TIDSTART;
	int prev = ActivatorTID();

	if(!SetActivator(tid))
		return;

	// The damage path looks for the owner in all three of these.
	SetPointer(AAPTR_TARGET, caster);
	SetActorProperty(0, APROP_TARGETTID, caster);
	SetActorProperty(0, APROP_SCORE, caster);

	SetUserVariable(0, "user_spellid", spell);
	SetUserVariable(0, "user_spellrank", GetSpellRank(pnum, spell, true));
	SetUserVariable(0, "user_extra", 0);

	if(prev)
		SetActivator(prev);
}

// Thrown along where the player is looking. Goes through CreateProjectile rather than SpawnForced
// plus SetActorVelocity: that path already owns the ProjectileHelper trick, which exists because
// moving the player to the muzzle position jitters them.
void SpawnSpellProjectile(int pnum, int spell, str actor, int speed, int flags = 0, int angle_off = 0) {
	int owner = pnum + P_TIDSTART;

	// + 1.0 before the wrap: an offset to the left is negative, and the modulo of a negative angle
	// does not come back inside 0..1 on its own.
	int a = (GetActorAngle(owner) + angle_off + 1.0) % 1.0;
	int pt = Clamp_Between(GetActorPitch(owner), -0.248, 0.248);
	int cosp = cos(pt);

	Vec3_T* vPos = GetVec3(GetActorX(owner), GetActorY(owner), GetActorZ(owner) + GetActorViewHeight(owner) - 5.0);
	Vec3_T* vProj = GetVec3();

	vProj.x = speed * FixedMul(cos(a), cosp);
	vProj.y = speed * FixedMul(sin(a), cosp);
	vProj.z = -sin(pt) * speed;

	// Handed back under a tid of our own: CreateProjectile releases TEMPORARY_ATTACK_TID before it
	// returns, so setting the spell up on that tid afterwards reached nothing at all and every spell
	// projectile flew with user_spellid still 0 -- resolving its damage as spell 0 rather than its own.
	int tid = TEMPORARY_SPELL_TID + pnum;
	CreateProjectile(owner, PROJECTILE_HELPER_TID + pnum, actor, a, pt, speed, vProj, vPos, flags, 0, 0, 0, tid);
	SetupSpellActor(tid, pnum, spell);
	Thing_ChangeTID(tid, 0);

	bcs::free(vProj);
	bcs::free(vPos);
}

// Placed rather than thrown: Immolation's ring, Frost Bomb's crystal, Blizzard, Volcano.
int SpawnSpellAnchor(int pnum, int spell, str actor, int x, int y, int z) {
	int tid = TEMPORARY_SPELL_TID + pnum;
	if(!SpawnForced(actor, x, y, z, tid, 0))
		return 0;

	SetupSpellActor(tid, pnum, spell);

	// Handed back so the next cast does not collide with it. Callers that need to keep talking to
	// the anchor should give it a tid of their own before this returns it.
	Thing_ChangeTID(tid, 0);
	return tid;
}

// Steps out from the middle alternating sides -- 0, +1, -1, +2, -2 -- so the shot on the player's
// aim stays the centre of the fan however many there are, and adding one never moves the others.
int FanOffset(int index, int step) {
	if(!index)
		return 0;

	int pair = (index + 1) / 2;
	return (index & 1) ? pair * step : -pair * step;
}

// ---- aiming ----------------------------------------------------------------------------------
// Where the player is pointing, for spells that spawn AT a point rather than fly to one. LineAttack's
// puff tid does work on this Zandronum -- the note in DnD_Vec.h predates that -- so the trace reads
// back on the same tic and a targeted spell can aim at the end of its cast rather than the start.
typedef struct {
	int x[MAXPLAYERS];
	int y[MAXPLAYERS];
	int z[MAXPLAYERS];
} spell_aim_T;

spell_aim_T module& GetSpellAim() {
	static spell_aim_T s;
	return s;
}

bool TraceSpellAim(int pnum, int maxdist) {
	int prev = ActivatorTID();
	if(!SetActivator(pnum + P_TIDSTART))
		return false;

	int tid = TEMPORARY_SPELL_TID + pnum;

	// Zero damage and no decal: this is a ruler, not an attack. ALWAYSPUFF on the puff means a trace
	// that hits nothing still lands one, at maxdist.
	LineAttack(0, GetActorAngle(0), GetActorPitch(0), 0, "DnD_SpellAimPuff", "None", maxdist,
		FHF_NORANDOMPUFFZ | FHF_NOIMPACTDECAL, tid);

	GetSpellAim().x[pnum] = GetActorX(tid);
	GetSpellAim().y[pnum] = GetActorY(tid);
	GetSpellAim().z[pnum] = GetActorZ(tid);

	// Released immediately -- SpawnSpellAnchor takes the same tid, and a cast does both.
	Thing_ChangeTID(tid, 0);

	if(prev)
		SetActivator(prev);

	return true;
}

// ---- hitscan ----------------------------------------------------------------------------------

#define DND_SPELL_HITSCANRANGE 2048.0

// How far past its own radius a monster may sit from the puff and still count as what was hit. The
// puff lands ON the victim's surface, so the true figure is the victim's radius; the slack only
// covers the puff being placed a little inside or outside that.
#define DND_SPELL_HITSCANSLACK 12.0

// What a hitscan landed on, or 0. LineAttack hands back no victim and a puff carries no dependable
// pointer to what it struck, so this works backwards from where the puff ended up: a puff sits on the
// victim's surface, so the victim is the monster whose own radius reaches it. A trace that hit
// geometry instead leaves the puff flat against a wall, where that test fails for everything standing
// near it -- which is what keeps a wall shot from igniting a bystander.
int FindHitscanVictim(int pufftid) {
	int i, mn, d, best = 0, bestd = 0;
	int px = GetActorX(pufftid), py = GetActorY(pufftid), pz = GetActorZ(pufftid);

	for(mn = 0; mn < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mn) {
		i = UsedMonsterTIDs[mn];
		if(!IsActorAlive(i) || !CheckFlag(i, "SHOOTABLE"))
			continue;

		// A cylinder, because that is the shape of a Doom hitbox -- and because a straight fdistance
		// is 3D while an actor's origin is at its FEET. A chest height hit on an imp is ~35 units up,
		// which swamps the 20 unit radius and put every victim out of range: the test never matched
		// and nothing was ever handed the hit effect.
		if(pz < GetActorZ(i) - DND_SPELL_HITSCANSLACK ||
			pz > GetActorZ(i) + GetActorProperty(i, APROP_HEIGHT) + DND_SPELL_HITSCANSLACK)
			continue;

		d = fdistance_delta(px - GetActorX(i), py - GetActorY(i), 0);
		if(d > GetActorProperty(i, APROP_RADIUS) + DND_SPELL_HITSCANSLACK)
			continue;

		if(!best || d < bestd) {
			best = i;
			bestd = d;
		}
	}

	return best;
}

// A damaging trace along where the caster is looking. Returns what it hit, or 0.
//
// The damage word is resolved BEFORE the trace, unlike every other spell shape here. LineAttack
// applies the damage it is handed and offers no impact hook, so there is no later moment to resolve
// it in -- a projectile gets one for free by carrying user_spellid until it lands. A throwaway anchor
// carries those same variables for one call so "DnD Spell Damage" is reused verbatim rather than
// reimplemented; the word it returns is packed exactly like HitscanDamageData's, which is what the
// damage handler already expects to unpack off a trace.
int SpellHitscan(int pnum, int spell, str puff, int range = DND_SPELL_HITSCANRANGE) {
	int caster = pnum + P_TIDSTART;
	int tid = TEMPORARY_SPELL_TID + pnum;
	int prev = ActivatorTID();

	// The carrier takes the dummy tid rather than sharing the puff's: the puff is measured from by tid
	// straight after, and nothing here should depend on the carrier's removal having already landed.
	int carrier = TEMPORARY_DATADUMMY_TID + pnum;
	if(!SpawnForced("DnD_SpellAnchor", GetActorX(caster), GetActorY(caster), GetActorZ(caster), carrier, 0))
		return 0;

	SetupSpellActor(carrier, pnum, spell);
	SetActivator(carrier);
	int packed = ACS_NamedExecuteWithResult("DnD Spell Damage", SPELLVAL_DAMAGE, 0);
	SetActivator(caster);
	Thing_Remove(carrier);

	// Damage type "None": the element rides in the packed word, the same as a weapon hitscan.
	LineAttack(caster, GetActorAngle(caster), GetActorPitch(caster), packed, puff, "None",
		range, FHF_NORANDOMPUFFZ, tid);

	int victim = FindHitscanVictim(tid);
	Thing_ChangeTID(tid, 0);

	if(prev)
		SetActivator(prev);

	return victim;
}

// ---- channelling -------------------------------------------------------------------------------

// A channel runs while ATTACK is held. There is no way to ask whether the hotbar key that started it
// is still down -- ACS only sees the standard inputs -- so attack is the button, and "DnD Can Fire
// Weapon" stands the weapon down for as long as DnD_SpellBusy is held.
#define DND_CHANNEL_BUTTON BT_ATTACK

// The cast came from a hotbar key, so the player needs a moment to get onto the button. Until it has
// been seen held ONCE, this window keeps the channel alive; after that, letting go ends it. A player
// who never presses it still gets the spell, for its full duration.
#define DND_CHANNEL_GRACE (TICRATE / 2)

// -1, the activator, not the player index. The channel script runs with the caster as activator and
// that is the form 34 of the mod's 37 GetPlayerInput calls use; the indexed form is only used from
// places that already know they are on the right machine for that player.
bool IsChannelHeld() {
	return !!(GetPlayerInput(-1, INPUT_BUTTONS) & DND_CHANNEL_BUTTON);
}

// Every path that makes the player unable to use their weapon goes through this pair, so there is one
// thing to hold and one thing to release. Counted, so a cast time running into a channel never drops
// the lock between them.
// A channel keeps paying for itself, once per DAMAGE INSTANCE -- whatever period the spell's own
// text gives for its damage is the period it is billed on, so the cost and the effect stay in step.
// The caller decides that cadence and calls this only on those tics; this just handles the first one.
//
// TryCastSpell took one cost at the press, which buys the first instance, so `first` is free here --
// otherwise a cast would be charged twice before doing anything.
//
// Returns false when the player can no longer afford it, which is one of the two ways a channel ends
// -- the other is letting go. Nothing is spent on the instance that fails, so running dry cannot
// leave a negative balance.
bool PayChannelTick(int pnum, int spell, bool first) {
	return first || SpendSpellMana(pnum, spell);
}

void BeginSpellBusy(int pnum) {
	GiveActorInventory(pnum + P_TIDSTART, "DnD_SpellBusy", 1);
}

void EndSpellBusy(int pnum) {
	TakeActorInventory(pnum + P_TIDSTART, "DnD_SpellBusy", 1);
}

// ---- casting ---------------------------------------------------------------------------------

enum {
	CAST_OK,
	CAST_NOSPELL,
	CAST_LOCKED,
	CAST_PASSIVE,
	CAST_COOLDOWN,
	CAST_NOMANA,
	CAST_BUSY
};

// "If a weapon is being fired, reloaded etc. the spell won't go off. Only when the weapon is idle."
// Every weapon refreshes DnD_WeaponIdle inside its Ready loop and the powerup lasts two tics, so
// leaving Ready for any reason lets it lapse -- no state has to remember to clear it.
bool IsWeaponIdleForCast(int pnum) {
	return !!CheckActorInventory(pnum + P_TIDSTART, "DnD_WeaponIdle");
}

int CanCastSpell(int pnum, int spell) {
	if(spell < 0 || spell >= MAX_SPELL_IDS)
		return CAST_NOSPELL;

	if(!GetSpellAllocatedRank(pnum, spell))
		return CAST_LOCKED;

	// Passives work from allocation and auras are toggled in the tree; neither is ever cast.
	if(SpellDefs[spell].flags & (SPLF_PASSIVE | SPLF_AURA))
		return CAST_PASSIVE;

	if(IsSpellOnCooldown(pnum, spell))
		return CAST_COOLDOWN;

	if(!CanAffordSpell(pnum, spell))
		return CAST_NOMANA;

	if((SpellDefs[spell].flags & SPLF_CHANNELED) && !IsWeaponIdleForCast(pnum))
		return CAST_BUSY;

	return CAST_OK;
}

// The one entry point. Mana is spent and the cooldown started here rather than inside each spell,
// so no spell can forget either, and a spell that fails to spend never reaches its effect.
int TryCastSpell(int pnum, int spell) {
	int res = CanCastSpell(pnum, spell);
	if(res != CAST_OK)
		return res;

	if(!SpendSpellMana(pnum, spell))
		return CAST_NOMANA;

	StartSpellCooldown(pnum, spell);
	ACS_NamedExecuteAlways("DnD Spell Cast", 0, spell, pnum);
	return CAST_OK;
}

int TryCastHotbarSlot(int pnum, int slot) {
	if(slot < 0 || slot >= GetHotbarSlotCount(pnum))
		return CAST_NOSPELL;
	return TryCastSpell(pnum, GetHotbarSpell(pnum, slot));
}

#endif
