// Pyroblast's bolt: flames ray-marched inside PYROBLAS.md3, a hull whose t is 0 outside, 1 inside (eye within).
// Engine inputs and GLSL 1.10 rules: .claude/notes/zan33-gl-shaders.md.

// NO_FOG marks the colormap pass, whose base declares neither.
#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

// Lengths in md3 units, before the model's scale; pyroblast.py builds the hull from the same numbers.
// Every density reaches exactly zero inside the hull, so its edge never shows.
#define HULL_R    15.5    // the hull: a ball round the head and a cone tangent to it, back past the tail
#define HULL_TIP  95.0
#define CORE      7.0     // the head's two blobs: the white core and the flame glow round it
#define GLOW      15.0
#define TAIL      72.0
#define TAIL_R    10.0    // the tail's radius at the head, before the flames swell or thin it
#define SPINE     3.0     // how far the tail's corkscrew strays from the line, at its end
#define WOBBLE    0.9     // corkscrew turns a second
#define FLOW      0.9     // how fast the flames stream back
#define SWIRL     0.12    // and twist round it
#define ACROSS    0.03    // the photo's scale: twist per unit out from the line, and along it
#define ALONG     0.011
#define BAND0     0.3     // the strip of the photo wrapped round the bolt: coarser than all of it
#define BAND      0.35
#define W_CORE    2.2     // brightness each part adds, before the 1 - exp roll-off
#define W_GLOW    2.8
#define W_TAIL    0.5
#define T_CORE    1.3     // heat per unit of that brightness: how far it pushes the colour up the ramp
#define T_GLOW    0.35
#define T_TAIL    0.45
#define COOL      0.7     // heat the tail loses by its end
#define RAMP_G0   0.15    // the ramp's green: G0 at no heat, rising toward G0 + G1, 63% of the way there at GK
#define RAMP_G1   0.75
#define RAMP_GK   2.2
#define NEAR0     20.0    // map units from the eye where the tail starts to show, so a fresh cast stays clear
#define NEAR1     56.0
#define STEPS     20

// Mirrored repeat, 0 to 1 and back every 2: the photo does not tile, so each edge meets its own reflection.
float pb_mirror(float x) {
	return abs(mod(x + 1.0, 2.0) - 1.0);
}

// A blob (1 - d^2/g^2)^2 integrated along a ray from s0 to s1 past its middle, b2 the miss squared.
// 1 straight through the middle; exactly 0 beyond g.
float pb_blob(float b2, float s0, float s1, float g) {
	float g2 = g * g;
	float c = 1.0 - b2 / g2;
	if(c <= 0.0)
		return 0.0;
	float w = g * sqrt(c);
	float a0 = clamp(s0, -w, w);
	float a1 = clamp(s1, -w, w);
	float f0 = c * c * a0 - 0.66666667 * c * a0 * a0 * a0 / g2 + a0 * a0 * a0 * a0 * a0 / (5.0 * g2 * g2);
	float f1 = c * c * a1 - 0.66666667 * c * a1 * a1 * a1 / g2 + a1 * a1 * a1 * a1 * a1 / (5.0 * g2 * g2);
	return (f1 - f0) * 0.9375 / g;
}

// Inside the hull's true shape. The mesh's facets sit just outside it, so the two never disagree.
bool pb_inside(vec3 p) {
	if(dot(p, p) <= HULL_R * HULL_R)
		return true;
	float xt = -HULL_R * HULL_R / HULL_TIP;
	float rt = HULL_R * sqrt(1.0 - HULL_R * HULL_R / (HULL_TIP * HULL_TIP));
	return p.x < xt && p.x > -HULL_TIP && length(p.yz) <= rt * (p.x + HULL_TIP) / (xt + HULL_TIP);
}

// Where a ray from e along d leaves the cylinder round the hull.
float pb_exit(vec3 e, vec3 d) {
	float a = dot(d.yz, d.yz);
	float b = dot(e.yz, d.yz);
	float c = dot(e.yz, e.yz) - HULL_R * HULL_R;
	float t = a > 1e-6 ? (-b + sqrt(max(b * b - a * c, 0.0))) / a : 1e6;
	if(d.x > 1e-5)
		t = min(t, (HULL_R - e.x) / d.x);
	else if(d.x < -1e-5)
		t = min(t, (-HULL_TIP - e.x) / d.x);
	return t;
}

