// Conviction's mark at a marked enemy's feet, on flat.md3, bound to CVMRING. Masks: R four brands, G the rims and dashes.
// It turns; the brands breathe. Faded in and out through the actor's alpha.

uniform float timer;

#define SPIN    0.12   // turns a second
#define TAU     6.2831853

float cm_smooth(float a, float b, float x) {
	float t = clamp((x - a) / (b - a), 0.0, 1.0);
	return t * t * (3.0 - 2.0 * t);
}

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;
	float mask = 1.0 - cm_smooth(0.93, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);
	float t = mod(timer, 512.0);
	float a = -TAU * fract(t * SPIN);
	vec2 q = vec2(p.x * cos(a) - p.y * sin(a), p.x * sin(a) + p.y * cos(a));
	vec4 m = getTexel(q + 0.5);
	float breathe = 0.8 + 0.2 * sin(TAU * fract(t * 0.8));
	vec3 col = vec3(0.66, 0.3, 1.0) * m.g * 1.5 + vec3(1.0, 0.82, 0.5) * m.r * 1.6 * breathe +
		vec3(0.5, 0.2, 0.9) * exp(-(r / 0.5) * (r / 0.5)) * 0.12;
	return vec4(col * mask * color.rgb, mask * color.a);
}
