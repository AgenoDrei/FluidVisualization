#version 330 core
layout(location = 0) in vec3 position; //object space vertex position

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vUV; //3D texture coordinates for texture lookup in the fragment shader
out vec3 posViewSpace;



void main() {
	//get the clipspace position
	gl_Position =  projection * view * model * vec4(position, 1.0f);

	//get the 3D texture coordinates by adding (0.5,0.5,0.5) to the object space
	//vertex position. Since the unit cube is at origin (min: (-0.5,-0.5,-0.5) and max: (0.5,0.5,0.5))
	//adding (0.5,0.5,0.5) to the unit cube object space position gives us values from (0,0,0) to
	//(1,1,1)
	vUV = position;
	posViewSpace = vec3(view * model * vec4(position, 1.0f));

}
