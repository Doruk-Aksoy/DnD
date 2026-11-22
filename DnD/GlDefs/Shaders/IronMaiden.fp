// Iron Maiden's aura on flat.md3, bound to IRONMAID, drawn Translucent. Masks: R the iron's shading, G coverage,
// B the heat. It turns slowly, takes the sector's light, and only the spikes' red-hot tips glow, each at its own pace.

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

#define K       18.0   // spikes
#define SPIN    0.02   // turns a second the band goes round

vec4 Process(vec4 color) {
	vec2 st = gl_TexCoord[0].st;
	vec2 p = st - 0.5;
	float r = length(p) * 2.0;
	float mask = 1.0 - au_smooth(0.93, 1.0, r);
	float t = mod(timer, 512.0);
	vec2 q = au_rot(p, -TAU * fract(t * SPIN));
	vec4 m = getTexel(q + 0.5);
	float a = m.g * mask;
	if(a < 0.01)
		discard;
	float ang = atan(q.y, q.x) / TAU + 0.5;
	float idx = floor(ang * K);
	float hb = 0.65 + 0.35 * sin(TAU * fract(t * 0.3 + au_hash(vec3(idx, 7.0, 1.0))));
	vec3 iron = vec3(0.58, 0.58, 0.64) * m.r * (1.6 * color.rgb + 0.14);
	vec3 glow = vec3(1.0, 0.28, 0.08) * m.b * hb * 1.6 + vec3(1.0, 0.6, 0.2) * m.b * m.b * hb;
	return vec4(min(iron + glow, vec3(1.0)), a);
}
