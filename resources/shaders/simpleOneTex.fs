#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoords1;

uniform vec4 customColor;
uniform sampler2D customTexture1; // RGB


void main()
{
    vec4 color1 = texture(customTexture1, texCoords1);

    FragColor = color1;
}