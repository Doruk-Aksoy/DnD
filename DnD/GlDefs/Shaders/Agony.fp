// Agony's aura, bound to AGONY. The skin is masks: R veins, G the thorn rim, B a flesh noise for the bruise haze.
// Only the veins pulse, a heartbeat whose waves run out from the middle; the thorns turn against the disc.

uniform float timer;

#define TAU        6.2831853
#define BEAT       1.15   // seconds a heartbeat
#define RIM_R      0.86   // the thorn band's radius, in disc radii
#define THORN_SPIN 0.06   // turns a second the thorns turn, against the disc
#define HEART_R    0.11   // where the veins start; the heartbeat's waves start here too
#define GAIN       1.6

float ag_smooth(float a, float b, float x) {
	float t = clamp((x - a) / (b - a), 0.0, 1.0);
	return t * t * (3.0 - 2.0 * t);
}

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;
	float mask = 1.0 - ag_smooth(0.93, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);

	vec3 t = getTexel(p + 0.5).rgb;
	float ta = -timer * THORN_SPIN * TAU;
	float c = cos(ta), s = sin(ta);
	float thorn = getTexel(vec2(p.x * c - p.y * s, p.x * s + p.y * c) + 0.5).g;

	// A lub and a smaller dub, then rest; each sends a wave out from the middle along the veins.
	float ph = fract(timer / BEAT);
	float beat = exp(-pow(ph / 0.045, 2.0)) + exp(-pow((ph - 1.0) / 0.045, 2.0)) + 0.7 * exp(-pow((ph - 0.2) / 0.045, 2.0));
	float front = HEART_R + ph * 2.4;
	float wave = exp(-pow((r - front) / 0.07, 2.0)) + 0.7 * exp(-pow((r - front + 0.48) / 0.07, 2.0));
	float core = exp(-pow(r / 0.15, 2.0));
	// The middle stays steady: the pulse builds as it travels out.
	float calm = ag_smooth(HEART_R, HEART_R + 0.3, r);

	float haze = ag_smooth(0.3, 0.8, t.b) * ag_smooth(0.05, 0.6, r) * (1.0 - ag_smooth(0.82, 0.95, r));
	vec3 green = vec3(0.32, 0.95, 0.18);
	vec3 hot = vec3(1.0, 0.35, 0.25);
	float vglow = t.r * (0.85 + (0.8 * beat + 1.4 * wave) * calm);
	vec3 col = vec3(0.30, 0.07, 0.34) * haze * 0.7 + green * vglow + hot * t.r * wave * calm * 0.6 +
		green * 0.3 * core + vec3(0.42, 0.55, 0.28) * thorn * 1.3;
	return vec4(col * GAIN * mask * color.rgb, mask * color.a);
}
