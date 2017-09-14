#version 410 core
layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec2 vertex_texcoord;

out vec3 frag_color;
uniform mat4 MVP;
uniform vec3 color;
void main() {
	frag_color = color;
	gl_Position = MVP * vec4(vertex_pos, 1.0f);
}
