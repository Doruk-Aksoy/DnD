// Purge's tendril along FORKBEAM.md3's stretched quads: three gold strands winding round each other as they stream to
// the target, pinched together at both ends, sparkles riding them; thins as the actor fades.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define LEN     64.0    // FORKBEAM.md3's length and half width, in model units
#define HALF    24.0
#define AMP     7.0     // how far the strands wind out from the line, world units
#define TWIST   120.0   // world units per winding
#define FLOW    2.2     // windings a second streaming toward the target
#define STRAND  0.75    // a strand's half width and its glow's
#define GLOW    3.2
#define SPARK_S 14.0    // world units between sparkle slots, their share that hold one
#define SPARK_P 0.45
#define EDGE0   0.12    // a plane fades out as it turns edge-on: gone at this facing, whole at EDGE1
#define EDGE1   0.45
#define TAU     6.2831853

float pr_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float pr_noise(float x, float s) {
	float i = floor(x);
	float f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	return mix(pr_hash(vec3(i, s, 2.0)), pr_hash(vec3(i + 1.0, s, 2.0)), f);
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
	float seed = floor(pr_hash(floor(gl_TextureMatrix[7][3].xyz) + floor(ax * 37.0)) * 200.0);

	// Pinched at the hands and at the target, widest between; thinner as the actor fades.
	float pinch = smoothstep(0.0, min(64.0, len * 0.3), u) * smoothstep(0.0, min(48.0, len * 0.25), len - u);
	float amp = AMP * pinch * (0.4 + 0.6 * a);
	float sw = 0.35 + 0.65 * a;

	// Each strand is a helix round the line; this plane shows its offset along its own side (sine on one plane,
	// cosine on the other), brighter on the near half of its turn.
	float s = 0.0;
	float g = 0.0;
	for(int j = 0; j < 3; j++) {
		float fj = float(j);
		float ph = TAU * (u / TWIST * (0.85 + 0.3 * pr_hash(vec3(fj, seed, 1.0))) - t * FLOW + fj / 3.0);
		ph += 2.4 * (pr_noise(u / 110.0 + fj * 7.0, seed) - 0.5);
		// each strand swells and narrows on its own, so the windings never repeat evenly
		float sa = 0.55 + 0.7 * pr_noise(u / 80.0 + fj * 13.0 - t * 0.8, seed + 5.0);
		float off = (plane < 0.5 ? sin(ph) : cos(ph)) * amp * sa;
		float near = 0.55 + 0.45 * (plane < 0.5 ? cos(ph) : -sin(ph));
		float d = w - off;
		s = max(s, exp(-d * d / (STRAND * STRAND * sw * sw)) * near);
		g = max(g, exp(-d * d / (GLOW * GLOW * sw)) * near);
	}
	float core = exp(-w * w / (8.0 * (0.3 + 0.7 * a))) * 0.22 * pinch;

	// Sparkles riding the stream.
	float uc = u - t * FLOW * TWIST * 0.8;
	float ci = floor(uc / SPARK_S);
	float sp = 0.0;
	for(int j = -1; j <= 1; j++) {
		float c = ci + float(j);
		float h = pr_hash(vec3(c, seed, 5.0 + plane));
		if(h > SPARK_P)
			continue;
		float mu = (c + 0.2 + 0.6 * pr_hash(vec3(c, seed, 3.0))) * SPARK_S + t * FLOW * TWIST * 0.8;
		float mw = (pr_hash(vec3(c, seed, 7.0)) * 2.0 - 1.0) * (amp + 3.0);
		float du = u - mu;
		float dw = w - mw;
		sp = max(sp, exp(-(du * du + dw * dw) / 0.9) * step(4.0, mu) * step(mu, len - 4.0));
	}

	vec3 col = vec3(1.0, 0.95, 0.78) * s * 1.2 + vec3(1.0, 0.72, 0.22) * (g * 0.55 + core) + vec3(1.0, 0.92, 0.65) * sp;
	vec3 v = pixelpos.xyz - camerapos;
	v -= ax * dot(v, ax);
	vec3 nrm = normalize(plane < 0.5 ? gl_TextureMatrix[7][1].xyz : gl_TextureMatrix[7][2].xyz);
	float face = smoothstep(EDGE0, EDGE1, abs(dot(nrm, v)) / max(length(v), 1e-4));
	col *= face * smoothstep(0.0, 6.0, u) * smoothstep(0.0, 6.0, len - u) * (1.0 - smoothstep(0.8, 1.0, abs(uv.y - 0.5) * 2.0));
	return vec4(min(col, vec3(1.0)) * a, 1.0);
}
