#version 130

uniform sampler2D out_map;

smooth in vec2 texcoord;
smooth in vec4 position;

out vec4 fragment_output;

void main(){
	//if (position.x <= 1.0 || (position.x >= 399.5 && position.x <= 400.5)|| position.y <= 1.0 || (position.y >= 249.5 && position.y <= 250.5) || position.x >= 799.0 || position.y >= 499.0){
	//	fragment_output = vec4(1.0, 1.0, 1.0, 1.0);
	//}else{
		//fragment_output = vec4(texture(out_map, texcoord).rgb, 1.0);
	//}

	fragment_output = vec4(texture(out_map, texcoord).rgb, 1.0);
}
