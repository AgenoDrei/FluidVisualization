#version 330 core
layout(location = 0) in vec3 position; //object space vertex position

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vUV; //3D texture coordinates for texture lookup in the fragment shader



void main() {
	//get the clipspace position
	gl_Position =  projection * view * model * vec4(position, 1.0f);
	vUV = position;
}
