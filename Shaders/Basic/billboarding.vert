uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 normal_matrix;

void main() {
	mat4 model_view_matrix = view_matrix * model_matrix;
	vec4 scale = vec4(model_view_matrix[0][0], model_view_matrix[1][1], model_view_matrix[2][2], 1.0);
	vec4 position = vec4(model_view_matrix[3].xyz, 0);
	gl_Position = projection_matrix * ((gl_Vertex * scale) + position);
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
