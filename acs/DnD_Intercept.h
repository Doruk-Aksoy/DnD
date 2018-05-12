#ifndef DND_INTERCEPT_IN
#define DND_INTERCEPT_IN

// old attempt of mine with intercept, not precise enough (1st iteration)
/*
	int target = GetActorProperty(0, APROP_TARGETTID);
	int tx = GetActorX(target) - GetActorX(0);
	int ty = GetActorY(target) - GetActorY(0);
	int tz = GetActorZ(target) + GetActorProperty(target, APROP_HEIGHT);
	int dist = fdistance(target, 0);
	int counter = 0;
	int time = dist / speed;
	// aim at player spot ahead in time
	while(counter < 3) {
		tx += FixedMul(GetActorVelX(target), time) / 3;
		ty += FixedMul(GetActorVelY(target), time) / 3;
		tz += FixedMul(GetActorVelZ(target), time) / 3;
		Delay(1);
		++counter;
	}
	
	SetActorPosition(0, GetActorX(0) +  24 * cos(GetActorAngle(0)), GetActorY(0), GetActorZ(0) + 48.0, 0);
	SpawnProjectile(0, "TorrasqueMace", VectorAngle(tx, ty) >> 8, speed * 8, 0, 0, DND_TORRASQUE_MACEID);
	SetActorPosition(0, GetActorX(0) -  24 * cos(GetActorAngle(0)), GetActorY(0), GetActorZ(0) - 48.0, 0);
	
	// calculate pitch
	tz -= GetActorZ(0) + GetActorProperty(0, APROP_HEIGHT);
	//tz = FixedDiv(tz, 256.0);
	tz = VectorAngle(dist, tz);
	
	SetActorVelocity(DND_TORRASQUE_MACEID, GetActorVelX(DND_TORRASQUE_MACEID), GetActorVelY(DND_TORRASQUE_MACEID), speed * sin(tz), 0, 0);
	Thing_ChangeTID(DND_TORRASQUE_MACEID, 0);
*/

int sq(int x) {
	return FixedMul(x, x);
}

int FixedAngMod(int fAngle) {
	if (fAngle > 1.0){
		fAngle %= 65536; 
	}
	else if (fAngle < 0) {
		fAngle %= (-65536);
		fAngle = fAngle + 65536;
	}
	return fAngle;
}

