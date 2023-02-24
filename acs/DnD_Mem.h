#ifndef DND_MEM_IN
#define DND_MEM_IN

#define MAX_VECTORS 1024

/*
---- GENERAL IDEA OF HOW THIS WORKS ----

	* When a vector is requested, one of the indices from a global array will be returned for use, that is not marked busy
	* The global array is referred to as "vec2" and "vec3" to make it look prettier... somewhat. These are the caches.
	* Users will have to free a vector after using
	* A vector will be the index in the cache. All vector functions will require these indices.
	* Behind the scenes the math will be done properly, but will use the indices.
	* Result of all math functions is an index to a vector from the cache.
*/

global Vec2_T 59: vec2[MAX_VECTORS];
global Vec3_T 60: vec3[MAX_VECTORS];

bool vec2_busylist[MAX_VECTORS];
bool vec3_busylist[MAX_VECTORS];

int GetUnusedVec2() {
	// find unused slot
	int i;
	for(i = 0; i < MAX_VECTORS && vec2_busylist[i]; ++i);
	
	// mark as busy
	vec2_busylist[i] = true;
	
	return i;
}

int GetVec2(int x = 0, int y = 0) {
	int i = GetUnusedVec2();

	vec2[i].x = x;
	vec2[i].y = y;
	
	return i;
}

int CopyVec2(int vec_id) {
	int i = GetUnusedVec2();
	
	vec2[i].x = vec2[vec_id].x;
	vec2[i].y = vec2[vec_id].y;
	
	return i;
}

void FreeVec2(int id) {
	vec2[id].x = 0;
	vec2[id].y = 0;
	vec2_busylist[id] = false;
}

int GetUnusedVec3() {
	// find unused slot
	int i;
	for(i = 0; i < MAX_VECTORS && vec3_busylist[i]; ++i);
	
	// mark as busy
	vec3_busylist[i] = true;
	
	return i;
}

int GetVec3(int x = 0, int y = 0, int z = 0) {
	int i = GetUnusedVec3();

	vec3[i].x = x;
	vec3[i].y = y;
	vec3[i].z = z;
	
	return i;
}

int CopyVec3(int vec_id) {
	int i = GetUnusedVec3();
	
	vec3[i].x = vec3[vec_id].x;
	vec3[i].y = vec3[vec_id].y;
	vec3[i].z = vec3[vec_id].z;
	
	return i;
}

void FreeVec3(int id) {
	vec3[id].x = 0;
	vec3[id].y = 0;
	vec3[id].z = 0;
	vec3_busylist[id] = false;
}

#endif
