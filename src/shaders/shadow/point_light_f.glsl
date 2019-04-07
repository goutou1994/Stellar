#version 410 core

in vec4 frag_pos;

uniform vec3 light_pos;
uniform float far_plane;

void main() {
    gl_FragDepth = distance(frag_pos.xyz / frag_pos.w, light_pos) / far_plane;
}
