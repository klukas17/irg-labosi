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

void main() {

    float a, b, c, d;
    vec4 v0, v1, v2;
    v0 = model_matrix * gl_in[0].gl_Position;
    v1 = model_matrix * gl_in[1].gl_Position;
    v2 = model_matrix * gl_in[2].gl_Position;

    vec3 v_0, v_1, v_2;
    v_0 = vec3(v0.x / v0.w, v0.y / v0.w, v0.z / v0.w);
    v_1 = vec3(v1.x / v1.w, v1.y / v1.w, v1.z / v1.w);
    v_2 = vec3(v2.x / v2.w, v2.y / v2.w, v2.z / v2.w);

    vec3 normal = cross(normalize(v_1 - v_0), normalize(v_2 - v_0));
    float normal_length = length(normal);
    normal.x = normal.x / normal_length;
    normal.y = normal.y / normal_length;
    normal.z = normal.z / normal_length;

    a = normal.x;
    b = normal.y;
    c = normal.z;
    d = -a * v_0.x - b * v_0.y - c * v_0.z;

    if (a * eye.x + b * eye.y + c * eye.z + d > 0) {
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