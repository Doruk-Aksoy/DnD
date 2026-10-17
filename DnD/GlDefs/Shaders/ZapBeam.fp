// A thin zigzag of lightning along a stretched beam, re-dealt many times a second, the same thickness at any length.

uniform float timer;

#define LEN    64.0    // the model's length and half width, in model units
#define HALF   10.0
#define RATE   20.0    // zigzags dealt a second
#define SEG    14.0    // world units between kinks
#define AMP    4.0     // world units a kink may stray
#define CORE   0.8     // world half width of the white core

float h1(float n) { return fract(sin(n) * 43758.5453); }
float tri(float x) { return abs(fract(x) - 0.5) * 4.0 - 1.0; }

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float plane = floor(uv.x);
	float k = length(gl_TextureMatrix[7][0].xyz);
	float len = LEN * k, wide = HALF * k;
	float u = fract(uv.x) * len;
	float w = (uv.y - 0.5) * 2.0 * wide;
	float seed = h1(dot(gl_TextureMatrix[7][3].xyz, vec3(0.131, 0.071, 0.057)) + plane * 3.7) * 61.0;
	float st = floor(timer * RATE);

	// Waves with phases dealt per flicker, so the line cannot tear: hashing each kink apart breaks it where GPU rounding differs.
	float r1 = h1(st * 1.91 + seed), r2 = h1(st * 2.53 + seed + 1.7), r3 = h1(st * 3.17 + seed + 4.1);
	float off = sin(u / (SEG * (0.5 + 0.25 * r1)) + r2 * 6.28) * 0.55 + sin(u / (SEG * 0.23) + r3 * 6.28) * 0.25 + tri(u / (SEG * 0.74) + r3) * 0.2;
	float ends = smoothstep(0.0, 40.0, u) * smoothstep(0.0, 10.0, len - u);
	float e = abs(w - off * min(AMP, wide * 0.45) * ends);

	vec3 col = vec3(0.75, 0.88, 1.0) * exp(-e * e / (CORE * CORE) * 1.5) + vec3(0.25, 0.5, 1.0) * exp(-e * e * 0.2) * 0.55;
	col *= smoothstep(4.0, 32.0, u) * (0.8 + 0.4 * h1(st * 1.37 + seed)) * (1.0 - smoothstep(0.75, 1.0, abs(uv.y - 0.5) * 2.0));
	return vec4(col, color.a);
}
