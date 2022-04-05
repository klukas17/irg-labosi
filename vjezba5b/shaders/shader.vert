#version 330 core
layout (location = 0) in vec4 aPos;
uniform vec3 u_color;

out vec3 color;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	color = u_color;
    gl_Position = projection_matrix * view_matrix * model_matrix * aPos;
}