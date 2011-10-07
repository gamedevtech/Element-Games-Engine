#version 130

uniform sampler2D depth_map;

smooth in vec2 texcoord;

out vec4 fragment_output;

void main(){
	vec2 camerarange = vec2(0.1, 100.0);
	float depth = (2.0 * camerarange.x) / (camerarange.y + camerarange.x - texture(depth_map, texcoord).x * (camerarange.y - camerarange.x));
	fragment_output = vec4(depth, depth, depth, 1.0);
}
