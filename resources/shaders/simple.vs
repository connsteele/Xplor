#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec2 bTexCoord;

out vec3 ourColor; // output to frag shader
out vec2 texCoords1;
out vec2 texCoords2;

uniform mat4 transform;
uniform mat4 liveTransform;

void main()
{
   gl_Position = liveTransform * vec4(aPos, 1.0);
   ourColor = aColor;
   texCoords1 = aTexCoord;
   texCoords2 = bTexCoord;
}