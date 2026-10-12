// Glacial Cascade's crystals. Bound to GCASCADE, a copy of Glacial Spike's ice under its own name.
// Each flat facet is lit in world space off the engine's pixel positions, so every face glints on its own.

// NO_FOG marks the colormap pass, whose base declares neither. See .claude/notes/bcs-acs-gotchas.md.
#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define SIDES       6.0    // GCASCADE.md3's facets round; the edge lines are drawn from them
#define TIP_T       0.75   // where the tip starts along t, as the generator builds it
#define BODY        0.60
#define CORE        0.40   // how dark the ice is, looking straight through a face
#define EDGE_W      0.075  // edge line width, as a share of a facet
#define EDGE        0.60
#define RIM         0.50
#define RIM_POW     2.0
#define KEY         0.80   // a light from overhead, so each facet catches it differently
#define KEY_TIGHT   22.0
#define CAM         0.70   // and one just above the viewer, so a face turned to you always shines
#define CAM_TIGHT   48.0
#define LIFT        0.70
#define FROST       0.35   // the frosted base, where it broke out of the floor
#define TIP         0.25
#define SPARK       1.20
#define SPARK_RATE  2.4
#define SPARK_SHARE 0.25

float gc_hash(vec2 p) {
	return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	vec3 p = pixelpos.xyz;

	// camerapos may arrive in either axis order; the right one sits about pixelpos.w away.
	vec3 c1 = camerapos;
	vec3 c2 = camerapos.xzy;
	float e1 = abs(distance(p, c1) - pixelpos.w);
	float e2 = abs(distance(p, c2) - pixelpos.w);
	vec3 v = normalize((e1 < e2 ? c1 : c2) - p);
	// Neither fitting means the positions are not world space: no glints, just the frosted ice.
	float trust = 1.0 - smoothstep(1.5, 3.0, min(e1, e2) / max(pixelpos.w, 1.0));

	// The facet's own normal, turned toward the viewer.
	vec3 n = cross(dFdx(p), dFdy(p));
	n /= max(length(n), 1e-9);
	if(dot(n, v) < 0.0)
		n = -n;
	float face = mix(0.6, clamp(dot(n, v), 0.0, 1.0), trust);
	float edge = 1.0 - face;

	vec3 art = getTexel(vec2(uv.s * 2.0, uv.t * 2.5 - timer * 0.02)).rgb;
	float ice = dot(art, vec3(0.299, 0.587, 0.114));
	vec3 col = mix(vec3(0.02, 0.09, 0.28), vec3(0.45, 0.78, 1.00), ice) * BODY * mix(CORE, 1.0, edge);

	// Crystal edges: across a side facet u runs 0..1; on a tip facet the sides close in on the apex.
	float u = fract(uv.s * SIDES);
	float ed = min(u, 1.0 - u);
	if(uv.t > TIP_T) {
		float hw = 0.5 * (1.0 - (uv.t - TIP_T) / (1.0 - TIP_T));
		ed = (hw - abs(u - 0.5)) / max(hw * 2.0, 0.0001);
	}
	ed = min(ed, abs(uv.t - TIP_T) * 3.0);
	col += vec3(0.80, 0.95, 1.00) * EDGE * (1.0 - smoothstep(0.0, EDGE_W, ed)) * (0.45 + 0.55 * edge);

	// The glassy rim, then glints from overhead and from just above the viewer.
	col += vec3(0.45, 0.80, 1.00) * RIM * pow(edge, RIM_POW) * trust;
	vec3 hk = normalize(normalize(vec3(0.35, 1.0, 0.25)) + v);
	vec3 hc = normalize(normalize(v + vec3(0.0, LIFT, 0.0)) + v);
	float sp = KEY * pow(max(dot(n, hk), 0.0), KEY_TIGHT) + CAM * pow(max(dot(n, hc), 0.0), CAM_TIGHT);
	col += vec3(0.92, 0.98, 1.00) * sp * trust;

	col = mix(col, vec3(0.85, 0.93, 1.00) * (0.4 + 0.6 * ice), (1.0 - smoothstep(0.06, 0.25, uv.t)) * FROST);
	col += vec3(0.40, 0.70, 1.00) * TIP * pow(uv.t, 4.0);

	// Twinkles, a few cells at a time.
	vec2 q = vec2(uv.s * 12.0, uv.t * 8.0);
	vec2 cell = floor(q);
	float hc2 = gc_hash(cell);
	if(hc2 < SPARK_SHARE) {
		vec2 d = fract(q) - 0.5;
		float tw = pow(max(0.0, sin(timer * SPARK_RATE * (0.6 + hc2) + hc2 * 40.0)), 10.0);
		col += vec3(0.90, 0.97, 1.00) * SPARK * tw * exp(-dot(d, d) * 40.0);
	}

	return vec4(col * color.rgb, color.a);
}
