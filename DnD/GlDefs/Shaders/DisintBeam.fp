// Disintegrate's ray along FORKBEAM.md3's stretched quads: a white-hot core in a violet sheath, fine arcs crackling
// along it in flickering segments. The actor's alpha is its intensity (0.5 to 1), rank 10's growing bonus: thicker, brighter.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define LEN     64.0    // FORKBEAM.md3's length and half width, in model units
#define HALF    24.0
#define EDGE0   0.12    // a plane fades out as it turns edge-on: gone at this facing, whole at EDGE1
#define EDGE1   0.45

float dg_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float dg_noise(float x, float s) {
	float i = floor(x);
	float f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	return mix(dg_hash(vec3(mod(i, 256.0), s, 2.0)), dg_hash(vec3(mod(i + 1.0, 256.0), s, 2.0)), f);
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float plane = floor(uv.x);
	float k = length(gl_TextureMatrix[7][0].xyz);
	float len = LEN * k;
	float u = fract(uv.x) * len;
	float w = (uv.y - 0.5) * 2.0 * HALF * k;
	float inten = clamp((color.a - 0.5) * 2.0, 0.0, 1.0);
	float t = mod(timer, 512.0);
	vec3 ax = gl_TextureMatrix[7][0].xyz / max(k, 1e-4);

	// narrow at the hands, flaring a little at the far end where it bites
	float taper = smoothstep(0.0, 12.0, u) * (0.85 + 0.25 * smoothstep(len - 20.0, len, u));
	float rad = (0.9 + 1.2 * inten) * taper;
	float core = exp(-w * w / max(rad * rad * 0.6, 0.05));
	float sh = (3.5 + 3.5 * inten) * taper;
	float sheath = exp(-w * w / max(sh * sh, 0.1));

	// crackling arcs: two thin lines wandering round the core in flickering segments, hugging it near the hands
	float arcs = 0.0;
	for(int j = 0; j < 2; j++) {
		float fj = float(j) * 31.0 + plane * 7.0;
		float off = (dg_noise(u / 14.0 + t * 7.0 + fj, 5.0) - 0.5) * 2.0 * sh * 0.9 * smoothstep(8.0, 48.0, u);
		float seg = smoothstep(0.45, 0.6, dg_noise(u / 26.0 - t * 9.0 + fj, 9.0));
		float d = w - off;
		arcs = max(arcs, exp(-d * d / 0.3) * seg);
	}
	float flick = 0.88 + 0.12 * dg_hash(vec3(floor(t * 30.0), plane, 3.0));

	vec3 col = vec3(1.0, 0.97, 1.0) * core * 1.3 + vec3(0.6, 0.3, 1.0) * sheath * (0.55 + 0.35 * inten) +
		vec3(0.85, 0.75, 1.0) * arcs * (0.8 + 0.4 * inten);
	col *= flick;
	vec3 v = pixelpos.xyz - camerapos;
	v -= ax * dot(v, ax);
	vec3 nrm = normalize(plane < 0.5 ? gl_TextureMatrix[7][1].xyz : gl_TextureMatrix[7][2].xyz);
	float face = smoothstep(EDGE0, EDGE1, abs(dot(nrm, v)) / max(length(v), 1e-4));
	col *= face * smoothstep(0.0, 3.0, u) * smoothstep(0.0, 4.0, len - u) * (1.0 - smoothstep(0.8, 1.0, abs(uv.y - 0.5) * 2.0));
	return vec4(min(col, vec3(1.0)), 1.0);
}
