// Blessed Aim's aura on flat.md3, bound to BLESSAIM. Masks: R the arrowheads, G the rims, B the compass rose.
// Arrows of light fly out along the rose's points, each at its own time, lighting the arrowhead they reach.

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

#define FLY     0.45   // flights a second along a point
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
	float kpt = floor(ang * 8.0 + 0.5);
	float rpos = 0.12 + 0.95 * fract(t * FLY + au_hash(vec3(mod(kpt, 8.0), 4.0, 9.0)));
	float dr = (r - rpos) / 0.04;
	float dash = exp(-dr * dr) * au_smooth(0.12, 0.25, r);
	float da = (ang * 8.0 - kpt) / 0.05;
	float aligned = exp(-da * da);
	float core = exp(-(r / 0.2) * (r / 0.2));
	vec3 col = vec3(0.85, 0.92, 1.0) * m.b * (0.32 + 1.0 * dash) + vec3(1.0, 0.82, 0.45) * m.r * (0.6 + 1.6 * dash * aligned) +
		vec3(1.0, 0.88, 0.6) * m.g * 0.8 + vec3(0.9, 0.95, 1.0) * (dash * aligned * 0.25 + core * 0.06);
	return vec4(col * GAIN * mask * color.rgb, mask * color.a);
}
