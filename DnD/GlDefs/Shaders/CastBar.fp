// Cast bar fill (SPLCSTF, silenced SPLCSTS): fluid energy flowing into the front. The art spans the trough, its outer
// rows lie on the frame's inner lips; DrawSpellCastBar passes the front's column in the alpha.

uniform float timer;

#define cb_TEXW 276.0
#define cb_TEXH 16.0
#define cb_TOP 2.0
#define cb_ROWS 12.0
#define cb_SPEED 46.0
#define cb_CODES 220.0
#define cb_TAU 6.2831853

// Wrapped to a power of two first, so a lattice corner hashes the same from both cells on every compiler.
float cb_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float cb_noise(vec3 p) {
	vec3 i = floor(p), f = fract(p);
	f = f * f * (3.0 - 2.0 * f);
	float a = mix(cb_hash(i), cb_hash(i + vec3(1.0, 0.0, 0.0)), f.x);
	float b = mix(cb_hash(i + vec3(0.0, 1.0, 0.0)), cb_hash(i + vec3(1.0, 1.0, 0.0)), f.x);
	float c = mix(cb_hash(i + vec3(0.0, 0.0, 1.0)), cb_hash(i + vec3(1.0, 0.0, 1.0)), f.x);
	float d = mix(cb_hash(i + vec3(0.0, 1.0, 1.0)), cb_hash(i + vec3(1.0, 1.0, 1.0)), f.x);
	return mix(mix(a, b, f.y), mix(c, d, f.y), f.z);
}

vec3 cb_ramp(float h, vec3 lo, vec3 mid, vec3 hi) {
	if(h < 0.55)
		return mix(lo, mid, h / 0.55);
	return mix(mid, hi, (h - 0.55) / 0.45);
}

// One-pixel embers thrown a few pixels past the front.
float cb_embers(vec2 px, float front, float t) {
	float e = 0.0;
	for(int k = 0; k < 2; ++k) {
		float tk = t * 1.7 + float(k) * 0.5;
		float ph = fract(tk);
		float row = cb_TOP + 1.0 + floor(cb_hash(vec3(floor(tk), float(k), 3.0)) * (cb_ROWS - 2.0));
		float sx = front + 1.0 + ph * 9.0;
		e += step(abs(px.x - floor(sx) - 0.5), 0.5) * step(abs(px.y - 0.5 - row - floor(ph * 1.5)), 0.5) * (1.0 - ph);
	}
	return e;
}

vec4 Process(vec4 color) {
	vec2 px = floor(gl_TexCoord[0].st * vec2(cb_TEXW, cb_TEXH)) + 0.5;
	// The front rides the alpha on byte bucket centres (FUA keeps alpha as a byte); the crimson art means silenced.
	float front = (floor((1.0 - color.a) * 255.0 - 0.25) + 0.5) * cb_TEXW / cb_CODES;
	float silent = 1.0 - step(0.3, texture2D(tex, vec2(0.5, 0.5)).g);
	float t = mod(timer, 512.0);

	float s = clamp((px.y - cb_TOP - cb_ROWS * 0.5) / (cb_ROWS * 0.5), -1.0, 1.0);
	float core = 1.0 - s * s;
	float lip = 1.0 - step(cb_TOP, px.y) * step(px.y, cb_TOP + cb_ROWS);
	float speed = cb_SPEED * mix(1.0, 0.2, silent);

	// rows surge and lag against each other, and the shapes churn as they travel
	float lag = (cb_noise(vec3(s * 1.6, t * 0.55, 3.0)) - 0.5) * 16.0;
	float x = px.x - t * speed + lag;
	vec3 q = vec3(x / 26.0, s * 1.2, t * 0.45);
	float wx = cb_noise(q) - 0.5;
	float wy = cb_noise(q + vec3(5.2, 1.3, 2.7)) - 0.5;
	vec3 p = vec3(x / 20.0 + wx * 1.7, s * 1.5 + wy * 1.4, t * 0.6);
	float f = cb_noise(p) * 0.65 + cb_noise(p * 2.1 + vec3(3.1, 7.7, 1.9)) * 0.35;
	float ridge = 1.0 - abs(cb_noise(vec3(x / 13.0 + wx * 2.4, s * 2.3 + wy * 2.2, t * 0.85 + 11.0)) * 2.0 - 1.0);
	float tendril = pow(ridge, 8.0);

	// sin phases kept within a turn, where every compiler's sin agrees
	float wob = (0.8 * sin(s * 4.0 + cb_TAU * fract(t * 1.1140846)) + 0.5 * sin(s * 9.0 - cb_TAU * fract(t * 1.7507044) + 1.7)) * (1.0 - 0.7 * silent);
	float df = front - px.x + wob;
	float near = exp(-max(df, 0.0) / 70.0);

	vec3 lo = mix(vec3(0.24, 0.06, 0.01), vec3(0.12, 0.03, 0.06), silent);
	vec3 mid = mix(vec3(0.97, 0.56, 0.13), vec3(0.48, 0.10, 0.16), silent);
	vec3 hi = mix(vec3(1.0, 0.93, 0.72), vec3(0.78, 0.42, 0.44), silent);
	vec3 hot = mix(vec3(1.0, 0.95, 0.80), hi, silent);

	// the frame's inner lips catch the light: faint along the fill, strongest at the front
	if(lip > 0.5) {
		float inner = (px.y < cb_TOP) ? px.y / cb_TOP : (cb_TEXH - px.y) / cb_TOP;
		// Ahead of the front the lips fade as fast as the trough's halo, or their glow runs past the fill.
		float edge = df >= 0.0 ? exp(-df / 10.0) : 0.5 * exp(df / 3.0);
		float l = (0.18 * step(0.0, df) * (0.5 + 0.5 * f) + 0.85 * edge) * inner * (1.0 - 0.5 * silent);
		return vec4(mix(mid, hi, 0.4) * color.rgb, clamp(l, 0.0, 1.0));
	}

	float body = (0.17 + 0.45 * near) * (0.6 + 0.4 * core) + 0.3 * (f - 0.5);
	float heat = clamp(body + (0.42 + 0.15 * near) * tendril * (0.4 + 0.6 * core), 0.0, 1.0);
	vec3 col = cb_ramp(heat, lo, mid, hi);

	float rim = exp(-df * df / 2.2) * (1.0 - 0.6 * silent);
	if(df > 0.0)
		return vec4(mix(col, hot, rim * (0.5 + 0.5 * core)) * color.rgb, 1.0);

	// past the front: a warm halo, a faint spill of light down the empty trough, embers
	float flick = 0.85 + 0.15 * sin(cb_TAU * fract(t * 2.0690142) + s * 3.0);
	float halo = exp(df / 3.0) * (0.4 + 0.6 * core) * flick;
	float spill = exp(df / 9.0) * 0.18 * core * smoothstep(-15.0, -5.0, df);
	float e = cb_embers(px, front, t) * (1.0 - silent);
	float a = clamp(max(max(halo * 0.9, rim), spill + e * 0.9) * (1.0 - 0.55 * silent), 0.0, 1.0);
	return vec4(mix(mix(mid, hi, 0.5), hot, max(rim, e)) * color.rgb, a);
}
