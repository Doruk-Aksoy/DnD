// Haste's aura on flat.md3, bound to HASTEAUR. Masks: R the chevrons, G the lines, B wind streaks.
// The streaks race round and lights run along the chevrons the way they point.

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

#define WALK    0.45   // turns a second the lights run
#define HEADS   4.0
#define SPIN    0.32   // turns a second the wind goes round
#define GAIN    1.9

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
	float ws = getTexel(au_rot(p, -TAU * fract(t * SPIN)) + 0.5).b;
	float dd = fract((t * WALK - ang) * HEADS);
	float walk = exp(-dd * 6.0) * (1.0 - au_smooth(0.95, 1.0, dd));
	float core = exp(-(r / 0.22) * (r / 0.22));
	vec3 col = vec3(1.0, 0.85, 0.45) * m.r * (0.8 + 1.4 * walk) + vec3(1.0, 0.78, 0.4) * m.g * 0.7 +
		vec3(0.85, 0.95, 1.0) * ws * 0.85 + vec3(1.0, 0.9, 0.65) * core * 0.08;
	return vec4(col * GAIN * mask * color.rgb, mask * color.a);
}
