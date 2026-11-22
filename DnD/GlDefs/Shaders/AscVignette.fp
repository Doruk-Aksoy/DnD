// Ascension's screen vignette (HUD image ASCVIG over the whole screen; the shader draws it all): an edge glow, breathing,
// and soft violet tendrils reaching in from the edges, each creeping inward and drawing back on its own slow rhythm.

uniform float timer;

#define TAU     6.2831853
#define TEND    7.0     // tendrils per screen height along each edge
#define REACH0  0.05    // a tendril's shortest reach, in screen heights from the nearest edge
#define REACH1  0.17    // its longest
#define BREATH  0.11    // its in-and-out cycles a second

float av_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Value noise wrapping every per along x, free along y.
float av_noise(vec2 x, float per) {
	vec2 i = floor(x);
	vec2 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float x0 = mod(i.x, per);
	float x1 = mod(i.x + 1.0, per);
	float a = av_hash(vec3(x0, i.y, 5.0));
	float b = av_hash(vec3(x1, i.y, 5.0));
	float c = av_hash(vec3(x0, i.y + 1.0, 5.0));
	float d = av_hash(vec3(x1, i.y + 1.0, 5.0));
	return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

// The tendrils of one edge: along runs down that edge, depth in from it (both in screen heights). Each stretch breathes
// on its own slow rhythm blended into the next; the bodies are ridges of noise, swaying a little sideways.
float av_tendrils(float along, float depth, float seed, float t) {
	float a = along * TEND + seed * 37.0;
	float cell = floor(a);
	float fa = a - cell;
	fa = fa * fa * (3.0 - 2.0 * fa);
	float ph0 = av_hash(vec3(mod(cell, 256.0), seed, 9.0));
	float ph1 = av_hash(vec3(mod(cell + 1.0, 256.0), seed, 9.0));
	float br0 = 0.5 + 0.5 * sin(TAU * fract(t * BREATH * (0.7 + 0.6 * ph0) + ph0));
	float br1 = 0.5 + 0.5 * sin(TAU * fract(t * BREATH * (0.7 + 0.6 * ph1) + ph1));
	float br = mix(br0, br1, fa);
	float reach = mix(REACH0, REACH1, br * (0.4 + 0.6 * av_noise(vec2(a * 0.5, t * 0.05 + seed), 256.0)));
	float sway = 0.8 * (av_noise(vec2(a * 0.25, t * 0.04 + 7.0 + seed), 256.0) - 0.5);
	float n = av_noise(vec2(a * 2.0 + sway, depth * 3.0 - t * 0.25 + seed * 5.0), 256.0);
	n = 1.0 - abs(2.0 * n - 1.0);
	n = n * n * n * n;
	return n * (1.0 - smoothstep(reach * 0.35, reach, depth)) * (0.6 + 0.4 * br);
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float t = mod(timer, 512.0);
	// the screen's width over its height, from the image's own stretch across a pixel
	float asp = abs(dFdy(uv.y)) / max(abs(dFdx(uv.x)), 1e-6);
	vec2 q = vec2((uv.x - 0.5) * asp, uv.y - 0.5);
	float dx = 0.5 * asp - abs(q.x);
	float dy = 0.5 - abs(q.y);
	// the edge glow's distance: the two edges blended, so the corners round off with no seam
	float ex = exp(-dx / 0.03);
	float ey = exp(-dy / 0.03);
	float ds = -0.03 * log(ex + ey);
	vec3 base = vec3(0.62, 0.32, 1.0) * exp(-max(ds, 0.0) / 0.09) * 0.8 + vec3(0.85, 0.72, 1.0) * exp(-max(ds, 0.0) / 0.02) * 0.3;

	// top and bottom tendrils reach down and up, the sides' across, each set weighted to its own edge near a corner
	float wy = ey / (ex + ey);
	float tend = wy * av_tendrils(q.x, dy, q.y < 0.0 ? 1.0 : 2.0, t) + (1.0 - wy) * av_tendrils(q.y, dx, q.x < 0.0 ? 3.0 : 4.0, t);

	float breathe = 0.85 + 0.15 * sin(TAU * fract(t * 0.08));
	vec3 col = base * breathe + vec3(0.62, 0.32, 1.0) * tend * 0.9 + vec3(0.9, 0.8, 1.0) * tend * tend * 0.5;
	return vec4(min(col, vec3(1.0)), color.a);
}
