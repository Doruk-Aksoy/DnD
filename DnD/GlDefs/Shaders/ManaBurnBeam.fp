// Mana Burn's drain along FORKBEAM.md3's stretched quads: a violet tendril writhing from the hands to the target, a
// blue-white core, and pulses of drained mana running back along it to the caster; thins as the actor fades.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define LEN     64.0    // FORKBEAM.md3's length and half width, in model units
#define HALF    24.0
#define AMP     4.5     // how far the tendril writhes from the line, world units
#define WAVE    70.0    // world units per writhe
#define PULSE_S 38.0    // world units between drained pulses
#define BACK    260.0   // world units a second the pulses run back to the caster
#define EDGE0   0.12    // a plane fades out as it turns edge-on: gone at this facing, whole at EDGE1
#define EDGE1   0.45
#define TAU     6.2831853

float mb_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float mb_noise(float x, float s) {
	float i = floor(x);
	float f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	return mix(mb_hash(vec3(i, s, 2.0)), mb_hash(vec3(i + 1.0, s, 2.0)), f);
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float plane = floor(uv.x);
	float k = length(gl_TextureMatrix[7][0].xyz);
	float len = LEN * k;
	float u = fract(uv.x) * len;
	float w = (uv.y - 0.5) * 2.0 * HALF * k;
	float a = color.a;
	float t = mod(timer, 512.0);
	vec3 ax = gl_TextureMatrix[7][0].xyz / max(k, 1e-4);
	float seed = floor(mb_hash(floor(gl_TextureMatrix[7][3].xyz) + floor(ax * 37.0)) * 200.0);

	// pinched at both ends; the writhe is a noise path, different on the two planes
	float pinch = smoothstep(0.0, min(48.0, len * 0.25), u) * smoothstep(0.0, min(40.0, len * 0.2), len - u);
	float off = (mb_noise(u / WAVE * 2.0 + t * 1.3, seed + plane * 9.0) - 0.5) * 2.0 * AMP * pinch;
	float d = w - off;
	float sw = 0.4 + 0.6 * a;
	float core = exp(-d * d / (0.5 * sw));
	float glow = exp(-d * d / (9.0 * sw));
	float haze = exp(-w * w / 60.0) * pinch * 0.18;

	// pulses of mana running back toward the hands
	float up = (u + t * BACK) / PULSE_S;
	float ph = fract(up);
	float on = step(0.35, mb_hash(vec3(floor(up), seed, 4.0)));
	float pulse = on * exp(-(ph - 0.5) * (ph - 0.5) / 0.012) * exp(-d * d / 3.0);

	vec3 col = vec3(0.85, 0.9, 1.0) * core * 1.1 + vec3(0.55, 0.3, 1.0) * glow * 0.8 + vec3(0.4, 0.3, 1.0) * haze +
		vec3(0.6, 0.85, 1.0) * pulse * 1.4;
	vec3 v = pixelpos.xyz - camerapos;
	v -= ax * dot(v, ax);
	vec3 nrm = normalize(plane < 0.5 ? gl_TextureMatrix[7][1].xyz : gl_TextureMatrix[7][2].xyz);
	float face = smoothstep(EDGE0, EDGE1, abs(dot(nrm, v)) / max(length(v), 1e-4));
	col *= face * smoothstep(0.0, 6.0, u) * smoothstep(0.0, 6.0, len - u) * (1.0 - smoothstep(0.8, 1.0, abs(uv.y - 0.5) * 2.0));
	return vec4(min(col, vec3(1.0)) * a, 1.0);
}
