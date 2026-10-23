// Rockslide's boulder, bound to ROCKSLGW (Shrapnel's skin under its own name): the rock cracked over a dim molten core.
// Rock and cracks both live in model space (matrix 7), so nothing breaks at a facet's edge and both tumble with it.

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define CELL     14.0   // md3 units between the big cracks' plate centres
#define WIDTH    0.32   // a crack's half width along the surface, in md3 units
#define KEEP     0.75   // the share of cell walls that crack at all
#define EMBER    0.22   // the glow at rest, well under the Rock Golem's
#define PULSE    0.32   // and what a pulse adds as it sweeps across the rock
#define PERIOD   1.6    // seconds a pulse
#define TEXSCALE 80.0   // md3 units the skin's width spans

vec3 rs_hash(vec3 c) {
	return fract(sin(vec3(dot(c, vec3(127.1, 311.7, 74.7)), dot(c, vec3(269.5, 183.3, 246.1)), dot(c, vec3(113.5, 271.9, 124.6)))) * 43758.5453);
}

// The distance to the wall between the nearest two cells, signed by side so it runs smoothly through zero there.
// pick is that wall's own die roll, the same from either side, so a wall is a crack or not along its whole length.
float rs_wall(vec3 p, out float pick) {
	vec3 b = floor(p), n1 = b, n2 = b;
	float f1 = 8.0, f2 = 8.0;
	for(int i = -1; i <= 1; ++i)
		for(int j = -1; j <= 1; ++j)
			for(int k = -1; k <= 1; ++k) {
				vec3 c = b + vec3(float(i), float(j), float(k));
				vec3 q = c + rs_hash(c) - p;
				float d = dot(q, q);
				if(d < f1) {
					f2 = f1;
					n2 = n1;
					f1 = d;
					n1 = c;
				}
				else if(d < f2) {
					f2 = d;
					n2 = c;
				}
			}
	pick = fract(sin(dot(n1 + n2, vec3(12.9898, 78.233, 37.719))) * 43758.5453);
	return (sqrt(f2) - sqrt(f1)) * 0.5 * sign(dot(n1 - n2, vec3(1.0, 57.0, 113.0)));
}

vec4 Process(vec4 color) {
	// The point in the model's own space: matrix 7 holds its whole placement, the tumble included.
	vec3 c0 = gl_TextureMatrix[7][0].xyz, c1 = gl_TextureMatrix[7][1].xyz, c2 = gl_TextureMatrix[7][2].xyz;
	vec3 x12 = cross(c1, c2), x20 = cross(c2, c0), x01 = cross(c0, c1);
	vec3 w = pixelpos.xyz - gl_TextureMatrix[7][3].xyz;
	vec3 p = vec3(dot(x12, w), dot(x20, w), dot(x01, w)) / dot(c0, x12);
	vec3 a = dFdx(p), b = dFdy(p);

	// The skin projected three ways and blended by the facet's facing, instead of the md3's window per facet.
	vec3 n = normalize(cross(a, b));
	vec3 wt = pow(abs(n), vec3(4.0));
	wt /= wt.x + wt.y + wt.z;
	vec2 sc = vec2(1.0 / TEXSCALE, 2.0 / TEXSCALE);
	vec3 rock = getTexel(p.yz * sc).rgb * wt.x + getTexel(p.xz * sc).rgb * wt.y + getTexel(p.xy * sc).rgb * wt.z;

	// The windows' baked top light, done for real: the facet's world facing against a fixed light from above.
	vec3 nw = normalize(n.x * x12 + n.y * x20 + n.z * x01);
	nw *= sign(dot(nw, camerapos - pixelpos.xyz));
	rock *= 0.55 + 0.75 * max(dot(nw, normalize(vec3(-0.35, 1.0, 0.25))), 0.0);

	// Straight walls with a slight roughness, so the cracks run like fractures rather than curves; only some walls crack.
	vec3 q = p + 0.12 * sin(p.yzx * 2.3 + p.zxy * 1.1);
	float pick;
	float sw = rs_wall(q / CELL, pick) * CELL;

	// Along the surface: the wall's distance over its slope across the facet, so a crack keeps one width however the wall
	// meets the facet, and a wall lying along a facet shows nothing instead of a smear.
	float aa = dot(a, a), ab = dot(a, b), bb = dot(b, b), sx = dFdx(sw), sy = dFdy(sw);
	float det = max(aa * bb - ab * ab, 1e-9);
	vec3 g = ((sx * bb - sy * ab) * a + (sy * aa - sx * ab) * b) / det;
	// Never under the straight distance: a 2x2 block across a facet edge mixes two facets and would fake a crack there.
	float d = max(abs(sw) / max(length(g), 0.05), abs(sw));

	// Each crack waxes and wanes along its length, so it tapers to points rather than stopping at a junction.
	vec3 dir = normalize(rs_hash(vec3(pick * 91.0, pick * 17.0, 3.0)) - 0.5);
	float width = WIDTH * smoothstep(0.1, 0.5, 0.5 + 0.5 * sin(dot(q, dir) * 0.3 + pick * 6.2832)) * step(pick, KEEP);
	float crack = 1.0 - smoothstep(width * 0.3, width + 0.001, d);
	float lip = (1.0 - smoothstep(width, width * 2.4 + 0.001, d)) * step(0.001, width);

	float ph = fract(timer / PERIOD + 0.03 * (p.x + p.y + p.z));
	float s = ph < 0.4 ? sin(3.14159265 * ph / 0.4) : 0.0;
	float i = crack * (EMBER + PULSE * s * s);
	// The Rock Golem's ramp: Upheaval's orange, warming to yellow only at the hottest.
	float hot = smoothstep(0.7, 1.6, i);
	vec3 glow = vec3(1.0, 0.42 + 0.4 * hot, 0.08 + 0.3 * hot * hot) * min(1.0, i * 1.4);

	// color carries the light alone: the rock is lit and charred at the lips, the glow is not.
	return vec4(rock * (1.0 - 0.4 * lip) * (1.0 - 0.6 * crack) * color.rgb + glow, color.a);
}
