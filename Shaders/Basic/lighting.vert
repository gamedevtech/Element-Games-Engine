uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;
uniform vec4 light_position;
uniform vec3 camera_position;

varying vec3 normal;
varying vec3 light;
varying vec3 view;
varying float light_distance;

void main(){
	normal = (normal_matrix * vec4(gl_Normal.xyz, 1.0)).xyz;

	vec3 transformed_vertex = (model_matrix * gl_Vertex).xyz;
	vec3 light_vector = transformed_vertex - light_position.xyz;

	view = normalize(camera_position - transformed_vertex);

	light_distance = length(light_vector);
	light = normalize(light_vector);

	gl_Position = projection_matrix * view_matrix * model_matrix * gl_Vertex;
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
