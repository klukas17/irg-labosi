//
// Created by mihael on 3/23/22.
//

#include "Object.h"
#include "Mesh.h"
#include "Transform.h"

Object::Object(Mesh* m, Transform* t, Material* mat, Texture* tex) {
    mesh = m;
    transform = t;
    material = mat;
    texture = tex;

    mesh->k_a = material->k_a;
    mesh->k_d = material->k_d;
    mesh->k_s = material->k_s;
    mesh->k_sn = material->k_sn;
    mesh->texture = texture->texture;
}
Object::~Object() {}

void Object::normalize() {
    mesh->calculate_bounding_box();
    transform->translate();
    transform->scale();
}

void Object::buffer_data() {
    mesh->buffer_data();
}