#version 130

smooth in vec3 world_position;
smooth in vec3 normal;
flat in vec3 sphere_center;

uniform vec3 camera_position;
uniform float material_specularity;
uniform vec4 material_color;
uniform int normal_mapping_enabled;
uniform int receives_lighting;
uniform sampler2D decal_map;
uniform vec4 light_position;
uniform float inner_radius;
uniform float outer_radius;

out vec4 fragment0;
out vec4 fragment1;
out vec4 fragment2;
out vec4 fragment3;

void main() {
    // constants
    float stretch_amount = 0.0;
    float exposure = 1.0;
    float g = -0.990;
    float g2 = g * g;
    float tweak_amount = 0.025;

    // discard back pixels
    // Implement this generically in all shaders as Back face culling! Perhaps do this on the vertex?
    vec3 camera_to_position = world_position - camera_position;
    if (dot(normalize(normal), normalize(camera_to_position)) < 0.0) {
        discard;
    }

    float outer_radius2 = outer_radius * outer_radius;
    float camera_height = length(camera_position);
    float far_distance = length(camera_to_position);
    vec3 light_direction = normalize(light_position.xyz - camera_position);

    vec3 ray_direction = camera_to_position / far_distance;
    float camera_height2 = camera_height * camera_height;

    float B = 2.0 * dot(camera_position.xyz, ray_direction);
    float C = camera_height2 - outer_radius2;
    float det = max(00, B*B - 4.0 * C);
    float near_distance = 0.5 * (-B - sqrt(det));
    vec3 near_position = camera_position.xyz + (ray_direction * near_distance);
    vec3 near_normal = normalize(near_position);

    float lc = dot(light_direction, camera_position / camera_height);
    float ln = dot(light_direction, normal);
    float lnn = dot(light_direction, near_normal);

    float altitude = camera_height - inner_radius;
    float horizon_distance = sqrt((altitude * altitude) + (2.0 * inner_radius * altitude));
    float max_dot = horizon_distance / camera_height;

    altitude = max(0.0, camera_height - outer_radius);
    horizon_distance = sqrt((altitude * altitude) + (2.0 * outer_radius * altitude));

    float min_dot = max(tweak_amount, horizon_distance / camera_height);

    float min_dot2 = ((camera_height - inner_radius) * (1.0 / outer_radius - inner_radius)) - (1.0 - tweak_amount);
    min_dot = min(min_dot, min_dot2);

    float pos_dot = dot(camera_to_position / far_distance, -camera_position.xyz / camera_height) - min_dot;

    float height = pos_dot * (1.0 / (max_dot - min_dot));

    ln = max(0.0, ln + stretch_amount);
    lnn = max(0.0, lnn + stretch_amount);

    float brightness = clamp(ln + (lnn * lc), 0.0, 1.0);
    vec2 uv = vec2(brightness * clamp(lc + 1.0 + stretch_amount, 0.0, 1.0), height);

    height -= min(0.0, min_dot2 + (ln * min_dot2));
    float alpha = height * brightness;

    vec3 negative_ray_dir = -ray_direction;
    vec3 new_light_direction = normalize(light_position.xyz - world_position.xyz);
    // Switch

    vec4 diffuse = texture(decal_map, uv);
    vec4 diffuse2 = texture(decal_map, vec2(min(0.5, uv.x), 1.0));

    //float cosine = dot(normalize(new_light_direction.xyz), normalize(negative_ray_dir));
    float cosine = dot(new_light_direction.xyz, negative_ray_dir) * inversesqrt(dot(new_light_direction.xyz, new_light_direction.xyz) * dot(negative_ray_dir, negative_ray_dir));
    float cosine2 = cosine * cosine;

    vec4 diffuse_color = diffuse * alpha;
    float mie_phase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + cosine2) /(1.0 + g2 - 2.0 * g * cosine);
    vec4 mie_color = diffuse2 * mie_phase * alpha;

    vec4 color_out = vec4(1.0) - exp((diffuse_color * (1.0 + uv.y) + mie_color) * -exposure);
    //color_out.a = alpha;
    /*if (alpha < 0.0 || alpha > 1.0) {
        discard;
    }*/

    fragment0 = vec4(0.0, 0.0, 0.0, color_out.a);
    fragment1 = vec4(0.0, 0.0, 0.0, color_out.a);
    fragment2 = vec4(0.0, 0.0, 0.0, color_out.a);
    fragment3 = vec4(color_out);
}
