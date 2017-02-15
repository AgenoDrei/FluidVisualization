#version 330 core

in vec3 ourColor;
in vec4 reflectionMapCoordinates;

out vec4 color;

uniform sampler2D ourTexture;

void main() {
    float transperency = 0.3;

    vec2 reflectionCoordinates = vec2(reflectionMapCoordinates.x / reflectionMapCoordinates.w / 2.0f + 0.5f, reflectionMapCoordinates.y / reflectionMapCoordinates.w / 2.0f + 0.5f);
    vec4 reflectiveColor = texture(ourTexture, reflectionCoordinates);
    reflectiveColor.w = transperency;

    vec4 dullColor = vec4(0.1f, 0.1f, 0.2f, 1.0f);
    float dullBlendFacor = 0.3f;
    color = (dullBlendFacor * dullColor) + reflectiveColor;
}
