// Living ice: the stone golem recoloured into pale see-through ice, lit from within by frost in its cracks, a beating
// heart in its chest, a cold rim and twinkling glints. Each IGLM frame's top-left 3x3 block holds chest uv and charge.

uniform float timer;

float ig_lum(vec4 c) {
	return dot(c.rgb, vec3(0.3, 0.59, 0.11)) * c.a;
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec4 tex = getTexel(uv);
	vec3 data = getTexel(vec2(0.025, 0.025)).rgb;
	if(tex.a < 0.5 || (uv.x < 0.12 && uv.y < 0.08 && all(lessThan(abs(tex.rgb - data), vec3(0.003)))))
		return vec4(0.0);

	float charge = data.b;
	float lum = ig_lum(tex);
	float l = clamp((lum - 0.1) / 0.7, 0.0, 1.0);

	// One texel is about (0.02, 0.016) on frames 35 to 69 wide and 57 to 71 tall.
	vec2 du = vec2(0.02, 0.0), dv = vec2(0.0, 0.016), dd = vec2(0.028, 0.0224), de = vec2(0.028, -0.0224);
	vec4 n1 = getTexel(uv + du), n2 = getTexel(uv - du), n3 = getTexel(uv + dv), n4 = getTexel(uv - dv);
	vec4 f1 = getTexel(uv + du * 2.0), f2 = getTexel(uv - du * 2.0), f3 = getTexel(uv + dv * 2.0), f4 = getTexel(uv - dv * 2.0);
	vec4 f5 = getTexel(uv + dd), f6 = getTexel(uv - dd), f7 = getTexel(uv + de), f8 = getTexel(uv - de);

	// A crack is a texel darker than the ring two texels out; the centre is blurred against the art's dither.
	float near = (ig_lum(n1) + ig_lum(n2) + ig_lum(n3) + ig_lum(n4)) / max(n1.a + n2.a + n3.a + n4.a, 0.001);
	float ring = (ig_lum(f1) + ig_lum(f2) + ig_lum(f3) + ig_lum(f4) + ig_lum(f5) + ig_lum(f6) + ig_lum(f7) + ig_lum(f8))
		/ max(f1.a + f2.a + f3.a + f4.a + f5.a + f6.a + f7.a + f8.a, 0.001);
	float crev = smoothstep(0.025, 0.12, ring - (lum + near) * 0.5);

	float edge = 1.0 - min(min(n1.a, n2.a), min(n3.a, n4.a));
	float rim = edge + 0.3 * (1.0 - min(min(f1.a, f2.a), min(f3.a, f4.a))) * (1.0 - edge);

	vec3 deep = vec3(0.06, 0.18, 0.36), mid = vec3(0.34, 0.62, 0.86), hi = vec3(0.84, 0.95, 1.0);
	vec3 ice = mix(mix(deep, mid, smoothstep(0.0, 0.55, l)), hi, smoothstep(0.5, 1.0, l));
	ice = mix(ice, deep * 0.7, smoothstep(0.02, 0.1, tex.r - tex.b) * 0.6);

	// The heart beats lub then dub, and the cracks near it glow with it.
	float t = timer * 1.15;
	float beat = pow(0.5 + 0.5 * sin(t * 6.2832), 10.0) + 0.6 * pow(0.5 + 0.5 * sin(t * 6.2832 - 1.1), 10.0);
	vec2 dc = (uv - data.rg) * vec2(0.82, 1.0);
	float r2 = dot(dc, dc);
	float core = exp(-r2 / 0.0018) * (0.55 + 0.45 * beat) * (1.0 + 0.5 * charge) + exp(-r2 / 0.01) * 0.2 * (0.5 + beat);
	float heart = exp(-sqrt(r2) / 0.18);

	// Frost crawling up the cracks, faster and brighter while it swings.
	float flow = 0.5 + 0.5 * sin(uv.y * 23.0 + uv.x * 9.0 - timer * (2.4 + 3.0 * charge) + 2.5 * sin(uv.x * 13.0 - uv.y * 7.0 + timer * 0.8));
	float vein = crev * (0.25 + 0.75 * flow * flow * flow) * (0.5 + heart * (0.6 + 0.5 * beat)) * (1.0 + 1.2 * charge);

	vec2 cell = floor(uv * vec2(56.0, 64.0));
	float tick = mod(floor(timer * 5.0), 97.0);
	float h = fract(sin(dot(cell + vec2(tick * 7.0, tick * 3.0), vec2(12.9898, 78.233))) * 43758.5453);
	float glint = step(0.992, h) * sin(fract(timer * 5.0) * 3.14159) * smoothstep(0.55, 0.8, l);

	float shimmer = 0.75 + 0.25 * sin(timer * 1.7 + uv.y * 9.0) + 0.5 * charge;
	vec3 glow = vec3(0.3, 0.78, 1.0) * (vein + rim * shimmer * 0.45) + vec3(0.75, 0.95, 1.0) * core + vec3(1.0) * glint;
	float a = clamp(0.55 + 0.3 * l + 0.6 * (vein + core + glint) + 0.2 * rim, 0.0, 1.0);
	return vec4(ice * color.rgb + glow, a * color.a);
}
