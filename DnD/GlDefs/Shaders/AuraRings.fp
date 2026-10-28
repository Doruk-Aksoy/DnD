// Anger, Hatred, Righteous Fire and Annihilus auras in one shader; the art says which one is drawing.

uniform float timer;

#define ang_SPIN_A    0.22
#define ang_SPIN_B   -0.15
#define ang_SHEAR     0.40
#define ang_SCALE_B   0.78
#define ang_RIM_SOFT  0.90
#define ang_CORE      0.26
#define ang_CORE_DIM  0.30
#define ang_CORE_EDGE 0.46
#define ang_EMBER     0.65
#define ang_CONTRAST  1.60
#define ang_GAIN      0.875

#define ang_PULSE_PERIOD 7.0
#define ang_PULSE_RISE0  0.12
#define ang_PULSE_RISE1  0.34
#define ang_PULSE_FALL0  0.62
#define ang_PULSE_FALL1  0.84
#define ang_PULSE_MIN    0.72
#define ang_PULSE_MAX    1.18

vec2 ang_an_spin(vec2 p, float a) {
	float s = sin(a);
	float c = cos(a);
	return vec2(p.x * c - p.y * s, p.x * s + p.y * c);
}

float ang_an_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

vec4 ang_process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;

	float mask = 1.0 - smoothstep(ang_RIM_SOFT, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);

	vec4 ta = getTexel(0.5 + ang_an_spin(p, timer * ang_SPIN_A + ang_SHEAR * r));
	vec4 tb = getTexel(0.5 + ang_an_spin(p * ang_SCALE_B, timer * ang_SPIN_B - ang_SHEAR * r));

	float heat = ang_an_lum(ta.rgb) * (0.45 + 0.75 * ang_an_lum(tb.rgb));
	heat = pow(clamp(heat, 0.0, 1.0), ang_CONTRAST);

	heat *= mix(ang_CORE_DIM, 1.0, smoothstep(0.0, ang_CORE_EDGE, r));

	float band = smoothstep(ang_CORE, 1.0, r);
	vec3 col = mix(ta.rgb, ta.rgb * vec3(1.00, 0.42, 0.12), ang_EMBER * band);

	col = mix(col, col * vec3(1.00, 0.68, 0.26), smoothstep(0.55, 1.0, heat));

	float ph = fract(timer / ang_PULSE_PERIOD);
	float env = min(smoothstep(ang_PULSE_RISE0, ang_PULSE_RISE1, ph),
		1.0 - smoothstep(ang_PULSE_FALL0, ang_PULSE_FALL1, ph));
	float pulse = mix(ang_PULSE_MIN, ang_PULSE_MAX, env);

	vec3 rgb = col * heat * ang_GAIN * pulse * mask;
	return vec4(rgb * color.rgb, mask * color.a);
}

#define hat_ASPECT      0.561
#define hat_SCROLL      0.035
#define hat_RIM_SOFT    0.80
#define hat_BACKDROP    0.45
#define hat_CONTRAST    1.35
#define hat_FROST       0.55
#define hat_GAIN        1.25
#define hat_SHEEN       1.00
#define hat_SHEEN_EVERY 6.0
#define hat_SHEEN_WIDTH 0.20
#define hat_CELLS       6.0
#define hat_SPARK_SHARE 0.60
#define hat_SPARK       2.40
#define hat_SPARK_RATE  1.60

float hat_ht_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

