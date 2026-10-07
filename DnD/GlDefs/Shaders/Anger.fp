// Anger's aura. Bound to ANGERAUR in GlDefs/Shaders.txt.
//
// This art has NO alpha channel -- it is a spiral burst drawn on black, and the actor renders Add,
// which makes black invisible on its own. So luminance is the shape and the intensity at once, and
// there is no cutout to mask with.
//
// Darker and slower than the Righteous Fire disc on purpose: this one sits under the player for as
// long as the aura is up, so it has to read as a presence rather than compete with the fight.

uniform float timer;

#define SPIN_A    0.22   // the spiral itself turns
#define SPIN_B   -0.15   // and a second copy turns back through it
#define SHEAR     0.40   // extra turn at the rim, so the arms trail
#define SCALE_B   0.78
#define RIM_SOFT  0.90
#define CORE      0.26
#define CORE_DIM  0.30   // how far the middle is held DOWN; the art is a flat white blob there
#define CORE_EDGE 0.46   // and where it has recovered to full
#define EMBER     0.65   // how far the outer arms cool toward red
#define CONTRAST  1.60   // harder than it looks: it is what separates the arms from the wash
#define GAIN      0.875   // deliberately under 1: the brief was darker

// The slow breath. A plain sine never sits still, so it reads as a flicker rather than a swell --
// these four marks are a phase each, giving it a hold at the bottom, a rise, a hold at the top and
// a fall. Fractions of one period.
#define PULSE_PERIOD 7.0
#define PULSE_RISE0  0.12
#define PULSE_RISE1  0.34
#define PULSE_FALL0  0.62
#define PULSE_FALL1  0.84
#define PULSE_MIN    0.72
#define PULSE_MAX    1.18

vec2 an_spin(vec2 p, float a) {
	float s = sin(a);
	float c = cos(a);
	return vec2(p.x * c - p.y * s, p.x * s + p.y * c);
}

float an_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;

	float mask = 1.0 - smoothstep(RIM_SOFT, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);

	// Rotation only, and the offset never exceeds 0.5, so nothing can leave the texture and there is
	// nothing to wrap. The art is already a spiral, so turning it IS the motion.
	vec4 ta = getTexel(0.5 + an_spin(p, timer * SPIN_A + SHEAR * r));
	vec4 tb = getTexel(0.5 + an_spin(p * SCALE_B, timer * SPIN_B - SHEAR * r));

	float heat = an_lum(ta.rgb) * (0.45 + 0.75 * an_lum(tb.rgb));
	heat = pow(clamp(heat, 0.0, 1.0), CONTRAST);

	// Hold the middle down. The art is blown to flat white at its centre, so every pixel in there
	// came out the same value and the whole middle rendered as one muddy plate -- there was simply
	// no structure left to show. Dimmed, the arms carry the image and the core reads as a glow
	// behind them rather than a hole punched in the floor.
	heat *= mix(CORE_DIM, 1.0, smoothstep(0.0, CORE_EDGE, r));

	// Keeps the art's own colour and only cools the outer arms, rather than repainting it.
	float band = smoothstep(CORE, 1.0, r);
	vec3 col = mix(ta.rgb, ta.rgb * vec3(1.00, 0.42, 0.12), EMBER * band);

	// Toward a saturated orange as it gets hottest, not toward white. Pale was the other half of
	// why the middle looked washed out.
	col = mix(col, col * vec3(1.00, 0.68, 0.26), smoothstep(0.55, 1.0, heat));

	// One period, four phases: hold low, rise, hold high, fall. min() of a rising and a falling edge
	// gives the flat tops and bottoms -- the dwell is the point, and a sine has none.
	float ph = fract(timer / PULSE_PERIOD);
	float env = min(smoothstep(PULSE_RISE0, PULSE_RISE1, ph),
		1.0 - smoothstep(PULSE_FALL0, PULSE_FALL1, ph));
	float pulse = mix(PULSE_MIN, PULSE_MAX, env);

	// No cutout in the alpha: the black surround already contributes nothing under Add, so the mask
	// only has to keep the rim circular.
	vec3 rgb = col * heat * GAIN * pulse * mask;
	return vec4(rgb * color.rgb, mask * color.a);
}
