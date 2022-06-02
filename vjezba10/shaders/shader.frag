#version 460 core
out vec4 FragColor;

in vec3 color;
in vec3 n;
in vec3 p;
in vec2 tex_coord;
in vec4 fragment_position_light_space;

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

uniform sampler2D tex;
uniform sampler2D shadow_map;

float shadow_calculation() {
    float bias = max(0.05 * (1.0 - dot(n, light_location)), 0.005);  ;
    vec3 projection_coordinates = fragment_position_light_space.xyz / fragment_position_light_space.w;
    projection_coordinates = projection_coordinates * 0.5 + 0.5;
    float closest_depth = texture(shadow_map, projection_coordinates.xy).r;
    float current_depth = projection_coordinates.z;
    return current_depth - bias > closest_depth ? 1 : 0;
}

void main()
{
    vec3 ambient = vec3(0, 0, 0), diffuse = vec3(0, 0, 0), specular = vec3(0, 0, 0);

    // AMBIENT COMPONENT
    ambient = I_a * k_a;

    // DIFFUSE COMPONENT
    vec3 l = normalize(light_location - p);
    float ln = dot(l, n);
    if (ln > 0)
    diffuse = I_d * k_d * ln;

    // SPECULAR COMPONENT
    vec3 v = normalize(eye - p);
    vec3 r_ = normalize(reflect(-light_location, n));
    float rv = dot(r_, v);
    if (rv > 0)
    specular = I_s * k_s.rgb * pow(rv, k_s.a);

    float shadow = shadow_calculation();

    vec3 color = ambient + (1 - shadow) * (diffuse + specular);
    FragColor = texture(tex, tex_coord) * vec4(color, 0.0);
}