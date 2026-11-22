// Holy Wrath's burst: a ring of holy fire sweeping out, white-gold at its foot, orange tongues licking up to red tips.
// HWRING.md3 is an upright band: s runs once round it, t from its foot (0) to its top (1); both faces carry it.

uniform float timer;

#define TONGUES 44.0    // flame tongues round the ring (whole, so the noise wraps)
#define RISE    2.6     // how fast the fire licks upward
#define TAU     6.2831853

float hf_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float hf_noise(vec2 x, float per) {
	vec2 i = floor(x);
	vec2 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float x0 = mod(i.x, per);
	float x1 = mod(i.x + 1.0, per);
	float a = hf_hash(vec3(x0, i.y, 5.0));
	float b = hf_hash(vec3(x1, i.y, 5.0));
	float c = hf_hash(vec3(x0, i.y + 1.0, 5.0));
	float d = hf_hash(vec3(x1, i.y + 1.0, 5.0));
	return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

vec4 Process(vec4 color) {
	float s = fract(gl_TexCoord[0].s);
	float h = clamp(gl_TexCoord[0].t, 0.0, 1.0);
	float t = mod(timer, 512.0);

	// Each tongue's height flickers; the fire's edge wavers as it climbs.
	float th = 0.3 + 0.7 * hf_noise(vec2(s * TONGUES, t * 2.3), TONGUES);
	float lick = hf_noise(vec2(s * TONGUES * 2.0, h * 3.0 - t * RISE), TONGUES * 2.0);
	float edge = th + (lick - 0.5) * 0.35;
	float body = 1.0 - smoothstep(edge - 0.25, edge, h);
	// apart into separate tongues above the foot
	float cell = fract(s * TONGUES);
	float tongue = 1.0 - smoothstep(0.18, 0.5, abs(cell - 0.5) + (lick - 0.5) * 0.15);
	body *= mix(1.0, tongue, smoothstep(0.12, 0.45, h));
	float heat = clamp(1.0 - h / max(edge, 0.05), 0.0, 1.0);
	float foot = exp(-h * h / 0.004);

	vec3 col = mix(vec3(0.75, 0.1, 0.03), vec3(1.0, 0.5, 0.12), smoothstep(0.0, 0.45, heat));
	col = mix(col, vec3(1.0, 0.92, 0.62), smoothstep(0.55, 1.0, heat));
	col = col * body * (0.65 + 0.5 * lick) + vec3(1.0, 0.85, 0.5) * foot * 0.9;
	return vec4(min(col, vec3(1.0)) * color.a, 1.0);
}
