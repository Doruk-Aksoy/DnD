// Dark Portal on PORTAL.md3: a shallow basin of polished, engraved gold round a void of black smoke turning inward, its
// layers drawn below the disc so it reads as a hole. UV: the basin's profile normal (s, t < 1) or the void (above 2).

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define TAU       6.2831853
#define VOID_H    6.6      // the void disc's height and radius in model units (portal.py)
#define VOID_R    18.6
#define GOLD      vec3(1.0, 0.74, 0.3)
#define GOLD_DEEP vec3(0.42, 0.24, 0.06)
#define LIT       0.6      // how much of the gold the sector light shades
#define UNDERGLOW vec3(0.42, 0.12, 0.7)    // the void's light on the inner lip
#define SMOKE     vec3(0.24, 0.13, 0.34)   // the void's wisps, and the glints deep in it
#define GLINT     vec3(0.75, 0.45, 1.0)
#define SPIN      0.05     // turns a second the smoke turns, and how much faster it turns near the middle
#define TWIST     9.0
#define SCRATCH   0.55     // how strongly the fine scratches catch the light, and the tarnish in the hollows
#define TARNISH   0.45
#define ARMS      3.0      // spiral arms the smoke gathers in, and the violet edge where the void meets the gold
#define EDGE      vec3(0.55, 0.16, 0.85)

// Rule-1 portable hash and value noise.
float dp_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float dp_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float v = 0.0;
	for(int c = 0; c < 8; c++) {
		vec3 o = vec3(mod(float(c), 2.0), mod(floor(float(c) * 0.5), 2.0), floor(float(c) * 0.25));
		vec3 w = mix(1.0 - f, f, o);
		v += w.x * w.y * w.z * dp_hash(i + o);
	}
	return v;
}

// Engraving depth on the outer wall: two border grooves and a glyph of strokes per cell round it.
float dp_engrave(vec3 p) {
	float a = atan(p.z, p.x + 1e-4) / TAU + 0.5;
	float cell = floor(a * 28.0);
	float u = fract(a * 28.0);
	float v = (p.y - 3.4) / 5.0;
	float g = max(1.0 - abs(p.y - 2.9) / 0.35, 1.0 - abs(p.y - 8.9) / 0.35);
	if(v > 0.05 && v < 0.95) {
		float h1 = dp_hash(vec3(cell, 3.0, 7.0)), h2 = dp_hash(vec3(cell, 11.0, 5.0));
		g = max(g, 1.0 - abs(u - 0.5) / 0.06);
		g = max(g, (1.0 - abs(v - (0.25 + 0.5 * h1) - (u - 0.5) * (h2 - 0.5) * 1.6) / 0.07) * step(abs(u - 0.5), 0.38));
		if(h2 > 0.55)
			g = max(g, 1.0 - abs(length(vec2(u - 0.5, (v - 0.8) * 0.8)) - 0.14) / 0.05);
	}
	return clamp(g, 0.0, 1.0);
}

