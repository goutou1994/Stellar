#version 410 core

uniform sampler2D tex;

in vec2 frag_texcoord;
out vec4 frag_color;

void main() {
//	frag_color = vec4(1.0f, 1.0f, 0, 1.0f);
	frag_color = texture(tex, frag_texcoord);
}
