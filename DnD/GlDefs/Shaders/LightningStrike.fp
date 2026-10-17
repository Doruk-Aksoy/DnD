// A falling bolt: a white core in a violet-blue glow, faint branches that blink, a soft haze, the top fading out.

uniform float timer;

#define RATE    24.0    // branch blinks dealt a second
#define CORE    0.2     // the main channel's solid white middle, as a share of its half width
#define TWIG    20.0    // the same for branches and twigs
#define TOPFADE 0.35    // share of the height, from the top, the bolt fades in over
#define HAZE    0.28

float h1(float n) { return fract(sin(n) * 43758.5453); }

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float id = floor(uv.x);
	float along = fract(uv.x);
	float d = abs(uv.y - 0.5) * 2.0;
	float seed = h1(dot(gl_TextureMatrix[7][3].xyz, vec3(0.131, 0.071, 0.057)));
	float st = mod(floor(timer * RATE + seed * 7.0), 997.0);
	float top = smoothstep(0.0, TOPFADE, along);
	vec3 col;

	if(id < 0.5) {
		col = vec3(1.0, 1.0, 1.0) * (1.0 - smoothstep(CORE * 0.55, CORE, d)) * 1.3
			+ vec3(0.42, 0.4, 1.0) * exp(-d * d * 4.5) * 0.75
			+ vec3(0.25, 0.2, 0.85) * exp(-d * d * 1.2) * 0.45;
		col *= (0.88 + 0.24 * h1(st * 1.37 + seed * 11.0)) * top;
	}
	else if(id < 17.5) {
		float on = step(0.2, h1(id * 13.7 + st * 2.9 + seed * 5.0)) * (0.8 + 0.6 * h1(id * 3.1 + seed));
		if(id > 9.5)
			on *= 0.65;
		col = (vec3(0.85, 0.88, 1.0) * exp(-d * d * TWIG) + vec3(0.45, 0.4, 1.0) * exp(-d * d * 3.0) * 0.35) * on * (1.0 - 0.6 * along);
	}
	else
		col = vec3(0.3, 0.25, 0.85) * exp(-d * d * 2.5) * HAZE * top;
	return vec4(col * (1.0 - smoothstep(0.7, 1.0, d)), color.a);
}
