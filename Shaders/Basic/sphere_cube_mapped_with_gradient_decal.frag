uniform samplerCube height;
uniform sampler2D decal;
uniform vec4 color;

varying vec3 texcoord;

void main(){
	float gradient_index = textureCube(height, normalize(texcoord)).r;
	gl_FragColor = texture2D(decal, vec2(gradient_index, 0.5)) * color;
}
