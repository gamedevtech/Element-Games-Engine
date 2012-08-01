#version 130

smooth in vec3 normal;
smooth in vec3 light;
smooth in vec3 view;
smooth in float light_distance;
smooth in vec4 color;
smooth in vec2 texcoord;
smooth in vec3 binormal;
smooth in vec3 bitangent;

uniform sampler2D decal_map;
uniform sampler2D normal_map;
uniform vec3 light_attenuation;
uniform float light_radius;
uniform vec3 light_color;
uniform int normal_mapping_enabled;
uniform vec4 material_color;

out vec4 fragment_output;

void main() {
    float light_factor = 0.0;

    vec3 normal_vector = normalize(normal);
    if (normal_mapping_enabled == 1){
        vec3 temp_bitangent = cross(normal, binormal);
        // NOTE: Using computed bitangent for now, not sure why precomputed one is not working!
        mat3 tbn_matrix = mat3(normalize(binormal), normalize(temp_bitangent), normal_vector);
        vec3 normal_map_value = normalize(2.0 * texture(normal_map, texcoord).xyz - 1.0);
        normal_vector = normalize(tbn_matrix * normal_map_value);
    }

    vec3 light_vector = normalize(light);
    vec4 color_out = texture(decal_map, texcoord.st);

    float normal_dot_light_vector = max(dot(normal_vector, light_vector), 0.0);
    if (normal_dot_light_vector > 0.0){
        float attenuation_factor = (1.0 / (light_attenuation.x + light_attenuation.y * light_distance + light_attenuation.z * light_distance * light_distance)) * (1.0 - pow((light_distance / light_radius), 2.0));
        light_factor += normal_dot_light_vector * attenuation_factor;
        light_factor += pow(max(dot(normalize(reflect(light_vector, normal_vector)), normalize(view)), 0.0), 16.0) * 0.7 * attenuation_factor;
    }

    fragment_output = material_color * color_out * vec4((light_color * light_factor), 1.0);
}
