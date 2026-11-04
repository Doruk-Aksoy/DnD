// Essence Theft's missile: a churning ball of malevolent purple smoke, burning violet-white deep inside, shedding a
// smoky tail that widens and thins away behind. Ray-marched in ETHEFT.md3's hull; alpha-blended, the actor's alpha is ALPHA_REF.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

// Units before the model's scale, model space x forward, y up (x1.2), z sideways.
#define HULL_C    vec3(-42.0, 0.0, 0.0)    // the ellipsoid it is drawn in, centre and half axes;
#define HULL_R    vec3(58.0, 24.0, 24.0)   // ETHEFT.md3's facets sit just outside it
#define HEAD      9.0      // the head's radius, and how much longer it runs out behind
#define HEAD_L    1.5
#define TAIL_L    80.0     // the tail's length, its half width leaving the head and at its end
#define TAIL_W0   6.0
#define TAIL_W1   15.0
#define SMOKE_S   6.0      // the smoke's grain across the flight, stretched SMOKE_L along it
#define SMOKE_L   14.0
#define SHED      90.0     // units a second the smoke streams back off the head
#define SPIN      0.6      // turns a second the head churns round the flight
#define K_HEAD    0.3      // opacity per unit of density, in the head and in the tail
#define K_TAIL    0.22
#define NEAR0     24.0     // map units from the eye where it starts to show, so a fresh cast stays clear
#define NEAR1     56.0
#define STEPS     20
#define ALPHA_REF 0.02     // the actor's alpha while whole
#define SOOT      vec3(0.09, 0.01, 0.15)
#define VIOLET    vec3(0.52, 0.12, 1.0)
#define BRIGHT    vec3(0.8, 0.52, 1.0)
#define CORE      vec3(0.95, 0.86, 1.0)
#define PULSE_AT  4.0      // the pulsing light: how far ahead of the middle (near the tip), its size, beats a second, strength
#define PULSE_R   3.6
#define PULSE_HZ  1.5
#define PULSE_K   0.55
#define PULSE_C   vec3(0.88, 0.62, 1.0)

// Rule-1 portable hash: wrapped to 256, no sin.
float et_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Smooth value noise 0..1, the eight corners in one loop.
float et_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float v = 0.0;
	for(int c = 0; c < 8; c++) {
		vec3 o = vec3(mod(float(c), 2.0), mod(floor(float(c) * 0.5), 2.0), floor(float(c) * 0.25));
		vec3 w = mix(1.0 - f, f, o);
		v += w.x * w.y * w.z * et_hash(i + o);
	}
	return v;
}

// Smoke at q: x density, y heat. The head churns round the flight; its smoke streams back into a tail that widens and
// fades by density, never to a point.
vec2 et_smoke(vec3 q, float t, vec3 seed) {
	vec3 h = (q - HULL_C) / HULL_R;
	if(dot(h, h) > 0.96)
		return vec2(0.0, 0.0);
	float a = 6.2831853 * fract(t * SPIN) - q.x * 0.05;
	float ca = cos(a), sa = sin(a);
	vec3 r = vec3(q.x + t * SHED, q.y * ca - q.z * sa, q.y * sa + q.z * ca);
	float n = 0.0;
	float fine = 0.0;
	for(int o = 0; o < 2; o++) {
		vec3 b = vec3(r.x / SMOKE_L, r.y / SMOKE_S, r.z / SMOKE_S);
		float v = et_noise(o == 0 ? b + seed : b * 2.17 + seed.zxy + vec3(4.1, 7.7, 1.3));
		if(o == 0)
			n = v;
		else
			fine = v;
	}
	n = 0.62 * n + 0.38 * fine;
	// The head: a ball run out a little behind, its skin torn by the smoke; hot only deep inside.
	float dh = length(vec3(q.x < 0.0 ? q.x / HEAD_L : q.x, q.y, q.z)) / HEAD;
	float vh = 1.1 - dh + 0.8 * (n - 0.5);
	// The tail: straight back, widening as it goes, thinning toward its end.
	float u = clamp(-q.x / TAIL_L, 0.0, 1.0);
	float w = TAIL_W0 + TAIL_W1 * u;
	float vt = (n * 1.7 - 0.2) - length(q.yz) / w - 0.3 * u;
	float tail = smoothstep(0.0, 0.35, vt) * (1.0 - smoothstep(0.4, 1.0, u)) * step(q.x, 2.0);
	float head = smoothstep(0.0, 0.45, vh);
	float dens = max(head * K_HEAD, tail * K_TAIL * mix(1.0, 0.5, u));
	// Dark streaks run through the head's glow, so it churns rather than shines.
	float streak = smoothstep(0.55, 0.75, fine);
	float hh = smoothstep(0.0, 0.85, vh) * (1.0 - 0.4 * streak);
	float ht = clamp(0.55 * (1.0 - u) * smoothstep(-0.1, 0.5, vt) + 0.05, 0.0, 1.0);
	return vec2(dens, max(hh, ht));
}

