uniform sampler2D decal;
uniform sampler2D height;
uniform vec3 light_attenuation;
uniform float light_radius;
uniform vec3 light_color;

varying vec3 normal;
varying vec3 light;
varying vec3 view;
varying float light_distance;

void main(){
	float light_factor = 0.0;
	vec3 normal_vector = normalize(normal);
	vec3 light_vector = normalize(light);
	vec4 color_out = texture2D(decal, gl_TexCoord[0].st);

	float normal_dot_light_vector = max(dot(normal_vector, light_vector), 0.0);
	if (normal_dot_light_vector > 0.0){
		float attenuation_factor = (1.0 / (light_attenuation.x + light_attenuation.y * light_distance + light_attenuation.z * light_distance * light_distance)) * (1.0 - pow((light_distance / light_radius), 2.0));
		light_factor += normal_dot_light_vector * attenuation_factor;
		light_factor += pow(max(dot(normalize(reflect(light_vector, normal_vector)), normalize(view)), 0.0), 16.0) * 0.7 * attenuation_factor;
	}

	color_out *= vec4((light_color * light_factor), 1.0);
	gl_FragColor = color_out;
}
