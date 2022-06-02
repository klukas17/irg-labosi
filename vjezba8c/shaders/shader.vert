#version 460 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 n;
out vec3 color;
out vec3 p;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    vec4 tmp = model_matrix * aPos;
    gl_Position = projection_matrix * view_matrix * model_matrix * aPos;
    n = normalize(mat3(transpose(inverse(model_matrix))) * aNormal);
    p = tmp.xyz / tmp.w;
}