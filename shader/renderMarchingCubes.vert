#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 ourColor;
out vec4 reflectionMapCoordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 reflectionView;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);

    //ourColor = vec3(0.3f, 0.8f, 0.2f);
    ourColor = normal;

    reflectionMapCoordinates = projection * reflectionView * model * vec4(position, 1.0f);
}

