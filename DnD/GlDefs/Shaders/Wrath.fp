// Wrath's aura. Bound to WRATH in GlDefs/Shaders.txt; the art is only the shaderless fallback.
// Soft arcs wander from a glowing core out to a crawling rim, built from sine waves alone so nothing can tear.

uniform float timer;

#define TAU        6.2831853
#define ARCS       7.0
#define ARC_WIDTH  0.016  // half width of an arc's bright core, in disc radii
#define ARC_GLOW   0.060  // and of the haze round it
#define WANDER     0.60   // radians an arc swings across its length
#define DRIFT      0.35   // turns a second the arcs sweep round
#define FORK_AT    0.42   // radius past which an arc splits
#define FORK_SPLAY 0.95
#define CORE_R     0.15
#define RIM_R      0.86
#define RIM_W      0.035
#define RIM_SOFT   0.93
#define GAIN       1.10
#define CORE_HOT   1.90   // the arcs' cores run past white so they still read white at half alpha
#define HAZE       0.70

float wr_wrap(float a) {
	return a - TAU * floor(a / TAU + 0.5);
}

// How far the arc's path swings at radius r; zero at the core so every arc leaves from it.
float wr_wander(float r, float ph) {
	return WANDER * r * (0.55 * sin(r * 5.0 + timer * 5.1 + ph) + 0.30 * sin(r * 11.0 - timer * 8.7 + ph * 1.7) +
		0.15 * sin(r * 23.0 + timer * 15.9 + ph * 2.3) + 0.08 * sin(r * 41.0 - timer * 29.0 + ph * 3.1));
}

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;
	float mask = 1.0 - smoothstep(RIM_SOFT, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);

	float ang = atan(p.y, p.x);
	float arc_r = max(r, 0.05);
	float span = smoothstep(CORE_R * 0.4, CORE_R + 0.12, r) * (1.0 - smoothstep(RIM_R + 0.02, RIM_SOFT, r));
	float core = 0.0;
	float haze = 0.0;
	float touch = 0.0;

	for(int k = 0; k < 7; ++k) {
		float ph = float(k) * 2.399;
		float base = float(k) * TAU / ARCS + timer * DRIFT * TAU * (1.0 + 0.3 * sin(ph));

		// Each arc swells and fades on its own clock, so the count on screen keeps changing.
		float life = smoothstep(0.4, 0.7, 0.5 + 0.5 * sin(timer * (3.4 + 0.7 * float(k)) + ph * 3.1));
		float w = ARC_WIDTH * (0.55 + 0.6 * life);

		float d = abs(wr_wrap(ang - base - wr_wander(r, ph))) * arc_r;
		core += exp(-d * d / (w * w)) * life;
		haze += exp(-d / ARC_GLOW) * life;

		// The fork: leaves the arc past FORK_AT and bends away from it, fading in rather than starting hard.
		float fr = FORK_AT + 0.18 * fract(ph * 0.37);
		float fork = base + wr_wander(r, ph) + FORK_SPLAY * sin(ph * 5.0) * max(r - fr, 0.0);
		float fin = smoothstep(fr, fr + 0.10, r) * life;
		float fd = abs(wr_wrap(ang - fork)) * arc_r;
		core += exp(-fd * fd / (w * w * 0.5)) * fin * 0.8;
		haze += exp(-fd / ARC_GLOW) * fin * 0.5;

		// Where the arc meets the rim the rim flares.
		float td = abs(wr_wrap(ang - base - wr_wander(RIM_R, ph))) * RIM_R;
		touch += exp(-td * td / 0.004) * life;
	}

	// The rim: a ring whose brightness crawls round it, and flares where arcs land. Whole multiples of ang, so no seam.
	float rd = (r - RIM_R) / RIM_W;
	float crawl = 0.55 + 0.45 * sin(ang * 9.0 + timer * 3.1) * sin(ang * 5.0 - timer * 2.3 + 1.7);
	float rim = exp(-rd * rd) * (crawl * 0.75 + touch * 1.6);

	float hub = exp(-(r * r) / (CORE_R * CORE_R)) * (0.75 + 0.25 * sin(timer * 7.0) * sin(timer * 3.3));

	vec3 col = vec3(0.16, 0.14, 0.42) * (0.25 + 0.30 * r) +
		vec3(0.34, 0.40, 1.00) * haze * HAZE * span +
		vec3(0.86, 0.92, 1.00) * core * CORE_HOT * span +
		vec3(0.56, 0.62, 1.00) * rim +
		vec3(0.75, 0.82, 1.00) * hub;

	vec3 rgb = col * GAIN * mask;
	return vec4(rgb * color.rgb, mask * color.a);
}
