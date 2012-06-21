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

smooth out float factor;

void main(){
    vec3 position = (model_matrix * vertex_position).xyz;
    vec3 normal = normalize(view_matrix * normal_matrix * vec4(vertex_normal.xyz, 0.0)).xyz;
    factor = 1.0 - dot(normalize(normal), normalize(vec3(0.0, 0.0, -1.0)));

    gl_Position = projection_matrix * view_matrix * vec4(position, 1.0);
}
