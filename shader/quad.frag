#version 330 core
in vec3 ourColor;
in vec2 texCord;

out vec4 color;
uniform sampler2D ourTexture;

void main()
{
    //color = vec4(ourColor, 1.0f);
    color = texture(ourTexture, texCord);
}
