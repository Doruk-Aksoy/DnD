// Glacial Cascade's floor. Bound to CSCFLOOR, a copy of Ice Nova's scratched ice under its own name.
// A glossy sheen across the ice and glints off its scratches, lit toward the camera in world space.

// NO_FOG marks the colormap pass, whose base declares neither. See .claude/notes/bcs-acs-gotchas.md.
#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define BODY        0.55
#define SCR_LO      0.60   // the scratches picked out as frost lines
#define SCR_HI      0.85
#define SCRATCH     0.45
#define BUMP        5.0    // how hard the scratches tilt the surface for the glints
#define BUMP_STEP   0.0065 // how far apart the bump samples sit, in texture units
#define SPEC        1.00
#define SPEC_TIGHT  60.0
#define SHEEN       0.75   // the gloss looking across the ice
#define SHEEN_POW   3.0
#define EDGE_IN     0.55   // where the patch starts thinning toward its ragged rim
#define RAGGED      0.55
#define SPARK       1.30
#define SPARK_RATE  2.6
#define SPARK_SHARE 0.22

float cf_lum(vec2 uv) {
	return dot(getTexel(uv).rgb, vec3(0.299, 0.587, 0.114));
}

float cf_hash(vec2 p) {
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
	// Neither fitting means the positions are not world space: no sheen or glints, just the ice.
	float trust = 1.0 - smoothstep(1.5, 3.0, min(e1, e2) / max(pixelpos.w, 1.0));

	// The floor is level, so up is the normal and the scratches tilt it; their axes need not match the world's.
	float ice = cf_lum(uv);
	float gu = cf_lum(uv + vec2(BUMP_STEP, 0.0)) - cf_lum(uv - vec2(BUMP_STEP, 0.0));
	float gv = cf_lum(uv + vec2(0.0, BUMP_STEP)) - cf_lum(uv - vec2(0.0, BUMP_STEP));
	vec3 n = normalize(vec3(-gu * BUMP, 1.0, -gv * BUMP));

	vec3 col = mix(vec3(0.01, 0.06, 0.20), vec3(0.40, 0.72, 1.00), ice) * BODY;
	col += vec3(0.70, 0.90, 1.00) * SCRATCH * smoothstep(SCR_LO, SCR_HI, ice);

	// Glossy looking across it, off the level floor so the sheen stays smooth; glints off the scratches.
	float graze = 1.0 - clamp(v.y, 0.0, 1.0);
	col += vec3(0.55, 0.80, 1.00) * SHEEN * pow(graze, SHEEN_POW) * trust;
	// The nudge keeps this defined when seen from straight under the light, now the underside shows.
	vec3 h = normalize(normalize(vec3(0.35, 1.0, 0.25)) + v + vec3(0.0, 0.0001, 0.0));
	col += vec3(0.92, 0.98, 1.00) * SPEC * pow(max(dot(n, h), 0.0), SPEC_TIGHT) * trust;

	// Twinkles, a few cells at a time.
	vec2 q = vec2(uv.s * 14.0, uv.t * 10.0);
	vec2 cell = floor(q);
	float hc = cf_hash(cell + vec2(3.0, 9.0));
	if(hc < SPARK_SHARE) {
		vec2 d = fract(q) - 0.5;
		float tw = pow(max(0.0, sin(timer * SPARK_RATE * (0.6 + hc) + hc * 40.0)), 10.0);
		col += vec3(0.90, 0.97, 1.00) * SPARK * tw * exp(-dot(d, d) * 40.0);
	}

	// A ragged frosty rim instead of the quad's square edge.
	float r = length(uv - 0.5) * 2.0;
	col *= 1.0 - smoothstep(EDGE_IN, 1.0, r + (ice - 0.55) * RAGGED);

	return vec4(col * color.rgb, color.a);
}
