#version 410

layout(location=0) in vec4 vertex_position;
layout(location=1) in vec4 vertex_normal;
layout(location=2) in vec4 vertex_texcoord;
layout(location=3) in vec4 vertex_binormal;
layout(location=4) in vec4 vertex_bitangent;

out vec4 vPosition;
out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vBinormal;
out vec3 vBitangent;
out float vDistance;

void main(){
	vPosition = vertex_position;//model_matrix * view_matrix * vertex_position;
	vTexCoord = vertex_texcoord.st;
	vNormal = vertex_normal.xyz;//(normal_matrix * vertex_normal).xyz;
	vBinormal = vertex_binormal.xyz;//(normal_matrix * vertex_binormal).xyz;
	vBitangent = vertex_bitangent.xyz;//(normal_matrix * vertex_bitangent).xyz;
}
