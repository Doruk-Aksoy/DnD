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

			// PLACEHOLDER ART: reuses the Asmodeus ring so the aura is visible in game at all. Wants
			// its own sprite before this ships.
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