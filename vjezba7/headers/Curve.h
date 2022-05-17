//
// Created by mihael on 05.05.22..
//

#ifndef IRGLAB_CURVE_H
#define IRGLAB_CURVE_H

#include "vector"
#include <glm/glm.hpp>
#include <glad/glad.h>

class Curve {
public:
    std::vector<glm::vec3> points;
    GLenum primitive;

    GLuint VAO;
    GLuint VBO;
    GLuint shader_id;
    GLint uniform_variable_location_color;
    glm::vec3 color;

    glm::mat4 view_matrix;
    GLint uniform_variable_location_view_matrix;
    glm::mat4 projection_matrix;
    GLint uniform_variable_location_projection_matrix;

    Curve(std::vector<glm::vec3> p, GLenum pr, GLuint s, GLint u_c, glm::vec3 c, GLint u_view_matrix, GLint u_projection_matrix);
    ~Curve();
    void buffer_data();
    void render();
};


#endif //IRGLAB_CURVE_H
