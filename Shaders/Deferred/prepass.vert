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

smooth out vec3 position;
smooth out vec3 normal;
smooth out vec3 binormal;
smooth out vec3 bitangent;
smooth out vec2 texcoord;

void main(){
	position = (model_matrix * vertex_position).xyz;
	texcoord = vertex_texcoord.st;
	normal = normalize(normal_matrix * vec4(vertex_normal.xyz, 0.0)).xyz;
	binormal = normalize(normal_matrix * vec4(vertex_binormal.xyz, 0.0)).xyz;
	bitangent = normalize(normal_matrix * vec4(vertex_bitangent.xyz, 0.0)).xyz;

	gl_Position = projection_matrix * view_matrix * model_matrix * vertex_position;
}
