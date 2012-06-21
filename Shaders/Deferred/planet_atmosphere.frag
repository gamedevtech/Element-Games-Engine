#version 130

smooth in float factor;

uniform float material_specularity;
uniform vec4 material_color;
uniform int normal_mapping_enabled;
uniform int receives_lighting;

out vec4 fragment0;
out vec4 fragment1;
out vec4 fragment2;
out vec4 fragment3;

void main(){
    fragment0 = vec4(0.0, 0.0, 0.0, factor);
    fragment1 = vec4(0.0, 0.0, 0.0, factor);
    fragment2 = vec4(0.0, 0.0, 0.0, factor);
    fragment3 = vec4(material_color.rgb, factor);
}
