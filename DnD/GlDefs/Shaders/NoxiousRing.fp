// Noxious Blast's closing poison ring, bound to NXRING and drawn Add. The actor's ScaleY is 1 + the shrink's progress
// (flat2 is flat, so it only lifts the disc a unit); the ring thickens and brightens as it closes.

uniform float timer;

#define RIM      0.485  // the disc's radius in uv
#define TILE     0.22   // texture repeats per unit of the disc's scale, so the bubbles keep their size as it closes
#define SWIRL    0.35
#define TWIST    0.7
#define WOBBLE   0.022
#define BAND0    0.26   // the slime band's width, a share of the radius, at the start and at the end
#define BAND1    0.6
#define FILL0    0.05   // and the haze inside it
#define FILL1    0.35
#define GAIN     1.0
#define DETAIL   1.3    // the photo relief in the skin's alpha repeats this often per slime tile
#define DEPTH    4.0    // and lights the slime this strongly off its slopes
#define SPEC     0.9    // with a wet glint on the slopes facing the light
#define EPS      0.0029296875

vec2 rot(vec2 p, float a) {
	float c = cos(a), s = sin(a);
	return vec2(p.x * c - p.y * s, p.x * s + p.y * c);
}

vec4 Process(vec4 color) {
	float prog = clamp(length(gl_TextureMatrix[7][1].xyz) - 1.0, 0.0, 1.0);
	float tile = TILE * length(gl_TextureMatrix[7][0].xyz);
	vec2 p = gl_TexCoord[0].st - 0.5;
	float ang = atan(p.y, p.x);
	float r = length(p) / RIM;
	r += WOBBLE * (sin(ang * 5.0 + timer * 3.0) + 0.6 * sin(ang * 11.0 - timer * 4.3));

	// Two layers of slime: one swirling in, twisted more toward the middle, one turning slowly the other way.
	vec2 a = rot(p, timer * SWIRL + (1.0 - clamp(r, 0.0, 1.0)) * TWIST * (1.0 + 2.0 * prog));
	vec2 b = rot(p, -timer * SWIRL * 0.7 + 1.3);
	vec3 s1 = getTexel(a * tile + vec2(0.5, 0.5 + timer * 0.04)).rgb;
	vec3 s2 = getTexel(b * tile * 1.7 + vec2(0.31, 0.77)).rgb;
	vec3 slime = s1 * 0.65 + s2 * 0.45 * dot(s1, vec3(0.3, 0.59, 0.11));

	float width = mix(BAND0, BAND1, prog);
	float band = clamp((r - (1.0 - width)) / (width * 0.7), 0.0, 1.0);
	band = band * band * clamp((1.0 - r) / 0.04, 0.0, 1.0);
	float fill = mix(FILL0, FILL1, prog * prog) * clamp((1.0 - r) / 0.15, 0.0, 1.0);
	float edge = exp(-pow((r - 0.975) / 0.02, 2.0)) * step(r, 1.02);

	vec3 col = slime * (band * 1.1 + fill) + vec3(0.59, 1.0, 0.31) * edge * (0.35 + 0.65 * prog);
	col *= GAIN * (1.0 + 0.8 * prog);

	// Depth: the slime darkened in the relief's hollows, lit on its slopes, glinting where it is bright and wet.
	vec2 dq = a * tile * DETAIL + vec2(0.13, 0.41 + timer * 0.04 * DETAIL);
	float hc = getTexel(dq).a;
	float hx = getTexel(dq + vec2(EPS, 0.0)).a - hc;
	float hy = getTexel(dq + vec2(0.0, EPS)).a - hc;
	float rel = 1.0 + DEPTH * (hx * 0.55 + hy * 0.83) / (EPS * 512.0);
	float glint = pow(clamp(rel - 1.0, 0.0, 1.0), 2.0) * SPEC;
	col = col * clamp(rel, 0.4, 1.8) * (0.55 + 0.6 * hc) + vec3(0.78, 1.0, 0.55) * glint * clamp(dot(col, vec3(0.3, 0.59, 0.11)) * 2.0, 0.0, 1.0);
	return vec4(col, color.a);
}
