// Holy Wrath's image (HWI1..HWI8, each class's frames padded for the light): gold then white-hot, a halo, rays at
// the peak. The actor's alpha carries it (Add): 0.5..1 the build-up at full strength, under 0.5 the fade at the peak.

uniform float timer;

#define LW      144.0  // the padded lump's size in pixels, the same for every rotation
#define LH      150.0
#define TAU     6.2831853

float hi_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float hi_noise(vec2 x) {
	vec2 i = floor(x);
	vec2 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float a = hi_hash(vec3(i, 2.0));
	float b = hi_hash(vec3(i + vec2(1.0, 0.0), 2.0));
	float c = hi_hash(vec3(i + vec2(0.0, 1.0), 2.0));
	float d = hi_hash(vec3(i + vec2(1.0, 1.0), 2.0));
	return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec4 tx = getTexel(uv);
	float e = color.a >= 0.5 ? color.a * 2.0 - 1.0 : 1.0;
	float op = color.a >= 0.5 ? 1.0 : color.a * 2.0;
	float t = mod(timer, 512.0);

	// The figure's outline blurred over two rings of taps: a tight glow and a wide bloom.
	vec2 px = vec2(1.0 / LW, 1.0 / LH);
	float h1 = 0.0;
	float h2 = 0.0;
	for(int i = 0; i < 12; i++) {
		float ang = TAU * float(i) / 12.0;
		vec2 o = vec2(cos(ang), sin(ang));
		h1 += getTexel(uv + o * px * 4.0).a;
		h2 += getTexel(uv + o * px * 11.0).a;
	}
	h1 /= 12.0;
	h2 /= 12.0;

	// The body turns to light, gold first, then white-hot; light rises through it.
	float lum = dot(tx.rgb, vec3(0.3, 0.59, 0.11));
	vec3 body = mix(vec3(1.0, 0.6, 0.22) * (0.3 + lum * 1.5), vec3(1.0, 0.96, 0.82), e * 0.85);
	float rise = 0.7 + 0.45 * hi_noise(vec2(uv.x * 7.0, uv.y * 12.0 + t * 4.0));
	float rim = tx.a * (1.0 - h1);
	vec3 col = body * tx.a * (0.55 + 0.6 * e) * rise + vec3(1.0, 0.9, 0.7) * rim * (0.4 + e);
	col += vec3(1.0, 0.58, 0.18) * (h1 * 0.5 + h2 * 0.65) * (1.0 - tx.a * 0.6) * (0.2 + 1.3 * e);

	// Near the peak, rays break out of it.
	vec2 c = (uv - vec2(0.5, 0.42)) * vec2(LW, LH) / max(LW, LH);
	float r = length(c);
	float ray = hi_noise(vec2((atan(c.y, c.x + 1e-4) / TAU + 0.5) * 22.0, t * 1.5));
	col += vec3(1.0, 0.82, 0.45) * pow(ray, 3.0) * (1.0 - smoothstep(0.08, 0.38, r)) * e * e * 0.9;
	// everything fades out before the canvas edge, so no glow is cut square
	vec2 eb = min(uv, 1.0 - uv) * vec2(LW, LH);
	col *= smoothstep(0.0, 14.0, min(eb.x, eb.y));
	return vec4(min(col, vec3(1.0)) * op, 1.0);
}
