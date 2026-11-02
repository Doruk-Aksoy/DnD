// Monster bar fill (FILLCRIT, FILLNORM): slow currents, rising bubbles and a heartbeat running to the end.
// Its tip (HPEDGER, HPEDGEG: the liquid's colour at alpha 0.5): a bulging, sloshing front.

uniform float timer;

#define hf_TEXW 202.0
#define hf_TEXH 18.0
#define hf_ROWS 16.0
#define hf_SPEED 9.0
#define hf_BEAT 2.0
#define he_EW 16.0
#define he_EH 18.0

float hf_hash(vec2 i) {
	return fract(sin(dot(i, vec2(127.1, 311.7))) * 43758.5453);
}

float hf_noise(vec2 p) {
	vec2 i = floor(p), f = fract(p);
	f = f * f * (3.0 - 2.0 * f);
	float a = hf_hash(i), b = hf_hash(i + vec2(1.0, 0.0)), c = hf_hash(i + vec2(0.0, 1.0)), d = hf_hash(i + vec2(1.0, 1.0));
	return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

// The art's own hue at full strength, lifted toward white.
vec3 hf_tint(vec3 c, float lift) {
	return mix(c / max(max(c.r, c.g), max(c.b, 0.001)), vec3(1.0), lift);
}

float hf_bubbles(vec2 px) {
	float cw = 13.0;
	float mx = (px.x - timer * hf_SPEED) / cw;
	float cell = floor(mx);
	float ph = fract(timer * (0.3 + 0.3 * hf_hash(vec2(cell, 11.0))) + hf_hash(vec2(cell, 19.0)));
	float row = hf_ROWS - 1.0 - floor(ph * (hf_ROWS - 1.0));
	float at = 2.0 + floor(hf_hash(vec2(cell, 29.0)) * (cw - 4.0) + 0.8 * sin(ph * 9.0 + cell) + 0.5);
	float life = smoothstep(0.0, 0.1, ph) * (1.0 - smoothstep(0.7, 1.0, ph));
	return step(0.5, hf_hash(vec2(cell, 7.0))) * step(abs(floor(fract(mx) * cw) - at), 0.5) * step(abs(px.y - 0.5 - row), 0.5) * life;
}

// Two close pulses travelling toward the fill's end.
float hf_beat(float x) {
	float ph = fract(timer / hf_BEAT - x / 400.0);
	float a = (ph - 0.06) / 0.03, b = (ph - 0.16) / 0.03;
	return exp(-a * a) + 0.6 * exp(-b * b);
}

vec4 hf_fill(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec4 tex = getTexel(uv);
	vec2 px = floor(uv * vec2(hf_TEXW, hf_TEXH)) + 0.5;
	float v = px.y / hf_ROWS;
	float depth = 1.0 - 0.6 * abs(v - 0.5) * 2.0;
	float t = timer;

	float w = hf_noise(vec2((px.x - t * hf_SPEED * 0.6) / 30.0, v * 1.5 + t * 0.07));
	float n = hf_noise(vec2((px.x - t * hf_SPEED) / 16.0 + w * 1.8, v * 4.0 + w * 2.0 - t * 0.12));
	float sheen = smoothstep(0.55, 0.92, n);
	float beat = hf_beat(px.x);

	vec3 col = tex.rgb * (0.82 + 0.36 * w + 0.22 * beat);
	col += hf_tint(tex.rgb, 0.25) * (sheen * 0.3 + beat * 0.08) * depth;
	col += hf_tint(tex.rgb, 0.6) * hf_bubbles(px) * 0.7;
	return vec4(col * color.rgb, tex.a * color.a);
}

// The front starts on the fill's last column at the top and bottom and bulges past it in between.
vec4 he_edge(vec4 color) {
	vec2 px = floor(gl_TexCoord[0].st * vec2(he_EW, he_EH)) + 0.5;
	vec3 art = texture2D(tex, vec2(0.5, 0.5)).rgb;
	vec3 tint = hf_tint(art, 0.0);
	float s = clamp(px.y / hf_ROWS, 0.0, 1.0) * 2.0 - 1.0;
	float bow = 1.0 - s * s;

	// the front surges out and back, waves run along it, and it rocks
	float t = timer;
	float surge = 2.0 + 0.9 * sin(t * 2.6) + 0.4 * sin(t * 4.1 + 1.7);
	float waves = 0.55 * sin(s * 7.0 - t * 6.0) + 0.3 * sin(s * 13.0 + t * 8.5);
	float dx = px.x - (he_EW * 0.5 - 0.5 + (surge + waves) * bow + 0.7 * sin(t * 1.3 + 0.8) * s);
	float liquid = step(he_EW * 0.5, px.x) * step(dx, 0.0);
	float glow = exp(-dx * dx / 8.0) * (dx < 0.0 ? 0.55 : 0.3);
	float rim = exp(-dx * dx / 0.7);

	// a glint sliding up and down the rim
	float gs = s - 0.75 * sin(t * 1.9);
	float glint = exp(-gs * gs / 0.06);

	// premultiplied, back to front: the pushed-out liquid, the glow, the rim
	vec4 p = vec4(art * (0.62 + 0.45 * bow) * liquid, liquid);
	p = vec4(mix(tint, vec3(1.0), 0.25 + 0.3 * glint), 1.0) * glow + p * (1.0 - glow);
	p = vec4(mix(tint, vec3(1.0), 0.4 + 0.6 * glint), 1.0) * rim + p * (1.0 - rim);
	return vec4(p.rgb / max(p.a, 0.001), p.a * color.a);
}

vec4 Process(vec4 color) {
	if(texture2D(tex, vec2(0.5, 0.5)).a < 0.75)
		return he_edge(color);
	return hf_fill(color);
}
