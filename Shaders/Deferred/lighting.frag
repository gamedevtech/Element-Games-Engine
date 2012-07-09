#version 130

smooth in vec2 texcoord;

uniform mat4 shadow_mapping_bias;
uniform int shadow_mapping_enabled;
uniform sampler2DShadow shadow_map;
uniform vec2 shadow_map_size;

uniform sampler2D position_map;
uniform sampler2D normal_map;
uniform vec3 camera_position;
uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 light_attenuation;
uniform float light_radius;

//out vec4 fragment_output;
out vec4 fragment0;
out vec4 fragment1;

void main(){
    vec4 position_specular = texture(position_map, texcoord);
    vec3 position = position_specular.xyz;
    vec3 light_vector = position - light_position;
    float shadow_influence = 1.0;
    float light_distance = abs(length(light_vector));
    if (light_distance > light_radius){
        discard;
    }

    float specular_factor = position_specular.w;
    //vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    vec4 normal_height = texture(normal_map, texcoord);
    vec3 normal_vector = normalize(normal_height.xyz);
    float height_value = normal_height.w;

    vec3 view_vector = normalize(camera_position - position);
    light_vector = normalize(light_vector);
    vec3 reflect_vector = normalize(reflect(light_vector, normal_vector));

    float diffuse = max(dot(normal_vector, light_vector), 0.0);
    vec3 diffuse_color = vec3(0.0, 0.0, 0.0);
    vec3 specular_color = vec3(0.0, 0.0, 0.0);
    if (diffuse > 0.0){
        float attenuation_factor = 1.0 / (light_attenuation.x + (light_attenuation.y * light_distance) + (light_attenuation.z * light_distance * light_distance));
        attenuation_factor *= (1.0 - pow((light_distance / light_radius), 2.0));

        float specular = pow(max(dot(reflect_vector, view_vector), 0.0), 16.0) * specular_factor;

        diffuse_color = (diffuse * attenuation_factor) * light_color;
        specular_color = (specular * attenuation_factor) * light_color;
        //color = vec4(diffuse_color + specular_color, 1.0);
    }

    if (shadow_mapping_enabled == 1){
        vec4 projected_coordinate = shadow_mapping_bias * vec4(position, 1.0);
        shadow_influence = textureProj(shadow_map, projected_coordinate);
    }

    //fragment_output = vec4(color.rgb * shadow_influence, 1.0);
    fragment0 = vec4(diffuse_color * shadow_influence, 1.0);
    fragment1 = vec4(specular_color * shadow_influence, 1.0);
}
