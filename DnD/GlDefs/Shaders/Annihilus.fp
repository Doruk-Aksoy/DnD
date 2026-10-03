// Annihilus' charge. Bound to ANNIHILS in GlDefs/Shaders.txt.
//
// The art is a FILLED lava disc: alpha is a clean circular cutout, RGB carries the crust and the
// hot patches. Shape from alpha, structure from luminance -- see the notes in .claude.
//
// Slow on purpose. This is a mass of molten rock building toward a detonation, not a flame, so
// everything here churns and swells rather than flickers.

uniform float timer;

#define CHURN_A    0.07   // inner crust rotation, radians/sec
#define CHURN_B   -0.11   // outer counter-rotates, so the two grind
#define SHEAR      0.35   // extra turn at the rim: convection, not a spinning plate
#define SCALE_B    0.86
#define RIM_SOFT   0.94   // the art cuts its own circle; this only guards the very edge
#define VEIN_LO    0.20   // luminance where crust starts to glow
#define VEIN_HI    0.68   // and where it is fully molten
#define VEIN_DRIFT 0.13   // how far the molten band wanders, so hot spots migrate
#define RIMHEAT    0.50   // molten rock glows hottest where it meets air
#define PULSE      0.20   // the slow swell of the whole mass
#define CONTRAST   1.25
#define GAIN       1.90   // the charge is meant to be the brightest thing in the room.
                          // Only the molten patches clip, which is what white hot lava is.

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
	float r = length(p) * 2.0;	// 0 centre, 1 rim
	float th = atan(p.y, p.x);

	float mask = 1.0 - smoothstep(RIM_SOFT, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);

	// Rotation only, and the offset never exceeds 0.5, so no sample can leave the texture and
	// there is nothing to wrap. A translated sample would tile the disc into the corners.
	vec4 ta = getTexel(0.5 + an_spin(p, timer * CHURN_A + SHEAR * r));
	vec4 tb = getTexel(0.5 + an_spin(p * SCALE_B, timer * CHURN_B - SHEAR * r));

	// Alpha is the cutout and nothing else; luminance is where the crust detail lives.
	float shape = ta.a;
	float heat = an_lum(ta.rgb) * (0.40 + 0.80 * an_lum(tb.rgb));
	heat = pow(clamp(heat, 0.0, 1.0), CONTRAST);

	// The molten threshold WANDERS instead of sitting still, so different parts of the crust open
	// up over time rather than the same patches being permanently bright.
	float lo = VEIN_LO + VEIN_DRIFT * sin(timer * 0.9 + th * 2.0 + r * 3.0);
	float vein = smoothstep(lo, VEIN_HI, heat);

	vec3 col = mix(vec3(0.14, 0.02, 0.01), vec3(0.92, 0.20, 0.02), smoothstep(0.0, 0.45, heat));
	col = mix(col, vec3(1.00, 0.70, 0.16), vein);
	col = mix(col, vec3(1.00, 0.94, 0.68), smoothstep(0.80, 1.0, heat) * vein);

	col += vec3(0.55, 0.16, 0.02) * RIMHEAT * smoothstep(0.55, 1.0, r) * heat;

	// Two frequencies again, so the swell never lands on an obvious beat.
	float pulse = 1.0 + PULSE * (sin(timer * 1.6) * 0.6 + sin(timer * 2.7 + 1.3) * 0.4);

	// 0.45 floor, not 0.35: the dark crust still has to read as glowing rock rather than as a
	// hole in the middle of the light it is casting.
	vec3 rgb = col * (0.45 + 0.85 * heat) * GAIN * pulse * mask;

	// Mask in the alpha too: under Add the result is roughly rgb*alpha.
	return vec4(rgb * color.rgb, shape * mask * color.a);
}
