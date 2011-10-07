#version 130

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;
uniform vec4 light_position;
uniform vec3 camera_position;

in vec4 vertex_position;
in vec4 vertex_normal;
in vec4 vertex_texcoord;
in vec4 vertex_binormal;
in vec4 vertex_bitangent;

smooth out vec3 normal;
smooth out vec3 binormal;
smooth out vec3 bitangent;
smooth out vec3 light;
smooth out vec3 view;
smooth out float light_distance;
smooth out vec2 texcoord;

void main(){
	normal = normalize(normal_matrix * vec4(vertex_normal.xyz, 0.0)).xyz;
	binormal = normalize(normal_matrix * vec4(vertex_binormal.xyz, 0.0)).xyz;
	bitangent = normalize(normal_matrix * vec4(vertex_bitangent.xyz, 0.0)).xyz;

	vec3 transformed_vertex = (model_matrix * vertex_position).xyz;
	vec3 light_vector = transformed_vertex - light_position.xyz;

	view = normalize(camera_position - transformed_vertex);

	light_distance = length(light_vector);
	light = normalize(light_vector);

	gl_Position = projection_matrix * view_matrix * model_matrix * vertex_position;
	texcoord = vertex_texcoord.st;
}
