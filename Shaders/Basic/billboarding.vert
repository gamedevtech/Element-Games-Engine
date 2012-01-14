uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 normal_matrix;

void main() {
  mat4 model_view_matrix = view_matrix * model_matrix;
  gl_Position = projection_matrix * (gl_Vertex + vec4(model_view_matrix[3].xyz, 0));
  gl_TexCoord[0] = gl_MultiTexCoord0;
}
