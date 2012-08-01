#version 130

smooth in vec3 position;
smooth in vec3 normal;
smooth in vec3 binormal;
smooth in vec3 bitangent;
smooth in vec2 texcoord;

uniform sampler2D decal_map;
uniform sampler2D normal_map;
uniform sampler2D height_map;
uniform sampler2D specular_map;
uniform int use_specular_map;

uniform float material_specularity;
uniform vec4 material_color;
uniform int normal_mapping_enabled;
uniform int receives_lighting;

out vec4 fragment0;
out vec4 fragment1;
out vec4 fragment2;
out vec4 fragment3;

void main(){
    if (receives_lighting == 1){
        // Position and Specular Factor
        float specularity = material_specularity;
        if (use_specular_map == 1) {
            specularity *= texture(specular_map, texcoord).r;
        }
        fragment0 = vec4(position, specularity);

        // Decal and Color
        fragment1 = texture(decal_map, texcoord) * material_color;

        // Height and Normal
        float height = texture(height_map, texcoord).r * 2.0 - 1.0;
        vec3 computed_normal = normalize(normal);
        if (normal_mapping_enabled == 1){
            vec3 temp_bitangent = cross(normal, binormal);
            // NOTE: Using computed bitangent for now, not sure why precomputed one is not working!
            mat3 tbn_matrix = mat3(normalize(binormal), normalize(temp_bitangent), normalize(normal));
            vec3 normal_map_value = normalize(2.0 * texture(normal_map, texcoord).xyz - 1.0);
            computed_normal = normalize(tbn_matrix * normal_map_value);
        }
        fragment2 = vec4(computed_normal, 1.0);

        // Placeholder for transparent data!
        fragment3 = vec4(0.0, 0.0, 0.0, 1.0);
    }else{
        vec4 color_output = texture(decal_map, texcoord) * material_color;
        fragment0 = vec4(0.0, 0.0, 0.0, color_output.a);
        fragment1 = vec4(0.0, 0.0, 0.0, color_output.a);
        fragment2 = vec4(0.0, 0.0, 0.0, color_output.a);
        fragment3 = color_output;
    }
}