vec3 et_colour(float h) {
	vec3 c = mix(SOOT, VIOLET, smoothstep(0.12, 0.5, h));
	c = mix(c, BRIGHT, smoothstep(0.55, 0.85, h));
	return mix(c, CORE, smoothstep(0.88, 1.0, h));
}

vec4 Process(vec4 color) {
	mat4 mm = gl_TextureMatrix[7];
	vec3 x12 = cross(mm[1].xyz, mm[2].xyz);
	vec3 x20 = cross(mm[2].xyz, mm[0].xyz);
	vec3 x01 = cross(mm[0].xyz, mm[1].xyz);
	float det = dot(mm[0].xyz, x12);
	vec3 wp = pixelpos.xyz - mm[3].xyz;
	vec3 we = camerapos - mm[3].xyz;
	vec3 sq = vec3(1.0, 1.2, 1.0);
	vec3 p = vec3(dot(x12, wp), dot(x20, wp), dot(x01, wp)) / det * sq;
	vec3 e = vec3(dot(x12, we), dot(x20, we), dot(x01, we)) / det * sq;
	vec3 d = normalize(p - e);

	float fade = smoothstep(NEAR0, NEAR1, length(we)) * clamp(color.a / ALPHA_REF, 0.0, 1.0);
	if(fade <= 0.0)
		discard;

	// One shell per pixel: the outer (t 0.5) from outside, the inner (t 1.5) with the eye inside the hull.
	vec3 oc = (e - HULL_C) / HULL_R;
	if((gl_TexCoord[0].t < 1.0) == (dot(oc, oc) < 1.0))
		discard;
	vec3 dc = d / HULL_R;
	float qa = dot(dc, dc);
	float hb = dot(oc, dc);
	float hd = hb * hb - qa * (dot(oc, oc) - 1.0);
	if(hd <= 0.0)
		discard;
	hd = sqrt(hd);
	float t0 = max((-hb - hd) / qa, 0.0);
	float t1 = (-hb + hd) / qa;

	// Each missile its own smoke, from ScaleX 1 + k x 0.004.
	float k = floor((length(mm[0].xyz) - 1.0) / 0.004 + 0.5);
	vec3 seed = vec3(k * 7.31, k * 3.17, k * 5.53);
	float t = mod(timer, 512.0);

	vec3 acc = vec3(0.0, 0.0, 0.0);
	float A = 0.0;
	// The light swells and dims smoothly (it grows a little as it brightens); half hidden by the smoke in front of it.
	float beat = 0.5 + 0.5 * sin(6.2831853 * fract(t * PULSE_HZ + k * 0.13));
	float pr = PULSE_R * (0.85 + 0.3 * beat);
	float G = 0.0;
	float jit = et_hash(vec3(floor(gl_FragCoord.xy), 5.0));
	float dt = (t1 - t0) / float(STEPS);
	for(int i = 0; i < STEPS; i++) {
		vec3 q = e + d * (t0 + (float(i) + jit) * dt);
		vec3 dq = q - vec3(PULSE_AT, 0.0, 0.0);
		G += exp(-dot(dq, dq) / (pr * pr)) * dt * (1.0 - 0.5 * A);
		vec2 f = et_smoke(q, t, seed);
		float a = 1.0 - exp(-f.x * dt);
		acc += (1.0 - A) * a * et_colour(f.y);
		A += (1.0 - A) * a;
		if(A > 0.97)
			break;
	}
	G *= PULSE_K * (0.45 + 0.55 * beat) / PULSE_R;
	float Ag = max(A, min(G, 1.0));
	if(Ag * fade < 0.01)
		discard;
	return vec4(clamp(acc / max(A, 0.001) * (A / Ag) + PULSE_C * G / Ag, 0.0, 1.0), Ag * fade);
}
