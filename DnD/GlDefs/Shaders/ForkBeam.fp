// A wildly curving bolt with little branches along a stretched beam, reshuffled many times a second, pinned at both ends.

uniform float timer;

#define LEN    64.0    // the model's length and half width, in model units
#define HALF   24.0
#define RATE   18.0    // reshuffles a second
#define BEND   12.0    // world units the big curves may stray
#define JAG    1.8     // world units the kinks may stray
#define SEG    16.0    // world units between kinks
#define CORE   1.1     // world half width of the white core's soft falloff

float h1(float n) { return fract(sin(n) * 43758.5453); }
float tri(float x) { return abs(fract(x) - 0.5) * 4.0 - 1.0; }

// Sideways offset of the bolt at u: waves with phases dealt per reshuffle, so the line cannot tear.
float boltAt(float u, float len, float wide, float seed, float st) {
	float r1 = h1(st * 1.91 + seed), r2 = h1(st * 2.53 + seed + 1.7), r3 = h1(st * 3.17 + seed + 4.1), r4 = h1(st * 0.73 + seed + 9.3);
	float bend = sin(u / (14.0 + 14.0 * r1) + r2 * 6.28) * 0.6 + sin(u / (6.0 + 4.0 * r3) + r4 * 6.28) * 0.4;
	float jag = tri(u / (SEG * (1.6 + 0.8 * r1)) + r2) * 0.6 + sin(u / (SEG * 0.3) + r3 * 6.28) * 0.4;
	return (bend * min(BEND, wide * 0.3) * smoothstep(0.0, 64.0, u) + jag * min(JAG, wide * 0.15) * smoothstep(0.0, 24.0, u)) * smoothstep(0.0, 24.0, len - u);
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float plane = floor(uv.x);
	float k = length(gl_TextureMatrix[7][0].xyz);
	float len = LEN * k, wide = HALF * k;
	float u = fract(uv.x) * len;
	float w = (uv.y - 0.5) * 2.0 * wide;
	float seed = h1(dot(gl_TextureMatrix[7][3].xyz, vec3(0.131, 0.071, 0.057)) + plane * 3.7) * 61.0;
	float st = floor(timer * RATE);

	float e = abs(w - boltAt(u, len, wide, seed, st));
	float core = exp(-e * e / (CORE * CORE) * 1.5);
	float glow = exp(-e * e * 0.15);

	// Little branches: each leaves the bolt somewhere, runs off to one side with a kink or two, and thins out.
	for(int i = 0; i < 8; ++i) {
		float b = float(i);
		float r = h1(b * 5.31 + st * 1.37 + seed);
		float bu = len * (0.12 + 0.76 * r);
		float bl = (10.0 + 18.0 * h1(b * 7.7 + st + seed)) * min(1.0, wide / 30.0);
		float slope = (h1(b * 3.3 + st * 0.5 + seed) < 0.5 ? -1.0 : 1.0) * (0.5 + 0.9 * h1(b * 9.1 + st * 2.2 + seed));
		float t = (u - bu) / bl;
		if(t > 0.0 && t < 1.0) {
			float be = abs(w - boltAt(bu, len, wide, seed, st) - slope * (u - bu) - tri(t * 2.5 + r) * 1.2 * t) / sqrt(1.0 + slope * slope);
			float fade = step(0.2, h1(b * 2.17 + st * 0.91 + seed)) * sqrt(1.0 - t);
			core = max(core, exp(-be * be * 4.0) * fade);
			glow = max(glow, exp(-be * be * 0.3) * fade * 0.75);
		}
	}

	vec3 col = vec3(0.85, 0.9, 1.0) * core + vec3(0.35, 0.45, 1.0) * glow * 0.6;
	col *= smoothstep(4.0, 28.0, u) * (0.85 + 0.3 * h1(st * 1.37 + seed)) * (0.8 + 0.2 * sin(u * 0.07 - timer * 16.0)) * (1.0 - smoothstep(0.8, 1.0, abs(uv.y - 0.5) * 2.0));
	return vec4(col, color.a);
}
