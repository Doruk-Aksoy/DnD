// A slam's shockwave: a sheet of force kicked up off the ground, brightest at its foot and along its crest, streaked
// with speed lines, over a band glowing on the floor. The skin's top row is the tint, so one shader serves every colour.

uniform float timer;

#define TAU 6.2831853

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec3 tint = getTexel(vec2(0.5, 0.01)).rgb;
	float t = uv.y, a = fract(uv.x) * TAU, glow;

	if(uv.x < 1.5) {
		// The wall. Whole multiples of the angle, so nothing has a seam round the ring.
		float top = 0.8 + 0.07 * sin(a * 7.0 + timer * 2.3) * sin(a * 3.0 - timer * 1.7) + 0.04 * sin(a * 23.0 + timer * 5.1);
		float body = smoothstep(top + 0.02, top - 0.12, t);
		float crest = exp(-(t - top + 0.07) * (t - top + 0.07) / 0.003);
		float speed = 0.72 + 0.28 * sin(a * 61.0 + 4.0 * sin(a * 7.0 + timer * 0.7));
		float flow = 0.82 + 0.18 * sin(a * 13.0 - t * 9.0 + timer * 18.0);
		float base = 0.3 + 0.7 * exp(-t * 3.2);
		float foot = exp(-(t - 0.025) * (t - 0.025) / 0.0008);
		glow = body * (base * speed * flow + crest * 0.85) + foot;
	}
	else {
		// The floor band, t from its inner edge out to the wall's foot.
		float band = exp(-(t - 0.88) * (t - 0.88) / 0.006) + 0.4 * exp((t - 1.0) * 3.0);
		glow = band * (0.75 + 0.25 * sin(a * 19.0 + timer * 4.0)) * 0.85;
	}

	vec3 col = tint * glow + vec3(1.0) * smoothstep(0.75, 1.5, glow) * 0.55;
	return vec4(col * color.rgb, color.a);
}
