#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform int textureIndex; // New uniform to select the texture

void main()
{
    if (textureIndex == 0)
        FragColor = texture(texture1, TexCoord);
    else if (textureIndex == 1)
        FragColor = texture(texture2, TexCoord);
}
