// Freezing Pulse's wave. Bound to FREEZPUL in GlDefs/Shaders.txt.

uniform float timer;

#define BASE_LIGHT  0.78  // floor under the whole body. The single most important number here:
                          // under Add, dim IS transparent, so the fill must never fall far.
#define TEX_VARY    0.34  // how much the skin is allowed to vary that, and no more
#define TOPLIGHT    0.26  // gentle tilt from underside to top -- a tilt, not a spotlight
#define EDGE_GAIN   0.40  // leading edge accent
#define EDGE_POW    2.0
#define HORN_SOFT   0.88  // horns thin out instead of ending in a cut
#define BACK_FADE   0.82
#define OPACITY     0.82  // the fill is opaque-ish and roughly even; the SHAPE is the geometry's job
#define CRAWL       0.04
#define VEIN_A      23.0
#define RATE_A       2.4
#define PULSE_RATE   5.4
#define PULSE        0.18
#define GAIN         1.30

float fp_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

vec4 Process(vec4 color) {
	float arc = gl_TexCoord[0].s;
	float ring = gl_TexCoord[0].t;

	float ang = ring * 6.28318531;
	float lead = cos(ang) * 0.5 + 0.5;   // 1 at the leading edge, 0 at the back
	float vert = sin(ang);               // +1 on top, -1 underneath

	// One drifting field, kept gentle: two high-frequency ones just aliased at this size.
	float veins = sin(arc * VEIN_A + timer * RATE_A) * 0.5 + 0.5;
	vec4 tex = getTexel(vec2(arc, fract(ring - veins * CRAWL)));

	float heat = fp_lum(tex.rgb);

	// Ice. The dark end of the ramp is still a LIT colour, because it is most of the model.
	vec3 col = mix(vec3(0.24, 0.62, 0.92), vec3(0.72, 0.93, 1.00), heat);

	// A tilt from underside to top. No peak anywhere, so no term can draw a line of its own.
	float form = (1.0 - TOPLIGHT) + TOPLIGHT * (vert * 0.5 + 0.5);

	// The leading edge is an accent on a lit body, not a substitute for one.
	col += vec3(0.30, 0.58, 0.85) * EDGE_GAIN * pow(lead, EDGE_POW);

	float horn = 1.0 - smoothstep(HORN_SOFT, 1.0, abs(arc * 2.0 - 1.0));
	float body = mix(1.0, BACK_FADE, 1.0 - lead);

	float pulse = 1.0 + PULSE * (sin(timer * PULSE_RATE) * 0.65 +
		sin(timer * PULSE_RATE * 1.73 + 1.1) * 0.35);

	vec3 rgb = col * (BASE_LIGHT + TEX_VARY * heat) * form * GAIN * pulse * horn * body;

	// Alpha is flat but for the horn taper; the silhouette belongs to the mesh.
	return vec4(rgb * color.rgb, OPACITY * horn * body * color.a);
}
