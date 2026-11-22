// Spectral Blade: a spectral sword, white edges round a cyan blade, crossguard and grip, energy running to the point,
// a streaked sheet behind. UVs: s hilt 0 to point 1; t across 0..1 on the blade, 1..2 back along the sheet. Additive.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define W_HALF  7.0      // the blade strip's half width in map units, as SBLADE.md3 is built
#define BLADE_L 56.0     // the blade's length in map units, as SBLADE.md3 is built
#define GUARD   0.175    // where the crossguard sits along it; the grip runs from 0 to here
#define BLADE_W 1.6      // the blade's half width at the guard, and its glow's
#define GLOW_W  3.6
#define GUARD_W 5.0      // the crossguard's half length
#define PULSE_HZ 5.0     // how fast the energy runs along the blade
#define NEAR0   16.0     // map units from the eye where a blade starts to show, so a fresh cast stays clear
#define NEAR1   48.0

float sb_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Smooth value noise, 0 to 1: the eight corners in one loop, one hash call site.
float sb_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float v = 0.0;
	for(int c = 0; c < 8; c++) {
		vec3 o = vec3(mod(float(c), 2.0), mod(floor(float(c) * 0.5), 2.0), floor(float(c) * 0.25));
		vec3 w = mix(1.0 - f, f, o);
		v += w.x * w.y * w.z * sb_hash(i + o);
	}
	return v;
}

vec4 Process(vec4 color) {
	vec3 we = camerapos - gl_TextureMatrix[7][3].xyz;
	float fade = smoothstep(NEAR0, NEAR1, length(we));
	if(fade <= 0.0)
		discard;
	float s = clamp(gl_TexCoord[0].s, 0.0, 1.0);
	float tt = gl_TexCoord[0].t;
	float t = mod(timer, 512.0);
	float a = color.a;

	// The blade's profile: narrowing a little from the guard, then to a point over its last fifth; nothing behind the guard.
	float bu = clamp((s - GUARD) / (1.0 - GUARD), 0.0, 1.0);
	float prof = (1.0 - 0.3 * bu) * clamp((1.0 - bu) / 0.2, 0.0, 1.0) * step(GUARD, s);
	float n = sb_noise(vec3(s * 9.0 - t * PULSE_HZ, 0.5, 3.0));
	vec3 col;
	if(tt < 1.0) {
		// Thinning as the actor fades, so a spent sword ends as a fine line.
		float vs = (tt * 2.0 - 1.0) * W_HALF;
		float v = abs(vs);
		float bw = max(BLADE_W * prof * a, 0.02);
		// the leading (forward, t over 0.5) edge cuts and burns bright; the spine behind it is dimmer
		float edge = exp(-(v - bw) * (v - bw) / 0.1) * step(0.001, prof) * mix(0.45, 1.25, step(0.0, vs));
		float fill = 1.0 - smoothstep(bw - 0.3, bw + 0.3, v);
		float fuller = exp(-v * v / 0.06) * prof;
		float glow = exp(-v * v / max(GLOW_W * GLOW_W * prof * prof * (0.3 + 0.7 * a), 0.01));
		float du = (s - GUARD) * BLADE_L;
		float guard = exp(-du * du / 0.45) * (1.0 - smoothstep(GUARD_W - 1.0, GUARD_W, v)) * a;
		float grip = exp(-v * v / 0.35) * (1.0 - step(GUARD, s)) * 0.8 * a;
		float dp = (s - 0.03) * BLADE_L;
		float pommel = exp(-(dp * dp + v * v) / 1.1) * a;
		float haze = exp(-v * v / (W_HALF * W_HALF * 0.35)) * (prof + 0.4 * (1.0 - step(GUARD, s)));
		col = vec3(1.0, 1.0, 1.0) * (edge * 1.1 + fuller * 0.5 + guard * 1.2 + pommel * 0.9)
			+ vec3(0.35, 0.8, 1.0) * (fill * 0.55 + grip) * (0.7 + 0.5 * n)
			+ vec3(0.3, 0.7, 1.0) * glow * (0.45 + 0.55 * n)
			+ vec3(0.15, 0.3, 0.9) * haze * 0.22;
	}
	else {
		// The trailing sheet: motion streaks fading back from the blade, none behind the grip.
		float b = clamp(tt - 1.0, 0.0, 1.0);
		float st = sb_noise(vec3(s * 26.0, b * 1.5 + t * 3.0, 7.0));
		col = vec3(0.3, 0.6, 1.0) * (0.3 + 0.7 * smoothstep(0.35, 0.8, st)) * (1.0 - b) * (1.0 - b) * prof * 1.15;
	}
	return vec4(min(col, vec3(1.0)) * fade * a, 1.0);
}
