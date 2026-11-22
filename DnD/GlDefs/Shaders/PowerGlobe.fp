// Power Globe: a dark globe torn by churning filaments of violet energy round a flickering white core, embers
// spiralling in, a crackling ring whose tilted plane turns slowly. Ray-marched in a sphere hull, additive.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

// Units before the model's scale, model space x forward, y up, z sideways, heights x1.2 so it reads as drawn.
#define HULL_R  vec3(18.0, 18.0, 18.0)   // the sphere everything is drawn inside; PWRGLOBE.md3's facets sit just outside it
#define RG      9.0      // the globe's radius and its white core's
#define CORE_R  1.6
#define RING_R  14.0     // the ring's radius and half thickness, how fast its plane turns and its energy streams round
#define RING_W  1.1
#define RING_TURN 0.07
#define RING_HZ 0.35
#define TILT    0.55     // the ring plane's lean from level, radians
#define EMBERS  14       // embers spiralling into the core, each taking 1 / EMBER_HZ seconds
#define EMBER_HZ 0.45
#define K_GLOW  0.42     // brightness per unit of glow
#define NEAR0   20.0     // map units from the eye where the globe starts to show, so a fresh cast stays clear
#define NEAR1   56.0
#define STEPS   26
#define TAU     6.2831853

float pg_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Smooth value noise, 0 to 1: the eight corners in one loop, one hash call site.
float pg_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float v = 0.0;
	for(int c = 0; c < 8; c++) {
		vec3 o = vec3(mod(float(c), 2.0), mod(floor(float(c) * 0.5), 2.0), floor(float(c) * 0.25));
		vec3 w = mix(1.0 - f, f, o);
		v += w.x * w.y * w.z * pg_hash(i + o);
	}
	return v;
}

// Sharp bright lines where the noise crosses its middle.
float pg_ridge(float n) {
	return 1.0 - abs(2.0 * n - 1.0);
}

vec3 pg_roty(vec3 p, float a) {
	float c = cos(a);
	float s = sin(a);
	return vec3(c * p.x - s * p.z, p.y, s * p.x + c * p.z);
}

