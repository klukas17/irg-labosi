#version 460 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 n;
out vec3 color;
out vec3 p;
out vec2 tex_coord;
out vec4 fragment_position_light_space;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 light_view_matrix;
uniform mat4 light_projection_matrix;

void main()
{
    vec4 tmp = model_matrix * aPos;
    gl_Position = projection_matrix * view_matrix * tmp;
    fragment_position_light_space = light_projection_matrix * light_view_matrix * tmp;
    n = normalize(mat3(transpose(inverse(model_matrix))) * aNormal);
    p = tmp.xyz / tmp.w;
    tex_coord = aTexCoord;
}