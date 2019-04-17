#version 410 core

in vec3 frag_pos;

uniform samplerCube texCube;

out vec4 frag_color;

const float sampleStep = .05f;
const float PI = 3.1415926f;

void main() {
	vec3 dir = normalize(frag_pos);
	dir.xy = -dir.xy;
	vec3 baseY = dir;
	vec3 baseZ = normalize(cross(vec3(0, 1.f, 0), baseY));
	vec3 baseX = cross(baseY, baseZ);
	mat3 trans = mat3(baseX, baseY, baseZ);

	vec3 acc_color = vec3(.0f);
	float sample_count = .0f;

    for (float theta = .0f; theta <= .5f * PI; theta += sampleStep) {
    	for (float phi = .0f; phi <= 2.f * PI; phi += sampleStep) {
    		vec3 tangent_pos = vec3(sin(theta) * sin(phi), cos(theta), sin(theta) * cos(phi));
    		vec3 world_pos = trans * tangent_pos;
    		acc_color += texture(texCube, world_pos).xyz * cos(theta) * sin(theta);
			sample_count += 1;
    	}
    }

    frag_color = vec4(acc_color * PI / sample_count, 1.f);
}
