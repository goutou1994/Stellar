#version 410 core
layout (location = 0) in vec3 oPos;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 oTexCoord;
layout (location = 3) in vec3 oTangent;

uniform mat4 trans;
uniform mat4 model_trans;
uniform mat4 model_normal_trans;
out vec3 frag_pos;
out vec3 frag_normal;
out vec2 frag_texcoord;
out vec3 frag_tangent;

void main() {
	gl_Position = trans * model_trans * vec4(oPos, 1.f);
	vec4 frag_pos_h = model_trans * vec4(oPos, 1.f);
	frag_pos = frag_pos_h.xyz / frag_pos_h.w;
	vec4 frag_normal_h = model_normal_trans * vec4(oNormal, 1.f);
	frag_normal = frag_normal_h.xyz / frag_normal_h.w;
	frag_texcoord = oTexCoord;
	vec4 frag_tangent_h = model_trans * vec4(oTangent, 1.f);
	frag_tangent = frag_tangent_h.xyz / frag_tangent_h.w;
}
