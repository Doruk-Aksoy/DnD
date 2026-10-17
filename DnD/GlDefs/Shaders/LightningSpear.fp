// Lightning Spear: blue strands that flicker with pulses racing to the tip, branches that blink, a soft glow round them.

uniform float timer;

#define RATE   20.0    // flicker deals a second
#define PULSE  9.0     // pulse cycles a second along a strand
#define CORE   14.0    // how thin a strand's hot middle is
#define HALO   0.38

float h1(float n) { return fract(sin(n) * 43758.5453); }

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float id = floor(uv.x);
	float along = fract(uv.x);
	float d = abs(uv.y - 0.5) * 2.0;
	float st = mod(floor(timer * RATE), 997.0);
	float h = h1(id * 17.13 + st * 3.71);
	float tc = mod(timer, 64.0);
	float lit;

	if(id < 7.5) {
		float pulse = pow(0.5 + 0.5 * sin((along * 3.0 - tc * PULSE + id * 0.37) * 6.2832), 6.0);
		lit = exp(-d * d * CORE) * (0.75 + 0.5 * h) * (0.7 + 1.1 * pulse) * 0.85 + exp(-d * d * 3.0) * 0.42;
	}
	else if(id < 14.5)
		lit = (exp(-d * d * CORE) * 1.3 + exp(-d * d * 3.0) * 0.25) * step(0.4, h) * (1.0 - along * 0.6) * (0.8 + 0.6 * h1(id + st * 1.3));
	else
		lit = exp(-d * d * 4.0) * HALO * (0.85 + 0.3 * h);

	return vec4(vec3(0.30, 0.60, 1.00) * lit, color.a);
}
