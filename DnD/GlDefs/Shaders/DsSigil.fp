// Divine Shield's hit sigil: one of six designs from the 3 x 2 atlas DSHIT.png, picked and turned by a hash of where
// the hit is, so every client draws the same one. Additive; the actor's alpha fades it.

uniform float timer;

#define DESIGNS 6.0
#define TAU     6.2831853

float sg_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

vec4 Process(vec4 color) {
	vec3 o = floor(gl_TextureMatrix[7][3].xyz);
	float k = floor(sg_hash(o) * DESIGNS);
	float ang = TAU * sg_hash(o + 13.0);
	vec2 c = gl_TexCoord[0].st - 0.5;
	c = vec2(c.x * cos(ang) - c.y * sin(ang), c.x * sin(ang) + c.y * cos(ang));
	if(length(c) > 0.49)
		discard;
	vec2 cell = vec2(mod(k, 3.0), floor(k / 3.0));
	vec2 uv = (cell + 0.5 + c) / vec2(3.0, 2.0);
	return vec4(getTexel(uv).rgb * color.a, 1.0);
}
