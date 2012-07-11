#version 130

smooth in vec3 camera_to_position;
smooth in vec2 uv_coord;
smooth in float alpha;
smooth in vec3 light_direction;

uniform float material_specularity;
uniform vec4 material_color;
uniform int normal_mapping_enabled;
uniform int receives_lighting;
uniform sampler2D decal_map;

out vec4 fragment0;
out vec4 fragment1;
out vec4 fragment2;
out vec4 fragment3;

void main() {
    const float exposure = 1.5;
    float g = -0.990;
    float g2 = g * g;
    vec4 diffuse = texture(decal_map, uv_coord);
    vec4 diffuse2 = texture(decal_map, vec2(min(0.5, uv_coord.x), 1.0));
    float cosine = dot(normalize(light_direction), normalize(camera_to_position));
    float cosine2 = cosine * cosine;
    vec4 diffuse_color = diffuse * alpha;

    float mie_phase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + cosine2) / (1.0 + g2 - 2.0 * g * cosine);
    vec4 mie_color = diffuse2 * mie_phase * alpha;

    float factor = max(alpha, 0.5);
    fragment0 = vec4(0.0, 0.0, 0.0, factor);
    fragment1 = vec4(0.0, 0.0, 0.0, factor);
    fragment2 = vec4(0.0, 0.0, 0.0, factor);

    vec4 color_out = vec4(1.0) - exp((diffuse_color * (1.0 + uv_coord.y) + mie_color) * -exposure);
    fragment3 = vec4(color_out.rgb, factor);
}
