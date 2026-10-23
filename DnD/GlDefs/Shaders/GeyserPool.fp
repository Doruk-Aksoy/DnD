// Geyser's acid, bound to GYPOOL (R slow noise, G distance to a bubble cell's middle, B that cell's roll): thick acid
// drifting two ways at once, bubbles swelling and popping in it. Unlit: it glows the same in the dark.

#ifdef NO_FOG
varying vec4 pixelpos;
#endif

uniform float timer;

#define TILE    22.0    // md3 units one repeat of the slow noise covers
#define BUBBLE  9.0     // and of the bubble cells
#define POOLR   8.4     // the pool's radius
#define RATE    0.55    // a bubble's swell and pop a second
#define GAIN    1.25

vec4 Process(vec4 color) {
	vec3 c0 = gl_TextureMatrix[7][0].xyz, c1 = gl_TextureMatrix[7][1].xyz, c2 = gl_TextureMatrix[7][2].xyz;
	vec3 x12 = cross(c1, c2), x20 = cross(c2, c0), x01 = cross(c0, c1);
	vec3 w = pixelpos.xyz - gl_TextureMatrix[7][3].xyz;
	vec2 q = vec2(dot(x12, w), dot(x01, w)) / dot(c0, x12);
	float r = clamp(length(q) / POOLR, 0.0, 1.0);

	// Two layers drifting different ways: the acid crawls, it does not spin.
	float body = getTexel(q / TILE + vec2(timer * 0.03, timer * 0.017)).r * 0.6
		+ getTexel(q / (TILE * 0.55) + vec2(0.37 - timer * 0.021, 0.61 + timer * 0.033)).r * 0.4;

	// Bubbles: a third of the cells, each swelling over its own beat, its skin bright, then popping.
	vec4 c = getTexel(q / BUBBLE + vec2(0.11, 0.53 + timer * 0.012));
	float ph = fract(timer * RATE + c.b * 7.0);
	float rad = 0.2 + 0.55 * ph;
	float skin = smoothstep(rad - 0.16, rad - 0.04, c.g) * (1.0 - smoothstep(rad - 0.04, rad, c.g));
	float inside = 1.0 - smoothstep(rad - 0.05, rad, c.g);
	float live = step(0.66, c.b) * (1.0 - smoothstep(0.86, 1.0, ph));

	vec3 deep = vec3(0.07, 0.36, 0.03), mid = vec3(0.38, 0.95, 0.09), hot = vec3(0.85, 1.0, 0.38);
	vec3 col = mix(deep, mid, smoothstep(0.3, 0.72, body));
	col = mix(col, col * 0.75 + hot * 0.25, inside * live);
	col = mix(col, hot, skin * live * 0.85);
	// Brightest where it wells up in the middle, dimmer where it meets the rock.
	col *= (0.72 + 0.4 * (1.0 - r * r)) * GAIN;
	return vec4(col, color.a);
}
