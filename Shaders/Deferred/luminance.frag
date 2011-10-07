#version 130

uniform sampler2D light_map, bloom_map;
smooth in vec2 texcoord;
out vec4 fragment_output;

void main(){
	fragment_output = texture(light_map, texcoord) + texture(bloom_map, texcoord);
}
