// Storm Brand's globe. Bound to STMBRND; WINTERORB.md3's UVs run twice round (s 0-2) and pole to pole (t).
// Two layers of the vein art drift against each other and flare where they cross; the orb breathes and twinkles.

uniform float timer;

#define SCROLL_A  vec2(0.09, 0.05)
#define SCROLL_B  vec2(-0.07, 0.08)
#define STRETCH_B 1.45   // pole to pole only: round the globe it must stay whole, or the seam shows
#define BASE      vec3(0.03, 0.07, 0.30)
#define CROSS     2.0
#define CELLS     9.0
#define SPARK     1.6

float sb_hash(vec2 p) {
	return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float sb_lum(vec3 c) {
	return dot(c, vec3(0.3, 0.5, 0.2));
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec3 a = getTexel(fract(uv + timer * SCROLL_A)).rgb;
	vec3 b = getTexel(fract(uv * vec2(1.0, STRETCH_B) + timer * SCROLL_B)).rgb;

	vec3 col = BASE + a * 0.55 + b * 0.38 + vec3(0.75, 0.88, 1.0) * sb_lum(a) * sb_lum(b) * CROSS;

	// The mesh pinches at the poles, so the veins are eased off there.
	float pole = smoothstep(0.0, 0.14, uv.t) * (1.0 - smoothstep(0.86, 1.0, uv.t));
	col *= mix(0.55, 1.0, pole);

	// Sparkles: a soft point in some cells, each on its own clock. Whole cells round s, so they ride the seam.
	vec2 q = uv * vec2(CELLS, CELLS * 0.5);
	vec2 cell = floor(q);
	float h = sb_hash(cell);
	vec2 d = fract(q) - 0.5;
	float tw = pow(max(0.0, sin(timer * (3.0 + 4.0 * h) + h * 50.0)), 12.0);
	col += vec3(0.9, 0.96, 1.0) * SPARK * tw * exp(-dot(d, d) * 40.0) * step(0.55, h);

	float breath = 0.85 + 0.15 * sin(timer * 3.1) + 0.08 * sin(timer * 7.3);
	return vec4(col * breath * color.rgb, color.a);
}
