// Poison Nova's bolt: a lumpy ball of toxic sludge on fire like a comet's head, green flames climbing off it and
// swept back by its flight. Alpha-blended: the actor's Alpha 0.1 only lowers the alpha test.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

// Units before the model's scale, model space x forward, y up, z sideways, heights x1.2 so a sphere looks round.
#define HULL_C    vec3(-17.0, 0.0, 0.0)    // the ellipsoid everything is drawn inside, centre and half axes;
#define HULL_R    vec3(26.0, 20.0, 20.0)   // PNOVA.md3's facets sit just outside it
#define BALL      6.0      // the ball's radius before its dents, how deep they go, and how much longer it runs out behind
#define DENT      1.3
#define STRETCH   1.35
#define ROLL      0.2      // turns a second the lumps roll forward
#define VSTEP     0.004    // the shape is picked at spawn as actor ScaleX 1 + k x VSTEP, k from 0 to 7
#define TRAIL     vec3(-1.0, 0.0, 0.0)     // the way the flames stream: straight back against the flight
#define FILM      1.8      // how far the flame film reaches past the ball's skin
#define FLAME_W   7.0      // the tail's half width (three quarters of it as it leaves the ball), and its length
#define FLAME_L   28.0
#define SPREAD    2.0      // how many more half widths it fans out to by its end, and how far along it thins away
#define FADE      0.5 
#define FRONT     0.35     // flame left on the ball's leading face, where its flight blows it back
#define RISE      26.0     // units a second the flames stream down the trail
#define TONGUE    vec3(0.38, 0.13, 0.38)   // the flame noise's scale: narrow across, long down the trail, so it draws tongues
#define K_FLAME   0.55     // opacity per unit of flame density
#define BUBBLE    2.6      // the bubbles' cell size on the ball, and how often each swells and pops
#define POP       0.55
#define NEAR0     26.0     // map units from the eye where the bolt starts to show, so a fresh cast stays clear
#define NEAR1     56.0
#define STEPS     24       // flame samples along a ray
#define SEEK      18       // steps looking for the ball's skin, then halvings to pin it down
#define PIN       5
#define SMOKE     vec3(0.06, 0.08, 0.035)

