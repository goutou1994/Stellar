#version 410 core

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_texcoord;
out vec4 frag_color;

uniform samplerCube diffuseIBL;
uniform samplerCube specIBL;
uniform sampler2D brdfLUT;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex3;
uniform vec3 eye_pos;

const vec3 dielectricSpec = vec3(.04);
const float PI = 3.1415926f;


vec3 FresnelSchlickRoughness(vec3 F0, float lh, float roughness) {
	float t = pow(1 - lh, 5);
	return mix(F0, vec3(1.0f - roughness), t);
}

vec3 FresnelSchlick(vec3 F0, float lh) {
	float t = pow(1 - lh, 5);
	return mix(F0, vec3(1.0f), t);
}

void main() {
	vec3 albedo = texture(tex0, frag_texcoord).xyz;
	float metallic = texture(tex1, frag_texcoord).x;
	float roughness = texture(tex3, frag_texcoord).x;
	if (roughness < .2f) roughness = .2f;

	vec3 N = normalize(frag_normal);
	vec3 V = normalize(eye_pos - frag_pos);
	vec3 L = reflect(-V, N);
	float NdotV = max(dot(N, V), .0f);

	vec3 F0 = mix(dielectricSpec, albedo, metallic);
//	vec3 F = FresnelSchlickRoughness(F0, NdotV, roughness);
//	vec3 F = FresnelSchlick(F0, NdotV);

	vec3 specIBLColor = textureLod(specIBL, L, roughness * 4.f).xyz;
	vec2 brdf = textureLod(brdfLUT, vec2(NdotV, 1 - roughness), .0f).xy;
	vec3 F = F0 * brdf.x + brdf.y;

	vec3 specColor = specIBLColor * F;

	vec3 kd = (1.f - F) * (1 - metallic);
//	vec3 kd = 1.f - F;
    vec3 diffuseIBLColor = texture(diffuseIBL, N).xyz;
    vec3 diffuseColor = kd * diffuseIBLColor * albedo;


	frag_color = vec4(specColor + diffuseColor, 1.f);
}
