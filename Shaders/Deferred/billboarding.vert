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

void main() {
	mat4 model_view_matrix = view_matrix * model_matrix;
	vec4 scale = vec4(model_matrix[0][0], model_matrix[1][1], model_matrix[2][2], 1.0);
	vec4 translate = vec4(model_view_matrix[3].xyz, 0);
	position = ((vertex_position * scale) + translate).xyz;
	texcoord = vertex_texcoord.st;
	normal = normalize(normal_matrix * vec4(vertex_normal.xyz, 0.0)).xyz;
	binormal = normalize(normal_matrix * vec4(vertex_binormal.xyz, 0.0)).xyz;
	bitangent = normalize(normal_matrix * vec4(vertex_bitangent.xyz, 0.0)).xyz;

	gl_Position = projection_matrix * vec4(position, 1.0);
}
