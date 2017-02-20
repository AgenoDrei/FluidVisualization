#version 330 core

in vec3 ourColor;
in vec4 reflectionMapCoordinates;

out vec4 color;

uniform sampler2D ourTexture;


/*uniform mat4 reflectionView;
varying vec3 interpolatedVertexObject;*/

void main() {
    float transperency = 0.3;

    vec2 reflectionCoordinates = vec2(reflectionMapCoordinates.x / reflectionMapCoordinates.w / 2.0f + 0.5f, reflectionMapCoordinates.y / reflectionMapCoordinates.w / 2.0f + 0.5f);
    vec4 reflectiveColor = texture(ourTexture, reflectionCoordinates);
    reflectiveColor.w = transperency;


    vec4 dullColor = vec4(0.1f, 0.1f, 0.2f, 1.0f);
    float dullBlendFacor = 0.3f;
    color = (dullBlendFacor * dullColor) + reflectiveColor;

    /*vec4 vClipReflection = reflectionView * vec4(interpolatedVertexObject.xy, 0.0 , 1.0);
    vec2 vDeviceReflection = vClipReflection.st / vClipReflection.q;
    vec2 vTextureReflection = vec2(0.5, 0.5) + 0.5 * vDeviceReflection;

    vec4 reflectionTextureColor = texture2D(ourTexture, vTextureReflection);

    // Framebuffer reflection can have alpha > 1
    reflectionTextureColor.a = 1.0;

    color = reflectionTextureColor;*/

}