// p turned by a about the unit axis k.
vec3 pg_turn(vec3 p, vec3 k, float a) {
	float c = cos(a);
	float s = sin(a);
	return p * c + cross(k, p) * s + k * dot(k, p) * (1.0 - c);
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

	float fade = smoothstep(NEAR0, NEAR1, length(we));
	if(fade <= 0.0)
		discard;
	float t = mod(timer, 512.0);

	// The ray inside the hull's sphere; the outer shell (t 0.5) from outside, the inner one (t 1.5) from within.
	vec3 oc = e / HULL_R;
	if((gl_TexCoord[0].t < 1.0) == (dot(oc, oc) < 1.0))
		discard;
	vec3 dc = d / HULL_R;
	float qa = dot(dc, dc);
	float hb = dot(oc, dc);
	float hd = hb * hb - qa * (dot(oc, oc) - 1.0);
	if(hd <= 0.0)
		discard;
	hd = sqrt(hd);
	float t0 = max((-hb - hd) / qa, 0.0);
	float t1 = (-hb + hd) / qa;

	// The ring's plane leans by TILT and the lean turns round the upright; its energy streams round it.
	float az = TAU * fract(t * RING_TURN);
	vec3 ax = vec3(sin(TILT) * cos(az), cos(TILT), sin(TILT) * sin(az));
	float ringa = TAU * fract(t * RING_HZ);
	float swirl = TAU * fract(t * 0.09);
	float flick = 0.7 + 0.6 * pg_noise(vec3(t * 7.0, 3.0, 1.0));

	float jit = pg_hash(vec3(floor(gl_FragCoord.xy), 7.0));
	float dt = (t1 - t0) / float(STEPS);
	vec3 C = vec3(0.0);
	for(int i = 0; i < STEPS; i++) {
		vec3 s = e + d * (t0 + (float(i) + jit) * dt);
		float r = length(s);

		// The body: a faint violet haze torn by filaments, warped by turbulence and twisted harder near the middle.
		if(r < RG) {
			float g = 1.0 - smoothstep(RG * 0.8, RG, r);
			vec3 q = pg_roty(s, swirl + 1.3 * (1.0 - r / RG)) * 0.32;
			float w1 = pg_noise(q * 1.4 + vec3(0.0, t * 0.5, 0.0));
			float w2 = pg_noise(q * 1.4 + vec3(5.2, -t * 0.4, 2.1));
			q += vec3(w1, w2, w1 - w2) * 2.0;
			// Two ridged fields are sheets; only where they cross is a filament, thin lines in 3D.
			float v = pow(pg_ridge(pg_noise(q * 1.1 + vec3(0.0, 0.0, t * 0.3))) * pg_ridge(pg_noise(q * 1.1 + vec3(7.0, 1.0, 3.0 - t * 0.25))), 5.0);
			float k = r / RG;
			vec3 vc = mix(vec3(1.0, 0.62, 1.0), vec3(0.95, 0.22, 0.85), smoothstep(0.0, 0.45, k));
			vc = mix(vc, vec3(0.5, 0.1, 1.0), smoothstep(0.45, 1.0, k));
			C += (vec3(0.22, 0.03, 0.45) * 0.1 * w1 + vc * v * 3.4) * g * dt;
		}

		// The flickering core, a hot thin rim and heat bleeding off it.
		C += (vec3(1.0, 0.85, 1.0) * exp(-r * r / (CORE_R * CORE_R)) * 4.0 + vec3(0.9, 0.2, 0.85) * exp(-r * r / 8.0) * 0.3) * flick * dt;
		// The rim and the heat off it light only where the ray meets the shell edge-on, so the face stays dark.
		float edge = 1.0 - abs(dot(s, d)) / max(r, 0.001);
		edge = edge * edge;
		C += vec3(0.75, 0.2, 1.0) * exp(-(r - RG) * (r - RG) / 0.6) * 0.75 * edge * dt;
		if(r > RG)
			C += vec3(0.45, 0.08, 0.9) * exp(-(r - RG) / 2.0) * 0.25 * edge * dt;

		// The ring: a band round the turning axis, crackling where its streaming noise crosses the middle.
		float h = dot(s, ax);
		float dr = length(s - h * ax) - RING_R;
		float rd = exp(-(dr * dr + h * h * 2.5) / (RING_W * RING_W));
		if(rd > 0.002) {
			float cr = pow(pg_ridge(pg_noise(pg_turn(s, ax, ringa) * 0.7 + vec3(7.0, 2.0, 4.0))), 4.0);
			C += mix(vec3(0.45, 0.08, 0.95), vec3(1.0, 0.75, 1.0), cr) * rd * (0.25 + 1.6 * cr) * 1.4 * dt;
		}
	}

	// Embers: points spiralling from the rim into the core, brightening as they fall, measured from the ray itself.
	vec3 su = normalize(cross(d, vec3(0.0, 1.0, 0.0)) + vec3(0.0001, 0.0, 0.0));
	vec3 sv = cross(su, d);
	float E = 0.0;
	for(int j = 0; j < EMBERS; j++) {
		float fj = float(j);
		float h1 = pg_hash(vec3(fj, 3.0, 9.0));
		float h2 = pg_hash(vec3(fj, 17.0, 5.0));
		float ph = fract(t * EMBER_HZ * (0.7 + 0.6 * h2) + h1);
		float y = 2.0 * pg_hash(vec3(fj, 29.0, 1.0)) - 1.0;
		float a = TAU * h2 + ph * 2.6;
		vec3 dir = vec3(sqrt(1.0 - y * y) * cos(a), y, sqrt(1.0 - y * y) * sin(a));
		vec3 w = dir * RG * 0.95 * (1.0 - ph) - e;
		float aa = dot(w, su);
		float bb = dot(w, sv);
		E = max(E, exp(-(aa * aa + bb * bb) / 0.06) * smoothstep(0.0, 0.15, ph) * (1.0 - smoothstep(0.85, 1.0, ph)) * (0.5 + ph));
	}

	vec3 col = 1.0 - exp(-C * K_GLOW);
	col = min(col + vec3(1.0, 0.75, 1.0) * E, vec3(1.0));
	if(max(col.r, col.b) < 0.004)
		discard;
	return vec4(col * fade * color.a, 1.0);
}
