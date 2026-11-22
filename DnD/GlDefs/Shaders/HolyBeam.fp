// Holy Light: a straight beam of white-gold light along FORKBEAM.md3's stretched quads, motes streaming to the target;
// narrow at the hands, it thins to a thread as the actor fades.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define LEN     64.0    // FORKBEAM.md3's length and half width, in model units
#define HALF    24.0
#define CORE    0.9     // world half widths: the white core, the gold body, the warm haze
#define BODY    3.6
#define HAZE    14.0
#define HAZE_IN 128.0   // the haze grows in over this many units from the hands and fades over the last HAZE_OUT
#define HAZE_OUT 32.0
#define NEAR    56.0    // the beam swells to full width over this many units from the hands
#define FLOW    300.0   // units a second the light streams toward the target
#define MOTE_S  16.0    // world units between motes, their stray off the axis, their size
#define MOTE_W  4.0
#define MOTE_R  0.85
#define MOTE_P  0.55    // share of mote slots that hold one
#define EDGE0   0.12    // a plane fades out as it turns edge-on: gone at this facing, whole at EDGE1
#define EDGE1   0.45

float hb_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Smooth 1D value noise along the beam.
float hb_noise(float x, float s) {
	float i = floor(x);
	float f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	return mix(hb_hash(vec3(i, s, 1.0)), hb_hash(vec3(i + 1.0, s, 1.0)), f);
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
	float seed = floor(hb_hash(floor(gl_TextureMatrix[7][3].xyz) + floor(ax * 37.0)) * 200.0) + plane * 37.0;

	// Narrow at the hands and thinning as the actor fades.
	float fw = (0.3 + 0.7 * smoothstep(0.0, NEAR, u)) * (0.3 + 0.7 * a);
	float flow = hb_noise((u - t * FLOW) / 48.0, seed);
	float core = exp(-w * w / (CORE * CORE * fw * fw));
	float body = exp(-w * w / (BODY * BODY * fw * fw)) * (0.75 + 0.5 * flow);
	// The haze gathers along the first stretch and tapers off at the far end, so no flat sheet spreads from the hands.
	float haze = exp(-w * w / (HAZE * HAZE * fw)) * (0.6 + 0.4 * flow) * smoothstep(16.0, HAZE_IN, u) * smoothstep(0.0, HAZE_OUT, len - u);

	// Motes riding the light toward the target, a little behind its flow.
	float uc = u - t * FLOW * 0.6;
	float ci = floor(uc / MOTE_S);
	float m = 0.0;
	for(int j = -1; j <= 1; j++) {
		float c = ci + float(j);
		float h3 = hb_hash(vec3(c, seed, 7.0));
		if(h3 > MOTE_P)
			continue;
		float mu = (c + 0.2 + 0.6 * hb_hash(vec3(c, seed, 3.0))) * MOTE_S + t * FLOW * 0.6;
		float mw = (hb_hash(vec3(c, seed, 5.0)) * 2.0 - 1.0) * MOTE_W * fw;
		float r = MOTE_R * (0.6 + 0.8 * h3 / MOTE_P);
		float du = u - mu;
		float dw = w - mw;
		m = max(m, exp(-(du * du * 0.2 + dw * dw) / (r * r)));
	}
	m *= smoothstep(8.0, 48.0, u);

	// Facing across the beam only; the glow is shared by how squarely each plane faces the eye (the shares sum to one),
	// so a plane seen aslant never shows as a sheet; the line and motes just fade out edge-on.
	vec3 v = pixelpos.xyz - camerapos;
	v -= ax * dot(v, ax);
	vec3 nrm = normalize(plane < 0.5 ? gl_TextureMatrix[7][1].xyz : gl_TextureMatrix[7][2].xyz);
	float f = abs(dot(nrm, v)) / max(length(v), 1e-4);
	float face = smoothstep(EDGE0, EDGE1, f);
	vec3 col = (vec3(1.0, 0.98, 0.9) * core * 1.15 + vec3(1.0, 0.93, 0.7) * m) * face
		+ (vec3(1.0, 0.74, 0.28) * body * 0.75 + vec3(0.85, 0.45, 0.12) * haze * 0.45) * f * f * 1.25;
	col *= smoothstep(0.0, 8.0, u) * smoothstep(0.0, 6.0, len - u) * (1.0 - smoothstep(0.8, 1.0, abs(uv.y - 0.5) * 2.0));
	return vec4(min(col, vec3(1.0)) * a, 1.0);
}
