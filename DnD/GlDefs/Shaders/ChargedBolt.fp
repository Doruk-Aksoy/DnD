// Charged Bolt: a crackling ball on a camera-facing sprite. Arms are strips of LIGBOLT, re-dealt many times a second.

uniform float timer;

#define ARMS   5
#define ROWS   10.0
#define ROWH   0.09375  // one arm strip's height in the frame
#define BAND   0.03125  // the seed band above the strips
#define RATE   17.5     // shapes a second
#define ARM_W  0.12     // half an arm's width, in the sprite's half widths
#define CORE   0.10
#define HALO   0.40

float h1(float n) { return fract(sin(n) * 43758.5453); }

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st * 2.0 - 1.0;
	float r = length(p);

	// The frame's seed band, so bolts on different frames take different shapes on the same tic.
	float seed = floor(getTexel(vec2(0.5, 0.5 * BAND)).r * 7.97);
	float k0 = mod(floor(timer * RATE + seed * 0.37), 997.0) * 7.31 + seed * 131.7;
	float tc = mod(timer, 64.0);

	float rot = h1(k0) * 6.2832;
	vec3 acc = vec3(0.0);
	for(int i = 0; i < ARMS; i++) {
		float fi = float(i);
		float on = i < 2 ? 1.0 : step(h1(k0 + fi * 3.7 + 1.1), 0.72);
		float ang = rot + (fi + (h1(k0 + fi * 5.3 + 2.2) - 0.5) * 0.8) * 1.2566;
		float len = 0.38 + 0.57 * h1(k0 + fi * 7.9 + 3.3);
		float row = floor(h1(k0 + fi * 11.3 + 4.4) * ROWS);
		float span = 0.5 + 0.5 * h1(k0 + fi * 13.1 + 5.5);
		float flip = h1(k0 + fi * 17.7 + 6.6) < 0.5 ? -1.0 : 1.0;

		vec2 d = vec2(cos(ang), sin(ang));
		float a = dot(p, d);
		float b = (d.x * p.y - d.y * p.x) * flip + sin(a * 21.0 + tc * 29.0 + fi * 1.7) * 0.018 * a;
		if(a > 0.0 && a < len && abs(b) < ARM_W) {
			float t = BAND + (row + 0.5 + 0.5 * b / ARM_W) * ROWH;
			float tip = 1.0 - smoothstep(0.6 * len, len, a);
			acc += getTexel(vec2(a / len * span, t)).rgb * tip * on;
		}
	}

	float lev = 0.8 + 0.4 * h1(k0 + 9.9);
	float core = exp(-r * r / (CORE * CORE));
	float halo = exp(-r * r / (HALO * HALO));
	vec3 col = acc * 1.6 * lev + vec3(0.80, 0.90, 1.00) * core * 1.2 * lev + vec3(0.30, 0.42, 1.00) * halo * 0.32;
	return vec4(col * (1.0 - smoothstep(0.85, 1.0, r)), color.a);
}
