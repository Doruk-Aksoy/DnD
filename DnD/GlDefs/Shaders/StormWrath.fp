// Storm Call's marked ground and Wrath's aura, one code path with each one's constants.
// The fallback art says which; Storm Call also reads its charge from the actor's alpha and crackles.

uniform float timer;

#define TAU 6.2831853

float sw_wrap(float a) {
	return a - TAU * floor(a / TAU + 0.5);
}

float sw_hash(vec2 p) {
	return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

// how far an arc's path swings at radius r; zero at the core so every arc leaves from it
float sw_wander(float r, float ph, float wander, vec4 amp, vec4 freq, vec4 rate) {
	return wander * r * (amp.x * sin(r * freq.x + timer * rate.x + ph) + amp.y * sin(r * freq.y - timer * rate.y + ph * 1.7) +
		amp.z * sin(r * freq.z + timer * rate.z + ph * 2.3) + amp.w * sin(r * freq.w - timer * rate.w + ph * 3.1));
}

vec4 Process(vec4 color) {
	bool storm = texture2D(tex, vec2(0.703, 0.215)).b - texture2D(tex, vec2(0.344, 0.305)).r > 0.2;

	float arcs = storm ? 11.0 : 7.0;
	int n_arcs = storm ? 11 : 7;
	float arc_width = storm ? 0.012 : 0.016;
	float arc_glow = storm ? 0.050 : 0.060;
	float wander = storm ? 0.62 : 0.60;
	float drift = storm ? 0.45 : 0.35;
	float fork_at = storm ? 0.35 : 0.42;
	float fork_splay = storm ? 1.20 : 0.95;
	float core_r = storm ? 0.12 : 0.15;
	float rim_r = storm ? 0.90 : 0.86;
	float rim_w = storm ? 0.030 : 0.035;
	float rim_soft = storm ? 0.95 : 0.93;
	vec4 w_amp = storm ? vec4(0.50, 0.30, 0.12, 0.05) : vec4(0.55, 0.30, 0.15, 0.08);
	vec4 w_freq = storm ? vec4(6.0, 13.0, 21.0, 37.0) : vec4(5.0, 11.0, 23.0, 41.0);
	vec4 w_rate = storm ? vec4(6.3, 10.9, 19.1, 33.0) : vec4(5.1, 8.7, 15.9, 29.0);

	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;
	float mask = 1.0 - smoothstep(rim_soft, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);

	float charge = clamp(color.a, 0.0, 1.0);
	float ang = atan(p.y, p.x);
	float arc_r = max(r, 0.05);
	float span = smoothstep(core_r * 0.4, core_r + (storm ? 0.1 : 0.12), r) * (1.0 - smoothstep(rim_r + 0.02, rim_soft, r));
	float core = 0.0;
	float haze = 0.0;
	float touch = 0.0;

	float base_wob = storm ? 0.4 : 0.3;
	float life_lo = storm ? 0.35 : 0.4;
	float life_hi = storm ? 0.65 : 0.7;
	float life_rate = storm ? 4.2 : 3.4;
	float life_step = storm ? 0.9 : 0.7;
	float w_floor = storm ? 0.6 : 0.55;
	float w_charge = storm ? 0.7 + 0.6 * charge : 1.0;
	float fork_spread = storm ? 0.25 : 0.18;
	float fork_fade = storm ? 0.08 : 0.10;
	float touch_w = storm ? 0.003 : 0.004;

	for(int k = 0; k < 11; ++k) {
		if(k >= n_arcs)
			break;
		float ph = float(k) * 2.399;
		float base = float(k) * TAU / arcs + timer * drift * TAU * (1.0 + base_wob * sin(ph));
		float life = smoothstep(life_lo, life_hi, 0.5 + 0.5 * sin(timer * (life_rate + life_step * float(k)) + ph * 3.1));
		float w = arc_width * (w_floor + 0.6 * life) * w_charge;

		float sway = sw_wander(r, ph, wander, w_amp, w_freq, w_rate);
		float d = abs(sw_wrap(ang - base - sway)) * arc_r;
		core += exp(-d * d / (w * w)) * life;
		haze += exp(-d / arc_glow) * life;

		float fr = fork_at + fork_spread * fract(ph * 0.37);
		float fork = base + sway + fork_splay * sin(ph * 5.0) * max(r - fr, 0.0);
		float fin = smoothstep(fr, fr + fork_fade, r) * life;
		float fd = abs(sw_wrap(ang - fork)) * arc_r;
		core += exp(-fd * fd / (w * w * 0.5)) * fin * 0.8;
		haze += exp(-fd / arc_glow) * fin * 0.5;

		float td = abs(sw_wrap(ang - base - sw_wander(rim_r, ph, wander, w_amp, w_freq, w_rate))) * rim_r;
		touch += exp(-td * td / touch_w) * life;
	}

	// Storm Call's crackle: hard little sparks popping across the field, more as it charges
	float spark = 0.0;
	if(storm) {
		vec2 q = p * 14.0;
		vec2 cell = floor(q);
		float h = sw_hash(cell);
		vec2 sd = fract(q) - 0.5 - (vec2(sw_hash(cell + 7.1), sw_hash(cell + 3.7)) - 0.5) * 0.5;
		float pop = pow(max(0.0, sin(timer * (9.0 + 8.0 * h) + h * 60.0)), 16.0) * step(1.0 - 0.45 * charge - 0.25, h);
		spark = pop * (exp(-dot(sd, sd) * 120.0) + exp(-abs(sd.x) * 24.0 - sd.y * sd.y * 1600.0) +
			exp(-abs(sd.y) * 24.0 - sd.x * sd.x * 1600.0));
	}

	float rd = (r - rim_r) / rim_w;
	float crawl = storm ? 0.55 + 0.45 * sin(ang * 12.0 + timer * 5.1) * sin(ang * 7.0 - timer * 3.7 + 1.7) :
		0.55 + 0.45 * sin(ang * 9.0 + timer * 3.1) * sin(ang * 5.0 - timer * 2.3 + 1.7);
	float rim = exp(-rd * rd) * (storm ? crawl * 0.9 + touch * 1.8 : crawl * 0.75 + touch * 1.6);

	float hub = exp(-(r * r) / (core_r * core_r)) * (storm ? 0.8 + 0.2 * sin(timer * 11.0) * sin(timer * 4.7) :
		0.75 + 0.25 * sin(timer * 7.0) * sin(timer * 3.3));

	vec3 col;
	if(storm)
		col = vec3(0.06, 0.10, 0.42) * (0.30 + 0.35 * r) +
			vec3(0.18, 0.36, 1.00) * haze * 0.75 * span +
			vec3(0.70, 0.84, 1.00) * core * 1.9 * span +
			vec3(0.40, 0.58, 1.00) * rim +
			vec3(0.60, 0.75, 1.00) * hub +
			vec3(0.80, 0.92, 1.00) * spark * 2.2 * span;
	else
		col = vec3(0.16, 0.14, 0.42) * (0.25 + 0.30 * r) +
			vec3(0.34, 0.40, 1.00) * haze * 0.70 * span +
			vec3(0.86, 0.92, 1.00) * core * 1.90 * span +
			vec3(0.56, 0.62, 1.00) * rim +
			vec3(0.75, 0.82, 1.00) * hub;

	vec3 rgb = col * (storm ? mix(0.45, 1.35, charge) : 1.10) * mask;
	return vec4(rgb * color.rgb, storm ? mask : mask * color.a);
}
