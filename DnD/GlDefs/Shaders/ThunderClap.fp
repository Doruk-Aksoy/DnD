// Thunder Clap's shockwave ring. Bound to TCLAPRNG; the art is only the shaderless fallback.
// ACS grows the quad and the actor's fade dims it; a bright wobbling front with crackling knots, glow trailing inward.

uniform float timer;

#define FRONT    0.92   // where the leading edge sits, in disc radii
#define FRONT_W  0.022
#define TRAIL    0.14   // how far the glow trails in behind it
#define KNOTS    2.6
#define WASH     0.10

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;
	if(r >= 1.0)
		return vec4(0.0);

	// Whole multiples of the angle, so the wobble and the knots have no seam.
	float ang = atan(p.y, p.x);
	float edge = FRONT + 0.005 * sin(ang * 13.0 + timer * 9.0) + 0.004 * sin(ang * 29.0 - timer * 17.0);
	float d = r - edge;
	float front = exp(-d * d / (FRONT_W * FRONT_W));
	float trail = d < 0.0 ? exp(d / TRAIL) : exp(-d / 0.02);
	float knots = pow(0.5 + 0.5 * sin(ang * 23.0 + timer * 11.0) * sin(ang * 7.0 - timer * 6.0), 3.0);

	vec3 col = vec3(0.85, 0.93, 1.00) * front * (0.8 + KNOTS * knots) +
		vec3(0.25, 0.50, 1.00) * trail * 0.6 +
		vec3(0.10, 0.20, 0.60) * smoothstep(0.2, edge, r) * WASH;

	float mask = 1.0 - smoothstep(0.97, 1.0, r);
	return vec4(col * mask * color.rgb, mask * color.a);
}
