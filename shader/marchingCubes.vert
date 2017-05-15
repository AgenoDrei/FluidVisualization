#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 ourColor;
out vec4 reflectionMapCoordinates;
out vec4 fragPosLight;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 reflectionView;
uniform mat4 depthBiasMVP;

//varying vec3 interpolatedVertexObject;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    fragPosLight = depthBiasMVP * vec4(position,1);
    fragPos = vec3(model * vec4(position, 1.0));

    //ourColor = vec3(0.3f, 0.8f, 0.2f);
    ourColor = normal;

    reflectionMapCoordinates = projection * reflectionView * model * vec4(position, 1.0f);
    //interpolatedVertexObject = position - vec3(0, 0.2, 0);


}

