// Upheaval's cracks. Bound to UPCRACK in GlDefs/Shaders.txt.
// The skin's alpha is the cutout and the crack mask: 0 off the crack, 0.55 at its lips, up to 1 down its middle.

#ifdef NO_FOG
varying vec4 pixelpos;
#endif

uniform float timer;

#define PERIOD   1.6    // seconds from one pulse to the next, under half of Threaded Volley's soil
#define WIDTH    0.3    // the share of a period one spot stays lit
#define LIP      0.55
#define EMBER    0.14   // the glow left between pulses while the rifts tear
#define GAIN     0.95
#define HEATBASE 0.1    // DND_UPHEAVAL_HEATBASE: from here up the upright stretch is a blast's heat
#define BLAST    1.7    // the glow at full heat

vec4 Process(vec4 color) {
	vec4 t = getTexel(gl_TexCoord[0].st);
	float deep = clamp((t.a - LIP) / (1.0 - LIP), 0.0, 1.0);
	vec3 glow;

	// ACS stretches the upright axis past the length's scale: not at all while the rifts tear, by HEATBASE + heat after.
	float v = length(gl_TextureMatrix[7][1].xyz) / length(gl_TextureMatrix[7][0].xyz) - 1.0;
	if(v < HEATBASE * 0.5) {
		// The beat comes from where the pixel lies, smooth everywhere, so a pulse runs on across the segments' joins.
		vec3 w = pixelpos.xyz;
		float x = fract(timer / PERIOD - 0.35 * (sin(w.x * 0.013) + sin(w.z * 0.011)) - (w.x + w.z) * 0.002);
		float s = x < WIDTH ? sin(3.14159265 * x / WIDTH) : 0.0;
		glow = vec3(1.0, 0.42, 0.08) * deep * deep * deep * (EMBER + s * s * GAIN);
	}
	else {
		// A blast: the whole crack bright orange, its core hotter, cooling to black as the heat runs out.
		float heat = clamp(v - HEATBASE, 0.0, 1.0);
		glow = mix(vec3(1.0, 0.4, 0.04), vec3(1.0, 0.62, 0.18), deep * deep * heat) * pow(deep, 1.4) * heat * BLAST;
	}

	// color carries the light alone. The glow is unlit, so it shows in the dark; the alpha test sees only the cutout.
	return vec4(t.rgb * color.rgb + glow, step(0.5, t.a) * color.a);
}
