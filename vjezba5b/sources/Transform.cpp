//
// Created by mihael on 3/24/22.
//

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Transform.h"
#include "Math.h"

Transform::Transform(Mesh* m) {
    mesh = m;
    position = glm::vec4(0);
    up_vector = glm::vec3(0, 1, 0);
    angle = 0;
    size = 1;
}

Transform::~Transform() {}

void Transform::translate() {
    float x_, y_, z_;
    x_ = (mesh->bounding_box_min_x + mesh->bounding_box_max_x) / 2;
    y_ = (mesh->bounding_box_min_y + mesh->bounding_box_max_y) / 2;
    z_ = (mesh->bounding_box_min_z + mesh->bounding_box_max_z) / 2;

    glm::mat4 translate = Math::translate(glm::vec3(-x_, -y_, -z_));

    for (int i = 0; i < mesh->vertices.size(); i++) {
        mesh->vertices[i] = translate * mesh->vertices[i];
    }
}

void Transform::scale() {
    float x_, y_, z_;
    x_ = mesh->bounding_box_max_x - mesh->bounding_box_min_x;
    y_ = mesh->bounding_box_max_y - mesh->bounding_box_min_y;
    z_ = mesh->bounding_box_max_z - mesh->bounding_box_min_z;

    float M = x_;
    if (y_ > M) M = y_;
    if (z_ > M) M = z_;

    float scale_factor = 2 / M;

    glm::mat4 scale = Math::scale(glm::vec3(scale_factor, scale_factor, scale_factor));

    for (int i = 0; i < mesh->vertices.size(); i++) {
        mesh->vertices[i] = scale * mesh->vertices[i];
    }
}

glm::mat4 Transform::getModelMatrix() {
    glm::mat4 identity = glm::mat4(1);
    glm::mat4 scale_matrix = Math::scale(glm::vec3(size, size, size));
    glm::mat4 rotation_matrix = glm::rotate(identity, angle, glm::vec3(up_vector.x, up_vector.y, -up_vector.z));
    glm::mat4 position_matrix = Math::translate(position);

    float xy_angle, xz_angle;
    xy_angle = up_vector.x / glm::length(up_vector);
    xz_angle = up_vector.z / glm::length(up_vector);

    glm::mat4 orientation_matrix = glm::inverse(glm::rotate(identity, glm::asin(xy_angle), glm::vec3(0, 0, 1))) *
            glm::inverse(glm::rotate(identity, glm::asin(xz_angle), glm::vec3(1, 0, 0)));

    return position_matrix * rotation_matrix * orientation_matrix * scale_matrix;
}

void Transform::setSize(float s) {
    size = s;
}

void Transform::setOrientation(glm::vec3 up_v, float a) {
    up_vector = up_v / glm::length(up_v);
    angle = a;
}

void Transform::setPosition(glm::vec3 p) {
    position = p;
}