// Righteous Fire's aura disc. Bound to RGHTFIRE in GlDefs/Shaders.txt.
//
// Two signals, two jobs: the art is a cutout, so its ALPHA is near-binary and says only WHERE fire
// is -- the structure lives in RGB luminance. Using alpha for brightness gives a flat fill.
// The art is also a RING, so the radius is remapped to spread its band across the disc.

uniform float timer;

#define RING_IN   0.50   // where the flame band starts in the ART, as a fraction of its radius
#define HOLE      0.16   // screen radius the fire starts at; lower = more filled in
#define SPIN_A    0.18   // inner layer, radians/sec
#define SPIN_B   -0.26   // outer layer counter-rotates
#define SHEAR_A   0.45   // extra turn at the rim, so it twists rather than slides
#define SHEAR_B  -0.30
#define SCALE_B   0.94   // layer B sits slightly further out, so the two never line up
#define RIM_SOFT  0.90   // only the outermost tenth is feathered; the art feathers its own tips
#define FLOOR_B   0.35   // how much of layer A survives where layer B is empty
#define CONTRAST  1.30   // gamma on LUMINANCE. The texture knob -- it must not touch the cutout
#define COOL      0.45   // how much redder the rim runs than the middle
#define TONGUE_1  0.055  // rim wobble, 5 lobes
#define TONGUE_2  0.030  // rim wobble, 8 lobes
#define FLICKER   0.16
#define GAIN      2.00   // ~parity with the raw texture; only the hottest cores clip, which
                        // is wanted. Past ~2.2 the whole band blows out to flat white again.

vec2 rf_spin(vec2 p, float a) {
	float s = sin(a);
	float c = cos(a);
	return vec2(p.x * c - p.y * s, p.x * s + p.y * c);
}

float rf_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float len = length(p);
	float r = len * 2.0;	// 0 centre, 1 rim
	float th = atan(p.y, p.x);

	// Flame tongues on the rim. INTEGER harmonics of theta only, so the edge closes on itself.
	float edge = 1.0
		- TONGUE_1 * sin(5.0 * th + timer * 1.3)
		- TONGUE_2 * sin(8.0 * th - timer * 2.1);

	float mask = 1.0 - smoothstep(edge * RIM_SOFT, edge, r);
	if(mask <= 0.0)
		return vec4(0.0);

	// Screen radius -> ART radius, stretching the ring band from HOLE out to the rim.
	float t = clamp((r - HOLE) / (1.0 - HOLE), 0.0, 1.0);
	float tra = mix(RING_IN, 1.0, t);
	vec2 dir = p / max(len, 1e-5);

	// Rotation only, offset never exceeds 0.5, so nothing can wrap and nothing can seam.
	vec4 ta = getTexel(0.5 + rf_spin(dir * tra * 0.5, timer * SPIN_A + SHEAR_A * r));
	vec4 tb = getTexel(0.5 + rf_spin(dir * tra * SCALE_B * 0.5, timer * SPIN_B + SHEAR_B * r));

	// SHAPE: where fire exists. Near-binary, and that is fine -- it only has to carve the outline.
	float shape = ta.a * (FLOOR_B + (1.0 - FLOOR_B) * tb.a);
	shape *= smoothstep(0.0, HOLE + 0.14, r);	// soft hole, not a stamped circle

	// TEXTURE: the flame's internal brightness. Alpha is flat inside the cutout, luminance is not.
	float lum = rf_lum(ta.rgb) * (FLOOR_B + (1.0 - FLOOR_B) * rf_lum(tb.rgb));
	lum = pow(clamp(lum, 0.0, 1.0), CONTRAST);

	float band = smoothstep(0.0, 1.0, r);

	// Keep the art's own colour -- it is already good fire -- and only cool it toward the rim.
	vec3 col = mix(ta.rgb, ta.rgb * vec3(1.00, 0.42, 0.12), COOL * band);

	// Two frequencies so it does not pulse on an obvious beat.
	float flick = 1.0 + FLICKER * band *
		(sin(timer * 6.3 + th * 3.0) * 0.6 + sin(timer * 9.1) * 0.4);

	// Brightness from luminance, silhouette from the cutout. Keeping them in separate channels is
	// what stops the result collapsing to a flat shape-coloured blob.
	vec3 rgb = col * lum * GAIN * flick * mask;
	return vec4(rgb * color.rgb, shape * mask * color.a);
}
