// Ethereal Body's screen haze (HUD image ETHVIG over the whole screen; the shader draws it all): a cold mist at the edges,
// thicker in drifting wisps that curl in a little way and thin out again.

uniform float timer;

#define TAU     6.2831853
#define DEPTH   0.09    // the haze's falloff in from the edges, in screen heights
#define WISP    0.16    // how far the wisps reach

float ev_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float ev_noise(vec2 x) {
	vec2 i = floor(x);
	vec2 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float a = ev_hash(vec3(mod(i, 256.0), 3.0));
	float b = ev_hash(vec3(mod(i + vec2(1.0, 0.0), 256.0), 3.0));
	float c = ev_hash(vec3(mod(i + vec2(0.0, 1.0), 256.0), 3.0));
	float d = ev_hash(vec3(mod(i + vec2(1.0, 1.0), 256.0), 3.0));
	return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

float ev_fbm(vec2 x) {
	return ev_noise(x) * 0.6 + ev_noise(x * 2.1 + 17.0) * 0.3 + ev_noise(x * 4.3 + 41.0) * 0.1;
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float t = mod(timer, 512.0);
	// the screen's width over its height, from the image's own stretch across a pixel
	float asp = abs(dFdy(uv.y)) / max(abs(dFdx(uv.x)), 1e-6);
	vec2 q = vec2((uv.x - 0.5) * asp, uv.y - 0.5);
	float dx = 0.5 * asp - abs(q.x);
	float dy = 0.5 - abs(q.y);
	float ds = max(-0.03 * log(exp(-dx / 0.03) + exp(-dy / 0.03)), 0.0);

	// the mist drifts slowly; its wisps breathe in and out together, gently
	vec2 drift = vec2(t * 0.035, -t * 0.025);
	float m = ev_fbm(q * 4.0 + drift);
	float w = ev_fbm(q * 7.0 - drift * 1.6 + 5.0);
	float breathe = 0.5 + 0.5 * sin(TAU * fract(t * 0.07));
	float reach = WISP * (0.75 + 0.25 * breathe);
	float haze = exp(-ds / DEPTH) * (0.55 + 0.45 * m);
	float wisp = smoothstep(0.5, 0.78, w) * (1.0 - smoothstep(reach * 0.3, reach, ds));
	vec3 col = vec3(0.5, 0.72, 0.85) * haze * 0.8 + vec3(0.8, 0.93, 1.0) * wisp * 0.55;
	return vec4(min(col, vec3(1.0)), color.a);
}
