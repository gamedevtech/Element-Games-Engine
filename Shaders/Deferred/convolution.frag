#version 130

smooth in vec2 texcoord;

uniform sampler2D map_in0;
uniform sampler2D map_in1;
uniform sampler2D translucent_map;
uniform vec2 size;

out vec4 fragment_output;

float step_w = 1.0 / size.x;
float step_h = 1.0 / size.y;

float k[9];
vec2 offset[9];

void main(){
	/*float scale = texture(map_in, texcoord).a;
	scale -= 1.0;
	scale = max(scale, 0.0);
	scale *= scale;*/

	k[0] = 1.0 / 16.0;
	k[1] = 2.0 / 16.0;
	k[2] = 1.0 / 16.0;
	k[3] = 2.0 / 16.0;
	k[4] = 4.0 / 16.0;
	k[5] = 2.0 / 16.0;
	k[6] = 1.0 / 16.0;
	k[7] = 2.0 / 16.0;
	k[8] = 1.0 / 16.0;
	offset[0] = vec2(-step_w, -step_h);
	offset[1] = vec2(0.0, -step_h);
	offset[2] = vec2(step_w, -step_h);
	offset[3] = vec2(-step_w, 0.0);
	offset[4] = vec2(0.0, 0.0);
	offset[5] = vec2(step_w, 0.0);
	offset[6] = vec2(-step_w, step_h);
	offset[7] = vec2(0.0, step_h);
	offset[8] = vec2(step_w, step_h);

	vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 temp = vec4(0.0, 0.0, 0.0, 0.0);

	temp = texture(map_in0, texcoord + offset[0]) + texture(map_in1, texcoord + offset[0]);
	//temp *= temp.a;
	sum += temp * k[0];

	temp = texture(map_in0, texcoord + offset[1]) + texture(map_in1, texcoord + offset[1]);
	//temp *= temp.a;
	sum += temp * k[1];

	temp = texture(map_in0, texcoord + offset[2]) + texture(map_in1, texcoord + offset[2]);
	//temp *= temp.a;
	sum += temp * k[2];

	temp = texture(map_in0, texcoord + offset[3]) + texture(map_in1, texcoord + offset[3]);
	//temp *= temp.a;
	sum += temp * k[3];

	temp = texture(map_in0, texcoord + offset[4]) + texture(map_in1, texcoord + offset[4]);
	//temp *= temp.a;
	sum += temp * k[4];

	temp = texture(map_in0, texcoord + offset[5]) + texture(map_in1, texcoord + offset[5]);
	//temp *= temp.a;
	sum += temp * k[5];

	temp = texture(map_in0, texcoord + offset[6]) + texture(map_in1, texcoord + offset[6]);
	//temp *= temp.a;
	sum += temp * k[6];

	temp = texture(map_in0, texcoord + offset[7]) + texture(map_in1, texcoord + offset[7]);
	//temp *= temp.a;
	sum += temp * k[7];

	temp = texture(map_in0, texcoord + offset[8]) + texture(map_in1, texcoord + offset[8]);
	//temp *= temp.a;
	sum += temp * k[8];

	fragment_output = sum + texture(translucent_map, texcoord);// * scale;
	//fragment_output = texture(map_in, texcoord);
}
