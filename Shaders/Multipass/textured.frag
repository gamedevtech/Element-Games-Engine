#version 130

uniform sampler2D decal;
uniform vec4 color;

smooth in vec2 texcoord;

out vec4 fragment_output;

void main(){
	fragment_output = vec4(texture(decal, texcoord).rgb * color.rgb, 1.0);
}
