#version 410 core

in vec3 frag_pos;
uniform samplerCube skybox;

out vec4 frag_color;

void main() {
	frag_color = texture(skybox, frag_pos);
}
