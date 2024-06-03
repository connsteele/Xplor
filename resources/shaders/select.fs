#version 330 core

out vec4 FragColor;

vec3 highlight_color = vec3(1.f, 1.f, 0.0f); // RGB
float opacity = 0.35f;

void main()
{
    FragColor = vec4(highlight_color, opacity);
}