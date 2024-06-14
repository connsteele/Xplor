// Depending on the axis that is passed the gizmo should render a different color
#version 330 core

// in vec3 frag_pos;
//in vec3 frag_normal;

out vec4 final_color;

void main()
{
    final_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
