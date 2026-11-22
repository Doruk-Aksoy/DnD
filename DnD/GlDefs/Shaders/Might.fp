// Might's aura on flat.md3, bound to MIGHTAUR. Masks: R the swords, G the heavy rim, B how far up a sword.
// A glint runs up each blade from the hilt to the tip, each sword at its own time.

uniform float timer;

#define TAU     6.2831853

float au_smooth(float a, float b, float x) {
	float t = clamp((x - a) / (b - a), 0.0, 1.0);
	return t * t * (3.0 - 2.0 * t);
}

float au_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

vec2 au_rot(vec2 p, float a) {
	float c = cos(a);
	float s = sin(a);
	return vec2(p.x * c - p.y * s, p.x * s + p.y * c);
}

#define K       12.0   // swords
#define PERIOD  2.6    // seconds between one sword's glints
#define GAIN    1.8

vec4 Process(vec4 color) {
	vec2 st = gl_TexCoord[0].st;
	vec2 p = st - 0.5;
	float r = length(p) * 2.0;
	float mask = 1.0 - au_smooth(0.93, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);
	float t = mod(timer, 512.0);
	float ang = atan(p.y, p.x) / TAU + 0.5;
	vec4 m = getTexel(st);
	float idx = floor(ang * K);
	float gp = -0.1 + 1.4 * fract(t / PERIOD + au_hash(vec3(idx, 11.0, 2.0)));
	float inside = au_smooth(0.05, 0.1, m.b);
	float gd = (m.b - gp) / 0.07;
	float glint = inside * exp(-gd * gd);
	float core = exp(-(r / 0.22) * (r / 0.22));
	float wash = au_smooth(0.25, 0.48, r) * (1.0 - au_smooth(0.48, 0.52, r));
	vec3 col = vec3(1.0, 0.72, 0.38) * m.r * 0.95 + vec3(1.0, 0.86, 0.58) * glint * (m.r + 0.25) * 1.4 +
		vec3(1.0, 0.6, 0.25) * m.g * 0.9 + vec3(1.0, 0.55, 0.2) * (core * 0.08 + wash * 0.06);
	return vec4(col * GAIN * mask * color.rgb, mask * color.a);
}
