// Arcane Bolt: a white-violet head trailing a comet tail of fine streaks, ray-marched in a hull. Additive.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

// World units, model space x forward, y up, z sideways, heights x1.2 so it reads as drawn.
#define HULL_C  vec3(-14.0, 0.0, 0.0)    // the ellipsoid everything is drawn inside, centre and half axes;
#define HULL_R  vec3(24.0, 10.5, 10.5)     // ARCBOLT.md3's facets sit just outside it
#define HEAD_R  2.2      // the white core's radius, and the violet glow round it
#define GLOW_R  3.6
#define LEN     30.0     // how far back the tail reaches before it has faded
#define TAIL_R0 2.6      // the tail's radius at the head and at LEN
#define TAIL_R1 8.6
#define STREAK  3.6      // streaks across the tail's width; how fast they stream back, units a second
#define RISE    90.0
#define K_GLOW  0.55     // brightness per unit of glow
#define VSTEP   0.0069   // each bolt's own phase: model scale BASE_S + k x VSTEP (actor ScaleX 0.45 + k x 0.0024)
#define BASE_S  1.3
#define NEAR0   36.0     // map units from the eye where the bolt starts to show, so a fresh cast stays clear
#define NEAR1   100.0
#define STEPS   24

float ab_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Smooth value noise, 0 to 1: the eight corners in one loop, one hash call site.
float ab_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float v = 0.0;
	for(int c = 0; c < 8; c++) {
		vec3 o = vec3(mod(float(c), 2.0), mod(floor(float(c) * 0.5), 2.0), floor(float(c) * 0.25));
		vec3 w = mix(1.0 - f, f, o);
		v += w.x * w.y * w.z * ab_hash(i + o);
	}
	return v;
}

// Deep indigo, violet, lavender, and white-pink only in the core.
vec3 ab_ramp(float h) {
	vec3 c = mix(vec3(0.13, 0.04, 0.45), vec3(0.45, 0.16, 1.0), smoothstep(0.05, 0.4, h));
	c = mix(c, vec3(0.78, 0.48, 1.0), smoothstep(0.45, 0.78, h));
	return mix(c, vec3(1.0, 0.9, 1.0), smoothstep(0.82, 1.0, h));
}

// Glow density and heat at q (t the time, ph this bolt's phase).
vec2 ab_glow(vec3 q, float t, float ph) {
	float r2 = q.y * q.y + q.z * q.z;
	float d2 = r2 + q.x * q.x;
	// A pointed head: sharp ahead, its white core drawn out behind along the line of flight.
	float cx = q.x > 0.0 ? q.x * 1.8 : q.x * 0.22;
	float core = exp(-(r2 * 1.6 + cx * cx) / (HEAD_R * HEAD_R));
	float gx = q.x > 0.0 ? q.x * 1.4 : q.x * 0.35;
	float glow = exp(-(r2 + gx * gx) / (GLOW_R * GLOW_R));

	// The tail widens behind the head and thins out by LEN, torn into streaks streaming back.
	float u = -q.x;
	float v = clamp(u / LEN, 0.0, 1.0);
	float rt = TAIL_R0 + (TAIL_R1 - TAIL_R0) * sqrt(v);
	float body = exp(-2.0 * r2 / (rt * rt)) * (1.0 - v) * (1.0 - v) * smoothstep(-1.5, 2.5, u);
	vec3 nq = vec3((u - t * RISE) * 0.045 + ph * 13.0, q.y * STREAK / rt, q.z * STREAK / rt);
	float n = 0.6 * ab_noise(nq) + 0.4 * ab_noise(nq * vec3(1.7, 2.1, 2.1) + vec3(3.1, ph * 7.0, 5.7));
	float streak = smoothstep(0.34 + 0.15 * v, 0.7, n);
	float tail = body * mix(0.35, 1.0, smoothstep(2.0, 10.0, u)) * mix(1.0, streak * 1.6, smoothstep(1.0, 8.0, u));

	float dens = core * 4.5 + glow * 0.7 + tail * 2.0;
	float heat = clamp((core * 4.5 + glow * 0.7 * 0.6 + tail * 2.0 * (0.66 - 0.38 * v + 0.2 * streak - 0.45 * min(r2 * 2.5 / (rt * rt), 1.0))) / max(dens, 0.0001), 0.0, 1.0);
	return vec2(dens, heat);
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

	float fade = smoothstep(NEAR0, NEAR1, length(we));
	if(fade <= 0.0)
		discard;
	float k = floor((length(mm[0].xyz) - BASE_S) / VSTEP + 0.5);
	float ph = k * 0.137;
	float t = mod(timer, 512.0);

	// The ray inside the hull's ellipsoid; the outer shell (t 0.5) from outside, the inner one (t 1.5) from within.
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

	float jit = ab_hash(vec3(floor(gl_FragCoord.xy), 7.0));
	float dt = (t1 - t0) / float(STEPS);
	float B = 0.0;
	float H = 0.0;
	for(int i = 0; i < STEPS; i++) {
		vec3 s = e + d * (t0 + (float(i) + jit) * dt);
		vec3 sc = (s - HULL_C) / HULL_R;
		vec2 gh = ab_glow(s, t, ph);
		float w = gh.x * dt * (1.0 - smoothstep(0.7, 1.0, dot(sc, sc)));
		B += w;
		H += w * gh.y;
	}
	if(B < 0.004)
		discard;
	float heat = H / B;
	float bright = 1.0 - exp(-B * K_GLOW);
	return vec4(ab_ramp(heat + 0.1 * bright) * bright * fade * color.a, 1.0);
}
