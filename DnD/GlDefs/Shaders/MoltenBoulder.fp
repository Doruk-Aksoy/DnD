// Molten Boulder's ball (MBOULDR): dark basalt cracked over lava, veins widening into molten zones, all in model space.
// The md3 is a ball squashed 1/1.2 round its origin (drawn round); the surface rolls here, about the sideways axis.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define MB_TOWORLD 0.8     // md3 units to world units (the actor's Scale); every size below is in world units
#define MB_CELL    19.0    // between the crust plates' centres
#define MB_HAIR    7.0     // between the hairline cracks' cells
#define MB_ROLL    1.66    // turns a second: a true roll at the boulder's ~14 units a tic
#define TAU        6.2831853

vec3 mb_hash(vec3 c) {
	vec3 p = fract(mod(c, 256.0) * vec3(0.1031, 0.1030, 0.0973));
	p += dot(p, p.yxz + 33.33);
	return fract((p.xxy + p.yxx) * p.zyx);
}

// Value noise, its eight corners through one loop.
float mb_noise(vec3 x) {
	vec3 i = floor(x), f = x - i;
	vec3 u = f * f * (3.0 - 2.0 * f);
	float n = 0.0;
	for(int k = 0; k < 8; k++) {
		vec3 o = vec3(mod(float(k), 2.0), mod(floor(float(k) / 2.0), 2.0), floor(float(k) / 4.0));
		vec3 w = mix(1.0 - u, u, o);
		n += w.x * w.y * w.z * mb_hash(i + o).x;
	}
	return n;
}

float mb_fbm(vec3 x) {
	float s = 0.0;
	for(int o = 0; o < 3; o++) {
		float f = o == 0 ? 1.0 : (o == 1 ? 2.07 : 4.13);
		float a = o == 0 ? 0.0 : (o == 1 ? 3.1 : 7.7);
		s += (o == 0 ? 0.55 : (o == 1 ? 0.3 : 0.15)) * mb_noise(x * f + a);
	}
	return s;
}

// The exact distance to the wall between the two nearest cells: (f2 - f1) / 2|b - a| on squared distances.
float mb_wall(vec3 p) {
	vec3 b = floor(p), a1 = p, a2 = p;
	float f1 = 9.0, f2 = 9.0;
	for(int k = 0; k < 27; k++) {
		vec3 c = b + vec3(mod(float(k), 3.0), mod(floor(float(k) / 3.0), 3.0), floor(float(k) / 9.0)) - 1.0;
		vec3 fp = c + mb_hash(c);
		vec3 q = fp - p;
		float d = dot(q, q);
		if(d < f1) {
			f2 = f1; a2 = a1;
			f1 = d; a1 = fp;
		}
		else if(d < f2) {
			f2 = d; a2 = fp;
		}
	}
	return (f2 - f1) / max(2.0 * length(a2 - a1), 0.0001);
}

// Ember red, orange, yellow, a warm near-white at the hottest.
vec3 mb_ramp(float h) {
	h = clamp(h, 0.0, 1.0);
	vec3 c = mix(vec3(0.06, 0.01, 0.0), vec3(0.45, 0.04, 0.0), clamp(h / 0.25, 0.0, 1.0));
	c = mix(c, vec3(0.95, 0.30, 0.02), clamp((h - 0.25) / 0.25, 0.0, 1.0));
	c = mix(c, vec3(1.0, 0.60, 0.10), clamp((h - 0.5) / 0.22, 0.0, 1.0));
	c = mix(c, vec3(1.0, 0.85, 0.40), clamp((h - 0.72) / 0.16, 0.0, 1.0));
	return mix(c, vec3(1.0, 0.97, 0.80), clamp((h - 0.88) / 0.12, 0.0, 1.0));
}