vec4 Process(vec4 color) {
	mat4 mm = gl_TextureMatrix[7];
	vec3 x12 = cross(mm[1].xyz, mm[2].xyz);
	vec3 x20 = cross(mm[2].xyz, mm[0].xyz);
	vec3 x01 = cross(mm[0].xyz, mm[1].xyz);
	float det = dot(mm[0].xyz, x12);
	vec3 p = vec3(dot(x12, pixelpos.xyz - mm[3].xyz), dot(x20, pixelpos.xyz - mm[3].xyz), dot(x01, pixelpos.xyz - mm[3].xyz)) / det;
	vec3 we = camerapos - mm[3].xyz;
	vec3 e = vec3(dot(x12, we), dot(x20, we), dot(x01, we)) / det;
	vec3 v = normalize(camerapos - pixelpos.xyz);
	vec2 uv = gl_TexCoord[0].st;
	float t = mod(timer, 512.0);
	float r = length(p.xz);

	if(uv.x > 1.5) {
		// The void: smoke layers below the disc along the view ray, front to back, then black.
		vec3 d = normalize(p - e);
		vec3 acc = vec3(0.0);
		float tr = 1.0;
		for(int k = 0; k < 4; k++) {
			float depth = 3.0 + 6.0 * float(k);
			float s = min(depth / max(-d.y, 0.08), 60.0);
			vec3 q = p + d * s;
			float rq = length(q.xz);
			float a = atan(q.z, q.x + 1e-4) + TWIST / (rq + 6.0) + TAU * fract(t * SPIN * (1.0 + 0.4 * float(k)));
			float n = dp_noise(vec3(cos(a) * rq * 0.16, sin(a) * rq * 0.16, float(k) * 3.7 + t * 0.12));
			float inside = 1.0 - smoothstep(VOID_R * 0.85, VOID_R * 1.05, rq);
			float arm = 0.55 + 0.45 * sin(TAU * fract(a * ARMS / TAU + rq * 0.05 - t * 0.12));
			float dens = smoothstep(0.32, 0.75, n * (0.6 + 0.6 * arm)) * inside * (0.9 - 0.15 * float(k));
			float sp = pow(dp_noise(vec3(cos(a) * rq * 0.5, sin(a) * rq * 0.5, float(k) * 5.3 + t * 0.3)), 9.0) * step(1.5, float(k));
			vec3 c = SMOKE * (1.0 - 0.2 * float(k)) + GLINT * sp * 2.5 * inside;
			acc += tr * dens * c;
			tr *= 1.0 - dens * 0.8;
		}
		// Darker toward the wall, a faint deep light in the middle.
		acc *= 1.0 - 0.7 * smoothstep(VOID_R * 0.6, VOID_R, r);
		acc += tr * UNDERGLOW * 0.12 * exp(-r * r / 40.0);
		acc += EDGE * 0.55 * exp(-(VOID_R - r) * (VOID_R - r) / 5.0);
		return vec4(acc, color.a);
	}

	// The gold: the lathe's normal from the UV, lit from above with a sheen of the room above it.
	vec2 pn = uv * 2.0 - 1.0;
	vec3 nm = normalize(vec3(p.x / max(r, 0.01) * pn.x, pn.y, p.z / max(r, 0.01) * pn.x));
	vec3 nw = normalize(nm.x * x12 + nm.y * x20 + nm.z * x01);
	nw *= sign(dot(nw, v));
	vec3 L = normalize(vec3(-0.3, 1.0, 0.2));
	float diff = max(dot(nw, L), 0.0);
	vec3 refl = reflect(-v, nw);
	float env = smoothstep(-0.35, 0.9, refl.y);
	float spec = pow(max(dot(refl, L), 0.0), 28.0);
	float ham = 0.88 + 0.24 * dp_noise(p * 0.9);
	// Wear: fine scratches in three directions (ridges of noise stretched along each, bent by a slow noise), and tarnish.
	float bend = dp_noise(p * 0.12 + vec3(4.0, 1.0, 7.0)) * 2.4;
	float scr = 0.0;
	for(int k = 0; k < 3; k++) {
		float an = bend + float(k) * 1.9;
		vec3 da = vec3(cos(an), 0.35 * sin(an * 1.7), sin(an));
		vec3 db = normalize(cross(da, vec3(0.3, 1.0, -0.2)));
		vec3 dc = cross(da, db);
		float n = dp_noise(vec3(dot(p, da) * 0.35, dot(p, db) * 6.0, dot(p, dc) * 6.0) + vec3(float(k) * 17.0, 3.0, 9.0));
		scr = max(scr, smoothstep(0.92, 0.985, 1.0 - abs(2.0 * n - 1.0)) * (0.5 + 0.5 * float(k == 0)));
	}
	scr *= smoothstep(0.35, 0.6, dp_noise(p * 0.3 + vec3(2.0, 8.0, 5.0)));
	float grime = smoothstep(0.45, 0.85, dp_noise(p * 0.45 + vec3(9.0, 2.0, 1.0)) * 0.7 + dp_noise(p * 1.7) * 0.3);
	float eng = dp_engrave(p) * step(23.5, r) * smoothstep(0.6, 0.1, abs(pn.y));
	// On top of the lip: two grooves and a diamond between them in each of its cells.
	float la = atan(p.z, p.x + 1e-4) / TAU + 0.5;
	float lu = fract(la * 40.0) - 0.5;
	float lv = (r - 22.65) / 0.95;
	float lip = max(1.0 - abs(abs(lv) - 1.0) / 0.22, 1.0 - (abs(lu) * 2.4 + abs(lv)) / 0.55 * 0.9) * step(abs(lv), 1.3);
	eng = max(eng, clamp(lip, 0.0, 1.0) * smoothstep(0.55, 0.85, pn.y));
	vec3 metal = mix(GOLD_DEEP, GOLD, 0.25 + 0.75 * env) * ham * (0.35 + 0.65 * diff + 0.25 * env);
	metal *= 1.0 - TARNISH * grime * 0.6;
	metal = mix(metal, GOLD_DEEP * 0.35, eng * 0.85) + vec3(1.0, 0.92, 0.65) * spec * (1.0 - eng) * (1.0 - 0.5 * grime);
	// A scratch is a bright line where it faces the light, a thin dark one where it does not.
	metal += vec3(1.0, 0.86, 0.55) * scr * SCRATCH * (0.25 + 0.75 * env) * (1.0 - eng);
	metal *= 1.0 - 0.25 * scr * (1.0 - env);
	// The void's light on the inner lip, breathing slowly.
	float under = smoothstep(10.5, VOID_H, p.y) * (1.0 - step(21.0, r));
	vec3 glow = UNDERGLOW * under * (0.45 + 0.15 * sin(TAU * fract(t * 0.3)));
	return vec4(metal * mix(vec3(1.0), color.rgb, LIT) + glow, color.a);
}
