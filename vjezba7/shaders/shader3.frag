#version 460 core
out vec4 FragColor;

in vec3 fcolor;

void main()
{
    FragColor = vec4(fcolor, 0.0);
}