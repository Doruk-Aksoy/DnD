// Conviction's aura on flat.md3, bound to CONVICTN. Masks: R the brands, G the rims and links, B the spokes.
// The brands breathe, each at its own pace; the sweep is Spell_Conviction_Wave, sent with each real marking pulse.

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

#define K       12.0   // brands
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
	float breathe = 0.8 + 0.2 * sin(TAU * fract(t * 0.3 + au_hash(vec3(idx, 5.0, 3.0))));
	float core = exp(-(r / 0.22) * (r / 0.22));
	vec3 col = vec3(0.62, 0.3, 1.0) * m.g * 0.8 + vec3(0.62, 0.3, 1.0) * m.b * 0.4 +
		vec3(0.95, 0.66, 0.45) * m.r * 0.85 * breathe + vec3(0.55, 0.3, 1.0) * core * 0.08;
	return vec4(col * GAIN * mask * color.rgb, mask * color.a);
}
