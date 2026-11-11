// A minion weapon burning: flames licking up from its outline (exact colour), green or red by the outline and tint.
// Green outline (0, 252, 0), whose KNRA tint lands at green 245 and burns red; or a red outline (252, 0, 0). Per texel.

uniform float timer;

#define KF_SIZE vec2(175.0, 180.0)          // the canvas every live frame sits on, in texels
#define KF_REACH 9                      // texels a tongue can rise above the outline
#define KF_RED_OUTLINE 1               // 1: the outline is (252, 0, 0) and always burns red (Death Knight)
#define TAU 6.2831853

float kf_hash(vec2 i) {
	vec3 p = fract(vec3(mod(i, 256.0).xyx) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Value noise, its four corners through one loop.
float kf_noise(vec2 x) {
	vec2 i = floor(x), f = x - i;
	vec2 u = f * f * (3.0 - 2.0 * f);
	float n = 0.0;
	for(int k = 0; k < 4; k++) {
		vec2 o = vec2(k == 1 || k == 3 ? 1.0 : 0.0, k >= 2 ? 1.0 : 0.0);
		vec2 w = mix(1.0 - u, u, o);
		n += w.x * w.y * kf_hash(i + o);
	}
	return n;
}

// The outline at a texel: 0 none, 1 green, 2 red (the tinted alias, or a red outline that always burns red).
float kf_outline(vec2 cell) {
	vec4 c = getTexel((clamp(cell, vec2(0.0), KF_SIZE - 1.0) + 0.5) / KF_SIZE);
	vec3 q = floor(c.rgb * 255.0 + 0.5);
#if KF_RED_OUTLINE
	return (c.a < 0.5 || q.g > 2.0 || q.b > 2.0 || q.r < 238.0) ? 0.0 : 2.0;
#else
	if(c.a < 0.5 || q.r > 2.0 || q.b > 2.0 || q.g < 238.0)
		return 0.0;
	return q.g < 248.5 ? 2.0 : 1.0;
#endif
}

// Green spectral fire, the red throw's, or crimson (red outline): dark at the tips, a pale core at the hottest.
vec3 kf_ramp(float h, float red) {
	h = clamp(h, 0.0, 1.0);
#if KF_RED_OUTLINE
	vec3 r = mix(vec3(0.16, 0.0, 0.02), vec3(0.52, 0.01, 0.05), clamp(h / 0.3, 0.0, 1.0));
	r = mix(r, vec3(0.82, 0.03, 0.09), clamp((h - 0.3) / 0.25, 0.0, 1.0));
	r = mix(r, vec3(0.98, 0.12, 0.16), clamp((h - 0.55) / 0.2, 0.0, 1.0));
	r = mix(r, vec3(1.0, 0.38, 0.38), clamp((h - 0.75) / 0.15, 0.0, 1.0));
	return mix(r, vec3(1.0, 0.72, 0.70), clamp((h - 0.9) / 0.1, 0.0, 1.0));
#endif
	if(red > 0.5) {
		vec3 c = mix(vec3(0.22, 0.01, 0.0), vec3(0.70, 0.05, 0.01), clamp(h / 0.3, 0.0, 1.0));
		c = mix(c, vec3(0.98, 0.20, 0.03), clamp((h - 0.3) / 0.25, 0.0, 1.0));
		c = mix(c, vec3(1.0, 0.42, 0.08), clamp((h - 0.55) / 0.2, 0.0, 1.0));
		c = mix(c, vec3(1.0, 0.68, 0.30), clamp((h - 0.75) / 0.15, 0.0, 1.0));
		return mix(c, vec3(1.0, 0.88, 0.65), clamp((h - 0.9) / 0.1, 0.0, 1.0));
	}
	vec3 g = mix(vec3(0.02, 0.16, 0.04), vec3(0.05, 0.48, 0.10), clamp(h / 0.35, 0.0, 1.0));
	g = mix(g, vec3(0.22, 0.92, 0.20), clamp((h - 0.35) / 0.25, 0.0, 1.0));
	g = mix(g, vec3(0.60, 1.00, 0.45), clamp((h - 0.6) / 0.2, 0.0, 1.0));
	return mix(g, vec3(0.90, 1.00, 0.82), clamp((h - 0.8) / 0.2, 0.0, 1.0));
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec4 tex = getTexel(uv);
	vec2 cell = floor(uv * KF_SIZE);
	float t = mod(timer, 512.0);
	float glow = 0.75 + 0.25 * kf_noise(vec2(t * 6.0, 0.5));

	// tongues: the outline up to KF_REACH texels below, the column swaying more the higher it climbs
	float len = float(KF_REACH) * (0.3 + 0.7 * kf_noise(vec2(cell.x * 0.6, t * 2.2)));
	float sway = 1.4 * sin(TAU * fract(t * 1.7 + cell.y * 0.11 + cell.x * 0.05));
	float f = 0.0, red = 0.0, own = 0.0;
	for(int k = 0; k <= KF_REACH; k++) {
		float fk = float(k);
		float e = kf_outline(cell + vec2(floor(sway * fk / float(KF_REACH) + 0.5), fk));
		own = k == 0 ? e : own;
		red = e > 1.5 ? 1.0 : red;
		f = max(f, step(0.5, e) * clamp(1.0 - fk / max(len, 0.001), 0.0, 1.0));
	}
	f = clamp(f * (0.55 + 0.6 * kf_noise(vec2(cell.x * 0.8, cell.y * 0.5 + t * 3.5))), 0.0, 1.0);

	float solid = step(0.5, tex.a);
	vec3 rgb = tex.rgb * color.rgb;
	// firelight on the blade and the hand round the outline
	float near = 0.0;
	for(int j = 0; j < 9; j++) {
		vec2 o = vec2(mod(float(j), 3.0) - 1.0, floor(float(j) / 3.0) - 1.0);
		float e = kf_outline(cell + o);
		near += step(0.5, e);
		red = e > 1.5 ? 1.0 : red;
	}
	rgb += kf_ramp(0.6, red) * clamp(near / 9.0 * 2.5, 0.0, 1.0) * 0.35 * glow * solid * (1.0 - step(0.5, own));

	float a = tex.a;
	if(f > 0.18) {
		vec3 fl = kf_ramp(f * (0.9 + 0.2 * glow), red);
		float w = own > 0.5 ? 1.0 : (solid > 0.5 ? 0.65 : 1.0);
		rgb = mix(rgb, fl, w);
		a = 1.0;
	}
	return vec4(rgb, a * color.a);
}
