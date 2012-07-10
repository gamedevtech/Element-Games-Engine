#version 130

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 normal_matrix;

uniform vec3 camera_position;

in vec4 vertex_position;
in vec4 vertex_normal;
in vec4 vertex_texcoord;
in vec4 vertex_binormal;
in vec4 vertex_bitangent;
in vec4 vertex_weights;
in ivec4 vertex_weight_indices;

smooth out vec3 camera_to_position;
smooth out vec2 uv_coord;
smooth out float alpha;
smooth out vec3 light_direction;

void main(){
    vec3 light_position = vec3(0.0, 6.0, -6.0);
    float inner_radius = 1.0;
    float outer_radius = 1.1;
    float stretch_amount = 0.01;
    float outer_radius2 = outer_radius * outer_radius;

    camera_to_position = vertex_position.xyz - camera_position;
    float far_distance = length(camera_to_position);
    light_direction = normalize(vertex_position.xyz - light_position);
    vec3 normal = normalize(vertex_position.xyz);
    vec3 ray_direction = camera_to_position / far_distance;
    float camera_height = length(camera_position - (model_matrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz/*((model_matrix * vertex_position).xyz)*/);
    float camera_height2 = camera_height * camera_height;

    float B = 2.0 * dot(camera_position, ray_direction);
    float C = camera_height2 - outer_radius2;
    float det = max(0.0, B*B - 4.0 * C);
    float near_distance = 0.5 * (-B - sqrt(det));
    vec3 near_position = camera_position + (ray_direction * near_distance);
    vec3 near_normal = normalize(near_position);

    float lc = dot(light_direction, camera_position / camera_height);
    float ln = dot(light_direction, normal);
    float lnn = dot(light_direction, near_normal);

    float altitude = camera_height - inner_radius;
    float horizon_distance = sqrt((altitude * altitude) + (2.0 * inner_radius * altitude));
    float max_dot = horizon_distance / camera_height;

    altitude = max(0.0, camera_height - outer_radius);
    horizon_distance = sqrt((altitude * altitude) + (2.0 * outer_radius * altitude));

    float tweak_amount = 0.1;
    float min_dot = max(tweak_amount, horizon_distance / camera_height);

    float min_dot2 = ((camera_height - outer_radius) * (1.0 / (outer_radius - inner_radius))) - (1.0 - tweak_amount);
    min_dot = min(min_dot, min_dot2);

    float pos_dot = dot(camera_to_position / far_distance, -camera_position / camera_height) - min_dot;

    float height = pos_dot * (1.0 / (max_dot - min_dot));

    ln = max(0.0, ln + stretch_amount);
    lnn = max(0.0, lnn + stretch_amount);

    float brightness = clamp(ln + (lnn * lc), 0.0, 1.0);

    uv_coord.x = brightness * clamp(lc + 1.0 + stretch_amount, 0.0, 1.0);
    uv_coord.y = height;

    height -= min(0.0, min_dot2 + (ln * min_dot2));
    alpha = height * brightness;
    camera_to_position = -ray_direction;
    light_direction = normalize(light_position - vertex_position.xyz);

    gl_Position = projection_matrix * view_matrix * model_matrix * vertex_position;
}
