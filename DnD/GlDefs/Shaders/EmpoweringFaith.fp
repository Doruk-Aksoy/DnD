// Empowering Faith's aura on flat.md3, bound to EMPFAITH. Masks: R the shields, G the lines, B how far up a shield.
// A sweep goes round; each shield fills with light from its point as it passes, then fades.

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

#define K       16.0   // shields
#define SWEEP   0.05   // turns a second
#define HEADS   2.0
#define FILL    0.12   // share of the gap behind a sweep a shield takes to fill
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
	float idx = floor(ang * K);
	float d = fract((t * SWEEP - (idx + 0.5) / K) * HEADS);
	float lvl = 0.1 + 0.95 * clamp(d / FILL, 0.0, 1.0);
	vec4 m = getTexel(st);
	float inside = au_smooth(0.05, 0.1, m.b);
	float lit = inside * (1.0 - au_smooth(lvl - 0.05, lvl, m.b));
	float fr = (m.b - lvl) / 0.035;
	float front = inside * exp(-fr * fr) * (1.0 - au_smooth(FILL * 0.8, FILL * 1.1, d));
	float fade = exp(-d * 2.5);
	float core = exp(-(r / 0.22) * (r / 0.22));
	float wash = au_smooth(0.3, 0.66, r) * (1.0 - au_smooth(0.68, 0.72, r));
	vec3 col = vec3(0.6, 0.82, 1.0) * m.r * (0.55 + 0.6 * fade) + vec3(0.45, 0.7, 1.0) * m.g * 0.75 +
		vec3(0.7, 0.9, 1.0) * lit * fade * 0.75 + vec3(1.0) * front * 1.1 + vec3(0.5, 0.75, 1.0) * (core * 0.1 + wash * 0.07);
	return vec4(col * GAIN * mask * color.rgb, mask * color.a);
}
