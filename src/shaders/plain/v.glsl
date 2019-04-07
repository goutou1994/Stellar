#version 410 core
layout (location = 0) in vec3 oPos;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 oTexCoord;

uniform mat4 trans;
out vec3 frag_pos;
out vec3 frag_normal;
out vec2 frag_texcoord;

void main() {
	gl_Position = trans * vec4(oPos, 1.0f);
	frag_pos = oPos;
	frag_normal = oNormal;
	frag_texcoord = oTexCoord;
}
