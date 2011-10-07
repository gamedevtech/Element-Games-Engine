#version 130

uniform sampler2D decal;
uniform samplerCube height;
uniform vec4 color;

smooth in vec3 texcoord;

out vec4 fragment_output;

void main(){
	float height_index = texture(height, texcoord).r;
	fragment_output = vec4(texture(decal, vec2(height_index, 0.5)).rgb * color.rgb, 1.0);
}
