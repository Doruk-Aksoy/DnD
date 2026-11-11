// Vampirism's aura, bound to VAMPIRE on VAMPRING.md3. Disc masks: R blood streams, G the dripping rim, B a stream's
// (progress x PULSES + phase) / 3, so the pulses run inward. Only the blood moves; the skin's alpha stays opaque.

uniform float timer;

#define FLOW    0.45   // stream lengths a second a pulse runs inward
#define PULSES  2.0    // pulses along a stream at once
#define GAIN    1.8
#define HEART_R 0.13   // where the streams end, round the caster's feet

float vp_smooth(float a, float b, float x) {
	float t = clamp((x - a) / (b - a), 0.0, 1.0);
	return t * t * (3.0 - 2.0 * t);
}

vec4 Process(vec4 color) {
	vec2 st = gl_TexCoord[0].st;
	float t = mod(timer, 512.0);

	vec2 p = st - 0.5;
	float r = length(p) * 2.0;
	float mask = 1.0 - vp_smooth(0.93, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);

	vec4 m = getTexel(st);
	float ph = fract(m.b * 3.0 - t * FLOW * PULSES);
	float d = min(ph, 1.0 - ph) / 0.09;
	float pulse = exp(-d * d);
	// The pulses die down as they reach the caster: nothing flashes at the middle.
	float calm = vp_smooth(HEART_R, HEART_R + 0.2, r);
	float core = exp(-(r / 0.16) * (r / 0.16));
	float wash = vp_smooth(0.45, 0.85, r) * (1.0 - vp_smooth(0.86, 0.95, r));

	vec3 col = vec3(0.62, 0.02, 0.06) * m.r * (0.7 + 1.3 * pulse * calm) + vec3(1.0, 0.28, 0.24) * m.r * pulse * calm * 0.55 +
		vec3(0.75, 0.05, 0.08) * m.g * 1.1 + vec3(0.35, 0.0, 0.03) * (0.35 * core + 0.12 * wash);
	return vec4(col * GAIN * mask * color.rgb, mask * color.a);
}
