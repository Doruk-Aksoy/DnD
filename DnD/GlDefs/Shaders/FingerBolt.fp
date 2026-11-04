// Finger of Death: a crackling red lightning bolt along FORKBEAM.md3's stretched quads, re-dealt every tic with sharp kinks
// and forks tapering to points; the actor's alpha strobes it in and out.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define LEN     64.0    // FORKBEAM.md3's length and half width, in model units
#define HALF    24.0
#define RATE    35.0    // fine kinks re-dealt a second (one a tic), the coarse ones every third
#define SEG     16.0    // world units between the fine kinks and how far they stray
#define JAG     4.5
#define SEG2    80.0    // the coarse kinks under them
#define BEND    14.0
#define CORE    0.75    // world half widths: the white-hot core, the red body, the dark glow
#define BODY    2.4
#define HAZE    9.0
#define FORKS   0.32    // share of kinks that throw a fork, and its length range
#define FORK_L0 10.0
#define FORK_L1 30.0
#define STRAND  0.45    // the faint second strand crackling beside the first
#define EDGE0   0.12    // a plane fades out as it turns edge-on: gone at this facing, whole at EDGE1
#define EDGE1   0.45

// Rule-1 portable hash: wrapped to 256, no sin.
float fd_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Distance from p to the segment a-b.
float fd_seg(vec2 p, vec2 a, vec2 b) {
	vec2 ab = b - a;
	float h = clamp(dot(p - a, ab) / max(dot(ab, ab), 1e-4), 0.0, 1.0);
	return length(p - a - ab * h);
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float plane = floor(uv.x);
	float k = length(gl_TextureMatrix[7][0].xyz);
	float len = LEN * k;
	float wide = HALF * k;
	float u = fract(uv.x) * len;
	float w = (uv.y - 0.5) * 2.0 * wide;
	// Each bolt its own: its origin and heading (a fan shares the origin), and each crossed plane its own.
	vec3 m0 = gl_TextureMatrix[7][0].xyz / max(k, 1e-4);
	float seed = floor(fd_hash(floor(gl_TextureMatrix[7][3].xyz) + floor(m0 * 37.0)) * 200.0) + plane * 37.0;
	float t = mod(timer, 512.0);
	float st = floor(t * RATE);
	float st2 = floor(st / 3.0);
	float bend = min(BEND, len * 0.06);
	float jag = min(JAG, len * 0.05);

	float core = 0.0;
	float body = 0.0;
	float haze = 0.0;
	float i0 = floor(u / SEG);
	// Pass 0 is the bolt, 1 the faint strand, 2 to 4 the forks off the last three kinks; one call site for the kinks.
	for(int j = 0; j < 5; j++) {
		float sd = j == 1 ? seed + 11.0 : seed;
		float ki = j < 2 ? i0 : i0 - float(j - 2);
		float fr = fd_hash(vec3(ki, st, sd + 3.0));
		if(j >= 2 && (fr > FORKS || ki < 1.0))
			continue;
		// The bolt's offset at uu: coarse kinks plus fine ones, both straight between, pinned at both ends.
		float uu = j < 2 ? u : ki * SEG;
		float c = floor(uu / SEG2);
		float cf = uu / SEG2 - c;
		float c0 = fd_hash(vec3(c, st2, sd + 1.0)) * 2.0 - 1.0;
		float c1 = fd_hash(vec3(c + 1.0, st2, sd + 1.0)) * 2.0 - 1.0;
		float f = floor(uu / SEG);
		float ff = uu / SEG - f;
		float f0 = fd_hash(vec3(f, st, sd + 2.0)) * 2.0 - 1.0;
		float f1 = fd_hash(vec3(f + 1.0, st, sd + 2.0)) * 2.0 - 1.0;
		float env = smoothstep(0.0, 40.0, uu) * smoothstep(0.0, 24.0, len - uu);
		float off = (mix(c0, c1, cf) * bend + mix(f0, f1, ff) * jag) * env;
		float e;
		float taper = 1.0;
		if(j < 2) {
			float slope = ((c1 - c0) * bend / SEG2 + (f1 - f0) * jag / SEG) * env;
			e = abs(w - off) / sqrt(1.0 + slope * slope);
		}
		else {
			// A fork: off the kink at a slant, one kink halfway, thinning to a point at its end.
			float fl = mix(FORK_L0, FORK_L1, fd_hash(vec3(ki, st, sd + 4.0))) * min(1.0, wide / 30.0);
			float sl = (fr < FORKS * 0.5 ? -1.0 : 1.0) * (0.45 + 0.9 * fd_hash(vec3(ki, st, sd + 5.0)));
			vec2 a = vec2(uu, off);
			vec2 b = a + vec2(fl * 0.5, sl * fl * 0.5);
			vec2 q = b + vec2(fl * 0.5, (sl * 0.35 + fd_hash(vec3(ki, st, sd + 6.0)) - 0.5) * fl * 0.5);
			vec2 p = vec2(u, w);
			e = min(fd_seg(p, a, b), fd_seg(p, b, q));
			taper = 1.0 - clamp(dot(p - a, q - a) / dot(q - a, q - a), 0.0, 1.0);
			e /= max(taper, 0.05);
		}
		float gain = j == 1 ? STRAND : (j >= 2 ? 0.8 * taper : 1.0);
		core = max(core, exp(-e * e / (CORE * CORE)) * gain);
		body = max(body, exp(-e * e / (BODY * BODY)) * gain);
		haze = max(haze, exp(-e * e / (HAZE * HAZE)) * gain);
	}

	// Crackle: a strobe dealt per tic, now and then nearly out.
	float fk = fd_hash(vec3(st, 7.0, seed));
	float strobe = fk < 0.12 ? 0.2 : 0.7 + 0.5 * fk;
	vec3 col = vec3(1.0, 0.86, 0.8) * core + vec3(1.0, 0.1, 0.04) * body * 0.85 + vec3(0.45, 0.0, 0.02) * haze * 0.5;
	// Facing across the bolt only, so looking down its length still shows whichever plane turns toward the eye.
	vec3 ax = gl_TextureMatrix[7][0].xyz / max(k, 1e-4);
	vec3 v = pixelpos.xyz - camerapos;
	v -= ax * dot(v, ax);
	vec3 nrm = normalize(plane < 0.5 ? gl_TextureMatrix[7][1].xyz : gl_TextureMatrix[7][2].xyz);
	float face = smoothstep(EDGE0, EDGE1, abs(dot(nrm, v)) / max(length(v), 1e-4));
	col *= face * strobe * smoothstep(2.0, 20.0, u) * (1.0 - smoothstep(0.8, 1.0, abs(uv.y - 0.5) * 2.0));
	return vec4(col, color.a);
}
