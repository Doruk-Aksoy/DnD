// Ice Nova's ring. Bound to ICENOVA in GlDefs/Shaders.txt. The texture is a flat sheet of scratched
// ice; the circle, the ring and its teeth are all made here.

uniform float timer;

#define TEETH_N      34.0   // teeth around the rim
#define TOOTH_SHARP   2.2   // >1 narrows each one into a point
#define TOOTH_DEPTH  0.085  // how far the tallest tips stand out
#define TOOTH_LOW    0.30   // shortest tooth as a fraction of the tallest
#define TOOTH_SHIMMER 0.15  // smooth wobble on top, so static teeth still glitter
#define BASE_R       0.915  // the ring's own edge; tips reach 1.0, which IS the blast radius
#define EDGE_SOFT    0.016
#define RIM          0.34   // width of the bright leading band, inward from the edge
#define BODY         0.72   // and what survives behind it -- the inside is a sheet, not a hoop
#define INNER_DIM    0.80   // the middle recedes a little, but stays lit
#define TEX_TILE      2.0   // repeats of the ice grain across the quad
#define TEX_INSET    0.62   // sample the middle of the sheet only; its edges are vignetted
#define ICE_FLOOR    0.78   // opacity where the grain is darkest -- NOT 0, or it reads as a hole
#define RIMGLOW      0.80
#define GAIN         1.10

float novaHash(float n) {
	return fract(sin(n * 127.1) * 43758.5453);
}

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;
	float th = atan(p.y, p.x);

	// Per TOOTH rather than per angle: each one gets its own height and its own lean, so the rim
	// is asymmetric instead of a cog. mod keeps tooth 0 and tooth N-1 from meeting as a seam.
	float tn = (th * 0.15915494 + 0.5) * TEETH_N;
	float ti = mod(floor(tn), TEETH_N);
	float tf = tn - floor(tn);

	float lean = 0.25 + 0.5 * novaHash(ti + 37.0);
	float tri = tf < lean ? tf / lean : (1.0 - tf) / (1.0 - lean);
	float tooth = pow(max(tri, 0.0), TOOTH_SHARP) * (TOOTH_LOW + (1.0 - TOOTH_LOW) * novaHash(ti));

	tooth *= 1.0 - TOOTH_SHIMMER + TOOTH_SHIMMER * sin(th * 13.0 + timer * 2.0);

	float edge = BASE_R + TOOTH_DEPTH * tooth;
	if(r > edge)
		return vec4(0.0, 0.0, 0.0, 0.0);

	float band = smoothstep(edge - RIM, edge, r);
	float body = BODY + (1.0 - BODY) * band;

	// Softened only at the very edge, so the teeth read as solid ice and not as a glow.
	body *= smoothstep(edge, edge - EDGE_SOFT, r);

	// Cartesian, not polar: sampling r down the texture's y stretched the grain into long radial
	// streaks, and the dark ones went fully transparent under additive blending.
	vec2 iuv = fract(p * TEX_TILE + 0.5) * TEX_INSET + (1.0 - TEX_INSET) * 0.5;
	float ice = dot(getTexel(iuv).rgb, vec3(0.299, 0.587, 0.114));

	vec3 col = mix(vec3(0.34, 0.56, 0.82), vec3(0.72, 0.92, 1.0), ice);
	col *= INNER_DIM + (1.0 - INNER_DIM) * band;
	col += vec3(0.34, 0.62, 0.96) * RIMGLOW * band * band;

	return vec4(col * GAIN * color.rgb, body * (ICE_FLOOR + (1.0 - ICE_FLOOR) * ice) * color.a);
}
