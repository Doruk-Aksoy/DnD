#ifndef DND_ATTACHMENT_IN
#define DND_ATTACHMENT_IN

typedef struct {
	int val;
} attachment_data_T;

// max 32, using an int as a bitfield
attachment_data_T module& GetMonsterAttachmentsUsed(int m_id) {
	static attachment_data_T MonsterAttachmentUsed[DND_MAX_MONSTERS];
	return MonsterAttachmentUsed[m_id];
}

enum {
	DND_ELITEFX_REFLECT = 1,
	DND_ELITEFX_CRIPPLEAURA,
	DND_ELITEFX_VIOLENTAURA,
	DND_ELITEFX_TEMPORALBUBBLE,
	DND_ELITEFX_ENSHROUD,

	DND_ELITEFX_WARDAURA,

	DND_SPECIALFX_ASMODEUSCIRCLE,

	DND_ATTACHMENT_PETICON,
	DND_ATTACHMENT_STUNICON,
	DND_ATTACHMENT_FLAMMABILITY,
	DND_ATTACHMENT_SNARE,
};

Script "DND Spawn Attachment" (int tid, int which) CLIENTSIDE {
	// non-zero hopefully
	int res = 0;
	if(tid) {
		//Delay(const:1);

		int i;
		int zoff = GetActorProperty(tid, APROP_HEIGHT) >> 1;
		int xoff = GetActorProperty(tid, APROP_RADIUS);
		if(xoff > 32.0)
			xoff -= 4.0;

		switch(which) {
			case DND_ELITEFX_REFLECT:
				for(i = 0; i < 3; ++i)
					CreateMonsterAttachment(tid, "EliteReflectingShield", xoff, 0, zoff, i * 1.0 / 3);
			break;
			case DND_ELITEFX_CRIPPLEAURA:
				res = CreateMonsterAttachment(tid, "CrippleAuraFX");
			break;
			case DND_ELITEFX_VIOLENTAURA:
				res = CreateMonsterAttachment(tid, "ViolentAuraFX");
			break;
			case DND_ELITEFX_TEMPORALBUBBLE:
				res = CreateMonsterAttachment(tid, "TemporalBubbleFX", 0, 0, zoff);
			break;
			case DND_ELITEFX_ENSHROUD:
				for(i = 0; i < 7; ++i)
					CreateMonsterAttachment(tid, "EnshroudFX", xoff, 0, zoff);
			break;
			
			case DND_ELITEFX_WARDAURA:
				res = CreateMonsterAttachment(tid, "WardAuraFX");
			break;

			case DND_SPECIALFX_ASMODEUSCIRCLE:
				res = CreateMonsterAttachment(tid, "AsmodeusAuraFX");
			break;

			case DND_ATTACHMENT_PETICON:
				zoff <<= 1;
                zoff += 10.0;
				res = CreateMonsterAttachment(tid, "DnD_PetIcon", xoff, 0, zoff);
			break;
			case DND_ATTACHMENT_STUNICON:
				zoff <<= 1;
                zoff += 12.0;
				res = CreateMonsterAttachment(tid, "StunFXMarker", 0, 0, zoff);
			break;
			case DND_ATTACHMENT_FLAMMABILITY:
				zoff <<= 1;
				zoff += 4.0;
				res = CreateMonsterAttachment(tid, "FlammabilityFXMarker", 0, 0, zoff);
			break;
			// Around the feet: this one is a shackle, not an overhead icon. The actor warps itself to
			// a fixed 16 up every tic, so this only decides where it first appears.
			case DND_ATTACHMENT_SNARE:
				res = CreateMonsterAttachment(tid, "SearingBondAttachmentFX", 0, 0, 16.0);
			break;

			// normal elite sparkles
			default:
				// spawn 3 with 120 degree increments on them
				for(i = 0; i < 3; ++i)
					CreateMonsterAttachment(tid, "EliteSpecialFX", xoff, 0, zoff, i * 1.0 / 3);
			break;
		}
	}
	SetResultValue(res);
}

