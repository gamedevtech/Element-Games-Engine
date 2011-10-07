uniform sampler2D decal;
uniform sampler2D height; // ignore
uniform vec4 color;

void main(){
	gl_FragColor = texture2D(decal, gl_TexCoord[0].st) * color;
}
