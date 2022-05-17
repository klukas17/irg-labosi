//
// Created by mihael on 3/23/22.
//

#include <iostream>
#include <cfloat>
#include <vector>

#include "Mesh.h"

Mesh::Mesh(std::vector<glm::vec4> v, std::vector<unsigned int> f, GLuint s, GLint u_c, glm::vec3 c, GLint u_model_matrix, GLint u_view_matrix, GLint u_projection_matrix, GLint u_eye) {
    vertices = v;
    faces = f;
    bounding_box_min_x = 0;
    bounding_box_max_x = 0;
    bounding_box_min_y = 0;
    bounding_box_max_y = 0;
    bounding_box_min_z = 0;
    bounding_box_max_z = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    shader_id = s;
    uniform_variable_location_color = u_c;
    color = c;
    uniform_variable_location_model_matrix = u_model_matrix;
    uniform_variable_location_view_matrix = u_view_matrix;
    uniform_variable_location_projection_matrix = u_projection_matrix;
    uniform_variable_location_eye = u_eye;
}

Mesh::~Mesh() {}

void Mesh::calculate_bounding_box() {

    bounding_box_min_x = bounding_box_min_y = bounding_box_min_z = DBL_MAX;
    bounding_box_max_x = bounding_box_max_y = bounding_box_max_z = DBL_MIN;

    for (auto v : vertices) {
        if (v.x < bounding_box_min_x) bounding_box_min_x = v.x;
        if (v.x > bounding_box_max_x) bounding_box_max_x = v.x;
        if (v.y < bounding_box_min_y) bounding_box_min_y = v.y;
        if (v.y > bounding_box_max_y) bounding_box_max_y = v.y;
        if (v.z < bounding_box_min_z) bounding_box_min_z = v.z;
        if (v.z > bounding_box_max_z) bounding_box_max_z = v.z;
    }
}

void Mesh::buffer_data() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), &faces[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Mesh::render() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUseProgram(shader_id);

    glUniform3f(uniform_variable_location_color, color.x, color.y, color.z);
    glUniformMatrix4fv(uniform_variable_location_model_matrix, 1, GL_FALSE, &model_matrix[0][0]);
    glUniformMatrix4fv(uniform_variable_location_view_matrix, 1, GL_FALSE, &view_matrix[0][0]);
    glUniformMatrix4fv(uniform_variable_location_projection_matrix, 1, GL_FALSE, &projection_matrix[0][0]);
    if (uniform_variable_location_eye) glUniform3f(uniform_variable_location_eye, eye.x, eye.y, eye.z);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}