// No reset lives here on purpose.
//
// There used to be a per monster one, dispatched from "DnD Monster Scale". That was a
// Zandronum trap: this bitfield is written by CLIENTSIDE scripts, so it lives in the CLIENT's
// copy of these statics, while Monster Scale runs on the SERVER. The reset it dispatched
// arrived AFTER the attachment spawns queued earlier in the same run and cleared a field that
// already held live entries. DisposeAttachments finds actors only through these bits, so
// every sparkle and reflect shield stayed welded to the corpse forever.
//
// Nothing replaces it. MonsterAttachmentUsed is a plain static, not one of the "global N:"
// arrays that survive a map change, so the engine zeroes it on every map load already.
// Within a map, DisposeAttachments shifts a slot down to 0 as it walks, so a monster that
// dies leaves its slot clean for the next m_id to reuse.

// do not send tid here, send monster id (tid - DND_MONSTERTID_BEGIN)
int CreateMonsterAttachment(int tid, str actor_name, int xoff = 0, int yoff = 0, int zoff = 0, int angle = 0) {
	// base tid skip
	int sfx_id = 0;
	int m_id = tid - DND_MONSTERTID_BEGIN;
	auto attachment_data = GetMonsterAttachmentsUsed(m_id);
	int temp = attachment_data.val;
	while(temp & 1) {
		temp >>= 1;
		++sfx_id;
	}

	// don't go over the bit limit
	if(sfx_id > 31)
		return 0;

	attachment_data.val |= 1 << sfx_id;

	//printbold(s:"give attachment to id ", d:sfx_id, s:" val: ", d:attachment_data.val);

	// offset to tid
	temp = sfx_id + DND_MONSTER_ATTACHMENT_TID_BEGIN + m_id * DND_MAX_MONSTER_ATTACHMENTS;
	SpawnForced(actor_name, GetActorX(tid) + xoff, GetActorY(tid) + yoff, GetActorZ(tid) + zoff, temp, angle);

	// setup the attachment
	SetActivator(temp);
	SetPointer(AAPTR_TARGET, tid);
	SetActorProperty(temp, APROP_TARGETTID, tid);

	// radius and other things
	SetActorProperty(temp, APROP_MASS, zoff >> 16);
	SetActorProperty(temp, APROP_SCORE, xoff >> 16);

	SetActivator(tid);

	return sfx_id;
}

void RemoveAttachment(int m_id, int sfx_id, bool wantStateChange = true) {
	auto attachment_data = GetMonsterAttachmentsUsed(m_id);
	int temp = attachment_data.val;
	int id_count = 0;
	int base = DND_MONSTER_ATTACHMENT_TID_BEGIN + m_id * DND_MAX_MONSTER_ATTACHMENTS;

	while(temp) {
		if((temp & 1) && id_count == sfx_id) {
			if(wantStateChange)
				SetActorState(sfx_id + base, "Disappear");
			attachment_data.val &= ~(1 << sfx_id);
			break;
		}
		++id_count;
		temp >>= 1;
	}
}

Script "DnD Remove Monster Attachment" (int tid, int sfx_id) CLIENTSIDE {
	RemoveAttachment(tid - DND_MONSTERTID_BEGIN, sfx_id);
	SetResultValue(0);
}

Script "DnD Remove Blind FX Count" (void) CLIENTSIDE {
	SetActivatorToTarget(0);
	
	int this = ActivatorTID();
	RemoveAttachment(this - DND_MONSTERTID_BEGIN, CheckInventory("DnD_BlindFXToRemove") - 1, false);
	SetInventory("DnD_BlindFXToRemove", 0);
	SetResultValue(0);
}

