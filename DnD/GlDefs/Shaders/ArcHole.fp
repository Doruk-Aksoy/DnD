// Arcane Trap's black hole on the floor (flat2, Translucent at a low actor alpha so the alpha returned here drives the
// blend): a black heart, violet arms spiralling in and turning, a pale rim round the heart, fading out at the edge.

uniform float timer;

#define TAU     6.2831853
#define ARMS    3.0
#define TWIST   2.6     // turns the arms take from the edge to the heart, in radians per e-fold of radius
#define SPIN    1.4     // turns a second

float ah_hash(vec2 p) {
	vec3 q = fract(mod(vec3(p.xyx), 256.0) * 0.1031);
	q += dot(q, q.yzx + 33.33);
	return fract((q.x + q.y) * q.z);
}

float ah_noise(vec2 p) {
	vec2 i = floor(p);
	vec2 f = p - i;
	f = f * f * (3.0 - 2.0 * f);
	return mix(mix(ah_hash(i), ah_hash(i + vec2(1.0, 0.0)), f.x), mix(ah_hash(i + vec2(0.0, 1.0)), ah_hash(i + vec2(1.0, 1.0)), f.x), f.y);
}

vec4 Process(vec4 color) {
	vec2 p = (gl_TexCoord[0].st - 0.5) * 2.0;
	float r = length(p);
	float t = mod(timer, 512.0);
	float a = atan(p.y, p.x + 1e-6);

	// the arms: a spiral phase, warped by noise so they fray rather than read as stripes
	float lr = log(max(r, 0.02));
	float ph = a * ARMS + lr * TWIST * ARMS + TAU * fract(t * SPIN);
	float wob = ah_noise(vec2(a * 3.0 + lr * 4.0, t * 0.7)) - 0.5;
	float arm = pow(0.5 + 0.5 * cos(ph + wob * 2.5), 3.0);
	arm *= smoothstep(0.08, 0.3, r) * (1.0 - smoothstep(0.82, 1.0, r));
	float grit = ah_noise(vec2(a * 9.0 + t * 3.0, lr * 14.0 - t * 5.0));
	arm *= 0.6 + 0.6 * grit;

	float heart = 1.0 - smoothstep(0.1, 0.32, r);
	float rim = exp(-(r - 0.2) * (r - 0.2) / 0.0015);
	float shade = (1.0 - smoothstep(0.6, 1.0, r)) * 0.55;

	vec3 col = vec3(0.012, 0.0, 0.03) * heart + vec3(0.58, 0.32, 1.0) * arm * 1.3 + vec3(0.9, 0.8, 1.0) * rim;
	float alpha = clamp(max(max(heart * 0.95, shade), arm * 0.9 + rim), 0.0, 1.0) * (1.0 - smoothstep(0.9, 1.0, r));
	return vec4(min(col, vec3(1.0)), alpha);
}
