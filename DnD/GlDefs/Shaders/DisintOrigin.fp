// Disintegrate's origin: an arcane circle standing across the ray at the hands, a runic band and a dashed ring turning
// opposite ways round a white-hot core. Alpha above 0.5 is the intensity (as on the ray); below it the circle fades out.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define TAU     6.2831853
#define RUNES   20.0    // glyph cells round the runic band, each a different rune
#define GLYPHS  32.0    // runes in the skin's strip
#define BAND0   0.70    // the runic band's inner and outer radius, in disc radii
#define BAND1   0.88

float dgo_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float dgo_line(float d, float wid) {
	return exp(-d * d / (wid * wid));
}

// one glyph of the skin's 32-rune strip; gx across the glyph, cy across the band (outward up), both 0..1, kept inside
float dgo_glyph(float cell, float gx, float cy) {
	float idx = mod(cell * 13.0, GLYPHS);
	float inside = step(0.0, gx) * step(gx, 1.0) * step(0.0, cy) * step(cy, 1.0);
	vec2 st = vec2((idx + clamp(gx, 0.02, 0.98)) / GLYPHS, 1.0 - clamp(cy, 0.02, 0.98));
	return getTexel(st).r * inside;
}

vec4 Process(vec4 color) {
	vec2 p = (gl_TexCoord[0].st - 0.5) * 2.0;
	float r = length(p);
	float t = mod(timer, 512.0);
	float inten = clamp((color.a - 0.5) * 2.0, 0.0, 1.0);
	float fade = smoothstep(0.0, 0.5, color.a);
	float a = atan(p.y, p.x + 1e-6);

	// the runic band, turning slowly
	float ra = fract(a / TAU + fract(t * 0.12)) * RUNES;
	float cell = floor(ra);
	float cy = (r - BAND0) / (BAND1 - BAND0);
	float gx = (fract(ra) - 0.5) * (TAU * r / RUNES) / (BAND1 - BAND0) + 0.5;
	float runes = dgo_glyph(cell, gx, cy);
	float rims = dgo_line(r - BAND1 - 0.04, 0.018) + dgo_line(r - BAND0 + 0.04, 0.015);

	// the dashed inner ring, turning the other way and faster
	float da = fract(a / TAU - fract(t * 0.3)) * 10.0;
	float dash = step(0.35, fract(da)) * step(0.25, dgo_hash(vec3(floor(da), 5.0, 7.0)));
	float inner = dgo_line(r - 0.48, 0.025) * dash;

	// the core the ray leaves from, and a soft haze inside the band
	float core = exp(-r * r / 0.025);
	float haze = exp(-r * r / 0.18) * 0.35;

	vec3 col = vec3(0.75, 0.55, 1.0) * (runes * 0.9 + rims * 0.8) + vec3(0.85, 0.7, 1.0) * inner +
		vec3(1.0, 0.97, 1.0) * core * (1.0 + 0.4 * inten) + vec3(0.55, 0.3, 1.0) * haze;
	col *= (0.7 + 0.3 * inten) * fade * (1.0 - smoothstep(0.92, 1.0, r));

	// edge-on the disc is a sliver: dim it rather than draw a bright line
	vec3 nrm = normalize(gl_TextureMatrix[7][0].xyz);
	vec3 v = pixelpos.xyz - camerapos;
	col *= 0.25 + 0.75 * smoothstep(0.08, 0.4, abs(dot(nrm, v)) / max(length(v), 1e-4));
	return vec4(min(col, vec3(1.0)), 1.0);
}
