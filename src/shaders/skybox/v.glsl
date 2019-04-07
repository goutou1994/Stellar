#version 410 core

layout (location = 0) in vec3 oPos;

uniform mat4 trans;
out vec3 frag_pos;

void main() {
	gl_Position = trans * vec4(oPos, 1.f);
	frag_pos = oPos;
}
