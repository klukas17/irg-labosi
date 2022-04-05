//
// Created by mihael on 3/23/22.
//

#ifndef IRGLAB_MESH_H
#define IRGLAB_MESH_H

#include <vector>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glad/glad.h>


class Mesh {
public:
    std::vector<glm::vec4> vertices;
    std::vector<unsigned int> faces;
    float bounding_box_min_x;
    float bounding_box_max_x;
    float bounding_box_min_y;
    float bounding_box_max_y;
    float bounding_box_min_z;
    float bounding_box_max_z;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint shader_id;
    GLint uniform_variable_location;
    glm::vec3 color;

    Mesh(std::vector<glm::vec4> v, std::vector<unsigned int> f, GLuint s, GLint u, glm::vec3 c);
    ~Mesh();
    void calculate_bounding_box();
    void render();
};


#endif //IRGLAB_MESH_H
