uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

//varying vec4 front_color;

void main(){
	//front_color = gl_Color;
	gl_FrontColor = gl_Color;
	gl_Position = projection_matrix * view_matrix * model_matrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
