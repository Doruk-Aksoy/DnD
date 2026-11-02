// Mana bar fill (SPLMANAF) and edge (SPLMANAE), monster bar fills (FILLCRIT, FILLNORM) and tips (HPEDGER, HPEDGEG),
// cast bar fill (SPLCSTF, SPLCSTS), skill wheel border (WHEELSEP).

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

#define mb_hf_TEXW 202.0
#define mb_hf_TEXH 18.0
#define mb_hf_ROWS 16.0
#define mb_hf_SPEED 9.0
#define mb_hf_BEAT 2.0
#define mb_he_EW 16.0
#define mb_he_EH 18.0

float mb_hf_hash(vec2 i) {
	return fract(sin(dot(i, vec2(127.1, 311.7))) * 43758.5453);
}

float mb_hf_noise(vec2 p) {
	vec2 i = floor(p), f = fract(p);
	f = f * f * (3.0 - 2.0 * f);
	float a = mb_hf_hash(i), b = mb_hf_hash(i + vec2(1.0, 0.0)), c = mb_hf_hash(i + vec2(0.0, 1.0)), d = mb_hf_hash(i + vec2(1.0, 1.0));
	return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

vec3 mb_hf_tint(vec3 c, float lift) {
	return mix(c / max(max(c.r, c.g), max(c.b, 0.001)), vec3(1.0), lift);
}

float mb_hf_bubbles(vec2 px) {
	float cw = 13.0;
	float mx = (px.x - timer * mb_hf_SPEED) / cw;
	float cell = floor(mx);
	float ph = fract(timer * (0.3 + 0.3 * mb_hf_hash(vec2(cell, 11.0))) + mb_hf_hash(vec2(cell, 19.0)));
	float row = mb_hf_ROWS - 1.0 - floor(ph * (mb_hf_ROWS - 1.0));
	float at = 2.0 + floor(mb_hf_hash(vec2(cell, 29.0)) * (cw - 4.0) + 0.8 * sin(ph * 9.0 + cell) + 0.5);
	float life = smoothstep(0.0, 0.1, ph) * (1.0 - smoothstep(0.7, 1.0, ph));
	return step(0.5, mb_hf_hash(vec2(cell, 7.0))) * step(abs(floor(fract(mx) * cw) - at), 0.5) * step(abs(px.y - 0.5 - row), 0.5) * life;
}

float mb_hf_beat(float x) {
	float ph = fract(timer / mb_hf_BEAT - x / 400.0);
	float a = (ph - 0.06) / 0.03, b = (ph - 0.16) / 0.03;
	return exp(-a * a) + 0.6 * exp(-b * b);
}

vec4 mb_hf_fill(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec4 tex = getTexel(uv);
	vec2 px = floor(uv * vec2(mb_hf_TEXW, mb_hf_TEXH)) + 0.5;
	float v = px.y / mb_hf_ROWS;
	float depth = 1.0 - 0.6 * abs(v - 0.5) * 2.0;
	float t = timer;

	float w = mb_hf_noise(vec2((px.x - t * mb_hf_SPEED * 0.6) / 30.0, v * 1.5 + t * 0.07));
	float n = mb_hf_noise(vec2((px.x - t * mb_hf_SPEED) / 16.0 + w * 1.8, v * 4.0 + w * 2.0 - t * 0.12));
	float sheen = smoothstep(0.55, 0.92, n);
	float beat = mb_hf_beat(px.x);

	vec3 col = tex.rgb * (0.82 + 0.36 * w + 0.22 * beat);
	col += mb_hf_tint(tex.rgb, 0.25) * (sheen * 0.3 + beat * 0.08) * depth;
	col += mb_hf_tint(tex.rgb, 0.6) * mb_hf_bubbles(px) * 0.7;
	return vec4(col * color.rgb, tex.a * color.a);
}

vec4 mb_he_edge(vec4 color) {
	vec2 px = floor(gl_TexCoord[0].st * vec2(mb_he_EW, mb_he_EH)) + 0.5;
	vec3 art = texture2D(tex, vec2(0.5, 0.5)).rgb;
	vec3 tint = mb_hf_tint(art, 0.0);
	float s = clamp(px.y / mb_hf_ROWS, 0.0, 1.0) * 2.0 - 1.0;
	float bow = 1.0 - s * s;

	float t = timer;
	float surge = 2.0 + 0.9 * sin(t * 2.6) + 0.4 * sin(t * 4.1 + 1.7);
	float waves = 0.55 * sin(s * 7.0 - t * 6.0) + 0.3 * sin(s * 13.0 + t * 8.5);
	float dx = px.x - (mb_he_EW * 0.5 - 0.5 + (surge + waves) * bow + 0.7 * sin(t * 1.3 + 0.8) * s);
	float liquid = step(mb_he_EW * 0.5, px.x) * step(dx, 0.0);
	float glow = exp(-dx * dx / 8.0) * (dx < 0.0 ? 0.55 : 0.3);
	float rim = exp(-dx * dx / 0.7);

	float gs = s - 0.75 * sin(t * 1.9);
	float glint = exp(-gs * gs / 0.06);

	vec4 p = vec4(art * (0.62 + 0.45 * bow) * liquid, liquid);
	p = vec4(mix(tint, vec3(1.0), 0.25 + 0.3 * glint), 1.0) * glow + p * (1.0 - glow);
	p = vec4(mix(tint, vec3(1.0), 0.4 + 0.6 * glint), 1.0) * rim + p * (1.0 - rim);
	return vec4(p.rgb / max(p.a, 0.001), p.a * color.a);
}

vec4 mb_process(vec4 color) {
	if(texture2D(tex, vec2(0.5, 0.5)).a < 0.75)
		return mb_he_edge(color);
	return mb_hf_fill(color);
}

#define cs_cb_TEXW 276.0
#define cs_cb_TEXH 16.0
#define cs_cb_TOP 2.0
#define cs_cb_ROWS 12.0
#define cs_cb_SPEED 46.0
#define cs_cb_CODES 220.0
#define cs_cb_TAU 6.2831853

float cs_cb_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float cs_cb_noise(vec3 p) {
	vec3 i = floor(p), f = fract(p);
	f = f * f * (3.0 - 2.0 * f);
	float a = mix(cs_cb_hash(i), cs_cb_hash(i + vec3(1.0, 0.0, 0.0)), f.x);
	float b = mix(cs_cb_hash(i + vec3(0.0, 1.0, 0.0)), cs_cb_hash(i + vec3(1.0, 1.0, 0.0)), f.x);
	float c = mix(cs_cb_hash(i + vec3(0.0, 0.0, 1.0)), cs_cb_hash(i + vec3(1.0, 0.0, 1.0)), f.x);
	float d = mix(cs_cb_hash(i + vec3(0.0, 1.0, 1.0)), cs_cb_hash(i + vec3(1.0, 1.0, 1.0)), f.x);
	return mix(mix(a, b, f.y), mix(c, d, f.y), f.z);
}

vec3 cs_cb_ramp(float h, vec3 lo, vec3 mid, vec3 hi) {
	if(h < 0.55)
		return mix(lo, mid, h / 0.55);
	return mix(mid, hi, (h - 0.55) / 0.45);
}

float cs_cb_embers(vec2 px, float front, float t) {
	float e = 0.0;
	for(int k = 0; k < 2; ++k) {
		float tk = t * 1.7 + float(k) * 0.5;
		float ph = fract(tk);
		float row = cs_cb_TOP + 1.0 + floor(cs_cb_hash(vec3(floor(tk), float(k), 3.0)) * (cs_cb_ROWS - 2.0));
		float sx = front + 1.0 + ph * 9.0;
		e += step(abs(px.x - floor(sx) - 0.5), 0.5) * step(abs(px.y - 0.5 - row - floor(ph * 1.5)), 0.5) * (1.0 - ph);
	}
	return e;
}

vec4 cs_process(vec4 color) {
	vec2 px = floor(gl_TexCoord[0].st * vec2(cs_cb_TEXW, cs_cb_TEXH)) + 0.5;

	float front = (floor((1.0 - color.a) * 255.0 - 0.25) + 0.5) * cs_cb_TEXW / cs_cb_CODES;
	float silent = 1.0 - step(0.3, texture2D(tex, vec2(0.5, 0.5)).g);
	float t = mod(timer, 512.0);

	float s = clamp((px.y - cs_cb_TOP - cs_cb_ROWS * 0.5) / (cs_cb_ROWS * 0.5), -1.0, 1.0);
	float core = 1.0 - s * s;
	float lip = 1.0 - step(cs_cb_TOP, px.y) * step(px.y, cs_cb_TOP + cs_cb_ROWS);
	float speed = cs_cb_SPEED * mix(1.0, 0.2, silent);

	float lag = (cs_cb_noise(vec3(s * 1.6, t * 0.55, 3.0)) - 0.5) * 16.0;
	float x = px.x - t * speed + lag;
	vec3 q = vec3(x / 26.0, s * 1.2, t * 0.45);
	float wx = cs_cb_noise(q) - 0.5;
	float wy = cs_cb_noise(q + vec3(5.2, 1.3, 2.7)) - 0.5;
	vec3 p = vec3(x / 20.0 + wx * 1.7, s * 1.5 + wy * 1.4, t * 0.6);
	float f = cs_cb_noise(p) * 0.65 + cs_cb_noise(p * 2.1 + vec3(3.1, 7.7, 1.9)) * 0.35;
	float ridge = 1.0 - abs(cs_cb_noise(vec3(x / 13.0 + wx * 2.4, s * 2.3 + wy * 2.2, t * 0.85 + 11.0)) * 2.0 - 1.0);
	float tendril = pow(ridge, 8.0);

	float wob = (0.8 * sin(s * 4.0 + cs_cb_TAU * fract(t * 1.1140846)) + 0.5 * sin(s * 9.0 - cs_cb_TAU * fract(t * 1.7507044) + 1.7)) * (1.0 - 0.7 * silent);
	float df = front - px.x + wob;
	float near = exp(-max(df, 0.0) / 70.0);

	vec3 lo = mix(vec3(0.24, 0.06, 0.01), vec3(0.12, 0.03, 0.06), silent);
	vec3 mid = mix(vec3(0.97, 0.56, 0.13), vec3(0.48, 0.10, 0.16), silent);
	vec3 hi = mix(vec3(1.0, 0.93, 0.72), vec3(0.78, 0.42, 0.44), silent);
	vec3 hot = mix(vec3(1.0, 0.95, 0.80), hi, silent);

	if(lip > 0.5) {
		float inner = (px.y < cs_cb_TOP) ? px.y / cs_cb_TOP : (cs_cb_TEXH - px.y) / cs_cb_TOP;

		float edge = df >= 0.0 ? exp(-df / 10.0) : 0.5 * exp(df / 3.0);
		float l = (0.18 * step(0.0, df) * (0.5 + 0.5 * f) + 0.85 * edge) * inner * (1.0 - 0.5 * silent);
		return vec4(mix(mid, hi, 0.4) * color.rgb, clamp(l, 0.0, 1.0));
	}

	float body = (0.17 + 0.45 * near) * (0.6 + 0.4 * core) + 0.3 * (f - 0.5);
	float heat = clamp(body + (0.42 + 0.15 * near) * tendril * (0.4 + 0.6 * core), 0.0, 1.0);
	vec3 col = cs_cb_ramp(heat, lo, mid, hi);

	float rim = exp(-df * df / 2.2) * (1.0 - 0.6 * silent);
	if(df > 0.0)
		return vec4(mix(col, hot, rim * (0.5 + 0.5 * core)) * color.rgb, 1.0);

	float flick = 0.85 + 0.15 * sin(cs_cb_TAU * fract(t * 2.0690142) + s * 3.0);
	float halo = exp(df / 3.0) * (0.4 + 0.6 * core) * flick;
	float spill = exp(df / 9.0) * 0.18 * core * smoothstep(-15.0, -5.0, df);
	float e = cs_cb_embers(px, front, t) * (1.0 - silent);
	float a = clamp(max(max(halo * 0.9, rim), spill + e * 0.9) * (1.0 - 0.55 * silent), 0.0, 1.0);
	return vec4(mix(mix(mid, hi, 0.5), hot, max(rim, e)) * color.rgb, a);
}

#define wl_SEP_SIZE 364.0
#define wl_SEP_LINE 226.0
#define wl_SEP_FROM 22.0
#define wl_SEP_FROMTOP 136.0
#define wl_SEP_TAU 6.2831853

vec4 wl_sep_texel(vec2 t) {
	vec2 f = t - 0.5;
	vec2 i = floor(f);
	vec2 w = f - i;
	vec2 base = (i + 0.5) / wl_SEP_SIZE;
	float d = 1.0 / wl_SEP_SIZE;
	vec4 a = texture2D(tex, base);
	vec4 b = texture2D(tex, base + vec2(d, 0.0));
	vec4 c = texture2D(tex, base + vec2(0.0, d));
	vec4 e = texture2D(tex, base + vec2(d, d));
	return mix(mix(a, b, w.x), mix(c, e, w.x), w.y);
}

vec4 wl_process(vec4 color) {
	float code = floor((1.0 - color.a) * 255.0 - 0.25);
	float start = 0.0;
	if(code >= wl_SEP_FROMTOP) {
		code -= wl_SEP_FROMTOP;
		start = 0.75;
	}
	float n = 1.0;
	for(int i = 0; i < 16; ++i) {
		if(code >= n) {
			code -= n;
			n += 1.0;
		}
	}
	float ang = fract(start + (code + 0.5) / n) * wl_SEP_TAU;
	vec2 p = (gl_TexCoord[0].st - 0.5) * wl_SEP_SIZE;
	float c = cos(ang), s = sin(ang);
	vec2 t = vec2(c * p.x + s * p.y, c * p.y - s * p.x);

	t.x = wl_SEP_LINE + (t.x - wl_SEP_FROM) * (wl_SEP_SIZE - wl_SEP_LINE) / (wl_SEP_SIZE * 0.5 - wl_SEP_FROM);
	t.y += wl_SEP_SIZE * 0.5;
	if(t.x < 0.5 || t.y < 0.5 || t.x > wl_SEP_SIZE - 0.5 || t.y > wl_SEP_SIZE - 0.5)
		return vec4(0.0);
	vec4 texel = wl_sep_texel(t);
	return vec4(texel.rgb * color.rgb, texel.a);
}

// The cast fill is gold or crimson with clear lip rows, the mana edge plain white, the monster bar arts red or green
// with next to no blue, the mana liquid blue; only the wheel border is clear at both probes.
vec4 Process(vec4 color) {
	vec4 a = texture2D(tex, vec2(0.5, 0.5)), b = texture2D(tex, vec2(0.3, 0.7));
	if(a.a + b.a < 0.02)
		return wl_process(color);
	if(a.r > 0.4 && a.b < 0.3 && texture2D(tex, vec2(0.5, 0.03)).a < 0.1)
		return cs_process(color);
	if(min(min(a.r, a.g), min(b.r, b.g)) > 0.9)
		return me_process(color);
	if(max(a.r, a.g) > a.b + 0.3)
		return mb_process(color);
	return mf_process(color);
}
