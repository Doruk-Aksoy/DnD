// Glacial Spike. Bound to GLACIAL in GlDefs/Shaders.txt.

uniform float timer;

#define BASE_LIGHT 0.62
#define TEX_VARY   0.52
#define SHEEN      0.16   // "shiny but not super shiny" lives here
#define SHEEN_TIGHT 12.0  // higher is a narrower glint
#define SHEEN_RATE  1.7
#define TIP_GLOW   0.16
#define PULSE      0.13
#define PULSE_RATE  3.1
#define GAIN       1.00

float gs_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

vec4 Process(vec4 color) {
	float around = gl_TexCoord[0].s;
	float along = gl_TexCoord[0].t;

	// The skin drifts a little along the spike so the ice is not frozen to the geometry.
	vec4 tex = getTexel(vec2(around, along + timer * 0.03));
	float ice = gs_lum(tex.rgb);

	vec3 col = mix(vec3(0.13, 0.33, 0.66), vec3(0.50, 0.76, 0.96), ice);

	// A glint travelling round the circumference; the model spins, so it reads as a facet.
	float glint = pow(max(0.0, sin(around * 6.28318531 + timer * SHEEN_RATE)), SHEEN_TIGHT);
	col += vec3(0.42, 0.70, 0.94) * SHEEN * glint;

	// The point is the business end, so it carries a little more light.
	col += vec3(0.28, 0.52, 0.86) * TIP_GLOW * pow(clamp(along, 0.0, 1.0), 3.0);

	float pulse = 1.0 + PULSE * sin(timer * PULSE_RATE);

	vec3 rgb = col * (BASE_LIGHT + TEX_VARY * ice) * GAIN * pulse;

	// The skin is RGB with no alpha, so opacity is the shader's to decide.
	return vec4(rgb * color.rgb, color.a);
}
