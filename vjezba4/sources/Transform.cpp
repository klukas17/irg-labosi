//
// Created by mihael on 3/24/22.
//

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"

Transform::Transform(Mesh *m) {
    mesh = m;
}

Transform::~Transform() {}

void Transform::translate() {
    float x_, y_, z_;
    x_ = (mesh->bounding_box_min_x + mesh->bounding_box_max_x) / 2;
    y_ = (mesh->bounding_box_min_y + mesh->bounding_box_max_y) / 2;
    z_ = (mesh->bounding_box_min_z + mesh->bounding_box_max_z) / 2;

    glm::mat4 identity = glm::mat4(1);
    glm::mat4 translate = glm::translate(identity, glm::vec3(-x_, -y_, -z_));

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

    glm::mat4 identity = glm::mat4(1);
    glm::mat4 scale = glm::scale(identity, glm::vec3(scale_factor, scale_factor, scale_factor));

    for (int i = 0; i < mesh->vertices.size(); i++) {
        mesh->vertices[i] = scale * mesh->vertices[i];
    }
}