// Doomguard minion sprites: white-hot visor slits churning as lava, the neck and nearby steel lit by them, rising embers.
// The art carries its roles in low bits: blue & 3 (1 molten, 2 lit by the visor), green & 3 firelight, red & 7 heat.

uniform float timer;

#define DG_SIZE vec2(214.0, 197.0)          // the canvas every shaded frame sits on, in texels
#define DG_ANCHOR vec2(130.5, 57.5)      // the visor's top middle on every frame: the embers rise from here
#define TAU 6.2831853

float dg_hash(vec2 i) {
	vec3 p = fract(vec3(mod(i, 256.0).xyx) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Value noise, its four corners through one loop.
float dg_noise(vec2 x) {
	vec2 i = floor(x), f = x - i;
	vec2 u = f * f * (3.0 - 2.0 * f);
	float n = 0.0;
	for(int k = 0; k < 4; k++) {
		vec2 o = vec2(k == 1 || k == 3 ? 1.0 : 0.0, k >= 2 ? 1.0 : 0.0);
		vec2 w = mix(1.0 - u, u, o);
		n += w.x * w.y * dg_hash(i + o);
	}
	return n;
}

float dg_fbm(vec2 p) {
	float s = 0.0;
	for(int o = 0; o < 3; o++) {
		float f = o == 0 ? 1.0 : (o == 1 ? 2.03 : 4.1);
		vec2 off = o == 0 ? vec2(0.0) : (o == 1 ? vec2(5.1, 1.7) : vec2(2.3, 7.9));
		s += (o == 0 ? 0.55 : (o == 1 ? 0.3 : 0.15)) * dg_noise(p * f + off);
	}
	return s;
}

// Ember red, orange, yellow, a warm near-white at the hottest.
vec3 dg_ramp(float h) {
	h = clamp(h, 0.0, 1.0);
	vec3 c = mix(vec3(0.25, 0.03, 0.0), vec3(0.75, 0.12, 0.01), clamp(h / 0.3, 0.0, 1.0));
	c = mix(c, vec3(1.0, 0.42, 0.05), clamp((h - 0.3) / 0.25, 0.0, 1.0));
	c = mix(c, vec3(1.0, 0.70, 0.20), clamp((h - 0.55) / 0.2, 0.0, 1.0));
	c = mix(c, vec3(1.0, 0.88, 0.50), clamp((h - 0.75) / 0.15, 0.0, 1.0));
	return mix(c, vec3(1.0, 0.98, 0.88), clamp((h - 0.9) / 0.1, 0.0, 1.0));
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec4 tex = getTexel(uv);
	vec2 tc = uv * DG_SIZE;
	vec2 cell = floor(tc) + 0.5;             // every value at the texel's centre: it stays pixel art
	float t = mod(timer, 512.0);
	vec3 q = floor(tex.rgb * 255.0 + 0.5);
	float solid = step(0.5, tex.a);
	float role = mod(q.b, 4.0) * solid;
	float molten = role == 1.0 ? 1.0 : 0.0;
	float lit = role == 2.0 ? 1.0 : 0.0;
	float spill = mod(q.g, 4.0) / 3.0 * solid * (1.0 - molten) * (1.0 - lit);
	float hb = mod(q.r, 8.0) / 7.0;

	// the visor's light: a slow breath and a quick flicker, shared by the lava, the neck and the firelight
	float breath = 0.5 + 0.5 * sin(TAU * fract(t * 0.45));
	float glow = 0.62 + 0.28 * breath + 0.14 * (dg_noise(vec2(t * 7.0, 0.5)) - 0.5);

	vec3 rgb = tex.rgb * color.rgb;
	float shim = 0.85 + 0.3 * (dg_noise(vec2(cell.x * 0.45 + 1.0, cell.y * 0.35 - t * 2.2)) - 0.5);
	rgb = mix(rgb, tex.rgb * glow * shim * 1.25, lit);
	rgb += vec3(1.0, 0.35, 0.05) * (0.25 + 1.4 * max(max(tex.r, tex.g), tex.b)) * spill * glow * 0.55;

	if(molten > 0.0) {
		// three layers through one call site: two rising churns and the crust
		float n1 = 0.0, n2 = 0.0, cr = 0.0;
		for(int j = 0; j < 3; j++) {
			vec2 p = j == 0 ? vec2(cell.x * 0.5, cell.y * 0.38 + t * 0.9)
				: (j == 1 ? vec2(cell.x * 0.9 + 3.0 - t * 0.25, cell.y * 0.7 + t * 1.4)
				: vec2(cell.x * 0.8 + 9.0, cell.y * 0.6 + t * 0.5));
			float v = dg_fbm(p);
			n1 = j == 0 ? v : n1;
			n2 = j == 1 ? v : n2;
			cr = j == 2 ? v : cr;
		}
		float churn = 0.6 * n1 + 0.4 * n2;
		float heat = 0.24 + 0.72 * hb + (0.32 + 0.3 * hb) * (churn - 0.45) + 0.1 * (glow - 0.62);
		float crust = clamp((cr - 0.64) / 0.04, 0.0, 1.0) * step(hb, 0.45);
		rgb = dg_ramp(heat * (1.0 - 0.3 * crust));
	}

	// embers: each lights the one texel it is in, dimming as it rises
	float ember = 0.0;
	vec2 id = floor(tc);
	for(int i = 0; i < 7; i++) {
		float fi = float(i);
		float ph = fract(t * 0.55 + fi / 7.0);
		float ex = DG_ANCHOR.x + (dg_hash(vec2(fi, 3.0)) - 0.5) * 9.0 + 1.5 * sin(TAU * fract(ph * 1.3 + fi * 0.37));
		float ey = DG_ANCHOR.y + 4.0 - ph * 22.0;
		if(floor(ex) == id.x && floor(ey) == id.y)
			ember = max(ember, pow(1.0 - ph, 1.3));
	}
	float a = tex.a;
	if(ember > 0.15) {
		rgb = dg_ramp(0.55 + 0.35 * ember);
		a = 1.0;
	}
	return vec4(rgb, a * color.a);
}
