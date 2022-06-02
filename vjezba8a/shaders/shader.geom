#version 460 core

layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

in vec3[] Normal;
out vec3 normal;
out vec3 color;

uniform vec3 light_location;
uniform vec3 I_a;
uniform vec3 I_d;
uniform vec3 I_s;
uniform vec3 k_a;
uniform vec3 k_d;
uniform vec4 k_s;
uniform vec3 eye;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
    vec4 v0, v1, v2;
    v0 = gl_in[0].gl_Position;
    v1 = gl_in[1].gl_Position;
    v2 = gl_in[2].gl_Position;

    vec3 n = normalize(cross(normalize(v1.xyz/v1.w - v0.xyz/v0.w), normalize(v2.xyz/v2.w - v0.xyz/v0.w)));
    vec3 p = (v0.xyz/v0.w + v1.xyz/v0.w + v2.xyz/v2.w) / 3;
    vec3 l = normalize(light_location - p);
    vec3 v = normalize(eye - p);
    vec3 r = normalize(reflect(-l, n));

    vec3 ambient, diffuse, specular;
    ambient = I_a * k_a;
    diffuse = I_d * k_d * max(dot(l, n), 0);
    specular = I_s * k_s.xyz * pow(max(dot(v, r), 0), k_s.w);
    vec3 c = ambient + diffuse + specular;

    gl_Position = projection_matrix * view_matrix * v0;
    color = c;
    normal = Normal[0];
    EmitVertex();
    gl_Position = projection_matrix * view_matrix * v1;
    color = c;
    normal = Normal[1];
    EmitVertex();
    gl_Position = projection_matrix * view_matrix * v2;
    color = c;
    normal = Normal[2];
    EmitVertex();
    EndPrimitive();
}