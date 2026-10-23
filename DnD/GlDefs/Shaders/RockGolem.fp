// Spell_RockGolem's basalt, on every RGLM frame: the sandstone turned cold grey, its seams and moss glowing molten.
// Each frame's top-left 3x3 holds chest uv and heat, its top-right 3x3 the frame's size, so a texel step is exact.

uniform float timer;

#define FLOW     0.55   // bands a second rising through the seams
#define BANDS    9.0    // bands over the frame's height
#define SEAM     1.4    // how bright the seams run at heat 1
#define FISSURE  1.6    // and the molten fissures where the moss was
#define BREATH   1.9    // radians a second of the slow throb

float rg_lum(vec4 c) {
	return dot(c.rgb, vec3(0.3, 0.59, 0.11)) * c.a;
}

float rg_moss(vec4 c) {
	return smoothstep(0.01, 0.07, c.g - c.r) * c.a;
}

// Upheaval's orange at any strength, warming to yellow only at the hottest; never red, so nothing reads as blood.
vec3 rg_fire(float i) {
	float hot = smoothstep(0.7, 1.6, i);
	return vec3(1.0, 0.42 + 0.4 * hot, 0.08 + 0.3 * hot * hot) * min(1.0, i * 1.4);
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec3 da = getTexel(vec2(0.025, 0.025)).rgb;
	vec3 db = getTexel(vec2(0.975, 0.025)).rgb;
	vec2 px = 1.0 / (db.rg * 255.0);
	vec4 tex = getTexel(uv);
	if(tex.a < 0.5 || uv.y < 3.0 * px.y)
		return vec4(0.0);

	float heat = da.b * 255.0 / 253.0;
	float lum = rg_lum(tex);
	float l = clamp((lum - 0.08) / 0.6, 0.0, 1.0);

	vec2 dx = vec2(px.x, 0.0), dy = vec2(0.0, px.y), dd = px * 1.4, de = vec2(px.x, -px.y) * 1.4;
	vec4 n1 = getTexel(uv + dx), n2 = getTexel(uv - dx), n3 = getTexel(uv + dy), n4 = getTexel(uv - dy);
	vec4 f1 = getTexel(uv + dx * 2.0), f2 = getTexel(uv - dx * 2.0), f3 = getTexel(uv + dy * 2.0), f4 = getTexel(uv - dy * 2.0);
	vec4 f5 = getTexel(uv + dd), f6 = getTexel(uv - dd), f7 = getTexel(uv + de), f8 = getTexel(uv - de);

	// A seam is a texel darker than the ring two texels out, the art's own muscle lines.
	float near = (rg_lum(n1) + rg_lum(n2) + rg_lum(n3) + rg_lum(n4)) / max(n1.a + n2.a + n3.a + n4.a, 0.001);
	float ring = (rg_lum(f1) + rg_lum(f2) + rg_lum(f3) + rg_lum(f4) + rg_lum(f5) + rg_lum(f6) + rg_lum(f7) + rg_lum(f8))
		/ max(f1.a + f2.a + f3.a + f4.a + f5.a + f6.a + f7.a + f8.a, 0.001);
	float crev = smoothstep(0.015, 0.08, ring - (lum + near) * 0.5);

	float m = rg_moss(tex);
	float deep = m * (rg_moss(n1) + rg_moss(n2) + rg_moss(n3) + rg_moss(n4)) * 0.25;

	// Basalt: the sandstone's shading kept, its colour swapped for cold grey, the moss burnt to black hollows.
	vec3 stone = mix(mix(vec3(0.045, 0.045, 0.055), vec3(0.22, 0.22, 0.25), smoothstep(0.0, 0.55, l)), vec3(0.56, 0.54, 0.52), smoothstep(0.5, 1.0, l));
	stone = mix(stone, vec3(0.03, 0.02, 0.02), m);

	vec2 dc = (uv - da.rg) * vec2(0.82, 1.0);
	float heart = exp(-length(dc) / 0.22);
	float breath = 0.85 + 0.15 * sin(timer * BREATH);
	// Bands of heat rising through the seams, bent a little so they never read as scanlines.
	float x = fract(timer * FLOW + uv.y * BANDS * 0.5 + 0.12 * sin(uv.x * 17.0 + uv.y * 5.0));
	float s = x < 0.35 ? sin(3.14159265 * x / 0.35) : 0.0;
	float flow = s * s;

	float seam = crev * (1.0 - m) * heat * SEAM * (0.25 + 0.75 * flow) * (0.45 + 0.9 * heart) * breath;
	float fis = pow(deep, 1.5) * heat * FISSURE * (0.7 + 0.3 * flow) * breath + m * (1.0 - deep) * heat * 0.35;

	// color carries the light alone: the stone is lit, the glow is not, so it shows in the dark.
	return vec4(stone * color.rgb + rg_fire(seam + fis), color.a);
}
