#version 410 core

struct PointLight {
    vec3 pos;
    vec3 color;
    float I;
};

struct Ambient {
	vec3 color;
	float I;
};

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_texcoord;

uniform int active_textures;
uniform sampler2D tex0;
uniform sampler2D tex1;

uniform PointLight point_lights[10];
uniform Ambient ambient;

uniform int point_light_num;
uniform vec3 eye_pos;

out vec4 frag_color;

vec3 diffuse(vec3 color, vec3 light_dir, vec3 normal) {
	return color * max(dot(-light_dir, normalize(normal)), 0);
}

vec3 specular(vec3 color, vec3 light_dir, vec3 normal, vec3 view_dir, int shiness) {
	return color * pow(max(dot(-normalize(light_dir + view_dir), normal), 0), shiness);
}

void main() {
	vec3 normal = normalize(frag_normal);
	vec3 acc_color = vec3(0, 0, 0);
	vec3 view_dir = normalize(frag_pos - eye_pos);
	for (int i = 0; i < point_light_num; i++) {
		vec3 light_dir = normalize(frag_pos - point_lights[i].pos);

		// diffuse
		if ((active_textures & 1) > 0) {
			acc_color += texture(tex0, frag_texcoord).xyz * ambient.color * ambient.I;
			acc_color += diffuse(texture(tex0, frag_texcoord).xyz, light_dir, normal);
		}

		// specular
		if ((active_textures & 2) > 0) {
			acc_color += specular(texture(tex1, frag_texcoord).xyz, light_dir, normal, view_dir, 8);
		}
	}
	frag_color = vec4(acc_color, 1.0f);
}