// A hash of a lattice point; it is only ever fed whole numbers, so it cannot tear.
float pn_hash(vec3 p) {
	p = fract(p * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Smooth value noise, 0 to 1.
float pn_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float a = mix(pn_hash(i), pn_hash(i + vec3(1.0, 0.0, 0.0)), f.x);
	float b = mix(pn_hash(i + vec3(0.0, 1.0, 0.0)), pn_hash(i + vec3(1.0, 1.0, 0.0)), f.x);
	float c = mix(pn_hash(i + vec3(0.0, 0.0, 1.0)), pn_hash(i + vec3(1.0, 0.0, 1.0)), f.x);
	float d = mix(pn_hash(i + vec3(0.0, 1.0, 1.0)), pn_hash(i + vec3(1.0, 1.0, 1.0)), f.x);
	return mix(mix(a, b, f.y), mix(c, d, f.y), f.z);
}

float pn_fbm(vec3 x) {
	return 0.55 * pn_noise(x) + 0.3 * pn_noise(x * 2.03 + vec3(1.7, 9.2, 3.1)) + 0.15 * pn_noise(x * 4.07 + vec3(8.3, 2.8, 5.5));
}

// Toxic fire by heat: deep green, acid green, sulfur yellow-green, and pale only at the very hottest.
vec3 pn_fire(float h) {
	vec3 c = mix(vec3(0.04, 0.18, 0.02), vec3(0.12, 0.55, 0.04), smoothstep(0.1, 0.4, h));
	c = mix(c, vec3(0.38, 0.95, 0.09), smoothstep(0.35, 0.65, h));
	c = mix(c, vec3(0.78, 1.0, 0.3), smoothstep(0.62, 0.88, h));
	return mix(c, vec3(0.95, 1.0, 0.75), smoothstep(0.9, 1.0, h));
}

// q in the ball's own turning frame: roll is the cosine and sine of how far it has rolled forward.
vec3 pn_rolled(vec3 q, vec2 roll) {
	return vec3(q.x * roll.x - q.y * roll.y, q.x * roll.y + q.y * roll.x, q.z);
}

// Distance from q to the ball's skin, q from its middle: a ball stretched out behind into a comet's head, lumps and
// dents rolling over it. Stretched and dented, it is no true distance, so the skin is found by stepping, not tracing.
float pn_sdf(vec3 q, vec3 seed, vec2 roll) {
	vec3 b = vec3(q.x < 0.0 ? q.x / STRETCH : q.x, q.y, q.z);
	float r = length(b);
	vec3 rd = pn_rolled(b / max(r, 0.001), roll);
	float dent = 2.0 * pn_noise(rd * 1.7 + seed) - 1.0 + 0.35 * (2.0 * pn_noise(rd * 4.1 + seed.zxy) - 1.0);
	return r - BALL - DENT * dent;
}

// Flame density and heat: a film over the ball, thin and cool where the ball should show, and a tail fanning out down
// the trail, both torn into tongues hottest inside and darker at the edge; the tail cools and thins away, never to a point.
vec2 pn_flame(vec3 q, vec3 seed, vec2 roll) {
	vec3 tr = normalize(TRAIL);
	float h = dot(q, tr);
	float up = h / max(length(q), 0.001);
	float u = clamp(h / FLAME_L, 0.0, 1.0);
	float g = 0.75 + SPREAD * u;
	// The dents matter only near the skin; farther out the plain stretched ball will do, blended so no seam shows.
	float sd = length(vec3(q.x < 0.0 ? q.x / STRETCH : q.x, q.y, q.z)) - BALL;
	if(sd < 5.0)
		sd = mix(pn_sdf(q, seed, roll), sd, smoothstep(3.0, 5.0, sd));
	float film = sd / (FILM * (FRONT + (1.0 - FRONT) * smoothstep(BALL, -0.5 * BALL, q.x)));
	float tail = length(q - tr * h) / (FLAME_W * g) + 2.0 * smoothstep(-2.0, -5.0, h) + 0.6 * u * u;
	// Too far from both for any tongue to reach: skip the noise, most of the hull is empty.
	if(film > 1.35 && tail > 1.75)
		return vec2(0.0, 0.0);
	// The noise across the trail widens with the tail, so the tongues spread apart as they go.
	float n = pn_fbm(vec3(dot(q, vec3(tr.y, -tr.x, 0.0)) / g, h - timer * RISE, q.z / g) * TONGUE + seed);
	float v = max(n * 1.35 - film, 0.65 + 2.2 * (n - 0.5) - tail);
	float dens = smoothstep(0.0, 0.2, v) * step(0.0, sd + 0.5) * mix(0.45, 1.0, smoothstep(-0.2, 0.7, up))
		* (1.0 - smoothstep(FADE, 1.0, u));
	float heat = clamp(0.25 + 1.6 * v + 0.3 * (1.0 - u) - 0.2 * u, 0.0, 1.0) * mix(0.7, 1.0, smoothstep(0.0, 0.8, up));
	return vec2(dens, heat);
}

// The bubbles on the ball's skin: one to a lattice cell, each swelling from nothing and popping on its own clock.
// x is how far into a bubble the point is (1 at its middle), y the ring of a fresh pop.
vec2 pn_bubbles(vec3 s) {
	vec3 g = s / BUBBLE;
	vec3 i = floor(g);
	vec3 f = g - i;
	float best = 0.0;
	float ring = 0.0;
	for(int z = -1; z <= 1; z++) {
		for(int y = -1; y <= 1; y++) {
			for(int x = -1; x <= 1; x++) {
				vec3 o = vec3(float(x), float(y), float(z));
				vec3 c = i + o;
				vec3 j = vec3(pn_hash(c), pn_hash(c + 19.0), pn_hash(c + 37.0));
				float life = fract(timer * POP * (0.6 + 0.8 * pn_hash(c + 53.0)) + pn_hash(c + 71.0));
				float rad = 0.5 * smoothstep(0.0, 0.85, life);
				float d = length(o + 0.2 + 0.6 * j - f);
				if(life < 0.9)
					best = max(best, 1.0 - d / max(rad, 0.001));
				else
					ring = max(ring, (1.0 - abs(d - rad - (life - 0.9) * 4.0) / 0.08) * (1.0 - life) * 10.0);
			}
		}
	}
	return vec2(clamp(best, 0.0, 1.0), clamp(ring, 0.0, 1.0));
}

// The ball's skin, sb in its turning frame, n its outward normal, seen along d: dark sludge churning in streaks,
// glowing goo where it runs thin, lit from the fire above so its lumps show; bright bubbles, an oily sheen, a burning rim.
vec3 pn_ball(vec3 sb, vec3 n, vec3 d) {
	float face = clamp(-dot(n, d), 0.0, 1.0);
	vec2 bb = pn_bubbles(sb);
	float w = pn_fbm(sb * 0.4 + vec3(0.0, -timer * 0.3, 0.0));
	float m = pn_fbm(sb * 0.8 + 2.0 * w + vec3(timer * 0.2, -timer * 0.5, timer * 0.1));
	vec3 hv = normalize(normalize(vec3(-0.3, 1.0, 0.0)) - d);
	float sheen = pow(max(dot(n, hv), 0.0), 24.0);
	float top = 0.55 + 0.45 * max(n.y, 0.0);
	vec3 col = mix(vec3(0.015, 0.035, 0.01), vec3(0.07, 0.16, 0.03), smoothstep(0.35, 0.6, m)) * top;
	col += vec3(0.2, 0.6, 0.05) * smoothstep(0.6, 0.75, m);
	col = mix(col, mix(vec3(0.25, 0.75, 0.08), vec3(0.85, 1.0, 0.45), bb.x * bb.x), smoothstep(0.0, 0.25, bb.x));
	col += vec3(0.6, 1.0, 0.3) * bb.y + vec3(0.5, 0.8, 0.25) * sheen * 0.6;
	return col + vec3(0.3, 0.9, 0.1) * pow(1.0 - face, 2.5) * 1.1;
}

vec4 Process(vec4 color) {
	// The eye and this point in model space, back through the model-to-world matrix; heights x1.2 against the view's stretch.
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

	// Nothing while the eye is close, so a fresh cast does not fill the caster's view.
	float fade = smoothstep(NEAR0, NEAR1, length(we));
	if(fade <= 0.0)
		discard;

	// This bolt's shape, read back from its scale, and how far its lumps have rolled.
	float k = floor((length(mm[0].xyz) - 1.0) / VSTEP + 0.5);
	vec3 seed = vec3(k * 7.31, k * 3.17, k * 5.53);
	float ra = 6.2831853 * (ROLL * timer + k * 0.37);
	vec2 roll = vec2(cos(ra), sin(ra));

	// The stretch of the ray inside the hull's ellipsoid, found as a unit sphere in its squashed space. Each pixel takes
	// one shell: the outer one (t 0.5) from outside, the inward-facing one (t 1.5) once the eye is in the long tail.
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

	// The ball's skin: stepped through the sphere it fits in until the ray crosses it, then pinned down by halving.
	float rb = BALL * STRETCH + DENT * 1.35 + 0.2;
	float b = dot(e, d);
	float disc = b * b - dot(e, e) + rb * rb;
	float ts = 1e6;
	if(disc > 0.0) {
		float sa = max(-b - sqrt(disc), t0);
		float sz = min(-b + sqrt(disc), t1);
		float st = (sz - sa) / float(SEEK);
		float prev = sa;
		for(int i = 1; i <= SEEK; i++) {
			float t = sa + float(i) * st;
			if(ts > 1e5 && pn_sdf(e + d * t, seed, roll) < 0.0) {
				float lo = prev;
				float hi = t;
				for(int j = 0; j < PIN; j++) {
					float mid = 0.5 * (lo + hi);
					if(pn_sdf(e + d * mid, seed, roll) < 0.0)
						hi = mid;
					else
						lo = mid;
				}
				ts = hi;
			}
			prev = t;
		}
	}
	bool hit = ts < 1e5;
	float tend = hit ? ts : t1;

	// The flames, front to back as far as the ball or until nothing more shows through. Each pixel starts its steps a
	// different fraction in, which trades the slices' bands for a fine grain.
	vec3 acc = vec3(0.0, 0.0, 0.0);
	float A = 0.0;
	float jit = pn_hash(vec3(floor(gl_FragCoord.xy), 3.0));
	float dt = (tend - t0) / float(STEPS);
	for(int i = 0; i < STEPS; i++) {
		vec2 fh = pn_flame(e + d * (t0 + (float(i) + jit) * dt), seed, roll);
		float lit = smoothstep(0.08, 0.3, fh.y);
		float a = (1.0 - exp(-fh.x * K_FLAME * dt * (0.3 + 0.7 * lit))) * fade;
		acc += (1.0 - A) * a * mix(SMOKE, pn_fire(fh.y), lit);
		A += (1.0 - A) * a;
		if(A > 0.97)
			break;
	}

	if(hit) {
		// The normal from the distance field's slope, four samples round the hit.
		vec3 s = e + d * ts;
		vec2 h = vec2(0.12, -0.12);
		vec3 n = normalize(h.xyy * pn_sdf(s + h.xyy, seed, roll) + h.yyx * pn_sdf(s + h.yyx, seed, roll)
			+ h.yxy * pn_sdf(s + h.yxy, seed, roll) + h.xxx * pn_sdf(s + h.xxx, seed, roll));
		acc += (1.0 - A) * fade * pn_ball(pn_rolled(s, roll) + seed, n, d);
		A += (1.0 - A) * fade;
	}

	if(A < 0.01)
		discard;
	return vec4(clamp(acc / A, 0.0, 1.0) * color.rgb, A);
}
