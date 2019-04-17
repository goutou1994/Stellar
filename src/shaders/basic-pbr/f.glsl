#version 410 core

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_texcoord;
in vec3 frag_tangent;
out vec4 frag_color;

uniform int active_textures;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

uniform samplerCube diffuseIBL;
uniform samplerCube specIBL;
uniform sampler2D brdfLUT;

struct Light {
	int type;	// 0: point 1: directional 2: spot
    vec3 pos;
    vec3 color;
    float I;
    // spot light
    vec3 dir;
    float fov;
};
uniform Light lights[10];
uniform int light_count;

uniform vec3 eye_pos;

const vec3 dielectricSpec = vec3(.04);
const float PI = 3.1415926f;

vec3 FresnelSchlick(vec3 F0, float lh) {
	float t = pow(1 - lh, 5);
	return mix(F0, vec3(1.0f), t);
}

vec3 FresnelSchlickRoughness(vec3 F0, float lh, float roughness) {
	float t = pow(1 - lh, 5);
	return mix(F0, vec3(1.0f - roughness), t);
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
	float metallic = texture(tex1, frag_texcoord).x;
	float roughness = texture(tex3, frag_texcoord).x;
//	if (roughness < .2f) roughness = .2f;
	vec3 V = -normalize(frag_pos - eye_pos);

	// normal
    vec3 N = normalize(frag_normal);
    if ((active_textures & (1 << 2)) > 0) {
    	vec3 normal_extra = texture(tex2, frag_texcoord).xyz * 2.0f - vec3(1.0f);
        vec3 bitangent = normalize(cross(frag_tangent, N));
    	N = normalize(normalize(cross(N, bitangent)) * normal_extra.y + bitangent * normal_extra.x + N * normal_extra.z);
    }

	// IBL
	vec3 L = reflect(-V, N);
	float NdotV = max(dot(N, V), .0f);
	vec3 F0 = mix(dielectricSpec, albedo, metallic);

	vec3 specIBLColor = textureLod(specIBL, L, roughness * 4.f).xyz;
    vec2 brdf = textureLod(brdfLUT, vec2(NdotV, 1 - roughness), .0f).xy;
    vec3 F = F0 * brdf.x + brdf.y;

    vec3 specColor = specIBLColor * F;

    vec3 kd = (1.f - F) * (1 - metallic);
//    vec3 kd = 1.f - F;
//	vec3 kd = 1.f - FresnelSchlickRoughness(F0, NdotV, roughness);

    vec3 diffuseIBLColor = texture(diffuseIBL, N).xyz;
    vec3 diffuseColor = kd * diffuseIBLColor * albedo;

	vec3 Lo = specColor + diffuseColor;

	// direct
	for (int i = 0; i < light_count; i++) {
		vec3 L;
		if (lights[i].type == 1) {
			L = -lights[i].dir;
		} else {
			L = -normalize(frag_pos - lights[i].pos);
		}
		vec3 H = normalize(V + L);

		// Fresnel
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

        float NdotL = max(dot(N, L), .0f);
        float d = distance(lights[i].pos, frag_pos);
        float I = lights[i].I;
        if (lights[i].type == 0) {
        	I *= min(1.f, 1 / pow(d - .5, 2));
        }
        if (lights[i].type == 2) {
        	I *= min(1.f, 1 / pow(d - .5, 2));
        	I *= clamp((lights[i].fov / 2 - acos(dot(-lights[i].dir, L)) * 2.f) + 1, .0f, 1.f);
        }
        Lo += (kd * albedo / PI + specular) * lights[i].color * I * NdotL * .5f;
	}
	Lo = Lo / (Lo + vec3(1.0));
    Lo = pow(Lo, vec3(1.0/2.2));
	frag_color = vec4(Lo, 1.0f);
}
