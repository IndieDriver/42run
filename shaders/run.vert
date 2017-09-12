#version 410 core
layout(location = 0) in vec3 vertex_pos;

out vec3 frag_color;
uniform mat4 MVP;
void main() {
	frag_color = vec3(1.0f, 0.0f, 0.0f);
	gl_Position = MVP * vec4(vertex_pos, 1.0f);
}
