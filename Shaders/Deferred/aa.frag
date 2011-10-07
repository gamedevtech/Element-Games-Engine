#version 130

uniform sampler2D depth_map;
uniform sampler2D color_map;
uniform vec2 camerarange;
uniform vec2 screensize;

smooth in vec2 texcoord;

out vec4 fragment_output;

float ReadDepth(in vec2 coord){
	return (2.0 * camerarange.x) / (camerarange.y + camerarange.x - texture(depth_map, coord).x * (camerarange.y - camerarange.x));
}

vec3 ReadColor(in vec2 coord){
	return texture(color_map, coord).rgb;
}

void main(){
	float depth = ReadDepth(texcoord);

	// Texel Step Sizes
	float tsx = 1.0 / screensize.x;
	float txy = 1.0 / screensize.y;
}
