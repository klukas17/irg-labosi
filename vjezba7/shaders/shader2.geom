#version 460 core

layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

in vec3 color[];
out vec3 fcolor;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec3 eye;

void main(void) {

    vec4 v0, v1, v2;
    v0 = model_matrix * gl_in[0].gl_Position;
    v1 = model_matrix * gl_in[1].gl_Position;
    v2 = model_matrix * gl_in[2].gl_Position;

    vec3 normal = cross(normalize(vec3(v1) - vec3(v0)), normalize(vec3(v2) - vec3(v0)));
    vec3 c = vec3((v0 + v1 + v2) / 3);
    vec3 e = eye - c;

    if (normal.x * e.x + normal.y * e.y + normal.z * e.z > 0) {
        gl_Position = projection_matrix * view_matrix * v0;
        fcolor = color[0];
        EmitVertex();
        gl_Position = projection_matrix * view_matrix * v1;
        fcolor = color[0];
        EmitVertex();
        gl_Position = projection_matrix * view_matrix * v2;
        fcolor = color[0];
        EmitVertex();
        EndPrimitive();
    }
}