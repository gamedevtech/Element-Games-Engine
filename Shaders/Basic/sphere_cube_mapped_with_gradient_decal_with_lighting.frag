uniform sampler2D decal;
uniform samplerCube height;
uniform vec3 light_attenuation;
uniform float light_radius;
uniform vec3 light_color;

varying vec3 texcoord;
varying vec3 normal;
varying vec3 light;
varying vec3 view;
varying float light_distance;

void main(){
	//float light_factor = 0.0;
	vec3 normal_vector = normalize(normal);
	vec3 light_vector = normalize(light);
	float gradient_index = textureCube(height, normalize(texcoord)).r;
	vec4 color_out = texture2D(decal, vec2(gradient_index, 0.5));

	float normal_dot_light_vector = max(dot(normal_vector, light_vector), 0.0);
	float diffuse_factor = 0.0;
	float specular_factor = 0.0;
	if (normal_dot_light_vector > 0.0){
		float attenuation_factor = (1.0 / (light_attenuation.x + light_attenuation.y * light_distance + light_attenuation.z * light_distance * light_distance)) * (1.0 - pow((light_distance / light_radius), 2.0));

		diffuse_factor = normal_dot_light_vector * attenuation_factor;
		specular_factor = pow(max(dot(-normalize(reflect(light_vector, normal_vector)), normalize(view)), 0.0), 16.0) * 0.5 * attenuation_factor;
	}

	color_out *= vec4((light_color * diffuse_factor), 1.0);
	color_out += vec4((light_color * specular_factor), 0.0);
	gl_FragColor = color_out;
}
