// Hatred's aura. Bound to HATRED in GlDefs/Shaders.txt.
// The art is a rectangle of snow on deep blue with no alpha: masked to a disc here, and Add drops the dark.

uniform float timer;

#define ASPECT      0.561  // 359 / 640: a square crop, so the flakes stay round on the disc
#define SCROLL      0.035  // texture heights a second the snow drifts down
#define RIM_SOFT    0.80
#define BACKDROP    0.45   // how much of the blue ground survives; the flakes carry the image
#define CONTRAST    1.35
#define FROST       0.55   // how far the flakes are pulled toward white
#define GAIN        1.25
#define SHEEN       1.00   // the glint sweeping across
#define SHEEN_EVERY 6.0    // seconds between sweeps
#define SHEEN_WIDTH 0.20
#define CELLS       6.0    // sparkle grid across the disc
#define SPARK_SHARE 0.60   // share of cells that ever sparkle
#define SPARK       2.40
#define SPARK_RATE  1.60

float ht_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

float ht_hash(vec2 p) {
	return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

// Mirrored repeat: the art does not tile, so it ping-pongs rather than showing a seam as it scrolls.
float ht_mirror(float x) {
	return 1.0 - abs(1.0 - mod(x, 2.0));
}

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;

	float mask = 1.0 - smoothstep(RIM_SOFT, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);

	float drift = timer * SCROLL;
	vec3 tex = getTexel(vec2(0.5 + p.x * ASPECT, ht_mirror(0.5 + p.y - drift))).rgb;
	float lum = ht_lum(tex);

	// Flakes up, backdrop down, and the flakes cooled toward frost white.
	float flake = pow(smoothstep(0.28, 0.95, lum), CONTRAST);
	vec3 col = mix(tex * BACKDROP, mix(tex, vec3(0.86, 0.95, 1.00), FROST), flake);

	// The shine: a soft diagonal band of light crossing every SHEEN_EVERY seconds, strongest on flakes.
	float band = mix(-1.6, 1.6, fract(timer / SHEEN_EVERY));
	// Squared by hand: pow() of a negative base is undefined in GLSL.
	float sd = (p.x + p.y - band) / SHEEN_WIDTH;
	float sheen = exp(-sd * sd);
	col += vec3(0.70, 0.88, 1.00) * SHEEN * sheen * (0.25 + flake);

	// Sparkles: a four point star in some cells, each on its own clock. They ride the drift.
	vec2 q = vec2(p.x, p.y - drift) * CELLS;
	vec2 cell = floor(q);
	float h = ht_hash(cell);
	if(h < SPARK_SHARE) {
		vec2 d = fract(q) - 0.5 - (vec2(ht_hash(cell + 7.1), ht_hash(cell + 3.7)) - 0.5) * 0.5;
		float tw = pow(max(0.0, sin(timer * SPARK_RATE * (0.6 + h) + h * 40.0)), 8.0);
		float star = exp(-dot(d, d) * 70.0) +
			exp(-abs(d.x) * 16.0 - d.y * d.y * 900.0) + exp(-abs(d.y) * 16.0 - d.x * d.x * 900.0);
		col += vec3(0.85, 0.95, 1.00) * SPARK * tw * star;
	}

	vec3 rgb = col * GAIN * mask;
	return vec4(rgb * color.rgb, mask * color.a);
}
