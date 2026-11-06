// Volcano's lava, bound to VLLAVA (R slow noise, G distance to a plate's edge, B the plate's roll): crust plates drifting
// on the pool and sliding down the flows, molten between them. A flow's UV: s = 2 + 2 x its length + 0.1..0.9 across, t along.

#ifdef NO_FOG
varying vec4 pixelpos;
#endif

uniform float timer;

#define POOLZ  51.6     // the pool's rest height
#define POOLR  10.6
#define TILE   15.0     // md3 units one plate tile covers
#define FLOW   1.6      // units a second the flows slide down
#define FWIDTH 4.2      // a flow's width in units, for the plate scale across it
#define TOE    7.0      // units from a flow's end where it crusts over

vec3 vl_fire(float i) {
	float hot = smoothstep(0.7, 1.6, i);
	return vec3(1.0, 0.42 + 0.4 * hot, 0.08 + 0.3 * hot * hot) * min(1.0, i * 1.4);
}

vec4 Process(vec4 color) {
	vec3 c0 = gl_TextureMatrix[7][0].xyz, c1 = gl_TextureMatrix[7][1].xyz, c2 = gl_TextureMatrix[7][2].xyz;
	vec3 x12 = cross(c1, c2), x20 = cross(c2, c0), x01 = cross(c0, c1);
	vec3 w = pixelpos.xyz - gl_TextureMatrix[7][3].xyz;
	vec3 p = vec3(dot(x12, w), dot(x20, w), dot(x01, w)) / dot(c0, x12);
	float t = mod(timer, 512.0);
	vec2 uv = gl_TexCoord[0].st;

	// A flow: plates at their true size sliding down it; the pool: plates drifting.
	float flow = step(2.0, uv.s);
	float len = floor((uv.s - 2.0) * 0.5);
	float x = (uv.s - 2.1 - 2.0 * len) / 0.8;
	float toe = flow * (1.0 - smoothstep(0.0, TOE, len - uv.t));
	vec2 q = mix(p.xz + vec2(t * 0.35, t * 0.22), vec2((x - 0.5) * FWIDTH, uv.t - t * FLOW), flow);
	vec4 a = getTexel(q / TILE);
	vec4 b = getTexel(q / 7.0 + 0.37);
	float edge = mix(length(p.xz) / POOLR, abs(x - 0.5) * 2.0, flow);
	float high = mix(1.0, smoothstep(4.0, 46.0, p.y), flow);
	// The pool welling up over its rest height is an eruption: hotter, the crust broken up.
	float erupt = smoothstep(0.8, 2.6, p.y - POOLZ) * (1.0 - flow);
	float heat = 0.35 + 0.6 * a.r + 0.25 * b.r + 0.25 * high - 0.3 * edge * edge + 0.8 * erupt - 0.35 * toe;

	// Whole plates crusted over, more at the edges, down the slope and in the toe; a thin molten seam round each.
	float cover = mix(0.5 + 0.3 * edge * edge, 0.42 + 0.45 * edge * edge + 0.3 * (1.0 - high) + 0.45 * toe, flow) * (1.0 - erupt);
	float plate = 1.0 - smoothstep(cover - 0.04, cover + 0.04, a.b);
	float crust = plate * smoothstep(0.04, 0.12, a.g);
	float rim = plate * (1.0 - smoothstep(0.1, 0.3, a.g));
	vec3 stone = vec3(0.2, 0.15, 0.13) * (0.5 + 0.8 * b.r) * color.rgb;
	vec3 col = mix(vl_fire(heat), stone + vl_fire(heat * rim * 0.5), crust);
	return vec4(col, color.a);
}
