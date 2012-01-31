#version 130

uniform sampler2D decal;
uniform vec4 color;

smooth in vec2 texcoord;

out vec4 fragment_output;

void main(){
	vec4 decal_color = texture(decal, texcoord);
	decal_color = vec4(1.0 - decal_color.r, 1.0 - decal_color.g, 1.0 - decal_color.b, decal_color.a);
	fragment_output = decal_color * color;
}
