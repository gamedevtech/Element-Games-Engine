#version 130

smooth in vec3 position;
smooth in vec3 normal;
smooth in vec3 binormal;
smooth in vec3 bitangent;
smooth in vec2 texcoord;

uniform sampler2D decal_map;
uniform sampler2D normal_map;
uniform sampler2D height_map;
// TODO: Add specular_map

uniform float material_specularity;
uniform vec4 material_color;
uniform int normal_mapping_enabled;
uniform int receives_lighting;
uniform vec3 camera_position;

// TODO: Must use glBindFragDataLocation(GLuint program, GLuint colorNumber /* this is really just the index of the output */, const char *out_name); to set these up in the code for >gl3 renderers in the shader loading!
out vec4 fragment0;
out vec4 fragment1;
out vec4 fragment2;
out vec4 fragment3;

void main(){
    if (receives_lighting == 1){
		// Position and Specular Factor
		fragment0 = vec4(position, material_specularity);

		// Decal and Color
		fragment1 = texture(decal_map, texcoord) * material_color;

		// Height and Normal
		float height = texture(height_map, texcoord).r * 2.0 - 1.0;
		vec3 computed_normal = normalize(normal);
		if (normal_mapping_enabled == 1){
			vec3 temp_bitangent = cross(normal, binormal);
			mat3 tbn_matrix = mat3(normalize(binormal), /*normalize(bitangent)*/ normalize(temp_bitangent), normalize(normal));
			vec3 normal_map_value = normalize(2.0 * texture(normal_map, texcoord).xyz - 1.0);
			computed_normal = normalize(tbn_matrix * normal_map_value);
			//computed_normal = texture(normal_map, texcoord).xyz;
		}
		fragment2 = vec4(computed_normal, height);

		// Placeholder for transparent data!
		fragment3 = vec4(0.0, 0.0, 0.0, 0.0);
	}else{
		vec4 color_output = texture(decal_map, texcoord) * material_color;
		fragment0 = vec4(0.0, 0.0, 0.0, color_output.a);
		fragment1 = vec4(0.0, 0.0, 0.0, color_output.a);
		fragment2 = vec4(0.0, 0.0, 0.0, color_output.a);
		fragment3 = color_output;
	}
}
