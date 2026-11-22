// Divine Judgement's sword: dark blued steel and gold lit per facet, runes glowing gold down the blade. The actor's
// X scale carries its stage: 1 + k x STEP, k 0..10 the runes flaring, 10..20 the sword burning away from the tip.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define STEP    0.002   // the X scale's step per stage, as the actors set it
#define AMBIENT 0.45    // facet light: what every face gets, and the key light from above
#define KEY     0.75
#define SPEC    0.9     // the glint on steel and gold
#define GLOW    vec3(1.0, 0.78, 0.36)
#define RISE    0.35    // the runes' slow pulse, blade lengths a second, running toward the hilt
#define BURN    vec3(1.0, 0.85, 0.5)

float js_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

float js_noise(vec2 x) {
	vec2 i = floor(x);
	vec2 f = x - i;
	f = f * f * (3.0 - 2.0 * f);
	float a = js_hash(vec3(i, 1.0));
	float b = js_hash(vec3(i + vec2(1.0, 0.0), 1.0));
	float c = js_hash(vec3(i + vec2(0.0, 1.0), 1.0));
	float d = js_hash(vec3(i + vec2(1.0, 1.0), 1.0));
	return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec4 tx = getTexel(uv);
	float t = mod(timer, 512.0);
	float k = floor((length(gl_TextureMatrix[7][0].xyz) - 1.0) / STEP + 0.5);
	float flare = clamp(k / 10.0, 0.0, 1.0);
	float burn = clamp((k - 10.0) / 10.0, 0.0, 1.0);

	// Burning away: the blade's tip first (v is 0 there), a ragged gold edge ahead of the hole.
	float n = js_noise(uv * vec2(24.0, 60.0)) * 0.6 + js_noise(uv * vec2(7.0, 14.0)) * 0.4;
	float h = uv.y * 0.55 + n * 0.45;
	float cut = burn * 1.15;
	if(burn > 0.0 && h < cut)
		discard;
	float edge = burn > 0.0 ? 1.0 - smoothstep(0.0, 0.06, h - cut) : 0.0;

	// Per-facet light from the surface's own slope, turned toward the eye.
	vec3 dx = dFdx(pixelpos.xyz);
	vec3 dy = dFdy(pixelpos.xyz);
	vec3 nrm = normalize(cross(dx, dy));
	vec3 eye = normalize(camerapos - pixelpos.xyz);
	if(dot(nrm, eye) < 0.0)
		nrm = -nrm;
	vec3 key = normalize(vec3(0.35, 1.0, 0.25));
	float dif = AMBIENT + KEY * max(dot(nrm, key), 0.0);
	float spec = pow(max(dot(reflect(-key, nrm), eye), 0.0), 18.0) * SPEC;
	float rim = pow(1.0 - max(dot(nrm, eye), 0.0), 3.0) * 0.35;
	vec3 lit = tx.rgb * (dif + rim) * mix(color.rgb, vec3(1.0), 0.55) + vec3(1.0, 0.9, 0.7) * spec * max(tx.r, 0.25);

	// The runes and edges glow; a slow pulse runs up the blade and every rune flares before the lights leave.
	float g = clamp((1.0 - tx.a) / 0.84, 0.0, 1.0);
	float pulse = 0.75 + 0.25 * smoothstep(0.6, 1.0, fract(uv.y * 1.5 - t * RISE));
	vec3 glow = GLOW * g * (1.3 * pulse + flare * 1.2) + vec3(1.0) * smoothstep(0.7, 1.0, g) * (0.4 + 0.5 * flare);
	vec3 col = lit + glow + BURN * edge * 1.5;
	return vec4(min(col, vec3(1.0)), 1.0);
}
