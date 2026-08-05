// Pulsing colored glow shader (Zandronum, opaque) -- by Combinebobnt
uniform float timer;

#define GLOW_INTENSITY 0.6    // how much glow color is added in at the peak of the pulse
#define GLOW_PERIOD    2.0    // seconds per full pulse cycle

#ifndef PI
#define PI 3.141592653589793
#endif

float sineInOut(float t) {
  return -0.5 * (cos(PI * t) - 1.0);
}

vec4 Process(vec4 color)
{
	vec2 uv = gl_TexCoord[0].st;

	vec4 texel = getTexel(uv);

	float phase = timer * (6.283185 / GLOW_PERIOD);
	float pulse = 0.5 + 0.5 * sineInOut(phase);

	vec3 glowColor = vec3(0.4, 0.4, 0.4);
	vec3 glowed = texel.rgb + glowColor * pulse * GLOW_INTENSITY;

	return vec4(glowed * color.rgb, texel.a * color.a);
}
