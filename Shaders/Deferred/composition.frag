#version 130

uniform sampler2D color_map;
uniform sampler2D light_map;
uniform sampler2D bloom_map;
uniform sampler2D ssao_map;
uniform sampler2D translucent_map;
uniform float luminance;
uniform float luminance_scale;
uniform float bloom_scale;
uniform int ssao_enabled;
uniform int bloom_enabled;

smooth in vec2 texcoord;

out vec4 fragment_output;

void main(){
	vec3 light = texture(light_map, texcoord).rgb;
	vec3 color = texture(color_map, texcoord).rgb;
	vec3 bloom = texture(bloom_map, texcoord).rgb;
	vec3 trans = texture(translucent_map, texcoord).rgb;

	float ssao = texture(ssao_map, texcoord).r;
	float bloom_amount = bloom_scale * (1.0 - (luminance * luminance_scale));

	if (ssao_enabled == 0){
		ssao = 1.0;
	}
	if (bloom_enabled == 0){
		bloom_amount = 0.0;
	}
	
	vec3 color_output = light * color * ssao;
	color_output += bloom * bloom_amount * ssao;
	fragment_output = vec4(color_output + trans, 1.0);
}
