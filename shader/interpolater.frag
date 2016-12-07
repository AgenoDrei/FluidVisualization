#version 330 core
in float density;

out vec4 color;

void main()
{
    //color = vec4(density, 0.0f, 0.0f, 1.0f);
    color = vec4(density, density, density, 1.0f);
}
