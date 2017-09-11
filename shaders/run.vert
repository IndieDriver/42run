#version 410 core
layout(location = 0) in vec3 vertex_pos;

out vec3 frag_color;
void main() {
	frag_color = vec3(1.0f, 0.0f, 0.0f);
	gl_Position = vec4(vertex_pos, 1.0f);
}
