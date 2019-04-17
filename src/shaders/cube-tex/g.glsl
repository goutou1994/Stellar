#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 trans[6];

out vec3 frag_pos;

void main() {
	for (int i = 0; i < 6; i++) {
		gl_Layer = i;
		for (int j = 0; j < 3; j++) {
			gl_Position = gl_in[j].gl_Position;
			vec4 frag_pos_h = trans[i] * gl_in[j].gl_Position;
			frag_pos = frag_pos_h.xyz / frag_pos_h.w;
			EmitVertex();
		}
		EndPrimitive();
	}
}
