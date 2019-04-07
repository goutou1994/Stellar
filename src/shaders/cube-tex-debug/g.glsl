#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 frag_texcoord;

void main() {
	gl_Position = vec4(-1.0f, -1.0f, 0, 1.0f);
	frag_texcoord = vec2(-1.0f, -1.0f);
	EmitVertex();

	gl_Position = vec4(-1.0f, 1.0f, 0, 1.0f);
    frag_texcoord = vec2(-1.0f, 1.0f);
    EmitVertex();

    gl_Position = vec4(1.0f, -1.0f, 0, 1.0f);
    frag_texcoord = vec2(1.0f, -1.0f);
    EmitVertex();

    gl_Position = vec4(1.0f, 1.0f, 0, 1.0f);
    frag_texcoord = vec2(1.0f, 1.0f);
    EmitVertex();
    EndPrimitive();
}
