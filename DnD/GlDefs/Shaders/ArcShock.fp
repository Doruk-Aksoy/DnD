// Arcane Trap's blast ring on the floor (flat2, Add): a bright thin front at the rim with a soft trail inside, broken
// in places; the actor's alpha fades it through rgb.

uniform float timer;

float as_hash(vec3 p) {
	p = fract(mod(p, 256.0) * 0.1031);
	p += dot(p, p.yzx + 33.33);
	return fract((p.x + p.y) * p.z);
}

vec4 Process(vec4 color) {
	vec2 p = (gl_TexCoord[0].st - 0.5) * 2.0;
	float r = length(p);
	float a = atan(p.y, p.x + 1e-6);
	float seg = floor((a / 6.2831853 + 0.5) * 28.0);
	float gap = 0.55 + 0.45 * step(0.2, as_hash(vec3(seg, 3.0, 1.0)));
	float front = exp(-(r - 0.9) * (r - 0.9) / 0.0006) * gap;
	float trail = smoothstep(0.35, 0.9, r) * (1.0 - smoothstep(0.9, 0.97, r)) * 0.3;
	vec3 col = vec3(0.9, 0.82, 1.0) * front * 0.5 + vec3(0.55, 0.3, 1.0) * trail * 0.25;
	return vec4(min(col, vec3(1.0)) * clamp(color.a, 0.0, 1.0), 1.0);
}
