// Purity of Elements' aura on flat.md3, bound to PURITYEL. Masks: R the four sigils, G the band, B the square inside.
// The disc turns slowly; each sigil takes its element's colour and the square blends between them.

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

#define SPIN    0.015  // turns a second
#define GAIN    1.8

vec3 pe_elem(float i) {
	if(i < 0.5)
		return vec3(1.0, 0.5, 0.15);
	if(i < 1.5)
		return vec3(0.7, 0.92, 1.0);
	if(i < 2.5)
		return vec3(0.35, 0.5, 1.0);
	return vec3(0.45, 1.0, 0.3);
}

vec4 Process(vec4 color) {
	vec2 st = gl_TexCoord[0].st;
	vec2 p = st - 0.5;
	float r = length(p) * 2.0;
	float mask = 1.0 - au_smooth(0.93, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);
	float t = mod(timer, 512.0);
	float ang = atan(p.y, p.x) / TAU + 0.5;
	vec2 q = au_rot(p, -TAU * fract(t * SPIN));
	vec4 m = getTexel(q + 0.5);
	float aq = atan(q.y, q.x) / TAU + 0.5;
	float idx = floor(aq * 4.0);
	float f = aq * 4.0 - 0.5;
	float i0 = floor(f);
	vec3 blend = mix(pe_elem(mod(i0, 4.0)), pe_elem(mod(i0 + 1.0, 4.0)), au_smooth(0.0, 1.0, f - i0));
	float breathe = 0.85 + 0.15 * sin(TAU * fract(t * 0.25 + idx * 0.25));
	float core = exp(-(r / 0.22) * (r / 0.22));
	vec3 col = pe_elem(idx) * m.r * 1.5 * breathe + vec3(0.9, 0.92, 1.0) * m.g * 0.7 + blend * m.b * 0.55 +
		vec3(0.8, 0.8, 0.85) * core * 0.06;
	return vec4(col * GAIN * mask * color.rgb, mask * color.a);
}
