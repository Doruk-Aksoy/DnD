// Avalanche's snow puffs. Bound to S018A0-D0 in GlDefs/Shaders.txt; built by .claude/tools/avalsnow.

uniform float timer;

#define BOIL       0.024   // how far the puff churns, as a share of its width
#define BOIL_RATE  0.9
#define BREATHE    0.22    // the fringe thinning and filling as it churns
#define SELF_LIGHT 0.20    // so a dark sector greys the snow rather than turning it to soot

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float rim = clamp(length(uv - 0.5) * 2.4, 0.0, 1.0);

	// Two slow waves push the lookup about, harder toward the rim: the edge boils, the middle holds.
	vec2 off = vec2(sin(uv.y * 17.0 + timer * BOIL_RATE * 1.3) + sin(uv.x * 11.0 - timer * BOIL_RATE),
		cos(uv.x * 15.0 - timer * BOIL_RATE * 1.1) + sin(uv.y * 9.0 + timer * BOIL_RATE * 0.8));
	vec4 tex = getTexel(uv + off * BOIL * (0.35 + 0.65 * rim));

	float breathe = sin(uv.x * 23.0 + uv.y * 19.0 + timer * 1.7) * sin(uv.y * 13.0 - timer * 1.1);
	float a = clamp(tex.a * (1.0 + BREATHE * breathe * rim), 0.0, 1.0);

	return vec4(tex.rgb * mix(color.rgb, vec3(1.0), SELF_LIGHT), a * color.a);
}