float hat_ht_hash(vec2 p) {
	return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float hat_ht_mirror(float x) {
	return 1.0 - abs(1.0 - mod(x, 2.0));
}

vec4 hat_process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;

	float mask = 1.0 - smoothstep(hat_RIM_SOFT, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);

	float drift = timer * hat_SCROLL;
	vec3 tex = getTexel(vec2(0.5 + p.x * hat_ASPECT, hat_ht_mirror(0.5 + p.y - drift))).rgb;
	float lum = hat_ht_lum(tex);

	float flake = pow(smoothstep(0.28, 0.95, lum), hat_CONTRAST);
	vec3 col = mix(tex * hat_BACKDROP, mix(tex, vec3(0.86, 0.95, 1.00), hat_FROST), flake);

	float band = mix(-1.6, 1.6, fract(timer / hat_SHEEN_EVERY));

	float sd = (p.x + p.y - band) / hat_SHEEN_WIDTH;
	float sheen = exp(-sd * sd);
	col += vec3(0.70, 0.88, 1.00) * hat_SHEEN * sheen * (0.25 + flake);

	vec2 q = vec2(p.x, p.y - drift) * hat_CELLS;
	vec2 cell = floor(q);
	float h = hat_ht_hash(cell);
	if(h < hat_SPARK_SHARE) {
		vec2 d = fract(q) - 0.5 - (vec2(hat_ht_hash(cell + 7.1), hat_ht_hash(cell + 3.7)) - 0.5) * 0.5;
		float tw = pow(max(0.0, sin(timer * hat_SPARK_RATE * (0.6 + h) + h * 40.0)), 8.0);
		float star = exp(-dot(d, d) * 70.0) +
			exp(-abs(d.x) * 16.0 - d.y * d.y * 900.0) + exp(-abs(d.y) * 16.0 - d.x * d.x * 900.0);
		col += vec3(0.85, 0.95, 1.00) * hat_SPARK * tw * star;
	}

	vec3 rgb = col * hat_GAIN * mask;
	return vec4(rgb * color.rgb, mask * color.a);
}

#define rf_RING_IN   0.50
#define rf_HOLE      0.16
#define rf_SPIN_A    0.18
#define rf_SPIN_B   -0.26
#define rf_SHEAR_A   0.45
#define rf_SHEAR_B  -0.30
#define rf_SCALE_B   0.94
#define rf_RIM_SOFT  0.90
#define rf_FLOOR_B   0.35
#define rf_CONTRAST  1.30
#define rf_COOL      0.45
#define rf_TONGUE_1  0.055
#define rf_TONGUE_2  0.030
#define rf_FLICKER   0.16
#define rf_GAIN      2.00

vec2 rf_rf_spin(vec2 p, float a) {
	float s = sin(a);
	float c = cos(a);
	return vec2(p.x * c - p.y * s, p.x * s + p.y * c);
}

float rf_rf_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

vec4 rf_process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float len = length(p);
	float r = len * 2.0;
	float th = atan(p.y, p.x);

	float edge = 1.0
		- rf_TONGUE_1 * sin(5.0 * th + timer * 1.3)
		- rf_TONGUE_2 * sin(8.0 * th - timer * 2.1);

	float mask = 1.0 - smoothstep(edge * rf_RIM_SOFT, edge, r);
	if(mask <= 0.0)
		return vec4(0.0);

	float t = clamp((r - rf_HOLE) / (1.0 - rf_HOLE), 0.0, 1.0);
	float tra = mix(rf_RING_IN, 1.0, t);
	vec2 dir = p / max(len, 1e-5);

	vec4 ta = getTexel(0.5 + rf_rf_spin(dir * tra * 0.5, timer * rf_SPIN_A + rf_SHEAR_A * r));
	vec4 tb = getTexel(0.5 + rf_rf_spin(dir * tra * rf_SCALE_B * 0.5, timer * rf_SPIN_B + rf_SHEAR_B * r));

	float shape = ta.a * (rf_FLOOR_B + (1.0 - rf_FLOOR_B) * tb.a);
	shape *= smoothstep(0.0, rf_HOLE + 0.14, r);

	float lum = rf_rf_lum(ta.rgb) * (rf_FLOOR_B + (1.0 - rf_FLOOR_B) * rf_rf_lum(tb.rgb));
	lum = pow(clamp(lum, 0.0, 1.0), rf_CONTRAST);

	float band = smoothstep(0.0, 1.0, r);

	vec3 col = mix(ta.rgb, ta.rgb * vec3(1.00, 0.42, 0.12), rf_COOL * band);

	float flick = 1.0 + rf_FLICKER * band *
		(sin(timer * 6.3 + th * 3.0) * 0.6 + sin(timer * 9.1) * 0.4);

	vec3 rgb = col * lum * rf_GAIN * flick * mask;
	return vec4(rgb * color.rgb, shape * mask * color.a);
}

