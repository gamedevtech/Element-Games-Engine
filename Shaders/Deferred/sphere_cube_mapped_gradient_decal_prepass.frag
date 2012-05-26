#version 410

in vec4 tePosition;
in vec2 teTexCoord;
in vec3 teNormal;
in vec3 teBinormal;
in vec3 teBitangent;
in vec3 teCubeMapTexCoord;
in float teHeightValue;

uniform sampler2D decal_map;
uniform samplerCube normal_map;
uniform samplerCube height_map;
// TODO: Add specular_map

uniform float material_specularity;
uniform vec4 material_color;
uniform int normal_mapping_enabled;
uniform int receives_lighting;

// TODO: Must use glBindFragDataLocation(GLuint program, GLuint colorNumber /* this is really just the index of the output */, const char *out_name); to set these up in the code for >gl3 renderers in the shader loading!
out vec4 fragment0;
out vec4 fragment1;
out vec4 fragment2;
out vec4 fragment3;

void main(){
	vec3 texcoord = normalize(tePosition.xyz);
	float height_index = texture(height_map, teCubeMapTexCoord).r;
	vec4 decal_color = texture(decal_map, vec2(height_index, 0.5));
	//vec4 decal_color = vec4(height_index, height_index, height_index, 1.0);
	if (object_is_lit == 1){
		// Position and Specular Factor
		fragment0 = vec4(tePosition.xyz, material_specularity);

		// Decal and Color
		fragment1 = decal_color * material_color;

		// Height and Normal
		float height = height_index * 2.0 - 1.0;
		vec3 computed_normal = normalize(-teNormal);
		if (normal_mapping_enabled == 1){
			vec3 temp_bitangent = cross(teNormal, teBinormal);
			mat3 tbn_matrix = mat3(normalize(teBinormal), /*normalize(bitangent)*/ normalize(temp_bitangent), normalize(teNormal));
			vec3 normal_map_value = normalize(2.0 * texture(normal_map, teCubeMapTexCoord).xyz - 1.0);
			computed_normal = normalize(tbn_matrix * normal_map_value);
		}
		fragment2 = vec4(computed_normal, height);

		// Placeholder for transparent data!
		fragment3 = vec4(0.0, 0.0, 0.0, 0.0);
	}else{
		vec4 color_output = decal_color * material_color;
		fragment0 = vec4(0.0, 0.0, 0.0, color_output.a);
		fragment1 = vec4(0.0, 0.0, 0.0, color_output.a);
		fragment2 = vec4(0.0, 0.0, 0.0, color_output.a);
		fragment3 = color_output;
	}
}
