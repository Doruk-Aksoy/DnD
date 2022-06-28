#ifndef DND_PHYS_IN
#define DND_PHYS_IN

#define DND_DMGPUSH_CAP 72.0

void HandleDamagePush(int dmg, int ox, int oy, int oz, int victim, bool oneTimeRipperHack = false) {
	// if not to be pushed, ignore
	if(CheckFlag(victim, "NODAMAGETHRUST"))
		return;

	// get push vector
	int dx, dy, dz;
	int m = GetActorProperty(victim, APROP_MASS) / 2;
	if(!m)
		m = 1;
	
	if(!oneTimeRipperHack) {
		dx =  GetActorX(victim) - ox;
		dy =  GetActorY(victim) - oy;
		dz =  GetActorZ(victim) + GetActorProperty(victim, APROP_HEIGHT) / 2 + 8.0 - oz;
	}
	else {
		dx = ox;
		dy = oy;
		dz = oz;
	}
	
	int len = magnitudeThree(dx >> 16, dy >> 16, dz >> 16);
	if(!len)
		len = 1;
		
	dx /= len;
	dy /= len;
	dz /= len;
	
	dx *= dmg;		dx /= m;
	dy *= dmg;		dy /= m;
	dz *= dmg;		dz /= m;
	
	//printbold(s:"impulse: ", d:dmg, s:", force vector: ", f:dx, s:" ", f:dy, s: " ", f:dz);
	dx = Clamp_Between(dx, -DND_DMGPUSH_CAP, DND_DMGPUSH_CAP);
	dy = Clamp_Between(dy, -DND_DMGPUSH_CAP, DND_DMGPUSH_CAP);
	dz = Clamp_Between(dz, -DND_DMGPUSH_CAP, DND_DMGPUSH_CAP);
	
	SetActorVelocity(victim, dx, dy, dz, true, false);
}

// player tid, monster tid, force on xy, force on z
void PushPlayerAway(int tid, int mtid, int f_xy, int f_z, bool z_AlwaysUp = false) {
	// angle to make monster face player
	int a = AngleToFace(mtid, tid);
	
	// push power is scaled with monster level by 5% per level -- one would need at least 125 * 100 = 12,500 knockback resist to be immune to this vs a monster of lvl 1
	// vs a monster of level 100 you'd need 125 * 500 = 725,000 knockback resist
	int m = 100 + 4 * MonsterProperties[mtid - DND_MONSTERTID_BEGIN].level - GetActorProperty(tid, APROP_MASS) / 125;
	if(m < 0)
		m = 0;
	
	// z dir is determined simply by sign of z_diff -- f_z is int
	int dz = z_AlwaysUp ? 1 : sign(GetActorZ(tid) + GetActorProperty(tid, APROP_HEIGHT) / 2 - GetActorZ(mtid) - GetActorProperty(mtid, APROP_HEIGHT) / 2);
	f_z <<= 16;
	
	SetActorVelocity(
		tid,
		f_xy * cos(a) * m / 100,
		f_xy * sin(a) * m / 100,
		f_z * dz * m / 100,
		true,
		false
	);
}

// height, bounding box top and bottom coords, dir vectors x,y,z, actor x,y,radius, actor tid
bool CheckProjectileCollision(int h, int top_x, int top_y, int bot_x, int bot_y, int dir_x, int dir_y, int dir_z, int a_x, int a_y, int a_r, int i) {
	// eliminate cases where it'd fail to touch
	/*if(GetActorClass(i) == "DoomImp2")
		printbold(
			s:"step ", d:s, s:" ",
			f:top_x - s * dir_x, s: " > ", f:a_x + a_r, s: " = ", d:top_x - s * dir_x > a_x + a_r, s: " ",
			f:bot_x - s * dir_x, s: " < ", f:a_x - a_r, s: " = ", d:bot_x - s * dir_x < a_x - a_r, s: " ",
			f:bot_y - s * dir_y, s: " > ", f:a_y + a_r, s: " = ", d:bot_y - s * dir_y > a_y + a_r, s: " ",
			f:top_y - s * dir_y, s: " < ", f:a_y - a_r, s: " = ", d:top_y - s * dir_y < a_y - a_r
		);
	*/
	if
	(
		top_x - dir_x > a_x + a_r ||
		bot_x - dir_x < a_x - a_r ||
		bot_y - dir_y > a_y + a_r ||
		top_y - dir_y < a_y - a_r
	)
		return true;
	
	//printbold(s:"x-y valid on: ", f:a_x, s: " ", f:a_y, s:" ", f:top_x + s * dir_x * r, s: " ", f:top_y + s * dir_y * r, s: " ", f:bot_x + s * dir_x * r, s: " ", f:bot_y + s * dir_y * r);
	/*printbold(d:s, s: " ", f:s * dir_z);
	printbold(
		s:"try z: ", f:GetActorZ(0) - s * dir_z, s: " > ", f:GetActorZ(i) + GetActorProperty(i, APROP_HEIGHT), s: " ",
		f:GetActorZ(0) + h - s * dir_z, s: " < ", f:GetActorZ(i), s: " || ", d:!CheckSight(0, i, 0)
	);*/
	return GetActorZ(0) - dir_z > GetActorZ(i) + GetActorProperty(i, APROP_HEIGHT) || GetActorZ(0) + h - dir_z < GetActorZ(i) || !CheckSight(0, i, 0);
}


#endif

