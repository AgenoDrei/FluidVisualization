#version 330 core
layout(location = 0) in vec3 vVertex; //object space vertex position

smooth out vec3 vUV; //3D texture coordinates for texture lookup in the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 pos_eye;
out vec3 n_eye;


void main() {
    gl_Position = projection * view * model * vec4(vVertex.xyz, 1);

	vUV = vVertex;

	pos_eye = vec3(view * model * vec4(vVertex.xyz, 1.0));
    n_eye = vec3(view * model * vec4(0.0f, 1.0f, 0.0f, 0.0));
}