int ProjInt_Brute(int spd, int ptid, int xoff, int yoff, int zoff, str ptype, int axoff, int ayoff, int azoff, int angoff, bool rand, int input_t) {
	int sX, sY, sZ, s_ang, p_ang;
	int tX, tY, tZ, tVelX, tVelY, tVelZ;
	int X_spd, Y_spd, Z_spd;
	
	bool check = 1, check2 = 1;
	int target = GetActorProperty(0, APROP_TARGETTID);
	int temp = 0, temp2 = 0;

	sX = GetActorX(0);
	sY = GetActorY(0);
	sZ = GetActorZ(0);
	s_ang = GetActorAngle(0);

	sZ += zoff;

	if(xoff > 0) {
		sX += FixedMul(cos(FixedAngMod(s_ang - 0.25)), xoff); 
		sY += FixedMul(sin(FixedAngMod(s_ang - 0.25)), xoff); 
	}
	else if(xoff < 0) {
		sX += FixedMul(cos(FixedAngMod(s_ang + 0.25)), xoff); 
		sY += FixedMul(sin(FixedAngMod(s_ang + 0.25)), xoff); 
	}

	if(yoff > 0) {
		sX += FixedMul(cos(s_ang), yoff); 
		sY += FixedMul(sin(s_ang), yoff); 
	}
	else if(yoff < 0) {
		sX -= FixedMul(cos(s_ang), yoff); 
		sY -= FixedMul(sin(s_ang), yoff); 
	}

	tX = GetActorX(target);
	tY = GetActorY(target);
	tZ = GetActorZ(target);
	tZ += 36.0;
	//tZ += 26.5; for comparison with Thing_ProjectileIntercept

	tVelX = GetActorVelX(target);
	tVelY = GetActorVelY(target);
	tVelZ = GetActorVelZ(target);

	if(!CheckFlag(target, "NOGRAVITY")) 
		tVelZ = 0;

	zoff = 0; // sml_t
	xoff = 0; // t
	yoff = 0; // nmax
	Y_spd = 0; // i
	while(check) {
		xoff = zoff - (temp / 2);
		if(!Y_spd){
			xoff = 1.0;
			temp = 10.0;
			yoff = 40; 
		}
		else if(Y_spd == 1) {
			if(zoff > 400.0) 
				check = 0; //stop trying to get closer if it's out of range, it's just a waste
			else {
				temp = 1.0;
				yoff = 10; 
			}
		}  
		else if(Y_spd == 2) {
			temp = 0.1;
			yoff = 10; 
		}       
		else if(Y_spd == 3) {
			temp = 0.01;
			yoff = 10; 
		}             
		else if(Y_spd == 4) {
			temp = 0.001;
			yoff = 10; 
		}           
		else if(Y_spd == 5) {
			temp = 1;
			yoff = 66; 
		}         
		else if(Y_spd == 6) 
			check = 0;      
		++Y_spd;
		temp2 = 0;
		check2 = 1; 

		while(check && check2) {
			target = abs(spd - VectorLength(VectorLength(FixedDiv(tX + FixedMul(xoff, tVelX) - sx, xoff), FixedDiv(tY + FixedMul(xoff, tVelY) - sy, xoff)), FixedDiv(FixedMul(xoff, tVelZ), xoff)));
			if((target < X_spd) || temp2 == 0) {
				X_spd = target;
				zoff = xoff; 
			}
			++temp2;
			xoff += temp;
			if(temp2 > yoff) 
				check2 = 0; 
		}
	}

	if(input_t) 
		xoff = input_t; 
	else {
		if(rand) 
			random(1, zoff);
		else 
			xoff = zoff;
	}
	
	temp = tX + FixedMul(xoff, tVelX);
	yoff = tY + FixedMul(xoff, tVelY);
	if(rand || input_t) {
		Z_spd = FixedDiv(tZ + FixedMul(xoff, tVelZ) - sZ, zoff);
		p_ang = VectorAngle(temp - sX, yoff - sY);
		temp2 = FixedSqrt(sq(spd) - sq(Z_spd));
		X_spd = FixedMul(cos(p_ang), temp2);
		Y_spd = FixedMul(sin(p_ang), temp2); 
	}
	else {
		Z_spd = FixedDiv(tZ + FixedMul(xoff, tVelZ) - sZ, xoff);
		X_spd = FixedDiv(temp - sX, xoff);
		Y_spd = FixedDiv(yoff - sY, xoff);
		p_ang = VectorAngle(temp - sX, yoff - sY); 
	}

	SetActorAngle(0, p_ang);

	sZ += azoff;

	if(axoff > 0) {
		sX -= FixedMul(cos(FixedAngMod(s_ang - 0.25)), axoff); 
		sY -= FixedMul(sin(FixedAngMod(s_ang - 0.25)), axoff); 
	}
	else if(axoff < 0) {
		sX += FixedMul(cos(FixedAngMod(s_ang + 0.25)), axoff); 
		sY += FixedMul(sin(FixedAngMod(s_ang + 0.25)), axoff); 
	}

	if(ayoff > 0) {
		sX += FixedMul(cos(s_ang), ayoff); 
		sY += FixedMul(sin(s_ang), ayoff); 
	}
	else if(ayoff < 0) {
		sX -= FixedMul(cos(s_ang), ayoff); 
		sY -= FixedMul(sin(s_ang), ayoff); 
	}

	if(angoff != 0) {
		p_ang = FixedAngMod(p_ang + angoff);
		temp2 = FixedSqrt(sq(X_spd) + sq(Y_spd));
		X_spd = FixedMul(cos(FixedAngMod(p_ang + angoff)), temp2);
		Y_spd = FixedMul(sin(FixedAngMod(p_ang + angoff)), temp2); 
	}

	temp = ActivatorTID();
	target = GetActorProperty(0, APROP_TARGETTID);
	if(!ptid)
		ptid = 131072;
	SpawnProjectile (0, ptype, 0, 0, 0, 0, ptid); 
	SetActivator(ptid); 
	SetPointer(AAPTR_TARGET, temp); // so doesn't collide with it
	SetPointer(AAPTR_TRACER, target);
	SetActorPosition(ptid, sX, sY, sZ, 0);
	SetActorAngle(ptid, p_ang);
	SetActorVelocity(ptid, X_spd, Y_spd, Z_spd, 0, 0);
	Thing_ChangeTID(ptid, 0);
	SetActivator(temp);
	return xoff;
}

#endif