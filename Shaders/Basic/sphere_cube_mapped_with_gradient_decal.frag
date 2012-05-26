uniform samplerCube height_map;
uniform sampler2D decal_map;
uniform vec4 material_color;

varying vec3 texcoord;

void main(){
	float gradient_index = textureCube(height_map, normalize(texcoord)).r;
	gl_FragColor = texture2D(decal_map, vec2(gradient_index, 0.5)) * material_color;
}
