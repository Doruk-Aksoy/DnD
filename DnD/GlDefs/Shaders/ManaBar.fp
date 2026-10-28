// Mana bar fill (SPLMANAF) and its glowing edge (SPLMANAE) in one shader; the plain white edge art says which.

uniform float timer;

#define mf_TEXW 299.0
#define mf_TEXH 30.0
#define mf_TOP 9.0
#define mf_BOT 20.0
#define mf_SPEED 14.0
#define mf_SLOW 0.3
#define mf_FAST 3.4

float mf_mb_hash(vec2 i) {
	return fract(sin(dot(i, vec2(127.1, 311.7))) * 43758.5453);
}

float mf_mb_noise(vec2 p) {
	vec2 i = floor(p), f = fract(p);
	f = f * f * (3.0 - 2.0 * f);
	float a = mf_mb_hash(i), b = mf_mb_hash(i + vec2(1.0, 0.0)), c = mf_mb_hash(i + vec2(0.0, 1.0)), d = mf_mb_hash(i + vec2(1.0, 1.0));
	return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

vec2 mf_mb_current(vec2 px, float v, float s, float seed) {
	float t = timer;
	float swell = mf_mb_noise(vec2((px.x - t * s * 0.55) / 26.0 + seed, v * 1.2 + t * 0.11));
	float f1 = mf_mb_noise(vec2((px.x - t * s) / 11.0 + seed * 3.0, v * 3.2 - t * 0.23));
	float f2 = mf_mb_noise(vec2((px.x - t * s * 1.3) / 7.0 + 31.0 + seed, v * 4.5 + t * 0.31));
	return vec2(swell, pow(1.0 - abs(f1 * 2.0 - 1.0), 6.0) + 0.6 * pow(1.0 - abs(f2 * 2.0 - 1.0), 8.0));
}

float mf_mb_motes(vec2 px, float s, float cw, float keep, float seed) {
	float mx = (px.x - timer * s) / cw;
	float cell = floor(mx);
	float roll = mf_mb_hash(vec2(cell, 7.0 + seed));
	float row = mf_TOP + 1.0 + floor(mf_mb_hash(vec2(cell, 13.0 + seed)) * (mf_BOT - mf_TOP - 1.0));
	float at = floor(mf_mb_hash(vec2(cell, 29.0 + seed)) * cw);
	float lx = floor(fract(mx) * cw);
	float tw = 0.5 + 0.5 * sin(timer * (5.0 + roll * 6.0) + roll * 40.0);
	return step(1.0 - keep, roll) * step(abs(lx - at), 0.5) * step(abs(px.y - 0.5 - row), 0.5) * tw;
}

vec4 mf_process(vec4 color) {
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

	vec2 px = floor(uv * vec2(mf_TEXW, mf_TEXH)) + 0.5;
	float v = clamp((px.y - mf_TOP) / (mf_BOT - mf_TOP + 1.0), 0.0, 1.0);
	float depth = 1.0 - 0.55 * abs(v - 0.4) * 2.0;

	vec2 cn = mf_mb_current(px, v, mf_SPEED, 0.0);
	vec2 cs = mf_mb_current(px, v, mf_SPEED * mf_FAST, 5.0);
	vec2 cl = mf_mb_current(px, v, mf_SPEED * mf_SLOW, 9.0);
	float swell = (cn.x * calm + cs.x * surge + cl.x * low) / sum;
	float ridge = (cn.y * calm + cs.y * surge * 1.6 + cl.y * low * 0.6) / sum;

	float ph = fract((px.x - timer * mf_SPEED * mf_FAST * 1.5) / 85.0 + 0.25 * mf_mb_noise(vec2(px.x / 40.0, v * 2.0)));
	float rush = smoothstep(0.0, 0.12, ph) * (1.0 - smoothstep(0.12, 0.45, ph));

	vec3 col = tex.rgb * (0.78 + 0.5 * swell);
	col += vec3(0.30, 0.62, 1.0) * ridge * 0.42 * depth;
	col += vec3(0.50, 0.78, 1.0) * rush * surge * 0.5 * depth;
	col *= 1.0 + 0.25 * surge;

	float breath = 0.5 + 0.5 * sin(timer * 6.2832 / 1.8);
	vec3 grey = vec3(dot(col, vec3(0.3, 0.59, 0.11)));
	col = mix(col, mix(col, grey, 0.35) * (0.58 + 0.14 * breath), low);

	float m = mf_mb_motes(px, 30.0, 9.0, 0.55, 0.0) * (calm + 0.35 * low) + mf_mb_motes(px, 105.0, 5.0, 0.7, 50.0) * surge;
	col += vec3(0.75, 0.9, 1.0) * m * 0.85;

	col = mix(tex.rgb, col, liquid);
	return vec4(col * color.rgb, tex.a);
}

#define me_EW 16.0
#define me_EH 30.0
#define me_TOP 9.0
#define me_BOT 20.0

float me_me_hash(float n) {
	return fract(sin(n * 91.7) * 43758.5453);
}

vec4 me_process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec2 px = floor(uv * vec2(me_EW, me_EH)) + 0.5;
	if(px.y < me_TOP || px.y > me_BOT + 1.0)
		return vec4(0.0, 0.0, 0.0, 1.0);

	float code = floor((1.0 - color.a) * 64.0 + 0.5);
	float surge = floor(code / 8.0) / 7.0;
	float low = mod(code, 8.0) / 7.0 * (1.0 - surge);
	float v = (px.y - me_TOP) / (me_BOT - me_TOP + 1.0);
	float agit = 1.0 + 1.8 * surge - 0.5 * low;

	float wob = 0.9 * sin(v * 5.3 + timer * 3.1 * agit) + 0.6 * sin(v * 11.7 - timer * 4.7 * agit + 1.3);
	float cx = me_EW * 0.5 - 0.5 + wob * (0.6 + 0.5 * surge);
	float dx = px.x - cx;
	float core = exp(-dx * dx / 0.9);
	float halo = exp(-dx * dx / 12.0) * (dx < 0.0 ? 0.8 : 0.3);
	float flick = 0.85 + 0.15 * sin(timer * 9.0 + v * 7.0);
	float rows = smoothstep(0.0, 0.2, v) * (1.0 - smoothstep(0.85, 1.05, v));
	float gain = flick * rows * (1.3 + 0.9 * surge) * (1.0 - 0.55 * low);

	float drops = 0.0;
	for(int k = 0; k < 3; ++k) {
		float tk = timer * 1.9 + float(k) * 0.37;
		float id = floor(tk) + float(k) * 17.0;
		float ph = fract(tk);
		float sy = me_TOP + 1.0 + floor(me_me_hash(id) * (me_BOT - me_TOP - 1.0));
		float sx = cx + 1.5 + ph * 5.5;
		drops += step(abs(px.x - floor(sx) - 0.5), 0.5) * step(abs(px.y - 0.5 - sy), 0.5) * (1.0 - ph);
	}

	vec3 c = vec3(0.35, 0.65, 1.0) * halo + vec3(0.8, 0.93, 1.0) * core;
	c = c * gain + vec3(0.7, 0.88, 1.0) * drops * surge * 0.9;
	return vec4(c, 1.0);
}

vec4 Process(vec4 color) {
	vec4 a = texture2D(tex, vec2(0.5, 0.5)), b = texture2D(tex, vec2(0.3, 0.7));
	if(min(min(a.r, a.g), min(b.r, b.g)) > 0.9)
		return me_process(color);
	return mf_process(color);
}
