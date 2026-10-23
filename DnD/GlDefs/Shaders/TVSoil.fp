// Threaded Volley's energized soil. Bound to TVSOIL in GlDefs/Shaders.txt.
// The skin's alpha is the cutout and the crack mask: 0 outside the disc, 0.56 on solid soil, up to 1 in a crack.

uniform float timer;

#define PERIOD   4.0    // seconds from one pulse to the next
#define WIDTH    0.18   // the share of a period one spot stays lit
#define TRAVEL   0.10   // how many periods the pulse takes to run from the centre to the rim
#define RIM      0.485  // the disc's radius in uv
#define CRACK_A0 0.56
#define GAIN     0.75

vec4 Process(vec4 color) {
	vec4 t = getTexel(gl_TexCoord[0].st);
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = clamp(length(p) / RIM, 0.0, 1.0);
	float crack = clamp((t.a - CRACK_A0) / (1.0 - CRACK_A0), 0.0, 1.0);

	// Each disc keeps its own beat: a phase from where it lies, the same for every pixel of the draw.
	vec3 c = gl_TextureMatrix[7][3].xyz;
	float seed = fract(sin(dot(c.xz, vec2(12.9898, 78.233))) * 43758.5453);

	float x = fract(timer / PERIOD + seed - r * TRAVEL);
	float s = x < WIDTH ? sin(3.14159265 * x / WIDTH) : 0.0;
	vec3 glow = vec3(1.0, 0.42, 0.08) * crack * crack * s * s * GAIN;

	// color carries the light alone. The glow is unlit, so it shows in the dark; the alpha test sees only the cutout.
	return vec4(t.rgb * color.rgb + glow, step(0.5, t.a) * color.a);
}
