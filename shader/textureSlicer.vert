#version 330 core
layout(location = 0) in vec3 vVertex; //object space vertex position

smooth out vec3 vUV; //3D texture coordinates for texture lookup in the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(vVertex.xyz, 1);

	vUV = vVertex;
}
