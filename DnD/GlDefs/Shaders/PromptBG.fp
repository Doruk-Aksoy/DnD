uniform float timer;

vec2 gtxc() {
	vec2 texCoord = gl_TexCoord[0].st;

	const float pi = 3.14159265358979323846;
	vec2 offset = vec2(0, 0);

	offset.y = 0.5 + sin(pi * 2.0 * (texCoord.y + timer * 0.2 + 900.0/8192.0)) + sin(pi * 2.0 * (texCoord.x * 2.0 + timer * 0.27 + 300.0/8192.0));
	offset.x = 0.5 + sin(pi * 2.0 * (texCoord.y + timer * 0.27 + 700.0/8192.0)) + sin(pi * 2.0 * (texCoord.x * 2.0 + timer * 0.2 + 1200.0/8192.0));

	return texCoord + offset * 0.0025;
}

vec4 Process(vec4 color) {
	vec2 st = gl_TexCoord[0].st;
	if(st.x < 0.1 || st.x > 0.9 || st.y < 0.1 || st.y > 0.9)
		return getTexel(gtxc()) * color;
	return getTexel(gl_TexCoord[0].st) * color;
}