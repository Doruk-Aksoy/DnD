// Geyser's rock pile, bound to GYROCK: the skin wrapped three ways in model space and lit per facet, so the rocks show
// no UV seams; green light rising off the acid inside the mouth, and a sulfur crust where it spatters the rim.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define TEXSCALE 48.0   // md3 units the skin spans
#define MOUTH    11.0   // the rim's radius
#define POOLZ    20.4   // the acid's edge and the rim, as md3 heights (world x1.2)
#define RIMZ     24.0
#define GLOW     0.6

vec4 Process(vec4 color) {
	// The point in the model's own space, GL order: md3 x, md3 z (up), md3 y.
	vec3 c0 = gl_TextureMatrix[7][0].xyz, c1 = gl_TextureMatrix[7][1].xyz, c2 = gl_TextureMatrix[7][2].xyz;
	vec3 x12 = cross(c1, c2), x20 = cross(c2, c0), x01 = cross(c0, c1);
	vec3 w = pixelpos.xyz - gl_TextureMatrix[7][3].xyz;
	vec3 p = vec3(dot(x12, w), dot(x20, w), dot(x01, w)) / dot(c0, x12);

	vec3 n = normalize(cross(dFdx(p), dFdy(p)));
	vec3 wt = pow(abs(n), vec3(4.0));
	wt /= wt.x + wt.y + wt.z;
	vec3 rock = getTexel(p.zy / TEXSCALE).rgb * wt.x + getTexel(p.xz / TEXSCALE).rgb * wt.y + getTexel(p.xy / TEXSCALE).rgb * wt.z;

	vec3 nw = normalize(n.x * x12 + n.y * x20 + n.z * x01);
	nw *= sign(dot(nw, camerapos - pixelpos.xyz));
	float lit = 0.5 + 0.75 * max(dot(nw, normalize(vec3(-0.35, 1.0, 0.25))), 0.0);

	float r = length(p.xz), h = p.y;
	float lum = dot(rock, vec3(0.3, 0.59, 0.11));

	// Sulfur crust on the rim's brightest stone, picked by the stone's own texture so it follows no pattern.
	float rim = (1.0 - smoothstep(MOUTH + 1.0, MOUTH + 8.0, r)) * smoothstep(RIMZ - 9.0, RIMZ - 2.0, h);
	float crust = rim * smoothstep(0.24, 0.38, lum);
	rock = mix(rock, vec3(0.5, 0.55, 0.2) * (0.6 + 1.4 * lum), crust * 0.45);

	// Light off the acid: strongest on the inner wall just over it, flickering as it churns.
	float inner = 1.0 - smoothstep(MOUTH - 1.5, MOUTH + 3.0, r);
	float glow = inner * exp(-max(h - POOLZ, 0.0) / 4.0) * smoothstep(POOLZ - 3.0, POOLZ, h);
	glow *= 0.82 + 0.18 * sin(timer * 4.7 + atan(p.z, p.x)) * sin(timer * 2.3);
	vec3 green = vec3(0.32, 1.0, 0.1) * glow * GLOW;

	// color carries the light alone: the rock is lit, the acid's light is not.
	return vec4(rock * lit * color.rgb + green, color.a);
}
