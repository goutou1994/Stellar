#version 410 core

uniform samplerCube tex;
uniform int face;
uniform mat4 trans[6];
in vec2 frag_texcoord;
out vec4 frag_color;

void main() {
	vec4 dir = trans[face] * vec4(frag_texcoord, 1.0f, 1.0f);
	frag_color = texture(tex, dir.xyz / dir.w);
}
