#version 410 core
layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec2 vertex_texcoord;
layout(location = 2) in vec3 vertex_normal;

out vec3 frag_pos;
out vec2 frag_uv;
out vec3 normal_modelview;
out mat4 mv;

uniform mat4 MVP;
uniform mat4 M;

void main() {
	gl_Position = MVP * vec4(vertex_pos, 1.0f);
	frag_pos = vec3(M * vec4(vertex_pos, 1.0f));
  /* frag_pos.y = -frag_pos.y; */
	frag_uv = vertex_texcoord;
  normal_modelview = transpose(inverse(mat3(M))) * normalize(vertex_normal);
}
