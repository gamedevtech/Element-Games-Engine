// gaussian horizontal
#version 130

smooth in vec2 texcoord;

uniform sampler2D map_in;
uniform vec2 size;

out vec4 fragment_output;

float step_w = 1.0 / size.x;

float weights[9];
vec2 offsets[9];

void main(){
	weights[0] = 1.0 / 56.0;
	weights[1] = 3.0 / 56.0;
	weights[2] = 7.0 / 56.0;
	weights[3] = 11.0 / 56.0;
	weights[4] = 12.0 / 56.0;
	weights[5] = 11.0 / 56.0;
	weights[6] = 7.0 / 56.0;
	weights[7] = 3.0 / 56.0;
	weights[8] = 1.0 / 56.0;
	offsets[0] = vec2(-(4.0 * step_w), 0.0);
	offsets[1] = vec2(-(3.0 * step_w), 0.0);
	offsets[2] = vec2(-(2.0 * step_w), 0.0);
	offsets[3] = vec2(-step_w, 0.0);
	offsets[4] = vec2(0.0, 0.0);
	offsets[5] = vec2(step_w, 0.0);
	offsets[6] = vec2((2.0 * step_w), 0.0);
	offsets[7] = vec2((3.0 * step_w), 0.0);
	offsets[8] = vec2((4.0 * step_w), 0.0);

	vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
	sum += texture(map_in, texcoord + offsets[0]) * weights[0];
	sum += texture(map_in, texcoord + offsets[1]) * weights[1];
	sum += texture(map_in, texcoord + offsets[2]) * weights[2];
	sum += texture(map_in, texcoord + offsets[3]) * weights[3];
	sum += texture(map_in, texcoord + offsets[4]) * weights[4];
	sum += texture(map_in, texcoord + offsets[5]) * weights[5];
	sum += texture(map_in, texcoord + offsets[6]) * weights[6];
	sum += texture(map_in, texcoord + offsets[7]) * weights[7];
	sum += texture(map_in, texcoord + offsets[8]) * weights[8];

	fragment_output = sum;
}
