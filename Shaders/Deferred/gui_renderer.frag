uniform sampler2D decal;
uniform int use_decal;
uniform vec4 color;

//varying vec4 front_color;

void main(){
	if (use_decal == 1){
		gl_FragColor = texture2D(decal, gl_TexCoord[0].st) * color * gl_Color;// * front_color;
	}else{
		gl_FragColor = color * gl_Color;//front_color;
	}
	//gl_FragColor = color;
	//gl_FragColor = front_color;
}
