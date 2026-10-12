// Ice Bolt and Frost Shards. Bound to ICEBOLT in GlDefs/Shaders.txt; built by .claude/tools/icebolt.
// s = facet id + across, the id packing the facet normal; t = spin rate + tail (0) to point (1).

uniform float timer;

#define BASE        0.13
#define KEY         0.55   // light from above, rolling over the facets as the model spins
#define FILL        0.08   // and a little from below
#define REAR        0.14   // facets facing back toward the caster
#define FLASH       0.80   // the glint as a facet passes face up
#define FLASH_TIGHT 20.0
#define EDGE        0.30   // the long edges of each facet
#define EDGE_POW    6.0
#define TIP_GLOW    0.30
#define TIP_POW     5.0
#define RUN         0.18   // a faint shimmer running down to the point
#define GAIN        0.90

float ib_hash(float n) {
	return fract(sin(n * 12.9898) * 43758.5453);
}

vec4 Process(vec4 color) {
	float id = floor(gl_TexCoord[0].s);
	float across = (gl_TexCoord[0].s - id - 0.02) / 0.96;
	float rate = floor(gl_TexCoord[0].t);
	float along = gl_TexCoord[0].t - rate;

	// The facet normal: azimuth from up in 64ths of a turn, and its forward part in eight steps.
	float az = mod(id, 64.0) / 64.0;
	float nx = floor(id / 64.0) / 3.5 - 1.0;

	// ROTATING turns a model rate * 0.175 times a second, on the same clock as timer.
	float up = sqrt(max(0.0, 1.0 - nx * nx)) * cos(6.28318531 * (az + fract(timer * rate * 0.175)));

	float ice = getTexel(vec2(across * 0.45 + ib_hash(id), along * 1.3)).r;

	float lit = BASE + KEY * max(up, 0.0) + FILL * max(-up, 0.0) + REAR * max(-nx, 0.0);
	lit *= 0.8 + 0.4 * ib_hash(id + 7.0);
	float edge = EDGE * pow(abs(across * 2.0 - 1.0), EDGE_POW);

	float tone = clamp(lit * 0.9 + 0.35 * (ice - 0.5) + edge * 0.5, 0.0, 1.0);
	vec3 col = mix(vec3(0.03, 0.12, 0.45), vec3(0.45, 0.75, 1.00), tone) * (lit + edge) * (0.7 + 0.6 * ice);
	col += vec3(0.75, 0.90, 1.00) * FLASH * pow(max(up, 0.0), FLASH_TIGHT);
	col += vec3(0.45, 0.70, 1.00) * TIP_GLOW * pow(along, TIP_POW);
	col += vec3(0.40, 0.65, 1.00) * RUN * pow(max(0.0, sin((along * 2.0 - timer * 0.8) * 6.28318531)), 16.0);

	// The skin is plain greyscale, so opacity is the actor's to decide.
	return vec4(col * GAIN * color.rgb, color.a);
}
