// Wheel sector border (WHEELSEP, hovered WHEELSEL): turns the canvas to the border the HudMessage alpha names, on
// byte bucket centres. Code n(n-1)/2 + k is border k of n sectors; past SEP_FROMTOP, the same counted from the top.
#define SEP_SIZE 364.0
#define SEP_LINE 226.0      // the art's line starts here in the canvas
#define SEP_FROM 22.0       // and is drawn from this radius, from under the middle ring
#define SEP_FROMTOP 136.0
#define SEP_TAU 6.2831853

// Manual bilinear: HUD art may be drawn unfiltered, and a turned 5 pixel line stairs without it.
vec4 sep_texel(vec2 t) {
	vec2 f = t - 0.5;
	vec2 i = floor(f);
	vec2 w = f - i;
	vec2 base = (i + 0.5) / SEP_SIZE;
	float d = 1.0 / SEP_SIZE;
	vec4 a = texture2D(tex, base);
	vec4 b = texture2D(tex, base + vec2(d, 0.0));
	vec4 c = texture2D(tex, base + vec2(0.0, d));
	vec4 e = texture2D(tex, base + vec2(d, d));
	return mix(mix(a, b, w.x), mix(c, e, w.x), w.y);
}

vec4 Process(vec4 color) {
	float code = floor((1.0 - color.a) * 255.0 - 0.25);
	float start = 0.0;
	if(code >= SEP_FROMTOP) {
		code -= SEP_FROMTOP;
		start = 0.75;
	}
	float n = 1.0;
	for(int i = 0; i < 16; ++i) {
		if(code >= n) {
			code -= n;
			n += 1.0;
		}
	}
	float ang = fract(start + (code + 0.5) / n) * SEP_TAU;
	vec2 p = (gl_TexCoord[0].st - 0.5) * SEP_SIZE;
	float c = cos(ang), s = sin(ang);
	vec2 t = vec2(c * p.x + s * p.y, c * p.y - s * p.x);
	// stretched along its length so it spans the ring to the canvas edge
	t.x = SEP_LINE + (t.x - SEP_FROM) * (SEP_SIZE - SEP_LINE) / (SEP_SIZE * 0.5 - SEP_FROM);
	t.y += SEP_SIZE * 0.5;
	if(t.x < 0.5 || t.y < 0.5 || t.x > SEP_SIZE - 0.5 || t.y > SEP_SIZE - 0.5)
		return vec4(0.0);
	vec4 texel = sep_texel(t);
	return vec4(texel.rgb * color.rgb, texel.a);
}
