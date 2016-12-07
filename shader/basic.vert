#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in float density;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    //ourColor = vec3(0.5f + density*1000);
    if(density == 10) { //ToDo fix
        ourColor = vec3(0.0f, 0.0f, 0.0f);
    } else {
        ourColor = vec3(0.5f + density*1000);
    }
}
