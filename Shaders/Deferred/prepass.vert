#version 130

#define max_bones 60

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;

uniform int has_animations;
uniform mat4 bone_transforms[60];

in vec4 vertex_position;
in vec4 vertex_normal;
in vec4 vertex_texcoord;
in vec4 vertex_binormal;
in vec4 vertex_bitangent;
in vec4 vertex_weights;
in vec4 vertex_weight_indices;

smooth out vec3 position;
smooth out vec3 normal;
smooth out vec3 binormal;
smooth out vec3 bitangent;
smooth out vec2 texcoord;

void main(){
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

    position = transformed_vertex;
    texcoord = vertex_texcoord.st;
    normal = normalize(normal_matrix * vec4(vertex_normal.xyz, 0.0)).xyz;
    binormal = normalize(normal_matrix * vec4(vertex_binormal.xyz, 0.0)).xyz;
    bitangent = normalize(normal_matrix * vec4(vertex_bitangent.xyz, 0.0)).xyz;

    gl_Position = projection_matrix * view_matrix * vec4(transformed_vertex, 1.0);
}
