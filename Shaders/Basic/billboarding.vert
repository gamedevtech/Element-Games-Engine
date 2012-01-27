uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 normal_matrix;

void main() {
	mat4 model_view_matrix = view_matrix * model_matrix;
	vec4 scale = vec4(model_matrix[0][0], model_matrix[1][1], model_matrix[2][2], 1.0);
	vec4 translate = vec4(model_view_matrix[3].xyz, 0);
	vec3 position = ((gl_Vertex * scale) + translate).xyz;
	gl_TexCoord[0] = gl_MultiTexCoord0;

	gl_Position = projection_matrix * vec4(position, 1.0);
}
