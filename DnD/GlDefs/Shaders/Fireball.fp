// Fireball's bolt: an opaque, spinning ball of FIREBALL.png's lava, a straight streak behind it cooling to smoke.
// Alpha-blended, not added: the actor's Alpha 0.1 only lowers the alpha test. Engine notes: .claude/notes/zan33-gl-shaders.md.

// NO_FOG marks the colormap pass, whose base declares neither.
#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

// Lengths in md3 units, before the model's scale. The hull is PYROBLAS.md3, so HULL_R and HULL_TIP stay Pyroblast's.
#define HULL_R    15.5
#define HULL_TIP  95.0
#define BALL      9.5     // the lava ball's radius
#define CORONA    4.5     // how far its flickering corona reaches past the surface
#define SPIN      1.4     // turns a second, rolling forward
#define TEXSCALE  0.45    // lava across the ball: centred in the texture and inside 0..1, so no mirror line crosses it
#define TAIL      72.0    // the streak's length behind the ball's middle, and its radius leaving the ball
#define TAIL_R    6.5
#define STREAK    2.0     // lava repeats round the streak, which is what draws its lines, and along it
#define ALONG     0.012
#define FLOW      2.2     // how fast the lines rush back
#define WANDER    1.0     // how far the streak's line strays by its end, so it is straight overall but never ruled
#define K_TAIL    0.6     // the streak's opacity per unit of density
#define NEAR0     20.0    // map units from the eye where the streak starts to show, so a fresh cast stays clear
#define NEAR1     56.0
#define STEPS     20
#define LUMA      vec3(0.3, 0.59, 0.11)
#define SMOKE     vec3(0.16, 0.13, 0.12)

// Mirrored repeat, 0 to 1 and back every 2, so each edge of the texture meets its own reflection.
vec2 fb_mirror(vec2 x) {
	return abs(mod(x + 1.0, 2.0) - 1.0);
}

// The lava round direction n, from three projections weighted by how squarely each faces it: no poles, no seam.
vec3 fb_lava(vec3 n, vec3 p) {
	vec3 w = n * n;
	w = w * w;
	w /= w.x + w.y + w.z;
	return getTexel(fb_mirror(p.yz)).rgb * w.x + getTexel(fb_mirror(p.zx)).rgb * w.y + getTexel(fb_mirror(p.xy)).rgb * w.z;
}

// Fire by heat: dark red, red, orange, yellow, and near white only at the very hottest.
vec3 fb_fire(float h) {
	vec3 c = mix(vec3(0.35, 0.03, 0.0), vec3(0.95, 0.22, 0.03), smoothstep(0.0, 0.35, h));
	c = mix(c, vec3(1.0, 0.55, 0.1), smoothstep(0.3, 0.65, h));
	c = mix(c, vec3(1.0, 0.85, 0.4), smoothstep(0.6, 0.9, h));
	return mix(c, vec3(1.0, 0.95, 0.75), smoothstep(0.88, 1.0, h));
}

// v turned by angle a about the unit axis k.
vec3 fb_rotate(vec3 v, vec3 k, float a) {
	float c = cos(a);
	float s = sin(a);
	return v * c + cross(k, v) * s + k * dot(k, v) * (1.0 - c);
}

// Inside the hull's true shape. The mesh's facets sit just outside it, so the two never disagree.
bool fb_inside(vec3 p) {
	if(dot(p, p) <= HULL_R * HULL_R)
		return true;
	float xt = -HULL_R * HULL_R / HULL_TIP;
	float rt = HULL_R * sqrt(1.0 - HULL_R * HULL_R / (HULL_TIP * HULL_TIP));
	return p.x < xt && p.x > -HULL_TIP && length(p.yz) <= rt * (p.x + HULL_TIP) / (xt + HULL_TIP);
}

