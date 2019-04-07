#version 410 core

struct PointLight {
    vec3 pos;
    vec3 color;
    float I;
};

uniform samplerCube shadowTex;
uniform int shadowIndex;

struct Ambient {
	vec3 color;
	float I;
};


in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_texcoord;
in vec3 frag_tangent;

uniform int active_textures;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

uniform PointLight point_lights[10];
uniform Ambient ambient;

uniform int point_light_num;
uniform vec3 eye_pos;

out vec4 frag_color;

vec3 diffuse(vec3 color, vec3 light_dir, vec3 normal) {
	return color * max(dot(-light_dir, normal), 0);
}

vec3 specular(vec3 color, vec3 light_dir, vec3 normal, vec3 view_dir, int shiness) {
	return color * pow(max(.0f, dot(-view_dir, reflect(light_dir, normal))), 8.0f);
}

void main() {

	vec3 acc_color = texture(tex0, frag_texcoord).xyz * .0;//ambient.I;// * ambient.I;
	vec3 view_dir = normalize(frag_pos - eye_pos);

	const float near_plane = 1.0f, far_plane = 200.f; // 这里要根据绘制阴影贴图时的设定进行修改

	// normal
	vec3 normal = normalize(frag_normal);
    if ((active_textures & (1 << 2)) > 0) {
      	vec3 normal_extra = texture(tex2, frag_texcoord).xyz * 2.0f - vec3(1.0f);
        vec3 bitangent = normalize(cross(frag_tangent, normal));
		normal = normalize(frag_tangent * normal_extra.y + bitangent * normal_extra.x + normal * normal_extra.z);
    }

	for (int i = 0; i < point_light_num; i++) {

		vec3 light_dir = normalize(frag_pos - point_lights[i].pos);

		if (i == shadowIndex) {
        	float depth = texture(shadowTex, light_dir).r * far_plane;
        	float curDepth = distance(frag_pos, point_lights[i].pos);
//        	if (texture(shadowTex, light_dir).r < 1) continue;
        	if (curDepth > depth + 0.5f) {
        		continue;
        	}
        }

		// diffuse
		if ((active_textures & 1) > 0) {
			acc_color += diffuse(texture(tex0, frag_texcoord).xyz, light_dir, normal);
		}

//		 specular
		if ((active_textures & 2) > 0) {
			acc_color += specular(texture(tex1, frag_texcoord).xyz, light_dir, normal, view_dir, 8);
		}
	}
	frag_color = vec4(acc_color, 1.0f);
}