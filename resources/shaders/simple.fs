#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform vec4 customColor;
uniform sampler2D customTexture;

void main()
{
    FragColor = texture(customTexture, texCoord);
}