uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 normal_matrix;

varying vec3 normal;

void main(){
	normal = normalize((view_matrix * model_matrix * vec4(gl_Vertex.xyz, 1.0)).xyz);
	gl_Position = projection_matrix * view_matrix * model_matrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
