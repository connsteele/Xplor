#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoords1;

uniform vec4 customColor;
uniform sampler2D customTexture1; // RGB
uniform sampler2D customTexture2; // Transparent


void main()
{
    vec4 color1 = texture(customTexture1, texCoords1);
    vec4 color2 = texture(customTexture2, texCoords1);

    // Output color is a weighted sum using the alpha from color 2
    FragColor = mix (
        color1,
        color2,
        color2.a
    );

}