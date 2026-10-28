// Ice Spear, the primed Ice Spear and Glacial Spike, one code path with each one's constants.
// The skin says which: Glacial Spike's is blue, the primed spear's carries half alpha, the spear's neither.

uniform float timer;

float ic_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

vec4 Process(vec4 color) {
	float around = gl_TexCoord[0].s;
	float along = gl_TexCoord[0].t;

	vec4 k0 = texture2D(tex, vec2(0.3, 0.3)), k1 = texture2D(tex, vec2(0.7, 0.3));
	vec4 k2 = texture2D(tex, vec2(0.3, 0.7)), k3 = texture2D(tex, vec2(0.7, 0.7));
	bool glacial = (k0.b - k0.r) + (k1.b - k1.r) + (k2.b - k2.r) + (k3.b - k3.r) > 0.6;
	bool primed = !glacial && k0.a < 0.75;

	// skin tiling and drift, palette, light
	float tile_s = glacial ? 1.0 : 2.0;
	float tile_t = glacial ? 1.0 : 5.0;
	float drift = glacial ? 0.03 : (primed ? 0.11 : 0.04);
	vec3 deep = glacial ? vec3(0.13, 0.33, 0.66) : (primed ? vec3(0.06, 0.20, 0.56) : vec3(0.10, 0.28, 0.62));
	vec3 pale = glacial ? vec3(0.50, 0.76, 0.96) : (primed ? vec3(0.36, 0.68, 0.98) : vec3(0.55, 0.80, 0.98));
	float base_light = glacial ? 0.62 : (primed ? 0.55 : 0.56);
	float tex_vary = glacial ? 0.52 : (primed ? 0.45 : 0.58);

	// the glint round the circumference
	float glint_freq = glacial ? 6.28318531 : 12.566;
	float sheen_rate = glacial ? 1.7 : (primed ? 3.4 : 2.1);
	float sheen_tight = glacial ? 12.0 : (primed ? 10.0 : 14.0);
	float sheen = glacial ? 0.16 : 0.22;
	vec3 sheen_col = glacial ? vec3(0.42, 0.70, 0.94) : (primed ? vec3(0.60, 0.85, 1.00) : vec3(0.50, 0.75, 1.00));

	// bands running down to the point: the spear's frost, the primed spear's surge, none on Glacial Spike
	float run_len = primed ? 5.0 : 3.0;
	float run_rate = primed ? 2.6 : 0.9;
	float run_pow = primed ? 10.0 : 18.0;
	float run_amt = glacial ? 0.0 : (primed ? 0.30 : 0.14);
	vec3 run_col = primed ? vec3(0.70, 0.90, 1.00) : vec3(0.35, 0.60, 0.95);

	vec3 tip_col = glacial ? vec3(0.28, 0.52, 0.86) : (primed ? vec3(0.70, 0.90, 1.00) : vec3(0.55, 0.78, 1.00));
	float tip_glow = glacial ? 0.16 : (primed ? 0.50 : 0.55);
	float tip_pow = glacial ? 3.0 : (primed ? 4.0 : 6.0);

	vec4 tex = getTexel(vec2(around * tile_s, along * tile_t + timer * drift));
	float ice = ic_lum(tex.rgb);

	vec3 col = mix(deep, pale, ice);

	float glint = pow(max(0.0, sin(around * glint_freq + timer * sheen_rate)), sheen_tight);
	col += sheen_col * sheen * glint;

	float run = pow(max(0.0, sin((along * run_len - timer * run_rate) * 6.28318531)), run_pow);
	col += run_col * run_amt * run;

	// the primed skin's brightest texels throb
	float vein_pulse = 0.65 + 0.35 * sin(timer * 7.0);
	col += vec3(0.45, 0.80, 1.00) * (primed ? 0.38 : 0.0) * smoothstep(0.80, 0.98, ice) * vein_pulse;

	col += tip_col * tip_glow * pow(clamp(along, 0.0, 1.0), tip_pow);

	// Glacial Spike's whole body swells; the spears hold steady
	float swell = glacial ? 1.0 + 0.13 * sin(timer * 3.1) : 1.0;

	vec3 rgb = col * (base_light + tex_vary * ice) * swell;
	return vec4(rgb * color.rgb, color.a);
}
