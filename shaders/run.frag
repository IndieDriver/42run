#version 410 core

in vec3 frag_color;

out vec4 out_color;
uniform mat4 MVP;
void main() {
	out_color = vec4(frag_color, 1.0f);
}