// Where a ray from e along d leaves the cylinder round the hull.
float fb_exit(vec3 e, vec3 d) {
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
	bool within = fb_inside(e);
	if((gl_TexCoord[0].t < 1.0) == within)
		discard;
	float t0 = within ? 0.0 : len;
	float t1 = within ? len : max(fb_exit(e, d), len);

	// The ball, if the ray meets it inside the hull; it hides everything behind it.
	float b = dot(e, d);
	float disc = b * b - dot(e, e) + BALL * BALL;
	float ts = disc > 0.0 ? -b - sqrt(disc) : 1e6;
	bool hit = ts > t0 && ts < t1;
	float tend = hit ? ts : t1;

	// The streak, composited front to back as far as the ball.
	vec3 acc = vec3(0.0, 0.0, 0.0);
	float A = 0.0;
	float dt = (tend - t0) / float(STEPS);
	for(int i = 0; i < STEPS; i++) {
		float t = t0 + (float(i) + 0.5) * dt;
		vec3 q = e + d * t;
		float u = -q.x / TAIL;
		vec2 yz = q.yz - WANDER * u * vec2(sin(q.x * 0.09 + timer * 1.7), cos(q.x * 0.07 - timer * 1.3));
		float ang = atan(yz.y, yz.x + 1e-6) * 0.31830989;
		// Two reads of the lava: fine lines rushing back that rag the edge, and coarse clumps that break the end apart.
		float n = dot(getTexel(fb_mirror(vec2(ang * STREAK, q.x * ALONG + timer * FLOW))).rgb, LUMA);
		float n2 = dot(getTexel(fb_mirror(vec2(ang * 0.7 + 0.37, q.x * ALONG * 0.45 + timer * FLOW * 0.6 + 0.21))).rgb, LUMA);
		float r = TAIL_R * sqrt(clamp(1.0 - u, 0.0, 1.0)) * (1.0 + 0.3 * u) * (0.65 + 0.45 * n) * (0.85 + 0.3 * n2) + 0.2;
		float rad = dot(yz, yz) / (r * r);
		float along = smoothstep(-0.02, 0.06, u) * smoothstep(u * 0.75 - 0.15, u * 0.75 + 0.1, n2);
		float a = 1.0 - exp(-max(1.0 - rad, 0.0) * along * (0.25 + 1.4 * n) * smoothstep(NEAR0, NEAR1, t * k) * dt * K_TAIL);
		// Heat picks the colour, not distance: hot lines run on down the tail, cool clumps smoke early; never white, that is the ball's.
		float h = clamp(0.8 - 1.15 * u + 1.1 * (n - 0.4) + 0.3 * (n2 - 0.4) + 0.25 * max(1.0 - rad, 0.0) * (1.0 - u), 0.0, 0.86);
		vec3 col = mix(fb_fire(h), SMOKE * (0.6 + 0.6 * n2), smoothstep(0.32, 0.08, h));
		acc += (1.0 - A) * a * col;
		A += (1.0 - A) * a;
	}

	// The corona: a ring of flickering lava light round the ball's edge, for rays that just miss it.
	if(!hit && -b > t0) {
		vec3 qn = normalize(e - d * b);
		float g = clamp(1.0 - (length(e - d * b) - BALL) / CORONA, 0.0, 1.0);
		float fl = dot(fb_lava(qn, qn * TEXSCALE + vec3(0.5, 0.5 + timer * 0.6, 0.5)), vec3(0.3, 0.59, 0.11));
		float a = g * g * (0.35 + 0.9 * fl);
		acc += (1.0 - A) * a * mix(vec3(0.95, 0.3, 0.05), vec3(1.0, 0.85, 0.4), g * fl);
		A += (1.0 - A) * a;
	}

	// The ball itself: the lava turned with time, its cracks glowing, its rim burning.
	if(hit) {
		vec3 n = (e + d * ts) / BALL;
		vec3 mn = fb_rotate(n, vec3(0.15, 0.25, 0.956), 6.2831853 * SPIN * timer);
		vec3 lava = fb_lava(mn, mn * TEXSCALE + vec3(0.5, 0.5, 0.5));
		float hot = smoothstep(0.45, 0.85, dot(lava, vec3(0.3, 0.59, 0.11)));
		float face = clamp(-dot(n, d), 0.0, 1.0);
		vec3 col = lava * (0.75 + 0.45 * face) + vec3(1.0, 0.8, 0.4) * hot * 0.5 + vec3(1.0, 0.45, 0.08) * pow(1.0 - face, 3.0) * 1.2;
		acc += (1.0 - A) * col;
		A = 1.0;
	}

	if(A < 0.01)
		discard;
	return vec4(clamp(acc / A, 0.0, 1.0) * color.rgb, A);
}
