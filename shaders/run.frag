#version 410 core

struct Light {
  vec4 position;
  vec3 color;
  float radius;
};

in vec3 frag_pos;
in vec2 frag_uv;
in vec3 normal_modelview;
in mat4 mv;

out vec4 out_color;
uniform mat4 MVP;
uniform sampler2D sampler_texture;

uniform Light lights[3];
uniform int light_nb;

void main() {
	vec3 fragColor = texture(sampler_texture, frag_uv).xyz * 0.5f;

	for (int i = 0; i < max(light_nb, 3); i++) {
		vec3 L = lights[i].position.xyz - frag_pos;
		float dist = length(L);

		L = normalize(L);

		float atten = lights[i].radius / (pow(dist, 2.0) + 1.0);

		vec3 N = normalize(-normal_modelview);
		float NdotL = max(0.0, dot(N, L));
		vec3 diff = lights[i].color * texture(sampler_texture, frag_uv).xyz * NdotL * atten;

		fragColor += diff;
	}

  out_color = vec4(fragColor, 1.0f);
}
