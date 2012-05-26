//#version 150
#version 130

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;
uniform vec2 resolution;

in vec4 vertex_position;
in vec4 vertex_normal;
in vec4 vertex_texcoord;
in vec4 vertex_binormal;
in vec4 vertex_bitangent;

smooth out vec2 texcoord;

void main(){
	// Sphere Method
	/*vec4 position = projection_matrix * view_matrix * model_matrix * vertex_position;
	texcoord = position.st / resolution;
	gl_Position = position; */

	// Quad Method
	texcoord = vertex_texcoord.st;
	gl_Position = projection_matrix * view_matrix * model_matrix * vertex_position;
}
