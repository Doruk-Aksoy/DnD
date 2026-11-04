// Leech ghost (SPLMANAG, H_BRGSTG/Y/R): the span a pending leech will still fill. The arts carry the dim look and
// alpha 0.85; the mana liquid keeps flowing, and soft bands run toward the ghost's end on both.

uniform float timer;

#define MW 299.0        // mana art and its liquid rows
#define MH 30.0
#define TOP 9.0
#define BOT 20.0
#define LW 86.0         // life and shield fill art width

float hash(vec2 i) {
	vec3 p = fract(vec3(mod(i, 256.0).xyx) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float noise(vec2 p) {
	vec2 i = floor(p), f = fract(p);
	f = f * f * (3.0 - 2.0 * f);
	float a = hash(i), b = hash(i + vec2(1.0, 0.0)), c = hash(i + vec2(0.0, 1.0)), d = hash(i + vec2(1.0, 1.0));
	return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

// soft bands carried toward the bar's end at v texels a second, spaced unevenly
float inflow(float x, float t, float v, float period) {
	float ph = fract((x - t * v) / period + 0.35 * noise(vec2(x / (period * 0.6), 3.0)));
	return smoothstep(0.0, 0.2, ph) * (1.0 - smoothstep(0.2, 0.55, ph));
}

vec3 managhost(vec3 c, vec2 uv, float t) {
	vec2 px = floor(uv * vec2(MW, MH)) + 0.5;
	float v = clamp((px.y - TOP) / (BOT - TOP + 1.0), 0.0, 1.0);
	float depth = 1.0 - 0.55 * abs(v - 0.4) * 2.0;
	float swell = noise(vec2((px.x - t * 6.0) / 26.0, v * 1.2 + t * 0.08));
	return c * (0.8 + 0.4 * swell) + vec3(0.14, 0.3, 0.6) * inflow(px.x, t, 42.0, 70.0) * depth;
}

vec3 lifeghost(vec3 c, vec2 uv, float t) {
	return c * (1.0 + 0.6 * inflow(floor(uv.x * LW) + 0.5, t, 17.0, 28.0));
}

// The mana art is clear along its top row, the life and shield fills are solid.
vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec4 texel = getTexel(uv);
	float t = mod(timer, 512.0);
	vec3 c = texture2D(tex, vec2(0.5, 0.03)).a < 0.5 ? managhost(texel.rgb, uv, t) : lifeghost(texel.rgb, uv, t);
	return vec4(c * color.rgb, texel.a * color.a);
}
