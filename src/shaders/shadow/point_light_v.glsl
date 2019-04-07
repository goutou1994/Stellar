#version 410 core
layout (location = 0) in vec3 oPos;

uniform mat4 model_trans;

void main() {
	gl_Position = model_trans * vec4(oPos, 1.0f);
}
