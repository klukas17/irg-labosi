#version 330 core
layout (location = 0) in vec3 aPos;
uniform vec3 u_color;

out vec3 color;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	color = u_color;
    gl_Position = projection_matrix * view_matrix * vec4(aPos, 1.0);
}