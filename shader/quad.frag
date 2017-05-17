#version 330 core
in vec3 ourColor;
in vec2 texCord;

out vec4 color;
uniform sampler2D ourTexture;

void main()
{
    //color = vec4(ourColor, 1.0f);
    float depthValue = texture(ourTexture, texCord).r;
    //color = vec4(vec3(depthValue), 1.0);
    color = vec4(depthValue, 0.0, 0.0f, 1.0f);
    //color =  texture(ourTexture, texCord);
    //color.a = 1;
}