// ---- player attachments -------------------------------------------------------------------------
//
// Deliberately NOT the monster system above. That one hands out tids from a band and tracks them in
// a bitfield, which works because a monster tid means the same thing on both sides. A PLAYER tid is
// assigned server side only, so a client cannot resolve one and nothing clientside could aim a warp
// at it.
//
// So a player attachment is carried as INVENTORY instead. Inventory crosses the network on its own,
// the marker is spawned by the player's OWN decorate on every machine that has it, and the effect
// tears itself down when the marker goes. No tid band, no bitfield, nothing per side to keep in step.
enum {
	DND_PLAYERFX_ANGERAURA,
	DND_PLAYERFX_HEATSHIELD,
	DND_PLAYERFX_BOILINGBLOOD,
	DND_PLAYERFX_IMMOLATION,
	DND_PLAYERFX_ALLYLOCK,
};

// How far out to either side a paired attachment sits, past a player's own 16 unit radius.
#define DND_PLAYERFX_SIDEDIST 24.0

// The item whose presence IS the attachment. The FX watches it and stops when it goes.
str GetPlayerAttachmentMarker(int which) {
	switch(which) {
		case DND_PLAYERFX_ANGERAURA: return "DnD_AngerAuraActive";
		// Owned by the spell, not by this system: the rank stamp already marks exactly when the
		// shield is up, and on the WEARER rather than the caster.
		case DND_PLAYERFX_HEATSHIELD: return "DnD_HeatShieldRank";
		case DND_PLAYERFX_BOILINGBLOOD: return "DnD_BoilingBloodActive";
		case DND_PLAYERFX_IMMOLATION: return "DnD_ImmolationActive";
		case DND_PLAYERFX_ALLYLOCK: return "DnD_AllyLockActive";
	}
	return "";
}

// The token that spawns it. A DnD_Activator, so giving it runs its Pickup state and nothing lingers.
str GetPlayerAttachmentSpawner(int which) {
	switch(which) {
		case DND_PLAYERFX_ANGERAURA: return "Spell_AngerAura_FXSpawner";
		case DND_PLAYERFX_HEATSHIELD: return "Spell_HeatShield_FXSpawner";
		case DND_PLAYERFX_BOILINGBLOOD: return "Spell_BoilingBlood_FXSpawner";
		case DND_PLAYERFX_IMMOLATION: return "Spell_Immolation_FXSpawner";
		case DND_PLAYERFX_ALLYLOCK: return "Spell_AllyLock_FXSpawner";
	}
	return "";
}

str GetPlayerAttachmentFX(int which) {
	switch(which) {
		case DND_PLAYERFX_ANGERAURA: return "Spell_AngerAuraFX";
		case DND_PLAYERFX_HEATSHIELD: return "Spell_HeatShieldFX";
		case DND_PLAYERFX_BOILINGBLOOD: return "Spell_BoilingBloodFX";
		case DND_PLAYERFX_IMMOLATION: return "Spell_ImmolationFX";
		case DND_PLAYERFX_ALLYLOCK: return "Spell_AllyLockFX";
	}
	return "";
}

// How many copies, and whether they sit to either side. One is the default -- an aura is a single
// thing under the player; Heat Shield is a pair, one on each flank.
int GetPlayerAttachmentSides(int which) {
	switch(which) {
		case DND_PLAYERFX_HEATSHIELD: return 2;
	}
	return 1;
}

