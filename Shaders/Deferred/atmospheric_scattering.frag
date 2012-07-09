// http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter16.html

#version 130

smooth in vec3 color0;
smooth in vec3 color1;
smooth in vec3 texcoord0;

uniform float material_specularity;
uniform vec4 material_color;
uniform int normal_mapping_enabled;
uniform int receives_lighting;
uniform vec3 light_direction;
uniform float g;
uniform float g2;

out vec4 fragment0;
out vec4 fragment1;
out vec4 fragment2;
out vec4 fragment3;

void main(){
    //float factor = 1.0 - length(cross(normalize(normal), normalize(vec3(0.0, 0.0, -1.0))));

    fragment0 = vec4(0.0, 0.0, 0.0, factor);
    fragment1 = vec4(0.0, 0.0, 0.0, factor);
    fragment2 = vec4(0.0, 0.0, 0.0, factor);
    //fragment3 = vec4(material_color.rgb, factor);

    float cosine = dot(light_direction, texcoord0) / length(texcoord0);
    float cosine2 = cosine * cosine;
    vec4 color = getRayleighPhase(cosine2) * color0 + getMiePhase(cosine, cosine2, g, g2) * color1;
    color.a = color.b;
    fragment3 = color;
}
