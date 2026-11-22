// Holy Prayers' aura on flat.md3, bound to HPRAYER. Masks: R the prayer script, G the lines, B the beads.
// A counting light walks round the beads and the script; the rays and the middle stay still.

uniform float timer;

#define WALK    0.07   // turns a second the counting light walks round
#define HEADS   2.0    // lights at once, evenly spaced
#define TRAIL   7.0    // how fast a bead dims behind the light
#define GAIN    1.9
#define TAU     6.2831853

float hp_smooth(float a, float b, float x) {
	float t = clamp((x - a) / (b - a), 0.0, 1.0);
	return t * t * (3.0 - 2.0 * t);
}

float hp_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Value noise wrapping every per, for angles round the disc.
float hp_noise(float x, float per) {
	float i = floor(x);
	float f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float a = hp_hash(vec3(mod(i, per), 0.0, 7.0));
	float b = hp_hash(vec3(mod(i + 1.0, per), 0.0, 7.0));
	return mix(a, b, f);
}

vec4 Process(vec4 color) {
	vec2 st = gl_TexCoord[0].st;
	vec2 p = st - 0.5;
	float r = length(p) * 2.0;
	float mask = 1.0 - hp_smooth(0.93, 1.0, r);
	if(mask <= 0.0)
		return vec4(0.0);

	vec4 m = getTexel(st);
	float t = mod(timer, 512.0);
	float ang = atan(p.y, p.x) / TAU + 0.5;
	float dd = fract((t * WALK - ang) * HEADS);
	float walk = exp(-dd * TRAIL) * (1.0 - hp_smooth(0.96, 1.0, dd));
	float ray = hp_noise(ang * 28.0, 28.0);
	float rays = ray * ray * ray * hp_smooth(0.12, 0.3, r) * (1.0 - hp_smooth(0.44, 0.7, r));
	float core = exp(-(r / 0.22) * (r / 0.22));
	float wash = hp_smooth(0.3, 0.68, r) * (1.0 - hp_smooth(0.72, 0.78, r));

	vec3 col = vec3(1.0, 0.76, 0.34) * m.r * (0.45 + 0.9 * walk) + vec3(1.0, 0.7, 0.3) * m.g * 0.75 +
		vec3(1.0, 0.93, 0.74) * m.b * (0.4 + 1.5 * walk) + vec3(0.95, 0.85, 0.5) * (rays * 0.4 + core * 0.12 + wash * 0.06);
	return vec4(col * GAIN * mask * color.rgb, mask * color.a);
}
