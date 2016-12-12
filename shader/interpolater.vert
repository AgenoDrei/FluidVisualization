#version 330 core

layout (location = 0) in vec3 position;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int size;
uniform float maxDistance;
uniform int width;
uniform sampler1D values;

out float density;

void main() {
    float currentDistance = 999999.0f;
    int nearestNeighbor = -1;

    int i = 0;
    for(i = 0; i < size; i++) {
        vec3 localPosition = texture(values, i).xyz;
        float tmpDistance = distance(localPosition, position);
        if(tmpDistance < currentDistance) {
            currentDistance = tmpDistance;
            nearestNeighbor = i;
        }
    }

    //if(currentDistance <= maxDistance && nearestNeighbor != -1) {
    if(currentDistance <= maxDistance) {
        density = texture(values, nearestNeighbor).w;
    } else {
        density = 0.0f;
    }
    gl_Position = projection * view * model * vec4(position, 1.0f);

}
