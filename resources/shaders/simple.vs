#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec2 bTexCoord;

out vec3 ourColor; // output to frag shader
out vec2 texCoords1;

uniform mat4 transform;
uniform mat4 liveTransform;

// Coordinate Spaces
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0f);
   texCoords1 = aTexCoord;
}