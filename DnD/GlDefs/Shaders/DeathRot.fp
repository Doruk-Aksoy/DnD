// Death and Decay's rotting ground on flat2.md3, bound to DDROT (masks: R veins, G their distance out, B their bruise).
// Brown-black rot creeping out from the middle with a ragged rim as the actor's alpha rises, violet veins, wet pocks.

uniform float timer;

// NO_FOG marks the colormap pass, whose base lump is the one that does not declare this.
#ifdef NO_FOG
varying vec4 pixelpos;
#endif

#define TAU       6.2831853
#define ALPHA_REF 0.1      // the actor's alpha when the rot has spread all the way
#define FRONT     0.8      // how far the front reaches when whole; its ragged rim bites RAG either way
#define RAG       0.2
#define FRESH     vec3(0.17, 0.10, 0.05)    // rot just reached: wet brown
#define OLD       vec3(0.03, 0.02, 0.015)   // long rotten: brown-black
#define ROTA_NEW  0.55     // how far the rot covers the floor at the rim, and where it is old
#define ROTA_OLD  0.93
#define VEIN      vec3(0.42, 0.10, 0.70)    // the faint violet veins, unlit
#define VEIN_GAIN 0.6
#define BRUISE    0.55     // how much darker the rot sinks round a vein
#define PULSE_RATE 0.28    // a glow runs out along the veins this often a second
#define POCK      44.0     // world units between wet pocks
#define POCK_RATE 0.22     // a pock bubbles this often a second, each on its own phase

// Rule-1 portable hash and value noise.
float dr_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float dr_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float v = 0.0;
	for(int c = 0; c < 8; c++) {
		vec3 o = vec3(mod(float(c), 2.0), mod(floor(float(c) * 0.5), 2.0), floor(float(c) * 0.25));
		vec3 w = mix(1.0 - f, f, o);
		v += w.x * w.y * w.z * dr_hash(i + o);
	}
	return v;
}

float dr_fbm(vec3 x) {
	return 0.55 * dr_noise(x) + 0.3 * dr_noise(x * 2.1 + 17.0) + 0.15 * dr_noise(x * 4.3 + 41.0);
}

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;
	float fade = clamp(color.a / ALPHA_REF, 0.0, 1.0);
	vec2 w = pixelpos.xz;      // world x, y: the rot keeps to the floor wherever the zone sits
	float t = mod(timer, 512.0);

	// The front creeps out from the middle; its rim is ragged, coarse lobes and finer notches.
	float rag = (dr_noise(vec3(w / 70.0, 3.0)) - 0.5) * 1.3 + (dr_noise(vec3(w / 23.0, 9.0)) - 0.5) * 0.7;
	float front = FRONT * fade;
	float e = r + rag * RAG;
	float reach = 1.0 - smoothstep(front - 0.1, front, e);
	if(reach <= 0.0 || r > 1.0)
		discard;
	// How long ago the front passed here: fresh brown at the rim, black where it is old.
	float age = clamp((front - e) / 0.45, 0.0, 1.0);

	float g = dr_fbm(vec3(w / 52.0, t * 0.015));
	vec3 rot = mix(FRESH, OLD, clamp(age * 0.8 + (g - 0.45) * 1.2, 0.0, 1.0));
	float ra = mix(ROTA_NEW, ROTA_OLD, age) * (0.85 + 0.25 * g);

	// Wet pocks: some cells of a jittered grid hold a dark pit whose bubble swells and bursts.
	vec2 cell = floor(w / POCK);
	float pit = 0.0, ringl = 0.0;
	for(int j = 0; j < 9; j++) {
		vec2 c = cell + vec2(mod(float(j), 3.0) - 1.0, floor(float(j) / 3.0) - 1.0);
		float h = dr_hash(vec3(c, 7.0));
		if(h > 0.4)
			continue;
		vec2 ctr = (c + 0.2 + 0.6 * vec2(dr_hash(vec3(c, 11.0)), dr_hash(vec3(c, 13.0)))) * POCK;
		float pr = 4.0 + 5.0 * dr_hash(vec3(c, 19.0));
		// a lumpy outline, so no pit is a clean disc
		float d = length(w - ctr) * (0.8 + 0.45 * dr_noise(vec3((w - ctr) / 4.0, h * 50.0)));
		float s = 1.0 - smoothstep(0.0, pr, d);
		pit = max(pit, s * s);
		float ph = fract(t * POCK_RATE + h * 7.0);
		float br = pr * 0.7 * smoothstep(0.0, 0.85, ph);
		ringl = max(ringl, (1.0 - smoothstep(0.0, 0.7, abs(d - br))) * (1.0 - smoothstep(0.7, 0.88, ph)) * smoothstep(0.05, 0.2, ph));
	}
	pit *= age;
	ringl *= age;
	rot = mix(rot, OLD * 0.4, pit);
	ra = mix(ra, 0.97, pit);

	// Veins branching out from the middle (R), sunk in a darker bruise (B); they reach only as far as the rot has, and
	// a slow glow runs out along them by their distance from the middle (G).
	vec3 vm = getTexel(p + 0.5).rgb;
	float grown = 1.0 - smoothstep(front - 0.12, front, vm.g * FRONT);
	rot *= 1.0 - BRUISE * vm.b * age;
	ra = min(1.0, ra + 0.1 * vm.b * age);
	float ph = fract(t * PULSE_RATE - vm.g * 1.3);
	float wave = smoothstep(0.0, 0.1, ph) * (1.0 - smoothstep(0.1, 0.4, ph));
	vec3 emit = VEIN * vm.r * VEIN_GAIN * (0.35 + 0.65 * wave) * grown * (1.0 - pit);

	// Lit by the sector; the bubbles' wet sheen catches a little more of it.
	vec3 lit = color.rgb * (rot * ra + vec3(0.22, 0.17, 0.2) * ringl);
	float A = clamp(ra + ringl * 0.3 + max(emit.r, max(emit.g, emit.b)), 0.0, 1.0) * reach;
	if(A < 0.01)
		discard;
	vec3 pm = (lit + emit) * reach;
	return vec4(clamp(pm / A, 0.0, 1.0), A);
}
