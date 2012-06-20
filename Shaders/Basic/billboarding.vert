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

smooth out vec2 texcoord;

void main() {
    mat4 model_view_matrix = view_matrix * model_matrix;
    vec4 scale = vec4(model_matrix[0][0], model_matrix[1][1], model_matrix[2][2], 1.0);
    vec4 translate = vec4(model_view_matrix[3].xyz, 0);
    vec3 position = ((vertex_position * scale) + translate).xyz;
    texcoord = vertex_texcoord.st;

    gl_Position = projection_matrix * vec4(position, 1.0);
}
