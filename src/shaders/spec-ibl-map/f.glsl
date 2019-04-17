#version 410 core

in vec3 frag_pos;

uniform samplerCube texCube;

uniform float roughness;

out vec4 frag_color;

const float PI = 3.1415926;

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness*roughness;

    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // from tangent-space vector to world-space sample vector
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

const uint sample_count = 1024u;

void main() {
	vec3 N = normalize(frag_pos);
	N.xy = -N.xy;
	vec3 R = N;
	vec3 V = R;
	float totalWeight = .0f;

	vec3 acc_color = vec3(.0f);

	float sample_num = .0f;
	for (uint i = 0; i < sample_count; i++) {
		vec2 Xi = Hammersley(i, sample_count);
		vec3 H = ImportanceSampleGGX(Xi, N, roughness);
		vec3 L = reflect(-V, H);

		float NL = dot(N, L);
		if (NL > .0f) {
			acc_color += texture(texCube, L).xyz * NL;
			totalWeight += NL;
		}

	}
	acc_color /= totalWeight;
	frag_color = vec4(acc_color, 1.f);
}
