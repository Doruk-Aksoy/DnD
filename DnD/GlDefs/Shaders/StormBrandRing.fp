// Storm Brand's rune circle round its globe. Bound to STMRUNE.
// The rune band turns one way and the ticks and dots inside it the other; a highlight chases round the rim.

uniform float timer;

#define SPIN_OUT  0.40
#define SPIN_IN  -0.65
#define SPLIT     0.695  // disc radius between the band's inner rim (0.72) and the ticks (0.67)
#define SWEEP     0.90
#define SWEEP_W   6.0    // higher is a narrower highlight

vec2 rr_spin(vec2 p, float a) {
	float s = sin(a);
	float c = cos(a);
	return vec2(p.x * c - p.y * s, p.x * s + p.y * c);
}

vec4 Process(vec4 color) {
	vec2 p = gl_TexCoord[0].st - 0.5;
	float r = length(p) * 2.0;
	if(r >= 1.0)
		return vec4(0.0);

	vec3 tex = getTexel(0.5 + rr_spin(p, timer * (r > SPLIT ? SPIN_OUT : SPIN_IN))).rgb;

	// sin of (angle - phase) has the angle's own period, so it is seamless across atan's wrap.
	float sweep = pow(0.5 + 0.5 * sin(atan(p.y, p.x) - timer * 2.4), SWEEP_W);
	float pulse = 0.85 + 0.15 * sin(timer * 4.3);

	vec3 col = tex * (pulse + sweep * SWEEP);
	return vec4(col * color.rgb, color.a);
}
