// Contagion's rune circle on flat2.md3, bound to CONTAGRN (masks: R rings and the star, G the runes, B each rune's roll).
// A dark smoky disc with violet runes turning round a still star; it grows out from the middle as the actor's alpha rises.

uniform float timer;

#define TAU       6.2831853
#define ALPHA_REF 0.1      // the actor's alpha when whole; below it the circle shrinks to the middle and fades
#define SPIN      0.025    // turns a second the rune band turns
#define BREATHE   0.35     // a rune's glow rises and falls this often a second, each on its own phase
#define DARK      0.8      // how far the disc darkens the floor, and its smoke's grain
#define SMOKE     5.0
#define HALO      0.006    // uv reach of the runes' soft glow
#define SOOT      vec3(0.02, 0.0, 0.035)
#define LINE_IN   vec3(0.36, 0.07, 0.78)   // the lines' body, deep violet at the star, magenta-violet at the rim
#define LINE_OUT  vec3(0.62, 0.08, 0.66)
#define LINE_HOT  vec3(0.95, 0.78, 1.0)    // their hot core where the current runs strong
#define LINE_GLOW vec3(0.3, 0.02, 0.48)    // the soft glow round them
#define CURRENT   0.22                     // how fast the slow currents of light drift round the circle
#define RUNE      vec3(0.5, 0.13, 0.9)
#define RUNE_HOT  vec3(0.92, 0.7, 1.0)

// Rule-1 portable hash and value noise for the smoke.
float cg_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float cg_noise(vec3 x) {
	vec3 i = floor(x);
	vec3 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float v = 0.0;
	for(int c = 0; c < 8; c++) {
		vec3 o = vec3(mod(float(c), 2.0), mod(floor(float(c) * 0.5), 2.0), floor(float(c) * 0.25));
		vec3 w = mix(1.0 - f, f, o);
		v += w.x * w.y * w.z * cg_hash(i + o);
	}
	return v;
}

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;
	float fade = clamp(color.a / ALPHA_REF, 0.0, 1.0);
	// It spreads out from the middle as it appears, and draws back in as it goes.
	float reach = smoothstep(r - 0.12, r + 0.04, fade * 1.2);
	if(r > 1.0 || reach <= 0.0)
		discard;
	float t = mod(timer, 512.0);

	float line = getTexel(p + 0.5).r;
	float ta = -TAU * fract(t * SPIN);
	float c = cos(ta), s = sin(ta);
	vec2 q = vec2(p.x * c - p.y * s, p.x * s + p.y * c);
	vec3 rt = getTexel(q + 0.5).rgb;
	// The glows: the runes' mask (turning) and the lines' (still) softened by a ring of taps round each point.
	float halo = 0.0;
	float lhalo = 0.0;
	for(int i = 0; i < 8; i++) {
		float a = float(i) * 0.785398;
		vec2 o = HALO * vec2(cos(a), sin(a));
		halo += getTexel(q + 0.5 + o).g;
		lhalo += getTexel(p + 0.5 + o * 1.6).r;
	}
	halo *= 0.125;
	lhalo *= 0.125;

	float beat = 0.55 + 0.45 * sin(TAU * fract(t * BREATHE + rt.b * 7.0));
	float smoke = cg_noise(vec3(p * SMOKE + vec2(t * 0.05, -t * 0.03), t * 0.08));
	float dark = DARK * (0.75 + 0.35 * smoke) * (1.0 - smoothstep(0.9, 1.0, r));
	// Slow currents of light drifting round the circle, from noise on the circle itself so there is no seam.
	float ang = atan(p.y, p.x);
	float cur = cg_noise(vec3(cos(ang) * 2.5 + 7.0, sin(ang) * 2.5 + 3.0, r * 3.0 + t * CURRENT));
	cur = smoothstep(0.3, 0.8, cur);
	vec3 body = mix(LINE_IN, LINE_OUT, smoothstep(0.6, 0.95, r));
	vec3 lines = mix(body, LINE_HOT, line * line * (0.15 + 0.6 * cur)) * line * (0.6 + 0.6 * cur) + LINE_GLOW * lhalo * (0.5 + 0.7 * cur);
	vec3 emit = lines + mix(RUNE, RUNE_HOT, rt.g * beat * 0.7) * rt.g * beat + RUNE * halo * 0.6 * beat;

	// Premultiplied: the soot darkens what is under it, the light adds on top.
	float A = clamp(dark + max(emit.r, max(emit.g, emit.b)), 0.0, 1.0) * reach * min(1.0, fade * 3.0);
	if(A < 0.01)
		discard;
	vec3 pm = (SOOT * dark + emit) * reach * min(1.0, fade * 3.0);
	return vec4(clamp(pm / A, 0.0, 1.0), A);
}
