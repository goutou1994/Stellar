#version 410 core

in vec3 oPos;
out vec3 frag_pos;

uniform mat4 trans;

void main() {
	gl_Position = vec4(oPos, 1.f);
	vec4 frag_pos_h = trans * gl_Position;
	frag_pos = frag_pos_h.xyz / frag_pos_h.w;
}
