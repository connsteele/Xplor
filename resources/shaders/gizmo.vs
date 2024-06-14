#version 330 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 frag_pos; // World position
out vec3 frag_normal;

void main()
{
    frag_pos = vec3(model * vec4(vert_pos, 1.0f)); // Transform the vertex pos to world space
    frag_normal = vec3(model * vec4(vert_normal, 1.0f));

    gl_Position = projection * view * model * vec4(vert_pos, 1.0f);
}