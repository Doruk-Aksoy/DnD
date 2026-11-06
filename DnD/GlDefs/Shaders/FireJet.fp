// Fire Jet's bolt: a white-hot pointed head whose fire splits into tongues fanning out behind it, ray-marched in a
// hull. Additive: the fire is summed, brightness rolled off, hue picked by heat.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

// Units before the model's scale, model space x forward, y up, z sideways, heights x1.2 so it reads as drawn.
#define HULL_C  vec3(-12.0, 0.0, 0.0)    // the ellipsoid everything is drawn inside, centre and half axes;
#define HULL_R  vec3(25.0, 18.0, 11.0)   // FIREJET.md3's facets sit just outside it
#define NOSE    9.0      // where the head's point is, ahead of the actor
#define LEN     42.0     // how far back the longest tongue reaches before it has faded
#define BODY_H  6.0      // the body's half height and half width once past the point
#define BODY_Z  3.4
#define FAN_H   6.5      // how much more the tongues fan out up and down, and sideways, by LEN
#define FAN_Z   2.4
#define THICK   0.2      // a tongue's half thickness, in body half heights
#define WAVE    0.55     // how far the tongues sway, and how often a sway runs down them a second
#define WAVEHZ  1.9
#define RISE    70.0     // units a second the fire streams back
#define K_FIRE  0.24     // brightness per unit of fire
#define VSTEP   0.004    // each bolt's own phase: actor ScaleX 1 + k x VSTEP, k from 0 to 7
#define NEAR0   28.0     // map units from the eye where the bolt starts to show, so a fresh cast stays clear
#define NEAR1   72.0
#define STEPS   28
#define TAU     6.2831853

float fj_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Smooth value noise, 0 to 1: the eight corners in one loop, one hash call site.
float fj_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float v = 0.0;
	for(int c = 0; c < 8; c++) {
		vec3 o = vec3(mod(float(c), 2.0), mod(floor(float(c) * 0.5), 2.0), floor(float(c) * 0.25));
		vec3 w = mix(1.0 - f, f, o);
		v += w.x * w.y * w.z * fj_hash(i + o);
	}
	return v;
}

// Fire by heat: deep red, orange, yellow, and pale only at the very hottest.
vec3 fj_ramp(float h) {
	vec3 c = mix(vec3(0.5, 0.06, 0.0), vec3(1.0, 0.42, 0.05), smoothstep(0.1, 0.45, h));
	c = mix(c, vec3(1.0, 0.78, 0.25), smoothstep(0.45, 0.8, h));
	return mix(c, vec3(1.0, 0.95, 0.75), smoothstep(0.9, 1.05, h));
}

// Fire density and heat at q (u = units back from the point, t the time, ph this bolt's phase).
vec2 fj_fire(vec3 q, float t, float ph) {
	float u = NOSE - q.x;
	if(u < -1.0)
		return vec2(0.0, 0.0);
	float a = clamp(u / 6.5, 0.0, 1.0);
	float grow = sqrt(a * (2.0 - a));
	float v = clamp(u / LEN, 0.0, 1.0);
	float hy = BODY_H * grow + FAN_H * v;
	float hz = BODY_Z * grow + FAN_Z * v;
	float hy0 = max(hy, 0.3);
	float hz0 = max(hz, 0.3);

	// The head: hottest and densest just behind the point, giving way to the tongues.
	float core = exp(-2.2 * ((q.y * q.y) / (hy0 * hy0) + (q.z * q.z) / (hz0 * hz0))) * (1.0 - smoothstep(4.0, 16.0, u)) * smoothstep(-1.0, 1.5, u);

	// Five tongues, one body at first, splitting apart as they stream back and fading out each at its own length.
	float split = smoothstep(4.0, 22.0, u);
	float sway = smoothstep(4.0, 24.0, u) * WAVE;
	float tong = 0.0;
	for(int i = 0; i < 5; i++) {
		float fi = float(i);
		float off = i == 0 ? -0.9 : i == 1 ? -0.42 : i == 2 ? 0.04 : i == 3 ? 0.5 : 0.92;
		float len = i == 0 ? 0.62 : i == 1 ? 0.92 : i == 2 ? 1.0 : i == 3 ? 0.84 : 0.58;
		float side = i == 0 ? 0.35 : i == 1 ? -0.3 : i == 2 ? 0.1 : i == 3 ? -0.4 : 0.25;
		float yc = off * hy * split + sway * sin(TAU * fract(u / 15.0 - t * WAVEHZ + ph + fi * 0.06));
		float zc = side * hz * split;
		float th = THICK * hy0 * (1.0 + 0.6 * v);
		float dy = (q.y - yc) / th;
		float dz = (q.z - zc) / hz0;
		tong += exp(-1.7 * dy * dy - dz * dz) * (1.0 - smoothstep(0.55 * len, len, v)) * 1.5;
	}

	// Torn by noise streaming back: long along the flight and fanning out with the tongues, so it streaks.
	vec3 nq = vec3((u + t * RISE) * 0.06, q.y * 1.8 / hy0, q.z * 1.2 / hz0);
	float n = 0.65 * fj_noise(nq + vec3(ph * 31.0, 0.0, 0.0)) + 0.35 * fj_noise(nq * vec3(2.3, 2.0, 2.0) + vec3(5.2, ph * 17.0, 1.3));
	float tear = smoothstep(0.12 + 0.3 * v, 0.5 + 0.22 * v, n);
	tong *= mix(0.3, 1.0, split);
	float dens = max(core * 2.0, tong * tear);
	float heat = clamp(core * 1.0 + tong * tear * (0.62 - 0.4 * v + 0.3 * (1.0 - split)) + 0.25 * (n - 0.5), 0.0, 1.0);
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
	float k = floor((length(mm[0].xyz) - 1.0) / VSTEP + 0.5);
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

	float jit = fj_hash(vec3(floor(gl_FragCoord.xy), 7.0));
	float dt = (t1 - t0) / float(STEPS);
	float B = 0.0;
	float H = 0.0;
	for(int i = 0; i < STEPS; i++) {
		vec3 s = e + d * (t0 + (float(i) + jit) * dt);
		vec3 sc = (s - HULL_C) / HULL_R;
		vec2 fh = fj_fire(s, t, ph);
		float w = fh.x * dt * (1.0 - smoothstep(0.75, 1.0, dot(sc, sc)));
		B += w;
		H += w * fh.y;
	}
	if(B < 0.004)
		discard;
	float heat = H / B;
	float bright = 1.0 - exp(-B * K_FIRE);
	return vec4(fj_ramp(heat + 0.08 * bright) * bright * fade * color.a, 1.0);
}
