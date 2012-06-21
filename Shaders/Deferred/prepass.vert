#version 130

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
in ivec4 vertex_weight_indices;

smooth out vec3 position;
smooth out vec3 normal;
smooth out vec3 binormal;
smooth out vec3 bitangent;
smooth out vec2 texcoord;
smooth out float vertex_weighted;

void main(){
    vertex_weighted = 0.0;
    vec3 transformed_vertex = vec3(0.0);
    if (has_animations == 1) {
        vec4 temp_vertex = vec4(vertex_position);
        vec4 temp_normal = vec4(vertex_normal.xyz, 1.0);

        // do for all four
        if (vertex_weight_indices[0] < 1000) {
            vertex_weighted = 1.0;
            temp_vertex += (bone_transforms[vertex_weight_indices[0]] * temp_vertex) * vertex_weights[0];
        }
        if (vertex_weight_indices[1] < 1000) {
            temp_vertex += (bone_transforms[vertex_weight_indices[1]] * temp_vertex) * vertex_weights[1];
        }
        if (vertex_weight_indices[2] < 1000) {
            temp_vertex += (bone_transforms[vertex_weight_indices[2]] * temp_vertex) * vertex_weights[2];
        }
        if (vertex_weight_indices[3] < 1000) {
            temp_vertex += (bone_transforms[vertex_weight_indices[3]] * temp_vertex) * vertex_weights[3];
        }

        transformed_vertex = (model_matrix * temp_vertex).xyz;
        normal = normalize((normal_matrix * vec4(vertex_normal.xyz, 0.0)).xyz);
    } else {
        transformed_vertex = (model_matrix * vertex_position).xyz;
        normal = normalize((normal_matrix * vec4(vertex_normal.xyz, 0.0)).xyz);
    }

    position = transformed_vertex;
    texcoord = vertex_texcoord.st;
    normal = normalize(normal_matrix * vec4(vertex_normal.xyz, 0.0)).xyz;
    binormal = normalize(normal_matrix * vec4(vertex_binormal.xyz, 0.0)).xyz;
    bitangent = normalize(normal_matrix * vec4(vertex_bitangent.xyz, 0.0)).xyz;

    gl_Position = projection_matrix * view_matrix * vec4(transformed_vertex, 1.0);
}
