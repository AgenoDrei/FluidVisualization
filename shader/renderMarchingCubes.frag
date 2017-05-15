#version 330 core

in vec3 ourColor;
in vec4 reflectionMapCoordinates;
in vec4 fragPosLight;
in vec3 fragPos;

out vec4 color;

uniform sampler2D ourTexture;
uniform sampler2D shadowMap;

uniform float reflection;
uniform float shadow;


/*uniform mat4 reflectionView;
varying vec3 interpolatedVertexObject;*/

void main() {
    float visibility = 1.0;
    /*if(shadow != 0) {
        if(shadowCoord.z > 0.1) {
            visibility = 1;
        } else {
            vec3 projCoords = shadowCoord.xyz / shadowCoord.w;
            projCoords = projCoords * 0.5 + 0.5;
            if(texture(shadowMap, projCoords.xy).r < projCoords.z){
                visibility = 0.5f;
                //visibility = texture(shadowMap, shadowCoord.xy).z;
            }
        }
    }*/
    vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    if (shadow != 0) {
        float currentDepth = projCoords.z;
        float shadow = currentDepth - 0.05 > closestDepth ? 1.0 : 0.0;
        if(shadow == 1.0f){
            visibility = 0.5f;
        }
    }

    //visibility = texture( shadowMap, vec3(shadowCoord.xy, (shadowCoord.z)/shadowCoord.w) );
    //float visibility = texture( shadowMap, vec3(shadowCoord.xy, (shadowCoord.z)/shadowCoord.w) );

    //color = vec4(texture(shadowMap, shadowCoord.xy).rgb, 1.0f);
    //return;
    //float visibility = texture(shadowMap, vec3(shadowCoord.xy, (shadowCoord.z)/shadowCoord.w));

    if(reflection != 0) {
        float transperency = 0.3;

        vec2 reflectionCoordinates = vec2(reflectionMapCoordinates.x / reflectionMapCoordinates.w / 2.0f + 0.5f, -reflectionMapCoordinates.y / reflectionMapCoordinates.w / 2.0f + 0.5f);
        vec4 reflectiveColor = texture(ourTexture, reflectionCoordinates);
        reflectiveColor.w = transperency;


        vec4 dullColor = vec4(0.1f, 0.1f, 0.2f, 1.0f);
        float dullBlendFacor = 0.3f;
        color = (dullBlendFacor * dullColor * visibility) + reflectiveColor * visibility;
    } else {
        color = vec4(abs(ourColor) * visibility, 1.0f);
    }
    /*vec4 vClipReflection = reflectionView * vec4(interpolatedVertexObject.xy, 0.0 , 1.0);
    vec2 vDeviceReflection = vClipReflection.st / vClipReflection.q;
    vec2 vTextureReflection = vec2(0.5, 0.5) + 0.5 * vDeviceReflection;

    vec4 reflectionTextureColor = texture2D(ourTexture, vTextureReflection);

    // Framebuffer reflection can have alpha > 1
    reflectionTextureColor.a = 1.0;

    color = reflectionTextureColor;*/

}
