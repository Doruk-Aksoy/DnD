// Soulstorm on SSTORM.md3: a black hole ringed by a blazing, tilted accretion disc and a halo of its bent light, inside a
// pink cosmic cloud swirling in spiral arms; it all pulses. Ray-marched in the hull, alpha-blended (actor alpha = ALPHA_REF).

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define TAU       6.2831853
// Model space x forward, y up x1.2 (as the view draws it), z sideways; the actor stands 32 over the floor.
#define HULL_C    vec3(0.0, 72.0, 0.0)     // the ellipsoid it is drawn in, and the hole's centre
#define HULL_R    vec3(175.0, 95.0, 175.0)
#define HOLE      14.0     // the hole's radius; the disc runs from DISC0 to DISC1 holes out, tilted TILT
#define DISC0     1.45
#define DISC1     4.2
#define TILT      0.1 
#define CLOUD_R   165.0    // the cloud's reach, its half thickness at the middle and at the rim
#define THICK0    56.0
#define THICK1    16.0
#define ARMS      2.0      // spiral arms in the cloud, how tightly they wind, and its slow turn
#define WIND      0.045
#define SPIN      0.06
#define BEAT      0.55     // pulses a second
#define K_CLOUD   0.1      // opacity per unit of cloud density
#define RAY0      30.0     // nothing nearer the eye than this along a ray, whole by RAY1
#define RAY1      110.0
#define STEPS     22
#define ALPHA_REF 0.1      // the actor's alpha when whole; below it the storm shrinks to its heart and fades
#define PINK      vec3(0.92, 0.38, 0.68)
#define VIOLET    vec3(0.38, 0.1, 0.55)
#define DUSK      vec3(0.1, 0.03, 0.12)
#define DISC_HOT  vec3(1.0, 0.9, 0.95)
#define DISC_MID  vec3(1.0, 0.45, 0.75)
#define DISC_EDGE vec3(0.55, 0.15, 0.7)
#define STAR      vec3(1.0, 0.86, 0.95)    // the stars in the cloud: colour, cell size, share of cells lit
#define STAR_CELL 6.0
#define STAR_SHARE 0.1

// Rule-1 portable hash and value noise.
float ss_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float ss_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float v = 0.0;
	for(int c = 0; c < 8; c++) {
		vec3 o = vec3(mod(float(c), 2.0), mod(floor(float(c) * 0.5), 2.0), floor(float(c) * 0.25));
		vec3 w = mix(1.0 - f, f, o);
		v += w.x * w.y * w.z * ss_hash(i + o);
	}
	return v;
}

// The disc's plane: its normal, tilted about x.
vec3 ss_axis() {
	return vec3(0.0, cos(TILT), sin(TILT));
}

