#version 130

uniform sampler2D color_map;
uniform sampler2D light_map;
uniform sampler2D bloom_map;

smooth in vec2 texcoord;

out vec4 fragment_output;

void main(){
	vec4 light = texture(light_map, texcoord);
	vec4 color = texture(color_map, texcoord);
	vec4 bloom = texture(bloom_map, texcoord);

	fragment_output = (light * color) + bloom;
}