// The photo wrapped round the bolt as two layers, twisting opposite ways and streaming back at different
// speeds. Round, not flat: a flat projection seen end on smears into stripes.
vec3 pb_flame(float x, vec2 yz) {
	float a = atan(yz.y, yz.x + 1e-6) * 0.31830989;
	float r = length(yz) * ACROSS;
	vec3 f1 = getTexel(vec2(BAND0 + BAND * pb_mirror(a + r + timer * SWIRL), pb_mirror(x * ALONG + timer * FLOW))).rgb;
	vec3 f2 = getTexel(vec2(BAND0 + BAND * pb_mirror(-a + r * 1.3 + 0.31 - timer * SWIRL * 0.7),
		pb_mirror(x * ALONG * 0.8 + timer * FLOW * 1.25 + 0.47))).rgb;
	return (f1 + f2) * 0.5;
}

// Heat to colour: dark red, orange, yellow; only the hottest heat, the core's middle, reaches white.
vec3 pb_heat(float t) {
	return vec3(1.0, RAMP_G0 + RAMP_G1 * (1.0 - exp(-t / RAMP_GK)), 0.03 + 0.55 * smoothstep(3.0, 6.0, t));
}

vec4 Process(vec4 color) {
	// The eye and this point in model space, back through the model-to-world matrix main.vp uses.
	mat4 m = gl_TextureMatrix[7];
	vec3 x12 = cross(m[1].xyz, m[2].xyz);
	vec3 x20 = cross(m[2].xyz, m[0].xyz);
	vec3 x01 = cross(m[0].xyz, m[1].xyz);
	float det = dot(m[0].xyz, x12);
	vec3 wp = pixelpos.xyz - m[3].xyz;
	vec3 we = camerapos - m[3].xyz;
	vec3 p = vec3(dot(x12, wp), dot(x20, wp), dot(x01, wp)) / det;
	vec3 e = vec3(dot(x12, we), dot(x20, we), dot(x01, we)) / det;
	float len = length(p - e);
	vec3 d = (p - e) / len;
	float k = distance(pixelpos.xyz, camerapos) / len;

	// Each pixel takes one face: the near outside one, or from within, the inside one ahead.
	bool within = pb_inside(e);
	if((gl_TexCoord[0].t < 1.0) == within)
		discard;
	float t0 = within ? 0.0 : len;
	float t1 = within ? len : max(pb_exit(e, d), len);

	// The head: both blobs integrated exactly along the ray, so it is round from every side.
	float tc = -dot(e, d);
	vec3 qh = e + d * clamp(tc, t0, t1);
	float b2 = dot(e + d * tc, e + d * tc);
	float core = pb_blob(b2, t0 - tc, t1 - tc, CORE);
	float glow = pb_blob(b2, t0 - tc, t1 - tc, GLOW);
	vec3 fh = pb_flame(qh.x, qh.yz);
	float nh = smoothstep(0.35, 0.85, dot(fh, vec3(0.3, 0.59, 0.11)));

	// The tail: marched, a tapering corkscrew of flame whose edge the photo breaks into tongues.
	float dt = (t1 - t0) / float(STEPS);
	float spin = 6.2831853 * timer * WOBBLE;
	float tb = 0.0;
	float th = 0.0;
	for(int i = 0; i < STEPS; i++) {
		float t = t0 + (float(i) + 0.5) * dt;
		vec3 q = e + d * t;
		float u = -q.x / TAIL;
		vec2 yz = q.yz - SPINE * u * vec2(sin(6.2831853 * u * 1.3 - spin), cos(6.2831853 * u * 1.1 - spin + 0.7));
		vec3 f = pb_flame(q.x, yz);
		float n = smoothstep(0.35, 0.85, dot(f, vec3(0.3, 0.59, 0.11)));
		float r = TAIL_R * clamp(1.0 - u, 0.0, 1.0) * (0.6 + 0.8 * n) + 0.5;
		float fall = max(1.0 - dot(yz, yz) / (r * r), 0.0);
		float along = clamp(1.0 - u, 0.0, 1.0) * smoothstep(-0.15, 0.02, u);
		float dens = fall * fall * along * (0.15 + 1.4 * n * n) * smoothstep(NEAR0, NEAR1, t * k);
		float s = dens * (0.5 + dot(f, vec3(0.3, 0.59, 0.11)));
		tb += s;
		th += s * (1.0 - COOL * clamp(u, 0.0, 1.0));
	}

	// Brightness rolls off alone and the heat picks the hue, so bright orange stays orange instead of clipping to yellow.
	float hb = core * W_CORE + glow * W_GLOW * (0.75 + 0.5 * nh);
	float bright = hb + tb * dt * W_TAIL;
	float heat = core * W_CORE * T_CORE + glow * W_GLOW * T_GLOW * (0.6 + 0.6 * nh) + th * dt * W_TAIL * T_TAIL;
	vec3 col = pb_heat(heat) * (1.0 - exp(-bright));

	float flicker = 0.92 + 0.08 * sin(timer * 31.0) * sin(timer * 17.0 + 1.3);
	return vec4(col * flicker * color.rgb, color.a);
}
