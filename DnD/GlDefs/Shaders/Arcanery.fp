// Arcanery's aura on flat.md3, bound to ARCANERY. Masks: R the outer rune ring, G the inner one, B the star.
// The rune rings turn against each other and their runes glimmer; the star stays still.

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

#define SPIN_O  0.025  // turns a second, the outer ring
#define SPIN_I  0.04   // the inner ring, the other way
#define RUNES_O 30.0
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
	vec2 qo = au_rot(p, -TAU * fract(t * SPIN_O));
	float mo = getTexel(qo + 0.5).r;
	float mi = getTexel(au_rot(p, TAU * fract(t * SPIN_I)) + 0.5).g;
	float mh = getTexel(st).b;
	float idx = floor((atan(qo.y, qo.x) / TAU + 0.5) * RUNES_O);
	float tw = 0.8 + 0.3 * sin(TAU * fract(t * 0.35 + au_hash(vec3(idx, 3.0, 5.0))));
	float core = exp(-(r / 0.22) * (r / 0.22));
	float wash = au_smooth(0.3, 0.6, r) * (1.0 - au_smooth(0.62, 0.7, r));
	vec3 col = vec3(0.72, 0.42, 1.0) * mo * tw * 1.3 + vec3(0.42, 0.62, 1.0) * mi * 1.2 + vec3(0.62, 0.5, 1.0) * mh * 0.7 +
		vec3(0.5, 0.35, 1.0) * (core * 0.08 + wash * 0.06);
	return vec4(col * GAIN * mask * color.rgb, mask * color.a);
}
