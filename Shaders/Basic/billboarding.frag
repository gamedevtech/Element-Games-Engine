#version 130

smooth in vec2 texcoord;

uniform sampler2D decal_map;
uniform sampler2D height_map; // ignore
uniform vec4 material_color;

out vec4 fragment_color;

void main(){
    fragment_color = texture(decal_map, texcoord) * material_color;
}
