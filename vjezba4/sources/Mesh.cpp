//
// Created by mihael on 3/23/22.
//

#include <iostream>
#include <cfloat>
#include <vector>

#include "Mesh.h"

Mesh::Mesh(std::vector<glm::vec4> v, std::vector<unsigned int> f, GLuint s, GLint u, glm::vec3 c) {
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
    uniform_variable_location = u;
    color = c;
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

void Mesh::render() {

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glUseProgram(shader_id);
    glUniform3f(uniform_variable_location, color.x, color.y, color.z);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), &faces[0], GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}