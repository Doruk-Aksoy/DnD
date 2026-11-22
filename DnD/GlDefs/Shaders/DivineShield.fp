// Divine Shield's bubble: clear face-on, gold where seen edge-on, three tilted bands of runes (the skin's strip) turning
// over it, soft veils of light. Alpha (Add): 0.5..1 its health, cracks spreading as it falls; under 0.5 its fade.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define BAND    0.075   // a rune band's half width on the unit sphere
#define TURN    0.018   // turns a second of the bands round their axes
#define CRACK   70.0    // the cracks' cell size on the skin, map units
#define NEAR0   12.0
#define NEAR1   40.0
#define TAU     6.2831853

float ds_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float ds_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float v = 0.0;
	for(int c = 0; c < 8; c++) {
		vec3 o = vec3(mod(float(c), 2.0), mod(floor(float(c) * 0.5), 2.0), floor(float(c) * 0.25));
		vec3 w = mix(1.0 - f, f, o);
		v += w.x * w.y * w.z * ds_hash(i + o);
	}
	return v;
}

// The two nearest feature points' distances of a 3D cell lattice: their difference is small along cell edges.
vec2 ds_cells(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	float d1 = 8.0;
	float d2 = 8.0;
	for(int k = 0; k < 27; k++) {
		vec3 o = vec3(mod(float(k), 3.0), mod(floor(float(k) / 3.0), 3.0), floor(float(k) / 9.0)) - 1.0;
		vec3 c = i + o;
		vec3 pt = o + vec3(ds_hash(c), ds_hash(c + 17.0), ds_hash(c + 41.0)) * 0.8 + 0.1;
		float d = length(pt - f);
		if(d < d1) {
			d2 = d1;
			d1 = d;
		}
		else if(d < d2)
			d2 = d;
	}
	return vec2(d1, d2);
}

// One band of runes round the axis ax: the strip read along the band; the angle's seam is hidden by taking whichever
// of two wrappings is smoother over the pixel quad.
float ds_band(vec3 n, vec3 ax, float spin) {
	float h = dot(n, ax);
	if(abs(h) > BAND)
		return 0.0;
	vec3 u = normalize(cross(ax, abs(ax.y) < 0.9 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0)));
	vec3 v = cross(ax, u);
	float a = atan(dot(n, v), dot(n, u)) / TAU + spin;
	float a1 = fract(a);
	float a2 = fract(a + 0.5) - 0.5;
	float s = fwidth(a1) < fwidth(a2) + 0.0001 ? a1 : a2;
	return getTexel(vec2(s, 0.5 - h / BAND * 0.5)).r;
}

vec4 Process(vec4 color) {
	vec3 o = gl_TextureMatrix[7][3].xyz;
	float R = 64.0 * length(gl_TextureMatrix[7][0].xyz);
	float t = mod(timer, 512.0);
	float a = color.a;
	float hp = a >= 0.5 ? a * 2.0 - 1.0 : 1.0;
	float fade = a >= 0.5 ? 1.0 : a * 2.0;

	// Round in the view: the model is flattened 1.2, so heights are measured x1.2 back.
	vec3 sq = vec3(1.0, 1.2, 1.0);
	vec3 p = (pixelpos.xyz - o) * sq;
	vec3 e = (camerapos - o) * sq;
	bool inside = dot(e, e) < R * R;
	if((gl_TexCoord[0].t < 1.0) == inside)
		discard;
	vec3 n = normalize(p);
	vec3 vd = normalize(pixelpos.xyz - camerapos);
	float rim = 1.0 - abs(dot(n, vd));
	rim = rim * rim * sqrt(rim);

	// Three bands of runes on tilted axes, each turning its own way.
	float runes = ds_band(n, vec3(0.0, 1.0, 0.0), fract(t * TURN));
	runes += ds_band(n, normalize(vec3(0.85, 0.5, 0.2)), fract(0.3 - t * TURN * 0.8));
	runes += ds_band(n, normalize(vec3(-0.5, 0.45, 0.75)), fract(0.6 + t * TURN * 1.2));
	runes = min(runes, 1.0);

	// Soft veils of light drifting over the skin.
	float veil = ds_noise(n * 2.6 + vec3(0.0, t * 0.12, t * 0.05));
	veil = smoothstep(0.5, 0.85, veil) * (0.06 + 0.3 * rim);

	// Damage: connected fractures spreading from three points as the health falls, thinning toward their ends.
	vec3 wq = n * R / CRACK;
	wq += (vec3(ds_noise(wq * 4.0), ds_noise(wq * 4.0 + 9.0), ds_noise(wq * 4.0 + 21.0)) - 0.5) * 0.18;
	vec2 k = ds_cells(wq + vec3(3.1, 7.7, 1.3));
	float near = max(max(dot(n, normalize(vec3(-0.7, 0.35, 0.3))), dot(n, normalize(vec3(0.6, 0.2, -0.75)))), dot(n, normalize(vec3(0.1, 0.8, 0.55))));
	float reach = (1.0 - hp) * 1.35;
	float spread = smoothstep(1.0 - reach, 1.0 - reach + 0.25, near);
	float crack = (1.0 - smoothstep(0.0, 0.012 + 0.02 * spread, k.y - k.x)) * spread;
	float flick = 0.6 + 0.4 * ds_hash(vec3(floor(t * 18.0), floor(near * 40.0), 3.0));

	vec3 gold = mix(vec3(1.0, 0.45, 0.15), vec3(1.0, 0.8, 0.42), hp);
	float glow = 0.012 + rim * 0.7 + veil + runes * (0.35 + 0.5 * rim) * (0.7 + 0.3 * hp);
	// from inside it stays faint, so the caster still sees out
	float view = inside ? 0.35 : 1.0;
	vec3 col = (gold * glow * (0.55 + 0.45 * hp) + vec3(1.0, 0.88, 0.65) * crack * flick * (0.45 + 0.5 * rim)) * view;
	float nf = smoothstep(NEAR0, NEAR1, length(pixelpos.xyz - camerapos));
	return vec4(min(col, vec3(1.0)) * fade * nf, 1.0);
}
