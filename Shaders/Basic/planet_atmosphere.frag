uniform sampler2D decal;
uniform sampler2D height; // ignore
uniform vec4 color;

varying vec3 normal;

void main(){
	float factor = 1.0 - dot(normalize(normal), normalize(vec3(0.0, 0.0, -1.0))) + 0.1;
	gl_FragColor = vec4(color.r, color.g, color.b, factor);
}
