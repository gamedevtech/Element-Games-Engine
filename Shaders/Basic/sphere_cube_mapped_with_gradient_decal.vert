uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 normal_matrix;

varying vec3 texcoord;

void main(){
	gl_Position = projection_matrix * view_matrix * model_matrix * gl_Vertex;
	texcoord = normalize(gl_Vertex.xyz);
}
