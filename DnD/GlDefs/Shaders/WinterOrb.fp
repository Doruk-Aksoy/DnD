// Winter Orb. Bound to WNTRORB, a copy of Ice Nova's scratched ice under its own name.
// Lit toward the camera off the engine's world positions, so the shine stays put while the orb spins.

// NO_FOG marks the colormap pass, whose base declares neither. See .claude/notes/bcs-acs-gotchas.md.
#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define SEGS        16.0   // WINTERORB.md3's segments round; the normal below is rebuilt from them
#define CROP_LO     0.14   // the art darkens toward its edges; only the clean middle is used
#define CROP_HI     0.86
#define DRIFT       0.02   // the frost creeps slowly round, under the model's own spin
#define POLE_SOFT   0.78   // where the pinched poles start fading to plain ice
#define BODY        0.80
#define CORE        0.35   // how dark the ice is, looking straight through its middle
#define SCR_LO      0.58   // the scratches picked out as frost lines
#define SCR_HI      0.86
#define SCRATCH     0.55
#define RIM_POW     2.5    // the glassy edge; higher is thinner
#define LIFT        0.90   // how far above the viewer the highlight's light sits
#define SPEC        1.80
#define SPEC_TIGHT  160.0
#define HALO        0.30
#define HALO_TIGHT  14.0
#define SPARK       1.40
#define SPARK_RATE  2.2
#define SPARK_SHARE 0.30

float wo_hash(vec2 p) {
	return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

// Mirrored repeat into the crop: every edge meets its own reflection, so there is no seam.
float wo_tile(float x) {
	float m = 1.0 - abs(1.0 - mod(x, 2.0));
	return mix(CROP_LO, CROP_HI, m);
}

vec4 Process(vec4 color) {
	vec2 uv = gl_TexCoord[0].st;
	float phi = uv.t * 3.14159265;

	vec3 p = pixelpos.xyz;
	vec3 dpx = dFdx(p);
	vec3 dpy = dFdy(p);
	vec2 dux = dFdx(uv);
	vec2 duy = dFdy(uv);
	float det = dux.x * duy.y - duy.x * dux.y;

	// camerapos may arrive in either axis order; the right one sits about pixelpos.w away.
	vec3 c1 = camerapos;
	vec3 c2 = camerapos.xzy;
	float e1 = abs(distance(p, c1) - pixelpos.w);
	float e2 = abs(distance(p, c2) - pixelpos.w);
	vec3 v = normalize((e1 < e2 ? c1 : c2) - p);
	// Neither fitting means the positions are not world space after all: shade it as flat frost.
	float trust = 1.0 - smoothstep(1.5, 3.0, min(e1, e2) / max(pixelpos.w, 1.0));

	// A facet's normal leans exactly toward its column's middle; turn that on to this pixel's longitude.
	vec3 nf = cross(dpx, dpy);
	if(dot(nf, v) < 0.0)
		nf = -nf;
	vec2 radmid = nf.xz / max(length(nf.xz), 1e-9);
	vec2 along = vec2(-radmid.y, radmid.x);
	if(dot((dpx * duy.y - dpy * dux.y).xz, along) * det < 0.0)
		along = -along;
	float seg = min(floor(uv.s * SEGS * 0.5), SEGS - 1.0);
	float delta = (uv.s - (seg + 0.5) * 2.0 / SEGS) * 3.14159265;
	vec2 rad = radmid * cos(delta) + along * sin(delta);
	// t runs from the bottom pole as built; the facet's own slope confirms which way is up.
	float up = ((dpy.y * dux.x - dpx.y * duy.x) * det < 0.0) ? -1.0 : 1.0;
	vec3 n = vec3(rad.x * sin(phi), -cos(phi) * up, rad.y * sin(phi));

	float face = mix(0.55, clamp(dot(n, v), 0.0, 1.0), trust);
	float edge = 1.0 - face;

	vec3 art = getTexel(vec2(wo_tile(uv.s + timer * DRIFT), wo_tile(uv.t * 2.0))).rgb;
	float ice = dot(art, vec3(0.299, 0.587, 0.114));
	float pole = smoothstep(POLE_SOFT, 1.0, abs(uv.t * 2.0 - 1.0));
	ice = mix(ice, 0.55, pole);

	// Deep ice, darkest straight through the middle, with the scratches standing out as frost.
	vec3 col = mix(vec3(0.01, 0.06, 0.20), vec3(0.45, 0.78, 1.00), ice) * BODY * mix(CORE, 1.0, edge);
	col += vec3(0.70, 0.90, 1.00) * SCRATCH * smoothstep(SCR_LO, SCR_HI, ice);

	// The glassy rim, and the sky caught in the upper half.
	col += vec3(0.50, 0.85, 1.00) * pow(edge, RIM_POW) * trust;
	vec3 r = reflect(-v, n);
	col += vec3(0.30, 0.55, 0.80) * smoothstep(-0.05, 0.30, r.y) * (0.35 + 0.65 * edge) * trust;

	// The highlight, from a light just above the viewer.
	vec3 h = normalize(normalize(v + vec3(0.0, LIFT, 0.0)) + v);
	float nh = max(dot(n, h), 0.0);
	col += vec3(0.92, 0.97, 1.00) * (SPEC * pow(nh, SPEC_TIGHT) + HALO * pow(nh, HALO_TIGHT)) * trust;

	// Twinkles on the frost, a few cells at a time, kept off the poles where the cells squeeze together.
	vec2 q = vec2(uv.s * 8.0, uv.t * 6.0);
	vec2 cell = floor(q);
	float hc = wo_hash(cell);
	if(hc < SPARK_SHARE) {
		vec2 d = fract(q) - 0.5;
		float tw = pow(max(0.0, sin(timer * SPARK_RATE * (0.6 + hc) + hc * 40.0)), 10.0);
		col += vec3(0.85, 0.95, 1.00) * SPARK * tw * exp(-dot(d, d) * 40.0) * (1.0 - pole) * face;
	}

	return vec4(col * color.rgb, color.a);
}
