#version 460 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 color;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec3 light_location;
uniform vec3 I_a;
uniform vec3 I_d;
uniform vec3 I_s;
uniform vec3 k_a;
uniform vec3 k_d;
uniform vec4 k_s;
uniform vec3 eye;

void main()
{
    vec4 position = model_matrix * aPos;
    vec3 p = position.xyz / position.w;
    vec3 n = normalize(mat3(transpose(inverse(model_matrix))) * aNormal);
    vec3 l = normalize(light_location - p);
    vec3 v = normalize(eye - p);
    vec3 r = normalize(reflect(-l, n));

    vec3 ambient, diffuse, specular;
    ambient = I_a * k_a;
    diffuse = I_d * k_d * max(dot(l, n), 0);
    specular = I_s * k_s.xyz * pow(max(dot(v, r), 0), k_s.w);

    gl_Position = projection_matrix * view_matrix * position;
    color = vec3(ambient + diffuse + specular);
}