vec4 Process(vec4 color) {
	// The point in the ball's own space (matrix 7 holds its placement and facing), unsquashed, from its centre.
	vec3 c0 = gl_TextureMatrix[7][0].xyz, c1 = gl_TextureMatrix[7][1].xyz, c2 = gl_TextureMatrix[7][2].xyz;
	vec3 x12 = cross(c1, c2), x20 = cross(c2, c0), x01 = cross(c0, c1);
	vec3 w = pixelpos.xyz - gl_TextureMatrix[7][3].xyz;
	vec3 m = vec3(dot(x12, w), dot(x20, w), dot(x01, w)) / dot(c0, x12);
	m.y *= 1.2;
	vec3 n = normalize(m);
	vec3 nw = normalize(n.x * x12 + n.y * x20 + n.z * x01);
	vec3 eye = normalize(camerapos - pixelpos.xyz);
	float t = mod(timer, 512.0);
	// The material point: the surface turned back by the roll (x forward, y up), so the cracks travel over the top forward.
	float ra = TAU * fract(t * MB_ROLL), cr = cos(ra), sr = sin(ra);
	vec3 p = vec3(m.x * cr - m.y * sr, m.x * sr + m.y * cr, m.z) * MB_TOWORLD;

	// Every fbm through one call site: the edges' warp (3), the crust's grain, the hot zones, the hairlines, the plates.
	float wx = 0.0, wy = 0.0, wz = 0.0, grain = 0.0, hotn = 0.0, hair = 0.0, platen = 0.0;
	for(int j = 0; j < 7; j++) {
		vec3 x = j < 3 ? p * 0.07 + (j == 0 ? 1.0 : (j == 1 ? 5.0 : 9.0)) : (j == 3 ? p * 0.22 : (j == 4 ? p * 0.05 + 11.0
			: (j == 5 ? p * 0.045 + 21.0 : p * 0.2 + vec3(0.0, 0.0, t * 0.5))));
		float v = mb_fbm(x);
		wx = j == 0 ? v : wx; wy = j == 1 ? v : wy; wz = j == 2 ? v : wz;
		grain = j == 3 ? v : grain; hotn = j == 4 ? v : hotn; hair = j == 5 ? v : hair; platen = j == 6 ? v : platen;
	}

	// Crust: dark basalt with grain, lit from above in the world, so the ball reads round while it rolls.
	vec3 rgb = vec3(0.11, 0.075, 0.065) * (0.6 + 0.8 * grain);
	rgb *= 0.45 + 0.85 * max(dot(nw, normalize(vec3(-0.35, 0.85, 0.4))), 0.0);

	// One connected fracture network: every plate edge cracks, its width set by smooth noise, the edges bent by a soft
	// warp; finer hairlines branch between them, fading in and out. In the hot zones the cracks open into molten veins.
	vec3 q = p + 6.0 * (vec3(wx, wy, wz) - 0.5);
	float hot = clamp((hotn - 0.52) / 0.14, 0.0, 1.0);
	float sw1 = 0.0, sw2 = 0.0;
	for(int lv = 0; lv < 2; lv++) {
		float sz = lv == 0 ? MB_CELL : MB_HAIR;
		float d = mb_wall(q / sz + (lv == 0 ? 0.0 : 31.0)) * sz;
		sw1 = lv == 0 ? d : sw1;
		sw2 = lv == 1 ? d : sw2;
	}
	float wn = mb_noise(q * 0.055 + 3.0);
	float width1 = 0.45 + 1.25 * wn * wn + hot * 2.2;
	float crack1 = 1.0 - clamp((sw1 - width1 * 0.3) / (width1 * 0.7 + 0.001), 0.0, 1.0);
	float width2 = 0.32 * clamp((hair - 0.5) / 0.14, 0.0, 1.0);
	float crack2 = (1.0 - clamp((sw2 - width2 * 0.3) / (width2 * 0.7 + 0.001), 0.0, 1.0)) * step(0.02, width2);
	float lip = 1.0 - clamp((sw1 - width1) / (width1 * 1.2 + 0.001), 0.0, 1.0);

	// The heat breathes slowly and flickers a little; the plates in a hot zone glow up through the crust.
	float breath = 0.5 + 0.5 * sin(TAU * fract(t * 0.35));
	float flick = mb_noise(p * 0.05 + vec3(0.0, t * 1.3, 0.0));
	float core = 0.6 + 0.25 * breath + 0.25 * (flick - 0.5) + 0.25 * hot + 0.12 * wn;
	float heat = max(crack1 * core, crack2 * (0.36 + 0.12 * breath)) + 0.14 * lip * (1.0 - crack1);
	float plate = hot * hot * clamp(sw1 / (MB_CELL * 0.4), 0.0, 1.0) * (0.25 + 0.2 * platen);
	heat = max(heat, plate * (0.85 + 0.3 * breath));

	// The crust is lit by the room; the lava and the faint heat at the rim are not.
	vec3 glow = mb_ramp(heat) * clamp(heat * 1.6, 0.0, 1.0);
	float rim = 1.0 - clamp(dot(nw, eye), 0.0, 1.0);
	rim = rim * rim * rim;
	rgb = rgb * color.rgb * (1.0 - 0.85 * clamp(heat * 1.5, 0.0, 1.0)) + glow
		+ vec3(0.5, 0.12, 0.02) * rim * 0.35 * (0.8 + 0.2 * breath);
	return vec4(rgb, color.a);
}
