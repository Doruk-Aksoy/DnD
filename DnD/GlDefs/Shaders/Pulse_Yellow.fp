// Pulsing colored glow shader (Zandronum, opaque) -- by Combinebobnt
uniform float timer;

#define GLOW_HUE       45.0   // hue 0.0-360.0 (0.0 == red, 120.0 == green, etc.)
#define GLOW_INTENSITY 0.6    // how much glow color is added in at the peak of the pulse
#define GLOW_PERIOD    1.0    // seconds per full pulse cycle

vec3 hueToRGB(float hue)
{
	float r = abs(hue * 6.0 - 3.0) - 1.0;
	float g = 2.0 - abs(hue * 6.0 - 2.0);
	float b = 2.0 - abs(hue * 6.0 - 4.0);
	return clamp(vec3(r, g, b), 0.0, 1.0);
}

vec4 Process(vec4 color)
{
	vec2 uv = gl_TexCoord[0].st;

	vec4 texel = getTexel(uv);

	float phase = timer * (6.283185 / GLOW_PERIOD);
	float pulse = 0.5 + 0.5 * sin(phase);

	vec3 glowColor = hueToRGB(GLOW_HUE / 360.0);
	vec3 glowed = texel.rgb + glowColor * pulse * GLOW_INTENSITY;

	return vec4(glowed * color.rgb, texel.a * color.a);
}