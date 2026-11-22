// Arcane Trap's sigil on the floor (flat2): two thin rings round a band of runes (the skin's 32-rune strip) turning one
// way and a triangle the other, a soft armed pulse running outward. The actor's alpha draws it in, sweeping round.

uniform float timer;

#define TAU     6.2831853
#define RUNES   12.0    // rune cells round the band, each a different rune
#define GLYPHS  32.0    // runes in the skin's strip
#define BAND0   0.66    // the band's inner and outer radius, in sigil radii
#define BAND1   0.86

float at_line(float d, float wid) {
	return exp(-d * d / (wid * wid));
}

float at_glyph(float cell, float gx, float cy) {
	float idx = mod(cell * 7.0 + 3.0, GLYPHS);
	float inside = step(0.0, gx) * step(gx, 1.0) * step(0.0, cy) * step(cy, 1.0);
	vec2 st = vec2((idx + clamp(gx, 0.02, 0.98)) / GLYPHS, 1.0 - clamp(cy, 0.02, 0.98));
	return getTexel(st).r * inside;
}

// distance from p to an equilateral triangle of circumradius r turned by rot
float at_tri(vec2 p, float r, float rot) {
	float d = -1e3;
	for(int k = 0; k < 3; k++) {
		float a = rot + TAU * (float(k) + 0.5) / 3.0;
		d = max(d, dot(p, vec2(cos(a), sin(a))) - r * 0.5);
	}
	return d;
}

vec4 Process(vec4 color) {
	vec2 p = (gl_TexCoord[0].st - 0.5) * 2.0;
	float r = length(p);
	float t = mod(timer, 512.0);
	float a = atan(p.y, p.x + 1e-6);
	float drawn = clamp(color.a / 0.9, 0.0, 1.0);

	// drawn in by the actor's alpha (whole from 0.9): the rings sweep round, the rest fades up behind them
	float sweep = drawn >= 1.0 ? 1.0 : 1.0 - smoothstep(drawn * 1.05 - 0.05, drawn * 1.05, fract(a / TAU + 0.25));
	float fill = smoothstep(0.4, 1.0, drawn);

	float rings = at_line(r - 0.92, 0.022) + at_line(r - 0.62, 0.018);
	float ra = fract(a / TAU + fract(t * 0.05)) * RUNES;
	float cy = (r - BAND0) / (BAND1 - BAND0);
	float gx = (fract(ra) - 0.5) * (TAU * r / RUNES) / (BAND1 - BAND0) + 0.5;
	float runes = at_glyph(floor(ra), gx, cy);
	float tri = at_line(at_tri(p, 0.56, -TAU * fract(t * 0.035)), 0.02);
	float core = at_line(r - 0.12, 0.02) + exp(-r * r / 0.004) * 0.6;

	// the armed pulse: a soft brightening running out every 1.6 seconds
	float ph = fract(t / 1.6);
	float pulse = exp(-(r - ph * 1.1) * (r - ph * 1.1) / 0.01) * (1.0 - ph);

	vec3 vio = vec3(0.62, 0.38, 1.0), pale = vec3(0.88, 0.8, 1.0);
	vec3 col = pale * rings * sweep + (vio * runes * 0.9 + vio * tri * 0.8 + pale * core * 0.7) * fill +
		vio * pulse * 0.35 * fill + vio * exp(-r * r / 0.35) * 0.08 * fill;
	col *= 0.85 * (1.0 - smoothstep(0.95, 1.0, r));
	return vec4(min(col, vec3(1.0)), 1.0);
}
