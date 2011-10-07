#version 130

uniform sampler2D normal_map;
uniform vec4 light_attenuation;
uniform float light_radius;
uniform vec3 light_color;

smooth in vec3 normal;
smooth in vec3 binormal;
smooth in vec3 bitangent;
smooth in vec3 light;
smooth in vec3 view;
smooth in float light_distance;
smooth in vec2 texcoord;

out vec4 fragment_output;

void main(){
	vec3 normal_vector = normalize(normal);
	vec3 binormal_vector = normalize(binormal);
	vec3 bitangent_vector = normalize(bitangent);
	mat3 tbn_matrix = mat3(normalize(binormal_vector), normalize(bitangent_vector), normalize(normal_vector));
	vec3 normal_map_value = normalize(2.0 * texture(normal_map, texcoord).xyz - 1.0);
	normal_vector = normalize(tbn_matrix * normal_map_value);

	vec3 light_vector = normalize(light);
	vec3 view_vector = normalize(view);
	vec3 reflect_vector = normalize(reflect(light_vector, normal_vector));

	float normal_dot_light_vector = 0.0;
	float normal_dot_half_vector = 0.0;
	float light_factor = 0.0;

	vec4 color_out = vec4(1.0, 1.0, 1.0, 1.0);//material_color;

	float ambient = 0.0;
	normal_dot_light_vector = max(dot(normal_vector, light_vector), ambient);
	if (normal_dot_light_vector > 0.0){
		float attenuation_factor = (1.0 / (light_attenuation.x + light_attenuation.y * light_distance + light_attenuation.z * light_distance * light_distance));
		float material_specularity = 0.5;
		attenuation_factor = 1.0;
		attenuation_factor *= (1.0 - pow((light_distance / light_radius), 2.0));

		light_factor += normal_dot_light_vector * attenuation_factor;

		normal_dot_half_vector = max(dot(reflect_vector, view_vector), 0.0);
		light_factor += pow(normal_dot_half_vector, 16.0) * material_specularity * attenuation_factor;
	}

	color_out *= vec4((light_color * light_factor), 1.0);

	fragment_output = color_out;
}
