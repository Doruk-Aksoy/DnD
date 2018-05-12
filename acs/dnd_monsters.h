#ifndef DND_MONSTERS_IN
#define DND_MONSTERS_IN

#include "DnD_Common.h"
#include "DnD_Orbs.h"
#include "DnD_Stat.h"


#define AVATAR_SOUL_TID 2000
#define AVATAR_CUBE_TID 6000
#define AVATAR_SOUL_COUNT 10

#define WAIT_MAX 18 // approx 2.5 seconds of flight max for jumping monsters (or lands on object and gets stuck, this is to unstuck it)

#define ZEALOT_SHIELD_TIME 4 * 35
#define ZEALOT_SHIELD_TIDADD 1000 

#define DARKLICH_WARP_RANDMAX 160
#define DARKLICH_WARP_VELZMAX 12.0

#define FLESHWIZARD_PERCENT 10
#define FLESHWIZARD_TETHERRANGE_MIN 56.0
#define FLESHWIZARD_TETHERRANGE_MAX 256.0
#define FLESHWIZARD_TETHERRANGE_RADIUS 64.0
#define FLESHWIZARD_DAMAGE_PERIOD 17

#define DND_TORRASQUE_MACEID 800

int GetHealthCapActor(int t) {
	return DND_VIT_INCREASE * CheckActorInventory(t, "PSTAT_Vitality");
}

int GetHealthMaxActor(int t) {
    return 100 + GetHealthCapActor(t);
}

#define MAX_DND_COLLIDERS 1
str Colliders[MAX_DND_COLLIDERS] = {
	"GodSlayerCollider"
};

void SetXYSpeed(int speed) {
	SetActorVelocity(0, speed * cos(GetActorAngle(0)), speed * sin(GetActorAngle(0)), GetActorVelZ(0), 0, 0);
}

void AddXYSpeed(int speed) {
	SetActorVelocity(0, speed * cos(GetActorAngle(0)), speed * sin(GetActorAngle(0)), 0, 1, 0);
}

void TryChangeDirection() {
	int i, j;
	for(i = random(1, 7), j = 0; j < 8; ++i, ++j)
		if(DoCollisionCheck(0, 72 * cos(0.125 * (i % 8)), 72 * sin(0.125 * (i % 8)), 0.0)) {
			break;
	}
	i = 0.125 * (i % 8) + random(-0.025, 0.025);
	SetActorAngle(0, GetActorAngle(0) + i);
}

int DoCollisionCheck(int collider, int xoff, int yoff, int zoff) {
	return Spawn(Colliders[collider], GetActorX(0) + FixedMul(xoff, cos(GetActorAngle(0))), GetActorY(0) + FixedMul(yoff, sin(GetActorAngle(0))), GetActorZ(0) + zoff);
}

#endif