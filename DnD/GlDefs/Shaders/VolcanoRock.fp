// Volcano's cone, bound to VLROCK: the stone wrapped three ways in model space and lit per facet; its crevices glow
// beside the flows and on the crater's lip (heat in the UV s, rest height in t, so it stays dark until the cone is up).

#ifdef NO_FOG
varying vec4 pixelpos;
uniform vec3 camerapos;
#endif

uniform float timer;

#define TEXSCALE 56.0   // md3 units the skin spans
#define GLOW     1.15

vec3 vr_fire(float i) {
	float hot = smoothstep(0.7, 1.6, i);
	return vec3(1.0, 0.42 + 0.4 * hot, 0.08 + 0.3 * hot * hot) * min(1.0, i * 1.4);
}

vec4 Process(vec4 color) {
	// The point in the model's own space, GL order: md3 x, up, md3 y.
	vec3 c0 = gl_TextureMatrix[7][0].xyz, c1 = gl_TextureMatrix[7][1].xyz, c2 = gl_TextureMatrix[7][2].xyz;
	vec3 x12 = cross(c1, c2), x20 = cross(c2, c0), x01 = cross(c0, c1);
	vec3 w = pixelpos.xyz - gl_TextureMatrix[7][3].xyz;
	vec3 p = vec3(dot(x12, w), dot(x20, w), dot(x01, w)) / dot(c0, x12);

	vec3 n = normalize(cross(dFdx(p), dFdy(p)));
	vec3 nw = normalize(n.x * x12 + n.y * x20 + n.z * x01);
	float face = sign(dot(nw, camerapos - pixelpos.xyz));
	n *= face;
	nw *= face;
	vec3 wt = pow(abs(n), vec3(4.0));
	wt /= wt.x + wt.y + wt.z;
	vec3 rock = getTexel(p.zy / TEXSCALE).rgb * wt.x + getTexel(p.xz / TEXSCALE).rgb * wt.y + getTexel(p.xy / TEXSCALE).rgb * wt.z;
	float lit = 0.5 + 0.75 * max(dot(nw, normalize(vec3(-0.35, 1.0, 0.25))), 0.0);

	// Heat only once the cone stands at its rest height: below it the rise or the sink is under way.
	vec2 uv = gl_TexCoord[0].st;
	float heat = uv.s * smoothstep(-8.0, -1.0, p.y - uv.t);
	rock *= 1.0 - 0.35 * smoothstep(0.15, 0.7, heat);
	float lum = dot(rock, vec3(0.3, 0.59, 0.11));
	float crev = 1.0 - smoothstep(0.05, 0.2, lum);
	float g = heat * heat * (0.1 + 0.9 * crev) * GLOW;

	// color carries the light alone: the stone is lit, the heat is not.
	return vec4(rock * lit * color.rgb + vr_fire(g), color.a);
}
