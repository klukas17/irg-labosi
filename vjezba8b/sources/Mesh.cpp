//
// Created by mihael on 3/23/22.
//

#include <iostream>
#include <cfloat>
#include <vector>

#include "Mesh.h"

Mesh::Mesh(std::vector<glm::vec4> v, std::vector<glm::vec3> n, std::vector<unsigned int> f, Shader* shader) {
    vertices = v;
    normals = n;
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

    shader_id = shader->ID;
    uniform_variable_location_model_matrix = glGetUniformLocation(shader->ID, "model_matrix");
    uniform_variable_location_view_matrix = glGetUniformLocation(shader->ID, "view_matrix");
    uniform_variable_location_projection_matrix = glGetUniformLocation(shader->ID, "projection_matrix");
    uniform_variable_location_light_location = glGetUniformLocation(shader->ID, "light_location");
    uniform_variable_location_I_a = glGetUniformLocation(shader->ID, "I_a");
    uniform_variable_location_I_d = glGetUniformLocation(shader->ID, "I_d");
    uniform_variable_location_I_s = glGetUniformLocation(shader->ID, "I_s");
    uniform_variable_location_k_a = glGetUniformLocation(shader->ID, "k_a");
    uniform_variable_location_k_d = glGetUniformLocation(shader->ID, "k_d");
    uniform_variable_location_k_s = glGetUniformLocation(shader->ID, "k_s");
    uniform_variable_location_k_sn = glGetUniformLocation(shader->ID, "k_sn");
    uniform_variable_location_eye = glGetUniformLocation(shader->ID, "eye");
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

    std::vector<float> vertex_info;
    for (int i = 0; i < vertices.size(); i++) {
        vertex_info.push_back(vertices[i].x);
        vertex_info.push_back(vertices[i].y);
        vertex_info.push_back(vertices[i].z);
        vertex_info.push_back(vertices[i].w);
        vertex_info.push_back(normals[i].x);
        vertex_info.push_back(normals[i].y);
        vertex_info.push_back(normals[i].z);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_info.size() * sizeof(vertex_info[0]), &vertex_info[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (4 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), &faces[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Mesh::render() {
    glUseProgram(shader_id);

    glUniformMatrix4fv(uniform_variable_location_model_matrix, 1, GL_FALSE, &model_matrix[0][0]);
    glUniformMatrix4fv(uniform_variable_location_view_matrix, 1, GL_FALSE, &view_matrix[0][0]);
    glUniformMatrix4fv(uniform_variable_location_projection_matrix, 1, GL_FALSE, &projection_matrix[0][0]);

    glUniform3f(uniform_variable_location_light_location, light_location.x, light_location.y, light_location.z);
    glUniform3f(uniform_variable_location_I_a, I_a.x, I_a.y, I_a.z);
    glUniform3f(uniform_variable_location_I_d, I_d.x, I_d.y, I_d.z);
    glUniform3f(uniform_variable_location_I_s, I_s.x, I_s.y, I_s.z);
    glUniform3f(uniform_variable_location_k_a, k_a.x, k_a.y, k_a.z);
    glUniform3f(uniform_variable_location_k_d, k_d.x, k_d.y, k_d.z);
    glUniform4f(uniform_variable_location_k_s, k_s.x, k_s.y, k_s.z, k_sn);
    glUniform3f(uniform_variable_location_eye, eye.x, eye.y, eye.z);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}