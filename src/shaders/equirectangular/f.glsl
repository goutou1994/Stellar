#version 410 core

in vec3 frag_pos;
out vec4 frag_color;

uniform sampler2D tex2D;

const float pi = 3.1415926;

void main() {
	vec3 dir = normalize(frag_pos);
	float phi = atan(dir.x, dir.z) / (2 * pi) + .5f;
	float theta = asin(dir.y) / pi + .5f;

	frag_color = texture(tex2D, vec2(phi, theta));
//	frag_color = vec4(vec3(theta), 1.f);

}
