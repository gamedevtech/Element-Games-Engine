#version 130

smooth in vec2 texcoord;

uniform sampler2D decal_map;
uniform vec4 material_color;

out vec4 fragment_color;

void main(){
    vec4 decal_color = texture(decal_map, texcoord);
    decal_color = vec4(1.0 - decal_color.r, 1.0 - decal_color.g, 1.0 - decal_color.b, decal_color.a);
    fragment_color = decal_color * material_color;
}
