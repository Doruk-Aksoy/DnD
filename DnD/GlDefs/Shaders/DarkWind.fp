// Dark Wind: a thick, dark fog rolling round its line of flight, wisps streaming off behind, a faint violet light deep
// inside. Ray-marched in DWIND.md3's hull; alpha-blended, the actor's alpha only lowers the alpha test and fades it.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

// Units before the model's scale, model space x forward, y up (x1.2), z sideways.
#define HULL_C    vec3(-40.0, 0.0, 0.0)    // the ellipsoid the fog lives in, centre and half axes;
#define HULL_R    vec3(100.0, 38.0, 54.0)   // DWIND.md3's facets sit just outside it
#define BILLOW    14.0     // size of the rolling billows across the flight, stretched BILLOW_L along it
#define BILLOW_L  30.0
#define WISP_W    5.0      // the long wisps across and along the flight
#define WISP_L    70.0
#define FLOW      32.0     // units a second the fog slides back along the flight
#define SPIN      0.13     // turns a second it rolls round the flight, and the extra twist per unit along it
#define TWIST     0.012
#define K_FOG     0.09     // opacity per unit of fog density
#define TAIL      0.6      // how thin the back of it gets against the front
#define NEAR0     48.0     // map units from the eye (to the model, and along each ray) where it starts to show
#define NEAR1     100.0
#define RAY0      20.0
#define RAY1      64.0
#define STEPS     18
#define ALPHA_REF 0.02     // the actor's alpha while whole: below it the fog fades out (its Death)
#define DEEP      vec3(0.022, 0.02, 0.03)
#define MIST      vec3(0.15, 0.14, 0.18)
#define GLOW      vec3(0.22, 0.13, 0.42)
#define SELF      0.24     // the mist's own faint light, so it still shows in the dark

// Rule-1 portable hash: wrapped to 256, no sin.
float dw_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Smooth value noise 0..1, the eight corners in one loop.
float dw_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float v = 0.0;
	for(int c = 0; c < 8; c++) {
		vec3 o = vec3(mod(float(c), 2.0), mod(floor(float(c) * 0.5), 2.0), floor(float(c) * 0.25));
		vec3 w = mix(1.0 - f, f, o);
		v += w.x * w.y * w.z * dw_hash(i + o);
	}
	return v;
}

// Fog at q: x density, y how deep in a billow (for the glow). Billows roll round the flight and slide back, long wisps
// stream along it at the edges; the whole thins toward the back and fades before the hull.
vec2 dw_fog(vec3 q, float t, vec3 seed) {
	vec3 h = (q - HULL_C) / HULL_R;
	float s = length(h);
	if(s > 0.98)
		return vec2(0.0, 0.0);
	float a = 6.2831853 * fract(t * SPIN) + q.x * TWIST;
	float ca = cos(a), sa = sin(a);
	vec3 r = vec3(q.x + t * FLOW, q.y * ca - q.z * sa, q.y * sa + q.z * ca);
	float n = 0.0;
	float wisp = 0.0;
	for(int o = 0; o < 3; o++) {
		vec3 b = vec3(r.x / BILLOW_L, r.y / BILLOW, r.z / BILLOW);
		vec3 x = o == 0 ? b : o == 1 ? b * 2.03 + vec3(5.1, 1.7, 8.3) : vec3(r.x / WISP_L, r.y / WISP_W, r.z / WISP_W) + vec3(3.3, 9.1, 2.6);
		float v = dw_noise(x + seed);
		if(o == 2)
			wisp = v;
		else
			n += o == 0 ? 0.65 * v : 0.35 * v;
	}
	// Billows in the front half, giving way to wisps streaming off behind.
	float back = smoothstep(HULL_C.x + 0.4 * HULL_R.x, HULL_C.x - 0.9 * HULL_R.x, q.x);
	float v = n * (1.5 - 0.6 * back) + 0.1 + (0.35 + 0.55 * back) * smoothstep(0.45, 0.85, wisp) - 1.1 * s;
	float dens = smoothstep(0.0, 0.35, v) * (1.0 - smoothstep(0.75, 0.98, s)) * mix(1.0, TAIL, back);
	return vec2(dens, smoothstep(0.45, 1.0, v));
}

vec4 Process(vec4 color) {
	// The eye and this point in model space; heights x1.2 against the view's stretch.
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

	// Clear of the caster's face while it leaves, and faded with the actor's alpha as it dies.
	float fade = smoothstep(NEAR0, NEAR1, length(we)) * clamp(color.a / ALPHA_REF, 0.0, 1.0);
	if(fade <= 0.0)
		discard;

	// One shell per pixel: the outer (t 0.5) from outside, the inner (t 1.5) with the eye inside the hull.
	vec3 oc = (e - HULL_C) / HULL_R;
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

	// Each wind its own fog, from the shape number its scale carries (ScaleX 1 + k x 0.004).
	float k = floor((length(mm[0].xyz) - 1.0) / 0.004 + 0.5);
	vec3 seed = vec3(k * 7.31, k * 3.17, k * 5.53);
	float t = mod(timer, 512.0);

	// Front to back; each pixel starts a different fraction in, grain instead of slices.
	vec3 lit = vec3(0.0, 0.0, 0.0);
	vec3 glow = vec3(0.0, 0.0, 0.0);
	float A = 0.0;
	float jit = dw_hash(vec3(floor(gl_FragCoord.xy), 3.0));
	float dt = (t1 - t0) / float(STEPS);
	for(int i = 0; i < STEPS; i++) {
		float ti = t0 + (float(i) + jit) * dt;
		vec3 q = e + d * ti;
		vec2 f = dw_fog(q, t, seed);
		float a = (1.0 - exp(-f.x * K_FOG * dt)) * smoothstep(RAY0, RAY1, ti);
		// Thin fog at the top and the rim catches the light, the cores stay dark, faint light deep in them.
		float top = smoothstep(-0.8, 0.9, (q.y - HULL_C.y) / HULL_R.y);
		vec3 c = mix(DEEP, MIST, clamp(0.35 * top + 0.65 * (1.0 - f.y), 0.0, 1.0) * (0.55 + 0.45 * top));
		lit += (1.0 - A) * a * c;
		glow += (1.0 - A) * a * (GLOW * f.y * f.y + c * SELF);
		A += (1.0 - A) * a;
		if(A > 0.97)
			break;
	}
	if(A * fade < 0.01)
		discard;
	return vec4(clamp((lit * color.rgb + glow) / A, 0.0, 1.0), A * fade);
}
