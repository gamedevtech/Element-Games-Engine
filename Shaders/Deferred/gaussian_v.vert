#version 130

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

in vec4 vertex_position;
in vec4 vertex_normal;
in vec4 vertex_texcoord;
in vec4 vertex_binormal;
in vec4 vertex_bitangent;

smooth out vec2 texcoord;

void main(){
	texcoord = vertex_texcoord.st;
	gl_Position = projection_matrix * view_matrix * model_matrix * vertex_position;
}
