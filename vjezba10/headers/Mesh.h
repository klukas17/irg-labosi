//
// Created by mihael on 3/23/22.
//

#ifndef IRGLAB_MESH_H
#define IRGLAB_MESH_H

#include <vector>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Shader.h"

class Mesh {
public:
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv_coordinates;
    std::vector<unsigned int> faces;

    float bounding_box_min_x;
    float bounding_box_max_x;
    float bounding_box_min_y;
    float bounding_box_max_y;
    float bounding_box_min_z;
    float bounding_box_max_z;

    GLuint VAO;
    GLuint VBO[3];
    GLuint EBO;
    GLuint shader_id;
    GLuint shader_map_id;

    GLint uniform_variable_location_model_matrix;
    GLint uniform_variable_location_view_matrix;
    GLint uniform_variable_location_projection_matrix;
    GLint uniform_variable_location_light_view_matrix;
    GLint uniform_variable_location_light_projection_matrix;
    GLint uniform_variable_location_light_location;
    GLint uniform_variable_location_I_a;
    GLint uniform_variable_location_I_d;
    GLint uniform_variable_location_I_s;
    GLint uniform_variable_location_k_a;
    GLint uniform_variable_location_k_d;
    GLint uniform_variable_location_k_s;
    GLint uniform_variable_location_k_sn;
    GLint uniform_variable_location_eye;

    GLint uniform_variable_location_shadow_model_matrix;
    GLint uniform_variable_location_shadow_view_matrix;
    GLint uniform_variable_location_shadow_projection_matrix;

    unsigned int texture;
    unsigned int texture_shadow;

    glm::mat4 model_matrix;
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    glm::mat4 light_view_matrix;
    glm::mat4 light_projection_matrix;
    glm::vec3 light_location;
    glm::vec3 I_a;
    glm::vec3 I_d;
    glm::vec3 I_s;
    glm::vec3 k_a;
    glm::vec3 k_d;
    glm::vec3 k_s;
    float k_sn;
    glm::vec3 eye;

    Mesh(std::vector<glm::vec4> v, std::vector<glm::vec3> n, std::vector<glm::vec2> uv, std::vector<unsigned int> f, Shader* shader, Shader* shader_map);
    ~Mesh();
    void calculate_bounding_box();
    void buffer_data();
    void render(int pass);
};


#endif //IRGLAB_MESH_H
