#version 460 core

layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

in vec3 color[];
out vec3 fcolor;

void main(void) {

    mat3 T = mat3(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz);
    if (determinant(T) > 0) {
        gl_Position = gl_in[0].gl_Position;
        fcolor = color[0];
        EmitVertex();
        gl_Position = gl_in[1].gl_Position;
        fcolor = color[0];
        EmitVertex();
        gl_Position = gl_in[2].gl_Position;
        fcolor = color[0];
        EmitVertex();
        EndPrimitive();
    }
}