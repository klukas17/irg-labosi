#version 460 core
layout (location = 0) in vec4 aPos;
uniform vec3 u_color;

out vec3 color;

void main()
{
	color = u_color;
    gl_Position = aPos;
}