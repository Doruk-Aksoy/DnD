// Mana bar edge (SPLMANAE): an additive glowing meniscus where the fill ends; same packed alpha as the fill.

uniform float timer;

#define EW 16.0         // art size, centred on the fill's end
#define EH 30.0
#define TOP 9.0
#define BOT 20.0

float me_hash(float n) {
	return fract(sin(n * 91.7) * 43758.5453);
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec2 px = floor(uv * vec2(EW, EH)) + 0.5;
	if(px.y < TOP || px.y > BOT + 1.0)
		return vec4(0.0, 0.0, 0.0, 1.0);

	float code = floor((1.0 - color.a) * 64.0 + 0.5);
	float surge = floor(code / 8.0) / 7.0;
	float low = mod(code, 8.0) / 7.0 * (1.0 - surge);
	float v = (px.y - TOP) / (BOT - TOP + 1.0);
	float agit = 1.0 + 1.8 * surge - 0.5 * low;

	// the meniscus wobbles a texel or two
	float wob = 0.9 * sin(v * 5.3 + timer * 3.1 * agit) + 0.6 * sin(v * 11.7 - timer * 4.7 * agit + 1.3);
	float cx = EW * 0.5 - 0.5 + wob * (0.6 + 0.5 * surge);
	float dx = px.x - cx;
	float core = exp(-dx * dx / 0.9);
	float halo = exp(-dx * dx / 12.0) * (dx < 0.0 ? 0.8 : 0.3);
	float flick = 0.85 + 0.15 * sin(timer * 9.0 + v * 7.0);
	float rows = smoothstep(0.0, 0.2, v) * (1.0 - smoothstep(0.85, 1.05, v));
	float gain = flick * rows * (1.3 + 0.9 * surge) * (1.0 - 0.55 * low);

	// droplets flung ahead of the edge while surging
	float drops = 0.0;
	for(int k = 0; k < 3; ++k) {
		float tk = timer * 1.9 + float(k) * 0.37;
		float id = floor(tk) + float(k) * 17.0;
		float ph = fract(tk);
		float sy = TOP + 1.0 + floor(me_hash(id) * (BOT - TOP - 1.0));
		float sx = cx + 1.5 + ph * 5.5;
		drops += step(abs(px.x - floor(sx) - 0.5), 0.5) * step(abs(px.y - 0.5 - sy), 0.5) * (1.0 - ph);
	}

	vec3 c = vec3(0.35, 0.65, 1.0) * halo + vec3(0.8, 0.93, 1.0) * core;
	c = c * gain + vec3(0.7, 0.88, 1.0) * drops * surge * 0.9;
	return vec4(c, 1.0);
}
