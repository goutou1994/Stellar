#version 410 core

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_texcoord;
out vec4 frag_color;

uniform sampler2D tex0;
uniform float metallic;
uniform float roughness;

struct PointLight {
    vec3 pos;
    vec3 color;
    float I;
};
uniform PointLight point_lights[10];
uniform int point_light_num;

uniform vec3 eye_pos;

const vec3 dielectricSpec = vec3(.04);
const float PI = 3.1415926f;

vec3 FresnelSchlick(vec3 F0, float lh) {
	float t = pow(1 - lh, 5);
	return mix(F0, vec3(1.0f), t);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

void main() {
	vec3 albedo = texture(tex0, frag_texcoord).xyz;
	vec3 N = frag_normal, V = -normalize(frag_pos - eye_pos);
	vec3 Lo = vec3(.0f);
	for (int i = 0; i < point_light_num; i++) {
		vec3 L = -normalize(frag_pos - point_lights[i].pos);
		vec3 H = normalize(V + L);

		// Fresnel
		vec3 F0 = mix(dielectricSpec, albedo, metallic);
		vec3 F = FresnelSchlick(F0, dot(L, H));

		// ks
		vec3 ks = F;

		// kd
		vec3 kd = (vec3(1.0f) - ks) * (1 - metallic);

		// specTerm
		float D = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 nominator = D * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular = nominator / denominator;

        float nl = max(dot(N, L), .0f);
        Lo += (kd * albedo / PI + specular) * point_lights[i].color * nl;
//        frag_color = vec4(vec3(0, 0, D), 1.0f);
	}
	frag_color = vec4(Lo * 2, 1.0f);
}
