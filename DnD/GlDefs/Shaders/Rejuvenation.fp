// Rejuvenation's sphere. Bound to REJUVSPH in GlDefs/Shaders.txt: two soft helices winding up it, green one way and a
// fainter gold the other, and a faint rim. Drawn Add, so it only brightens; the actor's alpha fades it.

// NO_FOG marks the colormap pass, whose base declares neither. See .claude/notes/bcs-acs-gotchas.md.
#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define RJ_GREEN vec3(0.35, 1.0, 0.45)
#define RJ_LEAF  vec3(0.55, 1.0, 0.25)
#define RJ_GOLD  vec3(1.0, 0.85, 0.35)
#define RJ_TURNS 2.0      // helix starts round the sphere
#define RJ_WIND  1.5      // and how far each winds up it
#define RJ_SPIN_A 0.06    // turns a second of the green one
#define RJ_SPIN_B 0.045   // and of the gold one, the other way
#define RJ_GAIN  0.55

float rj_gauss(float x, float w) {
	return exp(-(x / w) * (x / w));
}

float rj_wrap(float x) {
	return x - floor(x + 0.5);
}

vec4 Process(vec4 color) {
	float u = gl_TexCoord[0].s;
	float v = 1.0 - gl_TexCoord[0].t;

	float a = rj_wrap(u * RJ_TURNS + v * RJ_WIND - RJ_SPIN_A * timer);
	float b = rj_wrap(u * RJ_TURNS - v * RJ_WIND + RJ_SPIN_B * timer);
	float poles = smoothstep(0.0, 0.18, v) * smoothstep(0.0, 0.18, 1.0 - v);
	vec3 col = (RJ_GREEN * 0.5 * rj_gauss(a, 0.12) + RJ_GOLD * 0.3 * rj_gauss(b, 0.1)) * poles * RJ_GAIN;

	// Facing off the sphere's own centre, matrix 7's translation: a sphere's normal is its radius.
	vec3 n = normalize(pixelpos.xyz - gl_TextureMatrix[7][3].xyz);
	vec3 view = normalize(camerapos - pixelpos.xyz);
	float f = abs(dot(n, view));
	col += RJ_LEAF * pow(1.0 - f, 3.5) * 0.18;

	return vec4(col, color.a);
}
