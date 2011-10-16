#version 130

uniform sampler2D out_map;

smooth in vec2 texcoord;
smooth in vec4 position;

out vec4 fragment_output;

void main(){
	fragment_output = vec4(texture(out_map, texcoord).rgb, 1.0);
}
