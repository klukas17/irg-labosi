//
// Created by mihael on 05.05.22..
//

#include "Curve.h"

Curve::Curve(std::vector<glm::vec3> p, GLenum pr, GLuint s, GLint u_c, glm::vec3 c, GLint u_view_matrix, GLint u_projection_matrix) {
    points = p;
    primitive = pr;
    shader_id = s;
    uniform_variable_location_color = u_c;
    color = c;
    uniform_variable_location_view_matrix = u_view_matrix;
    uniform_variable_location_projection_matrix = u_projection_matrix;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

Curve::~Curve() {}

void Curve::buffer_data() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]), &points[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Curve::render() {
    glUseProgram(shader_id);
    glBindVertexArray(VAO);

    glUniform3f(uniform_variable_location_color, color.x, color.y, color.z);
    glUniformMatrix4fv(uniform_variable_location_view_matrix, 1, GL_FALSE, &view_matrix[0][0]);
    glUniformMatrix4fv(uniform_variable_location_projection_matrix, 1, GL_FALSE, &projection_matrix[0][0]);

    glBindVertexArray(VAO);
    glDrawArrays(primitive, 0, points.size());

    glBindVertexArray(0);
    glUseProgram(0);
}