// The cloud at q (q from the hole's centre, in the cloud's own level frame): x density, y how near the heart.
vec2 ss_cloud(vec3 q, float t, float g) {
	float r = length(q.xz);
	float th = mix(THICK0, THICK1, clamp(r / CLOUD_R, 0.0, 1.0)) * g;
	float prof = 1.0 - smoothstep(0.35, 1.0, abs(q.y) / th);
	float rad = (1.0 - smoothstep(CLOUD_R * 0.65 * g, CLOUD_R * g, r)) * smoothstep(HOLE * 2.2 * g, HOLE * 5.0 * g, r);
	if(prof * rad <= 0.0)
		return vec2(0.0, 0.0);
	// Turns faster near the heart; the arms wind with distance.
	float a = atan(q.z, q.x + 1e-4) + TAU * fract(t * SPIN) * (1.0 + 60.0 / (r + 25.0));
	float arm = 0.5 + 0.5 * sin(ARMS * a + r * WIND * TAU);
	vec3 s = vec3(cos(a) * r, q.y * 1.6, sin(a) * r) * 0.035 + vec3(0.0, 0.0, t * 0.05);
	float n = 0.0;
	for(int o = 0; o < 2; o++)
		n += (o == 0 ? 0.65 : 0.35) * ss_noise(o == 0 ? s : s * 2.3 + vec3(7.1, 3.3, 1.9));
	float dens = smoothstep(0.22, 0.72, n * (0.6 + 0.55 * arm)) * prof * rad;
	return vec2(dens, 1.0 - smoothstep(HOLE * 3.0 * g, CLOUD_R * 0.8 * g, r));
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

	float fade = clamp(color.a / ALPHA_REF, 0.0, 1.0);
	if(fade <= 0.0)
		discard;
	// It grows out from its heart as it forms and draws back in as it goes.
	float g = 0.25 + 0.75 * fade;
	float t = mod(timer, 512.0);
	float ph = fract(t * BEAT);
	float pulse = exp(-ph * 5.0) + 0.35 * exp(-abs(ph - 0.45) * 12.0);

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

	// The hole: where the ray first enters it, and how close the ray passes its centre (for the bent-light halo).
	vec3 ec = e - HULL_C;
	float rh = HOLE * g * (0.94 + 0.08 * pulse);
	float bc = -dot(ec, d);
	vec3 cl = ec + d * bc;
	float bmin = length(cl);
	float th = 1e6;
	if(bmin < rh && bc > 0.0)
		th = bc - sqrt(rh * rh - bmin * bmin);
	float tend = min(t1, th);

	// The disc: where the ray crosses its plane, and its colour and opacity there.
	vec3 ax = ss_axis();
	float dn = dot(d, ax);
	float tp = abs(dn) > 1e-4 ? -dot(ec, ax) / dn : -1.0;
	vec4 disc = vec4(0.0);
	if(tp > t0 && tp < tend) {
		vec3 q = ec + d * tp;
		float r = length(q) / g;
		float u = (r / HOLE - DISC0) / (DISC1 - DISC0);
		if(u > 0.0 && u < 1.0) {
			vec3 bx = normalize(cross(ax, vec3(1.0, 0.0, 0.0)));
			vec3 by = cross(ax, bx);
			float a = atan(dot(q, by), dot(q, bx));
			// Inner rings turn fastest; streaks by noise along each ring.
			float sp = TAU * fract(t * 0.35 * pow(1.0 / (0.25 + u), 1.5));
			float n = ss_noise(vec3(cos(a + sp) * 3.0, sin(a + sp) * 3.0, u * 9.0));
			float streak = 0.55 + 0.6 * n;
			// The side turning towards the eye blazes brighter.
			vec3 tang = cross(ax, normalize(q));
			float dop = 1.0 + 0.3 * dot(tang, -d);
			vec3 c = mix(DISC_HOT, DISC_MID, smoothstep(0.0, 0.45, u));
			c = mix(c, DISC_EDGE, smoothstep(0.4, 1.0, u));
			float lum = streak * dop * (1.0 - smoothstep(0.75, 1.0, u)) * smoothstep(0.0, 0.06, u) * (0.75 + 0.45 * pulse);
			disc = vec4(c * lum * 0.9, clamp(lum * 0.55, 0.0, 0.7));
		}
	}

	// The cloud lies level, centred on the hole.
	vec3 acc = vec3(0.0);
	float A = 0.0;
	bool discDone = disc.a <= 0.0;
	float jit = ss_hash(vec3(floor(gl_FragCoord.xy), 9.0));
	float dt = (tend - t0) / float(STEPS);
	for(int i = 0; i < STEPS; i++) {
		float ti = t0 + (float(i) + jit) * dt;
		if(!discDone && ti > tp) {
			acc += (1.0 - A) * disc.rgb * disc.a;
			A += (1.0 - A) * disc.a;
			discDone = true;
		}
		vec3 q = ec + d * ti;
		vec2 c = ss_cloud(q, t, g);
		// The hole's pull clears a window: the cloud thins wherever the ray passes near it, so it always shows.
		float a = (1.0 - exp(-c.x * K_CLOUD * dt)) * smoothstep(RAY0, RAY1, ti) * smoothstep(rh * 1.2, rh * 3.0, bmin);
		// Lit from the disc inside: hot pink at the heart, dusky violet out at the rim, the pulse running through it.
		vec3 col = mix(DUSK, VIOLET, smoothstep(0.2, 0.7, c.x));
		col = mix(col, PINK, c.y * (0.7 + 0.5 * pulse));
		acc += (1.0 - A) * a * col;
		A += (1.0 - A) * a;
		if(A > 0.97)
			break;
	}
	if(!discDone) {
		acc += (1.0 - A) * disc.rgb * disc.a;
		A += (1.0 - A) * disc.a;
	}

	// Stars: three level sheets through the cloud, each a grid of cells turning with it, a few cells lit, twinkling;
	// only where the cloud is, dimmed by what lies in front of them.
	for(int k = 0; k < 3; k++) {
		float lv = (float(k) - 1.0) * 18.0 * g;
		float tk = abs(d.y) > 1e-3 ? (lv - ec.y) / d.y : -1.0;
		if(tk > t0 && tk < tend) {
			vec3 q = ec + d * tk;
			float r = length(q.xz);
			float a = atan(q.z, q.x + 1e-4) + TAU * fract(t * SPIN * 0.6) * (1.0 + 40.0 / (r + 25.0));
			vec2 sp = vec2(cos(a), sin(a)) * r / STAR_CELL + float(k) * 13.7;
			vec2 cell = floor(sp);
			float h = ss_hash(vec3(cell, float(k) + 3.0));
			vec2 at = cell + 0.2 + 0.6 * vec2(ss_hash(vec3(cell, 11.0)), ss_hash(vec3(cell, 23.0)));
			float sz = (0.06 + 0.1 * h) * STAR_CELL;
			// The star back in the cloud's frame, measured from the ray in 3D so it stays round seen edge-on.
			vec2 sw = (at - float(k) * 13.7) * STAR_CELL;
			float rs = length(sw);
			float as = atan(sw.y, sw.x + 1e-4) - TAU * fract(t * SPIN * 0.6) * (1.0 + 40.0 / (rs + 25.0));
			float dd = length(cross(vec3(cos(as) * rs, lv, sin(as) * rs) - ec, d));
			float tw = 0.45 + 0.55 * sin(TAU * fract(t * (0.6 + h) + h * 7.0));
			vec2 cv = ss_cloud(q, t, g);
			float env = smoothstep(0.0, 0.15, cv.x) + 0.35 * step(r, CLOUD_R * 0.8 * g) * step(HOLE * 3.0 * g, r);
			float star = step(1.0 - STAR_SHARE, h) * exp(-dd * dd / (sz * sz)) * tw * clamp(env, 0.0, 1.0);
			acc += STAR * star * (1.0 - 0.7 * A) * smoothstep(RAY0, RAY1, tk);
		}
	}

	// The halo: the disc's light bent round the hole into a thin ring, brightest at the photon ring, flickering round it.
	if(bc > 0.0 && bc < t1) {
		vec3 hx = normalize(cross(d, vec3(0.0, 1.0, 0.0)) + vec3(1e-4, 0.0, 0.0));
		float ha = atan(dot(cl, cross(d, hx)), dot(cl, hx));
		float rn = ss_noise(vec3(cos(ha + TAU * fract(t * 0.4)) * 4.0, sin(ha + TAU * fract(t * 0.4)) * 4.0, t * 0.7));
		float k = bmin / rh;
		float ring = exp(-(k - 1.08) * (k - 1.08) * 220.0) * 0.55 + exp(-(k - 1.5) * (k - 1.5) * 18.0) * (0.15 + 0.25 * rn);
		float h = ring * (0.75 + 0.5 * pulse);
		acc += (1.0 - A) * DISC_MID * h * 0.7 + DISC_HOT * exp(-(k - 1.08) * (k - 1.08) * 600.0) * 0.25 * (1.0 - A);
		A += (1.0 - A) * clamp(h * 0.8, 0.0, 1.0);
	}

	// The hole itself: nothing comes back.
	if(th < 1e5)
		A = 1.0;
	A *= fade;
	if(A < 0.01)
		discard;
	return vec4(clamp(acc * fade / A, 0.0, 1.0), A);
}
