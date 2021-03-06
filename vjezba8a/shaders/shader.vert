#version 460 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;

uniform mat4 model_matrix;

void main()
{
    gl_Position = model_matrix * aPos;
    Normal = aNormal;
}