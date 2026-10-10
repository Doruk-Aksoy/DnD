// Winter Orb. Bound to WNTRORB, a copy of Ice Nova's scratched ice under its own name.
// s runs 0..2 round the sphere and t 0..1 pole to pole; the art does not tile, so it is mirrored here.

uniform float timer;

#define CROP_LO     0.14   // the art darkens toward its edges; only the clean middle is used
#define CROP_HI     0.86
#define DRIFT       0.02   // the frost creeps slowly round, under the model's own spin
#define POLE_SOFT   0.78   // where the pinched poles start fading to plain ice
#define BASE_LIGHT  0.55
#define CONTRAST    0.75   // how hard the scratches stand out
#define SHEEN       0.55   // glints sweeping round the sphere
#define SHEEN_TIGHT 18.0
#define SHEEN_RATE  1.4
#define SCRATCH     0.45   // extra light on the brightest scratches, the frost catching it
#define SPARK       1.30
#define SPARK_RATE  2.2
#define SPARK_SHARE 0.35

float wo_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

float wo_hash(vec2 p) {
	return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

// Mirrored repeat into the crop: every edge meets its own reflection, so there is no seam.
float wo_tile(float x) {
	float m = 1.0 - abs(1.0 - mod(x, 2.0));
	return mix(CROP_LO, CROP_HI, m);
}

vec4 Process(vec4 color) {
	float s = gl_TexCoord[0].s;
	float t = gl_TexCoord[0].t;

	vec3 tex = getTexel(vec2(wo_tile(s + timer * DRIFT), wo_tile(t * 2.0))).rgb;
	float ice = wo_lum(tex);

	// The poles pinch every column into a point; fade them to plain ice so the pinch does not read.
	float pole = smoothstep(POLE_SOFT, 1.0, abs(t * 2.0 - 1.0));
	ice = mix(ice, 0.62, pole);

	vec3 col = mix(vec3(0.07, 0.22, 0.52), vec3(0.60, 0.85, 1.00), ice);
	col *= BASE_LIGHT + CONTRAST * ice;
	col += vec3(0.75, 0.92, 1.00) * SCRATCH * smoothstep(0.72, 0.95, ice);

	// Glints sweeping round, brightest where the frost is.
	float glint = pow(max(0.0, sin(s * 6.28318531 - timer * SHEEN_RATE)), SHEEN_TIGHT);
	col += vec3(0.70, 0.90, 1.00) * SHEEN * glint * (0.35 + ice) * (1.0 - pole);

	// Twinkles, a few cells at a time, kept off the poles where the cells squeeze together.
	vec2 q = vec2(s * 8.0, t * 6.0);
	vec2 cell = floor(q);
	float h = wo_hash(cell);
	if(h < SPARK_SHARE) {
		vec2 d = fract(q) - 0.5;
		float tw = pow(max(0.0, sin(timer * SPARK_RATE * (0.6 + h) + h * 40.0)), 10.0);
		col += vec3(0.85, 0.95, 1.00) * SPARK * tw * exp(-dot(d, d) * 40.0) * (1.0 - pole);
	}

	return vec4(col * color.rgb, color.a);
}
