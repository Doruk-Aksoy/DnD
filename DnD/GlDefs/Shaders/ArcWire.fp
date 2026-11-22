// Arcane Trap's trip-wire along FORKBEAM.md3's stretched quads: a thin faint violet thread with sparks running both
// ways along it and a slow shimmer. The actor's alpha fades it (rgb: Add cannot fade by alpha).

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define LEN     64.0    // FORKBEAM.md3's length and half width, in model units
#define HALF    24.0
#define EDGE0   0.12    // a plane fades out as it turns edge-on: gone at this facing, whole at EDGE1
#define EDGE1   0.45
#define SPARK_S 34.0    // world units between spark slots
#define SPEED   55.0    // world units a second the sparks run

float aw_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float plane = floor(uv.x);
	float k = length(gl_TextureMatrix[7][0].xyz);
	float len = LEN * k;
	float u = fract(uv.x) * len;
	float w = (uv.y - 0.5) * 2.0 * HALF * k;
	float t = mod(timer, 512.0);
	vec3 ax = gl_TextureMatrix[7][0].xyz / max(k, 1e-4);

	float core = exp(-w * w / 0.18);
	float glow = exp(-w * w / 3.0);
	float shimmer = 0.75 + 0.25 * aw_hash(vec3(floor(u / 6.0), floor(t * 12.0), 4.0));

	// sparks: a few slots each way, each lit or not by its dice
	float sparks = 0.0;
	for(int j = 0; j < 2; j++) {
		float dir = j == 0 ? 1.0 : -1.0;
		float s = (u * dir + t * SPEED) / SPARK_S + float(j) * 0.37;
		float ph = fract(s);
		float on = step(0.55, aw_hash(vec3(floor(s), float(j), 9.0)));
		sparks += on * exp(-(ph - 0.5) * (ph - 0.5) / 0.002) * exp(-w * w / 0.5);
	}

	vec3 col = vec3(0.78, 0.66, 1.0) * core * 0.55 * shimmer + vec3(0.5, 0.3, 1.0) * glow * 0.22 + vec3(0.9, 0.85, 1.0) * sparks;
	vec3 v = pixelpos.xyz - camerapos;
	v -= ax * dot(v, ax);
	vec3 nrm = normalize(plane < 0.5 ? gl_TextureMatrix[7][1].xyz : gl_TextureMatrix[7][2].xyz);
	float face = smoothstep(EDGE0, EDGE1, abs(dot(nrm, v)) / max(length(v), 1e-4));
	col *= face * smoothstep(0.0, 4.0, u) * smoothstep(0.0, 4.0, len - u) * (1.0 - smoothstep(0.8, 1.0, abs(uv.y - 0.5) * 2.0));
	return vec4(min(col, vec3(1.0)) * clamp(color.a, 0.0, 1.0), 1.0);
}
