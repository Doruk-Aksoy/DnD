// Mana bar fill (SPLMANAF): liquid flowing left to right; DrawManaBar packs the surge and low-mana levels in the alpha.

uniform float timer;

#define TEXW 299.0      // art size, so effects snap to its pixel grid
#define TEXH 30.0
#define TOP 9.0         // first and last liquid rows of the art
#define BOT 20.0
#define SPEED 14.0      // texels a second, calm flow
#define SLOW 0.3        // low mana flow, share of SPEED
#define FAST 3.4        // surge flow, share of SPEED

float mb_hash(vec2 i) {
	return fract(sin(dot(i, vec2(127.1, 311.7))) * 43758.5453);
}

// value noise on an integer lattice, so the hash only ever sees exact cell ids
float mb_noise(vec2 p) {
	vec2 i = floor(p), f = fract(p);
	f = f * f * (3.0 - 2.0 * f);
	float a = mb_hash(i), b = mb_hash(i + vec2(1.0, 0.0)), c = mb_hash(i + vec2(0.0, 1.0)), d = mb_hash(i + vec2(1.0, 1.0));
	return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

// one current at s texels a second: x = broad swell, y = thin filaments
vec2 mb_current(vec2 px, float v, float s, float seed) {
	float t = timer;
	float swell = mb_noise(vec2((px.x - t * s * 0.55) / 26.0 + seed, v * 1.2 + t * 0.11));
	float f1 = mb_noise(vec2((px.x - t * s) / 11.0 + seed * 3.0, v * 3.2 - t * 0.23));
	float f2 = mb_noise(vec2((px.x - t * s * 1.3) / 7.0 + 31.0 + seed, v * 4.5 + t * 0.31));
	return vec2(swell, pow(1.0 - abs(f1 * 2.0 - 1.0), 6.0) + 0.6 * pow(1.0 - abs(f2 * 2.0 - 1.0), 8.0));
}

// one-texel sparkles in hashed cells, carried at s texels a second
float mb_motes(vec2 px, float s, float cw, float keep, float seed) {
	float mx = (px.x - timer * s) / cw;
	float cell = floor(mx);
	float roll = mb_hash(vec2(cell, 7.0 + seed));
	float row = TOP + 1.0 + floor(mb_hash(vec2(cell, 13.0 + seed)) * (BOT - TOP - 1.0));
	float at = floor(mb_hash(vec2(cell, 29.0 + seed)) * cw);
	float lx = floor(fract(mx) * cw);
	float tw = 0.5 + 0.5 * sin(timer * (5.0 + roll * 6.0) + roll * 40.0);
	return step(1.0 - keep, roll) * step(abs(lx - at), 0.5) * step(abs(px.y - 0.5 - row), 0.5) * tw;
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec4 tex = getTexel(uv);
	float liquid = smoothstep(0.05, 0.10, tex.b - tex.r) * tex.a;
	if(liquid <= 0.0)
		return vec4(tex.rgb * color.rgb, tex.a);

	float code = floor((1.0 - color.a) * 64.0 + 0.5);
	float surge = floor(code / 8.0) / 7.0;
	float low = mod(code, 8.0) / 7.0 * (1.0 - surge);
	float calm = 1.0 - max(surge, low);
	float sum = calm + surge + low;

	vec2 px = floor(uv * vec2(TEXW, TEXH)) + 0.5;
	float v = clamp((px.y - TOP) / (BOT - TOP + 1.0), 0.0, 1.0);
	float depth = 1.0 - 0.55 * abs(v - 0.4) * 2.0;

	vec2 cn = mb_current(px, v, SPEED, 0.0);
	vec2 cs = mb_current(px, v, SPEED * FAST, 5.0);
	vec2 cl = mb_current(px, v, SPEED * SLOW, 9.0);
	float swell = (cn.x * calm + cs.x * surge + cl.x * low) / sum;
	float ridge = (cn.y * calm + cs.y * surge * 1.6 + cl.y * low * 0.6) / sum;

	// surge: bright waves rushing right, spaced unevenly by noise
	float ph = fract((px.x - timer * SPEED * FAST * 1.5) / 85.0 + 0.25 * mb_noise(vec2(px.x / 40.0, v * 2.0)));
	float rush = smoothstep(0.0, 0.12, ph) * (1.0 - smoothstep(0.12, 0.45, ph));

	vec3 col = tex.rgb * (0.78 + 0.5 * swell);
	col += vec3(0.30, 0.62, 1.0) * ridge * 0.42 * depth;
	col += vec3(0.50, 0.78, 1.0) * rush * surge * 0.5 * depth;
	col *= 1.0 + 0.25 * surge;

	// low mana: dimmer, greyer, breathing slowly
	float breath = 0.5 + 0.5 * sin(timer * 6.2832 / 1.8);
	vec3 grey = vec3(dot(col, vec3(0.3, 0.59, 0.11)));
	col = mix(col, mix(col, grey, 0.35) * (0.58 + 0.14 * breath), low);

	float m = mb_motes(px, 30.0, 9.0, 0.55, 0.0) * (calm + 0.35 * low) + mb_motes(px, 105.0, 5.0, 0.7, 50.0) * surge;
	col += vec3(0.75, 0.9, 1.0) * m * 0.85;

	col = mix(tex.rgb, col, liquid);
	return vec4(col * color.rgb, tex.a);
}