#define anh_CHURN_A    0.07
#define anh_CHURN_B   -0.11
#define anh_SHEAR      0.35
#define anh_SCALE_B    0.86
#define anh_RIM_SOFT   0.94
#define anh_VEIN_LO    0.20
#define anh_VEIN_HI    0.68
#define anh_VEIN_DRIFT 0.13
#define anh_RIMHEAT    0.50
#define anh_PULSE      0.20
#define anh_CONTRAST   1.25
#define anh_GAIN       1.90

vec2 anh_an_spin(vec2 p, float a) {
	float s = sin(a);
	float c = cos(a);
	return vec2(p.x * c - p.y * s, p.x * s + p.y * c);
}

float anh_an_lum(vec3 c) {
	return dot(c, vec3(0.299, 0.587, 0.114));
}

vec4 anh_process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;
	float th = atan(p.y, p.x);

	float mask = 1.0 - smoothstep(anh_RIM_SOFT, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);

	vec4 ta = getTexel(0.5 + anh_an_spin(p, timer * anh_CHURN_A + anh_SHEAR * r));
	vec4 tb = getTexel(0.5 + anh_an_spin(p * anh_SCALE_B, timer * anh_CHURN_B - anh_SHEAR * r));

	float shape = ta.a;
	float heat = anh_an_lum(ta.rgb) * (0.40 + 0.80 * anh_an_lum(tb.rgb));
	heat = pow(clamp(heat, 0.0, 1.0), anh_CONTRAST);

	float lo = anh_VEIN_LO + anh_VEIN_DRIFT * sin(timer * 0.9 + th * 2.0 + r * 3.0);
	float vein = smoothstep(lo, anh_VEIN_HI, heat);

	vec3 col = mix(vec3(0.14, 0.02, 0.01), vec3(0.92, 0.20, 0.02), smoothstep(0.0, 0.45, heat));
	col = mix(col, vec3(1.00, 0.70, 0.16), vein);
	col = mix(col, vec3(1.00, 0.94, 0.68), smoothstep(0.80, 1.0, heat) * vein);

	col += vec3(0.55, 0.16, 0.02) * anh_RIMHEAT * smoothstep(0.55, 1.0, r) * heat;

	float pulse = 1.0 + anh_PULSE * (sin(timer * 1.6) * 0.6 + sin(timer * 2.7 + 1.3) * 0.4);

	vec3 rgb = col * (0.45 + 0.85 * heat) * anh_GAIN * pulse * mask;

	return vec4(rgb * color.rgb, shape * mask * color.a);
}

// Clear corners are the two cutouts, a clear centre Righteous Fire's ring; blue is Hatred's snow.
vec4 Process(vec4 color) {
	if(texture2D(tex, vec2(0.04, 0.04)).a + texture2D(tex, vec2(0.96, 0.96)).a < 1.0) {
		if(texture2D(tex, vec2(0.5, 0.5)).a < 0.5)
			return rf_process(color);
		return anh_process(color);
	}
	vec4 s = texture2D(tex, vec2(0.3, 0.3)) + texture2D(tex, vec2(0.7, 0.3)) + texture2D(tex, vec2(0.3, 0.7)) +
		texture2D(tex, vec2(0.7, 0.7)) + texture2D(tex, vec2(0.5, 0.5));
	if(s.b > s.r)
		return hat_process(color);
	return ang_process(color);
}
