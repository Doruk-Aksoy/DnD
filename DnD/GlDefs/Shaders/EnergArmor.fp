// Energized Armor's shell on DSHELL.md3 (outer surface only, from outside): faint violet-blue at the edges, soft electric
// filaments crawling over it and flickering. Add; the actor's alpha is its strength (a flash as it forms, a swell as it bursts).

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define NEAR0   10.0
#define NEAR1   36.0
#define TAU     6.2831853

float ea_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float ea_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float v = 0.0;
	for(int c = 0; c < 8; c++) {
		vec3 o = vec3(mod(float(c), 2.0), mod(floor(float(c) * 0.5), 2.0), floor(float(c) * 0.25));
		vec3 w = mix(1.0 - f, f, o);
		v += w.x * w.y * w.z * ea_hash(i + o);
	}
	return v;
}

vec4 Process(vec4 color) {
	vec3 o = gl_TextureMatrix[7][3].xyz;
	float R = 64.0 * length(gl_TextureMatrix[7][0].xyz);
	float t = mod(timer, 512.0);
	// round in the view: the model is flattened 1.2, so heights are measured x1.2 back
	vec3 sq = vec3(1.0, 1.2, 1.0);
	vec3 p = (pixelpos.xyz - o) * sq;
	vec3 e = (camerapos - o) * sq;
	if(dot(e, e) < R * R || gl_TexCoord[0].t >= 1.0)
		discard;
	vec3 n = normalize(p);
	vec3 vd = normalize(pixelpos.xyz - camerapos);
	float rim = 1.0 - abs(dot(n, vd));
	rim = rim * rim;

	// filaments: a warped field's ridges, two scales drifting different ways
	vec3 q = n * 2.4;
	vec3 w = vec3(ea_noise(q + vec3(0.0, t * 0.7, 0.0)), ea_noise(q + vec3(5.2, 0.0, t * 0.6)), ea_noise(q + vec3(t * 0.5, 9.1, 0.0))) - 0.5;
	q += w * 0.9;
	float r1 = 1.0 - abs(2.0 * ea_noise(q * 2.2 + vec3(0.0, 0.0, t * 1.1)) - 1.0);
	float r2 = 1.0 - abs(2.0 * ea_noise(q * 4.5 - vec3(t * 1.7, 0.0, 0.0)) - 1.0);
	float arcs = pow(r1, 12.0) + pow(r2, 16.0) * 0.6;
	float flick = 0.65 + 0.35 * ea_hash(vec3(floor(t * 14.0), 1.0, 2.0));

	vec3 col = vec3(0.35, 0.3, 1.0) * (0.03 + rim * 0.55) + vec3(0.55, 0.85, 1.0) * arcs * flick * (0.5 + 0.5 * rim) +
		vec3(1.0) * arcs * arcs * 0.4;
	float nf = smoothstep(NEAR0, NEAR1, length(pixelpos.xyz - camerapos));
	return vec4(min(col, vec3(1.0)) * color.a * nf, 1.0);
}
