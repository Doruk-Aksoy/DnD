#ifndef DND_PHYS_IN
#define DND_PHYS_IN

#define DND_DMGPUSH_CAP 72.0
#define DND_DMGPUSH_MASSCAP 32768

void HandleDamagePush(int dmg, int ox, int oy, int oz, int victim, bool oneTimeRipperHack = false) {
	// if not to be pushed, ignore -- but push players by their own self dmg!
	if(CheckFlag(victim, "NODAMAGETHRUST") && !IsPlayer(victim))
		return;

	// get push vector
	int m = GetActorProperty(victim, APROP_MASS) / 4;
	if(m > DND_DMGPUSH_MASSCAP)
		m = DND_DMGPUSH_MASSCAP;
	else if(!m)
		m = 1;
	
	if(!oneTimeRipperHack) {
		ox =  GetActorX(victim) - ox;
		oy =  GetActorY(victim) - oy;
		oz =  GetActorZ(victim) + GetActorProperty(victim, APROP_HEIGHT) / 2 + 8.0 - oz;
	}
	/*else {
		dx = ox;
		dy = oy;
		dz = oz;
	}*/
	
	int len = magnitudeThree(ox >> 16, oy >> 16, oz >> 16);
	if(!len)
		len = 1;
		
	ox /= len;
	oy /= len;
	oz /= len;
	
	ox *= dmg;		ox /= m;
	oy *= dmg;		oy /= m;
	oz *= dmg;		oz /= m;
	
	//printbold(s:"impulse: ", d:dmg, s:", force vector: ", f:dx, s:" ", f:dy, s: " ", f:dz);
	ox = Clamp_Between(ox, -DND_DMGPUSH_CAP, DND_DMGPUSH_CAP);
	oy = Clamp_Between(oy, -DND_DMGPUSH_CAP, DND_DMGPUSH_CAP);
	oz = Clamp_Between(oz, -DND_DMGPUSH_CAP, DND_DMGPUSH_CAP);
	
	SetActorVelocity(victim, ox, oy, oz, true, false);
}

// player tid, monster tid, force on xy, force on z
void PushPlayerAway(int tid, int mtid, int f_xy, int f_z, bool z_AlwaysUp = false) {
	// angle to make monster face player
	int a = AngleToFace(mtid, tid);
	
	// push power is scaled with monster level by 2% per level -- one would need at least 50 * 100 = 5000 knockback resist to be immune to this vs a monster of lvl 1
	// vs a monster of level 100 you'd need 125 * 500 = 725,000 knockback resist
	int m = 100 + 2 * MonsterProperties[mtid - DND_MONSTERTID_BEGIN].level - GetActorProperty(tid, APROP_MASS) / 25;
	if(m < 0)
		m = 0;
	
	// z dir is determined simply by sign of z_diff -- f_z is int
	int dz = z_AlwaysUp ? 1 : sign(GetActorZ(tid) + GetActorProperty(tid, APROP_HEIGHT) / 2 - GetActorZ(mtid) - GetActorProperty(mtid, APROP_HEIGHT) / 2);
	
	// clamp the xy and z forces to be at most double repel force
	f_xy *= m;
	f_xy / 100;
	f_xy = Clamp_Between(f_xy, 0, REPEL_FORCE * 2);
	
	f_z *= m;
	f_z /= 100;
	
	// default is repel_force/2 on z
	f_z = Clamp_Between(f_z, 0, REPEL_FORCE);
	f_z <<= 16;
	
	SetActorVelocity(
		tid,
		f_xy * cos(a),
		f_xy * sin(a),
		f_z * dz,
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

