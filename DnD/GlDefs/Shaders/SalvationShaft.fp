// Salvation's shaft over a redeemed corpse: Holy Wrath's pillar made gentle, white at its heart, pale gold at its rim,
// slow motes of light drifting down. HWPILLAR.md3 under its own skin; the ray's chord through the cylinder, as there.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define R0      48.0    // HWPILLAR.md3's radius before the actor's X scale
#define FALL    60.0    // units a second the streaks drift down
#define STREAKS 9.0     // streaks round the pillar (whole, so the noise wraps)
#define TOPFADE 240.0   // the light gathers over this many units down from the top
#define NEAR0   20.0    // map units from the eye where it starts to show, for a pillar on the caster
#define NEAR1   64.0
#define STEP    0.002   // the Y scale's step per stage of the descent (20 stages), as the actors set it
#define REACH   420.0   // how far below the top the light reaches at the last stage; the floor is H0 below it
#define H0      384.0
#define FRONT   80.0    // the soft front of the descending light
#define TAU     6.2831853

float hp_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

// Value noise wrapping every per along x, so the streaks close round the pillar.
float hp_noise(vec2 x, float per) {
	vec2 i = floor(x);
	vec2 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float x0 = mod(i.x, per);
	float x1 = mod(i.x + 1.0, per);
	float a = hp_hash(vec3(x0, i.y, 3.0));
	float b = hp_hash(vec3(x1, i.y, 3.0));
	float c = hp_hash(vec3(x0, i.y + 1.0, 3.0));
	float d = hp_hash(vec3(x1, i.y + 1.0, 3.0));
	return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

vec4 Process(vec4 color) {
	vec3 o = gl_TextureMatrix[7][3].xyz;
	float R = R0 * length(gl_TextureMatrix[7][0].xyz);
	vec3 e = camerapos - o;
	vec3 d = normalize(pixelpos.xyz - camerapos);
	float t = mod(timer, 512.0);

	// The ray against the upright cylinder (GL y is up): outer shell from outside, inner shell from within.
	vec2 e2 = e.xz;
	vec2 d2 = d.xz;
	float qa = max(dot(d2, d2), 1e-5);
	float qb = dot(e2, d2);
	float qc = dot(e2, e2) - R * R;
	if((gl_TexCoord[0].t < 1.0) == (qc < 0.0))
		discard;
	float disc = qb * qb - qa * qc;
	if(disc <= 0.0)
		discard;
	float sq = sqrt(disc);
	float t0 = max((-qb - sq) / qa, 0.0);
	float t1 = (-qb + sq) / qa;
	if(t1 <= t0)
		discard;

	// Its nearest pass to the axis: how deep into the light it goes, and where.
	float tm = clamp(-qb / qa, t0, t1);
	vec3 m = e + d * tm;
	float dm = length(m.xz) / R;
	float glow = (t1 - t0) / (2.0 * R) * exp(-dm * dm * 2.0);
	// The descent: the light reaches down to a soft, ragged front; the stage rides on the Y scale (base 1).
	float sy = length(gl_TextureMatrix[7][1].xyz);
	float base = sy < 0.95 ? 0.8333 : (sy < 1.1 ? 1.0 : 1.2);
	float k = clamp(floor((sy / base - 1.0) / STEP + 0.5), 0.0, 20.0);

	// Streaks pouring down, finer ones over broad ones.
	// round the pillar at the shell point this pixel lies on: the axis-nearest point flips sides across the middle
	vec3 sp = pixelpos.xyz - o;
	float ang = atan(sp.z, sp.x + 1e-4) / TAU + 0.5;
	float n = hp_noise(vec2(ang * STREAKS, (m.y + t * FALL) / 110.0), STREAKS);
	float n2 = hp_noise(vec2(ang * STREAKS * 3.0, (m.y + t * FALL * 1.4) / 45.0), STREAKS * 3.0);
	float streak = 0.35 + 0.95 * n * n + 0.45 * n2 * n2;
	// Only the share of the ray's path through the cylinder that lies between the top and the light's reach (never
	// under the floor) is lit: the ray climbs or drops along its chord, so one point on it would leak light.
	float reach = k / 20.0 * REACH + (n - 0.5) * 40.0;
	float lim = min(reach, H0);
	float b0 = -(e.y + d.y * t0);
	float b1 = -(e.y + d.y * t1);
	float lo = max(min(b0, b1), 0.0);
	float hi = min(max(b0, b1), lim);
	float span = abs(b1 - b0);
	float share = span < 1.0 ? step(0.0, b0) * step(b0, lim) : max(hi - lo, 0.0) / span;
	float below = 0.5 * (lo + hi);
	float top = share * smoothstep(0.0, TOPFADE, below);
	if(reach < H0)
		top *= 1.0 - smoothstep(reach - FRONT, reach, below);

	vec3 rim = vec3(0.75, 0.55, 0.22);
	vec3 mid = vec3(1.0, 0.85, 0.5);
	// the white heart warms toward orange as the pillar fades, so it never dims to a cold grey
	vec3 hot = mix(mid, vec3(1.0, 0.9, 0.62), color.a);
	vec3 col = mix(rim, mid, smoothstep(1.0, 0.55, dm));
	col = mix(col, hot, smoothstep(0.55, 0.1, dm));
	col *= glow * (0.6 + 0.4 * streak) * 0.8;
	float fade = smoothstep(NEAR0, NEAR1, length(pixelpos.xyz - camerapos));
	// faded before a soft roll-off, so a dimming heart keeps its warmth instead of clipping to white then greying
	col *= top * fade * color.a;
	return vec4(1.0 - exp(-col * 1.4), 1.0);
}
