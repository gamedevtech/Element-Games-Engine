#version 130

uniform sampler2D depth_map;
uniform vec2 camerarange;
uniform vec2 screensize;

smooth in vec2 texcoord;

out vec4 fragment_output;

float ReadDepth(in vec2 coord){
	return (2.0 * camerarange.x) / (camerarange.y + camerarange.x - texture(depth_map, coord).x * (camerarange.y - camerarange.x));
}

float CompareDepths(in float depth1, in float depth2){
	float aoCap = 1.0;
	float aoMultiplier = 10000.0;
	float depthTolerance = 0.000;
	// units in space the AO effect extends to (this gets divided by the camera far range
	//float aorange = 10.0;
	float aorange = 1.0;
	float diff = sqrt(clamp(1.0 - (depth1 - depth2) / (aorange / (camerarange.y - camerarange.x)), 0.0, 1.0));
	float ao = min(aoCap, max(0.0, depth1 - depth2 - depthTolerance) * aoMultiplier) * diff;
	return ao;
}

void main(){
	float depth = ReadDepth(texcoord);
	float d;

	float pw = 1.0 / screensize.x;
	float ph = 1.0 / screensize.y;

	float aoCap = 1.0;

	float ao = 0.0;

	float aoMultiplier=10000.0;

	float depthTolerance = 0.001;

	float aoscale=1.0;

	d=ReadDepth( vec2(texcoord.x+pw,texcoord.y+ph));
	ao+=CompareDepths(depth,d)/aoscale;

	d=ReadDepth( vec2(texcoord.x-pw,texcoord.y+ph));
	ao+=CompareDepths(depth,d)/aoscale;

	d=ReadDepth( vec2(texcoord.x+pw,texcoord.y-ph));
	ao+=CompareDepths(depth,d)/aoscale;

	d=ReadDepth( vec2(texcoord.x-pw,texcoord.y-ph));
	ao+=CompareDepths(depth,d)/aoscale;

	pw*=2.0;
	ph*=2.0;
	aoMultiplier/=2.0;
	aoscale*=1.2;

	d=ReadDepth( vec2(texcoord.x+pw,texcoord.y+ph));
	ao+=CompareDepths(depth,d)/aoscale;

	d=ReadDepth( vec2(texcoord.x-pw,texcoord.y+ph));
	ao+=CompareDepths(depth,d)/aoscale;

	d=ReadDepth( vec2(texcoord.x+pw,texcoord.y-ph));
	ao+=CompareDepths(depth,d)/aoscale;

	d=ReadDepth( vec2(texcoord.x-pw,texcoord.y-ph));
	ao+=CompareDepths(depth,d)/aoscale;

	pw*=2.0;
	ph*=2.0;
	aoMultiplier/=2.0;
	aoscale*=1.2;

	d=ReadDepth( vec2(texcoord.x+pw,texcoord.y+ph));
	ao+=CompareDepths(depth,d)/aoscale;

	d=ReadDepth( vec2(texcoord.x-pw,texcoord.y+ph));
	ao+=CompareDepths(depth,d)/aoscale;

	d=ReadDepth( vec2(texcoord.x+pw,texcoord.y-ph));
	ao+=CompareDepths(depth,d)/aoscale;

	d=ReadDepth( vec2(texcoord.x-pw,texcoord.y-ph));
	ao+=CompareDepths(depth,d)/aoscale;

	pw*=2.0;
	ph*=2.0;
	aoMultiplier/=2.0;
	aoscale*=1.2;

	d=ReadDepth( vec2(texcoord.x+pw,texcoord.y+ph));
	ao+=CompareDepths(depth,d)/aoscale;

	d=ReadDepth( vec2(texcoord.x-pw,texcoord.y+ph));
	ao+=CompareDepths(depth,d)/aoscale;

	d=ReadDepth( vec2(texcoord.x+pw,texcoord.y-ph));
	ao+=CompareDepths(depth,d)/aoscale;

	d=ReadDepth( vec2(texcoord.x-pw,texcoord.y-ph));
	ao+=CompareDepths(depth,d)/aoscale;

	ao/=16.0;

	//ao = pow(ao * 2.0, 2.0);
	ao = clamp(ao, 0.25, 0.5) - 0.25;
	fragment_output = vec4(1.0 - ao, 1.0 - ao, 1.0 - ao, 1.0);

	//fragment_output = vec4(1.0, 1.0, 1.0, 1.0);
}
