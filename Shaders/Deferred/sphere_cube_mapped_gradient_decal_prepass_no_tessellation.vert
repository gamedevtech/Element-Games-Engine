#version 130

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;

in vec4 vertex_position;
in vec4 vertex_normal;
in vec4 vertex_texcoord;
in vec4 vertex_binormal;
in vec4 vertex_bitangent;
in vec4 vertex_weights;
in ivec4 vertex_weight_indices;

smooth out vec3 position;
smooth out vec3 normal;
smooth out vec3 texcoord;
smooth out vec3 binormal;
smooth out vec3 bitangent;

void main(){
    texcoord = normalize(vertex_position).xyz;
    normal = normalize(normal_matrix * vertex_position).xyz;
    position = (model_matrix * vertex_position).xyz;
    binormal = normalize(normal_matrix * vec4(vertex_binormal.xyz, 0.0)).xyz;
    bitangent = normalize(normal_matrix * vec4(vertex_bitangent.xyz, 0.0)).xyz;
    gl_Position = projection_matrix * view_matrix * (model_matrix * vertex_position);
}
