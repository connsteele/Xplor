#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform vec4 customColor;
uniform sampler2D customTexture1; // RGB
uniform sampler2D customTexture2; // Transparent


void main()
{
    // FragColor = texture(customTexture1, texCoord);
    //FragColor = texture(customTexture1, texCoord) * vec4(ourColor, 1.0);

    // Don't render transparent areas
    vec4 color1 = texture(customTexture1, texCoord);
    vec4 color2 = texture(customTexture2, texCoord);

    // Output color is a weighted sum using the alpha from color 2
    FragColor = mix (
        color1,
        color2,
        color2.a
    );

}