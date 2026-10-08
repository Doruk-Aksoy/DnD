// Creeping Frost's patch. Bound to CRPFROST in GlDefs/Shaders.txt.
// Texture is sampled in WORLD space off pixelpos, so it stays put as the patch creeps.

uniform float timer;

// NO_FOG marks the colormap pass, whose base lump is the one that does not declare this.
// main.vp writes it either way. See .claude/notes/bcs-acs-gotchas.md.
#ifdef NO_FOG
varying vec4 pixelpos;
#endif

#define WORLD_SCALE  0.011  // texture units per world unit; lower is a coarser, bigger-grained ice
#define CRAG         0.40   // how deeply the edge bites in
#define CRAG_A        7.0   // coarse lobes
#define CRAG_B       13.0   // finer notches
#define CRAG_C       23.0   // the crackle on top
#define RATE_A        0.37
#define RATE_B       -0.23  // counter-drifting, so the shape never settles
#define RATE_C        0.61
#define EDGE_SOFT     0.09  // how hard the boundary is; crisper shows the squiggle
#define BREATHE       0.06  // the slow in-and-out of the whole patch
#define BREATHE_RATE  0.8
#define RIM           0.55  // frost is brightest where it is still advancing
#define RIM_WIDTH     0.22
#define BASE_LIGHT    0.58
#define TEX_VARY      0.55
#define GAIN          1.15

float cf_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;
	float th = atan(p.y, p.x);

	// Three bands at unrelated rates, so the outline never settles into a beat.
	float crag = sin(th * CRAG_A + timer * RATE_A) * 0.38 +
		sin(th * CRAG_B + timer * RATE_B) * 0.36 +
		sin(th * CRAG_C + timer * RATE_C) * 0.26;

	float edge = (1.0 - CRAG * (crag * 0.5 + 0.5)) * (1.0 + BREATHE * sin(timer * BREATHE_RATE));

	float mask = 1.0 - smoothstep(edge - EDGE_SOFT, edge, r);
	if(mask <= 0.001)
		return vec4(0.0, 0.0, 0.0, 0.0);

	// The whole point: sampled in WORLD space, so the ice stays put while the patch moves over it.
	vec4 tex = getTexel(pixelpos.xz * WORLD_SCALE);
	float ice = cf_lum(tex.rgb);

	vec3 col = mix(vec3(0.14, 0.34, 0.62), vec3(0.52, 0.78, 0.96), ice);

	// A brighter band just inside the boundary -- the frost is newest where it is still spreading.
	col += vec3(0.40, 0.68, 0.92) * RIM *
		smoothstep(edge - RIM_WIDTH, edge - EDGE_SOFT, r) * mask;

	vec3 rgb = col * (BASE_LIGHT + TEX_VARY * ice) * GAIN;

	return vec4(rgb * color.rgb, mask * color.a);
}
