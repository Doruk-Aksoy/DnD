// Implosion's needle on IMPLOSN.md3: a sharp crimson shard spinning on its length, facets catching a light from above,
// edges glowing toward the eye and a hot core brightening to the point. Mostly self-lit, so it reads through smoke.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define TIP       15.0     // the shard's points along its length, model units
#define TAIL      -7.0
#define DEEP      vec3(0.1, 0.0, 0.03)     // facets turned away
#define CRIMSON   vec3(0.62, 0.02, 0.12)  // facets in the light
#define RIM       vec3(1.0, 0.16, 0.28)    // the glow along the edges seen side on
#define HOT       vec3(1.0, 0.55, 0.6)     // the core near the point
#define BLOOD     vec3(0.34, 0.0, 0.02)    // the two ends of a facet's own shade
#define MAGENTA   vec3(0.68, 0.03, 0.26)
#define EMBER     vec3(1.0, 0.24, 0.12)    // the veins of light inside
#define GLINT     vec3(1.0, 0.8, 0.85)     // the highlight as a facet turns through the light
#define LIT       0.35     // how much of it the sector light still shades (the rest is its own glow)
#define PULSE_HZ  2.2      // the core's slow throb
#define VEIN      0.62     // the veins' grain (per model unit) and how fast they drift

// Rule-1 portable hash and value noise.
float im_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float im_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float v = 0.0;
	for(int c = 0; c < 8; c++) {
		vec3 o = vec3(mod(float(c), 2.0), mod(floor(float(c) * 0.5), 2.0), floor(float(c) * 0.25));
		vec3 w = mix(1.0 - f, f, o);
		v += w.x * w.y * w.z * im_hash(i + o);
	}
	return v;
}

vec4 Process(vec4 color) {
	// This point in model space, back through the model-to-world matrix.
	vec3 c0 = gl_TextureMatrix[7][0].xyz, c1 = gl_TextureMatrix[7][1].xyz, c2 = gl_TextureMatrix[7][2].xyz;
	vec3 x12 = cross(c1, c2), x20 = cross(c2, c0), x01 = cross(c0, c1);
	vec3 w = pixelpos.xyz - gl_TextureMatrix[7][3].xyz;
	vec3 p = vec3(dot(x12, w), dot(x20, w), dot(x01, w)) / dot(c0, x12);

	// The facet's normal from the derivatives, to world, turned to face the eye.
	vec3 n = normalize(cross(dFdx(p), dFdy(p)));
	vec3 nw = normalize(n.x * x12 + n.y * x20 + n.z * x01);
	vec3 v = normalize(camerapos - pixelpos.xyz);
	// The derivatives' sign follows the screen's y, which Vulkan flips: turn both to the eye before anything reads them.
	float fs = sign(dot(nw, v));
	nw *= fs;
	n *= fs;

	vec3 L = normalize(vec3(-0.3, 1.0, 0.2));
	float key = max(dot(nw, L), 0.0);
	// Each facet its own shade, from its model-space normal (it turns with the shard, so it holds while spinning).
	float roll = im_hash(floor(n * 3.0 + 4.0) + 17.0);
	float glint = pow(max(dot(reflect(-v, nw), L), 0.0), 30.0);
	float facing = abs(dot(nw, v));
	float rim = pow(1.0 - facing, 2.0);
	float along = clamp((p.x - TAIL) / (TIP - TAIL), 0.0, 1.0);
	float t = mod(timer, 512.0);
	float throb = 0.85 + 0.15 * sin(6.2831853 * fract(t * PULSE_HZ));

	vec3 shade = mix(mix(BLOOD, MAGENTA, roll), CRIMSON, 0.5);
	vec3 body = mix(DEEP, shade, 0.2 + 0.8 * key * key) * (0.75 + 0.5 * roll) * mix(0.55, 1.15, along);
	// Thin veins of light inside: the ridges of a drifting noise in model space.
	float rg = 1.0 - abs(2.0 * im_noise(p * VEIN + vec3(t * VEIN, 0.0, t * 0.2)) - 1.0);
	float veins = smoothstep(0.88, 0.98, rg);
	vec3 glow = RIM * rim * 1.1 + HOT * pow(along, 3.0) * 0.6 * throb + CRIMSON * 0.12 * throb + EMBER * veins * 0.7 * throb
		+ GLINT * glint * 0.9;
	vec3 col = body * mix(vec3(1.0), color.rgb, LIT) + glow;
	return vec4(clamp(col, 0.0, 1.0), 1.0);
}
