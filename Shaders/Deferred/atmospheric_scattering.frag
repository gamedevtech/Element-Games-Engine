// http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter16.html

#version 130

smooth in vec3 color0;
smooth in vec3 color1;
smooth in vec3 direction;

uniform float material_specularity;
uniform vec4 material_color;
uniform int normal_mapping_enabled;
uniform int receives_lighting;

const float g = -0.990;
const float g2 = 0.9801;

out vec4 fragment0;
out vec4 fragment1;
out vec4 fragment2;
out vec4 fragment3;

void main(){
    vec3 light_position = vec3(0.0, 0.0, 0.0); // NOTE: Hacked in for now.

    float cosine = dot(light_position, direction) / length(direction);
    float mie_phase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + cosine*cosine) / pow(1.0 + g2 - 2.0 * g * cosine, 1.5);
    vec3 color = color0 + mie_phase * color1;

    fragment0 = vec4(0.0, 0.0, 0.0, color.b);
    fragment1 = vec4(0.0, 0.0, 0.0, color.b);
    fragment2 = vec4(0.0, 0.0, 0.0, color.b);
    fragment3 = vec4(color.r, color.g, color.b, color.b);
}
