// Crackling Lance by stage: wide branches with no laser, a thin laser with fewer and shorter branches, tangled bolts round a
// bright core, then a big plasma laser with curling tendrils. The soft round glow round it is a chain of sprites from ACS.

// NO_FOG marks the colormap pass, whose base declares neither.
#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define LEN    64.0    // the model's length and its planes' half width, in model units
#define HALF   40.0
#define RATE   20.0    // reshuffles a second
#define CORE   16.0    // world half width of the laser's core at full intensity

float h1(float n) { return fract(sin(n) * 43758.5453); }

// Smooth value noise in -1..1, a random height at each whole x. Both sides of a whole x hash the same exact integer,
// so the curve stays joined there.
float vnoise(float x, float seed) {
	float n = floor(x), f = x - n;
	f = f * f * (3.0 - 2.0 * f);
	return mix(h1(n * 0.731 + seed), h1((n + 1.0) * 0.731 + seed), f) * 2.0 - 1.0;
}

// A continuous wandering curve of unit size: smooth waves crackling in two octaves at uneven spacing, phases dealt per reshuffle.
float wave(float u, float seed, float st, float scale) {
	float r1 = h1(st * 1.91 + seed), r2 = h1(st * 2.53 + seed + 1.7), r3 = h1(st * 3.17 + seed + 4.1);
	float x = u / (scale * 0.35 * (1.0 + r2)) + r3 * 37.0;
	return sin(u / (scale * (1.0 + r1)) + r2 * 6.28) * 0.55 + sin(u / (scale * 0.37 * (1.0 + r3)) + r1 * 6.28) * 0.3
		+ vnoise(x + vnoise(x * 0.31, seed + 5.3) * 1.5, seed) * 0.14 + vnoise(x * 2.7 + vnoise(x * 0.9, seed + 9.1) * 1.2, seed + 3.3) * 0.07;
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float plane = floor(uv.x);
	float k = length(gl_TextureMatrix[7][0].xyz);
	// ACS stretches the upright axis past the length's scale: 0.05 a stage, plus 0.04 x the far spread over the length.
	float v = length(gl_TextureMatrix[7][1].xyz) / k - 1.0;
	float stage = floor(v / 0.05 + 0.001);
	float spread = clamp((v - stage * 0.05) / 0.04, 0.0, 1.0);
	float prog = stage < 1.5 ? 0.0 : 0.66 + (stage - 2.0) * 0.34;
	float laser = step(0.5, stage);
	float widen = max(0.0, 1.0 - 0.33 * stage);
	float st = floor(timer * RATE);
	float len = LEN * k, wide = HALF * k;
	float u = fract(uv.x) * len;
	float w = (uv.y - 0.5) * 2.0 * wide;
	float seed = h1(dot(gl_TextureMatrix[7][3].xyz, vec3(0.131, 0.071, 0.057)) + plane * 3.7) * 61.0;
	// The laser leaves the hands thin and opens to its full width over the first stretch.
	float R = mix(1.0, CORE, prog * prog) * mix(0.25, 1.0, smoothstep(0.0, 160.0, u));
	float c = w / R;
	vec3 col = vec3(0.0);

	// How face on this plane is to the eye. Its wide glow fades edge on, so from behind the beam the planes do not fan
	// into blades; the round tube there is the glow sprites.
	float pa = plane * 1.0471976;
	vec3 nw = normalize(mat3(gl_TextureMatrix[7]) * vec3(0.0, -cos(pa), sin(pa)));
	float wideW = smoothstep(0.12, 0.5, abs(dot(normalize(camerapos - pixelpos.xyz), nw)));

	// The branches: wild and wide with no intensity, reaching at random toward the edge of the damage cone (half the far
	// spread at the hands, all of it at the end, less each stack); fewer and shorter with the first stack, gone after.
	float count = stage < 0.5 ? 14.0 : (stage < 1.5 ? 8.0 : 0.0);
	float wild = stage < 0.5 ? 1.0 : 0.6;
	float hot = 0.0, glow = 0.0;
	for(int i = 0; i < 14; ++i) {
		float b = float(i);
		float bu = len * (stage < 0.5 ? 0.02 + 0.3 * h1(b * 5.31 + st * 1.37 + seed) : 0.08 + 0.45 * h1(b * 5.31 + st * 1.37 + seed));
		float de = min(len, bu + len * (0.3 + 0.6 * h1(b * 7.7 + st + seed)));
		float bl = max(de - bu, 1.0);
		float reach = (h1(b * 3.3 + st * 0.5 + seed) < 0.5 ? -1.0 : 1.0) * mix(0.35, 1.0, h1(b * 9.1 + st * 2.2 + seed)) * wild
			* spread * LEN * k * (0.5 + 0.5 * widen * de / len);
		float t = (u - bu) / bl;
		if(b < count && t > 0.0 && t < 1.0) {
			float be = abs(w - reach * t - wave(u, seed + b * 7.1, st, 16.0) * mix(6.0, 11.0, wild) * sqrt(t));
			float fade = sqrt(1.0 - t) * (0.75 + 0.25 * sin(u * 0.09 + b * 2.0 - timer * 14.0));
			hot = max(hot, exp(-be * be * 3.0) * fade);
			glow = max(glow, exp(-be * be * 0.25) * fade);
		}
	}
	col += vec3(0.9, 0.92, 1.0) * hot + vec3(0.4, 0.45, 1.0) * glow * 0.45;

	// The core: a bright band widening with intensity, streaked with plasma flowing along it, and a hot middle line.
	float flow = 1.4 * sin(u * 0.03 - timer * 8.0) + 0.7 * sin(u * 0.081 + timer * 5.0);
	float streak = pow(0.5 + 0.5 * sin(c * 9.0 + flow), 4.0) + pow(0.5 + 0.5 * sin(c * 5.0 - flow * 0.8 + 2.0), 6.0);
	float band = exp(-c * c * 1.6);
	col += vec3(0.88, 0.94, 1.0) * (band * (0.5 + 0.5 * streak * smoothstep(0.2, 0.8, prog)) * wideW + exp(-c * c * 12.0) * 0.6) * min(mix(0.75, 1.0, prog), 1.1) * laser;
	col += vec3(0.35, 0.5, 1.0) * exp(-c * c * 0.35) * (0.15 + 0.3 * min(prog, 1.0)) * wideW * laser;

	// Tangled bolts weaving along the core, at their densest a stack before the top.
	float tw = smoothstep(0.15, 0.55, prog) * (1.0 - 0.55 * smoothstep(0.75, 1.0, prog));
	for(int i = 0; i < 6; ++i) {
		float b = float(i);
		float path = R * (0.7 + 0.9 * h1(b * 3.9 + seed)) * sin(u / (18.0 + 22.0 * h1(b * 2.3 + seed)) + b * 1.9 + timer * (1.5 + b * 0.4))
			+ wave(u, seed + b * 13.0, st, 10.0) * R * 0.45;
		float e = abs(w - path);
		float on = 0.55 + 0.45 * step(0.25, h1(b * 6.1 + st * 0.83 + seed));
		col += (vec3(0.92, 0.95, 1.0) * exp(-e * e * 2.5) + vec3(0.35, 0.45, 1.0) * exp(-e * e * 0.3) * 0.35) * tw * on;
	}

	// Curling tendrils round the full laser, in pieces that come and go along it.
	float lw = smoothstep(0.6, 1.0, prog);
	for(int i = 0; i < 4; ++i) {
		float b = float(i);
		float y = (mod(b, 2.0) < 0.5 ? 1.0 : -1.0) * R * (1.25 + 0.5 * sin(u * 0.013 * (1.0 + 0.4 * b) + b * 2.1 + timer * 1.6))
			+ R * 0.35 * sin(u * 0.05 * (1.0 + 0.3 * b) - timer * 3.0 + b);
		float e = abs(w - y);
		float seg = smoothstep(0.2, 0.6, 0.5 + 0.5 * sin(u * 0.008 * (1.0 + b * 0.5) + b * 1.7 + timer * 0.9));
		col += (vec3(0.5, 0.55, 1.0) * exp(-e * e * 1.2) * 0.7 + vec3(0.5, 0.4, 1.0) * exp(-e * e * 0.12) * 0.15) * seg * lw;
	}

	// Sparks flicking round the band from the middle stacks on.
	vec2 g = vec2(u, w) / 7.0;
	vec2 f = fract(g) - 0.5;
	float spark = exp(-dot(f, f) * 60.0) * step(0.93, h1(floor(g.x) * 13.1 + floor(g.y) * 71.7 + st * 0.71 + seed));
	col += vec3(0.6, 0.75, 1.0) * spark * smoothstep(0.3, 0.7, prog) * (1.0 - smoothstep(R * 1.5, R * 5.0 + 24.0, abs(w))) * 0.8 * wideW;

	// A soft haze round it all.
	col += vec3(0.3, 0.3, 0.9) * exp(-c * c * 0.06) * 0.12 * min(prog, 1.0) * wideW;

	col *= smoothstep(0.0, 48.0, len - u) * mix(1.0, smoothstep(0.0, 96.0, u), laser) * (0.9 + 0.2 * h1(st * 1.37 + seed)) * (1.0 - smoothstep(0.85, 1.0, abs(uv.y - 0.5) * 2.0));
	return vec4(col, color.a);
}
