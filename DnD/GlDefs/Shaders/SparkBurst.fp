// Spark's burst: white-blue filaments with violet edges that flicker, forks that blink, a white core in a violet haze.

uniform float timer;

#define RATE   18.0    // flicker deals a second
#define CORE   14.0    // how thin a filament's hot middle is
#define HAZE   0.45

float h1(float n) { return fract(sin(n) * 43758.5453); }

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float id = floor(uv.x);
	float along = fract(uv.x);
	float st = mod(floor(timer * RATE), 997.0);
	float h = h1(id * 17.13 + st * 3.71);
	vec3 col;

	if(id > 99.5) {
		vec2 q = vec2(along, uv.y) * 2.0 - 1.0;
		float r = length(q);
		if(id < 100.5)
			col = vec3(1.0, 0.97, 1.0) * exp(-r * r * 9.0) * 1.4 + vec3(0.65, 0.6, 1.0) * exp(-r * r * 3.0) * 0.35;
		else
			col = vec3(0.32, 0.26, 0.9) * exp(-r * r * 3.5) * HAZE;
		col *= 1.0 - smoothstep(0.9, 1.0, r);
	}
	else {
		float d = abs(uv.y - 0.5) * 2.0;
		float on = id < 31.5 ? 0.7 + 0.6 * h : step(0.3, h) * (0.8 + 0.5 * h1(id + st * 1.3));
		col = (vec3(0.85, 0.9, 1.0) * exp(-d * d * CORE) + vec3(0.45, 0.4, 1.0) * exp(-d * d * 3.0) * 0.35) * on * (1.0 - along * 0.55);
	}
	return vec4(col, color.a);
}
