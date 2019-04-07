#version 410 core
layout (location = 0) in vec3 oPos;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 oTexCoord;
layout (location = 3) in vec3 oTangent;

uniform mat4 trans;
uniform mat4 model_trans;
out vec3 frag_pos;
out vec3 frag_normal;
out vec2 frag_texcoord;
out vec3 frag_tangent;

void main() {
	gl_Position = trans * model_trans * vec4(oPos, 1.0f);
	vec4 frag_pos_h = model_trans * vec4(oPos, 1.f);
	frag_pos = frag_pos_h.xyz / frag_pos_h.w;
	frag_normal = oNormal;
	frag_texcoord = oTexCoord;
	frag_tangent = oTangent;
}