// Called BY the token's Pickup state, so the activator is the player wearing it. Built exactly like
// "DnD Wanderer Return Circle", which is the one player attached aura in the mod known to work:
// spawn onto a scratch tid, point it at the owner, release the tid.
//
// It must be reached with ACS_NamedExecuteWithResult. That call runs inline and keeps the actor as
// activator; ACS_NamedExecuteAlways starts a detached script and the activator is lost, which is
// exactly how this fails -- ActivatorTID() comes back 0 and the aura is pointed at nothing.
Script "DnD Spawn Player Aura" (int which) CLIENTSIDE {
	int tid = ActivatorTID();
	str fx = GetPlayerAttachmentFX(which);
	if(!tid || fx == "") {
		SetResultValue(0);
		Terminate;
	}

	// Offsets ride APROP_MASS and APROP_SCORE rather than user vars, the same way the monster
	// attachments carry theirs -- DECORATE reads them straight out of A_Warp.
	int zoff = GetActorProperty(tid, APROP_HEIGHT) >> 1;
	int pnum = PlayerNumber();
	int sides = GetPlayerAttachmentSides(which);
	int i, side, f = 1.0;

	// Grown by the caster's area modifiers, off whatever scale the actor declares in DECORATE, so the
	// art keeps its own size and this only multiplies it. Passing 1.0 as the radius makes
	// ScalePlayerAoERadius hand back the factor itself rather than a distance.
	if(pnum >= 0)
		f = ScalePlayerAoERadius(pnum, 1.0, DND_AOESRC_NONWEAPON);

	for(i = 0; i < sides; ++i) {
		if(!SpawnForced(fx, GetActorX(tid), GetActorY(tid), GetActorZ(tid), DND_PLAYERAURA_TID, 0))
			continue;

		SetActivator(DND_PLAYERAURA_TID);
		SetPointer(AAPTR_TARGET, tid);
		SetActorProperty(DND_PLAYERAURA_TID, APROP_TARGETTID, tid);

		// A single attachment sits centred; a pair is pushed out to either flank.
		side = 0;
		if(sides > 1)
			side = i ? -DND_PLAYERFX_SIDEDIST : DND_PLAYERFX_SIDEDIST;

		SetActorProperty(DND_PLAYERAURA_TID, APROP_MASS, zoff >> 16);
		SetActorProperty(DND_PLAYERAURA_TID, APROP_SCORE, side >> 16);

		if(f != 1.0) {
			SetActorProperty(DND_PLAYERAURA_TID, APROP_SCALEX,
				FixedMul(GetActorProperty(DND_PLAYERAURA_TID, APROP_SCALEX), f));
			SetActorProperty(DND_PLAYERAURA_TID, APROP_SCALEY,
				FixedMul(GetActorProperty(DND_PLAYERAURA_TID, APROP_SCALEY), f));
		}

		// Released straight away -- the next copy takes the same scratch tid.
		Thing_ChangeTID(0, 0);
		SetActivator(tid);
	}

	SetResultValue(0);
}

// For an attachment whose marker belongs to the spell rather than to this system. Nothing here
// writes the marker -- the caller has already decided the effect is up, and the FX watches it.
void RaisePlayerAttachment(int pnum, int which) {
	GiveActorInventory(pnum + P_TIDSTART, GetPlayerAttachmentSpawner(which), 1);
}

// Idempotent both ways, so it can be called every tick from whatever already knows the state.
void SetPlayerAttachment(int pnum, int which, bool on) {
	int ptid = pnum + P_TIDSTART;
	str marker = GetPlayerAttachmentMarker(which);
	if(marker == "")
		return;

	if(!on) {
		TakeActorInventory(ptid, marker, 1);
		return;
	}

	// Already up. Re-issuing the token would stack a second effect on one marker.
	if(CheckActorInventory(ptid, marker))
		return;

	GiveActorInventory(ptid, marker, 1);
	GiveActorInventory(ptid, GetPlayerAttachmentSpawner(which), 1);
}

// When a monster is killed this is called to do cleanup
void DisposeAttachments(int m_id) {
	auto attachment_data = GetMonsterAttachmentsUsed(m_id);
	// if theres any attachment
	//Log(s:" attachment used? ", d:attachment_data.val, s: " ", d:m_id);
	if(attachment_data.val) {
		int count = 0;
		int base = DND_MONSTER_ATTACHMENT_TID_BEGIN + m_id * DND_MAX_MONSTER_ATTACHMENTS;
		while(attachment_data.val) {
			if(attachment_data.val & 1)
				SetActorState(count + base, "Disappear");

			attachment_data.val >>= 1;
			++count;
		}
	}
}

#endif