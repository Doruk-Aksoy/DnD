// Ice Spear, forming and in its slow flight. Bound to ICESPEAR in GlDefs/Shaders.txt.
// s runs round the spear, t from the tail (0) to the point (1); the skin is tiled here, not in the uv.

uniform float timer;

#define BASE_LIGHT  0.56
#define TEX_VARY    0.58
#define SHEEN       0.22
#define SHEEN_TIGHT 14.0
#define SHEEN_RATE  2.1
#define FROST_RUN   0.14   // faint lines running down to the point
#define TIP_GLOW    0.55
#define TIP_POW     6.0

float is_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

vec4 Process(vec4 color) {
	float around = gl_TexCoord[0].s;
	float along = gl_TexCoord[0].t;

	vec4 tex = getTexel(vec2(around * 2.0, along * 5.0 + timer * 0.04));
	float ice = is_lum(tex.rgb);

	vec3 col = mix(vec3(0.10, 0.28, 0.62), vec3(0.55, 0.80, 0.98), ice);

	// A glint round the facets; the model spins, so it sweeps.
	float glint = pow(max(0.0, sin(around * 12.566 + timer * SHEEN_RATE)), SHEEN_TIGHT);
	col += vec3(0.50, 0.75, 1.00) * SHEEN * glint;

	float run = pow(max(0.0, sin((along * 3.0 - timer * 0.9) * 6.28318531)), 18.0);
	col += vec3(0.35, 0.60, 0.95) * FROST_RUN * run;

	// The point is the business end and carries the light.
	col += vec3(0.55, 0.78, 1.00) * TIP_GLOW * pow(clamp(along, 0.0, 1.0), TIP_POW);

	vec3 rgb = col * (BASE_LIGHT + TEX_VARY * ice);
	return vec4(rgb * color.rgb, color.a);
}
