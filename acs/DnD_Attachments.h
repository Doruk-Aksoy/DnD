#ifndef DND_ATTACHMENT_IN
#define DND_ATTACHMENT_IN

// max 32, using an int as a bitfield
global int 49: MonsterAttachmentUsed[DND_MAX_MONSTERS];

enum {
	DND_ELITEFX_REFLECT = 1,
	DND_ELITEFX_CRIPPLEAURA,
	DND_ELITEFX_VIOLENTAURA,
	DND_ELITEFX_TEMPORALBUBBLE,
	DND_ELITEFX_ENSHROUD,

	DND_ATTACHMENT_PETICON
};

Script "DND Spawn Attachment" (int tid, int which) CLIENTSIDE {
	// non-zero hopefully
	if(tid) {
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
				CreateMonsterAttachment(tid, "CrippleAuraFX");
			break;
			case DND_ELITEFX_VIOLENTAURA:
				CreateMonsterAttachment(tid, "ViolentAuraFX");
			break;
			case DND_ELITEFX_TEMPORALBUBBLE:
				CreateMonsterAttachment(tid, "TemporalBubbleFX", 0, 0, zoff);
			break;
			case DND_ELITEFX_ENSHROUD:
				for(i = 0; i < 7; ++i)
					CreateMonsterAttachment(tid, "EnshroudFX", xoff, 0, zoff);
			break;

			case DND_ATTACHMENT_PETICON:
				zoff <<= 1;
                zoff += 10.0;
				CreateMonsterAttachment(tid, "DnD_PetIcon", xoff, 0, zoff);
			break;

			// normal elite sparkles
			default:
				// spawn 3 with 120 degree increments on them
				for(i = 0; i < 3; ++i)
					CreateMonsterAttachment(tid, "EliteSpecialFX", xoff, 0, zoff, i * 1.0 / 3);
			break;
		}
	}
}

// do not send tid here, send monster id (tid - DND_MONSTERTID_BEGIN)
void CreateMonsterAttachment(int tid, str actor_name, int xoff = 0, int yoff = 0, int zoff = 0, int angle = 0) {
	// base tid skip
	int sfx_id = 0;
	int m_id = tid - DND_MONSTERTID_BEGIN;
	int temp = MonsterAttachmentUsed[m_id];
	while(temp & 1) {
		temp >>= 1;
		++sfx_id;
	}
	MonsterAttachmentUsed[m_id] |= 1 << sfx_id;

	// offset to tid
	sfx_id += DND_MONSTER_ATTACHMENT_TID_BEGIN + m_id * DND_MAX_MONSTER_ATTACHMENTS;
	//Log(s:"sfx tid: ", d:sfx_id, s: " for monster tid: ", d:tid);
	SpawnForced(actor_name, GetActorX(tid) + xoff, GetActorY(tid) + yoff, GetActorZ(tid) + zoff, sfx_id, angle);

	// setup the attachment
	SetActivator(sfx_id);
	SetPointer(AAPTR_TARGET, tid);
	SetActorProperty(sfx_id, APROP_TARGETTID, tid);

	// radius and other things
	SetActorProperty(sfx_id, APROP_MASS, zoff >> 16);
	SetActorProperty(sfx_id, APROP_SCORE, xoff >> 16);

	SetActivator(tid);
}

// When a monster is killed this is called to do cleanup
void DisposeAttachments(int m_id) {
	// if theres any attachment
	if(MonsterAttachmentUsed[m_id]) {
		int count = 0;
		int base = DND_MONSTER_ATTACHMENT_TID_BEGIN + m_id * DND_MAX_MONSTER_ATTACHMENTS;
		while(MonsterAttachmentUsed[m_id]) {
			if(MonsterAttachmentUsed[m_id] & 1)
				SetActorState(count + base, "Disappear");

			MonsterAttachmentUsed[m_id] >>= 1;
			++count;
		}
	}
}

#endif