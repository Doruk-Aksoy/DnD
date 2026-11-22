// Defiance's orbs on PWRGLOBE.md3's hull, the skin's red picking which: holy, a white heart in a gold corona with rays
// (Add); dark, a black heart in a churning violet rim (Translucent, its own alpha over the actor's 0.1 alpha test).

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define NEAR0   16.0    // map units from the eye where an orb starts to show, so a fresh release stays clear
#define NEAR1   56.0
#define HULL    18.0    // PWRGLOBE.md3's hull radius; shader units are world units (MODELDEF undoes the actor's scale)
#define TAU     6.2831853

float df_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Value noise wrapping every per along x, for angles round the orb.
float df_noise(vec2 x, float per) {
	vec2 i = floor(x);
	vec2 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float x0 = mod(i.x, per);
	float x1 = mod(i.x + 1.0, per);
	float a = df_hash(vec3(x0, i.y, 4.0));
	float b = df_hash(vec3(x1, i.y, 4.0));
	float c = df_hash(vec3(x0, i.y + 1.0, 4.0));
	float d = df_hash(vec3(x1, i.y + 1.0, 4.0));
	return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
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
	// one shell only: the outer from outside, the inner from within
	if((gl_TexCoord[0].t < 1.0) == (dot(e, e) < HULL * HULL))
		discard;

	// The ray's nearest pass to the middle: how far (b) and at what angle round the view axis (ang).
	float tm = max(-dot(e, d), 0.0);
	vec3 q = e + d * tm;
	float b = length(q);
	vec3 su = normalize(cross(d, vec3(0.0, 1.0, 0.0)) + vec3(0.0001, 0.0, 0.0));
	vec3 sv = cross(su, d);
	float ang = atan(dot(q, sv), dot(q, su)) / TAU + 0.5;
	float t = mod(timer, 512.0);
	float seed = floor(df_hash(floor(mm[3].xyz)) * 64.0);
	bool holy = getTexel(vec2(0.5, 0.5)).r > 0.5;
	float near = smoothstep(NEAR0, NEAR1, length(we));

	if(holy) {
		vec3 col;
		float core = exp(-pow(b / 4.0, 4.0));
		float corona = exp(-b / 2.4);
		float r = df_noise(vec2(ang * 18.0, t * 1.4 + seed), 18.0);
		float rays = r * r * r * exp(-b / 4.5) * smoothstep(3.0, 6.0, b);
		float pulse = 0.9 + 0.1 * sin(TAU * fract(t * 2.3 + seed * 0.13));
		col = vec3(1.0, 0.98, 0.9) * core * 1.2 + vec3(1.0, 0.75, 0.28) * corona * 1.3 * pulse + vec3(1.0, 0.85, 0.5) * rays * 1.4
			+ vec3(0.45, 0.75, 1.0) * exp(-(b - 8.0) * (b - 8.0) / 6.0) * 0.1;
		col *= 1.0 - smoothstep(11.0, 16.0, b);
		return vec4(min(col, vec3(1.0)) * color.a * near, 1.0);
	}
	else {
		vec3 col;
		float a;
		// the heart: black, a hint of violet at its edge
		float heart = 1.0 - smoothstep(5.5, 6.6, b);
		float sw = df_noise(vec2(ang * 10.0 - t * 1.6, b * 0.6 - t * 0.9 + seed), 10.0);
		float sw2 = df_noise(vec2(ang * 22.0 + t * 2.2, b * 1.3 + seed), 22.0);
		float rw = (b - 6.6) / (1.0 + 1.6 * sw);
		float rim = exp(-rw * rw) * (0.55 + 0.6 * sw2);
		float haze = exp(-max(b - 6.0, 0.0) / 3.4) * (0.4 + 0.6 * sw) * (1.0 - heart);
		col = vec3(0.04, 0.0, 0.07) * heart + vec3(0.62, 0.22, 1.0) * rim * 1.1 + vec3(0.18, 0.04, 0.32) * haze;
		a = clamp(heart * 0.96 + rim * 0.9 + haze * 0.7, 0.0, 1.0);
		a *= (1.0 - smoothstep(HULL - 3.0, HULL, b)) * near;
		if(a < 0.01)
			discard;
		return vec4(min(col / max(a, 0.001), vec3(1.0)) * mix(color.rgb, vec3(1.0), 0.8), a);
	}
}
