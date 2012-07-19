#version 130

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 normal_matrix;

in vec4 vertex_position;
in vec4 vertex_normal;
in vec4 vertex_texcoord;
in vec4 vertex_binormal;
in vec4 vertex_bitangent;
in vec4 vertex_weights;
in ivec4 vertex_weight_indices;

smooth out vec3 world_position;
smooth out vec3 normal;
flat out vec3 sphere_center;

void main() {
    world_position = (model_matrix * vertex_position).xyz;
    normal = normalize(vertex_position.xyz);
    sphere_center = model_matrix[3].xyz;

    gl_Position = projection_matrix * view_matrix * model_matrix * vertex_position;
}
