#version 410 core

in vec2 frag_uv;

out vec4 out_color;
uniform mat4 MVP;
uniform sampler2D sampler_texture;

void main() {
	out_color = texture(sampler_texture, frag_uv);
}
