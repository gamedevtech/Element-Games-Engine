#version 130

uniform sampler2D depth_map;
uniform sampler2D color_map;
uniform vec2 camerarange;
uniform vec2 screensize;
uniform vec2 middle_texcoord;
uniform float blur_clamp;
uniform float bias;

smooth in vec2 texcoord;

out vec4 fragment_output;

void main(){
	float aspect_ratio = screensize.x / screensize.y;
	vec2 aspect_correct = vec2(1.0, aspect_ratio);

	float focus_depth = (2.0 * camerarange.x) / (camerarange.y + camerarange.x - texture(depth_map, middle_texcoord).x * (camerarange.y - camerarange.x));
	float depth = (2.0 * camerarange.x) / (camerarange.y + camerarange.x - texture(depth_map, texcoord).x * (camerarange.y - camerarange.x));
	float factor = (depth - focus_depth);
	vec2 dof_blur = vec2(clamp(factor * bias, -blur_clamp, blur_clamp));

	vec4 blurred_output = vec4(0.0, 0.0, 0.0, 0.0);

	blurred_output += texture(color_map, texcoord);
	blurred_output += texture(color_map, texcoord + (vec2(0.0, 0.4) * aspect_correct) * dof_blur);
	blurred_output += texture(color_map, texcoord + (vec2(0.15, 0.37) * aspect_correct) * dof_blur);
	blurred_output += texture(color_map, texcoord + (vec2(0.29, 0.29) * aspect_correct) * dof_blur);
	blurred_output += texture(color_map, texcoord + (vec2(-0.37, 0.15) * aspect_correct) * dof_blur);	
	blurred_output += texture(color_map, texcoord + (vec2(0.4, 0.0) * aspect_correct) * dof_blur);	
	blurred_output += texture(color_map, texcoord + (vec2(0.37, -0.15) * aspect_correct) * dof_blur);	
	blurred_output += texture(color_map, texcoord + (vec2(0.29, -0.29) * aspect_correct) * dof_blur);	
	blurred_output += texture(color_map, texcoord + (vec2(-0.15, -0.37) * aspect_correct) * dof_blur);
	blurred_output += texture(color_map, texcoord + (vec2(0.0, -0.4) * aspect_correct) * dof_blur);	
	blurred_output += texture(color_map, texcoord + (vec2(-0.15, 0.37) * aspect_correct) * dof_blur);
	blurred_output += texture(color_map, texcoord + (vec2(-0.29, 0.29) * aspect_correct) * dof_blur);
	blurred_output += texture(color_map, texcoord + (vec2(0.37, 0.15) * aspect_correct) * dof_blur);	
	blurred_output += texture(color_map, texcoord + (vec2(-0.4, 0.0) * aspect_correct) * dof_blur);	
	blurred_output += texture(color_map, texcoord + (vec2(-0.37, -0.15) * aspect_correct) * dof_blur);	
	blurred_output += texture(color_map, texcoord + (vec2(-0.29, -0.29) * aspect_correct) * dof_blur);	
	blurred_output += texture(color_map, texcoord + (vec2(0.15, -0.37) * aspect_correct) * dof_blur);
	
	blurred_output += texture(color_map, texcoord + (vec2(0.15, 0.37) * aspect_correct) * dof_blur * 0.9);
	blurred_output += texture(color_map, texcoord + (vec2(-0.37, 0.15) * aspect_correct) * dof_blur * 0.9);		
	blurred_output += texture(color_map, texcoord + (vec2(0.37, -0.15) * aspect_correct) * dof_blur * 0.9);		
	blurred_output += texture(color_map, texcoord + (vec2(-0.15, -0.37) * aspect_correct) * dof_blur * 0.9);
	blurred_output += texture(color_map, texcoord + (vec2(-0.15, 0.37) * aspect_correct) * dof_blur * 0.9);
	blurred_output += texture(color_map, texcoord + (vec2(0.37, 0.15) * aspect_correct) * dof_blur * 0.9);		
	blurred_output += texture(color_map, texcoord + (vec2(-0.37, -0.15) * aspect_correct) * dof_blur * 0.9);	
	blurred_output += texture(color_map, texcoord + (vec2(0.15, -0.37) * aspect_correct) * dof_blur * 0.9);	
	
	blurred_output += texture(color_map, texcoord + (vec2(0.29, 0.29) * aspect_correct) * dof_blur * 0.7);
	blurred_output += texture(color_map, texcoord + (vec2(0.4, 0.0) * aspect_correct) * dof_blur * 0.7);	
	blurred_output += texture(color_map, texcoord + (vec2(0.29, -0.29) * aspect_correct) * dof_blur * 0.7);	
	blurred_output += texture(color_map, texcoord + (vec2(0.0, -0.4) * aspect_correct) * dof_blur * 0.7);	
	blurred_output += texture(color_map, texcoord + (vec2(-0.29, 0.29) * aspect_correct) * dof_blur * 0.7);
	blurred_output += texture(color_map, texcoord + (vec2(-0.4, 0.0) * aspect_correct) * dof_blur * 0.7);	
	blurred_output += texture(color_map, texcoord + (vec2(-0.29, -0.29) * aspect_correct) * dof_blur * 0.7);	
	blurred_output += texture(color_map, texcoord + (vec2(0.0, 0.4) * aspect_correct) * dof_blur * 0.7);
			 
	blurred_output += texture(color_map, texcoord + (vec2(0.29, 0.29) * aspect_correct) * dof_blur * 0.4);
	blurred_output += texture(color_map, texcoord + (vec2(0.4, 0.0) * aspect_correct) * dof_blur * 0.4);	
	blurred_output += texture(color_map, texcoord + (vec2(0.29, -0.29) * aspect_correct) * dof_blur * 0.4);	
	blurred_output += texture(color_map, texcoord + (vec2(0.0, -0.4) * aspect_correct) * dof_blur * 0.4);	
	blurred_output += texture(color_map, texcoord + (vec2(-0.29, 0.29) * aspect_correct) * dof_blur * 0.4);
	blurred_output += texture(color_map, texcoord + (vec2(-0.4, 0.0) * aspect_correct) * dof_blur * 0.4);	
	blurred_output += texture(color_map, texcoord + (vec2(-0.29, -0.29) * aspect_correct) * dof_blur * 0.4);	
	blurred_output += texture(color_map, texcoord + (vec2(0.0, 0.4) * aspect_correct) * dof_blur * 0.4);	

	fragment_output = vec4(blurred_output.rgb / 41.0, 1.0);
}
