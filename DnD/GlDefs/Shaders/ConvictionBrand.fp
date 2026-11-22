// Conviction's brand over a marked enemy, sprite CVMBA0. Masks: R the brand's lines, G its glow, B a circle of ticks.
// The glow pulses and the tick circle turns; drawn Add, faded through the actor's alpha.

uniform float timer;

#define SPIN    0.25   // turns a second, the tick circle
#define PULSE   0.9    // glow pulses a second
#define TAU     6.2831853

vec4 Process(vec4 color) {
	vec2 st = gl_TexCoord[0].st;
	vec2 p = st - 0.5;
	float t = mod(timer, 512.0);
	float a = TAU * fract(t * SPIN);
	vec2 q = vec2(p.x * cos(a) - p.y * sin(a), p.x * sin(a) + p.y * cos(a));
	vec4 m = getTexel(st);
	// the turned layer stops at the sprite's circle: past it Vulkan wraps the texture round
	float ring = getTexel(q + 0.5).b * (1.0 - smoothstep(0.46, 0.49, length(p)));
	float pulse = 0.75 + 0.25 * sin(TAU * fract(t * PULSE));
	vec3 col = vec3(1.0, 0.86, 0.56) * m.r * 1.35 + vec3(0.62, 0.26, 1.0) * m.g * pulse * 1.2 + vec3(0.78, 0.55, 1.0) * ring;
	return vec4(min(col, vec3(1.0)) * color.rgb, color.a);
}
