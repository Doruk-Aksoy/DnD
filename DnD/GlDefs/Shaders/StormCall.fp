// Storm Call's marked ground. Bound to STORMCAL; the art is only the shaderless fallback.
// Wrath's arcs, more of them, faster and bluer, with crackling sparks; the actor's alpha is the charge, 0 to 1.

uniform float timer;

#define TAU        6.2831853
#define ARCS       11.0
#define ARC_WIDTH  0.012
#define ARC_GLOW   0.050
#define WANDER     0.62
#define DRIFT      0.45   // turns a second
#define FORK_AT    0.35
#define FORK_SPLAY 1.20
#define CORE_R     0.12
#define RIM_R      0.90
#define RIM_W      0.030
#define RIM_SOFT   0.95
#define CELLS      14.0
#define SPARK      2.2

float sc_wrap(float a) {
	return a - TAU * floor(a / TAU + 0.5);
}

float sc_hash(vec2 p) {
	return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float sc_wander(float r, float ph) {
	return WANDER * r * (0.50 * sin(r * 6.0 + timer * 6.3 + ph) + 0.30 * sin(r * 13.0 - timer * 10.9 + ph * 1.7) +
		0.12 * sin(r * 21.0 + timer * 19.1 + ph * 2.3) + 0.05 * sin(r * 37.0 - timer * 33.0 + ph * 3.1));
}

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;
	float mask = 1.0 - smoothstep(RIM_SOFT, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);

	float charge = clamp(color.a, 0.0, 1.0);
	float ang = atan(p.y, p.x);
	float arc_r = max(r, 0.05);
	float span = smoothstep(CORE_R * 0.4, CORE_R + 0.1, r) * (1.0 - smoothstep(RIM_R + 0.02, RIM_SOFT, r));
	float core = 0.0;
	float haze = 0.0;
	float touch = 0.0;

	for(int k = 0; k < 11; ++k) {
		float ph = float(k) * 2.399;
		float base = float(k) * TAU / ARCS + timer * DRIFT * TAU * (1.0 + 0.4 * sin(ph));
		float life = smoothstep(0.35, 0.65, 0.5 + 0.5 * sin(timer * (4.2 + 0.9 * float(k)) + ph * 3.1));
		float w = ARC_WIDTH * (0.6 + 0.6 * life) * (0.7 + 0.6 * charge);

		float d = abs(sc_wrap(ang - base - sc_wander(r, ph))) * arc_r;
		core += exp(-d * d / (w * w)) * life;
		haze += exp(-d / ARC_GLOW) * life;

		float fr = FORK_AT + 0.25 * fract(ph * 0.37);
		float fork = base + sc_wander(r, ph) + FORK_SPLAY * sin(ph * 5.0) * max(r - fr, 0.0);
		float fin = smoothstep(fr, fr + 0.08, r) * life;
		float fd = abs(sc_wrap(ang - fork)) * arc_r;
		core += exp(-fd * fd / (w * w * 0.5)) * fin * 0.8;
		haze += exp(-fd / ARC_GLOW) * fin * 0.5;

		float td = abs(sc_wrap(ang - base - sc_wander(RIM_R, ph))) * RIM_R;
		touch += exp(-td * td / 0.003) * life;
	}

	// Crackle: hard little sparks popping across the field, more as it charges.
	vec2 q = p * CELLS;
	vec2 cell = floor(q);
	float h = sc_hash(cell);
	vec2 sd = fract(q) - 0.5 - (vec2(sc_hash(cell + 7.1), sc_hash(cell + 3.7)) - 0.5) * 0.5;
	float pop = pow(max(0.0, sin(timer * (9.0 + 8.0 * h) + h * 60.0)), 16.0) * step(1.0 - 0.45 * charge - 0.25, h);
	float spark = pop * (exp(-dot(sd, sd) * 120.0) + exp(-abs(sd.x) * 24.0 - sd.y * sd.y * 1600.0) +
		exp(-abs(sd.y) * 24.0 - sd.x * sd.x * 1600.0));

	float rd = (r - RIM_R) / RIM_W;
	float crawl = 0.55 + 0.45 * sin(ang * 12.0 + timer * 5.1) * sin(ang * 7.0 - timer * 3.7 + 1.7);
	float rim = exp(-rd * rd) * (crawl * 0.9 + touch * 1.8);

	float hub = exp(-(r * r) / (CORE_R * CORE_R)) * (0.8 + 0.2 * sin(timer * 11.0) * sin(timer * 4.7));

	vec3 col = vec3(0.06, 0.10, 0.42) * (0.30 + 0.35 * r) +
		vec3(0.18, 0.36, 1.00) * haze * 0.75 * span +
		vec3(0.70, 0.84, 1.00) * core * 1.9 * span +
		vec3(0.40, 0.58, 1.00) * rim +
		vec3(0.60, 0.75, 1.00) * hub +
		vec3(0.80, 0.92, 1.00) * spark * SPARK * span;

	vec3 rgb = col * mix(0.45, 1.35, charge) * mask;
	return vec4(rgb * color.rgb, mask);
}
