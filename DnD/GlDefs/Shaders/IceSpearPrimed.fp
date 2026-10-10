// Ice Spear once primed. Bound to ICESPRP; its own texture name purely so it gets its own shader.
// Same uv as IceSpear.fp: s round the spear, t from the tail (0) to the point (1).

uniform float timer;

#define BASE_LIGHT  0.70
#define TEX_VARY    0.62
#define SHEEN       0.30
#define SHEEN_TIGHT 10.0
#define SHEEN_RATE  3.4
#define SURGE       0.45   // bright bands racing to the point
#define SURGE_RATE  2.6
#define VEIN        0.50   // the skin's bright veins, pulsing
#define PULSE_RATE  7.0
#define TIP_GLOW    0.85
#define TIP_POW     4.0

float ip_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

vec4 Process(vec4 color) {
	float around = gl_TexCoord[0].s;
	float along = gl_TexCoord[0].t;

	vec4 tex = getTexel(vec2(around * 2.0, along * 5.0 + timer * 0.11));
	float ice = ip_lum(tex.rgb);

	vec3 col = mix(vec3(0.14, 0.36, 0.74), vec3(0.66, 0.88, 1.00), ice);

	float glint = pow(max(0.0, sin(around * 12.566 + timer * SHEEN_RATE)), SHEEN_TIGHT);
	col += vec3(0.60, 0.85, 1.00) * SHEEN * glint;

	float surge = pow(max(0.0, sin((along * 5.0 - timer * SURGE_RATE) * 6.28318531)), 10.0);
	col += vec3(0.70, 0.90, 1.00) * SURGE * surge;

	// The primed skin's veins are its brightest texels; they throb.
	float pulse = 0.65 + 0.35 * sin(timer * PULSE_RATE);
	col += vec3(0.45, 0.80, 1.00) * VEIN * smoothstep(0.80, 0.98, ice) * pulse;

	col += vec3(0.70, 0.90, 1.00) * TIP_GLOW * pow(clamp(along, 0.0, 1.0), TIP_POW);

	vec3 rgb = col * (BASE_LIGHT + TEX_VARY * ice);
	return vec4(rgb * color.rgb, color.a);
}
