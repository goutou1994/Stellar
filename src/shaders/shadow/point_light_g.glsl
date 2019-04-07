#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 trans[6];

out vec4 frag_pos;

void main() {
	for (int i = 0; i < 6; i++) {
		gl_Layer = i;
		for (int j = 0; j < 3; j++) {
			frag_pos = trans[i] * gl_in[j].gl_Position;
        	gl_Position = vec4(-frag_pos.x, -frag_pos.y, frag_pos.zw);
        	frag_pos = gl_in[j].gl_Position;
        	EmitVertex();
		}
		EndPrimitive();
	}
}
