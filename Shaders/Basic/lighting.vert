#version 130

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;
uniform vec4 light_position;
uniform vec3 camera_position;
uniform int has_animations;
uniform mat4 bone_transforms[64];

in vec4 vertex_position;
in vec4 vertex_normal;
in vec4 vertex_texcoord;
in vec4 vertex_binormal;
in vec4 vertex_bitangent;
in vec4 vertex_weights;
in vec4 vertex_weight_indices;

smooth out vec3 normal;
smooth out vec3 light;
smooth out vec3 view;
smooth out float light_distance;
smooth out vec2 texcoord;
smooth out vec3 binormal;
smooth out vec3 bitangent;

void main() {
    mat4 animation_matrix = mat4(1.0);
    if (has_animations == 1) {
        ivec4 indices = ivec4(vertex_weight_indices);
        animation_matrix = vertex_weights[0] * bone_transforms[indices[0]] +
                           vertex_weights[1] * bone_transforms[indices[1]] +
                           vertex_weights[2] * bone_transforms[indices[2]] +
                           vertex_weights[3] * bone_transforms[indices[3]];
    }
    vec3 transformed_vertex = (model_matrix * animation_matrix * vertex_position).xyz;
    normal = normalize((normal_matrix * animation_matrix * vec4(vertex_normal.xyz, 0.0)).xyz);

    vec3 light_vector = transformed_vertex - light_position.xyz;

    view = normalize(camera_position - transformed_vertex);

    light_distance = length(light_vector);
    light = normalize(light_vector);

    texcoord = vertex_texcoord.st;
    gl_Position = projection_matrix * view_matrix * vec4(transformed_vertex, 1.0);
}
